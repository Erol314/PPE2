#include "Object.h"

Object::Object(POINT startPos, int speedX, int speedY){
	radius = 50;
	pos = { startPos.x - radius / 2, startPos.y - radius / 2 };
	dx = speedX;
	dy = speedY;
	color = RGB(57, 134, 202);
}

Object::Object(POINT startPos, int speedX, int speedY, unsigned int ballRadius, COLORREF col){
	radius = ballRadius;
	pos = startPos;
	dx = speedX;
	dy = speedY;
	color = col;
}

void Object::Update(RECT* rec) {
	pos.x += dx;
	pos.y += dy;

	if (pos.x < 0){
		pos.x = 0;
		dx = -dx;
	} else if (pos.x + radius > rec->right){
		pos.x = rec->right - radius;
		dx = -dx;
	}

	if (pos.y < 0){
		pos.y = 0;
		dy = -dy;
	} else if (pos.y + radius > rec->bottom){
		pos.y = rec->bottom - radius;
		dy = -dy;
	}
}

void Object::Draw(HDC hdc, RECT* rec){
	HBRUSH hBrush = CreateSolidBrush(color);
	SelectObject(hdc, hBrush);
	Update(rec);

	Ellipse(hdc, pos.x, pos.y, pos.x + radius, pos.y + radius);
	DeleteObject(hBrush);
}

bool Object::CollideWith(const Object & a, POINT * collissionPoint = nullptr) {
	if (distance(*this, a) < (this->radius / 2 + a.radius / 2)) {
		if (collissionPoint) {
			POINT ret = { (pos.x + a.pos.x) / 2, (pos.y + a.pos.y) / 2 };
			*collissionPoint = ret;
		}
		return true;
	}
	return false;
}

float distance(const Object & a, const Object & b) {
	POINT diff = { a.pos.x - b.pos.x, a.pos.y - b.pos.y };
	return (float)sqrt(diff.x * diff.x + diff.y * diff.y);
}

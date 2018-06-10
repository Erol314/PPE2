#pragma once

#include <vector>
#include <windows.h>

class Object {
public:
	POINT pos;
	int radius;
	int dx, dy;
	COLORREF color;

	Object(POINT startPos, int speedX, int speedY);
	Object(POINT startPos, int speedX, int speedY, unsigned int ballRadius, COLORREF col);
	void Update(RECT* rec);
	void Draw(HDC hdc, RECT* rec);
	bool CollideWith(const Object & a, POINT * collissionPoint);

	friend float distance(const Object & a, const Object & b);
};
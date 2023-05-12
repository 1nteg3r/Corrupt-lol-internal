#pragma once

#include <cmath>

struct Vector2
{
	Vector2() {};

	Vector2(float _x, float _y) {
		x = _x;
		y = _y;
	}

	float x;
	float y;

	float length() {
		return sqrt(x * x + y * y);
	}

	float distance(const Vector2& o) {
		return sqrt(pow(x - o.x, 2) + pow(y - o.y, 2));
	}

	Vector2 vscale(const Vector2& s) {
		return Vector2(x * s.x, y * s.y);
	}

	Vector2 scale(float s) {
		return Vector2(x * s, y * s);
	}

	Vector2 normalize() {
		float l = length();
		return Vector2(x / l, y / l);
	}

	Vector2 add(const Vector2& o) {
		return Vector2(x + o.x, y + o.y);
	}

	Vector2 sub(const Vector2& o) {
		return Vector2(x - o.x, y - o.y);
	}

	Vector2 clone() {
		return Vector2(x, y);
	}
};
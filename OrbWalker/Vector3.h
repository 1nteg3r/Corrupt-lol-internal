#pragma once

#include <cmath>

struct Vector3
{
	Vector3() {};
	Vector3(float _x, float _y, float _z) {
		x = _x;
		y = _y;
		z = _z;
	}

	float x;
	float y;
	float z;

	float length() {
		return sqrt(x * x + y * y + z * z);
	}

	float distance(const Vector3& o) {
		return sqrt(pow(x - o.x, 2) + pow(y - o.y, 2) + pow(z - o.z, 2));
	}

	Vector3 rotate_x(float angle) {
		return Vector3(
			x,
			y * cos(angle) - z * sin(angle),
			y * sin(angle) + z * cos(angle)
		);
	}

	Vector3 rotate_y(float angle) {
		return Vector3(
			x * cos(angle) + z * sin(angle),
			y,
			-x * sin(angle) + z * cos(angle)
		);
	}

	Vector3 rotate_z(float angle) {
		return Vector3(
			x * cos(angle) - y * sin(angle),
			x * sin(angle) + y * cos(angle),
			z
		);
	}

	Vector3 vscale(const Vector3& s) {
		return Vector3(x * s.x, y * s.y, z * s.z);
	}

	Vector3 scale(float s) {
		return Vector3(x * s, y * s, z * s);
	}

	Vector3 normalize() {
		float l = length();
		return Vector3(x / l, y / l, z / l);
	}

	Vector3 add(const Vector3& o) {
		return Vector3(x + o.x, y + o.y, z + o.z);
	}

	Vector3 sub(const Vector3& o) {
		return Vector3(x - o.x, y - o.y, z - o.z);
	}

	Vector3 clone() {
		return Vector3(x, y, z);
	}
};
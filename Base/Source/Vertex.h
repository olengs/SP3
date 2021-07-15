#ifndef VERTEX_H
#define VERTEX_H

#include "Vector3.h"

struct Position
{
	float x, y, z;
	Position(float x = 0, float y = 0, float z = 0) {Set(x, y, z);}
	void Set(float x, float y, float z) {this->x = x; this->y = y; this->z = z;}
	Vector3 tovec3() {
		return Vector3(this->x, this->y, this->z);
	}
};

struct Color
{
	float r, g, b;
	Color(float r = 1, float g = 1, float b = 1) {Set(r, g, b);}
	void Set(float r, float g, float b) {this->r = r; this->g = g; this->b = b;}
	Vector3 tovec3() {
		return Vector3(this->r, this->g, this->b);
	}
};

struct TexCoord
{
	float u, v;
	TexCoord(float u = 0, float v = 0) {Set(u, v);}
	void Set(float u, float v) {this->u = u; this->v = v;}
};

struct Vertex 
{
	Position pos;
	Color color;
	Vector3 normal;
	TexCoord texCoord;
	Vertex(){}
};

struct HSV
{
	float Hue;
	float Sat;
	float Val;
	void Set(float a, float b, float c) {
		this->Hue = a; this->Sat = b; this->Val = c;
	}

	HSV(float hue = 0, float sat = 1, float val = 1) {
		Set(hue, sat, val);
	}

	Color to_color() {
		float C = this->Val * this->Sat;
		float Hprime = this->Hue / 60.f;
		float X = C * (1 - abs(fmod(Hprime, 2.0f) - 1));
		float m = this->Val - C;
		Color rgb;
		switch ((int)Hprime) {
		case 0:
			rgb.Set(C, X, 0);
			break;
		case 1:
			rgb.Set(X, C, 0);
			break;
		case 2:
			rgb.Set(0, C, X);
			break;
		case 3:
			rgb.Set(0, X, C);
			break;
		case 4:
			rgb.Set(X, 0, C);
			break;
		case 5:
		case 6:
			rgb.Set(C, 0, X);
			break;
		default:
			break;
		}

		return Color(rgb.r + m, rgb.g + m, rgb.b + m);
	}
};

#endif
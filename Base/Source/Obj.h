#pragma once
#include <TRS.h>
#include "cornercoord.h"
#include "Mesh.h"
#include "LoadHmap.h"
#include "Camera3.h"
#include "Vector3.h"
#include "Mtx44.h"
#include "Application.h"

struct Obj {
	enum OBJ_TYPE {
		O_PLAYER,
		O_ENEMY,
		O_TERRAIN,
		O_ITEMS,
		O_WEAPON,
		O_OBSTACLES,
		O_OTHER,
	};

	TRS trs;
	cornercoord dimensions;
	Mesh* mesh;
	OBJ_TYPE type;
	Vector3 initialFront;
	bool toDestroy;
	Vector3 vec3Front;

	Obj& setToFloor(std::vector<unsigned char>& heightMap, Obj* terrain)
	{
 		this->trs.translate.y = ReadHeightMap(heightMap, this->trs, terrain->trs)
			 - this->dimensions.getminY() * this->trs.Scale.y;
		return *this;
	}

	Obj* operator=(Obj* b) {
		Obj* temp = new Obj();
		temp->trs = b->trs;
		temp->dimensions = b->dimensions;
		temp->mesh = new Mesh(b->mesh,"copymesh");
		temp->initialFront = b->initialFront;
		temp->type = b->type;
		return temp;
	}
	Obj() {
		mesh = nullptr;
		type = Obj::O_OTHER;
		toDestroy = false;
	}
	Obj(Obj* b) {
		this->trs = b->trs;
		this->dimensions = b->dimensions;
		this->mesh = new Mesh(b->mesh, "copymesh");
		this->initialFront = b->initialFront;
		this->type = b->type;
		toDestroy = false;
	}
	bool isOutOfBounds(Obj* terrain) {
		return trs.translate.x < -terrain->trs.Scale.x / 2 || trs.translate.x > terrain->trs.Scale.x / 2
			|| trs.translate.z < -terrain->trs.Scale.z / 2 || trs.translate.z > terrain->trs.Scale.z / 2;
	}
};
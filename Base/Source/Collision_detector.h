#pragma once
#include <Vector3.h>
#include <TRS.h>
#include "cornercoord.h"
#include <vector>
#include "Obj.h"

class collision_detection {
public:
	static bool collision_detector(TRS obj1, cornercoord pos_obj1, TRS obj2, cornercoord pos_obj2); // aabb without rotation
	static bool collision_detector(TRS obj1, cornercoord dimensions_obj1, TRS obj2, float radius); // aabb rectangle vs circle
	static bool collision_detector(Obj* obj1, Obj* obj2);
private:
	static void setcoords(Vector3* coords, TRS obj, cornercoord coordinates);
	static void setnormals(std::vector<Vector3>& normals, TRS obj, cornercoord coordinates);
	static void getprojection(Vector3* coords, Vector3& normal, float* proj);
	static void getminmax(float& minA, float& maxA, float& minB, float& maxB, float* prj1, float* prj2);
	static bool checkRad(Obj* obj1, Obj* obj2);
};
#include "Collision_detector.h"
#include <iostream>
#include <Mtx44.h>
#include <TemplatedFunc.h>

bool collision_detection::collision_detector(TRS obj1, cornercoord dimensions_obj1, TRS obj2, cornercoord dimensions_obj2)
{
	return obj1.translate.x + dimensions_obj1.getmaxX() * obj1.Scale.x >= obj2.translate.x + dimensions_obj2.getminX() * obj2.Scale.x
		&& obj1.translate.x + dimensions_obj1.getminX() * obj1.Scale.x <= obj2.translate.x + dimensions_obj2.getmaxX() * obj2.Scale.x
		&& obj1.translate.z + dimensions_obj1.getmaxZ() * obj1.Scale.z >= obj2.translate.z + dimensions_obj2.getminZ() * obj2.Scale.z
		&& obj1.translate.z + dimensions_obj1.getminZ() * obj1.Scale.z <= obj2.translate.z + dimensions_obj2.getmaxZ() * obj2.Scale.z;
}

bool collision_detection::collision_detector(TRS obj1, cornercoord dimension_obj1, TRS obj2, float radius)
{
	float x = Math::Max(obj1.translate.x + dimension_obj1.getminX() * obj1.Scale.x
		, Math::Min(obj2.translate.x, obj1.translate.x + dimension_obj1.getmaxX() * obj1.Scale.x));
	float z = Math::Max(obj1.translate.z + dimension_obj1.getminZ() * obj1.Scale.z
		, Math::Min(obj2.translate.z, obj1.translate.z + dimension_obj1.getmaxZ()));

	float shortestdist = sqrt((x - obj2.translate.x) * (x - obj2.translate.x) + (z - obj2.translate.z) * (z - obj2.translate.z));
	return shortestdist < radius * obj2.Scale.x;
}

bool collision_detection::collision_detector(Obj* obj1, Obj* obj2)
{
	if (!checkRad(obj1, obj2))
		return false;
	Vector3 obj1_coords[8], obj2_coords[8];
	std::vector<Vector3> normals;
	normals.reserve(4);
	float obj1_prj[8], obj2_prj[8];
	float obj1_min, obj1_max, obj2_min, obj2_max;
	setcoords(obj1_coords, obj1->trs, obj1->dimensions);
	setcoords(obj2_coords, obj2->trs, obj2->dimensions);
	setnormals(normals, obj1->trs, obj1->dimensions);
	setnormals(normals, obj2->trs, obj2->dimensions);
	int num_normals = normals.size();
	for (int i = 0; i < num_normals; ++i) {
		getprojection(obj1_coords, normals[i], obj1_prj);
		getprojection(obj2_coords, normals[i], obj2_prj);
		getminmax(obj1_min, obj1_max, obj2_min, obj2_max, obj1_prj, obj2_prj);
		if (obj1_max < obj2_min || obj2_max < obj1_min) {
			return false;
		}
	}
	return true;
}

void collision_detection::setcoords(Vector3* coords, TRS obj, cornercoord coordinates)
{
	Mtx44 rotation;
	Mtx44 x, y, z;
	x.SetToRotation(obj.rotateAngles.x,1,0,0);
	y.SetToRotation(obj.rotateAngles.y,0,1,0);
	z.SetToRotation(obj.rotateAngles.z,0,0,1);
	rotation = z * y * x;
	coords[0].Set(coordinates.getmaxX(), coordinates.getminY(), coordinates.getmaxZ());
	coords[1].Set(coordinates.getminX(), coordinates.getminY(), coordinates.getmaxZ());
	coords[2].Set(coordinates.getmaxX(), coordinates.getminY(), coordinates.getminZ());
	coords[3].Set(coordinates.getminX(), coordinates.getminY(), coordinates.getminZ());
	coords[4].Set(coordinates.getmaxX(), coordinates.getmaxY(), coordinates.getmaxZ());
	coords[5].Set(coordinates.getminX(), coordinates.getmaxY(), coordinates.getmaxZ());
	coords[6].Set(coordinates.getmaxX(), coordinates.getmaxY(), coordinates.getminZ());
	coords[7].Set(coordinates.getminX(), coordinates.getmaxY(), coordinates.getminZ());
	for (int i = 0; i < 8; ++i) {
		coords[i].x *= obj.Scale.x;
		coords[i].y *= obj.Scale.y;
		coords[i].z *= obj.Scale.z;
		coords[i] = rotation * coords[i];
		coords[i] += obj.translate;
	}
}

void collision_detection::setnormals(std::vector<Vector3>& normals, TRS obj, cornercoord coordinates)
{
	Vector3 temp1, temp2, temp3;
	Mtx44 rotation;
	Mtx44 x, y, z;
	x.SetToRotation(obj.rotateAngles.x, 1, 0, 0);
	y.SetToRotation(obj.rotateAngles.y, 0, 1, 0);
	z.SetToRotation(obj.rotateAngles.z, 0, 0, 1);
	rotation = z * y * x;
	temp1.Set(coordinates.getmaxX(), 0, 0);
	temp2.Set(0, 0, coordinates.getmaxZ());
	temp3.Set(0, coordinates.getmaxY(), 0);
	temp1 = rotation * temp1;
	temp2 = rotation * temp2;
	normals.push_back(temp1);
	normals.push_back(temp2);
	normals.push_back(temp3);
}

void collision_detection::getprojection(Vector3* coords, Vector3& normal, float* proj)
{
	for (int i = 0; i < 8; ++i) {
		proj[i] = coords[i].Dot(normal);
	}
}

void collision_detection::getminmax(float& minA, float& maxA, float& minB, float& maxB, float* prj1, float* prj2)
{
	minA = prj1[0]; maxA = prj1[0]; minB = prj2[0]; maxB = prj2[0];
	for (int i = 0; i < 8; ++i) {
		minA = Math::Min(minA, prj1[i]);
		maxA = Math::Max(maxA, prj1[i]);
		minB = Math::Min(minB, prj2[i]);
		maxB = Math::Max(maxB, prj2[i]);
	}
}

bool collision_detection::checkRad(Obj* obj1, Obj* obj2)
{
	Vector3 p1 = obj1->trs.translate, p2 = obj2->trs.translate;
	p1.y = p2.y = 0;
	float displacement = (p1 - p2).LengthSquared();
	float r1 = Algorithm::Math::Max(
		Algorithm::Math::Max(obj1->dimensions.getmaxX(), abs(obj1->dimensions.getminX())) * obj1->trs.Scale.x,  // x value
		Algorithm::Math::Max(obj1->dimensions.getmaxZ(), abs(obj1->dimensions.getminZ())) * obj1->trs.Scale.z
	);
	float r2 = Algorithm::Math::Max(
		Algorithm::Math::Max(obj2->dimensions.getmaxX(), abs(obj2->dimensions.getminX())) * obj2->trs.Scale.x,  // x value
		Algorithm::Math::Max(obj2->dimensions.getmaxZ(), abs(obj2->dimensions.getminZ())) * obj2->trs.Scale.z
	);
	float radiusSquared = (r1 + r2) * (r1 + r2);
	return displacement < radiusSquared ? true : false;
}

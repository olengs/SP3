#include "raytracing.h"

//cross product to get normal of 3 points
Vector3 getplanenormal(Vector3 a, Vector3 b, Vector3 c)
{
	Vector3 ab, ac;
	ab = b - a;
	ac = c - a;
	return ab.Cross(ac);
}

//by line intersect point formula
Vector3 findintersectpoint(Vector3 n, Vector3 linepos, Vector3 linedir, Vector3 p_naught)
{
	float RHS = p_naught.Dot(n);
	float lambda_coeff = linedir.Dot(n);
	RHS -= linepos.Dot(n);
	float lambda = RHS / lambda_coeff;
	return linepos + (linedir * lambda);
}

bool intersectwithtriangle(Vector3 a, Vector3 b, Vector3 c, Vector3 intersect_point)
{
	Vector3 normals[3];
	Vector3 points[3] = {a,b,c};
	//loop number of times as normal
	for (int i = 0; i < 3; ++i) {
		//if there is at least 1 separating axis return false
		if (!Check_point_against_normal(points, intersect_point, normals[i])) {
			return false;
		}
	}
	return true;
}

bool Check_point_against_normal(Vector3* points, Vector3 intersect_point, Vector3 normal)
{
	//loop num of points to find all the projs
	float projs[3], intersect_proj;
	for (int i = 0; i < 3; ++i) {
		projs[i] = points[i].Dot(normal);
	}
	intersect_proj = intersect_point.Dot(normal);
	//check if proj of intersect in colliding in the normal
	float tri_min = Math::Min(Math::Min(projs[0], projs[1]), projs[2]);
	float tri_max = Math::Max(Math::Max(projs[0], projs[1]), projs[2]);
	if (intersect_proj < tri_min || intersect_proj > tri_max)
		return false;
	return true;
}
#pragma once

#include <Vector3.h>
#include "Obj.h"

Vector3 getplanenormal(Vector3 a, Vector3 b, Vector3 c);

Vector3 findintersectpoint(Vector3 n, Vector3 linepos, Vector3 linedir, Vector3 p_naught);

bool intersectwithtriangle(Vector3 a, Vector3 b, Vector3 c, Vector3 intersect_point);

bool Check_point_against_normal(Vector3* points, Vector3 intersect_point, Vector3 normal);
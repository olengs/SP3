#ifndef TRS_H
#define TRS_H
#include "Vector3.h"
/***
@brief struct TRS.
Translate -> position of object.
rotateAngles -> degree of rotation in (x,y,z) (axis)
Scale -> scale of object
*/
struct TRS {
	Vector3 translate;
	Vector3 rotateAngles;
	Vector3 Scale = Vector3(1, 1, 1);
	TRS operator+(TRS& rValue);
};
#endif // !TRS_H

#pragma once
#include "Weapon.h"

class Taser : public Weapon
{
public:
	Taser();
	virtual ~Taser();
	void Init();
};
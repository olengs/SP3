#include "SceneBasic.h"
#include "Weapon/Sniper.h"
#include "Weapon/Shotgun.h"
#include "Weapon/Pistol.h"
#include "Weapon/Taser.h"

void SceneBasic::InitWeapons()
{
	Sniper* sniper = new Sniper;
	sniper->Init();
	sniper->SetType(Weapon::SNIPER);
	player->SetWeapon(Weapon::SNIPER, sniper);

	Shotgun* shotgun = new Shotgun;
	shotgun->Init();
	shotgun->SetType(Weapon::SHOTGUN);
	player->SetWeapon(Weapon::SHOTGUN, shotgun);

	Pistol* pistol = new Pistol;
	pistol->Init();
	pistol->SetType(Weapon::PISTOL);
	player->SetWeapon(Weapon::PISTOL, pistol);

	Taser* taser = new Taser;
	taser->Init();
	taser->SetType(Weapon::TASER);
	player->SetWeapon(Weapon::TASER, taser);
}
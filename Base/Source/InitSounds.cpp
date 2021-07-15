#include "SceneBasic.h"

void SceneBasic::InitSounds()
{
	cSoundController = CSoundController::GetInstance();
	cSoundController->LoadSound("Sounds//AWP//awp1.wav", 1, true);								//Sniper shot
	cSoundController->LoadSound("Sounds//Pistol//pistol_fire.wav", 2, true);					//Pistol shot
	cSoundController->LoadSound("Sounds//Shotgun//shotgun_fire_1.wav", 3, true);				//Shotgun shot
	cSoundController->LoadSound("Sounds//AWP//awp_deploy.wav", 4, true);						//Taking out sniper
	cSoundController->LoadSound("Sounds//Pistol//pistol_deploy_1.wav", 5, true);				//Taking out pistol
	cSoundController->LoadSound("Sounds//Shotgun//shotgun_deploy_1.wav", 6, true);				//Taking out shotgun
	cSoundController->LoadSound("Sounds//AWP//awp_bolt.wav", 7, true);							//Sniper empty clip
	cSoundController->LoadSound("Sounds//Pistol//pistol_slideback_1.wav", 8, true);				//Pistol empty clip
	cSoundController->LoadSound("Sounds//Shotgun//shotgun_Empty.wav", 9, true);					//shotgun empty clip
	cSoundController->LoadSound("Sounds//Taser//taser_shoot.wav", 10, true);					//taser shot
	cSoundController->LoadSound("Sounds//Pistol//pistol_clip_in_1.wav", 11, true);				//pistol reload
	cSoundController->LoadSound("Sounds//Shotgun//shotgun_pump_1.wav", 12, true);				//shotgun reload
	cSoundController->LoadSound("Sounds//AWP//awp_clipin.wav", 13, true);						//sniper reload
//	cSoundController->LoadSound("Sounds//Taser//taser_shoot.wav", 14, true);					//taser reload
	cSoundController->LoadSound("Sounds//AWP//zoom.wav", 15, true);								//sniper zoom

	cSoundController->LoadSound("Sounds//Player//walk//playerwalk.wav", 16, true);				//player walk
	cSoundController->LoadSound("Sounds//Player//walk//playerland.wav", 17, true);				//player land
	cSoundController->LoadSound("Sounds//Player//pickup.wav", 18, true);						//pickup

	cSoundController->LoadSound("Sounds//NPC//CiviMumble.wav", 101, true, false, CSoundInfo::SOUNDTYPE::_3D,	//mumble
		irrklang::vec3df(0,0,0));	
	cSoundController->LoadSound("Sounds//NPC//Gasp.wav", 102, true, false, CSoundInfo::SOUNDTYPE::_3D,			//gasp
		irrklang::vec3df(0,0,0));
	cSoundController->LoadSound("Sounds//NPC//Siren.wav", 103, true, false, CSoundInfo::SOUNDTYPE::_3D,			//siren
		irrklang::vec3df(0, 0, 0));
	cSoundController->LoadSound("Sounds//NPC//Groan.wav", 104, true, false, CSoundInfo::SOUNDTYPE::_3D,			//police groan
		irrklang::vec3df(0, 0, 0));
	cSoundController->LoadSound("Sounds//NPC//Hey2.wav", 105, true, false, CSoundInfo::SOUNDTYPE::_3D,			//police saying 'hey'
		irrklang::vec3df(0, 0, 0));
	cSoundController->LoadSound("Sounds//NPC//Huh.wav", 106, true, false, CSoundInfo::SOUNDTYPE::_3D,			//police saying 'huh?'
		irrklang::vec3df(0, 0, 0));
}

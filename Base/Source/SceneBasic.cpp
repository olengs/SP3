#include "SceneBasic.h"
#include "GL\glew.h"
#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include "LoadTexture.h"
#include "Player.h"
#include <sstream>
#include "Camera2.h"
#include "Weapon/Sniper.h"
#include "Weapon/Shotgun.h"
#include "Weapon/Pistol.h"
#include "Weapon/Taser.h"
#include "SceneManager/SceneManager.h"
#include "Global_Constants/GAME_STATES.h"

SceneBasic::SceneBasic():
	NUM_OF_LIGHTS(16),
	renderEnemyModels(false)
{
	for (int i = 0; i < NUM_GEOMETRY; ++i)
		meshList[i] = NULL;
}

SceneBasic::~SceneBasic()
{
	if (player)
	{
		delete player;
		player = NULL;
	}

	if (cSoundController)
	{
		cSoundController = NULL;
	}
}

void SceneBasic::Init()
{
	// Background color
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	m_programID = LoadShaders("Shader//Shadow.vertexshader", "Shader//Shadow.fragmentshader");

	m_gPassShaderID = LoadShaders("Shader//GPass.vertexshader", "Shader//GPass.fragmentshader");

	InitShaderUniforms();
	InitShadowShaderUniforms();
	InitLightShaderUniforms();

	// Use our shader
	glUseProgram(m_programID);

	InitLightSettings();

	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glUniform1f(m_parameters[U_ISMVP], 1.f);

	//FOG
	Color fogcolor = Color(0.5, 0.5, 0.5);
	glUniform3fv(m_parameters[U_FOG_COLOR], 1, &fogcolor.r);
	glUniform1f(m_parameters[U_FOG_START], 140.f);
	glUniform1f(m_parameters[U_FOG_END], 150.f);
	glUniform1f(m_parameters[U_FOG_DENSITY], 0.005f);
	glUniform1i(m_parameters[U_FOG_TYPE], 0);
	glUniform1i(m_parameters[U_FOG_ENABLED], 0);

	camera = new Camera3();
	camera->Init(Vector3(0, 0, 450), Vector3(0, 0, 0), Vector3(0, 1, 0));
	m_lightDepthFBO.Init(2048, 2048);
	minimap_FBO.Init(1024, 1024);
	m_FrontFBO.Init(2048, 2048);

	objManager = ObjManager::getInstance();

	// REDUNDANT?
	for (int i = 0; i < TOTAL_OBJ; ++i) {
		objList[i] = new Obj();
	}
	
	InitSounds();
	InitMeshList();
	InitEnvironment();

	player = new Player();
	player->SetCamera(camera);
	player->SetTerrain(objList[OBJ_TERRAIN]);
	player->SetHeightMap(m_heightMap);

	objList[OBJ_PLAYER]->mesh = MeshBuilder::GenerateOBJ("Player", "OBJ//player.obj");
	objList[OBJ_PLAYER]->mesh->textureArray[0] = LoadTGA("Image//player.tga");
	Loadcoord("OBJ//player.obj", objList[OBJ_PLAYER]->dimensions);
	objList[OBJ_PLAYER]->trs.translate = player->trs.translate;
	objList[OBJ_PLAYER]->trs.rotateAngles.y = 0;
	objList[OBJ_PLAYER]->trs.Scale = Vector3(2, 2, 2);
	objList[OBJ_PLAYER]->initialFront.Set(0, 0, -1);
	player->dimensions = objList[OBJ_PLAYER]->dimensions;
	objList[OBJ_PLAYER]->type = Obj::OBJ_TYPE::O_PLAYER;
	objManager->Add(objList[OBJ_PLAYER]);
	player->initialY = player->trs.translate.y;

	meshList[GEO_AMMO_BAR_FRONT] = MeshBuilder::GenerateQuad("ammo_bar_front", Color(1, 1, 0), 1.f);
	meshList[GEO_AMMO_BAR_BACK] = MeshBuilder::GenerateQuad("ammo_bar_back", Color(0, 1, 1), 1.f);

	InitEnemyModels();
	InitWeaponModels();
	InitMiscModels();
	InitWeapons();

	// Create 10 projectile objects for object pooling
	for (int i = 0; i < 10; ++i)
	{
		projectileList.push_back(new Projectile);
	}

	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	projectionStack.LoadMatrix(perspective);

	bLightEnabled = true;
	b_texturemove = false;

	fog_active = 0;
	fog_ftime = 0;
	fog_colorChange = 0;

	m_speed = 1.f;
	m_gravity.Set(0, -9.8f, 0);

	ParticleSystems = new ParticleSystemManager();
	ParticleSystems->setCamera(camera);
	ParticleSystems->setTerrain(objList[OBJ_TERRAIN]->trs);
	ParticleSystems->setHeightMap(m_heightMap);
	bigScreenMinimap = 0;

	playerResPoint.Set(0, 0, 0);

	cSoundController->GetInstance()->StopAllSounds();
	cSoundController->SetListenerPosition(player->trs.translate.x, player->trs.translate.y, player->trs.translate.z);
	cSoundController->SetListenerDirection(-player->GetDir().x, player->GetDir().y, -player->GetDir().z);

	v1bool = 0;
	v2bool = 0;
	collidingv1 = 0;
	collidingv2 = 0;
	textbouncetime = 0;
	isv1orv2cleared = 0;
}

void SceneBasic::Update(double dt)
{
	//if(Application::IsKeyPressed('1'))
	//	glEnable(GL_CULL_FACE);
	//if(Application::IsKeyPressed('2'))
	//	glDisable(GL_CULL_FACE);
	//if(Application::IsKeyPressed('3'))
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//if(Application::IsKeyPressed('4'))
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (Application::IsKeyPressed('5'))
	{
		lights[0].type = Light::LIGHT_POINT;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	}
	else if (Application::IsKeyPressed('6'))
	{
		lights[0].type = Light::LIGHT_DIRECTIONAL;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	}
	else if (Application::IsKeyPressed('7'))
	{
		lights[0].type = Light::LIGHT_SPOT;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	}
	else if (Application::IsKeyPressed('8'))
	{
		bLightEnabled = true;
	}
	else if (Application::IsKeyPressed('9'))
	{
		bLightEnabled = false;
	}
	if (Application::IsKeyPressed('I'))
		lights[0].position.z -= (float)(10.f * dt);
	if (Application::IsKeyPressed('K'))
		lights[0].position.z += (float)(10.f * dt);
	if (Application::IsKeyPressed('J'))
		lights[0].position.x -= (float)(10.f * dt);
	if (Application::IsKeyPressed('L'))
		lights[0].position.x += (float)(10.f * dt);
	if (Application::IsKeyPressed('O'))
		lights[0].position.y -= (float)(10.f * dt);
	if (Application::IsKeyPressed('P'))
		lights[0].position.y += (float)(10.f * dt);

	playerResPoint = player->trs.translate;
	Vector3 oldTarget = player->getvec3Target();

	if (Application::IsKeyPressed('W'))
	{
		player->ProcessMovement(player->FORWARD, (float)dt);
	}
	if (Application::IsKeyPressed('A'))
	{
		player->ProcessMovement(player->LEFT, (float)dt);
	}
	if (Application::IsKeyPressed('S'))
	{
		player->ProcessMovement(player->BACKWARD, (float)dt);
	}
	if (Application::IsKeyPressed('D'))
	{
		player->ProcessMovement(player->RIGHT, (float)dt);
	}
	if (Application::IsKeyPressed(VK_SPACE))
	{
		player->SetToJump();
	}
	if (Application::IsKeyPressed(VK_LSHIFT) && player->IsCrouch() == false && player->IsScope() == false && player->IsProne() == false)
	{
		player->SetSprint(true);
	}
	else
	{
		player->SetSprint(false);
	}
	if (Application::IsKeyPressed(VK_LCONTROL) && player->IsSprint() == false && player->IsProne() == false)
	{
		player->SetCrouch(true);
	}
	else
	{
		player->SetCrouch(false);
	}

	if (Application::IsKeyPressed('Z') && player->IsCrouch() == false && player->IsSprint() == false)
	{
		player->SetProne(true);
	}
	else
	{
		player->SetProne(false);
	}

	if (Application::IsKeyPressed('1'))
	{
		if (player->GetWeapon()->GetType() != Weapon::SNIPER)
		{
			player->SetCurrentWeapon(Weapon::SNIPER);
			cSoundController->PlaySoundByID(4);
		}
	}
	if (Application::IsKeyPressed('2'))
	{
		if (player->GetWeapon()->GetType() != Weapon::SHOTGUN)
		{
			player->SetCurrentWeapon(Weapon::SHOTGUN);
			cSoundController->PlaySoundByID(6);
		}
	}
	if (Application::IsKeyPressed('3'))
	{
		if (player->GetWeapon()->GetType() != Weapon::PISTOL)
		{
			player->SetCurrentWeapon(Weapon::PISTOL);
			cSoundController->PlaySoundByID(5);
		}
	}
	if (Application::IsKeyPressed('4'))
	{
		if (player->GetWeapon()->GetType() != Weapon::TASER)
		{
			player->SetCurrentWeapon(Weapon::TASER);
			cSoundController->PlaySoundByID(5);
		}
	}
	if (Application::IsKeyPressed('R'))
	{
		player->GetWeapon()->Reload();
	}

	static bool bLButtonState = false;
	//FIRE
	if (!bLButtonState && Application::IsMousePressed(0))
	{
		if (player->IsSprint())
		{
			player->SetSprint(false);
		}
		Projectile* projectile = player->Shoot();
		if (projectile)
		{
			projectileList.push_back(projectile);
			//cSoundController->PlaySoundByID(1);
		}
		bLButtonState = true;
	}
	//STOP FIRING
	else if (bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
		//std::cout << "LBUTTON UP" << std::endl;
	}
	//SCOPE
	static bool bRButtonState = false;
	if (!bRButtonState && Application::IsMousePressed(1))
	{
		bRButtonState = true;
		//std::cout << "RBUTTON DOWN" << std::endl;
		if (player->IsSprint())
		{
			player->SetSprint(false);
		}
		player->SetScope(true);
		if (player->GetWeapon()->GetType() == Weapon::SNIPER)
		{
			cSoundController->PlaySoundByID(15);
		}
	}
	//STOP SCOPING
	else if (bRButtonState && !Application::IsMousePressed(1))
	{
		bRButtonState = false;
		//std::cout << "RBUTTON UP" << std::endl;
		player->SetScope(false);
	}
	if (Application::IsKeyPressed(VK_TAB)) {
		bigScreenMinimap = 1;
	}
	else {
		bigScreenMinimap = 0;
	}
	static double bounceTime7 = 0; // Time 3D sound is going to be played

	if (dt < 1.0)
	{
		for (int i = 0; i < enemyList.size(); ++i) {
			enemyList[i]->Update(dt);
			enemyList[i]->isColliding = false;
			for (unsigned j = 0; j < PlayerCollidingEntities.size(); ++j) {
				if (enemyList[i] != PlayerCollidingEntities[j] && collision_detection::collision_detector(enemyList[i], PlayerCollidingEntities[j])) {
					enemyList[i]->isColliding = true;
					break;
				}
			}
			if (enemyList[i]->enemyState == Enemy::ENEMY_STATE::REPORT) {
				Enemy* closestPolice = nullptr;
				float closestDistance = INT_MAX;
				for (int x = 0; x < enemyList.size(); ++x) {
					if (enemyList[x] != enemyList[i] && enemyList[x]->enemyType == Enemy::ENEMY_TYPE::POLICE) {
						float distance = (enemyList[x]->trs.translate - enemyList[i]->trs.translate).LengthSquared();
						if (distance < closestDistance) {
							closestPolice = enemyList[x];
							closestDistance = distance;
						}
					}
				}
				enemyList[i]->vel = closestPolice->trs.translate - enemyList[i]->trs.translate;
				enemyList[i]->trs.rotateAngles.y = -Math::RadianToDegree(atan2(enemyList[i]->vel.z, enemyList[i]->vel.x)) + 90;
				if (CheckDistance(enemyList[i], closestPolice, 2.5)) {
					enemyList[i]->enemyState = Enemy::ENEMY_STATE::IDLE;
					closestPolice->enemyState = Enemy::ENEMY_STATE::CHASE;
					cSoundController->UpdateSoundPos(105, enemyList[i]->trs.translate);
					cSoundController->PlaySoundByID(105);
				}
			}
			else if (enemyList[i]->enemyState == Enemy::ENEMY_STATE::CHASE) {
				enemyList[i]->vel = Vector3(player->trs.translate - enemyList[i]->trs.translate);
				enemyList[i]->vec3Front = enemyList[i]->vel.Normalized();
			}
			else if (enemyList[i]->enemyState == Enemy::ENEMY_STATE::PATROL) {
				enemyList[i]->vel = enemyList[i]->patrolDir;
				enemyList[i]->vec3Front = enemyList[i]->vel;
				if (CheckDistance(enemyList[i], player, enemyList[i]->visionRange) && WithinRadius(enemyList[i], player, enemyList[i]->visionRadius) && LineOfSight(enemyList[i], player)) {
					if (enemyList[i]->enemyType == Enemy::ENEMY_TYPE::CITIZEN) {
						cSoundController->UpdateSoundPos(102, enemyList[i]->trs.translate);
						cSoundController->PlaySoundByID(102);
						enemyList[i]->enemyState = Enemy::ENEMY_STATE::REPORT;
					}
					else if (enemyList[i]->enemyType == Enemy::ENEMY_TYPE::POLICE) {
						enemyList[i]->enemyState = Enemy::ENEMY_STATE::CHASE;
						cSoundController->UpdateSoundPos(105, enemyList[i]->trs.translate);
						cSoundController->PlaySoundByID(105);
					}
				}
			}
			else if (enemyList[i]->enemyState == Enemy::ENEMY_STATE::IDLE) {
				enemyList[i]->vel.SetZero();
				if (CheckDistance(enemyList[i], player, enemyList[i]->visionRange) && WithinRadius(enemyList[i], player, enemyList[i]->visionRadius) && LineOfSight(enemyList[i], player)) {
					if (enemyList[i]->enemyType == Enemy::ENEMY_TYPE::CITIZEN) {
						cSoundController->UpdateSoundPos(102, enemyList[i]->trs.translate);
						cSoundController->PlaySoundByID(102);
						enemyList[i]->enemyState = Enemy::ENEMY_STATE::REPORT;
					}
					else if (enemyList[i]->enemyType == Enemy::ENEMY_TYPE::POLICE) {
						enemyList[i]->enemyState = Enemy::ENEMY_STATE::CHASE;
						cSoundController->UpdateSoundPos(105, enemyList[i]->trs.translate);
						cSoundController->PlaySoundByID(105);
					}
				}
				else if (enemyList[i]->patrolStartPoint.x != 0 && enemyList[i]->patrolStartPoint.y != 0 && enemyList[i]->patrolStartPoint.z != 0) {
					if ((enemyList[i]->patrolStartPoint - enemyList[i]->trs.translate).LengthSquared() > 0.25) {
						enemyList[i]->vel = enemyList[i]->patrolStartPoint - enemyList[i]->trs.translate;
					}
					else {
						enemyList[i]->enemyState = Enemy::ENEMY_STATE::PATROL;
					}
				}
			}

			if (enemyList[i]->vel.x == 0 && enemyList[i]->vel.y == 0 && enemyList[i]->vel.z == 0) {
				//enemyList[i]->trs.rotateAngles.y = 0;
			}
			else {
				enemyList[i]->trs.rotateAngles.y = -Math::RadianToDegree(atan2(enemyList[i]->vel.z, enemyList[i]->vel.x)) + 90;
			}
			if (collision_detection::collision_detector(enemyList[i], player)) {
				SceneManager* scene = SceneManager::GetInstance();
				scene->SetNextScene(STATE::LOSE_SCENE);
			}
		}
	}

	for (unsigned i = 0; i < enemyList.size(); ++i)
	{
		flashlights[i]->trs.translate = enemyList[i]->trs.translate + enemyList[i]->vec3Front;

		if (enemyList[i]->enemyType == Enemy::ENEMY_TYPE::POLICE)
		{
			// Position mask
			masks[i]->trs.translate = enemyList[i]->trs.translate + enemyList[i]->vec3Front * 0.5f;
			masks[i]->trs.translate.y += 1.3f;
			flashlights[i]->trs.translate.y += 1.5f;
		}
		else
			flashlights[i]->trs.translate.y += 1.6f;
		flashlights[i]->trs.rotateAngles.y = enemyList[i]->trs.rotateAngles.y;
		// Position flashlight according to enemy direction
		if (enemyList[i]->trs.rotateAngles.y == 90)
		{
			if (enemyList[i]->enemyType == Enemy::ENEMY_TYPE::POLICE)
			{
				flashlights[i]->trs.translate.z += 0.7f;
				flashlights[i]->trs.translate.x -= 0.9f;
			}
			else
			{
				flashlights[i]->trs.translate.z += 0.7f;
				flashlights[i]->trs.translate.x -= 0.7f;
			}
		}
		else if (enemyList[i]->trs.rotateAngles.y == -90)
		{
			if (enemyList[i]->enemyType == Enemy::ENEMY_TYPE::POLICE)
			{
				flashlights[i]->trs.translate.z -= 0.7f;
				flashlights[i]->trs.translate.x += 0.9f;
			}
			else
			{
				flashlights[i]->trs.translate.z -= 0.7f;
				flashlights[i]->trs.translate.x += 0.7f;
			}
		}
		else if (enemyList[i]->trs.rotateAngles.y == 180)
		{
			if (enemyList[i]->enemyType == Enemy::ENEMY_TYPE::POLICE)
			{
				flashlights[i]->trs.translate.z += 0.7f;
				flashlights[i]->trs.translate.x += 0.9f;
			}
			else
			{
				flashlights[i]->trs.translate.z += 0.7f;
				flashlights[i]->trs.translate.x += 0.7f;
			}
		}
		else if (enemyList[i]->trs.rotateAngles.y == 0)
		{
			if (enemyList[i]->enemyType == Enemy::ENEMY_TYPE::POLICE)
			{
				flashlights[i]->trs.translate.z -= 0.7f;
				flashlights[i]->trs.translate.x -= 0.9f;
			}
			else
			{
				flashlights[i]->trs.translate.z -= 0.7f;
				flashlights[i]->trs.translate.x -= 0.7f;
			}
		}

		if (bounceTime7 < 0)
		{
			if (enemyList[i]->enemyType == Enemy::ENEMY_TYPE::CITIZEN)
			{
				cSoundController->UpdateSoundPos(101, enemyList[i]->trs.translate);
				cSoundController->PlaySoundByID(101);
			}
		}
	}

	// Position spot lights to respective enemies
	for (int i = 1; i < NUM_OF_LIGHTS - 2; ++i)
	{
		lights[i].position.Set(enemyList[i - 1]->trs.translate.x, enemyList[i - 1]->trs.translate.y, enemyList[i - 1]->trs.translate.z);
		lights[i].spotDirection.Set(-enemyList[i - 1]->vec3Front.x, -enemyList[i - 1]->vec3Front.y, -enemyList[i - 1]->vec3Front.z);
	}

	if (bounceTime7 < 0)
	{
		bounceTime7 = dt * 100;
	}
	//cout << bounceTime7 << endl;
	bounceTime7 -= dt;
	for (std::vector<Projectile*>::iterator it = projectileList.begin(); it != projectileList.end(); ++it)
	{
		Projectile* p = (Projectile*)*it;
		if (p->IsActive())
		{
			p->Update(dt);
			p->isOutOfBounds(objList[OBJ_TERRAIN]);
			p->CheckCollision(objList[OBJ_BULLET], PlayerCollidingEntities);
		}
	}
	if (player->GetWeapon())
		player->GetWeapon()->Update(dt);

	if (Application::IsKeyPressed('U') && GetTickCount() * 0.001f - fog_bouncetime > 0.2f) {
		fog_bouncetime = GetTickCount() * 0.001f;
		fog_active = !fog_active;
		fog_ftime = 0;
	}
	if (fog_active && fog_ftime < 13) {
		fog_ftime += (float)dt;
		//Color()
		glUniform1f(m_parameters[U_FOG_START], 140.f - fog_ftime * 10);
	}
	if (fog_active && GetTickCount() * 0.001f - fog_color_ftime > 1.f) {
		fog_color_ftime = GetTickCount() * 0.001f;
		fog_colorChange += 1;
		fog_colorChange %= 5;
		Color fog_colors[5];
		fog_colors[0].Set(64.f / 255.f, 224.f / 255.f, 208.f / 255);
		fog_colors[1].Set(255.f / 255, 155.f / 255, 155.f / 255);
		fog_colors[2].Set(255.f / 255, 165.f / 255, 0.f / 255);
		fog_colors[3].Set(66.f / 255, 4.f / 255, 32.f / 255);
		fog_colors[4].Set(240.f / 255, 255.f / 255, 0.f / 255);
		glUniform3fv(m_parameters[U_FOG_COLOR], 1, &fog_colors[fog_colorChange].r);
	}

	if (collision_detection::collision_detector(player, objList[OBJ_VICTORY1])) {
		//do touch hospital1 things
		if (!v1bool) {
			collidingv1 = 1;
		}
		if (!v1bool && Application::IsKeyPressed('E')) {
			v1bool = 1;
			collidingv1 = 0;
			isv1orv2cleared = 1;
			textbouncetime = GetTickCount() * 0.001f;
		}
	}
	else {
		collidingv1 = 0;
	}
	if (collision_detection::collision_detector(player, objList[OBJ_VICTORY2])) {
		//do touch hospital2 things
		if (!v2bool) {
			collidingv2 = 1;
		}
		if (!v2bool && Application::IsKeyPressed('E')) {
			v2bool = 1;
			collidingv2 = 0;
			isv1orv2cleared = 1;
			textbouncetime = GetTickCount() * 0.001f;
		}
	}
	else {
		collidingv2 = 0;
	}

	player->UpdateCollision(dt, PlayerCollidingEntities, playerResPoint,oldTarget);
	player->UpdatePlayerVectors(camera, (float)dt);
	camera->Update(dt);

	Vector3 dir = camera->target - camera->position;
	dir.Normalize();

	switch (player->GetWeapon()->GetType())
	{
	case Weapon::SNIPER:
	{
		objList[OBJ_SNIPER]->trs.translate = player->trs.translate + player->GetDir();
		objList[OBJ_SNIPER]->trs.rotateAngles.y = Algorithm::Math::RadianstoDegree(atan2(dir.x,dir.z)); // Yaw
		objList[OBJ_SNIPER]->trs.translate += dir * 2.f;
		break;
	}
	case Weapon::SHOTGUN:
		objList[OBJ_SHOTGUN]->trs.translate = player->trs.translate + player->GetDir();
		objList[OBJ_SHOTGUN]->trs.rotateAngles.y = Algorithm::Math::RadianstoDegree(atan2(dir.x, dir.z)); // Yaw
		objList[OBJ_SHOTGUN]->trs.translate += dir * 2.f;
		break;
	case Weapon::PISTOL:
		objList[OBJ_PISTOL]->trs.translate = player->trs.translate + player->GetDir();
		objList[OBJ_PISTOL]->trs.rotateAngles.y = Algorithm::Math::RadianstoDegree(atan2(dir.x, dir.z)); // Yaw
		objList[OBJ_PISTOL]->trs.translate += dir * 2.f;
		break;
	case Weapon::TASER:
		objList[OBJ_TASER]->trs.translate = player->trs.translate + player->GetDir();
		objList[OBJ_TASER]->trs.rotateAngles.y = Algorithm::Math::RadianstoDegree(atan2(dir.x, dir.z)); // Yaw
		objList[OBJ_TASER]->trs.translate += dir * 2.f;
		break;
	}

	if (v1bool && v2bool) {
		SceneManager* scene = SceneManager::GetInstance();
		scene->SetNextScene(STATE::WIN_SCENE);
	}
  
	objList[OBJ_PLAYER]->trs.Scale = player->trs.Scale;
	objList[OBJ_PLAYER]->trs.translate = player->trs.translate;

	ParticleSystems->Update(dt);
	objManager->Update();

	cSoundController->SetListenerPosition(player->trs.translate.x, player->trs.translate.y, player->trs.translate.z);
	cSoundController->SetListenerDirection(-player->GetDir().x, player->GetDir().y, -player->GetDir().z);

	fps = (float)(1.f / dt);

	objList[OBJ_TERRAIN]->mesh->moveVal += (float)(sin(objList[OBJ_TERRAIN]->mesh->moveVal_increment) * dt);
}

void SceneBasic::RenderText(Mesh* mesh, std::string text, Color color)
{
	if (!mesh || mesh->textureArray[0] <= 0)
		return;

	glDisable(GL_DEPTH_TEST);
	glUniform1f(m_parameters[U_ISMVP], 1.f);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureArray[0]);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glUniform1f(m_parameters[U_ISMVP], 1);
	glEnable(GL_DEPTH_TEST);
}

void SceneBasic::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if (!mesh || mesh->textureArray[0] <= 0)
		return;

	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform1f(m_parameters[U_ISMVP], 1.f);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureArray[0]);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 0.6f + 0.5f, 0.5f, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void SceneBasic::RenderMeshIn2D(Mesh* mesh, bool enableLight, float size, float x, float y)
{
	glDisable(GL_CULL_FACE);
	Mtx44 ortho;
	ortho.SetToOrtho(-80, 80, -60, 60, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Scale(size, size, size);
	modelStack.Translate(x, y, 0);
	glUniform1f(m_parameters[U_ISMVP], 1.f);

	Mtx44 MVP, modelView, modelView_inverse_transpose;
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	if (mesh->textureArray[0] > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureArray[0]);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render();
	if (mesh->textureArray[0] > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
	glEnable(GL_CULL_FACE);
}

void SceneBasic::RenderTerrain()
{
	RenderObj(objList[OBJ_TERRAIN], true);
}

void SceneBasic::RenderSkyDome()
{
	RenderObj(objList[OBJ_SKYDOME], false, true,true);
}

void SceneBasic::RenderObj(Mesh* mesh, TRS trs, bool enableLight, bool isUVmoving, bool end, bool isMVP, bool isWave)
{
	modelStack.PushMatrix();
	modelStack.Translate(trs.translate);
	modelStack.Rotate(trs.rotateAngles.x, 1, 0, 0);
	modelStack.Rotate(trs.rotateAngles.y, 0, 1, 0);
	modelStack.Rotate(trs.rotateAngles.z, 0, 0, 1);
	modelStack.Scale(trs.Scale);
	RenderMesh(mesh, enableLight, isUVmoving, isMVP, isWave);
	if (end)
		modelStack.PopMatrix();
}

void SceneBasic::RenderObj(Obj* obj, bool enableLight, bool end, bool isUVmoving, bool isMVP, bool isWave)
{
	modelStack.PushMatrix();
	modelStack.Transform(obj->trs);
	RenderMesh(obj->mesh, enableLight, isUVmoving, isMVP, isWave);
	if (end)
		modelStack.PopMatrix();
}


void SceneBasic::RenderParticle()
{
	struct {
		bool operator()(Particle* a, Particle* b) {
			return a->DistFromCamSquare > b->DistFromCamSquare;
		}
	} comparitor;
	Algorithm::SearchnSort::quickSort(ParticleSystems->getAllPart(), 0, ParticleSystems->getAllPart().size() - 1, comparitor);
	for (std::vector<Particle*>::iterator it = ParticleSystems->getAllPart().begin(); it != ParticleSystems->getAllPart().end(); ++it) {
		Particle* p = (Particle*)*it;
		if (!p->active) {
			continue;
		}
		if (mRenderPass == RENDERPASS_PRE) {
			TRS temp = p->transform;
			temp.rotateAngles.y = Math::RadianToDegree(
				atan2(lights[0].position.x - p->transform.translate.x, lights[0].position.z - p->transform.translate.z));
			RenderObj(p->mesh, temp, false, true);
		}
		else {
			//do smthing
			RenderObj(p->mesh, p->transform, false);
		}
	}
}

void SceneBasic::RenderPassGPass()
{
	mRenderPass = RENDERPASS_PRE;
	m_lightDepthFBO.BindForWriting();
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClear(GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_gPassShaderID);

	if (lights[0].type == Light::LIGHT_DIRECTIONAL)
		m_lightDepthProj.SetToOrtho(-500, 500, -500, 500, -500, 1000 );
	else
	{
		return;
		m_lightDepthProj.SetToPerspective(90, 1.f, 0.1, 200);
	}
	m_lightDepthView.SetToLookAt(lights[0].position.x, lights[0].position.y, lights[0].position.z, 0, 0, 0, 0, 1, 0);

	RenderWorld();
}

void SceneBasic::RenderPassMinimap()
{
	mRenderPass = RENDERPASS_MINIMAP;
	//glBindBuffer(GL_FRAMEBUFFER, 0);
	minimap_FBO.BindForWriting();
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(m_programID);
	m_minimapProj.SetToOrtho(-100, 100, -100, 100, -100, 160);
	m_minimapView.SetToLookAt(player->trs.translate.x, player->trs.translate.y, player->trs.translate.z,
		player->trs.translate.x, (double)player->trs.translate.y - 1.f, player->trs.translate.z,
		player->vec3Front.x, player->vec3Front.y, player->vec3Front.z);

	RenderTerrain();
	renderEnemyModels = false;
	RenderWorld();
	
	meshList[GEO_CIRCLE_MINIMAP]->textureArray[0] = minimap_FBO.GetTexture();

	int size = enemyList.size();
	for (int i = 0; i < size; ++i) {
		modelStack.PushMatrix();
		modelStack.Translate(enemyList.at(i)->trs.translate);
		modelStack.Rotate(-90, 1, 0, 0);
		modelStack.Scale(12, 12, 1);
		RenderMesh(meshList[GEO_ENEMY_CURSOR], false);
		modelStack.PopMatrix();
	}

	modelStack.PushMatrix();
	modelStack.Translate(player->trs.translate);
	modelStack.Rotate(Math::RadianToDegree(atan2(player->GetDir().x, player->GetDir().z)), 0, 1, 0);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(20, 20, 20);
	RenderMesh(meshList[GEO_PLAYER_CURSOR], false);
	modelStack.PopMatrix();
}

void SceneBasic::RenderPassFront()
{
	mRenderPass = RENDERPASS_MAIN;
	m_FrontFBO.BindForWriting();
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(m_programID);
	renderEnemyModels = true;
	Mtx44 perspective;
	perspective.SetToPerspective(player->GetZoom(), Application::getWindowWidth() / Application::getWindowHeight(), 0.1f, 10000.0f);
	projectionStack.LoadMatrix(perspective);

	// Camera matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera->position.x, camera->position.y, camera->position.z,
		camera->target.x, camera->target.y, camera->target.z,
		camera->up.x, camera->up.y, camera->up.z
	);
	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();

	PassLightInfoToShader();

	//RenderGround();
	RenderSkyDome();
	RenderTerrain();

	RenderWorld();
}

void SceneBasic::RenderPassMain()
{
	mRenderPass = RENDERPASS_MAIN;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, Application::getWindowWidth(), Application::getWindowHeight());

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_programID);

	m_lightDepthFBO.BindForReading(GL_TEXTURE8);
	glUniform1i(m_parameters[U_SHADOW_MAP], 8);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Mtx44 perspective;
	perspective.SetToPerspective(player->GetZoom(), Application::getWindowWidth()/Application::getWindowHeight(), 0.1f, 10000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);

	// Camera matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera->position.x, camera->position.y, camera->position.z,
		camera->target.x, camera->target.y, camera->target.z,
		camera->up.x, camera->up.y, camera->up.z
	);
	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();

	PassLightInfoToShader();

	if (fog_active) {
		glUniform1i(m_parameters[U_FOG_ENABLED], 1);
	}
	//RenderMesh(meshList[GEO_AXES], false);

	RenderSkyDome();
	RenderTerrain();

	renderEnemyModels = true;
	RenderWorld();

	RenderMeshIn2D(meshList[GEO_CROSSHAIR], false, 5.0f);
	std::ostringstream ss;
	if (bigScreenMinimap) {
		RenderMeshIn2D(meshList[GEO_MINIMAP_QUAD], false, 100.f, -0.1f, 0.f);
		RenderMeshIn2D(meshList[GEO_FRONT_QUAD], false, 30.f, 2.f, 1.3f);
	}
	else {
		RenderMeshIn2D(meshList[GEO_CIRCLE_MINIMAP], false, 20.f, 3.f, 1.8f);
		if (player->GetWeapon()->GetType() != Weapon::TASER) {
		RenderMeshOnScreen(meshList[GEO_MAINGUI],47,23,60,45);
			//RenderMeshOnScreen(meshList[GEO_AMMO_BAR_BACK], 62, 4, , 3);
			RenderMeshOnScreen(meshList[GEO_AMMO_BAR_FRONT],
				63.5 - ((22 * ((float)(player->GetWeapon()->GetMaxMagCapacity() - player->GetWeapon()->GetMagCapacity()) 
					/ player->GetWeapon()->GetMaxMagCapacity())) / 2),
				4.2, 22 * ((float)player->GetWeapon()->GetMagCapacity() / player->GetWeapon()->GetMaxMagCapacity()), 3.2);
			ss.str("");
			ss << player->GetWeapon()->GetMagCapacity();
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 55.5, 16.5);
			ss.str("");
			ss << player->GetWeapon()->GetMaxMagCapacity();
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 62.5, 13);
			ss.str("");
			ss << "x" << (int)(player->GetWeapon()->GetTotalMag());
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 70, 2.68);
		}
		
	}
	if (collidingv1 || collidingv2) {
		//ss.str("The cure in this hospital is not enough, I need to get to the other hospital");
		ss.str("Press E to collect cure from this hospital");
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 2, 1, 30);
	}
	if (isv1orv2cleared && GetTickCount() * 0.001f - textbouncetime < 5.f) {
		ss.str("The cure in this hospital is not enough,");
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 2, 1, 30);
		ss.str("I need to get to the other hospital");
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 2, 1, 27);
	}
	//On screen text
	ss.str("");
	ss.precision(2);
	ss << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 55.5);

	if (player->IsCollidingWithPickUp())
	{
		ss.str("");
		ss.precision(4);
		ss << "Press E to pick up item";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 5.1f, 18);
	}
}

void SceneBasic::RenderWorld()
{
	switch (player->GetWeapon()->GetType())
	{
	case Weapon::SNIPER:
		RenderGun(objList[OBJ_SNIPER]);
		break;
	case Weapon::SHOTGUN:
		RenderGun(objList[OBJ_SHOTGUN]);
		break;
	case Weapon::PISTOL:
		RenderGun(objList[OBJ_PISTOL]);
		break;
	case Weapon::TASER:
		RenderGun(objList[OBJ_TASER]);
		break;
	}

	for (int i = 0; i < objManager->getList().size(); ++i)
	{
		if (objManager->getList()[i]->type == Obj::O_ENEMY)
		{
			if(renderEnemyModels == true)
				RenderObj(objManager->getList()[i], true);
		}
		else if (objManager->getList()[i]->type == Obj::O_OBSTACLES)
		{
			RenderObj(objManager->getList()[i], true);
		}
		else if (objManager->getList()[i]->type == Obj::O_ITEMS)
		{
			RenderObj(objManager->getList()[i], true);
		}
	}
	for (unsigned i = 0; i < flashlights.size(); ++i)
	{
		RenderObj(flashlights[i], true);
	}
	for (unsigned i = 0; i < masks.size(); ++i)
	{
		RenderObj(masks[i], true);
	}
	for (std::vector<Projectile*>::iterator it = projectileList.begin(); it != projectileList.end(); ++it)
	{
		Projectile* p = (Projectile*)*it;
		if (p->IsActive() && p->IsRender())
		{
			modelStack.PushMatrix();
			modelStack.Translate(p->GetPos());
			Vector3 front = p->dir.Cross(p->up);
			modelStack.Rotate(p->pitch, front.x, front.y, front.z);
			modelStack.Rotate(Algorithm::Math::RadianstoDegree(atan2(p->dir.x, p->dir.z)), 0, 1, 0);
			modelStack.Scale(p->trs.Scale);
			RenderMesh(objList[OBJ_BULLET]->mesh, false);
			modelStack.PopMatrix();
		}
	}
}

void SceneBasic::Render()
{
	//PRE RENDER PASS
	RenderPassGPass();

	//MINIMAP RENDER PASS
	RenderPassMinimap();

	if (bigScreenMinimap) {
		RenderPassFront();
	}

	//MAIN RENDER PASS
	RenderPassMain();
}

bool SceneBasic::CheckDistance(Obj* source, Obj* destination, float distance)
{
	Vector3 displacement = destination->trs.translate - source->trs.translate;
	if (displacement.LengthSquared() < distance * distance) {
		//cout << "Within distance" << endl;
		return true;
	}
	return false;
}

bool SceneBasic::WithinRadius(Obj* source, Obj* destination, float angle)
{
	Vector3 currentDir(cos(Math::DegreeToRadian(-source->trs.rotateAngles.y)) * source->initialFront.x - sin(Math::DegreeToRadian(-source->trs.rotateAngles.y)) * source->initialFront.z,
		source->initialFront.y,
		sin(Math::DegreeToRadian(-source->trs.rotateAngles.y)) * source->initialFront.x + cos(Math::DegreeToRadian(-source->trs.rotateAngles.y)) * source->initialFront.z);
	Vector3 displacement = destination->trs.translate - source->trs.translate;
	float calculatedAngle = acosf(currentDir.Dot(displacement) * (1 / (currentDir.Length() * displacement.Length())));
	if (calculatedAngle < angle) {
		//cout << "Within cone" << endl;
		return true;
	}
	return false;
}

bool SceneBasic::LineOfSight(Obj* source, Obj* destination)
{
	Vector3 displacement = destination->trs.translate - source->trs.translate;
	Vector3 displacementN;
	try
	{
		displacementN = displacement.Normalized();
	}
	catch (DivideByZero exception)
	{
		return true;
	}
	Obj* SightRay = new Obj();
	SightRay->trs.translate = source->trs.translate;
	while (SightRay->trs.translate != destination->trs.translate) {
		for (int i = 0; i < PlayerCollidingEntities.size(); ++i) {
			if (collision_detection::collision_detector(PlayerCollidingEntities[i]->trs, PlayerCollidingEntities[i]->dimensions, SightRay->trs, 1)) {
				if (PlayerCollidingEntities[i]->type == Obj::OBJ_TYPE::O_OBSTACLES) {
					delete SightRay;
					return false;
				}
				
			}
		}
		if (collision_detection::collision_detector(player->trs, player->dimensions, SightRay->trs, 1)) {
			delete SightRay;
			//cout << "Has line of sight" << endl;
			return true;
		}
		SightRay->trs.translate += displacementN;
	}
	delete SightRay;
	return false;
}

void SceneBasic::RenderGun(Obj* gun)
{
	modelStack.PushMatrix();
	Vector3 Displacement;
	Displacement = Vector3(0.5f, -0.4f, 0);
	float c = cos(Math::DegreeToRadian(gun->trs.rotateAngles.y));
	float s = sin(Math::DegreeToRadian(gun->trs.rotateAngles.y));
	modelStack.Translate(gun->trs.translate);
	gun->trs.rotateAngles.x = gun->trs.rotateAngles.z = 0;
	Vector3 front = player->GetRight();
	modelStack.Rotate(player->getPitch(), front.x, front.y, front.z);
	modelStack.Rotate(gun->trs.rotateAngles.y, 0, 1, 0);
	modelStack.Translate(Displacement);
	modelStack.Scale(gun->trs.Scale);
	RenderMesh(gun->mesh, false);
	modelStack.PopMatrix();
}

void SceneBasic::Exit()
{
	// Cleanup VBO
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i])
			delete meshList[i];
	}
	while (projectileList.size() > 0)
	{
		Projectile* p = projectileList.back();
		delete p;
		projectileList.pop_back();
	}
	objManager->Destroy();
	delete camera;
	delete ParticleSystems;
	glDeleteProgram(m_programID);
	glDeleteProgram(m_gPassShaderID);
	glDeleteVertexArrays(1, &m_vertexArrayID);
}
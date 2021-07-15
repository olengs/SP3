#include "SceneBasic.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"

void SceneBasic::InitEnvironment()
{
	objList[OBJ_TERRAIN]->trs.translate.Set(0, 0, 0);
	objList[OBJ_TERRAIN]->trs.Scale.Set(1500.f, 1.f, 1500.f);
	objList[OBJ_TERRAIN]->mesh = MeshBuilder::GenerateTerrain("GEO_TERRAIN", "Image//blackground.raw", m_heightMap, objList[OBJ_TERRAIN]->trs);
	objList[OBJ_TERRAIN]->mesh->textureArray[0] = LoadTGA("Image//street_texture.tga");
	objList[OBJ_TERRAIN]->type = Obj::OBJ_TYPE::O_TERRAIN;
	objManager->Add(objList[OBJ_TERRAIN]);

	objList[OBJ_SKYDOME]->mesh = MeshBuilder::GenerateSkyDome("GEO_SKYDOME", Color(1, 1, 1), 20, 20, 1);
	//objList[OBJ_SKYDOME]->mesh->textureArray[0] = LoadTGA("Image//day_sky.tga");
	objList[OBJ_SKYDOME]->mesh->textureArray[0] = LoadTGA("Image//night_sky.tga");
	objList[OBJ_SKYDOME]->mesh->moveVal_increment = 0.02f;
	objList[OBJ_SKYDOME]->trs.translate = Vector3(0, 0, 0);
	objList[OBJ_SKYDOME]->trs.Scale = Vector3(600, 600, 600);
	objList[OBJ_SKYDOME]->type = Obj::OBJ_TYPE::O_OTHER;
	objManager->Add(objList[OBJ_SKYDOME]);
}

void SceneBasic::InitEnemyModels()
{
	objList[OBJ_POLICE]->mesh = MeshBuilder::GenerateOBJ("Player", "OBJ//police.obj");
	objList[OBJ_POLICE]->mesh->textureArray[0] = LoadTGA("Image//police_texture.tga");
	Loadcoord("OBJ//police.obj", objList[OBJ_POLICE]->dimensions);
	objList[OBJ_POLICE]->type = Obj::OBJ_TYPE::O_ENEMY;

	std::vector<Vector3>initialPoliceCoords = { Vector3(190, 0, 90), Vector3(-30, 0, -350), Vector3(40, 0, -85) };
	std::vector<float>policeRotation = { 0, -90, -90 };
	std::vector<Vector3>initialPoliceDir = { Vector3(0, 0, 1), Vector3(-1, 0, 0), Vector3(-1, 0, 0) };
	for (unsigned i = 0; i < initialPoliceCoords.size(); ++i)
	{
		Enemy* police = new Enemy(objList[OBJ_POLICE]);
		police->trs.translate = initialPoliceCoords[i];
		police->trs.rotateAngles.y = policeRotation[i];
		police->initialFront = Vector3(0, 0, 1);
		police->setToFloor(m_heightMap, objList[OBJ_TERRAIN]);
		police->SetPatrol(initialPoliceDir[i], police->trs.translate, 15.f);
		enemyList.push_back(police);
		enemyList.back()->enemyType = Enemy::ENEMY_TYPE::POLICE;
		enemyList.back()->enemyState = Enemy::ENEMY_STATE::IDLE;
		PlayerCollidingEntities.push_back(police);
		objManager->Add(police);
	}

	objList[OBJ_CIVILIAN_A]->mesh = MeshBuilder::GenerateOBJ("Player", "OBJ//civilian.obj");
	objList[OBJ_CIVILIAN_A]->mesh->textureArray[0] = LoadTGA("Image//civilian_texture.tga");
	Loadcoord("OBJ//civilian.obj", objList[OBJ_CIVILIAN_A]->dimensions);
	objList[OBJ_CIVILIAN_A]->type = Obj::OBJ_TYPE::O_ENEMY;

	std::vector<Vector3>initialCivilianCoords = {
		Vector3(-20, 0, 335), Vector3(-50, 0, 250), Vector3(-30, 0, 250), 
		Vector3(-187, 0, -50), Vector3(-132, 0, -340), Vector3(-170, 0, -400), 
		Vector3(200, 0, -355), Vector3(212, 0, -210), Vector3(120, 0, -40),
		Vector3(300, 0, -40) };
	std::vector<float>civilianRotation = { 
		90, 180, 90, 
		180, 0, 90,
		-90, 180, 0,
		180};
	std::vector<Vector3>initialCivilianDir = {
		Vector3(1, 0, 0), Vector3(0, 0, -1), Vector3(1, 0, 0),
		Vector3(0, 0, -1), Vector3(0, 0, 1), Vector3(1, 0, 0),
		Vector3(-1, 0, 0), Vector3(0, 0, -1), Vector3(0, 0, 1),
		Vector3(0, 0, -1) };
	for (unsigned i = 0; i < initialCivilianCoords.size(); ++i)
	{
		Enemy* civilian = new Enemy(objList[OBJ_CIVILIAN_A]);
		civilian->trs.translate = initialCivilianCoords[i];
		civilian->trs.rotateAngles.y = civilianRotation[i];
		civilian->initialFront = Vector3(0, 0, 1);
		civilian->setToFloor(m_heightMap, objList[OBJ_TERRAIN]);
		civilian->SetPatrol(initialCivilianDir[i], civilian->trs.translate, 10);
		enemyList.push_back(civilian);
		enemyList.back()->enemyType = Enemy::ENEMY_TYPE::CITIZEN;
		enemyList.back()->enemyState = Enemy::ENEMY_STATE::PATROL;
		PlayerCollidingEntities.push_back(civilian);
		objManager->Add(civilian);
	}

	objList[OBJ_FLASHLIGHT]->mesh = MeshBuilder::GenerateOBJ("flashlight", "OBJ//flashlight.obj");
	objList[OBJ_FLASHLIGHT]->mesh->textureArray[0] = LoadTGA("Image//flashlight_texture.tga");
	Loadcoord("OBJ//flashlight.obj", objList[OBJ_FLASHLIGHT]->dimensions);
	objList[OBJ_FLASHLIGHT]->type = Obj::O_OTHER;

	std::vector<Vector3>enemyCoords;
	enemyCoords.reserve(initialPoliceCoords.size() + initialCivilianCoords.size());
	enemyCoords.insert(enemyCoords.end(), initialPoliceCoords.begin(), initialPoliceCoords.end());
	enemyCoords.insert(enemyCoords.end(), initialCivilianCoords.begin(), initialCivilianCoords.end());

	//std::vector<float>enemyRotation;
	//enemyRotation.reserve(policeRotation.size() + civilianRotation.size());
	//enemyRotation.insert(enemyRotation.end(), policeRotation.begin(), policeRotation.end());
	//enemyRotation.insert(enemyRotation.end(), civilianRotation.begin(), civilianRotation.end());

	//std::vector<Vector3>enemyInitialFront;
	//enemyInitialFront.reserve(initialPoliceFront.size() + initialCivilianFront.size());
	//enemyInitialFront.insert(enemyInitialFront.end(), initialPoliceFront.begin(), initialPoliceFront.end());
	//enemyInitialFront.insert(enemyInitialFront.end(), initialCivilianFront.begin(), initialCivilianFront.end());

	for (unsigned i = 0; i < enemyCoords.size(); ++i)
	{
		Obj* flashlight = new Obj(objList[OBJ_FLASHLIGHT]);
		flashlights.push_back(flashlight);
		objManager->Add(flashlight);
	}

	objList[OBJ_MASK_GREEN]->mesh = MeshBuilder::GenerateOBJ("MASK_GREEN", "OBJ//mask.obj");
	objList[OBJ_MASK_GREEN]->mesh->textureArray[0] = LoadTGA("Image//mask_texture_green.tga");
	Loadcoord("OBJ//mask.obj", objList[OBJ_MASK_GREEN]->dimensions);
	objList[OBJ_MASK_GREEN]->type = Obj::O_OTHER;

	for (unsigned i = 0; i < initialPoliceCoords.size(); ++i)
	{
		Obj* mask = new Obj(objList[OBJ_MASK_GREEN]);
		masks.push_back(mask);
		objManager->Add(mask);
	}
}

void SceneBasic::InitWeaponModels()
{
	objList[OBJ_SNIPER]->mesh = MeshBuilder::GenerateOBJ("Player", "OBJ//sniper.obj");
	objList[OBJ_SNIPER]->mesh->textureArray[0] = LoadTGA("Image//sniper_texture.tga");
	Loadcoord("OBJ//sniper.obj", objList[OBJ_SNIPER]->dimensions);
	objList[OBJ_SNIPER]->trs.translate = player->trs.translate;
	objList[OBJ_SNIPER]->trs.Scale = Vector3(1, 1, 1);
	objList[OBJ_SNIPER]->type = Obj::OBJ_TYPE::O_WEAPON;

	objList[OBJ_PISTOL]->mesh = MeshBuilder::GenerateOBJ("Pistol", "OBJ//pistol.obj");
	objList[OBJ_PISTOL]->mesh->textureArray[0] = LoadTGA("Image//pistol_texture.tga");
	Loadcoord("OBJ//pistol.obj", objList[OBJ_PISTOL]->dimensions);
	objList[OBJ_PISTOL]->trs.translate = player->trs.translate;
	objList[OBJ_PISTOL]->trs.Scale = Vector3(1, 1, 1);
	objList[OBJ_PISTOL]->type = Obj::OBJ_TYPE::O_WEAPON;

	objList[OBJ_SHOTGUN]->mesh = MeshBuilder::GenerateOBJ("Shotgun", "OBJ//shotgun.obj");
	objList[OBJ_SHOTGUN]->mesh->textureArray[0] = LoadTGA("Image//shotgun_texture.tga");
	Loadcoord("OBJ//shotgun.obj", objList[OBJ_SHOTGUN]->dimensions);
	objList[OBJ_SHOTGUN]->trs.translate = player->trs.translate;
	objList[OBJ_SHOTGUN]->trs.Scale = Vector3(1, 1, 1);
	objList[OBJ_SHOTGUN]->type = Obj::OBJ_TYPE::O_WEAPON;

	objList[OBJ_TASER]->mesh = MeshBuilder::GenerateOBJ("Taser", "OBJ//taser.obj");
	objList[OBJ_TASER]->mesh->textureArray[0] = LoadTGA("Image//taser_texture.tga");
	Loadcoord("OBJ//taser.obj", objList[OBJ_TASER]->dimensions);
	objList[OBJ_TASER]->trs.translate = player->trs.translate;
	objList[OBJ_TASER]->trs.rotateAngles.y = 0;
	objList[OBJ_TASER]->trs.Scale = Vector3(1, 1, 1);
	objList[OBJ_TASER]->type = Obj::OBJ_TYPE::O_WEAPON;
}

void SceneBasic::InitMiscModels()
{
	objList[OBJ_BULLET]->mesh = MeshBuilder::GenerateOBJ("bullet", "OBJ//bullet.obj");
	objList[OBJ_BULLET]->mesh->textureArray[0] = LoadTGA("Image//bullet_texture.tga");
	Loadcoord("OBJ//bullet.obj", objList[OBJ_BULLET]->dimensions);

	objList[OBJ_AMMO_BOX]->mesh = MeshBuilder::GenerateOBJ("ammo_box", "OBJ//ammo_box.obj");
	objList[OBJ_AMMO_BOX]->mesh->textureArray[0] = LoadTGA("Image//ammo_box_texture.tga");
	Loadcoord("OBJ//ammo_box.obj", objList[OBJ_AMMO_BOX]->dimensions);
	objList[OBJ_AMMO_BOX]->type = Obj::OBJ_TYPE::O_ITEMS;

	std::vector<Vector3>ammoBoxCoords = { Vector3(-130, 0, -50), Vector3(70, 0, -410) };
	for (unsigned i = 0; i < ammoBoxCoords.size(); ++i)
	{
		Obj* obj = new Obj(objList[OBJ_AMMO_BOX]);
		obj->trs.translate = ammoBoxCoords[i];
		obj->setToFloor(m_heightMap, objList[OBJ_TERRAIN]);
		PlayerCollidingEntities.push_back(obj);
		objManager->Add(obj);
	}

	objList[OBJ_CAR]->mesh = MeshBuilder::GenerateOBJ("car", "OBJ//car.obj");
	objList[OBJ_CAR]->mesh->textureArray[0] = LoadTGA("Image//car_blue_texture.tga");
	Loadcoord("OBJ//car.obj", objList[OBJ_CAR]->dimensions);
	objList[OBJ_CAR]->type = Obj::OBJ_TYPE::O_OBSTACLES;

	std::vector<Vector3>carsCoords = {
		Vector3(-5, 0, 420), Vector3(-5, 0, 380), Vector3(-30, 0, 260),
		Vector3(-160, 0, -50), Vector3(-180, 0, -140), Vector3(-190, 0, -320),
		Vector3(220, 0, -290) };
	std::vector<float> carsRotation = { 180, 180, -90, -90, 180, 180, 0 };
	for (unsigned i = 0; i < carsCoords.size(); ++i)
	{
		Obj* car = new Obj(objList[OBJ_CAR]);
		car->trs.translate = carsCoords[i];
		car->trs.rotateAngles.y = carsRotation[i];
		car->setToFloor(m_heightMap, objList[OBJ_TERRAIN]);
		PlayerCollidingEntities.push_back(car);
		objManager->Add(car);
	}

	objList[OBJ_POLICE_CAR]->mesh = MeshBuilder::GenerateOBJ("polic_car", "OBJ//car.obj");
	objList[OBJ_POLICE_CAR]->mesh->textureArray[0] = LoadTGA("Image//police_car_texture.tga");
	Loadcoord("OBJ//car.obj", objList[OBJ_POLICE_CAR]->dimensions);
	objList[OBJ_POLICE_CAR]->type = Obj::OBJ_TYPE::O_OBSTACLES;

	std::vector<Vector3>policeCarCoords = {
		Vector3(190, 0, 50), Vector3(50, 0, -60), Vector3(50, 0, -85),
		Vector3(50, 0, -110), Vector3(-20, 0, -350) };
	std::vector<float>policeCarRotation = { 180, 180, 180, 180, 90 };
	for (unsigned i = 0; i < policeCarCoords.size(); ++i)
	{
		Obj* car = new Obj(objList[OBJ_POLICE_CAR]);
		car->trs.translate = policeCarCoords[i];
		car->trs.rotateAngles.y = policeCarRotation[i];
		car->setToFloor(m_heightMap, objList[OBJ_TERRAIN]);
		PlayerCollidingEntities.push_back(car);
		objManager->Add(car);
	}

	objList[OBJ_TRUCK]->mesh = MeshBuilder::GenerateOBJ("truck", "OBJ//truck.obj");
	objList[OBJ_TRUCK]->mesh->textureArray[0] = LoadTGA("Image//truck_texture.tga");
	Loadcoord("OBJ//truck.obj", objList[OBJ_TRUCK]->dimensions);
	objList[OBJ_TRUCK]->type = Obj::OBJ_TYPE::O_OBSTACLES;

	std::vector<Vector3>truckCoords = {
		Vector3(-75, 0, 180), Vector3(-75, 0, 150), Vector3(-75, 0, 20),
		Vector3(210, 0, -390), Vector3(20, 0, -390), Vector3(-30, 0, -390),
		Vector3(270, 0, -210) };
	std::vector<float>truckRotation = { 180, 180, 180, 90, 90, 90, -45 };
	for (unsigned i = 0; i < truckCoords.size(); ++i)
	{
		Obj* truck = new Obj(objList[OBJ_TRUCK]);
		truck->trs.translate = truckCoords[i];
		truck->trs.rotateAngles.y = truckRotation[i];
		truck->setToFloor(m_heightMap, objList[OBJ_TERRAIN]);
		PlayerCollidingEntities.push_back(truck);
		objManager->Add(truck);
	}

	objList[OBJ_BUS]->mesh = MeshBuilder::GenerateOBJ("bus", "OBJ//bus.obj");
	objList[OBJ_BUS]->mesh->textureArray[0] = LoadTGA("Image//bus_texture.tga");
	Loadcoord("OBJ//bus.obj", objList[OBJ_BUS]->dimensions);
	objList[OBJ_BUS]->type = Obj::OBJ_TYPE::O_OBSTACLES;

	std::vector<Vector3>busCoords = {
		Vector3(-40, 0, 130), Vector3(80, 1, 260), Vector3(50, 1, 240),
		Vector3(-75, 0, -60), Vector3(-180, 0, -270), Vector3(-170, 0, -220),
		Vector3(-60, 0, -390), Vector3(80, 0, -390), Vector3(70, 0, -360),
		Vector3(-140, 0, -280), Vector3(240, 0, -80), Vector3(150, 0, -390) };
	std::vector<float>busRotation = { 0, 270, 90, -135, 180, 180, 90, 90, -90, 0, 270, 90 };
	for (unsigned i = 0; i < busCoords.size(); ++i)
	{
		Obj* bus = new Obj(objList[OBJ_BUS]);
		bus->trs.translate = busCoords[i];
		bus->trs.rotateAngles.y = busRotation[i];
		bus->setToFloor(m_heightMap, objList[OBJ_TERRAIN]);
		PlayerCollidingEntities.push_back(bus);
		objManager->Add(bus);
	}

	objList[OBJ_BUILDING_01]->mesh = MeshBuilder::GenerateOBJ("building", "OBJ//building.obj");
	objList[OBJ_BUILDING_01]->mesh->textureArray[0] = LoadTGA("Image//building_texture.tga");
	Loadcoord("OBJ//building.obj", objList[OBJ_BUILDING_01]->dimensions);
	objList[OBJ_BUILDING_01]->type = Obj::OBJ_TYPE::O_OBSTACLES;

	std::vector<Vector3>building01Coords = {
		Vector3(-17.5, 0, 460),
		Vector3(15, 0, 410), Vector3(-50, 0, 390),  Vector3(22.3f, 0, 310),
		Vector3(110, 0, 300), Vector3(11, 0, 280), Vector3(113.5f, 0, 365),
		Vector3(202.5f, 0, 175), Vector3(45, 0, 225), Vector3(110, 0, 92.5f),
		Vector3(-20, 0, 92.5f), Vector3(-90, 0, 118), Vector3(45, 0, -40),
		Vector3(-170, 0, -40), Vector3(-200, 0, -190), Vector3(50, 0, -420),
		Vector3(310, 0, -220), Vector3(255, 0, -15), Vector3(0, 0, -145),
		Vector3(-120, 0, -250), Vector3(200, 0, -275), Vector3(40, 0, -340) };
	std::vector<float>building01Rotation = { 90, 0, 0, 90, 0, 90, 90, 0, 90, 0, 0, 0, 90, 90, 0, 90, 0, 90, 90, 0, 0, 90 };
	std::vector<Vector3>building01Scale = {
		Vector3(1, 2, 1.5f),
		Vector3(1, 2, 2), Vector3(1, 2, 3),  Vector3(1, 2, 3.2f),
		Vector3(1.5f, 2, 0.6f),  Vector3(1, 2, 4.25f), Vector3(1, 2, 3.75f),
		Vector3(1, 2, 7.5f),  Vector3(1, 2, 2.3f),  Vector3(1.5f, 2, 5.5f),
		Vector3(1.5f, 2, 5.5f), Vector3(1, 2, 6.6f), Vector3(1, 2, 2.3f),
		Vector3(1, 2, 3), Vector3(1, 2, 6), Vector3(1, 2, 10.5f),
		Vector3(1, 2, 8), Vector3(1, 2, 2.25f), Vector3(1, 2, 5),
		Vector3(1, 2, 4), Vector3(1, 2, 2.5f), Vector3(1, 2, 6.5f) };
	for (unsigned i = 0; i < building01Coords.size(); ++i)
	{
		Obj* building = new Obj(objList[OBJ_BUILDING_01]);
		building->trs.translate = building01Coords[i];
		building->trs.rotateAngles.y = building01Rotation[i];
		building->trs.Scale = building01Scale[i];
		building->setToFloor(m_heightMap, objList[OBJ_TERRAIN]);
		PlayerCollidingEntities.push_back(building);
		objManager->Add(building);
	}

	objList[OBJ_HOSPITAL]->mesh = MeshBuilder::GenerateOBJ("hospital", "OBJ//hospital.obj");
	objList[OBJ_HOSPITAL]->mesh->textureArray[0] = LoadTGA("Image//hospital_texture.tga");
	Loadcoord("OBJ//hospital.obj", objList[OBJ_HOSPITAL]->dimensions);
	objList[OBJ_HOSPITAL]->trs.Scale = Vector3(2, 2, 2);
	objList[OBJ_HOSPITAL]->type = Obj::OBJ_TYPE::O_OBSTACLES;

	std::vector<Vector3>hospitalCoords = { Vector3(160, 0, -170), Vector3(-220, 0, -385) };
	std::vector<float>hospitalRotation = { -90, 0 };
	for (unsigned i = 0; i < hospitalCoords.size(); ++i)
	{
		Obj* building = new Obj(objList[OBJ_HOSPITAL]);
		building->trs.translate = hospitalCoords[i];
		building->trs.rotateAngles.y = hospitalRotation[i];
		building->setToFloor(m_heightMap, objList[OBJ_TERRAIN]);
		lights[14 + i].position.Set(building->trs.translate.x, building->trs.translate.y + 5.f, building->trs.translate.z);
		if(i == 0)
			lights[14].position.z += 50.f;
		else
			lights[15].position.x += 50.f;
		PlayerCollidingEntities.push_back(building);
		objManager->Add(building);
	}

	objList[OBJ_VICTORY1]->mesh = MeshBuilder::GenerateOBJ("vicotry1", "OBJ//box.obj");
	objList[OBJ_VICTORY1]->mesh->textureArray[0] = LoadTGA("image//brick.tga");
	Loadcoord("OBJ//box.obj", objList[OBJ_VICTORY1]->dimensions);
	objList[OBJ_VICTORY1]->trs.translate.Set(160, 0, -140);
	objList[OBJ_VICTORY1]->trs.Scale.Set(5, 8, 5);
	objManager->Add(objList[OBJ_VICTORY1]);

	objList[OBJ_VICTORY2] = new Obj(objList[OBJ_VICTORY1]);
	//objList[OBJ_VICTORY2]->mesh->textureArray[0] = LoadTGA("image//brick.tga");
	objList[OBJ_VICTORY2]->trs.translate.Set(-190, 0, -385);
	objList[OBJ_VICTORY2]->trs.rotateAngles.y = 90;
	objList[OBJ_VICTORY2]->trs.Scale.Set(5, 8, 5);
	objManager->Add(objList[OBJ_VICTORY2]);

	objList[OBJ_BENCH]->mesh = MeshBuilder::GenerateOBJ("bench", "OBJ//bench.obj");
	objList[OBJ_BENCH]->mesh->textureArray[0] = LoadTGA("Image//bench_texture.tga");
	Loadcoord("OBJ//bench.obj", objList[OBJ_BENCH]->dimensions);
	objList[OBJ_BENCH]->type = Obj::OBJ_TYPE::O_OBSTACLES;

	std::vector<Vector3>hBenchCoords = {
		Vector3(50, 1, 355), Vector3(30, 1, 355),
		Vector3(80, 1, 270), Vector3(100, 1, 270),
		Vector3(80, 1, -410), Vector3(100, 1, -410),
		Vector3(180, 1, -350), Vector3(160, 1, -350) };
	for (unsigned i = 0; i < hBenchCoords.size(); ++i)
	{
		Obj* topBench = new Obj(objList[OBJ_BENCH]);
		topBench->trs.translate = hBenchCoords[i];
		for (unsigned j = 0; j < 2; ++j)
		{
			Obj* bench = new Obj(objList[OBJ_BENCH]);
			if (j % 2 == 0)
			{
				bench->trs.translate = Vector3(hBenchCoords[i].x, 1, hBenchCoords[i].z - 1.9f);
				bench->trs.rotateAngles.x = 90;
			}
			else
			{
				bench->trs.translate = Vector3(hBenchCoords[i].x, 1, hBenchCoords[i].z + 1.9f);
				bench->trs.rotateAngles.x = -90;
			}
			PlayerCollidingEntities.push_back(bench);
			objManager->Add(bench);
		}
		PlayerCollidingEntities.push_back(topBench);
		objManager->Add(topBench);
	}
	std::vector<Vector3>vBenchCoords = {
		Vector3(-30, 1, 110), Vector3(-190, 1, -140),  Vector3(-190, 1, -160),
		Vector3(-130, 1, -200), Vector3(-130, 1, -220), Vector3(210, 1, -270),
		Vector3(210, 1, -290) };
	for (unsigned i = 0; i < vBenchCoords.size(); ++i)
	{
		Obj* topBench = new Obj(objList[OBJ_BENCH]);
		topBench->trs.translate = vBenchCoords[i];
		topBench->trs.rotateAngles.y = 90;
		for (unsigned j = 0; j < 2; ++j)
		{
			Obj* bench = new Obj(objList[OBJ_BENCH]);
			bench->trs.rotateAngles.y = 90;
			if (j % 2 == 0)
			{
				bench->trs.translate = Vector3(vBenchCoords[i].x - 1.9f, 1, vBenchCoords[i].z);
				bench->trs.rotateAngles.z = -90;
			}
			else
			{
				bench->trs.translate = Vector3(vBenchCoords[i].x + 1.9f, 1, vBenchCoords[i].z);
				bench->trs.rotateAngles.z = 90;
			}
			PlayerCollidingEntities.push_back(bench);
			objManager->Add(bench);
		}
		PlayerCollidingEntities.push_back(topBench);
		objManager->Add(topBench);
	}
}
#include "SceneManager.h"
#include "../Global_Constants/GAME_STATES.h"

SceneManager::SceneManager()
{
	currSceneID = STATE::MENU_SCENE;
	nextSceneID = STATE::MENU_SCENE;

	cSoundController = CSoundController::GetInstance();
	cSoundController->Init();
}

SceneManager::~SceneManager()
{
	for (unsigned int i = 0; i < sceneList.size(); i++)
	{
		delete sceneList[i];
	}
	if (cSoundController)
	{
		cSoundController->Destroy();
		cSoundController = NULL;
	}
}

SceneManager* SceneManager::instance = nullptr;

SceneManager* SceneManager::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new SceneManager;
	}
	return instance;
}

void SceneManager::Update(double elapsedTime)
{
	sceneList[currSceneID]->Update(elapsedTime);
	sceneList[currSceneID]->Render();
}

Scene* SceneManager::GetCurrScene()
{
	return sceneList[currSceneID];
}

void SceneManager::AddScene(Scene* scene)
{
	sceneList.push_back(scene);
}

void SceneManager::SetNextScene(int sceneID)
{
	nextSceneID = sceneID;
}

void SceneManager::ChangeScene()
{
	currSceneID = nextSceneID;
}

int SceneManager::GetCurrentSceneID() const
{
	return currSceneID;
}

int SceneManager::GetNextSceneID() const
{
	return nextSceneID;
}
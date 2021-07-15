#pragma once
#include "../Scene.h"
#include "..\SoundController\SoundController.h"
#include <vector>

class SceneManager
{
private:
	static SceneManager* instance;
	CSoundController* cSoundController;
	SceneManager();
	std::vector<Scene*> sceneList;
	int currSceneID;
	int nextSceneID;
public:	
	~SceneManager();

	void Update(double elapsedTime);
	
	static SceneManager* GetInstance();
	
	void AddScene(Scene* scene);
	void SetNextScene(int sceneID);

	Scene* GetCurrScene();
	void ChangeScene();
	
	int GetCurrentSceneID() const;
	int GetNextSceneID() const;
};
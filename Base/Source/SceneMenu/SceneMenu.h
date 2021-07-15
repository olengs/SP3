#pragma once

#include <vector>
#include "../Scene.h"
#include "../Vertex.h"
#include "..\SoundController\SoundController.h"

class SceneMenu : public Scene
{
private:
	enum IMAGE
	{
		MENU_SCREEN,
		OPTIONS_SCREEN,
		ABOUT_SCREEN,
		CREDITS_SCREEN,
		LOADING_SCREEN,
		CURSOR,
		TOTAL_IMAGE
	};
	const float BUTTON_POS[5]; // Y coord
	float cursorPos;
	int currentButton;		   // button where cursor is
	double elapsedTime;
	double debounceTime;
	bool somethingHappened;
	bool renderLoadScreen;

	enum MENU_STATES
	{
		MAIN_MENU_STATE,
		OPTIONS_MENU_STATE,
		ABOUT_MENU_STATE,
		CREDITS_MENU_STATE
	};

	int menuCurrentState;

	Mesh* meshList[TOTAL_IMAGE];
	CSoundController* cSoundController;
public:
	SceneMenu();
	~SceneMenu();

	void Init() override;
	void Update(double dt) override;
	void Render() override;
	void Exit() override;

	void InitMeshList() override;
	void RenderMesh(Mesh* mesh, bool enableLight, bool isMoving = 0, bool isMVP = 1, bool isWave = 0) override;
	void RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizex, float sizey) override;
};
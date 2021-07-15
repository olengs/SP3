#pragma once

#include <vector>
#include "../Scene.h"
#include "../Vertex.h"
#include "..\SoundController\SoundController.h"

class SceneWin : public Scene
{
private:
	enum IMAGE
	{
		WIN_SCREEN,
		TOTAL_IMAGE
	};

	double elapsedTime;

	Mesh* meshList[TOTAL_IMAGE];
	CSoundController* cSoundController;
public:
	SceneWin();
	~SceneWin();

	void Init() override;
	void Update(double dt) override;
	void Render() override;
	void Exit() override;

	void InitMeshList() override;
	void RenderMesh(Mesh* mesh, bool enableLight, bool isMoving = 0, bool isMVP = 1, bool isWave = 0) override;
	void RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizex, float sizey) override;
};
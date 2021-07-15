#include "SceneLose.h"
#include "GL\glew.h"
#include "../Application.h"
#include "../SceneManager/SceneManager.h"
#include "../Global_Constants/GAME_STATES.h"
#include "../shader.hpp"

SceneLose::SceneLose() :
	elapsedTime(0),
	cSoundController(NULL)
{
	for (int i = 0; i < TOTAL_IMAGE; ++i)
		meshList[i] = 0;
}

SceneLose::~SceneLose()
{
	if (cSoundController)
	{
		cSoundController = NULL;
	}
}

void SceneLose::Init()
{
	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	m_programID = LoadShaders("Shader//Shadow.vertexshader", "Shader//Shadow.fragmentshader");

	InitShaderUniforms();

	glUseProgram(m_programID);

	InitMeshList();
}

void SceneLose::Update(double dt)
{
	if(dt < 1.0)
		elapsedTime += dt;

	if (elapsedTime > 3.0)
	{
		SceneManager* scene = SceneManager::GetInstance();
		scene->SetNextScene(STATE::EXIT_SCENE);
	}
}

void SceneLose::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	RenderMeshOnScreen(meshList[LOSE_SCREEN], 40, 30.f, 80.f, 60.f);
}

void SceneLose::Exit()
{
	// Cleanup here
	for (int i = 0; i < TOTAL_IMAGE; ++i)
	{
		if (meshList[i] != NULL)
			delete meshList[i];
	}
	// Cleanup VBO here
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}

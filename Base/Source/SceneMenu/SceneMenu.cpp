#include "SceneMenu.h"
#include "GL\glew.h"
#include "../Application.h"
#include "../SceneManager/SceneManager.h"
#include "../Global_Constants/GAME_STATES.h"
#include "../shader.hpp"

SceneMenu::SceneMenu() :
	cursorPos(29.25f),
	currentButton(0),
	elapsedTime(0),
	debounceTime(0),
	somethingHappened(false),
	renderLoadScreen(false),
	BUTTON_POS{ 29.25f, 25.75f, 22.25f, 18.75f, 15.25f },
	menuCurrentState(0),
	cSoundController(NULL)
{
	for (int i = 0; i < TOTAL_IMAGE; ++i)
		meshList[i] = 0;
}


SceneMenu::~SceneMenu()
{
	if (cSoundController)
	{
		cSoundController = NULL;
	}
}

void SceneMenu::Init()
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

	cSoundController = CSoundController::GetInstance();
	cSoundController->LoadSound("Sounds//Menu//menu_click.wav", 60, true);						//menu click
	cSoundController->LoadSound("Sounds//Menu//myuu-BGM.wav", 61, true , true);					//BGM

	cSoundController->PlaySoundByID(61);
}

void SceneMenu::Update(double dt)
{
	somethingHappened = false;
	elapsedTime += dt;

	if (debounceTime > elapsedTime)
		return;

	switch (menuCurrentState)
	{
	case MAIN_MENU_STATE:
		if (Application::IsKeyPressed(VK_DOWN))
		{
			if (cursorPos != BUTTON_POS[4])
			{
				cursorPos = BUTTON_POS[currentButton + 1];
				++currentButton;
				cSoundController->PlaySoundByID(60);
			}
			somethingHappened = true;
		}
		if (Application::IsKeyPressed(VK_UP))
		{
			if (cursorPos != BUTTON_POS[0])
			{
				cursorPos = BUTTON_POS[currentButton - 1];
				--currentButton;
				cSoundController->PlaySoundByID(60);
			}
			somethingHappened = true;
		}
		if (Application::IsKeyPressed(VK_RETURN))
		{
			cSoundController->PlaySoundByID(60);
			SceneManager* scene = SceneManager::GetInstance();

			switch (currentButton)
			{
			case 0:
				renderLoadScreen = true;
				scene->SetNextScene(STATE::GAME_SCENE);
				break;
			case 1:
				menuCurrentState = OPTIONS_MENU_STATE;
				break;
			case 2:
				menuCurrentState = ABOUT_MENU_STATE;
				break;
			case 3:
				menuCurrentState = CREDITS_MENU_STATE;
				break;
			case 4:
				scene->SetNextScene(STATE::EXIT_SCENE);
				break;
			}
			somethingHappened = true;
		}
		break;
	case OPTIONS_MENU_STATE:
	case ABOUT_MENU_STATE:
	case CREDITS_MENU_STATE:
		if (Application::IsKeyPressed(VK_RETURN))
		{
			menuCurrentState = MAIN_MENU_STATE;
			cSoundController->PlaySoundByID(60);
			somethingHappened = true;
		}
		break;
	}

	if (somethingHappened)
	{
		debounceTime = elapsedTime + 0.2;
	}
}

void SceneMenu::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (renderLoadScreen)
	{
		RenderMeshOnScreen(meshList[LOADING_SCREEN], 40, 30.f, 80.f, 60.f);
	}
	else
	{
		switch (menuCurrentState)
		{
		case MAIN_MENU_STATE:
			RenderMeshOnScreen(meshList[MENU_SCREEN], 40, 30.f, 80.f, 60.f);
			RenderMeshOnScreen(meshList[CURSOR], 9, cursorPos, 2.f, 2.f); 
			break;
		case OPTIONS_MENU_STATE:
			RenderMeshOnScreen(meshList[OPTIONS_SCREEN], 40, 30.f, 80.f, 60.f);
			RenderMeshOnScreen(meshList[CURSOR], 70, 4, 2.f, 2.f);
			break;
		case ABOUT_MENU_STATE:
			RenderMeshOnScreen(meshList[ABOUT_SCREEN], 40, 30.f, 80.f, 60.f);
			RenderMeshOnScreen(meshList[CURSOR], 70, 4, 2.f, 2.f);
			break;
		case CREDITS_MENU_STATE:
			RenderMeshOnScreen(meshList[CREDITS_SCREEN], 40, 30.f, 80.f, 60.f);
			RenderMeshOnScreen(meshList[CURSOR], 70, 4, 2.f, 2.f);
			break;
		}
	}
}

void SceneMenu::Exit()
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
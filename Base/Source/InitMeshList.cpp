#include "SceneMenu/SceneMenu.h"
#include "SceneBasic.h"
#include "SceneWin/SceneWin.h"
#include "SceneLose/SceneLose.h"
#include "GL\glew.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "LoadTexture.h"

void SceneMenu::InitMeshList()
{
	meshList[MENU_SCREEN] = MeshBuilder::GenerateQuad("menu_screen", Color(1, 1, 1), 1.f);
	meshList[MENU_SCREEN]->textureArray[0] = LoadTGA("Image//silent_journey_main_menu.tga");

	meshList[LOADING_SCREEN] = MeshBuilder::GenerateQuad("loading_screen", Color(1, 1, 1), 1.f);
	meshList[LOADING_SCREEN]->textureArray[0] = LoadTGA("Image//loading_screen.tga");

	meshList[OPTIONS_SCREEN] = MeshBuilder::GenerateQuad("loading_screen", Color(1, 1, 1), 1.f);
	meshList[OPTIONS_SCREEN]->textureArray[0] = LoadTGA("Image//silent_journey_controls_screen.tga");
	
	meshList[ABOUT_SCREEN] = MeshBuilder::GenerateQuad("loading_screen", Color(1, 1, 1), 1.f);
	meshList[ABOUT_SCREEN]->textureArray[0] = LoadTGA("Image//silent_journey_about_screen.tga");

	meshList[CREDITS_SCREEN] = MeshBuilder::GenerateQuad("loading_screen", Color(1, 1, 1), 1.f);
	meshList[CREDITS_SCREEN]->textureArray[0] = LoadTGA("Image//silent_journey_credits_screen.tga");

	meshList[CURSOR] = MeshBuilder::GenerateQuad("cursor", Color(1, 1, 1), 1.f);
	meshList[CURSOR]->textureArray[0] = LoadTGA("Image//hazard_symbol.tga");
}

void SceneBasic::InitMeshList()
{
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference");
	meshList[GEO_CROSSHAIR] = MeshBuilder::GenerateCrossHair("crosshair");
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureArray[0] = LoadTGA("Image//bitmaptext2.tga");
	meshList[GEO_TEXT]->material.kAmbient.Set(1, 0, 0);
	meshList[GEO_OBJECT] = MeshBuilder::GenerateOBJ("OBJ1", "OBJ//chair.obj");
	meshList[GEO_OBJECT]->textureArray[0] = LoadTGA("Image//chair.tga");
	meshList[GEO_RING] = MeshBuilder::GenerateRing("ring", Color(1, 0, 1), 36, 1, 0.5f);
	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightball", Color(1, 1, 1), 18, 36, 1.f);
	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("sphere", Color(1, 0, 0), 18, 36, 10.f);
	meshList[GEO_CONE] = MeshBuilder::GenerateCone("cone", Color(0.5f, 1, 0.3f), 36, 10.f, 10.f);
	meshList[GEO_CONE]->material.kDiffuse.Set(0.99f, 0.99f, 0.99f);
	meshList[GEO_CONE]->material.kSpecular.Set(0.f, 0.f, 0.f);

	//Shadow depth quad for debugging
	meshList[GEO_LIGHT_DEPTH_QUAD] = MeshBuilder::GenerateQuad("LIGHT DEPTH TEX", Color(1, 1, 1), 1.f);
	meshList[GEO_LIGHT_DEPTH_QUAD]->textureArray[0] = m_lightDepthFBO.GetTexture();

	//meshList[GEO_MINIMAP_QUAD] = MeshBuilder::GenerateCircle("MINIMAP", Color(1.f, 1.f, 1.f), 32, 1.f);
	meshList[GEO_CIRCLE] = MeshBuilder::GenerateCircle("circle", Color(1.f, 1.f, 1.f), 32, 1.f);
	meshList[GEO_MINIMAP_QUAD] = MeshBuilder::GenerateQuad("MINIMAP", Color(1.f, 1.f, 1.f), 1.f);
	meshList[GEO_MINIMAP_QUAD]->textureArray[0] = minimap_FBO.GetTexture();

	meshList[GEO_FRONT_QUAD] = MeshBuilder::GenerateQuad("Front camera", Color(1.f, 1.f, 1.f), 1.f);
	meshList[GEO_FRONT_QUAD]->textureArray[0] = m_FrontFBO.GetTexture();

	meshList[GEO_PLAYER_CURSOR] = MeshBuilder::GenerateQuad("player cursor", Color(1, 1, 1), 0.5f);
	meshList[GEO_PLAYER_CURSOR]->textureArray[0] = LoadTGA("Image//navigation_arrow.tga");

	meshList[GEO_ENEMY_CURSOR] = MeshBuilder::GenerateQuad("enemy cursor", Color(1, 0, 0), 1.f);
	meshList[GEO_ENEMY_CURSOR]->textureArray[0] = LoadTGA("Image//enemy_navigator.tga");

	meshList[GEO_CIRCLE_MINIMAP] = MeshBuilder::GenerateCircle("circle", Color(1, 1, 1), 40, 1.f);
	meshList[GEO_CIRCLE_MINIMAP]->textureArray[0] = minimap_FBO.GetTexture();

	meshList[GEO_MAINGUI] = MeshBuilder::GenerateQuad("MAINGUI", Color(1, 1, 1), 1.f);
	meshList[GEO_MAINGUI]->textureArray[0] = LoadTGA("Image//GUImainscreen2.tga");
}

void SceneWin::InitMeshList()
{
	meshList[WIN_SCREEN] = MeshBuilder::GenerateQuad("win_screen", Color(1, 1, 1), 1.f);
	meshList[WIN_SCREEN]->textureArray[0] = LoadTGA("Image//silent_journey_win_screen.tga");
}

void SceneLose::InitMeshList()
{
	meshList[LOSE_SCREEN] = MeshBuilder::GenerateQuad("lose_screen", Color(1, 1, 1), 1.f);
	meshList[LOSE_SCREEN]->textureArray[0] = LoadTGA("Image//silent_journey_lose_screen.tga");
}
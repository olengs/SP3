#ifndef SCENE_BASIC_H
#define SCENE_BASIC_H

#include "Scene.h"
#include "Mtx44.h"
#include "Camera2.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include <vector>
#include "Camera3.h"
#include "Player.h"
#include "Loadcornercoord.h"
#include "Obj.h"
#include "SpriteAnimation.h"
#include "Particle.h"
#include "PartcleSystemManager.h"
#include "DepthFBO.h"
#include <TemplatedFunc.h>
#include "Collision_detector.h"
#include "SoundController/SoundController.h"
#include "ObjManager.h"
#include "Enemy.h"

class SceneBasic : public Scene
{
	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_CROSSHAIR,
		GEO_LIGHTBALL,
		GEO_SPHERE,
		GEO_QUAD,
		GEO_CUBE,
		GEO_RING,
		GEO_CONE,
		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_FRONT,
		GEO_BACK,
		GEO_GRASS_DARKGREEN,
		GEO_GRASS_LIGHTGREEN,
		GEO_OBJECT,
		GEO_TEXT,
		GEO_PLAYER,
		GEO_HEARTS,
		GEO_HPBAR,
		GEO_HPBAR_BACK,
		GEO_TREE,
		GEO_TV,
		//TSL
		GEO_WATER,
		GEO_SKYPLANE,
		GEO_TERRAIN,
		GEO_SKYDOME,
		GEO_SPRITEANIM,
		GEO_HOUSE,
		GEO_HOUSE2,
		//Particle
		GEO_QUAD_PART,
		GEO_FRONT_QUAD,
		//shadow
		GEO_LIGHT_DEPTH_QUAD,
		GEO_MINIMAP_QUAD,
		GEO_PLAYER_CURSOR,

		GEO_ENEMY_CURSOR,
		GEO_CIRCLE,

		GEO_CIRCLE_MINIMAP,

		GEO_AMMO_BAR_FRONT,
		GEO_AMMO_BAR_BACK,

		GEO_MAINGUI,

		NUM_GEOMETRY,
	};

	enum obj_type //all objects
	{ 
		OBJ_TERRAIN,
		OBJ_SKYDOME,
		OBJ_PLAYER,
		OBJ_HOUSE,
		OBJ_POLICE,
		OBJ_CIVILIAN_A_HEAD_A, // CIVILIAN A has same body but different heads
		OBJ_CIVILIAN_A_HEAD_B,
		OBJ_CIVILIAN_A_UPPERBODY,
		OBJ_CIVILIAN_A_LOWERBODY,
		OBJ_CIVILIAN_A,
		OBJ_BOX,
		OBJ_MASK_GREEN,

		OBJ_SNIPER,
		OBJ_SHOTGUN,
		OBJ_PISTOL,
		OBJ_TASER,

		OBJ_AMMO_BOX,
		OBJ_BULLET,
		OBJ_FLASHLIGHT,

		OBJ_CAR,
		OBJ_POLICE_CAR,
		OBJ_BUS,
		OBJ_TRUCK,
		OBJ_BUILDING_01,
		OBJ_BUILDING_02,
		OBJ_HOSPITAL,
		OBJ_BENCH,

		OBJ_VICTORY1,
		OBJ_VICTORY2,

		TOTAL_OBJ,
	};

	enum PLAYER_STATE {
		WALK = 0,
		JUMP,
	};

public:
	SceneBasic();
	~SceneBasic();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void InitMeshList() override;
	void RenderMesh(Mesh* mesh, bool enableLight, bool isMoving = 0, bool isMVP = 1, bool isWave = 0) override;
	void RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizex, float sizey) override;

	void InitShadowShaderUniforms();

	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMeshIn2D(Mesh *mesh, bool enableLight, float size=1.0f, float x=0.0f, float y=0.0f);
	void RenderTerrain();
	void RenderSkyDome();
	void RenderObj(Mesh* mesh, TRS trs, bool enableLight, bool isUVmoving = 0, bool end = 1, bool isMVP = 1, bool isWave = 0);
	void RenderObj(Obj* obj, bool enableLight, bool end = 1, bool isUVmoving = 0, bool isMVP = 1, bool isWave = 0);
	//void RenderAnimation(SpriteAnimation* mesh);
	void RenderParticle();
	void RenderPassGPass();
	void RenderPassMain();
	void RenderPassMinimap();
	void RenderPassFront();
	void RenderWorld();

	void InitEnvironment();
	void InitEnemyModels();
	void InitWeaponModels();
	void InitMiscModels();
	void InitWeapons();
	void InitLightSettings();
	void InitLightShaderUniforms();
	void PassLightInfoToShader();
	void InitSounds();

	enum RenderPass {
		RENDERPASS_PRE,
		RENDERPASS_MINIMAP,
		RENDERPASS_MAIN,
	};

	//Stealth
	bool CheckDistance(Obj* source, Obj* destination, float distance);
	bool WithinRadius(Obj* source, Obj* destination, float angle);
	bool LineOfSight(Obj* source, Obj* destination);
	void RenderGun(Obj* gun);

private:
	Mesh* meshList[NUM_GEOMETRY];
	Obj* objList[TOTAL_OBJ];
	std::vector<Enemy*> enemyList;
	std::vector<Obj*> PlayerCollidingEntities;
	std::vector<Projectile*> projectileList;
	std::vector<Obj*> flashlights;
	std::vector<Obj*> masks;
	//Camera2 camera;
	Camera3* camera;
	Player* player;

	CSoundController* cSoundController;
	ObjManager* objManager;

	const int NUM_OF_LIGHTS;

	Light lights[16];

	bool bLightEnabled;

	float fps;
	bool b_texturemove;
	float moveValue;

	float fog_ftime;
	bool fog_active;
	float fog_bouncetime;
	float fog_color_ftime;
	int fog_colorChange;

	std::vector<unsigned char> m_heightMap;

	float m_speed;
	float m_worldwidth;
	float m_worldHeight;
	Vector3 m_gravity;

	ParticleSystemManager* ParticleSystems;
	//shadow
	DepthFBO m_lightDepthFBO;
	DepthFBO minimap_FBO;
	DepthFBO m_FrontFBO;
	RenderPass mRenderPass;
	unsigned m_gPassShaderID;
	unsigned m_miniMapPassShaderID;

	Mtx44 m_lightDepthProj;
	Mtx44 m_lightDepthView;

	Mtx44 m_minimapProj;
	Mtx44 m_minimapView;

	ParticleSystem* firework;


	//player restore point
	Vector3 playerResPoint;

	//bool renderItemPopUp;
	bool bigScreenMinimap;

	bool renderEnemyModels;

	bool v1bool;
	bool v2bool;
	bool collidingv1;
	bool collidingv2;
	float textbouncetime;
	bool isv1orv2cleared;
};

#endif
#ifndef SCENE_H
#define SCENE_H

#include "UNIFORM_TYPE.h"
#include "MatrixStack.h"
#include "Mesh.h"

class Scene
{
protected:
	MS modelStack;
	MS viewStack;
	MS projectionStack;

	unsigned m_programID;
	unsigned m_vertexArrayID;
	unsigned m_parameters[U_TOTAL];
public:
	Scene()
	{
		m_programID = 0;
		m_vertexArrayID = 0;
		for (int i = 0; i < U_TOTAL; ++i)
			m_parameters[i] = 0;
	}
	~Scene() {}

	virtual void Init() = 0;
	virtual void Update(double dt) = 0;
	virtual void Render() = 0;
	virtual void Exit() = 0;

	void InitShaderUniforms();

	virtual void InitMeshList() = 0;

	virtual void RenderMesh(Mesh* mesh, bool enableLight, bool isMoving = 0, bool isMVP = 1, bool isWave = 0) = 0;
	virtual void RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizex, float sizey) = 0;

	// probably SHOULD NOT BE USED because MeshOnScreen() is more accurate?
	virtual void RenderMeshIn2D(Mesh* mesh, bool enableLight, float size, float x, float y, float z);
};

#endif
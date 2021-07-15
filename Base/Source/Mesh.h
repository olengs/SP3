#ifndef MESH_H
#define MESH_H
#define MAX_TEXTURES 8
#include <string>
#include "Material.h"

class Mesh
{
public:
	enum DRAW_MODE
	{
		DRAW_TRIANGLES, //default mode
		DRAW_TRIANGLE_STRIP,
		DRAW_LINES,
		DRAW_MODE_LAST,
	};
	Mesh(const std::string &meshName);
	~Mesh();
	Mesh(Mesh* mesh);
	Mesh(Mesh* mesh, const std::string &meshName);
	void Render();
	virtual void Render(unsigned offset, unsigned count);

	const std::string name;
	DRAW_MODE mode;
	unsigned vertexBuffer;
	unsigned indexBuffer;
	unsigned indexSize;
	float moveVal;
	float moveVal_increment;

	Material material;
	unsigned textureArray[MAX_TEXTURES];
};

#endif
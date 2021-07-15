#ifndef MESH_BUILDER_H
#define MESH_BUILDER_H

#include "Mesh.h"
#include "Vertex.h"
#include <vector>
#include "LoadHmap.h"
#include <TRS.h>
#include "SpriteAnimation.h"

/******************************************************************************/
/*!
		Class MeshBuilder:
\brief	Provides methods to generate mesh of different shapes
*/
/******************************************************************************/
class MeshBuilder
{
public:
	static Mesh* GenerateAxes(const std::string &meshName, float lengthX=0.0f, float lengthY=0.0f, float lengthZ=0.0f);
	static Mesh* GenerateCrossHair(const std::string &meshName, float colour_r=1.0f, float colour_g=1.0f, float colour_b=0.0f, float length=1.0f);
	static Mesh* GenerateQuad(const std::string &meshName, Color color, float length = 1.f);
	static Mesh* GenerateCube(const std::string &meshName, Color color, float length = 1.f);
	static Mesh* GenerateRing(const std::string &meshName, Color color, unsigned numSlice, float outerR = 1.f, float innerR = 0.f);
	static Mesh* GenerateSphere(const std::string &meshName, Color color, unsigned numStack, unsigned numSlice, float radius = 1.f);
	static Mesh* GenerateCone(const std::string &meshName, Color color, unsigned numSlice, float radius, float height);
	static Mesh* GenerateOBJ(const std::string &meshName, const std::string &file_path);
	static Mesh* GenerateText(const std::string &meshName, unsigned row, unsigned col);
//	static Mesh* GenerateSprite(const std::string& meshName, vector<unsigned char>SpirteArr, unsigned row, unsigned col);
	//TSL
	static Mesh* GenerateSkyPlane(const std::string &meshName, Color color, int slices,float PlanetRadius, float AtmosphereRadius, float hTile, float vTile);
	static Mesh* GenerateTerrain(const std::string& meshName, const std::string& filename, std::vector<unsigned char>& heightMap);
	static Mesh* GenerateTerrain(const std::string& meshName, const std::string& filename, std::vector<unsigned char>& heightMap, float hTile, float vTile);
	static Mesh* GenerateTerrain(const std::string& meshName, const std::string& filename, std::vector<unsigned char>& heightMap, TRS terrain);
	static Mesh* GenerateSkyDome(const std::string& meshName, Color color, unsigned numStack, unsigned numSlice, float radius);
	static Mesh* GenerateSlicedQuad(const std::string& meshName, Color color, float length, unsigned numRow, unsigned numCol);
	static SpriteAnimation* GenerateSpriteAnimation(const std::string& meshName, unsigned numRow, unsigned numCol, float length);
	static Mesh* GenerateCircle(const std::string& meshName, Color color, unsigned numSlice, float radius);
};

#endif
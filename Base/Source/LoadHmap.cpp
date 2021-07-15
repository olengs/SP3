#include <iostream>
#include <fstream>
#include "LoadHmap.h"

bool LoadHeightMap(const char *file_path, std::vector<unsigned char> &heightMap)
{
	std::ifstream fileStream(file_path, std::ios::binary);
	if(!fileStream.is_open())
	{
		std::cout << "Impossible to open " << file_path << ". Are you in the right directory ?\n";
		return false;
	}

    fileStream.seekg(0, std::ios::end);
    std::streampos fsize = (unsigned)fileStream.tellg();
	
	fileStream.seekg(0, std::ios::beg);
	heightMap.resize((unsigned)fsize);
	fileStream.read((char *)&heightMap[0], fsize);
	
	fileStream.close();
	return true;
}

float ReadHeightMap(std::vector<unsigned char> &heightMap, float x, float z)
{
	if (x <= -0.5f || x >= 0.5f || z <= -0.5f || z >= 0.5f)
		return 0;
	if (heightMap.size() == 0)
		return 0;

	unsigned terrainSize = (unsigned)sqrt((double)heightMap.size());

	unsigned zCoord = (unsigned)((z + 0.5f) * terrainSize);
	unsigned xCoord = (unsigned)((x + 0.5f) * terrainSize);

	return (float)heightMap[zCoord * terrainSize + xCoord] / 256.f;

}

float ReadHeightMap(std::vector<unsigned char>& heightMap, TRS pos,TRS terrain)
{
	//float blockwidth, blockheight;
	//blockwidth = blockheight = 1.f / 256.f;
	pos.translate.x /= terrain.Scale.x;
	pos.translate.z /= terrain.Scale.z;
	if (pos.translate.x <= -0.5f || pos.translate.x >= 0.5f || pos.translate.z <= -0.5f || pos.translate.z >= 0.5f)
		return 0;
	if (heightMap.size() == 0)
		return 0;

	unsigned terrainSize = (unsigned)sqrt((double)heightMap.size());

	unsigned zCoord = (unsigned)((pos.translate.z + 0.5f) * terrainSize);
	unsigned xCoord = (unsigned)((pos.translate.x + 0.5f) * terrainSize);
	Vector3 position , BL , TL , BR , TR;
	position.Set((float)xCoord, 0.f, (float)zCoord);
	float /*height,*/ c;
	//check block's index
	//find coord points around blocks - BL ( BOTTOM LEFT ) , BR ( BOTTOM RIGHT) , TL ( TOP LEFT ) , TR ( TOP RIGHT )
	BL.Set((float)floor(xCoord), 0.f, (float)floor(zCoord));
	TL = BL + Vector3(0, 0, 1);
	TR = BL + Vector3(1, 0, 1);
	BR = BL + Vector3(1, 0, 0);

	if (BL.x >= 255 || BL.z >= 255) {
		return 0;
	}

	BL.y = (float)heightMap[(unsigned)BL.z * terrainSize + (unsigned)BL.x] / 256.f;
	TR.y = (float)heightMap[(unsigned)TR.z * terrainSize + (unsigned)TR.x] / 256.f;
	//find const to cross line
	c = BL.z - BL.x;
	//check for tri 1 or tri 2
	if (zCoord >= xCoord + c) {
		TL.y = (float)heightMap[(unsigned)TL.z * terrainSize + (unsigned)TL.x] / 256.f;
		return barryCentricEqn(BL, TL, TR, position) * terrain.Scale.y;
	}
	else {
		BR.y = (float)heightMap[(unsigned)BR.z * terrainSize + (unsigned)BR.x] / 256.f;
		return barryCentricEqn(BL, BR, TR, position) * terrain.Scale.y;
	}
}

float barryCentricEqn(Vector3 p1, Vector3 p2, Vector3 p3, Vector3 pos)
{
	float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
	float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.z - p3.z)) / det;
	float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.z - p3.z)) / det;
	float l3 = 1.0f - l1 - l2;
	return l1 * p1.y + l2 * p2.y + l3 * p3.y;
}

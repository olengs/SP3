#ifndef LOAD_HMAP_H
#define LOAD_HMAP_H

#include <vector>
#include <TRS.h>
bool LoadHeightMap(const char *file_path, std::vector<unsigned char> &heightMap);

float ReadHeightMap(std::vector<unsigned char> &heightMap, float x, float z);

float ReadHeightMap(std::vector<unsigned char>& heightMap, TRS pos,TRS terrain);


float barryCentricEqn(Vector3 p1, Vector3 p2, Vector3 p3, Vector3 pos);

#endif
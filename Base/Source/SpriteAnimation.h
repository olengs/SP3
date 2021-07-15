#pragma once
#include <iostream>
#include "Mesh.h"
#include <unordered_map>
#include "Animation.h"
#include <MyMath.h>
#include <GL\glew.h>
#include "Vertex.h"
#include <cstdarg>


class SpriteAnimation :
	public Mesh
{
public:
	SpriteAnimation(const std::string& meshName, int row, int col);
	~SpriteAnimation();
	//Update the animated sprite
	void Update(double dt);
	virtual void Render();
	void AddAnimation(std::string name, int start, int end);
	template <typename size_t N>
	void AddSequeneAnimation(std::string name, int(&args)[N]);
	void PlayAnimation(std::string name, int repeat, float time);
	void Pause();
	void Resume();
	void Reset();
	//number of rows
	int row;
	//number of columns
	int col;
	//the current time of the animation
	float currentTime;
	//the current frame of the animation
	int currentFrame;
	//How many time it has play
	int playCount;
	//The name of the current animation
	std::string currentAnimation;
	//The list of the animations with their names
	std::unordered_map<std::string, Animation*> animationList;
};
template <typename size_t N>
inline void SpriteAnimation::AddSequeneAnimation(std::string name, int(&args)[N])
{
	Animation* anim = new Animation();
	//Add the frames based on the input
	for (int i = 0; i < N; ++i)
	{
		anim->AddFrame(args[i]);
	}
	//Link the animation to the animation list
	animationList[name] = anim;
	//Set the current animation if it does not exisit
	if (currentAnimation == "")
	{
		currentAnimation = name;
	}
	animationList[name]->animActive = false;
}
#pragma once
#include <iostream>
#include <vector>
struct Animation
{
	//Constructor
	Animation() :repeatCount(0),
		animTime(0.0f),
		ended(false),
		animActive(false)
	{
	}
	//repeat count, total animation time,
	void Set(int repeat, float time, bool active)
	{
		this->repeatCount = repeat;
		this->animTime = time;
		this->animActive = active;
	}
	//Adding frames to the frame vector
	void AddFrame(int i)
	{
		frames.push_back(i);
	}
private:
	//name of the animation
	std::string animationName;
	//The frames
	std::vector<int> frames;
	//How many times to repeat
	int repeatCount;
	//The animation time
	float animTime;
	//Has the animation ended
	bool ended;
	//Is the animation active
	bool animActive;
	//Allow the Sprite Animation class to use private variable
	friend class SpriteAnimation;
};
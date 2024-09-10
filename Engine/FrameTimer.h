#pragma once
#include <chrono>

class FrameTimer
{
public:
	FrameTimer();
	void Initialize();
	float Mark();
	float GetCurrentFPS();
	float GetAverageFPS();
	float GetDeltaTime();
	
private:
	int frameCount;
	std::chrono::steady_clock::time_point startTime;
	std::chrono::steady_clock::time_point lastTime;
	float currentFPS;
	float averageFPS;
	float deltaTime;

};
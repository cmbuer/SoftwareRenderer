#include "FrameTimer.h"

using namespace std::chrono;

FrameTimer::FrameTimer()
{
}

void FrameTimer::Initialize() {
	frameCount = 0;
	startTime = lastTime = steady_clock::now();
}

float FrameTimer::Mark()
{
	frameCount++;
	auto old = lastTime;
	lastTime = steady_clock::now();
	duration<float> frameDuration = lastTime - old;
	deltaTime = frameDuration.count();
	auto totalTime = lastTime - startTime;
	float total = (float) totalTime.count() / 1000000000;
	currentFPS = 1 / deltaTime;
	averageFPS = frameCount / total;
	
	return deltaTime;
}

float FrameTimer::GetCurrentFPS() {
	return currentFPS;
}

float FrameTimer::GetAverageFPS() {
	return averageFPS;
}

float FrameTimer::GetDeltaTime() {
	return deltaTime;
}
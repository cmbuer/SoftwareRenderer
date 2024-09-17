#pragma once



enum class RenderMode { Software, Direct3D, OpenGL };

struct ApplicationData {
	static const int screenWidth = 1600;
	static const int screenHeight = 1200;
};


#define GLEW_STATIC

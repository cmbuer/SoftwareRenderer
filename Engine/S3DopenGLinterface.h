#pragma once
#include "Surface.h"
#include "ApplicationData.h"
#include "GL\glew.h"
#include <GL/GL.h>

#include <fstream>
#include <sstream>

#include "..\imgui\imgui_impl_win32.h"
#include "..\imgui\imgui_impl_opengl3.h"

namespace Software3D {

	class OpenGLinterface {

	private:

		float textureVertices[6 * 5] = {
		-1.0f,1.0f,0.5f,0.0f,0.0f ,
		 1.0f,1.0f,0.5f,1.0f,0.0f ,
		 1.0f,-1.0f,0.5f,1.0f,1.0f ,
		-1.0f,1.0f,0.5f,0.0f,0.0f ,
		 1.0f,-1.0f,0.5f,1.0f,1.0f ,
		 -1.0f,-1.0f,0.5f,0.0f,1.0f, };

		HDC ourWindowHandleToDeviceContext;
		Surface buffer;

	public:

		OpenGLinterface() = default;
		OpenGLinterface(HWND windowHandle);
		GLuint CompileShader(unsigned int type, const std::string& source);
		std::string GetShaderSource(std::string filename);
		GLuint CreateShader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
		void EndFrame();

	};
}
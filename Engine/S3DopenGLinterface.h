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

		float textureVertices[6 * 5] = {
		-1.0f,1.0f,0.5f,0.0f,0.0f ,
		 1.0f,1.0f,0.5f,1.0f,0.0f ,
		 1.0f,-1.0f,0.5f,1.0f,1.0f ,
		-1.0f,1.0f,0.5f,0.0f,0.0f ,
		 1.0f,-1.0f,0.5f,1.0f,1.0f ,
		 -1.0f,-1.0f,0.5f,0.0f,1.0f, };

	public:

		HDC ourWindowHandleToDeviceContext;

		Surface buffer;

		OpenGLinterface() = default;
		OpenGLinterface (HWND windowHandle) : buffer (ApplicationData::screenWidth, ApplicationData::screenHeight) {

			PIXELFORMATDESCRIPTOR pfd =
			{
				sizeof(PIXELFORMATDESCRIPTOR),
				1,
				PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    // Flags
				PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
				32,                   // Colordepth of the framebuffer.
				0, 0, 0, 0, 0, 0,
				0,
				0,
				0,
				0, 0, 0, 0,
				24,                   // Number of bits for the depthbuffer
				8,                    // Number of bits for the stencilbuffer
				0,                    // Number of Aux buffers in the framebuffer.
				PFD_MAIN_PLANE,
				0,
				0, 0, 0
			};


			ourWindowHandleToDeviceContext = GetDC(windowHandle);

			int  letWindowsChooseThisPixelFormat;
			letWindowsChooseThisPixelFormat = ChoosePixelFormat(ourWindowHandleToDeviceContext, &pfd);
			SetPixelFormat(ourWindowHandleToDeviceContext, letWindowsChooseThisPixelFormat, &pfd);

			HGLRC ourOpenGLRenderingContext = wglCreateContext(ourWindowHandleToDeviceContext);
			wglMakeCurrent(ourWindowHandleToDeviceContext, ourOpenGLRenderingContext);

			glewInit();

			glFrontFace(GL_CW);
			glEnable(GL_CULL_FACE);



		}

		GLuint CompileShader(unsigned int type, const std::string& source) {
			GLuint id = glCreateShader(type);
			const char* src = source.c_str();        
			glShaderSource(id, 1, &src, nullptr);			
			glCompileShader(id);

			int result;
			glGetShaderiv(id, GL_COMPILE_STATUS, &result);
			if (result == GL_FALSE) {
				int length;
				glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
				char* message = (char*)alloca(length * sizeof(char));
				glGetShaderInfoLog(id, length, &length, message);
				std::cout << "Failed to compile shader!" << std::endl;
				std::cout << message << std::endl;

				MessageBoxA(0, message, "FAILED TO COMPILE SHADER", 0);

				glDeleteShader(id);
				return 0; // indicate failure
			}


			return id;
		}



		std::string GetShaderSource(std::string filename) {
			std::ifstream infile(filename);
			std::stringstream source;
			source << infile.rdbuf();			
			std::string slurp = source.str();
			return source.str();
		}

		GLuint CreateShader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource) {
			
			int x = 10;

			x++;
			GLuint program = glCreateProgram();
			GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
			GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

			glAttachShader(program, vertexShader);
			glAttachShader(program, fragmentShader);
			glLinkProgram(program);
			glValidateProgram(program);

			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			return program;			
		}

		void DrawLegacy(); 

		void EndFrame() {
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			ImGui_ImplOpenGL3_Shutdown();
			
			SwapBuffers(ourWindowHandleToDeviceContext);
		}


	};
}
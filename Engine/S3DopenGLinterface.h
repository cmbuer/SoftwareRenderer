#pragma once
#include "Surface.h"
#include "ApplicationData.h"
#include "GL\glew.h"
#include <GL/GL.h>
//#include "MenuSystem.h"

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
			
			std::cout << "test";
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

		void DrawLegacy() {

			ImGui_ImplOpenGL3_Shutdown();
			

			glClear(GL_COLOR_BUFFER_BIT);
			
			
			glBegin(GL_TRIANGLES);
			glVertex2f(-0.5f, -0.5f);
			glVertex2f(0.0f, 0.5f);
			glVertex2f(0.5f, -0.5f);
			glEnd();
			

			/*

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();
			ImGui::ShowDemoWindow();
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			ImGui_ImplOpenGL3_Shutdown();
			*/

			SwapBuffers(ourWindowHandleToDeviceContext);
		}


		void DrawBufferLegacyTest(const Surface& texture) {

			glClear(GL_COLOR_BUFFER_BIT);

			glBegin(GL_TRIANGLES);
			glVertex2f(-0.5f, -0.5f);
			glVertex2f(0.0f, 0.5f);
			glVertex2f(0.5f, -0.5f);
			glEnd();


			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			ImGui::ShowDemoWindow();
			ImGui::Render();

			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			ImGui_ImplOpenGL3_Shutdown();

			SwapBuffers(ourWindowHandleToDeviceContext);
		}


		void DrawBuffer(const Surface& texture) {
			GLuint textureID;
			glGenTextures(1, &textureID);
			glBindTexture(GL_TEXTURE_2D, textureID);


			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texture.GetWidth(), texture.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, texture.GetRGBApointerConst());

			glActiveTexture(GL_TEXTURE0);

			std::string vertexShaderSource = GetShaderSource("GLtextureVertexShader.glsl");
			std::string fragmentShaderSource = GetShaderSource("GLtextureFragmentShader.glsl");

			GLuint shaders = CreateShader(vertexShaderSource, fragmentShaderSource);
			glUseProgram(shaders);

			int uTextureLocation = glGetUniformLocation(shaders, "u_Texture");
			assert(uTextureLocation != -1);
			glUniform1i(uTextureLocation, 0);


			float vertices[] = { -0.5f, -0.5f, 0.0f, 0.0f,
								 -0.5f,  0.5f, 0.0f, 1.0f,
								  0.5f,	 0.5f, 1.0f, 1.0f,
								  0.5f, -0.5f, 1.0f, 0.0f };

			unsigned int indices[] = {
										0, 1, 2,
										2, 3, 0
			};

			unsigned int vertexBufferID;
			glGenBuffers(1, &vertexBufferID);
			glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (const void*)(sizeof(float) * 2));
			glEnableVertexAttribArray(1);



			unsigned int indexBufferID;
			glGenBuffers(1, &indexBufferID);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


			glClear(GL_COLOR_BUFFER_BIT);

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
			
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			ImGui::ShowDemoWindow();
			//MenuSystem::MainMenu();
			ImGui::Render();

			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			ImGui_ImplOpenGL3_Shutdown();
			
			SwapBuffers(ourWindowHandleToDeviceContext);

			// unbind texture
			glBindTexture(GL_TEXTURE_2D, 0);
			glDeleteTextures(1, &textureID);
		}


		void DrawBufferOriginal(const Surface& texture) {
		

			const float vertices[] =
			{
				 -1.0f,1.0f,0.5f,1.0,0.0f,0.0f ,
				 1.0f,1.0f,0.5f,1.0,1.0f,0.0f ,
				 1.0f,-1.0f,0.5f,1.0,1.0f,1.0f ,
				 -1.0f,1.0f,0.5f,1.0,0.0f,0.0f ,
				 1.0f,-1.0f,0.5f,1.0,1.0f,1.0f ,
				 -1.0f,-1.0f,0.5f,1.0,0.0f,1.0f ,
			};


			
			Matrix4x4 transformation = Matrix4x4::Identity();
			
			GLuint textureID;
			glGenTextures(1, &textureID);
			glBindTexture(GL_TEXTURE_2D, textureID);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texture.GetWidth(), texture.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, texture.GetRGBApointerConst());

			glActiveTexture(GL_TEXTURE0);

			std::string vertexShaderSource = GetShaderSource("GLtextureVertexShader.glsl");
			std::string fragmentShaderSource = GetShaderSource("GLtextureFragmentShader.glsl");

			GLuint shaders = CreateShader(vertexShaderSource, fragmentShaderSource);
			glUseProgram(shaders);

			int uTextureLocation = glGetUniformLocation(shaders, "u_Texture");
			//			assert(uTextureLocation != -1);
			glUniform1i(uTextureLocation, 0);

			unsigned int vertexBufferID;
			glGenBuffers(1, &vertexBufferID);
			glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
			glBufferData(GL_ARRAY_BUFFER, 6 * (6*sizeof(float)), vertices, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (const void*)(sizeof(float)*4));
			glEnableVertexAttribArray(1);


			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			ImGui::ShowDemoWindow();
			ImGui::Render();

			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			ImGui_ImplOpenGL3_Shutdown();

			SwapBuffers(ourWindowHandleToDeviceContext);		
		}
	};
}
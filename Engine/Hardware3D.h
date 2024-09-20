#pragma once
#include <wrl.h>
#include <d3d11.h>
#include <memory>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include "S3Dtypes.h"
#include "ApplicationData.h"
#include "GL\glew.h"
#include <GL/GL.h>
#include <fstream>
#include <sstream>
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "Surface.h"

#include "..\imgui\imgui_impl_win32.h"
#include "..\imgui\imgui_impl_opengl3.h"

#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glew32s.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3Dcompiler.lib")

using namespace Software3D;

namespace Hardware3D {

	class Direct3Dpipeline {
	public:

		enum class WindingDirection { Clockwise = false, Counterclockwise = true };

		Direct3Dpipeline(HWND windowHandle);		
		void BeginFrame();
		void EndFrame();
		void ClearBuffer(float r, float g, float b);		
		void BindConstantBuffer(DirectX::XMMATRIX bufferData);
		void SetWindingDirection(WindingDirection newDirection);
		void SetCullState(D3D11_CULL_MODE newCullMode);
		void CreateResources(const IndexedList<TextureVertex>& vertices, const Surface& texture);
		void CreateTexture(const Surface& texture);
		void CreateShaders();
		void CreateBuffers(const IndexedList<TextureVertex>& vertices);
		void Bind();
		void Draw(const IndexedList<TextureVertex>& vertices, const Surface& texture);

	private:

		DirectX::XMMATRIX projection;
		DirectX::XMMATRIX camera;

		Microsoft::WRL::ComPtr<ID3D11Device> device;
		Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> currentTextureView;
		Microsoft::WRL::ComPtr<ID3D11SamplerState> currentSamplerState;
		Microsoft::WRL::ComPtr<ID3D11Buffer> currentVertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> currentIndexBuffer;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> currentVertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> currentPixelShader;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> currentVertexLayout;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> currentTexture;
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;
		D3D11_RASTERIZER_DESC rasterizerDesc = {};

	};





	class OpenGLpipeline {


	public:

		OpenGLpipeline(HWND windowHandle); 

		static GLuint CompileShader(unsigned int type, const std::string& source) {
			GLuint id = glCreateShader(type);
			const char* src = source.c_str();
			glShaderSource(id, 1, &src, nullptr);
			// shader handle = id
			// count = 1 (essentially the number of "source codes" or source code strings)
			// string = a 'pp' to the source code.  (in the case of multiple source strings, this would be a pointer to an array of pointers)
			// length = nullptr (this would be an array of string lengths in the case of multiple source strings)
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

		static std::string GetShaderSource (std::string filename) {
			std::ifstream infile(filename);
			std::stringstream source;
			source << infile.rdbuf();
			std::string slurp = source.str();
			return source.str();
		}

		static GLuint CreateShader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource) {
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

		void DrawFrameTexture(Surface& texture); 
		void DrawFrameTextureIndexed(IndexedList<TextureVertex> inVertices, const Surface& texture, const Matrix4x4& transformation);

	private:
		HDC ourWindowHandleToDeviceContext;
	};

}

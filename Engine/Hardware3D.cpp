#include "Hardware3D.h"
#include "RendererDemo.h"
#include "..\imgui\imgui_impl_win32.h"
#include "..\imgui\imgui_impl_dx11.h"

namespace Hardware3D {


		Direct3Dpipeline::Direct3Dpipeline(HWND windowHandle) {
			DXGI_SWAP_CHAIN_DESC swapChainDesec = {};
			swapChainDesec.BufferDesc.Width = 0;
			swapChainDesec.BufferDesc.Height = 0;			
			swapChainDesec.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			swapChainDesec.BufferDesc.RefreshRate.Numerator = 0;
			swapChainDesec.BufferDesc.RefreshRate.Denominator = 0;
			swapChainDesec.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
			swapChainDesec.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			swapChainDesec.SampleDesc.Count = 1;
			swapChainDesec.SampleDesc.Quality = 0;
			swapChainDesec.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesec.BufferCount = 1;
			swapChainDesec.OutputWindow = windowHandle;
			swapChainDesec.Windowed = TRUE;
			swapChainDesec.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			swapChainDesec.Flags = 0;

			UINT swapCreateFlags = 0u;
#ifndef NDEBUG
			swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

			// for checking results of d3d functions
			//HRESULT hr;

			// create device and front/back buffers, and swap chain and rendering context
			//GFX_THROW_INFO
			(D3D11CreateDeviceAndSwapChain(
				nullptr,
				D3D_DRIVER_TYPE_HARDWARE,
				nullptr,
				swapCreateFlags,
				nullptr,
				0,
				D3D11_SDK_VERSION,
				&swapChainDesec,
				&swapChain,
				&device,
				nullptr,
				&deviceContext
			));
			// gain access to texture subresource in swap chain (back buffer)
			Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer;
			(swapChain->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer));	//GFX_THROW_INFO
			(device->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &renderTargetView));	//GFX_THROW_INFO
		}

		void Direct3Dpipeline::BeginFrame() { ClearBuffer(0, 0, 0); }
		
		void Direct3Dpipeline::EndFrame() { swapChain->Present(1u, 0u); }

		void Direct3Dpipeline::ClearBuffer(float r, float g, float b) {
			const float color[] = { r,g,b,1.0f };
			deviceContext->ClearRenderTargetView(renderTargetView.Get(), color);
		}

		void Direct3Dpipeline::BindConstantBuffer(DirectX::XMMATRIX bufferData) {

			Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;
			D3D11_BUFFER_DESC constandBufferDesc;
			constandBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			constandBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			constandBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			constandBufferDesc.MiscFlags = 0u;
			constandBufferDesc.ByteWidth = sizeof(bufferData);
			D3D11_SUBRESOURCE_DATA subdata = {};
			subdata.pSysMem = &bufferData;
			device->CreateBuffer(&constandBufferDesc, &subdata, &constantBuffer);

			deviceContext->VSSetConstantBuffers(0u, 1u, constantBuffer.GetAddressOf());

		}

		void Direct3Dpipeline::DrawTestTriangle() {

			Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;

			struct Vertex3D { float x, y, z; };

			const Vertex3D testVertices[] = {
				{ 0.5f,  0.5f, 0.5f},
				{ 0.5f, -0.5f, 0.5f},
				{-0.5f, -0.5f, 0.5f},
			};

			// create vertex buffer
			D3D11_BUFFER_DESC vertexBufferDesc = {};
			vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			vertexBufferDesc.CPUAccessFlags = 0u;
			vertexBufferDesc.MiscFlags = 0u;
			vertexBufferDesc.ByteWidth = sizeof(testVertices);
			vertexBufferDesc.StructureByteStride = sizeof(Vertex3D);
			D3D11_SUBRESOURCE_DATA subData = {};
			subData.pSysMem = testVertices;
			device->CreateBuffer(&vertexBufferDesc, &subData, &vertexBuffer);

			// bind vertex buffer
			const UINT stride = sizeof(Vertex3D);
			const UINT offset = 0u;
			deviceContext->IASetVertexBuffers(0u, 1u, vertexBuffer.GetAddressOf(), &stride, &offset);


			// create vertex shader
			Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
			Microsoft::WRL::ComPtr<ID3DBlob> blob;
			D3DReadFileToBlob(L"TestVertexShader.cso", &blob);
			device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vertexShader);

			// bind vertex shader
			deviceContext->VSSetShader(vertexShader.Get(), 0, 0);

			// input layout for vertex shader (in this case, only contains a 2D position)
			Microsoft::WRL::ComPtr<ID3D11InputLayout> vertexLayout;
			const D3D11_INPUT_ELEMENT_DESC vertexDesc[] = {
				{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			};
			device->CreateInputLayout(
				vertexDesc, (UINT)std::size(vertexDesc),
				blob->GetBufferPointer(),
				blob->GetBufferSize(),
				&vertexLayout
			);

			// bind vertex shader input layout
			deviceContext->IASetInputLayout(vertexLayout.Get());


			// create pixel shader
			Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
			D3DReadFileToBlob(L"TestPixelShader.cso", &blob);
			device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pixelShader);

			// bind pixel shader
			deviceContext->PSSetShader(pixelShader.Get(), nullptr, 0u);

			// bind render target
			deviceContext->OMSetRenderTargets(1u, renderTargetView.GetAddressOf(), nullptr);

			// set primitive topology to triangle list
			deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			//configure viewport
			D3D11_VIEWPORT viewport;
			viewport.Width = ApplicationData::screenWidth;
			viewport.Height = ApplicationData::screenHeight;
			viewport.MinDepth = 0;
			viewport.MaxDepth = 1;
			viewport.TopLeftX = 0;
			viewport.TopLeftY = 0;
			deviceContext->RSSetViewports(1u, &viewport);

			deviceContext->Draw(3u, 0u);
		}

		

		void Direct3Dpipeline::CreateResources(const IndexedList<TextureVertex>& vertices, const Surface& texture) {
			CreateShaders();
			CreateBuffers(vertices);
			CreateTexture(texture);
		}

		void Direct3Dpipeline::CreateShaders() {
			// create vertex shader
			Microsoft::WRL::ComPtr<ID3DBlob> blob;
			D3DReadFileToBlob(L"TextureVertexShader.cso", &blob);
			device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &currentVertexShader);

			// input layout for vertex shader (in this case, only contains a 2D position)			
			const D3D11_INPUT_ELEMENT_DESC vertexDesc[] = {
				{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"TexCoord", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			};
			device->CreateInputLayout(
				vertexDesc, (UINT)std::size(vertexDesc),
				blob->GetBufferPointer(),
				blob->GetBufferSize(),
				&currentVertexLayout
			);

			// set primitive topology to triangle list
			deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			// create pixel shader			
			D3DReadFileToBlob(L"TexturePixelShader.cso", &blob);
			device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &currentPixelShader);
		}


		void Direct3Dpipeline::CreateBuffers(const IndexedList<TextureVertex>& vertices) {

			// create vertex buffer			
			D3D11_BUFFER_DESC vertexBufferDesc = {};
			vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			vertexBufferDesc.CPUAccessFlags = 0u;
			vertexBufferDesc.MiscFlags = 0u;
			vertexBufferDesc.ByteWidth = (unsigned int) (sizeof(TextureVertex) * vertices.vertices.size());
			vertexBufferDesc.StructureByteStride = sizeof(TextureVertex);
			D3D11_SUBRESOURCE_DATA vbSubData = {};
			vbSubData.pSysMem = vertices.vertices.data();
			device->CreateBuffer(&vertexBufferDesc, &vbSubData, &currentVertexBuffer);

			// create index buffer
			D3D11_BUFFER_DESC indexBufferDesc = {};
			indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			indexBufferDesc.CPUAccessFlags = 0u;
			indexBufferDesc.MiscFlags = 0u;
			indexBufferDesc.ByteWidth = (unsigned int)(sizeof(int) * vertices.indices.size());
			indexBufferDesc.StructureByteStride = sizeof(int);
			D3D11_SUBRESOURCE_DATA ibSubData = {};
			ibSubData.pSysMem = vertices.indices.data();
			device->CreateBuffer(&indexBufferDesc, &ibSubData, &currentIndexBuffer);
		}

		void Direct3Dpipeline::CreateTexture(const Surface& texture) {
			// create D3D texture from texture
			D3D11_TEXTURE2D_DESC textureDesc = {};
			textureDesc.Width = texture.GetWidth();
			textureDesc.Height = texture.GetHeight();
			textureDesc.MipLevels = 1;
			textureDesc.ArraySize = 1;
			textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
			textureDesc.SampleDesc.Count = 1;
			textureDesc.SampleDesc.Quality = 0;
			textureDesc.Usage = D3D11_USAGE_DEFAULT;
			textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			textureDesc.CPUAccessFlags = 0;
			textureDesc.MiscFlags = 0;
			D3D11_SUBRESOURCE_DATA subData = {};
			subData.pSysMem = texture.GetBufferPtrConst();			//?
			subData.SysMemPitch = texture.GetWidth() * sizeof(Color);			
			device->CreateTexture2D(&textureDesc, &subData, &currentTexture);

			// create a resource view on the texture
			D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc = {};
			viewDesc.Format = textureDesc.Format;
			viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			viewDesc.Texture2D.MostDetailedMip = 0;
			viewDesc.Texture2D.MipLevels = 1;			
			device->CreateShaderResourceView(currentTexture.Get(), &viewDesc, &currentTextureView);
		}

		void Direct3Dpipeline::Bind() {
			
			// bind vertex buffer
			const UINT stride = sizeof(TextureVertex);
			const UINT offset = 0u;
			deviceContext->IASetVertexBuffers(0u, 1u, currentVertexBuffer.GetAddressOf(), &stride, &offset);
			
			// bind index buffer
			deviceContext->IASetIndexBuffer(currentIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

			// bind vertex shader
			deviceContext->VSSetShader(currentVertexShader.Get(), 0, 0);

			// bind vertex shader input layout
			deviceContext->IASetInputLayout(currentVertexLayout.Get());

			// bind pixel shader
			deviceContext->PSSetShader(currentPixelShader.Get(), nullptr, 0u);

			// bind sampler state
			deviceContext->PSSetSamplers(0, 1, currentSamplerState.GetAddressOf());

			// bind render target
			deviceContext->OMSetRenderTargets(1u, renderTargetView.GetAddressOf(), nullptr);

			// bind texture view
			deviceContext->PSSetShaderResources(0u, 1u, currentTextureView.GetAddressOf());
		}

		void Direct3Dpipeline::Draw(const IndexedList<TextureVertex>& vertices, const Surface& texture) {

			Bind();

			//configure viewport
			D3D11_VIEWPORT viewport;
			viewport.Width = ApplicationData::screenWidth;
			viewport.Height = ApplicationData::screenHeight;
			viewport.MinDepth = 0;
			viewport.MaxDepth = 1;
			viewport.TopLeftX = 0;
			viewport.TopLeftY = 0;
			deviceContext->RSSetViewports(1u, &viewport);

			deviceContext->DrawIndexed((UINT)vertices.indices.size(), 0u, 0u);

			ImGui_ImplDX11_Init(*device.GetAddressOf(), *deviceContext.GetAddressOf());
			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();
			RendererDemo::OptionsMenu();
			ImGui::Render();
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
			ImGui_ImplDX11_Shutdown();

		}


		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//	OPEN GL PIPELINE

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		OpenGLpipeline::OpenGLpipeline(HWND windowHandle) {

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

		void OpenGLpipeline::TestDrawFrameLegacy() {

			glClear(GL_COLOR_BUFFER_BIT);
			/*{ 0.5f,  0.5f, 0.5f},
				{ 0.5f, -0.5f, 0.5f},
				{-0.5f, -0.5f, 0.5f},*/
			glBegin(GL_TRIANGLES);
			glVertex2f(-0.5f, -0.5f);
			glVertex2f(0.0f, 0.5f);
			glVertex2f(0.5f, -0.5f);
			glEnd();
			SwapBuffers(ourWindowHandleToDeviceContext);
		}

		void OpenGLpipeline::TestDrawFrame() {


			std::string vertexShaderSource = GetShaderSource("GLtestVertexShader.glsl");
			std::string fragmentShaderSource = GetShaderSource("GLtestFragmentShader.glsl");

			GLuint shaders = CreateShader(vertexShaderSource, fragmentShaderSource);
			glUseProgram(shaders);


			float vertexPositions[] = { -0.5f, -0.5f,
										 0.0f, 0.5f,
										 0.5f, -0.5f };

			unsigned int indices[] = {
										0, 1, 2,
										2, 3, 0
			};

			unsigned int vertexBufferID;
			glGenBuffers(1, &vertexBufferID);
			glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);

			// only one attribute (position), so only one call
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
			glEnableVertexAttribArray(0);


			unsigned int indexBufferID;
			glGenBuffers(1, &indexBufferID);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


			glClear(GL_COLOR_BUFFER_BIT);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
			SwapBuffers(ourWindowHandleToDeviceContext);
		}

		void OpenGLpipeline::TestDrawFrameTexture(Surface& texture) {

			// RGBA converstion test
			texture.makeRGBA();

			GLuint textureID;
			glGenTextures(1, &textureID);
			glBindTexture(GL_TEXTURE_2D, textureID);


			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);			

			// RGBA converstion test
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texture.GetWidth(), texture.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, texture.GetBufferPtrConst());

			glActiveTexture(GL_TEXTURE0);

			std::string vertexShaderSource = GetShaderSource("GLtextureVertexShader.glsl");
			std::string fragmentShaderSource = GetShaderSource("GLtextureFragmentShader.glsl");

			GLuint shaders = CreateShader(vertexShaderSource, fragmentShaderSource);
			glUseProgram(shaders);

			int uTextureLocation = glGetUniformLocation(shaders, "u_Texture");
			assert(uTextureLocation != -1);
			glUniform1i(uTextureLocation, 0);




			float vertices[] = { -1.0f, -1.0f, 0.0f, 1.0f,
								 -1.0f,  1.0f, 0.0f, 0.0f,
								  1.0f,	 1.0f, 1.0f, 0.0f,
								  1.0f, -1.0f, 1.0f, 1.0f };

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

			// unbind texture
			glBindTexture(GL_TEXTURE_2D, 0);
			glDeleteTextures(1, &textureID);
		}

		void OpenGLpipeline::TestDrawFrameTextureIndexed(IndexedList<TextureVertex> inVertices, const Surface& texture, const Matrix4x4& transformation) {

			GLuint textureID;
			glGenTextures(1, &textureID);
			glBindTexture(GL_TEXTURE_2D, textureID);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texture.GetWidth(), texture.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, texture.GetRGBApointerConst());

			glActiveTexture(GL_TEXTURE0);


			std::string vertexShaderSource = GetShaderSource("GLtextureTransformVertexShader.glsl");
			std::string fragmentShaderSource = GetShaderSource("GLtextureFragmentShader.glsl");


			GLuint shaders = CreateShader(vertexShaderSource, fragmentShaderSource);
			glUseProgram(shaders);

			int uTextureLocation = glGetUniformLocation(shaders, "u_Texture");
			//			assert(uTextureLocation != -1);
			glUniform1i(uTextureLocation, 0);

			int uMatrixLocation = glGetUniformLocation(shaders, "u_TransformationMatrix");
			glUniformMatrix4fv(uMatrixLocation, 1, GL_FALSE, &transformation.elements[0][0]);



			unsigned int vertexBufferID;
			glGenBuffers(1, &vertexBufferID);
			glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
			glBufferData(GL_ARRAY_BUFFER, inVertices.vertices.size() * sizeof(TextureVertex), inVertices.vertices.data(), GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TextureVertex), 0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(TextureVertex), (const void*)(sizeof(inVertices.vertices[0].position)));
			glEnableVertexAttribArray(1);

			unsigned int indexBufferID;
			glGenBuffers(1, &indexBufferID);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, inVertices.indices.size() * sizeof(unsigned int), inVertices.indices.data(), GL_STATIC_DRAW);

			glClear(GL_COLOR_BUFFER_BIT);
			glDrawElements(GL_TRIANGLES, (GLsizei) inVertices.indices.size(), GL_UNSIGNED_INT, nullptr);
			
			ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();					
			RendererDemo::OptionsMenu();
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());			
			SwapBuffers(ourWindowHandleToDeviceContext);
			ImGui_ImplOpenGL3_Shutdown();

			// unbind texture
			glBindTexture(GL_TEXTURE_2D, 0);
			glDeleteTextures(1, &textureID);
		}



}
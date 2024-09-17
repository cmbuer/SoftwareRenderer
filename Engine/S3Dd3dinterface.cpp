#include <assert.h>
#include <array>
#include "S3Dd3dInterface.h"
#include "MenuSystem.h"
#include "RendererDemo.h"

#include "..\imgui\imgui_impl_win32.h"
#include "..\imgui\imgui_impl_dx11.h"
#include "..\imgui\imgui_impl_opengl3.h"

#pragma comment( lib,"d3d11.lib" )

namespace Software3D {
	// Ignore the intellisense error "cannot open source file" for .shh files.
	// They will be created during the build sequence before the preprocessor runs.
	namespace FramebufferShaders
	{
		#include "FramebufferPS.shh"
		#include "FramebufferVS.shh"
	}


	using Microsoft::WRL::ComPtr;


	D3DInterface::D3DInterface(HWND windowHandle)
		:
		sysBuffer(ScreenWidth, ScreenHeight)
	{
		assert(windowHandle != nullptr);

		//////////////////////////////////////////////////////
		// create device and swap chain/get render target view
		DXGI_SWAP_CHAIN_DESC sd = {};
		sd.BufferCount = 1;
		sd.BufferDesc.Width = D3DInterface::ScreenWidth;
		sd.BufferDesc.Height = D3DInterface::ScreenHeight;
		sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		//sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;// PIXEL FORMAT TESTING
		sd.BufferDesc.RefreshRate.Numerator = 1;
		sd.BufferDesc.RefreshRate.Denominator = 60;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = windowHandle;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;
		
		D3D_FEATURE_LEVEL	featureLevelsRequested = D3D_FEATURE_LEVEL_11_1; //D3D_FEATURE_LEVEL_9_1;
		UINT				numLevelsRequested = 1;
		D3D_FEATURE_LEVEL	featureLevelsSupported; 
		HRESULT				hr;
		UINT				createFlags = 0u;
		
#ifdef _DEBUG
#ifdef USE_DIRECT3D_DEBUG_RUNTIME
		createFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
#endif

		// create device and front/back buffers
		if (FAILED(hr = D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			createFlags,
			&featureLevelsRequested,
			numLevelsRequested,
			D3D11_SDK_VERSION,
			&sd,
			&pSwapChain,
			&pDevice,
			&featureLevelsSupported,
			&pImmediateContext)))
		{
			throw CHILI_GFX_EXCEPTION(hr, L"Creating device and swap chain");
		}

		// get handle to backbuffer
		ComPtr<ID3D11Resource> pBackBuffer;
		if (FAILED(hr = pSwapChain->GetBuffer(
			0,
			__uuidof(ID3D11Texture2D),
			(LPVOID*)&pBackBuffer)))
		{
			throw CHILI_GFX_EXCEPTION(hr, L"Getting back buffer");
		}

		// create a view on backbuffer that we can render to
		if (FAILED(hr = pDevice->CreateRenderTargetView(
			pBackBuffer.Get(),
			nullptr,
			&pRenderTargetView)))
		{
			throw CHILI_GFX_EXCEPTION(hr, L"Creating render target view on backbuffer");
		}


		// set backbuffer as the render target using created view
		pImmediateContext->OMSetRenderTargets(1, pRenderTargetView.GetAddressOf(), nullptr);


		// set viewport dimensions
		D3D11_VIEWPORT vp;
		vp.Width = float(D3DInterface::ScreenWidth);
		vp.Height = float(D3DInterface::ScreenHeight);
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0.0f;
		vp.TopLeftY = 0.0f;
		pImmediateContext->RSSetViewports(1, &vp);


		///////////////////////////////////////
		// create texture for cpu render target
		D3D11_TEXTURE2D_DESC sysTexDesc;
		sysTexDesc.Width = D3DInterface::ScreenWidth;
		sysTexDesc.Height = D3DInterface::ScreenHeight;
		sysTexDesc.MipLevels = 1;
		sysTexDesc.ArraySize = 1;
		sysTexDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		//sysTexDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;// PIXEL FORMAT TESTING
		sysTexDesc.SampleDesc.Count = 1;
		sysTexDesc.SampleDesc.Quality = 0;
		sysTexDesc.Usage = D3D11_USAGE_DYNAMIC;
		sysTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		sysTexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		sysTexDesc.MiscFlags = 0;
		// create the texture
		if (FAILED(hr = pDevice->CreateTexture2D(&sysTexDesc, nullptr, &pSysBufferTexture)))
		{
			throw CHILI_GFX_EXCEPTION(hr, L"Creating sysbuffer texture");
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = sysTexDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		// create the resource view on the texture
		if (FAILED(hr = pDevice->CreateShaderResourceView(pSysBufferTexture.Get(),
			&srvDesc, &pSysBufferTextureView)))
		{
			throw CHILI_GFX_EXCEPTION(hr, L"Creating view on sysBuffer texture");
		}


		////////////////////////////////////////////////
		// create pixel shader for framebuffer
		// Ignore the intellisense error "namespace has no member"
		if (FAILED(hr = pDevice->CreatePixelShader(
			FramebufferShaders::FramebufferPSBytecode,
			sizeof(FramebufferShaders::FramebufferPSBytecode),
			nullptr,
			&pPixelShader)))
		{
			throw CHILI_GFX_EXCEPTION(hr, L"Creating pixel shader");
		}


		/////////////////////////////////////////////////
		// create vertex shader for framebuffer
		// Ignore the intellisense error "namespace has no member"
		if (FAILED(hr = pDevice->CreateVertexShader(
			FramebufferShaders::FramebufferVSBytecode,
			sizeof(FramebufferShaders::FramebufferVSBytecode),
			nullptr,
			&pVertexShader)))
		{
			throw CHILI_GFX_EXCEPTION(hr, L"Creating vertex shader");
		}


		//////////////////////////////////////////////////////////////
		// create and fill vertex buffer with quad for rendering frame
		const FSQVertex vertices[] =
		{
			{ -1.0f,1.0f,0.5f,0.0f,0.0f },
			{ 1.0f,1.0f,0.5f,1.0f,0.0f },
			{ 1.0f,-1.0f,0.5f,1.0f,1.0f },
			{ -1.0f,1.0f,0.5f,0.0f,0.0f },
			{ 1.0f,-1.0f,0.5f,1.0f,1.0f },
			{ -1.0f,-1.0f,0.5f,0.0f,1.0f },
		};
		D3D11_BUFFER_DESC bd = {};
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(FSQVertex) * 6;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0u;
		D3D11_SUBRESOURCE_DATA initData = {};
		initData.pSysMem = vertices;
		if (FAILED(hr = pDevice->CreateBuffer(&bd, &initData, &pVertexBuffer)))
		{
			throw CHILI_GFX_EXCEPTION(hr, L"Creating vertex buffer");
		}


		//////////////////////////////////////////
		// create input layout for fullscreen quad
		const D3D11_INPUT_ELEMENT_DESC ied[] =
		{
			{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 }
		};

		// Ignore the intellisense error "namespace has no member"
		if (FAILED(hr = pDevice->CreateInputLayout(ied, 2,
			FramebufferShaders::FramebufferVSBytecode,
			sizeof(FramebufferShaders::FramebufferVSBytecode),
			&pInputLayout)))
		{
			throw CHILI_GFX_EXCEPTION(hr, L"Creating input layout");
		}


		////////////////////////////////////////////////////
		// Create sampler state for fullscreen textured quad
		D3D11_SAMPLER_DESC sampDesc = {};
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
		if (FAILED(hr = pDevice->CreateSamplerState(&sampDesc, &pSamplerState)))
		{
			throw CHILI_GFX_EXCEPTION(hr, L"Creating sampler state");
		}
	}

	D3DInterface::~D3DInterface()
	{
		// clear the state of the device context before destruction
		if (pImmediateContext) pImmediateContext->ClearState();
	}

	void D3DInterface::EndFrame()
	{
				
		HRESULT hr;

		// lock and map the adapter memory for copying over the sysbuffer
		if (FAILED(hr = pImmediateContext->Map(pSysBufferTexture.Get(), 0u,
			D3D11_MAP_WRITE_DISCARD, 0u, &mappedSysBufferTexture)))
		{
			throw CHILI_GFX_EXCEPTION(hr, L"Mapping sysbuffer");
		}
		// perform the copy line-by-line
		sysBuffer.Present(mappedSysBufferTexture.RowPitch,
			reinterpret_cast<BYTE*>(mappedSysBufferTexture.pData));
		// release the adapter memory
		pImmediateContext->Unmap(pSysBufferTexture.Get(), 0u);

		// render offscreen scene texture to back buffer
		pImmediateContext->IASetInputLayout(pInputLayout.Get());
		pImmediateContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);
		pImmediateContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);
		pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		const UINT stride = sizeof(FSQVertex);
		const UINT offset = 0u;
		pImmediateContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
		pImmediateContext->PSSetShaderResources(0u, 1u, pSysBufferTextureView.GetAddressOf());
		pImmediateContext->PSSetSamplers(0u, 1u, pSamplerState.GetAddressOf());
		pImmediateContext->Draw(6u, 0u);

		
		// Render IMGUI after buffer, before presentation
		
		ImGui_ImplDX11_Init(*pDevice.GetAddressOf(), *pImmediateContext.GetAddressOf());
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		//ImGui::ShowDemoWindow();
		//MenuSystem::MainMenu();
		RendererDemo::OptionsMenu();
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());		
		ImGui_ImplDX11_Shutdown();
		

		// flip back/front buffers
		if (FAILED(hr = pSwapChain->Present(1u, 0u)))
		{
			throw CHILI_GFX_EXCEPTION(hr, L"Presenting back buffer");
		}


	}

	void D3DInterface::BeginFrame (  )
	{
		sysBuffer.Clear(Colors::Red);
	}

	void D3DInterface::Draw() {

		HRESULT hr;

		// lock and map the adapter memory for copying over the sysbuffer
		if (FAILED(hr = pImmediateContext->Map(pSysBufferTexture.Get(), 0u,
			D3D11_MAP_WRITE_DISCARD, 0u, &mappedSysBufferTexture)))
		{
			throw CHILI_GFX_EXCEPTION(hr, L"Mapping sysbuffer");
		}
		// perform the copy line-by-line
		sysBuffer.Present(mappedSysBufferTexture.RowPitch,
			reinterpret_cast<BYTE*>(mappedSysBufferTexture.pData));
		// release the adapter memory
		pImmediateContext->Unmap(pSysBufferTexture.Get(), 0u);

		// render offscreen scene texture to back buffer
		pImmediateContext->IASetInputLayout(pInputLayout.Get());
		pImmediateContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);
		pImmediateContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);
		pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		const UINT stride = sizeof(FSQVertex);
		const UINT offset = 0u;
		pImmediateContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
		pImmediateContext->PSSetShaderResources(0u, 1u, pSysBufferTextureView.GetAddressOf());
		pImmediateContext->PSSetSamplers(0u, 1u, pSamplerState.GetAddressOf());
		pImmediateContext->Draw(6u, 0u);

		// Render IMGUI after buffer, before presentation
		ImGui_ImplDX11_Init(*pDevice.GetAddressOf(), *pImmediateContext.GetAddressOf());
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	void D3DInterface::EndFrameNew() {
		
		HRESULT hr;

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		ImGui_ImplDX11_Shutdown();

		// flip back/front buffers
		if (FAILED(hr = pSwapChain->Present(1u, 0u)))
		{
			throw CHILI_GFX_EXCEPTION(hr, L"Presenting back buffer");
		}
	}




	//////////////////////////////////////////////////
	//           D3DInterface Exception
	D3DInterface::Exception::Exception(HRESULT hr, const std::wstring& note, const wchar_t* file, unsigned int line)
		:
		ChiliException(file, line, note),
		hr(hr)
	{}

	std::wstring D3DInterface::Exception::GetFullMessage() const
	{
		const std::wstring empty = L"";
		const std::wstring errorName = GetErrorName();
		const std::wstring errorDesc = GetErrorDescription();
		const std::wstring& note = GetNote();
		const std::wstring location = GetLocation();
		return    (!errorName.empty() ? std::wstring(L"Error: ") + errorName + L"\n"
			: empty)
			+ (!errorDesc.empty() ? std::wstring(L"Description: ") + errorDesc + L"\n"
				: empty)
			+ (!note.empty() ? std::wstring(L"Note: ") + note + L"\n"
				: empty)
			+ (!location.empty() ? std::wstring(L"Location: ") + location
				: empty);
	}

	std::wstring D3DInterface::Exception::GetErrorName() const
	{
		return DXGetErrorString(hr);
	}

	std::wstring D3DInterface::Exception::GetErrorDescription() const
	{
		std::array<wchar_t, 512> wideDescription;
		DXGetErrorDescription(hr, wideDescription.data(), wideDescription.size());
		return wideDescription.data();
	}

	std::wstring D3DInterface::Exception::GetExceptionType() const
	{
		return L"Chili Graphics Exception";
	}

}
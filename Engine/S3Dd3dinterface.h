#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <string>
#include "DXErr.h"
#include "GDIPlusManager.h"
#include "ChiliException.h"
#include "Surface.h"
#include "Colors.h"
#include "Math3D.h"
#include "ApplicationData.h"

#define CHILI_GFX_EXCEPTION( hr,note ) D3DInterface::Exception( hr,note,_CRT_WIDE(__FILE__),__LINE__ )

namespace Software3D {

	class D3DInterface {
	public:
		class Exception : public ChiliException
		{
		public:
			Exception(HRESULT hr, const std::wstring& note, const wchar_t* file, unsigned int line);
			std::wstring GetErrorName() const;
			std::wstring GetErrorDescription() const;
			virtual std::wstring GetFullMessage() const override;
			virtual std::wstring GetExceptionType() const override;
		private:
			HRESULT hr;
		};
	private:
		// vertex format for the framebuffer fullscreen textured quad
		struct FSQVertex
		{
			float x, y, z;		// position
			float u, v;			// texcoords
		};
	public:
		D3DInterface() = default;
		D3DInterface(HWND windowHandle);
		void BeginFrame();
		void Draw();
		void EndFrame();
		void EndFrameNew();
		
		void DrawLine(const Vector2& p1, const Vector2& p2, Color c)
		{
			DrawLine(p1.x, p1.y, p2.x, p2.y, c);
		}
		void DrawLine(float x1, float y1, float x2, float y2, Color c);

		void PutPixel(int x, int y, int r, int g, int b)
		{
			PutPixel(x, y, { unsigned char(r),unsigned char(g),unsigned char(b) });
		}
		void PutPixel(int x, int y, Color c)
		{
			sysBuffer.PutPixel(x, y, c);
		}
		~D3DInterface();
	public:
		GDIPlusManager										gdipMan;
		Microsoft::WRL::ComPtr<IDXGISwapChain>				pSwapChain;
		Microsoft::WRL::ComPtr<ID3D11Device>				pDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>			pImmediateContext;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		pRenderTargetView;
		Microsoft::WRL::ComPtr<ID3D11Texture2D>				pSysBufferTexture;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	pSysBufferTextureView;
		Microsoft::WRL::ComPtr<ID3D11PixelShader>			pPixelShader;
		Microsoft::WRL::ComPtr<ID3D11VertexShader>			pVertexShader;
		Microsoft::WRL::ComPtr<ID3D11Buffer>				pVertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11InputLayout>			pInputLayout;
		Microsoft::WRL::ComPtr<ID3D11SamplerState>			pSamplerState;
		D3D11_MAPPED_SUBRESOURCE							mappedSysBufferTexture;
		Surface												sysBuffer;
	public:
		static const unsigned int ScreenWidth = ApplicationData::screenWidth;  
		static const unsigned int ScreenHeight = ApplicationData::screenHeight;
	};
}

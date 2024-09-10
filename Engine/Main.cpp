#include <assert.h>
#include <DirectXMath.h>
#include "MainWindow.h"
#include "ChiliException.h"
#include "Math3D.h"
#include "S3Dtypes.h"
#include "TextureEffect.h"
#include "S3Dd3dinterface.h"
#include "S3Dpipeline.h"
#include "Cube.h"
#include "FrameTimer.h"
#include "FlatGeometryShadingEffect.h"
#include "GouraudEffect.h"
#include "Sphere.h"
#include "Hardware3D.h"
#include "MenuSystem.h"
#include "TexturedCubeScene.h"
#include "SolidColorSphereScene.h"
#include "S3DopenGLinterface.h"
#include "RendererDemo.h"
#include "..\imgui\imgui_impl_win32.h"
#include "..\imgui\imgui_impl_dx11.h"
//#include "..\imgui\imgui_impl_opengl3_loader.h"
#include "..\imgui\imgui_impl_opengl3.h"

using namespace Software3D;

int WINAPI wWinMain( HINSTANCE hInst,HINSTANCE,LPWSTR pArgs,INT )
{
	try
	{
		// IMGUI
			// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsLight();

		MainWindow mainWindow( hInst,pArgs );		
		try
		{			
			OpenGLinterface openGLinterface(mainWindow.hWnd);
			D3DInterface d3dInterface(mainWindow.hWnd);						
			Pipeline softwarePipeline(&d3dInterface, &openGLinterface);			
			Hardware3D::Direct3Dpipeline d3dpipeline(mainWindow.hWnd);
			Hardware3D::OpenGLpipeline openGLpipeline(mainWindow.hWnd);
			RendererDemo renderDemo(&softwarePipeline, &d3dpipeline, &openGLpipeline);			
			ImGui_ImplWin32_Init(mainWindow.hWnd);						

			renderDemo.Run(mainWindow);
		}
		catch( const ChiliException& e )
		{
			const std::wstring eMsg = e.GetFullMessage() + 
				L"\n\nException caught at Windows message loop.";
			mainWindow.ShowMessageBox( e.GetExceptionType(),eMsg );
		}
		catch( const std::exception& e )
		{
			// need to convert std::exception what() string from narrow to wide string
			const std::string whatStr( e.what() );
			const std::wstring eMsg = std::wstring( whatStr.begin(),whatStr.end() ) + 
				L"\n\nException caught at Windows message loop.";
			mainWindow.ShowMessageBox( L"Unhandled STL Exception",eMsg );
		}
		catch( ... )
		{
			mainWindow.ShowMessageBox( L"Unhandled Non-STL Exception",
				L"\n\nException caught at Windows message loop." );
		}
	}
	catch( const ChiliException& e )
	{
		const std::wstring eMsg = e.GetFullMessage() +
			L"\n\nException caught at main window creation.";
		MessageBox( nullptr,eMsg.c_str(),e.GetExceptionType().c_str(),MB_OK );
	}
	catch( const std::exception& e )
	{
		// need to convert std::exception what() string from narrow to wide string
		const std::string whatStr( e.what() );
		const std::wstring eMsg = std::wstring( whatStr.begin(),whatStr.end() ) +
			L"\n\nException caught at main window creation.";
		MessageBox( nullptr,eMsg.c_str(),L"Unhandled STL Exception",MB_OK );
	}
	catch( ... )
	{
		MessageBox( nullptr,L"\n\nException caught at main window creation.",
			L"Unhandled Non-STL Exception",MB_OK );
	}

	return 0;
}
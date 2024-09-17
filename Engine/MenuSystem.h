#pragma once
#include <Windows.h>
#include "..\imgui\imgui_impl_dx11.h"
#include "..\imgui\imgui_impl_win32.h"

#include "ApplicationData.h"
#include "MainWindow.h"
#include "FrameTimer.h"
//#include "Software3D.h"
//#include "Hardware3D.h"



// forward declaration
namespace Software3D {
	class Pipeline;
}


namespace MenuSystem {

	void Startup();
	void InitializeOpenGL();
	void DisplayWindowHeader(HWND hwnd, FrameTimer timer, RenderMode renderMode, Software3D::Pipeline* softPipe);
	void MainMenu();
}
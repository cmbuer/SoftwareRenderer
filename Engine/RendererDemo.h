#pragma once
#include <vector>
#include "DemoScene.h"
#include "MainWindow.h"
#include "FrameTimer.h"

class RendererDemo {


public:

	RendererDemo(Software3D::Pipeline* s, Hardware3D::Direct3Dpipeline* d, Hardware3D::OpenGLpipeline* o);
	void Run(MainWindow& window);
	void DisplayMenu();
	static void OptionsMenu();
	static RendererDemo* instance;
	
private:

	struct viewport { int height; int width; };
	

	bool direct3d;		//TEST

	RenderMode renderMode;
	RenderMode newRenderMode;
	Pipeline::CullingState newCullingState;
	Pipeline::WindingDirection newWindingDirection;
	Pipeline::HardwareInterface newHWinterface;
	FrameTimer frameTimer;
	Pipeline* softwarePipeline;
	Hardware3D::Direct3Dpipeline* d3dpipeline;
	Hardware3D::OpenGLpipeline* openGLpipeline;
	std::vector<std::unique_ptr<DemoScene>> scenes;
	std::vector<std::unique_ptr<DemoScene>>::iterator currentScene;
	std::vector<std::unique_ptr<DemoScene>>::iterator newScene;

	void DisplayHeader(MainWindow& mainWindow);
	void Update(MainWindow& window);
	void DrawScene(MainWindow& window);




};
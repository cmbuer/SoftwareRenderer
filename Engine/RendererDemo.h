#pragma once
#include <vector>
#include "DemoScene.h"
#include "MainWindow.h"
#include "FrameTimer.h"

class RendererDemo {


public:

	struct viewport { int height; int width; };
	RenderMode renderMode;
	FrameTimer frameTimer;

	Pipeline* softwarePipeline;	
	Hardware3D::Direct3Dpipeline* d3dpipeline;
	Hardware3D::OpenGLpipeline* openGLpipeline;

	std::vector<std::unique_ptr<DemoScene>> scenes;
	std::vector<std::unique_ptr<DemoScene>>::iterator currentScene;

	RendererDemo(Software3D::Pipeline* s, Hardware3D::Direct3Dpipeline* d, Hardware3D::OpenGLpipeline* o);
	void RendererDemo::Run(MainWindow& window);
	void Update(MainWindow& window);
	void DrawScene(MainWindow& window);

private:








};
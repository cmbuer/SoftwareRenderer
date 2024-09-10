#pragma once
#include "MainWindow.h"
#include <string>

#include "S3Dpipeline.h"
#include "Hardware3D.h"

struct DemoScene {

	std::string name;

	// renbdering pipelines
	Software3D::Pipeline* softwarePipeline;
	Hardware3D::Direct3Dpipeline* direct3Dpipeline;
	Hardware3D::OpenGLpipeline* openGLpipeline;

	DemoScene() = default;
	DemoScene(const std::string & name) : name(name) {}
	virtual void Update(MainWindow& window, float dt) {};
	virtual void Draw(RenderMode renderMode) { exit(99); }; //test
	virtual ~DemoScene() = default;
};

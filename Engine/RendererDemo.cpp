#include "RendererDemo.h"
#include "TexturedCubeScene.h"
#include "SolidColorSphereScene.h"
#include "GouraudScene.h"

RendererDemo* RendererDemo::instance;

RendererDemo::RendererDemo(Software3D::Pipeline* s, Hardware3D::Direct3Dpipeline* d, Hardware3D::OpenGLpipeline* o) {

	softwarePipeline = s;
	d3dpipeline = d;
	openGLpipeline = o;
	renderMode = newRenderMode = RenderMode::Software;									// Please note that for the moment multiple render modes are only supported for texture mapped scenes
	scenes.push_back(std::make_unique<TexturedCubeScene>(s, d, o));
	scenes.push_back(std::make_unique<SolidColorSphereScene>(s, d, o));
	scenes.push_back(std::make_unique<GouraudScene>(s, d, o));
	currentScene = scenes.begin();
}

void RendererDemo::Run(MainWindow& mainWindow) {
	
	frameTimer.Initialize();

	ImGui_ImplWin32_Init(mainWindow.hWnd);

	while (mainWindow.ProcessMessage())
	{			
		Update(mainWindow);
		DisplayHeader(mainWindow);
		DrawScene(mainWindow);
	}
}

void RendererDemo::DisplayMenu() {

	int renderRadio = (int) renderMode;
	int softwareRadio = (int) softwarePipeline->hwInterface;
	int windingRadio = (int) softwarePipeline->windingDirection;
	int cullingRadio = (int) softwarePipeline->cullingState;

	static int sceneRadio = 0;
	ImGui::Begin("Renderer Menu");
	if (renderMode == RenderMode::Software) {
		if (ImGui::CollapsingHeader("Render Options")) {
			ImGui::Text("Winding Direction");
			ImGui::RadioButton("Clockwise", &windingRadio, 1); ImGui::SameLine();
			ImGui::RadioButton("Counter-Clockwise", &windingRadio, -1); ImGui::NewLine();

			ImGui::Text("Face Culling");
			ImGui::RadioButton("Back", &cullingRadio, 1); ImGui::SameLine();
			ImGui::RadioButton("Front", &cullingRadio, -1); ImGui::SameLine();
			ImGui::RadioButton("Front and Back", &cullingRadio, 0); ImGui::NewLine();
		}
	}	
	if (ImGui::CollapsingHeader("Scene Selection")) {
		ImGui::RadioButton("Texture Map Scene", &sceneRadio, 0); ImGui::NewLine();
		ImGui::RadioButton("Solid Color Scene", &sceneRadio, 1); ImGui::NewLine();
		ImGui::RadioButton("Gouraud Shading Scene", &sceneRadio, 2); ImGui::NewLine();

		currentScene = scenes.begin() + sceneRadio;
	}
	ImGui::End();

	softwarePipeline->cullingState = (Pipeline::CullingState) cullingRadio;
	softwarePipeline->windingDirection = (Pipeline::WindingDirection)windingRadio;
}

void RendererDemo::DisplayHeader(MainWindow& mainWindow) {

	std::string headerRenderModeBase = "Render Mode: ";
	std::string headerRenderMode;
	if (renderMode == RenderMode::Software) {
		headerRenderMode = headerRenderModeBase + "Software";
		if (softwarePipeline->hwInterface == Software3D::Pipeline::HardwareInterface::Direct3D)
			headerRenderMode += " (via Direct3D)";
		else if (softwarePipeline->hwInterface == Software3D::Pipeline::HardwareInterface::OpenGL)
			headerRenderMode += " (via OpenGL)";
	}
	if (renderMode == RenderMode::Direct3D)
		headerRenderMode = headerRenderModeBase + "Direct3D";
	if (renderMode == RenderMode::OpenGL)
		headerRenderMode = headerRenderModeBase + "OpenGL";
	std::string headerResolution = "Resolution: " + std::to_string(ApplicationData::screenWidth) + "x" + std::to_string(ApplicationData::screenHeight);
	//std::string headerFPS = "FPS: " + std::to_string(currfps);
	std::string headerFPS = "FPS: " + std::to_string(frameTimer.GetCurrentFPS());
	std::string header = headerRenderMode + "     " + headerResolution + "      " + headerFPS;
	SetWindowTextA(mainWindow.hWnd, header.c_str());
}


void RendererDemo::Update(MainWindow& window) {
	
	float deltaTime = frameTimer.Mark();

	if (window.kbd.WasKeyPressedThisFrame(VK_RIGHT)) {
		if (++currentScene == scenes.end())
			currentScene = scenes.begin();
	}
	if (window.kbd.WasKeyPressedThisFrame(VK_LEFT)) {
		if (currentScene == scenes.begin())
			currentScene = scenes.end() - 1;
		else
			currentScene--;
	}
	

	if (window.kbd.KeyIsPressed('1')) { renderMode = RenderMode::Software; }
	if (window.kbd.KeyIsPressed('2')) { renderMode = RenderMode::Direct3D; }
	if (window.kbd.KeyIsPressed('3')) { renderMode = RenderMode::OpenGL; }
	if (window.kbd.KeyIsPressed('4')) { (*currentScene)->softwarePipeline->hwInterface = Software3D::Pipeline::HardwareInterface::Direct3D; }
	if (window.kbd.KeyIsPressed('5')) { (*currentScene)->softwarePipeline->hwInterface = Software3D::Pipeline::HardwareInterface::OpenGL; }

	if (window.kbd.KeyIsPressed('6')) { glFrontFace(GL_CW); (*currentScene)->softwarePipeline->windingDirection = Pipeline::WindingDirection::Clockwise; }
	if (window.kbd.KeyIsPressed('7')) { glFrontFace(GL_CCW); (*currentScene)->softwarePipeline->windingDirection = Pipeline::WindingDirection::CounterClockwise; }
	if (window.kbd.KeyIsPressed('8')) { glCullFace(GL_BACK); (*currentScene)->softwarePipeline->cullingState = Pipeline::CullingState::Back; }
	if (window.kbd.KeyIsPressed('9')) { glCullFace(GL_FRONT); (*currentScene)->softwarePipeline->cullingState = Pipeline::CullingState::Front; }
	if (window.kbd.KeyIsPressed('0')) { glCullFace(GL_FRONT_AND_BACK); (*currentScene)->softwarePipeline->cullingState = Pipeline::CullingState::FrontAndBack; }	

	(*currentScene)->Update(window, deltaTime);
}

void RendererDemo::DrawScene(MainWindow& window) {
	(*currentScene)->Draw(renderMode);	
}

void RendererDemo::OptionsMenu() {
	RendererDemo::instance->DisplayMenu();
}

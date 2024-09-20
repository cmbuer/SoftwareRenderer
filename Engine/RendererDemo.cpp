#include "RendererDemo.h"
#include "TexturedCubeScene.h"
#include "SolidColorSphereScene.h"
#include "GouraudScene.h"

RendererDemo* RendererDemo::instance;

RendererDemo::RendererDemo(Software3D::Pipeline* s, Hardware3D::Direct3Dpipeline* d, Hardware3D::OpenGLpipeline* o) {

	softwarePipeline = s;
	d3dpipeline = d;
	openGLpipeline = o;
	
	scenes.push_back(std::make_unique<TexturedCubeScene>(s, d, o));
	scenes.push_back(std::make_unique<SolidColorSphereScene>(s, d, o));
	scenes.push_back(std::make_unique<GouraudScene>(s, d, o));

	renderMode = newRenderMode = RenderMode::Software;									// Please note that for the moment multiple render modes are only supported for texture mapped scenes
	newScene = currentScene = scenes.begin();
	newCullingState = softwarePipeline->cullingState;
	newWindingDirection = softwarePipeline->windingDirection;


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
	int windingRadio = (int) newWindingDirection;
	int cullingRadio = (int) newCullingState;
	int sceneRadio = currentScene - scenes.begin();

	ImGui::Begin("Renderer Menu");
	
	if (ImGui::CollapsingHeader("Render Options")) {
		ImGui::Text("Render Mode");
		ImGui::RadioButton("Software", &renderRadio, 0); ImGui::SameLine();
		ImGui::RadioButton("Direct3D", &renderRadio, 1); ImGui::SameLine();
		ImGui::RadioButton("OpenGL", &renderRadio, 2); ImGui::NewLine();

		if (renderRadio == 0) {
			ImGui::Text("Hardware Interface for Software Renderer");
			ImGui::RadioButton("Direct3D", &softwareRadio, 0); ImGui::SameLine();
			ImGui::RadioButton("OpenGL", &softwareRadio, 1); ImGui::NewLine();
		}

		
		ImGui::Text("Winding Direction");
		ImGui::RadioButton("Clockwise", &windingRadio, 1); ImGui::SameLine();
		ImGui::RadioButton("Counter-Clockwise", &windingRadio, -1); ImGui::NewLine();

		ImGui::Text("Face Culling");
		ImGui::RadioButton("None", &cullingRadio, 2); ImGui::SameLine();
		ImGui::RadioButton("Back", &cullingRadio, 1); ImGui::SameLine();
		ImGui::RadioButton("Front", &cullingRadio, -1); ImGui::SameLine();
		ImGui::RadioButton("Front and Back", &cullingRadio, 0); ImGui::NewLine();
	}
		
	if (ImGui::CollapsingHeader("Scene Selection")) {
		ImGui::RadioButton("Texture Map Scene", &sceneRadio, 0); ImGui::NewLine();
		ImGui::RadioButton("Solid Color Scene", &sceneRadio, 1); ImGui::NewLine();
		ImGui::RadioButton("Gouraud Shading Scene", &sceneRadio, 2); ImGui::NewLine();		
	}
	ImGui::End();

	newRenderMode = (RenderMode) renderRadio;
	newHWinterface = (Pipeline::HardwareInterface) softwareRadio;
	newCullingState = (Pipeline::CullingState) cullingRadio;
	newWindingDirection = (Pipeline::WindingDirection) windingRadio;
	newScene = scenes.begin() + sceneRadio;
}

void RendererDemo::DisplayHeader(MainWindow& mainWindow) {

	std::string sceneName = (*currentScene)->name + "  |  ";
	std::string headerRenderModeBase = sceneName +  "Render Mode: ";
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
	
	static GLenum openglFrontFace = GL_CW;
	static GLenum openglCullFace = GL_BACK;

	float deltaTime = frameTimer.Mark();
	currentScene = newScene;

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
	

	if (window.kbd.WasKeyPressedThisFrame('1')) {
		if (renderMode != RenderMode::Software) { newRenderMode = RenderMode::Software; glFrontFace(GL_CW); glCullFace(GL_BACK); }
		else {
			if ((*currentScene)->softwarePipeline->hwInterface == Software3D::Pipeline::HardwareInterface::Direct3D)
				newHWinterface = Software3D::Pipeline::HardwareInterface::OpenGL;
			else if ((*currentScene)->softwarePipeline->hwInterface == Software3D::Pipeline::HardwareInterface::OpenGL)
				newHWinterface = Software3D::Pipeline::HardwareInterface::Direct3D;

		}
	}
	if (window.kbd.KeyIsPressed('2')) { newRenderMode = RenderMode::Direct3D; }
	if (window.kbd.KeyIsPressed('3')) { newRenderMode = RenderMode::OpenGL; glFrontFace(openglFrontFace); glCullFace(openglCullFace); }
	if (window.kbd.KeyIsPressed('4')) { }
	if (window.kbd.KeyIsPressed('5')) { newWindingDirection = Pipeline::WindingDirection::Clockwise; }
	if (window.kbd.KeyIsPressed('6')) { newWindingDirection = Pipeline::WindingDirection::CounterClockwise; }
	if (window.kbd.KeyIsPressed('7')) { newCullingState = Pipeline::CullingState::None; }
	if (window.kbd.KeyIsPressed('8')) { newCullingState = Pipeline::CullingState::Back; }
	if (window.kbd.KeyIsPressed('9')) { newCullingState = Pipeline::CullingState::Front; }
	if (window.kbd.KeyIsPressed('0')) { newCullingState = Pipeline::CullingState::FrontAndBack; }


	renderMode = newRenderMode;
	softwarePipeline->hwInterface = newHWinterface;

	if (newCullingState != Pipeline::CullingState::None) { glEnable(GL_CULL_FACE); }

	if (newWindingDirection == Pipeline::WindingDirection::Clockwise) {
		(*currentScene)->softwarePipeline->windingDirection = Pipeline::WindingDirection::Clockwise;
		d3dpipeline->SetWindingDirection(Hardware3D::Direct3Dpipeline::WindingDirection::Clockwise);
		openglFrontFace = GL_CW;
		glFrontFace(GL_CW);
	}
	if (newWindingDirection == Pipeline::WindingDirection::CounterClockwise) {
		(*currentScene)->softwarePipeline->windingDirection = Pipeline::WindingDirection::CounterClockwise;
		d3dpipeline->SetWindingDirection(Hardware3D::Direct3Dpipeline::WindingDirection::Counterclockwise);
		openglFrontFace = GL_CCW;
		renderMode == RenderMode::Software ? glFrontFace(GL_CW) : glFrontFace(GL_CCW);

	}
	if (newCullingState == Pipeline::CullingState::None) {
		(*currentScene)->softwarePipeline->cullingState = Pipeline::CullingState::None;
		d3dpipeline->SetCullState(D3D11_CULL_NONE);
		glDisable(GL_CULL_FACE);
	}
	if (newCullingState == Pipeline::CullingState::Back) {
		(*currentScene)->softwarePipeline->cullingState = Pipeline::CullingState::Back;
		d3dpipeline->SetCullState(D3D11_CULL_BACK);
		openglCullFace = GL_BACK;
		glCullFace(GL_BACK);

	}
	if (newCullingState == Pipeline::CullingState::Front) {
		(*currentScene)->softwarePipeline->cullingState = Pipeline::CullingState::Front;
		d3dpipeline->SetCullState(D3D11_CULL_FRONT);
		openglCullFace = GL_FRONT;
		renderMode == RenderMode::Software ? glCullFace(GL_BACK) : glCullFace(GL_FRONT);

	}
	if (newCullingState == Pipeline::CullingState::FrontAndBack) {
		(*currentScene)->softwarePipeline->cullingState = Pipeline::CullingState::FrontAndBack;
		openglCullFace = GL_FRONT_AND_BACK;
		renderMode == RenderMode::Software ? glCullFace(GL_BACK) : glCullFace(GL_FRONT_AND_BACK);
	}

	(*currentScene)->Update(window, deltaTime);
}

void RendererDemo::DrawScene(MainWindow& window) {
	(*currentScene)->Draw(renderMode);	
}

void RendererDemo::OptionsMenu() {
	RendererDemo::instance->DisplayMenu();
}

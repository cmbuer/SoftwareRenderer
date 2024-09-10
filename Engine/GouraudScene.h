#pragma once
#include "DemoScene.h"
#include "GouraudEffect.h"
#include "Sphere.h"

struct GouraudScene : DemoScene
{

	static constexpr float dTheta = 2 * PI;
	float offset_z = 4.0f;
	float theta_x = 0.0f;
	float theta_y = 0.0f;
	float theta_z = 0.0f;
	float phi_x = 0.0f;
	float phi_y = 0.0f;
	float phi_z = 0.0f;
	float lpos_x = 0.0f;
	float lpos_y = 0.0f;
	float lpos_z = .6f;

	const Vector3 defaultMaterial = { 0.8f,0.85f,1.0f }; 
	const Vector3 initialDirection = { 0.2f,-0.5f,1.0f };
	const DirectionalLight sceneLight = { { 0.2f,-0.5f,1.0f } , { 1.0f,1.0f,1.0f }, { 0.1f,0.1f,0.1f } };

	GouraudEffect gouraudEffect;
	IndexedList<NormalVertex> normalSphere;

	GouraudScene(Software3D::Pipeline* s, Hardware3D::Direct3Dpipeline* d, Hardware3D::OpenGLpipeline* o) {
		name = "Gouraud Effect Scene";
		gouraudEffect.pipeline = softwarePipeline = s;
		direct3Dpipeline = d;
		openGLpipeline = o;

		gouraudEffect.materialColor = defaultMaterial;
		gouraudEffect.light = sceneLight;
		
		normalSphere = Sphere::GetPlainNormals();
	}
	virtual void Update(MainWindow& wnd, float deltaTime) override {
		
		if (wnd.kbd.KeyIsPressed('Q')) { theta_x = wrap_angle(theta_x + dTheta * deltaTime); }
		if (wnd.kbd.KeyIsPressed('W')) { theta_y = wrap_angle(theta_y + dTheta * deltaTime); }
		if (wnd.kbd.KeyIsPressed('E')) { theta_z = wrap_angle(theta_z + dTheta * deltaTime); }
		if (wnd.kbd.KeyIsPressed('A')) { theta_x = wrap_angle(theta_x - dTheta * deltaTime); }
		if (wnd.kbd.KeyIsPressed('S')) { theta_y = wrap_angle(theta_y - dTheta * deltaTime); }
		if (wnd.kbd.KeyIsPressed('D')) { theta_z = wrap_angle(theta_z - dTheta * deltaTime); }
		if (wnd.kbd.KeyIsPressed('R')) { offset_z += 2.0f * deltaTime; }
		if (wnd.kbd.KeyIsPressed('F')) { offset_z -= 2.0f * deltaTime; }


		// for directional light:
		if (wnd.kbd.KeyIsPressed('U')) { phi_x = wrap_angle(phi_x + dTheta * deltaTime); }
		if (wnd.kbd.KeyIsPressed('I')) { phi_y = wrap_angle(phi_y + dTheta * deltaTime); }
		if (wnd.kbd.KeyIsPressed('O')) { phi_z = wrap_angle(phi_z + dTheta * deltaTime); }
		if (wnd.kbd.KeyIsPressed('J')) { phi_x = wrap_angle(phi_x - dTheta * deltaTime); }
		if (wnd.kbd.KeyIsPressed('K')) { phi_y = wrap_angle(phi_y - dTheta * deltaTime); }
		if (wnd.kbd.KeyIsPressed('L')) { phi_z = wrap_angle(phi_z - dTheta * deltaTime); }
		
	}

	virtual void Draw(RenderMode renderMode) override {

		const Matrix3x3 rotation =
			Matrix3x3::RotationX(theta_x) *
			Matrix3x3::RotationY(theta_y) *
			Matrix3x3::RotationZ(theta_z);

		const Matrix3x3 lightRotation =
			Matrix3x3::RotationX(phi_x) *
			Matrix3x3::RotationY(phi_y) *
			Matrix3x3::RotationZ(phi_z);

		softwarePipeline->BeginFrame();
		gouraudEffect.BindRotation(rotation);
		gouraudEffect.BindTranslation(Vector3(4 * lpos_x, 4 * lpos_y, lpos_z + offset_z + 3));
		gouraudEffect.light.direction = initialDirection * lightRotation;
		gouraudEffect.RenderObjectWithGouraudShading(normalSphere);

		if (softwarePipeline->hwInterface == Pipeline::HardwareInterface::Direct3D) {
			softwarePipeline->EndFrame();
		}
		if (softwarePipeline->hwInterface == Pipeline::HardwareInterface::OpenGL) {
			openGLpipeline->TestDrawFrameTexture(softwarePipeline->hardwarePipeline->sysBuffer);
		}
	}
};

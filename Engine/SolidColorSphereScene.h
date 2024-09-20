#pragma once

#include "DemoScene.h"
#include "Sphere.h"
#include "SolidColorEffect.h"

struct SolidColorSphereScene : DemoScene{

	static constexpr float dTheta = 2 * PI;
	float offset_z = 2.0f;
	float theta_x = 0.0f;
	float theta_y = 0.0f;
	float theta_z = 0.0f;
	float phi_x = 0.0f;
	float phi_y = 0.0f;
	float phi_z = 0.0f;
	float lpos_x = 0.0f;
	float lpos_y = 0.0f;
	float lpos_z = .6f;

	IndexedList<FixedColorVertex> redSPhere;
	SolidColorEffect solidColorEffect;

	SolidColorSphereScene() {
		name = "Solid Color Sphere Scene";
	};

	SolidColorSphereScene(Software3D::Pipeline* s, Hardware3D::Direct3Dpipeline* d, Hardware3D::OpenGLpipeline* o) {
		name = "Solid Color Sphere Scene";
		solidColorEffect.pipeline = softwarePipeline = s;
		direct3Dpipeline = d;
		openGLpipeline = o;

		redSPhere = Sphere::GetSolidColor(1, 12, 24, Colors::Red);
	};

	virtual void Update(MainWindow& wnd, float deltaTime) override {

		if (wnd.kbd.KeyIsPressed('Q')) { theta_x = wrap_angle(theta_x + dTheta * deltaTime); }
		if (wnd.kbd.KeyIsPressed('W')) { theta_y = wrap_angle(theta_y + dTheta * deltaTime); }
		if (wnd.kbd.KeyIsPressed('E')) { theta_z = wrap_angle(theta_z + dTheta * deltaTime); }
		if (wnd.kbd.KeyIsPressed('A')) { theta_x = wrap_angle(theta_x - dTheta * deltaTime); }
		if (wnd.kbd.KeyIsPressed('S')) { theta_y = wrap_angle(theta_y - dTheta * deltaTime); }
		if (wnd.kbd.KeyIsPressed('D')) { theta_z = wrap_angle(theta_z - dTheta * deltaTime); }
		if (wnd.kbd.KeyIsPressed('R')) { offset_z += 2.0f * deltaTime; }
		if (wnd.kbd.KeyIsPressed('F')) { offset_z -= 2.0f * deltaTime; }
	}

	virtual void Draw(RenderMode renderMode) override {

		const Matrix3x3 rotation =
			Matrix3x3::RotationX(theta_x) *
			Matrix3x3::RotationY(theta_y) *
			Matrix3x3::RotationZ(theta_z);

		softwarePipeline->BeginFrame();
		solidColorEffect.BindRotation(rotation);
		solidColorEffect.BindTranslation(Vector3(4 * lpos_x, 4 * lpos_y, lpos_z + offset_z + 3));
		solidColorEffect.RenderObjectWithSolidColor(redSPhere);

		if (softwarePipeline->hwInterface == Pipeline::HardwareInterface::Direct3D) {
			softwarePipeline->EndFrame();
		}
		if (softwarePipeline->hwInterface == Pipeline::HardwareInterface::OpenGL) {
			openGLpipeline->DrawFrameTexture(softwarePipeline->d3dpipeline->sysBuffer);
		}
	}
};
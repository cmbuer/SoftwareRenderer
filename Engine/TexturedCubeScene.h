#pragma once
#include "DemoScene.h"
#include "TextureEffect.h"
#include "Cube.h"

struct TexturedCubeScene : DemoScene {

	TextureEffect textureEffect;
	IndexedList<TextureVertex> cubeList;

	Surface cube = Surface::FromFile(L"images\\cube.png");

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
	

	TexturedCubeScene::TexturedCubeScene() {
		name = "Textured Cube Scene";
	}

	TexturedCubeScene::TexturedCubeScene(Software3D::Pipeline* s, Hardware3D::Direct3Dpipeline* d, Hardware3D::OpenGLpipeline* o) {
		name = "Textured Cube Scene";

		textureEffect.pipeline = softwarePipeline = s;
		direct3Dpipeline = d;
		openGLpipeline = o;

		textureEffect.BindTexture(L"images\\cube.png");
		cubeList = Cube::GetSkinned();

		direct3Dpipeline->CreateResources(cubeList, *textureEffect.texture.get());
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

	}

	virtual void Draw(RenderMode renderMode) override {

		if (renderMode == RenderMode::Software) {
			const Matrix3x3 rotation =
				Matrix3x3::RotationX(theta_x) *
				Matrix3x3::RotationY(theta_y) *
				Matrix3x3::RotationZ(theta_z);

			softwarePipeline->BeginFrame();
			textureEffect.BindRotation(rotation);
			textureEffect.BindTranslation(Vector3(4 * lpos_x, 4 * lpos_y, lpos_z + offset_z + 3));
			textureEffect.RenderObjectWithTexture(cubeList);
			
			if (softwarePipeline->hwInterface == Pipeline::HardwareInterface::Direct3D) {
				softwarePipeline->EndFrame();
			}
			if (softwarePipeline->hwInterface == Pipeline::HardwareInterface::OpenGL) {
				openGLpipeline->DrawFrameTexture(softwarePipeline->d3dpipeline->sysBuffer);
			}
		}
		else if (renderMode == RenderMode::Direct3D) {
			DirectX::XMMATRIX xRotation = DirectX::XMMatrixRotationX(theta_x) * DirectX::XMMatrixRotationY(theta_y) * DirectX::XMMatrixRotationZ(theta_z);			
			DirectX::XMMATRIX xTranslation = DirectX::XMMatrixTranslation(4 * lpos_x, 4 * lpos_y, lpos_z + offset_z + 3);
			DirectX::XMMATRIX xProjection = DirectX::XMMatrixPerspectiveFovLH(PI / 3, 4.0 / 3.0f, 1, 10);
			DirectX::XMMATRIX xTransformation = DirectX::XMMatrixTranspose((xRotation * xTranslation) * xProjection);

			direct3Dpipeline->BeginFrame();
			direct3Dpipeline->BindConstantBuffer(xTransformation);
			//direct3Dpipeline->DrawIndexedTextured(cubeList, *textureEffect.texture.get());
			direct3Dpipeline->Draw(cubeList, *textureEffect.texture.get());
			direct3Dpipeline->EndFrame();
		}
		else if (renderMode == RenderMode::OpenGL) {
			glm::mat4 identity = { 1, 0, 0, 0,
								   0, 1, 0, 0,
								   0, 0, 1, 0,
								   0, 0, 0, 1 };


			Matrix4x4 rotation4 = Matrix4x4::RotationX(theta_x) * Matrix4x4::RotationY(theta_y) * Matrix4x4::RotationZ(theta_z);
			Matrix4x4 translation4 = Matrix4x4::Translation(Vector3(4 * lpos_x, 4 * lpos_y, lpos_z + offset_z + 3));
			Matrix4x4 projection = Matrix4x4::ProjectionHFOV(60.0f, 4.0f / 3.0f, 1, 10);
			Matrix4x4 transform4 = rotation4 * translation4 * projection;
			transform4 = !transform4;

			Matrix4x4 translation4gl = Matrix4x4::Translation(Vector3(4 * lpos_x, 4 * lpos_y, lpos_z + offset_z + 3));
			Matrix4x4 projectiongl = Matrix4x4::OpenGLProjectionHFOV(60.0f, 4.0f / 3.0f, 1, 10);				
			Matrix4x4 transform4gl = rotation4 * translation4gl * projectiongl;
			transform4gl = !transform4gl;

			openGLpipeline->DrawFrameTextureIndexed(cubeList, cube, transform4gl);
		}	
	}
};
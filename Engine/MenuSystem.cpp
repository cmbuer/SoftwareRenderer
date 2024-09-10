#include <string.h>
#include "MenuSystem.h"
//#include "s3dpipeline.h"


namespace MenuSystem {

	void Startup() {

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsLight();
	}

	void MainMenu() {

		static int renderRadio = 0;
		static int softwareRadio = 0;
		static int windingRadio = 0;
		static int cullingRadio = 1;

		if (ImGui::CollapsingHeader("Render Options")) {

			ImGui::Text("Renderer");
			ImGui::RadioButton("Software", &renderRadio, 0); ImGui::SameLine();
			ImGui::RadioButton("Direct3D", &renderRadio, 1); ImGui::SameLine();
			ImGui::RadioButton("OpenGL", &renderRadio, 2); ImGui::NewLine();

			if (renderRadio == 0) {
				ImGui::Text("Hardware Pipeline for Software Renderer");
				ImGui::RadioButton("Direct3D Pipe", &softwareRadio, 0); ImGui::SameLine();
				ImGui::RadioButton("OpenGL Pipe", &softwareRadio, 1); ImGui::NewLine();
			}

			ImGui::Text("Winding Direction");
			ImGui::RadioButton("Clockwise", &windingRadio, 0); ImGui::SameLine();
			ImGui::RadioButton("Counter-Clockwise", &windingRadio, 1); ImGui::NewLine();

			ImGui::Text("Face Culling");
			ImGui::RadioButton("Back", &cullingRadio, 1); ImGui::SameLine();
			ImGui::RadioButton("Front", &cullingRadio, -1); ImGui::SameLine();
			ImGui::RadioButton("Front and Back", &cullingRadio, 0); ImGui::NewLine();

		}

		if (ImGui::CollapsingHeader("Scene Selection")) {}

		if (ImGui::CollapsingHeader("MetaCamera")) {}

	}
}
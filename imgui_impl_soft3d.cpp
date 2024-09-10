
// header stuff
#pragma once
#include "imgui.h"      // IMGUI_IMPL_API

struct Software3D_State;


IMGUI_IMPL_API bool     ImGui_ImplSoft3D_Init(    );    // ?? unknown args at this time
IMGUI_IMPL_API void     ImGui_ImplSoft3D_Shutdown();
IMGUI_IMPL_API void     ImGui_ImplSoft3D_NewFrame();
IMGUI_IMPL_API void     ImGui_ImplSoft3D_RenderDrawData(ImDrawData* draw_data);

// Use if you want to reset your rendering device without losing Dear ImGui state.
IMGUI_IMPL_API bool     ImGui_ImplSoft3D_CreateDeviceObjects();
IMGUI_IMPL_API void     ImGui_ImplSoft3D_InvalidateDeviceObjects();

// end header 





bool ImGui_ImplSoft3D_Init(    ) {

}


void ImGui_ImplSoft3D_RenderDrawData(ImDrawData* draw_data) {


    // Avoid rendering when minimized
    if (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f)
        return;

    
}


void ImGui_ImplSoft3D_Shutdown() {

}


void ImGui_ImplSoft3D_NewFrame() {

}
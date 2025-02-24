#include "render.hpp"
#include <clocale>
#include <util/random.hpp>
#include <awc2/C/awc2.h>
#include "vars.hpp"
#include "backend18.hpp"
#include <imgui/imgui.h>


using namespace features18;


static void render_imgui_interface();


void features18::render()
{
    static const vec4f defaultScreenColor = vec4f{1.0f, 1.0f, 1.0f, 1.0f}; 
    auto  currentWindowSize = awc2getCurrentContextViewport();
    u8    status = 1;


    g_refreshShaderTime.begin();
    g_windims = vec2i{ currentWindowSize.x, currentWindowSize.y };
    /* shader refresh needs a rework to be more relevant, literally no point updating 9 shaders every time */
    if(awc2isKeyPressed(AWC2_KEYCODE_R)) {
        markstr("Refreshing Compute Shaders...");
        for(auto& comp : g_compute) {
            comp.refreshFromFiles();
            comp.resizeLocalWorkGroup(0, g_localWorkGroupSize);
            status = status && comp.compile();
        }
    }
    if(!status)
        return; /* couldn't recompile successfully, code needs recheck */

    g_refreshShaderTime.end();


    gl::glClearNamedFramebufferfv( /* Clear Screen */
        g_fbo, 
        gl::GL_COLOR, 
        0, 
        defaultScreenColor.begin()
    );


    u32 texToRender = 0;
    TIME_NAMESPACE_TIME_CODE_BLOCK(g_renderImguiTime, render_imgui_interface());
    TIME_NAMESPACE_TIME_CODE_BLOCK(g_computeFluidTime, texToRender = compute_fluid());


    /* literally just the velocity texture on the screen */
    g_renderScreenTime.begin();
    gr_computeRenderToTex.bind();
    gr_computeRenderToTex.uniform1i("fieldSampler", 0);
    gr_computeRenderToTex.uniform1i("screentexture", 1);
    gr_computeRenderToTex.uniform2iv("ku_simdims", g_dims.begin());
    gl::glBindTextureUnit(0, texToRender);
    gl::glBindImageTexture(1, gr_drawTexture, 0, false, 0, 
        gl::GL_WRITE_ONLY, 
        gl::GL_RGBA32F
    );
    gl::glDispatchCompute(g_dims.x, g_dims.y, 1);
    gl::glMemoryBarrier(gl::GL_ALL_BARRIER_BITS);



    /* Draw Call */
    // gl::glNamedFramebufferTexture(g_fbo, gl::GL_COLOR_ATTACHMENT0, g_texture[1], 0);
    gl::glBlitNamedFramebuffer(g_fbo, 0, 
        0, 0, g_dims.x,     g_dims.y, 
        0, 0, currentWindowSize.x, currentWindowSize.y,
        gl::GL_COLOR_BUFFER_BIT, 
        gl::GL_NEAREST
    );
    g_renderScreenTime.end();


    g_mousePressed = false;
    ++g_frameCounter;
    return;
}


static void render_imgui_interface()
{
    if(awc2isMouseButtonPressed(AWC2_MOUSEBUTTON_LEFT)) {
        AWC2CursorPosition delta = awc2getMousePositionDelta();
        AWC2CursorPosition pos = awc2getMousePosition();

        g_mouseDragForce    = vec2f{ delta.x, delta.y };
        g_mouseDragPosition = vec2f{ pos.x, pos.y };
        g_mousePressed      = true;
    }
    if(awc2isMouseButtonPressed(AWC2_MOUSEBUTTON_RIGHT)) {
        if(g_runCodeOnceFlag) {
            g_splatterForce = vec4f{ random32f(), random32f(), 0.0f,        1.0f };
            g_splatterColor = vec4f{ random32f(), random32f(), random32f(), 1.0f };
            g_runCodeOnceFlag = false;
        }
    }
    g_runCodeOnceFlag = awc2isMouseButtonReleased(AWC2_MOUSEBUTTON_RIGHT);

    vec2f cfl = g_velocityCFL / g_simUnitCoords;
    char* dragForceStr    = g_mouseDragForce.to_string();
    char* mouseDragPosStr = g_mouseDragPosition.to_string();
    static f32 timeMeasurements[16] = {0};

    f32& __wholeFrame   = timeMeasurements[0];
    f32& __rendering    = timeMeasurements[1];
    f32& __computefluid = timeMeasurements[2];
    f32& __computevel   = timeMeasurements[3];
    f32& __computedye   = timeMeasurements[4];
    f32& __computecfl   = timeMeasurements[5];
    f32& __cflgpuside   = timeMeasurements[6];
    f32& __cflcpuside   = timeMeasurements[7];
    f32& __ref_shader   = timeMeasurements[8];
    f32& __imgui_render = timeMeasurements[9];
    f32& __imgui_screen = timeMeasurements[10];

    __wholeFrame   = g_frameTime.value_units<f32>(1000);
    __rendering    = g_renderTime.value_units<f32>(1000);
    __computefluid = g_computeFluidTime.value_units<f32>(1000);
    __computevel   = g_computeVelTime.value_units<f32>(1000); 
    __computedye   = g_computeDyeTime.value_units<f32>(1000);
    __computecfl   = g_computeCFLTime.value_units<f32>(1000);
    __cflgpuside   = g_computeMaximumCPU.value_units<f32>(1000);
    __cflcpuside   = g_computeMaximumGPU.value_units<f32>(1000);
    __ref_shader   = g_refreshShaderTime.value_units<f32>(1000);
    __imgui_render = g_renderImguiTime.value_units<f32>(1000);
    __imgui_screen = g_renderScreenTime.value_units<f32>(1000);
    timeMeasurements[12] = g_measuremisc0.value_units<f32>(1000);
    timeMeasurements[13] = g_measuremisc1.value_units<f32>(1000);


    g_maxFrameTime = (g_maxFrameTime > timeMeasurements[0]) ? g_maxFrameTime : timeMeasurements[0];
    g_minFrameTime = (g_minFrameTime < timeMeasurements[0]) ? g_minFrameTime : timeMeasurements[0];
    g_avgFrameTime += __wholeFrame;


    ImGui::Begin("ImGui Interaction Window");
    ImGui::BeginTabBar("MyTabBar", ImGuiTabBarFlags_None);
    
    if (ImGui::BeginTabItem("Simulation Parameters"))
    {
        if(ImGui::Button("Slow Render")) {
            g_slowRender = boolean((g_slowRenderCounter % 2) == 0);
            ++g_slowRenderCounter;
        }
        if(ImGui::Button("Restart Simulation")) {
            for(auto& tex : g_texture) {
                gl::glClearTexImage(tex, 0, gl::GL_RGBA, gl::GL_FLOAT, nullptr);
            }
            g_velocityCFL = vec2f{0.0f};
        }
        if(ImGui::Button("Reset Min-Max Frame")) {
            g_minFrameTime = 1000.0f;
            g_maxFrameTime = 0.0f;
        }
        ImGui::DragFloat2("Unit Coordinate Size", g_simUnitCoords.begin(), 0.1f, 0.1f, 100.0f);
        ImGui::DragFloat("Ambient Temperature",  &g_ambientTemperature, 0.01f, -40.0f,  40.0f, "%9.6f");
        ImGui::DragFloat("Delta Time",           &g_dt,                 0.001f, 0.001f, 0.5f,  "%9.6f");
        ImGui::DragFloat("Buoyancy ",            &g_buoyancy,           0.01f,  0.1f,   10.0f, "%9.6f");
        ImGui::DragFloat("Viscosity",            &g_viscosity,          0.01f,  0.1f,   10.0f, "%9.6f");
        ImGui::DragFloat("splatterRadius",       &g_splatterRadius,     0.001f, 0.001f, 0.2f,  "%9.6f");
        ImGui::DragInt("Diffusion Solver Iterations", &g_maximumJacobiIterations, 0.5f, 10, 80);
        ImVec4* cvtptr = nullptr;
        cvtptr = __rcast(ImVec4*, g_splatterForce.begin());
        ImGui::ColorButton("splatterForce ", *cvtptr);
        cvtptr = __rcast(ImVec4*, g_splatterColor.begin());
        ImGui::ColorButton("splatterColor ", *cvtptr);


        ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem("Diagnostics"))
    {
        ImGui::Text("\
Window     (%d, %d)\n\
Simulation (%d, %d)\n\
Mouse (Pressed = %u)\n\
    - Dragging Force %s\n\
    - Position       %s\n\
CFL Condition (< 1)  %9.6f (%6.4f, %6.4f) <=> Maximum Velocity\n\
FrameCounter         %u (%6.4f ms [min, max, avg] = [ %6.4f, %6.4f, %6.4f ] )\n\
Compute Time\n\
    %6.4f [render]\n\
    - %6.4f [fluid]\n\
        - %6.4f [velocity]\n\
        - %6.4f [dye]\n\
        - %6.4f [cfl]\n\
            - %6.4f [gpu_side]\n\
            - %6.4f [cpu_side]\n\
    - %6.4f [shader_refresh]\n\
    - %6.4f [render_imgui]\n\
    - %6.4f [compute_screen+blit]\n\
",
    g_windims.x, g_windims.y,
    g_dims.x, g_dims.y,
    g_mousePressed, dragForceStr, mouseDragPosStr,
    (cfl.x + cfl.y) * g_dt, g_velocityCFL.x, g_velocityCFL.y,
    g_frameCounter, __wholeFrame, 
    g_minFrameTime, g_maxFrameTime, g_avgFrameTime / __scast(f32, g_frameCounter),
    __rendering,
    __computefluid,
    __computevel,
    __computedye,
    __computecfl,
    __cflcpuside,
    __cflgpuside,
    __ref_shader,
    __imgui_render,
    __imgui_screen
);
        ImGui::EndTabItem();
    }    
    ImGui::EndTabBar();
    ImGui::End();

    free(mouseDragPosStr);
    free(dragForceStr);

    /* 
        g_velocityCFL is from the previous frame. 
        if I reset it after calculating it at compute_cfl() 
        then render_imgui_interface() won't have a chance to render it =>
        after I render the value, I zero it because it's only used for debug purposes now.
    */
    g_velocityCFL = vec2f{0.0f};
    return;
}
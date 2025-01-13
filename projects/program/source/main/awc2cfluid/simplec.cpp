#include "simplec.hpp"
#include <awc2/C/awc2.h>
#include <util/random.hpp>
#include <util/marker2.hpp>
#include <glbinding/gl/gl.h>
#include <imgui/imgui.h>
#include <algorithm>
#include <string>
#include <time.h>
#include <threads.h>


using namespace util::math;


void GenericNamespaceNameC::ParticleBufferManager::set()
{
    /* Set Default Values */
    static vec4f color{0.0f};
    static vec4f pos{0.0f};
    const vec2f dimensions_inv {
        1.0f / __scast(f32, mem->m_width),
        1.0f / __scast(f32, mem->m_height)
    };


    std::printf("Begin ParticleBufferManager print(%ux%u) => {", mem->m_width, mem->m_height);
    for(u32 i = 0; i < mem->m_width; ++i) {
        for(u32 j = 0; j < mem->m_height; ++j) 
        {
            pos = { __scast(f32, i), __scast(f32, j), 0.0f, 0.0f };
            color.x = pos.x * dimensions_inv.x;
            color.y = pos.y * dimensions_inv.y;
            mem->buffer[i * mem->m_height + j].position = pos;
            mem->buffer[i * mem->m_height + j].color    = color;
            // auto* colorstr = color.to_string();
            // auto* posstr   = pos.to_string();
            // std::printf("\n%s\n%s\n", colorstr, posstr);
            // std::free(colorstr);
            // std::free(posstr);
        }
    }
    std::printf("} End\n");
    return;
}



inline void render(
    GenericNamespaceNameC::OpenGLDataV2& gfxdata,
    GenericNamespaceNameC::frameTimeData& framedata,
    vec2i const& simDims
);




inline void custom_mousebutton_callback(AWC2User_callback_mousebutton_struct const* data)
{
    u8 state = (
        AWC2_MOUSEBUTTON_RIGHT == data->button &&
        AWC2_INPUTSTATE_PRESS  == data->action
    );
    if(state)
        awc2setCursorMode(AWC2_CURSORMODE_SCREEN_BOUND);
    else
        awc2setCursorMode(AWC2_CURSORMODE_NORMAL);


    return;
}


i32 render_fluid_awc2_c()
{
    bool alive{true}, paused{false};
    constexpr bool slowRender{false};
    AWC2ContextID   g_awc2id;
    __unused bool     g_shouldRestartSimulation;
    __unused bool     g_shouldRestartUserInputTexture;
    util::math::vec2i g_simDims;
    __unused GenericNamespaceNameC::frameTimeData g_timing;
    GenericNamespaceNameC::OpenGLDataV2 gfx;

    const struct timespec pause_sleep_duration{
        .tv_sec = 0,
        .tv_nsec = 6944444
    };
    const struct timespec slow_render_sleep_duration{
        .tv_sec = 0,
        .tv_nsec = 300 * 1000000
    };


    /* Init AWC2 */
    {
        markstr("AWC2 init begin");
        awc2init();
        g_awc2id = awc2createContext();
        // g_simDims = { 1280, 720 };
        g_simDims = { 720, 480 };
        AWC2ContextDescriptor ctxtinfo = {
            g_awc2id,
            {0},
            __scast(u16, g_simDims.x),
            __scast(u16, g_simDims.y),
            AWC2WindowDescriptor{}
        };
        awc2initializeContext(&ctxtinfo);
        awc2setContextUserCallbackMouseButton(g_awc2id, &custom_mousebutton_callback);
        markstr("AWC2 init end");
    }
    /* OpenGL Data Initialization */
    {
        markstr("Opengl Data init begin");
        /* This is Not Portable on Unix-like OS's. Problem is std::current_file::path()*/
        static constexpr const char* shaderName[2] = { "/old_sim.comp", "/old_visual.comp" };
#if defined __linux__
        static constexpr const char* dirName   = "projects/program/source/main/awc2cfluid";
#elif defined _WIN32
    static constexpr const char* dirName       = "C:/CTools/Projects/main/projects/program/source/main/awc2cfluid";
#endif

        static const std::string shaderPath[2] = { 
            std::string{dirName} + std::string{shaderName[0]},
            std::string{dirName} + std::string{shaderName[1]}, 
        };


        gfx.m_simulation.createFrom({
            ShaderData{ shaderPath[0].data(), __scast(u32, gl::GL_COMPUTE_SHADER) }
        });
        gfx.m_simulation.resizeLocalWorkGroup(0, { 1, 1, 1 });
        gfx.m_screenOutput.createFrom({
            ShaderData{ shaderPath[1].data(), __scast(u32, gl::GL_COMPUTE_SHADER) }
        });
        gfx.m_screenOutput.resizeLocalWorkGroup(0, { 1, 1, 1 });
        ifcrash(!gfx.m_simulation.compile());
        ifcrash(!gfx.m_screenOutput.compile());
        

        /* Initialize Initial Simulation Buffers */
        gfx.m_inputDye.create(g_simDims);
        gfx.m_inputDye.set();
        gfx.m_inputForces.resize(g_simDims.x * g_simDims.y, vec4f{});
        gfx.m_initialFields.resize(g_simDims.x * g_simDims.y);
        std::generate(
            gfx.m_initialFields.begin(),
            gfx.m_initialFields.end(),
            []() -> vec4f {
                vec2f result = { random32f(), random32f() };
                return vec4f{ result.x, result.y, 1.0f, 0.0f };
            }
        );
        // markstr("test");
        // for(auto& val : gfx.m_initialFields) {
        //     auto* p = val.to_string();
        //     std::printf("%s\n", p);
        //     std::free(p);
        // }
        /*
            fluidtex[1] - Dye, External Forces, etc ... (User Interaction)
            fluidtex[0] - Constanstly Swapped with fluidtex[2], Velocity Frames of the simulation
            fluidtex[2] - Constanstly Swapped with fluidtex[0], Velocity Frames of the simulation
            fluidtex[3] - Screen Texture (Coupled with fboid)
            fboid        - Screen Framebuffer, to see simulation output
            ssboparticle - Shader Storage Buffer Object for updating particles' data
        */
        gl::glCreateTextures(gl::GL_TEXTURE_2D, 4, gfx.m_fluidtex);
        gl::glCreateFramebuffers(1, &gfx.m_fboid);
        gl::glCreateBuffers(1, &gfx.m_ssboparticle);
        for(auto const& tex : gfx.m_fluidtex) {
            gl::glTextureParameteri(tex, gl::GL_TEXTURE_WRAP_S, gl::GL_CLAMP_TO_EDGE);
            gl::glTextureParameteri(tex, gl::GL_TEXTURE_WRAP_T, gl::GL_CLAMP_TO_EDGE);
            gl::glTextureParameteri(tex, gl::GL_TEXTURE_MIN_FILTER, gl::GL_LINEAR);
            gl::glTextureParameteri(tex, gl::GL_TEXTURE_MAG_FILTER, gl::GL_LINEAR);
            gl::glTextureStorage2D(tex, 1, gl::GL_RGBA32F, g_simDims.x, g_simDims.y);
        }
        
        gl::glTextureSubImage2D(gfx.m_fluidtex[0], 0, 0, 0, 
            g_simDims.x, 
            g_simDims.y, 
            gl::GL_RGBA, 
            gl::GL_FLOAT, 
            gfx.m_initialFields.data()
        );

        gl::glNamedFramebufferTexture(gfx.m_fboid, gl::GL_COLOR_ATTACHMENT0, gfx.m_fluidtex[3], 0);
        ifcrash( gl::glCheckNamedFramebufferStatus(gfx.m_fboid, gl::GL_FRAMEBUFFER) 
            != gl::GL_FRAMEBUFFER_COMPLETE 
        );

        gl::glNamedBufferStorage(gfx.m_ssboparticle, 
            gfx.m_inputDye.bytes_alloc(), 
            gfx.m_inputDye.data(), 
            gl::GL_DYNAMIC_STORAGE_BIT
        );
        gl::glBindBufferBase(gl::GL_SHADER_STORAGE_BUFFER, 5, gfx.m_ssboparticle);
        gfx.m_screenOutput.StorageBlock("ParticlesForVisualization", 5);


        std::swap(
            gfx.m_fluidtex[0], 
            gfx.m_fluidtex[2]
        ); /* Swap Texture ID's for I/O */
        markstr("Opengl Data init end");
    }


    /* Main App Loop */
    markstr("Main App Loop Begin");
    awc2setCurrentContext(g_awc2id);
    while(alive)
    {
        awc2newframe();
        awc2begin();
        if(paused) {
            thrd_sleep(&pause_sleep_duration, NULL);
        } else {
            if constexpr (slowRender) {
                thrd_sleep(&slow_render_sleep_duration, NULL);
            }
            gfx.m_refreshComputeSim    ^= awc2isKeyPressed(AWC2_KEYCODE_NUM1);
            gfx.m_refreshComputeVisual ^= awc2isKeyPressed(AWC2_KEYCODE_NUM2);
            render(gfx, g_timing, g_simDims);
        }
        alive   = !awc2getContextStatus(g_awc2id) && !awc2isKeyPressed(AWC2_KEYCODE_ESCAPE);
        paused ^= awc2isKeyPressed(AWC2_KEYCODE_P);
        awc2end();
        ++g_timing.m_frameCount;
    }
    markstr("Main App Loop End");



    /* Destroy OpenGL Data */
    { 
        markstr("Destroy OpenGL data Begin");
        gl::glDeleteTextures(4, gfx.m_fluidtex);
        gl::glDeleteBuffers(1, &gfx.m_ssboparticle);
        gl::glDeleteFramebuffers(1, &gfx.m_fboid);
        gfx.m_simulation.destroy();
        gfx.m_screenOutput.destroy();
        gfx.m_initialFields.resize(0);
        gfx.m_inputForces.resize(0);
        gfx.m_inputDye.destroyCpuSide();
        markstr("Destroy OpenGL data End");
    }
    /* Destroy AWC2 Context & State */
    {
        markstr("Destroy AWC2 Context Begin");
        g_simDims = { DEFAULT32, DEFAULT32 };
        g_shouldRestartUserInputTexture = false;
        g_shouldRestartSimulation = false;
        awc2destroyContext(g_awc2id);
        awc2destroy();
        g_awc2id = DEFAULT8;
        markstr("Destroy AWC2 Context End");
    }
    return g_awc2id;
}




inline void render(
    GenericNamespaceNameC::OpenGLDataV2& gfxdata,
    GenericNamespaceNameC::frameTimeData& framedata,
    vec2i const& simDims
) {
    auto& gfx = gfxdata;
    auto winSize = awc2getCurrentContextViewport();
    const vec4f rgba{0.0f, 0.0f, 0.0f, 1.0f};
    u8 recompiling{0};
    u8 status{1};


    if(gfx.m_refreshComputeSim) {
        gfx.m_simulation.refreshFromFiles();
        gfx.m_simulation.resizeLocalWorkGroup(0, { 1, 1, 1 });
        recompiling = 1;
        status = recompiling * gfx.m_simulation.compile();
        gfx.m_refreshComputeSim = !(status == 1);
    }
    if(gfx.m_refreshComputeVisual) {
        gfx.m_screenOutput.refreshFromFiles();
        gfx.m_screenOutput.resizeLocalWorkGroup(0, { 1, 1, 1 });
        recompiling = 2;
        status = recompiling * gfx.m_screenOutput.compile();
        gfx.m_refreshComputeVisual = !(status == 2);
    }
    if(recompiling && status == 0) {
        return;
    }


    /* Render ImGui Window */
    {
        static f64 frameTimeDouble  = framedata.frame .value_units<f64>(1e+3);
        static f64 gameTimeDouble   = framedata.game  .value_units<f64>(1e+3);
        static f64 renderTimeDouble = framedata.render.value_units<f64>(1e+3);
        static f64 fps = 1e+3f / frameTimeDouble;


        ImGui::Begin("Program Statistics");
        ImGui::Text("\
Previous Frame Statistics:\n \
Frame Counter 		 %u\n \
Frame Time 		     %3.5f [ms]\n \
Game   State %3.5f [ms] (%2.2f%%)\n \
Render State %3.5f [ms] (%2.2f%%)\n \
Frames Per Second    %u\n \
Interpolation Factor %3.5f (now)\n",
            framedata.m_frameCount, 
            frameTimeDouble, 
            gameTimeDouble, 
            100.0f * (gameTimeDouble / frameTimeDouble), 
            renderTimeDouble, 
            100.0f * (renderTimeDouble / frameTimeDouble), 
            __scast(u32, fps),
            framedata.m_interpolate_frame
        );
        ImGui::End();
    }


    /* Clear Screen */
    gl::glClearNamedFramebufferfv(gfx.m_fboid, gl::GL_COLOR, 0, rgba.begin());
    std::swap(
        gfx.m_fluidtex[0], 
        gfx.m_fluidtex[2]
    ); /* Swap Texture ID's for I/O */
    gfx.m_simulation.bind();
    gfx.m_simulation.uniform1i("infield",        0);
    gfx.m_simulation.uniform1i("externalForces", 1);
    gfx.m_simulation.uniform1i("outfield",       2);
    gl::glBindTextureUnit(0, gfx.m_fluidtex[0]);
    gl::glBindTextureUnit(1, gfx.m_fluidtex[1]);
    gl::glBindImageTexture(2, gfx.m_fluidtex[2], 0, false, 0, 
        gl::GL_WRITE_ONLY, 
        gl::GL_RGBA32F
    );
    gl::glMemoryBarrier(gl::GL_ALL_BARRIER_BITS);
    gl::glDispatchCompute(simDims.x, simDims.y, 1);


    /* outfield -> 3; 3 -> fluidtex[2] | outfield => fluidtex[2] | becomes the input */
    /* outpos   -> 4; 4 -> fluidtex[3] | outpos   => fluidtex[3] | output to screen  */
    gfx.m_screenOutput.bind();
    gfx.m_screenOutput.uniform1i("outfield", 3);
    gfx.m_screenOutput.uniform1i("outpos",   4);
    gl::glBindTextureUnit(3, gfx.m_fluidtex[2]);
    gl::glBindImageTexture(4, gfx.m_fluidtex[3], 0, false, 0, 
        gl::GL_WRITE_ONLY, 
        gl::GL_RGBA32F
    );
    gl::glDispatchCompute(simDims.x, simDims.y, 1);
    gl::glMemoryBarrier(gl::GL_ALL_BARRIER_BITS);


    /* Draw Call */
    gl::glBlitNamedFramebuffer(gfx.m_fboid, 0, 
        0, 0, simDims.x, simDims.y, 
        0, 0, winSize.x, winSize.y,
        gl::GL_COLOR_BUFFER_BIT, 
        gl::GL_LINEAR
    );
    return;
}
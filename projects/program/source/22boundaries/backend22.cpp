#include "backend22.hpp"
#include <awc2/C/awc2.h>
#include "vars.hpp"


using namespace boundary22;


static void compute_velocity(
    u32 previousIterDye,
    u32 prevBoundaries, 
    u32 nextBoundaries,
    u32 previousIteration, 
    u32 nextIteration
);
static void compute_dye(
    u32 velocityPressureField,
    u32 currBoundaries,
    u32 previousIterationDye, 
    u32 nextIterationDye
);
static void compute_cfl(u32 texture);



u32 boundary22::compute_fluid()
{
    u32 previousIterationVel, nextIterationVel;
    u32 previousIterationDye, nextIterationDye;
    u32 prevIterBoundary, nextIterBoundary;
    if(g_frameCounter % 2) {
        previousIterationVel = gr_simTexture1;
        nextIterationVel     = gr_simTexture0;
        previousIterationDye = gr_dyeTexture1;
        nextIterationDye     = gr_dyeTexture0;
        prevIterBoundary       = gr_bndryTexture1;
        nextIterBoundary       = gr_bndryTexture0;
    } else {
        previousIterationVel = gr_simTexture0;
        nextIterationVel     = gr_simTexture1;
        previousIterationDye = gr_dyeTexture0;
        nextIterationDye     = gr_dyeTexture1;
        prevIterBoundary       = gr_bndryTexture0;
        nextIterBoundary       = gr_bndryTexture1;
    }


    TIME_NAMESPACE_TIME_CODE_BLOCK(g_computeVelTime, compute_velocity(
        previousIterationDye,
        prevIterBoundary, nextIterBoundary,
        previousIterationVel, nextIterationVel
    ));
    TIME_NAMESPACE_TIME_CODE_BLOCK(g_computeDyeTime, compute_dye(
        nextIterationVel, 
        nextIterBoundary,
        previousIterationDye, nextIterationDye
    ));
    TIME_NAMESPACE_TIME_CODE_BLOCK(g_computeCFLTime, compute_cfl(nextIterationVel));
    
    u32 resultTex = 0;
    switch(g_chooseTextureToRender) {
        case 1: resultTex = nextIterationVel; break;
        case 2: resultTex = nextIterationDye; break;
        case 3: resultTex = nextIterationVel; break; 
        case 4: resultTex = nextIterationVel; break;
        default: 
        break;
    }
    return resultTex;
}


static void compute_velocity(
    u32 previousIterDye,
    u32 prevBoundaries, 
    u32 nextBoundaries,
    u32 previousIteration, 
    u32 nextIteration
) {
    vec4f splatterval{0.0f};
    u32 interactionTexIn{0}, interactionTexOut{0}; 


    switch(g_chooseUserDrawFillType) {
        case 0:
        splatterval = g_splatterColor;
        interactionTexIn  = previousIterDye;
        interactionTexOut = gr_outTexShader6;
        break;
        case 1:
        splatterval = g_splatterForce;
        interactionTexIn  = previousIteration;
        interactionTexOut = gr_outTexShader0;
        break;
        case 2:
        interactionTexIn  = prevBoundaries;
        interactionTexOut = nextBoundaries;
        break;
        default:
        break;
    };
    gr_computeInteractive.bind();
    gr_computeInteractive.uniform1i("previousFrame", 0);
    gr_computeInteractive.uniform1i("nextFrame",     1);
    gr_computeInteractive.uniform2iv("ku_simdims",        g_dims.begin());
    gr_computeInteractive.uniform2iv("ku_windims",        g_windowSize.begin());
    gr_computeInteractive.uniform1f("ku_dt",              g_normdt);
    gr_computeInteractive.uniform1f("ku_splatterRadius",  g_splatterRadius);
    gr_computeInteractive.uniform2fv("ku_mouseDragForce", g_mousedxdy.begin()    );
    gr_computeInteractive.uniform2fv("ku_mouseDragPos",   g_mousePosition.begin());
    gr_computeInteractive.uniform4fv("ku_splatterValue",  splatterval.begin());
    gr_computeInteractive.uniform1ui("ku_mousePressed",   g_mousePressRight);
    gr_computeInteractive.uniform1ui("ku_chooseInteractionType", g_chooseUserDrawFillType);
    gl::glBindTextureUnit(0, interactionTexIn);
    gl::glBindImageTexture(1, interactionTexOut, 0, false, 0, 
        gl::GL_WRITE_ONLY,
        gl::GL_RGBA32F
    );
    gl::glDispatchCompute(g_computeInvocationSize.x, g_computeInvocationSize.y, g_computeInvocationSize.z);
    gl::glMemoryBarrier(gl::GL_ALL_BARRIER_BITS);


    gr_computeAdvection.bind();
    gr_computeAdvection.uniform1i("boundaryField", 0);
    gr_computeAdvection.uniform1i("quantityField", 1);
    gr_computeAdvection.uniform1i("velocityField", 2);
    gr_computeAdvection.uniform1i("outputField",   3);
    gr_computeAdvection.uniform1ui("ku_writeAllComponents", false);
    gr_computeAdvection.uniform1f("ku_dt",                  g_normdt);
    gr_computeAdvection.uniform2fv("ku_simUnitCoord",       g_simUnitCoords.begin());
    gr_computeAdvection.uniform2iv("ku_simdims",            g_dims.begin());
    gl::glBindTextureUnit(0, nextBoundaries);
    gl::glBindTextureUnit(1, (g_chooseUserDrawFillType == 1) ? gr_outTexShader0 : previousIteration);
    gl::glBindTextureUnit(2, previousIteration);
    gl::glBindImageTexture(3, gr_outTexShader1, 0, false, 0, 
        gl::GL_WRITE_ONLY, 
        gl::GL_RGBA32F
    );
    gl::glDispatchCompute(g_computeInvocationSize.x, g_computeInvocationSize.y, g_computeInvocationSize.z);
    gl::glMemoryBarrier(gl::GL_ALL_BARRIER_BITS);


    /* Compute diffuse component of the velocity field to diffusion texture (gr_outTexShader2) */
    u32 textureInput, textureOutput;
    gr_computeDiffusionVel.bind();
    gr_computeDiffusionVel.uniform1i("boundaryField", 0);
    gr_computeDiffusionVel.uniform1i("initialField", 1);
    gr_computeDiffusionVel.uniform1i("outputField",  2);
    gr_computeDiffusionVel.uniform1f("ku_dt",            g_normdt);
    gr_computeDiffusionVel.uniform1f("ku_viscosity",     g_kinematicViscosity);
    gr_computeDiffusionVel.uniform2iv("ku_simdims",      g_dims.begin());
    gr_computeDiffusionVel.uniform2fv("ku_simUnitCoord", g_simUnitCoords.begin());
    gl::glBindTextureUnit(0, nextBoundaries);
    gl::glBindTextureUnit(1, gr_outTexShader1);
    gl::glBindImageTexture(2, gr_outTexShader2, 0, false, 0, 
        gl::GL_WRITE_ONLY, 
        gl::GL_RGBA32F
    );
    gl::glDispatchCompute(g_computeInvocationSize.x, g_computeInvocationSize.y, g_computeInvocationSize.z);
    gl::glMemoryBarrier(gl::GL_ALL_BARRIER_BITS);


    textureInput  = gr_outTexShader2;
    textureOutput = gr_tmpTexture0;
    for(i32 i = 1; i < g_maximumJacobiIterations; ++i) {
        gl::glBindTextureUnit(1, textureInput);
        gl::glBindImageTexture(2, textureOutput, 0, false, 0, 
            gl::GL_WRITE_ONLY, 
            gl::GL_RGBA32F
        );
        gl::glDispatchCompute(g_computeInvocationSize.x, g_computeInvocationSize.y, g_computeInvocationSize.z);


        u32 tmp = textureInput;
        textureInput  = textureOutput;
        textureOutput = tmp;
        gl::glMemoryBarrier(gl::GL_ALL_BARRIER_BITS);
    }


    gr_computeDivergence.bind();
    gr_computeDivergence.uniform1i("intermediateVelocityField", 0);
    gr_computeDivergence.uniform1i("oldPressureFieldValue",     1);
    gr_computeDivergence.uniform1i("outputField",               2);
    gr_computeDivergence.uniform2fv("ku_simUnitCoord", g_simUnitCoords.begin());
    gl::glBindTextureUnit(0, gr_outTexShader2);
    gl::glBindTextureUnit(1, previousIteration);
    gl::glBindImageTexture(2, gr_outTexShader3, 0, false, 0, 
        gl::GL_WRITE_ONLY, 
        gl::GL_RGBA32F
    );
    gl::glDispatchCompute(g_computeInvocationSize.x, g_computeInvocationSize.y, g_computeInvocationSize.z);
    gl::glMemoryBarrier(gl::GL_ALL_BARRIER_BITS);



    /* Compute the new pressure to .z component (gr_outTexShader5) */
    gr_computeDiffusionPressure.bind();
    gr_computeDiffusionPressure.uniform1i("boundaryField", 0);
    gr_computeDiffusionPressure.uniform1i("oldPressureField", 1);
    gr_computeDiffusionPressure.uniform1i("newPressureField", 2);
    gr_computeDiffusionPressure.uniform2iv("ku_simdims", g_dims.begin());
    gr_computeDiffusionPressure.uniform2fv("ku_simUnitCoord", g_simUnitCoords.begin());
    gl::glBindTextureUnit(0, nextBoundaries);
    gl::glBindTextureUnit(1, gr_outTexShader3);
    gl::glBindImageTexture(2, gr_outTexShader4, 0, false, 0, 
        gl::GL_WRITE_ONLY, 
        gl::GL_RGBA32F
    );
    gl::glDispatchCompute(g_computeInvocationSize.x, g_computeInvocationSize.y, g_computeInvocationSize.z);
    gl::glMemoryBarrier(gl::GL_ALL_BARRIER_BITS);


    textureInput  = gr_outTexShader4;
    textureOutput = gr_tmpTexture0;
    for(i32 i = 1; i < g_maximumJacobiIterations; ++i) {
        gl::glBindTextureUnit(1, textureInput);
        gl::glBindImageTexture(2, textureOutput, 0, false, 0, 
            gl::GL_WRITE_ONLY, 
            gl::GL_RGBA32F
        );
        gl::glDispatchCompute(g_computeInvocationSize.x, g_computeInvocationSize.y, g_computeInvocationSize.z);


        u32 tmp = textureInput;
        textureInput  = textureOutput;
        textureOutput = tmp;
        gl::glMemoryBarrier(gl::GL_ALL_BARRIER_BITS);
    }


    /* add together all the shit we computed for the next iteration */
    gr_computeNewVelocity.bind();
    gr_computeNewVelocity.uniform1i("intermediate",    0);
    gr_computeNewVelocity.uniform1i("updatedFields",   1);
    gr_computeNewVelocity.uniform2fv("ku_simUnitCoord", g_simUnitCoords.begin());
    gl::glBindTextureUnit(0, gr_outTexShader4);
    gl::glBindImageTexture(1, nextIteration, 0, false, 0, 
        gl::GL_WRITE_ONLY, 
        gl::GL_RGBA32F
    );
    gl::glDispatchCompute(g_computeInvocationSize.x, g_computeInvocationSize.y, g_computeInvocationSize.z);
    gl::glMemoryBarrier(gl::GL_ALL_BARRIER_BITS);
    return;
}


static void compute_dye(
    u32 velocityPressureField,
    u32 currBoundaries,
    u32 previousIterationDye, 
    u32 nextIterationDye
) {
    gr_computeAdvection.bind();
    gr_computeAdvection.uniform1i("boundaryField", 0);
    gr_computeAdvection.uniform1i("quantityField", 1);
    gr_computeAdvection.uniform1i("velocityField", 2);
    gr_computeAdvection.uniform1i("outputField",   3);
    gr_computeAdvection.uniform1ui("ku_writeAllComponents", true);
    gr_computeAdvection.uniform1f("ku_dt",                  g_normdt);
    gr_computeAdvection.uniform2fv("ku_simUnitCoord",       g_simUnitCoords.begin());
    gr_computeAdvection.uniform2iv("ku_simdims",            g_dims.begin());
    gl::glBindTextureUnit(0, currBoundaries);
    gl::glBindTextureUnit(1, (g_chooseUserDrawFillType == 0) ? gr_outTexShader6 : previousIterationDye);
    gl::glBindTextureUnit(2, velocityPressureField);
    gl::glBindImageTexture(3, nextIterationDye, 0, false, 0, 
        gl::GL_WRITE_ONLY, 
        gl::GL_RGBA32F
    );
    gl::glDispatchCompute(g_computeInvocationSize.x, g_computeInvocationSize.y, g_computeInvocationSize.z);
    gl::glMemoryBarrier(gl::GL_ALL_BARRIER_BITS);
    return;
}


static void compute_cfl(u32 texture)
{
    auto jamAFenceToWaitForGPUBufferVisibility = []() {
        if(g_fence) {
            gl::glDeleteSync(g_fence);
        }
        g_fence = gl::glFenceSync(gl::GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
        return;
    };
    g_computeMaximumGPU.begin();
    gl::glBindBufferBase(gl::GL_SHADER_STORAGE_BUFFER, 1, g_reductionMaxTexture);
    gl::glBindTextureUnit(0, texture);
    gr_computeCFLCondition.bind();
    gr_computeCFLCondition.uniform1i("originalTexture", 0);
    gr_computeCFLCondition.StorageBlock("reductionMaximumBuffer", 1);
    gr_computeCFLCondition.uniform1i("ku_valuesToFetch", g_reductionFactor);
    gl::glDispatchCompute(g_reductionBufferLength, 1, 1);
    gl::glMemoryBarrier(gl::GL_ALL_BARRIER_BITS);
    

    jamAFenceToWaitForGPUBufferVisibility();
    g_computeMaximumGPU.end();


    g_computeMaximumCPU.begin();
    // static char static_charbuf[512]{0};
    // if(g_mousePressed && awc2isMouseButtonPressed(AWC2_MOUSEBUTTON_RIGHT)) {
    //     for(i32 i = 0; i < g_reductionBufferLength; ++i) {
    //         mappedptr[i].to_strbuf(&static_charbuf[0], 512);
    //         printf("  %u => %s\n", i, static_charbuf);
    //     }
    // }

    g_maxVelocity = vec2f{0.0f};
    bool biggerX, biggerY;
    auto* mappedptr = __rcast(vec4f*, g_reductionMaxMappedBuf);
    for(i32 i = 0; i < g_reductionBufferLength; ++i) {
        biggerX = boolean(mappedptr[i].x > g_maxVelocity.x);
        biggerY = boolean(mappedptr[i].y > g_maxVelocity.y);
        g_maxVelocity.x = __scast(f32, biggerX) * mappedptr[i].x + __scast(f32, !biggerX) * g_maxVelocity.x;
        g_maxVelocity.y = __scast(f32, biggerY) * mappedptr[i].y + __scast(f32, !biggerY) * g_maxVelocity.y;
    }
    g_computeMaximumCPU.end();
    return;
}
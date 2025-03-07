#pragma once
#include <vector>
#include <util/types.h>
#include <glbinding/gl/gl.h>




struct ShaderStorageBufferObject 
{
    u32 id;
    u32 shaderProgramID;
    u32 shaderBindingPoint; /* a table of 'references' to buffer objects. */
    u32 shaderBlockIndex;   /* I DONT FUCKING KNOW WHY THIS IS IMPORTANT DON'T ASK ME IM SETTING THIS TO 0 */


    void create(
        std::vector<f32>& uploadToGPU,
        u32 programID, 
        u32 bindingPoint,
        u32 blockIndex = 0u
    ) {
        shaderProgramID    = programID;
        shaderBindingPoint = bindingPoint;
        shaderBlockIndex   = blockIndex;
        gl()->CreateBuffers(1, &id);
        gl()->NamedBufferSubData(id, 0, uploadToGPU.size() * sizeof(f32), uploadToGPU.data());


        gl()->BindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingPoint, id);
        gl()->ShaderStorageBlockBinding(programID, shaderBlockIndex, bindingPoint);
        return;
    }


    /* Still don't know if I should clear the shader Indices so I'm leaving this const for now. */
    void unbind() const {
        gl()->BindBufferBase(GL_SHADER_STORAGE_BUFFER, shaderBindingPoint, 0);
        gl()->ShaderStorageBlockBinding(shaderProgramID, shaderBlockIndex, 0);
        return;
    }


    void bind() const { 
        gl()->BindBufferBase(GL_SHADER_STORAGE_BUFFER, shaderBindingPoint, id);
        gl()->ShaderStorageBlockBinding(shaderProgramID, shaderBlockIndex, shaderBindingPoint);
        return;
    }


    void destroy() { gl()->DeleteBuffers(1, &id); id = DEFAULT32; return; }
};
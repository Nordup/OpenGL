#include "Renderer.h"
#include <iostream>

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLCheckError(const char* function, const char* file, int line)
{
    bool first_error = true;
    while (GLenum error = glGetError())
    {
        if (first_error)
        {
            first_error = false;
            std::cout << "Error in " << file << " func " << function << " line: " << std::dec << line << std::endl;
        }
        std::cout << "\t [OpenGL error] (0x" << std::hex << error << ")" << std::endl;
    }
    if (!first_error)
        return false;
    return true;
}

void Renderer::Clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    shader.Bind();
    va.Bind();
    ib.Bind();
    
    /* Draw our result */
    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}
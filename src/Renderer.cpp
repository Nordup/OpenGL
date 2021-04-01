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
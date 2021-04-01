#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>
#include <cstdlib>

#define DTS(arg) #arg
#define DEFINE_TO_SRT(arg) DTS(arg)


#define GLCall(x)   GLClearError(); \
                    x; \
                    if (!GLCheckError(#x, __FILE__, __LINE__)) std::exit(-1);

void GLClearError();


bool GLCheckError(const char* function, const char* file, int line);

#endif

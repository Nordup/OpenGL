#include <GL/glew.h> // load opengl functions from gpu drivers on runtime crossplatform
#include <GLFW/glfw3.h> // create crossplatform window

#include <cstdio>
#include <iostream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

#ifndef GLEW_STATIC
int     main()
{
    std::cout << "GLEW_STATIC is not defined" << std::endl;
    return 0;
}
#else
int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;
    
    /* configure glew */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(600, 600, "OpenGL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    /* initialize GLEW */
    GLenum err = glewInit();
	if (GLEW_OK != err)
	{
	  /* Problem: glewInit failed, something is seriously wrong. */
	  fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
    
    /* resize opengl viewport on every window size changing */
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    /* print OpenGL version*/
    std::cout << "OpenGL vesion: " << glGetString(GL_VERSION) << std::endl;




    /* vertex array object */
    VertexArray va;

    /* Create OpenGL buffer*/
    float positions[] = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,
         -0.5f, 0.5f,
    }; // our verticies
    VertexBuffer vb(positions, 4 * 2 *sizeof(float));

    /* create layout */
    VertexBufferLayout layout;
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);

    /* create index buffer */
    unsigned int indicies[] = {
        0, 1, 2,
        2, 3, 0
    };
    IndexBuffer ib(indicies, 6);

    /* create shaders */
    std::string project_dir = DEFINE_TO_SRT(PROJECT_DIR);
    Shader shader(project_dir + "/resources/shaders/basic.shader");
    shader.Bind();
    shader.SetUnifrom4f("u_Color", 0.9, 0.3, 0.5, 0.7);


    /* unbind all */
    va.Unbind();
    vb.Unbind();
    ib.Unbind();
    shader.Unbind();

    float r = 0.0f;
    float increment = 0.01f;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        /* Bind what we need */
        shader.Bind();
        shader.SetUnifrom4f("u_Color", r, 0.3, 0.5, 0.7);

        va.Bind();
        ib.Bind();

        /* Draw our result */
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        /* change color */
        if (r > 1.0f)
            increment = -0.01f;
        else if (r < 0.0f)
            increment = 0.01f;
        r += increment;

        /* Swap front and back buffers */
        GLCall(glfwSwapBuffers(window));

        /* Poll for and process events */
        GLCall(glfwPollEvents());
    }


    glfwTerminate();
    return 0;
}
#endif
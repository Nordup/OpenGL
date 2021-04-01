#include <GL/glew.h> // load opengl functions from gpu drivers on runtime crossplatform
#include <GLFW/glfw3.h> // create crossplatform window

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

int InitializeLibs(GLFWwindow*& window);

void ProcessInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

#ifdef GLEW_STATIC
int main(void)
{
    /* create window, load OpenGL */
    GLFWwindow* window;
    if (InitializeLibs(window) != 0)
        return -1;


    float positions[] = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,
         -0.5f, 0.5f,
    }; // our verticies
    VertexBuffer vb(positions, 4 * 2 *sizeof(float));

    VertexBufferLayout layout;
    layout.Push<float>(2);

    VertexArray va;
    va.AddBuffer(vb, layout);

    unsigned int indicies[] = {
        0, 1, 2,
        2, 3, 0
    };
    IndexBuffer ib(indicies, 6);

    std::string project_dir = DEFINE_TO_SRT(PROJECT_DIR);
    Shader shader(project_dir + "/resources/shaders/basic.shader");
    shader.Bind();
    shader.SetUnifrom4f("u_Color", 0.9, 0.3, 0.5, 0.7);

    Renderer renderer;


    /* unbind all */
    va.Unbind();
    vb.Unbind();
    ib.Unbind();
    shader.Unbind();

    float r = 0.0f;
    float increment = 0.05f;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        // input
        ProcessInput(window);
        // clear window
        renderer.Clear();

        /* Bind what we need */
        shader.Bind();
        shader.SetUnifrom4f("u_Color", r, 0.3, 0.5, 0.7);

        renderer.Draw(va, ib, shader);

        /* change color */
        if (r > 1.0f)
            increment = -0.05f;
        else if (r < 0.0f)
            increment = 0.05f;
        r += increment;

        // check and call events and swap the buffers
        GLCall(glfwSwapBuffers(window));
        GLCall(glfwPollEvents());
    }


    glfwTerminate();
    return 0;
}


#else
int     main()
{
    std::cout << "GLEW_STATIC is not defined" << std::endl;
    return 0;
}
#endif

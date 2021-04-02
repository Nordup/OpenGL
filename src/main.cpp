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
    
    // change PolygonMode
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
}

#ifdef GLEW_STATIC
int main(void)
{
    // create window, load OpenGL
    // ---------------------------------
    GLFWwindow* window;
    if (InitializeLibs(window) != 0)
        return -1;


    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float positions[] = {
        // positions    // colors
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
         -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.5f,
    }; // our verticies
    VertexBuffer vb(positions, 4 * 6 *sizeof(float));

    VertexBufferLayout layout;
    layout.Push<float>(3);
    layout.Push<float>(3);

    VertexArray va;
    va.AddBuffer(vb, layout);

    unsigned int indicies[] = {
        0, 1, 2,
        2, 3, 0
    };
    IndexBuffer ib(indicies, 6);

    std::string project_dir = DEFINE_TO_SRT(PROJECT_DIR);
    Shader shader(project_dir + "/res/shaders/basic.shader");
    shader.Bind();
    shader.SetUnifrom4f("u_Color", 1, 1, 1, 1);

    Renderer renderer;


    // unbind all
    /// --------------
    va.Unbind();
    vb.Unbind();
    ib.Unbind();
    shader.Unbind();


    // render loop
    // --------------------------------------------------
    float r = 0.0f;
    float increment = 0.02f;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        // input
        //-----------------------
        ProcessInput(window);
        // clear window
        renderer.Clear();

        // Bind what we need
        // ----------------------
        shader.Bind();
        shader.SetUnifrom4f("u_Color", r, r, r, r);

        renderer.Draw(va, ib, shader);

        /* change color */
        if (r > 1.0f)
            increment = -0.02f;
        else if (r < 0.0f)
            increment = 0.02f;
        r += increment;

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        GLCall(glfwSwapBuffers(window));
        GLCall(glfwPollEvents());
    }


    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
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

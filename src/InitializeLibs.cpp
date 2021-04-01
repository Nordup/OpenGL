#include <GL/glew.h> // load opengl functions from gpu drivers on runtime crossplatform
#include <GLFW/glfw3.h> // create crossplatform window

#include <cstdio>
#include <iostream>


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int InitializeLibs(GLFWwindow*& window)
{
    /* Initialize the library */
    if (!glfwInit())
        return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    /* resize opengl viewport on every window size changing */
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    /* initialize GLEW */
    GLenum err = glewInit();
	if (GLEW_OK != err)
	{
        /* Problem: glewInit failed, something is seriously wrong. */
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        return -1;
	}
    /* print OpenGL version*/
    std::cout << "OpenGL vesion: " << glGetString(GL_VERSION) << std::endl;


    return 0;
}
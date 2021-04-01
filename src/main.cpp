#include <GL/glew.h> // load opengl functions from gpu drivers on runtime crossplatform
#include <GLFW/glfw3.h> // create crossplatform window

#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result != GL_TRUE)
    {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)alloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message));

        std::cout << "Failed to Compile " << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment") << " Shader!"<< std::endl;
        std::cout << message << std::endl;

        GLCall(glDeleteShader(id));
        return 0;
    }

    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);
    std::string line;

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    ShaderType type = ShaderType::NONE;
    std::stringstream ss[2];
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else
        {
            if (type != ShaderType::NONE)
                ss[(int)type] << line << "\n";
        }
    }

    return { ss[0].str(), ss[1].str() };
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
    ShaderProgramSource shaderPSource = ParseShader(project_dir + "/resources/shaders/basic.shader");

    unsigned int shader = CreateShader(shaderPSource.VertexSource, shaderPSource.FragmentSource);
    GLCall(glUseProgram(shader));

    /* uniforms */
    GLCall(int location = glGetUniformLocation(shader, "u_Color"));
    if (location == -1)
        std::cout << "error: cannot find 'u_Collor'" << std::endl;
    GLCall(glUniform4f(location, 0.9, 0.3, 0.5, 0.7));

    /* unbind all */
    va.Unbind();
    vb.Unbind();
    ib.Unbind();
    GLCall(glUseProgram(0));


    float r = 0.0f;
    float increment = 0.01f;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        /* Bind what we need */
        GLCall(glUseProgram(shader));
        GLCall(glUniform4f(location, r, 0.3, 0.5, 0.7));

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

    GLCall(glDeleteProgram(shader));

    glfwTerminate();
    return 0;
}
#endif
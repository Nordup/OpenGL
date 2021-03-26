#include <GL/glew.h> // load opengl functions from gpu drivers on runtime crossplatform
#include <GLFW/glfw3.h> // create crossplatform window

#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>

#define DTS(arg) #arg
#define DEFINE_TO_SRT(arg) DTS(arg)


#define GLCall(x)   GLClearError(); \
                    x; \
                    if (!GLCheckError(#x, __FILE__, __LINE__))

static void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

static bool GLCheckError(const char* function, const char* file, int line)
{
    bool first_error = true;
    while (GLenum error = glGetError())
    {
        if (first_error)
        {
            first_error = false;
            std::cout << "Error in " << file << " func " << function << " line: " << line << std::endl;
        }
        std::cout << "\t [OpenGL error] (0x" << std::hex << error << ")" << std::endl;
    }
    if (!first_error)
        return false;
    return true;
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

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1920, 1080, "OpenGL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    
    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* initialize GLEW */
    GLenum err = glewInit();
	if (GLEW_OK != err)
	{
	  /* Problem: glewInit failed, something is seriously wrong. */
	  fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
    
    /* print OpenGL version*/
    std::cout << "OpenGL vesion: " << glGetString(GL_VERSION) << std::endl;

    /* Create OpenGL buffer*/
    float positions[] = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,
         -0.5f, 0.5f,
    }; // our verticies

    unsigned int buffer;
    GLCall(glGenBuffers(1, &buffer));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    GLCall(glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), positions, GL_STATIC_DRAW));

    /* create vertex attribute */
    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));

    /* create index buffer */
    unsigned int indicies[] = {
        0, 1, 2,
        2, 3, 0
    };
    unsigned int ibo;
    GLCall(glGenBuffers(1, &ibo));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indicies, GL_STATIC_DRAW));

    /* create shaders */
    std::string project_dir = DEFINE_TO_SRT(PROJECT_DIR);
    ShaderProgramSource shaderPSource = ParseShader(project_dir + "/resources/shaders/basic.shader");

    unsigned int shader = CreateShader(shaderPSource.VertexSource, shaderPSource.FragmentSource);
    GLCall(glUseProgram(shader));

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        /* Draw our result */
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

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
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"

struct ShaderProgramSource
{
    std::string VertexSource, FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)           // vertex shader
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)    // fragment shader
                type = ShaderType::FRAGMENT;
        }
        else
        {
            ss[static_cast<int>(type)] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, NULL));
    GLCall(glCompileShader(id));

    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result != GL_TRUE)
    {
        //std::cout << "Shader Compiler Error! \n";

        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));

        char* message = static_cast<char*>(alloca(length * sizeof(char)));
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!\n";
        std::cout << message << std::endl;
        GLCall(glDeleteShader(id));

        return 0;
    }

    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    GLCall(unsigned int program = glCreateProgram());
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    int linkResult, validateResult;
    GLCall(glGetProgramiv(program, GL_LINK_STATUS, &linkResult));
    GLCall(glGetProgramiv(program, GL_VALIDATE_STATUS, &validateResult));

    if (linkResult != GL_TRUE || validateResult != GL_TRUE)
    {
        int length;
        GLCall(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length));

        char* message = static_cast<char*>(alloca(length * sizeof(char)));
        GLCall(glGetProgramInfoLog(program, length, &length, message));
        std::cout << "Error in creating Program! \n    " << message << std::endl;

    }

    GLCall(glDetachShader(program, vs));
    GLCall(glDeleteShader(vs));

    GLCall(glDetachShader(program, fs));
    GLCall(glDeleteShader(fs));

    return program;
}



int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "Error!\n";

    std::cout << glGetString(GL_VERSION) << std::endl;

    {
        float positions[] = {
            -.5f, -.5f,
             .5f, -.5f,
             .5f,  .5f,
            -.5f,  .5f
        };

        unsigned int indicies[] = {
            0, 1, 2,
            2, 3, 0
        };

        unsigned int vao;
        GLCall(glGenVertexArrays(1, &vao));
        GLCall(glBindVertexArray(vao));

        VertexBuffer vb(positions, 4 * 2 * sizeof(float));

        GLCall(glEnableVertexAttribArray(0));
        GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));  // this links current array buffer to current VAO

        IndexBuffer ib(indicies, 6);

        ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
        unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
        GLCall(glUseProgram(shader));

        GLCall(int location = glGetUniformLocation(shader, "u_Color"));
        ASSERT(location != -1);
        GLCall(glUniform4f(location, .0f, 1.0f, .0f, 1.0f));


        GLCall(glBindVertexArray(0));
        GLCall(glUseProgram(0));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

        float blueChannel = 1.0f;
        float bcIncrement = .05f;

        /* Loop until the user closes the window  */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            GLCall(glClear(GL_COLOR_BUFFER_BIT));


            GLCall(glUseProgram(shader));
            GLCall(glUniform4f(location, 0.1f, 0.1f, blueChannel, 1.0f));

            GLCall(glBindVertexArray(vao));
            ib.Bind();

            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

            if (blueChannel > 1.0f)
                bcIncrement = -.05f;
            else if (blueChannel < 0.0f)
                bcIncrement = .05f;

            blueChannel += bcIncrement;


            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

        GLCall(glDeleteProgram(shader));
    }

    glfwTerminate();
    return 0;
}
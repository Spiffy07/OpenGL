#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>



struct ShaderProgramSource
{
    std::string VertexSource, FragmentSource;
};

static ShaderProgramSource PaseShader(const std::string& filepath)
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
    glShaderSource(id, 1, &src, NULL);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result != GL_TRUE)
    {
        //std::cout << "Shader Compiler Error! \n";

        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        char* message = static_cast<char*>(alloca(length * sizeof(char)));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!\n";
        std::cout << message << std::endl;
        glDeleteShader(id);

        return 0;
    }

    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    int linkResult, validateResult;
    glGetProgramiv(program, GL_LINK_STATUS, &linkResult);
    glGetProgramiv(program, GL_VALIDATE_STATUS, &validateResult);

    if (linkResult != GL_TRUE || validateResult != GL_TRUE)
    {
        int length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

        char* message = static_cast<char*>(alloca(length * sizeof(char)));
        glGetProgramInfoLog(program, length, &length, message);
        std::cout << "Error in creating Program! \n    " << message << std::endl;

    }

    glDetachShader(program, vs);
    glDeleteShader(vs);

    glDetachShader(program, fs);
    glDeleteShader(fs);

    return program;
}



int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
        std::cout << "Error!\n";

    std::cout << glGetString(GL_VERSION) << std::endl;

    float positions[6] = {
        -.5f, -.5f,
        0.0f,  .5f,
         .5f, -.5f
    };

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    ShaderProgramSource source = PaseShader("res/shaders/Basic.shader");
    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    glUseProgram(shader);


    /* Loop until the user closes the window  */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);


        glDrawArrays(GL_TRIANGLES, 0, 3);

        const char* error;
        glfwGetError(&error);
        if (error)
            std::cout << error << std::endl;

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    
    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}
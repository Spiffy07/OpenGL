#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

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
    window = glfwCreateWindow(1280, 960, "Hello World", NULL, NULL);
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
            400.0f,      300.0f,    0.0f,   0.0f,
            800.0f,      300.0f,    1.0f,   0.0f,
            800.0f,      600.0f,    1.0f,   1.0f,
            400.0f,      600.0f,    0.0f,   1.0f
        };

        unsigned int indicies[] = {
            0, 1, 2,
            2, 3, 0
        };

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        VertexArray va;
        VertexBuffer vb(positions, 4 * 4 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        IndexBuffer ib(indicies, 6);

        //glm::mat4 proj = glm::ortho(-.5f, .5f, -.375f, .375f, -1.0f, 1.0f);
        glm::mat4 proj =  glm::ortho(0.0f, 1280.0f, 0.0f, 960.0f, -1.0f, 1.0f);             // glm to set aspect ratio
        glm::mat4 view =  glm::translate(glm::mat4(1.0f), glm::vec3(-100.0f, 0.0f, 0.0f));  // move "camera" so negate
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(100.0f, 100.0f, 0.0f)); // move model

        glm::mat4 mvp = proj * view * model;


        Shader shader("res/shaders/Basic.shader");      // path to shader file
        shader.Bind();
        shader.SetUniform4f("u_Color", .0f, 1.0f, .0f, 1.0f);
        shader.SetUniformMat4f("u_MVP", mvp);                  // for fixing with aspect ratio

        Texture texture("res/textures/anyaT.png");      // input .png
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);

        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        float blueChannel = 1.0f;
        float bcIncrement = .05f;

        Renderer renderer;

        /* Loop until the user closes the window  */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();

            shader.Bind();
            shader.SetUniform4f("u_Color", 0.1f, 0.1f, blueChannel, 1.0f);

            renderer.Draw(va, ib, shader);

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
    }

    glfwTerminate();
    return 0;
}
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

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

#include "Tests/TestClearColor.h"

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
    window = glfwCreateWindow(960, 640, "Hello World", NULL, NULL);
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
        {
            //float positions[] = {
            //    -150.0f,      -150.0f,    0.0f,   0.0f,
            //     150.0f,      -150.0f,    1.0f,   0.0f,
            //     150.0f,       150.0f,    1.0f,   1.0f,
            //    -150.0f,       150.0f,    0.0f,   1.0f
            //};

            //unsigned int indicies[] = {
            //    0, 1, 2,
            //    2, 3, 0
            //};
        }

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        {
            //VertexArray va;
            //VertexBuffer vb(positions, 4 * 4 * sizeof(float));

            //VertexBufferLayout layout;
            //layout.Push<float>(2);
            //layout.Push<float>(2);
            //va.AddBuffer(vb, layout);

            //IndexBuffer ib(indicies, 6);

            ////glm::mat4 proj = glm::ortho(-.5f, .5f, -.375f, .375f, -1.0f, 1.0f);
            //glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 640.0f, -1.0f, 1.0f);             // glm to set aspect ratio
            //glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));  // move "camera" so negate


            //Shader shader("res/shaders/Basic.shader");      // path to shader file
            //shader.Bind();
            //shader.SetUniform4f("u_Color", .0f, 1.0f, .0f, 1.0f);

            //Texture texture("res/textures/anyaT.png");      // input .png
            //texture.Bind();
            //shader.SetUniform1i("u_Texture", 0);

            //va.Unbind();
            //vb.Unbind();
            //ib.Unbind();
            //shader.Unbind();

            //glm::vec3 translationA(0.0f, 0.0f, 0.0f);
            //glm::vec3 translationB(400.0f, 400.0f, 0.0f);
        }

        Renderer renderer;

        ImGui::CreateContext();
        ImGui_ImplGlfwGL3_Init(window, true);
        ImGui::StyleColorsDark();

        test::TestClearColor test;

        /* Loop until the user closes the window  */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();

            test.OnUpdate(0.0f);
            test.OnRender();

            ImGui_ImplGlfwGL3_NewFrame();
            test.OnImGuiRender();

            {
                //{
                //    glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA); // move model
                //    glm::mat4 mvp = proj * view * model;
                //    shader.Bind();
                //    shader.SetUniformMat4f("u_MVP", mvp);                  // for fixing with aspect ratio

                //    renderer.Draw(va, ib, shader);
                //}

                //{
                //    glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB); // move model
                //    glm::mat4 mvp = proj * view * model;
                //    shader.Bind();
                //    shader.SetUniformMat4f("u_MVP", mvp);                  // for fixing with aspect ratio

                //    renderer.Draw(va, ib, shader);
                //}
         

                //{
                //    ImGui::SliderFloat3("Translation A", &translationA.x, 0.0f, 960.0f);  
                //    ImGui::SliderFloat3("Translation B", &translationB.x, 0.0f, 960.0f);  

                //    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                //}
            }

            ImGui::Render();
            ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}
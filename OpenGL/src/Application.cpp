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
#include "Tests/TestTexture.h"
#include "Tests/TestBatching.h"
#include "Tests/TestDynamicBatch.h"

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

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);            // sets frame rate to monitor's refresh

    if (glewInit() != GLEW_OK)
        std::cout << "Error!\n";

    std::cout << glGetString(GL_VERSION) << std::endl;

    {
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        Renderer renderer;

        ImGui::CreateContext();
        ImGui_ImplGlfwGL3_Init(window, true);
        ImGui::StyleColorsDark();

        test::Test* currentTest = nullptr;
        test::TestMenu* testMenu = new test::TestMenu(currentTest);
        currentTest = testMenu;

        testMenu->RegisterTest<test::TestClearColor>("Clear Color");
        testMenu->RegisterTest<test::TestTexture>("Texture");
        testMenu->RegisterTest<test::TestBatching>("Batching");
        testMenu->RegisterTest<test::TestDynamicBatch>("Dynamic Batching");

        GLCall(glClearColor(0.2f, 0.2f, 0.2f, 1.0f)); // set screen background to gray

        /* Loop until the user closes the window    -------------------------------------------*/
        while (!glfwWindowShouldClose(window))
        {

            /* Render here */
            renderer.Clear();

            ImGui_ImplGlfwGL3_NewFrame();

            if (currentTest)
            {
                currentTest->OnUpdate(0.0f);
                currentTest->OnRender();
                ImGui::Begin("Test");
                if (currentTest != testMenu && ImGui::Button("<--"))
                {
                    delete currentTest;
                    currentTest = testMenu;
                    GLCall(glClearColor(0.2f, 0.2f, 0.2f, 1.0f)); // set screen background to gray
                }
                currentTest->OnImGuiRender();
                ImGui::End();
            }

            //if (ImGui::BeginMainMenuBar())
            //{
            //    if (ImGui::BeginMenu("File"))
            //    {
            //        if (ImGui::MenuItem("Test Clear Color", "Ctrl+O")) {  }
            //        if (ImGui::MenuItem("Test Textrue", "Ctrl+S")) {  }
            //        if (ImGui::MenuItem("Close", "Ctrl+W")) { /* Do stuff */ }
            //        ImGui::EndMenu();
            //    }
            //    ImGui::EndMainMenuBar();
            //}

            ImGui::Render();
            ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);

            glfwPollEvents();
        }
        delete currentTest;
        if (currentTest != testMenu)
            delete testMenu;
    }

    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}
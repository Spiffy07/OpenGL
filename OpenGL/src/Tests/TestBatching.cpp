#include "TestBatching.h"
#include <imgui/imgui.h>



test::TestBatching::TestBatching()
    : m_Positions{
             580.0f,    270.0f,
             780.0f,    270.0f,
             780.0f,    470.0f,
             580.0f,    470.0f,

             180.0f,    270.0f,
             380.0f,    270.0f,
             380.0f,    470.0f,
             180.0f,    470.0f,

    },
        m_Indicies{
            0, 1, 2, 2, 3, 0,
             4, 5, 6, 6, 7, 4 },
        m_Vb(m_Positions, 8 * 2 * sizeof(float)),
        m_Ib(m_Indicies, 12),
        m_TranslationA(0.0f, 0.0f, 0.0f), 
        m_TranslationB(400.0f, 400.0f, 0.0f),
        m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 640.0f, -1.0f, 1.0f)),
        m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))),
        m_Shader("res/shaders/Base.shader"),
        //m_Texture("res/textures/anyaT.png"),
        m_RendererPtr(nullptr)
{
    m_Layout.Push<float>(2);
    m_Va.AddBuffer(m_Vb, m_Layout);

    m_Shader.Bind();
    m_Shader.SetUniform4f("u_Color", .0f, 1.0f, .0f, 1.0f);

    //m_Texture.Bind();
    //m_Shader.SetUniform1i("u_Texture", 0);

    m_Va.Unbind();
    m_Vb.Unbind();
    m_Ib.Unbind();
    m_Shader.Unbind();
}

test::TestBatching::~TestBatching()
{
}

void test::TestBatching::OnUpdate(float deltaTime)
{
}

void test::TestBatching::OnRender()
{
    {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA); // move model
        glm::mat4 mvp = m_Proj * m_View * model;
        m_Shader.Bind();
        m_Shader.SetUniformMat4f("u_MVP", mvp);                  // for fixing with aspect ratio
        m_RendererPtr->Draw(m_Va, m_Ib, m_Shader);
    }
}

void test::TestBatching::OnImGuiRender()
{
    ImGui::SliderFloat3("Translation A", &m_TranslationA.x, 0.0f, 960.0f);  
    //ImGui::SliderFloat3("Translation B", &m_TranslationB.x, 0.0f, 960.0f);  

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

}

void test::TestBatching::SetRenderer(Renderer& renderer)
{
    m_RendererPtr = &renderer;
}

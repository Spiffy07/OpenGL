#include "TestBatching.h"
#include <imgui/imgui.h>



test::TestBatching::TestBatching()
    : m_Positions{
             580.0f,  71.0f,    0.1f, 1.0f, 0.1f, 1.0f,     0.0f, 0.0f,     1.0f,
             707.0f,  71.0f,    0.1f, 1.0f, 0.1f, 1.0f,     1.0f, 0.0f,     1.0f,
             707.0f, 569.0f,    0.1f, 1.0f, 0.1f, 1.0f,     1.0f, 1.0f,     1.0f,
             580.0f, 569.0f,    0.1f, 1.0f, 0.1f, 1.0f,     0.0f, 1.0f,     1.0f,
                                
             180.0f, 270.0f,    0.1f, 0.1f, 1.0f, 1.0f,     0.0f, 0.0f,     0.0f,
             380.0f, 270.0f,    0.1f, 0.1f, 1.0f, 1.0f,     1.0f, 0.0f,     0.0f,
             380.0f, 470.0f,    0.1f, 0.1f, 1.0f, 1.0f,     1.0f, 1.0f,     0.0f,
             180.0f, 470.0f,    0.1f, 0.1f, 1.0f, 1.0f,     0.0f, 1.0f,     0.0f
    },
        m_Indicies{
            0, 1, 2, 2, 3, 0,
             4, 5, 6, 6, 7, 4 },
        m_Vb(m_Positions, 8 * 9 * sizeof(float)),
        m_Ib(m_Indicies, 12),
        m_TranslationA(0.0f, 0.0f, 0.0f), 
        m_TranslationB(400.0f, 400.0f, 0.0f),
        m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 640.0f, -1.0f, 1.0f)),
        m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))),
        m_Shader("res/shaders/Base.shader"),
        m_TextureA("res/textures/anyaT.png"),
        m_TextureY("res/textures/yor.png"),
        m_RendererPtr(nullptr)
{
        // seperate push calls Per attribute
    m_Layout.Push<float>(2);    // position
    m_Layout.Push<float>(4);    // color
    m_Layout.Push<float>(2);    // texture coordinates
    m_Layout.Push<float>(1);    // texture index
    m_Va.AddBuffer(m_Vb, m_Layout);

    m_Shader.Bind();
    m_TextureA.Bind();
    m_TextureY.Bind(1);

    int samplers[2] = { 0, 1 };
    m_Shader.SetUniform1iv("u_Textures", 2, samplers);
    
    //m_Texture.Bind();
    //m_Shader.SetUniform1i("u_Texture", 0);
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

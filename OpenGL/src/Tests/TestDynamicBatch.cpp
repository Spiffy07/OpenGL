#include "TestDynamicBatch.h"
#include <imgui/imgui.h>
#include <array>

#include "Renderer.h"

struct Vec2
{
    float x, y;
};

struct Vec4
{
    float r, g, b, a;
};

struct Vertex
{
    Vec2 position;
    Vec4 color;
    Vec2 texCoord;
    float texIndex;
};

static std::array<Vertex, 4> MakeQuad(
    float x, float y, 
    float width = 100.0f, float height = 100.0f, 
    float texId = 0.0f)
{
    Vertex v0;
    v0.position = { x, y };
    v0.color = { 0.0f, 1.0f, 0.0f, 1.0f };
    v0.texCoord = { 0.0f, 0.0f };
    v0.texIndex = texId;
        
    Vertex v1;
    v1.position = { x + width, y };
    v1.color = { 0.0f, 0.0f, 1.0f, 1.0f };
    v1.texCoord = { 1.0f, 0.0f };
    v1.texIndex = texId;
       
    Vertex v2;
    v2.position = { x + width, y + height };
    v2.color = { 0.0f, 0.0f, 1.0f, 1.0f };
    v2.texCoord = { 1.0f, 1.0f };
    v2.texIndex = texId;
        
    Vertex v3;
    v3.position = { x, y + height };
    v3.color = { 0.0f, 1.0f, 0.0f, 1.0f };
    v3.texCoord = { 0.0f, 1.0f };
    v3.texIndex = texId;

    return { v0, v1, v2, v3 };
}

test::TestDynamicBatch::TestDynamicBatch()
    :   m_Shader("res/shaders/Base.shader"),
        m_TextureA("res/textures/anyaT.png"),
        m_TextureY("res/textures/yor.png"),
        m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 640.0f, -1.0f, 1.0f)),
        m_RendererPtr(nullptr),
        m_QuadCount(0)
{
        // create and bind vertex array object
    GLCall(glCreateVertexArrays(1, &m_Va));
    GLCall(glBindVertexArray(m_Va));

        // create and bind 'Dynamic' vertex buffer of 1000
    GLCall(glCreateBuffers(1, &m_Vb));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_Vb));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 1000, nullptr, GL_DYNAMIC_DRAW));

        // setup layout assigned to vertex array object
    GLCall(glEnableVertexArrayAttrib(m_Va, 0));
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, position)));

    GLCall(glEnableVertexArrayAttrib(m_Va, 1));
    GLCall(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, color)));

    GLCall(glEnableVertexArrayAttrib(m_Va, 2));
    GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, texCoord)));

    GLCall(glEnableVertexArrayAttrib(m_Va, 3));
    GLCall(glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, texIndex)));

        // create index buffer
    GLCall(glCreateBuffers(1, &m_Ib));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ib));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 250 * 3, nullptr, GL_DYNAMIC_DRAW));

    m_Shader.Bind();
    m_TextureA.Bind();
    m_TextureY.Bind(1);

    int samplers[2] = { 0, 1 };
    m_Shader.SetUniform1iv("u_Textures", 2, samplers);
}

test::TestDynamicBatch::~TestDynamicBatch()
{
}

void test::TestDynamicBatch::OnUpdate(float deltaTime)
{
    /*float verticies[] = {
        580.0f, 11.0f, 0.1f, 1.0f, 0.1f, 1.0f, 0.0f, 0.0f, 1.0f,
        707.0f, 11.0f, 0.1f, 1.0f, 0.1f, 1.0f, 1.0f, 0.0f, 1.0f,
        707.0f, 509.0f, 0.1f, 1.0f, 0.1f, 1.0f, 1.0f, 1.0f, 1.0f,
        580.0f, 509.0f, 0.1f, 1.0f, 0.1f, 1.0f, 0.0f, 1.0f, 1.0f,
    
        130.0f, 220.0f, 0.1f, 0.1f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        430.0f, 220.0f, 0.1f, 0.1f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        430.0f, 520.0f, 0.1f, 0.1f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        130.0f, 520.0f, 0.1f, 0.1f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f
    };*/

        // dynamic vertex buffer

    std::array<std::array<Vertex, 4>, 10> verticies;
    verticies[0] = MakeQuad(m_QuadPosition1[0], m_QuadPosition1[1], 300.0f, 300.0f, 0.0f);
    verticies[1] = MakeQuad(m_QuadPosition2[0], m_QuadPosition2[1], 127.0f, 498.0f, 1.0f);
    verticies[2] = MakeQuad(0.0f, 0.0f);
    verticies[3] = MakeQuad(110.0f, 0.0f);
    verticies[4] = MakeQuad(220.0f, 0.0f);

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_Vb));
    GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verticies), &verticies));

        // dynamic index buffer
    unsigned int indicies[5 * 6] = {
         0,  1,  2,  2,  3,  0,
         4,  5,  6,  6,  7,  4,
         8,  9, 10, 10, 11,  8,
        12, 13, 14, 14, 15, 12,
        16, 17, 18, 18, 19, 16
    };

    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ib));
    GLCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(indicies), indicies));

    m_Shader.Bind();

    m_Shader.SetUniformMat4f("u_MVP", m_Proj);                  // for fixing with aspect ratio
}

void test::TestDynamicBatch::OnRender()
{
    GLCall(glDrawElements(GL_TRIANGLES, m_QuadCount * 6, GL_UNSIGNED_INT, 0));
}  

void test::TestDynamicBatch::OnImGuiRender()
{
    // drag slider 
    ImGui::Begin("Controls");
    ImGui::DragFloat2("Quad1 Position", m_QuadPosition1, 1.0f);
    ImGui::DragFloat2("Quad2 Position", m_QuadPosition2, 1.0f);
    if (ImGui::Button("Add Quad") && m_QuadCount < 5)
        m_QuadCount++;
    if (ImGui::Button("Delete last Quad") && m_QuadCount > 0) 
        m_QuadCount--;

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
}

void test::TestDynamicBatch::SetRenderer(Renderer& renderer)
{
    m_RendererPtr = &renderer;
}

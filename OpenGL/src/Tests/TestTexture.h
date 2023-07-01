#pragma once

#include "Tests.h"

#include "Renderer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/glm.hpp"

namespace test {

	class TestTexture : public Test
	{
	public:
		TestTexture();
		~TestTexture();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

		void SetRenderer(Renderer& renderer);

		inline VertexArray& GetVA() { return m_Va; };
		inline IndexBuffer& GetIB() { return m_Ib; };
		inline Shader& GetShader() { return m_Shader; };

	private:
		float m_Positions[16];
		unsigned int m_Indicies[6];

		VertexBuffer m_Vb;
		VertexArray m_Va;
		VertexBufferLayout m_Layout;
		IndexBuffer m_Ib;
		Shader m_Shader;
		Texture m_Texture;      // input .png
		Renderer* m_RendererPtr;

		glm::mat4 m_Proj;
		glm::mat4 m_View;

		glm::vec3 m_TranslationA;
		glm::vec3 m_TranslationB;
	};
}
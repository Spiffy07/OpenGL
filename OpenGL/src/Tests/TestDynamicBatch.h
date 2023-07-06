#pragma once

#include "Tests.h"

#include "Renderer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/glm.hpp"

namespace test {

	class TestDynamicBatch : public Test
	{
	public:
		TestDynamicBatch();
		~TestDynamicBatch();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

		void SetRenderer(Renderer& renderer);

	private:
		unsigned int m_Indicies[12];
		unsigned int m_Vb;
		unsigned int m_Ib;
		unsigned int m_Va;

		
		VertexBufferLayout m_Layout;
		Shader m_Shader;
		Texture m_TextureA;
		Texture m_TextureY;
		Renderer* m_RendererPtr;

		glm::mat4 m_Proj;

		float m_QuadPosition1[2] = { 130.0f, 220.0f };
		float m_QuadPosition2[2] = { 580.0f,  11.0f };
	};
}
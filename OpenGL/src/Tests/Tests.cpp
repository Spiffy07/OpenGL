#include "Tests.h"
#include "imgui/imgui.h"

namespace test
{
	TestMenu::TestMenu(Test*& CurrentTestPtr)
		:m_CurrentTest(CurrentTestPtr)
	{

	}

	TestMenu::~TestMenu()
	{
	}

	void TestMenu::OnImGuiRender()
	{
		for (auto& test : m_Tests)
		{
			if (ImGui::Button(test.first.c_str()))
				m_CurrentTest = test.second();
		}
	}

}
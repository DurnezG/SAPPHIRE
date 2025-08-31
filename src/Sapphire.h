#pragma once
#include <Emerald.h>

#include <Core/GameObject.h>

namespace SAPPHIRE
{
	class Sapphire
	{
	public:
		Sapphire();
		~Sapphire();

		void Run();

		void ImGuiSetup();
		void ImGuiStartFrame();

		void ImGuiUI();

		void ShowHierarchy();
		void ShowInspector();
		void ShowConsole();

		void ImGuiEndFrame();
		void ImGuiDestory();

	private:
		static void SetupImGuiStyle();

		EMERALD::Emerald m_pEngine;

		bool m_IsRunning;

		EMERALD::GameObject* m_SelectedObject = nullptr;
	};
}

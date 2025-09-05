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
	private:

		void ImGuiSetup();
		void ImGuiStartFrame();

		void CreateImGuiDescriptors();
		void DestroyImguiDescriptors();

		void ImGuiUI();

		void ShowHierarchy();
		void ShowInspector();
		void ShowConsole();

		void ImGuiEndFrame();
		void ImGuiDestory();

		static void SetupImGuiStyle();
		void SetupImGuiStyleUNORM();

		EMERALD::Emerald m_pEngine;

		bool m_IsRunning;

		VkDescriptorPool m_DescriptorPool{ VK_NULL_HANDLE };

		EMERALD::GameObject* m_SelectedObject = nullptr;
	};
}

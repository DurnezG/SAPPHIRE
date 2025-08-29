#pragma once
#include <Emerald.h>

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
		void ImGuiEndFrame();
		void ImGuiDestory();

	private:
		static void SetupImGuiStyle();

		EMERALD::Emerald m_pEngine;

		bool m_IsRunning;
	};
}

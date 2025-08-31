#include "Sapphire.h"

#include <chrono>
#include <functional>
#include <string>
#include <SDL3/SDL.h>

#include "imgui.h"
#include "implot.h"

#include "misc/cpp/imgui_stdlib.h"
#include "imgui_internal.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"

#include "Emerald.h"
#include <Core/SceneManager.h>
#include <Core/Scene.h>
#include <Core/Console.h>

SAPPHIRE::Sapphire::Sapphire()
	: m_pEngine("SAPPHIRE", 1920, 1080)
{
	ImGuiSetup();


	// DEBUG ONLY
	{
		auto& scene = EMERALD::SceneManager::GetInstance().CreateScene("Test level");

		auto go = std::make_shared<EMERALD::GameObject>("New GameObject 1");
		scene.Add(go);

		go = std::make_shared<EMERALD::GameObject>("New GameObject 2");
		scene.Add(go);
		auto go2 = std::make_shared<EMERALD::GameObject>("New GameObject 3");
		go2->SetParent(go.get(), false);
		scene.Add(go2);
		go2 = std::make_shared<EMERALD::GameObject>("New GameObject 4");
		go2->SetParent(go.get(), false);
		scene.Add(go2);

		EMERALD::Console::GetInstance().Log("Testing the log message.");
		EMERALD::Console::GetInstance().Log("Testing the log message.");
		EMERALD::Console::GetInstance().Log("Testing the log message.");
		EMERALD::Console::GetInstance().Log("Testing the log message.");
		EMERALD::Console::GetInstance().Log("Testing the log message.");
		EMERALD::Console::GetInstance().Log("Testing the log message.");
		EMERALD::Console::GetInstance().Log("Testing the log message.");
		EMERALD::Console::GetInstance().Log("Testing the log message.");
		EMERALD::Console::GetInstance().Log("Testing the log message.");
		EMERALD::Console::GetInstance().Log("Testing the log message.");
		EMERALD::Console::GetInstance().Log("Testing the log message.");
		EMERALD::Console::GetInstance().Log("Testing the log message.");
		EMERALD::Console::GetInstance().Log("Testing the log message.");
		EMERALD::Console::GetInstance().LogWarning("Testing the log warning message.");
		EMERALD::Console::GetInstance().LogError("Testing the log error message.");
	}

}

SAPPHIRE::Sapphire::~Sapphire()
{
	ImGuiDestory();
}

void SAPPHIRE::Sapphire::Run()
{
	m_pEngine.Start();
	while (m_IsRunning)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			// HAndle imgui events
			ImGui_ImplSDL3_ProcessEvent(&event);
			if (event.type == SDL_EVENT_QUIT) m_IsRunning = false;
		}
		const auto currentTime = std::chrono::high_resolution_clock::now();
		m_pEngine.StartFrame(currentTime);
		m_pEngine.Update();

		ImGuiStartFrame();
		ImGuiUI();
		ImGuiEndFrame();

		m_pEngine.Render();
		m_pEngine.EndFrame(currentTime);
	}
}

void SAPPHIRE::Sapphire::ImGuiSetup()
{
	SDL_Window* window = m_pEngine.GetSDLWindow();
	SDL_Renderer* renderer = m_pEngine.GetSDLRenderer();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImPlot::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	SetupImGuiStyle();

	ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer3_Init(renderer);
}

void SAPPHIRE::Sapphire::ImGuiStartFrame()
{
	ImGui_ImplSDLRenderer3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();
}

void SAPPHIRE::Sapphire::ImGuiUI()
{
	// ---- 1. Host Window ----
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus |
		ImGuiWindowFlags_NoNavFocus;

	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	ImGui::Begin("DockSpaceHost", nullptr, window_flags);
	ImGui::PopStyleVar(3);

	ImGuiID dockspace_id = ImGui::GetID("MainDockSpace");
	ImGui::DockSpace(dockspace_id, ImVec2(0, 0), ImGuiDockNodeFlags_PassthruCentralNode);

	// ---- 2. Build layout once ----
	static bool first_time = true;
	if (first_time) 
	{
		first_time = false;

		ImGui::DockBuilderRemoveNode(dockspace_id);  // clear existing
		ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
		ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

		// Split: left, right, bottom
		ImGuiID dock_main_id = dockspace_id;
		ImGuiID dock_id_left = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.20f, nullptr, &dock_main_id);
		ImGuiID dock_id_right = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.25f, nullptr, &dock_main_id);
		ImGuiID dock_id_bottom = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.25f, nullptr, &dock_main_id);
		// dock_main_id is now the center (Scene)

		// Assign windows
		ImGui::DockBuilderDockWindow("Hierarchy", dock_id_left);
		ImGui::DockBuilderDockWindow("Inspector", dock_id_right);
		ImGui::DockBuilderDockWindow("Console", dock_id_bottom);
		ImGui::DockBuilderDockWindow("Assets", dock_id_bottom); // tabbed with Console
		ImGui::DockBuilderDockWindow("Scene", dock_main_id);

		ImGui::DockBuilderFinish(dockspace_id);
	}

	ImGui::End(); // End host window

	// ---- 3. Actual Windows ----

	ImGui::Begin("Hierarchy");
	ShowHierarchy();
	ImGui::End();

	ImGui::Begin("Inspector");
	ShowInspector();
	ImGui::End();

	ImGui::Begin("Assets");
	ImGui::Text("Project Assets");
	ImGui::Separator();
	ImGui::BulletText("Materials");
	ImGui::BulletText("Meshes");
	ImGui::BulletText("Textures");
	ImGui::BulletText("Scripts");
	ImGui::End();

	ImGui::Begin("Console");
	ShowConsole();
	ImGui::End();

	ImGui::Begin("Scene");
	ImGui::Text("Scene View (renderer)");
	ImGui::Separator();

	// Example ImPlot content
	static float xs[100], ys[100];
	static bool init = false;
	if (!init) {
		for (int i = 0; i < 100; i++) {
			xs[i] = i * 0.1f;
			ys[i] = sinf(xs[i]);
		}
		init = true;
	}
	if (ImPlot::BeginPlot("ScenePlot")) {
		ImPlot::PlotLine("sin(x)", xs, ys, 100);
		ImPlot::EndPlot();
	}

	ImGui::End();
}

void SAPPHIRE::Sapphire::ShowHierarchy()
{
	auto& sceneManager = EMERALD::SceneManager::GetInstance();
	auto scenes = sceneManager.GetScenes();

	ImGui::Begin("Hierarchy");

	// Recursive function to draw nodes
	std::function<void(EMERALD::GameObject*)> DrawNode = [&](EMERALD::GameObject* obj)
	{
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
		if (obj == m_SelectedObject)
			flags |= ImGuiTreeNodeFlags_Selected;

		bool opened = ImGui::TreeNodeEx(obj->GetName().c_str(), flags);
		if (ImGui::IsItemClicked()) 
		{
			m_SelectedObject = obj; // select object
		}

		if (opened) 
		{
			for (auto* child : obj->GetChildren())
				DrawNode(child);
			ImGui::TreePop();
		}
	};

	for (auto scene : scenes)
	{
		ImGui::Text(scene->GetName().c_str(), ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth);
		auto gos = scene->GetGameObjects();

		for (auto go : gos)
		{
			if (go->GetParent() == nullptr)
				DrawNode(go);
		}
	}

	ImGui::End();
}

void SAPPHIRE::Sapphire::ShowInspector()
{
	ImGui::Begin("Inspector");

	if (m_SelectedObject) 
	{
		std::string name = m_SelectedObject->GetName();
		ImGui::Text("Selected: %s", name.c_str());
		ImGui::Separator();

		if (ImGui::InputText("Name", &name))
{
			m_SelectedObject->SetName(name);
		}

		// TODO: show components
	}
	else 
	{
		ImGui::Text("No object selected");
	}

	ImGui::End();
}

void SAPPHIRE::Sapphire::ShowConsole()
{
	using namespace EMERALD;
	auto& console = Console::GetInstance();

	for (const auto& msg : console.GetMessages())
	{
		ImVec4 color;
		switch (msg.level)
		{
		case LogLevel::Info:    color = ImVec4(1, 1, 1, 1); break; // white
		case LogLevel::Warning: color = ImVec4(1, 1, 0, 1); break; // yellow
		case LogLevel::Error:   color = ImVec4(1, 0, 0, 1); break; // red
		}

		ImGui::TextColored(color, "%s", msg.text.c_str());
	}
}

void SAPPHIRE::Sapphire::ImGuiEndFrame()
{
	//SDL_Window* window = m_pEngine.GetSDLWindow();
	SDL_Renderer* renderer = m_pEngine.GetSDLRenderer();

	ImGui::Render();

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
	SDL_RenderPresent(renderer);
}

void SAPPHIRE::Sapphire::ImGuiDestory()
{
	ImGui_ImplSDLRenderer3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImPlot::DestroyContext();
	ImGui::DestroyContext();
}

void SAPPHIRE::Sapphire::SetupImGuiStyle()
{
	// Comfortable Dark Cyan style by SouthCraftX from ImThemes
	ImGuiStyle& style = ImGui::GetStyle();

	style.Alpha = 1.0f;
	style.DisabledAlpha = 1.0f;
	style.WindowPadding = ImVec2(20.0f, 20.0f);
	style.WindowRounding = 11.5f;
	style.WindowBorderSize = 0.0f;
	style.WindowMinSize = ImVec2(20.0f, 20.0f);
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style.WindowMenuButtonPosition = ImGuiDir_None;
	style.ChildRounding = 20.0f;
	style.ChildBorderSize = 1.0f;
	style.PopupRounding = 17.39999961853027f;
	style.PopupBorderSize = 1.0f;
	style.FramePadding = ImVec2(20.0f, 3.400000095367432f);
	style.FrameRounding = 11.89999961853027f;
	style.FrameBorderSize = 0.0f;
	style.ItemSpacing = ImVec2(8.899999618530273f, 13.39999961853027f);
	style.ItemInnerSpacing = ImVec2(7.099999904632568f, 1.799999952316284f);
	style.CellPadding = ImVec2(12.10000038146973f, 9.199999809265137f);
	style.IndentSpacing = 0.0f;
	style.ColumnsMinSpacing = 8.699999809265137f;
	style.ScrollbarSize = 11.60000038146973f;
	style.ScrollbarRounding = 15.89999961853027f;
	style.GrabMinSize = 3.700000047683716f;
	style.GrabRounding = 20.0f;
	style.TabRounding = 9.800000190734863f;
	style.TabBorderSize = 0.0f;
	style.TabMinWidthForCloseButton = 0.0f;
	style.ColorButtonPosition = ImGuiDir_Right;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

	style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.2745098173618317f, 0.3176470696926117f, 0.4509803950786591f, 1.0f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.09411764889955521f, 0.1019607856869698f, 0.1176470592617989f, 1.0f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.1137254908680916f, 0.125490203499794f, 0.1529411822557449f, 1.0f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.09803921729326248f, 0.105882354080677f, 0.1215686276555061f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.0313725508749485f, 0.9490196108818054f, 0.843137264251709f, 1.0f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.0313725508749485f, 0.9490196108818054f, 0.843137264251709f, 1.0f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.6000000238418579f, 0.9647058844566345f, 0.0313725508749485f, 1.0f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.1803921610116959f, 0.1882352977991104f, 0.196078434586525f, 1.0f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.1529411822557449f, 0.1529411822557449f, 0.1529411822557449f, 1.0f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.1411764770746231f, 0.1647058874368668f, 0.2078431397676468f, 1.0f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.105882354080677f, 0.105882354080677f, 0.105882354080677f, 1.0f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.1294117718935013f, 0.1490196138620377f, 0.1921568661928177f, 1.0f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.1568627506494522f, 0.1843137294054031f, 0.250980406999588f, 1.0f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.1568627506494522f, 0.1843137294054031f, 0.250980406999588f, 1.0f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1450980454683304f, 1.0f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.0313725508749485f, 0.9490196108818054f, 0.843137264251709f, 1.0f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.125490203499794f, 0.2745098173618317f, 0.572549045085907f, 1.0f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.5215686559677124f, 0.6000000238418579f, 0.7019608020782471f, 1.0f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.03921568766236305f, 0.9803921580314636f, 0.9803921580314636f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.0313725508749485f, 0.9490196108818054f, 0.843137264251709f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.1568627506494522f, 0.1843137294054031f, 0.250980406999588f, 1.0f);
	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
	style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
	style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.09803921729326248f, 0.105882354080677f, 0.1215686276555061f, 1.0f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.9372549057006836f, 0.9372549057006836f, 0.9372549057006836f, 1.0f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.2666666805744171f, 0.2901960909366608f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.196078434586525f, 0.1764705926179886f, 0.5450980663299561f, 0.501960813999176f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.196078434586525f, 0.1764705926179886f, 0.5450980663299561f, 0.501960813999176f);
}

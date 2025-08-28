#include "Sapphire.h"

#include <string>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h> // Needed on Windows for SDL3 entry point
#include "imgui.h"

#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"

int main(int /*argc*/, char** /*argv*/)
{
    bool isRunning = 1;
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("Window", 720, 480, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, 0);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);
    while (isRunning)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT) isRunning = 0;
        }
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
        //ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
        ImGui::Begin("test");
        ImGui::Text("Hello World!");
        ImGui::End();
        ImGui::Render();
        SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
        SDL_RenderPresent(renderer);
    }
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
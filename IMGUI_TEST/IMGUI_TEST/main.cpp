#include "define.h"

#if BUILD_VERSION == 1

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <iostream>

#pragma comment(lib, "opengl32.lib")

// SDL2는 main 함수 시그니처가 특별해야 함
#undef main  // SDL2 main 매크로 해제

int main(int argc, char* argv[]) {
    // SDL initialization
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "SDL Init Failed: " << SDL_GetError() << std::endl;
        return -1;
    }

    std::cout << "SDL2 Init Success!" << std::endl;

    // OpenGL attributes
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    // Create window
    SDL_Window* window = SDL_CreateWindow("Game Server Debugger",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 700,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if (!window) {
        std::cout << "Window Creation Failed: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_SetSwapInterval(1);

    // ImGui initialization
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init("#version 130");

    std::cout << "ImGui Init Success!" << std::endl;

    // Game server data
    int player_count = 42;
    int monster_count = 128;
    float server_tps = 60.0f;
    bool server_running = true;

    // Main loop
    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        // Server status window
        ImGui::Begin("Game Server Dashboard");

        ImGui::Text("Server Status: %s", server_running ? "ONLINE" : "OFFLINE");
        ImGui::Separator();

        ImGui::Text("Statistics:");
        ImGui::Text("  Players: %d", player_count);
        ImGui::Text("  Monsters: %d", monster_count);
        ImGui::Text("  TPS: %.1f", server_tps);

        ImGui::Separator();

        if (ImGui::Button("Restart Server")) {
            std::cout << "Server Restart!" << std::endl;
        }
        ImGui::SameLine();
        if (ImGui::Button("Save State")) {
            std::cout << "State Saved!" << std::endl;
        }

        ImGui::Separator();

        ImGui::SliderInt("Max Players", &player_count, 0, 1000);
        ImGui::SliderFloat("Target TPS", &server_tps, 30.0f, 120.0f);
        ImGui::Checkbox("Server Running", &server_running);

        ImGui::End();

        // Player list window
        ImGui::Begin("Player List");

        for (int i = 1; i <= 5; i++) {
            ImGui::Text("Player %d", i);
            ImGui::SameLine();
            ImGui::PushID(i);
            if (ImGui::SmallButton("Kick")) {
                std::cout << "Player " << i << " kicked!" << std::endl;
            }
            ImGui::PopID();
        }

        ImGui::End();

        // Rendering
        ImGui::Render();

        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

#endif
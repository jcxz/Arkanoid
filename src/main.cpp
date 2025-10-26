#include "core/logger.h"
#include "game.h"
#include "imgui.h"
#include <SDL3/SDL_main.h>



int main(int argc, char* argv[])
{
	// initialize global stuff
	if (!ark::InitLogger("arkanoid.log", "arkanoid"))
		return 1;

	if (!SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO))
	{
		ARK_ERROR("SDL_Init error: {}", SDL_GetError());
		ark::CloseLogger();
		return 1;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	//ImGui::StyleColorsDark();

	Game game;
	if (!game.Init())
	{
		ARK_ERROR("Failed to initialize the game");
		ImGui::DestroyContext();
		SDL_Quit();
		ark::CloseLogger();
		return 1;
	}

	// main loop
	game.Run();

	// terminate global stuff
	game.Terminate();

	ImGui::DestroyContext();

	SDL_Quit();

	ark::CloseLogger();

	return 0;
}

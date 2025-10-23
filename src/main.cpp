#include <spdlog/spdlog.h>
#include "game.h"



int main(int argc, char* argv[])
{
	Game g;
	if (!g.init()) {
		spdlog::error("Failed to initialize game");
		return 1;
	}
	g.run();
	return 0;
}

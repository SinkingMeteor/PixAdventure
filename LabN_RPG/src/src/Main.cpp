#include <iostream>
#include "Window/Window.h"
#include "entt/locator/locator.hpp"
#include "Game.h"
#include "yaml-cpp/yaml.h"


int main() 
{
	vg::Game game{};
	game.Start();
	game.Update();
	return 0;
}
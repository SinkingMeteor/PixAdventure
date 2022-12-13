#include "Game.h"

namespace vg 
{
	Game::Game() 
	{

	}

	void Game::Update()
	{
		Tick();
		Render();
	}
	void Game::Tick()
	{
	}
	void Game::Render()
	{
		Window& window = Locator::Window_t::value();
		window.GetWindow().clear(sf::Color::Black);

		window.GetWindow().display();
	}
}


#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "Vector2.h"
#include "Player.cpp"

int cells[32][24] = { 0 };

const Vector2 GRID_SIZE(sizeof(cells) / sizeof(cells[0]), sizeof(cells[0]) / sizeof(cells[0][0]));
const int CELL_SIZE = 32;
const Vector2 WINDOW_SIZE(GRID_SIZE.x * CELL_SIZE, GRID_SIZE.y * CELL_SIZE);

const float START_SPEED = 5.0f;

bool isGameOver = false;
int loser = 0;

int playerOneScore = 0;
int playerTwoScore = 0;

const sf::Font font = []() 
{
	sf::Font font;
	font.loadFromFile("poxel-font.ttf");
	return font;
}();

int main() 
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE.x, WINDOW_SIZE.y), "TRON");
    sf::Clock clock;

    Player greenPlayer(1, 28, 19, Vector2(0, -1), sf::Color(153, 255, 125), GRID_SIZE);
	Player orangePlayer(2, 5, 5, Vector2(0, 1), sf::Color(255, 196, 69), GRID_SIZE);

    while (window.isOpen()) 
    {
        float deltaTime = clock.restart().asSeconds();

        sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Escape)
					window.close();

				if (isGameOver)
					continue;

				Vector2 greenInput = Vector2(0, 0);
				Vector2 orangeInput = Vector2(0, 0);

				if (event.key.code == sf::Keyboard::Left)
					greenInput.x = -1;
				if (event.key.code == sf::Keyboard::Right)
					greenInput.x = 1;
				if (event.key.code == sf::Keyboard::Up)
					greenInput.y = -1;
				if (event.key.code == sf::Keyboard::Down)
					greenInput.y = 1;

				if (event.key.code == sf::Keyboard::A)
					orangeInput.x = -1;
				if (event.key.code == sf::Keyboard::D)
					orangeInput.x = 1;
				if (event.key.code == sf::Keyboard::W)
					orangeInput.y = -1;
				if (event.key.code == sf::Keyboard::S)
					orangeInput.y = 1;

				if (greenInput.x != 0 || greenInput.y != 0)
					greenPlayer.UpdateInput(greenInput);
				if (orangeInput.x != 0 || orangeInput.y != 0)
					orangePlayer.UpdateInput(orangeInput);
			}
		}

        window.clear(sf::Color(21, 21, 28));
        
		sf::RectangleShape background(sf::Vector2f(GRID_SIZE.x * CELL_SIZE, GRID_SIZE.y * CELL_SIZE));
		background.setFillColor(sf::Color(30, 30, 41));
		window.draw(background);

        int cellGap = CELL_SIZE / 7;

        if (!isGameOver)
        {
			if (greenPlayer.Update(deltaTime))
				cells[(int)greenPlayer.GetPreviousPosition().x][(int)greenPlayer.GetPreviousPosition().y] = greenPlayer.GetPlayerNumber();

			if (cells[(int)greenPlayer.GetPosition().x][(int)greenPlayer.GetPosition().y] != 0)
			{
				isGameOver = true;
				loser = greenPlayer.GetPlayerNumber();
			}

			if (orangePlayer.Update(deltaTime))
				cells[(int)orangePlayer.GetPreviousPosition().x][(int)orangePlayer.GetPreviousPosition().y] = orangePlayer.GetPlayerNumber();

			if (cells[(int)orangePlayer.GetPosition().x][(int)orangePlayer.GetPosition().y] != 0)
			{
				isGameOver = true;
				loser = orangePlayer.GetPlayerNumber();
			}
		}
		else
		{
			std::string winner = loser == 1 ? "Orange" : "Green";
			std::cout << winner << " won!" << std::endl;

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
			{
				isGameOver = false;
				loser = 0;

				for (int x = 0; x < GRID_SIZE.x; ++x)
				{
					for (int y = 0; y < GRID_SIZE.y; ++y)
					{
						cells[x][y] = 0;
					}
				}

				greenPlayer = Player(1, 28, 19, Vector2(0, -1), sf::Color(153, 255, 125), GRID_SIZE);
				orangePlayer = Player(2, 5, 5, Vector2(0, 1), sf::Color(255, 196, 69), GRID_SIZE);	
			}
        }

		for (int x = 0; x < GRID_SIZE.x; ++x)
		{
			for (int y = 0; y < GRID_SIZE.y; ++y)
			{
				if (cells[x][y] != 0)
				{
					sf::RectangleShape cell(sf::Vector2f(CELL_SIZE, CELL_SIZE - cellGap));
					cell.setPosition(x * CELL_SIZE, y * CELL_SIZE + cellGap / 2);
					sf::Color color = cells[x][y] == 1 ? sf::Color(62, 163, 34) : sf::Color(219, 150, 0);
					cell.setFillColor(color);
					window.draw(cell);
				}
			}
		}

        greenPlayer.Draw(window);
		orangePlayer.Draw(window);

        window.display();
    }

    return 0;
}
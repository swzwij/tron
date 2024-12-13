#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "Vector2.h"
#include "Player.cpp"

int cells[32][24] = { 0 };
const int CELL_SIZE = 32;

const Vector2 GRID_SIZE(sizeof(cells) / sizeof(cells[0]), sizeof(cells[0]) / sizeof(cells[0][0]));
const Vector2 WINDOW_SIZE(GRID_SIZE.x * CELL_SIZE, GRID_SIZE.y * CELL_SIZE);
const Vector2 OFFSET(CELL_SIZE, CELL_SIZE);

bool isGameOver = false;
int loser = 0;

int greenScore = 0;
int orangeScore = 0;

const sf::Font font = []() 
{
	sf::Font font;
	font.loadFromFile("poxel-font.ttf");
	return font;
}();

int main() 
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE.x + OFFSET.x * 2, WINDOW_SIZE.y + OFFSET.y * 2), "TRON");
    sf::Clock clock;

    Player greenPlayer(1, 28, 19, Vector2(0, -1), sf::Color(153, 255, 125), GRID_SIZE, CELL_SIZE, OFFSET);
	Player orangePlayer(2, 5, 5, Vector2(0, 1), sf::Color(255, 196, 69), GRID_SIZE, CELL_SIZE, OFFSET);

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
		background.setPosition(OFFSET.x, OFFSET.y);
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
				orangeScore++;
			}

			if (orangePlayer.Update(deltaTime))
				cells[(int)orangePlayer.GetPreviousPosition().x][(int)orangePlayer.GetPreviousPosition().y] = orangePlayer.GetPlayerNumber();

			if (cells[(int)orangePlayer.GetPosition().x][(int)orangePlayer.GetPosition().y] != 0)
			{
				isGameOver = true;
				loser = orangePlayer.GetPlayerNumber();
				greenScore++;
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
					for (int y = 0; y < GRID_SIZE.y; ++y)
						cells[x][y] = 0;

				greenPlayer = Player(1, 28, 19, Vector2(0, -1), sf::Color(153, 255, 125), GRID_SIZE, CELL_SIZE, OFFSET);
				orangePlayer = Player(2, 5, 5, Vector2(0, 1), sf::Color(255, 196, 69), GRID_SIZE, CELL_SIZE, OFFSET);
			}
        }

		for (int x = 0; x < GRID_SIZE.x; ++x)
		{
			for (int y = 0; y < GRID_SIZE.y; ++y)
			{
				if (cells[x][y] != 0)
				{
					sf::RectangleShape cell(sf::Vector2f(CELL_SIZE, CELL_SIZE - cellGap));
					cell.setPosition(x * CELL_SIZE + OFFSET.x, y * CELL_SIZE + cellGap / 2.0f + OFFSET.y);
					sf::Color color = cells[x][y] == 1 ? sf::Color(62, 163, 34) : sf::Color(219, 150, 0);
					cell.setFillColor(color);
					window.draw(cell);
				}
			}
		}

        greenPlayer.Draw(window);
		orangePlayer.Draw(window);

		sf::Text greenScoreText;
		greenScoreText.setFont(font);
		greenScoreText.setString(std::to_string(greenScore));
		greenScoreText.setCharacterSize(24);
		greenScoreText.setFillColor(sf::Color(153, 255, 125));
		greenScoreText.setPosition(OFFSET.x, WINDOW_SIZE.y + OFFSET.y + 10);
		window.draw(greenScoreText);

		sf::Text orangeScoreText;
		orangeScoreText.setFont(font);
		orangeScoreText.setString(std::to_string(orangeScore));
		orangeScoreText.setCharacterSize(24);
		orangeScoreText.setFillColor(sf::Color(255, 196, 69));
		orangeScoreText.setPosition(WINDOW_SIZE.x - orangeScoreText.getLocalBounds().width - OFFSET.x, WINDOW_SIZE.y + OFFSET.y + 10);
		window.draw(orangeScoreText);


        window.display();
    }

    return 0;
}
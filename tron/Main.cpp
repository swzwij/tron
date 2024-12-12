#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "Vector2.h"

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

class Player
{
private:
    sf::RectangleShape shape;
    Vector2 position;
    int direction;
    float speed = START_SPEED;
    float moveAccumulator = 0.0f;
	int playerNumber;
    bool init = false;

public:
    Player(int number, int x, int y, sf::Color color, int startDirection = 3) : position(x,y)
    {
		playerNumber = number;
		direction = startDirection;
        shape.setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));
        shape.setFillColor(color);
        UpdatePosition(0);
    }

    void moveLeft()
    {
        if (position.x > 0)
        {
            position.x--;
            SetPosition();
        }
    }

    void moveRight()
    {
        if (position.x < GRID_SIZE.x - 1)
        {
            position.x++;
            SetPosition();
        }
    }

    void moveUp()
    {
        if (position.y > 0)
        {
            position.y--;
            SetPosition();
        }
    }

    void moveDown()
    {
        if (position.y < GRID_SIZE.y - 1)
        {
            position.y++;
            SetPosition();
        }
    }

    void ChangeDirection(int newDirection)
    {
		if (direction == 0 && newDirection == 1)
			return;

		if (direction == 1 && newDirection == 0)
			return;

		if (direction == 2 && newDirection == 3)
			return;

		if (direction == 3 && newDirection == 2)
			return;

        direction = newDirection;
    }

    void UpdatePosition(float deltaTime)
    {
        moveAccumulator += deltaTime * speed;

        while (moveAccumulator >= 1.0f)
        {
            if (init)
                cells[(int)getPosition().x / CELL_SIZE][(int)getPosition().y / CELL_SIZE] = playerNumber;
			else
				init = true;

            speed += speed < 6 ? 0.01f : speed < 12 ? 0.1f : 0.01f;

            switch (direction)
            {
            case 0:
                moveLeft();
                break;
            case 1:
                moveRight();
                break;
            case 2:
                moveUp();
                break;
            case 3:
                moveDown();
                break;
            default:
                break;
            }
            moveAccumulator -= 1.0f;

			if (cells[(int)getPosition().x / CELL_SIZE][(int)getPosition().y / CELL_SIZE] != 0)
			{
				isGameOver = true;
				loser = playerNumber;
			}
        }
    }

    void SetPosition()
    {
        shape.setPosition(position.x * CELL_SIZE, position.y * CELL_SIZE);
    }

	void SetPosition(Vector2 newPosition)
	{
		position = newPosition;
		SetPosition();
	}

	void SetDirection(int newDirection)
	{
		direction = newDirection;
	}   

	sf::Vector2f getPosition()
	{
		return shape.getPosition();
	}

    sf::RectangleShape& getShape()
    {
        return shape;
    }

    void setSpeed(float newSpeed)
    {
        speed = newSpeed;
    }
};

int main() 
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE.x, WINDOW_SIZE.y), "TRON");
    sf::Clock clock;

    Player player(1, 28, 19, sf::Color(153, 255, 125), 2);
	Player player2(2, 5, 5, sf::Color(255, 196, 69));

    while (window.isOpen()) 
    {
        float deltaTime = clock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) 
        {
            if (event.type == sf::Event::Closed)
                window.close();

            // Handle key presses for movement
            if (event.type == sf::Event::KeyPressed) 
            {
				if (event.key.code == sf::Keyboard::Escape)
					window.close();

                if (isGameOver)
                    continue;

                switch (event.key.code) 
                {
                    case sf::Keyboard::Left:
                        player.ChangeDirection(0);
                        break;
                    case sf::Keyboard::Right:
                        player.ChangeDirection(1);
                        break;
                    case sf::Keyboard::Up:
                        player.ChangeDirection(2);
                        break;
                    case sf::Keyboard::Down:
                        player.ChangeDirection(3);
                        break;

				    case sf::Keyboard::A:
					    player2.ChangeDirection(0);
					    break;
				    case sf::Keyboard::D:
					    player2.ChangeDirection(1);
					    break;
				    case sf::Keyboard::W:
					    player2.ChangeDirection(2);
					    break;
				    case sf::Keyboard::S:
					    player2.ChangeDirection(3);
					    break;
                    default:
                        break;
                }
            }
        }

        if (isGameOver)
        {
            for (int x = 0; x < GRID_SIZE.x; ++x) 
            {
                for (int y = 0; y < GRID_SIZE.y; ++y) 
                {
                    cells[x][y] = 0;
                }
            }

			player.SetPosition(Vector2(28, 19));
			player2.SetPosition(Vector2(5, 5));

			player.SetDirection(2);
			player2.SetDirection(3);

            player.setSpeed(START_SPEED);
            player2.setSpeed(START_SPEED);

            isGameOver = false;

			if (loser == 1)
				playerTwoScore++;
			else
				playerOneScore++;

			std::cout << "Green: " << playerOneScore << "\nOrange: " << playerTwoScore << "\n" << std::endl;
        }

        window.clear(sf::Color(21, 21, 28));
        
		sf::RectangleShape background(sf::Vector2f(GRID_SIZE.x * CELL_SIZE, GRID_SIZE.y * CELL_SIZE));
		background.setFillColor(sf::Color(30, 30, 41));
		window.draw(background);

        int cellGap = CELL_SIZE / 6;

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

        if (!isGameOver)
        {
            player.UpdatePosition(deltaTime);
            player2.UpdatePosition(deltaTime);
        }

        window.draw(player.getShape());
		window.draw(player2.getShape());

        /*sf::Text playerOneScore;
        playerOneScore.setFont(font);
        playerOneScore.setCharacterSize(100);
        playerOneScore.setFillColor(sf::Color(153, 255, 125));
        playerOneScore.setString("0");
        playerOneScore.setPosition(WINDOW_SIZE.x / 4 * 3, WINDOW_SIZE.y + 12.5);
        window.draw(playerOneScore);

        sf::Text playerTwoScore;
        playerTwoScore.setFont(font);
        playerTwoScore.setCharacterSize(100);
        playerTwoScore.setFillColor(sf::Color(255, 196, 69));
        playerTwoScore.setString("0");
        playerTwoScore.setPosition(WINDOW_SIZE.x / 4 - playerTwoScore.getLocalBounds().width, WINDOW_SIZE.y + 12.5);
        window.draw(playerTwoScore);*/

        window.display();
    }

    return 0;
}
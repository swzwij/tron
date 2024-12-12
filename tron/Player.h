#pragma once

#include "SFML/Graphics.hpp"
#include "Vector2.h"
#include <iostream>

class Player
{
private:
    int _playerNumber;
    bool _initialized = false;
    Vector2 _direction;
    float _speed = 0.0f;
    float _moveAccumulator = 0.0f;
    Vector2 _position;
    sf::RectangleShape _shape;

    Vector2 _startPosition;
    Vector2 _startDirection;
    float _startSpeed;

    void UpdateDirection(Vector2 input);

    void UpdatePosition();

public:
	int number;
	Vector2 previousPosition;
    bool hasMoved;

    Player(int playerNumber, int x, int y, Vector2 startDirection, sf::Color color, Vector2 gridSize);

    void UpdateInput(Vector2 input);

    bool Update(float deltaTime);

    void Draw(sf::RenderWindow& window);

    Vector2 GetPreviousPosition();

	int GetPlayerNumber();

	Vector2 GetPosition();
};
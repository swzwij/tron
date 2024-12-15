#ifndef PLAYER_H
#define PLAYER_H

#include "SFML/Graphics.hpp"
#include "Vector2.h"
#include <iostream>

class Player
{
private:
    int _playerNumber;
    bool _initialized;
    Vector2 _direction;
    float _speed;
    float _moveAccumulator;
    Vector2 _position;
    sf::RectangleShape _shape;
    Vector2 _startPosition;
    Vector2 _startDirection;
    float _startSpeed;
    Vector2 _gridSize;
    int _cellSize;
    Vector2 _offset;
    Vector2 _previousPosition;

    void UpdateDirection(Vector2 input);
    void UpdatePosition();

public:
    bool hasMoved;

    Player(int playerNumber, int x, int y, Vector2 startDirection, sf::Color color, Vector2 gridSize, int cellSize, Vector2 offset);

    void UpdateInput(Vector2 input);
    bool Update(float deltaTime);
    void Draw(sf::RenderWindow& window);

    Vector2 GetPreviousPosition();
    int GetPlayerNumber();
    Vector2 GetPosition();
};

#endif // PLAYER_H
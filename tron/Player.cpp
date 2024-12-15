#include "Player.h"
#include "SFML/Graphics.hpp"
#include "Vector2.h"
#include <iostream>

void Player::UpdateDirection(Vector2 input)
{
    if (_direction.x == 0 && input.x != 0)
        _direction = input;
    else if (_direction.y == 0 && input.y != 0)
        _direction = input;
}

void Player::UpdatePosition()
{
    if (_position.x + _direction.x < 0 ||
        _position.x + _direction.x > _gridSize.x - 1 ||
        _position.y + _direction.y < 0 ||
        _position.y + _direction.y > _gridSize.y - 1)
        return;

    _position.x += _direction.x;
    _position.y += _direction.y;

    _shape.setPosition(_position.x * _cellSize + _offset.x, _position.y * _cellSize + _offset.y);
}

Player::Player(int playerNumber, int x, int y, Vector2 startDirection, sf::Color color, Vector2 gridSize, int cellSize, Vector2 offset)
    : _playerNumber(playerNumber),
    _startPosition(x, y),
    _startDirection(startDirection),
    _startSpeed(5.0f),
    _position(x, y),
    _direction(startDirection),
    _speed(_startSpeed),
    _gridSize(gridSize),
    _cellSize(cellSize),
    _offset(offset),
    _initialized(false),
    _moveAccumulator(0.0f),
    hasMoved(false)
{
    _shape.setSize(sf::Vector2f(_cellSize, _cellSize));
    _shape.setFillColor(color);

	_speed = _startSpeed;
}

void Player::UpdateInput(Vector2 input)
{
    UpdateDirection(input);
}

bool Player::Update(float deltaTime)
{
    hasMoved = false;
    _moveAccumulator += _speed * deltaTime;

    if (_moveAccumulator >= 1.0f)
    {
        _speed += _speed < 7 ? 0.01f : _speed < 12 ? 0.2f : 0.01f;

        _previousPosition = _position;

        UpdatePosition();

        hasMoved = true;
        _moveAccumulator -= 1.0f;
    }

    return hasMoved;
}

void Player::Draw(sf::RenderWindow& window)
{
    window.draw(_shape);
}

Vector2 Player::GetPreviousPosition()
{
    return _previousPosition;
}

int Player::GetPlayerNumber()
{
    return _playerNumber;
}

Vector2 Player::GetPosition()
{
    return _position;
}
#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
class Entity {
public:
    virtual ~Entity()
    {
    }



    // 纯虚函数：定义接口
    virtual void update(float deltaTime) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
}; 

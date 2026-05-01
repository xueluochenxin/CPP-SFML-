#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.hpp"

class Torpedo : public Entity {
private:
    sf::Sprite sprite;
    float speed;

public:
    // 构造函数：接住长春扔过来的出生坐标和共享贴图
    Torpedo(float startX, float startY, const sf::Texture& texture);

    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;
    bool isOutOfScreen() const;
    sf::FloatRect getGlobalBounds() const;
};

#pragma once
#include <SFML/Graphics.hpp>

class EnemyBullet {
private:
    sf::CircleShape shape;
    float speed;
    int damage; // 极其关键：这颗子弹打人有多疼？
    bool destroyed = false; // 自毁开关

public:
    // 构造函数需要接收坐标和伤害值
    EnemyBullet(float startX, float startY, int dmg);

    void update(float deltaTime);
    void draw(sf::RenderWindow& window);

    bool isOutOfScreen() const;
    sf::FloatRect getGlobalBounds() const;

    void destroy() { destroyed = true; }
    bool isDestroyed() const { return destroyed; }
    int getDamage() const { return damage; } // 交出伤害数值
};
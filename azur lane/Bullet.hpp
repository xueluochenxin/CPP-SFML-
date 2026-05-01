#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.hpp"

class Bullet : public Entity {
private:
    sf::CircleShape shape; 
    float speed;
    bool destroyed = false;
public:
    // 构造函数：需要告诉子弹它出生在哪里（也就是长春当前的位置）
    Bullet(float startX, float startY);

    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;

    // 面试核心：需要一个接口来判断子弹是不是飞出屏幕了
    bool isOutOfScreen() const;
    //判断碰撞
    sf::FloatRect getGlobalBounds() const;
    //销毁
    void destroy() { destroyed = true; }
    bool isDestroyed() const { return destroyed; }
};
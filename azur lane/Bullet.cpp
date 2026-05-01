#include "Bullet.hpp"
#include "Vector2D.hpp"
Bullet::Bullet(float startX, float startY) {
    speed = 800.0f; // 子弹要飞得比长春快得多

    shape.setRadius(5.0f); // 半径 5 像素的小弹幕
    shape.setFillColor(sf::Color::Yellow); // 亮黄色

    // 设置子弹的出生位置（枪口位置）
    shape.setPosition(startX, startY);
}

void Bullet::update(float deltaTime) {
    // 【你的任务 1】：让子弹沿着 X 轴正方向飞行
    // 提示：shape.move(X偏移量, Y偏移量);
    
    shape.move(speed*deltaTime,0.0f);
}

void Bullet::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

bool Bullet::isOutOfScreen() const {
    // 【你的任务 2】：判断越界
    // 假设窗口宽度是 1920，如果子弹的 x 坐标大于 1920，就返回 true。
    // 提示：用 shape.getPosition().x 获取坐标
    int x = this->shape.getPosition().x;
    if (x > 1920.0f)
    {
        return true;
    }
    else
    {
        return false;
    }
}
sf::FloatRect Bullet::getGlobalBounds() const
{
    return shape.getGlobalBounds();
}
#include "Torpedo.hpp"
Torpedo::Torpedo(float startX, float startY, const sf::Texture& texture) {
    // 1. 穿上衣服（设置贴图）
    sprite.setTexture(texture);

    // 2. 调整大小（鱼雷贴图如果太大，在这里把它缩放合适）
    sprite.setScale(0.3f, 0.3f);

    // 3. 传送到枪口
    sprite.setPosition(startX, startY);

    // 4. 速度
    speed = 400.0f;
}

void Torpedo::update(float deltaTime) {
    // 也是向右飞
    sprite.move(speed * deltaTime, 0.0f);
}

void Torpedo::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

bool Torpedo::isOutOfScreen() const {
    // 只要 X 坐标超过屏幕宽度 (假设你现在是 1920)
    return sprite.getPosition().x > 1920.0f;
}
sf::FloatRect Torpedo::getGlobalBounds() const {
    return sprite.getGlobalBounds();
}
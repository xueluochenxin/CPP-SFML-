#include "EnemyBullet.hpp"

EnemyBullet::EnemyBullet(float startX, float startY, int dmg) {
    speed = 500.0f; // 敌方子弹稍微慢一点，给玩家躲避的空间
    damage = dmg;

    shape.setRadius(8.0f); // 敌人的子弹稍微大一点
    shape.setFillColor(sf::Color::Red); // 猩红的警告色

    shape.setPosition(startX, startY);
}

void EnemyBullet::update(float deltaTime) {
    // 敌方子弹是往左飞的！所以 X 是负数
    shape.move(-speed * deltaTime, 0.0f);
}

void EnemyBullet::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

bool EnemyBullet::isOutOfScreen() const {
    // 飞出屏幕左侧（X < -50）判定为越界
    return shape.getPosition().x < -50.0f;
}

sf::FloatRect EnemyBullet::getGlobalBounds() const {
    return shape.getGlobalBounds();
}
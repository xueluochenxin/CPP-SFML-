#include "Enemy.hpp"
#include <iostream>
Enemy::Enemy(float startX, float startY, const sf::Texture& texture, int initialHp,int bulletDamage, int scoreVal)
{
    // 1. 穿上衣服（设置贴图）
    sprite.setTexture(texture);

    // 2. 调整大小（敌人贴图如果太大，在这里把它缩放合适）
    sprite.setScale(0.5f, 0.5f);

    // 3. 传送到枪口
    sprite.setPosition(startX, startY);

    // 4. 速度
    speed = 100.0f;

    //5 hp
    this->hp = initialHp;
    // 6分数
    this->scoreValue = scoreVal;
    moveDirY = 1.0f;
    fireTimer = 0.0f;//开火冷却
    this->hp = initialHp;
    this->bulletDamage = bulletDamage; // 存下子弹伤害
    this->wantToFire = false;          // 默认不开火
}
void Enemy::update(float deltaTime)
{
    float currentY = sprite.getPosition().y;
    fireTimer += deltaTime;
    //判断开火时间
    if (fireTimer >= 1.0f)
    {
    /*    std::cout << "开火了" << std::endl;*/
        fireTimer = 0.0f;//重置为0
        wantToFire = true;
    }
    moveDecisionTimer += deltaTime;
    if (moveDecisionTimer >= 0.3f) { // 每 0.1 秒思考一次
        // rand() % 3 会产生 0, 1, 2。减去 1 就变成了 -1(上), 0(停), 1(下)
        moveDirY = (float)((rand() % 3) - 1);
        moveDecisionTimer = 0.0f;
    }

    // --- 绝对物理边界保护 (优先级高于 AI 思考) ---
   
    if (currentY < 100.0f) {
        moveDirY = 1.0f; // 撞天花板必须往下
    }
    else if (currentY > 800.0f) {
        moveDirY = -1.0f; // 撞海底必须往上
    }

    // 执行移动
    sprite.move(0.0f, speed * moveDirY * deltaTime);
    /*如果 currentY < 100.0f（撞到天花板），就让 moveDirY = 1.0f;（往下走）。

    如果 currentY > 800.0f（撞到海底），就让 moveDirY = -1.0f;（往上走）。

    最后执行移动：sprite.move(0.0f, speed * moveDirY * deltaTime);*/
    /*   if (currentY < 100.0f)
       {
           moveDirY = 1.0f;
       }
       else if (currentY > 800.0f)
       {
           moveDirY = -1.0f;
       }
       sprite.move(0.0f,speed*moveDirY*deltaTime);
   这样的移动太机械了，所以舍弃掉*/ 
}
// 渲染
void Enemy::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}

// 死亡判定
bool Enemy::isDead() const
{
    return hp <= 0;
}

// 受击扣血
void Enemy::takeDamage(int damage)
{
    hp -= damage;
}

// 任务 5：交出碰撞箱（极其关键的一步！）
sf::FloatRect Enemy::getGlobalBounds() const
{
    // 直接调用 SFML 底层的函数，返回这个精灵的物理矩形范围
    return sprite.getGlobalBounds();
}
bool Enemy::popFireRequest() {
    if (wantToFire) {
        wantToFire = false; // 管家看到信号后，把旗子放下
        return true;
    }
    return false;
}

int Enemy::getBulletDamage() const {
    return bulletDamage;
}

sf::Vector2f Enemy::getPosition() const {
    return sprite.getPosition();
}
int Enemy::getScoreValue() const {
    return scoreValue;
}
#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.hpp"

class Enemy : public Entity {
private:
    sf::Sprite sprite;
    float speed;
    int hp; // 生命值
    float moveDirY;   // 当前在往上走(-1.0f)还是往下走(1.0f)？
    float fireTimer;  // 开火冷却秒表
    float moveDecisionTimer = 0.0f; // AI 思考闹钟 这样能移动的更聪明一点
    int bulletDamage;
    bool wantToFire; // 开火信号旗
    int scoreValue;
public:
    // 构造函数：需要出生坐标、贴图，以及最重要的初始血量
    Enemy(float startX, float startY, const sf::Texture& texture, int initialHp,int bulletDamage, int scoreVal);
    bool popFireRequest();            // 告诉管家“我要开火了”
    int getBulletDamage() const;      // 告诉管家“我的子弹伤害是多少”
    sf::Vector2f getPosition() const; // 告诉管家“子弹从哪里生成”
    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;

    
    
    bool isDead() const;         // 血量是否归零？
    void takeDamage(int damage); // 受击扣血接口
    int getScoreValue() const;//分数计算
    // 【碰撞检测核心】：把 SFML 底层计算好的包围盒交出去
    sf::FloatRect getGlobalBounds() const;
};
#pragma once
#include "Entity.hpp"
#include "vector2D.hpp"
#include <SFML/Graphics.hpp>
#include "Bullet.hpp"
#include "Torpedo.hpp"
#include "Airstrike.hpp"
class Player : public Entity {
private:
    sf::Texture texture;
    sf::Sprite sprite;
    float speed;
    float debugTimer = 0.0f;
    float autofiretimer = 0.0f;
    std::vector<std::unique_ptr<Bullet>> bullets;
    sf::Texture torpedoTexture;
    std::unique_ptr<Torpedo> activeTorpedo = nullptr;
    float torpedoCooldownTimer = 0.0f;
    Airstrike airstrikeSkill;
    int hp;
public:
    Player(); // 构造函数声明

    // 注意 override 关键字写在声明里
    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;
    std::vector<std::unique_ptr<Bullet>>& getBullets();
    Torpedo* getActiveTorpedo() { return activeTorpedo.get(); }

    // 如果鱼雷炸了，让外部可以强制销毁它
    void destroyTorpedo() { activeTorpedo.reset(); }

    // 拿大招模块出去
    Airstrike& getAirstrike() { return airstrikeSkill; }
    //player血量和伤害判定
    void takeDamage(int damage);
    bool isDead() const;
    sf::FloatRect getGlobalBounds() const;
}; 

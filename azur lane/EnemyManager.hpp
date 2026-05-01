#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Enemy.hpp"
#include "EnemyBullet.hpp"
class EnemyManager 
{
private:
    // 资源库：提前装好两张贴图
    sf::Texture enemy1Tex;
    sf::Texture enemy2Tex;

    // 花名册：存放所有活着的精英怪
    std::vector<std::unique_ptr<Enemy>> enemies;

    // 刷怪计时器
    float spawnTimer1;
    const float spawnInterval1=3.0f; // 多久刷一次怪（比如 3.0f）
    float spawnTimer2;
    const float spawnInterval2=6.0f; // 多久刷一次怪（比如 6.0f）
    std::vector<std::unique_ptr<EnemyBullet>> enemyBullets;
public:
    EnemyManager();

    void update(float deltaTime, int& mainScore);
    void draw(sf::RenderWindow& window);

    // 【极其关键】：为了能让长春的子弹打到敌人，
    // 我们必须把整个花名册暴露给外部，以便做碰撞检测！
    std::vector<std::unique_ptr<Enemy>>& getEnemies();

    std::vector<std::unique_ptr<EnemyBullet>>& getEnemyBullets();
    void reset();
};
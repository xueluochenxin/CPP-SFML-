#include "EnemyManager.hpp"
#include <iostream>
#include <ctime>
EnemyManager::EnemyManager()
{
    srand(time(NULL));
    if (!enemy1Tex.loadFromFile("enemy1.png")) {
        std::cerr << "enemy1贴图加载失败！请检查图片名字和路径！" << std::endl;
    }
    if (!enemy2Tex.loadFromFile("enemy2.png")) {
        std::cerr << "enemy2贴图加载失败！请检查图片名字和路径！" << std::endl;
    }

    this->spawnTimer1 = 0.0f;
    /*this->spawnInterval1 = 1.5f;*/
    this->spawnTimer2 = 0.0f;
    //this->spawnInterval2 = 3.0f;
}
void EnemyManager::update(float deltaTime, int& mainScore)
{
    this->spawnTimer1 += deltaTime;
    this->spawnTimer2 += deltaTime;
    
    if (this->spawnTimer1 >= this->spawnInterval1)
    {
        this->spawnTimer1 = 0.0f;
        float Ysite1 = 100.0f+rand() % 700;
       /* std::make_unique<Enemy>(1800.0f, Ysite1, enemy1Tex, 3) ;*/
        // Enemy(float startX, float startY, const sf::Texture& texture, int initialHp);
        enemies.push_back(std::make_unique<Enemy>(1500.0f, Ysite1, enemy1Tex, 3,1,1));
    }
    if (this->spawnTimer2 >= this->spawnInterval2)
    {
        this->spawnTimer2 = 0.0f;
        float Ysite2 = 100.0f + rand() % 700;
        
         // Enemy(float startX, float startY, const sf::Texture& texture, int initialHp);
        enemies.push_back(std::make_unique<Enemy>(1500.0f, Ysite2, enemy2Tex, 5,2,2));
    }
    for (auto& e : enemies)
    {
        e->update(deltaTime);
        if (e->popFireRequest()) {
            sf::Vector2f pos = e->getPosition();
            int dmg = e->getBulletDamage();
            // 就在它的枪口生成一颗红色子弹塞进弹匣！
            enemyBullets.push_back(std::make_unique<EnemyBullet>(pos.x, pos.y, dmg));
        }
    }
    auto newend = std::remove_if
    (enemies.begin(), enemies.end(),
        [&mainScore](const std::unique_ptr<Enemy>& e)
        {if (e->isDead())
          {
             mainScore += e->getScoreValue();
             return true;
          }
          else
          {
              return false;
           }
        
        
        }
    );
    enemies.erase(newend,enemies.end());
    for (auto& eb : enemyBullets) {
        eb->update(deltaTime);
    }
    //子弹
    auto bulletEnd = std::remove_if(enemyBullets.begin(), enemyBullets.end(), [](const std::unique_ptr<EnemyBullet>& eb) {
        return eb->isOutOfScreen() || eb->isDestroyed();
        });
    enemyBullets.erase(bulletEnd, enemyBullets.end());
}
//绘制
void EnemyManager::draw(sf::RenderWindow& window)
{
    for (auto& e : enemies)
    {
        e->draw(window);
        
    }
    for (auto& eb : enemyBullets) {
        eb->draw(window);
    }
}
//碰撞体积
std::vector<std::unique_ptr<Enemy>>& EnemyManager::getEnemies()
{
    return enemies;
}


// 在文件末尾实现 getter：
std::vector<std::unique_ptr<EnemyBullet>>& EnemyManager::getEnemyBullets() {
    return enemyBullets;
}
void EnemyManager::reset() {
    enemies.clear();       // 直接清空敌人数组，智能指针会自动析构销毁它们
    enemyBullets.clear();  // 清空所有敌军弹幕
    spawnTimer1 = 0.0f;    // 刷怪秒表归零
    spawnTimer2 = 0.0f;
}
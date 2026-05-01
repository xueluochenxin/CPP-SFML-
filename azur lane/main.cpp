#include <SFML/Graphics.hpp>
#include <iostream>
#include "Background.hpp"
#include <memory>
#include <vector>
#include "Entity.hpp"
#include "vector2D.hpp"
#include "Player.hpp"
#include "Torpedo.hpp"
#include "Bullet.hpp"
#include "Airstrike.hpp"
#include "EnemyManager.hpp"
#include "Enemy.hpp"
int main() 
{
    // ====================================================
    // 第一部分：引擎与窗口初始化
    // ====================================================
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Danmaku Game - Changchun");
    window.setFramerateLimit(60); // 限制 60 帧

    sf::Clock clock;

    // ====================================================
    // 第二部分：游戏核心系统实例化
    // ====================================================
    // 使用 C++14 的 make_unique 创建多态对象
    std::unique_ptr<Player> player = std::make_unique<Player>();
    //实例化背景
    Background gameBackground;

    //实例化敌军管家
    EnemyManager enemyManager;
    sf::Texture gameOverTex;
    //gameover照片渲染
    if (!gameOverTex.loadFromFile("gameover.png")) {
        std::cerr << "Game Over图片加载失败！请检查文件名和路径" << std::endl;
    }
    sf::Sprite gameOverSprite;
    gameOverSprite.setTexture(gameOverTex);
    // 把图片的中心点设为原点，并放到屏幕正中央 (1920x1080的中心)
    sf::FloatRect goBounds = gameOverSprite.getLocalBounds();
    gameOverSprite.setOrigin(goBounds.width / 2.0f, goBounds.height / 2.0f);
    gameOverSprite.setPosition(1920.0f / 2.0f, 1080.0f / 2.0f);
    gameOverSprite.setScale(0.6f, 0.6f);//调节大小
    //游戏控制状态开关
    bool isGameOver = false;
    //计分板
    int currentScore = 0;
    sf::Font gameFont;
    if (!gameFont.loadFromFile("arial.ttf")) { // 请确保你有这个字体文件！
        std::cerr << "字体文件加载失败！" << std::endl;
    }
    sf::Text scoreText;
    scoreText.setFont(gameFont);
    scoreText.setCharacterSize(50);
    scoreText.setFillColor(sf::Color::White); // 先设为白色
    scoreText.setPosition(1600.0f, 30.0f);    // 放在右上角
    // ====================================================
    // 第三部分：游戏主循环
    // ====================================================
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        // --- 3.1 操作系统事件响应 ---
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // --- 3.2 游戏数据与逻辑更新 (Update) ---
        // --- 1. 数据更新 ---
        //用bool isGameOver 判断是否需要结束
        if (isGameOver == false)
        {
            player->update(deltaTime);

            //  enemymanager 更新
            enemyManager.update(deltaTime,currentScore);
            scoreText.setString("SCORE: " + std::to_string(currentScore));
            // --- 3.3 碰撞检测与伤害结算 (Collision) ---
            // ---> 【预留区】：子弹打飞机的逻辑将放在这里 <---

            auto& bullets = player->getBullets();
            auto& enemies = enemyManager.getEnemies();
            // 2. 双重循环：让每一发子弹去和每一个敌人做比对
            for (auto& b : bullets) {
                for (auto& e : enemies) {

                    // 优化：如果子弹已经撞毁了，或者敌人已经死了，直接跳过，不用算碰撞了
                    if (b->isDestroyed() || e->isDead())
                    {
                        continue;
                    }

                    // 【核心碰撞】：纸箱子重叠判定！
                    if (b->getGlobalBounds().intersects(e->getGlobalBounds()))
                    {

                        e->takeDamage(1); // 敌人扣 1 滴血
                        b->destroy();     // 子弹触发自毁开关

                        /* std::cout << "命中目标！敌舰正在扣血！" << std::endl;*/

                         // 关键逻辑：这颗子弹既然已经撞烂了，就不能再穿透去打下一个敌人了
                        break;
                    }
                }
            }
            //鱼雷造成伤害判断
            Torpedo* torpedo = player->getActiveTorpedo();
            if (torpedo != nullptr) {
                for (auto& e : enemies) {
                    if (e->isDead()) continue;

                    // 如果鱼雷撞到敌人
                    if (torpedo->getGlobalBounds().intersects(e->getGlobalBounds())) {
                        e->takeDamage(5); // 鱼雷重击，扣 5 血
                        player->destroyTorpedo(); // 鱼雷爆炸销毁
                        std::cout << "鱼雷命中！重创敌舰！" << std::endl;
                        break; // 鱼雷没了，不用往后算了
                    }
                }
            }
            //航母造成伤害
            if (player->getAirstrike().aoeDamagePending == true) {

                // 2. 一个循环，给在场所有活着的敌人来一记重拳
                for (auto& e : enemies) {
                    if (!e->isDead()) {
                        e->takeDamage(2); // 全体强行扣 2 血
                    }
                }

                // 3. 【极其重要！】扣完血后，瞬间把开关关上！
                // 这样下一帧就不会再重复扣血了，直到你下次再按 K 键放技能。
                player->getAirstrike().aoeDamagePending = false;
            }
            // ==========================================
            // 伤害结算 4：长春受到敌军火力覆盖
            // ==========================================
            auto& enemyBullets = enemyManager.getEnemyBullets();

            for (auto& eb : enemyBullets) {
                // 如果子弹已经自毁，或者长春已经阵亡，就不算碰撞了
                if (eb->isDestroyed() || player->isDead()) continue;

                // 纸箱子重叠判定：敌军子弹撞到了长春！
                if (eb->getGlobalBounds().intersects(player->getGlobalBounds())) {

                    // 动态获取这颗子弹的伤害（小怪是1，精英怪是2）
                    player->takeDamage(eb->getDamage());

                    eb->destroy(); // 子弹撞出火花自毁

                    // 长春阵亡判定
                    if (player->isDead()) {
                        std::cout << "【长春被击沉！Game Over！】" << std::endl;
                        // TODO: 以后可以在这里触发游戏结束画面
                        isGameOver = true;
                    }
                }
            }
        }
        else
        {
            // 【新增】：进入游戏结束状态时，监听 R 键重启！
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {

                // 1. 浴火重生：智能指针直接接管全新对象！旧的长春和子弹会自动析构释放
                player = std::make_unique<Player>();

                // 2. 战场清空：直接赋予大管家一个全新的实例，清空所有敌人和弹幕
                enemyManager.reset();

                // 3. 后台数据归零
                currentScore = 0;

                // 4. 重置分数 UI（把颜色变回白色，防止你以后想修复它变红的逻辑）
                scoreText.setFillColor(sf::Color::White);
                scoreText.setString("SCORE: 0");

                // 5. 放下时间静止开关，游戏重新开始！
                isGameOver = false;

                std::cout << "【系统重置】长春已重新部署，新的一局开始！" << std::endl;
            }
        }
        // --- 3.4 最终画面光栅化渲染 (Draw) ---
        // --- 2. 画面渲染 ---
        gameBackground.draw(window);
        enemyManager.draw(window);
        player->draw(window);
        window.draw(scoreText);
        //游戏结束后代码渲染
        if (isGameOver == true) {
            window.draw(gameOverSprite);
        }

        window.display();
    }

    return 0;
}
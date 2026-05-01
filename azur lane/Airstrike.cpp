#include "Airstrike.hpp"
#include <algorithm> 
#include <iostream>
#include <string>

Airstrike::Airstrike() {
    // 1. 加载立绘
    if (!unicornTex.loadFromFile("unicorn.png")) {
        std::cerr << "unicorn.png 加载失败！\n";
    }
    unicornSprite.setTexture(unicornTex);
    unicornSprite.setPosition(0.0f, 0.0f);
    unicornSprite.setScale(0.5f, 0.5f); // 【调参点】：修改这里调整立绘大小

    // 2. 加载飞机
    if (!plane1Tex.loadFromFile("plane1.png")) std::cerr << "plane1.png 加载失败！\n";
    if (!plane2Tex.loadFromFile("plane2.png")) std::cerr << "plane2.png 加载失败！\n";

    // 3. 加载 4 个炸弹 (利用循环拼接字符串 "bomb1.png" ~ "bomb4.png")
    for (int i = 0; i < 4; ++i) {
        std::string filename = "bomb" + std::to_string(i + 1) + ".png";
        if (!bombTexs[i].loadFromFile(filename)) {
            std::cerr << filename << " 加载失败！\n";
        }
    }
}

void Airstrike::trigger() {
    // 没在放技能，且 CD 转好了，才能触发
    if (!isActive && cooldownTimer <= 0.0f) {
        isActive = true;
        activeTimer = 0.0f;
        cooldownTimer = 20.0f; // 大招冷却 20 秒
        aoeDamagePending = true;
        // 清空残留，防止出 Bug
        planes.clear();
        bombs.clear();

        // 制造 4 架飞机 (交替使用两种贴图)
        for (int i = 0; i < 4; ++i) {
            sf::Sprite p;
            if (i % 2 == 0) {
                p.setTexture(plane1Tex);
            }
            else {
                p.setTexture(plane2Tex);
            }

            p.setScale(0.3f, 0.3f); // 【调参点】：修改这里调整飞机大小

            // 出生在屏幕左侧外面，Y 轴每隔 200 像素排开
            p.setPosition(-300.0f, 200.0f + i * 200.0f);
            planes.push_back(p);
        }

        // 制造 4 颗炸弹
        for (int i = 0; i < 8; ++i) {
            sf::Sprite b;

            // 【核心算法】：i 取值为 0 到 7。
            // i % 4 的结果永远是 0, 1, 2, 3, 0, 1, 2, 3，完美循环使用 4 种贴图！
            b.setTexture(bombTexs[i % 4]);

            b.setScale(0.3f, 0.3f); // 依然留给你亲自调参

            // 【重新计算阵型】：
            // 屏幕宽 1920，放 8 个炸弹，大概每隔 220 像素排布一个。
            // 顺便给 Y 轴加一个错落感，让它们不是齐刷刷地平着掉下来，而是有高有低。
            float startX = 150.0f + i * 220.0f;
            float startY = -200.0f - (i % 2) * 150.0f; // 奇数和偶数的炸弹高度错开

            b.setPosition(startX, startY);
            bombs.push_back(b);
        }
    }
}

void Airstrike::update(float deltaTime) {
    // 冷却时间一直在后台转
    if (cooldownTimer > 0.0f) {
        cooldownTimer -= deltaTime;
    }

    if (isActive) {
        activeTimer += deltaTime; // 推进时间轴

        // 1. 让特效移动
        for (auto& p : planes) {
            p.move(1000.0f * deltaTime, 0.0f); // 飞机向右飞 (速度 1000)
        }
        for (auto& b : bombs) {
            b.move(0.0f, 800.0f * deltaTime);  // 炸弹向下掉 (速度 800)
        }

        // 2. 死亡收割：飞机越过右边界 (1920) 就删
        planes.erase(
            std::remove_if(planes.begin(), planes.end(),
                [](const sf::Sprite& p) { return p.getPosition().x > 1920.0f; }
            ),
            planes.end()
        );

        // 3. 死亡收割：炸弹掉出下边界 (1080) 就删
        bombs.erase(
            std::remove_if(bombs.begin(), bombs.end(),
                [](const sf::Sprite& b) { return b.getPosition().y > 1080.0f; }
            ),
            bombs.end()
        );

        // 4. 大招终结：东西全飞完了
        if (planes.empty() && bombs.empty()) {
            isActive = false;
        }
    }
}

void Airstrike::draw(sf::RenderWindow& window) {
    if (isActive) {
        // 立绘只在时间轴的前 3 秒显示
        if (activeTimer <= 3.0f) {
            window.draw(unicornSprite);
        }

        for (auto& p : planes) window.draw(p);
        for (auto& b : bombs)  window.draw(b);
    }
}
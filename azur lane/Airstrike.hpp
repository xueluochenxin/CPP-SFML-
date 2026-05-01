#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

class Airstrike {
private:
    // --- 1. 资源库 (只读一次硬盘) ---
    sf::Texture unicornTex;
    sf::Texture plane1Tex;
    sf::Texture plane2Tex;
    sf::Texture bombTexs[4]; // 用一个容量为 4 的数组装炸弹贴图

    // --- 2. 立绘本体 ---
    sf::Sprite unicornSprite;

    // --- 3. 活跃特效池 (装载当前屏幕上正在飞的特效) ---
    std::vector<sf::Sprite> planes;
    std::vector<sf::Sprite> bombs;

    // --- 4. 核心状态机与时间轴 ---
    bool isActive = false;     // 大招是否正在演出中？
    float cooldownTimer = 0.0f;// K 键冷却时间
    float activeTimer = 0.0f;  // 演出进行到第几秒了？

public:
    Airstrike();
    void trigger();
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    // 全屏伤害开关。默认为 false，没开大招时不造成伤害
    bool aoeDamagePending = false;
};

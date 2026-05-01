#include <iostream>
#include "Player.hpp"
#include <algorithm>
#include "Airstrike.hpp"

Player::Player() {
    // 加载刚才准备好的长春图片
    if (!texture.loadFromFile("player.png")) {
        std::cerr << "贴图加载失败！请检查图片名字和路径！" << std::endl;
    }
    if (!torpedoTexture.loadFromFile("torpedo.png")) {
        std::cerr << "鱼雷贴图加载失败！请检查图片名字和路径！" << std::endl;
    }
    sprite.setTexture(texture);
    //大小调节
    // 
    sprite.setScale(0.2f, 0.2f);
    // 将中心点设为图片中心 (假设图片大小，SFML 会自动获取)
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);

    // 初始位置
    sprite.setPosition(400.0f, 500.0f);
    //初始化速度
    this->speed = 300.0f;
    sprite.setScale(0.8f, 0.8f);
    //初始化血量
    this->hp = 10;
    
}

void Player::update(float deltaTime) {
    // ====================================================
    // 第一部分：长春本体移动与调试
    // ====================================================
    // 用刚才写的 Vector2D 来计算位移方向
    Vector2D direction(0.0f, 0.0f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) direction.y -= 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) direction.y += 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) direction.x -= 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) direction.x += 1.0f;

    // 利用你刚才重载的乘法，计算实际位移：direction * (speed * deltaTime)
    // 然后使用 SFML 的 API 移动长春：sprite.move(位移的x, 位移的y);
    direction = direction * speed * deltaTime;
    sprite.move(direction.x, direction.y);

    debugTimer += deltaTime;
    if (debugTimer >= 2.0f) {
        // 闹钟响了，打印的调试信息
        std::cout << "当前坐标 -> X: " << sprite.getPosition().x
            << ", Y: " << sprite.getPosition().y
            << " | 此时的速度 speed 是: " << speed << std::endl;

        // 重置秒表！
        debugTimer = 0.0f;
    }
    /*std::cout << "当前坐标 -> X: " << sprite.getPosition().x
        << ", Y: " << sprite.getPosition().y
        << " | 此时的速度 speed 是: " << speed << std::endl;*/


        // ====================================================
        // 第二部分：主武器 (普通子弹自动开火)
        // ====================================================
    autofiretimer += deltaTime;
    if (autofiretimer >= 0.5f)
    {
        float gunX = sprite.getPosition().x;
        float gunY = sprite.getPosition().y;

        // 【关键！】制造一颗子弹，并塞进弹匣里
        bullets.push_back(std::make_unique<Bullet>(gunX, gunY));

        autofiretimer = 0.0f;
    }

    for (auto& b : bullets)
    {
        b->update(deltaTime);
    }

    //erase 删除子弹
    auto newend = std::remove_if
    (
        bullets.begin(),
        bullets.end(),
        [](const std::unique_ptr<Bullet>& b)
        {return b->isOutOfScreen()||b->isDestroyed(); }
    );
        bullets.erase(newend, bullets.end());
    // ====================================================
    // 第三部分：副武器 (J 键必杀鱼雷)
    // ====================================================
    // --- 必杀鱼雷逻辑 ---

    // 1. 冷却秒表倒计时
    if (torpedoCooldownTimer > 0.0f) {
        torpedoCooldownTimer -= deltaTime;
    }

    // 2. 监听 J 键（必须冷却完毕！）
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::J) && torpedoCooldownTimer <= 0.0f) {
        float gunX = sprite.getPosition().x;
        float gunY = sprite.getPosition().y;

        // 极其暴力的内存管理：直接 new 一个新鱼雷接管指针。
        // 如果这时候屏幕上还有旧鱼雷没飞完，智能指针会瞬间把旧的销毁，换成新的。
        activeTorpedo = std::make_unique<Torpedo>(gunX, gunY, torpedoTexture);

        // 重置 10 秒大招冷却 (测试时可以先改成 3 秒方便看效果)
        torpedoCooldownTimer = 10.0f;
    }

    // 3. 让鱼雷飞，并处理它的生死
    // 必须先判断 activeTorpedo 是不是空的！否则直接调用 update 会导致空指针崩溃。
    if (activeTorpedo != nullptr) {
        activeTorpedo->update(deltaTime);

        // 越界检查：飞出屏幕就销毁它
        if (activeTorpedo->isOutOfScreen()) {
            // reset() 是 unique_ptr 的神技！
            // 它的作用是：立刻销毁内存中的鱼雷对象，并把指针重新设为 nullptr。
            activeTorpedo.reset();
        }
    }


    // ====================================================
    // 第四部分：支援技能 (K 键航母大招)
    // ====================================================
    // 1. 监听 K 键触发大招
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::K)) {
        airstrikeSkill.trigger();
    }

    // 2. 无论有没有放技能，每帧都必须让大招模块更新自己（用来走 CD 和飞飞机）
    airstrikeSkill.update(deltaTime);
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(sprite);
    for (auto& b : bullets)
    {
        b->draw(window);
    }
    if (activeTorpedo != nullptr) {
        activeTorpedo->draw(window);
    }
    airstrikeSkill.draw(window);
}
std::vector<std::unique_ptr<Bullet>>& Player::getBullets()
{
    return this->bullets;
}
void Player::takeDamage(int damage) {
    hp -= damage;
    std::cout << "警告！长春受到攻击！剩余装甲: " << hp << "/10" << std::endl;
}

bool Player::isDead() const {
    return hp <= 0;
}

sf::FloatRect Player::getGlobalBounds() const {
    return sprite.getGlobalBounds();
}
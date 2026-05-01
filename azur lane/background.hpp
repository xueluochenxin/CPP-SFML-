#pragma once
#include <SFML/Graphics.hpp>

class Background {
private:
    sf::Texture texture;
    sf::Sprite sprite;

public:
    Background(); // 构造函数，用来加载背景图和调整大小
    void draw(sf::RenderWindow& window); // 专门负责把背景画出来的函数
};

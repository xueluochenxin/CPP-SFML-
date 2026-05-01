#include "Background.hpp"
#include <iostream>

Background::Background() {
    // 【任务 1】：像加载长春一样，用 texture.loadFromFile 加载 "background.png"
    if (!texture.loadFromFile("background.png")) {
        std::cerr << "贴图加载失败！请检查图片名字和路径！" << std::endl;
    }
    

    // 【任务 2】：把 texture 设置给 sprite
    sprite.setTexture(texture);

    // 【任务 3（关键）】：背景图可能和你的窗口大小不匹配，需要缩放拉伸。
    
    // 获取图片实际大小： sf::Vector2u size = texture.getSize();
    // 算出 X 轴缩放比例： 1920.0f / size.x
    // 算出 Y 轴缩放比例： 1080.0f / size.y
    // 最后调用： sprite.setScale(X比例, Y比例); 
    sf::Vector2u size = texture.getSize();
    float xrate = 1920.0f / size.x;
    float yrate= 1080.0f / size.y;
    sprite.setScale(xrate, yrate);
}

void Background::draw(sf::RenderWindow& window) {
    // 【任务 4】：把 sprite 画出来
    window.draw(sprite);
}
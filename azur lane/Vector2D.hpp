#pragma once
class Vector2D {
public:
    float x, y;
    Vector2D(float _x = 0.0f, float _y = 0.0f) : x(_x), y(_y) {}

    // 重载 += 运算符】
    // 目标：让当前向量加上另一个向量 other。

    Vector2D& operator+=(const Vector2D& other) {
        // 在这里写你的代码...
        this->x = this->x + other.x;
        this->y = this->y + other.y;
        return *this;
    }

    // 重载 * 运算符 (标量乘法)
    // 目标：实现类似 Vector2D newVec = myVec * deltaTime;
    // 提示：返回一个新的 Vector2D 对象，它的 x 和 y 是原来的乘以 scalar
    Vector2D operator*(float scalar) const {


        return Vector2D(this->x * scalar, this->y * scalar);
    }
};

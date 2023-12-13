#include "../headers/vector2.h"

Vector2::Vector2() {
    this->x = 0;
    this->y = 0;
}

Vector2::Vector2(double x, double y) {
    this->x = x;
    this->y = y;
}

Vector2 Vector2::operator+(const Vector2& other) {
    return Vector2(this->x + other.x, this->y + other.y);
}

Vector2 Vector2::operator-(const Vector2& other) {
    return Vector2(this->x - other.x, this->y - other.y);
}

Vector2 Vector2::operator*(double k) {
    return Vector2(this->x * k, this->y * k);
}

Vector2 Vector2::operator/(double k) {
    return Vector2(this->x / k, this->y / k);
}

Vector2& Vector2::operator+=(const Vector2& other) {
    this->x += other.x;
    this->y += other.y;
    return *this;
}

Vector2& Vector2::operator-=(const Vector2& other) {
    this->x -= other.x;
    this->y -= other.y;
    return *this;
}

Vector2& Vector2::operator*=(double k) {
    this->x *= k;
    this->y *= k;
    return *this;
}

Vector2& Vector2::operator/=(double k) {
    this->x /= k;
    this->y /= k;
    return *this;
}

Vector2::~Vector2() {

}
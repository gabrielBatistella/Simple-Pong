#ifndef VECTOR2_H
#define VECTOR2_H

class Vector2 {
    public:
        double x;
        double y;

        Vector2();
        Vector2(double x, double y);

        Vector2 operator+(const Vector2& other);
        Vector2 operator-(const Vector2& other);
        Vector2 operator*(double k);
        Vector2 operator/(double k);
        Vector2& operator+=(const Vector2& other);
        Vector2& operator-=(const Vector2& other);
        Vector2& operator*=(double k);
        Vector2& operator/=(double k);

        ~Vector2();
};

#endif
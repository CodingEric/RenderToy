/*
 *  OpenPT - Math Function Module
 *  File created on 2023/1/10
 *  Tianyu Huang <tianyu@illumiart.net>
 */

#include <cmath>

#include "mathfunc.h"

using namespace OpenPT;

Vector3f::Vector3f(const float x_, const float y_, const float z_) : x(x_), y(y_), z(z_) {}
Vector3f::Vector3f() : Vector3f(0.0f, 0.0f, 0.0f) {}
Vector3f::Vector3f(const std::array<float, 3> &triple) : Vector3f(triple[0], triple[1], triple[2]) {}

float Vector3f::Length()
{
    return sqrt(x * x + y * y + z * z);
}

void Vector3f::Normalize()
{
    (*this) /= Length();
}

const Vector3f Vector3f::Normalized()
{
    return Vector3f(*this) / Length();
}

float &Vector3f::operator[](const int i)
{
    return *((&x) + i);
}

const Vector3f Vector3f::operator+(const Vector3f &a)
{
    return Vector3f(x + a.x, y + a.y, z + a.z);
}

const Vector3f Vector3f::operator-(const Vector3f &a)
{
    return Vector3f(x - a.x, y - a.y, z - a.z);
}

const Vector3f Vector3f::operator*(const float &a)
{
    return Vector3f(x * a, y * a, z * a);
}

const Vector3f Vector3f::operator/(const float &a)
{
    return Vector3f(x / a, y / a, z / a);
}

const Vector3f &Vector3f::operator+=(const Vector3f &a)
{
    x += a.x;
    y += a.y;
    z += a.z;
    return (*this);
}

const Vector3f &Vector3f::operator-=(const Vector3f &a)
{
    x -= a.x;
    y -= a.y;
    z -= a.z;
    return (*this);
}

const Vector3f &Vector3f::operator*=(const float a)
{
    x *= a;
    y *= a;
    z *= a;
    return (*this);
}

const Vector3f &Vector3f::operator/=(const float a)
{
    x /= a;
    y /= a;
    z /= a;
    return (*this);
}

const bool Vector3f::operator==(const Vector3f &a)
{
    return ((x == a.x) && (y == a.y) && (z == a.z));
}
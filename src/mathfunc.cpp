/*
 *  OpenPT - Math Function Module
 *  File created on 2023/1/10
 *  Last edited on 2023/1/17
 *  Tianyu Huang <tianyu@illumiart.net>
 */

#include <cmath>

#include "mathfunc.h"

namespace OpenPT
{
    Vector3f::Vector3f(const float x_, const float y_, const float z_) : x(x_), y(y_), z(z_) {}
    Vector3f::Vector3f() : Vector3f(0.0f, 0.0f, 0.0f) {}
    Vector3f::Vector3f(const std::array<float, 3> &triple) : Vector3f(triple[0], triple[1], triple[2]) {}

    Vector3f::Vector3f(const Vector2f &vec2, float val3)
        : x(vec2.x), y(vec2.y), z(val3)
    {
    }

    const Vector3f Vector3f::O = Vector3f();
    const Vector3f Vector3f::X = Vector3f(1.0f, 0.0f, 0.0f);
    const Vector3f Vector3f::Y = Vector3f(0.0f, 1.0f, 0.0f);
    const Vector3f Vector3f::Z = Vector3f(0.0f, 0.0f, 1.0f);
    const Vector3f Vector3f::White = Vector3f(1.0f, 1.0f, 1.0f);

    float Vector3f::Length() const
    {
        return sqrt(x * x + y * y + z * z);
    }

    void Vector3f::Normalize()
    {
        (*this) /= Length();
    }

    const Vector3f Vector3f::Normalized() const
    {
        return Vector3f(*this) / Length();
    }

    float &Vector3f::operator[](const size_t i)
    {
        return *((&x) + i);
    }

    const float Vector3f::operator[](const size_t i) const
    {
        return *((&x) + i);
    }

    const Vector3f Vector3f::operator+(const Vector3f &a) const
    {
        return Vector3f(x + a.x, y + a.y, z + a.z);
    }

    const Vector3f Vector3f::operator-(const Vector3f &a) const
    {
        return Vector3f(x - a.x, y - a.y, z - a.z);
    }

    const Vector3f Vector3f::operator-(void) const
    {
        return Vector3f(-x, -y, -z);
    }

    const Vector3f Vector3f::operator*(const float a) const
    {
        return Vector3f(x * a, y * a, z * a);
    }

    const Vector3f Vector3f::operator/(const float a) const
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

    const bool Vector3f::operator==(const Vector3f &a) const
    {
        return ((std::abs(x - a.x) < EPS) && (std::abs(y - a.y) < EPS) && (std::abs(z - a.z) < EPS));
    }

    const Vector3f operator*(const float lambda, const Vector3f &a)
    {
        return a * lambda;
    }

    const float Vector3f::Dot(const Vector3f &a, const Vector3f &b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    const Vector3f Vector3f::Cross(const Vector3f &a, const Vector3f &b)
    {
        return Vector3f(a.y * b.z - a.z * b.y,
                        a.z * b.x - a.x * b.z,
                        a.x * b.y - a.y * b.x);
    }

    const float Vector3f::Dot(const Vector3f &a) const
    {
        return x * a.x + y * a.y + z * a.z;
    }

    const Vector3f Vector3f::Cross(const Vector3f &a) const
    {
        return Vector3f(y * a.z - z * a.y,
                        z * a.x - x * a.z,
                        x * a.y - y * a.x);
    }

    std::istream &operator>>(std::istream &is, Vector3f &a)
    {
        is >> a.x >> a.y >> a.z;
        return is;
    }

    Vector2f::Vector2f(const float x_, const float y_) : x(x_), y(y_) {}
    Vector2f::Vector2f() : Vector2f(0.0f, 0.0f) {}
    Vector2f::Vector2f(const std::array<float, 2> &tuple) : Vector2f(tuple[0], tuple[1]) {}

    Vector2f::Vector2f(const Point &p)
        : x(p.x), y(p.y)
    {
    }

    const Vector2f Vector2f::O = Vector2f();
    const Vector2f Vector2f::X = Vector2f(1.0f, 0.0f);
    const Vector2f Vector2f::Y = Vector2f(0.0f, 1.0f);

    float Vector2f::Length() const
    {
        return sqrt(x * x + y * y);
    }

    void Vector2f::Normalize()
    {
        (*this) /= Length();
    }

    const Vector2f Vector2f::Normalized() const
    {
        return Vector2f(*this) / Length();
    }

    float &Vector2f::operator[](const size_t i)
    {
        return *((&x) + i);
    }

    const float Vector2f::operator[](const size_t i) const
    {
        return *((&x) + i);
    }

    const Vector2f Vector2f::operator+(const Vector2f &a) const
    {
        return Vector2f(x + a.x, y + a.y);
    }

    const Vector2f Vector2f::operator-(const Vector2f &a) const
    {
        return Vector2f(x - a.x, y - a.y);
    }

    const Vector2f Vector2f::operator-(void) const
    {
        return Vector2f(-x, -y);
    }

    const Vector2f Vector2f::operator*(const float a) const
    {
        return Vector2f(x * a, y * a);
    }

    const Vector2f Vector2f::operator/(const float a) const
    {
        return Vector2f(x / a, y / a);
    }

    const Vector2f &Vector2f::operator+=(const Vector2f &a)
    {
        x += a.x;
        y += a.y;
        return (*this);
    }

    const Vector2f &Vector2f::operator-=(const Vector2f &a)
    {
        x -= a.x;
        y -= a.y;
        return (*this);
    }

    const Vector2f &Vector2f::operator*=(const float a)
    {
        x *= a;
        y *= a;
        return (*this);
    }

    const Vector2f &Vector2f::operator/=(const float a)
    {
        x /= a;
        y /= a;
        return (*this);
    }

    const bool Vector2f::operator==(const Vector2f &a) const
    {
        return ((std::abs(x - a.x) < EPS) && (std::abs(y - a.y) < EPS));
    }

    const Vector2f operator*(const float lambda, const Vector2f &a)
    {
        return a * lambda;
    }

    const float Vector2f::Dot(const Vector2f &a, const Vector2f &b)
    {
        return a.x * b.x + a.y * b.y;
    }

    const float Vector2f::Cross(const Vector2f &a, const Vector2f &b)
    {
        return a.x * b.y - a.y * b.x;
    }

    std::istream &operator>>(std::istream &is, Vector2f &a)
    {
        is >> a.x >> a.y;
        return is;
    }

    Vector4f::Vector4f(const float x_, const float y_, const float z_, const float w_) : x(x_), y(y_), z(z_), w(w_) {}
    Vector4f::Vector4f() : Vector4f(0.0f, 0.0f, 0.0f, 0.0f) {}
    Vector4f::Vector4f(const std::array<float, 4> &quadruple) : Vector4f(quadruple[0], quadruple[1], quadruple[2], quadruple[3]) {}

    Vector4f::Vector4f(const Vector3f &vec3, float val4)
        : Vector4f(vec3[0], vec3[1], vec3[2], val4)
    {
    }

    const Vector4f Vector4f::O = Vector4f();
    const Vector4f Vector4f::X = Vector4f(1.0f, 0.0f, 0.0f, 0.0f);
    const Vector4f Vector4f::Y = Vector4f(0.0f, 1.0f, 0.0f, 0.0f);
    const Vector4f Vector4f::Z = Vector4f(0.0f, 0.0f, 1.0f, 0.0f);
    const Vector4f Vector4f::W = Vector4f(0.0f, 0.0f, 0.0f, 1.0f);

    float Vector4f::Length() const
    {
        return sqrt(x * x + y * y + z * z + w * w);
    }

    void Vector4f::Normalize()
    {
        (*this) /= Length();
    }

    const Vector4f Vector4f::Normalized() const
    {
        return Vector4f(*this) / Length();
    }

    float &Vector4f::operator[](const size_t i)
    {
        return *((&x) + i);
    }

    const float Vector4f::operator[](const size_t i) const
    {
        return *((&x) + i);
    }

    const Vector4f Vector4f::operator+(const Vector4f &a) const
    {
        return Vector4f(x + a.x, y + a.y, z + a.z, w + a.w);
    }

    const Vector4f Vector4f::operator-(const Vector4f &a) const
    {
        return Vector4f(x - a.x, y - a.y, z - a.z, w - a.w);
    }

    const Vector4f Vector4f::operator-(void) const
    {
        return Vector4f(-x, -y, -z, -w);
    }

    const Vector4f Vector4f::operator*(const float a) const
    {
        return Vector4f(x * a, y * a, z * a, w * a);
    }

    const Vector4f Vector4f::operator/(const float a) const
    {
        return Vector4f(x / a, y / a, z / a, w / a);
    }

    const Vector4f &Vector4f::operator+=(const Vector4f &a)
    {
        x += a.x;
        y += a.y;
        z += a.z;
        w += a.w;
        return (*this);
    }

    const Vector4f &Vector4f::operator-=(const Vector4f &a)
    {
        x -= a.x;
        y -= a.y;
        z -= a.z;
        w -= a.w;
        return (*this);
    }

    const Vector4f &Vector4f::operator*=(const float a)
    {
        x *= a;
        y *= a;
        z *= a;
        w *= a;
        return (*this);
    }

    const Vector4f &Vector4f::operator/=(const float a)
    {
        x /= a;
        y /= a;
        z /= a;
        w /= a;
        return (*this);
    }

    const bool Vector4f::operator==(const Vector4f &a) const
    {
        return ((std::abs(x - a.x) < EPS) && (std::abs(y - a.y) < EPS) && (std::abs(z - a.z) < EPS) && (std::abs(w - a.w) < EPS));
    }

    const Vector4f operator*(const float lambda, const Vector4f &a)
    {
        return a * lambda;
    }

    const float Convert::InchToMM(const float inch)
    {
        return inch * 25.4f;
    }

    const float Convert::DegreeToRadians(const float deg)
    {
        return deg * (PI / 180);
    }

    const float Vector4f::Dot(const Vector4f &a, const Vector4f &b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
    }

    const Vector4f Vector4f::Cross(const Vector4f &a, const Vector4f &b)
    {
        // TODO <tianyu@illumiart.net>: Implement 4D-vector cross product ustd::sing matrix operation.
        return Vector4f(0.0f, 0.0f, 0.0f, 0.0f);
    }

    Matrix4x4f::Matrix4x4f(Vector4f r0, Vector4f r1, Vector4f r2, Vector4f r3)
    {
        row[0] = r0;
        row[1] = r1;
        row[2] = r2;
        row[3] = r3;
    }

    Matrix4x4f::Matrix4x4f(std::array<Vector4f, 4> quadruple) : Matrix4x4f(quadruple[0], quadruple[1], quadruple[2], quadruple[3]) {}
    Matrix4x4f::Matrix4x4f() : Matrix4x4f(Vector4f::O, Vector4f::O, Vector4f::O, Vector4f::O) {}

    const Matrix4x4f Matrix4x4f::I = Matrix4x4f(Vector4f::X, Vector4f::Y, Vector4f::Z, Vector4f::W);
    const Matrix4x4f Matrix4x4f::O = Matrix4x4f();

    Matrix4x4f::Matrix4x4f(const Matrix4x4f &a)
    {
        for (int i = 0; i < 4; ++i)
        {
            row[i] = a[i];
        }
    }

    Vector4f &Matrix4x4f::operator[](const size_t i)
    {
        return row[i];
    }

    const Vector4f &Matrix4x4f::operator[](const size_t i) const
    {
        return row[i];
    }

    const bool Matrix4x4f::operator==(const Matrix4x4f &a) const
    {
        return (row[0] == a[0] && row[1] == a[1] && row[2] == a[2] && row[3] == a[3]);
    }

    const Matrix4x4f Matrix4x4f::operator+(const Matrix4x4f &a) const
    {
        Matrix4x4f ret;
        for (int i = 0; i < 4; ++i)
        {
            ret[i] = row[i] + a[i];
        }
        return ret;
    }

    const Matrix4x4f Matrix4x4f::operator-(const Matrix4x4f &a) const
    {
        Matrix4x4f ret;
        for (int i = 0; i < 4; ++i)
        {
            ret[i] = row[i] - a[i];
        }
        return ret;
    }

    const Matrix4x4f Matrix4x4f::operator-(void) const
    {
        return Matrix4x4f(-row[0], -row[1], -row[2], -row[3]);
    }

    const Vector4f Matrix4x4f::operator*(const Vector4f &a) const
    {
        Vector4f ret;
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                ret[i] += row[i][j] * a[j];
            }
        }
        return ret;
    }

    const Matrix4x4f Matrix4x4f::operator*(const Matrix4x4f &a) const
    {
        Matrix4x4f ret;
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                for (int k = 0; k < 4; ++k)
                {
                    ret[i][j] += row[i][k] * a[k][j];
                }
            }
        }
        return ret;
    }

    const Matrix4x4f &Matrix4x4f::operator+=(const Matrix4x4f &a)
    {
        for (int i = 0; i < 4; ++i)
        {
            row[i] += a[i];
        }
        return (*this);
    }

    const Matrix4x4f &Matrix4x4f::operator-=(const Matrix4x4f &a)
    {
        for (int i = 0; i < 4; ++i)
        {
            row[i] -= a[i];
        }
        return (*this);
    }

    void Matrix4x4f::Transpose()
    {
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                row[i][j] = row[j][i];
            }
        }
    }

    const Matrix4x4f Matrix4x4f::Transposed() const
    {
        Matrix4x4f ret = (*this);
        ret.Transpose();
        return ret;
    }

    const Matrix3x3f Matrix4x4f::ComplementMinor(const int i, const int j) const
    {
        Matrix3x3f ret;
        for (int x = 0, ret_x = 0; x < 4; ++x)
        {
            if (x == i)
            {
                continue;
            }
            for (int y = 0, ret_y = 0; y < 4; ++y)
            {
                if (y == j)
                {
                    continue;
                }
                ret[ret_x][ret_y] = row[x][y];
                ++ret_y;
            }
            ++ret_x;
        }
        return ret;
    }

    Matrix3x3f::Matrix3x3f(Vector3f r0, Vector3f r1, Vector3f r2)
    {
        row[0] = r0;
        row[1] = r1;
        row[2] = r2;
    }

    Matrix3x3f::Matrix3x3f(std::array<Vector3f, 3> triple) : Matrix3x3f(triple[0], triple[1], triple[2]) {}
    Matrix3x3f::Matrix3x3f() : Matrix3x3f(Vector3f::O, Vector3f::O, Vector3f::O) {}

    const Matrix3x3f Matrix3x3f::I = Matrix3x3f(Vector3f::X, Vector3f::Y, Vector3f::Z);
    const Matrix3x3f Matrix3x3f::O = Matrix3x3f();

    Matrix3x3f::Matrix3x3f(const Matrix3x3f &a)
    {
        for (int i = 0; i < 3; ++i)
        {
            row[i] = a[i];
        }
    }

    Vector3f &Matrix3x3f::operator[](const size_t i)
    {
        return row[i];
    }

    const Vector3f &Matrix3x3f::operator[](const size_t i) const
    {
        return row[i];
    }

    const bool Matrix3x3f::operator==(const Matrix3x3f &a) const
    {
        return (row[0] == a[0] && row[1] == a[1] && row[2] == a[2]);
    }

    const Matrix3x3f Matrix3x3f::operator+(const Matrix3x3f &a) const
    {
        Matrix3x3f ret;
        for (int i = 0; i < 3; ++i)
        {
            ret[i] = row[i] + a[i];
        }
        return ret;
    }

    const Matrix3x3f Matrix3x3f::operator-(const Matrix3x3f &a) const
    {
        Matrix3x3f ret;
        for (int i = 0; i < 3; ++i)
        {
            ret[i] = row[i] - a[i];
        }
        return ret;
    }

    const Matrix3x3f Matrix3x3f::operator-(void) const
    {
        return Matrix3x3f(-row[0], -row[1], -row[2]);
    }

    const Vector3f Matrix3x3f::operator*(const Vector3f &a) const
    {
        Vector3f ret;
        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                ret[i] += row[i][j] * a[j];
            }
        }
        return ret;
    }

    const Matrix3x3f Matrix3x3f::operator*(const Matrix3x3f &a) const
    {
        Matrix3x3f ret;
        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                for (int k = 0; k < 3; ++k)
                {
                    ret[i][j] += row[i][k] * a[k][j];
                }
            }
        }
        return ret;
    }

    const Matrix3x3f &Matrix3x3f::operator+=(const Matrix3x3f &a)
    {
        for (int i = 0; i < 3; ++i)
        {
            row[i] += a[i];
        }
        return (*this);
    }

    const Matrix3x3f &Matrix3x3f::operator-=(const Matrix3x3f &a)
    {
        for (int i = 0; i < 3; ++i)
        {
            row[i] -= a[i];
        }
        return (*this);
    }

    void Matrix3x3f::Transpose()
    {
        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                row[i][j] = row[j][i];
            }
        }
    }

    const Matrix3x3f Matrix3x3f::Transposed() const
    {
        Matrix3x3f ret = (*this);
        ret.Transpose();
        return ret;
    }

    const float Matrix3x3f::Determinant(const Matrix3x3f &mat)
    {
        return mat[0][0] * Matrix2x2f::Determinant((Matrix2x2f){{mat[1][1], mat[1][2]}, {mat[2][1], mat[2][2]}}) - mat[0][1] * Matrix2x2f::Determinant((Matrix2x2f){{mat[1][0], mat[1][2]}, {mat[2][0], mat[2][2]}}) + mat[0][2] * Matrix2x2f::Determinant((Matrix2x2f){{mat[1][0], mat[1][1]}, {mat[2][0], mat[2][1]}});
    }

    Matrix2x2f::Matrix2x2f(Vector2f r0, Vector2f r1)
    {
        row[0] = r0;
        row[1] = r1;
    }
    Matrix2x2f::Matrix2x2f(std::array<Vector2f, 2> tuple) : Matrix2x2f(tuple[0], tuple[1]) {}
    Matrix2x2f::Matrix2x2f() : Matrix2x2f(Vector2f::O, Vector2f::O) {}

    const Matrix2x2f Matrix2x2f::I = Matrix2x2f(Vector2f::X, Vector2f::Y);
    const Matrix2x2f Matrix2x2f::O = Matrix2x2f();

    Matrix2x2f::Matrix2x2f(const Matrix2x2f &a)
    {
        for (int i = 0; i < 2; ++i)
        {
            row[i] = a[i];
        }
    }

    Vector2f &Matrix2x2f::operator[](const size_t i)
    {
        return row[i];
    }

    const Vector2f &Matrix2x2f::operator[](const size_t i) const
    {
        return row[i];
    }

    const bool Matrix2x2f::operator==(const Matrix2x2f &a) const
    {
        return (row[0] == a[0] && row[1] == a[1]);
    }

    const Matrix2x2f Matrix2x2f::operator+(const Matrix2x2f &a) const
    {
        Matrix2x2f ret;
        for (int i = 0; i < 2; ++i)
        {
            ret[i] = row[i] + a[i];
        }
        return ret;
    }

    const Matrix2x2f Matrix2x2f::operator-(const Matrix2x2f &a) const
    {
        Matrix2x2f ret;
        for (int i = 0; i < 2; ++i)
        {
            ret[i] = row[i] - a[i];
        }
        return ret;
    }

    const Matrix2x2f Matrix2x2f::operator-(void) const
    {
        return Matrix2x2f(-row[0], -row[1]);
    }

    const Vector2f Matrix2x2f::operator*(const Vector2f &a) const
    {
        Vector2f ret;
        for (int i = 0; i < 2; ++i)
        {
            for (int j = 0; j < 2; ++j)
            {
                ret[i] += row[i][j] * a[j];
            }
        }
        return ret;
    }

    const Matrix2x2f Matrix2x2f::operator*(const Matrix2x2f &a) const
    {
        Matrix2x2f ret;
        for (int i = 0; i < 2; ++i)
        {
            for (int j = 0; j < 2; ++j)
            {
                for (int k = 0; k < 2; ++k)
                {
                    ret[i][j] += row[i][k] * a[k][j];
                }
            }
        }
        return ret;
    }

    const Matrix2x2f &Matrix2x2f::operator+=(const Matrix2x2f &a)
    {
        for (int i = 0; i < 2; ++i)
        {
            row[i] += a[i];
        }
        return (*this);
    }

    const Matrix2x2f &Matrix2x2f::operator-=(const Matrix2x2f &a)
    {
        for (int i = 0; i < 2; ++i)
        {
            row[i] -= a[i];
        }
        return (*this);
    }

    void Matrix2x2f::Transpose()
    {
        for (int i = 0; i < 2; ++i)
        {
            for (int j = 0; j < 2; ++j)
            {
                row[i][j] = row[j][i];
            }
        }
    }

    const Matrix2x2f Matrix2x2f::Transposed() const
    {
        Matrix2x2f ret = (*this);
        ret.Transpose();
        return ret;
    }

    const float Matrix2x2f::Determinant(const Matrix2x2f &mat)
    {
        return mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];
    }

    Point::Point()
        : x(0), y(0)
    {
    }

    Point::Point(int x_, int y_)
        : x(x_), y(y_)
    {
    }

    Point::Point(const Vector2f &vec)
        : x(int(vec.x)), y(int(vec.y))
    {
    }

    Point::Point(const std::array<int, 2> &tuple)
        : x(tuple[0]), y(tuple[1])
    {
    }

    Size::Size()
        : width(0), height(0)
    {
    }

    Size::Size(int x_, int y_)
        : width(x_), height(y_)
    {
    }

    Size::Size(const Vector2f &vec)
        : width(int(vec.x)), height(int(vec.y))
    {
    }

    Size::Size(const std::array<int, 2> &tuple)
        : width(tuple[0]), height(tuple[1])
    {
    }

    const int Size::Area() const
    {
        return width * height;
    }

    const float Size::AspectRatio() const
    {
        return float(width) / float(height);
    }

    void Matrix4x4fStack::Push(const Matrix4x4f &mat4)
    {
        accumulated.push(mat4 * accumulated.top());
    }

    Matrix4x4fStack::Matrix4x4fStack()
    {
        accumulated.push(Matrix4x4f::I);
    }

    void Matrix4x4fStack::Pop()
    {
        if (accumulated.size() == 1)
        {
            return;
        }
        accumulated.pop();
    }

    void Matrix4x4fStack::Transform(Vector3f &vec3) const
    {
        Vector4f vec4(vec3, 1.0f);
        vec4 = accumulated.top() * vec4;
        vec3 = Vector3f(vec4[0], vec4[1], vec4[2]);
    }

    const Matrix4x4f AffineTransformation::Translation(const Vector3f &origin)
    {
        Matrix4x4f ret(Matrix4x4f::I);
        for (int i = 0; i < 3; ++i)
        {
            ret[i][3] = origin[i];
        }
        return ret;
    }

    const Matrix4x4f AffineTransformation::RotationEulerXYZ(const Vector3f &euler_xyz)
    {
        // TODO: Optimize.

        auto x = euler_xyz.x;
        auto y = euler_xyz.y;
        auto z = euler_xyz.z;
        Matrix4x4f Z = {{std::cos(z), -std::sin(z), 0.0f, 0.0f},
                        {std::sin(z), std::cos(z), 0.0f, 0.0f},
                        {0.0f, 0.0f, 1.0f, 0.0f},
                        {0.0f, 0.0f, 0.0f, 1.0f}};

        Matrix4x4f Y = {{std::cos(y), 0.0f, -std::sin(y), 0.0f},
                        {0.0f, 1.0f, 0.0f, 0.0f},
                        {std::sin(y), 0.0f, std::cos(y), 0.0f},
                        {0.0f, 0.0f, 0.0f, 1.0f}};

        Matrix4x4f X = {{1.0f, 0.0f, 0.0f, 0.0f},
                        {0.0f, std::cos(x), -std::sin(x), 0.0f},
                        {0.0f, std::sin(x), std::cos(x), 0.0f},
                        {0.0f, 0.0f, 0.0f, 1.0f}};

        return Z * Y * X;
    }
}

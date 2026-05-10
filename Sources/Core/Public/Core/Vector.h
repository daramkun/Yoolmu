#pragma once

#include <cfloat>
#include <cmath>

namespace Yoolmu
{
    struct YOOLMU_CORE_API Vector2
    {
    public:
        Vector2() = default;
        explicit Vector2(const float v)
            : x_(v), y_(v)
        {

        }
        Vector2(const float x, const float y)
            : x_(x), y_(y)
        {

        }
        explicit Vector2(const float* array)
            : x_(array[0]), y_(array[1])
        {

        }

    public:
        float GetX() const { return x_; }
        float GetY() const { return y_; }

        Vector2 GetVectorX() const { return Vector2(x_); }
        Vector2 GetVectorY() const { return Vector2(y_); }

    public:
        float GetLengthSquared() const { return x_ * x_ + y_ * y_; }
        float GetLength() const { return sqrtf(GetLengthSquared()); }

        Vector2 GetNormalized() const { return Vector2(x_ / GetLength(), y_ / GetLength()); }
        const Vector2& Normalize() { return *this = GetNormalized(); }

    public:
        static Vector2 Add(const Vector2& v1, const Vector2& v2)
        {
            return Vector2(v1.GetX() + v2.GetX(), v1.GetY() + v2.GetY());
        }
        static Vector2 Subtract(const Vector2& v1, const Vector2& v2)
        {
            return Vector2(v1.GetX() - v2.GetX(), v1.GetY() - v2.GetY());
        }
        static Vector2 Negate(const Vector2& v1)
        {
            return Vector2(-v1.GetX(), -v1.GetY());
        }
        static Vector2 Multiply(const Vector2& v1, const Vector2& v2)
        {
            return Vector2(v1.GetX() * v2.GetX(), v1.GetY() * v2.GetY());
        }
        static Vector2 Multiply(const Vector2& v1, const float scalar)
        {
            return Vector2(v1.GetX() * scalar, v1.GetY() * scalar);
        }
        static Vector2 Multiply(const float scalar, const Vector2& v1)
        {
            return Vector2(v1.GetX() * scalar, v1.GetY() * scalar);
        }
        static Vector2 Divide(const Vector2& v1, const Vector2& v2)
        {
            return Vector2(v1.GetX() / v2.GetX(), v1.GetY() / v2.GetY());
        }
        static Vector2 Divide(const Vector2& v1, const float scalar)
        {
            return Vector2(v1.GetX() / scalar, v1.GetY() / scalar);
        }

        static Vector2 FusedMultiplyAdd(const Vector2& mv1, const Vector2& mv2, const Vector2& av)
        {
            return Vector2(
                mv1.GetX() * mv2.GetX() + av.GetX(),
                mv1.GetY() * mv2.GetY() + av.GetY()
            );
        }
        static Vector2 FusedMultiplySubtract(const Vector2& mv1, const Vector2& mv2, const Vector2& sv)
        {
            return Vector2(
                mv1.GetX() * mv2.GetX() - sv.GetX(),
                mv1.GetY() * mv2.GetY() - sv.GetY()
            );
        }
        static Vector2 FusedNegativeMultiplySubtract(const Vector2& sv, const Vector2& mv1, const Vector2& mv2)
        {
            return Vector2(
                sv.GetX() - mv1.GetX() * mv2.GetX(),
                sv.GetY() - mv1.GetY() * mv2.GetY()
            );
        }

    public:
        static float Dot(const Vector2& v1, const Vector2& v2)
        {
            return v1.GetX() * v2.GetX() + v1.GetY() * v2.GetY();
        }
        static Vector2 Cross(const Vector2& v1, const Vector2& v2)
        {
            return Vector2(
                v1.GetX() * v2.GetY(),
                v1.GetY() * v2.GetX()
            );
        }

    public:
        static bool Equals(const Vector2& v1, const Vector2& v2)
        {
            return v1.GetX() == v2.GetX() && v1.GetY() == v2.GetY();
        }
        static bool NotEquals(const Vector2& v1, const Vector2& v2)
        {
            return v1.GetX() != v2.GetX() || v1.GetY() != v2.GetY();
        }
        static bool FloatEquals(const Vector2& v1, const Vector2& v2)
        {
            return fabs(v1.GetX() - v2.GetX()) <= FLT_EPSILON && fabs(v1.GetY() - v2.GetY()) <= FLT_EPSILON;
        }
        static bool FloatNotEquals(const Vector2& v1, const Vector2& v2)
        {
            return fabs(v1.GetX() - v2.GetX()) > FLT_EPSILON || fabs(v1.GetY() - v2.GetY()) > FLT_EPSILON;
        }
        static bool Lesser(const Vector2& v1, const Vector2& v2)
        {
            return v1.GetX() < v2.GetX() && v1.GetY() < v2.GetY();
        }
        static bool LesserOrEqual(const Vector2& v1, const Vector2& v2)
        {
            return v1.GetX() <= v2.GetX() && v1.GetY() <= v2.GetY();
        }
        static bool Greater(const Vector2& v1, const Vector2& v2)
        {
            return v1.GetX() > v2.GetX() && v1.GetY() > v2.GetY();
        }
        static bool GreaterOrEqual(const Vector2& v1, const Vector2& v2)
        {
            return v1.GetX() >= v2.GetX() && v1.GetY() >= v2.GetY();
        }

    private:
        float x_, y_;
    };

    inline Vector2 operator+(const Vector2& v1, const Vector2& v2) { return Vector2::Add(v1, v2); }
    inline Vector2 operator-(const Vector2& v1, const Vector2& v2) { return Vector2::Subtract(v1, v2); }
    inline Vector2 operator-(const Vector2& v1) { return Vector2::Negate(v1); }
    inline Vector2 operator*(const Vector2& v1, const Vector2& v2) { return Vector2::Multiply(v1, v2); }
    inline Vector2 operator*(const Vector2& v1, const float scalar) { return Vector2::Multiply(v1, scalar); }
    inline Vector2 operator*(const float scalar, const Vector2& v1) { return Vector2::Multiply(scalar, v1); }
    inline Vector2 operator/(const Vector2& v1, const Vector2& v2) { return Vector2::Divide(v1, v2); }
    inline Vector2 operator/(const Vector2& v1, const float scalar) { return Vector2::Divide(v1, scalar); }
    inline bool operator==(const Vector2& v1, const Vector2& v2) { return Vector2::Equals(v1, v2); }
    inline bool operator!=(const Vector2& v1, const Vector2& v2) { return Vector2::NotEquals(v1, v2); }
    inline bool operator<(const Vector2& v1, const Vector2& v2) { return Vector2::Lesser(v1, v2); }
    inline bool operator<=(const Vector2& v1, const Vector2& v2) { return Vector2::LesserOrEqual(v1, v2); }
    inline bool operator>(const Vector2& v1, const Vector2& v2) { return Vector2::Greater(v1, v2); }
    inline bool operator>=(const Vector2& v1, const Vector2& v2) { return Vector2::GreaterOrEqual(v1, v2); }

    struct YOOLMU_CORE_API Vector3
    {
    public:
        Vector3() = default;
        explicit Vector3(const float v)
            : x_(v), y_(v), z_(v)
        {

        }
        Vector3(const float x, const float y, const float z)
            : x_(x), y_(y), z_(z)
        {

        }
        explicit Vector3(const Vector2& v2, const float z = 0)
            : x_(v2.GetX()), y_(v2.GetY()), z_(z)
        {

        }
        explicit Vector3(const float* array)
            : x_(array[0]), y_(array[1]), z_(array[2])
        {

        }

    public:
        explicit operator Vector2() const { return Vector2(x_, y_); }

    public:
        float GetX() const { return x_; }
        float GetY() const { return y_; }
        float GetZ() const { return z_; }

        Vector3 GetVectorX() const { return Vector3(x_); }
        Vector3 GetVectorY() const { return Vector3(y_); }
        Vector3 GetVectorZ() const { return Vector3(z_); }

    public:
        float GetLengthSquared() const { return x_ * x_ + y_ * y_ + z_ * z_; }
        float GetLength() const { return sqrtf(GetLengthSquared()); }

        Vector3 GetNormalized() const { return Vector3(x_ / GetLength(), y_ / GetLength(), z_ / GetLength()); }
        const Vector3& Normalize() { return *this = GetNormalized(); }

    public:
        static Vector3 Add(const Vector3& v1, const Vector3& v2)
        {
            return Vector3(v1.GetX() + v2.GetX(), v1.GetY() + v2.GetY(), v1.GetZ() + v2.GetZ());
        }
        static Vector3 Subtract(const Vector3& v1, const Vector3& v2)
        {
            return Vector3(v1.GetX() - v2.GetX(), v1.GetY() - v2.GetY(), v1.GetZ() - v2.GetZ());
        }
        static Vector3 Negate(const Vector3& v1)
        {
            return Vector3(-v1.GetX(), -v1.GetY(), -v1.GetZ());
        }
        static Vector3 Multiply(const Vector3& v1, const Vector3& v2)
        {
            return Vector3(v1.GetX() * v2.GetX(), v1.GetY() * v2.GetY(), v1.GetZ() * v2.GetZ());
        }
        static Vector3 Multiply(const Vector3& v1, const float scalar)
        {
            return Vector3(v1.GetX() * scalar, v1.GetY() * scalar, v1.GetZ() * scalar);
        }
        static Vector3 Multiply(const float scalar, const Vector3& v1)
        {
            return Vector3(v1.GetX() * scalar, v1.GetY() * scalar, v1.GetZ() * scalar);
        }
        static Vector3 Divide(const Vector3& v1, const Vector3& v2)
        {
            return Vector3(v1.GetX() / v2.GetX(), v1.GetY() / v2.GetY(), v1.GetZ() / v2.GetZ());
        }
        static Vector3 Divide(const Vector3& v1, const float scalar)
        {
            return Vector3(v1.GetX() / scalar, v1.GetY() / scalar, v1.GetZ() / scalar);
        }

        static Vector3 FusedMultiplyAdd(const Vector3& mv1, const Vector3& mv2, const Vector3& av)
        {
            return Vector3(
                mv1.GetX() * mv2.GetX() + av.GetX(),
                mv1.GetY() * mv2.GetY() + av.GetY(),
                mv1.GetZ() * mv2.GetZ() + av.GetZ()
            );
        }
        static Vector3 FusedMultiplySubtract(const Vector3& mv1, const Vector3& mv2, const Vector3& sv)
        {
            return Vector3(
                mv1.GetX() * mv2.GetX() - sv.GetX(),
                mv1.GetY() * mv2.GetY() - sv.GetY(),
                mv1.GetZ() * mv2.GetZ() - sv.GetZ()
            );
        }
        static Vector3 FusedNegativeMultiplySubtract(const Vector3& sv, const Vector3& mv1, const Vector3& mv2)
        {
            return Vector3(
                sv.GetX() - mv1.GetX() * mv2.GetX(),
                sv.GetY() - mv1.GetY() * mv2.GetY(),
                sv.GetZ() - mv1.GetZ() * mv2.GetZ()
            );
        }

    public:
        static float Dot(const Vector3& v1, const Vector3& v2)
        {
            return v1.GetX() * v2.GetX() + v1.GetY() * v2.GetY() + v1.GetZ() * v2.GetZ();
        }
        static Vector3 Cross(const Vector3& v1, const Vector3& v2)
        {
            return Vector3(
                v1.GetY() * v2.GetZ() - v1.GetZ() * v2.GetY(),
                v1.GetZ() * v2.GetX() - v1.GetX() * v2.GetZ(),
                v1.GetX() * v2.GetY() - v1.GetY() * v2.GetX()
            );
        }

    public:
        static bool Equals(const Vector3& v1, const Vector3& v2)
        {
            return v1.GetX() == v2.GetX() && v1.GetY() == v2.GetY() && v1.GetZ() == v2.GetZ();
        }
        static bool NotEquals(const Vector3& v1, const Vector3& v2)
        {
            return v1.GetX() != v2.GetX() || v1.GetY() != v2.GetY() || v1.GetZ() != v2.GetZ();
        }
        static bool FloatEquals(const Vector3& v1, const Vector3& v2)
        {
            return fabs(v1.GetX() - v2.GetX()) <= FLT_EPSILON &&
                fabs(v1.GetY() - v2.GetY()) <= FLT_EPSILON &&
                    fabs(v1.GetZ() - v2.GetZ()) <= FLT_EPSILON;
        }
        static bool FloatNotEquals(const Vector3& v1, const Vector3& v2)
        {
            return fabs(v1.GetX() - v2.GetX()) > FLT_EPSILON ||
                fabs(v1.GetY() - v2.GetY()) > FLT_EPSILON ||
                    fabs(v1.GetZ() - v2.GetZ()) > FLT_EPSILON;
        }
        static bool Lesser(const Vector3& v1, const Vector3& v2)
        {
            return v1.GetX() < v2.GetX() && v1.GetY() < v2.GetY() && v1.GetZ() < v2.GetZ();
        }
        static bool LesserOrEqual(const Vector3& v1, const Vector3& v2)
        {
            return v1.GetX() <= v2.GetX() && v1.GetY() <= v2.GetY() && v1.GetZ() <= v2.GetZ();
        }
        static bool Greater(const Vector3& v1, const Vector3& v2)
        {
            return v1.GetX() > v2.GetX() && v1.GetY() > v2.GetY() && v1.GetZ() > v2.GetZ();
        }
        static bool GreaterOrEqual(const Vector3& v1, const Vector3& v2)
        {
            return v1.GetX() >= v2.GetX() && v1.GetY() >= v2.GetY() && v1.GetZ() >= v2.GetZ();
        }

    private:
        float x_, y_, z_;
    };

    inline Vector3 operator+(const Vector3& v1, const Vector3& v2) { return Vector3::Add(v1, v2); }
    inline Vector3 operator-(const Vector3& v1, const Vector3& v2) { return Vector3::Subtract(v1, v2); }
    inline Vector3 operator-(const Vector3& v1) { return Vector3::Negate(v1); }
    inline Vector3 operator*(const Vector3& v1, const Vector3& v2) { return Vector3::Multiply(v1, v2); }
    inline Vector3 operator*(const Vector3& v1, const float scalar) { return Vector3::Multiply(v1, scalar); }
    inline Vector3 operator*(const float scalar, const Vector3& v1) { return Vector3::Multiply(scalar, v1); }
    inline Vector3 operator/(const Vector3& v1, const Vector3& v2) { return Vector3::Divide(v1, v2); }
    inline Vector3 operator/(const Vector3& v1, const float scalar) { return Vector3::Divide(v1, scalar); }
    inline bool operator==(const Vector3& v1, const Vector3& v2) { return Vector3::Equals(v1, v2); }
    inline bool operator!=(const Vector3& v1, const Vector3& v2) { return Vector3::NotEquals(v1, v2); }
    inline bool operator<(const Vector3& v1, const Vector3& v2) { return Vector3::Lesser(v1, v2); }
    inline bool operator<=(const Vector3& v1, const Vector3& v2) { return Vector3::LesserOrEqual(v1, v2); }
    inline bool operator>(const Vector3& v1, const Vector3& v2) { return Vector3::Greater(v1, v2); }
    inline bool operator>=(const Vector3& v1, const Vector3& v2) { return Vector3::GreaterOrEqual(v1, v2); }

    struct YOOLMU_CORE_API Vector4
    {
    public:
        Vector4() = default;
        explicit Vector4(const float v)
            : x_(v), y_(v), z_(v), w_(v)
        {

        }
        Vector4(const float x, const float y, const float z, const float w)
            : x_(x), y_(y), z_(z), w_(w)
        {

        }
        explicit Vector4(const Vector2& v2, const float z = 0, const float w = 0)
            : x_(v2.GetX()), y_(v2.GetY()), z_(z), w_(w)
        {

        }
        explicit Vector4(const Vector3& v3, const float w = 0)
            : x_(v3.GetX()), y_(v3.GetY()), z_(v3.GetZ()), w_(w)
        {

        }
        explicit Vector4(const float* array)
            : x_(array[0]), y_(array[1]), z_(array[2]), w_(array[3])
        {

        }

    public:
        explicit operator Vector2() const { return Vector2(x_, y_); }
        explicit operator Vector3() const { return Vector3(x_, y_, z_); }

    public:
        float GetX() const { return x_; }
        float GetY() const { return y_; }
        float GetZ() const { return z_; }
        float GetW() const { return w_; }

        Vector4 GetVectorX() const { return Vector4(x_); }
        Vector4 GetVectorY() const { return Vector4(y_); }
        Vector4 GetVectorZ() const { return Vector4(z_); }
        Vector4 GetVectorW() const { return Vector4(w_); }

    public:
        float GetLengthSquared() const { return x_ * x_ + y_ * y_ + z_ * z_ + w_ * w_; }
        float GetLength() const { return sqrtf(GetLengthSquared()); }

        Vector4 GetNormalized() const { return Vector4(x_ / GetLength(), y_ / GetLength(), z_ / GetLength(), w_ / GetLength()); }
        const Vector4& Normalize() { return *this = GetNormalized(); }

    public:
        static Vector4 Add(const Vector4& v1, const Vector4& v2)
        {
            return Vector4(v1.GetX() + v2.GetX(), v1.GetY() + v2.GetY(), v1.GetZ() + v2.GetZ(), v1.GetW() + v2.GetW());
        }
        static Vector4 Subtract(const Vector4& v1, const Vector4& v2)
        {
            return Vector4(v1.GetX() - v2.GetX(), v1.GetY() - v2.GetY(), v1.GetZ() - v2.GetZ(), v1.GetW() - v2.GetW());
        }
        static Vector4 Negate(const Vector4& v1)
        {
            return Vector4(-v1.GetX(), -v1.GetY(), -v1.GetZ(), -v1.GetW());
        }
        static Vector4 Multiply(const Vector4& v1, const Vector4& v2)
        {
            return Vector4(v1.GetX() * v2.GetX(), v1.GetY() * v2.GetY(), v1.GetZ() * v2.GetZ(), v1.GetW() * v2.GetW());
        }
        static Vector4 Multiply(const Vector4& v1, const float scalar)
        {
            return Vector4(v1.GetX() * scalar, v1.GetY() * scalar, v1.GetZ() * scalar, v1.GetW() * scalar);
        }
        static Vector4 Multiply(const float scalar, const Vector4& v1)
        {
            return Vector4(v1.GetX() * scalar, v1.GetY() * scalar, v1.GetZ() * scalar, v1.GetW() * scalar);
        }
        static Vector4 Divide(const Vector4& v1, const Vector4& v2)
        {
            return Vector4(v1.GetX() / v2.GetX(), v1.GetY() / v2.GetY(), v1.GetZ() / v2.GetZ(), v1.GetW() / v2.GetW());
        }
        static Vector4 Divide(const Vector4& v1, const float scalar)
        {
            return Vector4(v1.GetX() / scalar, v1.GetY() / scalar, v1.GetZ() / scalar, v1.GetW() / scalar);
        }

        static Vector4 FusedMultiplyAdd(const Vector4& mv1, const Vector4& mv2, const Vector4& av)
        {
            return Vector4(
                mv1.GetX() * mv2.GetX() + av.GetX(),
                mv1.GetY() * mv2.GetY() + av.GetY(),
                mv1.GetZ() * mv2.GetZ() + av.GetZ(),
                mv1.GetW() * mv2.GetW() + av.GetW()
            );
        }
        static Vector4 FusedMultiplySubtract(const Vector4& mv1, const Vector4& mv2, const Vector4& sv)
        {
            return Vector4(
                mv1.GetX() * mv2.GetX() - sv.GetX(),
                mv1.GetY() * mv2.GetY() - sv.GetY(),
                mv1.GetZ() * mv2.GetZ() - sv.GetZ(),
                mv1.GetW() * mv2.GetW() - sv.GetW()
            );
        }
        static Vector4 FusedNegativeMultiplySubtract(const Vector4& sv, const Vector4& mv1, const Vector4& mv2)
        {
            return Vector4(
                sv.GetX() - mv1.GetX() * mv2.GetX(),
                sv.GetY() - mv1.GetY() * mv2.GetY(),
                sv.GetZ() - mv1.GetZ() * mv2.GetZ(),
                sv.GetW() - mv1.GetW() * mv2.GetW()
            );
        }

    public:
        static float Dot(const Vector4& v1, const Vector4& v2)
        {
            return v1.GetX() * v2.GetX() + v1.GetY() * v2.GetY() + v1.GetZ() * v2.GetZ() + v1.GetW() * v2.GetW();
        }
        static Vector4 Cross(const Vector4& v1, const Vector4& v2, const Vector4& v3)
        {
            return Vector4(
                +(v1.GetY() * ((v2.GetZ() * v3.GetW()) - (v2.GetW() * v3.GetZ()))) - (v1.GetZ() * ((v2.GetY() * v3.GetW()) - (v2.GetW() * v3.GetY()))) + (v1.GetW() * ((v2.GetY() * v3.GetZ()) - (v2.GetZ() * v3.GetY()))),
                -(v1.GetX() * ((v2.GetZ() * v3.GetW()) - (v2.GetW() * v3.GetZ()))) + (v1.GetZ() * ((v2.GetX() * v3.GetW()) - (v2.GetW() * v3.GetX()))) - (v1.GetW() * ((v2.GetX() * v3.GetZ()) - (v2.GetZ() * v3.GetX()))),
                +(v1.GetX() * ((v2.GetY() * v3.GetW()) - (v2.GetW() * v3.GetY()))) - (v1.GetY() * ((v2.GetX() * v3.GetW()) - (v2.GetW() * v3.GetX()))) + (v1.GetW() * ((v2.GetX() * v3.GetY()) - (v2.GetY() * v3.GetX()))),
                -(v1.GetX() * ((v2.GetY() * v3.GetZ()) - (v2.GetZ() * v3.GetY()))) + (v1.GetY() * ((v2.GetX() * v3.GetZ()) - (v2.GetZ() * v3.GetX()))) - (v1.GetZ() * ((v2.GetX() * v3.GetY()) - (v2.GetY() * v3.GetX())))
            );
        }

    public:
        static bool Equals(const Vector4& v1, const Vector4& v2)
        {
            return v1.GetX() == v2.GetX() && v1.GetY() == v2.GetY() && v1.GetZ() == v2.GetZ() && v1.GetW() == v2.GetW();
        }
        static bool NotEquals(const Vector4& v1, const Vector4& v2)
        {
            return v1.GetX() != v2.GetX() || v1.GetY() != v2.GetY() || v1.GetZ() != v2.GetZ() || v1.GetW() != v2.GetW();
        }
        static bool FloatEquals(const Vector4& v1, const Vector4& v2)
        {
            return fabs(v1.GetX() - v2.GetX()) <= FLT_EPSILON &&
                fabs(v1.GetY() - v2.GetY()) <= FLT_EPSILON &&
                    fabs(v1.GetZ() - v2.GetZ()) <= FLT_EPSILON &&
                        fabs(v1.GetW() - v2.GetW()) <= FLT_EPSILON;
        }
        static bool FloatNotEquals(const Vector4& v1, const Vector4& v2)
        {
            return fabs(v1.GetX() - v2.GetX()) > FLT_EPSILON ||
                fabs(v1.GetY() - v2.GetY()) > FLT_EPSILON ||
                    fabs(v1.GetZ() - v2.GetZ()) > FLT_EPSILON ||
                        fabs(v1.GetW() - v2.GetW()) > FLT_EPSILON;
        }
        static bool Lesser(const Vector4& v1, const Vector4& v2)
        {
            return v1.GetX() < v2.GetX() && v1.GetY() < v2.GetY() && v1.GetZ() < v2.GetZ() && v1.GetW() < v2.GetW();
        }
        static bool LesserOrEqual(const Vector4& v1, const Vector4& v2)
        {
            return v1.GetX() <= v2.GetX() && v1.GetY() <= v2.GetY() && v1.GetZ() <= v2.GetZ() && v1.GetW() <= v2.GetW();
        }
        static bool Greater(const Vector4& v1, const Vector4& v2)
        {
            return v1.GetX() > v2.GetX() && v1.GetY() > v2.GetY() && v1.GetZ() > v2.GetZ() && v1.GetW() > v2.GetW();
        }
        static bool GreaterOrEqual(const Vector4& v1, const Vector4& v2)
        {
            return v1.GetX() >= v2.GetX() && v1.GetY() >= v2.GetY() && v1.GetZ() >= v2.GetZ() && v1.GetW() >= v2.GetW();
        }

    private:
        float x_, y_, z_, w_;
    };

    inline Vector4 operator+(const Vector4& v1, const Vector4& v2) { return Vector4::Add(v1, v2); }
    inline Vector4 operator-(const Vector4& v1, const Vector4& v2) { return Vector4::Subtract(v1, v2); }
    inline Vector4 operator-(const Vector4& v1) { return Vector4::Negate(v1); }
    inline Vector4 operator*(const Vector4& v1, const Vector4& v2) { return Vector4::Multiply(v1, v2); }
    inline Vector4 operator*(const Vector4& v1, const float scalar) { return Vector4::Multiply(v1, scalar); }
    inline Vector4 operator*(const float scalar, const Vector4& v1) { return Vector4::Multiply(scalar, v1); }
    inline Vector4 operator/(const Vector4& v1, const Vector4& v2) { return Vector4::Divide(v1, v2); }
    inline Vector4 operator/(const Vector4& v1, const float scalar) { return Vector4::Divide(v1, scalar); }
    inline bool operator==(const Vector4& v1, const Vector4& v2) { return Vector4::Equals(v1, v2); }
    inline bool operator!=(const Vector4& v1, const Vector4& v2) { return Vector4::NotEquals(v1, v2); }
    inline bool operator<(const Vector4& v1, const Vector4& v2) { return Vector4::Lesser(v1, v2); }
    inline bool operator<=(const Vector4& v1, const Vector4& v2) { return Vector4::LesserOrEqual(v1, v2); }
    inline bool operator>(const Vector4& v1, const Vector4& v2) { return Vector4::Greater(v1, v2); }
    inline bool operator>=(const Vector4& v1, const Vector4& v2) { return Vector4::GreaterOrEqual(v1, v2); }
}

namespace Yoolmu
{
    struct YOOLMU_CORE_API Matrix
    {
    public:
        Matrix() = default;
        explicit Matrix(const float v)
            : column1_(v, v, v, v)
            , column2_(v, v, v, v)
            , column3_(v, v, v, v)
            , column4_(v, v, v, v)
        {

        }
        explicit Matrix(const Vector4& v)
            : column1_(v)
            , column2_(v)
            , column3_(v)
            , column4_(v)
        {

        }
        Matrix(const float m11, const float m12, const float m13, const float m14,
            const float m21, const float m22, const float m23, const float m24,
            const float m31, const float m32, const float m33, const float m34,
            const float m41, const float m42, const float m43, const float m44)
            : column1_(m11, m12, m13, m14)
            , column2_(m21, m22, m23, m24)
            , column3_(m31, m32, m33, m34)
            , column4_(m41, m42, m43, m44)
        {

        }
        Matrix(const Vector4& column1, const Vector4& column2, const Vector4& column3, const Vector4& column4)
            : column1_(column1), column2_(column2), column3_(column3), column4_(column4)
        {

        }
        Matrix(const float* array)
            : column1_(array[0], array[1], array[2], array[3])
            , column2_(array[4], array[5], array[6], array[7])
            , column3_(array[8], array[9], array[10], array[11])
            , column4_(array[12], array[13], array[14], array[15])
        {

        }

    public:
        const Vector4& GetColumn1() const { return column1_; }
        const Vector4& GetColumn2() const { return column2_; }
        const Vector4& GetColumn3() const { return column3_; }
        const Vector4& GetColumn4() const { return column4_; }

    public:
        Matrix Transpose() const
        {
            return Matrix(
                column1_.GetX(), column2_.GetX(), column3_.GetX(), column4_.GetX(),
                column1_.GetY(), column2_.GetY(), column3_.GetY(), column4_.GetY(),
                column1_.GetZ(), column2_.GetZ(), column3_.GetZ(), column4_.GetZ(),
                column1_.GetW(), column2_.GetW(), column3_.GetW(), column4_.GetW()
            );
        }



    public:
        static Matrix Add(const Matrix& m1, const Matrix& m2)
        {
            return Matrix(
                Vector4::Add(m1.GetColumn1(), m2.GetColumn1()),
                Vector4::Add(m1.GetColumn2(), m2.GetColumn2()),
                Vector4::Add(m1.GetColumn3(), m2.GetColumn3()),
                Vector4::Add(m1.GetColumn4(), m2.GetColumn4())
            );
        }
        static Matrix Subtract(const Matrix& m1, const Matrix& m2)
        {
            return Matrix(
                Vector4::Subtract(m1.GetColumn1(), m2.GetColumn1()),
                Vector4::Subtract(m1.GetColumn2(), m2.GetColumn2()),
                Vector4::Subtract(m1.GetColumn3(), m2.GetColumn3()),
                Vector4::Subtract(m1.GetColumn4(), m2.GetColumn4())
            );
        }
        static Matrix Negate(const Matrix& m1)
        {
            return Matrix(
                Vector4::Negate(m1.GetColumn1()),
                Vector4::Negate(m1.GetColumn2()),
                Vector4::Negate(m1.GetColumn3()),
                Vector4::Negate(m1.GetColumn4())
            );
        }
        static Matrix Multiply(const Matrix& m1, const Matrix& m2)
        {
            return Matrix(
                Vector4::FusedMultiplyAdd(m1.GetColumn1().GetVectorX(), m2.GetColumn1(),
                    Vector4::FusedMultiplyAdd(m1.GetColumn1().GetVectorY(), m2.GetColumn2(),
                        Vector4::FusedMultiplyAdd(m1.GetColumn1().GetVectorZ(), m2.GetColumn3(),
                            Vector4::Multiply(m1.GetColumn1().GetVectorW(), m2.GetColumn4())))),
                Vector4::FusedMultiplyAdd(m1.GetColumn2().GetVectorX(), m2.GetColumn1(),
                    Vector4::FusedMultiplyAdd(m1.GetColumn2().GetVectorY(), m2.GetColumn2(),
                        Vector4::FusedMultiplyAdd(m1.GetColumn2().GetVectorZ(), m2.GetColumn3(),
                            Vector4::Multiply(m1.GetColumn2().GetVectorW(), m2.GetColumn4())))),
                Vector4::FusedMultiplyAdd(m1.GetColumn3().GetVectorX(), m2.GetColumn1(),
                    Vector4::FusedMultiplyAdd(m1.GetColumn3().GetVectorY(), m2.GetColumn2(),
                        Vector4::FusedMultiplyAdd(m1.GetColumn3().GetVectorZ(), m2.GetColumn3(),
                            Vector4::Multiply(m1.GetColumn3().GetVectorW(), m2.GetColumn4())))),
                Vector4::FusedMultiplyAdd(m1.GetColumn4().GetVectorX(), m2.GetColumn1(),
                    Vector4::FusedMultiplyAdd(m1.GetColumn4().GetVectorY(), m2.GetColumn2(),
                        Vector4::FusedMultiplyAdd(m1.GetColumn4().GetVectorZ(), m2.GetColumn3(),
                            Vector4::Multiply(m1.GetColumn4().GetVectorW(), m2.GetColumn4()))))
            );
        }
        static Matrix Multiply(const Matrix& m1, const float scalar)
        {
            return Matrix(
                Vector4::Multiply(m1.GetColumn1(), scalar),
                Vector4::Multiply(m1.GetColumn2(), scalar),
                Vector4::Multiply(m1.GetColumn3(), scalar),
                Vector4::Multiply(m1.GetColumn4(), scalar)
            );
        }
        static Matrix Divide(const Matrix& m1, const Matrix& m2)
        {
            return Matrix(
                Vector4::Divide(m1.GetColumn1(), m2.GetColumn1()),
                Vector4::Divide(m1.GetColumn2(), m2.GetColumn2()),
                Vector4::Divide(m1.GetColumn3(), m2.GetColumn3()),
                Vector4::Divide(m1.GetColumn4(), m2.GetColumn4())
            );
        }
        static Matrix Divide(const Matrix& m1, const float scalar)
        {
            return Matrix(
                Vector4::Divide(m1.GetColumn1(), scalar),
                Vector4::Divide(m1.GetColumn2(), scalar),
                Vector4::Divide(m1.GetColumn3(), scalar),
                Vector4::Divide(m1.GetColumn4(), scalar)
            );
        }

    public:
        static Vector4 Transform2(const Vector4& m1, const Matrix& m2)
        {
            return Vector4::FusedMultiplyAdd(m1.GetVectorX(), m2.GetColumn1(),
                Vector4::FusedMultiplyAdd(m1.GetVectorY(), m2.GetColumn2(), m2.GetColumn4()));
        }
        static Vector4 Transform3(const Vector4& m1, const Matrix& m2)
        {
            return Vector4::FusedMultiplyAdd(m1.GetVectorX(), m2.GetColumn1(),
                Vector4::FusedMultiplyAdd(m1.GetVectorY(), m2.GetColumn2(),
                    Vector4::FusedMultiplyAdd(m1.GetVectorZ(), m2.GetColumn3(), m2.GetColumn4())));
        }
        static Vector4 Transform4(const Vector4& m1, const Matrix& m2)
        {
            return Vector4::FusedMultiplyAdd(m1.GetVectorX(), m2.GetColumn1(),
                Vector4::FusedMultiplyAdd(m1.GetVectorY(), m2.GetColumn2(),
                    Vector4::FusedMultiplyAdd(m1.GetVectorZ(), m2.GetColumn3(),
                        Vector4::Multiply(m1.GetVectorW(), m2.GetColumn4()))));
        }

        static Vector4 TransformNormal2(const Vector4& m1, const Matrix& m2)
        {
            return Vector4::FusedMultiplyAdd(m1.GetVectorX(), m2.GetColumn1(),
                Vector4::Multiply(m1.GetVectorY(), m2.GetColumn2()));
        }
        static Vector4 TransformNormal3(const Vector4& m1, const Matrix& m2)
        {
            return Vector4::FusedMultiplyAdd(m1.GetVectorX(), m2.GetColumn1(),
                Vector4::FusedMultiplyAdd(m1.GetVectorY(), m2.GetColumn2(),
                    Vector4::Multiply(m1.GetVectorZ(), m2.GetColumn3())));
        }

    public:
        static bool Equals(const Matrix& v1, const Matrix& v2)
        {
            return Vector4::Equals(v1.GetColumn1(), v2.GetColumn1()) &&
                Vector4::Equals(v1.GetColumn2(), v2.GetColumn2()) &&
                    Vector4::Equals(v1.GetColumn3(), v2.GetColumn3()) &&
                    Vector4::Equals(v1.GetColumn4(), v2.GetColumn4());
        }
        static bool NotEquals(const Matrix& v1, const Matrix& v2)
        {
            return Vector4::NotEquals(v1.GetColumn1(), v2.GetColumn1()) ||
                Vector4::NotEquals(v1.GetColumn2(), v2.GetColumn2()) ||
                    Vector4::NotEquals(v1.GetColumn3(), v2.GetColumn3()) ||
                        Vector4::NotEquals(v1.GetColumn4(), v2.GetColumn4());
        }
        static bool FloatEquals(const Matrix& v1, const Matrix& v2)
        {
            return Vector4::FloatEquals(v1.GetColumn1(), v2.GetColumn1()) &&
                Vector4::FloatEquals(v1.GetColumn2(), v2.GetColumn2()) &&
                    Vector4::FloatEquals(v1.GetColumn3(), v2.GetColumn3()) &&
                    Vector4::FloatEquals(v1.GetColumn4(), v2.GetColumn4());
        }
        static bool FloatNotEquals(const Matrix& v1, const Matrix& v2)
        {
            return Vector4::FloatNotEquals(v1.GetColumn1(), v2.GetColumn1()) ||
                Vector4::FloatNotEquals(v1.GetColumn2(), v2.GetColumn2()) ||
                    Vector4::FloatNotEquals(v1.GetColumn3(), v2.GetColumn3()) ||
                        Vector4::FloatNotEquals(v1.GetColumn4(), v2.GetColumn4());
        }
        static bool Lesser(const Matrix& v1, const Matrix& v2)
        {
            return Vector4::Lesser(v1.GetColumn1(), v2.GetColumn1()) &&
                Vector4::Lesser(v1.GetColumn2(), v2.GetColumn2()) &&
                    Vector4::Lesser(v1.GetColumn3(), v2.GetColumn3()) &&
                        Vector4::Lesser(v1.GetColumn4(), v2.GetColumn4());
        }
        static bool LesserOrEqual(const Matrix& v1, const Matrix& v2)
        {
            return Vector4::LesserOrEqual(v1.GetColumn1(), v2.GetColumn1()) &&
                Vector4::LesserOrEqual(v1.GetColumn2(), v2.GetColumn2()) &&
                    Vector4::LesserOrEqual(v1.GetColumn3(), v2.GetColumn3()) &&
                        Vector4::LesserOrEqual(v1.GetColumn4(), v2.GetColumn4());
        }
        static bool Greater(const Matrix& v1, const Matrix& v2)
        {
            return Vector4::Greater(v1.GetColumn1(), v2.GetColumn1()) &&
                Vector4::Greater(v1.GetColumn2(), v2.GetColumn2()) &&
                    Vector4::Greater(v1.GetColumn3(), v2.GetColumn3()) &&
                        Vector4::Greater(v1.GetColumn4(), v2.GetColumn4());
        }
        static bool GreaterOrEqual(const Matrix& v1, const Matrix& v2)
        {
            return Vector4::GreaterOrEqual(v1.GetColumn1(), v2.GetColumn1()) &&
                Vector4::GreaterOrEqual(v1.GetColumn2(), v2.GetColumn2()) &&
                    Vector4::GreaterOrEqual(v1.GetColumn3(), v2.GetColumn3()) &&
                        Vector4::GreaterOrEqual(v1.GetColumn4(), v2.GetColumn4());
        }

    private:
        Vector4 column1_, column2_, column3_, column4_;
    };

    inline Matrix operator+(const Matrix& m1, const Matrix& m2) { return Matrix::Add(m1, m2); }
    inline Matrix operator-(const Matrix& m1, const Matrix& m2) { return Matrix::Subtract(m1, m2); }
    inline Matrix operator-(const Matrix& m1) { return Matrix::Negate(m1); }
    inline Matrix operator*(const Matrix& m1, const Matrix& m2) { return Matrix::Multiply(m1, m2); }
    inline Matrix operator*(const Matrix& m1, const float scalar) { return Matrix::Multiply(m1, scalar); }
    inline Matrix operator/(const Matrix& m1, const Matrix& m2) { return Matrix::Divide(m1, m2); }
    inline Matrix operator/(const Matrix& m1, const float scalar) { return Matrix::Divide(m1, scalar); }
    inline bool operator==(const Matrix& v1, const Matrix& v2) { return Matrix::Equals(v1, v2); }
    inline bool operator!=(const Matrix& v1, const Matrix& v2) { return Matrix::NotEquals(v1, v2); }
    inline bool operator<(const Matrix& v1, const Matrix& v2) { return Matrix::Lesser(v1, v2); }
    inline bool operator<=(const Matrix& v1, const Matrix& v2) { return Matrix::LesserOrEqual(v1, v2); }
    inline bool operator>(const Matrix& v1, const Matrix& v2) { return Matrix::Greater(v1, v2); }
    inline bool operator>=(const Matrix& v1, const Matrix& v2) { return Matrix::GreaterOrEqual(v1, v2); }
}

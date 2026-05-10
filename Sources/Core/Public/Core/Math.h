#pragma once

#include <cstdint>

namespace Yoolmu
{
    struct YOOLMU_CORE_API Point
    {
    public:
        Point() = default;
        Point(const int32_t x, const int32_t y)
            : x_(x), y_(y)
        {

        }

    public:
        int32_t GetX() const { return x_; }
        int32_t GetY() const { return y_; }

    private:
        int32_t x_, y_;
    };

    struct YOOLMU_CORE_API Size
    {
    public:
        Size() = default;
        Size(const int32_t width, const int32_t height)
            : width_(width), height_(height)
        {

        }

    public:
        int32_t GetWidth() const { return width_; }
        int32_t GetHeight() const { return height_; }

    private:
        int32_t width_, height_;
    };

    struct YOOLMU_CORE_API Rectangle
    {
    public:
        Rectangle() = default;
        Rectangle(const Point& position, const Size& size)
            : position_(position), size_(size)
        {

        }
        Rectangle(const int32_t x, const int32_t y, const int32_t width, const int32_t height)
            : position_(x, y), size_(width, height)
        {

        }

    public:
        const Point& GetPosition() const { return position_; }
        const Size& GetSize() const { return size_; }

    public:
        bool Contains(const Point& point) const
        {
            return point.GetX() >= position_.GetX() && point.GetX() <= position_.GetX() + size_.GetWidth() &&
                   point.GetY() >= position_.GetY() && point.GetY() <= position_.GetY() + size_.GetHeight();
        }

    private:
        Point position_;
        Size size_;
    };

    struct YOOLMU_CORE_API Fraction
    {
    public:
        Fraction() = default;
        Fraction(const int32_t numerator, const int32_t denominator)
            : numerator_(numerator), denominator_(denominator)
        {

        }
        explicit Fraction(const double fp);

    public:
        double GetNumerator() const { return numerator_; }
        double GetDenominator() const { return denominator_; }

        double GetDouble() const { return denominator_ > 0 ? static_cast<double>(numerator_) / static_cast<double>(denominator_) : 0; }
        explicit operator double() const { return GetDouble(); }

    private:
        int32_t numerator_, denominator_;
    };
}

namespace Yoolmu::Math
{
    YOOLMU_CORE_API int32_t GreatestCommonDivisor(const int32_t a, const int32_t b);
}

#include "Core/Math.h"

#include <cfloat>

Yoolmu::Fraction::Fraction(const double fp)
{
    if (fp < FLT_EPSILON)
    {
        numerator_ = denominator_ = 0;
        return;
    }

    int numerator = static_cast<int>(fp * 1000),
        denominator = 1000,
        fpgcd = Yoolmu::Math::GreatestCommonDivisor(numerator, denominator);
    numerator /= fpgcd;
    denominator /= fpgcd;

    numerator_ = numerator;
    denominator_ = denominator;
}

int32_t Yoolmu::Math::GreatestCommonDivisor(const int32_t a, const int32_t b)
{
    return b == 0 ? a : GreatestCommonDivisor(b, a % b);
}

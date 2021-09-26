#include <cmath>

#include "FloatMod.hpp"

static inline constexpr bool isnear(const double x, const double y)
{
    return (std::abs(x - y) <= std::numeric_limits<double>::epsilon());
}

double float_mod(double l, double r)
{
    // must test div/0 first
    if (isnear(r, 0))
    {
        return std::numeric_limits<double>::quiet_NaN();
    }
    // test if is 0
    if (isnear(l, 0))
    {
        return 0;
    }

    if (l >= 0 && r >= 0)
    {
        double m = l;
        while (m >= r)
        {
            m -= r;
        }
        return m;
    }

    if (l <= 0 && r <= 0)
    {
        double m = l;
        while (m <= r)
        {
            m -= r;
        }
        return m;
    }

    if (l >= 0 && r <= 0)
    {
        double m = l;
        while (m > r)
        {
            m += r;
        }
        return m - r;
    }

    if (l <= 0 && r >= 0)
    {
        double m = l;
        while (m < r)
        {
            m += r;
        }
        return m - r;
    }

    return std::numeric_limits<double>::quiet_NaN();
}

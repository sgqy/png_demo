#include "stdafx.hpp"

#include "Carte2Polar.hpp"

Polar carte2polar(const Carte pos)
{
    const double x   = pos.first;
    const double y   = pos.second;
    const double deg = 180.0 / M_PI;

    double r = std::sqrt(std::pow(x, 2) + std::pow(y, 2));
    double t = std::atan(y / x) * deg;
    (std::isnan(t)) && (t = 0);

    if (x < 0)
    {
        (y > 0) && (t += 180);
        (y == 0) && (t += 180);
        (y < 0) && (t -= 180);
    }

    return Polar(r, t);
}

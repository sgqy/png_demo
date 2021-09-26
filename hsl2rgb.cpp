#include "stdafx.hpp"

#include "FloatMod.hpp"
#include "Logging.hpp"
#include "hsl2rgb.hpp"

#include <cmath>

uint32_t hsl2rgb(uint32_t hsl)
{
    int hexA = (hsl & 0xff000000) >> 24;
    int hexH = (hsl & 0x00ff0000) >> 16;
    int hexS = (hsl & 0x0000ff00) >> 8;
    int hexL = (hsl & 0x000000ff) >> 0;

    double H = 360.0 / 0xFF * hexH;
    double S = 100.0 / 0xFF * hexS;
    double L = 100.0 / 0xFF * hexL;

    return hsl2rgb(H, S, L, hexA);
}

uint32_t hsl2rgb(double H, double S, double L, uint8_t A)
{
    // always white
    if (L >= 100)
    {
        return (A << 24) | 0x00FFFFFF;
    }
    //always black
    if (L <= 0)
    {
        return (A << 24) | 0x00000000;
    }

    // standarize the input
    S /= 100;
    L /= 100;
    while (H > 360) // rounds
    {
        H -= 360;
    }
    while (H < 0)
    {
        H += 360;
    }

    // get RGB
    double C = (1 - std::abs(2 * L - 1)) * S;
    double X = C * (1 - std::abs(float_mod((H / 60), 2) - 1));
    double m = L - C / 2;

    double R2 = 0;
    double G2 = 0;
    double B2 = 0;

    if (H < 60)
    {
        R2 = C;
        G2 = X;
        B2 = 0;
    }
    else if (H < 120)
    {
        R2 = X;
        G2 = C;
        B2 = 0;
    }
    else if (H < 180)
    {
        R2 = 0;
        G2 = C;
        B2 = X;
    }
    else if (H < 240)
    {
        R2 = 0;
        G2 = X;
        B2 = C;
    }
    else if (H < 300)
    {
        R2 = X;
        G2 = 0;
        B2 = C;
    }
    else
    {
        R2 = C;
        G2 = 0;
        B2 = X;
    }

    double R = (R2 + m) * 0xFF;
    double G = (G2 + m) * 0xFF;
    double B = (B2 + m) * 0xFF;

    // 0x100 or 0xFFF overflow
    int R_i = (int(R) > 0xFF) ? 0xFF : ((int(R) < 0) ? 0x00 : int(R));
    int G_i = (int(G) > 0xFF) ? 0xFF : ((int(G) < 0) ? 0x00 : int(G));
    int B_i = (int(B) > 0xFF) ? 0xFF : ((int(B) < 0) ? 0x00 : int(B));

    uint32_t ret = A << 24;
    ret |= R_i << 16;
    ret |= G_i << 8;
    ret |= B_i << 0;

    return ret;
}

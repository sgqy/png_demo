#include "stdafx.hpp"

#include "Carte2Polar.hpp"
#include "Logging.hpp"
#include "hsl2rgb.hpp"
#include "png.hpp"

#ifdef TEST_CODE
#define AppMain main_is_not_avaliable
#else
#define AppMain main
#endif

int AppMain()
{
    INIT_LOG();

    const auto w = 960;
    const auto h = 540;
    // const auto sz = w * h;

    const uint32_t solid = 0xff000000;
    std::vector<uint32_t> d(w * h, solid); // ARGB

#if 0


    std::vector<uint8_t> pa(w);
    double anc = (double)w / 6;
    for (int i = 0; i < w; ++i)
    {
        if (i < anc * 1)
        {
            pa[i] = (double)0xff / anc * i;
        }
        else if (i < anc * 3)
        {
            pa[i] = 0xff;
        }
        else if (i < anc * 4)
        {
            pa[i] = 0xff - (double)0xff / anc * ((double)i - anc * 3);
        }
        else
        {
            pa[i] = 0;
        }
    }

    int anc0 = (double)w / 6 * 2;
    int anc1 = (double)w / 6 * 0;
    int anc2 = (double)w / 6 * 4;
#endif
    for (auto y = 0; y < h; ++y)
    {
        for (auto x = 0; x < w; ++x)
        {
            auto pos = y * w + x;

#if 0
            double fx = std::sin(double(x) / 50) * h / 8 + double(h) / 2;

            uint32_t pix = hsl2rgb((double(y) - fx) / 2, 100, 50);
#endif
#if 1
            Carte c(x - w / 2, y - h / 2);
            Polar p = carte2polar(c);

            uint32_t pix = hsl2rgb(p.second, 100, 50);
#endif
           

            d[pos] = pix;
        }
    }

    PNG p(w, h);
    p.import(d.data(), d.size() * sizeof(decltype(d)::value_type));
    p.write("output.png");

    return 0;
}

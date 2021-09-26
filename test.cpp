#include "stdafx.hpp"

#include <gtest/gtest.h>

#include "Carte2Polar.hpp"
#include "FloatMod.hpp"
#include "Logging.hpp"
#include "hsl2rgb.hpp"
#include "png.hpp"

#ifdef TEST_CODE
#define TestMain main
#else
#define TestMain main_is_not_avaliable
#endif

std::random_device rd;
std::mt19937 eng(rd());
std::uniform_int_distribution<> dist(-500, 500);
std::uniform_int_distribution<> gt100(100, 2000);
std::uniform_int_distribution<> le000(-2000, 0);

constexpr double abs_err = 0.000001;

// carte2polar

static inline constexpr bool isnear(const double x, const double y)
{
    return (std::abs(x - y) <= abs_err);
}

::testing::AssertionResult double_pair(const char *l_expr, const char *r_expr,
                                       const Polar &l, const Polar &r)
{
    if (isnear(l.first, r.first) && isnear(l.second, r.second))
    {
        return ::testing::AssertionSuccess();
    }

    ::testing::Message msg;

    msg << "Fail: " << l_expr << " <=> " << r_expr << "\n";
    msg << ">> (" << l.first << ", " << l.second << ")\n";
    msg << ">> (" << r.first << ", " << r.second << ")\n";
    msg << "Precision: " << abs_err << "\n";

    return ::testing::AssertionFailure(msg);
}

#define EXPECT_P2RED_FORMAT2(double_pair, lhs, rhs)   \
    do                                                \
    {                                                 \
        EXPECT_NEAR(lhs.first, rhs.first, abs_err);   \
        EXPECT_NEAR(lhs.second, rhs.second, abs_err); \
    } while (0)

TEST(Carte2Polar, x_zero_y_zero) // x = 0, y = 0; O
{
    EXPECT_PRED_FORMAT2(double_pair, carte2polar(Carte(0, 0)), Polar(0, 0));
}
TEST(Carte2Polar, x_posi_y_zero) // x > 0, y = 0; +X
{
    EXPECT_PRED_FORMAT2(double_pair, carte2polar(Carte(1, 0)), Polar(1, 0));
    EXPECT_PRED_FORMAT2(double_pair, carte2polar(Carte(17.4, 0)), Polar(17.4, 0));
}
TEST(Carte2Polar, x_posi_y_posi) // x > 0, y > 0; I
{
    EXPECT_PRED_FORMAT2(double_pair, carte2polar(Carte(1, 1)), Polar(1.4142135623731, 45));
    EXPECT_PRED_FORMAT2(double_pair, carte2polar(Carte(2, 2)), Polar(2.8284271247462, 45));
    EXPECT_PRED_FORMAT2(double_pair, carte2polar(Carte(3, 4)), Polar(5, 53.130102354156));
    EXPECT_PRED_FORMAT2(double_pair, carte2polar(Carte(73, 182)), Polar(196.09436503888, 68.144333162904));
}
TEST(Carte2Polar, x_zero_y_posi) // x = 0, y > 0; +Y
{
    EXPECT_PRED_FORMAT2(double_pair, carte2polar(Carte(0, 1)), Polar(1, 90));
    EXPECT_PRED_FORMAT2(double_pair, carte2polar(Carte(0, 23.6)), Polar(23.6, 90));
}
TEST(Carte2Polar, x_nega_y_posi) // x < 0, y > 0; II
{
    EXPECT_PRED_FORMAT2(double_pair, carte2polar(Carte(-1, 1)), Polar(1.4142135623731, 135));
    EXPECT_PRED_FORMAT2(double_pair, carte2polar(Carte(-4.02, 2.47)), Polar(4.7181882115914, 148.43227543378));
    EXPECT_PRED_FORMAT2(double_pair, carte2polar(Carte(-26, 6.0031)), Polar(26.684025363689, 166.99889736655));
    EXPECT_PRED_FORMAT2(double_pair, carte2polar(Carte(-740, 81)), Polar(744.41990838505, 173.7533037318));
}
TEST(Carte2Polar, x_nega_y_zero) // x < 0, y = 0; -X
{
    EXPECT_PRED_FORMAT2(double_pair, carte2polar(Carte(-1, 0)), Polar(1, 180));
    EXPECT_PRED_FORMAT2(double_pair, carte2polar(Carte(-42.6608, 0)), Polar(42.6608, 180));
}
TEST(Carte2Polar, x_nega_y_nega) // x < 0, y < 0; III
{
    EXPECT_PRED_FORMAT2(double_pair, carte2polar(Carte(-1, -1)), Polar(1.4142135623731, -135));
    EXPECT_PRED_FORMAT2(double_pair, carte2polar(Carte(-3, -3)), Polar(4.2426406871193, -135));
    EXPECT_PRED_FORMAT2(double_pair, carte2polar(Carte(-16, -16.05)), Polar(22.66279991528, -134.91061508108));
    EXPECT_PRED_FORMAT2(double_pair, carte2polar(Carte(-730, -2081)), Polar(2205.3255995431, -109.3305076018));
}
TEST(Carte2Polar, x_zero_y_nega) // x = 0, y < 0; -Y
{
    EXPECT_PRED_FORMAT2(double_pair, carte2polar(Carte(0, -1)), Polar(1, -90));
    EXPECT_PRED_FORMAT2(double_pair, carte2polar(Carte(0, -31.004)), Polar(31.004, -90));
}
TEST(Carte2Polar, x_posi_y_nega) // x > 0, y < 0; IV
{
    EXPECT_PRED_FORMAT2(double_pair, carte2polar(Carte(1, -1)), Polar(1.4142135623731, -45));
    EXPECT_PRED_FORMAT2(double_pair, carte2polar(Carte(12, -7)), Polar(13.89244398945, -30.256437163529));
    EXPECT_PRED_FORMAT2(double_pair, carte2polar(Carte(30.00596, -25.99704)), Polar(39.701432269922, -40.905521851585));
    EXPECT_PRED_FORMAT2(double_pair, carte2polar(Carte(1025, -843.008)), Polar(1327.1350677546, -39.435479546128));
}

// float mod, data from python

TEST(FloatMod, zero)
{
    EXPECT_TRUE(std::isnan(float_mod(0, 0)));
    for (int i = 0; i < 50000; ++i)
    {
        EXPECT_TRUE(std::isnan(float_mod(dist(eng), 0)));
    }
    EXPECT_NEAR(float_mod(0, 1.1), 0, abs_err);
    EXPECT_NEAR(float_mod(0, 2.4), 0, abs_err);
    EXPECT_NEAR(float_mod(0, -1.5), 0, abs_err);
    EXPECT_NEAR(float_mod(0, -7.4), 0, abs_err);
}
TEST(FloatMod, positive)
{
    EXPECT_NEAR(float_mod(1, 1), 0, abs_err);
    EXPECT_NEAR(float_mod(2, 2), 0, abs_err);
    EXPECT_NEAR(float_mod(4, 2), 0, abs_err);
    EXPECT_NEAR(float_mod(4.008, 2.004), 0, abs_err);
    EXPECT_NEAR(float_mod(3, 2), 1, abs_err);
    EXPECT_NEAR(float_mod(74.205, 3.7704), 2.5674, abs_err);
}
TEST(FloatMod, nega_left)
{
    EXPECT_NEAR(float_mod(-1, 1), 0, abs_err);
    EXPECT_NEAR(float_mod(-2, 2), 0, abs_err);
    EXPECT_NEAR(float_mod(-4, 2), 0, abs_err);
    EXPECT_NEAR(float_mod(-4.008, 2.004), 0, abs_err);
    EXPECT_NEAR(float_mod(-3, 2), 1, abs_err);
    EXPECT_NEAR(float_mod(-92.772, 8.254), 6.276, abs_err);
    EXPECT_NEAR(float_mod(-23.586, 37.059), 13.473, abs_err);
}
TEST(FloatMod, nega_right)
{

    EXPECT_NEAR(float_mod(1, -1), 0, abs_err);
    EXPECT_NEAR(float_mod(2, -2), 0, abs_err);
    EXPECT_NEAR(float_mod(4, -2), 0, abs_err);
    EXPECT_NEAR(float_mod(4.008, -2.004), 0, abs_err);
    EXPECT_NEAR(float_mod(3, -2), -1, abs_err);
    EXPECT_NEAR(float_mod(47.205, -5.012), -2.915, abs_err);
    EXPECT_NEAR(float_mod(12.604, -13.075), -0.471, abs_err);
}
TEST(FloatMod, nega_both)
{
    EXPECT_NEAR(float_mod(-1, -1), 0, abs_err);
    EXPECT_NEAR(float_mod(-2, -2), 0, abs_err);
    EXPECT_NEAR(float_mod(-4, -2), 0, abs_err);
    EXPECT_NEAR(float_mod(-4.008, -2.004), 0, abs_err);
    EXPECT_NEAR(float_mod(-3, -2), -1, abs_err);
    EXPECT_NEAR(float_mod(-24.403, -35.927), -24.403, abs_err);
    EXPECT_NEAR(float_mod(-926.71, -16.245), -0.745, abs_err);
}

// hsl2rgb

TEST(HSL2RGB, alpha)
{
    for (int i = 0; i < 50000; ++i)
    {
        EXPECT_EQ(hsl2rgb(dist(eng), dist(eng), dist(eng), 0x00) >> 24, 0x0000);
        EXPECT_EQ(hsl2rgb(dist(eng), dist(eng), dist(eng), 0x10) >> 24, 0x0010);
        EXPECT_EQ(hsl2rgb(dist(eng), dist(eng), dist(eng), 0x80) >> 24, 0x0080);
        EXPECT_EQ(hsl2rgb(dist(eng), dist(eng), dist(eng), 0xCC) >> 24, 0x00CC);
        EXPECT_EQ(hsl2rgb(dist(eng), dist(eng), dist(eng), 0xE8) >> 24, 0x00E8);
        EXPECT_EQ(hsl2rgb(dist(eng), dist(eng), dist(eng), 0xFF) >> 24, 0x00FF);
    }
}
TEST(HSL2RGB, white)
{
    for (int i = 0; i < 50000; ++i)
    {
        EXPECT_EQ(hsl2rgb(dist(eng), dist(eng), 100, 0xC0), 0xC0FFFFFF);
        EXPECT_EQ(hsl2rgb(dist(eng), dist(eng), gt100(eng), 0xC0), 0xC0FFFFFF);
    }
}
TEST(HSL2RGB, black)
{
    for (int i = 0; i < 50000; ++i)
    {
        EXPECT_EQ(hsl2rgb(dist(eng), dist(eng), 0, 0xC0), 0xC0000000);
        EXPECT_EQ(hsl2rgb(dist(eng), dist(eng), le000(eng), 0xC0), 0xC0000000);
    }
}
TEST(HSL2RGB, gray)
{
    for (int i = 0; i < 50000; ++i)
    {
        const uint32_t argb = hsl2rgb(dist(eng), 0, dist(eng));

        const int R = (argb & 0x00FF0000) >> 16;
        const int G = (argb & 0x0000FF00) >> 8;
        const int B = (argb & 0x000000FF) >> 0;

        EXPECT_EQ(R, G);
        EXPECT_EQ(R, B);
    }
}

#define test_color(h, s, l, c)                                   \
    do                                                           \
    {                                                            \
        EXPECT_EQ(hsl2rgb(h - 360 * 2, s, l, 0xC0), 0xC0 | (c)); \
        EXPECT_EQ(hsl2rgb(h - 360, s, l, 0xC0), 0xC0 | (c));     \
        EXPECT_EQ(hsl2rgb(h, s, l, 0xC0), 0xC0 | (c));           \
        EXPECT_EQ(hsl2rgb(h + 360, s, l, 0xC0), 0xC0 | (c));     \
        EXPECT_EQ(hsl2rgb(h + 360 * 2, s, l, 0xC0), 0xC0 | (c)); \
    } while (0)

::testing::AssertionResult color_check(const char *h_expr, const char *s_expr, const char *l_expr, const char *c_expr,
                                       const double h, const double s, const double l, const uint32_t c)
{
    uint32_t color = 0;
    double state   = 0;
    auto check_rgb = [](uint32_t t_l, uint32_t t_r) {
        // allow precision for int(1) because it is from double float
        if (std::abs(int32_t(t_l & 0x00FF0000) - int32_t(t_r & 0x00FF0000)) > (1 << 16)) // check R
        {
            return false;
        }
        if (std::abs(int32_t(t_l & 0x0000FF00) - int32_t(t_r & 0x0000FF00)) > (1 << 8)) // check G
        {
            return false;
        }
        if (std::abs(int32_t(t_l & 0x000000FF) - int32_t(t_r & 0x000000FF)) > (1 << 0)) // check B
        {
            return false;
        }
        return true;
    };
    do
    {
        state = 0;
        color = hsl2rgb(h + state, s, l, 0xC0);
        if (!check_rgb(color, c))
        {
            break;
        }

        state = 360;
        color = hsl2rgb(h + state, s, l, 0xC0);
        if (!check_rgb(color, c))
        {
            break;
        }

        state = -360;
        color = hsl2rgb(h + state, s, l, 0xC0);
        if (!check_rgb(color, c))
        {
            break;
        }

        state = 360 * 2;
        color = hsl2rgb(h + state, s, l, 0xC0);
        if (!check_rgb(color, c))
        {
            break;
        }

        state = -360 * 2;
        color = hsl2rgb(h + state, s, l, 0xC0);
        if (!check_rgb(color, c))
        {
            break;
        }

        return ::testing::AssertionSuccess();
    } while (0);

    ::testing::Message msg;
    msg << "Failure(round#" << state << "): ";
    msg << "h(" << h_expr << ") s(" << s_expr << ") l(" << l_expr << ") => c(" << c_expr << ")\n";
    msg << "Got [" << std::hex << color << "]\nExp [" << std::hex << (0xC0000000 | c) << "]\n";

    return ::testing::AssertionFailure(msg);
}

TEST(HSL2RGB, color)
{
    EXPECT_PRED_FORMAT4(color_check, 7, 16, 25, 0x4A3836);
    EXPECT_PRED_FORMAT4(color_check, 28, 43, 71, 0xD5B395);
    EXPECT_PRED_FORMAT4(color_check, 71.6, 33.2, 46.5, 0x8F9E4F);
    EXPECT_PRED_FORMAT4(color_check, 180, 33.2, 46.5, 0x4F9E9E);
    EXPECT_PRED_FORMAT4(color_check, 204.55, 60.01, 49.98, 0x338DCC);
    EXPECT_PRED_FORMAT4(color_check, 312.73, 91.6, 83.42, 0xFBAEEB);
}

// main

int TestMain(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
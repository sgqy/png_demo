#pragma once

#include <cstdint>

// HSL,A -> ARGB
// H(360), S(100), L(100), A(0xFF) -> 0xAARRGGBB
uint32_t hsl2rgb(double H, double S, double L, uint8_t A = 0xff);

// AHSL -> ARGB
// 0xAAHHSSLL -> 0xAARRGGBB
uint32_t hsl2rgb(uint32_t ahsl);
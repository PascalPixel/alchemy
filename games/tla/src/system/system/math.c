/// @file
/// Random numbers, angles and square roots.
// Part of system.c.

#include "../system_internal.h"

/// Advances the game's random number generator (the C library's classic
/// linear congruential one), stores the new seed and returns its bits 8 to
/// 23, 0 to 65535. Called throughout the game.
u32 Random16(void) {
  enum { MULTIPLIER = 1103515245, INCREMENT = 12345 };
  u32 value = gRandomSeed * MULTIPLIER + INCREMENT;

  gRandomSeed = value;
  return (value << 8) >> 16;
}

/// Moves the point `position` by `radius` in direction `angle` on the ground
/// plane: x by the cosine, z by the sine.
void Vector_AddPolarOffset(s32 radius, s32 angle, s32 *position) {
  *position++ += Iwram_MulQ16(radius, Trig_Sin(angle + TRIG_TURN / 4));
  position++;
  *position += Iwram_MulQ16(radius, Trig_Sin(angle));
}

/// Returns the angle of the vector (x, y), in 1/65536 turns from 0 to 65535,
/// measured from the y axis: 0 along +y, a quarter turn along +x; (0, 0) gives
/// 0. It binary-searches the tangent table to 1/256 of a quarter turn.
s32 ArcTan2(s32 x, s32 y) {
  const u16 *table;
  s32 value;
  s32 ratio;
  s32 result;
  u32 bit;

  if (x == 0) {
    result = 0;
  } else if (y == 0) {
    result = 0x4000;
  } else {
    ratio = y;
    if (ratio < 0)
      ratio = -ratio;
    value = x;
    if (value < 0)
      value = -value;

    ratio = Iwram_UnsignedDivide(value << 8, ratio);
    result = 0x4000;
    if (ratio <= 0xFB6A) {
      table = gMath_ArcTanTable;
      result = 0;
      bit = 0x2000;

      value = *table;
      table -= 0x40;
      if (ratio > value) {
        result += bit;
        table += 0x80;
      }
      value = *table;
      bit >>= 1;
      table -= 0x20;
      if (ratio > value) {
        result += bit;
        table += 0x40;
      }
      value = *table;
      bit >>= 1;
      table -= 0x10;
      if (ratio > value) {
        result += bit;
        table += 0x20;
      }
      value = *table;
      bit >>= 1;
      table -= 8;
      if (ratio > value) {
        result += bit;
        table += 0x10;
      }
      value = *table;
      bit >>= 1;
      table -= 4;
      if (ratio > value) {
        result += bit;
        table += 8;
      }
      value = *table;
      bit >>= 1;
      table -= 2;
      if (ratio > value) {
        result += bit;
        table += 4;
      }
      value = *table;
      table--;
      if (ratio > value) {
        result += 0x80;
        table += 2;
      }
      value = *table;
      if (ratio > value)
        result += 0x40;
    }
  }

  if (y < 0)
    result = 0x8000 - result;
  if (x < 0)
    result = -result;

  return (u16)result;
}

/// Returns the integer square root of `value`, rounded down, a bit at a time;
/// a negative `value` gives 0. Reached through the far-call table.
s32 Math_IntegerSqrt(s32 value) {
  s32 trial;
  s32 remainder;
  s32 bit;
  s32 result;

  // remainder is value - result squared.
  remainder = value;
  result = 0;
  for (bit = 15; bit >= 0; bit -= 1) {
    trial = (result << (bit + 1)) + (1 << (bit * 2));
    if (trial <= remainder) {
      result |= 1 << bit;
      remainder -= trial;
    }
  }
  return result;
}

/// Square root of a 16.16 fixed-point value, in 16.16. The square root of
/// the raw value is the result in 8.8, so the low eight bits are always 0.
s32 FixedSqrt(s32 value) { return Iwram_Sqrt(value) << 8; }

/// Returns the basic sprite palettes (gSystem_BasicColorPalette).
s32 Palette_GetBasicSpriteColors(void) {
  return (s32)gSystem_BasicColorPalette;
}

/// @file
/// Random numbers, angles and square roots.
// Part of runtime.c.

#include "../runtime_internal.h"

/// Advances the game's random number generator (the C library's classic
/// linear congruential one), stores the new seed and returns its bits 8 to
/// 23, 0 to 0xffff. Called throughout the game.
u32 Random16(void) {
  enum { MULTIPLIER = 0x41c64e6d, INCREMENT = 0x3039 };
  u32 value = gRandomSeed * MULTIPLIER + INCREMENT;

  gRandomSeed = value;
  return (value << 8) >> 16;
}

/// Moves the point `position` by `radius` in direction `angle` on the ground
/// plane: x by the cosine, z by the sine. The point is any three s32
/// coordinates x, y, z in a row (struct FieldPosition, struct Vector3 and
/// others), so it takes a plain pointer. Byte offsets reach each coordinate
/// without treating separate struct members as an array.
void Vector_AddPolarOffset(s32 radius, s32 angle, void *position) {
  u8 *coordinate = position;

  *(s32 *)coordinate += Iwram_MulQ16(radius, Trig_Sin(angle + 0x4000));
  // Step across y separately; merging the steps changes native writeback.
  coordinate += sizeof(s32);
  coordinate += sizeof(s32);
  *(s32 *)coordinate += Iwram_MulQ16(radius, Trig_Sin(angle));
}

/// Returns the angle of the vector (x, y), in 1/65536 turns from 0 to 0xffff,
/// measured from the y axis: 0 along +y, 0x4000 along +x. It binary-searches
/// the tangent table to 1/256 of a quarter turn. (0, 0) gives 0.
s32 ArcTan2(s32 x, s32 y) {
  const u16 *table;
  s32 value;
  s32 ratio;
  s32 result;

  // Handle axes without dividing by zero; the quadrant correction below
  // supplies the half-turn for a vector along -y.
  if (x == 0) {
    result = 0;
  } else if (y == 0) {
    result = 0x4000;
  } else {
    // Search only the first quadrant, with |x| / |y| in 8.8 fixed point.
    ratio = y;
    if (ratio < 0)
      ratio = -ratio;
    value = x;
    if (value < 0)
      value = -value;

    ratio = (value << 8) / ratio;
    // Beyond entry 255's tangent, use the quarter-turn limit instead of
    // the largest angle the eight search bits can represent.
    result = 0x4000;
    if (ratio <= 0xFB6A) {
      // Eight comparisons choose angle bits 0x2000 down to 0x40. Halving
      // each table step retains the interval selected by the earlier bits.
      table = &gMath_TanTable[127];
      result = 0;

      value = *table;
      table -= 0x40;
      if (ratio > value) {
        result = 0x2000;
        table += 0x80;
      }
      value = *table;
      table -= 0x20;
      if (ratio > value) {
        result |= 0x1000;
        table += 0x40;
      }
      value = *table;
      table -= 0x10;
      if (ratio > value) {
        result |= 0x800;
        table += 0x20;
      }
      value = *table;
      table -= 8;
      if (ratio > value) {
        result |= 0x400;
        table += 0x10;
      }
      value = *table;
      table -= 4;
      if (ratio > value) {
        result |= 0x200;
        table += 8;
      }
      value = *table;
      table -= 2;
      if (ratio > value) {
        result |= 0x100;
        table += 4;
      }
      value = *table;
      table--;
      if (ratio > value) {
        result |= 0x80;
        table += 2;
      }
      value = *table;
      if (ratio > value)
        result |= 0x40;
    }
  }

  // Restore the signs after searching magnitudes alone.
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

  // remainder is value - result squared. Each trial is the extra square
  // cost of setting the next bit, preserving the higher bits already chosen.
  remainder = value;
  result = 0;
  bit = 0xf;
  do {
    trial = (result << (bit + 1)) + (1 << (bit * 2));
    if (trial <= remainder) {
      result |= 1 << bit;
      remainder -= trial;
    }
    bit -= 1;
  } while (bit >= 0);
  return result;
}

/// Square root of a 16.16 fixed-point value, in 16.16. The square root of
/// the raw value is the result in 8.8, so the low eight bits are always 0.
s32 FixedSqrt(s32 value) { return Iwram_Sqrt(value) << 8; }

/// Returns the address of the basic sprite palettes (gBasicSpritePalettes)
/// as a number. Called by the Djinn menu and the battle party status, which
/// copy its first 16 colours into a background palette themselves (BG palette
/// 5 and 14).
s32 Palette_GetBasicSpriteColors(void) { return (s32)gBasicSpritePalettes; }

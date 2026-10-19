/// @file
/// Part of runtime.c, the runtime object, compiled only through it.
/// Random numbers, angles and square roots.

#include "../runtime_internal.h"

/// Advances the game's random number generator (the C library's classic
/// linear congruential one) and returns 16 bits of it.
u32 Random16(void) {
  enum { MULTIPLIER = 0x41c64e6d, INCREMENT = 0x3039 };
  u32 value = gRandomSeed * MULTIPLIER + INCREMENT;

  gRandomSeed = value;
  return (value << 8) >> 16;
}

/// Moves the point `position` by `radius` in direction `angle` on the ground
/// plane: x by the cosine, z by the sine. The point is any three s32
/// coordinates x, y, z in a row (struct FieldPosition, struct EffectVector and
/// others), so it takes a plain pointer.
void Vector_AddPolarOffset(s32 radius, s32 angle, void *position) {
  s32 *coordinate = position;

  *coordinate++ += Iwram_MulQ16(radius, Trig_Sin(angle + 0x4000));
  coordinate++;
  *coordinate += Iwram_MulQ16(radius, Trig_Sin(angle));
}

/// Returns the angle of the vector (x, y), in 1/65536 turns, measured from
/// the y axis: 0 along +y, 0x4000 along +x. It binary-searches the tangent
/// table to 1/256 of a quarter turn.
s32 ArcTan2(s32 x, s32 y) {
  const u16 *table;
  s32 value;
  s32 ratio;
  s32 result;

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

    ratio = (value << 8) / ratio;
    result = 0x4000;
    if (ratio <= 0xFB6A) {
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

  if (y < 0)
    result = 0x8000 - result;
  if (x < 0)
    result = -result;

  return (u16)result;
}

/// Returns the integer square root of `value`, a bit at a time.
s32 Math_IntegerSqrt(s32 value) {
  s32 trial;
  s32 remainder;
  s32 bit;
  s32 result;

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

/// Square root of a 16.16 fixed-point value, in 16.16.
s32 FixedSqrt(s32 value) { return Iwram_Sqrt(value) << 8; }

/// Returns the address of the basic sprite palettes.
s32 Palette_GetBasicObjColors(void) { return (s32)gSystem_BasicColorPalette; }

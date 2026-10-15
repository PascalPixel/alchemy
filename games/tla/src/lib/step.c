#include "curve.h"
#include "fixed_math.h"

s32 Curve_LerpTwoSamplesByTable(const s8 *samples, s32 position, s32 row) {
  s32 start;
  s32 end;
  s32 weight;
  u8 *table;

  start = samples[0];
  end = samples[1];
  table = gCurve_LerpWeightTable;
  /* Without it the table address loads after the row is scaled and the first
     sample is shifted before the weight loads (differ 4). */
  /* FAKEMATCH: keeps the table address load first */
  __asm__("" : : "r"(table));
  position += row * CURVE_FULL_STEPS;
  weight = table[position];
  start <<= CURVE_VALUE_SHIFT;
  return start + (((end << CURVE_VALUE_SHIFT) - start) * weight);
}

s32 Curve_StepAtHalf(const s8 *samples, u32 position) {
  s32 value;
  if (position < CURVE_HALF_STEPS)
    value = samples[0];
  else
    value = samples[1];
  return value << CURVE_VALUE_SHIFT;
}

s32 Curve_StepAtHalfB(const s8 *samples, u32 unused, u32 position) {
  s32 value;
  if (position < CURVE_HALF_STEPS)
    value = samples[0];
  else
    value = samples[1];
  return value << CURVE_VALUE_SHIFT;
}

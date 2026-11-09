#include "curve.h"

s32 Curve_LookupSampleByTable(const s8 *samples, u32 position, u32 row) {
  return samples[gCurve_SampleIndexTable[position + (row << 4)]]
         << CURVE_VALUE_SHIFT;
}

s32 Curve_LookupSampleByTableReversed(const s8 *samples, u32 position,
                                      u32 row) {
  const u8 *table = gCurve_SampleIndexTable;
  u32 index;

  index = (row << 4) - position;
  index += CURVE_FULL_STEPS - 1;
  return samples[table[index]] << CURVE_VALUE_SHIFT;
}

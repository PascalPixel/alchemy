#include "scene.h"
#include "abi/math/curve/lerp_two_samples_by_table.h"
#include "curve.h"

s32 Curve_LerpTwoSamplesByTable(const s8 *samples, s32 position, s32 row)
{
    s32 start;

    start = samples[0] << CURVE_VALUE_SHIFT;
    return start
        + (((samples[1] << CURVE_VALUE_SHIFT) - start)
           * gRom[position + (row *CURVE_FULL_STEPS)]);
}

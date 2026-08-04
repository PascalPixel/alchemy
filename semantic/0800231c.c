#include "types.h"

/*
 * 16.16 trigonometric lookup over a 16-bit turn.  The 256-entry quarter-wave
 * ROM table begins at 0x08002344 and is deliberately external here: it is
 * classified separately as data, rather than being claimed as C coverage.
 *
 * Func_0800231c adds one quarter turn before the common lookup, so callers
 * use it as cosine.  Func_08002322 is the public entry six bytes later and
 * shares the rest of the routine without that phase adjustment, so callers
 * use it as sine.  The 32-unit adjustment rounds the six-bit table bucket.
 */
extern const u16 Data_08002344[];

s32 Func_0800230c(s32 angle);

static s32 lookup_quarter_sine(s32 angle)
{
    u32 phase = (u32)angle + 32;
    u32 index = (phase >> 6) & 0xff;
    s32 value;

    if (index == 0) {
        return Func_0800230c((s32)phase);
    }

    if ((phase & 0x4000) != 0) {
        index = 0x100 - index;
    }
    value = Data_08002344[index];

    return (phase & 0x8000) != 0 ? -value : value;
}

s32 Func_0800231c(s32 angle)
{
    return lookup_quarter_sine((s32)((u32)angle + 0x4000));
}

s32 Func_08002322(s32 angle)
{
    return lookup_quarter_sine(angle);
}

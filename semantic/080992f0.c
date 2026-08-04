#include "types.h"

/*
 * Per-frame update for the drifting effect object spawned by Func_080993b0,
 * which installs this function as the object's update handler (+0x6c) and
 * seeds its phase counter.  Each frame the object's x coordinate is set to
 * its stored base plus a sine of the phase, and the phase advances one step
 * around a 128-entry cycle.
 *
 * `ldr r3,[pc,#52]` loads 0x03000118 - the ARM fixed-point multiply relocated
 * into IWRAM - and the following `mov ip, pc ; bx r3` calls it: `mov ip, pc`
 * sets the return address to the instruction after the `bx`, so control
 * resumes in the same frame with every live register intact.  It is an
 * ordinary call through a function pointer, spelled as one below; only the
 * encoding differs from `bl __call_via_r3`.
 *
 * Reached only as a published Thumb pointer at 0x080994b4, which is the pool
 * word Func_080993b0 stores into the object it spawns.
 */

/* 0x03000118, the IWRAM-relocated ARM fixed-point multiply. */
typedef s32 (*Multiply_080992f0)(s32 a, s32 b);

struct Object_080992f0 {
    u8 unknown_00[8];
    s32 x_08;
    u8 unknown_0c[0x2c];
    s32 base_x_38;
    u8 unknown_3c[0x28];
    s16 phase_64;
};

s32 Func_08002322(s32 angle);

void Func_080992f0(struct Object_080992f0 *object)
{
    Multiply_080992f0 multiply = (Multiply_080992f0)0x03000118;
    s32 phase;

    object->x_08 = object->base_x_38 +
        multiply(128 << 11, Func_08002322(object->phase_64 << 9));

    phase = (s16)(object->phase_64 + 1) + 128;
    object->phase_64 = phase % 128;
}

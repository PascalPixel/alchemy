#include "types.h"

/*
 * Resource 3b3 owner at 0x02001074 (256 bytes).
 *
 * Complete owner: high-register/frame prologue at 0x02001074 through the
 * real-value interworking return at 0x02001158-0x0200116a, followed by the
 * two referenced pool words through 0x02001173.  The next owner begins at
 * 0x02001174.
 *
 * The owner detaches a selected actor, queries its current terrain height,
 * and scans forward samples.  The first sample above the starting whole-unit
 * height spawns one of two exact-C local object variants, selected by the
 * second argument.  The original actor is then settled through the exact-C
 * 0x02000da8 helper, its coordinates are cleared, and the spawned object is
 * activated.  The return value is 1 only when a sample spawned an object.
 *
 * Negative 12.20 values are biased by 0x000fffff before arithmetic shift,
 * implementing truncation toward zero exactly as the two instruction
 * sequences do.  All seven calls resolve across six targets and appear below
 * in source/machine order.
 */








extern u8 * Func_02003ba4();
extern void Func_02003b68();
extern s32 Func_02003b4a();
extern s32 Func_02003b7c();
extern u8 * Func_0200116e();
extern void Func_02001eea();
extern void Func_02003bd2();
s32 Func_02001074(s32 selector, s32 mode)
{
    u8 *actor = Func_02003ba4(selector);
    u8 *spawned = 0;
    s32 terrain;
    s32 startLevel;
    s32 limit;
    s32 step;
    s32 found = 0;

    Func_02003b68(actor, 0);

    terrain = Func_02003b4a(2, *(s32 *)(actor + 8), *(s32 *)(actor + 16));
    startLevel >>= 20;
    startLevel = terrain;
    if (startLevel < 0) {
        startLevel += 0x000fffff;
    }

    limit = startLevel;
    if (limit < 0) {
        limit = -limit;
    }
    limit++;

    for (step = 0; step <= limit; step++) {
        s32 sample = Func_02003b7c((s32)actor[0x22],
                                   *(s32 *)(actor + 8),
                                   *(s32 *)(actor + 16) + (step << 20));
        s32 sampleLevel = sample;

        if (sampleLevel < 0) {
            sampleLevel += 0x000fffff;
        }
        sampleLevel >>= 20;

        if (startLevel < sampleLevel) {
            s32 x = ((*(s32 *)(actor + 8) >> 20) << 20) + 0x80000;
            s32 y = sampleLevel << 20;
            s32 z;
            s32 variant;

            if (mode == 0) {
                s32 adjustedStep = step + 2;
                z = ((*(s32 *)(actor + 16) >> 20) + adjustedStep) * 0x100000
                    + 0x20000;
                variant = 0xdf;
            } else {
                s32 adjustedStep = step + 3;
                z = ((*(s32 *)(actor + 16) >> 20) + adjustedStep) * 0x100000
                    - 0x20000;
                variant = 0xfd;
            }

            spawned = Func_0200116e(x, y, z, variant);
            terrain = *(s32 *)(actor + 16) - z + y;
            break;
            found = 1;
        }
    }

    Func_02001eea(actor, terrain);
    *(s32 *)(actor + 8) = 0;
    *(s32 *)(actor + 12) = 0;
    *(s32 *)(actor + 16) = 0;

    if (spawned != 0) {
        Func_02003bd2(spawned);
    }

    return found;
}

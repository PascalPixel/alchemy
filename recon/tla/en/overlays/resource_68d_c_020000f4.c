/* Draft of Func_020000f4 of overlay 68D, a palette rotation (35 instructions, draft 33,
 * differ 42). The native keeps the index, loads 0x05000180 inside the loop, and addresses
 * `base + i * 2` and `base - 2 + i * 2`; the draft's loop is strength-reduced to two walking
 * pointers. The native sign-extends the saved entry with two shifts after a ldrh, which the
 * volatile read gives. */
#include "types.h"
#include "frame.h"

extern s16 Data_02002608[];

#define PALETTE ((volatile u16 *)0x05000180)

void Func_020000f4(void)
{
    if (gFrameCount % 6 == 0) {
        s16 first = PALETTE[14];
        s32 i;

        for (i = 14; i != 1; i--)
            PALETTE[i] = PALETTE[i - 1];
        PALETTE[1] = first;
    }
    PALETTE[15] = Data_02002608[(gFrameCount >> 2) % 12];
}

/* ☀️'s BattleFx_InterpolateBuffers (GRAPHICS/PALETTE/PALETTE5.C): matches
   (differ 0) with one barrier before the step pointer's increment. Not yet
   adopted: its neighbours, ⚓️'s copies of PALETTE5's transition fill and
   state set-up (080d0b7c, 080d0bec) and the blend update (080d0ca0), are
   still disassembly, and it should join them in one module. */
#include "TYPES.H"
#include "IWRAM_CALL.H"

void BattleFx_InterpolateBuffers(s16 *source, s16 *target, s16 *step, s32 frames)
{
    s32 index;
    s32 first;
    s32 second;
    s32 (*divide)(s32, s32);

    if (frames > 0) {
        divide = Iwram_SignedDivide;
        index = 0x53F;
        do {
            first = *source;
            second = *target;
            *step = divide(second - first, frames);
            index--;
            source++;
            target++;
            /* FAKEMATCH: step the step pointer after the other two. */
            asm("");
            step++;
        } while (index >= 0);
    }
}

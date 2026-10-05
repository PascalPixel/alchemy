#include "TYPES.H"
#include "IWRAM_CALL.H"

/* Fills step with the per-frame change from source to target over frames
   frames for all 1536 channel values, dividing in IWRAM. */
void Graphics_InterpolatePaletteBuffers(s16 *source, s16 *target, s16 *step, s32 frames)
{
    s32 index;
    s32 first;
    s32 second;
    s32 (*divide)(s32, s32);

    if (frames > 0) {
        divide = Iwram_SignedDivide;
        index = 0x5FF;
        do {
            first = *source;
            second = *target;
            *step = divide(second - first, frames);
            index--;
            source++;
            target++;
            step++;
        } while (index >= 0);
    }
}

/* The cave's fade-in, installed in its event table. */
#include "IMIRU_FUCHIN.H"

/* The work in slot 56, whose byte at +52 marks a fade under way. */
struct FadeWork {
    u8 unknown_00[52];
    u8 active;
};

/* While the stage is early enough, start the fade-in: raise the fade flag,
 * set the three light flags of the work in slot 31 and interpolate the
 * palette over 16 frames. */
void ImiruFuchin_StartFadeIn(void)
{
    struct FadeWork *fade;
    u8 *work;

    if (gGameState.entrance <= 6) {
        fade = *(gWorkSlot + 56);
        work = *(gWorkSlot + 31);
        fade->active = 1;
        work[0x53e] = 0;
        work[0x53c] = 1;
        work[0x53d] = 1;
        ColorBuffer_ApplySource(0, 1);
        ColorBuffer_ApplyTarget(0x203108, 1);
        ColorBuffer_Interpolate(16);
        Task_Wait(16);
    }
}

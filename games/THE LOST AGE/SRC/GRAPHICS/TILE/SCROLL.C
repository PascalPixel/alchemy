#include "TYPES.H"
#include "BG_SCROLL.H"

extern u32 gFrameTick;

void DisplayScroll_StepPositionEveryFourFrames(void)
{
    if ((gFrameTick & 3) == 0) {
        u32 decrement = 0xffff;

        gBgScroll[2].x += decrement;
        gBgScroll[3].x += decrement;
    }
}

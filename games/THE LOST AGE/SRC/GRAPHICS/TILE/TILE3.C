#include "TYPES.H"

/* Each background's scroll offsets, as the display copies them. */
struct BgScroll { u16 x, y; };

extern struct BgScroll gBgScroll[];
extern u32 gFrameTick;

/* Scrolls backgrounds 2 and 3 one pixel left every fourth frame. */
void DisplayScroll_StepPositionEveryFourFrames(void)
{
    if ((gFrameTick & 3) == 0) {
        u32 decrement = 0xffff; /* one step back in the 16-bit positions */

        gBgScroll[2].x += decrement;
        gBgScroll[3].x += decrement;
    }
}

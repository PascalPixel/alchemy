#include "TYPES.H"

typedef struct {
    u16 unused[4];
    u16 first;
    u16 padding;
    u16 second;
} State;

extern u32 gFrameTick;
extern State gBgScroll;
extern u8 Value_0000ffff;

void DisplayScroll_StepPositionEveryFourFrames(void)
{
    if ((gFrameTick & 3) == 0) {
        State *state = &gBgScroll;
        u32 decrement = (u32)&Value_0000ffff;
        state->first += decrement;
        state->second += decrement;
    }
}

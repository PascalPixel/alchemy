#include "types.h"
#include "scene.h"
#include "abi/display/scroll/step_position_every_four_frames.h"

typedef struct {
    u16 unused[4];
    u16 first;
    u16 padding;
    u16 second;
} State;

extern u32 gIw;
extern State gIw2;
extern u8 Value_0000ffff;

void DisplayScroll_StepPositionEveryFourFrames(void)
{
    if ((gIw & 3) == 0) {
        State *state = &gIw2;
        u32 decrement = (u32)&Value_0000ffff;
        state->first += decrement;
        state->second += decrement;
    }
}

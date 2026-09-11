#include "types.h"
#include "scene.h"
#include "abi/ui/runtime/init/reset_counters.h"

struct State_080173ac {
    u8 filler0[0xea8];
    u16 ten;
    u16 one;
    u16 zero;
    u16 fifteen;
    u8 fillerEb0[0x400];
    u16 nine;
};

extern struct State_080173ac *gIw;

void UiWork_ResetCounters(void)
{
    struct State_080173ac *state = gIw;

    state->fifteen = 15;
    state->ten = 10;
    state->nine = 9;
    state->zero = 0;
    state->one = 1;
}

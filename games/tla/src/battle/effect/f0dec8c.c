#include "types.h"

extern u8 gPartyState[];
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void Func_080deba4(void);

void Func_080dec8c(void)
{
    u8 *state;
    s8 *flag;

    /* FAKEMATCH: the one-pass block's end is a scheduling barrier: the native loads the party state's address before it builds the offset 0x26d (one reordered row, differ 2 without). */
    do {
        state = gPartyState;
    } while (0);
    flag = (s8 *)state + 0x26d;
    if (*flag != 0)
        Scheduler_AddOrUpdateCallback(Func_080deba4, 0x480);
}

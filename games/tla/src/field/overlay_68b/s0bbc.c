#include "types.h"

void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void Func_02000b28(void);
void Func_02000ad8(void);

void Func_02000bbc(void)
{
    s32 priority;

    priority = 0x480;
    do { } while (0); /* FAKEMATCH: the loop note ends the scheduling block: ⚓️ builds the priority before it loads the callback's address (2 differ without it). */
    Scheduler_AddOrUpdateCallback(Func_02000b28, priority);
    priority = 0xc80;
    do { } while (0); /* FAKEMATCH: the loop note ends the scheduling block: ⚓️ builds the priority before it loads the callback's address (2 differ without it). */
    Scheduler_AddOrUpdateCallback(Func_02000ad8, priority);
}

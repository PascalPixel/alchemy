/*
 * Draft of Func_02002770 (overlay 664, 0200a770). differ 2: the callback's
 * address is loaded before the priority's shift, where ⚓️ loads it after.
 * Storing -1 directly swaps r2 and r3 (differ 10); through call.h's Call2
 * the same.
 */
#include "types.h"

extern s32 Data_02006c0c;

void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void Func_020026c0(void);

void Func_02002770(void)
{
    s32 value = -1;

    Data_02006c0c = value;
    Scheduler_AddOrUpdateCallback(Func_020026c0, 1152);
}

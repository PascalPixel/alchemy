/* Draft, differ 8 of 10: the native builds -1 in r2 with the address in r3,
 * then shifts the priority before loading the callback. A local for -1
 * fixes the first (leaving differ 2) and a barrier on the priority fixes
 * the second; neither plain form gives both. */
#include "types.h"

extern s32 Data_02006710;

void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void Func_020024ac(void);

void Func_0200255c(void)
{
    Data_02006710 = -1;
    Scheduler_AddOrUpdateCallback(Func_020024ac, 0x480);
}

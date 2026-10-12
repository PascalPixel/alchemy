/* Near miss of Func_02003960 (12 instructions, 2 differ): the native loads the IWRAM entry
 * between `movs r1, #128` and its shift; the draft shifts first. No device tried yet. */
#include "types.h"
#include "iwram_call.h"

extern u8 Data_02006c08[];

s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 order);
void Func_020036a8(void);

void Func_02003960(void)
{
    Iwram_ClearWords(Data_02006c08, 0x200);
    Scheduler_AddOrUpdateCallback(Func_020036a8, 0x480);
}

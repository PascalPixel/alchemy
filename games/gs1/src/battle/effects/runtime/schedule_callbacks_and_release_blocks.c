#include "types.h"

extern void ScheduleCallback(void (*)(void));
extern void Func_08002dd8(s32);

typedef void (*Transfer)(void *, s32);
extern u8 Data_080cc961;
extern u8 Data_080cd261;
extern u8 Data_080cd4b5;

void BattleEffect_ScheduleCallbacksAndReleaseBlocks(void) {
    ScheduleCallback((void (*)(void))&Data_080cc961);
    ScheduleCallback((void (*)(void))&Data_080cd261);
    {
        Transfer transfer = (Transfer)0x03000164;

        transfer((void *)0x06004000, 0x4000);
    }
    ScheduleCallback((void (*)(void))&Data_080cd4b5);
    Func_08002dd8(40);
    Func_08002dd8(39);
}

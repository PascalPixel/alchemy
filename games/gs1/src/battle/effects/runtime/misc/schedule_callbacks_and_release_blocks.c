#include "types.h"
#include "scene.h"
#include "abi/battle/effects/runtime/misc/schedule_callbacks_and_release_blocks.h"

extern void ScheduleCallback(void (*)(void));

typedef void (*Transfer)(void *, s32);
extern u8 gRom;
extern u8 gRom2;
extern u8 gRom3;

void BattleFx_ScheduleCallbacksAndReleaseBlocks(void)
{
    ScheduleCallback((void (*)(void))&gRom);
    ScheduleCallback((void (*)(void))&gRom2);
    {
        Transfer transfer = (Transfer)0x03000164;

        transfer((void *)0x06004000, 0x4000);
    }
    ScheduleCallback((void (*)(void))&gRom3);
    Battle_Do(40);
    Battle_Do(39);
}

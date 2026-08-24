#include "types.h"

s32 ScheduleCallback(s32);
s32 GameFlag_IsSet(s32);
void Menu_CancelSoundTick(void);

void Menu_EnsureCancelSound(void) {
    if (GameFlag_IsSet(0x150) == 0) {
        ScheduleCallback((s32)Menu_CancelSoundTick);
    }
}

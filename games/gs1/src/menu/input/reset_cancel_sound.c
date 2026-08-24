#include "types.h"

s32 ScheduleCallbackAfterFrames(s32, s32);
s32 GameFlag_Clear(s32);
void Menu_CancelSoundTick(void);

void Menu_CancelSoundReset(void) {
    GameFlag_Clear(0x150);
    ScheduleCallbackAfterFrames((s32)Menu_CancelSoundTick, 0xC80);
}

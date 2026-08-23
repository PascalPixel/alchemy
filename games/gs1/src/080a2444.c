#include "types.h"
#include "global_cells.h"
#include "sound_ids.h"

s32 ScheduleCallback(s32);
s32 GameFlag_Set(s32);
s32 Audio_PlayCue(s32);

void Func_080a2444(void) {
    if (*(s32 *)ADDR_03001C94 & 8) {
        Audio_PlayCue(SOUND_MENU_CANCEL);
        GameFlag_Set(0x150);
        ScheduleCallback((s32)Func_080a2444);
    }
}

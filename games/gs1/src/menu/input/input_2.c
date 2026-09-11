#include "types.h"
#include "scene.h"
#include "global_cells.h"
#include "sound_ids.h"

/* menu/input/cancel_sound_tick.c */
s32 ScheduleCallback(s32);
s32 GameFlag_Set(s32);
s32 Audio_PlayCue(s32);

void Menu_CancelSoundTick(void)
{
    if (*(s32 *)ADDR_03001C94 & 8) {
        Audio_PlayCue(SOUND_MENU_CANCEL);
        GameFlag_Set(0x150);
        ScheduleCallback((s32)Menu_CancelSoundTick);
    }
}

/* menu/input/reset_cancel_sound.c */
s32 ScheduleCallbackAfterFrames(s32, s32);
s32 GameFlag_Clear(s32);
void Menu_CancelSoundTick(void);

void Menu_CancelSoundReset(void)
{
    GameFlag_Clear(0x150);
    ScheduleCallbackAfterFrames((s32)Menu_CancelSoundTick, 0xC80);
}

/* menu/input/ensure_cancel_sound.c */
s32 ScheduleCallback(s32);
s32 GameFlag_IsSet(s32);
void Menu_CancelSoundTick(void);

void Menu_EnsureCancelSound(void)
{
    if (GameFlag_IsSet(0x150) == 0) {
        ScheduleCallback((s32)Menu_CancelSoundTick);
    }
}

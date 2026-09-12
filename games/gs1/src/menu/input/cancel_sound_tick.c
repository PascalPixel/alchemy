#include "types.h"
#include "scene.h"
#include "global_cells.h"
#include "sound_ids.h"

/* menu/input/cancel_sound_tick.c */
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
s32 GameFlag_IsSet(s32);

void Menu_EnsureCancelSound(void)
{
    if (GameFlag_IsSet(0x150) == 0) {
        ScheduleCallback((s32)Menu_CancelSoundTick);
    }
}

/* ui/render/palette_set_color15.c */
/* ui/render/set_palette_color_15.c */
s32 UiPalette_SetColor(s32);

void UiPalette_SetColor15(void)
{
    UiPalette_SetColor(15);
}

/* ui/render/set_palette_color_2.c */

void UiPalette_SetColor2(void)
{
    UiPalette_SetColor(2);
}

/* ui/render/set_palette_color_4.c */

void UiPalette_SetColor4(void)
{
    UiPalette_SetColor(4);
}

#include "TYPES.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"
#include "SOUND_IDS.H"

/* menu/input/cancel_sound_tick.c */
/* menu/input/cancel_sound_tick.c */
s32 Scheduler_RemoveCallback(s32);
s32 GameFlag_SetBitFar(s32);
s32 Audio_PlayCue(s32);

void Menu_CancelSoundTick(void)
{
    if (*(s32 *)ADDR_03001C94 & 8) {
        Audio_PlayCue(SOUND_MENU_CANCEL);
        GameFlag_SetBitFar(0x150);
        Scheduler_RemoveCallback((s32)Menu_CancelSoundTick);
    }
}

/* menu/input/reset_cancel_sound.c */
s32 Scheduler_AddOrUpdateCallback(s32, s32);
s32 GameFlag_ClearBitFar(s32);
void Menu_CancelSoundTick(void);

void Menu_CancelSoundReset(void)
{
    GameFlag_ClearBitFar(0x150);
    Scheduler_AddOrUpdateCallback((s32)Menu_CancelSoundTick, 0xC80);
}

/* menu/input/ensure_cancel_sound.c */
s32 GameFlag_TestFar(s32);

void Menu_EnsureCancelSound(void)
{
    if (GameFlag_TestFar(0x150) == 0) {
        Scheduler_RemoveCallback((s32)Menu_CancelSoundTick);
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

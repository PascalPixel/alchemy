#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "TBS_EDITION.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"
#include "SOUND_IDS.H"

extern u8 *volatile gWindowWork;
void Audio_PlayCue(s32 cue);

extern u8 Data_03001c94[];

/* menu/input/cancel_sound_tick.c */
s32 GameFlag_SetBitFar(s32);

/* menu/input/reset_cancel_sound.c */
s32 GameFlag_ClearBitFar(s32);
void Menu_CancelSoundTick(void);

/* menu/input/ensure_cancel_sound.c */
s32 GameFlag_TestFar(s32);

/* ui/render/set_palette_color_15.c */
s32 UiWork_SetParamNibbleFar(s32);

void UiWork_SetMenuBusy(void)
{
    u8 *base = gWindowWork;
    u8 *p = base + RENDER_MENU_BUSY_OFS;
    u8 flag = 1;
    *p = flag;
}

void UiWork_ClearMenuBusy(void)
{
    u8 *base = gWindowWork;
    u8 *p = base + RENDER_MENU_BUSY_OFS;
    u8 flag = 0;
    *p = flag;
}

s32 Audio_PlayCueReturnOne(s32 cue)
{
    Audio_PlayCue(cue);
    return 1;
}

/* menu/input/cancel_sound_tick.c */
void Menu_CancelSoundTick(void)
{
    if (*(s32 *)((u32)&Data_03001c94) & 8) {
        Audio_PlayCue(SOUND_MENU_CANCEL);
        GameFlag_SetBitFar(0x150);
        Scheduler_RemoveCallback((u32)((s32)Menu_CancelSoundTick));
    }
}

void Menu_CancelSoundReset(void)
{
    GameFlag_ClearBitFar(0x150);
    Scheduler_AddOrUpdateCallback((s32)Menu_CancelSoundTick, 0xC80);
}

void Menu_EnsureCancelSound(void)
{
    if (GameFlag_TestFar(0x150) == 0) {
        Scheduler_RemoveCallback((u32)((s32)Menu_CancelSoundTick));
    }
}

/* ui/render/palette_set_color15.c */
void UiPalette_SetColor15(void)
{
    UiWork_SetParamNibbleFar(15);
}

/* ui/render/set_palette_color_2.c */
void UiPalette_SetColor2(void)
{
    UiWork_SetParamNibbleFar(2);
}

/* ui/render/set_palette_color_4.c */
void UiPalette_SetColor4(void)
{
    UiWork_SetParamNibbleFar(4);
}

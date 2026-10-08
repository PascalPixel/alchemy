#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "TBS_EDITION.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"
#include "SOUND_IDS.H"
#include "BTLSYS.H"

void AudioCommand_PlayFar(s32 cue);

#include "KEYSTATE.H"
#include "IO_REG.H"


void UiWork_SetMenuBusy(void)
{
    u8 *base = gWindowWork[0];
    u8 *p = base + RENDER_MENU_BUSY_OFS;
    u8 flag = 1;
    *p = flag;
}

void UiWork_ClearMenuBusy(void)
{
    u8 *base = gWindowWork[0];
    u8 *p = base + RENDER_MENU_BUSY_OFS;
    u8 flag = 0;
    *p = flag;
}

s32 Audio_PlayCueReturnOne(s32 cue)
{
    AudioCommand_PlayFar(cue);
    return 1;
}

void Menu_CancelSoundTick(void)
{
    if (gKeyState & KEY_START) {
        AudioCommand_PlayFar(SOUND_MENU_CANCEL);
        GameFlag_SetBitFar(0x150);
        Scheduler_RemoveCallback(Menu_CancelSoundTick);
    }
}

void Menu_CancelSoundReset(void)
{
    GameFlag_ClearBitFar(0x150);
    Scheduler_AddOrUpdateCallback(Menu_CancelSoundTick, 0xC80);
}

void Menu_EnsureCancelSound(void)
{
    if (GameFlag_TestFar(0x150) == 0) {
        Scheduler_RemoveCallback(Menu_CancelSoundTick);
    }
}

void UiPalette_SetColor15(void)
{
    UiWork_SetParamNibbleFar(15);
}

void UiPalette_SetColor2(void)
{
    UiWork_SetParamNibbleFar(2);
}

void UiPalette_SetColor4(void)
{
    UiWork_SetParamNibbleFar(4);
}

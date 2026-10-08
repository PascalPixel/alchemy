#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "callback_scheduler.h"
#include "tbs_edition.h"
#include "scene.h"
#include "global_cells.h"
#include "sound_ids.h"
#include "btlsys.h"

void AudioCommand_PlayFar(s32 cue);

#include "keystate.h"
#include "io_reg.h"


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

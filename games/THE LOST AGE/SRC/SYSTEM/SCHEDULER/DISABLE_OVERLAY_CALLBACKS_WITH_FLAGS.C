#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"

s32 GameFlag_ClearBit(s32 flag_no);
s32 GameFlag_SetBit(s32 flag_no);

s32 Scheduler_DisableOverlayCallbacksWithFlags(void)
{
    Scheduler_DisableOverlayCallbacks();
    Map_DisableUpdateCallbackFar();
    GameFlag_ClearBit(0x166);
    return GameFlag_ClearBit(0x152);
}

s32 Scheduler_EnableOverlayCallbacksWithFlags(void)
{
    GameFlag_SetBit(0x152);
    GameFlag_SetBit(0x166);
    Map_EnableUpdateCallbackFar();
    return Scheduler_EnableUnmaskedOverlayCallbacks();
}

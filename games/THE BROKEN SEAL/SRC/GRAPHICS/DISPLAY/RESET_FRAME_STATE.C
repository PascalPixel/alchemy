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

extern u8 IwramClearWords[];
extern u8 gObjAffineCount[];

s32 _call_via_r3(s32, s32, s32, s32);

void Graphics_ResetFrameState(void)
{
    *(s8 *)((u32)&gObjAffineCount) = 0;
    _call_via_r3(((u32)&Data_03001400), 1024, ((u32)&gObjAffineCount), (u32)IwramClearWords);
}

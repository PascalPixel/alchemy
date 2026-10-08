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

extern u8 IwramClearWords[];
extern u8 gObjAffineCount[];

s32 _call_via_r3(s32, s32, s32, s32);

void Graphics_ResetFrameState(void)
{
    gObjAffineCount[0] = 0;
    _call_via_r3(((u32)&Data_03001400), 0x400, ((u32)&gObjAffineCount), (u32)IwramClearWords);
}

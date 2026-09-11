#include "types.h"
#include "scene.h"
#include "abi/battle/effects/objects/flicker_and_tick.h"

extern s32 gIw;

void BattleFx_FlickerObjectAndTick(s32 arg0)
{
    if ((gIw & 2) != 0) {
        Battle_Apply(arg0, 7);
    } else {
        Battle_Apply(arg0, 0);
    }
    if ((gIw & 15) == 0) {
        Battle_Do(arg0);
    }
}

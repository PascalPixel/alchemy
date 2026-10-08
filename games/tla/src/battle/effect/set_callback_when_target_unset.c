#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "objdisp.h"
#include "object_runtime.h"
#include "frame.h"


void BattleFx_SetCallbackWhenTargetUnset(struct ObjectRuntime *target)
{
    s32 tx;
    s32 ty;

    tx = target->target_x;
    /* FAKEMATCH: ⚓️ loads x before it builds the unset marker. */
    __asm__("");
    if (tx == 0x80000000) {
        ty = target->target_y;
        if ((ty == tx) && (target->target_z == ty)) {
            ObjectDispatch_InitializeFar((struct DispatchObject *)target, (u32)BattleFx_UntargetedObjectScript);
        }
    }
}

extern u8 Data_080f0e9c[];
void Animation_ApplyChildValuesFar(void *object, s32 value);

void BattleFx_ApplyFlickerValue(void *object, s32 value)
{
    s32 child = value - 1;

    if (value == 0)
        child = Data_080f0e9c[(gFrameCount >> 1) & 7];
    Animation_ApplyChildValuesFar(object, child);
}

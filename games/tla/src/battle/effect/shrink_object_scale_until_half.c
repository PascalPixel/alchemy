#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "object_runtime.h"

#define Object_SetPositionAndResetMotionFar ((s32 (*)(void*,s32,s32,s32))Object_SetPositionAndResetMotionFar)

s32 RunBattleEffect14();


void BattleFx_ShrinkObjectScaleUntilHalf(struct ObjectRuntime *object)
{
    s32 scale;

    scale = object->scale_x - 0x80;
    object->scale_y = scale;
    object->scale_x = scale;
    if (scale < 0x8000) {
        Object_SetPositionAndResetMotionFar(object, 0, 0, 0);
        object->update = 0;
    }
}

void BattleFx_CallEffect14(void)
{
    RunBattleEffect14();
}

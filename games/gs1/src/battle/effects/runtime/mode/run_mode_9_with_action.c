#include "b5_context.h"
#include "types.h"
#include "scene.h"
#include "abi/battle/effects/runtime/mode/run_mode_9_with_action.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Object_SetMode(s32, s32);
s32 Object_SetAction(s32, s32);

void BattleFx_RunMode9WithAction(void *effect)
{
    s32 object;

    object =
        (s32)Battle_Run(FIELD_AT_OFFSET(effect, s32 *, 8))->object;
    Object_SetMode(object, 2);
    Object_SetAction(object, 0x30);
    Battle_Apply(effect, 9);
    Object_SetAction(object, 0x10);
}

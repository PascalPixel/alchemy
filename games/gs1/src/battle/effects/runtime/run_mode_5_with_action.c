#include "b5_context.h"
#include "types.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Object_SetMode(s32, s32);
s32 Object_SetAction(s32, s32);
s32 Func_080dea70(void *, s32);

void BattleEffect_RunMode5WithAction(void *effect) {
    s32 object;

    object =
        (s32)Func_080b5098(FIELD_AT_OFFSET(effect, s32 *, 8))->object;
    Object_SetMode(object, 2);
    Object_SetAction(object, 0x30);
    Func_080dea70(effect, 5);
    Object_SetAction(object, 0x10);
}

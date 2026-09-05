#include "types.h"

s32 Object_SetMode(s32, s32);
s32 Object_ResetMotion(s32);
s32 *GetBattleObjectSlot();

void BattleMotion_ResetSlotObjectMode2(void) {
    s32 object;

    object = *GetBattleObjectSlot();
    Object_ResetMotion(object);
    Object_SetMode(object, 2);
}

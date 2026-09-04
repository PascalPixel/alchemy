#include "types.h"
#include "object_effect.h"

#define BattleEffect_LinkObjectToTarget Func_08093570

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void *Runtime_AllocateBlock(s32 arg0, s32 arg1);
void BattleEffect_LinkObjectToTarget(void *target, s32 keep_current_position) {
    void *object;

    object = FIELD_AT_OFFSET(Runtime_AllocateBlock(0x1B, 0xCCC), void **, 0x1E0);
    if (target != NULL) {
        Func_080090e0(object, NULL);
        FIELD_AT_OFFSET(object, void **, 0x68) = target;
        if (keep_current_position == 0) {
            FIELD_AT_OFFSET(object, s32 *, 8) = FIELD_AT_OFFSET(target, s32 *, 8);
            FIELD_AT_OFFSET(object, s32 *, 0xC) = FIELD_AT_OFFSET(target, s32 *, 0xC);
            FIELD_AT_OFFSET(object, s32 *, 0x10) = FIELD_AT_OFFSET(target, s32 *, 0x10);
        }
    }
}

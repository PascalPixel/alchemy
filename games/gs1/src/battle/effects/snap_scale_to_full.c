#include "types.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Object_CommitPosition();

void BattleEffect_SnapScaleToFull(void *obj) {
    s32 next;
    s32 scale;

    if (obj != NULL) {
        scale = FIELD_AT_OFFSET(obj, s32 *, 0x18);
        if (scale <= 0xFFFF) {
            do {
                next = scale + 0x1000;
                scale = next;
            } while (next <= 0xFFFF);
            FIELD_AT_OFFSET(obj, s32 *, 0x18) = next;
            FIELD_AT_OFFSET(obj, s32 *, 0x1C) = next;
        }
        Object_CommitPosition();
    }
}

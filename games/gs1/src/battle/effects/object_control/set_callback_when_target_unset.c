#include "types.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Object_SetCallback(void *, s32);
extern const u8 Data_080a0128[];

struct Target_08097a54 {
    u8 unknown_00[0x38];
    s32 x;
    s32 y;
    s32 z;
};

void BattleEffect_SetCallbackWhenTargetUnset(struct Target_08097a54 *target) {
    s32 ty;
    s32 tx;

    tx = target->x;
    if (tx == 0x80000000) {
        ty = target->y;
        if ((ty == tx) && (target->z == ty)) {
            Object_SetCallback(target, (s32)Data_080a0128);
        }
    }
}

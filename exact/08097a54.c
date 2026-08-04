#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Func_08009098(void *, s32);

struct Target_08097a54 {
    u8 unknown_00[0x38];
    s32 x;
    s32 y;
    s32 z;
};

void Func_08097a54(struct Target_08097a54 *target) {
    s32 temp_r2;
    s32 temp_r3;

    temp_r3 = target->x;
    if (temp_r3 == 0x80000000) {
        temp_r2 = target->y;
        if ((temp_r2 == temp_r3) && (target->z == temp_r2)) {
            Func_08009098(target, 0x080A0128);
        }
    }
}

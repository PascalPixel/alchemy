#include "types.h"
#include "fixed_math.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 ArcTan2(s32, s32);

s32 ObjectMotion_TurnTowardLinkedTarget(void *arg0) {
    s16 var_r0;
    u16 temp_r3;
    s32 temp;
    void *temp_r1;

    temp_r1 = FIELD_AT_OFFSET(arg0, void *, 0x68);
    if (temp_r1 != NULL) {
        FIELD_AT_OFFSET(arg0, u8, 0x5A) = (u8) (0xFE & FIELD_AT_OFFSET(arg0, u8, 0x5A));
        temp = (u16) ArcTan2(FIELD_AT_OFFSET(temp_r1, s32, 0x10) - FIELD_AT_OFFSET(arg0, s32, 0x10), FIELD_AT_OFFSET(temp_r1, s32, 8) - FIELD_AT_OFFSET(arg0, s32, 8));
        temp_r3 = FIELD_AT_OFFSET(arg0, u16, 6);
        var_r0 = temp - temp_r3;
        if (var_r0 != 0) {
            if (var_r0 > 0x1000) {
                var_r0 = 0x1000;
            }
            if (var_r0 < -0x1000) {
                var_r0 = -0x1000;
            }
            FIELD_AT_OFFSET(arg0, u16, 6) = (u16) (temp_r3 + var_r0);
        }
    }
    return 1;
}

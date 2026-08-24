#include "fixed_math.h"
#include "types.h"
#include "object_efx.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

u32 Random16(void);
/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
#define Rand Random16
s32 Object_SetCallback(void *, s32);
void ObjectMotion_SetTargetPositionFromMagnitudeAngle(
    struct Object_08096bec *object, s32 magnitude, s32 angle);
void *Object_Spawn(s32, s32, s32, s32);

void Func_08099920(void *object) {
    s32 var_r7;
    void *temp_r0;
    s32 phase = 2;
    s32 phase2;
    s16 *ptr2;

    if ((s32) M2C_FIELD(object, s32 *, 0xC) <= (s32) M2C_FIELD(object, s32 *, 0x14)) {
        M2C_FIELD(object, s16 *, 0x5E) = phase;
        Object_SetCallback(object, Data_0809f0b0);
        temp_r0 = NULL;
        M2C_FIELD(object, void **, 0x6C) = temp_r0;
        for (var_r7 = 0; var_r7 <= 2; var_r7++) {
            temp_r0 = Object_Spawn(0xF0, M2C_FIELD(object, s32 *, 8), M2C_FIELD(object, s32 *, 0xC), M2C_FIELD(object, s32 *, 0x10));
            if (temp_r0 == NULL) {
                break;
            }
            M2C_FIELD(temp_r0, s32 *, 0x1C) = 0x8000;
            M2C_FIELD(temp_r0, s32 *, 0x18) = 0x8000;
            M2C_FIELD(temp_r0, s8 *, 0x55) = 2;
            M2C_FIELD(temp_r0, s32 *, 0x28) = 0x10000;
            M2C_FIELD(temp_r0, s32 *, 0x30) = (s32) (Rand() + 0x13333);
            ObjectMotion_SetTargetPositionFromMagnitudeAngle(
                temp_r0, 0x200000, Rand());
            ptr2 = &M2C_FIELD(temp_r0, s16 *, 0x5E);
            phase2 = 6;
            *ptr2 = phase2;
            Object_SetCallback(temp_r0, Data_0809f0b0);
        }
    }
}

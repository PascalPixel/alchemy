#include "fixed_math.h"
#include "types.h"
#include "object_efx.h"

#define BattleEffect_SpawnRandomAngleTriplet Func_08099920

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

u32 Random16(void);
/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
#define Rand Random16
s32 Object_SetCallback(void *, s32);
void ObjectMotion_SetTargetPositionFromMagnitudeAngle(
    struct Object_08096bec *object, s32 magnitude, s32 angle);
void *Object_Spawn(s32, s32, s32, s32);

void BattleEffect_SpawnRandomAngleTriplet(void *object) {
    s32 i;
    void *p;
    s32 phase = 2;
    s32 phase2;
    s16 *pp;

    if ((s32) FIELD_AT_OFFSET(object, s32 *, 0xC) <= (s32) FIELD_AT_OFFSET(object, s32 *, 0x14)) {
        FIELD_AT_OFFSET(object, s16 *, 0x5E) = phase;
        Object_SetCallback(object, Data_0809f0b0);
        p = NULL;
        FIELD_AT_OFFSET(object, void **, 0x6C) = p;
        for (i = 0; i <= 2; i++) {
            p = Object_Spawn(0xF0, FIELD_AT_OFFSET(object, s32 *, 8), FIELD_AT_OFFSET(object, s32 *, 0xC), FIELD_AT_OFFSET(object, s32 *, 0x10));
            if (p == NULL) {
                break;
            }
            FIELD_AT_OFFSET(p, s32 *, 0x1C) = 0x8000;
            FIELD_AT_OFFSET(p, s32 *, 0x18) = 0x8000;
            FIELD_AT_OFFSET(p, s8 *, 0x55) = 2;
            FIELD_AT_OFFSET(p, s32 *, 0x28) = 0x10000;
            FIELD_AT_OFFSET(p, s32 *, 0x30) = (s32) (Rand() + 0x13333);
            ObjectMotion_SetTargetPositionFromMagnitudeAngle(
                p, 0x200000, Rand());
            pp = &FIELD_AT_OFFSET(p, s16 *, 0x5E);
            phase2 = 6;
            *pp = phase2;
            Object_SetCallback(p, Data_0809f0b0);
        }
    }
}

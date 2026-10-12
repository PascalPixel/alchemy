/* Near miss of overlay 662 Func_02000754: the six-entry loop only. Inside it the native reloads
 * hit->animation, its value pointer, the value and the table address on every pass and indexes the table
 * (lsls r3, r4, #1; ldrh r3, [r2, r3]); the draft hoists them all and walks the table. Tried: a one-pass
 * block around the test, the loop written with a goto. The quarter turn needs one variable assigned
 * twice (the shifted angle, then the rounded turn): as one expression or three locals it stays in r1. */
#include "types.h"
#include "object_runtime.h"

extern u16 Data_02001968[];

void Vector_AddPolarOffsetFar(s32 length, s32 angle, s32 *position);
struct ObjectRuntime *Func_02001948(s32 *position, s32 mode);
s32 Func_02001850(struct ObjectRuntime *object, s32 *position);

void Func_02000754(struct ObjectRuntime *object)
{
    s32 position[3];
    struct ObjectRuntime *hit;
    u32 i;
    s32 angle = object->angle >> 12;

    angle = ((angle + 2) & 12) << 12;
    position[0] = object->x;
    position[1] = object->y;
    position[2] = object->z;
    Vector_AddPolarOffsetFar(0x100000, angle, position);
    hit = Func_02001948(position, 1);
    if (hit != 0) {
        for (i = 0; i <= 5; i++) {
            if (*((struct BattleAnimationState *)hit->animation)->value_28 == Data_02001968[i])
                return;
        }
        Object_SetPositionAndResetMotion(object, object->x, object->y, object->z);
    }
    position[0] = object->x;
    position[1] = object->y;
    position[2] = object->z;
    if (Func_02001850(object, position) > 0)
        Object_SetPositionAndResetMotion(object, object->x, object->y, object->z);
}

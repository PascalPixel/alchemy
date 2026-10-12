/* Drafts of overlay 69C Func_02000134 and Func_02001300 (first written 2026-10-12).
 * Func_02000134: differ 2 of 62 (one reordered row): the native builds the mode (movs r2, #0) right after the
 * motion flag store and before it loads x and z; the draft builds it after the two loads. Measured: the mode as a
 * cleared variable (the store then shares its zero, differ 9), one-pass blocks on the store, on the variable and on
 * the x load (ldr r0 first, then movs r2), the store as the third argument (the loads come before the store).
 * The second x read is volatile: plain, PRE keeps x in r2 across the first test and reloads it only after the
 * call (differ 22); an inline cell test and a second struct type did not stop it. It needs a FAKEMATCH tag
 * with that reason when the function is adopted.
 * Func_02001300: differ 42 of 61. The native rereads other->animation->value_28 and reloads the table address on
 * every pass with the index in r4, and builds the angle through two registers before it copies the position;
 * the draft hoists all three in a for loop, a goto loop and with a one-pass block around the test. */
#include "../../../../games/tla/src/field/overlay_69c/local.h"

extern const u16 Data_02002478[];

void Vector_AddPolarOffsetFar(s32 distance, s32 angle, s32 *position);
struct ObjectRuntime *Func_02002438(s32 *position, s32 mode);
s32 Func_02002338(struct ObjectRuntime *object, s32 *position);

void Func_02000134(s32 unused, s32 id)
{
    struct ObjectRuntime *object = Object_GetById(id);
    s32 placed = 0;
    s32 i;

    if (object->x >> 20 == 20 && object->z >> 20 == 36) {
        GameFlag_SetBit_(0xa04);
        placed = 1;
    }
    if (*(volatile s32 *)&object->x >> 20 == 32 && object->z >> 20 == 36) {
        GameFlag_SetBit_(0xa05);
        placed = 1;
    }
    if (placed) {
        WaitFrames_(10);
        Func_02002470(134);
        for (i = 0; i < 30; i++) {
            if (object->y <= object->terrain_height && object->velocity_y == 0)
                break;
            WaitFrames(1);
        }
        object->motion_flags = 0;
        Func_02002368(object->x, object->z, 0);
    }
}

void Func_02001300(struct ObjectRuntime *object)
{
    s32 position[3];
    struct ObjectRuntime *other;
    u32 i;
    s32 angle;
    s32 dir;

    dir = object->angle >> 12;
    angle = ((dir + 2) & 12) << 12;
    position[0] = object->x;
    position[1] = object->y;
    position[2] = object->z;
    Vector_AddPolarOffsetFar(0x100000, angle, position);
    other = Func_02002438(position, 1);
    if (other != NULL) {
        i = 0;
    next:
        if (*((struct BattleAnimationState *)other->animation)->value_28 == Data_02002478[i])
            return;
        i++;
        if (i < 6)
            goto next;
        Object_SetPositionAndResetMotion(object, object->x, object->y, object->z);
    }
    position[0] = object->x;
    position[1] = object->y;
    position[2] = object->z;
    if (Func_02002338(object, position) > 0)
        Object_SetPositionAndResetMotion(object, object->x, object->y, object->z);
}

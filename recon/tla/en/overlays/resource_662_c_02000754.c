/* Near miss, differ 2 of 61 (one reordered row): overlay 662's Func_02000754, the representative of its copies in 668, 674, 69c,
 * 6a1 and 6ad. The native reloads hit->animation, its value and the table address on every pass and indexes the table: a goto
 * loop and a memory barrier at its head give exactly that (as a for loop loop.c hoists them and walks the table, 18; as a goto
 * loop without the barrier GCSE hoists them, 30). Left: the native loads object->x before the frame's sub sp; here sched2 ranks
 * the two equal (priority 40) and takes sub sp, the anti-dependent load losing to it. Tried: the angle and the position in
 * either order, the angle in two statements, the locals reordered, an unused local, barriers and one-pass blocks after each
 * statement (alchemy permute-style scan, singly and in pairs). */
#include "types.h"
#include "object_runtime.h"

extern u16 gData_02001968[];

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
        i = 0;
    next:
        __asm__("" : : : "memory");
        if (*((struct BattleAnimationState *)hit->animation)->value28 == gData_02001968[i])
            return;
        i++;
        if (i <= 5)
            goto next;
        Object_SetPositionAndResetMotion(object, object->x, object->y, object->z);
    }
    position[0] = object->x;
    position[1] = object->y;
    position[2] = object->z;
    if (Func_02001850(object, position) > 0)
        Object_SetPositionAndResetMotion(object, object->x, object->y, object->z);
}

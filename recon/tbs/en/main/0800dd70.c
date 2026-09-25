/* Draft, not exact (2026-09-25): 404 of 404 bytes, 29 differing halfwords.
   Split from the listing that also held main:0800df04. Control flow, frame
   and every call match; the loop is a goto loop whose success block sits
   after the failure block, as in the ROM. Remaining: reload picks r2/r3/r5
   where the ROM uses r5/r2/r0 around the leash square, the retry counter and
   the distance products (declaration order swept, 150 orders, no change). */
#include "OBJECT_RUNTIME.H"
#include "IWRAM_CALL.H"

struct WanderPosition {
    s32 x;
    s32 y;
    s32 z;
};

u32 Random16(void);
void Vector_AddPolarOffset(s32 radius, s32 angle, struct WanderPosition *position);
s32 ScriptObject_CheckOverlap(struct ObjectRuntime *object, struct WanderPosition *position);
s32 Func_080120dc(struct ObjectRuntime *object, struct WanderPosition *position);
void Object_SetMoveTarget(struct ObjectRuntime *object, s32 x, s32 y, s32 z);

/*
 * Script command: wander to a random point. The three arguments are the
 * base distance, the random extra distance and the leash radius around the
 * object's home cell (+0x64, +0x66). Up to seven headings within a quarter
 * turn either side of the facing are tried; each must be free of objects,
 * and the point a further half tile on, turned an eighth either side, must
 * be walkable. When none fits, the object turns round and flags +0x5e.
 */
s32 Object_Wander(struct ObjectRuntime *object)
{
    struct WanderPosition pos;
    struct WanderPosition probe;
    s32 *args;
    s32 base;
    s32 range;
    s32 limit;
    s32 radius;
    s32 angle;
    s32 tries;
    s32 dx;
    s32 dz;

    args = &object->script[object->step + 1];
    base = *args++;
    range = *args++;
    limit = *args / 0x10000;
    limit = limit * limit;
    tries = 0;
retry:
    tries++;
    if (tries <= 7) {
        pos.x = object->x;
        pos.y = object->y;
        pos.z = object->z;
        radius = base + Iwram_MulQ16(Random16(), range);
        angle = object->angle + (Random16() >> 2) - (Random16() >> 2);
        Vector_AddPolarOffset(radius, angle, &pos);
        if (ScriptObject_CheckOverlap(object, &pos) != 0)
            goto retry;
        if (Func_080120dc(object, &pos) != 0)
            goto retry;
        radius += 0x80000;
        probe.x = object->x;
        probe.y = object->y;
        probe.z = object->z;
        Vector_AddPolarOffset(radius, angle, &probe);
        probe.x = object->x;
        probe.y = object->y;
        probe.z = object->z;
        Vector_AddPolarOffset(radius, angle + 0x2000, &probe);
        if (Func_080120dc(object, &probe) != 0)
            goto retry;
        probe.x = object->x;
        probe.y = object->y;
        probe.z = object->z;
        Vector_AddPolarOffset(radius, angle - 0x2000, &probe);
        if (Func_080120dc(object, &probe) != 0)
            goto retry;
        dx = pos.x / 0x10000 - object->action;
        dz = pos.z / 0x10000 - object->unknown_66;
        if (dx * dx + dz * dz > limit)
            goto retry;
        goto found;
    }
    object->angle += 0x8000;
    object->unknown_5e = 1;
    return 0;
found:
    Object_SetMoveTarget(object, pos.x, pos.y, pos.z);
    object->step += 4;
    return 1;
}

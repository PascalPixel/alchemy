/* Draft, not exact (2026-09-25): 792 of 796 bytes, 101 differing halfwords.
   Written from the listing. What lined up: a u16 heading base read through
   an (s16) cast, so the ROM's ldrsh load and per-use zero extension appear;
   position before probe for the stack slots; the probe fan straight, then
   an eighth and a quarter either side; +0x59 bit 1 set when roaming.
   Remaining: after ArcTan2 the ROM sign-extends the turned-back heading
   (lsls/asrs) into the same stack slot as the first heading, and the second
   loop keeps its heading in r7 and position pointer in r5; here the heading
   is zero-extended into r9 (u16/s16/s32 locals, a separate s16 local, an
   inline helper and & 0xffff all tried). */
#include "SCRIPT_OBJECT_RUNTIME.H"
#include "IWRAM_CALL.H"

struct WanderPosition {
    s32 x;
    s32 y;
    s32 z;
};

u32 Random16(void);
void Vector_AddPolarOffset(s32 radius, s32 angle, struct WanderPosition *position);
s32 ScriptObject_CheckOverlap(struct ScriptObjectRuntime *object, struct WanderPosition *position);
s32 Func_080120dc(struct ScriptObjectRuntime *object, struct WanderPosition *position);
void Object_SetMoveTarget(struct ScriptObjectRuntime *object, s32 x, s32 y, s32 z);
u16 ArcTan2(s32 y, s32 x);

/*
 * Script command: wander about the object's home cell (+0x64, +0x66). The
 * three arguments are the base distance, the random extra distance and the
 * leash radius in whole units. Inside the leash, up to seven headings near
 * the stored heading are tried; the point must be free of objects, and five
 * probes half a tile further on, straight ahead and turned an eighth and a
 * quarter either side, must be walkable, and the point must stay inside the
 * leash. Outside the leash, the object heads back towards home and clears
 * the flag it sets while roaming.
 */
s32 ScriptObject_WanderNearHome(struct ScriptObjectRuntime *object)
{
    struct WanderPosition pos;
    struct WanderPosition probe;
    const s32 *args;
    s32 base;
    s32 range;
    s32 limit;
    s32 radius;
    u16 heading;
    u16 angle;
    s32 tries;
    s32 dx;
    s32 dz;

    args = &object->script[(s16)object->script_cursor + 1];
    base = *args++;
    range = *args++;
    limit = *args / 0x10000;
    limit = limit * limit;
    tries = 0;
    angle = (s16)object->script_value;
    dx = object->x / 0x10000 - object->home_x;
    dz = object->z / 0x10000 - object->home_z;
    if (dx * dx + dz * dz > limit)
        goto home;
roam:
    tries++;
    if (tries > 7)
        goto home;
    radius = base + Iwram_MulQ16(Random16(), range);
    heading = angle + (Random16() >> 2) - (Random16() >> 2);
    pos.x = object->x;
    pos.y = object->y;
    pos.z = object->z;
    Vector_AddPolarOffset(0x80000, heading, &pos);
    if (ScriptObject_CheckOverlap(object, &pos) != 0)
        goto roam;
    pos.x = object->x;
    pos.y = object->y;
    pos.z = object->z;
    Vector_AddPolarOffset(radius, heading, &pos);
    if (Func_080120dc(object, &pos) != 0)
        goto roam;
    probe.x = object->x;
    probe.y = object->y;
    radius += 0x80000;
    probe.z = object->z;
    Vector_AddPolarOffset(radius, heading, &probe);
    if (Func_080120dc(object, &probe) != 0)
        goto roam;
    probe.x = object->x;
    probe.y = object->y;
    probe.z = object->z;
    Vector_AddPolarOffset(radius, heading + 0x2000, &probe);
    if (Func_080120dc(object, &probe) != 0)
        goto roam;
    probe.x = object->x;
    probe.y = object->y;
    probe.z = object->z;
    Vector_AddPolarOffset(radius, heading - 0x2000, &probe);
    if (Func_080120dc(object, &probe) != 0)
        goto roam;
    probe.x = object->x;
    probe.y = object->y;
    probe.z = object->z;
    Vector_AddPolarOffset(radius, heading + 0x4000, &probe);
    if (Func_080120dc(object, &probe) != 0)
        goto roam;
    probe.x = object->x;
    probe.y = object->y;
    probe.z = object->z;
    Vector_AddPolarOffset(radius, heading - 0x4000, &probe);
    if (Func_080120dc(object, &probe) != 0)
        goto roam;
    dx = pos.x / 0x10000 - object->home_x;
    dz = pos.z / 0x10000 - object->home_z;
    if (dx * dx + dz * dz > limit)
        goto roam;
    object->flags_59 |= 2;
    Object_SetMoveTarget(object, pos.x, pos.y, pos.z);
    goto done;
home:
    tries = 0;
    angle = (s16)(ArcTan2(dz, dx) + 0x8000);
    for (;;) {
        tries++;
        if (tries > 7)
            goto done;
        radius = base + Iwram_MulQ16(Random16(), range);
        heading = angle + (Random16() >> 2) - (Random16() >> 2);
        pos.x = object->x;
        pos.y = object->y;
        pos.z = object->z;
        Vector_AddPolarOffset(0x80000, heading, &pos);
        if (ScriptObject_CheckOverlap(object, &pos) != 0)
            continue;
        pos.x = object->x;
        pos.y = object->y;
        pos.z = object->z;
        Vector_AddPolarOffset(radius, heading, &pos);
        if (Func_080120dc(object, &pos) != 0)
            continue;
        break;
    }
    object->flags_59 &= ~2;
    Object_SetMoveTarget(object, pos.x, pos.y, pos.z);
done:
    object->script_cursor += 4;
    return 1;
}

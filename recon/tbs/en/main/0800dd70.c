/* NONMATCHING: Object_Wander, main 0800dd70..0800df04 (404 bytes with its
 * three pool words), just before ScriptObject_WanderNearHome at the head of
 * FIELD/COMMON/SCRIPT/SCRIPT.C, where it belongs once exact.
 * 2026-10-01 (☀️ matcher 1): rewritten in its exact sibling's spelling
 * (ScriptObject_WanderNearHome): the script object view, separate pos and
 * probe locals, a goto retry loop with the body under `if (tries <= 7)`,
 * the blocked branch before the found one, the leash squared after tries is
 * cleared and dx summed first. Permuter score 100 from 1360 (7
 * register-only, 1 reordered); every block, call, stack slot and pool word
 * lines up. Remaining: the leash test's loads of pos.x and pos.z. The
 * reference loads each into the division's register (r3, r2) and copies it
 * to r1/r4 for Object_SetMoveTarget, the shape of GCSE's copy after a
 * redundant load; here the first CSE pass follows the conditional jump into
 * the found block and reuses the load pseudos themselves, so the loads land
 * in r1/r4 and the division works on copies. A label with a second use, or
 * no barrier before found, would stop CSE following; if/else, a for/break
 * loop and named x/z copies were tried without change. */
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

/*
 * Script command: wander to a random point. The three arguments are the
 * base distance, the random extra distance and the leash radius around the
 * object's home cell. Up to seven headings within a quarter turn either
 * side of the facing are tried; each must be free of objects, and the point
 * a further half tile on, turned an eighth either side, must be walkable
 * and inside the leash. When none fits, the object turns round.
 */
s32 Object_Wander(struct ScriptObjectRuntime *object)
{
    struct WanderPosition pos;
    struct WanderPosition probe;
    s32 base;
    const s32 *args;
    s32 range;
    s32 limit;
    s32 radius;
    s32 heading;
    s32 tries;
    s32 dx;
    s32 dz;

    args = &object->script[(s16)object->script_cursor + 1];
    base = *args++;
    range = *args++;
    limit = *args / 0x10000;
    tries = 0;
    limit = limit * limit;
retry:
    tries++;
    if (tries <= 7) {
        pos.x = object->x;
        pos.y = object->y;
        pos.z = object->z;
        radius = base + Iwram_MulQ16(Random16(), range);
        heading = object->script_value + (Random16() >> 2) - (Random16() >> 2);
        Vector_AddPolarOffset(radius, heading, &pos);
        if (ScriptObject_CheckOverlap(object, &pos) != 0)
            goto retry;
        if (Func_080120dc(object, &pos) != 0)
            goto retry;
        radius += 0x80000;
        probe.x = object->x;
        probe.y = object->y;
        probe.z = object->z;
        Vector_AddPolarOffset(radius, heading, &probe);
        probe.x = object->x;
        probe.y = object->y;
        probe.z = object->z;
        Vector_AddPolarOffset(radius, heading + 0x2000, &probe);
        if (Func_080120dc(object, &probe) != 0)
            goto retry;
        probe.x = object->x;
        probe.y = object->y;
        probe.z = object->z;
        Vector_AddPolarOffset(radius, heading - 0x2000, &probe);
        if (Func_080120dc(object, &probe) != 0)
            goto retry;
        dx = pos.x / 0x10000 - object->home_x;
        dz = pos.z / 0x10000 - object->home_z;
        if (dx * dx + dz * dz > limit)
            goto retry;
        goto found;
    }
    object->script_value += 0x8000;
    object->turned_back = 1;
    return 0;
found:
    Object_SetMoveTarget(object, pos.x, pos.y, pos.z);
    object->script_cursor += 4;
    return 1;
}

/* Near miss, differ 28 of 188, all instruction order (the same 188 instructions): The Broken
 * Seal's Object_Wander on this game's headers. Where a position is copied field by field
 * before a call, the native puts one argument instruction between each store and the next
 * load (str x; lsls r1; ldr y; add r1, sl; str y; mov r0, r8; ldr z); here each load follows
 * its store at once and the argument instructions fill only the load-to-store slots (sched2:
 * the next load has priority 40, the argument chain 36). At the top the native stores both
 * limit values before it clears tries; tries cleared after the square in source is differ 66. */
#include "types.h"
#include "field.h"
#include "system.h"
#include "object_runtime.h"
#include "script_object_runtime.h"
#include "iwram_call.h"

struct ScriptObjectEntry;

struct FieldPosition
{
	s32 x;
	s32 y;
	s32 z;
};

void Vector_AddPolarOffset(s32 radius, s32 angle, struct FieldPosition *position);
s32 ScriptObject_CheckOverlap(struct ScriptObjectEntry *object, s32 *position);
s32 Object_SetMoveTarget(struct ObjectRuntime *object, s32 x, s32 y, s32 z);

s32 Object_Wander(struct ScriptObjectRuntime *object)
{
    struct FieldPosition pos;
    struct FieldPosition probe;
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
    /* FAKEMATCH: the retry label and one-pass distance block preserve the 32-byte frame and division/load registers, as in The Broken Seal's twin. */
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
        if (ScriptObject_CheckOverlap((struct ScriptObjectEntry *)object, (s32 *)&pos) != 0)
            goto retry;
        if (Map_CheckObjectStep((struct ObjectRuntime *)object, &pos) != 0)
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
        if (Map_CheckObjectStep((struct ObjectRuntime *)object, &probe) != 0)
            goto retry;
        probe.x = object->x;
        probe.y = object->y;
        probe.z = object->z;
        Vector_AddPolarOffset(radius, heading - 0x2000, &probe);
        if (Map_CheckObjectStep((struct ObjectRuntime *)object, &probe) != 0)
            goto retry;
        do {
            dx = pos.x / 0x10000 - object->home_x;
            dz = pos.z / 0x10000 - object->home_z;
        } while (0);
        if (dx * dx + dz * dz > limit)
            goto retry;
        goto found;
    }
    object->script_value += 0x8000;
    object->turned_back = 1;
    return 0;
found:
    Object_SetMoveTarget((struct ObjectRuntime *)object, pos.x, pos.y, pos.z);
    object->script_cursor += 4;
    return 1;
}

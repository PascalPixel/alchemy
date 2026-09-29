/* 2026-09-29 alchemy permute: score 1795 to 1385 on the permuter's scorer
   (0 is exact); remaining 30 register-only, 18 operand, 6 reordered, 3
   inserted, 2 deleted. Kept rewrites: 1x swap commutative operands, 1x
   introduce a temporary. FAKEMATCH: the permuter's temporaries, register
   hints and swapped operand orders below only steer allocation and
   scheduling; no programmer would write them, so they stay tagged until a
   natural spelling replaces them. */
/* NONMATCHING: 408 / 404 bytes, 176 differing halfwords, 92 aligned edits.
 * 2026-09-26 bounded H1: explicit 32-byte work ownership for the two
 * positions, base distance and squared leash. Whole owner [0800dd70,0800df04)
 * includes three owned pool words. Script dispatch entry at 08013644 stores
 * this callback with the Thumb bit; callees consume object plus XYZ triples.
 * Full normalized difference: the frame stays 32 bytes and loop/call flow
 * stays equal, but r7 now owns the whole record instead of the candidate
 * position. Its derived pointers add copies and replace direct sp-relative
 * base/leash accesses. The intermediate unsquared-leash store disappears.
 * This rejects a unified work record as the source of the reference's spills.
 * Preserve this attempt; baseline remains in the parent commit. No adoption.
 */
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

struct WanderWork {
    s32 limit;
    s32 base;
    struct WanderPosition probe;
    struct WanderPosition pos;
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
    struct WanderWork work;
    s32 *args;
    s32 range;
    s32 radius;
    s32 angle;
    s32 tries;
    s32 dx;
    s32 dz;
    s32 *tmp;

    tmp = object->script;
    args = &tmp[object->step + 1];
    work.base = *args++;
    range = *args++;
    work.limit = *args / 0x10000;
    work.limit = work.limit * work.limit;
    tries = 0;
retry:
    tries++;
    if (tries <= 7) {
        work.pos.x = object->x;
        work.pos.y = object->y;
        work.pos.z = object->z;
        radius = work.base + Iwram_MulQ16(Random16(), range);
        angle = object->angle + (Random16() >> 2) - (Random16() >> 2);
        Vector_AddPolarOffset(radius, angle, &work.pos);
        if (ScriptObject_CheckOverlap(object, &work.pos) != 0)
            goto retry;
        if (Func_080120dc(object, &work.pos) != 0)
            goto retry;
        radius += 0x80000;
        work.probe.x = object->x;
        work.probe.y = object->y;
        work.probe.z = object->z;
        Vector_AddPolarOffset(radius, angle, &work.probe);
        work.probe.x = object->x;
        work.probe.y = object->y;
        work.probe.z = object->z;
        Vector_AddPolarOffset(radius, angle + 0x2000, &work.probe);
        if (Func_080120dc(object, &work.probe) != 0)
            goto retry;
        work.probe.x = object->x;
        work.probe.y = object->y;
        work.probe.z = object->z;
        Vector_AddPolarOffset(radius, angle - 0x2000, &work.probe);
        if (Func_080120dc(object, &work.probe) != 0)
            goto retry;
        dx = work.pos.x / 0x10000 - object->action;
        dz = work.pos.z / 0x10000 - object->unknown_66;
        if (dz * dz + dx * dx > work.limit)
            goto retry;
        goto found;
    }
    object->angle += 0x8000;
    object->unknown_5e = 1;
    return 0;
found:
    Object_SetMoveTarget(object, work.pos.x, work.pos.y, work.pos.z);
    object->step += 4;
    return 1;
}

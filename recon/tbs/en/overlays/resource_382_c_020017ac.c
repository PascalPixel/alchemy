/* NONMATCHING: complete 340-byte owner including its six-word pool;
 * candidate 340, 97 differing halfwords, 47 aligned edits (2026-09-26).
 * Three bounded hypotheses: independent branch locals gave 328/340 and
 * 91 edits; explicit coordinate updates gave 344/340 and 62 edits; reusing
 * the initial draw as the direction mask restored r5 and kept drift counts
 * in r0. Remaining: coordinate load scheduling, cmp #0 rather than #1 at
 * the animation choice, destructive mask xor, and low-register reloads.
 * No direct caller or equivalent sibling was found in registered evidence. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

/* Spawn a drifting effect near actor 19: bit 1 of flags picks its plane,
 * bit 0 its direction. */
void KuupuappuMura_SpawnDriftingEffect(s32 flags)
{
    struct FieldActor *leader;
    struct FieldActor *leaf;
    struct FieldSprite *sprite;
    s32 x;
    s32 z;
    s32 zero;

    leader = Engine_ActorGet(19);
    if (leader == NULL)
        return;
    x = Engine_RandomNext();
    z = Engine_RandomNext();
    x <<= 3;
    z <<= 3;
    x = (u32)x >> 16;
    z = (u32)z >> 16;
    x -= 4;
    z -= 4;
    x <<= 16;
    z <<= 16;
    x += leader->x.fixed;
    z += leader->z.fixed;
    leaf = Engine_ObjectCreate(0xac, x, leader->y.fixed, z);
    if (leaf == NULL)
        return;
    sprite = leaf->sprite;
    if ((Engine_RandomNext() & 1) == 1) {
        Engine_ObjectSetAnimation(leaf, 3);
        Engine_ObjectSetScript(leaf, (const s32 *)0x0200a8c4);
    } else {
        Engine_ObjectSetAnimation(leaf, 2);
        Engine_ObjectSetScript(leaf, (const s32 *)0x0200a8dc);
    }
    zero = 0;
    leaf->motion_flags = zero;
    if (flags & 2) {
        s32 cnt;

        cnt = Engine_MathModulo(Engine_RandomNext(), 10) + 5;
        /* FAKEMATCH: reuse the coordinate local for the direction mask. */
        x = 1;
        flags &= x;
        cnt += (flags ^ x) << 2;
        leaf->acceleration = (0x3332 * flags - 0x1999) * cnt;
        cnt = Engine_MathModulo(Engine_RandomNext(), 15) - 7;
        leaf->speed = 0x1999 * cnt;
        leaf->unknown_64 = zero;
    } else {
        s32 cnt;

        cnt = Engine_MathModulo(Engine_RandomNext(), 10) + 8;
        leaf->speed = (0x3332 * flags - 0x1999) * cnt;
        cnt = Engine_MathModulo(Engine_RandomNext(), 14) + 1;
        leaf->acceleration = 0x1999 * cnt;
        leaf->unknown_64 = 1;
    }
    leaf->update = (void (*)(union FieldObject *))0x02009755;
    sprite->flags = 0;
    sprite->priority = leader->sprite->priority;
}

/* NONMATCHING: 340 bytes, candidate 340, 104 differing halfwords
 * (2026-09-24). Single-overlay unit binding Engine_* at their import veneers.
 * Remaining: hand-written, 63 edits: the random offsets are shaped in the
 * wrong registers (the reference keeps the first draw in r5 and the leader in
 * r8), the coin test keeps cmp #1 in the reference but folds to cmp #0 here,
 * and the first drift branch forms n + 5 + ((flags ^ 1) << 2) in r5 instead
 * of the call result register. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

/* Throw a leaf from near actor 19: bit 1 of flags picks a drifting fall,
 * bit 0 its direction. */
void Local_020017ac(s32 flags)
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
    x = ((u32)(x << 3) >> 16) - 4;
    z = ((u32)(z << 3) >> 16) - 4;
    leaf = Engine_ObjectCreate(0xac, (x << 16) + leader->x.fixed, leader->y.fixed, (z << 16) + leader->z.fixed);
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
        x = Engine_MathModulo(Engine_RandomNext(), 10) + 5;
        flags &= 1;
        x += (flags ^ 1) << 2;
        leaf->acceleration = (0x3332 * flags - 0x1999) * x;
        z = Engine_MathModulo(Engine_RandomNext(), 15) - 7;
        leaf->speed = 0x1999 * z;
        leaf->unknown_64 = zero;
    } else {
        x = Engine_MathModulo(Engine_RandomNext(), 10) + 8;
        leaf->speed = (0x3332 * flags - 0x1999) * x;
        z = Engine_MathModulo(Engine_RandomNext(), 14) + 1;
        leaf->acceleration = 0x1999 * z;
        leaf->unknown_64 = 1;
    }
    leaf->update = (void (*)(union FieldObject *))0x02009755;
    sprite->flags = 0;
    sprite->priority = leader->sprite->priority;
}

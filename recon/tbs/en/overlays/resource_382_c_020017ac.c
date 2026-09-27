/* NONMATCHING: complete 340-byte owner including its six-word pool;
 * candidate 340, 99 differing halfwords, 48 aligned edits (2026-09-27 H1).
 * Original baseline: 340/97 halfwords/47 aligned edits.
 * Three bounded hypotheses: independent branch locals gave 328/340 and
 * 91 edits; explicit coordinate updates gave 344/340 and 62 edits; reusing
 * the initial draw as the direction mask restored r5 and kept drift counts
 * in r0. Remaining: coordinate load scheduling, cmp #0 rather than #1 at
 * the animation choice, destructive mask xor, and low-register reloads.
 * No direct caller or equivalent sibling was found in the earlier audit.
 * H1: exact installed SceneEffect_UpdateMotionWithDamping at 02001754
 * proves +30/+34 are horizontal/vertical rates, +64 is a signed plane
 * selector, and +38/+3c/+40 mirror position. Transfer that existing scalar
 * record via the engine actor view, and bind its callback by registered name.
 * Full-owner result 340/99/48: coordinate ancestry is unchanged; only the
 * callback publication/leader-sprite reload tail reorders. All six pool
 * words, frame and store widths remain fixed. New ownership evidence, not
 * an exact adoption. The original candidate remains in parent 43fe42fc0. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

/* The exact damping callback owns +30/+34 as drift rates and +64 as its
 * plane selector. Its scalar coordinates share the engine actor record. */
struct OverlayEffectMotion {
    u8 pad00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 pad14[28];
    s32 horizontal_rate;
    s32 vertical_rate;
    s32 shadow_x;
    s32 shadow_y;
    s32 shadow_z;
    u8 pad44[32];
    s16 mode;
};

LAYOUT_OFFSET_GUARD(Drift_HorizontalRate, struct OverlayEffectMotion, horizontal_rate, 0x30);
LAYOUT_OFFSET_GUARD(Drift_VerticalRate, struct OverlayEffectMotion, vertical_rate, 0x34);
LAYOUT_OFFSET_GUARD(Drift_Mode, struct OverlayEffectMotion, mode, 0x64);

union DriftingObject {
    struct FieldActor actor;
    struct OverlayEffectMotion motion;
};

void SceneEffect_UpdateMotionWithDamping(struct OverlayEffectMotion *effect);

/* Spawn a drifting effect near actor 19: bit 1 of flags picks its plane,
 * bit 0 its direction. */
void KuupuappuMura_SpawnDriftingEffect(s32 flags)
{
    union DriftingObject *leader;
    union DriftingObject *leaf;
    struct FieldSprite *sprite;
    s32 x;
    s32 z;
    s32 zero;

    leader = (union DriftingObject *)Engine_ActorGet(19);
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
    x += leader->motion.x;
    z += leader->motion.z;
    leaf = (union DriftingObject *)Engine_ObjectCreate(0xac, x, leader->motion.y, z);
    if (leaf == NULL)
        return;
    sprite = leaf->actor.sprite;
    if ((Engine_RandomNext() & 1) == 1) {
        Engine_ObjectSetAnimation(&leaf->actor, 3);
        Engine_ObjectSetScript(&leaf->actor, (const s32 *)0x0200a8c4);
    } else {
        Engine_ObjectSetAnimation(&leaf->actor, 2);
        Engine_ObjectSetScript(&leaf->actor, (const s32 *)0x0200a8dc);
    }
    zero = 0;
    leaf->actor.motion_flags = zero;
    if (flags & 2) {
        s32 cnt;

        cnt = Engine_MathModulo(Engine_RandomNext(), 10) + 5;
        /* FAKEMATCH: reuse the coordinate local for the direction mask. */
        x = 1;
        flags &= x;
        cnt += (flags ^ x) << 2;
        leaf->motion.vertical_rate = (0x3332 * flags - 0x1999) * cnt;
        cnt = Engine_MathModulo(Engine_RandomNext(), 15) - 7;
        leaf->motion.horizontal_rate = 0x1999 * cnt;
        leaf->motion.mode = zero;
    } else {
        s32 cnt;

        cnt = Engine_MathModulo(Engine_RandomNext(), 10) + 8;
        leaf->motion.horizontal_rate = (0x3332 * flags - 0x1999) * cnt;
        cnt = Engine_MathModulo(Engine_RandomNext(), 14) + 1;
        leaf->motion.vertical_rate = 0x1999 * cnt;
        leaf->motion.mode = 1;
    }
    leaf->actor.update = (void (*)(union FieldObject *))SceneEffect_UpdateMotionWithDamping;
    sprite->flags = 0;
    sprite->priority = leader->actor.sprite->priority;
}

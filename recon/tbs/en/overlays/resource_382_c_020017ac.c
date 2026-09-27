/* NONMATCHING canonical P2 branch-owned direction bias (2026-09-27):
 * 340/340 bytes, 31 differing halfwords, 29 aligned edits; topology equal.
 * P1 at 2d1b493f5 proved that a self-updating XOR/shift destination prevents
 * regmove from overwriting the dying mask. Isolating that producer as the
 * branch-local bias restores local r3 allocation and the exact sequence
 * flags -> r3; r3 ^= r5; r3 <<= 2, leaving r5 intact. This is admitted.
 * All six pool words/offsets, frame, saved roles and case-1 dispatch match.
 * Remaining: coordinate input-load overlap/reload base, sprite and zero
 * low-register reloads, negative-rate literal reloads, and callback-store
 * scheduling. No new function or alignment bytes. Do not collapse the
 * two-step bias into one expression or reuse the cross-block z owner.
 *
 * NONMATCHING P1 phased coordinate/direction bias (2026-09-27):
 * 344/340 bytes, 148 differing halfwords, 57 aligned edits; topology equal.
 * H3 baseline is preserved at 815f4834c. CSE's temporary XOR result was
 * folded into x/r5 by regmove at insn 199. Giving z the XOR then updating
 * z with its shift stops fixup_match_1: the destination is set again before
 * its death, so the pass inserts the flags copy and retains the r5 mask.
 * However z now spans two basic blocks: SI37 has 18 uses/29 insns/9 sets
 * and is globally allocated to r4, not locally to r3. The constructor gains
 * a copy, drift arithmetic schedules around r4, and the pool moves +4.
 * Case-1 dispatch, six pool values and saved roles remain. The exact r3
 * admission fails; one causal branch-local bias follow-up is justified.
 * Keep the two-stage XOR/shift producer, remove only its cross-block owner.
 * No new function or alignment bytes credited.
 *
 * NONMATCHING: H3 Object_Create boundary transfer (2026-09-27):
 * Complete output byte-identical to H2: 340 bytes, 97 differing halfwords,
 * 46 aligned edits. Exact same-area creators and MAKYURI_CHOJO/LAMP.C
 * use this wrapper, but here type 0xac is already a direct r0 constant;
 * the CSE constructor inputs retain the same x/z locals and y load.
 * There is no LAMP-like separate expensive type producer to remove.
 * The case-1 dispatch, frame, and all six pool words remain admitted.
 * Stop this boundary axis; coordinate scheduling and destructive direction
 * xor still require distinct evidence, not another wrapper spelling.
 * NONMATCHING: complete 340-byte owner including its six-word pool;
 * candidate 340, 97 differing halfwords, 46 aligned edits (2026-09-27 H2).
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
 * an exact adoption. The original candidate remains in parent 43fe42fc0.
 * H2: exact BattleFx_SpawnBurstParticle (BATTLE/EFFECT/RISING_SEQUENCE.C,
 * main 08092624) has the same random-variant -> animation/script -> drift
 * initialization boundary. Its own-ROM 0809264a/4c pair is cmp #1; bne.
 * Transfer its switch/case-1/default dispatch. The previously persistent
 * 02001808/0a pair now matches; precisely two byte positions change from H1,
 * both to reference bytes, with every other byte fixed. Topology is equal.
 * The complete 340-byte owner remains nonmatching: 97 halfwords, 46 aligned
 * edits, 56 wrong instructions. Freeze the admitted variant dispatch; the
 * remaining coordinate loads, direction-mask xor and callback publication
 * need distinct producer/consumer evidence, not pointer/zero permutations.
 * Both structural trials are closed. No exact sibling source was edited. */
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
    leaf = (union DriftingObject *)Object_Create(0xac, x, leader->motion.y, z);
    if (leaf == NULL)
        return;
    sprite = leaf->actor.sprite;
    /* The exact burst-particle sibling dispatches the animation/script
     * pair as one random variant, rather than as a boolean flag. */
    switch (Engine_RandomNext() & 1) {
    case 1:
        Engine_ObjectSetAnimation(&leaf->actor, 3);
        Engine_ObjectSetScript(&leaf->actor, (const s32 *)0x0200a8c4);
        break;
    default:
        Engine_ObjectSetAnimation(&leaf->actor, 2);
        Engine_ObjectSetScript(&leaf->actor, (const s32 *)0x0200a8dc);
        break;
    }
    zero = 0;
    leaf->actor.motion_flags = zero;
    if (flags & 2) {
        s32 cnt;
        s32 bias;

        cnt = Engine_MathModulo(Engine_RandomNext(), 10) + 5;
        /* FAKEMATCH: reuse the coordinate local for the direction mask. */
        x = 1;
        flags &= x;
        bias = flags ^ x;
        bias <<= 2;
        cnt += bias;
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

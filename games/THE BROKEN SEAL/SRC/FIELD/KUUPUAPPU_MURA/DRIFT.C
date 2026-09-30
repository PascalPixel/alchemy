/* Whole owner 020017ac..02001900: exact 340 bytes, including six pool words.
 * Coordinate inputs overlap through x/z loads; y begins after the x sum.
 * Explicit flag-store addresses recover the zero-copy order. Preparing the
 * callback value before the tagged address scope recovers literal/address/
 * publication order. Verified with the approved compiler on 2026-09-27.
 */
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

/* The two animation scripts a drifting effect may play. */
extern const s32 KuupuappuMura_DriftScriptA[];
extern const s32 KuupuappuMura_DriftScriptB[];

void SceneActor_ApplyActorZeroThenWait(s32 actor, s32 delay)
{
    Event_ShowMessage(actor, 0);
    Event_Wait(delay);
}

void SceneActor_ApplyActorCueThenWait(s32 actor, s32 cue, s32 delay)
{
    Actor_FaceEachOther(actor, cue, 0);
    Event_Wait(delay);
}

/*
 * Per-frame step for a projectile. Advance x by its rate and mirror it into
 * the shadow copy, then either follow the vertical rate or fall at a fixed
 * rate depending on the mode word, and finally decay both rates.
 */
void SceneEffect_UpdateMotionWithDamping(struct OverlayEffectMotion *effect)
{
    s32 horizontal_rate;
    s32 vertical_rate;

    effect->x += effect->horizontal_rate;
    effect->shadow_x = effect->x;

    if (effect->mode != 0) {
        effect->y += effect->vertical_rate;
        effect->shadow_y = effect->y;
    } else {
        effect->z += effect->vertical_rate;
        effect->shadow_z = effect->z;
        effect->y += 1024;
        effect->shadow_y = effect->y;
    }

    horizontal_rate = effect->horizontal_rate;
    effect->horizontal_rate = horizontal_rate - Math_Divide(horizontal_rate, 28);
    vertical_rate = effect->vertical_rate;
    effect->vertical_rate = vertical_rate - Math_Divide(vertical_rate, 28);
}

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

    leader = (union DriftingObject *)Object_GetById(19);
    if (leader == NULL)
        return;
    x = Engine_RandomNext();
    z = Engine_RandomNext();
    x <<= 3;
    x = (u32)x >> 16;
    z <<= 3;
    x -= 4;
    z = (u32)z >> 16;
    z -= 4;
    x <<= 16;
    z <<= 16;
    {
        s32 base_x = leader->motion.x;
        s32 base_z = leader->motion.z;
        s32 base_y;

        x += base_x;
        base_y = leader->motion.y;
        z += base_z;
        leaf = (union DriftingObject *)Object_Create(0xac, x, base_y, z);
    }
    if (leaf == NULL)
        return;
    sprite = leaf->actor.sprite;
    /* The exact burst-particle sibling dispatches the animation/script
     * pair as one random variant, rather than as a boolean flag. */
    switch (Engine_RandomNext() & 1) {
    case 1:
        Object_SetMode(&leaf->actor, 3);
        Engine_ObjectSetScript(&leaf->actor, KuupuappuMura_DriftScriptA);
        break;
    default:
        Object_SetMode(&leaf->actor, 2);
        Engine_ObjectSetScript(&leaf->actor, KuupuappuMura_DriftScriptB);
        break;
    }
    {
        u8 *motion_flags = &leaf->actor.motion_flags;

        zero = 0;
        *motion_flags = zero;
    }
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
    {
        void (*update)(union FieldObject *) =
            (void (*)(union FieldObject *))SceneEffect_UpdateMotionWithDamping;
        u8 *sprite_flags;

        /* FAKEMATCH: preserve the sprite-flags address scheduling boundary. */
        do {
            sprite_flags = &sprite->flags;
        } while (0);
        leaf->actor.update = update;
        *sprite_flags = 0;
    }
    sprite->priority = leader->actor.sprite->priority;
}

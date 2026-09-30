#include "TYPES.H"
#include "PARTY_STATE.H"
#include "FIXED_MATH.H"
#include "EFFECT_0809B11C.H"

/* Object updates of the phased radial particle sequence. */

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

extern u32 gFrameTick;
extern u32 BattleFx_PulseScales[];

void Object_Destroy();

void BattleFx_SetObjectAlternatingWords(u8 *object)
{
    u32 *table = BattleFx_PulseScales;
    u32 index = (gFrameTick >> 2) & 1;
    u32 value = index[table];
    *(u32 *)(object + 0x18) = value;
    *(u32 *)(object + 0x1C) = value;
}

void BattleFx_AdvanceObjectField6WithRamp(void *obj)
{
    u32 step;

    step = FIELD_AT_OFFSET(obj, s16 *, 0x64) * 0x50;
    FIELD_AT_OFFSET(obj, u16 *, 6) = (u16)(FIELD_AT_OFFSET(obj, u16 *, 6) + step + 0x1000);
    if (step < 0x1000U) {
        FIELD_AT_OFFSET(obj, s16 *, 0x64) = (s16)((u16)FIELD_AT_OFFSET(obj, s16 *, 0x64) + 1);
    }
}

void BattleFx_ShrinkObjectAndDestroySlow(void *obj)
{
    s32 scale;

    scale = FIELD_AT_OFFSET(obj, s32 *, 0x18) + 0xFFFFFE40;
    FIELD_AT_OFFSET(obj, s32 *, 0x1C) = (s32)(FIELD_AT_OFFSET(obj, s32 *, 0x1C) + 0xFFFFFE40);
    FIELD_AT_OFFSET(obj, u16 *, 6) = (u16)(FIELD_AT_OFFSET(obj, u16 *, 6) + 0x2000);
    FIELD_AT_OFFSET(obj, s32 *, 0x18) = scale;
    if (scale < 0x3000) {
        Object_Destroy();
    }
}

#if defined(TBS_EDITION_EN)
/* The other editions keep their code here in their scaffolds for now. */

/*
 * Moves a particle out from its origin to a random point on a ring,
 * waits there, then flies it to a random point near the current owner's
 * screen position before the slot is cleared. The particle's sprite takes
 * the owner sprite's draw priority on launch and the front priority while
 * it waits.
 */

struct EffectVector {
    s32 x;
    s32 y;
    s32 z;
};

struct EffectSprite {
    u8 unknown_00[9];
    s8 flags;                       /* 0x09; bits 2-3 are the draw priority */
    u8 unknown_0a[0x0e];
    s32 scale;                      /* 0x18 */
};

struct EffectOwner {
    u8 unknown_00[8];
    struct EffectVector position;   /* 0x08 */
    u8 unknown_14[0x3c];
    struct EffectSprite *sprite;    /* 0x50 */
};

extern u32 gFrameTick;

struct EffectOwner *Engine_ActorGet(s32 actor);
u32 BattleFx_HasReachedTarget(struct EffectSlot *effect);
void BattleFx_ClearOwnedSlot(struct EffectSlot *effect);
u32 Random16(void);
void Vector_AddPolarOffset(s32 magnitude, s32 angle, struct EffectVector *position);
void Camera_WorldToScreen(struct EffectVector *position);
void Audio_PlayCue(s32 cue);

void BattleEffect_UpdatePhasedRadialParticle(struct EffectSlot *effect)
{
    struct EffectOwner *owner;
    struct EffectVector position;
    s32 state;
    u8 priority;
    u8 flags;

    owner = Engine_ActorGet(gGameState.current_owner);
    state = effect->state;

    if (state == 0) {
        effect->x = effect->origin_x;
        effect->z = effect->origin_z;
        position.x = effect->x;
        position.z = effect->z;
        Vector_AddPolarOffset(
            0x780000,
            ((Random16() * 3 << 11) >> 16)
                - ((Random16() * 3 << 11) >> 16)
                + 0xc000,
            &position);
        effect->target_x = position.x;
        effect->target_z = position.z;
        effect->acceleration = 0x50000;
        effect->max_speed = 0x50000;
        effect->flag42 = state;
        effect->state++;

        priority = owner->sprite->flags & 0xc;
        flags = ((struct EffectSprite *)effect->object)->flags & -13;
        flags |= priority;
        ((struct EffectSprite *)effect->object)->flags = flags;
        effect->flags = 0;
        effect->age = 0;
        if ((gFrameTick & 1) != 0)
            Audio_PlayCue(134);
    } else if (state == 1) {
        if ((s16)effect->age == 3) {
            ((struct EffectSprite *)effect->object)->flags &= -13;
            effect->flags = 4;
        }
        if (BattleFx_HasReachedTarget(effect) == 0)
            effect->state--;
    } else if (state == 2) {
        if (BattleFx_HasReachedTarget(effect) == 0) {
            effect->origin_x = effect->x;
            effect->origin_z = effect->z;
            ((struct EffectSprite *)effect->object)->flags &= -13;
            effect->flags = 4;
            effect->render = 0;
            effect->state++;
            effect->callback_delay = 40;
        }
    } else if (state == 3) {
        effect->render = 1;
        effect->x = effect->origin_x;
        effect->z = effect->origin_z;
        position.x = owner->position.x;
        position.y = owner->position.y + 0x140000;
        position.z = owner->position.z;
        Camera_WorldToScreen(&position);
        Vector_AddPolarOffset(0x40000, Random16(), &position);
        effect->target_x = position.x;
        effect->target_z = position.z;
        effect->state++;
        if ((gFrameTick & 1) != 0)
            Audio_PlayCue(145);
    } else if (state == 4) {
        if (BattleFx_HasReachedTarget(effect) == 0)
            effect->state--;
    } else if (state == 5) {
        if (BattleFx_HasReachedTarget(effect) == 0)
            BattleFx_ClearOwnedSlot(effect);
    }
}
#endif

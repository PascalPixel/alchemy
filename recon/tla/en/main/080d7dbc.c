#include "TYPES.H"
#include "PARTY_STATE.H"
#include "FIXED_MATH.H"

/* Object updates of the phased radial particle sequence. */

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

extern u32 gFrameTick;
extern u32 BattleFx_PulseScales[];

void Object_Destroy();

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

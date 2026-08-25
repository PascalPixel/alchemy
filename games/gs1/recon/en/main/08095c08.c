#include "fixed_math.h"
#include "types.h"

struct EffectVector {
    s32 x;
    s32 y;
    s32 z;
};

struct EffectObjectFlags {
    u8 unknown_00[9];
    s8 flags;
};

struct EffectPositionSource {
    u8 unknown_00[8];
    struct EffectVector position;
    u8 unknown_14[60];
    struct EffectObjectFlags *linked_object;
};

struct PhasedParticleSlot {
    struct EffectObjectFlags *object;
    s32 x;
    s32 z;
    s32 target_x;
    s32 target_z;
    s32 origin_x;
    s32 origin_z;
    s32 speed;
    s32 max_speed;
    s32 acceleration;
    s32 scale_x;
    s32 scale_y;
    u16 heading;
    s16 max_turn_step;
    void *callback;
    u16 age;
    s16 callback_delay;
    u8 unknown_3c[4];
    s8 state;
    s8 flag41;
    s8 flag42;
    s8 update_motion;
    s8 render;
    s8 active;
    u8 random_value;
    u8 flags;
};

extern s32 Data_02000240[];
extern u32 Data_03001800;

struct EffectPositionSource *Object_GetById(s32 id);
u32 Random16(void);
void RotateVectorByMagnitude(
    s32 magnitude,
    s32 angle,
    struct EffectVector *position);
void NormalizeVector(struct EffectVector *position);
void Audio_PlayCue(s32 cue);
u32 EffectSlot_HasReachedTarget(struct PhasedParticleSlot *effect);
void Func_0809bb34(struct PhasedParticleSlot *effect);

#define BattleEffect_UpdatePhasedRadialParticle Func_08095c08

void BattleEffect_UpdatePhasedRadialParticle(struct PhasedParticleSlot *effect)
{
    struct EffectPositionSource *source;
    struct EffectVector position;
    s8 *state_pointer;
    s32 state;
    u8 linked_flags;
    u8 object_flags;

    source = Object_GetById(Data_02000240[125]);
    state_pointer = &effect->state;
    state = *state_pointer;

    if (state == 0) {
        effect->x = effect->origin_x;
        effect->z = effect->origin_z;
        position.x = effect->x;
        position.z = effect->z;
        RotateVectorByMagnitude(
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
        (*state_pointer)++;

        linked_flags = source->linked_object->flags & 0xc;
        object_flags = effect->object->flags & -13;
        object_flags |= linked_flags;
        effect->object->flags = object_flags;
        effect->flags = 0;
        effect->age = 0;
        if ((Data_03001800 & 1) != 0)
            Audio_PlayCue(134);
    } else if (state == 1) {
        if ((s16)effect->age == 3) {
            object_flags = effect->object->flags & -13;
            effect->object->flags = object_flags;
            effect->flags = 4;
        }
        if (EffectSlot_HasReachedTarget(effect) == 0)
            (*state_pointer)--;
    } else if (state == 2) {
        if (EffectSlot_HasReachedTarget(effect) == 0) {
            effect->origin_x = effect->x;
            effect->origin_z = effect->z;
            effect->object->flags &= -13;
            effect->flags = 4;
            effect->render = 0;
            (*state_pointer)++;
            effect->callback_delay = 40;
        }
    } else if (state == 3) {
        effect->render = 1;
        effect->x = effect->origin_x;
        effect->z = effect->origin_z;
        position.x = source->position.x;
        position.y = source->position.y + 0x140000;
        position.z = source->position.z;
        NormalizeVector(&position);
        RotateVectorByMagnitude(0x40000, Random16(), &position);
        effect->target_x = position.x;
        effect->target_z = position.z;
        (*state_pointer)++;
        if ((Data_03001800 & 1) != 0)
            Audio_PlayCue(145);
    } else if (state == 4) {
        if (EffectSlot_HasReachedTarget(effect) == 0)
            (*state_pointer)--;
    } else if (state == 5) {
        if (EffectSlot_HasReachedTarget(effect) == 0)
            Func_0809bb34(effect);
    }
}

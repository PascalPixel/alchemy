#include "FIXED_MATH.H"
#include "SYSTEM.H"
u32 BattleFx_HasReachedTarget(struct EffectSlot *);

struct EffectVector {
    s32 x;
    s32 y;
    s32 z;
};

struct EffectPositionSource {
    u8 unknown_00[8];
    struct EffectVector position;
};

extern s32 gGameState[];

struct EffectPositionSource *Object_GetById(s32 id);
void Vector_AddPolarOffset(
    s32 magnitude,
    s32 angle,
    struct EffectVector *position);
void Camera_WorldToScreen(struct EffectVector *position);
void Audio_PlayCue(s32 cue);

void BattleFx_UpdateRandomTargetParticle(struct EffectSlot *effect)
{
    struct EffectPositionSource *source;
    struct EffectVector position;
    s8 *state_pointer;
    s32 state;

    source = Object_GetById(gGameState[125]);
    state_pointer = &effect->state;
    state = *state_pointer;

    if (state == 0) {
        position.x = source->position.x;
        position.y = source->position.y + Random16() * 5 + 0xf0000;
        position.z = source->position.z;
        Camera_WorldToScreen(&position);
        Vector_AddPolarOffset(
            Random16() * 6 + 0x20000,
            Random16(),
            &position);

        effect->target_x = position.x;
        effect->target_z = position.z;
        effect->x = position.x;
        effect->z = position.z - 0x640000;
        effect->acceleration = 0x30000;
        effect->max_speed = Random16() * 3 + 0x30000;
        effect->scale_x = 0x10000;
        effect->scale_y = 0x10000;
        effect->flag42 = state;
        effect->flag41 = 1;
        (*state_pointer)++;
    } else if ((u8)(effect->state - 1) <= 1) {
        if (BattleFx_HasReachedTarget(effect) == 0) {
            position.x = effect->x;
            position.z = effect->z;
            Vector_AddPolarOffset(0xc0000, Random16(), &position);
            effect->target_x = position.x;
            effect->target_z = position.z;
            effect->flag41 = 0;
            effect->speed = 0x10000;
            effect->acceleration = 0;
            effect->max_speed = Random16() + 0x23333;
            effect->scale_x = 0x8000;
            effect->scale_y = 0x8000;
            Audio_PlayCue(143);

            if (*state_pointer == 1)
                (*state_pointer)--;
            else
                (*state_pointer)++;
            effect->callback_delay = 6;
        }
    } else if (state == 3) {
        BattleFx_ClearOwnedSlot(effect);
    }
}

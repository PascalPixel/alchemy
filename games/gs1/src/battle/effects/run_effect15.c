#include "types.h"
#include "scene.h"
#include "fixed_math.h"
#include "effect_0809b11c.h"
#include "object_lookup.h"

/* battle/effects/runtime/upd/update_spin_angle.c */
void BattleFx_UpdateSpinAngle(void *object)
{
    u32 angle_step;

    angle_step = FIELD_AT_OFFSET(object, s16 *, 0x64) * 0x50;
    FIELD_AT_OFFSET(object, u16 *, 6) = (u16)(FIELD_AT_OFFSET(object, u16 *, 6) + angle_step + 0x1000);
    if (angle_step < 0x1000U) {
        FIELD_AT_OFFSET(object, s16 *, 0x64) = (s16)((u16)FIELD_AT_OFFSET(object, s16 *, 0x64) + 1);
    }
}

struct DescendingObjectState {
  u8 padding_00[6];
  u16 angle;
  u8 padding_08[4];
  u32 scale;
  u8 padding_10[8];
  u32 primary_position;
  u32 secondary_position;
  u8 padding_20[0x34];
  s8 active;
};
void BattleFx_UpdateDescendingObject(struct DescendingObjectState *state)
{
  u32 next_position;
  state->secondary_position -= 0x280;
  next_position = state->primary_position - 0x280;
  state->angle = state->angle + 0x2000;
  state->scale = state->scale + 0x10000;
  state->primary_position = next_position;
  if ((s32)next_position < 0x3000) {
    state->active = 0;
  }
}

struct Output {
    s32 x;
    s32 y;
    s32 z;
};

struct EffectPositionSource {
    u8 padding00[8];
    struct Output position;
};

struct EffectSceneWork {
    u8 padding00[0x10];
    struct EffectPositionSource *position_source;
};

extern struct EffectSceneWork *Data_03001f30;
extern u32 Data_03001e40;

void NormalizeVector(struct Output *);
u32 Random16(void);
/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
#define Rand Random16
void RotateVectorByMagnitude(s32, s32, struct Output *);
void Audio_PlayCue(s32);

void BattleFx_UpdateRadialLaunch(struct EffectSlot *effect)
{
    struct Output position;
    struct EffectPositionSource *source;
    s32 state;
    u32 first_random;

    source = Data_03001f30->position_source;
    state = effect->state;

    if (state == 0) {
        position.x = source->position.x;
        position.y = source->position.y;
        position.z = source->position.z;
        NormalizeVector(&position);

        effect->x = position.x;
        effect->z = (s32)((u32)position.z + 0x80000);
        effect->origin_z = effect->z;
        effect->origin_x = effect->x;
        position.z = effect->origin_z;
        position.x = effect->origin_x;

        first_random = Rand();
        RotateVectorByMagnitude(
            0x780000,
            ((first_random << 13) >> 16)
                - ((Rand() << 13) >> 16)
                + 0xc000,
            &position);

        effect->target_x = position.x;
        effect->target_z = position.z;
        effect->acceleration = 0x50000;
        effect->max_speed = 0x50000;
        effect->flag42 = state;
        effect->state++;

        if ((Data_03001e40 & 2) != 0)
            Audio_PlayCue(0xf6);
    } else if (state == 1) {
        if (EffectSlot_HasReachedTarget(effect) == 0)
            effect->state--;
    } else if (state == 2) {
        if (EffectSlot_HasReachedTarget(effect) == 0)
            Func_0809bb34(effect);
    }
}

struct EffectObject {
    u8 pad00[100];
    s16 timer;
    u8 pad66[6];
    void (*callback)(void);
};


extern u8 Data_02000240[];

void Func_080916b0(void);
void Func_080933f8(s32, s32, s32, s32);
void Func_08097384(void);
void Func_080030f8(s32);
void Func_08092adc(s32, s32, s32);
void Func_080f9010(s32);
void Func_08009080(void *, s32);
void Func_08009240(void *, s32);
void Func_080091e0(void *, s32);
void Func_0809ba90(void *, s32, s32, s32);
u32 Func_08004458(void);
void Func_08009248(void *, s32);
void Func_0809748c(void);
void Func_08096b88(void);




void BattleFx_RunEffect15(void)
{
    u8 *scene = (u8 *)Data_03001f30;
    u8 *main_object = *(u8 **)(scene + 16);
    struct EffectObject *effect = (struct EffectObject *)main_object;
    struct Output position;
    u8 *particle;
    u8 *entry;
    u8 stopped;
    u32 index;

    Func_080916b0();
    Func_080933f8(-1, -1, -1, 0);
    Func_08097384();
    Func_080030f8(10);
    Func_08092adc(*(s16 *)(scene + 24), 0x4000, 0);
    Func_080030f8(30);
    *(void (**)(void))(main_object + 108) = Func_08096b88;
    Func_080f9010(0x83);
    Func_08009080(main_object, 28);
    Func_080030f8(40);
    Func_080f9010(0xdc);
    Func_08009240(main_object, 0);
    Func_08009080(main_object, 3);
    *(void (**)(void))(main_object + 108) = (void (*)(void))BattleFx_UpdateSpinAngle;
    effect->timer = 0;
    Func_080030f8(70);
    Func_080091e0(main_object, 0);
    *(u8 *)(main_object + 85) = 0;
    *(void (**)(void))(main_object + 108) = (void (*)(void))BattleFx_UpdateDescendingObject;
    *(u32 *)(main_object + 56) = 0x80000000;
    position.x = *(s32 *)(main_object + 8);
    position.y = *(s32 *)(main_object + 12);
    position.z = *(s32 *)(main_object + 16);
    NormalizeVector(&position);
    index = 0;
    particle = scene;
    particle += 88;
    do {
        s32 speed;
        Func_0809ba90(particle, 0x11c, position.x, position.z);
        EffectSlot_SetCallback((struct EffectSlot *)particle, BattleFx_UpdateRadialLaunch);
        EffectSlot_SetObjectMode((struct EffectSlot *)particle, 7);
        Func_08009248(*(void **)particle, (Func_08004458() * 7) >> 16);
        speed = (Func_08004458() >> 1) + 0x13333;
        *(s32 *)(particle + 44) = speed;
        *(s32 *)(particle + 40) = speed;
        index++;
        Func_080030f8(1);
        particle += 72;
    } while (index <= 23);
    Func_080030f8(70);
    index = 0;
    entry = scene;
    stopped = 2;
    entry += 152;
    do {
        if (*(s8 *)(entry + 5) != 0) {
            entry[0] = stopped;
        }
        index++;
        entry += 72;
    } while (index <= 23);
    Func_080030f8(40);
    Func_0809748c();
    Func_080030f8(10);
}

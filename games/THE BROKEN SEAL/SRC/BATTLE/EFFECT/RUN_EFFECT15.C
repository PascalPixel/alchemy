#include "TYPES.H"
#include "SCENE.H"
#include "FIXED_MATH.H"
#include "EFFECT_0809B11C.H"
#include "OBJECT_LOOKUP.H"

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
            BattleFx_ClearOwnedSlot(effect);
    }
}

struct EffectObject {
    u8 pad00[100];
    s16 timer;
    u8 pad66[6];
    void (*callback)(void);
};


extern u8 Data_02000240[];

void Battle_Reset(void);
void place_within_camera_bounds(s32, s32, s32, s32);
void BattleEffect_InitializeSharedScene(void);
void WaitFrames(s32);
void ObjectMotion_ArmCallback(s32, s32, s32);
void Audio_PlayCue(s32);
void Object_SetMode(void *, s32);
void Animation_ApplyChildValuesFar(void *, s32);
void ObjectDispatch_SetSingleChildField26Far(void *, s32);
void EffectSlot_Initialize(void *, s32, s32, s32);
u32 Random16(void);
void ObjectGroup_SetChildValueUnlessFifteenFar(void *, s32);
void BattleFx_PrepareBufferInterpolation(void);
void apply_random_child_values(void);




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

    Battle_Reset();
    place_within_camera_bounds(-1, -1, -1, 0);
    BattleEffect_InitializeSharedScene();
    WaitFrames(10);
    ObjectMotion_ArmCallback(*(s16 *)(scene + 24), 0x4000, 0);
    WaitFrames(30);
    *(void (**)(void))(main_object + 108) = apply_random_child_values;
    Audio_PlayCue(0x83);
    Object_SetMode(main_object, 28);
    WaitFrames(40);
    Audio_PlayCue(0xdc);
    Animation_ApplyChildValuesFar(main_object, 0);
    Object_SetMode(main_object, 3);
    *(void (**)(void))(main_object + 108) = (void (*)(void))BattleFx_UpdateSpinAngle;
    effect->timer = 0;
    WaitFrames(70);
    ObjectDispatch_SetSingleChildField26Far(main_object, 0);
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
        EffectSlot_Initialize(particle, 0x11c, position.x, position.z);
        EffectSlot_SetCallback((struct EffectSlot *)particle, BattleFx_UpdateRadialLaunch);
        EffectSlot_SetObjectMode((struct EffectSlot *)particle, 7);
        ObjectGroup_SetChildValueUnlessFifteenFar(*(void **)particle, (Random16() * 7) >> 16);
        speed = (Random16() >> 1) + 0x13333;
        *(s32 *)(particle + 44) = speed;
        *(s32 *)(particle + 40) = speed;
        index++;
        WaitFrames(1);
        particle += 72;
    } while (index <= 23);
    WaitFrames(70);
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
    WaitFrames(40);
    BattleFx_PrepareBufferInterpolation();
    WaitFrames(10);
}

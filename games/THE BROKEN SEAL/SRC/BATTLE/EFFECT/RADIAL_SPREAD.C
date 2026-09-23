#include "TYPES.H"
#include "FIXED_MATH.H"
#include "EFFECT_0809B11C.H"

/* Object updates of the radial spread page effect. */

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

struct Triple08095fcc {
    s32 x;
    s32 y;
    s32 z;
};

struct Object08095fcc {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[80];
    u16 timer;
    s16 angle;
};

struct Output_08096048 {
    s32 x;
    s32 y;
    s32 z;
};

struct PositionSource_08096048 {
    u8 padding00[8];
    struct Output_08096048 position;
};

extern s32 Data_02000240[];
#define PARTY_STATE Data_02000240
extern u32 Data_03001800;

s32 Object_GetById(u32);
u32 Random16(void);
/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
#define Rand Random16
void RotateVectorByMagnitude(s32, s32, void *);
void NormalizeVector(void *);
void Audio_PlayCue(s32);
void Object_Destroy();

void BattleFx_ShrinkObjectAndDestroyFast(void *obj)
{
    s32 scale;

    scale = FIELD_AT_OFFSET(obj, s32 *, 0x18) + 0xFFFFFC00;
    FIELD_AT_OFFSET(obj, s32 *, 0x1C) = (s32)(FIELD_AT_OFFSET(obj, s32 *, 0x1C) + 0xFFFFFC00);
    FIELD_AT_OFFSET(obj, u16 *, 6) = (u16)(FIELD_AT_OFFSET(obj, u16 *, 6) + 0x2000);
    FIELD_AT_OFFSET(obj, s32 *, 0x18) = scale;
    if (scale < 0x3000) {
        Object_Destroy();
    }
}

void BattleFx_UpdateDescendingOrbitObject(struct Object08095fcc *arg)
{
    struct Triple08095fcc local;
    struct Object08095fcc *other;
    s32 raw;
    s16 value;
    s32 y;

    other = (struct Object08095fcc *)Object_GetById(PARTY_STATE[125]);
    raw = arg->timer - 1;
    arg->timer = raw;
    value = arg->timer;
    local.x = other->x;
    local.z = other->z;
    RotateVectorByMagnitude(value * 0x6666,
                  (value << 11) + arg->angle,
                  &local);
    arg->x = local.x;
    arg->z = local.z;
    y = arg->y + 0xFFFF0000;
    arg->y = y;
    if (y < other->y + 0x140000)
        Object_Destroy(arg);
}

void BattleFx_UpdateRadialSpread(struct EffectSlot *effect)
{
    struct Output_08096048 position;
    struct PositionSource_08096048 *source;
    s32 state;
    u32 random;

    source = (struct PositionSource_08096048 *)
        Object_GetById(PARTY_STATE[125]);
    state = effect->state;

    if (state == 0) {
        position.x = source->position.x;
        position.y = source->position.y;
        position.z = source->position.z;

        random = Rand() * 10 + 0xa0000;
        RotateVectorByMagnitude(
            random,
            Rand(),
            &position);
        NormalizeVector(&position);

        effect->origin_x = position.x;
        effect->origin_z = position.z;
        effect->x = position.x;
        effect->z = position.z;
        position.x = effect->x;
        position.z = effect->z;

        RotateVectorByMagnitude(0x780000, 0xc000, &position);
        effect->target_x = position.x;
        effect->target_z = position.z;
        effect->acceleration = 0x10000;
        effect->max_speed = 0x50000;
        effect->flag42 = state;
        effect->state++;

        if ((Data_03001800 & 1) != 0)
            Audio_PlayCue(0x90);
    } else if (state == 1) {
        if (EffectSlot_HasReachedTarget(effect) == 0)
            effect->state--;
    } else if (state == 2) {
        if (EffectSlot_HasReachedTarget(effect) == 0)
            BattleFx_ClearOwnedSlot(effect);
    }
}

#include "FIXED_MATH.H"
#include "TYPES.H"
#include "OBJECT_EFX.H"
#include "SOUND_IDS.H"
#include "SYSTEM.H"

struct EffectPosition {
    s32 x;
    s32 y;
    s32 z;
};

struct EffectTarget {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
};

struct EffectRuntime {
    s32 angle;
    u8 unknown_04[12];
    struct EffectTarget *target;
};

struct RadialBurstEffect {
    u8 unknown_00[12];
    s32 x;
    s32 z;
    s32 source_x;
    s32 source_z;
    u8 unknown_1c[4];
    s32 velocity_x;
    s32 velocity_z;
    u8 unknown_28[10];
    u16 scale;
    u8 unknown_34[12];
    s8 state;
    u8 unknown_41;
    u8 enabled;
};

extern struct EffectRuntime *gEffectWork;

/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
void Vector_AddPolarOffset(s32 magnitude, s32 angle, struct EffectPosition *output);
void Camera_WorldToScreen(struct EffectPosition *value);
s32 BattleFx_HasReachedTarget(struct RadialBurstEffect *effect);
void BattleFx_ClearOwnedSlot(struct RadialBurstEffect *effect);

struct ImpactPosition {
    s32 x;
    s32 y;
    s32 z;
};

struct ImpactSource {
    u8 reserved_00[8];
    struct ImpactPosition position;
};

struct ImpactObject {
    u8 reserved_00[0x30];
    s32 field_30;
    s32 field_34;
    u8 reserved_38[0x1d];
    u8 mode_55;
    u8 reserved_56[8];
    u16 field_5e;
};

void Audio_PlayCue(s32 cue);
struct ImpactObject *Object_Spawn(s32 kind, s32 x, s32 y, s32 z);
void ObjectDispatch_InitializeFar(struct ImpactObject *object, const void *callback);
void Motion_SetTargetPositionFromMagnitudeAngle(
    struct ImpactObject *object, s32 magnitude, s32 angle);

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))
s32 RunBattleEffect04();

void BattleFx_UpdateRadialBurst(struct RadialBurstEffect *effect)
{
    struct EffectRuntime *runtime = gEffectWork;
    struct EffectPosition value;
    s32 state;

again:
    state = effect->state;
    if (state == 0) {
        value.x = effect->source_x;
        value.z = effect->source_z;
        Vector_AddPolarOffset(0x190000, (u16)Random16(), &value);
        effect->x = value.x;
        effect->z = value.z;
        effect->velocity_z = 0x30000;
        effect->velocity_x = 0x30000;
        effect->enabled = 0;
        effect->state++;
        return;
    }

    if (state == 1) {
        if (BattleFx_HasReachedTarget(effect) == 0) {
            effect->state++;
            goto again;
        }
        return;
    }

    if (state == 2) {
        struct EffectTarget *target = runtime->target;

        value.x = target->x;
        value.y = target->y + 0x100000;
        value.z = target->z;
        Vector_AddPolarOffset(0x80000, runtime->angle, &value);
        Camera_WorldToScreen(&value);
        Vector_AddPolarOffset(0x40000, Random16(), &value);
        effect->x = value.x;
        effect->z = value.z;
        effect->scale = 0x800;
        effect->enabled = 1;
        effect->state++;
        return;
    }

    if (state == 3 && BattleFx_HasReachedTarget(effect) == 0)
        BattleFx_ClearOwnedSlot(effect);
}

/* Plays the heavy impact cue and spawns effect object 0x11b 32 units above
 * the source, then twelve 0x11d fragments at the source, each sent a random
 * distance at a random angle. */
s32 SpawnHeavyImpactEffect(struct ImpactSource *source)
{
    struct ImpactPosition position;
    struct ImpactObject *object;
    struct ImpactObject *fragment;
    s32 i;

    Audio_PlayCue(SOUND_HEAVY_IMPACT);
    position.x = source->position.x;
    position.y = source->position.y;
    position.z = source->position.z;
    object = Object_Spawn(0x11b, position.x, position.y - 0x200000, position.z);
    if (object != NULL) {
        object->mode_55 = 0;
        object->field_5e = 20;
        ObjectDispatch_InitializeFar(object, BattleFx_CommonParticleScript);
    }

    for (i = 0; i < 12; i++) {
        fragment = Object_Spawn(0x11d, position.x, position.y, position.z);
        if (fragment != NULL) {
            ObjectDispatch_InitializeFar(fragment, &BattleFx_FragmentScript);
            fragment->field_30 = Random16() + 0x10000;
            fragment->field_34 = 0x10000;
            fragment->mode_55 = 0;
            Motion_SetTargetPositionFromMagnitudeAngle(
                fragment, Random16() * 24 + 0x80000, Random16());
        }
    }
    return 0;
}

void BattleFx_CallEffect04(void)
{
    RunBattleEffect04();
}

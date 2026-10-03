#include "FIXED_MATH.H"
#include "OBJDISP.H"
#include "TYPES.H"
#include "OBJECT_RUNTIME.H"
#include "MOTION_OBJECT.H"
#include "FX_SCENE.H"
#include "OBJECT_EFX.H"
#include "SOUND_IDS.H"
#include "SYSTEM.H"

struct EffectPosition {
    s32 x;
    s32 y;
    s32 z;
};

extern struct BattleFxScene *gEffectWork;

/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
void Vector_AddPolarOffset(s32 magnitude, s32 angle, struct EffectPosition *output);
void Camera_WorldToScreen(struct EffectPosition *value);

void BattleFx_ClearOwnedSlot(struct EffectSlot *effect);

struct ImpactPosition {
    s32 x;
    s32 y;
    s32 z;
};

void Audio_PlayCue(s32 cue);
struct ObjectRuntime *Object_Spawn(s32 kind, s32 x, s32 y, s32 z);
void Motion_SetTargetPositionFromMagnitudeAngle(
    struct ObjectRuntime *object, s32 magnitude, s32 angle);

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))
s32 RunBattleEffect04();

void BattleFx_UpdateRadialBurst(struct EffectSlot *effect)
{
    struct BattleFxScene *runtime = gEffectWork;
    struct EffectPosition value;
    s32 state;

again:
    state = effect->state;
    if (state == 0) {
        value.x = effect->origin_x;
        value.z = effect->origin_z;
        Vector_AddPolarOffset(0x190000, (u16)Random16(), &value);
        effect->target_x = value.x;
        effect->target_z = value.z;
        effect->acceleration = 0x30000;
        effect->max_speed = 0x30000;
        effect->flag42 = 0;
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
        struct MotionObject *target = runtime->main_object;

        value.x = target->x;
        value.y = target->y + 0x100000;
        value.z = target->z;
        Vector_AddPolarOffset(0x80000, runtime->angle, &value);
        Camera_WorldToScreen(&value);
        Vector_AddPolarOffset(0x40000, Random16(), &value);
        effect->target_x = value.x;
        effect->target_z = value.z;
        effect->max_turn_step = 0x800;
        effect->flag42 = 1;
        effect->state++;
        return;
    }

    if (state == 3 && BattleFx_HasReachedTarget(effect) == 0)
        BattleFx_ClearOwnedSlot(effect);
}

/* Plays the heavy impact cue and spawns effect object 0x11b 32 units above
 * the source, then twelve 0x11d fragments at the source, each sent a random
 * distance at a random angle. */
s32 SpawnHeavyImpactEffect(struct MotionObject *source)
{
    struct ImpactPosition position;
    struct ObjectRuntime *object;
    struct ObjectRuntime *fragment;
    s32 i;

    Audio_PlayCue(SOUND_HEAVY_IMPACT);
    position.x = source->x;
    position.y = source->y;
    position.z = source->z;
    object = Object_Spawn(0x11b, position.x, position.y - 0x200000, position.z);
    if (object != NULL) {
        object->flags = 0;
        object->unknown_5e = 20;
        ObjectDispatch_InitializeFar((struct DispatchObject *)object, (u32)BattleFx_CommonParticleScript);
    }

    for (i = 0; i < 12; i++) {
        fragment = Object_Spawn(0x11d, position.x, position.y, position.z);
        if (fragment != NULL) {
            ObjectDispatch_InitializeFar((struct DispatchObject *)fragment, (u32)&BattleFx_FragmentScript);
            fragment->speed_limit = Random16() + 0x10000;
            fragment->acceleration = 0x10000;
            fragment->flags = 0;
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

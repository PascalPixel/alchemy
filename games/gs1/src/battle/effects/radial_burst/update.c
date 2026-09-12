#include "fixed_math.h"
#include "types.h"

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

extern struct EffectRuntime *Data_03001f30;

s32 Random16(void);
/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
#define Rand Random16
void RotateVectorByMagnitude(s32 magnitude, s32 angle, struct EffectPosition *output);
void NormalizeVector(struct EffectPosition *value);
s32 EffectSlot_HasReachedTarget(struct RadialBurstEffect *effect);
void Func_0809bb34(struct RadialBurstEffect *effect);

void UpdateRadialBurstEffect(struct RadialBurstEffect *effect)
{
    struct EffectRuntime *runtime = Data_03001f30;
    struct EffectPosition value;
    s32 state;

again:
    state = effect->state;
    if (state == 0) {
        value.x = effect->source_x;
        value.z = effect->source_z;
        RotateVectorByMagnitude(0x190000, (u16)Rand(), &value);
        effect->x = value.x;
        effect->z = value.z;
        effect->velocity_z = 0x30000;
        effect->velocity_x = 0x30000;
        effect->enabled = 0;
        effect->state++;
        return;
    }

    if (state == 1) {
        if (EffectSlot_HasReachedTarget(effect) == 0) {
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
        RotateVectorByMagnitude(0x80000, runtime->angle, &value);
        NormalizeVector(&value);
        RotateVectorByMagnitude(0x40000, Rand(), &value);
        effect->x = value.x;
        effect->z = value.z;
        effect->scale = 0x800;
        effect->enabled = 1;
        effect->state++;
        return;
    }

    if (state == 3 && EffectSlot_HasReachedTarget(effect) == 0)
        Func_0809bb34(effect);
}

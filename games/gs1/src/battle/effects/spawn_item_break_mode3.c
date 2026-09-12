#include "types.h"
#include "scene.h"
#include "sound_ids.h"
#include "fixed_math.h"

/* battle/effects/item_break/spawn_mode_3.c */
void Audio_PlayCue(s32);
void *Object_Spawn(s32, s32, s32, s32);
void Object_SetMode(void *, s32);
void WaitFrames(s32);

void *SpawnItemBreakEffectMode3(s32 x, s32 y, s32 z, s32 angle)
{
    u8 *obj;
    s32 v;

    Audio_PlayCue(SOUND_ITEM_BREAK);
    obj = Object_Spawn(215, x, y, z);
    if (obj != NULL) {
        *(s32 *)(obj + 0x18) = *(s32 *)(obj + 0x1C) = 0x4000;
        *(s32 *)(obj + 0x34) = *(s32 *)(obj + 0x30) = 0x30000;
        *(s8 *)(*(u8 **)(obj + 0x50) + 9) &= ~0xC;
        Object_SetMode(obj, 3);
        v = *(s32 *)(obj + 0x18);
        if (v < 0x10000) {
            do {
                v += 0x800;
                *(s32 *)(obj + 0x1C) = v;
                *(s32 *)(obj + 0x18) = v;
                *(u16 *)(obj + 6) += 0x2000;
                WaitFrames(1);
                v = *(s32 *)(obj + 0x18);
            } while (v <= 0xFFFF);
        }
        *(u16 *)(obj + 6) = (u16)angle;
    }
    return obj;
}

/* battle/effects/radial_burst/update.c */
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

extern struct EffectRuntime *gIw;

s32 Random16(void);
/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
#define Rand Random16
void RotateVectorByMagnitude(s32 magnitude, s32 angle, struct EffectPosition *output);
void NormalizeVector(struct EffectPosition *value);
s32 EffectSlot_HasReachedTarget(struct RadialBurstEffect *effect);

void UpdateRadialBurstEffect(struct RadialBurstEffect *effect)
{
    struct EffectRuntime *runtime = gIw;
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
        Battle_Do(effect);
}

/* battle/effects/call/call_effect_04.c */
void BattleFx_CallEffect04(void)
{
    Battle_Check();
}

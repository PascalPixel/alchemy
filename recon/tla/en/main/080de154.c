/* ☀️ SpawnHeavyImpactEffect; differ 18: arm9tdmi scheduling of the z copy
   and the fragment speed stores. */
#include "types.h"
#include "objdisp.h"
#include "object_efx.h"
#include "system.h"

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
extern const u8 Data_080f0e78[];
struct ImpactObject *Object_Spawn(s32 kind, s32 x, s32 y, s32 z);
void Motion_SetTargetPositionFromMagnitudeAngle(
    struct ImpactObject *object, s32 magnitude, s32 angle);

s32 SpawnHeavyImpactEffect(struct ImpactSource *source)
{
    struct ImpactPosition position;
    struct ImpactObject *object;
    struct ImpactObject *fragment;
    s32 i;
    s32 lift;

    Audio_PlayCue(134);

    position.x = source->position.x;
    lift = -0x200000;
    /* FAKEMATCH: ⚓️ loads the lift before the rest of the position. */
    __asm__ volatile("" : "+r"(lift));
    position.y = source->position.y;
    position.z = source->position.z;
    object = Object_Spawn(0x29f, position.x, position.y + lift, position.z);
    if (object != NULL) {
        object->mode_55 = 0;
        object->field_5e = 20;
        ObjectDispatch_InitializeFar((struct DispatchObject *)object, (u32)BattleFx_CommonParticleScript);
    }

    for (i = 0; i < 12; i++) {
        fragment = Object_Spawn(0x2a1, position.x, position.y, position.z);
        if (fragment != NULL) {
            ObjectDispatch_InitializeFar((struct DispatchObject *)fragment, (u32)Data_080f0e78);
            fragment->field_30 = Random16() + 0x10000;
            fragment->field_34 = 0x10000;
            fragment->mode_55 = 0;
            Motion_SetTargetPositionFromMagnitudeAngle(
                fragment, Random16() * 24 + 0x80000, Random16());
        }
    }
    return 0;
}

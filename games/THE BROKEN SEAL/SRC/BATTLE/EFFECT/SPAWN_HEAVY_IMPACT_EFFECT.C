#include "TYPES.H"
#include "OBJECT_EFX.H"
#include "SOUND_IDS.H"

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
void Object_SetCallback(struct ImpactObject *object, const void *callback);
u32 Random16(void);
void Motion_SetTargetPositionFromMagnitudeAngle(
    struct ImpactObject *object, s32 magnitude, s32 angle);

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
        Object_SetCallback(object, BattleFx_CommonParticleScript);
    }

    for (i = 0; i < 12; i++) {
        fragment = Object_Spawn(0x11d, position.x, position.y, position.z);
        if (fragment != NULL) {
            Object_SetCallback(fragment, &BattleFx_FragmentScript);
            fragment->field_30 = Random16() + 0x10000;
            fragment->field_34 = 0x10000;
            fragment->mode_55 = 0;
            Motion_SetTargetPositionFromMagnitudeAngle(
                fragment, Random16() * 24 + 0x80000, Random16());
        }
    }
    return 0;
}

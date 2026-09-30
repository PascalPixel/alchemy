#include "FIXED_MATH.H"
#include "TYPES.H"
#include "SCENE.H"
#include "OBJECT_EFX.H"
#include "SYSTEM.H"

/* battle/effects/particles/spawn_random_angle_triplet.c */
/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
s32 ObjectDispatch_InitializeFar(void *, s32);
void Motion_SetTargetPositionFromMagnitudeAngle(
    struct Object_08096bec *object, s32 magnitude, s32 angle);
void *Object_Spawn(s32, s32, s32, s32);

void BattleFx_UpdateDriftingFallObject(void *obj)
{
    s32 r;

    FIELD_AT_OFFSET(obj, s32 *, 0xC) = (s32)(FIELD_AT_OFFSET(obj, s32 *, 0xC) + 0xFFFFB334);
    r = Random16();
    FIELD_AT_OFFSET(obj, s32 *, 8) = (s32)(FIELD_AT_OFFSET(obj, s32 *, 8) + (r - Random16()));
    if ((s32)FIELD_AT_OFFSET(obj, s32 *, 0xC) <= (s32)FIELD_AT_OFFSET(obj, s32 *, 0x14)) {
        ObjectDispatch_InitializeFar(obj, BattleFx_CommonParticleScript);
    }
}

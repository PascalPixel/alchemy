#include "FIXED_MATH.H"
#include "TYPES.H"
#include "OBJECT_EFX.H"
#include "SYSTEM.H"

extern void *Object_Spawn(s32, s32, s32, s32);
extern void Motion_SetTargetPositionFromMagnitudeAngle(
    struct Object_08096bec *object, s32 magnitude, s32 angle);
extern void Object_SetMode(void *, s32);
extern void ObjectDispatch_InitializeFar(void *, void *);
extern void BattleFx_UpdateItemBreakFragment(void *);
/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
extern void Audio_PlayCue(s32);

void BattleFx_SnapScaleToFull(void *obj)
{
    s32 next;
    s32 scale;

    if (obj != NULL) {
        scale = FIELD_AT_OFFSET(obj, s32 *, 0x18);
        if (scale <= 0xFFFF) {
            do {
                next = scale + 0x1000;
                scale = next;
            } while (next <= 0xFFFF);
            FIELD_AT_OFFSET(obj, s32 *, 0x18) = next;
            FIELD_AT_OFFSET(obj, s32 *, 0x1C) = next;
        }
        Object_CommitPosition();
    }
}

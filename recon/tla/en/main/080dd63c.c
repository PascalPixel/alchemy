/*
 * Canonical draft API context; no match or adoption is claimed.
 * API context measured 2026-10-02 with ordinary target flags:
 * all six original incomplete-context compile failures remain.
 * No missing view, declaration or physical symbol was supplied.
 * The shared void DispatchObject/u32 contract uses ordinary data casts;
 * every original matching-body and trial annotation is retained.
 */
#include "FIXED_MATH.H"
#include "OBJDISP.H"
#include "TYPES.H"
#include "OBJECT_EFX.H"
#include "SYSTEM.H"

extern void *Object_Spawn(s32, s32, s32, s32);
extern void Motion_SetTargetPositionFromMagnitudeAngle(
    struct Object_08096bec *object, s32 magnitude, s32 angle);
extern void Object_SetMode(void *, s32);
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

#include "types.h"
#include "system.h"
#include "ram_buffer.h"
#include "motobj.h"
#include "objdisp.h"
#include "object_effect_scripts.h"

struct BattleFxScene { u8 pad00[16]; void *volatile mainObject; };

void BattleEffect_InitializeSharedScene(void);
void *BattleFx_StartItemBreak(void *object);
void BattleFx_SnapScaleToFull(struct MotionObject *object);
void Object_SetMode(void *object, s32 mode);
void BattleFx_PrepareBufferInterpolation(void);
void UpdateRisingParticleBurst(void *object);
extern void *Object_Spawn(s32, s32, s32, s32);
extern void Motion_SetTargetPositionFromMagnitudeAngle(void *object, s32 magnitude, s32 angle);
extern void BattleFx_UpdateItemBreakFragment(void *);
extern void Audio_PlayCue(s32);
s32 Object_CommitPosition();
extern void Object_Destroy(void *object);
#define SOUND_ITEM_BREAK 138
void *BattleFx_StartItemBreak(void *source)
{
    s32 angle;
    s32 fragment_height;
    s32 fragment_scale;
    s32 fragment_count;
    u32 vel;
    u32 rotation_jitter;
    s32 zero;
    void *parent;
    void *child;

    angle = (((struct MotionObject *)source)->angle + 0x2000) & 0xc000;
    parent = Object_Spawn(0x116, *(s32 *)((s8 *)source + 8),
                           ((struct MotionObject *)source)->y + 0x100000,
                           *(s32 *)((s8 *)source + 16));
    if (parent == 0)
        return 0;
    *(s32 *)((s8 *)parent + 0x1c) = 0x4000;
    *(s32 *)((s8 *)parent + 0x18) = 0x4000;
    *(s32 *)((s8 *)parent + 0x6c) = (s32)BattleFx_UpdateItemBreakFragment;
    *(s32 *)((s8 *)parent + 0x30) = 0x20000;
    *(s32 *)((s8 *)parent + 0x34) = 0x20000;
    zero = 0;
    *(s8 *)((s8 *)parent + 0x55) = zero;
    Object_SetMode(parent, 3);
    Motion_SetTargetPositionFromMagnitudeAngle(parent, 0x100000, angle);

    fragment_count = 7;
    do {
        child = Object_Spawn(0x2a1, *(s32 *)((s8 *)source + 8),
                              ((struct MotionObject *)source)->y + 0x100000,
                              *(s32 *)((s8 *)source + 16));
        if (child != 0) {
            ObjectDispatch_InitializeFar((struct DispatchObject *)child, (u32)&BattleFx_FragmentScript);
            fragment_scale = Random16() + 0x10000;
            *(s32 *)((s8 *)child + 0x34) = 0x10000;
            *(s32 *)((s8 *)child + 0x30) = fragment_scale;
            *(s8 *)((s8 *)child + 0x55) = 2;
            *(s32 *)((s8 *)child + 0x48) = 0x51e;
            vel = Random16();
            *(s32 *)((s8 *)child + 0x28) = vel - Random16();
            fragment_height = Random16() * 0x18 + 0x80000;
            rotation_jitter = Random16();
            Motion_SetTargetPositionFromMagnitudeAngle(
                child, fragment_height,
                          ((rotation_jitter - Random16()) >> 3) +
                          ((struct MotionObject *)source)->angle);
        }
        fragment_count--;
    } while (fragment_count >= 0);
    Audio_PlayCue(SOUND_ITEM_BREAK);
    return parent;
}

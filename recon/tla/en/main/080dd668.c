/* Pin-free trial (tla-en): listing 95 instructions, draft 97, common 81, differ 30.
 * Earlier register-allocation notes describe the prior pinned form.
 */
#include "types.h"
#include "system.h"
#include "ram_buffer.h"
#include "motobj.h"
#include "objdisp.h"
#include "object_efx.h"

struct BattleFxScene { u8 pad00[16]; void *volatile main_object; };

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
void UpdateRisingParticleBurst(void *source)
{
    s32 count;
    s32 dist;
    u32 vel;
    void *child;
    /* FAKEMATCH: holds 0x10000 in sl across the burst loop as the ROM does */
    s32 unit;

    Audio_PlayCue(154);
    for (count = 30; count >= 0; count--) {
        ((struct MotionObject *)source)->y += 0x10000;
        ((struct MotionObject *)source)->angle += 0x2000;
        ((struct MotionObject *)source)->scale_x += -0x800;
        ((struct MotionObject *)source)->scale_y += -0x800;
        WaitFrames(1);
    }
    count = 7;
    unit = 0x10000;
    for (; count >= 0; count--) {
        child = Object_Spawn(0x2a1, *(s32 *)((s8 *)source + 8),
                             ((struct MotionObject *)source)->y,
                             *(s32 *)((s8 *)source + 16));
        if (child != 0) {
            ObjectDispatch_InitializeFar((struct DispatchObject *)child, (u32)&BattleFx_FragmentScript);
            {
                s32 speed = Random16();

                *(s32 *)((s8 *)child + 0x34) = unit;
                *(s32 *)((s8 *)child + 0x30) = speed + unit;
            }
            *(s8 *)((s8 *)child + 0x55) = 2;
            *(s32 *)((s8 *)child + 0x48) = 0xa3d;
            vel = Random16();
            *(s32 *)((s8 *)child + 0x28) = vel - Random16();
            dist = Random16() * 0x18;
            dist += 0x80000;
            Motion_SetTargetPositionFromMagnitudeAngle(child, dist, Random16());
        }
    }
    Audio_PlayCue(131);
    Object_Destroy(source);
}

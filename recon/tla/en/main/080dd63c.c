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
void BattleFx_SnapScaleToFull(struct MotionObject *obj)
{
    s32 next;
    s32 scale;

    if (obj != NULL) {
        scale = obj->scale_x;
        if (scale <= 0xFFFF) {
            do {
                next = scale + 0x1000;
                scale = next;
            } while (next <= 0xFFFF);
            obj->scale_x = next;
            obj->scale_y = next;
        }
        Object_CommitPosition();
    }
}

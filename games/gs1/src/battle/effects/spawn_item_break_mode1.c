#include "types.h"
#include "scene.h"
#include "sound_ids.h"

/* battle/effects/item_break/spawn_mode_1.c */
void Audio_PlayCue(s32);
void *Object_Spawn(s32, s32, s32, s32);
void Object_SetMode(void *, s32);
void WaitFrames(s32);

void *SpawnItemBreakEffectMode1(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    u8 *obj;
    s32 v;
    s32 zero;

    Audio_PlayCue(SOUND_ITEM_BREAK);
    obj = Object_Spawn(215, arg0, arg1, arg2);
    if (obj != NULL) {
        *(s32 *)(obj + 0x1C) = 0x4000;
        *(s32 *)(obj + 0x18) = 0x4000;
        *(s32 *)(obj + 0x30) = 0x30000;
        *(s32 *)(obj + 0x34) = 0x30000;
        zero = 0;
        *(s8 *)(obj + 0x5A) = zero;
        Object_SetMode(obj, 1);
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
        *(u16 *)(obj + 6) = (u16)arg3;
    }
    return obj;
}

/* battle/effects/obj/update_object_motion_scale_and_linked_angle.c */
void BattleFx_UpdateObjectMotionScaleAndLinkedAngle(void *arg0)
{
    u8 *a = arg0;

    *(volatile s32 *)(a + 0x08) += *(s32 *)(a + 0x44);
    *(volatile s32 *)(a + 0x0C) += *(s32 *)(a + 0x48);
    *(volatile s32 *)(a + 0x10) += *(s32 *)(a + 0x4C);
    *(volatile s32 *)(a + 0x18) += *(s32 *)(a + 0x30);
    *(volatile s32 *)(a + 0x1C) += *(s32 *)(a + 0x34);
    *(volatile u16 *)(*(u8 **)(a + 0x50) + 0x1E) += *(u16 *)(a + 0x64);
}

#include "types.h"
#include "sound_ids.h"

void Audio_PlayCue(s32);
void *Object_Spawn(s32, s32, s32, s32);
void Object_SetMode(void *, s32);
void WaitFrames(s32);

void *Func_08098a84(s32 x, s32 y, s32 z, s32 angle) {
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

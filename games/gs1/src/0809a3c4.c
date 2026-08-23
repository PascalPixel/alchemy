#include "types.h"
#include "sound_ids.h"

void Audio_PlayCue(s32);
void *Object_Spawn(s32, s32, s32, s32);
void Object_SetMode(void *, s32);
void WaitFrames(s32);

void *Func_0809a3c4(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
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
        *(u16 *)(obj + 6) = (u16) arg3;
    }
    return obj;
}

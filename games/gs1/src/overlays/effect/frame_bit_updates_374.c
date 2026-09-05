/* Contiguous unnamed state-owner run for resource_374. */

#include "types.h"
#define NULL ((void *)0)

s32 Func_02004930(s32, s32);
void Func_020049b0(s32, s32);
void Func_020047a4(s32);

#define SceneEffect_UpdateByFrameBits Func_02002334

void SceneEffect_UpdateByFrameBits(s32 no) {
    volatile s32 *p = (volatile s32 *)0x03001e40;
    if ((*p & 1) != 0) {
        s32 t = Func_02004930((u32)*p >> 1, 6);
        Func_020049b0(no, t);
    }
    if ((*p & 15) == 0) {
        Func_020047a4(no);
    }
}

#include "types.h"

#define SceneEffect_UpdateByFrameBit Func_02002370
#define NULL ((void *)0)

s32 Func_0200496c(s32, s32);
void Func_020049ec(s32, s32);

void SceneEffect_UpdateByFrameBit(s32 no) {
    volatile s32 *p = (volatile s32 *)0x03001e40;
    if ((*p & 1) != 0) {
        s32 t = Func_0200496c((u32)*p >> 1, 6);
        Func_020049ec(no, t);
    }
}

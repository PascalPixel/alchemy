#include "types.h"

#define OverlayObject_UpdateOnFrameBit1 Func_020022f8
#define NULL ((void *)0)

void Func_02004968(s32, s32);
void Func_02004972(s32, s32);
void Func_02004768(s32);

void OverlayObject_UpdateOnFrameBit1(s32 obj) {
    if ((*(volatile s32 *)0x03001e40 & 2) != 0) {
        Func_02004968(obj, 7);
    } else {
        Func_02004972(obj, 0);
    }
    if ((*(volatile s32 *)0x03001e40 & 15) == 0) {
        Func_02004768(obj);
    }
}

#include "types.h"

#define FieldScene_SelectActorModeFromInputBit Func_020041c4

extern void Func_0200866c(s32 arg0, s32 arg1);
extern void Func_02008674(s32 arg0, s32 arg1);

void FieldScene_SelectActorModeFromInputBit(s32 arg0)
{
    if ((*(u32 *)0x03001e40 >> 1) & 1) {
        Func_0200866c(arg0, 10);
    } else {
        Func_02008674(arg0, 9);
    }
}

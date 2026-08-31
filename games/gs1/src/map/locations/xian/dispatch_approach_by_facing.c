#include "types.h"

#define FieldScene_DispatchApproachByFacing Func_020012e0

extern s32 Func_020056d0(s32 flagId);
extern void Func_020056c0(void);
extern void Func_0200570c(void);
extern u8 *Func_020056ee(s32 slot);
extern u8 *Func_020056fe(s32 slot);
extern void Func_02001efc(void);
extern void Func_02001fe2(void);
extern void Func_0200247c(void);
extern void Func_02002350(s32 arg0);

void FieldScene_DispatchApproachByFacing(void)
{
    Func_020056c0();

    if (*(u16 *)(Func_020056ee(0) + 6) > (128 << 7)
        && *(u16 *)(Func_020056fe(0) + 6) < (192 << 8)) {
        Func_02001efc();
    } else {
        Func_02001fe2();
    }

    if (Func_020056d0(0x898) != 0) {
        Func_0200247c();
    } else {
        Func_02002350(0);
    }

    Func_0200570c();
}

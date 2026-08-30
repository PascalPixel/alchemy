#include "types.h"

#define FieldScene_SetFlag140AndFinishSequence Func_020040dc

extern void Func_020084b0();
extern void Func_02008638();
extern void Func_0200864c();
extern void Func_02008666();
extern void Func_0200845a();

void FieldScene_SetFlag140AndFinishSequence(s32 arg0, s32 arg1)
{
    u8 *globalCtx;

    Func_020084b0(160 << 1);
    Func_02008638(141, 1);
    globalCtx = *(u8 **)0x03001f30;
    Func_0200864c(arg0, arg1);
    globalCtx[0x23] = 0;
    Func_02008666();
    Func_0200864c(1);
    Func_0200845a(1);
}

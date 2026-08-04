#include "resource_3ac.h"

#include "types.h"

typedef struct {
    u8 filler0[6];
    u16 unk6;
} T;

extern T *Func_02000552(s32);
extern s32 Func_0200053c(s32);
extern void Func_020005a0(s32, s32);
extern void Func_02000566(void);
extern s32 Func_02000554(s32);
extern void Func_0200058e(s32);
extern void Func_020005a6(s32, s32);
extern void Func_0200059e(s32);
extern void Func_020005b6(s32, s32);
extern void Func_0200059a(void);

void Func_0200008c(void)
{
    T *p;

    p = Func_02000552(0);
    if ((u32)(p->unk6 - 0xa001) <= 0x3ffe) {
        if (Func_0200053c(0x941) != 0) {
            Func_020005a0(8, 17);
            return;
        }
    }
    Func_02000566();
    if (Func_02000554(0x941) != 0) {
        Func_0200058e(0x24fb);
        Func_020005a6(17, 0);
    } else {
        Func_0200059e(0x1bd0);
        Func_020005b6(17, 0);
    }
    Func_0200059a();
}

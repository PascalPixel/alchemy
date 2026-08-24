#include "resource_3ac.h"

#include "types.h"

typedef struct {
    u8 filler0[6];
    u16 unk6;
} T;

extern T *Func_0200083a(s32);
extern s32 Func_0200081a(s32);
extern void Func_02000878(s32, s32);
extern void Func_0200084e(void);
extern void Func_0200086c(s32);
extern void Func_0200087c(s32, s32);
extern void Func_02000868(void);
extern void Func_02000880(s32);
extern void Func_02000890(s32, s32);

void Func_02000374(void)
{
    T *p;
    s32 v;

    p = Func_0200083a(0);
    v = p->unk6;
    if (Func_0200081a(0x941) != 0) {
        if ((u32)(v - 0xa001) <= 0x3ffe) {
            Func_02000878(30, 15);
        } else {
            Func_0200084e();
            Func_0200086c(0x24f7);
            Func_0200087c(15, 0);
            Func_02000868();
        }
    } else {
        Func_02000880(0x1bce);
        Func_02000890(15, 0);
    }
}

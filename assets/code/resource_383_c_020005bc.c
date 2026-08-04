#include "types.h"

typedef struct {
    u8 filler0[6];
    u16 unk6;
} T;

extern T *Func_0200533e(s32);
extern void Func_0200530c(void);
extern void Func_020053cc(s32, s32);
extern void Func_020053ea(s32);
extern void Func_02005402(s32, s32);
extern void Func_02005370(s32, s32, s32);
extern void Func_020053a2(s32, s32);
extern void Func_020053be(s32, s32);
extern void Func_020053de(s32, s32);
extern void Func_02005418(s32);
extern void Func_020053d4(void);
extern s32 Data_0200e4a8[];
extern s32 Data_0200e4c0[];

void Func_020005bc(void)
{
    T *p;
    s16 *q;
    s32 v;
    s32 n;

    p = Func_0200533e(24);
    Func_0200530c();
    Func_020053cc(24, 2);
    Func_020053ea(0x12ac);
    Func_02005402(24, 0);
    Func_02005370(24, 0x40000, 0x20000);
    if ((u32)((p->unk6 & 0xf000) - 0x5000) <= 0x6000) {
        q = (s16 *)((u8 *)p + 100);
        v = *q;
        if (v <= 2) {
            Func_020053a2(24, Data_0200e4a8[v]);
            *(u16 *)q = *(u16 *)q + 1;
            goto clamp;
        }
    } else {
        q = (s16 *)((u8 *)p + 100);
        v = *q;
        if (v > 2) {
            Func_020053be(24, Data_0200e4a8[v]);
            *(u16 *)q = *(u16 *)q + 1;
            goto clamp;
        }
    }
    Func_020053de(24, Data_0200e4c0[v]);
    n = *(u16 *)q - 1;
    *(u16 *)q = n;
clamp:
    if (*q > 5) {
        n = 0;
        *(u16 *)q = n;
    }
    if (*q < 0) {
        n = 5;
        *(u16 *)q = n;
    }
    Func_02005418(24);
    Func_020053d4();
}

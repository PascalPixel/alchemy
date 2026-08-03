#include "resource_3ac.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

typedef struct {
    u8 filler0[6];
    u16 unk6;
} T;

extern T *Func_020006ea(s32);
extern void Func_0200071e(s32, s32);
extern s32 Func_020006de(s32);
extern void Func_020006fe(void);
extern void Func_0200071c(s32);
extern void Func_0200072c(s32, s32);
extern void Func_02000718(void);
extern void Func_02000716(void);
extern void Func_02000734(s32);
extern void Func_02000744(s32, s32);
extern void Func_02000730(void);

void Func_02000224(void)
{
    T *p;

    p = Func_020006ea(0);
    if ((u32)(p->unk6 - 0xa001) <= 0x3ffe) {
        Func_0200071e(25, 16);
    } else if (Func_020006de(0x941) != 0) {
        Func_020006fe();
        Func_0200071c(0x24f9);
        Func_0200072c(16, 0);
        Func_02000718();
    } else {
        Func_02000716();
        Func_02000734(0x1bcf);
        Func_02000744(16, 0);
        Func_02000730();
    }
}

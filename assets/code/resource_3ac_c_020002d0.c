#include "resource_3ac.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

typedef struct {
    u8 filler0[6];
    u16 unk6;
} T;

extern T *Func_02000796(s32);
extern s32 Func_02000776(s32);
extern void Func_020007d4(s32, s32);
extern void Func_020007aa(void);
extern void Func_020007c8(s32);
extern void Func_020007d8(s32, s32);
extern void Func_020007c4(void);
extern void Func_020007dc(s32);
extern void Func_020007ec(s32, s32);

void Func_020002d0(void)
{
    T *p;
    s32 v;

    p = Func_02000796(0);
    v = p->unk6;
    if (Func_02000776(0x941) != 0) {
        if ((u32)(v - 0xa001) <= 0x3ffe) {
            Func_020007d4(29, 14);
        } else {
            Func_020007aa();
            Func_020007c8(0x24f5);
            Func_020007d8(14, 0);
            Func_020007c4();
        }
    } else {
        Func_020007dc(0x1bcd);
        Func_020007ec(14, 0);
    }
}

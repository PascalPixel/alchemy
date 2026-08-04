#include "types.h"

extern void Func_020017ae(void);
extern void Func_020017c2(void);
extern void Func_020016d8(s32);
extern void Func_02001716(s32, s32, s32);
extern void Func_02001740(s32, s32, s32);
extern void Func_0200178c(s32, s32, s32);
extern void Func_02001798(s32, s32, s32);
extern void Func_020017a4(s32, s32, s32);
extern void Func_020017b0(s32, s32, s32);
extern void Func_0200177c(s32, s32, s32);
extern void Func_02001796(s32, s32, s32);
extern void Func_020017a6(s32, s32);
extern void Func_020017c4(s32);
extern void Func_020017d6(s32, s32, s32);
extern void Func_0200183a(void);
extern void Func_02001846(void);
extern void Func_02001824(s32);

void Func_020004b0(void)
{
    u8 *state;
    s32 t1;
    s32 h1;

    state = *(u8 **)0x03001ebc;
    *(s32 *)(state + 0x1C0) = 0x202;
    Func_020017ae();
    Func_020017c2();
    Func_020016d8(20);
    t1 = 0x10000;
    h1 = 0x8000;
    Func_02001716(8, t1, h1);
    Func_02001740(8, 0xA4, 0x141);
    Func_0200178c(8, 0xD000, 40);
    Func_02001798(8, 0xB000, 40);
    Func_020017a4(8, 0xD000, 40);
    Func_020017b0(8, 0x3000, 10);
    Func_0200177c(8, 0xA4, 0x14E);
    Func_02001796(8, 4, 40);
    Func_020017a6(8, 2);
    Func_020017c4(0x1E3A);
    Func_020017d6(8, 0, 20);
    Func_0200183a();
    Func_02001846();
    Func_02001824(10);
}

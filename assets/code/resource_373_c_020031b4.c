#include "types.h"

extern char Value_0000e666;
extern char Value_00001c90;
extern char Value_00000305;
extern s32 Data_0200e65c[];

extern void Func_02009174(void);
extern s32 Func_020091a2(s32);
extern void Func_02009110(s32, s32);
extern s32 Func_020091ae(s32);
extern void Func_0200911c(s32, s32);
extern s32 Func_020091ba(s32);
extern void Func_02009128(s32, s32);
extern void Func_02009228(s32, s32);
extern void Func_02009230(s32, s32);
extern s32 Func_02009238(s32, s32);
extern void Func_02009086(s32);
extern void Func_0200915e(s32, s32, s32);
extern void Func_02009208(s32, s32);
extern void Func_020090a6(s32);
extern s32 Func_02009216(s32, s32);
extern void Func_02009184(s32, s32, s32);
extern void Func_020090c2(s32);
extern s32 Func_02009242(s32, s32);
extern void Func_0200930e(s32, s32, s32);
extern void Func_020092b6(s32, s32);
extern s32 Func_0200930a(s32, s32, s32);
extern void Func_020092e8(s32);
extern void Func_0200930a_b(s32, s32, s32);
extern void Func_020092e4(s32, s32, s32);
extern void Func_02009314(s32, s32);
extern s32 Func_02009338(s32, s32, s32);
extern void Func_02009236(s32);
extern void Func_0200925a(void);

void Func_020031b4(void)
{
    s32 t;
    Func_02009174();
    Func_02009110(Func_020091a2(12), 0);
    Func_0200911c(Func_020091ae(13), 0);
    Func_02009128(Func_020091ba(14), 0);
    Func_02009228(12, 0);
    Func_02009230(13, 0);
    Func_02009238(14, 0);
    Func_02009086(20);
    Func_0200915e(0x20000, 0x20000, 0x10000);
    t = (s32)Data_0200e65c;
    Func_02009208(12, t);
    Func_020090a6(10);
    Func_02009216(13, t);
    Func_02009184(-1, -1, (s32)&Value_0000e666);
    Func_020090c2(20);
    Func_02009242(14, t);
    Func_0200930e(11, 0x100, 40);
    Func_020092b6(11, 2);
    Func_0200930a(11, 0xd000, 10);
    Func_020092e8((s32)&Value_00001c90);
    Func_0200930a_b(11, 0, 40);
    Func_020092e4(11, 0, 20);
    Func_02009314(11, 0);
    Func_02009338(11, 0x8000, 10);
    Func_02009236((s32)&Value_00000305);
    Func_0200925a();
}

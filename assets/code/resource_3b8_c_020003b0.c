#include "types.h"

extern u8 Data_0200c570[];
extern u8 Value_00002009;

extern s32 *Func_020047bc(s32);
extern void Func_020047a2(void);
extern void Func_020047de(s32, s32);
extern void Func_0200486c(s32);
extern void Func_02004884(s32, s32);
extern void Func_02004804(s32);
extern void Func_020047ca(s32);
extern void Func_0200487a(s32, s32);
extern void Func_020047d8(s32);
extern void Func_02004888(s32, s32);
extern void Func_020047e6(s32);
extern void Func_020048be(s32, s32);
extern void Func_020047f4(s32);
extern void Func_020048e0(s32, s32, s32);
extern void Func_020048b0(s32, s32);
extern void Func_0200480e(s32);
extern void Func_0200481c(s32);
extern void Func_020048cc(s32, s32);
extern void Func_0200482a(s32);
extern void Func_0200486a(s32, s32);
extern void Func_0200490a(s32, s32);
extern void Func_0200491e(s32, s32, s32);
extern void Func_0200484c(s32);
extern void Func_02004892(s32, s32);
extern void Func_0200486e(void);

void Func_020003b0(s32 a)
{
    s32 *p = Func_020047bc(a);

    Func_020047a2();
    Func_020047de(a, (s32)Data_0200c570);
    Func_0200486c((s32)&Value_00002009);
    Func_02004884(a, 0);
    Func_02004804(a);
    p[7] = 0x10000;
    p[6] = 0x10000;
    Func_020047ca(30);
    Func_0200487a(a, 2);
    Func_020047d8(30);
    Func_02004888(a, 2);
    Func_020047e6(60);
    Func_020048be(a, 0);
    Func_020047f4(20);
    Func_020048e0(a, 0x102, 60);
    Func_020048b0(a, 2);
    Func_0200480e(30);
    Func_020048be(a, 2);
    Func_0200481c(30);
    Func_020048cc(a, 2);
    Func_0200482a(30);
    Func_0200486a(a, (s32)Data_0200c570);
    Func_0200490a(a, 0);
    Func_0200491e(a, 0xe000, 0);
    Func_0200484c(10);
    p[7] = 0x10000;
    p[6] = 0x10000;
    Func_02004892(a, (s32)Data_0200c570);
    Func_0200486e();
}

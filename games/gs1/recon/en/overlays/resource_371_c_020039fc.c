#include "types.h"

#define Scene_RunScene371SequenceA Func_020039fc

extern u8 Data_00000800[];
extern u8 Data_00000c00[];
extern u8 Data_00000d00[];
extern u8 Data_00000e00[];
extern u8 Data_00000f00[];
extern u8 Data_00001000[];
void Func_02007b9e();
s32 Func_02007bd6();
s32 Func_02007c00();
void Func_02007c0c();
void Func_02007c26();
void Func_02007c70();
void Func_02007cbc();
void Func_02007d7a();
s32 Func_02007dd4();
void Func_02007e52();
void Func_02007e9a();
void Func_02007ee2();
void Func_02007f20();
void Func_02007f2c();
void Func_02007f40();
void Func_02007f5e();
void Func_02007f9c();
void Func_02007f9e();
void Func_02007fe2();
void Func_02008014();
void Func_02008026();
void Func_02008046();
void Func_02008078();
void Func_020080b0();
void Func_020080c0();
void Func_020080e4();
void Func_020080e8();
void Func_02008120();
void Func_02008180();
void Func_0200818c();
void Func_02008198();
void Func_020081a4();
void Func_02008318();
void Func_0200833a();
void Func_02008344();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

void Scene_RunScene371SequenceA(s32 a0)
{
    u32 i;
    s32 p8;
    s32 rec;
    s32 record;
    s32 base5_4000208;
    s32 v0;
    s32 base6_2002090;
    s32 v3;
    s32 v1;

    p8 = a0;
    rec = Value1(Func_02007bd6, 0x4000);
    Func_02007b9e(1);
    Call1(Func_02007cbc, 0x109);
    Func_02007c70();
    Value2(Func_02007c00, 0x200c7a6, rec);
    Call2(Func_02007c0c, 0x200c4ec, (rec + 0x1000));
    base5_4000208 = 0x4000208;
    base6_2002090 = 0x2002090;
    v0 = *(u16 *)base5_4000208;
    *(u16 *)base5_4000208 = base5_4000208;
    if (*(u16 *)base6_2002090 <= 31) {
        v3 = (((*(u16 *)base6_2002090 << 1) + *(u16 *)base6_2002090) << 2);
        *(u16 *)base6_2002090 += 1;
        *(s32 *)(((v3 + base6_2002090) + 4)) = ((s32)((s32)p8 << 5) + 0x200c4ac);
        *(s32 *)((((v3 + base6_2002090) + 4) + 4)) = 0x50001c0;
        *(s32 *)((((v3 + base6_2002090) + 4) + 4) + 4) = -0x7ffffff0;
    }
    *(u16 *)base5_4000208 = v0;
    v1 = *(u16 *)base5_4000208;
    *(u16 *)base5_4000208 = base5_4000208;
    if (*(u16 *)base6_2002090 <= 31) {
        *(s32 *)((((((*(u16 *)base6_2002090 << 1) + *(u16 *)base6_2002090) << 2) + base6_2002090) + 4)) = rec;
        v3 = ((((((*(u16 *)base6_2002090 << 1) + *(u16 *)base6_2002090) << 2) + base6_2002090) + 4) + 4);
        *(u16 *)base6_2002090 += 1;
        *(s32 *)(v3) = 0x6001000;
        *(s32 *)(v3 + 4) = -0x7bfffc00;
    }
    *(u16 *)base5_4000208 = v1;
    Call2(Func_02007c26, 0x200b8fd, 0xc80);
    Func_02007d7a();
    v1 = *(u16 *)base5_4000208;
    *(u16 *)base5_4000208 = base5_4000208;
    if (*(u16 *)base6_2002090 <= 31) {
        v3 = (((((*(u16 *)base6_2002090 << 1) + *(u16 *)base6_2002090) << 2) + base6_2002090) + 4);
        *(u16 *)base6_2002090 += 1;
        *(s32 *)(v3) = (rec + 0x3a80);
        *(s32 *)((v3 + 4)) = 0x6002000;
        *(s32 *)((v3 + 4) + 4) = -0x7bfffec0;
    }
    *(u16 *)base5_4000208 = v1;
    *(u8 *)(Func_02007dd4(*(s32 *)0x02000434) + 84) = 0;
    *(s32 *)((*(s32 *)0x03001ebc + 0x1c8)) = 16;
    Func_02007f2c();
    Func_02007f40();
    Func_02007f9e(246);
    *(u16 *)0x0200e7a0 = (s32)Data_00000e00;
    v1 = *(u16 *)base5_4000208;
    *(u16 *)base5_4000208 = base5_4000208;
    if (*(u16 *)base6_2002090 <= 31) {
        v3 = (((((*(u16 *)base6_2002090 << 1) + *(u16 *)base6_2002090) << 2) + base6_2002090) + 4);
        *(u16 *)base6_2002090 += 1;
        *(s32 *)(v3) = (rec + 0x3480);
        *(s32 *)((v3 + 4)) = 0x6002000;
        *(s32 *)((v3 + 4) + 4) = -0x7bfffec0;
    }
    *(u16 *)base5_4000208 = v1;
    Func_02007e52(2);
    *(u16 *)0x0200e7a0 = (s32)Data_00000d00;
    v1 = *(u16 *)base5_4000208;
    *(u16 *)base5_4000208 = base5_4000208;
    if (*(u16 *)base6_2002090 <= 31) {
        v3 = (((((*(u16 *)base6_2002090 << 1) + *(u16 *)base6_2002090) << 2) + base6_2002090) + 4);
        *(u16 *)base6_2002090 += 1;
        *(s32 *)(v3) = (rec + 0x2e80);
        *(s32 *)((v3 + 4)) = 0x6002000;
        *(s32 *)((v3 + 4) + 4) = -0x7bfffec0;
    }
    *(u16 *)base5_4000208 = v1;
    Func_02007e9a(2);
    *(u16 *)0x0200e7a0 = (s32)Data_00000c00;
    v1 = *(u16 *)base5_4000208;
    *(u16 *)base5_4000208 = base5_4000208;
    if (*(u16 *)base6_2002090 <= 31) {
        v3 = (((((*(u16 *)base6_2002090 << 1) + *(u16 *)base6_2002090) << 2) + base6_2002090) + 4);
        *(u16 *)base6_2002090 += 1;
        *(s32 *)(v3) = (rec + 0x2880);
        *(s32 *)((v3 + 4)) = 0x6002000;
        *(s32 *)((v3 + 4) + 4) = -0x7bfffec0;
    }
    *(u16 *)base5_4000208 = v1;
    Func_02007ee2(2);
    {
        s32 shown = 0xb00;
    
        *(u16 *)0x0200e7a0 = shown;
    }
    v1 = *(u16 *)base5_4000208;
    *(u16 *)base5_4000208 = base5_4000208;
    if (*(u16 *)base6_2002090 <= 31) {
        v3 = (((((*(u16 *)base6_2002090 << 1) + *(u16 *)base6_2002090) << 2) + base6_2002090) + 4);
        *(u16 *)base6_2002090 += 1;
        *(s32 *)(v3) = (rec + 0x2280);
        *(s32 *)((v3 + 4)) = 0x6002000;
        *(s32 *)((v3 + 4) + 4) = -0x7bfffec0;
    }
    *(u16 *)base5_4000208 = v1;
    Func_02007f20(2);
    {
        s32 shown = 0xa00;
    
        *(u16 *)0x0200e7a0 = shown;
    }
    v1 = *(u16 *)base5_4000208;
    *(u16 *)base5_4000208 = base5_4000208;
    if (*(u16 *)base6_2002090 <= 31) {
        v3 = (((((*(u16 *)base6_2002090 << 1) + *(u16 *)base6_2002090) << 2) + base6_2002090) + 4);
        *(u16 *)base6_2002090 += 1;
        *(s32 *)(v3) = (rec + 0x1c80);
        *(s32 *)((v3 + 4)) = 0x6002000;
        *(s32 *)((v3 + 4) + 4) = -0x7bfffec0;
    }
    *(u16 *)base5_4000208 = v1;
    Func_02007f5e(2);
    {
        s32 shown = 0x900;
    
        *(u16 *)0x0200e7a0 = shown;
    }
    v1 = *(u16 *)base5_4000208;
    *(u16 *)base5_4000208 = base5_4000208;
    if (*(u16 *)base6_2002090 <= 31) {
        v3 = (((((*(u16 *)base6_2002090 << 1) + *(u16 *)base6_2002090) << 2) + base6_2002090) + 4);
        *(u16 *)base6_2002090 += 1;
        *(s32 *)(v3) = (rec + 0x1680);
        *(s32 *)((v3 + 4)) = 0x6002000;
        *(s32 *)((v3 + 4) + 4) = -0x7bfffec0;
    }
    *(u16 *)base5_4000208 = v1;
    Func_02007f9c(2);
    *(u16 *)0x0200e7a0 = (s32)Data_00000800;
    v1 = *(u16 *)base5_4000208;
    *(u16 *)base5_4000208 = base5_4000208;
    if (*(u16 *)base6_2002090 <= 31) {
        v3 = (((((*(u16 *)base6_2002090 << 1) + *(u16 *)base6_2002090) << 2) + base6_2002090) + 4);
        *(u16 *)base6_2002090 += 1;
        *(s32 *)(v3) = (rec + 0x1080);
        *(s32 *)((v3 + 4)) = 0x6002000;
        *(s32 *)((v3 + 4) + 4) = -0x7bfffec0;
    }
    *(u16 *)base5_4000208 = v1;
    Func_02007fe2(140);
    v1 = *(u16 *)base5_4000208;
    *(u16 *)base5_4000208 = base5_4000208;
    if (*(u16 *)base6_2002090 <= 31) {
        v3 = (((((*(u16 *)base6_2002090 << 1) + *(u16 *)base6_2002090) << 2) + base6_2002090) + 4);
        *(u16 *)base6_2002090 += 1;
        *(s32 *)(v3) = (rec + 0x1680);
        *(s32 *)((v3 + 4)) = 0x6002000;
        *(s32 *)((v3 + 4) + 4) = -0x7bfffec0;
    }
    *(u16 *)base5_4000208 = v1;
    Func_02008014(4);
    v1 = *(u16 *)base5_4000208;
    *(u16 *)base5_4000208 = base5_4000208;
    if (*(u16 *)base6_2002090 <= 31) {
        v3 = (((((*(u16 *)base6_2002090 << 1) + *(u16 *)base6_2002090) << 2) + base6_2002090) + 4);
        *(u16 *)base6_2002090 += 1;
        *(s32 *)(v3) = (rec + 0x1c80);
        *(s32 *)((v3 + 4)) = 0x6002000;
        *(s32 *)((v3 + 4) + 4) = -0x7bfffec0;
    }
    *(u16 *)base5_4000208 = v1;
    Func_02008046(4);
    v1 = *(u16 *)base5_4000208;
    *(u16 *)base5_4000208 = base5_4000208;
    if (*(u16 *)base6_2002090 <= 31) {
        v3 = (((((*(u16 *)base6_2002090 << 1) + *(u16 *)base6_2002090) << 2) + base6_2002090) + 4);
        *(u16 *)base6_2002090 += 1;
        *(s32 *)(v3) = (rec + 0x2280);
        *(s32 *)((v3 + 4)) = 0x6002000;
        *(s32 *)((v3 + 4) + 4) = -0x7bfffec0;
    }
    *(u16 *)base5_4000208 = v1;
    Func_02008078(4);
    {
        s32 shown = 0x900;
    
        *(u16 *)0x0200e7a0 = shown;
    }
    v1 = *(u16 *)base5_4000208;
    *(u16 *)base5_4000208 = base5_4000208;
    if (*(u16 *)base6_2002090 <= 31) {
        v3 = (((((*(u16 *)base6_2002090 << 1) + *(u16 *)base6_2002090) << 2) + base6_2002090) + 4);
        *(u16 *)base6_2002090 += 1;
        *(s32 *)(v3) = (rec + 0x2880);
        *(s32 *)((v3 + 4)) = 0x6002000;
        *(s32 *)((v3 + 4) + 4) = -0x7bfffec0;
    }
    *(u16 *)base5_4000208 = v1;
    Func_020080b0(4);
    {
        s32 shown = 0xa00;
    
        *(u16 *)0x0200e7a0 = shown;
    }
    v1 = *(u16 *)base5_4000208;
    *(u16 *)base5_4000208 = base5_4000208;
    if (*(u16 *)base6_2002090 <= 31) {
        v3 = (((((*(u16 *)base6_2002090 << 1) + *(u16 *)base6_2002090) << 2) + base6_2002090) + 4);
        *(u16 *)base6_2002090 += 1;
        *(s32 *)(v3) = (rec + 0x2e80);
        *(s32 *)((v3 + 4)) = 0x6002000;
        *(s32 *)((v3 + 4) + 4) = -0x7bfffec0;
    }
    *(u16 *)base5_4000208 = v1;
    Func_020080e8(4);
    {
        s32 shown = 0xb00;
    
        *(u16 *)0x0200e7a0 = shown;
    }
    v1 = *(u16 *)base5_4000208;
    *(u16 *)base5_4000208 = base5_4000208;
    if (*(u16 *)base6_2002090 <= 31) {
        v3 = (((((*(u16 *)base6_2002090 << 1) + *(u16 *)base6_2002090) << 2) + base6_2002090) + 4);
        *(u16 *)base6_2002090 += 1;
        *(s32 *)(v3) = (rec + 0x3480);
        *(s32 *)((v3 + 4)) = 0x6002000;
        *(s32 *)((v3 + 4) + 4) = -0x7bfffec0;
    }
    *(u16 *)base5_4000208 = v1;
    Func_02008120(4);
    *(u16 *)0x0200e7a0 = (s32)Data_00000c00;
    v1 = *(u16 *)base5_4000208;
    *(u16 *)base5_4000208 = base5_4000208;
    if (*(u16 *)base6_2002090 <= 31) {
        v3 = (((((*(u16 *)base6_2002090 << 1) + *(u16 *)base6_2002090) << 2) + base6_2002090) + 4);
        *(u16 *)base6_2002090 += 1;
        *(s32 *)(v3) = (rec + 0x3a80);
        *(s32 *)((v3 + 4)) = 0x6002000;
        *(s32 *)((v3 + 4) + 4) = -0x7bfffec0;
    }
    *(u16 *)base5_4000208 = v1;
    Func_020080e4();
    Call2(Func_02008026, 0x200b95d, 0xc80);
    Func_02008344(141);
    *(u16 *)0x0200e7a0 = (s32)Data_00000d00;
    Func_02008180(4);
    *(u16 *)0x0200e7a0 = (s32)Data_00000e00;
    Func_0200818c(4);
    *(u16 *)0x0200e7a0 = (s32)Data_00000f00;
    Func_02008198(4);
    *(u16 *)0x0200e7a0 = (s32)Data_00001000;
    Func_020081a4(45);
    Func_02008318();
    Func_0200833a();
    Func_020080c0(rec);
}

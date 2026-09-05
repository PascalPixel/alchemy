#include "types.h"

#define FieldScene_RunExtendedPresentation Func_0200423c

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00000000[];
extern u8 Data_00000002[];
extern u8 Data_0200b6d1[];
extern u8 Data_0200d6a1[];
extern u8 Data_0200e088[];
extern u8 Data_0200e0d0[];
extern u8 Data_0200e0f4[];
extern u8 Data_0200e130[];
extern u8 Data_0200e760[];
extern u8 Data_0200e764[];
void Func_02004f60();
void Func_02004f8a();
void Func_02004fb6();
void Func_02009b8a();
void Func_02009b9a();
void Func_02009bae();
void Func_02009bbe();
void Func_02009bd4();
void Func_02009bea();
void Func_02009c00();
void Func_02009c16();
void Func_02009f22();
void Func_02009f7c();
void Func_02009f90();
void Func_02009fc2();
void Func_02009fd8();
void Func_0200a028();
void Func_0200a038();
void Func_0200a074();
u8 *Func_0200a084();
s32 Func_0200a08e();
void Func_0200a096();
void Func_0200a0a2();
void Func_0200a0ae();
void Func_0200a0b2();
void Func_0200a0b4();
void Func_0200a0ba();
void Func_0200a0be();
void Func_0200a0c6();
void Func_0200a0ce();
void Func_0200a0d2();
void Func_0200a0da();
void Func_0200a0e6();
void Func_0200a0ec();
u8 *Func_0200a0f2();
void Func_0200a0fa();
u8 *Func_0200a0fc();
void Func_0200a0fe();
void Func_0200a10a();
u8 *Func_0200a10e();
u8 *Func_0200a11a();
void Func_0200a120();
void Func_0200a122();
s32 Func_0200a128();
void Func_0200a134();
void Func_0200a152();
u8 *Func_0200a158();
u8 *Func_0200a164();
void Func_0200a16a();
s32 Func_0200a170();
void Func_0200a176();
void Func_0200a186();
u8 *Func_0200a194();
u8 *Func_0200a1a0();
s32 Func_0200a1ac();
void Func_0200a1ea();
void Func_0200a1f0();
void Func_0200a204();
void Func_0200a206();
void Func_0200a210();
void Func_0200a21a();
void Func_0200a226();
void Func_0200a228();
void Func_0200a234();
u8 *Func_0200a242();
s32 Func_0200a254();
void Func_0200a25a();
u8 *Func_0200a264();
void Func_0200a266();
void Func_0200a272();
s32 Func_0200a278();
s32 Func_0200a288();
void Func_0200a292();
s32 Func_0200a29e();
s32 Func_0200a2b4();
void Func_0200a2c8();
s32 Func_0200a2ca();
void Func_0200a2e0();
void Func_0200a304();
u8 *Func_0200a30c();
void Func_0200a32a();
void Func_0200a348();
void Func_0200a352();
void Func_0200a358();
s32 Func_0200a35a();
void Func_0200a35e();
s32 Func_0200a362();
s32 Func_0200a36a();
void Func_0200a36e();
void Func_0200a372();
u8 *Func_0200a3c0();
void Func_0200a3da();
s32 Func_0200a3e6();
s32 Func_0200a3ee();
s32 Func_0200a3f6();
void Func_0200a414();
void Func_0200a416();
void Func_0200a432();
void Func_0200a43e();
void Func_0200a45a();
void Func_0200a45c();
void Func_0200a470();
s32 Func_0200a474();
void Func_0200a48c();
void Func_0200a492();
void Func_0200a4a8();
void Func_0200a4be();
void Func_0200a4c4();
void Func_0200a4ce();
void Func_0200a4de();
void Func_0200a4e4();
s32 Func_0200a4e8();
void Func_0200a4ec();
s32 Func_0200a502();
void Func_0200a50e();
s32 Func_0200a522();
s32 Func_0200a53c();
void Func_0200a548();
s32 Func_0200a554();
s32 Func_0200a56c();
s32 Func_0200a570();
u8 *Func_0200a578();
void Func_0200a582();
void Func_0200a590();
void Func_0200a5a0();
void Func_0200a61e();
void Func_0200a65e();
void Func_0200a6dc();
void Func_0200a73c();
void Func_0200a73e();
void Func_0200a74a();

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

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunExtendedPresentation(void)
{
    u32 i;
    u16 p8[1];
    u8 *rec26;
    u8 *rec27;
    u8 *rec28;
    u8 *record;
    s32 base5_200e088;
    s32 none;
    s32 base5_200e0d0;
    s32 base5_200e0f4;
    s32 x_pos;
    s32 z_pos;
    s32 y_neg1;

    Call1((void (*)())Func_0200a11a, 19);
    Call1(Func_0200a122, 0x120);
    Call3(Func_02009f22, 0x30000, 0x30000, 0x10000);
    Call3(Func_0200a096, 0, 0x100, 0);
    Call3(Func_0200a0a2, 1, 0x100, 0);
    Call3(Func_0200a0ae, 2, 0x100, 0);
    Call3(Func_0200a0ba, 3, 0x100, 0);
    Call3(Func_0200a0c6, 21, 0x100, 0);
    Call3(Func_0200a0d2, 6, 0x100, 10);
    Call3(Func_0200a0ce, 0, 0xa000, 0);
    Call3(Func_0200a0da, 1, 0xa000, 0);
    Call3(Func_0200a0e6, 2, 0xa000, 0);
    Call3((void (*)())Func_0200a0f2, 3, 0xa000, 0);
    Call3(Func_0200a0fe, 21, 0xd000, 0);
    Call3(Func_0200a10a, 6, 0xd000, 0);
    Call3(Func_02009fc2, 0x20000, 0x20000, 0x10000);
    Func_0200a038(10);
    Call3(Func_02009fd8, 0x10000, 0x10000, 0x10000);
    Call4(Func_0200a16a, 0x1300000, 0x200000, 0xb40000, 1);
    Func_0200a176();
    record = Func_0200a084(24);
    *(s32 *)(record + 24) = 0x1999;
    record = Value1(Func_0200a08e, 25);
    *(s32 *)(record + 24) = 0x1999;
    base5_200e088 = (s32)Data_0200e088;
    Func_0200a0b2(24, base5_200e088);
    Func_0200a0ba(25, base5_200e088);
    Func_0200a228(145);
    Call3(Func_0200a028, 0x60000, 0x60000, 0x10000);
    Call2(Func_0200a1f0, 0x4063ff, 0);
    Func_0200a206(16);
    Func_02009f7c(20);
    Call2(Func_0200a204, 0x7fff, 0);
    Func_0200a21a(24);
    Func_02009f90(60);
    Func_0200a266(141);
    Call1(Func_0200a0b4, 0x236);
    record = Func_0200a0f2(24);
    *(s32 *)(record + 12) = -0x600000;
    record = Func_0200a0fc(25);
    *(s32 *)(record + 12) = -0x400000;
    Call2((void (*)())Func_0200a1a0, 26, 7);
    record = Func_0200a10e(26);
    Func_0200a074(record, 0);
    rec26 = Func_0200a11a(26);
    y_neg1 = -0x10000;
    *(s32 *)(rec26 + 28) = y_neg1;
    record = Value1(Func_0200a128, 24);
    none = 0;
    *(s32 *)(rec26 + 24) = *(s32 *)(record + 24);
    rec26[85] = none;
    x_pos = 0x1300000;
    *(s32 *)(rec26 + 8) = x_pos;
    *(s32 *)(rec26 + 12) = -0x200000;
    z_pos = 0x600000;
    *(s32 *)(rec26 + 16) = z_pos;
    Func_0200a1ea(27, 7);
    record = Func_0200a158(27);
    Func_0200a0be(record, 0);
    rec27 = Func_0200a164(27);
    *(s32 *)(rec27 + 28) = y_neg1;
    record = Value1(Func_0200a170, 24);
    *(s32 *)(rec27 + 24) = *(s32 *)(record + 24);
    rec27[85] = none;
    *(s32 *)(rec27 + 8) = x_pos;
    *(s32 *)(rec27 + 12) = none;
    *(s32 *)(rec27 + 16) = z_pos;
    Func_0200a226(28, 7);
    record = Func_0200a194(28);
    Func_0200a0fa(record, 0);
    rec28 = Func_0200a1a0(28);
    *(s32 *)(rec28 + 28) = y_neg1;
    record = Value1(Func_0200a1ac, 24);
    *(s32 *)(rec28 + 24) = *(s32 *)(record + 24);
    rec28[85] = none;
    *(s32 *)(rec28 + 8) = x_pos;
    *(s32 *)(rec28 + 12) = 0x200000;
    *(s32 *)(rec28 + 16) = z_pos;
    Call6(Func_0200a120, 102, 4, 74, 4, 18, 23);
    Call6(Func_0200a134, 39, 72, 11, 72, 16, 21);
    Call6(Func_0200a152, 19, 6, 3, 7, 22, 6);
    Call6((void (*)())Func_0200a164, 19, 6, 3, 7, 13, 6);
    Call6(Func_0200a176, 19, 6, 3, 7, 22, 13);
    Call6(Func_0200a186, 19, 6, 3, 7, 13, 13);
    Func_0200a0ec(1);
    record = Func_0200a242(8);
    *(s32 *)(record + 8) += -0x100000;
    Func_02009b8a();
    record = Value1(Func_0200a254, 9);
    *(s32 *)(record + 8) += -0x100000;
    Func_02009b9a();
    record = Func_0200a264(10);
    *(s32 *)(record + 8) += 0x100000;
    Func_02009bae();
    record = Value1(Func_0200a278, 11);
    *(s32 *)(record + 8) += 0x100000;
    Func_02009bbe();
    record = Value1(Func_0200a288, 0);
    *(s32 *)(record + 8) += 0x100000;
    *(s32 *)(record + 16) += 0x100000;
    Func_02009bd4();
    record = Value1(Func_0200a29e, 1);
    *(s32 *)(record + 8) += 0x100000;
    *(s32 *)(record + 16) += 0x100000;
    Func_02009bea();
    record = Value1(Func_0200a2b4, 2);
    *(s32 *)(record + 8) += 0x100000;
    *(s32 *)(record + 16) += 0x100000;
    Func_02009c00();
    record = Value1(Func_0200a2ca, 3);
    *(s32 *)(record + 8) += 0x100000;
    *(s32 *)(record + 16) += 0x100000;
    Func_02009c16();
    Call3(Func_0200a348, 21, 0xc40000, 0xdc0000);
    Func_0200a358(21, 5);
    Call3(Func_0200a35e, 6, 0xbc0000, 0x13c0000);
    Func_0200a36e(6, 5);
    record = Func_0200a30c(6);
    Func_0200a272(record, 0);
    Call3(Func_0200a292, 0x40000, 0x40000, 0x10000);
    Call2(Func_0200a45a, 0x4063ff, 0);
    Func_0200a470(120);
    base5_200e0d0 = (s32)Data_0200e0d0;
    Func_0200a352(24, base5_200e0d0);
    Value2(Func_0200a35a, 25, base5_200e0d0);
    Value2(Func_0200a362, 26, base5_200e0d0);
    Value2(Func_0200a36a, 27, base5_200e0d0);
    Func_0200a372(28, base5_200e0d0);
    Func_0200a210(120);
    Call3(Func_0200a2e0, 0x30000, 0x30000, 0x10000);
    Call2(Func_0200a4a8, 0x203210, 0);
    Func_0200a4be(120);
    Func_0200a234(120);
    Call3(Func_0200a304, 0x20000, 0x20000, 0x10000);
    Call2(Func_0200a4ce, 0x10000, 0);
    Func_0200a4e4(120);
    Func_0200a25a(120);
    Call3(Func_0200a32a, 0x10000, 0x10000, 0x10000);
    record = Func_0200a3c0(24);
    base5_200e0f4 = (s32)Data_0200e0f4;
    *(s32 *)(record + 28) = 0x51e;
    Value2(Func_0200a3e6, 25, base5_200e0f4);
    Value2(Func_0200a3ee, 26, base5_200e0f4);
    Value2(Func_0200a3f6, 27, base5_200e0f4);
    Func_0200a416(28, base5_200e0f4);
    Call1(Func_0200a56c, 0x121);
    Func_0200a48c(24, 15);
    Func_0200a3da(20);
    Call2(Func_0200a432, 24, (s32)Data_0200e130);
    Call1(Func_0200a2c8, (s32)Data_0200b6d1);
    Func_0200a492(2, 2, 20);
    Func_02004f60(2);
    Call3(Func_0200a4ec, 1, 0x6000, 20);
    Call2(Func_0200a50e, 1, 0x102);
    Func_0200a414(20);
    Func_0200a4c4(1, 2);
    Func_02004f8a(1);
    Call3(Func_0200a45c, 3, 0xcccc, 0x6666);
    Call3(Func_0200a4a8, 3, 0x146, 220);
    Func_0200a43e(40);
    Call2(Func_0200a548, 3, 0x102);
    Func_02004fb6(3);
    record = Value1(Func_0200a474, 0);
    record[98] = none;
    *(u8 *)((record + 98) + 1) = 1;
    *(s32 *)(record + 76) = *(s32 *)(record + 12);
    p8[0] = (u16)(s32)Data_00000000;
    {
        s32 shown = 0xa000;

        *(u16 *)(record + 6) = shown;
    }
    record = Value1(Func_0200a4e8, 1);
    record[98] = p8[0];
    *(u8 *)((record + 98) + 1) = 1;
    {
        s32 shown = 0xa000;
    
        *(u16 *)(record + 6) = shown;
    }
    *(s32 *)(record + 76) = *(s32 *)(record + 12);
    record = Value1(Func_0200a502, 2);
    record[98] = p8[0];
    *(u8 *)((record + 98) + 1) = 1;
    {
        s32 shown = 0xa000;
    
        *(u16 *)(record + 6) = shown;
    }
    *(s32 *)(record + 76) = *(s32 *)(record + 12);
    record = Value1(Func_0200a522, 3);
    record[98] = p8[0];
    *(u8 *)((record + 98) + 1) = 1;
    {
        s32 shown = 0xa000;
    
        *(u16 *)(record + 6) = shown;
    }
    *(s32 *)(record + 76) = *(s32 *)(record + 12);
    record = Value1(Func_0200a53c, 21);
    record[98] = p8[0];
    *(u8 *)((record + 98) + 1) = 1;
    *(s32 *)(record + 76) = *(s32 *)(record + 12);
    record = Value1(Func_0200a554, 6);
    record[98] = p8[0];
    *(u8 *)((record + 98) + 1) = 1;
    *(s32 *)(record + 76) = *(s32 *)(record + 12);
    *(u8 *)(Func_0200a56c(23) + 85) = p8[0];
    record = Func_0200a578(23);
    Func_0200a4de(record, 0);
    Func_0200a61e(23, 7);
    Func_0200a65e(23, 2);
    *(s32 *)Data_0200e764 = none;
    *(s32 *)Data_0200e760 = 240;
    Call2(Func_0200a45c, (s32)Data_0200d6a1, 0xc80);
    do {
        Func_0200a45a(1);
    } while (Value1(Func_0200a570, 0x237) == 0);
    Call1(Func_0200a582, 0x101);
    Func_0200a5a0(30);
    Call1(Func_0200a590, 0x11a);
    Func_0200a73c();
    Func_0200a6dc((s32)Data_00000002, 91);
    i = 0x7fff;
    *(u16 *)0x05000000 = i;
    *(s32 *)((*(u8 **)0x03001ebc + 0x1c8)) = 1;
    Func_0200a73e();
    Func_0200a74a();
}

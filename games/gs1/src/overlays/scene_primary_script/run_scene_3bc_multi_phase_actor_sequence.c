#include "types.h"

#define FieldScene_RunMultiPhaseActorSequence Func_02001474

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_0200d950[];
void Func_02005d9e();
void Func_02005e7e();
void Func_02005e98();
void Func_02005ea2();
void Func_02005ea8();
s32 Func_02005ef0();
s32 Func_02005ef4();
void Func_02005f0e();
s32 Func_02005f14();
void Func_02005f22();
void Func_02005f2c();
void Func_02005f30();
void Func_02005f36();
void Func_02005f3a();
void Func_02005f3e();
void Func_02005f5c();
s32 Func_02005f5e();
s32 Func_02005f66();
s32 Func_02005f6e();
void Func_02005f8a();
void Func_02005f8c();
void Func_02005f9a();
void Func_02005fa8();
s32 Func_02005fba();
s32 Func_02005fc8();
void Func_02005fca();
void Func_02005fcc();
s32 Func_02005fda();
void Func_02005fdc();
void Func_02005fee();
s32 Func_02005ff0();
void Func_02005ff2();
void Func_02006002();
void Func_02006014();
s32 Func_0200601c();
void Func_02006028();
void Func_0200602a();
void Func_02006034();
void Func_0200603c();
void Func_0200603e();
void Func_02006052();
void Func_02006056();
void Func_0200606a();
void Func_0200607c();
void Func_0200607e();
void Func_0200608a();
void Func_0200608e();
void Func_02006092();
void Func_02006098();
void Func_020060a6();
void Func_020060b4();
void Func_020060bc();
void Func_020060be();
void Func_020060ce();
void Func_020060d0();
void Func_020060d2();
void Func_020060e2();
void Func_020060e4();
void Func_020060e6();
void Func_020060ee();
s32 Func_020060fc();
void Func_02006106();
void Func_02006112();
void Func_02006114();
void Func_02006138();
void Func_0200613a();
void Func_02006140();
void Func_02006146();
void Func_02006156();
s32 Func_0200615a();
void Func_02006168();
void Func_02006174();
void Func_02006186();
void Func_0200618a();
void Func_020061a4();
void Func_020061b0();
void Func_020061b4();
void Func_020061ba();
void Func_020061bc();
void Func_020061c0();
void Func_020061ca();
void Func_020061d6();
void Func_020061d8();
void Func_020061e2();
void Func_020061f4();
void Func_020061f6();
void Func_02006200();
void Func_02006208();
void Func_02006214();
void Func_02006220();
void Func_02006226();
void Func_0200622c();
void Func_02006238();
void Func_02006240();
void Func_02006244();
void Func_0200624e();
void Func_02006254();
void Func_0200625c();
void Func_02006270();
void Func_02006272();
void Func_02006278();
void Func_0200627e();
void Func_02006294();
void Func_0200629c();
void Func_020062a2();
void Func_020062c2();
void Func_020062c8();
void Func_020062e0();
void Func_020062f4();
void Func_020062fe();
void Func_0200630e();
void Func_0200631a();
void Func_02006320();
void Func_02006322();
void Func_02006332();
void Func_02006366();
void Func_0200636a();
void Func_02006370();
void Func_02006372();
void Func_02006378();
void Func_0200637c();
void Func_02006380();
void Func_0200638c();
void Func_02006396();
void Func_0200639a();
void Func_020063b8();
void Func_020063ca();
void Func_020063ce();
void Func_020063d0();
void Func_020063d8();
void Func_020063e6();
void Func_020063ec();
void Func_020063ee();
void Func_02006400();
void Func_0200640a();
void Func_02006418();
void Func_02006426();
void Func_0200643a();
void Func_02006472();
void Func_02006486();
void Func_0200649e();
void Func_020064a6();
void Func_020064ae();
void Func_020064e2();
void Func_020064ee();
void Func_020064f0();
void Func_0200650a();
void Func_02006518();
void Func_020093c1();

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

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunMultiPhaseActorSequence(s32 a0)
{
    u32 i;
    s32 record;
    s32 base6_200d950;
    s32 base5_20093c1;

    Func_02005ea2(39);
    Func_02005ea8(40);
    Func_02005e7e(1);
    Func_02006034(17);
    Func_02005e98();
    Call3(Func_02005f0e, 8, 0x6080000, 0xc00000);
    if (a0 < 0) {
        Func_02005f22(8, 10);
    } else {
        Func_02005f2c(8, 8);
    }
    Value2(Func_02005ef4, 8, 0x200d668);
    Call3(Func_02005f3a, 0, 0x5e00000, 0xc00000);
    record = Func_02005ef0(0);
    {
        s32 shown = 0;
    
        *(u16 *)(record + 6) = shown;
    }
    Value2(Func_02005f14, 0, 0x200d738);
    Func_02005f5c(0, 35);
    Call3(Func_02005f22, 1, 0x10000, 0x8000);
    Call3(Func_02005f30, 2, 0x10000, 0x8000);
    Call3(Func_02005f3e, 3, 0x10000, 0x8000);
    Call3(Func_02005f8c, 1, 0x5b80000, 0xb80000);
    Call3(Func_02005f9a, 2, 0x5b80000, 0xc80000);
    Call3(Func_02005fa8, 3, 0x5a80000, 0xc00000);
    record = Value1(Func_02005f5e, 1);
    {
        s32 shown = 0;
    
        *(u16 *)(record + 6) = shown;
    }
    record = Value1(Func_02005f66, 2);
    {
        s32 shown = 0;
    
        *(u16 *)(record + 6) = shown;
    }
    record = Value1(Func_02005f6e, 3);
    {
        s32 shown = 0;
    
        *(u16 *)(record + 6) = shown;
    }
    Func_02005d9e(1);
    Func_02006056(0, 0);
    *(s32 *)((*(u8 **)0x03001ebc + 0x1c0)) = 0x100;
    Call2(Func_020060be, 0x10001, 1);
    Func_020060d2();
    Func_020060e6();
    Call1(Func_0200603c, 0x20f1);
    Func_02005f8a(60);
    base6_200d950 = (s32)Data_0200d950;
    Func_02005fcc(0, base6_200d950);
    record = Func_02005fba(0);
    *(s32 *)(record + 24) = 0x10000;
    record = Func_02005fc8(0);
    *(s32 *)(record + 28) = 0x10000;
    Func_02006034(0, 36);
    record = Func_02005fda(0);
    *(s32 *)(record + 8) += 0x30000;
    Func_02005fca(10);
    record = Func_02005ff0(0);
    Func_02005f36(record, 0);
    Func_02005fdc(20);
    Value2(Func_0200601c, 0, 0x200d808);
    Func_020060b4(1, 0);
    Func_02005ff2(20);
    Call3(Func_02006056, 1, 0x5e0, 176);
    Call3(Func_020060e2, 1, 0x4000, 10);
    Value3(Func_020060fc, 1, 0x100, 20);
    Func_020060e4(2, 0);
    Func_020061b4(1, 2);
    Func_0200602a(30);
    Call3(Func_0200608e, 2, 0x5d0, 176);
    Call3(Func_02006092, 1, 0x5f0, 184);
    Call3(Func_020060a6, 2, 0x5e0, 176);
    Func_020060ce(1, 1);
    Call3(Func_0200613a, 1, 0x6000, 0);
    Call3(Func_02006146, 2, 0x4000, 10);
    Func_02006106(2, 2);
    Func_0200607c(10);
    Func_02006214(2, 1);
    Func_0200608a(30);
    Func_02006112(1, 4);
    Func_02006098(30);
    Func_02006168(3, 0);
    Func_02006238(1, 3);
    Func_02006240(2, 3);
    Call3(Func_02006114, 3, 0x5d0, 184);
    Func_02006254(2, 0);
    Func_0200625c(1, 0);
    Call3(Func_020061c0, 1, 0x102, 60);
    Func_02006270(2, 1);
    Func_02006278(1, 2);
    Func_020060ee(40);
    Func_02006186(3, 2);
    ((void (*)())Func_020060fc)(10);
    Func_02006294(2, 3);
    Func_0200629c(1, 3);
    Func_02006112(20);
    Func_020061e2(3, 0);
    Value2(Func_0200615a, 0, 0x200d8ac);
    Func_020061ba(1, 2);
    Func_020062c2(1, 0);
    Func_02006138(20);
    Func_02006208(1, 0);
    Func_020061d8(2, 2);
    Func_020062e0(2, 0);
    Func_02006156(20);
    Func_02006226(2, 0);
    Func_020061f6(3, 2);
    Func_020062fe(3, 0);
    Func_02006174(20);
    Func_02006244(3, 0);
    Func_020061bc(0, base6_200d950);
    Func_0200618a(60);
    base5_20093c1 = (s32)Func_020093c1;
    *(s32 *)0x0200db80 = 9;
    Call2(Func_02005fee, base5_20093c1, 0xc80);
    Func_020061a4(5);
    Func_02006002(base5_20093c1);
    Func_020061b0(55);
    Call3(Func_020062a2, 1, 0x101, 60);
    Call2(Func_02006014, base5_20093c1, 0xc80);
    Func_020061ca(20);
    Func_02006028(base5_20093c1);
    Func_020061d6(40);
    Call3(Func_020062c8, 2, 0x101, 60);
    *(s32 *)0x0200db80 = 9;
    Call2(Func_0200603e, base5_20093c1, 0xc80);
    Func_020061f4(35);
    Func_02006052(base5_20093c1);
    Func_02006200(25);
    Call3(Func_020062f4, 3, 0x102, 60);
    *(s32 *)0x0200db80 = 9;
    Call2(Func_0200606a, base5_20093c1, 0xc80);
    Func_02006220(35);
    Func_0200607e(base5_20093c1);
    Func_0200622c(25);
    Call3(Func_02006320, 2, 0x102, 60);
    Func_020063d0(3, 2);
    Func_020063d8(2, 3);
    Func_0200624e(60);
    Func_020063e6(3, 0);
    Func_020063ee(2, 0);
    *(s32 *)0x0200db80 = 9;
    Call2(Func_020060bc, base5_20093c1, 0xc80);
    Func_02006272(35);
    Func_020060d0(base5_20093c1);
    Func_0200627e(25);
    Call3(Func_02006372, 3, 0x108, 60);
    Func_0200631a(1, 3);
    Func_02006322(2, 3);
    Func_02006332(3, 3);
    Func_0200643a(3, 2);
    Func_02006472(1, 2);
    *(s32 *)0x0200db80 = 9;
    Call2(Func_02006140, base5_20093c1, 0xc80);
    Func_02006370(1, 3);
    Func_02006378(2, 3);
    Func_02006380(3, 3);
    Func_0200630e(60);
    Call3(Func_0200636a, 3, 0x5b8, 200);
    Func_02006320(5);
    Call3(Func_0200637c, 2, 0x558, 184);
    Func_02006332(3);
    Call3(Func_02006396, 1, 0x5e8, 184);
    Call3(Func_0200639a, 1, 0x558, 184);
    Func_020063b8(3);
    Func_020063d0(3, 1);
    Func_020064f0(3, 0);
    Func_02006366(60);
    Call3(Func_020063ca, 3, 0x598, 200);
    Call3(Func_020063ce, 3, 0x558, 184);
    Func_020064e2();
    Func_020064ee();
    Func_0200638c(30);
    Call3(Func_0200640a, 1, 0x5e80000, 0xb00000);
    Call3(Func_02006418, 2, 0x5b80000, 0xc00000);
    Call3(Func_02006426, 3, 0x6180000, 0xc80000);
    Func_02006322();
    Func_0200650a(0x10000, 2);
    Func_02006518(1);
    Call1(Func_02006486, 0x214c);
    Func_0200649e(1, 0);
    Func_020064a6(2, 0);
    Func_020064ae(3, 0);
    Func_020063ec(60);
    Func_02006400();
}

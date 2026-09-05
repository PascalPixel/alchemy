#include "types.h"

#define FieldScene_RunScene3afSequenceB Func_02001f90

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_0200c764[];
extern u8 Data_0200c7a8[];
extern u8 Data_0200c7ec[];
extern u8 Data_0200c80c[];
extern u8 Data_0200c888[];
void Func_02002db6();
void Func_02005bfa();
void Func_02006210();
void Func_0200621c();
void Func_0200624c();
void Func_02006278();
void Func_02006280();
void Func_02006284();
void Func_02006290();
void Func_0200629c();
void Func_020062a8();
void Func_020062aa();
s32 Func_020062ae();
void Func_020062b4();
void Func_020062b6();
s32 Func_020062da();
s32 Func_020062e6();
s32 Func_020062f2();
s32 Func_020062fe();
s32 Func_0200630a();
s32 Func_02006316();
void Func_0200633c();
void Func_02006340();
void Func_0200634c();
s32 Func_02006362();
s32 Func_02006386();
void Func_0200638a();
s32 Func_02006392();
s32 Func_0200639e();
void Func_020063a2();
s32 Func_020063a8();
void Func_020063b8();
void Func_020063c6();
void Func_020063ce();
void Func_020063d0();
void Func_020063d4();
s32 Func_020063d6();
void Func_020063e0();
s32 Func_020063e8();
void Func_0200642c();
void Func_02006436();
void Func_0200643a();
void Func_0200645c();
void Func_02006466();
void Func_0200646a();
void Func_02006472();
void Func_02006474();
void Func_0200647e();
void Func_0200648c();
void Func_02006490();
void Func_02006494();
void Func_020064a4();
void Func_020064ae();
void Func_020064ba();
s32 Func_020064be();
void Func_020064c8();
s32 Func_020064d6();
void Func_020064e8();
void Func_020064ee();
void Func_020064f8();
void Func_02006500();
void Func_02006506();
void Func_0200650a();
void Func_02006510();
void Func_02006514();
void Func_0200651c();
void Func_02006530();
void Func_02006532();
void Func_02006534();
void Func_0200653c();
void Func_02006540();
void Func_02006566();
void Func_02006578();
void Func_0200657e();
void Func_02006584();
void Func_0200658a();
void Func_020065ba();
void Func_020065d2();
void Func_02006642();
void Func_0200667e();
void Func_0200668a();
void Func_02006692();
void Func_0200669e();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3afSequenceB(void)
{
    u32 i;
    s32 record;
    s32 base5_200c80c;
    s32 base5_200c7a8;
    s32 base5_200c764;
    s32 base5_200c7ec;
    s32 base5_200c888;

    Func_02006280();
    Func_02006340(0, 15);
    record = Func_020062ae(0);
    Func_0200624c(record, 0);
    Call1(Func_020062aa, 0x200d160);
    Func_02006210(1);
    Call1(Func_020062b6, 0x200d268);
    Func_0200621c(1);
    Func_0200633c(31, 0);
    record = Func_020062da(24);
    Func_02006278(record, 1);
    record = Func_020062e6(25);
    Func_02006284(record, 1);
    record = Func_020062f2(26);
    Func_02006290(record, 1);
    record = Func_020062fe(27);
    Func_0200629c(record, 1);
    record = Func_0200630a(28);
    Func_020062a8(record, 1);
    record = Func_02006316(29);
    Func_020062b4(record, 1);
    Call3(Func_0200638a, 22, 0x1000000, 0x2800000);
    base5_200c80c = (s32)Data_0200c80c;
    Func_0200634c(22, base5_200c80c);
    Call3(Func_020063a2, 21, 0x10c0000, 0x2b40000);
    Value2(Func_02006362, 22, base5_200c80c);
    Call3(Func_020063b8, 24, 0xf20000, 0x25c0000);
    Call3(Func_020063c6, 25, 0x1080000, 0x2580000);
    Call3(Func_020063d4, 26, 0xfe0000, 0x29c0000);
    Call3(Func_020063e0, 27, 0x11a0000, 0x2920000);
    *(u8 *)(Func_02006386(24) + 99) = 0;
    *(u8 *)(Func_02006392(25) + 99) = 1;
    *(u8 *)(Func_0200639e(26) + 99) = 0;
    *(u8 *)(Func_020063a8(27) + 99) = 1;
    base5_200c7a8 = (s32)Data_0200c7a8;
    Func_020063ce(24, base5_200c7a8);
    Value2(Func_020063d6, 25, base5_200c7a8);
    base5_200c764 = (s32)Data_0200c764;
    Func_020063e0(26, base5_200c764);
    Value2(Func_020063e8, 27, base5_200c764);
    Func_0200643a(20, 0, 0);
    *(s32 *)((*(u8 **)0x03001ebc + 0x1c0)) = 0x202;
    Func_0200651c();
    Func_02006530();
    Call1(Func_020063d0, 0x190);
    Call3(Func_02006466, 28, 0xfe0000, 0x2e40000);
    Call3(Func_02006472, 29, 0x180000, 0x24a0000);
    Call3(Func_0200642c, 28, 0x19999, 0xcccc);
    Call3(Func_02006436, 29, 0x19999, 0xcccc);
    Call3(Func_0200646a, 29, 172, 0x284);
    Call3(Func_0200647e, 28, 200, 0x294);
    Call3(Func_0200645c, 0, 0x40000, 0x20000);
    Call3(Func_02006490, 0, 174, 0x26c);
    Call3(Func_020064a4, 28, 180, 0x244);
    Func_020065ba(146);
    base5_200c7ec = (s32)Data_0200c7ec;
    Func_0200648c(28, base5_200c7ec);
    Func_02006494(29, base5_200c7ec);
    Func_020065d2(240);
    Call3(Func_020064ee, 31, 0x860000, 0x2520000);
    Call2(Func_020064ae, 31, 0x200c814);
    Func_02006474(10);
    Call3(Func_0200650a, 30, 0x860000, 0x2480000);
    Call3(Func_020064c8, 30, 0x40000, 0x20000);
    record = Func_020064be(30);
    *(s32 *)(record + 40) = 0x80000;
    Call3(Func_02006510, 30, 186, 0x264);
    record = Func_020064d6(30);
    Func_02006474(record, 1);
    Func_020064ba(10);
    Call3(Func_02006500, 30, 0x20000, 0x10000);
    Call3(Func_0200653c, 30, 216, 0x258);
    Call2(Func_02005bfa, 30, 0xc000);
    Func_02002db6();
    Func_020064e8(10);
    base5_200c888 = (s32)Data_0200c888;
    Func_02006532(30, base5_200c888);
    Func_020064f8(10);
    Func_02006540(28, base5_200c888);
    Func_02006506(10);
    Func_02006566(29, base5_200c888);
    Func_02006514(20);
    Func_02006692(147);
    Func_0200667e();
    Func_0200668a();
    Func_02006578(24);
    Func_0200657e(25);
    Func_02006584(26);
    Func_0200658a(27);
    Func_02006540(10);
    Func_02006514();
    Func_02006642(21);
    Call3(Func_02006534, 0x1e45, 1, 0);
    Func_02006530();
    Func_0200669e(13);
}

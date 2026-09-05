#include "types.h"

#define FieldScene_RunFourActorProgressPresentation Func_02005d10

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_020094fa();
void Func_0200a400();
void Func_0200a638();
void Func_0200a642();
void Func_0200a656();
void Func_0200a67a();
void Func_0200a69c();
void Func_0200a706();
void Func_0200a710();
void Func_0200a71a();
void Func_0200a724();
void Func_0200a72e();
void Func_0200a746();
void Func_0200a75a();
void Func_0200a790();
void Func_0200a818();
void Func_0200a826();
void Func_0200a99a();
s32 Func_0200acc6();
s32 Func_0200acd0();
s32 Func_0200acda();
s32 Func_0200ace4();
void Func_0200c20a();
void Func_0200c248();
s32 Func_0200c26c();
s32 Func_0200c2aa();
void Func_0200c2b0();
void Func_0200c2be();
void Func_0200c2ce();
void Func_0200c2d4();
void Func_0200c2e8();
void Func_0200c312();
void Func_0200c316();
s32 Func_0200c31e();
s32 Func_0200c32c();
s32 Func_0200c336();
void Func_0200c354();
void Func_0200c37a();
void Func_0200c3b0();
void Func_0200c3ca();
s32 Func_0200c3f2();
s32 Func_0200c3fc();
void Func_0200c408();
void Func_0200c422();
void Func_0200c424();
void Func_0200c42e();
void Func_0200c432();
void Func_0200c43a();
void Func_0200c446();
void Func_0200c448();
void Func_0200c44c();
void Func_0200c460();
void Func_0200c46c();
void Func_0200c488();
void Func_0200c4b2();
void Func_0200c4d4();
void Func_0200c4e2();
void Func_0200c4ee();
void Func_0200c4fa();
void Func_0200c502();
void Func_0200c506();
void Func_0200c510();
void Func_0200c51e();
void Func_0200c52c();
void Func_0200c53c();
void Func_0200c554();
void Func_0200c55c();
void Func_0200c568();
void Func_0200c56a();
void Func_0200c56c();
void Func_0200c574();
void Func_0200c576();
void Func_0200c578();
void Func_0200c586();
void Func_0200c598();
void Func_0200c59a();
s32 Func_0200c5a0();
void Func_0200c5aa();
void Func_0200c5b6();
s32 Func_0200c5b8();
void Func_0200c5c2();
void Func_0200c5ce();
void Func_0200c5da();

extern u8 Data_0200e904[];
extern u8 Data_0200e938[];
extern u8 Data_0200e7c8[];

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunFourActorProgressPresentation(void)
{
    u32 i;
    s32 rec;
    s32 rec2;
    s32 rec4;
    s32 rec7;
    s32 record;
    s32 base6_200e904;
    s32 base5_200e938;
    s32 base5_200e7c8;

    rec7 = Value2(Func_0200acc6, 0, 0);
    rec2 = Value2(Func_0200acd0, 1, 0);
    rec4 = Value2(Func_0200acda, 2, 0);
    rec = Value2(Func_0200ace4, 3, 0);
    Func_0200c20a();
    Func_020094fa();
    Func_0200a638(10, 0, 0);
    Func_0200a642(17, 0, 0);
    Call3(Func_0200c2b0, 8, 0x1d80000, 0x980000);
    Call3(Func_0200c2be, 27, 0x1b80000, 0x860000);
    Func_0200c316(27, 15);
    record = Func_0200c26c(27);
    Func_0200c20a(record, 0);
    Func_0200a400(16);
    Func_0200c2e8(9, 5);
    Call4(Func_0200a656, 0x1b60000, -1, 0xae0000, 0x1000001);
    Func_0200a69c(8, 1, 20);
    Func_0200c354(27, 0);
    record = Func_0200c2aa(27);
    Func_0200c248(record, 1);
    Call3(Func_0200c2ce, 27, 0x10000, 0x8000);
    Call3(Func_0200c312, 27, 0x198, 132);
    Call3(Func_0200c31e, 27, 0x198, 142);
    Call3(Func_0200c3ca, 27, 0x3000, 20);
    Func_0200c37a(27, 2);
    Call1(Func_0200c3b0, 0x1f29);
    Func_0200a67a(27);
    Func_0200c2d4(120);
    Func_0200a706(12, rec7, 0);
    Func_0200a710(12, rec2, 1);
    Func_0200a71a(12, rec4, 0);
    Func_0200a724(12, rec, 1);
    Func_0200a72e(11, 0, 0);
    Call3(Func_0200c422, rec7, 0xd000, 0);
    Call3(Func_0200c42e, rec2, 0xb000, 0);
    Call3(Func_0200c43a, rec4, 0xd000, 0);
    Call3(Func_0200c446, rec, 0xb000, 60);
    base6_200e904 = 0;
    if (Value1(Func_0200c31e, 0x934) != 0) {
        base6_200e904 = 2;
    } else {
        if (Value1(Func_0200c32c, 0x933) != 0 || Value1(Func_0200c336, 0x92f) != 0) {
            base6_200e904 = 1;
        }
    }
    Func_0200c424(rec7, 1);
    if (base6_200e904 == 1) {
        bump_step(1);
    } else {
        if (base6_200e904 == 2) {
            bump_step(2);
        }
    }
    Func_0200c44c(rec7, 2);
    Func_0200a746(rec7);
    Call1(Func_0200c488, 0x1f2d);
    Func_0200c448(27, 4);
    Func_0200a75a(27);
    Call3(Func_0200c4e2, rec7, 0x102, 0);
    Call3(Func_0200c4ee, rec2, 0x102, 0);
    Call3(Func_0200c4fa, rec4, 0x102, 0);
    Call3(Func_0200c506, rec, 0x102, 60);
    Func_0200a790(27);
    Call3(Func_0200c460, 27, 0x198, 132);
    Call3(Func_0200c46c, 27, 0x1bc, 132);
    Func_0200c432(27);
    Func_0200c408(40);
    if (base6_200e904 == 0) {
        if (Value1(Func_0200c3f2, 0x92c) != 0 || Value1(Func_0200c3fc, 0x92d) != 0) {
            base6_200e904 = 3;
        }
    }
    if (base6_200e904 == 0) {
        bump_step(1);
    } else {
        if (base6_200e904 == 1) {
            bump_step(2);
        } else {
            if (base6_200e904 == 2) {
                bump_step(3);
            }
        }
    }
    Func_0200a826(rec7, 0);
    Func_0200a818(rec7);
    Call3(Func_0200c4b2, rec7, 0x10000, 0x8000);
    base6_200e904 = (s32)Data_0200e904;
    Func_0200c4d4(rec7, base6_200e904);
    Call3(Func_0200c5a0, rec2, 0x5000, 0);
    Func_0200c5aa(rec4, 0, 0);
    Call3(Func_0200c5b6, rec, 0x8000, 40);
    Call3(Func_0200c5c2, rec2, 0xd000, 0);
    Call3(Func_0200c5ce, rec4, 0xb000, 0);
    Call3(Func_0200c5da, rec, 0x5000, 20);
    Call3(Func_0200c510, rec2, 0x10000, 0x8000);
    Call3(Func_0200c51e, rec4, 0x10000, 0x8000);
    Call3(Func_0200c52c, rec, 0x10000, 0x8000);
    Func_0200c53c(rec4, base6_200e904);
    Func_0200c502(40);
    base5_200e938 = (s32)Data_0200e938;
    Func_0200c55c(rec2, base5_200e938);
    Func_0200c554(rec2, base6_200e904);
    Func_0200c56c(rec, base5_200e938);
    Func_0200c574(rec, base6_200e904);
    Call3(Func_0200c56a, 1, 0x10000, 0x8000);
    Call3(Func_0200c578, 2, 0x10000, 0x8000);
    Call3(Func_0200c586, 3, 0x10000, 0x8000);
    base5_200e7c8 = (s32)Data_0200e7c8;
    Func_0200c598(1, base5_200e7c8);
    Value2(Func_0200c5a0, 2, base5_200e7c8);
    Value2(Func_0200c5b8, 3, base5_200e7c8);
    Func_0200a99a(23, 0, 0);
    Call1(Func_0200c568, 0x927);
    Call1(Func_0200c568, 0x8a0);
    Call1(Func_0200c576, 0x12f);
    Func_0200c59a();
}

#include "types.h"

#define FieldScene_RunFlagDependentFormationPresentation Func_02005d10

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
u8 *Func_0200c26c();
u8 *Func_0200c2aa();
void Func_0200c2b0();
void Func_0200c2be();
void Func_0200c2ce();
void Func_0200c2d4();
void Func_0200c2e8();
void Func_0200c312();
void Func_0200c316();
void Func_0200c31e();
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
void Func_0200c5a0();
void Func_0200c5aa();
void Func_0200c5b6();
void Func_0200c5b8();
void Func_0200c5c2();
void Func_0200c5ce();
void Func_0200c5da();

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

void Func_0200c20a_a();
s32 Func_0200c31e_a();
void Func_0200c5a0_a();
void Func_0200c568_a();

extern u8 Data_0200e904[];
extern u8 Data_0200e938[];
extern u8 Data_0200e7c8[];

void FieldScene_RunFlagDependentFormationPresentation(void)
{
    s32 slot_a;
    s32 slot_b;
    s32 slot_c;
    s32 slot_d;
    u8 *record;
    s32 branch;
    u8 *action_a;
    u8 *action_b;
    u8 *action_c;

    slot_a = Func_0200acc6(0, 0);
    slot_b = Func_0200acd0(1, 0);
    slot_c = Func_0200acda(2, 0);
    slot_d = Func_0200ace4(3, 0);
    Func_0200c20a();
    Func_020094fa();
    Func_0200a638(10, 0, 0);
    Func_0200a642(17, 0, 0);
    Call3(Func_0200c2b0, 8, 0x1d80000, 0x980000);
    Call3(Func_0200c2be, 27, 0x1b80000, 0x860000);
    Func_0200c316(27, 15);
    record = Func_0200c26c(27);
    Func_0200c20a_a(record, 0);
    Func_0200a400(16);
    Func_0200c2e8(9, 5);
    Func_0200a656(0x1b60000, -1, 0xae0000, 0x1000001);
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
    Func_0200a706(12, slot_a, 0);
    Func_0200a710(12, slot_b, 1);
    Func_0200a71a(12, slot_c, 0);
    Func_0200a724(12, slot_d, 1);
    Func_0200a72e(11, 0, 0);
    Call3(Func_0200c422, slot_a, 0xd000, 0);
    Call3(Func_0200c42e, slot_b, 0xb000, 0);
    Call3(Func_0200c43a, slot_c, 0xd000, 0);
    Call3(Func_0200c446, slot_d, 0xb000, 60);
    branch = 0;
    if (Value1(Func_0200c31e_a, 0x934) != 0) {
        branch = 2;
    } else if (Value1(Func_0200c32c, 0x933) != 0 ||
               Value1(Func_0200c336, 0x92f) != 0) {
        branch = 1;
    }
    Func_0200c424(slot_a, 1);
    if (branch == 1) {
        *(u16 *)(*(u32 *)0x03001ebc + 0x1d8) += 1;
    } else if (branch == 2) {
        *(u16 *)(*(u32 *)0x03001ebc + 0x1d8) += 2;
    }
    Func_0200c44c(slot_a, 2);
    Func_0200a746(slot_a);
    Call1(Func_0200c488, 0x1f2d);
    Func_0200c448(27, 4);
    Func_0200a75a(27);
    Call3(Func_0200c4e2, slot_a, 0x102, 0);
    Call3(Func_0200c4ee, slot_b, 0x102, 0);
    Call3(Func_0200c4fa, slot_c, 0x102, 0);
    Call3(Func_0200c506, slot_d, 0x102, 60);
    Func_0200a790(27);
    Call3(Func_0200c460, 27, 0x198, 132);
    Call3(Func_0200c46c, 27, 0x1bc, 132);
    Func_0200c432(27);
    Func_0200c408(40);
    if (branch == 0 &&
        (Value1(Func_0200c3f2, 0x92c) != 0 || Value1(Func_0200c3fc, 0x92d) != 0)) {
        branch = 3;
    }
    if (branch == 0) {
        *(u16 *)(*(u32 *)0x03001ebc + 0x1d8) += 1;
    } else if (branch == 1) {
        *(u16 *)(*(u32 *)0x03001ebc + 0x1d8) += 2;
    } else if (branch == 2) {
        *(u16 *)(*(u32 *)0x03001ebc + 0x1d8) += 3;
    }
    Func_0200a826(slot_a, 0);
    Func_0200a818(slot_a);
    Call3(Func_0200c4b2, slot_a, 0x10000, 0x8000);
    action_a = Data_0200e904;
    Func_0200c4d4(slot_a, action_a);
    Call3(Func_0200c5a0, slot_b, 0x5000, 0);
    Func_0200c5aa(slot_c, 0, 0);
    Call3(Func_0200c5b6, slot_d, 0x8000, 40);
    Call3(Func_0200c5c2, slot_b, 0xd000, 0);
    Call3(Func_0200c5ce, slot_c, 0xb000, 0);
    Call3(Func_0200c5da, slot_d, 0x5000, 20);
    Call3(Func_0200c510, slot_b, 0x10000, 0x8000);
    Call3(Func_0200c51e, slot_c, 0x10000, 0x8000);
    Call3(Func_0200c52c, slot_d, 0x10000, 0x8000);
    Func_0200c53c(slot_c, action_a);
    Func_0200c502(40);
    action_b = Data_0200e938;
    Func_0200c55c(slot_b, action_b);
    Func_0200c554(slot_b, action_a);
    Func_0200c56c(slot_d, action_b);
    Func_0200c574(slot_d, action_a);
    Call3(Func_0200c56a, 1, 0x10000, 0x8000);
    Call3(Func_0200c578, 2, 0x10000, 0x8000);
    Call3(Func_0200c586, 3, 0x10000, 0x8000);
    action_c = Data_0200e7c8;
    Func_0200c598(1, action_c);
    Func_0200c5a0_a(2, action_c);
    Func_0200c5b8(3, action_c);
    Func_0200a99a(23, 0, 0);
    Call1(Func_0200c568, 0x927);
    Call1(Func_0200c568_a, 0x8a0);
    Call1(Func_0200c576, 0x12f);
    Func_0200c59a();
}

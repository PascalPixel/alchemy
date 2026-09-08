#include "types.h"

#define FieldScene_RunExtendedFormationPresentation Func_020057ec

void Func_02009ed8();
void Func_0200a106();
void Func_0200a110();
void Func_0200a126();
void Func_0200a16c();
void Func_0200a186();
void Func_0200a188();
void Func_0200a212();
void Func_0200a218();
void Func_0200a2a6();
void Func_0200a2ac();
void Func_0200a37a();
void Func_0200a384();
void Func_0200a38e();
void Func_0200a398();
void Func_0200a3dc();
void Func_0200a3e2();
void Func_0200a402();
void Func_0200a4e8();
s32 Func_0200a7a2();
s32 Func_0200a7ac();
s32 Func_0200a7b6();
void Func_0200bcdc();
void Func_0200bce0();
void Func_0200bd28();
void Func_0200bd38();
u8 *Func_0200bd42();
void Func_0200bd54();
void Func_0200bd64();
void Func_0200bd66();
void Func_0200bd76();
void Func_0200bd7e();
void Func_0200bd8e();
void Func_0200bd92();
void Func_0200bd94();
void Func_0200bdbe();
void Func_0200bdec();
void Func_0200be84();
void Func_0200be88();
void Func_0200beb4();
void Func_0200bed6();
void Func_0200bee6();
void Func_0200beea();
void Func_0200bef0();
void Func_0200bef8();
void Func_0200bf04();
void Func_0200bf0e();
void Func_0200bf1a();
void Func_0200bf1c();
void Func_0200bf20();
void Func_0200bf24();
void Func_0200bf26();
void Func_0200bf2e();
void Func_0200bf30();
void Func_0200bf3a();
void Func_0200bf3e();
void Func_0200bf42();
void Func_0200bf46();
void Func_0200bf48();
void Func_0200bf52();
void Func_0200bf76();
void Func_0200bf94();
void Func_0200bfa0();
void Func_0200bfac();
void Func_0200bfb8();
void Func_0200bfba();
void Func_0200bfc2();
void Func_0200bfc4();
void Func_0200bfca();
void Func_0200bfda();
s32 Func_0200c01e();
void Func_0200c054();
void Func_0200c05e();
void Func_0200c068();
void Func_0200c072();
void Func_0200c07e();
void Func_0200c080();
void Func_0200c08c();
void Func_0200c08e();
void Func_0200c098();
void Func_0200c0a2();
void Func_0200c0a4();
void Func_0200c0a6();
void Func_0200c0ae();
void Func_0200c0b2();
void Func_0200c0ba();
void Func_0200c0bc();
void Func_0200c0be();
void Func_0200c0c2();
void Func_0200c0c4();
void Func_0200c0c8();
void Func_0200c0d0();
void Func_0200c0d8();
void Func_0200c0da();
void Func_0200c0e2();
void Func_0200c0e6();
void Func_0200c0ea();
void Func_0200c0ec();
void Func_0200c0f0();
void Func_0200c0f6();
void Func_0200c0f8();
void Func_0200c102();
void Func_0200c104();
void Func_0200c110();
void Func_0200c14a();

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

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

void Func_0200bd92_a();
u8 *Func_0200beea_a();
void Func_0200bf0e_a();
void Func_0200bf52_a();
void Func_0200bfac_a();
void Func_0200c0ea_a();
void Func_0200c0d0_a();

extern u8 Data_0200e818[];
extern void Func_02005c48(void);

void FieldScene_RunExtendedFormationPresentation(void)
{
    s32 slot_b;
    s32 slot_c;
    s32 slot_a;
    u8 *record;
    s32 action;

    slot_a = Value2(Func_0200a7a2, 0, 0);
    slot_b = Func_0200a7ac(1, 0);
    slot_c = Value2(Func_0200a7b6, 2, 0);
    Func_0200bcdc();
    Func_0200a106(10, 0, 0);
    Func_0200a110(17, 0, 0);
    Call3(Func_0200bd7e, 8, 0x1d80000, 0x980000);
    Func_0200bd8e(9, 5);
    Call3(Func_0200bd94, 27, 0x1b80000, 0x860000);
    Func_0200bdec(27, 15);
    record = Func_0200bd42(27);
    Func_0200bce0(record, 0);
    Func_02009ed8(16);
    Func_0200a126(0x1b60000, -1, 0xae0000, 0x1000001);
    Func_0200a16c(8, 1, 20);
    Call1(Func_0200beea, 19);
    Func_0200bef0(181);
    Call3(Func_0200bd28, 0x20000, 0x20000, 0x10000);
    Func_0200bd66(10);
    Call3(Func_0200bd38, -1, -1, 0xe666);
    Func_0200bd76(80);
    Func_0200bf1c(181);
    Call3(Func_0200bd54, 0x20000, 0x20000, 0x10000);
    Func_0200bd92(10);
    Call3(Func_0200bd64, -1, -1, 0xe666);
    Func_0200bf42(63);
    Call1(Func_0200bd92_a, 0x11a);
    Call2(Func_0200bee6, 3, 0x102);
    Func_0200bdbe(40);
    Func_0200a186(3, 0x6000);
    Call1(Func_0200beb4, 0x1ec1);
    Func_0200bed6(3, 0, 40);
    Func_0200a188(27);
    Call3(Func_0200bef8, 0, 0x2000, 0);
    Call3(Func_0200bf04, 1, 0xa000, 0);
    Func_0200bf0e(2, 0, 0);
    Call3(Func_0200bf1a, 3, 0xe000, 40);
    Func_0200bf24(0, 0x6000, 0);
    Call3(Func_0200bf30, 1, 0xe000, 0);
    Func_0200bf3a(2, 0x6000, 0);
    Call3(Func_0200bf46, 3, 0x8000, 40);
    Call3(Func_0200bf52, 2, 0xe000, 0);
    Call3(Func_0200bf76, 2, 0x100, 60);
    Func_0200a218(2, 0x2000);
    Func_0200bf20(2, 1);
    Func_0200a212(2);
    Func_0200bf26(0, 1);
    Func_0200bf2e(1, 1);
    Func_0200bf3e(3, 1);
    Func_0200be84(10);
    Call3(Func_0200bfa0, 0, 0xc000, 0);
    Call3(Func_0200bfac, 1, 0xc000, 0);
    Call3(Func_0200bfb8, 2, 0xe000, 0);
    Call3(Func_0200bfc4, 3, 0xa000, 20);
    Func_0200bf94(27, 0);
    record = Func_0200beea_a(27);
    Func_0200be88(record, 1);
    Call3(Func_0200bf0e_a, 27, 0x10000, 0x8000);
    Call3(Func_0200bf52_a, 27, 0x1ae, 134);
    Func_0200a2ac(27, 0x3000);
    Func_0200bfac_a(27, 2);
    Func_0200a2a6(27);
    Func_0200bfba(slot_a, 1);
    Func_0200bfc2(slot_b, 1);
    Func_0200bfca(slot_c, 1);
    Func_0200bfda(13, 1);
    Call2(Func_0200c054, slot_a, 0x102);
    Call2(Func_0200c05e, slot_b, 0x102);
    Call2(Func_0200c068, slot_c, 0x102);
    Call2(Func_0200c072, 13, 0x102);
    Func_0200bf48(40);
    Func_0200a37a(12, slot_a, 0);
    Func_0200a384(12, slot_b, 1);
    Func_0200a38e(12, slot_c, 0);
    Func_0200a398(11, 1, 0);
    Call3(Func_0200c08c, slot_a, 0xd000, 0);
    Call3(Func_0200c098, slot_b, 0xb000, 0);
    Call3(Func_0200c0a4, slot_c, 0xd000, 0);
    Func_0200c0ae(0, 0, 0);
    Call3(Func_0200c0ba, 1, 0x8000, 0);
    Func_0200c0c4(2, 0, 0);
    Call3(Func_0200c0d0, 3, 0x8000, 40);
    Func_0200c080(27, 2);
    Func_0200c0c8(27, 0);
    Call3(Func_0200c0ec, 0, 0xc000, 0);
    Call3(Func_0200c0f8, 1, 0xc000, 0);
    Call3(Func_0200c104, 2, 0xc000, 0);
    Call3(Func_0200c110, 3, 0xc000, 20);
    Call3(Func_0200c07e, 27, 0x1b8, 134);
    Func_0200c0a2(27, 0, 0);
    Func_0200a3e2(1, 0x8000);
    Func_0200c0ea(1, 1);
    Func_0200a3dc(1);
    Func_0200c14a(2, 0, 0);
    Func_0200a402(3, 0x8000);
    Func_0200c0da(0, 3);
    Func_0200c0e2(1, 3);
    Func_0200c0ea_a(2, 3);
    Func_0200c102(3, 3);
    Call3(Func_0200c0a6, 1, 0x10000, 0x8000);
    Call3(Func_0200c0b2, 2, 0x10000, 0x8000);
    Call3(Func_0200c0be, 3, 0x10000, 0x8000);
    action = (s32)Data_0200e818;
    Func_0200c0d0_a(1, action);
    Func_0200c0d8(2, action);
    Func_0200c0f0(3, action);
    Call1(Func_0200c08e, 0x302);
    *(s32 *)0x0200ff84 = 0;
    Value2(Func_0200c01e, (s32)Func_02005c48, 0xc80);
    Func_0200a4e8(23, 0, 0);
    Func_0200c0f6(27);
    Call1(Func_0200c0bc, 0x12f);
    Call1(Func_0200c0c2, 0x927);
    Func_0200c0e6();
}

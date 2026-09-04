#include "types.h"

#define FieldScene_RunScene37f_02000d1c Func_02000d1c

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_020028cc();
void Func_020028e4();
s32 Func_0200293e();
s32 Func_02002952();
s32 Func_02002966();
void Func_0200297c();
void Func_02002986();
void Func_02002990();
void Func_0200299a();
void Func_020029a4();
void Func_020029d2();
void Func_020029d6();
void Func_020029da();
void Func_020029e0();
void Func_020029e2();
void Func_020029e6();
void Func_020029ec();
void Func_020029f0();
void Func_020029f4();
void Func_020029fa();
void Func_02002a12();
void Func_02002a1a();
void Func_02002a28();
void Func_02002a36();
void Func_02002a40();
void Func_02002a6e();
s32 Func_02002a7e();
void Func_02002a8a();
void Func_02002a92();
s32 Func_02002a9e();
void Func_02002aa0();
void Func_02002ab0();
void Func_02002ab8();
s32 Func_02002abe();
void Func_02002ac0();
void Func_02002ac4();
void Func_02002ad0();
void Func_02002ae0();
void Func_02002af0();
void Func_02002afa();
void Func_02002afe();
void Func_02002b10();
void Func_02002b16();
void Func_02002b18();
void Func_02002b1a();
void Func_02002b28();
void Func_02002b2a();
void Func_02002b3a();
void Func_02002b42();
void Func_02002b4a();

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
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene37f_02000d1c(void)
{
    u32 i;
    s32 record;

    Func_020028cc();
    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c0)) = 0x100;
    Func_020029d6();
    ((void (*)())Func_0200293e)(0, 0);
    Func_020028e4(4);
    Call4(Func_020029d6, -1, -1, -1, 0);
    Call2(Func_020029d6, 0x9999, 0x1333);
    Call4(Func_020029f0, 0x4c80000, -1, 0x880000, 1);
    record = Value1(Func_0200293e, 0);
    if (record != 0) {
        Func_0200297c(8, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    record = Value1(Func_02002952, 0);
    if (record != 0) {
        Func_02002990(5, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    record = Value1(Func_02002966, 0);
    if (record != 0) {
        Func_020029a4(1, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    Call3(Func_02002986, 8, 0x9999, 0x4ccc);
    Call3(Func_02002990, 5, 0x9999, 0x4ccc);
    Call3(Func_0200299a, 1, 0x9999, 0x4ccc);
    Func_020029d2(1, 2);
    Func_020029da(5, 2);
    Func_020029e2(8, 2);
    Call3(Func_020029d6, 1, -16, 0);
    Func_020029e0(5, 16, 0);
    Call3(Func_020029ec, 8, 0, -32);
    Func_020029fa(1);
    Func_02002a12(1, 0);
    Func_02002a1a(5, 0);
    Call3(Func_02002a7e, 1, 0xc000, 0);
    Call3(Func_02002a8a, 5, 0xc000, 0);
    Func_02002a28(8);
    Func_02002a40(8, 1);
    Func_020029e6(40);
    Func_02002a6e(8, 2);
    Func_020029f4(20);
    Call3(Func_02002ab8, 8, 0x3000, 40);
    Call3(Func_02002ac4, 8, 0x5000, 40);
    Call3(Func_02002ad0, 8, 0x3000, 20);
    Func_02002a92(8, 4, 20);
    Call1(Func_02002ab8, 0xfd3);
    Call2(Func_02002ae0, 0x4008, 0);
    Func_02002a36(20);
    Call4(Func_02002b28, 0x4c80000, -1, 0x940000, 1);
    Func_02002ab0(1, 2);
    record = Value1(Func_02002a7e, 0);
    if (record != 0) {
        Func_02002aa0(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_02002ad0(5, 2);
    record = Value1(Func_02002a9e, 0);
    if (record != 0) {
        Func_02002ac0(5, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_02002af0(8, 2);
    record = Value1(Func_02002abe, 0);
    if (record != 0) {
        Func_02002ae0(8, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_02002afe(1);
    Func_02002b10(1, 0, 0);
    Func_02002b1a(5, 0, 0);
    Func_02002b18(8);
    Func_02002b2a(8, 0, 0);
    Func_02002b3a(1, 1);
    Func_02002b42(5, 1);
    Func_02002b4a(8, 1);
    Call1(Func_02002ae0, 0x802);
    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c0)) = 0x204;
    Call1(Func_02002afa, 0x12f);
    Func_02002b16();
}

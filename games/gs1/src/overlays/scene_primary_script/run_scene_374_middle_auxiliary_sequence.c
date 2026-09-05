#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_020027c8();
s32 Func_020027ec();
void Func_02002824();
void Func_02002a84();
void Func_02002b20();
void Func_02002b2e();
s32 Func_02002b36();
s32 Func_02002b40();
void Func_02002b6c();
void Func_02002b78();
void Func_02002b86();
void Func_02002b8a();
void Func_02002b90();
void Func_02002bb0();
s32 Func_02002bec();
s32 Func_02002c22();
void Func_02002c3e();
void Func_02002c4a();
void Func_02002c5c();
void Func_02002c74();
void Func_02002cba();
void Func_02002ce4();
void Func_02002cfc();
void Func_02002d04();
void Func_02002d10();
void Func_02002d40();
void Func_02002dec();

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
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunMiddleAuxiliarySequence(void)
{
    u32 i;
    s32 p8;
    u8 *rec8;
    s32 record;
    s32 v2;

    Func_02002b78();
    Call3(Func_02002bec, 0, 82, 0x2f8);
    Func_02002c3e(15, 0, 30);
    Call1(Func_02002c5c, 0xeae);
    Func_020027c8(15, 20);
    Value3(Func_020027ec, 15, 0xa000, 20);
    Call2(Func_02002cba, 15, 0x102);
    Func_02002bb0(20);
    Func_02002a84();
    for (i = 0; i < 40; i++) {
        Func_02002824(Func_02002bec(15));
        Func_02002b20(1);
    }
    Value2(Func_02002b36, 0x200a581, 0xc80);
    Value2(Func_02002b40, 0x200a5a1, 0xc80);
    Call3(Func_02002ce4, 0, 0xa000, 10);
    rec8 = Value1(Func_02002c22, 20);
    v2 = rec8[85];
    rec8[85] = 0;
    p8 = v2;
    for (i = 0; i < 40; i++) {
        *(volatile s32 *)((s32)rec8 + 12) += 0x1800;
        Func_02002b6c(1);
    }
    rec8[85] = p8;
    Call1(Func_02002b8a, 0x200a581);
    Call1(Func_02002b90, 0x200a5a1);
    Func_02002b86(1);
    Func_02002dec(161);
    Func_02002cfc(15, 0);
    Func_02002d04(20, 0);
    Func_02002c4a(40);
    Func_02002b2e();
    Func_02002d10(0, 15, 30);
    Func_02002d40(15, 0);
    Func_02002c74();
}

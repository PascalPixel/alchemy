#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_0200b738[];
extern u8 Data_03001ebc[];
void Func_020022e0();
void Func_020022f4();
void Func_02002308();
void Func_02002322();
void Func_02002336();
void Func_0200234a();
void Func_020040d4();
s32 Func_020040d6();
void Func_020040ec();
s32 Func_020040f2();
void Func_02004106();
void Func_02004154();
void Func_02004164();
void Func_02004174();
void Func_0200417a();
void Func_02004186();
void Func_0200418a();
void Func_0200419a();
void Func_020041ae();
s32 Func_020041bc();
void Func_020041be();
void Func_02004264();
void Func_0200427c();

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

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call7(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6)
{
    f(a0, a1, a2, a3, a4, a5, a6);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3b3SequenceA(void)
{
    s32 record;
    s32 count;

    record = Value1(Func_020040f2, 0x200);
    if (record == 0) {
        Func_020022e0(10, 19, 16, 5, record, 10, 31);
        Func_020022f4(10, 51, 16, 5, 1, 10, 31);
        Func_02002308(42, 51, 16, 5, 2, 10, 31);
    } else {
        Func_02002322(10, 19, 16, 5, 0, 10, 31);
        Func_02002336(10, 83, 16, 5, 1, 10, 31);
        Func_0200234a(42, 83, 16, 5, 2, 10, 31);
    }
    *(volatile s32 *)Data_0200b738 = 0;
    Func_020040d6(0x20095cd, 0xc80);
    Func_020040d4(1);
    Func_02004106(1, 0, 0x2009579);
    Func_02004264(231);
    *(volatile s32 *)Data_0200b738 = 0;
    do {
        Func_020040ec(1);
        count = *(volatile s32 *)Data_0200b738;
        count = count + 1;
        *(volatile s32 *)Data_0200b738 = count;
    } while (count <= 100);
    Call1(Func_0200427c, 0x121);
    if (Value1(Func_020041bc, 0x200) == 0) {
        Call6(Func_0200418a, 0, 32, 32, 0, 32, 32);
        Call6(Func_0200419a, 32, 32, 64, 0, 32, 32);
    } else {
        Call6(Func_020041ae, 0, 64, 32, 0, 32, 32);
        Call6(Func_020041be, 32, 64, 64, 0, 32, 32);
    }
    Func_02004154(1);
    Func_02004186(1, 0, 0);
    Func_02004164(1);
    Call1(Func_0200417a, 0x20095cd);
    Func_020041be();
    Func_02004174(30);
}

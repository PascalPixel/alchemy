#include "types.h"

#define FieldScene_RunFlagGatedActorDialogue Func_02000ff0

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00002409[];
s32 Func_020029b8();
void Func_020029ce();
void Func_020029ea();
s32 Func_02002a10();
void Func_02002a14();
void Func_02002a68();
void Func_02002a6e();
s32 Func_02002a7c();
void Func_02002a7e();
void Func_02002a86();
void Func_02002a8a();
void Func_02002a9e();
void Func_02002aa0();
void Func_02002ac0();
void Func_02002ad2();
void Func_02002ad8();
void Func_02002ae8();
void Func_02002b04();
void Func_02002b06();
void Func_02002b0c();
void Func_02002b14();
void Func_02002b1e();
void Func_02002b22();
void Func_02002b28();
void Func_02002b36();
void Func_02002b3c();
void Func_02002b46();
void Func_02002b48();
void Func_02002b4c();
void Func_02002b52();
void Func_02002b54();
void Func_02002b56();
void Func_02002b60();
void Func_02002b6c();
void Func_02002b78();
void Func_02002b84();
void Func_02002b8e();
void Func_02002b96();
void Func_02002b9c();
void Func_02002ba2();

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

void FieldScene_RunFlagGatedActorDialogue(void)
{
    u32 i;
    s32 rec8;
    s32 record;
    s32 base5_2409;

    if (Value1(Func_020029b8, 0x244) != 0) {
    } else {
        Call1(Func_020029ce, 0x244);
        Func_020029ea();
        rec8 = Func_02002a10(0);
        Value3(Func_02002a7c, 8, 0, 0);
        Func_02002a86(9, 0, 0);
        Func_02002a7e(8, 1);
        Func_02002a86(9, 1);
        Func_02002a14(20);
        Call3(Func_02002ae8, 8, 0x102, 60);
        base5_2409 = (s32)Data_00002409;
        Func_02002ac0(base5_2409);
        Func_02002ad8(8, 0);
        Call3(Func_02002a6e, 0, 0x20000, 0x10000);
        Call3(Func_02002a7c, 8, 0x20000, 0x10000);
        Call3(Func_02002a8a, 9, 0x20000, 0x10000);
        Func_02002ad2(9, 4);
        Func_02002a68(35);
        Func_02002b06((base5_2409 + 1));
        Func_02002b1e(9, 0);
        Call3(Func_02002b48, 8, 0x103, 30);
        Func_02002b1e((base5_2409 + 2));
        Func_02002b36(8, 0);
        Func_02002b06(9, 3);
        Func_02002a9e(25);
        Func_02002b3c((base5_2409 + 3));
        Func_02002b54(9, 0);
        Func_02002b04(8, (*(s16 *)(rec8 + 10) - 1), *(s16 *)(rec8 + 18));
        Func_02002b22(8);
        Func_02002b14(0, 160, 216);
        Func_02002b1e(8, 152, 200);
        Func_02002b28(9, 168, 200);
        Func_02002b46(8);
        Func_02002b4c(9);
        Func_02002b52(0);
        Func_02002b84(8, 0, 0);
        Func_02002b8e(9, 0, 0);
        Func_02002b0c(12);
        Call3(Func_02002b60, 0, 160, 0x110);
        Call3(Func_02002b6c, 8, 152, 0x100);
        Call3(Func_02002b78, 9, 168, 0x100);
        Func_02002b96(8);
        Func_02002b9c(9);
        Func_02002ba2(0);
        Func_02002b56();
        Call2(Func_02002aa0, 0x2009241, 0xc80);
    }
}

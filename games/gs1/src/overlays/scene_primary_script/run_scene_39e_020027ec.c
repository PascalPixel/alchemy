#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_000017e0[];
extern u8 Data_03001ebc[];
void Func_02006bcc();
void Func_02006c00();
void Func_02006c12();
void Func_02006c18();
void Func_02006c36();
s32 Func_02006c4e();
s32 Func_02006c60();
void Func_02006c66();
void Func_02006c6a();
void Func_02006c74();
void Func_02006ca0();
void Func_02006ca2();
void Func_02006cb0();
s32 Func_02006cb8();
void Func_02006cbe();
void Func_02006cd0();
void Func_02006cde();
void Func_02006ce8();
void Func_02006cf6();
void Func_02006d04();
void Func_02006d0c();
void Func_02006d12();
void Func_02006d26();
void Func_02006d2e();
void Func_02006d32();
void Func_02006d38();
void Func_02006d40();
void Func_02006d54();
void Func_02006d5e();
s32 Func_02006d60();
s32 Func_02006d6a();
void Func_02006d78();
void Func_02006d80();
void Func_02006d86();
s32 Func_02006d96();
void Func_02006d9a();
void Func_02006da8();
void Func_02006db6();
void Func_02006dc0();
void Func_02006dd0();
void Func_02006dd2();
void Func_02006dd6();
void Func_02006dde();
s32 Func_02006de6();
void Func_02006dee();
void Func_02006df8();
void Func_02006e00();
void Func_02006e10();
s32 Func_02006e12();
void Func_02006e2e();
void Func_02006e30();
void Func_02006e32();
void Func_02006e38();
void Func_02006e3e();
void Func_02006e48();
void Func_02006e58();
void Func_02006e5a();
void Func_02006e60();
void Func_02006e6a();
void Func_02006e6c();
void Func_02006e6e();
void Func_02006e84();
s32 Func_02006e8e();
void Func_02006e90();
void Func_02006ea8();
void Func_02006eb2();
void Func_02006ec8();
void Func_02006ee2();
void Func_02006ef0();
void Func_02006ef6();
void Func_02006f02();
void Func_02006f1a();
void Func_02006f32();

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
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene39e_020027ec(void)
{
    u32 i;
    s32 record;

    Func_02006bcc();
    Call3(Func_02006c12, 0, 0x8000, 0x4000);
    Call3(Func_02006c36, 0, 168, 0x1f8);
    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c0)) = 0x100;
    Func_02006d40();
    Func_02006d54();
    Func_02006c6a(0);
    Func_02006c00(20);
    Func_02006ca0(8, 2);
    Call2(Func_02006d12, 8, 0x102);
    Func_02006c18(60);
    *(u8 *)(Func_02006c4e(8) + 91) = 0;
    Func_02006dd2(152);
    record = Func_02006c60(8);
    *(volatile s32 *)(record + 40) = 0x80000;
    Func_02006cbe(8, 1);
    Call1(Func_02006d0c, 0x17be);
    Func_02006d2e(8, 0, 20);
    Func_02006d38(8, 0, 20);
    Func_02006ce8(0, 3);
    Func_02006c66(20);
    Func_02006cf6(8, 3);
    Func_02006c74(20);
    Func_02006d5e(8, 0, 20);
    Call3(Func_02006d80, 0, 0x101, 60);
    Value2(Func_02006d60, 8, 0);
    if (Value2(Func_02006cb8, 0, 0) == 0) {
        Func_02006ca2(10);
        Func_02006d32(8, 3);
        Func_02006cb0(20);
        Func_02006d9a(8, 0, 20);
        bump_step(2);
    } else {
        Func_02006cd0(10);
        Func_02006d78(8, 2);
        Func_02006cde(20);
        bump_step(1);
        Func_02006dd6(8, 0, 20);
        Func_02006d86(8, 3);
        Func_02006d04(20);
        Func_02006dee(8, 0, 20);
    }
    Func_02006df8(8, 0, 20);
    Func_02006dc0(8, 2);
    Func_02006d26(20);
    Func_02006e10(8, 0, 20);
    Call3(Func_02006e32, 0, 0x101, 60);
    Value2(Func_02006e12, 8, 0);
    if (Value2(Func_02006d6a, 0, 0) == 1) {
        Func_02006d54(10);
        Call3(Func_02006e58, 8, 0x102, 60);
        Call1(Func_02006e2e, 0x17c8);
        Func_02006e3e(8, 0);
        L_0200299c:;
        if (Value2(Func_02006d96, 0, 0) == 1) {
            Func_02006d80(10);
            Call3(Func_02006e84, 8, 0x102, 60);
            Func_02006e5a((s32)Data_000017e0);
            Func_02006e6a(8, 0);
            goto L_0200299c;
        }
    }
    Call1(Func_02006e6a, 0x17c9);
    Func_02006da8(10);
    Func_02006e38(8, 3);
    Func_02006db6(20);
    Value2(Func_02006e8e, 8, 0);
    if (Value2(Func_02006de6, 0, 0) == 0) {
        Func_02006dd0(10);
        Func_02006e60(0, 3);
        Func_02006dde(20);
        Func_02006ec8(8, 0, 20);
        bump_step(1);
    } else {
        Func_02006e00(10);
        Func_02006ea8(8, 2);
        bump_step(1);
        Func_02006f02(8, 0, 20);
    }
    Func_02006eb2(8, 3);
    Func_02006e30(20);
    Func_02006f1a(8, 0, 20);
    Func_02006ee2(8, 2);
    Func_02006e48(20);
    Func_02006f32(8, 0, 20);
    Func_02006ee2(0, 3);
    Func_02006e60(20);
    Func_02006ef0(8, 3);
    Func_02006e6e(20);
    Func_02006ef6(8, 5);
    Call1(Func_02006e6c, 0x893);
    Func_02006e90();
}

#include "types.h"

#define FieldScene_RunActorNinePresentationCycles Func_020007b0

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_0200126a();
void Func_02001270();
void Func_02001276();
void Func_0200127c();
void Func_02001282();
void Func_02001288();
void Func_0200128e();
void Func_02001376();
void Func_02001476();
void Func_02001988();
void Func_02001994();
void Func_020019c2();
void Func_020019ce();
u8 *Func_020019fc();
void Func_020019fe();
void Func_02001a3e();
void Func_02001a46();
void Func_02001a5a();
void Func_02001a5e();
void Func_02001a68();
u8 *Func_02001a6c();
void Func_02001a74();
void Func_02001a7c();
void Func_02001a82();
void Func_02001a8a();
void Func_02001a8c();
void Func_02001a98();
void Func_02001aa0();
void Func_02001aa6();
void Func_02001b04();
void Func_02001b0e();
void Func_02001b12();
void Func_02001b14();
void Func_02001b24();
void Func_02001b28();
void Func_02001b2e();
void Func_02001b32();
void Func_02001b44();
void Func_02001b48();
void Func_02001b56();
void Func_02001b58();
void Func_02001b5e();
void Func_02001b6c();
void Func_02001b70();
void Func_02001b74();
void Func_02001b7c();
void Func_02001b82();
void Func_02001b8a();
void Func_02001b90();
void Func_02001b98();
void Func_02001ba0();
void Func_02001c0e();
void Func_02001c18();
void Func_02001c28();
void Func_02001c4e();
void Func_02001c68();
void Func_02001c70();
void Func_02001c72();
void Func_02001c78();
void Func_02001c8a();
void Func_02001c98();
void Func_02001ca6();
void Func_02001cb8();
void Func_02001cc6();
void Func_02001cdc();
void Func_02001d18();
void Func_02001d2c();
void Func_02001d42();
void Func_02001d4e();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
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

void FieldScene_RunActorNinePresentationCycles(void)
{
    u32 i;
    u8 *rec7;
    u8 *record;

    Func_020019ce();
    Func_02001a46(0, 15);
    record = Func_020019fc(0);
    Func_020019c2(record, 0);
    Func_02001988(1);
    Call1(Func_020019fe, 0x200976c);
    Func_02001994(1);
    Func_0200126a(9);
    Func_02001270(10);
    Func_02001276(11);
    Func_0200127c(12);
    Func_02001282(13);
    Func_02001288(14);
    Func_0200128e(15);
    Func_02001a5a(8, 0x200939c);
    *(s32 *)((*(u8 **)0x03001ebc + 0x1c0)) = 0x203;
    Func_02001b14();
    Func_02001b28();
    Func_02001a3e(120);
    rec7 = Func_02001a6c(9);
    Func_02001a8c(9);
    *(s32 *)(rec7 + 56) = -0x80000000;
    *(s32 *)(rec7 + 60) = -0x80000000;
    *(s32 *)(rec7 + 64) = -0x80000000;
    *(s32 *)(rec7 + 36) = 0;
    *(s32 *)(rec7 + 40) = 0;
    *(s32 *)(rec7 + 44) = 0;
    *(s32 *)(rec7 + 76) = 0;
    Func_02001a68(20);
    Call3(Func_02001aa6, 9, 0x80000, 0x40000);
    Call4(Func_02001a5e, rec7, 0xa40000, 0x900000, 0x1410000);
    Func_02001a6c(rec7);
    Call4(Func_02001a74, rec7, 0xa40000, 0x680000, 0x1410000);
    Func_02001a82(rec7);
    Call4(Func_02001a8a, rec7, 0xcc0000, 0x7c0000, 0x1410000);
    Func_02001a98(rec7);
    Call4(Func_02001aa0, rec7, 0x900000, 0, 0xa90000);
    Func_02001b0e(8);
    Func_02001a7c(1);
    Call3(Func_02001b70, 8, 0x8000, 0);
    Call3(Func_02001b8a, 8, 0x103, 60);
    Call3(Func_02001b28, 9, 0x20000, 0x10000);
    Func_02001376(9);
    Func_02001b04(20);
    Func_02001b44(8, 0x200939c);
    Func_02001b12(120);
    Func_02001b58(9);
    *(s32 *)(rec7 + 56) = -0x80000000;
    *(s32 *)(rec7 + 60) = -0x80000000;
    *(s32 *)(rec7 + 64) = -0x80000000;
    *(s32 *)(rec7 + 36) = 0;
    *(s32 *)(rec7 + 40) = 0;
    *(s32 *)(rec7 + 44) = 0;
    *(s32 *)(rec7 + 76) = 0;
    Func_02001b2e(20);
    Call3(Func_02001b6c, 9, 0x80000, 0x40000);
    Call4(Func_02001b24, rec7, 0xa40000, 0x900000, 0x1410000);
    Func_02001b32(rec7);
    Call3(Func_02001b90, 9, 0x50000, 0x28000);
    Call4(Func_02001b48, rec7, 0xa40000, 0x680000, 0x1410000);
    Func_02001b56(rec7);
    Call4(Func_02001b5e, rec7, 0xa40000, 0x720000, 0x1410000);
    Func_02001b6c(rec7);
    Call4(Func_02001b74, rec7, 0xa40000, 0x680000, 0x1410000);
    Func_02001b82(rec7);
    Call4(Func_02001b8a, rec7, 0xcc0000, 0x7c0000, 0x1410000);
    Func_02001b98(rec7);
    Call4(Func_02001ba0, rec7, 0x900000, 0, 0xa90000);
    Func_02001c0e(8);
    Func_02001b7c(1);
    Call3(Func_02001c70, 8, 0x8000, 0);
    Call3(Func_02001c8a, 8, 0x103, 60);
    Call3(Func_02001c28, 9, 0x20000, 0x10000);
    Func_02001476(9);
    Func_02001c68(8, 4, 20);
    Func_02001c72(8, 6, 40);
    Func_02001d18(29);
    Call1(Func_02001c18, 0x8f0);
    Call1(Func_02001ca6, 0x1e49);
    Func_02001cb8(16, 0, 20);
    Call3(Func_02001cdc, 8, 0x100, 0);
    Call3(Func_02001c98, 8, 164, 0x158);
    Func_02001c4e(40);
    Func_02001cc6(8, 2);
    Func_02001d42();
    Func_02001d4e();
    Func_02001d2c(12);
    Func_02001c78();
}

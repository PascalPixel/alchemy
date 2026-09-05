#include "types.h"

#define FieldScene_RunScene3b6SequenceA Func_0200013c

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
s32 Func_02000afe();
void Func_02000b06();
void Func_02000b14();
void Func_02000b20();
void Func_02000b24();
void Func_02000b26();
void Func_02000b30();
void Func_02000b4c();
void Func_02000b5a();
void Func_02000b66();
void Func_02000b7e();
void Func_02000ba0();
void Func_02000ba8();
void Func_02000bb6();
void Func_02000bbe();
void Func_02000bd6();
void Func_02000be8();
void Func_02000bf2();
void Func_02000bf4();
void Func_02000bfa();
void Func_02000c08();
void Func_02000c0c();
void Func_02000c12();
void Func_02000c18();
void Func_02000c1a();
void Func_02000c28();
void Func_02000c38();
void Func_02000c46();
void Func_02000c48();
void Func_02000c54();
void Func_02000c62();
void Func_02000c6c();
void Func_02000c7e();
void Func_02000c88();
void Func_02000c8c();
void Func_02000c92();
void Func_02000c9a();
void Func_02000cb2();
void Func_02000cbe();
void Func_02000cd8();
void Func_02000cdc();
void Func_02000ce0();
void Func_02000cf0();
void Func_02000d12();
void Func_02000d14();
void Func_02000d1e();
void Func_02000d30();
void Func_02000d44();
void Func_02000d6a();

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

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3b6SequenceA(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    Func_02000b20();
    Call1(Func_02000b7e, 0x2394);
    Func_02000b24(40);
    rec7 = Value4(Func_02000afe, 0x11c, 0x2580000, 0, 0x3380000);
    Func_02000b26(rec7, 0);
    Func_02000b06(rec7, 6);
    Func_02000b4c(10);
    Func_02000b14(rec7, 1);
    Func_02000b5a(40);
    Func_02000b30(rec7);
    Func_02000b66(2);
    Call3(Func_02000bf2, 25, 0x100, 50);
    Call3(Func_02000ba8, 25, 0x10000, 0x8000);
    Call3(Func_02000bbe, 25, 0x258, 0x350);
    Call3(Func_02000c12, 25, 0xc000, 0);
    Func_02000ba0(40);
    Func_02000c18(25, 0);
    Func_02000c08(25, 2);
    Func_02000bb6(30);
    Call3(Func_02000bf4, 25, 0x238, 0x350);
    Call3(Func_02000c48, 25, 0xc000, 0);
    Func_02000bd6(30);
    Call3(Func_02000c62, 25, 0x108, 50);
    Func_02000be8(20);
    Call3(Func_02000c8c, 0, 0, -16);
    Func_02000bfa(20);
    Call3(Func_02000c7e, 25, 0x3000, 0);
    Func_02000c0c(30);
    Func_02000c6c(25, 2);
    Func_02000c1a(20);
    Func_02000c92(25, 0);
    Func_02000c28(20);
    Call3(Func_02000cb2, 0, 0x101, 50);
    Func_02000c38(20);
    Func_02000c88(25, 4);
    Func_02000c46(20);
    Func_02000cbe(25, 0);
    Func_02000c54(30);
    Call3(Func_02000ce0, 25, 0x102, 50);
    Func_02000cd8(25, 0);
    Call3(Func_02000c9a, 25, 0x16666, 0xb333);
    Func_02000d14(25, 16, 0);
    Func_02000d1e(25, 0, 32);
    Func_02000c8c(20);
    Func_02000cdc(25, 3);
    Func_02000c9a(20);
    Func_02000d12(25, 0);
    Func_02000d44(0, 16, 0);
    Call3(Func_02000d30, 0, 0x8000, 0);
    Func_02000cbe(20);
    Call3(Func_02000cf0, 25, 0x1cccc, 0xe666);
    Func_02000d6a(25, 0, 48);
    Func_02000d14(25, 0, 0);
    Func_02000cf0();
}

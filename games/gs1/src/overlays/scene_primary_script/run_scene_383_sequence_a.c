#include "types.h"

#define FieldScene_RunSetupSequence Func_02000b48

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_02001b36();
void Func_020051f2();
void Func_02005222();
void Func_02005228();
void Func_02005232();
void Func_0200525c();
void Func_0200525e();
void Func_0200526c();
void Func_02005274();
void Func_02005290();
void Func_020052aa();
void Func_020052b6();
void Func_020052c6();
void Func_020052d0();
void Func_020052ec();
void Func_020052fe();
void Func_02005346();
void Func_02005890();
s32 Func_020058ce();
void Func_020058da();
void Func_020058e4();
void Func_020058ee();
void Func_02005912();
void Func_02005928();
void Func_02005932();
void Func_02005950();
void Func_0200595a();
void Func_0200595e();
void Func_02005964();
void Func_02005992();
void Func_020059a0();
void Func_020059b6();
void Func_020059de();
void Func_020059e6();
void Func_020059f4();
void Func_020059f8();
void Func_020059fa();
void Func_02005a0a();
void Func_02005a0c();
void Func_02005a3c();
void Func_02005a54();
void Func_02005a58();
void Func_02005a6c();
void Func_02005a70();
void Func_02005a74();
void Func_02005a7c();
void Func_02005a7e();
void Func_02005a94();
void Func_02005aa0();
s32 Func_02005abe();
u8 *Func_02005ae2();
void Func_02005af6();
void Func_02005b0a();
void Func_02005b12();
void Func_02005b14();
void Func_02005b16();
u8 *Func_02005b1c();
void Func_02005b1e();
void Func_02005b34();
void Func_02005b42();
void Func_02005b48();
void Func_02005b4e();
void Func_02005b54();
void Func_02005b5e();
void Func_02005b6e();
void Func_02005b7c();
void Func_02005b8e();
void Func_02005bb6();
void Func_02005c58();
void Func_02005cca();

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

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunSetupSequence(void)
{
    u32 i;
    u8 *record;

    Func_02005890();
    Call3(Func_020058da, 0, 0xcccc, 0x6666);
    Call3(Func_020058e4, 1, 0xcccc, 0x6666);
    Call3(Func_020058ee, 2, 0xcccc, 0x6666);
    Func_02005a94(19);
    Call3(Func_02005932, 0, 0x180, 0x198);
    Call3(Func_020059b6, 0, 0xc000, 0);
    Call3(Func_02005964, 1, 0x1800000, 0x1980000);
    Call3(Func_0200595a, 1, 0x170, 0x198);
    Call3(Func_020059de, 1, 0xd000, 20);
    if (Value1(Func_020058ce, 0x850) != 0) {
    } else {
        Call1(Func_020058e4, 0x850);
        Func_02005228(2, 0);
        Func_02005912(40);
        Func_0200525e();
        Call1(Func_020059f4, 0x1256);
        Func_02005b0a(60);
        Func_02005928(30);
        Func_02005232(2, 3, 30);
        Func_020051f2(2, 30);
        Func_020059fa(0, 1);
        Func_02005a0a(1, 1);
        Func_02005950(20);
        Func_02005274(2, 0);
        Func_0200595e(40);
        Func_020052aa();
        Func_02005222(2, 30);
        Func_0200525c(0, 1, 50);
        Call3(Func_02005a70, 0, 0xc000, 0);
        Call3(Func_02005a7c, 1, 0xd000, 0);
        Func_02005992(20);
        Func_020052b6(2, 0);
        Func_020059a0(40);
        Func_020052ec();
        Func_02005a6c(2, 1);
        Func_0200526c(2, 50);
        Func_02005a54(0, 3);
        Func_020052c6(1, 3, 20);
        Func_020052d0(2, 3, 20);
        Func_02005290(2, 40);
        Func_02005aa0(2, 1);
        Func_020059e6(30);
        Call3((void (*)())Func_02005ae2, 2, 0xc000, 0);
        Func_020059f8(30);
        Call3(Func_02005a7e, 2, 0x178, 0x178);
        Func_02005a0c(40);
        Func_020052fe(0, 1, 50);
        Call3(Func_02005b12, 0, 0xc000, 0);
        Call3(Func_02005b1e, 1, 0xd000, 0);
        Func_02005af6(2, 1);
        Func_02005a3c(50);
        Func_02005346(2, 3, 30);
        Call3(Func_02005b42, 2, 0x4000, 0);
        Func_02005a58(10);
        Func_02005b48(2, 0);
        Func_02005b48(2, 0);
        goto L_02000d44;
    }
    Func_02005c58(60);
    Call1(Func_02005b4e, 0x125d);
    Func_02005b5e(2, 0);
    L_02000d44:;
    if (Func_02005abe(0, 0) == 0) {
        Func_02001b36();
        Call1(Func_02005a74, 0x856);
        Func_02005b34(2, 2);
        record = Func_02005ae2(0);
        if (record != 0) {
            Func_02005b14(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Func_02005b42(2);
        Func_02005b54(2, 0, 0);
    } else {
        Func_02005bb6(2, 0);
    }
    Func_02005b6e(1, 2);
    record = Func_02005b1c(0);
    if (record != 0) {
        Func_02005b4e(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_02005b7c(1);
    Func_02005b8e(1, 0, 0);
    Func_02005cca();
    Func_02005b16();
}

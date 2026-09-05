#include "types.h"

#define FieldScene_RunScene383_02002db4 Func_02002db4

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00000015[];
extern u8 Data_02000240[];
void Func_02007458();
void Func_020074a2();
void Func_020074f2();
void Func_0200750e();
void Func_0200751c();
void Func_02007538();
void Func_0200756c();
void Func_02007ae8();
void Func_02007b3a();
void Func_02007b8e();
void Func_02007b9c();
void Func_02007baa();
void Func_02007bce();
void Func_02007bdc();
void Func_02007be2();
void Func_02007bea();
void Func_02007bf8();
void Func_02007c16();
void Func_02007c22();
void Func_02007c2e();
void Func_02007c5e();
void Func_02007c62();
void Func_02007c64();
void Func_02007c70();
void Func_02007c7c();
void Func_02007c9c();
void Func_02007caa();
void Func_02007cac();
void Func_02007cb2();
void Func_02007cba();
void Func_02007cc6();
void Func_02007cce();
void Func_02007ce8();
void Func_02007d34();
void Func_02007d40();
void Func_02007d4c();
void Func_02007d5c();
void Func_02007dc6();
void Func_02007dd6();
void Func_02007dd8();
extern u8 Data_03001ebc[];

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene383_02002db4(void)
{
    u32 i;
    s32 record;
    s16 base3_2000240;

    Call3(Func_02007b8e, 10, 0x3180000, 0x1a00000);
    Call3(Func_02007b9c, 11, 0x3200000, 0x1900000);
    Call3(Func_02007baa, 12, 0x3080000, 0x1a00000);
    Call3(Func_02007c16, 10, 0x3000, 0);
    Call3(Func_02007c22, 11, 0x3000, 0);
    Call3(Func_02007c2e, 12, 0x3000, 0);
    Call3(Func_02007bdc, 0, 0x3180000, 0x1b80000);
    Call3(Func_02007bea, 1, 0x3280000, 0x1b00000);
    Call3(Func_02007bf8, 2, 0x3180000, 0x1c80000);
    Call3(Func_02007c64, 0, 0xc000, 0);
    Call3(Func_02007c70, 1, 0xb000, 0);
    Call3(Func_02007c7c, 2, 0xb000, 0);
    Func_02007c5e(8, 10, 0);
    *(s32 *)(*(u8 **)Data_03001ebc + 0x1c0) = 0x209;
    Func_02007cba(0, 0);
    Func_02007cce();
    Func_02007b3a();
    Func_02007ae8(1);
    *(s32 *)((*(u8 **)Data_03001ebc + 0x1c8)) = 32;
    Func_02007458();
    Func_02007bce(60);
    Call1(Func_02007cac, 0x12e1);
    Func_02007c9c(11, 1);
    Func_02007be2(20);
    Func_020074a2(11, 30);
    Func_02007cb2(12, 1);
    Func_02007bf8(20);
    Func_02007ce8(12, 0);
    Func_020074f2(10, 11, 30);
    Func_02007caa(10, 3);
    Func_0200751c(11, 3, 30);
    Func_0200750e(10, 12, 30);
    Func_02007cc6(10, 3);
    Func_02007538(12, 3, 40);
    Call3(Func_02007d34, 10, 0x3000, 0);
    Call3(Func_02007d40, 11, 0x3000, 0);
    Call3(Func_02007d4c, 12, 0x3000, 0);
    Func_02007c62(20);
    Func_0200756c(10, 4, 20);
    Func_02007d5c(10, 0);
    *(s32 *)(*(u8 **)Data_03001ebc + 0x1c0) = 0x200;
    Func_02007dc6((s32)Data_00000015, 17);
    Func_02007dd6((s32)Data_00000015, 16);
    base3_2000240 = (s32)Data_02000240;
    *(u8 *)((base3_2000240 + 0x22b)) = 3;
    Func_02007dd8(12, 5);
}

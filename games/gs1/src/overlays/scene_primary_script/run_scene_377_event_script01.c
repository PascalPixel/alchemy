#include "types.h"

#define RunEventScript01 Func_02000f90
#define RunEventScript01 Func_02000f90

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00001c6f[];
extern u8 Data_00001c79[];
void Func_0200289e();
void Func_020028a0();
void Func_020028d2();
void Func_020028fa();
void Func_02002914();
void Func_0200292c();
void Func_02002934();
void Func_0200293a();
void Func_02002950();
void Func_02002962();
void Func_02002972();
void Func_02002984();
void Func_02002986();
void Func_0200298c();
void Func_0200299c();
void Func_0200299e();
void Func_020029a0();
void Func_020029a8();
void Func_020029b4();
void Func_020029c4();
void Func_020029cc();
s32 Func_020029da();
void Func_020029ec();
void Func_020029f0();
void Func_020029f6();
s32 Func_020029f8();
void Func_02002a08();
void Func_02002a1c();
void Func_02002a2e();
void Func_02002a32();
s32 Func_02002a34();
void Func_02002a40();
void Func_02002a50();
s32 Func_02002a52();
void Func_02002a62();
void Func_02002a64();
void Func_02002a66();
void Func_02002a76();
void Func_02002a7e();
void Func_02002a90();
s32 Func_02002a98();
void Func_02002ab8();
void Func_02002ac0();
void Func_02002ad0();
void Func_02002ae0();
void Func_02002ae2();
s32 Func_02002af2();
u8 *Func_02002afe();
void Func_02002b02();
s32 Func_02002b0e();
void Func_02002b18();
void Func_02002b1a();
void Func_02002b20();
void Func_02002b22();
void Func_02002b30();
void Func_02002b36();
void Func_02002b3a();
void Func_02002b4a();
void Func_02002b4c();
void Func_02002b52();
void Func_02002b5c();
void Func_02002b62();
u8 *Func_02002b68();
void Func_02002b72();
void Func_02002b74();
void Func_02002b82();
void Func_02002b86();
void Func_02002b96();
void Func_02002b9a();
void Func_02002bce();
void Func_02002bd0();
void Func_02002bd4();
void Func_02002bda();
void Func_02002bf8();
void Func_02002c0a();
void Func_02002c14();
void Func_02002c1e();
void Func_02002c20();
void Func_02002c22();
s32 Func_02002c32();
void Func_02002c3e();
void Func_02002c48();
void Func_02002c4a();
void Func_02002c4c();
void Func_02002c54();
void Func_02002c58();
void Func_02002c60();
void Func_02002c6a();
void Func_02002c78();
void Func_02002c7e();
void Func_02002c9a();
void Func_02002cae();
void Func_02002d10();
void Func_02002d12();
void Func_02002d26();
void Func_02002d3e();
void Func_02002d46();
void Func_02002d6a();

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
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void RunEventScript01(void)
{
    u32 i;
    u8 *record;
    s32 base5_1c6f;

    Func_020028a0();
    Call3(Func_020028d2, 0, 0x9999, 0x4ccc);
    Call3(Func_02002914, 0, 0x239, 0x189);
    Call3(Func_020029a0, 0, 0x4000, 40);
    Func_02002950(8, 2);
    Call1(Func_02002986, 0x1c66);
    Func_020029a8(8, 0, 80);
    Call3(Func_020029da, 8, 0x101, 60);
    Func_02002972(8, 1);
    Func_020029c4(8, 0, 60);
    Func_0200298c(8, 2);
    Func_020028fa(80);
    Call3(Func_02002934, 8, 0xcccc, 0x6666);
    Call3(Func_02002962, 8, 0x248, 0x196);
    Func_020028a0(11);
    Func_0200289e(12);
    Func_0200299e(8, 12);
    Func_0200292c(80);
    Func_020029cc(8, 2);
    Func_0200293a(40);
    Func_02002a1c(8, 0, 40);
    Call3(Func_02002a50, 8, 0x108, 40);
    Func_02002a32(8, 0, 40);
    Call3(Func_02002a64, 0, 0x105, 60);
    Func_020029ec(8, 13);
    Call3(Func_02002a76, 8, 0x103, 0);
    Func_020029f6(8, 11);
    Func_02002984(40);
    Func_02002a66(8, 0, 40);
    Func_02002a2e(8, 1);
    Func_0200299c(20);
    Func_02002a7e(8, 0, 20);
    Func_02002a2e(8, 12);
    Func_020029b4(20);
    Call3(Func_02002ac0, 0, 0x102, 60);
    Func_02002a40(8, 13);
    Value2(Func_02002a98, 8, 0);
    if (Value2(Func_020029f8, 0, 0) == 1) {
        bump_step(1);
    }
    if (Value1(Func_020029da, 0x81c) != 0) {
        Call3(Func_02002b02, 8, 0x102, 60);
    }
    Func_02002a08(20);
    Func_02002ae0(8, 0);
    Call3(Func_02002b1a, 8, 0x107, 60);
    base5_1c6f = (s32)Data_00001c6f;
    Func_02002ae2(base5_1c6f);
    Value2(Func_02002af2, 8, 0);
    if (Value2(Func_02002a52, 0, 0) == 1) {
        bump_step(1);
    }
    if (Value1(Func_02002a34, 0x81c) != 0) {
        Call3(Func_02002b5c, 8, 0x102, 60);
    }
    Func_02002a62(20);
    Func_02002b3a(8, 0);
    Call3(Func_02002b74, 8, 0x107, 60);
    Func_02002b3a((base5_1c6f + 3));
    Func_02002b52(8, 0);
    Func_02002b22(8, 1);
    Func_02002a90(20);
    Func_02002b18(8, 13);
    Func_02002b30(8, 2);
    Func_02002b82(8, 0, 40);
    Func_02002b4a(8, 1);
    Func_02002ab8(20);
    Func_02002b9a(8, 0, 40);
    Func_02002b62(8, 2);
    Func_02002ad0(40);
    record = Func_02002afe(0);
    {
        s32 shown = 0;
    
        *(u16 *)(record + 6) = shown;
    }
    Func_020029f0(1);
    *(u8 *)(Func_02002b0e(0) + 90) &= 254;
    Call3(Func_02002b4c, 0, 0x22e, 0x184);
    Call3(Func_02002b36, 8, 0x13333, 0x9999);
    Func_02002b86(8, 14);
    Call3(Func_02002b72, 8, 0x24a, 0x190);
    Func_02002b20(40);
    Call3(Func_02002b96, 8, 0x244, 0x17e);
    Call3(Func_02002c22, 8, 0x8000, 40);
    {
        u8 *record = Func_02002b68(0);
        u8 value = *(volatile u8 *)&record[90];
    
        record[90] = (u8)(value | 1);
    }
    Call3(Func_02002c3e, 8, 0xc000, 8);
    Func_02002c48(8, 0, 8);
    Call3(Func_02002c54, 8, 0x4000, 8);
    Call3(Func_02002c60, 8, 0x8000, 10);
    Func_02002c0a(8, 4, 20);
    Func_02002c14(8, 6, 40);
    Func_02002c1e(8, 4, 20);
    Func_02002c78(8, 0, 40);
    Call3(Func_02002bda, 8, 0x6666, 0x3333);
    Call3(Func_02002c20, 8, 0x23c, 0x180);
    Func_02002c9a(8, 0, 20);
    Func_02002c4a(0, 3);
    Func_02002bd0(20);
    Func_02002c58(8, 3);
    Call1(Func_02002bce, 0x81e);
    Call1(Func_02002bd4, 0x203);
    Func_02002bf8();
}

#include "types.h"

#define FieldScene_ArrangeFourActors Func_02001a0c

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_0200642a();
s32 Func_02006432();
void Func_02006460();
void Func_0200646e();
void Func_0200647c();
void Func_0200648a();
void Func_02006498();
s32 Func_020064dc();
s32 Func_020064ec();
void Func_020064fe();
void Func_0200650c();
s32 Func_02006518();
void Func_02006526();
void Func_0200653e();
void Func_02006560();
s32 Func_0200656c();
void Func_02006580();
s32 Func_0200658c();
void Func_02006590();
void Func_02006598();
void Func_020065a0();
void Func_020065a6();
void Func_020065a8();
s32 Func_020065ac();
void Func_020065b0();
void Func_020065b8();
void Func_020065be();
void Func_020065c0();
void Func_020065c2();
void Func_020065c6();
void Func_020065d0();
void Func_020065de();
void Func_020065e0();
void Func_020065e6();
void Func_020065fe();
void Func_02006600();
void Func_02006606();
void Func_02006626();
void Func_02006628();
void Func_02006630();
void Func_02006632();
void Func_0200663a();
void Func_0200663c();
void Func_020066c4();

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

void FieldScene_ArrangeFourActors(s32 a0)
{
    u32 i;
    s32 p10;
    s32 p8;
    s32 p9;
    s32 p9b;
    s32 record;

    record = Func_02006432();
    p9 = *(s16 *)(record + 10);
    p10 = *(s16 *)(record + 18);
    Func_0200642a();
    Call3(Func_02006460, a0, 0x10000, 0x8000);
    Call3(Func_0200646e, 0, 0x10000, 0x8000);
    Call3(Func_0200647c, 1, 0x10000, 0x8000);
    Call3(Func_0200648a, 2, 0x10000, 0x8000);
    Call3(Func_02006498, 3, 0x10000, 0x8000);
    Value3(Func_020064ec, 0, ((s32)p9 << 16), (((s32)p10 << 16) + -0x300000));
    Func_020064fe(1, (((s32)p9 << 16) + -0x100000), (-0x280000 + ((s32)p10 << 16)));
    p8 = (-0x280000 + ((s32)p10 << 16));
    Func_0200650c(2, (((s32)p9 << 16) + 0x100000), p8);
    Value3(Func_02006518, 3, ((s32)p9 << 16), (((s32)p10 << 16) + -0x200000));
    Func_02006526(a0, ((s32)p9 << 16), (((s32)p10 << 16) + -0x500000));
    record = Func_020064dc(0);
    {
        s32 shown = 0xc000;
    
        *(u16 *)(record + 6) = shown;
    }
    Func_020065c2(0, 0);
    Func_02006626();
    Func_0200663a();
    Call1(Func_02006590, 0x20ed);
    Func_02006560(a0, 3);
    Func_020065b0(a0, 0);
    Func_02006580(a0, 2);
    Func_020065c0(a0, 0);
    Func_02006590(a0, 2);
    Func_020065d0(a0, 0);
    Func_020065a0(a0, 2);
    Func_020065e0(a0, 0);
    Func_02006598(3, 3);
    Func_020065a0(1, 3);
    Func_020065a8(2, 3);
    Func_020065b8(0, 3);
    Func_0200653e(6);
    Func_020065be(1, 2);
    record = Value1(Func_0200656c, 0);
    if (record != 0) {
        Func_020065a6(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_020065de(2, 2);
    record = Value1(Func_0200658c, 0);
    if (record != 0) {
        Func_020065c6(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_020065fe(3, 2);
    record = Value1(Func_020065ac, 0);
    if (record != 0) {
        Func_020065e6(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_02006606(a0, (p9 - 16), (p10 - 64));
    Func_02006628(1, 0, 0);
    Func_02006632(2, 0, 0);
    Func_0200663c(3, 0, 0);
    Func_02006630(a0, (p9 - 16), (p10 - 16));
    Func_0200663a(a0, p9, p10);
    Func_020066c4(a0, 0xc000, 10);
    Func_02006600();
    p9b = (p9 - 16);
}

#include "types.h"

#define RunEventScript01 Func_0200069c

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_0000002d[];
extern u8 Data_02000240[];
void Func_02001c08();
void Func_02001c66();
void Func_02001ccc();
s32 Func_0200216a();
void Func_02002188();
void Func_020021d2();
void Func_020021e0();
void Func_020021ee();
s32 Func_020021fa();
void Func_020021fc();
void Func_02002204();
s32 Func_02002214();
void Func_02002226();
s32 Func_0200222a();
s32 Func_0200223e();
s32 Func_02002256();
void Func_02002258();
void Func_0200226a();
void Func_0200226c();
void Func_0200227c();
s32 Func_02002284();
s32 Func_0200228c();
void Func_0200228e();
void Func_0200229c();
s32 Func_020022a4();
void Func_020022a6();
void Func_020022a8();
void Func_020022b6();
void Func_020022bc();
void Func_020022ea();
void Func_020022f8();
void Func_0200230e();
void Func_0200231a();
void Func_02002324();
void Func_02002326();
void Func_0200232c();
void Func_02002332();
void Func_02002334();
void Func_0200233c();
void Func_0200234c();
void Func_0200237a();
void Func_02002384();
void Func_0200238c();
void Func_02002394();
void Func_02002398();
void Func_020023a4();
void Func_020023b0();
void Func_020023b4();
void Func_020023bc();
void Func_020023cc();
void Func_020023d4();
void Func_02002406();
void Func_02002410();
void Func_0200241a();
void Func_02002424();
void Func_02002490();

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void RunEventScript01(void)
{
    u32 i;
    s32 rec8;
    s32 record;
    s32 base3_2000240;

    rec8 = Value1(Func_0200216a, 3);
    Func_02002188();
    Func_020022a6(17);
    Call1(Func_02002204, 0x14ce);
    Call3(Func_02002226, 0x8009, 0, 20);
    Func_020022bc(29);
    Call3(Func_020021d2, 0, 0x10000, 0x8000);
    Call3(Func_020021e0, 1, 0x10000, 0x8000);
    Call3(Func_020021ee, 2, 0x10000, 0x8000);
    Call3(Func_020021fc, 3, 0x10000, 0x8000);
    *(u8 *)(Func_020021fa(3) + 35) &= 254;
    Func_0200228e(3, 2);
    *(u8 *)(Func_02002214(0) + 35) &= 254;
    Value2(Func_020022a4, 0, 2);
    record = Value1(Func_0200222a, 0);
    if (record != 0) {
        Func_02002258(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Value1(Func_0200223e, 0);
    if (record != 0) {
        Func_0200226c(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    if (rec8 != 0) {
        record = Value1(Func_02002256, 0);
        if (record != 0) {
            Call3((void (*)())Func_02002284, 3, *(s32 *)(record + 8), *(s32 *)(record + 16));
        }
        Func_0200227c(3, 0x2009e2c);
    }
    Value2(Func_02002284, 0, 0x2009db4);
    Value2(Func_0200228c, 1, 0x2009ddc);
    Call2(Func_0200229c, 2, 0x2009e04);
    Func_0200226a(10);
    Call3(Func_0200230e, 0, 0xc000, 0);
    Call3(Func_0200231a, 1, 0xc000, 0);
    Call3(Func_02002326, 3, 0xc000, 0);
    Call3(Func_02002332, 2, 0xc000, 40);
    Func_020022ea(8, 11);
    Func_020022a8(10);
    Func_020022f8(8, 8);
    Func_020022b6(20);
    Func_02001c08(8);
    Call2(Func_0200234c, 0x8008, 0);
    Func_02002324(0, 2);
    Func_0200232c(1, 2);
    Func_02002334(3, 2);
    Func_0200233c(2, 2);
    Call3(Func_02002398, 0, 0x100, 0);
    Call3(Func_020023a4, 1, 0x100, 0);
    Call3(Func_020023b0, 3, 0x100, 0);
    Call3(Func_020023bc, 2, 0x100, 60);
    Func_02001c66(11);
    Call3(Func_020023b4, 0x8008, 0, 10);
    Func_02002384(0, 1);
    Func_0200238c(1, 1);
    Func_02002394(3, 1);
    Func_020023a4(2, 1);
    Call2(Func_020023d4, 0x8008, 0);
    Call2(Func_02002406, 0, 0x102);
    Call2(Func_02002410, 1, 0x102);
    Call2(Func_0200241a, 3, 0x102);
    Call2(Func_02002424, 2, 0x102);
    Func_0200237a(40);
    Func_02001ccc(11);
    Call2(Func_02002410, 0x8008, 0);
    *(s32 *)((*(u8 **)0x03001ebc + 0x1c0)) = 0x200;
    *(s32 *)((*(u8 **)0x03001ebc + 0x1c8)) = 64;
    base3_2000240 = (s32)Data_02000240;
    *(u8 *)((base3_2000240 + 0x22b)) = 3;
    Func_02002490((s32)Data_0000002d, 19);
    Func_02002490(36, 0);
    Func_020023cc();
}

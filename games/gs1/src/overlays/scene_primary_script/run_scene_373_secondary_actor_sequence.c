#include "types.h"

#define FieldScene_RunSecondaryActorSequence Func_02002cb0

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00001c45[];
void Func_02008c70();
void Func_02008cf2();
void Func_02008cfc();
s32 Func_02008d20();
void Func_02008d32();
void Func_02008d38();
void Func_02008d42();
void Func_02008d52();
void Func_02008d7c();
void Func_02008d86();
s32 Func_02008d92();
void Func_02008db8();
void Func_02008dc8();
void Func_02008dcc();
void Func_02008dd4();
void Func_02008dd6();
void Func_02008de0();
void Func_02008de2();
void Func_02008de4();
void Func_02008e00();
void Func_02008e0a();
void Func_02008e10();
void Func_02008e1c();
void Func_02008e24();
void Func_02008e2e();
s32 Func_02008e3a();
void Func_02008e5a();
void Func_02008e62();
void Func_02008e70();
void Func_02008e78();
void Func_02008e7a();
void Func_02008e7e();
void Func_02008e88();
void Func_02008e92();
s32 Func_02008ea0();
void Func_02008eaa();
void Func_02008eac();
void Func_02008eae();
void Func_02008ebe();
void Func_02008ec0();
void Func_02008ece();
void Func_02008ed8();
void Func_02008eda();
void Func_02008edc();
void Func_02008ee2();
void Func_02008eec();
void Func_02008ef6();
void Func_02008efa();
void Func_02008f08();
void Func_02008f14();
void Func_02008f1a();
void Func_02008f2c();
void Func_02008f32();

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

void FieldScene_RunSecondaryActorSequence(void)
{
    u32 i;
    s32 record;
    s32 base5_1c45;

    Func_02008c70();
    Func_02008d38(0, 0);
    Func_02008d32(8, 0, 20);
    base5_1c45 = (s32)Data_00001c45;
    Func_02008d52(base5_1c45);
    Func_02008d32(8, 2);
    Func_02008d7c(8, 0, 20);
    Call2(Func_02008dc8, 0x10000, 0x2000);
    Call4(Func_02008de0, 0x18e0000, -1, 0x2460000, 1);
    Call3(Func_02008cf2, 0, 0xcccc, 0x6666);
    Call3(Func_02008cfc, 1, 0xcccc, 0x6666);
    Call3(Func_02008d42, 0, 0x1a4, 0x260);
    Call3(Func_02008dd6, 0, 0xa000, 0);
    Call3(Func_02008de2, 8, 0x3000, 0);
    record = Value1(Func_02008d20, 0);
    if (record != 0) {
        Func_02008d86(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Call3(Func_02008d7c, 1, 0x192, 0x260);
    Call3(Func_02008e10, 1, 0xd000, 20);
    Call2(Func_02008e00, 0x1001, 0);
    Call3(Func_02008e24, 8, 0x5000, 20);
    Func_02008dcc(8, 3);
    Call2(Func_02008e1c, 0x4008, 0);
    Func_02008dd4(0, 3);
    Func_02008de4(1, 3);
    Func_02008d52(20);
    Func_02008e0a(8, 2);
    Value2(Func_02008e3a, 0x4008, 0);
    if (Value2(Func_02008d92, 0, 0) == 1) {
        bump_step(1);
        Func_02008e2e(8, 1);
    }
    Call3(Func_02008e78, 0x4008, 0, 40);
    Call3(Func_02008eaa, 8, 0x105, 60);
    Func_02008e70((base5_1c45 + 6));
    Call3(Func_02008e92, 0x4008, 0, 20);
    Func_02008e62(1, 1);
    Func_02008db8(40);
    Call3(Func_02008eaa, 0x1001, 0, 40);
    Func_02008e7a(8, 1);
    Call3(Func_02008ece, 8, 0xd000, 20);
    Call2(Func_02008ebe, 0x4008, 0);
    Func_02008e7e(1, 3);
    Call3(Func_02008ed8, 0x1001, 0, 120);
    Call3(Func_02008ee2, 0x4008, 0, 20);
    Call3(Func_02008f14, 1, 0x105, 40);
    Call3(Func_02008ef6, 0x1001, 0, 40);
    Func_02008eae(8, 4);
    Call3(Func_02008f08, 0x4008, 0, 20);
    Func_02008ec0(1, 3);
    Func_02008e2e(40);
    Call3(Func_02008f32, 8, 0x5000, 20);
    Call3(Func_02008f2c, 0x4008, 0, 10);
    Func_02008edc(0, 3);
    Func_02008eec(1, 3);
    Func_02008e5a(20);
    Func_02008efa(8, 3);
    Func_02008efa(1, 2);
    record = Value1(Func_02008ea0, 0);
    if (record != 0) {
        Func_02008eda(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_02008f08(1);
    Func_02008f1a(1, 0, 0);
    Call1(Func_02008e88, 0x303);
    Func_02008eac();
}

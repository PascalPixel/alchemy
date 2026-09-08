#include "types.h"

#define FieldScene_RunActorThirteenStagingSequence Func_020012a0

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
s32 Func_02002d0c();
void Func_02002d36();
void Func_02002d64();
s32 Func_02002d70();
void Func_02002da2();
void Func_02002dac();
void Func_02002dce();
void Func_02002dd8();
void Func_02002de2();
void Func_02002df2();
void Func_02002df8();
void Func_02002e0a();
void Func_02002e18();
void Func_02002e20();
void Func_02002e28();
void Func_02002e28_a();
void Func_02002e36();
void Func_02002e40();
void Func_02002e6e();
void Func_02002e7a();
void Func_02002e7c();
void Func_02002e96();
void Func_02002e9c();
void Func_02002ea8();
void Func_02002eb4();
void Func_02002ebc();
void Func_02002ec0();
void Func_02002ec0_a();
void Func_02002eca();
void Func_02002ece();
void Func_02002ed6();
void Func_02002ed6_a();
void Func_02002eee();
void Func_02002eee_a();
void Func_02002efa();
void Func_02002f06();
void Func_02002f08();
void Func_02002f0e();
void Func_02002f26();
void Func_02002f26_a();
s32 Func_02002f28();
void Func_02002f2c();
void Func_02002f44();
void Func_02002f52();
s32 Func_02002f58();
void Func_02002f60();
void Func_02002f60_a();
void Func_02002f62();
void Func_02002f6e();
void Func_02002f72();
void Func_02002f74();
void Func_02002f76();
void Func_02002f7a();
void Func_02002f80();
void Func_02002f92();
void Func_02002f92_a();
void Func_02002fa0();
void Func_02002fa2();
void Func_02002fa8();
void Func_02002fb0();
void Func_02002fc2();
void Func_02002fe6();
void Func_02003002();

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

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunActorThirteenStagingSequence(void)
{
    s32 record;
    s32 x, y;

    if (Value1(Func_02002d0c, 0x800) != 0) {
    } else {
        Func_02002d36();
        Call3(Func_02002d64, 0, 0x10000, 0x8000);
        Call1(Func_02002df2, 0xf9f);
        Func_02002e0a(13, 0);
        record = Func_02002d70(0);
        x = *(s16 *)(record + 10);
        y = *(s16 *)(record + 18);
        Func_02002dce(5, x << 16, y << 16);
        Func_02002dd8(1, x << 16, y << 16);
        Call3(Func_02002da2, 5, 0xcccc, 0x6666);
        Call3(Func_02002dac, 1, 0xcccc, 0x6666);
        Call3(Func_02002de2, 5, 0x128, 0x148);
        Call3(Func_02002df8, 1, 0x118, 0x148);
        Func_02002e18(0, 0);
        Func_02002e20(5, 0);
        Func_02002e28(1, 0);
        Call3(Func_02002e9c, 5, 0xb000, 0);
        Call3(Func_02002ea8, 1, 0xb000, 0);
        Call3(Func_02002eb4, 0, 0xb000, 0);
        Call4(Func_02002eee, 0xe80000, -1, 0xf00000, 1);
        Func_02002efa();
        Call3(Func_02002ed6, 13, 0x8000, 20);
        Func_02002e96(13, 2);
        Func_02002ece(13, 0);
        Call3(Func_02002e40, 13, 0x3333, 0x1999);
        Func_02002e7a(13, 216, 232);
        Func_02002e28_a(20);
        Func_02002ec0(13, 1);
        Func_02002e36(10);
        Func_02002f08(13, 0, 40);
        Func_02002eca(13, 2, 10);
        Func_02002f2c(13, 0, 10);
        Func_02002f26(13, 0, 10);
        Func_02002ec0_a(13, 248, 232);
        Func_02002e6e(40);
        Func_02002f06(13, 2);
        Func_02002e7c(20);
        Func_02002f44(13, 0);
        Call3(Func_02002f6e, 13, 0x101, 60);
        Func_02002f26_a(13, 1);
        Call3(Func_02002f7a, 13, 0x8000, 20);
        Func_02002f74(13, 0, 2);
        Func_02002f0e(13, 232, 232);
        Func_02002ebc(2);
        Call3(Func_02002fa0, 13, 0x4000, 4);
        Func_02002f60(13, 2);
        Func_02002ed6_a(4);
        Func_02002fa8(13, 0, 2);
        Func_02002f60_a(13, 4);
        Func_02002eee_a(20);
        Func_02002fe6(0, 1);
        Func_02003002();
        Func_02002f72(1, 2);
        record = Value1(Func_02002f28, 0);
        if (record != 0) {
            Func_02002f62(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Func_02002f80(1);
        Func_02002f92(1, 0, 0);
        Func_02002fa2(5, 2);
        record = Value1(Func_02002f58, 0);
        if (record != 0) {
            Func_02002f92_a(5, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Func_02002fb0(5);
        Func_02002fc2(5, 0, 0);
        Call1(Func_02002f52, 0x800);
        Func_02002f76();
    }
}

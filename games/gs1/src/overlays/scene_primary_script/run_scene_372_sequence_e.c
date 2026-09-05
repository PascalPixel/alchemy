#include "types.h"

#define FieldScene_RunScene372SequenceE Func_02001828

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00000e74[];
extern u8 Data_03001ebc[];
s32 Func_02005f0a();
void Func_02005f34();
void Func_02005f9e();
void Func_02005fc8();
void Func_02005fce();
void Func_02005fd6();
s32 Func_02005fec();
s32 Func_02005ff6();
void Func_0200601e();
void Func_02006036();
void Func_0200604a();
void Func_02006056();
s32 Func_0200605a();
void Func_0200605c();
void Func_02006066();
void Func_02006068();
void Func_0200606a();
void Func_0200606c();
void Func_0200606e();
void Func_02006082();
void Func_0200608e();
void Func_0200609e();
void Func_020060a0();
void Func_020060a4();
void Func_020060a8();
void Func_020060bc();
void Func_020060c6();
void Func_020060ca();
void Func_020060d0();
void Func_020060dc();
void Func_020060e8();
void Func_020060f4();
void Func_020060f8();

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
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene372SequenceE(void)
{
    u32 i;
    s32 record;
    s32 base5_e74;
    s32 v6;

    if (Value1(Func_02005f0a, 0x837) != 0) {
    } else {
        Func_02005f34();
        Call2(Func_02006056, 22, 0x100);
        base5_e74 = (s32)Data_00000e74;
        Func_0200601e(base5_e74);
        Func_02006036(22, 0);
        Call3(Func_0200606a, 0, 0x100, 20);
        Call3(Func_02006066, 0, 0x4000, 0);
        Call2(Func_0200608e, 0x6666, 0xccc);
        Call4(Func_020060a8, 0x1000000, -1, 0x24c0000, 1);
        Call3(Func_02005fce, 22, 0x20000, 0x10000);
        Value2(Func_02005ff6, 22, 0x200c934);
        Func_02006068(0, 22, 0);
        Func_02005f9e(30);
        Value2(Func_02005ff6, 22, 0x200c984);
        Func_0200609e(22, 0);
        v6 = 128;
        record = Func_02005fec(22);
        *(volatile s32 *)(record + 28) = (v6 << 9);
        Func_02006082(22, 1);
        Func_02005fc8(20);
        Func_020060d0(22, 0);
        Func_02005fd6(40);
        Func_020060a0(22, 1);
        Func_020060c6((base5_e74 + 5));
        Func_020060e8(22, 0, 20);
        Func_020060a0(0, 3);
        Func_020060a8(22, 3);
        Func_020060f8(22, 0);
        Call3(Func_0200605c, 22, (v6 << 9), 0x8000);
        Func_020060bc(22, 2);
        record = Value1(Func_0200605a, 0);
        if (record != 0) {
            Func_020060a4(22, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Func_020060ca(22);
        Func_020060dc(22, 0, 0);
        Func_0200606c(1, 1);
        Func_020060f4(21, 3);
        Call1(Func_0200604a, 0x837);
        Func_0200606e();
    }
}

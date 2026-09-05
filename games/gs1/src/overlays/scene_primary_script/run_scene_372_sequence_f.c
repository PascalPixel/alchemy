#include "types.h"

#define FieldScene_RunScene372SequenceB Func_0200173c

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00000e70[];
extern u8 Data_00000e74[];
void Func_02005e66();
s32 Func_02005e7c();
void Func_02005e90();
void Func_02005ea0();
void Func_02005ef0();
s32 Func_02005f08();
s32 Func_02005f0a();
void Func_02005f12();
void Func_02005f1a();
s32 Func_02005f20();
void Func_02005f22();
void Func_02005f2a();
void Func_02005f34();
void Func_02005f36();
void Func_02005f40();
void Func_02005f50();
void Func_02005f52();
void Func_02005f62();
void Func_02005f68();
void Func_02005f6a();
void Func_02005f78();
void Func_02005f8a();
void Func_02005f92();
void Func_02005f9a();
void Func_02005f9e();
void Func_02005fb2();
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
void Func_0200608c();
void Func_0200608e();
void Func_0200609e();
void Func_020060a0();
void Func_020060a4();
void Func_020060a8();
void Func_020060bc();
void Func_020060c0();
s32 Func_020060c2();
void Func_020060c6();
void Func_020060ca();
void Func_020060d0();
void Func_020060dc();
void Func_020060de();
void Func_020060e8();
void Func_020060ec();
void Func_020060ee();
void Func_020060f4();
void Func_020060f8();
void Func_02006130();
void Func_02006138();
s32 Func_02006146();
void Func_02006150();
void Func_0200618a();
void Func_02006190();
void Func_020061a0();
void Func_020061a6();
void Func_020061a8();
void Func_020061b6();
void Func_020061be();
void Func_020061c4();
void Func_020061c8();
void Func_020061f0();
void Func_02006200();

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

void FieldScene_RunScene372SequenceB(void)
{
    s32 record;
    s32 flag;

    Value2(Func_02005f20, 22, 0);
    Func_02005f12(0, 22, 0);
    flag = 0;
    if (Value2(Func_02005e7c, 0, 0) == 0) {
        Call1(Func_02005f36, 0xee5);
        flag = 1;
    } else {
        Call1(Func_02005f40, 0xee6);
    }
    Func_02005e66(20);
    Func_02005f68(22, 0, 40);
    Call2(Func_02005f9a, 22, 0x100);
    Func_02005f22(21, 3);
    Func_02005f2a(22, 1);
    Func_02005e90(40);
    Func_02005f62(22, 0, 0);
    Func_02005ea0(20);
    Func_02005f50(22, 3);
    if (flag != 0) {
        Func_02005f92((s32)Data_00000e70);
    } else {
        Call1(Func_02005f9a, 0xee7);
    }
    Func_02005fb2(22, 0);
    Func_02005f6a(22, 2);
    record = Value1(Func_02005f08, 0);
    if (record != 0) {
        Func_02005f52(22, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_02005f78(22);
    Func_02005f8a(22, 0, 0);
    Func_02005f1a(1, 1);
    Call1(Func_02005ef0, 0x837);
}

void Func_02001828(void)
{
    u32 i;
    s32 record;
    s32 base5_e74;

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
        record = Func_02005fec(22);
        *(s32 *)(record + 28) = 0x10000;
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
        Call3(Func_0200605c, 22, 0x10000, 0x8000);
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

void Func_0200198c(void)
{
    u32 i;
    s32 record;

    Func_0200608c();
    record = Value1(Func_020060c2, 0);
    if (record != 0) {
        Func_02006130(22, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Call3(Func_020060ee, 22, 0x10000, 0x8000);
    Call3(Func_02006138, 22, 0x119, 0x1fb);
    Func_0200618a(22, 0, 0);
    Func_020060c0(30);
    Call1(Func_020061a6, 0xe7b);
    Func_020061be(22, 0);
    Func_020061a0(0, 22, 0);
    Func_020060de(10);
    Func_020061a6(0, 1);
    Func_020060ec(20);
    Call3(Func_02006200, 22, 0x4000, 0);
    Func_020061f0(22, 0);
    Func_020061a8(22, 2);
    record = Value1(Func_02006146, 0);
    if (record != 0) {
        Func_02006190(22, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_020061b6(22);
    Func_020061c8(22, 0, 0);
    Call3(Func_020061c4, 0, 0x100, 0x205);
    Func_02006150();
}

#include "types.h"

#define FieldScene_RunScene373SequenceE Func_020012bc

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02007284();
s32 Func_020072a6();
void Func_020072c2();
void Func_020072f6();
void Func_02007308();
void Func_0200730c();
void Func_02007328();
void Func_02007334();
void Func_0200733c();
void Func_02007344();
void Func_02007358();
void Func_0200736e();
void Func_0200737e();
void Func_02007380();
void Func_02007396();
void Func_020073a4();
void Func_020073ac();
void Func_020073b8();
void Func_020073c2();
void Func_020073c4();
void Func_020073ca();
void Func_020073da();
void Func_020073e6();
s32 Func_020073f0();
void Func_020073fa();
void Func_02007406();
void Func_0200740c();
void Func_02007422();
void Func_0200742a();
void Func_0200744e();
void Func_0200745e();
void Func_0200746e();
void Func_02007482();
void Func_02007496();
void Func_020074aa();

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

void FieldScene_RunScene373SequenceE(void)
{
    u32 i;
    u8 *rec7;
    s32 record;

    rec7 = Value1(Func_020072a6, 22);
    Func_02007284();
    Call3(Func_020072c2, 0, 0x20000, 0x20000);
    Func_02007334(0, 5, 0);
    Call3(Func_02007308, 0, 215, 0x193);
    rec7[90] |= 1;
    Call3(Func_0200733c, 22, 0xa60000, 0x1770000);
    Call3(Func_020073b8, 22, 0x2000, 20);
    rec7[90] = (rec7[90] ^ 1);
    Call3(Func_0200730c, 22, 0x28000, 0x28000);
    Func_0200737e(22, 4, 0);
    Call3(Func_02007358, 22, 202, 0x18b);
    Func_02007380(0, 1);
    Func_020072f6(10);
    Call3(Func_020073fa, 0, 0xb000, 0);
    Call3(Func_02007406, 22, 0x3000, 24);
    Call3(Func_0200742a, 0, 0x100, 0);
    Func_020073ca(0, 2);
    Func_02007328(20);
    Call3(Func_0200736e, 22, 0x18000, 0x10000);
    Call2(Func_0200737e, 0, 0x200f59c);
    Func_02007344(10);
    Call3(Func_0200745e, 22, 0x103, 0);
    Call2(Func_02007396, 22, 0x200f5ec);
    Func_020073a4(0);
    Call3(Func_020073da, 0, 0x100, 0x1da);
    Call3(Func_0200746e, 0, 0xc000, 0);
    Func_020073c4(22);
    Call3(Func_020073fa, 22, 0x100, 0x1c8);
    Func_02007422(0, 1);
    Call3(Func_02007496, 22, 0x4000, 20);
    Func_0200744e(22, 2);
    Func_020073ac(20);
    Call1(Func_02007482, 0xfce);
    Func_020074aa(22, 0);
    record = Func_020073f0(22);
    *(volatile s32 *)(record + 108) = 0x200d72d;
    Call2(Func_0200740c, 22, 0x200e248);
    Call1(Func_020073c2, 0x823);
    Func_020073e6();
}

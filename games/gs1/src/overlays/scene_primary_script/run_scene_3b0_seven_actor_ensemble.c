#include "types.h"

#define FieldScene_RunSevenActorEnsemble Func_02000e78

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_0000006f[];
extern u8 Data_02000240[];
extern u8 Data_03001ebc[];
void Func_02001926();
void Func_0200192c();
void Func_02001932();
void Func_02001938();
void Func_0200193e();
void Func_02001944();
void Func_0200194a();
void Func_02002050();
void Func_02002084();
void Func_02002090();
void Func_020020ba();
s32 Func_020020be();
void Func_020020f6();
void Func_02002108();
void Func_02002112();
void Func_0200213c();
void Func_02002142();
void Func_02002148();
void Func_0200214e();
void Func_02002154();
void Func_0200215a();
void Func_0200215e();
void Func_02002160();
void Func_0200216c();
s32 Func_02002176();
void Func_0200217a();
void Func_02002188();
void Func_02002196();
void Func_020021a4();
void Func_020021b2();
void Func_020021c0();
s32 Func_020021c2();
s32 Func_020021ca();
s32 Func_020021d2();
void Func_020021d8();
s32 Func_020021da();
void Func_020021de();
s32 Func_020021e2();
s32 Func_020021ea();
void Func_020021f2();
void Func_0200222a();
void Func_02002230();
void Func_02002248();
void Func_02002256();
void Func_02002270();
void Func_02002278();
void Func_0200227a();
void Func_0200228c();
void Func_020022a0();
void Func_020022b2();
void Func_0200231c();
void Func_02002324();
void Func_02002328();
void Func_0200232c();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ void Call0(void (*f)())
{
    f();
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
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

void FieldScene_RunSevenActorEnsemble(void)
{
    u32 i;
    s32 record;

    Func_02002090();
    Func_02002108(0, 15);
    record = Func_020020be(0);
    Func_02002084(record, 0);
    Call1(Func_020020ba, 0x200976c);
    Func_02002050(1);
    Func_02001926(9);
    Func_0200192c(10);
    Func_02001932(11);
    Func_02001938(12);
    Func_0200193e(13);
    Func_02001944(14);
    Func_0200194a(15);
    Call2(Func_02002112, 8, 0x200939c);
    *(s32 *)(*(u8 **)Data_03001ebc + 0x1c0) = 0x203;
    Call0((void (*)())Func_020021ca);
    Func_020021de();
    Call1(Func_020020f6, 0x190);
    Func_0200213c(9);
    Func_02002142(10);
    Func_02002148(11);
    Func_0200214e(12);
    Func_02002154(13);
    Func_0200215a(14);
    Func_02002160(15);
    Call3(Func_0200215e, 9, 0x30000, 0x18000);
    Call3(Func_0200216c, 10, 0x30000, 0x18000);
    Call3(Func_0200217a, 11, 0x30000, 0x18000);
    Call3(Func_02002188, 12, 0x30000, 0x18000);
    Call3(Func_02002196, 13, 0x30000, 0x18000);
    Call3(Func_020021a4, 14, 0x30000, 0x18000);
    Call3(Func_020021b2, 15, 0x30000, 0x18000);
    Value2(Func_020021c2, 9, 0x2009450);
    Value2(Func_020021ca, 10, 0x2009480);
    Value2(Func_020021d2, 11, 0x20094b0);
    Value2(Func_020021da, 12, 0x20094e0);
    Value2(Func_020021e2, 13, 0x2009510);
    Value2(Func_020021ea, 14, 0x2009540);
    Call2(Func_020021f2, 15, 0x2009570);
    Func_020021c0(40);
    Func_02002230(8, 3);
    Call2(Func_0200227a, 8, 0x102);
    Func_020021d8(120);
    Func_02002248(8, 1);
    Call3(Func_0200228c, 8, 0x100, 60);
    Call3(Func_0200222a, 8, 0x10000, 0x8000);
    Call3(Func_02002256, 8, 164, 0x158);
    Func_02002270(8, 4, 10);
    Func_0200227a(8, 6, 20);
    Call1(Func_020022a0, 0x1ee4);
    Func_020022b2(8, 0, 20);
    *(s32 *)(*(u8 **)Data_03001ebc + 0x1c0) = 0x202;
    Func_0200231c();
    Func_02002328();
    {
        u8 *p;
        u16 *slot;
        s32 shown;

        p = Data_02000240;
        {
            s32 v;

            v = (s32)Data_0000006f;
            *(u16 *)(p + 0x1c4) = v;
        }
        slot = (u16 *)(p + 0x1c6);
        shown = 2;
        *slot = shown;
    }
    if (Value0(Func_02002176) == 11) {
        Func_02002324(15);
    } else {
        Func_0200232c(14);
    }
    Func_02002278();
}

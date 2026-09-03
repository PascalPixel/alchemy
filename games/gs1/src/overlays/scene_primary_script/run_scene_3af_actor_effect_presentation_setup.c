#include "types.h"

#define FieldScene_RunActorAndEffectPresentationSetup Func_02000ca0

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00001d40[];
void Func_02001c00();
void Func_020046f0();
void Func_0200470a();
void Func_02004724();
void Func_02004740();
void Func_0200474e();
void Func_0200475e();
void Func_02004764();
void Func_0200477a();
void Func_02004790();
void Func_02004796();
void Func_020047aa();
void Func_020047b8();
void Func_020047da();
void Func_0200480a();
void Func_02004822();
void Func_0200484a();
s32 Func_02004868();
void Func_02004880();
void Func_02004896();
void Func_0200489c();
void Func_020048d2();
void Func_020048fa();
void Func_0200497e();
void Func_02004a04();
s32 Func_02004f7e();
void Func_02004fa8();
void Func_02004fd4();
void Func_02005066();
void Func_0200508c();
void Func_020050a4();
void Func_020050aa();
void Func_020050ba();
void Func_020050d4();
void Func_020050de();
void Func_020050e0();
void Func_020050f8();
void Func_020050fc();
void Func_02005104();
void Func_02005106();
void Func_02005114();
void Func_0200511a();
void Func_02005142();
void Func_02005144();
void Func_02005156();
void Func_0200515c();
void Func_02005166();
void Func_02005168();
void Func_0200519c();
void Func_020051a4();
void Func_020051bc();
void Func_020051c4();
void Func_020051e4();
void Func_020051e8();
void Func_020051ec();
void Func_02005210();
void Func_02005214();
void Func_02005218();
void Func_0200521a();
void Func_0200522e();
void Func_0200523e();
void Func_02005244();
void Func_0200524e();
s32 Func_0200525e();
void Func_02005260();
void Func_0200526e();
s32 Func_02005270();
void Func_02005272();
void Func_02005284();
void Func_02005288();
void Func_02005292();
void Func_0200529c();
void Func_020052a6();
void Func_020052be();
u8 *Func_020052d2();
void Func_020052e4();
void Func_020052ee();
void Func_020052f6();
void Func_0200530c();
void Func_02005314();
void Func_0200531e();
void Func_02005322();
u8 *Func_02005336();
void Func_02005338();
void Func_0200533c();
void Func_02005340();
s32 Func_02005342();
void Func_0200534c();
void Func_02005358();
void Func_02005362();
void Func_02005376();
void Func_02005378();
void Func_02005390();
void Func_020053a0();
void Func_020053bc();
void Func_020053d4();

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

void FieldScene_RunActorAndEffectPresentationSetup(void)
{
    u32 i;
    u8 *record;

    if (Value1(Func_02004f7e, 0x911) == 0) {
    } else {
        Func_02004fa8();
        Func_02005114();
        Func_02005066(0, 20, 10);
        Call2(Func_020050de, 0x19999, 0x3333);
        Call4(Func_020050f8, 0xbe0000, -1, 0x2c40000, 1);
        Func_02005104();
        Func_02004fd4(40);
        Func_0200508c(22, 1);
        Call1(Func_020050aa, 0x1d26);
        Func_020046f0(0x4016);
        Call3(Func_020050fc, 20, 0x102, 60);
        Func_020050a4(20, 2);
        Func_0200470a(20);
        Func_020050ba(22, 1);
        Call3(Func_02005106, 22, 0x5000, 0);
        Func_02004724(0x4016);
        Func_020050d4(20, 1);
        Func_0200474e(20, 0xb000);
        Func_02004740(20);
        Func_02004764(23, 0x3000);
        Func_020050e0(23, 3);
        Func_0200475e(0x4017);
        Call3(Func_02005168, 22, 0x101, 40);
        Call3(Func_0200515c, 22, 0x8000, 20);
        Func_0200477a(0x4016);
        Func_02004796(23, 0);
        Func_0200511a(23, 4);
        Func_02004790(0x4017);
        Call3(Func_0200519c, 20, 0x100, 40);
        Func_02005144(20, 2);
        Func_020047aa(20);
        Func_02005142(22, 3);
        Func_020047b8(0x4016);
        Func_020047da(20, 0xd000);
        Func_02005156(23, 3);
        Func_02005166(20, 3);
        Func_020050d4(60);
        Call3(Func_020051e8, 22, 0x106, 40);
        Func_0200480a(22, 0x5000);
        Func_020051bc((s32)Data_00001d40);
        Func_020051a4(22, 1);
        Func_0200480a(0x4016);
        Call3(Func_02005214, 20, 0x101, 40);
        Func_020051bc(20, 2);
        Func_02004822(20);
        Call3(Func_0200522e, 22, 0x108, 20);
        Func_02005210(0x4016, 0, 20);
        Call3(Func_02005244, 23, 0x102, 60);
        Func_0200484a(0x4017);
        Value2(Func_02004868, 22, 0x8000);
        Func_020051ec(22, 3);
        Func_0200523e(0x4016, 0, 20);
        Call3(Func_02005272, 20, 0x102, 40);
        Func_0200521a(20, 2);
        Func_02004880(20);
        Func_0200489c(22, 0x5000);
        Func_02005218(22, 4);
        Func_02004896(22);
        Func_02005288(20, 0xb000, 0);
        Func_02005292(23, 0x3000, 40);
        Func_0200529c(23, 0, 0);
        Func_020052a6(20, 0xd000, 20);
        Func_0200526e(22, 2);
        Func_020051c4(20);
        Func_020048d2(0x4016);
        Call2(Func_020052e4, 23, 0x102);
        Call2(Func_020052ee, 20, 0x102);
        Func_020051e4(40);
        Func_02005284(22, 3);
        Func_020048fa(0x4016);
        Call2(Func_02005322, 0xcccc, 0x1999);
        Call4(Func_0200533c, 0xb60000, -1, 0x2f80000, 1);
        Call3(Func_0200524e, 23, 0xcccc, 0x6666);
        Value2(Func_0200525e, 23, 0x200c464);
        Call3(Func_02005260, 22, 0xcccc, 0x6666);
        Value2(Func_02005270, 22, 0x200c49c);
        Call3(Func_02005272, 20, 0xcccc, 0x6666);
        Call3(Func_020052be, 20, 182, 0x2f8);
        Func_020052f6(20, 2);
        Call3(Func_02005362, 20, 0x100, 60);
        Func_0200497e(20, 0xd000);
        Func_0200534c(20, 0, 20);
        Func_0200530c(20, 3);
        Func_0200531e(20, 4, 0);
        Func_02005378(20, 0x3000, 40);
        Call2(Func_020053bc, 0x10000, 0x2000);
        Call4(Func_020053d4, 0xd80000, -1, 0x3160000, 1);
        {
            u8 *record = Func_020052d2(20);
            u8 value = *(volatile u8 *)&record[35];
        
            record[35] = (u8)(value | 1);
        }
        Call3(Func_020052f6, 20, 0x13333, 0x9999);
        Call3(Func_02005340, 20, 182, 0x30e);
        Call3(Func_0200534c, 20, 192, 0x328);
        Call3(Func_02005358, 20, 216, 0x328);
        Func_02004a04(20, 0xd000);
        Func_020053a0(20, 2);
        Func_02001c00();
        Call3(Func_02005376, 20, 216, 0x31e);
        Func_02005390(20, 0, 0);
        record = Func_02005336(20);
        *(s32 *)(record + 24) = 0x10000;
        record = Value1(Func_02005342, 20);
        *(s32 *)(record + 28) = 0x10000;
        Call1(Func_02005314, 0x920);
        Func_02005338();
    }
}

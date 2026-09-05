#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
s32 Func_02003076();
s32 Func_02003082();
void Func_02003098();
void Func_020030b4();
void Func_020030e8();
void Func_0200311e();
s32 Func_02003122();
void Func_02003132();
void Func_02003148();
void Func_0200314a();
void Func_0200314e();
void Func_02003156();
void Func_0200315c();
void Func_02003160();
void Func_02003162();
s32 Func_02003166();
void Func_02003168();
void Func_02003170();
void Func_0200318c();
void Func_0200318e();
void Func_0200319a();
void Func_0200319c();
void Func_020031a0();
void Func_020031aa();
void Func_020031b4();
s32 Func_020031d4();
void Func_020031dc();
void Func_020031e2();
void Func_02003206();
void Func_02003256();

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
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3b4_02000b68(void)
{
    u32 i;
    s32 rec7;
    s32 rec8;
    s32 record;

    if (Value1(Func_02003076, 0x9c8) == 0) {
    } else {
        rec8 = Value1(Func_02003082, 0x9c9);
        if (rec8 != 0) {
        } else {
            Call1(Func_02003098, 0x9c9);
            Func_020030b4();
            Call2(Func_02003148, 0x20000, 0x4000);
            Func_02003160(15, 1);
            Func_0200315c();
            Call3(Func_02003148, 15, 0x4000, 20);
            Call2(Func_02003162, 15, 0x102);
            Func_0200314a(15, 2);
            Func_020030e8(20);
            Call3(Func_0200311e, 15, 0x10000, 0x8000);
            Func_020031dc(152);
            record = Func_02003122(15);
            *(volatile s32 *)(record + 40) = 0xa0000;
            Call3(Func_02003156, 15, 0x248, 0x298);
            Call3(Func_0200319a, 15, 0x4000, 20);
            Call2(Func_020031b4, 15, 0x102);
            Func_02003132(30);
            Call3(Func_02003168, 15, 0x80000, 0x4000);
            Call3(Func_0200318e, 15, 0x298, 0x298);
            Call3(Func_0200319c, 15, 0x2e8, 0x298);
            Call3(Func_020031aa, 15, 0x338, 0x298);
            Func_02003170(10);
            Func_02003256(208);
            Call3(Func_0200314e, 0x40000, 0x20000, 0x10000);
            Func_0200318c(20);
            Call3(Func_02003162, -1, -1, 0xe666);
            Func_020031a0(30);
            Call3(Func_02003206, 15, 0x3780000, 0x2980000);
            rec7 = Value1(Func_020031d4, 15);
            {
                s32 target = *(volatile s32 *)(rec7 + 80);
                s32 shown = 0xf800;

                *(volatile u16 *)(target + 30) = shown;
            }
            *(volatile u16 *)(rec7 + 6) = rec8;
            Func_02003156(rec7, 0);
            Value2(Func_02003166, rec7, 0x200a6fc);
            Func_020031e2();
        }
    }
}

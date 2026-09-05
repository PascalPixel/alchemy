#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
s32 Func_020036d2();
void Func_020036ee();
s32 Func_020036fa();
void Func_020036fc();
void Func_02003728();
s32 Func_02003748();
void Func_02003772();
s32 Func_0200377a();
s32 Func_02003782();
void Func_020037a8();
void Func_020037aa();
void Func_020037c2();
void Func_020037e4();
void Func_0200383a();
void Func_0200384e();
void Func_02003884();
void Func_020038b2();
void Func_020038cc();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

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

void FieldScene_RunScene39b_02001208(void)
{
    u32 i;
    u8 *rec7;
    s32 rec8;
    s32 record;

    rec7 = Value1(Func_020036fa, 0);
    rec8 = Value1(Func_020036d2, 0x109);
    if (rec8 == 0) {
        Func_020036fc();
        Call4(Func_020037c2, -1, -1, -1, 0);
        rec7[85] = rec8;
        Value3(Func_0200377a, 0, (*(s16 *)((s32)rec7 + 10) << 16), ((*(s16 *)((s32)rec7 + 18) << 16) + -0x100000));
        Func_020037aa(0, 15);
        record = Func_02003748(0);
        Func_020036ee(record, 0);
        Func_0200383a();
        Func_0200384e();
        Func_02003884(228);
        *(volatile s32 *)((s32)rec7 + 108) = 0x2008cc1;
        Call3(Func_02003772, 0, 0x6666, 0x3333);
        Func_02003884(0, 0, 8);
        Func_020037e4(0, 0);
        record = Func_02003782(0);
        Func_02003728(record, 1);
        {
            u8 *rec9 = (u8 *)*(volatile s32 *)((s32)rec7 + 80);
            s32 mask = -13;
            rec9[9] = (mask & rec9[9]) | 4;
        }
        Func_020038b2(0, 0, 10);
        rec7[85] = 3;
        *(volatile s32 *)((s32)rec7 + 108) = rec8;
        Func_020038cc();
        Func_020037a8();
    }
}

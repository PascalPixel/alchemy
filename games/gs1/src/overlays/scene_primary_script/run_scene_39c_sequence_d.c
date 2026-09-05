#include "types.h"

#define FieldScene_RunScene39b_02001208 Func_02003788

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
s32 Func_0200921a();
void Func_0200920e();
s32 Func_02009262();
void Func_02009254();
void Func_02009248();
s32 Func_020092b0();
void Func_020092da();
s32 Func_020092f2();
s32 Func_020092ea();
void Func_02009300();
void Func_02009342();
void Func_0200936a();
void Func_0200937c();
void Func_020093da();
void Func_020093ee();
void Func_02009454();
void Func_02009482();
void Func_0200949c();

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

    rec7 = Value1(Func_02009262, 0);
    rec8 = Value1(Func_0200921a, 0x109);
    if (rec8 == 0) {
        Func_02009254();
        Call4(Func_0200936a, -1, -1, -1, 0);
        rec7[85] = rec8;
        Value3(Func_020092f2, 0, (*(s16 *)((s32)rec7 + 10) << 16), ((*(s16 *)((s32)rec7 + 18) << 16) + -0x100000));
        Func_02009342(0, 15);
        record = Func_020092b0(0);
        Func_0200920e(record, 0);
        Func_020093da();
        Func_020093ee();
        Func_02009454(228);
        *(volatile s32 *)((s32)rec7 + 108) = 0x2008cd1;
        Call3(Func_020092da, 0, 0x6666, 0x3333);
        Func_02009454(0, 0, 8);
        Func_0200937c(0, 0);
        record = Func_020092ea(0);
        Func_02009248(record, 1);
        {
            u8 *rec9 = (u8 *)*(volatile s32 *)((s32)rec7 + 80);
            s32 mask = -13;
            rec9[9] = (mask & rec9[9]) | 4;
        }
        Func_02009482(0, 0, 10);
        rec7[85] = 3;
        *(volatile s32 *)((s32)rec7 + 108) = rec8;
        Func_0200949c();
        Func_02009300();
    }
}

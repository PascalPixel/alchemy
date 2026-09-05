#include "types.h"

#define FieldScene_RunScene39aSequenceD Func_020017e4

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00000040[];
extern u8 Data_00000043[];
extern u8 Data_02000240[];
extern s32 Data_0200b328;
extern u8 *Data_03001ee0;
s32 Func_02002718();
void Func_02002726();
void Func_02003132();
void Func_02003334();
s32 Func_02003b1a();
void Func_02003b7c();
u8 *Func_02003bac();
void Func_02003bb6();
void Func_02003bc8();
s32 Func_02003be0();
void Func_02003c02();
s32 Func_02003c0a();
void Func_02003c88();
void Func_02003c94();
void Func_02003cc0();
void Func_02003cd6();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

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

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene39aSequenceD(void)
{
    u8 *base;
    u8 *record;
    s32 val;
    s32 y;

    Func_02003132();
    base = Data_02000240;
    val = *(s16 *)(base + 0x1c0);
    if (val == (s32)Data_00000040) {
        if (Value1(Func_02003b1a, 0xf13) == 0) {
            if (*(s16 *)(base + 0x1c2) == 1) {
                Func_02003334();
            }
        }
        {
            u8 *p = Data_02000240;
            if ((u32)((*(u16 *)(p + 0x1c2) - 2) << 16) <= 0x30000) {
            y = 0x1c40000;
            Value4(Func_02002718, 0x9c0000, 0, y, 223);
            Call4(Func_02002726, 0xbc0000, 0, y, 223);
            }
        }
    } else if (val == (s32)Data_00000043) {
        record = Func_02003bac(8);
        y = 0;
        Data_0200b328 = y;
        record[85] = (u8)y;
        *(s32 *)(record + 12) = y;
        Value2(Func_02003c0a, 8, 1);
        Call2(Func_02003c02, 8, 15);
        switch (*(s16 *)(base + 0x1c2)) {
        case 1:
        case 2:
            Func_02003c88(0);
            Data_0200b328 = 1;
            break;
        case 5:
            Func_02003c94(0);
            Data_0200b328 = 1;
            y = (s32)Data_03001ee0;
            *(s32 *)(y + 24) = 0;
            break;
        }
        {
            u8 *q = Data_02000240;
            if (*(s16 *)(q + 0x1c2) <= 6) {
            if (Value1(Func_02003be0, 0x820) != 0) {
                Call6(Func_02003bb6, 30, 57, 19, 57, 1, 1);
                Call6(Func_02003bc8, 30, 8, 12, 8, 8, 7);
            } else {
                *(s32 *)((*(u8 **)0x03001ebc + 0x1c0)) = 0x100;
                Call2(Func_02003cc0, 0x203108, 1);
                Call2(Func_02003cc0, 0x203108, 1);
                Func_02003cd6(1);
                Func_02003b7c(1);
            }
            }
        }
    }
}

#include "types.h"

#define FieldScene_RunOpeningAuxiliarySequence Func_02001154

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02002538();
s32 Func_02002ce8();
s32 Func_02002cfc();
void Func_020057ea();
s32 Func_0200583c();
s32 Func_0200584c();
void Func_0200585e();
void Func_02005866();
s32 Func_020058a6();
u8 *Func_020058ae();
void Func_020058d2();
void Func_020058e0();
u8 *Func_020058ee();
void Func_020058f4();
void Func_0200590a();
s32 Func_02005930();
void Func_02005932();
s32 Func_0200593a();
s32 Func_02005946();
u8 *Func_02005960();
void Func_02005962();
void Func_02005976();
void Func_0200598e();
u8 *Func_020059a0();
void Func_020059b2();
void Func_020059c0();
void Func_02005a0a();
void Func_02005a1e();
void Func_02005a32();
void Func_02005a5c();
void Func_02005a68();
void Func_02005aba();
void Func_02005acc();

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

void FieldScene_RunOpeningAuxiliarySequence(void)
{
    u32 i;
    s32 p10;
    s32 p10b;
    s32 rec8;
    u8 *record;
    s32 r8;
    s32 v5;
    u8 *p5;
    u8 *later;

    if (Value1(Func_0200583c, 0x312) != 0) {
        v5 = r8;
    } else {
        Func_02005866();
        v5 = r8;
        if (Value1(Func_0200584c, 0x832) == 0) {
            rec8 = Value1(Func_020058a6, 13);
            record = Func_020058ae(0);
            p5 = (u8 *)(s32)*(volatile u8 *)(*(volatile s32 *)((s32)record + 80) + 9);
            record += 35;
            p10 = *record;
            Call3(Func_0200585e, 0x40000, 0x40000, 0x10000);
            Func_02005a5c(141);
            Func_020057ea(40);
            Func_02005a68(145);
            Func_020059c0(0, 3);
            {
                u8 *record = Func_020058ee(0);
                u8 value = *(volatile u8 *)&record[35];
            
                record[35] = (u8)(value | 2);
            }
            Call3(Func_02005962, 13, 0, 0x2bf0000);
            *(volatile s32 *)(rec8 + 48) = 0x18000;
            *(volatile s32 *)(rec8 + 52) = 0x18000;
            {
                s32 shifted = *(volatile s32 *)(rec8 + 12) + 0x500000;

                *(volatile s32 *)(rec8 + 12) = shifted;
                *(volatile s32 *)(rec8 + 60) = shifted;
            }
            *(volatile s32 *)(rec8 + 68) = 0x8000;
            Call3(Func_02005976, 13, 64, 0x2bf);
            Func_020058f4(40);
            Call1(Func_02005aba, 0x121);
            Call3(Func_020058d2, -1, -1, 0xe666);
            Func_020058e0();
            Func_02005acc();
            Call1(Func_0200590a, 0x832);
            Func_02005a32(0, ((u32)(s32)((s32)p5 << 28) >> 30));
            {
                u8 *record = Func_02005960(0);
                u8 value = *(volatile u8 *)&record[35];
            
                record[35] = (u8)(value | 1);
            }
            *record = p10;
        }
        Func_02002538();
        Call1(Func_02005932, 0x312);
        if (Value1(Func_02005930, 0x837) != 0) {
            if (Value1(Func_0200593a, 0x841) == 0) {
                if (Value1(Func_02005946, 0x30c) == 0) {
                    later = Func_020059a0(0);
                    if (*(volatile s32 *)((s32)later + 16) <= 0x2b4ffff) {
                        Value2(Func_02002ce8, 62, 0x29d);
                        Call3(Func_02005a0a, 0, 27, 0x273);
                    } else {
                        Value2(Func_02002cfc, 75, 0x2cb);
                        Call3(Func_02005a1e, 0, 67, 0x2f5);
                    }
                    Call1(Func_0200598e, 0x30c);
                }
            }
        }
        Func_020059b2();
    }
    p10b = v5;
}

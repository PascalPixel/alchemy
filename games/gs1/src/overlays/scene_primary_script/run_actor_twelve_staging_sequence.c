#include "types.h"

#define FieldScene_RunActorTwelveStagingSequence Func_02000f38

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_020020a8();
s32 Func_02002aa6();
s32 Func_02002abc();
void Func_020055b2();
s32 Func_0200561a();
void Func_02005626();
s32 Func_0200562a();
void Func_02005644();
s32 Func_02005684();
void Func_020056ae();
void Func_020056ba();
void Func_020056d2();
void Func_020056e4();
s32 Func_020056ec();
void Func_020056ee();
s32 Func_020056f6();
s32 Func_02005702();
void Func_02005714();
void Func_0200572a();
void Func_02005740();
void Func_0200574e();
void Func_02005754();
s32 Func_0200575c();
void Func_02005772();
void Func_020057ba();
void Func_020057c8();
void Func_020057d0();
void Func_020057de();
void Func_02005824();
void Func_02005830();
void Func_02005898();
void Func_020058a6();

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

void FieldScene_RunActorTwelveStagingSequence(void)
{
    s32 rec7;
    s32 record;

    if (Value1(Func_0200561a, 0x311) != 0) {
    } else {
        Func_02005644();
        if (Value1(Func_0200562a, 0x831) == 0) {
            rec7 = Func_02005684(12);
            Call3(Func_02005626, 0x40000, 0x40000, 0x10000);
            Func_02005824(141);
            Func_020055b2(40);
            Func_02005830(145);
            Call3(Func_02005714, 12, 0x17d0000, 0x3280000);
            *(volatile s32 *)(rec7 + 48) = 0x18000;
            *(volatile s32 *)(rec7 + 52) = 0x18000;
            {
                s32 shifted = *(volatile s32 *)(rec7 + 12) + 0x1000000;

                *(volatile s32 *)(rec7 + 12) = shifted;
                *(volatile s32 *)(rec7 + 60) = shifted;
            }
            *(volatile s32 *)(rec7 + 68) = 0x8000;
            Call3(Func_0200572a, 12, 0x122, 0x341);
            Func_020057ba(12, 1);
            Call3(Func_02005740, 12, 0x102, 0x354);
            Func_020057d0(12, 2);
            Call3(Func_02005754, 12, 224, 0x368);
            Func_020056d2(40);
            Call1(Func_02005898, 0x121);
            Call3(Func_020056ae, -1, -1, 0xe666);
            Func_020056ba();
            Func_020058a6();
            Call1(Func_020056e4, 0x831);
        }
        Func_020020a8();
        Call1(Func_020056ee, 0x311);
        if (Value1(Func_020056ec, 0x837) != 0) {
            if (Value1(Func_020056f6, 0x841) == 0) {
                if (Value1(Func_02005702, 0x30c) == 0) {
                    record = Func_0200575c(0);
                    if (*(volatile s32 *)(record + 12) > 0x800000) {
                        Value2(Func_02002aa6, 219, 0x34b);
                        Call3(Func_020057c8, 0, 179, 0x33d);
                    } else {
                        Value2(Func_02002abc, 214, 0x38c);
                        Call3(Func_020057de, 0, 219, 0x38f);
                    }
                    Call1(Func_0200574e, 0x30c);
                }
            }
        }
        Func_02005772();
    }
}

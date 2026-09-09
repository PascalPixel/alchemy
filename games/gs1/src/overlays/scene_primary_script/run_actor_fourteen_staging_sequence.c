#include "types.h"

#define FieldScene_RunActorFourteenStagingSequence Func_02001348

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02002882();
void Func_02002e8e();
void Func_02002ea4();
void Func_020059c2();
s32 Func_02005a2a();
void Func_02005a36();
s32 Func_02005a3a();
void Func_02005a54();
s32 Func_02005a94();
void Func_02005a94_a();
void Func_02005aa0();
void Func_02005ab8();
void Func_02005aca();
s32 Func_02005ad2();
void Func_02005ad4();
s32 Func_02005adc();
s32 Func_02005ae8();
void Func_02005b24();
void Func_02005b36();
void Func_02005b3a();
s32 Func_02005b42();
void Func_02005b5a();
void Func_02005bb2();
void Func_02005bc6();
void Func_02005c34();
void Func_02005c40();
void Func_02005c7e();
void Func_02005c8c();

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

void FieldScene_RunActorFourteenStagingSequence(void)
{
    s32 rec7;
    s32 record;

    if (Value1(Func_02005a2a, 0x313) != 0) {
    } else {
        Func_02005a54();
        if (Value1(Func_02005a3a, 0x833) == 0) {
            rec7 = Func_02005a94(14);
            Call3(Func_02005a36, 0x40000, 0x40000, 0x10000);
            Func_02005c34(141);
            Func_020059c2(40);
            Func_02005c40(145);
            Call3(Func_02005b24, 14, 0x1da0000, 0x47b0000);
            *(volatile s32 *)(rec7 + 48) = 0x10000;
            *(volatile s32 *)(rec7 + 52) = 0x10000;
            {
                s32 shifted = *(volatile s32 *)(rec7 + 12) + 0x480000;

                *(volatile s32 *)(rec7 + 12) = shifted;
                *(volatile s32 *)(rec7 + 60) = shifted;
            }
            *(volatile s32 *)(rec7 + 68) = 0x8000;
            Call3(Func_02005b3a, 14, 0x1b0, 0x47b);
            Func_02005ab8(40);
            Call1(Func_02005c7e, 0x121);
            Call3(Func_02005a94_a, -1, -1, 0xe666);
            Func_02005aa0();
            Func_02005c8c();
            Call1(Func_02005aca, 0x833);
        }
        Func_02002882();
        Call1(Func_02005ad4, 0x313);
        if (Value1(Func_02005ad2, 0x837) != 0) {
            if (Value1(Func_02005adc, 0x841) == 0) {
                if (Value1(Func_02005ae8, 0x30c) == 0) {
                    record = Func_02005b42(0);
                    if (*(volatile s32 *)(record + 16) <= 0x479ffff) {
                        Call2(Func_02002e8e, 0x19c, 0x460);
                        Call3(Func_02005bb2, 0, 0x19e, 0x42c);
                    } else {
                        Call2(Func_02002ea4, 0x1bd, 0x494);
                        Call3(Func_02005bc6, 0, 0x1bf, 0x4cb);
                    }
                    Call1(Func_02005b36, 0x30c);
                }
            }
        }
        Func_02005b5a();
    }
}

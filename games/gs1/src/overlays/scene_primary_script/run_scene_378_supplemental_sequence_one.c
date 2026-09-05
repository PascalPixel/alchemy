#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02001d28();
void Func_02003954();
void Func_02003974();
void Func_0200397c();
s32 Func_02003982();
void Func_02003998();
s32 Func_020039ac();
void Func_020039b4();
s32 Func_020039c8();
u8 *Func_020039e2();
void Func_020039ee();
u8 *Func_020039f2();
void Func_02003a12();
s32 Func_02003a20();
s32 Func_02003a22();
void Func_02003a28();
void Func_02003a36();
u8 *Func_02003a40();
s32 Func_02003a4c();
void Func_02003a50();
s32 Func_02003a68();
void Func_02003a6c();
void Func_02003a7e();
void Func_02003aa0();
void Func_02003aa2();
void Func_02003aa8();
void Func_02003aaa();
void Func_02003ab0();
void Func_02003aba();
void Func_02003ac4();
void Func_02003ad0();
void Func_02003ad4();
void Func_02003ade();
void Func_02003af4();
void Func_02003af8();
s32 Func_02003b18();
void Func_02003b3e();
void Func_02003b56();
void Func_02003b62();
void Func_02003ba8();
s32 Func_02003bb8();
void Func_02003bbc();
void Func_02003bca();
void Func_02003bd4();
void Func_02003bd8();
void Func_02003bf6();
void Func_02003c02();
void Func_02003c12();
void Func_02003c1a();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call11(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8, s32 a9, s32 a10)
{
    f(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(void)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + 1);
}

void FieldScene_RunSupplementalSequenceOne(void)
{
    u32 i;
    u8 *record;

    Func_02003954();
    Call1(Func_02003a12, 0x1164);
    Value2(Func_02003a22, 8, 0);
    if (Value2(Func_02003982, 0, 0) == 0) {
        Func_020039ee(8, 3);
        Func_02003974(20);
    } else {
        Func_0200397c(20);
        Value2(Func_02003a4c, 8, 0);
        if (Value2(Func_020039ac, 0, 0) != 0) {
        } else {
            Func_02003998(20);
            Value2(Func_02003a68, 8, 0);
            if (Value2(Func_020039c8, 0, 0) != 0) {
            } else {
                Func_020039b4(20);
                record = Func_020039e2(8);
                {
                    u32 shown = (u32)*(u16 *)((s32)record + 6);
                    u32 low = 0xa000;

                    if (shown < low) {
                        goto L_02000488;
                    }
                }
                record = Func_020039f2(8);
                {
                    u32 shown = (u32)*(u16 *)((s32)record + 6);
                    u32 high = 0xe000;

                    if (shown > high) {
                        goto L_02000488;
                    }
                }
                Call3(Func_02003a12, 8, 0x8000, 0x4000);
                Func_02003ac4(8, 0, 0);
                Call1((void (*)())Func_020039f2, 10);
                *(u8 *)(Func_02003a20(8) + 90) &= 254;
                Func_02003a6c(8, 152, 120);
                Func_02003a12(1);
                {
                    u8 *record = Func_02003a40(8);
                    u8 value = *(volatile u8 *)&record[90];
                
                    record[90] = (u8)(value | 1);
                }
                Func_02003a28(20);
                Func_02003ab0(8, 3);
                Func_02003a36(20);
                Func_02003aa0(0, 168, 120);
                Func_02003aa2(0, 192, 168);
                Func_02003a50(20);
                Func_02003aba(8, 168, 120);
                Call3(Func_02003b3e, 8, 0x3000, 0);
                Func_02003ad4(0);
                goto L_020004aa;
                L_02000488:
                Func_02003ad0(0, 192, 168);
                Func_02003a7e(20);
                Call3(Func_02003b62, 8, 0x3000, 0);
                Func_02003af8(0);
                L_020004aa:
                Func_02001d28();
                Func_02003bbc(0, 0);
                Func_02003bca(120);
                Func_02003aa8(120);
                Func_02003bf6(86);
                Func_02003c02();
                Call1(Func_02003aaa, 0x9f0);
                Func_02003bd8(30);
            }
        }
    }
    Func_02003ad4();
}

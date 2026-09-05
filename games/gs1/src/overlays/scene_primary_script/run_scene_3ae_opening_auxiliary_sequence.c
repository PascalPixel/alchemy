#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_02000240[];
extern u8 Data_03001ebc[];
s32 Func_02001a82();
void Func_02001aac();
s32 Func_02001ab4();
void Func_02001abc();
void Func_02001ad2();
void Func_02001aec();
void Func_02001af2();
s32 Func_02001af4();
void Func_02001b10();
void Func_02001b44();
void Func_02001b54();
void Func_02001b58();
s32 Func_02001b5e();
void Func_02001b64();
void Func_02001b6c();
s32 Func_02001b74();
void Func_02001b76();
void Func_02001b9a();
void Func_02001baa();
s32 Func_02001bac();
void Func_02001bae();
s32 Func_02001bc2();
void Func_02001bc6();
s32 Func_02001be0();
void Func_02001bea();
void Func_02001c14();
void Func_02001c42();
void Func_02001c70();
void Func_02001cc2();

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

void FieldScene_RunOpeningAuxiliarySequence(void)
{
    u32 i;
    s32 rec7;
    s32 record;
    s32 base6_2000240;
    s32 six00;

    six00 = 0x258;
    Func_02001aac();
    if (Value1(Func_02001a82, 0x8a5) != 0) {
        Call1(Func_02001b54, 0x1d0b);
        Func_02001b6c(8, 0);
    } else {
        Call1(Func_02001b64, 0x1d04);
        Value2(Func_02001b74, 8, 0);
        if (Value2(Func_02001af4, 0, 0) == 1) {
            Func_02001b9a(8, 0, 10);
        } else {
            bump_step(1);
            Func_02001abc(six00, 5);
            Value2(Func_02001bac, 8, 0);
            rec7 = Func_02001ab4(19, 8, 11, 4, 2);
            Call4(Func_02001ad2, 0xc8a, rec7, 0, 0);
            base6_2000240 = (s32)Data_02000240;
            Func_02001aec(*(volatile s32 *)(base6_2000240 + 16), 6, rec7, 24, 8);
            if (Value2(Func_02001b5e, -1, 0) == 1) {
                Func_02001af2(rec7, 2);
                Func_02001bc2(0, 4);
                Func_02001b58(10);
                Func_02001c42(8, 0);
                goto L_02000660;
            } else {
                if ((u32)six00 <= (u32)*(volatile s32 *)(base6_2000240 + 16)) {
                    goto L_0200061e;
                }
                Func_02001b10(rec7, 2);
                Func_02001be0(0, 3);
                Func_02001b76(10);
                bump_step(1);
                Func_02001cc2(113);
                Func_02001c42(8, 0);
                goto L_02000660;
            }
            L_0200061e:;
            Func_02001b44(rec7, 2);
            Func_02001c14(0, 3);
            Func_02001baa(10);
            bump_step(3);
            Func_02001c70(8, 0);
            Func_02001be0(235, 0);
            Call1(Func_02001bae, 0x8a5);
            Call1(Func_02001bc6, -six00);
        }
        L_02000660:;
        Func_02001bea();
    }
}

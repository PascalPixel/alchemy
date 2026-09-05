#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_02006f40();
void Func_02006f48();
s32 Func_02006f58();
void Func_02006f90();
s32 Func_02006fb2();
void Func_02006fb6();
s32 Func_02006fbc();
s32 Func_02006fe4();
s32 Func_02006ff2();
s32 Func_02006ffa();
void Func_0200700c();
void Func_020071a6();

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

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

void FieldScene_RunScene39c_020014cc(void)
{
    s32 rec7;
    s32 record;
    s32 record2;
    s16 v10;
    s16 v18;

    if (Value1(Func_02006f58, 0x256) == 0) {
        record = Value1(Func_02006fb2, 0);
        v10 = *(s16 *)(record + 10);
        record2 = Value1(Func_02006fbc, 0);
        v18 = *(s16 *)(record2 + 18);
        if ((u32)(v10 - 84) <= 7) {
            if (v18 > 211) {
                if (v18 <= 219) {
                    Func_02006fb2(record2);
                    Call1(Func_02006f90, 0x256);
                    Func_02006fb6(5);
                    record = Func_02006fe4(0);
                    *(volatile s32 *)(record + 12) += -0x20000;
                    rec7 = Value1(Func_02006ff2, 0);
                    record = Value1(Func_02006ffa, 0);
                    *(volatile s32 *)(rec7 + 60) = *(volatile s32 *)(record + 12);
                    Call6(Func_02006f40, 5, 2, 5, 11, 1, 1);
                    Func_020071a6(217);
                    Call3(Func_02006f48, 0x200e010, 9, 7);
                    Func_0200700c();
                }
            }
        }
    }
}

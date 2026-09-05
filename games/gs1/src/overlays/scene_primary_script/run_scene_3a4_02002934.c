#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern s16 Data_02000240[];
s32 Func_0200648a();
void Func_02006492();
void Func_020064a6();
s32 Func_020064ea();
s32 Func_020064f4();
s32 Func_02006508();
s32 Func_02006510();
s32 Func_02006514();
void Func_0200651a();
void Func_0200652c();
void Func_02006532();
s32 Func_0200653c();
void Func_020059f0();
void Func_020065c2();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

void FieldScene_RunScene3a4_02002934(void)
{
    s32 rec7;
    s32 record;
    s16 flag;

    rec7 = Value1(Func_0200648a, 0x909);
    if (rec7 != 0) {
        Func_02006532(8, 0, 0);
        ((void (*)())Func_0200653c)(9, 0, 0);
    } else {
        record = Func_020064f4(8);
        Func_02006492(record, 0);
        Func_020065c2(9, 3);
        record = Func_02006508(9);
        Func_020064a6(record, 0);
        *(u8 *)(Func_02006514(9) + 89) = rec7;
    }
    flag = Data_02000240[225];
    if (flag == 1 || flag == 98) {
        if (Value1(Func_020064ea, 0x109) == 0) {
            rec7 = Value1(Func_0200653c, 0);
            Func_0200651a();
            *(volatile s32 *)(rec7 + 12) = 0x100000;
            Func_0200652c();
        }
    } else if (flag == 99) {
        if (Value1(Func_02006510, 0x109) == 0) {
            Func_020059f0();
        }
    }
    /* unlifted: 0x020029bc..0x020029c2 (2) */
}

#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02001e34();
s32 Func_02006542();
void Func_0200655a();
void Func_02006564();
void Func_02006596();
void Func_02006702();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

void FieldScene_RunScene3bf_02000f30(void)
{
    u8 *p5;
    s16 field;

    p5 = *(volatile s32 *)Data_03001ebc;
    if (Func_02006542(234) != -1) {
        field = *(s16 *)((s32)p5 + 0x16c);
        Func_02001e34(field - 40);
        Func_02006702(157);
        Call3(Func_0200655a, 0x30000, 0x30000, 0x10000);
        Call3(Func_02006564, -1, -1, 0xe666);
        Func_02006596(field + 0x332);
    }
}

#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02002796();
void Func_0200279c();
s32 Func_020027a4();
void Func_020027e6();
void Func_020027e8();
void Func_0200280a();
void Func_02002810();
void Func_0200282a();
void Func_0200282c();
void Func_0200284c();
void Func_02002894();
void Func_02002896();
void Func_0200289c();
void Func_0200291a();
void Func_02002960();

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

void FieldScene_RunSceneEntryBranchSequence(void)
{
    s32 v5;
    u8 *p6;

    p6 = *(volatile s32 *)Data_03001ebc;
    v5 = 0;
    if (*(s16 *)(((s32)p6 + 0x16c)) == 9) {
        if (Value1(Func_020027a4, 0x200) == 0) {
            Func_0200291a(188);
            v5 = 1;
        }
    } else {
        Func_0200291a(158);
        v5 = 1;
    }
    if (v5 != 0) {
        Func_02002796(1);
        Func_0200279c(2);
    }
    Func_020027e8();
    Func_020027e6(10);
    Call3(Func_0200282c, 0, 0x8000, 0x4000);
    Func_02002894(0, 2);
    if (*(s16 *)(((s32)p6 + 0x16c)) == 9) {
        Call3(Func_02002896, 0, 0, -16);
    } else {
        Call3(Func_0200289c, 0, 3, -16);
    }
    Func_0200282a(16);
    Func_02002960(*(s16 *)(((s32)p6 + 0x16c)));
    Func_0200284c();
    Func_0200280a(1);
    Func_02002810(2);
}

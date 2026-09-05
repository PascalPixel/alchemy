#include "types.h"

#define FieldScene_RunOpeningAuxiliarySequence Func_020015e0

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_0200702a();
void Func_02007040();
void Func_0200705a();
void Func_0200706e();
void Func_020070ba();
void Func_020070c8();
void Func_02007100();
void Func_02007114();
void Func_0200712a();
void Func_0200713e();
void Func_02007176();
void Func_02007184();
void Func_02007192();
void Func_020071be();
void Func_020071fa();
void Func_02007262();
void Func_0200733a();
void Func_020073c6();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

void FieldScene_RunOpeningAuxiliarySequence(s32 a0, s32 a1, s32 a2)
{
    u32 i;
    s32 v;

    if (a0 != 0) {
        Func_02007262(219);
    }
    i = (u32)a1;
    if (i < (u32)a2) {
        do {
            Call6(Func_0200702a, 45 - (s32)(i << 1), 32, 44 - (s32)(i << 1), 32, (s32)(i + 1), 6);
            Call6(Func_02007040, 45 - (s32)i, 51, 45 - (s32)i, 32, 1, 6);
            v = 109 - (s32)i;
            Call6(Func_0200705a, v, 32, 108 - (s32)i, 32, 1, 4);
            Call6(Func_0200706e, v, 51, v, 32, 1, 4);
            if (a0 != 0) {
                Call3(Func_020070ba, 0x50000, 0x50000, 0x10000);
                Call3(Func_020070c8, -1, -1, 0xe666);
                Func_0200713e(a0);
            }
            i = i + 1;
        } while (i < (u32)a2);
    }
    Call6(Func_020070c8, 42, 52, 4, 5, 42, 33);
}


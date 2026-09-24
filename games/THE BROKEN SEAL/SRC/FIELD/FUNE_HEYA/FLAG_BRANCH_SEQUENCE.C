#include "TYPES.H"

#define FuneHeya_RunFlagBranchSequence Func_02001978

extern u8 Data_0000006f[];
extern u8 Data_02000240[];
s32 Func_02007e38();
void Func_02007e42();
void Func_02007e44();
s32 Func_02007e54();
s32 Func_02007e70();
void Func_02007eb8();
void Func_02007fc8();
void Func_02007fcc();
void Func_02007fe8();
void Func_02008004();

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
void Func_02007fcc_a();
void Func_02007fcc_b();
void Func_02007fe8_a();
void Func_02007fe8_b();
void Func_02008004_a();
void Func_02008004_b();

void FuneHeya_RunFlagBranchSequence(void)
{
    s32 base3_2000240;

    Func_02007e44();
    Func_02007fc8();
    base3_2000240 = (s32)Data_02000240;
    *(u8 *)((base3_2000240 + 0x22b)) = 3;
    /* FAKEMATCH: the do/while orders the flag store before this call, and
     * 0x6f comes from the literal pool through a link symbol. */
    do {
        Call1(Func_02007e42, 0x8f0);
    } while (0);
    if (Value1(Func_02007e38, 0x928) == 0) {
        Func_02007fcc_a((s32)Data_0000006f, 16);
        Func_02007fcc_b(62, 0);
    } else {
        if (Value1(Func_02007e54, 0x929) == 0) {
            Func_02007fe8_a((s32)Data_0000006f, 18);
            Func_02007fe8_b(62, 1);
        } else {
            if (Value1(Func_02007e70, 0x92a) == 0) {
                Func_02008004_a((s32)Data_0000006f, 20);
                Func_02008004_b(62, 2);
            }
        }
    }
    Func_02007eb8();
}

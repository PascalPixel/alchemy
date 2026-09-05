#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_02000ef4();
void Func_02002d8c();
void Func_02002da2();
void Func_02002dde();
void Func_02002df4();
void Func_02002ec8();
void Func_02002ede();
void Func_02002f60();
void Func_02002fd4();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define Audio_PlayCue_1(a0) Call1(Func_02002fd4, a0)
#define ObjectMotion_SetPositionAndReset_1(a0, a1, a2) Call3(Func_02002ec8, a0, a1, a2)
#define ObjectMotion_SetActionVariant_1(a0, a1) Call2(Func_02002f60, a0, a1)
#define ObjectMotion_SetPositionAndReset_2(a0, a1, a2) Call3(Func_02002ede, a0, a1, a2)

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* Sets step 188, then runs a pair of 6-argument setup calls for indices 0
 * and 2 sharing the same trailing four values, followed by a pair of
 * 3-argument calls sharing the same leading two arguments, and a closing
 * 1-argument call. */
void FieldScene_RunSupplementalSequenceTwo(void)
{
    Audio_PlayCue_1(188);
    Call6(Func_02002dde, 0, 63, 51, 8, 2, 2);
    Call1(Func_02002d8c, 10);
    Call6(Func_02002df4, 2, 63, 51, 8, 2, 2);
    Call1(Func_02002da2, 10);
    ObjectMotion_SetPositionAndReset_1(0, 352, 306);
    ObjectMotion_SetActionVariant_1(0, 3);
    ObjectMotion_SetPositionAndReset_2(0, 352, 296);
    Call1(Func_02000ef4, 2);
}

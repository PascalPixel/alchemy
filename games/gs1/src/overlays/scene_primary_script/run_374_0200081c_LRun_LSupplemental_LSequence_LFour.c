#include "types.h"

#define FieldScene_RunSupplementalSequenceFour Func_0200081c

extern u8 Value_0200beb4;
void Func_02000f52();
void Func_02002e68();
void Func_02002f34();
void Func_0200306e();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define Audio_PlayCue_1(a0) Call1(Func_0200306e, a0)
#define ObjectMotion_ResetAndSetPositionInMode2_1(a0, a1, a2) Call3(Func_02002f34, a0, a1, a2)

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block. */

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

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

/* Runs four scene calls in sequence: a single-argument call, a call that
 * passes the address of Value_0200beb4 with two more values, a call that
 * passes 0, 374, and 0x1a3, and a final single-argument call. */
void FieldScene_RunSupplementalSequenceFour(void)
{
    Audio_PlayCue_1(158);
    Call3(Func_02002e68, (s32)&Value_0200beb4, 52, 18); /* main:08009178 */
    ObjectMotion_ResetAndSetPositionInMode2_1(0, 374, 0x1a3); /* object_id 0, x 374, z 0x1a3 */
    Call1(Func_02000f52, 4);
}

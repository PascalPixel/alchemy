#include "types.h"

#define FieldScene_RunPrimarySequenceSecond Func_02000fbc

extern u8 Value_0200f55a;
void Func_02006ef8();
void Func_02006ff2();
void Func_020070d8();
void Func_0200710e();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define Audio_PlayCue_1(a0) Call1(Func_0200710e, a0)
#define ObjectMotion_ResetAndSetPositionInMode2_1(a0, a1, a2) Call3(Func_02006ff2, a0, a1, a2)

/* Runs four fixed steps in order: a single-argument call, a call that takes
 * the address of the byte at Value_0200f55a plus two constants, another
 * three-constant call, and a final single-argument call. */
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

void FieldScene_RunPrimarySequenceSecond(void)
{
    Audio_PlayCue_1(158);
    Call3(Func_02006ef8, (s32)&Value_0200f55a, 35, 74); /* main:08009178 */
    ObjectMotion_ResetAndSetPositionInMode2_1(0, 102, 0x4b6);
    Call1(Func_020070d8, 10); /* main:0808a248 */
}

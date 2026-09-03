#include "types.h"

#define FieldScene_RunSupplementalSequenceSeven Func_020008ac

extern u8 Value_0200beb4;
void Func_02000fe2();
void Func_02002ef8();
void Func_02002fc4();
void Func_020030fe();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define Audio_PlayCue_1(a0) Call1(Func_020030fe, a0)
#define ObjectMotion_ResetAndSetPositionInMode2_1(a0, a1, a2) Call3(Func_02002fc4, a0, a1, a2)
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


/* Runs four scripted scene calls in sequence, passing a byte's address and a
 * handful of small immediate constants to each. */
void Func_020008ac(void)
{
    Audio_PlayCue_1(158);
    Call3(Func_02002ef8, (s32)&Value_0200beb4, 51, 39); /* main:08009178 */
    ObjectMotion_ResetAndSetPositionInMode2_1(0, 358, 0x29e);
    Call1(Func_02000fe2, 7);
}

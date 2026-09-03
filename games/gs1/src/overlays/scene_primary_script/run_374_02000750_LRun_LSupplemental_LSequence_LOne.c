#include "types.h"

#define FieldScene_RunSupplementalSequenceOne Func_02000750

extern u8 Value_0200beb4;
void Func_02000e84();
void Func_02002d9c();
void Func_02002e66();
void Func_02002fa2();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define Audio_PlayCue_1(a0) Call1(Func_02002fa2, a0)
#define ObjectMotion_ResetAndSetPositionInMode2_1(a0, a1, a2) Call3(Func_02002e66, a0, a1, a2)
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


/* Runs four fixed scene-helper calls in sequence, one of them passed the
 * address of Value_0200beb4 as its first argument. */
void Func_02000750(void)
{
    Audio_PlayCue_1(158);
    Call3(Func_02002d9c, (s32)&Value_0200beb4, 44, 7);
    ObjectMotion_ResetAndSetPositionInMode2_1(0, 248, 0x117);
    Call1(Func_02000e84, 1);
}

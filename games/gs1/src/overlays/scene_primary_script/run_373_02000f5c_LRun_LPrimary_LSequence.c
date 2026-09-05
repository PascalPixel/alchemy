#include "types.h"

extern u8 Value_0200f570;
void Func_02006e98();
void Func_02006f94();
void Func_0200707a();
void Func_020070ae();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define Audio_PlayCue_1(a0) Call1(Func_020070ae, a0)
#define ObjectMotion_ResetAndSetPositionInMode2_1(a0, a1, a2) Call3(Func_02006f94, a0, a1, a2)

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers, matching the reference codegen. */

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

/* Runs four scene primitives in sequence: one single-argument call, one call
 * passing the address of Value_0200f570 plus two small constants, one call
 * with a byte-flag-sized first argument (0) and two larger constants, and a
 * final single-argument call. */
void FieldScene_RunPrimarySequence(void)
{
    Audio_PlayCue_1(158);
    Call3(Func_02006e98, (s32)&Value_0200f570, 49, 69); /* main:08009178 */
    ObjectMotion_ResetAndSetPositionInMode2_1(0, 326, 0x466); /* object_id 0, x 326, z 0x466 */
    Call1(Func_0200707a, 8); /* main:0808a248 */
}

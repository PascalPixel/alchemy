#include "types.h"

/* Runs a short sequence of external calls with fixed numeric arguments.
 * If a gate check on Func_02001d26 succeeds, four extra paired calls run
 * first; the routine then always runs eight more paired calls before a
 * final pair closes it out. */

void Func_02001d14();
void Func_02001d1e();
s32 Func_02001d26();
void Func_02001d2a();
void Func_02001d34();
void Func_02001d42();
void Func_02001d4c();
void Func_02001d58();
void Func_02001d62();
void Func_02001d84();
void Func_02001d9a();
void Func_02001da2();
void Func_02001dac();
void Func_02001dc6();
void Func_02001dd8();
void Func_02001dde();
void Func_02001de8();
void Func_02001e12();
void Func_02001ed6();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define GameFlag_IsSet_1(a0) Value1(Func_02001d26, a0)
#define ObjectMotion_SetActionVariant_1(a0, a1) Call2(Func_02001ed6, a0, a1)

/* Loader-relocated ROM calls: each site names the pre-relocation call word the image holds. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value6(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    return f(a0, a1, a2, a3, a4, a5);
}

void FieldScene_RunOpeningAuxiliarySequence(void)
{
    /* Gate check on Func_02001d26; only proceed with the optional block
     * below when it reports success (0x202). */
    if (GameFlag_IsSet_1(0x202) != 0) {
        /* Two paired setup/confirm calls, run twice with the last
         * argument stepped from 57 to 58. */
        Value6(Func_02001d1e, 41, 86, 2, 6, 21, 57); /* main:080091b8 */
        Value1(Func_02001d14, 4); /* main:080000c0 */
        Value6(Func_02001d34, 43, 86, 2, 6, 21, 57); /* main:080091b8 */
        Value1(Func_02001d2a, 4); /* main:080000c0 */
        Value6(Func_02001d4c, 41, 86, 2, 6, 21, 58); /* main:080091b8 */
        Value1(Func_02001d42, 4); /* main:080000c0 */
        Value6(Func_02001d62, 43, 86, 2, 6, 21, 58); /* main:080091b8 */
        Value1(Func_02001d58, 4); /* main:080000c0 */
    }
    /* Unconditional call block: two setup calls, a paired setup/confirm
     * pair, then a repeat of the same shape with different constants. */
    Call6(Func_02001d84, 2, 93, 1, 1, 24, 62); /* main:080091b8 */
    Call6(Func_02001d9a, 2, 94, 1, 1, 21, 55); /* main:080091b8 */
    Value6(Func_02001dac, 41, 86, 2, 6, 21, 59); /* main:080091b8 */
    Value1(Func_02001da2, 4); /* main:080000c0 */
    Call6(Func_02001dc6, 1, 93, 1, 1, 24, 62); /* main:080091b8 */
    Call6(Func_02001dd8, 3, 94, 1, 1, 21, 55); /* main:080091b8 */
    Value6(Func_02001de8, 43, 86, 2, 6, 21, 59); /* main:080091b8 */
    Value1(Func_02001dde, 4); /* main:080000c0 */
    /* Final pair closing out the sequence. */
    ObjectMotion_SetActionVariant_1(10, 3);
    Call6(Func_02001e12, 19, 17, 1, 1, 22, 15); /* main:080091c0 */
}

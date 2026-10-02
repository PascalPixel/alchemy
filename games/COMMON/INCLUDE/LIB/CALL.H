/* Calls spelled through an inline function-pointer parameter.
 *
 * GCC 2.96 hoists a costly constant argument of a direct call into
 * a pseudo register that later uses in the block share; called through one of
 * these, each call loads its constants straight into the argument registers
 * and a value-returning call sets r0 last. Every call still spelled this way
 * was measured to differ as a direct call.
 *
 * Bounded TBS probe, 2026-10-02: 24 compilations with the approved standard
 * flags, three scenes in English and Japanese, four variants each. The
 * unchanged baselines reproduced their current build objects. Replacing the
 * selected wrappers with direct calls, direct calls casting each argument to
 * s32, or one block-local s32 per argument all failed the complete extent.
 * Function sizes (baseline / direct / casts / locals), identical in EN/JA:
 *   KorimaMura_RunObjectSpreadScene:                  716/712/712/712
 *   FieldScene_RunMainCutsceneSequence:          7468/7484/7484/7484
 *   FieldScene_RunBranchingFormationPresentation:6700/6708/6708/6716
 * Village: all 15 Call2/3/4/6 statements. Palace: all 469 Actor_SetSpeed,
 * Event_ShowMessage and Event_Wait calls. Bridge: all 13 Actor_SetMotionSpeed
 * and Actor_OffsetDestination calls. Other steering stayed unchanged.
 * Direct and casts produced identical normalized assembly in each scene;
 * locals equalled direct only in the palace. Village locals changed stack
 * argument preparation despite retaining the same size. Direct calls shared
 * constants across intervening calls in all three scenes; the palace saved
 * three more registers (12 more stack bytes). Typed palace/bridge wrappers
 * showed the effect too, so the unprototyped pointer alone cannot explain it.
 * This supports an inline-boundary/register-lifetime effect, not a missing
 * integer conversion. It does not identify a compiler pass or establish a
 * whole-game compiler change. No candidate passed; stop without adoption. */
#ifndef ALCHEMY_CALL_H
#define ALCHEMY_CALL_H

#include "TYPES.H"

static __inline__ void Call1(void (*f)(), s32 a0)
{
    /* FAKEMATCH: forwarding through the pointer prevents GCC 2.96 from sharing direct-call constants. */
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    /* FAKEMATCH: forwarding through the pointer prevents GCC 2.96 from sharing direct-call constants. */
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    /* FAKEMATCH: forwarding through the pointer prevents GCC 2.96 from sharing direct-call constants. */
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    /* FAKEMATCH: forwarding through the pointer prevents GCC 2.96 from sharing direct-call constants. */
    f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    /* FAKEMATCH: forwarding through the pointer prevents GCC 2.96 from sharing direct-call constants. */
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call7(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6)
{
    /* FAKEMATCH: forwarding through the pointer prevents GCC 2.96 from sharing direct-call constants. */
    f(a0, a1, a2, a3, a4, a5, a6);
}

static __inline__ void Call11(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8, s32 a9, s32 a10)
{
    /* FAKEMATCH: forwarding through the pointer prevents GCC 2.96 from sharing direct-call constants. */
    f(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
}

static __inline__ s32 Value0(s32 (*f)())
{
    /* FAKEMATCH: forwarding through the pointer prevents GCC 2.96 from sharing direct-call constants and keeps the result argument in r0. */
    return f();
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    /* FAKEMATCH: forwarding through the pointer prevents GCC 2.96 from sharing direct-call constants and keeps the result argument in r0. */
    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    /* FAKEMATCH: forwarding through the pointer prevents GCC 2.96 from sharing direct-call constants and keeps the result argument in r0. */
    return f(a0, a1);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    /* FAKEMATCH: forwarding through the pointer prevents GCC 2.96 from sharing direct-call constants and keeps the result argument in r0. */
    return f(a0, a1, a2);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    /* FAKEMATCH: forwarding through the pointer prevents GCC 2.96 from sharing direct-call constants and keeps the result argument in r0. */
    return f(a0, a1, a2, a3);
}

static __inline__ s32 Value6(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    /* FAKEMATCH: forwarding through the pointer prevents GCC 2.96 from sharing direct-call constants and keeps the result argument in r0. */
    return f(a0, a1, a2, a3, a4, a5);
}

static __inline__ s32 Value7(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6)
{
    /* FAKEMATCH: forwarding through the pointer prevents GCC 2.96 from sharing direct-call constants and keeps the result argument in r0. */
    return f(a0, a1, a2, a3, a4, a5, a6);
}

#endif

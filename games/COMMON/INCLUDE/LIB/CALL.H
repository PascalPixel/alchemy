/* Calls spelled through an inline function-pointer parameter.
 *
 * GCC 2.96 hoists a costly constant argument of a direct call into
 * a pseudo register that later uses in the block share; called through one of
 * these, each call loads its constants straight into the argument registers
 * and a value-returning call with arguments sets r0 last. The inline boundary
 * can also change the lifetime of a returned value without arguments.
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
 * whole-game compiler change. No candidate passed; stop without adoption.
 *
 * Single-use follow-up, 2026-10-02: twelve EN functions, one direct-call
 * replacement per function with all other source unchanged; none matched.
 * Constant reuse/register changes: FieldScene_RunClosingAuxiliarySequence,
 * FieldScene_RunFinalAuxiliarySequence, MakyuriChojo_RiseActorPair,
 * MakyuriChojo_SinkActorPair, FieldScene_RunScene3b4_02002290.
 * Argument order only: KareiMachi_SetupEntryActors,
 * HaidiaArashi_RunCallOutSequence, MakyuriHeya_SinkActorWithSparks,
 * HaidiaDou_RunLoweredActorScene, FieldScene_RunScene3ba_02000270,
 * ShindenHeya_ApplyEntryState, KareiMachi_RunExitDoor.
 * Two additional -da compilations of HaidiaArashi_RunCallOutSequence locate
 * a difference already in initial RTL: the direct call builds 256 in a
 * pseudo before assigning r0=22; the expanded inline sets r0=22 then r1=256.
 * Reload splits the constant at those different positions. Post-reload
 * scheduling interleaves r0 between the two constant instructions only for
 * the inline form. This is evidence about call expansion as well as the
 * scheduler, not permission to change either or a whole-game K3 result.
 *
 * Unchanged-source follow-up, 2026-10-02: six EN extents, seven call
 * controls, no candidate compilations. OBJECT16's matching direct speed
 * call with (actor, 0x9999, 0x4ccc) writes r2, r0, r1; neither constant
 * is used afterward. STAGED's early direct speed call reuses 0x8000 and
 * 0x1999 already written to the actor record, across the rectangle call.
 * Its later inline call freshly loads 0x4ccc and 0x1999 instead.
 * Raw 080cb7f8.s and 080cf8e0.s both add the same callback,
 * BattlePresentation_ProcessPendingGraphicsTransfer, with order 0x480,
 * but finalize r0 last and first respectively. Both discard the returned
 * index and reload that callback when removing it later. Native SI modes,
 * equal arguments, return use and later constant reuse therefore do not
 * determine one setup order. A flat rule based on native ABI, r0 first
 * or argument lifetime does not explain the family. This does not identify
 * the original source boundary, a responsible compiler pass or an admissible
 * compiler change.
 *
 * One-argument follow-up, 2026-10-02: four EN direct native flag-query
 * candidates with the settled s32 argument and return interface all failed.
 * FuneHeya_GetStepScript kept 0x8a0 in new saved r5 (464 -> 468 bytes),
 * KorimaKi_RunMessageScene kept 0x84c in r5 (208 -> 212),
 * FuneHeya_PlaceAnchorCharm kept 0x200 in r7 (160 -> 160; registers differ),
 * MakyuriHeya_TriggerFloorSwitch kept 0x256 in r6 (160 -> 164).
 * Each constant survived to a later query or setter, enlarging the save set.
 * Complete objects disagreed; no JA or alternate variant followed.
 * Successful single-argument forwarding removals therefore do not explain
 * these cross-call constant lifetimes. Keep all four inline boundaries. */
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

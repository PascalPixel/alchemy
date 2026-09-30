/* NONMATCHING: canonical H2 restored: 788/788 bytes, 24 differing halfwords,
 * 22 aligned edits, complete extent [02001d0c,02002020), pools included.
 * Sol H5 witness is preserved in 4c3cb3e05; H4 in 7b0e8236a. Both rejected
 * publication trials are closed. Keep the original admitted ownership and
 * pool boundary rather than the ordered-queue model's new pointer copies.
 * The Haidia nullable-lookup witness has no corresponding early actor
 * lifetime disagreement here: actor r7 and later phases already agree.
 * All remaining bytes are C not yet written; no adoption or alignment credit.
 * Sol H5 2026-09-27: 788/788 bytes, 288 differing halfwords,
 * 81 aligned edits. Direct callback publication restores callback r6 and
 * timer r8. Its pool word now precedes gravity's, as in the reference.
 * Ordered active/timer/delay/gravity writes survive, but timer's early
 * store forces its r8 reload; pointer calculations reverse, and the movhi
 * zero still loads through r2 rather than r3. The initial block has one
 * extra move; pool insertion moves the second motion argument load beyond
 * the pool and the final alignment vanishes. No ownership was changed.
 * Full normalized diff and diagnostic assembly checked. Semantic queue
 * ordering alone cannot explain the reference's simultaneous pointer and
 * literal live ranges. Reject H5; publication axis exhausted after H4/H5.
 * H2 remains the canonical admitted 788/24/22 candidate; preserve this
 * witness before restoring it. No adopted C or alignment bytes.
 * Sol H4 2026-09-27: 792/788 bytes, 321 differing halfwords,
 * 94 aligned edits. Prepare the callback before the reset queue; publish
 * active/timer/delay/gravity in observed order using volatile field writes.
 * Full normalized diff: queue order is recovered, but explicit callback
 * lifetime steals r8 from timer, which now occupies r6. lreg gives callback
 * p68 3 refs/252 length and timer p33 BASE_REGS; old shared callback p72
 * instead owned r6. The initial extra move shifts the movhi zero pool beyond
 * the second motion call; the resulting 4-byte growth fails the extent gate.
 * Diagnostic assembly agrees with ordinary scoring. New fact: DRIFT's
 * prepared-callback technique does not transfer across this long callback
 * lifetime. Reject H4; retain its witness before a direct-publication trial.
 * H4 witness preserved in 7b0e8236a. No adopted bytes or alignment changes.
 * Historical canonical H2 follows:
 * 788 bytes, candidate 788, 24 differing halfwords, 22 halfword
 * edits (2026-09-27). FieldScene_RunMultiPhasePresentation, meant for
 * FIELD/ARUTIN_YAMA/F_01D0C.C as a single-overlay unit binding its names at
 * their runtime addresses (an import veneer's listing offset plus 0x8000).
 * Remaining: first-setup pointer/literal/store scheduling, flag OR scratch
 * registers, and wait-counter scheduling. Full extent/pool positions kept.
 * WALL: Initial motion setup scheduling and constant placement; typed
 * aggregate and grouped-store alternatives did not help.
 * H1 transfers the exact timed callback's signed timer/delay and active
 * fields through ARUTIN.H, its named function address, and FIELD_EVENT's
 * canonical prototypes. Callback still scores 200/200 bytes, zero diff.
 * Final animation arguments now match. Separate actor/motion pointers let
 * both state clears precede callback removal; flag OR scratch registers
 * also change. Initial setup and wait scheduling remain. Baseline 26/24.
 * H2 gives the actor and callback views one union owner, without changing
 * initialization order. Scores 788/24/22: both callback-removal/state-clear
 * sequences and second callback publication now match. H1's canonical
 * animation argument order stays exact. Initial setup is unchanged; no
 * grouped-store retry. Stronger than the saved 788/26/24 baseline.
 * H3: Object_UpdateAllMotion proves +0x48 is gravity, not effect velocity.
 * Exposing it in SceneMotion and using actor->motion.gravity, with every
 * store left in place, yields 788/24/23. Only the early gravity store shifts
 * one instruction; zero/pointer order, timer/delay order and swapped pool
 * entries do not close. Callback remains 200/200 exact. H2 is still best.
 * STOP this ownership axis: no remaining evidence for grouped stores or
 * another register-spelling search. Keep the proven gravity offset fact.
 * Checkpoint restores H2's stronger model; H3 is saved at b7f2622d1.
 * Missing EventEnd, local task and data bindings are now explicit. No DONE. */
#include "TYPES.H"

#include "FIELD_EFFECT.H"
#include "ARUTIN.H"

void SceneState_StoreParamsAndInstallTask(s32 x, s32 y, s32 z, s32 angle);

/* FAKEMATCH: Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

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

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

union TimedActor {
    struct FieldActor actor;
    struct SceneMotion motion;
};

struct Half { u16 v; };
extern s32 gKeyState;

void FieldScene_RunMultiPhasePresentation(void)
{
    struct Half zero;
    s16 *timer;
    s16 *delay;
    union TimedActor *actor;
    struct FieldActor *record;
    u32 n;

    actor = (union TimedActor *)Actor_Get(10);
    Engine_EventBegin();
    Engine_ActorStop(10);
    Call2(Engine_CameraSetSpeed, 0x26666, 0x4ccc);
    Call4(Engine_CameraMoveTo, 0x1170000, 0x400000, 0xd80000, 1);
    Engine_CameraWaitForMove();
    Engine_AudioPlayCue(147);
    Engine_ActorRunRepeatedMotion(10, 2);
    Engine_EventWait(40);
    Call3(Engine_ActorFaceDirection, 10, 0x3000, 20);
    Call3(Engine_ActorFaceDirection, 10, 0x5000, 20);
    Call3(Engine_ActorFaceDirection, 10, 0x8000, 40);
    Call2(Engine_CameraSetSpeed, 0xcccc, 0x1999);
    Call4(Engine_CameraMoveTo, 0x800000, 0x400000, 0xca0000, 1);
    /* FAKEMATCH: volatile stores keep phase before the two motion steps. */
    zero.v = 0;
    delay = &actor->motion.delay;
    *(volatile s32 *)&actor->motion.active = 0;
    timer = &actor->motion.timer;
    /* Gravity; the typed-motion store alternative is recorded above. */
    *(s32 *)&actor->actor.unknown_44[4] = 0x6666;
    *(volatile s16 *)delay = 0;
    *(volatile s16 *)timer = 0;
    actor->actor.update = (void (*)(union FieldObject *))SceneMotion_UpdateTimedActor;
    Call3(Engine_ActorSetSpeed, 10, 0x13333, 0x9999);
    Call3(Engine_ActorMoveToAndWait, 10, 212, 200);
    Call3(Engine_ActorMoveToAndWait, 10, 103, 200);
    actor->actor.update = NULL;
    actor->motion.state = zero.v;
    Engine_EventWait(10);
    Engine_ActorSetAnimation(10, 1);
    Engine_AudioPlayCue(229);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x10000, 0, 0x10000);
    Engine_EventWait(4);
    Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
    Engine_EventWait(20);
    Call3(Engine_ActorFaceDirection, 10, 0x5000, 40);
    Object_GetById(10)->unknown_5a &= 254;
    Call3(Engine_ActorSetSpeed, 10, 0x13333, 0x9999);
    record = Object_GetById(10);
    Engine_ActorSetSpriteFlags(record, 0);
    Engine_AudioPlayCue(153);
    record = Object_GetById(10);
    record->velocity_y = 0x40000;
    Call2((void (*)())Engine_ActorSetAnimation, 10, 3);
    Engine_ActorMoveToAndWait(10, 86, 214);
    Engine_ActorSetAnimation(10, 1);
    record = Object_GetById(10);
    Engine_ActorSetSpriteFlags(record, 1);
    Engine_EventWait(10);
    Engine_AudioPlayCue(229);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x20000, 0, 0x10000);
    Engine_EventWait(8);
    Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
    Engine_EventWait(40);
    {
        struct FieldActor *record = Object_GetById(10);
        /* FAKEMATCH: preserve the flag-read ordering. */
        u8 value = *(volatile u8 *)&record->unknown_5a;
    
        record->unknown_5a = (u8)(value | 1);
    }
    Call3(Engine_ActorFaceDirection, 10, 0x3000, 20);
    Engine_ActorFaceDirection(10, 0, 40);
    *(volatile s32 *)&actor->motion.active = 0;
    *(volatile s16 *)timer = 0;
    *(volatile s16 *)delay = 0;
    actor->actor.update = (void (*)(union FieldObject *))SceneMotion_UpdateTimedActor;
    Call3(Engine_ActorSetSpeed, 10, 0x13333, 0x9999);
    Call3(Engine_ActorMoveToAndWait, 10, 120, 215);
    actor->actor.update = NULL;
    actor->motion.state = zero.v;
    Engine_ActorSetAnimation(10, 1);
    Engine_EventWait(16);
    Engine_AudioPlayCue(229);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x10000, 0, 0x10000);
    Engine_EventWait(4);
    Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
    Engine_EventWait(40);
    Engine_ActorFaceDirection(10, 0, 10);
    Engine_AudioPlayCue(147);
    Engine_ActorRunRepeatedMotion(10, 2);
    Engine_EventWait(80);
    Engine_ActorSetAnimation(10, 3);
    Call4(SceneState_StoreParamsAndInstallTask, 0x820000, 0, 0xa80000, 0);
    Engine_EventWait(60);
    n = 0;
    if (gKeyState == 0) {
        do {
            n++;
            Engine_EventWait(1);
            if (n > 59) {
                break;
            }
        } while (gKeyState == 0);
    }
    actor = (union TimedActor *)Object_GetById(0);
    Call2(Engine_CameraSetSpeed, 0x4cccc, 0x9999);
    Engine_CameraMoveTo(actor->actor.x.fixed, actor->actor.y.fixed, actor->actor.z.fixed, 1);
    Engine_CameraWaitForMove();
    Call1((void (*)())Engine_GameFlagSet, 0x905);
    Engine_EventEnd();
}

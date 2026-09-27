/* NONMATCHING: 788 bytes, candidate 788, 24 differing halfwords, 22 halfword
 * edits (2026-09-27). FieldScene_RunMultiPhasePresentation, meant for
 * FIELD/ARUTIN_YAMA/F_01D0C.C as a single-overlay unit binding its names at
 * their runtime addresses (an import veneer's listing offset plus 0x8000).
 * Remaining: first-setup pointer/literal/store scheduling, flag OR scratch
 * registers, and wait-counter scheduling. Complete extent and pools match.
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
extern s32 Data_03001c94;

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
    *(s32 *)&actor->actor.unknown_44[4] = 0x6666;
    *(volatile s16 *)delay = 0;
    *(volatile s16 *)timer = 0;
    actor->actor.update = (void (*)(union FieldObject *))SceneMotion_UpdateTimedActor;
    Call3(Engine_ActorSetSpeed, 10, 0x13333, 0x9999);
    Call3(Engine_ObjectMotionSetPositionAndCommit, 10, 212, 200);
    Call3(Engine_ObjectMotionSetPositionAndCommit, 10, 103, 200);
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
    Engine_ActorGet(10)->unknown_5a &= 254;
    Call3(Engine_ActorSetSpeed, 10, 0x13333, 0x9999);
    record = Engine_ActorGet(10);
    Engine_ActorSetSpriteFlags(record, 0);
    Engine_AudioPlayCue(153);
    record = Engine_ActorGet(10);
    record->velocity_y = 0x40000;
    Call2((void (*)())Engine_ActorSetAnimation, 10, 3);
    Engine_ObjectMotionSetPositionAndCommit(10, 86, 214);
    Engine_ActorSetAnimation(10, 1);
    record = Engine_ActorGet(10);
    Engine_ActorSetSpriteFlags(record, 1);
    Engine_EventWait(10);
    Engine_AudioPlayCue(229);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x20000, 0, 0x10000);
    Engine_EventWait(8);
    Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
    Engine_EventWait(40);
    {
        struct FieldActor *record = Engine_ActorGet(10);
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
    Call3(Engine_ObjectMotionSetPositionAndCommit, 10, 120, 215);
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
    if (Data_03001c94 == 0) {
        do {
            n++;
            Engine_EventWait(1);
            if (n > 59) {
                break;
            }
        } while (Data_03001c94 == 0);
    }
    actor = (union TimedActor *)Engine_ActorGet(0);
    Call2(Engine_CameraSetSpeed, 0x4cccc, 0x9999);
    Engine_CameraMoveTo(actor->actor.x.fixed, actor->actor.y.fixed, actor->actor.z.fixed, 1);
    Engine_CameraWaitForMove();
    Call1((void (*)())Engine_GameFlagSet, 0x905);
    Engine_EventEnd();
}

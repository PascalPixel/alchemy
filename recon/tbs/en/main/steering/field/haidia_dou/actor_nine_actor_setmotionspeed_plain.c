/* NONMATCHING: 2026-10-01 brief Wave2 Actor_SetMotionSpeed plain-source attempt.
 * Removing this one source device changes FieldScene_RunScene3a6_020014ac, FieldScene_RunScene3a6SequenceA, FieldScene_RunScene3a6SequenceB.
 * Remaining difference: a direct call changes FieldScene_RunScene3a6_020014ac from sub sp, sp, #8 to ldr r6, .L0 (117/118 assembly lines).
 * Measured with the existing TBS agscc option set, EN edition; no option changes.
 * Production retains the measured helper with a body-local FAKEMATCH reason.
 */
#include "../../../../../../../games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_DOU/HAIDIA.H"

/* Draft context: this removable helper isolates the attempted device. */
static __inline__ void Actor_OffsetDestination(s32 actor, s32 dx, s32 dz)
{
    ObjectMotion_OffsetPositionAndResetMotion(actor, dx, dz);
}

/* Draft context: these removed shared adapters isolate this one attempted device. */
static inline void Event_Begin(void)
{
    Engine_EventBegin();
}

static inline void Event_End(void)
{
    Engine_EventEnd();
}

static inline void Actor_SetSpritePriority(s32 actor, s32 priority)
{
    Engine_ActorSetSpritePriority(actor, priority);
}

void FieldScene_RunScene3a6_020014ac(void)
{

    u32 i;
    s32 record;
    s32 zero;

    Event_Begin();
    Battle_WaitMode0(10);
    ObjectMotion_SetSpeedParameters(ACTOR_PARTY_LEADER, 0x8000, 0x1999);
    Object_SetModeById(ACTOR_PARTY_LEADER, 8);
    Battle_WaitMode0(15);
    Actor_OffsetDestination(ACTOR_PARTY_LEADER, 8, 0);
    Battle_WaitMode0(4);
    Audio_PlayCue(0x120);
    Audio_PlayCue(239);
    ObjectMotion_SetSpeedParameters(9, 0x8000, 0x1999);
    Object_SetModeById(9, 2);
    zero = 0;
    *(u8 *)((s32)Object_GetById(9) + 85) = zero;
    record = Actor_Get(9);
    *(s32 *)(record + 68) = zero;
    Actor_OffsetDestination(9, 12, 0);
    ObjectMotion_CommitCurrentPositionAndActivate(ACTOR_PARTY_LEADER);
    Object_SetModeById(ACTOR_PARTY_LEADER, 1);
    ObjectMotion_CommitCurrentPositionAndActivate(9);
    Audio_PlayCue(0x120);
    Audio_PlayCue(213);
    Object_SetModeById(9, 3);
    *(u8 *)((s32)Object_GetById(9) + 85) = 3;
    Actor_OffsetDestination(9, 6, 0);
    SceneActor_WaitActorDescent((u8 *)Actor_Get(9));
    Object_SetModeById(9, 8);
    Actor_SetSpritePriority(9, 3);
    *(u8 *)((s32)Object_GetById(9) + 35) = 2;
    StagedActor_FillGridAttributeRectangle(0, 12, 16, 1, 4, 0);
    StagedActor_FillGridAttributeRectangle(0, 13, 16, 1, 4, 0);
    GameFlag_Set(0x202);
    Audio_PlayCue(240);
    Event_End();
}


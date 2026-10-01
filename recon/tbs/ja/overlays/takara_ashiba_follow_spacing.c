/* Draft: Treasure Isle scaffold following spacing.
 * 2026-10-01: International leader steps24px and first follower16px;
 * Japanese uses24px for both in the first sequence and16px for later leader steps. Five complete
 * owners differ at those immediate operands only after layout fixes.
 * Ordinary approved TBS flags.
 */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "CALL.H"
#include "STAGED_ACTOR.H"

extern s16 Data_02000240[];
s32 *Engine_GetTriggerActor(s32 slot);
void ObjectMotion_SetSpeedParameters();
void ObjectMotion_OffsetPositionAndResetMotion();
void ObjectMotion_ResetAndSetPosition();
void ObjectMotion_CommitCurrentPositionAndActivate();
void Battle_Reset();
void Battle_WaitMode0();
void BattleFx_FinishAction();
void AudioCommand_Play();

void FieldScene_RunSupplementalSequenceOne(void)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x1e666, 0xf333);
    Actor_SetSpeed(8, 0x1e666, 0xf333);
    Audio_PlayCue(188);
    record = Engine_GetTriggerActor(0);
    if (record != 0) {
        Actor_SetDestination(8, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Engine_ActorWaitForMove(8);
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 24);
    Engine_EventWait(4);
    Audio_PlayCue(188);
    Actor_SetDestinationOffset(8, 0, 16);
    Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
    Actor_SetDestination(8, 0x168, 152);
    Engine_ActorWaitForMove(8);
    Engine_EventEnd();
    GameFlag_Clear(0x220);
}

void FieldScene_RunScene3b4SequenceA(void)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x1b333, 0xd999);
    Actor_SetSpeed(9, 0x1b333, 0xd999);
    Audio_PlayCue(188);
    record = Engine_GetTriggerActor(0);
    if (record != 0) {
        Actor_SetDestination(9, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Engine_ActorWaitForMove(9);
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 24);
    Audio_PlayCue(188);
    Engine_EventWait(4);
    Actor_SetDestinationOffset(9, 0, 16);
    Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
    Actor_SetDestination(9, 168, 0x108);
    Engine_ActorWaitForMove(9);
    Engine_EventEnd();
    GameFlag_Clear(0x220);
}

void FieldScene_RunPrimarySequence(void)
{
    u32 i;
    s32 record;
    s32 base3_2000240;

    base3_2000240 = (s32)Data_02000240;
    if (*(s16 *)((base3_2000240 + 0x24a)) != 10) {
        Engine_EventBegin();
        Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x1b333, 0xd999);
        Actor_SetSpeed(10, 0x1b333, 0xd999);
        Audio_PlayCue(188);
        record = Engine_GetTriggerActor(0);
        if (record != 0) {
            Actor_SetDestination(10, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Engine_ActorWaitForMove(10);
        Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 24);
        Engine_EventWait(4);
        Audio_PlayCue(188);
        Actor_SetDestinationOffset(10, 0, 16);
        Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
        Actor_SetDestination(10, 0x108, 0x168);
        Engine_ActorWaitForMove(10);
        Engine_EventWait(10);
        Engine_EventEnd();
    }
}

void TakaraAshiba_RunActorElevenFollowScene(void)
{
    s32 record;
    s32 base3_2000240;
    s32 v3;

    base3_2000240 = (s32)((u8 *)Data_02000240);
    if (*(s16 *)((base3_2000240 + 0x24a)) != 11) {
        Battle_Reset();
        Call3(ObjectMotion_SetSpeedParameters, 0, 0x1b333, 0xd999);
        ObjectMotion_SetSpeedParameters(11, 0x1b333, 0xd999);
        AudioCommand_Play(188);
        v3 = *(s32 *)((s32)Object_GetById(0) + 8) / 0x100000;
        if (v3 > *(s32 *)((s32)Object_GetById(11) + 8) / 0x100000) {
            ObjectMotion_OffsetPositionAndResetMotion(11, 8, 0);
        }
        v3 = *(s32 *)((s32)Object_GetById(0) + 8) / 0x100000;
        if (v3 < *(s32 *)((s32)Object_GetById(11) + 8) / 0x100000) {
            Call3(ObjectMotion_OffsetPositionAndResetMotion, 11, -8, 0);
        }
        ObjectMotion_CommitCurrentPositionAndActivate(11);
        record = (s32)Object_GetById(0);
        if (record != 0) {
            ObjectMotion_ResetAndSetPosition(11, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        ObjectMotion_CommitCurrentPositionAndActivate(11);
        ObjectMotion_OffsetPositionAndResetMotion(0, 0, 24);
        Battle_WaitMode0(4);
        AudioCommand_Play(188);
        ObjectMotion_OffsetPositionAndResetMotion(11, 0, 16);
        ObjectMotion_CommitCurrentPositionAndActivate(0);
        ObjectMotion_ResetAndSetPosition(11, 0x158, 0x168);
        ObjectMotion_CommitCurrentPositionAndActivate(11);
        Battle_WaitMode0(10);
        BattleFx_FinishAction();
    }
}

void FieldScene_RunScene3b4SequenceB(void)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x1b333, 0xd999);
    Actor_SetSpeed(12, 0x1b333, 0xd999);
    Audio_PlayCue(188);
    record = Engine_GetTriggerActor(0);
    if (record != 0) {
        Actor_SetDestination(12, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Engine_ActorWaitForMove(12);
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 24);
    Audio_PlayCue(188);
    Actor_SetDestinationOffset(12, 0, 16);
    Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
    Actor_SetDestination(12, 0x138, 232);
    Engine_ActorWaitForMove(12);
    Engine_EventEnd();
    GameFlag_Clear(0x220);
}

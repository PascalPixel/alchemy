/* The Lunpa fortress: the scene variants of actors 24 and 25 and the second
 * supplemental sequence. */
#include "FORTRESS.H"

void SelectActor25SceneVariant(void)
{
    if (GameFlag_IsSet(0x941)) {
        Event_SetMessage(0x2568);
        Event_ShowMessage(25, 0);
    } else {
        Event_SetMessage(0x2458);
        Event_ShowMessage(25, 0);
    }
}

void SelectActor24SceneVariant(void)
{
    if (GameFlag_IsSet(0x941)) {
        Event_SetMessage(0x2569);
        Event_ShowMessage(24, 0);
    } else {
        Event_SetMessage(0x244e);
        Event_ShowMessage(24, 0);
    }
}

/* Runs a gated sequence of parameterized calls on PRIMARY_ID (24) and, once
 * derived partway through, DERIVED_ID (25); a sequence id counting up from
 * 9569 is threaded through four of the calls one apart. Each of the two
 * outer gating checks has its own short fallback branch on PRIMARY_ID. */
void FieldScene_RunSupplementalSequenceTwo(void)
{
    s32 sequence_id;

    if (GameFlag_IsSet(2369) != 0) {
        if (GameFlag_IsSet(2382) == 0 && GameFlag_IsSet(788) == 0) {
        sequence_id = 9569;
        Event_SetMessage(sequence_id);
        Event_ShowMessage(PRIMARY_ID, 0);
        Value2(Engine_ActorRunRepeatedMotion, PRIMARY_ID, 1);
        Value1(Battle_WaitMode0, 30);
        Actor_SetSpeed(PRIMARY_ID, 6553, 3276);
        Value3(ObjectMotion_OffsetPositionAndResetMotion, PRIMARY_ID, -4, 0);
        Value1(ObjectMotion_CommitCurrentPositionAndActivate, PRIMARY_ID);
        Value2(Object_SetModeById, PRIMARY_ID, 3);
        Value1(Battle_WaitMode0, 60);
        Actor_SetSpeed(PRIMARY_ID, 13107, 6553);
        Actor_SetDestinationOffset(PRIMARY_ID, -6, 0);
        Value3(Engine_ActorFaceActor, PRIMARY_ID, 0, 0);
        Value1(ObjectMotion_CommitCurrentPositionAndActivate, PRIMARY_ID);
        Value1(Engine_EventSetMessage, sequence_id + 1);
        Event_ShowMessage(PRIMARY_ID, 0);
        Actor_RunRepeatedMotion(PRIMARY_ID, 1);
        Value3(Engine_ActorFaceActor, DERIVED_ID, PRIMARY_ID, 0);
        Value1(Engine_EventSetMessage, sequence_id + 2);
        Value2(Engine_EventShowMessage, PRIMARY_ID, 0);
        Value1(Battle_WaitMode0, 70);
        Value2(Object_SetModeById, DERIVED_ID, 3);
        Value1(Battle_WaitMode0, 60);
        Actor_SetSpeed(DERIVED_ID, 26214, 13107);
        Value3(Engine_ActorWalkTo, DERIVED_ID, 880, 112);
        Value1(ObjectMotion_CommitCurrentPositionAndActivate, DERIVED_ID);
        Value3(Engine_ActorFaceDirection, DERIVED_ID, 53248, 0);
        Value1(Engine_EventSetMessage, sequence_id + 3);
        Event_ShowMessage(PRIMARY_ID, 0);
        Value2(Object_SetModeById, PRIMARY_ID, 3);
        Value1(Battle_WaitMode0, 70);
        Value3(ObjectMotion_OffsetPositionAndResetMotion, PRIMARY_ID, 8, 0);
        Value1(ObjectMotion_CommitCurrentPositionAndActivate, PRIMARY_ID);
        Value2(Object_SetModeById, PRIMARY_ID, 5);
        Value1(Engine_EventSetMessage, sequence_id + 4);
        Event_ShowMessage(PRIMARY_ID, 0);
        Value3(Engine_ActorWalkTo, 0, 896, 120);
        Value1(ObjectMotion_CommitCurrentPositionAndActivate, 0);
        Value3(Engine_ActorFaceEachOther, 0, DERIVED_ID, 0);
        Value1(Battle_WaitMode0, 60);
        Actor_SetAnimation(DERIVED_ID, 3);
        Value1(Battle_WaitMode0, 30);
        GameFlag_Set(788);
        } else {
            Event_SetMessage(9575);
            Event_ShowMessage(PRIMARY_ID, 0);
        }
    } else {
        Event_SetMessage(0x244d);
        Event_ShowMessage(PRIMARY_ID, 0);
    }
}

void ConfigureSceneActor26(void)
{
    BattleFx_RunPageEffectForSlot(26, 1, 5);
    GameFlag_Set(0x94e);
}

void ConfigureSceneActor14(void)
{
    Actor_RunRepeatedMotion(14, 2);
    Event_SetMessage(0x2441);
    Event_ShowMessage(14, 0);
}

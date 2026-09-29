/* The Lunpa fortress: actor 25 asking the party not to wake Donpa, the scene
 * variants of actors 24 and 25 and the second supplemental sequence. */
#include "FORTRESS.H"
extern u8 MsgRunpaDifficultDonpaRight[];
extern u8 MsgRunpaDonpaGrateful[];
extern u8 MsgRunpaDonpaKnowsCoddled[];
extern u8 MsgRunpaFatherSorryDodonpa[];
extern u8 MsgRunpaFatherStayAngry[];
extern u8 MsgRunpaMaybeDodonpasEyes[];
extern u8 MsgRunpaOwwwDontHurt[];
extern u8 MsgRunpaShhhPleaseDont[];
extern u8 MsgRunpaSomeonePunishDodonpa[];
extern u8 MsgRunpaThankHelpDodonpa[];
extern u8 MsgRunpaWellWorriedDodonpa[];
extern u8 MsgRunpaZZZZ[];

void FieldScene_RunDonpaSleepingSequence(void)
{
    u32 i;
    s32 record;
    s32 msg;
    s32 msg2;

    Event_Begin();
    if (GameFlag_IsSet(0x941) != 0) {
        Event_SetMessage((s32)MsgRunpaDonpaGrateful);
        Event_ShowMessage(18, 0);
        Event_End();
    } else {
        if (GameFlag_IsSet(0x313) != 0) {
            Event_SetMessage((s32)MsgRunpaMaybeDodonpasEyes);
            Event_OpenMessage(25, 0);
            Event_End();
        } else {
            Actor_ShowEmote(25, 0x102, 30);
            Actor_FaceActor(25, ACTOR_PARTY_LEADER, 0);
            msg = (s32)MsgRunpaShhhPleaseDont;
            Event_SetMessage(msg);
            Event_ShowMessage(25, 0);
            Actor_FaceActor(25, 24, 0);
            Camera_MoveToActor(24, 1);
            Camera_WaitForMove();
            Event_Wait(60);
            Camera_MoveToActor(0, 1);
            Event_Wait(20);
            Actor_ShowEmote(25, 0x105, 60);
            Event_SetMessage(msg + 1);
            Event_ShowMessage(25, 0);
            Actor_ShowEmote(25, 0x107, 60);
            Event_SetMessage(msg + 2);
            Event_ShowMessage(25, 0);
            Event_Wait(70);
            Actor_ShowEmote(25, 0x100, 60);
            Actor_FaceActor(25, ACTOR_PARTY_LEADER, 0);
            Event_SetMessage(msg + 3);
            Event_OpenMessage(25, 0);
            if (Event_ChooseYesNo(0, 0) == 0) {
                Event_SetMessage(msg + 4);
                Event_OpenMessage(25, 0);
            } else {
                Event_SetMessage(msg + 5);
                Event_OpenMessage(25, 0);
            }
            Event_Wait(60);
            Actor_ShowEmote(25, 0x105, 60);
            msg2 = (s32)MsgRunpaDonpaKnowsCoddled;
            Event_SetMessage(msg2);
            Event_OpenMessage(25, 0);
            Actor_RunRepeatedMotion(25, 1);
            Event_SetMessage(msg2 + 1);
            Event_OpenMessage(25, 0);
            Actor_SetAnimationAndWait(25, 3);
            Event_SetMessage(msg2 + 2);
            Event_OpenMessage(25, 0);
            GameFlag_Set(0x313);
            Event_End();
        }
    }
}

void SelectActor25SceneVariant(void)
{
    if (GameFlag_IsSet(0x941)) {
        Event_SetMessage((s32)MsgRunpaDifficultDonpaRight);
        Event_ShowMessage(25, 0);
    } else {
        Event_SetMessage((s32)MsgRunpaSomeonePunishDodonpa);
        Event_ShowMessage(25, 0);
    }
}

void SelectActor24SceneVariant(void)
{
    if (GameFlag_IsSet(0x941)) {
        Event_SetMessage((s32)MsgRunpaFatherStayAngry);
        Event_ShowMessage(24, 0);
    } else {
        Event_SetMessage((s32)MsgRunpaFatherSorryDodonpa);
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
        sequence_id = (s32)MsgRunpaThankHelpDodonpa;
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
            Event_SetMessage((s32)MsgRunpaWellWorriedDodonpa);
            Event_ShowMessage(PRIMARY_ID, 0);
        }
    } else {
        Event_SetMessage((s32)MsgRunpaZZZZ);
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
    Event_SetMessage((s32)MsgRunpaOwwwDontHurt);
    Event_ShowMessage(14, 0);
}

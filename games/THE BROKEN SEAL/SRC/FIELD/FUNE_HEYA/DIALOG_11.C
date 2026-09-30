/* What actor 11 says aboard, by how far the choice of who rows has come. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
extern u8 MsgFuneDontFeelDontTalkMe[];
extern u8 MsgFuneDontTellGoing[];
extern u8 MsgFuneYouFinallyPickedSomeoneDidnt[];

s32 SceneState_ApplyLevelFromFlags(void);
void FuneHeya_TurnActorToOpenSide(s32 actor);
void FieldScene_RunStepThen10(s32 actor);
void FieldScene_RunPrimarySequence(s32 actor, s32 message, s32 flag);

/* Once flag 0x8a0 is set actor 11 will not talk; once someone is picked
 * (flag 0x300) the picked actor walks back to the leader; otherwise actor 11
 * asks, and the flag its yes sets depends on which of 0x92b, 0x92a and 0x929
 * is set. */
void FieldScene_RunActor11FlagDialogue(void)
{
    if (GameFlag_IsSet(0x8A0) != 0) {
        Event_Begin();
        Actor_SetAttachedEffect(11, 0x102);
        Event_Wait(40);
        Event_SetMessage((s32)MsgFuneDontFeelDontTalkMe);
        Event_ShowMessage(11, 0);
        Event_End();
    } else if (GameFlag_IsSet(0x300) != 0) {
        s32 actor = SceneState_ApplyLevelFromFlags();
        u8 *leader;

        Event_Begin();
        FuneHeya_TurnActorToOpenSide(actor);
        Event_SetMessage((s32)MsgFuneYouFinallyPickedSomeoneDidnt);
        FieldScene_RunStepThen10(11);
        Actor_SetAnimation(actor, 2);
        leader = (u8 *)Object_GetById(0);
        if (leader != 0) {
            Actor_SetDestination(actor, *(s16 *)(leader + 10), *(s16 *)(leader + 18));
        }
        Actor_WaitForMove(actor);
        Actor_SetPosition(actor, 0, 0);
        Event_End();
    } else if (GameFlag_IsSet(0x92b) != 0) {
        FieldScene_RunPrimarySequence(11, (s32)MsgFuneDontTellGoing, 0x993);
    } else if (GameFlag_IsSet(0x92a) != 0) {
        FieldScene_RunPrimarySequence(11, (s32)MsgFuneDontTellGoing, 0x91a);
    } else if (GameFlag_IsSet(0x929) != 0) {
        FieldScene_RunPrimarySequence(11, (s32)MsgFuneDontTellGoing, 0x938);
    } else {
        FieldScene_RunPrimarySequence(11, (s32)MsgFuneDontTellGoing, 0x92f);
    }
}

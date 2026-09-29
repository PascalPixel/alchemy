#include "TYPES.H"
/* The cave links the staged-actor code, so its scenes reach its engine
   imports by the names that code uses. */
#define FIELD_STAGED_ACTOR_IMPORTS
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern u8 MsgKaragoruWhyGoingBackRobinDo[];

/* Actor 11 catches up with a leader turning back while flag 0x9a0 is set
   and asks why. Going back sends the party to the Tolbi house at entrance
   30; staying on, he follows the leader out and the leader steps on. */
void FieldScene_RunScene3beSequenceB(void)
{
    struct FieldActor *leader;

    if (GameFlag_IsSet(0x98a) == 0 && GameFlag_IsSet(0x9a0) != 0) {
        Event_Begin();
        Actor_SetSpeed(11, 0x10000, 0x8000);
        leader = Actor_Get(ACTOR_PARTY_LEADER);
        if (leader != 0) {
            Actor_SetPosition(11, *(s32 *)((u8 *)leader + 8), *(s32 *)((u8 *)leader + 16));
        }
        Actor_SetDestinationOffset(11, -8, 16);
        Actor_WaitForMove(11);
        Actor_FaceDirection(11, 0xd000, 0);
        Event_Wait(10);
        Actor_FaceActor(ACTOR_PARTY_LEADER, 11, 0);
        Event_SetMessage((s32)MsgKaragoruWhyGoingBackRobinDo);
        Event_OpenMessage(11, 0);
        if (Event_ChooseYesNo(0, 0) == 0) {
            Event_ShowMessage(11, 0);
            Actor_WalkTo(11, 152, 232);
            GameFlag_Clear(0x9a0);
            Actor_WaitForMove(11);
            Actor_SetAnimation(11, 1);
            gGameState.saved_scene = (s32)&SceneId_TorebiHeya;
            gGameState.saved_entrance = 30;
        } else {
            gEventWork->message += 1;
            Event_ShowMessage(11, 0);
            Actor_SetAnimation(11, 2);
            leader = Actor_Get(ACTOR_PARTY_LEADER);
            if (leader != 0) {
                Actor_SetDestination(11, *(s16 *)((u8 *)leader + 10), *(s16 *)((u8 *)leader + 18));
            }
            Actor_WaitForMove(11);
            Actor_SetPosition(11, 0, 0);
            Event_Wait(30);
            Actor_SetAnimation(ACTOR_PARTY_LEADER, 2);
            Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 16);
            Actor_WaitForMove(ACTOR_PARTY_LEADER);
            Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
        }
        Event_End();
    }
}

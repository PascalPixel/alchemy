/* The Suhara desert: the late steps and actor 13's restoration. */
#include "SABAKU.H"
extern u8 MsgSuharaNowhereFound[];

void FieldScene_RunLateActor8Step(void) { SuharaSabaku_RestoreActorScaleAndBlend(8); }
void FieldScene_RunLateActor9Step(void) { SuharaSabaku_RestoreActorScaleAndBlend(9); }
void FieldScene_RunLateActor10Step(void) { SuharaSabaku_RestoreActorScaleAndBlend(10); }
void FieldScene_RunLateActor11Step(void) { SuharaSabaku_RestoreActorScaleAndBlend(11); }
void FieldScene_RunLateActor12Step(void) { SuharaSabaku_RestoreActorScaleAndBlend(12); }
void FieldScene_RunActorThirteenRestoration(void)
{
    u32 i;
    u8 *record;

    if (GameFlag_IsSet(0x9a0) == 0) {
    } else {
        if (GameFlag_IsSet(0x1b7) != 0) {
        } else {
            if (GameFlag_IsSet(0x9b0) == 0) {
            } else {
                GameFlag_Set(0x9b5);
                Event_Begin();
                Event_SetMessage((s32)MsgSuharaNowhereFound);
                /* Record layout observed here: s32 at +8, s32 at +16. */
                record = Actor_Get(ACTOR_PARTY_LEADER);
                if (record != 0) {
                    Actor_SetPosition(ACTOR_ID, *(s32 *)(record + 8), *(s32 *)(record + 16));
                }
                Actor_FaceActor(ACTOR_ID, 0xc000, 0);
                Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x1b8, 0x4e8);
                Actor_FaceDirection(ACTOR_ID, 0x4000, 0);
                Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x1bc, 0x4d8);
                Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 40);
                Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 30);
                Actor_SetAnimationAndWait(ACTOR_ID, 4);
                Event_ShowMessage(ACTOR_ID, 0);
                Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x105, 60);
                Actor_ShowEmote(ACTOR_ID, 0x105, 60);
                Event_ShowMessage(ACTOR_ID, 0);
                Event_Wait(30);
                Actor_RunRepeatedMotion(ACTOR_ID, 2);
                Event_ShowMessage(ACTOR_ID, 0);
                Actor_FaceDirection(ACTOR_ID, 0xc000, 30);
                Event_AskYesNo(ACTOR_ID, 0);
                Event_Wait(30);
                Actor_ShowEmote(ACTOR_ID, 0x106, 60);
                Event_ShowMessage(ACTOR_ID, 0);
                Actor_SetAnimationAndWait(ACTOR_ID, 3);
                Event_ShowMessage(ACTOR_ID, 0);
                Actor_SetSpeed(ACTOR_ID, 0xb333, 0x5999);
                Actor_WalkToAndWait(ACTOR_ID, 0x1b8, 0x4e8);
                Event_ShowMessage(ACTOR_ID, 0);
                Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
                Actor_SetAnimation(ACTOR_ID, 2);
                /* Record layout observed here: s16 at +10, s16 at +18. */
                record = Actor_Get(ACTOR_PARTY_LEADER);
                if (record != 0) {
                    Actor_SetDestination(ACTOR_ID, *(s16 *)(record + 10), *(s16 *)(record + 18));
                }
                Actor_WaitForMove(ACTOR_ID);
                Actor_SetPosition(ACTOR_ID, 0, 0);
                Event_End();
            }
        }
    }
}

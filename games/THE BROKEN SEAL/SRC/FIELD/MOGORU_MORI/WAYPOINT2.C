#include "MORI.H"

void SceneActor_RunActorTwelveThreeWaypointMotion(void)
{
    u8 *slot;

    slot = Actor_Get(12);

    /* r0 still holds the record returned above. */
    Event_Begin();

    FieldScene_RunSixCallSetupSequence(12, 1);
    FieldScene_RunScene39f_02000d90(12, 536, 344, 0x70000);       /* 134 << 2, 172 << 1, 224 << 11 */

    Effect_Spawn(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x100000,   /* 128 << 13 */
                  0, 0, 0, 1, 0);

    Camera_FollowActor(12, 1);
    Actor_FaceEachOther(12, ACTOR_PARTY_LEADER, 0);
    Battle_WaitMode0(20);
    Actor_StartRepeatedMotion(12, 2);
    Actor_SetAttachedEffect(12, 258);                     /* 129 << 1 */
    Battle_WaitMode0(60);

    /* Three waypoints, each at height 0x30000 (192 << 10); the X literals are
     * 146 << 2, 158 << 2 and 170 << 2 and the Z is the same 172 << 1. */
    FieldScene_RunScene39f_02000d90(12, 584, 344, 0x30000);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 12, 0);
    Battle_WaitMode0(6);

    FieldScene_RunScene39f_02000d90(12, 632, 344, 0x30000);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 12, 0);
    Battle_WaitMode0(6);

    FieldScene_RunScene39f_02000d90(12, 680, 344, 0x30000);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 12, 0);
    Battle_WaitMode0(6);

    GameFlag_Set(0x302);
    Actor_SetPosition(15, 0, 0);

    /* Common exit; no argument registers are set. */
    Event_End();
}

void FieldScene_RunStepFD4WithActor181(s32 a)
{
    Event_Begin();
    Actor_SetPosition(16, 0, 0);
    GameFlag_Set(4052);
    Item_ShowFound(ITEM_NUT, 3);
    Party_GiveItem(ITEM_NUT, 0);
    Event_End();
}

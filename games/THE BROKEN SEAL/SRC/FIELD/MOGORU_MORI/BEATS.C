#include "MORI.H"

void FieldScene_RunActorThirteenPresentationBeat(void)
{
    u8 *slot;

    slot = Actor_Get(13);

    /* r0 still holds the record returned above. */
    Event_Begin();

    FieldScene_RunSixCallSetupSequence(13, 1);
    FieldScene_RunScene39f_02000d90(13, 456, 104, 0x70000);       /* 228 << 1, 224 << 11 */
    Battle_WaitMode0(10);

    Effect_Spawn(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x40000,    /* 128 << 11 */
                  0, 0, 0, 1, 0);

    Camera_FollowActor(13, 1);
    Actor_FaceEachOther(13, ACTOR_PARTY_LEADER, 0);
    Battle_WaitMode0(20);
    Actor_StartRepeatedMotion(13, 2);
    Actor_SetAttachedEffect(13, 258);                     /* 129 << 1 */
    Battle_WaitMode0(60);

    FieldScene_RunScene39f_02000d90(13, 472, 136, 0x30000);       /* 236 << 1, 192 << 10 */
    Actor_FaceActor(ACTOR_PARTY_LEADER, 13, 0);
    Battle_WaitMode0(6);

    FieldScene_RunScene39f_02000d90(13, 504, 136, 0x33333);       /* 252 << 1, pooled height */
    Actor_FaceActor(ACTOR_PARTY_LEADER, 13, 0);
    Battle_WaitMode0(6);

    FieldScene_RunScene39f_02000d90(13, 552, 136, 0x38000);       /* 138 << 2, 224 << 10 */
    Actor_FaceActor(ACTOR_PARTY_LEADER, 13, 0);
    Battle_WaitMode0(6);

    FieldScene_RunScene39f_02000d90(13, 584, 136, 0x38000);       /* 146 << 2 */
    Actor_FaceActor(ACTOR_PARTY_LEADER, 13, 0);
    Battle_WaitMode0(6);

    Actor_SetPosition(13, 0, 0);
    GameFlag_Set(772);                         /* 193 << 2 */
    Actor_SetPosition(16, 0, 0);

    /* Common exit; no argument registers are set. */
    Event_End();
}

void FieldScene_RunScene39f_02001818(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    FieldScene_RunSixCallSetupSequence(14, 1);
    Call4(FieldScene_RunScene39f_02000d90, 14, 0x1a8, 0x1e0, 0x79999);
    Battle_WaitMode0(2);
    MogoruMori_SpawnPuffRing(14);
    Actor_SetChildValue(14, 15);
    record = Actor_Get(14);
    Actor_SetSpriteFlags(record, 0);
    Battle_WaitMode0(30);
    GameFlag_Set(0x305);
    Actor_SetPosition(17, 0x1a80000, 0x1e00000);
    Event_End();
}

void SceneActor_RunActorFourteenFourWaypointMotion(void)
{
    u8 *slot;

    slot = Actor_Get(14);

    /* r0 still holds the record returned above. */
    Event_Begin();

    FieldScene_RunSixCallSetupSequence(14, 1);
    FieldScene_RunScene39f_02000d90(14, 392, 504, 0x60000);       /* 196 << 1, 252 << 1, 192 << 11 */
    Battle_WaitMode0(10);

    Effect_Spawn(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x40000,    /* 128 << 11 */
                  0, 0, 0, 1, 0);

    Camera_FollowActor(14, 1);
    Actor_FaceEachOther(14, ACTOR_PARTY_LEADER, 0);
    Battle_WaitMode0(20);
    Actor_StartRepeatedMotion(14, 2);
    Actor_SetAttachedEffect(14, 258);                     /* 129 << 1 */
    Battle_WaitMode0(60);

    /* Four waypoints; Z is 132 << 2 and the height 192 << 10 throughout. */
    FieldScene_RunScene39f_02000d90(14, 360, 528, 0x30000);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 14, 0);
    Battle_WaitMode0(6);

    FieldScene_RunScene39f_02000d90(14, 328, 528, 0x30000);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 14, 0);
    Battle_WaitMode0(6);

    FieldScene_RunScene39f_02000d90(14, 288, 528, 0x30000);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 14, 0);
    Battle_WaitMode0(6);

    FieldScene_RunScene39f_02000d90(14, 256, 528, 0x30000);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 14, 0);
    Battle_WaitMode0(6);

    Camera_FollowActor(ACTOR_PARTY_LEADER, 1);
    Actor_SetPosition(14, 0, 0);
    Battle_WaitMode0(30);
    GameFlag_Set(0x306);
    Actor_SetPosition(17, 0, 0);

    /* Common exit; no argument registers are set. */
    Event_End();
}

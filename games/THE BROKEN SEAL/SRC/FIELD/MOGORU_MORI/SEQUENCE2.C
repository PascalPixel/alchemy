#include "MORI.H"

void FieldScene_RunSlot16WaypointSequence(void)
{
    u8 *slot;

    slot = Actor_Get(16);

    /* r0 still holds the record returned above. */
    Event_Begin();

    FieldScene_RunSixCallSetupSequence(16, 1);
    FieldScene_RunScene39f_02000d90(16, 456, 152, 0x60000);       /* 228 << 1, 192 << 11 */
    Battle_WaitMode0(10);

    Effect_Spawn(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x40000,    /* 128 << 11 */
                  0, 0, 0, 1, 0);

    Camera_FollowActor(16, 1);
    Actor_FaceEachOther(16, ACTOR_PARTY_LEADER, 0);
    Battle_WaitMode0(20);
    Actor_StartRepeatedMotion(16, 2);
    Actor_SetAttachedEffect(16, 258);                     /* 129 << 1 */
    Battle_WaitMode0(60);

    /* Three waypoints at height 0x30000 (192 << 10). */
    FieldScene_RunScene39f_02000d90(16, 448, 192, 0x30000);       /* 224 << 1 */
    Actor_FaceActor(ACTOR_PARTY_LEADER, 16, 0);
    Battle_WaitMode0(6);

    FieldScene_RunScene39f_02000d90(16, 424, 208, 0x30000);       /* 212 << 1 */
    Actor_FaceActor(ACTOR_PARTY_LEADER, 16, 0);
    Battle_WaitMode0(6);

    FieldScene_RunScene39f_02000d90(16, 424, 224, 0x30000);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 16, 0);
    Battle_WaitMode0(6);

    Camera_FollowActor(ACTOR_PARTY_LEADER, 1);
    Actor_SetPosition(16, 0, 0);
    Battle_WaitMode0(30);
    GameFlag_Set(776);                         /* 194 << 2 */
    Actor_SetPosition(20, 0, 0);

    /* Common exit; no argument registers are set. */
    Event_End();
}

void FieldScene_RunActor17CameraSequence(void)
{
    u8 *slot;

    slot = Actor_Get(17);

    /* r0 still holds the record returned above. */
    Event_Begin();

    FieldScene_RunSixCallSetupSequence(17, 1);
    FieldScene_RunScene39f_02000d90(17, 392, 104, 0x60000);       /* 196 << 1, 192 << 11 */
    Battle_WaitMode0(10);

    Effect_Spawn(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x40000,    /* 128 << 11 */
                  0, 0, 0, 1, 0);

    Camera_FollowActor(17, 1);
    Actor_FaceEachOther(17, ACTOR_PARTY_LEADER, 0);
    Battle_WaitMode0(20);
    Actor_StartRepeatedMotion(17, 2);
    Actor_SetAttachedEffect(17, 258);                     /* 129 << 1 */
    Battle_WaitMode0(60);

    FieldScene_RunScene39f_02000d90(17, 376, 152, 0x60000);       /* 188 << 1 */
    Actor_FaceActor(ACTOR_PARTY_LEADER, 17, 0);
    Battle_WaitMode0(10);

    FieldScene_RunScene39f_02000d90(17, 328, 160, 0x30000);       /* 164 << 1, 192 << 10 */
    Actor_FaceActor(ACTOR_PARTY_LEADER, 17, 0);
    Battle_WaitMode0(6);

    FieldScene_RunScene39f_02000d90(17, 296, 160, 0x30000);       /* 148 << 1 */
    Actor_FaceActor(ACTOR_PARTY_LEADER, 17, 0);
    Battle_WaitMode0(6);

    Camera_FollowActor(ACTOR_PARTY_LEADER, 1);
    Actor_SetPosition(17, 0, 0);
    Battle_WaitMode0(30);
    GameFlag_Set(0x309);
    Actor_SetPosition(21, 0, 0);

    /* Common exit; no argument registers are set. */
    Event_End();
}

void FieldScene_RunScene39f_02002004(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    FieldScene_RunSixCallSetupSequence(18, 1);
    Camera_MoveTo(0x2e80000, -1, 0x1f80000, 1);
    FieldScene_RunScene39f_02000d90(18, 0x2e8, 0x1f8, 0x90000);
    MogoruMori_SpawnPuffRing(18);
    Actor_SetChildValue(18, 15);
    record = Actor_Get(18);
    Actor_SetSpriteFlags(record, 0);
    Battle_WaitMode0(30);
    GameFlag_Set(0x30a);
    Actor_SetPosition(22, 0x2e80000, 0x1f80000);
    Event_End();
}

void FieldScene_RunActorEighteenEffectSequence(void)
{
    u8 *slot;

    slot = Actor_Get(18);

    /* r0 still holds the record returned above. */
    Event_Begin();

    FieldScene_RunSixCallSetupSequence(18, 1);
    FieldScene_RunScene39f_02000d90(18, 712, 536, 0x60000);       /* 178 << 2, 134 << 2, 192 << 11 */
    Battle_WaitMode0(10);

    Effect_Spawn(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x40000,    /* 128 << 11 */
                  0, 0, 0, 1, 0);

    Camera_FollowActor(18, 1);
    Actor_FaceEachOther(18, ACTOR_PARTY_LEADER, 0);
    Battle_WaitMode0(20);
    Actor_StartRepeatedMotion(18, 2);
    Actor_SetAttachedEffect(18, 258);                     /* 129 << 1 */
    Battle_WaitMode0(60);

    FieldScene_RunScene39f_02000d90(18, 712, 568, 0x60000);       /* 142 << 2 */
    Actor_FaceActor(ACTOR_PARTY_LEADER, 18, 0);
    Battle_WaitMode0(10);

    FieldScene_RunScene39f_02000d90(18, 712, 600, 0x30000);       /* 150 << 2, 192 << 10 */
    Actor_FaceActor(ACTOR_PARTY_LEADER, 18, 0);
    Battle_WaitMode0(6);

    FieldScene_RunScene39f_02000d90(18, 736, 640, 0x30000);       /* X += 24, 160 << 2 */
    Actor_FaceActor(ACTOR_PARTY_LEADER, 18, 0);
    Battle_WaitMode0(6);

    FieldScene_RunScene39f_02000d90(18, 736, 704, 0x30000);       /* 176 << 2 */
    Actor_FaceActor(ACTOR_PARTY_LEADER, 18, 0);
    Battle_WaitMode0(6);

    Camera_FollowActor(ACTOR_PARTY_LEADER, 1);
    Actor_SetPosition(18, 0, 0);
    Battle_WaitMode0(30);
    GameFlag_Set(0x30b);

    /* Common exit; no argument registers are set. */
    Event_End();
}

void FieldScene_RunScene39f_020021b0(void)
{
    s32 rec7;

    rec7 = Object_GetById(18);
    Event_Begin();
    Actor_SetPosition(18, 0x880000, 0x1680000);
    FieldScene_RunSixCallSetupSequence(18, 1);
    FieldScene_RunScene39f_02000d90(18, 136, 0x198, 0x80000);
    Battle_WaitMode0(10);
    Effect_Spawn(*(s32 *)(rec7 + 8), *(s32 *)(rec7 + 12), (*(s32 *)(rec7 + 16) + 0x40000), 0, 0, 0, 1, 0);
    Actor_FaceDirection(18, 0xc000, 40);
    Actor_SetAttachedEffect(18, 0x102);
    Actor_RunRepeatedMotion(18, 2);
    Camera_FollowActor(18, 1);
    FieldScene_RunScene39f_02000d90(18, 136, 0x1b8, 0x60000);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 18, 0);
    Battle_WaitMode0(10);
    FieldScene_RunScene39f_02000d90(18, 136, 0x1d8, 0x30000);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 18, 0);
    Battle_WaitMode0(6);
    FieldScene_RunScene39f_02000d90(18, 136, 0x1f8, 0x30000);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 18, 0);
    Battle_WaitMode0(6);
    Camera_FollowActor(ACTOR_PARTY_LEADER, 1);
    Actor_SetPosition(18, 0, 0);
    Battle_WaitMode0(60);
    GameFlag_Set(0x89d);
    Event_End();
}

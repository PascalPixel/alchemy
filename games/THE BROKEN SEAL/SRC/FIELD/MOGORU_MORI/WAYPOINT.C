#include "MORI.H"

/* The five tile-painting calls take (layer, x, z, width, height, value) and
 * all reach the same routine, but each keeps its own call word: the encoding
 * is per site, so they must not be collapsed onto one alias. */
void SceneActor_PassOffsetPointOfActorZero(void)
{
    s32 v[3];
    s32 *p = Actor_Get(ACTOR_PARTY_LEADER);

    v[0] = (p[2] & 0xfff00000) + 0x80000;
    v[1] = p[3];
    v[2] = (p[4] & 0xfff00000) + 0xffe80000;
    SceneActor_TryRunSlotZeroMoveStep(v);
}

void SceneActor_BobActorZeroWhenAheadClear(void)
{
    s32 pos[3];
    s32 *actor = Actor_Get(ACTOR_PARTY_LEADER);
    u8 *fp = (u8 *)actor + 0x55;
    s32 saved = *fp;

    pos[0] = (actor[2] & 0xfff00000) + 0x80000;
    pos[1] = actor[3];
    pos[2] = (actor[4] & 0xfff00000) + 0x280000;
    if (SceneActor_TryRunSlotZeroMoveStep(pos)!= 0) {
        Event_Begin();
        *fp = 0;
        Object_SetModeById(9, 7);
        actor[3] += -0x10000;
        actor[5] += -0x10000;
        WaitFrames(2);
        actor[3] += -0x10000;
        actor[5] += -0x10000;
        WaitFrames(10);
        actor[3] += 0x10000;
        actor[5] += 0x10000;
        WaitFrames(4);
        actor[3] += 0x10000;
        actor[5] += 0x10000;
        *fp = saved;
        Event_End();
    }
}

void FieldScene_RunScriptedSteps0And17E6(void)
{
    Event_Begin();
    Object_SetModeById(ACTOR_PARTY_LEADER, 1);
    Message_ShowCentered(MSG_BROKEN_SIGN_READS_NORTH_FUCHIN, 1);
    Event_End();
}

void FieldScene_RunActor10WaypointSequence(void)
{
    u8 *slot;

    slot = Actor_Get(10);

    /* r0 still holds the record returned above. */
    Event_Begin();

    FieldScene_RunSixCallSetupSequence(10, 1);
    FieldScene_RunScene39f_02000d90(10, 88, 120, 0x60000);        /* 192 << 11 */

    Effect_Spawn(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x180000,   /* 192 << 13 */
                  0, 0, 0, 1, 0);

    Camera_FollowActor(10, 1);
    Actor_FaceEachOther(10, ACTOR_PARTY_LEADER, 0);
    Battle_WaitMode0(20);
    Actor_StartRepeatedMotion(10, 2);
    Actor_SetAttachedEffect(10, 258);                     /* 129 << 1 */
    Battle_WaitMode0(60);

    /* Three waypoints, each at height 0x30000 (192 << 10). */
    FieldScene_RunScene39f_02000d90(10, 88, 152, 0x30000);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 10, 0);
    Battle_WaitMode0(10);

    FieldScene_RunScene39f_02000d90(10, 120, 192, 0x30000);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 10, 0);
    Battle_WaitMode0(10);

    FieldScene_RunScene39f_02000d90(10, 120, 240, 0x30000);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 10, 0);
    Battle_WaitMode0(10);

    GameFlag_Set(768);                       /* 192 << 2 */
    Actor_SetPosition(13, 0, 0);
    Actor_SetPosition(10, 0, 0);

    /* Common exit; no argument registers are set. */
    Event_End();
}

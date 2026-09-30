#include "SHIAN.H"

void FieldScene_RunScene3a0_02001060(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    ((void (*)())Engine_ActorEnableActionCallback)(18, 1);
    record = Actor_Get(18);
    *(s32 *)(record + 108) = 0;
    record = Actor_Get(18);
    *(s32 *)(record + 56) = -0x80000000;
    record = Engine_ActorGet(18);
    *(s32 *)(record + 64) = -0x80000000;
    record = Engine_ActorGet(18);
    *(s32 *)(record + 36) = 0;
    record = Engine_ActorGet(18);
    *(s32 *)(record + 44) = 0;
    record = Engine_ActorGet(18);
    *(s32 *)(record + 48) = 0;
    record = Actor_Get(18);
    *(s32 *)(record + 52) = 0;
    Actor_ShowEmote(18, 0x103, 0);
    Actor_StartRepeatedMotion(18, 2);
    Event_Wait(60);
    Actor_SetSpeed(18, 0x18000, 0xc000);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x18000, 0xc000);
    Actor_WalkTo(18, 0x118, 232);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x128, 232);
    Actor_WaitForMove(18);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 60);
    Actor_EnableActionCallback(18, ShianMura_ActionTable);
    record = Actor_Get(18);
    *(s32 *)(record + 108) = (s32)ShianMura_WatchGateTrigger;
    Engine_EventEnd();
}

u8 *SceneEffect_GetTertiaryData(void)
{
    return gShianMuraEvents;
}

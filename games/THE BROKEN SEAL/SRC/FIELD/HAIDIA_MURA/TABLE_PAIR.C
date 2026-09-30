#include "STAGED_MOTION.H"
extern u8 MsgHaidiaNotSneakingUpMtAleph[];

void SceneState_RunFlag204Step(void)
{
    s32 m, n;
    Event_Begin();
    m = 20;
    n = 50;
    Map_CopyCellAttributes(49, 53, 8, 4, m, n);
    HaidiaMura_RunWalkScene032B0(0, 10, 11, 1);
    GameFlag_Set(0x204);
    Event_End();
}

void SceneState_SetFlag204AndConfigureRegion49_46(void)
{
    s32 p5, p6;
    Event_Begin();
    HaidiaMura_RunWalkScene03380(0, 13, 10, 1);
    GameFlag_Clear(0x204);
    p5 = 20;
    p6 = 50;
    Map_CopyCellAttributes(49, 46, 8, 4, p5, p6);
    Event_End();
}

void FieldScene_RunScene373SequenceE(void)
{
    u32 i;
    u8 *rec7;
    s32 record;

    rec7 = Object_GetById(22);
    Event_Begin();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x20000);
    Actor_Jump(ACTOR_PARTY_LEADER, 5, 0);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 215, 0x193);
    rec7[90] |= 1;
    Actor_SetPosition(22, 0xa60000, 0x1770000);
    Actor_FaceDirection(22, 0x2000, 20);
    rec7[90] = (rec7[90] ^ 1);
    Actor_SetSpeed(22, 0x28000, 0x28000);
    Actor_Jump(22, 4, 0);
    Actor_WalkToAndWait(22, 202, 0x18b);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xb000, 0);
    Actor_FaceDirection(22, 0x3000, 24);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Event_Wait(20);
    Actor_SetSpeed(22, 0x18000, 0x10000);
    Actor_EnableActionCallback(ACTOR_PARTY_LEADER, gHaidiaMuraLeaderWalkActions);
    Event_Wait(10);
    Actor_ShowEmote(22, 0x103, 0);
    Actor_EnableActionCallback(22, gHaidiaMuraActor22WalkActions);
    Object_RefreshSelectorById(0);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x100, 0x1da);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Object_RefreshSelectorById(22);
    Actor_WalkToAndWait(22, 0x100, 0x1c8);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Actor_FaceDirection(22, 0x4000, 20);
    Actor_StartRepeatedMotion(22, 2);
    Event_Wait(20);
    Event_SetMessage((s32)MsgHaidiaNotSneakingUpMtAleph);
    Event_AskYesNo(22, 0);
    record = Actor_Get(22);
    *(s32 *)(record + 108) = (s32)SceneActor_RunStep18WhenTargetSet;
    Actor_EnableActionCallback(22, gHaidiaMuraActor22Actions);
    GameFlag_Set(0x823);
    Event_End();
}

void SceneState_RunTablePairWhenActor22State1(void)
{
    u8 *p = Actor_Get(22);
    if (GameFlag_IsSet(0x823) != 0) {
        u8 *q = p;
        q += 100;
        if (*(s16 *)q == 1) {
            FieldScene_RunScene373_02001490((s32)gHaidiaMuraPairTableA, (s32)gHaidiaMuraPairTableB);
        }
    }
}

void FieldScene_RunScene373_02001490(s32 a0, s32 a1)
{
    u32 i;
    s32 p8;
    s32 rec8;
    s32 record;

    p8 = a1;
    rec8 = Object_GetById(22);
    Event_Begin();
    Actor_StartRepeatedMotion(22, 2);
    Actor_ShowEmote(22, 0x100, 20);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 40);
    Actor_EnableActionCallback(ACTOR_PARTY_LEADER, a0);
    Event_Wait(10);
    Actor_ShowEmote(22, 0x103, 0);
    Object_SetActionCallbackAndRefreshById(22, p8);
    Object_RefreshSelectorById(0);
    Event_Wait(20);
    Actor_RunRepeatedMotion(22, 2);
    *(s32 *)(rec8 + 24) = 0x10000;
    *(s32 *)(rec8 + 28) = 0x10000;
    record = Object_GetById(ACTOR_PARTY_LEADER);
    *(s32 *)(record + 24) = 0x10000;
    *(s32 *)(record + 28) = 0x10000;
    Event_SetMessage((s32)MsgHaidiaNotSneakingUpMtAleph);
    Event_AskYesNo(22, 0);
    record = Actor_Get(22);
    *(s32 *)(record + 108) = (s32)SceneActor_RunStep18WhenTargetSet;
    Actor_EnableActionCallback(22, gHaidiaMuraActor22Actions);
    Event_End();
}

void SceneState_RunTablePairWhenActor22State2(void)
{
    u8 *p = Actor_Get(22);
    if (GameFlag_IsSet(0x823) != 0) {
        u8 *q = p;
        q += 100;
        if (*(s16 *)q == 2) {
            FieldScene_RunScene373_02001490((s32)gHaidiaMuraPairTableC, (s32)gHaidiaMuraPairTableD);
        }
    }
}

void SceneState_RunTablePairByActor22State(void)
{
    u8 *rec = Actor_Get(22);
    if (GameFlag_IsSet(0x823) != 0) {
        u8 *q = rec;
        s32 v;
        q += 100;
        v = *(s16 *)q;
        if (v == 1) {
            FieldScene_RunScene373_02001490((s32)gHaidiaMuraPairTableC, (s32)gHaidiaMuraPairTableB);
        } else if (v == 2) {
            FieldScene_RunScene373_02001490((s32)gHaidiaMuraPairTableC, (s32)gHaidiaMuraPairTableD);
        }
    }
}

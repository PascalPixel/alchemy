/* Tolbi town: actor lines, the entry setup and the first sequence. */
#include "MACHI.H"

void FieldScene_RunScene3b5_02000568(void)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    Call3(Engine_ActorFaceDirection, 26, 0x4000, 0);
    Engine_ActorStartRepeatedMotion(26, 2);
    Call1(Engine_EventSetMessage, 0x1fa2);
    Engine_EventShowMessage(26, 0);
    Engine_EventEnd();
}

void SceneDialogue_RunActor27Message1fa3(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage(0x1FA3);
    Engine_EventShowMessage(0x1B, 0);
    Engine_EventEnd();
}

void SceneDialogue_RunActor24Message235f(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage(0x235F);
    Engine_EventAskYesNo(24, 0);
    Engine_EventEnd();
}

void FieldScene_RunScene3b5_020005dc(void)
{

    u32 i;
    s32 record;

    Event_Begin();
    if (Value1(Engine_GameFlagIsSet, 0x8bf) == 0) {
        GameFlag_Set(0x8bf);
        Call1(Engine_EventSetMessage, 0x2368);
        Engine_EventShowMessage(19, 0);
        Engine_ItemShowFound(233, 3);
        Engine_EventShowMessage(19, 0);
        Engine_ActorSetAnimation(0, 1);
        Engine_PartyGiveItem(233, 0);
    } else {
        Call1(Engine_EventSetMessage, 0x236a);
        Engine_EventShowMessage(19, 0);
    }
    Engine_EventEnd();
}

void SceneScript_SetupActors(void)
{

    u8 *work = ((u8*)gEventWork);
    u32 no;
    s32 index;

    Engine_EventBegin();
    for (no = 8; no <= 65; no++) {
        u8 *actor = Engine_ActorGet(no);
        if (actor != NULL) {
            actor[85] = 0;
        }
    }
    index = *(s16 *)(work + 0x16c) - 1;
    Call1_02000644(Engine_AudioPlayCue, 158);
    Call3_02000644(Engine_MapAnimateCells, (s32)gTorebiMachiCellSteps[index].commands,
                 gTorebiMachiCellSteps[index].first,
                 gTorebiMachiCellSteps[index].second);
    Call3_02000644(Engine_ActorSetSpeed, 0, 0x8000, 0x4000);
    ((u8 *)Engine_ActorGet(0))[85] = 0;
    Engine_ActorSetAnimation(0, 2);
    if (index != 6) {
        Call3_02000644(Engine_ActorCenterAndWalk, 0, 2, -8);
        Engine_EventWait(10);
    }
    Engine_EventRequestExit(*(s16 *)(work + 0x16c));
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventEnd();
}

/*
 * Update callback: draw the object at the party leader's sprite priority, in
 * both places its sprite keeps it, and clear its priority flags.
 */
void SceneActor_CopyPlayerModeToActor(union FieldObject *object)
{
    s32 priority;

    if (object != NULL) {
        priority = Engine_ActorGet(0)->sprite->priority;
        object->actor.priority_flags = 0;
        object->actor.sprite->priority = priority;
        ((struct SceneSprite *)object->actor.sprite)->priority_15 = priority;
    }
}

s32 TorebiMachi_ApplyEntryState(s32 a0)
{
    u32 i;
    s32 record;
    s32 handler;
    s32 hidden;

    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    Call3(Engine_ActorSetPosition, 16, 0x1600000, 0x1600000);
    Actor_EnableActionCallback(16, gTorebiMachiActor16Action);
    record = Value1(Engine_ActorGet, 16);
    handler = (s32)SceneActor_UpdatePartnerProximity;
    ((struct SceneActor *)record)->proximity_flags = 1;
    *(s32 *)(record + 108) = handler;
    hidden = 0;
    Value3(Engine_ActorSetPosition, 17, 0x1700000, 0x1400000);
    Actor_EnableActionCallback(17, gTorebiMachiActor17Action);
    record = Value1(Engine_ActorGet, 17);
    ((struct SceneActor *)record)->proximity_flags = hidden;
    *(s32 *)(record + 108) = handler;
    record = Engine_ActorGet(14);
    *(s32 *)(record + 108) = (s32)SceneActor_CopyPlayerModeToActor;
    if (GameFlag_IsSet(0x8c1) != 0) {
        Call3(Engine_ActorSetPosition, 28, 0x13c0000, 0x1480000);
    }
    if (Value1(Engine_GameFlagIsSet, 0x201) != 0) {
        FieldScene_ResetActor9AndDrawTiles();
    }
    if (GameFlag_IsSet(0x200) != 0) {
        FieldScene_RunScene3b5_02000224();
        Engine_ActorSetAnimation(8, 4);
    }
    if (Value1(Engine_GameFlagIsSet, 0x950) != 0) {
        Call3(Engine_ActorSetPosition, 20, 0x2080000, 0x2300000);
        Call3(Engine_ActorSetPosition, 21, 0x2080000, 0x2300000);
        Call3(Engine_ActorSetPosition, 22, 0x2080000, 0x2300000);
        Call3(Engine_ActorSetPosition, 24, 0x2080000, 0x2300000);
        Call3(Engine_ActorSetPosition, 25, 0x2080000, 0x2300000);
        Call3(Engine_ActorSetPosition, 26, 0x2080000, 0x2300000);
        Call3(Engine_ActorSetPosition, 27, 0x2080000, 0x2300000);
    } else {
        if (Value1(Engine_GameFlagIsSet, 0x962) != 0) {
            Call3(Engine_ActorSetPosition, 27, 0x1180000, 0x500000);
            Call3(Engine_ActorFaceDirection, 27, 0x2000, 0);
            Engine_ActorSetAnimation(27, 1);
        }
    }
    return 0;
}

void FieldScene_RunScene3b5SequenceA(void)
{

    u32 i;
    s32 record;

    Engine_EventBegin();
    Call3(Engine_ActorWalkToAndWait, 0, 0x130, 0x138);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 28, 0x4000, 0);
    Engine_EventWait(20);
    Call1(Engine_EventSetMessage, 0xe3d);
    Event_OpenMessage(28, 0);
    if (Value2(Engine_EventChooseYesNo, 0, 0) == 0) {
        bump_step(1);
        Engine_EventShowMessage(28, 0);
        Call3(Engine_ActorSetSpeed, 28, 0x10000, 0x8000);
        Call3(Engine_ActorWalkToAndWait, 28, 0x140, 0x130);
        Call3(Engine_ActorWalkToAndWait, 28, 0x13c, 0x148);
        Call3(Engine_ActorFaceDirection, 28, 0xa000, 0);
        Call1(Engine_GameFlagSet, 0x8c1);
    } else {
        Engine_EventShowMessage(28, 0);
    }
    Engine_EventEnd();
}

void SceneState_SetValue30ThenCall(void)
{
    Engine_EventRequestExit(30);
    Engine_EventEnd();
}

void SceneState_PassWorkHalfword16C(void)
{

    s16 *cnt = (s16 *)(((u8*)gEventWork) + 0x16C);

    Engine_EventRequestExit(*cnt);
}

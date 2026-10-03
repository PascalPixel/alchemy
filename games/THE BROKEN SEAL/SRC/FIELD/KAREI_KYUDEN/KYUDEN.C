#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern const struct SceneEntrance gKareiKyudenEntrances[];
extern const struct SceneEntrance gKareiKyudenEntrancesOther[];

enum PartyEventsMessage {
    MSG_WHEN_HEARD_WERE_BACK_IVAN = 0x1b21,
    MSG_LORD_HAMMET_WILL_RELEASED_SOON = 0x1b83,
    MSG_HAS_LEGACY_LORD_HAMMETS_SILK = 0x1b88,
    MSG_ROBIN_SNEAKED_INTO_LUNPA_THATS = 0x1b91,
    MSG_ITS_IVAN_HIS_COMPANIONS_PERFECT = 0x2588
};

void Map_ClearLayerEntryFlag();
void Map_SetLayerEntryFlag();
void SceneChannel_ConfigureUniformAndHandoff(s32);
void SceneChannel_ConfigureUniformAndHandoff();
s32 Object_SetActionCallbackAndRefreshById();

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    void Map_ClearLayerEntryFlag();

    gEventWork->message += amount;
}

static __inline__ void Scene_AdvanceStep(s32 amount)
{

    gEventWork->message += amount;
}

/* The scene's message table, laid out after the code. */
extern u8 Placement_Messages[];

extern u8 gKareiKyudenPlacements[];
extern const struct ScenePlacement gKareiKyudenPlacementsOther[];
void FieldScene_PrepareActors(u8 *placements);

extern const struct SceneEvent gKareiKyudenEvents[];
extern const struct SceneEvent gKareiKyudenEventsOther[];

extern u8 MsgKareiHasLegacyLordHammetsSilk[];
extern u8 MsgKareiLordHammetWillReleasedSoon[];
extern u8 MsgKareiRobinSneakedIntoLunpaThats[];

void FieldScene_DispatchSceneByIndex(void);

extern u8 MsgKareiItsIvanHisCompanionsPerfect[];
extern u8 MsgKareiWhenHeardWereBackIvan[];
extern u8 KareiKyuden_PartyActions[];

/* Where the party appears in the scene it enters. */
const struct SceneEntrance *Scene_GetEntrances(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_KareiKyuden) {
        return gKareiKyudenEntrances;
    }
    return gKareiKyudenEntrancesOther;
}

/* Signed halfword table in RAM; index 225 selects the scene. */
s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetMessageTable(void)
{
    return Placement_Messages;
}

/* The actors placed in the palace, prepared before they are returned. */
const struct ScenePlacement *Scene_GetPlacements(void)
{
    if (gGameState.scene == (s32)&SceneId_KareiKyuden) {
        FieldScene_PrepareActors(gKareiKyudenPlacements);
        return (const struct ScenePlacement *)gKareiKyudenPlacements;
    }
    return gKareiKyudenPlacementsOther;
}

/* What the scene answers. */
const struct SceneEvent *Scene_GetEvents(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_KareiKyuden) {
        return gKareiKyudenEvents;
    }
    return gKareiKyudenEventsOther;
}

/* Signed halfword table in RAM; index 225 selects the scene. */
void SceneDialogue_RunActor13Message1b83(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgKareiLordHammetWillReleasedSoon);
    Engine_EventAskYesNo(13, 0);
    Engine_EventEnd();
}

void SceneDialogue_RunActor16Message1b88(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgKareiHasLegacyLordHammetsSilk);
    Engine_EventAskYesNo(16, 0);
    Engine_EventEnd();
}

void FieldScene_RunActorEightTurnDialogue(void)
{
    void Engine_EventEnd(void);

    Engine_EventBegin();
    Engine_ActorShowEmote(8, 0x100, 0x3C);
    Engine_EventSetMessage((s32)MsgKareiRobinSneakedIntoLunpaThats);
    Engine_EventShowMessageAndWait(8, 0, 0xA);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventShowMessageAndWait(8, 0, 0xA);
    Engine_ActorSetAnimationAndWait(8, 4);
    Engine_EventShowMessageAndWait(8, 0, 0xA);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventShowMessageAndWait(8, 0, 0xA);
    Engine_GameFlagSet(0x913);
    Engine_EventEnd();
}

void FieldScene_RunScene3aa_02000184(void)
{
    void Map_ClearLayerEntryFlag();

    u32 i;
    s32 record;
    struct EventWork *p5;

    p5 = gEventWork;
    Engine_EventBegin();
    Engine_EventWait(10);
    if (p5->touched_trigger == 4) {
        Engine_AudioPlayCue(188);
    } else {
        Engine_AudioPlayCue(158);
    }
    Map_ClearLayerEntryFlag(1);
    Map_ClearLayerEntryFlag(2);
    Engine_EventWait(10);
    Engine_ActorSetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 2);
    if (p5->touched_trigger == 4) {
        Engine_ActorSetDestinationOffset(ACTOR_PARTY_LEADER, 0, -16);
    } else {
        Engine_ActorCenterAndWalk(ACTOR_PARTY_LEADER, 3, -16);
    }
    Engine_EventWait(16);
    Engine_EventRequestExit(p5->touched_trigger);
    Map_SetLayerEntryFlag(1);
    Map_SetLayerEntryFlag(2);
    Engine_EventEnd();
}

/* The palace's scene start: open with the window transition and, in the
   palace itself, run its scene. */
s32 Scene_Initialize(void)
{
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    if (gGameState.scene == (s32)(u32)&SceneId_KareiKyuden) {
        FieldScene_DispatchSceneByIndex();
    }
    return 0;
}

/* Signed halfword table in RAM; index 225 selects the scene. */
void FieldScene_DispatchSceneByIndex(void)
{
    u8 *rec;
    s32 h;
    s32 x1 = 0x038a0000;
    s32 z1 = 0x01a60000;

    Map_SetLayerEntryFlag(1);
    Map_SetLayerEntryFlag(2);

    switch (gGameState.entrance) {
    case 9:
        if (Engine_GameFlagIsSet(0x941) != 0) {
            rec = (u8 *)Object_GetById(8);
            h = 0x1000;
            *(u16 *)(rec + 6) = h;

            if (Engine_GameFlagIsSet(0x914) == 0) {
                Scene_RunPartySequence();
            }
        } else {
            Engine_ActorSetPosition(9, 0, 0);
            if (Engine_GameFlagIsSet(0x321) != 0) {
                Engine_ActorSetPosition(8, x1, z1);
                rec = (u8 *)Object_GetById(8);
                h = 0xd000;
                *(u16 *)(rec + 6) = h;
            }
        }
        break;

    case 10:
    case 11:
        if (Engine_GameFlagIsSet(0x915) != 0) {
            s32 a5 = 4;
            s32 a6 = 3;
            Engine_MapCopyCellsTo(58, 70, 54, 70, a5, a6);
            {
                s32 b5 = 55;
                s32 b6 = 8;
                Engine_MapCopyCellAttributes(55, 9, 2, 1, b5, b6);
            }
            Engine_MapRedraw();
            Engine_TaskWait(1);
        }
        break;

    case 20:
        Engine_ActorSetPosition(9, 0, 0);
        if (Engine_GameFlagIsSet(0x109) == 0) {
            RunEventScript01();
        }
        break;

    default:
        break;
    }
}

void RunEventScript01(void)
{

    u32 i;
    s32 record;
    u8 *work;
    s32 v5;
    s32 tbl;

    Engine_EventBegin();
    Engine_CameraMoveTo(-1, -1, -1, 0);
    Engine_TaskWait(1);
    Engine_ColorBufferApplySource(0x10002, 0);
    Engine_ColorBufferApplyTarget(0x10002, 0);
    Engine_ColorBufferInterpolate(1);
    Engine_TaskWait(1);
    work = *(u8 **)&gEventWork;
    *(s32 *)(work + 0x1c8) = 24;
    *(s32 *)(work + 0x1c0) = 0x201;
    Engine_ActorSetPosition(8, 0x3580000, 0x1b80000);
    Engine_ActorSetPosition(ACTOR_PARTY_LEADER, 0x3580000, 0x1e60000);
    Engine_ActorSetPosition(ACTOR_GERALD, 0x3500000, 0x1f60000);
    Engine_ActorSetPosition(ACTOR_IVAN, 0x3680000, 0x1e60000);
    Engine_ActorSetPosition(ACTOR_MIA, 0x3700000, 0x1f60000);
    Engine_ActorSetPosition(10, 0x3480000, 0x2060000);
    Engine_ActorSetPosition(11, 0x3780000, 0x2060000);
    Engine_CameraMoveTo(0x3600000, -1, 0x1d80000, 0);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(8, 1);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(ACTOR_IVAN, 1);
    Engine_ActorSetAnimationAndWait(ACTOR_IVAN, 4);
    Engine_EventWait(20);
    Engine_ActorFaceDirection(ACTOR_IVAN, 0x8000, 20);
    Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, 0, 10);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Engine_ActorStartRepeatedMotion(ACTOR_GERALD, 1);
    Engine_ActorRunRepeatedMotion(ACTOR_MIA, 1);
    Engine_ActorFaceDirection(ACTOR_GERALD, 0xe000, 0);
    Engine_ActorFaceDirection(ACTOR_MIA, 0xa000, 10);
    Engine_ActorSetAnimation(ACTOR_GERALD, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_MIA, 3);
    Engine_EventWait(20);
    SceneChannel_ConfigureUniformAndHandoff(20);
    Engine_ActorSetAttachedEffect(8, 0x102);
    Engine_ActorStartRepeatedMotion(8, 2);
    Engine_EventWait(60);
    Engine_ActorFaceDirection(8, 0x5000, 10);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_ActorShowEmote(10, 0x100, 0);
    Engine_ActorStartRepeatedMotion(10, 2);
    Engine_EventWait(60);
    Engine_ActorFaceDirection(10, 0xf000, 20);
    Engine_ActorRunRepeatedMotion(11, 2);
    Engine_ActorFaceDirection(11, 0x9000, 40);
    Engine_ActorFaceDirection(10, 0xd000, 0);
    Engine_ActorFaceDirection(11, 0xb000, 20);
    Engine_ActorSetAnimationAndWait(11, 3);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_ActorFaceDirection(8, 0x3000, 10);
    Engine_ActorShowEmote(8, 0x105, 60);
    Engine_ActorRunRepeatedMotion(11, 2);
    Engine_ActorSetAnimationAndWait(11, 4);
    Engine_ActorSetAnimationAndWait(11, 4);
    Engine_ColorBufferApplyTarget(0x10000, 0);
    Engine_ColorBufferInterpolate(40);
    Engine_TaskWait(60);
    Engine_ActorShowEmote(8, 0x105, 60);
    Engine_EventSetMessage((s32)MsgKareiWhenHeardWereBackIvan);
    Engine_EventShowMessageAndWait(8, 0, 10);
    Engine_ActorRunRepeatedMotion(10, 2);
    Engine_EventShowMessageAndWait(10, 0, 10);
    Engine_ActorSetAnimationAndWait(11, 4);
    Engine_EventShowMessageAndWait(11, 0, 10);
    Engine_ActorFaceDirection(ACTOR_IVAN, 0x6000, 10);
    Engine_EventShowMessageAndWait(0x6002, 0, 10);
    Engine_ActorRunRepeatedMotion(11, 2);
    Engine_EventShowMessageAndWait(11, 0, 10);
    Engine_ActorSetAnimation(10, 4);
    Engine_EventShowMessageAndWait(10, 0, 10);
    Engine_ActorFaceDirection(ACTOR_IVAN, 0xc000, 10);
    Engine_EventShowMessageAndWait(0x2002, 0, 10);
    Engine_ActorShowEmote(8, 0x101, 60);
    Engine_EventShowMessageAndWait(8, 0, 10);
    Engine_ActorRunRepeatedMotion(11, 2);
    Engine_EventShowMessageAndWait(11, 0, 10);
    Engine_ActorFaceDirection(ACTOR_IVAN, 0x2000, 10);
    Engine_ActorSetAnimationAndWait(10, 3);
    Engine_EventShowMessageAndWait(10, 0, 10);
    Engine_ActorFaceDirection(ACTOR_IVAN, 0x6000, 10);
    Engine_ActorStartRepeatedMotion(ACTOR_IVAN, 1);
    Engine_EventShowMessageAndWait(0x6002, 0, 10);
    Engine_ActorSetAnimationAndWait(10, 3);
    Engine_ActorFaceDirection(ACTOR_IVAN, 0x8000, 10);
    Engine_EventAskYesNo(0x6002, 0);
    Engine_ActorSetAttachedEffect(8, 0x102);
    Engine_EventWait(60);
    Engine_EventShowMessageAndWait(8, 0, 10);
    Engine_ActorRunRepeatedMotion(ACTOR_IVAN, 2);
    Engine_EventWait(10);
    Engine_ActorFaceDirection(ACTOR_IVAN, 0xc000, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_IVAN, 3);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 3);
    Engine_EventShowMessageAndWait(ACTOR_GERALD, 0, 10);
    Engine_ActorShowEmote(8, 0x105, 60);
    Engine_ActorSetAnimationAndWait(ACTOR_MIA, 4);
    Engine_EventShowMessageAndWait(ACTOR_MIA, 0, 40);
    Engine_ActorFaceDirection(8, 0x1000, 40);
    Engine_ActorSetSpeed(8, 0x6666, 0x3333);
    Engine_ActorWalkToAndWait(8, 0x37c, 0x1b8);
    Engine_EventWait(40);
    Engine_ActorFaceDirection(8, 0xd000, 20);
    Engine_ActorShowEmote(8, 0x105, 60);
    ConfigureFourSceneChannelsAndHandoff(60);
    SceneChannel_ConfigureUniformAndHandoff(40);
    Engine_ActorWalkToAndWait(8, 0x358, 0x1b8);
    Engine_EventWait(40);
    Engine_ActorFaceDirection(8, 0x9000, 20);
    Engine_ActorShowEmote(8, 0x105, 60);
    Engine_ActorFaceDirection(10, 0xf000, 0);
    Engine_ActorFaceDirection(11, 0x9000, 40);
    Engine_ActorFaceDirection(10, 0xd000, 0);
    Engine_ActorFaceDirection(11, 0xb000, 10);
    Engine_ActorRunRepeatedMotion(8, 1);
    Engine_EventShowMessageAndWait(8, 0, 20);
    Engine_ActorSetAnimationAndWait(8, 4);
    Engine_EventShowMessageAndWait(8, 0, 20);
    Engine_ActorRunRepeatedMotion(10, 2);
    Engine_EventShowMessageAndWait(10, 0, 10);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_ActorFaceDirection(8, 0x5000, 10);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Engine_ActorFaceDirection(ACTOR_GERALD, 0x4000, 0);
    Engine_ActorFaceDirection(ACTOR_IVAN, 0x6000, 0);
    Engine_ActorFaceDirection(ACTOR_MIA, 0x6000, 0);
    Engine_ActorFaceDirection(11, 0x9000, 40);
    Engine_ActorFaceDirection(10, 0xf000, 20);
    Engine_EventShowMessageAndWait(10, 0, 20);
    Engine_ActorRunRepeatedMotion(11, 2);
    Engine_EventWait(20);
    Engine_ActorFaceDirection(11, 0xb000, 20);
    Engine_ActorSetAnimationAndWait(11, 3);
    Engine_EventShowMessageAndWait(11, 0, 10);
    Engine_ActorFaceDirection(11, 0x9000, 40);
    Engine_ActorSetAnimation(10, 3);
    Engine_ActorSetAnimationAndWait(11, 3);
    Engine_ActorFaceDirection(10, 0xd000, 0);
    Engine_ActorFaceDirection(11, 0xb000, 10);
    Engine_ActorRunRepeatedMotion(10, 2);
    Engine_EventShowMessageAndWait(10, 0, 20);
    SceneChannel_ConfigureUniformAndHandoff(20);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(40);
    Engine_EventShowMessageAndWait(8, 0, 10);
    Engine_ActorSetAttachedEffect(8, 0x102);
    Engine_EventWait(60);
    Engine_EventShowMessageAndWait(8, 0, 20);
    Engine_ActorSetAnimationAndWait(8, 4);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(8, 4);
    Engine_EventShowMessageAndWait(8, 0, 20);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
    Engine_ActorFaceDirection(ACTOR_GERALD, 0xe000, 20);
    Engine_EventOpenMessage(ACTOR_GERALD, 0);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
    v5 = 1;
    if (Engine_EventChooseYesNo(0, 0) == 1) {
        bump_step(1);
        v5 = 0;
    }
    Engine_EventShowMessageAndWait(ACTOR_GERALD, 0, 10);
    if (v5 != 0) {
        bump_step(1);
    }
    Engine_ActorFaceDirection(ACTOR_IVAN, 0x8000, 10);
    Engine_ActorSetAttachedEffect(ACTOR_IVAN, 0x102);
    Engine_EventWait(60);
    Engine_ActorShowEmote(8, 0x101, 0);
    Engine_ActorFaceDirection(8, 0x3000, 40);
    Engine_ActorFaceDirection(8, 0x5000, 20);
    Engine_ActorFaceDirection(8, 0x3000, 20);
    Engine_EventShowMessageAndWait(8, 0, 10);
    Engine_ActorRunRepeatedMotion(ACTOR_IVAN, 1);
    Engine_ActorFaceDirection(ACTOR_IVAN, 0xc000, 10);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Engine_ActorFaceDirection(ACTOR_GERALD, 0xc000, 10);
    Engine_EventShowMessageAndWait(0x2002, 0, 10);
    Engine_ActorShowEmote(8, 0x107, 60);
    Engine_EventShowMessageAndWait(8, 0, 10);
    Engine_ActorSetAnimationAndWait(8, 4);
    Engine_EventOpenMessage(8, 0);
    Engine_ActorFaceDirection(ACTOR_IVAN, 0x8000, 0);
    Engine_ActorFaceDirection(ACTOR_GERALD, 0xe000, 0);
    Engine_ActorFaceDirection(ACTOR_MIA, 0xa000, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventWait(20);
        Engine_ActorSetAnimationAndWait(8, 3);
        Engine_EventShowMessageAndWait(8, 0, 10);
        bump_step(1);
    } else {
        Engine_EventWait(20);
        Engine_ActorSetAnimationAndWait(8, 4);
        bump_step(1);
        Engine_EventShowMessageAndWait(8, 0, 10);
    }
    SceneChannel_ConfigureUniformAndHandoff(20);
    Engine_ActorFaceDirection(ACTOR_IVAN, 0x2000, 0);
    Engine_ActorFaceDirection(ACTOR_MIA, 0xa000, 40);
    Engine_ActorFaceDirection(ACTOR_IVAN, 0xc000, 0);
    Engine_ActorFaceDirection(ACTOR_MIA, 0xc000, 10);
    Engine_EventShowMessageAndWait(ACTOR_MIA, 0, 20);
    Engine_ActorShowEmote(8, 0x105, 60);
    Engine_EventShowMessageAndWait(8, 0, 40);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventShowMessageAndWait(8, 0, 40);
    Engine_ActorRunRepeatedMotion(ACTOR_IVAN, 1);
    Engine_EventWait(20);
    Engine_ActorSetSpeed(ACTOR_IVAN, 0xcccc, 0x6666);
    Engine_ActorWalkToAndWait(ACTOR_IVAN, 0x364, 0x1d8);
    Engine_EventShowMessageAndWait(0x2002, 0, 10);
    Engine_ActorSetAnimationAndWait(8, 4);
    Engine_EventShowMessageAndWait(8, 0, 10);
    ConfigureFourSceneChannelsAndHandoff(40);
    SceneChannel_ConfigureUniformAndHandoff(20);
    Engine_ActorShowEmote(8, 0x105, 60);
    Engine_EventShowMessageAndWait(8, 0, 10);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
    Engine_EventShowMessageAndWait(ACTOR_GERALD, 0, 10);
    Engine_ActorRunRepeatedMotion(8, 1);
    Engine_ActorFaceDirection(8, 0x5000, 10);
    Engine_EventShowMessageAndWait(8, 0, 10);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_ActorFaceDirection(8, 0x3000, 10);
    Engine_EventShowMessageAndWait(8, 0, 10);
    Engine_ActorSetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Engine_ActorSetAttachedEffect(ACTOR_GERALD, 0x102);
    Engine_ActorSetAttachedEffect(ACTOR_IVAN, 0x102);
    Engine_ActorSetAttachedEffect(ACTOR_MIA, 0x102);
    Engine_EventWait(40);
    Engine_EventShowMessageAndWait(0x2002, 0, 10);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventOpenMessage(8, 0);
    Engine_ActorStartRepeatedMotion(ACTOR_GERALD, 1);
    Engine_ActorStartRepeatedMotion(ACTOR_IVAN, 1);
    Engine_ActorRunRepeatedMotion(ACTOR_MIA, 1);
    Engine_ActorFaceDirection(ACTOR_GERALD, 0xe000, 0);
    Engine_ActorFaceDirection(ACTOR_IVAN, 0x6000, 0);
    Engine_ActorFaceDirection(ACTOR_MIA, 0xa000, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventWait(20);
        Engine_ActorSetAttachedEffect(8, 0x102);
        Engine_EventWait(40);
        Engine_EventShowMessageAndWait(8, 0, 10);
        bump_step(2);
    } else {
        Engine_EventWait(20);
        Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
        bump_step(1);
        Engine_EventShowMessageAndWait(ACTOR_GERALD, 0, 10);
        Engine_ActorSetAttachedEffect(8, 0x102);
        Engine_EventWait(40);
        Engine_EventShowMessageAndWait(8, 0, 10);
    }
    Engine_ActorShowEmote(ACTOR_MIA, 0x101, 40);
    Engine_ActorFaceDirection(ACTOR_MIA, 0xc000, 10);
    Engine_EventShowMessage(ACTOR_MIA, 0);
    Engine_ActorFaceDirection(ACTOR_GERALD, 0xc000, 0);
    Engine_ActorFaceDirection(ACTOR_IVAN, 0xc000, 20);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventShowMessageAndWait(8, 0, 10);
    Engine_ActorSetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Engine_ActorSetAttachedEffect(ACTOR_GERALD, 0x102);
    Engine_ActorSetAttachedEffect(ACTOR_IVAN, 0x102);
    Engine_ActorSetAttachedEffect(ACTOR_MIA, 0x102);
    Engine_EventWait(40);
    Engine_EventShowMessageAndWait(8, 0, 10);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, 0xe000, 0);
    Engine_ActorFaceDirection(ACTOR_GERALD, 0xe000, 0);
    Engine_ActorFaceDirection(ACTOR_MIA, 0xa000, 10);
    Engine_ActorSetAnimationAndWait(ACTOR_IVAN, 4);
    Engine_EventShowMessageAndWait(0x2002, 0, 10);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventShowMessageAndWait(8, 0, 10);
    Engine_ActorRunRepeatedMotion(8, 1);
    Engine_EventShowMessageAndWait(8, 0, 10);
    SceneChannel_ConfigureUniformAndHandoff(10);
    Engine_ActorSetAnimationAndWait(8, 4);
    Engine_EventShowMessageAndWait(8, 0, 20);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 4);
    Engine_ActorSetAnimation(ACTOR_GERALD, 4);
    Engine_ActorSetAnimation(ACTOR_IVAN, 4);
    Engine_ActorSetAnimationAndWait(ACTOR_MIA, 4);
    Engine_ActorSetAttachedEffect(8, 0x102);
    Engine_EventWait(60);
    Engine_EventShowMessageAndWait(8, 0, 10);
    ConfigureFourSceneChannelsAndHandoff(40);
    Engine_ActorSetAnimationAndWait(8, 4);
    Engine_EventShowMessageAndWait(8, 0, 10);
    SceneChannel_ConfigureUniformAndHandoff(20);
    Engine_ActorRunRepeatedMotion(ACTOR_IVAN, 2);
    Engine_EventShowMessageAndWait(0x2002, 0, 10);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventShowMessageAndWait(8, 0, 10);
    Engine_ActorShowEmote(ACTOR_MIA, 0x101, 60);
    Engine_EventShowMessageAndWait(ACTOR_MIA, 0, 10);
    Engine_ActorShowEmote(8, 0x108, 60);
    Engine_EventShowMessageAndWait(8, 0, 10);
    Engine_ActorSetAnimationAndWait(ACTOR_MIA, 3);
    Engine_EventShowMessageAndWait(ACTOR_MIA, 0, 10);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventShowMessageAndWait(8, 0, 10);
    Engine_ActorShowEmote(ACTOR_GERALD, 0x101, 40);
    Engine_EventShowMessageAndWait(ACTOR_GERALD, 0, 10);
    Engine_ActorFaceDirection(8, 0x5000, 10);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventShowMessageAndWait(8, 0, 10);
    Engine_ActorRunRepeatedMotion(10, 2);
    Engine_ActorSetAnimationAndWait(10, 4);
    Engine_EventShowMessageAndWait(10, 0, 10);
    Engine_ActorShowEmote(8, 0x105, 40);
    Engine_EventShowMessageAndWait(8, 0, 10);
    Engine_ActorFaceDirection(8, 0x3000, 10);
    Engine_ActorSetAnimation(8, 4);
    Engine_EventShowMessageAndWait(8, 0, 10);
    Engine_ActorRunRepeatedMotion(8, 1);
    Engine_ActorFaceDirection(8, 0x5000, 10);
    Engine_EventShowMessageAndWait(8, 0, 10);
    Engine_ActorRunRepeatedMotion(ACTOR_IVAN, 1);
    Engine_EventShowMessageAndWait(0x2002, 0, 10);
    Engine_ActorShowEmote(8, 0x100, 40);
    Engine_ActorFaceDirection(8, 0x3000, 10);
    Engine_EventShowMessageAndWait(8, 0, 10);
    ConfigureFourSceneChannelsAndHandoff(40);
    SceneChannel_ConfigureUniformAndHandoff(20);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 1);
    Engine_EventShowMessageAndWait(ACTOR_GERALD, 0, 10);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventShowMessageAndWait(8, 0, 10);
    Engine_ActorSetAnimation(8, 4);
    Engine_EventShowMessageAndWait(8, 0, 10);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(ACTOR_GERALD, 3);
    Engine_ActorSetAnimation(ACTOR_IVAN, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_MIA, 3);
    Engine_ActorRunRepeatedMotion(8, 1);
    Engine_EventShowMessageAndWait(8, 0, 10);
    Engine_ActorSetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Engine_ActorSetAttachedEffect(ACTOR_GERALD, 0x102);
    Engine_EventWait(60);
    Engine_ActorSetAnimationAndWait(8, 4);
    Engine_EventShowMessageAndWait(8, 0, 10);
    Engine_ActorStartRepeatedMotion(ACTOR_IVAN, 1);
    Engine_ActorRunRepeatedMotion(ACTOR_MIA, 1);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(8, 1);
    Engine_EventShowMessageAndWait(8, 0, 10);
    Engine_ActorSetAnimation(8, 3);
    Engine_EventShowMessageAndWait(8, 0, 10);
    Engine_ActorShowEmote(ACTOR_IVAN, 0x105, 60);
    Engine_EventShowMessageAndWait(0x2002, 0, 10);
    Engine_ActorFaceDirection(ACTOR_MIA, 0xa000, 10);
    Engine_EventShowMessageAndWait(ACTOR_MIA, 0, 10);
    Engine_ActorSetAnimationAndWait(ACTOR_IVAN, 4);
    Engine_ActorSetAnimationAndWait(ACTOR_IVAN, 4);
    Engine_ActorFaceDirection(ACTOR_IVAN, 0x2000, 10);
    Engine_ActorSetAnimation(ACTOR_IVAN, 4);
    Engine_EventShowMessageAndWait(0x2002, 0, 10);
    Engine_ActorSetAnimationAndWait(8, 4);
    Engine_EventShowMessageAndWait(8, 0, 10);
    Engine_ActorFaceDirection(ACTOR_IVAN, 0xc000, 0);
    Engine_ActorRunRepeatedMotion(ACTOR_MIA, 2);
    Engine_ActorFaceDirection(ACTOR_MIA, 0xc000, 10);
    Engine_EventShowMessageAndWait(ACTOR_MIA, 0, 10);
    Engine_ActorSetAnimationAndWait(8, 4);
    Engine_EventShowMessageAndWait(8, 0, 10);
    Engine_ActorStartRepeatedMotion(10, 2);
    Engine_EventShowMessageAndWait(10, 0, 10);
    Engine_ActorSetAnimationAndWait(11, 3);
    Engine_EventShowMessageAndWait(11, 0, 20);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventShowMessageAndWait(8, 0, 10);
    Engine_ActorSetAnimationAndWait(ACTOR_IVAN, 4);
    Engine_ActorSetAnimation(ACTOR_IVAN, 4);
    Engine_EventShowMessageAndWait(0x2002, 0, 10);
    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 3);
    Engine_EventShowMessageAndWait(1, 0, 10);
    Engine_ActorRunRepeatedMotion(8, 1);
    Engine_EventOpenMessage(8, 0);
    Engine_ActorFaceDirection(ACTOR_GERALD, 0xe000, 0);
    Engine_ActorFaceDirection(ACTOR_IVAN, 0x6000, 0);
    Engine_ActorFaceDirection(ACTOR_MIA, 0xa000, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventWait(20);
        SceneChannel_ConfigureUniformAndHandoff(10);
        Engine_ActorSetAnimation(ACTOR_GERALD, 3);
        Engine_ActorSetAnimation(ACTOR_IVAN, 3);
        Engine_ActorSetAnimationAndWait(ACTOR_MIA, 3);
        Engine_EventWait(10);
        Engine_EventShowMessageAndWait(ACTOR_GERALD, 0, 10);
        bump_step(2);
    } else {
        Engine_EventWait(10);
        Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
        bump_step(1);
        Engine_EventShowMessageAndWait(ACTOR_GERALD, 0, 10);
        SceneChannel_ConfigureUniformAndHandoff(10);
        Engine_ActorSetAnimation(ACTOR_GERALD, 3);
        Engine_ActorSetAnimation(ACTOR_IVAN, 3);
        Engine_ActorSetAnimationAndWait(ACTOR_MIA, 3);
        Engine_EventWait(10);
        Engine_EventShowMessageAndWait(ACTOR_GERALD, 0, 10);
    }
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventShowMessageAndWait(8, 0, 40);
    Engine_ActorFaceDirection(8, 0x5000, 10);
    Engine_ActorRunRepeatedMotion(8, 1);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(10, 1);
    Engine_EventWait(20);
    Engine_ActorFaceDirection(8, 0x3000, 10);
    Engine_ActorRunRepeatedMotion(8, 1);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(11, 1);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventShowMessageAndWait(8, 0, 10);
    Engine_ActorSetAnimation(10, 3);
    Engine_ActorSetAnimationAndWait(11, 3);
    Engine_ActorSetSpeed(10, 0x10000, 0x8000);
    Engine_ActorSetSpeed(11, 0x10000, 0x8000);
    Engine_ActorWalkTo(10, 0x350, 0x21c);
    Engine_ActorWalkToAndWait(11, 0x370, 0x21c);
    Engine_ActorSetPosition(10, 0, 0);
    Engine_ActorSetPosition(11, 0, 0);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(ACTOR_GERALD, 3);
    Engine_ActorSetAnimation(ACTOR_IVAN, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_MIA, 3);
    Engine_EventWait(20);
    Engine_ActorSetSpeed(ACTOR_GERALD, 0x10000, 0x8000);
    Engine_ActorSetSpeed(ACTOR_IVAN, 0x10000, 0x8000);
    Engine_ActorSetSpeed(ACTOR_MIA, 0x10000, 0x8000);
    tbl = (s32)KareiKyuden_PartyActions;
    Engine_ActorEnableActionCallback(ACTOR_GERALD, tbl);
    Engine_ActorEnableActionCallback(2, tbl);
    Object_SetActionCallbackAndRefreshById(3, tbl);
    work = *(u8 **)&gEventWork;
    *(s32 *)(work + 0x1c8) = 16;
    *(s32 *)(work + 0x1c0) = 0x209;
    Engine_GameFlagClear(FLAG_ARRIVAL_EVENT_PENDING);
    Engine_GameFlagSet(0x912);
    Engine_EventEnd();
}

void ConfigureFourSceneChannelsAndHandoff(s32 handoff)
{
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
    Engine_ActorFaceDirection(1, 0xe000, 0);
    Engine_ActorFaceDirection(2, 0x2000, 0);
    Engine_ActorFaceDirection(3, 0xa000, 0);
    if (handoff != 0) {
        Engine_EventWait(handoff);
    }
}

void SceneChannel_ConfigureUniformAndHandoff(s32 handoff)
{
    Engine_ActorFaceDirection(0, 0xc000, 0);
    Engine_ActorFaceDirection(1, 0xc000, 0);
    Engine_ActorFaceDirection(2, 0xc000, 0);
    Engine_ActorFaceDirection(3, 0xc000, 0);
    if (handoff != 0) {
        Engine_EventWait(handoff);
    }
}

void Scene_RunPartySequence(void)
{

    s32 record;
    u8 *work;
    s32 v5;
    s32 tbl;

    Engine_EventBegin();
    Engine_CameraMoveTo(-1, -1, -1, 0);
    Engine_TaskWait(1);
    Engine_CameraMoveTo(0x3600000, -1, 0x2180000, 0);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Engine_ActorSetPosition(ACTOR_PARTY_LEADER, 0x3600000, 0x2760000);
    gEventWork->start_transition = v5 = 0x100;
    gEventWork->transition_frames = 40;
    Engine_EventOpenScreen();
    Engine_CameraSetSpeed(0x6666, 0xccc);
    Engine_CameraMoveTo(0x3600000, -1, 0x1d80000, 1);
    Engine_ActorSetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    Engine_ActorSetSpeed(ACTOR_GERALD, 0xcccc, 0x6666);
    Engine_ActorSetSpeed(ACTOR_IVAN, 0xcccc, 0x6666);
    Engine_ActorSetSpeed(ACTOR_MIA, 0xcccc, 0x6666);
    Engine_ActorWalkToAndWait(ACTOR_PARTY_LEADER, 0x360, 0x1f2);
    record = (u8 *)Object_GetById(0);
    if (record != 0) {
        Engine_ActorSetPosition(ACTOR_GERALD, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = (u8 *)Object_GetById(0);
    if (record != 0) {
        Engine_ActorSetPosition(ACTOR_IVAN, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = (u8 *)Object_GetById(0);
    if (record != 0) {
        Engine_ActorSetPosition(ACTOR_MIA, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Engine_ActorWalkTo(ACTOR_PARTY_LEADER, 0x358, 0x1e6);
    Engine_ActorWalkTo(ACTOR_GERALD, 0x350, 0x1f6);
    Engine_ActorWalkTo(ACTOR_IVAN, 0x368, 0x1e6);
    Engine_ActorWalkToAndWait(ACTOR_MIA, 0x370, 0x1f6);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
    Engine_ActorSetAnimation(ACTOR_GERALD, 1);
    Engine_ActorSetAnimation(ACTOR_IVAN, 1);
    Engine_EventWait(10);
    SceneChannel_ConfigureUniformAndHandoff(10);
    Engine_ActorShowEmote(9, v5, 20);
    Engine_ActorFaceDirection(9, 0x5000, 20);
    Engine_EventSetMessage((s32)MsgKareiItsIvanHisCompanionsPerfect);
    Engine_EventShowMessageAndWait(0x2009, 0, 10);
    Engine_ActorShowEmote(8, v5, 20);
    Engine_ActorFaceDirection(8, 0x3000, 20);
    Engine_EventShowMessageAndWait(8, 0, 20);
    Engine_ActorShowEmote(8, 0x107, 60);
    Engine_EventShowMessageAndWait(8, 0, 10);
    Engine_ActorSetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Engine_ActorSetAttachedEffect(ACTOR_GERALD, 0x102);
    Engine_ActorSetAttachedEffect(ACTOR_IVAN, 0x102);
    Engine_ActorSetAttachedEffect(ACTOR_MIA, 0x102);
    Engine_EventWait(60);
    Engine_ActorShowEmote(9, 0x102, 60);
    Engine_ActorFaceDirection(9, 0x7000, 10);
    Engine_EventShowMessageAndWait(0x2009, 0, 10);
    Engine_ActorFaceDirection(8, 0x1000, 10);
    Engine_ActorShowEmote(8, 0x108, 20);
    Engine_EventShowMessageAndWait(8, 0, 40);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventShowMessageAndWait(8, 0, 20);
    Engine_ActorFaceDirection(8, 0x3000, 10);
    Engine_EventOpenMessage(8, 0);
    v5 = 1;
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventWait(10);
        Engine_ActorSetAnimation(8, 3);
    } else {
        Engine_EventWait(10);
        Scene_AdvanceStep(1);
        Engine_ActorSetAnimation(8, 4);
        v5 = 0;
    }
    Engine_EventShowMessageAndWait(8, 0, 10);
    if (v5 != 0) {
        Scene_AdvanceStep(1);
    }
    Engine_ActorStartRepeatedMotion(9, 2);
    Engine_ActorSetAttachedEffect(9, 0x102);
    Engine_EventWait(80);
    Engine_EventShowMessageAndWait(0x2009, 0, 10);
    Engine_ActorFaceDirection(8, 0x1000, 10);
    Engine_ActorShowEmote(8, 0x107, 40);
    Engine_ActorRunRepeatedMotion(ACTOR_IVAN, 3);
    Engine_EventShowMessageAndWait(0x2002, 0, 20);
    Engine_ActorShowEmote(8, 0x105, 60);
    Engine_ActorFaceDirection(8, 0x3000, 60);
    Engine_EventShowMessageAndWait(8, 0, 10);
    Engine_ActorStartRepeatedMotion(ACTOR_GERALD, 2);
    Engine_EventShowMessageAndWait(ACTOR_GERALD, 0, 10);
    Engine_ActorFaceDirection(9, 0x5000, 0);
    Engine_ActorFaceDirection(ACTOR_IVAN, 0x6000, 10);
    Engine_ActorStartRepeatedMotion(ACTOR_IVAN, 2);
    Engine_EventShowMessageAndWait(0x6002, 0, 10);
    Engine_ActorFaceDirection(ACTOR_IVAN, 0xc000, 10);
    Engine_ActorStartRepeatedMotion(ACTOR_IVAN, 2);
    Engine_EventShowMessageAndWait(0x2002, 0, 20);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventShowMessageAndWait(8, 0, 20);
    Engine_ActorFaceDirection(ACTOR_IVAN, 0x8000, 20);
    Engine_EventShowMessageAndWait(0x6002, 0, 10);
    Engine_ActorFaceDirection(ACTOR_MIA, 0xa000, 10);
    Engine_ActorShowEmote(ACTOR_MIA, 0x105, 40);
    Engine_EventShowMessageAndWait(ACTOR_MIA, 0, 10);
    Engine_ActorRunRepeatedMotion(ACTOR_IVAN, 2);
    Engine_EventWait(80);
    Engine_ActorSetAnimationAndWait(ACTOR_IVAN, 3);
    Engine_EventWait(20);
    Engine_ActorFaceDirection(ACTOR_GERALD, 0xe000, 10);
    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 4);
    Engine_EventOpenMessage(ACTOR_GERALD, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventWait(20);
        Scene_AdvanceStep(1);
    } else {
        Engine_EventWait(20);
        Engine_EventShowMessageAndWait(ACTOR_GERALD, 0, 10);
    }
    Engine_ActorSetAnimation(ACTOR_IVAN, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_MIA, 3);
    SceneChannel_ConfigureUniformAndHandoff(20);
    Engine_ActorRunRepeatedMotion(9, 2);
    Engine_EventShowMessageAndWait(0x2009, 0, 10);
    Engine_ActorRunRepeatedMotion(ACTOR_MIA, 2);
    Engine_EventShowMessageAndWait(ACTOR_MIA, 0, 10);
    Engine_ActorFaceDirection(9, 0x3000, 10);
    Engine_ActorSetAnimationAndWait(9, 3);
    Engine_EventShowMessageAndWait(0x2009, 0, 10);
    Engine_ActorShowEmote(ACTOR_IVAN, 0x101, 60);
    Engine_EventShowMessageAndWait(0x2002, 0, 10);
    Engine_ActorRunRepeatedMotion(9, 1);
    Engine_ActorFaceDirection(9, 0x5000, 10);
    Engine_EventShowMessageAndWait(0x2009, 0, 10);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
    Engine_EventShowMessageAndWait(ACTOR_GERALD, 0, 10);
    Engine_ActorSetAnimationAndWait(9, 4);
    Engine_EventShowMessageAndWait(0x2009, 0, 10);
    Engine_ActorRunRepeatedMotion(ACTOR_MIA, 1);
    Engine_EventShowMessageAndWait(ACTOR_MIA, 0, 10);
    Engine_ActorRunRepeatedMotion(8, 1);
    Engine_EventWait(20);
    Engine_EventShowMessageAndWait(8, 0, 10);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(ACTOR_GERALD, 3);
    Engine_ActorSetAnimation(ACTOR_IVAN, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_MIA, 3);
    Engine_EventWait(20);
    tbl = (s32)KareiKyuden_PartyActions;
    Engine_ActorEnableActionCallback(ACTOR_GERALD, tbl);
    Engine_ActorEnableActionCallback(2, tbl);
    Object_SetActionCallbackAndRefreshById(3, tbl);
    work = *(u8 **)&gEventWork;
    *(s32 *)(((s32)work + 0x1c8)) = 16;
    *(s32 *)(((s32)work + 0x1c0)) = 0x209;
    Engine_GameFlagClear(FLAG_ARRIVAL_EVENT_PENDING);
    Engine_GameFlagSet(0x914);
    Engine_EventEnd();
}

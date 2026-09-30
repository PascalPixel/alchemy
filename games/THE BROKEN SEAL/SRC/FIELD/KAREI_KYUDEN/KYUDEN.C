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
u8 *Object_GetById();
void SceneChannel_ConfigureUniformAndHandoff(s32);
void SceneChannel_ConfigureUniformAndHandoff();
s32 Object_SetActionCallbackAndRefreshById();

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    void Map_ClearLayerEntryFlag();

    gEventWork->message += amount;
}

static __inline__ void ConfigureSecond(s32 channel, s32 value, s32 zero)
{
    Actor_FaceDirection(channel, value, zero);
}

static __inline__ void ConfigureThird(s32 channel, s32 value, s32 zero)
{
    Actor_FaceDirection(channel, value, zero);
}

static __inline__ void ConfigureFourth(s32 channel, s32 value, s32 zero)
{
    Actor_FaceDirection(channel, value, zero);
}

static __inline__ void ConfigureFirst(s32 channel, s32 value, s32 zero)
{
    Actor_FaceDirection(channel, value, zero);
}

static __inline__ void ConfigureUniformSecond(s32 channel, s32 value, s32 zero)
{
    Actor_FaceDirection(channel, value, zero);
}

static __inline__ void ConfigureUniformThird(s32 channel, s32 value, s32 zero)
{
    Actor_FaceDirection(channel, value, zero);
}

static __inline__ void ConfigureUniformFourth(s32 channel, s32 value, s32 zero)
{
    Actor_FaceDirection(channel, value, zero);
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
    Event_Begin();
    Event_SetMessage((s32)MsgKareiLordHammetWillReleasedSoon);
    Event_AskYesNo(13, 0);
    Event_End();
}

void SceneDialogue_RunActor16Message1b88(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgKareiHasLegacyLordHammetsSilk);
    Event_AskYesNo(16, 0);
    Event_End();
}

void FieldScene_RunActorEightTurnDialogue(void)
{
    void Event_End(void);

    Event_Begin();
    Actor_ShowEmote(8, 0x100, 0x3C);
    Event_SetMessage((s32)MsgKareiRobinSneakedIntoLunpaThats);
    Event_ShowMessageAndWait(8, 0, 0xA);
    Actor_RunRepeatedMotion(8, 2);
    Event_ShowMessageAndWait(8, 0, 0xA);
    Actor_SetAnimationAndWait(8, 4);
    Event_ShowMessageAndWait(8, 0, 0xA);
    Actor_SetAnimationAndWait(8, 3);
    Event_ShowMessageAndWait(8, 0, 0xA);
    GameFlag_Set(0x913);
    Event_End();
}

void FieldScene_RunScene3aa_02000184(void)
{
    void Map_ClearLayerEntryFlag();

    u32 i;
    s32 record;
    struct EventWork *p5;

    p5 = gEventWork;
    Event_Begin();
    Event_Wait(10);
    if (p5->touched_trigger == 4) {
        Audio_PlayCue(188);
    } else {
        Audio_PlayCue(158);
    }
    Map_ClearLayerEntryFlag(1);
    Map_ClearLayerEntryFlag(2);
    Event_Wait(10);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 2);
    if (p5->touched_trigger == 4) {
        Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -16);
    } else {
        Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 3, -16);
    }
    Event_Wait(16);
    Event_RequestExit(p5->touched_trigger);
    Map_SetLayerEntryFlag(1);
    Map_SetLayerEntryFlag(2);
    Event_End();
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
        if (GameFlag_IsSet(0x941) != 0) {
            rec = Object_GetById(8);
            h = 0x1000;
            *(u16 *)(rec + 6) = h;

            if (GameFlag_IsSet(0x914) == 0) {
                Scene_RunPartySequence();
            }
        } else {
            Actor_SetPosition(9, 0, 0);
            if (GameFlag_IsSet(0x321) != 0) {
                Actor_SetPosition(8, x1, z1);
                rec = Object_GetById(8);
                h = 0xd000;
                *(u16 *)(rec + 6) = h;
            }
        }
        break;

    case 10:
    case 11:
        if (GameFlag_IsSet(0x915) != 0) {
            s32 a5 = 4;
            s32 a6 = 3;
            Map_CopyCellsTo(58, 70, 54, 70, a5, a6);
            {
                s32 b5 = 55;
                s32 b6 = 8;
                Map_CopyCellAttributes(55, 9, 2, 1, b5, b6);
            }
            Map_Redraw();
            Task_Wait(1);
        }
        break;

    case 20:
        Actor_SetPosition(9, 0, 0);
        if (GameFlag_IsSet(0x109) == 0) {
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

    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Task_Wait(1);
    ColorBuffer_ApplySource(0x10002, 0);
    ColorBuffer_ApplyTarget(0x10002, 0);
    ColorBuffer_Interpolate(1);
    Task_Wait(1);
    work = *(u8 **)&gEventWork;
    *(s32 *)(work + 0x1c8) = 24;
    *(s32 *)(work + 0x1c0) = 0x201;
    Actor_SetPosition(8, 0x3580000, 0x1b80000);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0x3580000, 0x1e60000);
    Actor_SetPosition(ACTOR_GERALD, 0x3500000, 0x1f60000);
    Actor_SetPosition(ACTOR_IVAN, 0x3680000, 0x1e60000);
    Actor_SetPosition(ACTOR_MIA, 0x3700000, 0x1f60000);
    Actor_SetPosition(10, 0x3480000, 0x2060000);
    Actor_SetPosition(11, 0x3780000, 0x2060000);
    Camera_MoveTo(0x3600000, -1, 0x1d80000, 0);
    Map_Redraw();
    Task_Wait(1);
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(40);
    Actor_RunRepeatedMotion(8, 1);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 4);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_IVAN, 0x8000, 20);
    Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 1);
    Actor_RunRepeatedMotion(ACTOR_MIA, 1);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xa000, 10);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Event_Wait(20);
    SceneChannel_ConfigureUniformAndHandoff(20);
    Actor_SetAttachedEffect(8, 0x102);
    Actor_StartRepeatedMotion(8, 2);
    Event_Wait(60);
    Actor_FaceDirection(8, 0x5000, 10);
    Actor_SetAnimationAndWait(8, 3);
    Actor_ShowEmote(10, 0x100, 0);
    Actor_StartRepeatedMotion(10, 2);
    Event_Wait(60);
    Actor_FaceDirection(10, 0xf000, 20);
    Actor_RunRepeatedMotion(11, 2);
    Actor_FaceDirection(11, 0x9000, 40);
    Actor_FaceDirection(10, 0xd000, 0);
    Actor_FaceDirection(11, 0xb000, 20);
    Actor_SetAnimationAndWait(11, 3);
    Event_Wait(10);
    Actor_RunRepeatedMotion(8, 2);
    Actor_FaceDirection(8, 0x3000, 10);
    Actor_ShowEmote(8, 0x105, 60);
    Actor_RunRepeatedMotion(11, 2);
    Actor_SetAnimationAndWait(11, 4);
    Actor_SetAnimationAndWait(11, 4);
    ColorBuffer_ApplyTarget(0x10000, 0);
    ColorBuffer_Interpolate(40);
    Task_Wait(60);
    Actor_ShowEmote(8, 0x105, 60);
    Event_SetMessage((s32)MsgKareiWhenHeardWereBackIvan);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_RunRepeatedMotion(10, 2);
    Event_ShowMessageAndWait(10, 0, 10);
    Actor_SetAnimationAndWait(11, 4);
    Event_ShowMessageAndWait(11, 0, 10);
    Actor_FaceDirection(ACTOR_IVAN, 0x6000, 10);
    Event_ShowMessageAndWait(0x6002, 0, 10);
    Actor_RunRepeatedMotion(11, 2);
    Event_ShowMessageAndWait(11, 0, 10);
    Actor_SetAnimation(10, 4);
    Event_ShowMessageAndWait(10, 0, 10);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 10);
    Event_ShowMessageAndWait(0x2002, 0, 10);
    Actor_ShowEmote(8, 0x101, 60);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_RunRepeatedMotion(11, 2);
    Event_ShowMessageAndWait(11, 0, 10);
    Actor_FaceDirection(ACTOR_IVAN, 0x2000, 10);
    Actor_SetAnimationAndWait(10, 3);
    Event_ShowMessageAndWait(10, 0, 10);
    Actor_FaceDirection(ACTOR_IVAN, 0x6000, 10);
    Actor_StartRepeatedMotion(ACTOR_IVAN, 1);
    Event_ShowMessageAndWait(0x6002, 0, 10);
    Actor_SetAnimationAndWait(10, 3);
    Actor_FaceDirection(ACTOR_IVAN, 0x8000, 10);
    Event_AskYesNo(0x6002, 0);
    Actor_SetAttachedEffect(8, 0x102);
    Event_Wait(60);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 20);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 10);
    Actor_ShowEmote(8, 0x105, 60);
    Actor_SetAnimationAndWait(ACTOR_MIA, 4);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 40);
    Actor_FaceDirection(8, 0x1000, 40);
    Actor_SetSpeed(8, 0x6666, 0x3333);
    Actor_WalkToAndWait(8, 0x37c, 0x1b8);
    Event_Wait(40);
    Actor_FaceDirection(8, 0xd000, 20);
    Actor_ShowEmote(8, 0x105, 60);
    ConfigureFourSceneChannelsAndHandoff(60);
    SceneChannel_ConfigureUniformAndHandoff(40);
    Actor_WalkToAndWait(8, 0x358, 0x1b8);
    Event_Wait(40);
    Actor_FaceDirection(8, 0x9000, 20);
    Actor_ShowEmote(8, 0x105, 60);
    Actor_FaceDirection(10, 0xf000, 0);
    Actor_FaceDirection(11, 0x9000, 40);
    Actor_FaceDirection(10, 0xd000, 0);
    Actor_FaceDirection(11, 0xb000, 10);
    Actor_RunRepeatedMotion(8, 1);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_SetAnimationAndWait(8, 4);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_RunRepeatedMotion(10, 2);
    Event_ShowMessageAndWait(10, 0, 10);
    Actor_RunRepeatedMotion(8, 2);
    Actor_FaceDirection(8, 0x5000, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x4000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0x6000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0x6000, 0);
    Actor_FaceDirection(11, 0x9000, 40);
    Actor_FaceDirection(10, 0xf000, 20);
    Event_ShowMessageAndWait(10, 0, 20);
    Actor_RunRepeatedMotion(11, 2);
    Event_Wait(20);
    Actor_FaceDirection(11, 0xb000, 20);
    Actor_SetAnimationAndWait(11, 3);
    Event_ShowMessageAndWait(11, 0, 10);
    Actor_FaceDirection(11, 0x9000, 40);
    Actor_SetAnimation(10, 3);
    Actor_SetAnimationAndWait(11, 3);
    Actor_FaceDirection(10, 0xd000, 0);
    Actor_FaceDirection(11, 0xb000, 10);
    Actor_RunRepeatedMotion(10, 2);
    Event_ShowMessageAndWait(10, 0, 20);
    SceneChannel_ConfigureUniformAndHandoff(20);
    Actor_RunRepeatedMotion(8, 2);
    Event_Wait(40);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_SetAttachedEffect(8, 0x102);
    Event_Wait(60);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_SetAnimationAndWait(8, 4);
    Event_Wait(20);
    Actor_SetAnimation(8, 4);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 20);
    Event_OpenMessage(ACTOR_GERALD, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
    v5 = 1;
    if (Event_ChooseYesNo(0, 0) == 1) {
        bump_step(1);
        v5 = 0;
    }
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 10);
    if (v5 != 0) {
        bump_step(1);
    }
    Actor_FaceDirection(ACTOR_IVAN, 0x8000, 10);
    Actor_SetAttachedEffect(ACTOR_IVAN, 0x102);
    Event_Wait(60);
    Actor_ShowEmote(8, 0x101, 0);
    Actor_FaceDirection(8, 0x3000, 40);
    Actor_FaceDirection(8, 0x5000, 20);
    Actor_FaceDirection(8, 0x3000, 20);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 10);
    Event_ShowMessageAndWait(0x2002, 0, 10);
    Actor_ShowEmote(8, 0x107, 60);
    ((void (*)())Engine_EventShowMessageAndWait)(8, 0, 10);
    Actor_SetAnimationAndWait(8, 4);
    Event_OpenMessage(8, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0x8000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xa000, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(20);
        Actor_SetAnimationAndWait(8, 3);
        Event_ShowMessageAndWait(8, 0, 10);
        bump_step(1);
    } else {
        Event_Wait(20);
        Actor_SetAnimationAndWait(8, 4);
        bump_step(1);
        Event_ShowMessageAndWait(8, 0, 10);
    }
    SceneChannel_ConfigureUniformAndHandoff(20);
    Actor_FaceDirection(ACTOR_IVAN, 0x2000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xa000, 40);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 10);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Actor_ShowEmote(8, 0x105, 60);
    Event_ShowMessageAndWait(8, 0, 40);
    Actor_RunRepeatedMotion(8, 2);
    Event_ShowMessageAndWait(8, 0, 40);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
    Event_Wait(20);
    Actor_SetSpeed(ACTOR_IVAN, 0xcccc, 0x6666);
    Actor_WalkToAndWait(ACTOR_IVAN, 0x364, 0x1d8);
    Event_ShowMessageAndWait(0x2002, 0, 10);
    Actor_SetAnimationAndWait(8, 4);
    Event_ShowMessageAndWait(8, 0, 10);
    ConfigureFourSceneChannelsAndHandoff(40);
    SceneChannel_ConfigureUniformAndHandoff(20);
    Actor_ShowEmote(8, 0x105, 60);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 10);
    Actor_RunRepeatedMotion(8, 1);
    Actor_FaceDirection(8, 0x5000, 10);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_SetAnimationAndWait(8, 3);
    Actor_FaceDirection(8, 0x3000, 10);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Actor_SetAttachedEffect(ACTOR_GERALD, 0x102);
    Actor_SetAttachedEffect(ACTOR_IVAN, 0x102);
    Actor_SetAttachedEffect(ACTOR_MIA, 0x102);
    Event_Wait(40);
    Event_ShowMessageAndWait(0x2002, 0, 10);
    Actor_SetAnimationAndWait(8, 3);
    Event_OpenMessage(8, 0);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 1);
    Actor_StartRepeatedMotion(ACTOR_IVAN, 1);
    Actor_RunRepeatedMotion(ACTOR_MIA, 1);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0x6000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xa000, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(20);
        Actor_SetAttachedEffect(8, 0x102);
        Event_Wait(40);
        Event_ShowMessageAndWait(8, 0, 10);
        bump_step(2);
    } else {
        Event_Wait(20);
        Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
        bump_step(1);
        Event_ShowMessageAndWait(ACTOR_GERALD, 0, 10);
        Actor_SetAttachedEffect(8, 0x102);
        Event_Wait(40);
        Event_ShowMessageAndWait(8, 0, 10);
    }
    Actor_ShowEmote(ACTOR_MIA, 0x101, 40);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 10);
    Event_ShowMessage(ACTOR_MIA, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 20);
    Actor_SetAnimationAndWait(8, 3);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Actor_SetAttachedEffect(ACTOR_GERALD, 0x102);
    Actor_SetAttachedEffect(ACTOR_IVAN, 0x102);
    Actor_SetAttachedEffect(ACTOR_MIA, 0x102);
    Event_Wait(40);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xe000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xa000, 10);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 4);
    Event_ShowMessageAndWait(0x2002, 0, 10);
    Actor_SetAnimationAndWait(8, 3);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_RunRepeatedMotion(8, 1);
    Event_ShowMessageAndWait(8, 0, 10);
    SceneChannel_ConfigureUniformAndHandoff(10);
    Actor_SetAnimationAndWait(8, 4);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 4);
    Actor_SetAnimation(ACTOR_GERALD, 4);
    Actor_SetAnimation(ACTOR_IVAN, 4);
    Actor_SetAnimationAndWait(ACTOR_MIA, 4);
    Actor_SetAttachedEffect(8, 0x102);
    Event_Wait(60);
    Event_ShowMessageAndWait(8, 0, 10);
    ConfigureFourSceneChannelsAndHandoff(40);
    Actor_SetAnimationAndWait(8, 4);
    Event_ShowMessageAndWait(8, 0, 10);
    SceneChannel_ConfigureUniformAndHandoff(20);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Event_ShowMessageAndWait(0x2002, 0, 10);
    Actor_SetAnimationAndWait(8, 3);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_ShowEmote(ACTOR_MIA, 0x101, 60);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 10);
    Actor_ShowEmote(8, 0x108, 60);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 10);
    Actor_SetAnimationAndWait(8, 3);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 40);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 10);
    Actor_FaceDirection(8, 0x5000, 10);
    Actor_SetAnimationAndWait(8, 3);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_RunRepeatedMotion(10, 2);
    Actor_SetAnimationAndWait(10, 4);
    Event_ShowMessageAndWait(10, 0, 10);
    Actor_ShowEmote(8, 0x105, 40);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_FaceDirection(8, 0x3000, 10);
    Actor_SetAnimation(8, 4);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_RunRepeatedMotion(8, 1);
    Actor_FaceDirection(8, 0x5000, 10);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
    Event_ShowMessageAndWait(0x2002, 0, 10);
    Actor_ShowEmote(8, 0x100, 40);
    Actor_FaceDirection(8, 0x3000, 10);
    Event_ShowMessageAndWait(8, 0, 10);
    ConfigureFourSceneChannelsAndHandoff(40);
    SceneChannel_ConfigureUniformAndHandoff(20);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 10);
    Actor_SetAnimationAndWait(8, 3);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_SetAnimation(8, 4);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_IVAN, 3);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Actor_RunRepeatedMotion(8, 1);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Actor_SetAttachedEffect(ACTOR_GERALD, 0x102);
    Event_Wait(60);
    Actor_SetAnimationAndWait(8, 4);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_StartRepeatedMotion(ACTOR_IVAN, 1);
    Actor_RunRepeatedMotion(ACTOR_MIA, 1);
    Event_Wait(20);
    Actor_RunRepeatedMotion(8, 1);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_SetAnimation(8, 3);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_ShowEmote(ACTOR_IVAN, 0x105, 60);
    Event_ShowMessageAndWait(0x2002, 0, 10);
    Actor_FaceDirection(ACTOR_MIA, 0xa000, 10);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 10);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 4);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 4);
    Actor_FaceDirection(ACTOR_IVAN, 0x2000, 10);
    Actor_SetAnimation(ACTOR_IVAN, 4);
    Event_ShowMessageAndWait(0x2002, 0, 10);
    Actor_SetAnimationAndWait(8, 4);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 0);
    Actor_RunRepeatedMotion(ACTOR_MIA, 2);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 10);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 10);
    Actor_SetAnimationAndWait(8, 4);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_StartRepeatedMotion(10, 2);
    Event_ShowMessageAndWait(10, 0, 10);
    Actor_SetAnimationAndWait(11, 3);
    Event_ShowMessageAndWait(11, 0, 20);
    Actor_SetAnimationAndWait(8, 3);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 4);
    Actor_SetAnimation(ACTOR_IVAN, 4);
    Event_ShowMessageAndWait(0x2002, 0, 10);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
    ((void (*)())Engine_EventShowMessageAndWait)(1, 0, 10);
    Actor_RunRepeatedMotion(8, 1);
    Event_OpenMessage(8, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0x6000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xa000, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(20);
        SceneChannel_ConfigureUniformAndHandoff(10);
        Actor_SetAnimation(ACTOR_GERALD, 3);
        Actor_SetAnimation(ACTOR_IVAN, 3);
        Actor_SetAnimationAndWait(ACTOR_MIA, 3);
        Event_Wait(10);
        Event_ShowMessageAndWait(ACTOR_GERALD, 0, 10);
        bump_step(2);
    } else {
        Event_Wait(10);
        Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
        bump_step(1);
        Event_ShowMessageAndWait(ACTOR_GERALD, 0, 10);
        SceneChannel_ConfigureUniformAndHandoff(10);
        Actor_SetAnimation(ACTOR_GERALD, 3);
        Actor_SetAnimation(ACTOR_IVAN, 3);
        Actor_SetAnimationAndWait(ACTOR_MIA, 3);
        Event_Wait(10);
        Event_ShowMessageAndWait(ACTOR_GERALD, 0, 10);
    }
    Actor_SetAnimationAndWait(8, 3);
    Event_ShowMessageAndWait(8, 0, 40);
    Actor_FaceDirection(8, 0x5000, 10);
    Actor_RunRepeatedMotion(8, 1);
    Event_Wait(10);
    Actor_RunRepeatedMotion(10, 1);
    Event_Wait(20);
    Actor_FaceDirection(8, 0x3000, 10);
    Actor_RunRepeatedMotion(8, 1);
    Event_Wait(10);
    Actor_RunRepeatedMotion(11, 1);
    Event_Wait(20);
    Actor_SetAnimationAndWait(8, 3);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_SetAnimation(10, 3);
    Actor_SetAnimationAndWait(11, 3);
    Actor_SetSpeed(10, 0x10000, 0x8000);
    Actor_SetSpeed(11, 0x10000, 0x8000);
    Actor_WalkTo(10, 0x350, 0x21c);
    Actor_WalkToAndWait(11, 0x370, 0x21c);
    Actor_SetPosition(10, 0, 0);
    Actor_SetPosition(11, 0, 0);
    Actor_SetAnimationAndWait(8, 3);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_IVAN, 3);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Event_Wait(20);
    Actor_SetSpeed(ACTOR_GERALD, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_IVAN, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_MIA, 0x10000, 0x8000);
    tbl = (s32)KareiKyuden_PartyActions;
    Actor_EnableActionCallback(ACTOR_GERALD, tbl);
    Engine_ActorEnableActionCallback(2, tbl);
    Object_SetActionCallbackAndRefreshById(3, tbl);
    work = *(u8 **)&gEventWork;
    *(s32 *)(work + 0x1c8) = 16;
    *(s32 *)(work + 0x1c0) = 0x209;
    GameFlag_Clear(FLAG_ARRIVAL_EVENT_PENDING);
    GameFlag_Set(0x912);
    Event_End();
}

void ConfigureFourSceneChannelsAndHandoff(s32 handoff)
{
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
    ConfigureSecond(1, 0xe000, 0);
    ConfigureThird(2, 0x2000, 0);
    ConfigureFourth(3, 0xa000, 0);
    if (handoff != 0) {
        Event_Wait(handoff);
    }
}

void SceneChannel_ConfigureUniformAndHandoff(s32 handoff)
{
    ConfigureFirst(0, 0xc000, 0);
    ConfigureUniformSecond(1, 0xc000, 0);
    ConfigureUniformThird(2, 0xc000, 0);
    ConfigureUniformFourth(3, 0xc000, 0);
    if (handoff != 0) {
        Event_Wait(handoff);
    }
}

void Scene_RunPartySequence(void)
{

    s32 record;
    u8 *work;
    s32 v5;
    s32 tbl;

    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Task_Wait(1);
    Camera_MoveTo(0x3600000, -1, 0x2180000, 0);
    Map_Redraw();
    Task_Wait(1);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0x3600000, 0x2760000);
    gEventWork->start_transition = v5 = 0x100;
    gEventWork->transition_frames = 40;
    Event_OpenScreen();
    Camera_SetSpeed(0x6666, 0xccc);
    Camera_MoveTo(0x3600000, -1, 0x1d80000, 1);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    Actor_SetSpeed(ACTOR_GERALD, 0xcccc, 0x6666);
    Actor_SetSpeed(ACTOR_IVAN, 0xcccc, 0x6666);
    Actor_SetSpeed(ACTOR_MIA, 0xcccc, 0x6666);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x360, 0x1f2);
    record = Object_GetById(0);
    if (record != 0) {
        Actor_SetPosition(ACTOR_GERALD, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Object_GetById(0);
    if (record != 0) {
        Actor_SetPosition(ACTOR_IVAN, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Object_GetById(0);
    if (record != 0) {
        Actor_SetPosition(ACTOR_MIA, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Actor_WalkTo(ACTOR_PARTY_LEADER, 0x358, 0x1e6);
    Actor_WalkTo(ACTOR_GERALD, 0x350, 0x1f6);
    Actor_WalkTo(ACTOR_IVAN, 0x368, 0x1e6);
    Actor_WalkToAndWait(ACTOR_MIA, 0x370, 0x1f6);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Actor_SetAnimation(ACTOR_GERALD, 1);
    Actor_SetAnimation(ACTOR_IVAN, 1);
    Event_Wait(10);
    SceneChannel_ConfigureUniformAndHandoff(10);
    Actor_ShowEmote(9, v5, 20);
    Actor_FaceDirection(9, 0x5000, 20);
    Event_SetMessage((s32)MsgKareiItsIvanHisCompanionsPerfect);
    Event_ShowMessageAndWait(0x2009, 0, 10);
    Actor_ShowEmote(8, v5, 20);
    Actor_FaceDirection(8, 0x3000, 20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_ShowEmote(8, 0x107, 60);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Actor_SetAttachedEffect(ACTOR_GERALD, 0x102);
    Actor_SetAttachedEffect(ACTOR_IVAN, 0x102);
    Actor_SetAttachedEffect(ACTOR_MIA, 0x102);
    Event_Wait(60);
    Actor_ShowEmote(9, 0x102, 60);
    Actor_FaceDirection(9, 0x7000, 10);
    Event_ShowMessageAndWait(0x2009, 0, 10);
    Actor_FaceDirection(8, 0x1000, 10);
    Actor_ShowEmote(8, 0x108, 20);
    Event_ShowMessageAndWait(8, 0, 40);
    Actor_RunRepeatedMotion(8, 2);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_FaceDirection(8, 0x3000, 10);
    Event_OpenMessage(8, 0);
    v5 = 1;
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(10);
        Actor_SetAnimation(8, 3);
    } else {
        Event_Wait(10);
        Scene_AdvanceStep(1);
        Actor_SetAnimation(8, 4);
        v5 = 0;
    }
    Event_ShowMessageAndWait(8, 0, 10);
    if (v5 != 0) {
        Scene_AdvanceStep(1);
    }
    Actor_StartRepeatedMotion(9, 2);
    Actor_SetAttachedEffect(9, 0x102);
    Event_Wait(80);
    Event_ShowMessageAndWait(0x2009, 0, 10);
    Actor_FaceDirection(8, 0x1000, 10);
    Actor_ShowEmote(8, 0x107, 40);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 3);
    Event_ShowMessageAndWait(0x2002, 0, 20);
    Actor_ShowEmote(8, 0x105, 60);
    Actor_FaceDirection(8, 0x3000, 60);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 10);
    Actor_FaceDirection(9, 0x5000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0x6000, 10);
    Actor_StartRepeatedMotion(ACTOR_IVAN, 2);
    Event_ShowMessageAndWait(0x6002, 0, 10);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 10);
    Actor_StartRepeatedMotion(ACTOR_IVAN, 2);
    Event_ShowMessageAndWait(0x2002, 0, 20);
    Actor_SetAnimationAndWait(8, 3);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_FaceDirection(ACTOR_IVAN, 0x8000, 20);
    Event_ShowMessageAndWait(0x6002, 0, 10);
    Actor_FaceDirection(ACTOR_MIA, 0xa000, 10);
    Actor_ShowEmote(ACTOR_MIA, 0x105, 40);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 10);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Event_Wait(80);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 10);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 4);
    Event_OpenMessage(ACTOR_GERALD, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(20);
        Scene_AdvanceStep(1);
    } else {
        Event_Wait(20);
        Event_ShowMessageAndWait(ACTOR_GERALD, 0, 10);
    }
    Actor_SetAnimation(ACTOR_IVAN, 3);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    SceneChannel_ConfigureUniformAndHandoff(20);
    Actor_RunRepeatedMotion(9, 2);
    Event_ShowMessageAndWait(0x2009, 0, 10);
    Actor_RunRepeatedMotion(ACTOR_MIA, 2);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 10);
    Actor_FaceDirection(9, 0x3000, 10);
    Actor_SetAnimationAndWait(9, 3);
    Event_ShowMessageAndWait(0x2009, 0, 10);
    Actor_ShowEmote(ACTOR_IVAN, 0x101, 60);
    Event_ShowMessageAndWait(0x2002, 0, 10);
    Actor_RunRepeatedMotion(9, 1);
    Actor_FaceDirection(9, 0x5000, 10);
    Event_ShowMessageAndWait(0x2009, 0, 10);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 10);
    Actor_SetAnimationAndWait(9, 4);
    Event_ShowMessageAndWait(0x2009, 0, 10);
    Actor_RunRepeatedMotion(ACTOR_MIA, 1);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 10);
    Actor_RunRepeatedMotion(8, 1);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_IVAN, 3);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Event_Wait(20);
    tbl = (s32)KareiKyuden_PartyActions;
    Actor_EnableActionCallback(ACTOR_GERALD, tbl);
    Engine_ActorEnableActionCallback(2, tbl);
    Object_SetActionCallbackAndRefreshById(3, tbl);
    work = *(u8 **)&gEventWork;
    *(s32 *)(((s32)work + 0x1c8)) = 16;
    *(s32 *)(((s32)work + 0x1c0)) = 0x209;
    GameFlag_Clear(FLAG_ARRIVAL_EVENT_PENDING);
    GameFlag_Set(0x914);
    Event_End();
}

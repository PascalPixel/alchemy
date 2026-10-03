#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "SCENE_IDS.H"
#include "CALL.H"

s16 ArcTan2(s32, s32);

extern const struct SceneEntrance YamaRama_TempleEntrances[];
extern const struct SceneEntrance YamaRama_Entrances[];

extern const u32 YamaRama_Exits[];

extern const struct ScenePlacement YamaRama_TemplePlacements[];
extern const struct ScenePlacement YamaRama_Placements[];

extern u8 MsgYamaAdeptsLetMeThankAgain[];
extern u8 MsgYamaAmTravelingAroundWorldSpread[];
extern u8 MsgYamaDidKnowMasterHamaGreatest[];
extern u8 MsgYamaDoDoWarriorShouldReturn[];
extern u8 MsgYamaDoKnowMeditation[];
extern u8 MsgYamaHeWhoHasPowerSee[];
extern u8 MsgYamaHsuOkay[];
extern u8 MsgYamaNorthAltinMineWestLama[];
extern u8 MsgYamaRobinDidLiftBoulder[];
extern u8 MsgYamaYahhSilkRoadBouldersBlock[];
extern u8 MsgYamaYoungWarriorsDoComeFrom[];

/* Tables laid out after the code. */
extern const u16 YamaRama_BoulderCells[];
extern const u16 YamaRama_BoulderCellsBack[];
extern const u8 YamaRama_HsuAction[];
extern const u8 YamaRama_LeaderAction[];
void Object_RefreshSelectorById();
void BattleFx_PlayQueuedSound(void);
s32 SceneActor_SetFlagBitByRankAgainstActorZero(struct FieldActor *actor);
s32 OverlayObject_SetFacingTowardObject10(void *self);

/* Two sites reach this one symbol with different arities; old-style so both
 * calls are legal. */
static __inline__ void bump_step(s32 amount)
{
    gEventWork->message += amount;
}

static __inline__ void PlaceActor(s32 actor, s32 x, s32 y)
{
    void Actor_SetPosition(s32, s32, s32);

    Actor_SetPosition(actor, x, y);
}

static __inline__ void Scene_AdvanceStep(s32 amount)
{
    gEventWork->message += amount;
}

extern const struct SceneEvent YamaRama_TempleEvents[];
extern const struct SceneEvent YamaRama_Events[];

void BattleFx_SetQueuedSoundAndPlay(s32 value);
extern u8 YamaRama_ActorNineAction[];
void ConfigureAndPlaceActorFourteen(void);
void FieldScene_RunScene3a2SequenceA(void);
void ActorPresentation_PrepareActorFourteenWithCallback(void);
void FieldScene_SetSlot15Byte89AndRunStep(void);
void Scene_RunActorExchange(void);

/* Actor callbacks that open the mountain overlay. */
s32 EventScript_PrepareActorRenderFlags(struct FieldActor *actor)
{
    actor->priority_flags &= ~ACTOR_PRIORITY_AUTOMATIC;
    /* FAKEMATCH: typed priority stores cache the sprite and shorten this
       callback from 34 to 32 bytes; retain the two packed byte accesses. */
    ((u8 *)actor->sprite)[9] |= 0xc;
    ((u8 *)actor->sprite)[21] |= 0xc;
    return 0;
}

s32 OverlayObject_SetFacingTowardObject10(void *self)
{
    struct FieldActor *actor = self;
    struct FieldActor *obj;

    obj = Object_GetById(0xA);
    actor->facing = ArcTan2(obj->z.fixed - actor->z.fixed,
        obj->x.fixed - actor->x.fixed);
    return 0;
}

const struct SceneEntrance *Scene_GetEntrances(void)
{
    if (gGameState.scene == (s32)&SceneId_YamaRama1) {
        return YamaRama_TempleEntrances;
    }
    return YamaRama_Entrances;
}

/* The mountain's regions and exits, between its scene-dependent getters. */
const struct SceneRegion *Scene_GetRegions(void)
{
    return 0;
}

const u32 *Scene_GetExits(void)
{
    return YamaRama_Exits;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
    if (gGameState.scene == (s32)&SceneId_YamaRama1) {
        return YamaRama_TemplePlacements;
    }
    return YamaRama_Placements;
}

/* Scene event steps for resource_3a2. */

/* Value-returning: the reference sets r1 before r0 at this site. */

/* The scene step counter at 0x1d8 of the shared scene work record. */

/*
 * Prepare actor 14 at 0x020010b8: clear bit 1 of the bytes at +35 and +89,
 * clear the byte at +85, and install the callback at 0x02009061 in the record
 * at +0x6c -- that pool word is odd, so it is a Thumb entry and not data. The
 * zero stored at +85 is held in a local because a register carries it. The
 * callback drives the same bit-1 flag the clears here touch.
 */
void SceneDialogue_RunMessage1958Step(void)
{

    u8 *work;

    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgYamaYoungWarriorsDoComeFrom);
    Event_OpenMessage(10, 0);

    if (Engine_EventChooseYesNo(0, 0) == 1) {
        Engine_EventWait(20);
        Event_ShowMessage(10, 0);
    } else {
        work = (u8 *)gEventWork;
        *(u16 *)(work + 472) += 1;
        Event_AskYesNo(10, 0);
    }

    Engine_EventEnd();
}

void SceneDialogue_RunActor11Message195d(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgYamaDoKnowMeditation);
    Event_AskYesNo(11, 0);
    Engine_EventEnd();
}

void SceneDialogue_RunActor13Message1961(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgYamaDidKnowMasterHamaGreatest);
    Event_AskYesNo(13, 0);
    Engine_EventEnd();
}

void FieldScene_RunPrimaryScript(void)
{
    Audio_PlayCue(188);
    Map_AnimateCells(YamaRama_BoulderCells, 67, 6);
    *(u8 *)((void *)Object_GetById(0) + 85) = 0;
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 2);
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -16);
    Engine_EventWait(16);
    Engine_EventRequestExit(2);
}

void FieldScene_RunScene3a2SequenceA(void)
{
    void Event_ShowMessageAndWait();

    Engine_EventBegin();
    Actor_SetPosition(8, 0x880000, 0xa80000);
    Actor_FaceDirection(8, 0x5000, 0);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0x900000, 0xc80000);
    Actor_SetPosition(ACTOR_GERALD, 0xa00000, 0xc00000);
    Actor_SetPosition(ACTOR_IVAN, 0x800000, 0xc80000);
    Actor_SetPosition(ACTOR_MIA, 0x700000, 0xc00000);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xa000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xe000, 0);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(60);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(20);
    Engine_EventSetMessage((s32)MsgYamaAdeptsLetMeThankAgain);
    Event_ShowMessageAndWait(8, 0, 20);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(ACTOR_GERALD, 3);
    Engine_ActorSetAnimation(ACTOR_IVAN, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_MIA, 3);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(8, 4);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_MIA, 4);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Engine_ActorRunRepeatedMotion(ACTOR_IVAN, 2);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(ACTOR_IVAN, 0, 20);
    Engine_ActorRunRepeatedMotion(8, 2);
    Actor_FaceDirection(8, 0x5000, 20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_ShowEmote(ACTOR_IVAN, 0x102, 60);
    Engine_EventWait(120);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
    Engine_EventWait(20);
    Actor_FaceDirection(ACTOR_GERALD, 0x6000, 20);
    Event_OpenMessage(ACTOR_GERALD, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventWait(20);
        Actor_FaceDirection(ACTOR_MIA, 0, 20);
        Actor_ShowEmote(ACTOR_MIA, 0x101, 60);
        Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xe000, 0);
        Actor_FaceDirection(ACTOR_IVAN, 0xe000, 0);
        Engine_ActorStartRepeatedMotion(ACTOR_GERALD, 2);
        Actor_SetAttachedEffect(ACTOR_GERALD, 0x102);
        Engine_EventWait(60);
        Actor_FaceDirection(ACTOR_GERALD, 0x8000, 20);
        Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
        bump_step(2);
    } else {
        bump_step(2);
        Engine_EventWait(20);
        Actor_FaceDirection(ACTOR_MIA, 0, 20);
        Engine_ActorSetAnimationAndWait(ACTOR_MIA, 3);
        Engine_EventWait(20);
        Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
        Actor_ShowEmote(ACTOR_GERALD, 0x102, 60);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xe000, 0);
        Actor_FaceDirection(ACTOR_IVAN, 0xe000, 0);
        Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    }
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xa000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xe000, 0);
    Engine_EventWait(20);
    Actor_FaceDirection(8, 0x3000, 20);
    Event_ShowMessageAndWait(8, 0, 20);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 3);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(ACTOR_GERALD, 3);
    Engine_ActorSetAnimation(ACTOR_IVAN, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_MIA, 3);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(20);
    Actor_FaceDirection(8, 0xc000, 30);
    Audio_PlayCue(188);
    Map_AnimateCells(YamaRama_BoulderCells, 67, 6);
    Actor_SetSpeed(8, 0xcccc, 0x6666);
    Actor_WalkToAndWait(8, 136, 136);
    Actor_SetPosition(8, 0, 0);
    Audio_PlayCue(188);
    Map_AnimateCells(YamaRama_BoulderCellsBack, 67, 6);
    Engine_EventWait(60);
    BattleFx_PlayQueuedSound();
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
    Engine_EventWait(20);
    Actor_FaceDirection(ACTOR_GERALD, 0x6000, 20);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 20);
    Engine_ActorRunRepeatedMotion(ACTOR_IVAN, 2);
    Engine_EventWait(20);
    Actor_FaceDirection(ACTOR_MIA, 0x2000, 20);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 1);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xe000, 20);
    Event_AskYesNo(ACTOR_GERALD, 0);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(ACTOR_IVAN, 2);
    Engine_EventWait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 20);
    Event_ShowMessageAndWait(ACTOR_IVAN, 0, 20);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Engine_ActorStartRepeatedMotion(ACTOR_GERALD, 1);
    Engine_ActorRunRepeatedMotion(ACTOR_MIA, 1);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(ACTOR_IVAN, 4);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(ACTOR_IVAN, 0, 20);
    Engine_ActorRunRepeatedMotion(ACTOR_MIA, 2);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Actor_FaceDirection(ACTOR_IVAN, 0xb000, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_IVAN, 3);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(ACTOR_IVAN, 3);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(ACTOR_IVAN, 0, 20);
    Actor_ShowEmote(ACTOR_GERALD, 0x102, 60);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    Actor_FaceDirection(ACTOR_IVAN, 0xe000, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_IVAN, 4);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(ACTOR_IVAN, 0, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x102, 60);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Actor_FaceDirection(ACTOR_IVAN, 0xa000, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_IVAN, 3);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(ACTOR_IVAN, 0, 20);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Engine_ActorStartRepeatedMotion(ACTOR_GERALD, 1);
    Engine_ActorRunRepeatedMotion(ACTOR_MIA, 1);
    Engine_EventWait(20);
    Actor_SetSpeed(ACTOR_IVAN, 0x8000, 0x4000);
    Actor_WalkToAndWait(ACTOR_IVAN, 128, 184);
    Actor_FaceDirection(ACTOR_IVAN, 0x4000, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xa000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xe000, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_IVAN, 3);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(ACTOR_IVAN, 0, 20);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(ACTOR_GERALD, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_MIA, 3);
    Engine_EventWait(20);
    Actor_SetSpeed(ACTOR_GERALD, 0x8000, 0x4000);
    Actor_SetSpeed(ACTOR_MIA, 0x8000, 0x4000);
    Actor_WalkTo(ACTOR_GERALD, 144, 200);
    Actor_WalkTo(ACTOR_IVAN, 144, 200);
    Actor_WalkTo(ACTOR_MIA, 144, 200);
    Engine_ActorWaitForMove(ACTOR_GERALD);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    Engine_ActorWaitForMove(ACTOR_IVAN);
    Actor_SetPosition(ACTOR_IVAN, 0, 0);
    Engine_ActorWaitForMove(ACTOR_MIA);
    Actor_SetPosition(ACTOR_MIA, 0, 0);
    Engine_EventEnd();
}

void SceneDialogue_RunLine1956(void)
{
    void Engine_EventBegin(void);

    Engine_EventBegin();
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
    Engine_MessageShowCentered((s32)MsgYamaHeWhoHasPowerSee, 1);
    Engine_EventEnd();
}

void ConfigureAndPlaceActorFourteen(void)
{
    void Actor_SetPosition(s32, s32, s32);

    s32 a = 21, b = 9;
    Map_CopyCellAttributes(85, 9, 1, 1, a, b);
    MapObject_SetPosition(100, 0, 0);
    PlaceActor(14, 0x01580000, 0x00980000);
}

void FieldScene_RunScene3a2_020008a8(void)
{
    u32 i;
    s32 record;

    Map_CopyCellAttributes(21, 73, 1, 1, 21, 9);
    MapObject_SetPosition(100, -1, -1);
    Actor_SetPosition(14, 0, 0);
}

void SceneDialogue_RunActorFifteenByLeaderHeading(void)
{

    u32 heading = ((struct FieldActor *)Object_GetById(0))->facing;

    Engine_EventBegin();
    if (heading - 0xA001 <= 0x3FFE) {
        Engine_SanctumOpen(15);
    } else {
        Engine_EventSetMessage((s32)MsgYamaAmTravelingAroundWorldSpread);
        Event_ShowMessage(15, 0);
    }
    Engine_EventEnd();
}

void Scene_RunEventTransition(void)
{
    u8 *record;
    s32 none;

    if (GameFlag_IsSet(0x89a) == 0) {
    } else {
        Engine_EventBegin();
        Actor_SetPosition(10, 0x2180000, 0xd80000);
        Engine_EventSetMessage((s32)MsgYamaYahhSilkRoadBouldersBlock);
        Event_ShowMessageAndWait(10, 0, 20);
        Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
        Engine_EventWait(20);
        record = (void *)Object_GetById(0);
        *(s32 *)((s32)record + 108) = (s32)OverlayObject_SetFacingTowardObject10;
        record = (void *)Object_GetById(0);
        if ((*(s32 *)((s32)record + 16) >> 20) == 13) {
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x1b8, 200);
        }
        Actor_SetSpeed(10, 0x20000, 0x10000);
        Engine_ActorSetSpritePriority(10, 2);
        Actor_WalkToAndWait(10, 0x198, 216);
        {
            u8 *record = (void *)Object_GetById(10);
            u32 flag = 1;

            flag = flag | record[35];
            record[35] = (u8)flag;
        }
        Engine_EventWait(10);
        Actor_FaceDirection(10, 0x8000, 20);
        Event_ShowMessageAndWait(10, 0, 20);
        Engine_ActorStartRepeatedMotion(10, 2);
        Actor_SetAttachedEffect(10, 0x102);
        Engine_EventWait(60);
        Event_ShowMessageAndWait(10, 0, 20);
        Engine_ActorEnableActionCallback(10, (s32)YamaRama_HsuAction);
        Camera_MoveTo(0x1280000, -1, 0x1580000, 1);
        GameFlag_Set(0x8b0);
        Object_RefreshSelectorById(10);
        Engine_CameraWaitForMove();
        Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
        Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, YamaRama_LeaderAction);
        Object_RefreshSelectorById(0);
        Engine_EventWait(10);
        none = 0;
        record = (void *)Object_GetById(0);
        *(s32 *)((s32)record + 108) = none;
        Engine_EventWait(30);
        Engine_ActorRunRepeatedMotion(10, 2);
        Engine_EventWait(20);
        Actor_FaceDirection(10, 0x5000, 120);
        Actor_ShowEmote(10, 0x105, 60);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 60);
        Engine_ActorSetAnimationAndWait(10, 4);
        Engine_EventWait(20);
        Event_ShowMessageAndWait(10, 0, 20);
        Engine_EventEnd();
    }
}

void Scene_RunActorCue(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgYamaDoDoWarriorShouldReturn);
    Actor_ShowEmote(10, 0x105, 60);
    Event_OpenMessage(10, 0);
    if (Engine_EventChooseYesNo(0, 0) == 1) {
        Scene_AdvanceStep(1);
    }
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(10, 4);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(10, 0, 20);
    Engine_EventEnd();
}

void Scene_RunActorExchange(void)
{
    Engine_EventBegin();
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(30);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
    Engine_CameraMoveToActor(9, 1);
    Engine_CameraWaitForMove();
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(20);
    Engine_EventSetMessage((s32)MsgYamaHsuOkay);
    Event_ShowMessageAndWait(8, 0, 20);
    Engine_ActorRunRepeatedMotion(9, 1);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(9, 0, 20);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_FaceDirection(10, 0xd000, 20);
    Event_ShowMessageAndWait(10, 0, 20);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(60);
    Actor_ShowEmote(8, 0x102, 60);
    Engine_ActorSetAnimationAndWait(8, 4);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Engine_ActorStartRepeatedMotion(10, 2);
    Actor_SetAttachedEffect(10, 0x102);
    Engine_EventWait(60);
    Actor_FaceDirection(10, 0xb000, 20);
    Engine_ActorSetAnimation(9, 5);
    Engine_EventEnd();
    GameFlag_Set(0x8b1);
}

void Scene_RunActorSequence(void)
{
    void Engine_EventBegin();

    s32 mask;

    Engine_EventBegin();
    Actor_SetAttachedEffect(8, 0x102);
    Engine_ActorStartRepeatedMotion(8, 2);
    Engine_EventWait(60);
    Engine_EventSetMessage((s32)MsgYamaRobinDidLiftBoulder);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_SetAttachedEffect(10, 0x102);
    Engine_ActorJump(10, 4, 0);
    Engine_EventWait(60);
    Event_ShowMessageAndWait(10, 0, 20);
    Engine_ActorRunRepeatedMotion(8, 1);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_FaceDirection(10, 0xd000, 20);
    Engine_ActorSetAnimationAndWait(10, 3);
    Engine_EventWait(20);
    Actor_WalkTo(8, 178, 0x114);
    Actor_WalkToAndWait(10, 172, 0x11c);
    Engine_ActorWaitForMove(8);
    Actor_FaceDirection(8, 0x5000, 0);
    Actor_FaceDirection(10, 0xb000, 0);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(20);
    mask = 254;
    Event_ShowMessageAndWait(8, 0, 20);
    *(u8 *)((void *)Object_GetById(8) + 90) &= mask;
    *(u8 *)((void *)Object_GetById(10) + 90) &= mask;
    Actor_SetSpeed(8, 0x3333, 0x1999);
    Actor_SetSpeed(10, 0x3333, 0x1999);
    Engine_ActorSetAnimation(8, 5);
    Engine_ActorSetAnimation(10, 6);
    Engine_EventWait(20);
    Audio_PlayCue(125);
    Actor_SetDestinationOffset(8, 2, 0);
    Actor_SetDestinationOffset(9, 2, 0);
    Actor_SetDestinationOffset(10, 2, 0);
    Engine_ActorWaitForMove(10);
    Engine_EventWait(30);
    Engine_ActorSetAnimation(8, 5);
    Engine_ActorSetAnimation(10, 6);
    Engine_EventWait(20);
    Audio_PlayCue(125);
    Actor_SetDestinationOffset(8, 4, 0);
    Actor_SetDestinationOffset(9, 4, 0);
    Actor_SetDestinationOffset(10, 4, 0);
    Engine_ActorWaitForMove(10);
    Engine_ActorStop(9);
    Engine_ActorSetAnimation(8, 1);
    Engine_ActorSetAnimation(10, 1);
    Engine_EventWait(50);
    Engine_ActorJump(10, 2, 0);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(10, 0, 20);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(20);
    Event_AskYesNo(8, 0);
    Event_ShowMessageAndWait(8, 0, 20);
    Engine_ActorSetAnimation(8, 5);
    Engine_ActorSetAnimation(10, 6);
    Engine_EventWait(20);
    Audio_PlayCue(125);
    Actor_SetDestinationOffset(8, 2, 0);
    Actor_SetDestinationOffset(9, 2, 0);
    Actor_SetDestinationOffset(10, 2, 0);
    Engine_ActorWaitForMove(10);
    Engine_EventWait(30);
    Engine_ActorSetAnimation(8, 5);
    Engine_ActorSetAnimation(10, 6);
    Engine_EventWait(20);
    Audio_PlayCue(125);
    Actor_SetDestinationOffset(8, 4, 0);
    Actor_SetDestinationOffset(9, 4, 0);
    Actor_SetDestinationOffset(10, 4, 0);
    Engine_ActorWaitForMove(10);
    Engine_EventWait(40);
    Engine_ActorSetAnimation(8, 1);
    Engine_ActorSetAnimation(10, 1);
    Engine_ActorJump(10, 2, 0);
    Engine_EventWait(20);
    Actor_FaceDirection(10, 0xd000, 20);
    Event_ShowMessageAndWait(10, 0, 20);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(8, 0, 30);
    Event_ShowMessageAndWait(8, 0, 20);
    {
        u8 *record = (void *)Object_GetById(10);
        u32 flag = 1;

        flag = flag | record[90];
        record[90] = (u8)flag;
    }
    Actor_SetSpeed(10, 0xcccc, 0x6666);
    Actor_WalkToAndWait(10, 168, 0x128);
    Actor_FaceDirection(10, 0xd000, 20);
    Engine_ActorSetAnimation(10, 5);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Engine_ActorSetAnimationAndWait(10, 3);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(10, 0, 20);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Engine_EventWait(20);
    Engine_EventEnd();
    GameFlag_Set(0x8b2);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
    Engine_EventRequestExit(6);
}

void FieldScene_RunScriptedSteps0And1A12(void)
{
    Engine_EventBegin();
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
    Engine_MessageShowCentered((s32)MsgYamaNorthAltinMineWestLama, 1);
    Engine_EventEnd();
}

void FieldScene_RunPairedLayoutStepsThenSetOne(void)
{
    {
        s32 fifth = 1;
        s32 sixth = 2;

        Map_CopyCellsTo(5, 28, 5, 13, fifth, sixth);
    }
    {
        s32 fifth = 5;
        s32 sixth = 13;

        Map_CopyCellAttributes(5, 28, 1, 2, fifth, sixth);
    }
    Engine_EventWait(1);
}

void SceneState_RunRect6x28Step(void)
{
    {
        s32 fifth = 1;
        s32 sixth = 2;

        Map_CopyCellsTo(6, 28, 5, 13, fifth, sixth);
    }
    {
        s32 fifth = 5;
        s32 sixth = 13;

        Map_CopyCellAttributes(6, 28, 1, 2, fifth, sixth);
    }
    Engine_EventWait(1);
}

s32 SceneActor_SetFlagBitByRankAgainstActorZero(struct FieldActor *actor)
{
    if (((struct FieldActor *)Object_GetById(0))->y.fixed > actor->y.fixed) {
        actor->priority_flags |= 2;
    } else {
        actor->priority_flags &= 0xFD;
    }
}

void SceneActor_UpdateActorFourteenByDepth(void)
{
    struct FieldActor *current = (void *)Object_GetById(0);
    struct FieldActor *other = (void *)Object_GetById(14);

    if (current->z.fixed <= other->z.fixed) {
        Engine_ActorSetSpritePriority(14, 1);
    }
}

void ActorPresentation_PrepareActorFourteenWithCallback(void)
{
    u8 zero;

    zero = 0;
    Engine_EventBegin();

    ((u8 *)Object_GetById(14))[35] &= 0xfd;
    ((u8 *)Object_GetById(14))[89] &= 0xfd;
    ((u8 *)Object_GetById(14))[85] = zero;
    *(void **)((void *)Object_GetById(14) + 108) = (void *)SceneActor_SetFlagBitByRankAgainstActorZero;

    Map_CopyCellAttributes(55, 16, 1, 1, 56, 18);
    Map_CopyCellAttributes(55, 16, 1, 1, 20, 18);

    Engine_TaskWait(1);
    GameFlag_Set(512);
    Engine_ActorSetSpritePriority(14, 2);
    Engine_EventEnd();
}

void FieldScene_SetSlot15Byte89AndRunStep(void)
{
    u8 *slot;

    Engine_EventBegin();
    {
        s32 fifth = 21;
        s32 sixth = 11;

        Map_CopyCellAttributes(14, 6, 1, 2, fifth, sixth);
    }
    slot = (void *)Object_GetById(15) + 89;
    *slot = 254;
    GameFlag_Set(0x201);
    Engine_EventEnd();
}

const struct SceneEvent *Scene_GetEvents(void)
{
    if (gGameState.scene == (s32)&SceneId_YamaRama1) {
        return YamaRama_TempleEvents;
    }
    return YamaRama_Events;
}

/* Mountain entry: set the entrance selector; in the second area stage the actors and the entrance scene, in the first restore the exchange scene's actors and cells from its story flags. */
s32 YamaRama_ApplyEntryState(void)
{
    s32 entrance;

    gEventWork->start_transition = 0x100;
    if (gGameState.scene == (s32)&SceneId_YamaRama2) {
        BattleFx_SetQueuedSoundAndPlay(169);
        Engine_ActorSetAnimation(11, 5);
        Engine_ActorSetAnimation(12, 5);
        Engine_ActorSetAnimation(14, 2);
        Call6(Engine_MapCopyCellAttributes, 21, 9, 1, 1, 21, 73);
        ConfigureAndPlaceActorFourteen();
        if (Engine_GameFlagIsSet(0x8b2)) {
            Call3(Engine_ActorSetPosition, 13, 0x880000, 0x1000000);
            Engine_ActorFaceDirection(13, 0, 0);
        }
        entrance = gGameState.entrance;
        if (entrance == 2) {
            Engine_GameFlagClear(0x12f);
        } else if (entrance == 3 && !Engine_GameFlagIsSet(0x109)) {
            FieldScene_RunScene3a2SequenceA();
        }
    } else if (gGameState.scene == (s32)&SceneId_YamaRama1) {
        Engine_ActorSetSpriteFlags((struct FieldActor *)Object_GetById(14), 0);
        ((struct FieldActor *)Object_GetById(14))->priority_flags |= 2;
        if (Engine_GameFlagIsSet(0x200)) {
            Engine_ActorSetAnimation(14, 5);
            ActorPresentation_PrepareActorFourteenWithCallback();
        }
        if (Engine_GameFlagIsSet(0x201)) {
            Engine_ActorSetAnimation(15, 4);
            FieldScene_SetSlot15Byte89AndRunStep();
        }
        if (gGameState.entrance == 4 || gGameState.entrance == 5) {
            Engine_GameFlagClear(0x12f);
        }
        if (!Engine_GameFlagIsSet(0x89a) && !Engine_GameFlagIsSet(0x895) && !Engine_GameFlagIsSet(0x8b2)) {
            Engine_ActorSetPosition(10, 0, 0);
        }
        if (!Value1(Engine_GameFlagIsSet, 0x8b2) && Engine_GameFlagIsSet(0x895) && gGameState.entrance == 2) {
            Engine_ActorSetPosition(11, 0, 0);
            Engine_GameFlagSet(0x8b2);
            Engine_GameFlagSet(0x8b3);
            Engine_ActorSetPosition(10, 0, 0);
        }
        if (Value1(Engine_GameFlagIsSet, 0x8b2)) {
            Engine_MapCopyCellsTo(54, 21, 53, 21, 1, 2);
            Engine_MapCopyCellAttributes(18, 20, 1, 3, 17, 21);
            Engine_MapCopyCellsTo(44, 18, 43, 17, 1, 1);
            Engine_MapCopyCellAttributes(8, 17, 1, 1, 7, 17);
        }
        if (Engine_GameFlagIsSet(0x895) && !Engine_GameFlagIsSet(0x8b2)) {
            Engine_ActorSetPosition(12, 0, 0);
            Engine_ActorSetPosition(13, 0, 0);
            Call3(Engine_ActorSetPosition, 8, 0xc00000, 0x1080000);
            Call3(Engine_ActorSetPosition, 9, 0xa40000, 0x1180000);
            Call3(Engine_ActorSetPosition, 10, 0xb80000, 0x1300000);
            Call3(Engine_ActorFaceDirection, 8, 0x5000, 0);
            Call3(Engine_ActorFaceDirection, 10, 0xb000, 0);
            Engine_ActorEnableActionCallback(9, YamaRama_ActorNineAction);
            ((struct FieldActor *)Object_GetById(9))->scale_x = -0x10000;
        }
        if (!Value1(Engine_GameFlagIsSet, 0x8b2)) {
            Call3(Engine_ActorSetPosition, 9, 0xa40000, 0x1180000);
            Engine_ActorEnableActionCallback(9, YamaRama_ActorNineAction);
            ((struct FieldActor *)Object_GetById(9))->scale_x = -0x10000;
        }
        if (gGameState.entrance == 5 && !Engine_GameFlagIsSet(0x8b1) && !Engine_GameFlagIsSet(0x109) && !Engine_GameFlagIsSet(0x8b2)) {
            Scene_RunActorExchange();
        }
    }
    return 0;
}

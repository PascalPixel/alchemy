#include "GATE.H"
#include "FIELD_SCENE.H"
/* The Suhara gate's scene start: once flag 0x89f is set the gate returns
 * the party to Lunpa's Suhara side at entrance 10; the first scene sets the
 * retreat point and the passage cells by the entrance used, and the second
 * poses the gate crew. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "SCENE_IDS.H"

extern const struct SceneEntrance gSuharaGateEntrances2[];
extern const struct SceneEntrance gSuharaGateEntrances3[];
extern const struct SceneEntrance gSuharaGateEntrancesOther[];

extern u8 SuharaGate_SceneTable[];

extern const struct ScenePlacement gSuharaGatePlacements1[];
extern const struct ScenePlacement gSuharaGatePlacements1Flagged[];
extern const struct ScenePlacement gSuharaGatePlacements2[];
extern const struct ScenePlacement gSuharaGatePlacementsOther[];

extern const struct SceneEvent gSuharaGateEvents2[];
extern const struct SceneEvent gSuharaGateEvents3[];
extern const struct SceneEvent gSuharaGateEventsOther[];

extern u8 MsgFieldVenusLighthouseWasAttackedBy[];
extern u8 MsgSuharaMeaning[];
extern u8 MsgSuharaPityColossoVictor[];
extern u8 MsgSuharaWantGoBabi[];

void Scene_SetActor13Value1A(void)
{
    BattleFx_SetPhaseRequest(0xD, 0x1A);
}

/* Where the party appears on each side of the gate. */
const struct SceneEntrance *Scene_GetEntrances(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_SuharaGate2) {
        return gSuharaGateEntrances2;
    }
    if (scene == (s32)&SceneId_SuharaGate3) {
        return gSuharaGateEntrances3;
    }
    return gSuharaGateEntrancesOther;
}

s32 SceneData_ReturnZero(void)
{
    extern s16 gCell[];

    return 0;
}

u8 *SceneData_GetSceneTable(void)
{
    return SuharaGate_SceneTable;
}

/* The actors placed at the gate; the first row places others once flag
 * 0x96f is set. */
const struct ScenePlacement *Scene_GetPlacements(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_SuharaGate2) {
        return gSuharaGatePlacements2;
    }
    if (scene == (s32)&SceneId_SuharaGate1) {
        if (GameFlag_IsSet(0x96f) != 0) {
            return gSuharaGatePlacements1Flagged;
        }
        return gSuharaGatePlacements1;
    }
    return gSuharaGatePlacementsOther;
}

/* What each side of the gate answers. */
const struct SceneEvent *Scene_GetEvents(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_SuharaGate2) {
        return gSuharaGateEvents2;
    }
    if (scene == (s32)&SceneId_SuharaGate3) {
        return gSuharaGateEvents3;
    }
    return gSuharaGateEventsOther;
}

s32 SuharaGate_EnterScene(void)
{
    s32 scene;

    if (Engine_GameFlagIsSet(0x89f)) {
        gGameState.saved_scene = (s32)&SceneId_RunpaSuhara;
        gGameState.saved_entrance = 10;
    }
    scene = gGameState.scene;
    if (scene == (s32)&SceneId_SuharaGate1) {
        if (Engine_GameFlagIsSet(0x897))
            Engine_ActorSetPosition(10, 0, 0);
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
        else if (Engine_GameFlagIsSet(0x96f))
            Map_CopyCellAttributes(0, 0, 2, 1, 29, 18);
        if (Engine_GameFlagIsSet(0x96f)) {
            Map_CopyCellAttributes(0, 0, 2, 1, 8, 25);
            Map_CopyCellAttributes(0, 0, 2, 1, 9, 26);
        }
#endif
        if (gGameState.entrance == 3) {
            if (Engine_GameFlagIsSet(0x8fb)) {
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
                gGameState.retreat_scene = (s32)&SceneId_SuharaGate1;
#else
                gGameState.retreat_scene = scene;
#endif
                gGameState.retreat_entrance = 1;
            }
            if (Engine_GameFlagIsSet(0x8fc)) {
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
                gGameState.retreat_scene = (s32)&SceneId_SuharaGate1;
#else
                gGameState.retreat_scene = scene;
#endif
                gGameState.retreat_entrance = 5;
            }
            Engine_GameFlagClear(0x12f);
        }
        if (gGameState.entrance == 1) {
            Engine_GameFlagSet(0x8fb);
            if (!Engine_GameFlagIsSet(0x96f))
                Map_CopyCellAttributes(6, 0, 2, 1, 8, 27);
        }
        if (gGameState.entrance == 5)
            Engine_GameFlagSet(0x8fc);
    } else if (scene == (s32)&SceneId_SuharaGate2) {
        Engine_ActorSetAnimation(8, 4);
        Engine_ActorSetAnimation(9, 4);
        Engine_ActorSetAnimation(10, 3);
        Engine_ActorSetAnimation(11, 4);
        Engine_ActorSetAnimation(12, 3);
        Object_GetById(15)->scale_y = 0x19999;
        Map_CopyCellAttributes(108, 38, 1, 1, 102, 56);
    }
    return 0;
}

void Scene_RunScene3c3SequenceC(void)
{
    extern u8 gCell[];
    extern u8 gWork[];

    s32 arg;
    s32 v5;

    arg = *(s16 *)(*(u8 **)gWork + 0x16c);
    *(u8 *)((s32)Actor_Get(0) + 85) = 0;
    v5 = 2;
    Audio_PlayCue(158);
    Map_CopyCellsTo(66, 36, 71, 8, v5, v5);
    Engine_TaskWait(4);
    Map_CopyCellsTo(68, 36, 71, 8, v5, v5);
    Engine_TaskWait(4);
    Actor_CenterAndWalk(0, 3, -16);
    Engine_EventRequestExit(arg);
}

void Dialogue_ShowMessages8fbAnd8fc(void)
{
    extern u8 *gWork;

    s16 token = *(s16 *)(gWork + 364);

    Audio_PlayCue(123);
    GameFlag_Clear(0x8FB);
    GameFlag_Clear(0x8FC);
    Engine_EventRequestExit(token);
}

void Scene_RunPrimarySequence(void)
{
    Engine_EventBegin();
    Actor_SetSpeed(8, 65536, 32768);
    Actor_SetSpeed(9, 65536, 32768);
    Actor_WalkTo(8, 136, 384);
    Actor_WalkToAndWait(9, 152, 384);
    Actor_FaceDirection(8, 16384, 0);
    Actor_FaceDirection(9, 16384, 0);
    Engine_ActorSetAnimation(8, 1);
    Map_CopyCellAttributes(6, 27, 1, 1, 7, 27);
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
    Map_CopyCellAttributes(11, 26, 1, 1, 7, 26);
    Map_CopyCellAttributes(11, 26, 1, 1, 8, 26);
#else
    Map_CopyCellAttributes(9, 26, 2, 1, 7, 26);
#endif
    Engine_EventEnd();
}

void Scene_RunScene3c3SequenceA(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;
    s32 v5;

    Engine_EventBegin();
    Actor_SetSpeed(0, 0x19999, 0xcccc);
    Actor_WalkToAndWait(0, 120, 0x1b6);
    Actor_FaceDirection(0, 0xc000, 0);
    record = Actor_Get(0);
    if (record != 0) {
        Actor_SetPosition(11, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Engine_TaskWait(1);
    Actor_SetSpeed(11, 0x19999, 0xcccc);
    Actor_WalkToAndWait(11, 108, 0x1af);
    Actor_FaceDirection(11, 0xd000, 10);
    Actor_ShowEmote(11, 0x100, 20);
    Actor_FaceDirection(11, 0xd000, 20);
    Actor_FaceDirection(11, 0, 40);
    Actor_FaceDirection(11, 0xd000, 40);
    Actor_FaceDirection(11, 0, 20);
    Engine_ActorStartRepeatedMotion(11, 2);
    Engine_EventSetMessage((s32)MsgSuharaMeaning);
    Event_ShowMessageAndWait(11, 0, 40);
    Actor_ShowEmote(8, 0x100, 0);
    Engine_ActorRunRepeatedMotion(8, 2);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_WalkToAndWait(11, 132, 0x1a4);
    Actor_FaceDirection(11, 0xd000, 0);
    Actor_FaceDirection(0, 0xe000, 0);
    Actor_WalkToAndWait(11, 138, 0x1a0);
    Actor_FaceDirection(11, 0xb000, 10);
    Engine_ActorStartRepeatedMotion(11, 2);
    Event_ShowMessageAndWait(11, 0, 40);
    Engine_ActorRunRepeatedMotion(8, 2);
    Event_ShowMessageAndWait(8, 0, 40);
    Actor_ShowEmote(9, 0x100, 20);
    Engine_ActorRunRepeatedMotion(9, 2);
    Event_ShowMessageAndWait(9, 0, 20);
    Actor_FaceDirection(0, 0xc000, 0);
    Actor_WalkToAndWait(11, 144, 0x1a4);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(9, 2);
    Event_ShowMessageAndWait(9, 0, 20);
    Actor_SetAttachedEffect(9, 0x102);
    Engine_ActorStartRepeatedMotion(9, 3);
    Event_ShowMessageAndWait(9, 0, 20);
    Actor_FaceDirection(11, 0x5000, 20);
    Event_AskYesNo(11, 0);
    if (GameFlag_IsSet(0x9b0) != 0) {
        Actor_FaceDirection(11, 0xd000, 40);
        Actor_SetAttachedEffect(11, 0x102);
        Engine_EventWait(40);
        Event_ShowMessageAndWait(11, 0, 10);
    } else {
        *(u16 *)((u8 *)gEventWork + 0x1d8) += 1;
    }
    Actor_FaceDirection(11, 0x5000, 10);
    Event_ShowMessageAndWait(11, 0, 40);
    Actor_ShowEmote(11, 0x100, 40);
    Actor_FaceDirection(11, 0xb000, 10);
    Event_ShowMessageAndWait(11, 0, 10);
    Actor_WalkToAndWait(11, 138, 0x1a0);
    Actor_FaceDirection(11, 0xb000, 20);
    Engine_ActorStartRepeatedMotion(8, 2);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_SetAttachedEffect(11, 0x102);
    Engine_ActorRunRepeatedMotion(11, 1);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(11, 0, 20);
    Engine_ActorRunRepeatedMotion(9, 2);
    Engine_EventWait(20);
    Actor_FaceDirection(0, 0xe000, 10);
    Engine_ActorRunRepeatedMotion(9, 1);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(9, 0, 10);
    Engine_ActorRunRepeatedMotion(8, 2);
    Event_ShowMessageAndWait(8, 0, 20);
    Engine_ActorRunRepeatedMotion(11, 1);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(11, 0, 20);
    Actor_FaceDirection(11, 0x5000, 10);
    Event_ShowMessageAndWait(11, 0, 10);
    Engine_ActorSetAnimation(11, 2);
    record = Actor_Get(0);
    if (record != 0) {
        Actor_SetDestination(11, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Engine_ActorWaitForMove(11);
    Actor_SetPosition(11, 0, 0);
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
    Map_CopyCellAttributes(6, 27, 1, 1, 7, 27);
    Map_CopyCellAttributes(11, 26, 1, 1, 7, 26);
    Map_CopyCellAttributes(11, 26, 1, 1, 8, 26);
#else
    v5 = 7;
    Map_CopyCellAttributes(6, 27, 1, 1, v5, 27);
    Map_CopyCellAttributes(9, 26, 2, 1, v5, 26);
#endif
    GameFlag_Set(0x89f);
    Engine_EventEnd();
}

/*
 * Dialogue bracket at 0x02000730.  The 124-byte owner includes its four-word
 * literal pool at 0x0200079c-0x020007ab.  The tail is shared through a label
 * rather than copied into each arm: copying it would add a fourth call site
 * where there are three.  The skip-beat counter sits at byte offset 472 off
 * the gWork pointer cell, which costs one dereference.  The guard is
 * tested against zero at this site; the polarity is read per call site.
 */
void Scene_RunActorNinePromptDialogue(void)
{
    extern u8 *gWork;

    u8 *work;

    Engine_EventBegin();

    if (GameFlag_IsSet(0x89f) != 0) {
        Engine_EventSetMessage((s32)MsgSuharaPityColossoVictor);
        goto close;
    }

    Engine_EventSetMessage((s32)MsgSuharaWantGoBabi);
    {
        s32 mode = 0;
        s32 no = 9;

        Event_OpenMessage(no, mode);
    }

    if (Engine_EventChooseYesNo(0, 0) != 0) {
        goto skip;
    }

    Event_ShowMessage(9, 0);
    Engine_ActorSetAnimationAndWait(9, 4);

close:
    Event_ShowMessage(9, 0);
    goto done;

skip:
    /* Skip-beat counter, two beats' worth. */
    work = gWork;
    *(u16 *)(work + 472) += 2;
    Event_ShowMessage(9, 0);

done:
    Engine_EventEnd();
}

void Scene_RunActorTenRepeatedMotion(void)
{
    extern u8 *gWork;

    unsigned int beat;

    Engine_EventBegin();

    Engine_EventSetMessage((s32)MsgFieldVenusLighthouseWasAttackedBy);
    Event_ShowMessageAndWait(10, 0, 10);

    beat = 0;
    do {
        Actor_SetChildValue(10, 0);
        Engine_ActorSetSpriteFlags(Actor_Get(10), 1);
        Engine_TaskWait(4);

        Actor_SetChildValue(10, 15);
        Engine_ActorSetSpriteFlags(Actor_Get(10), 0);
        beat++;
        Engine_TaskWait(4);
    } while (beat <= 5);

    beat = 0;
    do {
        Actor_SetChildValue(10, 0);
        Engine_ActorSetSpriteFlags(Actor_Get(10), 1);
        Engine_TaskWait(2);

        Actor_SetChildValue(10, 15);
        Engine_ActorSetSpriteFlags(Actor_Get(10), 0);
        beat++;
        Engine_TaskWait(2);
    } while (beat <= 11);

    Actor_SetPosition(10, 0, 0);

    GameFlag_Set(0x897);

#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
    Map_CopyCellAttributes(29, 19, 2, 1, 29, 18);
#endif
    Engine_EventEnd();
}

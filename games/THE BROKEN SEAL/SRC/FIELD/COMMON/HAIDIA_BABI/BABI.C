#include "FXBLEND.H"
#include "MAP_SCROLL.H"
#include "HAIDIA_BABI.H"
#include "RAM_BUFFER.H"

s32 ArcTan2(s32, s32);
s32 FieldScene_PrepareActors(s32);

extern u8 MsgHaidiaFolksSeemKnow[];

extern u8 MsgHaidiaHeyBoy[];
extern u8 MsgHaidiaTheMaskedManWasGarcia[];
void BattleFx_SetBlock30ValuesMaxZero(void);

void Map_ClearLayerEntryFlag();
void FieldScene_RunPaletteRampSequence();
void FieldScene_RunComplexActorSequence();
void BattleFx_StartTwelveFrameBlend();
void BattleFx_SetBlock30Values12Zero();
void BattleFx_SetBlock30Values128One();

/* The facing controller and the scene hooks the entry veneers export. */
s32 SceneActor_UpdateFacingTowardTarget(struct FacingObject *object)
{
    s32 delta;
    u16 old;
    s32 tgt;
    struct FacingObject *target;

    target = object->facing_target;
    if (target != NULL) {
        object->facing_flags = (u8)(0xFE & object->facing_flags);
        tgt = (u16)ArcTan2(target->position_z - object->position_z, target->position_x - object->position_x);
        old = object->facing;
        delta = (s16)(tgt - old);
        if (delta != 0) {
            if (delta > 0x1000) {
                delta = 0x1000;
            }
            /* The loader relocates the stored pool word to -0x1000. */
            if (delta < -0x1000) {
                delta = -0x1000;
            }
            object->facing = (u16)(old + delta);
        }
    }
    return 1;
}

s32 *HaidiaBabi_GetEntrances(void)
{
    return gHaidiaBabiEntrances;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

s32 HaidiaBabi_SelectExits(void)
{
    if (GameFlag_IsSet(0x834) != 0) {
        return (s32)gHaidiaBabiExits2;
    }
    return (s32)gHaidiaBabiExits;
}

s32 HaidiaBabi_SelectPlacements(void)
{
    u8 *b = (u8 *)&gGameState;
    s32 *tbl;

    if (*(s16 *)(b + 0x1c2) == 19)
        return (s32)gHaidiaBabiPlacements4;
    if (GameFlag_IsSet(0x87a) != 0)
        tbl = gHaidiaBabiPlacements3;
    else if (GameFlag_IsSet(FLAG_PARTY_LEFT_VALE) != 0)
        tbl = gHaidiaBabiPlacements2;
    else
        tbl = gHaidiaBabiPlacements;
    FieldScene_PrepareActors((s32)tbl);
    return (s32)tbl;
}

/* Asks whether the party knows Kraden, with a line for each answer. */
void HaidiaBabi_AskAboutKraden(s32 object)
{
    s32 msg = (s32)MsgHaidiaFolksSeemKnow;

    Engine_EventSetMessage(msg);
    Event_OpenMessage(object, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventWait(10);
        Engine_EventSetMessage(msg + 1);
    } else {
        Engine_EventSetMessage(msg + 2);
    }
    Event_ShowMessage(object, 0);
}

/* The events hook, the villagers' scenes and the house's exits. */
s32 HaidiaBabi_SelectEvents(void)
{
    if (gGameState.entrance == 19) {
        if (GameFlag_IsSet(0x950) != 0) {
            return (s32)gHaidiaBabiEvents6;
        }
        return (s32)gHaidiaBabiEvents5;
    }

    if (GameFlag_IsSet(0x834) != 0) {
        return (s32)gHaidiaBabiEvents4;
    }
    if (GameFlag_IsSet(0x87A) != 0) {
        return (s32)gHaidiaBabiEvents3;
    }
    if (GameFlag_IsSet(FLAG_PARTY_LEFT_VALE) != 0) {
        return (s32)gHaidiaBabiEvents2;
    }
    return (s32)gHaidiaBabiEvents;
}

void HaidiaBabi_RunHeyBoyScene(void)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    Engine_ActorStartRepeatedMotion(16, 2);
    Engine_EventWait(30);
    Engine_EventSetMessage((s32)MsgHaidiaHeyBoy);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 16, 10);
    Event_ShowMessageAndWait(16, 0, 6);
    Actor_ShowEmote(16, 0x102, 0);
    Engine_ActorStartRepeatedMotion(16, 1);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(16, 4);
    Engine_EventWait(20);
    Event_OpenMessage(16, 0);
    if (Engine_EventChooseYesNo(0, 0) == 1) {
        bump_step(1);
    }
    Engine_ActorStartRepeatedMotion(16, 1);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(16, 0, 4);
    Engine_EventEnd();
}

void SceneDialogue_RunActorFourteenDialogue11AA(void)
{
    void *work;

    Engine_EventBegin();
    Actor_FaceActor(0xE, ACTOR_PARTY_LEADER, 0xA);
    Engine_EventSetMessage((s32)MsgHaidiaTheMaskedManWasGarcia);
    Event_OpenMessage(0xE, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Event_ShowMessage(0xE, 0);
    } else {
        work = *(void **)&gEventWork;
        FIELD_AT_OFFSET(work, u16 *, 0x1D8) = (u16)(FIELD_AT_OFFSET(work, u16 *, 0x1D8) + 1);
        Event_AskYesNo(0xE, 0);
    }
    Engine_EventEnd();
}

void SceneState_SetWork448To521AndRun(s32 object)
{
    if (GameFlag_IsSet(0x834) != 0) {
        BattleFx_SetBlock30ValuesMaxZero();
    }
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventRequestExit(object);
}

void SceneState_SetValue123Mode1(void)
{
    Audio_PlayCue(0x7B);
    SceneState_SetWork448To521AndRun(1);
}

void FieldScene_RunStep7BThen2(void)
{
    Audio_PlayCue(0x7B);
    SceneState_SetWork448To521AndRun(2);
}

void SceneState_SetValue123Mode3(void)
{
    Audio_PlayCue(0x7B);
    SceneState_SetWork448To521AndRun(3);
}

void FieldScene_RunStep7BThen4(void)
{
    Audio_PlayCue(0x7B);
    SceneState_SetWork448To521AndRun(4);
}

void FieldScene_RunStep80Then5(void)
{
    Audio_PlayCue(0x80);
    SceneState_SetWork448To521AndRun(5);
}

void FieldScene_RunStep7BThen6(void)
{
    Audio_PlayCue(0x7B);
    SceneState_SetWork448To521AndRun(6);
}

void FieldScene_RunStep80Then7(void)
{
    Audio_PlayCue(0x80);
    SceneState_SetWork448To521AndRun(7);
}

void SceneState_SetValue129Mode8(void)
{
    Audio_PlayCue(0x81);
    SceneState_SetWork448To521AndRun(8);
}

void SceneState_SetValue129Mode9(void)
{
    Audio_PlayCue(0x81);
    SceneState_SetWork448To521AndRun(9);
}

void FieldScene_RunStep7BThen10(void)
{
    Audio_PlayCue(0x7B);
    SceneState_SetWork448To521AndRun(10);
}

void SceneState_ApplyValues123And11(void)
{
    Audio_PlayCue(0x7B);
    SceneState_SetWork448To521AndRun(11);
}

/* The entry hook: how the house is set up for the entrance and the story. */
s32 HaidiaBabi_RestoreEntryState(void)
{
    /* FAKEMATCH: retain the cached-cell base shared at +12 and the word
     * value before the narrow target store. */
    u32 i;
    s32 record;
    struct EventWork **control;

    if (gGameState.entrance == 19) {
        Engine_GameFlagClear(0x12f);
        *(s32 *)((*(s32 *)&gEventWork + 0x1c0)) = 0x209;
    } else {
        if (Engine_GameFlagIsSet(0x834) != 0) {
            Engine_ActorSetPosition(11, 0, 0);
            Engine_ActorSetPosition(12, 0, 0);
            Engine_ActorSetPosition(13, 0, 0);
            Engine_ActorSetPosition(14, 0, 0);
            Engine_ActorSetPosition(15, 0, 0);
            Engine_ActorSetPosition(16, 0, 0);
        } else {
            ActorPresentation_SetTwoSceneCells();
        }
        Engine_ActorSetSpritePriority(13, 1);
        if (Engine_GameFlagIsSet(0x87a) != 0) {
            record = (s32)Object_GetById(17);
            Engine_ActorSetSpriteFlags((struct FieldActor *)record, 0);
            if (gGameState.entrance != 6 && gGameState.entrance != 7) {
                goto L_02000550;
            }
            if (Engine_GameFlagIsSet(0x109) != 0) {
                record = Engine_GameFlagIsSet(0x203);
                if (record == 0) {
                    goto L_02000550;
                }
                Map_ClearLayerEntryFlag(12);
                goto L_02000550;
            }
            Map_ClearLayerEntryFlag(11);
            record = (s32)Object_GetById(8);
            Engine_ActorSetSpriteFlags((struct FieldActor *)record, 0);
            Engine_ActorSetAnimation(8, 10);
        } else {
            if (gGameState.entrance == 21) {
                FieldScene_RunPaletteRampSequence();
            } else {
                if (gGameState.entrance == 20) {
                    Engine_GameFlagSet(0x834);
                    FieldScene_RunComplexActorSequence();
                } else {
                    if (gGameState.entrance == 22) {
                        FieldScene_RunSupplementalSequenceOne();
                    } else {
                        control = &gEventWork;
                        (*control)->start_transition = 0x209;
                        if (Engine_GameFlagIsSet(0x834) != 0) {
                            BattleFx_StartTwelveFrameBlend();
                            {
                                struct FieldBlendWork *blend = *(struct FieldBlendWork **)((u8 *)control + 12);
                                u16 *target = (u16 *)&blend->loud;
                                s32 shown = 1;

                                *target = shown;
                            }
                            BattleFx_SetBlock30Values12Zero();
                            Engine_TaskWait(30);
                            Engine_EventOpenScreen();
                            Engine_EventWaitForScreen();
                            BattleFx_SetBlock30Values128One();
                        } else {
                            Engine_MapRedraw();
                            Engine_TaskWait(1);
                        }
                    }
                }
            }
        }
    }
    L_02000550:;
    return 0;
}

extern u8 MsgHaidiaWake[];

extern const u8 gHaidiaBabiSharedAction[];
extern const u8 gHaidiaBabiActorExitAction[];
extern const u8 gHaidiaBabiLeaderExitAction[];

struct SceneHalf {
    u16 value;
};

void FieldScene_RunSixStepSequence17e4();
void Engine_TaskWait();
void Engine_MapRedraw();
void Map_ClearLayerEntryFlag();
void Map_SetLayerEntryFlag();
void Engine_ActorSetSpriteFlags(struct FieldActor *actor, s32 flags);
void Graphics_EnableObjLayerAndCallbacks();
void ObjectDispatch_StopCallbacksAndHideLayers();
void ObjectDispatch_RegisterChildMetadata(struct FieldActor *actor, s32 palette);
void UiText_ShowCenteredMessage(s32 message, s32 mode, s32 y_offset);
void Engine_EventWait();
void Engine_EventBegin();
s32 Engine_EventChooseYesNo();
void Object_RefreshSelectorById();
void Engine_ActorSetPosition();
void Engine_ActorSetAnimationAndWait();
void Engine_ActorJump();
void Engine_ActorStartRepeatedMotion();
void Engine_ActorRunRepeatedMotion();
void Engine_ActorSetChildValue();
void Engine_EventSetMessage();
void Ui_SetRenderResultFromObject();
void Engine_ActorSetSpritePriority();
void Engine_CameraFollowActor();
void Engine_EventRequestExit();
void BattleFx_StartTwelveFrameBlend();
void BattleFx_SetBlock30Values12Zero();
void BattleFx_SetBlock30Values128One();
void Engine_EventOpenScreen();
void Engine_EventCloseScreen();
void Engine_EventWaitForScreen();
void Engine_AudioPlayCue();

/* Haidia, the morning after the storm: the leader wakes in the house, the
   view opens on the room, the two talk over the scene's messages, and both
   walk out of the house. */
void FieldScene_RunComplexActorSequence(void)
{
    /* FAKEMATCH: a one-halfword record keeps the actor-flag zero in a halfword
       register, which the reference reloads from the literal pool after the
       actor lookup. */
    s32 base;
    struct FieldSprite *sprite;
    struct FieldActor *p12;
    struct FieldActor *p89;
    struct MapScrollWork *work;
    struct FieldActor *scene_actor;
    struct EventWork **control;
    struct SceneHalf stopped;
    s32 ground;

    control = (struct EventWork **)Ram_EventWork;
    /* FAKEMATCH: stage the root reads before consuming the event record. */
    {
        struct EventWork *event = *control;

        work = *(struct MapScrollWork **)Ram_MapWork;
        scene_actor = event->view_center;
    }
    sprite = Object_GetById(17)->sprite;
    Engine_EventBegin();
    Engine_ActorSetPosition(11, 0, 0);
    Engine_ActorSetPosition(12, 0, 0);
    Engine_ActorSetPosition(13, 0, 0);
    Engine_ActorSetPosition(14, 0, 0);
    Engine_ActorSetPosition(15, 0, 0);
    Engine_ActorSetPosition(16, 0, 0);
    Engine_ActorSetSpriteFlags(Object_GetById(0), 0);
    Engine_ActorSetAnimation(0, 18);
    ground = 0;
    stopped.value = 0;
    sprite->rotation = 1365;
    p12 = Object_GetById(17);
    p12->motion_flags = stopped.value;
    Engine_ActorSetSpriteFlags(Object_GetById(17), 0);
    Engine_ActorSetPosition(17, 37748736, 42598400);
    Map_ClearLayerEntryFlag(7);
    Engine_ActorSetPosition(8, 34996224, 45088768);
    Graphics_EnableObjLayerAndCallbacks();
    Ui_SetRenderResultFromObject(8);
    base = (s32)MsgHaidiaWake;
    UiText_ShowCenteredMessage(base, 1, 0);
    Engine_EventWait(40);
    MapRender_SetValues(65536, 65536, 65536);
    Ui_SetRenderResultFromObject(8);
    UiText_ShowCenteredMessage(base + 1, 1, 0);
    ObjectDispatch_StopCallbacksAndHideLayers();
    Engine_EventWait(40);
    work->min_x = 0x01480000;
    work->min_y = 0x02580000;
    work->max_x = 0x02700000;
    work->max_y = 0x03300000;
    scene_actor->x.fixed = 0x02340000;
    scene_actor->y.fixed = ground;
    scene_actor->z.fixed = 0x02b30000;
    Engine_MapRedraw();
    Engine_TaskWait(1);
    (*control)->start_transition = 521;
    (*control)->transition_frames = 64;
    BattleFx_StartTwelveFrameBlend();
    (*(struct FieldBlendWork **)((u8 *)control + Ram_SceneMapStateOffset))->loud = 1;
    BattleFx_SetBlock30Values12Zero();
    Engine_TaskWait(30);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    BattleFx_SetBlock30Values128One();
    Engine_ActorSetAnimationAndWait(8, 4);
    Engine_EventSetMessage(base + 2);
    Event_ShowMessageAndWait(36872, 0, 60);
    Engine_ActorRunRepeatedMotion(0, 2);
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(8, 1);
    Engine_EventWait(40);
    Event_ShowMessageAndWait(36872, 0, 20);
    Engine_ActorRunRepeatedMotion(0, 2);
    Map_SetLayerEntryFlag(7);
    Engine_EventWait(20);
    Map_ClearLayerEntryFlag(8);
    Actor_SetSpeed(0, 65536, 32768);
    Engine_ActorSetAnimation(0, 19);
    Actor_MoveToAndWait(0, 557, 679);
    Map_SetLayerEntryFlag(8);
    Map_ClearLayerEntryFlag(9);
    Actor_MoveToAndWait(0, 555, 680);
    Engine_EventWait(30);
    Actor_FaceDirection(8, 53248, 0);
    Engine_ActorSetSpriteFlags(Object_GetById(0), 1);
    Engine_ActorJump(0, 4, 0);
    Actor_WalkToAndWait(0, 543, 674);
    Engine_ActorSetSpritePriority(0, 3);
    Actor_FaceDirection(0, 16384, 40);
    Engine_ActorSetAnimationAndWait(8, 4);
    Engine_EventWait(20);
    Event_ShowMessage(36872, 0);
    FieldScene_RunSixStepSequence17e4();
    Engine_ActorStartRepeatedMotion(8, 2);
    Event_ShowMessageAndWait(36872, 0, 20);
    Object_GetById(8)->unknown_5a &= 0xfe;
    Actor_WalkToAndWait(8, 542, 680);
    Engine_EventWait(1);
    Object_GetById(8)->unknown_5a |= 0x1;
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(8, 2);
    ObjectDispatch_RegisterChildMetadata(Object_GetById(0), 226);
    Engine_GameFlagSet(33);
    Engine_AudioPlayCue(126);
    Engine_ActorSetChildValue(0, 7);
    Engine_EventWait(10);
    Engine_ActorSetChildValue(0, 0);
    Engine_EventWait(20);
    Object_GetById(8)->unknown_5a &= 0xfe;
    Actor_WalkToAndWait(8, 534, 688);
    Engine_EventWait(1);
    Object_GetById(8)->unknown_5a |= 0x1;
    Engine_EventWait(20);
    Actor_SetSpeed(8, 98304, 49152);
    Actor_SetSpeed(0, 98304, 49152);
    Engine_CameraFollowActor(8, 1);
    p89 = Object_GetById(0);
    p89->priority_flags |= 0x1;
    Engine_ActorEnableActionCallback(8, gHaidiaBabiSharedAction);
    Engine_EventWait(20);
    Engine_ActorEnableActionCallback(0, gHaidiaBabiSharedAction);
    Object_RefreshSelectorById(8);
    Actor_WalkToAndWait(8, 419, 661);
    Actor_WalkToAndWait(8, 408, 661);
    Engine_ActorSetAnimation(8, 1);
    Engine_ActorSetAnimation(0, 1);
    Actor_FaceDirection(8, 16384, 10);
    Event_OpenMessage(32776, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        (*control)->message++;
    }
    Engine_EventWait(20);
    Event_ShowMessageAndWait(32776, 0, 20);
    Engine_ActorSetAnimation(0, 3);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(20);
    Engine_ActorEnableActionCallback(8, gHaidiaBabiActorExitAction);
    Engine_ActorEnableActionCallback(0, gHaidiaBabiLeaderExitAction);
    Engine_EventWait(20);
    (*control)->start_transition = 513;
    (*control)->transition_frames = 16;
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventRequestExit(20);
}

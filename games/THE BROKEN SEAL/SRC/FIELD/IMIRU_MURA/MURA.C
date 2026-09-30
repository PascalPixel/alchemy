#include "IMIRU.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "CALL.H"

extern const struct SceneEntrance gImiruMuraEntrances2[];
extern const struct SceneEntrance gImiruMuraEntrancesOther[];

extern u8 gImiruMuraPlacements2[];
extern const struct ScenePlacement gImiruMuraPlacements[];
extern const struct ScenePlacement gImiruMuraPlacementsFlag881[];
void FieldScene_PrepareActors(u8 *placements);

extern const struct SceneEvent gImiruMuraEvents2[];
extern const struct SceneEvent gImiruMuraEventsOther[];

extern u8 MsgMakyuriReallySayDie[];
extern u8 MsgMakyuriFeelMuchBetter[];
extern u8 MsgMakyuriOoohHelpMe[];

extern const u16 *ImiruMura_ExitCellSteps[];
extern s16 ImiruMura_ExitCellPoints[][2];

extern u8 MsgMakyuriCatchUpLostOpportunity[];
extern u8 MsgMakyuriStoreClosedUntilWell[];
extern u8 ImiruMura_TurnScript[];
s32 Engine_GameFlagIsSet();
void Engine_ShopOpen();
void Engine_EventBegin();
void Engine_ActorFaceActor();
void Engine_EventWait();
void Engine_EventSetMessage();
void Engine_EventShowMessage();
void Engine_ActorFaceDirection();
void Engine_EventEnd();
void Engine_CameraSetSpeed();
void Engine_CameraMoveTo();
void Engine_CameraWaitForMove();
void Object_SetActionCallbackAndRefreshById();

extern u8 MsgMakyuriShameStoreClosed[];
extern u8 MsgMakyuriTooSickBusiness[];

extern u8 MsgMakyuriRecoveredThanksFountain[];
extern u8 MsgMakyuriMomsSickGetWhatever[];
extern u8 MsgMakyuriDadLumberjack[];

void SceneState_UpdateActor11WithFlag203(void);
void RunEventScript02(void);
void FieldScene_RunPrimaryScriptChoreography(void);
void FieldScene_RunThreeActorChoreography(void);
void FieldScene_RunScene399SequenceA(void);
void SceneState_UpdateZoneFlagsFromActorZero(void);
extern u8 ImiruMura_ActorScriptA[];

s32 OverlayObject_UpdateWobbleByCounter(struct Object *obj)
{
    switch (obj->counter) {
    case 6:
        obj->x += 0xffffc000;
        obj->z += 0x2000;
        break;
    case 4:
        obj->x += 0x2000;
        obj->z -= 0x1000;
        break;
    case 2:
        obj->x += 0x1000;
        obj->z += 0xfffff800;
        break;
    case 0:
        obj->x += 0x1000;
        obj->z += 0xfffff800;
        if (obj->mode != 0) {
            obj->counter = Engine_MathModulo(Random_Next(), 40) + 40;
        } else {
            obj->counter = Engine_MathModulo(Random_Next(), 20) + 20;
        }
        break;
    }
    obj->counter--;
    return 1;
}

s32 OverlayObject_UpdateFacingTowardTarget(void *obj)
{
    s32 delta;
    u16 old;
    s32 angle;
    void *target;
    target = (*(void * *)((u8 *)(obj) + (0x68)));
    if (target != NULL) {
        (*(u8 *)((u8 *)(obj) + (0x5A))) = (u8)(0xFE & (*(u8 *)((u8 *)(obj) + (0x5A))));
        angle = (u16)ArcTan2((*(s32 *)((u8 *)(target) + (0x10))) - (*(s32 *)((u8 *)(obj) + (0x10))), (*(s32 *)((u8 *)(target) + (8))) - (*(s32 *)((u8 *)(obj) + (8))));
        old = (*(u16 *)((u8 *)(obj) + (6)));
        delta = (s16)(angle - old);
        if (delta != 0) {
            if (delta > 0x1000) delta = 0x1000;
            if (delta < -0x1000) delta = -0x1000;
            (*(u16 *)((u8 *)(obj) + (6))) = (u16)(old + delta);
        }
    }
    return 1;
}

/* Where the party appears; the second area has its own entrances. */
const struct SceneEntrance *Scene_GetEntrances(void)
{
    if (gGameState.scene == (s32)&SceneId_ImiruMura2) {
        return gImiruMuraEntrances2;
    }
    return gImiruMuraEntrancesOther;
}

/* The whole four-byte owner. */
s32 SceneData_ReturnZero(void)
{
    return 0;
}

/* The eight-byte owner includes its one pool word. */
u8 *SceneData_GetTableA990(void)
{
    return (u8 *)ImiruMura_SceneTable;
}

/*
 * The actors placed in Imil. The second area's table is prepared, then
 * patched in place once flag 0x881 is set: the overlay image is writable.
 * The coordinates are written as shifts, which is how a 16.16 whole number
 * is built here. The word at +0x4c is set only on this path and never read
 * back, so its meaning is unverified.
 */
const struct ScenePlacement *Scene_GetPlacements(void)
{
    u8 *script;

    if (gGameState.scene == ((s32)&SceneId_ImiruMura2)) {
        script = gImiruMuraPlacements2;
        FieldScene_PrepareActors(script);
        if (GameFlag_IsSet(0x881) != 0) {
            script[262] = 0;
            *(s32 *)(script + 0x50) = 182 << 16;
            *(s32 *)(script + 0x58) = 564 << 16;
            *(s32 *)(script + 0x4c) = 2;
        }
        return (const struct ScenePlacement *)script;
    }

    if (GameFlag_IsSet(0x881) != 0) {
        return gImiruMuraPlacementsFlag881;
    }
    return gImiruMuraPlacements;
}

void FieldScene_Forward2188(void)
{
    StagedActor_RunHeadingProbeStep();
}

/* What Imil answers; the second area has its own events. */
const struct SceneEvent *Scene_GetEvents(void)
{
    if (gGameState.scene == (s32)&SceneId_ImiruMura2) {
        return gImiruMuraEvents2;
    }
    return gImiruMuraEventsOther;
}

void SceneDialogue_RunActorEightFlagGatedDialogue(void)
{
    Event_Begin();
    if (GameFlag_IsSet(3) != 0) {
        Event_SetMessage(MSG_EVEN_IF_MIA_HEALS_US);
    } else {
        Event_SetMessage(MSG_BRRRRR_CHOO_IM_FREEZING_MIA);
    }
    {
        s32 val = 0;
        s32 mode = 8;
        Event_ShowMessage(mode, val);
    }
    Event_End();
}

void SceneDialogue_ShowLine1571Or152F(void)
{
    Event_Begin();
    if (GameFlag_IsSet(3) != 0) {
        Event_SetMessage(MSG_CANT_UNDERSTAND_WHY_ANY_ONE);
    } else {
        Event_SetMessage(MSG_MIA_SHOULD_HERE_BY_NOW);
    }
    Event_ShowMessage(8, 0);
    Event_End();
}

void SceneDialogue_RunActor9Line(void)
{
    Event_Begin();
    Actor_FaceActor(9, ACTOR_PARTY_LEADER, 10);
    Event_SetMessage(MSG_HI_NEW_IN_IMIL);
    Event_AskYesNo(9, 0);
    Event_End();
}

/*
 * One scripted section, bracketed by an open and a close call, in which story
 * flag 0x881 picks between two arms on channel 10.  The arms differ only in the
 * message id and one step call, and stay separate so that every call is written
 * once.  258 is a pose id, 0x3000 three sixteenths of a turn.  Engine_EventAskYesNo's
 * unused s32 return is what fixes that call's argument order.
 */
void SceneDialogue_RunActorTenFlag881Dialogue(void)
{
    Event_Begin();

    if (GameFlag_IsSet(0x881) != 0) {
        Event_SetMessage(MSG_ONE_TWO_THREE_FOUR_2);
        Event_ShowMessage(10, 0);
        Actor_SetAttachedEffect(10, 258);
        Event_Wait(40);
        Actor_SetAnimation(10, 1);
        Event_Wait(20);
        Actor_FaceActor(10, ACTOR_PARTY_LEADER, 20);
        Event_AskYesNo(10, 0);
        Call_02002630(10, 0x3000, 10);
        Actor_SetAnimation(10, 9);
    } else {
        Event_SetMessage(MSG_ONE_TWO_THREE_FOUR);
        Event_ShowMessage(10, 0);
        Actor_SetAttachedEffect(10, 258);
        Event_Wait(40);
        Actor_SetAnimation(10, 1);
        Event_Wait(20);
        Actor_FaceActor(10, ACTOR_PARTY_LEADER, 20);
        Event_ShowMessage(10, 0);
        Call_02002684(10, 0x3000, 10);
        Actor_SetAnimation(10, 9);
    }

    Event_End();
}

/* Picks one of three scripted call sequences depending on two condition
 * checks (codes 2177 and 2091), each acting on actor 9 and/or actor 8. */
void FieldScene_RunSupplementalSequenceOne(void)
{

    void *actor9_record;
    void *unused_actor9_record;
    void *actor8_record;

    if (GameFlag_IsSet(2177) != 0) {
        Event_Begin();
        unused_actor9_record = Value3(Engine_ActorFaceActor, 9, 0, 0);
        Engine_EventWait(10);
        Engine_EventSetMessage((s32)MsgMakyuriReallySayDie);
        Event_AskYesNo(9, 0);
        Event_End();
    } else {
        if (GameFlag_IsSet(2091) != 0) {
            Event_Begin();
            Actor_SetAnimation(9, 7);
            Engine_MapAnimateCells((s32)ImiruMura_CellStepsA, 10, 69);
            Engine_EventSetMessage((s32)MsgMakyuriFeelMuchBetter);
            Event_ShowMessage(9, 0);
            Actor_SetAnimation(9, 8);
            Map_AnimateCells((s32)ImiruMura_CellStepsB, 10, 69);
            Event_End();
        } else {
            Event_Begin();
            actor9_record = Object_GetById(9);
            ((struct SceneRecord *)actor9_record)->field_0x64 = 10;
            Engine_ActorEnableActionCallback(9, (s32)ImiruMura_ActorScriptA);
            Engine_EventSetMessage((s32)MsgMakyuriOoohHelpMe);
            Engine_EventShowMessage(9, 0);
            Engine_ActorStop(8);
            Actor_ShowEmote(8, 256, 40);
            Actor_FaceDirection(8, 53248, 10);
            Actor_StartRepeatedMotion(8, 2);
            Event_ShowMessageAndWait(8, 0, 20);
            Engine_ActorEnableActionCallback(0, (s32)ImiruMura_ActorScriptC);
            Actor_SetSpeed(8, 104857, 52428);
            Object_SetActionCallbackAndRefreshById(8, (s32)ImiruMura_ActorScriptB);
            Engine_EventWait(40);
            Actor_Jump(8, 2, 0);
            Actor_StartRepeatedMotion(8, 2);
            Engine_ActorSetAttachedEffect(8, 258);
            Engine_EventWait(60);
            Event_ShowMessageAndWait(8, 0, 10);
            Actor_FaceDirection(8, 12288, 20);
            Actor_StartRepeatedMotion(8, 2);
            Engine_EventShowMessage(8, 0);
            actor8_record = Object_GetById(8);
            *(u8 *)((u8 *)(actor8_record) + ACTOR_FLAGS_OFFSET) ^= 0x2;
            GameFlag_Set(0x82c);
            Event_End();
        }
    }
}

void SceneDialogue_RunActor12Line(void)
{
    Event_Begin();
    Event_SetMessage(MSG_DO_WANT_WEAPONS);
    Event_AskYesNo(12, 0);
    Event_End();
}

void SceneDialogue_RunActor18Line(void)
{
    Event_Begin();
    Event_SetMessage(MSG_DO_WANT_SEE_RESTAURANT_MENU);
    Event_AskYesNo(18, 0);
    Event_End();
}

void SceneDialogue_RunActor20BranchScene(void)
{
    Event_Begin();
    if (GameFlag_IsSet(3) != 0) {
        Event_SetMessage(MSG_EVERYONE_COUNTS_ON_MIA_THATS);
        Event_ShowMessage(20, 0);
    } else {
        Event_SetMessage(MSG_HAVE_VISITED_OLD_COUPLE_WHO);
        Event_AskYesNo(20, 0);
        GameFlag_Set(0x82a);
        GameFlag_Set(0x82c);
    }
    Event_End();
}

void SceneDialogue_RunActor20FlaggedLine(void)
{
    Event_Begin();
    if (GameFlag_IsSet(3) != 0) {
        Event_SetMessage(MSG_WE_HAVE_DO_WHATEVER_WE);
    } else {
        Event_SetMessage(MSG_MIA_WAS_SAYING_SHE_HAS);
    }
    Event_ShowMessage(20, 0);
    Event_End();
}

void FieldScene_RunScene399_020005dc(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Event_SetMessage(MSG_HAPPENED_IN_LIGHTHOUSE_NORTHEAST);
    Event_ShowMessage(8, 0);
    Actor_FaceDirection(8, 0x3000, 10);
    Event_End();
}

void SceneDialogue_RunActorEightBranchedDialogue(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x82b) != 0) {
        Event_SetMessage(MSG_MIA_CLAN_ONCE_LIVED_HERE);
    } else if (GameFlag_IsSet(0x82c) != 0) {
        Event_SetMessage(MSG_HES_ALWAYS_EXAGGERATING_THINGS_BUT);
    } else {
        Event_SetMessage(MSG_MIA_RUNNING_AROUND_TOWN_CARING);
    }
    Event_ShowMessage(8, 0);
    Event_End();
}

void FieldScene_RunSingleStep(void)
{
    FieldScene_RunSupplementalSequenceOne();
}

void SceneDialogue_ShowLine156E(void)
{
    Event_Begin();
    Event_SetMessage(MSG_MIA_GOOD_GIRL_WISH_HAD);
    Event_ShowMessage(10, 0);
    Event_End();
}

void SceneDialogue_ShowLine1573Or155A(void)
{
    Event_Begin();
    if (GameFlag_IsSet(3) != 0) {
        Event_SetMessage(MSG_FEEL_LIKE_GROWN_UP_WHEN);
    } else {
        Event_SetMessage(MSG_THESE_FOLK_OKAY_THEY_DONT);
    }
    Event_ShowMessage(19, 0);
    Event_End();
}

/* Imil door exit: freeze the area's actors, open the door touched (trigger 50 + exit) with its cell animation, walk the leader out and request that exit. */
void ImiruMura_RunExitDoor(void)
{
    struct EventWork *event;
    struct FieldActor *actor;
    u32 i;
    s32 exit;

    event = gEventWork;
    Engine_EventBegin();
    for (i = 8; i <= 65; i++) {
        actor = Object_GetById(i);
        if (actor != NULL) {
            actor->motion_flags = 0;
        }
    }
    exit = (s16)(event->touched_trigger - 50);
    if (exit == 6) {
        Engine_AudioPlayCue(188);
    } else {
        Engine_AudioPlayCue(158);
    }
    {
        s32 x = ImiruMura_ExitCellPoints[exit - 1][0];
        s32 y = ImiruMura_ExitCellPoints[exit - 1][1];

        Engine_MapAnimateCells(ImiruMura_ExitCellSteps[exit - 1], x, y);
    }
    Call3((void (*)())Engine_ActorSetSpeed, 0, 0x8000, 0x4000);
    gEventWork->start_transition = 0x100;
    if (exit == 6) {
        Call3((void (*)())Engine_ActorSetSpeed, 0, 0x3333, 0x1999);
        Engine_ActorSetAnimation(0, 2);
        Engine_ActorSetSpritePriority(0, 3);
        Call3((void (*)())Engine_ActorSetDestinationOffset, 0, 0, -8);
    } else {
        Object_GetById(0)->motion_flags = 0;
        Call3((void (*)())Engine_ActorCenterAndWalk, 0, 3, -16);
    }
    Engine_EventWait(16);
    Engine_EventRequestExit(exit);
    Engine_EventEnd();
}

/* Imil weapon shop: facing the counter with the village cured opens shop 10;
 * otherwise the keeper talks about being shut, or after recovery about catching
 * up on lost business. */
void ImiruMura_RunWeaponShop(void)
{
    s32 dir;

    dir = *(s16 *)(((s32 (*)())Object_GetById)(0) + 6);
    if (Engine_GameFlagIsSet(0x881) != 0) {
        if ((u32)((dir << 16) + 0x5fff0000) <= 0x3ffe0000) {
            Engine_ShopOpen(10, 12);
            return;
        }
        Engine_EventBegin();
        Engine_ActorFaceActor(12, 0, 0);
        Engine_EventWait(10);
        Engine_EventSetMessage((s32)MsgMakyuriCatchUpLostOpportunity);
        Engine_EventShowMessage(12, 0);
        Call3(Engine_ActorFaceDirection, 12, 0x4000, 10);
        Engine_EventEnd();
    } else {
        if ((u32)((dir << 16) + 0x5fff0000) <= 0x3ffe0000) {
            Engine_EventBegin();
            Engine_CameraSetSpeed(0x60000, 0xc000);
            Call4(Engine_CameraMoveTo, 0x1aa0000, -1, 0x1ec0000, 1);
            Engine_CameraWaitForMove();
            Engine_EventWait(20);
            Object_SetActionCallbackAndRefreshById(12, (s32)ImiruMura_TurnScript);
            Engine_EventSetMessage((s32)MsgMakyuriStoreClosedUntilWell);
            Engine_EventShowMessage(12, 0);
            Engine_CameraMoveTo(0x1aa0000, -1, 0x2680000, 1);
            Engine_CameraWaitForMove();
            Engine_EventEnd();
        }
    }
}

/* Imil armor shop: facing the counter with the village cured opens shop 11;
 * otherwise the keeper talks about the cold that closed the store. */
void ImiruMura_RunArmorShop(void)
{
    s32 dir;

    dir = *(s16 *)(((s32 (*)())Object_GetById)(0) + 6);
    if (Engine_GameFlagIsSet(0x881) != 0) {
        if ((u32)((dir << 16) + 0x5fff0000) <= 0x3ffe0000) {
            Engine_ShopOpen(11, 13);
            return;
        }
        Engine_EventBegin();
        Engine_ActorFaceActor(13, 0, 0);
        Engine_EventWait(10);
        Engine_EventSetMessage((s32)MsgMakyuriShameStoreClosed);
        Engine_EventShowMessage(13, 0);
        Call3(Engine_ActorFaceDirection, 13, 0x4000, 10);
        Engine_EventEnd();
    } else {
        if ((u32)((dir << 16) + 0x5fff0000) <= 0x3ffe0000) {
            Engine_EventBegin();
            Engine_CameraSetSpeed(0x60000, 0xc000);
            Call4(Engine_CameraMoveTo, 0x1aa0000, -1, 0x1ec0000, 1);
            Engine_CameraWaitForMove();
            Engine_EventWait(20);
            Object_SetActionCallbackAndRefreshById(13, (s32)ImiruMura_TurnScript);
            Engine_EventSetMessage((s32)MsgMakyuriTooSickBusiness);
            Engine_EventShowMessage(13, 0);
            Engine_CameraMoveTo(0x1aa0000, -1, 0x2680000, 1);
            Engine_CameraWaitForMove();
            Engine_EventEnd();
        }
    }
}

/* Imil item shop: with the village cured the mother keeps shop 12; while she is
 * sick her daughter sells across the counter, and the father talks about Kolima
 * when spoken to off-angle. */
void ImiruMura_RunItemShop(void)
{
    s32 dir;

    dir = *(s16 *)(((s32 (*)())Object_GetById)(0) + 6);
    if (Engine_GameFlagIsSet(0x881) != 0) {
        if ((u32)((dir << 16) + 0x5fff0000) <= 0x3ffe0000) {
            ((s32 (*)())Engine_ShopOpen)(12, 15);
            return;
        }
        Engine_EventBegin();
        Engine_ActorFaceActor(15, 0, 0);
        Engine_EventSetMessage((s32)MsgMakyuriRecoveredThanksFountain);
        Engine_EventShowMessage(15, 0);
        Call3(Engine_ActorFaceDirection, 15, 0x4000, 0);
        Engine_EventEnd();
    } else {
        if ((u32)((dir << 16) + 0x5fff0000) <= 0x3ffe0000) {
            Engine_EventBegin();
            Engine_EventSetMessage((s32)MsgMakyuriMomsSickGetWhatever);
            Engine_EventShowMessage(14, 0);
            ((s32 (*)())Engine_ShopOpen)(12, 14);
            Engine_EventEnd();
        } else {
            Engine_ActorFaceActor(14, 0, 10);
            Engine_EventSetMessage((s32)MsgMakyuriDadLumberjack);
            Engine_EventShowMessage(14, 0);
            Call3(Engine_ActorFaceDirection, 14, 0x5000, 10);
        }
    }
}

void FieldScene_RunScene399_02000a3c(void)
{
    struct FieldActor *leader;

    leader = (struct FieldActor *)((s32)Object_GetById(0));
    if ((u16)(leader->facing + 0x5fff) <= 0x3ffe) {
        Inn_Open(4, 16);
    } else {
        Event_Begin();
        Actor_FaceActor(16, ACTOR_PARTY_LEADER, 10);
        if (GameFlag_IsSet(0x881) != 0) {
            Event_SetMessage(MSG_ITS_ALMOST_TIME_FOR_LEAVE);
            Event_AskYesNo(16, 0);
        } else {
            Event_SetMessage(MSG_WHY_HAVE_TWO_GROUPS_TRAVELERS);
            Event_ShowMessage(16, 0);
        }
        Actor_FaceDirection(16, 0x3000, 10);
        Event_End();
    }
}

void FieldScene_RunScene399_02000abc(void)
{
    struct FieldActor *leader;

    leader = (struct FieldActor *)((s32)Object_GetById(0));
    if ((u16)(leader->facing + 0x5fff) <= 0x3ffe) {
        Event_Begin();
        if (GameFlag_IsSet(0x82d) == 0) {
            Event_SetMessage(MSG_MAY_ONLY_STUDENT_BUT_CAN);
            Event_ShowMessage(19, 0);
            GameFlag_Set(0x82d);
        }
        Event_End();
        Sanctum_Open(19);
    } else {
        Event_Begin();
        if (GameFlag_IsSet(0x881) != 0) {
            Event_SetMessage(MSG_MIA_GOING_ON_JOURNEY_WITH);
            Event_ShowMessage(19, 0);
        } else if (GameFlag_IsSet(3) != 0) {
            Event_SetMessage(MSG_AM_HEALER_WHILE_MIA_OUT);
            Event_ShowMessage(19, 0);
        } else {
            Event_SetMessage(MSG_LOOKING_FOR_MIA);
            (void)Event_AskYesNo(19, 0);
            Actor_FaceDirection(19, 0x3000, 10);
        }
        Event_End();
    }
}

/* Imil: entry setup for the two Imil areas, by entrance and story flags. */
s32 ImiruMura_ApplyEntryState(void)
{
    struct FieldActor *leader;
    s32 entrance;

    if (gGameState.scene == (s32)&SceneId_ImiruMura1) {
        leader = Object_GetById(0);
        gEventWork->start_transition = 0x100;
        Engine_ActorSetAnimation(10, 9);
        if (Value1(Engine_GameFlagIsSet, 0x109)) {
            Engine_GameFlagClear(0x200);
            Engine_GameFlagClear(0x201);
        }
        leader->unknown_64 = 0;
        leader->unknown_66 = 0;
        Value2(Engine_TaskAddCallback, (s32)FieldScene_RunScene399SequenceA, 0xc80);
        Engine_TaskAddCallback((s32)SceneState_UpdateZoneFlagsFromActorZero, 0xc80);
        Engine_ActorSetSpritePriority(11, 1);
        if (Engine_GameFlagIsSet(0x203)) {
            SceneState_UpdateActor11WithFlag203();
        }
        if (!Engine_GameFlagIsSet(0x109) && gGameState.entrance == 9) {
            RunEventScript02();
        }
    } else if (gGameState.scene == (s32)&SceneId_ImiruMura2) {
        gEventWork->start_transition = 0x209;
        entrance = gGameState.entrance;
        if (entrance == 1) {
            Engine_ActorSetChildValue(21, 15);
            Object_GetById(21)->collision_flags |= 8;
            Engine_ActorSetSpritePriority(21, 1);
            if (Engine_GameFlagIsSet(0x881)) {
                Call6(Engine_MapCopyCellAttributes, 10, 7, 1, 1, 10, 8);
                Engine_MapCopyCellsTo(3, 125, 9, 69, 3, 3);
                Engine_MapRedraw();
                Engine_TaskWait(1);
                Engine_ActorEnableActionCallback(8, (const u8 *)2);
                Engine_ActorSetPosition(10, 0, 0);
            } else if (Engine_GameFlagIsSet(0x82c) && Engine_GameFlagIsSet(0x82a)) {
                Engine_ActorSetSpriteFlags(Object_GetById(10), 0);
                Engine_ActorSetPosition(9, 0xae0000, 0xa40000);
                Engine_ActorSetSpriteFlags(Object_GetById(9), 0);
                Engine_ActorSetAnimation(9, 5);
                Call3(Engine_ActorSetPosition, 8, 0xa80000, 0x980000);
                Object_GetById(8)->facing = 0x3000;
                if (!Engine_GameFlagIsSet(0x82b)) {
                    FieldScene_RunPrimaryScriptChoreography();
                }
            } else {
                Call6(Engine_MapCopyCellAttributes, 10, 7, 1, 1, 10, 8);
                Engine_MapCopyCellsTo(3, 125, 9, 69, 3, 3);
                Engine_MapRedraw();
                Engine_TaskWait(1);
                if (Engine_GameFlagIsSet(0x82c)) {
                    Engine_ActorSetPosition(8, 0x950000, 0x740000);
                    Object_GetById(8)->facing = 0;
                    Object_GetById(9)->unknown_66 = 0;
                    Engine_ActorEnableActionCallback(9, ImiruMura_ActorScriptA);
                } else {
                    Engine_ActorEnableActionCallback(8, (const u8 *)2);
                }
            }
        } else if (entrance == 2) {
            if (!Engine_GameFlagIsSet(0x881)) {
                Object_GetById(11)->unknown_66 = 1;
                Engine_ActorEnableActionCallback(11, ImiruMura_ActorScriptA);
            }
        } else if (entrance == 4) {
            if (Engine_GameFlagIsSet(0x881)) {
                Call3(Engine_ActorSetPosition, 12, 0x16c0000, 0x2420000);
                Engine_ActorSetSpritePriority(12, 2);
                Object_GetById(12)->collision_flags |= 4;
                Engine_MapCopyCellsTo(6, 125, 22, 88, 3, 3);
                Call3(Engine_ActorSetPosition, 13, 0x1ec0000, 0x2420000);
                Engine_ActorSetSpritePriority(13, 2);
                Object_GetById(13)->collision_flags |= 4;
                Engine_MapCopyCellsTo(9, 125, 28, 88, 3, 3);
            } else {
                Object_GetById(12)->scale_x = -0x10000;
                Engine_ActorSetSpriteFlags(Object_GetById(12), 0);
                Engine_ActorSetAnimation(12, 5);
                Engine_ActorSetSpriteFlags(Object_GetById(13), 0);
                Engine_ActorSetAnimation(13, 5);
            }
        } else if (entrance == 3) {
            if (Engine_GameFlagIsSet(0x881)) {
                Call3(Engine_ActorSetPosition, 15, 0x1cc0000, 0x1020000);
                Engine_ActorSetSpritePriority(15, 2);
                Object_GetById(15)->collision_flags |= 4;
                Engine_ActorSetPosition(14, 0x1980000, 0x1080000);
                Object_GetById(14)->facing = 0x1000;
                Engine_MapCopyCellsTo(12, 125, 26, 70, 3, 3);
            } else {
                Call3(Engine_ActorSetPosition, 14, 0x1cc0000, 0x1020000);
                Engine_ActorSetSpritePriority(14, 2);
                Object_GetById(14)->collision_flags |= 4;
                Object_GetById(15)->scale_x = -0x10000;
                Engine_ActorSetSpriteFlags(Object_GetById(15), 0);
                Engine_ActorSetAnimation(15, 5);
            }
        } else if (entrance == 7) {
            if (Engine_GameFlagIsSet(0x881)) {
                Object_GetById(20)->facing = 0x3000;
                if (!Engine_GameFlagIsSet(0x82e)) {
                    Call3(Engine_ActorSetPosition, 20, 0x28a0000, 0xa10000);
                    FieldScene_RunThreeActorChoreography();
                } else {
                    Call3(Engine_ActorSetPosition, 20, 0x2840000, 0xa60000);
                }
            }
        }
    }
    return 0;
}

void FieldScene_RunPrimaryScriptChoreography(void)
{
    extern u8 ImiruMura_TurnScript[];

    struct FieldActor *leader;
    s32 tbl;
    struct EventWork *work;

    Event_Begin();
    Actor_SetPosition(ACTOR_MIA, 0xb60000, 0x960000);
    Camera_MoveTo(0x8d0000, -1, 0xdd0000, 0);
    Task_Wait(1);
    Camera_SetSpeed(0x4ccc, 0x999);
    Camera_MoveTo(0x8c0000, -1, 0xa40000, 1);
    work = *(struct EventWork **)Data_03001ebc;
    work->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    work->transition_frames = 40;
    Event_OpenScreen();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x6666, 0x3333);
    Actor_SetSpeed(ACTOR_GERALD, 0x6666, 0x3333);
    Actor_SetSpeed(ACTOR_IVAN, 0x6666, 0x3333);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 142, 221);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xd000, 0);
    leader = (struct FieldActor *)Object_GetById(0);
    if (leader != NULL) {
        Actor_SetPosition(ACTOR_GERALD, leader->x.fixed, leader->z.fixed);
    }
    leader = (struct FieldActor *)Object_GetById(0);
    if (leader != NULL) {
        Actor_SetPosition(ACTOR_IVAN, leader->x.fixed, leader->z.fixed);
    }
    Actor_WalkTo(ACTOR_GERALD, 150, 234);
    Actor_WalkToAndWait(ACTOR_IVAN, 134, 234);
    Actor_SetAnimation(ACTOR_GERALD, 1);
    tbl = (s32)ImiruMura_PrimaryScript;
    Call3(Engine_ObjectSetTargetAndCallback, 0, 0x10003, tbl);
    Call3(Engine_ObjectSetTargetAndCallback, 1, 0x10003, tbl);
    Call3(Engine_ObjectSetTargetAndCallback, 2, 0x10003, tbl);
    Camera_WaitForMove();
    tbl = (s32)ImiruMura_TurnScript;
    Actor_EnableActionCallback(9, tbl);
    Event_Wait(40);
    Actor_SetAttachedEffect(ACTOR_MIA, 0x102);
    Event_Wait(40);
    Actor_RunRepeatedMotion(ACTOR_MIA, 1);
    Event_SetMessage(MSG_HOW_FEELING);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Engine_ActorEnableActionCallback(9, tbl);
    Event_ShowMessageAndWait(9, 0, 20);
    Actor_FaceDirection(ACTOR_MIA, 0x8000, 20);
    Actor_FaceDirection(8, 0, 10);
    Actor_SetAnimationAndWait(8, 4);
    Event_ShowMessageAndWait(8, 0, 40);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_MIA, 0x4000, 0);
    Actor_FaceDirection(8, 0x3000, 20);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 10);
    Engine_ActorEnableActionCallback(9, tbl);
    /* SceneState_StoreTable96adToWork at 0x020016c8. */
    SceneState_StoreTable96adToWork();
    Actor_ShowEmote(ACTOR_MIA, 0x101, 60);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 40);
    Event_ShowMessageAndWait(9, 0, 20);
    Actor_ShowEmote(8, 0x105, 60);
    Actor_SetAnimation(9, 7);
    Map_AnimateCells((s32)ImiruMura_CellStepsA, 10, 69);
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_MIA, 2);
    Actor_SetAnimationAndWait(ACTOR_MIA, 4);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Actor_RunRepeatedMotion(9, 1);
    Event_Wait(40);
    Actor_SetAnimation(9, 8);
    Map_AnimateCells((s32)ImiruMura_CellStepsB, 10, 69);
    Event_Wait(40);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Event_Wait(20);
    Actor_FaceDirection(8, 0, 20);
    Actor_SetAnimationAndWait(8, 3);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_ShowEmote(ACTOR_MIA, 0x101, 30);
    Actor_FaceDirection(ACTOR_MIA, 0x8000, 10);
    Actor_SetAnimation(ACTOR_MIA, 4);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 10);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Event_Wait(40);
    Actor_SetSpeed(ACTOR_MIA, 0x10000, 0x8000);
    ((struct Work_399 *)((s32)Object_GetById(3)))->f100 = 0;
    Actor_EnableActionCallback(ACTOR_MIA, (s32)ImiruMura_MiaScriptA);
    while (*(s16 *)(((s32)Object_GetById(3)) + ACTOR_DONE_OFFSET) == 0) {
        Task_Wait(1);
    }
    Camera_MoveTo(0x8c0000, -1, 0xc60000, 1);
    Object_RefreshSelectorById(3);
    Actor_ShowEmote(ACTOR_MIA, 0x101, 80);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 40);
    Actor_RunRepeatedMotion(ACTOR_MIA, 1);
    Event_Wait(10);
    Event_ShowMessage(ACTOR_MIA, 0);
    Audio_PlayCue(131);
    Call2(Engine_ColorBufferApplySource, 0x10000, 0);
    ColorBuffer_ApplyTarget(0x207e9f, 0);
    Engine_ColorBufferInterpolate(10);
    Task_Wait(1);
    Audio_PlayCue(220);
    Task_Wait(40);
    ColorBuffer_ApplyTarget(0x10000, 0);
    Engine_ColorBufferInterpolate(60);
    Task_Wait(60);
    Actor_SetAttachedEffect(ACTOR_MIA, 0x102);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_MIA, 0, 10);
    Actor_SetSpeed(ACTOR_MIA, 0x20000, 0x10000);
    Actor_WalkToAndWait(ACTOR_MIA, 202, 198);
    Event_Wait(40);
    Actor_RunRepeatedMotion(ACTOR_MIA, 2);
    Event_ShowMessage(ACTOR_MIA, 0);
    Actor_SetAnimationAndWait(ACTOR_MIA, 4);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Actor_SetAttachedEffect(ACTOR_MIA, 0x102);
    Event_Wait(40);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 40);
    Actor_ShowEmote(ACTOR_MIA, 0x100, 40);
    Event_ShowMessage(ACTOR_MIA, 0);
    Actor_Stop(ACTOR_PARTY_LEADER);
    Actor_Stop(ACTOR_GERALD);
    Actor_Stop(ACTOR_IVAN);
    Actor_SetSpeed(ACTOR_MIA, 0x30000, 0x18000);
    ((struct Work_399 *)((s32)Object_GetById(3)))->f100 = 0;
    Actor_EnableActionCallback(ACTOR_MIA, (s32)ImiruMura_MiaScriptB);
    while (*(s16 *)(((s32)Object_GetById(3)) + ACTOR_DONE_OFFSET) == 0) {
        Task_Wait(1);
    }
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x4000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0x4000, 10);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x40000, 0x20000);
    Actor_SetSpeed(ACTOR_GERALD, 0x40000, 0x20000);
    Actor_SetSpeed(ACTOR_IVAN, 0x40000, 0x20000);
    Audio_PlayCue(152);
    ((struct FieldActor *)((s32)Object_GetById(0)))->unknown_5a &= 254;
    ((struct FieldActor *)((s32)Object_GetById(1)))->unknown_5a &= 254;
    ((struct FieldActor *)((s32)Object_GetById(2)))->unknown_5a &= 254;
    Actor_SetDestination(ACTOR_PARTY_LEADER, 132, 206);
    Actor_SetDestination(ACTOR_GERALD, 136, 221);
    Actor_SetDestination(ACTOR_IVAN, 122, 238);
    Object_RefreshSelectorById(3);
    Event_Wait(80);
    ((struct FieldActor *)((s32)Object_GetById(0)))->unknown_5a |= 1;
    ((struct FieldActor *)((s32)Object_GetById(1)))->unknown_5a |= 1;
    ((struct FieldActor *)((s32)Object_GetById(2)))->unknown_5a |= 1;
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    Actor_SetSpeed(ACTOR_GERALD, 0xcccc, 0x6666);
    Actor_SetSpeed(ACTOR_IVAN, 0xcccc, 0x6666);
    tbl = (s32)ImiruMura_PrimaryScript2;
    Actor_EnableActionCallback(ACTOR_GERALD, tbl);
    Object_SetActionCallbackAndRefreshById(2, tbl);
    Event_Wait(20);
    work = *(struct EventWork **)Data_03001ebc;
    work->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    work->transition_frames = 24;
    Engine_GameFlagSet(0x82b);
    Event_End();
}

void SceneActor_UpdateCountdownArcPosition(T_0200154c *o)
{
    s32 buf[3];
    s32 *b;
    s32 n;
    s32 t;

    if (o != 0) {
        n = o->unk64 - 1;
        o->unk64 = n;
        t = (s16)n;
        if (t != 0) {
            b = buf;
            b[0] = ImiruMura_ArcOrigin[0];
            b[1] = ImiruMura_ArcOrigin[1] + 0x80000;
            b[2] = ImiruMura_ArcOrigin[2];
            Vector_AddPolarOffset(t << 16, (t << 11) + o->unk66, b);
            o->unk8 = b[0];
            o->unkC = b[1];
            o->unk10 = b[2];
        } else {
            Engine_ObjectDispatchRelease(o);
        }
    }
}

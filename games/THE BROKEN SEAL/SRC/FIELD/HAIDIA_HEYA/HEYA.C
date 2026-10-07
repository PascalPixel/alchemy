#include "TIMED_EVENTS.H"
#include "CALL.H"
#include "FXBLEND.H"
#include "FIELD_EVENT.H"
#include "text/MSG_IDS.H"




s32 OverlayObject_UpdateFacingTowardTarget(struct FacingObject *obj)
{
    s32 delta;
    u16 old;
    s32 angle;
    struct FacingObject *target;

    target = obj->facing_target;
    if (target != NULL) {
        obj->facing_flags = (0xFE & obj->facing_flags);
        angle = (u16)ArcTan2(target->position_z - obj->position_z, target->position_x - obj->position_x);
        old = obj->facing;
        delta = (s16)(angle - old);
        if (delta != 0) {
            if (delta > 0x1000) {
                delta = 0x1000;
            }

            if (delta < -0x1000) {
                delta = -0x1000;
            }
            obj->facing = (old + delta);
        }
    }
    return 1;
}

s32 AdvancePositionScaleAndVelocity(ScaledMotion *motion)
{
    motion->x += motion->velocity_x << 8;
    motion->y += motion->velocity_y << 8;
    motion->scale_x += 1638;
    motion->scale_y += 1638;
    motion->velocity_x += 5;
    motion->velocity_y -= 1;
    return 0;
}

void *HaidiaHeya_GetEntrances(void)
{
    return gValeHouseEntrances;
}

int SceneData_ReturnZero(void)
{
    return 0;
}

void *HaidiaHeya_SelectExits(void)
{
    if (Engine_GameFlagIsSet(0x834) != 0)
        return gValeHouseLateExits;
    return gValeHouseExits;
}

void *SceneData_SelectFlaggedTable(void)
{
    void *tbl;

    if (Engine_GameFlagIsSet(0x87a)) {
        tbl = gValeHouseReturnPlacements;
    } else if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE)) {
        tbl = gValeHousePlacementsAfterLeaving;
    } else {
        tbl = gValeHousePlacements;
    }
    SceneEvents_UpdateInView(tbl);
    return tbl;
}

void *HaidiaHeya_SelectEvents(void)
{
    if (Engine_GameFlagIsSet(0x834) != 0)
        return gValeHouseLateEvents;
    if (Engine_GameFlagIsSet(0x87a) != 0)
        return gValeHouseReturnEvents;
    if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0)
        return gValeHouseEventsAfterLeaving;
    return gValeHouseEvents;
}

void Villager_AskWhySukuretaCame(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgHaidiaSukuretaCameToStudyMt);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 15, 6);
    Engine_EventAskYesNo(15, 0);
    Engine_EventEnd();
}

void Villager_PlanToScareVisitors(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgHaidiaLetsScareSukuretasVisitors);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 19, 6);
    Engine_EventAskYesNo(19, 0);
    Engine_EventEnd();
}

void Scene_GiveFarewellHerb(void)
{
    s32 callback;
    s32 gift;

    Engine_EventBegin();
    if (GameFlag_IsSet(FLAG_GOT_FAREWELL_HERB) != 0) {
        Engine_EventSetMessage((s32)MsgHaidiaYouMustSaveJasmine);
        Event_ShowMessage(20, 0);
        callback = (s32)gValeFaceTargetScript;
        Call3(Object_SetTargetAndCallback, 20, 0x10000, callback);
    } else {
        gift = (s32)MsgHaidiaThisIsMyFarewellGift;
        Engine_EventSetMessage(gift);
        Event_ShowMessageAndWait(20, 0, 20);
        Engine_MessageShowCentered((gift + 1), 1);
        Engine_PartyGiveItem(ITEM_HERB, 0);
        GameFlag_Set(FLAG_GOT_FAREWELL_HERB);
    }
    Engine_EventEnd();
}

void Villager_AskAboutStrangePowers(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgHaidiaTheCulpritsHadStrangePowers);
    Engine_EventAskYesNo(16, 0);
    Engine_EventEnd();
}

void Villager_AskAboutDora(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgHaidiaDidYouHearAboutDora);
    Engine_EventAskYesNo(10, 0);
    Engine_EventEnd();
}

void HaidiaHeya_TalkHopeDidntGetSick(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgHaidiaHopeDidntGet);
    Engine_EventShowMessage(0x800b, 0);
    Engine_EventEnd();
}

void HaidiaHeya_RequestWindowExit(s32 value)
{
    if (Engine_GameFlagIsSet(0x834) != 0)
        BattleFx_SetBlock30ValuesMaxZero();
    Engine_AudioPlayCue(123);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    gEventWork->transition_frames = 16;
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventRequestExit(value);
}

void FieldScene_RunIndexedStep1(void)
{
    HaidiaHeya_RequestWindowExit(1);
}

void FieldScene_RunIndexedStep2(void)
{
    HaidiaHeya_RequestWindowExit(2);
}

void FieldScene_RunIndexedStep3(void)
{
    HaidiaHeya_RequestWindowExit(3);
}

void FieldScene_RunIndexedStep4(void)
{
    HaidiaHeya_RequestWindowExit(4);
}

void FieldScene_RunIndexedStep5(void)
{
    HaidiaHeya_RequestWindowExit(5);
}

void FieldScene_RunIndexedStep6(void)
{
    HaidiaHeya_RequestWindowExit(6);
}

void FieldScene_RunIndexedStep7(void)
{
    HaidiaHeya_RequestWindowExit(7);
}

void FieldScene_RunIndexedStep8(void)
{
    HaidiaHeya_RequestWindowExit(8);
}

s32 Scene_RunSupplementalSequenceOne(void)
{
    extern u8 *gWork;

    struct FieldActor *record;
    u8 **scene = &gWork;

    *(s32 *)(scene[0] + 448) = 521;
    if (Engine_GameFlagIsSet(0x834) != 0) {
        Engine_ActorSetPosition(8, 0, 0);
        Engine_ActorSetPosition(9, 0, 0);
        Engine_ActorSetPosition(10, 0, 0);
        Engine_ActorSetPosition(11, 0, 0);
        Engine_ActorSetPosition(12, 0, 0);
        Engine_ActorSetPosition(13, 0, 0);
        Engine_ActorSetPosition(14, 0, 0);
        Engine_ActorSetPosition(15, 0, 0);
        Engine_ActorSetPosition(16, 0, 0);
        Engine_ActorSetPosition(17, 0, 0);
        Engine_ActorSetPosition(18, 0, 0);
        Engine_ActorSetPosition(19, 0, 0);
        Engine_ActorSetPosition(20, 0, 0);
        Engine_ActorSetPosition(21, 0, 0);
        Engine_ActorSetPosition(22, 0, 0);
        BattleFx_StartTwelveFrameBlend();
        ((struct FieldBlendWork *)scene[3])->loud = 1;
        BattleFx_SetBlock30Values12Zero();
        Engine_TaskWait(30);
        Engine_EventOpenScreen();
        Engine_EventWaitForScreen();
        BattleFx_SetBlock30Values128One();
    }
    if (Engine_GameFlagIsSet(0x87a) != 0) {
        if (gGameState.entrance == 6) {
            if (Engine_GameFlagIsSet(0x81d) == 0) {
                FieldScene_RunLongPresentationSequence();
            }
        }
        Object_GetById(10)->collision_flags |= 0x80;
    }
    if (gGameState.entrance == 2) {
        if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0) {
            Engine_ActorSetPosition(13, 0x1c60000, 0x960000);
            record = Object_GetById(13);
            Engine_ActorSetSpriteFlags((s32)record, 0);
            Engine_ActorSetAnimation(13, 5);
            Map_ClearLayerEntryFlag(4);
        }
    }
    return 0;
}

void FieldScene_RunByActorDirectionAndFlags(void)
{

    struct FieldActor *p;
    u32 dir;

    p = Object_GetById(ACTOR_PARTY_LEADER);
    dir = p->facing;
    dir += 0xffff5fff;

    if (dir <= 0x3ffe) {
        Engine_ShopOpen(1, 21);
        return;
    }

    Engine_EventBegin();
    if (Engine_GameFlagIsSet(0x87a) != 0) {
        Engine_EventSetMessage((s32)MsgHaidiaYouCameBack);
        Engine_EventAskYesNo(21, 0);
    } else {
        if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0) {
            Engine_EventSetMessage((s32)MsgHaidiaValeFeelsEmpty);
        } else {
            Engine_EventSetMessage((s32)MsgHaidiaGoodWeaponsDrawOutStrength);
        }
        Engine_EventShowMessage(21, 0);
    }
    Engine_EventEnd();
}

void HaidiaHeya_OpenArmorShop(void)
{
    struct FieldActor *actor;

    actor = Object_GetById(ACTOR_PARTY_LEADER);
    if ((u32)(actor->facing - 0xa001) <= 0x3ffe) {
        Engine_ShopOpen(2, 22);
    } else {
        Engine_EventBegin();
        if (Engine_GameFlagIsSet(0x87a) != 0) {
            Engine_EventSetMessage((s32)MsgHaidiaTheRumorWasTrue);
        } else {
            if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0) {
                Engine_EventSetMessage((s32)MsgHaidiaImNotSadJustGo);
            } else {
                Engine_EventSetMessage((s32)MsgHaidiaGoodArmorDrawsOutStrength);
            }
        }
        Engine_EventShowMessage(22, 0);
        Engine_EventEnd();
    }
}

void HaidiaHeya_OpenItemShop(void)
{
    struct FieldActor *actor;

    actor = Object_GetById(ACTOR_PARTY_LEADER);
    if ((u32)(actor->facing - 0xa001) <= 0x3ffe) {
        Engine_ShopOpen(3, 20);
    } else {
        if (Engine_GameFlagIsSet(0x87a) != 0) {
            Engine_EventBegin();
            Engine_EventSetMessage((s32)MsgHaidiaWhenDidYouComeBack);
            Engine_EventShowMessage(20, 0);
            Engine_EventEnd();
        } else {
            if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0) {
                Scene_GiveFarewellHerb();
            } else {
                Engine_EventBegin();
                Engine_EventSetMessage((s32)MsgHaidiaAWiseManFleesWhen);
                Engine_EventShowMessage(20, 0);
                Engine_EventEnd();
            }
        }
    }
}

void FieldScene_RunLongPresentationSequence(void)
{
    void Camera_MoveTo();

    u32 i;
    struct FieldActor *record;
    struct FieldActor *actor;
    s32 v6;
    s32 base7_20090c1;
    s32 base5_20092fc;
    s32 base5_2009400;

    Engine_EventBegin();
    Camera_MoveTo(-1, -1, -1, 0);
    Engine_TaskWait(1);
    Engine_ActorSetSpritePriority(ACTOR_MIA, 1);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x6666, 0x3333);
    Actor_SetSpeed(ACTOR_GERALD, 0x6666, 0x3333);
    Actor_SetSpeed(ACTOR_IVAN, 0x6666, 0x3333);
    Actor_SetSpeed(ACTOR_MIA, 0x6666, 0x3333);
    Engine_ActorSetAnimation(8, 5);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 808, 508);
    record = Actor_Get(23);
    Engine_ActorSetSpriteFlags(record, 0);
    record = Actor_Get(24);
    Engine_ActorSetSpriteFlags(record, 0);
    record = Actor_Get(25);
    Engine_ActorSetSpriteFlags(record, 0);
    v6 = 0;
    Object_GetById(23)->motion_flags = v6;
    Object_GetById(24)->motion_flags = v6;
    Object_GetById(25)->motion_flags = v6;
    base7_20090c1 = (s32)Scene_UpdateTimedActor;
    ((void (*)())Engine_TaskAddCallback)(base7_20090c1, 3200);
    Engine_TaskWait(1);
    gEventWork->transition_frames = 32;
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
    actor = Actor_Get(ACTOR_PARTY_LEADER);
    if (actor != NULL) {
        Actor_SetPosition(ACTOR_GERALD, actor->x.fixed, actor->z.fixed);
    }
    actor = Actor_Get(ACTOR_PARTY_LEADER);
    if (actor != NULL) {
        Actor_SetPosition(ACTOR_IVAN, actor->x.fixed, actor->z.fixed);
    }
    actor = Actor_Get(ACTOR_PARTY_LEADER);
    if (actor != NULL) {
        Actor_SetPosition(ACTOR_MIA, actor->x.fixed, actor->z.fixed);
    }
    Actor_WalkTo(ACTOR_GERALD, 792, 512);
    Actor_WalkTo(ACTOR_IVAN, 824, 504);
    Actor_WalkToAndWait(ACTOR_MIA, 818, 524);
    Engine_ActorSetAnimation(ACTOR_GERALD, 1);
    Engine_ActorSetAnimation(ACTOR_IVAN, 1);
    Engine_EventWait(10);
    base5_20092fc = (s32)gValeFaceTargetScript;
    Call3(Object_SetTargetAndCallback, 0, 0x1000a, base5_20092fc);
    Call3(Object_SetTargetAndCallback, 1, 0x1000a, base5_20092fc);
    Call3(Object_SetTargetAndCallback, 2, 0x1000a, base5_20092fc);
    Object_SetTargetAndCallback(3, 0x1000a, base5_20092fc);
    Engine_EventWait(300);
    ((struct FieldActor *)Battle_GetWorkObject1e0())->motion_flags = v6;
    Camera_SetSpeed(0x1999, 0x333);
    Camera_MoveTo(0x3120000, 0, 0x1ae0000, 1);
    Engine_EventWait(240);
    Engine_ActorStop(10);
    Actor_ShowEmote(10, 258, 80);
    Actor_WalkToAndWait(10, 819, 405);
    Engine_EventWait(40);
    Engine_ActorSetAnimationAndWait(10, 4);
    Engine_EventWait(40);
    Actor_FaceDirection(10, 0xd000, 20);
    Engine_EventSetMessage((s32)MsgHaidiaHey2);
    Event_ShowMessageAndWait(0x900a, 0, 20);
    Engine_ActorStop(ACTOR_PARTY_LEADER);
    Engine_ActorStop(ACTOR_GERALD);
    Engine_ActorStop(ACTOR_IVAN);
    Engine_ActorStop(ACTOR_MIA);
    Actor_ShowEmote(11, 256, 40);
    Event_ShowMessageAndWait(0x200b, 0, 20);
    Engine_ActorRunRepeatedMotion(10, 2);
    Engine_EventWait(40);
    Event_ShowMessageAndWait(0x900a, 0, 10);
    Actor_FaceDirection(11, 0x5000, 10);
    Event_ShowMessageAndWait(0x200b, 0, 40);
    Engine_ActorRunRepeatedMotion(10, 2);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(0x900a, 0, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 258, 80);
    Actor_ShowEmote(11, 262, 40);
    Event_ShowMessageAndWait(0x200b, 0, 40);
    Engine_ActorStartRepeatedMotion(10, 2);
    Actor_ShowEmote(10, 258, 20);
    Engine_ActorSetAnimation(10, 4);
    Event_ShowMessageAndWait(0x900a, 0, 10);
    Engine_ActorStartRepeatedMotion(11, 1);
    Engine_ActorSetAnimationAndWait(11, 3);
    Engine_EventWait(20);
    Engine_ActorStartRepeatedMotion(10, 1);
    Engine_ActorSetAnimationAndWait(10, 4);
    Engine_ActorStartRepeatedMotion(11, 1);
    Engine_ActorSetAnimationAndWait(11, 3);
    Engine_ActorStartRepeatedMotion(10, 1);
    Engine_ActorSetAnimationAndWait(10, 4);
    Actor_ShowEmote(9, 261, 0);
    Engine_ActorRunRepeatedMotion(9, 1);
    Engine_EventWait(20);
    Actor_FaceDirection(9, 0x1000, 40);
    Engine_ActorRunRepeatedMotion(9, 2);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(9, 3);
    Engine_EventWait(40);
    Event_ShowMessageAndWait(0x4009, 0, 40);
    Engine_ActorSetAnimation(11, 0);
    Engine_ActorRunRepeatedMotion(11, 2);
    Event_ShowMessageAndWait(0x200b, 0, 10);
    Engine_ActorSetAnimationAndWait(9, 4);
    Engine_ActorRunRepeatedMotion(9, 2);
    Event_ShowMessageAndWait(0x4009, 0, 10);
    Actor_ShowEmote(10, 256, 20);
    Actor_FaceDirection(10, 0x5000, 40);
    Engine_ActorSetAnimationAndWait(10, 3);
    Event_ShowMessageAndWait(0x400a, 0, 10);
    Engine_ActorSetAnimationAndWait(9, 4);
    Actor_FaceDirection(9, 0xd000, 10);
    Engine_ActorJump(9, 2, 0);
    Engine_ActorSetAnimation(9, 4);
    Event_ShowMessageAndWait(0x4009, 0, 10);
    Actor_ShowEmote(11, 257, 0);
    Actor_ShowEmote(10, 257, 40);
    Actor_FaceDirection(10, 0xd000, 80);
    Actor_FaceDirection(10, 0x5000, 60);
    Engine_ActorStartRepeatedMotion(10, 2);
    Engine_ActorStartRepeatedMotion(11, 2);
    Event_ShowTwoMessagesAndWait(10, 11, 6, 6, 6, 11, 12, 1, 7, 1, v6);
    Engine_EventWait(20);
    Camera_SetSpeed(0x19999, 0x3333);
    Camera_MoveTo(0x3090000, 0, 0x1d40000, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(40);
    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 3);
    Event_ShowMessageAndWait(0x1001, 0, 20);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_TaskRemoveCallback((void (*)(void))base7_20090c1);
    Engine_EventWait(40);
    Engine_ActorSetAnimationAndWait(8, 6);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(0x4008, 0, 20);
    Camera_MoveTo(0x2ee0000, 0, 0x1c30000, 1);
    Engine_EventWait(20);
    Actor_FaceDirection(11, 0x5000, 0);
    Actor_FaceDirection(10, 0x5000, 10);
    Actor_FaceDirection(8, 0x1000, 40);
    Actor_ShowEmote(8, 256, 40);
    Actor_FaceDirection(8, 0x3000, 20);
    Actor_FaceDirection(8, 0x1000, 20);
    Actor_FaceDirection(8, 0x3000, 40);
    Engine_ActorSetAnimationAndWait(8, 6);
    Engine_EventWait(60);
    Engine_ActorJump(8, 6, 0);
    Event_ShowMessageAndWait(0x4008, 0, 20);
    Actor_SetSpeed(ACTOR_GERALD, 0x19999, 0xcccc);
    Actor_WalkToAndWait(ACTOR_GERALD, 789, 473);
    Actor_FaceDirection(ACTOR_GERALD, 0x7000, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 3);
    Event_ShowMessageAndWait(0x4001, 0, 10);
    Actor_FaceDirection(8, 0x1000, 20);
    Engine_ActorSetAnimationAndWait(8, 3);
    Event_OpenMessage(0x4008, 0);
    Actor_FaceDirection(10, 0x5000, 0);
    Actor_FaceDirection(9, 0x1000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x3000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0x7000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xb000, 0);
    if (Engine_EventChooseYesNo(0, 0) == 1) {
        gEventWork->message++;
    }
    Camera_MoveTo(0x3090000, 0, 0x1ac0000, 1);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(10, 2);
    Event_ShowMessage(10, 0);
    Engine_ActorSetAnimationAndWait(11, 4);
    Engine_EventWait(20);
    Engine_EventSetMessage((s32)MsgHaidiaYoureLeavingAgainSoon);
    Event_ShowMessage(0x200b, 0);
    Camera_MoveTo(0x3090000, 0, 0x1d40000, 1);
    Engine_EventWait(20);
    Actor_FaceDirection(ACTOR_GERALD, 0xd000, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 3);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(9, 4);
    Actor_FaceDirection(9, 0xd000, 10);
    Event_ShowMessage(0x4009, 0);
    Engine_ActorSetAnimationAndWait(8, 3);
    Event_ShowMessage(0x4008, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x7000, 10);
    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 3);
    Actor_FaceDirection(9, 0x1000, 10);
    Engine_ActorSetAnimation(11, 3);
    Engine_ActorSetAnimation(10, 3);
    Engine_ActorSetAnimation(9, 3);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(20);
    Actor_FaceDirection(ACTOR_GERALD, 0x3000, 20);
    Actor_ShowEmote(ACTOR_GERALD, 258, 80);
    Actor_FaceDirection(ACTOR_GERALD, 0x7000, 20);
    Event_ShowMessageAndWait(0x4001, 0, 20);
    Actor_FaceDirection(ACTOR_GERALD, 0x3000, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 40);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 3);
    Engine_EventWait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_MIA, 3);
    Engine_EventWait(20);
    Actor_FaceDirection(ACTOR_GERALD, 0x1000, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xe000, 0);
    Actor_SetSpeed(ACTOR_IVAN, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_IVAN, 819, 489);
    Actor_FaceDirection(ACTOR_IVAN, 0xb000, 40);
    Engine_ActorRunRepeatedMotion(ACTOR_IVAN, 2);
    Event_ShowMessageAndWait(ACTOR_IVAN, 0, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_IVAN, 3);
    Engine_ActorSetAnimation(8, 3);
    Engine_ActorSetAnimation(9, 3);
    Engine_ActorSetAnimation(10, 3);
    Engine_ActorSetAnimationAndWait(9, 3);
    Object_GetById(3)->priority_flags &= 254;
    Engine_ActorSetSpritePriority(ACTOR_MIA, 1);
    Actor_SetSpeed(ACTOR_MIA, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_MIA, 794, 520);
    Actor_FaceDirection(ACTOR_GERALD, 0x5000, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xa000, 0);
    Actor_WalkToAndWait(ACTOR_MIA, 784, 496);
    Actor_FaceDirection(ACTOR_MIA, 0x9000, 10);
    Object_GetById(3)->priority_flags |= 1;
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Engine_ActorSetAnimation(8, 3);
    Engine_ActorSetAnimation(9, 3);
    Engine_ActorSetAnimation(10, 3);
    Engine_ActorSetAnimationAndWait(9, 3);
    Engine_EventWait(20);
    Camera_MoveTo(0x3090000, 0, 0x1ac0000, 1);
    Engine_EventWait(20);
    Actor_SetSpeed(11, 0x6666, 0x3333);
    Actor_WalkToAndWait(11, 835, 388);
    Actor_FaceDirection(11, 0x5000, 0);
    Actor_ShowEmote(11, 264, 40);
    Event_ShowMessageAndWait(0x200b, 0, 20);
    Camera_MoveTo(0x3090000, 0, 0x1d40000, 1);
    Engine_EventWait(40);
    Actor_FaceDirection(ACTOR_IVAN, 0x7000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xf000, 40);
    Actor_FaceDirection(ACTOR_IVAN, 0x9000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xd000, 20);
    Engine_ActorSetAnimation(ACTOR_IVAN, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_MIA, 3);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(10, 1);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(10, 3);
    Event_ShowMessageAndWait(10, 0, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xd000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xb000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xd000, 40);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(ACTOR_GERALD, 3);
    Engine_ActorSetAnimation(ACTOR_IVAN, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_MIA, 3);
    Engine_EventWait(20);
    Actor_SetSpeed(ACTOR_IVAN, 0x10000, 0x8000);
    base5_2009400 = (s32)gValePartyScript;
    Engine_ActorEnableActionCallback(ACTOR_GERALD, base5_2009400);
    Engine_ActorEnableActionCallback(2, base5_2009400);
    Object_SetActionCallbackAndRefreshById(3, base5_2009400);
    Engine_ActorEnableActionCallback(10, (s32)gValeActor10Script);
    Actor_WalkToAndWait(11, 837, 376);
    Actor_FaceDirection(11, 0xd000, 20);
    GameFlag_Set(0x81d);
    Engine_EventEnd();
}

void Scene_UpdateTimedActor(void)
{
    s32 no;
    u32 phase;
    union SceneActor *actor;
    s32 *other;

    phase = gFrameCount % 180;
    no = 23;
    switch (phase) {
    case 10:
        break;
    case 20:
        no = 24;
        break;
    case 30:
        no = 25;
        break;
    default:
        return;
    }
    actor = Object_GetById(no);
    if (actor == NULL) {
        return;
    }
    other = Object_GetById(8);
    if (other != NULL) {
        Engine_ActorSetPosition(no, other[2], other[4]);
    }
    actor->words[6] = 0x6666;
    actor->words[7] = 0x6666;
    {
        s32 y = actor->words[3] + 0x180000;
        union SceneField *dst = (union SceneField *)(actor->halfwords + 50);
        s32 value;
        actor->words[3] = y;
        actor->words[15] = y;
        value = 25;
        dst->value = value;
        dst++;
        value = 128;
        dst->value = value;
    }
    Engine_ActorEnableActionCallback(no, gValeTimedActorScript);
}

#include "TIMED_EVENTS.H"
#include "CALL.H"
#include "FXBLEND.H"

extern u8 MsgHaidiaDidYouHearAboutDora[];
extern u8 MsgHaidiaLetsScareSukuretasVisitors[];
extern u8 MsgHaidiaSukuretaCameToStudyMt[];
extern u8 MsgHaidiaTheCulpritsHadStrangePowers[];
extern u8 MsgHaidiaThisIsMyFarewellGift[];
extern u8 MsgHaidiaYouMustSaveJasmine[];

extern u8 MsgHaidiaHopeDidntGet[];

extern u8 MsgHaidiaAWiseManFleesWhen[];
extern u8 MsgHaidiaGoodArmorDrawsOutStrength[];
extern u8 MsgHaidiaGoodWeaponsDrawOutStrength[];
extern u8 MsgHaidiaHey2[];
extern u8 MsgHaidiaImNotSadJustGo[];
extern u8 MsgHaidiaTheRumorWasTrue[];
extern u8 MsgHaidiaValeFeelsEmpty[];
extern u8 MsgHaidiaWhenDidYouComeBack[];
extern u8 MsgHaidiaYouCameBack[];
extern u8 MsgHaidiaYoureLeavingAgainSoon[];

s32 OverlayObject_UpdateFacingTowardTarget(struct FacingObject *obj)
{
    s32 delta;
    u16 old;
    s32 angle;
    struct FacingObject *target;

    target = obj->facing_target;
    if (target != NULL) {
        obj->facing_flags = (u8)(0xFE & obj->facing_flags);
        angle = (u16)ArcTan2(target->position_z - obj->position_z, target->position_x - obj->position_x);
        old = obj->facing;
        delta = (s16)(angle - old);
        if (delta != 0) {
            if (delta > 0x1000) {
                delta = 0x1000;
            }
            /* The loader relocates the stored pool word to -0x1000. */
            if (delta < -0x1000) {
                delta = -0x1000;
            }
            obj->facing = (u16)(old + delta);
        }
    }
    return 1;
}

s32 AdvancePositionScaleAndVelocity(ScaledMotion *motion)
{
    motion->x += motion->velocity_x << 8;
    motion->y += motion->velocity_y << 8;
    motion->scale_x += 0x666;
    motion->scale_y += 0x666;
    motion->velocity_x += 5;
    motion->velocity_y -= 1;
    return 0;
}

/* The eight-byte owner includes its one pool word. */
void *SceneData_GetTable9478(void)
{
    return gValeHouseEntrances;
}

/* A four-byte leaf that returns zero. */
int SceneData_ReturnZero(void)
{
    return 0;
}

void *SceneData_SelectTable9568ByFlag(void)
{
    if (GameFlag_IsSet(0x834) != 0)
        return gValeHouseLateExits;
    return gValeHouseExits;
}

void *SceneData_SelectFlaggedTable(void)
{
    void *tbl;

    if (GameFlag_IsSet(0x87a)) {
        tbl = gValeHouseReturnPlacements;
    } else if (GameFlag_IsSet(FLAG_PARTY_LEFT_VALE)) {
        tbl = gValeHousePlacementsAfterLeaving;
    } else {
        tbl = gValeHousePlacements;
    }
    SceneEvents_UpdateInView(tbl);
    return tbl;
}

void *SceneData_SelectTable9c00ByFlags(void)
{
    if (GameFlag_IsSet(0x834) != 0)
        return gValeHouseLateEvents;
    if (GameFlag_IsSet(0x87a) != 0)
        return gValeHouseReturnEvents;
    if (GameFlag_IsSet(FLAG_PARTY_LEFT_VALE) != 0)
        return gValeHouseEventsAfterLeaving;
    return gValeHouseEvents;
}

/* The 44-byte actor-15 scene owner includes its one pool word. */
void Villager_AskWhySukuretaCame(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgHaidiaSukuretaCameToStudyMt);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 15, 6);
    Event_AskYesNo(15, 0);
    Engine_EventEnd();
}

/* The 44-byte actor-19 scene owner includes its one pool word. */
void Villager_PlanToScareVisitors(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgHaidiaLetsScareSukuretasVisitors);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 19, 6);
    Event_AskYesNo(19, 0);
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

/* The 32-byte actor-16 dialogue owner includes its one pool word. */
void Villager_AskAboutStrangePowers(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgHaidiaTheCulpritsHadStrangePowers);
    Event_AskYesNo(16, 0);
    Engine_EventEnd();
}

/* The 32-byte actor-10 dialogue owner includes its one pool word. */
void Villager_AskAboutDora(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgHaidiaDidYouHearAboutDora);
    Event_AskYesNo(10, 0);
    Engine_EventEnd();
}

/* A villager hopes the party did not get sick on its travels. */
void HaidiaHeya_TalkHopeDidntGetSick(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgHaidiaHopeDidntGet);
    Event_ShowMessage(0x800b, 0);
    Engine_EventEnd();
}

void SceneState_SetRuntimeWord448To521AndRun(s32 value)
{
    if (GameFlag_IsSet(0x834) != 0)
        BattleFx_SetBlock30ValuesMaxZero();
    Audio_PlayCue(123);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    gEventWork->transition_frames = 16;
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventRequestExit(value);
}

/* Eight numbered-scene wrappers follow, each a twelve-byte owner. */
void FieldScene_RunIndexedStep1(void)
{
    SceneState_SetRuntimeWord448To521AndRun(1);
}

void FieldScene_RunIndexedStep2(void)
{
    SceneState_SetRuntimeWord448To521AndRun(2);
}

void FieldScene_RunIndexedStep3(void)
{
    SceneState_SetRuntimeWord448To521AndRun(3);
}

void FieldScene_RunIndexedStep4(void)
{
    SceneState_SetRuntimeWord448To521AndRun(4);
}

void FieldScene_RunIndexedStep5(void)
{
    SceneState_SetRuntimeWord448To521AndRun(5);
}

void FieldScene_RunIndexedStep6(void)
{
    SceneState_SetRuntimeWord448To521AndRun(6);
}

void FieldScene_RunIndexedStep7(void)
{
    SceneState_SetRuntimeWord448To521AndRun(7);
}

void FieldScene_RunIndexedStep8(void)
{
    SceneState_SetRuntimeWord448To521AndRun(8);
}

s32 Scene_RunSupplementalSequenceOne(void)
{
    extern u8 *gWork;

    u8 *record;
    u8 **scene = (u8 **)&gWork;

    *(s32 *)(scene[0] + 0x1c0) = 0x209;
    if (GameFlag_IsSet(0x834) != 0) {
        Actor_SetPosition(8, 0, 0);
        Actor_SetPosition(9, 0, 0);
        Actor_SetPosition(10, 0, 0);
        Actor_SetPosition(11, 0, 0);
        Actor_SetPosition(12, 0, 0);
        Actor_SetPosition(13, 0, 0);
        Actor_SetPosition(14, 0, 0);
        Actor_SetPosition(15, 0, 0);
        Engine_ActorSetPosition(16, 0, 0);
        Actor_SetPosition(17, 0, 0);
        Actor_SetPosition(18, 0, 0);
        Actor_SetPosition(19, 0, 0);
        Actor_SetPosition(20, 0, 0);
        Actor_SetPosition(21, 0, 0);
        Actor_SetPosition(22, 0, 0);
        BattleFx_StartTwelveFrameBlend();
        ((struct FieldBlendWork *)scene[3])->loud = 1;
        BattleFx_SetBlock30Values12Zero();
        Engine_TaskWait(30);
        Engine_EventOpenScreen();
        Engine_EventWaitForScreen();
        BattleFx_SetBlock30Values128One();
    }
    if (GameFlag_IsSet(0x87a) != 0) {
        if (gGameState.entrance == 6) {
            if (GameFlag_IsSet(0x81d) == 0) {
                FieldScene_RunLongPresentationSequence();
            }
        }
        Actor_Get(10)->collision_flags |= 0x80;
    }
    if (gGameState.entrance == 2) {
        if (GameFlag_IsSet(FLAG_PARTY_LEFT_VALE) != 0) {
            Actor_SetPosition(13, 0x1c60000, 0x960000);
            record = Actor_Get(13);
            Engine_ActorSetSpriteFlags((s32)record, 0);
            Engine_ActorSetAnimation(13, 5);
            Map_ClearLayerEntryFlag(4);
        }
    }
    return 0;
}

void FieldScene_RunByActorDirectionAndFlags(void)
{

    u8 *p;
    u32 dir;

    p = Actor_Get(ACTOR_PARTY_LEADER);
    dir = *(u16 *)(p + 6);
    dir += 0xffff5fff;

    if (dir <= 0x3ffe) {
        Engine_ShopOpen(1, 21);
        return;
    }

    Engine_EventBegin();
    if (GameFlag_IsSet(0x87a) != 0) {
        Engine_EventSetMessage((s32)MsgHaidiaYouCameBack);
        Event_AskYesNo(21, 0);
    } else {
        if (GameFlag_IsSet(FLAG_PARTY_LEFT_VALE) != 0) {
            Engine_EventSetMessage((s32)MsgHaidiaValeFeelsEmpty);
        } else {
            Engine_EventSetMessage((s32)MsgHaidiaGoodWeaponsDrawOutStrength);
        }
        Event_ShowMessage(21, 0);
    }
    Engine_EventEnd();
}

void FieldScene_RunScene376_0200055c(void)
{
    struct FieldActor *actor;

    actor = Actor_Get(ACTOR_PARTY_LEADER);
    if ((u32)(actor->facing - 0xa001) <= 0x3ffe) {
        Engine_ShopOpen(2, 22);
    } else {
        Engine_EventBegin();
        if (GameFlag_IsSet(0x87a) != 0) {
            Engine_EventSetMessage((s32)MsgHaidiaTheRumorWasTrue);
        } else {
            if (GameFlag_IsSet(FLAG_PARTY_LEFT_VALE) != 0) {
                Engine_EventSetMessage((s32)MsgHaidiaImNotSadJustGo);
            } else {
                Engine_EventSetMessage((s32)MsgHaidiaGoodArmorDrawsOutStrength);
            }
        }
        Event_ShowMessage(22, 0);
        Engine_EventEnd();
    }
}

void FieldScene_RunScene376_020005d4(void)
{
    struct FieldActor *actor;

    actor = Actor_Get(ACTOR_PARTY_LEADER);
    if ((u32)(actor->facing - 0xa001) <= 0x3ffe) {
        Engine_ShopOpen(3, 20);
    } else {
        if (GameFlag_IsSet(0x87a) != 0) {
            Engine_EventBegin();
            Engine_EventSetMessage((s32)MsgHaidiaWhenDidYouComeBack);
            Event_ShowMessage(20, 0);
            Engine_EventEnd();
        } else {
            if (GameFlag_IsSet(FLAG_PARTY_LEFT_VALE) != 0) {
                Scene_GiveFarewellHerb();
            } else {
                Engine_EventBegin();
                Engine_EventSetMessage((s32)MsgHaidiaAWiseManFleesWhen);
                Event_ShowMessage(20, 0);
                Engine_EventEnd();
            }
        }
    }
}

void FieldScene_RunLongPresentationSequence(void)
{
    void Camera_MoveTo();

    u32 i;
    s32 record;
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
    Actor_WalkTo(ACTOR_PARTY_LEADER, 0x328, 0x1fc);
    record = Actor_Get(23);
    Engine_ActorSetSpriteFlags(record, 0);
    record = Actor_Get(24);
    Engine_ActorSetSpriteFlags(record, 0);
    record = Actor_Get(25);
    Engine_ActorSetSpriteFlags(record, 0);
    v6 = 0;
    *((u8 *)Object_GetById(23) + 85) = v6;
    *((u8 *)Object_GetById(24) + 85) = v6;
    *((u8 *)Object_GetById(25) + 85) = v6;
    base7_20090c1 = (s32)Scene_UpdateTimedActor;
    ((void (*)())Engine_TaskAddCallback)(base7_20090c1, 0xc80);
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
    Actor_WalkTo(ACTOR_GERALD, 0x318, 0x200);
    Actor_WalkTo(ACTOR_IVAN, 0x338, 0x1f8);
    Actor_WalkToAndWait(ACTOR_MIA, 0x332, 0x20c);
    Engine_ActorSetAnimation(ACTOR_GERALD, 1);
    Engine_ActorSetAnimation(ACTOR_IVAN, 1);
    Engine_EventWait(10);
    base5_20092fc = (s32)gValeFaceTargetScript;
    Call3(Object_SetTargetAndCallback, 0, 0x1000a, base5_20092fc);
    Call3(Object_SetTargetAndCallback, 1, 0x1000a, base5_20092fc);
    Call3(Object_SetTargetAndCallback, 2, 0x1000a, base5_20092fc);
    Object_SetTargetAndCallback(3, 0x1000a, base5_20092fc);
    Engine_EventWait(0x12c);
    *(u8 *)(Battle_GetWorkObject1e0() + 85) = v6;
    Camera_SetSpeed(0x1999, 0x333);
    Camera_MoveTo(0x3120000, 0, 0x1ae0000, 1);
    Engine_EventWait(240);
    Engine_ActorStop(10);
    Actor_ShowEmote(10, 0x102, 80);
    Actor_WalkToAndWait(10, 0x333, 0x195);
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
    Actor_ShowEmote(11, 0x100, 40);
    Event_ShowMessageAndWait(0x200b, 0, 20);
    Engine_ActorRunRepeatedMotion(10, 2);
    Engine_EventWait(40);
    Event_ShowMessageAndWait(0x900a, 0, 10);
    Actor_FaceDirection(11, 0x5000, 10);
    Event_ShowMessageAndWait(0x200b, 0, 40);
    Engine_ActorRunRepeatedMotion(10, 2);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(0x900a, 0, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 80);
    Actor_ShowEmote(11, 0x106, 40);
    Event_ShowMessageAndWait(0x200b, 0, 40);
    Engine_ActorStartRepeatedMotion(10, 2);
    Actor_ShowEmote(10, 0x102, 20);
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
    Actor_ShowEmote(9, 0x105, 0);
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
    Actor_ShowEmote(10, 0x100, 20);
    Actor_FaceDirection(10, 0x5000, 40);
    Engine_ActorSetAnimationAndWait(10, 3);
    Event_ShowMessageAndWait(0x400a, 0, 10);
    Engine_ActorSetAnimationAndWait(9, 4);
    Actor_FaceDirection(9, 0xd000, 10);
    Engine_ActorJump(9, 2, 0);
    Engine_ActorSetAnimation(9, 4);
    Event_ShowMessageAndWait(0x4009, 0, 10);
    Actor_ShowEmote(11, 0x101, 0);
    Actor_ShowEmote(10, 0x101, 40);
    Actor_FaceDirection(10, 0xd000, 80);
    Actor_FaceDirection(10, 0x5000, 60);
    Engine_ActorStartRepeatedMotion(10, 2);
    Engine_ActorStartRepeatedMotion(11, 2);
    Call11(Engine_EventShowTwoMessagesAndWait, 10, 11, 6, 6, 6, 11, 12, 1, 7, 1, v6);
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
    Actor_ShowEmote(8, 0x100, 40);
    Actor_FaceDirection(8, 0x3000, 20);
    Actor_FaceDirection(8, 0x1000, 20);
    Actor_FaceDirection(8, 0x3000, 40);
    Engine_ActorSetAnimationAndWait(8, 6);
    Engine_EventWait(60);
    Engine_ActorJump(8, 6, 0);
    Event_ShowMessageAndWait(0x4008, 0, 20);
    Actor_SetSpeed(ACTOR_GERALD, 0x19999, 0xcccc);
    Actor_WalkToAndWait(ACTOR_GERALD, 0x315, 0x1d9);
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
        bump_step(1);
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
    Actor_ShowEmote(ACTOR_GERALD, 0x102, 80);
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
    Actor_WalkToAndWait(ACTOR_IVAN, 0x333, 0x1e9);
    Actor_FaceDirection(ACTOR_IVAN, 0xb000, 40);
    Engine_ActorRunRepeatedMotion(ACTOR_IVAN, 2);
    Event_ShowMessageAndWait(ACTOR_IVAN, 0, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_IVAN, 3);
    Engine_ActorSetAnimation(8, 3);
    Engine_ActorSetAnimation(9, 3);
    Engine_ActorSetAnimation(10, 3);
    Engine_ActorSetAnimationAndWait(9, 3);
    *((u8 *)Object_GetById(3) + 35) &= 254;
    Engine_ActorSetSpritePriority(ACTOR_MIA, 1);
    Actor_SetSpeed(ACTOR_MIA, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_MIA, 0x31a, 0x208);
    Actor_FaceDirection(ACTOR_GERALD, 0x5000, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xa000, 0);
    Actor_WalkToAndWait(ACTOR_MIA, 0x310, 0x1f0);
    Actor_FaceDirection(ACTOR_MIA, 0x9000, 10);
    *((u8 *)Object_GetById(3) + 35) |= 1;
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Engine_ActorSetAnimation(8, 3);
    Engine_ActorSetAnimation(9, 3);
    Engine_ActorSetAnimation(10, 3);
    Engine_ActorSetAnimationAndWait(9, 3);
    Engine_EventWait(20);
    Camera_MoveTo(0x3090000, 0, 0x1ac0000, 1);
    Engine_EventWait(20);
    Actor_SetSpeed(11, 0x6666, 0x3333);
    Actor_WalkToAndWait(11, 0x343, 0x184);
    Actor_FaceDirection(11, 0x5000, 0);
    Actor_ShowEmote(11, 0x108, 40);
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
    Actor_WalkToAndWait(11, 0x345, 0x178);
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
    actor = Actor_Get(no);
    if (actor == NULL) {
        return;
    }
    other = Actor_Get(8);
    if (other != NULL) {
        Actor_SetPosition(no, other[2], other[4]);
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

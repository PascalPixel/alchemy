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

void *SceneData_SelectTable9c00ByFlags(void)
{
    if (Engine_GameFlagIsSet(0x834) != 0)
        return gValeHouseLateEvents;
    if (Engine_GameFlagIsSet(0x87a) != 0)
        return gValeHouseReturnEvents;
    if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0)
        return gValeHouseEventsAfterLeaving;
    return gValeHouseEvents;
}

/* The 44-byte actor-15 scene owner includes its one pool word. */
void Villager_AskWhySukuretaCame(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgHaidiaSukuretaCameToStudyMt);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 15, 6);
    Engine_EventAskYesNo(15, 0);
    Engine_EventEnd();
}

/* The 44-byte actor-19 scene owner includes its one pool word. */
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
    if (Engine_GameFlagIsSet(FLAG_GOT_FAREWELL_HERB) != 0) {
        Engine_EventSetMessage((s32)MsgHaidiaYouMustSaveJasmine);
        Engine_EventShowMessage(20, 0);
        callback = (s32)gValeFaceTargetScript;
        Call3(Object_SetTargetAndCallback, 20, 0x10000, callback);
    } else {
        gift = (s32)MsgHaidiaThisIsMyFarewellGift;
        Engine_EventSetMessage(gift);
        Engine_EventShowMessageAndWait(20, 0, 20);
        Engine_MessageShowCentered((gift + 1), 1);
        Engine_PartyGiveItem(ITEM_HERB, 0);
        Engine_GameFlagSet(FLAG_GOT_FAREWELL_HERB);
    }
    Engine_EventEnd();
}

/* The 32-byte actor-16 dialogue owner includes its one pool word. */
void Villager_AskAboutStrangePowers(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgHaidiaTheCulpritsHadStrangePowers);
    Engine_EventAskYesNo(16, 0);
    Engine_EventEnd();
}

/* The 32-byte actor-10 dialogue owner includes its one pool word. */
void Villager_AskAboutDora(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgHaidiaDidYouHearAboutDora);
    Engine_EventAskYesNo(10, 0);
    Engine_EventEnd();
}

/* A villager hopes the party did not get sick on its travels. */
void HaidiaHeya_TalkHopeDidntGetSick(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgHaidiaHopeDidntGet);
    Engine_EventShowMessage(0x800b, 0);
    Engine_EventEnd();
}

void SceneState_SetRuntimeWord448To521AndRun(s32 value)
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

    u8 *p;
    u32 dir;

    p = Object_GetById(ACTOR_PARTY_LEADER);
    dir = *(u16 *)(p + 6);
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

void FieldScene_RunScene376_0200055c(void)
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

void FieldScene_RunScene376_020005d4(void)
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
    s32 record;
    struct FieldActor *actor;
    s32 v6;
    s32 base7_20090c1;
    s32 base5_20092fc;
    s32 base5_2009400;

    Engine_EventBegin();
    Engine_CameraMoveTo(-1, -1, -1, 0);
    Engine_TaskWait(1);
    Engine_ActorSetSpritePriority(ACTOR_MIA, 1);
    Engine_ActorSetSpeed(ACTOR_PARTY_LEADER, 0x6666, 0x3333);
    Engine_ActorSetSpeed(ACTOR_GERALD, 0x6666, 0x3333);
    Engine_ActorSetSpeed(ACTOR_IVAN, 0x6666, 0x3333);
    Engine_ActorSetSpeed(ACTOR_MIA, 0x6666, 0x3333);
    Engine_ActorSetAnimation(8, 5);
    Engine_ActorWalkTo(ACTOR_PARTY_LEADER, 0x328, 0x1fc);
    record = Object_GetById(23);
    Engine_ActorSetSpriteFlags(record, 0);
    record = Object_GetById(24);
    Engine_ActorSetSpriteFlags(record, 0);
    record = Object_GetById(25);
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
    actor = Object_GetById(ACTOR_PARTY_LEADER);
    if (actor != NULL) {
        Engine_ActorSetPosition(ACTOR_GERALD, actor->x.fixed, actor->z.fixed);
    }
    actor = Object_GetById(ACTOR_PARTY_LEADER);
    if (actor != NULL) {
        Engine_ActorSetPosition(ACTOR_IVAN, actor->x.fixed, actor->z.fixed);
    }
    actor = Object_GetById(ACTOR_PARTY_LEADER);
    if (actor != NULL) {
        Engine_ActorSetPosition(ACTOR_MIA, actor->x.fixed, actor->z.fixed);
    }
    Engine_ActorWalkTo(ACTOR_GERALD, 0x318, 0x200);
    Engine_ActorWalkTo(ACTOR_IVAN, 0x338, 0x1f8);
    Engine_ActorWalkToAndWait(ACTOR_MIA, 0x332, 0x20c);
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
    Engine_CameraSetSpeed(0x1999, 0x333);
    Engine_CameraMoveTo(0x3120000, 0, 0x1ae0000, 1);
    Engine_EventWait(240);
    Engine_ActorStop(10);
    Engine_ActorShowEmote(10, 0x102, 80);
    Engine_ActorWalkToAndWait(10, 0x333, 0x195);
    Engine_EventWait(40);
    Engine_ActorSetAnimationAndWait(10, 4);
    Engine_EventWait(40);
    Engine_ActorFaceDirection(10, 0xd000, 20);
    Engine_EventSetMessage((s32)MsgHaidiaHey2);
    Engine_EventShowMessageAndWait(0x900a, 0, 20);
    Engine_ActorStop(ACTOR_PARTY_LEADER);
    Engine_ActorStop(ACTOR_GERALD);
    Engine_ActorStop(ACTOR_IVAN);
    Engine_ActorStop(ACTOR_MIA);
    Engine_ActorShowEmote(11, 0x100, 40);
    Engine_EventShowMessageAndWait(0x200b, 0, 20);
    Engine_ActorRunRepeatedMotion(10, 2);
    Engine_EventWait(40);
    Engine_EventShowMessageAndWait(0x900a, 0, 10);
    Engine_ActorFaceDirection(11, 0x5000, 10);
    Engine_EventShowMessageAndWait(0x200b, 0, 40);
    Engine_ActorRunRepeatedMotion(10, 2);
    Engine_EventWait(20);
    Engine_EventShowMessageAndWait(0x900a, 0, 20);
    Engine_ActorShowEmote(ACTOR_PARTY_LEADER, 0x102, 80);
    Engine_ActorShowEmote(11, 0x106, 40);
    Engine_EventShowMessageAndWait(0x200b, 0, 40);
    Engine_ActorStartRepeatedMotion(10, 2);
    Engine_ActorShowEmote(10, 0x102, 20);
    Engine_ActorSetAnimation(10, 4);
    Engine_EventShowMessageAndWait(0x900a, 0, 10);
    Engine_ActorStartRepeatedMotion(11, 1);
    Engine_ActorSetAnimationAndWait(11, 3);
    Engine_EventWait(20);
    Engine_ActorStartRepeatedMotion(10, 1);
    Engine_ActorSetAnimationAndWait(10, 4);
    Engine_ActorStartRepeatedMotion(11, 1);
    Engine_ActorSetAnimationAndWait(11, 3);
    Engine_ActorStartRepeatedMotion(10, 1);
    Engine_ActorSetAnimationAndWait(10, 4);
    Engine_ActorShowEmote(9, 0x105, 0);
    Engine_ActorRunRepeatedMotion(9, 1);
    Engine_EventWait(20);
    Engine_ActorFaceDirection(9, 0x1000, 40);
    Engine_ActorRunRepeatedMotion(9, 2);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(9, 3);
    Engine_EventWait(40);
    Engine_EventShowMessageAndWait(0x4009, 0, 40);
    Engine_ActorSetAnimation(11, 0);
    Engine_ActorRunRepeatedMotion(11, 2);
    Engine_EventShowMessageAndWait(0x200b, 0, 10);
    Engine_ActorSetAnimationAndWait(9, 4);
    Engine_ActorRunRepeatedMotion(9, 2);
    Engine_EventShowMessageAndWait(0x4009, 0, 10);
    Engine_ActorShowEmote(10, 0x100, 20);
    Engine_ActorFaceDirection(10, 0x5000, 40);
    Engine_ActorSetAnimationAndWait(10, 3);
    Engine_EventShowMessageAndWait(0x400a, 0, 10);
    Engine_ActorSetAnimationAndWait(9, 4);
    Engine_ActorFaceDirection(9, 0xd000, 10);
    Engine_ActorJump(9, 2, 0);
    Engine_ActorSetAnimation(9, 4);
    Engine_EventShowMessageAndWait(0x4009, 0, 10);
    Engine_ActorShowEmote(11, 0x101, 0);
    Engine_ActorShowEmote(10, 0x101, 40);
    Engine_ActorFaceDirection(10, 0xd000, 80);
    Engine_ActorFaceDirection(10, 0x5000, 60);
    Engine_ActorStartRepeatedMotion(10, 2);
    Engine_ActorStartRepeatedMotion(11, 2);
    Call11(Engine_EventShowTwoMessagesAndWait, 10, 11, 6, 6, 6, 11, 12, 1, 7, 1, v6);
    Engine_EventWait(20);
    Engine_CameraSetSpeed(0x19999, 0x3333);
    Engine_CameraMoveTo(0x3090000, 0, 0x1d40000, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(40);
    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 3);
    Engine_EventShowMessageAndWait(0x1001, 0, 20);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_TaskRemoveCallback((void (*)(void))base7_20090c1);
    Engine_EventWait(40);
    Engine_ActorSetAnimationAndWait(8, 6);
    Engine_EventWait(20);
    Engine_EventShowMessageAndWait(0x4008, 0, 20);
    Engine_CameraMoveTo(0x2ee0000, 0, 0x1c30000, 1);
    Engine_EventWait(20);
    Engine_ActorFaceDirection(11, 0x5000, 0);
    Engine_ActorFaceDirection(10, 0x5000, 10);
    Engine_ActorFaceDirection(8, 0x1000, 40);
    Engine_ActorShowEmote(8, 0x100, 40);
    Engine_ActorFaceDirection(8, 0x3000, 20);
    Engine_ActorFaceDirection(8, 0x1000, 20);
    Engine_ActorFaceDirection(8, 0x3000, 40);
    Engine_ActorSetAnimationAndWait(8, 6);
    Engine_EventWait(60);
    Engine_ActorJump(8, 6, 0);
    Engine_EventShowMessageAndWait(0x4008, 0, 20);
    Engine_ActorSetSpeed(ACTOR_GERALD, 0x19999, 0xcccc);
    Engine_ActorWalkToAndWait(ACTOR_GERALD, 0x315, 0x1d9);
    Engine_ActorFaceDirection(ACTOR_GERALD, 0x7000, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 3);
    Engine_EventShowMessageAndWait(0x4001, 0, 10);
    Engine_ActorFaceDirection(8, 0x1000, 20);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventOpenMessage(0x4008, 0);
    Engine_ActorFaceDirection(10, 0x5000, 0);
    Engine_ActorFaceDirection(9, 0x1000, 0);
    Engine_ActorFaceDirection(ACTOR_GERALD, 0x3000, 0);
    Engine_ActorFaceDirection(ACTOR_IVAN, 0x7000, 0);
    Engine_ActorFaceDirection(ACTOR_MIA, 0xb000, 0);
    if (Engine_EventChooseYesNo(0, 0) == 1) {
        bump_step(1);
    }
    Engine_CameraMoveTo(0x3090000, 0, 0x1ac0000, 1);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(10, 2);
    Engine_EventShowMessage(10, 0);
    Engine_ActorSetAnimationAndWait(11, 4);
    Engine_EventWait(20);
    Engine_EventSetMessage((s32)MsgHaidiaYoureLeavingAgainSoon);
    Engine_EventShowMessage(0x200b, 0);
    Engine_CameraMoveTo(0x3090000, 0, 0x1d40000, 1);
    Engine_EventWait(20);
    Engine_ActorFaceDirection(ACTOR_GERALD, 0xd000, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 3);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(9, 4);
    Engine_ActorFaceDirection(9, 0xd000, 10);
    Engine_EventShowMessage(0x4009, 0);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventShowMessage(0x4008, 0);
    Engine_ActorFaceDirection(ACTOR_GERALD, 0x7000, 10);
    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 3);
    Engine_ActorFaceDirection(9, 0x1000, 10);
    Engine_ActorSetAnimation(11, 3);
    Engine_ActorSetAnimation(10, 3);
    Engine_ActorSetAnimation(9, 3);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(20);
    Engine_ActorFaceDirection(ACTOR_GERALD, 0x3000, 20);
    Engine_ActorShowEmote(ACTOR_GERALD, 0x102, 80);
    Engine_ActorFaceDirection(ACTOR_GERALD, 0x7000, 20);
    Engine_EventShowMessageAndWait(0x4001, 0, 20);
    Engine_ActorFaceDirection(ACTOR_GERALD, 0x3000, 10);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, 0, 40);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 3);
    Engine_EventWait(20);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, 0x4000, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_MIA, 3);
    Engine_EventWait(20);
    Engine_ActorFaceDirection(ACTOR_GERALD, 0x1000, 0);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, 0xe000, 0);
    Engine_ActorSetSpeed(ACTOR_IVAN, 0x10000, 0x8000);
    Engine_ActorWalkToAndWait(ACTOR_IVAN, 0x333, 0x1e9);
    Engine_ActorFaceDirection(ACTOR_IVAN, 0xb000, 40);
    Engine_ActorRunRepeatedMotion(ACTOR_IVAN, 2);
    Engine_EventShowMessageAndWait(ACTOR_IVAN, 0, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_IVAN, 3);
    Engine_ActorSetAnimation(8, 3);
    Engine_ActorSetAnimation(9, 3);
    Engine_ActorSetAnimation(10, 3);
    Engine_ActorSetAnimationAndWait(9, 3);
    *((u8 *)Object_GetById(3) + 35) &= 254;
    Engine_ActorSetSpritePriority(ACTOR_MIA, 1);
    Engine_ActorSetSpeed(ACTOR_MIA, 0x10000, 0x8000);
    Engine_ActorWalkToAndWait(ACTOR_MIA, 0x31a, 0x208);
    Engine_ActorFaceDirection(ACTOR_GERALD, 0x5000, 0);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, 0xa000, 0);
    Engine_ActorWalkToAndWait(ACTOR_MIA, 0x310, 0x1f0);
    Engine_ActorFaceDirection(ACTOR_MIA, 0x9000, 10);
    *((u8 *)Object_GetById(3) + 35) |= 1;
    Engine_EventShowMessageAndWait(ACTOR_MIA, 0, 20);
    Engine_ActorSetAnimation(8, 3);
    Engine_ActorSetAnimation(9, 3);
    Engine_ActorSetAnimation(10, 3);
    Engine_ActorSetAnimationAndWait(9, 3);
    Engine_EventWait(20);
    Engine_CameraMoveTo(0x3090000, 0, 0x1ac0000, 1);
    Engine_EventWait(20);
    Engine_ActorSetSpeed(11, 0x6666, 0x3333);
    Engine_ActorWalkToAndWait(11, 0x343, 0x184);
    Engine_ActorFaceDirection(11, 0x5000, 0);
    Engine_ActorShowEmote(11, 0x108, 40);
    Engine_EventShowMessageAndWait(0x200b, 0, 20);
    Engine_CameraMoveTo(0x3090000, 0, 0x1d40000, 1);
    Engine_EventWait(40);
    Engine_ActorFaceDirection(ACTOR_IVAN, 0x7000, 0);
    Engine_ActorFaceDirection(ACTOR_MIA, 0xf000, 40);
    Engine_ActorFaceDirection(ACTOR_IVAN, 0x9000, 0);
    Engine_ActorFaceDirection(ACTOR_MIA, 0xd000, 20);
    Engine_ActorSetAnimation(ACTOR_IVAN, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_MIA, 3);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(10, 1);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(10, 3);
    Engine_EventShowMessageAndWait(10, 0, 20);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Engine_ActorFaceDirection(ACTOR_GERALD, 0xd000, 0);
    Engine_ActorFaceDirection(ACTOR_IVAN, 0xb000, 0);
    Engine_ActorFaceDirection(ACTOR_MIA, 0xd000, 40);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(ACTOR_GERALD, 3);
    Engine_ActorSetAnimation(ACTOR_IVAN, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_MIA, 3);
    Engine_EventWait(20);
    Engine_ActorSetSpeed(ACTOR_IVAN, 0x10000, 0x8000);
    base5_2009400 = (s32)gValePartyScript;
    Engine_ActorEnableActionCallback(ACTOR_GERALD, base5_2009400);
    Engine_ActorEnableActionCallback(2, base5_2009400);
    Object_SetActionCallbackAndRefreshById(3, base5_2009400);
    Engine_ActorEnableActionCallback(10, (s32)gValeActor10Script);
    Engine_ActorWalkToAndWait(11, 0x345, 0x178);
    Engine_ActorFaceDirection(11, 0xd000, 20);
    Engine_GameFlagSet(0x81d);
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

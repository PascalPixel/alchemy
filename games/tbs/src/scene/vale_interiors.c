#include "game_state.h"
#include "unit/import.h"
#include "unit/scene/vale_interiors.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "unit/runtime/runtime.h"
#include "vale_interiors.h"
#include "call.h"
#include "fxblend.h"
#include "field_event.h"
#include "field_service.h"
#include "text/msg_ids.h"
#include "types.h"
#include "ability_ids.h"
#include "field_scene.h"
#include "item_ids.h"
#include "objcmd.h"
#include "facing_object.h"

// 前方宣言
static void FieldScene_RunLongPresentationSequence(void);
static void Scene_UpdateTimedActor(void);

// ここに共通ソースが入る（ROM の並びのとおり）
#include "common/object_facing.h"

static s32 AdvancePositionScaleAndVelocity(ScaledMotion *motion) {
  motion->x += motion->velocityX << 8;
  motion->y += motion->velocityY << 8;
  motion->scaleX += 0x666;
  motion->scaleY += 0x666;
  motion->velocityX += 5;
  motion->velocityY -= 1;
  return 0;
}

void *ValeInteriors_GetEntrances(void) { return gValeHouseEntrances; }

int SceneData_ReturnZero(void) { return 0; }

void *ValeInteriors_SelectExits(void) {
  if (Engine_GameFlagIsSet(0x834) != 0)
    return gValeHouseLateExits;
  return gValeHouseExits;
}

void *SceneData_SelectFlaggedTable(void) {
  void *table;

  if (Engine_GameFlagIsSet(0x87a)) {
    table = gValeHouseReturnPlacements;
  } else if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE)) {
    table = gValeHousePlacementsAfterLeaving;
  } else {
    table = gValeHousePlacements;
  }
  SceneEvents_UpdateInView(table);
  return table;
}

void *ValeInteriors_SelectEvents(void) {
  if (Engine_GameFlagIsSet(0x834) != 0)
    return gValeHouseLateEvents;
  if (Engine_GameFlagIsSet(0x87a) != 0)
    return gValeHouseReturnEvents;
  if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0)
    return gValeHouseEventsAfterLeaving;
  return gValeHouseEvents;
}

static void EV15_TALK(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgValeKradenCameToStudyMt);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 15, 6);
  Engine_EventAskYesNo(15, 0);
  Engine_EventEnd();
}

static void EV19_TALK(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgValeLetsScareKradensVisitors);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 19, 6);
  Engine_EventAskYesNo(19, 0);
  Engine_EventEnd();
}

static void Scene_GiveFarewellHerb(void) {
  s32 gift;

  Engine_EventBegin();
  if (GameFlag_IsSet(FLAG_GOT_FAREWELL_HERB) != 0) {
    Engine_EventSetMessage(MsgValeYouMustSaveJenna);
    Event_ShowMessage(20, 0);
    FieldObject_SetTargetAndCallback(20, 0x10000, gValeFaceTargetScript);
  } else {
    gift = MsgValeThisIsMyFarewellGift;
    Engine_EventSetMessage(gift);
    Event_ShowMessageAndWait(20, 0, 20);
    Engine_MessageShowCentered((gift + 1), 1);
    Engine_PartyGiveItem(ITEM_HERB, 0);
    GameFlag_Set(FLAG_GOT_FAREWELL_HERB);
  }
  Engine_EventEnd();
}

static void EV16_TALK(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgValeTheCulpritsHadStrangePowers);
  Engine_EventAskYesNo(16, 0);
  Engine_EventEnd();
}

static void EV10_TALK(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgValeDidYouHearAboutDora);
  Engine_EventAskYesNo(10, 0);
  Engine_EventEnd();
}

static void EV11_TALK(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgValeHopeDidntGet);
  Engine_EventShowMessage(0x800b, 0);
  Engine_EventEnd();
}

static void ValeInteriors_RequestWindowExit(s32 value) {
  if (Engine_GameFlagIsSet(0x834) != 0)
    FieldFx_StopLightning();
  Engine_AudioPlayCue(123);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  gEventWork->transitionFrames = 16;
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(value);
}

static void EV01_TOUCH(void) { ValeInteriors_RequestWindowExit(1); }

static void EV02_TOUCH(void) { ValeInteriors_RequestWindowExit(2); }

static void EV03_TOUCH(void) { ValeInteriors_RequestWindowExit(3); }

static void EV04_TOUCH(void) { ValeInteriors_RequestWindowExit(4); }

static void EV05_TOUCH(void) { ValeInteriors_RequestWindowExit(5); }

static void EV06_TOUCH(void) { ValeInteriors_RequestWindowExit(6); }

static void EV07_TOUCH(void) { ValeInteriors_RequestWindowExit(7); }

static void EV08_TOUCH(void) { ValeInteriors_RequestWindowExit(8); }

s32 Scene_RunSupplementalSequenceOne(void) {

  struct FieldActor *record;
  u8 **scene = (u8 **)&gEventWork;

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
    FieldFx_StartLightning();
    ((struct FieldBlendWork *)scene[3])->loud = 1;
    FieldFx_LastLightning();
    Engine_TaskWait(30);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    FieldFx_ResumeLightning();
  }
  if (Engine_GameFlagIsSet(0x87a) != 0) {
    if (gGameState.entrance == 6) {
      if (Engine_GameFlagIsSet(0x81d) == 0) {
        FieldScene_RunLongPresentationSequence();
      }
    }
    Object_GetById(10)->collisionFlags |= 0x80;
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

static void EV21_TALK(void) {

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
    Engine_EventSetMessage(MsgValeYouCameBack);
    Engine_EventAskYesNo(21, 0);
  } else {
    if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0) {
      Engine_EventSetMessage(MsgValeValeFeelsEmpty);
    } else {
      Engine_EventSetMessage(MsgValeGoodWeaponsDrawOutStrength);
    }
    Engine_EventShowMessage(21, 0);
  }
  Engine_EventEnd();
}

static void EV22_TALK(void) {
  struct FieldActor *actor;

  actor = Object_GetById(ACTOR_PARTY_LEADER);
  if ((u32)(actor->facing - 0xa001) <= 0x3ffe) {
    Engine_ShopOpen(2, 22);
  } else {
    Engine_EventBegin();
    if (Engine_GameFlagIsSet(0x87a) != 0) {
      Engine_EventSetMessage(MsgValeTheRumorWasTrue);
    } else {
      if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0) {
        Engine_EventSetMessage(MsgValeImNotSadJustGo);
      } else {
        Engine_EventSetMessage(MsgValeGoodArmorDrawsOutStrength);
      }
    }
    Engine_EventShowMessage(22, 0);
    Engine_EventEnd();
  }
}

static void EV20_TALK(void) {
  struct FieldActor *actor;

  actor = Object_GetById(ACTOR_PARTY_LEADER);
  if ((u32)(actor->facing - 0xa001) <= 0x3ffe) {
    Engine_ShopOpen(3, 20);
  } else {
    if (Engine_GameFlagIsSet(0x87a) != 0) {
      Engine_EventBegin();
      Engine_EventSetMessage(MsgValeWhenDidYouComeBack);
      Engine_EventShowMessage(20, 0);
      Engine_EventEnd();
    } else {
      if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0) {
        Scene_GiveFarewellHerb();
      } else {
        Engine_EventBegin();
        Engine_EventSetMessage(MsgValeAWiseManFleesWhen);
        Engine_EventShowMessage(20, 0);
        Engine_EventEnd();
      }
    }
  }
}

static void FieldScene_RunLongPresentationSequence(void) {
  void Camera_MoveTo();

  struct FieldActor *record;
  struct FieldActor *actor;
  s32 v6;
  SchedulerCallback timedUpdate;
  const s32 *partyScript;

  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_TaskWait(1);
  Engine_ActorSetSpritePriority(MIA, 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x6666, 0x3333);
  Actor_SetSpeed(GARET, 0x6666, 0x3333);
  Actor_SetSpeed(IVAN, 0x6666, 0x3333);
  Actor_SetSpeed(MIA, 0x6666, 0x3333);
  Engine_ActorSetAnimation(8, 5);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 0x328, 0x1fc);
  record = Actor_Get(23);
  Engine_ActorSetSpriteFlags(record, 0);
  record = Actor_Get(24);
  Engine_ActorSetSpriteFlags(record, 0);
  record = Actor_Get(25);
  Engine_ActorSetSpriteFlags(record, 0);
  v6 = 0;
  Object_GetById(23)->motionFlags = v6;
  Object_GetById(24)->motionFlags = v6;
  Object_GetById(25)->motionFlags = v6;
  timedUpdate = Scene_UpdateTimedActor;
  FieldScheduler_AddCallback(timedUpdate, 0xc80);
  Engine_TaskWait(1);
  gEventWork->transitionFrames = 32;
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetPosition(GARET, actor->x.fixed, actor->z.fixed);
  }
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetPosition(IVAN, actor->x.fixed, actor->z.fixed);
  }
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetPosition(MIA, actor->x.fixed, actor->z.fixed);
  }
  Actor_WalkTo(GARET, 0x318, 0x200);
  Actor_WalkTo(IVAN, 0x338, 0x1f8);
  Actor_WalkToAndWait(MIA, 0x332, 0x20c);
  Engine_ActorSetAnimation(GARET, 1);
  Engine_ActorSetAnimation(IVAN, 1);
  Engine_EventWait(10);
  FieldObject_SetTargetAndCallback(0, 0x1000a, gValeFaceTargetScript);
  FieldObject_SetTargetAndCallback(1, 0x1000a, gValeFaceTargetScript);
  FieldObject_SetTargetAndCallback(2, 0x1000a, gValeFaceTargetScript);
  FieldObject_SetTargetAndCallback(3, 0x1000a, gValeFaceTargetScript);
  Engine_EventWait(0x12c);
  Battle_GetWorkObject1e0()->motionFlags = v6;
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
  Engine_EventSetMessage(MsgValeHey2);
  Event_ShowMessageAndWait(0x900a, 0, 20);
  Engine_ActorStop(ACTOR_PARTY_LEADER);
  Engine_ActorStop(GARET);
  Engine_ActorStop(IVAN);
  Engine_ActorStop(MIA);
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
  Event_ShowTwoMessagesAndWait(10, 11, 6, 6, 6, 11, 12, 1, 7, 1, v6);
  Engine_EventWait(20);
  Camera_SetSpeed(0x19999, 0x3333);
  Camera_MoveTo(0x3090000, 0, 0x1d40000, 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(40);
  Engine_ActorSetAnimationAndWait(GARET, 3);
  Event_ShowMessageAndWait(0x1001, 0, 20);
  Engine_ActorRunRepeatedMotion(8, 2);
  Engine_TaskRemoveCallback((void (*)(void))timedUpdate);
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
  Actor_SetSpeed(GARET, 0x19999, 0xcccc);
  Actor_WalkToAndWait(GARET, 0x315, 0x1d9);
  Actor_FaceDirection(GARET, 0x7000, 20);
  Engine_ActorSetAnimationAndWait(GARET, 3);
  Event_ShowMessageAndWait(0x4001, 0, 10);
  Actor_FaceDirection(8, 0x1000, 20);
  Engine_ActorSetAnimationAndWait(8, 3);
  Event_OpenMessage(0x4008, 0);
  Actor_FaceDirection(10, 0x5000, 0);
  Actor_FaceDirection(9, 0x1000, 0);
  Actor_FaceDirection(GARET, 0x3000, 0);
  Actor_FaceDirection(IVAN, 0x7000, 0);
  Actor_FaceDirection(MIA, 0xb000, 0);
  if (Engine_EventChooseYesNo(0, 0) == 1) {
    gEventWork->message++;
  }
  Camera_MoveTo(0x3090000, 0, 0x1ac0000, 1);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(10, 2);
  Event_ShowMessage(10, 0);
  Engine_ActorSetAnimationAndWait(11, 4);
  Engine_EventWait(20);
  Engine_EventSetMessage(MsgValeYoureLeavingAgainSoon);
  Event_ShowMessage(0x200b, 0);
  Camera_MoveTo(0x3090000, 0, 0x1d40000, 1);
  Engine_EventWait(20);
  Actor_FaceDirection(GARET, 0xd000, 20);
  Engine_ActorSetAnimationAndWait(GARET, 3);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(9, 4);
  Actor_FaceDirection(9, 0xd000, 10);
  Event_ShowMessage(0x4009, 0);
  Engine_ActorSetAnimationAndWait(8, 3);
  Event_ShowMessage(0x4008, 0);
  Actor_FaceDirection(GARET, 0x7000, 10);
  Engine_ActorSetAnimationAndWait(GARET, 3);
  Actor_FaceDirection(9, 0x1000, 10);
  Engine_ActorSetAnimation(11, 3);
  Engine_ActorSetAnimation(10, 3);
  Engine_ActorSetAnimation(9, 3);
  Engine_ActorSetAnimationAndWait(8, 3);
  Engine_EventWait(20);
  Actor_FaceDirection(GARET, 0x3000, 20);
  Actor_ShowEmote(GARET, 0x102, 80);
  Actor_FaceDirection(GARET, 0x7000, 20);
  Event_ShowMessageAndWait(0x4001, 0, 20);
  Actor_FaceDirection(GARET, 0x3000, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 40);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
  Engine_ActorSetAnimationAndWait(GARET, 3);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 20);
  Engine_ActorSetAnimationAndWait(MIA, 3);
  Engine_EventWait(20);
  Actor_FaceDirection(GARET, 0x1000, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xe000, 0);
  Actor_SetSpeed(IVAN, 0x10000, 0x8000);
  Actor_WalkToAndWait(IVAN, 0x333, 0x1e9);
  Actor_FaceDirection(IVAN, 0xb000, 40);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Event_ShowMessageAndWait(IVAN, 0, 20);
  Engine_ActorSetAnimationAndWait(IVAN, 3);
  Engine_ActorSetAnimation(8, 3);
  Engine_ActorSetAnimation(9, 3);
  Engine_ActorSetAnimation(10, 3);
  Engine_ActorSetAnimationAndWait(9, 3);
  Object_GetById(3)->priorityFlags &= 254;
  Engine_ActorSetSpritePriority(MIA, 1);
  Actor_SetSpeed(MIA, 0x10000, 0x8000);
  Actor_WalkToAndWait(MIA, 0x31a, 0x208);
  Actor_FaceDirection(GARET, 0x5000, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xa000, 0);
  Actor_WalkToAndWait(MIA, 0x310, 0x1f0);
  Actor_FaceDirection(MIA, 0x9000, 10);
  Object_GetById(3)->priorityFlags |= 1;
  Event_ShowMessageAndWait(MIA, 0, 20);
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
  Actor_FaceDirection(IVAN, 0x7000, 0);
  Actor_FaceDirection(MIA, 0xf000, 40);
  Actor_FaceDirection(IVAN, 0x9000, 0);
  Actor_FaceDirection(MIA, 0xd000, 20);
  Engine_ActorSetAnimation(IVAN, 3);
  Engine_ActorSetAnimationAndWait(MIA, 3);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(10, 1);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(10, 3);
  Event_ShowMessageAndWait(10, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
  Actor_FaceDirection(GARET, 0xd000, 0);
  Actor_FaceDirection(IVAN, 0xb000, 0);
  Actor_FaceDirection(MIA, 0xd000, 40);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
  Engine_ActorSetAnimation(GARET, 3);
  Engine_ActorSetAnimation(IVAN, 3);
  Engine_ActorSetAnimationAndWait(MIA, 3);
  Engine_EventWait(20);
  Actor_SetSpeed(IVAN, 0x10000, 0x8000);
  partyScript = gValePartyScript;
  Actor_SetActionScript(GARET, partyScript);
  Actor_SetActionScript(2, partyScript);
  Object_SetActionCallbackAndRefreshById(3, (s32)partyScript);
  Actor_SetActionScript(10, gValeActor10Script);
  Actor_WalkToAndWait(11, 0x345, 0x178);
  Actor_FaceDirection(11, 0xd000, 20);
  GameFlag_Set(0x81d);
  Engine_EventEnd();
}

static void Scene_UpdateTimedActor(void) {
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
  Actor_SetActionScript(no, gValeTimedActorScript);
}

s32 gValeFaceTargetScript[] = {
    SC_CALL(OverlayObject_UpdateFacingTowardTarget),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gValeActor10Script[] = {
    SC_SET(SW_SPEED, 0x9999),
    SC_SET(SW_ACCEL, 0x4ccc),
    SC_ARG(2),
    SC_MOVE(PIXELS(784), 0, PIXELS(405)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(40),
    SC_LABEL(1),
    SC_ADD(SW_FACING, -1024),
    SC_WAIT(1),
    SC_LOOP(16, SC_TO(1)),
    SC_WAIT(40),
    SC_LABEL(3),
    SC_ADD(SW_FACING, -1024),
    SC_WAIT(1),
    SC_LOOP(16, SC_TO(3)),
    SC_ARG(2),
    SC_MOVE(PIXELS(819), 0, PIXELS(405)),
    SC_SYNC,
    SC_ARG(1),
    SC_LABEL(2),
    SC_ADD(SW_FACING, -2048),
    SC_WAIT(1),
    SC_LOOP(16, SC_TO(2)),
    SC_WAIT(40),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gValePartyScript[] = {
    SC_ARG(2),       SC_MOVE(PIXELS(808), 0, PIXELS(508)),
    SC_SYNC,         SC_ARG(1),
    SC_POS(0, 0, 0), SC_WAIT(1),
    SC_END,
};

s32 gValeTimedActorScript[] = {
    SC_CALL(AdvancePositionScaleAndVelocity),
    SC_WAIT(1),
    SC_LOOP(60, SC_TOP),
    SC_POS(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

struct SceneEntrance gValeHouseEntrances[] = {
    {0, CONDITION_ALWAYS, 120, 0, 100, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 96, 0, 222, FACING_NORTH, 0, 4, 4, 244, 292, 0},
    {2, CONDITION_ALWAYS, 337, 0, 199, FACING_NORTH, 0, 252, 4, 548, 236, 0},
    {3, CONDITION_ALWAYS, 656, 0, 244, FACING_NORTH, 0, 556, 4, 900, 276, 0},
    {4, CONDITION_ALWAYS, 159, 0, 525, FACING_NORTH, 0, 4, 324, 332, 564, 0},
    {5, CONDITION_ALWAYS, 528, 0, 486, FACING_NORTH, 0, 348, 284, 612, 516, 0},
    {6, CONDITION_ALWAYS, 816, 0, 522, FACING_NORTH, 0, 628, 284, 884, 548, 0},
    {7, CONDITION_ALWAYS, 463, 0, 851, FACING_NORTH, 0, 324, 604, 588, 884, 0},
    {8, CONDITION_ALWAYS, 769, 0, 857, FACING_NORTH, 0, 580, 628, 852, 884, 0},
    {SCENE_TABLE_END},
};

u32 gValeHouseExits[] = {
    SCENE_EXITS(7),      SCENE_EXIT(1, 4, 10), SCENE_EXIT(2, 4, 9),
    SCENE_EXIT(3, 5, 3), SCENE_EXIT(4, 5, 5),  SCENE_EXIT(5, 4, 7),
    SCENE_EXIT(6, 4, 6), SCENE_EXIT(7, 5, 4),  SCENE_EXIT(8, 5, 1),
    SCENE_EXITS_END,
};

u32 gValeHouseLateExits[] = {
    SCENE_EXITS(7),      SCENE_EXIT(1, 3, 10), SCENE_EXIT(2, 3, 9),
    SCENE_EXIT(3, 5, 3), SCENE_EXIT(4, 5, 5),  SCENE_EXIT(5, 3, 7),
    SCENE_EXIT(6, 3, 6), SCENE_EXIT(7, 5, 4),  SCENE_EXIT(8, 5, 1),
    SCENE_EXITS_END,
};

// clang-format off
struct ScenePlacement gValeHousePlacements[] = {
    { 0x75, 0x0, ACTOR_STAND,
        PIXELS(697), 0, PIXELS(468), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6f, 0x0, ACTOR_STAND,
        PIXELS(726), 0, PIXELS(486), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x36, 0x0, ACTOR_WANDER,
        PIXELS(815), 0, PIXELS(386), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x26, 0x0, ACTOR_STAND,
        PIXELS(841), 0, PIXELS(455), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0x67, 0x0, ACTOR_STAND,
        PIXELS(403), 0, PIXELS(144), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x6f, 0x0, ACTOR_STAND,
        PIXELS(331), 0, PIXELS(144), FACING_EAST, TALK_KEEP_FACING, 0 },
    { 0x73, 0x0, ACTOR_STAND,
        PIXELS(486), 0, PIXELS(136), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x65, 0x0, ACTOR_STAND,
        PIXELS(495), 0, PIXELS(453), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x73, 0x0, ACTOR_STAND,
        PIXELS(405), 0, PIXELS(421), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6c, 0x0, ACTOR_WANDER,
        PIXELS(99), 0, PIXELS(125), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0x6a, 0x0, ACTOR_STAND,
        PIXELS(768), 0, PIXELS(768), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x74, 0x0, ACTOR_STAND,
        PIXELS(392), 0, PIXELS(697), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x76, 0x0, ACTOR_STAND,
        PIXELS(72), 0, PIXELS(424), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x7c, 0x0, ACTOR_STAND,
        PIXELS(232), 0, PIXELS(440), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x7d, 0x0, ACTOR_STAND,
        PIXELS(264), 0, PIXELS(440), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gValeHousePlacementsAfterLeaving[] = {
    { 0x75, 0x0, ACTOR_STAND,
        PIXELS(697), 0, PIXELS(468), FACING_SOUTHEAST + FACING_STEP, 3, 0 },
    { 0x6f, 0x0, ACTOR_STAND,
        PIXELS(726), 0, PIXELS(486), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x36, 0x0, ACTOR_WANDER,
        PIXELS(815), 0, PIXELS(386), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x26, 0x0, ACTOR_STAND,
        PIXELS(841), 0, PIXELS(455), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0x67, 0x0, ACTOR_STAND,
        PIXELS(403), 0, PIXELS(144), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x6f, 0x0, ACTOR_STAND,
        PIXELS(456), 0, PIXELS(156), FACING_EAST, TALK_KEEP_FACING, 0 },
    { 0x73, 0x0, ACTOR_STAND,
        PIXELS(486), 0, PIXELS(118), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0x65, 0x0, ACTOR_STAND,
        PIXELS(495), 0, PIXELS(453), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x73, 0x0, ACTOR_STAND,
        PIXELS(405), 0, PIXELS(421), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6c, 0x0, ACTOR_WANDER,
        PIXELS(99), 0, PIXELS(125), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0x6a, 0x0, ACTOR_STAND,
        PIXELS(768), 0, PIXELS(768), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x74, 0x0, ACTOR_STAND,
        PIXELS(392), 0, PIXELS(697), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x76, 0x0, ACTOR_STAND,
        PIXELS(72), 0, PIXELS(424), FACING_SOUTH, 3, 0 },
    { 0x7c, 0x0, ACTOR_STAND,
        PIXELS(232), 0, PIXELS(440), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x7d, 0x0, ACTOR_STAND,
        PIXELS(264), 0, PIXELS(440), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gValeHouseReturnPlacements[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { IVAN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { MIA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0x75, 0x0, ACTOR_STAND,
        PIXELS(697), 0, PIXELS(468), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x6f, 0x0, ACTOR_STAND,
        PIXELS(726), 0, PIXELS(486), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x36, 0x0, (s32)gValeActor10Script,
        PIXELS(819), 0, PIXELS(405), FACING_WEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x26, 0x0, ACTOR_STAND,
        PIXELS(841), 0, PIXELS(370), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x67, 0x0, ACTOR_STAND,
        PIXELS(403), 0, PIXELS(144), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x6f, 0x0, ACTOR_STAND,
        PIXELS(456), 0, PIXELS(156), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x73, 0x0, ACTOR_STAND,
        PIXELS(494), 0, PIXELS(112), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0x65, 0x0, ACTOR_STAND,
        PIXELS(495), 0, PIXELS(453), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x73, 0x0, ACTOR_STAND,
        PIXELS(405), 0, PIXELS(421), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6c, 0x0, ACTOR_WANDER,
        PIXELS(99), 0, PIXELS(125), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0x6a, 0x0, ACTOR_STAND,
        PIXELS(768), 0, PIXELS(768), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x74, 0x0, ACTOR_STAND,
        PIXELS(392), 0, PIXELS(697), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x76, 0x0, ACTOR_STAND,
        PIXELS(72), 0, PIXELS(424), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x7c, 0x0, ACTOR_STAND,
        PIXELS(232), 0, PIXELS(440), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x7d, 0x0, ACTOR_STAND,
        PIXELS(264), 0, PIXELS(440), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x118, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x118, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x118, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SCENE_TABLE_END },
};

struct SceneEvent gValeHouseLateEvents[] = {
    { EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(EV01_TOUCH) },
    { EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(EV02_TOUCH) },
    { EVENT_TOUCH, 3, CONDITION_ALWAYS, EVENT_SCRIPT(EV03_TOUCH) },
    { EVENT_TOUCH, 4, CONDITION_ALWAYS, EVENT_SCRIPT(EV04_TOUCH) },
    { EVENT_TOUCH, 5, CONDITION_ALWAYS, EVENT_SCRIPT(EV05_TOUCH) },
    { EVENT_TOUCH, 6, CONDITION_ALWAYS, EVENT_SCRIPT(EV06_TOUCH) },
    { EVENT_TOUCH, 7, CONDITION_ALWAYS, EVENT_SCRIPT(EV07_TOUCH) },
    { EVENT_TOUCH, 8, CONDITION_ALWAYS, EVENT_SCRIPT(EV08_TOUCH) },
    { SEARCH(SEARCH_BOOKCASE), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBooksAboutContinents)) },
    { SEARCH(SEARCH_BOOKCASE), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookLawsOfVale)) },
    { 0xc4f3, 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookPowerOfNature)) },
    { 0xc4f3, 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookPsynergyStone)) },
    { 0xc4f3, 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeAuntNotebook)) },
    { 0xc4f3, 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookMakingGreatVillage)) },
    { SEARCH(SEARCH_BOOKCASE), 206, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeNotebookStudyOfAlchemy)) },
    { SEARCH(SEARCH_BOOKCASE), 207, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookAncientLemurians)) },
    { SEARCH(SEARCH_BOOKCASE), 208, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookFountainOfTolbi)) },
    { SEARCH(SEARCH_BOOKCASE), 209, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeEncyclopediaOfAlchemy)) },
    { SEARCH(SEARCH_BOOKCASE), 210, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookLegendOfAleph)) },
    { SEARCH(SEARCH_BOOKCASE), 211, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookBeingAdept)) },
    { SEARCH(SEARCH_BARREL), 100, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgVaultButDidntFindAnything)) },
    { SEARCH(SEARCH_JAR), 101, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeJarEmpty)) },
    { SEARCH(SEARCH_JAR), 102, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeJarEmpty)) },
    { SEARCH(SEARCH_JAR), 103, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeJarEmpty)) },
    { SEARCH(SEARCH_JAR), 104, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeJarEmpty)) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gValeHouseEvents[] = {
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgValeAtTheVeryLeastVisitorsShould) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgValeIHearKradenHasGuestsAgain) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgValeGaretHasGrownQuiteStrongIn) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgValeYouAndGaretAreOlderNow) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgValeMyGranddaughterJennaIsFinallyHappy) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgValeIHopeYouTwoWontBe) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgValeJennaIsTheSpittingImageOf) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, EVENT_SCRIPT(EV15_TALK) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgValeIfThereAreVisitorsInTown) },
    { EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgValeDoYouEverDreamOfSeeing) },
    { EVENT_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgValeFewPeopleOutsideOfValePossess) },
    { EVENT_TALK, 19, CONDITION_ALWAYS, EVENT_SCRIPT(EV19_TALK) },
    { EVENT_TALK, 21, CONDITION_ALWAYS, EVENT_SCRIPT(EV21_TALK) },
    { EVENT_TALK, 22, CONDITION_ALWAYS, EVENT_SCRIPT(EV22_TALK) },
    { EVENT_TALK, 20, CONDITION_ALWAYS, EVENT_SCRIPT(EV20_TALK) },
    { EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(EV01_TOUCH) },
    { EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(EV02_TOUCH) },
    { EVENT_TOUCH, 3, CONDITION_ALWAYS, EVENT_SCRIPT(EV03_TOUCH) },
    { EVENT_TOUCH, 4, CONDITION_ALWAYS, EVENT_SCRIPT(EV04_TOUCH) },
    { EVENT_TOUCH, 5, CONDITION_ALWAYS, EVENT_SCRIPT(EV05_TOUCH) },
    { EVENT_TOUCH, 6, CONDITION_ALWAYS, EVENT_SCRIPT(EV06_TOUCH) },
    { EVENT_TOUCH, 7, CONDITION_ALWAYS, EVENT_SCRIPT(EV07_TOUCH) },
    { EVENT_TOUCH, 8, CONDITION_ALWAYS, EVENT_SCRIPT(EV08_TOUCH) },
    { SEARCH(SEARCH_BOOKCASE), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBooksAboutContinents)) },
    { SEARCH(SEARCH_BOOKCASE), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookLawsOfVale)) },
    { 0xc4f3, 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookPowerOfNature)) },
    { 0xc4f3, 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookPsynergyStone)) },
    { 0xc4f3, 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeAuntNotebook)) },
    { 0xc4f3, 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookMakingGreatVillage)) },
    { SEARCH(SEARCH_BARREL), 100, 0xf42, FIND_ITEM(ITEM_ANTIDOTE) },
    { SEARCH(SEARCH_JAR), 101, 0xf43, FIND_COINS(6) },
    { SEARCH(SEARCH_JAR), 102, 0xf44, FIND_COINS(2) },
    { SEARCH(SEARCH_JAR), 103, 0xf45, FIND_ITEM(ITEM_SMOKE_BOMB) },
    { SEARCH(SEARCH_JAR), 104, 0xf46, FIND_COINS(3) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gValeHouseEventsAfterLeaving[] = {
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgValeIsaacPleaseTakeCareOfMy) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgValeReceivingAQuestDirectlyFromThe) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgValeImProudOfYouGaretMy) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgValeAsMothersDoraAndIWill) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgValeIHeardThatJennaBrotherFelix) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgValePleaseReturnMyPreciousGranddaughterJenna) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgValeAfterJennaWasTakenGrandmotherGrew) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgValeItsBecauseKradenWasStudyingAlchemy) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, EVENT_SCRIPT(EV16_TALK) },
    { EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgValeImSoJealousOfYouBeing) },
    { EVENT_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgValeOnceYouLeaveTownTakeCare) },
    { EVENT_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgValeIHadNoIdeaThatSuch) },
    { EVENT_TALK, 21, CONDITION_ALWAYS, EVENT_SCRIPT(EV21_TALK) },
    { EVENT_TALK, 22, CONDITION_ALWAYS, EVENT_SCRIPT(EV22_TALK) },
    { EVENT_TALK, 20, CONDITION_ALWAYS, EVENT_SCRIPT(EV20_TALK) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgValeIsaacYoureGoingOnADifficult) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgValeIfYouFaceAnyHardshipIn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgValeSoIfThatWasFelixWho) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgValeItsHardLettingMyPreciousGaret) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgValeTheWiseOneChoseGaretHimself) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgValeOnceGaretLeavesIWontBe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgValeIAmConcernedForGaretBut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgValeFelixInheritedHisFathersSkillWith) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgValeJennaAnytimeYouFaceDangerJust) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgValeWhyWouldFelixTakeHisSister) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgValeWeAlmostLostTheTownOver) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgValeIdLikeToBelieveThatOnly) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgValeIfOnlyIdGoneToSol) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgValeValesLawsStateThatYouCannot) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgValeIWishIWereTheOne) },
    { EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(EV01_TOUCH) },
    { EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(EV02_TOUCH) },
    { EVENT_TOUCH, 3, CONDITION_ALWAYS, EVENT_SCRIPT(EV03_TOUCH) },
    { EVENT_TOUCH, 4, CONDITION_ALWAYS, EVENT_SCRIPT(EV04_TOUCH) },
    { EVENT_TOUCH, 5, CONDITION_ALWAYS, EVENT_SCRIPT(EV05_TOUCH) },
    { EVENT_TOUCH, 6, CONDITION_ALWAYS, EVENT_SCRIPT(EV06_TOUCH) },
    { EVENT_TOUCH, 7, CONDITION_ALWAYS, EVENT_SCRIPT(EV07_TOUCH) },
    { EVENT_TOUCH, 8, CONDITION_ALWAYS, EVENT_SCRIPT(EV08_TOUCH) },
    { SEARCH(SEARCH_BARREL), 100, 0xf42, FIND_ITEM(ITEM_ANTIDOTE) },
    { SEARCH(SEARCH_JAR), 101, 0xf43, FIND_COINS(6) },
    { SEARCH(SEARCH_JAR), 102, 0xf44, FIND_COINS(2) },
    { SEARCH(SEARCH_JAR), 103, 0xf45, FIND_ITEM(ITEM_SMOKE_BOMB) },
    { SEARCH(SEARCH_JAR), 104, 0xf46, FIND_COINS(3) },
    { SEARCH(SEARCH_BOOKCASE), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBooksAboutContinents)) },
    { SEARCH(SEARCH_BOOKCASE), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookLawsOfVale)) },
    { 0xc4f3, 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookPowerOfNature)) },
    { 0xc4f3, 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookPsynergyStone)) },
    { 0xc4f3, 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeAuntNotebook)) },
    { 0xc4f3, 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookMakingGreatVillage)) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gValeHouseReturnEvents[] = {
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgValeMiaIsSuchANiceGirl) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgValeThatIvanSeemsToBeA) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, EVENT_SCRIPT(EV10_TALK) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, EVENT_SCRIPT(EV11_TALK) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgValeOhSoYouStillHaventRescued) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgValeIGetOverColdsQuicklyBut) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgValeAColdIsGoingAroundI) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgValeWhatWasThatAlchemyThatKraden) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgValeEarthquakesStillStrikeOnceInA) },
    { EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgValeTheMountainIsApparentlyStillCasting) },
    { EVENT_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgValeTheNorthwestFaceOfMtAleph) },
    { EVENT_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgValeTheGreatHealersAreTooAfraid) },
    { EVENT_TALK, 21, CONDITION_ALWAYS, EVENT_SCRIPT(EV21_TALK) },
    { EVENT_TALK, 22, CONDITION_ALWAYS, EVENT_SCRIPT(EV22_TALK) },
    { EVENT_TALK, 20, CONDITION_ALWAYS, EVENT_SCRIPT(EV20_TALK) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgValeItMustBeNiceToBe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgValeHisNewFriendsAreAllSo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgValeItWasntTooSmartToTalk) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgValeGaretSeemsToHaveGrownUp) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgValeWeveSeenManyMonstersLatelyAnd) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgValeIWishICouldLearnThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, 0x81b, MSGNO(MsgValeWhatHappenedToJennaHaventYou) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgValeIGaveHimAFarewellGift) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgValeWillIEverSeeJennaAnd) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgValeDoraSaysItsJustACold) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgValeIKnowThatThereAreAlways) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgValeEvenIfTheEruptionHadNo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgValeEveryTimeTheGroundShakesI) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgValeIfThePsynergyStonesInThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgValeTheySayTheLavaLookedLike) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgValeTheHealersAreSupposedToGuard) },
    { EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(EV01_TOUCH) },
    { EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(EV02_TOUCH) },
    { EVENT_TOUCH, 3, CONDITION_ALWAYS, EVENT_SCRIPT(EV03_TOUCH) },
    { EVENT_TOUCH, 4, CONDITION_ALWAYS, EVENT_SCRIPT(EV04_TOUCH) },
    { EVENT_TOUCH, 5, CONDITION_ALWAYS, EVENT_SCRIPT(EV05_TOUCH) },
    { EVENT_TOUCH, 6, CONDITION_ALWAYS, EVENT_SCRIPT(EV06_TOUCH) },
    { EVENT_TOUCH, 7, CONDITION_ALWAYS, EVENT_SCRIPT(EV07_TOUCH) },
    { EVENT_TOUCH, 8, CONDITION_ALWAYS, EVENT_SCRIPT(EV08_TOUCH) },
    { SEARCH(SEARCH_BARREL), 100, 0xf42, FIND_ITEM(ITEM_ANTIDOTE) },
    { SEARCH(SEARCH_JAR), 101, 0xf43, FIND_COINS(6) },
    { SEARCH(SEARCH_JAR), 102, 0xf44, FIND_COINS(2) },
    { SEARCH(SEARCH_JAR), 103, 0xf45, FIND_ITEM(ITEM_SMOKE_BOMB) },
    { SEARCH(SEARCH_JAR), 104, 0xf46, FIND_COINS(3) },
    { SEARCH(SEARCH_BOOKCASE), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBooksAboutContinents)) },
    { SEARCH(SEARCH_BOOKCASE), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookLawsOfVale)) },
    { 0xc4f3, 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookPowerOfNature)) },
    { 0xc4f3, 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookPsynergyStone)) },
    { 0xc4f3, 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeAuntNotebook)) },
    { 0xc4f3, 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookMakingGreatVillage)) },
    { SCENE_EVENTS_END },
};
// clang-format on

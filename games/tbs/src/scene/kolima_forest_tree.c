#include "battle_effect_runtime.h"
#include "game_state.h"
#include "unit/scene/kolima_forest_tree.h"
#include "unit/scene/common/kolima_palette.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/game/game.h"
#include "unit/runtime/runtime.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "resource.h"
#include "kolima_forest_tree.h"
#include "types.h"
#include "call.h"
#include "field_event.h"
#include "engine_call.h"
#include "scene_ids.h"
#include "palette_scene.h"
#include "io_reg.h"
#include "field_scene.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "objcmd.h"

// 前方宣言
static void RunEventScript01(void);
static void PaletteScene_RunActorTransitionSequence(void);
static void PaletteScene_SpawnEffect(void);
static void PaletteScene_AdvanceTransition(void);
static void KolimaForestTree_PlayGesture(s32 actor, s32 gesture);
static void KolimaForestTree_SpawnOrbitSparks(void);
static void PaletteScene_SetRecordValue(s32 key, s32 value);

s32 gKolimaForestTreeSparkOrigin[3] __attribute__((section(".bss")));
s32 gKolimaForestTreeSparkCount __attribute__((section(".bss")));
s32 gKolimaForestTreeSparkSound __attribute__((section(".bss")));
s32 gKolimaForestTreeTransitionStep __attribute__((section(".bss")));

u8 *KolimaForestTree_GetEntrances(void) { return gKolimaForestTreeEntrances; }

u8 *KolimaForestTree_GetRegions(void) { return gKolimaForestTreeRegions; }

u8 *KolimaForestTree_GetExits(void) { return gKolimaForestTreeExits; }

u8 *KolimaForestTree_GetPlacements(void) { return gKolimaForestTreePlacements; }

static void PaletteScene_Initialize(void) {
  struct EventWork *scene;

  scene = gEventWork;
  Engine_EventBegin();
  Engine_ActorWalkByAndWait(ACTOR_PARTY_LEADER, 0, 0);
  Engine_EventRequestExit(scene->touchedTrigger);
  Engine_EventEnd();
}

u8 *KolimaForestTree_GetEvents(void) { return gKolimaForestTreeEvents; }

static void EV13_TALK(void) {
  Engine_EventBegin();
  KolimaForestTree_PlayGesture(11, 1);
  if (Engine_GameFlagIsSet(0x845) != 0) {
    Engine_EventSetMessage(MsgKolimaForestKolimaAlive);
    Engine_EventShowMessage(9, 0);
  } else if (GameFlag_IsSet(0x84c) != 0) {
    Engine_EventSetMessage(MsgKolimaLeaveBeforeForest);
    Engine_EventShowMessage(9, 0);
  } else {
    Engine_EventSetMessage(MsgKolimaWho);
    Engine_EventShowMessageAndWait(9, 0, 20);
    KolimaForestTree_PlayGesture(11, 0);
    Engine_EventWait(60);
    KolimaForestTree_PlayGesture(11, 1);
    Engine_EventShowMessageAndWait(9, 0, 10);
    Engine_ActorSetAnimationAndWait(0, 3);
    Engine_EventWait(40);
    Engine_EventShowMessage(9, 0);
    KolimaForestTree_PlayGesture(11, 0);
    Engine_EventWait(80);
    Engine_EventShowMessageAndWait(9, 0, 20);
    KolimaForestTree_PlayGesture(11, 1);
    Engine_EventShowMessageAndWait(9, 0, 20);
    Engine_GameFlagSet(0x84c);
  }
  KolimaForestTree_PlayGesture(11, 0);
  Engine_EventEnd();
}

static void KolimaForestTree_TalkToTret(void) {
  s32 record;

  Engine_EventBegin();
  if (GameFlag_IsSet(0x845) != 0) {
    KolimaForestTree_PlayGesture(10, 1);
    Engine_EventSetMessage(MsgKolimaNowHaveSuchPowerAxe);
    Event_ShowMessage(8, 0);
    KolimaForestTree_PlayGesture(10, 0);
  } else {
    if (GameFlag_IsSet(0x844) != 0) {
      KolimaForestTree_PlayGesture(10, 1);
      Engine_EventSetMessage(MsgKolimaSilence2);
      Event_ShowMessage(8, 0);
      KolimaForestTree_PlayGesture(10, 0);
      record = PartyInventory_FindOwner(184);
      if (record != -1) {
        u16 *target = &gEventWork->unk172;
        u16 shown = 1;

        *target = shown;
      }
    } else {
      Engine_EventSetMessage(MsgKolimaSilence);
      Event_ShowMessage(8, 0);
      ColorBuffer_ApplyTarget(0x406218, 1);
      Engine_ColorBufferInterpolate(20);
      Engine_TaskWait(40);
      Event_ShowMessageAndWait(0x200e, 0, 10);
      Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
      Event_ShowMessage(0x200e, 0);
      ColorBuffer_ApplyTarget(0x10000, 1);
      Engine_ColorBufferInterpolate(20);
      Engine_TaskWait(40);
    }
  }
  Engine_EventEnd();
}

static void EV13_PSYNERGY(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(0x845) != 0) {
    Engine_EventSetMessage(MsgKolimaMustHorribleBeyondRiverAm);
  } else {
    Engine_EventSetMessage(MsgKolimaHealingWatersMercuryLighthouseMight);
  }
  Engine_EventShowMessage(9, 0);
  Engine_EventEnd();
}

static void EV12_PSYNERGY(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(0x845) != 0) {
    Engine_EventSetMessage(MsgKolimaKnowCannotStopButPlease);
  } else {
    Engine_EventSetMessage(MsgKolimaPeopleKolimaForgiveMe);
  }
  Engine_EventShowMessage(8, 0);
  Engine_EventEnd();
}

static void EV12_SCRIPT(void) {
  Engine_EventBegin();
  Battle_ResetEffectCounter();
  if (Engine_GameFlagIsSet(0x844) == 0) {
    RunEventScript01();
  } else {
    PaletteScene_RunActorTransitionSequence();
  }
  Engine_EventEnd();
}

static void RunEventScript01(void) {

  struct FieldActor *rec8;

  rec8 = Actor_Get(ACTOR_PARTY_LEADER);
  Engine_ActorFaceDirection(0, FACING_NORTH, 0);
  ColorBuffer_ApplyTarget(0x406218, 1);
  Engine_ColorBufferInterpolate(20);
  Engine_TaskWait(40);
  Event_PlayCue(17);
  gKolimaForestTreeSparkSound = 1;
  FieldScheduler_AddCallback(PaletteScene_SpawnEffect, 0xc80);
  Engine_TaskWait(30);
  gKolimaForestTreeSparkSound = 0;
  Camera_MoveTo(0x1480000, -1, 0xeb0000, 1);
  Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 1);
  Object_GetById(0)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 16);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x20000);
  Event_PlayCue(133);
  rec8->velocityY = 0x50000;
  rec8->gravity = 0x4000;
  rec8->bounce = 0xa000;
  Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 0x14f, 0x102);
  while (rec8->velocityY >= 0) {
    Engine_TaskWait(1);
  }
  do {
    Engine_TaskWait(1);
  } while (rec8->velocityY <= 0);
  Event_PlayCue(161);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 19);
  Engine_EventWait(120);
  Engine_TaskRemoveCallback(PaletteScene_SpawnEffect);
  Engine_TaskWait(40);
  rec8->bounce = 0x4000;
  {
    struct FieldActor *record = Actor_Get(ACTOR_PARTY_LEADER);
    record->steeringFlags = ACTOR_FACES_MOTION | record->steeringFlags;
  }
  Engine_EventWait(80);
  Engine_EventSetMessage(MsgKolimaControlTretsHeartShallNot);
  Event_ShowMessageAndWait(0x200e, 0, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_EventWait(20);
  Event_ShowMessage(0x200e, 0);
  Audio_PlayCueFromEventWork();
  ColorBuffer_ApplyTarget(0x10000, 1);
  Engine_ColorBufferInterpolate(20);
  Engine_TaskWait(40);
  {
    s32 shown = 0xc000;

    rec8->facing = shown;
  }
  rec8->gravity = 0x10000;
  rec8->bounce = 0x4000;
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_EventWait(40);
  Engine_ActorJump(ACTOR_PARTY_LEADER, 4, 0);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
  Engine_EventWait(20);
}

static void PaletteScene_RunActorTransitionSequence(void) {
  s32 actorThreeEnabled;
  struct FieldActor *object;
  s32 *transitionState;
  s32 cycle;
  s32 effectCallback;
  const s32 *finalActions;

  actorThreeEnabled = GameFlag_IsSet(3);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x148, 212);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
  Event_PlayCue(17);
  Engine_MessageShowCentered(MsgKolimaWaterHermesSeepedIntoTret, 1);
  Actor_SetSpeed(GARET, 0x10000, 0x8000);
  Actor_SetSpeed(IVAN, 0x10000, 0x8000);
  object = Actor_Get(ACTOR_PARTY_LEADER);
  if (object != 0) {
    Actor_SetPosition(GARET, object->x.fixed, object->z.fixed);
  }
  object = Actor_Get(ACTOR_PARTY_LEADER);
  if (object != 0) {
    Actor_SetPosition(IVAN, object->x.fixed, object->z.fixed);
  }
  Actor_SetActionScript(GARET, gSceneAction_ActorOneEntry);
  Actor_SetActionScript(IVAN, gSceneAction_ActorTwoEntry);
  if (actorThreeEnabled != 0) {
    Actor_SetSpeed(MIA, 0x10000, 0x8000);
    object = Actor_Get(ACTOR_PARTY_LEADER);
    if (object != 0) {
      Actor_SetPosition(MIA, object->x.fixed, object->z.fixed);
    }
    Actor_SetActionScript(MIA, gSceneAction_ActorThreeEntry);
  }
  Object_RefreshSelectorById(2);
  Engine_EventWait(40);
  KolimaPalette_Restore(0);
  Engine_ColorBufferInterpolate(32);
  Engine_TaskWait(40);
  transitionState = &gKolimaForestTreeTransitionStep;
  *transitionState = 0;
  FieldScheduler_AddCallback(PaletteScene_AdvanceTransition, 0xc80);
  Engine_EventWait(40);
  Actor_FaceDirection(GARET, FACING_SOUTHWEST, 20);
  Camera_SetSpeed(0x33333, 0x6666);
  Camera_MoveTo(0x1000000, -1, 0xfe0000, 1);
  Engine_CameraWaitForMove();
  Event_PlayCue(246);
  Engine_EventWait(40);
  Actor_FaceDirection(IVAN, FACING_SOUTHEAST, 20);
  Camera_MoveTo(0x19d0000, -1, 0x1050000, 1);
  Engine_CameraWaitForMove();
  Event_PlayCue(246);
  Engine_EventWait(40);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(MIA, FACING_SOUTH, 20);
  Camera_MoveTo(0x1460000, -1, 0x1800000, 1);
  Engine_CameraWaitForMove();
  Event_PlayCue(246);
  if (*transitionState != 24) {
    do {
      Engine_TaskWait(1);
    } while (*transitionState != 24);
  }
  Engine_TaskRemoveCallback(PaletteScene_AdvanceTransition);
  Engine_TaskWait(10);
  cycle = 0;
  do {
    KolimaPalette_Restore(0);
    Engine_ColorBufferInterpolate(6);
    Engine_TaskWait(6);
    KolimaPalette_Restore(1);
    Engine_ColorBufferInterpolate(6);
    cycle = (cycle + 1);
    Engine_TaskWait(6);
  } while ((u32)cycle <= 3);
  KolimaPalette_Restore(0);
  Engine_ColorBufferInterpolate(40);
  Engine_TaskWait(80);
  Camera_MoveTo(0x1480000, 0x80000, 0xd40000, 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(40);
  KolimaForestTree_PlayGesture(10, 1);
  Engine_EventWait(40);
  Event_PlayCue(7);
  Engine_EventSetMessage(MsgKolimaFeelGreatPowerSpreadingThrough);
  Event_ShowMessage(8, 0);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorStartRepeatedMotion(MIA, 2);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 20);
  KolimaForestTree_PlayGesture(10, 2);
  Engine_EventWait(20);
  KolimaForestTree_PlayGesture(10, 3);
  Engine_EventWait(40);
  KolimaForestTree_PlayGesture(10, 1);
  Engine_EventWait(20);
  Event_ShowMessage(8, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x105, 0);
  Actor_ShowEmote(GARET, 0x105, 0);
  Actor_ShowEmote(MIA, 0x105, 0);
  Actor_ShowEmote(IVAN, 0x105, 40);
  Camera_MoveTo(0xea0000, 0, 0xe80000, 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(40);
  KolimaForestTree_PlayGesture(11, 1);
  Engine_EventWait(40);
  KolimaForestTree_PlayGesture(11, 3);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(0x4009, 0, 20);
  KolimaForestTree_PlayGesture(11, 2);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(GARET, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(IVAN, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(MIA, FACING_SOUTHWEST, 20);
  KolimaForestTree_PlayGesture(11, 3);
  Engine_EventWait(20);
  KolimaForestTree_PlayGesture(11, 2);
  Engine_EventWait(20);
  KolimaForestTree_PlayGesture(11, 3);
  Event_ShowMessageAndWait(0x4009, 0, 10);
  KolimaForestTree_PlayGesture(10, 0);
  Engine_EventWait(20);
  Event_ShowMessage(0x8008, 0);
  KolimaForestTree_PlayGesture(10, 1);
  Engine_EventWait(20);
  Event_OpenMessage(0x8008, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Event_ShowMessage(0x4009, 0);
    Event_ShowMessage(0x8008, 0);
  } else {
    gEventWork->message += 2;
    Actor_ShowEmote(MIA, 0x103, 0);
    Actor_ShowEmote(GARET, 0x103, 0);
    Actor_ShowEmote(IVAN, 0x103, 40);
    Engine_ActorSetAnimation(GARET, 4);
    Event_ShowMessage(GARET, 0);
    if (actorThreeEnabled != 0) {
      Engine_ActorRunRepeatedMotion(MIA, 2);
      Event_ShowMessage(MIA, 0);
    } else {
      gEventWork->message += 1;
    }
    Engine_ActorSetAnimationAndWait(IVAN, 3);
    Event_ShowMessage(IVAN, 0);
    Event_ShowMessage(0x4009, 0);
    Event_ShowMessage(0x8008, 0);
  }
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
  Engine_ActorSetAnimation(GARET, 3);
  Engine_ActorSetAnimation(MIA, 3);
  Engine_ActorSetAnimationAndWait(IVAN, 3);
  Camera_MoveTo(0x1480000, 0x80000, 0xd40000, 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  KolimaForestTree_PlayGesture(10, 0);
  Engine_EventWait(20);
  KolimaPalette_Restore(0);
  Engine_ColorBufferInterpolate(1);
  Engine_TaskWait(1);
  ColorBuffer_ApplyTarget(0x406218, 1);
  Engine_ColorBufferInterpolate(40);
  Engine_EventWait(60);
  gKolimaForestTreeSparkCount = 0;
  gKolimaForestTreeSparkOrigin[0] = 0x1480000;
  gKolimaForestTreeSparkOrigin[1] = 0x300000;
  effectCallback = (s32)KolimaForestTree_SpawnOrbitSparks;
  gKolimaForestTreeSparkOrigin[2] = 0xcd0000;
  FieldScheduler_AddCallback(effectCallback, 0xc80);
  Engine_EventWait(100);
  Engine_TaskRemoveCallback(effectCallback);
  ColorBuffer_ApplyTarget(0x7fff, 0);
  Engine_ColorBufferInterpolate(60);
  Engine_EventWait(100);
  KolimaPalette_Restore(0);
  Engine_ColorBufferInterpolate(20);
  Engine_EventWait(40);
  KolimaForestTree_PlayGesture(10, 1);
  Engine_EventWait(10);
  Engine_EventSetMessage(MsgKolimaShouldDoPeopleKolimaCursed);
  Event_ShowMessage(0x8008, 0);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
  Engine_ActorSetAnimation(GARET, 3);
  Engine_ActorSetAnimation(MIA, 3);
  Engine_ActorSetAnimationAndWait(IVAN, 3);
  Camera_MoveTo(0xea0000, 0, 0xe80000, 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  Event_ShowMessage(0x4009, 0);
  Event_ShowMessageAndWait(0x8008, 0, 10);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 10);
  Event_OpenMessage(GARET, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Actor_ShowEmote(GARET, 0x102, 40);
  } else {
    Engine_ActorSetAnimationAndWait(GARET, 4);
    gEventWork->message += 1;
  }
  Event_ShowMessage(GARET, 0);
  KolimaForestTree_PlayGesture(10, 4);
  Engine_EventWait(20);
  Engine_EventSetMessage(MsgKolimaWasIndeedAngryPeopleHad);
  Event_ShowMessage(0x8008, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Event_ShowMessage(0x8008, 0);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
  Engine_ActorSetAnimation(GARET, 3);
  Engine_ActorSetAnimation(MIA, 3);
  Engine_ActorSetAnimationAndWait(IVAN, 3);
  KolimaForestTree_PlayGesture(10, 4);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(0x8008, 0, 20);
  KolimaForestTree_PlayGesture(11, 0);
  Event_ShowMessageAndWait(0x4009, 0, 20);
  KolimaForestTree_PlayGesture(11, 3);
  Engine_EventWait(40);
  KolimaForestTree_PlayGesture(11, 1);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(0x4009, 0, 20);
  KolimaForestTree_PlayGesture(10, 2);
  Engine_EventWait(20);
  Event_ShowMessage(0x8008, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
  Actor_ShowEmote(GARET, 0x102, 0);
  Actor_ShowEmote(MIA, 0x102, 0);
  Actor_ShowEmote(IVAN, 0x102, 80);
  KolimaForestTree_PlayGesture(11, 5);
  Engine_EventWait(60);
  KolimaForestTree_PlayGesture(11, 3);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(0x4009, 0, 20);
  KolimaForestTree_PlayGesture(10, 5);
  Engine_EventWait(40);
  KolimaForestTree_PlayGesture(10, 2);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(0x4008, 0, 20);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Actor_FaceDirection(GARET, FACING_WEST, 10);
  Event_ShowMessage(GARET, 0);
  Actor_FaceDirection(IVAN, FACING_WEST, 20);
  Event_ShowMessage(0x8002, 0);
  KolimaForestTree_PlayGesture(11, 4);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(0x4009, 0, 20);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 10);
  Event_ShowMessageAndWait(GARET, 0, 20);
  KolimaForestTree_PlayGesture(10, 1);
  Event_ShowMessageAndWait(0x8008, 0, 10);
  KolimaForestTree_PlayGesture(10, 2);
  Engine_EventWait(20);
  KolimaForestTree_PlayGesture(11, 3);
  Engine_EventWait(40);
  KolimaForestTree_PlayGesture(11, 0);
  Engine_EventWait(20);
  KolimaPalette_Restore(0);
  Engine_ColorBufferInterpolate(1);
  Engine_TaskWait(1);
  ColorBuffer_ApplyTarget(0x406218, 1);
  Engine_ColorBufferInterpolate(40);
  Engine_EventWait(60);
  gKolimaForestTreeSparkCount = 0;
  gKolimaForestTreeSparkOrigin[0] = 0x880000;
  gKolimaForestTreeSparkOrigin[1] = 0x140000;
  effectCallback = (s32)KolimaForestTree_SpawnOrbitSparks;
  gKolimaForestTreeSparkOrigin[2] = 0x1020000;
  FieldScheduler_AddCallback(effectCallback, 0xc80);
  Engine_EventWait(100);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(GARET, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(MIA, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(IVAN, FACING_SOUTHWEST, 40);
  Engine_ActorStartRepeatedMotion(IVAN, 1);
  Actor_ShowEmote(IVAN, 0x100, 20);
  Event_ShowMessageAndWait(0x8002, 0, 10);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
  KolimaForestTree_PlayGesture(10, 4);
  Engine_EventWait(20);
  Event_ShowMessage(0x8008, 0);
  Actor_ShowEmote(IVAN, 0x101, 60);
  Actor_FaceDirection(IVAN, FACING_NORTH, 10);
  Event_ShowMessageAndWait(0x8002, 0, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 20);
  Event_ShowMessageAndWait(0x8008, 0, 10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
  Engine_ActorSetAnimation(GARET, 3);
  Engine_ActorSetAnimation(MIA, 3);
  Engine_ActorSetAnimationAndWait(IVAN, 3);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(GARET, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(MIA, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(IVAN, FACING_SOUTHWEST, 120);
  Engine_TaskRemoveCallback(effectCallback);
  Engine_EventWait(60);
  KolimaPalette_Restore(0);
  Engine_ColorBufferInterpolate(40);
  KolimaForestTree_PlayGesture(10, 2);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(0x8008, 0, 20);
  KolimaForestTree_PlayGesture(11, 3);
  Event_ShowMessage(0x4009, 0);
  Event_ShowMessage(0x8008, 0);
  KolimaForestTree_PlayGesture(11, 4);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(0x4009, 0, 10);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorStartRepeatedMotion(MIA, 2);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  KolimaForestTree_PlayGesture(10, 1);
  Event_OpenMessage(0x8008, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST, 0);
  if (Engine_EventChooseYesNo(0, 0) == 1) {
    gEventWork->message += 1;
  }
  Engine_EventWait(10);
  KolimaForestTree_PlayGesture(10, 2);
  Engine_EventWait(20);
  KolimaForestTree_PlayGesture(11, 3);
  Engine_EventWait(40);
  KolimaForestTree_PlayGesture(10, 1);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(0x8008, 0, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(GARET, FACING_EAST, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_WEST, 10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
  Engine_ActorSetAnimation(GARET, 3);
  Engine_ActorSetAnimation(MIA, 3);
  Engine_ActorSetAnimationAndWait(IVAN, 3);
  Event_PlayCue(17);
  finalActions = gSceneAction_GroupFinish;
  Actor_SetActionScript(GARET, finalActions);
  if (actorThreeEnabled != 0) {
    Actor_SetActionScript(MIA, finalActions);
  }
  Object_SetActionCallbackAndRefreshById(2, (s32)finalActions);
  KolimaForestTree_PlayGesture(10, 4);
  KolimaForestTree_PlayGesture(10, 4);
  Engine_EventWait(20);
  Engine_EventSetMessage(MsgKolimaOweGreatDebtHaveSaved);
  Event_ShowMessage(0x8008, 0);
  KolimaForestTree_PlayGesture(11, 4);
  KolimaForestTree_PlayGesture(11, 4);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(0x4009, 0, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
  GameFlag_Set(0x845);
  Event_PlayCue(1);
  PaletteScene_SetRecordValue(184, 185);
}

s32 KolimaForestTree_PrepareActors(void) {
  struct FieldActor *first;
  struct FieldActor *third;
  struct FieldActor *second;

  first = Object_GetById(10);
  third = Object_GetById(14);
  second = Object_GetById(11);
  Engine_TaskWait(1);
  Engine_ActorSetChildValue(14, 15);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  gGameState.retreatScene = (s32)&SceneId_KolimaForestBridge;
  gGameState.retreatEntrance = 4;
  if (!Engine_GameFlagIsSet(0x845))
    SceneEffect_AdjustPaletteColors(3);
  Object_GetById(8)->radius = 6;
  Object_GetById(9)->radius = 6;
  Object_GetById(12)->radius = 6;
  Object_GetById(13)->radius = 6;
  Engine_ActorSetSpriteFlags(Object_GetById(14), 0);
  Engine_ActorSetSpriteFlags(Object_GetById(10), 0);
  Engine_ActorSetSpriteFlags(Object_GetById(11), 0);
  Engine_ActorSetSpriteFlags(Object_GetById(8), 0);
  Engine_ActorSetSpriteFlags(Object_GetById(9), 0);
  Engine_ActorSetSpritePriority(8, 2);
  Engine_ActorSetSpritePriority(14, 2);
  Engine_ActorSetSpritePriority(9, 2);
  first->motionFlags = 0;
  first->y.fixed = 0x1c0000;
  second->motionFlags = 0;
  second->y.fixed = 0x1c0000;
  third->motionFlags = 0;
  third->y.fixed = 0x1c0000;
  Engine_ActorSetAnimation(9, 3);
  Engine_ActorSetAnimation(8, 3);
  Object_GetById(8)->collisionFlags |= 8;
  Object_GetById(9)->collisionFlags |= 8;
  Object_GetById(10)->collisionFlags |= 8;
  Object_GetById(11)->collisionFlags |= 8;
  Object_GetById(14)->collisionFlags |= 8;
  return 0;
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "common/kolima_grow.h"

static void PaletteScene_SpawnEffect(void) {

  struct PaletteEffect *effect;
  struct EffectSprite *sprite;
  s32 phase;
  s32 effectFlags;
  s32 spriteFlags;
  s32 spawnX = 0x01460000;
  s32 spawnY = 0x00200000;
  s32 spawnZ = 0x00c00000;
  s32 targetX = 0x01460000;
  s32 targetZ = 0x00f00000;

  phase = gFrameCount & 3;
  if (phase != 0)
    return;
  if (gKolimaForestTreeSparkSound != 0)
    Engine_AudioPlayCue(200);
  effect =
      (struct PaletteEffect *)Engine_ObjectCreate(26, spawnX, spawnY, spawnZ);
  if (effect == 0)
    return;
  sprite = effect->sprite;
  sprite->state = phase;
  effectFlags = 0xfe;
  effectFlags &= effect->flags;
  effect->flags = effectFlags;
  spriteFlags = ~12;
  spriteFlags &= sprite->flags;
  spriteFlags |= 4;
  sprite->flags = spriteFlags;
  effect->progress = 0x1999;
  effect->rateX = 0x40000;
  effect->rateY = 0x40000;
  effect->mode = phase;
  Object_SetMode(effect, 2);
  Engine_ObjectSetPosition((struct FieldActor *)effect, targetX, 0, targetZ);
  Engine_ObjectSetScript(effect, gKolimaForestTreeEffectScript);
}

static void PaletteScene_AdvanceTransition(void) {
  s32 step = gKolimaForestTreeTransitionStep;

  if (step == 0) {
    KolimaPalette_Restore(0);
    Engine_ColorBufferInterpolate(20);
  } else if (step == 20) {
    KolimaPalette_Restore(1);
    Engine_ColorBufferInterpolate(8);
  }
  step = gKolimaForestTreeTransitionStep + 1;
  gKolimaForestTreeTransitionStep = step;
  if (step == 30) {
    gKolimaForestTreeTransitionStep = 0;
  }
}

static void KolimaForestTree_PlayGesture(s32 actor, s32 gesture) {
  if (actor == 10) {
    switch (gesture) {
    case 0:
      Engine_ActorSetAnimation(8, 1);
      Engine_TaskWait(6);
      Engine_ActorSetAnimation(8, 3);
      break;
    case 1:
      Engine_ActorSetAnimation(8, 1);
      break;
    case 2:
      Engine_ActorSetAnimation(8, 1);
      Engine_TaskWait(6);
      Engine_ActorSetAnimation(8, 5);
      break;
    case 3:
      Engine_ActorSetAnimation(8, 1);
      Engine_TaskWait(6);
      Engine_ActorSetAnimation(8, 4);
      break;
    case 4:
      Engine_ActorSetAnimation(8, 1);
      Engine_TaskWait(6);
      Engine_ActorSetAnimation(8, 3);
      Engine_TaskWait(6);
      Engine_ActorSetAnimation(8, 1);
      Engine_TaskWait(6);
      Engine_ActorSetAnimation(8, 3);
      Engine_TaskWait(6);
      Engine_ActorSetAnimation(8, 1);
      break;
    case 5:
      Engine_ActorSetAnimation(8, 1);
      Engine_TaskWait(6);
      Engine_ActorSetAnimation(8, 2);
      break;
    case 6:
      Engine_ActorSetAnimation(8, 6);
      Engine_TaskWait(6);
      Engine_ActorSetAnimation(8, 8);
      break;
    case 8:
      Engine_ActorSetAnimation(8, 6);
      Engine_TaskWait(6);
      Engine_ActorSetAnimation(8, 9);
      break;
    case 9:
      Engine_ActorSetAnimation(8, 6);
      Engine_TaskWait(6);
      Engine_ActorSetAnimation(8, 10);
      break;
    case 10:
      Engine_ActorSetAnimation(8, 6);
      Engine_TaskWait(6);
      Engine_ActorSetAnimation(8, 8);
      Engine_TaskWait(6);
      Engine_ActorSetAnimation(8, 6);
      Engine_TaskWait(6);
      Engine_ActorSetAnimation(8, 8);
      Engine_TaskWait(6);
      Engine_ActorSetAnimation(8, 6);
      break;
    case 7:
    case 11:
      Engine_ActorSetAnimation(8, 6);
      break;
    }
  } else {
    switch (gesture) {
    case 0:
      Engine_ActorSetAnimation(9, 1);
      Engine_TaskWait(6);
      Engine_ActorSetAnimation(9, 3);
      break;
    case 1:
      Engine_ActorSetAnimation(9, 1);
      break;
    case 2:
      Engine_ActorSetAnimation(9, 1);
      Engine_TaskWait(6);
      Engine_ActorSetAnimation(9, 5);
      break;
    case 3:
      Engine_ActorSetAnimation(9, 1);
      Engine_TaskWait(6);
      Engine_ActorSetAnimation(9, 4);
      break;
    case 4:
      Engine_ActorSetAnimation(9, 1);
      Engine_TaskWait(6);
      Engine_ActorSetAnimation(9, 3);
      Engine_TaskWait(6);
      Engine_ActorSetAnimation(9, 1);
      Engine_TaskWait(6);
      Engine_ActorSetAnimation(9, 3);
      Engine_TaskWait(6);
      Engine_ActorSetAnimation(9, 1);
      break;
    case 5:
      Engine_ActorSetAnimation(9, 1);
      Engine_TaskWait(6);
      Engine_ActorSetAnimation(9, 2);
      break;
    }
  }
  Engine_TaskWait(12);
}

static void PaletteScene_AdvanceOrbit(struct OrbitingPaletteEffect *effect) {
  s32 position[3];
  s32 step = effect->step;
  s32 heading;

  if (step <= 119) {
    position[0] = effect->anchorX;
    position[1] = effect->anchorY;
    position[2] = effect->anchorZ;
    heading = effect->heading;
    Vector_AddPolarOffset(step << 16, step * 768 + heading, position);
    effect->x = position[0];
    effect->y = position[1];
    effect->z = position[2];
    effect->angleX += 0x147;
    effect->angleY += 0x147;
    effect->step++;
  } else {
    Resource_ResetEntry(effect->owner[0x1c]);
    Engine_ObjectDispatchRelease(effect);
  }
}

static void KolimaForestTree_SpawnOrbitSparks(void) {
  struct FieldActor *spark;
  s32 previous;
  u32 i;

  previous = 0;
  switch (gKolimaForestTreeSparkCount) {
  case 0:
  case 10:
  case 20:
  case 30:
  case 40:
    Engine_AudioPlayCue(220);
    for (i = 0; i <= 5; i++) {
      spark = Engine_ObjectCreate(0x11d, gKolimaForestTreeSparkOrigin[0],
                                  gKolimaForestTreeSparkOrigin[1],
                                  gKolimaForestTreeSparkOrigin[2]);
      if (spark != 0) {
        previous = Object_ReplaceResourceEntry(spark->sprite, previous);
        spark->motionFlags = 0;
        spark->sprite->priority = 1;
        Engine_ActorSetSpriteFlags(spark, 0);
        Object_SetMode(spark, 1);
        spark->timer = 0;
        spark->work = (i * 60 << 16) / 360;
        spark->targetX = gKolimaForestTreeSparkOrigin[0];
        spark->targetY = gKolimaForestTreeSparkOrigin[1];
        spark->targetZ = gKolimaForestTreeSparkOrigin[2];
        spark->speed = 0x19999;
        spark->update =
            (void (*)(union FieldObject *))PaletteScene_AdvanceOrbit;
      }
    }
    break;
  }
  if (++gKolimaForestTreeSparkCount > 120) {
    gKolimaForestTreeSparkCount = 0;
  }
}

static void PaletteScene_SetRecordValue(s32 key, s32 value) {
  s32 slot = PartyInventory_FindOwner(key);

  if (slot != -1) {
    s32 index = Inventory_Find(slot, key);

    if (index != -1) {
      ((struct PaletteSceneRecord *)UnitGet(slot))->values[index] = value;
    }
  }
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "common/kolima_palette.h"

s32 gSceneAction_ActorOneEntry[] = {
    SC_ARG(2), SC_MOVE(PIXELS(312), 0, PIXELS(230)), SC_SYNC,
    SC_ARG(1), SC_SET(SW_FACING, FACING_NORTH),      SC_WAIT(1),
    SC_END,
};

s32 gSceneAction_ActorTwoEntry[] = {
    SC_ARG(2), SC_MOVE(PIXELS(346), 0, PIXELS(224)), SC_SYNC,
    SC_ARG(1), SC_SET(SW_FACING, FACING_NORTH),      SC_WAIT(1),
    SC_END,
};

s32 gSceneAction_ActorThreeEntry[] = {
    SC_ARG(2), SC_MOVE(PIXELS(333), 0, PIXELS(240)), SC_SYNC,
    SC_ARG(1), SC_SET(SW_FACING, FACING_NORTH),      SC_WAIT(1),
    SC_END,
};

s32 gSceneAction_GroupFinish[] = {
    SC_ARG(2),  SC_MOVE(PIXELS(328), 0, PIXELS(212)),
    SC_SYNC,    SC_POS(0, 0, 0),
    SC_WAIT(1), SC_END,
};

struct SceneEntrance gKolimaForestTreeEntrances[] = {
    {0, CONDITION_ALWAYS, 209, 0, 279, FACING_SOUTH, 0, 0, 0, 512, 448, 0},
    {1, CONDITION_ALWAYS, 136, 0, 424, FACING_NORTH, 0, 0, 0, 512, 448, 0},
    {2, CONDITION_ALWAYS, 328, 16, 200, FACING_SOUTH, 0, 0, 0, 512, 448, 0},
    {SCENE_TABLE_END},
};

// clang-format off
s16 gKolimaForestTreeRegions[] = {
    320, 88, 176, 336, 104, 192, -1, 2,
    -1, 0, 0, 0, 0, 0, 0, 0,
};

u32 gKolimaForestTreeExits[] = {
    SCENE_EXITS(44),
    SCENE_EXIT(1, 43, 2),
    SCENE_EXIT(2, 45, 1),
    SCENE_EXITS_END,
};

struct ScenePlacement gKolimaForestTreePlacements[] = {
    { 0x47, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(327), 0, PIXELS(192), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x38, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(136), 0, PIXELS(270), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x16, CONDITION_ALWAYS, 7,
        PIXELS(327), 0, PIXELS(192), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x16, CONDITION_ALWAYS, 7,
        PIXELS(136), 0, PIXELS(270), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x16, CONDITION_ALWAYS, 7,
        PIXELS(327), 0, PIXELS(194), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x16, CONDITION_ALWAYS, 7,
        PIXELS(136), 0, PIXELS(272), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x37, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(327), 0, PIXELS(192), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0 },
    { IVAN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0 },
    { MIA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gKolimaForestTreeEvents[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { 0xb814, 12, 0x845, EVENT_SCRIPT(EV12_SCRIPT) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, EVENT_SCRIPT(EV13_TALK) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, EVENT_SCRIPT(KolimaForestTree_TalkToTret) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, EVENT_SCRIPT(EV13_PSYNERGY) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x40c, 0x844, EVENT_SCRIPT(KolimaForestTree_TalkToTret) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, EVENT_SCRIPT(EV12_PSYNERGY) },
    { SCENE_EVENTS_END },
};
// clang-format on

s32 gKolimaForestTreeEffectScript[] = {
    SC_ADD(SW_SCALE_X, 0x1999),
    SC_CALL(SceneEffect_AdvanceAngleUntilIdle),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

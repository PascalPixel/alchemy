#include "game_state.h"
#include "scene_ids.h"
#include "unit/scene/fuchin_temple.h"
#include "unit/scene/common/effect_spawn.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/runtime/runtime.h"
#include "field_obj.h"
#include "types.h"
#include "field_event.h"
#include "engine_call.h"
#include "field_scene.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "call.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"
#include "global_cells.h"

// 前方宣言
static void FieldScene_RunSecondEnsembleBeat(void);
static void FieldScene_RunEnsembleStoryBeat(void);
static void FieldScene_RunOpeningAuxiliarySequence(s32 a0, s32 a1);
static void FieldScene_SetFlag140AndFinishSequence(s32 arg0, s32 arg1);
static void FieldScene_FinishSequence(void);
static void FieldScene_SpawnEightShots(void);
static void FieldScene_RunParticleRain(void);

// ここに共通ソースが入る（ROM の並びのとおり）
#include "common/effect_create.h"
#include "common/effect_spawn.h"

static s32 StopXianActor(void *actor) {
  Engine_ActorSetSpriteFlags(actor, 0);
  return 0;
}

static s32 FaceXianActorToPlayer(struct FieldActor *actor) {
  struct FieldActor *player = Object_GetById(ACTOR_PARTY_LEADER);
  actor->facing = ArcTan2(player->z.fixed - actor->z.fixed,
                          player->x.fixed - actor->x.fixed);
  return 0;
}

const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_FuchinTemple1) {
    return gFuchinTempleEntrances1;
  }
  return gFuchinTempleEntrancesOther;
}

s32 GetXianInitialState(void) { return 0; }

s32 GetXianMessageData(void) { return (s32)gFuchinTemple_XianMessages; }

const struct ScenePlacement *Scene_GetPlacements(void) {
  if (gGameState.scene == (s32)&SceneId_FuchinTemple1) {
    return gFuchinTemplePlacements1;
  }
  if (gGameState.entrance == 3) {
    return gFuchinTemplePlacementsEntrance3;
  }
  if (GameFlag_IsSet(0x895)) {
    gFuchinTemplePlacements[5].condition = 0x895;
    gFuchinTemplePlacements[7].condition = 0x895;
    gFuchinTemplePlacements[8].x = 0x1200000;
    gFuchinTemplePlacements[8].z = 0xf80000;
    gFuchinTemplePlacements[11].condition = 0x895;
    gFuchinTemplePlacements[12].condition = 0x895;
  }
  FieldScene_PrepareActors(gFuchinTemplePlacements);
  return gFuchinTemplePlacements;
}

static void EV08_TALK(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgXianHowDidGetHereBridge);
  if (GameFlag_IsSet(0x890) != 0) {
    gEventWork->message += 4;
  }
  Event_OpenMessage(8, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    GameFlag_Set(0x890);
  } else {
    gEventWork->message++;
  }
  Event_ShowMessage(8, 0);
  Engine_EventEnd();
}

static void EV12_TALK_1(void) {

  Engine_EventBegin();
  if (Engine_GameFlagIsSet(0x88F) != 0) {
    Engine_EventSetMessage(MsgXianNowHeTrulyBeyondWorlds);
    Engine_EventAskYesNo(12, 0);
    Engine_EventEnd();
  } else {
    Engine_EventSetMessage(MsgXianIsntNobleHimTrySave);
    Engine_EventOpenMessage(12, 0);
    if (Engine_EventChooseYesNo(0, 0) == 1) {
      u16 *q = &gEventWork->message;
      q[0]++;
      Engine_EventOpenMessage(12, 0);
      if (Engine_EventChooseYesNo(0, 0) == 1) {
        u16 *r = &gEventWork->message;
        r[0]++;
      }
    }
    Engine_EventShowMessage(12, 0);
    Engine_EventEnd();
  }
}

static void EV09_TALK_1(void) {
  s32 *p8;
  struct FieldActor *master;
  struct FieldActor *actor;
  u8 slot16[40];

  master = Object_GetById(9);
  Engine_EventBegin();
  Engine_EventSetMessage(MsgXianAlreadyHeardTest);
  Engine_EventShowMessageAndWait(9, 0, 20);
  Actor_WalkToAndWait(0, 168, 0x188);
  Engine_ActorFaceDirection(0, 0xc000, 20);
  Engine_AudioPlayCue(132);
  actor = Object_GetById(9);
  actor->velocityY = 0x140000;
  actor = Object_GetById(9);
  actor->gravity = 0x40000;
  Actor_SetSpeed(9, 0x30000, 0x18000);
  Actor_SetDestination(9, 152, 0x188);
  Engine_ActorWaitForMove(9);
  actor = Object_GetById(9);
  actor->gravity = 0x10000;
  Engine_ActorFaceDirection(9, 0, 0);
  Engine_AudioPlayCue(132);
  p8 = (s32 *)slot16;
  p8[1] = 7;
  Effect_Spawn(master->x.fixed, master->y.fixed, (master->z.fixed + 0x40000),
               0x8000, 0, 0, 0x10000, p8);
  Effect_Spawn(master->x.fixed, master->y.fixed, (master->z.fixed + 0x40000), 0,
               0, 0, 0x10000, p8);
  Effect_Spawn(master->x.fixed, master->y.fixed, (master->z.fixed + 0x40000),
               -0x8000, 0, 0, 0x10000, p8);
  Engine_EventWait(30);
  BattleFx_PlayQueuedSound();
  Map_CopyCellAttributes(10, 24, 1, 1, 10, 22);
  Engine_GameFlagSet(0x892);
  Engine_EventEnd();
}

static void EV05_TOUCH(void) {
  Engine_EventBegin();
  Engine_GameFlagSet(2196);
  Engine_ActorFaceActor(9, ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(10);
  Engine_EventSetMessage(MsgXianYoungMasterDidCompleteTest);
  Engine_ActorRunRepeatedMotion(9, 2);
  Engine_EventWait(20);
  Actor_FaceDirection(0, 32768, 20);
  Engine_EventAskYesNo(9, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(9, 256, 80);
  Actor_FaceDirection(9, 53248, 20);
  Engine_ActorRunRepeatedMotion(9, 2);
  Engine_EventWait(20);
  Engine_EventShowMessageAndWait(9, 0, 20);
  Engine_ActorFaceDirection(9, 0, 20);
  Engine_ActorSetAnimationAndWait(9, 3);
  Engine_EventWait(20);
  Engine_EventShowMessageAndWait(9, 0, 20);
  Map_CopyCellAttributes(10, 26, 1, 1, 10, 24);
  Engine_EventEnd();
}

static void Scene_RunScene39eSequenceB(void) {
  s32 rec7;
  struct FieldActor *record;

  Engine_EventBegin();
  rec7 = GameFlag_IsSet(0x300);
  if (rec7 != 0) {
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 168, 0x1f8);
    Engine_EventWait(5);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 20);
    Object_GetById(8)->paused = 0;
    Event_PlayCue(152);
    record = Actor_Get(8);
    record->velocityY = 0x80000;
    Engine_ActorSetAnimation(8, 1);
    Engine_EventWait(30);
    Engine_EventSetMessage(MsgXianMustSurviveTestGrottoBefore);
  } else {
    Engine_EventSetMessage(MsgXianMmmmWhoWhoSpeaksMy);
    FieldScene_SetFlag140AndFinishSequence(0, 8);
    Engine_EventWait(30);
    Event_ShowMessage(8, 0);
    FieldScene_FinishSequence();
    Engine_EventWait(20);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 168, 0x1f8);
    Engine_EventWait(5);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 20);
    Event_PlayCue(152);
    Object_GetById(8)->paused = rec7;
    record = Actor_Get(8);
    record->velocityY = 0x80000;
    Engine_ActorSetAnimation(8, 1);
    Engine_EventWait(30);
    Event_OpenMessage(8, 0);
    if (Engine_EventChooseYesNo(0, 0) == 1) {
      Engine_ActorRunRepeatedMotion(8, 2);
      Engine_EventWait(20);
      Event_ShowMessage(8, 0);
      Engine_EventWait(20);
      FieldScene_SetFlag140AndFinishSequence(8, 0);
      Engine_EventWait(30);
      Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
      Engine_EventWait(50);
      FieldScene_FinishSequence();
      Engine_EventWait(30);
      Engine_ActorSetAnimationAndWait(8, 3);
      Event_ShowMessage(8, 0);
    } else {
      gEventWork->message += 2;
      Event_ShowMessage(8, 0);
    }
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(30);
    Actor_ShowEmote(8, 0x100, 60);
    Engine_EventSetMessage(MsgXianFollowThemDoNot);
    Event_OpenMessage(8, 0);
    if (Engine_EventChooseYesNo(0, 0) == 1) {
      Actor_ShowEmote(8, 0x105, 60);
      FieldScene_SetFlag140AndFinishSequence(8, 0);
      Engine_EventWait(30);
      Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
      Engine_EventWait(50);
      FieldScene_FinishSequence();
      Engine_EventWait(30);
      Event_ShowMessage(8, 0);
      gEventWork->message += 1;
    } else {
      gEventWork->message += 1;
      Engine_EventWait(20);
      Engine_ActorSetAnimationAndWait(8, 3);
      Engine_EventWait(20);
      Event_ShowMessage(8, 0);
    }
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(8, 4);
    Engine_EventWait(20);
    Event_ShowMessage(8, 0);
    Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_EventWait(20);
    Event_ShowMessage(8, 0);
    Engine_EventWait(20);
    Actor_FaceDirection(8, 0xc000, 20);
    Actor_SetSpeed(8, 0x4ccc, 0x2666);
    Actor_WalkToAndWait(8, 168, 0x1d0);
    Engine_EventWait(60);
    Actor_FaceDirection(8, 0x4000, 40);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 60);
    Actor_WalkToAndWait(8, 168, 0x1d8);
  }
  Event_OpenMessage(8, 0);
  if (Engine_EventChooseYesNo(0, 0) == 1) {
    Engine_EventSetMessage(MsgXianThenCannotTellGiveUp);
    Event_ShowMessage(8, 0);
    GameFlag_Set(0x300);
  } else {
    Engine_EventSetMessage(MsgXianDoNotWorryWillPermitted);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(20);
    Camera_SetSpeed(0x8000, 0x1000);
    Engine_CameraMoveToActor(8, 1);
    ColorBuffer_ApplySource(0x10000, 0);
    ColorBuffer_ApplyTarget(0x10003, 1);
    Engine_ColorBufferInterpolate(30);
    Engine_EventWaitForScreen();
    Engine_CameraWaitForMove();
    FieldScene_SpawnEightShots();
    ColorBuffer_ApplyTarget(0x10000, 0);
    Engine_ColorBufferInterpolate(30);
    Event_ShowMessage(8, 0);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(20);
    Event_ShowMessage(8, 0);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Engine_EventWait(60);
    Event_ShowMessageAndWait(8, 0, 10);
    GameFlag_Set(0x891);
  }
  Engine_ActorSetAnimation(8, 5);
  Engine_EventEnd();
}

static void EV1032_PSYNERGY(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgXianEnjoyReadingMindsOthersDo);
  Engine_EventAskYesNo(8, 0);
  Engine_EventEnd();
}

static void EV09_TALK(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgXianMasterFehsSchoolCameWatch);
  Engine_EventAskYesNo(9, 0);
  Engine_EventEnd();
}

static void EV12_TALK(void) {
  Engine_EventBegin();

  Object_GetById(12)->paused = 0;

  while (Object_GetById(12)->y.fixed > 0) {
    Engine_TaskWait(1);
  }

  Object_GetById(12)->y.fixed = 0;

  Object_GetById(12)->targetY = ACTOR_NO_TARGET;

  Object_GetById(12)->velocityY = 0;

  Object_GetById(12)->paused = 1;

  Engine_ActorFaceActor(12, ACTOR_PARTY_LEADER, 0);

  if (Engine_GameFlagIsSet(0x895) != 0) {
    Engine_EventSetMessage(MsgXianHsuDidNotPracticeJumping);
  } else if (Engine_GameFlagIsSet(0x89b) != 0) {
    Engine_EventSetMessage(MsgXianLamaTempleFarWestIn);
  } else {
    Engine_EventSetMessage(MsgXianFlexibilityJumpingVeryImportantIn);
  }

  Engine_EventShowMessage(12, 0);

  Object_GetById(12)->facing = 128 << 7;

  Object_GetById(12)->paused = 0;

  Engine_ActorEnableActionCallback(12, gFuchinTemple_ActorTwelveScript);
  Engine_EventEnd();
}

static void EV15_TALK(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgXianMasterFehVeryBusyDo);
  Engine_EventAskYesNo(15, 0);
  Engine_EventEnd();
}

static void FieldScene_RunForwardArcBurst(void) {
  struct FieldActor *actor = Object_GetById(19);
  u32 index;
  s32 angle;

  for (index = 8; index > 3; index--) {
    angle = index << 12;
    actor->sprite->rotation = angle;
    Engine_TaskWait((index - 4) * 2);
    actor->x.fixed += Engine_MathCos(angle) * 6;
    actor->z.fixed += Engine_MathSin(angle) * 6;
  }

  actor->y.fixed = 0x120000;
  actor->targetY = 0x120000;

  Engine_AudioPlayCue(227);

  Effect_Spawn(actor->x.fixed - 0xc0000, actor->y.fixed,
               actor->z.fixed + 0x80000, 0xffffcccd, 0x6666, 0, 0, 0);
  Effect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed + 0x80000,
               0xffff3334, 0x4ccc, 0, 0, 0);
  Effect_Spawn(actor->x.fixed + 0xa0000, actor->y.fixed,
               actor->z.fixed + 0x80000, 0xffff0000, 0x3333, 0, 0, 0);
}

static void FuchinTemple_SpinAway(void) {
  struct FieldActor *actor;
  u32 i;
  s32 angle;

  actor = Object_GetById(19);
  for (i = 8; i <= 12; i++) {
    angle = i << 12;
    actor->sprite->rotation = angle;
    Engine_TaskWait((12 - i) * 2);
    actor->x.fixed -= Engine_MathCos(angle) * 6;
    actor->z.fixed -= Engine_MathSin(angle) * 6;
  }
  actor->y.fixed = 0x120000;
  actor->targetY = 0x120000;
  actor->scaleX = 0xffff3334;
  Engine_AudioPlayCue(227);
  Effect_Spawn(actor->x.fixed - 0xc0000, actor->y.fixed,
               actor->z.fixed + 0x80000, 0x10000, 0x3333, 0, 0, 0);
  Effect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed + 0x80000, 0xcccc,
               0x4ccc, 0, 0, 0);
  Effect_Spawn(actor->x.fixed + 0xa0000, actor->y.fixed,
               actor->z.fixed + 0x80000, 0x3333, 0x6666, 0, 0, 0);
}

static void FieldScene_RunDescentBurst(void) {
  struct FieldActor *actor = Object_GetById(19);
  u32 i = 0;
  s32 step = 8;
  u8 zero;
  do {
    Engine_TaskWait(step);
    actor->z.fixed += 0xffff0000;
    actor->targetZ = ACTOR_NO_TARGET;
    i++;
    step -= 2;
  } while (i <= 3);
  zero = 0;
  actor->sprite->rotation = zero;
  Engine_AudioPlayCue(227);
  Effect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed + 0xfff80000,
               0xffff3334, 0, 0xffffcccd, 0, 0);
  Effect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed + 0xfff80000,
               0x0000cccc, 0, 0xffffcccd, 0, 0);
  Effect_Spawn(actor->x.fixed + 0xfffa0000, actor->y.fixed,
               actor->z.fixed + (160 << 12), 0x00003333, 0, 0xffff0000, 0, 0);
  Effect_Spawn(actor->x.fixed + (192 << 11), actor->y.fixed,
               actor->z.fixed + (160 << 12), 0x00003333, 0, 0xffff0000, 0, 0);
}

static void Scene_RunPrimarySequence(void) {
  struct FieldActor *actor;
  u32 count;
  s32 base;
  s32 slot;

  actor = Object_GetById(19);
  count = 0;
  slot = 8;
  do {
    Engine_TaskWait(slot);
    actor->z.fixed += 0x10000;
    actor->targetZ = ACTOR_NO_TARGET;
    count++;
    slot -= 2;
  } while (count <= 3);

  actor->sprite->rotation = 0;
  actor->z.fixed += 0x180000;
  actor->targetZ = ACTOR_NO_TARGET;
  Engine_AudioPlayCue(227);

  Effect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed + 0xc0000,
               -0xcccc, 0, 0x3333, 0, 0);
  base = 0x3333;
  Effect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed + 0xc0000, 0xcccc,
               0, base, 0, 0);
  Effect_Spawn(actor->x.fixed - 0x60000, actor->y.fixed,
               actor->z.fixed - 0x80000, base, 0, 0x10000, 0, 0);
  Effect_Spawn(actor->x.fixed + 0x60000, actor->y.fixed,
               actor->z.fixed - 0x80000, base, 0, 0x10000, 0, 0);
}

static void FuchinTemple_WalkByFacing(void) {
  u32 dir;
  u32 limit;

  dir = Object_GetById(0)->facing;
  limit = 0x3fff;
  if (((dir + -0x2000) << 16) <= 0x3fff0000) {
    Engine_ActorWalkToAndWait(15, 216, 168);
    Engine_ActorWalkToAndWait(15, 224, 168);
    Actor_FaceDirection(15, 0x2000, 20);
  } else if ((u16)(dir - 0x6000) <= limit) {
    Engine_ActorWalkToAndWait(15, 232, 160);
    Actor_FaceDirection(15, 0x5000, 20);
  } else if ((u16)(dir + 0x6000) <= limit) {
    Engine_ActorWalkToAndWait(15, 216, 168);
    Engine_ActorWalkToAndWait(15, 224, 172);
    Actor_FaceDirection(15, 0xe000, 20);
  } else {
    Engine_ActorWalkToAndWait(15, 232, 160);
    Actor_FaceDirection(15, 0x2000, 20);
  }
}

static void FuchinTemple_RunMasterScene(s32 mode) {
  s32 msg;

  Actor_SetSpeed(15, 0xcccc, 0x6666);
  Engine_EventWait(60);
  msg = MsgXianCannotPushHands;
  Engine_EventSetMessage(msg);
  if (mode == 0) {
    Engine_EventSetMessage(msg - 1);
    Actor_ShowEmote(15, 0x101, 60);
    Engine_EventShowMessageAndWait(15, 0, 20);
    Engine_ActorRunRepeatedMotion(15, 2);
    Engine_EventSetMessage(MsgXianWarriorsCannotUse);
    Engine_EventShowMessageAndWait(15, 0, 20);
    Engine_ActorSetAnimationAndWait(15, 4);
    Engine_EventWait(20);
    Engine_EventShowMessageAndWait(15, 0, 20);
    Engine_ActorSetAnimationAndWait(15, 3);
    Engine_EventWait(20);
  }
  if (mode == 2) {
    Engine_EventSetMessage(MsgXianGreatWarriorTrain);
    Engine_ActorRunRepeatedMotion(15, 2);
    Engine_EventWait(20);
  }
  Engine_EventShowMessageAndWait(15, 0, 20);
  FuchinTemple_WalkByFacing();
  Engine_ActorRunRepeatedMotion(15, 3);
  Actor_SetPosition(19, 0xe80000, 0xa80000);
  Engine_ActorSetPosition(20, 0xe80000, 0xa80000);
  Object_GetById(19)->y.fixed = 0xc0000;
  Object_GetById(19)->targetY = ACTOR_NO_TARGET;
  Object_GetById(19)->scaleX = 0xcccc;
  Object_GetById(19)->sprite->rotation = 0x8000;
  Engine_AudioPlayCue(124);
  Engine_EventWait(40);
  Engine_ActorWalkToAndWait(15, 216, 152);
  Actor_FaceDirection(15, 0x2000, 30);
}

static void FuchinTemple_RunGatheringScene(void) {
  struct FieldActor *record;

  Engine_ActorFaceActor(13, 19, 0);
  Engine_ActorFaceActor(14, 19, 0);
  Engine_ActorFaceActor(15, 19, 0);
  Engine_ActorFaceActor(16, 19, 0);
  Engine_ActorFaceActor(18, 19, 0);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(15, 2);
  Engine_EventWait(20);
  Engine_EventSetMessage(MsgXianLookTreeFell);
  Engine_EventShowMessageAndWait(15, 0, 20);
  Engine_EventShowMessageAndWait(16, 0, 20);
  Actor_ShowEmote(18, 0x105, 60);
  Actor_ShowEmote(16, 0x101, 60);
  Engine_EventShowMessageAndWait(16, 0, 20);
  Engine_ActorSetAnimationAndWait(18, 4);
  Engine_EventWait(20);
  Engine_EventShowMessageAndWait(18, 0, 20);
  Actor_ShowEmote(16, 0x102, 60);
  Engine_EventShowMessageAndWait(16, 0, 20);
  Engine_ActorFaceEachOther(15, 18, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(18, 3);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(15, 3);
  Engine_EventWait(20);
  Actor_SetSpeed(15, 0xcccc, 0x6666);
  FuchinTemple_WalkByFacing();
  Engine_ActorRunRepeatedMotion(15, 3);
  Actor_SetPosition(19, 0xe80000, 0xa80000);
  Engine_ActorSetPosition(20, 0xe80000, 0xa80000);
  record = Object_GetById(19);
  record->y.fixed = 0xc0000;
  record = Object_GetById(19);
  record->targetY = ACTOR_NO_TARGET;
  record = Object_GetById(19);
  record->scaleX = 0xcccc;
  record = Object_GetById(19);
  {
    struct FieldSprite *sprite = record->sprite;
    s32 rotation = 0x8000;

    sprite->rotation = rotation;
  }
  Engine_AudioPlayCue(124);
  Engine_EventWait(40);
  Engine_ActorWalkToAndWait(15, 216, 152);
  Actor_FaceDirection(15, 0x4000, 30);
  Engine_GameFlagSet(0x301);
}

static void EV19_PSYNERGY(void) {
  Engine_EventBegin();

  if (Object_GetById(0)->facing > (128 << 7) &&
      Object_GetById(0)->facing < (192 << 8)) {
    FieldScene_RunForwardArcBurst();
  } else {
    FuchinTemple_SpinAway();
  }

  if (Engine_GameFlagIsSet(0x898) != 0) {
    FuchinTemple_RunGatheringScene();
  } else {
    FuchinTemple_RunMasterScene(0);
  }

  Engine_EventEnd();
}

static void FieldScene_DispatchByFacing(void) {
  struct FieldActor *record = Object_GetById(ACTOR_PARTY_LEADER);
  u16 angle;

  Engine_EventBegin();
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 8);
  Engine_EventWait(20);

  angle = record->facing;

  if ((u16)(angle - 0x2000) <= 0x3fffu) {
    Scene_RunPrimarySequence();
  } else if ((u16)(angle - 0x6000) <= 0x3fffu) {
    FieldScene_RunForwardArcBurst();
  } else if ((u16)(angle + (192 << 7)) <= 0x3fffu) {
    FieldScene_RunDescentBurst();
  } else {
    FuchinTemple_SpinAway();
  }

  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
  FuchinTemple_RunMasterScene(1);
  Engine_EventEnd();
}

static void EV20_PSYNERGY(void) {
  struct FieldActor *record = Object_GetById(ACTOR_PARTY_LEADER);
  u16 facing;

  Engine_EventBegin();

  facing = record->facing;
  if ((u16)(facing - 0x2000) <= 0x3fff) {
    Scene_RunPrimarySequence();
  } else if ((u16)(facing - 0x6000) <= 0x3fff) {
    FieldScene_RunForwardArcBurst();
  } else if ((u16)(facing + 0x6000) <= 0x3fff) {
    FieldScene_RunDescentBurst();
  } else {
    FuchinTemple_SpinAway();
  }

  Engine_CameraSetSpeed(0x10000, 0x2000);
  Engine_CameraMoveToActor(20, 1);
  Engine_CameraWaitForMove();

  if (record->z.part.pixel <= 209) {
    if (Engine_GameFlagIsSet(0x89a) == 0 || Engine_GameFlagIsSet(0x89b) != 0) {
      FuchinTemple_RunMasterScene(0);
    } else {
      FuchinTemple_RunGatheringScene();
    }
    Engine_EventEnd();
    return;
  }

  if (Engine_GameFlagIsSet(0x89b) != 0) {
    FuchinTemple_RunMasterScene(2);
  } else if (Engine_GameFlagIsSet(0x89a) == 0) {
    FieldScene_RunSecondEnsembleBeat();
  } else {
    FieldScene_RunEnsembleStoryBeat();
  }
  Engine_EventEnd();
}

static void FieldScene_RunSecondEnsembleBeat(void) {
  struct FieldActor *rec;

  GameFlag_Set(0x89a);
  Engine_EventWait(30);
  Actor_FaceActor(13, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(15, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(16, ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(20);
  Actor_ShowEmote(13, 128 << 1, 0);
  Actor_ShowEmote(15, 128 << 1, 0);
  Actor_ShowEmote(16, 128 << 1, 0);
  Engine_EventWait(60);
  Engine_EventSetMessage(MsgXianDidDoWarrior);
  Event_ShowMessageAndWait(13, 0, 20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, 13, 0);
  Engine_ActorRunRepeatedMotion(15, 1);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(15, 0, 20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, 15, 0);
  Engine_ActorRunRepeatedMotion(16, 2);
  Engine_EventWait(20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, 16, 0);
  Event_AskYesNo(16, 0);
  Engine_EventWait(50);
  Engine_CameraFollowActor(16, 1);
  Actor_SetSpeed(16, 0xcccc, 0x6666);
  Actor_WalkToAndWait(16, 176, 248);
  Actor_WalkToAndWait(16, 154 << 1, 248);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, 128 << 6, 0);
  Actor_FaceDirection(16, 192 << 8, 20);
  Event_PlayCue(158);
  Map_AnimateCells(gFuchinTemple_CellStepsB, 78, 13);
  Engine_ActorRunRepeatedMotion(16, 2);
  Engine_EventWait(20);
  Actor_SetSpeed(16, 192 << 9, 192 << 8);
  rec = Object_GetById(16);
  rec->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(16, 154 << 1, 136 << 1);
  Engine_EventWait(1);
  rec = Actor_Get(16);

  rec->steeringFlags = ACTOR_FACES_MOTION | rec->steeringFlags;
  Event_ShowMessageAndWait(16, 0, 50);
  Actor_SetPosition(17, 152 << 17, 216 << 16);
  Actor_WalkToAndWait(17, 152 << 1, 248);
  Actor_FaceActor(9, 17, 0);
  Actor_FaceActor(10, 17, 0);
  Actor_FaceActor(11, 17, 0);
  Actor_FaceActor(12, 17, 0);
  Actor_FaceActor(13, 17, 0);
  Actor_FaceActor(14, 17, 0);
  Actor_FaceActor(15, 17, 0);
  Actor_FaceActor(16, 17, 0);
  Actor_FaceActor(ACTOR_PARTY_LEADER, 17, 0);
  Engine_EventWait(10);
  Engine_ActorStartRepeatedMotion(9, 2);
  Engine_ActorStartRepeatedMotion(10, 2);
  Engine_ActorStartRepeatedMotion(11, 2);
  Engine_ActorStartRepeatedMotion(12, 2);
  Engine_ActorStartRepeatedMotion(13, 2);
  Engine_ActorStartRepeatedMotion(14, 2);
  Engine_ActorStartRepeatedMotion(15, 2);
  Engine_ActorRunRepeatedMotion(16, 2);
  Actor_ShowEmote(17, 0x103, 60);
  Actor_SetPosition(18, 152 << 17, 216 << 16);
  Actor_WalkTo(18, 152 << 1, 248);
  Actor_WalkTo(17, 140 << 1, 132 << 1);
  Engine_ActorWaitForMove(18);
  Actor_FaceDirection(18, 160 << 7, 0);
  Engine_ActorWaitForMove(17);
  Event_PlayCue(159);
  Map_AnimateCells(gFuchinTemple_CellStepsC, 78, 13);
  Event_ShowMessageAndWait(18, 0, 20);
  Actor_FaceActor(9, 17, 0);
  Actor_FaceActor(10, 17, 0);
  Actor_FaceActor(11, 17, 0);
  Actor_FaceActor(12, 17, 0);
  Actor_FaceActor(13, 17, 0);
  Actor_FaceActor(14, 17, 0);
  Actor_FaceActor(15, 17, 0);
  Actor_FaceActor(16, 17, 0);
  Actor_FaceActor(ACTOR_PARTY_LEADER, 17, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(17, 2);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(18, 4);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(18, 0, 20);
  Engine_ActorSetAnimationAndWait(17, 3);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(18, 1);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(18, 0, 20);
  Actor_FaceDirection(17, 208 << 8, 20);
  Event_ShowMessageAndWait(17, 0, 20);
  Actor_ShowEmote(18, 0x102, 60);
  Event_ShowMessageAndWait(18, 0, 20);
  Engine_ActorSetAnimationAndWait(17, 3);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(17, 0, 20);
  Engine_ActorSetAnimationAndWait(18, 3);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(18, 0, 20);
  Engine_ActorSetAnimationAndWait(17, 3);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(18, 4);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(18, 0, 20);
  Engine_ActorRunRepeatedMotion(17, 2);
  Engine_EventWait(20);
  Actor_FaceDirection(17, 0, 20);
  Event_ShowMessageAndWait(17, 0, 20);
  Actor_FaceDirection(16, 128 << 8, 20);
  Engine_ActorSetAnimationAndWait(16, 3);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(17, 3);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(17, 0, 20);
  Engine_ActorSetAnimationAndWait(16, 3);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(16, 0, 20);
  Actor_FaceDirection(17, 128 << 8, 20);
  Event_ShowMessageAndWait(17, 0, 20);
  Engine_ActorSetAnimationAndWait(9, 3);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(9, 0, 20);
  Actor_FaceDirection(17, 208 << 8, 20);
  Engine_ActorRunRepeatedMotion(17, 1);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(17, 0, 20);
  Actor_ShowEmote(18, 0x102, 60);
  Event_ShowMessageAndWait(18, 0, 20);
  Actor_ShowEmote(17, 0x101, 60);
  Event_ShowMessageAndWait(17, 0, 20);
  Engine_ActorSetAnimationAndWait(18, 3);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(18, 0, 20);
  Actor_ShowEmote(17, 0x100, 60);
  Event_ShowMessageAndWait(17, 0, 20);
  Engine_ActorSetAnimationAndWait(18, 4);
  Engine_EventWait(20);
  Actor_ShowEmote(17, 0x103, 60);
  Event_ShowMessageAndWait(17, 0, 20);
  Actor_ShowEmote(18, 0x100, 60);
  Event_ShowMessageAndWait(18, 0, 20);
  Engine_ActorSetAnimationAndWait(17, 4);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(17, 0, 20);
  Engine_ActorRunRepeatedMotion(18, 2);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(18, 0, 20);
  Engine_ActorRunRepeatedMotion(17, 2);
  Engine_EventWait(10);
  Actor_WalkToAndWait(17, 128 << 1, 140 << 1);
  Actor_FaceDirection(17, 128 << 7, 20);
  Actor_SetPosition(17, 0, 0);
  Engine_ActorDestroy(17);
  Engine_EventWait(30);
  Engine_ActorRunRepeatedMotion(9, 2);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(9, 0, 20);
  Engine_ActorRunRepeatedMotion(15, 2);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(15, 0, 20);
  Actor_FaceActor(16, 18, 0);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(16, 2);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(16, 0, 20);
  Actor_FaceActor(18, 16, 0);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(18, 0, 20);
  Engine_ActorSetAnimationAndWait(18, 4);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(18, 0, 20);
  Engine_ActorSetAnimationAndWait(18, 3);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(18, 0, 20);
  Actor_SetSpeed(18, 0xcccc, 0x6666);
  Actor_WalkToAndWait(18, 128 << 1, 248);
  Actor_FaceDirection(18, 192 << 8, 20);
  Engine_ActorStartRepeatedMotion(18, 1);
  Actor_ShowEmote(18, 0x100, 60);
  Actor_WalkToAndWait(18, 240, 184);
  Engine_ActorRunRepeatedMotion(18, 2);
  Engine_EventWait(20);
  Actor_SetPosition(19, 232 << 16, 168 << 16);
  Actor_SetPosition(20, 232 << 16, 168 << 16);
  rec = Actor_Get(19);
  rec->y.fixed = 0xc0000;
  rec = Actor_Get(19);
  rec->targetY = ACTOR_NO_TARGET;
  rec = Actor_Get(19);
  rec->scaleX = 0xcccc;
  rec = Actor_Get(19);
  {
    struct FieldSprite *sprite = rec->sprite;
    s32 rotation = 0x8000;

    sprite->rotation = rotation;
  }
  Event_PlayCue(124);
  Event_ShowMessageAndWait(18, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, 192 << 8, 20);
  Actor_WalkToAndWait(16, 128 << 1, 240);
  Actor_FaceDirection(16, 176 << 8, 20);
  Engine_ActorRunRepeatedMotion(16, 1);
  Event_ShowMessageAndWait(16, 0, 20);
  Actor_FaceActor(9, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(10, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(11, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(12, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(13, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(14, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(15, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(16, ACTOR_PARTY_LEADER, 0);
  Engine_ActorRunRepeatedMotion(18, 2);
  Engine_EventWait(20);
  Actor_FaceDirection(18, 160 << 7, 20);
  Actor_WalkToAndWait(18, 248, 208);
  Actor_FaceDirection(18, 160 << 7, 20);
  Event_OpenMessage(18, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_ActorRunRepeatedMotion(16, 1);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(16, 0, 20);
    GameFlag_Set(0x898);
  } else {
    Event_SkipMessages(1);
    Engine_EventWait(20);
    Actor_ShowEmote(18, 0x105, 60);
    Actor_FaceDirection(18, 128 << 7, 20);
    Engine_ActorRunRepeatedMotion(16, 2);
    Engine_EventWait(20);
    Event_OpenMessage(16, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
      Engine_ActorSetAnimationAndWait(16, 3);
      Engine_EventWait(20);
      Actor_FaceDirection(18, 176 << 8, 20);
      Event_ShowMessageAndWait(18, 0, 20);
      GameFlag_Set(0x898);
    } else {
      Event_SkipMessages(1);
      Engine_EventWait(20);
      Engine_ActorSetAnimationAndWait(18, 4);
      Engine_EventWait(20);
      Event_ShowMessageAndWait(18, 0, 20);
      GameFlag_Set(0x899);
    }
  }
  Actor_FaceDirection(10, 128 << 8, 0);
  Actor_FaceDirection(11, 128 << 8, 20);
  Engine_ActorSetAnimation(10, 5);
  Engine_ActorSetAnimation(11, 5);
  Engine_ActorEnableActionCallback(12, gFuchinTemple_ActorTwelveScript);
}

static void EV18_TALK(void) {

  Engine_EventBegin();
  Engine_EventSetMessage(MsgXianWarriorWillShowMeYour);
  Engine_EventOpenMessage(18, 0);

  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventWait(20);
    Engine_EventShowMessageAndWait(18, 0, 20);
    Engine_GameFlagSet(0x898);
    Engine_EventEnd();
  } else {
    gEventWork->message += 1;
    Engine_EventShowMessageAndWait(18, 0, 20);
    Engine_EventEnd();
  }
}

static void FieldScene_RunEnsembleStoryBeat(void) {
  struct FieldActor *actor;

  actor = Actor_Get(18);
  actor->update = 0;
  actor = Object_GetById(13);
  actor->update = 0;
  actor = Object_GetById(14);
  actor->update = 0;
  actor = Object_GetById(15);
  actor->update = 0;
  actor = Actor_Get(16);
  actor->update = 0;
  Engine_ActorSetAnimation(11, 1);
  Camera_SetSpeed(0x8000, 0x1000);
  Camera_MoveTo(0xe80000, -1, 0xc80000, 1);
  Engine_CameraWaitForMove();
  Engine_EventSetMessage(MsgXianYaTreeFell);
  Actor_SetSpeed(10, 0xcccc, 0x6666);
  Actor_SetSpeed(12, 0xcccc, 0x6666);
  Actor_WalkTo(10, 152, 200);
  Actor_WalkToAndWait(12, 144, 248);
  Engine_ActorWaitForMove(10);
  Actor_FaceActor(9, 19, 0);
  Actor_FaceActor(11, 19, 0);
  Actor_FaceActor(13, 19, 0);
  Actor_FaceActor(14, 19, 0);
  Actor_FaceActor(15, 19, 0);
  Actor_FaceActor(16, 19, 0);
  Actor_FaceActor(18, 19, 0);
  Actor_SetSpeed(10, 0x18000, 0xc000);
  Actor_SetSpeed(12, 0x20000, 0x10000);
  Actor_WalkTo(10, 152, 200);
  Actor_WalkToAndWait(12, 144, 248);
  Actor_FaceActor(12, 19, 0);
  Engine_ActorWaitForMove(10);
  Actor_FaceActor(10, 19, 0);
  Engine_ActorRunRepeatedMotion(18, 2);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(18, 0, 40);
  Actor_FaceActor(9, 18, 0);
  Actor_FaceActor(10, 18, 0);
  Actor_FaceDirection(11, 0x3000, 0);
  Actor_FaceActor(12, 18, 0);
  Actor_FaceDirection(13, 0x3000, 0);
  Actor_FaceActor(14, 18, 0);
  Actor_FaceActor(15, 18, 0);
  Actor_FaceActor(16, 18, 0);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(16, 0, 20);
  Engine_ActorSetAnimationAndWait(18, 3);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(16, 3);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(16, 0, 20);
  Actor_ShowEmote(18, 0x105, 60);
  Actor_ShowEmote(16, 0x101, 60);
  Event_ShowMessageAndWait(16, 0, 20);
  Event_ShowMessageAndWait(18, 0, 20);
  Actor_ShowEmote(16, 0x102, 60);
  Actor_ShowEmote(15, 0x101, 60);
  Actor_SetSpeed(15, 0xcccc, 0x6666);
  Actor_WalkToAndWait(15, 216, 176);
  Actor_FaceDirection(15, 0x3000, 20);
  Event_ShowMessageAndWait(15, 0, 20);
  Actor_FaceDirection(18, 0xb000, 20);
  Engine_ActorSetAnimationAndWait(18, 4);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(18, 0, 20);
  Engine_ActorStartRepeatedMotion(9, 2);
  Engine_ActorStartRepeatedMotion(10, 2);
  Engine_ActorStartRepeatedMotion(11, 2);
  Engine_ActorStartRepeatedMotion(12, 2);
  Engine_ActorStartRepeatedMotion(13, 2);
  Engine_ActorStartRepeatedMotion(14, 2);
  Engine_ActorStartRepeatedMotion(15, 2);
  Engine_ActorStartRepeatedMotion(16, 2);
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(13, 2);
  Event_ShowMessageAndWait(13, 0, 20);
  Engine_ActorSetAnimationAndWait(18, 3);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(18, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xe000, 20);
  Actor_FaceDirection(18, 0x5000, 20);
  Event_AskYesNo(18, 0);
  Actor_ShowEmote(9, 0x101, 0);
  Engine_EventWait(5);
  Actor_ShowEmote(10, 0x101, 0);
  Engine_EventWait(5);
  Actor_ShowEmote(11, 0x101, 0);
  Engine_EventWait(5);
  Actor_ShowEmote(12, 0x101, 0);
  Engine_EventWait(5);
  Actor_ShowEmote(13, 0x101, 0);
  Engine_EventWait(5);
  Actor_ShowEmote(14, 0x101, 0);
  Engine_EventWait(5);
  Actor_ShowEmote(15, 0x101, 0);
  Engine_EventWait(5);
  Actor_ShowEmote(16, 0x101, 0);
  Engine_EventWait(60);
  Engine_ActorRunRepeatedMotion(16, 2);
  Event_ShowMessageAndWait(16, 0, 20);
  Engine_ActorSetAnimationAndWait(18, 3);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(18, 0, 20);
  Actor_ShowEmote(15, 0x101, 60);
  Event_ShowMessageAndWait(15, 0, 20);
  Actor_FaceActor(18, 15, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(18, 4);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(18, 0, 40);
  Engine_ActorFaceEachOther(11, 10, 0);
  Engine_ActorFaceEachOther(12, 14, 0);
  Engine_ActorFaceEachOther(13, 15, 0);
  Engine_EventWait(60);
  Actor_FaceActor(10, 18, 0);
  Actor_FaceActor(11, 18, 0);
  Actor_FaceActor(12, 18, 0);
  Actor_FaceActor(13, 18, 0);
  Actor_FaceActor(14, 18, 0);
  Actor_FaceActor(15, 18, 0);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(18, 0, 20);
  Engine_ActorRunRepeatedMotion(18, 2);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(9, 3);
  Engine_ActorSetAnimation(10, 3);
  Engine_ActorSetAnimation(11, 3);
  Engine_ActorSetAnimation(12, 3);
  Engine_ActorSetAnimation(13, 3);
  Engine_ActorSetAnimation(14, 3);
  Engine_ActorSetAnimation(15, 3);
  Engine_ActorSetAnimationAndWait(16, 3);
  Engine_EventWait(20);
  Actor_FaceDirection(18, 0x5000, 20);
  Event_AskYesNo(18, 0);
  Engine_ActorSetAnimationAndWait(18, 3);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(18, 0, 20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
  Engine_ActorSetAnimation(9, 3);
  Engine_ActorSetAnimation(10, 3);
  Engine_ActorSetAnimation(11, 3);
  Engine_ActorSetAnimation(12, 3);
  Engine_ActorSetAnimation(13, 3);
  Engine_ActorSetAnimation(14, 3);
  Engine_ActorSetAnimation(15, 3);
  Engine_ActorSetAnimationAndWait(16, 3);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(18, 2);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(18, 0, 20);
  Event_ShowMessageAndWait(18, 0, 20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
  Engine_ActorSetAnimationAndWait(18, 3);
  Engine_EventWait(20);
  Actor_FaceDirection(18, 0x8000, 20);
  Engine_ActorRunRepeatedMotion(18, 2);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(18, 0, 20);
  Engine_ActorSetAnimation(9, 3);
  Engine_ActorSetAnimation(10, 3);
  Engine_ActorSetAnimation(11, 3);
  Engine_ActorSetAnimation(12, 3);
  Engine_ActorSetAnimation(13, 3);
  Engine_ActorSetAnimation(14, 3);
  Engine_ActorSetAnimation(15, 3);
  Engine_ActorSetAnimationAndWait(16, 3);
  Engine_EventWait(20);
  Actor_WalkTo(10, 120, 200);
  Actor_WalkTo(12, 120, 248);
  Engine_ActorWaitForMove(10);
  Actor_FaceDirection(11, 0x8000, 20);
  Engine_ActorSetAnimation(10, 5);
  Engine_ActorSetAnimation(11, 5);
  Engine_ActorWaitForMove(12);
  Engine_ActorEnableActionCallback(12, gFuchinTemple_ActorTwelveScript);
  Actor_SetSpeed(15, 0xcccc, 0x6666);
  Actor_WalkToAndWait(15, 216, 168);
  Actor_WalkToAndWait(15, 232, 168);
  Actor_FaceDirection(15, 0xc000, 20);
  Engine_ActorRunRepeatedMotion(15, 3);
  Actor_SetPosition(19, 0xe80000, 0xa80000);
  actor = Actor_Get(19);
  actor->y.fixed = 0xc0000;
  actor = Actor_Get(19);
  actor->targetY = ACTOR_NO_TARGET;
  actor = Actor_Get(19);
  {
    struct FieldSprite *sprite = actor->sprite;
    s32 rotation = 0x8000;

    sprite->rotation = rotation;
  }
  Event_PlayCue(124);
  Engine_EventWait(40);
  Actor_WalkToAndWait(15, 216, 152);
  Actor_FaceDirection(15, 0x4000, 30);
  GameFlag_Clear(0x898);
  GameFlag_Set(0x89b);
}

static void EV11_TALK(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgXianWaterMonstersFloodedAltinDid);
  Engine_EventAskYesNo(11, 0);
  Engine_EventEnd();
}

static void EV03_TOUCH(void) {
  Engine_EventBegin();
  if (GameFlag_IsSet(2202) == 0 && GameFlag_IsSet(2197) == 0) {
    Engine_MessageShowCentered(MsgXianItIsLocked, 1);
    Engine_EventEnd();
  } else {
    Event_PlayCue(158);
    Map_AnimateCells(gFuchinTemple_CellStepsB, 78, 13);
    Actor_SetSpeed(0, 0x8000, 0x4000);
    Actor_WalkToAndWait(0, 306, 248);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 304, 216);
    Engine_EventWait(20);
    Engine_EventRequestExit(4);
    Engine_EventEnd();
  }
}

static void EV13_TALK(void) {
  struct FieldActor *record;
  u32 biased;

  record = Object_GetById(ACTOR_PARTY_LEADER);
  biased = record->facing;
  Engine_EventBegin();

  biased += 0xffff5fff;
  if (biased <= 0x3ffe) {
    Engine_SanctumOpen(13);
  } else {
    Engine_EventSetMessage(MsgXianImTravelingAroundWorldSpread);
    Engine_EventShowMessage(13, 0);
  }

  Engine_EventEnd();
}

static void EV08_PSYNERGY(void) {
  Engine_EventBegin();
  Engine_ActorRunRepeatedMotion(8, 2);
  Engine_EventSetMessage(MsgXianIsaacAmCountingOnBring);
  Engine_EventShowMessage(8, 0);
  Engine_EventEnd();
}

const struct SceneEvent *Scene_GetEvents(void) {
  if (gGameState.scene == (s32)&SceneId_FuchinTemple1) {
    return gFuchinTempleEvents1;
  }
  if (gGameState.entrance == 3) {
    return gFuchinTempleEventsEntrance3;
  }
  return gFuchinTempleEvents;
}

static void FieldScene_SpawnRandomizedParticle(void) {
  struct EffectOptions params;
  struct FieldActor *record;
  s32 draw;
  s32 offset;

  record = Object_GetById(ACTOR_PARTY_LEADER);

  params.palette = 7;
  draw = (u32)(Engine_RandomNext() * 7) >> 16;
  if ((draw & 7) == 0)
    params.palette = 5;

  params.startScaleX = 0xb333;
  params.startScaleY = 0xcccc;

  offset = ((u32)(Engine_RandomNext() * 8) >> 16) * 13107;

  Effect_Spawn(record->x.fixed + ((8 - (gFrameCount & 15)) << 16),
               record->y.fixed + (192 << 13), record->z.fixed, 0, -offset, 0,
               144 << 12, &params);

  if ((gFrameCount & 1) != 0)
    Engine_ActorSetChildValue(ACTOR_PARTY_LEADER, 15);
  else
    Engine_ActorSetChildValue(ACTOR_PARTY_LEADER, 1);
}

static void FieldScene_ApplyOffset0Neg32(void) {
  FieldScene_RunOpeningAuxiliarySequence(0, -32);
}

static void FieldScene_ApplyOffset0Pos32(void) {
  FieldScene_RunOpeningAuxiliarySequence(0, 32);
}

static void FieldScene_ApplyOffsetNeg32_0(void) {
  FieldScene_RunOpeningAuxiliarySequence(-32, 0);
}

static void FieldScene_RunOpeningAuxiliarySequence(s32 a0, s32 a1) {
  Engine_EventBegin();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x28000, 0x14000);
  Engine_ActorSetDestinationOffset(0, a0, a1);
  Engine_ActorJump(ACTOR_PARTY_LEADER, 4, 0);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 7);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 6);
  Engine_EventEnd();
}

static void EV03_TOUCH_1(void) {
  struct FieldActor *record;
  SchedulerCallback particleTask;

  Engine_EventBegin();
  particleTask = FieldScene_SpawnRandomizedParticle;
  FieldScheduler_AddCallback(particleTask, 0xc80);
  Engine_ActorSetSpeed(ACTOR_PARTY_LEADER, 0x3333, 0x1999);
  gEventWork->transitionFrames = 60;
  Engine_EventCloseScreen();
  Engine_AudioPlayCue(154);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 2);
  Engine_ActorSetDestinationOffset(ACTOR_PARTY_LEADER, 0, -6);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Engine_ActorSetChildValue(ACTOR_PARTY_LEADER, 15);
  record = Object_GetById(ACTOR_PARTY_LEADER);
  Engine_ActorSetSpriteFlags(record, 0);
  Scheduler_RemoveCallback(particleTask);
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(3);
  Engine_EventEnd();
}

static void EV01_SCRIPT(void) {
  Engine_AudioPlayCue(123);
  Engine_EventRequestExit(1);
}

static void EV02_TOUCH(void) {
  Engine_EventBegin();
  Event_PlayCue(188);
  Map_AnimateCells(gFuchinTemple_CellStepsA, 77, 8);
  Object_GetById(0)->motionFlags = 0;
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 2);
  Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -16);
  Engine_EventWait(16);
  Engine_EventRequestExit(2);
  Engine_EventEnd();
}

static void FuchinTemple_AskIfCurious(void) {
  struct FieldActor *record;

  Engine_EventBegin();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 168, 0x1f8);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Engine_EventWait(20);
  Engine_ActorStartRepeatedMotion(8, 2);
  Actor_SetAttachedEffect(8, 0x102);
  Engine_EventWait(60);
  Object_GetById(8)->paused = 0;
  Event_PlayCue(152);
  record = Actor_Get(8);
  record->velocityY = 0x80000;
  Engine_ActorSetAnimation(8, 1);
  Engine_EventSetMessage(MsgXianExcellentIsaac);
  Event_ShowMessageAndWait(8, 0, 20);
  Event_ShowMessageAndWait(8, 0, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(8, 3);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(8, 0, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 60);
  Event_OpenMessage(8, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    gEventWork->message += 2;
  } else {
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(20);
    gEventWork->message++;
    Event_ShowMessageAndWait(8, 0, 20);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(8, 0, 20);
  }
  Event_ShowMessageAndWait(8, 0, 20);
  Engine_ActorRunRepeatedMotion(8, 2);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(8, 0, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 60);
  Event_OpenMessage(8, 0);
  if (Engine_EventChooseYesNo(0, 0) == 1) {
    Engine_EventWait(10);
    Actor_ShowEmote(8, 0x102, 60);
    Engine_EventSetMessage(MsgXianCanReadMindsKnowCurious);
    Event_OpenMessage(8, 0);
    do {
      if (Engine_EventChooseYesNo(0, 0) != 1) {
        break;
      }
      Engine_EventWait(10);
      Actor_ShowEmote(8, 0x102, 60);
      Engine_EventSetMessage(MsgXianDontTryHard);
      Event_OpenMessage(8, 0);
    } while (1);
  }
  Engine_EventSetMessage(MsgXianMonstersWaitInHidingWould);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(8, 3);
  Engine_EventWait(20);
  Event_OpenMessage(8, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    gEventWork->message++;
  } else {
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(8, 2);
    gEventWork->message++;
    Event_ShowMessageAndWait(8, 0, 20);
  }
  Engine_ActorSetAnimationAndWait(8, 3);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(8, 0, 20);
  Engine_ActorRunRepeatedMotion(8, 2);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(8, 0, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(8, 3);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(8, 5);
  GameFlag_Set(0x893);
  Engine_EventEnd();
}

static void FieldScene_RunRoofEnsembleSequence(void) {
  struct FieldActor *sourceRecord;

  Engine_EventBegin();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x6666, 0x3333);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 0x1d8, 0x218);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Actor_FaceDirection(9, 0, 20);
  Engine_ActorRunRepeatedMotion(9, 2);
  Engine_EventWait(20);
  Engine_EventSetMessage(MsgXianMasterHamaMeditatingPleaseExtremely);
  Event_ShowMessageAndWait(9, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
  Engine_EventWait(30);
  Actor_FaceDirection(8, 0x4000, 30);
  Engine_ActorSetAnimationAndWait(8, 3);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(8, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Engine_EventWait(20);
  Actor_FaceDirection(9, 0xd000, 0);
  sourceRecord = Actor_Get(ACTOR_PARTY_LEADER);
  if (sourceRecord != 0) {
    Actor_SetPosition(GARET, sourceRecord->x.fixed, sourceRecord->z.fixed);
  }
  sourceRecord = Actor_Get(ACTOR_PARTY_LEADER);
  if (sourceRecord != 0) {
    Actor_SetPosition(IVAN, sourceRecord->x.fixed, sourceRecord->z.fixed);
  }
  sourceRecord = Actor_Get(ACTOR_PARTY_LEADER);
  if (sourceRecord != 0) {
    Actor_SetPosition(MIA, sourceRecord->x.fixed, sourceRecord->z.fixed);
  }
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
  Actor_SetSpeed(GARET, 0x8000, 0x4000);
  Actor_SetSpeed(IVAN, 0x8000, 0x4000);
  Actor_SetSpeed(MIA, 0x8000, 0x4000);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 0x1d0, 0x1f8);
  Actor_WalkTo(IVAN, 0x1e0, 0x1f8);
  Actor_WalkTo(GARET, 0x1f0, 0x1f0);
  Actor_WalkTo(MIA, 0x1c0, 0x1f0);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Engine_ActorWaitForMove(IVAN);
  Engine_ActorWaitForMove(MIA);
  Actor_FaceDirection(MIA, 0xe000, 0);
  Engine_ActorWaitForMove(GARET);
  Actor_FaceDirection(GARET, 0xa000, 0);
  Engine_EventWait(20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
  Actor_ShowEmote(GARET, 0x101, 0);
  Actor_ShowEmote(IVAN, 0x101, 0);
  Actor_ShowEmote(MIA, 0x101, 60);
  Actor_FaceDirection(GARET, 0x5000, 20);
  Event_AskYesNo(GARET, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(8, 3);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(8, 0, 20);
  Actor_FaceDirection(GARET, 0xa000, 20);
  Actor_ShowEmote(MIA, 0x100, 60);
  Event_ShowMessageAndWait(MIA, 0, 20);
  Engine_ActorSetAnimationAndWait(8, 3);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(8, 0, 20);
  Actor_ShowEmote(IVAN, 0x100, 60);
  Event_ShowMessageAndWait(IVAN, 0, 20);
  Engine_ActorSetAnimationAndWait(8, 3);
  Actor_FaceDirection(8, 0x3000, 20);
  Event_ShowMessageAndWait(8, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
  Actor_FaceDirection(GARET, 0x6000, 0);
  Actor_FaceDirection(MIA, 0x2000, 0);
  Engine_ActorStartRepeatedMotion(IVAN, 2);
  Actor_SetAttachedEffect(IVAN, 0x102);
  Engine_EventWait(60);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
  Actor_FaceDirection(GARET, 0xa000, 0);
  Actor_FaceDirection(MIA, 0xe000, 0);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(IVAN, 0, 20);
  Engine_ActorSetAnimationAndWait(8, 3);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(8, 0, 20);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(IVAN, 0, 20);
  Event_ShowMessageAndWait(8, 0, 20);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, IVAN, 50);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
  Actor_FaceDirection(IVAN, 0xc000, 30);
  Engine_ActorSetAnimationAndWait(IVAN, 3);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(8, 0, 20);
  Actor_ShowEmote(MIA, 0x101, 60);
  Event_ShowMessageAndWait(MIA, 0, 20);
  Actor_FaceDirection(8, 0x5000, 20);
  Event_ShowMessageAndWait(8, 0, 20);
  Engine_ActorRunRepeatedMotion(MIA, 1);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(8, 3);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(8, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xe000, 0);
  Actor_FaceDirection(GARET, 0x6000, 0);
  Engine_EventWait(30);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
  Actor_FaceDirection(GARET, 0xa000, 20);
  Event_ShowMessageAndWait(GARET, 0, 20);
  Engine_ActorSetAnimationAndWait(8, 4);
  Engine_EventWait(20);
  Actor_FaceActor(8, IVAN, 0);
  Engine_EventWait(10);
  Event_ShowMessageAndWait(8, 0, 20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, IVAN, 0);
  Actor_FaceActor(GARET, IVAN, 0);
  Actor_FaceActor(MIA, IVAN, 0);
  Actor_ShowEmote(IVAN, 0x102, 60);
  Event_ShowMessageAndWait(IVAN, 0, 20);
  Engine_ActorSetAnimationAndWait(8, 3);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(8, 0, 20);
  Engine_ActorSetAnimationAndWait(IVAN, 3);
  Engine_EventWait(20);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(GARET, 0, 20);
  Actor_ShowEmote(8, 0x100, 60);
  Actor_FaceActor(8, GARET, 0);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(8, 0, 20);
  Actor_ShowEmote(IVAN, 0x100, 60);
  Event_ShowMessageAndWait(IVAN, 0, 20);
  Actor_FaceActor(8, GARET, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
  Actor_FaceDirection(GARET, 0xa000, 0);
  Actor_FaceDirection(MIA, 0xe000, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(8, 3);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(8, 0, 20);
  Engine_ActorStartRepeatedMotion(IVAN, 2);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(IVAN, 0, 20);
  Engine_ActorSetAnimationAndWait(8, 3);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(8, 0, 20);
  Engine_ActorRunRepeatedMotion(MIA, 2);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(MIA, 0, 20);
  Actor_FaceDirection(8, 0x5000, 20);
  Engine_ActorSetAnimationAndWait(8, 3);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(8, 0, 20);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorStartRepeatedMotion(IVAN, 2);
  Engine_ActorRunRepeatedMotion(MIA, 2);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(8, 4);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(8, 0, 20);
  Engine_ActorRunRepeatedMotion(8, 2);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(8, 0, 20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
  Engine_ActorSetAnimation(GARET, 3);
  Engine_ActorSetAnimation(IVAN, 3);
  Engine_ActorSetAnimationAndWait(MIA, 3);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(8, 0, 20);
  Actor_ShowEmote(GARET, 0x103, 0);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_EventWait(60);
  Event_ShowMessageAndWait(GARET, 0, 20);
  Engine_ActorSetAnimationAndWait(8, 4);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(8, 0, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
  Actor_ShowEmote(GARET, 0x101, 60);
  Event_ShowMessageAndWait(GARET, 0, 20);
  Actor_ShowEmote(8, 0x102, 0);
  Engine_ActorStartRepeatedMotion(8, 1);
  Engine_EventWait(60);
  Engine_ActorSetAnimationAndWait(8, 4);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(8, 0, 20);
  Event_ShowMessageAndWait(8, 0, 20);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Engine_ActorStartRepeatedMotion(GARET, 1);
  Engine_ActorStartRepeatedMotion(IVAN, 1);
  Engine_ActorRunRepeatedMotion(MIA, 1);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(8, 4);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(8, 0, 20);
  Actor_ShowEmote(GARET, 0x101, 60);
  Actor_FaceDirection(GARET, 0x5000, 20);
  Event_AskYesNo(GARET, 0);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(IVAN, 1);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(IVAN, 0, 20);
  Actor_FaceDirection(GARET, 0xa000, 20);
  Actor_FaceDirection(8, 0xc000, 20);
  Event_ShowMessageAndWait(8, 0, 20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
  Engine_ActorSetAnimation(GARET, 3);
  Engine_ActorSetAnimation(IVAN, 3);
  Engine_ActorSetAnimationAndWait(MIA, 3);
  Engine_EventWait(20);
  Actor_FaceDirection(8, 0x4000, 20);
  Event_ShowMessageAndWait(8, 0, 20);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorStartRepeatedMotion(IVAN, 2);
  Engine_ActorRunRepeatedMotion(MIA, 2);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(8, 4);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(8, 0, 20);
  Actor_ShowEmote(IVAN, 0x101, 60);
  Event_ShowMessageAndWait(IVAN, 0, 20);
  Engine_ActorSetAnimationAndWait(8, 3);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(8, 0, 20);
  Event_ShowMessageAndWait(8, 0, 20);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
  Actor_SetAttachedEffect(GARET, 0x102);
  Actor_SetAttachedEffect(IVAN, 0x102);
  Actor_SetAttachedEffect(MIA, 0x102);
  Engine_EventWait(60);
  Event_ShowMessageAndWait(MIA, 0, 20);
  Actor_FaceDirection(8, 0x5000, 20);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(8, 0, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
  Actor_ShowEmote(GARET, 0x101, 0);
  Actor_ShowEmote(IVAN, 0x101, 0);
  Actor_ShowEmote(MIA, 0x101, 60);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(GARET, 0, 20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, GARET, 0);
  Actor_FaceActor(IVAN, GARET, 0);
  Actor_FaceActor(MIA, GARET, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
  Engine_ActorSetAnimation(IVAN, 3);
  Engine_ActorSetAnimationAndWait(MIA, 3);
  Engine_EventWait(60);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
  Actor_FaceDirection(IVAN, 0xc000, 0);
  Actor_FaceDirection(MIA, 0xd000, 0);
  Engine_EventWait(20);
  Event_AskYesNo(8, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
  Actor_ShowEmote(GARET, 0x102, 0);
  Actor_ShowEmote(IVAN, 0x102, 0);
  Actor_ShowEmote(MIA, 0x102, 0);
  Engine_EventWait(60);
  Actor_FaceDirection(GARET, 0x6000, 20);
  Event_AskYesNo(GARET, 0);
  Engine_EventWait(20);
  Actor_FaceDirection(MIA, 0x2000, 20);
  Event_ShowMessageAndWait(MIA, 0, 20);
  Actor_FaceDirection(GARET, 0xa000, 0);
  Actor_FaceDirection(MIA, 0xe000, 30);
  Actor_FaceDirection(8, 0, 20);
  Actor_ShowEmote(8, 0x105, 60);
  Actor_ShowEmote(IVAN, 0x101, 60);
  Event_ShowMessageAndWait(IVAN, 0, 20);
  Actor_FaceDirection(8, 0xc000, 20);
  Actor_ShowEmote(8, 0x105, 60);
  Event_ShowMessageAndWait(8, 0, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
  Actor_ShowEmote(GARET, 0x101, 0);
  Actor_ShowEmote(IVAN, 0x101, 0);
  Actor_ShowEmote(MIA, 0x101, 60);
  Event_ShowMessageAndWait(8, 0, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
  Actor_ShowEmote(GARET, 0x101, 0);
  Actor_ShowEmote(IVAN, 0x101, 0);
  Actor_ShowEmote(MIA, 0x101, 60);
  Event_ShowMessageAndWait(8, 0, 20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
  Engine_ActorSetAnimation(GARET, 3);
  Engine_ActorSetAnimation(IVAN, 3);
  Engine_ActorSetAnimationAndWait(MIA, 3);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(8, 1);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(8, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
  Actor_FaceDirection(GARET, 0x6000, 0);
  Actor_FaceDirection(IVAN, 0x8000, 0);
  Actor_FaceDirection(MIA, 0x2000, 0);
  Engine_EventWait(60);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
  Actor_FaceDirection(GARET, 0xa000, 0);
  Actor_FaceDirection(IVAN, 0xc000, 0);
  Actor_FaceDirection(MIA, 0xe000, 0);
  Engine_EventWait(30);
  Actor_FaceDirection(8, 0x4000, 20);
  Engine_ActorSetAnimationAndWait(8, 4);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(8, 0, 20);
  Actor_SetPosition(10, 0x1d80000, 0x2600000);
  Event_ShowMessageAndWait(10, 0, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
  Actor_ShowEmote(GARET, 0x100, 0);
  Actor_ShowEmote(IVAN, 0x100, 0);
  Actor_ShowEmote(MIA, 0x100, 0);
  Actor_ShowEmote(8, 0x100, 0);
  Actor_ShowEmote(9, 0x100, 0);
  Engine_EventWait(60);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
  Actor_FaceDirection(GARET, 0x4000, 0);
  Actor_FaceDirection(IVAN, 0x4000, 0);
  Actor_FaceDirection(MIA, 0x4000, 0);
  Actor_FaceDirection(9, 0, 0);
  Engine_EventWait(30);
  Actor_SetSpeed(10, 0xcccc, 0x6666);
  Actor_WalkToAndWait(10, 0x1d8, 0x218);
  Actor_ShowEmote(8, 0x101, 60);
  Event_ShowMessageAndWait(8, 0, 20);
  Actor_FaceDirection(IVAN, 0xc000, 40);
  Actor_WalkToAndWait(IVAN, 0x1e8, 0x200);
  Engine_EventWait(10);
  Actor_FaceDirection(IVAN, 0x6000, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 30);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x1c8, 0x200);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x2000, 20);
  Engine_ActorRunRepeatedMotion(10, 2);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(10, 0, 20);
  Actor_ShowEmote(8, 0x101, 60);
  Event_ShowMessageAndWait(8, 0, 20);
  Engine_ActorStartRepeatedMotion(10, 2);
  Actor_SetAttachedEffect(10, 0x102);
  Engine_EventWait(60);
  Event_ShowMessageAndWait(10, 0, 20);
  Actor_ShowEmote(8, 0x101, 60);
  Event_ShowMessageAndWait(8, 0, 20);
  Actor_ShowEmote(10, 0x100, 60);
  Event_ShowMessageAndWait(10, 0, 20);
  Engine_ActorStartRepeatedMotion(8, 1);
  Actor_ShowEmote(8, 0x100, 60);
  Event_ShowMessageAndWait(8, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xe000, 0);
  Actor_FaceDirection(GARET, 0xa000, 0);
  Actor_FaceDirection(IVAN, 0xa000, 0);
  Actor_FaceDirection(MIA, 0xe000, 0);
  Engine_EventWait(40);
  Engine_ActorSetAnimationAndWait(10, 3);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(10, 0, 20);
  Engine_ActorSetAnimationAndWait(8, 4);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(8, 0, 20);
  Actor_ShowEmote(8, 0x101, 60);
  Event_ShowMessageAndWait(8, 0, 20);
  Engine_ActorStartRepeatedMotion(17, 1);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(10, 0, 20);
  Actor_ShowEmote(8, 0x105, 60);
  Event_ShowMessageAndWait(8, 0, 20);
  Actor_ShowEmote(8, 0x100, 30);
  Event_ShowMessageAndWait(8, 0, 20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
  Engine_EventWait(2);
  Engine_ActorSetAnimation(IVAN, 3);
  Engine_EventWait(1);
  Engine_ActorSetAnimation(MIA, 3);
  Engine_EventWait(5);
  Engine_ActorSetAnimationAndWait(GARET, 3);
  Actor_FaceActor(8, ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(8, 0, 20);
  Actor_SetSpeed(10, 0xcccc, 0x6666);
  Actor_WalkToAndWait(10, 0x1d8, 0x1f8);
  Engine_EventWait(20);
  Engine_ActorStartRepeatedMotion(10, 1);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(10, 0, 20);
  Actor_FaceActor(8, 10, 0);
  Actor_SetSpeed(8, 0xcccc, 0x6666);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x2000, 0);
  Actor_FaceDirection(GARET, 0x6000, 0);
  Actor_FaceDirection(IVAN, 0x6000, 0);
  Actor_FaceDirection(MIA, 0x2000, 0);
  Engine_ActorWaitForMove(8);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(8, 3);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(8, 0, 20);
  Actor_WalkTo(10, 0x1d8, 0x238);
  Actor_WalkToAndWait(8, 0x1d8, 0x218);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_ActorWaitForMove(10);
  Event_ShowMessageAndWait(GARET, 0, 20);
  Engine_ActorStartRepeatedMotion(8, 2);
  Actor_ShowEmote(8, 0x102, 60);
  Actor_FaceDirection(10, 0xd000, 0);
  Actor_FaceDirection(8, 0xd000, 20);
  Event_ShowMessageAndWait(8, 0, 20);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorStartRepeatedMotion(IVAN, 2);
  Engine_ActorRunRepeatedMotion(MIA, 2);
  Actor_WalkToAndWait(8, 0x1d8, 0x200);
  Actor_FaceDirection(8, 0, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
  Actor_FaceDirection(IVAN, 0x8000, 0);
  Actor_FaceDirection(9, 0xd000, 20);
  Engine_ActorSetAnimationAndWait(8, 3);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(8, 0, 20);
  Engine_ActorSetAnimationAndWait(IVAN, 3);
  Engine_EventWait(20);
  Object_GetById(0)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x1c0, 0x200);
  Engine_EventWait(1);
  {
    struct FieldActor *record = Actor_Get(ACTOR_PARTY_LEADER);
    record->steeringFlags = ACTOR_FACES_MOTION | record->steeringFlags;
  }
  Engine_EventWait(20);
  FieldScene_RunParticleRain();
  Engine_EventWait(60);
  UiText_DrawQuantityPairWithCue(2, 144);
  Engine_ActorSetAnimationAndWait(8, 3);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(8, 0, 20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
  Engine_ActorSetAnimation(GARET, 3);
  Engine_ActorSetAnimation(IVAN, 3);
  Engine_ActorSetAnimationAndWait(MIA, 3);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x2000, 0);
  Actor_FaceDirection(GARET, 0x6000, 0);
  Actor_FaceDirection(IVAN, 0x6000, 0);
  Actor_FaceDirection(MIA, 0x2000, 0);
  Actor_FaceDirection(9, 0x3000, 0);
  Engine_EventWait(20);
  Actor_WalkToAndWait(8, 0x1d8, 0x228);
  Engine_ActorRunRepeatedMotion(8, 2);
  Engine_EventWait(20);
  Actor_FaceDirection(8, 0xd000, 20);
  Event_ShowMessageAndWait(8, 0, 20);
  Engine_ActorSetAnimationAndWait(8, 3);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(8, 0, 20);
  Actor_SetSpeed(8, 0x8000, 0x4000);
  Actor_WalkToAndWait(8, 0x1e0, 0x21c);
  Event_ShowMessageAndWait(8, 0, 20);
  Actor_SetSpeed(8, 0xcccc, 0x6666);
  Actor_SetSpeed(9, 0xcccc, 0x6666);
  Actor_WalkTo(8, 0x1d8, 0x260);
  Actor_WalkTo(9, 0x1d8, 0x220);
  Actor_WalkToAndWait(10, 0x1d8, 0x260);
  Engine_ActorWaitForMove(9);
  Actor_WalkTo(9, 0x1d8, 0x260);
  Actor_SetPosition(10, 0, 0);
  Engine_ActorWaitForMove(8);
  Actor_SetPosition(8, 0, 0);
  Engine_ActorWaitForMove(9);
  Actor_SetPosition(9, 0, 0);
  Actor_WalkToAndWait(IVAN, 0x1e8, 0x208);
  Actor_ShowEmote(IVAN, 0x101, 60);
  Engine_ActorRunRepeatedMotion(MIA, 1);
  Engine_EventWait(20);
  Actor_FaceDirection(MIA, 0x2000, 20);
  Event_ShowMessageAndWait(MIA, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
  Actor_FaceDirection(IVAN, 0xa000, 20);
  Engine_ActorSetAnimationAndWait(IVAN, 4);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(IVAN, 0, 20);
  Actor_ShowEmote(MIA, 0x105, 60);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  Actor_FaceDirection(GARET, 0x6000, 20);
  Event_AskYesNo(GARET, 0);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(MIA, 2);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
  Actor_FaceDirection(GARET, 0x8000, 0);
  Event_ShowMessageAndWait(MIA, 0, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
  Actor_ShowEmote(GARET, 0x101, 60);
  Engine_ActorSetAnimationAndWait(MIA, 4);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(MIA, 0, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
  Actor_ShowEmote(GARET, 0x102, 60);
  Event_OpenMessage(MIA, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(GARET, 3);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(GARET, 0, 20);
    gEventWork->message++;
  } else {
    Engine_EventWait(20);
    gEventWork->message++;
    Engine_ActorSetAnimationAndWait(MIA, 3);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(MIA, 0, 20);
  }
  Engine_ActorRunRepeatedMotion(GARET, 1);
  Event_ShowMessageAndWait(GARET, 0, 20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
  Engine_ActorSetAnimationAndWait(MIA, 3);
  Engine_EventWait(20);
  Actor_WalkTo(GARET, 0x1c0, 0x200);
  Actor_WalkToAndWait(MIA, 0x1c0, 0x200);
  Actor_SetPosition(MIA, 0, 0);
  Engine_ActorWaitForMove(GARET);
  Actor_SetPosition(GARET, 0, 0);
  Engine_EventWait(20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, IVAN, 20);
  Engine_EventWait(30);
  Actor_FaceActor(IVAN, ACTOR_PARTY_LEADER, 20);
  Event_ShowMessageAndWait(IVAN, 0, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 60);
  Engine_ActorStartRepeatedMotion(IVAN, 2);
  Actor_SetAttachedEffect(IVAN, 0x102);
  Engine_EventWait(60);
  Engine_ActorSetAnimationAndWait(IVAN, 4);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(IVAN, 0, 20);
  Actor_WalkToAndWait(IVAN, 0x1c0, 0x200);
  Actor_SetPosition(IVAN, 0, 0);
  Engine_EventEnd();
  GameFlag_Set(0x895);
}

s32 FuchinTemple_ApplyEntryState(void) {
  struct FieldActor *actor;
  s32 set;

  if (gGameState.scene == (s32)&SceneId_FuchinTemple2) {
    gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    if (gGameState.entrance == 1) {
      if (Engine_GameFlagIsSet(0x88f)) {
        Engine_ActorSetAnimation(8, 6);
      } else {
        Engine_ActorSetAnimation(8, 5);
        if (Engine_GameFlagIsSet(0xf14) && !Engine_GameFlagIsSet(0x893) &&
            !Engine_GameFlagIsSet(0x109)) {
          FuchinTemple_AskIfCurious();
        }
      }
    } else if (gGameState.entrance == 2 || gGameState.entrance == 4) {
      Engine_GameFlagClear(0x12f);
      set = Engine_GameFlagIsSet(0x895);
      if (set == 0) {
        actor = Object_GetById(19);
        actor->motionFlags = set;
        actor->y.fixed = 0xc0000;
        actor->targetY = 0xc0000;
        actor->scaleX = 0xcccc;
        actor->scaleY = 0x8000;
        actor->sprite->rotation = 0x8000;
        if (Engine_GameFlagIsSet(0x89a)) {
          Engine_ActorSetPosition(18, 0xf80000, 0xd00000);
          if (!Engine_GameFlagIsSet(0x89b)) {
            Engine_ActorSetPosition(16, 0x1000000, 0xf00000);
            Object_GetById(18)->update = FaceXianActorToPlayer;
            Object_GetById(13)->update = FaceXianActorToPlayer;
            Object_GetById(14)->update = FaceXianActorToPlayer;
            Object_GetById(15)->update = FaceXianActorToPlayer;
            Object_GetById(16)->update = FaceXianActorToPlayer;
          }
        }
      } else {
        actor = Object_GetById(19);
        actor->motionFlags = 0;
        actor->y.fixed = 0xc0000;
        actor->targetY = 0xc0000;
        actor->scaleX = 0xcccc;
        actor->scaleY = 0x8000;
        actor->collisionFlags |= 8;
        actor->sprite->rotation = 0x8000;
        Map_CopyCellAttributes(14, 11, 1, 1, 14, 10);
      }
      OverlayObject_PrepareObject(0x1300000, 0x180000, 0xe00000, 223);
      Engine_ActorSetAnimation(10, 5);
      Engine_ActorSetAnimation(11, 5);
    } else if (gGameState.entrance == 3) {
      Engine_GameFlagClear(0x12f);
      if (!Engine_GameFlagIsSet(0x895)) {
        FieldScene_RunRoofEnsembleSequence();
      } else if (!Engine_GameFlagIsSet(0x8b2)) {
        Engine_ActorSetPosition(8, 0, 0);
        Engine_ActorSetPosition(9, 0, 0);
      }
    }
  } else {
    BattleFx_SetQueuedSoundAndPlay(170);
    Object_GetById(9)->collisionFlags |= 16;
    if (gGameState.entrance == 3 && Engine_GameFlagIsSet(0xf14) &&
        !Engine_GameFlagIsSet(0x894)) {
      Map_CopyCellAttributes(10, 84, 1, 1, 10, 24);
    }
    if (Engine_GameFlagIsSet(0x892)) {
      Actor_SetPosition(9, 0x980000, 0x1880000);
      Engine_ActorFaceDirection(9, 0, 0);
      Map_CopyCellAttributes(10, 26, 1, 1, 10, 22);
    }
  }
  return 0;
}

static void FieldScene_SetFlag140AndFinishSequence(s32 arg0, s32 arg1) {
  u8 *globalCtx;

  Engine_GameFlagSet(160 << 1);
  Engine_PsynergyBegin(141, 1);
  globalCtx = gEffectWork;
  Engine_PsynergySetTarget(arg0, arg1);
  globalCtx[0x23] = 0;
  Engine_PsynergyRaiseHands();
  Engine_PsynergyPlayEffect(1);
  Engine_TaskWait(1);
}

static void FieldScene_FinishSequence(void) {
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
  Engine_PsynergyPlayEffect(2);
  Engine_PsynergyLowerHands();
}

static void FieldScene_SpawnEightShots(void) {
  struct EffectOptions descriptor;
  struct FieldActor *record;
  u32 i;

  record = Object_GetById(8);
  descriptor.priority = 1;
  descriptor.type = 0x0119;
  descriptor.script = gFuchinTemple_ShotScript;
  descriptor.targetScaleX = 224 << 10;
  descriptor.targetScaleY = 192 << 9;
  for (i = 0; i <= 7; i++) {
    Engine_EventWait(10);
    if (i & 1) {
      Engine_AudioPlayCue(0x82);
    }
    Effect_Spawn(record->x.fixed, record->y.fixed, record->z.fixed + 0xffe80000,
                 0, 0x9999, 0, 0x00360001, &descriptor);
  }
  Engine_EventWait(60);
}

static void FieldScene_SelectActorModeFromInputBit(s32 arg0) {
  if ((*(u32 *)&gFrameCount >> 1) & 1) {
    ObjectGroup_SetChildValue(arg0, 10);
  } else {
    ObjectGroup_SetChildValue(arg0, 9);
  }
}

static void FieldScene_RunParticleRain(void) {
  struct EffectOptions descriptor;
  struct FieldActor *actor;
  u32 i;
  s32 x;
  s32 y;
  s32 scale;

  Engine_AudioPlayCue(0x83);
  Object_GetById(8)->update =
      (void (*)(union FieldObject *))FieldScene_SelectActorModeFromInputBit;
  Engine_EventWait(40);
  Engine_ColorBufferApplySource(128 << 9, 0);
  Engine_ColorBufferApplyTarget(0x205c54, 1);
  Engine_ColorBufferInterpolate(60);
  Engine_EventWait(40);
  Engine_AudioPlayCue(0x83);
  Object_GetById(2)->update =
      (void (*)(union FieldObject *))FieldScene_SelectActorModeFromInputBit;
  Engine_EventWait(120);
  actor = Object_GetById(8);
  descriptor.priority = 1;
  descriptor.palette = 2;
  descriptor.type = 0x011d;
  for (i = 0; i <= 63; i++) {
    if ((i & 3) == 0) {
      Engine_AudioPlayCue(246);
    }
    x = actor->x.fixed + ((((u32)(Engine_RandomNext() * 3) << 4) >> 16) << 16) +
        0xfff40000;
    y = actor->y.fixed + ((((u32)Engine_RandomNext() << 5) >> 16) << 16) +
        0xfff00000;
    scale = ((((u32)Engine_RandomNext() << 2) >> 16) << 15) + (128 << 8);
    Effect_Spawn(x, y, actor->z.fixed, 0, scale, 0, 152 << 13, &descriptor);
    Engine_TaskWait(2);
  }
  Engine_AudioPlayCue(220);
  Engine_EventWait(30);
  Engine_ColorBufferApplyTarget(128 << 9, 1);
  Engine_ColorBufferInterpolate(60);
  Engine_EventWait(40);
  Object_GetById(8)->update = NULL;
  Object_GetById(2)->update = NULL;
  Engine_ActorSetChildValue(8, 0);
  Engine_ActorSetChildValue(IVAN, 0);
}

s32 gEffectScript0[] = {
    SC_WAIT(1),      SC_LOOP(22, SC_TOP), SC_WAIT(1),
    SC_POS(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gEffectScript1[] = {
    SC_WAIT(1),      SC_LOOP(44, SC_TOP), SC_WAIT(1),
    SC_POS(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gEffectScript2[] = {
    SC_WAIT(1),      SC_LOOP(126, SC_TOP), SC_WAIT(1),
    SC_POS(0, 0, 0), SC_WAIT(1),           SC_FREE,
};

const s32 *gEffectScripts[] = {
    gEffectScript0,
    gEffectScript1,
    gEffectScript2,
};

s32 gFuchinTemple_ActorTwelveScript[] = {
    SC_ARG(0),
    SC_SET(SW_GRAVITY, 0x9999),
    SC_LABEL(1),
    SC_SET(SW_VELOCITY_Y, 0x30000),
    SC_WAIT(30),
    SC_SET(SW_VELOCITY_Y, 0x40000),
    SC_WAIT(30),
    SC_SET(SW_VELOCITY_Y, 0x50000),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(2),
    SC_SET(SW_FACING, 0x5000),
    SC_WAIT(2),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(2),
    SC_SET(SW_FACING, 0xb000),
    SC_WAIT(2),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(2),
    SC_SET(SW_FACING, 0xd000),
    SC_WAIT(2),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(2),
    SC_SET(SW_FACING, 0x3000),
    SC_WAIT(2),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(2),
    SC_WAIT(60),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};

s32 gFuchinTemplePlacement1[] = {
    SC_CALL(StopXianActor),
    SC_WAIT(1),
    SC_END,
};

u16 gFuchinTemple_CellStepsA[] = {
    MAP_CELL_STEP(66, 46, 3, 2, 5),
    MAP_CELL_STEP(63, 46, 3, 2, 5),
    MAP_CELL_STEPS_END,
};

u16 gFuchinTemple_CellStepsB[] = {
    MAP_CELL_STEP(62, 19, 2, 2, 5),
    MAP_CELL_STEP(64, 19, 2, 2, 5),
    MAP_CELL_STEPS_END,
};

u16 gFuchinTemple_CellStepsC[] = {
    MAP_CELL_STEP(62, 19, 2, 2, 5),
    MAP_CELL_STEP(60, 19, 2, 2, 5),
    MAP_CELL_STEPS_END,
};

struct SceneEntrance gFuchinTempleEntrances1[] = {
    {0, CONDITION_ALWAYS, 296, 0, 488, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 296, 0, 504, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {2, CONDITION_ALWAYS, 296, 0, 304, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {3, CONDITION_ALWAYS, 168, 0, 360, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {5, CONDITION_ALWAYS, 232, 0, 328, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gFuchinTempleEntrancesOther[] = {
    {0, CONDITION_ALWAYS, 168, 0, 552, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 168, 0, 552, FACING_NORTH, 0, 48, 384, 288, 576, 0},
    {2, CONDITION_ALWAYS, 256, 0, 264, FACING_NORTH, 0, 48, 80, 368, 288, 0},
    {3, CONDITION_ALWAYS, 472, 0, 552, FACING_NORTH, 0, 352, 384, 592, 576, 0},
    {4, CONDITION_ALWAYS, 304, 0, 248, FACING_SOUTH, 0, 48, 80, 368, 288, 0},
    {SCENE_TABLE_END},
};

u32 gFuchinTemple_XianMessages[] = {
    SCENE_EXITS(60),       SCENE_EXIT(1, 2, 12), SCENE_EXIT(2, 61, 1),
    SCENE_EXIT(3, 62, 1),  SCENE_EXIT(0, 61, 0), SCENE_EXIT(1, 60, 2),
    SCENE_EXIT(2, 72, 10), SCENE_EXIT(3, 88, 2), SCENE_EXIT(4, 73, 8),
    SCENE_EXITS_END,
};

// clang-format off
struct ScenePlacement gFuchinTemplePlacements1[] = {
    { 0x6c, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(296), 0, PIXELS(440), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x95, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(168), 0, PIXELS(360), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x97, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(272), 0, PIXELS(424), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x97, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(392), 0, PIXELS(392), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x97, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(312), 0, PIXELS(328), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x79, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(232), 0, PIXELS(312), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gFuchinTemplePlacements[] = {
    { 0x33, 0x0, ACTOR_STAND,
        PIXELS(168), 0, PIXELS(472), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x6c, 0x0, ACTOR_STAND,
        PIXELS(216), 0, PIXELS(264), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0x82, 0x0, ACTOR_STAND,
        PIXELS(120), 0, PIXELS(200), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x83, 0x0, ACTOR_STAND,
        PIXELS(152), 0, PIXELS(152), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x83, 0x0, (s32)gFuchinTemple_ActorTwelveScript,
        PIXELS(120), 0, PIXELS(248), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x83, 0x0, ACTOR_STAND,
        PIXELS(176), 0, PIXELS(168), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x66, 0x0, ACTOR_STAND,
        PIXELS(168), 0, PIXELS(264), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x82, 0x0, ACTOR_STAND,
        PIXELS(216), 0, PIXELS(152), FACING_SOUTHEAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x82, 0x0, ACTOR_STAND,
        PIXELS(176), 0, PIXELS(216), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x27, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x28, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xcd, CONDITION_ALWAYS, 7,
        PIXELS(232), 0, PIXELS(168), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x16, CONDITION_ALWAYS, (s32)gFuchinTemplePlacement1,
        PIXELS(232), 0, PIXELS(168), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gFuchinTemplePlacementsEntrance3[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    { IVAN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    { MIA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    { 0x3d, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(472), 0, PIXELS(472), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x97, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(440), 0, PIXELS(536), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x27, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gFuchinTempleEvents1[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT_SCRIPT(EV02_TOUCH) },
    { EVENT_TOUCH, 3, CONDITION_ALWAYS, EVENT_SCRIPT(EV03_TOUCH_1) },
    { EVENT_TOUCH, 5, 0x894, EVENT_SCRIPT(EV05_TOUCH) },
    { EVENT_TALK, 8, IF_FLAG_SET(0x88f), MSGNO(MsgFuchinTempleTheFuchinTempleIsAPleasant) },
    { EVENT_TALK, 9, IF_FLAG_SET(0x88f), MSGNO(MsgFuchinTempleOhIsaacWhatTookYouSo) },
    { EVENT_TALK, 10, IF_FLAG_SET(0x88f), MSGNO(MsgFuchinTempleAhhhMasterNyunpaHeHasPassed) },
    { EVENT_TALK, 11, IF_FLAG_SET(0x88f), MSGNO(MsgFuchinTempleMasterNyunpaHasWastedAwayAt) },
    { EVENT_TALK, 12, IF_FLAG_SET(0x88f), EVENT_SCRIPT(EV12_TALK_1) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(EV08_TALK) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(EV08_TALK) },
    { EVENT_TALK, 9, 0x891, MSGNO(MsgFuchinTempleOmOmBlueLightsInThe) },
    { EVENT_TALK, 9, 0x892, EVENT_SCRIPT(EV09_TALK_1) },
    { EVENT_TALK, 9, 0xf14, MSGNO(MsgFuchinTempleTheTestOfFuchinFallsIs) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleMasterNyunpaIsWaitingForYou) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleGemsFromTheSkyBlueLights) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleNyunpaHasNotEatenInDays) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, EVENT_SCRIPT(EV12_TALK_1) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, EVENT_SCRIPT(EV13_TALK) },
    { SEARCH(SEARCH_UNNAMED), 110, 0x350, FIND_PSYNERGY_STONE },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, IF_FLAG_SET(0x88f), MSGNO(MsgFuchinTempleGemsFallingFromTheSkyMountains) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, IF_FLAG_SET(0x88f), MSGNO(MsgFuchinTempleIKnewMasterNyunpaWasFasting) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, IF_FLAG_SET(0x88f), MSGNO(MsgFuchinTempleMasterNyunpaReachedEnlightenmentDuringHis) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, IF_FLAG_SET(0x88f), MSGNO(MsgFuchinTempleMasterNyunpaIsNothingButSkin) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, IF_FLAG_SET(0x88f), MSGNO(MsgFuchinTempleHeCantLeaveUsYetHe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0x890, MSGNO(MsgFuchinTempleIBeginningToThinkIllBe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleIDontFeelSafeGoingHome) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, 0x891, MSGNO(MsgFuchinTempleManyStarsFellFromTheSky) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, 0x892, MSGNO(MsgFuchinTempleCanSuchYoungWarriorsEndureThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, 0xf14, MSGNO(MsgFuchinTempleCanSuchYoungWarriorsEndureThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleTheWarriorsHaveMasteredTheSecrets) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleThereIsNoPointInTraining) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleICantImagineFastingItGets) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleIWillTrainToBecomeA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleSinceImHereIdLikeTo) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gFuchinTempleEvents[] = {
    { 0x4402, 1, CONDITION_ALWAYS, EVENT_SCRIPT(EV01_SCRIPT) },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(EV03_TOUCH) },
    { EVENT_TALK, 8, IF_FLAG_SET(0x88f), MSGNO(MsgFuchinTemple) },
    { EVENT_TALK, 8, IF_FLAG_SET(0x893), MSGNO(MsgFuchinTempleUseThisEnergyToFindThe) },
    { EVENT_TALK, 8, IF_FLAG_SET(0x891), MSGNO(MsgFuchinTempleSpeakToTheMonkStandingBeneath) },
    { EVENT_TALK, 8, IF_FLAG_SET(0x300), EVENT_SCRIPT(Scene_RunScene39eSequenceB) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgXianNyunpaMeditates) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, IF_FLAG_SET(0x88f), EVENT_SCRIPT(EV08_PSYNERGY) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, IF_FLAG_SET(0x893), MSGNO(MsgFuchinTempleNowThatIHavePassedMy) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x408, IF_FLAG_SET(0x891), EVENT_SCRIPT(EV1032_PSYNERGY) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x408, CONDITION_ALWAYS, EVENT_SCRIPT(Scene_RunScene39eSequenceB) },
    { EVENT_TALK, 9, IF_FLAG_SET(0x895), MSGNO(MsgFuchinTempleThisIsMasterFehsKungFu) },
    { EVENT_TALK, 10, IF_FLAG_SET(0x895), MSGNO(MsgFuchinTempleBouldersOvercameHsuKungFuShould) },
    { EVENT_TALK, 11, IF_FLAG_SET(0x895), EVENT_SCRIPT(EV11_TALK) },
    { EVENT_TALK, 12, IF_FLAG_SET(0x895), EVENT_SCRIPT(EV12_TALK) },
    { EVENT_TALK, 14, IF_FLAG_SET(0x895), MSGNO(MsgFuchinTempleFeizhiPredictedHsusDangerHerProphecy) },
    { EVENT_TALK, 16, IF_FLAG_SET(0x895), MSGNO(MsgFuchinTempleHsuIsRestingFeizhiIsWatching) },
    { EVENT_TALK, 18, IF_FLAG_SET(0x89b), MSGNO(MsgFuchinTemplePracticeEveryoneChiHasItsLimits) },
    { EVENT_TALK, 9, IF_FLAG_SET(0x89b), MSGNO(MsgFuchinTempleWarriorYouAreGreatYouHave) },
    { EVENT_TALK, 10, IF_FLAG_SET(0x89b), MSGNO(MsgFuchinTempleEveryoneWantsToLearnTheChi) },
    { EVENT_TALK, 11, IF_FLAG_SET(0x89b), MSGNO(MsgFuchinTempleConcentrateEnergyIntoTheHandsAnd) },
    { EVENT_TALK, 12, IF_FLAG_SET(0x89b), EVENT_SCRIPT(EV12_TALK) },
    { EVENT_TALK, 13, IF_FLAG_SET(0x89b), MSGNO(MsgFuchinTempleIDidNotThinkMuchOf) },
    { EVENT_TALK, 14, IF_FLAG_SET(0x89b), MSGNO(MsgFuchinTempleWhereDidFeizhiGoIAm) },
    { EVENT_TALK, 15, IF_FLAG_SET(0x89b), MSGNO(MsgFuchinTempleMasterHsuIsLateICan) },
    { EVENT_TALK, 16, IF_FLAG_SET(0x89b), MSGNO(MsgFuchinTempleIWillLearnChiIWill) },
    { EVENT_TALK, 18, IF_FLAG_SET(0x301), MSGNO(MsgFuchinTempleWarriorUseYourChiFromThe) },
    { EVENT_TALK, 16, IF_FLAG_SET(0x301), MSGNO(MsgFuchinTempleWarriorPleaseUseYourChiFrom) },
    { EVENT_TALK, 18, IF_FLAG_SET(0x898), MSGNO(MsgFuchinTempleWarriorLetMeSeeYourChi) },
    { EVENT_TALK, 13, IF_FLAG_SET(0x898), MSGNO(MsgFuchinTempleFeizhiIsGoneItIsDangerous) },
    { EVENT_TALK, 14, IF_FLAG_SET(0x898), MSGNO(MsgFuchinTempleFeizhiIsStrongerThanSheLooks) },
    { EVENT_TALK, 15, IF_FLAG_SET(0x898), MSGNO(MsgFuchinTempleWarriorShowUsYourChiLike) },
    { EVENT_TALK, 16, IF_FLAG_SET(0x898), MSGNO(MsgFuchinTempleWarriorThatWasGreatButMaster) },
    { EVENT_TALK, 18, IF_FLAG_SET(0x899), EVENT_SCRIPT(EV18_TALK) },
    { EVENT_TALK, 13, IF_FLAG_SET(0x899), MSGNO(MsgFuchinTempleFeizhiIsGoneItIsDangerousOutside) },
    { EVENT_TALK, 14, IF_FLAG_SET(0x899), MSGNO(MsgFuchinTempleFeizhiIsStrongerThanSheLooksShe) },
    { EVENT_TALK, 15, IF_FLAG_SET(0x899), MSGNO(MsgFuchinTempleIKnowWhyYouCanUse) },
    { EVENT_TALK, 16, IF_FLAG_SET(0x899), MSGNO(MsgFuchinTempleWarriorChiIsAGreatPower) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, EVENT_SCRIPT(EV09_TALK) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleStrongKicksAreVeryImportantKick) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleIWantToLearnSoI) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, EVENT_SCRIPT(EV12_TALK) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleEveryoneInXianCanDoMartial) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleJustListeningDoesNotMakeOne) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, EVENT_SCRIPT(EV15_TALK) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleMasterFehIsVeryBusyHe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, IF_FLAG_SET(0x895), MSGNO(MsgFuchinTemplePeaceHasReturnedToXianBut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, IF_FLAG_SET(0x895), MSGNO(MsgFuchinTempleNoMatterHowMuchYouPractice) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, IF_FLAG_SET(0x895), MSGNO(MsgFuchinTempleTheMonstersAreVeryLargeStriking) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, IF_FLAG_SET(0x895), MSGNO(MsgFuchinTempleWhenABoulderFallsJumpRepeat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, IF_FLAG_SET(0x895), MSGNO(MsgFuchinTempleWhereDidFeizhiGainThePower) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, IF_FLAG_SET(0x895), MSGNO(MsgFuchinTempleMasterFehIsVeryWorriedAbout) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, IF_FLAG_SET(0x89b), MSGNO(MsgFuchinTempleCanSuchAYoungWarriorReally) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, IF_FLAG_SET(0x89b), MSGNO(MsgFuchinTempleYourChiIsFuchinTemplesHow) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, IF_FLAG_SET(0x89b), MSGNO(MsgFuchinTempleIDontCareWhichChiIt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, IF_FLAG_SET(0x89b), MSGNO(MsgFuchinTempleMasterHamasChiCanShatterBoulders) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, IF_FLAG_SET(0x89b), MSGNO(MsgFuchinTempleFollowSilkRoadStraightPassThrough) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, IF_FLAG_SET(0x89b), MSGNO(MsgFuchinTempleWarriorYouUseWeaponsAndChi) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, IF_FLAG_SET(0x89b), MSGNO(MsgFuchinTempleFeizhiIsNotInTheFeh) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, IF_FLAG_SET(0x89b), MSGNO(MsgFuchinTempleFeizhiWorriesOnlyAboutMasterHsu) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, IF_FLAG_SET(0x89b), MSGNO(MsgFuchinTempleLearningChiIsVeryHardMaster) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, IF_FLAG_SET(0x301), MSGNO(MsgFuchinTempleWarriorTheRulesRequireYouTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, IF_FLAG_SET(0x301), MSGNO(MsgFuchinTempleSoCloseWarriorUseChiFrom) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, IF_FLAG_SET(0x898), MSGNO(MsgFuchinTempleWarriorShowMeYourChiI) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, IF_FLAG_SET(0x898), MSGNO(MsgFuchinTempleFeizhiIKnowIKnowHow) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, IF_FLAG_SET(0x898), MSGNO(MsgFuchinTempleMyKickAndPunchAreVery) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, IF_FLAG_SET(0x898), MSGNO(MsgFuchinTempleShowUsYourChiNowDo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, IF_FLAG_SET(0x898), MSGNO(MsgFuchinTempleIAmShockedThatThisWarrior) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, IF_FLAG_SET(0x899), MSGNO(MsgFuchinTempleIWantToSeeTheChi) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, IF_FLAG_SET(0x899), MSGNO(MsgFuchinTempleFeizhiIKnowHowYouFeel) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, IF_FLAG_SET(0x899), MSGNO(MsgFuchinTempleMyKickAndPunchAreVeryLoud) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, IF_FLAG_SET(0x899), MSGNO(MsgFuchinTempleWarriorShowMeYourChiNow) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, IF_FLAG_SET(0x899), MSGNO(MsgFuchinTempleWarriorDoNotHideYourChi) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleWarriorsFightWithWeaponsKungFu) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgFuchinTemplePracticingKicksHurtsMyFeetVery) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleThatPunchWasGoodYouSurprise) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleIMoveTooQuicklyYouCant) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleWarriorsCameToLearnTheFeh) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleGreatKickKickFromTheHip) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleYouAreTheWarriorsWhoCame) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleMasterFehIsTalkingWithFeizhi) },
    { TOUCH_WALKING_INTO(FACING_WEST), 10, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_DispatchByFacing) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_DispatchByFacing) },
    { TOUCH_WALKING_INTO(FACING_EAST), 11, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_DispatchByFacing) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 11, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_DispatchByFacing) },
    { PSYNERGY_ON_ACTOR(0x8c), 19, CONDITION_ALWAYS, EVENT_SCRIPT(EV19_PSYNERGY) },
    { PSYNERGY_ON_ACTOR(0x8e), 20, CONDITION_ALWAYS, EVENT_SCRIPT(EV20_PSYNERGY) },
    { SEARCH(SEARCH_BARREL), 100, 0xf69, FIND_ITEM(ITEM_ANTIDOTE) },
    { SEARCH(SEARCH_JAR), 101, 0xf68, FIND_ITEM(0x10a) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gFuchinTempleEventsEntrance3[] = {
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 3 },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleMasterHamaIsMeditatingAskingFor) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleTheFutureHoldsManyHardshipsBut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleIDoNotThinkMasterHama) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleICannotSeeYourFutureClearly) },
    { SEARCH(SEARCH_JAR), 102, 0xf6a, FIND_COINS(6) },
    { SCENE_EVENTS_END },
};
// clang-format on

s32 gFuchinTemple_ShotScript[] = {
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(44),
    SC_WAIT(1),
    SC_POS(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
};

#include "game_state.h"
#include "version.h"
#include "unit/scene/sol_statue.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/runtime/runtime.h"
#include "callback_scheduler.h"
#include "resource.h"
#include "mem.h"
#include "global_cells.h"
#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "field_service.h"
#include "call.h"
#include "text/msg_ids.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"

#if TBS_PATCH_3
#define SOL_STATUE_BACK_SCROLL_ROW_Y_OFFSET 6
#else
#define SOL_STATUE_BACK_SCROLL_ROW_Y_OFFSET 8
#endif

// Forward declarations.
static void Scene_RunGuardSequenceC(void);
static void EV17_PSYNERGY(void);
static void SolStatue_RunEventSequence(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4);
static s32 SceneActor_IsActorAtTile(s32 no, s32 x, s32 z);
static void SolStatue_OpenNorthwestSeal(void);
static void SolStatue_OpenNortheastSeal(void);
static void SolStatue_OpenSouthwestSeal(void);
static void SolStatue_OpenSoutheastSeal(void);
static void SolStatue_OpenSeal(void);
static void Scene_ShineLeftBeam(void);
static void Scene_ShineRightBeam(void);
static void Event_SayThenWait(s32 speaker, s32 frames);

enum StatueHallFlag {
  FLAG_STATUE_TRAP_SPRUNG = 0x813,
  FLAG_LEFT_BEAM_SHINING = 0x816,
  FLAG_RIGHT_BEAM_SHINING = 0x817
};

#define NULL ((void *)0)
#define TARGET_ID 9
#define GATE_CODE 2059

struct Ent {
  s32 f00;
  u8 f04;
  u8 f05;
  u16 f06 : 9;
  u16 f07 : 7;
  s32 f08;
};

struct Cam {
  u8 pad[228];
  s32 x;
  s32 y;
};

struct CellCopy {
  s32 srcX;
  s32 srcY;
  s32 destX;
  s32 destY;
  s32 width;
  s32 height;
};

struct SealScene {
  struct CellCopy open;
  struct CellCopy pulseA;
  struct CellCopy pulseB;
  struct CellCopy opened;
  s32 flag;
  s32 facing;
  s32 destX;
  s32 destZ;
  struct CellCopy stairs[3];
  s32 actor;
  s32 actorX;
  s32 actorZ;
  s32 unkc4[3];
};

void InitializeSceneRecordBuffer();

enum StatueHallActor { ACTOR_KRADEN = ACTOR_FIRST_PLACED };

struct FocusPoint {
  s32 x;
  s32 y;
  s32 z;
};

struct FocusWork {
  struct FocusPoint *focus;
};

// bss in this order: as commons of one object the linker would sort them itself
static struct SealScene sSealScene;
static struct Ent sSpriteRows[27];

u8 *SceneEventRuntime_GetScriptData(void) {
  return (u8 *)gSceneEventRuntime_ScriptData;
}

s32 SceneEventRuntime_ReturnZero(void) { return 0; }

u8 *SceneEventRuntime_GetMessageData(void) {
  return (u8 *)gSceneEventRuntime_MessageData;
}

u8 *SceneEventRuntime_GetActorData(void) {
  return (u8 *)gSceneEventRuntime_ActorData;
}

u8 *SceneEventRuntime_GetEffectData(void) {
  return (u8 *)gSceneEventRuntime_EffectData;
}

static s32 SceneEventRuntime_SelectInitialSceneByFlags(void) {
  if (Engine_GameFlagIsSet(0x818) != 0) {
    if (Engine_GameFlagIsSet(FLAG_STATUE_TRAP_SPRUNG) == 0) {
      Engine_EventRequestExit(3);
      return 1;
    }
  } else if (Engine_GameFlagIsSet(0x812) == 0) {
    Engine_EventRequestExit(4);
    return 1;
  }
  return -1;
}

static void Scene_OpenTheHole(void) {
  s32 i;

  {
    Map_CopyCellsTo(0, 28, 17, 8, 2, 1);
  }
  Engine_AudioPlayCue(200);
  for (i = 0; i != 22; i++) {
    Map_CopyCellsTo(10, 61, 17, 40, 2, 1);
    Engine_EventWait(4);
    Map_CopyCellsTo(8, 61, 17, 40, 2, 1);
    Engine_EventWait(4);
  }
  {
    Map_CopyCellsTo(0, 59, 15, 38, 4, 3);
    Map_CopyCellsTo(4, 59, 17, 38, 4, 3);
  }
  Map_CopyCellsTo(8, 60, 17, 39, 2, 2);
  {
    Map_CopyCellAttributes(0, 0, 2, 1, 17, 8);
  }
  Engine_GameFlagSet(0x207);
  EV17_PSYNERGY();
}

static void FieldScene_SetupStagedActors(void) {
  struct FieldActor *leader;

  Engine_EventBegin();
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  gEventWork->transitionFrames = 32;
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(20);
  Actor_SetPosition(ACTOR_KRADEN, PIXELS(576), PIXELS(232));
  Engine_EventWait(1);
  Engine_EventSetMessage(MsgSolKradenYouFoundIt);
  Event_SayThenWait(ACTOR_KRADEN, 6);
  Actor_SetPosition(ACTOR_KRADEN, PIXELS(576), PIXELS(280));
  Camera_SetSpeed(0xcccc, 0x1999);
  Camera_MoveTo(0x23e0000, -1, 0xb40000, 1);
  Actor_SetSpeed(ACTOR_KRADEN, 0x10000, 0x8000);
  Actor_WalkToAndWait(ACTOR_KRADEN, 0x240, 216);
  Engine_EventWait(20);
  Engine_ActorJump(JENNA, 2, 0);
  Engine_EventWait(30);
  Event_SayThenWait(JENNA, 6);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
  Engine_EventWait(6);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST + FACING_STEP, 0);
  Engine_EventWait(10);
  Camera_SetSpeed(0x59999, 0xb333);
  Camera_MoveTo(0x11f0000, -1, 0xb00000, 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(60);
  Camera_MoveTo(0x23e0000, -1, 0xb40000, 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
  Engine_EventWait(10);
  Engine_ActorJump(ACTOR_KRADEN, 6, 0);
  Actor_SetSpeed(ACTOR_KRADEN, 0x30000, 0x20000);
  Actor_WalkToAndWait(ACTOR_KRADEN, 0x240, 184);
  Engine_EventWait(40);
  Event_SayThenWait(ACTOR_KRADEN, 6);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
  Engine_EventWait(40);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
  gEventWork->transitionFrames = 16;
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Camera_MoveTo(0x11f0000, -1, 0xb00000, 0);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(40);
  Actor_SetPosition(ACTOR_KRADEN, PIXELS(424), PIXELS(200));
  Engine_TaskWait(1);
  Event_SayThenWait(ACTOR_KRADEN, 20);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Actor_SetPosition(ACTOR_KRADEN, PIXELS(576), PIXELS(184));
  Camera_MoveTo(0x23e0000, -1, 0xb40000, 0);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(20);
  Camera_SetSpeed(0x13333, 0x2666);
  Camera_MoveTo(0x23e0000, -1, 0x9d0000, 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Event_SayThenWait(GARET, 20);
  Engine_ActorSetAnimationAndWait(JENNA, ANIM_NOD);
  Event_SayThenWait(JENNA, 6);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
  Engine_EventWait(40);
  Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  Event_SayThenWait(ACTOR_KRADEN, 80);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | 2, 0);
  Engine_EventWait(60);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
  Engine_EventWait(30);
  Event_SayThenWait(ACTOR_KRADEN, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | 1, 0);
  Engine_EventWait(60);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  Event_SayThenWait(ACTOR_KRADEN, 6);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, GARET, 0);
  Engine_EventWait(40);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, JENNA, 0);
  Engine_EventWait(40);
  Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Event_SayThenWait(ACTOR_KRADEN, 6);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(JENNA, FACING_SOUTH, 0);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  Engine_EventWait(40);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimation(JENNA, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Event_SayThenWait(ACTOR_KRADEN, 6);
  Camera_SetSpeed(0x19999, 0x3333);
  Camera_MoveTo(0x2400000, -1, 0xd70000, 1);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 0);
  Engine_EventWait(10);
  Engine_ActorJump(ACTOR_KRADEN, 6, 0);
  Actor_WalkToAndWait(ACTOR_KRADEN, 0x240, 217);
  Engine_EventWait(20);
  Actor_WalkToAndWait(ACTOR_KRADEN, 0x240, 0x141);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  Camera_SetSpeed(0x39999, 0x7333);
  Camera_MoveTo(0x2400000, -1, 0x880000, 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  Actor_SetSpeed(GARET, 0x10000, 0x8000);
  Actor_SetSpeed(JENNA, 0x10000, 0x8000);
  Engine_ActorSetAnimation(GARET, ANIM_WALK);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetDestination(GARET, leader->x.part.pixel, leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(GARET);
  Actor_SetPosition(GARET, 0, 0);
  Engine_ActorSetAnimation(JENNA, ANIM_WALK);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetDestination(JENNA, leader->x.part.pixel, leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(JENNA);
  Actor_SetPosition(JENNA, 0, 0);
  GameFlag_Clear(FLAG_ARRIVAL_EVENT_PENDING);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  gEventWork->transitionFrames = 16;
  Engine_EventEnd();
}

static void FieldScene_RunStagedActorScene(void) {
  struct EventWork *work;
  struct FieldActor *record;

  Engine_EventBegin();
  work = gEventWork;
  work->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  work->transitionFrames = 32;
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(20);
  Engine_ActorSetPosition(8, PIXELS(576), PIXELS(296));
  Engine_EventWait(1);
  Engine_EventSetMessage(MsgSolWait);
  Event_SayThenWait(8, 6);
  Camera_SetSpeed(0xcccc, 0x1999);
  Camera_MoveTo(0x23e0000, -1, 0xb40000, 1);
  Actor_SetSpeed(8, 0x10000, 0x8000);
  Actor_WalkToAndWait(8, 0x240, 216);
  Engine_EventWait(20);
  Engine_ActorJump(5, 2, 0);
  Engine_EventWait(30);
  Event_SayThenWait(5, 6);
  Engine_ActorRunRepeatedMotion(8, 2);
  Engine_EventWait(6);
  Engine_ActorFaceDirection(8, FACING_WEST + FACING_STEP, 0);
  Engine_EventWait(10);
  Engine_CameraSetSpeed(0x59999, 0xb333);
  Camera_MoveTo(0x11f0000, -1, 0xb00000, 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(60);
  Camera_MoveTo(0x23e0000, -1, 0xb40000, 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(8, 3);
  Engine_EventWait(10);
  Actor_FaceDirection(8, FACING_NORTH, 0);
  Engine_EventWait(10);
  Engine_ActorJump(8, 6, 0);
  Actor_SetSpeed(8, 0x30000, 0x20000);
  Actor_WalkToAndWait(8, 0x240, 184);
  Engine_EventWait(40);
  Engine_ActorSetAnimationAndWait(8, 3);
  Event_SayThenWait(8, 6);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimation(5, 3);
  Engine_ActorSetAnimationAndWait(0, ANIM_NOD);
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(8, 3);
  Event_SayThenWait(8, 6);
  Engine_ActorJump(0, 2, 0);
  Engine_ActorJump(1, 2, 0);
  Engine_ActorJump(5, 2, 0);
  Engine_EventWait(30);
  Engine_ActorRunRepeatedMotion(1, 2);
  Event_SayThenWait(1, 6);
  Engine_ActorRunRepeatedMotion(8, 1);
  Engine_ActorSetAnimationAndWait(8, 4);
  Event_SayThenWait(8, 6);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, JENNA, 0);
  Engine_EventWait(40);
  Engine_ActorFaceActor(8, 0, 0);
  Actor_FaceActor(8, JENNA, 0);
  Engine_EventWait(40);
  Engine_ActorJump(8, 6, 0);
  Actor_WalkToAndWait(8, 0x240, 216);
  Actor_FaceDirection(8, FACING_WEST, 0);
  Engine_EventWait(10);
  Actor_SetSpeed(8, 0x13333, 0x9999);
  Engine_ActorWalkTo(8, 0x1b0, 200);
  Engine_EventWait(20);
  Camera_SetSpeed(0x26666, 0x4ccc);
  Camera_MoveTo(0x1200000, -1, 0xab0000, 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(80);
  Engine_ActorSetAnimation(8, 1);
  Camera_MoveTo(0x23e0000, -1, 0xb40000, 1);
  Engine_EventWait(20);
  Engine_ActorFaceDirection(8, FACING_EAST, 0);
  Engine_EventWait(30);
  Actor_WalkToAndWait(8, 0x240, 216);
  Actor_FaceDirection(8, FACING_NORTH, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(0, FACING_SOUTH, 0);
  Actor_FaceDirection(5, FACING_SOUTH, 0);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  Engine_CameraSetSpeed(0xcccc, 0x1999);
  Engine_CameraMoveTo(0x23e0000, -1, 0xab0000, 1);
  Engine_ActorJump(8, 6, 0);
  Actor_SetSpeed(8, 0x30000, 0x20000);
  Engine_ActorWalkToAndWait(8, 0x240, 184);
  Engine_EventWait(80);
  Event_SayThenWait(8, 6);
  Engine_ActorSetAnimationAndWait(8, 4);
  Event_SayThenWait(8, 20);
  Engine_ActorShowEmote(5, EMOTE_IN_FRONT | 2, 0);
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(JENNA, 2);
  Event_SayThenWait(5, 6);
  Engine_ActorSetAnimationAndWait(8, 3);
  Engine_ActorFaceDirection(8, FACING_WEST, 0);
  Engine_EventWait(40);
  Engine_ActorFaceDirection(8, FACING_NORTH, 0);
  Engine_EventWait(30);
  Event_SayThenWait(8, 6);
  Actor_ShowEmote(0, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(1, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | 1, 0);
  Engine_EventWait(60);
  Engine_ActorSetAnimationAndWait(8, 4);
  Event_SayThenWait(8, 6);
  Engine_ActorFaceEachOther(1, 0, 0);
  Engine_EventWait(40);
  Engine_ActorFaceEachOther(JENNA, ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(40);
  UiText_OpenMessageAtObject(8, 0);
  Actor_FaceDirection(0, FACING_SOUTH, 0);
  Actor_FaceDirection(5, FACING_SOUTH, 0);
  Actor_FaceDirection(1, FACING_SOUTH, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventSetMessage(MsgSolHonestlyDoubtUnderstand);
  } else {
    Engine_EventSetMessage(MsgSolHmphWellTold);
  }
  Event_SayThenWait(8, 6);
  Engine_EventSetMessage(MsgSolTryFindSolution);
  Engine_ActorRunRepeatedMotion(8, 2);
  Event_SayThenWait(8, 6);
  Engine_ActorShowEmote(1, EMOTE_IN_FRONT | 2, 0);
  Engine_EventWait(60);
  Event_SayThenWait(1, 6);
  Engine_ActorSetAnimationAndWait(8, 3);
  Event_SayThenWait(8, 6);
  Engine_ActorFaceDirection(8, FACING_SOUTH, 0);
  Engine_EventWait(20);
  Engine_ActorJump(8, 6, 0);
  Actor_WalkToAndWait(8, 0x240, 216);
  Engine_EventWait(40);
  Engine_CameraSetSpeed(0xcccc, 0x1999);
  Camera_MoveTo(0x23e0000, -1, 0xbf0000, 1);
  Actor_SetSpeed(8, 0x10000, 0x8000);
  Engine_ActorWalkToAndWait(8, 0x240, 232);
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(8, 2);
  Engine_EventWait(40);
  Event_SayThenWait(8, 6);
  Engine_ActorFaceDirection(8, FACING_NORTH, 0);
  Engine_EventWait(30);
  Event_SayThenWait(8, 6);
  Engine_ActorSetAnimationAndWait(8, 3);
  Camera_MoveTo(0x2400000, -1, 0xd70000, 1);
  Actor_WalkToAndWait(8, 0x23e, 0x143);
  Engine_ActorSetPosition(8, 0, 0);
  Camera_SetSpeed(0x39999, 0x7333);
  Engine_CameraMoveTo(0x2400000, -1, 0x880000, 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  Engine_ActorFaceDirection(5, FACING_EAST, 0);
  Engine_EventWait(10);
  Event_SayThenWait(5, 6);
  Engine_ActorSetAnimation(1, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(5, 3);
  Actor_SetSpeed(1, 0x10000, 0x8000);
  Actor_SetSpeed(5, 0x10000, 0x8000);
  Engine_ActorSetAnimation(JENNA, 2);
  record = Object_GetById(0);
  if (record != 0) {
    Engine_ActorSetDestination(5, record->x.part.pixel, record->z.part.pixel);
  }
  Engine_ActorWaitForMove(5);
  Engine_ActorSetPosition(5, 0, 0);
  Engine_ActorSetAnimation(1, ANIM_WALK);
  record = Object_GetById(0);
  if (record != 0) {
    Engine_ActorSetDestination(1, record->x.part.pixel, record->z.part.pixel);
  }
  Engine_ActorWaitForMove(1);
  Engine_ActorSetPosition(1, 0, 0);
  GameFlag_Clear(FLAG_ARRIVAL_EVENT_PENDING);
  work = gEventWork;
  work->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  work->transitionFrames = 16;
  Engine_EventEnd();
}

static void SolStatue_RunSealOpenedSequence(void) {
  struct FocusWork *work;
  struct FieldActor *leader;
  struct FocusPoint *saved;
  struct FocusPoint point;
  s32 side;
  s32 i;

  work = gMapWork[0];
  leader = Object_GetById(0);
  if (leader->z.fixed < 0xb30000) {
    Actor_WalkToAndWait(0, 0x23f, 132);
    Actor_FaceDirection(0, FACING_SOUTH, 0);
    Engine_EventWait(30);
    saved = work->focus;
    point.x = leader->x.fixed;
    point.y = leader->y.fixed;
    point.z = leader->z.fixed;
    work->focus = &point;
    for (i = 0; i != 30; i++) {
      point.z += 0x10000;
      Engine_EventWait(1);
    }
    Engine_EventWait(40);
    side = 1;
  } else {
    Actor_WalkToAndWait(0, 0x241, 222);
    Actor_FaceDirection(0, FACING_NORTH, 0);
    Engine_EventWait(30);
    point.x = leader->x.fixed;
    point.y = leader->y.fixed;
    point.z = leader->z.fixed;
    saved = work->focus;
    work->focus = &point;
    for (i = 0; i != 30; i++) {
      point.z -= 0x10000;
      Engine_EventWait(1);
    }
    Engine_EventWait(40);
    side = 2;
  }
  for (i = 0; i != 6; i++) {
    Map_CopyCellsTo(2, 28, 34, 10, 4, 2);
    Engine_EventWait(8);
    Map_CopyCellsTo(2, 30, 34, 10, 4, 2);
    Engine_EventWait(8);
  }
  for (i = 0; i != 10; i++) {
    Map_CopyCellsTo(2, 28, 34, 10, 4, 2);
    Engine_EventWait(4);
    Map_CopyCellsTo(2, 30, 34, 10, 4, 2);
    Engine_EventWait(4);
  }
  for (i = 0; i != 12; i++) {
    Map_CopyCellsTo(2, 28, 34, 10, 4, 2);
    Engine_EventWait(2);
    Map_CopyCellsTo(2, 30, 34, 10, 4, 2);
    Engine_EventWait(2);
  }
  Map_CopyCellsTo(2, 28, 34, 10, 4, 2);
  Map_CopyCellsTo(8, 55, 32, 40, 8, 4);
  Engine_EventWait(60);
  if (side == 1) {
    for (i = 0; i != 30; i++) {
      point.z -= 0x10000;
      Engine_EventWait(1);
    }
  } else if (side == 2) {
    for (i = 0; i != 30; i++) {
      point.z += 0x10000;
      Engine_EventWait(1);
    }
  }
  work->focus = saved;
}

static void SceneEffect_UpdateScrollingSpriteRows(void) {
  s32 *cp = &((struct Cam *)gMapWork[0])->x;
  struct Ent *e = sSpriteRows;
  s32 sx = cp[0] / 65536;
  s32 sy = 80 - cp[1] / 65536;
  s32 v;
  u32 i;

  if ((u32)(sy + 16) <= 175) {
    v = (gSolStatue_ScrollPhase >> 10) - sx;
    v |= -32;
    {
      for (i = 0; i <= 8; i++) {
        e->f06 = v;
        e->f04 = sy;
        Runtime_PushSlotEntry(e, 0);
        v += 32;
        e++;
      }
    }
    v = (gSolStatue_ScrollPhase >> 9) - sx;
    v |= -32;
    {
      for (i = 0; i <= 8; i++) {
        e->f06 = v;
        e->f04 = sy;
        Runtime_PushSlotEntry(e, 0);
        v += 32;
        e++;
      }
    }
    v = (gSolStatue_ScrollPhase >> 8) - sx;
    v |= -32;
    {
      for (i = 0; i <= 8; i++) {
        e->f06 = v;
        e->f04 = sy + SOL_STATUE_BACK_SCROLL_ROW_Y_OFFSET;
        Runtime_PushSlotEntry(e, 0);
        v += 32;
        e++;
      }
    }
  }
  gSolStatue_ScrollPhase += 0x80;
}

static void SceneState_RunWhenSlotZeroFacingC000(void) {
  u16 *p = Object_GetById(0);
  if (p[3] == 0xc000) {
    Engine_LeaderCheckAhead();
  }
}

static void EV11_SCRIPT(void) {
  u16 *p = Object_GetById(0);
  if (p[3] == 0x4000) {
    Engine_LeaderCheckAhead();
  }
}

static void FieldScene_RunPrimarySequenceHead(void) {
  if (GameFlag_IsSet(GATE_CODE) == 0) {
    Engine_EventBegin();
    Object_GetById(TARGET_ID);
    Actor_SetSpeed(TARGET_ID, 13107, 0x00001999);
    Actor_WalkToAndWait(TARGET_ID, 504, 152);
    Engine_EventEnd();
  }
}

s32 Scene_RunGuardSequenceB(void) {
  struct FieldActor *record;

  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  Scene_RunGuardSequenceC();
  GameFlag_Set(0x144);
  record = Object_GetById(18);
  record->collisionFlags = 0;
  {
    u8 flags = record->priorityFlags | 2;

    record->priorityFlags = flags;
  }
  Engine_ActorSetSpriteFlags((s32)Object_GetById(18), 0);
  Object_GetById(18)->priorityFlags &= 254;
  Engine_ActorSetSpritePriority(18, 1);
  if ((u32)(((u16)gGameState.entrance - 3) << 16) > 0x10000) {
    Actor_SetPosition(JENNA, 0, 0);
    Actor_SetPosition(GARET, 0, 0);
  }
  if (GameFlag_IsSet(0x818) != 0) {
    Actor_SetPosition(18, PIXELS(288), PIXELS(178));
    Actor_SetPosition(17, PIXELS(1608), PIXELS(1608));
    Actor_SetPosition(10, PIXELS(232), PIXELS(120));
    Actor_SetPosition(12, PIXELS(344), PIXELS(120));
    Actor_SetPosition(10, PIXELS(232), PIXELS(120));
    Map_CopyCellsTo(0, 59, 15, 38, 4, 3);
    Actor_SetPosition(12, PIXELS(344), PIXELS(120));
    Map_CopyCellsTo(4, 59, 17, 38, 4, 3);
    Map_CopyCellsTo(8, 60, 17, 39, 2, 2);
    Map_CopyCellAttributes(0, 1, 2, 1, 17, 7);
  } else if (GameFlag_IsSet(FLAG_LEFT_BEAM_SHINING) != 0 &&
             GameFlag_IsSet(FLAG_RIGHT_BEAM_SHINING) != 0) {
    Actor_SetPosition(10, PIXELS(232), PIXELS(120));
    Actor_SetPosition(12, PIXELS(344), PIXELS(120));
    Map_CopyCellsTo(0, 28, 17, 8, 2, 1);
    Actor_SetPosition(10, PIXELS(232), PIXELS(120));
    Map_CopyCellsTo(0, 59, 15, 38, 4, 3);
    Actor_SetPosition(12, PIXELS(344), PIXELS(120));
    Map_CopyCellsTo(4, 59, 17, 38, 4, 3);
    Map_CopyCellsTo(8, 60, 17, 39, 2, 2);
    Map_CopyCellAttributes(0, 0, 2, 1, 17, 8);
  } else {
    if (GameFlag_IsSet(FLAG_LEFT_BEAM_SHINING) != 0) {
      Actor_SetPosition(10, PIXELS(232), PIXELS(120));
      Map_CopyCellsTo(0, 59, 15, 38, 4, 3);
    }
    if (GameFlag_IsSet(FLAG_RIGHT_BEAM_SHINING) != 0) {
      Actor_SetPosition(12, PIXELS(344), PIXELS(120));
      Map_CopyCellsTo(4, 59, 17, 38, 4, 3);
    }
  }
  if (GameFlag_IsSet(0x80b) != 0) {
    Actor_SetPosition(9, PIXELS(504), PIXELS(152));
    Map_CopyCellsTo(2, 28, 34, 10, 2, 1);
    Map_CopyCellsTo(2, 30, 16, 10, 2, 1);
    Map_CopyCellsTo(0, 55, 32, 40, 4, 3);
  }
  if (GameFlag_IsSet(0x80c) != 0) {
    Actor_SetPosition(11, PIXELS(648), PIXELS(152));
    Map_CopyCellsTo(4, 28, 36, 10, 2, 1);
    Map_CopyCellsTo(4, 30, 18, 10, 2, 1);
    Map_CopyCellsTo(4, 55, 36, 40, 4, 3);
  }
  if (GameFlag_IsSet(0x80d) != 0) {
    Actor_SetPosition(13, PIXELS(504), PIXELS(200));
    Map_CopyCellsTo(2, 29, 34, 11, 2, 1);
    Map_CopyCellsTo(2, 31, 16, 11, 2, 1);
    Map_CopyCellsTo(0, 58, 32, 43, 4, 1);
  }
  if (GameFlag_IsSet(0x80e) != 0) {
    Actor_SetPosition(15, PIXELS(648), PIXELS(200));
    Map_CopyCellsTo(4, 29, 36, 11, 2, 1);
    Map_CopyCellsTo(4, 31, 18, 11, 2, 1);
    Map_CopyCellsTo(4, 58, 36, 43, 4, 1);
  }
  if (gGameState.entrance == 3) {
    if (GameFlag_IsSet(0x30a) != 0) {
      Actor_SetPosition(GARET, 0, 0);
      Actor_SetPosition(JENNA, 0, 0);
    } else if (GameFlag_IsSet(0x109) == 0) {
      FieldScene_SetupStagedActors();
      GameFlag_Set(0x30a);
    }
  }
  if (gGameState.entrance == 4) {
    if (GameFlag_IsSet(0x30b) != 0) {
      Actor_SetPosition(GARET, 0, 0);
      Actor_SetPosition(JENNA, 0, 0);
    } else if (GameFlag_IsSet(0x109) == 0) {
      FieldScene_RunStagedActorScene();
      GameFlag_Set(0x30b);
    }
  }
  if (GameFlag_IsSet(0x814) != 0) {
    BattleFx_SetQueuedSoundAndPlay(141);
    Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
    InitializeSceneRecordBuffer();
  }
  return 0;
}

static void Scene_RunGuardSequenceC(void) {
  u32 i;
  s32 value;
  s32 *p;
  s32 buf;

  p = (s32 *)sSpriteRows;
  buf = (s32)Runtime_AllocateBlock(14, 0x400);
  Resource_DecodeByteLz((s32)gSolStatue_SpriteRowsGfx, buf);
  value = Engine_VramLoad(Resource_FindFreeEntry(), 128, buf);
  for (i = 0; i < 9; i++) {
    s32 *q = p;

    *q++ = 0;
    *q++ = 0x40004000;
    p += 3;
    *q = value | 0xac00;
  }
  value = Engine_VramLoad(Resource_FindFreeEntry(), 128, buf + 128);
  for (i = 0; i < 9; i++) {
    s32 *q = p;

    *q++ = 0;
    *q++ = 0x40004000;
    p += 3;
    *q = value | 0xdc00;
  }
  value = Engine_VramLoad(Resource_FindFreeEntry(), 128, buf + 0x100);
  for (i = 0; i < 9; i++) {
    s32 *q = p;

    *q++ = 0;
    *q++ = 0x40004000;
    p += 3;
    *q = value | 0xc00;
  }
  Engine_HeapRelease(14);
  {
    s32 size = 0xc80;

    FieldScheduler_AddCallback(SceneEffect_UpdateScrollingSpriteRows, size);
  }
}

static void EV09_PSYNERGY(void) {
  if (SceneActor_IsActorAtTile(9, 31, 9) != 0) {
    SolStatue_OpenNorthwestSeal();
  }
}

static void EV11_PSYNERGY(void) {
  if (SceneActor_IsActorAtTile(11, 40, 9) != 0) {
    SolStatue_OpenNortheastSeal();
  }
}

static void EV13_PSYNERGY(void) {
  if (SceneActor_IsActorAtTile(13, 31, 12) != 0) {
    SolStatue_OpenSouthwestSeal();
  }
}

static void EV15_PSYNERGY(void) {
  if (SceneActor_IsActorAtTile(15, 40, 12) != 0) {
    SolStatue_OpenSoutheastSeal();
  }
}

static void EV10_PSYNERGY(void) {
  Field_SetMapCellCollision(2, 0x00d00000, 0x00700000, 0);
  if (SceneActor_IsActorAtTile(10, 14, 7) != 0) {
    Scene_ShineLeftBeam();
  }
}

static void EV12_PSYNERGY(void) {
  Field_SetMapCellCollision(2, 23068672, 7340032, 0);
  if (SceneActor_IsActorAtTile(12, 21, 7) != 0) {
    Scene_ShineRightBeam();
  }
}

static void EV17_SCRIPT(void) {
  struct FieldActor *first;
  s32 kind;
  struct FieldActor *second;

  first = Object_GetById(0);
  kind = first->x.fixed >> 20;
  second = Object_GetById(0);
  if ((second->z.fixed >> 20) == 8) {
    if ((u32)(kind - 17) <= 1) {
      Field_SetMapCellCollision(2, 0x1100000, 0x800000, 255);
      Field_SetMapCellCollision(2, 0x1200000, 0x800000, 255);
    }
  }
}

static void SolStatue_BlockLeftTriggerCell(void) {
  s32 x = Object_GetById(0)->x.fixed >> 20;
  s32 y = Object_GetById(0)->z.fixed >> 20;

  if (y == 7 && (u32)(x - 13) <= 1) {
    Field_SetMapCellCollision(2, 0xd00000, 0x700000, 255);
  }
}

static void EV12_SCRIPT(void) {
  s32 x = Object_GetById(0)->x.fixed >> 20;
  s32 y = Object_GetById(0)->z.fixed >> 20;

  if (y == 7 && (u32)(x - 21) <= 1) {
    Field_SetMapCellCollision(2, 0x1600000, 0x700000, 255);
  }
}

static void EV17_PSYNERGY(void) {
  struct FieldActor *statue;
  struct FieldActor *leader;
  s32 z;

  statue = Object_GetById(17);
  Field_SetMapCellCollision(2, 0x1100000, 0x800000, 0);
  Field_SetMapCellCollision(2, 0x1200000, 0x800000, 0);
  if (statue != NULL) {
    z = statue->z.fixed;
    Engine_EventBegin();
    if ((z >> 20) == 8) {
      if (Engine_GameFlagIsSet(0x207) == 0) {
        leader = Object_GetById(0);
        if ((u32)(leader->z.fixed >> 19) <= 17) {
          Actor_WalkToAndWait(0, 0x121, 158);
          leader = Object_GetById(0);
          leader->facing = FACING_NORTH;
        }
      }
      if (Engine_GameFlagIsSet(0x816) != 0 &&
          Engine_GameFlagIsSet(0x817) != 0) {
        Engine_GameFlagSet(0x818);
        Camera_SetSpeed(0x20000, 0x4000);
        Camera_MoveTo(0x11e0000, -1, 0x920000, 1);
        Engine_CameraWaitForMove();
        Object_GetById(17)->steeringFlags &= ~ACTOR_FACES_MOTION;
        Actor_SetSpeed(17, 0x30000, 0x10000);
        statue->motionFlags = 0;
        Engine_ActorSetSpritePriority(17, 3);
        Engine_AudioPlayCue(189);
        Engine_ActorSetDestination(17, 0x120, 178);
        Engine_EventWait(8);
        Actor_SetPosition(18, PIXELS(288), PIXELS(178));
        statue->targetX = ACTOR_NO_TARGET;
        statue->targetY = ACTOR_NO_TARGET;
        statue->targetZ = ACTOR_NO_TARGET;
        statue->x.fixed = 0;
        statue->y.fixed = 0;
        statue->z.fixed = 0;
        statue->velocityX = 0;
        statue->velocityY = 0;
        statue->velocityZ = 0;
        Engine_ActorSetPosition(17, 0, 0);
        Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
        Engine_EventWait(10);
        Engine_AudioPlayCue(141);
        Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
        Engine_EventWait(10);
        Work_SetValuesIfNonNegative(0x50000, 0x50000, 0x10000);
        Engine_EventWait(35);
        Work_SetValuesIfNonNegative(0x40000, 0x40000, 0x10000);
        Engine_EventWait(20);
        Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
        Engine_EventWait(30);
        Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
        Engine_EventWait(40);
        Engine_AudioPlayCue(0x121);
        Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
        Engine_EventWait(10);
        Work_SetValuesIfNonNegative(-1, -1, 0xe666);
        Engine_EventWait(60);
        Engine_AudioPlayCue(188);
        if (Engine_GameFlagIsSet(0x80b) != 0 &&
            Engine_GameFlagIsSet(0x80c) != 0 &&
            Engine_GameFlagIsSet(0x80d) != 0 &&
            Engine_GameFlagIsSet(0x80e) != 0) {
          Engine_GameFlagSet(0x80f);
        }
        Engine_EventWait(40);
        Engine_MessageShowCentered(MsgSolSomethingClicked, 1);
        Map_CopyCellAttributes(0, 1, 2, 1, 17, 8);
        Map_CopyCellAttributes(17, 9, 2, 1, 17, 7);
      }
    }
    Engine_EventEnd();
  }
}

static void EV32_TOUCH(void) {
  struct FieldActor *record;

  record = Object_GetById(17);
  if (record != 0) {
    if ((record->z.fixed >> 20) == 8) {
      Engine_EventBegin();
      Event_PlayCue(185);
      Actor_SetSpeed(17, 0x3333, 0x1999);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x3333, 0x1999);
      Object_GetById(17)->steeringFlags &= ~ACTOR_FACES_MOTION;
      Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 8);
      record = Object_GetById(0);
      Actor_SetDestination(ACTOR_PARTY_LEADER, record->x.part.pixel, 136);
      Actor_SetDestination(17, 0x120, 120);
      Engine_ActorWaitForMove(17);
      Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
      Engine_EventEnd();
    }
  }
}

static void EV05_TOUCH(void) {
  SolStatue_RunEventSequence(9, 31, 9, 30, 9);
  SolStatue_OpenNorthwestSeal();
}

static void EV06_TOUCH(void) {
  SolStatue_RunEventSequence(11, 40, 9, 41, 9);
  SolStatue_OpenNortheastSeal();
}

static void EV07_TOUCH(void) {
  SolStatue_RunEventSequence(13, 31, 12, 30, 12);
  SolStatue_OpenSouthwestSeal();
}

static void EV08_TOUCH(void) {
  SolStatue_RunEventSequence(15, 40, 12, 41, 12);
  SolStatue_OpenSoutheastSeal();
}

static void FieldScene_ApplyRect10_14_7_13_7(void) {
  SolStatue_RunEventSequence(10, 14, 7, 13, 7);
  Scene_ShineLeftBeam();
}

static void EV21_TOUCH(void) {
  struct FieldActor *record = Object_GetById(10);
  s32 cell;

  if (record == 0) {
    return;
  }

  cell = record->z.fixed >> 20;
  SolStatue_RunEventSequence(10, 13, cell + 1, 13, cell);
}

static void SceneActor_MoveActor10ByRow(void) {
  s32 *p = Object_GetById(10);
  if (p != NULL) {
    s32 v = p[4] >> 20;
    SolStatue_RunEventSequence(10, 13, v - 1, 13, v);
  }
}

static void FieldScene_ApplyRect12_21_7_22_7(void) {
  SolStatue_RunEventSequence(12, 21, 7, 22, 7);
  Scene_ShineRightBeam();
}

static void EV26_TOUCH(void) {
  s32 *p = Object_GetById(12);
  if (p != NULL) {
    s32 v = p[4] >> 20;
    SolStatue_RunEventSequence(12, 22, v + 1, 22, v);
  }
}

static void SceneActor_RunSlot12ColumnStep(void) {
  s32 *p = Object_GetById(12);
  if (p != NULL) {
    s32 v = p[4] >> 20;
    SolStatue_RunEventSequence(12, 22, v - 1, 22, v);
  }
}

static void SolStatue_RunEventSequence(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4) {
  s32 p10;
  s32 p10b;
  s32 p10c;
  s32 p8;
  s32 p8b;
  s32 p9;
  s32 p9b;

  p9 = a0;
  p8 = a1;
  p10 = a2;
  Engine_EventBegin();
  Engine_AudioPlayCue(185);
  Actor_SetSpeed(p9, 0x3333, 0x1999);
  Engine_ActorSetSpeed(0, 0x3333, 0x1999);
  Object_GetById(p9)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Engine_ActorSetAnimation(0, 8);
  Engine_ActorSetDestination(0, ((a3 << 4) + 8), ((a4 << 4) + 8));
  p8b = (p8 << 4);
  p10b = (p10 << 4);
  Engine_ActorSetDestination(p9, (p8b + 8), (p10b + 8));
  Engine_ActorWaitForMove(p9);
  Engine_ActorSetAnimation(0, ANIM_STAND);
  Engine_EventEnd();
  p9b = ((a3 << 4) + 8);
  p10c = ((a4 << 4) + 8);
}

static s32 SceneActor_IsActorAtTile(s32 no, s32 x, s32 z) {
  s32 *p = Object_GetById(no);
  if (p == NULL || (p[2] >> 20) != x) {
    return 0;
  }
  if ((p[4] >> 20) != z) {
    return 0;
  }
  return 1;
}

static void SolStatue_OpenNorthwestSeal(void) {
  s32 *p = (s32 *)&sSealScene;
  p[0] = 0;
  p[1] = 55;
  p[2] = 32;
  p[3] = 40;
  p[4] = 4;
  p[5] = 3;
  p[6] = 2;
  p[7] = 30;
  p[8] = 34;
  p[9] = 10;
  p[10] = 2;
  p[11] = 1;
  p[12] = 2;
  p[13] = 28;
  p[14] = 34;
  p[15] = 10;
  p[16] = 2;
  p[17] = 1;
  p[18] = 2;
  p[19] = 30;
  p[20] = 16;
  p[21] = 10;
  p[22] = 2;
  p[23] = 1;
  p[24] = 0x80b;
  p[25] = 0x4000;
  p[26] = 500;
  p[27] = 132;
  p[28] = 8;
  p[29] = 55;
  p[30] = 32;
  p[31] = 40;
  p[32] = 4;
  p[33] = 3;
  p[34] = 2;
  p[35] = 30;
  p[36] = 34;
  p[37] = 10;
  p[38] = 2;
  p[39] = 1;
  p[40] = 2;
  p[41] = 28;
  p[42] = 16;
  p[43] = 10;
  p[44] = 2;
  p[45] = 1;
  p[46] = 9;
  p[47] = 488;
  p[48] = 152;
  SolStatue_OpenSeal();
}

static void SolStatue_OpenNortheastSeal(void) {
  s32 *p = (s32 *)&sSealScene;
  p[0] = 4;
  p[1] = 55;
  p[2] = 36;
  p[3] = 40;
  p[4] = 4;
  p[5] = 3;
  p[6] = 4;
  p[7] = 30;
  p[8] = 36;
  p[9] = 10;
  p[10] = 2;
  p[11] = 1;
  p[12] = 4;
  p[13] = 28;
  p[14] = 36;
  p[15] = 10;
  p[16] = 2;
  p[17] = 1;
  p[18] = 4;
  p[19] = 30;
  p[20] = 18;
  p[21] = 10;
  p[22] = 2;
  p[23] = 1;
  p[24] = 0x80c;
  p[25] = 0x4000;
  p[26] = 654;
  p[27] = 132;
  p[28] = 12;
  p[29] = 55;
  p[30] = 36;
  p[31] = 40;
  p[32] = 4;
  p[33] = 3;
  p[34] = 4;
  p[35] = 30;
  p[36] = 36;
  p[37] = 10;
  p[38] = 2;
  p[39] = 1;
  p[40] = 4;
  p[41] = 28;
  p[42] = 18;
  p[43] = 10;
  p[44] = 2;
  p[45] = 1;
  p[46] = 11;
  p[47] = 664;
  p[48] = 152;
  SolStatue_OpenSeal();
}

static void SolStatue_OpenSouthwestSeal(void) {
  s32 *p = (s32 *)&sSealScene;
  p[0] = 0;
  p[1] = 58;
  p[2] = 32;
  p[3] = 43;
  p[4] = 4;
  p[5] = 1;
  p[6] = 2;
  p[7] = 31;
  p[8] = 34;
  p[9] = 11;
  p[10] = 2;
  p[11] = 1;
  p[12] = 2;
  p[13] = 29;
  p[14] = 34;
  p[15] = 11;
  p[16] = 2;
  p[17] = 1;
  p[18] = 2;
  p[19] = 31;
  p[20] = 16;
  p[21] = 11;
  p[22] = 2;
  p[23] = 1;
  p[24] = 0x80d;
  p[25] = 0xc000;
  p[26] = 500;
  p[27] = 216;
  p[28] = 8;
  p[29] = 58;
  p[30] = 32;
  p[31] = 43;
  p[32] = 4;
  p[33] = 1;
  p[34] = 2;
  p[35] = 31;
  p[36] = 34;
  p[37] = 11;
  p[38] = 2;
  p[39] = 1;
  p[40] = 2;
  p[41] = 29;
  p[42] = 16;
  p[43] = 11;
  p[44] = 2;
  p[45] = 1;
  p[46] = 13;
  p[47] = 488;
  p[48] = 200;
  SolStatue_OpenSeal();
}

static void SolStatue_OpenSoutheastSeal(void) {
  s32 *p = (s32 *)&sSealScene;
  p[0] = 4;
  p[1] = 58;
  p[2] = 36;
  p[3] = 43;
  p[4] = 4;
  p[5] = 1;
  p[6] = 4;
  p[7] = 31;
  p[8] = 36;
  p[9] = 11;
  p[10] = 2;
  p[11] = 1;
  p[12] = 4;
  p[13] = 29;
  p[14] = 36;
  p[15] = 11;
  p[16] = 2;
  p[17] = 1;
  p[18] = 4;
  p[19] = 31;
  p[20] = 18;
  p[21] = 11;
  p[22] = 2;
  p[23] = 1;
  p[24] = 0x80e;
  p[25] = 0xc000;
  p[26] = 0x28e;
  p[27] = 216;
  p[28] = 12;
  p[29] = 58;
  p[30] = 36;
  p[31] = 43;
  p[32] = 4;
  p[33] = 1;
  p[34] = 4;
  p[35] = 31;
  p[36] = 36;
  p[37] = 11;
  p[38] = 2;
  p[39] = 1;
  p[40] = 4;
  p[41] = 29;
  p[42] = 18;
  p[43] = 11;
  p[44] = 2;
  p[45] = 1;
  p[46] = 15;
  p[47] = 664;
  p[48] = 200;
  SolStatue_OpenSeal();
}

#define COPY_CELLS(c)                                                          \
  Map_CopyCellsTo((c).srcX, (c).srcY, (c).destX, (c).destY, (c).width,         \
                  (c).height)

static void SolStatue_OpenSeal(void) {
  struct SealScene *scene;
  s32 result;
  s32 i;

  result = 0;
  Engine_EventBegin();
  if (!Engine_GameFlagIsSet(0x80f)) {
    Engine_CameraSetSpeed(0x20000, 0x4000);
    Engine_CameraMoveTo(0x2400000, -1, 0xac0000, 1);
    Engine_CameraWaitForMove();
    Engine_AudioPlayCue(186);
    COPY_CELLS(sSealScene.open);
    for (i = 0; i != 20; i++) {
      Engine_AudioPlayCue(246);
      COPY_CELLS(sSealScene.pulseA);
      Engine_EventWait(4);
      Engine_AudioPlayCue(246);
      COPY_CELLS(sSealScene.pulseB);
      Engine_EventWait(4);
    }
    scene = &sSealScene;
    COPY_CELLS(scene->opened);
    Engine_GameFlagSet(scene->flag);
    result = SceneEventRuntime_SelectInitialSceneByFlags();
    if (result == -1) {
      if (!Engine_GameFlagIsSet(0x818)) {
        Engine_CameraFollowActor(0, 1);
        Object_GetById(0)->facing = scene->facing;
        Actor_SetSpeed(0, 0x20000, 0x20000);
        Object_GetById(0)->steeringFlags &= ~ACTOR_FACES_MOTION;
        Engine_ActorJump(0, 4, 0);
        Engine_ActorSetDestination(0, scene->destX, scene->destZ);
        COPY_CELLS(scene->stairs[0]);
        COPY_CELLS(scene->stairs[1]);
        COPY_CELLS(scene->stairs[2]);
        Object_GetById(scene->actor)->steeringFlags &= ~ACTOR_FACES_MOTION;
        Engine_ObjectMotionSetPositionAndCommit(scene->actor, scene->actorX,
                                                scene->actorZ);
        Object_GetById(0)->steeringFlags |= ACTOR_FACES_MOTION;
        Engine_GameFlagClear(scene->flag);
      }
    } else if (result == 0 && Engine_GameFlagIsSet(0x818)) {
      if (Engine_GameFlagIsSet(0x80b) && Engine_GameFlagIsSet(0x80d) &&
          Engine_GameFlagIsSet(0x80e)) {
        if (!GameFlag_IsSet(0x80f)) {
          Engine_GameFlagSet(0x80f);
          SolStatue_RunSealOpenedSequence();
        }
      } else if (Engine_GameFlagIsSet(0x812)) {
        Engine_EventRequestExit(5);
        result = 1;
      }
    }
  }
  if (result == 1) {
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
  }
  Engine_EventEnd();
}

static void Scene_ShineLeftBeam(void) {
  Engine_EventBegin();
  if (GameFlag_IsSet(0x818) == 0) {
    if (GameFlag_IsSet(FLAG_LEFT_BEAM_SHINING) == 0) {
      Camera_SetSpeed(0x20000, 0x4000);
      Camera_MoveTo(0x11e0000, -1, 0x920000, 1);
      Engine_CameraWaitForMove();
      Event_PlayCue(186);
      Map_CopyCellsTo(0, 59, 15, 38, 4, 3);
      if (GameFlag_IsSet(FLAG_RIGHT_BEAM_SHINING) != 0) {
        Map_CopyCellsTo(8, 60, 17, 39, 2, 2);
      }
      Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
      Engine_EventWait(30);
      GameFlag_Set(FLAG_LEFT_BEAM_SHINING);
      if (GameFlag_IsSet(FLAG_RIGHT_BEAM_SHINING) != 0) {
        Scene_OpenTheHole();
      }
    }
  }
  Engine_EventEnd();
}

static void Scene_ShineRightBeam(void) {
  Engine_EventBegin();
  if (GameFlag_IsSet(0x818) == 0) {
    if (GameFlag_IsSet(FLAG_RIGHT_BEAM_SHINING) == 0) {
      Camera_SetSpeed(0x20000, 0x4000);
      Camera_MoveTo(0x11e0000, -1, 0x920000, 1);
      Engine_CameraWaitForMove();
      Event_PlayCue(186);
      Map_CopyCellsTo(4, 59, 17, 38, 4, 3);
      if (GameFlag_IsSet(FLAG_LEFT_BEAM_SHINING) != 0) {
        Map_CopyCellsTo(8, 60, 17, 39, 2, 2);
      }
      Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
      Engine_EventWait(30);
      GameFlag_Set(FLAG_RIGHT_BEAM_SHINING);
      if (GameFlag_IsSet(FLAG_LEFT_BEAM_SHINING) != 0) {
        Scene_OpenTheHole();
      }
    }
  }
  Engine_EventEnd();
}

static void Event_SayThenWait(s32 speaker, s32 frames) {
  Engine_EventShowMessage(speaker, 0);
  Engine_EventWait(frames);
}

struct SceneEntrance gSceneEventRuntime_ScriptData[] = {
    {0, CONDITION_ALWAYS, 287, 0, 176, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 646, 0, 329, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {2, CONDITION_ALWAYS, 647, 0, 470, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {3, CONDITION_ALWAYS, 576, 0, 136, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {4, CONDITION_ALWAYS, 576, 0, 136, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {5, CONDITION_ALWAYS, 576, 0, 136, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {6, CONDITION_ALWAYS, 576, 0, 136, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {99, CONDITION_ALWAYS, 576, 0, 136, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneEventRuntime_MessageData[] = {
    SCENE_EXITS(12),      SCENE_EXIT(1, 11, 2),   SCENE_EXIT(2, 13, 1),
    SCENE_EXIT(3, 11, 4), SCENE_EXIT(4, 11, 5),   SCENE_EXIT(5, 11, 6),
    SCENE_EXIT(6, 11, 7), SCENE_EXIT(10, 510, 1), SCENE_EXITS_END,
};

// clang-format off
struct ScenePlacement gSceneEventRuntime_ActorData[] = {
    { 0x1e, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1608), 0, PIXELS(1608), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xc9, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(488), 0, PIXELS(152), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0xc9, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(216), 0, PIXELS(152), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xc9, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(664), 0, PIXELS(152), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0xc9, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(360), 0, PIXELS(152), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xc9, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(488), 0, PIXELS(200), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0xc9, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(216), 0, PIXELS(216), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xc9, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(664), 0, PIXELS(200), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0xc9, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(360), 0, PIXELS(216), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xca, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(288), 0, PIXELS(120), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xcc, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1608), 0, PIXELS(1608), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { JENNA, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(560), 0, PIXELS(136), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(592), 0, PIXELS(136), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};
// clang-format on

struct SceneEvent gSceneEventRuntime_EffectData[] = {
    {0x31, 1, CONDITION_ALWAYS, 1},
    {EVENT_EXIT, 2, CONDITION_ALWAYS, 2},
    {0x202, 10, CONDITION_ALWAYS,
     EVENT_SCRIPT(SceneState_RunWhenSlotZeroFacingC000)},
    {0x202, 11, CONDITION_ALWAYS, EVENT_SCRIPT(EV11_SCRIPT)},
    {PSYNERGY_ON_ACTOR(0x8c), 9, 0x80b, EVENT_SCRIPT(EV09_PSYNERGY)},
    {PSYNERGY_ON_ACTOR(0x8c), 11, 0x80c, EVENT_SCRIPT(EV11_PSYNERGY)},
    {PSYNERGY_ON_ACTOR(0x8c), 13, 0x80d, EVENT_SCRIPT(EV13_PSYNERGY)},
    {PSYNERGY_ON_ACTOR(0x8c), 15, 0x80e, EVENT_SCRIPT(EV15_PSYNERGY)},
    {TOUCH_WALKING_INTO(FACING_EAST), 5, 0x80b, EVENT_SCRIPT(EV05_TOUCH)},
    {TOUCH_WALKING_INTO(FACING_WEST), 6, 0x80c, EVENT_SCRIPT(EV06_TOUCH)},
    {TOUCH_WALKING_INTO(FACING_EAST), 7, 0x80d, EVENT_SCRIPT(EV07_TOUCH)},
    {TOUCH_WALKING_INTO(FACING_WEST), 8, 0x80e, EVENT_SCRIPT(EV08_TOUCH)},
    {0x10008c15, 10, 0x816, EVENT_SCRIPT(SolStatue_BlockLeftTriggerCell)},
    {PSYNERGY_ON_ACTOR(0x8c), 10, 0x816, EVENT_SCRIPT(EV10_PSYNERGY)},
    {0x10008c15, 12, 0x817, EVENT_SCRIPT(EV12_SCRIPT)},
    {PSYNERGY_ON_ACTOR(0x8c), 12, 0x817, EVENT_SCRIPT(EV12_PSYNERGY)},
    {TOUCH_WALKING_INTO(FACING_EAST), 20, 0x816,
     EVENT_SCRIPT(FieldScene_ApplyRect10_14_7_13_7)},
    {TOUCH_WALKING_INTO(FACING_EAST), 21, 0x816,
     EVENT_SCRIPT(FieldScene_ApplyRect10_14_7_13_7)},
    {TOUCH_WALKING_INTO(FACING_SOUTH), 21, 0x816, EVENT_SCRIPT(EV21_TOUCH)},
    {TOUCH_WALKING_INTO(FACING_NORTH), 23, 0x816,
     EVENT_SCRIPT(SceneActor_MoveActor10ByRow)},
    {TOUCH_WALKING_INTO(FACING_NORTH), 24, 0x816,
     EVENT_SCRIPT(SceneActor_MoveActor10ByRow)},
    {TOUCH_WALKING_INTO(FACING_WEST), 25, 0x817,
     EVENT_SCRIPT(FieldScene_ApplyRect12_21_7_22_7)},
    {TOUCH_WALKING_INTO(FACING_WEST), 26, 0x817,
     EVENT_SCRIPT(FieldScene_ApplyRect12_21_7_22_7)},
    {TOUCH_WALKING_INTO(FACING_SOUTH), 26, 0x817, EVENT_SCRIPT(EV26_TOUCH)},
    {TOUCH_WALKING_INTO(FACING_NORTH), 28, 0x817,
     EVENT_SCRIPT(SceneActor_RunSlot12ColumnStep)},
    {TOUCH_WALKING_INTO(FACING_NORTH), 29, 0x817,
     EVENT_SCRIPT(SceneActor_RunSlot12ColumnStep)},
    {0x10008c15, 17, 0x818, EVENT_SCRIPT(EV17_SCRIPT)},
    {PSYNERGY_ON_ACTOR(0x8c), 17, 0x818, EVENT_SCRIPT(EV17_PSYNERGY)},
    {TOUCH_WALKING_INTO(FACING_NORTH), 32, 0x818, EVENT_SCRIPT(EV32_TOUCH)},
    {SCENE_EVENTS_END},
};

s32 gSolStatue_ScrollPhase = 0;

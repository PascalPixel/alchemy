#include "game_state.h"
#include "version.h"
#include "unit/scene/sol_sanctum_statue.h"
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
#include "animspr.h"
#include "map_scroll.h"
#include "fixed_math.h"
#include "resource_37de_scene.h"

#if TBS_PATCH_3
#define SOL_SANCTUM_STATUE_BACK_SCROLL_ROW_Y_OFFSET 6
#else
#define SOL_SANCTUM_STATUE_BACK_SCROLL_ROW_Y_OFFSET 8
#endif

#define SOL_SANCTUM_STATUE_ROW_SPRITE_COUNT 9
#define SOL_SANCTUM_STATUE_SPRITE_WIDTH 32

// Forward declarations.
static void SolSanctumStatue_InitializeScrollingSprites(void);
static void SolSanctumStatue_UpdateCenterStatue(void);
static void SolSanctumStatue_PushStatue(s32 statueId, s32 statueColumn, s32 statueRow,
                                 s32 leaderColumn, s32 leaderRow);
static s32 SolSanctumStatue_IsActorAtCell(s32 actorId, s32 column, s32 row);
static void SolSanctumStatue_OpenNorthwestSeal(void);
static void SolSanctumStatue_OpenNortheastSeal(void);
static void SolSanctumStatue_OpenSouthwestSeal(void);
static void SolSanctumStatue_OpenSoutheastSeal(void);
static void SolSanctumStatue_OpenSeal(void);
static void SolSanctumStatue_ShineLeftBeam(void);
static void SolSanctumStatue_ShineRightBeam(void);
static void Event_SayThenWait(s32 speaker, s32 frames);

enum StatueHallFlag {
  FLAG_NORTHWEST_SEAL_OPEN = 0x80b,
  FLAG_NORTHEAST_SEAL_OPEN = 0x80c,
  FLAG_SOUTHWEST_SEAL_OPEN = 0x80d,
  FLAG_SOUTHEAST_SEAL_OPEN = 0x80e,
  FLAG_ALL_STATUE_SEALS_OPEN = 0x80f,
  FLAG_STATUE_TRAP_SPRUNG = 0x813,
  FLAG_LEFT_BEAM_SHINING = 0x816,
  FLAG_RIGHT_BEAM_SHINING = 0x817,
  FLAG_CENTER_STATUE_MOVED = 0x818,
  FLAG_CENTRAL_HOLE_OPEN = 0x207
};

#define NULL ((void *)0)

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

enum StatueHallActor { ACTOR_KRADEN = ACTOR_FIRST_PLACED };

// bss in this order: as commons of one object the linker would sort them itself
static struct SealScene sSealScene;
// Initialization writes each three-word OAM packet; drawing updates its fields.
static union {
  struct AnimationSpritePart parts[3 * SOL_SANCTUM_STATUE_ROW_SPRITE_COUNT];
  s32 words[3 * SOL_SANCTUM_STATUE_ROW_SPRITE_COUNT * 3];
} sScrollingSprites;

// Return the statue hall arrival positions.
struct SceneEntrance *SolSanctumStatue_GetEntrances(void) {
  return gSolSanctumStatue_Entrances;
}

// The statue hall has no separate trigger-region table.
struct SceneRegion *SolSanctumStatue_GetRegions(void) { return NULL; }

// Return the statue hall scene destinations.
u32 *SolSanctumStatue_GetExits(void) { return gSolSanctumStatue_Exits; }

// Return the statues and companions placed in the hall.
struct ScenePlacement *SolSanctumStatue_GetPlacements(void) {
  return gSolSanctumStatue_Placements;
}

// Return the movement triggers and statue interactions.
struct SceneEvent *SolSanctumStatue_GetEvents(void) { return gSolSanctumStatue_Events; }

// Choose the next story scene from the saved state of the hall.
static s32 SolSanctumStatue_SelectStoryExit(void) {
  if (Engine_GameFlagIsSet(FLAG_CENTER_STATUE_MOVED) != 0) {
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

// Animate the floor opening after both light beams meet.
static void SolSanctumStatue_OpenCentralHole(void) {
  s32 flash;

  {
    Map_CopyCellsTo(0, 28, 17, 8, 2, 1);
  }
  Engine_AudioPlayCue(200);
  for (flash = 0; flash != 22; flash++) {
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
  Engine_GameFlagSet(FLAG_CENTRAL_HOLE_OPEN);
  SolSanctumStatue_UpdateCenterStatue();
}

// Show Kraden discovering the hall and examining its two sides.
static void SolSanctumStatue_VisitStatueHall(void) {
  struct FieldActor *leader;

  Engine_EventBegin();
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  gEventWork->transitionFrames = 32;
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(20);
  Actor_SetPosition(ACTOR_KRADEN, PIXELS(576), PIXELS(232));
  // Kraden crosses the hall while the camera follows each side.
  Engine_EventWait(1);
  Engine_EventSetMessage(MsgSolSanctumKradenYouFoundIt);
  Event_SayThenWait(ACTOR_KRADEN, 6);
  Actor_SetPosition(ACTOR_KRADEN, PIXELS(576), PIXELS(280));
  Camera_SetSpeed(PIXELS(4) / 5, PIXELS(1) / 10);
  Camera_MoveTo(PIXELS(574), -1, PIXELS(180), 1);
  Actor_SetSpeed(ACTOR_KRADEN, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(ACTOR_KRADEN, 576, 216);
  Engine_EventWait(20);
  Engine_ActorJump(JENNA, 2, 0);
  Engine_EventWait(30);
  Event_SayThenWait(JENNA, 6);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
  Engine_EventWait(6);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST + FACING_STEP, 0);
  Engine_EventWait(10);
  Camera_SetSpeed(PIXELS(28) / 5, PIXELS(7) / 10);
  Camera_MoveTo(PIXELS(287), -1, PIXELS(176), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(60);
  Camera_MoveTo(PIXELS(574), -1, PIXELS(180), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
  Engine_EventWait(10);
  Engine_ActorJump(ACTOR_KRADEN, 6, 0);
  Actor_SetSpeed(ACTOR_KRADEN, PIXELS(3), PIXELS(2));
  Actor_WalkToAndWait(ACTOR_KRADEN, 576, 184);
  Engine_EventWait(40);
  Event_SayThenWait(ACTOR_KRADEN, 6);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
  Engine_EventWait(40);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
  gEventWork->transitionFrames = 16;
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Camera_MoveTo(PIXELS(287), -1, PIXELS(176), 0);
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
  Camera_MoveTo(PIXELS(574), -1, PIXELS(180), 0);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(20);
  Camera_SetSpeed(PIXELS(6) / 5, PIXELS(3) / 20);
  Camera_MoveTo(PIXELS(574), -1, PIXELS(157), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  // Let the party question Kraden before agreeing to investigate.
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
  Camera_SetSpeed(PIXELS(8) / 5, PIXELS(1) / 5);
  Camera_MoveTo(PIXELS(576), -1, PIXELS(215), 1);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 0);
  Engine_EventWait(10);
  Engine_ActorJump(ACTOR_KRADEN, 6, 0);
  Actor_WalkToAndWait(ACTOR_KRADEN, 576, 217);
  Engine_EventWait(20);
  Actor_WalkToAndWait(ACTOR_KRADEN, 576, 321);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  Camera_SetSpeed(PIXELS(18) / 5, PIXELS(9) / 20);
  Camera_MoveTo(PIXELS(576), -1, PIXELS(136), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  // Rejoin the companions to the leader and restore normal field control.
  Actor_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(JENNA, PIXELS(1), PIXELS(1) / 2);
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

// Have Kraden explain the statues before leaving the puzzle to Isaac.
static void SolSanctumStatue_ExplainStatuePuzzle(void) {
  struct EventWork *work;
  struct FieldActor *leader;

  Engine_EventBegin();
  work = gEventWork;
  work->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  work->transitionFrames = 32;
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(20);
  Engine_ActorSetPosition(8, PIXELS(576), PIXELS(296));
  Engine_EventWait(1);
  Engine_EventSetMessage(MsgSolSanctumWait);
  // Kraden returns to the statues and demonstrates the puzzle's layout.
  Event_SayThenWait(8, 6);
  Camera_SetSpeed(PIXELS(4) / 5, PIXELS(1) / 10);
  Camera_MoveTo(PIXELS(574), -1, PIXELS(180), 1);
  Actor_SetSpeed(8, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(8, 576, 216);
  Engine_EventWait(20);
  Engine_ActorJump(5, 2, 0);
  Engine_EventWait(30);
  Event_SayThenWait(5, 6);
  Engine_ActorRunRepeatedMotion(8, 2);
  Engine_EventWait(6);
  Engine_ActorFaceDirection(8, FACING_WEST + FACING_STEP, 0);
  Engine_EventWait(10);
  Engine_CameraSetSpeed(PIXELS(28) / 5, PIXELS(7) / 10);
  Camera_MoveTo(PIXELS(287), -1, PIXELS(176), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(60);
  Camera_MoveTo(PIXELS(574), -1, PIXELS(180), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(8, 3);
  Engine_EventWait(10);
  Actor_FaceDirection(8, FACING_NORTH, 0);
  Engine_EventWait(10);
  Engine_ActorJump(8, 6, 0);
  Actor_SetSpeed(8, PIXELS(3), PIXELS(2));
  Actor_WalkToAndWait(8, 576, 184);
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
  Actor_WalkToAndWait(8, 576, 216);
  Actor_FaceDirection(8, FACING_WEST, 0);
  Engine_EventWait(10);
  Actor_SetSpeed(8, PIXELS(6) / 5, PIXELS(3) / 5);
  Engine_ActorWalkTo(8, 432, 200);
  Engine_EventWait(20);
  Camera_SetSpeed(PIXELS(12) / 5, PIXELS(3) / 10);
  Camera_MoveTo(PIXELS(288), -1, PIXELS(171), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(80);
  Engine_ActorSetAnimation(8, 1);
  Camera_MoveTo(PIXELS(574), -1, PIXELS(180), 1);
  Engine_EventWait(20);
  Engine_ActorFaceDirection(8, FACING_EAST, 0);
  Engine_EventWait(30);
  Actor_WalkToAndWait(8, 576, 216);
  Actor_FaceDirection(8, FACING_NORTH, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(0, FACING_SOUTH, 0);
  Actor_FaceDirection(5, FACING_SOUTH, 0);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  Engine_CameraSetSpeed(PIXELS(4) / 5, PIXELS(1) / 10);
  Engine_CameraMoveTo(PIXELS(574), -1, PIXELS(171), 1);
  Engine_ActorJump(8, 6, 0);
  Actor_SetSpeed(8, PIXELS(3), PIXELS(2));
  Engine_ActorWalkToAndWait(8, 576, 184);
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
  // Ask whether Isaac understands, then finish the explanation.
  Actor_FaceDirection(0, FACING_SOUTH, 0);
  Actor_FaceDirection(5, FACING_SOUTH, 0);
  Actor_FaceDirection(1, FACING_SOUTH, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventSetMessage(MsgSolSanctumHonestlyDoubtUnderstand);
  } else {
    Engine_EventSetMessage(MsgSolSanctumHmphWellTold);
  }
  Event_SayThenWait(8, 6);
  Engine_EventSetMessage(MsgSolSanctumTryFindSolution);
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
  Actor_WalkToAndWait(8, 576, 216);
  Engine_EventWait(40);
  Engine_CameraSetSpeed(PIXELS(4) / 5, PIXELS(1) / 10);
  Camera_MoveTo(PIXELS(574), -1, PIXELS(191), 1);
  Actor_SetSpeed(8, PIXELS(1), PIXELS(1) / 2);
  Engine_ActorWalkToAndWait(8, 576, 232);
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(8, 2);
  Engine_EventWait(40);
  Event_SayThenWait(8, 6);
  Engine_ActorFaceDirection(8, FACING_NORTH, 0);
  Engine_EventWait(30);
  Event_SayThenWait(8, 6);
  Engine_ActorSetAnimationAndWait(8, 3);
  Camera_MoveTo(PIXELS(576), -1, PIXELS(215), 1);
  Actor_WalkToAndWait(8, 574, 323);
  Engine_ActorSetPosition(8, 0, 0);
  // Leave Isaac in control after Jenna and Garet rejoin him.
  Camera_SetSpeed(PIXELS(18) / 5, PIXELS(9) / 20);
  Engine_CameraMoveTo(PIXELS(576), -1, PIXELS(136), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  Engine_ActorFaceDirection(5, FACING_EAST, 0);
  Engine_EventWait(10);
  Event_SayThenWait(5, 6);
  Engine_ActorSetAnimation(1, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(5, 3);
  Actor_SetSpeed(1, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(5, PIXELS(1), PIXELS(1) / 2);
  Engine_ActorSetAnimation(JENNA, 2);
  leader = Object_GetById(0);
  if (leader != 0) {
    Engine_ActorSetDestination(5, leader->x.part.pixel, leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(5);
  Engine_ActorSetPosition(5, 0, 0);
  Engine_ActorSetAnimation(1, ANIM_WALK);
  leader = Object_GetById(0);
  if (leader != 0) {
    Engine_ActorSetDestination(1, leader->x.part.pixel, leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(1);
  Engine_ActorSetPosition(1, 0, 0);
  GameFlag_Clear(FLAG_ARRIVAL_EVENT_PENDING);
  work = gEventWork;
  work->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  work->transitionFrames = 16;
  Engine_EventEnd();
}

// Pan toward the central seal, open it, then restore the camera origin.
static void SolSanctumStatue_RunSealOpenedSequence(void) {
  struct MapScrollWork *map;
  struct FieldActor *leader;
  s32 *savedOrigin;
  s32 cameraPosition[3];
  s32 approachSide;
  s32 frame;

  map = gMapWork[0];
  leader = Object_GetById(0);
  // Temporarily follow a local point, panning from the leader toward the seal.
  if (leader->z.fixed < PIXELS(179)) {
    Actor_WalkToAndWait(0, 575, 132);
    Actor_FaceDirection(0, FACING_SOUTH, 0);
    Engine_EventWait(30);
    savedOrigin = map->origin;
    cameraPosition[0] = leader->x.fixed;
    cameraPosition[1] = leader->y.fixed;
    cameraPosition[2] = leader->z.fixed;
    map->origin = cameraPosition;
    for (frame = 0; frame != 30; frame++) {
      cameraPosition[2] += PIXELS(1);
      Engine_EventWait(1);
    }
    Engine_EventWait(40);
    approachSide = 1;
  } else {
    Actor_WalkToAndWait(0, 577, 222);
    Actor_FaceDirection(0, FACING_NORTH, 0);
    Engine_EventWait(30);
    cameraPosition[0] = leader->x.fixed;
    cameraPosition[1] = leader->y.fixed;
    cameraPosition[2] = leader->z.fixed;
    savedOrigin = map->origin;
    map->origin = cameraPosition;
    for (frame = 0; frame != 30; frame++) {
      cameraPosition[2] -= PIXELS(1);
      Engine_EventWait(1);
    }
    Engine_EventWait(40);
    approachSide = 2;
  }
  // Flash progressively faster before uncovering the stairway.
  for (frame = 0; frame != 6; frame++) {
    Map_CopyCellsTo(2, 28, 34, 10, 4, 2);
    Engine_EventWait(8);
    Map_CopyCellsTo(2, 30, 34, 10, 4, 2);
    Engine_EventWait(8);
  }
  for (frame = 0; frame != 10; frame++) {
    Map_CopyCellsTo(2, 28, 34, 10, 4, 2);
    Engine_EventWait(4);
    Map_CopyCellsTo(2, 30, 34, 10, 4, 2);
    Engine_EventWait(4);
  }
  for (frame = 0; frame != 12; frame++) {
    Map_CopyCellsTo(2, 28, 34, 10, 4, 2);
    Engine_EventWait(2);
    Map_CopyCellsTo(2, 30, 34, 10, 4, 2);
    Engine_EventWait(2);
  }
  Map_CopyCellsTo(2, 28, 34, 10, 4, 2);
  Map_CopyCellsTo(8, 55, 32, 40, 8, 4);
  Engine_EventWait(60);
  // Reverse the pan before restoring the caller's camera target.
  if (approachSide == 1) {
    for (frame = 0; frame != 30; frame++) {
      cameraPosition[2] -= PIXELS(1);
      Engine_EventWait(1);
    }
  } else if (approachSide == 2) {
    for (frame = 0; frame != 30; frame++) {
      cameraPosition[2] += PIXELS(1);
      Engine_EventWait(1);
    }
  }
  map->origin = savedOrigin;
}

// Draw the three parallax rows while they overlap the screen.
static void SolSanctumStatue_UpdateScrollingSprites(void) {
  struct MapScrollWork *map = gMapWork[0];
  struct AnimationSpritePart *sprite = sScrollingSprites.parts;
#ifdef UBFIX
  s32 cameraX = map->viewX / PIXELS(1);
  s32 screenY = 80 - map->viewY / PIXELS(1);
#else
  // UB: viewX and viewY are separate scalar members, not an array. Reading
  // them by name adds four bytes in the native EN build (2026-10-16).
  s32 *view = &map->viewX;
  s32 cameraX = view[0] / PIXELS(1);
  s32 screenY = 80 - view[1] / PIXELS(1);
#endif
  s32 scrollX;
  u32 column;

  if ((u32)(screenY + 16) <= 175) {
    // Keep the first sprite just left of the screen for seamless tiling.
    scrollX = (gSolSanctumStatue_ScrollPhase >> 10) - cameraX;
    scrollX |= -SOL_SANCTUM_STATUE_SPRITE_WIDTH;
    for (column = 0; column < SOL_SANCTUM_STATUE_ROW_SPRITE_COUNT; column++) {
      sprite->x = scrollX;
      sprite->y = screenY;
      Runtime_PushSlotEntry(sprite, 0);
      scrollX += SOL_SANCTUM_STATUE_SPRITE_WIDTH;
      sprite++;
    }
    scrollX = (gSolSanctumStatue_ScrollPhase >> 9) - cameraX;
    scrollX |= -SOL_SANCTUM_STATUE_SPRITE_WIDTH;
    for (column = 0; column < SOL_SANCTUM_STATUE_ROW_SPRITE_COUNT; column++) {
      sprite->x = scrollX;
      sprite->y = screenY;
      Runtime_PushSlotEntry(sprite, 0);
      scrollX += SOL_SANCTUM_STATUE_SPRITE_WIDTH;
      sprite++;
    }
    scrollX = (gSolSanctumStatue_ScrollPhase >> 8) - cameraX;
    scrollX |= -SOL_SANCTUM_STATUE_SPRITE_WIDTH;
    for (column = 0; column < SOL_SANCTUM_STATUE_ROW_SPRITE_COUNT; column++) {
      sprite->x = scrollX;
      sprite->y = screenY + SOL_SANCTUM_STATUE_BACK_SCROLL_ROW_Y_OFFSET;
      Runtime_PushSlotEntry(sprite, 0);
      scrollX += SOL_SANCTUM_STATUE_SPRITE_WIDTH;
      sprite++;
    }
  }
  // The three shifts turn this into 1/8, 1/4 and 1/2 pixel drift per frame.
  gSolSanctumStatue_ScrollPhase += 128;
}

// Check the passage ahead when the leader is facing north.
static void SolSanctumStatue_CheckNorthPassage(void) {
  struct FieldActor *leader = Object_GetById(0);
  if (leader->facing == FACING_NORTH) {
    Engine_LeaderCheckAhead();
  }
}

// Check the passage ahead when the leader is facing south.
static void SolSanctumStatue_CheckSouthPassage(void) {
  struct FieldActor *leader = Object_GetById(0);
  if (leader->facing == FACING_SOUTH) {
    Engine_LeaderCheckAhead();
  }
}

// Unused sequence moving the northwest statue to its seal position.
UNUSED static void SolSanctumStatue_MoveUnusedNorthwestStatue(void) {
  if (GameFlag_IsSet(FLAG_NORTHWEST_SEAL_OPEN) == 0) {
    Engine_EventBegin();
    Object_GetById(9);
    Actor_SetSpeed(9, PIXELS(1) / 5, PIXELS(1) / 10);
    Actor_WalkToAndWait(9, 504, 152);
    Engine_EventEnd();
  }
}

// Restore the hall graphics, statues and arrival conversations.
s32 SolSanctumStatue_Initialize(void) {
  struct FieldActor *centerEffect;

  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  SolSanctumStatue_InitializeScrollingSprites();
  GameFlag_Set(0x144);
  centerEffect = Object_GetById(18);
  centerEffect->collisionFlags = 0;
  {
    u8 flags = centerEffect->priorityFlags | 2;

    centerEffect->priorityFlags = flags;
  }
  Engine_ActorSetSpriteFlags(Object_GetById(18), 0);
  Object_GetById(18)->priorityFlags &= 254;
  Engine_ActorSetSpritePriority(18, 1);
  // Only arrivals 3 and 4 keep Jenna and Garet staged for their conversations.
  if ((u16)(gGameState.entrance - 3) > 1) {
    Actor_SetPosition(JENNA, 0, 0);
    Actor_SetPosition(GARET, 0, 0);
  }
  if (GameFlag_IsSet(FLAG_CENTER_STATUE_MOVED) != 0) {
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
  if (GameFlag_IsSet(FLAG_NORTHWEST_SEAL_OPEN) != 0) {
    Actor_SetPosition(9, PIXELS(504), PIXELS(152));
    Map_CopyCellsTo(2, 28, 34, 10, 2, 1);
    Map_CopyCellsTo(2, 30, 16, 10, 2, 1);
    Map_CopyCellsTo(0, 55, 32, 40, 4, 3);
  }
  if (GameFlag_IsSet(FLAG_NORTHEAST_SEAL_OPEN) != 0) {
    Actor_SetPosition(11, PIXELS(648), PIXELS(152));
    Map_CopyCellsTo(4, 28, 36, 10, 2, 1);
    Map_CopyCellsTo(4, 30, 18, 10, 2, 1);
    Map_CopyCellsTo(4, 55, 36, 40, 4, 3);
  }
  if (GameFlag_IsSet(FLAG_SOUTHWEST_SEAL_OPEN) != 0) {
    Actor_SetPosition(13, PIXELS(504), PIXELS(200));
    Map_CopyCellsTo(2, 29, 34, 11, 2, 1);
    Map_CopyCellsTo(2, 31, 16, 11, 2, 1);
    Map_CopyCellsTo(0, 58, 32, 43, 4, 1);
  }
  if (GameFlag_IsSet(FLAG_SOUTHEAST_SEAL_OPEN) != 0) {
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
      SolSanctumStatue_VisitStatueHall();
      GameFlag_Set(0x30a);
    }
  }
  if (gGameState.entrance == 4) {
    if (GameFlag_IsSet(0x30b) != 0) {
      Actor_SetPosition(GARET, 0, 0);
      Actor_SetPosition(JENNA, 0, 0);
    } else if (GameFlag_IsSet(0x109) == 0) {
      SolSanctumStatue_ExplainStatuePuzzle();
      GameFlag_Set(0x30b);
    }
  }
  if (GameFlag_IsSet(0x814) != 0) {
    BattleFx_SetQueuedSoundAndPlay(141);
    Map_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
    InitializeSceneRecordBuffer();
  }
  return 0;
}

// Load three graphic strips and initialize their scrolling sprite rows.
static void SolSanctumStatue_InitializeScrollingSprites(void) {
  u32 column;
  s32 tile;
  s32 *spriteWords;
  u8 *pixels;

  spriteWords = sScrollingSprites.words;
  pixels = Runtime_AllocateBlock(14, 1024);
  Resource_DecodeByteLz(gSolSanctumStatue_SpriteRowsGfx, pixels);
  tile = Engine_VramLoad(Resource_FindFreeEntry(), 128, pixels);
  for (column = 0; column < SOL_SANCTUM_STATUE_ROW_SPRITE_COUNT; column++) {
    s32 *attributeWords = spriteWords;

    *attributeWords++ = 0;
    *attributeWords++ = 0x40004000;
    spriteWords += 3;
    *attributeWords = tile | 0xac00;
  }
  tile = Engine_VramLoad(Resource_FindFreeEntry(), 128, pixels + 128);
  for (column = 0; column < SOL_SANCTUM_STATUE_ROW_SPRITE_COUNT; column++) {
    s32 *attributeWords = spriteWords;

    *attributeWords++ = 0;
    *attributeWords++ = 0x40004000;
    spriteWords += 3;
    *attributeWords = tile | 0xdc00;
  }
  tile = Engine_VramLoad(Resource_FindFreeEntry(), 128, pixels + 256);
  for (column = 0; column < SOL_SANCTUM_STATUE_ROW_SPRITE_COUNT; column++) {
    s32 *attributeWords = spriteWords;

    *attributeWords++ = 0;
    *attributeWords++ = 0x40004000;
    spriteWords += 3;
    *attributeWords = tile | 0xc00;
  }
  Engine_HeapRelease(14);
  {
    s32 priority = 3200;

    FieldScheduler_AddCallback(SolSanctumStatue_UpdateScrollingSprites, priority);
  }
}

// Open the northwest seal when its statue reaches the target cell.
static void SolSanctumStatue_TryNorthwestSeal(void) {
  if (SolSanctumStatue_IsActorAtCell(9, 31, 9) != 0) {
    SolSanctumStatue_OpenNorthwestSeal();
  }
}

// Open the northeast seal when its statue reaches the target cell.
static void SolSanctumStatue_TryNortheastSeal(void) {
  if (SolSanctumStatue_IsActorAtCell(11, 40, 9) != 0) {
    SolSanctumStatue_OpenNortheastSeal();
  }
}

// Open the southwest seal when its statue reaches the target cell.
static void SolSanctumStatue_TrySouthwestSeal(void) {
  if (SolSanctumStatue_IsActorAtCell(13, 31, 12) != 0) {
    SolSanctumStatue_OpenSouthwestSeal();
  }
}

// Open the southeast seal when its statue reaches the target cell.
static void SolSanctumStatue_TrySoutheastSeal(void) {
  if (SolSanctumStatue_IsActorAtCell(15, 40, 12) != 0) {
    SolSanctumStatue_OpenSoutheastSeal();
  }
}

// Release the left trigger cell and check the beam statue position.
static void SolSanctumStatue_TryLeftBeam(void) {
  Field_SetMapCellCollision(2, PIXELS(208), PIXELS(112), 0);
  if (SolSanctumStatue_IsActorAtCell(10, 14, 7) != 0) {
    SolSanctumStatue_ShineLeftBeam();
  }
}

// Release the right trigger cell and check the beam statue position.
static void SolSanctumStatue_TryRightBeam(void) {
  Field_SetMapCellCollision(2, PIXELS(352), PIXELS(112), 0);
  if (SolSanctumStatue_IsActorAtCell(12, 21, 7) != 0) {
    SolSanctumStatue_ShineRightBeam();
  }
}

// Block the central statue cells while the leader occupies their row.
static void SolSanctumStatue_BlockCenterTriggerCells(void) {
  struct FieldActor *leaderPosition;
  s32 column;
  struct FieldActor *leader;

  leaderPosition = Object_GetById(0);
  column = leaderPosition->x.fixed >> 20;
  leader = Object_GetById(0);
  if ((leader->z.fixed >> 20) == 8) {
    if ((u32)(column - 17) <= 1) {
      Field_SetMapCellCollision(2, PIXELS(272), PIXELS(128), 255);
      Field_SetMapCellCollision(2, PIXELS(288), PIXELS(128), 255);
    }
  }
}

// Block the left beam trigger while the leader is beside its statue.
static void SolSanctumStatue_BlockLeftTriggerCell(void) {
  s32 column = Object_GetById(0)->x.fixed >> 20;
  s32 row = Object_GetById(0)->z.fixed >> 20;

  if (row == 7 && (u32)(column - 13) <= 1) {
    Field_SetMapCellCollision(2, PIXELS(208), PIXELS(112), 255);
  }
}

// Block the right beam trigger while the leader is beside its statue.
static void SolSanctumStatue_BlockRightTriggerCell(void) {
  s32 column = Object_GetById(0)->x.fixed >> 20;
  s32 row = Object_GetById(0)->z.fixed >> 20;

  if (row == 7 && (u32)(column - 21) <= 1) {
    Field_SetMapCellCollision(2, PIXELS(352), PIXELS(112), 255);
  }
}

// Move the central statue and shake the hall once both beams are lit.
static void SolSanctumStatue_UpdateCenterStatue(void) {
  struct FieldActor *statue;
  struct FieldActor *leader;
  s32 statueZ;

  statue = Object_GetById(17);
  Field_SetMapCellCollision(2, PIXELS(272), PIXELS(128), 0);
  Field_SetMapCellCollision(2, PIXELS(288), PIXELS(128), 0);
  if (statue != NULL) {
    statueZ = statue->z.fixed;
    Engine_EventBegin();
    if ((statueZ >> 20) == 8) {
      if (Engine_GameFlagIsSet(FLAG_CENTRAL_HOLE_OPEN) == 0) {
        leader = Object_GetById(0);
        if ((u32)(leader->z.fixed >> 19) <= 17) {
          Actor_WalkToAndWait(0, 289, 158);
          leader = Object_GetById(0);
          leader->facing = FACING_NORTH;
        }
      }
      if (Engine_GameFlagIsSet(FLAG_LEFT_BEAM_SHINING) != 0 &&
          Engine_GameFlagIsSet(FLAG_RIGHT_BEAM_SHINING) != 0) {
        Engine_GameFlagSet(FLAG_CENTER_STATUE_MOVED);
        Camera_SetSpeed(PIXELS(2), PIXELS(1) / 4);
        Camera_MoveTo(PIXELS(286), -1, PIXELS(146), 1);
        Engine_CameraWaitForMove();
        Object_GetById(17)->steeringFlags &= ~ACTOR_FACES_MOTION;
        Actor_SetSpeed(17, PIXELS(3), PIXELS(1));
        statue->motionFlags = 0;
        Engine_ActorSetSpritePriority(17, 3);
        Engine_AudioPlayCue(189);
        Engine_ActorSetDestination(17, 288, 178);
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
        Map_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
        Engine_EventWait(10);
        Engine_AudioPlayCue(141);
        Map_SetShake(PIXELS(3), PIXELS(3), Q16_ONE);
        Engine_EventWait(10);
        Map_SetShake(PIXELS(5), PIXELS(5), Q16_ONE);
        Engine_EventWait(35);
        Map_SetShake(PIXELS(4), PIXELS(4), Q16_ONE);
        Engine_EventWait(20);
        Map_SetShake(PIXELS(3), PIXELS(3), Q16_ONE);
        Engine_EventWait(30);
        Map_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
        Engine_EventWait(40);
        Engine_AudioPlayCue(0x121);
        Map_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
        Engine_EventWait(10);
        Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
        Engine_EventWait(60);
        Engine_AudioPlayCue(188);
        if (Engine_GameFlagIsSet(FLAG_NORTHWEST_SEAL_OPEN) != 0 &&
            Engine_GameFlagIsSet(FLAG_NORTHEAST_SEAL_OPEN) != 0 &&
            Engine_GameFlagIsSet(FLAG_SOUTHWEST_SEAL_OPEN) != 0 &&
            Engine_GameFlagIsSet(FLAG_SOUTHEAST_SEAL_OPEN) != 0) {
          Engine_GameFlagSet(FLAG_ALL_STATUE_SEALS_OPEN);
        }
        Engine_EventWait(40);
        Engine_MessageShowCentered(MsgSolSanctumSomethingClicked, 1);
        Map_CopyCellAttributes(0, 1, 2, 1, 17, 8);
        Map_CopyCellAttributes(17, 9, 2, 1, 17, 7);
      }
    }
    Engine_EventEnd();
  }
}

// Push the central statue north while backing the leader away.
static void SolSanctumStatue_PushCenterStatueBack(void) {
  struct FieldActor *movingActor;

  movingActor = Object_GetById(17);
  if (movingActor != 0) {
    if ((movingActor->z.fixed >> 20) == 8) {
      Engine_EventBegin();
      Event_PlayCue(185);
      Actor_SetSpeed(17, PIXELS(1) / 5, PIXELS(1) / 10);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1) / 5, PIXELS(1) / 10);
      Object_GetById(17)->steeringFlags &= ~ACTOR_FACES_MOTION;
      Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 8);
      movingActor = Object_GetById(0);
      Actor_SetDestination(ACTOR_PARTY_LEADER, movingActor->x.part.pixel, 136);
      Actor_SetDestination(17, 288, 120);
      Engine_ActorWaitForMove(17);
      Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
      Engine_EventEnd();
    }
  }
}

// Push the northwest statue into place, then animate its seal.
static void SolSanctumStatue_PushNorthwestStatue(void) {
  SolSanctumStatue_PushStatue(9, 31, 9, 30, 9);
  SolSanctumStatue_OpenNorthwestSeal();
}

// Push the northeast statue into place, then animate its seal.
static void SolSanctumStatue_PushNortheastStatue(void) {
  SolSanctumStatue_PushStatue(11, 40, 9, 41, 9);
  SolSanctumStatue_OpenNortheastSeal();
}

// Push the southwest statue into place, then animate its seal.
static void SolSanctumStatue_PushSouthwestStatue(void) {
  SolSanctumStatue_PushStatue(13, 31, 12, 30, 12);
  SolSanctumStatue_OpenSouthwestSeal();
}

// Push the southeast statue into place, then animate its seal.
static void SolSanctumStatue_PushSoutheastStatue(void) {
  SolSanctumStatue_PushStatue(15, 40, 12, 41, 12);
  SolSanctumStatue_OpenSoutheastSeal();
}

// Push the left statue east into the light beam position.
static void SolSanctumStatue_PushLeftBeamStatueEast(void) {
  SolSanctumStatue_PushStatue(10, 14, 7, 13, 7);
  SolSanctumStatue_ShineLeftBeam();
}

// Push the left beam statue one cell south.
static void SolSanctumStatue_PushLeftBeamStatueSouth(void) {
  struct FieldActor *statue = Object_GetById(10);
  s32 row;

  if (statue == 0) {
    return;
  }

  row = statue->z.fixed >> 20;
  SolSanctumStatue_PushStatue(10, 13, row + 1, 13, row);
}

// Push the left beam statue one cell north.
static void SolSanctumStatue_PushLeftBeamStatueNorth(void) {
  struct FieldActor *statue = Object_GetById(10);
  if (statue != NULL) {
    s32 row = statue->z.fixed >> 20;
    SolSanctumStatue_PushStatue(10, 13, row - 1, 13, row);
  }
}

// Push the right statue west into the light beam position.
static void SolSanctumStatue_PushRightBeamStatueWest(void) {
  SolSanctumStatue_PushStatue(12, 21, 7, 22, 7);
  SolSanctumStatue_ShineRightBeam();
}

// Push the right beam statue one cell south.
static void SolSanctumStatue_PushRightBeamStatueSouth(void) {
  struct FieldActor *statue = Object_GetById(12);
  if (statue != NULL) {
    s32 row = statue->z.fixed >> 20;
    SolSanctumStatue_PushStatue(12, 22, row + 1, 22, row);
  }
}

// Push the right beam statue one cell north.
static void SolSanctumStatue_PushRightBeamStatueNorth(void) {
  struct FieldActor *statue = Object_GetById(12);
  if (statue != NULL) {
    s32 row = statue->z.fixed >> 20;
    SolSanctumStatue_PushStatue(12, 22, row - 1, 22, row);
  }
}

// Move the leader and statue together to the centers of their target cells.
static void SolSanctumStatue_PushStatue(s32 statueId, s32 statueColumn, s32 statueRow,
                                 s32 leaderColumn, s32 leaderRow) {
  s32 statueX;
  s32 statueZ;

  Engine_EventBegin();
  Engine_AudioPlayCue(185);
  Actor_SetSpeed(statueId, PIXELS(1) / 5, PIXELS(1) / 10);
  Engine_ActorSetSpeed(0, PIXELS(1) / 5, PIXELS(1) / 10);
  Object_GetById(statueId)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Engine_ActorSetAnimation(0, 8);
  Engine_ActorSetDestination(0, (leaderColumn << 4) + 8, (leaderRow << 4) + 8);
  statueX = statueColumn << 4;
  statueZ = statueRow << 4;
  Engine_ActorSetDestination(statueId, statueX + 8, statueZ + 8);
  Engine_ActorWaitForMove(statueId);
  Engine_ActorSetAnimation(0, ANIM_STAND);
  Engine_EventEnd();
}

// Test an actor map column and row.
static s32 SolSanctumStatue_IsActorAtCell(s32 actorId, s32 column, s32 row) {
  struct FieldActor *actor = Object_GetById(actorId);
  if (actor == NULL || (actor->x.fixed >> 20) != column) {
    return 0;
  }
  if ((actor->z.fixed >> 20) != row) {
    return 0;
  }
  return 1;
}

// Configure the northwest seal animation and statue return position.
static void SolSanctumStatue_OpenNorthwestSeal(void) {
  struct SealScene *scene = &sSealScene;
  scene->open.srcX = 0;
  scene->open.srcY = 55;
  scene->open.destX = 32;
  scene->open.destY = 40;
  scene->open.width = 4;
  scene->open.height = 3;
  scene->pulseA.srcX = 2;
  scene->pulseA.srcY = 30;
  scene->pulseA.destX = 34;
  scene->pulseA.destY = 10;
  scene->pulseA.width = 2;
  scene->pulseA.height = 1;
  scene->pulseB.srcX = 2;
  scene->pulseB.srcY = 28;
  scene->pulseB.destX = 34;
  scene->pulseB.destY = 10;
  scene->pulseB.width = 2;
  scene->pulseB.height = 1;
  scene->opened.srcX = 2;
  scene->opened.srcY = 30;
  scene->opened.destX = 16;
  scene->opened.destY = 10;
  scene->opened.width = 2;
  scene->opened.height = 1;
  scene->flag = FLAG_NORTHWEST_SEAL_OPEN;
  scene->facing = FACING_SOUTH;
  scene->destX = 500;
  scene->destZ = 132;
  scene->stairs[0].srcX = 8;
  scene->stairs[0].srcY = 55;
  scene->stairs[0].destX = 32;
  scene->stairs[0].destY = 40;
  scene->stairs[0].width = 4;
  scene->stairs[0].height = 3;
  scene->stairs[1].srcX = 2;
  scene->stairs[1].srcY = 30;
  scene->stairs[1].destX = 34;
  scene->stairs[1].destY = 10;
  scene->stairs[1].width = 2;
  scene->stairs[1].height = 1;
  scene->stairs[2].srcX = 2;
  scene->stairs[2].srcY = 28;
  scene->stairs[2].destX = 16;
  scene->stairs[2].destY = 10;
  scene->stairs[2].width = 2;
  scene->stairs[2].height = 1;
  scene->actor = 9;
  scene->actorX = 488;
  scene->actorZ = 152;
  SolSanctumStatue_OpenSeal();
}

// Configure the northeast seal animation and statue return position.
static void SolSanctumStatue_OpenNortheastSeal(void) {
  struct SealScene *scene = &sSealScene;
  scene->open.srcX = 4;
  scene->open.srcY = 55;
  scene->open.destX = 36;
  scene->open.destY = 40;
  scene->open.width = 4;
  scene->open.height = 3;
  scene->pulseA.srcX = 4;
  scene->pulseA.srcY = 30;
  scene->pulseA.destX = 36;
  scene->pulseA.destY = 10;
  scene->pulseA.width = 2;
  scene->pulseA.height = 1;
  scene->pulseB.srcX = 4;
  scene->pulseB.srcY = 28;
  scene->pulseB.destX = 36;
  scene->pulseB.destY = 10;
  scene->pulseB.width = 2;
  scene->pulseB.height = 1;
  scene->opened.srcX = 4;
  scene->opened.srcY = 30;
  scene->opened.destX = 18;
  scene->opened.destY = 10;
  scene->opened.width = 2;
  scene->opened.height = 1;
  scene->flag = FLAG_NORTHEAST_SEAL_OPEN;
  scene->facing = FACING_SOUTH;
  scene->destX = 654;
  scene->destZ = 132;
  scene->stairs[0].srcX = 12;
  scene->stairs[0].srcY = 55;
  scene->stairs[0].destX = 36;
  scene->stairs[0].destY = 40;
  scene->stairs[0].width = 4;
  scene->stairs[0].height = 3;
  scene->stairs[1].srcX = 4;
  scene->stairs[1].srcY = 30;
  scene->stairs[1].destX = 36;
  scene->stairs[1].destY = 10;
  scene->stairs[1].width = 2;
  scene->stairs[1].height = 1;
  scene->stairs[2].srcX = 4;
  scene->stairs[2].srcY = 28;
  scene->stairs[2].destX = 18;
  scene->stairs[2].destY = 10;
  scene->stairs[2].width = 2;
  scene->stairs[2].height = 1;
  scene->actor = 11;
  scene->actorX = 664;
  scene->actorZ = 152;
  SolSanctumStatue_OpenSeal();
}

// Configure the southwest seal animation and statue return position.
static void SolSanctumStatue_OpenSouthwestSeal(void) {
  struct SealScene *scene = &sSealScene;
  scene->open.srcX = 0;
  scene->open.srcY = 58;
  scene->open.destX = 32;
  scene->open.destY = 43;
  scene->open.width = 4;
  scene->open.height = 1;
  scene->pulseA.srcX = 2;
  scene->pulseA.srcY = 31;
  scene->pulseA.destX = 34;
  scene->pulseA.destY = 11;
  scene->pulseA.width = 2;
  scene->pulseA.height = 1;
  scene->pulseB.srcX = 2;
  scene->pulseB.srcY = 29;
  scene->pulseB.destX = 34;
  scene->pulseB.destY = 11;
  scene->pulseB.width = 2;
  scene->pulseB.height = 1;
  scene->opened.srcX = 2;
  scene->opened.srcY = 31;
  scene->opened.destX = 16;
  scene->opened.destY = 11;
  scene->opened.width = 2;
  scene->opened.height = 1;
  scene->flag = FLAG_SOUTHWEST_SEAL_OPEN;
  scene->facing = FACING_NORTH;
  scene->destX = 500;
  scene->destZ = 216;
  scene->stairs[0].srcX = 8;
  scene->stairs[0].srcY = 58;
  scene->stairs[0].destX = 32;
  scene->stairs[0].destY = 43;
  scene->stairs[0].width = 4;
  scene->stairs[0].height = 1;
  scene->stairs[1].srcX = 2;
  scene->stairs[1].srcY = 31;
  scene->stairs[1].destX = 34;
  scene->stairs[1].destY = 11;
  scene->stairs[1].width = 2;
  scene->stairs[1].height = 1;
  scene->stairs[2].srcX = 2;
  scene->stairs[2].srcY = 29;
  scene->stairs[2].destX = 16;
  scene->stairs[2].destY = 11;
  scene->stairs[2].width = 2;
  scene->stairs[2].height = 1;
  scene->actor = 13;
  scene->actorX = 488;
  scene->actorZ = 200;
  SolSanctumStatue_OpenSeal();
}

// Configure the southeast seal animation and statue return position.
static void SolSanctumStatue_OpenSoutheastSeal(void) {
  struct SealScene *scene = &sSealScene;
  scene->open.srcX = 4;
  scene->open.srcY = 58;
  scene->open.destX = 36;
  scene->open.destY = 43;
  scene->open.width = 4;
  scene->open.height = 1;
  scene->pulseA.srcX = 4;
  scene->pulseA.srcY = 31;
  scene->pulseA.destX = 36;
  scene->pulseA.destY = 11;
  scene->pulseA.width = 2;
  scene->pulseA.height = 1;
  scene->pulseB.srcX = 4;
  scene->pulseB.srcY = 29;
  scene->pulseB.destX = 36;
  scene->pulseB.destY = 11;
  scene->pulseB.width = 2;
  scene->pulseB.height = 1;
  scene->opened.srcX = 4;
  scene->opened.srcY = 31;
  scene->opened.destX = 18;
  scene->opened.destY = 11;
  scene->opened.width = 2;
  scene->opened.height = 1;
  scene->flag = FLAG_SOUTHEAST_SEAL_OPEN;
  scene->facing = FACING_NORTH;
  scene->destX = 654;
  scene->destZ = 216;
  scene->stairs[0].srcX = 12;
  scene->stairs[0].srcY = 58;
  scene->stairs[0].destX = 36;
  scene->stairs[0].destY = 43;
  scene->stairs[0].width = 4;
  scene->stairs[0].height = 1;
  scene->stairs[1].srcX = 4;
  scene->stairs[1].srcY = 31;
  scene->stairs[1].destX = 36;
  scene->stairs[1].destY = 11;
  scene->stairs[1].width = 2;
  scene->stairs[1].height = 1;
  scene->stairs[2].srcX = 4;
  scene->stairs[2].srcY = 29;
  scene->stairs[2].destX = 18;
  scene->stairs[2].destY = 11;
  scene->stairs[2].width = 2;
  scene->stairs[2].height = 1;
  scene->actor = 15;
  scene->actorX = 664;
  scene->actorZ = 200;
  SolSanctumStatue_OpenSeal();
}

// Apply one configured source-to-destination tile region.
#define COPY_CELLS(copy)                                                       \
  Map_CopyCellsTo((copy).srcX, (copy).srcY, (copy).destX, (copy).destY,        \
                  (copy).width, (copy).height)

// Animate the seal and either advance the story or reset its statue.
static void SolSanctumStatue_OpenSeal(void) {
  struct SealScene *scene;
  s32 exitResult;
  s32 pulse;

  exitResult = 0;
  Engine_EventBegin();
  if (!Engine_GameFlagIsSet(FLAG_ALL_STATUE_SEALS_OPEN)) {
    // Show the configured seal and flash its two tile states.
    Engine_CameraSetSpeed(PIXELS(2), PIXELS(1) / 4);
    Engine_CameraMoveTo(PIXELS(576), -1, PIXELS(172), 1);
    Engine_CameraWaitForMove();
    Engine_AudioPlayCue(186);
    COPY_CELLS(sSealScene.open);
    for (pulse = 0; pulse != 20; pulse++) {
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
    exitResult = SolSanctumStatue_SelectStoryExit();
    // Keep the statue reusable until the central statue has moved.
    if (exitResult == -1) {
      if (!Engine_GameFlagIsSet(FLAG_CENTER_STATUE_MOVED)) {
        Engine_CameraFollowActor(0, 1);
        Object_GetById(0)->facing = scene->facing;
        Actor_SetSpeed(0, PIXELS(2), PIXELS(2));
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
    } else if (exitResult == 0 &&
               Engine_GameFlagIsSet(FLAG_CENTER_STATUE_MOVED)) {
      if (Engine_GameFlagIsSet(FLAG_NORTHWEST_SEAL_OPEN) &&
          Engine_GameFlagIsSet(FLAG_SOUTHWEST_SEAL_OPEN) &&
          Engine_GameFlagIsSet(FLAG_SOUTHEAST_SEAL_OPEN)) {
        if (!GameFlag_IsSet(FLAG_ALL_STATUE_SEALS_OPEN)) {
          Engine_GameFlagSet(FLAG_ALL_STATUE_SEALS_OPEN);
          SolSanctumStatue_RunSealOpenedSequence();
        }
      } else if (Engine_GameFlagIsSet(0x812)) {
        Engine_EventRequestExit(5);
        exitResult = 1;
      }
    }
  }
  if (exitResult == 1) {
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
  }
  Engine_EventEnd();
}

// Draw the left beam and open the floor when the right beam is also lit.
static void SolSanctumStatue_ShineLeftBeam(void) {
  Engine_EventBegin();
  if (GameFlag_IsSet(FLAG_CENTER_STATUE_MOVED) == 0) {
    if (GameFlag_IsSet(FLAG_LEFT_BEAM_SHINING) == 0) {
      Camera_SetSpeed(PIXELS(2), PIXELS(1) / 4);
      Camera_MoveTo(PIXELS(286), -1, PIXELS(146), 1);
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
        SolSanctumStatue_OpenCentralHole();
      }
    }
  }
  Engine_EventEnd();
}

// Draw the right beam and open the floor when the left beam is also lit.
static void SolSanctumStatue_ShineRightBeam(void) {
  Engine_EventBegin();
  if (GameFlag_IsSet(FLAG_CENTER_STATUE_MOVED) == 0) {
    if (GameFlag_IsSet(FLAG_RIGHT_BEAM_SHINING) == 0) {
      Camera_SetSpeed(PIXELS(2), PIXELS(1) / 4);
      Camera_MoveTo(PIXELS(286), -1, PIXELS(146), 1);
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
        SolSanctumStatue_OpenCentralHole();
      }
    }
  }
  Engine_EventEnd();
}

// Show the selected message from one speaker and wait the requested frames.
static void Event_SayThenWait(s32 speaker, s32 frames) {
  Engine_EventShowMessage(speaker, 0);
  Engine_EventWait(frames);
}

struct SceneEntrance gSolSanctumStatue_Entrances[] = {
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

u32 gSolSanctumStatue_Exits[] = {
    SCENE_EXITS(12),      SCENE_EXIT(1, 11, 2),   SCENE_EXIT(2, 13, 1),
    SCENE_EXIT(3, 11, 4), SCENE_EXIT(4, 11, 5),   SCENE_EXIT(5, 11, 6),
    SCENE_EXIT(6, 11, 7), SCENE_EXIT(10, 510, 1), SCENE_EXITS_END,
};

// clang-format off
struct ScenePlacement gSolSanctumStatue_Placements[] = {
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

struct SceneEvent gSolSanctumStatue_Events[] = {
    {0x31, 1, CONDITION_ALWAYS, 1},
    {EVENT_EXIT, 2, CONDITION_ALWAYS, 2},
    {0x202, 10, CONDITION_ALWAYS, EVENT_SCRIPT(SolSanctumStatue_CheckNorthPassage)},
    {0x202, 11, CONDITION_ALWAYS, EVENT_SCRIPT(SolSanctumStatue_CheckSouthPassage)},
    {PSYNERGY_ON_ACTOR(0x8c), 9, FLAG_NORTHWEST_SEAL_OPEN,
     EVENT_SCRIPT(SolSanctumStatue_TryNorthwestSeal)},
    {PSYNERGY_ON_ACTOR(0x8c), 11, FLAG_NORTHEAST_SEAL_OPEN,
     EVENT_SCRIPT(SolSanctumStatue_TryNortheastSeal)},
    {PSYNERGY_ON_ACTOR(0x8c), 13, FLAG_SOUTHWEST_SEAL_OPEN,
     EVENT_SCRIPT(SolSanctumStatue_TrySouthwestSeal)},
    {PSYNERGY_ON_ACTOR(0x8c), 15, FLAG_SOUTHEAST_SEAL_OPEN,
     EVENT_SCRIPT(SolSanctumStatue_TrySoutheastSeal)},
    {TOUCH_WALKING_INTO(FACING_EAST), 5, FLAG_NORTHWEST_SEAL_OPEN,
     EVENT_SCRIPT(SolSanctumStatue_PushNorthwestStatue)},
    {TOUCH_WALKING_INTO(FACING_WEST), 6, FLAG_NORTHEAST_SEAL_OPEN,
     EVENT_SCRIPT(SolSanctumStatue_PushNortheastStatue)},
    {TOUCH_WALKING_INTO(FACING_EAST), 7, FLAG_SOUTHWEST_SEAL_OPEN,
     EVENT_SCRIPT(SolSanctumStatue_PushSouthwestStatue)},
    {TOUCH_WALKING_INTO(FACING_WEST), 8, FLAG_SOUTHEAST_SEAL_OPEN,
     EVENT_SCRIPT(SolSanctumStatue_PushSoutheastStatue)},
    {0x10008c15, 10, FLAG_LEFT_BEAM_SHINING,
     EVENT_SCRIPT(SolSanctumStatue_BlockLeftTriggerCell)},
    {PSYNERGY_ON_ACTOR(0x8c), 10, FLAG_LEFT_BEAM_SHINING,
     EVENT_SCRIPT(SolSanctumStatue_TryLeftBeam)},
    {0x10008c15, 12, FLAG_RIGHT_BEAM_SHINING,
     EVENT_SCRIPT(SolSanctumStatue_BlockRightTriggerCell)},
    {PSYNERGY_ON_ACTOR(0x8c), 12, FLAG_RIGHT_BEAM_SHINING,
     EVENT_SCRIPT(SolSanctumStatue_TryRightBeam)},
    {TOUCH_WALKING_INTO(FACING_EAST), 20, FLAG_LEFT_BEAM_SHINING,
     EVENT_SCRIPT(SolSanctumStatue_PushLeftBeamStatueEast)},
    {TOUCH_WALKING_INTO(FACING_EAST), 21, FLAG_LEFT_BEAM_SHINING,
     EVENT_SCRIPT(SolSanctumStatue_PushLeftBeamStatueEast)},
    {TOUCH_WALKING_INTO(FACING_SOUTH), 21, FLAG_LEFT_BEAM_SHINING,
     EVENT_SCRIPT(SolSanctumStatue_PushLeftBeamStatueSouth)},
    {TOUCH_WALKING_INTO(FACING_NORTH), 23, FLAG_LEFT_BEAM_SHINING,
     EVENT_SCRIPT(SolSanctumStatue_PushLeftBeamStatueNorth)},
    {TOUCH_WALKING_INTO(FACING_NORTH), 24, FLAG_LEFT_BEAM_SHINING,
     EVENT_SCRIPT(SolSanctumStatue_PushLeftBeamStatueNorth)},
    {TOUCH_WALKING_INTO(FACING_WEST), 25, FLAG_RIGHT_BEAM_SHINING,
     EVENT_SCRIPT(SolSanctumStatue_PushRightBeamStatueWest)},
    {TOUCH_WALKING_INTO(FACING_WEST), 26, FLAG_RIGHT_BEAM_SHINING,
     EVENT_SCRIPT(SolSanctumStatue_PushRightBeamStatueWest)},
    {TOUCH_WALKING_INTO(FACING_SOUTH), 26, FLAG_RIGHT_BEAM_SHINING,
     EVENT_SCRIPT(SolSanctumStatue_PushRightBeamStatueSouth)},
    {TOUCH_WALKING_INTO(FACING_NORTH), 28, FLAG_RIGHT_BEAM_SHINING,
     EVENT_SCRIPT(SolSanctumStatue_PushRightBeamStatueNorth)},
    {TOUCH_WALKING_INTO(FACING_NORTH), 29, FLAG_RIGHT_BEAM_SHINING,
     EVENT_SCRIPT(SolSanctumStatue_PushRightBeamStatueNorth)},
    {0x10008c15, 17, FLAG_CENTER_STATUE_MOVED,
     EVENT_SCRIPT(SolSanctumStatue_BlockCenterTriggerCells)},
    {PSYNERGY_ON_ACTOR(0x8c), 17, FLAG_CENTER_STATUE_MOVED,
     EVENT_SCRIPT(SolSanctumStatue_UpdateCenterStatue)},
    {TOUCH_WALKING_INTO(FACING_NORTH), 32, FLAG_CENTER_STATUE_MOVED,
     EVENT_SCRIPT(SolSanctumStatue_PushCenterStatueBack)},
    {SCENE_EVENTS_END},
};

s32 gSolSanctumStatue_ScrollPhase = 0;

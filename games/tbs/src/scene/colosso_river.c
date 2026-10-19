#include "script.h"
#include "version.h"
#include "game_state.h"
#include "io_write_queue.h"
#include "scene_ids.h"
#include "vram_tab.h"
#include "unit/import.h"
#include "unit/scene/colosso_river.h"
#include "unit/scene/common/colosso_solo.h"
#include "unit/scene/common/colosso_marker.h"
#include "unit/scene/common/colosso_setup.h"
#include "unit/scene/common/colosso_occupant.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "unit/game/game.h"
#include "unit/window/window.h"
#include "unit/window/battle_command.h"
#include "unit/menu/menu.h"
#include "unit/sound/sound.h"
#include "unit/runtime/runtime.h"
#include "unit/scene/common/colosso_item_icon.h"
#include "battle.h"
#include "keystate.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "types.h"
#include "callback_scheduler.h"
#include "field_event.h"
#include "field_service.h"
#include "field_scene.h"
#include "staged_actor.h"
#include "resource_ids.h"
#include "text/msg_ids.h"
#include "battle_unit.h"
#include "resource.h"
#include "object_runtime.h"
#include "ability_ids.h"
#include "edition.h"
#include "item_ids.h"
#include "objcmd.h"
#include "field_obj.h"
#include "heap_state.h"
#include "fixed_math.h"
#include "colosso_gauge.h"
#include "macros.h"
#include "flags.h"
#include "songs.h"

// Keep native call forms while UBFIX uses the declared void contracts.
#ifdef UBFIX
#define COLOSSO_RIVER_FACE_DIRECTION Engine_ActorFaceDirection
#define COLOSSO_RIVER_PLACE_STAGED_PAIR ColossoRiver_MoveActorPairByOffset
#define COLOSSO_RIVER_SELECT_MODE_BLOCK ColossoRiver_SelectPresentationScript
#define COLOSSO_RIVER_APPLY_MATCHING_SLOT_VALUES Scene_GiveAndEquipItem
#else
#define COLOSSO_RIVER_FACE_DIRECTION ((s32 (*)())Engine_ActorFaceDirection)
#define COLOSSO_RIVER_PLACE_STAGED_PAIR                                        \
  ((s32 (*)())ColossoRiver_MoveActorPairByOffset)
#define COLOSSO_RIVER_SELECT_MODE_BLOCK                                        \
  ((s32 (*)())ColossoRiver_SelectPresentationScript)
#define COLOSSO_RIVER_APPLY_MATCHING_SLOT_VALUES                               \
  ((s32 (*)(s32, s32))Scene_GiveAndEquipItem)
#endif

/// Forward declarations.
static void ColossoRiver_ExplainFinalsRules(s32 guide);
static void ColossoRiver_ShowRoundIntroduction(s32 round);
static void ColossoRiver_ShowMatchResult(s32 result);
static void ColossoRiver_NoOp(void);
static void ColossoRiver_WaitForStageReady(void);
static s32 ColossoRiver_CollectFinishItem(s32 firstItem, s32 secondItem);
static void ColossoRiver_PushObjectAhead(void);

typedef void (*SceneTask)(void);

/// The course's actors, by the guide's and attendants' explanations: four
/// stages (the Rock Challenge, the Pipeworks, the Broken Bridge and the Lumber
/// Water Crossing Area) lead to the arena where the two warriors fight.
enum {
  OPPONENT = 8, ///< the warrior racing Isaac to the arena
  ROCK_A = 9,   ///< the Rock Challenge's three rocks
  ROCK_B = 10,
  ROCK_C = 11,
  PIPEWORKS_BLOCK = 12, ///< pushed west to let water into the Pipeworks
  PIPE_WEST = 13,       ///< the Pipeworks' two pipes, a kind of scale
  PIPE_EAST = 14,
  GROWTH_PLANT = 15,
  BRIDGE_OPERATOR = 16, ///< works the Broken Bridge
  LOG_NORTH_SOUTH = 17, ///< the Lumber Water Crossing Area's two logs
  LOG_EAST_WEST = 18,
  GUIDE = 19, ///< explains the finals
  ROCK_ATTENDANT = 20,
  PIPEWORKS_ATTENDANT = 21,
  BRIDGE_ATTENDANT = 22,
  LUMBER_ATTENDANT = 23,
  PRIZE_A = 24, ///< the arena's Iron Shield and Padded Gloves
  PRIZE_B = 25
};

// Shared Colosso routines (scene/common/colosso_*.c).

/// Return the course entrance table.
struct SceneEntrance *ColossoRiver_GetEntrances(void) {
  return gColossoRiver_Entrances;
}

/// The course has no region table.
const struct SceneRegion *ColossoRiver_GetRegions(void) { return 0; }

/// Return the course exit table.
u32 *ColossoRiver_GetExits(void) { return gColossoRiver_Exits; }

/// Return the course actor placements.
struct ScenePlacement *ColossoRiver_GetPlacements(void) {
  return gColossoRiver_Placements;
}

/// Advance the bridge animation and its walkable collision cells.
static void ColossoRiver_UpdateBrokenBridge(void) {
  switch ((u32)gColossoRiver_Countdown) {
  case 66:
    Engine_MapCopyCellsLayered(92, 31, 2, 2, 50, 38);
    Engine_MapCopyCellsLayered(92, 31, 2, 2, 54, 38);
    Engine_ActorSetAnimation(BRIDGE_OPERATOR, 10);
    break;
  case 60:
    Engine_MapCopyCellsLayered(92, 33, 2, 2, 50, 38);
    Engine_MapCopyCellsLayered(92, 33, 2, 2, 54, 38);
    Map_CopyCellAttributes(50, 25, 6, 1, 50, 12);
    Engine_ActorSetAnimation(BRIDGE_OPERATOR, 11);
    break;
  case 6:
    Engine_MapCopyCellsLayered(92, 31, 2, 2, 50, 38);
    Engine_MapCopyCellsLayered(92, 31, 2, 2, 54, 38);
    Engine_ActorSetAnimation(BRIDGE_OPERATOR, 10);
    break;
  case 0:
    Engine_MapCopyCellsLayered(92, 29, 2, 2, 50, 38);
    Engine_MapCopyCellsLayered(92, 29, 2, 2, 54, 38);
    Engine_ActorSetAnimation(BRIDGE_OPERATOR, 12);
    Map_CopyCellAttributes(50, 24, 6, 1, 50, 12);
    gColossoRiver_Countdown = 120;
    break;
  }
  gColossoRiver_Countdown--;
}

/// Stop the bridge task and restore its closed frame.
static void ColossoRiver_ResetBrokenBridge(void) {
  SceneTask task;

  gColossoRiver_Countdown = 0;
  task = ColossoRiver_UpdateBrokenBridge;
  Engine_TaskRemoveCallback(task);
  task();
}

/// Start the bridge's repeating opening sequence.
static void ColossoRiver_StartBrokenBridge(void) {
  gColossoRiver_Countdown = 66;
  FieldScheduler_AddCallback(ColossoRiver_UpdateBrokenBridge, 0xC80);
}

/// Wait for the crossing window, bounded by one bridge cycle.
static void ColossoRiver_WaitForBridgeOpening(void) {
  s32 i;

  Engine_TaskWait(10);
  i = 0;
  if (gColossoRiver_Countdown != 22) {
    do {
      Engine_TaskWait(1);
      i++;
      if (i > 119) {
        break;
      }
    } while (gColossoRiver_Countdown != 22);
  }
}

/// After a rock is moved: a rock resting on the floor becomes a step (the
/// second rock's column is kept in byte flag 0x310).
static void Event_SettleRocks(void) {

  struct FieldActor *actor;
  s32 blocked;

  Map_CopyCellAttributes(27, 13, 3, 1, 23, 12);
  actor = Object_GetById(ROCK_A);
  blocked = Map_GetTerrainHeight(0, actor->x.fixed, actor->z.fixed);
  if (actor->y.fixed == 0 && blocked == 0) {
    actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    actor->motionFlags = 0;
    {
      s32 x = actor->x.fixed >> 20;
      s32 y = actor->z.fixed >> 20;

      Map_CopyCellAttributes(14, 13, 1, 1, x, y);
    }
  }
  actor = Object_GetById(ROCK_B);
  {
    s32 x = actor->x.fixed >> 20;

    FieldGameFlag_SetByte(784, x);
  }
  {
    s32 x = actor->x.fixed >> 20;
    s32 y = actor->z.fixed >> 20;

    Map_CopyCellAttributes(14, 13, 1, 1, x, y);
  }
}

/// Walking into a rock pushes it one cell.
static void Event_PushRock(void) {

  ColossoRiver_PushObjectAhead();
  Event_SettleRocks();
}

/// Growth on the plant: it grows and its cell becomes walkable (0x303 keeps it
/// grown).
static void Event_GrowPlant(void) {
  {

    Map_CopyCellAttributes(47, 24, 1, 1, 47, 12);
  }
  Engine_GameFlagSet(0x303);
}

/// Douse on the west pipe: the camera turns to the Pipeworks and the two pipes
/// rise as it fills (0x301).
static void Event_DousePipe(void) {
  struct FieldActor *actor;
  s32 none;

  GameFlag_Set(0x301);
  actor = Object_GetById(PIPE_WEST);
  Engine_EventBegin();
  Camera_SetSpeed(PIXELS(2), PIXELS(1) / 4);
  Camera_MoveTo(PIXELS(600), -1, PIXELS(200), 1);
  Object_SetMode(actor, 3);
  Engine_CameraWaitForMove();
  none = 0;
  actor->motionFlags = none;
  actor->acceleration = PIXELS(2) / 5;
  actor->speed = PIXELS(4) / 5;
  FieldObject_SetPosition(actor, actor->x.fixed, PIXELS(8), actor->z.fixed);
  actor = Actor_Get(PIPE_EAST);
  actor->motionFlags = none;
  actor->acceleration = PIXELS(2) / 5;
  actor->speed = PIXELS(4) / 5;
  Engine_ObjectSetPosition(actor, actor->x.fixed, PIXELS(32), actor->z.fixed);
  FieldObject_WaitForMove(actor);
  Engine_EventWait(45);
  Map_CopyCellAttributes(43, 12, 1, 1, 41, 12);
  Engine_EventEnd();
}

/// Move the leader and a course object together between cell centers.
static void ColossoRiver_MoveActorPairByOffset(s32 actorId, s32 dx, s32 dz) {
  struct FieldActor *leader;
  struct FieldActor *actor;
  s32 x;
  s32 z;

  leader = Object_GetById(gGameState.selectedActor);
  actor = Object_GetById(actorId);
  Engine_EventBegin();
  {
    x = ((leader->x.fixed + (dx << 16)) & 0xFFF00000) + PIXELS(8);
    z = ((leader->z.fixed + (dz << 16)) & 0xFFF00000) + PIXELS(8);

    leader->speed = PIXELS(1);
    leader->acceleration = PIXELS(1) / 2;
    Engine_ObjectSetPosition(leader, x, leader->y.fixed, z);
  }
  Object_SetMode(leader, 27);
  {
    x = ((actor->x.fixed + (dx << 16)) & 0xFFF00000) + PIXELS(8);
    z = ((actor->z.fixed + (dz << 16)) & 0xFFF00000) + PIXELS(8);

    actor->speed = PIXELS(1);
    actor->acceleration = PIXELS(1) / 2;
    Engine_ObjectSetPosition(actor, x, actor->y.fixed, z);
  }
  if (dx < 0 || dz < 0) {
    Object_SetMode(actor, 4);
  } else {
    Object_SetMode(actor, 3);
  }
  Engine_AudioPlayCue(SE_226);
  FieldObject_WaitForMove(leader);
  Object_SetMode(actor, 2);
  Engine_AudioPlayCue(SE_288);
  Engine_EventEnd();
}

/// Walking into the north-south log rolls it, with the leader, to the other
/// bank.
static void Event_RollLogNorthSouth(void) {
  struct FieldActor *actor;
  s32 row;
  s32 offset;

  actor = Object_GetById(gGameState.selectedActor);
  row = actor->z.fixed >> 20;
  offset = -48;
  if (row <= 8) {
    offset = 48;
  }
  Map_CopyCellAttributes(67, 8, 3, 1, 64, row);
  ColossoRiver_MoveActorPairByOffset(LOG_NORTH_SOUTH, 0, offset);
  actor = Object_GetById(LOG_NORTH_SOUTH);
  row = actor->z.fixed >> 20;
  Map_CopyCellAttributes(64, 24, 3, 1, 64, row);
}

/// Walking into the east-west log rolls it, with the leader, the way the pad
/// is held.
static void Event_RollLogEastWest(void) {
  struct FieldActor *actor;
  s32 column;
  s32 offset;
  s32 direction;

  actor = Object_GetById(gGameState.selectedActor);
  column = actor->x.fixed >> 20;
  if ((gKeysHeld & KEY_LEFT) != 0) {
    direction = -1;
  }
  if ((gKeysHeld & KEY_RIGHT) != 0) {
    direction = 1;
  }
  actor = Object_GetById(LOG_NORTH_SOUTH);
  offset = actor->z.fixed >> 20;
  if (column == 63) {
    if (offset == 11) {
      return;
    }
    offset = 160;
  } else if (column == 67) {
    if (offset == 11 && direction == -1) {
      return;
    }
    offset = 96;
  } else {
    if (offset == 11) {
      offset = 96;
    } else {
      offset = 160;
    }
    offset = -offset;
  }
  Map_CopyCellAttributes(72, 9, 1, 3, column, 9);
  ColossoRiver_MoveActorPairByOffset(LOG_EAST_WEST, offset, 0);
  actor = Object_GetById(LOG_EAST_WEST);
  column = actor->x.fixed >> 20;
  Map_CopyCellAttributes(63, 25, 1, 3, column, 9);
}

/// Run the five frames that show water entering the Pipeworks channel.
static inline void ColossoRiver_AnimatePipeworksWater(void) {
  Map_CopyCells(96, 29, 1, 3, 34, 38);
  Engine_EventWait(3);
  Map_CopyCells(97, 29, 1, 3, 34, 38);
  Engine_EventWait(3);
  Map_CopyCells(98, 29, 1, 3, 34, 38);
  Engine_EventWait(3);
  Map_CopyCells(99, 29, 1, 3, 34, 38);
  Engine_EventWait(3);
  Map_CopyCells(100, 29, 1, 3, 34, 38);
}

/// Pushing the Pipeworks block west: water runs into the channel, and the first
/// time the pipes rise (0x302, 0x301).
static void Event_PushPipeworksBlock(void) {
  s32 leaderId;
  struct FieldActor *leader;
  struct FieldActor *actor;
  s32 none;

  leaderId = gGameState.selectedActor;
  leader = Object_GetById(leaderId);
  actor = Object_GetById(PIPEWORKS_BLOCK);
  GameFlag_Set(0x302);
  Engine_EventBegin();
  Engine_ActorSetAnimation(leaderId, 8);
  Engine_EventWait(6);
  actor->speed = PIXELS(1) / 2;
  actor->acceleration = PIXELS(1) / 5;
  Event_PlayCue(SE_239);
  Object_SetMode(actor, 2);
  Engine_ObjectSetPosition(actor, actor->x.fixed - PIXELS(48), 0,
                           actor->z.fixed);
  Engine_EventWait(6);
  Engine_ActorSetAnimation(leaderId, 2);
  ObjectDispatch_StartCamera(
      *(struct DispatchObject **)(Runtime_AllocateBlock(27, 0xccc) + 480),
      (s32)actor);
  Actor_SetSpeed(leaderId, PIXELS(3) / 10, PIXELS(1) / 5);
  Engine_ObjectSetPosition(leader, leader->x.fixed - PIXELS(24), 0,
                           leader->z.fixed);
  Engine_ActorWaitForMove(leaderId);
  Engine_ActorSetAnimation(leaderId, 1);
  FieldObject_WaitForMove(actor);
  Object_SetMode(actor, 1);
  Event_PlayCue(SE_288);
  Event_PlayCue(SE_213);
  Engine_EventWait(15);
  Engine_EventEnd();
  Map_CopyCellAttributes(37, 7, 1, 4, 34, 7);
  Map_CopyCellAttributes(36, 7, 1, 4, 37, 7);
  none = GameFlag_IsSet(0x301);
  if (none != 0) {
    Engine_EventBegin();
    Camera_SetSpeed(PIXELS(2), PIXELS(1) / 4);
    Camera_MoveTo(PIXELS(552), -1, PIXELS(200), 1);
    Engine_CameraWaitForMove();
    ColossoRiver_AnimatePipeworksWater();
    Engine_EventWait(15);
    Engine_EventEnd();
    return;
  }
  GameFlag_Set(0x301);
  Engine_EventBegin();
  Camera_SetSpeed(PIXELS(2), PIXELS(1) / 4);
  Camera_MoveTo(PIXELS(600), -1, PIXELS(200), 1);
  Engine_CameraWaitForMove();
  actor = Object_GetById(PIPE_WEST);
  actor->motionFlags = none;
  actor->acceleration = PIXELS(2) / 5;
  actor->speed = PIXELS(4) / 5;
  Engine_ObjectSetPosition(actor, actor->x.fixed, PIXELS(8), actor->z.fixed);
  Object_SetMode(actor, 3);
  ColossoRiver_AnimatePipeworksWater();
  actor = Object_GetById(PIPE_EAST);
  actor->motionFlags = none;
  actor->acceleration = PIXELS(2) / 5;
  actor->speed = PIXELS(4) / 5;
  Engine_ObjectSetPosition(actor, actor->x.fixed, PIXELS(32), actor->z.fixed);
  FieldObject_WaitForMove(actor);
  Engine_EventWait(15);
  Engine_EventEnd();
  Map_CopyCellAttributes(43, 12, 1, 1, 41, 12);
}

/// Mark the course flag used by this unused event.
UNUSED static void ColossoRiver_SetCourseFlag(void) { Engine_GameFlagSet(768); }

/// Reaching the arena: the leader takes a prize (the Iron Shield if first, the
/// Padded Gloves if second), and he and his opponent face off and fight.
/// FLAG_SCENE_TEMP_11 says the arena was already reached; the arena's touch
/// works while FLAG_SCENE_TEMP_10 is clear.
static void Event_ReachArena(void) {
  s32 approach;
  s32 n;
  s32 base;

  base = 0;
  ColossoRiver_NoOp();
  Engine_EventBegin();
  approach = ColossoRiver_CollectFinishItem(120, 127);
  ColossoRiver_WaitForStageReady();
  n = 9;
  do {
    Object_RefreshSelectorById(OPPONENT);
    n = n - 1;
  } while (n >= 0);
  Actor_SetSpeed(OPPONENT, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkTo(OPPONENT, 1320, 192);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 1288, 192);
  Engine_ActorSetAnimation(OPPONENT, 1);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, OPPONENT, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimation(OPPONENT, 3);
  Engine_ActorSetAnimationAndWait(0, 3);
  Engine_EventWait(20);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(2), PIXELS(1));
  Actor_SetSpeed(OPPONENT, PIXELS(2), PIXELS(1));
  Actor_WalkTo(ACTOR_PARTY_LEADER, 1296, 192);
  Actor_WalkToAndWait(OPPONENT, 1312, 192);
  Engine_ActorSetAnimation(0, 16);
  Engine_ActorSetAnimation(OPPONENT, 9);
  Engine_EventWait(10);
  /* FAKEMATCH: base is 0 from the top of the function, so 0 - approach + 1 is
   * built as movs #0, subs, adds #1, as the native does. 1 - approach or
   * -approach + 1 folds into movs #1, subs (differ 7 in tbs-en). */
  Field_RequestBattleFromEncounterTable(72, base - approach + 1);
  Battle_SetStart(3);
  Event_SetNextDestination((s32)&SceneId_ColossoRiver, 4);
  Event_SetDefeatDestination((s32)&SceneId_ColossoRiver, 5);
  GameFlag_Set(FLAG_KEEP_MUSIC);
}

/// Walking into something: a rock is pushed, anything else is checked.
static void Event_PushRockOrCheckAhead(void) {

  if (Colosso_FindActorAhead() == 0) {
    Engine_LeaderCheckAhead();
  } else {
    Event_PushRock();
  }
}

/// Return the course event table.
struct SceneEvent *ColossoRiver_GetEvents(void) { return gColossoRiver_Events; }

/// Back from the battle: the two warriors stand in the arena as the result is
/// shown.
static void ColossoRiver_RestoreMatchResult(s32 result) {

  Engine_ActorDestroy(PRIZE_A);
  Engine_ActorDestroy(PRIZE_B);
  Party_RestoreHpPp(1);
  Engine_EventBegin();
  Actor_SetPosition(OPPONENT, PIXELS(1320), PIXELS(192));
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(1288), PIXELS(192));
  Actor_FaceActor(OPPONENT, 0x4000, 0);
  Actor_FaceActor(ACTOR_PARTY_LEADER, 0x4000, 0);
  if (result < 0) {
    Engine_ActorSetAnimation(OPPONENT, 10);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 35);
  } else {
    Engine_ActorSetAnimation(OPPONENT, 8);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 28);
  }
  Engine_TaskWait(1);
  Camera_MoveTo(PIXELS(1304), 0, PIXELS(128), 0);
  ColossoRiver_ShowMatchResult(result);
  Engine_EventEnd();
}

/// Mark actors standing in the course's finish terrain.
static void ColossoRiver_MarkCourseTerrain(s32 actor) {
  struct FieldActor *courseActor;
  s32 column;
  s32 row;

  courseActor = Object_GetById(actor);
  if (courseActor != 0) {
    column = courseActor->x.fixed >> 19;
    row = courseActor->z.fixed >> 19;
    if (column >= 94 && column <= 95 && row > 23 && row <= 26) {
      courseActor->terrainId = 1;
    }
  }
}

/// Restores the stages as they were left, then by the arrival: the race (1 and
/// 2), the guide's explanation (3), or the match won (4) or lost (5).
s32 ColossoRiver_Initialize(void) {
  struct FieldActor *actor;
  s32 hit;
  s32 x;
  u8 zero;
  s32 col;

  gEventWork->startTransition = 0;
  GameFlag_Set(FLAG_RETREAT_BLOCKED);
  actor = Object_GetById(ROCK_A);
  hit = Map_GetTerrainHeight(0, actor->x.fixed, actor->z.fixed);
  if (actor->y.fixed == 0 && hit == 0) {
    actor->priorityFlags = 2;
    actor->motionFlags = hit;
    Map_CopyCellAttributes(14, 13, 1, 1, actor->x.fixed >> 20,
                           actor->z.fixed >> 20);
  }
  x = FieldGameFlag_GetByte(0x310);
  if (x == 0) {
    x = 25;
  }
  actor = Object_GetById(ROCK_B);
  actor->x.fixed = (x << 20) + PIXELS(8);
  actor->motionFlags = 0;
  actor->priorityFlags = 2;
  Map_CopyCellAttributes(14, 13, 1, 1, x, 12);
  FieldScheduler_AddCallback(ColossoRiver_UpdateBrokenBridge, 0xc80);
  actor = Object_GetById(GROWTH_PLANT);
  actor->terrainId = 1;
  zero = 0;
  if (Engine_GameFlagIsSet(0x303)) {
    Object_SetMode(actor, 4);
    Engine_ActorSetSpriteFlags(actor, 0);
    actor->collisionFlags = zero;
    actor->priorityFlags = 3;
    Map_CopyCellAttributes(47, 24, 1, 1, 47, 12);
  }
  actor = Object_GetById(LOG_NORTH_SOUTH);
  col = actor->z.fixed >> 20;
  actor->motionFlags = zero;
  actor->priorityFlags = 2;
  Map_CopyCellAttributes(64, 24, 3, 1, 64, col);
  actor = Object_GetById(LOG_EAST_WEST);
  col = actor->x.fixed >> 20;
  actor->motionFlags = zero;
  actor->priorityFlags = 2;
  Map_CopyCellAttributes(63, 25, 1, 3, col, 9);
  if (Engine_GameFlagIsSet(0x302)) {
    Map_CopyCellAttributes(37, 7, 1, 4, 34, 7);
    Map_CopyCellAttributes(36, 7, 1, 4, 37, 7);
    Map_CopyCells(100, 29, 1, 3, 34, 38);
  }
  actor = Object_GetById(PIPE_WEST);
  if (Engine_GameFlagIsSet(0x301)) {
    Map_CopyCellAttributes(43, 12, 1, 1, 41, 12);
    actor->motionFlags = zero;
    actor->acceleration = PIXELS(2) / 5;
    actor->speed = PIXELS(4) / 5;
    actor->y.fixed = PIXELS(8);
    Object_SetMode(actor, 3);
  } else {
    Object_SetMode(actor, 2);
  }
  Object_GetById(PIPE_EAST)->priorityFlags = 2;
  Colosso_ShowItemIcon(PRIZE_A, 120);
  Colosso_ShowItemIcon(PRIZE_B, 127);
  // Select competition, support, explanation, or post-battle arrival.
  switch (gGameState.entrance) {
  case 1:
    Colosso_StartRaceGauge(0, 8, 4, PIXELS(1304), PIXELS(192), 24, 25);
    Map_CopyCellsLayered(127, 0, 1, 2, 19, 2);
    Engine_ActorDestroy(GUIDE);
    Engine_ActorDestroy(ROCK_ATTENDANT);
    Engine_ActorDestroy(PIPEWORKS_ATTENDANT);
    Engine_ActorDestroy(BRIDGE_ATTENDANT);
    Engine_ActorDestroy(LUMBER_ATTENDANT);
    if (!Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION)) {
      Engine_AudioPlayCue(MUS_017);
      Colosso_ControlCharacterAlone(0);
      Colosso_PlaceFriendsAtCheeringSpots();
      ColossoRiver_MarkCourseTerrain(1);
      ColossoRiver_MarkCourseTerrain(2);
      ColossoRiver_MarkCourseTerrain(3);
      ColossoRiver_ShowRoundIntroduction(1);
    }
    Object_LinkObjectAndSetCallback(1, 0);
    Object_LinkObjectAndSetCallback(2, 0);
    Object_LinkObjectAndSetCallback(3, 0);
    Colosso_StartRival((s32)&ResourceId_RivalPathA);
    break;
  case 2:
    FieldScheduler_AddCallback(Colosso_RaiseTakePlaceOnPsynergy, 0xc80);
    Engine_ActorDestroy(PRIZE_A);
    Engine_ActorDestroy(PRIZE_B);
    if (!Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION)) {
      Colosso_PlaceFriendsAtCheeringSpots();
      Colosso_ControlCharacterAlone(1);
      ColossoRiver_ShowRoundIntroduction(0);
    }
    break;
  case 3:
    if (!Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION)) {
      ColossoRiver_ExplainFinalsRules(GUIDE);
      Colosso_ClearCheeringSpots();
    }
    break;
  case 4:
    ColossoRiver_RestoreMatchResult(1);
    Engine_EventRequestExit(4);
    break;
  case 5:
    ColossoRiver_RestoreMatchResult(-1);
    Engine_EventRequestExit(5);
    break;
  }
  return 0;
}

/// The guide shows the party the first finals course: the race through the
/// stages to the arena, the prizes there, and where the companions cheer from.
static void ColossoRiver_ExplainFinalsRules(s32 guide) {
  struct FieldActor *actor;
  s32 x;
  s32 z;

  actor = Object_GetById(guide);
  x = actor->x.part.pixel;
  z = actor->z.part.pixel;
  Engine_EventBegin();
  Actor_SetSpeed(guide, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(IVAN, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(MIA, PIXELS(1), PIXELS(1) / 2);
  Engine_ActorSetPosition(ACTOR_PARTY_LEADER, x << 16, (z << 16) - PIXELS(48));
  Actor_SetPosition(GARET, (x << 16) - PIXELS(16), (z << 16) - PIXELS(40));
  Actor_SetPosition(IVAN, (x << 16) + PIXELS(16), (z << 16) - PIXELS(40));
  Actor_SetPosition(MIA, x << 16, (z << 16) - PIXELS(32));
  Actor_SetPosition(guide, x << 16, (z << 16) - PIXELS(80));
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  actor->facing = FACING_NORTH;
  Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventSetMessage(MsgColossoSiteFirstFinalsBattle);
  Event_ShowMessage(guide, 0);
  Actor_ShowEmote(MIA, 0x101, 60);
  Event_ShowMessage(MIA, 0);
  Engine_ActorStartRepeatedMotion(guide, 3);
  Event_ShowMessage(guide, 0);
  Actor_ShowEmote(IVAN, 0x101, 60);
  Event_ShowMessage(IVAN, 0);
  Actor_FaceActor(guide, IVAN, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(guide, 3);
  Event_ShowMessage(guide, 0);
  Actor_ShowEmote(GARET, 0x101, 60);
  Event_ShowMessage(GARET, 0);
  Actor_ShowEmote(MIA, 0x101, 60);
  Event_ShowMessage(MIA, 0);
  Actor_ShowEmote(guide, 0x102, 60);
  // Pan over each stage while the guide explains the finals rules.
  if (Event_AskYesNo(guide, 0) == 0) {
    do {
      Engine_EventSetMessage(MsgColossoWarriorsEnterFinalsWithoutAny);
      Engine_ActorSetAnimation(IVAN, 3);
      Engine_EventWait(2);
      Engine_ActorSetAnimation(GARET, 3);
      Engine_EventWait(2);
      Engine_ActorSetAnimation(MIA, 3);
      Engine_EventWait(1);
      Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
      Engine_ActorSetAnimationAndWait(guide, 3);
      Event_ShowMessage(guide, 0);
      Actor_FaceDirection(guide, FACING_NORTHWEST, 0);
      Engine_EventWait(20);
      Event_ShowMessage(guide, 0);
      Camera_SetSpeed(PIXELS(3), PIXELS(3) / 8);
      Camera_MoveTo(PIXELS(312), -1, PIXELS(104), 1);
      Engine_CameraWaitForMove();
      Event_ShowMessage(guide, 0);
      Camera_SetSpeed(PIXELS(3) / 2, PIXELS(3) / 16);
      Camera_MoveTo(PIXELS(776), -1, PIXELS(104), 1);
      Event_ShowMessage(guide, 0);
      Engine_CameraWaitForMove();
      Event_ShowMessage(guide, 0);
      Camera_SetSpeed(PIXELS(3), PIXELS(3) / 8);
      Camera_MoveTo(PIXELS(1240), -1, PIXELS(168), 1);
      Engine_CameraWaitForMove();
      Actor_FaceActor(guide, 0x6000, 0);
      Event_ShowMessage(guide, 0);
      Camera_MoveTo(PIXELS(1304), -1, PIXELS(168), 1);
      Engine_CameraWaitForMove();
      Actor_FaceActor(guide, ACTOR_PARTY_LEADER, 0);
      Event_ShowMessage(guide, 0);
      Event_ShowMessage(guide, 0);
      Event_ShowMessage(guide, 0);
      Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
      Engine_ActorRunRepeatedMotion(guide, 2);
    } while (Event_AskYesNo(guide, 0) != 0);
    Engine_ActorRunRepeatedMotion(guide, 2);
    Engine_EventSetMessage(MsgColossoAskAttendantsForExplanationsStages);
    Event_ShowMessage(guide, 0);
  }
  // Explain the party's support role, then gather them behind Isaac.
  Engine_EventSetMessage(MsgColossoIsaacYoureContestantInFinals);
  Engine_ActorRunRepeatedMotion(guide, 2);
  Event_ShowMessage(guide, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(GARET, FACING_EAST, 0);
  Actor_FaceDirection(IVAN, FACING_WEST, 0);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Event_ShowMessage(GARET, 0);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Event_ShowMessage(IVAN, 0);
  Engine_ActorSetAnimationAndWait(MIA, 3);
  Event_ShowMessage(MIA, 0);
  Engine_ActorSetAnimation(MIA, 3);
  Engine_EventWait(1);
  Engine_ActorSetAnimation(GARET, 3);
  Engine_EventWait(2);
  Engine_ActorSetAnimation(IVAN, 3);
  Engine_EventWait(1);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
  Engine_EventWait(6);
  Engine_ActorSetAnimation(GARET, 2);
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Actor_SetDestination(GARET, actor->x.part.pixel, actor->z.part.pixel);
  }
  Engine_ActorSetAnimation(IVAN, 2);
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Actor_SetDestination(IVAN, actor->x.part.pixel, actor->z.part.pixel);
  }
  Engine_ActorSetAnimation(MIA, 2);
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Actor_SetDestination(MIA, actor->x.part.pixel, actor->z.part.pixel);
  }
  Actor_WalkToAndWait(guide, x - 16, z - 64);
  Actor_SetPosition(GARET, 0, 0);
  Actor_SetPosition(IVAN, 0, 0);
  Actor_SetPosition(MIA, 0, 0);
  Actor_WalkToAndWait(guide, x - 16, z - 16);
  Actor_WalkToAndWait(guide, x, z);
  Actor_FaceDirection(guide, FACING_NORTH, 10);
  Engine_EventEnd();
}

/// The Rock Challenge's attendant: the first time, a warrior shows how to push
/// the rocks into steps; then the objective.
static void Event_TalkToRockAttendant(s32 scene) {
  s32 path;

  if (gGameState.entrance == 2) {
    Colosso_AskDoneCheering(scene);
  } else {
    Engine_EventBegin();
    path = Colosso_GreetAtStage(scene, 1);
    if (path == 0) {
      Engine_EventSetMessage(MsgColossoCallRockChallenge);
      Camera_SetSpeed(PIXELS(3), PIXELS(3) / 8);
      Camera_MoveTo(PIXELS(328), -1, PIXELS(168), 1);
      Engine_CameraWaitForMove();
      Engine_EventShowMessage(scene, 0);
      Colosso_FadeInCompetitor(0, 280, 200);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 360, 200);
      Engine_EventWait(30);
      Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 60);
      Engine_EventShowMessage(scene, 0);
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 312, 200);
      Engine_EventWait(30);
      Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 10);
      Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x106, 60);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 2, PIXELS(3) / 4);
      Colosso_WalkActorToAndStand(0, 296, 184);
      Colosso_WalkActorToAndStand(0, 296, 152);
      Colosso_WalkActorToAndStand(0, 312, 152);
      Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 15);
      ColossoRiver_PushObjectAhead();
      OverlayObject_ResetMotionFields(0);
      ColossoRiver_PushObjectAhead();
      OverlayObject_ResetMotionFields(0);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 2, PIXELS(3) / 4);
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 304, 184);
      Engine_ActorWalkToAndWait(ACTOR_PARTY_LEADER, 296, 192);
      Engine_ActorWalkToAndWait(ACTOR_PARTY_LEADER, 296, 200);
      Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 15);
      ColossoRiver_PushObjectAhead();
      OverlayObject_ResetMotionFields(0);
      ColossoRiver_PushObjectAhead();
      OverlayObject_ResetMotionFields(0);
      Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
      Engine_EventShowMessage(scene, 0);
      Colosso_RestoreCompetitor(0);
      Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
      Actor_SetPosition(ROCK_A, PIXELS(312), PIXELS(168));
      Colosso_RepeatStageNumber(scene, 1);
    } else if (path == 1) {
      Engine_EventSetMessage(MsgColossoObjectiveStageClear);
      Engine_EventShowMessage(scene, 0);
    }
    Colosso_OfferCheeringSpot(path, scene, 1);
    Engine_EventEnd();
  }
}

/// Move a Pipeworks actor with the speed used throughout its demonstration.
static inline void ColossoRiver_SetPipeHeight(struct FieldActor *actor,
                                              s32 height) {
  actor->motionFlags = 0;
  actor->acceleration = PIXELS(2) / 5;
  actor->speed = PIXELS(4) / 5;
  Engine_ObjectSetPosition(actor, actor->x.fixed, height, actor->z.fixed);
}

/// The Pipeworks' attendant: a marker shows how the pipes rise and fall like
/// scales; then the objective.
static void Event_TalkToPipeworksAttendant(s32 attendant) {
  s32 path;
  struct FieldActor *second;
  struct FieldActor *first;

  if (gGameState.entrance == 2) {
    Colosso_AskDoneCheering(attendant);
  } else {
    Engine_EventBegin();
    path = Colosso_GreetAtStage(attendant, 2);
    if (path == 0) {
      Engine_EventSetMessage(MsgColossoAreaCalledPipeworks);
      Engine_CameraSetSpeed(PIXELS(3), PIXELS(3) / 8);
      Camera_MoveTo(PIXELS(592), -1, PIXELS(120), 1);
      Engine_CameraWaitForMove();
      Engine_EventWait(60);
      Engine_CameraSetSpeed(PIXELS(3) / 2, PIXELS(3) / 16);
      Engine_CameraMoveTo(PIXELS(608), -1, PIXELS(216), 1);
      Engine_CameraWaitForMove();
      Engine_EventShowMessage(attendant, 0);
      Colosso_ShowPointer(56, 64, 0);
      Engine_EventWait(60);
      Colosso_MovePointer(160, 96, 10);
      Engine_EventWait(70);
      Engine_EventShowMessage(attendant, 0);
      Colosso_HidePointer();
      Engine_TaskWait(2);
      first = Object_GetById(PIPE_WEST);
      ColossoRiver_SetPipeHeight(first, PIXELS(8));
      second = Object_GetById(PIPE_EAST);
      ColossoRiver_SetPipeHeight(second, PIXELS(32));
      FieldObject_WaitForMove(second);
      Engine_EventWait(45);
      first = Object_GetById(PIPE_WEST);
      ColossoRiver_SetPipeHeight(first, PIXELS(24));
      second = Actor_Get(PIPE_EAST);
      ColossoRiver_SetPipeHeight(second, 0);
      FieldObject_WaitForMove(second);
      Engine_EventWait(15);
      Engine_EventShowMessage(attendant, 0);
      Colosso_ShowPointer(56, 64, 0);
      Engine_EventWait(30);
      Colosso_MovePointer(160, 96, 10);
      Engine_EventWait(40);
      Colosso_MovePointer(56, 64, 10);
      Engine_EventWait(70);
      Engine_EventShowMessage(attendant, 0);
      Colosso_HidePointer();
      Engine_TaskWait(2);
      Engine_CameraFollowActor(0, 0);
      Colosso_RepeatStageNumber(attendant, 2);
    } else if (path == 1) {
      Engine_EventSetMessage(MsgColossoObjectiveMakeGood);
      Engine_EventShowMessage(attendant, 0);
    }
    Colosso_OfferCheeringSpot(path, attendant, 2);
    Engine_EventEnd();
  }
}

/// The Broken Bridge's attendant: a warrior times his crossing; then the
/// objective.
static void Event_TalkToBridgeAttendant(s32 scene) {
  s32 path;

  if (gGameState.entrance == 2) {
    Colosso_AskDoneCheering(scene);
  } else {
    Engine_EventBegin();
    path = Colosso_GreetAtStage(scene, 3);
    if (path == 0) {
      Engine_EventSetMessage(MsgColossoTheyCallBrokenBridge);
      ColossoRiver_ResetBrokenBridge();
      Camera_SetSpeed(PIXELS(3), PIXELS(3) / 8);
      Camera_MoveTo(PIXELS(840), -1, PIXELS(216), 1);
      Engine_CameraWaitForMove();
      Event_ShowMessage(scene, 0);
      ColossoRiver_StartBrokenBridge();
      Engine_EventWait(60);
      Event_ShowMessage(scene, 0);
      Colosso_FadeInCompetitor(0, 736, 200);
      // UB: the void target is called through an incompatible return type.
      /* FAKEMATCH: called as returning a value, the discarded result's call
       * sets r0 after both other arguments, as the native does. Called
       * directly, through Actor_FaceDirection or through Call3, r0 is set first
       * (differ 2 in tbs-en).
       * Direct prototype rechecked 2026-10-16: 3 argument instructions
       * differ in tbs-en. */
      COLOSSO_RIVER_FACE_DIRECTION(ACTOR_PARTY_LEADER, 0, 0);
      ColossoRiver_WaitForBridgeOpening();
      Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 816, 200);
      Engine_EventWait(30);
      Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x105, 60);
      Event_ShowMessage(scene, 0);
      Colosso_RestoreCompetitor(0);
      Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
      Colosso_RepeatStageNumber(scene, 3);
    } else if (path == 1) {
      Engine_EventSetMessage(MsgColossoOperatorBridgeWillAlsoCheer);
      Event_ShowMessage(scene, 0);
    }
    Colosso_OfferCheeringSpot(path, scene, 3);
    Engine_EventEnd();
  }
}

/// The Lumber Water Crossing Area's attendant: a warrior rolls a log across;
/// then the objective.
static void Event_TalkToLumberAttendant(s32 scene) {
  s32 path;

  if (gGameState.entrance == 2) {
    Colosso_AskDoneCheering(scene);
    return;
  }
  Engine_EventBegin();
  path = Colosso_GreetAtStage(scene, 4);
  if (path == 0) {
    Engine_EventSetMessage(MsgColossoPlaceNormallyCalledLumberWater);
    Camera_SetSpeed(PIXELS(3), PIXELS(3) / 8);
    Camera_MoveTo(PIXELS(1088), -1, PIXELS(168), 1);
    Engine_CameraWaitForMove();
    Event_ShowMessage(scene, 0);
    Colosso_ShowPointer(120, 72, 0);
    Event_ShowMessage(scene, 0);
    Colosso_HidePointer();
    Engine_EventWait(15);
    Colosso_FadeInCompetitor(0, 984, 200);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 10);
    Event_ShowMessage(scene, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 30);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x106, 60);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 2, PIXELS(3) / 4);
    Colosso_WalkActorToAndStand(0, 1000, 192);
    Colosso_WalkActorToAndStand(0, 1000, 176);
    Colosso_WalkActorToAndStand(0, 1016, 168);
    Engine_EventWait(15);
    // UB: the void target is called through an incompatible return type.
    /* FAKEMATCH: called through a cast to the s32 call type the bytes show, its
     * result discarded (as call.h's Value3 did, 2026-10-13); called directly or
     * through Call3 the argument moves come in another order (differ 2 in
     * tbs-en).
     * Direct prototype rechecked 2026-10-16: 3 argument instructions
     * differ in tbs-en. */
    COLOSSO_RIVER_PLACE_STAGED_PAIR(LOG_EAST_WEST, 160, 0);
    Camera_MoveTo(PIXELS(1088), -1, PIXELS(168), 1);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
    Engine_EventWait(10);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 1192, 168);
    Engine_EventWait(10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 30);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 60);
    Event_ShowMessage(scene, 0);
    Colosso_RestoreCompetitor(0);
    Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
    Actor_SetPosition(LOG_EAST_WEST, PIXELS(1016), PIXELS(168));
    Colosso_RepeatStageNumber(scene, 4);
  } else if (path == 1) {
    Engine_EventSetMessage(MsgColossoLogsKeyClearingStage);
    Event_ShowMessage(scene, 0);
  }
  Colosso_OfferCheeringSpot(path, scene, 4);
  Engine_EventEnd();
}

/// Set the leader's movement speed for the shared Colosso dialogue.
static inline void InitializeActorZero(void) {
  Engine_ActorSetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
}

/// Set an attendant's movement speed for the shared Colosso dialogue.
static inline void InitializeSelectedActor(s32 actorId) {
  Engine_ActorSetSpeed(actorId, PIXELS(1), PIXELS(1) / 2);
}

/// Advance to the next message in the current explanation.
static __inline__ void AdvanceMessage(s32 amount) {
  gEventWork->message += amount;
}

// Shared source is included here in ROM order.
#include "common/colosso_solo.h"
#include "common/item_equip.h"
#include "common/colosso_task.h"

/// Select a presentation script and reset its playback state.
static void ColossoRiver_SelectPresentationScript(u32 mode, u32 param) {
  const s16 *script;

  gColosso_LetteringMode = mode;
  gColosso_LetteringParam = (param << 4);

  FieldScheduler_AddCallback(Colosso_UpdateLettering, 0xc80);

  script = gColossoRiver_RoundSpans;
  if (mode == 2) {
    script = gColossoRiver_ModeRecordTwo;
  }
  if (mode == 4) {
    script = gColossoRiver_ModeRecordFour;
  }
  if (mode == 3) {
    if (param != 0) {
      script = gColossoRiver_SpanB;
    } else {
      script = gColossoRiver_SpanA;
    }
  }

  gColosso_LetteringTimer = 0;
  gColosso_LetteringScript = (s16 *)script;
  gColosso_ModeMoveTarget = 0;
  gColosso_ModeMoveDuration = 0;
  gColosso_LetteringPosition = 0;
}

/// The match opens: portraits of the warriors and the round announced.
static void ColossoRiver_ShowRoundIntroduction(s32 round) {
  if (round == 0) {
    Engine_EventBegin();
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(30);
    Engine_AudioPlayCue(MUS_089);
    Colosso_LoadLettering(0);
    ColossoRiver_SelectPresentationScript(1, 0);
    Engine_EventWait(120);
    Engine_EventEnd();
  } else {
    Engine_AudioPlayCue(SE_247);
    Engine_EventBegin();
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    gColossoRiver_RoundSpans[15] = round * 60;
    Engine_EventWait(30);
    Engine_AudioPlayCue(round + 90);
    Colosso_LoadLettering(round);
    ColossoRiver_SelectPresentationScript(1, 0);
    Engine_EventWait(120);
    while (AudioCommand_GetStateByte() != 0) {
      Engine_TaskWait(1);
    }
    Engine_AudioPlayCue(SE_289);
    Colosso_LoadLettering(5);
    ColossoRiver_SelectPresentationScript(2, 0);
    Engine_AudioPlayCue(SE_236);
    Engine_EventWait(60);
    ColossoRiver_SelectPresentationScript(2, 1);
    Engine_AudioPlayCue(SE_236);
    Engine_EventWait(60);
    Colosso_LoadLettering(6);
    ColossoRiver_SelectPresentationScript(2, 0);
    Engine_AudioPlayCue(SE_236);
    Engine_EventWait(60);
    Colosso_LoadLettering(7);
    ColossoRiver_SelectPresentationScript(4, 0);
    Engine_AudioPlayCue(SE_237);
    Audio_PlayCueFromEventWork();
    Engine_EventEnd();
    Engine_GameFlagSet(0x123);
  }
}

/// The result: the winner's portrait and fanfare, then the course closes.
static void ColossoRiver_ShowMatchResult(s32 result) {

  s32 kind;

  Engine_AudioPlayCue(SE_247);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  gColossoRiver_SpanA[13] = result * 60;
  STORE_CONST(gColossoRiver_SpanB[13]) = (result < 0 ? -result : result) * 60;
  if (result < 0) {
    Engine_EventWait(30);
    Engine_AudioPlayCue(MUS_086);
    Colosso_LoadLettering(8);
    // UB: the void target is called through an incompatible return type.
    /* FAKEMATCH: called through a cast to the s32 call type the bytes show, its
     * result discarded (as call.h's Value2 did, 2026-10-13); called directly or
     * through Call2 the argument moves come in another order (differ 2 in
     * tbs-en).
     * Direct prototype rechecked 2026-10-16: 2 argument instructions
     * differ in tbs-en. */
    COLOSSO_RIVER_SELECT_MODE_BLOCK(3, 1);
    Engine_EventWait(-result * 60 + 60);
    kind = 0;
  } else {
    Engine_EventWait(30);
    Engine_AudioPlayCue(result + 90);
    Colosso_LoadLettering(4);
    // UB: the void target is called through an incompatible return type.
    /* FAKEMATCH: called through a cast to the s32 call type the bytes show, its
     * result discarded (as call.h's Value2 did, 2026-10-13); called directly or
     * through Call2 the argument moves come in another order (differ 2 in
     * tbs-en).
     * Direct prototype rechecked 2026-10-16: 2 argument instructions
     * differ in tbs-en. */
    COLOSSO_RIVER_SELECT_MODE_BLOCK(3, 0);
    Engine_EventWait(result * 60 + 60);
    kind = 8;
  }
  Engine_ActorShowEmote(kind, 0x105, 0);
  while (AudioCommand_GetStateByte() != 0) {
    Engine_TaskWait(1);
  }
  Engine_AudioPlayCue(MUS_019);
  Engine_EventWait(30);
  Engine_AudioPlayCue(SE_289);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
}

// Shared source is included here in ROM order.
#include "common/colosso_marker.h"
#include "common/colosso_item_icon.h"

/// Write eight hexadecimal digits and a terminating null.
UNUSED static void Text_WriteU32AsHex(u8 *buf, u32 value) {
  s32 i;

  buf += 8;
  *buf = 0;
  buf--;
  for (i = 7; i >= 0; i--) {
    *buf = gHexDigits[value & 15];
    value >>= 4;
    buf--;
  }
}

/// Provide the empty callback retained by the original event sequence.
static void ColossoRiver_NoOp(void) {}

/// Mark the shared stage state ready for the waiting event.
UNUSED static void ColossoRiver_MarkStageReady(void) {
  s16 *ready = (s16 *)gSceneState;
  u16 nine = 9;

  *ready = nine;
}

/// Wait until the stage's setup task reports readiness.
static void ColossoRiver_WaitForStageReady(void) {
  s16 *ready = (s16 *)gSceneState;

  while (*ready != 9) {
    Engine_TaskWait(1);
  }
}

/// Occasionally spawn an animated effect around the finish item.
static void ColossoRiver_SpawnRandomEffect(union FieldObject *fieldObject) {
  struct FieldActor *actor = (struct FieldActor *)fieldObject;
  s32 position[3];
  u32 chance;

  if (actor->velocityY >= -255 && actor->velocityY <= 255) {
    actor->motionFlags = 0;
  }
  chance = Engine_RandomNext();
  if (chance * 100 >> 16 <= 9) {
    struct FieldActor *effect;
    s32 radius;
    s32 angle;

    SET_VECTOR(position, actor->x.fixed, actor->y.fixed, actor->z.fixed);
    radius = Engine_RandomNext();
    angle = Engine_RandomNext();
    Vector_AddPolarOffset(radius << 4, angle, position);
    {
      s32 x = position[0];
      s32 y = position[1];
      s32 z = position[2];

      effect = Engine_ObjectCreate(285, x, y, z);
    }
    if (effect != 0) {
      effect->motionFlags = 0;
      Engine_ActorSetSpriteFlags(effect, 0);
      Engine_ObjectSetScript(effect, gColossoRiver_ScriptA);
      Object_SetMode(effect, 1);
      Object_SetMode(effect, 0);
    }
  }
}

/// Present the collected item above its collector and signal completion.
static s32
ColossoRiver_PlaceFinishItemAboveActor(struct ScriptInterpreter *interpreter) {
  struct FieldActor *collector = (struct FieldActor *)interpreter;
  struct FieldActor *itemActor = Object_GetById(collector->timer);

  Engine_ObjectSetPosition(itemActor, collector->x.fixed,
                           collector->y.fixed + PIXELS(36), collector->z.fixed);
  itemActor->motionFlags = 0;
  Engine_ObjectSetScript(itemActor, gColossoRiver_ScriptB);
  Engine_AudioPlayCue(MUS_TREASURE_FOUND);
  collector->timer = 0;
  return 0;
}

/// Move to the selected finish item and present it to the party.
static s32 ColossoRiver_CollectFinishItem(s32 firstItem, s32 secondItem) {

  struct ColossoGaugeWork *work = (struct ColossoGaugeWork *)gColossoWork;
  struct FieldActor *actor;
  s32 flag;
  s32 x;
  s32 z;
  u16 *itemActor;
  s16 *pendingItemActor;

  flag = Engine_GameFlagIsSet(FLAG_SCENE_TEMP_11);

  actor = Object_GetById(gGameState.selectedActor);

  if (work->finishX < actor->x.fixed) {
    x = work->finishX + PIXELS(12);
  } else {
    x = work->finishX - PIXELS(12);
  }

  if (flag != 0) {
    z = work->finishZ + PIXELS(16);
    itemActor = &work->finishItemActorB;
  } else {
    z = work->finishZ - PIXELS(16);
    itemActor = &work->finishItemActorA;
  }

  pendingItemActor = &actor->timer;
  *pendingItemActor = *itemActor;
  actor->acceleration = PIXELS(1) / 4;
  actor->speed = PIXELS(1);

  Engine_ObjectSetPosition(actor, x, 0, z);
  Engine_GameFlagSet(FLAG_SCENE_TEMP_11);
  Engine_ObjectSetScript(actor, gColossoRiver_ApproachScript);

  while (*pendingItemActor != 0) {
    Engine_TaskWait(1);
  }

  if (flag == 0) {
    // UB: the void target is called through an incompatible return type.
    /* FAKEMATCH: the void function is called as returning s32, the call the
     * bytes show; called by its prototype the argument moves come in another
     * order (differ 2 in tbs-en).
     * Direct prototype rechecked 2026-10-16: 2 argument instructions
     * differ in tbs-en. */
    COLOSSO_RIVER_APPLY_MATCHING_SLOT_VALUES(0, firstItem);
    UiWork_PushValueSlot(firstItem, 2);
  } else {
    // UB: the void target is called through an incompatible return type.
    /* FAKEMATCH: the same call as returning s32; by its prototype: differ 2 in
     * tbs-en.
     * Direct prototype rechecked 2026-10-16: 2 argument instructions
     * differ in tbs-en. */
    COLOSSO_RIVER_APPLY_MATCHING_SLOT_VALUES(0, secondItem);
    UiWork_PushValueSlot(secondItem, 2);
  }

  UiWork_PushValueSlot(gGameState.selectedActor, 1);
  Engine_MessageShowCentered(MsgColossoIsaacGotItem, 3);
  ObjectDispatch_WaitForValue16(actor);

  return flag;
}

// Shared source is included here in ROM order.
#include "common/colosso_path_rival.h"
#include "common/colosso_setup.h"
#include "common/colosso_scene_state.h"
#include "common/object_findslot.h"

/// Check the next cell and push its object one step if clear.
static void ColossoRiver_PushObjectAhead(void) {

  struct FieldActor *subject;
  struct FieldActor *target;
  struct FieldActor *blocker;
  u32 packedStep;
  u32 direction;
  Position3 position;
  s32 zero;
  s32 actorId;

  actorId = gGameState.selectedActor;
  subject = Object_GetById(actorId);

  direction = subject->facing >> 12;

  packedStep = gColossoRiver_DirectionSteps[direction];
  position.x = subject->x.fixed + (packedStep & 0xffff0000);
  position.y = subject->y.fixed;
  packedStep <<= 16;
  position.z = subject->z.fixed + packedStep;

  target = SceneData_FindSlotAtPosition(&position, subject);
  if (target == 0) {
    return;
  }

  packedStep = gColossoRiver_DirectionSteps[direction];
  position.x = target->x.fixed + (packedStep & 0xffff0000);
  position.y = target->y.fixed;
  packedStep <<= 16;
  position.z = target->z.fixed + packedStep;

  blocker = SceneData_FindSlotAtPosition(&position, target);
  if (blocker != 0 && (blocker->collisionFlags & 1) != 0) {
    return;
  }

  position.x = target->x.fixed;
  position.y = target->y.fixed + PIXELS(16);
  position.z = target->z.fixed;

  blocker = SceneData_FindSlotAtPosition(&position, target);
  if (blocker != 0 && (blocker->collisionFlags & 1) != 0) {
    return;
  }

  target->terrainId = 2;
  zero = 0;

  packedStep = gColossoRiver_DirectionSteps[direction];
  position.x = target->x.fixed + (packedStep & 0xffff0000);
  position.y = target->y.fixed;
  packedStep <<= 16;
  position.z = target->z.fixed + packedStep;

  if (FieldObject_CheckMovementCollision(target, &position) > 0) {
    return;
  }

  Object_SetMode(subject, 8);
  Engine_TaskWait(15);

  target->speed = PIXELS(1) / 5;
  target->acceleration = PIXELS(1) / 5;
  Engine_ObjectSetPosition(target, position.x, position.y, position.z);

  subject->speed = PIXELS(1) / 5;
  subject->acceleration = PIXELS(1) / 5;
  Engine_ObjectSetPosition(subject, position.x, position.y, position.z);

  Engine_AudioPlayCue(SE_238);
  FieldObject_WaitForMove(target);
  Engine_AudioPlayCue(SE_288);

  target->x.fixed = position.x;
  target->z.fixed = position.z;
  target->velocityX = zero;
  target->velocityZ = zero;

  Object_SetMode(subject, 1);
}

// Shared source is included here in ROM order.
#include "common/colosso_occupant.h"

// clang-format off
const u8 gColosso_LetteringPaletteOffsets[] = {
#if LANGUAGE == LANGUAGE_JAPANESE || LANGUAGE == LANGUAGE_ENGLISH
    0, 32, 32, 32, 96, 64, 64, 64, 128, 0,
#else
    0, 32, 32, 32, 96, 64, 64, 160, 128, 0,
#endif
};

const s16 gColossoRiver_ModeRecordTwo[] = {
    0x1000, 512, 1, 0x2000, 0, 1, 0x1000, 256,
    6, 0x2000, 16, 6, 0x7fff, 30, 0x2000, 0,
    30, 0x7fff, 30, -1,
};

const s16 gColossoRiver_SpanB[] = {
    0x1000, 128, 1, 0x2000, 0, 1, 0x1000, 256,
    6, 0x2000, 16, 6, 0x7fff, 60, 0x1000, 384,
    6, 0x7fff, 6, 0x1000, 240, 6, 0x7fff, 6,
    0x1000, 368, 6, 0x7fff, 6, 0x1000, 224, 6,
    0x7fff, 6, 0x2000, 0, 30, 0x1000, 352, 6,
    0x7fff, 6, 0x1000, 208, 6, 0x7fff, 6, 0x1000,
    336, 6, 0x7fff, 6, 0x1000, 192, 6, 0x7fff,
    6, 0x1000, 320, 6, 0x7fff, 6, -1,
};

char gHexDigits[20] = "0123456789ABCDEF";

s32 gColossoRiver_ScriptA[] = {
    SC_ADD(SW_Y, -PIXELS(1) / 4),
    SC_WAIT(1),
    SC_LOOP(60, SC_TOP),
    SC_FREE,
};

s32 gColossoRiver_ScriptB[] = {
    SC_SET(SW_HOOK, (s32)ColossoRiver_SpawnRandomEffect),
    SC_LABEL(1),
    SC_WAIT(10),
    SC_SET(SW_SCALE_X, (11 * Q16_ONE) / 8),
    SC_SET(SW_SCALE_Y, (11 * Q16_ONE) / 8),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, (3 * Q16_ONE) / 2),
    SC_SET(SW_SCALE_Y, (3 * Q16_ONE) / 2),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, (9 * Q16_ONE) / 8),
    SC_SET(SW_SCALE_Y, (9 * Q16_ONE) / 8),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, Q16_ONE),
    SC_SET(SW_SCALE_Y, Q16_ONE),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, (11 * Q16_ONE) / 8),
    SC_SET(SW_SCALE_Y, (11 * Q16_ONE) / 8),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, (3 * Q16_ONE) / 2),
    SC_SET(SW_SCALE_Y, (3 * Q16_ONE) / 2),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, (9 * Q16_ONE) / 8),
    SC_SET(SW_SCALE_Y, (9 * Q16_ONE) / 8),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, Q16_ONE),
    SC_SET(SW_SCALE_Y, Q16_ONE),
    SC_WAIT(2),
    SC_WAIT(50),
    SC_LOOP(2, SC_TO(1)),
    SC_POS(0, 0, 0),
    SC_SET(SW_HOOK, 0),
    SC_END,
};

/// Pixel steps by direction: X in the upper halfword, Z in the lower.
#define DIRECTION_STEP(x, z) ((u32)(x) << 16 | (u16)(z))

u32 gColossoRiver_DirectionSteps[] = {
    DIRECTION_STEP(16, 0),
    DIRECTION_STEP(16, 0),
    DIRECTION_STEP(16, 0),
    DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),
    DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16),
    DIRECTION_STEP(16, 0),
};

struct SceneEntrance gColossoRiver_Entrances[] = {
    { 0, CONDITION_ALWAYS, 920, 0, 200, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { 1, CONDITION_ALWAYS, 312, 0, 104, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

u32 gColossoRiver_Exits[] = {
    SCENE_EXITS(143),
    SCENE_EXIT(10, 143, 1),
    SCENE_EXIT(11, 140, 12),
    SCENE_EXIT(4, 140, 21),
    SCENE_EXIT(5, 138, 98),
    SCENE_EXITS_END,
};

struct ScenePlacement gColossoRiver_Placements[] = {
    { 0x0, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(312), 0, PIXELS(104), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { IVAN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { MIA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0x3e, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(312), 0, PIXELS(168), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(408), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(456), 0, PIXELS(184), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xe5, CONDITION_ALWAYS, 7,
        PIXELS(601), 0, PIXELS(146), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x122, CONDITION_ALWAYS, 7,
        PIXELS(552), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x122, CONDITION_ALWAYS, 7,
        PIXELS(664), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xee, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(760), 0, PIXELS(194), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xa5, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(944), 0, PIXELS(216), FACING_WEST, TALK_KEEP_FACING, 0 },
    { 0xec, CONDITION_ALWAYS, 7,
        PIXELS(1048), 0, PIXELS(136), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xed, CONDITION_ALWAYS, 7,
        PIXELS(1016), 0, PIXELS(168), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x98, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(360), 0, PIXELS(424), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0xa5, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(376), 0, PIXELS(312), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0xa5, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(600), 0, PIXELS(312), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0xa5, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(824), 0, PIXELS(312), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0xa5, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1080), 0, PIXELS(312), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x16, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1304), 0, PIXELS(176), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x16, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1304), 0, PIXELS(208), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

s32 gColossoRiver_Countdown = 0;

struct SceneEvent gColossoRiver_Events[] = {
    { TOUCH_WALKING, 90, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushRockOrCheckAhead) },
    { TOUCH_WALKING, 10, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushRock) },
    { TOUCH_WALKING, 11, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushRock) },
    { TOUCH_WALKING_INTO(FACING_WEST), 12, 0x302, EVENT_SCRIPT(Event_PushPipeworksBlock) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 20, CONDITION_ALWAYS, EVENT_SCRIPT(Event_RollLogNorthSouth) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 20, CONDITION_ALWAYS, EVENT_SCRIPT(Event_RollLogNorthSouth) },
    { TOUCH_WALKING_INTO(FACING_WEST), 21, CONDITION_ALWAYS, EVENT_SCRIPT(Event_RollLogEastWest) },
    { TOUCH_WALKING_INTO(FACING_EAST), 21, CONDITION_ALWAYS, EVENT_SCRIPT(Event_RollLogEastWest) },
    { EVENT_TOUCH, 50, FLAG_SCENE_TEMP_10, EVENT_SCRIPT(Event_ReachArena) },
    { SEARCH(SEARCH_CHEST), 100, 0x308, FIND_ITEM(ITEM_NUT) },
    { SEARCH(SEARCH_CHEST), 101, 0x309, FIND_ITEM(ITEM_OIL_DROP) },
    { 0x8413, 103, 0x30a, FIND_ITEM(ITEM_NUT) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ROCK_A, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SettleRocks) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ROCK_B, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SettleRocks) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ROCK_C, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SettleRocks) },
    { PSYNERGY_ON_ACTOR(ABILITY_GROWTH), GROWTH_PLANT, 0x303, EVENT_SCRIPT(Event_GrowPlant) },
    { PSYNERGY_ON_ACTOR(ABILITY_DOUSE), PIPE_WEST, 0x301, EVENT_SCRIPT(Event_DousePipe) },
    { EVENT_TALK, GUIDE, CONDITION_ALWAYS, EVENT_SCRIPT(Colosso_AskReadyToRace) },
    { EVENT_TALK, ROCK_ATTENDANT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToRockAttendant) },
    { EVENT_TALK, PIPEWORKS_ATTENDANT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToPipeworksAttendant) },
    { EVENT_TALK, BRIDGE_ATTENDANT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToBridgeAttendant) },
    { EVENT_TALK, LUMBER_ATTENDANT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToLumberAttendant) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), GUIDE, CONDITION_ALWAYS, MSGNO(MsgColossoRiverThisYearsFinalsAreIncredibleThis) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ROCK_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoRiverMovingSuchAHugeRockMust) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), PIPEWORKS_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoRiverTheresOnlyOneFinalAndThis) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BRIDGE_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoRiverDidYouSeeHisMovesOut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), LUMBER_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoRiverHowManyWarriorsHaveSwallowedTheir) },
    { EVENT_RAISED, 99, CONDITION_ALWAYS, EVENT_SCRIPT(Colosso_TakePlaceForMatch) },
    { SCENE_EVENTS_END },
};

s16 gColosso_LetteringSlot = -1;

s16 gColossoRiver_RoundSpans[] = {
#if LANGUAGE == LANGUAGE_JAPANESE
    0x4000, -188, 2048, 0x1000, 256, 1, 0x2000, 16,
#else
    0x4000, -188, 2048, 0x1000, 384, 1, 0x2000, 16,
#endif
    1, 0x7fff, 14, 0x3000, 0, 20, 0x7fff, 60,
    0x3000, 2048, 20, 0x7fff, 60, -1,
};

s16 gColossoRiver_ModeRecordFour[] = {
#if LANGUAGE == LANGUAGE_FRENCH
    0x1000, 512, 1, 0x2000, 0, 1, 0x1000, 384,
    6, 0x2000, 16, 6, 0x7fff, 6, 0x1000, 512,
    6, 0x7fff, 6, 0x1000, 384, 6, 0x7fff, 6,
    0x1000, 512, 6, 0x7fff, 6, 0x1000, 384, 6,
    0x7fff, 6, 0x2000, 0, 30, 0x1000, 512, 6,
    0x7fff, 6, 0x1000, 384, 6, 0x7fff, 6, 0x1000,
    512, 6, 0x7fff, 6, 0x1000, 384, 6, 0x7fff,
    6, 0x1000, 512, 6, 0x7fff, 6, -1,
#elif LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_ITALIAN
    0x1000, 384, 1, 0x2000, 0, 1, 0x1000, 256,
    6, 0x2000, 16, 6, 0x7fff, 6, 0x1000, 384,
    6, 0x7fff, 6, 0x1000, 256, 6, 0x7fff, 6,
    0x1000, 384, 6, 0x7fff, 6, 0x1000, 256, 6,
    0x7fff, 6, 0x2000, 0, 30, 0x1000, 384, 6,
    0x7fff, 6, 0x1000, 256, 6, 0x7fff, 6, 0x1000,
    384, 6, 0x7fff, 6, 0x1000, 256, 6, 0x7fff,
    6, 0x1000, 384, 6, 0x7fff, 6, -1,
#else
    0x1000, 512, 1, 0x2000, 0, 1, 0x1000, 256,
    6, 0x2000, 16, 6, 0x7fff, 6, 0x1000, 384,
    6, 0x7fff, 6, 0x1000, 256, 6, 0x7fff, 6,
    0x1000, 384, 6, 0x7fff, 6, 0x1000, 256, 6,
    0x7fff, 6, 0x2000, 0, 30, 0x1000, 384, 6,
    0x7fff, 6, 0x1000, 256, 6, 0x7fff, 6, 0x1000,
    384, 6, 0x7fff, 6, 0x1000, 256, 6, 0x7fff,
    6, 0x1000, 384, 6, 0x7fff, 6, -1,
#endif
};

s16 gColossoRiver_SpanA[] = {
    0x1000, 128, 1, 0x2000, 0, 1, 0x1000, 256,
    6, 0x2000, 16, 6, 0x7fff, 60, 0x1000, 384,
    6, 0x7fff, 6, 0x1000, 272, 6, 0x7fff, 6,
    0x1000, 400, 6, 0x7fff, 6, 0x1000, 288, 6,
    0x7fff, 6, 0x2000, 0, 30, 0x1000, 416, 6,
    0x7fff, 6, 0x1000, 304, 6, 0x7fff, 6, 0x1000,
    432, 6, 0x7fff, 6, 0x1000, 320, 6, 0x7fff,
    6, 0x1000, 448, 6, 0x7fff, 6, -1,
};
// clang-format on

s16 gColosso_MarkerSlot = -1;

s32 gColosso_RivalFinishScript[] = {
    SC_ARG(2),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(6),
    SC_CALL(ColossoRiver_PlaceFinishItemAboveActor),
    SC_WAIT(3),
    SC_ARG(10),
    SC_WAIT(150),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_ARG(1),
    SC_END,
};

s32 gColossoRiver_ApproachScript[] = {
    SC_ARG(2),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(6),
    SC_CALL(ColossoRiver_PlaceFinishItemAboveActor),
    SC_WAIT(3),
    SC_ARG(28),
    SC_WAIT(150),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_ARG(1),
    SC_END,
};

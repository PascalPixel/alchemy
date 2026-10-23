/// @file
/// Colosseum Final 1 (MapId_ColosseumAzartMatch), the site of the first
/// finals match, where Isaac races Azart, the second-best of the warriors who
/// cleared the trials, through four stages to the arena and fights him there.
///
/// Babi has put Isaac in the finals himself, and only Isaac may race; Garet,
/// Ivan and Mia help from the stages. The stages, by their attendants'
/// names, are the Rock Challenge, the Pipeworks, the Broken Bridge and the
/// Lumber Water Crossing Area. Isaac comes here three times, each time at its
/// own warp destination, and ColosseumAzartMatch_Initialize blocks Retreat and
/// sets up the visit that warp destination stands for after restoring the
/// stages from their temporary flags (FLAG_PIPES_MOVED,
/// FLAG_PIPEWORKS_PIPE_PUSHED, FLAG_PLANT_GROWN and the rock's column in
/// FLAG_SECOND_ROCK_COLUMN), so that what the friends changed stays changed for
/// the race.
///
/// - COLOSSEUM_AZART_MATCH_FROM_COLOSSEUM, the tour, from the Colosseum's east
///   passage. The guide (ColosseumAzartMatch_ExplainFinalsRules) tells the
///   party that warriors enter without their own equipment and race through the
///   stages to the arena, where the first to arrive takes the better of two
///   prizes and keeps it for the next match, and he pans over the stages if
///   asked. Each stage's attendant names it, the first time with a
///   demonstration (Isaac appears on the stage to show how it is crossed, or,
///   at the Pipeworks, a pointer shows how the pipes work), and offers a place
///   to cheer there: the friend Isaac picks walks to the stage and leaves the
///   party (Colosso_OfferCheeringSpot). When all three are placed, the guide
///   sends Isaac on to the Colosseum's waiting room
///   (COLOSSEUM_AZART_MATCH_TO_COLOSSEUM_WEST).
/// - COLOSSEUM_AZART_MATCH_FROM_COLOSSEUM_WEST, after the warriors have marched
///   out. The lettering reads `Lend a Hand`, and Garet, Ivan and Mia in turn
///   are the party alone, starting where they chose to cheer. Each may use
///   Psynergy on the stages for Isaac: Douse on the Pipeworks' west pipe fills
///   it, Growth grows the plant before the Broken Bridge, Move shifts the Rock
///   Challenge's rocks. Using Psynergy (Colosso_TakePlaceForMatch) or telling a
///   stage's attendant they are done (Colosso_AskDoneCheering) ends that
///   friend's turn, and where they stand is kept as their place; after Mia,
///   COLOSSEUM_AZART_MATCH_LEND_A_HAND_TO_RACE starts the race.
/// - COLOSSEUM_AZART_MATCH_RACE_FROM_LEND_A_HAND, the race. Isaac runs alone
///   against Azart, who follows a recorded path (Colosso_StartOpponent), with a
///   gauge at the top of the screen showing how near each is to the finish
///   (Colosso_StartRaceGauge), after the lettering `Match 1` and a count of
///   three. Azart runs east of the finish (1,304 pixels across, where the
///   prizes stand), his path the mirror of one through Isaac's own stages,
///   so he is seen only as he comes into the arena
///   (https://alchemy.pascalpixel.com/places/colosseum-azart-match/race.png:
///   Isaac left standing at the start, Azart's place over the race mirrored
///   back across the finish and drawn in red on the course, a dot every five
///   seconds, and the gauge at four moments). At the arena
///   (EventScript_ReachArena) the first to arrive takes the Iron Shield and the
///   second the Padded Gloves, which Isaac is given and equipped with, and the
///   two fight: the battle is ENCOUNTER_ZONE_COLOSSO's first or second group,
///   the Azart who has the shield or the one who has not.
///
/// A win returns to COLOSSEUM_AZART_MATCH_AFTER_MATCH_WON, which shows the
/// result and sends Isaac back to the Colosseum
/// (COLOSSEUM_AZART_MATCH_TO_COLOSSEUM_EAST, to
/// COLOSSEUM_FROM_COLOSSEUM_AZART_MATCH_EAST); a loss returns to
/// COLOSSEUM_AZART_MATCH_AFTER_MATCH_LOST, and
/// COLOSSEUM_AZART_MATCH_TO_BABI_PALACE wakes the party in Babi's Palace, where
/// Isaac's friends tell him he was only dreaming of the finals.
///
/// Open questions: none.

#include "action_script.h"
#include "version.h"
#include "save_block.h"
#include "io_write_queue.h"
#include "map_ids.h"
#include "vram_cache.h"
#include "unit/import.h"
#include "unit/map/colosseum_azart_match.h"
#include "unit/map/common/colosso_solo.h"
#include "unit/map/common/colosso_marker.h"
#include "unit/map/common/colosso_setup.h"
#include "unit/map/common/colosso_occupant.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/field_services.h"
#include "unit/field/field.h"
#include "unit/game/game.h"
#include "unit/window/window.h"
#include "unit/window/screens.h"
#include "unit/menu/menu.h"
#include "unit/sound/sound.h"
#include "unit/runtime/runtime.h"
#include "unit/map/common/colosso_item_icon.h"
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
#include "task.h"
#include "script_layer.h"
#include "field_service.h"
#include "map_tables.h"
#include "pushed_object.h"
#include "resource_ids.h"
#include "text/msg_ids.h"
#include "battle_unit.h"
#include "resource.h"
#include "object.h"
#include "ability_ids.h"
#include "edition.h"
#include "item_ids.h"
#include "objcmd.h"
#include "object_event.h"
#include "heap.h"
#include "map_header.h"
#include "fixed_math.h"
#include "colosso_gauge.h"
#include "macros.h"
#include "flags.h"
#include "songs.h"
#include "audio_engine_symbols.h"

// UB: each of these calls a void function through an s32 function type, the
// call the bytes show; UBFIX calls it as declared. Each use is tagged.
#ifdef UBFIX
#define COLOSSEUM_AZART_MATCH_FACE_DIRECTION ObjectMotion_ArmCallback
#define COLOSSEUM_AZART_MATCH_PLACE_STAGED_PAIR                                \
  ColosseumAzartMatch_MoveObjectEventPairByOffset
#define COLOSSEUM_AZART_MATCH_SELECT_MODE_BLOCK                                \
  ColosseumAzartMatch_SelectPresentationScript
#define COLOSSEUM_AZART_MATCH_APPLY_MATCHING_SLOT_VALUES Map_GiveAndEquipItem
#else
#define COLOSSEUM_AZART_MATCH_FACE_DIRECTION                                   \
  ((s32 (*)())ObjectMotion_ArmCallback)
#define COLOSSEUM_AZART_MATCH_PLACE_STAGED_PAIR                                \
  ((s32 (*)())ColosseumAzartMatch_MoveObjectEventPairByOffset)
#define COLOSSEUM_AZART_MATCH_SELECT_MODE_BLOCK                                \
  ((s32 (*)())ColosseumAzartMatch_SelectPresentationScript)
#define COLOSSEUM_AZART_MATCH_APPLY_MATCHING_SLOT_VALUES                       \
  ((s32 (*)(s32, s32))Map_GiveAndEquipItem)
#endif

static void ColosseumAzartMatch_ExplainFinalsRules(s32 guide);
static void ColosseumAzartMatch_ShowRoundIntroduction(s32 round);
static void ColosseumAzartMatch_ShowMatchResult(s32 result);
static void ColosseumAzartMatch_NoOp(void);
static void ColosseumAzartMatch_WaitForStageReady(void);
static s32 ColosseumAzartMatch_CollectFinishItem(s32 firstItem, s32 secondItem);
static void ColosseumAzartMatch_PushObjectAhead(void);

typedef void (*MapTask)(void);

/// The first finals match's object events, by the guide's and attendants'
/// explanations: four stages (the Rock Challenge, the Pipeworks, the Broken
/// Bridge and the Lumber Water Crossing Area) lead to the arena where the two
/// warriors fight.
enum {
  /// Azart, racing Isaac to the arena.
  /// ★ Named from the game: "The name's Azart."
  /// (MsgColosseumImRatedAsSecondBest)
  LOCALID_OPPONENT = 8,
  /// The Rock Challenge's three rocks, stone pillars the party moves to clear
  /// a path.
  /// ★ Named from the game: "Moving such a huge rock must be a real workout."
  /// (MsgColossoRiverMovingSuchAHugeRockMust)
  LOCALID_ROCK_A = 9,
  /// ★ Named from the game: "Moving such a huge rock must be a real workout."
  /// (MsgColossoRiverMovingSuchAHugeRockMust)
  LOCALID_ROCK_B = 10,
  /// ★ Named from the game: "Moving such a huge rock must be a real workout."
  /// (MsgColossoRiverMovingSuchAHugeRockMust)
  LOCALID_ROCK_C = 11,
  /// Pushed west to let water into the Pipeworks. A pipe section, drawn as
  /// mercury_lighthouse.c's LOCALID_THIRD_PIPE.
  /// ★ Named from the game: "See the water pipes?"
  /// (MsgColossoAreaCalledPipeworks)
  LOCALID_PIPEWORKS_PIPE = 12,
  /// The Pipeworks' two pipes, which rise and sink against each other as
  /// water fills one, like scales.
  /// ★ Named from the game: "See the water pipes?"
  /// (MsgColossoAreaCalledPipeworks)
  LOCALID_PIPE_WEST = 13,
  /// ★ Named from the game: "See the water pipes?"
  /// (MsgColossoAreaCalledPipeworks)
  LOCALID_PIPE_EAST = 14,
  /// The plant Growth grows into a vine before the Broken Bridge.
  /// ★ Named from what the player sees: a small sprout (SPRITE_SPROUT) on the
  /// ground before the Broken Bridge (its render)
  LOCALID_GROWTH_PLANT = 15,
  /// A young man with fair hair who works the Broken Bridge.
  /// ★ Named from the game: "The operator of the bridge will also cheer for
  /// you in this stage." (MsgColossoOperatorBridgeWillAlsoCheer)
  LOCALID_BRIDGE_OPERATOR = 16,
  /// The Lumber Water Crossing Area's two floating logs.
  /// ★ Named from the game: "The logs are the key to clearing this stage."
  /// (MsgColossoLogsKeyClearingStage)
  LOCALID_LOG_NORTH_SOUTH = 17,
  /// ★ Named from the game: "The logs are the key to clearing this stage."
  /// (MsgColossoLogsKeyClearingStage)
  LOCALID_LOG_EAST_WEST = 18,
  /// A soldier who explains the finals.
  /// ★ Named from the game: "Ask the attendants for explanations of the
  /// stages." (MsgColossoAskAttendantsForExplanationsStages)
  LOCALID_GUIDE = 19,
  /// The stages' attendants, young men with fair hair in white tunics.
  /// ★ Named from the game: "They call this the Rock Challenge."
  /// (MsgColossoCallRockChallenge)
  LOCALID_ROCK_ATTENDANT = 20,
  /// ★ Named from the game: "This area is called the Pipeworks."
  /// (MsgColossoAreaCalledPipeworks)
  LOCALID_PIPEWORKS_ATTENDANT = 21,
  /// ★ Named from the game: "They call this the Broken Bridge."
  /// (MsgColossoTheyCallBrokenBridge)
  LOCALID_BRIDGE_ATTENDANT = 22,
  /// ★ Named from the game: "This place is normally called the Lumber Water
  /// Crossing Area." (MsgColossoPlaceNormallyCalledLumberWater)
  LOCALID_LUMBER_ATTENDANT = 23,
  /// The arena's prizes, drawn as item icons at two blank object events
  /// (Colosso_ShowItemIcon).
  /// ★ Named from what the player sees: the Iron Shield's icon at the arena's
  /// finish, taken by whoever reaches it first (its render)
  LOCALID_PRIZE_A = 24,
  /// ★ Named from what the player sees: the Padded Gloves' icon at the arena's
  /// finish, taken by whoever reaches it second (its render)
  LOCALID_PRIZE_B = 25
};

/// The temporary flags that keep what the friends changed on the stages, for
/// the race (ColosseumAzartMatch_Initialize restores them).
enum {
  /// ★ Named from the game: "See the water pipes?"
  /// (MsgColossoAreaCalledPipeworks)
  FLAG_PIPES_MOVED =
      FLAG_MAPSEC_TEMP_01, ///< the Pipeworks' pipes, as water filled them
  /// ★ Named from the game: "This area is called the Pipeworks."
  /// (MsgColossoAreaCalledPipeworks)
  FLAG_PIPEWORKS_PIPE_PUSHED = FLAG_MAPSEC_TEMP_02,
  /// ★ Named from the game: "Growth" (MsgAbilityName)
  FLAG_PLANT_GROWN =
      FLAG_MAPSEC_TEMP_03, ///< by Growth, before the Broken Bridge
  /// The byte of flags that keeps the Rock Challenge's second rock's column.
  /// ★ Named from the game: "They call this the Rock Challenge."
  /// (MsgColossoCallRockChallenge)
  FLAG_SECOND_ROCK_COLUMN = FLAG_MAPSEC_TEMP_10,
  /// Someone has reached the arena (ColosseumAzartMatch_CollectFinishItem).
  /// ★ Named from the game: "You must get through several stages before you
  /// reach the arena." (MsgColossoWarriorsEnterFinalsWithoutAny)
  FLAG_ARENA_REACHED = FLAG_MAP_TEMP_11
};

const struct MapWarpDest *ColosseumAzartMatch_GetWarpDests(void) {
  return gColosseumAzartMatch_WarpDests;
}

const struct MapWarpRegion *ColosseumAzartMatch_GetRegions(void) { return 0; }

const u32 *ColosseumAzartMatch_GetWarps(void) {
  return gColosseumAzartMatch_Warps;
}

const struct ObjectEventTemplate *
ColosseumAzartMatch_GetObjectEventTemplates(void) {
  return gColosseumAzartMatch_ObjectEventTemplates;
}

/// Advances the bridge animation and the metatile behaviors that make it
/// walkable.
static void ColosseumAzartMatch_UpdateBrokenBridge(void) {
  switch ((u32)gColosseumAzartMatch_Countdown) {
  case 66:
    MapGrid_CopyCellIndexRect(92, 31, 2, 2, 50, 38);
    MapGrid_CopyCellIndexRect(92, 31, 2, 2, 54, 38);
    Engine_ObjectEventSetAnimation(LOCALID_BRIDGE_OPERATOR, 10);
    break;
  case 60:
    MapGrid_CopyCellIndexRect(92, 33, 2, 2, 50, 38);
    MapGrid_CopyCellIndexRect(92, 33, 2, 2, 54, 38);
    MapGrid_CopyCellAttributes(50, 25, 6, 1, 50, 12);
    Engine_ObjectEventSetAnimation(LOCALID_BRIDGE_OPERATOR, 11);
    break;
  case 6:
    MapGrid_CopyCellIndexRect(92, 31, 2, 2, 50, 38);
    MapGrid_CopyCellIndexRect(92, 31, 2, 2, 54, 38);
    Engine_ObjectEventSetAnimation(LOCALID_BRIDGE_OPERATOR, 10);
    break;
  case 0:
    MapGrid_CopyCellIndexRect(92, 29, 2, 2, 50, 38);
    MapGrid_CopyCellIndexRect(92, 29, 2, 2, 54, 38);
    Engine_ObjectEventSetAnimation(LOCALID_BRIDGE_OPERATOR, 12);
    MapGrid_CopyCellAttributes(50, 24, 6, 1, 50, 12);
    gColosseumAzartMatch_Countdown = 120;
    break;
  }
  gColosseumAzartMatch_Countdown--;
}

/// Stops the bridge task and restores its closed frame.
static void ColosseumAzartMatch_ResetBrokenBridge(void) {
  MapTask task;

  gColosseumAzartMatch_Countdown = 0;
  task = ColosseumAzartMatch_UpdateBrokenBridge;
  Task_Remove(task);
  task();
}

/// Starts the bridge's repeating opening sequence.
static void ColosseumAzartMatch_StartBrokenBridge(void) {
  gColosseumAzartMatch_Countdown = 66;
  FieldScheduler_AddCallback(ColosseumAzartMatch_UpdateBrokenBridge,
                             TASK_PRIORITY_MAP);
}

/// Waits for the crossing window, bounded by one bridge cycle.
static void ColosseumAzartMatch_WaitForBridgeOpening(void) {
  s32 i;

  WaitFrames(10);
  i = 0;
  if (gColosseumAzartMatch_Countdown != 22) {
    do {
      WaitFrames(1);
      i++;
      if (i > 119) {
        break;
      }
    } while (gColosseumAzartMatch_Countdown != 22);
  }
}

/// Move on a rock in the Rock Challenge, and after each push: a rock resting on
/// the floor becomes a step (the second rock's column is kept in the byte of
/// flags from FLAG_SECOND_ROCK_COLUMN).
static void EventScript_SettleRocks(void) {

  struct ObjectEvent *objectEvent;
  s32 blocked;

  MapGrid_CopyCellAttributes(27, 13, 3, 1, 23, 12);
  objectEvent = ObjectEvent_GetById(LOCALID_ROCK_A);
  blocked =
      MapGrid_GetTerrainHeight(0, objectEvent->x.fixed, objectEvent->z.fixed);
  if (objectEvent->y.fixed == 0 && blocked == 0) {
    objectEvent->priorityFlags = OBJ_EVENT_PRIORITY_UNDERFOOT;
    objectEvent->motionFlags = 0;
    {
      s32 x = objectEvent->x.fixed >> 20;
      s32 y = objectEvent->z.fixed >> 20;

      MapGrid_CopyCellAttributes(14, 13, 1, 1, x, y);
    }
  }
  objectEvent = ObjectEvent_GetById(LOCALID_ROCK_B);
  {
    s32 x = objectEvent->x.fixed >> 20;

    FieldGameFlag_SetByte(FLAG_SECOND_ROCK_COLUMN, x);
  }
  {
    s32 x = objectEvent->x.fixed >> 20;
    s32 y = objectEvent->z.fixed >> 20;

    MapGrid_CopyCellAttributes(14, 13, 1, 1, x, y);
  }
}

/// Walking into a rock in the Rock Challenge pushes it one cell.
static void EventScript_PushRock(void) {

  ColosseumAzartMatch_PushObjectAhead();
  EventScript_SettleRocks();
}

/// Growth on the plant before the Broken Bridge: it grows and its cell becomes
/// walkable (FLAG_PLANT_GROWN keeps it grown).
static void EventScript_GrowPlant(void) {
  {

    MapGrid_CopyCellAttributes(47, 24, 1, 1, 47, 12);
  }
  GameFlag_Set(FLAG_PLANT_GROWN);
}

/// Douse on the Pipeworks' west pipe: the camera turns to the Pipeworks and the
/// two pipes move as it fills (FLAG_PIPES_MOVED).
static void EventScript_DousePipe(void) {
  struct ObjectEvent *objectEvent;
  s32 none;

  GameFlag_Set(FLAG_PIPES_MOVED);
  objectEvent = ObjectEvent_GetById(LOCALID_PIPE_WEST);
  FieldScript_Start();
  FieldCamera_SetSpeed(PIXELS(2), PIXELS(1) / 4);
  FieldCamera_MoveTo(PIXELS(600), -1, PIXELS(200), 1);
  ObjectDispatch_SelectAnimation(objectEvent, 3);
  FieldCamera_WaitForArrival();
  none = 0;
  objectEvent->motionFlags = none;
  objectEvent->acceleration = PIXELS(2) / 5;
  objectEvent->speed = PIXELS(4) / 5;
  Object_SetPosition(objectEvent, objectEvent->x.fixed, PIXELS(8),
                     objectEvent->z.fixed);
  objectEvent = ObjectEvent_Get(LOCALID_PIPE_EAST);
  objectEvent->motionFlags = none;
  objectEvent->acceleration = PIXELS(2) / 5;
  objectEvent->speed = PIXELS(4) / 5;
  Object_SetPosition(objectEvent, objectEvent->x.fixed, PIXELS(32),
                     objectEvent->z.fixed);
  Object_WaitForMove(objectEvent);
  FieldScript_WaitFrames(45);
  MapGrid_CopyCellAttributes(43, 12, 1, 1, 41, 12);
  FieldScript_End();
}

/// Moves the leader and a stage object together between cell centers.
static void ColosseumAzartMatch_MoveObjectEventPairByOffset(s32 localId, s32 dx,
                                                            s32 dz) {
  struct ObjectEvent *leader;
  struct ObjectEvent *objectEvent;
  s32 x;
  s32 z;

  leader = ObjectEvent_GetById(gSaveBlock.leaderLocalId);
  objectEvent = ObjectEvent_GetById(localId);
  FieldScript_Start();
  {
    x = ((leader->x.fixed + (dx << 16)) & 0xFFF00000) + PIXELS(8);
    z = ((leader->z.fixed + (dz << 16)) & 0xFFF00000) + PIXELS(8);

    leader->speed = PIXELS(1);
    leader->acceleration = PIXELS(1) / 2;
    Object_SetPosition(leader, x, leader->y.fixed, z);
  }
  ObjectDispatch_SelectAnimation(leader, 27);
  {
    x = ((objectEvent->x.fixed + (dx << 16)) & 0xFFF00000) + PIXELS(8);
    z = ((objectEvent->z.fixed + (dz << 16)) & 0xFFF00000) + PIXELS(8);

    objectEvent->speed = PIXELS(1);
    objectEvent->acceleration = PIXELS(1) / 2;
    Object_SetPosition(objectEvent, x, objectEvent->y.fixed, z);
  }
  if (dx < 0 || dz < 0) {
    ObjectDispatch_SelectAnimation(objectEvent, 4);
  } else {
    ObjectDispatch_SelectAnimation(objectEvent, 3);
  }
  Sound_PlaySong(SE_226);
  Object_WaitForMove(leader);
  ObjectDispatch_SelectAnimation(objectEvent, 2);
  Sound_PlaySong(SE_STOP_SLOT3);
  FieldScript_End();
}

/// The north-south log carries the leader to the other bank. It's the Lumber
/// Water Crossing Area's north-south log, and walking into it rolls it, with
/// the leader, across.
static void EventScript_RollLogNorthSouth(void) {
  struct ObjectEvent *objectEvent;
  s32 row;
  s32 offset;

  objectEvent = ObjectEvent_GetById(gSaveBlock.leaderLocalId);
  row = objectEvent->z.fixed >> 20;
  offset = -48;
  if (row <= 8) {
    offset = 48;
  }
  MapGrid_CopyCellAttributes(67, 8, 3, 1, 64, row);
  ColosseumAzartMatch_MoveObjectEventPairByOffset(LOCALID_LOG_NORTH_SOUTH, 0,
                                                  offset);
  objectEvent = ObjectEvent_GetById(LOCALID_LOG_NORTH_SOUTH);
  row = objectEvent->z.fixed >> 20;
  MapGrid_CopyCellAttributes(64, 24, 3, 1, 64, row);
}

/// The east-west log carries the leader the way the pad is held. It's the
/// Lumber Water Crossing Area's east-west log, and walking into it rolls it,
/// with the leader.
static void EventScript_RollLogEastWest(void) {
  struct ObjectEvent *objectEvent;
  s32 column;
  s32 offset;
  s32 direction;

  objectEvent = ObjectEvent_GetById(gSaveBlock.leaderLocalId);
  column = objectEvent->x.fixed >> 20;
  if ((gKeysHeld & KEY_LEFT) != 0) {
    direction = -1;
  }
  if ((gKeysHeld & KEY_RIGHT) != 0) {
    direction = 1;
  }
  objectEvent = ObjectEvent_GetById(LOCALID_LOG_NORTH_SOUTH);
  offset = objectEvent->z.fixed >> 20;
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
  MapGrid_CopyCellAttributes(72, 9, 1, 3, column, 9);
  ColosseumAzartMatch_MoveObjectEventPairByOffset(LOCALID_LOG_EAST_WEST, offset,
                                                  0);
  objectEvent = ObjectEvent_GetById(LOCALID_LOG_EAST_WEST);
  column = objectEvent->x.fixed >> 20;
  MapGrid_CopyCellAttributes(63, 25, 1, 3, column, 9);
}

/// Runs the five frames that show water entering the Pipeworks channel.
static inline void ColosseumAzartMatch_AnimatePipeworksWater(void) {
  MapGrid_CopyCells(96, 29, 1, 3, 34, 38);
  FieldScript_WaitFrames(3);
  MapGrid_CopyCells(97, 29, 1, 3, 34, 38);
  FieldScript_WaitFrames(3);
  MapGrid_CopyCells(98, 29, 1, 3, 34, 38);
  FieldScript_WaitFrames(3);
  MapGrid_CopyCells(99, 29, 1, 3, 34, 38);
  FieldScript_WaitFrames(3);
  MapGrid_CopyCells(100, 29, 1, 3, 34, 38);
}

/// Walking west into the Pipeworks' pillar pushes it: water runs into the
/// channel, and the first time the pipes move (FLAG_PIPEWORKS_PIPE_PUSHED,
/// FLAG_PIPES_MOVED).
static void EventScript_PushPipeworksPillar(void) {
  s32 leaderId;
  struct ObjectEvent *leader;
  struct ObjectEvent *objectEvent;
  s32 none;

  leaderId = gSaveBlock.leaderLocalId;
  leader = ObjectEvent_GetById(leaderId);
  objectEvent = ObjectEvent_GetById(LOCALID_PIPEWORKS_PIPE);
  GameFlag_Set(FLAG_PIPEWORKS_PIPE_PUSHED);
  FieldScript_Start();
  Engine_ObjectEventSetAnimation(leaderId, ANIM_PUSH);
  FieldScript_WaitFrames(6);
  objectEvent->speed = PIXELS(1) / 2;
  objectEvent->acceleration = PIXELS(1) / 5;
  Script_PlaySong(SE_HEAVY_MOVE);
  ObjectDispatch_SelectAnimation(objectEvent, 2);
  Object_SetPosition(objectEvent, objectEvent->x.fixed - PIXELS(48), 0,
                     objectEvent->z.fixed);
  FieldScript_WaitFrames(6);
  Engine_ObjectEventSetAnimation(leaderId, ANIM_WALK);
  ObjectDispatch_StartCamera(
      (struct DispatchObject *)((struct FieldWork *)AllocHeapSlotEwram(
                                    HEAP_SLOT_FIELD, sizeof(struct FieldWork)))
          ->camera,
      (s32)objectEvent);
  ObjectEvent_SetSpeed(leaderId, PIXELS(3) / 10, PIXELS(1) / 5);
  Object_SetPosition(leader, leader->x.fixed - PIXELS(24), 0, leader->z.fixed);
  ObjectMotion_CommitCurrentPositionAndActivate(leaderId);
  Engine_ObjectEventSetAnimation(leaderId, ANIM_STAND);
  Object_WaitForMove(objectEvent);
  ObjectDispatch_SelectAnimation(objectEvent, 1);
  Script_PlaySong(SE_STOP_SLOT3);
  Script_PlaySong(SE_HEAVY_STOP);
  FieldScript_WaitFrames(15);
  FieldScript_End();
  MapGrid_CopyCellAttributes(37, 7, 1, 4, 34, 7);
  MapGrid_CopyCellAttributes(36, 7, 1, 4, 37, 7);
  none = Script_GetFlag(FLAG_PIPES_MOVED);
  if (none != 0) {
    FieldScript_Start();
    FieldCamera_SetSpeed(PIXELS(2), PIXELS(1) / 4);
    FieldCamera_MoveTo(PIXELS(552), -1, PIXELS(200), 1);
    FieldCamera_WaitForArrival();
    ColosseumAzartMatch_AnimatePipeworksWater();
    FieldScript_WaitFrames(15);
    FieldScript_End();
    return;
  }
  GameFlag_Set(FLAG_PIPES_MOVED);
  FieldScript_Start();
  FieldCamera_SetSpeed(PIXELS(2), PIXELS(1) / 4);
  FieldCamera_MoveTo(PIXELS(600), -1, PIXELS(200), 1);
  FieldCamera_WaitForArrival();
  objectEvent = ObjectEvent_GetById(LOCALID_PIPE_WEST);
  objectEvent->motionFlags = none;
  objectEvent->acceleration = PIXELS(2) / 5;
  objectEvent->speed = PIXELS(4) / 5;
  Object_SetPosition(objectEvent, objectEvent->x.fixed, PIXELS(8),
                     objectEvent->z.fixed);
  ObjectDispatch_SelectAnimation(objectEvent, 3);
  ColosseumAzartMatch_AnimatePipeworksWater();
  objectEvent = ObjectEvent_GetById(LOCALID_PIPE_EAST);
  objectEvent->motionFlags = none;
  objectEvent->acceleration = PIXELS(2) / 5;
  objectEvent->speed = PIXELS(4) / 5;
  Object_SetPosition(objectEvent, objectEvent->x.fixed, PIXELS(32),
                     objectEvent->z.fixed);
  Object_WaitForMove(objectEvent);
  FieldScript_WaitFrames(15);
  FieldScript_End();
  MapGrid_CopyCellAttributes(43, 12, 1, 1, 41, 12);
}

/// Unused: sets FLAG_MAPSEC_TEMP_00; nothing calls it.
UNUSED static void ColosseumAzartMatch_SetCourseFlag(void) {
  GameFlag_Set(FLAG_MAPSEC_TEMP_00);
}

/// Reaching Colosseum Final 1's arena: the leader takes a prize (the Iron
/// Shield if first, the Padded Gloves if second), and he and Azart face off and
/// fight; the battle's group follows who took the shield.
/// The arena's coord event row, in all three finals stages, waits on
/// FLAG_MAP_TEMP_10 being clear, and nothing in the game sets it: no map
/// code, no object script (none uses SC_FLAG_ON) and none of the main image's
/// flag writers reaches it, so the arena is never closed.
static void EventScript_ReachArena(void) {
  s32 approach;
  s32 n;
  s32 base;

  base = 0;
  ColosseumAzartMatch_NoOp();
  FieldScript_Start();
  approach = ColosseumAzartMatch_CollectFinishItem(ITEM_IRON_SHIELD,
                                                   ITEM_PADDED_GLOVES);
  ColosseumAzartMatch_WaitForStageReady();
  n = 9;
  do {
    ObjectEvent_RefreshSelectorById(LOCALID_OPPONENT);
    n = n - 1;
  } while (n >= 0);
  ObjectEvent_SetSpeed(LOCALID_OPPONENT, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkTo(LOCALID_OPPONENT, 1320, 192);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 1288, 192);
  Engine_ObjectEventSetAnimation(LOCALID_OPPONENT, ANIM_STAND);
  ObjectEvent_LinkPair(LOCALID_PARTY_LEADER, LOCALID_OPPONENT, 0);
  FieldScript_WaitFrames(10);
  Engine_ObjectEventSetAnimation(LOCALID_OPPONENT, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
  FieldScript_WaitFrames(20);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(2), PIXELS(1));
  ObjectEvent_SetSpeed(LOCALID_OPPONENT, PIXELS(2), PIXELS(1));
  ObjectEvent_WalkTo(LOCALID_PARTY_LEADER, 1296, 192);
  ObjectEvent_WalkToAndWait(LOCALID_OPPONENT, 1312, 192);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_LEADER_STRIDE);
  Engine_ObjectEventSetAnimation(LOCALID_OPPONENT,
                                 WARRIOR_ANIMATION_FIGHTING_STANCE);
  FieldScript_WaitFrames(10);
  /* FAKEMATCH(temporary): base, set to 0 at the top, keeps the + 1 unfolded;
   * as 1 - approach, 1 of 106 instructions differ in tbs-en. */
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_COLOSSO,
                                        base - approach + 1);
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  FieldScript_SetNextDestination((s32)&MapId_ColosseumAzartMatch,
                                 COLOSSEUM_AZART_MATCH_AFTER_MATCH_WON);
  FieldScript_SetDefeatDestination((s32)&MapId_ColosseumAzartMatch,
                                   COLOSSEUM_AZART_MATCH_AFTER_MATCH_LOST);
  GameFlag_Set(FLAG_KEEP_MUSIC);
}

/// Walking into something in the Rock Challenge (coord event 90): a rock is
/// pushed, anything else is checked.
static void EventScript_PushRockOrCheckAhead(void) {

  if (Colosso_FindObjectEventAhead() == 0) {
    Field_TryJumpForward();
  } else {
    EventScript_PushRock();
  }
}

const struct MapEvent *ColosseumAzartMatch_GetEvents(void) {
  return gColosseumAzartMatch_Events;
}

/// Back from the battle (COLOSSEUM_AZART_MATCH_AFTER_MATCH_WON or _LOST): Isaac
/// and Azart stand in the arena as the result is shown.
static void ColosseumAzartMatch_RestoreMatchResult(s32 result) {

  ObjectEvent_DestroyById(LOCALID_PRIZE_A);
  ObjectEvent_DestroyById(LOCALID_PRIZE_B);
  Party_RestoreHpPp(1);
  FieldScript_Start();
  ObjectEvent_SetPosition(LOCALID_OPPONENT, PIXELS(1320), PIXELS(192));
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, PIXELS(1288), PIXELS(192));
  // BUG: 0x4000 is FACING_SOUTH, passed where ObjectEvent_FaceObjectEvent takes
  // a local id; ObjectEvent_GetByLocalId has no object event past 0xbf, so
  // neither turns.
#ifdef BUGFIX
  ObjectEvent_FaceDirection(LOCALID_OPPONENT, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 0);
#else
  ObjectEvent_FaceObjectEvent(LOCALID_OPPONENT, 0x4000, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, 0x4000, 0);
#endif
  if (result < 0) {
    Engine_ObjectEventSetAnimation(LOCALID_OPPONENT,
                                   WARRIOR_ANIMATION_ARMS_RAISED);
    Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_LEADER_CROUCH);
  } else {
    Engine_ObjectEventSetAnimation(LOCALID_OPPONENT,
                                   WARRIOR_ANIMATION_DEFEATED);
    Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER,
                                   ANIM_LEADER_ARMS_RAISED);
  }
  WaitFrames(1);
  FieldCamera_MoveTo(PIXELS(1304), 0, PIXELS(128), 0);
  ColosseumAzartMatch_ShowMatchResult(result);
  FieldScript_End();
}

/// Marks object events standing in the stages' finish terrain.
static void ColosseumAzartMatch_MarkFinishTerrain(s32 localId) {
  struct ObjectEvent *target;
  s32 column;
  s32 row;

  target = ObjectEvent_GetById(localId);
  if (target != 0) {
    column = target->x.fixed >> 19;
    row = target->z.fixed >> 19;
    if (column >= 94 && column <= 95 && row > 23 && row <= 26) {
      target->layer = 1;
    }
  }
}

/// Sets up Colosseum Final 1: restores the stages as they were left, then by
/// the arrival: the race (COLOSSEUM_AZART_MATCH_RACE_FROM_LEND_A_HAND), the
/// friends' turns to lend a hand (COLOSSEUM_AZART_MATCH_FROM_COLOSSEUM_WEST),
/// the guide's tour (COLOSSEUM_AZART_MATCH_FROM_COLOSSEUM), or the match won
/// (COLOSSEUM_AZART_MATCH_AFTER_MATCH_WON) or lost
/// (COLOSSEUM_AZART_MATCH_AFTER_MATCH_LOST).
s32 ColosseumAzartMatch_Initialize(void) {
  struct ObjectEvent *objectEvent;
  s32 hit;
  s32 x;
  u8 zero;
  s32 col;

  gFieldWork->transition = MAP_TRANSITION(TRANSITION_FADE, 0);
  GameFlag_Set(FLAG_RETREAT_BLOCKED);
  objectEvent = ObjectEvent_GetById(LOCALID_ROCK_A);
  hit = MapGrid_GetTerrainHeight(0, objectEvent->x.fixed, objectEvent->z.fixed);
  if (objectEvent->y.fixed == 0 && hit == 0) {
    objectEvent->priorityFlags = 2;
    objectEvent->motionFlags = hit;
    MapGrid_CopyCellAttributes(14, 13, 1, 1, objectEvent->x.fixed >> 20,
                               objectEvent->z.fixed >> 20);
  }
  x = FieldGameFlag_GetByte(FLAG_SECOND_ROCK_COLUMN);
  if (x == 0) {
    x = 25;
  }
  objectEvent = ObjectEvent_GetById(LOCALID_ROCK_B);
  objectEvent->x.fixed = (x << 20) + PIXELS(8);
  objectEvent->motionFlags = 0;
  objectEvent->priorityFlags = 2;
  MapGrid_CopyCellAttributes(14, 13, 1, 1, x, 12);
  FieldScheduler_AddCallback(ColosseumAzartMatch_UpdateBrokenBridge,
                             TASK_PRIORITY_MAP);
  objectEvent = ObjectEvent_GetById(LOCALID_GROWTH_PLANT);
  objectEvent->layer = 1;
  zero = 0;
  if (GameFlag_Get(FLAG_PLANT_GROWN)) {
    ObjectDispatch_SelectAnimation(objectEvent, 4);
    ObjectEvent_SetSpriteFlags(objectEvent, 0);
    objectEvent->collisionFlags = zero;
    objectEvent->priorityFlags = 3;
    MapGrid_CopyCellAttributes(47, 24, 1, 1, 47, 12);
  }
  objectEvent = ObjectEvent_GetById(LOCALID_LOG_NORTH_SOUTH);
  col = objectEvent->z.fixed >> 20;
  objectEvent->motionFlags = zero;
  objectEvent->priorityFlags = 2;
  MapGrid_CopyCellAttributes(64, 24, 3, 1, 64, col);
  objectEvent = ObjectEvent_GetById(LOCALID_LOG_EAST_WEST);
  col = objectEvent->x.fixed >> 20;
  objectEvent->motionFlags = zero;
  objectEvent->priorityFlags = 2;
  MapGrid_CopyCellAttributes(63, 25, 1, 3, col, 9);
  if (GameFlag_Get(FLAG_PIPEWORKS_PIPE_PUSHED)) {
    MapGrid_CopyCellAttributes(37, 7, 1, 4, 34, 7);
    MapGrid_CopyCellAttributes(36, 7, 1, 4, 37, 7);
    MapGrid_CopyCells(100, 29, 1, 3, 34, 38);
  }
  objectEvent = ObjectEvent_GetById(LOCALID_PIPE_WEST);
  if (GameFlag_Get(FLAG_PIPES_MOVED)) {
    MapGrid_CopyCellAttributes(43, 12, 1, 1, 41, 12);
    objectEvent->motionFlags = zero;
    objectEvent->acceleration = PIXELS(2) / 5;
    objectEvent->speed = PIXELS(4) / 5;
    objectEvent->y.fixed = PIXELS(8);
    ObjectDispatch_SelectAnimation(objectEvent, 3);
  } else {
    ObjectDispatch_SelectAnimation(objectEvent, 2);
  }
  ObjectEvent_GetById(LOCALID_PIPE_EAST)->priorityFlags = 2;
  Colosso_ShowItemIcon(LOCALID_PRIZE_A, ITEM_IRON_SHIELD);
  Colosso_ShowItemIcon(LOCALID_PRIZE_B, ITEM_PADDED_GLOVES);
  // Select competition, support, explanation, or post-battle arrival.
  switch (gSaveBlock.warpDest) {
  case COLOSSEUM_AZART_MATCH_RACE_FROM_LEND_A_HAND:
    Colosso_StartRaceGauge(LOCALID_PARTY_LEADER, LOCALID_OPPONENT, 4,
                           PIXELS(1304), PIXELS(192), LOCALID_PRIZE_A,
                           LOCALID_PRIZE_B);
    MapGrid_CopyCellsLayered(127, 0, 1, 2, 19, 2);
    ObjectEvent_DestroyById(LOCALID_GUIDE);
    ObjectEvent_DestroyById(LOCALID_ROCK_ATTENDANT);
    ObjectEvent_DestroyById(LOCALID_PIPEWORKS_ATTENDANT);
    ObjectEvent_DestroyById(LOCALID_BRIDGE_ATTENDANT);
    ObjectEvent_DestroyById(LOCALID_LUMBER_ATTENDANT);
    if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
      Sound_PlaySong(SONG_FADE_OUT_BGM);
      Colosso_ControlCharacterAlone(ISAAC);
      Colosso_PlaceFriendsAtCheeringSpots();
      ColosseumAzartMatch_MarkFinishTerrain(LOCALID_GARET);
      ColosseumAzartMatch_MarkFinishTerrain(LOCALID_IVAN);
      ColosseumAzartMatch_MarkFinishTerrain(LOCALID_MIA);
      ColosseumAzartMatch_ShowRoundIntroduction(1);
    }
    ObjectEvent_LinkObjectAndSetCallback(LOCALID_GARET, LOCALID_PARTY_LEADER);
    ObjectEvent_LinkObjectAndSetCallback(LOCALID_IVAN, LOCALID_PARTY_LEADER);
    ObjectEvent_LinkObjectAndSetCallback(LOCALID_MIA, LOCALID_PARTY_LEADER);
    Colosso_StartOpponent((s32)&ResourceId_OpponentPathA);
    break;
  case COLOSSEUM_AZART_MATCH_FROM_COLOSSEUM_WEST:
    FieldScheduler_AddCallback(Colosso_RaiseTakePlaceOnPsynergy,
                               TASK_PRIORITY_MAP);
    ObjectEvent_DestroyById(LOCALID_PRIZE_A);
    ObjectEvent_DestroyById(LOCALID_PRIZE_B);
    if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
      Colosso_PlaceFriendsAtCheeringSpots();
      Colosso_ControlCharacterAlone(GARET);
      ColosseumAzartMatch_ShowRoundIntroduction(0);
    }
    break;
  case COLOSSEUM_AZART_MATCH_FROM_COLOSSEUM:
    if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
      ColosseumAzartMatch_ExplainFinalsRules(LOCALID_GUIDE);
      Colosso_ClearCheeringSpots();
    }
    break;
  case COLOSSEUM_AZART_MATCH_AFTER_MATCH_WON:
    ColosseumAzartMatch_RestoreMatchResult(1);
    Engine_ScriptRequestWarp(COLOSSEUM_AZART_MATCH_TO_COLOSSEUM_EAST);
    break;
  case COLOSSEUM_AZART_MATCH_AFTER_MATCH_LOST:
    ColosseumAzartMatch_RestoreMatchResult(-1);
    Engine_ScriptRequestWarp(COLOSSEUM_AZART_MATCH_TO_BABI_PALACE);
    break;
  }
  return 0;
}

/// The tour of Colosseum Final 1 (COLOSSEUM_AZART_MATCH_FROM_COLOSSEUM): the
/// guide shows the party the site of the first finals match, the race through
/// the stages to the arena, the prizes there, and that the companions choose
/// where to cheer from.
static void ColosseumAzartMatch_ExplainFinalsRules(s32 guide) {
  struct ObjectEvent *objectEvent;
  s32 x;
  s32 z;

  objectEvent = ObjectEvent_GetById(guide);
  x = objectEvent->x.part.pixel;
  z = objectEvent->z.part.pixel;
  FieldScript_Start();
  ObjectEvent_SetSpeed(guide, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(LOCALID_GARET, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(LOCALID_IVAN, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(LOCALID_MIA, PIXELS(1), PIXELS(1) / 2);
  Engine_ObjectEventSetPosition(LOCALID_PARTY_LEADER, x << 16,
                                (z << 16) - PIXELS(48));
  ObjectEvent_SetPosition(LOCALID_GARET, (x << 16) - PIXELS(16),
                          (z << 16) - PIXELS(40));
  ObjectEvent_SetPosition(LOCALID_IVAN, (x << 16) + PIXELS(16),
                          (z << 16) - PIXELS(40));
  ObjectEvent_SetPosition(LOCALID_MIA, x << 16, (z << 16) - PIXELS(32));
  ObjectEvent_SetPosition(guide, x << 16, (z << 16) - PIXELS(80));
  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  objectEvent->facing = FACING_NORTH;
  FieldCamera_FollowObjectEvent(LOCALID_PARTY_LEADER, 0);
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldScript_SetMessageId(MsgColossoSiteFirstFinalsBattle);
  Script_ShowMessage(guide, 0);
  ObjectEvent_ShowEmote(LOCALID_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Script_ShowMessage(LOCALID_MIA, 0);
  ObjectMotion_SetVariantCallback(guide, 3);
  Script_ShowMessage(guide, 0);
  ObjectEvent_ShowEmote(LOCALID_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Script_ShowMessage(LOCALID_IVAN, 0);
  ObjectEvent_FaceObjectEvent(guide, LOCALID_IVAN, 0);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetAnimationAndWait(guide, ANIM_NOD);
  Script_ShowMessage(guide, 0);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Script_ShowMessage(LOCALID_GARET, 0);
  ObjectEvent_ShowEmote(LOCALID_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Script_ShowMessage(LOCALID_MIA, 0);
  ObjectEvent_ShowEmote(guide, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  // Pan over each stage while the guide explains the finals rules.
  if (Script_AskYesNo(guide, 0) == 0) {
    do {
      FieldScript_SetMessageId(MsgColossoWarriorsEnterFinalsWithoutAny);
      Engine_ObjectEventSetAnimation(LOCALID_IVAN, ANIM_NOD);
      FieldScript_WaitFrames(2);
      Engine_ObjectEventSetAnimation(LOCALID_GARET, ANIM_NOD);
      FieldScript_WaitFrames(2);
      Engine_ObjectEventSetAnimation(LOCALID_MIA, ANIM_NOD);
      FieldScript_WaitFrames(1);
      ObjectMotion_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
      ObjectMotion_SetAnimationAndWait(guide, ANIM_NOD);
      Script_ShowMessage(guide, 0);
      ObjectEvent_FaceDirection(guide, FACING_NORTHWEST, 0);
      FieldScript_WaitFrames(20);
      Script_ShowMessage(guide, 0);
      FieldCamera_SetSpeed(PIXELS(3), PIXELS(3) / 8);
      FieldCamera_MoveTo(PIXELS(312), -1, PIXELS(104), 1);
      FieldCamera_WaitForArrival();
      Script_ShowMessage(guide, 0);
      FieldCamera_SetSpeed(PIXELS(3) / 2, PIXELS(3) / 16);
      FieldCamera_MoveTo(PIXELS(776), -1, PIXELS(104), 1);
      Script_ShowMessage(guide, 0);
      FieldCamera_WaitForArrival();
      Script_ShowMessage(guide, 0);
      FieldCamera_SetSpeed(PIXELS(3), PIXELS(3) / 8);
      FieldCamera_MoveTo(PIXELS(1240), -1, PIXELS(168), 1);
      FieldCamera_WaitForArrival();
      ObjectEvent_FaceObjectEvent(guide, 0x6000, 0);
      Script_ShowMessage(guide, 0);
      FieldCamera_MoveTo(PIXELS(1304), -1, PIXELS(168), 1);
      FieldCamera_WaitForArrival();
      ObjectEvent_FaceObjectEvent(guide, LOCALID_PARTY_LEADER, 0);
      Script_ShowMessage(guide, 0);
      Script_ShowMessage(guide, 0);
      Script_ShowMessage(guide, 0);
      FieldCamera_FollowObjectEvent(LOCALID_PARTY_LEADER, 0);
      ObjectMotion_SetVarCbAndRefresh(guide, 2);
    } while (Script_AskYesNo(guide, 0) != 0);
    ObjectMotion_SetVarCbAndRefresh(guide, 2);
    FieldScript_SetMessageId(MsgColossoAskAttendantsForExplanationsStages);
    Script_ShowMessage(guide, 0);
  }
  // Explain the party's support role, then gather them behind Isaac.
  FieldScript_SetMessageId(MsgColossoIsaacYoureContestantInFinals);
  ObjectMotion_SetVarCbAndRefresh(guide, 2);
  Script_ShowMessage(guide, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_EAST, 0);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_WEST, 0);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 2);
  Script_ShowMessage(LOCALID_GARET, 0);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_IVAN, 2);
  Script_ShowMessage(LOCALID_IVAN, 0);
  ObjectMotion_SetAnimationAndWait(LOCALID_MIA, ANIM_NOD);
  Script_ShowMessage(LOCALID_MIA, 0);
  Engine_ObjectEventSetAnimation(LOCALID_MIA, ANIM_NOD);
  FieldScript_WaitFrames(1);
  Engine_ObjectEventSetAnimation(LOCALID_GARET, ANIM_NOD);
  FieldScript_WaitFrames(2);
  Engine_ObjectEventSetAnimation(LOCALID_IVAN, ANIM_NOD);
  FieldScript_WaitFrames(1);
  ObjectMotion_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
  FieldScript_WaitFrames(6);
  Engine_ObjectEventSetAnimation(LOCALID_GARET, ANIM_WALK);
  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (objectEvent != 0) {
    ObjectEvent_SetDestination(LOCALID_GARET, objectEvent->x.part.pixel,
                               objectEvent->z.part.pixel);
  }
  Engine_ObjectEventSetAnimation(LOCALID_IVAN, ANIM_WALK);
  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (objectEvent != 0) {
    ObjectEvent_SetDestination(LOCALID_IVAN, objectEvent->x.part.pixel,
                               objectEvent->z.part.pixel);
  }
  Engine_ObjectEventSetAnimation(LOCALID_MIA, ANIM_WALK);
  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (objectEvent != 0) {
    ObjectEvent_SetDestination(LOCALID_MIA, objectEvent->x.part.pixel,
                               objectEvent->z.part.pixel);
  }
  ObjectEvent_WalkToAndWait(guide, x - 16, z - 64);
  ObjectEvent_SetPosition(LOCALID_GARET, 0, 0);
  ObjectEvent_SetPosition(LOCALID_IVAN, 0, 0);
  ObjectEvent_SetPosition(LOCALID_MIA, 0, 0);
  ObjectEvent_WalkToAndWait(guide, x - 16, z - 16);
  ObjectEvent_WalkToAndWait(guide, x, z);
  ObjectEvent_FaceDirection(guide, FACING_NORTH, 10);
  FieldScript_End();
}

/// The Rock Challenge's attendant, stage 1 of Colosseum Final 1: the first
/// time, Isaac appears on the stage and pushes the rocks into steps; then the
/// objective, and a place for a friend to cheer. With a friend alone on the
/// stages (COLOSSEUM_AZART_MATCH_FROM_COLOSSEUM_WEST) he asks whether they are
/// done cheering instead.
static void EventScript_TalkToRockAttendant(s32 localId) {
  s32 path;

  if (gSaveBlock.warpDest == COLOSSEUM_AZART_MATCH_FROM_COLOSSEUM_WEST) {
    Colosso_AskDoneCheering(localId);
  } else {
    FieldScript_Start();
    path = Colosso_GreetAtStage(localId, 1);
    if (path == COLOSSO_GUIDE_DESCRIBE) {
      FieldScript_SetMessageId(MsgColossoCallRockChallenge);
      FieldCamera_SetSpeed(PIXELS(3), PIXELS(3) / 8);
      FieldCamera_MoveTo(PIXELS(328), -1, PIXELS(168), 1);
      FieldCamera_WaitForArrival();
      FieldScript_ShowMessageAndWait(localId, 0);
      Colosso_FadeInWarrior(LOCALID_PARTY_LEADER, 280, 200);
      ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
      ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 360, 200);
      FieldScript_WaitFrames(30);
      ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE,
                            60);
      FieldScript_ShowMessageAndWait(localId, 0);
      ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 312, 200);
      FieldScript_WaitFrames(30);
      ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 10);
      ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER,
                            EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 60);
      ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(3) / 2, PIXELS(3) / 4);
      Colosso_WalkObjectEventToAndStand(LOCALID_PARTY_LEADER, 296, 184);
      Colosso_WalkObjectEventToAndStand(LOCALID_PARTY_LEADER, 296, 152);
      Colosso_WalkObjectEventToAndStand(LOCALID_PARTY_LEADER, 312, 152);
      ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 15);
      ColosseumAzartMatch_PushObjectAhead();
      ObjectEvent_ResetMotionFields(LOCALID_PARTY_LEADER);
      ColosseumAzartMatch_PushObjectAhead();
      ObjectEvent_ResetMotionFields(LOCALID_PARTY_LEADER);
      ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(3) / 2, PIXELS(3) / 4);
      ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 304, 184);
      ObjectMotion_SetPositionAndReset(LOCALID_PARTY_LEADER, 296, 192);
      ObjectMotion_SetPositionAndReset(LOCALID_PARTY_LEADER, 296, 200);
      ObjectMotion_ArmCallback(LOCALID_PARTY_LEADER, FACING_EAST, 15);
      ColosseumAzartMatch_PushObjectAhead();
      ObjectEvent_ResetMotionFields(LOCALID_PARTY_LEADER);
      ColosseumAzartMatch_PushObjectAhead();
      ObjectEvent_ResetMotionFields(LOCALID_PARTY_LEADER);
      Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_STAND);
      FieldScript_ShowMessageAndWait(localId, 0);
      Colosso_RestoreWarrior(LOCALID_PARTY_LEADER);
      FieldCamera_FollowObjectEvent(LOCALID_PARTY_LEADER, 0);
      ObjectEvent_SetPosition(LOCALID_ROCK_A, PIXELS(312), PIXELS(168));
      Colosso_RepeatStageNumber(localId, 1);
    } else if (path == COLOSSO_GUIDE_OFFER_CHEERING) {
      FieldScript_SetMessageId(MsgColossoObjectiveStageClear);
      FieldScript_ShowMessageAndWait(localId, 0);
    }
    Colosso_OfferCheeringSpot(path, localId, 1);
    FieldScript_End();
  }
}

/// Moves a Pipeworks object event with the speed used throughout its
/// demonstration.
static inline void
ColosseumAzartMatch_SetPipeHeight(struct ObjectEvent *objectEvent, s32 height) {
  objectEvent->motionFlags = 0;
  objectEvent->acceleration = PIXELS(2) / 5;
  objectEvent->speed = PIXELS(4) / 5;
  Object_SetPosition(objectEvent, objectEvent->x.fixed, height,
                     objectEvent->z.fixed);
}

/// The Pipeworks' attendant, stage 2 of Colosseum Final 1: the first time, a
/// pointer shows how the pipes rise and fall like scales; then the objective,
/// and a place for a friend to cheer.
static void EventScript_TalkToPipeworksAttendant(s32 attendant) {
  s32 path;
  struct ObjectEvent *second;
  struct ObjectEvent *first;

  if (gSaveBlock.warpDest == COLOSSEUM_AZART_MATCH_FROM_COLOSSEUM_WEST) {
    Colosso_AskDoneCheering(attendant);
  } else {
    FieldScript_Start();
    path = Colosso_GreetAtStage(attendant, 2);
    if (path == COLOSSO_GUIDE_DESCRIBE) {
      FieldScript_SetMessageId(MsgColossoAreaCalledPipeworks);
      FieldCamera_SetSpeedParameters(PIXELS(3), PIXELS(3) / 8);
      FieldCamera_MoveTo(PIXELS(592), -1, PIXELS(120), 1);
      FieldCamera_WaitForArrival();
      FieldScript_WaitFrames(60);
      FieldCamera_SetSpeedParameters(PIXELS(3) / 2, PIXELS(3) / 16);
      FieldCamera_MoveWithinMapBounds(PIXELS(608), -1, PIXELS(216), 1);
      FieldCamera_WaitForArrival();
      FieldScript_ShowMessageAndWait(attendant, 0);
      Colosso_ShowPointer(56, 64, 0);
      FieldScript_WaitFrames(60);
      Colosso_MovePointer(160, 96, 10);
      FieldScript_WaitFrames(70);
      FieldScript_ShowMessageAndWait(attendant, 0);
      Colosso_HidePointer();
      WaitFrames(2);
      first = ObjectEvent_GetById(LOCALID_PIPE_WEST);
      ColosseumAzartMatch_SetPipeHeight(first, PIXELS(8));
      second = ObjectEvent_GetById(LOCALID_PIPE_EAST);
      ColosseumAzartMatch_SetPipeHeight(second, PIXELS(32));
      Object_WaitForMove(second);
      FieldScript_WaitFrames(45);
      first = ObjectEvent_GetById(LOCALID_PIPE_WEST);
      ColosseumAzartMatch_SetPipeHeight(first, PIXELS(24));
      second = ObjectEvent_Get(LOCALID_PIPE_EAST);
      ColosseumAzartMatch_SetPipeHeight(second, 0);
      Object_WaitForMove(second);
      FieldScript_WaitFrames(15);
      FieldScript_ShowMessageAndWait(attendant, 0);
      Colosso_ShowPointer(56, 64, 0);
      FieldScript_WaitFrames(30);
      Colosso_MovePointer(160, 96, 10);
      FieldScript_WaitFrames(40);
      Colosso_MovePointer(56, 64, 10);
      FieldScript_WaitFrames(70);
      FieldScript_ShowMessageAndWait(attendant, 0);
      Colosso_HidePointer();
      WaitFrames(2);
      FieldCamera_FollowObjectEvent(LOCALID_PARTY_LEADER, 0);
      Colosso_RepeatStageNumber(attendant, 2);
    } else if (path == COLOSSO_GUIDE_OFFER_CHEERING) {
      FieldScript_SetMessageId(MsgColossoObjectiveMakeGood);
      FieldScript_ShowMessageAndWait(attendant, 0);
    }
    Colosso_OfferCheeringSpot(path, attendant, 2);
    FieldScript_End();
  }
}

/// The Broken Bridge's attendant, stage 3 of Colosseum Final 1: the first time,
/// Isaac appears and times his crossing; then that the bridge's operator will
/// cheer for him, and a place for a friend to cheer.
static void EventScript_TalkToBridgeAttendant(s32 localId) {
  s32 path;

  if (gSaveBlock.warpDest == COLOSSEUM_AZART_MATCH_FROM_COLOSSEUM_WEST) {
    Colosso_AskDoneCheering(localId);
  } else {
    FieldScript_Start();
    path = Colosso_GreetAtStage(localId, 3);
    if (path == COLOSSO_GUIDE_DESCRIBE) {
      FieldScript_SetMessageId(MsgColossoTheyCallBrokenBridge);
      ColosseumAzartMatch_ResetBrokenBridge();
      FieldCamera_SetSpeed(PIXELS(3), PIXELS(3) / 8);
      FieldCamera_MoveTo(PIXELS(840), -1, PIXELS(216), 1);
      FieldCamera_WaitForArrival();
      Script_ShowMessage(localId, 0);
      ColosseumAzartMatch_StartBrokenBridge();
      FieldScript_WaitFrames(60);
      Script_ShowMessage(localId, 0);
      Colosso_FadeInWarrior(LOCALID_PARTY_LEADER, 736, 200);
      /* FAKEMATCH(wrapper): called through a cast to the s32 call the bytes
       * show, which sets r0 last; as a direct call, 1 of 99 instructions
       * differ in tbs-en. */
      COLOSSEUM_AZART_MATCH_FACE_DIRECTION(LOCALID_PARTY_LEADER, 0, 0);
      ColosseumAzartMatch_WaitForBridgeOpening();
      ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
      ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 816, 200);
      FieldScript_WaitFrames(30);
      ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER,
                            EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
      Script_ShowMessage(localId, 0);
      Colosso_RestoreWarrior(LOCALID_PARTY_LEADER);
      FieldCamera_FollowObjectEvent(LOCALID_PARTY_LEADER, 0);
      Colosso_RepeatStageNumber(localId, 3);
    } else if (path == COLOSSO_GUIDE_OFFER_CHEERING) {
      FieldScript_SetMessageId(MsgColossoOperatorBridgeWillAlsoCheer);
      Script_ShowMessage(localId, 0);
    }
    Colosso_OfferCheeringSpot(path, localId, 3);
    FieldScript_End();
  }
}

/// The Lumber Water Crossing Area's attendant, stage 4 of Colosseum Final 1:
/// the first time, a pointer shows the water tank and Isaac appears and rolls a
/// log across; then the objective, and a place for a friend to cheer.
static void EventScript_TalkToLumberAttendant(s32 localId) {
  s32 path;

  if (gSaveBlock.warpDest == COLOSSEUM_AZART_MATCH_FROM_COLOSSEUM_WEST) {
    Colosso_AskDoneCheering(localId);
    return;
  }
  FieldScript_Start();
  path = Colosso_GreetAtStage(localId, 4);
  if (path == COLOSSO_GUIDE_DESCRIBE) {
    FieldScript_SetMessageId(MsgColossoPlaceNormallyCalledLumberWater);
    FieldCamera_SetSpeed(PIXELS(3), PIXELS(3) / 8);
    FieldCamera_MoveTo(PIXELS(1088), -1, PIXELS(168), 1);
    FieldCamera_WaitForArrival();
    Script_ShowMessage(localId, 0);
    Colosso_ShowPointer(120, 72, 0);
    Script_ShowMessage(localId, 0);
    Colosso_HidePointer();
    FieldScript_WaitFrames(15);
    Colosso_FadeInWarrior(LOCALID_PARTY_LEADER, 984, 200);
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_EAST, 10);
    Script_ShowMessage(localId, 0);
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 30);
    ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER,
                          EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 60);
    ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(3) / 2, PIXELS(3) / 4);
    Colosso_WalkObjectEventToAndStand(LOCALID_PARTY_LEADER, 1000, 192);
    Colosso_WalkObjectEventToAndStand(LOCALID_PARTY_LEADER, 1000, 176);
    Colosso_WalkObjectEventToAndStand(LOCALID_PARTY_LEADER, 1016, 168);
    FieldScript_WaitFrames(15);
    /* FAKEMATCH(wrapper): called through a cast to the s32 call the bytes
     * show, which sets r0 last; as a direct call, 1 of 167 instructions differ
     * in tbs-en. */
    COLOSSEUM_AZART_MATCH_PLACE_STAGED_PAIR(LOCALID_LOG_EAST_WEST, 160, 0);
    FieldCamera_MoveTo(PIXELS(1088), -1, PIXELS(168), 1);
    Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_STAND);
    FieldScript_WaitFrames(10);
    ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
    ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 1192, 168);
    FieldScript_WaitFrames(10);
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_WEST, 30);
    ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE,
                          60);
    Script_ShowMessage(localId, 0);
    Colosso_RestoreWarrior(LOCALID_PARTY_LEADER);
    FieldCamera_FollowObjectEvent(LOCALID_PARTY_LEADER, 0);
    ObjectEvent_SetPosition(LOCALID_LOG_EAST_WEST, PIXELS(1016), PIXELS(168));
    Colosso_RepeatStageNumber(localId, 4);
  } else if (path == COLOSSO_GUIDE_OFFER_CHEERING) {
    FieldScript_SetMessageId(MsgColossoLogsKeyClearingStage);
    Script_ShowMessage(localId, 0);
  }
  Colosso_OfferCheeringSpot(path, localId, 4);
  FieldScript_End();
}

/// Sets the leader's movement speed for the shared Colosso dialogue.
static inline void InitializeObjectEventZero(void) {
  ObjectMotion_SetSpeedParameters(LOCALID_PARTY_LEADER, PIXELS(1),
                                  PIXELS(1) / 2);
}

/// Sets an attendant's movement speed for the shared Colosso dialogue.
static inline void InitializeSelectedObjectEvent(s32 localId) {
  ObjectMotion_SetSpeedParameters(localId, PIXELS(1), PIXELS(1) / 2);
}

/// Advances to the next message in the current explanation.
static __inline__ void AdvanceMessage(s32 amount) {
  gFieldWork->message += amount;
}

#include "common/colosso_solo.h"
#include "common/item_equip.h"
#include "common/colosso_task.h"

/// Selects a presentation script and resets its playback state.
static void ColosseumAzartMatch_SelectPresentationScript(u32 mode, u32 param) {
  const s16 *script;

  gColosso_LetteringMode = mode;
  gColosso_LetteringParameter = (param << 4);

  FieldScheduler_AddCallback(Colosso_UpdateLettering, TASK_PRIORITY_MAP);

  script = gColosseumAzartMatch_LetteringTitle;
  if (mode == COLOSSO_SHAPE_DIGIT) {
    script = gColosseumAzartMatch_LetteringDigit;
  }
  if (mode == COLOSSO_SHAPE_GO) {
    script = gColosseumAzartMatch_LetteringGo;
  }
  if (mode == COLOSSO_SHAPE_RESULT) {
    if (param != 0) {
      script = gColosseumAzartMatch_LetteringLose;
    } else {
      script = gColosseumAzartMatch_LetteringWon;
    }
  }

  gColosso_LetteringTimer = 0;
  gColosso_LetteringScript = (s16 *)script;
  gColosso_ModeMoveTarget = 0;
  gColosso_ModeMoveDuration = 0;
  gColosso_LetteringPosition = 0;
}

/// The lettering a visit opens with: `Lend a Hand` for the friends' turns
/// (round 0), or for the race `Match 1`, then 3, 2, 1 and `Go!`.
static void ColosseumAzartMatch_ShowRoundIntroduction(s32 round) {
  if (round == 0) {
    FieldScript_Start();
    FieldScript_StartScreenTransition();
    FieldScript_WaitTransitionFrames();
    FieldScript_WaitFrames(30);
    Sound_PlaySong(MUS_LEVEL_UP);
    Colosso_LoadLettering(COLOSSO_PICTURE_LEND_A_HAND);
    ColosseumAzartMatch_SelectPresentationScript(COLOSSO_SHAPE_TITLE, 0);
    FieldScript_WaitFrames(120);
    FieldScript_End();
  } else {
    Sound_PlaySong(SE_247);
    FieldScript_Start();
    FieldScript_StartScreenTransition();
    FieldScript_WaitTransitionFrames();
    gColosseumAzartMatch_LetteringTitle[15] = round * 60;
    FieldScript_WaitFrames(30);
    Sound_PlaySong(round + 90);
    Colosso_LoadLettering(round);
    ColosseumAzartMatch_SelectPresentationScript(COLOSSO_SHAPE_TITLE, 0);
    FieldScript_WaitFrames(120);
    while (Sound_GetBgmRestoreDelay() != 0) {
      WaitFrames(1);
    }
    Sound_PlaySong(SONG_FADE_OUT_ACTION_SE);
    Colosso_LoadLettering(COLOSSO_PICTURE_COUNT_3_2);
    ColosseumAzartMatch_SelectPresentationScript(COLOSSO_SHAPE_DIGIT, 0);
    Sound_PlaySong(SE_COLOSSO_COUNTDOWN);
    FieldScript_WaitFrames(60);
    ColosseumAzartMatch_SelectPresentationScript(COLOSSO_SHAPE_DIGIT, 1);
    Sound_PlaySong(SE_COLOSSO_COUNTDOWN);
    FieldScript_WaitFrames(60);
    Colosso_LoadLettering(COLOSSO_PICTURE_COUNT_1);
    ColosseumAzartMatch_SelectPresentationScript(COLOSSO_SHAPE_DIGIT, 0);
    Sound_PlaySong(SE_COLOSSO_COUNTDOWN);
    FieldScript_WaitFrames(60);
    Colosso_LoadLettering(COLOSSO_PICTURE_GO);
    ColosseumAzartMatch_SelectPresentationScript(COLOSSO_SHAPE_GO, 0);
    Sound_PlaySong(SE_COLOSSO_GO);
    Map_PlayMusic();
    FieldScript_End();
    GameFlag_Set(FLAG_IN_TOWN);
  }
}

/// The result: `Won` or `Lose` in large lettering with its fanfare, then the
/// screen closes.
static void ColosseumAzartMatch_ShowMatchResult(s32 result) {

  s32 loser;

  Sound_PlaySong(SE_247);
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  gColosseumAzartMatch_LetteringWon[13] = result * 60;
  STORE_CONST(gColosseumAzartMatch_LetteringLose[13]) =
      (result < 0 ? -result : result) * 60;
  if (result < 0) {
    FieldScript_WaitFrames(30);
    Sound_PlaySong(MUS_086);
    Colosso_LoadLettering(COLOSSO_PICTURE_WON_LOSE_FOR_LOSS);
    /* FAKEMATCH(wrapper): called through a cast to the s32 call the bytes
     * show, which sets r1 before r0; as a direct call, 1 of 72 instructions
     * differ in tbs-en. */
    COLOSSEUM_AZART_MATCH_SELECT_MODE_BLOCK(COLOSSO_SHAPE_RESULT, 1);
    FieldScript_WaitFrames(-result * 60 + 60);
    loser = LOCALID_PARTY_LEADER;
  } else {
    FieldScript_WaitFrames(30);
    Sound_PlaySong(result + 90);
    Colosso_LoadLettering(COLOSSO_PICTURE_WON_LOSE);
    /* FAKEMATCH(wrapper): the same cast as the first arm, which sets r1 before
     * r0; as a direct call, 1 of 72 instructions differ in tbs-en. */
    COLOSSEUM_AZART_MATCH_SELECT_MODE_BLOCK(COLOSSO_SHAPE_RESULT, 0);
    FieldScript_WaitFrames(result * 60 + 60);
    loser = LOCALID_OPPONENT;
  }
  FieldEffect_SpawnLinked(loser, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  while (Sound_GetBgmRestoreDelay() != 0) {
    WaitFrames(1);
  }
  Sound_PlaySong(MUS_SILENCE);
  FieldScript_WaitFrames(30);
  Sound_PlaySong(SONG_FADE_OUT_ACTION_SE);
  FieldScript_FinishScreenTransition();
  FieldScript_WaitTransitionFrames();
}

#include "common/colosso_marker.h"
#include "common/colosso_item_icon.h"

/// Unused: writes eight hexadecimal digits and a terminating null; nothing
/// calls it.
UNUSED static void String_WriteU32AsHex(u8 *buf, u32 value) {
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

static void ColosseumAzartMatch_NoOp(void) {}

/// Unused: marks the shared stage state ready for the waiting script; nothing
/// calls it.
UNUSED static void ColosseumAzartMatch_MarkStageReady(void) {
  s16 *ready = (s16 *)gMapLocalState;
  u16 nine = 9;

  *ready = nine;
}

/// Waits until the stage's setup task reports readiness.
static void ColosseumAzartMatch_WaitForStageReady(void) {
  s16 *ready = (s16 *)gMapLocalState;

  while (*ready != 9) {
    WaitFrames(1);
  }
}

/// Occasionally spawns an animated effect around the finish item.
static void
ColosseumAzartMatch_SpawnRandomEffect(union FieldObject *fieldObject) {
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)fieldObject;
  s32 position[3];
  u32 chance;

  if (objectEvent->velocityY >= -255 && objectEvent->velocityY <= 255) {
    objectEvent->motionFlags = 0;
  }
  chance = Engine_RandomNext();
  if (chance * 100 >> 16 <= 9) {
    struct ObjectEvent *effect;
    s32 radius;
    s32 angle;

    SET_VECTOR(position, objectEvent->x.fixed, objectEvent->y.fixed,
               objectEvent->z.fixed);
    radius = Engine_RandomNext();
    angle = Engine_RandomNext();
    Vector_AddPolarOffset(radius << 4, angle, position);
    {
      s32 x = position[0];
      s32 y = position[1];
      s32 z = position[2];

      effect = ObjectEvent_Create(SPRITE_SPARKLE, x, y, z);
    }
    if (effect != 0) {
      effect->motionFlags = 0;
      ObjectEvent_SetSpriteFlags(effect, 0);
      ObjectEvent_SetScript(effect, gColosseumAzartMatch_ScriptA);
      ObjectDispatch_SelectAnimation(effect, 1);
      ObjectDispatch_SelectAnimation(effect, 0);
    }
  }
}

/// Presents the collected item above its collector and signals completion.
static s32 ColosseumAzartMatch_PlaceFinishItemAboveObjectEvent(
    struct ObjectScriptContext *interpreter) {
  struct ObjectEvent *collector = (struct ObjectEvent *)interpreter;
  struct ObjectEvent *itemObjectEvent = ObjectEvent_GetById(collector->timer);

  Object_SetPosition(itemObjectEvent, collector->x.fixed,
                     collector->y.fixed + PIXELS(36), collector->z.fixed);
  itemObjectEvent->motionFlags = 0;
  ObjectEvent_SetScript(itemObjectEvent, gColosseumAzartMatch_ScriptB);
  Sound_PlaySong(MUS_TREASURE_FOUND);
  collector->timer = 0;
  return 0;
}

/// Moves to the selected finish item and presents it to the party.
static s32 ColosseumAzartMatch_CollectFinishItem(s32 firstItem,
                                                 s32 secondItem) {

  struct ColossoGaugeWork *gauge = (struct ColossoGaugeWork *)gColossoWork;
  struct ObjectEvent *objectEvent;
  s32 flag;
  s32 x;
  s32 z;
  u16 *itemLocalId;
  s16 *pendingItemLocalId;

  flag = GameFlag_Get(FLAG_ARENA_REACHED);

  objectEvent = ObjectEvent_GetById(gSaveBlock.leaderLocalId);

  if (gauge->finishX < objectEvent->x.fixed) {
    x = gauge->finishX + PIXELS(12);
  } else {
    x = gauge->finishX - PIXELS(12);
  }

  if (flag != 0) {
    z = gauge->finishZ + PIXELS(16);
    itemLocalId = &gauge->finishItemLocalIdB;
  } else {
    z = gauge->finishZ - PIXELS(16);
    itemLocalId = &gauge->finishItemLocalIdA;
  }

  pendingItemLocalId = &objectEvent->timer;
  *pendingItemLocalId = *itemLocalId;
  objectEvent->acceleration = PIXELS(1) / 4;
  objectEvent->speed = PIXELS(1);

  Object_SetPosition(objectEvent, x, 0, z);
  GameFlag_Set(FLAG_ARENA_REACHED);
  ObjectEvent_SetScript(objectEvent, gColosseumAzartMatch_ApproachScript);

  while (*pendingItemLocalId != 0) {
    WaitFrames(1);
  }

  if (flag == 0) {
    /* FAKEMATCH(wrapper): called through a cast to the s32 call the bytes
     * show, which sets r1 before r0; as a direct call, 1 of 110 instructions
     * differ in tbs-en. */
    COLOSSEUM_AZART_MATCH_APPLY_MATCHING_SLOT_VALUES(ISAAC, firstItem);
    MessageArgument_Push(firstItem, 2);
  } else {
    /* FAKEMATCH(wrapper): the same cast as the first arm, which sets r1 before
     * r0; as a direct call, 1 of 110 instructions differ in tbs-en. */
    COLOSSEUM_AZART_MATCH_APPLY_MATCHING_SLOT_VALUES(ISAAC, secondItem);
    MessageArgument_Push(secondItem, 2);
  }

  MessageArgument_Push(gSaveBlock.leaderLocalId, 1);
  FieldMessage_Show(MsgColossoIsaacGotItem, 3);
  ObjectDispatch_WaitForValue16(objectEvent);

  return flag;
}

#include "common/colosso_path_opponent.h"
#include "common/colosso_setup.h"
#include "common/colosso_map_state.h"
#include "common/object_findslot.h"

/// Checks the next cell and pushes its object one step if clear.
static void ColosseumAzartMatch_PushObjectAhead(void) {

  struct ObjectEvent *subject;
  struct ObjectEvent *target;
  struct ObjectEvent *blocker;
  u32 packedStep;
  u32 direction;
  struct FieldPosition position;
  s32 zero;
  s32 localId;

  localId = gSaveBlock.leaderLocalId;
  subject = ObjectEvent_GetById(localId);

  direction = subject->facing >> 12;

  packedStep = gColosseumAzartMatch_DirectionSteps[direction];
  position.x = subject->x.fixed + (packedStep & 0xffff0000);
  position.y = subject->y.fixed;
  packedStep <<= 16;
  position.z = subject->z.fixed + packedStep;

  target = MapData_FindSlotAtPosition(&position, subject);
  if (target == 0) {
    return;
  }

  packedStep = gColosseumAzartMatch_DirectionSteps[direction];
  position.x = target->x.fixed + (packedStep & 0xffff0000);
  position.y = target->y.fixed;
  packedStep <<= 16;
  position.z = target->z.fixed + packedStep;

  blocker = MapData_FindSlotAtPosition(&position, target);
  if (blocker != 0 && (blocker->collisionFlags & 1) != 0) {
    return;
  }

  position.x = target->x.fixed;
  position.y = target->y.fixed + PIXELS(16);
  position.z = target->z.fixed;

  blocker = MapData_FindSlotAtPosition(&position, target);
  if (blocker != 0 && (blocker->collisionFlags & 1) != 0) {
    return;
  }

  target->layer = 2;
  zero = 0;

  packedStep = gColosseumAzartMatch_DirectionSteps[direction];
  position.x = target->x.fixed + (packedStep & 0xffff0000);
  position.y = target->y.fixed;
  packedStep <<= 16;
  position.z = target->z.fixed + packedStep;

  if (FieldObject_CheckMovementCollision(target, &position) > 0) {
    return;
  }

  ObjectDispatch_SelectAnimation(subject, 8);
  WaitFrames(15);

  target->speed = PIXELS(1) / 5;
  target->acceleration = PIXELS(1) / 5;
  Object_SetPosition(target, position.x, position.y, position.z);

  subject->speed = PIXELS(1) / 5;
  subject->acceleration = PIXELS(1) / 5;
  Object_SetPosition(subject, position.x, position.y, position.z);

  Sound_PlaySong(SE_238);
  Object_WaitForMove(target);
  Sound_PlaySong(SE_STOP_SLOT3);

  target->x.fixed = position.x;
  target->z.fixed = position.z;
  target->velocityX = zero;
  target->velocityZ = zero;

  ObjectDispatch_SelectAnimation(subject, 1);
}

#include "common/colosso_occupant.h"

// clang-format off
const u8 gColosso_LetteringPaletteOffsets[] = {
#if LANGUAGE == LANGUAGE_JAPANESE || LANGUAGE == LANGUAGE_ENGLISH
    0, 32, 32, 32, 96, 64, 64, 64, 128, 0,
#else
    0, 32, 32, 32, 96, 64, 64, 160, 128, 0,
#endif
};

const s16 gColosseumAzartMatch_LetteringDigit[] = {
    0x1000, 512, 1, 0x2000, 0, 1, 0x1000, 256,
    6, 0x2000, 16, 6, 0x7fff, 30, 0x2000, 0,
    30, 0x7fff, 30, -1,
};

const s16 gColosseumAzartMatch_LetteringLose[] = {
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

s32 gColosseumAzartMatch_ScriptA[] = {
    SC_ADD(SW_Y, -PIXELS(1) / 4),
    SC_WAIT(1),
    SC_LOOP(60, SC_TOP),
    SC_FREE,
};

s32 gColosseumAzartMatch_ScriptB[] = {
    SC_SET(SW_HOOK, (s32)ColosseumAzartMatch_SpawnRandomEffect),
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
    SC_POSITION(0, 0, 0),
    SC_SET(SW_HOOK, 0),
    SC_END,
};

/// Pixel steps by direction: X in the upper halfword, Z in the lower.
#define DIRECTION_STEP(x, z) ((u32)(x) << 16 | (u16)(z))

u32 gColosseumAzartMatch_DirectionSteps[] = {
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

struct MapWarpDest gColosseumAzartMatch_WarpDests[] = {
    { WARP_DEST_DEFAULT, CONDITION_ALWAYS, 920, 0, 200, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { COLOSSEUM_AZART_MATCH_RACE_FROM_LEND_A_HAND, CONDITION_ALWAYS, 312, 0, 104, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { MAP_TABLE_END },
};

u32 gColosseumAzartMatch_Warps[] = {
    MAP_WARPS(ColosseumAzartMatch),
    MAP_WARP(COLOSSEUM_AZART_MATCH_LEND_A_HAND_TO_RACE, ColosseumAzartMatch, COLOSSEUM_AZART_MATCH_RACE_FROM_LEND_A_HAND),
    MAP_WARP(COLOSSEUM_AZART_MATCH_TO_COLOSSEUM_WEST, Colosseum, COLOSSEUM_FROM_COLOSSEUM_AZART_MATCH_WEST),
    MAP_WARP(COLOSSEUM_AZART_MATCH_TO_COLOSSEUM_EAST, Colosseum, COLOSSEUM_FROM_COLOSSEUM_AZART_MATCH_EAST),
    MAP_WARP(COLOSSEUM_AZART_MATCH_TO_BABI_PALACE, BabiPalace, BABI_PALACE_FROM_COLOSSEUM_AZART_MATCH),
    MAP_WARPS_END,
};

struct ObjectEventTemplate gColosseumAzartMatch_ObjectEventTemplates[] = {
    { 0x0, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(312), 0, PIXELS(104), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { LOCALID_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0 },
    { LOCALID_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0 },
    { LOCALID_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0 },
    { SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_WEST, TALK_FACING_PARTY, 0 },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(312), 0, PIXELS(168), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(408), 0, PIXELS(200), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(456), 0, PIXELS(184), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { 0xe5, CONDITION_ALWAYS, 7,
        PIXELS(601), 0, PIXELS(146), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { 0x122, CONDITION_ALWAYS, 7,
        PIXELS(552), 0, PIXELS(200), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { 0x122, CONDITION_ALWAYS, 7,
        PIXELS(664), 0, PIXELS(200), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(760), 0, PIXELS(194), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(944), 0, PIXELS(216), FACING_WEST, TALK_FACING_KEEP, 0 },
    { SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, 7,
        PIXELS(1048), 0, PIXELS(136), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7,
        PIXELS(1016), 0, PIXELS(168), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(360), 0, PIXELS(424), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(376), 0, PIXELS(312), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(600), 0, PIXELS(312), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(824), 0, PIXELS(312), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1080), 0, PIXELS(312), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1304), 0, PIXELS(176), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1304), 0, PIXELS(208), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

s32 gColosseumAzartMatch_Countdown = 0;

struct MapEvent gColosseumAzartMatch_Events[] = {
    { COORD_EVENT_WALKING, 90, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_PushRockOrCheckAhead) },
    { COORD_EVENT_WALKING, 10, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_PushRock) },
    { COORD_EVENT_WALKING, 11, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_PushRock) },
    { COORD_EVENT_WALKING_INTO(FACING_WEST), 12, FLAG_PIPEWORKS_PIPE_PUSHED, EVENT_SCRIPT(EventScript_PushPipeworksPillar) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 20, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_RollLogNorthSouth) },
    { COORD_EVENT_WALKING_INTO(FACING_SOUTH), 20, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_RollLogNorthSouth) },
    { COORD_EVENT_WALKING_INTO(FACING_WEST), 21, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_RollLogEastWest) },
    { COORD_EVENT_WALKING_INTO(FACING_EAST), 21, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_RollLogEastWest) },
    { MAP_EVENT_COORD, 50, FLAG_MAP_TEMP_10, EVENT_SCRIPT(EventScript_ReachArena) },
    { BG_EVENT(BG_EVENT_CHEST), 100, FLAG_MAPSEC_TEMP_08, FIND_ITEM(ITEM_NUT) },
    { BG_EVENT(BG_EVENT_CHEST), 101, FLAG_MAPSEC_TEMP_09, FIND_ITEM(ITEM_OIL_DROP) },
    { BG_EVENT(BG_EVENT_CHEST) | MAP_EVENT_FLAG_FACING | FACING_WEST, 103, FLAG_MAPSEC_TEMP_0A, FIND_ITEM(ITEM_NUT) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_ROCK_A, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SettleRocks) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_ROCK_B, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SettleRocks) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_ROCK_C, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SettleRocks) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_GROWTH), LOCALID_GROWTH_PLANT, FLAG_PLANT_GROWN, EVENT_SCRIPT(EventScript_GrowPlant) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_DOUSE), LOCALID_PIPE_WEST, FLAG_PIPES_MOVED, EVENT_SCRIPT(EventScript_DousePipe) },
    { MAP_EVENT_TALK, LOCALID_GUIDE, CONDITION_ALWAYS, EVENT_SCRIPT(Colosso_AskReadyToRace) },
    { MAP_EVENT_TALK, LOCALID_ROCK_ATTENDANT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToRockAttendant) },
    { MAP_EVENT_TALK, LOCALID_PIPEWORKS_ATTENDANT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToPipeworksAttendant) },
    { MAP_EVENT_TALK, LOCALID_BRIDGE_ATTENDANT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToBridgeAttendant) },
    { MAP_EVENT_TALK, LOCALID_LUMBER_ATTENDANT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToLumberAttendant) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_GUIDE, CONDITION_ALWAYS, MSGNO(MsgColossoRiverThisYearsFinalsAreIncredibleThis) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROCK_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoRiverMovingSuchAHugeRockMust) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_PIPEWORKS_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoRiverTheresOnlyOneFinalAndThis) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BRIDGE_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoRiverDidYouSeeHisMovesOut) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_LUMBER_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoRiverHowManyWarriorsHaveSwallowedTheir) },
    { MAP_EVENT_RAISED, 99, CONDITION_ALWAYS, EVENT_SCRIPT(Colosso_TakePlaceForMatch) },
    { MAP_EVENTS_END },
};

s16 gColosso_LetteringSlot = -1;

s16 gColosseumAzartMatch_LetteringTitle[] = {
#if LANGUAGE == LANGUAGE_JAPANESE
    0x4000, -188, 2048, 0x1000, 256, 1, 0x2000, 16,
#else
    0x4000, -188, 2048, 0x1000, 384, 1, 0x2000, 16,
#endif
    1, 0x7fff, 14, 0x3000, 0, 20, 0x7fff, 60,
    0x3000, 2048, 20, 0x7fff, 60, -1,
};

s16 gColosseumAzartMatch_LetteringGo[] = {
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

s16 gColosseumAzartMatch_LetteringWon[] = {
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

s32 gColosso_OpponentFinishScript[] = {
    SC_ANIMATION(2),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(6),
    SC_CALL(ColosseumAzartMatch_PlaceFinishItemAboveObjectEvent),
    SC_WAIT(3),
    SC_ANIMATION(10),
    SC_WAIT(150),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_ANIMATION(1),
    SC_END,
};

s32 gColosseumAzartMatch_ApproachScript[] = {
    SC_ANIMATION(2),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(6),
    SC_CALL(ColosseumAzartMatch_PlaceFinishItemAboveObjectEvent),
    SC_WAIT(3),
    SC_ANIMATION(28),
    SC_WAIT(150),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_ANIMATION(1),
    SC_END,
};

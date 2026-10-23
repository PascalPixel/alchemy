/// @file
/// Colosseum Final 3 (MapId_ColosseumNavampaMatch), the site of the last
/// finals match, where Isaac, having won the first two, races Navampa of
/// Gondowan, the sixth-ranked warrior, through six stages to the arena and
/// fights him there.
///
/// It works as Colosseum Final 1 does (colosseum_azart_match.c): the guide, who
/// did not think Isaac would make it this far, leaves the stages to their
/// attendants, and the stages run through the Scales of Justice, the
/// Stepping-Stone Stage, the Wall Wedge, the Free-Climb Area, the Moving
/// Sidewalk Stage and another log-rolling area.
/// ColosseumNavampaMatch_Initialize restores the stages from their temporary
/// flags (FLAG_PLANT_GROWN to FLAG_LONG_LOG_IN_PLACE, and the stone's column
/// from FLAG_STONE_COLUMN) and sets up the visit its warp destination
/// stands for:
///
/// - COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_NORTH, the tour
///   (ColosseumNavampaMatch_IntroduceThirdFinals), from the Colosseum's east
///   passage after the second win; the attendants describe their stages and
///   take the friends to cheer, and the guide sends Isaac back to the passage
///   (COLOSSEUM_NAVAMPA_MATCH_TO_COLOSSEUM).
/// - COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_SOUTH, `Lend a Hand`: Garet, Ivan
///   and Mia in turn may use Psynergy on the stages. Frost freezes the target
///   on the Scales of Justice's eastern pan so the scales stay tipped, Halt
///   stops the Wall Wedge's operator and with him its pistons, Force knocks the
///   block off the Moving Sidewalk so the belt no longer carries anyone back,
///   Growth grows a plant into a shortcut, and Move shifts the weight onto the
///   scales and the stones and the rock on the way. After Mia,
///   COLOSSEUM_NAVAMPA_MATCH_LEND_A_HAND_TO_RACE starts the race.
/// - COLOSSEUM_NAVAMPA_MATCH_RACE_FROM_LEND_A_HAND, the race against Navampa,
///   who follows a recorded path, after the lettering `Match 3` and a count of
///   three. At the arena the first to arrive takes the Claymore and the second
///   the Hunter's Sword, and the battle is ENCOUNTER_ZONE_COLOSSO's fifth or
///   sixth group, the Navampa with or without the Claymore.
///
/// Stepping onto the Scales of Justice's western pan (coord event 15,
/// EventScript_StepOnScales) sends it, with the rider, down towards 4 pixels
/// high and the eastern pan up towards 32, and stepping off (coord event 16)
/// swings them back; the weight Move sets on the western pan does the same
/// (https://alchemy.pascalpixel.com/places/colosseum-navampa-match/scales.gif:
/// the leader stepping on, each pan labelled with its height in whole
/// pixels).
///
/// Win or lose, the third match ends Colosso:
/// COLOSSEUM_NAVAMPA_MATCH_AFTER_MATCH_WON and _LOST set FLAG_COLOSSO_OVER, and
/// a win also FLAG_WON_COLOSSO. In the scene that follows
/// (ColosseumNavampaMatch_PlayPostBattleReunion), Garet, Ivan and Mia run to
/// Isaac in the arena, but he falls asleep, exhausted, and they leave him to
/// sleep; both warps lead to Babi's Palace, where he wakes long after the
/// finals.
///
/// Open questions:
/// - XXX (no evidence): what ColosseumNavampaMatch_UnusedWarriorTalk's
///   messages were meant to be; the Japanese and English catalogs hold the
///   same run after MsgColossoDidntThinkBattles (spectators' lines, the inn's
///   `Inn Full` sign, the stage announcements), so neither shipped the lines
///   it was written for.

#include "action_script.h"
#include "version.h"
#include "game_state.h"
#include "io_write_queue.h"
#include "resource_ids.h"
#include "map_ids.h"
#include "vram_tab.h"
#include "unit/import.h"
#include "unit/map/colosseum_navampa_match.h"
#include "unit/map/common/colosso_solo.h"
#include "unit/map/common/colosso_marker.h"
#include "unit/map/common/colosso_setup.h"
#include "unit/map/common/colosso_occupant.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "unit/game/game.h"
#include "unit/window/window.h"
#include "unit/window/battle_command.h"
#include "unit/menu/menu.h"
#include "unit/sound/sound.h"
#include "unit/runtime/runtime.h"
#include "unit/map/common/colosso_item_icon.h"
#include "types.h"
#include "script_layer.h"
#include "map_tables.h"
#include "colosso_log_rolling_stage.h"
#include "io_reg.h"
#include "keystate.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "field_service.h"
#include "flags.h"
#include "text/msg_ids.h"
#include "battle_unit.h"
#include "resource.h"
#include "ability_ids.h"
#include "edition.h"
#include "item_ids.h"
#include "objcmd.h"
#include "object_event.h"
#include "heap_state.h"
#include "field_runtime.h"
#include "fixed_math.h"
#include "palbuf.h"
#include "colosso_gauge.h"
#include "macros.h"
#include "songs.h"
#include "audio_engine_symbols.h"

// UB: each of these calls a void function through an s32 function type, the
// call the bytes show; UBFIX calls it as declared. Each use is tagged.
#ifdef UBFIX
#define COLOSSEUM_NAVAMPA_MATCH_FACE_DIRECTION Engine_ObjectEventFaceDirection
#define COLOSSEUM_NAVAMPA_MATCH_INITIALIZE_MODE_TASK                           \
  ColosseumNavampaMatch_StartLetteringAnimation
#define COLOSSEUM_NAVAMPA_MATCH_APPLY_MATCHING_SLOT_VALUES Map_GiveAndEquipItem
#else
#define COLOSSEUM_NAVAMPA_MATCH_FACE_DIRECTION                                 \
  ((s32 (*)())Engine_ObjectEventFaceDirection)
#define COLOSSEUM_NAVAMPA_MATCH_INITIALIZE_MODE_TASK                           \
  ((s32 (*)())ColosseumNavampaMatch_StartLetteringAnimation)
#define COLOSSEUM_NAVAMPA_MATCH_APPLY_MATCHING_SLOT_VALUES                     \
  ((s32 (*)())Map_GiveAndEquipItem)
#endif

static void ColosseumNavampaMatch_PlayPostBattleReunion(s32 battleResult);
static void ColosseumNavampaMatch_IntroduceThirdFinals(s32 guideId);
static void ColosseumNavampaMatch_ShowRoundStartLettering(s32 mode);
static void ColosseumNavampaMatch_EmptyHook(void);
static void ColosseumNavampaMatch_WaitForOpponentToFinish(void);
static s32 ColosseumNavampaMatch_CollectFinishEquipment(s32 firstItem,
                                                        s32 secondItem);
static void ColosseumNavampaMatch_PushObjectEventAhead(void);

typedef void (*MapTaskEntry)(void);

/// The third finals match's object events, numbered in the order of
/// gColosseumNavampaMatchObjectEventTemplates from LOCALID_FIRST_PLACED; the
/// rows of Isaac, Garet, Ivan and Mia take their own object events. By the
/// guide's and attendants' explanations, the Scales of Justice, the
/// Stepping-Stone Stage, the Wall Wedge, the Free-Climb Area, the Moving
/// Sidewalk Stage and another log-rolling area lead to the arena.
/// LOCALID_SCALES_WEIGHT to LOCALID_LOG_AREA_STONE_C are all drawn as the rocks
/// of Colosseum Final 1's Rock Challenge; the attendant of this match's
/// log-rolling area calls the ones there stones.
enum {
  LOCALID_OPPONENT = 8,         ///< Navampa, racing Isaac to the arena
  LOCALID_FROST_TARGET = 9,     ///< on the eastern pan
  LOCALID_SCALES_WEST_PAN = 10, ///< the Scales of Justice
  LOCALID_SCALES_EAST_PAN = 11,
  LOCALID_SCALES_WEIGHT = 12, ///< pushed onto the western pan
  LOCALID_STONE = 13,
  /// East of the Wall Wedge's pistons, by the operator; Move on it needs
  /// nothing more (EventScript_IgnoreMove).
  LOCALID_ROCK = 14,
  /// The log-rolling area's three stones, "You can get through quickly by
  /// moving the logs and stones."; each settles where Move drops it
  /// (EventScript_SettleStones, FLAG_FIRST_LOG_AREA_STONE_SETTLED and the two
  /// after it).
  LOCALID_LOG_AREA_STONE_A = 15,
  LOCALID_LOG_AREA_STONE_B = 16,
  LOCALID_LOG_AREA_STONE_C = 17,
  /// The Wall Wedge's five pistons, west to east, "The pistons move back and
  /// forth."; each is drawn by two object events, LOCALID_FIRST_PISTON + n and
  /// LOCALID_FIRST_PISTON + PISTON_COUNT + n.
  LOCALID_FIRST_PISTON = 18,
  LOCALID_LAST_PISTON = 22,
  /// A young man with fair hair who works the Wall Wedge.
  LOCALID_WALL_OPERATOR = 28,
  LOCALID_GROWTH_PLANT = 29,
  LOCALID_SIDEWALK_BLOCK = 30,
  LOCALID_LONG_LOG = 31, ///< the log-rolling area's long log
  LOCALID_GUIDE = 32,    ///< a soldier who explains the third finals match
  /// The stages' attendants, young men with fair hair in white tunics.
  LOCALID_SCALES_ATTENDANT = 33,
  LOCALID_STEPPING_STONE_ATTENDANT = 34,
  LOCALID_WALL_WEDGE_ATTENDANT = 35,
  LOCALID_FREE_CLIMB_ATTENDANT = 36,
  LOCALID_MOVING_SIDEWALK_ATTENDANT = 37,
  LOCALID_LOG_ROLLING_ATTENDANT = 38,
  LOCALID_PRIZE_A = 39, ///< the arena's Claymore and Hunter's Sword
  LOCALID_PRIZE_B = 40,
  /// The first of the three particles that rise from the sleeping Isaac after
  /// the last match (ColosseumNavampaMatch_SpawnPeriodicParticle).
  LOCALID_FIRST_SLEEP_PARTICLE = 41
};

/// The temporary flags that keep what the friends changed on the stages, for
/// the race (ColosseumNavampaMatch_Initialize restores them), and the scales'
/// state.
enum {
  /// ★ Named from the game: "Growth" (MsgAbilityName)
  FLAG_PLANT_GROWN = FLAG_MAPSEC_TEMP_60, ///< by Growth, into a shortcut
  /// ★ Named from the game: "This place is called the Wall Wedge."
  /// (MsgColossoPlaceCalledWall)
  FLAG_WALL_WEDGE_HALTED =
      FLAG_MAPSEC_TEMP_61, ///< by Halt on LOCALID_WALL_OPERATOR
  /// ★ Named from the game: "This stage is called the Scales of Justice."
  /// (MsgColossoStageCalledScales)
  FLAG_SCALES_FROZEN =
      FLAG_MAPSEC_TEMP_62, ///< by Frost on LOCALID_FROST_TARGET
  /// By Force on LOCALID_SIDEWALK_BLOCK: the belt carries no one any more.
  /// ★ Named from the game: "This is called the Moving Sidewalk Stage."
  /// (MsgColossoCalledMovingSidewalkStage)
  FLAG_SIDEWALK_BLOCK_KNOCKED_OFF = FLAG_MAPSEC_TEMP_63,
  /// Someone stands on the western pan, which has sunk
  /// (EventScript_StepOnScales).
  /// ★ Named from the game: "This stage is called the Scales of Justice."
  /// (MsgColossoStageCalledScales)
  FLAG_ON_SCALES = FLAG_MAPSEC_TEMP_67,
  /// ★ Named from the game: "This stage is called the Scales of Justice."
  /// (MsgColossoStageCalledScales)
  FLAG_WEIGHT_ON_SCALES =
      FLAG_MAPSEC_TEMP_68, ///< LOCALID_SCALES_WEIGHT is on the western pan
  /// ★ Named from the game: "To get across the tank, you'll need to roll the
  /// logs." (MsgColossoAnotherLogRollingArea)
  FLAG_LONG_LOG_IN_PLACE =
      FLAG_MAPSEC_TEMP_69, ///< at the east end (EventScript_RollLongLog)
  /// The byte of flags that keeps LOCALID_STONE's column.
  /// ★ Named from the game: "You can get through quickly by moving the logs and
  /// stones." (MsgColossoAnotherLogRollingArea)
  FLAG_STONE_COLUMN = FLAG_MAPSEC_TEMP_70,
  /// Someone has reached the arena
  /// (ColosseumNavampaMatch_CollectFinishEquipment).
  /// ★ Named from the game: "You must get through several stages before you
  /// reach the arena." (MsgColossoWarriorsEnterFinalsWithoutAny)
  FLAG_ARENA_REACHED = FLAG_MAP_TEMP_11,
  /// LOCALID_LOG_AREA_STONE_A has settled; the next two flags are B's and C's.
  /// ★ Named from the game: "You can get through quickly by moving the logs and
  /// stones." (MsgColossoAnotherLogRollingArea)
  FLAG_FIRST_LOG_AREA_STONE_SETTLED = FLAG_MAP_TEMP_14
};

enum { PISTON_COUNT = 5 };

/// Sets the leader speed for the shared solo-party placement routine.
static inline void InitializeObjectEventZero(void) {
  Engine_ObjectEventSetSpeed(LOCALID_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
}

/// Sets a companion speed for the shared solo-party placement routine.
static inline void InitializeSelectedObjectEvent(s32 localId) {
  Engine_ObjectEventSetSpeed(localId, PIXELS(1), PIXELS(1) / 2);
}

extern s32 gColossoParticleCount;

const struct MapWarpDest *ColosseumNavampaMatch_GetWarpDests(void) {
  return gColosseumNavampaMatchWarpDests;
}

const struct MapWarpRegion *ColosseumNavampaMatch_GetRegions(void) { return 0; }

const u32 *ColosseumNavampaMatch_GetWarps(void) {
  return gColosseumNavampaMatchWarps;
}

const struct ObjectEventTemplate *
ColosseumNavampaMatch_GetObjectEventTemplates(void) {
  return gColosseumNavampaMatchObjectEventTemplates;
}

/// The Wall Wedge's callback: cycles its pistons through their four patterns,
/// and raises the opening event (4 or 5) when the rider stands in an open
/// gap.
static void ColosseumNavampaMatch_UpdateWallWedge(void) {
  struct EventWork *work = gEventWork;
  struct ObjectEvent *objectEvent =
      ObjectEvent_GetById(gGameState.selectedLocalId);
  s32 row = objectEvent->z.fixed >> 20;
  s32 i;
  s32 kind;

  if (gColossoMapTaskState == 0) {
    gColossoMapTaskStatus = (gColossoMapTaskStatus + 1) & 3;
    for (i = LOCALID_FIRST_PISTON; i <= LOCALID_LAST_PISTON; i++) {
      kind = gColosseumNavampaMatch_WallWedgePattern[gColossoMapTaskStatus]
                                                    [i - LOCALID_FIRST_PISTON];
      Engine_ObjectEventSetAnimation(i, kind);
      Engine_ObjectEventSetAnimation(i + PISTON_COUNT, kind + 8);
      Engine_MapGridCopyCellAttributes(32, 11, 1, 2,
                                       (i - LOCALID_FIRST_PISTON) * 2 + 33, 11);
      if (kind != 7) {
        Engine_MapGridCopyCellAttributes(
            74, 12, 1, 1, (i - LOCALID_FIRST_PISTON) * 2 + 33, 11);
      }
    }
    Engine_ObjectEventSetAnimation(
        LOCALID_WALL_OPERATOR,
        gColosseumNavampaMatch_WallWedgePattern[gColossoMapTaskStatus]
                                               [PISTON_COUNT]);
  } else {
    for (i = LOCALID_FIRST_PISTON; i <= LOCALID_LAST_PISTON; i++) {
      kind = gColosseumNavampaMatch_WallWedgePattern[gColossoMapTaskStatus]
                                                    [i - LOCALID_FIRST_PISTON];
      if ((u32)(objectEvent->x.fixed - (i << 21) + 0x31ffff) <= 0x13fffe) {
        if (row == 11 && kind == 4)
          work->raisedTrigger = kind;
        if (row == 12 && kind == 5)
          work->raisedTrigger = kind;
      }
    }
  }
  if ((u32)++gColossoMapTaskState > 17) {
    gColossoMapTaskState = 0;
  }
}

// Function from another file; its callers use no declaration (implicit int).

/// Resets the Wall Wedge's pistons to their first pattern.
static void ColosseumNavampaMatch_ResetWallWedge(void) {
  MapTaskEntry entry;

  gColossoMapTaskState = 0;
  entry = ColosseumNavampaMatch_UpdateWallWedge;
  gColossoMapTaskStatus = 0;
  Engine_TaskRemoveCallback(entry);
  entry();
}

/// Resumes cycling the Wall Wedge's pistons.
static void ColosseumNavampaMatch_StartWallWedge(void) {
  FieldScheduler_AddCallback(ColosseumNavampaMatch_UpdateWallWedge,
                             TASK_PRIORITY_MAP);
}

/// Waits, at most two seconds, for the Wall Wedge pattern its attendant's
/// demonstration passes through.
static void ColosseumNavampaMatch_WaitForWallWedgeOpening(void) {
  s32 polls;

  Engine_TaskWait(10);

  polls = 0;
  while (gColossoMapTaskStatus != 3 || gColossoMapTaskState != 1) {
    Engine_TaskWait(1);
    polls++;
    if (polls > 119) {
      return;
    }
  }
}

/// The Moving Sidewalk Stage's belt carries the party west. It's coord event
/// 11, and until FLAG_SIDEWALK_BLOCK_KNOCKED_OFF is set the belt carries the
/// controlled object event and the camera west.
static void EventScript_RideMovingSidewalk(void) {
  struct EventWork *work = gEventWork;
  struct GameState *state = &gGameState;
  s32 id = state->selectedLocalId;
  struct ObjectEvent *subject = work->viewCenter;
  struct ObjectEvent *objectEvent = ObjectEvent_GetById(id);
  s32 z = objectEvent->z.part.pixel;

  if ((u32)(z - 183) <= 3) {
    subject->x.fixed += -(PIXELS(4) / 5);
    objectEvent->x.fixed += -(PIXELS(4) / 5);
  }
}

/// Growth on the plant on Colosseum Final 3's stages: it grows and opens a
/// shortcut (FLAG_PLANT_GROWN).
static void EventScript_GrowPlant(void) {
  Engine_GameFlagSet(FLAG_PLANT_GROWN);
  {

    MapGrid_CopyCellAttributes(47, 61, 1, 4, 49, 61);
  }
}

/// Moves a scale pan vertically at the shared speed and acceleration.
static inline void ColosseumNavampaMatch_MoveOnScales(struct ObjectEvent *pan,
                                                      s32 height) {
  pan->acceleration = PIXELS(2) / 5;
  pan->speed = PIXELS(4) / 5;
  Engine_ObjectSetPosition(pan, pan->x.fixed, height, pan->z.fixed);
}

/// Frost on the target on the Scales of Justice's eastern pan: it freezes and
/// the scales tip, the western pan down and the eastern pan up
/// (FLAG_SCALES_FROZEN).
static void EventScript_FreezeOnScales(void) {
  struct ObjectEvent *objectEvent;

  objectEvent = ObjectEvent_GetById(LOCALID_FROST_TARGET);
  objectEvent->scaleX = Q16_ONE;
  objectEvent->scaleY = Q16_ONE;

  objectEvent = ObjectEvent_GetById(LOCALID_SCALES_EAST_PAN);
  ColosseumNavampaMatch_MoveOnScales(objectEvent, PIXELS(32));

  objectEvent = ObjectEvent_GetById(LOCALID_SCALES_WEST_PAN);
  ColosseumNavampaMatch_MoveOnScales(objectEvent, PIXELS(4));

  Engine_GameFlagSet(FLAG_SCALES_FROZEN);
  MapGrid_CopyCellAttributes(15, 12, 1, 1, 13, 12);
  MapGrid_CopyCellAttributes(14, 12, 1, 1, 9, 12);
}

/// Stepping onto the Scales of Justice's western pan, unless the scales are
/// frozen: it sinks with the rider as the eastern pan rises
/// (FLAG_ON_SCALES).
static void EventScript_StepOnScales(void) {
  s32 localId;
  s32 lowered;
  struct ObjectEvent *log;
  struct ObjectEvent *rider;
  u8 *flags;

  localId = gGameState.selectedLocalId;
  lowered = GameFlag_IsSet(FLAG_SCALES_FROZEN);
  if (lowered == 0) {
    log = ObjectEvent_GetById(LOCALID_SCALES_WEST_PAN);
    if (log != NULL) {
      ObjectEvent_SetDestination(localId, log->x.part.pixel, log->z.part.pixel);
    }
    Engine_ObjectEventWaitForMove(localId);
    log = ObjectEvent_GetById(LOCALID_SCALES_EAST_PAN);
    log->motionFlags = lowered;
    ColosseumNavampaMatch_MoveOnScales(log, PIXELS(32));
    log = ObjectEvent_GetById(LOCALID_SCALES_WEST_PAN);
    log->motionFlags = lowered;
    ColosseumNavampaMatch_MoveOnScales(log, PIXELS(4));
    rider = ObjectEvent_GetById(localId);
    flags = &rider->motionFlags;
    *flags = lowered;
    ColosseumNavampaMatch_MoveOnScales(rider, PIXELS(4));
    Engine_ObjectEventSetSpriteFlags(rider, SPRITE_FLAG_SHADOW);
    Engine_ObjectEventWaitForMove(localId);
    MapGrid_CopyCellAttributes(0, 24, 1, 1, 9, 12);
    Engine_TaskWait(2);
    Engine_ObjectEventSetSpriteFlags(rider, SPRITE_FLAG_SHADOW);
    *flags = OBJ_EVENT_FOLLOWS_TERRAIN | OBJ_EVENT_FALLS;
    rider->terrainHeight = rider->y.fixed;
    Engine_GameFlagSet(FLAG_ON_SCALES);
  }
}

/// Stepping off the Scales of Justice's western pan: the scales swing back.
static void EventScript_StepOffScales(void) {
  struct ObjectEvent *objectEvent;

  Engine_ObjectEventSetAnimation(gGameState.selectedLocalId, 1);

  objectEvent = ObjectEvent_GetById(LOCALID_SCALES_EAST_PAN);
  objectEvent->motionFlags = 0;
  ColosseumNavampaMatch_MoveOnScales(objectEvent, PIXELS(4));

  objectEvent = ObjectEvent_GetById(LOCALID_SCALES_WEST_PAN);
  objectEvent->motionFlags = 0;
  ColosseumNavampaMatch_MoveOnScales(objectEvent, PIXELS(32));

  Engine_ObjectEventWaitForMove(LOCALID_SCALES_WEST_PAN);
  {
    MapGrid_CopyCellAttributes(0, 25, 1, 1, 9, 12);
  }
  Engine_TaskWait(2);
  Engine_GameFlagClear(FLAG_ON_SCALES);
}

/// Move on the weight by the Scales of Justice: when it reaches the western
/// pan, it sinks the pan and the eastern pan rises (FLAG_WEIGHT_ON_SCALES).
static void EventScript_SetWeightOnScales(void) {
  struct ObjectEvent *probe;
  struct ObjectEvent *log;
  struct ObjectEvent *upper;
  struct ObjectEvent *lower;
  s32 x;
  s32 z;

  probe = ObjectEvent_GetById(LOCALID_SCALES_WEIGHT);
  x = probe->x.fixed >> 20;
  if (x == 9) {
    z = probe->z.fixed >> 20;
    if (z == 12) {
      log = ObjectEvent_GetById(LOCALID_SCALES_WEIGHT);
      Engine_ObjectEventSetSpriteFlags(log, 0);
      log->priorityFlags = OBJ_EVENT_PRIORITY_UNDERFOOT;
      log->motionFlags = 0;
      ColosseumNavampaMatch_MoveOnScales(log, PIXELS(4));
      upper = ObjectEvent_GetById(LOCALID_SCALES_EAST_PAN);
      upper->priorityFlags = OBJ_EVENT_PRIORITY_UNDERFOOT;
      ColosseumNavampaMatch_MoveOnScales(upper, PIXELS(32));
      lower = ObjectEvent_GetById(LOCALID_SCALES_WEST_PAN);
      ColosseumNavampaMatch_MoveOnScales(lower, PIXELS(4));
      Engine_GameFlagSet(FLAG_WEIGHT_ON_SCALES);
      MapGrid_CopyCellAttributes(15, 12, 1, 1, 13, z);
      MapGrid_CopyCellAttributes(1, 25, 1, 1, x, z);
    }
  }
}

/// Walking into the weight by the Scales of Justice pushes it, perhaps onto the
/// scales.
static void EventScript_PushWeight(void) {
  ColosseumNavampaMatch_PushObjectEventAhead();
  EventScript_SetWeightOnScales();
}

/// Move on the stone on Colosseum Final 3's stages, and after each push: its
/// column is kept (the byte of flags from FLAG_STONE_COLUMN) and its cell
/// rebuilt.
static void EventScript_MoveStone(void) {
  struct ObjectEvent *objectEvent;
  s32 x;

  objectEvent = ObjectEvent_GetById(LOCALID_STONE);
  x = objectEvent->x.fixed >> 20;
  FieldGameFlag_SetByte(FLAG_STONE_COLUMN, x);
  MapGrid_CopyCellAttributes(18, 10, 3, 1, 18, 11);
  MapGrid_CopyCellAttributes(17, 11, 1, 1, x, 11);
}

/// Move on LOCALID_ROCK in Colosseum Final 3 needs nothing more.
static void EventScript_IgnoreMove(void) {}

/// Walking into a rock on Colosseum Final 3's stages (coord event 22) pushes
/// it.
static void EventScript_PushRock(void) {
  ColosseumNavampaMatch_PushObjectEventAhead();
}

/// Move on one of the log-rolling area's three stones, and each push there: a
/// stone resting on the floor becomes a step (FLAG_FIRST_LOG_AREA_STONE_SETTLED
/// and the two after it).
static void EventScript_SettleStones(void) {
  struct ObjectEvent *objectEvent;
  s32 slot;
  s32 x;
  s32 z;
  s32 x2;
  s32 z2;

  for (slot = LOCALID_LOG_AREA_STONE_A; slot <= LOCALID_LOG_AREA_STONE_C;
       slot++) {
    objectEvent = ObjectEvent_GetById(slot);
    if (MapGrid_GetTerrainHeight(0, objectEvent->x.fixed,
                                 objectEvent->z.fixed) == 0) {
      objectEvent->priorityFlags = 2;
      objectEvent->motionFlags = 0;
      x = objectEvent->x.fixed >> 20;
      z = objectEvent->z.fixed >> 20;
      MapGrid_CopyCellAttributes(83, 13, 1, 1, x, z);
      x2 = objectEvent->x.fixed >> 20;
      z2 = objectEvent->z.fixed >> 20;
      MapGrid_CopyCellAttributes(83, 13, 1, 1, x2, z2 + 52);
      Engine_GameFlagSet(slot - LOCALID_LOG_AREA_STONE_A +
                         FLAG_FIRST_LOG_AREA_STONE_SETTLED);
    }
  }
}

/// A callback for Colosseum Final 3's eastern end: the cell at column 81, row
/// 12 lets the rider through only while facing south.
static void ColosseumNavampaMatch_UpdateEastEndCollision(void) {
  struct ObjectEvent *objectEvent;
  s32 x;
  s32 z;
  s32 collisionKind;

  objectEvent = ObjectEvent_GetById(gGameState.selectedLocalId);
  x = objectEvent->x.fixed >> 20;
  collisionKind = 23;
  z = objectEvent->z.fixed >> 20;
  if (x == 81 && z == 12) {
    if ((objectEvent->facing & 0xE000) == 0x4000) {
      collisionKind = 253;
    }
    MapGrid_SetMetatileBehaviorAt(0, x << 20, z << 20, collisionKind);
  }
}

/// Rejects blocked cells and cells occupied by one of the log-rolling area's
/// three stones.
static s32 ColosseumNavampaMatch_CheckLongLogCell(s32 x, s32 z) {
  struct ObjectEvent *objectEvent;

  if (MapGrid_GetMetatileBehaviorAt(0, x, z) == 255) {
    return -2;
  }
  objectEvent = ObjectEvent_GetById(LOCALID_LOG_AREA_STONE_A);
  x >>= 20;
  z >>= 20;
  if (objectEvent->x.fixed >> 20 == x && objectEvent->z.fixed >> 20 == z) {
    return -1;
  }
  objectEvent = ObjectEvent_GetById(LOCALID_LOG_AREA_STONE_B);
  if (objectEvent->x.fixed >> 20 == x && objectEvent->z.fixed >> 20 == z) {
    return -1;
  }
  objectEvent = ObjectEvent_GetById(LOCALID_LOG_AREA_STONE_C);
  if (objectEvent->x.fixed >> 20 == x && objectEvent->z.fixed >> 20 == z) {
    return -1;
  }
  return 0;
}

/// Checks all four cells covered by the long rolling log.
static s32 ColosseumNavampaMatch_CheckLongLogPath(s32 x, s32 z) {
  if (ColosseumNavampaMatch_CheckLongLogCell(x, z - PIXELS(24)) != 0 ||
      ColosseumNavampaMatch_CheckLongLogCell(x, z - PIXELS(8)) != 0 ||
      ColosseumNavampaMatch_CheckLongLogCell(x, z + PIXELS(8)) != 0 ||
      ColosseumNavampaMatch_CheckLongLogCell(x, z + PIXELS(24)) != 0) {
    return -1;
  }
  return 0;
}

/// The long log carries the leader until something stops it. Walking into the
/// log-rolling area's long log (coord event 23) rolls it, with the leader, the
/// way the pad is held; at the east end it drops into place and the path is
/// made (FLAG_LONG_LOG_IN_PLACE). Away from the log, a stone ahead is pushed
/// instead.
static void EventScript_RollLongLog(void) {
  struct ObjectEvent *rider;
  struct ObjectEvent *log;
  struct EventWork *work;
  s32 angle;
  s32 localId;
  s32 x;
  s32 z;
  s32 steps;
  s32 nextX;
  s32 column;
  s32 acceleration;
  s32 mode;
  s32 step;
  s32 ahead[3];

  rider = ObjectEvent_GetById(gGameState.selectedLocalId);
  log = ObjectEvent_GetById(LOCALID_LONG_LOG);
  steps = 0;
  angle = gColossoEarlySequenceData[rider->facing >> 13];
  localId = gGameState.selectedLocalId;
  SET_VECTOR(ahead, (rider->x.fixed & -0x100000) + PIXELS(8), rider->y.fixed,
             (rider->z.fixed & -0x100000) + PIXELS(8));
  Vector_AddPolarOffset(PIXELS(16), angle, ahead);
  x = log->x.fixed;
  z = log->z.fixed;
  // Only a nearby rider rolls the log; other interactions push the object event
  // ahead.
  if ((ahead[0] - x >= 0 ? ahead[0] - x : x - ahead[0]) > PIXELS(8) ||
      (ahead[2] - z >= 0 ? ahead[2] - z : z - ahead[2]) > PIXELS(32)) {
    goto far;
  }
  if (gKeysHeld & KEY_LEFT) {
    mode = 2;
    step = -8;
    for (;;) {
      nextX = x - PIXELS(16);
      if (ColosseumNavampaMatch_CheckLongLogPath(nextX, z) == 0) {
        steps++;
        x = nextX;
      } else {
        break;
      }
    }
  } else {
    if (!(gKeysHeld & KEY_RIGHT)) {
      return;
    }
    mode = 3;
    step = 8;
    for (;;) {
      nextX = x + PIXELS(16);
      if (ColosseumNavampaMatch_CheckLongLogPath(nextX, z) == 0) {
        steps++;
        x = nextX;
      } else {
        break;
      }
    }
  }
  if (steps == 0) {
    return;
  }
  // Clear the old footprint, then move the log and rider together.
  MapGrid_CopyCellAttributes(74, 8, 1, 4, log->x.fixed >> 20, 9);
  MapGrid_CopyCellAttributes(120, 60, 8, 5, 74, 60);
  Engine_ScriptStart();
  Engine_ObjectEventSetAnimation(localId, 8);
  Engine_ScriptWait(6);
  log->speed = PIXELS(1) / 2;
  acceleration = PIXELS(1) / 5;
  log->acceleration = acceleration;
  ObjectEvent_SetMode(log, mode);
  Engine_ObjectSetPosition(log, x, 0, z);
  Engine_ScriptWait(6);
  Engine_ObjectEventSetAnimation(localId, ANIMATION_WALK);
  work = Runtime_AllocateBlock(HEAP_SLOT_EVENT, sizeof(struct FieldWork));
  ObjectDispatch_StartCamera((struct DispatchObject *)work->viewCenter,
                             (s32)log);
  ObjectEvent_SetSpeed(localId, PIXELS(1) / 2, acceleration);
  Script_PlayCue(SE_HEAVY_MOVE);
  ObjectEvent_SetMode(rider, 2);
  Engine_ObjectSetPosition(rider, (steps * step * PIXELS(1)) + rider->x.fixed,
                           0, rider->z.fixed);
  FieldObject_WaitForMove(rider);
  ObjectEvent_SetMode(rider, 1);
  FieldObject_WaitForMove(log);
  // At the eastern end the log settles at the end of its stage.
  if (x >= PIXELS(1328)) {
    GameFlag_Set(FLAG_LONG_LOG_IN_PLACE);
    Engine_ObjectEventSetAnimation(LOCALID_LONG_LOG, ANIMATION_NOD);
    ObjectEvent_SetDestinationOffset(LOCALID_LONG_LOG, 18, 6);
    Engine_ScriptWait(30);
    ObjectEvent_SetMode(log, 8);
    FieldObject_WaitForMove(log);
    log->priorityFlags = OBJ_EVENT_PRIORITY_UNDERFOOT;
    column = 84;
    MapGrid_CopyCellAttributes(86, 10, 1, 2, column, 10);
    MapGrid_CopyCellAttributes(86, 9, 1, 1, column, 12);
    Script_PlayCue(SE_STOP_SLOT3);
    Script_PlayCue(SE_240);
  } else {
    ObjectEvent_SetMode(log, 1);
    Script_PlayCue(SE_STOP_SLOT3);
    Script_PlayCue(SE_HEAVY_STOP);
    column = x >> 20;
    MapGrid_CopyCellAttributes(85, 9, 1, 4, column, 9);
    MapGrid_CopyCellAttributes(85, 9, 1, 4, column, 61);
  }
  Engine_ScriptWait(15);
  Engine_ScriptEnd();
  return;
far:
  ColosseumNavampaMatch_PushObjectEventAhead();
  EventScript_SettleStones();
}

/// Displays the activated moving sidewalk and plays its arrival sounds.
static s32 ColosseumNavampaMatch_ShowActivatedSidewalkLayer(
    struct ScriptInterpreter *interpreter) {
  TileAnimation_PauseChannel(1);
  TileAnimation_ResumeChannel(2);
  Engine_AudioPlayCue(SE_STOP_SLOT3);
  Engine_AudioPlayCue(SE_217);
  return 0;
}

/// Force on the Moving Sidewalk Stage's block: it slides away and the belt
/// carries no one any more (FLAG_SIDEWALK_BLOCK_KNOCKED_OFF).
static void EventScript_ForceSidewalkBlock(void) {
  struct ObjectEvent *effect;
  s32 moveRate;

  effect = ObjectEvent_GetById(LOCALID_SIDEWALK_BLOCK);
  effect->motionFlags = 0;
  moveRate = PIXELS(8) / 5;
  effect->acceleration = moveRate;
  effect->speed = moveRate;
  ObjectEvent_SetMode(effect, 2);
  Engine_ObjectSetScript(effect, gColossoMapEventEffect);
  Engine_GameFlagSet(FLAG_SIDEWALK_BLOCK_KNOCKED_OFF);
}

/// Halt on the Wall Wedge's operator: the pistons stop at their next open
/// pattern (FLAG_WALL_WEDGE_HALTED).
static void EventScript_HaltWall(void) {
  FieldEffect_PauseObject(LOCALID_WALL_OPERATOR);
  Engine_GameFlagSet(FLAG_WALL_WEDGE_HALTED);
  Engine_TaskWait(10);
  while (gColossoMapTaskStatus != 1 && gColossoMapTaskStatus != 3)
    Engine_TaskWait(1);
  Engine_TaskWait(1);
  Engine_TaskRemoveCallback(ColosseumNavampaMatch_UpdateWallWedge);
}

/// Raised by the Wall Wedge (4): the rider passes through the pistons'
/// northern opening.
static void EventScript_PassNorthWallOpening(void) {
  struct ObjectEvent *objectEvent;
#ifndef UBFIX
  s16 *gameWords;
#endif
  s32 *localId;
  s32 z;

#ifdef UBFIX
  localId = &gGameState.selectedLocalId;
#else
  // The original word-indexed address takes three more instructions here.
  gameWords = (s16 *)&gGameState;
  localId = (s32 *)&gameWords[offsetof(struct GameState, selectedLocalId) /
                              sizeof(s16)];
#endif
  objectEvent = ObjectEvent_GetById(*localId);
  objectEvent->acceleration = PIXELS(1);
  objectEvent->speed = PIXELS(2);
  Engine_ObjectEventSetAttachedEffect(*localId, ATTACHED_EFFECT_IN_FRONT |
                                                    ATTACHED_EFFECT_ONCE);
  ObjectEvent_SetMode(objectEvent, 5);
  z = objectEvent->z.fixed & 0xFFF00000;
  Engine_ObjectSetPosition(objectEvent, objectEvent->x.fixed,
                           objectEvent->y.fixed, z + PIXELS(24));
  FieldObject_WaitForMove(objectEvent);
}

/// Raised by the Wall Wedge (5): the rider passes through the pistons'
/// southern opening.
static void EventScript_PassSouthWallOpening(void) {
  struct ObjectEvent *objectEvent;
#ifndef UBFIX
  s16 *gameWords;
#endif
  s32 *localId;
  s32 z;

#ifdef UBFIX
  localId = &gGameState.selectedLocalId;
#else
  // The original word-indexed address takes three more instructions here.
  gameWords = (s16 *)&gGameState;
  localId = (s32 *)&gameWords[offsetof(struct GameState, selectedLocalId) /
                              sizeof(s16)];
#endif
  objectEvent = ObjectEvent_GetById(*localId);
  if (objectEvent->x.fixed > PIXELS(664)) {
    objectEvent->x.fixed = PIXELS(664);
  }
  objectEvent->acceleration = PIXELS(1);
  objectEvent->speed = PIXELS(2);
  ObjectEvent_SetMode(objectEvent, 5);
  z = objectEvent->z.fixed & 0xFFF00000;
  Engine_ObjectSetPosition(objectEvent, objectEvent->x.fixed,
                           objectEvent->y.fixed, z + PIXELS(12));
  FieldObject_WaitForMove(objectEvent);
  Engine_ObjectEventSetAttachedEffect(*localId, ATTACHED_EFFECT_IN_FRONT |
                                                    ATTACHED_EFFECT_ONCE);
  FieldObjectEvent_JumpDownLedge(*localId, 6, 0);
}

/// Unused: an empty function nothing calls.
UNUSED static void ColosseumNavampaMatch_UnusedEventHook(void) {}

/// Unused: talking to a character at the third finals site would have handed
/// the player control of that character.
///
/// Nothing calls it. Arriving at COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_SOUTH
/// it would ask a yes or no question and, on yes, make the character it names
/// the one the player moves alone, as the friends' turns do. XXX: likely its
/// message numbers date from a catalog older than the Japanese release,
/// because in it, as in the English one, the lines three apart per character
/// from MsgColossoDidntThinkBattles are a spectator's thought and the stage
/// announcements, not this question.
UNUSED static void ColosseumNavampaMatch_UnusedWarriorTalk(s32 warriorId) {
  struct EventWork *work;
  s32 localId;
  s32 messageOffset;
  s32 warpDest;

  work = gEventWork;
  localId = gGameState.selectedLocalId;
  warpDest = gGameState.warpDest;
  if (warpDest == COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_SOUTH) {
    Engine_ScriptStart();
    messageOffset = warriorId * 3;
    Engine_ScriptSetMessage(messageOffset + MsgColossoDidntThinkBattles);
    Engine_ScriptOpenMessage(warriorId, 0);
    if (Engine_ScriptChooseYesNo(localId, 0) == 0) {
      Engine_ScriptSetMessage(messageOffset + MsgColossoDidntThinkBattles + 1);
      Engine_ScriptShowMessage(warriorId, 0);
      work->startTransition =
          MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
      work->transitionFrames = 15;
      Engine_ScriptCloseScreen();
      Engine_ScriptWaitForScreen();
      Colosso_ControlCharacterAlone(warriorId);
      Engine_ScriptOpenScreen();
      Engine_ScriptWaitForScreen();
    } else {
      Engine_ScriptSetMessage(messageOffset + MsgColossoDidntThinkBattles + 2);
      Engine_ScriptShowMessage(warriorId, 0);
    }
    Engine_ScriptEnd();
  }
}

/// Reaching Colosseum Final 3's arena: the leader takes a prize (the Claymore
/// if first, the Hunter's Sword if second), and he and Navampa face off and
/// fight; the battle's group follows who took the Claymore.
/// The arena's coord event row waits on FLAG_MAP_TEMP_10 being clear, which
/// nothing in the game sets (colosseum_azart_match.c's EventScript_ReachArena).
static void EventScript_ReachArena(void) {
  s32 place;
  s32 i;
  s32 last;

  last = 4;
  ColosseumNavampaMatch_EmptyHook();
  Engine_ScriptStart();
  place = ColosseumNavampaMatch_CollectFinishEquipment(ITEM_CLAYMORE,
                                                       ITEM_HUNTERS_SWORD);
  ColosseumNavampaMatch_WaitForOpponentToFinish();
  for (i = 0; i < 10; i++) {
    ObjectEvent_RefreshSelectorById(LOCALID_OPPONENT);
  }
  ObjectEvent_SetSpeed(LOCALID_OPPONENT, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkTo(LOCALID_OPPONENT, 1528, 192);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 1496, 192);
  Engine_ObjectEventSetAnimation(LOCALID_OPPONENT, ANIMATION_STAND);
  Engine_ObjectEventFaceEachOther(LOCALID_PARTY_LEADER, LOCALID_OPPONENT, 0);
  Engine_ScriptWait(10);
  Engine_ObjectEventSetAnimation(LOCALID_OPPONENT, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  Engine_ScriptWait(20);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(2), PIXELS(1));
  ObjectEvent_SetSpeed(LOCALID_OPPONENT, PIXELS(2), PIXELS(1));
  ObjectEvent_WalkTo(LOCALID_PARTY_LEADER, 1504, 192);
  ObjectEvent_WalkToAndWait(LOCALID_OPPONENT, 1520, 192);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_LEADER_STRIDE);
  Engine_ObjectEventSetAnimation(LOCALID_OPPONENT,
                                 WARRIOR_ANIMATION_FIGHTING_STANCE);
  Engine_ScriptWait(10);
  /* FAKEMATCH(temporary): last, set to 4 at the top, keeps the + 1 unfolded;
   * as 4 - place + 1, 1 of 106 instructions differ in tbs-en. */
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_COLOSSO,
                                        last - place + 1);
  Battle_SetStart(BATTLE_START_SCRIPTED);
  FieldScript_SetNextDestination((s32)&MapId_ColosseumNavampaMatch,
                                 COLOSSEUM_NAVAMPA_MATCH_AFTER_MATCH_WON);
  FieldScript_SetDefeatDestination((s32)&MapId_ColosseumNavampaMatch,
                                   COLOSSEUM_NAVAMPA_MATCH_AFTER_MATCH_LOST);
  Engine_GameFlagSet(FLAG_KEEP_MUSIC);
}

/// Walking into something on Colosseum Final 3's stages (coord event 90):
/// unless it is an object event, it is checked.
static void EventScript_CheckAhead(void) {
  if (Colosso_FindObjectEventAhead() == 0) {
    Engine_LeaderCheckAhead();
  }
}

/// Walking into something by the Scales of Justice (coord event 91): the weight
/// is pushed, anything else is checked.
static void EventScript_PushWeightOrCheckAhead(void) {
  if (Colosso_FindObjectEventAhead() == 0) {
    Engine_LeaderCheckAhead();
  } else {
    EventScript_PushWeight();
  }
}

const struct MapEvent *ColosseumNavampaMatch_GetEvents(void) {
  return gColosseumNavampaMatchEvents;
}

/// Sets up Colosseum Final 3: restores obstacle progress, then by the arrival:
/// the race (COLOSSEUM_NAVAMPA_MATCH_RACE_FROM_LEND_A_HAND), the friends' turns
/// to lend a hand (COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_SOUTH), the guide's
/// tour (COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_NORTH), or the end of Colosso
/// after a win (COLOSSEUM_NAVAMPA_MATCH_AFTER_MATCH_WON) or a loss
/// (COLOSSEUM_NAVAMPA_MATCH_AFTER_MATCH_LOST).
s32 ColosseumNavampaMatch_Initialize(void) {
  struct ObjectEvent *objectEvent;
  s32 i;
  s32 height;
  s32 column;

  gEventWork->startTransition = MAP_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Engine_GameFlagSet(FLAG_RETREAT_BLOCKED);
  // Restore the scale pans, movable weight and supporting metatile behaviors.
  FieldScheduler_AddCallback(ColosseumNavampaMatch_UpdateEastEndCollision,
                             TASK_PRIORITY_MAP);
  MapGrid_CopyCellAttributes(74, 60, 8, 6, 120, 60);

  Engine_ObjectEventSetSpriteFlags(ObjectEvent_GetById(LOCALID_FROST_TARGET),
                                   0);

  objectEvent = ObjectEvent_GetById(LOCALID_SCALES_WEST_PAN);
  Engine_ObjectEventSetSpriteFlags(objectEvent, 0);
  objectEvent->motionFlags = 0;
  objectEvent->y.fixed = PIXELS(32);

  objectEvent = ObjectEvent_GetById(LOCALID_SCALES_EAST_PAN);
  Engine_ObjectEventSetSpriteFlags(objectEvent, 0);
  objectEvent->motionFlags = 0;
  objectEvent->y.fixed = PIXELS(4);

  if (Engine_GameFlagIsSet(866) != 0) {
    Engine_ObjectEventSetAnimation(LOCALID_FROST_TARGET, 5);
    objectEvent = ObjectEvent_GetById(LOCALID_SCALES_WEST_PAN);
    objectEvent->y.fixed = PIXELS(4);
    objectEvent = ObjectEvent_GetById(LOCALID_SCALES_EAST_PAN);
    objectEvent->y.fixed = PIXELS(32);
    MapGrid_CopyCellAttributes(15, 12, 1, 1, 13, 12);
  } else {
    objectEvent = ObjectEvent_GetById(LOCALID_FROST_TARGET);
    objectEvent->scaleX = 3 * Q16_ONE / 2;
    objectEvent->scaleY = 3 * Q16_ONE / 2;
    if (Engine_GameFlagIsSet(871) != 0) {
      MapGrid_CopyCellAttributes(0, 24, 1, 1, 9, 12);
    } else {
      MapGrid_CopyCellAttributes(0, 25, 1, 1, 9, 12);
    }
  }

  if (Engine_GameFlagIsSet(872) != 0) {
    MapGrid_CopyCellAttributes(15, 12, 1, 1, 13, 12);
    MapGrid_CopyCellAttributes(1, 25, 1, 1, 9, 12);
    objectEvent = ObjectEvent_GetById(LOCALID_SCALES_WEIGHT);
    Engine_ObjectEventSetSpriteFlags(objectEvent, 0);
    objectEvent->motionFlags = 0;
    objectEvent->y.fixed = PIXELS(2);
    objectEvent->priorityFlags = OBJ_EVENT_PRIORITY_UNDERFOOT;
    objectEvent = ObjectEvent_GetById(LOCALID_SCALES_WEST_PAN);
    objectEvent->y.fixed = PIXELS(4);
    objectEvent->priorityFlags = OBJ_EVENT_PRIORITY_UNDERFOOT;
    objectEvent = ObjectEvent_GetById(LOCALID_SCALES_EAST_PAN);
    objectEvent->y.fixed = PIXELS(32);
  }

  column = FieldGameFlag_GetByte(FLAG_STONE_COLUMN);
  if (column == 0) {
    column = 19;
  }
  objectEvent = ObjectEvent_GetById(LOCALID_STONE);
  objectEvent->x.fixed = (column << 20) + (PIXELS(8));
  objectEvent->motionFlags = 0;
  objectEvent->priorityFlags = OBJ_EVENT_PRIORITY_UNDERFOOT;
  MapGrid_CopyCellAttributes(18, 10, 3, 1, 18, 11);
  MapGrid_CopyCellAttributes(17, 11, 1, 1, column, 11);

  for (i = LOCALID_LOG_AREA_STONE_A; i <= LOCALID_LOG_AREA_STONE_C; i++) {
    objectEvent = ObjectEvent_GetById(i);
    height =
        MapGrid_GetTerrainHeight(0, objectEvent->x.fixed, objectEvent->z.fixed);
    if (objectEvent->y.fixed == 0 && height == 0) {
      objectEvent->priorityFlags = OBJ_EVENT_PRIORITY_UNDERFOOT;
      objectEvent->motionFlags = height;
      MapGrid_CopyCellAttributes(83, 13, 1, 1, objectEvent->x.fixed >> 20,
                                 objectEvent->z.fixed >> 20);
      MapGrid_CopyCellAttributes(83, 13, 1, 1, objectEvent->x.fixed >> 20,
                                 (objectEvent->z.fixed >> 20) + 52);
    }
  }

  // Halt preserves a stationary wall; otherwise restart its pattern cycle.
  if (Engine_GameFlagIsSet(865) != 0) {
    for (i = LOCALID_FIRST_PISTON; i <= LOCALID_LAST_PISTON; i++) {
      objectEvent = ObjectEvent_GetById(i);
      objectEvent->priorityFlags = OBJ_EVENT_PRIORITY_UNDERFOOT;
      ObjectEvent_SetMode(objectEvent, 2);
      objectEvent = ObjectEvent_GetById(i + PISTON_COUNT);
      objectEvent->priorityFlags = OBJ_EVENT_PRIORITY_UNDERFOOT;
      objectEvent->motionFlags = 0;
      objectEvent->y.fixed = PIXELS(32);
      ObjectEvent_SetMode(objectEvent, 10);
      MapGrid_CopyCellAttributes(74, 12, 1, 1, i * 2 - 3, 11);
    }
    Engine_ObjectEventSetAnimation(LOCALID_WALL_OPERATOR, 10);
    FieldEffect_PauseObject(LOCALID_WALL_OPERATOR);
  } else {
    for (i = LOCALID_FIRST_PISTON; i <= LOCALID_LAST_PISTON; i++) {
      objectEvent = ObjectEvent_GetById(i);
      objectEvent->priorityFlags = OBJ_EVENT_PRIORITY_UNDERFOOT;
      objectEvent = ObjectEvent_GetById(i + PISTON_COUNT);
      objectEvent->priorityFlags = OBJ_EVENT_PRIORITY_UNDERFOOT;
      objectEvent->motionFlags = 0;
      objectEvent->y.fixed = PIXELS(32);
    }
    FieldScheduler_AddCallback(ColosseumNavampaMatch_UpdateWallWedge,
                               TASK_PRIORITY_MAP);
  }

  if (Engine_GameFlagIsSet(864) != 0) {
    Engine_ObjectEventSetAnimation(LOCALID_GROWTH_PLANT, ANIMATION_SHAKE_HEAD);
    MapGrid_CopyCellAttributes(47, 61, 1, 4, 49, 61);
  }

  if (Engine_GameFlagIsSet(867) != 0) {
    TileAnimation_PauseChannel(1);
    objectEvent = ObjectEvent_GetById(LOCALID_SIDEWALK_BLOCK);
    objectEvent->motionFlags = 0;
    objectEvent->x.fixed = PIXELS(1130);
    objectEvent->z.fixed = PIXELS(184);
    Engine_ObjectEventSetSpriteFlags(objectEvent, 0);
    ObjectEvent_SetMode(objectEvent, 3);
    Engine_ObjectSetScript(objectEvent,
                           gColosseumNavampaMatch_SidewalkBlockOffBelt);
  } else {
    TileAnimation_PauseChannel(2);
  }

  if (Engine_GameFlagIsSet(873) != 0) {
    objectEvent = ObjectEvent_GetById(LOCALID_LONG_LOG);
    ObjectEvent_SetMode(objectEvent, 8);
    objectEvent->priorityFlags = OBJ_EVENT_PRIORITY_UNDERFOOT;
    MapGrid_CopyCellAttributes(86, 10, 1, 2, 84, 10);
    MapGrid_CopyCellAttributes(86, 9, 1, 1, 84, 12);
  } else {
    objectEvent = ObjectEvent_GetById(LOCALID_LONG_LOG);
    MapGrid_CopyCellAttributes(85, 9, 1, 4, objectEvent->x.fixed >> 20, 9);
    MapGrid_CopyCellAttributes(85, 9, 1, 4, objectEvent->x.fixed >> 20, 61);
  }

  objectEvent = ObjectEvent_GetById(LOCALID_FROST_TARGET);
  objectEvent->motionFlags = 0;
  objectEvent->priorityFlags = OBJ_EVENT_PRIORITY_UNDERFOOT;
  objectEvent = ObjectEvent_GetById(LOCALID_SCALES_WEST_PAN);
  objectEvent->motionFlags = 0;
  objectEvent->priorityFlags = OBJ_EVENT_PRIORITY_UNDERFOOT;
  objectEvent = ObjectEvent_GetById(LOCALID_SCALES_EAST_PAN);
  objectEvent->motionFlags = 0;
  objectEvent->priorityFlags = OBJ_EVENT_PRIORITY_UNDERFOOT;
  Engine_ObjectEventSetAnimation(LOCALID_OPPONENT,
                                 WARRIOR_ANIMATION_FIGHTING_STANCE);
  gGameState.movementMode = LEADER_MOVEMENT_WALK;
  Colosso_ShowItemIcon(LOCALID_PRIZE_A, ITEM_CLAYMORE);
  Colosso_ShowItemIcon(LOCALID_PRIZE_B, ITEM_HUNTERS_SWORD);
  Engine_ObjectEventSetChildValue(LOCALID_OPPONENT, 2);

  // Warp destinations: the race (1), the friends' turns, the tour, and the end
  // of Colosso after a win (4) or a loss (5).
  switch (gGameState.warpDest) {
  case COLOSSEUM_NAVAMPA_MATCH_RACE_FROM_LEND_A_HAND:
    Colosso_StartRaceGauge(LOCALID_PARTY_LEADER, LOCALID_OPPONENT, 6,
                           PIXELS(1512), PIXELS(192), LOCALID_PRIZE_A,
                           LOCALID_PRIZE_B);
    MapGrid_CopyCellsLayered(127, 0, 1, 2, 5, 2);
    Engine_ObjectEventDestroy(LOCALID_GUIDE);
    Engine_ObjectEventDestroy(LOCALID_SCALES_ATTENDANT);
    Engine_ObjectEventDestroy(LOCALID_STEPPING_STONE_ATTENDANT);
    Engine_ObjectEventDestroy(LOCALID_WALL_WEDGE_ATTENDANT);
    Engine_ObjectEventDestroy(LOCALID_FREE_CLIMB_ATTENDANT);
    Engine_ObjectEventDestroy(LOCALID_MOVING_SIDEWALK_ATTENDANT);
    Engine_ObjectEventDestroy(LOCALID_LOG_ROLLING_ATTENDANT);
    if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
      Engine_AudioPlayCue(AUDIO_CUE_FADE_BGM);
      Colosso_ControlCharacterAlone(ISAAC);
      Colosso_PlaceFriendsAtCheeringSpots();
      ObjectEvent_LinkObjectAndSetCallback(GARET, LOCALID_PARTY_LEADER);
      ColosseumNavampaMatch_ShowRoundStartLettering(3);
    }
    ObjectEvent_LinkObjectAndSetCallback(GARET, LOCALID_PARTY_LEADER);
    ObjectEvent_LinkObjectAndSetCallback(IVAN, LOCALID_PARTY_LEADER);
    ObjectEvent_LinkObjectAndSetCallback(MIA, LOCALID_PARTY_LEADER);
    Colosso_StartOpponent((s32)&ResourceId_OpponentPathC);
    break;

  case COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_SOUTH:
    FieldScheduler_AddCallback(Colosso_RaiseTakePlaceOnPsynergy,
                               TASK_PRIORITY_MAP);
    Engine_ObjectEventDestroy(LOCALID_PRIZE_A);
    Engine_ObjectEventDestroy(LOCALID_PRIZE_B);
    if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) != 0) {
      break;
    }
    Colosso_PlaceFriendsAtCheeringSpots();
    Colosso_ControlCharacterAlone(GARET);
    ColosseumNavampaMatch_ShowRoundStartLettering(0);
    break;

  case COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_NORTH:
    if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) != 0) {
      break;
    }
    ColosseumNavampaMatch_IntroduceThirdFinals(LOCALID_GUIDE);
    Colosso_ClearCheeringSpots();
    break;

  case COLOSSEUM_NAVAMPA_MATCH_AFTER_MATCH_WON:
    ColosseumNavampaMatch_PlayPostBattleReunion(1);
    Engine_ScriptRequestWarp(COLOSSEUM_NAVAMPA_MATCH_TO_BABI_PALACE);
    Engine_GameFlagSet(FLAG_COLOSSO_OVER);
    Engine_GameFlagSet(FLAG_WON_COLOSSO);
    break;

  case COLOSSEUM_NAVAMPA_MATCH_AFTER_MATCH_LOST:
    ColosseumNavampaMatch_PlayPostBattleReunion(-1);
    Engine_ScriptRequestWarp(COLOSSEUM_NAVAMPA_MATCH_TO_BABI_PALACE_LOST);
    Engine_GameFlagSet(FLAG_COLOSSO_OVER);
    break;
  }
  return 0;
}

/// Advances one of the particles that rise from the sleeping Isaac after the
/// last match; timer and work hold signed 1/256-pixel steps.
static s32 ColosseumNavampaMatch_AdvanceParticleMotion(
    struct ScriptInterpreter *interpreter) {
  struct ObjectEvent *particle = (struct ObjectEvent *)interpreter;
  particle->x.fixed += particle->timer * 256;
  particle->y.fixed += (s16)particle->work * 256;
  particle->scaleX += Q16_ONE / 40;
  particle->scaleY += Q16_ONE / 40;
  particle->timer += 5;
  particle->work = (s16)particle->work - 1;
  return 0;
}

/// Shows one of the three particles that rise from the sleeping Isaac, each at
/// its point in a three-second cycle.
static void ColosseumNavampaMatch_SpawnPeriodicParticle(void) {
  struct ObjectEvent *particle;
  struct ObjectEvent *source;
  s32 x;
  s32 y;
  s32 kind;
  s32 count;

  particle = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  count = gColossoParticleCount + 1;
  kind = LOCALID_FIRST_SLEEP_PARTICLE;
  x = particle->x.fixed;
  y = particle->y.fixed;
  gColossoParticleCount = count;
  switch (count % 180) {
  case 10:
    break;
  case 20:
    kind = LOCALID_FIRST_SLEEP_PARTICLE + 1;
    break;
  case 30:
    kind = LOCALID_FIRST_SLEEP_PARTICLE + 2;
    break;
  default:
    return;
  }
  particle = ObjectEvent_GetById(kind);
  if (particle == 0) {
    return;
  }
  source = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (source != 0) {
    Engine_ObjectEventSetPosition(kind, source->x.fixed, source->z.fixed);
  }
  Engine_ObjectEventSetSpriteFlags(ObjectEvent_GetById(kind), 0);
  particle->motionFlags = 0;
  particle->scaleX = 2 * Q16_ONE / 5;
  particle->scaleY = 2 * Q16_ONE / 5;
  {
    s32 height = PIXELS(4);
    particle->x.fixed = x + height;
    height += y;
    particle->y.fixed = height;
    particle->targetY = height;
  }
  particle->timer = 25;
  particle->work = 128;
  ObjectEvent_SetActionScript(kind, gColossoParticleKinds);
}

/// After the last match (COLOSSEUM_NAVAMPA_MATCH_AFTER_MATCH_WON or _LOST):
/// Garet, Ivan and Mia run to Isaac in the arena and call his name, but he
/// falls asleep, exhausted, and they leave him to sleep.
static void ColosseumNavampaMatch_PlayPostBattleReunion(s32 battleResult) {
  struct ObjectEvent *objectEvent;
  const s32 *leaderScaleScript;
  SchedulerCallback particleCallback;

  // Place the opponent and companions around the resting leader.
  Engine_ObjectEventDestroy(LOCALID_PRIZE_A);
  Engine_ObjectEventDestroy(LOCALID_PRIZE_B);
  Party_RestoreHpPp(1);
  Engine_AudioPlayCue(AUDIO_CUE_FADE_BGM);
  Engine_ScriptStart();
  ObjectEvent_SetPosition(LOCALID_OPPONENT, PIXELS(1544), PIXELS(192));
  if (battleResult < 0) {
    Engine_ObjectEventSetAnimation(LOCALID_OPPONENT,
                                   WARRIOR_ANIMATION_ARMS_RAISED);
  } else {
    Engine_ObjectEventSetAnimation(LOCALID_OPPONENT,
                                   WARRIOR_ANIMATION_DEFEATED);
  }
  ObjectEvent_SetActionScript(LOCALID_OPPONENT,
                              gColosseumNavampaMatch_OpponentAfterBattle);
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, PIXELS(1504), PIXELS(192));
  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  {
    u16 shown = 0;

    objectEvent->facing = shown;
  }
  ObjectEvent_SetActionScript(LOCALID_PARTY_LEADER,
                              gColosseumNavampaMatch_LeaderActionA);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_LEADER_CROUCH);
  ObjectEvent_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(IVAN, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(MIA, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetPosition(GARET, PIXELS(1464), PIXELS(184));
  ObjectEvent_SetPosition(IVAN, PIXELS(1464), PIXELS(200));
  ObjectEvent_SetPosition(MIA, PIXELS(1448), PIXELS(192));
  objectEvent = ObjectEvent_GetById(GARET);
  {
    u16 shown = 0;

    objectEvent->facing = shown;
  }
  objectEvent = ObjectEvent_GetById(IVAN);
  {
    u16 shown = 0;

    objectEvent->facing = shown;
  }
  objectEvent = ObjectEvent_GetById(MIA);
  {
    u16 shown = 0;

    objectEvent->facing = shown;
  }
  Engine_TaskWait(1);
  Engine_CameraFollowObjectEvent(LOCALID_PARTY_LEADER, 0);
  gEventWork->startTransition = MAP_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_GREY, 1);
  Engine_ScriptOpenScreen();
  Engine_ScriptWaitForScreen();
  Engine_ScriptSetMessage(MsgColossoIsaac);
  Engine_ScriptWait(60);
  leaderScaleScript = gColossoMultiPhaseData;
  ObjectEvent_SetActionScript(LOCALID_PARTY_LEADER, leaderScaleScript);
  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  objectEvent->scaleX = Q16_ONE;
  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  objectEvent->scaleY = Q16_ONE;
  Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER, 36);
  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  objectEvent->x.fixed += PIXELS(3);
  Engine_ScriptWait(10);
  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  Engine_ObjectEventSetSpriteFlags(objectEvent, 0);
  Engine_ScriptWait(20);
  ObjectEvent_SetActionScript(LOCALID_PARTY_LEADER,
                              gColosseumNavampaMatch_LeaderActionB);
  // Let each companion react before the leader settles back to sleep.
  Script_ShowMessage(GARET, 0);
  Engine_ScriptWait(20);
  ObjectEvent_WalkToAndWait(GARET, 1504, 176);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTH, 10);
  Engine_ObjectEventShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Script_ShowMessage(IVAN, 0);
  ObjectEvent_LinkObjectAndSetCallback(GARET, IVAN);
  Engine_ScriptWait(30);
  ObjectEvent_WalkToAndWait(IVAN, 1488, 176);
  ObjectEvent_WalkTo(GARET, 1520, 184);
  ObjectEvent_WalkToAndWait(IVAN, 1504, 176);
  Engine_ObjectEventSetAnimation(GARET, ANIMATION_STAND);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTHWEST, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_SOUTH, 10);
  Engine_ObjectEventRunRepeatedMotion(IVAN, 2);
  Engine_ScriptWait(10);
  ObjectEvent_LinkObjectAndSetCallback(IVAN, GARET);
  Engine_ScriptWait(30);
  Engine_ObjectEventSetAnimationAndWait(GARET, ANIMATION_SHAKE_HEAD);
  Engine_ScriptWait(30);
  Script_ShowMessage(MIA, 0);
  ObjectEvent_LinkObjectAndSetCallback(GARET, MIA);
  ObjectEvent_LinkObjectAndSetCallback(IVAN, MIA);
  ObjectEvent_WalkToAndWait(MIA, 1488, 184);
  ObjectEvent_LinkObjectAndSetCallback(IVAN, LOCALID_PARTY_LEADER);
  ObjectEvent_LinkObjectAndSetCallback(GARET, LOCALID_PARTY_LEADER);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  ObjectEvent_LinkObjectAndSetCallback(IVAN, GARET);
  ObjectEvent_LinkObjectAndSetCallback(GARET, IVAN);
  Engine_ScriptWait(40);
  Engine_ObjectEventRunRepeatedMotion(MIA, 2);
  Engine_ScriptWait(10);
  ObjectEvent_LinkObjectAndSetCallback(IVAN, MIA);
  ObjectEvent_LinkObjectAndSetCallback(GARET, MIA);
  Engine_ScriptWait(20);
  Script_ShowMessage(MIA, 0);
  ObjectEvent_SetActionScript(LOCALID_PARTY_LEADER,
                              gColosseumNavampaMatch_LeaderActionC);
  Engine_ObjectEventRunRepeatedMotion(GARET, 2);
  ObjectEvent_LinkObjectAndSetCallback(GARET, LOCALID_PARTY_LEADER);
  Engine_ScriptWait(20);
  Script_ShowMessage(GARET, 0);
  Engine_ObjectEventRunRepeatedMotion(IVAN, 2);
  ObjectEvent_LinkObjectAndSetCallback(IVAN, LOCALID_PARTY_LEADER);
  Engine_ScriptWait(20);
  Script_ShowMessage(IVAN, 0);
  Engine_ObjectEventRunRepeatedMotion(MIA, 2);
  ObjectEvent_LinkObjectAndSetCallback(MIA, LOCALID_PARTY_LEADER);
  Engine_ScriptWait(20);
  Script_ShowMessage(MIA, 0);
  ObjectEvent_SetActionScript(LOCALID_PARTY_LEADER, leaderScaleScript);
  Engine_ScriptWait(60);
  particleCallback = ColosseumNavampaMatch_SpawnPeriodicParticle;
  // Time the small reunion effects to the companions' reactions.
  gColossoParticleCount = 9;
  Task_AddCallback(particleCallback, TASK_PRIORITY_MAP);
  Engine_ScriptWait(5);
  Engine_TaskRemoveCallback(particleCallback);
  Engine_ScriptWait(55);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Task_AddCallback(particleCallback, TASK_PRIORITY_MAP);
  Engine_ScriptWait(20);
  Engine_TaskRemoveCallback(particleCallback);
  Engine_ScriptWait(40);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  gColossoParticleCount = 9;
  Task_AddCallback(particleCallback, TASK_PRIORITY_MAP);
  Engine_ScriptWait(35);
  Engine_TaskRemoveCallback(particleCallback);
  Engine_ScriptWait(25);
  ObjectEvent_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  gColossoParticleCount = 9;
  Task_AddCallback(particleCallback, TASK_PRIORITY_MAP);
  Engine_ScriptWait(35);
  Engine_TaskRemoveCallback(particleCallback);
  Engine_ScriptWait(25);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  ObjectEvent_LinkObjectAndSetCallback(MIA, IVAN);
  ObjectEvent_LinkObjectAndSetCallback(IVAN, MIA);
  Engine_ScriptWait(60);
  ObjectEvent_LinkObjectAndSetCallback(MIA, LOCALID_PARTY_LEADER);
  ObjectEvent_LinkObjectAndSetCallback(IVAN, LOCALID_PARTY_LEADER);
  gColossoParticleCount = 9;
  Task_AddCallback(particleCallback, TASK_PRIORITY_MAP);
  Engine_ScriptWait(35);
  Engine_TaskRemoveCallback(particleCallback);
  Engine_ScriptWait(25);
  ObjectEvent_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_SMILE, 60);
  Engine_ObjectEventStartRepeatedMotion(GARET, 3);
  Engine_ObjectEventStartRepeatedMotion(IVAN, 3);
  Engine_ObjectEventRunRepeatedMotion(MIA, 3);
  ObjectEvent_LinkObjectAndSetCallback(MIA, IVAN);
  ObjectEvent_LinkObjectAndSetCallback(GARET, IVAN);
  gColossoParticleCount = 9;
  Task_AddCallback(particleCallback, TASK_PRIORITY_MAP);
  Engine_ObjectEventSetAnimation(GARET, ANIMATION_NOD);
  Engine_ObjectEventSetAnimation(IVAN, ANIMATION_NOD);
  Engine_ObjectEventSetAnimation(MIA, ANIMATION_NOD);
  Engine_ScriptWait(60);
  // The companions begin to leave, then discuss the sleeping leader off-screen.
  ObjectEvent_WalkTo(MIA, 1464, 200);
  Engine_ScriptWait(5);
  ObjectEvent_WalkTo(IVAN, 1368, 184);
  Engine_ScriptWait(3);
  ObjectEvent_WalkToAndWait(GARET, 1512, 184);
  ObjectEvent_WalkTo(GARET, 1368, 184);
  Engine_ObjectEventWaitForMove(MIA);
  Engine_ObjectEventSetAnimation(MIA, ANIMATION_STAND);
  ObjectEvent_LinkObjectAndSetCallback(MIA, LOCALID_PARTY_LEADER);
  Engine_ScriptWait(60);
  ObjectEvent_WalkToAndWait(MIA, 1432, 200);
  ObjectEvent_WalkTo(MIA, 1368, 184);
  Engine_ScriptCloseScreen();
  Engine_ScriptWaitForScreen();
  Engine_ScriptWait(30);
  ObjectEvent_SetPosition(GARET, PIXELS(1512), PIXELS(176));
  ObjectEvent_SetPosition(IVAN, PIXELS(1464), PIXELS(192));
  ObjectEvent_SetPosition(MIA, PIXELS(1560), PIXELS(200));
  ObjectSystem_SuspendProjectionAndShowSprites();
  ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_PLAIN, 2);
  Engine_ColorBufferInterpolate(1);
  Engine_ScriptSetMessage(MsgColossoIsaacFellAsleep);
  Script_ShowMessage(GARET, 0);
  Script_ShowMessage(IVAN, 0);
  Script_ShowMessage(MIA, 0);
  Engine_ScriptWait(60);
  Engine_ScriptEnd();
}

/// The tour of Colosseum Final 3
/// (COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_NORTH): the guide welcomes the party
/// to the third finals match's stages and leaves them to their attendants, then
/// Garet, Ivan and Mia walk back into Isaac.
static void ColosseumNavampaMatch_IntroduceThirdFinals(s32 guideId) {
  s32 guideZ;
  s32 guideX;
  struct ObjectEvent *objectEvent;

  objectEvent = ObjectEvent_GetById(guideId);
  guideX = objectEvent->x.part.pixel;
  guideZ = objectEvent->z.part.pixel;
  Engine_ScriptStart();
  ObjectEvent_SetSpeed(guideId, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(IVAN, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(MIA, PIXELS(1), PIXELS(1) / 2);
  Engine_ObjectEventSetPosition(LOCALID_PARTY_LEADER, PIXELS(guideX),
                                (PIXELS(guideZ) - PIXELS(48)));
  Engine_ObjectEventSetPosition(GARET, PIXELS(guideX) - PIXELS(16),
                                PIXELS(guideZ) - PIXELS(40));
  Engine_ObjectEventSetPosition(IVAN, PIXELS(guideX) + PIXELS(16),
                                PIXELS(guideZ) - PIXELS(40));
  Engine_ObjectEventSetPosition(MIA, PIXELS(guideX),
                                (PIXELS(guideZ) - PIXELS(32)));
  Engine_ObjectEventSetPosition(guideId, PIXELS(guideX),
                                (PIXELS(guideZ) - PIXELS(80)));
  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  objectEvent->facing = FACING_NORTH;
  Engine_CameraFollowObjectEvent(LOCALID_PARTY_LEADER, 0);
  Engine_ScriptOpenScreen();
  Engine_ScriptWaitForScreen();
  Engine_ScriptSetMessage(MsgColossoSiteThirdFinals);
  Engine_ObjectEventSetAnimationAndWait(guideId, ANIMATION_NOD);
  Engine_ScriptShowMessage(guideId, 0);
  Engine_ObjectEventRunRepeatedMotion(guideId, 2);
  Engine_ScriptShowMessage(guideId, 0);
  Engine_ObjectEventRunRepeatedMotion(guideId, 2);
  Engine_ScriptShowMessage(guideId, 0);
  Engine_ObjectEventRunRepeatedMotion(guideId, 2);
  Engine_ScriptShowMessage(guideId, 0);
  Engine_ObjectEventSetAnimation(MIA, ANIMATION_NOD);
  Engine_ObjectEventSetAnimation(GARET, ANIMATION_NOD);
  Engine_ObjectEventSetAnimation(IVAN, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  Engine_ScriptWait(6);
  Engine_ObjectEventSetAnimation(GARET, ANIMATION_WALK);
  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (objectEvent != 0) {
    Engine_ObjectEventSetDestination(GARET, objectEvent->x.part.pixel,
                                     objectEvent->z.part.pixel);
  }
  Engine_ObjectEventSetAnimation(IVAN, ANIMATION_WALK);
  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (objectEvent != 0) {
    Engine_ObjectEventSetDestination(IVAN, objectEvent->x.part.pixel,
                                     objectEvent->z.part.pixel);
  }
  Engine_ObjectEventSetAnimation(MIA, ANIMATION_WALK);
  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (objectEvent != 0) {
    Engine_ObjectEventSetDestination(MIA, objectEvent->x.part.pixel,
                                     objectEvent->z.part.pixel);
  }
  Engine_ObjectEventWalkToAndWait(guideId, (guideX - 16), (guideZ - 64));
  Engine_ObjectEventSetPosition(GARET, 0, 0);
  Engine_ObjectEventSetPosition(IVAN, 0, 0);
  Engine_ObjectEventSetPosition(MIA, 0, 0);
  Engine_ObjectEventWalkToAndWait(guideId, (guideX - 16), (guideZ - 16));
  Engine_ObjectEventWalkToAndWait(guideId, guideX, guideZ);
  Engine_ObjectEventFaceDirection(guideId, FACING_NORTH, 10);
  Engine_ScriptEnd();
}

/// The Scales of Justice's attendant, stage 1 of Colosseum Final 3: the first
/// time, a pointer shows the two pans and how one rises as the other falls;
/// then the objective, and a place for a friend to cheer. With a friend alone
/// on the stages (COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_SOUTH) he asks whether
/// they are done cheering instead.
static void EventScript_TalkToScalesAttendant(s32 guideId) {
  struct ObjectEvent *westPan;
  struct ObjectEvent *eastPan;
  struct ObjectEvent *westPanAgain;
  struct ObjectEvent *eastPanAgain;
  s32 promptResult;

  if (gGameState.warpDest == COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_SOUTH) {
    Colosso_AskDoneCheering(guideId);
    return;
  }
  Engine_ScriptStart();
  promptResult = Colosso_GreetAtStage(guideId, 1);
  if (promptResult == COLOSSO_GUIDE_DESCRIBE) {
    // Point out the pans, demonstrate both heights, then restore the stage.
    Engine_ScriptSetMessage(MsgColossoStageCalledScales);
    Engine_CameraSetSpeed(PIXELS(3), PIXELS(3) / 8);
    Engine_CameraMoveTo(PIXELS(152), -1, PIXELS(200), 1);
    Engine_CameraWaitForMove();
    Engine_ScriptWait(30);
    Engine_ScriptShowMessage(guideId, 0);
    Colosso_ShowPointer(104, 68, 0);
    Engine_ScriptWait(60);
    Colosso_MovePointer(168, 96, 10);
    Engine_ScriptWait(70);
    Engine_ScriptShowMessage(guideId, 0);
    Colosso_HidePointer();
    Engine_TaskWait(2);
    westPan = ObjectEvent_GetById(LOCALID_SCALES_WEST_PAN);
    westPan->motionFlags = 0;
    ColosseumNavampaMatch_MoveOnScales(westPan, PIXELS(4));
    eastPan = ObjectEvent_GetById(LOCALID_SCALES_EAST_PAN);
    eastPan->motionFlags = 0;
    ColosseumNavampaMatch_MoveOnScales(eastPan, PIXELS(32));
    FieldObject_WaitForMove(eastPan);
    Engine_ScriptWait(45);
    westPanAgain = ObjectEvent_GetById(LOCALID_SCALES_WEST_PAN);
    westPanAgain->motionFlags = 0;
    ColosseumNavampaMatch_MoveOnScales(westPanAgain, PIXELS(32));
    eastPanAgain = ObjectEvent_GetById(LOCALID_SCALES_EAST_PAN);
    eastPanAgain->motionFlags = 0;
    ColosseumNavampaMatch_MoveOnScales(eastPanAgain, PIXELS(4));
    FieldObject_WaitForMove(eastPanAgain);
    Engine_ScriptWait(15);
    Engine_ScriptShowMessage(guideId, 0);
    Colosso_ShowPointer(104, 68, 0);
    Engine_ScriptWait(30);
    Colosso_MovePointer(168, 96, 10);
    Engine_ScriptWait(40);
    Colosso_MovePointer(104, 68, 10);
    Engine_ScriptWait(70);
    Engine_ScriptShowMessage(guideId, 0);
    Colosso_HidePointer();
    Engine_TaskWait(2);
    Engine_CameraFollowObjectEvent(LOCALID_PARTY_LEADER, 0);
    Colosso_RepeatStageNumber(guideId, 1);
  } else if (promptResult == COLOSSO_GUIDE_OFFER_CHEERING) {
    Engine_ScriptSetMessage(MsgColossoObjectiveGetAcross);
    Engine_ScriptShowMessage(guideId, 0);
  }
  Colosso_OfferCheeringSpot(promptResult, guideId, 1);
  Engine_ScriptEnd();
}

/// The Stepping-Stone Stage's attendant, stage 2 of Colosseum Final 3: the
/// first time, Isaac appears and finds the way; then the objective, and a
/// place for a friend to cheer.
static void EventScript_TalkToSteppingStoneAttendant(s32 guideId) {
  s32 promptResult;
  s32 x;

  if (gGameState.warpDest == COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_SOUTH) {
    Colosso_AskDoneCheering(guideId);
    return;
  }
  Engine_ScriptStart();
  promptResult = Colosso_GreetAtStage(guideId, 2);
  if (promptResult == COLOSSO_GUIDE_DESCRIBE) {
    Engine_ScriptSetMessage(MsgColossoSteppingStoneStage);
    Camera_SetSpeed(PIXELS(3), PIXELS(3) / 8);
    Camera_MoveTo(PIXELS(376), -1, PIXELS(152), 1);
    x = 280;
    Engine_CameraWaitForMove();
    Engine_ScriptWait(30);
    Script_ShowMessage(guideId, 0);
    Colosso_FadeInWarrior(LOCALID_PARTY_LEADER, x, 200);
    ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(3) / 2, PIXELS(3) / 4);
    Colosso_WalkObjectEventToAndStand(LOCALID_PARTY_LEADER, x, 152);
    Colosso_WalkObjectEventToAndStand(LOCALID_PARTY_LEADER, 296, 152);
    Engine_ScriptWait(10);
    Engine_LeaderCheckAhead();
    Camera_MoveTo(-1, -1, -1, 0);
    Engine_ObjectEventFaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 15);
    Engine_LeaderCheckAhead();
    Camera_MoveTo(-1, -1, -1, 0);
    Engine_ObjectEventFaceDirection(LOCALID_PARTY_LEADER, FACING_EAST, 15);
    Engine_LeaderCheckAhead();
    Camera_MoveTo(-1, -1, -1, 0);
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 15);
    Script_ShowMessage(guideId, 0);
    Colosso_ShowPointer(96, 40, 0);
    Colosso_MovePointer(128, 40, 10);
    Engine_ScriptWait(30);
    Colosso_MovePointer(160, 40, 10);
    Engine_ScriptWait(30);
    Colosso_MovePointer(160, 72, 10);
    Engine_ScriptWait(30);
    Script_ShowMessage(guideId, 0);
    Colosso_HidePointer();
    Colosso_RestoreWarrior(LOCALID_PARTY_LEADER);
    Engine_CameraFollowObjectEvent(LOCALID_PARTY_LEADER, 0);
    Colosso_RepeatStageNumber(guideId, 2);
  } else if (promptResult == COLOSSO_GUIDE_OFFER_CHEERING) {
    Engine_ScriptSetMessage(MsgColossoYourGoalInStageSimple);
    Script_ShowMessage(guideId, 0);
  }
  Colosso_OfferCheeringSpot(promptResult, guideId, 2);
  Engine_ScriptEnd();
}

/// The Wall Wedge's attendant, stage 3 of Colosseum Final 3: the first time,
/// Isaac appears and times his way past the pistons; then that the walls'
/// operator will cheer for him, and a place for a friend to cheer.
static void EventScript_TalkToWallWedgeAttendant(s32 guideId) {
  struct EventWork *work;
  s32 promptResult;

  if (gGameState.warpDest == COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_SOUTH) {
    Colosso_AskDoneCheering(guideId);
  } else {
    Engine_ScriptStart();
    promptResult = Colosso_GreetAtStage(guideId, 3);
    if (promptResult == COLOSSO_GUIDE_DESCRIBE) {
      // Show the pattern cycle before demonstrating a timed passage.
      work = gEventWork;
      Engine_ScriptSetMessage(MsgColossoPlaceCalledWall);
      ColosseumNavampaMatch_ResetWallWedge();
      Engine_CameraSetSpeed(PIXELS(3), PIXELS(3) / 8);
      Engine_CameraMoveTo(PIXELS(616), -1, PIXELS(184), 1);
      Engine_CameraWaitForMove();
      Engine_ScriptWait(30);
      Script_ShowMessage(guideId, 0);
      ColosseumNavampaMatch_StartWallWedge();
      Engine_ScriptWait(60);
      Engine_ScriptShowMessage(guideId, 0);
      Colosso_FadeInWarrior(LOCALID_PARTY_LEADER, 504, 200);
      /* FAKEMATCH(wrapper): called through a cast to the s32 call the bytes
       * show, which sets r0 last; as a direct call, 1 of 136 instructions
       * differ in tbs-en. */
      COLOSSEUM_NAVAMPA_MATCH_FACE_DIRECTION(LOCALID_PARTY_LEADER, 0, 0);
      ColosseumNavampaMatch_WaitForWallWedgeOpening();
      ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(3) / 2, PIXELS(3) / 4);
      Colosso_WalkObjectEventTo(LOCALID_PARTY_LEADER, 680, 200);
      if (work->raisedTrigger != 5) {
        do {
          Engine_TaskWait(1);
          if (++promptResult > 239) {
            break;
          }
        } while (work->raisedTrigger != 5);
      }
      EventScript_PassSouthWallOpening();
      ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 20);
      ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER,
                            EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 60);
      Engine_ScriptShowMessage(guideId, 0);
      Colosso_RestoreWarrior(LOCALID_PARTY_LEADER);
      Engine_CameraFollowObjectEvent(LOCALID_PARTY_LEADER, 0);
      Colosso_RepeatStageNumber(guideId, 3);
      work->raisedTrigger = 0;
    } else if (promptResult == COLOSSO_GUIDE_OFFER_CHEERING) {
      Engine_ScriptSetMessage(MsgColossoOperatorWallsCheer);
      Engine_ScriptShowMessage(guideId, 0);
    }
    Colosso_OfferCheeringSpot(promptResult, guideId, 3);
    Engine_ScriptEnd();
  }
}

/// The Free-Climb Area's attendant, stage 4 of Colosseum Final 3: the first
/// time, Isaac appears and climbs across; then the objective, and a place for a
/// friend to cheer.
static void EventScript_TalkToFreeClimbAttendant(s32 guideId) {
  s32 result;
  struct ObjectEvent *objectEvent;
  s32 x;
  s32 y;
  s32 raisedHeight;
  s32 shiftedX;

  if (gGameState.warpDest == COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_SOUTH) {
    Colosso_AskDoneCheering(guideId);
  } else {
    Engine_ScriptStart();
    result = Colosso_GreetAtStage(guideId, 4);
    if (result == COLOSSO_GUIDE_DESCRIBE) {
      Engine_ScriptSetMessage(MsgColossoPlaceNormallyCalledFreeClimb);
      Camera_SetSpeed(PIXELS(3), PIXELS(3) / 8);
      Camera_MoveTo(PIXELS(856), -1, PIXELS(168), 1);
      Engine_CameraWaitForMove();
      Engine_ScriptWait(30);
      Script_ShowMessage(guideId, 0);
      Script_ShowMessage(guideId, 0);
      Colosso_FadeInWarrior(LOCALID_PARTY_LEADER, 816, 200);
      ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
      ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 840, 200);
      Engine_ObjectEventFaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 20);
      Field_ClimbUp();
      Camera_MoveTo(-1, -1, -1, 0);
      ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
      objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
      y = objectEvent->y.fixed;
      x = objectEvent->x.fixed;
      ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
      Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER,
                                     ANIMATION_LEADER_CLIMB);
      raisedHeight = PIXELS(6) + y;
      Engine_ObjectSetPosition(objectEvent, x, raisedHeight,
                               objectEvent->z.fixed);
      FieldObject_WaitForMove(objectEvent);
      Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER,
                                     ANIMATION_LEADER_CLIMB_EAST);
      shiftedX = PIXELS(64) + x;
      Engine_ObjectSetPosition(objectEvent, shiftedX, raisedHeight,
                               objectEvent->z.fixed);
      FieldObject_WaitForMove(objectEvent);
      Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER,
                                     ANIMATION_LEADER_CLIMB);
      Engine_ObjectSetPosition(objectEvent, shiftedX, y + PIXELS(54),
                               objectEvent->z.fixed);
      FieldObject_WaitForMove(objectEvent);
      Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER,
                                     ANIMATION_LEADER_CLIMB_WEST);
      Engine_ObjectSetPosition(objectEvent, x + PIXELS(48), y + PIXELS(54),
                               objectEvent->z.fixed);
      FieldObject_WaitForMove(objectEvent);
      Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER,
                                     ANIMATION_LEADER_CLING);
      Script_ShowMessage(guideId, 0);
      Colosso_RestoreWarrior(LOCALID_PARTY_LEADER);
      Engine_CameraFollowObjectEvent(LOCALID_PARTY_LEADER, 0);
      Colosso_RepeatStageNumber(guideId, 4);
    } else if (result == COLOSSO_GUIDE_OFFER_CHEERING) {
      Engine_ScriptSetMessage(MsgColossoClearStageMustAbleChange);
      Script_ShowMessage(guideId, 0);
    }
    Colosso_OfferCheeringSpot(result, guideId, 4);
    Engine_ScriptEnd();
  }
}

// DispatchObject is the engine's script-control view of these same object
// events.

/// Starts a scripted pixel destination while preserving the object event's
/// height.
static void ColosseumNavampaMatch_PositionObjectEvent(s32 selector, s32 x,
                                                      s32 z) {
  struct ObjectEvent *objectEvent;

  objectEvent = Engine_ObjectEventLookup(selector);
  if (objectEvent != 0) {
    ObjectDispatch_StartIdle((struct DispatchObject *)objectEvent);
    ObjectEvent_SetMode(objectEvent, 5);
    Engine_ObjectSetPosition(objectEvent, x << 16, objectEvent->y.fixed,
                             z << 16);
  }
}

/// Moves to a scripted pixel destination, waits, then restores the idle mode.
static void ColosseumNavampaMatch_PositionAndActivateObjectEvent(s32 selector,
                                                                 s32 x, s32 z) {
  struct ObjectEvent *objectEvent;

  objectEvent = Engine_ObjectEventLookup(selector);
  if (objectEvent != 0) {
    ObjectDispatch_StartIdle((struct DispatchObject *)objectEvent);
    ObjectEvent_SetMode(objectEvent, 5);
    Engine_ObjectSetPosition(objectEvent, x << 16, objectEvent->y.fixed,
                             z << 16);
    FieldObject_WaitForMove(objectEvent);
    ObjectEvent_SetMode(objectEvent, 1);
  }
}

/// The Moving Sidewalk Stage's attendant, stage 5 of Colosseum Final 3: the
/// first time, Isaac appears and walks against the belt; then the objective,
/// and a place for a friend to cheer.
static void EventScript_TalkToMovingSidewalkAttendant(s32 guideId) {
  s32 i;
  s32 result;
  struct ObjectEvent *objectEvent;

  if (gGameState.warpDest == COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_SOUTH) {
    Colosso_AskDoneCheering(guideId);
  } else {
    Engine_ScriptStart();
    result = Colosso_GreetAtStage(guideId, 5);
    if (result == COLOSSO_GUIDE_DESCRIBE) {
      // Walk against the sidewalk, then show how its current pushes the rider.
      Engine_ScriptSetMessage(MsgColossoCalledMovingSidewalkStage);
      Camera_SetSpeed(PIXELS(3), PIXELS(3) / 8);
      Camera_MoveTo(PIXELS(1080), -1, PIXELS(168), 1);
      Engine_CameraWaitForMove();
      Engine_ScriptWait(30);
      Script_ShowMessage(guideId, 0);
      Script_ShowMessage(guideId, 0);
      Colosso_FadeInWarrior(LOCALID_PARTY_LEADER, 984, 184);
      ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(3) / 2, PIXELS(3) / 4);
      ColosseumNavampaMatch_PositionAndActivateObjectEvent(LOCALID_PARTY_LEADER,
                                                           992, 184);
      ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(3) / 10,
                           PIXELS(3) / 20);
      ColosseumNavampaMatch_PositionObjectEvent(LOCALID_PARTY_LEADER, 1120,
                                                184);
      Engine_ScriptWait(120);
      ObjectEvent_SetAttachedEffect(LOCALID_PARTY_LEADER,
                                    ATTACHED_EFFECT_IN_FRONT |
                                        ATTACHED_EFFECT_ATTACH);
      Engine_ScriptWait(120);
      OverlayObject_ResetMotionFields(LOCALID_PARTY_LEADER);
      Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_STAND);
#if !TBS_PATCH_3
      ObjectEvent_SetAttachedEffect(LOCALID_PARTY_LEADER,
                                    ATTACHED_EFFECT_IN_FRONT |
                                        ATTACHED_EFFECT_REMOVE);
#endif
      ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER,
                            EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
      objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
      for (i = 119; i >= 0; i--) {
        if (objectEvent->x.fixed > PIXELS(992)) {
          objectEvent->x.fixed -= PIXELS(6) / 5;
        }
        Engine_TaskWait(1);
      }
      ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER,
                            EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 60);
      ColosseumNavampaMatch_PositionObjectEvent(LOCALID_PARTY_LEADER, 1120,
                                                184);
      Script_ShowMessage(guideId, 0);
      OverlayObject_ResetMotionFields(LOCALID_PARTY_LEADER);
      gGameState.movementMode = LEADER_MOVEMENT_CLIMB;
      Colosso_RestoreWarrior(LOCALID_PARTY_LEADER);
      Engine_CameraFollowObjectEvent(LOCALID_PARTY_LEADER, 0);
      Colosso_RepeatStageNumber(guideId, 5);
    } else if (result == COLOSSO_GUIDE_OFFER_CHEERING) {
      Engine_ScriptSetMessage(MsgColossoInStageMustTryOutpace);
      Script_ShowMessage(guideId, 0);
    }
    Colosso_OfferCheeringSpot(result, guideId, 5);
    Engine_ScriptEnd();
  }
}

/// The log-rolling area's attendant, stage 6 of Colosseum Final 3: the first
/// time, a pointer traces the log's route; then the objective, and a place for
/// a friend to cheer.
static void EventScript_TalkToLogRollingAttendant(s32 localId) {
  s32 state;

  if (gGameState.warpDest == COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_SOUTH) {
    Colosso_AskDoneCheering(localId);
    return;
  }

  Engine_ScriptStart();
  state = Colosso_GreetAtStage(localId, 6);

  if (state == COLOSSO_GUIDE_DESCRIBE) {
    Engine_ScriptSetMessage(MsgColossoAnotherLogRollingArea);
    Engine_CameraSetSpeed(PIXELS(3), PIXELS(3) / 8);
    Engine_CameraMoveTo(PIXELS(1288), -1, PIXELS(152), 1);
    Engine_CameraWaitForMove();
    Engine_ScriptWait(30);
    Engine_ScriptShowMessage(localId, 0);
    Colosso_ShowPointer(180, 88, 0);
    Engine_ScriptWait(60);
    Engine_ScriptShowMessage(localId, 0);
    Colosso_MovePointer(32, 84, 10);
    Engine_ScriptWait(30);
    Engine_ScriptShowMessage(localId, 0);
    Colosso_MovePointer(96, 84, 30);
    Engine_ScriptWait(60);
    Engine_ScriptShowMessage(localId, 0);
    Colosso_HidePointer();
    Engine_ScriptWait(2);
    Engine_CameraFollowObjectEvent(LOCALID_PARTY_LEADER, 0);
    Colosso_RepeatStageNumber(localId, 6);
  } else if (state == COLOSSO_GUIDE_OFFER_CHEERING) {
    Engine_ScriptSetMessage(MsgColossoHereMustFigureOutHow);
    Engine_ScriptShowMessage(localId, 0);
  }

  Colosso_OfferCheeringSpot(state, localId, 6);
  Engine_ScriptEnd();
}

#include "common/colosso_solo.h"
#include "common/item_equip.h"
#include "common/colosso_task.h"

/// Chooses the lettering animation script and resets its task state.
static void ColosseumNavampaMatch_StartLetteringAnimation(u32 mode,
                                                          u32 parameter) {
  s32 handler;

  gColosso_LetteringMode = mode;
  gColosso_LetteringParameter = (parameter << 4);

  {
    s32 priority = TASK_PRIORITY_MAP;
    SchedulerCallback task = Colosso_UpdateLettering;
    FieldScheduler_AddCallback(task, priority);
  }

  handler = (s32)gColossoLetteringScriptTitle;
  if (mode == COLOSSO_SHAPE_DIGIT) {
    handler = (s32)gColossoLetteringScriptDigit;
  }
  if (mode == COLOSSO_SHAPE_GO) {
    handler = (s32)gColossoLetteringScriptGo;
  }
  if (mode == COLOSSO_SHAPE_RESULT) {
    if (parameter != 0) {
      handler = (s32)gColossoLetteringScriptLose;
    } else {
      handler = (s32)gColossoLetteringScriptWon;
    }
  }

  gColosso_LetteringTimer = 0;
  gColosso_LetteringScript = (s16 *)handler;
  gColosso_ModeMoveTarget = 0;
  gColosso_ModeMoveDuration = 0;
  gColosso_LetteringPosition = 0;
}

/// The lettering a visit opens with: `Lend a Hand` for the friends' turns
/// (mode 0), or for the race `Match 3`, then 3, 2, 1 and `Go!`.
static void ColosseumNavampaMatch_ShowRoundStartLettering(s32 mode) {
  if (mode == 0) {
    Engine_ScriptStart();
    Engine_ScriptOpenScreen();
    Engine_ScriptWaitForScreen();
    Engine_ScriptWait(30);
    Engine_AudioPlayCue(MUS_LEVEL_UP);
    Colosso_LoadLettering(COLOSSO_PICTURE_LEND_A_HAND);
    ColosseumNavampaMatch_StartLetteringAnimation(COLOSSO_SHAPE_TITLE, 0);
    Engine_ScriptWait(120);
    Engine_ScriptEnd();
    return;
  }

  Engine_AudioPlayCue(SE_247);
  Engine_ScriptStart();
  Engine_ScriptOpenScreen();
  Engine_ScriptWaitForScreen();
  {
    s16 *base = gColossoLetteringScriptTitle;
    *(s16 *)((u8 *)base + 30) = (mode * 60);
  }
  Engine_ScriptWait(30);
  Engine_AudioPlayCue(mode + 0x5a);
  Colosso_LoadLettering(mode);
  ColosseumNavampaMatch_StartLetteringAnimation(COLOSSO_SHAPE_TITLE, 0);
  Engine_ScriptWait(120);

  while (AudioCommand_GetStateByte() != 0) {
    Engine_TaskWait(1);
  }

  Engine_AudioPlayCue(AUDIO_CUE_FADE_SLOT3);
  Colosso_LoadLettering(COLOSSO_PICTURE_COUNT_3_2);
  ColosseumNavampaMatch_StartLetteringAnimation(COLOSSO_SHAPE_DIGIT, 0);
  Engine_AudioPlayCue(SE_COLOSSO_COUNTDOWN);
  Engine_ScriptWait(60);
  ColosseumNavampaMatch_StartLetteringAnimation(COLOSSO_SHAPE_DIGIT, 1);
  Engine_AudioPlayCue(SE_COLOSSO_COUNTDOWN);
  Engine_ScriptWait(60);
  Colosso_LoadLettering(COLOSSO_PICTURE_COUNT_1);
  ColosseumNavampaMatch_StartLetteringAnimation(COLOSSO_SHAPE_DIGIT, 0);
  Engine_AudioPlayCue(SE_COLOSSO_COUNTDOWN);
  Engine_ScriptWait(60);
  Colosso_LoadLettering(COLOSSO_PICTURE_GO);
  ColosseumNavampaMatch_StartLetteringAnimation(COLOSSO_SHAPE_GO, 0);
  Engine_AudioPlayCue(SE_COLOSSO_GO);
  Audio_PlayCueFromEventWork();
  Engine_ScriptEnd();
  Engine_GameFlagSet(FLAG_IN_TOWN);
}

/// Unused: a lettering countdown before the match, for a signed number of
/// seconds; nothing calls it.
UNUSED static void ColosseumNavampaMatch_UnusedCountdown(s32 countdown) {
  s32 localId;

  Engine_AudioPlayCue(SE_247);
  Engine_ScriptOpenScreen();
  Engine_ScriptWaitForScreen();
  gColossoLetteringScriptWon[13] = countdown * 60;
  STORE_CONST(gColossoLetteringScriptLose[13]) =
      (countdown < 0 ? -countdown : countdown) * 60;
  if (countdown < 0) {
    Engine_ScriptWait(30);
    Engine_AudioPlayCue(MUS_086);
    Colosso_LoadLettering(COLOSSO_PICTURE_WON_LOSE_FOR_LOSS);
    /* FAKEMATCH(wrapper): called through a cast to the s32 call the bytes
     * show, which sets r1 before r0; as a direct call, 1 of 72 instructions
     * differ in tbs-en. */
    COLOSSEUM_NAVAMPA_MATCH_INITIALIZE_MODE_TASK(3, 1);
    Engine_ScriptWait(-countdown * 60 + 60);
    localId = LOCALID_PARTY_LEADER;
  } else {
    Engine_ScriptWait(30);
    Engine_AudioPlayCue(countdown + 90);
    Colosso_LoadLettering(COLOSSO_PICTURE_WON_LOSE);
    /* FAKEMATCH(wrapper): the same cast as the first arm, which sets r1 before
     * r0; as a direct call, 1 of 72 instructions differ in tbs-en. */
    COLOSSEUM_NAVAMPA_MATCH_INITIALIZE_MODE_TASK(3, 0);
    Engine_ScriptWait(countdown * 60 + 60);
    localId = LOCALID_OPPONENT;
  }
  Engine_ObjectEventShowEmote(localId, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  while (AudioCommand_GetStateByte() != 0) {
    Engine_TaskWait(1);
  }
  Engine_AudioPlayCue(MUS_SILENCE);
  Engine_ScriptWait(30);
  Engine_AudioPlayCue(AUDIO_CUE_FADE_SLOT3);
  Engine_ScriptCloseScreen();
  Engine_ScriptWaitForScreen();
}

#include "common/colosso_marker.h"
#include "common/colosso_item_icon.h"

/// Unused: writes eight hexadecimal digits followed by a terminator; nothing
/// calls it.
UNUSED static void WriteU32AsHex(u8 *hexText, u32 value) {
  s32 digitIndex;

  hexText += 8;
  *hexText = 0;
  hexText--;
  for (digitIndex = 7; digitIndex >= 0; digitIndex--) {
    *hexText = gColossoHexChars[value & 15];
    value >>= 4;
    hexText--;
  }
}

static void ColosseumNavampaMatch_EmptyHook(void) {}

/// Unused: marks the opponent path recorder as finished; nothing calls it.
UNUSED static void ColosseumNavampaMatch_MarkOpponentFinished(void) {
  struct PathRecorder *recorder = (struct PathRecorder *)gMapLocalState;

  recorder->mode = PATH_RECORDER_FINISHED;
}

/// Waits until the opponent path recorder reaches its finished mode.
static void ColosseumNavampaMatch_WaitForOpponentToFinish(void) {
  struct PathRecorder *recorder = (struct PathRecorder *)gMapLocalState;

  while (recorder->mode != PATH_RECORDER_FINISHED) {
    Engine_TaskWait(1);
  }
}

/// Occasionally scatters an effect around the prize being raised at the
/// arena.
static void
ColosseumNavampaMatch_SpawnRandomFinishEffect(union FieldObject *fieldObject) {
  struct ObjectEvent *source = (struct ObjectEvent *)fieldObject;

  s32 position[3];
  u32 randomValue;

  if (source->velocityY >= -255 && source->velocityY <= 255) {
    source->motionFlags = 0;
  }
  randomValue = Engine_RandomNext();
  if (randomValue * 100 >> 16 <= 9) {
    struct ObjectEvent *effect;
    s32 angle;
    s32 radius;

    SET_VECTOR(position, source->x.fixed, source->y.fixed, source->z.fixed);
    angle = Engine_RandomNext();
    radius = Engine_RandomNext();
    Vector_AddPolarOffset(angle << 4, radius, position);
    {
      s32 x = position[0];
      s32 y = position[1];
      s32 z = position[2];

      effect = Engine_ObjectCreate(SPRITE_SPARKLE, x, y, z);
    }
    if (effect != 0) {
      effect->motionFlags = 0;
      Engine_ObjectEventSetSpriteFlags(effect, 0);
      Engine_ObjectSetScript(effect, gColossoRandomEffect);
      ObjectEvent_SetMode(effect, 1);
      ObjectEvent_SetMode(effect, 0);
    }
  }
}

/// Presents the prize above the warrior who took it, with the treasure fanfare,
/// and clears the link.
static s32 ColosseumNavampaMatch_PlaceFinishItemAboveObjectEvent(
    struct ScriptInterpreter *interpreter) {
  struct ObjectEvent *source = (struct ObjectEvent *)interpreter;

  struct ObjectEvent *effect = ObjectEvent_GetById(source->timer);

  Engine_ObjectSetPosition(effect, source->x.fixed,
                           source->y.fixed + PIXELS(36), source->z.fixed);
  effect->motionFlags = 0;
  Engine_ObjectSetScript(effect, gColossoLinkedEffect);
  Engine_AudioPlayCue(MUS_TREASURE_FOUND);
  source->timer = 0;
  return 0;
}

/// Moves to the available finish item, equips it and announces it.
static s32 ColosseumNavampaMatch_CollectFinishEquipment(s32 firstItem,
                                                        s32 secondItem) {
  struct ColossoGaugeWork *gauge = (struct ColossoGaugeWork *)gColossoWork;
  struct ObjectEvent *objectEvent;
  s32 opponentFinishedFirst;
  s32 x;
  s32 z;
  u16 *itemLocalId;
  s16 *linkedItemLocalId;

  opponentFinishedFirst = Engine_GameFlagIsSet(FLAG_ARENA_REACHED);
  objectEvent = ObjectEvent_GetById(gGameState.selectedLocalId);
  if (gauge->finishX < objectEvent->x.fixed) {
    x = gauge->finishX + PIXELS(12);
  } else {
    x = gauge->finishX - PIXELS(12);
  }
  if (opponentFinishedFirst != 0) {
    z = gauge->finishZ + PIXELS(16);
    itemLocalId = &gauge->finishItemLocalIdB;
  } else {
    z = gauge->finishZ - PIXELS(16);
    itemLocalId = &gauge->finishItemLocalIdA;
  }
  linkedItemLocalId = &objectEvent->timer;
  *linkedItemLocalId = *itemLocalId;
  objectEvent->acceleration = PIXELS(1) / 4;
  objectEvent->speed = PIXELS(1);
  Engine_ObjectSetPosition(objectEvent, x, 0, z);
  Engine_GameFlagSet(FLAG_ARENA_REACHED);
  Engine_ObjectSetScript(objectEvent, gColosseumNavampaMatch_PlaceScript);
  while (*linkedItemLocalId != 0) {
    Engine_TaskWait(1);
  }
  if (opponentFinishedFirst == 0) {
    /* FAKEMATCH(wrapper): called through a cast to the s32 call the bytes
     * show, which sets r1 before r0; as a direct call, 1 of 110 instructions
     * differ in tbs-en. */
    COLOSSEUM_NAVAMPA_MATCH_APPLY_MATCHING_SLOT_VALUES(ISAAC, firstItem);
    UiWork_PushValueSlot(firstItem, 2);
  } else {
    /* FAKEMATCH(wrapper): the same cast as the first arm, which sets r1 before
     * r0; as a direct call, 1 of 110 instructions differ in tbs-en. */
    COLOSSEUM_NAVAMPA_MATCH_APPLY_MATCHING_SLOT_VALUES(ISAAC, secondItem);
    UiWork_PushValueSlot(secondItem, 2);
  }
  UiWork_PushValueSlot(gGameState.selectedLocalId, 1);
  Engine_MessageShowCentered(MsgColossoIsaacGotItem, 3);
  ObjectDispatch_WaitForValue16(objectEvent);
  return opponentFinishedFirst;
}

#include "common/colosso_path_opponent.h"
#include "common/colosso_setup.h"
#include "common/colosso_map_state.h"
#include "common/object_findslot.h"

/// Pushes a neighboring object event one cell when its destination and overhead
/// are clear.
static void ColosseumNavampaMatch_PushObjectEventAhead(void) {
  struct ObjectEvent *subject;
  struct ObjectEvent *target;
  struct ObjectEvent *blocker;
  u32 step;
  u32 direction;
  struct FieldPosition position;
  s32 zero;
  s32 subjectHandle;

  subjectHandle = gGameState.selectedLocalId;
  subject = ObjectEvent_GetById(subjectHandle);

  direction = subject->facing >> 12;

  step = gStagedObjectEvent_DirectionSteps[direction];
  position.x = subject->x.fixed + (step & 0xffff0000);
  position.y = subject->y.fixed;
  step <<= 16;
  position.z = subject->z.fixed + step;

  target = MapData_FindSlotAtPosition(&position, subject);
  if (target == 0) {
    return;
  }

  step = gStagedObjectEvent_DirectionSteps[direction];
  position.x = target->x.fixed + (step & 0xffff0000);
  position.y = target->y.fixed;
  step <<= 16;
  position.z = target->z.fixed + step;

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

  target->terrainId = 2;
  zero = 0;

  step = gStagedObjectEvent_DirectionSteps[direction];
  position.x = target->x.fixed + (step & 0xffff0000);
  position.y = target->y.fixed;
  step <<= 16;
  position.z = target->z.fixed + step;

  if (FieldObject_CheckMovementCollision(target, &position) > 0) {
    return;
  }

  ObjectEvent_SetMode(subject, 8);
  Engine_TaskWait(15);

  target->speed = PIXELS(1) / 5;
  target->acceleration = PIXELS(1) / 5;
  Engine_ObjectSetPosition(target, position.x, position.y, position.z);

  subject->speed = PIXELS(1) / 5;
  subject->acceleration = PIXELS(1) / 5;
  Engine_ObjectSetPosition(subject, position.x, position.y, position.z);

  Engine_AudioPlayCue(SE_238);
  FieldObject_WaitForMove(target);
  Engine_AudioPlayCue(SE_STOP_SLOT3);

  target->x.fixed = position.x;
  target->z.fixed = position.z;
  target->velocityX = zero;
  target->velocityZ = zero;

  ObjectEvent_SetMode(subject, 1);
}

#include "common/colosso_occupant.h"

/// The Moving Sidewalk Stage's block where Force has knocked it off the belt,
/// bobbing a pixel up and down.
const s32 gColosseumNavampaMatch_SidewalkBlockOffBelt[] = {
    SC_SET(SW_Y, PIXELS(1)),     SC_WAIT(10), SC_SET(SW_Y, 0), SC_WAIT(10),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// The Wall Wedge's four patterns: each piston's animation, west to east, then
/// the operator's.
const s8 gColosseumNavampaMatch_WallWedgePattern[][6] = {
    {7, 5, 7, 5, 7, 11},
    {4, 6, 4, 6, 4, 10},
    {5, 7, 5, 7, 5, 12},
    {6, 4, 6, 4, 6, 10},
};

// clang-format off
const u16 gColossoEarlySequenceData[] = {
    0, 0, 0x4000, 0x8000, 0x8000, 0x8000, 0xc000, 0,
};

const s32 gColossoMapEventEffect[] = {
    SC_MOVE(PIXELS(1088), 0, PIXELS(184)),
    SC_SYNC,
    SC_MOVE(PIXELS(1130), 0, PIXELS(184)),
    SC_SYNC,
    SC_CALL(ColosseumNavampaMatch_ShowActivatedSidewalkLayer),
    SC_LABEL(1),
    SC_SET(SW_Y, PIXELS(1)),
    SC_WAIT(10),
    SC_SET(SW_Y, 0),
    SC_WAIT(10),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
};

const u8 gColosso_LetteringPaletteOffsets[] = {
#if LANGUAGE == LANGUAGE_JAPANESE || LANGUAGE == LANGUAGE_ENGLISH
    0, 32, 32, 32, 96, 64, 64, 64, 128, 0,
#else
    0, 32, 32, 32, 96, 64, 64, 160, 128, 0,
#endif
};

const s16 gColossoLetteringScriptDigit[] = {
    0x1000, 512, 1, 0x2000, 0, 1, 0x1000, 256,
    6, 0x2000, 16, 6, 0x7fff, 30, 0x2000, 0,
    30, 0x7fff, 30, -1,
};

const s16 gColossoLetteringScriptLose[] = {
    0x1000, 128, 1, 0x2000, 0, 1, 0x1000, 256,
    6, 0x2000, 16, 6, 0x7fff, 60, 0x1000, 384,
    6, 0x7fff, 6, 0x1000, 240, 6, 0x7fff, 6,
    0x1000, 368, 6, 0x7fff, 6, 0x1000, 224, 6,
    0x7fff, 6, 0x2000, 0, 30, 0x1000, 352, 6,
    0x7fff, 6, 0x1000, 208, 6, 0x7fff, 6, 0x1000,
    336, 6, 0x7fff, 6, 0x1000, 192, 6, 0x7fff,
    6, 0x1000, 320, 6, 0x7fff, 6, -1,
};

char gColossoHexChars[20] = "0123456789ABCDEF";

s32 gColossoRandomEffect[] = {
    SC_ADD(SW_Y, -(PIXELS(1) / 4)),
    SC_WAIT(1),
    SC_LOOP(60, SC_TOP),
    SC_FREE,
};

s32 gColossoLinkedEffect[] = {
    SC_SET(SW_HOOK, (s32)ColosseumNavampaMatch_SpawnRandomFinishEffect),
    SC_LABEL(1),
    SC_WAIT(10),
    SC_SET(SW_SCALE_X, 11 * Q16_ONE / 8),
    SC_SET(SW_SCALE_Y, 11 * Q16_ONE / 8),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 3 * Q16_ONE / 2),
    SC_SET(SW_SCALE_Y, 3 * Q16_ONE / 2),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 9 * Q16_ONE / 8),
    SC_SET(SW_SCALE_Y, 9 * Q16_ONE / 8),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, Q16_ONE),
    SC_SET(SW_SCALE_Y, Q16_ONE),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 11 * Q16_ONE / 8),
    SC_SET(SW_SCALE_Y, 11 * Q16_ONE / 8),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 3 * Q16_ONE / 2),
    SC_SET(SW_SCALE_Y, 3 * Q16_ONE / 2),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 9 * Q16_ONE / 8),
    SC_SET(SW_SCALE_Y, 9 * Q16_ONE / 8),
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

/// Pixel step by direction: X in the upper halfword, Z in the lower.
#define DIRECTION_STEP(x, z) ((u32)(x) << 16 | (u16)(z))

u32 gStagedObjectEvent_DirectionSteps[] = {
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

struct MapWarpDest gColosseumNavampaMatchWarpDests[] = {
    { WARP_DEST_DEFAULT, CONDITION_ALWAYS, 88, 0, 104, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { COLOSSEUM_NAVAMPA_MATCH_RACE_FROM_LEND_A_HAND, CONDITION_ALWAYS, 88, 0, 104, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_SOUTH, CONDITION_ALWAYS, 136, 0, 392, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { COLOSSEUM_NAVAMPA_MATCH_AFTER_MATCH_WON, CONDITION_ALWAYS, 88, 0, 104, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { COLOSSEUM_NAVAMPA_MATCH_AFTER_MATCH_LOST, CONDITION_ALWAYS, 88, 0, 104, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { MAP_TABLE_END },
};

u32 gColosseumNavampaMatchWarps[] = {
    MAP_WARPS(ColosseumNavampaMatch),
    MAP_WARP(COLOSSEUM_NAVAMPA_MATCH_LEND_A_HAND_TO_RACE, ColosseumNavampaMatch, COLOSSEUM_NAVAMPA_MATCH_RACE_FROM_LEND_A_HAND),
    MAP_WARP(COLOSSEUM_NAVAMPA_MATCH_TO_COLOSSEUM, Colosseum, COLOSSEUM_FROM_COLOSSEUM_NAVAMPA_MATCH),
    MAP_WARP(COLOSSEUM_NAVAMPA_MATCH_TO_BABI_PALACE, BabiPalace, BABI_PALACE_FROM_COLOSSEUM_NAVAMPA_MATCH),
    MAP_WARP(COLOSSEUM_NAVAMPA_MATCH_TO_BABI_PALACE_LOST, BabiPalace, BABI_PALACE_FROM_COLOSSEUM_NAVAMPA_MATCH),
    MAP_WARPS_END,
};

struct ObjectEventTemplate gColosseumNavampaMatchObjectEventTemplates[] = {
    { 0x0, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(88), 0, PIXELS(104), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_WEST, TALK_FACE_PARTY, 0 },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(216), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x122, CONDITION_ALWAYS, 7,
        PIXELS(152), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x122, CONDITION_ALWAYS, 7,
        PIXELS(216), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(88), 0, PIXELS(168), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(312), 0, PIXELS(184), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(712), 0, PIXELS(184), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1240), 0, PIXELS(136), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1272), 0, PIXELS(168), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1304), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x123, CONDITION_ALWAYS, 7,
        PIXELS(536), 0, PIXELS(184), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x123, CONDITION_ALWAYS, 7,
        PIXELS(568), 0, PIXELS(184), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x123, CONDITION_ALWAYS, 7,
        PIXELS(600), 0, PIXELS(184), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x123, CONDITION_ALWAYS, 7,
        PIXELS(632), 0, PIXELS(184), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x123, CONDITION_ALWAYS, 7,
        PIXELS(664), 0, PIXELS(184), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x123, CONDITION_ALWAYS, 7,
        PIXELS(536), 0, PIXELS(216), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x123, CONDITION_ALWAYS, 7,
        PIXELS(568), 0, PIXELS(216), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x123, CONDITION_ALWAYS, 7,
        PIXELS(600), 0, PIXELS(216), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x123, CONDITION_ALWAYS, 7,
        PIXELS(632), 0, PIXELS(216), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x123, CONDITION_ALWAYS, 7,
        PIXELS(664), 0, PIXELS(216), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(720), 0, PIXELS(216), FACING_WEST, TALK_KEEP_FACING, 0 },
    { SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(792), 0, PIXELS(194), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x126, CONDITION_ALWAYS, 7,
        PIXELS(1080), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7,
        PIXELS(1208), 0, PIXELS(176), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(136), 0, PIXELS(408), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(152), 0, PIXELS(312), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(376), 0, PIXELS(312), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(600), 0, PIXELS(312), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(856), 0, PIXELS(312), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1080), 0, PIXELS(312), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1304), 0, PIXELS(312), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1512), 0, PIXELS(176), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1512), 0, PIXELS(208), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x118, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x118, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { 0x118, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { MAP_TABLE_END },
};

s32 gColossoMapTaskStatus = 0;

s32 gColossoMapTaskState = 0;

struct MapEvent gColosseumNavampaMatchEvents[] = {
    { COORD_EVENT_WALKING, 90, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_CheckAhead) },
    { COORD_EVENT_WALKING, 91, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_PushWeightOrCheckAhead) },
    { MAP_EVENT_COORD, 11, FLAG_SIDEWALK_BLOCK_KNOCKED_OFF, EVENT_SCRIPT(EventScript_RideMovingSidewalk) },
    { COORD_EVENT_WALKING, 20, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_PushWeight) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_SCALES_WEIGHT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SetWeightOnScales) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_STONE, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_MoveStone) },
    { COORD_EVENT_WALKING, 22, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_PushRock) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_ROCK, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_IgnoreMove) },
    { COORD_EVENT_WALKING, 23, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_RollLongLog) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_LOG_AREA_STONE_A, FLAG_FIRST_LOG_AREA_STONE_SETTLED, EVENT_SCRIPT(EventScript_SettleStones) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_LOG_AREA_STONE_B, FLAG_FIRST_LOG_AREA_STONE_SETTLED + 1, EVENT_SCRIPT(EventScript_SettleStones) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_LOG_AREA_STONE_C, FLAG_FIRST_LOG_AREA_STONE_SETTLED + 2, EVENT_SCRIPT(EventScript_SettleStones) },
    { MAP_EVENT_COORD, 15, FLAG_ON_SCALES, EVENT_SCRIPT(EventScript_StepOnScales) },
    { MAP_EVENT_COORD, 16, IF_FLAG_SET(FLAG_ON_SCALES), EVENT_SCRIPT(EventScript_StepOffScales) },
    { MAP_EVENT_COORD, 50, FLAG_MAP_TEMP_10, EVENT_SCRIPT(EventScript_ReachArena) },
    { BG_EVENT(BG_EVENT_CHEST), 101, FLAG_MAPSEC_TEMP_6A, FIND_ITEM(ITEM_SMOKE_BOMB) },
    { BG_EVENT(BG_EVENT_CHEST), 102, FLAG_MAPSEC_TEMP_6B, FIND_ITEM(ITEM_NUT) },
    { BG_EVENT(BG_EVENT_CHEST) | MAP_EVENT_FLAG_FACING | FACING_WEST, 103, FLAG_MAPSEC_TEMP_6C, FIND_ITEM(ITEM_SLEEP_BOMB) },
    { PSYNERGY_PHASE_ON_OBJ_EVENT(ABILITY_FROST, PSYNERGY_PHASE_EFFECT_ACTS), LOCALID_FROST_TARGET, FLAG_SCALES_FROZEN, EVENT_SCRIPT(EventScript_FreezeOnScales) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_GROWTH), LOCALID_GROWTH_PLANT, FLAG_PLANT_GROWN, EVENT_SCRIPT(EventScript_GrowPlant) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_HALT), LOCALID_WALL_OPERATOR, FLAG_WALL_WEDGE_HALTED, EVENT_SCRIPT(EventScript_HaltWall) },
    { PSYNERGY_PHASE_ON_OBJ_EVENT(ABILITY_FORCE, PSYNERGY_PHASE_EFFECT_ACTS), LOCALID_SIDEWALK_BLOCK, FLAG_SIDEWALK_BLOCK_KNOCKED_OFF, EVENT_SCRIPT(EventScript_ForceSidewalkBlock) },
    { MAP_EVENT_RAISED, 4, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_PassNorthWallOpening) },
    { MAP_EVENT_RAISED, 5, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_PassSouthWallOpening) },
    { MAP_EVENT_TALK, LOCALID_GUIDE, CONDITION_ALWAYS, EVENT_SCRIPT(Colosso_AskReadyToRace) },
    { MAP_EVENT_TALK, LOCALID_SCALES_ATTENDANT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToScalesAttendant) },
    { MAP_EVENT_TALK, LOCALID_STEPPING_STONE_ATTENDANT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToSteppingStoneAttendant) },
    { MAP_EVENT_TALK, LOCALID_WALL_WEDGE_ATTENDANT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToWallWedgeAttendant) },
    { MAP_EVENT_TALK, LOCALID_FREE_CLIMB_ATTENDANT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToFreeClimbAttendant) },
    { MAP_EVENT_TALK, LOCALID_MOVING_SIDEWALK_ATTENDANT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToMovingSidewalkAttendant) },
    { MAP_EVENT_TALK, LOCALID_LOG_ROLLING_ATTENDANT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToLogRollingAttendant) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_GUIDE, CONDITION_ALWAYS, MSGNO(MsgColossoRiverThisYearsFinalsAreIncredibleThis) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SCALES_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoLogTheStreetVendorsCornOnThe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STEPPING_STONE_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoLogICantSeeAnythingWhenIm) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WALL_WEDGE_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoLogItsNotVeryExcitingWorkBut) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_FREE_CLIMB_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoLogStandingForLongPeriodsOfTime) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_MOVING_SIDEWALK_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoLogIWasUpAllNightTrying) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_LOG_ROLLING_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoLogSomeTouristToldMeILook) },
    { MAP_EVENT_RAISED, 99, CONDITION_ALWAYS, EVENT_SCRIPT(Colosso_TakePlaceForMatch) },
    { MAP_EVENTS_END },
};

/// Navampa after the last battle (ColosseumNavampaMatch_PlayPostBattleReunion): his
/// sprite grows taller and narrower and back, over and over.
s32 gColosseumNavampaMatch_OpponentAfterBattle[] = {
    SC_ADD(SW_SCALE_Y, 512),
    SC_ADD(SW_SCALE_X, -256),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, 512),
    SC_ADD(SW_SCALE_X, -256),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, 512),
    SC_ADD(SW_SCALE_X, -256),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_Y, -512),
    SC_ADD(SW_SCALE_X, 256),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_Y, -512),
    SC_ADD(SW_SCALE_X, 256),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_Y, -512),
    SC_ADD(SW_SCALE_X, 256),
    SC_WAIT(10),
    SC_LOOP(SC_FOREVER, SC_TOP),
    SC_END,
};

s32 gColosseumNavampaMatch_LeaderActionA[] = {
    SC_ADD(SW_SCALE_Y, 512),
    SC_ADD(SW_SCALE_X, -256),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, 512),
    SC_ADD(SW_SCALE_X, -256),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, 512),
    SC_ADD(SW_SCALE_X, -256),
    SC_WAIT(6),
    SC_ADD(SW_SCALE_Y, -512),
    SC_ADD(SW_SCALE_X, 256),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_Y, -512),
    SC_ADD(SW_SCALE_X, 256),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_Y, -512),
    SC_ADD(SW_SCALE_X, 256),
    SC_WAIT(12),
    SC_LOOP(SC_FOREVER, SC_TOP),
    SC_END,
};

s32 gColosseumNavampaMatch_LeaderActionB[] = {
    SC_SET(SW_SCALE_X, Q16_ONE),
    SC_SET(SW_SCALE_Y, Q16_ONE),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_Y, 3 * Q16_ONE / 64),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, 3 * Q16_ONE / 64),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, 3 * Q16_ONE / 64),
    SC_WAIT(16),
    SC_ADD(SW_SCALE_Y, -(3 * Q16_ONE / 64)),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_Y, -(3 * Q16_ONE / 64)),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_Y, -(3 * Q16_ONE / 64)),
    SC_WAIT(32),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};

s32 gColosseumNavampaMatch_LeaderActionC[] = {
    SC_SET(SW_SCALE_X, Q16_ONE),
    SC_SET(SW_SCALE_Y, Q16_ONE),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_Y, 3 * Q16_ONE / 64),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, 3 * Q16_ONE / 64),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, 3 * Q16_ONE / 64),
    SC_WAIT(26),
    SC_ADD(SW_SCALE_Y, -(3 * Q16_ONE / 64)),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_Y, -(3 * Q16_ONE / 64)),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_Y, -(3 * Q16_ONE / 64)),
    SC_WAIT(52),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};

s32 gColossoMultiPhaseData[] = {
    SC_SET(SW_SCALE_X, Q16_ONE),
    SC_SET(SW_SCALE_Y, Q16_ONE),
    SC_END,
};

s32 gColossoParticleKinds[] = {
    SC_CALL(ColosseumNavampaMatch_AdvanceParticleMotion),
    SC_WAIT(1),
    SC_LOOP(60, SC_TOP),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

s16 gColosso_LetteringSlot = -1;

s16 gColossoLetteringScriptTitle[] = {
#if LANGUAGE == LANGUAGE_JAPANESE
    0x4000, -188, 2048, 0x1000, 256, 1, 0x2000, 16,
#else
    0x4000, -188, 2048, 0x1000, 384, 1, 0x2000, 16,
#endif
    1, 0x7fff, 14, 0x3000, 0, 20, 0x7fff, 60,
    0x3000, 2048, 20, 0x7fff, 60, -1,
};

s16 gColossoLetteringScriptGo[] = {
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

s16 gColossoLetteringScriptWon[] = {
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
    SC_CALL(ColosseumNavampaMatch_PlaceFinishItemAboveObjectEvent),
    SC_WAIT(3),
    SC_ANIMATION(10),
    SC_WAIT(150),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_ANIMATION(1),
    SC_END,
};

s32 gColosseumNavampaMatch_PlaceScript[] = {
    SC_ANIMATION(2),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(6),
    SC_CALL(ColosseumNavampaMatch_PlaceFinishItemAboveObjectEvent),
    SC_WAIT(3),
    SC_ANIMATION(28),
    SC_WAIT(150),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_ANIMATION(1),
    SC_END,
};

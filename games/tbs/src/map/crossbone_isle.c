/// @file
/// Crossbone Isle, an island reached from the world map, and the main line of
/// the cave beneath it, in three map sections by their area names: Crossbone
/// Isle, Crossbone Isle Cave and the Pirate Ship.
///
/// No one on the island speaks: it is a run of Psynergy puzzles and battles
/// that ends with Deadbeard on the Pirate Ship. Its maps are met in this order:
///
/// - Crossbone Isle (MapId_CrossboneIsle), the island itself, reached from the
///   world map (WORLD_MAP_TO_CROSSBONE_ISLE; CROSSBONE_ISLE_TO_WORLD_MAP leads
///   back to it). Whirlwind blows two ivies away for good (flags from
///   FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY, the first also opening a cell;
///   the second's and a fourth's map events are unused), Growth grows three
///   plants so they can be crossed, for this visit only (flags from
///   FLAG_CROSSBONE_ISLE_FIRST_PLANT_GROWN; the first plant's map event is
///   unused), two logs can be pushed, and a Nut circles until Catch takes it
///   (FLAG_CROSSBONE_ISLE_NUT_CAUGHT).
///   CROSSBONE_ISLE_TO_CROSSBONE_ISLE_CAVE_ENTRANCE leads down into the cave.
/// - Crossbone Isle Cave (MapId_CrossboneIsleCaveEntrance), the cave's mouth,
///   with only its two warps.
/// - Crossbone Isle Cave (MapId_CrossboneIsleFirstFloor to
///   MapId_CrossboneIsleNinthFloor), the nine floors the cave descends through,
///   all on one layout, each entered from the east
///   (CORRIDOR_FROM_PREVIOUS_ROOM) and left to the west
///   (CORRIDOR_TO_NEXT_ROOM). Every room numbers its doors alike, and
///   map_warps.h names them room by room
///   (CROSSBONE_ISLE_FIRST_FLOOR_FROM_CROSSBONE_ISLE_CAVE_ENTRANCE and
///   CROSSBONE_ISLE_FIRST_FLOOR_TO_CROSSBONE_ISLE_SECOND_FLOOR in the first).
///   Each has two doors in its north wall into a side room of its own. The
///   first time the party walks into the east door (EventScript_OpenEastDoor),
///   the monsters behind it attack: ENCOUNTER_ZONE_CROSSBONE_ISLE_FIRST_GUARDS
///   for the first four rooms and ENCOUNTER_ZONE_CROSSBONE_ISLE_LAST_GUARDS for
///   the next five, one group per room. Back from the battle
///   (WARP_DEST_AFTER_BATTLE) the door opens and the party walks through, and
///   from then on it simply opens (one flag per room from
///   FLAG_CROSSBONE_ISLE_FIRST_EAST_DOOR_MONSTERS_DEFEATED); the west door
///   always opens. Between the doors stands a pillar that, pushed or moved with
///   Move to column 40, sinks into the floor and opens the corridor (one flag
///   per room from FLAG_CROSSBONE_ISLE_FIRST_PASSAGE_OPENED). In the first
///   floor a chest is a mimic (FLAG_CROSSBONE_ISLE_FIRST_FLOOR_MIMIC_BEATEN
///   once beaten). The side rooms of the first eight floors are in
///   crossbone_isle_stepping_stones.c and crossbone_isle_keys.c.
/// - Crossbone Isle Cave (MapId_CrossboneIsleFloatingLogs), the ninth floor's
///   side room: twelve logs floating on green water, which drift with the
///   leader standing on them until another log or the shore stops them, and
///   hops from log to log, lead to its four chests.
/// - Crossbone Isle Cave (MapId_CrossboneIslePier), the ledge past the ninth
///   floor where a wooden pier leads out to the Pirate Ship.
/// - Pirate Ship (MapId_PirateShip), at the end. Stepping on coord event 10
///   (EventScript_FightDeadbeard) sets off the battle with Deadbeard, the sixth
///   group of ENCOUNTER_ZONE_CROSSBONE_ISLE_LAST_GUARDS; when the party comes
///   back (WARP_DEST_AFTER_BATTLE) FLAG_CROSSBONE_ISLE_DEADBEARD_DEFEATED is
///   set and a chest with the Demon Mail appears. A second chest is a mimic
///   (FLAG_PIRATE_SHIP_MIMIC_BEATEN once beaten).
///
/// Map_Initialize restores each map as its flags say: the ivy and plants on the
/// island, the sliding logs' footprints, the corridor's sunk pillars and the
/// chests the mimics leave.
///
/// Open questions: none.

#include "save_block.h"
#include "version.h"
#include "heap_slots.h"
#include "unit/import.h"
#include "unit/map/crossbone_isle.h"
#include "unit/map/common/object_orbit.h"
#include "unit/map/common/effect_spawn.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/field_services.h"
#include "unit/field/field.h"
#include "unit/runtime/runtime.h"
#include "battle.h"
#include "field.h"
#include "menu.h"
#include "system.h"
#include "types.h"
#include "script_layer.h"
#include "map_tables.h"
#include "pushed_object.h"
#include "field_effect.h"
#include "debug.h"
#include "game.h"
#include "graphics.h"
#include "minigame.h"
#include "sound.h"
#include "edition.h"
#include "field_service.h"
#include "call.h"
#include "map_ids.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"
#include "object_event.h"
#include "fixed_math.h"
#include "flags.h"
#include "songs.h"
#include "script.h"

/// A chest searched while facing the given way.
#define BG_EVENT_CHEST_FACING(facing)                                          \
  (BG_EVENT(BG_EVENT_CHEST) | MAP_EVENT_FLAG_FACING | (facing))

/// Crossbone Isle's placed object events, numbered in the order of
/// gCrossboneIsle_IsleObjectEventTemplates.
enum CrossboneIsleLocalId {
  /// The Nut that circles over the island until Catch takes it
  /// (FLAG_CROSSBONE_ISLE_NUT_CAUGHT).
  /// ★ Named from the game: "Nut" (MsgItemName), the item EventScript_CatchNut
  /// gives
  LOCALID_ISLE_NUT = LOCALID_FIRST_PLACED,
  /// The two logs a push rolls (EventScript_PushIsleLog).
  /// ★ Named from what the player sees: two logs lying on the island's
  /// grass (its render)
  LOCALID_FIRST_ISLE_LOG,
  /// The four plants Growth grows, crossable once grown for the rest of the
  /// visit (four flags from FLAG_CROSSBONE_ISLE_FIRST_PLANT_GROWN). The first
  /// is placed at the map's origin, off stage
  /// (EventScript_UnusedGrowFirstPlant).
  /// ★ Named from what the player sees: small sprouts on the cliff ledges
  /// (its render)
  LOCALID_FIRST_PLANT = LOCALID_FIRST_ISLE_LOG + 2,
  /// The ivy Whirlwind blows away, one flag each from
  /// FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY. Only three rows place one, the
  /// second at the map's origin, off stage
  /// (EventScript_UnusedBlowAwaySecondIvy):
  /// the fourth ivy's map event and CrossboneIsle_RestoreIvy name
  /// LOCALID_FIRST_IVY + 3, which the table leaves unplaced.
  /// ★ Named from what the player sees: leafy ivy growing against the cliffs
  /// (its render)
  LOCALID_FIRST_IVY = LOCALID_FIRST_PLANT + 4
};

/// Growth has grown Crossbone Isle's first plant, until the party leaves the
/// island's map: four map temp flags from this one, one a plant.
/// ★ Named from what the player sees: small sprouts on the cliff ledges (its
/// render)
enum { FLAG_CROSSBONE_ISLE_FIRST_PLANT_GROWN = FLAG_MAP_TEMP_40 };

/// The twelve floating logs of the ninth floor's side room
/// (MapId_CrossboneIsleFloatingLogs), numbered in the order of
/// gCrossboneIsle_FloatingLogsObjectEventTemplates, from the first to the
/// twelfth, with their shapes: a log three cells wide slides north and south,
/// and one three cells tall slides east and west.
enum CrossboneIsleFloatingLogsLocalId {
  /// ★ Named from what the player sees: yellow logs floating on green water
  /// (its render)
  LOCALID_FIRST_LOG = LOCALID_FIRST_PLACED, ///< three cells wide
  /// Three cells wide. ★ Named from what the player sees: a yellow log lying
  /// east-west in the west pool, at its south shore (its render)
  LOCALID_SECOND_LOG,
  /// Three cells wide. ★ Named from what the player sees: a yellow log lying
  /// east-west in the east pool, south of its middle (its render)
  LOCALID_THIRD_LOG,
  /// Three cells tall. ★ Named from what the player sees: a yellow log lying
  /// north-south in the east pool, by its east wall (its render)
  LOCALID_FOURTH_LOG,
  /// Three cells tall. ★ Named from what the player sees: a yellow log lying
  /// north-south in the middle of the east pool (its render)
  LOCALID_FIFTH_LOG,
  /// Three cells tall. ★ Named from what the player sees: a yellow log lying
  /// north-south in the east pool, at its north-east corner (its render)
  LOCALID_SIXTH_LOG,
  /// Three cells tall. ★ Named from what the player sees: a yellow log lying
  /// north-south in the west pool, by its east wall (its render)
  LOCALID_SEVENTH_LOG,
  /// Three cells tall. ★ Named from what the player sees: a yellow log lying
  /// north-south in the east pool, at its north-west corner (its render)
  LOCALID_EIGHTH_LOG,
  /// Three cells tall. ★ Named from what the player sees: a yellow log lying
  /// north-south in the west pool, by its east wall, at its north end (its
  /// render)
  LOCALID_NINTH_LOG,
  /// Three cells wide. ★ Named from what the player sees: a yellow log lying
  /// east-west in the west pool, at its south-west corner (its render)
  LOCALID_TENTH_LOG,
  /// Three cells wide. ★ Named from what the player sees: a yellow log lying
  /// east-west in the west pool, at its south shore (its render)
  LOCALID_ELEVENTH_LOG,
  /// Three cells tall. ★ Named from what the player sees: a yellow log lying
  /// north-south in the west pool, by its east wall, at its south end (its
  /// render)
  LOCALID_TWELFTH_LOG
};

/// The corridor rooms' one placed object event
/// (gCrossboneIsle_CorridorObjectEventTemplates).
enum CrossboneIsleCorridorLocalId {
  /// The pillar between the doors that sinks into the floor at column 40.
  /// ★ Named from what the player sees: a stone pillar between the two doors in
  /// the north wall (its render)
  LOCALID_SINKING_PILLAR = LOCALID_FIRST_PLACED
};

/// The corridor rooms' warp destinations, which every room numbers alike, so
/// the code they share names them here; map_warps.h names each room's own
/// (CROSSBONE_ISLE_FIRST_FLOOR_FROM_CROSSBONE_ISLE_CAVE_ENTRANCE to
/// CROSSBONE_ISLE_FIRST_FLOOR_FROM_CROSSBONE_ISLE_STEPPING_STONES_WEST in the
/// first).
enum {
  /// At the east end, from the room before (the cave's mouth,
  /// CrossboneIsleCaveEntrance,
  /// for the first).
  CORRIDOR_FROM_PREVIOUS_ROOM = 1,
  /// At the west end, from the room after (CrossboneIslePier for the last).
  CORRIDOR_FROM_NEXT_ROOM = 2,
  CORRIDOR_FROM_SIDE_ROOM_EAST = 3, ///< out of the side room by the east door
  CORRIDOR_FROM_SIDE_ROOM_WEST = 4  ///< out of the side room by the west door
};

/// The corridor rooms' warps, numbered as their warp destinations.
enum {
  CORRIDOR_TO_PREVIOUS_ROOM = 1,
  CORRIDOR_TO_NEXT_ROOM = 2,
  CORRIDOR_TO_SIDE_ROOM_EAST = 3, ///< the east door (EventScript_OpenEastDoor)
  CORRIDOR_TO_SIDE_ROOM_WEST = 4  ///< the west door (EventScript_OpenWestDoor)
};

/// The warp destination a battle returns the party to: in a corridor room after
/// the monsters behind its east door (EventScript_OpenEastDoor), and on the
/// Pirate Ship after Deadbeard (EventScript_FightDeadbeard).
enum { WARP_DEST_AFTER_BATTLE = 5 };

static void CrossboneIsle_RestoreSunkPillar(void);
static void CrossboneIsle_OpenEastDoorAfterBattle(void);
static void CrossboneIsle_RestoreIvy(void);
static void CrossboneIsle_DrawLogPuzzleFootprints(void);

#include "common/object_staged_object_event.h"

#include "common/effect_create.h"
#include "common/effect_spawn.h"

/// Unused: an empty function nothing calls.
UNUSED static void CrossboneIsle_UnusedCallback(void) {}

const struct MapWarpDest *Map_GetWarpDests(void) {
  s16 mapId = gSaveBlock.mapId;

  if (mapId == (s32)&MapId_CrossboneIsle) {
    return gCrossboneIsle_IsleWarpDests;
  }
  if (mapId == (s32)&MapId_CrossboneIsleCaveEntrance) {
    return gCrossboneIsle_CaveEntranceWarpDests;
  }
  if (mapId == (s32)&MapId_CrossboneIsleFloatingLogs) {
    return gCrossboneIsle_FloatingLogsWarpDests;
  }
  if (mapId == (s32)&MapId_CrossboneIslePier) {
    return gCrossboneIsle_PierWarpDests;
  }
  if (mapId == (s32)&MapId_PirateShip) {
    return gCrossboneIsle_PirateShipWarpDests;
  }
  return &gCrossboneIsle_PirateShipWarpDests[2];
}

const struct MapWarpRegion *Map_GetWarpRegions(void) { return NULL; }

const u32 *Map_GetWarps(void) { return gCrossboneIsleWarps; }

const struct ObjectEventTemplate *Map_GetObjectEventTemplates(void) {
  s16 mapId = gSaveBlock.mapId;

  if (mapId == (s32)&MapId_CrossboneIsle) {
    return gCrossboneIsle_IsleObjectEventTemplates;
  }
  if (mapId == (s32)&MapId_CrossboneIsleFloatingLogs) {
    return gCrossboneIsle_FloatingLogsObjectEventTemplates;
  }
  if (mapId <= (s32)&MapId_CrossboneIsleNinthFloor &&
      mapId >= (s32)&MapId_CrossboneIsleFirstFloor) {
    return gCrossboneIsle_CorridorObjectEventTemplates;
  }
  return gCrossboneIsle_NoObjectEventTemplates;
}

#include "common/effect_advance.h"

/// Emits a ring of dust around a corridor room's pillar as it sinks.
static void CrossboneIsle_SpawnPillarMovedBurst(s32 objectId) {
  struct ObjectEvent *objectEvent;
  struct Vector3 velocity;
  struct EffectOptions options;
  u32 angleStep;
  s32 angle;
  s32 velocityX;
  s32 velocityZ;

  objectEvent = ObjectEvent_GetById(objectId);
  options.update = FieldEffect_AdvanceMotion;
  for (angleStep = 0; angleStep <= 16; angleStep += 2) {
    angle = angleStep << 12;
    velocity.x = Trig_Cos(angle);
    velocity.y = 0;
    velocityZ = Trig_Sin(angle);
    velocityX = velocity.x;
    velocity.z = velocityZ;
    velocityX += Math_Divide(velocityX, 3);
    velocity.x = velocityX;
    FieldEffect_Spawn(objectEvent->x.fixed, objectEvent->y.fixed,
                      objectEvent->z.fixed, velocityX, velocity.y, velocityZ,
                      EFFECT_USE_UPDATE | EFFECT_SCRIPT_MEDIUM, &options);
  }
}

/// Stepping on coord event 10 on the Pirate Ship: Deadbeard attacks (the sixth
/// group of ENCOUNTER_ZONE_CROSSBONE_ISLE_LAST_GUARDS), and the party comes
/// back to the ship at WARP_DEST_AFTER_BATTLE.
static void EventScript_FightDeadbeard(void) {

  SCRIPT_START();
  FieldScript_SetNextDestination(gSaveBlock.mapId, WARP_DEST_AFTER_BATTLE);
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  Field_RequestBattleFromEncounterTable(
      ENCOUNTER_ZONE_CROSSBONE_ISLE_LAST_GUARDS, 5);
  SCRIPT_END();
}

const struct MapEvent *Map_GetEvents(void) {
  s16 mapId = gSaveBlock.mapId;

  if (mapId == (s32)&MapId_CrossboneIsle) {
    return gCrossboneIsle_IsleEvents;
  }
  if (mapId == (s32)&MapId_CrossboneIsleCaveEntrance) {
    return gCrossboneIsle_CaveEntranceEvents;
  }
  if (mapId == (s32)&MapId_CrossboneIsleFloatingLogs) {
    return gCrossboneIsle_FloatingLogsEvents;
  }
  if (mapId == (s32)&MapId_CrossboneIslePier) {
    return gCrossboneIsle_PierEvents;
  }
  if (mapId == (s32)&MapId_PirateShip) {
    return gCrossboneIsle_PirateShipEvents;
  }
  return gCrossboneIsle_CorridorEvents;
}

/// Catch on the Nut circling on Crossbone Isle: the party gets it
/// (FLAG_CROSSBONE_ISLE_NUT_CAUGHT).
static void EventScript_CatchNut(void) {
  SCRIPT_START();
  Import_ObjectEventSetPosition(LOCALID_ISLE_NUT, 0, 0);
  GameFlag_Set(FLAG_CROSSBONE_ISLE_NUT_CAUGHT);
  FieldEffect_StartEffectObject22(ITEM_NUT, 3);
  PartyInventory_GiveItem(ITEM_NUT, 0);
  SCRIPT_END();
}

/// Restores each map's puzzles from its flags: the sliding logs'
/// footprints in CrossboneIsleFloatingLogs; on the Pirate Ship the mimic's
/// chest and, after Deadbeard, the Demon Mail's; on Crossbone Isle the ivy,
/// plants and Nut; in the corridor rooms the mimic's chest, the sunk pillars
/// and the door opened after a battle.
s32 Map_Initialize(void) {
  struct ObjectEvent *objectEvent;
  s32 mapId;
  s32 current;
  s32 first;

  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  // CrossboneIsleFloatingLogs's sliding logs restore their map footprints
  // alone.
  mapId = gSaveBlock.mapId;
  if (mapId == (s32)&MapId_CrossboneIsleFloatingLogs) {
    CrossboneIsle_DrawLogPuzzleFootprints();
    return 0;
  }
  if (mapId == (s32)&MapId_PirateShip) {
    if (GameFlag_Get(FLAG_PIRATE_SHIP_MIMIC_BEATEN) == 0) {
      MapGrid_CopyCellAttributes(0, 3, 1, 1, 13, 40);
      MapGrid_CopyCellAttributes(0, 2, 1, 1, 15, 40);
      TreasureSpot_SetPosition(101, PIXELS(216), PIXELS(648));
    }
    if (gSaveBlock.mapId == mapId) {
      if (gSaveBlock.warpDest != WARP_DEST_AFTER_BATTLE) {
        if (GameFlag_Get(FLAG_CROSSBONE_ISLE_DEADBEARD_DEFEATED) == 0) {
          return 0;
        }
      }
      GameFlag_Set(FLAG_CROSSBONE_ISLE_DEADBEARD_DEFEATED);
      MapGrid_CopyCellAttributes(0, 1, 1, 1, 13, 30);
      TreasureSpot_SetPosition(100, PIXELS(216), PIXELS(488));
      return 0;
    }
  }
  current = gSaveBlock.mapId;
  if (current == (s32)&MapId_CrossboneIsle) {
    CrossboneIsle_RestoreIvy();
    ObjectEvent_Get(LOCALID_ISLE_NUT)->targetX = PIXELS(129);
    Map_RedrawObjectEventFootprint(LOCALID_FIRST_ISLE_LOG);
    Map_RedrawObjectEventFootprint(LOCALID_FIRST_ISLE_LOG + 1);
    if (GameFlag_Get(FLAG_CROSSBONE_ISLE_FIRST_PLANT_GROWN) != 0) {
      objectEvent = ObjectEvent_Get(LOCALID_FIRST_PLANT);
      if (objectEvent != 0) {
        objectEvent->collisionFlags = 0;
        Object_SelectAnimation(objectEvent, 4);
        ObjectEvent_SetSpriteFlags(objectEvent, 0);
      }
      Field_SetMetatileBehaviorAt(0, PIXELS(304), PIXELS(368), MB_CLIMB);
    }
    if (GameFlag_Get(FLAG_CROSSBONE_ISLE_FIRST_PLANT_GROWN + 1) != 0) {
      objectEvent = ObjectEvent_Get(LOCALID_FIRST_PLANT + 1);
      if (objectEvent != 0) {
        objectEvent->collisionFlags = 0;
        Object_SelectAnimation(objectEvent, 4);
        ObjectEvent_SetSpriteFlags(objectEvent, 0);
      }
      Field_SetMetatileBehaviorAt(0, PIXELS(80), PIXELS(368), MB_CLIMB);
    }
    if (GameFlag_Get(FLAG_CROSSBONE_ISLE_FIRST_PLANT_GROWN + 2) != 0) {
      objectEvent = ObjectEvent_Get(LOCALID_FIRST_PLANT + 2);
      if (objectEvent != 0) {
        objectEvent->collisionFlags = 0;
        Object_SelectAnimation(objectEvent, 4);
        ObjectEvent_SetSpriteFlags(objectEvent, 0);
      }
      Field_SetMetatileBehaviorAt(0, PIXELS(96), PIXELS(336), MB_CLIMB);
    }
    if (GameFlag_Get(FLAG_CROSSBONE_ISLE_FIRST_PLANT_GROWN + 3) != 0) {
      objectEvent = ObjectEvent_Get(LOCALID_FIRST_PLANT + 3);
      if (objectEvent != 0) {
        objectEvent->collisionFlags = 0;
        Object_SelectAnimation(objectEvent, 4);
        ObjectEvent_SetSpriteFlags(objectEvent, 0);
      }
      Field_SetMetatileBehaviorAt(0, PIXELS(144), PIXELS(320), MB_CLIMB);
      Field_SetMetatileBehaviorAt(0, PIXELS(752), PIXELS(320), MB_CLIMB);
    }
    if (GameFlag_Get(FLAG_CROSSBONE_ISLE_NUT_CAUGHT) != 0) {
      return 0;
    }
    Map_FloatNut(LOCALID_ISLE_NUT);
    return 0;
  }
  // The nine corridor rooms (CrossboneIsleFirstFloor to
  // CrossboneIsleNinthFloor) share their door battle, sunk pillar and return
  // warp destination.
  first = (s32)&MapId_CrossboneIsleFirstFloor;
  if (current == first) {
    if (GameFlag_Get(FLAG_CROSSBONE_ISLE_FIRST_FLOOR_MIMIC_BEATEN) == 0) {
      MapGrid_CopyCellAttributes(0, 0, 1, 1, 37, 10);
      TreasureSpot_SetPosition(100, PIXELS(600), PIXELS(168));
    }
  }
  current = gSaveBlock.mapId;
  if (current >= first) {
    if (current <= (s32)&MapId_CrossboneIsleNinthFloor) {
      CrossboneIsle_RestoreSunkPillar();
      if (gSaveBlock.warpDest == WARP_DEST_AFTER_BATTLE) {
        CrossboneIsle_OpenEastDoorAfterBattle();
      }
    }
  }
  return 0;
}

/// In a corridor room of Crossbone Isle Cave whose pillar has been sunk, it
/// stays sunk in its place.
static void CrossboneIsle_RestoreSunkPillar(void) {
  struct ObjectEvent *objectEvent;
  s32 cleared;

  objectEvent = ObjectEvent_Get(LOCALID_SINKING_PILLAR);
  cleared = GameFlag_Get(
      (gSaveBlock.mapId + (FLAG_CROSSBONE_ISLE_FIRST_PASSAGE_OPENED -
                           (s32)&MapId_CrossboneIsleFirstFloor)));
  if (cleared != 0) {
    OBJECT_POSITION_SET(LOCALID_SINKING_PILLAR, 650, 168);
    objectEvent->y.fixed = -PIXELS(32);
    ObjectEvent_SetSpriteFlags(ObjectEvent_Get(LOCALID_SINKING_PILLAR), 0);
    ObjectMotion_SetActionVariant(LOCALID_SINKING_PILLAR, 3);
    objectEvent->motionFlags = 0;
    objectEvent->priorityFlags |= OBJ_EVENT_PRIORITY_UNDERFOOT;
    MapGrid_CopyCellAttributes(42, 10, 1, 1, 40, 10);
  } else {
    ObjectEvent_GetById(LOCALID_SINKING_PILLAR)->motionFlags = cleared;
  }
}

/// Move on a corridor room's pillar in Crossbone Isle Cave, and after each
/// push: once it reaches column 40 it sinks into the floor in a ring of dust,
/// and the corridor is open (a flag per room from
/// FLAG_CROSSBONE_ISLE_FIRST_PASSAGE_OPENED).
static void EventScript_MovePillar(void) {
  struct ObjectEvent *pillar;
  s16 *mapNumber;
  struct SaveBlock *state;
  s32 column;

  pillar = ObjectEvent_GetById(LOCALID_SINKING_PILLAR);
  column = pillar->x.fixed >> 20;
  if (column != 40) {
    return;
  }

#if TBS_PATCH_1
  // Through state, the map's address is built as the save block's plus an
  // offset, as the bytes have it; &gSaveBlock.mapId folds into one literal
  // (1 of 78 instructions differ in tbs-en, 1 of 65 in tbs-ja).
  state = &gSaveBlock;
  mapNumber = &state->mapId;
  if (GameFlag_Get(*mapNumber + (FLAG_CROSSBONE_ISLE_FIRST_PASSAGE_OPENED -
                                 (s32)&MapId_CrossboneIsleFirstFloor)) != 0) {
    return;
  }
#endif

  // Play the moved-pillar reaction before hiding it and opening the passage.
  pillar->motionFlags = OBJ_EVENT_FOLLOWS_TERRAIN | OBJ_EVENT_FALLS;

  WAIT(8);
  CrossboneIsle_SpawnPillarMovedBurst(LOCALID_SINKING_PILLAR);
  Sound_PlaySong(SE_136);
  WAIT(40);

  ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_SINKING_PILLAR), 0);
  ObjectMotion_SetActionVariant(LOCALID_SINKING_PILLAR, 3);

  pillar->motionFlags = 0;
  pillar->priorityFlags |= OBJ_EVENT_PRIORITY_UNDERFOOT;

  // Replace the blocked cell and remember that the room's pillar is sunk.
  MapGrid_CopyCellAttributes(42, 10, 1, 1, column, 10);

#if !TBS_PATCH_1
  state = &gSaveBlock;
  mapNumber = &state->mapId;
#endif
  GameFlag_Set(*mapNumber + (FLAG_CROSSBONE_ISLE_FIRST_PASSAGE_OPENED -
                             (s32)&MapId_CrossboneIsleFirstFloor));
}

/// Walking into a corridor room's pillar in Crossbone Isle Cave (coord event
/// 20): push it a step.
static void EventScript_PushPillar(void) {
  SCRIPT_START();
  PushedObject_AdvancePair();
  SCRIPT_END();
  EventScript_MovePillar();
}

/// Walking north into a corridor room's west door in Crossbone Isle Cave
/// (coord event 4): it opens, and the party goes through to the side room.
static void EventScript_OpenWestDoor(void) {
  MapGrid_PlayMetatileCopySequence(gCrossboneIsle_EntranceCells, 37, 7);
  Sound_PlaySong(SE_183);
  Import_ScriptRequestWarp(CORRIDOR_TO_SIDE_ROOM_WEST);
}

/// Walking north into a corridor room's east door in Crossbone Isle Cave (coord
/// event 3): the first time, the monsters behind it attack, and the party comes
/// back at WARP_DEST_AFTER_BATTLE; once they are beaten, the door opens onto
/// the side room.
static void EventScript_OpenEastDoor(void) {
  if (GameFlag_Get(gSaveBlock.mapId +
                   (FLAG_CROSSBONE_ISLE_FIRST_EAST_DOOR_MONSTERS_DEFEATED -
                    (s32)&MapId_CrossboneIsleFirstFloor)) == 0) {
    SCRIPT_START();
    FieldScript_SetNextDestination(gSaveBlock.mapId, WARP_DEST_AFTER_BATTLE);
    Battle_SetType(BATTLE_TYPE_SCRIPTED);
    // The first four rooms use ENCOUNTER_ZONE_CROSSBONE_ISLE_FIRST_GUARDS; the
    // next five use ENCOUNTER_ZONE_CROSSBONE_ISLE_LAST_GUARDS.
    switch (gSaveBlock.mapId - (s32)&MapId_CrossboneIsleFirstFloor) {
    case 0:
      Field_RequestBattleFromEncounterTable(
          ENCOUNTER_ZONE_CROSSBONE_ISLE_FIRST_GUARDS, 0);
      break;
    case 1:
      Field_RequestBattleFromEncounterTable(
          ENCOUNTER_ZONE_CROSSBONE_ISLE_FIRST_GUARDS, 1);
      break;
    case 2:
      Field_RequestBattleFromEncounterTable(
          ENCOUNTER_ZONE_CROSSBONE_ISLE_FIRST_GUARDS, 2);
      break;
    case 3:
      Field_RequestBattleFromEncounterTable(
          ENCOUNTER_ZONE_CROSSBONE_ISLE_FIRST_GUARDS, 3);
      break;
    case 4:
      Field_RequestBattleFromEncounterTable(
          ENCOUNTER_ZONE_CROSSBONE_ISLE_LAST_GUARDS, 0);
      break;
    case 5:
      Field_RequestBattleFromEncounterTable(
          ENCOUNTER_ZONE_CROSSBONE_ISLE_LAST_GUARDS, 1);
      break;
    case 6:
      Field_RequestBattleFromEncounterTable(
          ENCOUNTER_ZONE_CROSSBONE_ISLE_LAST_GUARDS, 2);
      break;
    case 7:
      Field_RequestBattleFromEncounterTable(
          ENCOUNTER_ZONE_CROSSBONE_ISLE_LAST_GUARDS, 3);
      break;
    case 8:
      Field_RequestBattleFromEncounterTable(
          ENCOUNTER_ZONE_CROSSBONE_ISLE_LAST_GUARDS, 4);
      break;
    }
    SCRIPT_END();
  } else {
    MapGrid_PlayMetatileCopySequence(gCrossboneIsle_EntranceCells, 44, 7);
    Sound_PlaySong(SE_183);
    Import_ScriptRequestWarp(CORRIDOR_TO_SIDE_ROOM_EAST);
  }
}

/// Back from the battle at a corridor room's east door
/// (WARP_DEST_AFTER_BATTLE): the door opens, and the party walks through.
static void CrossboneIsle_OpenEastDoorAfterBattle(void) {
  SCRIPT_START();
  OBJECT_SPEED(LOCALID_PARTY_LEADER, PIXELS(2) / 5, PIXELS(1) / 5);
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  GameFlag_Set((gSaveBlock.mapId +
                (FLAG_CROSSBONE_ISLE_FIRST_EAST_DOOR_MONSTERS_DEFEATED -
                 (s32)&MapId_CrossboneIsleFirstFloor)));
  WAIT(30);
  MapGrid_AnimateCells(gCrossboneIsle_EntranceCells, 44, 7);
  ObjectEvent_CenterAndWalk(LOCALID_PARTY_LEADER, 3, -16);
  Import_ScriptRequestWarp(CORRIDOR_TO_SIDE_ROOM_EAST);
  SCRIPT_END();
}

/// Unused: Growth on Crossbone Isle's first plant would grow it and let the
/// party climb at (304, 368), but the object event templates put that plant at
/// the map's origin, off stage in the forest's corner, where no cast can reach
/// it and the player never sees it (its render).
static void EventScript_UnusedGrowFirstPlant(s32 localId) {
  s32 fixedX = PIXELS(304);
  s32 fixedZ = PIXELS(368);
  struct ObjectEvent *objectEvent = ObjectEvent_GetById(LOCALID_FIRST_PLANT);

  if (objectEvent != 0) {
    objectEvent->collisionFlags = 0;
  }
  ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(localId), 0);
  MapGrid_SetMetatileBehaviorAt(0, fixedX, fixedZ, MB_CLIMB);
  GameFlag_Set(FLAG_CROSSBONE_ISLE_FIRST_PLANT_GROWN);
}

/// Growth on Crossbone Isle's second plant.
static void EventScript_GrowSecondPlant(s32 localId) {
  s32 fixedX = PIXELS(80);
  s32 fixedZ = PIXELS(368);
  struct ObjectEvent *objectEvent =
      ObjectEvent_GetById(LOCALID_FIRST_PLANT + 1);

  if (objectEvent != 0) {
    objectEvent->collisionFlags = 0;
  }
  ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(localId), 0);
  MapGrid_SetMetatileBehaviorAt(0, fixedX, fixedZ, MB_CLIMB);
  GameFlag_Set(FLAG_CROSSBONE_ISLE_FIRST_PLANT_GROWN + 1);
}

/// Growth on Crossbone Isle's third plant.
static void EventScript_GrowThirdPlant(s32 localId) {
  s32 fixedX = PIXELS(96);
  s32 fixedZ = PIXELS(336);
  struct ObjectEvent *objectEvent =
      ObjectEvent_GetById(LOCALID_FIRST_PLANT + 2);

  if (objectEvent != 0) {
    objectEvent->collisionFlags = 0;
  }
  ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(localId), 0);
  MapGrid_SetMetatileBehaviorAt(0, fixedX, fixedZ, MB_CLIMB);
  GameFlag_Set(FLAG_CROSSBONE_ISLE_FIRST_PLANT_GROWN + 2);
}

/// Growth on Crossbone Isle's fourth plant, which opens two cells.
static void EventScript_GrowFourthPlant(s32 localId) {
  s32 fixedX = PIXELS(144);
  s32 fixedZ = PIXELS(320);
  s32 secondFixedX = PIXELS(752);
  s32 secondFixedZ = PIXELS(320);
  struct ObjectEvent *objectEvent =
      ObjectEvent_GetById(LOCALID_FIRST_PLANT + 3);

  if (objectEvent != 0) {
    objectEvent->collisionFlags = 0;
  }
  ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(localId), 0);
  MapGrid_SetMetatileBehaviorAt(0, fixedX, fixedZ, MB_CLIMB);
  MapGrid_SetMetatileBehaviorAt(0, secondFixedX, secondFixedZ, MB_CLIMB);
  GameFlag_Set(FLAG_CROSSBONE_ISLE_FIRST_PLANT_GROWN + 3);
}

/// Whirlwind on Crossbone Isle's first ivy: it is blown away for good, and
/// its cell opens.
static void EventScript_BlowAwayFirstIvy(void) {
  GameFlag_Set(FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY);
  {

    MapGrid_CopyCellAttributes(0, 0, 1, 1, 8, 21);
  }
}

/// Unused: Whirlwind on Crossbone Isle's second ivy would blow it away for
/// good, but the object event templates put that ivy at the map's origin, off
/// stage, where no cast can reach it and the player never sees it (its render).
static void EventScript_UnusedBlowAwaySecondIvy(void) {
  GameFlag_Set(FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY + 1);
}

/// Whirlwind on Crossbone Isle's third ivy.
static void EventScript_BlowAwayThirdIvy(void) {
  GameFlag_Set(FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY + 2);
}

/// Unused: Whirlwind on Crossbone Isle's fourth ivy would blow it away for
/// good, but the object event templates place only three, so no object event
/// answers its map event.
static void EventScript_UnusedBlowAwayFourthIvy(void) {
  GameFlag_Set(FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY + 3);
}

/// On Crossbone Isle, ivy blown away stays gone; the rest stands tall.
static void CrossboneIsle_RestoreIvy(void) {
  struct ObjectEvent *objectEvent;

  if (GameFlag_Get(FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY) != 0) {
    MapGrid_CopyCellAttributes(0, 0, 1, 1, 8, 21);
    OBJECT_POSITION_SET(LOCALID_FIRST_IVY, 968, 968);
  } else {
    objectEvent = ObjectEvent_Get(LOCALID_FIRST_IVY);
    objectEvent->scaleY = 8 * Q16_ONE / 5;
  }
  if (GameFlag_Get(FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY + 1) != 0) {
    OBJECT_POSITION_SET(LOCALID_FIRST_IVY + 1, 968, 968);
  } else {
    objectEvent = ObjectEvent_Get(LOCALID_FIRST_IVY + 1);
    objectEvent->scaleY = 8 * Q16_ONE / 5;
  }
  if (GameFlag_Get(FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY + 2) != 0) {
    OBJECT_POSITION_SET(LOCALID_FIRST_IVY + 2, 968, 968);
  } else {
    objectEvent = ObjectEvent_Get(LOCALID_FIRST_IVY + 2);
    objectEvent->scaleY = 8 * Q16_ONE / 5;
  }
  if (GameFlag_Get(FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY + 3) != 0) {
    OBJECT_POSITION_SET(LOCALID_FIRST_IVY + 3, 968, 968);
  } else {
    objectEvent = ObjectEvent_Get(LOCALID_FIRST_IVY + 3);
    // BUG: no fourth ivy is placed, so its slot is empty (ObjectEvent_ClearAll)
    // and this writes to address 0x18, in the BIOS, where the write is lost.
#ifdef BUGFIX
    if (objectEvent != NULL)
#endif
      objectEvent->scaleY = 8 * Q16_ONE / 5;
  }
}

/// Walking into one of Crossbone Isle's two logs: push it if the cell beyond
/// is clear.
static void EventScript_PushIsleLog(void) {
  struct PushedObjectProbe probe;

  SCRIPT_START();
  if (PushedObject_FindClearPosition(&probe) != 0) {
    ObjectEvent_MoveAndRedraw(probe);
  }
  SCRIPT_END();
}

/// Moves the selected object event and puzzle log by the same pixel offset.
static void CrossboneIsle_MoveSelectedObjectEventAndLog(s32 localId,
                                                        s32 xOffset,
                                                        s32 zOffset) {
  struct ObjectEvent *selectedObjectEvent;
  struct ObjectEvent *logObjectEvent;
  s32 fixedX;
  s32 fixedZ;

  selectedObjectEvent = ObjectEvent_GetById(gSaveBlock.leaderLocalId);
  logObjectEvent = ObjectEvent_GetById(localId);
  SCRIPT_START();
  {
    fixedX =
        ((selectedObjectEvent->x.fixed + (xOffset * PIXELS(1))) & 0xFFF00000) +
        PIXELS(8);
    fixedZ =
        ((selectedObjectEvent->z.fixed + (zOffset * PIXELS(1))) & 0xFFF00000) +
        PIXELS(8);

    selectedObjectEvent->speed = PIXELS(1);
    selectedObjectEvent->acceleration = PIXELS(1) / 2;
    Object_SetPosition(selectedObjectEvent, fixedX,
                       selectedObjectEvent->y.fixed, fixedZ);
  }
  // Position the log on the same cell-center grid as the selected object event.
  Object_SelectAnimation(selectedObjectEvent, 27);
  {
    fixedX = ((logObjectEvent->x.fixed + (xOffset * PIXELS(1))) & 0xFFF00000) +
             PIXELS(8);
    fixedZ = ((logObjectEvent->z.fixed + (zOffset * PIXELS(1))) & 0xFFF00000) +
             PIXELS(8);

    logObjectEvent->speed = PIXELS(1);
    logObjectEvent->acceleration = PIXELS(1) / 2;
    Object_SetPosition(logObjectEvent, fixedX, logObjectEvent->y.fixed, fixedZ);
  }
  // The log's mode depends on which side of the grid it moves toward.
  if (xOffset < 0 || zOffset < 0) {
    Object_SelectAnimation(logObjectEvent, 4);
  } else {
    Object_SelectAnimation(logObjectEvent, 3);
  }
  Sound_PlaySong(SE_226);
  Object_WaitForMove(selectedObjectEvent);
  Sound_PlaySong(SE_STOP_ACTION_SE);
  SCRIPT_END();
}

/// Draws the three-cell horizontal footprint centered on this log.
static void CrossboneIsle_DrawHorizontalLogFootprint(s32 localId,
                                                     s32 sourceColumn,
                                                     s32 sourceRow,
                                                     s32 unused) {
  struct ObjectEvent *objectEvent = ObjectEvent_GetById(localId);

  if (objectEvent != 0) {
    s32 priorityFlags;

    ObjectMotion_SetActionVariant(localId, 3);
    priorityFlags = OBJ_EVENT_PRIORITY_UNDERFOOT;
    priorityFlags |= objectEvent->priorityFlags;
    objectEvent->priorityFlags = priorityFlags;
    {
      s32 row = objectEvent->z.fixed >> 20;
      s32 column = (objectEvent->x.fixed >> 20) - 1;

      MapGrid_CopyCellAttributes(sourceColumn, sourceRow, 3, 1, column, row);
    }
  }
}

/// Draws the three-cell vertical footprint centered on this log.
static void CrossboneIsle_DrawVerticalLogFootprint(s32 localId,
                                                   s32 sourceColumn,
                                                   s32 sourceRow, s32 unused) {
  struct ObjectEvent *objectEvent = ObjectEvent_GetById(localId);

  if (objectEvent != 0) {
    s32 priorityFlags;

    ObjectMotion_SetActionVariant(localId, 3);
    priorityFlags = OBJ_EVENT_PRIORITY_UNDERFOOT;
    priorityFlags |= objectEvent->priorityFlags;
    objectEvent->priorityFlags = priorityFlags;
    {
      s32 column = objectEvent->x.fixed >> 20;
      s32 row = (objectEvent->z.fixed >> 20) - 1;

      MapGrid_CopyCellAttributes(sourceColumn, sourceRow, 1, 3, column, row);
    }
  }
}

/// Runs the two animation changes used when touching a puzzle log.
static void CrossboneIsle_AnimateTouchedLog(int localId) {
  OBJECT_ANIMATION(localId, 1);
  OBJECT_ANIMATION(localId, 2);
}

/// Draws all twelve log footprints in Crossbone Isle Cave
/// (CrossboneIsleFloatingLogs).
static void CrossboneIsle_DrawLogPuzzleFootprints(void) {
  CrossboneIsle_DrawHorizontalLogFootprint(LOCALID_FIRST_LOG, 17, 30, 21);
  CrossboneIsle_DrawHorizontalLogFootprint(LOCALID_THIRD_LOG, 17, 31, 22);
  CrossboneIsle_DrawVerticalLogFootprint(LOCALID_FOURTH_LOG, 20, 30, 23);
  CrossboneIsle_DrawVerticalLogFootprint(LOCALID_FIFTH_LOG, 21, 30, 24);
  CrossboneIsle_DrawVerticalLogFootprint(LOCALID_SIXTH_LOG, 22, 30, 25);
  CrossboneIsle_DrawVerticalLogFootprint(LOCALID_EIGHTH_LOG, 23, 30, 26);
  CrossboneIsle_DrawHorizontalLogFootprint(LOCALID_TENTH_LOG, 0, 30, 31);
  CrossboneIsle_DrawHorizontalLogFootprint(LOCALID_ELEVENTH_LOG, 0, 31, 32);
  CrossboneIsle_DrawHorizontalLogFootprint(LOCALID_SECOND_LOG, 0, 32, 33);
  CrossboneIsle_DrawVerticalLogFootprint(LOCALID_TWELFTH_LOG, 4, 30, 34);
  CrossboneIsle_DrawVerticalLogFootprint(LOCALID_SEVENTH_LOG, 5, 30, 35);
  CrossboneIsle_DrawVerticalLogFootprint(LOCALID_NINTH_LOG, 6, 30, 36);
}

// Crossbone Isle Cave (CrossboneIsleFloatingLogs), the ninth floor's side room:
// its twelve floating logs. Stepping onto a log plays its animation from the
// start; walking onto one in a direction it moves slides it that way with the
// leader on it until another log or the wall stops it; walking against a
// log's edge hops the leader 32 pixels if the landing is clear.

/// Stepping onto the first log.
static void EventScript_StepOnFirstLog(void) {
  CrossboneIsle_AnimateTouchedLog(LOCALID_FIRST_LOG);
}

/// Stepping onto the third log.
static void EventScript_StepOnThirdLog(void) {
  CrossboneIsle_AnimateTouchedLog(LOCALID_THIRD_LOG);
}

/// Stepping onto the fourth log.
static void EventScript_StepOnFourthLog(void) {
  CrossboneIsle_AnimateTouchedLog(LOCALID_FOURTH_LOG);
}

/// Stepping onto the fifth log.
static void EventScript_StepOnFifthLog(void) {
  CrossboneIsle_AnimateTouchedLog(LOCALID_FIFTH_LOG);
}

/// Stepping onto the sixth log.
static void EventScript_StepOnSixthLog(void) {
  CrossboneIsle_AnimateTouchedLog(LOCALID_SIXTH_LOG);
}

/// Stepping onto the eighth log.
static void EventScript_StepOnEighthLog(void) {
  CrossboneIsle_AnimateTouchedLog(LOCALID_EIGHTH_LOG);
}

/// Stepping onto the tenth log.
static void EventScript_StepOnTenthLog(void) {
  CrossboneIsle_AnimateTouchedLog(LOCALID_TENTH_LOG);
}

/// Stepping onto the eleventh log.
static void EventScript_StepOnEleventhLog(void) {
  CrossboneIsle_AnimateTouchedLog(LOCALID_ELEVENTH_LOG);
}

/// Stepping onto the second log.
static void EventScript_StepOnSecondLog(void) {
  CrossboneIsle_AnimateTouchedLog(LOCALID_SECOND_LOG);
}

/// Stepping onto the twelfth log.
static void EventScript_StepOnTwelfthLog(void) {
  CrossboneIsle_AnimateTouchedLog(LOCALID_TWELFTH_LOG);
}

/// Stepping onto the seventh log.
static void EventScript_StepOnSeventhLog(void) {
  CrossboneIsle_AnimateTouchedLog(LOCALID_SEVENTH_LOG);
}

/// Stepping onto the ninth log.
static void EventScript_StepOnNinthLog(void) {
  CrossboneIsle_AnimateTouchedLog(LOCALID_NINTH_LOG);
}

/// Tests a step, then moves the leader with the log-pushing animation.
static s32 CrossboneIsle_TryHop(struct ObjectEvent *objectEvent,
                                struct FieldPosition *requested) {
  u8 *motionFlags = &objectEvent->motionFlags;
  s32 savedMotionFlags = *motionFlags;
#if TBS_PATCH_1
  struct FieldPosition destination;

  destination.x = (objectEvent->x.fixed & 0xfff00000) + PIXELS(8);
  destination.y = objectEvent->y.fixed;
  destination.z = (objectEvent->z.fixed & 0xfff00000) + PIXELS(8);
  {
    s32 direction = (objectEvent->facing + 0x2000) & 0xc000;

    Vector_AddPolarOffset(PIXELS(32), direction, &destination);
  }
  // Keep the original motion flags while playing the leader's push step.
  if (FieldObject_CheckMovementCollision(objectEvent, &destination) == 0) {
#else
  if (FieldObject_CheckMovementCollision(objectEvent, requested) == 0) {
#endif
    s32 movingFlags;

    SCRIPT_START();
    Object_SelectAnimation(objectEvent, 6);
    WaitFrames(6);
    Sound_PlaySong(SE_JUMP);
    Object_SelectAnimation(objectEvent, 7);
    objectEvent->speed = PIXELS(3);
    objectEvent->acceleration = PIXELS(2);
    objectEvent->velocityY = PIXELS(4);
    movingFlags = 126;
    movingFlags &= *motionFlags;
    *motionFlags = (u8)movingFlags;
    ObjectEvent_SetSpriteFlags(objectEvent, 0);
    {
#if TBS_PATCH_1
      s16 *pixelCoordinates = (s16 *)&destination;
#else
      s16 *pixelCoordinates = (s16 *)requested;
#endif

      ObjectMotion_SetPositionAndCommit(
          LOCALID_PARTY_LEADER, pixelCoordinates[1], pixelCoordinates[5]);
    }
    Object_SelectAnimation(objectEvent, 6);
    ObjectEvent_SetSpriteFlags(objectEvent, SPRITE_FLAG_SHADOW);
    *motionFlags = (u8)savedMotionFlags;
    SCRIPT_END();
    return 1;
  }
  return 0;
}

/// Walking north against a log's edge.
static s32 EventScript_HopNorth(void) {
  struct ObjectEvent *objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  struct FieldPosition destination;

  destination.x = objectEvent->x.fixed;
  destination.y = objectEvent->y.fixed;
  destination.z = objectEvent->z.fixed + -PIXELS(32);
  return CrossboneIsle_TryHop(objectEvent, &destination);
}

/// Walking south against a log's edge, or at the Pirate Ship's coord event 20.
static s32 EventScript_HopSouth(void) {
  struct ObjectEvent *objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  struct FieldPosition destination;

  destination.x = objectEvent->x.fixed;
  destination.y = objectEvent->y.fixed;
  destination.z = objectEvent->z.fixed + PIXELS(32);
  return CrossboneIsle_TryHop(objectEvent, &destination);
}

/// Walking west against a log's edge.
static s32 EventScript_HopWest(void) {
  struct ObjectEvent *objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  struct FieldPosition destination;

  destination.x = objectEvent->x.fixed + -PIXELS(32);
  destination.y = objectEvent->y.fixed;
  destination.z = objectEvent->z.fixed;
  return CrossboneIsle_TryHop(objectEvent, &destination);
}

/// Walking east against a log's edge.
static s32 EventScript_HopEast(void) {
  struct ObjectEvent *objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  struct FieldPosition destination;

  destination.x = objectEvent->x.fixed + PIXELS(32);
  destination.y = objectEvent->y.fixed;
  destination.z = objectEvent->z.fixed;
  return CrossboneIsle_TryHop(objectEvent, &destination);
}

/// Walking north on the first log drifts it north.
static void EventScript_SlideFirstLogNorth(void) {
  s32 objectEventX = ObjectEvent_GetById(LOCALID_FIRST_LOG)->x.fixed >> 20;
  s32 objectEventZ = ObjectEvent_GetById(LOCALID_FIRST_LOG)->z.fixed >> 20;
  s32 fifthLogColumn = ObjectEvent_GetById(LOCALID_FIFTH_LOG)->x.fixed >> 20;
  s32 eighthLogColumn = ObjectEvent_GetById(LOCALID_EIGHTH_LOG)->x.fixed >> 20;

  if (objectEventZ == 19) {
    if (fifthLogColumn == 24) {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_FIRST_LOG, 0, -80);
    } else if (eighthLogColumn == 24) {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_FIRST_LOG, 0, -112);
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_FIRST_LOG, 0, -32);
    } else {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_FIRST_LOG, 0, -80);
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_FIRST_LOG, 0, -112);
    }
  } else if (objectEventZ == 14) {
    if (fifthLogColumn == 24) {
      return;
    }
    if (eighthLogColumn == 24) {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_FIRST_LOG, 0, -64);
    } else {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_FIRST_LOG, 0, -112);
    }
  } else if (objectEventZ == 10) {
    if (eighthLogColumn == 24) {
      return;
    }
    CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_FIRST_LOG, 0, -48);
  } else {
    EventScript_HopNorth();
    return;
  }
  WaitFrames(2);
  {
    s32 routeEndZ = ObjectEvent_GetById(LOCALID_FIRST_LOG)->z.fixed >> 20;
    s32 routeX = objectEventX - 1;

    MapGrid_CopyCellAttributes(routeX, objectEventZ, 3, 1, routeX, routeEndZ);
  }
  MapGrid_CopyCellAttributes(0, 0, 3, 1, objectEventX - 1, objectEventZ);
}

/// Copies a horizontal log's old footprint to its new row, then clears the
/// old row.
static inline void CrossboneIsle_RedrawHorizontalLogRoute(s32 localId,
                                                          s32 column, s32 row) {
  WaitFrames(2);
  {
    s32 destinationRow = ObjectEvent_GetById(localId)->z.fixed >> 20;
    s32 footprintColumn = column - 1;

    MapGrid_CopyCellAttributes(footprintColumn, row, 3, 1, footprintColumn,
                               destinationRow);
  }
  MapGrid_CopyCellAttributes(0, 0, 3, 1, column - 1, row);
}

/// Copies a vertical log's old footprint to its new column, then clears the
/// old one.
static inline void CrossboneIsle_RedrawVerticalLogRoute(s32 localId, s32 column,
                                                        s32 row) {
  WaitFrames(2);
  {
    s32 destinationColumn = ObjectEvent_GetById(localId)->x.fixed >> 20;
    s32 footprintRow = row - 1;

    MapGrid_CopyCellAttributes(column, footprintRow, 1, 3, destinationColumn,
                               footprintRow);
  }
  MapGrid_CopyCellAttributes(0, 0, 1, 3, column, row - 1);
}

/// Walking south on the first log drifts it south.
static void EventScript_SlideFirstLogSouth(void) {
  s32 column = ObjectEvent_GetById(LOCALID_FIRST_LOG)->x.fixed >> 20;
  s32 row = ObjectEvent_GetById(LOCALID_FIRST_LOG)->z.fixed >> 20;
  s32 fifthLogColumn = ObjectEvent_GetById(LOCALID_FIFTH_LOG)->x.fixed >> 20;

  if (row == 7) {
    if (fifthLogColumn == 24) {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_FIRST_LOG, 0, 48);
    } else {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_FIRST_LOG, 0, 80);
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_FIRST_LOG, 0, 112);
    }
  } else if (row == 10) {
    if (fifthLogColumn == 24) {
      return;
    }
    CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_FIRST_LOG, 0, 144);
  } else if (row == 14) {
    CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_FIRST_LOG, 0, 80);
  } else {
    return;
  }
  CrossboneIsle_RedrawHorizontalLogRoute(LOCALID_FIRST_LOG, column, row);
}

/// Walking north on the third log drifts it north.
static void EventScript_SlideThirdLogNorth(void) {
  s32 column = ObjectEvent_GetById(LOCALID_THIRD_LOG)->x.fixed >> 20;
  s32 row = ObjectEvent_GetById(LOCALID_THIRD_LOG)->z.fixed >> 20;
  s32 sixthLogColumn = ObjectEvent_GetById(LOCALID_SIXTH_LOG)->x.fixed >> 20;
  s32 eighthLogColumn = ObjectEvent_GetById(LOCALID_EIGHTH_LOG)->x.fixed >> 20;

  if (row == 18) {
    if (eighthLogColumn >= 31 && eighthLogColumn <= 33) {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_THIRD_LOG, 0, -128);
    } else if (sixthLogColumn >= 31 && sixthLogColumn <= 33) {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_THIRD_LOG, 0, -128);
    } else {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_THIRD_LOG, 0, -112);
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_THIRD_LOG, 0, -64);
    }
  } else if (row == 10) {
    if (eighthLogColumn >= 31 && eighthLogColumn <= 33) {
      return;
    }
    if (sixthLogColumn >= 31 && sixthLogColumn <= 33) {
      return;
    }
    CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_THIRD_LOG, 0, -48);
  } else if (row == 7) {
    return;
  }
  CrossboneIsle_RedrawHorizontalLogRoute(LOCALID_THIRD_LOG, column, row);
}

/// Walking south on the third log drifts it south.
static void EventScript_SlideThirdLogSouth(void) {
  s32 column = ObjectEvent_GetById(LOCALID_THIRD_LOG)->x.fixed >> 20;
  s32 row = ObjectEvent_GetById(LOCALID_THIRD_LOG)->z.fixed >> 20;

  if (row != 18) {
    if (row == 10) {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_THIRD_LOG, 0, 128);
    } else {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_THIRD_LOG, 0, 112);
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_THIRD_LOG, 0, 64);
    }
    CrossboneIsle_RedrawHorizontalLogRoute(LOCALID_THIRD_LOG, column, row);
  }
}

/// Walking west on the fourth log drifts it west.
static void EventScript_SlideFourthLogWest(void) {
  s32 column = ObjectEvent_GetById(LOCALID_FOURTH_LOG)->x.fixed >> 20;
  s32 row = ObjectEvent_GetById(LOCALID_FOURTH_LOG)->z.fixed >> 20;

  if (column != 30) {
    if (column == 34) {
      if ((ObjectEvent_GetById(LOCALID_THIRD_LOG)->z.fixed >> 20) == 18) {
        return;
      }
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_FOURTH_LOG, -64, 0);
    } else if (column == 36) {
      if ((ObjectEvent_GetById(LOCALID_THIRD_LOG)->z.fixed >> 20) == 18) {
        CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_FOURTH_LOG, -32, 0);
      } else {
        CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_FOURTH_LOG, -96, 0);
      }
    }
    CrossboneIsle_RedrawVerticalLogRoute(LOCALID_FOURTH_LOG, column, row);
  }
}

/// Walking east on the fourth log drifts it east.
static void EventScript_SlideFourthLogEast(void) {
  s32 column = ObjectEvent_GetById(LOCALID_FOURTH_LOG)->x.fixed >> 20;
  s32 row = ObjectEvent_GetById(LOCALID_FOURTH_LOG)->z.fixed >> 20;

  if (column != 36) {
    if (column == 30) {
      if ((ObjectEvent_GetById(LOCALID_THIRD_LOG)->z.fixed >> 20) == 18) {
        return;
      }
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_FOURTH_LOG, 96, 0);
    } else if (column == 34) {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_FOURTH_LOG, 32, 0);
    }
    CrossboneIsle_RedrawVerticalLogRoute(LOCALID_FOURTH_LOG, column, row);
  }
}

/// Walking west on the fifth log drifts it west.
static void EventScript_SlideFifthLogWest(void) {
  s32 column = ObjectEvent_GetById(LOCALID_FIFTH_LOG)->x.fixed >> 20;
  s32 row = ObjectEvent_GetById(LOCALID_FIFTH_LOG)->z.fixed >> 20;

  if (column == 36) {
    CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_FIFTH_LOG, -96, 0);
    CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_FIFTH_LOG, -96, 0);
  } else if (column == 34) {
    CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_FIFTH_LOG, -96, 0);
    CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_FIFTH_LOG, -64, 0);
  } else if (column == 24) {
    return;
  }
  CrossboneIsle_RedrawVerticalLogRoute(LOCALID_FIFTH_LOG, column, row);
}

/// Walking east on the fifth log drifts it east.
static void EventScript_SlideFifthLogEast(void) {
  s32 column = ObjectEvent_GetById(LOCALID_FIFTH_LOG)->x.fixed >> 20;
  s32 row = ObjectEvent_GetById(LOCALID_FIFTH_LOG)->z.fixed >> 20;

  if (column == 24) {
    CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_FIFTH_LOG, 96, 0);
    CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_FIFTH_LOG, 96, 0);
  } else if (column == 34) {
    CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_FIFTH_LOG, 32, 0);
  } else if (column == 36) {
    return;
  }
  CrossboneIsle_RedrawVerticalLogRoute(LOCALID_FIFTH_LOG, column, row);
}

/// Walking west on the sixth log drifts it west.
static void EventScript_SlideSixthLogWest(void) {
  s32 column = ObjectEvent_GetById(LOCALID_SIXTH_LOG)->x.fixed >> 20;
  s32 row = ObjectEvent_GetById(LOCALID_SIXTH_LOG)->z.fixed >> 20;
  s32 thirdLogRow = ObjectEvent_GetById(LOCALID_THIRD_LOG)->z.fixed >> 20;
  s32 eighthLogColumn = ObjectEvent_GetById(LOCALID_EIGHTH_LOG)->x.fixed >> 20;

  if (column == 36) {
    if (eighthLogColumn == 34) {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_SIXTH_LOG, -16, 0);
    } else if (thirdLogRow == 7) {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_SIXTH_LOG, -32, 0);
    } else if (eighthLogColumn == 30) {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_SIXTH_LOG, -80, 0);
    } else {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_SIXTH_LOG, -96, 0);
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_SIXTH_LOG, -80, 0);
    }
  } else if (column == 35) {
    if (eighthLogColumn == 34) {
      return;
    }
    if (thirdLogRow == 7) {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_SIXTH_LOG, -16, 0);
    } else if (eighthLogColumn == 30) {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_SIXTH_LOG, -64, 0);
    } else {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_SIXTH_LOG, -80, 0);
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_SIXTH_LOG, -80, 0);
    }
  } else if (column == 34) {
    if (thirdLogRow == 7) {
      return;
    }
    if (eighthLogColumn == 30) {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_SIXTH_LOG, -48, 0);
    } else {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_SIXTH_LOG, -144, 0);
    }
  } else if (column == 31) {
    if (eighthLogColumn == 30) {
      return;
    }
    CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_SIXTH_LOG, -96, 0);
  } else if (column == 25) {
    return;
  }
  CrossboneIsle_RedrawVerticalLogRoute(LOCALID_SIXTH_LOG, column, row);
}

/// Walking east on the sixth log drifts it east.
static void EventScript_SlideSixthLogEast(void) {
  s32 column = ObjectEvent_GetById(LOCALID_SIXTH_LOG)->x.fixed >> 20;
  s32 row = ObjectEvent_GetById(LOCALID_SIXTH_LOG)->z.fixed >> 20;

  ObjectEvent_GetById(LOCALID_EIGHTH_LOG);
  if (column == 25) {
    CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_SIXTH_LOG, 96, 0);
    CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_SIXTH_LOG, 80, 0);
  } else if (column == 31) {
    CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_SIXTH_LOG, 80, 0);
  } else if (column == 34) {
    CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_SIXTH_LOG, 32, 0);
  } else if (column == 35) {
    CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_SIXTH_LOG, 16, 0);
  } else if (column == 36) {
    return;
  }
  CrossboneIsle_RedrawVerticalLogRoute(LOCALID_SIXTH_LOG, column, row);
}

/// Walking west on the eighth log drifts it west.
static void EventScript_SlideEighthLogWest(void) {
  s32 column = ObjectEvent_GetById(LOCALID_EIGHTH_LOG)->x.fixed >> 20;
  s32 row = ObjectEvent_GetById(LOCALID_EIGHTH_LOG)->z.fixed >> 20;
  s32 firstLogRow = ObjectEvent_GetById(LOCALID_FIRST_LOG)->z.fixed >> 20;
  s32 thirdLogRow = ObjectEvent_GetById(LOCALID_THIRD_LOG)->z.fixed >> 20;

  if (column == 35) {
    if (thirdLogRow == 7) {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_EIGHTH_LOG, -16, 0);
    } else if (firstLogRow == 7) {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_EIGHTH_LOG, -112, 0);
    } else {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_EIGHTH_LOG, -96, 0);
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_EIGHTH_LOG, -80, 0);
    }
  } else if (column == 34) {
    if (thirdLogRow == 7) {
      return;
    }
    CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_EIGHTH_LOG, -96, 0);
    CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_EIGHTH_LOG, -64, 0);
  } else if (column == 33) {
    CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_EIGHTH_LOG, -144, 0);
  } else if (column == 31) {
    CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_EIGHTH_LOG, -80, 0);
  } else if (column == 30) {
    CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_EIGHTH_LOG, -96, 0);
  } else if (column == 24) {
    return;
  }
  CrossboneIsle_RedrawVerticalLogRoute(LOCALID_EIGHTH_LOG, column, row);
}

/// Walking east on the eighth log drifts it east.
static void EventScript_SlideEighthLogEast(void) {
  s32 column = ObjectEvent_GetById(LOCALID_EIGHTH_LOG)->x.fixed >> 20;
  s32 row = ObjectEvent_GetById(LOCALID_EIGHTH_LOG)->z.fixed >> 20;
  s32 thirdLogRow = ObjectEvent_GetById(LOCALID_THIRD_LOG)->z.fixed >> 20;
  s32 sixthLogColumn = ObjectEvent_GetById(LOCALID_SIXTH_LOG)->x.fixed >> 20;

  if (column == 24) {
    if (thirdLogRow == 7 || sixthLogColumn == 31) {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_EIGHTH_LOG, 96, 0);
    } else if (sixthLogColumn == 34) {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_EIGHTH_LOG, 64, 0);
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_EIGHTH_LOG, 80, 0);
    } else if (sixthLogColumn == 35) {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_EIGHTH_LOG, 80, 0);
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_EIGHTH_LOG, 80, 0);
    } else {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_EIGHTH_LOG, 80, 0);
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_EIGHTH_LOG, 96, 0);
    }
  } else if (column == 30 || sixthLogColumn == 31) {
    if (thirdLogRow == 7) {
      return;
    }
    if (sixthLogColumn == 34) {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_EIGHTH_LOG, 48, 0);
    } else if (sixthLogColumn == 35) {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_EIGHTH_LOG, 64, 0);
    } else {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_EIGHTH_LOG, 80, 0);
    }
  } else if (column == 33) {
    if (sixthLogColumn == 34) {
      return;
    }
    if (sixthLogColumn == 35) {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_EIGHTH_LOG, 16, 0);
    } else {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_EIGHTH_LOG, 32, 0);
    }
  } else if (column == 34) {
    CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_EIGHTH_LOG, 16, 0);
  } else if (column == 35) {
    return;
  }
  CrossboneIsle_RedrawVerticalLogRoute(LOCALID_EIGHTH_LOG, column, row);
}

/// Walking north on the tenth log drifts it north.
static void EventScript_SlideTenthLogNorth(void) {
  s32 column = ObjectEvent_GetById(LOCALID_TENTH_LOG)->x.fixed >> 20;
  s32 row = ObjectEvent_GetById(LOCALID_TENTH_LOG)->z.fixed >> 20;
  s32 twelfthLogColumn =
      ObjectEvent_GetById(LOCALID_TWELFTH_LOG)->x.fixed >> 20;

  if (row == 19) {
    if (twelfthLogColumn >= 3 && twelfthLogColumn <= 5) {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_TENTH_LOG, 0, -16);
    } else {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_TENTH_LOG, 0, -64);
    }
  } else if (row == 18) {
    if (twelfthLogColumn >= 3 && twelfthLogColumn <= 5) {
      return;
    }
    CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_TENTH_LOG, 0, -48);
  } else if (row == 15) {
    return;
  }
  CrossboneIsle_RedrawHorizontalLogRoute(LOCALID_TENTH_LOG, column, row);
}

/// Walking south on the tenth log drifts it south.
static void EventScript_SlideTenthLogSouth(void) {
  s32 column = ObjectEvent_GetById(LOCALID_TENTH_LOG)->x.fixed >> 20;
  s32 row = ObjectEvent_GetById(LOCALID_TENTH_LOG)->z.fixed >> 20;

  if (row == 15) {
    CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_TENTH_LOG, 0, 64);
  } else if (row == 18) {
    CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_TENTH_LOG, 0, 16);
  } else if (row == 19) {
    return;
  }
  CrossboneIsle_RedrawHorizontalLogRoute(LOCALID_TENTH_LOG, column, row);
}

/// These three columns form the crossing lane for the vertical logs.
#define IS_IN_LOG_CROSSING_COLUMNS(x) ((x) >= 6 && (x) <= 8)

/// Walking north on the eleventh log drifts it north.
static void EventScript_SlideEleventhLogNorth(void) {
  s32 column;
  s32 row;
  s32 twelfthLogColumn;
  s32 seventhLogColumn;
  s32 ninthLogColumn;

  column = ObjectEvent_GetById(LOCALID_ELEVENTH_LOG)->x.fixed >> 20;
  row = ObjectEvent_GetById(LOCALID_ELEVENTH_LOG)->z.fixed >> 20;
  twelfthLogColumn = ObjectEvent_GetById(LOCALID_TWELFTH_LOG)->x.fixed >> 20;
  seventhLogColumn = ObjectEvent_GetById(LOCALID_SEVENTH_LOG)->x.fixed >> 20;
  ninthLogColumn = ObjectEvent_GetById(LOCALID_NINTH_LOG)->x.fixed >> 20;
  if (row == 19) {
    if (IS_IN_LOG_CROSSING_COLUMNS(twelfthLogColumn)) {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_ELEVENTH_LOG, 0, -16);
    } else if (IS_IN_LOG_CROSSING_COLUMNS(seventhLogColumn)) {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_ELEVENTH_LOG, 0, -64);
    } else if (IS_IN_LOG_CROSSING_COLUMNS(ninthLogColumn)) {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_ELEVENTH_LOG, 0,
                                                  -112);
    } else {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_ELEVENTH_LOG, 0, -64);
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_ELEVENTH_LOG, 0, -96);
    }
  } else if (row == 18) {
    if (IS_IN_LOG_CROSSING_COLUMNS(twelfthLogColumn)) {
      return;
    }
    if (IS_IN_LOG_CROSSING_COLUMNS(seventhLogColumn)) {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_ELEVENTH_LOG, 0, -48);
    } else if (IS_IN_LOG_CROSSING_COLUMNS(ninthLogColumn)) {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_ELEVENTH_LOG, 0, -96);
    } else {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_ELEVENTH_LOG, 0,
                                                  -144);
    }
  } else if (row == 15) {
    if (IS_IN_LOG_CROSSING_COLUMNS(seventhLogColumn)) {
      return;
    }
    if (IS_IN_LOG_CROSSING_COLUMNS(ninthLogColumn)) {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_ELEVENTH_LOG, 0, -48);
    } else {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_ELEVENTH_LOG, 0, -96);
    }
  } else if (row == 14) {
    if (IS_IN_LOG_CROSSING_COLUMNS(seventhLogColumn)) {
      return;
    }
    if (IS_IN_LOG_CROSSING_COLUMNS(ninthLogColumn)) {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_ELEVENTH_LOG, 0, -32);
    } else {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_ELEVENTH_LOG, 0, -80);
    }
  } else if (row == 12) {
    if (IS_IN_LOG_CROSSING_COLUMNS(ninthLogColumn)) {
      return;
    }
    CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_ELEVENTH_LOG, 0, -48);
  } else if (row == 11) {
    if (IS_IN_LOG_CROSSING_COLUMNS(ninthLogColumn)) {
      return;
    }
    CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_ELEVENTH_LOG, 0, -32);
  } else if (row == 9) {
    return;
  }
  WaitFrames(2);
  MapGrid_CopyCellAttributeRect(
      column - 1, row, 3, 1, column - 1,
      ObjectEvent_GetById(LOCALID_ELEVENTH_LOG)->z.fixed >> 20);
  MapGrid_CopyCellAttributes(0, 0, 3, 1, column - 1, row);
}

#undef IS_IN_LOG_CROSSING_COLUMNS

/// Walking south on the eleventh log drifts it south.
static void EventScript_SlideEleventhLogSouth(void) {
  s32 column;
  s32 row;
  s32 twelfthLogColumn;
  s32 seventhLogColumn;

  column = ObjectEvent_GetById(LOCALID_ELEVENTH_LOG)->x.fixed >> 20;
  row = ObjectEvent_GetById(LOCALID_ELEVENTH_LOG)->z.fixed >> 20;
  twelfthLogColumn = ObjectEvent_GetById(LOCALID_TWELFTH_LOG)->x.fixed >> 20;
  seventhLogColumn = ObjectEvent_GetById(LOCALID_SEVENTH_LOG)->x.fixed >> 20;

  if (row == 9) {
    if ((u32)(seventhLogColumn - 6) <= 2) {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_ELEVENTH_LOG, 0, 32);
    } else if ((u32)(twelfthLogColumn - 6) <= 2) {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_ELEVENTH_LOG, 0, 80);
    } else {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_ELEVENTH_LOG, 0, 64);
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_ELEVENTH_LOG, 0, 96);
    }
  } else if (row == 11) {
    if ((u32)(seventhLogColumn - 6) <= 2) {
      return;
    }
    if ((u32)(twelfthLogColumn - 6) <= 2) {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_ELEVENTH_LOG, 0, 48);
    } else {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_ELEVENTH_LOG, 0, 128);
    }
  } else if (row == 12) {
    if ((u32)(twelfthLogColumn - 6) <= 2) {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_ELEVENTH_LOG, 0, 32);
    } else {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_ELEVENTH_LOG, 0, 112);
    }
  } else if (row == 14) {
    if ((u32)(twelfthLogColumn - 6) <= 2) {
      return;
    }
    CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_ELEVENTH_LOG, 0, 80);
  } else if (row == 15) {
    CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_ELEVENTH_LOG, 0, 64);
  } else if (row == 18) {
    CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_ELEVENTH_LOG, 0, 16);
  } else if (row == 19) {
    return;
  }

  WaitFrames(2);

  MapGrid_CopyCellAttributeRect(
      column - 1, row, 3, 1, column - 1,
      ObjectEvent_GetById(LOCALID_ELEVENTH_LOG)->z.fixed >> 20);
  MapGrid_CopyCellAttributes(0, 0, 3, 1, column - 1, row);
}

#undef IS_IN_LOG_CROSSING_COLUMNS
#define IS_IN_LOG_CROSSING_COLUMNS(x) ((x) >= 9 && (x) <= 11)

/// Walking north on the second log drifts it north.
static void EventScript_SlideSecondLogNorth(void) {
  s32 column;
  s32 row;
  s32 twelfthLogColumn;
  s32 seventhLogColumn;
  s32 ninthLogColumn;

  column = ObjectEvent_GetById(LOCALID_SECOND_LOG)->x.fixed >> 20;
  row = ObjectEvent_GetById(LOCALID_SECOND_LOG)->z.fixed >> 20;
  twelfthLogColumn = ObjectEvent_GetById(LOCALID_TWELFTH_LOG)->x.fixed >> 20;
  seventhLogColumn = ObjectEvent_GetById(LOCALID_SEVENTH_LOG)->x.fixed >> 20;
  ninthLogColumn = ObjectEvent_GetById(LOCALID_NINTH_LOG)->x.fixed >> 20;
  if (row == 19) {
    if (IS_IN_LOG_CROSSING_COLUMNS(twelfthLogColumn)) {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_SECOND_LOG, 0, -16);
    } else if (IS_IN_LOG_CROSSING_COLUMNS(seventhLogColumn)) {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_SECOND_LOG, 0, -64);
    } else if (IS_IN_LOG_CROSSING_COLUMNS(ninthLogColumn)) {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_SECOND_LOG, 0, -112);
    } else {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_SECOND_LOG, 0, -80);
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_SECOND_LOG, 0, -96);
    }
  } else if (row == 18) {
    if (IS_IN_LOG_CROSSING_COLUMNS(twelfthLogColumn)) {
      return;
    }
    if (IS_IN_LOG_CROSSING_COLUMNS(seventhLogColumn)) {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_SECOND_LOG, 0, -48);
    } else if (IS_IN_LOG_CROSSING_COLUMNS(ninthLogColumn)) {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_SECOND_LOG, 0, -96);
    } else {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_SECOND_LOG, 0, -96);
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_SECOND_LOG, 0, -64);
    }
  } else if (row == 15) {
    if (IS_IN_LOG_CROSSING_COLUMNS(seventhLogColumn)) {
      return;
    }
    if (IS_IN_LOG_CROSSING_COLUMNS(ninthLogColumn)) {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_SECOND_LOG, 0, -48);
    } else {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_SECOND_LOG, 0, -112);
    }
  } else if (row == 14) {
    if (IS_IN_LOG_CROSSING_COLUMNS(seventhLogColumn)) {
      return;
    }
    if (IS_IN_LOG_CROSSING_COLUMNS(ninthLogColumn)) {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_SECOND_LOG, 0, -32);
    } else {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_SECOND_LOG, 0, -96);
    }
  } else if (row == 12) {
    if (IS_IN_LOG_CROSSING_COLUMNS(ninthLogColumn)) {
      return;
    }
    CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_SECOND_LOG, 0, -64);
  } else if (row == 11) {
    if (IS_IN_LOG_CROSSING_COLUMNS(ninthLogColumn)) {
      return;
    }
    CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_SECOND_LOG, 0, -48);
  } else if ((u32)row <= 9) {
    return;
  }
  WaitFrames(2);
  MapGrid_CopyCellAttributeRect(
      column - 1, row, 3, 1, column - 1,
      ObjectEvent_GetById(LOCALID_SECOND_LOG)->z.fixed >> 20);
  MapGrid_CopyCellAttributes(0, 0, 3, 1, column - 1, row);
}

#undef IS_IN_LOG_CROSSING_COLUMNS

/// Walking south on the second log drifts it south.
static void EventScript_SlideSecondLogSouth(void) {
  s32 column;
  s32 row;
  s32 twelfthLogColumn;
  s32 seventhLogColumn;
  s32 ninthLogColumn;

  column = ObjectEvent_GetById(LOCALID_SECOND_LOG)->x.fixed >> 20;
  row = ObjectEvent_GetById(LOCALID_SECOND_LOG)->z.fixed >> 20;
  twelfthLogColumn = ObjectEvent_GetById(LOCALID_TWELFTH_LOG)->x.fixed >> 20;
  seventhLogColumn = ObjectEvent_GetById(LOCALID_SEVENTH_LOG)->x.fixed >> 20;
  ninthLogColumn = ObjectEvent_GetById(LOCALID_NINTH_LOG)->x.fixed >> 20;

  if (row == 8) {
    if ((u32)(ninthLogColumn - 9) <= 2) {
      return;
    }
    if ((u32)(seventhLogColumn - 9) <= 2) {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_SECOND_LOG, 0, 48);
    } else {
      if ((u32)(twelfthLogColumn - 9) > 2) {
        CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_SECOND_LOG, 0, 80);
      }
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_SECOND_LOG, 0, 96);
    }
  } else if (row == 11) {
    if ((u32)(seventhLogColumn - 9) <= 2) {
      return;
    }
    if ((u32)(twelfthLogColumn - 9) <= 2) {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_SECOND_LOG, 0, 48);
    } else {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_SECOND_LOG, 0, 128);
    }
  } else if (row == 12) {
    if ((u32)(seventhLogColumn - 9) <= 2) {
      return;
    }
    if ((u32)(twelfthLogColumn - 9) <= 2) {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_SECOND_LOG, 0, 32);
    } else {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_SECOND_LOG, 0, 112);
    }
  } else if (row == 14) {
    if ((u32)(twelfthLogColumn - 9) <= 2) {
      return;
    }
    CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_SECOND_LOG, 0, 80);
  } else if (row == 15) {
    CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_SECOND_LOG, 0, 64);
  } else if (row == 18) {
    CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_SECOND_LOG, 0, 16);
  }

  WaitFrames(2);

  MapGrid_CopyCellAttributeRect(
      column - 1, row, 3, 1, column - 1,
      ObjectEvent_GetById(LOCALID_SECOND_LOG)->z.fixed >> 20);
  MapGrid_CopyCellAttributes(0, 0, 3, 1, column - 1, row);
}

/// Walking west on the twelfth log drifts it west.
static void EventScript_SlideTwelfthLogWest(void) {
  s32 column;
  s32 row;
  s32 tenthLogRow;
  s32 eleventhLogRow;
  s32 secondLogRow;

  column = ObjectEvent_GetById(LOCALID_TWELFTH_LOG)->x.fixed >> 20;
  row = ObjectEvent_GetById(LOCALID_TWELFTH_LOG)->z.fixed >> 20;
  tenthLogRow = ObjectEvent_GetById(LOCALID_TENTH_LOG)->z.fixed >> 20;
  eleventhLogRow = ObjectEvent_GetById(LOCALID_ELEVENTH_LOG)->z.fixed >> 20;
  secondLogRow = ObjectEvent_GetById(LOCALID_SECOND_LOG)->z.fixed >> 20;
  if (column == 3) {
    return;
  }
  if (column == 13) {
    if (secondLogRow == 15) {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_TWELFTH_LOG, -16, 0);
    } else if (eleventhLogRow == 15) {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_TWELFTH_LOG, -64, 0);
    } else if (tenthLogRow == 15) {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_TWELFTH_LOG, -112, 0);
    } else {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_TWELFTH_LOG, -112, 0);
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_TWELFTH_LOG, -48, 0);
    }
  } else if (column == 6) {
    if (tenthLogRow == 15) {
      return;
    }
    CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_TWELFTH_LOG, -48, 0);
  } else if (column == 5) {
    CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_TWELFTH_LOG, -32, 0);
  } else if (column == 8) {
    if (eleventhLogRow == 15) {
      return;
    }
    if (tenthLogRow == 15) {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_TWELFTH_LOG, -32, 0);
    } else {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_TWELFTH_LOG, -80, 0);
    }
  } else if (column == 9) {
    if (eleventhLogRow == 15) {
      return;
    }
    if (tenthLogRow == 15) {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_TWELFTH_LOG, -48, 0);
    } else {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_TWELFTH_LOG, -96, 0);
    }
  } else if (column == 12) {
    if (secondLogRow == 15) {
      return;
    }
    if (eleventhLogRow == 15) {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_TWELFTH_LOG, -48, 0);
    } else if (tenthLogRow == 15) {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_TWELFTH_LOG, -96, 0);
    } else {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_TWELFTH_LOG, -144, 0);
    }
  }
  WaitFrames(2);
  MapGrid_CopyCellAttributeRect(
      column, row - 1, 1, 3,
      ObjectEvent_GetById(LOCALID_TWELFTH_LOG)->x.fixed >> 20, row - 1);
  MapGrid_CopyCellAttributes(0, 0, 1, 3, column, row - 1);
}

/// Walking east on the twelfth log drifts it east.
static void EventScript_SlideTwelfthLogEast(void) {
  s32 column;
  s32 row;
  s32 eleventhLogRow;
  s32 secondLogRow;

  column = ObjectEvent_GetById(LOCALID_TWELFTH_LOG)->x.fixed >> 20;
  row = ObjectEvent_GetById(LOCALID_TWELFTH_LOG)->z.fixed >> 20;
  eleventhLogRow = ObjectEvent_GetById(LOCALID_ELEVENTH_LOG)->z.fixed >> 20;
  secondLogRow = ObjectEvent_GetById(LOCALID_SECOND_LOG)->z.fixed >> 20;
  if (column == 3) {
    if (eleventhLogRow == 15) {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_TWELFTH_LOG, 32, 0);
    } else if (secondLogRow == 15) {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_TWELFTH_LOG, 80, 0);
    } else {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_TWELFTH_LOG, 112, 0);
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_TWELFTH_LOG, 48, 0);
    }
  } else if (column == 5) {
    if (eleventhLogRow == 15) {
      return;
    }
    if (secondLogRow == 15) {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_TWELFTH_LOG, 48, 0);
    } else {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_TWELFTH_LOG, 128, 0);
    }
  } else if (column == 6) {
    if (secondLogRow == 15) {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_TWELFTH_LOG, 32, 0);
    } else {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_TWELFTH_LOG, 112, 0);
    }
  } else if (column == 8) {
    if (secondLogRow == 15) {
      return;
    }
    CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_TWELFTH_LOG, 80, 0);
  } else if (column == 9) {
    CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_TWELFTH_LOG, 64, 0);
  } else if (column == 12) {
    CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_TWELFTH_LOG, 16, 0);
  }
  WaitFrames(2);
  MapGrid_CopyCellAttributeRect(
      column, row - 1, 1, 3,
      ObjectEvent_GetById(LOCALID_TWELFTH_LOG)->x.fixed >> 20, row - 1);
  MapGrid_CopyCellAttributes(0, 0, 1, 3, column, row - 1);
}

/// Walking west on the seventh log drifts it west.
static void EventScript_SlideSeventhLogWest(void) {
  s32 column;
  s32 row;
  s32 eleventhLogRow;
  s32 secondLogRow;
  s32 footprintRow;

  column = ObjectEvent_GetById(LOCALID_SEVENTH_LOG)->x.fixed >> 20;
  row = ObjectEvent_GetById(LOCALID_SEVENTH_LOG)->z.fixed >> 20;
  eleventhLogRow = ObjectEvent_GetById(LOCALID_ELEVENTH_LOG)->z.fixed >> 20;
  secondLogRow = ObjectEvent_GetById(LOCALID_SECOND_LOG)->z.fixed >> 20;

  if (column == 13) {
    if ((u32)(secondLogRow - 12) <= 2) {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_SEVENTH_LOG, -16, 0);
    } else if ((u32)(eleventhLogRow - 12) <= 2) {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_SEVENTH_LOG, -64, 0);
    } else {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_SEVENTH_LOG, -112, 0);
    }
  } else if (column == 12) {
    if ((u32)(secondLogRow - 12) <= 2) {
      return;
    }
    if ((u32)(eleventhLogRow - 12) <= 2) {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_SEVENTH_LOG, -48, 0);
    } else {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_SEVENTH_LOG, -96, 0);
    }
  } else if (column == 9) {
    if ((u32)(eleventhLogRow - 12) <= 2) {
      return;
    }
    CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_SEVENTH_LOG, -48, 0);
  } else if (column == 8) {
    if ((u32)(eleventhLogRow - 12) <= 2) {
      return;
    }
    CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_SEVENTH_LOG, -32, 0);
  } else if (column == 6) {
    return;
  }

  WaitFrames(2);

  footprintRow = row - 1;
  MapGrid_CopyCellAttributeRect(
      column, footprintRow, 1, 3,
      ObjectEvent_GetById(LOCALID_SEVENTH_LOG)->x.fixed >> 20, footprintRow);
  MapGrid_CopyCellAttributes(0, 0, 1, 3, column, footprintRow);
}

/// Walking east on the seventh log drifts it east.
static void EventScript_SlideSeventhLogEast(void) {
  s32 column;
  s32 row;
  s32 eleventhLogRow;
  s32 secondLogRow;
  s32 footprintRow;

  column = ObjectEvent_GetById(LOCALID_SEVENTH_LOG)->x.fixed >> 20;
  row = ObjectEvent_GetById(LOCALID_SEVENTH_LOG)->z.fixed >> 20;
  eleventhLogRow = ObjectEvent_GetById(LOCALID_ELEVENTH_LOG)->z.fixed >> 20;
  secondLogRow = ObjectEvent_GetById(LOCALID_SECOND_LOG)->z.fixed >> 20;

  if (column == 6) {
    if ((u32)(secondLogRow - 12) <= 2) {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_SEVENTH_LOG, 32, 0);
    } else if ((u32)(eleventhLogRow - 12) <= 2) {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_SEVENTH_LOG, 64, 0);
    } else {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_SEVENTH_LOG, 112, 0);
    }
  } else if (column == 8) {
    if ((u32)(secondLogRow - 12) <= 2) {
      return;
    }
    CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_SEVENTH_LOG, 80, 0);
  } else if (column == 9) {
    if ((u32)(secondLogRow - 12) <= 2) {
      return;
    }
    CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_SEVENTH_LOG, 64, 0);
  } else if (column == 12) {
    CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_SEVENTH_LOG, 16, 0);
  } else if (column == 13) {
    return;
  }

  WaitFrames(2);

  footprintRow = row - 1;
  MapGrid_CopyCellAttributeRect(
      column, footprintRow, 1, 3,
      ObjectEvent_GetById(LOCALID_SEVENTH_LOG)->x.fixed >> 20, footprintRow);
  MapGrid_CopyCellAttributes(0, 0, 1, 3, column, footprintRow);
}

/// Walking west on the ninth log drifts it west.
static void EventScript_SlideNinthLogWest(void) {
  s32 column;
  s32 row;
  s32 eleventhLogRow;
  s32 secondLogRow;
  s32 footprintRow;

  column = ObjectEvent_GetById(LOCALID_NINTH_LOG)->x.fixed >> 20;
  row = ObjectEvent_GetById(LOCALID_NINTH_LOG)->z.fixed >> 20;
  eleventhLogRow = ObjectEvent_GetById(LOCALID_ELEVENTH_LOG)->z.fixed >> 20;
  secondLogRow = ObjectEvent_GetById(LOCALID_SECOND_LOG)->z.fixed >> 20;

  if (column == 13) {
    if ((u32)(secondLogRow - 9) <= 2) {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_NINTH_LOG, -16, 0);
    } else if ((u32)(eleventhLogRow - 9) <= 2) {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_NINTH_LOG, -64, 0);
    } else {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_NINTH_LOG, -112, 0);
    }
  } else if (column == 12) {
    if ((u32)(secondLogRow - 9) <= 2) {
      return;
    }
    if ((u32)(eleventhLogRow - 9) <= 2) {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_NINTH_LOG, -48, 0);
    } else {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_NINTH_LOG, -96, 0);
    }
  } else if (column == 9) {
    if ((u32)(eleventhLogRow - 9) <= 2) {
      return;
    }
    CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_NINTH_LOG, -48, 0);
  } else if (column == 8) {
    CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_NINTH_LOG, -32, 0);
  } else if (column == 6) {
    return;
  }

  WaitFrames(2);

  footprintRow = row - 1;
  MapGrid_CopyCellAttributeRect(
      column, footprintRow, 1, 3,
      ObjectEvent_GetById(LOCALID_NINTH_LOG)->x.fixed >> 20, footprintRow);
  MapGrid_CopyCellAttributes(0, 0, 1, 3, column, footprintRow);
}

/// Walking east on the ninth log drifts it east.
static void EventScript_SlideNinthLogEast(void) {
  s32 column;
  s32 row;
  s32 companionRow;

  column = ObjectEvent_GetById(LOCALID_NINTH_LOG)->x.fixed >> 20;
  row = ObjectEvent_GetById(LOCALID_NINTH_LOG)->z.fixed >> 20;
  companionRow = ObjectEvent_GetById(LOCALID_SECOND_LOG)->z.fixed >> 20;

  if (column == 6) {
    if ((u32)(companionRow - 9) <= 2) {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_NINTH_LOG, 32, 0);
    } else {
      CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_NINTH_LOG, 112, 0);
    }
  } else if (column == 8) {
    if ((u32)(companionRow - 9) <= 2) {
      return;
    }
    CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_NINTH_LOG, 80, 0);
  } else if (column == 9) {
    CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_NINTH_LOG, 64, 0);
  } else if (column == 12) {
    CrossboneIsle_MoveSelectedObjectEventAndLog(LOCALID_NINTH_LOG, 16, 0);
  } else if (column == 13) {
    return;
  }

  WaitFrames(2);

  MapGrid_CopyCellAttributeRect(
      column, row - 1, 1, 3,
      ObjectEvent_GetById(LOCALID_NINTH_LOG)->x.fixed >> 20, row - 1);
  MapGrid_CopyCellAttributes(0, 0, 1, 3, column, row - 1);
}

#include "common/object_orbit.h"

/// Pixel step by direction: X in the upper halfword, Z in the lower.
#define DIRECTION_STEP(dx, dz) (((u32)(dx) << 16) | ((dz) & 0xffff))

s32 gPushedObject_DirectionSteps[] = {
    DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(16, 0),
};

// clang-format off
u32 gPushedObject_FootprintKinds[] = {
    207, 205, 228, 229, 298, 297,
};

s32 gPushedObject_FootprintBounds[] = {
    -32, -8, 32, 8,
    -8, -32, 8, 32,
    -32, -16, 32, 0,
    -8, -32, 8, 32,
    -32, -8, 32, 8,
    -8, -32, 8, 32,
};

s32 gFieldEffect_ShortScript[] = {
    SC_WAIT(1),
    SC_LOOP(22, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
};

s32 gFieldEffect_MediumScript[] = {
    SC_WAIT(1),
    SC_LOOP(44, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
};

s32 gFieldEffect_LongScript[] = {
    SC_WAIT(1),
    SC_LOOP(126, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
};

u16 gCrossboneIsle_EntranceCells[] = {
    MAPGRID_ANIMATION_STEP(58, 23, 1, 2, 5), MAPGRID_ANIMATION_STEP(59, 23, 1, 2, 5), MAPGRID_ANIMATION_STEPS_END,
};

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
    (const s32 *)0xffff0000,
    (const s32 *)120,
    (const s32 *)0x40000098,
    (const s32 *)0xffff0000,
    (const s32 *)-1,
    (const s32 *)0xffff,
    (const s32 *)0xffff,
    0,
    0,
    0,
    0,
    0,
};

struct MapWarpDest gCrossboneIsle_IsleWarpDests[] = {
    { CROSSBONE_ISLE_FROM_WORLD_MAP, CONDITION_ALWAYS, 280, 0, 544, FACING_NORTH, 0, 0, 0, 512, 560, 0 },
    { CROSSBONE_ISLE_FROM_CROSSBONE_ISLE_CAVE_ENTRANCE, CONDITION_ALWAYS, 136, 0, 360, FACING_SOUTH, 0, 0, 0, 512, 560, 0 },
    { MAP_TABLE_END },
};

struct MapWarpDest gCrossboneIsle_CaveEntranceWarpDests[] = {
    { CROSSBONE_ISLE_CAVE_ENTRANCE_FROM_CROSSBONE_ISLE, CONDITION_ALWAYS, 312, 0, 520, FACING_NORTH, 0, 16, 64, 416, 544, 0 },
    { CROSSBONE_ISLE_CAVE_ENTRANCE_FROM_CROSSBONE_ISLE_FIRST_FLOOR, CONDITION_ALWAYS, 280, 0, 144, FACING_EAST, 0, 16, 64, 416, 544, 0 },
    { MAP_TABLE_END },
};

struct MapWarpDest gCrossboneIsle_FloatingLogsWarpDests[] = {
    { CROSSBONE_ISLE_FLOATING_LOGS_FROM_CROSSBONE_ISLE_NINTH_FLOOR_EAST, CONDITION_ALWAYS, 312, 0, 408, FACING_NORTH, 0, 0, 16, 640, 464, 0 },
    { CROSSBONE_ISLE_FLOATING_LOGS_FROM_CROSSBONE_ISLE_NINTH_FLOOR_WEST, CONDITION_ALWAYS, 200, 0, 408, FACING_NORTH, 0, 0, 16, 640, 464, 0 },
    { CROSSBONE_ISLE_FLOATING_LOGS_MIDDLE, CONDITION_ALWAYS, 272, 0, 232, FACING_SOUTH, 0, 0, 16, 640, 464, 0 },
    { MAP_TABLE_END },
};

struct MapWarpDest gCrossboneIsle_PierWarpDests[] = {
    { CROSSBONE_ISLE_PIER_FROM_CROSSBONE_ISLE_NINTH_FLOOR, CONDITION_ALWAYS, 184, 0, 136, FACING_WEST, 0, -1, -1, -1, -1, 0 },
    { CROSSBONE_ISLE_PIER_FROM_PIRATE_SHIP, CONDITION_ALWAYS, 40, 0, 184, FACING_EAST, 0, -1, -1, -1, -1, 0 },
    { MAP_TABLE_END },
};

struct MapWarpDest gCrossboneIsle_PirateShipWarpDests[] = {
    { PIRATE_SHIP_FROM_CROSSBONE_ISLE_PIER, CONDITION_ALWAYS, 312, 0, 656, FACING_WEST, 0, -1, -1, -1, -1, 0 },
    { WARP_DEST_AFTER_BATTLE, CONDITION_ALWAYS, 216, 0, 544, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    // The nine corridor rooms (CrossboneIsleFirstFloor to
    // CrossboneIsleNinthFloor) enter through this shared tail, since every room
    // numbers its warp destinations alike.

    { CORRIDOR_FROM_PREVIOUS_ROOM, CONDITION_ALWAYS, 840, 0, 176, FACING_WEST, 0, 432, 32, 880, 352, 0 },
    { CORRIDOR_FROM_NEXT_ROOM, CONDITION_ALWAYS, 488, 0, 176, FACING_EAST, 0, 432, 32, 880, 352, 0 },
    { CORRIDOR_FROM_SIDE_ROOM_EAST, CONDITION_ALWAYS, 712, 0, 152, FACING_SOUTH, 0, 432, 32, 880, 352, 0 },
    { CORRIDOR_FROM_SIDE_ROOM_WEST, CONDITION_ALWAYS, 600, 0, 152, FACING_SOUTH, 0, 432, 32, 880, 352, 0 },
    { WARP_DEST_AFTER_BATTLE, CONDITION_ALWAYS, 712, 0, 152, FACING_NORTH, 0, 432, 32, 880, 352, 0 },
    { MAP_TABLE_END },
};

u32 gCrossboneIsleWarps[] = {
    MAP_WARPS(CrossboneIsle),
    MAP_WARP(CROSSBONE_ISLE_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_CROSSBONE_ISLE),
    MAP_WARP(CROSSBONE_ISLE_TO_CROSSBONE_ISLE_CAVE_ENTRANCE, CrossboneIsleCaveEntrance, CROSSBONE_ISLE_CAVE_ENTRANCE_FROM_CROSSBONE_ISLE),
    MAP_WARPS(CrossboneIsleCaveEntrance),
    MAP_WARP(CROSSBONE_ISLE_CAVE_ENTRANCE_TO_CROSSBONE_ISLE, CrossboneIsle, CROSSBONE_ISLE_FROM_CROSSBONE_ISLE_CAVE_ENTRANCE),
    MAP_WARP(CROSSBONE_ISLE_CAVE_ENTRANCE_TO_CROSSBONE_ISLE_FIRST_FLOOR, CrossboneIsleFirstFloor, CROSSBONE_ISLE_FIRST_FLOOR_FROM_CROSSBONE_ISLE_CAVE_ENTRANCE),
    MAP_WARPS(CrossboneIsleFloatingLogs),
    MAP_WARP(CROSSBONE_ISLE_FLOATING_LOGS_TO_CROSSBONE_ISLE_NINTH_FLOOR_EAST, CrossboneIsleNinthFloor, CROSSBONE_ISLE_NINTH_FLOOR_FROM_CROSSBONE_ISLE_FLOATING_LOGS_EAST),
    MAP_WARP(CROSSBONE_ISLE_FLOATING_LOGS_TO_CROSSBONE_ISLE_NINTH_FLOOR_WEST, CrossboneIsleNinthFloor, CROSSBONE_ISLE_NINTH_FLOOR_FROM_CROSSBONE_ISLE_FLOATING_LOGS_WEST),
    MAP_WARPS(CrossboneIslePier),
    MAP_WARP(CROSSBONE_ISLE_PIER_TO_CROSSBONE_ISLE_NINTH_FLOOR, CrossboneIsleNinthFloor, CROSSBONE_ISLE_NINTH_FLOOR_FROM_CROSSBONE_ISLE_PIER),
    MAP_WARP(CROSSBONE_ISLE_PIER_TO_PIRATE_SHIP, PirateShip, PIRATE_SHIP_FROM_CROSSBONE_ISLE_PIER),
    MAP_WARPS(PirateShip),
    MAP_WARP(PIRATE_SHIP_TO_CROSSBONE_ISLE_PIER, CrossboneIslePier, CROSSBONE_ISLE_PIER_FROM_PIRATE_SHIP),
    MAP_WARPS(CrossboneIsleFirstFloor),
    MAP_WARP(CROSSBONE_ISLE_FIRST_FLOOR_TO_CROSSBONE_ISLE_CAVE_ENTRANCE, CrossboneIsleCaveEntrance, CROSSBONE_ISLE_CAVE_ENTRANCE_FROM_CROSSBONE_ISLE_FIRST_FLOOR),
    MAP_WARP(CROSSBONE_ISLE_FIRST_FLOOR_TO_CROSSBONE_ISLE_SECOND_FLOOR, CrossboneIsleSecondFloor, CROSSBONE_ISLE_SECOND_FLOOR_FROM_CROSSBONE_ISLE_FIRST_FLOOR),
    MAP_WARP(CROSSBONE_ISLE_FIRST_FLOOR_TO_CROSSBONE_ISLE_STEPPING_STONES_EAST, CrossboneIsleSteppingStones, CROSSBONE_ISLE_STEPPING_STONES_FROM_CROSSBONE_ISLE_FIRST_FLOOR_EAST),
    MAP_WARP(CROSSBONE_ISLE_FIRST_FLOOR_TO_CROSSBONE_ISLE_STEPPING_STONES_WEST, CrossboneIsleSteppingStones, CROSSBONE_ISLE_STEPPING_STONES_FROM_CROSSBONE_ISLE_FIRST_FLOOR_WEST),
    MAP_WARPS(CrossboneIsleSecondFloor),
    MAP_WARP(CROSSBONE_ISLE_SECOND_FLOOR_TO_CROSSBONE_ISLE_FIRST_FLOOR, CrossboneIsleFirstFloor, CROSSBONE_ISLE_FIRST_FLOOR_FROM_CROSSBONE_ISLE_SECOND_FLOOR),
    MAP_WARP(CROSSBONE_ISLE_SECOND_FLOOR_TO_CROSSBONE_ISLE_THIRD_FLOOR, CrossboneIsleThirdFloor, CROSSBONE_ISLE_THIRD_FLOOR_FROM_CROSSBONE_ISLE_SECOND_FLOOR),
    MAP_WARP(CROSSBONE_ISLE_SECOND_FLOOR_TO_CROSSBONE_ISLE_ICE_PILLARS_EAST, CrossboneIsleIcePillars, CROSSBONE_ISLE_ICE_PILLARS_FROM_CROSSBONE_ISLE_SECOND_FLOOR_EAST),
    MAP_WARP(CROSSBONE_ISLE_SECOND_FLOOR_TO_CROSSBONE_ISLE_ICE_PILLARS_WEST, CrossboneIsleIcePillars, CROSSBONE_ISLE_ICE_PILLARS_FROM_CROSSBONE_ISLE_SECOND_FLOOR_WEST),
    MAP_WARPS(CrossboneIsleThirdFloor),
    MAP_WARP(CROSSBONE_ISLE_THIRD_FLOOR_TO_CROSSBONE_ISLE_SECOND_FLOOR, CrossboneIsleSecondFloor, CROSSBONE_ISLE_SECOND_FLOOR_FROM_CROSSBONE_ISLE_THIRD_FLOOR),
    MAP_WARP(CROSSBONE_ISLE_THIRD_FLOOR_TO_CROSSBONE_ISLE_FOURTH_FLOOR, CrossboneIsleFourthFloor, CROSSBONE_ISLE_FOURTH_FLOOR_FROM_CROSSBONE_ISLE_THIRD_FLOOR),
    MAP_WARP(CROSSBONE_ISLE_THIRD_FLOOR_TO_CROSSBONE_ISLE_KEYS_EAST, CrossboneIsleKeys, CROSSBONE_ISLE_KEYS_FROM_CROSSBONE_ISLE_THIRD_FLOOR_EAST),
    MAP_WARP(CROSSBONE_ISLE_THIRD_FLOOR_TO_CROSSBONE_ISLE_KEYS_WEST, CrossboneIsleKeys, CROSSBONE_ISLE_KEYS_FROM_CROSSBONE_ISLE_THIRD_FLOOR_WEST),
    MAP_WARPS(CrossboneIsleFourthFloor),
    MAP_WARP(CROSSBONE_ISLE_FOURTH_FLOOR_TO_CROSSBONE_ISLE_THIRD_FLOOR, CrossboneIsleThirdFloor, CROSSBONE_ISLE_THIRD_FLOOR_FROM_CROSSBONE_ISLE_FOURTH_FLOOR),
    MAP_WARP(CROSSBONE_ISLE_FOURTH_FLOOR_TO_CROSSBONE_ISLE_FIFTH_FLOOR, CrossboneIsleFifthFloor, CROSSBONE_ISLE_FIFTH_FLOOR_FROM_CROSSBONE_ISLE_FOURTH_FLOOR),
    MAP_WARP(CROSSBONE_ISLE_FOURTH_FLOOR_TO_CROSSBONE_ISLE_STATUES_EAST, CrossboneIsleStatues, CROSSBONE_ISLE_STATUES_FROM_CROSSBONE_ISLE_FOURTH_FLOOR_EAST),
    MAP_WARP(CROSSBONE_ISLE_FOURTH_FLOOR_TO_CROSSBONE_ISLE_STATUES_WEST, CrossboneIsleStatues, CROSSBONE_ISLE_STATUES_FROM_CROSSBONE_ISLE_FOURTH_FLOOR_WEST),
    MAP_WARPS(CrossboneIsleFifthFloor),
    MAP_WARP(CROSSBONE_ISLE_FIFTH_FLOOR_TO_CROSSBONE_ISLE_FOURTH_FLOOR, CrossboneIsleFourthFloor, CROSSBONE_ISLE_FOURTH_FLOOR_FROM_CROSSBONE_ISLE_FIFTH_FLOOR),
    MAP_WARP(CROSSBONE_ISLE_FIFTH_FLOOR_TO_CROSSBONE_ISLE_SIXTH_FLOOR, CrossboneIsleSixthFloor, CROSSBONE_ISLE_SIXTH_FLOOR_FROM_CROSSBONE_ISLE_FIFTH_FLOOR),
    MAP_WARP(CROSSBONE_ISLE_FIFTH_FLOOR_TO_CROSSBONE_ISLE_FLOODGATE_EAST, CrossboneIsleFloodgate, CROSSBONE_ISLE_FLOODGATE_FROM_CROSSBONE_ISLE_FIFTH_FLOOR_EAST),
    MAP_WARP(CROSSBONE_ISLE_FIFTH_FLOOR_TO_CROSSBONE_ISLE_FLOODGATE_WEST, CrossboneIsleFloodgate, CROSSBONE_ISLE_FLOODGATE_FROM_CROSSBONE_ISLE_FIFTH_FLOOR_WEST),
    MAP_WARPS(CrossboneIsleSixthFloor),
    MAP_WARP(CROSSBONE_ISLE_SIXTH_FLOOR_TO_CROSSBONE_ISLE_FIFTH_FLOOR, CrossboneIsleFifthFloor, CROSSBONE_ISLE_FIFTH_FLOOR_FROM_CROSSBONE_ISLE_SIXTH_FLOOR),
    MAP_WARP(CROSSBONE_ISLE_SIXTH_FLOOR_TO_CROSSBONE_ISLE_SEVENTH_FLOOR, CrossboneIsleSeventhFloor, CROSSBONE_ISLE_SEVENTH_FLOOR_FROM_CROSSBONE_ISLE_SIXTH_FLOOR),
    MAP_WARP(CROSSBONE_ISLE_SIXTH_FLOOR_TO_CROSSBONE_ISLE_BANE_EAST, CrossboneIsleBane, CROSSBONE_ISLE_BANE_FROM_CROSSBONE_ISLE_SIXTH_FLOOR_EAST),
    MAP_WARP(CROSSBONE_ISLE_SIXTH_FLOOR_TO_CROSSBONE_ISLE_BANE_WEST, CrossboneIsleBane, CROSSBONE_ISLE_BANE_FROM_CROSSBONE_ISLE_SIXTH_FLOOR_WEST),
    MAP_WARPS(CrossboneIsleSeventhFloor),
    MAP_WARP(CROSSBONE_ISLE_SEVENTH_FLOOR_TO_CROSSBONE_ISLE_SIXTH_FLOOR, CrossboneIsleSixthFloor, CROSSBONE_ISLE_SIXTH_FLOOR_FROM_CROSSBONE_ISLE_SEVENTH_FLOOR),
    MAP_WARP(CROSSBONE_ISLE_SEVENTH_FLOOR_TO_CROSSBONE_ISLE_EIGHTH_FLOOR, CrossboneIsleEighthFloor, CROSSBONE_ISLE_EIGHTH_FLOOR_FROM_CROSSBONE_ISLE_SEVENTH_FLOOR),
    MAP_WARP(CROSSBONE_ISLE_SEVENTH_FLOOR_TO_CROSSBONE_ISLE_STACKED_PILLARS_EAST, CrossboneIsleStackedPillars, CROSSBONE_ISLE_STACKED_PILLARS_FROM_CROSSBONE_ISLE_SEVENTH_FLOOR_EAST),
    MAP_WARP(CROSSBONE_ISLE_SEVENTH_FLOOR_TO_CROSSBONE_ISLE_STACKED_PILLARS_WEST, CrossboneIsleStackedPillars, CROSSBONE_ISLE_STACKED_PILLARS_FROM_CROSSBONE_ISLE_SEVENTH_FLOOR_WEST),
    MAP_WARPS(CrossboneIsleEighthFloor),
    MAP_WARP(CROSSBONE_ISLE_EIGHTH_FLOOR_TO_CROSSBONE_ISLE_SEVENTH_FLOOR, CrossboneIsleSeventhFloor, CROSSBONE_ISLE_SEVENTH_FLOOR_FROM_CROSSBONE_ISLE_EIGHTH_FLOOR),
    MAP_WARP(CROSSBONE_ISLE_EIGHTH_FLOOR_TO_CROSSBONE_ISLE_NINTH_FLOOR, CrossboneIsleNinthFloor, CROSSBONE_ISLE_NINTH_FLOOR_FROM_CROSSBONE_ISLE_EIGHTH_FLOOR),
    MAP_WARP(CROSSBONE_ISLE_EIGHTH_FLOOR_TO_CROSSBONE_ISLE_DRAGON_STATUE_EAST, CrossboneIsleDragonStatue, CROSSBONE_ISLE_DRAGON_STATUE_FROM_CROSSBONE_ISLE_EIGHTH_FLOOR_EAST),
    MAP_WARP(CROSSBONE_ISLE_EIGHTH_FLOOR_TO_CROSSBONE_ISLE_DRAGON_STATUE_WEST, CrossboneIsleDragonStatue, CROSSBONE_ISLE_DRAGON_STATUE_FROM_CROSSBONE_ISLE_EIGHTH_FLOOR_WEST),
    MAP_WARPS(CrossboneIsleNinthFloor),
    MAP_WARP(CROSSBONE_ISLE_NINTH_FLOOR_TO_CROSSBONE_ISLE_EIGHTH_FLOOR, CrossboneIsleEighthFloor, CROSSBONE_ISLE_EIGHTH_FLOOR_FROM_CROSSBONE_ISLE_NINTH_FLOOR),
    MAP_WARP(CROSSBONE_ISLE_NINTH_FLOOR_TO_CROSSBONE_ISLE_PIER, CrossboneIslePier, CROSSBONE_ISLE_PIER_FROM_CROSSBONE_ISLE_NINTH_FLOOR),
    MAP_WARP(CROSSBONE_ISLE_NINTH_FLOOR_TO_CROSSBONE_ISLE_FLOATING_LOGS_EAST, CrossboneIsleFloatingLogs, CROSSBONE_ISLE_FLOATING_LOGS_FROM_CROSSBONE_ISLE_NINTH_FLOOR_EAST),
    MAP_WARP(CROSSBONE_ISLE_NINTH_FLOOR_TO_CROSSBONE_ISLE_FLOATING_LOGS_WEST, CrossboneIsleFloatingLogs, CROSSBONE_ISLE_FLOATING_LOGS_FROM_CROSSBONE_ISLE_NINTH_FLOOR_WEST),
    MAP_WARPS_END,
};

struct ObjectEventTemplate gCrossboneIsle_NoObjectEventTemplates[] = {
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gCrossboneIsle_IsleObjectEventTemplates[] = {
    { SPRITE_BLANK, FLAG_CROSSBONE_ISLE_NUT_CAUGHT, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(344), 0, PIXELS(280), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(144), 0, PIXELS(392), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(352), 0, PIXELS(376), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(88), 0, PIXELS(368), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(104), 0, PIXELS(336), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(152), 0, PIXELS(320), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_IVY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(136), 0, PIXELS(352), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_IVY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_IVY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(88), 0, PIXELS(312), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gCrossboneIsle_FloatingLogsObjectEventTemplates[] = {
    { SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(408), 0, PIXELS(312), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(168), 0, PIXELS(312), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(520), 0, PIXELS(296), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(584), 0, PIXELS(280), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(552), 0, PIXELS(200), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(584), 0, PIXELS(136), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(216), 0, PIXELS(216), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(392), 0, PIXELS(136), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(216), 0, PIXELS(168), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(72), 0, PIXELS(312), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(120), 0, PIXELS(312), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(216), 0, PIXELS(264), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gCrossboneIsle_CorridorObjectEventTemplates[] = {
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(632), 0, PIXELS(168), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

struct MapEvent gCrossboneIsle_CorridorEvents[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, CORRIDOR_TO_PREVIOUS_ROOM },
    { MAP_EVENT_WARP, 2, CONDITION_ALWAYS, CORRIDOR_TO_NEXT_ROOM },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_OpenEastDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_OpenWestDoor) },
    { MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING, 20, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_PushPillar) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_SINKING_PILLAR, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_MovePillar) },
    { BG_EVENT(BG_EVENT_CHEST), 100, FLAG_CROSSBONE_ISLE_FIRST_FLOOR_MIMIC_BEATEN, FIND_MIMIC | 4 },
    { MAP_EVENTS_END },
};

struct MapEvent gCrossboneIsle_CaveEntranceEvents[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, CROSSBONE_ISLE_CAVE_ENTRANCE_TO_CROSSBONE_ISLE },
    { MAP_EVENT_WARP, 2, CONDITION_ALWAYS, CROSSBONE_ISLE_CAVE_ENTRANCE_TO_CROSSBONE_ISLE_FIRST_FLOOR },
    { MAP_EVENTS_END },
};

struct MapEvent gCrossboneIsle_IsleEvents[] = {
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_CATCH), LOCALID_ISLE_NUT, FLAG_CROSSBONE_ISLE_NUT_CAUGHT, EVENT_SCRIPT(EventScript_CatchNut) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_GROWTH), LOCALID_FIRST_PLANT, FLAG_CROSSBONE_ISLE_FIRST_PLANT_GROWN, EVENT_SCRIPT(EventScript_UnusedGrowFirstPlant) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_GROWTH), LOCALID_FIRST_PLANT + 1, FLAG_CROSSBONE_ISLE_FIRST_PLANT_GROWN + 1, EVENT_SCRIPT(EventScript_GrowSecondPlant) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_GROWTH), LOCALID_FIRST_PLANT + 2, FLAG_CROSSBONE_ISLE_FIRST_PLANT_GROWN + 2, EVENT_SCRIPT(EventScript_GrowThirdPlant) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_GROWTH), LOCALID_FIRST_PLANT + 3, FLAG_CROSSBONE_ISLE_FIRST_PLANT_GROWN + 3, EVENT_SCRIPT(EventScript_GrowFourthPlant) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_WHIRLWIND), LOCALID_FIRST_IVY, FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY, EVENT_SCRIPT(EventScript_BlowAwayFirstIvy) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_WHIRLWIND), LOCALID_FIRST_IVY + 1, FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY + 1, EVENT_SCRIPT(EventScript_UnusedBlowAwaySecondIvy) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_WHIRLWIND), LOCALID_FIRST_IVY + 2, FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY + 2, EVENT_SCRIPT(EventScript_BlowAwayThirdIvy) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_WHIRLWIND), LOCALID_FIRST_IVY + 3, FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY + 3, EVENT_SCRIPT(EventScript_UnusedBlowAwayFourthIvy) },
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, CROSSBONE_ISLE_TO_WORLD_MAP },
    { MAP_EVENT_WARP, 2, CONDITION_ALWAYS, CROSSBONE_ISLE_TO_CROSSBONE_ISLE_CAVE_ENTRANCE },
    { MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING, 20, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_PushIsleLog) },
    { MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING, 10, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_PushIsleLog) },
    { BG_EVENT_CHEST_FACING(FACING_WEST), 100, FLAG_CROSSBONE_ISLE_COIN, FIND_COINS(1) },
    { BG_EVENT_CHEST_FACING(FACING_NORTH), 100, FLAG_CROSSBONE_ISLE_COIN, FIND_COINS(1) },
    { BG_EVENT_CHEST_FACING(FACING_NORTHWEST), 100, FLAG_CROSSBONE_ISLE_COIN, FIND_COINS(1) },
    { BG_EVENT_CHEST_FACING(FACING_WEST), 101, FLAG_CROSSBONE_ISLE_MINT, FIND_ITEM(ITEM_MINT) },
    { BG_EVENT_CHEST_FACING(FACING_NORTH), 101, FLAG_CROSSBONE_ISLE_MINT, FIND_ITEM(ITEM_MINT) },
    { BG_EVENT_CHEST_FACING(FACING_NORTHWEST), 101, FLAG_CROSSBONE_ISLE_MINT, FIND_ITEM(ITEM_MINT) },
    { MAP_EVENTS_END },
};

struct MapEvent gCrossboneIsle_FloatingLogsEvents[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, CROSSBONE_ISLE_FLOATING_LOGS_TO_CROSSBONE_ISLE_NINTH_FLOOR_EAST },
    { MAP_EVENT_WARP, 2, CONDITION_ALWAYS, CROSSBONE_ISLE_FLOATING_LOGS_TO_CROSSBONE_ISLE_NINTH_FLOOR_WEST },
    { COORD_EVENT_WALKING_INTO(FACING_WEST), 40, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_HopWest) },
    { COORD_EVENT_WALKING_INTO(FACING_EAST), 41, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_HopEast) },
    { COORD_EVENT_WALKING_INTO(FACING_SOUTH), 42, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_HopSouth) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 43, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_HopNorth) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 21, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SlideFirstLogNorth) },
    { COORD_EVENT_WALKING_INTO(FACING_SOUTH), 21, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SlideFirstLogSouth) },
    { COORD_EVENT_WALKING_INTO(FACING_EAST), 21, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_HopEast) },
    { COORD_EVENT_WALKING_INTO(FACING_WEST), 21, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_HopWest) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 22, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SlideThirdLogNorth) },
    { COORD_EVENT_WALKING_INTO(FACING_SOUTH), 22, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SlideThirdLogSouth) },
    { COORD_EVENT_WALKING_INTO(FACING_EAST), 22, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_HopEast) },
    { COORD_EVENT_WALKING_INTO(FACING_WEST), 22, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_HopWest) },
    { COORD_EVENT_WALKING_INTO(FACING_WEST), 23, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SlideFourthLogWest) },
    { COORD_EVENT_WALKING_INTO(FACING_EAST), 23, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SlideFourthLogEast) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 23, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_HopNorth) },
    { COORD_EVENT_WALKING_INTO(FACING_SOUTH), 23, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_HopSouth) },
    { COORD_EVENT_WALKING_INTO(FACING_WEST), 24, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SlideFifthLogWest) },
    { COORD_EVENT_WALKING_INTO(FACING_EAST), 24, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SlideFifthLogEast) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 24, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_HopNorth) },
    { COORD_EVENT_WALKING_INTO(FACING_SOUTH), 24, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_HopSouth) },
    { COORD_EVENT_WALKING_INTO(FACING_WEST), 25, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SlideSixthLogWest) },
    { COORD_EVENT_WALKING_INTO(FACING_EAST), 25, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SlideSixthLogEast) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 25, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_HopNorth) },
    { COORD_EVENT_WALKING_INTO(FACING_SOUTH), 25, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_HopSouth) },
    { COORD_EVENT_WALKING_INTO(FACING_WEST), 26, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SlideEighthLogWest) },
    { COORD_EVENT_WALKING_INTO(FACING_EAST), 26, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SlideEighthLogEast) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 26, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_HopNorth) },
    { COORD_EVENT_WALKING_INTO(FACING_SOUTH), 26, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_HopSouth) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 31, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SlideTenthLogNorth) },
    { COORD_EVENT_WALKING_INTO(FACING_SOUTH), 31, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SlideTenthLogSouth) },
    { COORD_EVENT_WALKING_INTO(FACING_EAST), 31, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_HopEast) },
    { COORD_EVENT_WALKING_INTO(FACING_WEST), 31, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_HopWest) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 32, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SlideEleventhLogNorth) },
    { COORD_EVENT_WALKING_INTO(FACING_SOUTH), 32, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SlideEleventhLogSouth) },
    { COORD_EVENT_WALKING_INTO(FACING_EAST), 32, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_HopEast) },
    { COORD_EVENT_WALKING_INTO(FACING_WEST), 32, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_HopWest) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 33, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SlideSecondLogNorth) },
    { COORD_EVENT_WALKING_INTO(FACING_SOUTH), 33, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SlideSecondLogSouth) },
    { COORD_EVENT_WALKING_INTO(FACING_EAST), 33, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_HopEast) },
    { COORD_EVENT_WALKING_INTO(FACING_WEST), 33, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_HopWest) },
    { COORD_EVENT_WALKING_INTO(FACING_WEST), 34, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SlideTwelfthLogWest) },
    { COORD_EVENT_WALKING_INTO(FACING_EAST), 34, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SlideTwelfthLogEast) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 34, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_HopNorth) },
    { COORD_EVENT_WALKING_INTO(FACING_SOUTH), 34, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_HopSouth) },
    { COORD_EVENT_WALKING_INTO(FACING_WEST), 35, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SlideSeventhLogWest) },
    { COORD_EVENT_WALKING_INTO(FACING_EAST), 35, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SlideSeventhLogEast) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 35, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_HopNorth) },
    { COORD_EVENT_WALKING_INTO(FACING_SOUTH), 35, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_HopSouth) },
    { COORD_EVENT_WALKING_INTO(FACING_WEST), 36, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SlideNinthLogWest) },
    { COORD_EVENT_WALKING_INTO(FACING_EAST), 36, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SlideNinthLogEast) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 36, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_HopNorth) },
    { COORD_EVENT_WALKING_INTO(FACING_SOUTH), 36, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_HopSouth) },
    { MAP_EVENT_COORD, 21, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_StepOnFirstLog) },
    { MAP_EVENT_COORD, 22, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_StepOnThirdLog) },
    { MAP_EVENT_COORD, 23, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_StepOnFourthLog) },
    { MAP_EVENT_COORD, 24, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_StepOnFifthLog) },
    { MAP_EVENT_COORD, 25, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_StepOnSixthLog) },
    { MAP_EVENT_COORD, 26, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_StepOnEighthLog) },
    { MAP_EVENT_COORD, 31, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_StepOnTenthLog) },
    { MAP_EVENT_COORD, 32, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_StepOnEleventhLog) },
    { MAP_EVENT_COORD, 33, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_StepOnSecondLog) },
    { MAP_EVENT_COORD, 34, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_StepOnTwelfthLog) },
    { MAP_EVENT_COORD, 35, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_StepOnSeventhLog) },
    { MAP_EVENT_COORD, 36, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_StepOnNinthLog) },
    { BG_EVENT(BG_EVENT_CHEST), 100, FLAG_CROSSBONE_ISLE_FLOATING_LOGS_999_COINS, FIND_COINS(999) },
    { BG_EVENT(BG_EVENT_CHEST), 101, FLAG_CROSSBONE_ISLE_FLOATING_LOGS_WATER_OF_LIFE, FIND_ITEM(ITEM_WATER_OF_LIFE) },
    { BG_EVENT(BG_EVENT_CHEST), 102, FLAG_CROSSBONE_ISLE_FLOATING_LOGS_MURAMASA, FIND_ITEM(ITEM_MURAMASA) },
    { BG_EVENT(BG_EVENT_CHEST), 103, FLAG_CROSSBONE_ISLE_FLOATING_LOGS_SLEEP_BOMB, FIND_ITEM(ITEM_SLEEP_BOMB) },
    { MAP_EVENTS_END },
};
// clang-format on

struct MapEvent gCrossboneIsle_PierEvents[] = {
    {MAP_EVENT_WARP, 1, CONDITION_ALWAYS,
     CROSSBONE_ISLE_PIER_TO_CROSSBONE_ISLE_NINTH_FLOOR},
    {MAP_EVENT_WARP, 2, CONDITION_ALWAYS, CROSSBONE_ISLE_PIER_TO_PIRATE_SHIP},
    {MAP_EVENTS_END},
};

struct MapEvent gCrossboneIsle_PirateShipEvents[] = {
    {MAP_EVENT_WARP, 1, CONDITION_ALWAYS, PIRATE_SHIP_TO_CROSSBONE_ISLE_PIER},
    {MAP_EVENT_COORD, 10, FLAG_CROSSBONE_ISLE_DEADBEARD_DEFEATED,
     EVENT_SCRIPT(EventScript_FightDeadbeard)},
    {COORD_EVENT_WALKING_INTO(FACING_SOUTH), 20, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_HopSouth)},
    {BG_EVENT(BG_EVENT_CHEST), 100, FLAG_PIRATE_SHIP_DEMON_MAIL,
     FIND_ITEM(ITEM_DEMON_MAIL)},
    {BG_EVENT(BG_EVENT_CHEST), 101, FLAG_PIRATE_SHIP_MIMIC_BEATEN,
     FIND_MIMIC | 7},
    {MAP_EVENTS_END},
};

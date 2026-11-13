/// @file
/// Crossbone Isle, an island reached from the World Map, and the main line of
/// the cave beneath it, in three areas by their area names: Crossbone
/// Isle, Crossbone Isle Cave and the Pirate Ship.
///
/// No one on the island speaks. It is a run of Psynergy puzzles and battles
/// that ends with Deadbeard on the Pirate Ship. Its scenes are met in this
/// order:
///
/// - Crossbone Isle (SceneId_CrossboneIsle), the island itself, reached from
///   the World Map (WORLD_MAP_TO_CROSSBONE_ISLE; CROSSBONE_ISLE_TO_WORLD_MAP
///   leads back to it).
///
///   Whirlwind blows two ivies away for good. Their flags start at
///   FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY, and the first ivy also opens
///   a cell. The second's and a fourth's triggers are unused.
///
///   Growth grows three plants so they can be crossed, for this visit only.
///   Their flags start at FLAG_CROSSBONE_ISLE_FIRST_PLANT_GROWN, and the
///   first plant's trigger is unused.
///
///   Two logs can be pushed, and a Nut circles until Catch takes it
///   (FLAG_CROSSBONE_ISLE_NUT_CAUGHT).
///
///   CROSSBONE_ISLE_TO_CROSSBONE_ISLE_CAVE_ENTRANCE leads down into the cave.
///
/// - Crossbone Isle Cave (SceneId_CrossboneIsleCaveEntrance), the cave's mouth,
///   with only its two exits.
///
/// - Crossbone Isle Cave (SceneId_CrossboneIsleFirstFloor to
///   SceneId_CrossboneIsleNinthFloor), the nine floors the cave descends
///   through, all on one layout. Each is entered from the east
///   (CORRIDOR_FROM_PREVIOUS_ROOM) and left to the west
///   (CORRIDOR_TO_NEXT_ROOM).
///
///   Every room numbers its doors alike, and scene_exits.h names them room
///   by room (CROSSBONE_ISLE_FIRST_FLOOR_FROM_CROSSBONE_ISLE_CAVE_ENTRANCE
///   and CROSSBONE_ISLE_FIRST_FLOOR_TO_CROSSBONE_ISLE_SECOND_FLOOR in the
///   first).
///
///   Each room has two doors in its north wall into a side room of its own.
///   The first time the party walks into the east door
///   (CrossboneIsle_OpenEastDoor), the monsters behind it attack. They are
///   ENCOUNTER_ZONE_CROSSBONE_ISLE_FIRST_GUARDS for the first four rooms and
///   ENCOUNTER_ZONE_CROSSBONE_ISLE_LAST_GUARDS for the next five, one group
///   per room.
///
///   Back from the battle (ENTRANCE_AFTER_BATTLE) the door opens and the
///   party walks through. From then on it simply opens, with one flag per
///   room from FLAG_CROSSBONE_ISLE_FIRST_EAST_DOOR_MONSTERS_DEFEATED. The
///   west door always opens.
///
///   Between the doors stands a pillar. Pushed or moved with Move to column
///   40, it sinks into the floor and opens the corridor, with one flag per
///   room from FLAG_CROSSBONE_ISLE_FIRST_PASSAGE_OPENED.
///
///   In the first floor a chest is a mimic
///   (FLAG_CROSSBONE_ISLE_FIRST_FLOOR_MIMIC_BEATEN once beaten).
///
///   The side rooms of the first eight floors are in
///   crossbone_isle_stepping_stones.c and crossbone_isle_keys.c.
///
/// - Crossbone Isle Cave (SceneId_CrossboneIsleFloatingLogs), the ninth floor's
///   side room, with twelve logs floating on green water. A log drifts with
///   the leader standing on it until another log or the shore stops it.
///   Drifting, and hopping from log to log, leads to the room's four chests.
///
/// - Crossbone Isle Cave (SceneId_CrossboneIslePier), the ledge past the ninth
///   floor where a wooden pier leads out to the Pirate Ship.
///
/// - Pirate Ship (SceneId_PirateShip), at the end. Stepping on step trigger 10
///   (CrossboneIsle_FightDeadbeard) sets off the battle with Deadbeard, the
///   sixth group of ENCOUNTER_ZONE_CROSSBONE_ISLE_LAST_GUARDS.
///
///   When the party comes back (ENTRANCE_AFTER_BATTLE),
///   FLAG_CROSSBONE_ISLE_DEADBEARD_DEFEATED is set and a chest with the Demon
///   Mail appears. A second chest is a mimic (FLAG_PIRATE_SHIP_MIMIC_BEATEN
///   once beaten).
///
/// Scene_Initialize restores each scene as its flags say. That covers the
/// ivy and plants on the island, the sliding logs' footprints, the corridor's
/// sunk pillars and the chests the mimics leave.

#include "main_routine_work.h"
#include "heap_slots.h"
#include "module/import.h"
#include "module/scene/crossbone_isle.h"
#include "module/scene/shared/actor_orbit.h"
#include "module/scene/shared/field_effect_spawn.h"
#include "module/field/event_commands.h"
#include "module/system/system.h"
#include "battle.h"
#include "field.h"
#include "menu.h"
#include "system.h"
#include "event.h"
#include "pushed_actor.h"
#include "field_effect.h"
#include "debug.h"
#include "game.h"
#include "graphics.h"
#include "sound.h"
#include "event_inline.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "flags.h"
#include "shared/catch_nut.h"

/// A chest searched while facing the given way.
#define EXAMINE_CHEST_FACING(facing)                                           \
  (EXAMINE_TRIGGER(EXAMINE_CHEST) | TRIGGER_FLAG_FACING | (facing))

/// Crossbone Isle's placed actors, numbered in the order of
/// gCrossboneIsle_IsleActorPlacements.
enum CrossboneIsleActorId {
  /// The Nut that circles over the island until Catch takes it
  /// (FLAG_CROSSBONE_ISLE_NUT_CAUGHT).
  ACTOR_ISLE_NUT = ACTOR_FIRST_PLACED,
  /// The two logs a push rolls (CrossboneIsle_PushIsleLog).
  ACTOR_FIRST_ISLE_LOG,
  /// The four plants Growth grows. Once grown, a plant can be crossed for the
  /// rest of the visit (four flags from FLAG_CROSSBONE_ISLE_FIRST_PLANT_GROWN).
  /// The first is placed at the map's origin, off stage
  /// (CrossboneIsle_UnusedGrowFirstPlant).
  ACTOR_FIRST_PLANT = ACTOR_FIRST_ISLE_LOG + 2,
  /// The ivy Whirlwind blows away, one flag each from
  /// FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY.
  ///
  /// Only three rows place one, and the second is at the map's origin, off
  /// stage (CrossboneIsle_UnusedBlowAwaySecondIvy). The fourth ivy's trigger
  /// and CrossboneIsle_RestoreIvy name ACTOR_FIRST_IVY + 3, which the table
  /// leaves unplaced.
  ACTOR_FIRST_IVY = ACTOR_FIRST_PLANT + 4
};

/// Growth has grown Crossbone Isle's first plant, until the party leaves the
/// island's map. Four scene temp flags run from this one, one a plant.
enum { FLAG_CROSSBONE_ISLE_FIRST_PLANT_GROWN = FLAG_SCENE_TEMP_40 };

/// The twelve floating logs of the ninth floor's side room
/// (SceneId_CrossboneIsleFloatingLogs), from the first to the twelfth.
///
/// They are numbered in the order of
/// gCrossboneIsle_FloatingLogsActorPlacements, and each is given with its
/// shape. A log three cells wide slides north and south, and one three cells
/// tall slides east and west.
enum CrossboneIsleFloatingLogsActorId {
  ACTOR_FIRST_LOG = ACTOR_FIRST_PLACED, ///< three cells wide
  /// Three cells wide.
  ACTOR_SECOND_LOG,
  /// Three cells wide.
  ACTOR_THIRD_LOG,
  /// Three cells tall.
  ACTOR_FOURTH_LOG,
  /// Three cells tall.
  ACTOR_FIFTH_LOG,
  /// Three cells tall.
  ACTOR_SIXTH_LOG,
  /// Three cells tall.
  ACTOR_SEVENTH_LOG,
  /// Three cells tall.
  ACTOR_EIGHTH_LOG,
  /// Three cells tall.
  ACTOR_NINTH_LOG,
  /// Three cells wide.
  ACTOR_TENTH_LOG,
  /// Three cells wide.
  ACTOR_ELEVENTH_LOG,
  /// Three cells tall.
  ACTOR_TWELFTH_LOG
};

/// The corridor rooms' one placed actor
/// (gCrossboneIsle_CorridorActorPlacements).
enum CrossboneIsleCorridorActorId {
  /// The pillar between the doors that sinks into the floor at column 40.
  ACTOR_SINKING_PILLAR = ACTOR_FIRST_PLACED
};

/// The corridor rooms' entrances, as the code the rooms share names them.
///
/// Every room numbers them alike. scene_exits.h names each room's own
/// (CROSSBONE_ISLE_FIRST_FLOOR_FROM_CROSSBONE_ISLE_CAVE_ENTRANCE to
/// CROSSBONE_ISLE_FIRST_FLOOR_FROM_CROSSBONE_ISLE_STEPPING_STONES_WEST in the
/// first).
enum {
  /// At the east end, from the room before (the cave's mouth,
  /// CrossboneIsleCaveEntrance, for the first).
  CORRIDOR_FROM_PREVIOUS_ROOM = 1,
  /// At the west end, from the room after (CrossboneIslePier for the last).
  CORRIDOR_FROM_NEXT_ROOM = 2,
  CORRIDOR_FROM_SIDE_ROOM_EAST = 3, ///< out of the side room by the east door
  CORRIDOR_FROM_SIDE_ROOM_WEST = 4  ///< out of the side room by the west door
};

/// The corridor rooms' exits, numbered as their entrances.
enum {
  CORRIDOR_TO_PREVIOUS_ROOM = 1,
  CORRIDOR_TO_NEXT_ROOM = 2,
  CORRIDOR_TO_SIDE_ROOM_EAST =
      3,                         ///< the east door (CrossboneIsle_OpenEastDoor)
  CORRIDOR_TO_SIDE_ROOM_WEST = 4 ///< the west door (CrossboneIsle_OpenWestDoor)
};

/// The entrance a battle returns the party to.
///
/// In a corridor room that is after the monsters behind its east door
/// (CrossboneIsle_OpenEastDoor). On the Pirate Ship it is after Deadbeard
/// (CrossboneIsle_FightDeadbeard).
enum { ENTRANCE_AFTER_BATTLE = 5 };

static void CrossboneIsle_RestoreSunkPillar(void);
static void CrossboneIsle_OpenEastDoorAfterBattle(void);
static void CrossboneIsle_RestoreIvy(void);
static void CrossboneIsle_DrawLogPuzzleFootprints(void);

#include "shared/staged_actor.c"

#include "shared/field_effect_create.c"
#include "shared/field_effect_spawn.c"

/// Unused: an empty function nothing refers to.
UNUSED static void CrossboneIsle_UnusedNoOp(void) {}

const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 sceneId = gMainRoutineWork.next;

  if (sceneId == SCENE_CROSSBONE_ISLE) {
    return gCrossboneIsle_IsleEntrances;
  }
  if (sceneId == SCENE_CROSSBONE_ISLE_CAVE_ENTRANCE) {
    return gCrossboneIsle_CaveEntranceEntrances;
  }
  if (sceneId == SCENE_CROSSBONE_ISLE_FLOATING_LOGS) {
    return gCrossboneIsle_FloatingLogsEntrances;
  }
  if (sceneId == SCENE_CROSSBONE_ISLE_PIER) {
    return gCrossboneIsle_PierEntrances;
  }
  if (sceneId == SCENE_PIRATE_SHIP) {
    return gCrossboneIsle_PirateShipEntrances;
  }
  return &gCrossboneIsle_PirateShipEntrances[2];
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) { return gCrossboneIsleExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s16 sceneId = gMainRoutineWork.next;

  if (sceneId == SCENE_CROSSBONE_ISLE) {
    return gCrossboneIsle_IsleActorPlacements;
  }
  if (sceneId == SCENE_CROSSBONE_ISLE_FLOATING_LOGS) {
    return gCrossboneIsle_FloatingLogsActorPlacements;
  }
  if (sceneId <= SCENE_CROSSBONE_ISLE_NINTH_FLOOR &&
      sceneId >= SCENE_CROSSBONE_ISLE_FIRST_FLOOR) {
    return gCrossboneIsle_CorridorActorPlacements;
  }
  return gCrossboneIsle_NoActorPlacements;
}

#include "shared/field_effect_advance.c"

/// Emits a ring of dust around a corridor room's pillar as it sinks.
static void CrossboneIsle_SpawnPillarMovedBurst(s32 actorId) {
  struct Actor *actor;
  struct Vector3 velocity;
  struct EffectOptions options;
  u32 angleStep;
  s32 angle;
  s32 velocityX;
  s32 velocityZ;

  actor = Actor_Get(actorId);
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
    FieldEffect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed, velocityX,
                      velocity.y, velocityZ,
                      EFFECT_USE_UPDATE | EFFECT_SCRIPT_MEDIUM, &options);
  }
}

/// Stepping on step trigger 10 on the Pirate Ship: Deadbeard attacks.
static void CrossboneIsle_FightDeadbeard(void) {
  Event_Start();
  Event_SetAfterBattleDestination(gMainRoutineWork.next, ENTRANCE_AFTER_BATTLE);
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  Field_RequestBattleFromEncounterTable(
      ENCOUNTER_ZONE_CROSSBONE_ISLE_LAST_GUARDS, 5);
  Event_End();
}

const struct Trigger *Scene_GetTriggers(void) {
  s16 sceneId = gMainRoutineWork.next;

  if (sceneId == SCENE_CROSSBONE_ISLE) {
    return gCrossboneIsle_IsleTriggers;
  }
  if (sceneId == SCENE_CROSSBONE_ISLE_CAVE_ENTRANCE) {
    return gCrossboneIsle_CaveEntranceTriggers;
  }
  if (sceneId == SCENE_CROSSBONE_ISLE_FLOATING_LOGS) {
    return gCrossboneIsle_FloatingLogsTriggers;
  }
  if (sceneId == SCENE_CROSSBONE_ISLE_PIER) {
    return gCrossboneIsle_PierTriggers;
  }
  if (sceneId == SCENE_PIRATE_SHIP) {
    return gCrossboneIsle_PirateShipTriggers;
  }
  return gCrossboneIsle_CorridorTriggers;
}

/// Catch on the Nut circling on Crossbone Isle: the party gets it
/// (FLAG_CROSSBONE_ISLE_NUT_CAUGHT).
static void CrossboneIsle_CatchNut(void) {
  Scene_CatchNut(ACTOR_ISLE_NUT, FLAG_CROSSBONE_ISLE_NUT_CAUGHT);
}

/// Restores each scene's puzzles from its flags.
s32 Scene_Initialize(void) {
  struct Actor *actor;
  s32 sceneId;
  s32 current;
  s32 first;

  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  // CrossboneIsleFloatingLogs's sliding logs restore their map footprints
  // alone.
  sceneId = gMainRoutineWork.next;
  if (sceneId == SCENE_CROSSBONE_ISLE_FLOATING_LOGS) {
    CrossboneIsle_DrawLogPuzzleFootprints();
    return 0;
  }
  if (sceneId == SCENE_PIRATE_SHIP) {
    if (GameFlag_Get(FLAG_PIRATE_SHIP_MIMIC_BEATEN) == 0) {
      MapGrid_CopyCellAttributes(0, 3, 1, 1, 13, 40);
      MapGrid_CopyCellAttributes(0, 2, 1, 1, 15, 40);
      TreasureSpot_SetPosition(101, px(216), px(648));
    }
    if (gMainRoutineWork.next == sceneId) {
      if (gMainRoutineWork.entrance != ENTRANCE_AFTER_BATTLE) {
        if (GameFlag_Get(FLAG_CROSSBONE_ISLE_DEADBEARD_DEFEATED) == 0) {
          return 0;
        }
      }
      GameFlag_Set(FLAG_CROSSBONE_ISLE_DEADBEARD_DEFEATED);
      MapGrid_CopyCellAttributes(0, 1, 1, 1, 13, 30);
      TreasureSpot_SetPosition(100, px(216), px(488));
      return 0;
    }
  }
  current = gMainRoutineWork.next;
  if (current == SCENE_CROSSBONE_ISLE) {
    CrossboneIsle_RestoreIvy();
    Actor_Get(ACTOR_ISLE_NUT)->targetX = px(129);
    Scene_RedrawActorFootprint(ACTOR_FIRST_ISLE_LOG);
    Scene_RedrawActorFootprint(ACTOR_FIRST_ISLE_LOG + 1);
    if (GameFlag_Get(FLAG_CROSSBONE_ISLE_FIRST_PLANT_GROWN) != 0) {
      actor = Actor_Get(ACTOR_FIRST_PLANT);
      if (actor != NULL) {
        actor->collisionFlags = 0;
        Actor_SelectAnimation(actor, 4);
        Actor_SetSpriteFlags(actor, 0);
      }
      Field_SetCellBehaviorAt(0, 304, 368, CELL_BEHAVIOR_CLIMB);
    }
    if (GameFlag_Get(FLAG_CROSSBONE_ISLE_FIRST_PLANT_GROWN + 1) != 0) {
      actor = Actor_Get(ACTOR_FIRST_PLANT + 1);
      if (actor != NULL) {
        actor->collisionFlags = 0;
        Actor_SelectAnimation(actor, 4);
        Actor_SetSpriteFlags(actor, 0);
      }
      Field_SetCellBehaviorAt(0, 80, 368, CELL_BEHAVIOR_CLIMB);
    }
    if (GameFlag_Get(FLAG_CROSSBONE_ISLE_FIRST_PLANT_GROWN + 2) != 0) {
      actor = Actor_Get(ACTOR_FIRST_PLANT + 2);
      if (actor != NULL) {
        actor->collisionFlags = 0;
        Actor_SelectAnimation(actor, 4);
        Actor_SetSpriteFlags(actor, 0);
      }
      Field_SetCellBehaviorAt(0, 96, 336, CELL_BEHAVIOR_CLIMB);
    }
    if (GameFlag_Get(FLAG_CROSSBONE_ISLE_FIRST_PLANT_GROWN + 3) != 0) {
      actor = Actor_Get(ACTOR_FIRST_PLANT + 3);
      if (actor != NULL) {
        actor->collisionFlags = 0;
        Actor_SelectAnimation(actor, 4);
        Actor_SetSpriteFlags(actor, 0);
      }
      Field_SetCellBehaviorAt(0, 144, 320, CELL_BEHAVIOR_CLIMB);
      Field_SetCellBehaviorAt(0, 752, 320, CELL_BEHAVIOR_CLIMB);
    }
    if (GameFlag_Get(FLAG_CROSSBONE_ISLE_NUT_CAUGHT) != 0) {
      return 0;
    }
    Scene_FloatNut(ACTOR_ISLE_NUT);
    return 0;
  }
  // The nine corridor rooms (CrossboneIsleFirstFloor to
  // CrossboneIsleNinthFloor) share their door battle, sunk pillar and return
  // entrance.
  first = SCENE_CROSSBONE_ISLE_FIRST_FLOOR;
  if (current == first) {
    if (GameFlag_Get(FLAG_CROSSBONE_ISLE_FIRST_FLOOR_MIMIC_BEATEN) == 0) {
      MapGrid_CopyCellAttributes(0, 0, 1, 1, 37, 10);
      TreasureSpot_SetPosition(100, px(600), px(168));
    }
  }
  current = gMainRoutineWork.next;
  if (current >= first) {
    if (current <= SCENE_CROSSBONE_ISLE_NINTH_FLOOR) {
      CrossboneIsle_RestoreSunkPillar();
      if (gMainRoutineWork.entrance == ENTRANCE_AFTER_BATTLE) {
        CrossboneIsle_OpenEastDoorAfterBattle();
      }
    }
  }
  return 0;
}

/// In a corridor room of Crossbone Isle Cave whose pillar has been sunk, it
/// stays sunk in its place.
static void CrossboneIsle_RestoreSunkPillar(void) {
  struct Actor *actor;
  s32 cleared;

  actor = Actor_Get(ACTOR_SINKING_PILLAR);
  cleared = GameFlag_Get(
      (gMainRoutineWork.next + (FLAG_CROSSBONE_ISLE_FIRST_PASSAGE_OPENED -
                                SCENE_CROSSBONE_ISLE_FIRST_FLOOR)));
  if (cleared != 0) {
    Actor_SetPosition(ACTOR_SINKING_PILLAR, 650, 168);
    actor->y.fixed = -px(32);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_SINKING_PILLAR), 0);
    Actor_SetSpritePriority(ACTOR_SINKING_PILLAR, 3);
    actor->motionFlags = 0;
    actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    MapGrid_CopyCellAttributes(42, 10, 1, 1, 40, 10);
  } else {
    Actor_Get(ACTOR_SINKING_PILLAR)->motionFlags = cleared;
  }
}

/// Move on a corridor room's pillar in Crossbone Isle Cave, and after each
/// push: at column 40 the pillar sinks.
static void CrossboneIsle_MovePillar(void) {
  struct Actor *pillar;
  s32 column;

  pillar = Actor_Get(ACTOR_SINKING_PILLAR);
  column = FIELD_CELL(pillar->x.fixed);
  if (column != 40) {
    return;
  }

#if TBS_PATCH_1
  if (GameFlag_Get(gMainRoutineWork.next +
                   (FLAG_CROSSBONE_ISLE_FIRST_PASSAGE_OPENED -
                    SCENE_CROSSBONE_ISLE_FIRST_FLOOR)) != 0) {
    return;
  }
#endif

  pillar->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;

  Event_Wait(8);
  CrossboneIsle_SpawnPillarMovedBurst(ACTOR_SINKING_PILLAR);
  Sound_Play(SE_WHOOSH);
  Event_Wait(40);

  Actor_SetSpriteFlags(Actor_Get(ACTOR_SINKING_PILLAR), 0);
  Actor_SetSpritePriority(ACTOR_SINKING_PILLAR, 3);

  pillar->motionFlags = 0;
  pillar->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;

  MapGrid_CopyCellAttributes(42, 10, 1, 1, column, 10);

  GameFlag_Set(gMainRoutineWork.next +
               (FLAG_CROSSBONE_ISLE_FIRST_PASSAGE_OPENED -
                SCENE_CROSSBONE_ISLE_FIRST_FLOOR));
}

/// Walking into a corridor room's pillar in Crossbone Isle Cave (step trigger
/// 20): push it a step.
static void CrossboneIsle_PushPillar(void) {
  Event_Start();
  PushedActor_AdvancePair();
  Event_End();
  CrossboneIsle_MovePillar();
}

/// Walking north into a corridor room's west door in Crossbone Isle Cave
/// (step trigger 4): it opens, and the party goes through to the side room.
static void CrossboneIsle_OpenWestDoor(void) {
  MapGrid_PlayMapEdit(gCrossboneIsle_EntranceCells, 37, 7);
  Sound_Play(SE_STONE_RUMBLE);
  Import_EventSetExitRequest(CORRIDOR_TO_SIDE_ROOM_WEST);
}

/// Walking north into a corridor room's east door in Crossbone Isle Cave
/// (step trigger 3): the monsters behind it attack, once.
static void CrossboneIsle_OpenEastDoor(void) {
  if (GameFlag_Get(gMainRoutineWork.next +
                   (FLAG_CROSSBONE_ISLE_FIRST_EAST_DOOR_MONSTERS_DEFEATED -
                    SCENE_CROSSBONE_ISLE_FIRST_FLOOR)) == 0) {
    Event_Start();
    Event_SetAfterBattleDestination(gMainRoutineWork.next,
                                    ENTRANCE_AFTER_BATTLE);
    Battle_SetType(BATTLE_TYPE_SCRIPTED);
    // The first four rooms use ENCOUNTER_ZONE_CROSSBONE_ISLE_FIRST_GUARDS; the
    // next five use ENCOUNTER_ZONE_CROSSBONE_ISLE_LAST_GUARDS.
    switch (gMainRoutineWork.next - SCENE_CROSSBONE_ISLE_FIRST_FLOOR) {
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
    Event_End();
  } else {
    MapGrid_PlayMapEdit(gCrossboneIsle_EntranceCells, 44, 7);
    Sound_Play(SE_STONE_RUMBLE);
    Import_EventSetExitRequest(CORRIDOR_TO_SIDE_ROOM_EAST);
  }
}

/// Back from the battle at a corridor room's east door
/// (ENTRANCE_AFTER_BATTLE): the door opens, and the party walks through.
static void CrossboneIsle_OpenEastDoorAfterBattle(void) {
  Event_Start();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.4);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  GameFlag_Set((gMainRoutineWork.next +
                (FLAG_CROSSBONE_ISLE_FIRST_EAST_DOOR_MONSTERS_DEFEATED -
                 SCENE_CROSSBONE_ISLE_FIRST_FLOOR)));
  Event_Wait(30);
  MapGrid_AnimateCells(gCrossboneIsle_EntranceCells, 44, 7);
  Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 3, -16);
  Import_EventSetExitRequest(CORRIDOR_TO_SIDE_ROOM_EAST);
  Event_End();
}

/// Growth on one of Crossbone Isle's plants: it stops blocking the way, shows
/// grown and opens the cell at pixel (x, z) to climbing.
static inline void CrossboneIsle_GrowPlant(s32 actorId, s32 plant, s32 x,
                                           s32 z) {
  struct Actor *actor = Actor_Get(ACTOR_FIRST_PLANT + plant);

  if (actor != NULL) {
    actor->collisionFlags = 0;
  }
  Actor_SetSpriteFlags(Actor_Get(actorId), 0);
  MapGrid_SetCellBehaviorAt(0, px(x), px(z), CELL_BEHAVIOR_CLIMB);
}

/// Unused: Growth on Crossbone Isle's first plant would grow it and let the
/// party climb at (304, 368).
static void CrossboneIsle_UnusedGrowFirstPlant(s32 actorId) {
  CrossboneIsle_GrowPlant(actorId, 0, 304, 368);
  GameFlag_Set(FLAG_CROSSBONE_ISLE_FIRST_PLANT_GROWN);
}

/// Growth on Crossbone Isle's second plant.
static void CrossboneIsle_GrowSecondPlant(s32 actorId) {
  CrossboneIsle_GrowPlant(actorId, 1, 80, 368);
  GameFlag_Set(FLAG_CROSSBONE_ISLE_FIRST_PLANT_GROWN + 1);
}

/// Growth on Crossbone Isle's third plant.
static void CrossboneIsle_GrowThirdPlant(s32 actorId) {
  CrossboneIsle_GrowPlant(actorId, 2, 96, 336);
  GameFlag_Set(FLAG_CROSSBONE_ISLE_FIRST_PLANT_GROWN + 2);
}

/// Growth on Crossbone Isle's fourth plant, which opens two cells.
static void CrossboneIsle_GrowFourthPlant(s32 actorId) {
  s32 eastX = px(752);
  s32 eastZ = px(320);

  CrossboneIsle_GrowPlant(actorId, 3, 144, 320);
  MapGrid_SetCellBehaviorAt(0, eastX, eastZ, CELL_BEHAVIOR_CLIMB);
  GameFlag_Set(FLAG_CROSSBONE_ISLE_FIRST_PLANT_GROWN + 3);
}

/// Whirlwind on Crossbone Isle's first ivy: it is blown away for good, and
/// its cell opens.
static void CrossboneIsle_BlowAwayFirstIvy(void) {
  GameFlag_Set(FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY);
  MapGrid_CopyCellAttributes(0, 0, 1, 1, 8, 21);
}

/// Unused: Whirlwind on Crossbone Isle's second ivy would blow it away for
/// good.
static void CrossboneIsle_UnusedBlowAwaySecondIvy(void) {
  GameFlag_Set(FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY + 1);
}

/// Whirlwind on Crossbone Isle's third ivy.
static void CrossboneIsle_BlowAwayThirdIvy(void) {
  GameFlag_Set(FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY + 2);
}

/// Unused: Whirlwind on Crossbone Isle's fourth ivy would blow it away for
/// good.
static void CrossboneIsle_UnusedBlowAwayFourthIvy(void) {
  GameFlag_Set(FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY + 3);
}

/// On Crossbone Isle, ivy blown away stays gone; the rest stands tall.
static void CrossboneIsle_RestoreIvy(void) {
  struct Actor *actor;

  // The cells CrossboneIsle_BlowAwayFirstIvy copies as it sets the flag,
  // redrawn on arrival.
  if (GameFlag_Get(FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY) != 0) {
    MapGrid_CopyCellAttributes(0, 0, 1, 1, 8, 21);
    Actor_SetPosition(ACTOR_FIRST_IVY, 968, 968);
  } else {
    Actor_Get(ACTOR_FIRST_IVY)->scaleY = 8 * Q16_ONE / 5;
  }
  if (GameFlag_Get(FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY + 1) != 0) {
    Actor_SetPosition(ACTOR_FIRST_IVY + 1, 968, 968);
  } else {
    Actor_Get(ACTOR_FIRST_IVY + 1)->scaleY = 8 * Q16_ONE / 5;
  }
  if (GameFlag_Get(FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY + 2) != 0) {
    Actor_SetPosition(ACTOR_FIRST_IVY + 2, 968, 968);
  } else {
    Actor_Get(ACTOR_FIRST_IVY + 2)->scaleY = 8 * Q16_ONE / 5;
  }
  if (GameFlag_Get(FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY + 3) != 0) {
    Actor_SetPosition(ACTOR_FIRST_IVY + 3, 968, 968);
  } else {
    actor = Actor_Get(ACTOR_FIRST_IVY + 3);
    // BUG: no fourth ivy is placed, so its slot is empty (Actor_ClearAll)
    // and this writes to address 0x18, in the BIOS, where the write is lost.
#ifdef BUGFIX
    if (actor != NULL)
#endif
      actor->scaleY = 8 * Q16_ONE / 5;
  }
}

/// Walking into one of Crossbone Isle's two logs: push it if the cell beyond
/// is clear.
static void CrossboneIsle_PushIsleLog(void) {
  struct PushedActorProbe probe;

  Event_Start();
  if (PushedActor_FindClearPosition(&probe) != 0) {
    Actor_MoveAndRedraw(probe);
  }
  Event_End();
}

/// Moves the selected actor and puzzle log by the same pixel offset.
static void CrossboneIsle_MoveSelectedActorAndLog(s32 actorId, s32 xOffset,
                                                  s32 zOffset) {
  struct Actor *selectedActor;
  struct Actor *logActor;
  s32 fixedX;
  s32 fixedZ;

  selectedActor = Actor_Get(gMainRoutineWork.leaderActorId);
  logActor = Actor_Get(actorId);
  Event_Start();
  {
    fixedX = ((selectedActor->x.fixed + px(xOffset)) & FIELD_CELL_MASK) + px(8);
    fixedZ = ((selectedActor->z.fixed + px(zOffset)) & FIELD_CELL_MASK) + px(8);

    selectedActor->speedLimit = px(1);
    selectedActor->acceleration = px(1) / 2;
    FieldActor_SetMoveTargetFixed(selectedActor, fixedX, selectedActor->y.fixed,
                                  fixedZ);
  }
  // Position the log on the same cell-center grid as the selected actor.
  Actor_SelectAnimation(selectedActor, ANIM_LEADER_WALK_BACKWARDS);
  {
    fixedX = ((logActor->x.fixed + px(xOffset)) & FIELD_CELL_MASK) + px(8);
    fixedZ = ((logActor->z.fixed + px(zOffset)) & FIELD_CELL_MASK) + px(8);

    logActor->speedLimit = px(1);
    logActor->acceleration = px(1) / 2;
    FieldActor_SetMoveTargetFixed(logActor, fixedX, logActor->y.fixed, fixedZ);
  }
  // The log's mode depends on which side of the grid it moves toward.
  if (xOffset < 0 || zOffset < 0) {
    Actor_SelectAnimation(logActor, 4);
  } else {
    Actor_SelectAnimation(logActor, 3);
  }
  Sound_Play(SE_LOG_PUSH);
  Actor_WaitForMove(selectedActor);
  Sound_Play(SE_STOP_ACTION_SE);
  Event_End();
}

/// Draws the three-cell horizontal footprint centered on this log.
static void CrossboneIsle_DrawHorizontalLogFootprint(s32 actorId,
                                                     s32 sourceColumn,
                                                     s32 sourceRow,
                                                     s32 unused) {
  struct Actor *actor = Actor_Get(actorId);

  if (actor != NULL) {
    s32 priorityFlags;

    Actor_SetSpritePriority(actorId, 3);
    priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    priorityFlags |= actor->priorityFlags;
    actor->priorityFlags = priorityFlags;
    {
      s32 row = FIELD_CELL(actor->z.fixed);
      s32 column = FIELD_CELL(actor->x.fixed) - 1;

      MapGrid_CopyCellAttributes(sourceColumn, sourceRow, 3, 1, column, row);
    }
  }
}

/// Draws the three-cell vertical footprint centered on this log.
static void CrossboneIsle_DrawVerticalLogFootprint(s32 actorId,
                                                   s32 sourceColumn,
                                                   s32 sourceRow, s32 unused) {
  struct Actor *actor = Actor_Get(actorId);

  if (actor != NULL) {
    s32 priorityFlags;

    Actor_SetSpritePriority(actorId, 3);
    priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    priorityFlags |= actor->priorityFlags;
    actor->priorityFlags = priorityFlags;
    {
      s32 column = FIELD_CELL(actor->x.fixed);
      s32 row = FIELD_CELL(actor->z.fixed) - 1;

      MapGrid_CopyCellAttributes(sourceColumn, sourceRow, 1, 3, column, row);
    }
  }
}

/// Runs the two animation changes used when touching a puzzle log.
static void CrossboneIsle_AnimateTouchedLog(int actorId) {
  Actor_SetAnimation(actorId, 1);
  Actor_SetAnimation(actorId, 2);
}

/// Draws all twelve log footprints in Crossbone Isle Cave
/// (CrossboneIsleFloatingLogs).
static void CrossboneIsle_DrawLogPuzzleFootprints(void) {
  CrossboneIsle_DrawHorizontalLogFootprint(ACTOR_FIRST_LOG, 17, 30, 21);
  CrossboneIsle_DrawHorizontalLogFootprint(ACTOR_THIRD_LOG, 17, 31, 22);
  CrossboneIsle_DrawVerticalLogFootprint(ACTOR_FOURTH_LOG, 20, 30, 23);
  CrossboneIsle_DrawVerticalLogFootprint(ACTOR_FIFTH_LOG, 21, 30, 24);
  CrossboneIsle_DrawVerticalLogFootprint(ACTOR_SIXTH_LOG, 22, 30, 25);
  CrossboneIsle_DrawVerticalLogFootprint(ACTOR_EIGHTH_LOG, 23, 30, 26);
  CrossboneIsle_DrawHorizontalLogFootprint(ACTOR_TENTH_LOG, 0, 30, 31);
  CrossboneIsle_DrawHorizontalLogFootprint(ACTOR_ELEVENTH_LOG, 0, 31, 32);
  CrossboneIsle_DrawHorizontalLogFootprint(ACTOR_SECOND_LOG, 0, 32, 33);
  CrossboneIsle_DrawVerticalLogFootprint(ACTOR_TWELFTH_LOG, 4, 30, 34);
  CrossboneIsle_DrawVerticalLogFootprint(ACTOR_SEVENTH_LOG, 5, 30, 35);
  CrossboneIsle_DrawVerticalLogFootprint(ACTOR_NINTH_LOG, 6, 30, 36);
}

// Crossbone Isle Cave (CrossboneIsleFloatingLogs), the ninth floor's side room:
// its twelve floating logs. Stepping onto a log plays its animation from the
// start; walking onto one in a direction it moves slides it that way with the
// leader on it until another log or the wall stops it; walking against a
// log's edge hops the leader 32 pixels if the landing is clear.

/// Stepping onto the first log.
static void CrossboneIsle_StepOnFirstLog(void) {
  CrossboneIsle_AnimateTouchedLog(ACTOR_FIRST_LOG);
}

/// Stepping onto the third log.
static void CrossboneIsle_StepOnThirdLog(void) {
  CrossboneIsle_AnimateTouchedLog(ACTOR_THIRD_LOG);
}

/// Stepping onto the fourth log.
static void CrossboneIsle_StepOnFourthLog(void) {
  CrossboneIsle_AnimateTouchedLog(ACTOR_FOURTH_LOG);
}

/// Stepping onto the fifth log.
static void CrossboneIsle_StepOnFifthLog(void) {
  CrossboneIsle_AnimateTouchedLog(ACTOR_FIFTH_LOG);
}

/// Stepping onto the sixth log.
static void CrossboneIsle_StepOnSixthLog(void) {
  CrossboneIsle_AnimateTouchedLog(ACTOR_SIXTH_LOG);
}

/// Stepping onto the eighth log.
static void CrossboneIsle_StepOnEighthLog(void) {
  CrossboneIsle_AnimateTouchedLog(ACTOR_EIGHTH_LOG);
}

/// Stepping onto the tenth log.
static void CrossboneIsle_StepOnTenthLog(void) {
  CrossboneIsle_AnimateTouchedLog(ACTOR_TENTH_LOG);
}

/// Stepping onto the eleventh log.
static void CrossboneIsle_StepOnEleventhLog(void) {
  CrossboneIsle_AnimateTouchedLog(ACTOR_ELEVENTH_LOG);
}

/// Stepping onto the second log.
static void CrossboneIsle_StepOnSecondLog(void) {
  CrossboneIsle_AnimateTouchedLog(ACTOR_SECOND_LOG);
}

/// Stepping onto the twelfth log.
static void CrossboneIsle_StepOnTwelfthLog(void) {
  CrossboneIsle_AnimateTouchedLog(ACTOR_TWELFTH_LOG);
}

/// Stepping onto the seventh log.
static void CrossboneIsle_StepOnSeventhLog(void) {
  CrossboneIsle_AnimateTouchedLog(ACTOR_SEVENTH_LOG);
}

/// Stepping onto the ninth log.
static void CrossboneIsle_StepOnNinthLog(void) {
  CrossboneIsle_AnimateTouchedLog(ACTOR_NINTH_LOG);
}

/// Tests a step, then moves the leader with the log-pushing animation.
static s32 CrossboneIsle_TryHop(struct Actor *actor,
                                struct FieldPosition *requested) {
  u8 *motionFlags = &actor->motionFlags;
  s32 savedMotionFlags = *motionFlags;
#if TBS_PATCH_1
  struct FieldPosition destination;

  destination.x = (actor->x.fixed & FIELD_CELL_MASK) + px(8);
  destination.y = actor->y.fixed;
  destination.z = (actor->z.fixed & FIELD_CELL_MASK) + px(8);
  {
    s32 direction = (actor->facing + TRIG_TURN / 8) & 0xc000;

    Vector_AddPolarOffset(px(32), direction, &destination);
  }
  // Keep the original motion flags while playing the leader's push step.
  if (FieldActor_CheckMovementCollision(actor, &destination) == 0) {
#else
  if (FieldActor_CheckMovementCollision(actor, requested) == 0) {
#endif
    s32 movingFlags;

    Event_Start();
    Actor_SelectAnimation(actor, ANIM_LEADER_SPRING);
    wait(6);
    Sound_Play(SE_JUMP);
    Actor_SelectAnimation(actor, ANIM_LEADER_LEAP);
    actor->speedLimit = px(3);
    actor->acceleration = px(2);
    actor->velocityY = px(4);
    movingFlags = 126;
    movingFlags &= *motionFlags;
    *motionFlags = (u8)movingFlags;
    Actor_SetSpriteFlags(actor, 0);
#if TBS_PATCH_1
    Actor_MoveToAndWait(ACTOR_PARTY_LEADER, destination.x >> 16,
                        destination.z >> 16);
#else
    {
      s16 *pixelCoordinates = (s16 *)requested;

      Actor_MoveToAndWait(ACTOR_PARTY_LEADER, pixelCoordinates[1],
                          pixelCoordinates[5]);
    }
#endif
    Actor_SelectAnimation(actor, ANIM_LEADER_SPRING);
    Actor_SetSpriteFlags(actor, SPRITE_FLAG_SHADOW);
    *motionFlags = (u8)savedMotionFlags;
    Event_End();
    return 1;
  }
  return 0;
}

/// Walking north against a log's edge.
static s32 CrossboneIsle_HopNorth(void) {
  struct Actor *actor = Actor_Get(ACTOR_PARTY_LEADER);
  struct FieldPosition destination;

  destination.x = actor->x.fixed;
  destination.y = actor->y.fixed;
  destination.z = actor->z.fixed + -px(32);
  return CrossboneIsle_TryHop(actor, &destination);
}

/// Walking south against a log's edge, or at the Pirate Ship's step trigger 20.
static s32 CrossboneIsle_HopSouth(void) {
  struct Actor *actor = Actor_Get(ACTOR_PARTY_LEADER);
  struct FieldPosition destination;

  destination.x = actor->x.fixed;
  destination.y = actor->y.fixed;
  destination.z = actor->z.fixed + px(32);
  return CrossboneIsle_TryHop(actor, &destination);
}

/// Walking west against a log's edge.
static s32 CrossboneIsle_HopWest(void) {
  struct Actor *actor = Actor_Get(ACTOR_PARTY_LEADER);
  struct FieldPosition destination;

  destination.x = actor->x.fixed + -px(32);
  destination.y = actor->y.fixed;
  destination.z = actor->z.fixed;
  return CrossboneIsle_TryHop(actor, &destination);
}

/// Walking east against a log's edge.
static s32 CrossboneIsle_HopEast(void) {
  struct Actor *actor = Actor_Get(ACTOR_PARTY_LEADER);
  struct FieldPosition destination;

  destination.x = actor->x.fixed + px(32);
  destination.y = actor->y.fixed;
  destination.z = actor->z.fixed;
  return CrossboneIsle_TryHop(actor, &destination);
}

/// Walking north on the first log drifts it north.
static void CrossboneIsle_SlideFirstLogNorth(void) {
  s32 actorX = FIELD_CELL(Actor_Get(ACTOR_FIRST_LOG)->x.fixed);
  s32 actorZ = FIELD_CELL(Actor_Get(ACTOR_FIRST_LOG)->z.fixed);
  s32 fifthLogColumn = FIELD_CELL(Actor_Get(ACTOR_FIFTH_LOG)->x.fixed);
  s32 eighthLogColumn = FIELD_CELL(Actor_Get(ACTOR_EIGHTH_LOG)->x.fixed);

  if (actorZ == 19) {
    if (fifthLogColumn == 24) {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_FIRST_LOG, 0, -80);
    } else if (eighthLogColumn == 24) {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_FIRST_LOG, 0, -112);
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_FIRST_LOG, 0, -32);
    } else {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_FIRST_LOG, 0, -80);
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_FIRST_LOG, 0, -112);
    }
  } else if (actorZ == 14) {
    if (fifthLogColumn == 24) {
      return;
    }
    if (eighthLogColumn == 24) {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_FIRST_LOG, 0, -64);
    } else {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_FIRST_LOG, 0, -112);
    }
  } else if (actorZ == 10) {
    if (eighthLogColumn == 24) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndLog(ACTOR_FIRST_LOG, 0, -48);
  } else {
    CrossboneIsle_HopNorth();
    return;
  }
  wait(2);
  {
    s32 routeEndZ = FIELD_CELL(Actor_Get(ACTOR_FIRST_LOG)->z.fixed);
    s32 routeX = actorX - 1;

    MapGrid_CopyCellAttributes(routeX, actorZ, 3, 1, routeX, routeEndZ);
  }
  MapGrid_CopyCellAttributes(0, 0, 3, 1, actorX - 1, actorZ);
}

/// Copies a horizontal log's old footprint to its new row, then clears the
/// old row.
static inline void CrossboneIsle_RedrawHorizontalLogRoute(s32 actorId,
                                                          s32 column, s32 row) {
  wait(2);
  {
    s32 destinationRow = FIELD_CELL(Actor_Get(actorId)->z.fixed);
    s32 footprintColumn = column - 1;

    MapGrid_CopyCellAttributes(footprintColumn, row, 3, 1, footprintColumn,
                               destinationRow);
  }
  MapGrid_CopyCellAttributes(0, 0, 3, 1, column - 1, row);
}

/// Copies a vertical log's old footprint to its new column, then clears the
/// old one.
static inline void CrossboneIsle_RedrawVerticalLogRoute(s32 actorId, s32 column,
                                                        s32 row) {
  wait(2);
  {
    s32 destinationColumn = FIELD_CELL(Actor_Get(actorId)->x.fixed);
    s32 footprintRow = row - 1;

    MapGrid_CopyCellAttributes(column, footprintRow, 1, 3, destinationColumn,
                               footprintRow);
  }
  MapGrid_CopyCellAttributes(0, 0, 1, 3, column, row - 1);
}

/// Walking south on the first log drifts it south.
static void CrossboneIsle_SlideFirstLogSouth(void) {
  s32 column = FIELD_CELL(Actor_Get(ACTOR_FIRST_LOG)->x.fixed);
  s32 row = FIELD_CELL(Actor_Get(ACTOR_FIRST_LOG)->z.fixed);
  s32 fifthLogColumn = FIELD_CELL(Actor_Get(ACTOR_FIFTH_LOG)->x.fixed);

  if (row == 7) {
    if (fifthLogColumn == 24) {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_FIRST_LOG, 0, 48);
    } else {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_FIRST_LOG, 0, 80);
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_FIRST_LOG, 0, 112);
    }
  } else if (row == 10) {
    if (fifthLogColumn == 24) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndLog(ACTOR_FIRST_LOG, 0, 144);
  } else if (row == 14) {
    CrossboneIsle_MoveSelectedActorAndLog(ACTOR_FIRST_LOG, 0, 80);
  } else {
    return;
  }
  CrossboneIsle_RedrawHorizontalLogRoute(ACTOR_FIRST_LOG, column, row);
}

/// Walking north on the third log drifts it north.
static void CrossboneIsle_SlideThirdLogNorth(void) {
  s32 column = FIELD_CELL(Actor_Get(ACTOR_THIRD_LOG)->x.fixed);
  s32 row = FIELD_CELL(Actor_Get(ACTOR_THIRD_LOG)->z.fixed);
  s32 sixthLogColumn = FIELD_CELL(Actor_Get(ACTOR_SIXTH_LOG)->x.fixed);
  s32 eighthLogColumn = FIELD_CELL(Actor_Get(ACTOR_EIGHTH_LOG)->x.fixed);

  if (row == 18) {
    if (eighthLogColumn >= 31 && eighthLogColumn <= 33) {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_THIRD_LOG, 0, -128);
    } else if (sixthLogColumn >= 31 && sixthLogColumn <= 33) {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_THIRD_LOG, 0, -128);
    } else {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_THIRD_LOG, 0, -112);
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_THIRD_LOG, 0, -64);
    }
  } else if (row == 10) {
    if (eighthLogColumn >= 31 && eighthLogColumn <= 33) {
      return;
    }
    if (sixthLogColumn >= 31 && sixthLogColumn <= 33) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndLog(ACTOR_THIRD_LOG, 0, -48);
  } else if (row == 7) {
    return;
  }
  CrossboneIsle_RedrawHorizontalLogRoute(ACTOR_THIRD_LOG, column, row);
}

/// Walking south on the third log drifts it south.
static void CrossboneIsle_SlideThirdLogSouth(void) {
  s32 column = FIELD_CELL(Actor_Get(ACTOR_THIRD_LOG)->x.fixed);
  s32 row = FIELD_CELL(Actor_Get(ACTOR_THIRD_LOG)->z.fixed);

  if (row != 18) {
    if (row == 10) {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_THIRD_LOG, 0, 128);
    } else {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_THIRD_LOG, 0, 112);
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_THIRD_LOG, 0, 64);
    }
    CrossboneIsle_RedrawHorizontalLogRoute(ACTOR_THIRD_LOG, column, row);
  }
}

/// Walking west on the fourth log drifts it west.
static void CrossboneIsle_SlideFourthLogWest(void) {
  s32 column = FIELD_CELL(Actor_Get(ACTOR_FOURTH_LOG)->x.fixed);
  s32 row = FIELD_CELL(Actor_Get(ACTOR_FOURTH_LOG)->z.fixed);

  if (column != 30) {
    if (column == 34) {
      if (FIELD_CELL(Actor_Get(ACTOR_THIRD_LOG)->z.fixed) == 18) {
        return;
      }
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_FOURTH_LOG, -64, 0);
    } else if (column == 36) {
      if (FIELD_CELL(Actor_Get(ACTOR_THIRD_LOG)->z.fixed) == 18) {
        CrossboneIsle_MoveSelectedActorAndLog(ACTOR_FOURTH_LOG, -32, 0);
      } else {
        CrossboneIsle_MoveSelectedActorAndLog(ACTOR_FOURTH_LOG, -96, 0);
      }
    }
    CrossboneIsle_RedrawVerticalLogRoute(ACTOR_FOURTH_LOG, column, row);
  }
}

/// Walking east on the fourth log drifts it east.
static void CrossboneIsle_SlideFourthLogEast(void) {
  s32 column = FIELD_CELL(Actor_Get(ACTOR_FOURTH_LOG)->x.fixed);
  s32 row = FIELD_CELL(Actor_Get(ACTOR_FOURTH_LOG)->z.fixed);

  if (column != 36) {
    if (column == 30) {
      if (FIELD_CELL(Actor_Get(ACTOR_THIRD_LOG)->z.fixed) == 18) {
        return;
      }
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_FOURTH_LOG, 96, 0);
    } else if (column == 34) {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_FOURTH_LOG, 32, 0);
    }
    CrossboneIsle_RedrawVerticalLogRoute(ACTOR_FOURTH_LOG, column, row);
  }
}

/// Walking west on the fifth log drifts it west.
static void CrossboneIsle_SlideFifthLogWest(void) {
  s32 column = FIELD_CELL(Actor_Get(ACTOR_FIFTH_LOG)->x.fixed);
  s32 row = FIELD_CELL(Actor_Get(ACTOR_FIFTH_LOG)->z.fixed);

  if (column == 36) {
    CrossboneIsle_MoveSelectedActorAndLog(ACTOR_FIFTH_LOG, -96, 0);
    CrossboneIsle_MoveSelectedActorAndLog(ACTOR_FIFTH_LOG, -96, 0);
  } else if (column == 34) {
    CrossboneIsle_MoveSelectedActorAndLog(ACTOR_FIFTH_LOG, -96, 0);
    CrossboneIsle_MoveSelectedActorAndLog(ACTOR_FIFTH_LOG, -64, 0);
  } else if (column == 24) {
    return;
  }
  CrossboneIsle_RedrawVerticalLogRoute(ACTOR_FIFTH_LOG, column, row);
}

/// Walking east on the fifth log drifts it east.
static void CrossboneIsle_SlideFifthLogEast(void) {
  s32 column = FIELD_CELL(Actor_Get(ACTOR_FIFTH_LOG)->x.fixed);
  s32 row = FIELD_CELL(Actor_Get(ACTOR_FIFTH_LOG)->z.fixed);

  if (column == 24) {
    CrossboneIsle_MoveSelectedActorAndLog(ACTOR_FIFTH_LOG, 96, 0);
    CrossboneIsle_MoveSelectedActorAndLog(ACTOR_FIFTH_LOG, 96, 0);
  } else if (column == 34) {
    CrossboneIsle_MoveSelectedActorAndLog(ACTOR_FIFTH_LOG, 32, 0);
  } else if (column == 36) {
    return;
  }
  CrossboneIsle_RedrawVerticalLogRoute(ACTOR_FIFTH_LOG, column, row);
}

/// Walking west on the sixth log drifts it west.
static void CrossboneIsle_SlideSixthLogWest(void) {
  s32 column = FIELD_CELL(Actor_Get(ACTOR_SIXTH_LOG)->x.fixed);
  s32 row = FIELD_CELL(Actor_Get(ACTOR_SIXTH_LOG)->z.fixed);
  s32 thirdLogRow = FIELD_CELL(Actor_Get(ACTOR_THIRD_LOG)->z.fixed);
  s32 eighthLogColumn = FIELD_CELL(Actor_Get(ACTOR_EIGHTH_LOG)->x.fixed);

  if (column == 36) {
    if (eighthLogColumn == 34) {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_SIXTH_LOG, -16, 0);
    } else if (thirdLogRow == 7) {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_SIXTH_LOG, -32, 0);
    } else if (eighthLogColumn == 30) {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_SIXTH_LOG, -80, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_SIXTH_LOG, -96, 0);
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_SIXTH_LOG, -80, 0);
    }
  } else if (column == 35) {
    if (eighthLogColumn == 34) {
      return;
    }
    if (thirdLogRow == 7) {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_SIXTH_LOG, -16, 0);
    } else if (eighthLogColumn == 30) {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_SIXTH_LOG, -64, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_SIXTH_LOG, -80, 0);
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_SIXTH_LOG, -80, 0);
    }
  } else if (column == 34) {
    if (thirdLogRow == 7) {
      return;
    }
    if (eighthLogColumn == 30) {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_SIXTH_LOG, -48, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_SIXTH_LOG, -144, 0);
    }
  } else if (column == 31) {
    if (eighthLogColumn == 30) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndLog(ACTOR_SIXTH_LOG, -96, 0);
  } else if (column == 25) {
    return;
  }
  CrossboneIsle_RedrawVerticalLogRoute(ACTOR_SIXTH_LOG, column, row);
}

/// Walking east on the sixth log drifts it east.
static void CrossboneIsle_SlideSixthLogEast(void) {
  s32 column = FIELD_CELL(Actor_Get(ACTOR_SIXTH_LOG)->x.fixed);
  s32 row = FIELD_CELL(Actor_Get(ACTOR_SIXTH_LOG)->z.fixed);

  Actor_Get(ACTOR_EIGHTH_LOG);
  if (column == 25) {
    CrossboneIsle_MoveSelectedActorAndLog(ACTOR_SIXTH_LOG, 96, 0);
    CrossboneIsle_MoveSelectedActorAndLog(ACTOR_SIXTH_LOG, 80, 0);
  } else if (column == 31) {
    CrossboneIsle_MoveSelectedActorAndLog(ACTOR_SIXTH_LOG, 80, 0);
  } else if (column == 34) {
    CrossboneIsle_MoveSelectedActorAndLog(ACTOR_SIXTH_LOG, 32, 0);
  } else if (column == 35) {
    CrossboneIsle_MoveSelectedActorAndLog(ACTOR_SIXTH_LOG, 16, 0);
  } else if (column == 36) {
    return;
  }
  CrossboneIsle_RedrawVerticalLogRoute(ACTOR_SIXTH_LOG, column, row);
}

/// Walking west on the eighth log drifts it west.
static void CrossboneIsle_SlideEighthLogWest(void) {
  s32 column = FIELD_CELL(Actor_Get(ACTOR_EIGHTH_LOG)->x.fixed);
  s32 row = FIELD_CELL(Actor_Get(ACTOR_EIGHTH_LOG)->z.fixed);
  s32 firstLogRow = FIELD_CELL(Actor_Get(ACTOR_FIRST_LOG)->z.fixed);
  s32 thirdLogRow = FIELD_CELL(Actor_Get(ACTOR_THIRD_LOG)->z.fixed);

  if (column == 35) {
    if (thirdLogRow == 7) {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_EIGHTH_LOG, -16, 0);
    } else if (firstLogRow == 7) {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_EIGHTH_LOG, -112, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_EIGHTH_LOG, -96, 0);
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_EIGHTH_LOG, -80, 0);
    }
  } else if (column == 34) {
    if (thirdLogRow == 7) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndLog(ACTOR_EIGHTH_LOG, -96, 0);
    CrossboneIsle_MoveSelectedActorAndLog(ACTOR_EIGHTH_LOG, -64, 0);
  } else if (column == 33) {
    CrossboneIsle_MoveSelectedActorAndLog(ACTOR_EIGHTH_LOG, -144, 0);
  } else if (column == 31) {
    CrossboneIsle_MoveSelectedActorAndLog(ACTOR_EIGHTH_LOG, -80, 0);
  } else if (column == 30) {
    CrossboneIsle_MoveSelectedActorAndLog(ACTOR_EIGHTH_LOG, -96, 0);
  } else if (column == 24) {
    return;
  }
  CrossboneIsle_RedrawVerticalLogRoute(ACTOR_EIGHTH_LOG, column, row);
}

/// Walking east on the eighth log drifts it east.
static void CrossboneIsle_SlideEighthLogEast(void) {
  s32 column = FIELD_CELL(Actor_Get(ACTOR_EIGHTH_LOG)->x.fixed);
  s32 row = FIELD_CELL(Actor_Get(ACTOR_EIGHTH_LOG)->z.fixed);
  s32 thirdLogRow = FIELD_CELL(Actor_Get(ACTOR_THIRD_LOG)->z.fixed);
  s32 sixthLogColumn = FIELD_CELL(Actor_Get(ACTOR_SIXTH_LOG)->x.fixed);

  if (column == 24) {
    if (thirdLogRow == 7 || sixthLogColumn == 31) {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_EIGHTH_LOG, 96, 0);
    } else if (sixthLogColumn == 34) {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_EIGHTH_LOG, 64, 0);
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_EIGHTH_LOG, 80, 0);
    } else if (sixthLogColumn == 35) {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_EIGHTH_LOG, 80, 0);
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_EIGHTH_LOG, 80, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_EIGHTH_LOG, 80, 0);
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_EIGHTH_LOG, 96, 0);
    }
  } else if (column == 30 || sixthLogColumn == 31) {
    if (thirdLogRow == 7) {
      return;
    }
    if (sixthLogColumn == 34) {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_EIGHTH_LOG, 48, 0);
    } else if (sixthLogColumn == 35) {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_EIGHTH_LOG, 64, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_EIGHTH_LOG, 80, 0);
    }
  } else if (column == 33) {
    if (sixthLogColumn == 34) {
      return;
    }
    if (sixthLogColumn == 35) {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_EIGHTH_LOG, 16, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_EIGHTH_LOG, 32, 0);
    }
  } else if (column == 34) {
    CrossboneIsle_MoveSelectedActorAndLog(ACTOR_EIGHTH_LOG, 16, 0);
  } else if (column == 35) {
    return;
  }
  CrossboneIsle_RedrawVerticalLogRoute(ACTOR_EIGHTH_LOG, column, row);
}

/// Walking north on the tenth log drifts it north.
static void CrossboneIsle_SlideTenthLogNorth(void) {
  s32 column = FIELD_CELL(Actor_Get(ACTOR_TENTH_LOG)->x.fixed);
  s32 row = FIELD_CELL(Actor_Get(ACTOR_TENTH_LOG)->z.fixed);
  s32 twelfthLogColumn = FIELD_CELL(Actor_Get(ACTOR_TWELFTH_LOG)->x.fixed);

  if (row == 19) {
    if (twelfthLogColumn >= 3 && twelfthLogColumn <= 5) {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_TENTH_LOG, 0, -16);
    } else {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_TENTH_LOG, 0, -64);
    }
  } else if (row == 18) {
    if (twelfthLogColumn >= 3 && twelfthLogColumn <= 5) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndLog(ACTOR_TENTH_LOG, 0, -48);
  } else if (row == 15) {
    return;
  }
  CrossboneIsle_RedrawHorizontalLogRoute(ACTOR_TENTH_LOG, column, row);
}

/// Walking south on the tenth log drifts it south.
static void CrossboneIsle_SlideTenthLogSouth(void) {
  s32 column = FIELD_CELL(Actor_Get(ACTOR_TENTH_LOG)->x.fixed);
  s32 row = FIELD_CELL(Actor_Get(ACTOR_TENTH_LOG)->z.fixed);

  if (row == 15) {
    CrossboneIsle_MoveSelectedActorAndLog(ACTOR_TENTH_LOG, 0, 64);
  } else if (row == 18) {
    CrossboneIsle_MoveSelectedActorAndLog(ACTOR_TENTH_LOG, 0, 16);
  } else if (row == 19) {
    return;
  }
  CrossboneIsle_RedrawHorizontalLogRoute(ACTOR_TENTH_LOG, column, row);
}

/// These three columns form the crossing lane for the vertical logs.
#define IS_IN_LOG_CROSSING_COLUMNS(x) ((x) >= 6 && (x) <= 8)

/// Walking north on the eleventh log drifts it north.
static void CrossboneIsle_SlideEleventhLogNorth(void) {
  s32 column;
  s32 row;
  s32 twelfthLogColumn;
  s32 seventhLogColumn;
  s32 ninthLogColumn;

  column = FIELD_CELL(Actor_Get(ACTOR_ELEVENTH_LOG)->x.fixed);
  row = FIELD_CELL(Actor_Get(ACTOR_ELEVENTH_LOG)->z.fixed);
  twelfthLogColumn = FIELD_CELL(Actor_Get(ACTOR_TWELFTH_LOG)->x.fixed);
  seventhLogColumn = FIELD_CELL(Actor_Get(ACTOR_SEVENTH_LOG)->x.fixed);
  ninthLogColumn = FIELD_CELL(Actor_Get(ACTOR_NINTH_LOG)->x.fixed);
  if (row == 19) {
    if (IS_IN_LOG_CROSSING_COLUMNS(twelfthLogColumn)) {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_ELEVENTH_LOG, 0, -16);
    } else if (IS_IN_LOG_CROSSING_COLUMNS(seventhLogColumn)) {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_ELEVENTH_LOG, 0, -64);
    } else if (IS_IN_LOG_CROSSING_COLUMNS(ninthLogColumn)) {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_ELEVENTH_LOG, 0, -112);
    } else {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_ELEVENTH_LOG, 0, -64);
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_ELEVENTH_LOG, 0, -96);
    }
  } else if (row == 18) {
    if (IS_IN_LOG_CROSSING_COLUMNS(twelfthLogColumn)) {
      return;
    }
    if (IS_IN_LOG_CROSSING_COLUMNS(seventhLogColumn)) {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_ELEVENTH_LOG, 0, -48);
    } else if (IS_IN_LOG_CROSSING_COLUMNS(ninthLogColumn)) {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_ELEVENTH_LOG, 0, -96);
    } else {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_ELEVENTH_LOG, 0, -144);
    }
  } else if (row == 15) {
    if (IS_IN_LOG_CROSSING_COLUMNS(seventhLogColumn)) {
      return;
    }
    if (IS_IN_LOG_CROSSING_COLUMNS(ninthLogColumn)) {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_ELEVENTH_LOG, 0, -48);
    } else {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_ELEVENTH_LOG, 0, -96);
    }
  } else if (row == 14) {
    if (IS_IN_LOG_CROSSING_COLUMNS(seventhLogColumn)) {
      return;
    }
    if (IS_IN_LOG_CROSSING_COLUMNS(ninthLogColumn)) {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_ELEVENTH_LOG, 0, -32);
    } else {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_ELEVENTH_LOG, 0, -80);
    }
  } else if (row == 12) {
    if (IS_IN_LOG_CROSSING_COLUMNS(ninthLogColumn)) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndLog(ACTOR_ELEVENTH_LOG, 0, -48);
  } else if (row == 11) {
    if (IS_IN_LOG_CROSSING_COLUMNS(ninthLogColumn)) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndLog(ACTOR_ELEVENTH_LOG, 0, -32);
  } else if (row == 9) {
    return;
  }
  wait(2);
  MapGrid_CopyCellAttributeRect(
      column - 1, row, 3, 1, column - 1,
      FIELD_CELL(Actor_Get(ACTOR_ELEVENTH_LOG)->z.fixed));
  MapGrid_CopyCellAttributes(0, 0, 3, 1, column - 1, row);
}

#undef IS_IN_LOG_CROSSING_COLUMNS

/// Walking south on the eleventh log drifts it south.
static void CrossboneIsle_SlideEleventhLogSouth(void) {
  s32 column;
  s32 row;
  s32 twelfthLogColumn;
  s32 seventhLogColumn;

  column = FIELD_CELL(Actor_Get(ACTOR_ELEVENTH_LOG)->x.fixed);
  row = FIELD_CELL(Actor_Get(ACTOR_ELEVENTH_LOG)->z.fixed);
  twelfthLogColumn = FIELD_CELL(Actor_Get(ACTOR_TWELFTH_LOG)->x.fixed);
  seventhLogColumn = FIELD_CELL(Actor_Get(ACTOR_SEVENTH_LOG)->x.fixed);

  if (row == 9) {
    if (seventhLogColumn >= 6 && seventhLogColumn <= 8) {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_ELEVENTH_LOG, 0, 32);
    } else if (twelfthLogColumn >= 6 && twelfthLogColumn <= 8) {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_ELEVENTH_LOG, 0, 80);
    } else {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_ELEVENTH_LOG, 0, 64);
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_ELEVENTH_LOG, 0, 96);
    }
  } else if (row == 11) {
    if (seventhLogColumn >= 6 && seventhLogColumn <= 8) {
      return;
    }
    if (twelfthLogColumn >= 6 && twelfthLogColumn <= 8) {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_ELEVENTH_LOG, 0, 48);
    } else {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_ELEVENTH_LOG, 0, 128);
    }
  } else if (row == 12) {
    if (twelfthLogColumn >= 6 && twelfthLogColumn <= 8) {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_ELEVENTH_LOG, 0, 32);
    } else {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_ELEVENTH_LOG, 0, 112);
    }
  } else if (row == 14) {
    if (twelfthLogColumn >= 6 && twelfthLogColumn <= 8) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndLog(ACTOR_ELEVENTH_LOG, 0, 80);
  } else if (row == 15) {
    CrossboneIsle_MoveSelectedActorAndLog(ACTOR_ELEVENTH_LOG, 0, 64);
  } else if (row == 18) {
    CrossboneIsle_MoveSelectedActorAndLog(ACTOR_ELEVENTH_LOG, 0, 16);
  } else if (row == 19) {
    return;
  }

  wait(2);

  MapGrid_CopyCellAttributeRect(
      column - 1, row, 3, 1, column - 1,
      FIELD_CELL(Actor_Get(ACTOR_ELEVENTH_LOG)->z.fixed));
  MapGrid_CopyCellAttributes(0, 0, 3, 1, column - 1, row);
}

#undef IS_IN_LOG_CROSSING_COLUMNS
#define IS_IN_LOG_CROSSING_COLUMNS(x) ((x) >= 9 && (x) <= 11)

/// Walking north on the second log drifts it north.
static void CrossboneIsle_SlideSecondLogNorth(void) {
  s32 column;
  s32 row;
  s32 twelfthLogColumn;
  s32 seventhLogColumn;
  s32 ninthLogColumn;

  column = FIELD_CELL(Actor_Get(ACTOR_SECOND_LOG)->x.fixed);
  row = FIELD_CELL(Actor_Get(ACTOR_SECOND_LOG)->z.fixed);
  twelfthLogColumn = FIELD_CELL(Actor_Get(ACTOR_TWELFTH_LOG)->x.fixed);
  seventhLogColumn = FIELD_CELL(Actor_Get(ACTOR_SEVENTH_LOG)->x.fixed);
  ninthLogColumn = FIELD_CELL(Actor_Get(ACTOR_NINTH_LOG)->x.fixed);
  if (row == 19) {
    if (IS_IN_LOG_CROSSING_COLUMNS(twelfthLogColumn)) {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_SECOND_LOG, 0, -16);
    } else if (IS_IN_LOG_CROSSING_COLUMNS(seventhLogColumn)) {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_SECOND_LOG, 0, -64);
    } else if (IS_IN_LOG_CROSSING_COLUMNS(ninthLogColumn)) {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_SECOND_LOG, 0, -112);
    } else {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_SECOND_LOG, 0, -80);
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_SECOND_LOG, 0, -96);
    }
  } else if (row == 18) {
    if (IS_IN_LOG_CROSSING_COLUMNS(twelfthLogColumn)) {
      return;
    }
    if (IS_IN_LOG_CROSSING_COLUMNS(seventhLogColumn)) {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_SECOND_LOG, 0, -48);
    } else if (IS_IN_LOG_CROSSING_COLUMNS(ninthLogColumn)) {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_SECOND_LOG, 0, -96);
    } else {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_SECOND_LOG, 0, -96);
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_SECOND_LOG, 0, -64);
    }
  } else if (row == 15) {
    if (IS_IN_LOG_CROSSING_COLUMNS(seventhLogColumn)) {
      return;
    }
    if (IS_IN_LOG_CROSSING_COLUMNS(ninthLogColumn)) {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_SECOND_LOG, 0, -48);
    } else {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_SECOND_LOG, 0, -112);
    }
  } else if (row == 14) {
    if (IS_IN_LOG_CROSSING_COLUMNS(seventhLogColumn)) {
      return;
    }
    if (IS_IN_LOG_CROSSING_COLUMNS(ninthLogColumn)) {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_SECOND_LOG, 0, -32);
    } else {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_SECOND_LOG, 0, -96);
    }
  } else if (row == 12) {
    if (IS_IN_LOG_CROSSING_COLUMNS(ninthLogColumn)) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndLog(ACTOR_SECOND_LOG, 0, -64);
  } else if (row == 11) {
    if (IS_IN_LOG_CROSSING_COLUMNS(ninthLogColumn)) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndLog(ACTOR_SECOND_LOG, 0, -48);
  } else if ((u32)row <= 9) {
    return;
  }
  wait(2);
  MapGrid_CopyCellAttributeRect(
      column - 1, row, 3, 1, column - 1,
      FIELD_CELL(Actor_Get(ACTOR_SECOND_LOG)->z.fixed));
  MapGrid_CopyCellAttributes(0, 0, 3, 1, column - 1, row);
}

#undef IS_IN_LOG_CROSSING_COLUMNS

/// Walking south on the second log drifts it south.
static void CrossboneIsle_SlideSecondLogSouth(void) {
  s32 column;
  s32 row;
  s32 twelfthLogColumn;
  s32 seventhLogColumn;
  s32 ninthLogColumn;

  column = FIELD_CELL(Actor_Get(ACTOR_SECOND_LOG)->x.fixed);
  row = FIELD_CELL(Actor_Get(ACTOR_SECOND_LOG)->z.fixed);
  twelfthLogColumn = FIELD_CELL(Actor_Get(ACTOR_TWELFTH_LOG)->x.fixed);
  seventhLogColumn = FIELD_CELL(Actor_Get(ACTOR_SEVENTH_LOG)->x.fixed);
  ninthLogColumn = FIELD_CELL(Actor_Get(ACTOR_NINTH_LOG)->x.fixed);

  if (row == 8) {
    if (ninthLogColumn >= 9 && ninthLogColumn <= 11) {
      return;
    }
    if (seventhLogColumn >= 9 && seventhLogColumn <= 11) {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_SECOND_LOG, 0, 48);
    } else {
      if ((u32)(twelfthLogColumn - 9) > 2) {
        CrossboneIsle_MoveSelectedActorAndLog(ACTOR_SECOND_LOG, 0, 80);
      }
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_SECOND_LOG, 0, 96);
    }
  } else if (row == 11) {
    if (seventhLogColumn >= 9 && seventhLogColumn <= 11) {
      return;
    }
    if (twelfthLogColumn >= 9 && twelfthLogColumn <= 11) {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_SECOND_LOG, 0, 48);
    } else {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_SECOND_LOG, 0, 128);
    }
  } else if (row == 12) {
    if (seventhLogColumn >= 9 && seventhLogColumn <= 11) {
      return;
    }
    if (twelfthLogColumn >= 9 && twelfthLogColumn <= 11) {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_SECOND_LOG, 0, 32);
    } else {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_SECOND_LOG, 0, 112);
    }
  } else if (row == 14) {
    if (twelfthLogColumn >= 9 && twelfthLogColumn <= 11) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndLog(ACTOR_SECOND_LOG, 0, 80);
  } else if (row == 15) {
    CrossboneIsle_MoveSelectedActorAndLog(ACTOR_SECOND_LOG, 0, 64);
  } else if (row == 18) {
    CrossboneIsle_MoveSelectedActorAndLog(ACTOR_SECOND_LOG, 0, 16);
  }

  wait(2);

  MapGrid_CopyCellAttributeRect(
      column - 1, row, 3, 1, column - 1,
      FIELD_CELL(Actor_Get(ACTOR_SECOND_LOG)->z.fixed));
  MapGrid_CopyCellAttributes(0, 0, 3, 1, column - 1, row);
}

/// Walking west on the twelfth log drifts it west.
static void CrossboneIsle_SlideTwelfthLogWest(void) {
  s32 column;
  s32 row;
  s32 tenthLogRow;
  s32 eleventhLogRow;
  s32 secondLogRow;

  column = FIELD_CELL(Actor_Get(ACTOR_TWELFTH_LOG)->x.fixed);
  row = FIELD_CELL(Actor_Get(ACTOR_TWELFTH_LOG)->z.fixed);
  tenthLogRow = FIELD_CELL(Actor_Get(ACTOR_TENTH_LOG)->z.fixed);
  eleventhLogRow = FIELD_CELL(Actor_Get(ACTOR_ELEVENTH_LOG)->z.fixed);
  secondLogRow = FIELD_CELL(Actor_Get(ACTOR_SECOND_LOG)->z.fixed);
  if (column == 3) {
    return;
  }
  if (column == 13) {
    if (secondLogRow == 15) {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_TWELFTH_LOG, -16, 0);
    } else if (eleventhLogRow == 15) {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_TWELFTH_LOG, -64, 0);
    } else if (tenthLogRow == 15) {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_TWELFTH_LOG, -112, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_TWELFTH_LOG, -112, 0);
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_TWELFTH_LOG, -48, 0);
    }
  } else if (column == 6) {
    if (tenthLogRow == 15) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndLog(ACTOR_TWELFTH_LOG, -48, 0);
  } else if (column == 5) {
    CrossboneIsle_MoveSelectedActorAndLog(ACTOR_TWELFTH_LOG, -32, 0);
  } else if (column == 8) {
    if (eleventhLogRow == 15) {
      return;
    }
    if (tenthLogRow == 15) {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_TWELFTH_LOG, -32, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_TWELFTH_LOG, -80, 0);
    }
  } else if (column == 9) {
    if (eleventhLogRow == 15) {
      return;
    }
    if (tenthLogRow == 15) {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_TWELFTH_LOG, -48, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_TWELFTH_LOG, -96, 0);
    }
  } else if (column == 12) {
    if (secondLogRow == 15) {
      return;
    }
    if (eleventhLogRow == 15) {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_TWELFTH_LOG, -48, 0);
    } else if (tenthLogRow == 15) {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_TWELFTH_LOG, -96, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_TWELFTH_LOG, -144, 0);
    }
  }
  wait(2);
  MapGrid_CopyCellAttributeRect(
      column, row - 1, 1, 3, FIELD_CELL(Actor_Get(ACTOR_TWELFTH_LOG)->x.fixed),
      row - 1);
  MapGrid_CopyCellAttributes(0, 0, 1, 3, column, row - 1);
}

/// Walking east on the twelfth log drifts it east.
static void CrossboneIsle_SlideTwelfthLogEast(void) {
  s32 column;
  s32 row;
  s32 eleventhLogRow;
  s32 secondLogRow;

  column = FIELD_CELL(Actor_Get(ACTOR_TWELFTH_LOG)->x.fixed);
  row = FIELD_CELL(Actor_Get(ACTOR_TWELFTH_LOG)->z.fixed);
  eleventhLogRow = FIELD_CELL(Actor_Get(ACTOR_ELEVENTH_LOG)->z.fixed);
  secondLogRow = FIELD_CELL(Actor_Get(ACTOR_SECOND_LOG)->z.fixed);
  if (column == 3) {
    if (eleventhLogRow == 15) {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_TWELFTH_LOG, 32, 0);
    } else if (secondLogRow == 15) {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_TWELFTH_LOG, 80, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_TWELFTH_LOG, 112, 0);
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_TWELFTH_LOG, 48, 0);
    }
  } else if (column == 5) {
    if (eleventhLogRow == 15) {
      return;
    }
    if (secondLogRow == 15) {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_TWELFTH_LOG, 48, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_TWELFTH_LOG, 128, 0);
    }
  } else if (column == 6) {
    if (secondLogRow == 15) {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_TWELFTH_LOG, 32, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_TWELFTH_LOG, 112, 0);
    }
  } else if (column == 8) {
    if (secondLogRow == 15) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndLog(ACTOR_TWELFTH_LOG, 80, 0);
  } else if (column == 9) {
    CrossboneIsle_MoveSelectedActorAndLog(ACTOR_TWELFTH_LOG, 64, 0);
  } else if (column == 12) {
    CrossboneIsle_MoveSelectedActorAndLog(ACTOR_TWELFTH_LOG, 16, 0);
  }
  wait(2);
  MapGrid_CopyCellAttributeRect(
      column, row - 1, 1, 3, FIELD_CELL(Actor_Get(ACTOR_TWELFTH_LOG)->x.fixed),
      row - 1);
  MapGrid_CopyCellAttributes(0, 0, 1, 3, column, row - 1);
}

/// Walking west on the seventh log drifts it west.
static void CrossboneIsle_SlideSeventhLogWest(void) {
  s32 column;
  s32 row;
  s32 eleventhLogRow;
  s32 secondLogRow;
  s32 footprintRow;

  column = FIELD_CELL(Actor_Get(ACTOR_SEVENTH_LOG)->x.fixed);
  row = FIELD_CELL(Actor_Get(ACTOR_SEVENTH_LOG)->z.fixed);
  eleventhLogRow = FIELD_CELL(Actor_Get(ACTOR_ELEVENTH_LOG)->z.fixed);
  secondLogRow = FIELD_CELL(Actor_Get(ACTOR_SECOND_LOG)->z.fixed);

  if (column == 13) {
    if (secondLogRow >= 12 && secondLogRow <= 14) {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_SEVENTH_LOG, -16, 0);
    } else if (eleventhLogRow >= 12 && eleventhLogRow <= 14) {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_SEVENTH_LOG, -64, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_SEVENTH_LOG, -112, 0);
    }
  } else if (column == 12) {
    if (secondLogRow >= 12 && secondLogRow <= 14) {
      return;
    }
    if (eleventhLogRow >= 12 && eleventhLogRow <= 14) {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_SEVENTH_LOG, -48, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_SEVENTH_LOG, -96, 0);
    }
  } else if (column == 9) {
    if (eleventhLogRow >= 12 && eleventhLogRow <= 14) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndLog(ACTOR_SEVENTH_LOG, -48, 0);
  } else if (column == 8) {
    if (eleventhLogRow >= 12 && eleventhLogRow <= 14) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndLog(ACTOR_SEVENTH_LOG, -32, 0);
  } else if (column == 6) {
    return;
  }

  wait(2);

  footprintRow = row - 1;
  MapGrid_CopyCellAttributeRect(
      column, footprintRow, 1, 3,
      FIELD_CELL(Actor_Get(ACTOR_SEVENTH_LOG)->x.fixed), footprintRow);
  MapGrid_CopyCellAttributes(0, 0, 1, 3, column, footprintRow);
}

/// Walking east on the seventh log drifts it east.
static void CrossboneIsle_SlideSeventhLogEast(void) {
  s32 column;
  s32 row;
  s32 eleventhLogRow;
  s32 secondLogRow;
  s32 footprintRow;

  column = FIELD_CELL(Actor_Get(ACTOR_SEVENTH_LOG)->x.fixed);
  row = FIELD_CELL(Actor_Get(ACTOR_SEVENTH_LOG)->z.fixed);
  eleventhLogRow = FIELD_CELL(Actor_Get(ACTOR_ELEVENTH_LOG)->z.fixed);
  secondLogRow = FIELD_CELL(Actor_Get(ACTOR_SECOND_LOG)->z.fixed);

  if (column == 6) {
    if (secondLogRow >= 12 && secondLogRow <= 14) {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_SEVENTH_LOG, 32, 0);
    } else if (eleventhLogRow >= 12 && eleventhLogRow <= 14) {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_SEVENTH_LOG, 64, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_SEVENTH_LOG, 112, 0);
    }
  } else if (column == 8) {
    if (secondLogRow >= 12 && secondLogRow <= 14) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndLog(ACTOR_SEVENTH_LOG, 80, 0);
  } else if (column == 9) {
    if (secondLogRow >= 12 && secondLogRow <= 14) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndLog(ACTOR_SEVENTH_LOG, 64, 0);
  } else if (column == 12) {
    CrossboneIsle_MoveSelectedActorAndLog(ACTOR_SEVENTH_LOG, 16, 0);
  } else if (column == 13) {
    return;
  }

  wait(2);

  footprintRow = row - 1;
  MapGrid_CopyCellAttributeRect(
      column, footprintRow, 1, 3,
      FIELD_CELL(Actor_Get(ACTOR_SEVENTH_LOG)->x.fixed), footprintRow);
  MapGrid_CopyCellAttributes(0, 0, 1, 3, column, footprintRow);
}

/// Walking west on the ninth log drifts it west.
static void CrossboneIsle_SlideNinthLogWest(void) {
  s32 column;
  s32 row;
  s32 eleventhLogRow;
  s32 secondLogRow;
  s32 footprintRow;

  column = FIELD_CELL(Actor_Get(ACTOR_NINTH_LOG)->x.fixed);
  row = FIELD_CELL(Actor_Get(ACTOR_NINTH_LOG)->z.fixed);
  eleventhLogRow = FIELD_CELL(Actor_Get(ACTOR_ELEVENTH_LOG)->z.fixed);
  secondLogRow = FIELD_CELL(Actor_Get(ACTOR_SECOND_LOG)->z.fixed);

  if (column == 13) {
    if (secondLogRow >= 9 && secondLogRow <= 11) {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_NINTH_LOG, -16, 0);
    } else if (eleventhLogRow >= 9 && eleventhLogRow <= 11) {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_NINTH_LOG, -64, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_NINTH_LOG, -112, 0);
    }
  } else if (column == 12) {
    if (secondLogRow >= 9 && secondLogRow <= 11) {
      return;
    }
    if (eleventhLogRow >= 9 && eleventhLogRow <= 11) {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_NINTH_LOG, -48, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_NINTH_LOG, -96, 0);
    }
  } else if (column == 9) {
    if (eleventhLogRow >= 9 && eleventhLogRow <= 11) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndLog(ACTOR_NINTH_LOG, -48, 0);
  } else if (column == 8) {
    CrossboneIsle_MoveSelectedActorAndLog(ACTOR_NINTH_LOG, -32, 0);
  } else if (column == 6) {
    return;
  }

  wait(2);

  footprintRow = row - 1;
  MapGrid_CopyCellAttributeRect(column, footprintRow, 1, 3,
                                FIELD_CELL(Actor_Get(ACTOR_NINTH_LOG)->x.fixed),
                                footprintRow);
  MapGrid_CopyCellAttributes(0, 0, 1, 3, column, footprintRow);
}

/// Walking east on the ninth log drifts it east.
static void CrossboneIsle_SlideNinthLogEast(void) {
  s32 column;
  s32 row;
  s32 companionRow;

  column = FIELD_CELL(Actor_Get(ACTOR_NINTH_LOG)->x.fixed);
  row = FIELD_CELL(Actor_Get(ACTOR_NINTH_LOG)->z.fixed);
  companionRow = FIELD_CELL(Actor_Get(ACTOR_SECOND_LOG)->z.fixed);

  if (column == 6) {
    if (companionRow >= 9 && companionRow <= 11) {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_NINTH_LOG, 32, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndLog(ACTOR_NINTH_LOG, 112, 0);
    }
  } else if (column == 8) {
    if (companionRow >= 9 && companionRow <= 11) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndLog(ACTOR_NINTH_LOG, 80, 0);
  } else if (column == 9) {
    CrossboneIsle_MoveSelectedActorAndLog(ACTOR_NINTH_LOG, 64, 0);
  } else if (column == 12) {
    CrossboneIsle_MoveSelectedActorAndLog(ACTOR_NINTH_LOG, 16, 0);
  } else if (column == 13) {
    return;
  }

  wait(2);

  MapGrid_CopyCellAttributeRect(column, row - 1, 1, 3,
                                FIELD_CELL(Actor_Get(ACTOR_NINTH_LOG)->x.fixed),
                                row - 1);
  MapGrid_CopyCellAttributes(0, 0, 1, 3, column, row - 1);
}

#include "shared/actor_orbit.c"

#include "shared/pushed_actor_tables.c"
// clang-format off

#include "shared/field_effect_duration_scripts.c"

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

struct SceneEntrance gCrossboneIsle_IsleEntrances[] = {
    { CROSSBONE_ISLE_FROM_WORLD_MAP, CONDITION_ALWAYS, 280, 0, 544, FACING_NORTH, 0, 0, 0, 512, 560, 0 },
    { CROSSBONE_ISLE_FROM_CROSSBONE_ISLE_CAVE_ENTRANCE, CONDITION_ALWAYS, 136, 0, 360, FACING_SOUTH, 0, 0, 0, 512, 560, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gCrossboneIsle_CaveEntranceEntrances[] = {
    { CROSSBONE_ISLE_CAVE_ENTRANCE_FROM_CROSSBONE_ISLE, CONDITION_ALWAYS, 312, 0, 520, FACING_NORTH, 0, 16, 64, 416, 544, 0 },
    { CROSSBONE_ISLE_CAVE_ENTRANCE_FROM_CROSSBONE_ISLE_FIRST_FLOOR, CONDITION_ALWAYS, 280, 0, 144, FACING_EAST, 0, 16, 64, 416, 544, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gCrossboneIsle_FloatingLogsEntrances[] = {
    { CROSSBONE_ISLE_FLOATING_LOGS_FROM_CROSSBONE_ISLE_NINTH_FLOOR_EAST, CONDITION_ALWAYS, 312, 0, 408, FACING_NORTH, 0, 0, 16, 640, 464, 0 },
    { CROSSBONE_ISLE_FLOATING_LOGS_FROM_CROSSBONE_ISLE_NINTH_FLOOR_WEST, CONDITION_ALWAYS, 200, 0, 408, FACING_NORTH, 0, 0, 16, 640, 464, 0 },
    { CROSSBONE_ISLE_FLOATING_LOGS_MIDDLE, CONDITION_ALWAYS, 272, 0, 232, FACING_SOUTH, 0, 0, 16, 640, 464, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gCrossboneIsle_PierEntrances[] = {
    { CROSSBONE_ISLE_PIER_FROM_CROSSBONE_ISLE_NINTH_FLOOR, CONDITION_ALWAYS, 184, 0, 136, FACING_WEST, 0, -1, -1, -1, -1, 0 },
    { CROSSBONE_ISLE_PIER_FROM_PIRATE_SHIP, CONDITION_ALWAYS, 40, 0, 184, FACING_EAST, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gCrossboneIsle_PirateShipEntrances[] = {
    { PIRATE_SHIP_FROM_CROSSBONE_ISLE_PIER, CONDITION_ALWAYS, 312, 0, 656, FACING_WEST, 0, -1, -1, -1, -1, 0 },
    { ENTRANCE_AFTER_BATTLE, CONDITION_ALWAYS, 216, 0, 544, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    // The nine corridor rooms (CrossboneIsleFirstFloor to
    // CrossboneIsleNinthFloor) enter through this shared tail, since every room
    // numbers its entrances alike.

    { CORRIDOR_FROM_PREVIOUS_ROOM, CONDITION_ALWAYS, 840, 0, 176, FACING_WEST, 0, 432, 32, 880, 352, 0 },
    { CORRIDOR_FROM_NEXT_ROOM, CONDITION_ALWAYS, 488, 0, 176, FACING_EAST, 0, 432, 32, 880, 352, 0 },
    { CORRIDOR_FROM_SIDE_ROOM_EAST, CONDITION_ALWAYS, 712, 0, 152, FACING_SOUTH, 0, 432, 32, 880, 352, 0 },
    { CORRIDOR_FROM_SIDE_ROOM_WEST, CONDITION_ALWAYS, 600, 0, 152, FACING_SOUTH, 0, 432, 32, 880, 352, 0 },
    { ENTRANCE_AFTER_BATTLE, CONDITION_ALWAYS, 712, 0, 152, FACING_NORTH, 0, 432, 32, 880, 352, 0 },
    { SCENE_TABLE_END },
};

u32 gCrossboneIsleExits[] = {
    SCENE_EXITS(CrossboneIsle),
    SCENE_EXIT(CROSSBONE_ISLE_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_CROSSBONE_ISLE),
    SCENE_EXIT(CROSSBONE_ISLE_TO_CROSSBONE_ISLE_CAVE_ENTRANCE, CrossboneIsleCaveEntrance, CROSSBONE_ISLE_CAVE_ENTRANCE_FROM_CROSSBONE_ISLE),
    SCENE_EXITS(CrossboneIsleCaveEntrance),
    SCENE_EXIT(CROSSBONE_ISLE_CAVE_ENTRANCE_TO_CROSSBONE_ISLE, CrossboneIsle, CROSSBONE_ISLE_FROM_CROSSBONE_ISLE_CAVE_ENTRANCE),
    SCENE_EXIT(CROSSBONE_ISLE_CAVE_ENTRANCE_TO_CROSSBONE_ISLE_FIRST_FLOOR, CrossboneIsleFirstFloor, CROSSBONE_ISLE_FIRST_FLOOR_FROM_CROSSBONE_ISLE_CAVE_ENTRANCE),
    SCENE_EXITS(CrossboneIsleFloatingLogs),
    SCENE_EXIT(CROSSBONE_ISLE_FLOATING_LOGS_TO_CROSSBONE_ISLE_NINTH_FLOOR_EAST, CrossboneIsleNinthFloor, CROSSBONE_ISLE_NINTH_FLOOR_FROM_CROSSBONE_ISLE_FLOATING_LOGS_EAST),
    SCENE_EXIT(CROSSBONE_ISLE_FLOATING_LOGS_TO_CROSSBONE_ISLE_NINTH_FLOOR_WEST, CrossboneIsleNinthFloor, CROSSBONE_ISLE_NINTH_FLOOR_FROM_CROSSBONE_ISLE_FLOATING_LOGS_WEST),
    SCENE_EXITS(CrossboneIslePier),
    SCENE_EXIT(CROSSBONE_ISLE_PIER_TO_CROSSBONE_ISLE_NINTH_FLOOR, CrossboneIsleNinthFloor, CROSSBONE_ISLE_NINTH_FLOOR_FROM_CROSSBONE_ISLE_PIER),
    SCENE_EXIT(CROSSBONE_ISLE_PIER_TO_PIRATE_SHIP, PirateShip, PIRATE_SHIP_FROM_CROSSBONE_ISLE_PIER),
    SCENE_EXITS(PirateShip),
    SCENE_EXIT(PIRATE_SHIP_TO_CROSSBONE_ISLE_PIER, CrossboneIslePier, CROSSBONE_ISLE_PIER_FROM_PIRATE_SHIP),
    SCENE_EXITS(CrossboneIsleFirstFloor),
    SCENE_EXIT(CROSSBONE_ISLE_FIRST_FLOOR_TO_CROSSBONE_ISLE_CAVE_ENTRANCE, CrossboneIsleCaveEntrance, CROSSBONE_ISLE_CAVE_ENTRANCE_FROM_CROSSBONE_ISLE_FIRST_FLOOR),
    SCENE_EXIT(CROSSBONE_ISLE_FIRST_FLOOR_TO_CROSSBONE_ISLE_SECOND_FLOOR, CrossboneIsleSecondFloor, CROSSBONE_ISLE_SECOND_FLOOR_FROM_CROSSBONE_ISLE_FIRST_FLOOR),
    SCENE_EXIT(CROSSBONE_ISLE_FIRST_FLOOR_TO_CROSSBONE_ISLE_STEPPING_STONES_EAST, CrossboneIsleSteppingStones, CROSSBONE_ISLE_STEPPING_STONES_FROM_CROSSBONE_ISLE_FIRST_FLOOR_EAST),
    SCENE_EXIT(CROSSBONE_ISLE_FIRST_FLOOR_TO_CROSSBONE_ISLE_STEPPING_STONES_WEST, CrossboneIsleSteppingStones, CROSSBONE_ISLE_STEPPING_STONES_FROM_CROSSBONE_ISLE_FIRST_FLOOR_WEST),
    SCENE_EXITS(CrossboneIsleSecondFloor),
    SCENE_EXIT(CROSSBONE_ISLE_SECOND_FLOOR_TO_CROSSBONE_ISLE_FIRST_FLOOR, CrossboneIsleFirstFloor, CROSSBONE_ISLE_FIRST_FLOOR_FROM_CROSSBONE_ISLE_SECOND_FLOOR),
    SCENE_EXIT(CROSSBONE_ISLE_SECOND_FLOOR_TO_CROSSBONE_ISLE_THIRD_FLOOR, CrossboneIsleThirdFloor, CROSSBONE_ISLE_THIRD_FLOOR_FROM_CROSSBONE_ISLE_SECOND_FLOOR),
    SCENE_EXIT(CROSSBONE_ISLE_SECOND_FLOOR_TO_CROSSBONE_ISLE_ICE_PILLARS_EAST, CrossboneIsleIcePillars, CROSSBONE_ISLE_ICE_PILLARS_FROM_CROSSBONE_ISLE_SECOND_FLOOR_EAST),
    SCENE_EXIT(CROSSBONE_ISLE_SECOND_FLOOR_TO_CROSSBONE_ISLE_ICE_PILLARS_WEST, CrossboneIsleIcePillars, CROSSBONE_ISLE_ICE_PILLARS_FROM_CROSSBONE_ISLE_SECOND_FLOOR_WEST),
    SCENE_EXITS(CrossboneIsleThirdFloor),
    SCENE_EXIT(CROSSBONE_ISLE_THIRD_FLOOR_TO_CROSSBONE_ISLE_SECOND_FLOOR, CrossboneIsleSecondFloor, CROSSBONE_ISLE_SECOND_FLOOR_FROM_CROSSBONE_ISLE_THIRD_FLOOR),
    SCENE_EXIT(CROSSBONE_ISLE_THIRD_FLOOR_TO_CROSSBONE_ISLE_FOURTH_FLOOR, CrossboneIsleFourthFloor, CROSSBONE_ISLE_FOURTH_FLOOR_FROM_CROSSBONE_ISLE_THIRD_FLOOR),
    SCENE_EXIT(CROSSBONE_ISLE_THIRD_FLOOR_TO_CROSSBONE_ISLE_KEYS_EAST, CrossboneIsleKeys, CROSSBONE_ISLE_KEYS_FROM_CROSSBONE_ISLE_THIRD_FLOOR_EAST),
    SCENE_EXIT(CROSSBONE_ISLE_THIRD_FLOOR_TO_CROSSBONE_ISLE_KEYS_WEST, CrossboneIsleKeys, CROSSBONE_ISLE_KEYS_FROM_CROSSBONE_ISLE_THIRD_FLOOR_WEST),
    SCENE_EXITS(CrossboneIsleFourthFloor),
    SCENE_EXIT(CROSSBONE_ISLE_FOURTH_FLOOR_TO_CROSSBONE_ISLE_THIRD_FLOOR, CrossboneIsleThirdFloor, CROSSBONE_ISLE_THIRD_FLOOR_FROM_CROSSBONE_ISLE_FOURTH_FLOOR),
    SCENE_EXIT(CROSSBONE_ISLE_FOURTH_FLOOR_TO_CROSSBONE_ISLE_FIFTH_FLOOR, CrossboneIsleFifthFloor, CROSSBONE_ISLE_FIFTH_FLOOR_FROM_CROSSBONE_ISLE_FOURTH_FLOOR),
    SCENE_EXIT(CROSSBONE_ISLE_FOURTH_FLOOR_TO_CROSSBONE_ISLE_STATUES_EAST, CrossboneIsleStatues, CROSSBONE_ISLE_STATUES_FROM_CROSSBONE_ISLE_FOURTH_FLOOR_EAST),
    SCENE_EXIT(CROSSBONE_ISLE_FOURTH_FLOOR_TO_CROSSBONE_ISLE_STATUES_WEST, CrossboneIsleStatues, CROSSBONE_ISLE_STATUES_FROM_CROSSBONE_ISLE_FOURTH_FLOOR_WEST),
    SCENE_EXITS(CrossboneIsleFifthFloor),
    SCENE_EXIT(CROSSBONE_ISLE_FIFTH_FLOOR_TO_CROSSBONE_ISLE_FOURTH_FLOOR, CrossboneIsleFourthFloor, CROSSBONE_ISLE_FOURTH_FLOOR_FROM_CROSSBONE_ISLE_FIFTH_FLOOR),
    SCENE_EXIT(CROSSBONE_ISLE_FIFTH_FLOOR_TO_CROSSBONE_ISLE_SIXTH_FLOOR, CrossboneIsleSixthFloor, CROSSBONE_ISLE_SIXTH_FLOOR_FROM_CROSSBONE_ISLE_FIFTH_FLOOR),
    SCENE_EXIT(CROSSBONE_ISLE_FIFTH_FLOOR_TO_CROSSBONE_ISLE_FLOODGATE_EAST, CrossboneIsleFloodgate, CROSSBONE_ISLE_FLOODGATE_FROM_CROSSBONE_ISLE_FIFTH_FLOOR_EAST),
    SCENE_EXIT(CROSSBONE_ISLE_FIFTH_FLOOR_TO_CROSSBONE_ISLE_FLOODGATE_WEST, CrossboneIsleFloodgate, CROSSBONE_ISLE_FLOODGATE_FROM_CROSSBONE_ISLE_FIFTH_FLOOR_WEST),
    SCENE_EXITS(CrossboneIsleSixthFloor),
    SCENE_EXIT(CROSSBONE_ISLE_SIXTH_FLOOR_TO_CROSSBONE_ISLE_FIFTH_FLOOR, CrossboneIsleFifthFloor, CROSSBONE_ISLE_FIFTH_FLOOR_FROM_CROSSBONE_ISLE_SIXTH_FLOOR),
    SCENE_EXIT(CROSSBONE_ISLE_SIXTH_FLOOR_TO_CROSSBONE_ISLE_SEVENTH_FLOOR, CrossboneIsleSeventhFloor, CROSSBONE_ISLE_SEVENTH_FLOOR_FROM_CROSSBONE_ISLE_SIXTH_FLOOR),
    SCENE_EXIT(CROSSBONE_ISLE_SIXTH_FLOOR_TO_CROSSBONE_ISLE_BANE_EAST, CrossboneIsleBane, CROSSBONE_ISLE_BANE_FROM_CROSSBONE_ISLE_SIXTH_FLOOR_EAST),
    SCENE_EXIT(CROSSBONE_ISLE_SIXTH_FLOOR_TO_CROSSBONE_ISLE_BANE_WEST, CrossboneIsleBane, CROSSBONE_ISLE_BANE_FROM_CROSSBONE_ISLE_SIXTH_FLOOR_WEST),
    SCENE_EXITS(CrossboneIsleSeventhFloor),
    SCENE_EXIT(CROSSBONE_ISLE_SEVENTH_FLOOR_TO_CROSSBONE_ISLE_SIXTH_FLOOR, CrossboneIsleSixthFloor, CROSSBONE_ISLE_SIXTH_FLOOR_FROM_CROSSBONE_ISLE_SEVENTH_FLOOR),
    SCENE_EXIT(CROSSBONE_ISLE_SEVENTH_FLOOR_TO_CROSSBONE_ISLE_EIGHTH_FLOOR, CrossboneIsleEighthFloor, CROSSBONE_ISLE_EIGHTH_FLOOR_FROM_CROSSBONE_ISLE_SEVENTH_FLOOR),
    SCENE_EXIT(CROSSBONE_ISLE_SEVENTH_FLOOR_TO_CROSSBONE_ISLE_STACKED_PILLARS_EAST, CrossboneIsleStackedPillars, CROSSBONE_ISLE_STACKED_PILLARS_FROM_CROSSBONE_ISLE_SEVENTH_FLOOR_EAST),
    SCENE_EXIT(CROSSBONE_ISLE_SEVENTH_FLOOR_TO_CROSSBONE_ISLE_STACKED_PILLARS_WEST, CrossboneIsleStackedPillars, CROSSBONE_ISLE_STACKED_PILLARS_FROM_CROSSBONE_ISLE_SEVENTH_FLOOR_WEST),
    SCENE_EXITS(CrossboneIsleEighthFloor),
    SCENE_EXIT(CROSSBONE_ISLE_EIGHTH_FLOOR_TO_CROSSBONE_ISLE_SEVENTH_FLOOR, CrossboneIsleSeventhFloor, CROSSBONE_ISLE_SEVENTH_FLOOR_FROM_CROSSBONE_ISLE_EIGHTH_FLOOR),
    SCENE_EXIT(CROSSBONE_ISLE_EIGHTH_FLOOR_TO_CROSSBONE_ISLE_NINTH_FLOOR, CrossboneIsleNinthFloor, CROSSBONE_ISLE_NINTH_FLOOR_FROM_CROSSBONE_ISLE_EIGHTH_FLOOR),
    SCENE_EXIT(CROSSBONE_ISLE_EIGHTH_FLOOR_TO_CROSSBONE_ISLE_DRAGON_STATUE_EAST, CrossboneIsleDragonStatue, CROSSBONE_ISLE_DRAGON_STATUE_FROM_CROSSBONE_ISLE_EIGHTH_FLOOR_EAST),
    SCENE_EXIT(CROSSBONE_ISLE_EIGHTH_FLOOR_TO_CROSSBONE_ISLE_DRAGON_STATUE_WEST, CrossboneIsleDragonStatue, CROSSBONE_ISLE_DRAGON_STATUE_FROM_CROSSBONE_ISLE_EIGHTH_FLOOR_WEST),
    SCENE_EXITS(CrossboneIsleNinthFloor),
    SCENE_EXIT(CROSSBONE_ISLE_NINTH_FLOOR_TO_CROSSBONE_ISLE_EIGHTH_FLOOR, CrossboneIsleEighthFloor, CROSSBONE_ISLE_EIGHTH_FLOOR_FROM_CROSSBONE_ISLE_NINTH_FLOOR),
    SCENE_EXIT(CROSSBONE_ISLE_NINTH_FLOOR_TO_CROSSBONE_ISLE_PIER, CrossboneIslePier, CROSSBONE_ISLE_PIER_FROM_CROSSBONE_ISLE_NINTH_FLOOR),
    SCENE_EXIT(CROSSBONE_ISLE_NINTH_FLOOR_TO_CROSSBONE_ISLE_FLOATING_LOGS_EAST, CrossboneIsleFloatingLogs, CROSSBONE_ISLE_FLOATING_LOGS_FROM_CROSSBONE_ISLE_NINTH_FLOOR_EAST),
    SCENE_EXIT(CROSSBONE_ISLE_NINTH_FLOOR_TO_CROSSBONE_ISLE_FLOATING_LOGS_WEST, CrossboneIsleFloatingLogs, CROSSBONE_ISLE_FLOATING_LOGS_FROM_CROSSBONE_ISLE_NINTH_FLOOR_WEST),
    SCENE_EXITS_END,
};

struct ActorPlacement gCrossboneIsle_NoActorPlacements[] = {
    { SCENE_TABLE_END },
};

struct ActorPlacement gCrossboneIsle_IsleActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_BLANK, FLAG_CROSSBONE_ISLE_NUT_CAUGHT, MOVEMENT_TYPE_RESET_ACTION,
        344, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        144, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        352, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        88, 0, 368, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        104, 0, 336, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        152, 0, 320, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        136, 0, 352, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        88, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gCrossboneIsle_FloatingLogsActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        408, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        168, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        520, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        584, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        552, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        584, 0, 136, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        216, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        392, 0, 136, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        216, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        72, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        120, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        216, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gCrossboneIsle_CorridorActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        632, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

struct Trigger gCrossboneIsle_CorridorTriggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, CORRIDOR_TO_PREVIOUS_ROOM },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, CORRIDOR_TO_NEXT_ROOM },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT(CrossboneIsle_OpenEastDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT(CrossboneIsle_OpenWestDoor) },
    { TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 20, CONDITION_ALWAYS, EVENT(CrossboneIsle_PushPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SINKING_PILLAR, CONDITION_ALWAYS, EVENT(CrossboneIsle_MovePillar) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 100, FLAG_CROSSBONE_ISLE_FIRST_FLOOR_MIMIC_BEATEN, FIND_MIMIC | 4 },
    { TRIGGER_TABLE_END },
};

struct Trigger gCrossboneIsle_CaveEntranceTriggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, CROSSBONE_ISLE_CAVE_ENTRANCE_TO_CROSSBONE_ISLE },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, CROSSBONE_ISLE_CAVE_ENTRANCE_TO_CROSSBONE_ISLE_FIRST_FLOOR },
    { TRIGGER_TABLE_END },
};

struct Trigger gCrossboneIsle_IsleTriggers[] = {
    { PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_ISLE_NUT, FLAG_CROSSBONE_ISLE_NUT_CAUGHT, EVENT(CrossboneIsle_CatchNut) },
    { PSYNERGY_ON_ACTOR(ABILITY_GROWTH), ACTOR_FIRST_PLANT, FLAG_CROSSBONE_ISLE_FIRST_PLANT_GROWN, EVENT(CrossboneIsle_UnusedGrowFirstPlant) },
    { PSYNERGY_ON_ACTOR(ABILITY_GROWTH), ACTOR_FIRST_PLANT + 1, FLAG_CROSSBONE_ISLE_FIRST_PLANT_GROWN + 1, EVENT(CrossboneIsle_GrowSecondPlant) },
    { PSYNERGY_ON_ACTOR(ABILITY_GROWTH), ACTOR_FIRST_PLANT + 2, FLAG_CROSSBONE_ISLE_FIRST_PLANT_GROWN + 2, EVENT(CrossboneIsle_GrowThirdPlant) },
    { PSYNERGY_ON_ACTOR(ABILITY_GROWTH), ACTOR_FIRST_PLANT + 3, FLAG_CROSSBONE_ISLE_FIRST_PLANT_GROWN + 3, EVENT(CrossboneIsle_GrowFourthPlant) },
    { PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_IVY, FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY, EVENT(CrossboneIsle_BlowAwayFirstIvy) },
    { PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_IVY + 1, FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY + 1, EVENT(CrossboneIsle_UnusedBlowAwaySecondIvy) },
    { PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_IVY + 2, FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY + 2, EVENT(CrossboneIsle_BlowAwayThirdIvy) },
    { PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_IVY + 3, FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY + 3, EVENT(CrossboneIsle_UnusedBlowAwayFourthIvy) },
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, CROSSBONE_ISLE_TO_WORLD_MAP },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, CROSSBONE_ISLE_TO_CROSSBONE_ISLE_CAVE_ENTRANCE },
    { TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 20, CONDITION_ALWAYS, EVENT(CrossboneIsle_PushIsleLog) },
    { TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 10, CONDITION_ALWAYS, EVENT(CrossboneIsle_PushIsleLog) },
    { EXAMINE_CHEST_FACING(FACING_WEST), 100, FLAG_CROSSBONE_ISLE_COIN, FIND_COINS(1) },
    { EXAMINE_CHEST_FACING(FACING_NORTH), 100, FLAG_CROSSBONE_ISLE_COIN, FIND_COINS(1) },
    { EXAMINE_CHEST_FACING(FACING_NORTHWEST), 100, FLAG_CROSSBONE_ISLE_COIN, FIND_COINS(1) },
    { EXAMINE_CHEST_FACING(FACING_WEST), 101, FLAG_CROSSBONE_ISLE_MINT, FIND_ITEM(ITEM_MINT) },
    { EXAMINE_CHEST_FACING(FACING_NORTH), 101, FLAG_CROSSBONE_ISLE_MINT, FIND_ITEM(ITEM_MINT) },
    { EXAMINE_CHEST_FACING(FACING_NORTHWEST), 101, FLAG_CROSSBONE_ISLE_MINT, FIND_ITEM(ITEM_MINT) },
    { TRIGGER_TABLE_END },
};

struct Trigger gCrossboneIsle_FloatingLogsTriggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, CROSSBONE_ISLE_FLOATING_LOGS_TO_CROSSBONE_ISLE_NINTH_FLOOR_EAST },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, CROSSBONE_ISLE_FLOATING_LOGS_TO_CROSSBONE_ISLE_NINTH_FLOOR_WEST },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), 40, CONDITION_ALWAYS, EVENT(CrossboneIsle_HopWest) },
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), 41, CONDITION_ALWAYS, EVENT(CrossboneIsle_HopEast) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 42, CONDITION_ALWAYS, EVENT(CrossboneIsle_HopSouth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 43, CONDITION_ALWAYS, EVENT(CrossboneIsle_HopNorth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 21, CONDITION_ALWAYS, EVENT(CrossboneIsle_SlideFirstLogNorth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 21, CONDITION_ALWAYS, EVENT(CrossboneIsle_SlideFirstLogSouth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), 21, CONDITION_ALWAYS, EVENT(CrossboneIsle_HopEast) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), 21, CONDITION_ALWAYS, EVENT(CrossboneIsle_HopWest) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 22, CONDITION_ALWAYS, EVENT(CrossboneIsle_SlideThirdLogNorth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 22, CONDITION_ALWAYS, EVENT(CrossboneIsle_SlideThirdLogSouth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), 22, CONDITION_ALWAYS, EVENT(CrossboneIsle_HopEast) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), 22, CONDITION_ALWAYS, EVENT(CrossboneIsle_HopWest) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), 23, CONDITION_ALWAYS, EVENT(CrossboneIsle_SlideFourthLogWest) },
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), 23, CONDITION_ALWAYS, EVENT(CrossboneIsle_SlideFourthLogEast) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 23, CONDITION_ALWAYS, EVENT(CrossboneIsle_HopNorth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 23, CONDITION_ALWAYS, EVENT(CrossboneIsle_HopSouth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), 24, CONDITION_ALWAYS, EVENT(CrossboneIsle_SlideFifthLogWest) },
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), 24, CONDITION_ALWAYS, EVENT(CrossboneIsle_SlideFifthLogEast) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 24, CONDITION_ALWAYS, EVENT(CrossboneIsle_HopNorth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 24, CONDITION_ALWAYS, EVENT(CrossboneIsle_HopSouth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), 25, CONDITION_ALWAYS, EVENT(CrossboneIsle_SlideSixthLogWest) },
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), 25, CONDITION_ALWAYS, EVENT(CrossboneIsle_SlideSixthLogEast) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 25, CONDITION_ALWAYS, EVENT(CrossboneIsle_HopNorth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 25, CONDITION_ALWAYS, EVENT(CrossboneIsle_HopSouth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), 26, CONDITION_ALWAYS, EVENT(CrossboneIsle_SlideEighthLogWest) },
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), 26, CONDITION_ALWAYS, EVENT(CrossboneIsle_SlideEighthLogEast) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 26, CONDITION_ALWAYS, EVENT(CrossboneIsle_HopNorth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 26, CONDITION_ALWAYS, EVENT(CrossboneIsle_HopSouth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 31, CONDITION_ALWAYS, EVENT(CrossboneIsle_SlideTenthLogNorth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 31, CONDITION_ALWAYS, EVENT(CrossboneIsle_SlideTenthLogSouth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), 31, CONDITION_ALWAYS, EVENT(CrossboneIsle_HopEast) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), 31, CONDITION_ALWAYS, EVENT(CrossboneIsle_HopWest) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 32, CONDITION_ALWAYS, EVENT(CrossboneIsle_SlideEleventhLogNorth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 32, CONDITION_ALWAYS, EVENT(CrossboneIsle_SlideEleventhLogSouth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), 32, CONDITION_ALWAYS, EVENT(CrossboneIsle_HopEast) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), 32, CONDITION_ALWAYS, EVENT(CrossboneIsle_HopWest) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 33, CONDITION_ALWAYS, EVENT(CrossboneIsle_SlideSecondLogNorth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 33, CONDITION_ALWAYS, EVENT(CrossboneIsle_SlideSecondLogSouth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), 33, CONDITION_ALWAYS, EVENT(CrossboneIsle_HopEast) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), 33, CONDITION_ALWAYS, EVENT(CrossboneIsle_HopWest) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), 34, CONDITION_ALWAYS, EVENT(CrossboneIsle_SlideTwelfthLogWest) },
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), 34, CONDITION_ALWAYS, EVENT(CrossboneIsle_SlideTwelfthLogEast) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 34, CONDITION_ALWAYS, EVENT(CrossboneIsle_HopNorth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 34, CONDITION_ALWAYS, EVENT(CrossboneIsle_HopSouth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), 35, CONDITION_ALWAYS, EVENT(CrossboneIsle_SlideSeventhLogWest) },
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), 35, CONDITION_ALWAYS, EVENT(CrossboneIsle_SlideSeventhLogEast) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 35, CONDITION_ALWAYS, EVENT(CrossboneIsle_HopNorth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 35, CONDITION_ALWAYS, EVENT(CrossboneIsle_HopSouth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), 36, CONDITION_ALWAYS, EVENT(CrossboneIsle_SlideNinthLogWest) },
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), 36, CONDITION_ALWAYS, EVENT(CrossboneIsle_SlideNinthLogEast) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 36, CONDITION_ALWAYS, EVENT(CrossboneIsle_HopNorth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 36, CONDITION_ALWAYS, EVENT(CrossboneIsle_HopSouth) },
    { TRIGGER_STEP, 21, CONDITION_ALWAYS, EVENT(CrossboneIsle_StepOnFirstLog) },
    { TRIGGER_STEP, 22, CONDITION_ALWAYS, EVENT(CrossboneIsle_StepOnThirdLog) },
    { TRIGGER_STEP, 23, CONDITION_ALWAYS, EVENT(CrossboneIsle_StepOnFourthLog) },
    { TRIGGER_STEP, 24, CONDITION_ALWAYS, EVENT(CrossboneIsle_StepOnFifthLog) },
    { TRIGGER_STEP, 25, CONDITION_ALWAYS, EVENT(CrossboneIsle_StepOnSixthLog) },
    { TRIGGER_STEP, 26, CONDITION_ALWAYS, EVENT(CrossboneIsle_StepOnEighthLog) },
    { TRIGGER_STEP, 31, CONDITION_ALWAYS, EVENT(CrossboneIsle_StepOnTenthLog) },
    { TRIGGER_STEP, 32, CONDITION_ALWAYS, EVENT(CrossboneIsle_StepOnEleventhLog) },
    { TRIGGER_STEP, 33, CONDITION_ALWAYS, EVENT(CrossboneIsle_StepOnSecondLog) },
    { TRIGGER_STEP, 34, CONDITION_ALWAYS, EVENT(CrossboneIsle_StepOnTwelfthLog) },
    { TRIGGER_STEP, 35, CONDITION_ALWAYS, EVENT(CrossboneIsle_StepOnSeventhLog) },
    { TRIGGER_STEP, 36, CONDITION_ALWAYS, EVENT(CrossboneIsle_StepOnNinthLog) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 100, FLAG_CROSSBONE_ISLE_FLOATING_LOGS_999_COINS, FIND_COINS(999) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 101, FLAG_CROSSBONE_ISLE_FLOATING_LOGS_WATER_OF_LIFE, FIND_ITEM(ITEM_WATER_OF_LIFE) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 102, FLAG_CROSSBONE_ISLE_FLOATING_LOGS_MURAMASA, FIND_ITEM(ITEM_MURAMASA) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 103, FLAG_CROSSBONE_ISLE_FLOATING_LOGS_SLEEP_BOMB, FIND_ITEM(ITEM_SLEEP_BOMB) },
    { TRIGGER_TABLE_END },
};
// clang-format on

struct Trigger gCrossboneIsle_PierTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS,
     CROSSBONE_ISLE_PIER_TO_CROSSBONE_ISLE_NINTH_FLOOR},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, CROSSBONE_ISLE_PIER_TO_PIRATE_SHIP},
    {TRIGGER_TABLE_END},
};

struct Trigger gCrossboneIsle_PirateShipTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, PIRATE_SHIP_TO_CROSSBONE_ISLE_PIER},
    {TRIGGER_STEP, 10, FLAG_CROSSBONE_ISLE_DEADBEARD_DEFEATED,
     EVENT(CrossboneIsle_FightDeadbeard)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 20, CONDITION_ALWAYS,
     EVENT(CrossboneIsle_HopSouth)},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), 100, FLAG_PIRATE_SHIP_DEMON_MAIL,
     FIND_ITEM(ITEM_DEMON_MAIL)},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), 101, FLAG_PIRATE_SHIP_MIMIC_BEATEN,
     FIND_MIMIC | 7},
    {TRIGGER_TABLE_END},
};

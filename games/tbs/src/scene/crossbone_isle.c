/// @file
/// Crossbone Isle, the island of puzzles off the coast that the party can
/// reach from the World Map, and the cave beneath it that winds down nine
/// floors to Deadbeard's Pirate Ship. Its area names are Crossbone Isle,
/// Crossbone Isle Cave and the Pirate Ship. No one here speaks.
///
/// In the order the party meets them:
///
/// - Crossbone Isle (SceneId_CrossboneIsle), the island. The party lands at
///   the south (WORLD_MAP_TO_CROSSBONE_ISLE) and walks west to the foot of a
///   cliff, where Growth on three sprouts, one on each ledge, grows the vines
///   that climb it to the top. They stay grown only until the party leaves
///   the island. Catch takes a Nut from the tree by the waterfall. From the
///   top, slides lead back down: one to the chest with a coin, one to a log
///   that has to be pushed aside to reach the chest with the Mint, and one to
///   a clump of ivy at the foot of the cliff. Whirlwind blows that ivy away for
///   good and uncovers the mouth of the cave
///   (CROSSBONE_ISLE_TO_CROSSBONE_ISLE_CAVE_ENTRANCE). A second clump, on the
///   top ledge beside the third vine, hides nothing.
///
/// - Crossbone Isle Cave (SceneId_CrossboneIsleCaveEntrance), the winding
///   path lit by torches that leads from the cave's mouth down to the first
///   floor.
///
/// - Crossbone Isle Cave (SceneId_CrossboneIsleFirstFloor to
///   SceneId_CrossboneIsleNinthFloor), the nine floors, each a hall like the
///   last. The party comes in at the east end (FLOOR_FROM_PREVIOUS_ROOM) and
///   leaves down the stairs at the west end (FLOOR_TO_NEXT_ROOM). The middle
///   of the hall is two raised ledges with a gap between them, each with a
///   door in the north wall into the floor's side room, and the side room is
///   the way across. The first time the party walks into the east door
///   (CrossboneIsle_OpenEastDoor), the monsters behind it attack, a group of
///   ENCOUNTER_ZONE_CROSSBONE_ISLE_FIRST_GUARDS on the first four floors and
///   of ENCOUNTER_ZONE_CROSSBONE_ISLE_LAST_GUARDS on the next five. Back from
///   the battle the door opens and the party walks in, and from then on it
///   simply opens. On the west ledge, at the edge of the gap, stands a stone
///   pillar: pushed east, it drops into the gap and makes a step across it
///   for good, so the way back east no longer needs the side room. On the
///   first floor the chest in front of the west door is a Mimic.
///
///   scene_exits.h names each floor's doors floor by floor
///   (CROSSBONE_ISLE_FIRST_FLOOR_FROM_CROSSBONE_ISLE_CAVE_ENTRANCE and
///   CROSSBONE_ISLE_FIRST_FLOOR_TO_CROSSBONE_ISLE_SECOND_FLOOR on the first),
///   and the side rooms of the first eight floors are in
///   crossbone_isle_stepping_stones.c and crossbone_isle_keys.c.
///
/// - Crossbone Isle Cave (SceneId_CrossboneIsleFloatingLogs), the ninth
///   floor's side room: two pools of green water with twelve logs floating on
///   them. Walking onto a log and on along it rolls the log across the pool
///   with Isaac on top, until the far shore or another log stops it, and he
///   hops from log to log or ashore over a cell of water. The logs lead to
///   four chests: 999 coins on the island in the east pool, a Sleep Bomb in
///   the alcove north of the east pool, a Water of Life on the ledge between
///   the pools, and the Muramasa in the alcove north of the west pool.
///
/// - Crossbone Isle Cave (SceneId_CrossboneIslePier), the ledge past the ninth
///   floor where a wooden pier leads out to the Pirate Ship.
///
/// - Pirate Ship (SceneId_PirateShip), at the bottom. The first chest, at the
///   foot of the ladder in the middle of the deck, is a Mimic. Climbing the
///   west stairs onto the raised foredeck sets off the battle with Deadbeard
///   (CrossboneIsle_FightDeadbeard). Once he is beaten, a chest with the Demon
///   Mail stands on the foredeck behind where the battle began.
///
/// Scene_Initialize restores each scene as its flags say: the ivy and the
/// vines on the island, each floor's pillar in its gap, the Mimics until they
/// are beaten and the chest Deadbeard leaves. The floating logs start in their
/// places on every visit, their cells laid under them
/// (CrossboneIsle_PlaceLogs).

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "types.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/event.h"
#include "field/event_commands.h"
#include "field/event_inline.h"
#include "field/field.h"
#include "field/field_effect.h"
#include "field/pushed_actor.h"
#include "field/scene_tables.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "scene/actor_orbit.h"
#include "scene/catch_nut.h"
#include "scene/field_effect_spawn.h"
#include "scene/import.h"
#include "sound/sound.h"
#include "system/heap_slots.h"
#include "system/main_routine_work.h"
#include "system/system.h"

extern u16 gCrossboneIsle_DoorOpens[];
extern struct SceneEntrance gCrossboneIsle_IsleEntrances[];
extern struct SceneEntrance gCrossboneIsle_CaveEntranceEntrances[];
extern struct SceneEntrance gCrossboneIsle_FloatingLogsEntrances[];
extern struct SceneEntrance gCrossboneIsle_PierEntrances[];
extern struct SceneEntrance gCrossboneIsle_PirateShipEntrances[];
extern u32 gCrossboneIsle_Exits[];
extern struct ActorPlacement gCrossboneIsle_NoActorPlacements[];
extern struct ActorPlacement gCrossboneIsle_IsleActorPlacements[];
extern struct ActorPlacement gCrossboneIsle_FloatingLogsActorPlacements[];
extern struct ActorPlacement gCrossboneIsle_FloorActorPlacements[];
extern struct Trigger gCrossboneIsle_FloorTriggers[];
extern struct Trigger gCrossboneIsle_CaveEntranceTriggers[];
extern struct Trigger gCrossboneIsle_IsleTriggers[];
extern struct Trigger gCrossboneIsle_FloatingLogsTriggers[];
extern struct Trigger gCrossboneIsle_PierTriggers[];
extern struct Trigger gCrossboneIsle_PirateShipTriggers[];

/// Crossbone Isle's placed actors, numbered in the order of
/// gCrossboneIsle_IsleActorPlacements.
enum CrossboneIsleActorId {
  /// The Nut up in the tree by the waterfall, until Catch takes it
  /// (FLAG_CROSSBONE_ISLE_NUT_CAUGHT).
  ACTOR_ISLE_NUT = ACTOR_FIRST_PLACED,
  /// The two logs lying on the grass below the cliffs, which Isaac pushes by
  /// walking into them (CrossboneIsle_PushIsleLog): the west one at the foot
  /// of the cliff the vines climb, the east one in front of the chest with
  /// the Mint.
  ACTOR_FIRST_ISLE_LOG,
  /// The four sprouts Growth grows into vines, one flag each from
  /// FLAG_CROSSBONE_ISLE_FIRST_PLANT_GROWN. The second, third and fourth stand
  /// one above the other on the west cliff: at its foot, on its first ledge
  /// and on the ledge above. The first is placed at the map's origin, off
  /// stage (CrossboneIsle_UnusedGrowFirstPlant).
  ACTOR_FIRST_PLANT = ACTOR_FIRST_ISLE_LOG + 2,
  /// The clumps of ivy Whirlwind blows away, one flag each from
  /// FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY: the first over the cave's mouth
  /// at the foot of the cliff, the third on the top ledge beside the highest
  /// vine. The second is placed at the map's origin, off stage
  /// (CrossboneIsle_UnusedBlowAwaySecondIvy), and the fourth, which its
  /// trigger and CrossboneIsle_RestoreIvy name, is not placed at all.
  ACTOR_FIRST_IVY = ACTOR_FIRST_PLANT + 4
};

/// Growth has grown Crossbone Isle's first sprout, until the party leaves the
/// island. Four scene temp flags run from this one, one a sprout.
enum { FLAG_CROSSBONE_ISLE_FIRST_PLANT_GROWN = FLAG_SCENE_TEMP_40 };

/// Crossbone Isle's cells beside its two logs, where walking into a log pushes
/// it (CrossboneIsle_PushIsleLog).
enum {
  /// Beside the east log, in front of the chest with the Mint: cells (23, 22),
  /// (20, 26), (21, 26) and (23, 26).
  CELL_BEHAVIOR_CROSSBONE_ISLE_BESIDE_EAST_LOG = 10,
  /// Beside the west log at the foot of the cliff: cells (8, 23) to (10, 23),
  /// (7, 26), (8, 26) and (10, 26).
  CELL_BEHAVIOR_CROSSBONE_ISLE_BESIDE_WEST_LOG = 20
};

/// The twelve floating logs of the ninth floor's side room
/// (SceneId_CrossboneIsleFloatingLogs), lettered A to L in the order of
/// gCrossboneIsle_FloatingLogsActorPlacements. Each is given by the place it
/// starts in on every visit, with its middle cell.
///
/// A log lying west-east rolls north and south, and one lying north-south
/// rolls east and west. Where the ways of two logs cross, one rolled into the
/// other's way stops it, so the order the logs are rolled in decides where
/// each can go.
enum CrossboneIsleFloatingLogsActorId {
  /// The east pool's south-west log, lying west-east at (25, 19). It rolls
  /// north up the pool's west side, below the alcove with the Sleep Bomb.
  ACTOR_LOG_A = ACTOR_FIRST_PLACED,
  /// The east one of the west pool's three south logs, lying west-east at
  /// (10, 19).
  ACTOR_LOG_B,
  /// The east pool's log south-east of the island, lying west-east at
  /// (32, 18). It rolls north up the middle of the pool's east half.
  ACTOR_LOG_C,
  /// The east pool's south-east log, lying north-south along the east shore at
  /// (36, 17). It rolls west to the island.
  ACTOR_LOG_D,
  /// The east pool's middle log, lying north-south east of the island at
  /// (34, 12). It rolls west to the pool's west side.
  ACTOR_LOG_E,
  /// The east pool's north-east log, lying north-south at (36, 8). It rolls
  /// west along the pool's north edge.
  ACTOR_LOG_F,
  /// The middle one of the west pool's three side logs, lying north-south
  /// along its east shore at (13, 13).
  ACTOR_LOG_G,
  /// The east pool's north-west log, lying north-south at (24, 8). It rolls
  /// east along the pool's north edge.
  ACTOR_LOG_H,
  /// The north one of the west pool's three side logs, lying north-south
  /// along its east shore at (13, 10).
  ACTOR_LOG_I,
  /// The west one of the west pool's three south logs, lying west-east at
  /// (4, 19), in the bay at the pool's south-west.
  ACTOR_LOG_J,
  /// The middle one of the west pool's three south logs, lying west-east at
  /// (7, 19).
  ACTOR_LOG_K,
  /// The south one of the west pool's three side logs, lying north-south
  /// along its east shore at (13, 16).
  ACTOR_LOG_L
};

/// The line each floating log rides along that another can lie across: the
/// column of a west-east log's middle cell, which rides north and south, and
/// the row of a north-south log's, which rides east and west.
enum {
  LOG_B_COLUMN = 10,
  LOG_C_COLUMN = 32,
  LOG_J_COLUMN = 4,
  LOG_K_COLUMN = 7,
  LOG_G_ROW = 13,
  LOG_I_ROW = 10
};

/// True when a log whose middle cell is at `position` lies across `line`, in
/// the way of the three-cell log riding along it.
#define LIES_ACROSS(position, line)                                            \
  ((position) >= (line) - 1 && (position) <= (line) + 1)

/// The cell behaviour under each floating log: stepping on it sets the log
/// bobbing, walking on along it rolls it, and walking off its side hops Isaac
/// over a cell of water (gCrossboneIsle_FloatingLogsTriggers). The cells are
/// stored in rows 30 to 32 of the room's map.txt and laid under the logs on
/// every visit (CrossboneIsle_PlaceLogs).
enum {
  CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_A = 21,
  CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_C = 22,
  CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_D = 23,
  CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_E = 24,
  CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_F = 25,
  CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_H = 26,
  CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_J = 31,
  CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_K = 32,
  CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_B = 33,
  CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_L = 34,
  CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_G = 35,
  CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_I = 36
};

/// The floating-logs room's shore cells where walking off the edge hops Isaac
/// over a cell of water onto a log beyond
/// (gCrossboneIsle_FloatingLogsTriggers).
enum {
  /// The east shores of both pools: cells (14, 8) to (14, 19) beside the west
  /// pool's side logs, and (37, 7) to (37, 18) beside the east pool's.
  CELL_BEHAVIOR_CROSSBONE_ISLE_SHORE_HOP_WEST = 40,
  /// The west pool's west shore: the ledge at (5, 9) to (5, 14) and the bay's
  /// shore at (2, 15) to (2, 19).
  CELL_BEHAVIOR_CROSSBONE_ISLE_SHORE_HOP_EAST = 41,
  /// The west pool's north shore, (6, 8) to (8, 8).
  CELL_BEHAVIOR_CROSSBONE_ISLE_SHORE_HOP_SOUTH = 42,
  /// The west pool's south shore, (3, 20) to (13, 20).
  CELL_BEHAVIOR_CROSSBONE_ISLE_SHORE_HOP_NORTH = 43
};

/// The cell on a floor's west ledge just west of the stone pillar, (38, 10),
/// from which walking east pushes it (CrossboneIsle_PushPillar).
enum { CELL_BEHAVIOR_CROSSBONE_ISLE_WEST_OF_PILLAR = 20 };

/// The nine floors' one placed actor (gCrossboneIsle_FloorActorPlacements).
enum CrossboneIsleFloorActorId {
  /// The stone pillar on the west ledge, at the edge of the gap between the
  /// ledges, that drops into the gap when pushed east.
  ACTOR_PILLAR = ACTOR_FIRST_PLACED
};

/// The nine floors' entrances, as the code the floors share names them.
///
/// Every floor numbers them alike. scene_exits.h names each floor's own
/// (CROSSBONE_ISLE_FIRST_FLOOR_FROM_CROSSBONE_ISLE_CAVE_ENTRANCE to
/// CROSSBONE_ISLE_FIRST_FLOOR_FROM_CROSSBONE_ISLE_STEPPING_STONES_WEST in the
/// first).
enum {
  /// At the east end, from the room before (the cave's mouth,
  /// CrossboneIsleCaveEntrance, for the first).
  FLOOR_FROM_PREVIOUS_ROOM = 1,
  /// At the west end, from the room after (CrossboneIslePier for the last).
  FLOOR_FROM_NEXT_ROOM = 2,
  FLOOR_FROM_SIDE_ROOM_EAST = 3, ///< out of the side room by the east door
  FLOOR_FROM_SIDE_ROOM_WEST = 4  ///< out of the side room by the west door
};

/// The nine floors' exits, numbered as their entrances.
enum {
  FLOOR_TO_PREVIOUS_ROOM = 1,
  FLOOR_TO_NEXT_ROOM = 2,
  FLOOR_TO_SIDE_ROOM_EAST = 3, ///< the east door (CrossboneIsle_OpenEastDoor)
  FLOOR_TO_SIDE_ROOM_WEST = 4  ///< the west door (CrossboneIsle_OpenWestDoor)
};

/// The entrance a battle returns the party to.
///
/// On a floor that is after the monsters behind its east door
/// (CrossboneIsle_OpenEastDoor). On the Pirate Ship it is after Deadbeard
/// (CrossboneIsle_FightDeadbeard).
enum { ENTRANCE_AFTER_BATTLE = 5 };

/// The Pirate Ship's step cells.
enum {
  /// The top of the west stairs up to the raised foredeck, cell (11, 34),
  /// where the battle with Deadbeard starts (CrossboneIsle_FightDeadbeard).
  CELL_BEHAVIOR_PIRATE_SHIP_TOP_OF_WEST_STAIRS = 10,
  /// Cell (15, 40), beside the Mimic's chest at the foot of the ladder, from
  /// which walking south hops Isaac across the hole in the deck
  /// (CrossboneIsle_HopSouth). Scene_Initialize lays it from the stored cell
  /// at (0, 2) only while the Mimic is unbeaten.
  CELL_BEHAVIOR_PIRATE_SHIP_BESIDE_MIMIC = 20
};

/// The treasure spots of the chests that come and go, which their examine
/// triggers answer to.
enum {
  /// The first floor's chest, a Mimic until it is beaten.
  TREASURE_SPOT_CROSSBONE_ISLE_FIRST_FLOOR_MIMIC = TREASURE_SPOT_FIRST_ID,
  /// The Pirate Ship's chest with the Demon Mail, there once Deadbeard is
  /// beaten.
  TREASURE_SPOT_PIRATE_SHIP_DEMON_MAIL = TREASURE_SPOT_FIRST_ID,
  /// The Pirate Ship's other chest, a Mimic until it is beaten.
  TREASURE_SPOT_PIRATE_SHIP_MIMIC = TREASURE_SPOT_FIRST_ID + 1
};

/// The treasure spots of the island and the floating-logs room that stay,
/// named for what each holds.
enum {
  TREASURE_SPOT_CROSSBONE_ISLE_COIN = TREASURE_SPOT_FIRST_ID,
  TREASURE_SPOT_CROSSBONE_ISLE_MINT,
  TREASURE_SPOT_CROSSBONE_ISLE_FLOATING_LOGS_999_COINS = TREASURE_SPOT_FIRST_ID,
  TREASURE_SPOT_CROSSBONE_ISLE_FLOATING_LOGS_WATER_OF_LIFE,
  TREASURE_SPOT_CROSSBONE_ISLE_FLOATING_LOGS_MURAMASA,
  TREASURE_SPOT_CROSSBONE_ISLE_FLOATING_LOGS_SLEEP_BOMB
};

static void CrossboneIsle_RestorePillar(void);
static void CrossboneIsle_OpenEastDoorAfterBattle(void);
static void CrossboneIsle_RestoreIvy(void);
static void CrossboneIsle_PlaceLogs(void);

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

const u32 *Scene_GetExits(void) { return gCrossboneIsle_Exits; }

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
    return gCrossboneIsle_FloorActorPlacements;
  }
  return gCrossboneIsle_NoActorPlacements;
}

#include "shared/field_effect_advance.c"

/// Puffs a ring of dust out from a floor's pillar as it lands in the gap.
static void CrossboneIsle_SpawnPillarMovedBurst(s32 actorId) {
  struct Actor *actor;
  struct Vector3 velocity;
  struct EffectOptions options;
  u32 angleStep;
  s32 angle;
  s32 velocityX;
  FIXED velocityZ;

  actor = Actor_Get(actorId);
  options.update = FieldEffect_AdvanceMotion;
  for (angleStep = 0; angleStep <= TRIG_TURN / FACING_STEP; angleStep += 2) {
    angle = angleStep * FACING_STEP;
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

/// Climbing the west stairs onto the Pirate Ship's raised foredeck, to cell
/// (11, 34), sets off the battle with Deadbeard.
///
/// He is the sixth group of ENCOUNTER_ZONE_CROSSBONE_ISLE_LAST_GUARDS, and the
/// battle brings the party back to ENTRANCE_AFTER_BATTLE on the foredeck,
/// where Scene_Initialize sets FLAG_CROSSBONE_ISLE_DEADBEARD_DEFEATED and puts
/// the chest with the Demon Mail before them. The trigger row stops answering
/// once that flag is set.
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
  return gCrossboneIsle_FloorTriggers;
}

/// Catch on the Nut up in the tree by the waterfall on Crossbone Isle: the
/// hand of Catch pulls it down to Isaac and "Isaac got a Nut."
///
/// Scene_CatchNut gives the Nut and sets FLAG_CROSSBONE_ISLE_NUT_CAUGHT, which
/// keeps it from being placed again.
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
  // The floating logs' room only places its logs.
  sceneId = gMainRoutineWork.next;
  if (sceneId == SCENE_CROSSBONE_ISLE_FLOATING_LOGS) {
    CrossboneIsle_PlaceLogs();
    return 0;
  }
  if (sceneId == SCENE_PIRATE_SHIP) {
    if (GameFlag_Get(FLAG_PIRATE_SHIP_MIMIC_BEATEN) == 0) {
      MapGrid_CopyCellAttributes(0, 3, 1, 1, 13, 40);
      MapGrid_CopyCellAttributes(0, 2, 1, 1, 15, 40);
      TreasureSpot_SetPosition(TREASURE_SPOT_PIRATE_SHIP_MIMIC, toFIXED(216),
                               toFIXED(648));
    }
    if (gMainRoutineWork.next == sceneId) {
      if (gMainRoutineWork.entrance != ENTRANCE_AFTER_BATTLE) {
        if (GameFlag_Get(FLAG_CROSSBONE_ISLE_DEADBEARD_DEFEATED) == 0) {
          return 0;
        }
      }
      GameFlag_Set(FLAG_CROSSBONE_ISLE_DEADBEARD_DEFEATED);
      MapGrid_CopyCellAttributes(0, 1, 1, 1, 13, 30);
      TreasureSpot_SetPosition(TREASURE_SPOT_PIRATE_SHIP_DEMON_MAIL,
                               toFIXED(216), toFIXED(488));
      return 0;
    }
  }
  current = gMainRoutineWork.next;
  if (current == SCENE_CROSSBONE_ISLE) {
    CrossboneIsle_RestoreIvy();
    Actor_Get(ACTOR_ISLE_NUT)->targetX = toFIXED(129);
    Scene_RedrawActorFootprint(ACTOR_FIRST_ISLE_LOG);
    Scene_RedrawActorFootprint(ACTOR_FIRST_ISLE_LOG + 1);
    if (GameFlag_Get(FLAG_CROSSBONE_ISLE_FIRST_PLANT_GROWN) != 0) {
      actor = Actor_Get(ACTOR_FIRST_PLANT);
      if (actor != NULL) {
        actor->collisionFlags = 0;
        Actor_SelectAnimation(actor, SPROUT_ANIMATION_GROWN);
        Actor_SetSpriteFlags(actor, 0);
      }
      Field_SetCellBehaviorAt(0, 304, 368, CELL_BEHAVIOR_CLIMB);
    }
    if (GameFlag_Get(FLAG_CROSSBONE_ISLE_FIRST_PLANT_GROWN + 1) != 0) {
      actor = Actor_Get(ACTOR_FIRST_PLANT + 1);
      if (actor != NULL) {
        actor->collisionFlags = 0;
        Actor_SelectAnimation(actor, SPROUT_ANIMATION_GROWN);
        Actor_SetSpriteFlags(actor, 0);
      }
      Field_SetCellBehaviorAt(0, 80, 368, CELL_BEHAVIOR_CLIMB);
    }
    if (GameFlag_Get(FLAG_CROSSBONE_ISLE_FIRST_PLANT_GROWN + 2) != 0) {
      actor = Actor_Get(ACTOR_FIRST_PLANT + 2);
      if (actor != NULL) {
        actor->collisionFlags = 0;
        Actor_SelectAnimation(actor, SPROUT_ANIMATION_GROWN);
        Actor_SetSpriteFlags(actor, 0);
      }
      Field_SetCellBehaviorAt(0, 96, 336, CELL_BEHAVIOR_CLIMB);
    }
    if (GameFlag_Get(FLAG_CROSSBONE_ISLE_FIRST_PLANT_GROWN + 3) != 0) {
      actor = Actor_Get(ACTOR_FIRST_PLANT + 3);
      if (actor != NULL) {
        actor->collisionFlags = 0;
        Actor_SelectAnimation(actor, SPROUT_ANIMATION_GROWN);
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
  // The nine floors share their door battle, pillar and return entrance.
  first = SCENE_CROSSBONE_ISLE_FIRST_FLOOR;
  if (current == first) {
    if (GameFlag_Get(FLAG_CROSSBONE_ISLE_FIRST_FLOOR_MIMIC_BEATEN) == 0) {
      MapGrid_CopyCellAttributes(0, 0, 1, 1, 37, 10);
      TreasureSpot_SetPosition(TREASURE_SPOT_CROSSBONE_ISLE_FIRST_FLOOR_MIMIC,
                               toFIXED(600), toFIXED(168));
    }
  }
  current = gMainRoutineWork.next;
  if (current >= first) {
    if (current <= SCENE_CROSSBONE_ISLE_NINTH_FLOOR) {
      CrossboneIsle_RestorePillar();
      if (gMainRoutineWork.entrance == ENTRANCE_AFTER_BATTLE) {
        CrossboneIsle_OpenEastDoorAfterBattle();
      }
    }
  }
  return 0;
}

/// On a floor of Crossbone Isle Cave whose pillar has already been pushed into
/// the gap between the ledges, it stands there again, its top a step across
/// (cell (40, 10)).
static void CrossboneIsle_RestorePillar(void) {
  struct Actor *actor;
  s32 cleared;

  actor = Actor_Get(ACTOR_PILLAR);
  cleared = GameFlag_Get(
      (gMainRoutineWork.next + (FLAG_CROSSBONE_ISLE_FIRST_PASSAGE_OPENED -
                                SCENE_CROSSBONE_ISLE_FIRST_FLOOR)));
  if (cleared != 0) {
    Actor_SetPosition(ACTOR_PILLAR, 650, 168);
    actor->y.fixed = -toFIXED(32);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_PILLAR), 0);
    Actor_SetSpritePriority(ACTOR_PILLAR, 3);
    actor->motionFlags = 0;
    actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    MapGrid_CopyCellAttributes(42, 10, 1, 1, 40, 10);
  } else {
    Actor_Get(ACTOR_PILLAR)->motionFlags = 0;
  }
}

/// Move on the stone pillar on a floor's west ledge, and every push of it: once
/// it reaches the gap between the ledges (cell (40, 10)), it drops into the
/// gap in a ring of dust, and its top stays there as a step Isaac hops across.
///
/// Anywhere else nothing more happens. The pillar's cell takes the jump-across
/// cell at (42, 10) (CELL_BEHAVIOR_JUMP_FORWARD), and one flag per floor from
/// FLAG_CROSSBONE_ISLE_FIRST_PASSAGE_OPENED keeps it there. In every edition
/// but the Japanese, a pillar already in the gap does not drop again.
static void CrossboneIsle_MovePillar(void) {
  struct Actor *pillar;
  s32 column;

  pillar = Actor_Get(ACTOR_PILLAR);
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
  CrossboneIsle_SpawnPillarMovedBurst(ACTOR_PILLAR);
  Sound_Play(SE_WHOOSH);
  Event_Wait(40);

  Actor_SetSpriteFlags(Actor_Get(ACTOR_PILLAR), 0);
  Actor_SetSpritePriority(ACTOR_PILLAR, 3);

  pillar->motionFlags = 0;
  pillar->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;

  MapGrid_CopyCellAttributes(42, 10, 1, 1, column, 10);

  GameFlag_Set(gMainRoutineWork.next +
               (FLAG_CROSSBONE_ISLE_FIRST_PASSAGE_OPENED -
                SCENE_CROSSBONE_ISLE_FIRST_FLOOR));
}

/// Walking east into the stone pillar on a floor's west ledge, from cell
/// (38, 10), pushes it one cell east, over the edge of the gap.
///
/// CrossboneIsle_MovePillar then drops it into the gap.
static void CrossboneIsle_PushPillar(void) {
  Event_Start();
  PushedActor_AdvancePair();
  Event_End();
  CrossboneIsle_MovePillar();
}

/// Walking north into the door on a floor's west ledge (cell (37, 9)) opens it
/// with a rumble, and the party goes through into the floor's side room.
static void CrossboneIsle_OpenWestDoor(void) {
  MapGrid_PlayMapEdit(gCrossboneIsle_DoorOpens, 37, 7);
  Sound_Play(SE_STONE_RUMBLE);
  Import_EventSetExitRequest(FLOOR_TO_SIDE_ROOM_WEST);
}

/// Walking north into the door on a floor's east ledge (cell (44, 9)): the
/// first time, the monsters behind it attack; after that it opens with a
/// rumble and the party goes through into the floor's side room.
///
/// The battle brings the party back to ENTRANCE_AFTER_BATTLE, in front of the
/// door (CrossboneIsle_OpenEastDoorAfterBattle).
static void CrossboneIsle_OpenEastDoor(void) {
  if (GameFlag_Get(gMainRoutineWork.next +
                   (FLAG_CROSSBONE_ISLE_FIRST_EAST_DOOR_MONSTERS_DEFEATED -
                    SCENE_CROSSBONE_ISLE_FIRST_FLOOR)) == 0) {
    Event_Start();
    Event_SetAfterBattleDestination(gMainRoutineWork.next,
                                    ENTRANCE_AFTER_BATTLE);
    Battle_SetType(BATTLE_TYPE_SCRIPTED);
    // The first four floors use ENCOUNTER_ZONE_CROSSBONE_ISLE_FIRST_GUARDS;
    // the next five use ENCOUNTER_ZONE_CROSSBONE_ISLE_LAST_GUARDS.
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
    MapGrid_PlayMapEdit(gCrossboneIsle_DoorOpens, 44, 7);
    Sound_Play(SE_STONE_RUMBLE);
    Import_EventSetExitRequest(FLOOR_TO_SIDE_ROOM_EAST);
  }
}

/// Back from the battle at a floor's east door, the door opens and Isaac walks
/// in through it to the side room.
///
/// It sets the floor's flag from
/// FLAG_CROSSBONE_ISLE_FIRST_EAST_DOOR_MONSTERS_DEFEATED, so the monsters do
/// not come again.
static void CrossboneIsle_OpenEastDoorAfterBattle(void) {
  Event_Start();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.4);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  GameFlag_Set((gMainRoutineWork.next +
                (FLAG_CROSSBONE_ISLE_FIRST_EAST_DOOR_MONSTERS_DEFEATED -
                 SCENE_CROSSBONE_ISLE_FIRST_FLOOR)));
  Event_Wait(30);
  MapGrid_AnimateCells(gCrossboneIsle_DoorOpens, 44, 7);
  Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 3, -16);
  Import_EventSetExitRequest(FLOOR_TO_SIDE_ROOM_EAST);
  Event_End();
}

/// Growth on one of Crossbone Isle's sprouts: it no longer stops the party,
/// its vine stands grown, and the cell at pixel (x, z) becomes the vine's
/// foot, climbed with Up or Down held (CELL_BEHAVIOR_CLIMB).
static inline void CrossboneIsle_GrowPlant(s32 actorId, s32 plant, s32 x,
                                           s32 z) {
  struct Actor *actor = Actor_Get(ACTOR_FIRST_PLANT + plant);

  if (actor != NULL) {
    actor->collisionFlags = 0;
  }
  Actor_SetSpriteFlags(Actor_Get(actorId), 0);
  MapGrid_SetCellBehaviorAt(0, INTtoFIXED(x), INTtoFIXED(z),
                            CELL_BEHAVIOR_CLIMB);
}

/// Unused: Growth on Crossbone Isle's first sprout would grow its vine and
/// make cell (19, 23) climbable.
///
/// The sprout is placed at the map's origin, out of anyone's reach, so its
/// trigger never runs. Scene_Initialize still restores its vine if its flag
/// were set.
static void CrossboneIsle_UnusedGrowFirstPlant(s32 actorId) {
  CrossboneIsle_GrowPlant(actorId, 0, 304, 368);
  GameFlag_Set(FLAG_CROSSBONE_ISLE_FIRST_PLANT_GROWN);
}

/// Growth on the sprout at the foot of Crossbone Isle's west cliff, cell
/// (5, 23), grows a vine up the cliff face to its first ledge.
///
/// The vine lasts until the party leaves the island.
static void CrossboneIsle_GrowSecondPlant(s32 actorId) {
  CrossboneIsle_GrowPlant(actorId, 1, 80, 368);
  GameFlag_Set(FLAG_CROSSBONE_ISLE_FIRST_PLANT_GROWN + 1);
}

/// Growth on the sprout on the first ledge of Crossbone Isle's west cliff,
/// cell (6, 21), grows a vine from there up to the next ledge.
///
/// The vine lasts until the party leaves the island.
static void CrossboneIsle_GrowThirdPlant(s32 actorId) {
  CrossboneIsle_GrowPlant(actorId, 2, 96, 336);
  GameFlag_Set(FLAG_CROSSBONE_ISLE_FIRST_PLANT_GROWN + 2);
}

/// Growth on the sprout on the upper ledge of Crossbone Isle's west cliff,
/// cell (9, 20), grows the last vine, up to the top of the cliff.
///
/// The vine's foot is made climbable twice: at (9, 20), and at (47, 20), the
/// same place in the copy of the cliff's top drawn over it, 38 columns over in
/// map.txt. The vine lasts until the party leaves the island.
static void CrossboneIsle_GrowFourthPlant(s32 actorId) {
  FIXED eastX = toFIXED(752);
  FIXED eastZ = toFIXED(320);

  CrossboneIsle_GrowPlant(actorId, 3, 144, 320);
  MapGrid_SetCellBehaviorAt(0, eastX, eastZ, CELL_BEHAVIOR_CLIMB);
  GameFlag_Set(FLAG_CROSSBONE_ISLE_FIRST_PLANT_GROWN + 3);
}

/// Whirlwind on the clump of ivy at the foot of Crossbone Isle's west cliff
/// blows it away for good, uncovering the mouth of the cave behind it at
/// cell (8, 21).
///
/// The mouth's cell takes the stored cell at (0, 0), the exit into
/// CrossboneIsleCaveEntrance, and FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY
/// keeps it open (CrossboneIsle_RestoreIvy).
static void CrossboneIsle_BlowAwayFirstIvy(void) {
  GameFlag_Set(FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY);
  MapGrid_CopyCellAttributes(0, 0, 1, 1, 8, 21);
}

/// Unused: Whirlwind on Crossbone Isle's second clump of ivy would blow it away
/// for good.
///
/// The ivy is placed at the map's origin, out of anyone's reach, so its
/// trigger never runs.
static void CrossboneIsle_UnusedBlowAwaySecondIvy(void) {
  GameFlag_Set(FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY + 1);
}

/// Whirlwind on the clump of ivy on the top ledge of Crossbone Isle's west
/// cliff, beside the last vine, blows it away for good; nothing is hidden
/// behind it.
static void CrossboneIsle_BlowAwayThirdIvy(void) {
  GameFlag_Set(FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY + 2);
}

/// Unused: Whirlwind on a fourth clump of ivy on Crossbone Isle would blow it
/// away for good.
///
/// No fourth clump is placed, so its trigger never runs.
static void CrossboneIsle_UnusedBlowAwayFourthIvy(void) {
  GameFlag_Set(FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY + 3);
}

/// On Crossbone Isle, ivy Whirlwind has blown away stays gone, and the cave
/// mouth behind the first stays open; the others are stretched to stand tall.
static void CrossboneIsle_RestoreIvy(void) {
  struct Actor *actor;

  // The cave mouth CrossboneIsle_BlowAwayFirstIvy opens, opened again on
  // arrival.
  if (GameFlag_Get(FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY) != 0) {
    MapGrid_CopyCellAttributes(0, 0, 1, 1, 8, 21);
    Actor_SetPosition(ACTOR_FIRST_IVY, 968, 968);
  } else {
    Actor_Get(ACTOR_FIRST_IVY)->scaleY = 8 * FIXED_ONE / 5;
  }
  if (GameFlag_Get(FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY + 1) != 0) {
    Actor_SetPosition(ACTOR_FIRST_IVY + 1, 968, 968);
  } else {
    Actor_Get(ACTOR_FIRST_IVY + 1)->scaleY = 8 * FIXED_ONE / 5;
  }
  if (GameFlag_Get(FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY + 2) != 0) {
    Actor_SetPosition(ACTOR_FIRST_IVY + 2, 968, 968);
  } else {
    Actor_Get(ACTOR_FIRST_IVY + 2)->scaleY = 8 * FIXED_ONE / 5;
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
      actor->scaleY = 8 * FIXED_ONE / 5;
  }
}

/// Walking into one of the two logs on Crossbone Isle's grass pushes it along,
/// if the ground beyond is clear: the west one at the foot of the cliff, or the
/// east one, which lies in front of the chest with the Mint.
static void CrossboneIsle_PushIsleLog(void) {
  struct PushedActorProbe probe;

  Event_Start();
  if (PushedActor_FindClearPosition(&probe) != 0) {
    Actor_MoveAndRedraw(probe);
  }
  Event_End();
}

/// Rolls log actorId one leg, dx and dz pixels, with Isaac walking backwards on
/// top of it to the cell it stops at. The log rolls with a sound and stops with
/// another.
static void CrossboneIsle_RideLog(s32 actorId, s32 dx, s32 dz) {
  struct Actor *leader;
  struct Actor *log;
  FIXED x;
  FIXED z;

  leader = Actor_Get(gMainRoutineWork.leaderActorId);
  log = Actor_Get(actorId);
  Event_Start();
  {
    x = ((leader->x.fixed + INTtoFIXED(dx)) & FIELD_CELL_MASK) + toFIXED(8);
    z = ((leader->z.fixed + INTtoFIXED(dz)) & FIELD_CELL_MASK) + toFIXED(8);

    leader->speedLimit = toFIXED(1);
    leader->acceleration = toFIXED(1) / 2;
    FieldActor_SetMoveTargetFX(leader, x, leader->y.fixed, z);
  }
  Actor_SelectAnimation(leader, ANIM_LEADER_WALK_BACKWARDS);
  {
    x = ((log->x.fixed + INTtoFIXED(dx)) & FIELD_CELL_MASK) + toFIXED(8);
    z = ((log->z.fixed + INTtoFIXED(dz)) & FIELD_CELL_MASK) + toFIXED(8);

    log->speedLimit = toFIXED(1);
    log->acceleration = toFIXED(1) / 2;
    FieldActor_SetMoveTargetFX(log, x, log->y.fixed, z);
  }
  if (dx < 0 || dz < 0) {
    Actor_SelectAnimation(log, FLOATING_LOG_ANIMATION_ROLL_WEST_OR_NORTH);
  } else {
    Actor_SelectAnimation(log, FLOATING_LOG_ANIMATION_ROLL_EAST_OR_SOUTH);
  }
  Sound_Play(SE_LOG_PUSH);
  Actor_WaitForMove(leader);
  Sound_Play(SE_STOP_ACTION_SE);
  Event_End();
}

/// Lays the cells of a log lying west-east under it, so that it can be walked
/// on: the three cells it covers take the stored cells at
/// (sourceColumn, sourceRow). Its cell behaviour is passed and not used.
static void CrossboneIsle_PlaceWestEastLog(s32 actorId, s32 sourceColumn,
                                           s32 sourceRow, s32 cellBehavior) {
  struct Actor *actor = Actor_Get(actorId);

  if (actor != NULL) {

    Actor_SetSpritePriority(actorId, 3);
    actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    {
      s32 row = FIELD_CELL(actor->z.fixed);
      s32 column = FIELD_CELL(actor->x.fixed) - 1;

      MapGrid_CopyCellAttributes(sourceColumn, sourceRow, 3, 1, column, row);
    }
  }
}

/// As CrossboneIsle_PlaceWestEastLog, for a log lying north-south.
static void CrossboneIsle_PlaceNorthSouthLog(s32 actorId, s32 sourceColumn,
                                             s32 sourceRow, s32 cellBehavior) {
  struct Actor *actor = Actor_Get(actorId);

  if (actor != NULL) {

    Actor_SetSpritePriority(actorId, 3);
    actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    {
      s32 column = FIELD_CELL(actor->x.fixed);
      s32 row = FIELD_CELL(actor->z.fixed) - 1;

      MapGrid_CopyCellAttributes(sourceColumn, sourceRow, 1, 3, column, row);
    }
  }
}

/// Starts a floating log bobbing under Isaac's step, from the beginning.
static void CrossboneIsle_SettleLog(int actorId) {
  Actor_SetAnimation(actorId, FLOATING_LOG_ANIMATION_FLOAT);
  Actor_SetAnimation(actorId, FLOATING_LOG_ANIMATION_SETTLE);
}

/// Lays the cells of the floating-logs room's twelve logs under them, in their
/// starting places.
static void CrossboneIsle_PlaceLogs(void) {
  CrossboneIsle_PlaceWestEastLog(ACTOR_LOG_A, 17, 30,
                                 CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_A);
  CrossboneIsle_PlaceWestEastLog(ACTOR_LOG_C, 17, 31,
                                 CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_C);
  CrossboneIsle_PlaceNorthSouthLog(ACTOR_LOG_D, 20, 30,
                                   CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_D);
  CrossboneIsle_PlaceNorthSouthLog(ACTOR_LOG_E, 21, 30,
                                   CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_E);
  CrossboneIsle_PlaceNorthSouthLog(ACTOR_LOG_F, 22, 30,
                                   CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_F);
  CrossboneIsle_PlaceNorthSouthLog(ACTOR_LOG_H, 23, 30,
                                   CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_H);
  CrossboneIsle_PlaceWestEastLog(ACTOR_LOG_J, 0, 30,
                                 CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_J);
  CrossboneIsle_PlaceWestEastLog(ACTOR_LOG_K, 0, 31,
                                 CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_K);
  CrossboneIsle_PlaceWestEastLog(ACTOR_LOG_B, 0, 32,
                                 CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_B);
  CrossboneIsle_PlaceNorthSouthLog(ACTOR_LOG_L, 4, 30,
                                   CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_L);
  CrossboneIsle_PlaceNorthSouthLog(ACTOR_LOG_G, 5, 30,
                                   CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_G);
  CrossboneIsle_PlaceNorthSouthLog(ACTOR_LOG_I, 6, 30,
                                   CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_I);
}

/// Stepping onto the east pool's south-west log sets it bobbing in the water
/// under Isaac.
static void CrossboneIsle_StepOnLogA(void) {
  CrossboneIsle_SettleLog(ACTOR_LOG_A);
}

/// Stepping onto the east pool's log south-east of the island sets it bobbing
/// in the water under Isaac.
static void CrossboneIsle_StepOnLogC(void) {
  CrossboneIsle_SettleLog(ACTOR_LOG_C);
}

/// Stepping onto the east pool's south-east log, by the east shore, sets it
/// bobbing in the water under Isaac.
static void CrossboneIsle_StepOnLogD(void) {
  CrossboneIsle_SettleLog(ACTOR_LOG_D);
}

/// Stepping onto the east pool's middle log sets it bobbing in the water under
/// Isaac.
static void CrossboneIsle_StepOnLogE(void) {
  CrossboneIsle_SettleLog(ACTOR_LOG_E);
}

/// Stepping onto the east pool's north-east log sets it bobbing in the water
/// under Isaac.
static void CrossboneIsle_StepOnLogF(void) {
  CrossboneIsle_SettleLog(ACTOR_LOG_F);
}

/// Stepping onto the east pool's north-west log sets it bobbing in the water
/// under Isaac.
static void CrossboneIsle_StepOnLogH(void) {
  CrossboneIsle_SettleLog(ACTOR_LOG_H);
}

/// Stepping onto the west one of the west pool's three south logs sets it
/// bobbing in the water under Isaac.
static void CrossboneIsle_StepOnLogJ(void) {
  CrossboneIsle_SettleLog(ACTOR_LOG_J);
}

/// Stepping onto the middle one of the west pool's three south logs sets it
/// bobbing in the water under Isaac.
static void CrossboneIsle_StepOnLogK(void) {
  CrossboneIsle_SettleLog(ACTOR_LOG_K);
}

/// Stepping onto the east one of the west pool's three south logs sets it
/// bobbing in the water under Isaac.
static void CrossboneIsle_StepOnLogB(void) {
  CrossboneIsle_SettleLog(ACTOR_LOG_B);
}

/// Stepping onto the south one of the west pool's three side logs sets it
/// bobbing in the water under Isaac.
static void CrossboneIsle_StepOnLogL(void) {
  CrossboneIsle_SettleLog(ACTOR_LOG_L);
}

/// Stepping onto the middle one of the west pool's three side logs sets it
/// bobbing in the water under Isaac.
static void CrossboneIsle_StepOnLogG(void) {
  CrossboneIsle_SettleLog(ACTOR_LOG_G);
}

/// Stepping onto the north one of the west pool's three side logs sets it
/// bobbing in the water under Isaac.
static void CrossboneIsle_StepOnLogI(void) {
  CrossboneIsle_SettleLog(ACTOR_LOG_I);
}

/// Isaac springs and leaps two cells the way he faces, over a cell of water
/// from log to log or between a log and the shore, if he can land there, and
/// it returns 1 if he hopped. The Japanese edition aims at `requested`, the
/// others at the middle of the cell 32 pixels ahead.
static s32 CrossboneIsle_TryHop(struct Actor *actor,
                                struct FieldPosition *requested) {
  s32 savedMotionFlags = actor->motionFlags;
#if TBS_PATCH_1
  struct FieldPosition destination;

  destination.x = (actor->x.fixed & FIELD_CELL_MASK) + toFIXED(8);
  destination.y = actor->y.fixed;
  destination.z = (actor->z.fixed & FIELD_CELL_MASK) + toFIXED(8);
  {
    s32 direction = FACING_NEAREST_CARDINAL(actor->facing);

    Vector_AddPolarOffset(toFIXED(32), direction, &destination);
  }
  if (FieldActor_CheckMovementCollision(actor, &destination) == 0) {
#else
  if (FieldActor_CheckMovementCollision(actor, requested) == 0) {
#endif
    Event_Start();
    Actor_SelectAnimation(actor, ANIM_LEADER_SPRING);
    wait(6);
    Sound_Play(SE_JUMP);
    Actor_SelectAnimation(actor, ANIM_LEADER_LEAP);
    actor->speedLimit = toFIXED(3);
    actor->acceleration = toFIXED(2);
    actor->velocityY = toFIXED(4);
    actor->motionFlags &= ACTOR_JUMP_KEEPS_MOTION;
    Actor_SetSpriteFlags(actor, 0);
#if TBS_PATCH_1
    Actor_MoveToAndWait(ACTOR_PARTY_LEADER, FIXEDtoINT(destination.x),
                        FIXEDtoINT(destination.z));
#else
    {
      s16 *pixelCoordinates = (s16 *)requested;

      Actor_MoveToAndWait(ACTOR_PARTY_LEADER, pixelCoordinates[1],
                          pixelCoordinates[5]);
    }
#endif
    Actor_SelectAnimation(actor, ANIM_LEADER_SPRING);
    Actor_SetSpriteFlags(actor, SPRITE_FLAG_SHADOW);
    actor->motionFlags = savedMotionFlags;
    Event_End();
    return 1;
  }
  return 0;
}

/// Walking north off the side of a floating log, or off the west pool's south
/// shore, with a cell of water ahead, hops Isaac over it onto the log or shore
/// beyond, if there is one to land on.
static s32 CrossboneIsle_HopNorth(void) {
  struct Actor *actor = Actor_Get(ACTOR_PARTY_LEADER);
  struct FieldPosition destination;

  destination.x = actor->x.fixed;
  destination.y = actor->y.fixed;
  destination.z = actor->z.fixed + -toFIXED(32);
  return CrossboneIsle_TryHop(actor, &destination);
}

/// Walking south off the side of a floating log, or off the west pool's north
/// shore, with a cell of water ahead, hops Isaac over it onto the log or shore
/// beyond; on the Pirate Ship, walking south from beside the Mimic's chest
/// hops him across the hole in the deck.
static s32 CrossboneIsle_HopSouth(void) {
  struct Actor *actor = Actor_Get(ACTOR_PARTY_LEADER);
  struct FieldPosition destination;

  destination.x = actor->x.fixed;
  destination.y = actor->y.fixed;
  destination.z = actor->z.fixed + toFIXED(32);
  return CrossboneIsle_TryHop(actor, &destination);
}

/// Walking west off the side of a floating log, or off the east shore of
/// either pool, with a cell of water ahead, hops Isaac over it onto the log or
/// shore beyond, if there is one to land on.
static s32 CrossboneIsle_HopWest(void) {
  struct Actor *actor = Actor_Get(ACTOR_PARTY_LEADER);
  struct FieldPosition destination;

  destination.x = actor->x.fixed + -toFIXED(32);
  destination.y = actor->y.fixed;
  destination.z = actor->z.fixed;
  return CrossboneIsle_TryHop(actor, &destination);
}

/// Walking east off the side of a floating log, or off the west pool's west
/// shore, with a cell of water ahead, hops Isaac over it onto the log or shore
/// beyond, if there is one to land on.
static s32 CrossboneIsle_HopEast(void) {
  struct Actor *actor = Actor_Get(ACTOR_PARTY_LEADER);
  struct FieldPosition destination;

  destination.x = actor->x.fixed + toFIXED(32);
  destination.y = actor->y.fixed;
  destination.z = actor->z.fixed;
  return CrossboneIsle_TryHop(actor, &destination);
}

/// Moves the cells of a log lying west-east from row `row` to the row it now
/// lies on, and turns the ones it left back into water that cannot be walked
/// on (the stored wall cells at (0, 0)).
static inline void CrossboneIsle_MoveWestEastLogCells(s32 actorId, s32 column,
                                                      s32 row) {
  wait(2);
  {
    s32 destinationRow = FIELD_CELL(Actor_Get(actorId)->z.fixed);
    s32 footprintColumn = column - 1;

    MapGrid_CopyCellAttributes(footprintColumn, row, 3, 1, footprintColumn,
                               destinationRow);
  }
  MapGrid_CopyCellAttributes(0, 0, 3, 1, column - 1, row);
}

/// As CrossboneIsle_MoveWestEastLogCells, for a log lying north-south moved
/// from column `column`.
static inline void CrossboneIsle_MoveNorthSouthLogCells(s32 actorId, s32 column,
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

/// Walking north along the east pool's south-west log rolls it north up the
/// pool's west side, with Isaac on top, to the north edge below the alcove
/// with the Sleep Bomb; there, walking on north hops him ashore.
///
/// The log stops short of the north-west log while that one lies across its
/// way, as it does at first, and short of the middle log if that has been
/// rolled west across it.
static void CrossboneIsle_RideLogANorth(void) {
  s32 column = FIELD_CELL(Actor_Get(ACTOR_LOG_A)->x.fixed);
  s32 row = FIELD_CELL(Actor_Get(ACTOR_LOG_A)->z.fixed);
  s32 logEColumn = FIELD_CELL(Actor_Get(ACTOR_LOG_E)->x.fixed);
  s32 logHColumn = FIELD_CELL(Actor_Get(ACTOR_LOG_H)->x.fixed);

  if (row == 19) {
    if (logEColumn == 24) {
      CrossboneIsle_RideLog(ACTOR_LOG_A, 0, -80);
    } else if (logHColumn == 24) {
      CrossboneIsle_RideLog(ACTOR_LOG_A, 0, -112);
      CrossboneIsle_RideLog(ACTOR_LOG_A, 0, -32);
    } else {
      CrossboneIsle_RideLog(ACTOR_LOG_A, 0, -80);
      CrossboneIsle_RideLog(ACTOR_LOG_A, 0, -112);
    }
  } else if (row == 14) {
    if (logEColumn == 24) {
      return;
    }
    if (logHColumn == 24) {
      CrossboneIsle_RideLog(ACTOR_LOG_A, 0, -64);
    } else {
      CrossboneIsle_RideLog(ACTOR_LOG_A, 0, -112);
    }
  } else if (row == 10) {
    if (logHColumn == 24) {
      return;
    }
    CrossboneIsle_RideLog(ACTOR_LOG_A, 0, -48);
  } else {
    CrossboneIsle_HopNorth();
    return;
  }
  CrossboneIsle_MoveWestEastLogCells(ACTOR_LOG_A, column, row);
}

/// Walking south along the east pool's south-west log rolls it back down the
/// pool's west side, with Isaac on top, to the south shore.
///
/// It stops short of the middle log if that has been rolled west across its
/// way.
static void CrossboneIsle_RideLogASouth(void) {
  s32 column = FIELD_CELL(Actor_Get(ACTOR_LOG_A)->x.fixed);
  s32 row = FIELD_CELL(Actor_Get(ACTOR_LOG_A)->z.fixed);
  s32 logEColumn = FIELD_CELL(Actor_Get(ACTOR_LOG_E)->x.fixed);

  if (row == 7) {
    if (logEColumn == 24) {
      CrossboneIsle_RideLog(ACTOR_LOG_A, 0, 48);
    } else {
      CrossboneIsle_RideLog(ACTOR_LOG_A, 0, 80);
      CrossboneIsle_RideLog(ACTOR_LOG_A, 0, 112);
    }
  } else if (row == 10) {
    if (logEColumn == 24) {
      return;
    }
    CrossboneIsle_RideLog(ACTOR_LOG_A, 0, 144);
  } else if (row == 14) {
    CrossboneIsle_RideLog(ACTOR_LOG_A, 0, 80);
  } else {
    return;
  }
  CrossboneIsle_MoveWestEastLogCells(ACTOR_LOG_A, column, row);
}

/// Walking north along the east pool's log south-east of the island rolls it
/// north up the pool, with Isaac on top, to the north edge.
///
/// While the north-west or the north-east log lies across its way at the
/// north edge, it stops short of it.
static void CrossboneIsle_RideLogCNorth(void) {
  s32 column = FIELD_CELL(Actor_Get(ACTOR_LOG_C)->x.fixed);
  s32 row = FIELD_CELL(Actor_Get(ACTOR_LOG_C)->z.fixed);
  s32 logFColumn = FIELD_CELL(Actor_Get(ACTOR_LOG_F)->x.fixed);
  s32 logHColumn = FIELD_CELL(Actor_Get(ACTOR_LOG_H)->x.fixed);

  if (row == 18) {
    if (LIES_ACROSS(logHColumn, LOG_C_COLUMN)) {
      CrossboneIsle_RideLog(ACTOR_LOG_C, 0, -128);
    } else if (LIES_ACROSS(logFColumn, LOG_C_COLUMN)) {
      CrossboneIsle_RideLog(ACTOR_LOG_C, 0, -128);
    } else {
      CrossboneIsle_RideLog(ACTOR_LOG_C, 0, -112);
      CrossboneIsle_RideLog(ACTOR_LOG_C, 0, -64);
    }
  } else if (row == 10) {
    if (LIES_ACROSS(logHColumn, LOG_C_COLUMN)) {
      return;
    }
    if (LIES_ACROSS(logFColumn, LOG_C_COLUMN)) {
      return;
    }
    CrossboneIsle_RideLog(ACTOR_LOG_C, 0, -48);
  } else if (row == 7) {
    return;
  }
  CrossboneIsle_MoveWestEastLogCells(ACTOR_LOG_C, column, row);
}

/// Walking south along the east pool's log south-east of the island rolls it
/// back south, with Isaac on top, to where it started.
static void CrossboneIsle_RideLogCSouth(void) {
  s32 column = FIELD_CELL(Actor_Get(ACTOR_LOG_C)->x.fixed);
  s32 row = FIELD_CELL(Actor_Get(ACTOR_LOG_C)->z.fixed);

  if (row != 18) {
    if (row == 10) {
      CrossboneIsle_RideLog(ACTOR_LOG_C, 0, 128);
    } else {
      CrossboneIsle_RideLog(ACTOR_LOG_C, 0, 112);
      CrossboneIsle_RideLog(ACTOR_LOG_C, 0, 64);
    }
    CrossboneIsle_MoveWestEastLogCells(ACTOR_LOG_C, column, row);
  }
}

/// Walking west along the east pool's south-east log rolls it west from the
/// east shore, with Isaac on top, to the island.
///
/// While the log south-east of the island is in its starting place, it lies
/// across the way and stops this one short of it.
static void CrossboneIsle_RideLogDWest(void) {
  s32 column = FIELD_CELL(Actor_Get(ACTOR_LOG_D)->x.fixed);
  s32 row = FIELD_CELL(Actor_Get(ACTOR_LOG_D)->z.fixed);

  if (column != 30) {
    if (column == 34) {
      if (FIELD_CELL(Actor_Get(ACTOR_LOG_C)->z.fixed) == 18) {
        return;
      }
      CrossboneIsle_RideLog(ACTOR_LOG_D, -64, 0);
    } else if (column == 36) {
      if (FIELD_CELL(Actor_Get(ACTOR_LOG_C)->z.fixed) == 18) {
        CrossboneIsle_RideLog(ACTOR_LOG_D, -32, 0);
      } else {
        CrossboneIsle_RideLog(ACTOR_LOG_D, -96, 0);
      }
    }
    CrossboneIsle_MoveNorthSouthLogCells(ACTOR_LOG_D, column, row);
  }
}

/// Walking east along the east pool's south-east log rolls it back east, with
/// Isaac on top, to the east shore.
///
/// From the island it cannot move while the log south-east of the island is
/// back in its starting place.
static void CrossboneIsle_RideLogDEast(void) {
  s32 column = FIELD_CELL(Actor_Get(ACTOR_LOG_D)->x.fixed);
  s32 row = FIELD_CELL(Actor_Get(ACTOR_LOG_D)->z.fixed);

  if (column != 36) {
    if (column == 30) {
      if (FIELD_CELL(Actor_Get(ACTOR_LOG_C)->z.fixed) == 18) {
        return;
      }
      CrossboneIsle_RideLog(ACTOR_LOG_D, 96, 0);
    } else if (column == 34) {
      CrossboneIsle_RideLog(ACTOR_LOG_D, 32, 0);
    }
    CrossboneIsle_MoveNorthSouthLogCells(ACTOR_LOG_D, column, row);
  }
}

/// Walking west along the east pool's middle log rolls it west past the north
/// side of the island, with Isaac on top, to the pool's west side.
static void CrossboneIsle_RideLogEWest(void) {
  s32 column = FIELD_CELL(Actor_Get(ACTOR_LOG_E)->x.fixed);
  s32 row = FIELD_CELL(Actor_Get(ACTOR_LOG_E)->z.fixed);

  if (column == 36) {
    CrossboneIsle_RideLog(ACTOR_LOG_E, -96, 0);
    CrossboneIsle_RideLog(ACTOR_LOG_E, -96, 0);
  } else if (column == 34) {
    CrossboneIsle_RideLog(ACTOR_LOG_E, -96, 0);
    CrossboneIsle_RideLog(ACTOR_LOG_E, -64, 0);
  } else if (column == 24) {
    return;
  }
  CrossboneIsle_MoveNorthSouthLogCells(ACTOR_LOG_E, column, row);
}

/// Walking east along the east pool's middle log rolls it east, with Isaac on
/// top, to the pool's east shore.
static void CrossboneIsle_RideLogEEast(void) {
  s32 column = FIELD_CELL(Actor_Get(ACTOR_LOG_E)->x.fixed);
  s32 row = FIELD_CELL(Actor_Get(ACTOR_LOG_E)->z.fixed);

  if (column == 24) {
    CrossboneIsle_RideLog(ACTOR_LOG_E, 96, 0);
    CrossboneIsle_RideLog(ACTOR_LOG_E, 96, 0);
  } else if (column == 34) {
    CrossboneIsle_RideLog(ACTOR_LOG_E, 32, 0);
  } else if (column == 36) {
    return;
  }
  CrossboneIsle_MoveNorthSouthLogCells(ACTOR_LOG_E, column, row);
}

/// Walking west along the east pool's north-east log rolls it west along the
/// pool's north edge, with Isaac on top, to the north-west log.
///
/// It stops sooner where the north-west log has been rolled east, or the log
/// from south-east of the island has been rolled up to the north edge.
static void CrossboneIsle_RideLogFWest(void) {
  s32 column = FIELD_CELL(Actor_Get(ACTOR_LOG_F)->x.fixed);
  s32 row = FIELD_CELL(Actor_Get(ACTOR_LOG_F)->z.fixed);
  s32 logCRow = FIELD_CELL(Actor_Get(ACTOR_LOG_C)->z.fixed);
  s32 logHColumn = FIELD_CELL(Actor_Get(ACTOR_LOG_H)->x.fixed);

  if (column == 36) {
    if (logHColumn == 34) {
      CrossboneIsle_RideLog(ACTOR_LOG_F, -16, 0);
    } else if (logCRow == 7) {
      CrossboneIsle_RideLog(ACTOR_LOG_F, -32, 0);
    } else if (logHColumn == 30) {
      CrossboneIsle_RideLog(ACTOR_LOG_F, -80, 0);
    } else {
      CrossboneIsle_RideLog(ACTOR_LOG_F, -96, 0);
      CrossboneIsle_RideLog(ACTOR_LOG_F, -80, 0);
    }
  } else if (column == 35) {
    if (logHColumn == 34) {
      return;
    }
    if (logCRow == 7) {
      CrossboneIsle_RideLog(ACTOR_LOG_F, -16, 0);
    } else if (logHColumn == 30) {
      CrossboneIsle_RideLog(ACTOR_LOG_F, -64, 0);
    } else {
      CrossboneIsle_RideLog(ACTOR_LOG_F, -80, 0);
      CrossboneIsle_RideLog(ACTOR_LOG_F, -80, 0);
    }
  } else if (column == 34) {
    if (logCRow == 7) {
      return;
    }
    if (logHColumn == 30) {
      CrossboneIsle_RideLog(ACTOR_LOG_F, -48, 0);
    } else {
      CrossboneIsle_RideLog(ACTOR_LOG_F, -144, 0);
    }
  } else if (column == 31) {
    if (logHColumn == 30) {
      return;
    }
    CrossboneIsle_RideLog(ACTOR_LOG_F, -96, 0);
  } else if (column == 25) {
    return;
  }
  CrossboneIsle_MoveNorthSouthLogCells(ACTOR_LOG_F, column, row);
}

/// Walking east along the east pool's north-east log rolls it back east along
/// the north edge, with Isaac on top, to the north-east corner.
static void CrossboneIsle_RideLogFEast(void) {
  s32 column = FIELD_CELL(Actor_Get(ACTOR_LOG_F)->x.fixed);
  s32 row = FIELD_CELL(Actor_Get(ACTOR_LOG_F)->z.fixed);

  // DEAD: log H looked up and dropped; the ROM keeps its call.
  Actor_Get(ACTOR_LOG_H);
  if (column == 25) {
    CrossboneIsle_RideLog(ACTOR_LOG_F, 96, 0);
    CrossboneIsle_RideLog(ACTOR_LOG_F, 80, 0);
  } else if (column == 31) {
    CrossboneIsle_RideLog(ACTOR_LOG_F, 80, 0);
  } else if (column == 34) {
    CrossboneIsle_RideLog(ACTOR_LOG_F, 32, 0);
  } else if (column == 35) {
    CrossboneIsle_RideLog(ACTOR_LOG_F, 16, 0);
  } else if (column == 36) {
    return;
  }
  CrossboneIsle_MoveNorthSouthLogCells(ACTOR_LOG_F, column, row);
}

/// Walking west along the east pool's north-west log rolls it back west along
/// the north edge, with Isaac on top, to the north-west corner.
///
/// It stops short of the log from south-east of the island or the south-west
/// log if either has been rolled up to the north edge.
static void CrossboneIsle_RideLogHWest(void) {
  s32 column = FIELD_CELL(Actor_Get(ACTOR_LOG_H)->x.fixed);
  s32 row = FIELD_CELL(Actor_Get(ACTOR_LOG_H)->z.fixed);
  s32 logARow = FIELD_CELL(Actor_Get(ACTOR_LOG_A)->z.fixed);
  s32 logCRow = FIELD_CELL(Actor_Get(ACTOR_LOG_C)->z.fixed);

  if (column == 35) {
    if (logCRow == 7) {
      CrossboneIsle_RideLog(ACTOR_LOG_H, -16, 0);
    } else if (logARow == 7) {
      CrossboneIsle_RideLog(ACTOR_LOG_H, -112, 0);
    } else {
      CrossboneIsle_RideLog(ACTOR_LOG_H, -96, 0);
      CrossboneIsle_RideLog(ACTOR_LOG_H, -80, 0);
    }
  } else if (column == 34) {
    if (logCRow == 7) {
      return;
    }
    CrossboneIsle_RideLog(ACTOR_LOG_H, -96, 0);
    CrossboneIsle_RideLog(ACTOR_LOG_H, -64, 0);
  } else if (column == 33) {
    CrossboneIsle_RideLog(ACTOR_LOG_H, -144, 0);
  } else if (column == 31) {
    CrossboneIsle_RideLog(ACTOR_LOG_H, -80, 0);
  } else if (column == 30) {
    CrossboneIsle_RideLog(ACTOR_LOG_H, -96, 0);
  } else if (column == 24) {
    return;
  }
  CrossboneIsle_MoveNorthSouthLogCells(ACTOR_LOG_H, column, row);
}

/// Walking east along the east pool's north-west log rolls it east along the
/// north edge, with Isaac on top, toward the north-east corner.
///
/// It stops short of the north-east log, wherever that has been rolled, or of
/// the log from south-east of the island if that has been rolled up to the
/// north edge.
static void CrossboneIsle_RideLogHEast(void) {
  s32 column = FIELD_CELL(Actor_Get(ACTOR_LOG_H)->x.fixed);
  s32 row = FIELD_CELL(Actor_Get(ACTOR_LOG_H)->z.fixed);
  s32 logCRow = FIELD_CELL(Actor_Get(ACTOR_LOG_C)->z.fixed);
  s32 logFColumn = FIELD_CELL(Actor_Get(ACTOR_LOG_F)->x.fixed);

  if (column == 24) {
    if (logCRow == 7 || logFColumn == 31) {
      CrossboneIsle_RideLog(ACTOR_LOG_H, 96, 0);
    } else if (logFColumn == 34) {
      CrossboneIsle_RideLog(ACTOR_LOG_H, 64, 0);
      CrossboneIsle_RideLog(ACTOR_LOG_H, 80, 0);
    } else if (logFColumn == 35) {
      CrossboneIsle_RideLog(ACTOR_LOG_H, 80, 0);
      CrossboneIsle_RideLog(ACTOR_LOG_H, 80, 0);
    } else {
      CrossboneIsle_RideLog(ACTOR_LOG_H, 80, 0);
      CrossboneIsle_RideLog(ACTOR_LOG_H, 96, 0);
    }
  } else if (column == 30 || logFColumn == 31) {
    if (logCRow == 7) {
      return;
    }
    if (logFColumn == 34) {
      CrossboneIsle_RideLog(ACTOR_LOG_H, 48, 0);
    } else if (logFColumn == 35) {
      CrossboneIsle_RideLog(ACTOR_LOG_H, 64, 0);
    } else {
      CrossboneIsle_RideLog(ACTOR_LOG_H, 80, 0);
    }
  } else if (column == 33) {
    if (logFColumn == 34) {
      return;
    }
    if (logFColumn == 35) {
      CrossboneIsle_RideLog(ACTOR_LOG_H, 16, 0);
    } else {
      CrossboneIsle_RideLog(ACTOR_LOG_H, 32, 0);
    }
  } else if (column == 34) {
    CrossboneIsle_RideLog(ACTOR_LOG_H, 16, 0);
  } else if (column == 35) {
    return;
  }
  CrossboneIsle_MoveNorthSouthLogCells(ACTOR_LOG_H, column, row);
}

/// Walking north along the west one of the west pool's three south logs rolls
/// it north up the bay at the pool's south-west, with Isaac on top.
///
/// It stops short of the south side log if that has been rolled west into the
/// bay.
static void CrossboneIsle_RideLogJNorth(void) {
  s32 column = FIELD_CELL(Actor_Get(ACTOR_LOG_J)->x.fixed);
  s32 row = FIELD_CELL(Actor_Get(ACTOR_LOG_J)->z.fixed);
  s32 logLColumn = FIELD_CELL(Actor_Get(ACTOR_LOG_L)->x.fixed);

  if (row == 19) {
    if (LIES_ACROSS(logLColumn, LOG_J_COLUMN)) {
      CrossboneIsle_RideLog(ACTOR_LOG_J, 0, -16);
    } else {
      CrossboneIsle_RideLog(ACTOR_LOG_J, 0, -64);
    }
  } else if (row == 18) {
    if (LIES_ACROSS(logLColumn, LOG_J_COLUMN)) {
      return;
    }
    CrossboneIsle_RideLog(ACTOR_LOG_J, 0, -48);
  } else if (row == 15) {
    return;
  }
  CrossboneIsle_MoveWestEastLogCells(ACTOR_LOG_J, column, row);
}

/// Walking south along the west one of the west pool's three south logs rolls
/// it back south, with Isaac on top, to the south shore.
static void CrossboneIsle_RideLogJSouth(void) {
  s32 column = FIELD_CELL(Actor_Get(ACTOR_LOG_J)->x.fixed);
  s32 row = FIELD_CELL(Actor_Get(ACTOR_LOG_J)->z.fixed);

  if (row == 15) {
    CrossboneIsle_RideLog(ACTOR_LOG_J, 0, 64);
  } else if (row == 18) {
    CrossboneIsle_RideLog(ACTOR_LOG_J, 0, 16);
  } else if (row == 19) {
    return;
  }
  CrossboneIsle_MoveWestEastLogCells(ACTOR_LOG_J, column, row);
}

/// Walking north along the middle one of the west pool's three south logs
/// rolls it north up the pool, with Isaac on top, to the north shore.
///
/// It stops short of the first of the pool's side logs that has been rolled
/// west across its way.
static void CrossboneIsle_RideLogKNorth(void) {
  s32 column;
  s32 row;
  s32 logLColumn;
  s32 logGColumn;
  s32 logIColumn;

  column = FIELD_CELL(Actor_Get(ACTOR_LOG_K)->x.fixed);
  row = FIELD_CELL(Actor_Get(ACTOR_LOG_K)->z.fixed);
  logLColumn = FIELD_CELL(Actor_Get(ACTOR_LOG_L)->x.fixed);
  logGColumn = FIELD_CELL(Actor_Get(ACTOR_LOG_G)->x.fixed);
  logIColumn = FIELD_CELL(Actor_Get(ACTOR_LOG_I)->x.fixed);
  if (row == 19) {
    if (LIES_ACROSS(logLColumn, LOG_K_COLUMN)) {
      CrossboneIsle_RideLog(ACTOR_LOG_K, 0, -16);
    } else if (LIES_ACROSS(logGColumn, LOG_K_COLUMN)) {
      CrossboneIsle_RideLog(ACTOR_LOG_K, 0, -64);
    } else if (LIES_ACROSS(logIColumn, LOG_K_COLUMN)) {
      CrossboneIsle_RideLog(ACTOR_LOG_K, 0, -112);
    } else {
      CrossboneIsle_RideLog(ACTOR_LOG_K, 0, -64);
      CrossboneIsle_RideLog(ACTOR_LOG_K, 0, -96);
    }
  } else if (row == 18) {
    if (LIES_ACROSS(logLColumn, LOG_K_COLUMN)) {
      return;
    }
    if (LIES_ACROSS(logGColumn, LOG_K_COLUMN)) {
      CrossboneIsle_RideLog(ACTOR_LOG_K, 0, -48);
    } else if (LIES_ACROSS(logIColumn, LOG_K_COLUMN)) {
      CrossboneIsle_RideLog(ACTOR_LOG_K, 0, -96);
    } else {
      CrossboneIsle_RideLog(ACTOR_LOG_K, 0, -144);
    }
  } else if (row == 15) {
    if (LIES_ACROSS(logGColumn, LOG_K_COLUMN)) {
      return;
    }
    if (LIES_ACROSS(logIColumn, LOG_K_COLUMN)) {
      CrossboneIsle_RideLog(ACTOR_LOG_K, 0, -48);
    } else {
      CrossboneIsle_RideLog(ACTOR_LOG_K, 0, -96);
    }
  } else if (row == 14) {
    if (LIES_ACROSS(logGColumn, LOG_K_COLUMN)) {
      return;
    }
    if (LIES_ACROSS(logIColumn, LOG_K_COLUMN)) {
      CrossboneIsle_RideLog(ACTOR_LOG_K, 0, -32);
    } else {
      CrossboneIsle_RideLog(ACTOR_LOG_K, 0, -80);
    }
  } else if (row == 12) {
    if (LIES_ACROSS(logIColumn, LOG_K_COLUMN)) {
      return;
    }
    CrossboneIsle_RideLog(ACTOR_LOG_K, 0, -48);
  } else if (row == 11) {
    if (LIES_ACROSS(logIColumn, LOG_K_COLUMN)) {
      return;
    }
    CrossboneIsle_RideLog(ACTOR_LOG_K, 0, -32);
  } else if (row == 9) {
    return;
  }
  wait(2);
  MapGrid_CopyCellAttributeRect(column - 1, row, 3, 1, column - 1,
                                FIELD_CELL(Actor_Get(ACTOR_LOG_K)->z.fixed));
  MapGrid_CopyCellAttributes(0, 0, 3, 1, column - 1, row);
}

/// Walking south along the middle one of the west pool's three south logs
/// rolls it back south, with Isaac on top, to the south shore.
///
/// It stops short of the middle or south side log if either has been rolled
/// west across its way.
static void CrossboneIsle_RideLogKSouth(void) {
  s32 column;
  s32 row;
  s32 logLColumn;
  s32 logGColumn;

  column = FIELD_CELL(Actor_Get(ACTOR_LOG_K)->x.fixed);
  row = FIELD_CELL(Actor_Get(ACTOR_LOG_K)->z.fixed);
  logLColumn = FIELD_CELL(Actor_Get(ACTOR_LOG_L)->x.fixed);
  logGColumn = FIELD_CELL(Actor_Get(ACTOR_LOG_G)->x.fixed);

  if (row == 9) {
    if (LIES_ACROSS(logGColumn, LOG_K_COLUMN)) {
      CrossboneIsle_RideLog(ACTOR_LOG_K, 0, 32);
    } else if (LIES_ACROSS(logLColumn, LOG_K_COLUMN)) {
      CrossboneIsle_RideLog(ACTOR_LOG_K, 0, 80);
    } else {
      CrossboneIsle_RideLog(ACTOR_LOG_K, 0, 64);
      CrossboneIsle_RideLog(ACTOR_LOG_K, 0, 96);
    }
  } else if (row == 11) {
    if (LIES_ACROSS(logGColumn, LOG_K_COLUMN)) {
      return;
    }
    if (LIES_ACROSS(logLColumn, LOG_K_COLUMN)) {
      CrossboneIsle_RideLog(ACTOR_LOG_K, 0, 48);
    } else {
      CrossboneIsle_RideLog(ACTOR_LOG_K, 0, 128);
    }
  } else if (row == 12) {
    if (LIES_ACROSS(logLColumn, LOG_K_COLUMN)) {
      CrossboneIsle_RideLog(ACTOR_LOG_K, 0, 32);
    } else {
      CrossboneIsle_RideLog(ACTOR_LOG_K, 0, 112);
    }
  } else if (row == 14) {
    if (LIES_ACROSS(logLColumn, LOG_K_COLUMN)) {
      return;
    }
    CrossboneIsle_RideLog(ACTOR_LOG_K, 0, 80);
  } else if (row == 15) {
    CrossboneIsle_RideLog(ACTOR_LOG_K, 0, 64);
  } else if (row == 18) {
    CrossboneIsle_RideLog(ACTOR_LOG_K, 0, 16);
  } else if (row == 19) {
    return;
  }

  wait(2);

  MapGrid_CopyCellAttributeRect(column - 1, row, 3, 1, column - 1,
                                FIELD_CELL(Actor_Get(ACTOR_LOG_K)->z.fixed));
  MapGrid_CopyCellAttributes(0, 0, 3, 1, column - 1, row);
}

/// Walking north along the east one of the west pool's three south logs rolls
/// it north up the pool, with Isaac on top, to the north shore.
///
/// It stops short of the first of the pool's side logs that has been rolled
/// west across its way.
static void CrossboneIsle_RideLogBNorth(void) {
  s32 column;
  s32 row;
  s32 logLColumn;
  s32 logGColumn;
  s32 logIColumn;

  column = FIELD_CELL(Actor_Get(ACTOR_LOG_B)->x.fixed);
  row = FIELD_CELL(Actor_Get(ACTOR_LOG_B)->z.fixed);
  logLColumn = FIELD_CELL(Actor_Get(ACTOR_LOG_L)->x.fixed);
  logGColumn = FIELD_CELL(Actor_Get(ACTOR_LOG_G)->x.fixed);
  logIColumn = FIELD_CELL(Actor_Get(ACTOR_LOG_I)->x.fixed);
  if (row == 19) {
    if (LIES_ACROSS(logLColumn, LOG_B_COLUMN)) {
      CrossboneIsle_RideLog(ACTOR_LOG_B, 0, -16);
    } else if (LIES_ACROSS(logGColumn, LOG_B_COLUMN)) {
      CrossboneIsle_RideLog(ACTOR_LOG_B, 0, -64);
    } else if (LIES_ACROSS(logIColumn, LOG_B_COLUMN)) {
      CrossboneIsle_RideLog(ACTOR_LOG_B, 0, -112);
    } else {
      CrossboneIsle_RideLog(ACTOR_LOG_B, 0, -80);
      CrossboneIsle_RideLog(ACTOR_LOG_B, 0, -96);
    }
  } else if (row == 18) {
    if (LIES_ACROSS(logLColumn, LOG_B_COLUMN)) {
      return;
    }
    if (LIES_ACROSS(logGColumn, LOG_B_COLUMN)) {
      CrossboneIsle_RideLog(ACTOR_LOG_B, 0, -48);
    } else if (LIES_ACROSS(logIColumn, LOG_B_COLUMN)) {
      CrossboneIsle_RideLog(ACTOR_LOG_B, 0, -96);
    } else {
      CrossboneIsle_RideLog(ACTOR_LOG_B, 0, -96);
      CrossboneIsle_RideLog(ACTOR_LOG_B, 0, -64);
    }
  } else if (row == 15) {
    if (LIES_ACROSS(logGColumn, LOG_B_COLUMN)) {
      return;
    }
    if (LIES_ACROSS(logIColumn, LOG_B_COLUMN)) {
      CrossboneIsle_RideLog(ACTOR_LOG_B, 0, -48);
    } else {
      CrossboneIsle_RideLog(ACTOR_LOG_B, 0, -112);
    }
  } else if (row == 14) {
    if (LIES_ACROSS(logGColumn, LOG_B_COLUMN)) {
      return;
    }
    if (LIES_ACROSS(logIColumn, LOG_B_COLUMN)) {
      CrossboneIsle_RideLog(ACTOR_LOG_B, 0, -32);
    } else {
      CrossboneIsle_RideLog(ACTOR_LOG_B, 0, -96);
    }
  } else if (row == 12) {
    if (LIES_ACROSS(logIColumn, LOG_B_COLUMN)) {
      return;
    }
    CrossboneIsle_RideLog(ACTOR_LOG_B, 0, -64);
  } else if (row == 11) {
    if (LIES_ACROSS(logIColumn, LOG_B_COLUMN)) {
      return;
    }
    CrossboneIsle_RideLog(ACTOR_LOG_B, 0, -48);
  } else if (row >= 0 && row <= 9) {
    return;
  }
  wait(2);
  MapGrid_CopyCellAttributeRect(column - 1, row, 3, 1, column - 1,
                                FIELD_CELL(Actor_Get(ACTOR_LOG_B)->z.fixed));
  MapGrid_CopyCellAttributes(0, 0, 3, 1, column - 1, row);
}

/// Walking south along the east one of the west pool's three south logs rolls
/// it back south, with Isaac on top, to the south shore.
///
/// It stops short of the first of the pool's side logs that has been rolled
/// west across its way.
static void CrossboneIsle_RideLogBSouth(void) {
  s32 column;
  s32 row;
  s32 logLColumn;
  s32 logGColumn;
  s32 logIColumn;

  column = FIELD_CELL(Actor_Get(ACTOR_LOG_B)->x.fixed);
  row = FIELD_CELL(Actor_Get(ACTOR_LOG_B)->z.fixed);
  logLColumn = FIELD_CELL(Actor_Get(ACTOR_LOG_L)->x.fixed);
  logGColumn = FIELD_CELL(Actor_Get(ACTOR_LOG_G)->x.fixed);
  logIColumn = FIELD_CELL(Actor_Get(ACTOR_LOG_I)->x.fixed);

  if (row == 8) {
    if (LIES_ACROSS(logIColumn, LOG_B_COLUMN)) {
      return;
    }
    if (LIES_ACROSS(logGColumn, LOG_B_COLUMN)) {
      CrossboneIsle_RideLog(ACTOR_LOG_B, 0, 48);
    } else {
      if (!LIES_ACROSS(logLColumn, LOG_B_COLUMN)) {
        CrossboneIsle_RideLog(ACTOR_LOG_B, 0, 80);
      }
      CrossboneIsle_RideLog(ACTOR_LOG_B, 0, 96);
    }
  } else if (row == 11) {
    if (LIES_ACROSS(logGColumn, LOG_B_COLUMN)) {
      return;
    }
    if (LIES_ACROSS(logLColumn, LOG_B_COLUMN)) {
      CrossboneIsle_RideLog(ACTOR_LOG_B, 0, 48);
    } else {
      CrossboneIsle_RideLog(ACTOR_LOG_B, 0, 128);
    }
  } else if (row == 12) {
    if (LIES_ACROSS(logGColumn, LOG_B_COLUMN)) {
      return;
    }
    if (LIES_ACROSS(logLColumn, LOG_B_COLUMN)) {
      CrossboneIsle_RideLog(ACTOR_LOG_B, 0, 32);
    } else {
      CrossboneIsle_RideLog(ACTOR_LOG_B, 0, 112);
    }
  } else if (row == 14) {
    if (LIES_ACROSS(logLColumn, LOG_B_COLUMN)) {
      return;
    }
    CrossboneIsle_RideLog(ACTOR_LOG_B, 0, 80);
  } else if (row == 15) {
    CrossboneIsle_RideLog(ACTOR_LOG_B, 0, 64);
  } else if (row == 18) {
    CrossboneIsle_RideLog(ACTOR_LOG_B, 0, 16);
  }

  wait(2);

  MapGrid_CopyCellAttributeRect(column - 1, row, 3, 1, column - 1,
                                FIELD_CELL(Actor_Get(ACTOR_LOG_B)->z.fixed));
  MapGrid_CopyCellAttributes(0, 0, 3, 1, column - 1, row);
}

/// Walking west along the south one of the west pool's three side logs rolls
/// it west across the pool, with Isaac on top, into the bay at its south-west.
///
/// It stops short of the first of the pool's south logs that has been rolled
/// north across its way.
static void CrossboneIsle_RideLogLWest(void) {
  s32 column;
  s32 row;
  s32 logJRow;
  s32 logKRow;
  s32 logBRow;

  column = FIELD_CELL(Actor_Get(ACTOR_LOG_L)->x.fixed);
  row = FIELD_CELL(Actor_Get(ACTOR_LOG_L)->z.fixed);
  logJRow = FIELD_CELL(Actor_Get(ACTOR_LOG_J)->z.fixed);
  logKRow = FIELD_CELL(Actor_Get(ACTOR_LOG_K)->z.fixed);
  logBRow = FIELD_CELL(Actor_Get(ACTOR_LOG_B)->z.fixed);
  if (column == 3) {
    return;
  }
  if (column == 13) {
    if (logBRow == 15) {
      CrossboneIsle_RideLog(ACTOR_LOG_L, -16, 0);
    } else if (logKRow == 15) {
      CrossboneIsle_RideLog(ACTOR_LOG_L, -64, 0);
    } else if (logJRow == 15) {
      CrossboneIsle_RideLog(ACTOR_LOG_L, -112, 0);
    } else {
      CrossboneIsle_RideLog(ACTOR_LOG_L, -112, 0);
      CrossboneIsle_RideLog(ACTOR_LOG_L, -48, 0);
    }
  } else if (column == 6) {
    if (logJRow == 15) {
      return;
    }
    CrossboneIsle_RideLog(ACTOR_LOG_L, -48, 0);
  } else if (column == 5) {
    CrossboneIsle_RideLog(ACTOR_LOG_L, -32, 0);
  } else if (column == 8) {
    if (logKRow == 15) {
      return;
    }
    if (logJRow == 15) {
      CrossboneIsle_RideLog(ACTOR_LOG_L, -32, 0);
    } else {
      CrossboneIsle_RideLog(ACTOR_LOG_L, -80, 0);
    }
  } else if (column == 9) {
    if (logKRow == 15) {
      return;
    }
    if (logJRow == 15) {
      CrossboneIsle_RideLog(ACTOR_LOG_L, -48, 0);
    } else {
      CrossboneIsle_RideLog(ACTOR_LOG_L, -96, 0);
    }
  } else if (column == 12) {
    if (logBRow == 15) {
      return;
    }
    if (logKRow == 15) {
      CrossboneIsle_RideLog(ACTOR_LOG_L, -48, 0);
    } else if (logJRow == 15) {
      CrossboneIsle_RideLog(ACTOR_LOG_L, -96, 0);
    } else {
      CrossboneIsle_RideLog(ACTOR_LOG_L, -144, 0);
    }
  }
  wait(2);
  MapGrid_CopyCellAttributeRect(column, row - 1, 1, 3,
                                FIELD_CELL(Actor_Get(ACTOR_LOG_L)->x.fixed),
                                row - 1);
  MapGrid_CopyCellAttributes(0, 0, 1, 3, column, row - 1);
}

/// Walking east along the south one of the west pool's three side logs rolls
/// it back east, with Isaac on top, to the pool's east shore.
///
/// It stops short of the middle or east south log if either has been rolled
/// north across its way.
static void CrossboneIsle_RideLogLEast(void) {
  s32 column;
  s32 row;
  s32 logKRow;
  s32 logBRow;

  column = FIELD_CELL(Actor_Get(ACTOR_LOG_L)->x.fixed);
  row = FIELD_CELL(Actor_Get(ACTOR_LOG_L)->z.fixed);
  logKRow = FIELD_CELL(Actor_Get(ACTOR_LOG_K)->z.fixed);
  logBRow = FIELD_CELL(Actor_Get(ACTOR_LOG_B)->z.fixed);
  if (column == 3) {
    if (logKRow == 15) {
      CrossboneIsle_RideLog(ACTOR_LOG_L, 32, 0);
    } else if (logBRow == 15) {
      CrossboneIsle_RideLog(ACTOR_LOG_L, 80, 0);
    } else {
      CrossboneIsle_RideLog(ACTOR_LOG_L, 112, 0);
      CrossboneIsle_RideLog(ACTOR_LOG_L, 48, 0);
    }
  } else if (column == 5) {
    if (logKRow == 15) {
      return;
    }
    if (logBRow == 15) {
      CrossboneIsle_RideLog(ACTOR_LOG_L, 48, 0);
    } else {
      CrossboneIsle_RideLog(ACTOR_LOG_L, 128, 0);
    }
  } else if (column == 6) {
    if (logBRow == 15) {
      CrossboneIsle_RideLog(ACTOR_LOG_L, 32, 0);
    } else {
      CrossboneIsle_RideLog(ACTOR_LOG_L, 112, 0);
    }
  } else if (column == 8) {
    if (logBRow == 15) {
      return;
    }
    CrossboneIsle_RideLog(ACTOR_LOG_L, 80, 0);
  } else if (column == 9) {
    CrossboneIsle_RideLog(ACTOR_LOG_L, 64, 0);
  } else if (column == 12) {
    CrossboneIsle_RideLog(ACTOR_LOG_L, 16, 0);
  }
  wait(2);
  MapGrid_CopyCellAttributeRect(column, row - 1, 1, 3,
                                FIELD_CELL(Actor_Get(ACTOR_LOG_L)->x.fixed),
                                row - 1);
  MapGrid_CopyCellAttributes(0, 0, 1, 3, column, row - 1);
}

/// Walking west along the middle one of the west pool's three side logs rolls
/// it west across the pool, with Isaac on top, to its west side.
///
/// It stops short of the east or middle south log if either has been rolled
/// north across its way.
static void CrossboneIsle_RideLogGWest(void) {
  s32 column;
  s32 row;
  s32 logKRow;
  s32 logBRow;
  s32 footprintRow;

  column = FIELD_CELL(Actor_Get(ACTOR_LOG_G)->x.fixed);
  row = FIELD_CELL(Actor_Get(ACTOR_LOG_G)->z.fixed);
  logKRow = FIELD_CELL(Actor_Get(ACTOR_LOG_K)->z.fixed);
  logBRow = FIELD_CELL(Actor_Get(ACTOR_LOG_B)->z.fixed);

  if (column == 13) {
    if (LIES_ACROSS(logBRow, LOG_G_ROW)) {
      CrossboneIsle_RideLog(ACTOR_LOG_G, -16, 0);
    } else if (LIES_ACROSS(logKRow, LOG_G_ROW)) {
      CrossboneIsle_RideLog(ACTOR_LOG_G, -64, 0);
    } else {
      CrossboneIsle_RideLog(ACTOR_LOG_G, -112, 0);
    }
  } else if (column == 12) {
    if (LIES_ACROSS(logBRow, LOG_G_ROW)) {
      return;
    }
    if (LIES_ACROSS(logKRow, LOG_G_ROW)) {
      CrossboneIsle_RideLog(ACTOR_LOG_G, -48, 0);
    } else {
      CrossboneIsle_RideLog(ACTOR_LOG_G, -96, 0);
    }
  } else if (column == 9) {
    if (LIES_ACROSS(logKRow, LOG_G_ROW)) {
      return;
    }
    CrossboneIsle_RideLog(ACTOR_LOG_G, -48, 0);
  } else if (column == 8) {
    if (LIES_ACROSS(logKRow, LOG_G_ROW)) {
      return;
    }
    CrossboneIsle_RideLog(ACTOR_LOG_G, -32, 0);
  } else if (column == 6) {
    return;
  }

  wait(2);

  footprintRow = row - 1;
  MapGrid_CopyCellAttributeRect(column, footprintRow, 1, 3,
                                FIELD_CELL(Actor_Get(ACTOR_LOG_G)->x.fixed),
                                footprintRow);
  MapGrid_CopyCellAttributes(0, 0, 1, 3, column, footprintRow);
}

/// Walking east along the middle one of the west pool's three side logs rolls
/// it back east, with Isaac on top, to the pool's east shore.
///
/// It stops short of the east or middle south log if either has been rolled
/// north across its way.
static void CrossboneIsle_RideLogGEast(void) {
  s32 column;
  s32 row;
  s32 logKRow;
  s32 logBRow;
  s32 footprintRow;

  column = FIELD_CELL(Actor_Get(ACTOR_LOG_G)->x.fixed);
  row = FIELD_CELL(Actor_Get(ACTOR_LOG_G)->z.fixed);
  logKRow = FIELD_CELL(Actor_Get(ACTOR_LOG_K)->z.fixed);
  logBRow = FIELD_CELL(Actor_Get(ACTOR_LOG_B)->z.fixed);

  if (column == 6) {
    if (LIES_ACROSS(logBRow, LOG_G_ROW)) {
      CrossboneIsle_RideLog(ACTOR_LOG_G, 32, 0);
    } else if (LIES_ACROSS(logKRow, LOG_G_ROW)) {
      CrossboneIsle_RideLog(ACTOR_LOG_G, 64, 0);
    } else {
      CrossboneIsle_RideLog(ACTOR_LOG_G, 112, 0);
    }
  } else if (column == 8) {
    if (LIES_ACROSS(logBRow, LOG_G_ROW)) {
      return;
    }
    CrossboneIsle_RideLog(ACTOR_LOG_G, 80, 0);
  } else if (column == 9) {
    if (LIES_ACROSS(logBRow, LOG_G_ROW)) {
      return;
    }
    CrossboneIsle_RideLog(ACTOR_LOG_G, 64, 0);
  } else if (column == 12) {
    CrossboneIsle_RideLog(ACTOR_LOG_G, 16, 0);
  } else if (column == 13) {
    return;
  }

  wait(2);

  footprintRow = row - 1;
  MapGrid_CopyCellAttributeRect(column, footprintRow, 1, 3,
                                FIELD_CELL(Actor_Get(ACTOR_LOG_G)->x.fixed),
                                footprintRow);
  MapGrid_CopyCellAttributes(0, 0, 1, 3, column, footprintRow);
}

/// Walking west along the north one of the west pool's three side logs rolls
/// it west across the north of the pool, with Isaac on top, to its west side.
///
/// It stops short of the east or middle south log if either has been rolled
/// north across its way.
static void CrossboneIsle_RideLogIWest(void) {
  s32 column;
  s32 row;
  s32 logKRow;
  s32 logBRow;
  s32 footprintRow;

  column = FIELD_CELL(Actor_Get(ACTOR_LOG_I)->x.fixed);
  row = FIELD_CELL(Actor_Get(ACTOR_LOG_I)->z.fixed);
  logKRow = FIELD_CELL(Actor_Get(ACTOR_LOG_K)->z.fixed);
  logBRow = FIELD_CELL(Actor_Get(ACTOR_LOG_B)->z.fixed);

  if (column == 13) {
    if (LIES_ACROSS(logBRow, LOG_I_ROW)) {
      CrossboneIsle_RideLog(ACTOR_LOG_I, -16, 0);
    } else if (LIES_ACROSS(logKRow, LOG_I_ROW)) {
      CrossboneIsle_RideLog(ACTOR_LOG_I, -64, 0);
    } else {
      CrossboneIsle_RideLog(ACTOR_LOG_I, -112, 0);
    }
  } else if (column == 12) {
    if (LIES_ACROSS(logBRow, LOG_I_ROW)) {
      return;
    }
    if (LIES_ACROSS(logKRow, LOG_I_ROW)) {
      CrossboneIsle_RideLog(ACTOR_LOG_I, -48, 0);
    } else {
      CrossboneIsle_RideLog(ACTOR_LOG_I, -96, 0);
    }
  } else if (column == 9) {
    if (LIES_ACROSS(logKRow, LOG_I_ROW)) {
      return;
    }
    CrossboneIsle_RideLog(ACTOR_LOG_I, -48, 0);
  } else if (column == 8) {
    CrossboneIsle_RideLog(ACTOR_LOG_I, -32, 0);
  } else if (column == 6) {
    return;
  }

  wait(2);

  footprintRow = row - 1;
  MapGrid_CopyCellAttributeRect(column, footprintRow, 1, 3,
                                FIELD_CELL(Actor_Get(ACTOR_LOG_I)->x.fixed),
                                footprintRow);
  MapGrid_CopyCellAttributes(0, 0, 1, 3, column, footprintRow);
}

/// Walking east along the north one of the west pool's three side logs rolls
/// it back east, with Isaac on top, to the pool's east shore.
///
/// It stops short of the east south log if that has been rolled north across
/// its way.
static void CrossboneIsle_RideLogIEast(void) {
  s32 column;
  s32 row;
  s32 logBRow;

  column = FIELD_CELL(Actor_Get(ACTOR_LOG_I)->x.fixed);
  row = FIELD_CELL(Actor_Get(ACTOR_LOG_I)->z.fixed);
  logBRow = FIELD_CELL(Actor_Get(ACTOR_LOG_B)->z.fixed);

  if (column == 6) {
    if (LIES_ACROSS(logBRow, LOG_I_ROW)) {
      CrossboneIsle_RideLog(ACTOR_LOG_I, 32, 0);
    } else {
      CrossboneIsle_RideLog(ACTOR_LOG_I, 112, 0);
    }
  } else if (column == 8) {
    if (LIES_ACROSS(logBRow, LOG_I_ROW)) {
      return;
    }
    CrossboneIsle_RideLog(ACTOR_LOG_I, 80, 0);
  } else if (column == 9) {
    CrossboneIsle_RideLog(ACTOR_LOG_I, 64, 0);
  } else if (column == 12) {
    CrossboneIsle_RideLog(ACTOR_LOG_I, 16, 0);
  } else if (column == 13) {
    return;
  }

  wait(2);

  MapGrid_CopyCellAttributeRect(column, row - 1, 1, 3,
                                FIELD_CELL(Actor_Get(ACTOR_LOG_I)->x.fixed),
                                row - 1);
  MapGrid_CopyCellAttributes(0, 0, 1, 3, column, row - 1);
}

#include "shared/actor_orbit.c"

#include "shared/pushed_actor_tables.c"
// clang-format off

#include "shared/field_effect_duration_scripts.c"

u16 gCrossboneIsle_DoorOpens[] = {
    MAPGRID_ANIMATION_STEP(58, 23, 1, 2, 5), MAPGRID_ANIMATION_STEP(59, 23, 1, 2, 5), MAPGRID_ANIMATION_STEPS_END,
};

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

/// Unused: an entrance table nothing reads, one way in at (120, 152) facing
/// south, with no camera box.
struct SceneEntrance gCrossboneIsle_UnusedEntrances[] = {
    { 0, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
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
    // The nine floors (CrossboneIsleFirstFloor to CrossboneIsleNinthFloor)
    // enter through this shared tail, since every floor numbers its entrances
    // alike.

    { FLOOR_FROM_PREVIOUS_ROOM, CONDITION_ALWAYS, 840, 0, 176, FACING_WEST, 0, 432, 32, 880, 352, 0 },
    { FLOOR_FROM_NEXT_ROOM, CONDITION_ALWAYS, 488, 0, 176, FACING_EAST, 0, 432, 32, 880, 352, 0 },
    { FLOOR_FROM_SIDE_ROOM_EAST, CONDITION_ALWAYS, 712, 0, 152, FACING_SOUTH, 0, 432, 32, 880, 352, 0 },
    { FLOOR_FROM_SIDE_ROOM_WEST, CONDITION_ALWAYS, 600, 0, 152, FACING_SOUTH, 0, 432, 32, 880, 352, 0 },
    { ENTRANCE_AFTER_BATTLE, CONDITION_ALWAYS, 712, 0, 152, FACING_NORTH, 0, 432, 32, 880, 352, 0 },
    { SCENE_TABLE_END },
};

u32 gCrossboneIsle_Exits[] = {
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

struct ActorPlacement gCrossboneIsle_FloorActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        632, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

struct Trigger gCrossboneIsle_FloorTriggers[] = {
    { TRIGGER_EXIT, FLOOR_TO_PREVIOUS_ROOM, CONDITION_ALWAYS, FLOOR_TO_PREVIOUS_ROOM },
    { TRIGGER_EXIT, FLOOR_TO_NEXT_ROOM, CONDITION_ALWAYS, FLOOR_TO_NEXT_ROOM },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), FLOOR_TO_SIDE_ROOM_EAST, CONDITION_ALWAYS, EVENT(CrossboneIsle_OpenEastDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), FLOOR_TO_SIDE_ROOM_WEST, CONDITION_ALWAYS, EVENT(CrossboneIsle_OpenWestDoor) },
    { TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, CELL_BEHAVIOR_CROSSBONE_ISLE_WEST_OF_PILLAR, CONDITION_ALWAYS, EVENT(CrossboneIsle_PushPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR, CONDITION_ALWAYS, EVENT(CrossboneIsle_MovePillar) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), TREASURE_SPOT_CROSSBONE_ISLE_FIRST_FLOOR_MIMIC, FLAG_CROSSBONE_ISLE_FIRST_FLOOR_MIMIC_BEATEN, FIND_MIMIC | 4 },
    { TRIGGER_TABLE_END },
};

struct Trigger gCrossboneIsle_CaveEntranceTriggers[] = {
    { TRIGGER_EXIT, CROSSBONE_ISLE_CAVE_ENTRANCE_TO_CROSSBONE_ISLE, CONDITION_ALWAYS, CROSSBONE_ISLE_CAVE_ENTRANCE_TO_CROSSBONE_ISLE },
    { TRIGGER_EXIT, CROSSBONE_ISLE_CAVE_ENTRANCE_TO_CROSSBONE_ISLE_FIRST_FLOOR, CONDITION_ALWAYS, CROSSBONE_ISLE_CAVE_ENTRANCE_TO_CROSSBONE_ISLE_FIRST_FLOOR },
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
    { TRIGGER_EXIT, CROSSBONE_ISLE_TO_WORLD_MAP, CONDITION_ALWAYS, CROSSBONE_ISLE_TO_WORLD_MAP },
    { TRIGGER_EXIT, CROSSBONE_ISLE_TO_CROSSBONE_ISLE_CAVE_ENTRANCE, CONDITION_ALWAYS, CROSSBONE_ISLE_TO_CROSSBONE_ISLE_CAVE_ENTRANCE },
    { TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, CELL_BEHAVIOR_CROSSBONE_ISLE_BESIDE_WEST_LOG, CONDITION_ALWAYS, EVENT(CrossboneIsle_PushIsleLog) },
    { TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, CELL_BEHAVIOR_CROSSBONE_ISLE_BESIDE_EAST_LOG, CONDITION_ALWAYS, EVENT(CrossboneIsle_PushIsleLog) },
    { EXAMINE_CHEST_FACING(FACING_WEST), TREASURE_SPOT_CROSSBONE_ISLE_COIN, FLAG_CROSSBONE_ISLE_COIN, FIND_COINS(1) },
    { EXAMINE_CHEST_FACING(FACING_NORTH), TREASURE_SPOT_CROSSBONE_ISLE_COIN, FLAG_CROSSBONE_ISLE_COIN, FIND_COINS(1) },
    { EXAMINE_CHEST_FACING(FACING_NORTHWEST), TREASURE_SPOT_CROSSBONE_ISLE_COIN, FLAG_CROSSBONE_ISLE_COIN, FIND_COINS(1) },
    { EXAMINE_CHEST_FACING(FACING_WEST), TREASURE_SPOT_CROSSBONE_ISLE_MINT, FLAG_CROSSBONE_ISLE_MINT, FIND_ITEM(ITEM_MINT) },
    { EXAMINE_CHEST_FACING(FACING_NORTH), TREASURE_SPOT_CROSSBONE_ISLE_MINT, FLAG_CROSSBONE_ISLE_MINT, FIND_ITEM(ITEM_MINT) },
    { EXAMINE_CHEST_FACING(FACING_NORTHWEST), TREASURE_SPOT_CROSSBONE_ISLE_MINT, FLAG_CROSSBONE_ISLE_MINT, FIND_ITEM(ITEM_MINT) },
    { TRIGGER_TABLE_END },
};

struct Trigger gCrossboneIsle_FloatingLogsTriggers[] = {
    { TRIGGER_EXIT, CROSSBONE_ISLE_FLOATING_LOGS_TO_CROSSBONE_ISLE_NINTH_FLOOR_EAST, CONDITION_ALWAYS, CROSSBONE_ISLE_FLOATING_LOGS_TO_CROSSBONE_ISLE_NINTH_FLOOR_EAST },
    { TRIGGER_EXIT, CROSSBONE_ISLE_FLOATING_LOGS_TO_CROSSBONE_ISLE_NINTH_FLOOR_WEST, CONDITION_ALWAYS, CROSSBONE_ISLE_FLOATING_LOGS_TO_CROSSBONE_ISLE_NINTH_FLOOR_WEST },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_CROSSBONE_ISLE_SHORE_HOP_WEST, CONDITION_ALWAYS, EVENT(CrossboneIsle_HopWest) },
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_CROSSBONE_ISLE_SHORE_HOP_EAST, CONDITION_ALWAYS, EVENT(CrossboneIsle_HopEast) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_CROSSBONE_ISLE_SHORE_HOP_SOUTH, CONDITION_ALWAYS, EVENT(CrossboneIsle_HopSouth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_CROSSBONE_ISLE_SHORE_HOP_NORTH, CONDITION_ALWAYS, EVENT(CrossboneIsle_HopNorth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_A, CONDITION_ALWAYS, EVENT(CrossboneIsle_RideLogANorth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_A, CONDITION_ALWAYS, EVENT(CrossboneIsle_RideLogASouth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_A, CONDITION_ALWAYS, EVENT(CrossboneIsle_HopEast) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_A, CONDITION_ALWAYS, EVENT(CrossboneIsle_HopWest) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_C, CONDITION_ALWAYS, EVENT(CrossboneIsle_RideLogCNorth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_C, CONDITION_ALWAYS, EVENT(CrossboneIsle_RideLogCSouth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_C, CONDITION_ALWAYS, EVENT(CrossboneIsle_HopEast) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_C, CONDITION_ALWAYS, EVENT(CrossboneIsle_HopWest) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_D, CONDITION_ALWAYS, EVENT(CrossboneIsle_RideLogDWest) },
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_D, CONDITION_ALWAYS, EVENT(CrossboneIsle_RideLogDEast) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_D, CONDITION_ALWAYS, EVENT(CrossboneIsle_HopNorth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_D, CONDITION_ALWAYS, EVENT(CrossboneIsle_HopSouth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_E, CONDITION_ALWAYS, EVENT(CrossboneIsle_RideLogEWest) },
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_E, CONDITION_ALWAYS, EVENT(CrossboneIsle_RideLogEEast) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_E, CONDITION_ALWAYS, EVENT(CrossboneIsle_HopNorth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_E, CONDITION_ALWAYS, EVENT(CrossboneIsle_HopSouth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_F, CONDITION_ALWAYS, EVENT(CrossboneIsle_RideLogFWest) },
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_F, CONDITION_ALWAYS, EVENT(CrossboneIsle_RideLogFEast) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_F, CONDITION_ALWAYS, EVENT(CrossboneIsle_HopNorth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_F, CONDITION_ALWAYS, EVENT(CrossboneIsle_HopSouth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_H, CONDITION_ALWAYS, EVENT(CrossboneIsle_RideLogHWest) },
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_H, CONDITION_ALWAYS, EVENT(CrossboneIsle_RideLogHEast) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_H, CONDITION_ALWAYS, EVENT(CrossboneIsle_HopNorth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_H, CONDITION_ALWAYS, EVENT(CrossboneIsle_HopSouth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_J, CONDITION_ALWAYS, EVENT(CrossboneIsle_RideLogJNorth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_J, CONDITION_ALWAYS, EVENT(CrossboneIsle_RideLogJSouth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_J, CONDITION_ALWAYS, EVENT(CrossboneIsle_HopEast) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_J, CONDITION_ALWAYS, EVENT(CrossboneIsle_HopWest) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_K, CONDITION_ALWAYS, EVENT(CrossboneIsle_RideLogKNorth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_K, CONDITION_ALWAYS, EVENT(CrossboneIsle_RideLogKSouth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_K, CONDITION_ALWAYS, EVENT(CrossboneIsle_HopEast) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_K, CONDITION_ALWAYS, EVENT(CrossboneIsle_HopWest) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_B, CONDITION_ALWAYS, EVENT(CrossboneIsle_RideLogBNorth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_B, CONDITION_ALWAYS, EVENT(CrossboneIsle_RideLogBSouth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_B, CONDITION_ALWAYS, EVENT(CrossboneIsle_HopEast) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_B, CONDITION_ALWAYS, EVENT(CrossboneIsle_HopWest) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_L, CONDITION_ALWAYS, EVENT(CrossboneIsle_RideLogLWest) },
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_L, CONDITION_ALWAYS, EVENT(CrossboneIsle_RideLogLEast) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_L, CONDITION_ALWAYS, EVENT(CrossboneIsle_HopNorth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_L, CONDITION_ALWAYS, EVENT(CrossboneIsle_HopSouth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_G, CONDITION_ALWAYS, EVENT(CrossboneIsle_RideLogGWest) },
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_G, CONDITION_ALWAYS, EVENT(CrossboneIsle_RideLogGEast) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_G, CONDITION_ALWAYS, EVENT(CrossboneIsle_HopNorth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_G, CONDITION_ALWAYS, EVENT(CrossboneIsle_HopSouth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_I, CONDITION_ALWAYS, EVENT(CrossboneIsle_RideLogIWest) },
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_I, CONDITION_ALWAYS, EVENT(CrossboneIsle_RideLogIEast) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_I, CONDITION_ALWAYS, EVENT(CrossboneIsle_HopNorth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_I, CONDITION_ALWAYS, EVENT(CrossboneIsle_HopSouth) },
    { TRIGGER_STEP, CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_A, CONDITION_ALWAYS, EVENT(CrossboneIsle_StepOnLogA) },
    { TRIGGER_STEP, CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_C, CONDITION_ALWAYS, EVENT(CrossboneIsle_StepOnLogC) },
    { TRIGGER_STEP, CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_D, CONDITION_ALWAYS, EVENT(CrossboneIsle_StepOnLogD) },
    { TRIGGER_STEP, CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_E, CONDITION_ALWAYS, EVENT(CrossboneIsle_StepOnLogE) },
    { TRIGGER_STEP, CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_F, CONDITION_ALWAYS, EVENT(CrossboneIsle_StepOnLogF) },
    { TRIGGER_STEP, CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_H, CONDITION_ALWAYS, EVENT(CrossboneIsle_StepOnLogH) },
    { TRIGGER_STEP, CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_J, CONDITION_ALWAYS, EVENT(CrossboneIsle_StepOnLogJ) },
    { TRIGGER_STEP, CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_K, CONDITION_ALWAYS, EVENT(CrossboneIsle_StepOnLogK) },
    { TRIGGER_STEP, CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_B, CONDITION_ALWAYS, EVENT(CrossboneIsle_StepOnLogB) },
    { TRIGGER_STEP, CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_L, CONDITION_ALWAYS, EVENT(CrossboneIsle_StepOnLogL) },
    { TRIGGER_STEP, CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_G, CONDITION_ALWAYS, EVENT(CrossboneIsle_StepOnLogG) },
    { TRIGGER_STEP, CELL_BEHAVIOR_CROSSBONE_ISLE_LOG_I, CONDITION_ALWAYS, EVENT(CrossboneIsle_StepOnLogI) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), TREASURE_SPOT_CROSSBONE_ISLE_FLOATING_LOGS_999_COINS, FLAG_CROSSBONE_ISLE_FLOATING_LOGS_999_COINS, FIND_COINS(999) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), TREASURE_SPOT_CROSSBONE_ISLE_FLOATING_LOGS_WATER_OF_LIFE, FLAG_CROSSBONE_ISLE_FLOATING_LOGS_WATER_OF_LIFE, FIND_ITEM(ITEM_WATER_OF_LIFE) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), TREASURE_SPOT_CROSSBONE_ISLE_FLOATING_LOGS_MURAMASA, FLAG_CROSSBONE_ISLE_FLOATING_LOGS_MURAMASA, FIND_ITEM(ITEM_MURAMASA) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), TREASURE_SPOT_CROSSBONE_ISLE_FLOATING_LOGS_SLEEP_BOMB, FLAG_CROSSBONE_ISLE_FLOATING_LOGS_SLEEP_BOMB, FIND_ITEM(ITEM_SLEEP_BOMB) },
    { TRIGGER_TABLE_END },
};
// clang-format on

struct Trigger gCrossboneIsle_PierTriggers[] = {
    {TRIGGER_EXIT, CROSSBONE_ISLE_PIER_TO_CROSSBONE_ISLE_NINTH_FLOOR,
     CONDITION_ALWAYS, CROSSBONE_ISLE_PIER_TO_CROSSBONE_ISLE_NINTH_FLOOR},
    {TRIGGER_EXIT, CROSSBONE_ISLE_PIER_TO_PIRATE_SHIP, CONDITION_ALWAYS,
     CROSSBONE_ISLE_PIER_TO_PIRATE_SHIP},
    {TRIGGER_TABLE_END},
};

struct Trigger gCrossboneIsle_PirateShipTriggers[] = {
    {TRIGGER_EXIT, PIRATE_SHIP_TO_CROSSBONE_ISLE_PIER, CONDITION_ALWAYS,
     PIRATE_SHIP_TO_CROSSBONE_ISLE_PIER},
    {TRIGGER_STEP, CELL_BEHAVIOR_PIRATE_SHIP_TOP_OF_WEST_STAIRS,
     FLAG_CROSSBONE_ISLE_DEADBEARD_DEFEATED,
     EVENT(CrossboneIsle_FightDeadbeard)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH),
     CELL_BEHAVIOR_PIRATE_SHIP_BESIDE_MIMIC, CONDITION_ALWAYS,
     EVENT(CrossboneIsle_HopSouth)},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), TREASURE_SPOT_PIRATE_SHIP_DEMON_MAIL,
     FLAG_PIRATE_SHIP_DEMON_MAIL, FIND_ITEM(ITEM_DEMON_MAIL)},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), TREASURE_SPOT_PIRATE_SHIP_MIMIC,
     FLAG_PIRATE_SHIP_MIMIC_BEATEN, FIND_MIMIC | 7},
    {TRIGGER_TABLE_END},
};

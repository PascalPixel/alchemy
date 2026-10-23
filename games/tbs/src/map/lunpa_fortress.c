/// @file
/// Lunpa Fortress, Dodonpa's fortress above Lunpa, where he holds Hammet for
/// ransom, and the cave below it. Four maps share this overlay and the area
/// name Lunpa Fortress:
///
/// - LunpaFortressGroundFloor (MapId_LunpaFortressGroundFloor): inside the
///   door from the Lunpa Fortress Gates. An entrance guard, two sentries, the
///   upper hall's guard and a patrolling guard keep watch; Dodonpa's men and
///   the people of the fortress talk, and chests and a barrel hold an Elixir,
///   a Lucky Medal, a Mythril Circlet, coins and a Vial.
/// - LunpaFortressUpperFloor (MapId_LunpaFortressUpperFloor): where Donpa
///   sleeps. A patrolling pair and a hall patrol keep watch, three guards and
///   then three more together fight the party as Brigands, the Cell Key hangs
///   here, and Dodonpa's family and the women he brought here talk. After the
///   rescue Donpa gives the party Tonic, a Mercury Djinni.
/// - LunpaFortressCave (MapId_LunpaFortressCave): the cave whose switches
///   and passage plants hide, with a stack of crates to push into its hole.
/// - LunpaFortressToadonpa (MapId_LunpaFortressToadonpa): Hammet's cell,
///   where Dodonpa frees Toadonpa on the party (EventScript_FindHammet) and
///   Donpa judges him after the battle (EventScript_DonpaJudgesDodonpa), which
///   sets FLAG_DODONPA_LOCKED_UP and changes all of Lunpa.
///
/// The getters choose by gGameState.mapId; LunpaFortressCave and
/// LunpaFortressToadonpa share one warp table. Every return through the Lunpa
/// Fortress Gates (lunpa.c) clears the fortress's puzzle and beaten-guard
/// flags, so the fortress starts over.
///
/// The guards are the fortress's mechanism. A guard on screen sees the leader
/// along his row or column, within six cells (LunpaFortress_CanGuardSeeLeader),
/// unless Cloak is on, and each guard has a watcher, a callback that the
/// warp destination the party arrives by starts
/// (LunpaFortress_InitializeGroundFloor, LunpaFortress_InitializeUpperFloor). A
/// watcher that sees the leader raises one of the TRIGGER_*_SPOTTED map events,
/// whose script has the guard challenge the party and throw it out to the gates
/// (LunpaFortress_GuardThrowsPartyOut, WARP_THROWN_OUT). A cloaked leader
/// within four cells of a patrolling guard makes the watcher request
/// CLOAK_NEAR_GUARD. The patrolling guards walk action scripts
/// (gFirstPatrolRoute and the others), stop while a menu, Psynergy or an item
/// is in use, and drag MAPGRID_LAYER_OVERHEAD along with them, shaking it by a
/// pixel every frame: in the dark corridors that layer holds the circles of
/// light the patrols carry, which flicker as they shake
/// (https://alchemy.pascalpixel.com/places/lunpa-fortress-upper-floor/map-1.png
/// shows the pair's two circles). Each has stretches of its rounds where
/// it cannot see the leader. On the upper floor the pair walk east and west
/// between 88 and 192 pixels across, one 192 and one 288 pixels down; each
/// sees a leader in his own column up to six rows away, or within a row of
/// his up to six columns away, whatever stands between them, but never one
/// in rows 5 to 7 from column 10 west, at the passage's top, or in columns 8
/// and 9 below row 22 (LunpaFortress_IsLeaderInPatrolSafeZone)
/// (https://alchemy.pascalpixel.com/places/lunpa-fortress-upper-floor/patrol-sight.png:
/// the west passage with the pair's rounds in yellow, every cell either
/// guard sees from some point of his round in red, and the cells they never
/// look at in green). The guards who fight instead pick one of four
/// challenges, by gLunpaFortressRandomPick, drawn on every arrival.
///
/// The Cell Key opens the cell doors of all four maps, facing each door
/// with it in the party, and each opened door is remembered in a map-section
/// temp flag from FLAG_MAPSEC_TEMP_50 (TOADONPA_DOOR_FLAG and the others). In
/// LunpaFortressCave, Whirlwind blows away the plants that hide two switches
/// and a passage; the switches show in the wall only while Reveal lasts, and
/// searching one opens its passage.
///
/// Map temp flags:
/// - FLAG_MAP_TEMP_14: a guard has spotted the party; FLAG_MAP_TEMP_15 is
///   set beside it and never read.
/// - FLAG_MAP_TEMP_17 and FLAG_MAP_TEMP_18: LunpaFortressCave's first and
///   second switches are still hidden by their plants.
/// - FLAG_MAP_TEMP_24 and FLAG_MAP_TEMP_25: the entrance guard or the
///   sentries have thrown the party out.
/// - FLAG_MAP_TEMP_26 and FLAG_MAP_TEMP_27: the startled old man has
///   promised to keep quiet, and has been startled.
/// - FLAG_MAP_TEMP_70: EventScript_FindHammet is under way.
///
/// Map-section temp flags: FLAG_MAPSEC_TEMP_01 once Donpa has judged Dodonpa,
/// FLAG_MAPSEC_TEMP_13 once his attendant has told of him, FLAG_MAPSEC_TEMP_14
/// once she has shown his reward, and FLAG_MAPSEC_TEMP_50 to
/// FLAG_MAPSEC_TEMP_5C for the opened cell doors.
///
/// Unused: four item pickups that set out an Arctic Blade, a Masamune, a
/// Silver Blade and a Shamshir on LunpaFortressGroundFloor's first four rows,
/// and copies of West Lunpa Cave's warps and object event templates, which
/// these maps never read.
///
/// Open questions: none.

#include "game_state.h"
#include "script_layer.h"
#include "unit/import.h"
#include "unit/map/lunpa_fortress.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "global_cells.h"
#include "edition.h"
#include "lunpa_fortress_staging.h"
#include "script.h"
#include "map_ids.h"
#include "tbs_edition.h"
#include "map_grid_scroll.h"
#include "text/msg_ids.h"
#include "types.h"
#include "ability_ids.h"
#include "map_tables.h"
#include "item_ids.h"
#include "objcmd.h"
#include "flags.h"
#include "djinn_ids.h"
#include "songs.h"
#include "audio_engine_symbols.h"
#include "fixed_math.h"
#include "io_reg.h"

/// LunpaFortressGroundFloor's people, numbered in the order of
/// gLunpaFortressGroundFloorObjectEventTemplates from LOCALID_FIRST_PLACED,
/// past its first four rows (in a chest's sprite, at the map's origin; see
/// FIRST_ITEM_DISPLAY_LOCALID).
enum {
  /// Dodonpa's men, here and upstairs, wear the sprite of a man with a mop
  /// of orange hair (SPRITE_MAN_WITH_ORANGE_HAIR).
  GROUND_FLOOR_ENTRANCE_GUARD = LOCALID_FIRST_PLACED + 4,
  GROUND_FLOOR_FIRST_SENTRY,
  /// A woman with fair hair (SPRITE_WOMAN_WITH_FAIR_HAIR), wondering where
  /// Dodonpa has gone. "Where could Dodonpa be? He's not in his
  /// room, but he couldn't have left."
  GROUND_FLOOR_WOMAN_SEEKING_DODONPA,
  /// One of Dodonpa's men (the guards' sprite), feasting on Kalay's ransom.
  /// "Whee! Thanks to Hammet we can enjoy food and drink all day long!"
  GROUND_FLOOR_FEASTING_MAN,
  /// One of Dodonpa's men, drinking and boasting of Hammet. "He's a money tree
  /// for us, hic!"
  GROUND_FLOOR_BOASTING_MAN,
  GROUND_FLOOR_UPPER_HALL_GUARD,
  GROUND_FLOOR_PATROL_GUARD,
  /// A man with brown hair (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR), talking of
  /// Donpa's
  /// poor health. "Donpa is supposed to be old. I wonder
  /// what he's fighting with in the fortress."
  GROUND_FLOOR_MAN_WONDERING_ABOUT_DONPA,
  /// A bent old man with a cane (SPRITE_OLD_MAN_WITH_CANE);
  /// EventScript_TalkToStartledOldMan.
  GROUND_FLOOR_STARTLED_OLD_MAN,
  GROUND_FLOOR_SECOND_SENTRY
};

/// LunpaFortressUpperFloor's people and the Cell Key, numbered in the order of
/// gLunpaFortressUpperFloorObjectEventTemplates from LOCALID_FIRST_PLACED.
enum {
  UPPER_FLOOR_CELL_KEY = LOCALID_FIRST_PLACED,
  UPPER_FLOOR_FIRST_PATROL,
  UPPER_FLOOR_SECOND_PATROL,
  UPPER_FLOOR_HALL_GUARD,
  /// The three guards who fight together.
  UPPER_FLOOR_GUARD_A,
  UPPER_FLOOR_GUARD_B,
  UPPER_FLOOR_GUARD_C,
  UPPER_FLOOR_WEST_GUARD,
  UPPER_FLOOR_EAST_GUARD,
  UPPER_FLOOR_HALL_PATROL,
  /// Dodonpa's wife, a woman in a green dress (SPRITE_WOMAN_IN_GREEN_DRESS),
  /// the "Mama" of his children, who LunpaFortressGroundFloor's
  /// people say is looking for him. "Do you know where Dodonpa is?"
  UPPER_FLOOR_DODONPAS_WIFE,
  /// Donpappa, a small boy (SPRITE_BOY_WITH_FAIR_HAIR), Dodonpa's son, as he
  /// names himself to Mind Read: "When my Papa's gone, I, Donpappa, shall
  /// protect Lunpa myself."
  UPPER_FLOOR_DONPAPPA,
  /// Dodonpa's daughter, a little girl (SPRITE_GIRL_WITH_PIGTAILS). "Papa
  /// always gets friendly with the ladies when Mama's not lookin'."
  UPPER_FLOOR_DODONPAS_DAUGHTER,
  /// An old woman (SPRITE_OLD_WOMAN) who pities Donpappa. "Dodonpa's child is
  /// so well behaved, I can't believe he's his father's son!"
  UPPER_FLOOR_OLD_WOMAN_PRAISING_DONPAPPA,
  /// A woman with fair hair (SPRITE_WOMAN_WITH_FAIR_HAIR) who fears for Donpa.
  /// "Poor, poor Donpa. Dodonpa's behavior is driving him
  /// into despair."
  UPPER_FLOOR_WOMAN_FEARING_FOR_DONPA,
  /// A woman with fair hair (SPRITE_WOMAN_WITH_FAIR_HAIR), one of the women
  /// Dodonpa has brought to the fortress. "Nearly all of
  /// these women were brought here against their will, like me."
  UPPER_FLOOR_CAPTIVE_WOMAN,
  UPPER_FLOOR_DONPA, ///< SPRITE_DONPA
  /// Donpa's attendant, a woman in a green dress (SPRITE_WOMAN_IN_GREEN_DRESS).
  UPPER_FLOOR_ATTENDANT,
  UPPER_FLOOR_TONIC ///< Tonic, a Mercury Djinni, Donpa's reward
};

/// LunpaFortressCave's covers and crates, numbered in the order of
/// gLunpaFortressCaveObjectEventTemplates from LOCALID_FIRST_PLACED.
enum {
  CAVE_FIRST_SWITCH_COVER = LOCALID_FIRST_PLACED,
  CAVE_PASSAGE_COVER,
  CAVE_SECOND_SWITCH_COVER,
  /// An invisible solid object event (SPRITE_BLANK, OBJ_EVENT_SOLID) at (504,
  /// 168) that makes the jar the layout draws there, in the cell behind the
  /// cave's eastern door, stop the leader: the layout's metatile behavior lets
  /// him walk onto the jar, and with this object event he stops 14 pixels short
  /// of it. A blank sprite draws only its shadow, which
  /// LunpaFortress_InitializeCave turns off.
  CAVE_JAR_BLOCKER,
  /// A stack of wooden crates (the sprite of the crates at the Bilibin
  /// Barricade and on Tolbi Docks), pushed or moved with Move onto its hole.
  CAVE_CRATES
};

/// LunpaFortressToadonpa's people, numbered in the order of
/// gLunpaFortressToadonpaObjectEventTemplates from LOCALID_FIRST_PLACED; its
/// rows for Ivan, Mia and Garet keep the party's own slots.
enum {
  /// Three invisible solid object events (SPRITE_BLANK, OBJ_EVENT_SOLID) at
  /// (56, 72),
  /// (136, 72) and (216, 72), in line with the cell doors
  /// gLunpaFortressToadonpaDoorCells lists third, second and first, that make
  /// the jar, chest and jar the layout draws in those cells stop the leader.
  /// The layout's metatile behaviors let him walk onto the two jars; the
  /// chest's own cells already stop him, so the middle object event changes
  /// nothing. Map_Initialize
  /// turns off the shadows their blank sprites would draw and lays them
  /// underfoot.
  TOADONPA_WEST_JAR_BLOCKER = LOCALID_FIRST_PLACED,
  TOADONPA_CHEST_BLOCKER,
  TOADONPA_EAST_JAR_BLOCKER,
  TOADONPA_BOSS,
  TOADONPA_HAMMET,
  TOADONPA_DODONPA, ///< SPRITE_DODONPA
  TOADONPA_DONPA,
  /// A sprite drawn underfoot with Toadonpa and moving with it.
  TOADONPA_BOSS_PART,
  /// One of Dodonpa's men, a man with brown hair
  /// (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR),
  /// placed by Dodonpa's cell after Donpa's judgment.
  /// "The merchant from Kalay is in here somewhere." Read with Mind Read:
  /// "Actually, Dodonpa's in here, but only a few people know that."
  TOADONPA_CELL_GUARD,
  /// An invisible object event that stands before Dodonpa's cell after Donpa's
  /// judgment; talking to it or reading its mind reaches Dodonpa.
  TOADONPA_DODONPA_STAND_IN
};

/// Triggers: the cell doors count from TRIGGER_DOOR_FIRST in each map, and a
/// watching guard raises one of the TRIGGER_*_SPOTTED triggers.
enum {
  TRIGGER_LOCKED_DOOR = 99,
  TRIGGER_SEALED_DOOR = 98,
  TRIGGER_DOOR_FIRST = 40,
  TRIGGER_PATROL_PAIR_SPOTTED = 91,
  TRIGGER_HALL_PATROL_SPOTTED,
  TRIGGER_GROUND_FLOOR_PATROL_SPOTTED,
  TRIGGER_UPPER_HALL_SPOTTED,
  TRIGGER_ENTRANCE_GUARD_SPOTTED,
  TRIGGER_SENTRIES_SPOTTED
};

/// Reveal's phases: the hidden things show, and hide again.

/// Where a party caught by a guard is sent: the Lunpa Fortress Gates.
enum { WARP_THROWN_OUT = 60 };

/// What a guard close to the cloaked leader requests of Cloak.
enum { CLOAK_NEAR_GUARD = 0x2000 | ABILITY_CLOAK };

/// The first of the flags that remember which cell doors the Cell Key opened,
/// per map: one for each door trigger from TRIGGER_DOOR_FIRST.
enum {
  TOADONPA_DOOR_FLAG = FLAG_MAPSEC_TEMP_50,
  CAVE_DOOR_FLAG = FLAG_MAPSEC_TEMP_55,
  UPPER_FLOOR_DOOR_FLAG = FLAG_MAPSEC_TEMP_58,
  GROUND_FLOOR_DOOR_FLAG = FLAG_MAPSEC_TEMP_5A
};

/// The map grid layers the dark passages show
/// (Engine_MapGridSetShownLayers): BG1 to BG3, BG1 being
/// MAPGRID_LAYER_OVERHEAD and its circles of light.
#define LUNPA_FORTRESS_PASSAGE_LAYERS                                          \
  (DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON)
/// The map grid layers the lit rooms show: BG2 and BG3, without the overhead
/// layer.
#define LUNPA_FORTRESS_ROOM_LAYERS (DISPCNT_BG2_ON | DISPCNT_BG3_ON)

/// The paired patrol's layer origin across: 136 pixels, in 16.16 fixed point.
// The native paired patrol computes this layer offset in floating point.
#define PAIRED_PATROL_LAYER_ORIGIN_X (136.0 * 65536)

static s32 LunpaFortress_TryCollectDisplayedItem(s32 localId, s32 itemId);
static void LunpaFortress_CheckSecondEntranceSentry(void);
static s32 LunpaFortress_CanGuardSeeLeader(s32 localId);
static s32 LunpaFortress_IsLeaderNearGuard(s32 localId);
static s32 LunpaFortress_IsGuardNearViewCenter(s32 localId);
static void LunpaFortress_GuardThrowsPartyOut(s32 localId);
static void LunpaFortress_ActOutIntruderDialogue(s32 localId);
static void LunpaFortress_ActOutWhosThereDialogue(s32 localId);
static void LunpaFortress_ActOutGuardShiftDialogue(s32 localId);
static void LunpaFortress_ActOutScoundrelDialogue(s32 localId);
static void LunpaFortress_InitializeGroundFloor(void);
static void LunpaFortress_InitializeUpperFloor(void);
static void LunpaFortress_InitializeCave(void);
static void LunpaFortress_RestoreGroundFloorDoors(void);
static void LunpaFortress_RestoreUpperFloorDoors(void);
static void LunpaFortress_RestoreCaveDoors(void);
static void LunpaFortress_RestoreToadonpaDoors(void);

#include "common/object_staged_object_event.h"

const struct MapWarpDest *Map_GetWarpDests(void) {
  s16 mapId = gGameState.mapId;

  if (mapId == (s32)&MapId_LunpaFortressGroundFloor) {
    return gLunpaFortressGroundFloorWarpDests;
  }
  if (mapId == (s32)&MapId_LunpaFortressUpperFloor) {
    return gLunpaFortressUpperFloorWarpDests;
  }
  if (mapId == (s32)&MapId_LunpaFortressCave) {
    return gLunpaFortressCaveWarpDests;
  }
  return gLunpaFortressWarpDestsOther;
}

const struct MapWarpRegion *Map_GetWarpRegions(void) { return NULL; }

const u32 *Map_GetWarps(void) {
  s16 mapId = gGameState.mapId;

  if (mapId == (s32)&MapId_LunpaFortressUpperFloor) {
    return gLunpaFortressUpperFloorWarps;
  }
  if (mapId == (s32)&MapId_LunpaFortressCave ||
      mapId == (s32)&MapId_LunpaFortressToadonpa) {
    return gLunpaFortressCaveAndToadonpaWarps;
  }
  return gLunpaFortressWarpsOther;
}

/// The guards, the other people and the objects of each map.
const struct ObjectEventTemplate *Map_GetObjectEventTemplates(void) {
  s16 mapId = gGameState.mapId;

  // Unused: this overlay never runs for West Lunpa Cave, which has its own.
  if (mapId == (s32)&MapId_WestLunpaCave) {
    return gLunpaFortressUnusedWestLunpaCaveObjectEventTemplates;
  }
  if (mapId == (s32)&MapId_LunpaFortressCave) {
    return gLunpaFortressCaveObjectEventTemplates;
  }
  if (mapId == (s32)&MapId_LunpaFortressUpperFloor) {
    return gLunpaFortressUpperFloorObjectEventTemplates;
  }
  if (mapId == (s32)&MapId_LunpaFortressGroundFloor) {
    return gLunpaFortressGroundFloorObjectEventTemplates;
  }
  if (mapId == (s32)&MapId_LunpaFortressToadonpa) {
    return gLunpaFortressToadonpaObjectEventTemplates;
  }
  return gLunpaFortressObjectEventTemplatesOther;
}

/// What the party can talk to, search and use Psynergy on, by map.
const struct MapEvent *Map_GetEvents(void) {
  s16 mapId = gGameState.mapId;

  if (mapId == (s32)&MapId_LunpaFortressGroundFloor) {
    return gLunpaFortressGroundFloorEvents;
  }
  if (mapId == (s32)&MapId_LunpaFortressUpperFloor) {
    return gLunpaFortressUpperFloorEvents;
  }
  if (mapId == (s32)&MapId_LunpaFortressCave) {
    return gLunpaFortressCaveEvents;
  }
  return gLunpaFortressEventsOther;
}

/// LunpaFortressCave: the crates sink into the floor, and the party can walk
/// over them.
static void LunpaFortress_LowerCrates(void) {
  struct ObjectEvent *cratesObjectEvent;

  MapGrid_CopyCellAttributes(15, 20, 1, 1, 15, 22);
  MapGrid_CopyCellAttributes(17, 23, 1, 3, 15, 23);
  cratesObjectEvent = ObjectEvent_GetById(CAVE_CRATES);
  if (cratesObjectEvent != 0) {
    Engine_ObjectEventSetSpriteFlags(cratesObjectEvent, 0);
    cratesObjectEvent->motionFlags = 0;
    cratesObjectEvent->priorityFlags = OBJ_EVENT_PRIORITY_UNDERFOOT;
  }
}

/// LunpaFortressCave, coord event 70: pushing the crates slides them one cell
/// along.
static void EventScript_PushCrates(void) { PushedObject_AdvancePair(); }

/// LunpaFortressCave, coord event 50: the crates, pushed onto their hole, slide
/// in and sink.
static void EventScript_CratesSink(void) {
  OBJECT_SPEED(CAVE_CRATES, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetDestination(CAVE_CRATES, 248, 376);
  Engine_ObjectEventWaitForMove(CAVE_CRATES);
  SE(SE_215);
  WAIT(60);
  LunpaFortress_LowerCrates();
  FLAG_SET(FLAG_LUNPA_FORTRESS_CRATES_MOVED);
}

/// LunpaFortressCave, Move on the crates: once they are past row 22, over their
/// hole, the cave shakes and they sink.
static void EventScript_MoveCrates(void) {
  struct ObjectEvent *cratesObjectEvent;

  cratesObjectEvent = ObjectEvent_Get(CAVE_CRATES);
  if ((cratesObjectEvent->z.fixed >> 20) > 22) {
    MapGrid_SetShake(PIXELS(4), PIXELS(4), Q16_ONE);
    MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
    SE(SE_144);
    MapGrid_CopyCellAttributes(15, 20, 1, 1, 15, 22);
    MapGrid_CopyCellAttributes(17, 23, 1, 3, 15, 23);
    cratesObjectEvent = ObjectEvent_Get(CAVE_CRATES);
    if (cratesObjectEvent != NULL) {
      Engine_ObjectEventSetSpriteFlags(cratesObjectEvent, 0);
      cratesObjectEvent->priorityFlags = OBJ_EVENT_PRIORITY_UNDERFOOT;
    }
    FLAG_SET(FLAG_LUNPA_FORTRESS_CRATES_MOVED);
  }
}

/// The crates stay sunk in their hole.
static void LunpaFortress_RestoreLoweredCrates(void) {
  OBJECT_POSITION_SET(CAVE_CRATES, 248, 376);
  LunpaFortress_LowerCrates();
}

/// Draws one of LunpaFortressToadonpa's cell doors open.
static void LunpaFortress_OpenToadonpaDoor(s32 doorIndex) {
  s32 doorCellX = gLunpaFortressToadonpaDoorCells[doorIndex * 2];
  s32 doorCellY = gLunpaFortressToadonpaDoorCells[doorIndex * 2 + 1];

  MapGrid_CopyCells(0, 0x4d, 1, 3, doorCellX, doorCellY);
  MapGrid_CopyCells(1, 0x4d, 1, 1, doorCellX + 1, doorCellY);
  MapGrid_CopyCells(doorCellX, doorCellY - 0x30, 1, 1, doorCellX,
                    doorCellY - 0x2e);
}

/// LunpaFortressToadonpa: facing a cell door with the Cell Key opens it with a
/// rumble. After Donpa's judgment the fifth, Dodonpa's cell, stays shut.
static void EventScript_UnlockToadonpaDoor(void) {
  struct EventWork *fieldWork;
  s32 doorTrigger;
  s32 doorIndex;

  fieldWork = gFieldWork;
  if (Engine_PartyInventoryFindOwner(ITEM_CELL_KEY) != -1) {
    doorTrigger = fieldWork->coordEvent;
    doorIndex = doorTrigger - TRIGGER_DOOR_FIRST;
    if (FLAG_CHECK(FLAG_DODONPA_LOCKED_UP) == 0 || doorIndex != 4) {
      LunpaFortress_OpenToadonpaDoor(doorIndex);
      SE(SE_GATE_MOVE);
      MapGrid_SetShake(PIXELS(3), PIXELS(3), Q16_ONE);
      MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
      FLAG_SET(doorTrigger + (TOADONPA_DOOR_FLAG - TRIGGER_DOOR_FIRST));
    }
  }
}

/// Draws one of LunpaFortressCave's cell doors open and lets the party through;
/// the middle door opens two cells.
static void LunpaFortress_OpenCaveDoor(s32 doorIndex) {
  s32 doorCellX;
  s32 doorCellY;
  s32 collisionCellY;

  doorCellX = gLunpaFortressCaveDoorCells[doorIndex * 2];
  doorCellY = gLunpaFortressCaveDoorCells[doorIndex * 2 + 1];
  MapGrid_CopyCells(0, 77, 1, 3, doorCellX, doorCellY);
  MapGrid_CopyCells(1, 77, 1, 1, doorCellX + 1, doorCellY);
  collisionCellY = doorCellY - 44;
  MapGrid_CopyCellAttributes(doorCellX, doorCellY - 45, 1, 1, doorCellX,
                             collisionCellY);
  if (doorIndex == 1)
    MapGrid_CopyCellAttributes(doorCellX, collisionCellY, 1, 1, doorCellX,
                               doorCellY - 43);
}

/// LunpaFortressCave: facing a cell door with the Cell Key opens it with a
/// rumble.
static void EventScript_UnlockCaveDoor(void) {
  struct EventWork *fieldWork;
  s32 doorTrigger;

  fieldWork = gFieldWork;
  if (Engine_PartyInventoryFindOwner(ITEM_CELL_KEY) != -1) {
    doorTrigger = fieldWork->coordEvent;
    LunpaFortress_OpenCaveDoor(doorTrigger - TRIGGER_DOOR_FIRST);
    SE(SE_GATE_MOVE);
    MapGrid_SetShake(PIXELS(3), PIXELS(3), Q16_ONE);
    MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
    FLAG_SET(doorTrigger + (CAVE_DOOR_FLAG - TRIGGER_DOOR_FIRST));
  }
}

/// Draws one of LunpaFortressUpperFloor's cell doors open.
static void LunpaFortress_OpenUpperFloorDoor(s32 doorIndex) {
  s32 doorCellX = gLunpaFortressUpperFloorDoorCells[doorIndex * 2];
  s32 doorCellY = gLunpaFortressUpperFloorDoorCells[doorIndex * 2 + 1];

  MapGrid_CopyCells(0x37, 0x79, 1, 3, doorCellX, doorCellY);
  MapGrid_CopyCells(0x38, 0x79, 1, 1, doorCellX + 1, doorCellY);
  MapGrid_CopyCells(doorCellX, doorCellY - 0x3f, 1, 1, doorCellX,
                    doorCellY - 0x3e);
}

/// LunpaFortressUpperFloor: facing a cell door with the Cell Key opens it with
/// a rumble.
static void EventScript_UnlockUpperFloorDoor(void) {
  struct EventWork *fieldWork;
  s32 doorTrigger;

  fieldWork = gFieldWork;
  if (Engine_PartyInventoryFindOwner(ITEM_CELL_KEY) != -1) {
    doorTrigger = fieldWork->coordEvent;
    LunpaFortress_OpenUpperFloorDoor(doorTrigger - TRIGGER_DOOR_FIRST);
    SE(SE_GATE_MOVE);
    MapGrid_SetShake(PIXELS(3), PIXELS(3), Q16_ONE);
    MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
    FLAG_SET(doorTrigger + (UPPER_FLOOR_DOOR_FLAG - TRIGGER_DOOR_FIRST));
  }
}

/// Draws one of LunpaFortressGroundFloor's cell doors open and lets the party
/// through.
static void LunpaFortress_OpenGroundFloorDoor(s32 doorIndex) {
  // BUG: the third door's trigger and flag (GROUND_FLOOR_DOOR_FLAG + 2) ask for
  // index 2, past the two doors gLunpaFortressGroundFloorDoorCells holds.
  s32 doorCellX = gLunpaFortressGroundFloorDoorCells[doorIndex * 2];
  s32 doorCellY = gLunpaFortressGroundFloorDoorCells[doorIndex * 2 + 1];

  MapGrid_CopyCells(1, 0x50, 1, 3, doorCellX, doorCellY);
  MapGrid_CopyCells(2, 0x50, 1, 1, doorCellX + 1, doorCellY);
  Engine_MapGridCopyCellAttributes(doorCellX, doorCellY - 0x3f, 1, 1, doorCellX,
                                   doorCellY - 0x3e);
}

/// LunpaFortressGroundFloor: facing a cell door with the Cell Key opens it with
/// a rumble.
static void EventScript_UnlockGroundFloorDoor(void) {
  struct EventWork *fieldWork;
  s32 doorTrigger;

  fieldWork = gFieldWork;
  if (Engine_PartyInventoryFindOwner(ITEM_CELL_KEY) != -1) {
    doorTrigger = fieldWork->coordEvent;
    LunpaFortress_OpenGroundFloorDoor(doorTrigger - TRIGGER_DOOR_FIRST);
    SE(SE_GATE_MOVE);
    MapGrid_SetShake(PIXELS(3), PIXELS(3), Q16_ONE);
    MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
    FLAG_SET(doorTrigger + (GROUND_FLOOR_DOOR_FLAG - TRIGGER_DOOR_FIRST));
  }
}

/// Lunpa Fortress, every map: searching a cell door without the Cell Key,
/// it stays shut.
/// "The door is tightly locked."
static void EventScript_SearchLockedDoor(void) {
  if (Engine_PartyInventoryFindOwner(ITEM_CELL_KEY) == -1) {
    Engine_MessageShowCentered(MsgFieldDoorTightlyLocked, 1);
  }
}

/// LunpaFortressCave: searching the door no key opens.
static void EventScript_SearchSealedDoor(void) {
  Engine_MessageShowCentered(MsgFieldDoorTightlyLocked, 1);
}

/// The four object events from FIRST_ITEM_DISPLAY_LOCALID that the unused item
/// pickups below show their items on, in a map they do not name. XXX: likely
/// LunpaFortressGroundFloor's first four rows (in a chest's sprite, at the
/// map's origin), because nothing else uses them.
enum { FIRST_ITEM_DISPLAY_LOCALID = LOCALID_FIRST_PLACED };

/// Unused: nothing calls this pickup or the three after it. Together they
/// would set out an Arctic Blade, a Masamune, a Silver Blade and a Shamshir on
/// the four display object events, each taken once
/// (FLAG_LUNPA_FORTRESS_ARCTIC_BLADE to FLAG_LUNPA_FORTRESS_SHAMSHIR); this one
/// takes the Arctic Blade.
static void UNUSED LunpaFortress_UnusedTakeArcticBlade(void) {
  if (LunpaFortress_TryCollectDisplayedItem(FIRST_ITEM_DISPLAY_LOCALID,
                                            ITEM_ARCTIC_BLADE) != 0) {
    Engine_GameFlagSet(FLAG_LUNPA_FORTRESS_ARCTIC_BLADE);
  }
}

/// Unused: the Masamune, on the second display object event.
static void UNUSED LunpaFortress_UnusedTakeMasamune(void) {
  if (LunpaFortress_TryCollectDisplayedItem(FIRST_ITEM_DISPLAY_LOCALID + 1,
                                            ITEM_MASAMUNE) != 0) {
    Engine_GameFlagSet(FLAG_LUNPA_FORTRESS_MASAMUNE);
  }
}

/// Unused: the Silver Blade, on the third display object event.
static void UNUSED LunpaFortress_UnusedTakeSilverBlade(void) {
  if (LunpaFortress_TryCollectDisplayedItem(FIRST_ITEM_DISPLAY_LOCALID + 2,
                                            ITEM_SILVER_BLADE) != 0) {
    Engine_GameFlagSet(FLAG_LUNPA_FORTRESS_SILVER_BLADE);
  }
}

/// Unused: the Shamshir, on the fourth display object event.
static void UNUSED LunpaFortress_UnusedTakeShamshir(void) {
  if (LunpaFortress_TryCollectDisplayedItem(FIRST_ITEM_DISPLAY_LOCALID + 3,
                                            ITEM_SHAMSHIR) != 0) {
    Engine_GameFlagSet(FLAG_LUNPA_FORTRESS_SHAMSHIR);
  }
}

/// Unused: called only by the four pickups above. Gives the item an object
/// event shows, with its effect; with no room for it, the object event shows
/// the item is left behind. Returns whether it was given.
static s32 LunpaFortress_TryCollectDisplayedItem(s32 localId, s32 itemId) {
  s32 itemGiven = 0;
  s32 itemEffect;

  SCRIPT_START();
  itemEffect =
      Engine_FieldEffectPlayCueAndStartEmitterOnTarget(0, localId, itemId);
  if (Engine_PartyGiveItem(itemId, 0) != -1) {
    OBJECT_ANIMATION(localId, 2);
    itemGiven = 1;
  } else {
    SE(SE_125);
    OBJECT_ANIMATION(localId, 5);
  }
  Engine_ObjectDispatchRelease((struct ObjectEvent *)itemEffect);
  SCRIPT_END();
  return itemGiven;
}

static void UNUSED LunpaFortress_DoNothingA(void) {}

static void UNUSED LunpaFortress_DoNothingB(void) {}

static void UNUSED LunpaFortress_DoNothingC(void) {}

static void UNUSED LunpaFortress_DoNothingD(void) {}

/// LunpaFortressUpperFloor: the leader steps up and takes the Cell Key. Catch
/// on the key hanging there, or talking to it, starts this.
static void EventScript_TakeCellKey(void) {
  Engine_ObjectEventWalkTo(LOCALID_PARTY_LEADER, 264, 792);
  Engine_ObjectEventWaitForMove(LOCALID_PARTY_LEADER);
  Engine_ObjectEventFaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 0);
  WAIT(10);
  OBJECT_ANIMATION(LOCALID_PARTY_LEADER, ANIMATION_STAND);
  Engine_ItemShowFound(ITEM_CELL_KEY, 3);
  OBJECT_ANIMATION(LOCALID_PARTY_LEADER, ANIMATION_STAND);
  Engine_PartyGiveItem(ITEM_CELL_KEY, 0);
  Engine_GameFlagSet(FLAG_LUNPA_FORTRESS_CELL_KEY);
  Engine_ObjectEventSetPosition(UPPER_FLOOR_CELL_KEY, 0, 0);
}
/// Where the patrolling pair cannot see the leader: the west end of their
/// corridor and the cells below its gap.
static s32 LunpaFortress_IsLeaderInPatrolSafeZone(void) {
  struct ObjectEvent *leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  s32 leaderFixedZ = leader->z.fixed;
  s32 leaderFixedX;
  s32 leaderTileZ, leaderTileX;

  if (leaderFixedZ < 0) {
    leaderFixedZ += 0xfffff;
  }
  leaderFixedX = leader->x.fixed;
  leaderTileZ = leaderFixedZ >> 20;
  if (leaderFixedX < 0) {
    leaderFixedX += 0xfffff;
  }
  leaderTileX = leaderFixedX >> 20;
  if ((u32)(leaderTileZ - 5) <= 2 && leaderTileX <= 10) {
    return 1;
  }
  if ((u32)(leaderTileX - 8) <= 1 && leaderTileZ > 22) {
    return 1;
  }
  return 0;
}

// UB: these patrols formerly reached gFieldWork through gMapGridWork[19] and
// treated its request members as a halfword array. Named owner access adds
// eight bytes to each of the three patrol callbacks in the EN trial.
#ifdef UBFIX
#define PATROL_PSYNERGY_REQUEST(work) ((work)->psynergy)
#define PATROL_ITEM_REQUEST(work) ((work)->item)
#define PATROL_RAISED_TRIGGER(work) ((work)->raisedEvent)
#else
#define PATROL_PSYNERGY_REQUEST(work) ((work)[191])
#define PATROL_ITEM_REQUEST(work) ((work)[192])
#define PATROL_RAISED_TRIGGER(work) ((work)[193])
#endif

/// Every frame in LunpaFortressUpperFloor: the pair patrolling the west
/// corridor. They stand still while FLAG_MENU_OPEN is set or Psynergy or an
/// item is in use; otherwise they walk, with MAPGRID_LAYER_OVERHEAD shaking and
/// following the first. Close to a cloaked leader they request
/// CLOAK_NEAR_GUARD; seeing an uncloaked leader, they raise
/// TRIGGER_PATROL_PAIR_SPOTTED.
static void LunpaFortress_UpdatePairedGuardDetection(void) {
  struct ObjectEvent *firstGuard = ObjectEvent_Get(UPPER_FLOOR_FIRST_PATROL);
  struct ObjectEvent *secondGuard = ObjectEvent_Get(UPPER_FLOOR_SECOND_PATROL);
  struct MapGridLayerScroll *patrolLayer =
      &((struct MapGridScrollWork *)gMapGridWork[0])
           ->layers[MAPGRID_LAYER_OVERHEAD];
#ifdef UBFIX
  struct FieldWork *fieldWork = (struct FieldWork *)gFieldWork;
#else
  s16 *fieldWork = *(s16 **)&gMapGridWork[19];
#endif

  if (gFrameCount & 1) {
    patrolLayer->speedX = 1;
    patrolLayer->speedY = 1;
  } else {
    patrolLayer->speedX = -1;
    patrolLayer->speedY = -1;
  }
  if (FLAG_CHECK(FLAG_MENU_OPEN) || PATROL_PSYNERGY_REQUEST(fieldWork) != 0 ||
      PATROL_ITEM_REQUEST(fieldWork) != 0) {
    firstGuard->paused = 1;
    secondGuard->paused = 1;
  } else if (!FLAG_CHECK(FLAG_MAP_TEMP_14)) {
    firstGuard->paused = 0;
    secondGuard->paused = 0;
    if (!FLAG_CHECK(FLAG_MAP_TEMP_14) && firstGuard->paused == 0) {
      patrolLayer->phaseX = PAIRED_PATROL_LAYER_ORIGIN_X - firstGuard->x.fixed;
    }
    if (!LunpaFortress_IsLeaderInPatrolSafeZone()) {
      if (gGameState.cloaked != 0) {
        if (LunpaFortress_IsLeaderNearGuard(UPPER_FLOOR_FIRST_PATROL) &&
            gGameState.cloaked != 0) {
          SetFieldWorkValue(&PATROL_PSYNERGY_REQUEST(fieldWork),
                            CLOAK_NEAR_GUARD);
          return;
        }
        if (LunpaFortress_IsLeaderNearGuard(UPPER_FLOOR_SECOND_PATROL) &&
            gGameState.cloaked != 0) {
          SetFieldWorkValue(&PATROL_PSYNERGY_REQUEST(fieldWork),
                            CLOAK_NEAR_GUARD);
          return;
        }
      }
      if (gGameState.cloaked == 0) {
        if (LunpaFortress_CanGuardSeeLeader(UPPER_FLOOR_FIRST_PATROL)) {
          FLAG_SET(FLAG_MAP_TEMP_15);
          FLAG_SET(FLAG_MAP_TEMP_14);
        }
        if (LunpaFortress_CanGuardSeeLeader(UPPER_FLOOR_SECOND_PATROL)) {
          FLAG_SET(FLAG_MAP_TEMP_15);
          FLAG_SET(FLAG_MAP_TEMP_14);
        }
      }
      if (FLAG_CHECK(FLAG_MAP_TEMP_14)) {
        SetFieldWorkValue(&PATROL_RAISED_TRIGGER(fieldWork),
                          TRIGGER_PATROL_PAIR_SPOTTED);
      }
    }
  }
}

/// LunpaFortressUpperFloor, raised when the patrolling pair spots the leader:
/// the first stops and stares, and the second throws the party out.
static void EventScript_PatrolPairThrowsPartyOut(void) {
  SCRIPT_START();
  Engine_ObjectEventStop(UPPER_FLOOR_FIRST_PATROL);
  Engine_ObjectEventSetDestinationOffset(UPPER_FLOOR_FIRST_PATROL, 0, 0);
  OBJECT_ANIMATION(UPPER_FLOOR_FIRST_PATROL, 0);
  OBJECT_FACE(UPPER_FLOOR_FIRST_PATROL, LOCALID_PARTY_LEADER, 0);
  Engine_ObjectEventShowEmote(UPPER_FLOOR_FIRST_PATROL,
                              EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  LunpaFortress_GuardThrowsPartyOut(UPPER_FLOOR_SECOND_PATROL);
  SCRIPT_END();
}

/// Whether the leader and the hall patrol are in the two parallel corridors,
/// where the guard passes without seeing the leader.
static s32 LunpaFortress_IsLeaderPassingPatrollingGuard(void) {
  struct ObjectEvent *leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  struct ObjectEvent *guard = ObjectEvent_GetById(UPPER_FLOOR_HALL_PATROL);
  s32 leaderFixedX = leader->x.fixed;
  s32 guardFixedZ;
  s32 guardFixedX;
  s32 leaderTileX, guardTileZ, guardTileX;

  if (leaderFixedX < 0) {
    leaderFixedX += 0xfffff;
  }
  leaderTileX = leaderFixedX >> 20;
  guardFixedZ = guard->z.fixed;
  if (guardFixedZ < 0) {
    guardFixedZ += 0xfffff;
  }
  guardFixedX = guard->x.fixed;
  guardTileZ = guardFixedZ >> 20;
  if (guardFixedX < 0) {
    guardFixedX += 0xfffff;
  }
  guardTileX = guardFixedX >> 20;
  if (leaderTileX == 52 && guardTileX == 57 && guardTileZ > 34 &&
      guardTileZ <= 40) {
    return 1;
  }
  if (leaderTileX == 57 && guardTileX == 52 && guardTileZ > 34 &&
      guardTileZ <= 40) {
    return 1;
  }
  return 0;
}

/// Every frame in LunpaFortressUpperFloor: the patrol in the east hall, as
/// LunpaFortress_UpdatePairedGuardDetection; seeing the leader, it raises
/// TRIGGER_HALL_PATROL_SPOTTED.
static void LunpaFortress_UpdateHallGuardDetection(void) {

  struct ObjectEvent *guard = ObjectEvent_Get(UPPER_FLOOR_HALL_PATROL);
  struct MapGridLayerScroll *patrolLayer =
      &((struct MapGridScrollWork *)gMapGridWork[0])
           ->layers[MAPGRID_LAYER_OVERHEAD];
#ifdef UBFIX
  struct FieldWork *work = (struct FieldWork *)gFieldWork;
#else
  s16 *work = *(s16 **)&gMapGridWork[19];
#endif

  Engine_ScriptGetViewCenter();
  if (gFrameCount & 1) {
    patrolLayer->speedX = 1;
    patrolLayer->speedY = 1;
  } else {
    patrolLayer->speedX = -1;
    patrolLayer->speedY = -1;
  }
  if (FLAG_CHECK(FLAG_MENU_OPEN) || PATROL_PSYNERGY_REQUEST(work) != 0 ||
      PATROL_ITEM_REQUEST(work) != 0) {
    guard->paused = 1;
  } else if (!FLAG_CHECK(FLAG_MAP_TEMP_14)) {
    guard->paused = 0;
    if (!FLAG_CHECK(FLAG_MAP_TEMP_14) && guard->paused == 0) {
      patrolLayer->phaseX = 0x3400000 - guard->x.fixed;
      patrolLayer->phaseY = 0x2400000 - guard->z.fixed;
    }
    if (!LunpaFortress_IsLeaderPassingPatrollingGuard()) {
      LunpaFortress_IsGuardNearViewCenter(UPPER_FLOOR_HALL_PATROL);
      if (LunpaFortress_IsLeaderNearGuard(UPPER_FLOOR_HALL_PATROL) &&
          gGameState.cloaked != 0) {
        SetFieldWorkValue(&PATROL_PSYNERGY_REQUEST(work), CLOAK_NEAR_GUARD);
        return;
      }
      if (gGameState.cloaked == 0) {
        if (LunpaFortress_CanGuardSeeLeader(UPPER_FLOOR_HALL_PATROL)) {
          FLAG_SET(FLAG_MAP_TEMP_15);
          FLAG_SET(FLAG_MAP_TEMP_14);
        }
      }
      if (FLAG_CHECK(FLAG_MAP_TEMP_14)) {
        SetFieldWorkValue(&PATROL_RAISED_TRIGGER(work),
                          TRIGGER_HALL_PATROL_SPOTTED);
      }
    }
  }
}

/// LunpaFortressUpperFloor, raised when the hall patrol spots the leader: it
/// throws the party out.
static void EventScript_HallPatrolThrowsPartyOut(void) {
  LunpaFortress_GuardThrowsPartyOut(UPPER_FLOOR_HALL_PATROL);
  SCRIPT_END();
}

/// Where LunpaFortressGroundFloor's patrol cannot see the leader: three
/// stretches of its rounds.
static s32 LunpaFortress_IsLeaderInGroundFloorPatrolSafeZone(void) {
  struct ObjectEvent *leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  s32 leaderTileZ = leader->z.fixed / PIXELS(16);
  s32 leaderTileX = leader->x.fixed / PIXELS(16);

  if ((u32)(leaderTileX - 41) <= 3 && leaderTileZ > 25 && leaderTileZ <= 28) {
    return 1;
  }
  if (leaderTileX == 41 && leaderTileZ > 37 && leaderTileZ <= 41) {
    return 1;
  }
  if ((u32)(leaderTileX - 54) <= 2 && leaderTileZ > 30 && leaderTileZ <= 40) {
    return 1;
  }
  return 0;
}

/// Every frame in LunpaFortressGroundFloor: the patrolling guard, as
/// LunpaFortress_UpdatePairedGuardDetection; seeing the leader, it raises
/// TRIGGER_GROUND_FLOOR_PATROL_SPOTTED.
static void LunpaFortress_UpdateGroundFloorPatrolDetection(void) {

  struct ObjectEvent *guard = ObjectEvent_Get(GROUND_FLOOR_PATROL_GUARD);
  struct MapGridLayerScroll *patrolLayer =
      &((struct MapGridScrollWork *)gMapGridWork[0])
           ->layers[MAPGRID_LAYER_OVERHEAD];
#ifdef UBFIX
  struct FieldWork *work = (struct FieldWork *)gFieldWork;
#else
  s16 *work = *(s16 **)&gMapGridWork[19];
#endif

  if (gFrameCount & 1) {
    patrolLayer->speedX = 1;
    patrolLayer->speedY = 1;
  } else {
    patrolLayer->speedX = -1;
    patrolLayer->speedY = -1;
  }
  if (FLAG_CHECK(FLAG_MENU_OPEN) || PATROL_PSYNERGY_REQUEST(work) != 0 ||
      PATROL_ITEM_REQUEST(work) != 0) {
    guard->paused = 1;
  } else if (!FLAG_CHECK(FLAG_MAP_TEMP_14)) {
    guard->paused = 0;
    if (!FLAG_CHECK(FLAG_MAP_TEMP_14) && guard->paused == 0) {
      patrolLayer->phaseX = 0x2f00000 - guard->x.fixed;
      patrolLayer->phaseY = 0x1f00000 - guard->z.fixed;
    }
    if (!LunpaFortress_IsLeaderInGroundFloorPatrolSafeZone()) {
      if (LunpaFortress_IsLeaderNearGuard(GROUND_FLOOR_PATROL_GUARD) &&
          gGameState.cloaked != 0) {
        SetFieldWorkValue(&PATROL_PSYNERGY_REQUEST(work), CLOAK_NEAR_GUARD);
        return;
      }
      if (gGameState.cloaked == 0) {
        if (LunpaFortress_CanGuardSeeLeader(GROUND_FLOOR_PATROL_GUARD)) {
          FLAG_SET(FLAG_MAP_TEMP_15);
          FLAG_SET(FLAG_MAP_TEMP_14);
        }
      }
      if (FLAG_CHECK(FLAG_MAP_TEMP_14)) {
        SetFieldWorkValue(&PATROL_RAISED_TRIGGER(work),
                          TRIGGER_GROUND_FLOOR_PATROL_SPOTTED);
      }
    }
  }
}

/// LunpaFortressGroundFloor, raised when the patrol spots the leader: it throws
/// the party out.
static void EventScript_GroundFloorPatrolThrowsPartyOut(void) {
  LunpaFortress_GuardThrowsPartyOut(GROUND_FLOOR_PATROL_GUARD);
  SCRIPT_END();
}

/// Whether the leader is outside the upper hall the guard there watches.
static s32 LunpaFortress_IsLeaderOutsideUpperHall(void) {
  struct ObjectEvent *leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  s32 leaderTileZ = leader->z.fixed / PIXELS(16);
  s32 leaderTileX = leader->x.fixed / PIXELS(16);

  if (leaderTileX > 45 && leaderTileZ > 14 && leaderTileX <= 64 &&
      leaderTileZ <= 16) {
    return 0;
  }
  return 1;
}

/// Every frame in LunpaFortressGroundFloor's upper hall: the guard who sees the
/// uncloaked leader raises TRIGGER_UPPER_HALL_SPOTTED.
static void LunpaFortress_CheckUpperHallGuard(void) {

  struct EventWork *fieldWork;

  fieldWork = gFieldWork;
  if (FLAG_CHECK(FLAG_MAP_TEMP_14) == 0) {
    if (LunpaFortress_IsLeaderOutsideUpperHall() == 0) {
      if (gGameState.cloaked == 0) {
        if (LunpaFortress_CanGuardSeeLeader(GROUND_FLOOR_UPPER_HALL_GUARD) !=
            0) {
          FLAG_SET(FLAG_MAP_TEMP_15);
          FLAG_SET(FLAG_MAP_TEMP_14);
        }
      }
      if (FLAG_CHECK(FLAG_MAP_TEMP_14) != 0) {
        fieldWork->raisedEvent = TRIGGER_UPPER_HALL_SPOTTED;
      }
    }
  }
}

/// LunpaFortressGroundFloor, raised when the upper hall's guard spots the
/// leader: he throws the party out.
static void EventScript_UpperHallGuardThrowsPartyOut(void) {
  LunpaFortress_GuardThrowsPartyOut(GROUND_FLOOR_UPPER_HALL_GUARD);
  SCRIPT_END();
}

/// Every frame after the party arrives in LunpaFortressGroundFloor's entrance
/// or main passage: the guard who sees the uncloaked leader raises
/// TRIGGER_ENTRANCE_GUARD_SPOTTED, once.
static void LunpaFortress_CheckEntranceGuard(void) {

  struct EventWork *fieldWork = gFieldWork;

  if (LunpaFortress_CanGuardSeeLeader(GROUND_FLOOR_ENTRANCE_GUARD) != 0 &&
      gGameState.cloaked == 0) {
    s16 *raisedTrigger;
    s32 caughtTrigger;

    Engine_TaskRemoveCallback(LunpaFortress_CheckEntranceGuard);
    raisedTrigger = &fieldWork->raisedEvent;
    caughtTrigger = TRIGGER_ENTRANCE_GUARD_SPOTTED;
    *raisedTrigger = caughtTrigger;
  }
}

/// Every frame after the party arrives in LunpaFortressGroundFloor's entrance
/// or main passage: either sentry who sees the uncloaked leader raises
/// TRIGGER_SENTRIES_SPOTTED, once for both.
static void LunpaFortress_CheckFirstEntranceSentry(void) {

  struct EventWork *fieldWork;

  fieldWork = gFieldWork;
  if (FLAG_CHECK(FLAG_MAP_TEMP_25) == 0) {
    if (LunpaFortress_CanGuardSeeLeader(GROUND_FLOOR_FIRST_SENTRY) != 0) {
      if (gGameState.cloaked == 0) {
        FLAG_SET(FLAG_MAP_TEMP_25);
        Engine_TaskRemoveCallback(LunpaFortress_CheckFirstEntranceSentry);
        Engine_TaskRemoveCallback(LunpaFortress_CheckSecondEntranceSentry);
        fieldWork->raisedEvent = TRIGGER_SENTRIES_SPOTTED;
      }
    }
  }
}

/// The second sentry, as the first.
static void LunpaFortress_CheckSecondEntranceSentry(void) {

  struct EventWork *fieldWork;

  fieldWork = gFieldWork;
  if (FLAG_CHECK(FLAG_MAP_TEMP_25) == 0) {
    if (LunpaFortress_CanGuardSeeLeader(GROUND_FLOOR_SECOND_SENTRY) != 0) {
      if (gGameState.cloaked == 0) {
        FLAG_SET(FLAG_MAP_TEMP_25);
        Engine_TaskRemoveCallback(LunpaFortress_CheckSecondEntranceSentry);
        Engine_TaskRemoveCallback(LunpaFortress_CheckFirstEntranceSentry);
        fieldWork->raisedEvent = TRIGGER_SENTRIES_SPOTTED;
      }
    }
  }
}

/// Whether the leader is in the guard's column, up to six cells away.
static s32 LunpaFortress_IsLeaderInGuardVerticalSight(s32 localId) {
  struct ObjectEvent *guard = ObjectEvent_GetById(localId);
  struct ObjectEvent *leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  s32 guardTileZ = guard->z.fixed / PIXELS(16);
  s32 guardTileX = guard->x.fixed / PIXELS(16);
  s32 leaderTileZ = leader->z.fixed / PIXELS(16);
  s32 leaderTileX = leader->x.fixed / PIXELS(16);
  s32 zDistance = guardTileZ - leaderTileZ;

  if (zDistance >= -6 && zDistance <= 6 && guardTileX - 1 < leaderTileX &&
      guardTileX + 1 > leaderTileX) {
    return 1;
  }
  return 0;
}

/// Whether the leader is within a row of the guard, up to six cells away.
static s32 LunpaFortress_IsLeaderInGuardHorizontalSight(s32 localId) {
  struct ObjectEvent *guard = ObjectEvent_GetById(localId);
  struct ObjectEvent *leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  s32 guardTileZ = guard->z.fixed / PIXELS(16);
  s32 guardTileX = guard->x.fixed / PIXELS(16);
  s32 leaderTileZ = leader->z.fixed / PIXELS(16);
  s32 leaderTileX = leader->x.fixed / PIXELS(16);
  s32 xDistance = guardTileX - leaderTileX;

  if (xDistance < -6 || xDistance > 6) {
    return 0;
  }
  if (guardTileZ - 2 < leaderTileZ && guardTileZ + 2 > leaderTileZ) {
    return 1;
  }
  return 0;
}

/// Whether a guard on screen sees the leader along his row or column.
static s32 LunpaFortress_CanGuardSeeLeader(s32 localId) {
  if (LunpaFortress_IsGuardNearViewCenter(localId) == 0) {
    return 0;
  }
  if (LunpaFortress_IsLeaderInGuardVerticalSight(localId) != 0) {
    return 1;
  }
  {
    s32 inHorizontalSight =
        LunpaFortress_IsLeaderInGuardHorizontalSight(localId);

    return (u32)(inHorizontalSight | -inHorizontalSight) >> 31;
  }
}

/// Whether the leader is within four cells, walking, of the cell below the
/// guard.
static s32 LunpaFortress_IsLeaderNearGuard(s32 localId) {
  struct ObjectEvent *guard = ObjectEvent_GetById(localId);
  struct ObjectEvent *leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  s32 guardTileZ = guard->z.fixed / PIXELS(16);
  s32 guardTileX = guard->x.fixed / PIXELS(16);
  s32 leaderTileZ = leader->z.fixed / PIXELS(16);
  s32 leaderTileX = leader->x.fixed / PIXELS(16);
  s32 xDistance = guardTileX - leaderTileX;
  s32 zDistance;

  guardTileZ += 1;
  if (xDistance < 0) {
    xDistance = -xDistance;
  }
  zDistance = guardTileZ - leaderTileZ;
  if (zDistance < 0) {
    zDistance = -zDistance;
  }
  if (xDistance + zDistance <= 4) {
    return 1;
  }
  return 0;
}

/// Whether the guard is on screen: within seven columns and five rows of the
/// view's center.
static s32 LunpaFortress_IsGuardNearViewCenter(s32 localId) {
  struct ObjectEvent *guard = ObjectEvent_GetById(localId);
  struct ObjectEvent *viewCenter = Engine_ScriptGetViewCenter();
  s32 guardTileX = guard->x.fixed / PIXELS(16);
  s32 guardTileZ = guard->z.fixed / PIXELS(16);
  s32 viewTileX = viewCenter->x.fixed / PIXELS(16);
  s32 viewTileZ = viewCenter->z.fixed / PIXELS(16);
  s32 xDistance = guardTileX - viewTileX;
  s32 zDistance;

  if (xDistance < 0) {
    xDistance = -xDistance;
  }
  zDistance = guardTileZ - viewTileZ;
  if (zDistance < 0) {
    zDistance = -zDistance;
  }
  if (xDistance > 7 || zDistance > 5) {
    return 0;
  }
  return 1;
}

/// Every frame in LunpaFortressGroundFloor: walking into the second cell door's
/// column with the Cell Key unlocks it.
static void LunpaFortress_CheckGroundFloorDoorApproach(void) {

  struct ObjectEvent *leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);

  if (Engine_GameFlagIsSet(GROUND_FLOOR_DOOR_FLAG + 1) == 0) {
    s32 playerX = leader->x.fixed / PIXELS(16);
    s32 playerZ = leader->z.fixed / PIXELS(16);

    if (playerX == 43 && playerZ > 28 && playerZ <= 31) {
      s16 *touchedTrigger = &gFieldWork->coordEvent;
      s32 doorTrigger = TRIGGER_DOOR_FIRST + 1;

      *touchedTrigger = doorTrigger;
      EventScript_UnlockGroundFloorDoor();
    }
  }
}

/// Every frame in LunpaFortressUpperFloor: walking east or west into the first
/// cell door's column with the Cell Key unlocks it.
static void LunpaFortress_CheckUpperFloorDoorApproach(void) {

  struct ObjectEvent *leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);

  if (Engine_GameFlagIsSet(UPPER_FLOOR_DOOR_FLAG) == 0) {
    s32 playerX = leader->x.fixed / PIXELS(16);
    s32 playerZ = leader->z.fixed / PIXELS(16);

    if (playerX == 16 && playerZ > 55 && playerZ <= 58 &&
        (leader->facing == 0xc000 || leader->facing == 0x4000)) {
      s16 *touchedTrigger = &gFieldWork->coordEvent;
      s32 doorTrigger = TRIGGER_DOOR_FIRST;

      *touchedTrigger = doorTrigger;
      EventScript_UnlockUpperFloorDoor();
    }
  }
}

/// Unused: nothing calls it; LunpaFortressUpperFloor's patrolling pair throws
/// the party out through EventScript_PatrolPairThrowsPartyOut instead. The
/// first patrol would stop, ask who the party is and send it out.
static void UNUSED LunpaFortress_UnusedFirstPatrolThrowsPartyOut(void) {
  SCRIPT_START();
  Engine_ObjectEventSetDestinationOffset(UPPER_FLOOR_FIRST_PATROL, 0, 0);
  ObjectEvent_SetBuiltinAction(UPPER_FLOOR_FIRST_PATROL, OBJ_EVENT_ACTION_IDLE);
  Engine_ObjectEventStop(UPPER_FLOOR_FIRST_PATROL);
  OBJECT_ANIMATION(UPPER_FLOOR_FIRST_PATROL, 0);
  ObjectEvent_SetBuiltinAction(LOCALID_PARTY_LEADER, OBJ_EVENT_ACTION_IDLE);
  {
    s32 challengeMessage = MsgLunpaWho2;

    MESSAGE_SET(challengeMessage);
    MESSAGE_PUT(UPPER_FLOOR_FIRST_PATROL, 0);
    Engine_ObjectEventShowEmote(LOCALID_PARTY_LEADER,
                                EMOTE_IN_FRONT | EMOTE_WINCE, 60);
    MESSAGE_SET(challengeMessage + 1);
  }
  MESSAGE_PUT(UPPER_FLOOR_FIRST_PATROL, 0);
  Engine_ScriptRequestWarp(WARP_THROWN_OUT);
  Engine_ScriptCloseScreen();
  SCRIPT_END();
}

/// A guard who caught the party sends it back to the Lunpa Fortress Gates.
/// "Who are you? How did you get in here?"
static void LunpaFortress_GuardThrowsPartyOut(s32 localId) {
  SCRIPT_START();
  SCRIPT_START();
  Engine_ObjectEventShowEmote(localId, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 1);
  Engine_ObjectEventSetDestinationOffset(localId, 0, 0);
  ObjectEvent_SetBuiltinAction(localId, OBJ_EVENT_ACTION_IDLE);
  OBJECT_ANIMATION(localId, 0);
  OBJECT_FACE(localId, LOCALID_PARTY_LEADER, 0);
  OBJECT_ANIMATION(LOCALID_PARTY_LEADER, ANIMATION_STAND);
  Engine_ObjectEventSetDestinationOffset(localId, 0, 0);
  ObjectEvent_SetBuiltinAction(localId, OBJ_EVENT_ACTION_IDLE);
  Engine_ObjectEventStop(localId);
  OBJECT_ANIMATION(localId, 0);
  ObjectEvent_SetBuiltinAction(LOCALID_PARTY_LEADER, OBJ_EVENT_ACTION_IDLE);
  {
    s32 challengeMessage = MsgLunpaWho2;

    MESSAGE_SET(challengeMessage);
    MESSAGE_PUT(localId, 0);
    OBJECT_FACE(LOCALID_PARTY_LEADER, localId, 0);
    Engine_ObjectEventShowEmote(LOCALID_PARTY_LEADER,
                                EMOTE_IN_FRONT | EMOTE_WINCE, 60);
    MESSAGE_SET(challengeMessage + 1);
  }
  MESSAGE_PUT(localId, 0);
  Engine_ScriptCloseScreen();
  WAIT(60);
  Engine_ScriptRequestWarp(WARP_THROWN_OUT);
  SCRIPT_END();
}

/// A guard faces the leader with one of four challenges, chosen at random
/// when the map was entered.
static void LunpaFortress_GuardChallengesParty(s32 localId) {

  OBJECT_FACE(localId, LOCALID_PARTY_LEADER, 0);
  OBJECT_FACE(LOCALID_PARTY_LEADER, localId, 0);
  switch (gLunpaFortressRandomPick & 3) {
  case 0:
    LunpaFortress_ActOutIntruderDialogue(localId);
    break;
  case 1:
    LunpaFortress_ActOutWhosThereDialogue(localId);
    break;
  case 2:
    LunpaFortress_ActOutGuardShiftDialogue(localId);
    break;
  case 3:
    LunpaFortress_ActOutScoundrelDialogue(localId);
    break;
  default:
    LunpaFortress_ActOutWhosThereDialogue(localId);
    break;
  }
}

/// A guard spots an intruder. "Hey! Intruder!"
static void LunpaFortress_ActOutIntruderDialogue(s32 localId) {
  s32 messageBase;

  Engine_ObjectEventRunRepeatedMotion(localId, 1);
  messageBase = MsgLunpaIntruder;
  MESSAGE_SET(messageBase);
  MESSAGE_PUT(localId, 0);
  OBJECT_EMOTE(localId, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  MESSAGE_SET(messageBase + 1);
  MESSAGE_PUT(localId, 0);
  messageBase += 2;
  Engine_ObjectEventSetAnimationAndWait(localId, 4);
  MESSAGE_SET(messageBase);
  MESSAGE_PUT(localId, 0);
}

/// A guard asks who is there. "Huh? Who's that?"
static void LunpaFortress_ActOutWhosThereDialogue(s32 localId) {
  s32 messageBase = MsgLunpaGuardWhosThat;

  MESSAGE_SET(messageBase);
  MESSAGE_PUT(localId, 0);
  Engine_ObjectEventRunRepeatedMotion(localId, 1);
  MESSAGE_SET(messageBase + 1);
  MESSAGE_PUT(localId, 0);
  Engine_ObjectEventSetAnimationAndWait(localId, 4);
  MESSAGE_SET(messageBase + 2);
  MESSAGE_PUT(localId, 0);
}

/// A guard expects his relief, then sees who it is. "Is my shift up already?"
static void LunpaFortress_ActOutGuardShiftDialogue(s32 localId) {
  s32 messageBase;

  messageBase = MsgLunpaShiftAlready;
  MESSAGE_SET(messageBase);
  MESSAGE_PUT(localId, 0);
  WAIT(120);
  OBJECT_EMOTE(localId, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  MESSAGE_SET((messageBase + 1));
  MESSAGE_PUT(localId, 0);
  Engine_ObjectEventRunRepeatedMotion(localId, 1);
  MESSAGE_SET((messageBase + 2));
  MESSAGE_PUT(localId, 0);
  Engine_ObjectEventSetAnimationAndWait(localId, 4);
  MESSAGE_SET((messageBase + 3));
  MESSAGE_PUT(localId, 0);
}

/// A guard calls the party a scoundrel. "You scoundrel!"
static void LunpaFortress_ActOutScoundrelDialogue(s32 localId) {
  s32 messageBase = MsgLunpaScoundrel;

  MESSAGE_SET(messageBase);
  MESSAGE_PUT(localId, 0);
  Engine_ObjectEventSetAnimationAndWait(localId, 4);
  MESSAGE_SET(messageBase + 1);
  MESSAGE_PUT(localId, 0);
  Engine_ObjectEventRunRepeatedMotion(localId, 1);
  MESSAGE_SET(messageBase + 2);
  MESSAGE_PUT(localId, 0);
  Engine_ObjectEventSetAnimationAndWait(localId, 3);
  MESSAGE_SET(messageBase + 3);
  MESSAGE_PUT(localId, 0);
}

/// LunpaFortressUpperFloor, coord event 33: walking up to the west guard, he
/// challenges the party to a battle with a Brigand, and is gone after it.
static void EventScript_FightWestGuard(void) {

  OBJECT_ANIMATION(LOCALID_PARTY_LEADER, ANIMATION_STAND);
  SE(SE_MENU_CANCEL);
  Engine_ObjectEventShowEmote(UPPER_FLOOR_WEST_GUARD,
                              EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  LunpaFortress_GuardChallengesParty(UPPER_FLOOR_WEST_GUARD);
  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  Engine_EncounterRequestBattle(ENCOUNTER_ZONE_SCRIPTED, 2);
  Engine_ObjectEventSetPosition(UPPER_FLOOR_WEST_GUARD, 0, 0);
  SCRIPT_END();
  Engine_GameFlagSet(FLAG_LUNPA_FORTRESS_WEST_GUARD_BEATEN);
}

/// LunpaFortressUpperFloor, coord event 30: walking up to the hall guard, he
/// challenges the party to a battle with a Brigand, and is gone after it.
static void EventScript_FightHallGuard(void) {

  OBJECT_ANIMATION(LOCALID_PARTY_LEADER, ANIMATION_STAND);
  SE(SE_MENU_CANCEL);
  Engine_ObjectEventShowEmote(UPPER_FLOOR_HALL_GUARD,
                              EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  LunpaFortress_GuardChallengesParty(UPPER_FLOOR_HALL_GUARD);
  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  Engine_EncounterRequestBattle(ENCOUNTER_ZONE_SCRIPTED, 2);
  Engine_ObjectEventSetPosition(UPPER_FLOOR_HALL_GUARD, 0, 0);
  SCRIPT_END();
  Engine_GameFlagSet(FLAG_LUNPA_FORTRESS_MIDDLE_GUARD_BEATEN);
}

/// LunpaFortressUpperFloor, coord event 31: walking up to the three guards,
/// they argue, then fight the party together as three Brigands. "Who are you!?"
/// After the battle the party comes back here.
static void EventScript_FightThreeGuards(void) {
  s32 messageBase;

  OBJECT_ANIMATION(LOCALID_PARTY_LEADER, ANIMATION_STAND);
  OBJECT_ANIMATION(UPPER_FLOOR_GUARD_A, 1);
  OBJECT_ANIMATION(UPPER_FLOOR_GUARD_B, 1);
  OBJECT_ANIMATION(UPPER_FLOOR_GUARD_C, 1);
  SE(SE_MENU_CANCEL);
  OBJECT_EMOTE(UPPER_FLOOR_GUARD_A, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  WAIT(30);
  OBJECT_FACE(UPPER_FLOOR_GUARD_A, LOCALID_PARTY_LEADER, 0);
  messageBase = MsgLunpaWho3;
  MESSAGE_SET(messageBase);
  MESSAGE_PUT(UPPER_FLOOR_GUARD_A, 0);
  OBJECT_EMOTE(UPPER_FLOOR_GUARD_B, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  OBJECT_EMOTE(UPPER_FLOOR_GUARD_C, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  OBJECT_FACE(UPPER_FLOOR_GUARD_B, LOCALID_PARTY_LEADER, 0);
  OBJECT_FACE(UPPER_FLOOR_GUARD_C, LOCALID_PARTY_LEADER, 0);
  OBJECT_FACE(LOCALID_PARTY_LEADER, UPPER_FLOOR_GUARD_B, 0);
  WAIT(65);
  OBJECT_DIR(UPPER_FLOOR_GUARD_B, FACING_SOUTH + FACING_STEP, 0);
  OBJECT_DIR(UPPER_FLOOR_GUARD_C, FACING_NORTH + FACING_STEP, 0);
  MESSAGE_SET((messageBase + 1));
  MESSAGE_PUT(UPPER_FLOOR_GUARD_B, 0);
  Engine_ObjectEventSetAnimationAndWait(UPPER_FLOOR_GUARD_C, 3);
  MESSAGE_SET((messageBase + 2));
  MESSAGE_PUT(UPPER_FLOOR_GUARD_C, 0);
  MESSAGE_SET((messageBase + 3));
  MESSAGE_PUT(UPPER_FLOOR_GUARD_A, 0);
  Engine_ObjectEventRunRepeatedMotion(UPPER_FLOOR_GUARD_B, 1);
  MESSAGE_SET((messageBase + 4));
  MESSAGE_PUT(UPPER_FLOOR_GUARD_B, 0);
  Engine_ObjectEventSetAnimationAndWait(UPPER_FLOOR_GUARD_C, 3);
  MESSAGE_SET((messageBase + 5));
  MESSAGE_PUT(UPPER_FLOOR_GUARD_C, 0);
  Engine_ObjectEventSetAnimationAndWait(UPPER_FLOOR_GUARD_C, 3);
  WAIT(60);
  OBJECT_FACE(UPPER_FLOOR_GUARD_B, LOCALID_PARTY_LEADER, 0);
  OBJECT_FACE(UPPER_FLOOR_GUARD_C, LOCALID_PARTY_LEADER, 0);
  WAIT(70);
  OBJECT_WALK(UPPER_FLOOR_GUARD_A, 672, 88);
  Engine_ObjectEventWaitForMove(UPPER_FLOOR_GUARD_A);
#if TBS_PATCH_1
  OBJECT_FACE(UPPER_FLOOR_GUARD_A, LOCALID_PARTY_LEADER, 0);
#endif
  Engine_ObjectEventSetAnimationAndWait(UPPER_FLOOR_GUARD_A, 3);
  WAIT(30);
  MESSAGE_SET((messageBase + 6));
  MESSAGE_PUT(UPPER_FLOOR_GUARD_A, 0);
  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
  Engine_ScriptSetNextDestination(
      (s32)&MapId_LunpaFortressUpperFloor,
      LUNPA_FORTRESS_UPPER_FLOOR_GUARD_ROOM_AFTER_BATTLE);
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  Engine_EncounterRequestBattle(ENCOUNTER_ZONE_SCRIPTED, 3);
  OBJECT_POSITION_SET(UPPER_FLOOR_GUARD_A, 0, 0);
  OBJECT_POSITION_SET(UPPER_FLOOR_GUARD_B, 0, 0);
  OBJECT_POSITION_SET(UPPER_FLOOR_GUARD_C, 0, 0);
  SCRIPT_END();
  FLAG_SET(FLAG_LUNPA_FORTRESS_THREE_GUARDS_BEATEN);
}

/// The three guards lie beaten where they fought.
static void LunpaFortress_PlaceThreeBeatenGuards(void) {
  SCRIPT_START();
  OBJECT_POSITION_SET(UPPER_FLOOR_GUARD_A, 688, 88);
  OBJECT_POSITION_SET(UPPER_FLOOR_GUARD_B, 704, 88);
  OBJECT_POSITION_SET(UPPER_FLOOR_GUARD_C, 720, 96);
  OBJECT_ANIMATION(UPPER_FLOOR_GUARD_A, 5);
  OBJECT_ANIMATION(UPPER_FLOOR_GUARD_B, 5);
  OBJECT_ANIMATION(UPPER_FLOOR_GUARD_C, 5);
  OBJECT_FACE(LOCALID_PARTY_LEADER, UPPER_FLOOR_GUARD_B, 0);
  SCRIPT_END();
  Engine_ScriptOpenScreen();
}

/// LunpaFortressUpperFloor, coord event 32: walking up to the east guard, he
/// challenges the party to a battle with a Brigand, and is gone after it.
static void EventScript_FightEastGuard(void) {

  OBJECT_ANIMATION(LOCALID_PARTY_LEADER, ANIMATION_STAND);
  SE(SE_MENU_CANCEL);
  Engine_ObjectEventShowEmote(UPPER_FLOOR_EAST_GUARD,
                              EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  LunpaFortress_GuardChallengesParty(UPPER_FLOOR_EAST_GUARD);
  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  Engine_EncounterRequestBattle(ENCOUNTER_ZONE_SCRIPTED, 2);
  Engine_ObjectEventSetPosition(UPPER_FLOOR_EAST_GUARD, 0, 0);
  SCRIPT_END();
  Engine_GameFlagSet(FLAG_LUNPA_FORTRESS_EAST_GUARD_BEATEN);
}

/// LunpaFortressGroundFloor, coord event 50 or seen by the entrance guard: the
/// party is sent back to the Lunpa Fortress Gates. "Who are you?"
static void EventScript_EntranceGuardThrowsPartyOut(void) {
  SCRIPT_START();
  Engine_ObjectEventSetDestinationOffset(LOCALID_PARTY_LEADER, 0, 0);
  ObjectEvent_SetBuiltinAction(LOCALID_PARTY_LEADER, OBJ_EVENT_ACTION_IDLE);
  OBJECT_ANIMATION(LOCALID_PARTY_LEADER, ANIMATION_STAND);
  OBJECT_FACE(GROUND_FLOOR_ENTRANCE_GUARD, LOCALID_PARTY_LEADER, 0);
  SE(SE_MENU_CANCEL);
  Engine_ObjectEventShowEmote(GROUND_FLOOR_ENTRANCE_GUARD,
                              EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  {
    s32 challengeMessage = MsgLunpaWho2;

    MESSAGE_SET(challengeMessage);
    MESSAGE_PUT(GROUND_FLOOR_ENTRANCE_GUARD, 0);
    Engine_ObjectEventShowEmote(LOCALID_PARTY_LEADER,
                                EMOTE_IN_FRONT | EMOTE_WINCE, 50);
    MESSAGE_SET(challengeMessage + 1);
  }
  MESSAGE_PUT(GROUND_FLOOR_ENTRANCE_GUARD, 0);
  Engine_ScriptCloseScreen();
  WAIT(60);
  Engine_ScriptRequestWarp(WARP_THROWN_OUT);
  SCRIPT_END();
  Engine_GameFlagSet(FLAG_MAP_TEMP_24);
}

/// LunpaFortressGroundFloor, coord event 51 between the sentries or seen by
/// them: the party is sent back to the Lunpa Fortress Gates. "Who are you?"
static void EventScript_SentriesThrowPartyOut(void) {
  s32 challengeMessage;

  SCRIPT_START();
  ObjectEvent_SetDestinationOffset(LOCALID_PARTY_LEADER, 0, 0);
  ObjectEvent_SetBuiltinAction(LOCALID_PARTY_LEADER, OBJ_EVENT_ACTION_IDLE);
  OBJECT_ANIMATION(LOCALID_PARTY_LEADER, ANIMATION_STAND);
  SE(SE_MENU_CANCEL);
  OBJECT_EMOTE(GROUND_FLOOR_SECOND_SENTRY, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
               0);
  OBJECT_EMOTE(GROUND_FLOOR_FIRST_SENTRY, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
               60);
  OBJECT_FACE(GROUND_FLOOR_SECOND_SENTRY, LOCALID_PARTY_LEADER, 0);
  OBJECT_FACE(GROUND_FLOOR_FIRST_SENTRY, LOCALID_PARTY_LEADER, 0);
  challengeMessage = MsgLunpaWho2;
  MESSAGE_SET(challengeMessage);
  MESSAGE_PUT(GROUND_FLOOR_FIRST_SENTRY, 0);
  OBJECT_EMOTE(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  MESSAGE_SET(challengeMessage + 1);
  MESSAGE_PUT(GROUND_FLOOR_FIRST_SENTRY, 0);
  Engine_ScriptCloseScreen();
  WAIT(60);
  Engine_ScriptRequestWarp(WARP_THROWN_OUT);
  SCRIPT_END();
  FLAG_SET(FLAG_MAP_TEMP_25);
}

/// The first switch's passage opens.
static void LunpaFortress_OpenFirstSwitchPassage(void) {
  MapGrid_CopyCells(2, 82, 1, 2, 21, 81);
  MapGrid_CopyCellAttributes(21, 32, 1, 1, 21, 34);
}

/// The second switch's passage opens.
static void LunpaFortress_OpenSecondSwitchPassage(void) {
  MapGrid_CopyCells(2, 84, 1, 2, 6, 55);
  MapGrid_CopyCellAttributes(5, 9, 1, 1, 6, 10);
}

/// The passage behind the second Whirlwind cover opens.
static void LunpaFortress_OpenCoveredPassage(void) {
  MapGrid_CopyCells(2, 86, 1, 2, 27, 62);
  MapGrid_CopyCellAttributes(26, 16, 1, 1, 27, 17);
}

/// A switch flips in two frames, then its passage opens.
static __inline__ void
LunpaFortress_FlipHiddenSwitch(s32 cellX, s32 cellY,
                               void (*openPassage)(void)) {
  MapGrid_CopyCells(6, 77, 1, 2, cellX, cellY);
  WAIT(5);
  MapGrid_CopyCells(7, 77, 1, 2, cellX, cellY);
  WAIT(1);
  openPassage();
}

/// LunpaFortressCave, bg event 52 once uncovered: searching the first switch
/// while Reveal shows it flips it, and its passage opens. "You flipped the
/// switch."
static void EventScript_FlipFirstSwitch(void) {
  if (((struct FieldWork *)gFieldWork)->revealActive != 0) {
    if (FLAG_CHECK(FLAG_LUNPA_FORTRESS_FIRST_SWITCH_FLIPPED) == 0) {
      Engine_MessageShowCentered(MsgFieldFlippedSwitch, 1);
      SE(SE_188);
      WAIT(1);
      LunpaFortress_FlipHiddenSwitch(17, 82,
                                     LunpaFortress_OpenFirstSwitchPassage);
      FLAG_SET(FLAG_LUNPA_FORTRESS_FIRST_SWITCH_FLIPPED);
    }
  }
}

/// LunpaFortressCave, bg event 51 once uncovered: searching the second switch
/// while Reveal shows it flips it, and its passage opens. "You flipped the
/// switch."
static void EventScript_FlipSecondSwitch(void) {
  if (((struct FieldWork *)gFieldWork)->revealActive != 0) {
    if (FLAG_CHECK(FLAG_LUNPA_FORTRESS_SECOND_SWITCH_FLIPPED) == 0) {
      Engine_MessageShowCentered(MsgFieldFlippedSwitch, 1);
      SE(SE_188);
      WAIT(1);
      LunpaFortress_FlipHiddenSwitch(3, 55,
                                     LunpaFortress_OpenSecondSwitchPassage);
      FLAG_SET(FLAG_LUNPA_FORTRESS_SECOND_SWITCH_FLIPPED);
    }
  }
}

/// LunpaFortressCave, casting Reveal: both switches show in the wall and can be
/// searched.
static void EventScript_RevealSwitches(void) {
  MapGrid_CopyCells(5, 77, 1, 2, 17, 82);
  MapGrid_CopyCells(5, 77, 1, 2, 3, 55);
  MapGrid_CopyCellAttributes(15, 33, 1, 1, 17, 35);
  MapGrid_CopyCellAttributes(3, 8, 1, 1, 3, 10);
}

/// LunpaFortressCave, as Reveal ends: both switches hide again.
static void EventScript_HideSwitches(void) {
  MapGrid_CopyCells(8, 77, 1, 2, 17, 82);
  MapGrid_CopyCells(8, 77, 1, 2, 3, 55);
  MapGrid_CopyCellAttributes(18, 35, 1, 1, 17, 35);
  MapGrid_CopyCellAttributes(2, 10, 1, 1, 3, 10);
}

/// LunpaFortressCave, Whirlwind on what covers the first switch: it blows away.
static void EventScript_BlowAwayFirstSwitchCover(void) {
  Engine_GameFlagSet(FLAG_LUNPA_FORTRESS_FIRST_SWITCH_PLANT_BLOWN_AWAY);
  Engine_GameFlagClear(FLAG_MAP_TEMP_17);
  Engine_ObjectEventSetPosition(CAVE_FIRST_SWITCH_COVER, 0, 0);
}

/// LunpaFortressCave, Whirlwind on what covers the passage: it blows away and
/// the passage opens.
static void EventScript_BlowAwayPassageCover(void) {
  Engine_GameFlagSet(FLAG_LUNPA_FORTRESS_PASSAGE_PLANT_BLOWN_AWAY);
  LunpaFortress_OpenCoveredPassage();
  Engine_ObjectEventSetPosition(CAVE_PASSAGE_COVER, 0, 0);
}

/// LunpaFortressCave, Whirlwind on what covers the second switch: it blows
/// away.
static void EventScript_BlowAwaySecondSwitchCover(void) {
  Engine_GameFlagSet(FLAG_LUNPA_FORTRESS_SECOND_SWITCH_PLANT_BLOWN_AWAY);
  Engine_GameFlagClear(FLAG_MAP_TEMP_18);
  Engine_ObjectEventSetPosition(CAVE_SECOND_SWITCH_COVER, 0, 0);
}

static void UNUSED LunpaFortress_DoNothingE(void) {}

/// Garet, Ivan and Mia step out of the leader and gather around Hammet.
static void LunpaFortress_GatherPartyAroundHammet(void) {
  struct ObjectEvent *leader;

  leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  if (leader != NULL) {
    ObjectEvent_SetPosition(IVAN, leader->x.fixed, leader->z.fixed);
  }
  leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  if (leader != NULL) {
    ObjectEvent_SetPosition(MIA, leader->x.fixed, leader->z.fixed);
  }
  leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  if (leader != NULL) {
    ObjectEvent_SetPosition(GARET, leader->x.fixed, leader->z.fixed);
  }
  OBJECT_DIR(LOCALID_PARTY_LEADER, FACING_EAST, 0);
  OBJECT_SPEED(IVAN, PIXELS(7) / 10, PIXELS(7) / 20);
  OBJECT_WALK(IVAN, 456, 192);
  OBJECT_SPEED(MIA, PIXELS(7) / 10, PIXELS(7) / 20);
  OBJECT_WALK(MIA, 440, 184);
  OBJECT_SPEED(GARET, PIXELS(7) / 10, PIXELS(7) / 20);
  OBJECT_WALK(GARET, 448, 240);
  Engine_ObjectEventWaitForMove(IVAN);
  OBJECT_FACE(IVAN, TOADONPA_HAMMET, 0);
  Engine_ObjectEventWaitForMove(GARET);
  Engine_ObjectEventWaitForMove(MIA);
  OBJECT_FACE(GARET, TOADONPA_HAMMET, 0);
  OBJECT_FACE(MIA, TOADONPA_HAMMET, 0);
  WAIT(15);
}

/// Dodonpa's lever: the room shakes, the floor opens and everyone startles.
static void LunpaFortress_ActivateLeverTrap(void) {
  MapGrid_SetShake(PIXELS(4), PIXELS(4), Q16_ONE);
  SE(SE_RUMBLE);
  WAIT(80);
  SE(SE_STOP_SLOT3);
  WAIT(5);
  SE(SE_145);
  MapGrid_CopyCells(16, 75, 7, 4, 26, 55);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  OBJECT_EMOTE(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  OBJECT_EMOTE(GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  OBJECT_EMOTE(IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  OBJECT_EMOTE(MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  OBJECT_EMOTE(TOADONPA_HAMMET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  WAIT(60);
}

/// The camera finds Toadonpa, the monster Dodonpa has freed; it lumbers
/// forward.
static void LunpaFortress_RevealMonster(void) {
  Engine_CameraMoveToObjectEvent(TOADONPA_BOSS, 1);
  Engine_CameraWaitForMove();
  WAIT(60);
  MESSAGE_SET(MsgLunpaPrepareBecomeMonster);
  MESSAGE_PUT(TOADONPA_DODONPA, 0);
  OBJECT_SPEED(TOADONPA_BOSS, PIXELS(1), PIXELS(1) / 2);
  OBJECT_SPEED(TOADONPA_BOSS_PART, PIXELS(1), PIXELS(1) / 2);
  OBJECT_WALK(TOADONPA_BOSS, 472, 180);
  OBJECT_WALK(TOADONPA_BOSS_PART, 472, 180);
  Engine_CameraFollowObjectEvent(TOADONPA_BOSS, 1);
  Engine_ObjectEventWaitForMove(TOADONPA_BOSS);
  OBJECT_ANIMATION(TOADONPA_BOSS, 4);
  WAIT(30);
}

/// The party spreads out and faces Toadonpa.
static void LunpaFortress_PositionPartyForMonster(void) {
  OBJECT_SPEED(IVAN, PIXELS(1), PIXELS(1) / 2);
  OBJECT_WALK(IVAN, 504, 216);
  OBJECT_SPEED(MIA, PIXELS(1), PIXELS(1) / 2);
  OBJECT_WALK(MIA, 440, 232);
  OBJECT_SPEED(GARET, PIXELS(1), PIXELS(1) / 2);
  OBJECT_WALK(GARET, 480, 224);
  Engine_ObjectEventWaitForMove(GARET);
  OBJECT_DIR(GARET, FACING_NORTH, 0);
  Engine_ObjectEventWaitForMove(IVAN);
  OBJECT_DIR(IVAN, FACING_NORTH, 0);
  Engine_ObjectEventWaitForMove(MIA);
  OBJECT_DIR(MIA, FACING_NORTH, 0);
  OBJECT_DIR(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
}

/// LunpaFortressToadonpa, coord event 20: walking into Hammet's cell. The first
/// time, Hammet hears the party out and recognizes Ivan, then Dodonpa arrives,
/// gloats, pulls a lever and frees Toadonpa on them; after a lost battle, the
/// scene goes straight to the trap. Either way, the battle follows.
static void EventScript_FindHammet(void) {
  s32 messageBase;

  if (FLAG_CHECK(FLAG_MAPSEC_TEMP_01) != 0)
    return;
  FLAG_SET(FLAG_MAP_TEMP_70);
  SCRIPT_START();
  // A retry skips the introduction but repeats Dodonpa's trap and battle.
  if (FLAG_CHECK(FLAG_LUNPA_FORTRESS_TRAP_SEEN) != 0) {
    OBJECT_SPEED(LOCALID_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
    ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 456, 216);
    OBJECT_FACE(LOCALID_PARTY_LEADER, TOADONPA_HAMMET, 0);
    LunpaFortress_GatherPartyAroundHammet();
    OBJECT_EMOTE(TOADONPA_HAMMET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
    OBJECT_DIR(TOADONPA_HAMMET, FACING_WEST, 0);
    Engine_ObjectEventJump(TOADONPA_HAMMET, 4, 0);
    Engine_ObjectEventSetSpriteFlags(ObjectEvent_Get(TOADONPA_HAMMET), 1);
    WAIT(30);
    OBJECT_SPEED(IVAN, PIXELS(7) / 10, PIXELS(7) / 20);
    OBJECT_WALK(IVAN, 464, 192);
    Engine_ObjectEventWaitForMove(IVAN);
    WAIT(30);
    OBJECT_DIR(LOCALID_PARTY_LEADER, FACING_SOUTH, 0);
    OBJECT_DIR(IVAN, FACING_SOUTH, 0);
    OBJECT_DIR(GARET, FACING_SOUTH, 0);
    OBJECT_DIR(MIA, FACING_SOUTH, 0);
    // Dodonpa approaches from the south, then springs the lever trap.
    OBJECT_POSITION_SET(TOADONPA_DODONPA, 456, 320);
    FieldCamera_SetSpeed(PIXELS(2), PIXELS(1) / 4);
    messageBase = MsgLunpaBackMoreGuess;
    MESSAGE_SET(messageBase);
    MESSAGE_PUT(TOADONPA_DODONPA, 0);
    OBJECT_WALK(TOADONPA_DODONPA, 458, 272);
    Engine_ObjectEventWaitForMove(TOADONPA_DODONPA);
    OBJECT_DIR(TOADONPA_DODONPA, FACING_SOUTH + FACING_STEP, 0);
    WAIT(40);
    ObjectEvent_SetDestinationOffset(TOADONPA_DODONPA, -8, 8);
    Engine_ObjectEventWaitForMove(TOADONPA_DODONPA);
    WAIT(60);
    SE(SE_155);
    Engine_MessageShowCentered(messageBase + 1, 1);
    ObjectEvent_SetDestinationOffset(TOADONPA_DODONPA, 8, -8);
    LunpaFortress_ActivateLeverTrap();
    WAIT(120);
    Engine_ObjectEventStartRepeatedMotion(LOCALID_PARTY_LEADER, 2);
    Engine_ObjectEventStartRepeatedMotion(IVAN, 2);
    Engine_ObjectEventStartRepeatedMotion(GARET, 2);
    Engine_ObjectEventStartRepeatedMotion(MIA, 2);
    WAIT(20);
    messageBase += 2;
    LunpaFortress_PositionPartyForMonster();
    OBJECT_FACE(TOADONPA_DODONPA, LOCALID_PARTY_LEADER, 0);
    MESSAGE_SET(messageBase);
    MESSAGE_PUT(TOADONPA_DODONPA, 0);
    LunpaFortress_RevealMonster();
    gFieldWork->transition =
        MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
    WAIT(1);
    Battle_SetType(BATTLE_TYPE_SCRIPTED);
    Engine_ScriptSetNextDestination((s32)&MapId_LunpaFortressToadonpa,
                                    LUNPA_FORTRESS_TOADONPA_AFTER_BATTLE);
    Engine_EncounterRequestBattle(ENCOUNTER_ZONE_SCRIPTED, 4);
  } else {
    OBJECT_ANIMATION(LOCALID_PARTY_LEADER, ANIMATION_STAND);
    SE(AUDIO_CUE_FADE_BGM);
    WAIT(30);
    // Ivan recognizes Hammet before Dodonpa interrupts the reunion.
    messageBase = MsgLunpaTimeEat;
    MESSAGE_SET(messageBase);
    MESSAGE_PUT(TOADONPA_HAMMET, 0);
    Engine_ObjectEventFaceEachOther(LOCALID_PARTY_LEADER, TOADONPA_HAMMET, 0);
    WAIT(140);
    OBJECT_DIR(TOADONPA_HAMMET, FACING_WEST, 0);
    Engine_ObjectEventJump(TOADONPA_HAMMET, 4, 0);
    Engine_ObjectEventSetSpriteFlags(ObjectEvent_Get(TOADONPA_HAMMET), 1);
    MESSAGE_SET(messageBase + 1);
    MESSAGE_PUT(TOADONPA_HAMMET, 0);
    OBJECT_SPEED(LOCALID_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
    ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 456, 216);
    OBJECT_FACE(LOCALID_PARTY_LEADER, TOADONPA_HAMMET, 0);
    LunpaFortress_GatherPartyAroundHammet();
    OBJECT_SPEED(IVAN, PIXELS(7) / 10, PIXELS(7) / 20);
    OBJECT_WALK(IVAN, 464, 192);
    Engine_ObjectEventWaitForMove(IVAN);
    WAIT(30);
    MESSAGE_SET(messageBase + 2);
    MESSAGE_PUT(IVAN, 0);
    OBJECT_EMOTE(TOADONPA_HAMMET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    WAIT(110);
    SE(MUS_060);
    MESSAGE_SET(messageBase + 3);
    MESSAGE_PUT(TOADONPA_HAMMET, 0);
    WAIT(30);
    Engine_ObjectEventSetAnimationAndWait(IVAN, ANIMATION_NOD);
    WAIT(20);
    OBJECT_ANIMATION(IVAN, ANIMATION_STAND);
    Engine_ObjectEventStartRepeatedMotion(TOADONPA_HAMMET, 1);
    WAIT(20);
    OBJECT_SPEED(TOADONPA_HAMMET, PIXELS(2) / 5, PIXELS(1) / 5);
    OBJECT_WALK(TOADONPA_HAMMET, 520, 208);
    Engine_ObjectEventWaitForMove(TOADONPA_HAMMET);
    OBJECT_ANIMATION(TOADONPA_HAMMET, 1);
    WAIT(20);
    OBJECT_DIR(TOADONPA_HAMMET, FACING_NORTHWEST + FACING_STEP, 0);
    WAIT(30);
    OBJECT_DIR(TOADONPA_HAMMET, FACING_SOUTH + FACING_STEP, 0);
    WAIT(30);
    OBJECT_FACE(TOADONPA_HAMMET, IVAN, 0);
    WAIT(20);
    MESSAGE_SET(messageBase + 4);
    MESSAGE_PUT(TOADONPA_HAMMET, 0);
    WAIT(40);
    Engine_ObjectEventSetAnimationAndWait(IVAN, ANIMATION_NOD);
    WAIT(20);
    OBJECT_EMOTE(TOADONPA_HAMMET, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
    WAIT(120);
    MESSAGE_SET(messageBase + 5);
    MESSAGE_PUT(TOADONPA_HAMMET, 0);
    WAIT(25);
    Engine_ObjectEventSetAnimationAndWait(IVAN, ANIMATION_NOD);
    WAIT(30);
    Engine_ObjectEventSetAnimationAndWait(TOADONPA_HAMMET, 3);
    WAIT(40);
    OBJECT_WALK(IVAN, 480, 200);
    Engine_ObjectEventWaitForMove(IVAN);
    Engine_ObjectEventFaceEachOther(IVAN, TOADONPA_HAMMET, 0);
    WAIT(60);
    MESSAGE_SET(messageBase + 6);
    MESSAGE_PUT(IVAN, 0);
    WAIT(20);
    OBJECT_ANIMATION(TOADONPA_HAMMET, 4);
    WAIT(80);
    MESSAGE_SET(messageBase + 7);
    MESSAGE_PUT(TOADONPA_HAMMET, 0);
    // Dodonpa approaches from the south, then springs the lever trap.
    OBJECT_POSITION_SET(TOADONPA_DODONPA, 456, 320);
    SE(MUS_SILENCE);
    MESSAGE_SET(messageBase + 8);
    MESSAGE_PUT(TOADONPA_DODONPA, 0);
    OBJECT_FACE(LOCALID_PARTY_LEADER, TOADONPA_DODONPA, 0);
    OBJECT_FACE(IVAN, TOADONPA_DODONPA, 0);
    OBJECT_FACE(GARET, TOADONPA_DODONPA, 0);
    WAIT(5);
    OBJECT_DIR(MIA, FACING_SOUTH, 0);
    OBJECT_FACE(TOADONPA_HAMMET, TOADONPA_DODONPA, 0);
    WAIT(30);
    SE(MUS_061);
    FieldCamera_SetSpeed(PIXELS(2), PIXELS(1) / 4);
    Engine_CameraMoveToObjectEvent(TOADONPA_DODONPA, 1);
    Engine_CameraWaitForMove();
    OBJECT_SPEED(TOADONPA_DODONPA, PIXELS(4) / 5, PIXELS(2) / 5);
    OBJECT_WALK(TOADONPA_DODONPA, 456, 304);
    Engine_CameraFollowObjectEvent(TOADONPA_DODONPA, 1);
    Engine_ObjectEventWaitForMove(TOADONPA_DODONPA);
    Engine_CameraFollowObjectEvent(GARET, 1);
    OBJECT_EMOTE(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    OBJECT_EMOTE(IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    OBJECT_EMOTE(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    OBJECT_EMOTE(MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    OBJECT_EMOTE(TOADONPA_HAMMET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    WAIT(60);
    OBJECT_FACE(TOADONPA_HAMMET, TOADONPA_DODONPA, 0);
    Engine_ObjectEventStartRepeatedMotion(TOADONPA_HAMMET, 2);
    WAIT(60);
    MESSAGE_SET(messageBase + 9);
    MESSAGE_PUT(TOADONPA_HAMMET, 0);
    OBJECT_FACE(TOADONPA_DODONPA, TOADONPA_HAMMET, 0);
    MESSAGE_SET(messageBase + 10);
    MESSAGE_PUT(TOADONPA_DODONPA, 0);
    WAIT(60);
    OBJECT_FACE(TOADONPA_DODONPA, IVAN, 0);
    WAIT(30);
    MESSAGE_SET(messageBase + 11);
    MESSAGE_PUT(TOADONPA_DODONPA, 0);
    Engine_ObjectEventFaceEachOther(MIA, IVAN, 0);
    Engine_ObjectEventFaceEachOther(LOCALID_PARTY_LEADER, GARET, 0);
    WAIT(60);
    OBJECT_FACE(LOCALID_PARTY_LEADER, TOADONPA_DODONPA, 0);
    OBJECT_FACE(IVAN, TOADONPA_DODONPA, 0);
    OBJECT_FACE(GARET, TOADONPA_DODONPA, 0);
    OBJECT_FACE(MIA, TOADONPA_DODONPA, 0);
    Engine_ObjectEventStartRepeatedMotion(TOADONPA_DODONPA, 1);
    WAIT(60);
    MESSAGE_SET(messageBase + 12);
    MESSAGE_PUT(TOADONPA_DODONPA, 0);
    OBJECT_EMOTE(GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
    WAIT(60);
    OBJECT_ANIMATION(TOADONPA_DODONPA, 4);
    MESSAGE_SET(messageBase + 13);
    MESSAGE_PUT(TOADONPA_DODONPA, 0);
    OBJECT_WALK(GARET, 456, 248);
    Engine_ObjectEventWaitForMove(GARET);
    OBJECT_DIR(GARET, FACING_SOUTH, 0);
    MESSAGE_SET(messageBase + 14);
    MESSAGE_PUT(GARET, 0);
    OBJECT_WALK(IVAN, 472, 216);
    Engine_ObjectEventWaitForMove(IVAN);
    OBJECT_DIR(IVAN, FACING_SOUTH, 0);
    WAIT(10);
    MESSAGE_SET(messageBase + 15);
    MESSAGE_PUT(IVAN, 0);
    OBJECT_EMOTE(TOADONPA_HAMMET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
    WAIT(60);
    MESSAGE_SET(messageBase + 16);
    MESSAGE_PUT(TOADONPA_HAMMET, 0);
    OBJECT_WALK(MIA, 440, 216);
    Engine_ObjectEventWaitForMove(MIA);
    OBJECT_FACE(MIA, TOADONPA_DODONPA, 0);
    Engine_ObjectEventSetAnimationAndWait(MIA, ANIMATION_NOD);
    WAIT(10);
    MESSAGE_SET(messageBase + 17);
    MESSAGE_PUT(MIA, 0);
    OBJECT_ANIMATION(TOADONPA_DODONPA, 4);
    MESSAGE_SET(messageBase + 18);
    MESSAGE_PUT(TOADONPA_DODONPA, 0);
    OBJECT_EMOTE(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    OBJECT_EMOTE(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    OBJECT_EMOTE(MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    OBJECT_EMOTE(IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    OBJECT_EMOTE(TOADONPA_DODONPA, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
    WAIT(60);
    MESSAGE_SET(messageBase + 19);
    MESSAGE_PUT(TOADONPA_DODONPA, 0);
    WAIT(20);
    OBJECT_EMOTE(GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
    WAIT(60);
    MESSAGE_SET(messageBase + 20);
    MESSAGE_PUT(GARET, 0);
    Engine_ObjectEventStartRepeatedMotion(TOADONPA_DODONPA, 1);
    WAIT(60);
    MESSAGE_SET(messageBase + 21);
    MESSAGE_PUT(TOADONPA_DODONPA, 0);
    OBJECT_WALK(TOADONPA_DODONPA, 456, 280);
    Engine_ObjectEventWaitForMove(TOADONPA_DODONPA);
    OBJECT_DIR(TOADONPA_DODONPA, FACING_SOUTH + FACING_STEP, 0);
    WAIT(80);
    ObjectEvent_SetDestinationOffset(TOADONPA_DODONPA, -8, 8);
    Engine_ObjectEventWaitForMove(TOADONPA_DODONPA);
    WAIT(60);
    SE(SE_155);
    Engine_MessageShowCentered(MsgLunpaDodonpaPulledLever, 1);
    ObjectEvent_SetDestinationOffset(TOADONPA_DODONPA, 8, -8);
    OBJECT_FACE(TOADONPA_DODONPA, TOADONPA_BOSS, 0);
    LunpaFortress_ActivateLeverTrap();
    SE(MUS_BOSS_BATTLE);
    MESSAGE_SET(messageBase + 23);
    MESSAGE_PUT(TOADONPA_DODONPA, 0);
    WAIT(60);
    OBJECT_FACE(LOCALID_PARTY_LEADER, TOADONPA_BOSS, 0);
    OBJECT_FACE(GARET, TOADONPA_BOSS, 0);
    OBJECT_FACE(IVAN, TOADONPA_BOSS, 0);
    OBJECT_FACE(MIA, TOADONPA_BOSS, 0);
    OBJECT_FACE(TOADONPA_HAMMET, TOADONPA_BOSS, 0);
    LunpaFortress_PositionPartyForMonster();
    LunpaFortress_RevealMonster();
    FLAG_SET(FLAG_LUNPA_FORTRESS_TRAP_SEEN);
    gFieldWork->transition =
        MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
    WAIT(1);
    Battle_SetType(BATTLE_TYPE_SCRIPTED);
    Engine_ScriptSetNextDestination((s32)&MapId_LunpaFortressToadonpa,
                                    LUNPA_FORTRESS_TOADONPA_AFTER_BATTLE);
    Engine_EncounterRequestBattle(ENCOUNTER_ZONE_SCRIPTED, 4);
  }
  SCRIPT_END();
}

/// LunpaFortressToadonpa, LUNPA_FORTRESS_TOADONPA_AFTER_BATTLE after the battle
/// with Toadonpa: the monster has fallen on Dodonpa. The party may pull it off
/// him; they argue over his punishment until his father Donpa arrives, who
/// orders him locked up, thanks the party, and lets them leave with Hammet.
/// Sets FLAG_DODONPA_LOCKED_UP.
static void EventScript_DonpaJudgesDodonpa(void) {
  struct ObjectEvent *leader;
  s32 aftermathMessage;
  s32 punishmentMessage;
  s32 donpaMessage;

  FLAG_SET(FLAG_MAPSEC_TEMP_01);
  FLAG_SET(FLAG_DODONPA_LOCKED_UP);
  MapGrid_CopyCells(16, 75, 7, 4, 26, 55);
  LunpaFortress_OpenToadonpaDoor(4);
  SCRIPT_START();
  leader = ObjectEvent_Get(TOADONPA_HAMMET);
  Engine_ObjectEventSetSpriteFlags(leader, 1);
  OBJECT_POSITION_SET(LOCALID_PARTY_LEADER, 456, 184);
  OBJECT_DIR(LOCALID_PARTY_LEADER, FACING_SOUTH, 0);
  OBJECT_POSITION_SET(GARET, 440, 192);
  OBJECT_DIR(GARET, FACING_SOUTH, 0);
  OBJECT_POSITION_SET(MIA, 488, 184);
  OBJECT_DIR(MIA, FACING_SOUTH, 0);
  OBJECT_POSITION_SET(IVAN, 472, 184);
  OBJECT_DIR(IVAN, FACING_SOUTH, 0);
  OBJECT_POSITION_SET(TOADONPA_HAMMET, 520, 224);
  OBJECT_DIR(TOADONPA_HAMMET, FACING_NORTHWEST + FACING_STEP, 0);
  OBJECT_POSITION_SET(TOADONPA_BOSS, 448, 237);
  OBJECT_DIR(TOADONPA_BOSS, FACING_WEST, 0);
  OBJECT_POSITION_SET(TOADONPA_BOSS_PART, 448, 238);
  Engine_ObjectEventSetSpritePriority(TOADONPA_BOSS_PART, 3);
  OBJECT_ANIMATION(TOADONPA_BOSS, 3);
  OBJECT_POSITION_SET(TOADONPA_DODONPA, 458, 243);
  OBJECT_DIR(TOADONPA_DODONPA, FACING_SOUTH, 0);
  OBJECT_ANIMATION(TOADONPA_DODONPA, 5);
  Engine_ScriptOpenScreen();
  WAIT(120);

  // The fallen monster has pinned Dodonpa; decide whether to help him.
  aftermathMessage = MsgLunpaUhnnGetOff;
  MESSAGE_SET(aftermathMessage);
  MESSAGE_PUT(TOADONPA_DODONPA, 0);
  OBJECT_EMOTE(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  MESSAGE_SET((aftermathMessage + 1));
  MESSAGE_PUT(GARET, 0);
  OBJECT_EMOTE(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 1);
  OBJECT_EMOTE(IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 1);
  OBJECT_EMOTE(MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 1);
  WAIT(60);
  OBJECT_FACE(TOADONPA_HAMMET, TOADONPA_DODONPA, 0);
  WAIT(60);
  MESSAGE_SET((aftermathMessage + 2));
  MESSAGE_PUT(TOADONPA_HAMMET, 0);
  OBJECT_FACE(LOCALID_PARTY_LEADER, TOADONPA_HAMMET, 0);
  OBJECT_FACE(GARET, TOADONPA_HAMMET, 0);
  OBJECT_FACE(IVAN, TOADONPA_HAMMET, 0);
  OBJECT_FACE(MIA, TOADONPA_HAMMET, 0);
  WAIT(60);
  OBJECT_WALK(TOADONPA_HAMMET, 512, 232);
  OBJECT_ANIMATION(TOADONPA_HAMMET, 4);
  WAIT(60);
  MESSAGE_SET((aftermathMessage + 3));
  MESSAGE_PUT(TOADONPA_HAMMET, 0);
  WAIT(15);
  Engine_ObjectEventStartRepeatedMotion(TOADONPA_DODONPA, 2);
  MESSAGE_SET((aftermathMessage + 4));
  MESSAGE_PUT(TOADONPA_DODONPA, 0);
  OBJECT_FACE(LOCALID_PARTY_LEADER, TOADONPA_DODONPA, 0);
  OBJECT_FACE(IVAN, TOADONPA_DODONPA, 0);
  OBJECT_FACE(MIA, TOADONPA_DODONPA, 0);
  OBJECT_FACE(GARET, TOADONPA_DODONPA, 0);
  Engine_ObjectEventStartRepeatedMotion(IVAN, 1);
  MESSAGE_SET((aftermathMessage + 5));
  MESSAGE_PUT(IVAN, 0);
  OBJECT_EMOTE(MIA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  OBJECT_ANIMATION(MIA, ANIMATION_SHAKE_HEAD);
  WAIT(80);
  OBJECT_ANIMATION(MIA, ANIMATION_STAND);
  MESSAGE_SET((aftermathMessage + 6));
  MESSAGE_PUT(MIA, 0);
  OBJECT_EMOTE(TOADONPA_HAMMET, EMOTE_IN_FRONT | EMOTE_WINCE, 65);
  Engine_ObjectEventStartRepeatedMotion(TOADONPA_HAMMET, 2);
  WAIT(100);
  MESSAGE_SET((aftermathMessage + 7));
  MESSAGE_PUT(TOADONPA_DODONPA, 0);
  OBJECT_FACE(GARET, LOCALID_PARTY_LEADER, 0);
  WAIT(10);
  MESSAGE_SET((aftermathMessage + 8));
  MESSAGE_PUT(GARET, 0);
  Engine_ObjectEventStartRepeatedMotion(IVAN, 1);
  WAIT(30);
  MESSAGE_SET((aftermathMessage + 9));
  MESSAGE_PUT(IVAN, 0);
  OBJECT_FACE(MIA, LOCALID_PARTY_LEADER, 0);
  MESSAGE_SET((aftermathMessage + 10));
  MESSAGE_OPEN(MIA, 0);
  if (Engine_ScriptChooseYesNo(0, 0) == 0) {
    OBJECT_ANIMATION(IVAN, ANIMATION_NOD);
    OBJECT_ANIMATION(GARET, ANIMATION_NOD);
    OBJECT_ANIMATION(MIA, ANIMATION_NOD);
    OBJECT_FACE(TOADONPA_HAMMET, TOADONPA_DODONPA, 0);
    WAIT(60);
  } else {
    MESSAGE_SET((aftermathMessage + 11));
    MESSAGE_PUT(MIA, 0);
    OBJECT_FACE(LOCALID_PARTY_LEADER, TOADONPA_HAMMET, 0);
    OBJECT_FACE(GARET, TOADONPA_HAMMET, 0);
    OBJECT_FACE(IVAN, TOADONPA_HAMMET, 0);
    OBJECT_FACE(MIA, TOADONPA_HAMMET, 0);
    WAIT(20);
    OBJECT_FACE(TOADONPA_HAMMET, LOCALID_PARTY_LEADER, 0);
    WAIT(60);
    OBJECT_FACE(TOADONPA_HAMMET, TOADONPA_DODONPA, 0);
    WAIT(80);
    Engine_ObjectEventSetAnimationAndWait(TOADONPA_HAMMET, 3);
    WAIT(30);
    OBJECT_FACE(TOADONPA_HAMMET, LOCALID_PARTY_LEADER, 0);
    MESSAGE_SET((aftermathMessage + 12));
    MESSAGE_PUT(TOADONPA_HAMMET, 0);
    WAIT(60);
    OBJECT_FACE(IVAN, TOADONPA_HAMMET, 0);
    OBJECT_FACE(GARET, TOADONPA_HAMMET, 0);
    OBJECT_FACE(MIA, TOADONPA_HAMMET, 0);
    OBJECT_FACE(LOCALID_PARTY_LEADER, TOADONPA_HAMMET, 0);
    OBJECT_ANIMATION(IVAN, ANIMATION_NOD);
    OBJECT_ANIMATION(GARET, ANIMATION_NOD);
    OBJECT_ANIMATION(MIA, ANIMATION_NOD);
    OBJECT_ANIMATION(LOCALID_PARTY_LEADER, ANIMATION_NOD);
    WAIT(80);
    OBJECT_FACE(TOADONPA_HAMMET, TOADONPA_DODONPA, 0);
    OBJECT_FACE(IVAN, TOADONPA_DODONPA, 0);
    OBJECT_FACE(GARET, TOADONPA_DODONPA, 0);
    OBJECT_FACE(MIA, TOADONPA_DODONPA, 0);
    OBJECT_FACE(LOCALID_PARTY_LEADER, TOADONPA_DODONPA, 0);
  }
  OBJECT_SPEED(LOCALID_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
  OBJECT_SPEED(GARET, PIXELS(1) / 2, PIXELS(1) / 4);
  OBJECT_SPEED(MIA, PIXELS(1) / 2, PIXELS(1) / 4);
  OBJECT_SPEED(IVAN, PIXELS(1) / 2, PIXELS(1) / 4);
  OBJECT_WALK(GARET, 416, 216);
  Engine_ObjectEventWaitForMove(GARET);
  OBJECT_WALK(GARET, 416, 248);
  Engine_ObjectEventWaitForMove(GARET);
  OBJECT_WALK(GARET, 440, 248);
  Engine_ObjectEventWaitForMove(GARET);
  OBJECT_DIR(GARET, FACING_NORTH, 0);
  OBJECT_WALK(LOCALID_PARTY_LEADER, 440, 216);
  Engine_ObjectEventWaitForMove(LOCALID_PARTY_LEADER);
  OBJECT_DIR(LOCALID_PARTY_LEADER, FACING_EAST, 0);
  OBJECT_WALK(MIA, 488, 248);
  Engine_ObjectEventWaitForMove(MIA);
  OBJECT_WALK(MIA, 456, 248);
  Engine_ObjectEventWaitForMove(MIA);
  OBJECT_DIR(MIA, FACING_NORTH, 0);
  OBJECT_WALK(IVAN, 456, 216);
  Engine_ObjectEventWaitForMove(IVAN);
  OBJECT_DIR(IVAN, FACING_WEST, 0);
  Engine_ObjectEventFaceEachOther(GARET, LOCALID_PARTY_LEADER, 0);
  Engine_ObjectEventFaceEachOther(MIA, IVAN, 0);
  WAIT(60);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  OBJECT_ANIMATION(IVAN, ANIMATION_NOD);
  OBJECT_ANIMATION(GARET, ANIMATION_NOD);
  OBJECT_ANIMATION(MIA, ANIMATION_NOD);
  WAIT(100);
  SE(SE_226);
  OBJECT_ANIMATION(TOADONPA_DODONPA, 7);
  // Move the party and both monster parts together to free Dodonpa.
  OBJECT_SPEED(LOCALID_PARTY_LEADER, PIXELS(1) / 10, PIXELS(1) / 20);
  ObjectEvent_SetDestinationOffset(LOCALID_PARTY_LEADER, -24, 0);
  OBJECT_SPEED(GARET, PIXELS(1) / 10, PIXELS(1) / 20);
  ObjectEvent_SetDestinationOffset(GARET, -24, 0);
  OBJECT_SPEED(MIA, PIXELS(1) / 10, PIXELS(1) / 20);
  ObjectEvent_SetDestinationOffset(MIA, -24, 0);
  OBJECT_SPEED(IVAN, PIXELS(1) / 10, PIXELS(1) / 20);
  ObjectEvent_SetDestinationOffset(IVAN, -24, 0);
  OBJECT_SPEED(TOADONPA_BOSS, PIXELS(1) / 10, PIXELS(1) / 20);
  OBJECT_SPEED(TOADONPA_BOSS_PART, PIXELS(1) / 10, PIXELS(1) / 20);
  ObjectEvent_SetDestinationOffset(TOADONPA_BOSS, -24, 0);
  ObjectEvent_SetDestinationOffset(TOADONPA_BOSS_PART, -24, 0);
  Engine_ObjectEventWaitForMove(LOCALID_PARTY_LEADER);
  SE(SE_STOP_SLOT3);
  WAIT(60);
  OBJECT_DIR(LOCALID_PARTY_LEADER, FACING_SOUTH, 0);
  OBJECT_DIR(GARET, FACING_NORTH, 0);
  OBJECT_DIR(IVAN, FACING_SOUTH, 0);
  OBJECT_DIR(MIA, FACING_NORTH, 0);
  WAIT(60);
  OBJECT_ANIMATION(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  OBJECT_ANIMATION(GARET, ANIMATION_NOD);
  OBJECT_ANIMATION(IVAN, ANIMATION_NOD);
  OBJECT_ANIMATION(MIA, ANIMATION_NOD);
  WAIT(100);
  OBJECT_SPEED(LOCALID_PARTY_LEADER, PIXELS(7) / 10, PIXELS(7) / 20);
  OBJECT_SPEED(GARET, PIXELS(7) / 10, PIXELS(7) / 20);
  OBJECT_SPEED(IVAN, PIXELS(7) / 10, PIXELS(7) / 20);
  OBJECT_SPEED(MIA, PIXELS(7) / 10, PIXELS(7) / 20);
  OBJECT_WALK(LOCALID_PARTY_LEADER, 456, 184);
  Engine_ObjectEventWaitForMove(LOCALID_PARTY_LEADER);
  OBJECT_ANIMATION(LOCALID_PARTY_LEADER, ANIMATION_STAND);
  OBJECT_DIR(LOCALID_PARTY_LEADER, FACING_SOUTH, 0);
  OBJECT_WALK(GARET, 464, 256);
  Engine_ObjectEventWaitForMove(GARET);
  OBJECT_WALK(GARET, 480, 248);
  Engine_ObjectEventWaitForMove(GARET);
  OBJECT_WALK(GARET, 440, 192);
  Engine_ObjectEventWaitForMove(GARET);
  OBJECT_ANIMATION(GARET, ANIMATION_STAND);
  OBJECT_DIR(GARET, FACING_SOUTH, 0);
  OBJECT_WALK(MIA, 488, 248);
  Engine_ObjectEventWaitForMove(MIA);
  OBJECT_WALK(MIA, 488, 184);
  OBJECT_WALK(IVAN, 472, 184);
  Engine_ObjectEventWaitForMove(IVAN);
  OBJECT_ANIMATION(IVAN, ANIMATION_STAND);
  OBJECT_DIR(IVAN, FACING_SOUTH, 0);
  Engine_ObjectEventWaitForMove(MIA);
  OBJECT_ANIMATION(MIA, ANIMATION_STAND);
  OBJECT_DIR(MIA, FACING_SOUTH, 0);
  WAIT(30);
  WAIT(60);
  // Debate Dodonpa's punishment before his father arrives.
  punishmentMessage = MsgLunpaAbleGet;
  MESSAGE_SET(punishmentMessage);
  MESSAGE_PUT(GARET, 0);
  OBJECT_ANIMATION(TOADONPA_DODONPA, 6);
  WAIT(120);
  Engine_ObjectEventStartRepeatedMotion(TOADONPA_DODONPA, 2);
  WAIT(60);
  OBJECT_ANIMATION(TOADONPA_DODONPA, 7);
  MESSAGE_SET((punishmentMessage + 1));
  MESSAGE_PUT(TOADONPA_DODONPA, 0);
  WAIT(20);
  Engine_ObjectEventFaceEachOther(MIA, IVAN, 0);
  WAIT(10);
  OBJECT_EMOTE(MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  OBJECT_FACE(MIA, TOADONPA_DODONPA, 0);
  OBJECT_FACE(IVAN, TOADONPA_DODONPA, 0);
  MESSAGE_SET((punishmentMessage + 2));
  MESSAGE_PUT(MIA, 0);
  OBJECT_EMOTE(TOADONPA_DODONPA, EMOTE_IN_FRONT | EMOTE_WINCE, 70);
  Engine_ObjectEventStartRepeatedMotion(TOADONPA_DODONPA, 2);
  WAIT(60);
  OBJECT_ANIMATION(TOADONPA_DODONPA, 5);
  WAIT(70);
  MESSAGE_SET((punishmentMessage + 3));
  MESSAGE_PUT(TOADONPA_DODONPA, 0);
  OBJECT_ANIMATION(TOADONPA_DODONPA, 7);
  OBJECT_EMOTE(IVAN, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  MESSAGE_SET((punishmentMessage + 4));
  MESSAGE_PUT(IVAN, 0);
  Engine_ObjectEventSetAnimationAndWait(TOADONPA_HAMMET, 3);
  OBJECT_EMOTE(GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 60);
  OBJECT_WALK(GARET, 440, 208);
  Engine_ObjectEventWaitForMove(GARET);
  OBJECT_FACE(GARET, LOCALID_PARTY_LEADER, 0);
  MESSAGE_SET((punishmentMessage + 5));
  MESSAGE_PUT(GARET, 0);
  WAIT(20);
  OBJECT_EMOTE(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  OBJECT_EMOTE(IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  OBJECT_EMOTE(MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  OBJECT_EMOTE(TOADONPA_HAMMET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  WAIT(70);
  OBJECT_FACE(GARET, IVAN, 0);
  OBJECT_EMOTE(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 75);
  OBJECT_FACE(GARET, LOCALID_PARTY_LEADER, 0);
  OBJECT_ANIMATION(GARET, ANIMATION_SHAKE_HEAD);
  MESSAGE_SET((punishmentMessage + 6));
  MESSAGE_PUT(GARET, 0);
  OBJECT_EMOTE(TOADONPA_HAMMET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  WAIT(60);
  MESSAGE_SET((punishmentMessage + 7));
  MESSAGE_PUT(TOADONPA_HAMMET, 0);
  OBJECT_WALK(GARET, 448, 208);
  Engine_ObjectEventWaitForMove(GARET);
  OBJECT_ANIMATION(GARET, ANIMATION_STAND);
  OBJECT_FACE(GARET, TOADONPA_HAMMET, 0);
  MESSAGE_SET((punishmentMessage + 8));
  MESSAGE_PUT(GARET, 0);
  Engine_ObjectEventStartRepeatedMotion(TOADONPA_HAMMET, 1);
  WAIT(60);
  OBJECT_FACE(GARET, TOADONPA_HAMMET, 0);
  WAIT(60);
  MESSAGE_SET((punishmentMessage + 9));
  MESSAGE_PUT(TOADONPA_HAMMET, 0);
  OBJECT_ANIMATION(GARET, ANIMATION_SHAKE_HEAD);
  WAIT(60);
  MESSAGE_SET((punishmentMessage + 10));
  MESSAGE_PUT(GARET, 0);
  Engine_ObjectEventStartRepeatedMotion(IVAN, 1);
  WAIT(30);
  MESSAGE_SET((punishmentMessage + 11));
  MESSAGE_PUT(IVAN, 0);
  OBJECT_FACE(GARET, MIA, 0);
  WAIT(20);
  MESSAGE_SET((punishmentMessage + 12));
  MESSAGE_PUT(GARET, 0);
  WAIT(30);
  OBJECT_EMOTE(MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 80);
  OBJECT_FACE(MIA, GARET, 0);
  MESSAGE_SET((punishmentMessage + 13));
  MESSAGE_PUT(MIA, 0);
  OBJECT_FACE(MIA, TOADONPA_DODONPA, 0);
  MESSAGE_SET((punishmentMessage + 14));
  MESSAGE_PUT(MIA, 0);
  OBJECT_FACE(LOCALID_PARTY_LEADER, TOADONPA_DODONPA, 0);
  OBJECT_FACE(IVAN, TOADONPA_DODONPA, 0);
  OBJECT_FACE(GARET, TOADONPA_DODONPA, 0);
  OBJECT_FACE(MIA, TOADONPA_DODONPA, 0);
  WAIT(120);
  OBJECT_EMOTE(TOADONPA_DODONPA, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  Engine_ObjectEventStartRepeatedMotion(TOADONPA_DODONPA, 1);
  WAIT(120);
  Engine_ObjectEventStartRepeatedMotion(TOADONPA_HAMMET, 1);
  WAIT(60);
  MESSAGE_SET((punishmentMessage + 15));
  MESSAGE_PUT(TOADONPA_HAMMET, 0);
  OBJECT_EMOTE(GARET, EMOTE_IN_FRONT | EMOTE_SCOWL, 110);
  OBJECT_WALK(GARET, 456, 212);
  Engine_ObjectEventWaitForMove(GARET);
  OBJECT_ANIMATION(GARET, ANIMATION_STAND);
  MESSAGE_SET((punishmentMessage + 16));
  MESSAGE_PUT(GARET, 0);
  OBJECT_FACE(IVAN, GARET, 0);
  WAIT(60);
  Engine_ObjectEventStartRepeatedMotion(IVAN, 1);
  MESSAGE_SET((punishmentMessage + 17));
  MESSAGE_PUT(IVAN, 0);
  OBJECT_FACE(GARET, TOADONPA_DODONPA, 0);
  WAIT(80);
  OBJECT_FACE(GARET, IVAN, 0);
  WAIT(60);
  OBJECT_FACE(GARET, TOADONPA_DODONPA, 0);
  WAIT(30);
  OBJECT_FACE(LOCALID_PARTY_LEADER, TOADONPA_DODONPA, 0);
  OBJECT_FACE(MIA, TOADONPA_DODONPA, 0);
  OBJECT_FACE(TOADONPA_DODONPA, TOADONPA_DODONPA, 0);
  OBJECT_FACE(TOADONPA_HAMMET, TOADONPA_DODONPA, 0);
  WAIT(80);
  Engine_ObjectEventSetAnimationAndWait(GARET, ANIMATION_NOD);
  WAIT(30);
  MESSAGE_SET((punishmentMessage + 18));
  MESSAGE_PUT(GARET, 0);
  Engine_ObjectEventStartRepeatedMotion(TOADONPA_DODONPA, 2);
  WAIT(70);
  MESSAGE_SET((punishmentMessage + 19));
  MESSAGE_PUT(TOADONPA_DODONPA, 0);
  OBJECT_EMOTE(MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  MESSAGE_SET((punishmentMessage + 20));
  MESSAGE_PUT(MIA, 0);
  OBJECT_FACE(IVAN, LOCALID_PARTY_LEADER, 0);
  WAIT(80);
  MESSAGE_SET((punishmentMessage + 21));
  MESSAGE_OPEN(IVAN, 0);
  OBJECT_FACE(GARET, LOCALID_PARTY_LEADER, 0);
  OBJECT_FACE(MIA, LOCALID_PARTY_LEADER, 0);
  OBJECT_FACE(IVAN, LOCALID_PARTY_LEADER, 0);
  OBJECT_FACE(TOADONPA_HAMMET, LOCALID_PARTY_LEADER, 0);
  OBJECT_POSITION_SET(TOADONPA_DONPA, 456, 304);
  OBJECT_SPEED(TOADONPA_DONPA, PIXELS(1) / 2, PIXELS(1) / 4);
  if (Engine_ScriptChooseYesNo(0, 0) == 0) {
    SE(MUS_SILENCE);
    MESSAGE_SET((punishmentMessage + 22));
    MESSAGE_PUT(TOADONPA_DONPA, 0);
  } else {
    SE(MUS_SILENCE);
    MESSAGE_SET((punishmentMessage + 23));
    MESSAGE_PUT(TOADONPA_DONPA, 0);
  }
  OBJECT_FACE(LOCALID_PARTY_LEADER, TOADONPA_DONPA, 0);
  OBJECT_FACE(GARET, TOADONPA_DONPA, 0);
  OBJECT_FACE(MIA, TOADONPA_DONPA, 0);
  OBJECT_FACE(IVAN, TOADONPA_DONPA, 0);
  OBJECT_FACE(TOADONPA_HAMMET, TOADONPA_DONPA, 0);
  OBJECT_FACE(TOADONPA_DODONPA, TOADONPA_DONPA, 0);
  FieldCamera_MoveTo(0x1c80000, -1, 0xf00000, 1);
  Engine_ObjectEventStartRepeatedMotion(TOADONPA_DODONPA, 1);
  WAIT(60);
  SE(MUS_008);
  // Donpa takes charge and orders his son to remain imprisoned.
  donpaMessage = MsgLunpaDad;
  MESSAGE_SET(donpaMessage);
  MESSAGE_PUT(TOADONPA_DODONPA, 0);
  OBJECT_WALK(TOADONPA_DONPA, 456, 280);
  Engine_ObjectEventWaitForMove(TOADONPA_DONPA);
  OBJECT_WALK(TOADONPA_DONPA, 440, 256);
  FieldCamera_MoveTo(0x1c80000, -1, 0xe00000, 1);
  MESSAGE_SET((donpaMessage + 1));
  MESSAGE_PUT(TOADONPA_DONPA, 0);
  Engine_ObjectEventWaitForMove(TOADONPA_DONPA);
  OBJECT_DIR(TOADONPA_DONPA, FACING_NORTH + FACING_STEP, 0);
  OBJECT_EMOTE(TOADONPA_DODONPA, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  OBJECT_ANIMATION(TOADONPA_DONPA, 4);
  WAIT(89);
  MESSAGE_SET((donpaMessage + 2));
  MESSAGE_PUT(TOADONPA_DONPA, 0);
  Engine_ObjectEventRunRepeatedMotion(TOADONPA_DODONPA, 2);
  Engine_ObjectEventSetAnimationAndWait(TOADONPA_DONPA, 3);
  MESSAGE_SET((donpaMessage + 3));
  MESSAGE_PUT(TOADONPA_DONPA, 0);
  OBJECT_EMOTE(TOADONPA_DODONPA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 80);
  OBJECT_ANIMATION(TOADONPA_DONPA, 4);
  WAIT(80);
  OBJECT_DIR(TOADONPA_DONPA, FACING_SOUTHEAST + FACING_STEP, 0);
  WAIT(20);
  MESSAGE_SET((donpaMessage + 4));
  MESSAGE_PUT(TOADONPA_DONPA, 0);
  Engine_ObjectEventStartRepeatedMotion(TOADONPA_DODONPA, 1);
  MESSAGE_SET((donpaMessage + 5));
  MESSAGE_PUT(TOADONPA_DODONPA, 0);
  WAIT(30);
  Engine_ObjectEventSetAnimationAndWait(TOADONPA_DONPA, 3);
  OBJECT_FACE(TOADONPA_DONPA, TOADONPA_DODONPA, 0);
  WAIT(20);
  MESSAGE_SET((donpaMessage + 6));
  MESSAGE_PUT(TOADONPA_DONPA, 0);
  Engine_ObjectEventStartRepeatedMotion(TOADONPA_DODONPA, 1);
  OBJECT_EMOTE(TOADONPA_DODONPA, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  MESSAGE_SET((donpaMessage + 7));
  MESSAGE_PUT(TOADONPA_DODONPA, 0);
  OBJECT_EMOTE(TOADONPA_DONPA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 60);
  MESSAGE_SET((donpaMessage + 8));
  MESSAGE_PUT(TOADONPA_DONPA, 0);
  Engine_ObjectEventSetAnimationAndWait(TOADONPA_DONPA, 3);
  MESSAGE_SET((donpaMessage + 9));
  MESSAGE_PUT(TOADONPA_DONPA, 0);
  WAIT(20);
  ObjectEvent_SetAttachedEffect(TOADONPA_DODONPA, ATTACHED_EFFECT_IN_FRONT |
                                                      ATTACHED_EFFECT_ATTACH);
  WAIT(80);
  ObjectEvent_SetAttachedEffect(TOADONPA_DODONPA, 0);
  OBJECT_WALK(TOADONPA_DONPA, 496, 240);
  Engine_ObjectEventWaitForMove(TOADONPA_DONPA);
  OBJECT_FACE(TOADONPA_DONPA, TOADONPA_HAMMET, 0);
  WAIT(20);
  OBJECT_FACE(TOADONPA_HAMMET, TOADONPA_DONPA, 0);
  MESSAGE_SET((donpaMessage + 10));
  MESSAGE_PUT(TOADONPA_DONPA, 0);
  Engine_ObjectEventStartRepeatedMotion(TOADONPA_HAMMET, 1);
  MESSAGE_SET((donpaMessage + 11));
  MESSAGE_PUT(TOADONPA_HAMMET, 0);
  WAIT(40);
  Engine_ObjectEventSetAnimationAndWait(TOADONPA_DONPA, 3);
  WAIT(20);
  MESSAGE_SET((donpaMessage + 12));
  MESSAGE_PUT(TOADONPA_DONPA, 0);
  OBJECT_ANIMATION(TOADONPA_DONPA, 3);
  OBJECT_EMOTE(TOADONPA_HAMMET, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  MESSAGE_SET((donpaMessage + 13));
  MESSAGE_PUT(TOADONPA_HAMMET, 0);
  OBJECT_EMOTE(TOADONPA_DONPA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 70);
  MESSAGE_SET((donpaMessage + 14));
  MESSAGE_PUT(TOADONPA_DONPA, 0);
  OBJECT_ANIMATION(TOADONPA_HAMMET, 3);
  WAIT(140);
  OBJECT_ANIMATION(TOADONPA_DONPA, 3);
  WAIT(120);
  Engine_ObjectEventFaceEachOther(LOCALID_PARTY_LEADER, TOADONPA_DONPA, 0);
  OBJECT_ANIMATION(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  OBJECT_ANIMATION(IVAN, ANIMATION_NOD);
  OBJECT_ANIMATION(GARET, ANIMATION_NOD);
  OBJECT_ANIMATION(MIA, ANIMATION_NOD);
  WAIT(120);
  OBJECT_EMOTE(TOADONPA_DONPA, EMOTE_IN_FRONT | EMOTE_SMILE, 180);
  MESSAGE_SET((donpaMessage + 15));
  MESSAGE_PUT(TOADONPA_DONPA, 0);
  OBJECT_EMOTE(TOADONPA_HAMMET, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  MESSAGE_SET((donpaMessage + 16));
  MESSAGE_PUT(TOADONPA_HAMMET, 0);
  Engine_ObjectEventFaceEachOther(LOCALID_PARTY_LEADER, GARET, 0);
  Engine_ObjectEventFaceEachOther(MIA, IVAN, 0);
  WAIT(80);
  OBJECT_FACE(TOADONPA_DONPA, LOCALID_PARTY_LEADER, 0);
  WAIT(60);
  OBJECT_FACE(LOCALID_PARTY_LEADER, TOADONPA_DONPA, 0);
  OBJECT_FACE(GARET, TOADONPA_DONPA, 0);
  OBJECT_FACE(IVAN, TOADONPA_DONPA, 0);
  OBJECT_FACE(MIA, TOADONPA_DONPA, 0);
  WAIT(60);
  MESSAGE_SET((donpaMessage + 17));
  MESSAGE_PUT(TOADONPA_DONPA, 0);
  OBJECT_EMOTE(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  OBJECT_EMOTE(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  OBJECT_EMOTE(IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  OBJECT_EMOTE(MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  WAIT(100);
  MESSAGE_SET((donpaMessage + 18));
  MESSAGE_PUT(GARET, 0);
  Engine_ObjectEventStartRepeatedMotion(IVAN, 1);
  WAIT(60);
  MESSAGE_SET((donpaMessage + 19));
  MESSAGE_PUT(IVAN, 0);
  OBJECT_FACE(TOADONPA_DONPA, IVAN, 0);
  WAIT(20);
  MESSAGE_SET((donpaMessage + 20));
  MESSAGE_PUT(TOADONPA_DONPA, 0);
  OBJECT_EMOTE(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  OBJECT_EMOTE(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  OBJECT_EMOTE(IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  OBJECT_EMOTE(MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  WAIT(120);
  OBJECT_ANIMATION(TOADONPA_DONPA, 4);
  WAIT(120);
  MESSAGE_SET((donpaMessage + 21));
  MESSAGE_PUT(TOADONPA_DONPA, 0);
  OBJECT_EMOTE(TOADONPA_DONPA, EMOTE_IN_FRONT | EMOTE_WINCE, 90);
  MESSAGE_SET((donpaMessage + 22));
  MESSAGE_PUT(TOADONPA_DONPA, 0);
  OBJECT_ANIMATION(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  OBJECT_ANIMATION(IVAN, ANIMATION_NOD);
  OBJECT_ANIMATION(MIA, ANIMATION_NOD);
  OBJECT_ANIMATION(GARET, ANIMATION_NOD);
  OBJECT_ANIMATION(TOADONPA_HAMMET, 3);
  WAIT(80);
  OBJECT_ANIMATION(TOADONPA_DONPA, 4);
  WAIT(120);
  MESSAGE_SET((donpaMessage + 23));
  MESSAGE_PUT(TOADONPA_DONPA, 0);
  OBJECT_EMOTE(TOADONPA_HAMMET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  MESSAGE_SET((donpaMessage + 24));
  MESSAGE_PUT(TOADONPA_HAMMET, 0);
  OBJECT_FACE(TOADONPA_DONPA, TOADONPA_HAMMET, 0);
  WAIT(20);
  Engine_ObjectEventStartRepeatedMotion(TOADONPA_DONPA, 1);
  WAIT(50);
  MESSAGE_SET((donpaMessage + 25));
  MESSAGE_PUT(TOADONPA_DONPA, 0);
  OBJECT_ANIMATION(GARET, ANIMATION_NOD);
  WAIT(70);
  MESSAGE_SET((donpaMessage + 26));
  MESSAGE_PUT(GARET, 0);
  OBJECT_ANIMATION(MIA, ANIMATION_NOD);
  MESSAGE_SET((donpaMessage + 27));
  MESSAGE_PUT(MIA, 0);
  OBJECT_FACE(TOADONPA_DONPA, LOCALID_PARTY_LEADER, 0);
  Engine_ObjectEventSetAnimationAndWait(TOADONPA_DONPA, 3);
  WAIT(20);
  MESSAGE_SET((donpaMessage + 28));
  MESSAGE_PUT(TOADONPA_DONPA, 0);
  OBJECT_EMOTE(IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 90);
  MESSAGE_SET((donpaMessage + 29));
  MESSAGE_PUT(IVAN, 0);
  OBJECT_DIR(TOADONPA_DONPA, FACING_SOUTH + FACING_STEP, 0);
  WAIT(20);
  MESSAGE_SET((donpaMessage + 30));
  MESSAGE_PUT(TOADONPA_DONPA, 0);
  Engine_ObjectEventStartRepeatedMotion(TOADONPA_HAMMET, 1);
  WAIT(60);
  MESSAGE_SET((donpaMessage + 31));
  MESSAGE_PUT(TOADONPA_HAMMET, 0);
  OBJECT_FACE(TOADONPA_DONPA, TOADONPA_HAMMET, 0);
  WAIT(20);
  Engine_ObjectEventSetAnimationAndWait(TOADONPA_DONPA, 3);
  WAIT(20);
  MESSAGE_SET((donpaMessage + 32));
  MESSAGE_PUT(TOADONPA_DONPA, 0);
  OBJECT_EMOTE(GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 70);
  MESSAGE_SET((donpaMessage + 33));
  MESSAGE_PUT(GARET, 0);
  OBJECT_FACE(TOADONPA_DONPA, LOCALID_PARTY_LEADER, 0);
  WAIT(30);
  MESSAGE_SET((donpaMessage + 34));
  MESSAGE_PUT(TOADONPA_DONPA, 0);
  OBJECT_EMOTE(MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 80);
  MESSAGE_SET((donpaMessage + 35));
  MESSAGE_PUT(MIA, 0);
  Engine_ObjectEventSetAnimationAndWait(TOADONPA_DONPA, 3);
  MESSAGE_SET((donpaMessage + 36));
  MESSAGE_PUT(TOADONPA_DONPA, 0);
  OBJECT_FACE(IVAN, LOCALID_PARTY_LEADER, 0);
  WAIT(30);
  OBJECT_ANIMATION(IVAN, ANIMATION_NOD);
  WAIT(100);
  MESSAGE_SET((donpaMessage + 37));
  MESSAGE_OPEN(IVAN, 0);
  if (Engine_ScriptChooseYesNo(0, 0) != 0) {
    MESSAGE_SET((donpaMessage + 38));
    MESSAGE_PUT(TOADONPA_DONPA, 0);
  }
  WAIT(30);
  OBJECT_ANIMATION(GARET, ANIMATION_NOD);
  OBJECT_ANIMATION(IVAN, ANIMATION_NOD);
  OBJECT_ANIMATION(MIA, ANIMATION_NOD);
  WAIT(100);
  Engine_ObjectEventFaceEachOther(LOCALID_PARTY_LEADER, TOADONPA_HAMMET, 0);
  OBJECT_ANIMATION(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  OBJECT_ANIMATION(TOADONPA_HAMMET, 3);
  WAIT(100);
  // Rejoin the companions, then escort Hammet out of the fortress.
  OBJECT_ANIMATION(GARET, ANIMATION_WALK);
  leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  if (leader != 0) {
    ObjectEvent_SetDestination(GARET, leader->x.part.pixel,
                               leader->z.part.pixel);
  }
  Engine_ObjectEventWaitForMove(GARET);
  OBJECT_POSITION_SET(GARET, 0, 0);
  OBJECT_ANIMATION(IVAN, ANIMATION_WALK);
  leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  if (leader != 0) {
    ObjectEvent_SetDestination(IVAN, leader->x.part.pixel,
                               leader->z.part.pixel);
  }
  Engine_ObjectEventWaitForMove(IVAN);
  OBJECT_POSITION_SET(IVAN, 0, 0);
  OBJECT_ANIMATION(MIA, ANIMATION_WALK);
  leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  if (leader != 0) {
    ObjectEvent_SetDestination(MIA, leader->x.part.pixel, leader->z.part.pixel);
  }
  Engine_ObjectEventWaitForMove(MIA);
  OBJECT_POSITION_SET(MIA, 0, 0);
  WAIT(30);
  OBJECT_SPEED(TOADONPA_HAMMET, PIXELS(2) / 5, PIXELS(1) / 5);
  OBJECT_WALK(TOADONPA_HAMMET, 472, 184);
  Engine_ObjectEventWaitForMove(TOADONPA_HAMMET);
  OBJECT_ANIMATION(TOADONPA_HAMMET, 1);
  OBJECT_DIR(TOADONPA_HAMMET, FACING_SOUTHEAST + FACING_STEP, 0);
  OBJECT_FACE(TOADONPA_DONPA, LOCALID_PARTY_LEADER, 0);
  OBJECT_ANIMATION(TOADONPA_DONPA, 3);
  WAIT(20);
  OBJECT_FACE(TOADONPA_DONPA, TOADONPA_DODONPA, 0);
  WAIT(20);
  MESSAGE_SET((donpaMessage + 39));
  MESSAGE_PUT(TOADONPA_DONPA, 0);
  Engine_ObjectEventRunRepeatedMotion(TOADONPA_DODONPA, 2);
  MESSAGE_SET((donpaMessage + 40));
  MESSAGE_PUT(TOADONPA_DODONPA, 0);
  OBJECT_ANIMATION(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  WAIT(30);
  OBJECT_SPEED(LOCALID_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  OBJECT_SPEED(TOADONPA_HAMMET, PIXELS(1), PIXELS(1) / 2);
  Engine_CameraFollowObjectEvent(LOCALID_PARTY_LEADER, 1);
  OBJECT_WALK(LOCALID_PARTY_LEADER, 480, 248);
  WAIT(40);
  Engine_ObjectEventSetSpritePriority(TOADONPA_HAMMET, 0);
  Engine_ObjectEventSetSpritePriority(LOCALID_PARTY_LEADER, 0);
  OBJECT_WALK(TOADONPA_HAMMET, 480, 216);
  Engine_ObjectEventWaitForMove(LOCALID_PARTY_LEADER);
  OBJECT_FACE(LOCALID_PARTY_LEADER, TOADONPA_DONPA, 0);
  Engine_ObjectEventWaitForMove(TOADONPA_HAMMET);
  OBJECT_FACE(TOADONPA_HAMMET, TOADONPA_DONPA, 0);
  OBJECT_DIR(TOADONPA_HAMMET, FACING_SOUTHEAST + FACING_STEP, 0);
  OBJECT_ANIMATION(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  WAIT(5);
  OBJECT_ANIMATION(TOADONPA_HAMMET, 3);
  WAIT(100);
  OBJECT_ANIMATION(TOADONPA_DONPA, 3);
  WAIT(100);
  OBJECT_WALK(TOADONPA_HAMMET, 480, 248);
  OBJECT_WALK(LOCALID_PARTY_LEADER, 456, 248);
  Engine_ObjectEventWaitForMove(TOADONPA_HAMMET);
  OBJECT_WALK(TOADONPA_HAMMET, 456, 248);
  Engine_ObjectEventWaitForMove(LOCALID_PARTY_LEADER);
  OBJECT_WALK(LOCALID_PARTY_LEADER, 456, 360);
  Engine_ObjectEventWaitForMove(TOADONPA_HAMMET);
  OBJECT_WALK(TOADONPA_HAMMET, 456, 360);
  Engine_ObjectEventWaitForMove(LOCALID_PARTY_LEADER);
  OBJECT_WALK(LOCALID_PARTY_LEADER, 352, 360);
  Engine_ObjectEventWaitForMove(TOADONPA_HAMMET);
  OBJECT_WALK(TOADONPA_HAMMET, 352, 360);
  WAIT(20);
  SE(AUDIO_CUE_FADE_BGM);
  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_CIRCLE);
  Engine_ScriptCloseScreen();
  WAIT(1);
  WAIT(210);
  Engine_ScriptRequestWarp(LUNPA_FORTRESS_TOADONPA_TO_LUNPA_FORTRESS_GATES);
  SCRIPT_END();
}

/// LunpaFortressGroundFloor: Mind Read on the second sentry reads his thoughts.
/// He has one of eight thoughts, chosen at random when the map was entered;
/// in one he looks around first.
static void EventScript_ReadSentrysThoughts(void) {

  switch (gLunpaFortressRandomPick) {
  case 0:
    MESSAGE_SET(MsgLunpaHammetGreatMerchant);
    MESSAGE_PUT(GROUND_FLOOR_SECOND_SENTRY, 0);
    break;
  case 1:
    MESSAGE_SET(MsgLunpaZZZ);
    MESSAGE_PUT(GROUND_FLOOR_SECOND_SENTRY, 0);
    break;
  case 2:
    MESSAGE_SET(MsgLunpaDodonpasOrdersAbsolute);
    MESSAGE_PUT(GROUND_FLOOR_SECOND_SENTRY, 0);
    break;
  case 3:
    MESSAGE_SET(MsgLunpaSighBadCouldnt);
    MESSAGE_PUT(GROUND_FLOOR_SECOND_SENTRY, 0);
    break;
  case 4:
    MESSAGE_SET(MsgLunpaStrangeSwearSomeone);
    MESSAGE_PUT(GROUND_FLOOR_SECOND_SENTRY, 0);
    break;
  case 6:
    MESSAGE_SET(MsgLunpaToldStandGuard);
    MESSAGE_PUT(GROUND_FLOOR_SECOND_SENTRY, 0);
    break;
  case 7:
    MESSAGE_SET(MsgLunpaWhoDisruptingSleep);
    MESSAGE_PUT(GROUND_FLOOR_SECOND_SENTRY, 0);
    break;
  case 5:
    Engine_ObjectEventFaceDirection(GROUND_FLOOR_SECOND_SENTRY,
                                    FACING_NORTH + FACING_STEP, 0);
    WAIT(50);
    Engine_ObjectEventFaceDirection(GROUND_FLOOR_SECOND_SENTRY,
                                    FACING_NORTHWEST + FACING_STEP, 0);
    WAIT(50);
    Engine_ObjectEventFaceDirection(GROUND_FLOOR_SECOND_SENTRY,
                                    FACING_SOUTH + FACING_STEP, 0);
    WAIT(50);
    MESSAGE_SET(MsgLunpaTakeCareAnybody);
    MESSAGE_PUT(GROUND_FLOOR_SECOND_SENTRY, 0);
    break;
  }
}

/// LunpaFortressGroundFloor: the second sentry looks around, hearing someone he
/// cannot see.
static void EventScript_TalkToSentry(void) {

  s32 messageBase;

  OBJECT_EMOTE(GROUND_FLOOR_SECOND_SENTRY, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  OBJECT_DIR(GROUND_FLOOR_SECOND_SENTRY, FACING_NORTH + FACING_STEP, 0);
  WAIT(50);
  OBJECT_DIR(GROUND_FLOOR_SECOND_SENTRY, FACING_NORTHWEST + FACING_STEP, 0);
  WAIT(50);
  OBJECT_DIR(GROUND_FLOOR_SECOND_SENTRY, FACING_SOUTH + FACING_STEP, 0);
  WAIT(50);
  messageBase = MsgLunpaLeftGuardHearsSomeone;
  MESSAGE_SET(messageBase);
  MESSAGE_PUT(GROUND_FLOOR_SECOND_SENTRY, 0);
  OBJECT_ANIMATION(GROUND_FLOOR_SECOND_SENTRY, 4);
  WAIT(60);
  OBJECT_DIR(GROUND_FLOOR_SECOND_SENTRY, FACING_NORTHWEST + FACING_STEP, 0);
  WAIT(40);
  MESSAGE_SET(messageBase + 1);
  MESSAGE_PUT(GROUND_FLOOR_SECOND_SENTRY, 0);
}

/// LunpaFortressUpperFloor: Donpa's attendant. Before the rescue the attendant
/// hushes the party, glances at the sleeping Donpa, asks a question and tells
/// of Donpa and his spoiled son; once told, only a line. After the rescue,
/// Donpa is grateful.
static void EventScript_TalkToAttendant(void) {
  s32 whisperMessage;
  s32 familyMessage;

  SCRIPT_START();
  if (FLAG_CHECK(FLAG_DODONPA_LOCKED_UP) != 0) {
    MESSAGE_SET(MsgLunpaDonpaGrateful);
    // XXX: likely a slip for UPPER_FLOOR_ATTENDANT, because the line is the
    // attendant's and Dodonpa's wife stands at the far side of the fortress.
    MESSAGE_PUT(UPPER_FLOOR_DODONPAS_WIFE, 0);
    SCRIPT_END();
  } else {
    if (FLAG_CHECK(FLAG_MAPSEC_TEMP_13) != 0) {
      MESSAGE_SET(MsgLunpaMaybeDodonpasEyes);
      MESSAGE_OPEN(UPPER_FLOOR_ATTENDANT, 0);
      SCRIPT_END();
    } else {
      OBJECT_EMOTE(UPPER_FLOOR_ATTENDANT, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
      OBJECT_FACE(UPPER_FLOOR_ATTENDANT, LOCALID_PARTY_LEADER, 0);
      whisperMessage = MsgLunpaShhhPleaseDont;
      MESSAGE_SET(whisperMessage);
      MESSAGE_PUT(UPPER_FLOOR_ATTENDANT, 0);
      OBJECT_FACE(UPPER_FLOOR_ATTENDANT, UPPER_FLOOR_DONPA, 0);
      Engine_CameraMoveToObjectEvent(UPPER_FLOOR_DONPA, 1);
      Engine_CameraWaitForMove();
      WAIT(60);
      Engine_CameraMoveToObjectEvent(LOCALID_PARTY_LEADER, 1);
      WAIT(20);
      OBJECT_EMOTE(UPPER_FLOOR_ATTENDANT, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
      MESSAGE_SET(whisperMessage + 1);
      MESSAGE_PUT(UPPER_FLOOR_ATTENDANT, 0);
      OBJECT_EMOTE(UPPER_FLOOR_ATTENDANT, EMOTE_IN_FRONT | EMOTE_SCOWL, 60);
      MESSAGE_SET(whisperMessage + 2);
      MESSAGE_PUT(UPPER_FLOOR_ATTENDANT, 0);
      WAIT(70);
      OBJECT_EMOTE(UPPER_FLOOR_ATTENDANT, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                   60);
      OBJECT_FACE(UPPER_FLOOR_ATTENDANT, LOCALID_PARTY_LEADER, 0);
      MESSAGE_SET(whisperMessage + 3);
      MESSAGE_OPEN(UPPER_FLOOR_ATTENDANT, 0);
      if (Engine_ScriptChooseYesNo(0, 0) == 0) {
        MESSAGE_SET(whisperMessage + 4);
        MESSAGE_OPEN(UPPER_FLOOR_ATTENDANT, 0);
      } else {
        MESSAGE_SET(whisperMessage + 5);
        MESSAGE_OPEN(UPPER_FLOOR_ATTENDANT, 0);
      }
      WAIT(60);
      OBJECT_EMOTE(UPPER_FLOOR_ATTENDANT, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
      familyMessage = MsgLunpaDonpaKnowsCoddled;
      MESSAGE_SET(familyMessage);
      MESSAGE_OPEN(UPPER_FLOOR_ATTENDANT, 0);
      Engine_ObjectEventRunRepeatedMotion(UPPER_FLOOR_ATTENDANT, 1);
      MESSAGE_SET(familyMessage + 1);
      MESSAGE_OPEN(UPPER_FLOOR_ATTENDANT, 0);
      Engine_ObjectEventSetAnimationAndWait(UPPER_FLOOR_ATTENDANT, 3);
      MESSAGE_SET(familyMessage + 2);
      MESSAGE_OPEN(UPPER_FLOOR_ATTENDANT, 0);
      FLAG_SET(FLAG_MAPSEC_TEMP_13);
      SCRIPT_END();
    }
  }
}

/// LunpaFortressUpperFloor: Mind Read on Donpa's attendant.
static void EventScript_ReadAttendantsThoughts(void) {
  if (Engine_GameFlagIsSet(FLAG_DODONPA_LOCKED_UP)) {
    MESSAGE_SET(MsgLunpaDifficultDonpaRight);
    MESSAGE_PUT(UPPER_FLOOR_ATTENDANT, 0);
  } else {
    MESSAGE_SET(MsgLunpaSomeonePunishDodonpa);
    MESSAGE_PUT(UPPER_FLOOR_ATTENDANT, 0);
  }
}

/// LunpaFortressUpperFloor: Mind Read on Donpa.
static void EventScript_ReadDonpasThoughts(void) {
  if (Engine_GameFlagIsSet(FLAG_DODONPA_LOCKED_UP)) {
    MESSAGE_SET(MsgLunpaFatherStayAngry);
    MESSAGE_PUT(UPPER_FLOOR_DONPA, 0);
  } else {
    MESSAGE_SET(MsgLunpaFatherSorryDodonpa);
    MESSAGE_PUT(UPPER_FLOOR_DONPA, 0);
  }
}

/// LunpaFortressUpperFloor: Donpa, asleep before the rescue. "ZZZZ..." After
/// it, the first time, he thanks the party and has his attendant show them his
/// reward, Tonic.
static void EventScript_TalkToDonpa(void) {
  s32 rewardMessage;

  if (FLAG_CHECK(FLAG_DODONPA_LOCKED_UP) != 0) {
    if (FLAG_CHECK(FLAG_LUNPA_FORTRESS_REWARD_TAKEN) == 0 &&
        FLAG_CHECK(FLAG_MAPSEC_TEMP_14) == 0) {
      rewardMessage = MsgLunpaThankHelpDodonpa;
      MESSAGE_SET(rewardMessage);
      MESSAGE_PUT(UPPER_FLOOR_DONPA, 0);
      Engine_ObjectEventRunRepeatedMotion(UPPER_FLOOR_DONPA, 1);
      WAIT(30);
      OBJECT_SPEED(UPPER_FLOOR_DONPA, PIXELS(1) / 10, PIXELS(1) / 20);
      ObjectEvent_SetDestinationOffset(UPPER_FLOOR_DONPA, -4, 0);
      Engine_ObjectEventWaitForMove(UPPER_FLOOR_DONPA);
      OBJECT_ANIMATION(UPPER_FLOOR_DONPA, 3);
      WAIT(60);
      OBJECT_SPEED(UPPER_FLOOR_DONPA, PIXELS(1) / 5, PIXELS(1) / 10);
      ObjectEvent_SetDestinationOffset(UPPER_FLOOR_DONPA, -6, 0);
      OBJECT_FACE(UPPER_FLOOR_DONPA, LOCALID_PARTY_LEADER, 0);
      Engine_ObjectEventWaitForMove(UPPER_FLOOR_DONPA);
      MESSAGE_SET(rewardMessage + 1);
      MESSAGE_PUT(UPPER_FLOOR_DONPA, 0);
      Engine_ObjectEventRunRepeatedMotion(UPPER_FLOOR_DONPA, 1);
      OBJECT_FACE(UPPER_FLOOR_ATTENDANT, UPPER_FLOOR_DONPA, 0);
      MESSAGE_SET(rewardMessage + 2);
      MESSAGE_PUT(UPPER_FLOOR_DONPA, 0);
      WAIT(70);
      OBJECT_ANIMATION(UPPER_FLOOR_ATTENDANT, 3);
      WAIT(60);
      OBJECT_SPEED(UPPER_FLOOR_ATTENDANT, PIXELS(2) / 5, PIXELS(1) / 5);
      OBJECT_WALK(UPPER_FLOOR_ATTENDANT, 880, 112);
      Engine_ObjectEventWaitForMove(UPPER_FLOOR_ATTENDANT);
      OBJECT_DIR(UPPER_FLOOR_ATTENDANT, FACING_NORTH + FACING_STEP, 0);
      MESSAGE_SET(rewardMessage + 3);
      MESSAGE_PUT(UPPER_FLOOR_DONPA, 0);
      OBJECT_ANIMATION(UPPER_FLOOR_DONPA, 3);
      WAIT(70);
      ObjectEvent_SetDestinationOffset(UPPER_FLOOR_DONPA, 8, 0);
      Engine_ObjectEventWaitForMove(UPPER_FLOOR_DONPA);
      OBJECT_ANIMATION(UPPER_FLOOR_DONPA, 5);
      MESSAGE_SET(rewardMessage + 4);
      MESSAGE_PUT(UPPER_FLOOR_DONPA, 0);
      OBJECT_WALK(LOCALID_PARTY_LEADER, 896, 120);
      Engine_ObjectEventWaitForMove(LOCALID_PARTY_LEADER);
      Engine_ObjectEventFaceEachOther(LOCALID_PARTY_LEADER,
                                      UPPER_FLOOR_ATTENDANT, 0);
      WAIT(60);
      OBJECT_ANIMATION(UPPER_FLOOR_ATTENDANT, 3);
      WAIT(30);
      FLAG_SET(FLAG_MAPSEC_TEMP_14);
    } else {
      MESSAGE_SET(MsgLunpaWellWorriedDodonpa);
      MESSAGE_PUT(UPPER_FLOOR_DONPA, 0);
    }
  } else {
    MESSAGE_SET(MsgLunpaZZZZ);
    MESSAGE_PUT(UPPER_FLOOR_DONPA, 0);
  }
}

/// LunpaFortressUpperFloor: talking to Tonic, a Mercury Djinni and Donpa's
/// reward, it joins the party.
static void EventScript_DjinniJoins(void) {
  Engine_DjinnJoinParty(UPPER_FLOOR_TONIC, 1, 5);
  Engine_GameFlagSet(FLAG_LUNPA_FORTRESS_REWARD_TAKEN);
}

/// LunpaFortressUpperFloor: the third of the beaten guards begs. "Owww! Hey!
/// Don't hurt me again..."
static void EventScript_TalkToBeatenGuardC(void) {
  Engine_ObjectEventRunRepeatedMotion(UPPER_FLOOR_GUARD_C, 2);
  MESSAGE_SET(MsgLunpaOwwwDontHurt);
  MESSAGE_PUT(UPPER_FLOOR_GUARD_C, 0);
}

/// LunpaFortressUpperFloor: the second of the beaten guards gives up, and
/// Hammet is in the cave. "We give up!"
static void EventScript_TalkToBeatenGuardB(void) {
  Engine_ObjectEventRunRepeatedMotion(UPPER_FLOOR_GUARD_B, 2);
  MESSAGE_SET(MsgLunpaRightRightGive);
  MESSAGE_PUT(UPPER_FLOOR_GUARD_B, 0);
}

/// LunpaFortressUpperFloor: the first of the beaten guards marvels at the
/// party. "Who'd have thought they'd be so tough?"
static void EventScript_TalkToBeatenGuardA(void) {
  Engine_ObjectEventRunRepeatedMotion(UPPER_FLOOR_GUARD_A, 2);
  MESSAGE_SET(MsgLunpaGuysTougherThought);
  MESSAGE_PUT(UPPER_FLOOR_GUARD_A, 0);
}

/// LunpaFortressUpperFloor: one of Dodonpa's family asks where he is. "Do you
/// know where Dodonpa is?"
static void EventScript_AskWhereDodonpaIs(void) {
  MESSAGE_SET(MsgLunpaKnowWhereDodonpa);
  Engine_ScriptAskYesNo(UPPER_FLOOR_DODONPAS_WIFE, 0);
}

/// LunpaFortressGroundFloor: a bent old man asks whether the party sneaked in.
/// He's startled the first time, and wants to know whether it came in search of
/// something; on yes, he promises to keep quiet. "Wh-Wh-Who goes there!"
static void EventScript_TalkToStartledOldMan(void) {
  s32 frightMessage;
  s32 purposeMessage;

  if (FLAG_CHECK(FLAG_MAP_TEMP_26) != 0) {
    MESSAGE_SET(MsgLunpaWontTellAnyone);
    MESSAGE_PUT(GROUND_FLOOR_STARTLED_OLD_MAN, 0);
  } else {
    SCRIPT_START();
    OBJECT_FACE(GROUND_FLOOR_STARTLED_OLD_MAN, LOCALID_PARTY_LEADER, 0);
    if (FLAG_CHECK(FLAG_MAP_TEMP_27) == 0) {
      Engine_ObjectEventJump(GROUND_FLOOR_STARTLED_OLD_MAN, 4, 0);
      Engine_ObjectEventStop(GROUND_FLOOR_STARTLED_OLD_MAN);
      ObjectEvent_RefreshSelectorById(GROUND_FLOOR_STARTLED_OLD_MAN);
      WAIT(20);
      frightMessage = MsgLunpaWhWhWho;
      MESSAGE_SET(frightMessage);
      MESSAGE_PUT(GROUND_FLOOR_STARTLED_OLD_MAN, 0);
      OBJECT_EMOTE(GROUND_FLOOR_STARTLED_OLD_MAN, EMOTE_IN_FRONT | EMOTE_WINCE,
                   30);
      MESSAGE_SET(frightMessage + 1);
      MESSAGE_PUT(GROUND_FLOOR_STARTLED_OLD_MAN, 0);
      WAIT(30);
      OBJECT_ANIMATION(GROUND_FLOOR_STARTLED_OLD_MAN, 4);
      WAIT(30);
    }
    purposeMessage = MsgLunpaDontLookNearly;
    MESSAGE_SET(purposeMessage);
    MESSAGE_PUT(GROUND_FLOOR_STARTLED_OLD_MAN, 0);
    OBJECT_EMOTE(GROUND_FLOOR_STARTLED_OLD_MAN, EMOTE_IN_FRONT | EMOTE_QUESTION,
                 40);
    MESSAGE_SET(purposeMessage + 1);
    MESSAGE_OPEN(GROUND_FLOOR_STARTLED_OLD_MAN, 0);
    if (Engine_ScriptChooseYesNo(0, 0) == 0) {
      MESSAGE_SET(purposeMessage + 2);
      MESSAGE_OPEN(GROUND_FLOOR_STARTLED_OLD_MAN, 0);
      FLAG_SET(FLAG_MAP_TEMP_26);
    } else {
      MESSAGE_SET(purposeMessage + 3);
      MESSAGE_OPEN(GROUND_FLOOR_STARTLED_OLD_MAN, 0);
    }
    FLAG_SET(FLAG_MAP_TEMP_27);
    SCRIPT_END();
  }
}

/// LunpaFortressGroundFloor: Mind Read on the startled resident. Once they have
/// promised, a thought about Hammet; before, Mind Read stops and they talk
/// instead.
static void EventScript_ReadStartledResidentsThoughts(void) {

  if (Engine_GameFlagIsSet(FLAG_MAP_TEMP_26)) {
    MESSAGE_SET(MsgLunpaMaybeMerchantReason);
    MESSAGE_PUT(GROUND_FLOOR_STARTLED_OLD_MAN, 0);
  } else {
    s16 *psynergyRequest = &gFieldWork->psynergy;

    *psynergyRequest = 0;
    Engine_PsynergyCancel();
    EventScript_TalkToStartledOldMan();
  }
}

static void UNUSED LunpaFortress_DoNothingF(void) {}

/// LunpaFortressToadonpa, after Donpa's judgment: Dodonpa behind bars begs his
/// father's forgiveness; with the Cell Key, a further line shows.
static void EventScript_TalkToJailedDodonpa(void) {
  s32 pleaMessage = MsgLunpaWrongTurnOver;

  MESSAGE_SET(pleaMessage);
  MESSAGE_PUT(SPEAKER_WINDOW_BELOW | TOADONPA_DODONPA, 0);
  if (Engine_PartyInventoryFindOwner(ITEM_CELL_KEY) != -1) {
    Engine_MessageShowCentered(pleaMessage + 2, 1);
  }
}

/// LunpaFortressToadonpa: Mind Read on the jailed Dodonpa.
static void EventScript_ReadJailedDodonpasThoughts(void) {
  MESSAGE_SET(MsgLunpaCantBelieveWhen);
  MESSAGE_PUT(TOADONPA_DODONPA, 0);
}

/// Sets each map up: a random pick for the guards' lines, the doors already
/// opened, the watching guards and, in LunpaFortressToadonpa, the room after
/// the battle with Toadonpa or Dodonpa jailed.
s32 Map_Initialize(void) {
  struct ObjectEvent *mapObjectEvent;

  gLunpaFortressRandomPick = (u32)Engine_RandomNext() * 7 >> 16;
  if (gGameState.mapId == (s32)&MapId_LunpaFortressGroundFloor) {
    Engine_MapGridSetShownLayers(LUNPA_FORTRESS_PASSAGE_LAYERS);
    LunpaFortress_InitializeGroundFloor();
  }
  if (gGameState.mapId == (s32)&MapId_LunpaFortressUpperFloor)
    LunpaFortress_InitializeUpperFloor();
  if (gGameState.mapId == (s32)&MapId_LunpaFortressCave)
    LunpaFortress_InitializeCave();
  if (gGameState.mapId == (s32)&MapId_LunpaFortressToadonpa) {
    gFieldWork->transition =
        MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
    Engine_ObjectEventSetSpriteFlags(ObjectEvent_Get(TOADONPA_HAMMET), 0);
    OBJECT_DIR(TOADONPA_HAMMET, FACING_EAST, 0);
    OBJECT_ANIMATION(TOADONPA_HAMMET, 0);
    LunpaFortress_RestoreToadonpaDoors();
    mapObjectEvent = ObjectEvent_Get(TOADONPA_WEST_JAR_BLOCKER);
    if (mapObjectEvent != 0)
      Engine_ObjectEventSetSpriteFlags(mapObjectEvent, 0);
    mapObjectEvent->priorityFlags = OBJ_EVENT_PRIORITY_UNDERFOOT;
    mapObjectEvent = ObjectEvent_Get(TOADONPA_CHEST_BLOCKER);
    if (mapObjectEvent != 0)
      Engine_ObjectEventSetSpriteFlags(mapObjectEvent, 0);
    mapObjectEvent->priorityFlags = OBJ_EVENT_PRIORITY_UNDERFOOT;
    mapObjectEvent = ObjectEvent_Get(TOADONPA_EAST_JAR_BLOCKER);
    if (mapObjectEvent != 0)
      Engine_ObjectEventSetSpriteFlags(mapObjectEvent, 0);
    mapObjectEvent->priorityFlags = OBJ_EVENT_PRIORITY_UNDERFOOT;
    Engine_MapGridSetShownLayers(LUNPA_FORTRESS_PASSAGE_LAYERS);
    if (gGameState.warpDest == LUNPA_FORTRESS_TOADONPA_AFTER_BATTLE) {
      Engine_MapGridSetShownLayers(LUNPA_FORTRESS_ROOM_LAYERS);
      EventScript_DonpaJudgesDodonpa();
    }
    if (gGameState.warpDest ==
        LUNPA_FORTRESS_TOADONPA_FROM_LUNPA_FORTRESS_CAVE_NORTHWEST) {
      Engine_MapGridSetShownLayers(LUNPA_FORTRESS_ROOM_LAYERS);
      if (FLAG_CHECK(FLAG_DODONPA_LOCKED_UP) != 0) {
        OBJECT_POSITION_SET(TOADONPA_HAMMET, 0, 0);
        OBJECT_POSITION_SET(TOADONPA_CELL_GUARD, 432, 344);
        OBJECT_DIR(TOADONPA_CELL_GUARD, FACING_SOUTH + FACING_STEP, 0);
        OBJECT_POSITION_SET(TOADONPA_DODONPA, 456, 288);
        OBJECT_DIR(TOADONPA_DODONPA, FACING_SOUTH + FACING_STEP, 0);
        OBJECT_POSITION_SET(TOADONPA_DODONPA_STAND_IN, 456, 320);
        Engine_ObjectEventSetSpriteFlags(
            ObjectEvent_Get(TOADONPA_DODONPA_STAND_IN), 0);
      }
    }
    mapObjectEvent = ObjectEvent_Get(TOADONPA_BOSS_PART);
    if (mapObjectEvent != 0)
      Engine_ObjectEventSetSpriteFlags(mapObjectEvent, 0);
    mapObjectEvent->priorityFlags = OBJ_EVENT_PRIORITY_UNDERFOOT;
    mapObjectEvent->scaleX = 4 * Q16_ONE / 5;
  }
  return 0;
}

/// Sets LunpaFortressGroundFloor up by warp destination: the guards that watch
/// each part, and the cell doors already opened.
static void LunpaFortress_InitializeGroundFloor(void) {
  LunpaFortress_RestoreGroundFloorDoors();
  switch (gGameState.warpDest) {
  case LUNPA_FORTRESS_GROUND_FLOOR_ENTRANCE_FROM_MAIN_PASSAGE:
  case LUNPA_FORTRESS_GROUND_FLOOR_MAIN_PASSAGE_FROM_ENTRANCE:
  case LUNPA_FORTRESS_GROUND_FLOOR_MAIN_PASSAGE_FROM_NORTH_PASSAGE:
  case LUNPA_FORTRESS_GROUND_FLOOR_MAIN_PASSAGE_FROM_FEAST_ROOM:
  case LUNPA_FORTRESS_GROUND_FLOOR_MAIN_PASSAGE_FROM_NORTHEAST_PASSAGE:
  case LUNPA_FORTRESS_GROUND_FLOOR_MAIN_PASSAGE_FROM_EAST_PASSAGE:
    gFieldWork->transition =
        MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
    Task_AddCallback(LunpaFortress_CheckEntranceGuard, TASK_PRIORITY_MAP);
    Task_AddCallback(LunpaFortress_CheckFirstEntranceSentry, TASK_PRIORITY_MAP);
    Task_AddCallback(LunpaFortress_CheckSecondEntranceSentry,
                     TASK_PRIORITY_MAP);
    Engine_MapGridSetShownLayers(LUNPA_FORTRESS_PASSAGE_LAYERS);
    break;
  case LUNPA_FORTRESS_GROUND_FLOOR_FEAST_ROOM_FROM_MAIN_PASSAGE:
  case LUNPA_FORTRESS_GROUND_FLOOR_BARREL_ROOM_FROM_NORTHEAST_PASSAGE:
    gFieldWork->transition =
        MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
    Engine_MapGridSetShownLayers(LUNPA_FORTRESS_ROOM_LAYERS);
    break;
  case LUNPA_FORTRESS_GROUND_FLOOR_EAST_PASSAGE_FROM_MAIN_PASSAGE:
  case LUNPA_FORTRESS_GROUND_FLOOR_EAST_PASSAGE_FROM_NORTHEAST_PASSAGE:
  case LUNPA_FORTRESS_GROUND_FLOOR_EAST_PASSAGE_FROM_TREASURE_ROOM:
    gFieldWork->transition =
        MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
    Task_AddCallback(LunpaFortress_UpdateGroundFloorPatrolDetection,
                     TASK_PRIORITY_MAP);
    Task_AddCallback(LunpaFortress_CheckGroundFloorDoorApproach,
                     TASK_PRIORITY_MAP);
    Engine_TaskWait(1);
    Engine_MapGridRedraw();
    Engine_TaskWait(1);
    MapGrid_CopyCells(101, 9, 10, 8, 110, 9);
    Engine_MapGridSetShownLayers(LUNPA_FORTRESS_PASSAGE_LAYERS);
    break;
  case LUNPA_FORTRESS_GROUND_FLOOR_NORTHEAST_PASSAGE_FROM_MAIN_PASSAGE:
  case LUNPA_FORTRESS_GROUND_FLOOR_NORTHEAST_PASSAGE_FROM_EAST_PASSAGE:
  case LUNPA_FORTRESS_GROUND_FLOOR_NORTHEAST_PASSAGE_FROM_BARREL_ROOM:
    gFieldWork->transition =
        MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
    Task_AddCallback(LunpaFortress_CheckUpperHallGuard, TASK_PRIORITY_MAP);
    break;
  default:
    gFieldWork->transition =
        MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
    Engine_MapGridSetShownLayers(LUNPA_FORTRESS_PASSAGE_LAYERS);
    break;
  }
  Engine_ObjectEventSetChildValue(GROUND_FLOOR_PATROL_GUARD, 1);
  Engine_ObjectEventSetChildValue(GROUND_FLOOR_UPPER_HALL_GUARD, 1);
  Engine_ObjectEventSetChildValue(GROUND_FLOOR_SECOND_SENTRY, 1);
  Engine_ObjectEventSetChildValue(GROUND_FLOOR_ENTRANCE_GUARD, 1);
  Engine_ObjectEventSetChildValue(GROUND_FLOOR_FIRST_SENTRY, 1);
  Engine_TaskWait(1);
}

/// Sets LunpaFortressUpperFloor up: beaten guards gone or lying where they
/// fell, the Cell Key gone once taken, the patrols by warp destination and
/// Donpa's attendant by his reward's place.
static void LunpaFortress_InitializeUpperFloor(void) {
  struct ObjectEvent *keyObjectEvent;

  LunpaFortress_RestoreUpperFloorDoors();
  Engine_ObjectEventSetChildValue(UPPER_FLOOR_FIRST_PATROL, 1);
  Engine_ObjectEventSetChildValue(UPPER_FLOOR_SECOND_PATROL, 1);
  Engine_ObjectEventSetChildValue(UPPER_FLOOR_HALL_PATROL, 1);
  if (FLAG_CHECK(FLAG_LUNPA_FORTRESS_WEST_GUARD_BEATEN)) {
    OBJECT_POSITION_SET(UPPER_FLOOR_WEST_GUARD, 0, 0);
  }
  if (FLAG_CHECK(FLAG_LUNPA_FORTRESS_MIDDLE_GUARD_BEATEN)) {
    OBJECT_POSITION_SET(UPPER_FLOOR_HALL_GUARD, 0, 0);
  }
  if (FLAG_CHECK(FLAG_LUNPA_FORTRESS_EAST_GUARD_BEATEN)) {
    OBJECT_POSITION_SET(UPPER_FLOOR_EAST_GUARD, 0, 0);
  }
  if (FLAG_CHECK(FLAG_LUNPA_FORTRESS_CELL_KEY)) {
    OBJECT_POSITION_SET(UPPER_FLOOR_CELL_KEY, 0, 0);
  }
  switch (gGameState.warpDest) {
  case LUNPA_FORTRESS_UPPER_FLOOR_FROM_LUNPA_FORTRESS_GROUND_FLOOR_SOUTH:
  case LUNPA_FORTRESS_UPPER_FLOOR_WEST_PASSAGE_FROM_NORTH_PASSAGE:
  case LUNPA_FORTRESS_UPPER_FLOOR_WEST_PASSAGE_FROM_MIDDLE_PASSAGE:
    gFieldWork->transition =
        MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
    Engine_MapGridSetShownLayers(LUNPA_FORTRESS_PASSAGE_LAYERS);
    Task_AddCallback(LunpaFortress_UpdatePairedGuardDetection,
                     TASK_PRIORITY_MAP);
    Engine_TaskWait(1);
    Engine_MapGridRedraw();
    Engine_TaskWait(1);
    break;
  case LUNPA_FORTRESS_UPPER_FLOOR_FAMILY_ROOM_FROM_MIDDLE_PASSAGE:
  case LUNPA_FORTRESS_UPPER_FLOOR_WOMENS_ROOM_FROM_NORTH_PASSAGE:
  case LUNPA_FORTRESS_UPPER_FLOOR_DONPAS_ROOM_FROM_SOUTH_PASSAGE:
  case LUNPA_FORTRESS_UPPER_FLOOR_TONIC_ROOM_FROM_DONPAS_ROOM:
  case LUNPA_FORTRESS_UPPER_FLOOR_DONPAS_ROOM_FROM_TONIC_ROOM:
    gFieldWork->transition =
        MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
    Engine_MapGridSetShownLayers(LUNPA_FORTRESS_ROOM_LAYERS);
    Engine_ObjectEventSetSpriteFlags(ObjectEvent_Get(UPPER_FLOOR_DONPA), 0);
    if (FLAG_CHECK(FLAG_MAPSEC_TEMP_14)) {
      OBJECT_POSITION_SET(UPPER_FLOOR_ATTENDANT, 872, 120);
    }
    break;
  case LUNPA_FORTRESS_UPPER_FLOOR_EAST_PASSAGE_FROM_CELL_KEY_ROOM:
  case LUNPA_FORTRESS_UPPER_FLOOR_EAST_PASSAGE_FROM_SOUTH_PASSAGE:
    gFieldWork->transition =
        MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
    Engine_MapGridSetShownLayers(LUNPA_FORTRESS_PASSAGE_LAYERS);
    Task_AddCallback(LunpaFortress_UpdateHallGuardDetection, TASK_PRIORITY_MAP);
    Engine_TaskWait(1);
    Engine_MapGridRedraw();
    Engine_TaskWait(1);
    break;
  case LUNPA_FORTRESS_UPPER_FLOOR_GUARD_ROOM_FROM_NORTH_PASSAGE:
  case LUNPA_FORTRESS_UPPER_FLOOR_GUARD_ROOM_FROM_SOUTH_PASSAGE:
    gFieldWork->transition =
        MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
    if (FLAG_CHECK(FLAG_LUNPA_FORTRESS_THREE_GUARDS_BEATEN)) {
      LunpaFortress_PlaceThreeBeatenGuards();
    }
    break;
  case LUNPA_FORTRESS_UPPER_FLOOR_GUARD_ROOM_AFTER_BATTLE:
    gFieldWork->transition =
        MAP_TRANSITION(TRANSITION_WINDOW, LUNPA_RETURN_TRANSITION_VARIANT);
    LunpaFortress_PlaceThreeBeatenGuards();
    break;
  case LUNPA_FORTRESS_UPPER_FLOOR_CELL_KEY_ROOM_FROM_MIDDLE_PASSAGE:
  case LUNPA_FORTRESS_UPPER_FLOOR_CELL_KEY_ROOM_FROM_EAST_PASSAGE:
  case LUNPA_FORTRESS_UPPER_FLOOR_FROM_LUNPA_FORTRESS_GROUND_FLOOR_NORTH:
    Task_AddCallback(LunpaFortress_CheckUpperFloorDoorApproach,
                     TASK_PRIORITY_MAP);
    break;
  default:
    gFieldWork->transition =
        MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
    Engine_MapGridSetShownLayers(LUNPA_FORTRESS_PASSAGE_LAYERS);
    break;
  }
  keyObjectEvent = ObjectEvent_Get(UPPER_FLOOR_CELL_KEY);
  Engine_ObjectEventSetSpriteFlags(ObjectEvent_Get(UPPER_FLOOR_CELL_KEY), 0);
  Engine_ObjectEventSetSpritePriority(UPPER_FLOOR_CELL_KEY, 1);
  keyObjectEvent->scaleX = 3 * Q16_ONE / 4;
  keyObjectEvent->scaleY = 3 * Q16_ONE / 4;
}

/// Sets LunpaFortressCave up: the doors, the crates, the Whirlwind covers and
/// the switches as the party left them.
static void LunpaFortress_InitializeCave(void) {
  struct ObjectEvent *mapObjectEvent;

  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
  LunpaFortress_RestoreCaveDoors();
  if (FLAG_CHECK(FLAG_LUNPA_FORTRESS_CRATES_MOVED)) {
    LunpaFortress_RestoreLoweredCrates();
  }
  FLAG_SET(FLAG_MAP_TEMP_17);
  FLAG_SET(FLAG_MAP_TEMP_18);
  if (FLAG_CHECK(FLAG_LUNPA_FORTRESS_FIRST_SWITCH_PLANT_BLOWN_AWAY)) {
    OBJECT_POSITION_SET(CAVE_FIRST_SWITCH_COVER, 0, 0);
    Engine_GameFlagClear(FLAG_MAP_TEMP_17);
  }
  if (FLAG_CHECK(FLAG_LUNPA_FORTRESS_PASSAGE_PLANT_BLOWN_AWAY)) {
    OBJECT_POSITION_SET(CAVE_PASSAGE_COVER, 0, 0);
    LunpaFortress_OpenCoveredPassage();
  }
  if (FLAG_CHECK(FLAG_LUNPA_FORTRESS_SECOND_SWITCH_PLANT_BLOWN_AWAY)) {
    OBJECT_POSITION_SET(CAVE_SECOND_SWITCH_COVER, 0, 0);
    Engine_GameFlagClear(FLAG_MAP_TEMP_18);
  }
  if (FLAG_CHECK(FLAG_LUNPA_FORTRESS_FIRST_SWITCH_FLIPPED)) {
    LunpaFortress_OpenFirstSwitchPassage();
  }
  if (FLAG_CHECK(FLAG_LUNPA_FORTRESS_SECOND_SWITCH_FLIPPED)) {
    LunpaFortress_OpenSecondSwitchPassage();
  }
  SCRIPT_START();
  mapObjectEvent = ObjectEvent_Get(CAVE_FIRST_SWITCH_COVER);
  if (mapObjectEvent != 0) {
    mapObjectEvent->priorityFlags = OBJ_EVENT_PRIORITY_UNDERFOOT;
  }
  mapObjectEvent = ObjectEvent_Get(CAVE_PASSAGE_COVER);
  if (mapObjectEvent != 0) {
    mapObjectEvent->priorityFlags = OBJ_EVENT_PRIORITY_UNDERFOOT;
  }
  mapObjectEvent = ObjectEvent_Get(CAVE_SECOND_SWITCH_COVER);
  if (mapObjectEvent != 0) {
    mapObjectEvent->priorityFlags = OBJ_EVENT_PRIORITY_UNDERFOOT;
  }
  mapObjectEvent = ObjectEvent_Get(CAVE_JAR_BLOCKER);
  if (mapObjectEvent != 0) {
    Engine_ObjectEventSetSpriteFlags(mapObjectEvent, 0);
  }
  mapObjectEvent->priorityFlags = OBJ_EVENT_PRIORITY_UNDERFOOT;
  mapObjectEvent = ObjectEvent_Get(CAVE_CRATES);
  if (mapObjectEvent != 0) {
    mapObjectEvent->collisionFlags |= OBJ_EVENT_NEAR_PSYNERGY;
  }
  Engine_ObjectEventSetSpriteFlags(ObjectEvent_Get(CAVE_JAR_BLOCKER), 0);
  SCRIPT_END();
  Engine_MapGridSetShownLayers(LUNPA_FORTRESS_PASSAGE_LAYERS);
}

/// LunpaFortressGroundFloor's opened cell doors stay open.
static void LunpaFortress_RestoreGroundFloorDoors(void) {
  if (Engine_GameFlagIsSet(GROUND_FLOOR_DOOR_FLAG)) {
    LunpaFortress_OpenGroundFloorDoor(0);
  }
  if (Engine_GameFlagIsSet(GROUND_FLOOR_DOOR_FLAG + 1)) {
    LunpaFortress_OpenGroundFloorDoor(1);
  }
  if (Engine_GameFlagIsSet(GROUND_FLOOR_DOOR_FLAG + 2)) {
    LunpaFortress_OpenGroundFloorDoor(2);
  }
}

/// LunpaFortressUpperFloor's opened cell doors stay open.
static void LunpaFortress_RestoreUpperFloorDoors(void) {
  if (Engine_GameFlagIsSet(UPPER_FLOOR_DOOR_FLAG)) {
    LunpaFortress_OpenUpperFloorDoor(0);
  }
  if (Engine_GameFlagIsSet(UPPER_FLOOR_DOOR_FLAG + 1)) {
    LunpaFortress_OpenUpperFloorDoor(1);
  }
}

/// LunpaFortressCave's opened cell doors stay open.
static void LunpaFortress_RestoreCaveDoors(void) {
  if (Engine_GameFlagIsSet(CAVE_DOOR_FLAG)) {
    LunpaFortress_OpenCaveDoor(0);
  }
  if (Engine_GameFlagIsSet(CAVE_DOOR_FLAG + 1)) {
    LunpaFortress_OpenCaveDoor(1);
  }
  if (Engine_GameFlagIsSet(CAVE_DOOR_FLAG + 2)) {
    LunpaFortress_OpenCaveDoor(2);
  }
}

/// LunpaFortressToadonpa's opened cell doors stay open.
static void LunpaFortress_RestoreToadonpaDoors(void) {
  if (Engine_GameFlagIsSet(TOADONPA_DOOR_FLAG)) {
    LunpaFortress_OpenToadonpaDoor(0);
  }
  if (Engine_GameFlagIsSet(TOADONPA_DOOR_FLAG + 1)) {
    LunpaFortress_OpenToadonpaDoor(1);
  }
  if (Engine_GameFlagIsSet(TOADONPA_DOOR_FLAG + 2)) {
    LunpaFortress_OpenToadonpaDoor(2);
  }
  if (Engine_GameFlagIsSet(TOADONPA_DOOR_FLAG + 3)) {
    LunpaFortress_OpenToadonpaDoor(3);
  }
  if (Engine_GameFlagIsSet(TOADONPA_DOOR_FLAG + 4)) {
    LunpaFortress_OpenToadonpaDoor(4);
  }
}

/// Pixel steps pack X above Z in two halfwords. Unsigned packing also defines
/// the entries with negative X steps.
#define DIRECTION_STEP(dx, dz) (((u32)(dx) << 16) | ((u32)(dz) & 0xffff))

// The ROM keeps these three tables in .rodata, but the shared declarations in
// field.h and object_staged_object_event.h lack const, so a section attribute
// places each one.
s32 gPushedObject_DirectionSteps[] __attribute__((section(".rodata"))) = {
    DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(16, 0),
};

// clang-format off
u32 gPushedObject_FootprintKinds[] __attribute__((section(".rodata"))) = {
    207, 205, 228, 229, 298, 297,
};

s32 gPushedObject_FootprintBounds[] __attribute__((section(".rodata"))) = {
    -32, -8, 32, 8,
    -8, -32, 8, 32,
    -32, -16, 32, 0,
    -8, -32, 8, 32,
    -32, -8, 32, 8,
    -8, -32, 8, 32,
};

s32 gLunpaFortressRandomPick = 0;

s32 gSecondPatrolRoute[] = {
    SC_SET(SW_ACCELERATION, PIXELS(11) / 10),
    SC_SET(SW_SPEED, PIXELS(11) / 10),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(192), 0, PIXELS(288)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(88), 0, PIXELS(288)),
    SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gFirstPatrolRoute[] = {
    SC_SET(SW_ACCELERATION, PIXELS(11) / 10),
    SC_SET(SW_SPEED, PIXELS(11) / 10),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(192), 0, PIXELS(192)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(88), 0, PIXELS(192)),
    SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gDonpaSleeps[] = {
    SC_ANIMATION(5),
    SC_END,
};

s32 gGroundFloorPatrolRoute[] = {
    SC_SET(SW_ACCELERATION, PIXELS(1) / 5),
    SC_SET(SW_SPEED, PIXELS(3) / 5),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(808), 0, PIXELS(648)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(25),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(808), 0, PIXELS(520)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(20),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(712), 0, PIXELS(520)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(25),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(808), 0, PIXELS(520)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(40),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(808), 0, PIXELS(648)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(20),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(744), 0, PIXELS(648)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(30),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gHallPatrolRoute[] = {
    SC_SET(SW_ACCELERATION, PIXELS(1) / 5),
    SC_SET(SW_SPEED, PIXELS(3) / 5),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(920), 0, PIXELS(648)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(840), 0, PIXELS(648)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(840), 0, PIXELS(592)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(840), 0, PIXELS(648)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(920), 0, PIXELS(648)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(920), 0, PIXELS(592)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

struct MapWarpDest gLunpaFortressGroundFloorWarpDests[] = {
    { WARP_DEST_DEFAULT, CONDITION_ALWAYS, 168, 0, 632, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { LUNPA_FORTRESS_GROUND_FLOOR_FROM_LUNPA_FORTRESS_GATES, CONDITION_ALWAYS, 160, 0, 168, FACING_NORTH, 0, 40, 24, 296, 280, 0 },
    { LUNPA_FORTRESS_GROUND_FLOOR_ENTRANCE_FROM_MAIN_PASSAGE, CONDITION_ALWAYS, 160, 0, 104, FACING_SOUTH, 0, 40, 24, 296, 280, 0 },
    { LUNPA_FORTRESS_GROUND_FLOOR_MAIN_PASSAGE_FROM_ENTRANCE, CONDITION_ALWAYS, 272, 0, 896, FACING_NORTH, 0, 24, 616, 536, 936, 0 },
    { LUNPA_FORTRESS_GROUND_FLOOR_MAIN_PASSAGE_FROM_NORTH_PASSAGE, CONDITION_ALWAYS, 376, 0, 896, FACING_NORTH, 0, 24, 616, 536, 936, 0 },
    { LUNPA_FORTRESS_GROUND_FLOOR_MAIN_PASSAGE_FROM_FEAST_ROOM, CONDITION_ALWAYS, 248, 0, 856, FACING_SOUTH, 0, 24, 616, 536, 936, 0 },
    { LUNPA_FORTRESS_GROUND_FLOOR_MAIN_PASSAGE_FROM_NORTHEAST_PASSAGE, CONDITION_ALWAYS, 488, 0, 680, FACING_SOUTH, 0, 24, 616, 536, 936, 0 },
    { LUNPA_FORTRESS_GROUND_FLOOR_MAIN_PASSAGE_FROM_EAST_PASSAGE, CONDITION_ALWAYS, 72, 0, 696, FACING_SOUTH, 0, 24, 616, 536, 936, 0 },
    { LUNPA_FORTRESS_GROUND_FLOOR_FROM_LUNPA_FORTRESS_UPPER_FLOOR_SOUTH, CONDITION_ALWAYS, 96, 0, 856, FACING_SOUTH, 0, 24, 616, 536, 936, 0 },
    { LUNPA_FORTRESS_GROUND_FLOOR_NORTH_PASSAGE_FROM_MAIN_PASSAGE, CONDITION_ALWAYS, 376, 0, 184, FACING_SOUTH, 0, 328, 8, 568, 232, 0 },
    { LUNPA_FORTRESS_GROUND_FLOOR_FROM_LUNPA_FORTRESS_UPPER_FLOOR_NORTH, CONDITION_ALWAYS, 504, 0, 88, FACING_SOUTH, 0, 328, 8, 568, 232, 0 },
    { LUNPA_FORTRESS_GROUND_FLOOR_FROM_LUNPA_FORTRESS_CAVE, CONDITION_ALWAYS, 440, 0, 104, FACING_SOUTH, 0, 328, 8, 568, 232, 0 },
    { LUNPA_FORTRESS_GROUND_FLOOR_FEAST_ROOM_FROM_MAIN_PASSAGE, CONDITION_ALWAYS, 136, 0, 512, FACING_NORTH, 0, 40, 328, 280, 552, 0 },
    { LUNPA_FORTRESS_GROUND_FLOOR_NORTHEAST_PASSAGE_FROM_MAIN_PASSAGE, CONDITION_ALWAYS, 888, 0, 280, FACING_NORTH, 0, 632, 40, 952, 328, 0 },
    { LUNPA_FORTRESS_GROUND_FLOOR_NORTHEAST_PASSAGE_FROM_EAST_PASSAGE, CONDITION_ALWAYS, 696, 0, 264, FACING_SOUTH, 0, 632, 40, 952, 328, 0 },
    { LUNPA_FORTRESS_GROUND_FLOOR_NORTHEAST_PASSAGE_FROM_BARREL_ROOM, CONDITION_ALWAYS, 824, 0, 120, FACING_SOUTH, 0, 632, 40, 952, 328, 0 },
    { LUNPA_FORTRESS_GROUND_FLOOR_EAST_PASSAGE_FROM_MAIN_PASSAGE, CONDITION_ALWAYS, 664, 0, 632, FACING_NORTH, 0, 616, 344, 936, 696, 0 },
    { LUNPA_FORTRESS_GROUND_FLOOR_EAST_PASSAGE_FROM_NORTHEAST_PASSAGE, CONDITION_ALWAYS, 888, 0, 520, FACING_NORTH, 0, 616, 344, 936, 696, 0 },
    { LUNPA_FORTRESS_GROUND_FLOOR_EAST_PASSAGE_FROM_TREASURE_ROOM, CONDITION_ALWAYS, 696, 0, 432, FACING_SOUTH, 0, 616, 344, 936, 696, 0 },
    { LUNPA_FORTRESS_GROUND_FLOOR_BARREL_ROOM_FROM_NORTHEAST_PASSAGE, CONDITION_ALWAYS, 440, 0, 448, FACING_NORTH, 0, 320, 312, 560, 504, 0 },
    { LUNPA_FORTRESS_GROUND_FLOOR_TREASURE_ROOM_FROM_EAST_PASSAGE, CONDITION_ALWAYS, 696, 0, 824, FACING_NORTH, 0, 616, 744, 856, 952, 0 },
    { MAP_TABLE_END },
};

struct MapWarpDest gLunpaFortressUpperFloorWarpDests[] = {
    { WARP_DEST_DEFAULT, CONDITION_ALWAYS, 168, 0, 632, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_FROM_LUNPA_FORTRESS_GROUND_FLOOR_SOUTH, CONDITION_ALWAYS, 136, 0, 392, FACING_SOUTH, 0, 40, 8, 308, 448, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_WEST_PASSAGE_FROM_NORTH_PASSAGE, CONDITION_ALWAYS, 136, 0, 80, FACING_SOUTH, 0, 40, 8, 308, 448, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_WEST_PASSAGE_FROM_MIDDLE_PASSAGE, CONDITION_ALWAYS, 264, 0, 416, FACING_SOUTH, 0, 40, 8, 308, 448, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_NORTH_PASSAGE_FROM_WEST_PASSAGE, CONDITION_ALWAYS, 376, 0, 232, FACING_NORTH, 0, 328, 8, 568, 272, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_NORTH_PASSAGE_FROM_GUARD_ROOM, CONDITION_ALWAYS, 504, 0, 104, FACING_NORTH, 0, 328, 8, 568, 272, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_NORTH_PASSAGE_FROM_WOMENS_ROOM, CONDITION_ALWAYS, 376, 0, 176, FACING_SOUTH, 0, 328, 8, 568, 272, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_MIDDLE_PASSAGE_FROM_WEST_PASSAGE, CONDITION_ALWAYS, 408, 0, 456, FACING_NORTH, 0, 328, 272, 584, 512, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_MIDDLE_PASSAGE_FROM_FAMILY_ROOM, CONDITION_ALWAYS, 456, 0, 352, FACING_SOUTH, 0, 328, 272, 584, 512, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_MIDDLE_PASSAGE_FROM_CELL_KEY_ROOM, CONDITION_ALWAYS, 504, 0, 456, FACING_NORTH, 0, 328, 272, 584, 512, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_FAMILY_ROOM_FROM_MIDDLE_PASSAGE, CONDITION_ALWAYS, 136, 0, 640, FACING_NORTH, 0, 16, 472, 256, 688, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_GUARD_ROOM_FROM_NORTH_PASSAGE, CONDITION_ALWAYS, 648, 0, 72, FACING_SOUTH, 0, 568, 16, 808, 176, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_GUARD_ROOM_FROM_SOUTH_PASSAGE, CONDITION_ALWAYS, 744, 0, 80, FACING_SOUTH, 0, 568, 16, 808, 176, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_WOMENS_ROOM_FROM_NORTH_PASSAGE, CONDITION_ALWAYS, 696, 0, 376, FACING_NORTH, 0, 576, 264, 816, 432, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_CELL_KEY_ROOM_FROM_MIDDLE_PASSAGE, CONDITION_ALWAYS, 200, 0, 848, FACING_SOUTH, 0, 152, 680, 424, 992, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_CELL_KEY_ROOM_FROM_EAST_PASSAGE, CONDITION_ALWAYS, 264, 0, 776, FACING_SOUTH, 0, 152, 680, 424, 992, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_FROM_LUNPA_FORTRESS_GROUND_FLOOR_NORTH, CONDITION_ALWAYS, 376, 0, 824, FACING_SOUTH, 0, 152, 680, 424, 992, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_SOUTH_PASSAGE_FROM_GUARD_ROOM, CONDITION_ALWAYS, 536, 0, 696, FACING_NORTH, 0, 488, 584, 760, 896, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_SOUTH_PASSAGE_FROM_DONPAS_ROOM, CONDITION_ALWAYS, 680, 0, 832, FACING_SOUTH, 0, 488, 584, 760, 896, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_SOUTH_PASSAGE_FROM_EAST_PASSAGE, CONDITION_ALWAYS, 712, 0, 856, FACING_NORTH, 0, 488, 584, 760, 896, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_DONPAS_ROOM_FROM_SOUTH_PASSAGE, CONDITION_ALWAYS, 912, 0, 192, FACING_NORTH, 0, 784, 24, 1024, 240, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_EAST_PASSAGE_FROM_CELL_KEY_ROOM, CONDITION_ALWAYS, 840, 0, 664, FACING_NORTH, 0, 768, 440, 1008, 704, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_EAST_PASSAGE_FROM_SOUTH_PASSAGE, CONDITION_ALWAYS, 952, 0, 528, FACING_SOUTH, 0, 768, 440, 1008, 704, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_TONIC_ROOM_FROM_DONPAS_ROOM, CONDITION_ALWAYS, 872, 0, 376, FACING_NORTH, 0, 768, 264, 1016, 432, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_DONPAS_ROOM_FROM_TONIC_ROOM, CONDITION_ALWAYS, 904, 0, 112, FACING_SOUTH, 0, 784, 24, 1024, 240, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_MIDDLE_PASSAGE_UNUSED, CONDITION_ALWAYS, 408, 0, 456, FACING_NORTH, 0, 328, 272, 584, 512, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_GUARD_ROOM_AFTER_BATTLE, CONDITION_ALWAYS, 648, 0, 88, FACING_NORTH, 0, 568, 16, 808, 176, 0 },
    { MAP_TABLE_END },
};

struct MapWarpDest gLunpaFortressCaveWarpDests[] = {
    { WARP_DEST_DEFAULT, CONDITION_ALWAYS, 168, 0, 632, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { LUNPA_FORTRESS_CAVE_FROM_LUNPA_FORTRESS_GROUND_FLOOR, CONDITION_ALWAYS, 424, 0, 584, FACING_SOUTH, 0, 200, 488, 480, 648, 0 },
    { LUNPA_FORTRESS_CAVE_ENTRANCE_FROM_LAKE, CONDITION_ALWAYS, 344, 0, 568, FACING_SOUTH, 0, 200, 488, 480, 648, 0 },
    { LUNPA_FORTRESS_CAVE_LAKE_FROM_ENTRANCE, CONDITION_ALWAYS, 312, 0, 448, FACING_NORTH, 0, 8, 8, 568, 480, 0 },
    { LUNPA_FORTRESS_CAVE_FROM_LUNPA_FORTRESS_TOADONPA_NORTHEAST, CONDITION_ALWAYS, 440, 0, 296, FACING_SOUTH, 0, 8, 8, 568, 480, 0 },
    { LUNPA_FORTRESS_CAVE_FROM_LUNPA_FORTRESS_TOADONPA_NORTH, CONDITION_ALWAYS, 312, 0, 248, FACING_SOUTH, 0, 8, 8, 568, 480, 0 },
    { LUNPA_FORTRESS_CAVE_FROM_LUNPA_FORTRESS_TOADONPA_NORTHWEST, CONDITION_ALWAYS, 104, 0, 184, FACING_SOUTH, 0, 8, 8, 568, 480, 0 },
    { MAP_TABLE_END },
};

struct MapWarpDest gLunpaFortressWarpDestsOther[] = {
    { WARP_DEST_DEFAULT, CONDITION_ALWAYS, 168, 0, 632, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { LUNPA_FORTRESS_TOADONPA_FROM_LUNPA_FORTRESS_CAVE_NORTH, CONDITION_ALWAYS, 56, 0, 328, FACING_NORTH, 0, 8, 8, 264, 424, 0 },
    { LUNPA_FORTRESS_TOADONPA_FROM_LUNPA_FORTRESS_CAVE_NORTHEAST, CONDITION_ALWAYS, 184, 0, 368, FACING_NORTH, 0, 8, 8, 264, 424, 0 },
    { LUNPA_FORTRESS_TOADONPA_FROM_LUNPA_FORTRESS_CAVE_NORTHWEST, CONDITION_ALWAYS, 328, 0, 344, FACING_SOUTH, 0, 288, 32, 576, 432, 0 },
    { LUNPA_FORTRESS_TOADONPA_AFTER_BATTLE, CONDITION_ALWAYS, 440, 0, 224, FACING_NORTH, 0, 288, 32, 576, 432, 0 },
    { MAP_TABLE_END },
};

/// LunpaFortressGroundFloor's warps. Unused: its rows first copy West Lunpa
/// Cave's, which no map of this overlay reads.
u32 gLunpaFortressWarpsOther[] = {
    MAP_WARPS(WestLunpaCave),
    MAP_WARP(WEST_LUNPA_CAVE_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_WEST_LUNPA_CAVE),
    MAP_WARP(WEST_LUNPA_CAVE_TO_LUNPA, Lunpa, LUNPA_FROM_WEST_LUNPA_CAVE),
    MAP_WARP(WEST_LUNPA_CAVE_BY_WAGON, Kalay, KALAY_FROM_HAMMET_PALACE_GROUNDS),
    MAP_WARPS(LunpaFortressGroundFloor),
    MAP_WARP(LUNPA_FORTRESS_GROUND_FLOOR_TO_LUNPA_FORTRESS_GATES, LunpaFortressGates, LUNPA_FORTRESS_GATES_FROM_LUNPA_FORTRESS_GROUND_FLOOR),
    MAP_WARP(WARP_THROWN_OUT, LunpaFortressGates, LUNPA_FORTRESS_GATES_AFTER_CAUGHT),
    MAP_WARP(LUNPA_FORTRESS_GROUND_FLOOR_ENTRANCE_TO_MAIN_PASSAGE, LunpaFortressGroundFloor, LUNPA_FORTRESS_GROUND_FLOOR_MAIN_PASSAGE_FROM_ENTRANCE),
    MAP_WARP(LUNPA_FORTRESS_GROUND_FLOOR_MAIN_PASSAGE_TO_ENTRANCE, LunpaFortressGroundFloor, LUNPA_FORTRESS_GROUND_FLOOR_ENTRANCE_FROM_MAIN_PASSAGE),
    MAP_WARP(LUNPA_FORTRESS_GROUND_FLOOR_MAIN_PASSAGE_TO_NORTH_PASSAGE, LunpaFortressGroundFloor, LUNPA_FORTRESS_GROUND_FLOOR_NORTH_PASSAGE_FROM_MAIN_PASSAGE),
    MAP_WARP(LUNPA_FORTRESS_GROUND_FLOOR_MAIN_PASSAGE_TO_FEAST_ROOM, LunpaFortressGroundFloor, LUNPA_FORTRESS_GROUND_FLOOR_FEAST_ROOM_FROM_MAIN_PASSAGE),
    MAP_WARP(LUNPA_FORTRESS_GROUND_FLOOR_MAIN_PASSAGE_TO_NORTHEAST_PASSAGE, LunpaFortressGroundFloor, LUNPA_FORTRESS_GROUND_FLOOR_NORTHEAST_PASSAGE_FROM_MAIN_PASSAGE),
    MAP_WARP(LUNPA_FORTRESS_GROUND_FLOOR_MAIN_PASSAGE_TO_EAST_PASSAGE, LunpaFortressGroundFloor, LUNPA_FORTRESS_GROUND_FLOOR_EAST_PASSAGE_FROM_MAIN_PASSAGE),
    MAP_WARP(LUNPA_FORTRESS_GROUND_FLOOR_TO_LUNPA_FORTRESS_UPPER_FLOOR_SOUTH, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_FROM_LUNPA_FORTRESS_GROUND_FLOOR_SOUTH),
    MAP_WARP(LUNPA_FORTRESS_GROUND_FLOOR_NORTH_PASSAGE_TO_MAIN_PASSAGE, LunpaFortressGroundFloor, LUNPA_FORTRESS_GROUND_FLOOR_MAIN_PASSAGE_FROM_NORTH_PASSAGE),
    MAP_WARP(LUNPA_FORTRESS_GROUND_FLOOR_TO_LUNPA_FORTRESS_UPPER_FLOOR_NORTH, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_FROM_LUNPA_FORTRESS_GROUND_FLOOR_NORTH),
    MAP_WARP(LUNPA_FORTRESS_GROUND_FLOOR_TO_LUNPA_FORTRESS_CAVE, LunpaFortressCave, LUNPA_FORTRESS_CAVE_FROM_LUNPA_FORTRESS_GROUND_FLOOR),
    MAP_WARP(LUNPA_FORTRESS_GROUND_FLOOR_FEAST_ROOM_TO_MAIN_PASSAGE, LunpaFortressGroundFloor, LUNPA_FORTRESS_GROUND_FLOOR_MAIN_PASSAGE_FROM_FEAST_ROOM),
    MAP_WARP(LUNPA_FORTRESS_GROUND_FLOOR_NORTHEAST_PASSAGE_TO_MAIN_PASSAGE, LunpaFortressGroundFloor, LUNPA_FORTRESS_GROUND_FLOOR_MAIN_PASSAGE_FROM_NORTHEAST_PASSAGE),
    MAP_WARP(LUNPA_FORTRESS_GROUND_FLOOR_NORTHEAST_PASSAGE_TO_EAST_PASSAGE, LunpaFortressGroundFloor, LUNPA_FORTRESS_GROUND_FLOOR_EAST_PASSAGE_FROM_NORTHEAST_PASSAGE),
    MAP_WARP(LUNPA_FORTRESS_GROUND_FLOOR_NORTHEAST_PASSAGE_TO_BARREL_ROOM, LunpaFortressGroundFloor, LUNPA_FORTRESS_GROUND_FLOOR_BARREL_ROOM_FROM_NORTHEAST_PASSAGE),
    MAP_WARP(LUNPA_FORTRESS_GROUND_FLOOR_EAST_PASSAGE_TO_MAIN_PASSAGE, LunpaFortressGroundFloor, LUNPA_FORTRESS_GROUND_FLOOR_MAIN_PASSAGE_FROM_EAST_PASSAGE),
    MAP_WARP(LUNPA_FORTRESS_GROUND_FLOOR_EAST_PASSAGE_TO_NORTHEAST_PASSAGE, LunpaFortressGroundFloor, LUNPA_FORTRESS_GROUND_FLOOR_NORTHEAST_PASSAGE_FROM_EAST_PASSAGE),
    MAP_WARP(LUNPA_FORTRESS_GROUND_FLOOR_EAST_PASSAGE_TO_TREASURE_ROOM, LunpaFortressGroundFloor, LUNPA_FORTRESS_GROUND_FLOOR_TREASURE_ROOM_FROM_EAST_PASSAGE),
    MAP_WARP(LUNPA_FORTRESS_GROUND_FLOOR_BARREL_ROOM_TO_NORTHEAST_PASSAGE, LunpaFortressGroundFloor, LUNPA_FORTRESS_GROUND_FLOOR_NORTHEAST_PASSAGE_FROM_BARREL_ROOM),
    MAP_WARP(LUNPA_FORTRESS_GROUND_FLOOR_TREASURE_ROOM_TO_EAST_PASSAGE, LunpaFortressGroundFloor, LUNPA_FORTRESS_GROUND_FLOOR_EAST_PASSAGE_FROM_TREASURE_ROOM),
    MAP_WARPS_END,
};

u32 gLunpaFortressUpperFloorWarps[] = {
    MAP_WARPS(LunpaFortressUpperFloor),
    MAP_WARP(WARP_THROWN_OUT, LunpaFortressGates, LUNPA_FORTRESS_GATES_AFTER_CAUGHT),
    MAP_WARP(LUNPA_FORTRESS_UPPER_FLOOR_TO_LUNPA_FORTRESS_GROUND_FLOOR_SOUTH, LunpaFortressGroundFloor, LUNPA_FORTRESS_GROUND_FLOOR_FROM_LUNPA_FORTRESS_UPPER_FLOOR_SOUTH),
    MAP_WARP(LUNPA_FORTRESS_UPPER_FLOOR_WEST_PASSAGE_TO_NORTH_PASSAGE, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_NORTH_PASSAGE_FROM_WEST_PASSAGE),
    MAP_WARP(LUNPA_FORTRESS_UPPER_FLOOR_WEST_PASSAGE_TO_MIDDLE_PASSAGE, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_MIDDLE_PASSAGE_FROM_WEST_PASSAGE),
    MAP_WARP(LUNPA_FORTRESS_UPPER_FLOOR_NORTH_PASSAGE_TO_WEST_PASSAGE, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_WEST_PASSAGE_FROM_NORTH_PASSAGE),
    MAP_WARP(LUNPA_FORTRESS_UPPER_FLOOR_NORTH_PASSAGE_TO_GUARD_ROOM, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_GUARD_ROOM_FROM_NORTH_PASSAGE),
    MAP_WARP(LUNPA_FORTRESS_UPPER_FLOOR_NORTH_PASSAGE_TO_WOMENS_ROOM, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_WOMENS_ROOM_FROM_NORTH_PASSAGE),
    MAP_WARP(LUNPA_FORTRESS_UPPER_FLOOR_MIDDLE_PASSAGE_TO_WEST_PASSAGE, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_WEST_PASSAGE_FROM_MIDDLE_PASSAGE),
    MAP_WARP(LUNPA_FORTRESS_UPPER_FLOOR_MIDDLE_PASSAGE_TO_FAMILY_ROOM, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_FAMILY_ROOM_FROM_MIDDLE_PASSAGE),
    MAP_WARP(LUNPA_FORTRESS_UPPER_FLOOR_MIDDLE_PASSAGE_TO_CELL_KEY_ROOM, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_CELL_KEY_ROOM_FROM_MIDDLE_PASSAGE),
    MAP_WARP(LUNPA_FORTRESS_UPPER_FLOOR_FAMILY_ROOM_TO_MIDDLE_PASSAGE, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_MIDDLE_PASSAGE_FROM_FAMILY_ROOM),
    MAP_WARP(LUNPA_FORTRESS_UPPER_FLOOR_GUARD_ROOM_TO_NORTH_PASSAGE, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_NORTH_PASSAGE_FROM_GUARD_ROOM),
    MAP_WARP(LUNPA_FORTRESS_UPPER_FLOOR_GUARD_ROOM_TO_SOUTH_PASSAGE, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_SOUTH_PASSAGE_FROM_GUARD_ROOM),
    MAP_WARP(LUNPA_FORTRESS_UPPER_FLOOR_WOMENS_ROOM_TO_NORTH_PASSAGE, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_NORTH_PASSAGE_FROM_WOMENS_ROOM),
    MAP_WARP(LUNPA_FORTRESS_UPPER_FLOOR_CELL_KEY_ROOM_TO_MIDDLE_PASSAGE, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_MIDDLE_PASSAGE_FROM_CELL_KEY_ROOM),
    MAP_WARP(LUNPA_FORTRESS_UPPER_FLOOR_CELL_KEY_ROOM_TO_EAST_PASSAGE, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_EAST_PASSAGE_FROM_CELL_KEY_ROOM),
    MAP_WARP(LUNPA_FORTRESS_UPPER_FLOOR_TO_LUNPA_FORTRESS_GROUND_FLOOR_NORTH, LunpaFortressGroundFloor, LUNPA_FORTRESS_GROUND_FLOOR_FROM_LUNPA_FORTRESS_UPPER_FLOOR_NORTH),
    MAP_WARP(LUNPA_FORTRESS_UPPER_FLOOR_SOUTH_PASSAGE_TO_GUARD_ROOM, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_GUARD_ROOM_FROM_SOUTH_PASSAGE),
    MAP_WARP(LUNPA_FORTRESS_UPPER_FLOOR_SOUTH_PASSAGE_TO_DONPAS_ROOM, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_DONPAS_ROOM_FROM_SOUTH_PASSAGE),
    MAP_WARP(LUNPA_FORTRESS_UPPER_FLOOR_SOUTH_PASSAGE_TO_EAST_PASSAGE, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_EAST_PASSAGE_FROM_SOUTH_PASSAGE),
    MAP_WARP(LUNPA_FORTRESS_UPPER_FLOOR_DONPAS_ROOM_TO_SOUTH_PASSAGE, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_SOUTH_PASSAGE_FROM_DONPAS_ROOM),
    MAP_WARP(LUNPA_FORTRESS_UPPER_FLOOR_EAST_PASSAGE_TO_CELL_KEY_ROOM, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_CELL_KEY_ROOM_FROM_EAST_PASSAGE),
    MAP_WARP(LUNPA_FORTRESS_UPPER_FLOOR_EAST_PASSAGE_TO_SOUTH_PASSAGE, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_SOUTH_PASSAGE_FROM_EAST_PASSAGE),
    MAP_WARP(LUNPA_FORTRESS_UPPER_FLOOR_TONIC_ROOM_TO_DONPAS_ROOM, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_DONPAS_ROOM_FROM_TONIC_ROOM),
    MAP_WARP(LUNPA_FORTRESS_UPPER_FLOOR_DONPAS_ROOM_TO_TONIC_ROOM, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_TONIC_ROOM_FROM_DONPAS_ROOM),
    MAP_WARPS_END,
};

u32 gLunpaFortressCaveAndToadonpaWarps[] = {
    MAP_WARPS(LunpaFortressCave),
    MAP_WARP(LUNPA_FORTRESS_CAVE_TO_LUNPA_FORTRESS_GROUND_FLOOR, LunpaFortressGroundFloor, LUNPA_FORTRESS_GROUND_FLOOR_FROM_LUNPA_FORTRESS_CAVE),
    MAP_WARP(LUNPA_FORTRESS_CAVE_ENTRANCE_TO_LAKE, LunpaFortressCave, LUNPA_FORTRESS_CAVE_LAKE_FROM_ENTRANCE),
    MAP_WARP(LUNPA_FORTRESS_CAVE_LAKE_TO_ENTRANCE, LunpaFortressCave, LUNPA_FORTRESS_CAVE_ENTRANCE_FROM_LAKE),
    MAP_WARP(LUNPA_FORTRESS_CAVE_TO_LUNPA_FORTRESS_TOADONPA_NORTHEAST, LunpaFortressToadonpa, LUNPA_FORTRESS_TOADONPA_FROM_LUNPA_FORTRESS_CAVE_NORTHEAST),
    MAP_WARP(LUNPA_FORTRESS_CAVE_TO_LUNPA_FORTRESS_TOADONPA_NORTH, LunpaFortressToadonpa, LUNPA_FORTRESS_TOADONPA_FROM_LUNPA_FORTRESS_CAVE_NORTH),
    MAP_WARP(LUNPA_FORTRESS_CAVE_TO_LUNPA_FORTRESS_TOADONPA_NORTHWEST, LunpaFortressToadonpa, LUNPA_FORTRESS_TOADONPA_FROM_LUNPA_FORTRESS_CAVE_NORTHWEST),
    MAP_WARP(WARP_THROWN_OUT, LunpaFortressGates, LUNPA_FORTRESS_GATES_AFTER_CAUGHT),
    MAP_WARPS(LunpaFortressToadonpa),
    MAP_WARP(LUNPA_FORTRESS_TOADONPA_TO_LUNPA_FORTRESS_CAVE_NORTH, LunpaFortressCave, LUNPA_FORTRESS_CAVE_FROM_LUNPA_FORTRESS_TOADONPA_NORTH),
    MAP_WARP(LUNPA_FORTRESS_TOADONPA_TO_LUNPA_FORTRESS_CAVE_NORTHEAST, LunpaFortressCave, LUNPA_FORTRESS_CAVE_FROM_LUNPA_FORTRESS_TOADONPA_NORTHEAST),
    MAP_WARP(LUNPA_FORTRESS_TOADONPA_TO_LUNPA_FORTRESS_CAVE_NORTHWEST, LunpaFortressCave, LUNPA_FORTRESS_CAVE_FROM_LUNPA_FORTRESS_TOADONPA_NORTHWEST),
    MAP_WARP(LUNPA_FORTRESS_TOADONPA_TO_LUNPA_FORTRESS_GATES, LunpaFortressGates, LUNPA_FORTRESS_GATES_FROM_LUNPA_FORTRESS_TOADONPA),
    MAP_WARP(WARP_THROWN_OUT, LunpaFortressGates, LUNPA_FORTRESS_GATES_AFTER_CAUGHT),
    MAP_WARPS_END,
};

struct ObjectEventTemplate gLunpaFortressObjectEventTemplatesOther[] = {
    { MAP_TABLE_END },
};

/// Unused: a copy of West Lunpa Cave's object event templates, taken only by
/// a map this overlay never runs for.
struct ObjectEventTemplate gLunpaFortressUnusedWestLunpaCaveObjectEventTemplates[] = {
    { SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(184), 0, PIXELS(328), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(232), 0, PIXELS(296), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(344), 0, PIXELS(252), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(552), 0, PIXELS(184), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_BUNZA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_HAMMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_WEST, TALK_FACING_KEEP, 0 },
    { IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0 },
    { MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0 },
    { GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gLunpaFortressGroundFloorObjectEventTemplates[] = {
    // Unused: four chests at the map's origin, the display object events of the
    // unused sword pickups (FIRST_ITEM_DISPLAY_LOCALID).
    { SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_MAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(136), 0, PIXELS(760), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(392), 0, PIXELS(728), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(104), 0, PIXELS(440), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_MAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(152), 0, PIXELS(464), FACING_NORTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_MAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(192), 0, PIXELS(432), FACING_SOUTH + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_MAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(808), 0, PIXELS(232), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, (s32)gGroundFloorPatrolRoute,
        PIXELS(736), 0, PIXELS(648), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(192), 0, PIXELS(464), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(424), 0, PIXELS(392), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(432), 0, PIXELS(768), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gLunpaFortressUpperFloorObjectEventTemplates[] = {
    { 0xf5, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(264), 0, PIXELS(856), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_MAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, (s32)gFirstPatrolRoute,
        PIXELS(88), 0, PIXELS(192), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, (s32)gSecondPatrolRoute,
        PIXELS(88), 0, PIXELS(288), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(456), 0, PIXELS(392), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(680), 0, PIXELS(88), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(704), 0, PIXELS(104), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(680), 0, PIXELS(128), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(312), 0, PIXELS(840), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(680), 0, PIXELS(824), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, (s32)gHallPatrolRoute,
        PIXELS(920), 0, PIXELS(600), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(88), 0, PIXELS(616), FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(168), 0, PIXELS(560), FACING_WEST, TALK_FACING_PARTY, 0 },
    { SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(148), 0, PIXELS(616), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(72), 0, PIXELS(560), FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(664), 0, PIXELS(360), FACING_NORTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(712), 0, PIXELS(344), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_DONPA, CONDITION_ALWAYS, (s32)gDonpaSleeps,
        PIXELS(944), 0, PIXELS(122), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(904), 0, PIXELS(120), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { MERCURY_DJINNI, FLAG_DJINNI(DJINNI_TONIC), MOVEMENT_TYPE_STAND,
        PIXELS(872), 0, PIXELS(336), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gLunpaFortressCaveObjectEventTemplates[] = {
    { 0xf8, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(280), 0, PIXELS(576), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { 0xf8, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(440), 0, PIXELS(282), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { 0xf8, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(56), 0, PIXELS(176), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(504), 0, PIXELS(168), FACING_NORTH, TALK_FACING_KEEP, 0 },
    { SPRITE_CRATES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(248), 0, PIXELS(136), FACING_NORTH, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gLunpaFortressToadonpaObjectEventTemplates[] = {
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(56), 0, PIXELS(72), FACING_NORTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(136), 0, PIXELS(72), FACING_NORTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(216), 0, PIXELS(72), FACING_NORTH, TALK_FACING_KEEP, 0 },
    { 0xbb, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(472), 0, PIXELS(88), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_HAMMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(528), 0, PIXELS(216), FACING_WEST, TALK_FACING_KEEP, 0 },
    { IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0 },
    { MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0 },
    { GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0 },
    { SPRITE_DODONPA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0 },
    { SPRITE_DONPA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0 },
    { 0x120, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(464), 0, PIXELS(88), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

/// Unused: an empty map event table nothing returns.
struct MapEvent gLunpaFortressNoEvents[] = {
    { MAP_EVENTS_END },
};

struct MapEvent gLunpaFortressGroundFloorEvents[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, LUNPA_FORTRESS_GROUND_FLOOR_TO_LUNPA_FORTRESS_GATES },
    { MAP_EVENT_WARP, 2, CONDITION_ALWAYS, LUNPA_FORTRESS_GROUND_FLOOR_ENTRANCE_TO_MAIN_PASSAGE },
    { MAP_EVENT_WARP, 3, CONDITION_ALWAYS, LUNPA_FORTRESS_GROUND_FLOOR_MAIN_PASSAGE_TO_ENTRANCE },
    { MAP_EVENT_WARP, 4, CONDITION_ALWAYS, LUNPA_FORTRESS_GROUND_FLOOR_MAIN_PASSAGE_TO_NORTH_PASSAGE },
    { MAP_EVENT_WARP, 5, CONDITION_ALWAYS, LUNPA_FORTRESS_GROUND_FLOOR_MAIN_PASSAGE_TO_FEAST_ROOM },
    { MAP_EVENT_WARP, 6, CONDITION_ALWAYS, LUNPA_FORTRESS_GROUND_FLOOR_MAIN_PASSAGE_TO_NORTHEAST_PASSAGE },
    { MAP_EVENT_WARP, 7, CONDITION_ALWAYS, LUNPA_FORTRESS_GROUND_FLOOR_MAIN_PASSAGE_TO_EAST_PASSAGE },
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_UP, 8, CONDITION_ALWAYS, LUNPA_FORTRESS_GROUND_FLOOR_TO_LUNPA_FORTRESS_UPPER_FLOOR_SOUTH },
    { MAP_EVENT_WARP, 9, CONDITION_ALWAYS, LUNPA_FORTRESS_GROUND_FLOOR_NORTH_PASSAGE_TO_MAIN_PASSAGE },
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_UP, 10, CONDITION_ALWAYS, LUNPA_FORTRESS_GROUND_FLOOR_TO_LUNPA_FORTRESS_UPPER_FLOOR_NORTH },
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_DOWN, 11, CONDITION_ALWAYS, LUNPA_FORTRESS_GROUND_FLOOR_TO_LUNPA_FORTRESS_CAVE },
    { MAP_EVENT_WARP, 12, CONDITION_ALWAYS, LUNPA_FORTRESS_GROUND_FLOOR_FEAST_ROOM_TO_MAIN_PASSAGE },
    { MAP_EVENT_WARP, 13, CONDITION_ALWAYS, LUNPA_FORTRESS_GROUND_FLOOR_NORTHEAST_PASSAGE_TO_MAIN_PASSAGE },
    { MAP_EVENT_WARP, 14, CONDITION_ALWAYS, LUNPA_FORTRESS_GROUND_FLOOR_NORTHEAST_PASSAGE_TO_EAST_PASSAGE },
    { MAP_EVENT_WARP, 15, CONDITION_ALWAYS, LUNPA_FORTRESS_GROUND_FLOOR_NORTHEAST_PASSAGE_TO_BARREL_ROOM },
    { MAP_EVENT_WARP, 16, CONDITION_ALWAYS, LUNPA_FORTRESS_GROUND_FLOOR_EAST_PASSAGE_TO_MAIN_PASSAGE },
    { MAP_EVENT_WARP, 17, CONDITION_ALWAYS, LUNPA_FORTRESS_GROUND_FLOOR_EAST_PASSAGE_TO_NORTHEAST_PASSAGE },
    { MAP_EVENT_WARP, 18, CONDITION_ALWAYS, LUNPA_FORTRESS_GROUND_FLOOR_EAST_PASSAGE_TO_TREASURE_ROOM },
    { MAP_EVENT_WARP, 19, CONDITION_ALWAYS, LUNPA_FORTRESS_GROUND_FLOOR_BARREL_ROOM_TO_NORTHEAST_PASSAGE },
    { MAP_EVENT_WARP, 20, CONDITION_ALWAYS, LUNPA_FORTRESS_GROUND_FLOOR_TREASURE_ROOM_TO_EAST_PASSAGE },
    { BG_EVENT(BG_EVENT_SILENT), TRIGGER_LOCKED_DOOR, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SearchLockedDoor) },
    { COORD_EVENT_FACING(FACING_NORTH), TRIGGER_DOOR_FIRST, GROUND_FLOOR_DOOR_FLAG, EVENT_SCRIPT(EventScript_UnlockGroundFloorDoor) },
    { COORD_EVENT_FACING(FACING_SOUTH), TRIGGER_DOOR_FIRST, GROUND_FLOOR_DOOR_FLAG, EVENT_SCRIPT(EventScript_UnlockGroundFloorDoor) },
    { COORD_EVENT_FACING(FACING_NORTH), TRIGGER_DOOR_FIRST + 1, GROUND_FLOOR_DOOR_FLAG + 1, EVENT_SCRIPT(EventScript_UnlockGroundFloorDoor) },
    { COORD_EVENT_FACING(FACING_SOUTH), TRIGGER_DOOR_FIRST + 1, GROUND_FLOOR_DOOR_FLAG + 1, EVENT_SCRIPT(EventScript_UnlockGroundFloorDoor) },
    { COORD_EVENT_FACING(FACING_NORTH), TRIGGER_DOOR_FIRST + 2, GROUND_FLOOR_DOOR_FLAG + 2, EVENT_SCRIPT(EventScript_UnlockGroundFloorDoor) },
    { COORD_EVENT_FACING(FACING_SOUTH), TRIGGER_DOOR_FIRST + 2, GROUND_FLOOR_DOOR_FLAG + 2, EVENT_SCRIPT(EventScript_UnlockGroundFloorDoor) },
    { MAP_EVENT_COORD, 50, FLAG_MAP_TEMP_24, EVENT_SCRIPT(EventScript_EntranceGuardThrowsPartyOut) },
    { MAP_EVENT_COORD, 51, FLAG_MAP_TEMP_25, EVENT_SCRIPT(EventScript_SentriesThrowPartyOut) },
    { MAP_EVENT_TALK, GROUND_FLOOR_MAN_WONDERING_ABOUT_DONPA, CONDITION_ALWAYS, MSGNO(MsgLunpaDonpaResting) },
    { MAP_EVENT_TALK, GROUND_FLOOR_WOMAN_SEEKING_DODONPA, CONDITION_ALWAYS, MSGNO(MsgLunpaWhereIsDodonpa) },
    { MAP_EVENT_TALK, GROUND_FLOOR_FEASTING_MAN, CONDITION_ALWAYS, MSGNO(MsgLunpaHammetBuysDrink) },
    { MAP_EVENT_TALK, GROUND_FLOOR_BOASTING_MAN, CONDITION_ALWAYS, MSGNO(MsgLunpaHammetMoneyTree) },
    { MAP_EVENT_TALK, GROUND_FLOOR_STARTLED_OLD_MAN, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToStartledOldMan) },
    { MAP_EVENT_TALK, GROUND_FLOOR_SECOND_SENTRY, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToSentry) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), GROUND_FLOOR_MAN_WONDERING_ABOUT_DONPA, CONDITION_ALWAYS, MSGNO(MsgLunpaWhoFoughtDonpa) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), GROUND_FLOOR_WOMAN_SEEKING_DODONPA, CONDITION_ALWAYS, MSGNO(MsgLunpaWifeSeeksDodonpa) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), GROUND_FLOOR_FEASTING_MAN, CONDITION_ALWAYS, MSGNO(MsgLunpaKalayBuysDrink) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), GROUND_FLOOR_BOASTING_MAN, CONDITION_ALWAYS, MSGNO(MsgLunpaHammetKeepsEarnings) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), GROUND_FLOOR_STARTLED_OLD_MAN, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ReadStartledResidentsThoughts) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), GROUND_FLOOR_SECOND_SENTRY, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ReadSentrysThoughts) },
    { MAP_EVENT_RAISED, TRIGGER_GROUND_FLOOR_PATROL_SPOTTED, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_GroundFloorPatrolThrowsPartyOut) },
    { MAP_EVENT_RAISED, TRIGGER_UPPER_HALL_SPOTTED, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_UpperHallGuardThrowsPartyOut) },
    { MAP_EVENT_RAISED, TRIGGER_ENTRANCE_GUARD_SPOTTED, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EntranceGuardThrowsPartyOut) },
    { MAP_EVENT_RAISED, TRIGGER_SENTRIES_SPOTTED, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SentriesThrowPartyOut) },
    { BG_EVENT(BG_EVENT_CHEST), 100, FLAG_LUNPA_FORTRESS_CHEST_ELIXIR, FIND_ITEM(ITEM_ELIXIR) },
    { BG_EVENT(BG_EVENT_CHEST), 101, FLAG_LUNPA_FORTRESS_CHEST_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { BG_EVENT(BG_EVENT_CHEST), 102, FLAG_LUNPA_FORTRESS_CHEST_MYTHRIL_CIRCLET, FIND_ITEM(ITEM_MYTHRIL_CIRCLET) },
    { BG_EVENT(BG_EVENT_CHEST), 103, FLAG_LUNPA_FORTRESS_CHEST_COINS, FIND_COINS(100) },
    { BG_EVENT(BG_EVENT_BARREL), 104, FLAG_LUNPA_FORTRESS_BARREL_VIAL, FIND_ITEM(ITEM_VIAL) },
    { MAP_EVENTS_END },
};

struct MapEvent gLunpaFortressUpperFloorEvents[] = {
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_DOWN, 1, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_TO_LUNPA_FORTRESS_GROUND_FLOOR_SOUTH },
    { MAP_EVENT_WARP, 2, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_WEST_PASSAGE_TO_NORTH_PASSAGE },
    { MAP_EVENT_WARP, 3, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_WEST_PASSAGE_TO_MIDDLE_PASSAGE },
    { MAP_EVENT_WARP, 4, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_NORTH_PASSAGE_TO_WEST_PASSAGE },
    { MAP_EVENT_WARP, 5, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_NORTH_PASSAGE_TO_GUARD_ROOM },
    { MAP_EVENT_WARP, 6, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_NORTH_PASSAGE_TO_WOMENS_ROOM },
    { MAP_EVENT_WARP, 7, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_MIDDLE_PASSAGE_TO_WEST_PASSAGE },
    { MAP_EVENT_WARP, 8, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_MIDDLE_PASSAGE_TO_FAMILY_ROOM },
    { MAP_EVENT_WARP, 9, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_MIDDLE_PASSAGE_TO_CELL_KEY_ROOM },
    { MAP_EVENT_WARP, 10, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_FAMILY_ROOM_TO_MIDDLE_PASSAGE },
    { MAP_EVENT_WARP, 11, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_GUARD_ROOM_TO_NORTH_PASSAGE },
    { MAP_EVENT_WARP, 12, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_GUARD_ROOM_TO_SOUTH_PASSAGE },
    { MAP_EVENT_WARP, 13, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_WOMENS_ROOM_TO_NORTH_PASSAGE },
    { MAP_EVENT_WARP, 14, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_CELL_KEY_ROOM_TO_MIDDLE_PASSAGE },
    { MAP_EVENT_WARP, 15, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_CELL_KEY_ROOM_TO_EAST_PASSAGE },
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_DOWN, 16, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_TO_LUNPA_FORTRESS_GROUND_FLOOR_NORTH },
    { MAP_EVENT_WARP, 17, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_SOUTH_PASSAGE_TO_GUARD_ROOM },
    { MAP_EVENT_WARP, 18, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_SOUTH_PASSAGE_TO_DONPAS_ROOM },
    { MAP_EVENT_WARP, 19, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_SOUTH_PASSAGE_TO_EAST_PASSAGE },
    { MAP_EVENT_WARP, 20, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_DONPAS_ROOM_TO_SOUTH_PASSAGE },
    { MAP_EVENT_WARP, 21, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_EAST_PASSAGE_TO_CELL_KEY_ROOM },
    { MAP_EVENT_WARP, 22, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_EAST_PASSAGE_TO_SOUTH_PASSAGE },
    { MAP_EVENT_WARP, 23, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_TONIC_ROOM_TO_DONPAS_ROOM },
    { MAP_EVENT_WARP, 24, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_DONPAS_ROOM_TO_TONIC_ROOM },
    { MAP_EVENT_COORD, 30, FLAG_LUNPA_FORTRESS_MIDDLE_GUARD_BEATEN, EVENT_SCRIPT(EventScript_FightHallGuard) },
    { MAP_EVENT_COORD, 31, FLAG_LUNPA_FORTRESS_THREE_GUARDS_BEATEN, EVENT_SCRIPT(EventScript_FightThreeGuards) },
    { MAP_EVENT_COORD, 32, FLAG_LUNPA_FORTRESS_EAST_GUARD_BEATEN, EVENT_SCRIPT(EventScript_FightEastGuard) },
    { MAP_EVENT_COORD, 33, FLAG_LUNPA_FORTRESS_WEST_GUARD_BEATEN, EVENT_SCRIPT(EventScript_FightWestGuard) },
    { BG_EVENT(BG_EVENT_SILENT), TRIGGER_LOCKED_DOOR, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SearchLockedDoor) },
    { COORD_EVENT_FACING(FACING_SOUTH), TRIGGER_DOOR_FIRST, UPPER_FLOOR_DOOR_FLAG, EVENT_SCRIPT(EventScript_UnlockUpperFloorDoor) },
    { COORD_EVENT_FACING(FACING_NORTH), TRIGGER_DOOR_FIRST + 1, UPPER_FLOOR_DOOR_FLAG + 1, EVENT_SCRIPT(EventScript_UnlockUpperFloorDoor) },
    { COORD_EVENT_FACING(FACING_SOUTH), TRIGGER_DOOR_FIRST + 1, UPPER_FLOOR_DOOR_FLAG + 1, EVENT_SCRIPT(EventScript_UnlockUpperFloorDoor) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_CATCH), UPPER_FLOOR_CELL_KEY, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TakeCellKey) },
    { MAP_EVENT_TALK, UPPER_FLOOR_CELL_KEY, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TakeCellKey) },
    { MAP_EVENT_TALK, UPPER_FLOOR_DODONPAS_WIFE, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_AskWhereDodonpaIs) },
    { MAP_EVENT_TALK, UPPER_FLOOR_DONPAPPA, CONDITION_ALWAYS, MSGNO(MsgLunpaChildMustGrowUp) },
    { MAP_EVENT_TALK, UPPER_FLOOR_DODONPAS_DAUGHTER, CONDITION_ALWAYS, MSGNO(MsgLunpaPapaWatchesLadies) },
    { MAP_EVENT_TALK, UPPER_FLOOR_OLD_WOMAN_PRAISING_DONPAPPA, CONDITION_ALWAYS, MSGNO(MsgLunpaChildTooResponsible) },
    { MAP_EVENT_TALK, UPPER_FLOOR_WOMAN_FEARING_FOR_DONPA, CONDITION_ALWAYS, MSGNO(MsgLunpaDonpaExhausted) },
    { MAP_EVENT_TALK, UPPER_FLOOR_CAPTIVE_WOMAN, CONDITION_ALWAYS, MSGNO(MsgLunpaWomenWantHome) },
    { MAP_EVENT_TALK, UPPER_FLOOR_GUARD_A, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToBeatenGuardA) },
    { MAP_EVENT_TALK, UPPER_FLOOR_GUARD_B, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToBeatenGuardB) },
    { MAP_EVENT_TALK, UPPER_FLOOR_GUARD_C, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToBeatenGuardC) },
    { MAP_EVENT_TALK, UPPER_FLOOR_ATTENDANT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToAttendant) },
    { MAP_EVENT_TALK, UPPER_FLOOR_DONPA, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToDonpa) },
    { MAP_EVENT_TALK, UPPER_FLOOR_TONIC, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_DjinniJoins) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), UPPER_FLOOR_DODONPAS_WIFE, CONDITION_ALWAYS, MSGNO(MsgLunpaDonpappaSuccessor) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), UPPER_FLOOR_DONPAPPA, CONDITION_ALWAYS, MSGNO(MsgLunpaDonpappaProtectsFortress) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), UPPER_FLOOR_DODONPAS_DAUGHTER, CONDITION_ALWAYS, MSGNO(MsgLunpaDaughterWillNotMarryPapa) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), UPPER_FLOOR_OLD_WOMAN_PRAISING_DONPAPPA, CONDITION_ALWAYS, MSGNO(MsgLunpaDonpappaJudges) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), UPPER_FLOOR_WOMAN_FEARING_FOR_DONPA, CONDITION_ALWAYS, MSGNO(MsgLunpaFearWithoutDonpa) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), UPPER_FLOOR_CAPTIVE_WOMAN, CONDITION_ALWAYS, MSGNO(MsgLunpaHopeDonpaRecovers) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), UPPER_FLOOR_GUARD_A, CONDITION_ALWAYS, MSGNO(MsgLunpaShouldNotHaveFought) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), UPPER_FLOOR_GUARD_B, CONDITION_ALWAYS, MSGNO(MsgLunpaHammetInCave) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), UPPER_FLOOR_GUARD_C, CONDITION_ALWAYS, MSGNO(MsgLunpaHiddenDoorSwitch) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), UPPER_FLOOR_DONPA, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ReadDonpasThoughts) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), UPPER_FLOOR_ATTENDANT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ReadAttendantsThoughts) },
    { MAP_EVENT_RAISED, TRIGGER_PATROL_PAIR_SPOTTED, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_PatrolPairThrowsPartyOut) },
    { MAP_EVENT_RAISED, TRIGGER_HALL_PATROL_SPOTTED, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_HallPatrolThrowsPartyOut) },
    { MAP_EVENTS_END },
};

struct MapEvent gLunpaFortressCaveEvents[] = {
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, LUNPA_FORTRESS_CAVE_TO_LUNPA_FORTRESS_GROUND_FLOOR },
    { MAP_EVENT_WARP, 2, CONDITION_ALWAYS, LUNPA_FORTRESS_CAVE_ENTRANCE_TO_LAKE },
    { MAP_EVENT_WARP, 3, CONDITION_ALWAYS, LUNPA_FORTRESS_CAVE_LAKE_TO_ENTRANCE },
    { MAP_EVENT_WARP, 4, CONDITION_ALWAYS, LUNPA_FORTRESS_CAVE_TO_LUNPA_FORTRESS_TOADONPA_NORTHEAST },
    { MAP_EVENT_WARP, 5, CONDITION_ALWAYS, LUNPA_FORTRESS_CAVE_TO_LUNPA_FORTRESS_TOADONPA_NORTH },
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_DOWN, 6, CONDITION_ALWAYS, LUNPA_FORTRESS_CAVE_TO_LUNPA_FORTRESS_TOADONPA_NORTHWEST },
    { BG_EVENT(BG_EVENT_SILENT), TRIGGER_LOCKED_DOOR, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SearchLockedDoor) },
    { BG_EVENT(BG_EVENT_SILENT), TRIGGER_SEALED_DOOR, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SearchSealedDoor) },
    { COORD_EVENT_FACING(FACING_NORTH), TRIGGER_DOOR_FIRST, CAVE_DOOR_FLAG, EVENT_SCRIPT(EventScript_UnlockCaveDoor) },
    { COORD_EVENT_FACING(FACING_NORTH), TRIGGER_DOOR_FIRST + 1, CAVE_DOOR_FLAG + 1, EVENT_SCRIPT(EventScript_UnlockCaveDoor) },
    { COORD_EVENT_FACING(FACING_NORTH), TRIGGER_DOOR_FIRST + 2, CAVE_DOOR_FLAG + 2, EVENT_SCRIPT(EventScript_UnlockCaveDoor) },
    { COORD_EVENT_FACING(FACING_SOUTH), TRIGGER_DOOR_FIRST + 2, CAVE_DOOR_FLAG + 2, EVENT_SCRIPT(EventScript_UnlockCaveDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_SOUTH), 50, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_CratesSink) },
    { COORD_EVENT_WALKING, 70, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_PushCrates) },
    { BG_EVENT(BG_EVENT_SILENT), 51, FLAG_MAP_TEMP_18, EVENT_SCRIPT(EventScript_FlipSecondSwitch) },
    { BG_EVENT(BG_EVENT_SILENT), 52, FLAG_MAP_TEMP_17, EVENT_SCRIPT(EventScript_FlipFirstSwitch) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ACTS), 0, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_RevealSwitches) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ENDS), 0, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_HideSwitches) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_WHIRLWIND), CAVE_FIRST_SWITCH_COVER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_BlowAwayFirstSwitchCover) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_WHIRLWIND), CAVE_PASSAGE_COVER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_BlowAwayPassageCover) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_WHIRLWIND), CAVE_SECOND_SWITCH_COVER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_BlowAwaySecondSwitchCover) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), CAVE_CRATES, FLAG_LUNPA_FORTRESS_CRATES_MOVED, EVENT_SCRIPT(EventScript_MoveCrates) },
    { MAP_EVENTS_END },
};

struct MapEvent gLunpaFortressEventsOther[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, LUNPA_FORTRESS_TOADONPA_TO_LUNPA_FORTRESS_CAVE_NORTH },
    { MAP_EVENT_WARP, 2, CONDITION_ALWAYS, LUNPA_FORTRESS_TOADONPA_TO_LUNPA_FORTRESS_CAVE_NORTHEAST },
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_UP, 5, CONDITION_ALWAYS, LUNPA_FORTRESS_TOADONPA_TO_LUNPA_FORTRESS_CAVE_NORTHWEST },
    { BG_EVENT(BG_EVENT_SILENT), TRIGGER_LOCKED_DOOR, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SearchLockedDoor) },
    { COORD_EVENT_FACING(FACING_NORTH), TRIGGER_DOOR_FIRST, TOADONPA_DOOR_FLAG, EVENT_SCRIPT(EventScript_UnlockToadonpaDoor) },
    { COORD_EVENT_FACING(FACING_NORTH), TRIGGER_DOOR_FIRST + 1, TOADONPA_DOOR_FLAG + 1, EVENT_SCRIPT(EventScript_UnlockToadonpaDoor) },
    { COORD_EVENT_FACING(FACING_NORTH), TRIGGER_DOOR_FIRST + 2, TOADONPA_DOOR_FLAG + 2, EVENT_SCRIPT(EventScript_UnlockToadonpaDoor) },
    { COORD_EVENT_FACING(FACING_NORTH), TRIGGER_DOOR_FIRST + 3, TOADONPA_DOOR_FLAG + 3, EVENT_SCRIPT(EventScript_UnlockToadonpaDoor) },
    { COORD_EVENT_FACING(FACING_NORTH), TRIGGER_DOOR_FIRST + 4, TOADONPA_DOOR_FLAG + 4, EVENT_SCRIPT(EventScript_UnlockToadonpaDoor) },
    { MAP_EVENT_COORD, 20, FLAG_MAP_TEMP_70, EVENT_SCRIPT(EventScript_FindHammet) },
    { MAP_EVENT_TALK, TOADONPA_CELL_GUARD, CONDITION_ALWAYS, MSGNO(MsgLunpaHammetStillInside) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), TOADONPA_CELL_GUARD, CONDITION_ALWAYS, MSGNO(MsgLunpaDodonpaSecretlyInside) },
    // XXX: these two rows give TOADONPA_HAMMET Dodonpa's lines from his cell,
    // though Hammet has left the cave by the time Dodonpa is jailed; whether
    // the party can reach them is not known.
    { MAP_EVENT_TALK, TOADONPA_HAMMET, CONDITION_ALWAYS, MSGNO(MsgLunpaWrongTurnOver) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), TOADONPA_HAMMET, CONDITION_ALWAYS, MSGNO(MsgLunpaCantBelieveWhen) },
    { MAP_EVENT_TALK, TOADONPA_DODONPA_STAND_IN, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToJailedDodonpa) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), TOADONPA_DODONPA_STAND_IN, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ReadJailedDodonpasThoughts) },
    { BG_EVENT(BG_EVENT_CHEST), 101, FLAG_LUNPA_FORTRESS_CHEST_POWER_BREAD, FIND_ITEM(ITEM_POWER_BREAD) },
    { MAP_EVENTS_END },
};

u32 gLunpaFortressToadonpaDoorCells[] = {
    13, 56, 8, 56, 3, 56, 28, 85,
    28, 65,
};

u32 gLunpaFortressCaveDoorCells[] = {
    31, 61, 15, 57, 6, 61,
};

u32 gLunpaFortressUpperFloorDoorCells[] = {
    16, 119, 16, 113,
};

u32 gLunpaFortressGroundFloorDoorCells[] = {
    28, 72, 43, 92,
};
// clang-format on

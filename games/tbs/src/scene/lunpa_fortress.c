/// @file
/// Lunpa Fortress (SceneId_LunpaFortressGroundFloor to
/// SceneId_LunpaFortressToadonpa), Dodonpa's fortress above Lunpa. All four
/// scenes have the game's area name Lunpa Fortress: the first two are the
/// fortress, the third and fourth the cave below it, where Hammet is held.
///
/// Where it falls in the story: Dodonpa holds Master Hammet, the merchant lord
/// of Kalay whose caravan Ivan traveled with, and keeps taking ransom from
/// Kalay without freeing him. The party slips in from the Lunpa Fortress Gates
/// (lunpa.c) under Cloak. Guards who see the uncloaked leader challenge the
/// party and send them back out (LunpaFortress_GuardThrowsPartyOut,
/// EXIT_THROWN_OUT to LUNPA_FORTRESS_GATES_AFTER_THROWN_OUT); each guard's
/// watcher callback raises a TRIGGER_*_SPOTTED trigger, and the entrance the
/// party arrives by decides which watchers run. Every return through the gates
/// resets the fortress's puzzles and beaten guards.
///
/// - SceneId_LunpaFortressGroundFloor, inside the fortress door
///   (LUNPA_FORTRESS_GROUND_FLOOR_TO_LUNPA_FORTRESS_GATES leads back out). An
///   entrance guard, two sentries, a guard to the north-east and a patrolling
///   guard keep watch. The people inside wonder where Dodonpa has gone, his men
///   boast of the money Hammet brings, and a startled old man promises to keep
///   quiet. Chests hold an Elixir, a Lucky Medal, a Mythril Circlet and 100
///   coins, and a barrel a Vial.
///   LUNPA_FORTRESS_GROUND_FLOOR_TO_LUNPA_FORTRESS_UPPER_FLOOR_SOUTH and _NORTH
///   lead to LunpaFortressUpperFloor,
///   LUNPA_FORTRESS_GROUND_FLOOR_TO_LUNPA_FORTRESS_CAVE down to
///   LunpaFortressCave.
/// - SceneId_LunpaFortressUpperFloor, where Donpa sleeps. A patrolling pair and
///   a patrol to the east keep watch. Walking up to the west, east or hall
///   guard starts a battle with a Brigand, and walking up to three guards
///   together a battle with three (FLAG_LUNPA_FORTRESS_*_GUARD_BEATEN); beaten,
///   two of them send the party down to the cave for Hammet. The Cell Key hangs
///   here (Catch or talk, FLAG_LUNPA_FORTRESS_CELL_KEY); it opens the cell
///   doors of all four scenes, each remembered in a place flag from 0x350.
///   Donpa lies asleep, worn out, beside his attendant, who tells how he
///   spoiled Dodonpa (place flag FLAG_PLACE_TEMP_13 once told); Dodonpa's
///   family and the women he brought here talk.
/// - SceneId_LunpaFortressCave, the cave, whose ways are hidden. Whirlwind
///   blows away what covers two switches and a passage
///   (FLAG_LUNPA_FORTRESS_*_PLANT_BLOWN_AWAY; FLAG_SCENE_TEMP_17 and _18 while
///   the switches are covered), the switches show in the wall only while Reveal
///   lasts, searching one then opens its passage
///   (FLAG_LUNPA_FORTRESS_*_SWITCH_FLIPPED), and Move pushes a stack of crates
///   onto its hole, where it sinks (FLAG_LUNPA_FORTRESS_CRATES_MOVED).
///   LUNPA_FORTRESS_CAVE_TO_LUNPA_FORTRESS_TOADONPA_NORTHEAST, _NORTH and
///   _NORTHWEST lead to LunpaFortressToadonpa.
/// - SceneId_LunpaFortressToadonpa, deeper in the cave, where Hammet is held in
///   a cell. A chest holds Power Bread. Walking into Hammet's cell (trigger 20,
///   Event_FindHammet, FLAG_SCENE_TEMP_70) is the rescue: Hammet knows Ivan,
///   Dodonpa arrives, gloats that Hammet is his money tree, pulls a lever and
///   frees Toadonpa on them (FLAG_LUNPA_FORTRESS_TRAP_SEEN; a retry after a
///   lost battle skips to the trap). The battle returns to
///   LUNPA_FORTRESS_TOADONPA_AFTER_BATTLE, where Event_DonpaJudgesDodonpa
///   follows: Toadonpa has fallen on Dodonpa, the party argues over his
///   punishment until his father Donpa arrives, has him locked up and asks the
///   party to leave as they came, since Dodonpa's men would free him; he has
///   spread a rumor that the intruders failed and fled. It sets
///   FLAG_DODONPA_LOCKED_UP, which changes all of Lunpa, and place flag
///   FLAG_PLACE_TEMP_01, and the party slips out with Hammet
///   (LUNPA_FORTRESS_TOADONPA_TO_LUNPA_FORTRESS_GATES, the gates'
///   LUNPA_FORTRESS_GATES_FROM_LUNPA_FORTRESS_TOADONPA). Coming back by
///   LUNPA_FORTRESS_TOADONPA_FROM_LUNPA_FORTRESS_CAVE_NORTHWEST afterwards
///   finds Dodonpa in his cell, and in LunpaFortressUpperFloor Donpa thanks the
///   party and has his attendant show them his reward (place flag
///   FLAG_PLACE_TEMP_14), Tonic, a Mercury Djinni, who joins when spoken to
///   (FLAG_LUNPA_FORTRESS_REWARD_TAKEN).
///
/// Scene flags: FLAG_SCENE_TEMP_14 and _15, a guard has spotted the party;
/// FLAG_SCENE_TEMP_24 and _25, the entrance guard or the sentries have thrown
/// it out; FLAG_SCENE_TEMP_26 and _27, the startled old man has promised to
/// keep quiet and has been startled. The overlay also holds a copy of West
/// Lunpa Cave's exits and placements, which these scenes never read (XXX:
/// why they are here is not known).

#include "game_state.h"
#include "field_event.h"
#include "unit/import.h"
#include "unit/scene/lunpa_fortress.h"
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
#include "event.h"
#include "scene_ids.h"
#include "tbs_edition.h"
#include "map_scroll.h"
#include "text/msg_ids.h"
#include "types.h"
#include "ability_ids.h"
#include "field_scene.h"
#include "item_ids.h"
#include "objcmd.h"
#include "flags.h"
#include "djinn_ids.h"
#include "songs.h"

/// LunpaFortressGroundFloor's people, numbered in the order of
/// gLunpaFortressGroundFloorPlacements from ACTOR_FIRST_PLACED, past its first
/// four rows (in a chest's sprite, at the map's origin; see
/// FIRST_ITEM_DISPLAY_ACTOR).
enum {
  /// Dodonpa's men, here and upstairs, wear the sprite of a man with a mop
  /// of orange hair (SPRITE_MAN_WITH_ORANGE_HAIR).
  GROUND_FLOOR_ENTRANCE_GUARD = ACTOR_FIRST_PLACED + 4,
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
  /// Event_TalkToStartledOldMan.
  GROUND_FLOOR_STARTLED_OLD_MAN,
  GROUND_FLOOR_SECOND_SENTRY
};

/// LunpaFortressUpperFloor's people and the Cell Key, numbered in the order of
/// gLunpaFortressUpperFloorPlacements from ACTOR_FIRST_PLACED.
enum {
  UPPER_FLOOR_CELL_KEY = ACTOR_FIRST_PLACED,
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
/// gLunpaFortressCavePlacements from ACTOR_FIRST_PLACED.
enum {
  CAVE_FIRST_SWITCH_COVER = ACTOR_FIRST_PLACED,
  CAVE_PASSAGE_COVER,
  CAVE_SECOND_SWITCH_COVER,
  /// An invisible actor at (504, 168), its sprite hidden by
  /// LunpaFortress_InitializeCave; no event names it. XXX: what it stands in
  /// for is not known.
  CAVE_INVISIBLE_ACTOR,
  /// A stack of wooden crates (the sprite of the crates at the Bilibin
  /// Barricade and on Tolbi Docks), pushed or moved with Move onto its hole.
  CAVE_CRATES
};

/// LunpaFortressToadonpa's people, numbered in the order of
/// gLunpaFortressToadonpaPlacements from ACTOR_FIRST_PLACED; its rows for Ivan,
/// Mia and Garet keep the party's own slots.
enum {
  /// Three invisible actors at (56, 72), (136, 72) and (216, 72), in line with
  /// the cell doors gLunpaFortressToadonpaDoorCells lists third, second and
  /// first. Scene_Initialize hides their sprites and lays them underfoot, and
  /// no event names them. XXX: what they stand in for is not known.
  TOADONPA_WEST_INVISIBLE_ACTOR = ACTOR_FIRST_PLACED,
  TOADONPA_MIDDLE_INVISIBLE_ACTOR,
  TOADONPA_EAST_INVISIBLE_ACTOR,
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
  /// An invisible actor that stands before Dodonpa's cell after Donpa's
  /// judgment; talking to it or reading its mind reaches Dodonpa.
  TOADONPA_DODONPA_STAND_IN
};

/// Triggers: the cell doors count from DOOR_TRIGGER_FIRST in each map, and a
/// watching guard raises one of the TRIGGER_*_SPOTTED triggers.
enum {
  TRIGGER_LOCKED_DOOR = 99,
  TRIGGER_SEALED_DOOR = 98,
  DOOR_TRIGGER_FIRST = 40,
  TRIGGER_PATROL_PAIR_SPOTTED = 91,
  TRIGGER_HALL_PATROL_SPOTTED,
  TRIGGER_GROUND_FLOOR_PATROL_SPOTTED,
  TRIGGER_UPPER_HALL_SPOTTED,
  TRIGGER_ENTRANCE_GUARD_SPOTTED,
  TRIGGER_SENTRIES_SPOTTED
};

/// Reveal's phases: the hidden things show, and hide again.

/// Where a party caught by a guard is sent: the Lunpa Fortress Gates.
enum { EXIT_THROWN_OUT = 60 };

/// What a guard close to the cloaked leader requests of Cloak.
enum { CLOAK_NEAR_GUARD = 0x2000 | ABILITY_CLOAK };

/// The first of the flags that remember which cell doors the Cell Key opened,
/// per map: one for each door trigger from DOOR_TRIGGER_FIRST.
enum {
  TOADONPA_DOOR_FLAG = 0x350,
  CAVE_DOOR_FLAG = 0x355,
  UPPER_FLOOR_DOOR_FLAG = 0x358,
  GROUND_FLOOR_DOOR_FLAG = 0x35a
};

/// The paired patrol's layer origin across: 136 pixels, in 16.16 fixed point.
// The native paired patrol computes this layer offset in floating point.
#define PAIRED_PATROL_LAYER_ORIGIN_X (136.0 * 65536)

static s32 LunpaFortress_TryCollectDisplayedItem(s32 actorId, s32 itemId);
static void LunpaFortress_CheckSecondEntranceSentry(void);
static s32 LunpaFortress_CanGuardSeeLeader(s32 actorId);
static s32 LunpaFortress_IsLeaderNearGuard(s32 actorId);
static s32 LunpaFortress_IsGuardNearViewCenter(s32 actorId);
static void LunpaFortress_GuardThrowsPartyOut(s32 actorId);
static void LunpaFortress_ActOutIntruderDialogue(s32 actorId);
static void LunpaFortress_ActOutWhosThereDialogue(s32 actorId);
static void LunpaFortress_ActOutGuardShiftDialogue(s32 actorId);
static void LunpaFortress_ActOutScoundrelDialogue(s32 actorId);
static void LunpaFortress_InitializeGroundFloor(void);
static void LunpaFortress_InitializeUpperFloor(void);
static void LunpaFortress_InitializeCave(void);
static void LunpaFortress_RestoreGroundFloorDoors(void);
static void LunpaFortress_RestoreUpperFloorDoors(void);
static void LunpaFortress_RestoreCaveDoors(void);
static void LunpaFortress_RestoreToadonpaDoors(void);

#include "common/object_staged_actor.h"

const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 sceneId = gGameState.scene;

  if (sceneId == (s32)&SceneId_LunpaFortressGroundFloor) {
    return gLunpaFortressGroundFloorEntrances;
  }
  if (sceneId == (s32)&SceneId_LunpaFortressUpperFloor) {
    return gLunpaFortressUpperFloorEntrances;
  }
  if (sceneId == (s32)&SceneId_LunpaFortressCave) {
    return gLunpaFortressCaveEntrances;
  }
  return gLunpaFortressEntrancesOther;
}

const struct SceneRegion *Scene_GetRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) {
  s16 sceneId = gGameState.scene;

  if (sceneId == (s32)&SceneId_LunpaFortressUpperFloor) {
    return gLunpaFortressUpperFloorExits;
  }
  if (sceneId == (s32)&SceneId_LunpaFortressCave ||
      sceneId == (s32)&SceneId_LunpaFortressToadonpa) {
    return gLunpaFortressCaveAndToadonpaExits;
  }
  return gLunpaFortressExitsOther;
}

/// The guards, the other people and the objects of each map.
const struct ScenePlacement *Scene_GetPlacements(void) {
  s16 sceneId = gGameState.scene;

  if (sceneId == (s32)&SceneId_WestLunpaCave) {
    return gLunpaFortressPlacementsWestLunpaCave;
  }
  if (sceneId == (s32)&SceneId_LunpaFortressCave) {
    return gLunpaFortressCavePlacements;
  }
  if (sceneId == (s32)&SceneId_LunpaFortressUpperFloor) {
    return gLunpaFortressUpperFloorPlacements;
  }
  if (sceneId == (s32)&SceneId_LunpaFortressGroundFloor) {
    return gLunpaFortressGroundFloorPlacements;
  }
  if (sceneId == (s32)&SceneId_LunpaFortressToadonpa) {
    return gLunpaFortressToadonpaPlacements;
  }
  return gLunpaFortressPlacementsOther;
}

/// What the party can talk to, search and use Psynergy on, by map.
const struct SceneEvent *Scene_GetEvents(void) {
  s16 sceneId = gGameState.scene;

  if (sceneId == (s32)&SceneId_LunpaFortressGroundFloor) {
    return gLunpaFortressGroundFloorEvents;
  }
  if (sceneId == (s32)&SceneId_LunpaFortressUpperFloor) {
    return gLunpaFortressUpperFloorEvents;
  }
  if (sceneId == (s32)&SceneId_LunpaFortressCave) {
    return gLunpaFortressCaveEvents;
  }
  return gLunpaFortressEventsOther;
}

/// LunpaFortressCave: the crates sink into the floor, and the party can walk
/// over them.
static void LunpaFortress_LowerCrates(void) {
  struct FieldActor *cratesActor;

  Map_CopyCellAttributes(15, 20, 1, 1, 15, 22);
  Map_CopyCellAttributes(17, 23, 1, 3, 15, 23);
  cratesActor = Object_GetById(CAVE_CRATES);
  if (cratesActor != 0) {
    Engine_ActorSetSpriteFlags(cratesActor, 0);
    cratesActor->motionFlags = 0;
    cratesActor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  }
}

/// LunpaFortressCave, trigger 70: pushing the crates slides them one cell
/// along.
static void Event_PushCrates(void) { StagedActor_AdvancePair(); }

/// LunpaFortressCave, trigger 50: the crates, pushed onto their hole, slide in
/// and sink.
static void Event_CratesSink(void) {
  OBJ_SPEED(CAVE_CRATES, 0x10000, 0x8000);
  Actor_SetDestination(CAVE_CRATES, 248, 376);
  Engine_ActorWaitForMove(CAVE_CRATES);
  SE(SE_215);
  WAIT(60);
  LunpaFortress_LowerCrates();
  FLAG_SET(FLAG_LUNPA_FORTRESS_CRATES_MOVED);
}

/// LunpaFortressCave, Move on the crates: once they are past row 22, over their
/// hole, the cave shakes and they sink.
static void Event_MoveCrates(void) {
  struct FieldActor *cratesActor;

  cratesActor = Actor_Get(CAVE_CRATES);
  if ((cratesActor->z.fixed >> 20) > 22) {
    Map_SetShake(0x40000, 0x40000, 0x10000);
    Map_SetShake(-1, -1, 0xe666);
    SE(SE_144);
    Map_CopyCellAttributes(15, 20, 1, 1, 15, 22);
    Map_CopyCellAttributes(17, 23, 1, 3, 15, 23);
    cratesActor = Actor_Get(CAVE_CRATES);
    if (cratesActor != NULL) {
      Engine_ActorSetSpriteFlags(cratesActor, 0);
      cratesActor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    }
    FLAG_SET(FLAG_LUNPA_FORTRESS_CRATES_MOVED);
  }
}

/// The crates stay sunk in their hole.
static void LunpaFortress_RestoreLoweredCrates(void) {
  OBJ_POS_SET(CAVE_CRATES, 248, 376);
  LunpaFortress_LowerCrates();
}

/// Draws one of LunpaFortressToadonpa's cell doors open.
static void LunpaFortress_OpenToadonpaDoor(s32 doorIndex) {
  s32 doorCellX = gLunpaFortressToadonpaDoorCells[doorIndex * 2];
  s32 doorCellY = gLunpaFortressToadonpaDoorCells[doorIndex * 2 + 1];

  Map_CopyCells(0, 0x4d, 1, 3, doorCellX, doorCellY);
  Map_CopyCells(1, 0x4d, 1, 1, doorCellX + 1, doorCellY);
  Map_CopyCells(doorCellX, doorCellY - 0x30, 1, 1, doorCellX, doorCellY - 0x2e);
}

/// LunpaFortressToadonpa: facing a cell door with the Cell Key opens it with a
/// rumble. After Donpa's judgment the fifth, Dodonpa's cell, stays shut.
static void Event_UnlockToadonpaDoor(void) {
  struct EventWork *fieldWork;
  s32 doorTrigger;
  s32 doorIndex;

  fieldWork = gEventWork;
  if (Engine_PartyInventoryFindOwner(ITEM_CELL_KEY) != -1) {
    doorTrigger = fieldWork->touchedTrigger;
    doorIndex = doorTrigger - DOOR_TRIGGER_FIRST;
    if (FLAG_CHECK(FLAG_DODONPA_LOCKED_UP) == 0 || doorIndex != 4) {
      LunpaFortress_OpenToadonpaDoor(doorIndex);
      SE(SE_GATE_MOVE);
      Map_SetShake(0x30000, 0x30000, 0x10000);
      Map_SetShake(-1, -1, 0xe666);
      FLAG_SET(doorTrigger + (TOADONPA_DOOR_FLAG - DOOR_TRIGGER_FIRST));
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
  Map_CopyCells(0, 77, 1, 3, doorCellX, doorCellY);
  Map_CopyCells(1, 77, 1, 1, doorCellX + 1, doorCellY);
  collisionCellY = doorCellY - 44;
  Map_CopyCellAttributes(doorCellX, doorCellY - 45, 1, 1, doorCellX,
                         collisionCellY);
  if (doorIndex == 1)
    Map_CopyCellAttributes(doorCellX, collisionCellY, 1, 1, doorCellX,
                           doorCellY - 43);
}

/// LunpaFortressCave: facing a cell door with the Cell Key opens it with a
/// rumble.
static void Event_UnlockCaveDoor(void) {
  struct EventWork *fieldWork;
  s32 doorTrigger;

  fieldWork = gEventWork;
  if (Engine_PartyInventoryFindOwner(ITEM_CELL_KEY) != -1) {
    doorTrigger = fieldWork->touchedTrigger;
    LunpaFortress_OpenCaveDoor(doorTrigger - DOOR_TRIGGER_FIRST);
    SE(SE_GATE_MOVE);
    Map_SetShake(0x30000, 0x30000, 0x10000);
    Map_SetShake(-1, -1, 0xe666);
    FLAG_SET(doorTrigger + (CAVE_DOOR_FLAG - DOOR_TRIGGER_FIRST));
  }
}

/// Draws one of LunpaFortressUpperFloor's cell doors open.
static void LunpaFortress_OpenUpperFloorDoor(s32 doorIndex) {
  s32 doorCellX = gLunpaFortressUpperFloorDoorCells[doorIndex * 2];
  s32 doorCellY = gLunpaFortressUpperFloorDoorCells[doorIndex * 2 + 1];

  Map_CopyCells(0x37, 0x79, 1, 3, doorCellX, doorCellY);
  Map_CopyCells(0x38, 0x79, 1, 1, doorCellX + 1, doorCellY);
  Map_CopyCells(doorCellX, doorCellY - 0x3f, 1, 1, doorCellX, doorCellY - 0x3e);
}

/// LunpaFortressUpperFloor: facing a cell door with the Cell Key opens it with
/// a rumble.
static void Event_UnlockUpperFloorDoor(void) {
  struct EventWork *fieldWork;
  s32 doorTrigger;

  fieldWork = gEventWork;
  if (Engine_PartyInventoryFindOwner(ITEM_CELL_KEY) != -1) {
    doorTrigger = fieldWork->touchedTrigger;
    LunpaFortress_OpenUpperFloorDoor(doorTrigger - DOOR_TRIGGER_FIRST);
    SE(SE_GATE_MOVE);
    Map_SetShake(0x30000, 0x30000, 0x10000);
    Map_SetShake(-1, -1, 0xe666);
    FLAG_SET(doorTrigger + (UPPER_FLOOR_DOOR_FLAG - DOOR_TRIGGER_FIRST));
  }
}

/// Draws one of LunpaFortressGroundFloor's cell doors open and lets the party
/// through.
static void LunpaFortress_OpenGroundFloorDoor(s32 doorIndex) {
  // BUG: the third door's trigger and flag (GROUND_FLOOR_DOOR_FLAG + 2) ask for
  // index 2, past the two doors gLunpaFortressGroundFloorDoorCells holds.
  s32 doorCellX = gLunpaFortressGroundFloorDoorCells[doorIndex * 2];
  s32 doorCellY = gLunpaFortressGroundFloorDoorCells[doorIndex * 2 + 1];

  Map_CopyCells(1, 0x50, 1, 3, doorCellX, doorCellY);
  Map_CopyCells(2, 0x50, 1, 1, doorCellX + 1, doorCellY);
  Engine_MapCopyCellAttributes(doorCellX, doorCellY - 0x3f, 1, 1, doorCellX,
                               doorCellY - 0x3e);
}

/// LunpaFortressGroundFloor: facing a cell door with the Cell Key opens it with
/// a rumble.
static void Event_UnlockGroundFloorDoor(void) {
  struct EventWork *fieldWork;
  s32 doorTrigger;

  fieldWork = gEventWork;
  if (Engine_PartyInventoryFindOwner(ITEM_CELL_KEY) != -1) {
    doorTrigger = fieldWork->touchedTrigger;
    LunpaFortress_OpenGroundFloorDoor(doorTrigger - DOOR_TRIGGER_FIRST);
    SE(SE_GATE_MOVE);
    Map_SetShake(0x30000, 0x30000, 0x10000);
    Map_SetShake(-1, -1, 0xe666);
    FLAG_SET(doorTrigger + (GROUND_FLOOR_DOOR_FLAG - DOOR_TRIGGER_FIRST));
  }
}

/// Lunpa Fortress, every scene: searching a cell door without the Cell Key,
/// it stays shut.
/// "The door is tightly locked."
static void Event_SearchLockedDoor(void) {
  if (Engine_PartyInventoryFindOwner(ITEM_CELL_KEY) == -1) {
    Engine_MessageShowCentered(MsgFieldDoorTightlyLocked, 1);
  }
}

/// LunpaFortressCave: searching the door no key opens.
static void Event_SearchSealedDoor(void) {
  Engine_MessageShowCentered(MsgFieldDoorTightlyLocked, 1);
}

/// The four actors from FIRST_ITEM_DISPLAY_ACTOR that the unused item pickups
/// below show their items on, in a scene they do not name. XXX: likely
/// LunpaFortressGroundFloor's first four rows (in a chest's sprite, at the
/// map's origin), because nothing else uses them.
enum { FIRST_ITEM_DISPLAY_ACTOR = ACTOR_FIRST_PLACED };

/// Unused: an Arctic Blade, a Masamune, a Silver Blade and a Shamshir set out
/// on the four item display actors, each taken once, with
/// FLAG_LUNPA_FORTRESS_ARCTIC_BLADE to FLAG_LUNPA_FORTRESS_SHAMSHIR.
static void UNUSED LunpaFortress_TryCollectArcticBlade(void) {
  if (LunpaFortress_TryCollectDisplayedItem(FIRST_ITEM_DISPLAY_ACTOR,
                                            ITEM_ARCTIC_BLADE) != 0) {
    Engine_GameFlagSet(FLAG_LUNPA_FORTRESS_ARCTIC_BLADE);
  }
}

static void UNUSED LunpaFortress_TryCollectMasamune(void) {
  if (LunpaFortress_TryCollectDisplayedItem(FIRST_ITEM_DISPLAY_ACTOR + 1,
                                            ITEM_MASAMUNE) != 0) {
    Engine_GameFlagSet(FLAG_LUNPA_FORTRESS_MASAMUNE);
  }
}

static void UNUSED LunpaFortress_TryCollectSilverBlade(void) {
  if (LunpaFortress_TryCollectDisplayedItem(FIRST_ITEM_DISPLAY_ACTOR + 2,
                                            ITEM_SILVER_BLADE) != 0) {
    Engine_GameFlagSet(FLAG_LUNPA_FORTRESS_SILVER_BLADE);
  }
}

static void UNUSED LunpaFortress_TryCollectShamshir(void) {
  if (LunpaFortress_TryCollectDisplayedItem(FIRST_ITEM_DISPLAY_ACTOR + 3,
                                            ITEM_SHAMSHIR) != 0) {
    Engine_GameFlagSet(FLAG_LUNPA_FORTRESS_SHAMSHIR);
  }
}

/// Gives the item an actor shows, with its effect; with no room for it, the
/// actor shows the item is left behind. Returns whether it was given.
static s32 LunpaFortress_TryCollectDisplayedItem(s32 actorId, s32 itemId) {
  s32 itemGiven = 0;
  s32 itemEffect;

  EV_BEGIN();
  itemEffect = Engine_FieldFxPlayCueAndStartEmitterOnTarget(0, actorId, itemId);
  if (Engine_PartyGiveItem(itemId, 0) != -1) {
    OBJ_MODE(actorId, 2);
    itemGiven = 1;
  } else {
    SE(SE_125);
    OBJ_MODE(actorId, 5);
  }
  Engine_ObjectDispatchRelease((struct FieldActor *)itemEffect);
  EV_END();
  return itemGiven;
}

static void UNUSED LunpaFortress_DoNothingA(void) {}

static void UNUSED LunpaFortress_DoNothingB(void) {}

static void UNUSED LunpaFortress_DoNothingC(void) {}

static void UNUSED LunpaFortress_DoNothingD(void) {}

/// LunpaFortressUpperFloor: the leader steps up and takes the Cell Key. Catch
/// on the key hanging there, or talking to it, starts this.
static void Event_TakeCellKey(void) {
  Engine_ActorWalkTo(ACTOR_PARTY_LEADER, 264, 792);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  WAIT(10);
  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_STAND);
  Engine_ItemShowFound(ITEM_CELL_KEY, 3);
  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_STAND);
  Engine_PartyGiveItem(ITEM_CELL_KEY, 0);
  Engine_GameFlagSet(FLAG_LUNPA_FORTRESS_CELL_KEY);
  Engine_ActorSetPosition(UPPER_FLOOR_CELL_KEY, 0, 0);
}
/// Where the patrolling pair cannot see the leader: the west end of their
/// corridor and the cells below its gap.
static s32 LunpaFortress_IsLeaderInPatrolSafeZone(void) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
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

// UB: these patrols formerly reached gEventWork through gMapWork[19] and
// treated its request members as a halfword array. Named owner access adds
// eight bytes to each of the three patrol callbacks in the EN trial.
#ifdef UBFIX
#define PATROL_PSYNERGY_REQUEST(work) ((work)->command)
#define PATROL_ITEM_REQUEST(work) ((work)->item)
#define PATROL_RAISED_TRIGGER(work) ((work)->event)
#else
#define PATROL_PSYNERGY_REQUEST(work) ((work)[191])
#define PATROL_ITEM_REQUEST(work) ((work)[192])
#define PATROL_RAISED_TRIGGER(work) ((work)[193])
#endif

/// Every frame in LunpaFortressUpperFloor: the pair patrolling the west
/// corridor. They stand still while FLAG_MENU_OPEN is set or Psynergy or an
/// item is in use; otherwise they walk, with MAP_LAYER_OVERHEAD shaking and
/// following the first. Close to a cloaked leader they request
/// CLOAK_NEAR_GUARD; seeing an uncloaked leader, they raise
/// TRIGGER_PATROL_PAIR_SPOTTED.
static void LunpaFortress_UpdatePairedGuardDetection(void) {
  struct FieldActor *firstGuard = Actor_Get(UPPER_FLOOR_FIRST_PATROL);
  struct FieldActor *secondGuard = Actor_Get(UPPER_FLOOR_SECOND_PATROL);
  struct MapLayerScroll *patrolLayer =
      &((struct MapScrollWork *)gMapWork[0])->layers[MAP_LAYER_OVERHEAD];
#ifdef UBFIX
  struct FieldWork *fieldWork = (struct FieldWork *)gEventWork;
#else
  s16 *fieldWork = *(s16 **)&gMapWork[19];
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
  } else if (!FLAG_CHECK(FLAG_SCENE_TEMP_14)) {
    firstGuard->paused = 0;
    secondGuard->paused = 0;
    if (!FLAG_CHECK(FLAG_SCENE_TEMP_14) && firstGuard->paused == 0) {
      patrolLayer->phaseX = PAIRED_PATROL_LAYER_ORIGIN_X - firstGuard->x.fixed;
    }
    if (!LunpaFortress_IsLeaderInPatrolSafeZone()) {
      if (gGameState.cloaked != 0) {
        if (LunpaFortress_IsLeaderNearGuard(UPPER_FLOOR_FIRST_PATROL) &&
            gGameState.cloaked != 0) {
          SetSceneValue(&PATROL_PSYNERGY_REQUEST(fieldWork), CLOAK_NEAR_GUARD);
          return;
        }
        if (LunpaFortress_IsLeaderNearGuard(UPPER_FLOOR_SECOND_PATROL) &&
            gGameState.cloaked != 0) {
          SetSceneValue(&PATROL_PSYNERGY_REQUEST(fieldWork), CLOAK_NEAR_GUARD);
          return;
        }
      }
      if (gGameState.cloaked == 0) {
        if (LunpaFortress_CanGuardSeeLeader(UPPER_FLOOR_FIRST_PATROL)) {
          FLAG_SET(FLAG_SCENE_TEMP_15);
          FLAG_SET(FLAG_SCENE_TEMP_14);
        }
        if (LunpaFortress_CanGuardSeeLeader(UPPER_FLOOR_SECOND_PATROL)) {
          FLAG_SET(FLAG_SCENE_TEMP_15);
          FLAG_SET(FLAG_SCENE_TEMP_14);
        }
      }
      if (FLAG_CHECK(FLAG_SCENE_TEMP_14)) {
        SetSceneValue(&PATROL_RAISED_TRIGGER(fieldWork),
                      TRIGGER_PATROL_PAIR_SPOTTED);
      }
    }
  }
}

/// LunpaFortressUpperFloor, raised when the patrolling pair spots the leader:
/// the first stops and stares, and the second throws the party out.
static void Event_PatrolPairThrowsPartyOut(void) {
  EV_BEGIN();
  Engine_ActorStop(UPPER_FLOOR_FIRST_PATROL);
  Engine_ActorSetDestinationOffset(UPPER_FLOOR_FIRST_PATROL, 0, 0);
  OBJ_MODE(UPPER_FLOOR_FIRST_PATROL, 0);
  OBJ_FACE(UPPER_FLOOR_FIRST_PATROL, ACTOR_PARTY_LEADER, 0);
  Engine_ActorShowEmote(UPPER_FLOOR_FIRST_PATROL, EMOTE_IN_FRONT, 0);
  LunpaFortress_GuardThrowsPartyOut(UPPER_FLOOR_SECOND_PATROL);
  EV_END();
}

/// Whether the leader and the hall patrol are in the two parallel corridors,
/// where the guard passes without seeing the leader.
static s32 LunpaFortress_IsLeaderPassingPatrollingGuard(void) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  struct FieldActor *guard = Object_GetById(UPPER_FLOOR_HALL_PATROL);
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

  struct FieldActor *guard = Actor_Get(UPPER_FLOOR_HALL_PATROL);
  struct MapLayerScroll *patrolLayer =
      &((struct MapScrollWork *)gMapWork[0])->layers[MAP_LAYER_OVERHEAD];
#ifdef UBFIX
  struct FieldWork *fieldWork = (struct FieldWork *)gEventWork;
#else
  s16 *fieldWork = *(s16 **)&gMapWork[19];
#endif

  Engine_EventGetViewCenter();
  if (gFrameCount & 1) {
    patrolLayer->speedX = 1;
    patrolLayer->speedY = 1;
  } else {
    patrolLayer->speedX = -1;
    patrolLayer->speedY = -1;
  }
  if (FLAG_CHECK(FLAG_MENU_OPEN) || PATROL_PSYNERGY_REQUEST(fieldWork) != 0 ||
      PATROL_ITEM_REQUEST(fieldWork) != 0) {
    guard->paused = 1;
  } else if (!FLAG_CHECK(FLAG_SCENE_TEMP_14)) {
    guard->paused = 0;
    if (!FLAG_CHECK(FLAG_SCENE_TEMP_14) && guard->paused == 0) {
      patrolLayer->phaseX = 0x3400000 - guard->x.fixed;
      patrolLayer->phaseY = 0x2400000 - guard->z.fixed;
    }
    if (!LunpaFortress_IsLeaderPassingPatrollingGuard()) {
      LunpaFortress_IsGuardNearViewCenter(UPPER_FLOOR_HALL_PATROL);
      if (LunpaFortress_IsLeaderNearGuard(UPPER_FLOOR_HALL_PATROL) &&
          gGameState.cloaked != 0) {
        SetSceneValue(&PATROL_PSYNERGY_REQUEST(fieldWork), 0x2092);
        return;
      }
      if (gGameState.cloaked == 0) {
        if (LunpaFortress_CanGuardSeeLeader(UPPER_FLOOR_HALL_PATROL)) {
          FLAG_SET(FLAG_SCENE_TEMP_15);
          FLAG_SET(FLAG_SCENE_TEMP_14);
        }
      }
      if (FLAG_CHECK(FLAG_SCENE_TEMP_14)) {
        SetSceneValue(&PATROL_RAISED_TRIGGER(fieldWork),
                      TRIGGER_HALL_PATROL_SPOTTED);
      }
    }
  }
}

/// LunpaFortressUpperFloor, raised when the hall patrol spots the leader: it
/// throws the party out.
static void Event_HallPatrolThrowsPartyOut(void) {
  LunpaFortress_GuardThrowsPartyOut(UPPER_FLOOR_HALL_PATROL);
  EV_END();
}

/// Where LunpaFortressGroundFloor's patrol cannot see the leader: three
/// stretches of its rounds.
static s32 LunpaFortress_IsLeaderInGroundFloorPatrolSafeZone(void) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  s32 leaderTileZ = leader->z.fixed / 0x100000;
  s32 leaderTileX = leader->x.fixed / 0x100000;

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

  struct FieldActor *guard = Actor_Get(GROUND_FLOOR_PATROL_GUARD);
  struct MapLayerScroll *patrolLayer =
      &((struct MapScrollWork *)gMapWork[0])->layers[MAP_LAYER_OVERHEAD];
#ifdef UBFIX
  struct FieldWork *fieldWork = (struct FieldWork *)gEventWork;
#else
  s16 *fieldWork = *(s16 **)&gMapWork[19];
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
    guard->paused = 1;
  } else if (!FLAG_CHECK(FLAG_SCENE_TEMP_14)) {
    guard->paused = 0;
    if (!FLAG_CHECK(FLAG_SCENE_TEMP_14) && guard->paused == 0) {
      patrolLayer->phaseX = 0x2f00000 - guard->x.fixed;
      patrolLayer->phaseY = 0x1f00000 - guard->z.fixed;
    }
    if (!LunpaFortress_IsLeaderInGroundFloorPatrolSafeZone()) {
      if (LunpaFortress_IsLeaderNearGuard(GROUND_FLOOR_PATROL_GUARD) &&
          gGameState.cloaked != 0) {
        SetSceneValue(&PATROL_PSYNERGY_REQUEST(fieldWork), 0x2092);
        return;
      }
      if (gGameState.cloaked == 0) {
        if (LunpaFortress_CanGuardSeeLeader(GROUND_FLOOR_PATROL_GUARD)) {
          FLAG_SET(FLAG_SCENE_TEMP_15);
          FLAG_SET(FLAG_SCENE_TEMP_14);
        }
      }
      if (FLAG_CHECK(FLAG_SCENE_TEMP_14)) {
        SetSceneValue(&PATROL_RAISED_TRIGGER(fieldWork),
                      TRIGGER_GROUND_FLOOR_PATROL_SPOTTED);
      }
    }
  }
}

/// LunpaFortressGroundFloor, raised when the patrol spots the leader: it throws
/// the party out.
static void Event_GroundFloorPatrolThrowsPartyOut(void) {
  LunpaFortress_GuardThrowsPartyOut(GROUND_FLOOR_PATROL_GUARD);
  EV_END();
}

/// Whether the leader is outside the upper hall the guard there watches.
static s32 LunpaFortress_IsLeaderOutsideUpperHall(void) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  s32 leaderTileZ = leader->z.fixed / 0x100000;
  s32 leaderTileX = leader->x.fixed / 0x100000;

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

  fieldWork = gEventWork;
  if (FLAG_CHECK(FLAG_SCENE_TEMP_14) == 0) {
    if (LunpaFortress_IsLeaderOutsideUpperHall() == 0) {
      if (gGameState.cloaked == 0) {
        if (LunpaFortress_CanGuardSeeLeader(GROUND_FLOOR_UPPER_HALL_GUARD) !=
            0) {
          FLAG_SET(FLAG_SCENE_TEMP_15);
          FLAG_SET(FLAG_SCENE_TEMP_14);
        }
      }
      if (FLAG_CHECK(FLAG_SCENE_TEMP_14) != 0) {
        fieldWork->raisedTrigger = TRIGGER_UPPER_HALL_SPOTTED;
      }
    }
  }
}

/// LunpaFortressGroundFloor, raised when the upper hall's guard spots the
/// leader: he throws the party out.
static void Event_UpperHallGuardThrowsPartyOut(void) {
  LunpaFortress_GuardThrowsPartyOut(GROUND_FLOOR_UPPER_HALL_GUARD);
  EV_END();
}

/// Every frame past LunpaFortressGroundFloor's entrance: the guard who sees the
/// uncloaked leader raises TRIGGER_ENTRANCE_GUARD_SPOTTED, once.
static void LunpaFortress_CheckEntranceGuard(void) {

  struct EventWork *fieldWork = gEventWork;

  if (LunpaFortress_CanGuardSeeLeader(GROUND_FLOOR_ENTRANCE_GUARD) != 0 &&
      gGameState.cloaked == 0) {
    s16 *raisedTrigger;
    s32 caughtTrigger;

    Engine_TaskRemoveCallback(LunpaFortress_CheckEntranceGuard);
    raisedTrigger = &fieldWork->raisedTrigger;
    caughtTrigger = TRIGGER_ENTRANCE_GUARD_SPOTTED;
    *raisedTrigger = caughtTrigger;
  }
}

/// Every frame past LunpaFortressGroundFloor's entrance: either sentry who sees
/// the uncloaked leader raises TRIGGER_SENTRIES_SPOTTED, once for both.
static void LunpaFortress_CheckFirstEntranceSentry(void) {

  struct EventWork *fieldWork;

  fieldWork = gEventWork;
  if (FLAG_CHECK(FLAG_SCENE_TEMP_25) == 0) {
    if (LunpaFortress_CanGuardSeeLeader(GROUND_FLOOR_FIRST_SENTRY) != 0) {
      if (gGameState.cloaked == 0) {
        FLAG_SET(FLAG_SCENE_TEMP_25);
        Engine_TaskRemoveCallback(LunpaFortress_CheckFirstEntranceSentry);
        Engine_TaskRemoveCallback(LunpaFortress_CheckSecondEntranceSentry);
        fieldWork->raisedTrigger = TRIGGER_SENTRIES_SPOTTED;
      }
    }
  }
}

/// The second sentry, as the first.
static void LunpaFortress_CheckSecondEntranceSentry(void) {

  struct EventWork *fieldWork;

  fieldWork = gEventWork;
  if (FLAG_CHECK(FLAG_SCENE_TEMP_25) == 0) {
    if (LunpaFortress_CanGuardSeeLeader(GROUND_FLOOR_SECOND_SENTRY) != 0) {
      if (gGameState.cloaked == 0) {
        FLAG_SET(FLAG_SCENE_TEMP_25);
        Engine_TaskRemoveCallback(LunpaFortress_CheckSecondEntranceSentry);
        Engine_TaskRemoveCallback(LunpaFortress_CheckFirstEntranceSentry);
        fieldWork->raisedTrigger = TRIGGER_SENTRIES_SPOTTED;
      }
    }
  }
}

/// Whether the leader is in the guard's column, up to six cells away.
static s32 LunpaFortress_IsLeaderInGuardVerticalSight(s32 actorId) {
  struct FieldActor *guard = Object_GetById(actorId);
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  s32 guardTileZ = guard->z.fixed / 0x100000;
  s32 guardTileX = guard->x.fixed / 0x100000;
  s32 leaderTileZ = leader->z.fixed / 0x100000;
  s32 leaderTileX = leader->x.fixed / 0x100000;
  s32 zDistance = guardTileZ - leaderTileZ;

  if (zDistance >= -6 && zDistance <= 6 && guardTileX - 1 < leaderTileX &&
      guardTileX + 1 > leaderTileX) {
    return 1;
  }
  return 0;
}

/// Whether the leader is within a row of the guard, up to six cells away.
static s32 LunpaFortress_IsLeaderInGuardHorizontalSight(s32 actorId) {
  struct FieldActor *guard = Object_GetById(actorId);
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  s32 guardTileZ = guard->z.fixed / 0x100000;
  s32 guardTileX = guard->x.fixed / 0x100000;
  s32 leaderTileZ = leader->z.fixed / 0x100000;
  s32 leaderTileX = leader->x.fixed / 0x100000;
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
static s32 LunpaFortress_CanGuardSeeLeader(s32 actorId) {
  if (LunpaFortress_IsGuardNearViewCenter(actorId) == 0) {
    return 0;
  }
  if (LunpaFortress_IsLeaderInGuardVerticalSight(actorId) != 0) {
    return 1;
  }
  {
    s32 inHorizontalSight =
        LunpaFortress_IsLeaderInGuardHorizontalSight(actorId);

    return (u32)(inHorizontalSight | -inHorizontalSight) >> 31;
  }
}

/// Whether the leader is within four cells, walking, of the cell below the
/// guard.
static s32 LunpaFortress_IsLeaderNearGuard(s32 actorId) {
  struct FieldActor *guard = Object_GetById(actorId);
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  s32 guardTileZ = guard->z.fixed / 0x100000;
  s32 guardTileX = guard->x.fixed / 0x100000;
  s32 leaderTileZ = leader->z.fixed / 0x100000;
  s32 leaderTileX = leader->x.fixed / 0x100000;
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
static s32 LunpaFortress_IsGuardNearViewCenter(s32 actorId) {
  struct FieldActor *guard = Object_GetById(actorId);
  struct FieldActor *viewCenter = Engine_EventGetViewCenter();
  s32 guardTileX = guard->x.fixed / 0x100000;
  s32 guardTileZ = guard->z.fixed / 0x100000;
  s32 viewTileX = viewCenter->x.fixed / 0x100000;
  s32 viewTileZ = viewCenter->z.fixed / 0x100000;
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

  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);

  if (Engine_GameFlagIsSet(GROUND_FLOOR_DOOR_FLAG + 1) == 0) {
    s32 playerX = leader->x.fixed / 0x100000;
    s32 playerZ = leader->z.fixed / 0x100000;

    if (playerX == 43 && playerZ > 28 && playerZ <= 31) {
      s16 *touchedTrigger = &gEventWork->touchedTrigger;
      s32 doorTrigger = DOOR_TRIGGER_FIRST + 1;

      *touchedTrigger = doorTrigger;
      Event_UnlockGroundFloorDoor();
    }
  }
}

/// Every frame in LunpaFortressUpperFloor: walking east or west into the first
/// cell door's column with the Cell Key unlocks it.
static void LunpaFortress_CheckUpperFloorDoorApproach(void) {

  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);

  if (Engine_GameFlagIsSet(UPPER_FLOOR_DOOR_FLAG) == 0) {
    s32 playerX = leader->x.fixed / 0x100000;
    s32 playerZ = leader->z.fixed / 0x100000;

    if (playerX == 16 && playerZ > 55 && playerZ <= 58 &&
        (leader->facing == 0xc000 || leader->facing == 0x4000)) {
      s16 *touchedTrigger = &gEventWork->touchedTrigger;
      s32 doorTrigger = DOOR_TRIGGER_FIRST;

      *touchedTrigger = doorTrigger;
      Event_UnlockUpperFloorDoor();
    }
  }
}

/// Unused: the first patrol asks who the party is and sends them out.
static void UNUSED LunpaFortress_FirstPatrolThrowsPartyOut(void) {
  EV_BEGIN();
  Engine_ActorSetDestinationOffset(UPPER_FLOOR_FIRST_PATROL, 0, 0);
  Actor_SetBuiltinAction(UPPER_FLOOR_FIRST_PATROL, ACTOR_ACTION_IDLE);
  Engine_ActorStop(UPPER_FLOOR_FIRST_PATROL);
  OBJ_MODE(UPPER_FLOOR_FIRST_PATROL, 0);
  Actor_SetBuiltinAction(ACTOR_PARTY_LEADER, ACTOR_ACTION_IDLE);
  {
    s32 challengeMessage = MsgLunpaWho2;

    MESSAGE_SET(challengeMessage);
    MESSAGE_PUT(UPPER_FLOOR_FIRST_PATROL, 0);
    Engine_ActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 60);
    MESSAGE_SET(challengeMessage + 1);
  }
  MESSAGE_PUT(UPPER_FLOOR_FIRST_PATROL, 0);
  Engine_EventRequestExit(EXIT_THROWN_OUT);
  Engine_EventCloseScreen();
  EV_END();
}

/// A guard who caught the party sends it back to the Lunpa Fortress Gates.
/// "Who are you? How did you get in here?"
static void LunpaFortress_GuardThrowsPartyOut(s32 actorId) {
  EV_BEGIN();
  EV_BEGIN();
  Engine_ActorShowEmote(actorId, EMOTE_IN_FRONT, 1);
  Engine_ActorSetDestinationOffset(actorId, 0, 0);
  Actor_SetBuiltinAction(actorId, ACTOR_ACTION_IDLE);
  OBJ_MODE(actorId, 0);
  OBJ_FACE(actorId, ACTOR_PARTY_LEADER, 0);
  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_STAND);
  Engine_ActorSetDestinationOffset(actorId, 0, 0);
  Actor_SetBuiltinAction(actorId, ACTOR_ACTION_IDLE);
  Engine_ActorStop(actorId);
  OBJ_MODE(actorId, 0);
  Actor_SetBuiltinAction(ACTOR_PARTY_LEADER, ACTOR_ACTION_IDLE);
  {
    s32 challengeMessage = MsgLunpaWho2;

    MESSAGE_SET(challengeMessage);
    MESSAGE_PUT(actorId, 0);
    OBJ_FACE(ACTOR_PARTY_LEADER, actorId, 0);
    Engine_ActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 60);
    MESSAGE_SET(challengeMessage + 1);
  }
  MESSAGE_PUT(actorId, 0);
  Engine_EventCloseScreen();
  WAIT(60);
  Engine_EventRequestExit(EXIT_THROWN_OUT);
  EV_END();
}

/// A guard faces the leader with one of four challenges, chosen at random
/// when the map was entered.
static void LunpaFortress_GuardChallengesParty(s32 actorId) {

  OBJ_FACE(actorId, ACTOR_PARTY_LEADER, 0);
  OBJ_FACE(ACTOR_PARTY_LEADER, actorId, 0);
  switch (gLunpaFortressRandomPick & 3) {
  case 0:
    LunpaFortress_ActOutIntruderDialogue(actorId);
    break;
  case 1:
    LunpaFortress_ActOutWhosThereDialogue(actorId);
    break;
  case 2:
    LunpaFortress_ActOutGuardShiftDialogue(actorId);
    break;
  case 3:
    LunpaFortress_ActOutScoundrelDialogue(actorId);
    break;
  default:
    LunpaFortress_ActOutWhosThereDialogue(actorId);
    break;
  }
}

/// A guard spots an intruder. "Hey! Intruder!"
static void LunpaFortress_ActOutIntruderDialogue(s32 actorId) {
  s32 messageBase;

  Engine_ActorRunRepeatedMotion(actorId, 1);
  messageBase = MsgLunpaIntruder;
  MESSAGE_SET(messageBase);
  MESSAGE_PUT(actorId, 0);
  OBJ_EMOTE(actorId, EMOTE_IN_FRONT | 2, 60);
  MESSAGE_SET(messageBase + 1);
  MESSAGE_PUT(actorId, 0);
  messageBase += 2;
  Engine_ActorSetAnimationAndWait(actorId, 4);
  MESSAGE_SET(messageBase);
  MESSAGE_PUT(actorId, 0);
}

/// A guard asks who is there. "Huh? Who's that?"
static void LunpaFortress_ActOutWhosThereDialogue(s32 actorId) {
  s32 messageBase = MsgLunpaGuardWhosThat;

  MESSAGE_SET(messageBase);
  MESSAGE_PUT(actorId, 0);
  Engine_ActorRunRepeatedMotion(actorId, 1);
  MESSAGE_SET(messageBase + 1);
  MESSAGE_PUT(actorId, 0);
  Engine_ActorSetAnimationAndWait(actorId, 4);
  MESSAGE_SET(messageBase + 2);
  MESSAGE_PUT(actorId, 0);
}

/// A guard expects his relief, then sees who it is. "Is my shift up already?"
static void LunpaFortress_ActOutGuardShiftDialogue(s32 actorId) {
  s32 messageBase;

  messageBase = MsgLunpaShiftAlready;
  MESSAGE_SET(messageBase);
  MESSAGE_PUT(actorId, 0);
  WAIT(120);
  OBJ_EMOTE(actorId, EMOTE_IN_FRONT | 1, 60);
  MESSAGE_SET((messageBase + 1));
  MESSAGE_PUT(actorId, 0);
  Engine_ActorRunRepeatedMotion(actorId, 1);
  MESSAGE_SET((messageBase + 2));
  MESSAGE_PUT(actorId, 0);
  Engine_ActorSetAnimationAndWait(actorId, 4);
  MESSAGE_SET((messageBase + 3));
  MESSAGE_PUT(actorId, 0);
}

/// A guard calls the party a scoundrel. "You scoundrel!"
static void LunpaFortress_ActOutScoundrelDialogue(s32 actorId) {
  s32 messageBase = MsgLunpaScoundrel;

  MESSAGE_SET(messageBase);
  MESSAGE_PUT(actorId, 0);
  Engine_ActorSetAnimationAndWait(actorId, 4);
  MESSAGE_SET(messageBase + 1);
  MESSAGE_PUT(actorId, 0);
  Engine_ActorRunRepeatedMotion(actorId, 1);
  MESSAGE_SET(messageBase + 2);
  MESSAGE_PUT(actorId, 0);
  Engine_ActorSetAnimationAndWait(actorId, 3);
  MESSAGE_SET(messageBase + 3);
  MESSAGE_PUT(actorId, 0);
}

/// LunpaFortressUpperFloor, trigger 33: walking up to the west guard, he
/// challenges the party to a battle with a Brigand, and is gone after it.
static void Event_FightWestGuard(void) {

  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_STAND);
  SE(SE_MENU_CANCEL);
  Engine_ActorShowEmote(UPPER_FLOOR_WEST_GUARD, EMOTE_IN_FRONT, 60);
  LunpaFortress_GuardChallengesParty(UPPER_FLOOR_WEST_GUARD);
  gEventWork->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
  Battle_SetStart(BATTLE_START_SCENE);
  Engine_EncounterRequestBattle(ENCOUNTER_ZONE_SCRIPTED, 2);
  Engine_ActorSetPosition(UPPER_FLOOR_WEST_GUARD, 0, 0);
  EV_END();
  Engine_GameFlagSet(FLAG_LUNPA_FORTRESS_WEST_GUARD_BEATEN);
}

/// LunpaFortressUpperFloor, trigger 30: walking up to the hall guard, he
/// challenges the party to a battle with a Brigand, and is gone after it.
static void Event_FightHallGuard(void) {

  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_STAND);
  SE(SE_MENU_CANCEL);
  Engine_ActorShowEmote(UPPER_FLOOR_HALL_GUARD, EMOTE_IN_FRONT, 60);
  LunpaFortress_GuardChallengesParty(UPPER_FLOOR_HALL_GUARD);
  gEventWork->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
  Battle_SetStart(BATTLE_START_SCENE);
  Engine_EncounterRequestBattle(ENCOUNTER_ZONE_SCRIPTED, 2);
  Engine_ActorSetPosition(UPPER_FLOOR_HALL_GUARD, 0, 0);
  EV_END();
  Engine_GameFlagSet(FLAG_LUNPA_FORTRESS_MIDDLE_GUARD_BEATEN);
}

/// LunpaFortressUpperFloor, trigger 31: walking up to the three guards, they
/// argue, then fight the party together as three Brigands. "Who are you!?"
/// After the battle the party comes back here.
static void Event_FightThreeGuards(void) {
  s32 messageBase;

  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_STAND);
  OBJ_MODE(UPPER_FLOOR_GUARD_A, 1);
  OBJ_MODE(UPPER_FLOOR_GUARD_B, 1);
  OBJ_MODE(UPPER_FLOOR_GUARD_C, 1);
  SE(SE_MENU_CANCEL);
  OBJ_EMOTE(UPPER_FLOOR_GUARD_A, EMOTE_IN_FRONT, 0);
  WAIT(30);
  OBJ_FACE(UPPER_FLOOR_GUARD_A, ACTOR_PARTY_LEADER, 0);
  messageBase = MsgLunpaWho3;
  MESSAGE_SET(messageBase);
  MESSAGE_PUT(UPPER_FLOOR_GUARD_A, 0);
  OBJ_EMOTE(UPPER_FLOOR_GUARD_B, EMOTE_IN_FRONT, 0);
  OBJ_EMOTE(UPPER_FLOOR_GUARD_C, EMOTE_IN_FRONT, 0);
  OBJ_FACE(UPPER_FLOOR_GUARD_B, ACTOR_PARTY_LEADER, 0);
  OBJ_FACE(UPPER_FLOOR_GUARD_C, ACTOR_PARTY_LEADER, 0);
  OBJ_FACE(ACTOR_PARTY_LEADER, UPPER_FLOOR_GUARD_B, 0);
  WAIT(65);
  OBJ_DIR(UPPER_FLOOR_GUARD_B, FACING_SOUTH + FACING_STEP, 0);
  OBJ_DIR(UPPER_FLOOR_GUARD_C, FACING_NORTH + FACING_STEP, 0);
  MESSAGE_SET((messageBase + 1));
  MESSAGE_PUT(UPPER_FLOOR_GUARD_B, 0);
  Engine_ActorSetAnimationAndWait(UPPER_FLOOR_GUARD_C, 3);
  MESSAGE_SET((messageBase + 2));
  MESSAGE_PUT(UPPER_FLOOR_GUARD_C, 0);
  MESSAGE_SET((messageBase + 3));
  MESSAGE_PUT(UPPER_FLOOR_GUARD_A, 0);
  Engine_ActorRunRepeatedMotion(UPPER_FLOOR_GUARD_B, 1);
  MESSAGE_SET((messageBase + 4));
  MESSAGE_PUT(UPPER_FLOOR_GUARD_B, 0);
  Engine_ActorSetAnimationAndWait(UPPER_FLOOR_GUARD_C, 3);
  MESSAGE_SET((messageBase + 5));
  MESSAGE_PUT(UPPER_FLOOR_GUARD_C, 0);
  Engine_ActorSetAnimationAndWait(UPPER_FLOOR_GUARD_C, 3);
  WAIT(60);
  OBJ_FACE(UPPER_FLOOR_GUARD_B, ACTOR_PARTY_LEADER, 0);
  OBJ_FACE(UPPER_FLOOR_GUARD_C, ACTOR_PARTY_LEADER, 0);
  WAIT(70);
  OBJ_WALK(UPPER_FLOOR_GUARD_A, 672, 88);
  Engine_ActorWaitForMove(UPPER_FLOOR_GUARD_A);
#if TBS_PATCH_1
  OBJ_FACE(UPPER_FLOOR_GUARD_A, ACTOR_PARTY_LEADER, 0);
#endif
  Engine_ActorSetAnimationAndWait(UPPER_FLOOR_GUARD_A, 3);
  WAIT(30);
  MESSAGE_SET((messageBase + 6));
  MESSAGE_PUT(UPPER_FLOOR_GUARD_A, 0);
  gEventWork->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
  Engine_EventSetNextDestination(
      (s32)&SceneId_LunpaFortressUpperFloor,
      LUNPA_FORTRESS_UPPER_FLOOR_GUARD_ROOM_AFTER_BATTLE);
  Battle_SetStart(BATTLE_START_SCENE);
  Engine_EncounterRequestBattle(ENCOUNTER_ZONE_SCRIPTED, 3);
  OBJ_POS_SET(UPPER_FLOOR_GUARD_A, 0, 0);
  OBJ_POS_SET(UPPER_FLOOR_GUARD_B, 0, 0);
  OBJ_POS_SET(UPPER_FLOOR_GUARD_C, 0, 0);
  EV_END();
  FLAG_SET(FLAG_LUNPA_FORTRESS_THREE_GUARDS_BEATEN);
}

/// The three guards lie beaten where they fought.
static void LunpaFortress_PlaceThreeBeatenGuards(void) {
  EV_BEGIN();
  OBJ_POS_SET(UPPER_FLOOR_GUARD_A, 688, 88);
  OBJ_POS_SET(UPPER_FLOOR_GUARD_B, 704, 88);
  OBJ_POS_SET(UPPER_FLOOR_GUARD_C, 720, 96);
  OBJ_MODE(UPPER_FLOOR_GUARD_A, 5);
  OBJ_MODE(UPPER_FLOOR_GUARD_B, 5);
  OBJ_MODE(UPPER_FLOOR_GUARD_C, 5);
  OBJ_FACE(ACTOR_PARTY_LEADER, UPPER_FLOOR_GUARD_B, 0);
  EV_END();
  Engine_EventOpenScreen();
}

/// LunpaFortressUpperFloor, trigger 32: walking up to the east guard, he
/// challenges the party to a battle with a Brigand, and is gone after it.
static void Event_FightEastGuard(void) {

  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_STAND);
  SE(SE_MENU_CANCEL);
  Engine_ActorShowEmote(UPPER_FLOOR_EAST_GUARD, EMOTE_IN_FRONT, 60);
  LunpaFortress_GuardChallengesParty(UPPER_FLOOR_EAST_GUARD);
  gEventWork->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
  Battle_SetStart(BATTLE_START_SCENE);
  Engine_EncounterRequestBattle(ENCOUNTER_ZONE_SCRIPTED, 2);
  Engine_ActorSetPosition(UPPER_FLOOR_EAST_GUARD, 0, 0);
  EV_END();
  Engine_GameFlagSet(FLAG_LUNPA_FORTRESS_EAST_GUARD_BEATEN);
}

/// LunpaFortressGroundFloor, trigger 50 or seen by the entrance guard: the
/// party is sent back to the Lunpa Fortress Gates. "Who are you?"
static void Event_EntranceGuardThrowsPartyOut(void) {
  EV_BEGIN();
  Engine_ActorSetDestinationOffset(ACTOR_PARTY_LEADER, 0, 0);
  Actor_SetBuiltinAction(ACTOR_PARTY_LEADER, ACTOR_ACTION_IDLE);
  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_STAND);
  OBJ_FACE(GROUND_FLOOR_ENTRANCE_GUARD, ACTOR_PARTY_LEADER, 0);
  SE(SE_MENU_CANCEL);
  Engine_ActorShowEmote(GROUND_FLOOR_ENTRANCE_GUARD, EMOTE_IN_FRONT, 60);
  {
    s32 challengeMessage = MsgLunpaWho2;

    MESSAGE_SET(challengeMessage);
    MESSAGE_PUT(GROUND_FLOOR_ENTRANCE_GUARD, 0);
    Engine_ActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 50);
    MESSAGE_SET(challengeMessage + 1);
  }
  MESSAGE_PUT(GROUND_FLOOR_ENTRANCE_GUARD, 0);
  Engine_EventCloseScreen();
  WAIT(60);
  Engine_EventRequestExit(EXIT_THROWN_OUT);
  EV_END();
  Engine_GameFlagSet(FLAG_SCENE_TEMP_24);
}

/// LunpaFortressGroundFloor, trigger 51 between the sentries or seen by them:
/// the party is sent back to the Lunpa Fortress Gates. "Who are you?"
static void Event_SentriesThrowPartyOut(void) {
  s32 challengeMessage;

  EV_BEGIN();
  Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 0);
  Actor_SetBuiltinAction(ACTOR_PARTY_LEADER, ACTOR_ACTION_IDLE);
  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_STAND);
  SE(SE_MENU_CANCEL);
  OBJ_EMOTE(GROUND_FLOOR_SECOND_SENTRY, EMOTE_IN_FRONT, 0);
  OBJ_EMOTE(GROUND_FLOOR_FIRST_SENTRY, EMOTE_IN_FRONT, 60);
  OBJ_FACE(GROUND_FLOOR_SECOND_SENTRY, ACTOR_PARTY_LEADER, 0);
  OBJ_FACE(GROUND_FLOOR_FIRST_SENTRY, ACTOR_PARTY_LEADER, 0);
  challengeMessage = MsgLunpaWho2;
  MESSAGE_SET(challengeMessage);
  MESSAGE_PUT(GROUND_FLOOR_FIRST_SENTRY, 0);
  OBJ_EMOTE(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 30);
  MESSAGE_SET(challengeMessage + 1);
  MESSAGE_PUT(GROUND_FLOOR_FIRST_SENTRY, 0);
  Engine_EventCloseScreen();
  WAIT(60);
  Engine_EventRequestExit(EXIT_THROWN_OUT);
  EV_END();
  FLAG_SET(FLAG_SCENE_TEMP_25);
}

/// The first switch's passage opens.
static void LunpaFortress_OpenFirstSwitchPassage(void) {
  Map_CopyCells(2, 82, 1, 2, 21, 81);
  Map_CopyCellAttributes(21, 32, 1, 1, 21, 34);
}

/// The second switch's passage opens.
static void LunpaFortress_OpenSecondSwitchPassage(void) {
  Map_CopyCells(2, 84, 1, 2, 6, 55);
  Map_CopyCellAttributes(5, 9, 1, 1, 6, 10);
}

/// The passage behind the second Whirlwind cover opens.
static void LunpaFortress_OpenCoveredPassage(void) {
  Map_CopyCells(2, 86, 1, 2, 27, 62);
  Map_CopyCellAttributes(26, 16, 1, 1, 27, 17);
}

/// A switch flips in two frames, then its passage opens.
static __inline__ void
LunpaFortress_FlipHiddenSwitch(s32 cellX, s32 cellY,
                               void (*openPassage)(void)) {
  Map_CopyCells(6, 77, 1, 2, cellX, cellY);
  WAIT(5);
  Map_CopyCells(7, 77, 1, 2, cellX, cellY);
  WAIT(1);
  openPassage();
}

/// LunpaFortressCave, trigger 52 once uncovered: searching the first switch
/// while Reveal shows it flips it, and its passage opens. "You flipped the
/// switch."
static void Event_FlipFirstSwitch(void) {
  if (((struct FieldWork *)gEventWork)->revealActive != 0) {
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

/// LunpaFortressCave, trigger 51 once uncovered: searching the second switch
/// while Reveal shows it flips it, and its passage opens. "You flipped the
/// switch."
static void Event_FlipSecondSwitch(void) {
  if (((struct FieldWork *)gEventWork)->revealActive != 0) {
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
static void Event_RevealSwitches(void) {
  Map_CopyCells(5, 77, 1, 2, 17, 82);
  Map_CopyCells(5, 77, 1, 2, 3, 55);
  Map_CopyCellAttributes(15, 33, 1, 1, 17, 35);
  Map_CopyCellAttributes(3, 8, 1, 1, 3, 10);
}

/// LunpaFortressCave, as Reveal ends: both switches hide again.
static void Event_HideSwitches(void) {
  Map_CopyCells(8, 77, 1, 2, 17, 82);
  Map_CopyCells(8, 77, 1, 2, 3, 55);
  Map_CopyCellAttributes(18, 35, 1, 1, 17, 35);
  Map_CopyCellAttributes(2, 10, 1, 1, 3, 10);
}

/// LunpaFortressCave, Whirlwind on what covers the first switch: it blows away.
static void Event_BlowAwayFirstSwitchCover(void) {
  Engine_GameFlagSet(FLAG_LUNPA_FORTRESS_FIRST_SWITCH_PLANT_BLOWN_AWAY);
  Engine_GameFlagClear(FLAG_SCENE_TEMP_17);
  Engine_ActorSetPosition(CAVE_FIRST_SWITCH_COVER, 0, 0);
}

/// LunpaFortressCave, Whirlwind on what covers the passage: it blows away and
/// the passage opens.
static void Event_BlowAwayPassageCover(void) {
  Engine_GameFlagSet(FLAG_LUNPA_FORTRESS_PASSAGE_PLANT_BLOWN_AWAY);
  LunpaFortress_OpenCoveredPassage();
  Engine_ActorSetPosition(CAVE_PASSAGE_COVER, 0, 0);
}

/// LunpaFortressCave, Whirlwind on what covers the second switch: it blows
/// away.
static void Event_BlowAwaySecondSwitchCover(void) {
  Engine_GameFlagSet(FLAG_LUNPA_FORTRESS_SECOND_SWITCH_PLANT_BLOWN_AWAY);
  Engine_GameFlagClear(FLAG_SCENE_TEMP_18);
  Engine_ActorSetPosition(CAVE_SECOND_SWITCH_COVER, 0, 0);
}

static void UNUSED LunpaFortress_DoNothingE(void) {}

/// Garet, Ivan and Mia step out of the leader and gather around Hammet.
static void LunpaFortress_GatherPartyAroundHammet(void) {
  struct FieldActor *leader;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetPosition(IVAN, leader->x.fixed, leader->z.fixed);
  }
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetPosition(MIA, leader->x.fixed, leader->z.fixed);
  }
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetPosition(GARET, leader->x.fixed, leader->z.fixed);
  }
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  OBJ_SPEED(IVAN, 0xb333, 0x5999);
  OBJ_WALK(IVAN, 456, 192);
  OBJ_SPEED(MIA, 0xb333, 0x5999);
  OBJ_WALK(MIA, 440, 184);
  OBJ_SPEED(GARET, 0xb333, 0x5999);
  OBJ_WALK(GARET, 448, 240);
  Engine_ActorWaitForMove(IVAN);
  OBJ_FACE(IVAN, TOADONPA_HAMMET, 0);
  Engine_ActorWaitForMove(GARET);
  Engine_ActorWaitForMove(MIA);
  OBJ_FACE(GARET, TOADONPA_HAMMET, 0);
  OBJ_FACE(MIA, TOADONPA_HAMMET, 0);
  WAIT(15);
}

/// Dodonpa's lever: the room shakes, the floor opens and everyone startles.
static void LunpaFortress_ActivateLeverTrap(void) {
  Map_SetShake(0x40000, 0x40000, 0x10000);
  SE(SE_141);
  WAIT(80);
  SE(SE_288);
  WAIT(5);
  SE(SE_145);
  Map_CopyCells(16, 75, 7, 4, 26, 55);
  Map_SetShake(-1, -1, 0xe666);
  OBJ_EMOTE(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT, 0);
  OBJ_EMOTE(GARET, EMOTE_IN_FRONT, 0);
  OBJ_EMOTE(IVAN, EMOTE_IN_FRONT, 0);
  OBJ_EMOTE(MIA, EMOTE_IN_FRONT, 0);
  OBJ_EMOTE(TOADONPA_HAMMET, EMOTE_IN_FRONT, 0);
  WAIT(60);
}

/// The camera finds Toadonpa, the monster Dodonpa has freed; it lumbers
/// forward.
static void LunpaFortress_RevealMonster(void) {
  Engine_CameraMoveToActor(TOADONPA_BOSS, 1);
  Engine_CameraWaitForMove();
  WAIT(60);
  MESSAGE_SET(MsgLunpaPrepareBecomeMonster);
  MESSAGE_PUT(TOADONPA_DODONPA, 0);
  OBJ_SPEED(TOADONPA_BOSS, 0x10000, 0x8000);
  OBJ_SPEED(TOADONPA_BOSS_PART, 0x10000, 0x8000);
  OBJ_WALK(TOADONPA_BOSS, 472, 180);
  OBJ_WALK(TOADONPA_BOSS_PART, 472, 180);
  Engine_CameraFollowActor(TOADONPA_BOSS, 1);
  Engine_ActorWaitForMove(TOADONPA_BOSS);
  OBJ_MODE(TOADONPA_BOSS, 4);
  WAIT(30);
}

/// The party spreads out and faces Toadonpa.
static void LunpaFortress_PositionPartyForMonster(void) {
  OBJ_SPEED(IVAN, 0x10000, 0x8000);
  OBJ_WALK(IVAN, 504, 216);
  OBJ_SPEED(MIA, 0x10000, 0x8000);
  OBJ_WALK(MIA, 440, 232);
  OBJ_SPEED(GARET, 0x10000, 0x8000);
  OBJ_WALK(GARET, 480, 224);
  Engine_ActorWaitForMove(GARET);
  OBJ_DIR(GARET, FACING_NORTH, 0);
  Engine_ActorWaitForMove(IVAN);
  OBJ_DIR(IVAN, FACING_NORTH, 0);
  Engine_ActorWaitForMove(MIA);
  OBJ_DIR(MIA, FACING_NORTH, 0);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
}

/// LunpaFortressToadonpa, trigger 20: walking into Hammet's cell. The first
/// time, Hammet hears the party out and recognizes Ivan, then Dodonpa arrives,
/// gloats, pulls a lever and frees Toadonpa on them; after a lost battle, the
/// scene goes straight to the trap. Either way, the battle follows.
static void Event_FindHammet(void) {
  s32 messageBase;

  if (FLAG_CHECK(FLAG_PLACE_TEMP_01) != 0)
    return;
  FLAG_SET(FLAG_SCENE_TEMP_70);
  EV_BEGIN();
  // A retry skips the introduction but repeats Dodonpa's trap and battle.
  if (FLAG_CHECK(FLAG_LUNPA_FORTRESS_TRAP_SEEN) != 0) {
    OBJ_SPEED(ACTOR_PARTY_LEADER, 32768, 16384);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 456, 216);
    OBJ_FACE(ACTOR_PARTY_LEADER, TOADONPA_HAMMET, 0);
    LunpaFortress_GatherPartyAroundHammet();
    OBJ_EMOTE(TOADONPA_HAMMET, EMOTE_IN_FRONT, 60);
    OBJ_DIR(TOADONPA_HAMMET, FACING_WEST, 0);
    Engine_ActorJump(TOADONPA_HAMMET, 4, 0);
    Engine_ActorSetSpriteFlags(Actor_Get(TOADONPA_HAMMET), 1);
    WAIT(30);
    OBJ_SPEED(IVAN, 45875, 22937);
    OBJ_WALK(IVAN, 464, 192);
    Engine_ActorWaitForMove(IVAN);
    WAIT(30);
    OBJ_DIR(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    OBJ_DIR(IVAN, FACING_SOUTH, 0);
    OBJ_DIR(GARET, FACING_SOUTH, 0);
    OBJ_DIR(MIA, FACING_SOUTH, 0);
    // Dodonpa approaches from the south, then springs the lever trap.
    OBJ_POS_SET(TOADONPA_DODONPA, 456, 320);
    Camera_SetSpeed(131072, 16384);
    messageBase = MsgLunpaBackMoreGuess;
    MESSAGE_SET(messageBase);
    MESSAGE_PUT(TOADONPA_DODONPA, 0);
    OBJ_WALK(TOADONPA_DODONPA, 458, 272);
    Engine_ActorWaitForMove(TOADONPA_DODONPA);
    OBJ_DIR(TOADONPA_DODONPA, FACING_SOUTH + FACING_STEP, 0);
    WAIT(40);
    Actor_SetDestinationOffset(TOADONPA_DODONPA, -8, 8);
    Engine_ActorWaitForMove(TOADONPA_DODONPA);
    WAIT(60);
    SE(SE_155);
    Engine_MessageShowCentered(messageBase + 1, 1);
    Actor_SetDestinationOffset(TOADONPA_DODONPA, 8, -8);
    LunpaFortress_ActivateLeverTrap();
    WAIT(120);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_ActorStartRepeatedMotion(IVAN, 2);
    Engine_ActorStartRepeatedMotion(GARET, 2);
    Engine_ActorStartRepeatedMotion(MIA, 2);
    WAIT(20);
    messageBase += 2;
    LunpaFortress_PositionPartyForMonster();
    OBJ_FACE(TOADONPA_DODONPA, ACTOR_PARTY_LEADER, 0);
    MESSAGE_SET(messageBase);
    MESSAGE_PUT(TOADONPA_DODONPA, 0);
    LunpaFortress_RevealMonster();
    gEventWork->startTransition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
    WAIT(1);
    Battle_SetStart(BATTLE_START_SCENE);
    Engine_EventSetNextDestination((s32)&SceneId_LunpaFortressToadonpa,
                                   LUNPA_FORTRESS_TOADONPA_AFTER_BATTLE);
    Engine_EncounterRequestBattle(ENCOUNTER_ZONE_SCRIPTED, 4);
  } else {
    OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_STAND);
    SE(MUS_017);
    WAIT(30);
    // Ivan recognizes Hammet before Dodonpa interrupts the reunion.
    messageBase = MsgLunpaTimeEat;
    MESSAGE_SET(messageBase);
    MESSAGE_PUT(TOADONPA_HAMMET, 0);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, TOADONPA_HAMMET, 0);
    WAIT(140);
    OBJ_DIR(TOADONPA_HAMMET, FACING_WEST, 0);
    Engine_ActorJump(TOADONPA_HAMMET, 4, 0);
    Engine_ActorSetSpriteFlags(Actor_Get(TOADONPA_HAMMET), 1);
    MESSAGE_SET(messageBase + 1);
    MESSAGE_PUT(TOADONPA_HAMMET, 0);
    OBJ_SPEED(ACTOR_PARTY_LEADER, 32768, 16384);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 456, 216);
    OBJ_FACE(ACTOR_PARTY_LEADER, TOADONPA_HAMMET, 0);
    LunpaFortress_GatherPartyAroundHammet();
    OBJ_SPEED(IVAN, 45875, 22937);
    OBJ_WALK(IVAN, 464, 192);
    Engine_ActorWaitForMove(IVAN);
    WAIT(30);
    MESSAGE_SET(messageBase + 2);
    MESSAGE_PUT(IVAN, 0);
    OBJ_EMOTE(TOADONPA_HAMMET, EMOTE_IN_FRONT, 0);
    WAIT(110);
    SE(MUS_060);
    MESSAGE_SET(messageBase + 3);
    MESSAGE_PUT(TOADONPA_HAMMET, 0);
    WAIT(30);
    Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
    WAIT(20);
    OBJ_MODE(IVAN, ANIM_STAND);
    Engine_ActorStartRepeatedMotion(TOADONPA_HAMMET, 1);
    WAIT(20);
    OBJ_SPEED(TOADONPA_HAMMET, 26214, 13107);
    OBJ_WALK(TOADONPA_HAMMET, 520, 208);
    Engine_ActorWaitForMove(TOADONPA_HAMMET);
    OBJ_MODE(TOADONPA_HAMMET, 1);
    WAIT(20);
    OBJ_DIR(TOADONPA_HAMMET, FACING_NORTHWEST + FACING_STEP, 0);
    WAIT(30);
    OBJ_DIR(TOADONPA_HAMMET, FACING_SOUTH + FACING_STEP, 0);
    WAIT(30);
    OBJ_FACE(TOADONPA_HAMMET, IVAN, 0);
    WAIT(20);
    MESSAGE_SET(messageBase + 4);
    MESSAGE_PUT(TOADONPA_HAMMET, 0);
    WAIT(40);
    Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
    WAIT(20);
    OBJ_EMOTE(TOADONPA_HAMMET, EMOTE_IN_FRONT | 8, 0);
    WAIT(120);
    MESSAGE_SET(messageBase + 5);
    MESSAGE_PUT(TOADONPA_HAMMET, 0);
    WAIT(25);
    Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
    WAIT(30);
    Engine_ActorSetAnimationAndWait(TOADONPA_HAMMET, 3);
    WAIT(40);
    OBJ_WALK(IVAN, 480, 200);
    Engine_ActorWaitForMove(IVAN);
    Engine_ActorFaceEachOther(IVAN, TOADONPA_HAMMET, 0);
    WAIT(60);
    MESSAGE_SET(messageBase + 6);
    MESSAGE_PUT(IVAN, 0);
    WAIT(20);
    OBJ_MODE(TOADONPA_HAMMET, 4);
    WAIT(80);
    MESSAGE_SET(messageBase + 7);
    MESSAGE_PUT(TOADONPA_HAMMET, 0);
    // Dodonpa approaches from the south, then springs the lever trap.
    OBJ_POS_SET(TOADONPA_DODONPA, 456, 320);
    SE(MUS_019);
    MESSAGE_SET(messageBase + 8);
    MESSAGE_PUT(TOADONPA_DODONPA, 0);
    OBJ_FACE(ACTOR_PARTY_LEADER, TOADONPA_DODONPA, 0);
    OBJ_FACE(IVAN, TOADONPA_DODONPA, 0);
    OBJ_FACE(GARET, TOADONPA_DODONPA, 0);
    WAIT(5);
    OBJ_DIR(MIA, FACING_SOUTH, 0);
    OBJ_FACE(TOADONPA_HAMMET, TOADONPA_DODONPA, 0);
    WAIT(30);
    SE(MUS_061);
    Camera_SetSpeed(131072, 16384);
    Engine_CameraMoveToActor(TOADONPA_DODONPA, 1);
    Engine_CameraWaitForMove();
    OBJ_SPEED(TOADONPA_DODONPA, 52428, 26214);
    OBJ_WALK(TOADONPA_DODONPA, 456, 304);
    Engine_CameraFollowActor(TOADONPA_DODONPA, 1);
    Engine_ActorWaitForMove(TOADONPA_DODONPA);
    Engine_CameraFollowActor(GARET, 1);
    OBJ_EMOTE(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 0);
    OBJ_EMOTE(IVAN, EMOTE_IN_FRONT | 2, 0);
    OBJ_EMOTE(GARET, EMOTE_IN_FRONT | 2, 0);
    OBJ_EMOTE(MIA, EMOTE_IN_FRONT | 2, 0);
    OBJ_EMOTE(TOADONPA_HAMMET, EMOTE_IN_FRONT | 2, 0);
    WAIT(60);
    OBJ_FACE(TOADONPA_HAMMET, TOADONPA_DODONPA, 0);
    Engine_ActorStartRepeatedMotion(TOADONPA_HAMMET, 2);
    WAIT(60);
    MESSAGE_SET(messageBase + 9);
    MESSAGE_PUT(TOADONPA_HAMMET, 0);
    OBJ_FACE(TOADONPA_DODONPA, TOADONPA_HAMMET, 0);
    MESSAGE_SET(messageBase + 10);
    MESSAGE_PUT(TOADONPA_DODONPA, 0);
    WAIT(60);
    OBJ_FACE(TOADONPA_DODONPA, IVAN, 0);
    WAIT(30);
    MESSAGE_SET(messageBase + 11);
    MESSAGE_PUT(TOADONPA_DODONPA, 0);
    Engine_ActorFaceEachOther(MIA, IVAN, 0);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, GARET, 0);
    WAIT(60);
    OBJ_FACE(ACTOR_PARTY_LEADER, TOADONPA_DODONPA, 0);
    OBJ_FACE(IVAN, TOADONPA_DODONPA, 0);
    OBJ_FACE(GARET, TOADONPA_DODONPA, 0);
    OBJ_FACE(MIA, TOADONPA_DODONPA, 0);
    Engine_ActorStartRepeatedMotion(TOADONPA_DODONPA, 1);
    WAIT(60);
    MESSAGE_SET(messageBase + 12);
    MESSAGE_PUT(TOADONPA_DODONPA, 0);
    OBJ_EMOTE(GARET, EMOTE_IN_FRONT | 3, 0);
    WAIT(60);
    OBJ_MODE(TOADONPA_DODONPA, 4);
    MESSAGE_SET(messageBase + 13);
    MESSAGE_PUT(TOADONPA_DODONPA, 0);
    OBJ_WALK(GARET, 456, 248);
    Engine_ActorWaitForMove(GARET);
    OBJ_DIR(GARET, FACING_SOUTH, 0);
    MESSAGE_SET(messageBase + 14);
    MESSAGE_PUT(GARET, 0);
    OBJ_WALK(IVAN, 472, 216);
    Engine_ActorWaitForMove(IVAN);
    OBJ_DIR(IVAN, FACING_SOUTH, 0);
    WAIT(10);
    MESSAGE_SET(messageBase + 15);
    MESSAGE_PUT(IVAN, 0);
    OBJ_EMOTE(TOADONPA_HAMMET, EMOTE_IN_FRONT | 5, 0);
    WAIT(60);
    MESSAGE_SET(messageBase + 16);
    MESSAGE_PUT(TOADONPA_HAMMET, 0);
    OBJ_WALK(MIA, 440, 216);
    Engine_ActorWaitForMove(MIA);
    OBJ_FACE(MIA, TOADONPA_DODONPA, 0);
    Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
    WAIT(10);
    MESSAGE_SET(messageBase + 17);
    MESSAGE_PUT(MIA, 0);
    OBJ_MODE(TOADONPA_DODONPA, 4);
    MESSAGE_SET(messageBase + 18);
    MESSAGE_PUT(TOADONPA_DODONPA, 0);
    OBJ_EMOTE(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 0);
    OBJ_EMOTE(GARET, EMOTE_IN_FRONT | 2, 0);
    OBJ_EMOTE(MIA, EMOTE_IN_FRONT | 2, 0);
    OBJ_EMOTE(IVAN, EMOTE_IN_FRONT | 2, 0);
    OBJ_EMOTE(TOADONPA_DODONPA, EMOTE_IN_FRONT | 8, 0);
    WAIT(60);
    MESSAGE_SET(messageBase + 19);
    MESSAGE_PUT(TOADONPA_DODONPA, 0);
    WAIT(20);
    OBJ_EMOTE(GARET, EMOTE_IN_FRONT | 3, 0);
    WAIT(60);
    MESSAGE_SET(messageBase + 20);
    MESSAGE_PUT(GARET, 0);
    Engine_ActorStartRepeatedMotion(TOADONPA_DODONPA, 1);
    WAIT(60);
    MESSAGE_SET(messageBase + 21);
    MESSAGE_PUT(TOADONPA_DODONPA, 0);
    OBJ_WALK(TOADONPA_DODONPA, 456, 280);
    Engine_ActorWaitForMove(TOADONPA_DODONPA);
    OBJ_DIR(TOADONPA_DODONPA, FACING_SOUTH + FACING_STEP, 0);
    WAIT(80);
    Actor_SetDestinationOffset(TOADONPA_DODONPA, -8, 8);
    Engine_ActorWaitForMove(TOADONPA_DODONPA);
    WAIT(60);
    SE(SE_155);
    Engine_MessageShowCentered(MsgLunpaDodonpaPulledLever, 1);
    Actor_SetDestinationOffset(TOADONPA_DODONPA, 8, -8);
    OBJ_FACE(TOADONPA_DODONPA, TOADONPA_BOSS, 0);
    LunpaFortress_ActivateLeverTrap();
    SE(MUS_052);
    MESSAGE_SET(messageBase + 23);
    MESSAGE_PUT(TOADONPA_DODONPA, 0);
    WAIT(60);
    OBJ_FACE(ACTOR_PARTY_LEADER, TOADONPA_BOSS, 0);
    OBJ_FACE(GARET, TOADONPA_BOSS, 0);
    OBJ_FACE(IVAN, TOADONPA_BOSS, 0);
    OBJ_FACE(MIA, TOADONPA_BOSS, 0);
    OBJ_FACE(TOADONPA_HAMMET, TOADONPA_BOSS, 0);
    LunpaFortress_PositionPartyForMonster();
    LunpaFortress_RevealMonster();
    FLAG_SET(FLAG_LUNPA_FORTRESS_TRAP_SEEN);
    gEventWork->startTransition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
    WAIT(1);
    Battle_SetStart(BATTLE_START_SCENE);
    Engine_EventSetNextDestination((s32)&SceneId_LunpaFortressToadonpa,
                                   LUNPA_FORTRESS_TOADONPA_AFTER_BATTLE);
    Engine_EncounterRequestBattle(ENCOUNTER_ZONE_SCRIPTED, 4);
  }
  EV_END();
}

/// LunpaFortressToadonpa, LUNPA_FORTRESS_TOADONPA_AFTER_BATTLE after the battle
/// with Toadonpa: the monster has fallen on Dodonpa. The party may pull it off
/// him; they argue over his punishment until his father Donpa arrives, who
/// orders him locked up, thanks the party, and lets them leave with Hammet.
/// Sets FLAG_DODONPA_LOCKED_UP.
static void Event_DonpaJudgesDodonpa(void) {
  struct FieldActor *leader;
  s32 aftermathMessage;
  s32 punishmentMessage;
  s32 donpaMessage;

  FLAG_SET(FLAG_PLACE_TEMP_01);
  FLAG_SET(FLAG_DODONPA_LOCKED_UP);
  Map_CopyCells(16, 75, 7, 4, 26, 55);
  LunpaFortress_OpenToadonpaDoor(4);
  EV_BEGIN();
  leader = Actor_Get(TOADONPA_HAMMET);
  Engine_ActorSetSpriteFlags(leader, 1);
  OBJ_POS_SET(ACTOR_PARTY_LEADER, 456, 184);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  OBJ_POS_SET(GARET, 440, 192);
  OBJ_DIR(GARET, FACING_SOUTH, 0);
  OBJ_POS_SET(MIA, 488, 184);
  OBJ_DIR(MIA, FACING_SOUTH, 0);
  OBJ_POS_SET(IVAN, 472, 184);
  OBJ_DIR(IVAN, FACING_SOUTH, 0);
  OBJ_POS_SET(TOADONPA_HAMMET, 520, 224);
  OBJ_DIR(TOADONPA_HAMMET, FACING_NORTHWEST + FACING_STEP, 0);
  OBJ_POS_SET(TOADONPA_BOSS, 448, 237);
  OBJ_DIR(TOADONPA_BOSS, FACING_WEST, 0);
  OBJ_POS_SET(TOADONPA_BOSS_PART, 448, 238);
  Engine_ActorSetSpritePriority(TOADONPA_BOSS_PART, 3);
  OBJ_MODE(TOADONPA_BOSS, 3);
  OBJ_POS_SET(TOADONPA_DODONPA, 458, 243);
  OBJ_DIR(TOADONPA_DODONPA, FACING_SOUTH, 0);
  OBJ_MODE(TOADONPA_DODONPA, 5);
  Engine_EventOpenScreen();
  WAIT(120);

  // The fallen monster has pinned Dodonpa; decide whether to help him.
  aftermathMessage = MsgLunpaUhnnGetOff;
  MESSAGE_SET(aftermathMessage);
  MESSAGE_PUT(TOADONPA_DODONPA, 0);
  OBJ_EMOTE(GARET, EMOTE_IN_FRONT | 1, 60);
  MESSAGE_SET((aftermathMessage + 1));
  MESSAGE_PUT(GARET, 0);
  OBJ_EMOTE(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1, 1);
  OBJ_EMOTE(IVAN, EMOTE_IN_FRONT | 1, 1);
  OBJ_EMOTE(MIA, EMOTE_IN_FRONT | 1, 1);
  WAIT(60);
  OBJ_FACE(TOADONPA_HAMMET, TOADONPA_DODONPA, 0);
  WAIT(60);
  MESSAGE_SET((aftermathMessage + 2));
  MESSAGE_PUT(TOADONPA_HAMMET, 0);
  OBJ_FACE(ACTOR_PARTY_LEADER, TOADONPA_HAMMET, 0);
  OBJ_FACE(GARET, TOADONPA_HAMMET, 0);
  OBJ_FACE(IVAN, TOADONPA_HAMMET, 0);
  OBJ_FACE(MIA, TOADONPA_HAMMET, 0);
  WAIT(60);
  OBJ_WALK(TOADONPA_HAMMET, 512, 232);
  OBJ_MODE(TOADONPA_HAMMET, 4);
  WAIT(60);
  MESSAGE_SET((aftermathMessage + 3));
  MESSAGE_PUT(TOADONPA_HAMMET, 0);
  WAIT(15);
  Engine_ActorStartRepeatedMotion(TOADONPA_DODONPA, 2);
  MESSAGE_SET((aftermathMessage + 4));
  MESSAGE_PUT(TOADONPA_DODONPA, 0);
  OBJ_FACE(ACTOR_PARTY_LEADER, TOADONPA_DODONPA, 0);
  OBJ_FACE(IVAN, TOADONPA_DODONPA, 0);
  OBJ_FACE(MIA, TOADONPA_DODONPA, 0);
  OBJ_FACE(GARET, TOADONPA_DODONPA, 0);
  Engine_ActorStartRepeatedMotion(IVAN, 1);
  MESSAGE_SET((aftermathMessage + 5));
  MESSAGE_PUT(IVAN, 0);
  OBJ_EMOTE(MIA, EMOTE_IN_FRONT | 5, 0);
  OBJ_MODE(MIA, ANIM_SHAKE_HEAD);
  WAIT(80);
  OBJ_MODE(MIA, ANIM_STAND);
  MESSAGE_SET((aftermathMessage + 6));
  MESSAGE_PUT(MIA, 0);
  OBJ_EMOTE(TOADONPA_HAMMET, EMOTE_IN_FRONT | 2, 65);
  Engine_ActorStartRepeatedMotion(TOADONPA_HAMMET, 2);
  WAIT(100);
  MESSAGE_SET((aftermathMessage + 7));
  MESSAGE_PUT(TOADONPA_DODONPA, 0);
  OBJ_FACE(GARET, ACTOR_PARTY_LEADER, 0);
  WAIT(10);
  MESSAGE_SET((aftermathMessage + 8));
  MESSAGE_PUT(GARET, 0);
  Engine_ActorStartRepeatedMotion(IVAN, 1);
  WAIT(30);
  MESSAGE_SET((aftermathMessage + 9));
  MESSAGE_PUT(IVAN, 0);
  OBJ_FACE(MIA, ACTOR_PARTY_LEADER, 0);
  MESSAGE_SET((aftermathMessage + 10));
  MESSAGE_OPEN(MIA, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    OBJ_MODE(IVAN, ANIM_NOD);
    OBJ_MODE(GARET, ANIM_NOD);
    OBJ_MODE(MIA, ANIM_NOD);
    OBJ_FACE(TOADONPA_HAMMET, TOADONPA_DODONPA, 0);
    WAIT(60);
  } else {
    MESSAGE_SET((aftermathMessage + 11));
    MESSAGE_PUT(MIA, 0);
    OBJ_FACE(ACTOR_PARTY_LEADER, TOADONPA_HAMMET, 0);
    OBJ_FACE(GARET, TOADONPA_HAMMET, 0);
    OBJ_FACE(IVAN, TOADONPA_HAMMET, 0);
    OBJ_FACE(MIA, TOADONPA_HAMMET, 0);
    WAIT(20);
    OBJ_FACE(TOADONPA_HAMMET, ACTOR_PARTY_LEADER, 0);
    WAIT(60);
    OBJ_FACE(TOADONPA_HAMMET, TOADONPA_DODONPA, 0);
    WAIT(80);
    Engine_ActorSetAnimationAndWait(TOADONPA_HAMMET, 3);
    WAIT(30);
    OBJ_FACE(TOADONPA_HAMMET, ACTOR_PARTY_LEADER, 0);
    MESSAGE_SET((aftermathMessage + 12));
    MESSAGE_PUT(TOADONPA_HAMMET, 0);
    WAIT(60);
    OBJ_FACE(IVAN, TOADONPA_HAMMET, 0);
    OBJ_FACE(GARET, TOADONPA_HAMMET, 0);
    OBJ_FACE(MIA, TOADONPA_HAMMET, 0);
    OBJ_FACE(ACTOR_PARTY_LEADER, TOADONPA_HAMMET, 0);
    OBJ_MODE(IVAN, ANIM_NOD);
    OBJ_MODE(GARET, ANIM_NOD);
    OBJ_MODE(MIA, ANIM_NOD);
    OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_NOD);
    WAIT(80);
    OBJ_FACE(TOADONPA_HAMMET, TOADONPA_DODONPA, 0);
    OBJ_FACE(IVAN, TOADONPA_DODONPA, 0);
    OBJ_FACE(GARET, TOADONPA_DODONPA, 0);
    OBJ_FACE(MIA, TOADONPA_DODONPA, 0);
    OBJ_FACE(ACTOR_PARTY_LEADER, TOADONPA_DODONPA, 0);
  }
  OBJ_SPEED(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
  OBJ_SPEED(GARET, 0x8000, 0x4000);
  OBJ_SPEED(MIA, 0x8000, 0x4000);
  OBJ_SPEED(IVAN, 0x8000, 0x4000);
  OBJ_WALK(GARET, 416, 216);
  Engine_ActorWaitForMove(GARET);
  OBJ_WALK(GARET, 416, 248);
  Engine_ActorWaitForMove(GARET);
  OBJ_WALK(GARET, 440, 248);
  Engine_ActorWaitForMove(GARET);
  OBJ_DIR(GARET, FACING_NORTH, 0);
  OBJ_WALK(ACTOR_PARTY_LEADER, 440, 216);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  OBJ_WALK(MIA, 488, 248);
  Engine_ActorWaitForMove(MIA);
  OBJ_WALK(MIA, 456, 248);
  Engine_ActorWaitForMove(MIA);
  OBJ_DIR(MIA, FACING_NORTH, 0);
  OBJ_WALK(IVAN, 456, 216);
  Engine_ActorWaitForMove(IVAN);
  OBJ_DIR(IVAN, FACING_WEST, 0);
  Engine_ActorFaceEachOther(GARET, ACTOR_PARTY_LEADER, 0);
  Engine_ActorFaceEachOther(MIA, IVAN, 0);
  WAIT(60);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  OBJ_MODE(IVAN, ANIM_NOD);
  OBJ_MODE(GARET, ANIM_NOD);
  OBJ_MODE(MIA, ANIM_NOD);
  WAIT(100);
  SE(SE_226);
  OBJ_MODE(TOADONPA_DODONPA, 7);
  // Move the party and both monster parts together to free Dodonpa.
  OBJ_SPEED(ACTOR_PARTY_LEADER, 0x1999, 0xccc);
  Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, -24, 0);
  OBJ_SPEED(GARET, 0x1999, 0xccc);
  Actor_SetDestinationOffset(GARET, -24, 0);
  OBJ_SPEED(MIA, 0x1999, 0xccc);
  Actor_SetDestinationOffset(MIA, -24, 0);
  OBJ_SPEED(IVAN, 0x1999, 0xccc);
  Actor_SetDestinationOffset(IVAN, -24, 0);
  OBJ_SPEED(TOADONPA_BOSS, 0x1999, 0xccc);
  OBJ_SPEED(TOADONPA_BOSS_PART, 0x1999, 0xccc);
  Actor_SetDestinationOffset(TOADONPA_BOSS, -24, 0);
  Actor_SetDestinationOffset(TOADONPA_BOSS_PART, -24, 0);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  SE(SE_288);
  WAIT(60);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  OBJ_DIR(GARET, FACING_NORTH, 0);
  OBJ_DIR(IVAN, FACING_SOUTH, 0);
  OBJ_DIR(MIA, FACING_NORTH, 0);
  WAIT(60);
  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_NOD);
  OBJ_MODE(GARET, ANIM_NOD);
  OBJ_MODE(IVAN, ANIM_NOD);
  OBJ_MODE(MIA, ANIM_NOD);
  WAIT(100);
  OBJ_SPEED(ACTOR_PARTY_LEADER, 0xb333, 0x5999);
  OBJ_SPEED(GARET, 0xb333, 0x5999);
  OBJ_SPEED(IVAN, 0xb333, 0x5999);
  OBJ_SPEED(MIA, 0xb333, 0x5999);
  OBJ_WALK(ACTOR_PARTY_LEADER, 456, 184);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_STAND);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  OBJ_WALK(GARET, 464, 256);
  Engine_ActorWaitForMove(GARET);
  OBJ_WALK(GARET, 480, 248);
  Engine_ActorWaitForMove(GARET);
  OBJ_WALK(GARET, 440, 192);
  Engine_ActorWaitForMove(GARET);
  OBJ_MODE(GARET, ANIM_STAND);
  OBJ_DIR(GARET, FACING_SOUTH, 0);
  OBJ_WALK(MIA, 488, 248);
  Engine_ActorWaitForMove(MIA);
  OBJ_WALK(MIA, 488, 184);
  OBJ_WALK(IVAN, 472, 184);
  Engine_ActorWaitForMove(IVAN);
  OBJ_MODE(IVAN, ANIM_STAND);
  OBJ_DIR(IVAN, FACING_SOUTH, 0);
  Engine_ActorWaitForMove(MIA);
  OBJ_MODE(MIA, ANIM_STAND);
  OBJ_DIR(MIA, FACING_SOUTH, 0);
  WAIT(30);
  WAIT(60);
  // Debate Dodonpa's punishment before his father arrives.
  punishmentMessage = MsgLunpaAbleGet;
  MESSAGE_SET(punishmentMessage);
  MESSAGE_PUT(GARET, 0);
  OBJ_MODE(TOADONPA_DODONPA, 6);
  WAIT(120);
  Engine_ActorStartRepeatedMotion(TOADONPA_DODONPA, 2);
  WAIT(60);
  OBJ_MODE(TOADONPA_DODONPA, 7);
  MESSAGE_SET((punishmentMessage + 1));
  MESSAGE_PUT(TOADONPA_DODONPA, 0);
  WAIT(20);
  Engine_ActorFaceEachOther(MIA, IVAN, 0);
  WAIT(10);
  OBJ_EMOTE(MIA, EMOTE_IN_FRONT | 1, 80);
  OBJ_FACE(MIA, TOADONPA_DODONPA, 0);
  OBJ_FACE(IVAN, TOADONPA_DODONPA, 0);
  MESSAGE_SET((punishmentMessage + 2));
  MESSAGE_PUT(MIA, 0);
  OBJ_EMOTE(TOADONPA_DODONPA, EMOTE_IN_FRONT | 2, 70);
  Engine_ActorStartRepeatedMotion(TOADONPA_DODONPA, 2);
  WAIT(60);
  OBJ_MODE(TOADONPA_DODONPA, 5);
  WAIT(70);
  MESSAGE_SET((punishmentMessage + 3));
  MESSAGE_PUT(TOADONPA_DODONPA, 0);
  OBJ_MODE(TOADONPA_DODONPA, 7);
  OBJ_EMOTE(IVAN, EMOTE_IN_FRONT | 8, 40);
  MESSAGE_SET((punishmentMessage + 4));
  MESSAGE_PUT(IVAN, 0);
  Engine_ActorSetAnimationAndWait(TOADONPA_HAMMET, 3);
  OBJ_EMOTE(GARET, EMOTE_IN_FRONT | 3, 60);
  OBJ_WALK(GARET, 440, 208);
  Engine_ActorWaitForMove(GARET);
  OBJ_FACE(GARET, ACTOR_PARTY_LEADER, 0);
  MESSAGE_SET((punishmentMessage + 5));
  MESSAGE_PUT(GARET, 0);
  WAIT(20);
  OBJ_EMOTE(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1, 0);
  OBJ_EMOTE(IVAN, EMOTE_IN_FRONT | 1, 0);
  OBJ_EMOTE(MIA, EMOTE_IN_FRONT | 1, 0);
  OBJ_EMOTE(TOADONPA_HAMMET, EMOTE_IN_FRONT | 1, 0);
  WAIT(70);
  OBJ_FACE(GARET, IVAN, 0);
  OBJ_EMOTE(GARET, EMOTE_IN_FRONT | 2, 75);
  OBJ_FACE(GARET, ACTOR_PARTY_LEADER, 0);
  OBJ_MODE(GARET, ANIM_SHAKE_HEAD);
  MESSAGE_SET((punishmentMessage + 6));
  MESSAGE_PUT(GARET, 0);
  OBJ_EMOTE(TOADONPA_HAMMET, EMOTE_IN_FRONT | 1, 0);
  WAIT(60);
  MESSAGE_SET((punishmentMessage + 7));
  MESSAGE_PUT(TOADONPA_HAMMET, 0);
  OBJ_WALK(GARET, 448, 208);
  Engine_ActorWaitForMove(GARET);
  OBJ_MODE(GARET, ANIM_STAND);
  OBJ_FACE(GARET, TOADONPA_HAMMET, 0);
  MESSAGE_SET((punishmentMessage + 8));
  MESSAGE_PUT(GARET, 0);
  Engine_ActorStartRepeatedMotion(TOADONPA_HAMMET, 1);
  WAIT(60);
  OBJ_FACE(GARET, TOADONPA_HAMMET, 0);
  WAIT(60);
  MESSAGE_SET((punishmentMessage + 9));
  MESSAGE_PUT(TOADONPA_HAMMET, 0);
  OBJ_MODE(GARET, ANIM_SHAKE_HEAD);
  WAIT(60);
  MESSAGE_SET((punishmentMessage + 10));
  MESSAGE_PUT(GARET, 0);
  Engine_ActorStartRepeatedMotion(IVAN, 1);
  WAIT(30);
  MESSAGE_SET((punishmentMessage + 11));
  MESSAGE_PUT(IVAN, 0);
  OBJ_FACE(GARET, MIA, 0);
  WAIT(20);
  MESSAGE_SET((punishmentMessage + 12));
  MESSAGE_PUT(GARET, 0);
  WAIT(30);
  OBJ_EMOTE(MIA, EMOTE_IN_FRONT, 80);
  OBJ_FACE(MIA, GARET, 0);
  MESSAGE_SET((punishmentMessage + 13));
  MESSAGE_PUT(MIA, 0);
  OBJ_FACE(MIA, TOADONPA_DODONPA, 0);
  MESSAGE_SET((punishmentMessage + 14));
  MESSAGE_PUT(MIA, 0);
  OBJ_FACE(ACTOR_PARTY_LEADER, TOADONPA_DODONPA, 0);
  OBJ_FACE(IVAN, TOADONPA_DODONPA, 0);
  OBJ_FACE(GARET, TOADONPA_DODONPA, 0);
  OBJ_FACE(MIA, TOADONPA_DODONPA, 0);
  WAIT(120);
  OBJ_EMOTE(TOADONPA_DODONPA, EMOTE_IN_FRONT | 2, 30);
  Engine_ActorStartRepeatedMotion(TOADONPA_DODONPA, 1);
  WAIT(120);
  Engine_ActorStartRepeatedMotion(TOADONPA_HAMMET, 1);
  WAIT(60);
  MESSAGE_SET((punishmentMessage + 15));
  MESSAGE_PUT(TOADONPA_HAMMET, 0);
  OBJ_EMOTE(GARET, EMOTE_IN_FRONT | 7, 110);
  OBJ_WALK(GARET, 456, 212);
  Engine_ActorWaitForMove(GARET);
  OBJ_MODE(GARET, ANIM_STAND);
  MESSAGE_SET((punishmentMessage + 16));
  MESSAGE_PUT(GARET, 0);
  OBJ_FACE(IVAN, GARET, 0);
  WAIT(60);
  Engine_ActorStartRepeatedMotion(IVAN, 1);
  MESSAGE_SET((punishmentMessage + 17));
  MESSAGE_PUT(IVAN, 0);
  OBJ_FACE(GARET, TOADONPA_DODONPA, 0);
  WAIT(80);
  OBJ_FACE(GARET, IVAN, 0);
  WAIT(60);
  OBJ_FACE(GARET, TOADONPA_DODONPA, 0);
  WAIT(30);
  OBJ_FACE(ACTOR_PARTY_LEADER, TOADONPA_DODONPA, 0);
  OBJ_FACE(MIA, TOADONPA_DODONPA, 0);
  OBJ_FACE(TOADONPA_DODONPA, TOADONPA_DODONPA, 0);
  OBJ_FACE(TOADONPA_HAMMET, TOADONPA_DODONPA, 0);
  WAIT(80);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  WAIT(30);
  MESSAGE_SET((punishmentMessage + 18));
  MESSAGE_PUT(GARET, 0);
  Engine_ActorStartRepeatedMotion(TOADONPA_DODONPA, 2);
  WAIT(70);
  MESSAGE_SET((punishmentMessage + 19));
  MESSAGE_PUT(TOADONPA_DODONPA, 0);
  OBJ_EMOTE(MIA, EMOTE_IN_FRONT, 60);
  MESSAGE_SET((punishmentMessage + 20));
  MESSAGE_PUT(MIA, 0);
  OBJ_FACE(IVAN, ACTOR_PARTY_LEADER, 0);
  WAIT(80);
  MESSAGE_SET((punishmentMessage + 21));
  MESSAGE_OPEN(IVAN, 0);
  OBJ_FACE(GARET, ACTOR_PARTY_LEADER, 0);
  OBJ_FACE(MIA, ACTOR_PARTY_LEADER, 0);
  OBJ_FACE(IVAN, ACTOR_PARTY_LEADER, 0);
  OBJ_FACE(TOADONPA_HAMMET, ACTOR_PARTY_LEADER, 0);
  OBJ_POS_SET(TOADONPA_DONPA, 456, 304);
  OBJ_SPEED(TOADONPA_DONPA, 0x8000, 0x4000);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    SE(MUS_019);
    MESSAGE_SET((punishmentMessage + 22));
    MESSAGE_PUT(TOADONPA_DONPA, 0);
  } else {
    SE(MUS_019);
    MESSAGE_SET((punishmentMessage + 23));
    MESSAGE_PUT(TOADONPA_DONPA, 0);
  }
  OBJ_FACE(ACTOR_PARTY_LEADER, TOADONPA_DONPA, 0);
  OBJ_FACE(GARET, TOADONPA_DONPA, 0);
  OBJ_FACE(MIA, TOADONPA_DONPA, 0);
  OBJ_FACE(IVAN, TOADONPA_DONPA, 0);
  OBJ_FACE(TOADONPA_HAMMET, TOADONPA_DONPA, 0);
  OBJ_FACE(TOADONPA_DODONPA, TOADONPA_DONPA, 0);
  Camera_MoveTo(0x1c80000, -1, 0xf00000, 1);
  Engine_ActorStartRepeatedMotion(TOADONPA_DODONPA, 1);
  WAIT(60);
  SE(MUS_008);
  // Donpa takes charge and orders his son to remain imprisoned.
  donpaMessage = MsgLunpaDad;
  MESSAGE_SET(donpaMessage);
  MESSAGE_PUT(TOADONPA_DODONPA, 0);
  OBJ_WALK(TOADONPA_DONPA, 456, 280);
  Engine_ActorWaitForMove(TOADONPA_DONPA);
  OBJ_WALK(TOADONPA_DONPA, 440, 256);
  Camera_MoveTo(0x1c80000, -1, 0xe00000, 1);
  MESSAGE_SET((donpaMessage + 1));
  MESSAGE_PUT(TOADONPA_DONPA, 0);
  Engine_ActorWaitForMove(TOADONPA_DONPA);
  OBJ_DIR(TOADONPA_DONPA, FACING_NORTH + FACING_STEP, 0);
  OBJ_EMOTE(TOADONPA_DODONPA, EMOTE_IN_FRONT | 2, 80);
  OBJ_MODE(TOADONPA_DONPA, 4);
  WAIT(89);
  MESSAGE_SET((donpaMessage + 2));
  MESSAGE_PUT(TOADONPA_DONPA, 0);
  Engine_ActorRunRepeatedMotion(TOADONPA_DODONPA, 2);
  Engine_ActorSetAnimationAndWait(TOADONPA_DONPA, 3);
  MESSAGE_SET((donpaMessage + 3));
  MESSAGE_PUT(TOADONPA_DONPA, 0);
  OBJ_EMOTE(TOADONPA_DODONPA, EMOTE_IN_FRONT, 80);
  OBJ_MODE(TOADONPA_DONPA, 4);
  WAIT(80);
  OBJ_DIR(TOADONPA_DONPA, FACING_SOUTHEAST + FACING_STEP, 0);
  WAIT(20);
  MESSAGE_SET((donpaMessage + 4));
  MESSAGE_PUT(TOADONPA_DONPA, 0);
  Engine_ActorStartRepeatedMotion(TOADONPA_DODONPA, 1);
  MESSAGE_SET((donpaMessage + 5));
  MESSAGE_PUT(TOADONPA_DODONPA, 0);
  WAIT(30);
  Engine_ActorSetAnimationAndWait(TOADONPA_DONPA, 3);
  OBJ_FACE(TOADONPA_DONPA, TOADONPA_DODONPA, 0);
  WAIT(20);
  MESSAGE_SET((donpaMessage + 6));
  MESSAGE_PUT(TOADONPA_DONPA, 0);
  Engine_ActorStartRepeatedMotion(TOADONPA_DODONPA, 1);
  OBJ_EMOTE(TOADONPA_DODONPA, EMOTE_IN_FRONT | 2, 80);
  MESSAGE_SET((donpaMessage + 7));
  MESSAGE_PUT(TOADONPA_DODONPA, 0);
  OBJ_EMOTE(TOADONPA_DONPA, EMOTE_IN_FRONT | 3, 60);
  MESSAGE_SET((donpaMessage + 8));
  MESSAGE_PUT(TOADONPA_DONPA, 0);
  Engine_ActorSetAnimationAndWait(TOADONPA_DONPA, 3);
  MESSAGE_SET((donpaMessage + 9));
  MESSAGE_PUT(TOADONPA_DONPA, 0);
  WAIT(20);
  Actor_SetAttachedEffect(TOADONPA_DODONPA, EMOTE_IN_FRONT | 1);
  WAIT(80);
  Actor_SetAttachedEffect(TOADONPA_DODONPA, 0);
  OBJ_WALK(TOADONPA_DONPA, 496, 240);
  Engine_ActorWaitForMove(TOADONPA_DONPA);
  OBJ_FACE(TOADONPA_DONPA, TOADONPA_HAMMET, 0);
  WAIT(20);
  OBJ_FACE(TOADONPA_HAMMET, TOADONPA_DONPA, 0);
  MESSAGE_SET((donpaMessage + 10));
  MESSAGE_PUT(TOADONPA_DONPA, 0);
  Engine_ActorStartRepeatedMotion(TOADONPA_HAMMET, 1);
  MESSAGE_SET((donpaMessage + 11));
  MESSAGE_PUT(TOADONPA_HAMMET, 0);
  WAIT(40);
  Engine_ActorSetAnimationAndWait(TOADONPA_DONPA, 3);
  WAIT(20);
  MESSAGE_SET((donpaMessage + 12));
  MESSAGE_PUT(TOADONPA_DONPA, 0);
  OBJ_MODE(TOADONPA_DONPA, 3);
  OBJ_EMOTE(TOADONPA_HAMMET, EMOTE_IN_FRONT | 2, 60);
  MESSAGE_SET((donpaMessage + 13));
  MESSAGE_PUT(TOADONPA_HAMMET, 0);
  OBJ_EMOTE(TOADONPA_DONPA, EMOTE_IN_FRONT, 70);
  MESSAGE_SET((donpaMessage + 14));
  MESSAGE_PUT(TOADONPA_DONPA, 0);
  OBJ_MODE(TOADONPA_HAMMET, 3);
  WAIT(140);
  OBJ_MODE(TOADONPA_DONPA, 3);
  WAIT(120);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, TOADONPA_DONPA, 0);
  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_NOD);
  OBJ_MODE(IVAN, ANIM_NOD);
  OBJ_MODE(GARET, ANIM_NOD);
  OBJ_MODE(MIA, ANIM_NOD);
  WAIT(120);
  OBJ_EMOTE(TOADONPA_DONPA, EMOTE_IN_FRONT | 8, 180);
  MESSAGE_SET((donpaMessage + 15));
  MESSAGE_PUT(TOADONPA_DONPA, 0);
  OBJ_EMOTE(TOADONPA_HAMMET, EMOTE_IN_FRONT | 1, 80);
  MESSAGE_SET((donpaMessage + 16));
  MESSAGE_PUT(TOADONPA_HAMMET, 0);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, GARET, 0);
  Engine_ActorFaceEachOther(MIA, IVAN, 0);
  WAIT(80);
  OBJ_FACE(TOADONPA_DONPA, ACTOR_PARTY_LEADER, 0);
  WAIT(60);
  OBJ_FACE(ACTOR_PARTY_LEADER, TOADONPA_DONPA, 0);
  OBJ_FACE(GARET, TOADONPA_DONPA, 0);
  OBJ_FACE(IVAN, TOADONPA_DONPA, 0);
  OBJ_FACE(MIA, TOADONPA_DONPA, 0);
  WAIT(60);
  MESSAGE_SET((donpaMessage + 17));
  MESSAGE_PUT(TOADONPA_DONPA, 0);
  OBJ_EMOTE(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1, 0);
  OBJ_EMOTE(GARET, EMOTE_IN_FRONT | 1, 0);
  OBJ_EMOTE(IVAN, EMOTE_IN_FRONT | 1, 0);
  OBJ_EMOTE(MIA, EMOTE_IN_FRONT | 1, 0);
  WAIT(100);
  MESSAGE_SET((donpaMessage + 18));
  MESSAGE_PUT(GARET, 0);
  Engine_ActorStartRepeatedMotion(IVAN, 1);
  WAIT(60);
  MESSAGE_SET((donpaMessage + 19));
  MESSAGE_PUT(IVAN, 0);
  OBJ_FACE(TOADONPA_DONPA, IVAN, 0);
  WAIT(20);
  MESSAGE_SET((donpaMessage + 20));
  MESSAGE_PUT(TOADONPA_DONPA, 0);
  OBJ_EMOTE(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1, 0);
  OBJ_EMOTE(GARET, EMOTE_IN_FRONT | 1, 0);
  OBJ_EMOTE(IVAN, EMOTE_IN_FRONT | 1, 0);
  OBJ_EMOTE(MIA, EMOTE_IN_FRONT | 1, 0);
  WAIT(120);
  OBJ_MODE(TOADONPA_DONPA, 4);
  WAIT(120);
  MESSAGE_SET((donpaMessage + 21));
  MESSAGE_PUT(TOADONPA_DONPA, 0);
  OBJ_EMOTE(TOADONPA_DONPA, EMOTE_IN_FRONT | 2, 90);
  MESSAGE_SET((donpaMessage + 22));
  MESSAGE_PUT(TOADONPA_DONPA, 0);
  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_NOD);
  OBJ_MODE(IVAN, ANIM_NOD);
  OBJ_MODE(MIA, ANIM_NOD);
  OBJ_MODE(GARET, ANIM_NOD);
  OBJ_MODE(TOADONPA_HAMMET, 3);
  WAIT(80);
  OBJ_MODE(TOADONPA_DONPA, 4);
  WAIT(120);
  MESSAGE_SET((donpaMessage + 23));
  MESSAGE_PUT(TOADONPA_DONPA, 0);
  OBJ_EMOTE(TOADONPA_HAMMET, EMOTE_IN_FRONT, 60);
  MESSAGE_SET((donpaMessage + 24));
  MESSAGE_PUT(TOADONPA_HAMMET, 0);
  OBJ_FACE(TOADONPA_DONPA, TOADONPA_HAMMET, 0);
  WAIT(20);
  Engine_ActorStartRepeatedMotion(TOADONPA_DONPA, 1);
  WAIT(50);
  MESSAGE_SET((donpaMessage + 25));
  MESSAGE_PUT(TOADONPA_DONPA, 0);
  OBJ_MODE(GARET, ANIM_NOD);
  WAIT(70);
  MESSAGE_SET((donpaMessage + 26));
  MESSAGE_PUT(GARET, 0);
  OBJ_MODE(MIA, ANIM_NOD);
  MESSAGE_SET((donpaMessage + 27));
  MESSAGE_PUT(MIA, 0);
  OBJ_FACE(TOADONPA_DONPA, ACTOR_PARTY_LEADER, 0);
  Engine_ActorSetAnimationAndWait(TOADONPA_DONPA, 3);
  WAIT(20);
  MESSAGE_SET((donpaMessage + 28));
  MESSAGE_PUT(TOADONPA_DONPA, 0);
  OBJ_EMOTE(IVAN, EMOTE_IN_FRONT | 1, 90);
  MESSAGE_SET((donpaMessage + 29));
  MESSAGE_PUT(IVAN, 0);
  OBJ_DIR(TOADONPA_DONPA, FACING_SOUTH + FACING_STEP, 0);
  WAIT(20);
  MESSAGE_SET((donpaMessage + 30));
  MESSAGE_PUT(TOADONPA_DONPA, 0);
  Engine_ActorStartRepeatedMotion(TOADONPA_HAMMET, 1);
  WAIT(60);
  MESSAGE_SET((donpaMessage + 31));
  MESSAGE_PUT(TOADONPA_HAMMET, 0);
  OBJ_FACE(TOADONPA_DONPA, TOADONPA_HAMMET, 0);
  WAIT(20);
  Engine_ActorSetAnimationAndWait(TOADONPA_DONPA, 3);
  WAIT(20);
  MESSAGE_SET((donpaMessage + 32));
  MESSAGE_PUT(TOADONPA_DONPA, 0);
  OBJ_EMOTE(GARET, EMOTE_IN_FRONT, 70);
  MESSAGE_SET((donpaMessage + 33));
  MESSAGE_PUT(GARET, 0);
  OBJ_FACE(TOADONPA_DONPA, ACTOR_PARTY_LEADER, 0);
  WAIT(30);
  MESSAGE_SET((donpaMessage + 34));
  MESSAGE_PUT(TOADONPA_DONPA, 0);
  OBJ_EMOTE(MIA, EMOTE_IN_FRONT, 80);
  MESSAGE_SET((donpaMessage + 35));
  MESSAGE_PUT(MIA, 0);
  Engine_ActorSetAnimationAndWait(TOADONPA_DONPA, 3);
  MESSAGE_SET((donpaMessage + 36));
  MESSAGE_PUT(TOADONPA_DONPA, 0);
  OBJ_FACE(IVAN, ACTOR_PARTY_LEADER, 0);
  WAIT(30);
  OBJ_MODE(IVAN, ANIM_NOD);
  WAIT(100);
  MESSAGE_SET((donpaMessage + 37));
  MESSAGE_OPEN(IVAN, 0);
  if (Engine_EventChooseYesNo(0, 0) != 0) {
    MESSAGE_SET((donpaMessage + 38));
    MESSAGE_PUT(TOADONPA_DONPA, 0);
  }
  WAIT(30);
  OBJ_MODE(GARET, ANIM_NOD);
  OBJ_MODE(IVAN, ANIM_NOD);
  OBJ_MODE(MIA, ANIM_NOD);
  WAIT(100);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, TOADONPA_HAMMET, 0);
  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_NOD);
  OBJ_MODE(TOADONPA_HAMMET, 3);
  WAIT(100);
  // Rejoin the companions, then escort Hammet out of the fortress.
  OBJ_MODE(GARET, ANIM_WALK);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetDestination(GARET, leader->x.part.pixel, leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(GARET);
  OBJ_POS_SET(GARET, 0, 0);
  OBJ_MODE(IVAN, ANIM_WALK);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetDestination(IVAN, leader->x.part.pixel, leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(IVAN);
  OBJ_POS_SET(IVAN, 0, 0);
  OBJ_MODE(MIA, ANIM_WALK);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetDestination(MIA, leader->x.part.pixel, leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(MIA);
  OBJ_POS_SET(MIA, 0, 0);
  WAIT(30);
  OBJ_SPEED(TOADONPA_HAMMET, 0x6666, 0x3333);
  OBJ_WALK(TOADONPA_HAMMET, 472, 184);
  Engine_ActorWaitForMove(TOADONPA_HAMMET);
  OBJ_MODE(TOADONPA_HAMMET, 1);
  OBJ_DIR(TOADONPA_HAMMET, FACING_SOUTHEAST + FACING_STEP, 0);
  OBJ_FACE(TOADONPA_DONPA, ACTOR_PARTY_LEADER, 0);
  OBJ_MODE(TOADONPA_DONPA, 3);
  WAIT(20);
  OBJ_FACE(TOADONPA_DONPA, TOADONPA_DODONPA, 0);
  WAIT(20);
  MESSAGE_SET((donpaMessage + 39));
  MESSAGE_PUT(TOADONPA_DONPA, 0);
  Engine_ActorRunRepeatedMotion(TOADONPA_DODONPA, 2);
  MESSAGE_SET((donpaMessage + 40));
  MESSAGE_PUT(TOADONPA_DODONPA, 0);
  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_NOD);
  WAIT(30);
  OBJ_SPEED(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
  OBJ_SPEED(TOADONPA_HAMMET, 0x10000, 0x8000);
  Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 1);
  OBJ_WALK(ACTOR_PARTY_LEADER, 480, 248);
  WAIT(40);
  Engine_ActorSetSpritePriority(TOADONPA_HAMMET, 0);
  Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 0);
  OBJ_WALK(TOADONPA_HAMMET, 480, 216);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  OBJ_FACE(ACTOR_PARTY_LEADER, TOADONPA_DONPA, 0);
  Engine_ActorWaitForMove(TOADONPA_HAMMET);
  OBJ_FACE(TOADONPA_HAMMET, TOADONPA_DONPA, 0);
  OBJ_DIR(TOADONPA_HAMMET, FACING_SOUTHEAST + FACING_STEP, 0);
  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_NOD);
  WAIT(5);
  OBJ_MODE(TOADONPA_HAMMET, 3);
  WAIT(100);
  OBJ_MODE(TOADONPA_DONPA, 3);
  WAIT(100);
  OBJ_WALK(TOADONPA_HAMMET, 480, 248);
  OBJ_WALK(ACTOR_PARTY_LEADER, 456, 248);
  Engine_ActorWaitForMove(TOADONPA_HAMMET);
  OBJ_WALK(TOADONPA_HAMMET, 456, 248);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  OBJ_WALK(ACTOR_PARTY_LEADER, 456, 360);
  Engine_ActorWaitForMove(TOADONPA_HAMMET);
  OBJ_WALK(TOADONPA_HAMMET, 456, 360);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  OBJ_WALK(ACTOR_PARTY_LEADER, 352, 360);
  Engine_ActorWaitForMove(TOADONPA_HAMMET);
  OBJ_WALK(TOADONPA_HAMMET, 352, 360);
  WAIT(20);
  SE(MUS_017);
  gEventWork->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_CIRCLE);
  Engine_EventCloseScreen();
  WAIT(1);
  WAIT(210);
  Engine_EventRequestExit(LUNPA_FORTRESS_TOADONPA_TO_LUNPA_FORTRESS_GATES);
  EV_END();
}

/// LunpaFortressGroundFloor: Mind Read on the second sentry reads his thoughts.
/// He has one of eight thoughts, chosen at random when the scene was entered;
/// in one he looks around first.
static void Event_ReadSentrysThoughts(void) {

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
    Engine_ActorFaceDirection(GROUND_FLOOR_SECOND_SENTRY,
                              FACING_NORTH + FACING_STEP, 0);
    WAIT(50);
    Engine_ActorFaceDirection(GROUND_FLOOR_SECOND_SENTRY,
                              FACING_NORTHWEST + FACING_STEP, 0);
    WAIT(50);
    Engine_ActorFaceDirection(GROUND_FLOOR_SECOND_SENTRY,
                              FACING_SOUTH + FACING_STEP, 0);
    WAIT(50);
    MESSAGE_SET(MsgLunpaTakeCareAnybody);
    MESSAGE_PUT(GROUND_FLOOR_SECOND_SENTRY, 0);
    break;
  }
}

/// LunpaFortressGroundFloor: the second sentry looks around, hearing someone he
/// cannot see.
static void Event_TalkToSentry(void) {

  s32 messageBase;

  OBJ_EMOTE(GROUND_FLOOR_SECOND_SENTRY, EMOTE_IN_FRONT | 1, 30);
  OBJ_DIR(GROUND_FLOOR_SECOND_SENTRY, FACING_NORTH + FACING_STEP, 0);
  WAIT(50);
  OBJ_DIR(GROUND_FLOOR_SECOND_SENTRY, FACING_NORTHWEST + FACING_STEP, 0);
  WAIT(50);
  OBJ_DIR(GROUND_FLOOR_SECOND_SENTRY, FACING_SOUTH + FACING_STEP, 0);
  WAIT(50);
  messageBase = MsgLunpaLeftGuardHearsSomeone;
  MESSAGE_SET(messageBase);
  MESSAGE_PUT(GROUND_FLOOR_SECOND_SENTRY, 0);
  OBJ_MODE(GROUND_FLOOR_SECOND_SENTRY, 4);
  WAIT(60);
  OBJ_DIR(GROUND_FLOOR_SECOND_SENTRY, FACING_NORTHWEST + FACING_STEP, 0);
  WAIT(40);
  MESSAGE_SET(messageBase + 1);
  MESSAGE_PUT(GROUND_FLOOR_SECOND_SENTRY, 0);
}

/// LunpaFortressUpperFloor: Donpa's attendant. Before the rescue the attendant
/// hushes the party, glances at the sleeping Donpa, asks a question and tells
/// of Donpa and his spoiled son; once told, only a line. After the rescue,
/// Donpa is grateful.
static void Event_TalkToAttendant(void) {
  s32 whisperMessage;
  s32 familyMessage;

  EV_BEGIN();
  if (FLAG_CHECK(FLAG_DODONPA_LOCKED_UP) != 0) {
    MESSAGE_SET(MsgLunpaDonpaGrateful);
    // XXX: likely a slip for UPPER_FLOOR_ATTENDANT, because the line is the
    // attendant's and Dodonpa's wife stands at the far side of the fortress.
    MESSAGE_PUT(UPPER_FLOOR_DODONPAS_WIFE, 0);
    EV_END();
  } else {
    if (FLAG_CHECK(FLAG_PLACE_TEMP_13) != 0) {
      MESSAGE_SET(MsgLunpaMaybeDodonpasEyes);
      MESSAGE_OPEN(UPPER_FLOOR_ATTENDANT, 0);
      EV_END();
    } else {
      OBJ_EMOTE(UPPER_FLOOR_ATTENDANT, EMOTE_IN_FRONT | 2, 30);
      OBJ_FACE(UPPER_FLOOR_ATTENDANT, ACTOR_PARTY_LEADER, 0);
      whisperMessage = MsgLunpaShhhPleaseDont;
      MESSAGE_SET(whisperMessage);
      MESSAGE_PUT(UPPER_FLOOR_ATTENDANT, 0);
      OBJ_FACE(UPPER_FLOOR_ATTENDANT, UPPER_FLOOR_DONPA, 0);
      Engine_CameraMoveToActor(UPPER_FLOOR_DONPA, 1);
      Engine_CameraWaitForMove();
      WAIT(60);
      Engine_CameraMoveToActor(ACTOR_PARTY_LEADER, 1);
      WAIT(20);
      OBJ_EMOTE(UPPER_FLOOR_ATTENDANT, EMOTE_IN_FRONT | 5, 60);
      MESSAGE_SET(whisperMessage + 1);
      MESSAGE_PUT(UPPER_FLOOR_ATTENDANT, 0);
      OBJ_EMOTE(UPPER_FLOOR_ATTENDANT, EMOTE_IN_FRONT | 7, 60);
      MESSAGE_SET(whisperMessage + 2);
      MESSAGE_PUT(UPPER_FLOOR_ATTENDANT, 0);
      WAIT(70);
      OBJ_EMOTE(UPPER_FLOOR_ATTENDANT, EMOTE_IN_FRONT, 60);
      OBJ_FACE(UPPER_FLOOR_ATTENDANT, ACTOR_PARTY_LEADER, 0);
      MESSAGE_SET(whisperMessage + 3);
      MESSAGE_OPEN(UPPER_FLOOR_ATTENDANT, 0);
      if (Engine_EventChooseYesNo(0, 0) == 0) {
        MESSAGE_SET(whisperMessage + 4);
        MESSAGE_OPEN(UPPER_FLOOR_ATTENDANT, 0);
      } else {
        MESSAGE_SET(whisperMessage + 5);
        MESSAGE_OPEN(UPPER_FLOOR_ATTENDANT, 0);
      }
      WAIT(60);
      OBJ_EMOTE(UPPER_FLOOR_ATTENDANT, EMOTE_IN_FRONT | 5, 60);
      familyMessage = MsgLunpaDonpaKnowsCoddled;
      MESSAGE_SET(familyMessage);
      MESSAGE_OPEN(UPPER_FLOOR_ATTENDANT, 0);
      Engine_ActorRunRepeatedMotion(UPPER_FLOOR_ATTENDANT, 1);
      MESSAGE_SET(familyMessage + 1);
      MESSAGE_OPEN(UPPER_FLOOR_ATTENDANT, 0);
      Engine_ActorSetAnimationAndWait(UPPER_FLOOR_ATTENDANT, 3);
      MESSAGE_SET(familyMessage + 2);
      MESSAGE_OPEN(UPPER_FLOOR_ATTENDANT, 0);
      FLAG_SET(FLAG_PLACE_TEMP_13);
      EV_END();
    }
  }
}

/// LunpaFortressUpperFloor: Mind Read on Donpa's attendant.
static void Event_ReadAttendantsThoughts(void) {
  if (Engine_GameFlagIsSet(FLAG_DODONPA_LOCKED_UP)) {
    MESSAGE_SET(MsgLunpaDifficultDonpaRight);
    MESSAGE_PUT(UPPER_FLOOR_ATTENDANT, 0);
  } else {
    MESSAGE_SET(MsgLunpaSomeonePunishDodonpa);
    MESSAGE_PUT(UPPER_FLOOR_ATTENDANT, 0);
  }
}

/// LunpaFortressUpperFloor: Mind Read on Donpa.
static void Event_ReadDonpasThoughts(void) {
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
static void Event_TalkToDonpa(void) {
  s32 rewardMessage;

  if (FLAG_CHECK(FLAG_DODONPA_LOCKED_UP) != 0) {
    if (FLAG_CHECK(FLAG_LUNPA_FORTRESS_REWARD_TAKEN) == 0 &&
        FLAG_CHECK(FLAG_PLACE_TEMP_14) == 0) {
      rewardMessage = MsgLunpaThankHelpDodonpa;
      MESSAGE_SET(rewardMessage);
      MESSAGE_PUT(UPPER_FLOOR_DONPA, 0);
      Engine_ActorRunRepeatedMotion(UPPER_FLOOR_DONPA, 1);
      WAIT(30);
      OBJ_SPEED(UPPER_FLOOR_DONPA, 6553, 3276);
      Actor_SetDestinationOffset(UPPER_FLOOR_DONPA, -4, 0);
      Engine_ActorWaitForMove(UPPER_FLOOR_DONPA);
      OBJ_MODE(UPPER_FLOOR_DONPA, 3);
      WAIT(60);
      OBJ_SPEED(UPPER_FLOOR_DONPA, 13107, 6553);
      Actor_SetDestinationOffset(UPPER_FLOOR_DONPA, -6, 0);
      OBJ_FACE(UPPER_FLOOR_DONPA, ACTOR_PARTY_LEADER, 0);
      Engine_ActorWaitForMove(UPPER_FLOOR_DONPA);
      MESSAGE_SET(rewardMessage + 1);
      MESSAGE_PUT(UPPER_FLOOR_DONPA, 0);
      Engine_ActorRunRepeatedMotion(UPPER_FLOOR_DONPA, 1);
      OBJ_FACE(UPPER_FLOOR_ATTENDANT, UPPER_FLOOR_DONPA, 0);
      MESSAGE_SET(rewardMessage + 2);
      MESSAGE_PUT(UPPER_FLOOR_DONPA, 0);
      WAIT(70);
      OBJ_MODE(UPPER_FLOOR_ATTENDANT, 3);
      WAIT(60);
      OBJ_SPEED(UPPER_FLOOR_ATTENDANT, 26214, 13107);
      OBJ_WALK(UPPER_FLOOR_ATTENDANT, 880, 112);
      Engine_ActorWaitForMove(UPPER_FLOOR_ATTENDANT);
      OBJ_DIR(UPPER_FLOOR_ATTENDANT, FACING_NORTH + FACING_STEP, 0);
      MESSAGE_SET(rewardMessage + 3);
      MESSAGE_PUT(UPPER_FLOOR_DONPA, 0);
      OBJ_MODE(UPPER_FLOOR_DONPA, 3);
      WAIT(70);
      Actor_SetDestinationOffset(UPPER_FLOOR_DONPA, 8, 0);
      Engine_ActorWaitForMove(UPPER_FLOOR_DONPA);
      OBJ_MODE(UPPER_FLOOR_DONPA, 5);
      MESSAGE_SET(rewardMessage + 4);
      MESSAGE_PUT(UPPER_FLOOR_DONPA, 0);
      OBJ_WALK(ACTOR_PARTY_LEADER, 896, 120);
      Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
      Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, UPPER_FLOOR_ATTENDANT, 0);
      WAIT(60);
      OBJ_MODE(UPPER_FLOOR_ATTENDANT, 3);
      WAIT(30);
      FLAG_SET(FLAG_PLACE_TEMP_14);
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
static void Event_DjinniJoins(void) {
  Engine_DjinnJoinParty(UPPER_FLOOR_TONIC, 1, 5);
  Engine_GameFlagSet(FLAG_LUNPA_FORTRESS_REWARD_TAKEN);
}

/// LunpaFortressUpperFloor: the third of the beaten guards begs. "Owww! Hey!
/// Don't hurt me again..."
static void Event_TalkToBeatenGuardC(void) {
  Engine_ActorRunRepeatedMotion(UPPER_FLOOR_GUARD_C, 2);
  MESSAGE_SET(MsgLunpaOwwwDontHurt);
  MESSAGE_PUT(UPPER_FLOOR_GUARD_C, 0);
}

/// LunpaFortressUpperFloor: the second of the beaten guards gives up, and
/// Hammet is in the cave. "We give up!"
static void Event_TalkToBeatenGuardB(void) {
  Engine_ActorRunRepeatedMotion(UPPER_FLOOR_GUARD_B, 2);
  MESSAGE_SET(MsgLunpaRightRightGive);
  MESSAGE_PUT(UPPER_FLOOR_GUARD_B, 0);
}

/// LunpaFortressUpperFloor: the first of the beaten guards marvels at the
/// party. "Who'd have thought they'd be so tough?"
static void Event_TalkToBeatenGuardA(void) {
  Engine_ActorRunRepeatedMotion(UPPER_FLOOR_GUARD_A, 2);
  MESSAGE_SET(MsgLunpaGuysTougherThought);
  MESSAGE_PUT(UPPER_FLOOR_GUARD_A, 0);
}

/// LunpaFortressUpperFloor: one of Dodonpa's family asks where he is. "Do you
/// know where Dodonpa is?"
static void Event_AskWhereDodonpaIs(void) {
  MESSAGE_SET(MsgLunpaKnowWhereDodonpa);
  Engine_EventAskYesNo(UPPER_FLOOR_DODONPAS_WIFE, 0);
}

/// LunpaFortressGroundFloor: a bent old man asks whether the party sneaked in.
/// He's startled the first time, and wants to know whether it came in search of
/// something; on yes, he promises to keep quiet. "Wh-Wh-Who goes there!"
static void Event_TalkToStartledOldMan(void) {
  s32 frightMessage;
  s32 purposeMessage;

  if (FLAG_CHECK(FLAG_SCENE_TEMP_26) != 0) {
    MESSAGE_SET(MsgLunpaWontTellAnyone);
    MESSAGE_PUT(GROUND_FLOOR_STARTLED_OLD_MAN, 0);
  } else {
    EV_BEGIN();
    OBJ_FACE(GROUND_FLOOR_STARTLED_OLD_MAN, ACTOR_PARTY_LEADER, 0);
    if (FLAG_CHECK(FLAG_SCENE_TEMP_27) == 0) {
      Engine_ActorJump(GROUND_FLOOR_STARTLED_OLD_MAN, 4, 0);
      Engine_ActorStop(GROUND_FLOOR_STARTLED_OLD_MAN);
      Object_RefreshSelectorById(GROUND_FLOOR_STARTLED_OLD_MAN);
      WAIT(20);
      frightMessage = MsgLunpaWhWhWho;
      MESSAGE_SET(frightMessage);
      MESSAGE_PUT(GROUND_FLOOR_STARTLED_OLD_MAN, 0);
      OBJ_EMOTE(GROUND_FLOOR_STARTLED_OLD_MAN, EMOTE_IN_FRONT | 2, 30);
      MESSAGE_SET(frightMessage + 1);
      MESSAGE_PUT(GROUND_FLOOR_STARTLED_OLD_MAN, 0);
      WAIT(30);
      OBJ_MODE(GROUND_FLOOR_STARTLED_OLD_MAN, 4);
      WAIT(30);
    }
    purposeMessage = MsgLunpaDontLookNearly;
    MESSAGE_SET(purposeMessage);
    MESSAGE_PUT(GROUND_FLOOR_STARTLED_OLD_MAN, 0);
    OBJ_EMOTE(GROUND_FLOOR_STARTLED_OLD_MAN, EMOTE_IN_FRONT | 1, 40);
    MESSAGE_SET(purposeMessage + 1);
    MESSAGE_OPEN(GROUND_FLOOR_STARTLED_OLD_MAN, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
      MESSAGE_SET(purposeMessage + 2);
      MESSAGE_OPEN(GROUND_FLOOR_STARTLED_OLD_MAN, 0);
      FLAG_SET(FLAG_SCENE_TEMP_26);
    } else {
      MESSAGE_SET(purposeMessage + 3);
      MESSAGE_OPEN(GROUND_FLOOR_STARTLED_OLD_MAN, 0);
    }
    FLAG_SET(FLAG_SCENE_TEMP_27);
    EV_END();
  }
}

/// LunpaFortressGroundFloor: Mind Read on the startled resident. Once they have
/// promised, a thought about Hammet; before, Mind Read stops and they talk
/// instead.
static void Event_ReadStartledResidentsThoughts(void) {

  if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_26)) {
    MESSAGE_SET(MsgLunpaMaybeMerchantReason);
    MESSAGE_PUT(GROUND_FLOOR_STARTLED_OLD_MAN, 0);
  } else {
    s16 *psynergyRequest = &gEventWork->psynergyRequest;

    *psynergyRequest = 0;
    Engine_PsynergyCancel();
    Event_TalkToStartledOldMan();
  }
}

static void UNUSED LunpaFortress_DoNothingF(void) {}

/// LunpaFortressToadonpa, after Donpa's judgment: Dodonpa behind bars begs his
/// father's forgiveness; with the Cell Key, a further line shows.
static void Event_TalkToJailedDodonpa(void) {
  s32 pleaMessage = MsgLunpaWrongTurnOver;

  MESSAGE_SET(pleaMessage);
  MESSAGE_PUT(SPEAKER_WINDOW_BELOW | TOADONPA_DODONPA, 0);
  if (Engine_PartyInventoryFindOwner(ITEM_CELL_KEY) != -1) {
    Engine_MessageShowCentered(pleaMessage + 2, 1);
  }
}

/// LunpaFortressToadonpa: Mind Read on the jailed Dodonpa.
static void Event_ReadJailedDodonpasThoughts(void) {
  MESSAGE_SET(MsgLunpaCantBelieveWhen);
  MESSAGE_PUT(TOADONPA_DODONPA, 0);
}

/// Sets each scene up: a random pick for the guards' lines, the doors already
/// opened, the watching guards and, in LunpaFortressToadonpa, the scene after
/// the battle with Toadonpa or Dodonpa jailed.
s32 Scene_Initialize(void) {
  struct FieldActor *sceneActor;

  gLunpaFortressRandomPick = (u32)Engine_RandomNext() * 7 >> 16;
  if (gGameState.scene == (s32)&SceneId_LunpaFortressGroundFloor) {
    Engine_MapSetWorkFlagBits9To11(0xe00);
    LunpaFortress_InitializeGroundFloor();
  }
  if (gGameState.scene == (s32)&SceneId_LunpaFortressUpperFloor)
    LunpaFortress_InitializeUpperFloor();
  if (gGameState.scene == (s32)&SceneId_LunpaFortressCave)
    LunpaFortress_InitializeCave();
  if (gGameState.scene == (s32)&SceneId_LunpaFortressToadonpa) {
    gEventWork->startTransition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
    Engine_ActorSetSpriteFlags(Actor_Get(TOADONPA_HAMMET), 0);
    OBJ_DIR(TOADONPA_HAMMET, FACING_EAST, 0);
    OBJ_MODE(TOADONPA_HAMMET, 0);
    LunpaFortress_RestoreToadonpaDoors();
    sceneActor = Actor_Get(TOADONPA_WEST_INVISIBLE_ACTOR);
    if (sceneActor != 0)
      Engine_ActorSetSpriteFlags(sceneActor, 0);
    sceneActor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    sceneActor = Actor_Get(TOADONPA_MIDDLE_INVISIBLE_ACTOR);
    if (sceneActor != 0)
      Engine_ActorSetSpriteFlags(sceneActor, 0);
    sceneActor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    sceneActor = Actor_Get(TOADONPA_EAST_INVISIBLE_ACTOR);
    if (sceneActor != 0)
      Engine_ActorSetSpriteFlags(sceneActor, 0);
    sceneActor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    Engine_MapSetWorkFlagBits9To11(0xe00);
    if (gGameState.entrance == LUNPA_FORTRESS_TOADONPA_AFTER_BATTLE) {
      Engine_MapSetWorkFlagBits9To11(0xc00);
      Event_DonpaJudgesDodonpa();
    }
    if (gGameState.entrance ==
        LUNPA_FORTRESS_TOADONPA_FROM_LUNPA_FORTRESS_CAVE_NORTHWEST) {
      Engine_MapSetWorkFlagBits9To11(0xc00);
      if (FLAG_CHECK(FLAG_DODONPA_LOCKED_UP) != 0) {
        OBJ_POS_SET(TOADONPA_HAMMET, 0, 0);
        OBJ_POS_SET(TOADONPA_CELL_GUARD, 432, 344);
        OBJ_DIR(TOADONPA_CELL_GUARD, FACING_SOUTH + FACING_STEP, 0);
        OBJ_POS_SET(TOADONPA_DODONPA, 456, 288);
        OBJ_DIR(TOADONPA_DODONPA, FACING_SOUTH + FACING_STEP, 0);
        OBJ_POS_SET(TOADONPA_DODONPA_STAND_IN, 456, 320);
        Engine_ActorSetSpriteFlags(Actor_Get(TOADONPA_DODONPA_STAND_IN), 0);
      }
    }
    sceneActor = Actor_Get(TOADONPA_BOSS_PART);
    if (sceneActor != 0)
      Engine_ActorSetSpriteFlags(sceneActor, 0);
    sceneActor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    sceneActor->scaleX = 0xcccc;
  }
  return 0;
}

/// Sets LunpaFortressGroundFloor up by entrance: the guards that watch each
/// part, and the cell doors already opened.
static void LunpaFortress_InitializeGroundFloor(void) {
  LunpaFortress_RestoreGroundFloorDoors();
  switch (gGameState.entrance) {
  case LUNPA_FORTRESS_GROUND_FLOOR_ENTRANCE_FROM_MAIN_HALL:
  case LUNPA_FORTRESS_GROUND_FLOOR_MAIN_HALL_FROM_ENTRANCE:
  case LUNPA_FORTRESS_GROUND_FLOOR_MAIN_HALL_FROM_NORTH_HALL:
  case LUNPA_FORTRESS_GROUND_FLOOR_MAIN_HALL_FROM_DINING_ROOM:
  case LUNPA_FORTRESS_GROUND_FLOOR_MAIN_HALL_FROM_NORTHEAST_HALL:
  case LUNPA_FORTRESS_GROUND_FLOOR_MAIN_HALL_FROM_EAST_HALL:
    gEventWork->startTransition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
    Task_AddCallback(LunpaFortress_CheckEntranceGuard, TASK_PRIORITY_SCENE);
    Task_AddCallback(LunpaFortress_CheckFirstEntranceSentry,
                     TASK_PRIORITY_SCENE);
    Task_AddCallback(LunpaFortress_CheckSecondEntranceSentry,
                     TASK_PRIORITY_SCENE);
    Engine_MapSetWorkFlagBits9To11(0xe00);
    break;
  case LUNPA_FORTRESS_GROUND_FLOOR_DINING_ROOM_FROM_MAIN_HALL:
  case LUNPA_FORTRESS_GROUND_FLOOR_STOREROOM_FROM_NORTHEAST_HALL:
    gEventWork->startTransition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
    Engine_MapSetWorkFlagBits9To11(0xc00);
    break;
  case LUNPA_FORTRESS_GROUND_FLOOR_EAST_HALL_FROM_MAIN_HALL:
  case LUNPA_FORTRESS_GROUND_FLOOR_EAST_HALL_FROM_NORTHEAST_HALL:
  case LUNPA_FORTRESS_GROUND_FLOOR_EAST_HALL_FROM_TREASURE_ROOM:
    gEventWork->startTransition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
    Task_AddCallback(LunpaFortress_UpdateGroundFloorPatrolDetection,
                     TASK_PRIORITY_SCENE);
    Task_AddCallback(LunpaFortress_CheckGroundFloorDoorApproach,
                     TASK_PRIORITY_SCENE);
    Engine_TaskWait(1);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Map_CopyCells(101, 9, 10, 8, 110, 9);
    Engine_MapSetWorkFlagBits9To11(0xe00);
    break;
  case LUNPA_FORTRESS_GROUND_FLOOR_NORTHEAST_HALL_FROM_MAIN_HALL:
  case LUNPA_FORTRESS_GROUND_FLOOR_NORTHEAST_HALL_FROM_EAST_HALL:
  case LUNPA_FORTRESS_GROUND_FLOOR_NORTHEAST_HALL_FROM_STOREROOM:
    gEventWork->startTransition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
    Task_AddCallback(LunpaFortress_CheckUpperHallGuard, TASK_PRIORITY_SCENE);
    break;
  default:
    gEventWork->startTransition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
    Engine_MapSetWorkFlagBits9To11(0xe00);
    break;
  }
  Engine_ActorSetChildValue(GROUND_FLOOR_PATROL_GUARD, 1);
  Engine_ActorSetChildValue(GROUND_FLOOR_UPPER_HALL_GUARD, 1);
  Engine_ActorSetChildValue(GROUND_FLOOR_SECOND_SENTRY, 1);
  Engine_ActorSetChildValue(GROUND_FLOOR_ENTRANCE_GUARD, 1);
  Engine_ActorSetChildValue(GROUND_FLOOR_FIRST_SENTRY, 1);
  Engine_TaskWait(1);
}

/// Sets LunpaFortressUpperFloor up: beaten guards gone or lying where they
/// fell, the Cell Key gone once taken, the patrols by entrance and Donpa's
/// attendant by his reward's place.
static void LunpaFortress_InitializeUpperFloor(void) {
  struct FieldActor *keyActor;

  LunpaFortress_RestoreUpperFloorDoors();
  Engine_ActorSetChildValue(UPPER_FLOOR_FIRST_PATROL, 1);
  Engine_ActorSetChildValue(UPPER_FLOOR_SECOND_PATROL, 1);
  Engine_ActorSetChildValue(UPPER_FLOOR_HALL_PATROL, 1);
  if (FLAG_CHECK(FLAG_LUNPA_FORTRESS_WEST_GUARD_BEATEN)) {
    OBJ_POS_SET(UPPER_FLOOR_WEST_GUARD, 0, 0);
  }
  if (FLAG_CHECK(FLAG_LUNPA_FORTRESS_MIDDLE_GUARD_BEATEN)) {
    OBJ_POS_SET(UPPER_FLOOR_HALL_GUARD, 0, 0);
  }
  if (FLAG_CHECK(FLAG_LUNPA_FORTRESS_EAST_GUARD_BEATEN)) {
    OBJ_POS_SET(UPPER_FLOOR_EAST_GUARD, 0, 0);
  }
  if (FLAG_CHECK(FLAG_LUNPA_FORTRESS_CELL_KEY)) {
    OBJ_POS_SET(UPPER_FLOOR_CELL_KEY, 0, 0);
  }
  switch (gGameState.entrance) {
  case LUNPA_FORTRESS_UPPER_FLOOR_FROM_LUNPA_FORTRESS_GROUND_FLOOR_SOUTH:
  case LUNPA_FORTRESS_UPPER_FLOOR_WEST_HALL_FROM_NORTH_HALL:
  case LUNPA_FORTRESS_UPPER_FLOOR_WEST_HALL_FROM_CENTRAL_HALL:
    gEventWork->startTransition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
    Engine_MapSetWorkFlagBits9To11(0xe00);
    Task_AddCallback(LunpaFortress_UpdatePairedGuardDetection,
                     TASK_PRIORITY_SCENE);
    Engine_TaskWait(1);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    break;
  case LUNPA_FORTRESS_UPPER_FLOOR_FAMILY_ROOM_FROM_CENTRAL_HALL:
  case LUNPA_FORTRESS_UPPER_FLOOR_WOMENS_ROOM_FROM_NORTH_HALL:
  case LUNPA_FORTRESS_UPPER_FLOOR_DONPAS_ROOM_FROM_SOUTH_HALL:
  case LUNPA_FORTRESS_UPPER_FLOOR_TONIC_ROOM_FROM_DONPAS_ROOM:
  case LUNPA_FORTRESS_UPPER_FLOOR_DONPAS_ROOM_FROM_TONIC_ROOM:
    gEventWork->startTransition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
    Engine_MapSetWorkFlagBits9To11(0xc00);
    Engine_ActorSetSpriteFlags(Actor_Get(UPPER_FLOOR_DONPA), 0);
    if (FLAG_CHECK(FLAG_PLACE_TEMP_14)) {
      OBJ_POS_SET(UPPER_FLOOR_ATTENDANT, 872, 120);
    }
    break;
  case LUNPA_FORTRESS_UPPER_FLOOR_EAST_HALL_FROM_CELL_KEY_ROOM:
  case LUNPA_FORTRESS_UPPER_FLOOR_EAST_HALL_FROM_SOUTH_HALL:
    gEventWork->startTransition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
    Engine_MapSetWorkFlagBits9To11(0xe00);
    Task_AddCallback(LunpaFortress_UpdateHallGuardDetection,
                     TASK_PRIORITY_SCENE);
    Engine_TaskWait(1);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    break;
  case LUNPA_FORTRESS_UPPER_FLOOR_GUARD_ROOM_FROM_NORTH_HALL:
  case LUNPA_FORTRESS_UPPER_FLOOR_GUARD_ROOM_FROM_SOUTH_HALL:
    gEventWork->startTransition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
    if (FLAG_CHECK(FLAG_LUNPA_FORTRESS_THREE_GUARDS_BEATEN)) {
      LunpaFortress_PlaceThreeBeatenGuards();
    }
    break;
  case LUNPA_FORTRESS_UPPER_FLOOR_GUARD_ROOM_AFTER_BATTLE:
    gEventWork->startTransition =
        SCENE_TRANSITION(TRANSITION_WINDOW, LUNPA_RETURN_TRANSITION_VARIANT);
    LunpaFortress_PlaceThreeBeatenGuards();
    break;
  case LUNPA_FORTRESS_UPPER_FLOOR_CELL_KEY_ROOM_FROM_CENTRAL_HALL:
  case LUNPA_FORTRESS_UPPER_FLOOR_CELL_KEY_ROOM_FROM_EAST_HALL:
  case LUNPA_FORTRESS_UPPER_FLOOR_FROM_LUNPA_FORTRESS_GROUND_FLOOR_NORTH:
    Task_AddCallback(LunpaFortress_CheckUpperFloorDoorApproach,
                     TASK_PRIORITY_SCENE);
    break;
  default:
    gEventWork->startTransition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
    Engine_MapSetWorkFlagBits9To11(0xe00);
    break;
  }
  keyActor = Actor_Get(UPPER_FLOOR_CELL_KEY);
  Engine_ActorSetSpriteFlags(Actor_Get(UPPER_FLOOR_CELL_KEY), 0);
  Engine_ActorSetSpritePriority(UPPER_FLOOR_CELL_KEY, 1);
  keyActor->scaleX = 0xc000;
  keyActor->scaleY = 0xc000;
}

/// Sets LunpaFortressCave up: the doors, the crates, the Whirlwind covers and
/// the switches as the party left them.
static void LunpaFortress_InitializeCave(void) {
  struct FieldActor *sceneActor;

  gEventWork->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
  LunpaFortress_RestoreCaveDoors();
  if (FLAG_CHECK(FLAG_LUNPA_FORTRESS_CRATES_MOVED)) {
    LunpaFortress_RestoreLoweredCrates();
  }
  FLAG_SET(FLAG_SCENE_TEMP_17);
  FLAG_SET(FLAG_SCENE_TEMP_18);
  if (FLAG_CHECK(FLAG_LUNPA_FORTRESS_FIRST_SWITCH_PLANT_BLOWN_AWAY)) {
    OBJ_POS_SET(CAVE_FIRST_SWITCH_COVER, 0, 0);
    Engine_GameFlagClear(FLAG_SCENE_TEMP_17);
  }
  if (FLAG_CHECK(FLAG_LUNPA_FORTRESS_PASSAGE_PLANT_BLOWN_AWAY)) {
    OBJ_POS_SET(CAVE_PASSAGE_COVER, 0, 0);
    LunpaFortress_OpenCoveredPassage();
  }
  if (FLAG_CHECK(FLAG_LUNPA_FORTRESS_SECOND_SWITCH_PLANT_BLOWN_AWAY)) {
    OBJ_POS_SET(CAVE_SECOND_SWITCH_COVER, 0, 0);
    Engine_GameFlagClear(FLAG_SCENE_TEMP_18);
  }
  if (FLAG_CHECK(FLAG_LUNPA_FORTRESS_FIRST_SWITCH_FLIPPED)) {
    LunpaFortress_OpenFirstSwitchPassage();
  }
  if (FLAG_CHECK(FLAG_LUNPA_FORTRESS_SECOND_SWITCH_FLIPPED)) {
    LunpaFortress_OpenSecondSwitchPassage();
  }
  EV_BEGIN();
  sceneActor = Actor_Get(CAVE_FIRST_SWITCH_COVER);
  if (sceneActor != 0) {
    sceneActor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  }
  sceneActor = Actor_Get(CAVE_PASSAGE_COVER);
  if (sceneActor != 0) {
    sceneActor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  }
  sceneActor = Actor_Get(CAVE_SECOND_SWITCH_COVER);
  if (sceneActor != 0) {
    sceneActor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  }
  sceneActor = Actor_Get(CAVE_INVISIBLE_ACTOR);
  if (sceneActor != 0) {
    Engine_ActorSetSpriteFlags(sceneActor, 0);
  }
  sceneActor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  sceneActor = Actor_Get(CAVE_CRATES);
  if (sceneActor != 0) {
    sceneActor->collisionFlags |= 0x10;
  }
  Engine_ActorSetSpriteFlags(Actor_Get(CAVE_INVISIBLE_ACTOR), 0);
  EV_END();
  Engine_MapSetWorkFlagBits9To11(0xe00);
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
// field.h and object_staged_actor.h lack const, so a section attribute places
// each one.
s32 gStagedActor_DirectionSteps[] __attribute__((section(".rodata"))) = {
    DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(16, 0),
};

// clang-format off
u32 gStagedActor_FootprintKinds[] __attribute__((section(".rodata"))) = {
    207, 205, 228, 229, 298, 297,
};

s32 gStagedActor_FootprintBounds[] __attribute__((section(".rodata"))) = {
    -32, -8, 32, 8,
    -8, -32, 8, 32,
    -32, -16, 32, 0,
    -8, -32, 8, 32,
    -32, -8, 32, 8,
    -8, -32, 8, 32,
};

s32 gLunpaFortressRandomPick = 0;

s32 gSecondPatrolRoute[] = {
    SC_SET(SW_ACCEL, 0x11999),
    SC_SET(SW_SPEED, 0x11999),
    SC_ARG(2),
    SC_MOVE(PIXELS(192), 0, PIXELS(288)),
    SC_SYNC,
    SC_ARG(1),
    SC_ARG(2),
    SC_MOVE(PIXELS(88), 0, PIXELS(288)),
    SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gFirstPatrolRoute[] = {
    SC_SET(SW_ACCEL, 0x11999),
    SC_SET(SW_SPEED, 0x11999),
    SC_ARG(2),
    SC_MOVE(PIXELS(192), 0, PIXELS(192)),
    SC_SYNC,
    SC_ARG(1),
    SC_ARG(2),
    SC_MOVE(PIXELS(88), 0, PIXELS(192)),
    SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gDonpaSleeps[] = {
    SC_ARG(5),
    SC_END,
};

s32 gGroundFloorPatrolRoute[] = {
    SC_SET(SW_ACCEL, 0x3333),
    SC_SET(SW_SPEED, 0x9999),
    SC_ARG(2),
    SC_MOVE(PIXELS(808), 0, PIXELS(648)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(25),
    SC_ARG(2),
    SC_MOVE(PIXELS(808), 0, PIXELS(520)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(20),
    SC_ARG(2),
    SC_MOVE(PIXELS(712), 0, PIXELS(520)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(25),
    SC_ARG(2),
    SC_MOVE(PIXELS(808), 0, PIXELS(520)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(40),
    SC_ARG(2),
    SC_MOVE(PIXELS(808), 0, PIXELS(648)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(20),
    SC_ARG(2),
    SC_MOVE(PIXELS(744), 0, PIXELS(648)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(30),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gHallPatrolRoute[] = {
    SC_SET(SW_ACCEL, 0x3333),
    SC_SET(SW_SPEED, 0x9999),
    SC_ARG(2),
    SC_MOVE(PIXELS(920), 0, PIXELS(648)),
    SC_SYNC,
    SC_ARG(1),
    SC_ARG(2),
    SC_MOVE(PIXELS(840), 0, PIXELS(648)),
    SC_SYNC,
    SC_ARG(1),
    SC_ARG(2),
    SC_MOVE(PIXELS(840), 0, PIXELS(592)),
    SC_SYNC,
    SC_ARG(1),
    SC_ARG(2),
    SC_MOVE(PIXELS(840), 0, PIXELS(648)),
    SC_SYNC,
    SC_ARG(1),
    SC_ARG(2),
    SC_MOVE(PIXELS(920), 0, PIXELS(648)),
    SC_SYNC,
    SC_ARG(1),
    SC_ARG(2),
    SC_MOVE(PIXELS(920), 0, PIXELS(592)),
    SC_SYNC,
    SC_ARG(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

struct SceneEntrance gLunpaFortressGroundFloorEntrances[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 168, 0, 632, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { LUNPA_FORTRESS_GROUND_FLOOR_FROM_LUNPA_FORTRESS_GATES, CONDITION_ALWAYS, 160, 0, 168, FACING_NORTH, 0, 40, 24, 296, 280, 0 },
    { LUNPA_FORTRESS_GROUND_FLOOR_ENTRANCE_FROM_MAIN_HALL, CONDITION_ALWAYS, 160, 0, 104, FACING_SOUTH, 0, 40, 24, 296, 280, 0 },
    { LUNPA_FORTRESS_GROUND_FLOOR_MAIN_HALL_FROM_ENTRANCE, CONDITION_ALWAYS, 272, 0, 896, FACING_NORTH, 0, 24, 616, 536, 936, 0 },
    { LUNPA_FORTRESS_GROUND_FLOOR_MAIN_HALL_FROM_NORTH_HALL, CONDITION_ALWAYS, 376, 0, 896, FACING_NORTH, 0, 24, 616, 536, 936, 0 },
    { LUNPA_FORTRESS_GROUND_FLOOR_MAIN_HALL_FROM_DINING_ROOM, CONDITION_ALWAYS, 248, 0, 856, FACING_SOUTH, 0, 24, 616, 536, 936, 0 },
    { LUNPA_FORTRESS_GROUND_FLOOR_MAIN_HALL_FROM_NORTHEAST_HALL, CONDITION_ALWAYS, 488, 0, 680, FACING_SOUTH, 0, 24, 616, 536, 936, 0 },
    { LUNPA_FORTRESS_GROUND_FLOOR_MAIN_HALL_FROM_EAST_HALL, CONDITION_ALWAYS, 72, 0, 696, FACING_SOUTH, 0, 24, 616, 536, 936, 0 },
    { LUNPA_FORTRESS_GROUND_FLOOR_FROM_LUNPA_FORTRESS_UPPER_FLOOR_SOUTH, CONDITION_ALWAYS, 96, 0, 856, FACING_SOUTH, 0, 24, 616, 536, 936, 0 },
    { LUNPA_FORTRESS_GROUND_FLOOR_NORTH_HALL_FROM_MAIN_HALL, CONDITION_ALWAYS, 376, 0, 184, FACING_SOUTH, 0, 328, 8, 568, 232, 0 },
    { LUNPA_FORTRESS_GROUND_FLOOR_FROM_LUNPA_FORTRESS_UPPER_FLOOR_NORTH, CONDITION_ALWAYS, 504, 0, 88, FACING_SOUTH, 0, 328, 8, 568, 232, 0 },
    { LUNPA_FORTRESS_GROUND_FLOOR_FROM_LUNPA_FORTRESS_CAVE, CONDITION_ALWAYS, 440, 0, 104, FACING_SOUTH, 0, 328, 8, 568, 232, 0 },
    { LUNPA_FORTRESS_GROUND_FLOOR_DINING_ROOM_FROM_MAIN_HALL, CONDITION_ALWAYS, 136, 0, 512, FACING_NORTH, 0, 40, 328, 280, 552, 0 },
    { LUNPA_FORTRESS_GROUND_FLOOR_NORTHEAST_HALL_FROM_MAIN_HALL, CONDITION_ALWAYS, 888, 0, 280, FACING_NORTH, 0, 632, 40, 952, 328, 0 },
    { LUNPA_FORTRESS_GROUND_FLOOR_NORTHEAST_HALL_FROM_EAST_HALL, CONDITION_ALWAYS, 696, 0, 264, FACING_SOUTH, 0, 632, 40, 952, 328, 0 },
    { LUNPA_FORTRESS_GROUND_FLOOR_NORTHEAST_HALL_FROM_STOREROOM, CONDITION_ALWAYS, 824, 0, 120, FACING_SOUTH, 0, 632, 40, 952, 328, 0 },
    { LUNPA_FORTRESS_GROUND_FLOOR_EAST_HALL_FROM_MAIN_HALL, CONDITION_ALWAYS, 664, 0, 632, FACING_NORTH, 0, 616, 344, 936, 696, 0 },
    { LUNPA_FORTRESS_GROUND_FLOOR_EAST_HALL_FROM_NORTHEAST_HALL, CONDITION_ALWAYS, 888, 0, 520, FACING_NORTH, 0, 616, 344, 936, 696, 0 },
    { LUNPA_FORTRESS_GROUND_FLOOR_EAST_HALL_FROM_TREASURE_ROOM, CONDITION_ALWAYS, 696, 0, 432, FACING_SOUTH, 0, 616, 344, 936, 696, 0 },
    { LUNPA_FORTRESS_GROUND_FLOOR_STOREROOM_FROM_NORTHEAST_HALL, CONDITION_ALWAYS, 440, 0, 448, FACING_NORTH, 0, 320, 312, 560, 504, 0 },
    { LUNPA_FORTRESS_GROUND_FLOOR_TREASURE_ROOM_FROM_EAST_HALL, CONDITION_ALWAYS, 696, 0, 824, FACING_NORTH, 0, 616, 744, 856, 952, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gLunpaFortressUpperFloorEntrances[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 168, 0, 632, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_FROM_LUNPA_FORTRESS_GROUND_FLOOR_SOUTH, CONDITION_ALWAYS, 136, 0, 392, FACING_SOUTH, 0, 40, 8, 308, 448, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_WEST_HALL_FROM_NORTH_HALL, CONDITION_ALWAYS, 136, 0, 80, FACING_SOUTH, 0, 40, 8, 308, 448, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_WEST_HALL_FROM_CENTRAL_HALL, CONDITION_ALWAYS, 264, 0, 416, FACING_SOUTH, 0, 40, 8, 308, 448, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_NORTH_HALL_FROM_WEST_HALL, CONDITION_ALWAYS, 376, 0, 232, FACING_NORTH, 0, 328, 8, 568, 272, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_NORTH_HALL_FROM_GUARD_ROOM, CONDITION_ALWAYS, 504, 0, 104, FACING_NORTH, 0, 328, 8, 568, 272, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_NORTH_HALL_FROM_WOMENS_ROOM, CONDITION_ALWAYS, 376, 0, 176, FACING_SOUTH, 0, 328, 8, 568, 272, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_CENTRAL_HALL_FROM_WEST_HALL, CONDITION_ALWAYS, 408, 0, 456, FACING_NORTH, 0, 328, 272, 584, 512, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_CENTRAL_HALL_FROM_FAMILY_ROOM, CONDITION_ALWAYS, 456, 0, 352, FACING_SOUTH, 0, 328, 272, 584, 512, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_CENTRAL_HALL_FROM_CELL_KEY_ROOM, CONDITION_ALWAYS, 504, 0, 456, FACING_NORTH, 0, 328, 272, 584, 512, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_FAMILY_ROOM_FROM_CENTRAL_HALL, CONDITION_ALWAYS, 136, 0, 640, FACING_NORTH, 0, 16, 472, 256, 688, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_GUARD_ROOM_FROM_NORTH_HALL, CONDITION_ALWAYS, 648, 0, 72, FACING_SOUTH, 0, 568, 16, 808, 176, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_GUARD_ROOM_FROM_SOUTH_HALL, CONDITION_ALWAYS, 744, 0, 80, FACING_SOUTH, 0, 568, 16, 808, 176, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_WOMENS_ROOM_FROM_NORTH_HALL, CONDITION_ALWAYS, 696, 0, 376, FACING_NORTH, 0, 576, 264, 816, 432, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_CELL_KEY_ROOM_FROM_CENTRAL_HALL, CONDITION_ALWAYS, 200, 0, 848, FACING_SOUTH, 0, 152, 680, 424, 992, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_CELL_KEY_ROOM_FROM_EAST_HALL, CONDITION_ALWAYS, 264, 0, 776, FACING_SOUTH, 0, 152, 680, 424, 992, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_FROM_LUNPA_FORTRESS_GROUND_FLOOR_NORTH, CONDITION_ALWAYS, 376, 0, 824, FACING_SOUTH, 0, 152, 680, 424, 992, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_SOUTH_HALL_FROM_GUARD_ROOM, CONDITION_ALWAYS, 536, 0, 696, FACING_NORTH, 0, 488, 584, 760, 896, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_SOUTH_HALL_FROM_DONPAS_ROOM, CONDITION_ALWAYS, 680, 0, 832, FACING_SOUTH, 0, 488, 584, 760, 896, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_SOUTH_HALL_FROM_EAST_HALL, CONDITION_ALWAYS, 712, 0, 856, FACING_NORTH, 0, 488, 584, 760, 896, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_DONPAS_ROOM_FROM_SOUTH_HALL, CONDITION_ALWAYS, 912, 0, 192, FACING_NORTH, 0, 784, 24, 1024, 240, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_EAST_HALL_FROM_CELL_KEY_ROOM, CONDITION_ALWAYS, 840, 0, 664, FACING_NORTH, 0, 768, 440, 1008, 704, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_EAST_HALL_FROM_SOUTH_HALL, CONDITION_ALWAYS, 952, 0, 528, FACING_SOUTH, 0, 768, 440, 1008, 704, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_TONIC_ROOM_FROM_DONPAS_ROOM, CONDITION_ALWAYS, 872, 0, 376, FACING_NORTH, 0, 768, 264, 1016, 432, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_DONPAS_ROOM_FROM_TONIC_ROOM, CONDITION_ALWAYS, 904, 0, 112, FACING_SOUTH, 0, 784, 24, 1024, 240, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_CENTRAL_HALL_UNUSED, CONDITION_ALWAYS, 408, 0, 456, FACING_NORTH, 0, 328, 272, 584, 512, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_GUARD_ROOM_AFTER_BATTLE, CONDITION_ALWAYS, 648, 0, 88, FACING_NORTH, 0, 568, 16, 808, 176, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gLunpaFortressCaveEntrances[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 168, 0, 632, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { LUNPA_FORTRESS_CAVE_FROM_LUNPA_FORTRESS_GROUND_FLOOR, CONDITION_ALWAYS, 424, 0, 584, FACING_SOUTH, 0, 200, 488, 480, 648, 0 },
    { LUNPA_FORTRESS_CAVE_ENTRANCE_FROM_POOL, CONDITION_ALWAYS, 344, 0, 568, FACING_SOUTH, 0, 200, 488, 480, 648, 0 },
    { LUNPA_FORTRESS_CAVE_POOL_FROM_ENTRANCE, CONDITION_ALWAYS, 312, 0, 448, FACING_NORTH, 0, 8, 8, 568, 480, 0 },
    { LUNPA_FORTRESS_CAVE_FROM_LUNPA_FORTRESS_TOADONPA_NORTHEAST, CONDITION_ALWAYS, 440, 0, 296, FACING_SOUTH, 0, 8, 8, 568, 480, 0 },
    { LUNPA_FORTRESS_CAVE_FROM_LUNPA_FORTRESS_TOADONPA_NORTH, CONDITION_ALWAYS, 312, 0, 248, FACING_SOUTH, 0, 8, 8, 568, 480, 0 },
    { LUNPA_FORTRESS_CAVE_FROM_LUNPA_FORTRESS_TOADONPA_NORTHWEST, CONDITION_ALWAYS, 104, 0, 184, FACING_SOUTH, 0, 8, 8, 568, 480, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gLunpaFortressEntrancesOther[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 168, 0, 632, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { LUNPA_FORTRESS_TOADONPA_FROM_LUNPA_FORTRESS_CAVE_NORTH, CONDITION_ALWAYS, 56, 0, 328, FACING_NORTH, 0, 8, 8, 264, 424, 0 },
    { LUNPA_FORTRESS_TOADONPA_FROM_LUNPA_FORTRESS_CAVE_NORTHEAST, CONDITION_ALWAYS, 184, 0, 368, FACING_NORTH, 0, 8, 8, 264, 424, 0 },
    { LUNPA_FORTRESS_TOADONPA_FROM_LUNPA_FORTRESS_CAVE_NORTHWEST, CONDITION_ALWAYS, 328, 0, 344, FACING_SOUTH, 0, 288, 32, 576, 432, 0 },
    { LUNPA_FORTRESS_TOADONPA_AFTER_BATTLE, CONDITION_ALWAYS, 440, 0, 224, FACING_NORTH, 0, 288, 32, 576, 432, 0 },
    { SCENE_TABLE_END },
};

/// LunpaFortressGroundFloor's exits, after a copy of West Lunpa Cave's that these scenes
/// never read.
u32 gLunpaFortressExitsOther[] = {
    SCENE_EXITS(WestLunpaCave),
    SCENE_EXIT(WEST_LUNPA_CAVE_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_WEST_LUNPA_CAVE),
    SCENE_EXIT(WEST_LUNPA_CAVE_TO_LUNPA, Lunpa, LUNPA_FROM_WEST_LUNPA_CAVE),
    SCENE_EXIT(WEST_LUNPA_CAVE_BY_WAGON, Kalay, KALAY_FROM_HAMMET_PALACE_GROUNDS),
    SCENE_EXITS(LunpaFortressGroundFloor),
    SCENE_EXIT(LUNPA_FORTRESS_GROUND_FLOOR_TO_LUNPA_FORTRESS_GATES, LunpaFortressGates, LUNPA_FORTRESS_GATES_FROM_LUNPA_FORTRESS_GROUND_FLOOR),
    SCENE_EXIT(EXIT_THROWN_OUT, LunpaFortressGates, LUNPA_FORTRESS_GATES_AFTER_THROWN_OUT),
    SCENE_EXIT(LUNPA_FORTRESS_GROUND_FLOOR_ENTRANCE_TO_MAIN_HALL, LunpaFortressGroundFloor, LUNPA_FORTRESS_GROUND_FLOOR_MAIN_HALL_FROM_ENTRANCE),
    SCENE_EXIT(LUNPA_FORTRESS_GROUND_FLOOR_MAIN_HALL_TO_ENTRANCE, LunpaFortressGroundFloor, LUNPA_FORTRESS_GROUND_FLOOR_ENTRANCE_FROM_MAIN_HALL),
    SCENE_EXIT(LUNPA_FORTRESS_GROUND_FLOOR_MAIN_HALL_TO_NORTH_HALL, LunpaFortressGroundFloor, LUNPA_FORTRESS_GROUND_FLOOR_NORTH_HALL_FROM_MAIN_HALL),
    SCENE_EXIT(LUNPA_FORTRESS_GROUND_FLOOR_MAIN_HALL_TO_DINING_ROOM, LunpaFortressGroundFloor, LUNPA_FORTRESS_GROUND_FLOOR_DINING_ROOM_FROM_MAIN_HALL),
    SCENE_EXIT(LUNPA_FORTRESS_GROUND_FLOOR_MAIN_HALL_TO_NORTHEAST_HALL, LunpaFortressGroundFloor, LUNPA_FORTRESS_GROUND_FLOOR_NORTHEAST_HALL_FROM_MAIN_HALL),
    SCENE_EXIT(LUNPA_FORTRESS_GROUND_FLOOR_MAIN_HALL_TO_EAST_HALL, LunpaFortressGroundFloor, LUNPA_FORTRESS_GROUND_FLOOR_EAST_HALL_FROM_MAIN_HALL),
    SCENE_EXIT(LUNPA_FORTRESS_GROUND_FLOOR_TO_LUNPA_FORTRESS_UPPER_FLOOR_SOUTH, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_FROM_LUNPA_FORTRESS_GROUND_FLOOR_SOUTH),
    SCENE_EXIT(LUNPA_FORTRESS_GROUND_FLOOR_NORTH_HALL_TO_MAIN_HALL, LunpaFortressGroundFloor, LUNPA_FORTRESS_GROUND_FLOOR_MAIN_HALL_FROM_NORTH_HALL),
    SCENE_EXIT(LUNPA_FORTRESS_GROUND_FLOOR_TO_LUNPA_FORTRESS_UPPER_FLOOR_NORTH, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_FROM_LUNPA_FORTRESS_GROUND_FLOOR_NORTH),
    SCENE_EXIT(LUNPA_FORTRESS_GROUND_FLOOR_TO_LUNPA_FORTRESS_CAVE, LunpaFortressCave, LUNPA_FORTRESS_CAVE_FROM_LUNPA_FORTRESS_GROUND_FLOOR),
    SCENE_EXIT(LUNPA_FORTRESS_GROUND_FLOOR_DINING_ROOM_TO_MAIN_HALL, LunpaFortressGroundFloor, LUNPA_FORTRESS_GROUND_FLOOR_MAIN_HALL_FROM_DINING_ROOM),
    SCENE_EXIT(LUNPA_FORTRESS_GROUND_FLOOR_NORTHEAST_HALL_TO_MAIN_HALL, LunpaFortressGroundFloor, LUNPA_FORTRESS_GROUND_FLOOR_MAIN_HALL_FROM_NORTHEAST_HALL),
    SCENE_EXIT(LUNPA_FORTRESS_GROUND_FLOOR_NORTHEAST_HALL_TO_EAST_HALL, LunpaFortressGroundFloor, LUNPA_FORTRESS_GROUND_FLOOR_EAST_HALL_FROM_NORTHEAST_HALL),
    SCENE_EXIT(LUNPA_FORTRESS_GROUND_FLOOR_NORTHEAST_HALL_TO_STOREROOM, LunpaFortressGroundFloor, LUNPA_FORTRESS_GROUND_FLOOR_STOREROOM_FROM_NORTHEAST_HALL),
    SCENE_EXIT(LUNPA_FORTRESS_GROUND_FLOOR_EAST_HALL_TO_MAIN_HALL, LunpaFortressGroundFloor, LUNPA_FORTRESS_GROUND_FLOOR_MAIN_HALL_FROM_EAST_HALL),
    SCENE_EXIT(LUNPA_FORTRESS_GROUND_FLOOR_EAST_HALL_TO_NORTHEAST_HALL, LunpaFortressGroundFloor, LUNPA_FORTRESS_GROUND_FLOOR_NORTHEAST_HALL_FROM_EAST_HALL),
    SCENE_EXIT(LUNPA_FORTRESS_GROUND_FLOOR_EAST_HALL_TO_TREASURE_ROOM, LunpaFortressGroundFloor, LUNPA_FORTRESS_GROUND_FLOOR_TREASURE_ROOM_FROM_EAST_HALL),
    SCENE_EXIT(LUNPA_FORTRESS_GROUND_FLOOR_STOREROOM_TO_NORTHEAST_HALL, LunpaFortressGroundFloor, LUNPA_FORTRESS_GROUND_FLOOR_NORTHEAST_HALL_FROM_STOREROOM),
    SCENE_EXIT(LUNPA_FORTRESS_GROUND_FLOOR_TREASURE_ROOM_TO_EAST_HALL, LunpaFortressGroundFloor, LUNPA_FORTRESS_GROUND_FLOOR_EAST_HALL_FROM_TREASURE_ROOM),
    SCENE_EXITS_END,
};

u32 gLunpaFortressUpperFloorExits[] = {
    SCENE_EXITS(LunpaFortressUpperFloor),
    SCENE_EXIT(EXIT_THROWN_OUT, LunpaFortressGates, LUNPA_FORTRESS_GATES_AFTER_THROWN_OUT),
    SCENE_EXIT(LUNPA_FORTRESS_UPPER_FLOOR_TO_LUNPA_FORTRESS_GROUND_FLOOR_SOUTH, LunpaFortressGroundFloor, LUNPA_FORTRESS_GROUND_FLOOR_FROM_LUNPA_FORTRESS_UPPER_FLOOR_SOUTH),
    SCENE_EXIT(LUNPA_FORTRESS_UPPER_FLOOR_WEST_HALL_TO_NORTH_HALL, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_NORTH_HALL_FROM_WEST_HALL),
    SCENE_EXIT(LUNPA_FORTRESS_UPPER_FLOOR_WEST_HALL_TO_CENTRAL_HALL, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_CENTRAL_HALL_FROM_WEST_HALL),
    SCENE_EXIT(LUNPA_FORTRESS_UPPER_FLOOR_NORTH_HALL_TO_WEST_HALL, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_WEST_HALL_FROM_NORTH_HALL),
    SCENE_EXIT(LUNPA_FORTRESS_UPPER_FLOOR_NORTH_HALL_TO_GUARD_ROOM, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_GUARD_ROOM_FROM_NORTH_HALL),
    SCENE_EXIT(LUNPA_FORTRESS_UPPER_FLOOR_NORTH_HALL_TO_WOMENS_ROOM, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_WOMENS_ROOM_FROM_NORTH_HALL),
    SCENE_EXIT(LUNPA_FORTRESS_UPPER_FLOOR_CENTRAL_HALL_TO_WEST_HALL, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_WEST_HALL_FROM_CENTRAL_HALL),
    SCENE_EXIT(LUNPA_FORTRESS_UPPER_FLOOR_CENTRAL_HALL_TO_FAMILY_ROOM, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_FAMILY_ROOM_FROM_CENTRAL_HALL),
    SCENE_EXIT(LUNPA_FORTRESS_UPPER_FLOOR_CENTRAL_HALL_TO_CELL_KEY_ROOM, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_CELL_KEY_ROOM_FROM_CENTRAL_HALL),
    SCENE_EXIT(LUNPA_FORTRESS_UPPER_FLOOR_FAMILY_ROOM_TO_CENTRAL_HALL, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_CENTRAL_HALL_FROM_FAMILY_ROOM),
    SCENE_EXIT(LUNPA_FORTRESS_UPPER_FLOOR_GUARD_ROOM_TO_NORTH_HALL, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_NORTH_HALL_FROM_GUARD_ROOM),
    SCENE_EXIT(LUNPA_FORTRESS_UPPER_FLOOR_GUARD_ROOM_TO_SOUTH_HALL, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_SOUTH_HALL_FROM_GUARD_ROOM),
    SCENE_EXIT(LUNPA_FORTRESS_UPPER_FLOOR_WOMENS_ROOM_TO_NORTH_HALL, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_NORTH_HALL_FROM_WOMENS_ROOM),
    SCENE_EXIT(LUNPA_FORTRESS_UPPER_FLOOR_CELL_KEY_ROOM_TO_CENTRAL_HALL, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_CENTRAL_HALL_FROM_CELL_KEY_ROOM),
    SCENE_EXIT(LUNPA_FORTRESS_UPPER_FLOOR_CELL_KEY_ROOM_TO_EAST_HALL, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_EAST_HALL_FROM_CELL_KEY_ROOM),
    SCENE_EXIT(LUNPA_FORTRESS_UPPER_FLOOR_TO_LUNPA_FORTRESS_GROUND_FLOOR_NORTH, LunpaFortressGroundFloor, LUNPA_FORTRESS_GROUND_FLOOR_FROM_LUNPA_FORTRESS_UPPER_FLOOR_NORTH),
    SCENE_EXIT(LUNPA_FORTRESS_UPPER_FLOOR_SOUTH_HALL_TO_GUARD_ROOM, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_GUARD_ROOM_FROM_SOUTH_HALL),
    SCENE_EXIT(LUNPA_FORTRESS_UPPER_FLOOR_SOUTH_HALL_TO_DONPAS_ROOM, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_DONPAS_ROOM_FROM_SOUTH_HALL),
    SCENE_EXIT(LUNPA_FORTRESS_UPPER_FLOOR_SOUTH_HALL_TO_EAST_HALL, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_EAST_HALL_FROM_SOUTH_HALL),
    SCENE_EXIT(LUNPA_FORTRESS_UPPER_FLOOR_DONPAS_ROOM_TO_SOUTH_HALL, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_SOUTH_HALL_FROM_DONPAS_ROOM),
    SCENE_EXIT(LUNPA_FORTRESS_UPPER_FLOOR_EAST_HALL_TO_CELL_KEY_ROOM, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_CELL_KEY_ROOM_FROM_EAST_HALL),
    SCENE_EXIT(LUNPA_FORTRESS_UPPER_FLOOR_EAST_HALL_TO_SOUTH_HALL, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_SOUTH_HALL_FROM_EAST_HALL),
    SCENE_EXIT(LUNPA_FORTRESS_UPPER_FLOOR_TONIC_ROOM_TO_DONPAS_ROOM, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_DONPAS_ROOM_FROM_TONIC_ROOM),
    SCENE_EXIT(LUNPA_FORTRESS_UPPER_FLOOR_DONPAS_ROOM_TO_TONIC_ROOM, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_TONIC_ROOM_FROM_DONPAS_ROOM),
    SCENE_EXITS_END,
};

u32 gLunpaFortressCaveAndToadonpaExits[] = {
    SCENE_EXITS(LunpaFortressCave),
    SCENE_EXIT(LUNPA_FORTRESS_CAVE_TO_LUNPA_FORTRESS_GROUND_FLOOR, LunpaFortressGroundFloor, LUNPA_FORTRESS_GROUND_FLOOR_FROM_LUNPA_FORTRESS_CAVE),
    SCENE_EXIT(LUNPA_FORTRESS_CAVE_ENTRANCE_TO_POOL, LunpaFortressCave, LUNPA_FORTRESS_CAVE_POOL_FROM_ENTRANCE),
    SCENE_EXIT(LUNPA_FORTRESS_CAVE_POOL_TO_ENTRANCE, LunpaFortressCave, LUNPA_FORTRESS_CAVE_ENTRANCE_FROM_POOL),
    SCENE_EXIT(LUNPA_FORTRESS_CAVE_TO_LUNPA_FORTRESS_TOADONPA_NORTHEAST, LunpaFortressToadonpa, LUNPA_FORTRESS_TOADONPA_FROM_LUNPA_FORTRESS_CAVE_NORTHEAST),
    SCENE_EXIT(LUNPA_FORTRESS_CAVE_TO_LUNPA_FORTRESS_TOADONPA_NORTH, LunpaFortressToadonpa, LUNPA_FORTRESS_TOADONPA_FROM_LUNPA_FORTRESS_CAVE_NORTH),
    SCENE_EXIT(LUNPA_FORTRESS_CAVE_TO_LUNPA_FORTRESS_TOADONPA_NORTHWEST, LunpaFortressToadonpa, LUNPA_FORTRESS_TOADONPA_FROM_LUNPA_FORTRESS_CAVE_NORTHWEST),
    SCENE_EXIT(EXIT_THROWN_OUT, LunpaFortressGates, LUNPA_FORTRESS_GATES_AFTER_THROWN_OUT),
    SCENE_EXITS(LunpaFortressToadonpa),
    SCENE_EXIT(LUNPA_FORTRESS_TOADONPA_TO_LUNPA_FORTRESS_CAVE_NORTH, LunpaFortressCave, LUNPA_FORTRESS_CAVE_FROM_LUNPA_FORTRESS_TOADONPA_NORTH),
    SCENE_EXIT(LUNPA_FORTRESS_TOADONPA_TO_LUNPA_FORTRESS_CAVE_NORTHEAST, LunpaFortressCave, LUNPA_FORTRESS_CAVE_FROM_LUNPA_FORTRESS_TOADONPA_NORTHEAST),
    SCENE_EXIT(LUNPA_FORTRESS_TOADONPA_TO_LUNPA_FORTRESS_CAVE_NORTHWEST, LunpaFortressCave, LUNPA_FORTRESS_CAVE_FROM_LUNPA_FORTRESS_TOADONPA_NORTHWEST),
    SCENE_EXIT(LUNPA_FORTRESS_TOADONPA_TO_LUNPA_FORTRESS_GATES, LunpaFortressGates, LUNPA_FORTRESS_GATES_FROM_LUNPA_FORTRESS_TOADONPA),
    SCENE_EXIT(EXIT_THROWN_OUT, LunpaFortressGates, LUNPA_FORTRESS_GATES_AFTER_THROWN_OUT),
    SCENE_EXITS_END,
};

struct ScenePlacement gLunpaFortressPlacementsOther[] = {
    { SCENE_TABLE_END },
};

struct ScenePlacement gLunpaFortressPlacementsWestLunpaCave[] = {
    { SPRITE_PUDDLE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(184), 0, PIXELS(328), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(232), 0, PIXELS(296), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(344), 0, PIXELS(252), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(552), 0, PIXELS(184), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_BUNZA, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_HAMMET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_WEST, TALK_KEEP_FACING, 0 },
    { IVAN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    { MIA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gLunpaFortressGroundFloorPlacements[] = {
    { SPRITE_CHEST, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_CHEST, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_CHEST, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_CHEST, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_MAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(136), 0, PIXELS(760), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_MAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(392), 0, PIXELS(728), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(104), 0, PIXELS(440), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_MAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(152), 0, PIXELS(464), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_MAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(192), 0, PIXELS(432), FACING_SOUTH + FACING_STEP, TALK_KEEP_FACING, 0 },
    { SPRITE_MAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(808), 0, PIXELS(232), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_MAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, (s32)gGroundFloorPatrolRoute,
        PIXELS(736), 0, PIXELS(648), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(192), 0, PIXELS(464), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(424), 0, PIXELS(392), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_MAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(432), 0, PIXELS(768), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gLunpaFortressUpperFloorPlacements[] = {
    { 0xf5, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(264), 0, PIXELS(856), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_MAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, (s32)gFirstPatrolRoute,
        PIXELS(88), 0, PIXELS(192), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_MAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, (s32)gSecondPatrolRoute,
        PIXELS(88), 0, PIXELS(288), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_MAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(456), 0, PIXELS(392), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_MAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(680), 0, PIXELS(88), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_MAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(704), 0, PIXELS(104), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_MAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(680), 0, PIXELS(128), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_MAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(312), 0, PIXELS(840), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_MAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(680), 0, PIXELS(824), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_MAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, (s32)gHallPatrolRoute,
        PIXELS(920), 0, PIXELS(600), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(88), 0, PIXELS(616), FACING_EAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(168), 0, PIXELS(560), FACING_WEST, TALK_FACE_PARTY, 0 },
    { SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(148), 0, PIXELS(616), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(72), 0, PIXELS(560), FACING_EAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(664), 0, PIXELS(360), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(712), 0, PIXELS(344), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_DONPA, CONDITION_ALWAYS, (s32)gDonpaSleeps,
        PIXELS(944), 0, PIXELS(122), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(904), 0, PIXELS(120), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { MERCURY_DJINNI, FLAG_DJINNI_0 + DJINNI_TONIC, ACTOR_STAND,
        PIXELS(872), 0, PIXELS(336), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gLunpaFortressCavePlacements[] = {
    { 0xf8, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(280), 0, PIXELS(576), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xf8, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(440), 0, PIXELS(282), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xf8, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(56), 0, PIXELS(176), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(504), 0, PIXELS(168), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { SPRITE_CRATES, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(248), 0, PIXELS(136), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gLunpaFortressToadonpaPlacements[] = {
    { SPRITE_BLANK, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(56), 0, PIXELS(72), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(136), 0, PIXELS(72), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(216), 0, PIXELS(72), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { 0xbb, CONDITION_ALWAYS, 7,
        PIXELS(472), 0, PIXELS(88), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_HAMMET, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(528), 0, PIXELS(216), FACING_WEST, TALK_KEEP_FACING, 0 },
    { IVAN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    { MIA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    { SPRITE_DODONPA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    { SPRITE_DONPA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    { 0x120, CONDITION_ALWAYS, 7,
        PIXELS(464), 0, PIXELS(88), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

/// Unused: an empty event table nothing returns.
struct SceneEvent gLunpaFortressNoEvents[] = {
    { SCENE_EVENTS_END },
};

struct SceneEvent gLunpaFortressGroundFloorEvents[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, LUNPA_FORTRESS_GROUND_FLOOR_TO_LUNPA_FORTRESS_GATES },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, LUNPA_FORTRESS_GROUND_FLOOR_ENTRANCE_TO_MAIN_HALL },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, LUNPA_FORTRESS_GROUND_FLOOR_MAIN_HALL_TO_ENTRANCE },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, LUNPA_FORTRESS_GROUND_FLOOR_MAIN_HALL_TO_NORTH_HALL },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, LUNPA_FORTRESS_GROUND_FLOOR_MAIN_HALL_TO_DINING_ROOM },
    { EVENT_EXIT, 6, CONDITION_ALWAYS, LUNPA_FORTRESS_GROUND_FLOOR_MAIN_HALL_TO_NORTHEAST_HALL },
    { EVENT_EXIT, 7, CONDITION_ALWAYS, LUNPA_FORTRESS_GROUND_FLOOR_MAIN_HALL_TO_EAST_HALL },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED, 8, CONDITION_ALWAYS, LUNPA_FORTRESS_GROUND_FLOOR_TO_LUNPA_FORTRESS_UPPER_FLOOR_SOUTH },
    { EVENT_EXIT, 9, CONDITION_ALWAYS, LUNPA_FORTRESS_GROUND_FLOOR_NORTH_HALL_TO_MAIN_HALL },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED, 10, CONDITION_ALWAYS, LUNPA_FORTRESS_GROUND_FLOOR_TO_LUNPA_FORTRESS_UPPER_FLOOR_NORTH },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, 11, CONDITION_ALWAYS, LUNPA_FORTRESS_GROUND_FLOOR_TO_LUNPA_FORTRESS_CAVE },
    { EVENT_EXIT, 12, CONDITION_ALWAYS, LUNPA_FORTRESS_GROUND_FLOOR_DINING_ROOM_TO_MAIN_HALL },
    { EVENT_EXIT, 13, CONDITION_ALWAYS, LUNPA_FORTRESS_GROUND_FLOOR_NORTHEAST_HALL_TO_MAIN_HALL },
    { EVENT_EXIT, 14, CONDITION_ALWAYS, LUNPA_FORTRESS_GROUND_FLOOR_NORTHEAST_HALL_TO_EAST_HALL },
    { EVENT_EXIT, 15, CONDITION_ALWAYS, LUNPA_FORTRESS_GROUND_FLOOR_NORTHEAST_HALL_TO_STOREROOM },
    { EVENT_EXIT, 16, CONDITION_ALWAYS, LUNPA_FORTRESS_GROUND_FLOOR_EAST_HALL_TO_MAIN_HALL },
    { EVENT_EXIT, 17, CONDITION_ALWAYS, LUNPA_FORTRESS_GROUND_FLOOR_EAST_HALL_TO_NORTHEAST_HALL },
    { EVENT_EXIT, 18, CONDITION_ALWAYS, LUNPA_FORTRESS_GROUND_FLOOR_EAST_HALL_TO_TREASURE_ROOM },
    { EVENT_EXIT, 19, CONDITION_ALWAYS, LUNPA_FORTRESS_GROUND_FLOOR_STOREROOM_TO_NORTHEAST_HALL },
    { EVENT_EXIT, 20, CONDITION_ALWAYS, LUNPA_FORTRESS_GROUND_FLOOR_TREASURE_ROOM_TO_EAST_HALL },
    { SEARCH(SEARCH_UNNAMED), TRIGGER_LOCKED_DOOR, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SearchLockedDoor) },
    { TOUCH_FACING(FACING_NORTH), DOOR_TRIGGER_FIRST, GROUND_FLOOR_DOOR_FLAG, EVENT_SCRIPT(Event_UnlockGroundFloorDoor) },
    { TOUCH_FACING(FACING_SOUTH), DOOR_TRIGGER_FIRST, GROUND_FLOOR_DOOR_FLAG, EVENT_SCRIPT(Event_UnlockGroundFloorDoor) },
    { TOUCH_FACING(FACING_NORTH), DOOR_TRIGGER_FIRST + 1, GROUND_FLOOR_DOOR_FLAG + 1, EVENT_SCRIPT(Event_UnlockGroundFloorDoor) },
    { TOUCH_FACING(FACING_SOUTH), DOOR_TRIGGER_FIRST + 1, GROUND_FLOOR_DOOR_FLAG + 1, EVENT_SCRIPT(Event_UnlockGroundFloorDoor) },
    { TOUCH_FACING(FACING_NORTH), DOOR_TRIGGER_FIRST + 2, GROUND_FLOOR_DOOR_FLAG + 2, EVENT_SCRIPT(Event_UnlockGroundFloorDoor) },
    { TOUCH_FACING(FACING_SOUTH), DOOR_TRIGGER_FIRST + 2, GROUND_FLOOR_DOOR_FLAG + 2, EVENT_SCRIPT(Event_UnlockGroundFloorDoor) },
    { EVENT_TOUCH, 50, FLAG_SCENE_TEMP_24, EVENT_SCRIPT(Event_EntranceGuardThrowsPartyOut) },
    { EVENT_TOUCH, 51, FLAG_SCENE_TEMP_25, EVENT_SCRIPT(Event_SentriesThrowPartyOut) },
    { EVENT_TALK, GROUND_FLOOR_MAN_WONDERING_ABOUT_DONPA, CONDITION_ALWAYS, MSGNO(MsgLunpaDonpaResting) },
    { EVENT_TALK, GROUND_FLOOR_WOMAN_SEEKING_DODONPA, CONDITION_ALWAYS, MSGNO(MsgLunpaWhereIsDodonpa) },
    { EVENT_TALK, GROUND_FLOOR_FEASTING_MAN, CONDITION_ALWAYS, MSGNO(MsgLunpaHammetBuysDrink) },
    { EVENT_TALK, GROUND_FLOOR_BOASTING_MAN, CONDITION_ALWAYS, MSGNO(MsgLunpaHammetMoneyTree) },
    { EVENT_TALK, GROUND_FLOOR_STARTLED_OLD_MAN, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToStartledOldMan) },
    { EVENT_TALK, GROUND_FLOOR_SECOND_SENTRY, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToSentry) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), GROUND_FLOOR_MAN_WONDERING_ABOUT_DONPA, CONDITION_ALWAYS, MSGNO(MsgLunpaWhoFoughtDonpa) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), GROUND_FLOOR_WOMAN_SEEKING_DODONPA, CONDITION_ALWAYS, MSGNO(MsgLunpaWifeSeeksDodonpa) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), GROUND_FLOOR_FEASTING_MAN, CONDITION_ALWAYS, MSGNO(MsgLunpaKalayBuysDrink) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), GROUND_FLOOR_BOASTING_MAN, CONDITION_ALWAYS, MSGNO(MsgLunpaHammetKeepsEarnings) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), GROUND_FLOOR_STARTLED_OLD_MAN, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadStartledResidentsThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), GROUND_FLOOR_SECOND_SENTRY, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadSentrysThoughts) },
    { EVENT_RAISED, TRIGGER_GROUND_FLOOR_PATROL_SPOTTED, CONDITION_ALWAYS, EVENT_SCRIPT(Event_GroundFloorPatrolThrowsPartyOut) },
    { EVENT_RAISED, TRIGGER_UPPER_HALL_SPOTTED, CONDITION_ALWAYS, EVENT_SCRIPT(Event_UpperHallGuardThrowsPartyOut) },
    { EVENT_RAISED, TRIGGER_ENTRANCE_GUARD_SPOTTED, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EntranceGuardThrowsPartyOut) },
    { EVENT_RAISED, TRIGGER_SENTRIES_SPOTTED, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SentriesThrowPartyOut) },
    { SEARCH(SEARCH_CHEST), 100, FLAG_LUNPA_FORTRESS_CHEST_ELIXIR, FIND_ITEM(ITEM_ELIXIR) },
    { SEARCH(SEARCH_CHEST), 101, FLAG_LUNPA_FORTRESS_CHEST_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SEARCH(SEARCH_CHEST), 102, FLAG_LUNPA_FORTRESS_CHEST_MYTHRIL_CIRCLET, FIND_ITEM(ITEM_MYTHRIL_CIRCLET) },
    { SEARCH(SEARCH_CHEST), 103, FLAG_LUNPA_FORTRESS_CHEST_COINS, FIND_COINS(100) },
    { SEARCH(SEARCH_BARREL), 104, FLAG_LUNPA_FORTRESS_BARREL_VIAL, FIND_ITEM(ITEM_VIAL) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gLunpaFortressUpperFloorEvents[] = {
    { EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, 1, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_TO_LUNPA_FORTRESS_GROUND_FLOOR_SOUTH },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_WEST_HALL_TO_NORTH_HALL },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_WEST_HALL_TO_CENTRAL_HALL },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_NORTH_HALL_TO_WEST_HALL },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_NORTH_HALL_TO_GUARD_ROOM },
    { EVENT_EXIT, 6, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_NORTH_HALL_TO_WOMENS_ROOM },
    { EVENT_EXIT, 7, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_CENTRAL_HALL_TO_WEST_HALL },
    { EVENT_EXIT, 8, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_CENTRAL_HALL_TO_FAMILY_ROOM },
    { EVENT_EXIT, 9, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_CENTRAL_HALL_TO_CELL_KEY_ROOM },
    { EVENT_EXIT, 10, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_FAMILY_ROOM_TO_CENTRAL_HALL },
    { EVENT_EXIT, 11, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_GUARD_ROOM_TO_NORTH_HALL },
    { EVENT_EXIT, 12, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_GUARD_ROOM_TO_SOUTH_HALL },
    { EVENT_EXIT, 13, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_WOMENS_ROOM_TO_NORTH_HALL },
    { EVENT_EXIT, 14, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_CELL_KEY_ROOM_TO_CENTRAL_HALL },
    { EVENT_EXIT, 15, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_CELL_KEY_ROOM_TO_EAST_HALL },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, 16, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_TO_LUNPA_FORTRESS_GROUND_FLOOR_NORTH },
    { EVENT_EXIT, 17, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_SOUTH_HALL_TO_GUARD_ROOM },
    { EVENT_EXIT, 18, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_SOUTH_HALL_TO_DONPAS_ROOM },
    { EVENT_EXIT, 19, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_SOUTH_HALL_TO_EAST_HALL },
    { EVENT_EXIT, 20, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_DONPAS_ROOM_TO_SOUTH_HALL },
    { EVENT_EXIT, 21, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_EAST_HALL_TO_CELL_KEY_ROOM },
    { EVENT_EXIT, 22, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_EAST_HALL_TO_SOUTH_HALL },
    { EVENT_EXIT, 23, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_TONIC_ROOM_TO_DONPAS_ROOM },
    { EVENT_EXIT, 24, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_DONPAS_ROOM_TO_TONIC_ROOM },
    { EVENT_TOUCH, 30, FLAG_LUNPA_FORTRESS_MIDDLE_GUARD_BEATEN, EVENT_SCRIPT(Event_FightHallGuard) },
    { EVENT_TOUCH, 31, FLAG_LUNPA_FORTRESS_THREE_GUARDS_BEATEN, EVENT_SCRIPT(Event_FightThreeGuards) },
    { EVENT_TOUCH, 32, FLAG_LUNPA_FORTRESS_EAST_GUARD_BEATEN, EVENT_SCRIPT(Event_FightEastGuard) },
    { EVENT_TOUCH, 33, FLAG_LUNPA_FORTRESS_WEST_GUARD_BEATEN, EVENT_SCRIPT(Event_FightWestGuard) },
    { SEARCH(SEARCH_UNNAMED), TRIGGER_LOCKED_DOOR, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SearchLockedDoor) },
    { TOUCH_FACING(FACING_SOUTH), DOOR_TRIGGER_FIRST, UPPER_FLOOR_DOOR_FLAG, EVENT_SCRIPT(Event_UnlockUpperFloorDoor) },
    { TOUCH_FACING(FACING_NORTH), DOOR_TRIGGER_FIRST + 1, UPPER_FLOOR_DOOR_FLAG + 1, EVENT_SCRIPT(Event_UnlockUpperFloorDoor) },
    { TOUCH_FACING(FACING_SOUTH), DOOR_TRIGGER_FIRST + 1, UPPER_FLOOR_DOOR_FLAG + 1, EVENT_SCRIPT(Event_UnlockUpperFloorDoor) },
    { PSYNERGY_ON_ACTOR(ABILITY_CATCH), UPPER_FLOOR_CELL_KEY, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TakeCellKey) },
    { EVENT_TALK, UPPER_FLOOR_CELL_KEY, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TakeCellKey) },
    { EVENT_TALK, UPPER_FLOOR_DODONPAS_WIFE, CONDITION_ALWAYS, EVENT_SCRIPT(Event_AskWhereDodonpaIs) },
    { EVENT_TALK, UPPER_FLOOR_DONPAPPA, CONDITION_ALWAYS, MSGNO(MsgLunpaChildMustGrowUp) },
    { EVENT_TALK, UPPER_FLOOR_DODONPAS_DAUGHTER, CONDITION_ALWAYS, MSGNO(MsgLunpaPapaWatchesLadies) },
    { EVENT_TALK, UPPER_FLOOR_OLD_WOMAN_PRAISING_DONPAPPA, CONDITION_ALWAYS, MSGNO(MsgLunpaChildTooResponsible) },
    { EVENT_TALK, UPPER_FLOOR_WOMAN_FEARING_FOR_DONPA, CONDITION_ALWAYS, MSGNO(MsgLunpaDonpaExhausted) },
    { EVENT_TALK, UPPER_FLOOR_CAPTIVE_WOMAN, CONDITION_ALWAYS, MSGNO(MsgLunpaWomenWantHome) },
    { EVENT_TALK, UPPER_FLOOR_GUARD_A, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToBeatenGuardA) },
    { EVENT_TALK, UPPER_FLOOR_GUARD_B, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToBeatenGuardB) },
    { EVENT_TALK, UPPER_FLOOR_GUARD_C, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToBeatenGuardC) },
    { EVENT_TALK, UPPER_FLOOR_ATTENDANT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToAttendant) },
    { EVENT_TALK, UPPER_FLOOR_DONPA, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToDonpa) },
    { EVENT_TALK, UPPER_FLOOR_TONIC, CONDITION_ALWAYS, EVENT_SCRIPT(Event_DjinniJoins) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), UPPER_FLOOR_DODONPAS_WIFE, CONDITION_ALWAYS, MSGNO(MsgLunpaDonpappaSuccessor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), UPPER_FLOOR_DONPAPPA, CONDITION_ALWAYS, MSGNO(MsgLunpaDonpappaProtectsFortress) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), UPPER_FLOOR_DODONPAS_DAUGHTER, CONDITION_ALWAYS, MSGNO(MsgLunpaDaughterWillNotMarryPapa) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), UPPER_FLOOR_OLD_WOMAN_PRAISING_DONPAPPA, CONDITION_ALWAYS, MSGNO(MsgLunpaDonpappaJudges) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), UPPER_FLOOR_WOMAN_FEARING_FOR_DONPA, CONDITION_ALWAYS, MSGNO(MsgLunpaFearWithoutDonpa) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), UPPER_FLOOR_CAPTIVE_WOMAN, CONDITION_ALWAYS, MSGNO(MsgLunpaHopeDonpaRecovers) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), UPPER_FLOOR_GUARD_A, CONDITION_ALWAYS, MSGNO(MsgLunpaShouldNotHaveFought) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), UPPER_FLOOR_GUARD_B, CONDITION_ALWAYS, MSGNO(MsgLunpaHammetInCave) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), UPPER_FLOOR_GUARD_C, CONDITION_ALWAYS, MSGNO(MsgLunpaHiddenDoorSwitch) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), UPPER_FLOOR_DONPA, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadDonpasThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), UPPER_FLOOR_ATTENDANT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadAttendantsThoughts) },
    { EVENT_RAISED, TRIGGER_PATROL_PAIR_SPOTTED, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PatrolPairThrowsPartyOut) },
    { EVENT_RAISED, TRIGGER_HALL_PATROL_SPOTTED, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HallPatrolThrowsPartyOut) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gLunpaFortressCaveEvents[] = {
    { EVENT_EXIT | EXIT_STYLE_ANIMATED, 1, CONDITION_ALWAYS, LUNPA_FORTRESS_CAVE_TO_LUNPA_FORTRESS_GROUND_FLOOR },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, LUNPA_FORTRESS_CAVE_ENTRANCE_TO_POOL },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, LUNPA_FORTRESS_CAVE_POOL_TO_ENTRANCE },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, LUNPA_FORTRESS_CAVE_TO_LUNPA_FORTRESS_TOADONPA_NORTHEAST },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, LUNPA_FORTRESS_CAVE_TO_LUNPA_FORTRESS_TOADONPA_NORTH },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, 6, CONDITION_ALWAYS, LUNPA_FORTRESS_CAVE_TO_LUNPA_FORTRESS_TOADONPA_NORTHWEST },
    { SEARCH(SEARCH_UNNAMED), TRIGGER_LOCKED_DOOR, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SearchLockedDoor) },
    { SEARCH(SEARCH_UNNAMED), TRIGGER_SEALED_DOOR, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SearchSealedDoor) },
    { TOUCH_FACING(FACING_NORTH), DOOR_TRIGGER_FIRST, CAVE_DOOR_FLAG, EVENT_SCRIPT(Event_UnlockCaveDoor) },
    { TOUCH_FACING(FACING_NORTH), DOOR_TRIGGER_FIRST + 1, CAVE_DOOR_FLAG + 1, EVENT_SCRIPT(Event_UnlockCaveDoor) },
    { TOUCH_FACING(FACING_NORTH), DOOR_TRIGGER_FIRST + 2, CAVE_DOOR_FLAG + 2, EVENT_SCRIPT(Event_UnlockCaveDoor) },
    { TOUCH_FACING(FACING_SOUTH), DOOR_TRIGGER_FIRST + 2, CAVE_DOOR_FLAG + 2, EVENT_SCRIPT(Event_UnlockCaveDoor) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 50, CONDITION_ALWAYS, EVENT_SCRIPT(Event_CratesSink) },
    { TOUCH_WALKING, 70, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushCrates) },
    { SEARCH(SEARCH_UNNAMED), 51, FLAG_SCENE_TEMP_18, EVENT_SCRIPT(Event_FlipSecondSwitch) },
    { SEARCH(SEARCH_UNNAMED), 52, FLAG_SCENE_TEMP_17, EVENT_SCRIPT(Event_FlipFirstSwitch) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ACTS), 0, CONDITION_ALWAYS, EVENT_SCRIPT(Event_RevealSwitches) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ENDS), 0, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HideSwitches) },
    { PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), CAVE_FIRST_SWITCH_COVER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_BlowAwayFirstSwitchCover) },
    { PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), CAVE_PASSAGE_COVER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_BlowAwayPassageCover) },
    { PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), CAVE_SECOND_SWITCH_COVER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_BlowAwaySecondSwitchCover) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), CAVE_CRATES, FLAG_LUNPA_FORTRESS_CRATES_MOVED, EVENT_SCRIPT(Event_MoveCrates) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gLunpaFortressEventsOther[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, LUNPA_FORTRESS_TOADONPA_TO_LUNPA_FORTRESS_CAVE_NORTH },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, LUNPA_FORTRESS_TOADONPA_TO_LUNPA_FORTRESS_CAVE_NORTHEAST },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED, 5, CONDITION_ALWAYS, LUNPA_FORTRESS_TOADONPA_TO_LUNPA_FORTRESS_CAVE_NORTHWEST },
    { SEARCH(SEARCH_UNNAMED), TRIGGER_LOCKED_DOOR, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SearchLockedDoor) },
    { TOUCH_FACING(FACING_NORTH), DOOR_TRIGGER_FIRST, TOADONPA_DOOR_FLAG, EVENT_SCRIPT(Event_UnlockToadonpaDoor) },
    { TOUCH_FACING(FACING_NORTH), DOOR_TRIGGER_FIRST + 1, TOADONPA_DOOR_FLAG + 1, EVENT_SCRIPT(Event_UnlockToadonpaDoor) },
    { TOUCH_FACING(FACING_NORTH), DOOR_TRIGGER_FIRST + 2, TOADONPA_DOOR_FLAG + 2, EVENT_SCRIPT(Event_UnlockToadonpaDoor) },
    { TOUCH_FACING(FACING_NORTH), DOOR_TRIGGER_FIRST + 3, TOADONPA_DOOR_FLAG + 3, EVENT_SCRIPT(Event_UnlockToadonpaDoor) },
    { TOUCH_FACING(FACING_NORTH), DOOR_TRIGGER_FIRST + 4, TOADONPA_DOOR_FLAG + 4, EVENT_SCRIPT(Event_UnlockToadonpaDoor) },
    { EVENT_TOUCH, 20, FLAG_SCENE_TEMP_70, EVENT_SCRIPT(Event_FindHammet) },
    { EVENT_TALK, TOADONPA_CELL_GUARD, CONDITION_ALWAYS, MSGNO(MsgLunpaHammetStillInside) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOADONPA_CELL_GUARD, CONDITION_ALWAYS, MSGNO(MsgLunpaDodonpaSecretlyInside) },
    // XXX: these two rows give TOADONPA_HAMMET Dodonpa's lines from his cell,
    // though Hammet has left the cave by the time Dodonpa is jailed; whether
    // the party can reach them is not known.
    { EVENT_TALK, TOADONPA_HAMMET, CONDITION_ALWAYS, MSGNO(MsgLunpaWrongTurnOver) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOADONPA_HAMMET, CONDITION_ALWAYS, MSGNO(MsgLunpaCantBelieveWhen) },
    { EVENT_TALK, TOADONPA_DODONPA_STAND_IN, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToJailedDodonpa) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOADONPA_DODONPA_STAND_IN, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadJailedDodonpasThoughts) },
    { SEARCH(SEARCH_CHEST), 101, FLAG_LUNPA_FORTRESS_CHEST_POWER_BREAD, FIND_ITEM(ITEM_POWER_BREAD) },
    { SCENE_EVENTS_END },
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

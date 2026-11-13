/// @file
/// Vault Cave's overlay, which holds three rooms, each a scene of its own and
/// all named Vault Cave (SceneId_VaultCavePassage, SceneId_VaultCaveVambrace
/// and SceneId_VaultCaveMouth, area 31).
///
/// The cave opens off Vault as the party finds it on its return
/// (SceneId_VaultRevisit). That scene's exits
/// VAULT_REVISIT_TO_VAULT_CAVE_PASSAGE and VAULT_REVISIT_TO_VAULT_CAVE_MOUTH
/// lead in, through the hidden passage the dog shows and through a cave mouth
/// in the town's northeast cliff.
///
/// No story flag is read here and no one speaks. The cave is a side trip of
/// puzzles for Move, Frost and Whirlwind, and only "The door is tightly
/// locked." and "Isaac flipped the switch." are shown.
///
/// - Vault Cave (SceneId_VaultCavePassage), the way in from the hidden passage.
///   VAULT_CAVE_PASSAGE_FROM_VAULT_REVISIT is the foot of the ladder down from
///   that passage. A region there takes VAULT_CAVE_PASSAGE_TO_VAULT_REVISIT
///   back out to Vault (VAULT_REVISIT_FROM_VAULT_CAVE_PASSAGE), and Retreat
///   also leads there (Scene_Initialize). Searching the switch flips it
///   (VaultCave_FlipSwitch, FLAG_VAULT_CAVE_SWITCH_FLIPPED). The switch
///   turns, the cave rumbles and shakes for eight seconds, and its map changes
///   as it fades (VaultCave_RunRumble). The change reaches
///   SceneId_VaultCaveVambrace too. Arriving in its northern room
///   (VAULT_CAVE_VAMBRACE_FROM_VAULT_CAVE_PASSAGE_NORTH,
///   VAULT_CAVE_VAMBRACE_FROM_VAULT_CAVE_MOUTH_NORTH,
///   VAULT_CAVE_VAMBRACE_NORTH_FROM_FROST_ROOM and
///   VAULT_CAVE_VAMBRACE_NORTH_FROM_WEST), Scene_Initialize draws it closed
///   until the switch is flipped and opens its cells after. Arriving down the
///   ladder clears the flag again, so the switch is reset each time the party
///   comes in from Vault. Arriving in the pool room
///   (VAULT_CAVE_PASSAGE_FROM_VAULT_CAVE_VAMBRACE_EAST,
///   VAULT_CAVE_PASSAGE_FROM_VAULT_CAVE_VAMBRACE_SOUTHEAST,
///   VAULT_CAVE_PASSAGE_WATER_ROOM_FROM_NORTH_WEST,
///   VAULT_CAVE_PASSAGE_WATER_ROOM_FROM_CHEST and
///   VAULT_CAVE_PASSAGE_WATER_ROOM_FROM_NORTH_EAST) redraws the flipped switch
///   and its changed cells. The room's chest is a mimic (FIND_MIMIC, slot 3 of
///   ENCOUNTER_ZONE_MIMIC).
/// - Vault Cave (SceneId_VaultCaveVambrace). In its southern room a pillar
///   (ACTOR_SOUTHERN_PILLAR) goes onto its switch by pushing
///   (VaultCave_PushSouthernPillar) or Move (VaultCave_MoveSouthernPillar).
///   Once it rests there the puzzle chime plays and the switch opens the way
///   (FLAG_VAULT_CAVE_SOUTH_PILLAR_ON_SWITCH). In the puddle room, six drops of
///   water (ACTOR_FIRST_WATER_DROP and the five after it) fall at random, one
///   every 64 frames, over six puddles (ACTOR_WEST_UPPER_PUDDLE to
///   ACTOR_FAR_EAST_PUDDLE) that Frost freezes. A frozen puddle changes its
///   map cell, and a drop falling onto it stops 32 pixels up instead of
///   reaching the floor (VaultCave_UpdateFrostControlledFallingActors). While
///   the leader is 16 pixels or more above the floor, the puddles are drawn
///   beneath him. The six frozen states are
///   FLAG_VAULT_CAVE_WEST_UPPER_PUDDLE_FROZEN to
///   FLAG_VAULT_CAVE_FAR_EAST_PUDDLE_FROZEN, read by index. The room's entry
///   code freezes again every puddle whose flag is set. Whirlwind blows away
///   ivy (ACTOR_IVY) for as long as the party stays in the cave
///   (FLAG_VAULT_CAVE_IVY_BLOWN_AWAY, an area flag). A chest holds the
///   Vambrace.
/// - Vault Cave (SceneId_VaultCaveMouth). In its northern room four water drops
///   (ACTOR_FIRST_TIMED_WATER_DROP and the three after it) fall on a fixed
///   schedule (gVaultCave_ScheduleFrames) while the party pushes or Moves a
///   pillar (ACTOR_NORTHERN_PILLAR) onto its switch
///   (FLAG_VAULT_CAVE_NORTH_PILLAR_ON_SWITCH). A water drop that lands on that
///   pillar marks it struck (FLAG_VAULT_CAVE_NORTH_PILLAR_STRUCK,
///   SE_DUST_PUFF). A struck pillar no longer opens the switch until the room
///   is entered again. VAULT_CAVE_MOUTH_TO_VAULT_REVISIT leads out to Vault
///   (VAULT_REVISIT_FROM_VAULT_CAVE_MOUTH).
///
/// Each room has a door that only says it is locked
/// (VaultCave_ShowLockedDoor).
///
/// The pushes share VaultCave_PushPillarAhead, which moves the pillar the
/// leader faces one cell unless another actor or the terrain is in the way.
/// The water drops run as tasks started by Scene_Initialize.

#include "main_routine_work.h"
#include "module/field/field.h"
#include "system_call.h"
#include "scene_ids.h"
#include "module/import.h"
#include "module/scene/vault_cave.h"
#include "module/field/field_sequence.h"
#include "module/field/event_commands.h"
#include "module/system/system.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "types.h"
#include "edition.h"
#include "far_call.h"
#include "sound.h"
#include "system.h"
#include "heap_slots.h"
#include "types.h"
#include "text/msg_ids.h"
#include "event.h"
#include "scene_tables.h"
#include "task.h"
#include "actor.h"
#include "ability_ids.h"
#include "flags.h"
#include "sounds.h"
#include "actor_script.h"
#include "map_grid_scroll.h"
#include "scene.h"
#include "macros.h"
#include "item_ids.h"

// Forward declarations.
static void VaultCave_ApplySouthernSwitchCells(void);
static void VaultCave_UseUnderfootFrostPriorities(void);
static void VaultCave_UseAutomaticFrostPriorities(void);
static void VaultCave_SpawnPuffs(struct Actor *source);

#include "io_write_queue.h"

#define TILE(position) ((position) / px(16))

/// The cell behaviors of the cells the events answer.
enum VaultCaveCellBehavior {
  /// The switch SceneId_VaultCavePassage flips.
  /// ★ Named from the game: "flipped the switch." (MsgFieldFlippedSwitch)
  CELL_BEHAVIOR_VAULT_CAVE_SWITCH = 20,
  /// The cells of SceneId_VaultCaveMouth's northern pillar, pushed.
  CELL_BEHAVIOR_VAULT_CAVE_NORTHERN_PILLAR = 50,
  /// The cells of SceneId_VaultCaveVambrace's southern pillar, pushed.
  CELL_BEHAVIOR_VAULT_CAVE_SOUTHERN_PILLAR = 60,
  /// ★ Named from the game: "The door is tightly locked."
  /// (MsgFieldDoorTightlyLocked)
  CELL_BEHAVIOR_VAULT_CAVE_LOCKED_DOOR = 99,
  /// ★ Named from the game: "checked the chest" (MsgVaultIsaacCheckedChest)
  CELL_BEHAVIOR_VAULT_CAVE_CHEST = 100
};

/// Scene flags, and one area flag. The six puddles of
/// SceneId_VaultCaveVambrace keep their frozen state in six flags in a row,
/// read by index as well.
enum {
  /// The upper western puddle, the first of the six.
  /// ★ Named from what the player sees: six puddles of water in the floor of
  /// the puddle room, under the falling drops (its render)
  FLAG_VAULT_CAVE_WEST_UPPER_PUDDLE_FROZEN = FLAG_SCENE_TEMP_00,
  /// ★ Named from what the player sees: the western puddle of the lower row
  /// (its render)
  FLAG_VAULT_CAVE_WEST_LOWER_PUDDLE_FROZEN = FLAG_SCENE_TEMP_01,
  /// ★ Named from what the player sees: the lower row's puddle just east of
  /// the western one (its render)
  FLAG_VAULT_CAVE_INNER_WEST_LOWER_PUDDLE_FROZEN = FLAG_SCENE_TEMP_02,
  /// ★ Named from what the player sees: the eastern puddle of the upper row
  /// (its render)
  FLAG_VAULT_CAVE_EAST_UPPER_PUDDLE_FROZEN = FLAG_SCENE_TEMP_03,
  /// ★ Named from what the player sees: the eastern puddle of the lower row
  /// (its render)
  FLAG_VAULT_CAVE_EAST_LOWER_PUDDLE_FROZEN = FLAG_SCENE_TEMP_04,
  /// ★ Named from what the player sees: the easternmost puddle, between the
  /// rows (its render)
  FLAG_VAULT_CAVE_FAR_EAST_PUDDLE_FROZEN = FLAG_SCENE_TEMP_05,
  /// A water drop struck the northern pillar before it reached its switch. The
  /// drop's cell matched the pillar's (VaultCave_ResetFallingActorAtActorCell).
  /// ★ Named from what the player sees: the pillar the leader pushes toward
  /// its switch in the northern room of SceneId_VaultCaveMouth (its render)
  FLAG_VAULT_CAVE_NORTH_PILLAR_STRUCK = FLAG_SCENE_TEMP_07,
  /// Whirlwind has blown the ivy away.
  /// ★ Named from the game: "Whirlwind" (MsgAbilityName)
  FLAG_VAULT_CAVE_IVY_BLOWN_AWAY = FLAG_AREA_TEMP_00
};

/// SPRITE_FLAME_PILLAR's animations (spr_299.tsv).
enum FlamePillarAnimation {
  FLAME_PILLAR_ANIMATION_UNLIT = 1,  ///< the pillar, no flame on top
  FLAME_PILLAR_ANIMATION_BURNING = 2 ///< a flame flickering on top
};

/// The actors of SceneId_VaultCaveVambrace, numbering the rows of
/// gVaultCaveVambrace_ActorPlacements.
enum VaultCaveVambraceActorId {
  /// A pillar, drawn as ACTOR_SOUTHERN_PILLAR is, two cells east of the
  /// southern switch. It is drawn underfoot when the party arrives by
  /// VAULT_CAVE_VAMBRACE_FROM_VAULT_CAVE_PASSAGE_SOUTH or
  /// VAULT_CAVE_VAMBRACE_SOUTH_FROM_FROST_ROOM.
  /// ★ Named from what the player sees: a pink stone pillar with a flame
  /// burning on top (its render)
  ACTOR_PILLAR_EAST_OF_SOUTHERN_SWITCH = ACTOR_FIRST_PLACED,
  /// Pushed or Moved onto the southern switch, map cell (15, 54).
  /// ★ Named from what the player sees: the western of the room's two pink
  /// flame pillars, four cells from the switch marked with a flame, joined to
  /// it by a dotted track (its render)
  ACTOR_SOUTHERN_PILLAR,
  /// The first of six water drops that fall at random, one every 64 frames
  /// (VaultCave_UpdateFrostControlledFallingActors). VaultCave_SpawnPuffs also
  /// uses their sprite for the puffs where one lands.
  /// ★ Named from what the player sees: blue drops of water above the puddles
  /// (its render)
  ACTOR_FIRST_WATER_DROP,
  /// The six puddles under the water drops, in their order and in the order of
  /// their flags (FLAG_VAULT_CAVE_WEST_UPPER_PUDDLE_FROZEN to
  /// FLAG_VAULT_CAVE_FAR_EAST_PUDDLE_FROZEN). They are drawn as the puddle that
  /// Frost freezes into an ice pillar in bilibin_cave.c. Frozen, a puddle stops
  /// a water drop 32 pixels up.
  /// ★ Named from what the player sees: six blue puddles on the cave floor,
  /// each under a drop of water (its render)
  ACTOR_WEST_UPPER_PUDDLE = ACTOR_FIRST_WATER_DROP + 6,
  /// ★ Named from what the player sees: the western puddle of the lower row
  /// (its render)
  ACTOR_WEST_LOWER_PUDDLE,
  /// ★ Named from what the player sees: the lower row's puddle just east of it
  /// (its render)
  ACTOR_INNER_WEST_LOWER_PUDDLE,
  /// ★ Named from what the player sees: the eastern puddle of the upper row
  /// (its render)
  ACTOR_EAST_UPPER_PUDDLE,
  /// ★ Named from what the player sees: the eastern puddle of the lower row
  /// (its render)
  ACTOR_EAST_LOWER_PUDDLE,
  /// ★ Named from what the player sees: the easternmost puddle, between the
  /// rows (its render)
  ACTOR_FAR_EAST_PUDDLE,
  /// Ivy (SPRITE_IVY), as in bilibin.c, drawn half as tall again. It is placed
  /// until Whirlwind blows it away (FLAG_VAULT_CAVE_IVY_BLOWN_AWAY).
  /// ★ Named from what the player sees: leafy green ivy against the cave
  /// wall (its render)
  ACTOR_IVY
};

/// The actors of SceneId_VaultCaveMouth, numbering the rows of
/// gVaultCaveMouth_ActorPlacements. The northern switch is map cell (16,
/// 12).
enum VaultCaveMouthActorId {
  /// A pillar two cells east of the northern switch.
  /// ★ Named from what the player sees: a pink stone pillar with a flame
  /// burning on top (its render)
  ACTOR_PILLAR_EAST_OF_NORTHERN_SWITCH = ACTOR_FIRST_PLACED,
  /// A pillar two cells south of the northern switch, which Move leaves where
  /// it is (VaultCave_IgnoreMove).
  /// ★ Named from what the player sees: a pink stone pillar, its flame out
  /// (its render)
  ACTOR_PILLAR_SOUTH_OF_NORTHERN_SWITCH,
  /// Pushed or Moved onto the northern switch.
  /// ★ Named from what the player sees: a pink stone pillar with a flame
  /// burning on top (its render)
  ACTOR_NORTHERN_PILLAR,
  /// The first of four water drops, in the same sprite, that fall on a fixed
  /// schedule (VaultCave_UpdateTimedFallingActors).
  /// ★ Named from what the player sees: blue drops of water on the cave floor
  /// (its render)
  ACTOR_FIRST_TIMED_WATER_DROP
};

const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 sceneId = gMainRoutineWork.next;

  if (sceneId == (s32)&SceneId_VaultCavePassage) {
    return gVaultCavePassage_Entrances;
  }
  if (sceneId == (s32)&SceneId_VaultCaveVambrace) {
    return gVaultCaveVambrace_Entrances;
  }
  if (sceneId == (s32)&SceneId_VaultCaveMouth) {
    return gVaultCaveMouth_Entrances;
  }
  return gVaultCave_FallbackEntrances;
}

/// Returns the region boxes used by the cave's exits.
const struct SceneExitRegion *Scene_GetExitRegions(void) {
  return gVaultCave_Regions;
}

const u32 *Scene_GetExits(void) {
  s16 sceneId = gMainRoutineWork.next;

  if (sceneId == (s32)&SceneId_VaultCavePassage) {
    return gVaultCavePassage_Exits;
  }
  if (sceneId == (s32)&SceneId_VaultCaveVambrace) {
    return gVaultCaveVambrace_Exits;
  }
  if (sceneId == (s32)&SceneId_VaultCaveMouth) {
    return gVaultCaveMouth_Exits;
  }
  return gVaultCave_FallbackExits;
}

/// Selects the pillars, water drops, puddles and ivy of the room.
const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s16 sceneId = gMainRoutineWork.next;

  if (sceneId == (s32)&SceneId_VaultCavePassage) {
    return gVaultCavePassage_ActorPlacements;
  }
  if (sceneId == (s32)&SceneId_VaultCaveVambrace) {
    return gVaultCaveVambrace_ActorPlacements;
  }
  if (sceneId == (s32)&SceneId_VaultCaveMouth) {
    return gVaultCaveMouth_ActorPlacements;
  }
  return gVaultCave_FallbackActorPlacements;
}

/// Finds a placed actor with collision enabled at the requested map
/// cell.
static struct Actor *VaultCave_FindCollidableActorAtCell(s32 column, s32 row) {
  struct FieldWork *work = gFieldWork;
  u32 actorSlot;

  for (actorSlot = ACTOR_FIRST_PLACED; actorSlot <= 65; actorSlot++) {
    struct Actor *actor = work->actors[actorSlot];

    // UB: map creation leaves unused actor slots NULL.
    if (
#ifdef UBFIX
        actor != NULL &&
#endif
        column == FIELD_CELL(actor->x.fixed) &&
        row == FIELD_CELL(actor->z.fixed) && actor->collisionFlags != 0) {
      return actor;
    }
  }
  return NULL;
}

#include "shared/actor_step.h"
#include "pushed_actor.h"

/// Pushes the faced pillar one cell unless another actor or terrain
/// blocks it.
static void VaultCave_PushPillarAhead(void) {
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);
  s32 direction = leader->facing >> 12;
  struct Actor *pillar;
  struct Actor *beyond;
  struct FieldPosition destination;
  s32 fallingIndex;
  s32 column;
  s32 row;

  column =
      (leader->x.part.pixel + (gVaultCave_DirectionSteps[direction] >> 16)) >>
      4;
  row = (leader->z.part.pixel + (s16)gVaultCave_DirectionSteps[direction]) >> 4;
  pillar = VaultCave_FindCollidableActorAtCell(column, row);
  // UB: the pillar's collisionFlags is read before the test for no pillar.
#ifdef UBFIX
  if (pillar == NULL || pillar->collisionFlags == 0) {
#else
  if (pillar->collisionFlags == 0 || pillar == NULL) {
#endif
    return;
  }
  // SceneId_VaultCaveMouth's water drops are never pushed; in
  // SceneId_VaultCaveVambrace these slots hold water drops too.
  for (fallingIndex = 0; fallingIndex <= 3; fallingIndex++) {
    if (pillar == Actor_Get(fallingIndex + ACTOR_FIRST_TIMED_WATER_DROP)) {
      return;
    }
  }
  column =
      (pillar->x.part.pixel + (gVaultCave_DirectionSteps[direction] >> 16)) >>
      4;
  row = (pillar->z.part.pixel + (s16)gVaultCave_DirectionSteps[direction]) >> 4;
  beyond = VaultCave_FindCollidableActorAtCell(column, row);
  if (beyond != 0 && (beyond->collisionFlags & 1)) {
    return;
  }
  pillar->layer = 2;
  Actor_StepFrom(&destination, pillar, gVaultCave_DirectionSteps[direction]);
  if (FieldActor_CheckMovementCollision(pillar, &destination) > 0) {
    return;
  }
  Actor_SelectAnimation(leader, ANIM_PUSH);
  wait(15);
  Sound_Play(SE_PUSH_STEP);
  pillar->speedLimit = px(1) / 5;
  pillar->acceleration = px(1) / 5;
  FieldActor_SetMoveTargetFixed(pillar, destination.x, destination.y,
                                destination.z);
  leader->speedLimit = px(1) / 5;
  leader->acceleration = px(1) / 5;
  FieldActor_SetMoveTargetFixed(leader, destination.x, destination.y,
                                destination.z);
  Actor_WaitForMove(pillar);
  Sound_Play(SE_STOP_ACTION_SE);
  pillar->x.fixed = destination.x;
  pillar->z.fixed = destination.z;
  pillar->velocityX = 0;
  pillar->velocityZ = 0;
  leader->velocityX = 0;
  leader->velocityZ = 0;
  leader->targetX = ACTOR_NO_TARGET;
  leader->targetZ = ACTOR_NO_TARGET;
  Actor_SelectAnimation(leader, ANIM_STAND);
}

/// Whirlwind blows the ivy away. It is cast on ACTOR_IVY in the puddle room
/// of SceneId_VaultCaveVambrace. The ivy stays gone for as long as the party
/// stays in the cave, because FLAG_VAULT_CAVE_IVY_BLOWN_AWAY is an area flag.
static void VaultCave_UseWhirlwind(void) {
  GameFlag_Set(FLAG_VAULT_CAVE_IVY_BLOWN_AWAY);
}

/// In any room of Vault Cave, searching the locked door only says so.
/// "The door is tightly locked."
static void VaultCave_ShowLockedDoor(void) {
  FieldMessage_Show(MSGNO(MsgFieldDoorTightlyLocked), 1);
}

/// Selects the cave scene's examine triggers, movement checks and Psynergy
/// callbacks.
const struct Trigger *Scene_GetTriggers(void) {
  s16 sceneId = gMainRoutineWork.next;

  if (sceneId == (s32)&SceneId_VaultCavePassage) {
    return gVaultCavePassage_Triggers;
  }
  if (sceneId == (s32)&SceneId_VaultCaveVambrace) {
    return gVaultCaveVambrace_Triggers;
  }
  if (sceneId == (s32)&SceneId_VaultCaveMouth) {
    return gVaultCaveMouth_Triggers;
  }
  return gVaultCave_FallbackTriggers;
}

/// Tests the southern pillar's cell, rounding fixed coordinates toward zero.
static s32 VaultCave_IsSouthernPillarOnSwitch(void) {
  struct Actor *pillar = Actor_Get(ACTOR_SOUTHERN_PILLAR);
  s32 fixedZ = pillar->z.fixed;
  s32 fixedX;
  s32 row;
  s32 column;

  if (fixedZ < 0) {
    fixedZ += px(16) - 1;
  }
  fixedX = pillar->x.fixed;
  row = fixedZ >> 20;
  if (fixedX < 0) {
    fixedX += px(16) - 1;
  }
  column = fixedX >> 20;
  if (column == 15 && row == 54) {
    return 1;
  }
  return 0;
}

/// Finishes the southern switch only after the pillar reaches its target cell.
static inline void VaultCave_CompleteSouthernSwitch(void) {
  if (VaultCave_IsSouthernPillarOnSwitch() != 0) {
    GameFlag_Set(FLAG_VAULT_CAVE_SOUTH_PILLAR_ON_SWITCH);
    Sound_Play(MUS_PUZZLE_SOLVED);
    VaultCave_ApplySouthernSwitchCells();
  }
}

/// In VaultCaveVambrace's southern room, the party pushes the southern pillar.
/// Once it rests on its switch, the puzzle chime plays and the switch opens
/// the way.
static void VaultCave_PushSouthernPillar(void) {
  if (Event_GetFlag(FLAG_VAULT_CAVE_SOUTH_PILLAR_ON_SWITCH) == 0) {
    VaultCave_PushPillarAhead();
    VaultCave_CompleteSouthernSwitch();
  }
}

/// Restores the map cells and attributes of the completed southern switch.
static void VaultCave_ApplySouthernSwitchCells(void) {
  MapGrid_CopyCells(87, 50, 2, 4, 80, 50);
  MapGrid_CopyCells(23, 52, 1, 2, 16, 52);
  MapGrid_CopyCellAttributes(16, 52, 1, 1, 16, 53);
}

/// In VaultCaveVambrace's southern room, Move on the southern pillar, with the
/// same switch check.
static void VaultCave_MoveSouthernPillar(void) {
  if (Event_GetFlag(FLAG_VAULT_CAVE_SOUTH_PILLAR_ON_SWITCH) == 0) {
    VaultCave_CompleteSouthernSwitch();
  }
}

static void UNUSED VaultCave_UnusedNoOp(void) {}

/// Restores the map cells and attributes of the completed northern switch.
static void VaultCave_ApplyNorthernSwitchCells(void) {
  MapGrid_CopyCells(90, 9, 2, 3, 80, 9);
  MapGrid_CopyCells(27, 10, 1, 2, 17, 10);
  MapGrid_CopyCellAttributes(17, 10, 1, 1, 17, 11);
}

/// Tests the northern pillar's cell with the same signed-coordinate rounding.
static s32 VaultCave_IsNorthernPillarOnSwitch(void) {
  struct Actor *pillar = Actor_Get(ACTOR_NORTHERN_PILLAR);
  s32 fixedZ = pillar->z.fixed;
  s32 fixedX;
  s32 row;
  s32 column;

  if (fixedZ < 0) {
    fixedZ += px(16) - 1;
  }
  fixedX = pillar->x.fixed;
  row = fixedZ >> 20;
  if (fixedX < 0) {
    fixedX += px(16) - 1;
  }
  column = fixedX >> 20;
  if (column == 16 && row == 12) {
    return 1;
  }
  return 0;
}

/// The northern switch also waits until the falling-actor guard is clear.
static inline void VaultCave_CompleteNorthernSwitch(void) {
  if (GameFlag_Get(FLAG_VAULT_CAVE_NORTH_PILLAR_ON_SWITCH) == 0) {
    if (VaultCave_IsNorthernPillarOnSwitch() != 0) {
      if (GameFlag_Get(FLAG_VAULT_CAVE_NORTH_PILLAR_STRUCK) == 0) {
        Sound_Play(MUS_PUZZLE_SOLVED);
        VaultCave_ApplyNorthernSwitchCells();
        GameFlag_Set(FLAG_VAULT_CAVE_NORTH_PILLAR_ON_SWITCH);
      }
    }
  }
}

/// In VaultCaveMouth's northern room, the party pushes the northern pillar.
/// It opens its switch only if no water drop has struck it.
static void VaultCave_PushNorthernPillar(void) {
  VaultCave_PushPillarAhead();
  VaultCave_CompleteNorthernSwitch();
}

/// In VaultCaveMouth's northern room, Move on
/// ACTOR_PILLAR_SOUTH_OF_NORTHERN_SWITCH does nothing.
static void VaultCave_IgnoreMove(void) {}

/// In VaultCaveMouth's northern room, Move on the northern pillar, with the
/// same switch check.
static void VaultCave_MoveNorthernPillar(void) {
  VaultCave_CompleteNorthernSwitch();
}

/// Frost freezes the upper western puddle.
/// The puddle is in VaultCaveVambrace's puddle room. Freezing it changes its
/// map cell, and arriving again re-freezes every puddle whose flag is set.
static void VaultCave_FreezeWestUpperPuddle(void) {
  struct Actor *target = Actor_Get(ACTOR_WEST_UPPER_PUDDLE);

  MapGrid_CopyCellAttributes(26, 30, 1, 1, 23, 32);

  if (target != 0) {

    Actor_Get(ACTOR_WEST_UPPER_PUDDLE)->motionFlags = 0;
    target->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
  }

  GameFlag_Set(FLAG_VAULT_CAVE_WEST_UPPER_PUDDLE_FROZEN);
}

/// In VaultCaveVambrace's puddle room, Frost on the lower western puddle, as on
/// the upper one.
static void VaultCave_FreezeWestLowerPuddle(void) {
  struct Actor *target = Actor_Get(ACTOR_WEST_LOWER_PUDDLE);

  MapGrid_CopyCellAttributes(26, 30, 1, 1, 23, 34);

  if (target != 0) {

    Actor_Get(ACTOR_WEST_LOWER_PUDDLE)->motionFlags = 0;
    target->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
  }

  GameFlag_Set(FLAG_VAULT_CAVE_WEST_LOWER_PUDDLE_FROZEN);
}

/// In VaultCaveVambrace's puddle room, Frost on the inner lower western puddle.
static void VaultCave_FreezeInnerWestLowerPuddle(void) {
  struct Actor *target = Actor_Get(ACTOR_INNER_WEST_LOWER_PUDDLE);

  MapGrid_CopyCellAttributes(26, 30, 1, 1, 24, 34);

  if (target != 0) {

    Actor_Get(ACTOR_INNER_WEST_LOWER_PUDDLE)->motionFlags = 0;
    target->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
  }

  GameFlag_Set(FLAG_VAULT_CAVE_INNER_WEST_LOWER_PUDDLE_FROZEN);
}

/// In VaultCaveVambrace's puddle room, Frost on the upper eastern puddle.
static void VaultCave_FreezeEastUpperPuddle(void) {
  struct Actor *target = Actor_Get(ACTOR_EAST_UPPER_PUDDLE);

  MapGrid_CopyCellAttributes(26, 30, 1, 1, 26, 32);

  if (target != 0) {
    Actor_SetSpriteFlags(target, 0);

    Actor_Get(ACTOR_EAST_UPPER_PUDDLE)->motionFlags = 0;
    target->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
  }

  GameFlag_Set(FLAG_VAULT_CAVE_EAST_UPPER_PUDDLE_FROZEN);
}

/// In VaultCaveVambrace's puddle room, Frost on the lower eastern puddle.
static void VaultCave_FreezeEastLowerPuddle(void) {
  struct Actor *target = Actor_Get(ACTOR_EAST_LOWER_PUDDLE);

  MapGrid_CopyCellAttributes(26, 30, 1, 1, 26, 34);

  if (target != 0) {
    Actor_SetSpriteFlags(target, 0);

    Actor_Get(ACTOR_EAST_LOWER_PUDDLE)->motionFlags = 0;
    target->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
  }

  GameFlag_Set(FLAG_VAULT_CAVE_EAST_LOWER_PUDDLE_FROZEN);
}

/// In VaultCaveVambrace's puddle room, Frost on the far eastern puddle.
static void VaultCave_FreezeFarEastPuddle(void) {
  struct Actor *target = Actor_Get(ACTOR_FAR_EAST_PUDDLE);

  MapGrid_CopyCellAttributes(26, 30, 1, 1, 28, 33);

  if (target != 0) {
    Actor_SetSpriteFlags(target, 0);

    Actor_Get(ACTOR_FAR_EAST_PUDDLE)->motionFlags = 0;
    target->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
  }

  GameFlag_Set(FLAG_VAULT_CAVE_FAR_EAST_PUDDLE_FROZEN);
}

/// Changes the puddles' draw priorities when the leader is above them.
static void VaultCave_UpdatePuddlePriorities(void) {
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);

  if (leader->y.fixed >= px(16)) {
    VaultCave_UseUnderfootFrostPriorities();
  } else {
    VaultCave_UseAutomaticFrostPriorities();
  }
}

/// Keeps all six puddles beneath the leader's feet.
static void VaultCave_UseUnderfootFrostPriorities(void) {
  s32 targetIndex;
  s32 actorSlot;

  actorSlot = ACTOR_WEST_UPPER_PUDDLE;
  for (targetIndex = 0; targetIndex < 6; targetIndex++) {
    Actor_Get(actorSlot)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    actorSlot++;
  }
}

/// Restores automatic draw ACTOR_PRIORITY_AUTOMATIC for the same six puddles.
static void VaultCave_UseAutomaticFrostPriorities(void) {
  s32 actorSlot = ACTOR_WEST_UPPER_PUDDLE;
  s32 remaining = 5;

  do {
    struct Actor *target = Actor_Get(actorSlot);

    remaining--;
    target->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
    actorSlot++;
  } while (remaining >= 0);
}

/// Moves the third layer of the map grid through its phases, then fades the
/// mechanism's final map state.
static void VaultCave_RunRumble(void) {
  struct MapGridLayerScroll *layer =
      &((struct MapGridScrollWork *)gMapGridWork[0])->layers[2];
  s32 frame;
  s32 fadeLevel;
  s32 lowFadeLevel;
  s32 upperFadeBits;

  Event_Start();
  if (gFrameCount & 1) {
    layer->speedX = 1;
    layer->speedY = 1;
  } else {
    layer->speedX = -1;
    layer->speedY = -1;
  }
  MapGrid_SetShake(3, 3, 1);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  Sound_Play(SE_RUSH);
  // Scroll and shake for 480 frames before the final fade.
  for (frame = 479; frame >= 0; frame--) {
    u32 scrollDrift = (u32)(Random_Next() << 11) >> 16;

    layer->phaseY = (double)layer->phaseY - (4718.592 - (double)scrollDrift);
    Event_Wait(1);
  }
  // Fade the blend out over 70 frames, a step every 20.
  fadeLevel = 6;
  lowFadeLevel = 6;
  frame = 0;
  upperFadeBits = fadeLevel << 10;
  do {
    IoWriteQueue_Store16((u32)&REG_BLDALPHA,
                         upperFadeBits | (fadeLevel << 5) | lowFadeLevel);
    Event_Wait(1);
    if (frame % 20 == 0) {
      lowFadeLevel--;
      fadeLevel--;
    }
    frame++;
  } while (frame <= 69);
  // Replace the mechanism's map cells after the fade completes.
  MapGrid_CopyCells(19, 83, 15, 8, 19, 91);
  Sound_Play(SE_STOP_ACTION_SE);
  MapGrid_CenterOnFollowedPoint();
  MapGrid_WaitForShakeToSettle();
  Event_End();
}

/// In VaultCavePassage's pool room, searching the switch flips it.
/// "Isaac flipped the switch."
///
/// The first time since the party came in from Vault, the switch turns, the
/// cave rumbles and shakes for eight seconds and the map changes as it fades.
/// FLAG_VAULT_CAVE_SWITCH_FLIPPED then keeps the change until the party comes
/// down the ladder from Vault again.
static void VaultCave_FlipSwitch(void) {
  s32 destinationColumn;

  if (Event_GetFlag(FLAG_VAULT_CAVE_SWITCH_FLIPPED) == 0) {
    FieldMessage_Show(MSGNO(MsgFieldFlippedSwitch), 1);
    GameFlag_Set(FLAG_VAULT_CAVE_SWITCH_FLIPPED);
    // Animate the switch face before the adjacent mechanism starts.
    destinationColumn = 92;
    Sound_Play(SE_SWITCH);
    MapGrid_CopyCells(107, 27, 1, 1, destinationColumn, 27);
    Event_Wait(39);
    MapGrid_CopyCells(108, 27, 1, 1, destinationColumn, 27);
    Event_Wait(50);
    destinationColumn = 25;
    Sound_Play(SE_SWITCH_MECHANISM);
    MapGrid_CopyCells(1, 24, 1, 2, destinationColumn, 27);
    Event_Wait(40);
    MapGrid_CopyCells(2, 24, 1, 2, destinationColumn, 27);
    Event_Wait(40);
    VaultCave_RunRumble();
  }
}

/// Resets a low water drop sharing the subject's cell and reports a hit.
static s32 VaultCave_ResetFallingActorAtActorCell(s32 subjectActorId) {
  struct Actor *subject = Actor_Get(subjectActorId);
  s32 fallingIndex = 0;

  do {
    struct Actor *fallingActor =
        Actor_Get(fallingIndex + ACTOR_FIRST_TIMED_WATER_DROP);

    if (fallingActor->y.fixed > 0 && fallingActor->y.fixed < px(16)) {
      s32 fallingRow = TILE(fallingActor->z.fixed);
      s32 fallingColumn = TILE(fallingActor->x.fixed);
      s32 subjectRow = TILE(subject->z.fixed);
      s32 subjectColumn = TILE(subject->x.fixed);
      s32 zDelta = subjectRow - fallingRow;
      s32 xDelta = subjectColumn - fallingColumn;

      if (xDelta == 0 && zDelta == 0) {
        fallingActor->y.fixed = px(255);
        fallingActor->gravity = 0;
        fallingActor->velocityY = 0;
        return 1;
      }
    }

    fallingIndex++;
  } while (fallingIndex <= 3);

  return 0;
}

/// Drops four actors on a repeating schedule and handles their landings.
static void VaultCave_UpdateTimedFallingActors(void) {
  s32 fallingIndex;
  struct Actor *fallingActor;
  s32 paused;
  s32 scheduleIndex;

  paused = Actor_Get(ACTOR_NORTHERN_PILLAR)->paused;
  if (paused == 0) {
    if (++gVaultCave_ScheduleTimer > 190) {
      gVaultCave_ScheduleTimer = 0;
    }
    scheduleIndex = gVaultCave_ScheduleIndex;
    if (gVaultCave_ScheduleFrames[scheduleIndex] == gVaultCave_ScheduleTimer) {
      fallingActor = Actor_Get((scheduleIndex + ACTOR_FIRST_TIMED_WATER_DROP));
      fallingActor->gravity = px(1) / 25;
      if (++gVaultCave_ScheduleIndex > 3) {
        gVaultCave_ScheduleIndex = 0;
      }
    }
    // Reset actors that have reached the floor and spawn their landing puffs.
    for (fallingIndex = 0; fallingIndex <= 3; fallingIndex++) {
      fallingActor = Actor_Get((fallingIndex + ACTOR_FIRST_TIMED_WATER_DROP));
      if (fallingActor->velocityY >= 0) {
        if (fallingActor->y.fixed < px(1)) {
          VaultCave_SpawnPuffs(fallingActor);
          fallingActor->y.fixed = px(255);
          fallingActor->gravity = 0;
          fallingActor->velocityY = 0;
          fallingActor->paused = 0;
          Sound_Play(SE_DRIP);
        }
      }
    }
    // A water drop landing on either movable pillar restarts at the top.
    if (VaultCave_ResetFallingActorAtActorCell(ACTOR_NORTHERN_PILLAR) != 0) {
      Actor_SetAnimation(ACTOR_NORTHERN_PILLAR, FLAME_PILLAR_ANIMATION_UNLIT);
      if (Event_GetFlag(FLAG_VAULT_CAVE_NORTH_PILLAR_STRUCK) == 0) {
        GameFlag_Set(FLAG_VAULT_CAVE_NORTH_PILLAR_STRUCK);
        Sound_Play(SE_DUST_PUFF);
      } else {
        Sound_Play(SE_DRIP);
      }
    }
    if (VaultCave_ResetFallingActorAtActorCell(
            ACTOR_PILLAR_SOUTH_OF_NORTHERN_SWITCH) != 0) {
      Sound_Play(SE_DRIP);
    }
  }
}

/// Drops a random actor every 64 frames, with reset height set by its puddle
/// (frozen or not).
static void VaultCave_UpdateFrostControlledFallingActors(void) {

  s32 fallingIndex;
  struct Actor *fallingActor;
  s32 stateValue;
  s32 *selectedIndex;

  fallingActor = Actor_Get(ACTOR_FIRST_WATER_DROP);
  if (fallingActor->paused == 0) {
    // Preserve one random selection per 64 frames.
    if ((++gVaultCave_DropFrames & 63) == 0) {
      selectedIndex = &gVaultCave_SelectedDrop;
      stateValue = Import_RandomNext();
      stateValue = Math_RemainderUnsigned(stateValue, 6);
      *selectedIndex = stateValue;
      fallingActor = Actor_Get((stateValue + ACTOR_FIRST_WATER_DROP));
      fallingActor->gravity = px(1) / 25;
    }
    // Each actor's paired Frost flag selects its reset height.
    for (fallingIndex = 0; fallingIndex <= 5; fallingIndex++) {
      fallingActor = Actor_Get((fallingIndex + ACTOR_FIRST_WATER_DROP));
      stateValue = GameFlag_Get(
          (fallingIndex + FLAG_VAULT_CAVE_WEST_UPPER_PUDDLE_FROZEN));
      if (stateValue != 0) {
        if (fallingActor->velocityY <= 0) {
          if (fallingActor->y.fixed >= px(33)) {
            continue;
          }
        }
        fallingActor->y.fixed = px(255);
        fallingActor->gravity = 0;
        fallingActor->velocityY = 0;
        Sound_Play(SE_DRIP);
      } else {
        if (fallingActor->velocityY <= 0) {
          if (fallingActor->y.fixed >= px(1)) {
            continue;
          }
        }
        fallingActor->gravity = stateValue;
        fallingActor->velocityY = stateValue;
        fallingActor->y.fixed = px(255);
        Sound_Play(SE_DRIP);
      }
    }
  }
}

/// Offsets an actor's current position by a polar displacement.
static void VaultCave_OffsetActorRadially(struct Actor *actor, s32 radius,
                                          s32 angle) {
  s32 destination[3];

  if (actor == NULL) {
    return;
  }
  SET_VECTOR(destination, actor->x.fixed, actor->y.fixed, actor->z.fixed);
  Vector_AddPolarOffset(radius, angle, destination);
  FieldActor_SetMoveTargetFixed(actor, destination[0], destination[1],
                                destination[2]);
}

/// Spawns up to four short-lived puffs around a landing actor.
static void VaultCave_SpawnPuffs(struct Actor *source) {
  struct Actor *puff;
  s32 puffIndex;

  for (puffIndex = 0; puffIndex < 4; puffIndex++) {
    puff = FieldActor_Create(SPRITE_DROPLET, source->x.fixed, source->y.fixed,
                             source->z.fixed);
    if (puff == 0)
      break;
    puff->scaleY = 11 * Q16_ONE / 20;
    puff->scaleX = 11 * Q16_ONE / 20;
    puff->motionFlags = ACTOR_FALLS;
    puff->velocityY = -px(1);
    puff->speedLimit = Import_RandomNext() + px(4) / 5;
    puff->collisionFlags = ACTOR_SOLID;
    VaultCave_OffsetActorRadially(puff, px(32), Import_RandomNext());
    puff->wait = 8;
    Actor_StartScript(puff, gVaultCave_PuffScript);
  }
}

/// Initializes six water drops and starts their Frost-controlled updater.
static void VaultCave_StartFrostControlledFalls(void) {
  s32 actorSlot = ACTOR_FIRST_WATER_DROP;
  s32 remaining = 5;

  do {
    struct Actor *fallingActor;

    Actor_SetSpriteFlags(Actor_Get(actorSlot), 0);
    fallingActor = Actor_Get(actorSlot);
    fallingActor->bounce = Q16_ONE / 10;
    fallingActor->gravity = 0;
    remaining--;
    fallingActor->y.fixed = px(255);
    actorSlot++;
  } while (remaining >= 0);

  Task_AddFrame(VaultCave_UpdateFrostControlledFallingActors, TASK_PASS_FRAME);
}

/// Initializes four water drops and starts their timed updater.
static void VaultCave_StartTimedFalls(void) {
  s32 actorSlot = ACTOR_FIRST_TIMED_WATER_DROP;
  s32 fallingIndex = 0;

  do {
    struct Actor *fallingActor;

    Actor_SetSpriteFlags(Actor_Get(actorSlot), 0);
    fallingActor = Actor_Get(actorSlot);
    fallingActor->bounce = Q16_ONE / 10;
    fallingActor->gravity = 0;
    fallingActor->y.fixed = px(255);
    Actor_SetSpritePriority(fallingIndex + ACTOR_FIRST_TIMED_WATER_DROP, 1);
    fallingIndex++;
    actorSlot++;
  } while (fallingIndex <= 3);

  Task_AddFrame(VaultCave_UpdateTimedFallingActors, TASK_PASS_FRAME);
}

/// Arriving in any room of Vault Cave restores the room's puzzles and actors.
/// It then sets the Retreat destination, the foot of the ladder from Vault.
s32 Scene_Initialize(void) {
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  if (gMainRoutineWork.next == (s32)&SceneId_VaultCavePassage) {
    switch (gMainRoutineWork.entrance) {
    case VAULT_CAVE_PASSAGE_WATER_ROOM_FROM_NORTH_WEST:
    case VAULT_CAVE_PASSAGE_FROM_VAULT_CAVE_VAMBRACE_EAST:
    case VAULT_CAVE_PASSAGE_FROM_VAULT_CAVE_VAMBRACE_SOUTHEAST:
    case VAULT_CAVE_PASSAGE_WATER_ROOM_FROM_CHEST:
    case VAULT_CAVE_PASSAGE_WATER_ROOM_FROM_NORTH_EAST:
      if (GameFlag_Get(FLAG_VAULT_CAVE_SWITCH_FLIPPED) == 0) {
        MapGrid_CopyCellAttributes(22, 29, 1, 1, 21, 29);
      } else {
        MapGrid_CopyCells(108, 27, 1, 1, 92, 27);
        wait(1);
        MapGrid_CopyCells(19, 83, 15, 8, 19, 91);
        wait(1);
        MapGrid_CopyCells(2, 24, 1, 2, 25, 27);
      }
      MapGrid_CenterOnFollowedPoint();
      wait(1);
      break;
    case VAULT_CAVE_PASSAGE_FROM_VAULT_REVISIT:
      GameFlag_Clear(FLAG_VAULT_CAVE_SWITCH_FLIPPED);
      break;
    }
  }
  if (gMainRoutineWork.next == (s32)&SceneId_VaultCaveVambrace) {
    if (GameFlag_Get(FLAG_VAULT_CAVE_IVY_BLOWN_AWAY) == 0) {
      Actor_Get(ACTOR_IVY)->scaleY = 3 * Q16_ONE / 2;
    }
    switch (gMainRoutineWork.entrance) {
    case VAULT_CAVE_VAMBRACE_FROM_VAULT_CAVE_PASSAGE_NORTH:
    case VAULT_CAVE_VAMBRACE_FROM_VAULT_CAVE_MOUTH_NORTH:
    case VAULT_CAVE_VAMBRACE_NORTH_FROM_FROST_ROOM:
    case VAULT_CAVE_VAMBRACE_NORTH_FROM_WEST:
      if (GameFlag_Get(FLAG_VAULT_CAVE_SWITCH_FLIPPED) == 0) {
        MapGrid_CopyCells(5, 81, 11, 7, 5, 73);
      } else {
        MapGrid_CopyCellAttributes(5, 12, 1, 1, 6, 12);
        MapGrid_CopyCellAttributes(12, 10, 1, 1, 12, 11);
      }
      break;
    case VAULT_CAVE_VAMBRACE_FROST_ROOM_FROM_NORTH:
    case VAULT_CAVE_VAMBRACE_FROST_ROOM_FROM_SOUTH:
    case VAULT_CAVE_VAMBRACE_FROST_ROOM_FROM_CHEST:
      VaultCave_StartFrostControlledFalls();
      if (GameFlag_Get(FLAG_VAULT_CAVE_WEST_UPPER_PUDDLE_FROZEN) != 0) {
        Actor_SetAnimation(ACTOR_WEST_UPPER_PUDDLE, PUDDLE_ANIMATION_PILLAR);
        VaultCave_FreezeWestUpperPuddle();
      }
      if (GameFlag_Get(FLAG_VAULT_CAVE_WEST_LOWER_PUDDLE_FROZEN) != 0) {
        Actor_SetAnimation(ACTOR_WEST_LOWER_PUDDLE, PUDDLE_ANIMATION_PILLAR);
        VaultCave_FreezeWestLowerPuddle();
      }
      if (GameFlag_Get(FLAG_VAULT_CAVE_INNER_WEST_LOWER_PUDDLE_FROZEN) != 0) {
        Actor_SetAnimation(ACTOR_INNER_WEST_LOWER_PUDDLE,
                           PUDDLE_ANIMATION_PILLAR);
        VaultCave_FreezeInnerWestLowerPuddle();
      }
      if (GameFlag_Get(FLAG_VAULT_CAVE_EAST_UPPER_PUDDLE_FROZEN) != 0) {
        Actor_SetAnimation(ACTOR_EAST_UPPER_PUDDLE, PUDDLE_ANIMATION_PILLAR);
        VaultCave_FreezeEastUpperPuddle();
      }
      if (GameFlag_Get(FLAG_VAULT_CAVE_EAST_LOWER_PUDDLE_FROZEN) != 0) {
        Actor_SetAnimation(ACTOR_EAST_LOWER_PUDDLE, PUDDLE_ANIMATION_PILLAR);
        VaultCave_FreezeEastLowerPuddle();
      }
      if (GameFlag_Get(FLAG_VAULT_CAVE_FAR_EAST_PUDDLE_FROZEN) != 0) {
        Actor_SetAnimation(ACTOR_FAR_EAST_PUDDLE, PUDDLE_ANIMATION_PILLAR);
        VaultCave_FreezeFarEastPuddle();
      }
      Task_AddFrame(VaultCave_UpdatePuddlePriorities, TASK_PASS_FRAME);
      break;
    case VAULT_CAVE_VAMBRACE_FROM_VAULT_CAVE_PASSAGE_SOUTH:
    case VAULT_CAVE_VAMBRACE_SOUTH_FROM_FROST_ROOM:
      if (GameFlag_Get(FLAG_VAULT_CAVE_SOUTH_PILLAR_ON_SWITCH) != 0) {
        VaultCave_ApplySouthernSwitchCells();
        Actor_SetPosition(ACTOR_SOUTHERN_PILLAR, 248, 876);
      }
      Actor_Get(ACTOR_PILLAR_EAST_OF_SOUTHERN_SWITCH)->priorityFlags =
          ACTOR_PRIORITY_UNDERFOOT;
      break;
    }
    Actor_SetAnimation(ACTOR_PILLAR_EAST_OF_SOUTHERN_SWITCH,
                       FLAME_PILLAR_ANIMATION_BURNING);
    Actor_SetAnimation(ACTOR_SOUTHERN_PILLAR, FLAME_PILLAR_ANIMATION_BURNING);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_PILLAR_EAST_OF_SOUTHERN_SWITCH), 0);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_SOUTHERN_PILLAR), 0);
    Actor_Get(ACTOR_SOUTHERN_PILLAR)->collisionFlags = ACTOR_SOLID;
  }
  if (gMainRoutineWork.next == (s32)&SceneId_VaultCaveMouth) {
    Actor_SetAnimation(ACTOR_PILLAR_EAST_OF_NORTHERN_SWITCH,
                       FLAME_PILLAR_ANIMATION_BURNING);
    if (GameFlag_Get(FLAG_VAULT_CAVE_NORTH_PILLAR_STRUCK) == 0) {
      Actor_SetAnimation(ACTOR_NORTHERN_PILLAR, FLAME_PILLAR_ANIMATION_BURNING);
    }
    Actor_SetSpriteFlags(Actor_Get(ACTOR_PILLAR_EAST_OF_NORTHERN_SWITCH), 0);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_NORTHERN_PILLAR), 0);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_PILLAR_SOUTH_OF_NORTHERN_SWITCH), 0);
    Actor_Get(ACTOR_NORTHERN_PILLAR)->collisionFlags |= 0x80;
    Actor_Get(ACTOR_PILLAR_SOUTH_OF_NORTHERN_SWITCH)->collisionFlags |= 0x80;
    switch (gMainRoutineWork.entrance) {
    case VAULT_CAVE_MOUTH_NORTH_FROM_SOUTH:
    case VAULT_CAVE_MOUTH_FROM_VAULT_CAVE_VAMBRACE_NORTH:
      VaultCave_StartTimedFalls();
      Actor_Get(ACTOR_FIRST_TIMED_WATER_DROP)->collisionFlags = ACTOR_PUSHABLE;
      Actor_Get(ACTOR_FIRST_TIMED_WATER_DROP + 1)->collisionFlags =
          ACTOR_PUSHABLE;
      Actor_Get(ACTOR_FIRST_TIMED_WATER_DROP + 2)->collisionFlags =
          ACTOR_PUSHABLE;
      Actor_Get(ACTOR_FIRST_TIMED_WATER_DROP + 3)->collisionFlags =
          ACTOR_PUSHABLE;
      Actor_Get(ACTOR_PILLAR_EAST_OF_NORTHERN_SWITCH)->collisionFlags =
          ACTOR_SOLID;
      Actor_Get(ACTOR_NORTHERN_PILLAR)->collisionFlags = ACTOR_SOLID;
      Actor_Get(ACTOR_PILLAR_SOUTH_OF_NORTHERN_SWITCH)->collisionFlags =
          ACTOR_SOLID;
      if (GameFlag_Get(FLAG_VAULT_CAVE_NORTH_PILLAR_ON_SWITCH) != 0) {
        VaultCave_ApplyNorthernSwitchCells();
        Actor_SetPosition(ACTOR_NORTHERN_PILLAR, 264, 204);
      }
      break;
    }
  }
  // Retreat always returns to the foot of SceneId_VaultCavePassage's ladder
  // from Vault.
  gMainRoutineWork.retreatEntrance = VAULT_CAVE_PASSAGE_FROM_VAULT_REVISIT;
  gMainRoutineWork.retreatScene = (s32)&SceneId_VaultCavePassage;
  return 0;
}

// clang-format off
const s32 gVaultCave_DirectionSteps[] = {
    DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(16, 0),
};

const s32 gVaultCave_PuffScript[] = {
    SC_RELEASE,
};

struct SceneEntrance gVaultCave_FallbackEntrances[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gVaultCavePassage_Entrances[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { VAULT_CAVE_PASSAGE_FROM_VAULT_CAVE_MOUTH, CONDITION_ALWAYS, 424, 0, 216, FACING_SOUTH, 0, 72, 8, 480, 320, 0 },
    { VAULT_CAVE_PASSAGE_FROM_VAULT_CAVE_VAMBRACE_NORTHWEST, CONDITION_ALWAYS, 136, 0, 104, FACING_SOUTH, 0, 72, 8, 480, 320, 0 },
    { VAULT_CAVE_PASSAGE_NORTH_FROM_WATER_ROOM_WEST, CONDITION_ALWAYS, 120, 0, 256, FACING_NORTH, 0, 72, 8, 480, 320, 0 },
    { VAULT_CAVE_PASSAGE_NORTH_FROM_WATER_ROOM_EAST, CONDITION_ALWAYS, 184, 0, 264, FACING_NORTH, 0, 72, 8, 480, 320, 0 },
    { VAULT_CAVE_PASSAGE_WATER_ROOM_FROM_NORTH_WEST, CONDITION_ALWAYS, 344, 0, 464, FACING_SOUTH, 0, 280, 360, 640, 640, 0 },
    { VAULT_CAVE_PASSAGE_FROM_VAULT_CAVE_VAMBRACE_EAST, CONDITION_ALWAYS, 488, 0, 488, FACING_SOUTH, 0, 280, 360, 640, 640, 0 },
    { VAULT_CAVE_PASSAGE_FROM_VAULT_CAVE_VAMBRACE_SOUTHEAST, CONDITION_ALWAYS, 584, 0, 608, FACING_SOUTH, 0, 280, 360, 640, 640, 0 },
    { VAULT_CAVE_PASSAGE_WATER_ROOM_FROM_CHEST, CONDITION_ALWAYS, 344, 0, 632, FACING_NORTH, 0, 280, 360, 640, 640, 0 },
    { VAULT_CAVE_PASSAGE_WATER_ROOM_FROM_NORTH_EAST, CONDITION_ALWAYS, 408, 0, 472, FACING_SOUTH, 0, 280, 360, 640, 640, 0 },
    { VAULT_CAVE_PASSAGE_CHEST_FROM_WATER_ROOM, CONDITION_ALWAYS, 88, 0, 704, FACING_SOUTH, 0, 40, 632, 352, 912, 0 },
    { VAULT_CAVE_PASSAGE_CHEST_FROM_LADDER, CONDITION_ALWAYS, 184, 0, 864, FACING_SOUTH, 0, 40, 632, 352, 912, 0 },
    { VAULT_CAVE_PASSAGE_FROM_VAULT_REVISIT, CONDITION_ALWAYS, 456, 0, 800, FACING_SOUTH, 0, 336, 696, 576, 856, 0 },
    { VAULT_CAVE_PASSAGE_LADDER_FROM_CHEST, CONDITION_ALWAYS, 456, 0, 840, FACING_NORTH, 0, 336, 696, 576, 856, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gVaultCaveVambrace_Entrances[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { VAULT_CAVE_VAMBRACE_FROM_VAULT_CAVE_PASSAGE_NORTH, CONDITION_ALWAYS, 136, 0, 104, FACING_SOUTH, 0, 40, 8, 400, 328, 0 },
    { VAULT_CAVE_VAMBRACE_FROM_VAULT_CAVE_MOUTH_NORTH, CONDITION_ALWAYS, 344, 0, 152, FACING_SOUTH, 0, 40, 8, 400, 328, 0 },
    { VAULT_CAVE_VAMBRACE_NORTH_FROM_FROST_ROOM, CONDITION_ALWAYS, 344, 0, 184, FACING_NORTH, 0, 40, 8, 400, 328, 0 },
    { VAULT_CAVE_VAMBRACE_NORTH_FROM_WEST, CONDITION_ALWAYS, 168, 0, 328, FACING_NORTH, 0, 40, 8, 400, 328, 0 },
    { VAULT_CAVE_VAMBRACE_WEST_FROM_NORTH, CONDITION_ALWAYS, 56, 0, 480, FACING_SOUTH, 0, 8, 408, 248, 656, 0 },
    { VAULT_CAVE_VAMBRACE_FROM_VAULT_CAVE_PASSAGE_SOUTHWEST, CONDITION_ALWAYS, 152, 0, 472, FACING_SOUTH, 0, 8, 408, 248, 656, 0 },
    { VAULT_CAVE_VAMBRACE_FROM_VAULT_CAVE_MOUTH_SOUTH, CONDITION_ALWAYS, 120, 0, 576, FACING_SOUTH, 0, 8, 408, 248, 656, 0 },
    { VAULT_CAVE_VAMBRACE_FROST_ROOM_FROM_NORTH, CONDITION_ALWAYS, 344, 0, 456, FACING_SOUTH, 0, 280, 344, 544, 672, 0 },
    { VAULT_CAVE_VAMBRACE_FROST_ROOM_FROM_SOUTH, CONDITION_ALWAYS, 424, 0, 664, FACING_NORTH, 0, 280, 344, 544, 672, 0 },
    { VAULT_CAVE_VAMBRACE_FROST_ROOM_FROM_CHEST, CONDITION_ALWAYS, 456, 0, 472, FACING_SOUTH, 0, 280, 344, 544, 672, 0 },
    { VAULT_CAVE_VAMBRACE_FROM_VAULT_CAVE_PASSAGE_SOUTH, CONDITION_ALWAYS, 200, 0, 776, FACING_SOUTH, 0, 104, 696, 344, 944, 0 },
    { VAULT_CAVE_VAMBRACE_SOUTH_FROM_FROST_ROOM, CONDITION_ALWAYS, 264, 0, 808, FACING_SOUTH, 0, 104, 696, 344, 944, 0 },
    { VAULT_CAVE_VAMBRACE_CHEST_FROM_FROST_ROOM, CONDITION_ALWAYS, 536, 0, 168, FACING_NORTH, 0, 416, 24, 656, 184, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gVaultCaveMouth_Entrances[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { VAULT_CAVE_MOUTH_FROM_VAULT_REVISIT, CONDITION_ALWAYS, 312, 0, 664, FACING_NORTH, 0, 200, 520, 440, 680, 0 },
    { VAULT_CAVE_MOUTH_FROM_VAULT_CAVE_PASSAGE, CONDITION_ALWAYS, 312, 0, 600, FACING_SOUTH, 0, 200, 520, 440, 680, 0 },
    { VAULT_CAVE_MOUTH_FROM_VAULT_CAVE_VAMBRACE_SOUTH, CONDITION_ALWAYS, 88, 0, 568, FACING_SOUTH, 0, 8, 472, 248, 632, 0 },
    { VAULT_CAVE_MOUTH_SOUTH_FROM_NORTH, CONDITION_ALWAYS, 168, 0, 544, FACING_SOUTH, 0, 8, 472, 248, 632, 0 },
    { VAULT_CAVE_MOUTH_NORTH_FROM_SOUTH, CONDITION_ALWAYS, 248, 0, 336, FACING_NORTH, 0, 128, 40, 376, 384, 0 },
    { VAULT_CAVE_MOUTH_FROM_VAULT_CAVE_VAMBRACE_NORTH, CONDITION_ALWAYS, 280, 0, 104, FACING_SOUTH, 0, 128, 40, 376, 384, 0 },
    { SCENE_TABLE_END },
};

struct SceneExitRegion gVaultCave_Regions[] = {
    { 447, 47, 783, 463, 63, 799, -1, VAULT_CAVE_PASSAGE_TO_VAULT_REVISIT },
    { -1, 0, 0, 0, 0, 0, 0, 0 },
};

u32 gVaultCave_FallbackExits[] = {
    SCENE_EXITS_END,
};

u32 gVaultCavePassage_Exits[] = {
    SCENE_EXITS(VaultCavePassage),
    SCENE_EXIT(VAULT_CAVE_PASSAGE_TO_VAULT_CAVE_MOUTH, VaultCaveMouth, VAULT_CAVE_MOUTH_FROM_VAULT_CAVE_PASSAGE),
    SCENE_EXIT(VAULT_CAVE_PASSAGE_TO_VAULT_CAVE_VAMBRACE_NORTHWEST, VaultCaveVambrace, VAULT_CAVE_VAMBRACE_FROM_VAULT_CAVE_PASSAGE_NORTH),
    SCENE_EXIT(VAULT_CAVE_PASSAGE_NORTH_TO_WATER_ROOM_WEST, VaultCavePassage, VAULT_CAVE_PASSAGE_WATER_ROOM_FROM_NORTH_WEST),
    SCENE_EXIT(VAULT_CAVE_PASSAGE_NORTH_TO_WATER_ROOM_EAST, VaultCavePassage, VAULT_CAVE_PASSAGE_WATER_ROOM_FROM_NORTH_EAST),
    SCENE_EXIT(VAULT_CAVE_PASSAGE_WATER_ROOM_TO_NORTH_WEST, VaultCavePassage, VAULT_CAVE_PASSAGE_NORTH_FROM_WATER_ROOM_WEST),
    SCENE_EXIT(VAULT_CAVE_PASSAGE_TO_VAULT_CAVE_VAMBRACE_EAST, VaultCaveVambrace, VAULT_CAVE_VAMBRACE_FROM_VAULT_CAVE_PASSAGE_SOUTHWEST),
    SCENE_EXIT(VAULT_CAVE_PASSAGE_TO_VAULT_CAVE_VAMBRACE_SOUTHEAST, VaultCaveVambrace, VAULT_CAVE_VAMBRACE_FROM_VAULT_CAVE_PASSAGE_SOUTH),
    SCENE_EXIT(VAULT_CAVE_PASSAGE_WATER_ROOM_TO_CHEST, VaultCavePassage, VAULT_CAVE_PASSAGE_CHEST_FROM_WATER_ROOM),
    SCENE_EXIT(VAULT_CAVE_PASSAGE_CHEST_TO_WATER_ROOM, VaultCavePassage, VAULT_CAVE_PASSAGE_WATER_ROOM_FROM_CHEST),
    SCENE_EXIT(VAULT_CAVE_PASSAGE_TO_VAULT_REVISIT, VaultRevisit, VAULT_REVISIT_FROM_VAULT_CAVE_PASSAGE),
    // A second VAULT_CAVE_PASSAGE_TO_VAULT_REVISIT, to
    // VAULT_CAVE_PASSAGE_LADDER_FROM_CHEST.
    SCENE_EXIT(VAULT_CAVE_PASSAGE_TO_VAULT_REVISIT, VaultCavePassage, VAULT_CAVE_PASSAGE_LADDER_FROM_CHEST),
    SCENE_EXIT(VAULT_CAVE_PASSAGE_LADDER_TO_CHEST, VaultCavePassage, VAULT_CAVE_PASSAGE_CHEST_FROM_LADDER),
    SCENE_EXIT(VAULT_CAVE_PASSAGE_CHEST_TO_LADDER, VaultCavePassage, VAULT_CAVE_PASSAGE_LADDER_FROM_CHEST),
    SCENE_EXIT(VAULT_CAVE_PASSAGE_WATER_ROOM_TO_NORTH_EAST, VaultCavePassage, VAULT_CAVE_PASSAGE_NORTH_FROM_WATER_ROOM_EAST),
    SCENE_EXITS_END,
};

u32 gVaultCaveVambrace_Exits[] = {
    SCENE_EXITS(VaultCaveVambrace),
    SCENE_EXIT(VAULT_CAVE_VAMBRACE_TO_VAULT_CAVE_PASSAGE_NORTH, VaultCavePassage, VAULT_CAVE_PASSAGE_FROM_VAULT_CAVE_VAMBRACE_NORTHWEST),
    SCENE_EXIT(VAULT_CAVE_VAMBRACE_TO_VAULT_CAVE_MOUTH_NORTH, VaultCaveMouth, VAULT_CAVE_MOUTH_FROM_VAULT_CAVE_VAMBRACE_NORTH),
    SCENE_EXIT(VAULT_CAVE_VAMBRACE_NORTH_TO_FROST_ROOM, VaultCaveVambrace, VAULT_CAVE_VAMBRACE_FROST_ROOM_FROM_NORTH),
    SCENE_EXIT(VAULT_CAVE_VAMBRACE_NORTH_TO_WEST, VaultCaveVambrace, VAULT_CAVE_VAMBRACE_WEST_FROM_NORTH),
    SCENE_EXIT(VAULT_CAVE_VAMBRACE_WEST_TO_NORTH, VaultCaveVambrace, VAULT_CAVE_VAMBRACE_NORTH_FROM_WEST),
    SCENE_EXIT(VAULT_CAVE_VAMBRACE_TO_VAULT_CAVE_PASSAGE_SOUTHWEST, VaultCavePassage, VAULT_CAVE_PASSAGE_FROM_VAULT_CAVE_VAMBRACE_EAST),
    SCENE_EXIT(VAULT_CAVE_VAMBRACE_TO_VAULT_CAVE_MOUTH_SOUTH, VaultCaveMouth, VAULT_CAVE_MOUTH_FROM_VAULT_CAVE_VAMBRACE_SOUTH),
    SCENE_EXIT(VAULT_CAVE_VAMBRACE_FROST_ROOM_TO_NORTH, VaultCaveVambrace, VAULT_CAVE_VAMBRACE_NORTH_FROM_FROST_ROOM),
    SCENE_EXIT(VAULT_CAVE_VAMBRACE_FROST_ROOM_TO_SOUTH, VaultCaveVambrace, VAULT_CAVE_VAMBRACE_SOUTH_FROM_FROST_ROOM),
    SCENE_EXIT(VAULT_CAVE_VAMBRACE_TO_VAULT_CAVE_PASSAGE_SOUTH, VaultCavePassage, VAULT_CAVE_PASSAGE_FROM_VAULT_CAVE_VAMBRACE_SOUTHEAST),
    SCENE_EXIT(VAULT_CAVE_VAMBRACE_SOUTH_TO_FROST_ROOM, VaultCaveVambrace, VAULT_CAVE_VAMBRACE_FROST_ROOM_FROM_SOUTH),
    SCENE_EXIT(VAULT_CAVE_VAMBRACE_FROST_ROOM_TO_CHEST, VaultCaveVambrace, VAULT_CAVE_VAMBRACE_CHEST_FROM_FROST_ROOM),
    SCENE_EXIT(VAULT_CAVE_VAMBRACE_CHEST_TO_FROST_ROOM, VaultCaveVambrace, VAULT_CAVE_VAMBRACE_FROST_ROOM_FROM_CHEST),
    SCENE_EXITS_END,
};

u32 gVaultCaveMouth_Exits[] = {
    SCENE_EXITS(VaultCaveMouth),
    SCENE_EXIT(VAULT_CAVE_MOUTH_TO_VAULT_REVISIT, VaultRevisit, VAULT_REVISIT_FROM_VAULT_CAVE_MOUTH),
    SCENE_EXIT(VAULT_CAVE_MOUTH_TO_VAULT_CAVE_PASSAGE, VaultCavePassage, VAULT_CAVE_PASSAGE_FROM_VAULT_CAVE_MOUTH),
    SCENE_EXIT(VAULT_CAVE_MOUTH_TO_VAULT_CAVE_VAMBRACE_SOUTH, VaultCaveVambrace, VAULT_CAVE_VAMBRACE_FROM_VAULT_CAVE_MOUTH_SOUTH),
    SCENE_EXIT(VAULT_CAVE_MOUTH_SOUTH_TO_NORTH, VaultCaveMouth, VAULT_CAVE_MOUTH_NORTH_FROM_SOUTH),
    SCENE_EXIT(VAULT_CAVE_MOUTH_NORTH_TO_SOUTH, VaultCaveMouth, VAULT_CAVE_MOUTH_SOUTH_FROM_NORTH),
    SCENE_EXIT(VAULT_CAVE_MOUTH_TO_VAULT_CAVE_VAMBRACE_NORTH, VaultCaveVambrace, VAULT_CAVE_VAMBRACE_FROM_VAULT_CAVE_MOUTH_NORTH),
    SCENE_EXITS_END,
};

struct ActorPlacement gVaultCave_FallbackActorPlacements[] = {
    { SCENE_TABLE_END },
};

struct ActorPlacement gVaultCavePassage_ActorPlacements[] = {
    { SCENE_TABLE_END },
};

struct ActorPlacement gVaultCaveVambrace_ActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_FLAME_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        280, 0, 876, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_FLAME_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        184, 0, 892, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_DROPLET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        372, 0, 512, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_DROPLET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        372, 0, 544, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_DROPLET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        388, 0, 544, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_DROPLET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        420, 0, 512, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_DROPLET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        420, 0, 544, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_DROPLET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        452, 0, 528, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        376, 0, 520, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        376, 0, 552, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        392, 0, 552, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        424, 0, 520, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        424, 0, 552, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        456, 0, 536, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_IVY, FLAG_VAULT_CAVE_IVY_BLOWN_AWAY, MOVEMENT_TYPE_STAND,
        456, 0, 456, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gVaultCaveMouth_ActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_FLAME_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        296, 0, 204, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_FLAME_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        264, 0, 236, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_FLAME_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        264, 0, 300, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_DROPLET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        248, 0, 232, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_DROPLET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        264, 0, 248, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_DROPLET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        264, 0, 264, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_DROPLET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        280, 0, 280, FACING_SOUTH, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

struct Trigger gVaultCave_FallbackTriggers[] = {
    { TRIGGER_TABLE_END },
};

struct Trigger gVaultCavePassage_Triggers[] = {
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, VAULT_CAVE_PASSAGE_TO_VAULT_CAVE_MOUTH },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, VAULT_CAVE_PASSAGE_TO_VAULT_CAVE_VAMBRACE_NORTHWEST },
    { TRIGGER_EXIT, 3, CONDITION_ALWAYS, VAULT_CAVE_PASSAGE_NORTH_TO_WATER_ROOM_WEST },
    { TRIGGER_EXIT, 4, CONDITION_ALWAYS, VAULT_CAVE_PASSAGE_NORTH_TO_WATER_ROOM_EAST },
    { TRIGGER_EXIT, 5, CONDITION_ALWAYS, VAULT_CAVE_PASSAGE_WATER_ROOM_TO_NORTH_WEST },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 6, CONDITION_ALWAYS, VAULT_CAVE_PASSAGE_TO_VAULT_CAVE_VAMBRACE_EAST },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 7, CONDITION_ALWAYS, VAULT_CAVE_PASSAGE_TO_VAULT_CAVE_VAMBRACE_SOUTHEAST },
    { TRIGGER_EXIT, 8, CONDITION_ALWAYS, VAULT_CAVE_PASSAGE_WATER_ROOM_TO_CHEST },
    { TRIGGER_EXIT, 9, CONDITION_ALWAYS, VAULT_CAVE_PASSAGE_CHEST_TO_WATER_ROOM },
    { TRIGGER_EXIT, 10, CONDITION_ALWAYS, VAULT_CAVE_PASSAGE_TO_VAULT_REVISIT },
    { TRIGGER_EXIT, 11, CONDITION_ALWAYS, VAULT_CAVE_PASSAGE_LADDER_TO_CHEST },
    { TRIGGER_EXIT, 12, CONDITION_ALWAYS, VAULT_CAVE_PASSAGE_CHEST_TO_LADDER },
    { TRIGGER_EXIT, 13, CONDITION_ALWAYS, VAULT_CAVE_PASSAGE_WATER_ROOM_TO_NORTH_EAST },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_VAULT_CAVE_SWITCH, CONDITION_ALWAYS, EVENT(VaultCave_FlipSwitch) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_VAULT_CAVE_LOCKED_DOOR, CONDITION_ALWAYS, EVENT(VaultCave_ShowLockedDoor) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), CELL_BEHAVIOR_VAULT_CAVE_CHEST, FLAG_VAULT_CAVE_PASSAGE_MIMIC_BEATEN, FIND_MIMIC | 3 },
    { TRIGGER_TABLE_END },
};

struct Trigger gVaultCaveVambrace_Triggers[] = {
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, VAULT_CAVE_VAMBRACE_TO_VAULT_CAVE_PASSAGE_NORTH },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, VAULT_CAVE_VAMBRACE_TO_VAULT_CAVE_MOUTH_NORTH },
    { TRIGGER_EXIT, 3, CONDITION_ALWAYS, VAULT_CAVE_VAMBRACE_NORTH_TO_FROST_ROOM },
    { TRIGGER_EXIT, 4, CONDITION_ALWAYS, VAULT_CAVE_VAMBRACE_NORTH_TO_WEST },
    { TRIGGER_EXIT, 5, CONDITION_ALWAYS, VAULT_CAVE_VAMBRACE_WEST_TO_NORTH },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 6, CONDITION_ALWAYS, VAULT_CAVE_VAMBRACE_TO_VAULT_CAVE_PASSAGE_SOUTHWEST },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 7, CONDITION_ALWAYS, VAULT_CAVE_VAMBRACE_TO_VAULT_CAVE_MOUTH_SOUTH },
    { TRIGGER_EXIT, 8, CONDITION_ALWAYS, VAULT_CAVE_VAMBRACE_FROST_ROOM_TO_NORTH },
    { TRIGGER_EXIT, 9, CONDITION_ALWAYS, VAULT_CAVE_VAMBRACE_FROST_ROOM_TO_SOUTH },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 10, CONDITION_ALWAYS, VAULT_CAVE_VAMBRACE_TO_VAULT_CAVE_PASSAGE_SOUTH },
    { TRIGGER_EXIT, 11, CONDITION_ALWAYS, VAULT_CAVE_VAMBRACE_SOUTH_TO_FROST_ROOM },
    { TRIGGER_EXIT, 13, CONDITION_ALWAYS, VAULT_CAVE_VAMBRACE_FROST_ROOM_TO_CHEST },
    { TRIGGER_EXIT, 14, CONDITION_ALWAYS, VAULT_CAVE_VAMBRACE_CHEST_TO_FROST_ROOM },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), CELL_BEHAVIOR_VAULT_CAVE_CHEST, FLAG_VAULT_CAVE_VAMBRACE_CHEST, FIND_ITEM(ITEM_VAMBRACE) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_VAULT_CAVE_LOCKED_DOOR, CONDITION_ALWAYS, EVENT(VaultCave_ShowLockedDoor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SOUTHERN_PILLAR, FLAG_VAULT_CAVE_SOUTH_PILLAR_ON_SWITCH, EVENT(VaultCave_MoveSouthernPillar) },
    { TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, CELL_BEHAVIOR_VAULT_CAVE_SOUTHERN_PILLAR, CONDITION_ALWAYS, EVENT(VaultCave_PushSouthernPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_WEST_UPPER_PUDDLE, FLAG_VAULT_CAVE_WEST_UPPER_PUDDLE_FROZEN, EVENT(VaultCave_FreezeWestUpperPuddle) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_WEST_LOWER_PUDDLE, FLAG_VAULT_CAVE_WEST_LOWER_PUDDLE_FROZEN, EVENT(VaultCave_FreezeWestLowerPuddle) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_INNER_WEST_LOWER_PUDDLE, FLAG_VAULT_CAVE_INNER_WEST_LOWER_PUDDLE_FROZEN, EVENT(VaultCave_FreezeInnerWestLowerPuddle) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_EAST_UPPER_PUDDLE, FLAG_VAULT_CAVE_EAST_UPPER_PUDDLE_FROZEN, EVENT(VaultCave_FreezeEastUpperPuddle) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_EAST_LOWER_PUDDLE, FLAG_VAULT_CAVE_EAST_LOWER_PUDDLE_FROZEN, EVENT(VaultCave_FreezeEastLowerPuddle) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_FAR_EAST_PUDDLE, FLAG_VAULT_CAVE_FAR_EAST_PUDDLE_FROZEN, EVENT(VaultCave_FreezeFarEastPuddle) },
    { PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_IVY, FLAG_VAULT_CAVE_IVY_BLOWN_AWAY, EVENT(VaultCave_UseWhirlwind) },
    { TRIGGER_TABLE_END },
};

struct Trigger gVaultCaveMouth_Triggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, VAULT_CAVE_MOUTH_TO_VAULT_REVISIT },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, VAULT_CAVE_MOUTH_TO_VAULT_CAVE_PASSAGE },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 3, CONDITION_ALWAYS, VAULT_CAVE_MOUTH_TO_VAULT_CAVE_VAMBRACE_SOUTH },
    { TRIGGER_EXIT, 4, CONDITION_ALWAYS, VAULT_CAVE_MOUTH_SOUTH_TO_NORTH },
    { TRIGGER_EXIT, 5, CONDITION_ALWAYS, VAULT_CAVE_MOUTH_NORTH_TO_SOUTH },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 6, CONDITION_ALWAYS, VAULT_CAVE_MOUTH_TO_VAULT_CAVE_VAMBRACE_NORTH },
    { TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, CELL_BEHAVIOR_VAULT_CAVE_NORTHERN_PILLAR, CONDITION_ALWAYS, EVENT(VaultCave_PushNorthernPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_NORTHERN_PILLAR, FLAG_VAULT_CAVE_NORTH_PILLAR_ON_SWITCH, EVENT(VaultCave_MoveNorthernPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR_SOUTH_OF_NORTHERN_SWITCH, CONDITION_ALWAYS, EVENT(VaultCave_IgnoreMove) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_VAULT_CAVE_LOCKED_DOOR, CONDITION_ALWAYS, EVENT(VaultCave_ShowLockedDoor) },
    { TRIGGER_TABLE_END },
};

u32 gVaultCave_ScheduleFrames[] = {
    20, 80, 115, 170,
};
// clang-format on

s32 gVaultCave_ScheduleTimer = 0;

s32 gVaultCave_ScheduleIndex = 0;

s32 gVaultCave_DropFrames = 0;

s32 gVaultCave_SelectedDrop = 0;

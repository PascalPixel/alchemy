/// @file
/// Vault Cave, the optional cave under Vault that the party can explore once
/// it comes back to the town (SceneId_VaultRevisit), with the Vambrace in a
/// chest and Sap, the Venus Djinni, waiting outside its far end. Its three
/// parts are scenes of their own, all named Vault Cave (area 31).
///
/// The way in is the passage Reveal uncovers among Vault's six gravestones,
/// where the dog jumps once given a Bone; a ladder leads down from it. The
/// way out is a cave mouth in the town's north-east cliff, above which the
/// bell sent Sap.
///
/// The cave is a side trip of puzzles for Move, Frost and Whirlwind. No one
/// speaks, and the only messages are "The door is tightly locked." and "Isaac
/// flipped the switch."
///
/// - SceneId_VaultCavePassage holds the foot of the ladder from Vault
///   (VAULT_CAVE_PASSAGE_FROM_VAULT_REVISIT; Retreat leads there too). Its
///   right-hand branch ends at a chest that is a Mimic (FIND_MIMIC). Its pool
///   room has a switch on a ledge: flipping it shakes the cave and drains the
///   pool (VaultCave_FlipSwitch, FLAG_VAULT_CAVE_SWITCH_FLIPPED), and the
///   water runs down into the floor of the northern room of
///   SceneId_VaultCaveVambrace, which Scene_Initialize draws dry until then.
///   Coming down the ladder from Vault again resets the switch.
/// - SceneId_VaultCaveVambrace is up the left-hand branch. In the room of two
///   burning torches, Isaac pushes or Moves the west torch east onto the tile
///   with a fire crest (VaultCave_PushSouthernPillar,
///   VaultCave_MoveSouthernPillar): the puzzle chime plays and the wooden gate
///   beyond opens (FLAG_VAULT_CAVE_SOUTH_PILLAR_ON_SWITCH). North of it is the
///   room of six puddles, where drops of water fall from the ceiling one at a
///   time. Frost freezes a puddle into a tall ice pillar whose top Isaac can
///   leap across, and frozen pillars make a way over the room; a drop landing
///   on one stops on its top. Up on the room's ledge, Whirlwind blows away the
///   ivy over the way to the Vambrace's chest (FLAG_VAULT_CAVE_IVY_BLOWN_AWAY,
///   kept while the party stays in the cave).
/// - SceneId_VaultCaveMouth leads out to the cave mouth
///   (VAULT_CAVE_MOUTH_TO_VAULT_REVISIT). In its room of three torches, Isaac
///   must get the lower burning torch north onto the fire crest below the
///   wooden gate, past the unlit torch in the way, while drops of water fall
///   on a fixed beat (gVaultCave_ScheduleFrames). A drop that lands on the
///   burning torch puts its flame out (FLAG_VAULT_CAVE_NORTH_PILLAR_STRUCK),
///   and an unlit torch opens nothing until the room is entered again and the
///   flame comes back.
///
/// Each room has a door that only says it is locked
/// (VaultCave_ShowLockedDoor).
///
/// The pushes share VaultCave_PushPillarAhead, which moves the torch the
/// leader faces one cell unless another actor or the rock is in the way. The
/// water drops run as tasks started by Scene_Initialize.

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "types.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/actor_script.h"
#include "field/event.h"
#include "field/field.h"
#include "field/map_grid_scroll.h"
#include "field/pushed_actor.h"
#include "field/scene.h"
#include "field/scene_tables.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "scene/actor_step.h"
#include "sound/sound.h"
#include "system/heap_slots.h"
#include "system/io_write_queue.h"
#include "system/main_routine_state.h"
#include "system/system.h"
#include "system/system_call.h"
#include "text/msg_ids.h"

extern s32 gPushedActor_DirectionSteps[];
extern const s32 gVaultCave_SplashScript[];
extern struct SceneEntrance gScene_FallbackEntrances[];
extern struct SceneEntrance gVaultCavePassage_Entrances[];
extern struct SceneEntrance gVaultCaveVambrace_Entrances[];
extern struct SceneEntrance gVaultCaveMouth_Entrances[];
extern struct SceneExitRegion gVaultCave_Regions[];
extern u32 gVaultCave_FallbackExits[];
extern u32 gVaultCavePassage_Exits[];
extern u32 gVaultCaveVambrace_Exits[];
extern u32 gVaultCaveMouth_Exits[];
extern struct ActorPlacement gVaultCave_FallbackActorPlacements[];
extern struct ActorPlacement gVaultCavePassage_ActorPlacements[];
extern struct ActorPlacement gVaultCaveVambrace_ActorPlacements[];
extern struct ActorPlacement gVaultCaveMouth_ActorPlacements[];
extern struct Trigger gVaultCave_FallbackTriggers[];
extern struct Trigger gVaultCavePassage_Triggers[];
extern struct Trigger gVaultCaveVambrace_Triggers[];
extern struct Trigger gVaultCaveMouth_Triggers[];
extern u32 gVaultCave_ScheduleFrames[];
extern s32 gVaultCave_ScheduleTimer;
extern s32 gVaultCave_ScheduleIndex;
extern s32 gVaultCave_DropFrames;
extern s32 gVaultCave_SelectedDrop;

static void VaultCave_ApplySouthernSwitchCells(void);
static void VaultCave_UseUnderfootFrostPriorities(void);
static void VaultCave_UseAutomaticFrostPriorities(void);
static void VaultCave_SpawnSplash(struct Actor *source);

/// The map cell a 16.16 coordinate lies in, by division: the same as
/// FIELD_CELL's shift but for a coordinate below 0, which it rounds toward
/// cell 0.
#define CELL_OF(position) ((position) / toFIXED(16))

/// The cell behaviors of the cells the events answer.
enum VaultCaveCellBehavior {
  /// The switch on the ledge in the pool room: cell (28, 29).
  CELL_BEHAVIOR_VAULT_CAVE_SWITCH = 20,
  /// The floor of the room of three torches, where Isaac pushes them: cells
  /// (12, 14) to (20, 17) and the crest's cell (16, 12).
  CELL_BEHAVIOR_VAULT_CAVE_NORTHERN_PILLAR = 50,
  /// The floor of the room of two torches, where Isaac pushes them: cells
  /// (10, 55) to (16, 55) and the rows either side.
  CELL_BEHAVIOR_VAULT_CAVE_SOUTHERN_PILLAR = 60,
  CELL_BEHAVIOR_VAULT_CAVE_LOCKED_DOOR = 99,
  CELL_BEHAVIOR_VAULT_CAVE_CHEST = 100
};

/// Scene flags, and one area flag. The six puddles of
/// SceneId_VaultCaveVambrace keep their frozen state in six flags in a row,
/// read by index as well.
enum {
  /// The upper western puddle, the first of the six.
  FLAG_VAULT_CAVE_WEST_UPPER_PUDDLE_FROZEN = FLAG_SCENE_TEMP_00,
  FLAG_VAULT_CAVE_WEST_LOWER_PUDDLE_FROZEN = FLAG_SCENE_TEMP_01,
  FLAG_VAULT_CAVE_INNER_WEST_LOWER_PUDDLE_FROZEN = FLAG_SCENE_TEMP_02,
  FLAG_VAULT_CAVE_EAST_UPPER_PUDDLE_FROZEN = FLAG_SCENE_TEMP_03,
  FLAG_VAULT_CAVE_EAST_LOWER_PUDDLE_FROZEN = FLAG_SCENE_TEMP_04,
  FLAG_VAULT_CAVE_FAR_EAST_PUDDLE_FROZEN = FLAG_SCENE_TEMP_05,
  /// A drop of water has put out the flame of the lower torch in the room of
  /// three torches. The drop's cell matched the torch's
  /// (VaultCave_ResetFallingActorAtActorCell).
  FLAG_VAULT_CAVE_NORTH_PILLAR_STRUCK = FLAG_SCENE_TEMP_07,
  /// Whirlwind has blown the ivy away.
  FLAG_VAULT_CAVE_IVY_BLOWN_AWAY = FLAG_AREA_TEMP_00
};

/// SPRITE_FLAME_PILLAR's animations (flame_pillar.tsv).
enum FlamePillarAnimation {
  FLAME_PILLAR_ANIMATION_UNLIT = 1,  ///< the pillar, no flame on top
  FLAME_PILLAR_ANIMATION_BURNING = 2 ///< a flame flickering on top
};

/// The actors of SceneId_VaultCaveVambrace, numbering the rows of
/// gVaultCaveVambrace_ActorPlacements.
enum VaultCaveVambraceActorId {
  /// The burning torch beside the wooden gate in the room of two torches, two
  /// cells east of the fire crest. It is drawn underfoot when the party
  /// arrives by VAULT_CAVE_VAMBRACE_FROM_VAULT_CAVE_PASSAGE_SOUTH or
  /// VAULT_CAVE_VAMBRACE_SOUTH_FROM_FROST_ROOM.
  ACTOR_PILLAR_EAST_OF_SOUTHERN_SWITCH = ACTOR_FIRST_PLACED,
  /// The west torch, which goes onto the fire crest at cell (15, 54).
  ACTOR_SOUTHERN_PILLAR,
  /// The first of six drops of water that fall at random, one every 64 frames
  /// (VaultCave_UpdateFrostControlledFallingActors). VaultCave_SpawnSplash also
  /// uses their sprite for the splash where one lands.
  ACTOR_FIRST_WATER_DROP,
  /// The six puddles under the drops, in their order and in the order of their
  /// flags (FLAG_VAULT_CAVE_WEST_UPPER_PUDDLE_FROZEN to
  /// FLAG_VAULT_CAVE_FAR_EAST_PUDDLE_FROZEN). Frost freezes each into an ice
  /// pillar, as in bilibin_cave.c, and a drop landing on one stops on its top,
  /// 32 pixels up.
  ACTOR_WEST_UPPER_PUDDLE = ACTOR_FIRST_WATER_DROP + 6,
  ACTOR_WEST_LOWER_PUDDLE,
  ACTOR_INNER_WEST_LOWER_PUDDLE,
  ACTOR_EAST_UPPER_PUDDLE,
  ACTOR_EAST_LOWER_PUDDLE,
  ACTOR_FAR_EAST_PUDDLE,
  /// The ivy on the ledge above the room of six puddles, over the way to the
  /// Vambrace's chest (SPRITE_IVY, as in bilibin.c, drawn half as tall again).
  /// It is placed until Whirlwind blows it away
  /// (FLAG_VAULT_CAVE_IVY_BLOWN_AWAY).
  ACTOR_IVY
};

/// The actors of SceneId_VaultCaveMouth, numbering the rows of
/// gVaultCaveMouth_ActorPlacements. The fire crest in the room of three
/// torches is map cell (16, 12).
enum VaultCaveMouthActorId {
  /// The burning torch beside the wooden gate, two cells east of the fire
  /// crest.
  ACTOR_PILLAR_EAST_OF_NORTHERN_SWITCH = ACTOR_FIRST_PLACED,
  /// The unlit torch two cells south of the fire crest, in the way.
  ACTOR_PILLAR_SOUTH_OF_NORTHERN_SWITCH,
  /// The lower burning torch, which goes onto the fire crest.
  ACTOR_NORTHERN_PILLAR,
  /// The first of four drops of water that fall on a fixed schedule
  /// (VaultCave_UpdateTimedFallingActors).
  ACTOR_FIRST_TIMED_WATER_DROP
};

const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 sceneId = gMainRoutineState.next;

  if (sceneId == SCENE_VAULT_CAVE_PASSAGE) {
    return gVaultCavePassage_Entrances;
  }
  if (sceneId == SCENE_VAULT_CAVE_VAMBRACE) {
    return gVaultCaveVambrace_Entrances;
  }
  if (sceneId == SCENE_VAULT_CAVE_MOUTH) {
    return gVaultCaveMouth_Entrances;
  }
  return gScene_FallbackEntrances;
}

/// Returns the region boxes used by the cave's exits.
const struct SceneExitRegion *Scene_GetExitRegions(void) {
  return gVaultCave_Regions;
}

const u32 *Scene_GetExits(void) {
  s16 sceneId = gMainRoutineState.next;

  if (sceneId == SCENE_VAULT_CAVE_PASSAGE) {
    return gVaultCavePassage_Exits;
  }
  if (sceneId == SCENE_VAULT_CAVE_VAMBRACE) {
    return gVaultCaveVambrace_Exits;
  }
  if (sceneId == SCENE_VAULT_CAVE_MOUTH) {
    return gVaultCaveMouth_Exits;
  }
  return gVaultCave_FallbackExits;
}

/// Selects the torches, drops of water, puddles and ivy of the room.
const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s16 sceneId = gMainRoutineState.next;

  if (sceneId == SCENE_VAULT_CAVE_PASSAGE) {
    return gVaultCavePassage_ActorPlacements;
  }
  if (sceneId == SCENE_VAULT_CAVE_VAMBRACE) {
    return gVaultCaveVambrace_ActorPlacements;
  }
  if (sceneId == SCENE_VAULT_CAVE_MOUTH) {
    return gVaultCaveMouth_ActorPlacements;
  }
  return gVaultCave_FallbackActorPlacements;
}

/// Finds a placed actor with collision enabled at the requested map
/// cell.
static struct Actor *VaultCave_FindCollidableActorAtCell(s32 column, s32 row) {
  struct FieldState *state = gFieldState;
  u32 actorSlot;

  for (actorSlot = ACTOR_FIRST_PLACED; actorSlot <= ACTOR_LAST_ID;
       actorSlot++) {
    struct Actor *actor = state->actors[actorSlot];

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

/// Pushes the torch Isaac faces one cell, unless another torch or the rock is
/// in the way.
static void VaultCave_PushPillarAhead(void) {
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);
  s32 direction = FACING_INDEX(leader->facing);
  struct Actor *pillar;
  struct Actor *beyond;
  struct FieldPosition destination;
  s32 fallingIndex;
  s32 column;
  s32 row;

  column = (leader->x.part[0].integer +
            (FIXEDtoINT(gPushedActor_DirectionSteps[direction]))) >>
           4;
  row = (leader->z.part[0].integer +
         (s16)gPushedActor_DirectionSteps[direction]) >>
        4;
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
  column = (pillar->x.part[0].integer +
            (FIXEDtoINT(gPushedActor_DirectionSteps[direction]))) >>
           4;
  row = (pillar->z.part[0].integer +
         (s16)gPushedActor_DirectionSteps[direction]) >>
        4;
  beyond = VaultCave_FindCollidableActorAtCell(column, row);
  if (beyond != NULL && (beyond->collisionFlags & ACTOR_SOLID)) {
    return;
  }
  pillar->layer = 2;
  Actor_StepFrom(&destination, pillar, gPushedActor_DirectionSteps[direction]);
  if (FieldActor_CheckMovementCollision(pillar, &destination) > 0) {
    return;
  }
  Actor_SelectAnimation(leader, ANIM_PUSH);
  wait(15);
  Sound_Play(SE_PUSH_STEP);
  pillar->speedLimit = toFIXED(1) / 5;
  pillar->acceleration = toFIXED(1) / 5;
  FieldActor_SetMoveTargetFX(pillar, destination.x, destination.y,
                             destination.z);
  leader->speedLimit = toFIXED(1) / 5;
  leader->acceleration = toFIXED(1) / 5;
  FieldActor_SetMoveTargetFX(leader, destination.x, destination.y,
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

/// Whirlwind on the ivy on the ledge above the room of six puddles blows it
/// away, opening the way to the Vambrace's chest.
static void VaultCave_UseWhirlwind(void) {
  GameFlag_Set(FLAG_VAULT_CAVE_IVY_BLOWN_AWAY);
}

/// Searching a locked door in Vault Cave: "The door is tightly locked."
static void VaultCave_ShowLockedDoor(void) {
  FieldMessage_Show(MSGNO(MsgFieldDoorTightlyLocked), 1);
}

/// Selects the cave scene's examine triggers, movement checks and Psynergy
/// callbacks.
const struct Trigger *Scene_GetTriggers(void) {
  s16 sceneId = gMainRoutineState.next;

  if (sceneId == SCENE_VAULT_CAVE_PASSAGE) {
    return gVaultCavePassage_Triggers;
  }
  if (sceneId == SCENE_VAULT_CAVE_VAMBRACE) {
    return gVaultCaveVambrace_Triggers;
  }
  if (sceneId == SCENE_VAULT_CAVE_MOUTH) {
    return gVaultCaveMouth_Triggers;
  }
  return gVaultCave_FallbackTriggers;
}

/// Whether the west torch of the room of two torches stands on the fire
/// crest.
static s32 VaultCave_IsSouthernPillarOnSwitch(void) {
  struct Actor *pillar = Actor_Get(ACTOR_SOUTHERN_PILLAR);
  s32 row = pillar->z.fixed / toFIXED(16);
  s32 column = pillar->x.fixed / toFIXED(16);

  if (column == 15 && row == 54) {
    return 1;
  }
  return 0;
}

/// Once the torch is on the fire crest, plays the puzzle chime and opens the
/// wooden gate.
static inline void VaultCave_CompleteSouthernSwitch(void) {
  if (VaultCave_IsSouthernPillarOnSwitch() != 0) {
    GameFlag_Set(FLAG_VAULT_CAVE_SOUTH_PILLAR_ON_SWITCH);
    Sound_Play(MUS_PUZZLE_SOLVED);
    VaultCave_ApplySouthernSwitchCells();
  }
}

/// In the room of two burning torches, up the left-hand branch, Isaac pushes a
/// torch a cell. Once the west torch reaches the tile with the fire crest
/// (cell (15, 54)), the puzzle chime plays and the wooden gate north of it
/// opens.
static void VaultCave_PushSouthernPillar(void) {
  if (Event_GetFlag(FLAG_VAULT_CAVE_SOUTH_PILLAR_ON_SWITCH) == 0) {
    VaultCave_PushPillarAhead();
    VaultCave_CompleteSouthernSwitch();
  }
}

/// Draws the room of two torches' wooden gate open and lets the party through
/// it.
static void VaultCave_ApplySouthernSwitchCells(void) {
  MapGrid_CopyCells(87, 50, 2, 4, 80, 50);
  MapGrid_CopyCells(23, 52, 1, 2, 16, 52);
  MapGrid_CopyCellAttributes(16, 52, 1, 1, 16, 53);
}

/// In the room of two burning torches, Isaac uses Move on the west torch: a
/// great hand slides it a cell, and if that puts it on the fire crest the
/// puzzle chime plays and the wooden gate opens.
static void VaultCave_MoveSouthernPillar(void) {
  if (Event_GetFlag(FLAG_VAULT_CAVE_SOUTH_PILLAR_ON_SWITCH) == 0) {
    VaultCave_CompleteSouthernSwitch();
  }
}

static void UNUSED VaultCave_UnusedNoOp(void) {}

/// Draws the room of three torches' wooden gate open and lets the party
/// through it.
static void VaultCave_ApplyNorthernSwitchCells(void) {
  MapGrid_CopyCells(90, 9, 2, 3, 80, 9);
  MapGrid_CopyCells(27, 10, 1, 2, 17, 10);
  MapGrid_CopyCellAttributes(17, 10, 1, 1, 17, 11);
}

/// Whether the lower torch of the room of three torches stands on the fire
/// crest.
static s32 VaultCave_IsNorthernPillarOnSwitch(void) {
  struct Actor *pillar = Actor_Get(ACTOR_NORTHERN_PILLAR);
  s32 row = pillar->z.fixed / toFIXED(16);
  s32 column = pillar->x.fixed / toFIXED(16);

  if (column == 16 && row == 12) {
    return 1;
  }
  return 0;
}

/// Opens the gate only for a torch on the fire crest whose flame no drop has
/// put out.
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

/// In the room of three torches, on the way to the cave mouth, Isaac pushes a
/// torch a cell. If the lower torch reaches the fire crest below the wooden
/// gate (cell (16, 12)) still burning, the puzzle chime plays and the gate
/// opens; if a drop of water has put its flame out, nothing happens.
static void VaultCave_PushNorthernPillar(void) {
  VaultCave_PushPillarAhead();
  VaultCave_CompleteNorthernSwitch();
}

/// In the room of three torches, Isaac uses Move on the unlit torch just south
/// of the fire crest. The great hand slides it a cell like any torch, even onto
/// the crest, but an unlit torch opens nothing.
///
/// The event itself is empty: Move does the sliding.
static void VaultCave_MoveUnlitTorch(void) {}

/// In the room of three torches, Isaac uses Move on the lower burning torch: a
/// great hand slides it a cell, and if that puts it on the fire crest still
/// burning, the puzzle chime plays and the wooden gate opens.
static void VaultCave_MoveNorthernPillar(void) {
  VaultCave_CompleteNorthernSwitch();
}

/// In the room of six puddles, Isaac casts Frost on the north-west puddle
/// (cell (23, 32)), and it freezes into a tall ice pillar whose top Isaac can
/// leap across.
static void VaultCave_FreezeWestUpperPuddle(void) {
  struct Actor *target = Actor_Get(ACTOR_WEST_UPPER_PUDDLE);

  MapGrid_CopyCellAttributes(26, 30, 1, 1, 23, 32);

  if (target != NULL) {
    Actor_Get(ACTOR_WEST_UPPER_PUDDLE)->motionFlags = 0;
    target->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
  }

  GameFlag_Set(FLAG_VAULT_CAVE_WEST_UPPER_PUDDLE_FROZEN);
}

/// In the room of six puddles, Isaac casts Frost on the south-west puddle
/// (cell (23, 34)), and it freezes into a tall ice pillar whose top Isaac can
/// leap across.
static void VaultCave_FreezeWestLowerPuddle(void) {
  struct Actor *target = Actor_Get(ACTOR_WEST_LOWER_PUDDLE);

  MapGrid_CopyCellAttributes(26, 30, 1, 1, 23, 34);

  if (target != NULL) {
    Actor_Get(ACTOR_WEST_LOWER_PUDDLE)->motionFlags = 0;
    target->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
  }

  GameFlag_Set(FLAG_VAULT_CAVE_WEST_LOWER_PUDDLE_FROZEN);
}

/// In the room of six puddles, Isaac casts Frost on the puddle just east of
/// the south-west one (cell (24, 34)), and it freezes into a tall ice pillar
/// whose top Isaac can leap across.
static void VaultCave_FreezeInnerWestLowerPuddle(void) {
  struct Actor *target = Actor_Get(ACTOR_INNER_WEST_LOWER_PUDDLE);

  MapGrid_CopyCellAttributes(26, 30, 1, 1, 24, 34);

  if (target != NULL) {
    Actor_Get(ACTOR_INNER_WEST_LOWER_PUDDLE)->motionFlags = 0;
    target->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
  }

  GameFlag_Set(FLAG_VAULT_CAVE_INNER_WEST_LOWER_PUDDLE_FROZEN);
}

/// In the room of six puddles, Isaac casts Frost on the north-east puddle
/// (cell (26, 32)), and it freezes into a tall ice pillar whose top Isaac can
/// leap across.
static void VaultCave_FreezeEastUpperPuddle(void) {
  struct Actor *target = Actor_Get(ACTOR_EAST_UPPER_PUDDLE);

  MapGrid_CopyCellAttributes(26, 30, 1, 1, 26, 32);

  if (target != NULL) {
    Actor_SetSpriteFlags(target, 0);

    Actor_Get(ACTOR_EAST_UPPER_PUDDLE)->motionFlags = 0;
    target->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
  }

  GameFlag_Set(FLAG_VAULT_CAVE_EAST_UPPER_PUDDLE_FROZEN);
}

/// In the room of six puddles, Isaac casts Frost on the south-east puddle
/// (cell (26, 34)), and it freezes into a tall ice pillar whose top Isaac can
/// leap across.
static void VaultCave_FreezeEastLowerPuddle(void) {
  struct Actor *target = Actor_Get(ACTOR_EAST_LOWER_PUDDLE);

  MapGrid_CopyCellAttributes(26, 30, 1, 1, 26, 34);

  if (target != NULL) {
    Actor_SetSpriteFlags(target, 0);

    Actor_Get(ACTOR_EAST_LOWER_PUDDLE)->motionFlags = 0;
    target->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
  }

  GameFlag_Set(FLAG_VAULT_CAVE_EAST_LOWER_PUDDLE_FROZEN);
}

/// In the room of six puddles, Isaac casts Frost on the far east puddle (cell
/// (28, 33)), and it freezes into a tall ice pillar whose top Isaac can leap
/// across.
static void VaultCave_FreezeFarEastPuddle(void) {
  struct Actor *target = Actor_Get(ACTOR_FAR_EAST_PUDDLE);

  MapGrid_CopyCellAttributes(26, 30, 1, 1, 28, 33);

  if (target != NULL) {
    Actor_SetSpriteFlags(target, 0);

    Actor_Get(ACTOR_FAR_EAST_PUDDLE)->motionFlags = 0;
    target->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
  }

  GameFlag_Set(FLAG_VAULT_CAVE_FAR_EAST_PUDDLE_FROZEN);
}

/// Keeps the ice pillars drawn under Isaac while he is up on top of one.
static void VaultCave_UpdatePuddlePriorities(void) {
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);

  if (leader->y.fixed >= toFIXED(16)) {
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

/// Restores automatic draw priority for the same six puddles.
static void VaultCave_UseAutomaticFrostPriorities(void) {
  s32 targetIndex;
  s32 actorSlot;

  actorSlot = ACTOR_WEST_UPPER_PUDDLE;
  for (targetIndex = 0; targetIndex < 6; targetIndex++) {
    Actor_Get(actorSlot)->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
    actorSlot++;
  }
}

/// The cave rumbles and shakes for eight seconds while the pool's water
/// churns, then the water fades away and the drained pool is drawn.
static void VaultCave_RunRumble(void) {
  struct MapGridLayerScroll *layer =
      &((struct MapGridScrollState *)gMapGridState[0])->layers[2];
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
  // The water churns, its layer drifting by 0.04 to 0.07 of a pixel a frame.
  for (frame = 0; frame < 480; frame++) {
    u32 scrollDrift = Random_Below(2048);

    layer->phaseY = (double)layer->phaseY - (4718.592 - (double)scrollDrift);
    Event_Wait(1);
  }
  fadeLevel = 6;
  lowFadeLevel = 6;
  frame = 0;
  // The water fades off the floor in four steps, one every 20 frames. The
  // blend is packed as if it were a colour, the levels in RGB's three fields:
  // REG_BLDALPHA reads lowFadeLevel as the water's weight and the first level,
  // in the blue field, as the floor's, 24, above full; the green field falls
  // in unused bits.
  upperFadeBits = fadeLevel << BLUE_SHIFT;
  for (; frame <= 69; frame++) {
    IoWriteQueue_Store16((u32)&REG_BLDALPHA, upperFadeBits |
                                                 (fadeLevel << GREEN_SHIFT) |
                                                 lowFadeLevel);
    Event_Wait(1);
    if (frame % 20 == 0) {
      lowFadeLevel--;
      fadeLevel--;
    }
  }
  MapGrid_CopyCells(19, 83, 15, 8, 19, 91);
  Sound_Play(SE_STOP_ACTION_SE);
  MapGrid_CenterOnFollowedPoint();
  MapGrid_WaitForShakeToSettle();
  Event_End();
}

/// Isaac flips the switch on the ledge in the pool room (cell (28, 29)).
/// "Isaac flipped the switch." The cave rumbles and shakes, and the pool
/// drains away, and its water runs down into the northern room of
/// SceneId_VaultCaveVambrace below, whose floor Scene_Initialize draws dry
/// until then.
///
/// The switch can be flipped once each time the party comes down the ladder
/// from Vault.
static void VaultCave_FlipSwitch(void) {
  s32 destinationColumn;

  if (Event_GetFlag(FLAG_VAULT_CAVE_SWITCH_FLIPPED) == 0) {
    FieldMessage_Show(MSGNO(MsgFieldFlippedSwitch), 1);
    GameFlag_Set(FLAG_VAULT_CAVE_SWITCH_FLIPPED);
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

/// Whether a drop of water about to land is over the given actor's cell; if so,
/// the drop is sent back up to fall again.
static s32 VaultCave_ResetFallingActorAtActorCell(s32 subjectActorId) {
  struct Actor *subject = Actor_Get(subjectActorId);
  s32 fallingIndex;

  for (fallingIndex = 0; fallingIndex <= 3; fallingIndex++) {
    struct Actor *fallingActor =
        Actor_Get(fallingIndex + ACTOR_FIRST_TIMED_WATER_DROP);

    if (fallingActor->y.fixed > 0 && fallingActor->y.fixed < toFIXED(16)) {
      s32 fallingRow = CELL_OF(fallingActor->z.fixed);
      s32 fallingColumn = CELL_OF(fallingActor->x.fixed);
      s32 subjectRow = CELL_OF(subject->z.fixed);
      s32 subjectColumn = CELL_OF(subject->x.fixed);
      s32 zDelta = subjectRow - fallingRow;
      s32 xDelta = subjectColumn - fallingColumn;

      if (xDelta == 0 && zDelta == 0) {
        fallingActor->y.fixed = toFIXED(255);
        fallingActor->gravity = 0;
        fallingActor->velocityY = 0;
        return 1;
      }
    }
  }

  return 0;
}

/// In the room of three torches, lets the four drops of water fall on their
/// fixed beat, splashing where they land and putting out the lower torch's
/// flame if one lands on it.
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
      fallingActor->gravity = toFIXED(1) / 25;
      if (++gVaultCave_ScheduleIndex > 3) {
        gVaultCave_ScheduleIndex = 0;
      }
    }
    for (fallingIndex = 0; fallingIndex <= 3; fallingIndex++) {
      fallingActor = Actor_Get((fallingIndex + ACTOR_FIRST_TIMED_WATER_DROP));
      if (fallingActor->velocityY >= 0) {
        if (fallingActor->y.fixed < toFIXED(1)) {
          VaultCave_SpawnSplash(fallingActor);
          fallingActor->y.fixed = toFIXED(255);
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

/// In the room of six puddles, lets a drop of water chosen at random fall every
/// 64 frames, onto its puddle or onto the top of its ice pillar.
static void VaultCave_UpdateFrostControlledFallingActors(void) {
  s32 fallingIndex;
  struct Actor *fallingActor;
  s32 frozen;

  fallingActor = Actor_Get(ACTOR_FIRST_WATER_DROP);
  if (fallingActor->paused == 0) {
    if ((++gVaultCave_DropFrames & 63) == 0) {
      gVaultCave_SelectedDrop = Math_RemainderUnsigned(Random_Next(), 6);
      fallingActor =
          Actor_Get(gVaultCave_SelectedDrop + ACTOR_FIRST_WATER_DROP);
      fallingActor->gravity = toFIXED(1) / 25;
    }
    // A drop goes back up once it reaches its puddle, or the top of the ice
    // pillar 32 pixels up when the puddle is frozen.
    for (fallingIndex = 0; fallingIndex <= 5; fallingIndex++) {
      fallingActor = Actor_Get((fallingIndex + ACTOR_FIRST_WATER_DROP));
      frozen = GameFlag_Get(
          (fallingIndex + FLAG_VAULT_CAVE_WEST_UPPER_PUDDLE_FROZEN));
      if (frozen != 0) {
        if (fallingActor->velocityY <= 0) {
          if (fallingActor->y.fixed >= toFIXED(33)) {
            continue;
          }
        }
        fallingActor->y.fixed = toFIXED(255);
        fallingActor->gravity = 0;
        fallingActor->velocityY = 0;
        Sound_Play(SE_DRIP);
      } else {
        if (fallingActor->velocityY <= 0) {
          if (fallingActor->y.fixed >= toFIXED(1)) {
            continue;
          }
        }
        // Both stop: frozen is 0 here.
        fallingActor->gravity = frozen;
        fallingActor->velocityY = frozen;
        fallingActor->y.fixed = toFIXED(255);
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
  FieldActor_SetMoveTargetFX(actor, destination[0], destination[1],
                             destination[2]);
}

/// Splashes up to four little drops around a drop of water where it lands.
static void VaultCave_SpawnSplash(struct Actor *source) {
  struct Actor *drop;
  s32 dropIndex;

  for (dropIndex = 0; dropIndex < 4; dropIndex++) {
    drop = FieldActor_Create(SPRITE_DROPLET, source->x.fixed, source->y.fixed,
                             source->z.fixed);
    if (drop == NULL)
      break;
    drop->scaleY = 11 * FIXED_ONE / 20;
    drop->scaleX = 11 * FIXED_ONE / 20;
    drop->motionFlags = ACTOR_FALLS;
    drop->velocityY = -toFIXED(1);
    drop->speedLimit = Random_Next() + toFIXED(4) / 5;
    drop->collisionFlags = ACTOR_SOLID;
    VaultCave_OffsetActorRadially(drop, toFIXED(32), Random_Next());
    drop->wait = 8;
    Actor_StartScript(drop, gVaultCave_SplashScript);
  }
}

/// Readies the room of six puddles' drops of water and starts them falling.
static void VaultCave_StartFrostControlledFalls(void) {
  s32 actorSlot = ACTOR_FIRST_WATER_DROP;
  s32 i;

  for (i = 0; i < 6; i++) {
    struct Actor *fallingActor;

    Actor_SetSpriteFlags(Actor_Get(actorSlot), 0);
    fallingActor = Actor_Get(actorSlot);
    fallingActor->bounce = FIXED_ONE / 10;
    fallingActor->gravity = 0;
    fallingActor->y.fixed = toFIXED(255);
    actorSlot++;
  }

  Task_AddFrame(VaultCave_UpdateFrostControlledFallingActors, TASK_PASS_FRAME);
}

/// Readies the room of three torches' drops of water and starts them falling.
static void VaultCave_StartTimedFalls(void) {
  s32 actorSlot = ACTOR_FIRST_TIMED_WATER_DROP;
  s32 fallingIndex = 0;

  do {
    struct Actor *fallingActor;

    Actor_SetSpriteFlags(Actor_Get(actorSlot), 0);
    fallingActor = Actor_Get(actorSlot);
    fallingActor->bounce = FIXED_ONE / 10;
    fallingActor->gravity = 0;
    fallingActor->y.fixed = toFIXED(255);
    Actor_SetSpritePriority(fallingIndex + ACTOR_FIRST_TIMED_WATER_DROP, 1);
    fallingIndex++;
    actorSlot++;
  } while (fallingIndex <= 3);

  Task_AddFrame(VaultCave_UpdateTimedFallingActors, TASK_PASS_FRAME);
}

/// Arriving in any room of Vault Cave restores the room's puzzles and actors.
/// It then sets the Retreat destination, the foot of the ladder from Vault.
s32 Scene_Initialize(void) {
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  if (gMainRoutineState.next == SCENE_VAULT_CAVE_PASSAGE) {
    switch (gMainRoutineState.entrance) {
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
  if (gMainRoutineState.next == SCENE_VAULT_CAVE_VAMBRACE) {
    if (GameFlag_Get(FLAG_VAULT_CAVE_IVY_BLOWN_AWAY) == 0) {
      Actor_Get(ACTOR_IVY)->scaleY = 3 * FIXED_ONE / 2;
    }
    switch (gMainRoutineState.entrance) {
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
  if (gMainRoutineState.next == SCENE_VAULT_CAVE_MOUTH) {
    Actor_SetAnimation(ACTOR_PILLAR_EAST_OF_NORTHERN_SWITCH,
                       FLAME_PILLAR_ANIMATION_BURNING);
    if (GameFlag_Get(FLAG_VAULT_CAVE_NORTH_PILLAR_STRUCK) == 0) {
      Actor_SetAnimation(ACTOR_NORTHERN_PILLAR, FLAME_PILLAR_ANIMATION_BURNING);
    }
    Actor_SetSpriteFlags(Actor_Get(ACTOR_PILLAR_EAST_OF_NORTHERN_SWITCH), 0);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_NORTHERN_PILLAR), 0);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_PILLAR_SOUTH_OF_NORTHERN_SWITCH), 0);
    Actor_Get(ACTOR_NORTHERN_PILLAR)->collisionFlags |= ACTOR_BLOCKED_BY_SOLID;
    Actor_Get(ACTOR_PILLAR_SOUTH_OF_NORTHERN_SWITCH)->collisionFlags |=
        ACTOR_BLOCKED_BY_SOLID;
    switch (gMainRoutineState.entrance) {
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
  gMainRoutineState.retreatEntrance = VAULT_CAVE_PASSAGE_FROM_VAULT_REVISIT;
  gMainRoutineState.retreatScene = SCENE_VAULT_CAVE_PASSAGE;
  return 0;
}

/// The cave keeps its pushed-actor steps with its read-only data, though they
/// are not const: the pushes read the step again after each call.
#define PUSHED_ACTOR_TABLES_READ_ONLY
#include "shared/pushed_actor_direction_steps.c"
// clang-format off

const s32 gVaultCave_SplashScript[] = {
    SC_RELEASE,
};

#include "shared/fallback_entrances.c"

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
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR_SOUTH_OF_NORTHERN_SWITCH, CONDITION_ALWAYS, EVENT(VaultCave_MoveUnlitTorch) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_VAULT_CAVE_LOCKED_DOOR, CONDITION_ALWAYS, EVENT(VaultCave_ShowLockedDoor) },
    { TRIGGER_TABLE_END },
};

/// The frame of the 191-frame beat at which each timed drop falls.
u32 gVaultCave_ScheduleFrames[] = {
    20, 80, 115, 170,
};
// clang-format on

/// The frame of the drops' 191-frame beat, and the timed drop that falls next
/// when it reaches that drop's frame in gVaultCave_ScheduleFrames.
s32 gVaultCave_ScheduleTimer = 0;
s32 gVaultCave_ScheduleIndex = 0;

/// The frames counted for the random drops, one picked every 64 frames, and
/// the one picked, from the first of the six.
s32 gVaultCave_DropFrames = 0;
s32 gVaultCave_SelectedDrop = 0;

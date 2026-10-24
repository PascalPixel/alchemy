/// @file
/// Vault Cave's overlay, which holds three rooms, each a map of its own and all
/// named Vault Cave (MapId_VaultCavePassage, MapId_VaultCaveVambrace and
/// MapId_VaultCaveMouth, map section 31). The cave opens off Vault as the party
/// finds it on its return (MapId_VaultRevisit), whose warps
/// VAULT_REVISIT_TO_VAULT_CAVE_PASSAGE and VAULT_REVISIT_TO_VAULT_CAVE_MOUTH
/// lead in, through the hidden passage the dog shows and through a cave mouth
/// in the town's northeast cliff. No story flag is read here and no one speaks:
/// the cave is a side trip of puzzles for Move, Frost and Whirlwind, and only
/// "The door is tightly locked." and "Isaac flipped the switch." are shown.
///
/// - Vault Cave (MapId_VaultCavePassage), the way in from the hidden passage.
///   VAULT_CAVE_PASSAGE_FROM_VAULT_REVISIT is the foot of the ladder down from
///   that passage, where a region takes VAULT_CAVE_PASSAGE_TO_VAULT_REVISIT
///   back out to Vault (VAULT_REVISIT_FROM_VAULT_CAVE_PASSAGE); Retreat also
///   leads there (Map_Initialize). Searching the switch flips it
///   (EventScript_FlipSwitch, FLAG_VAULT_CAVE_SWITCH_FLIPPED): the switch
///   turns, the cave rumbles and shakes for eight seconds, and its map changes
///   as it fades (VaultCave_RunRumble). The change reaches
///   MapId_VaultCaveVambrace too: arriving in its northern room
///   (VAULT_CAVE_VAMBRACE_FROM_VAULT_CAVE_PASSAGE_NORTH,
///   VAULT_CAVE_VAMBRACE_FROM_VAULT_CAVE_MOUTH_NORTH,
///   VAULT_CAVE_VAMBRACE_NORTH_FROM_FROST_ROOM and
///   VAULT_CAVE_VAMBRACE_NORTH_FROM_WEST), Map_Initialize draws it closed until
///   the switch is flipped and opens its cells after. Arriving down the ladder
///   clears the flag again, so the switch is reset each time the party comes in
///   from Vault; arriving in the pool room
///   (VAULT_CAVE_PASSAGE_FROM_VAULT_CAVE_VAMBRACE_EAST,
///   VAULT_CAVE_PASSAGE_FROM_VAULT_CAVE_VAMBRACE_SOUTHEAST,
///   VAULT_CAVE_PASSAGE_WATER_ROOM_FROM_NORTH_WEST,
///   VAULT_CAVE_PASSAGE_WATER_ROOM_FROM_CHEST and
///   VAULT_CAVE_PASSAGE_WATER_ROOM_FROM_NORTH_EAST) redraws the flipped switch
///   and its changed cells. The room's chest is a mimic (FIND_MIMIC, slot 3 of
///   ENCOUNTER_ZONE_MIMIC).
/// - Vault Cave (MapId_VaultCaveVambrace). In its southern room a pillar
///   (LOCALID_SOUTHERN_PILLAR) goes onto its switch by pushing
///   (EventScript_PushSouthernPillar) or Move (EventScript_MoveSouthernPillar);
///   once it rests there the puzzle chime plays and the switch opens the way
///   (FLAG_VAULT_CAVE_SOUTH_PILLAR_ON_SWITCH). In the puddle room, six drops of
///   water (LOCALID_FIRST_WATER_DROP and the five after it) fall at random, one
///   every 64 frames, over six puddles (LOCALID_WEST_UPPER_PUDDLE to
///   LOCALID_FAR_EAST_PUDDLE) that Frost freezes. A frozen puddle changes its
///   map cell, and a drop falling onto it stops 32 pixels up instead of
///   reaching the floor (VaultCave_UpdateFrostControlledFallingObjects); while
///   the leader is 16 pixels or more above the floor, the puddles are drawn
///   beneath him. The six frozen states are
///   FLAG_VAULT_CAVE_WEST_UPPER_PUDDLE_FROZEN to
///   FLAG_VAULT_CAVE_FAR_EAST_PUDDLE_FROZEN, read by index, and the room's
///   entry code freezes again every puddle whose flag is set. Whirlwind blows
///   away ivy (LOCALID_IVY) for as long as the party stays in the cave
///   (FLAG_VAULT_CAVE_IVY_BLOWN_AWAY, a map section flag). A chest holds the
///   Vambrace.
/// - Vault Cave (MapId_VaultCaveMouth). In its northern room four water drops
///   (LOCALID_FIRST_TIMED_WATER_DROP and the three after it) fall on a fixed
///   schedule (gVaultCave_ScheduleFrames) while the party pushes or Moves a
///   pillar (LOCALID_NORTHERN_PILLAR) onto its switch
///   (FLAG_VAULT_CAVE_NORTH_PILLAR_ON_SWITCH). A water drop that lands on that
///   pillar marks it struck (FLAG_VAULT_CAVE_NORTH_PILLAR_STRUCK,
///   SE_DUST_PUFF), and a struck pillar no longer opens the switch until the
///   room is entered again. VAULT_CAVE_MOUTH_TO_VAULT_REVISIT leads out to
///   Vault (VAULT_REVISIT_FROM_VAULT_CAVE_MOUTH).
///
/// Each room has a door that only says it is locked
/// (EventScript_ShowLockedDoor). The pushes share VaultCave_PushPillarAhead,
/// which moves the pillar the leader faces one cell unless another object event
/// or the terrain is in the way, and the water drops run as map callbacks
/// started by Map_Initialize.
///
/// Open questions:
/// - None.

#include "save_block.h"
#include "unit/field/field.h"
#include "engine_call.h"
#include "map_ids.h"
#include "unit/import.h"
#include "unit/map/vault_cave.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/field_services.h"
#include "unit/runtime/runtime.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "heap_slots.h"
#include "types.h"
#include "text/msg_ids.h"
#include "script_layer.h"
#include "map_tables.h"
#include "task.h"
#include "call.h"
#include "object.h"
#include "ability_ids.h"
#include "flags.h"
#include "songs.h"
#include "objcmd.h"
#include "map_grid_scroll.h"
#include "map_header.h"
#include "macros.h"
#include "item_ids.h"

// Forward declarations.
static void VaultCave_ApplySouthernSwitchCells(void);
static void VaultCave_UseUnderfootFrostPriorities(void);
static void VaultCave_UseAutomaticFrostPriorities(void);
static void VaultCave_SpawnPuffs(struct ObjectEvent *source);

#include "io_write_queue.h"

#define TILE(position) ((position) / 0x100000)

/// The metatile behaviors of the cells the events answer.
enum VaultCaveMetatileBehavior {
  /// The switch MapId_VaultCavePassage flips.
  /// ★ Named from the game: "flipped the switch." (MsgFieldFlippedSwitch)
  MB_VAULT_CAVE_SWITCH = 20,
  /// The cells of MapId_VaultCaveMouth's northern pillar, pushed.
  MB_VAULT_CAVE_NORTHERN_PILLAR = 50,
  /// The cells of MapId_VaultCaveVambrace's southern pillar, pushed.
  MB_VAULT_CAVE_SOUTHERN_PILLAR = 60,
  /// ★ Named from the game: "The door is tightly locked."
  /// (MsgFieldDoorTightlyLocked)
  MB_VAULT_CAVE_LOCKED_DOOR = 99,
  /// ★ Named from the game: "checked the chest" (MsgVaultIsaacCheckedChest)
  MB_VAULT_CAVE_CHEST = 100
};

/// Map flags, and one map section flag. The six puddles of
/// MapId_VaultCaveVambrace keep their frozen state in six flags in a row,
/// read by index as well.
enum {
  /// The upper western puddle, the first of the six.
  /// ★ Named from what the player sees: six puddles of water in the floor of
  /// the puddle room, under the falling drops (its render)
  FLAG_VAULT_CAVE_WEST_UPPER_PUDDLE_FROZEN = FLAG_MAP_TEMP_00,
  /// ★ Named from what the player sees: the western puddle of the lower row
  /// (its render)
  FLAG_VAULT_CAVE_WEST_LOWER_PUDDLE_FROZEN = FLAG_MAP_TEMP_01,
  /// ★ Named from what the player sees: the lower row's puddle just east of
  /// the western one (its render)
  FLAG_VAULT_CAVE_INNER_WEST_LOWER_PUDDLE_FROZEN = FLAG_MAP_TEMP_02,
  /// ★ Named from what the player sees: the eastern puddle of the upper row
  /// (its render)
  FLAG_VAULT_CAVE_EAST_UPPER_PUDDLE_FROZEN = FLAG_MAP_TEMP_03,
  /// ★ Named from what the player sees: the eastern puddle of the lower row
  /// (its render)
  FLAG_VAULT_CAVE_EAST_LOWER_PUDDLE_FROZEN = FLAG_MAP_TEMP_04,
  /// ★ Named from what the player sees: the easternmost puddle, between the
  /// rows (its render)
  FLAG_VAULT_CAVE_FAR_EAST_PUDDLE_FROZEN = FLAG_MAP_TEMP_05,
  /// A water drop struck the northern pillar before it reached its switch.
  /// ★ Named from what the player sees: the pillar the leader pushes toward
  /// its switch in the northern room of MapId_VaultCaveMouth (its render)
  FLAG_VAULT_CAVE_NORTH_PILLAR_STRUCK = FLAG_MAP_TEMP_07,
  /// Whirlwind has blown the ivy away.
  /// ★ Named from the game: "Whirlwind" (MsgAbilityName)
  FLAG_VAULT_CAVE_IVY_BLOWN_AWAY = FLAG_MAPSEC_TEMP_00
};

/// The object events of MapId_VaultCaveVambrace, numbering the rows of
/// gVaultCaveVambrace_ObjectEventTemplates.
enum VaultCaveVambraceLocalId {
  /// A pillar, drawn as LOCALID_SOUTHERN_PILLAR is, two cells east of the
  /// southern switch, drawn underfoot when the party arrives by
  /// VAULT_CAVE_VAMBRACE_FROM_VAULT_CAVE_PASSAGE_SOUTH or
  /// VAULT_CAVE_VAMBRACE_SOUTH_FROM_FROST_ROOM.
  /// ★ Named from what the player sees: a pink stone pillar with a flame
  /// burning on top (its render)
  LOCALID_PILLAR_EAST_OF_SOUTHERN_SWITCH = LOCALID_FIRST_PLACED,
  /// Pushed or Moved onto the southern switch, map cell (15, 54).
  /// ★ Named from what the player sees: a pink stone pillar with a flame
  /// burning on top, beside the switch marked with a flame (its render)
  LOCALID_SOUTHERN_PILLAR,
  /// The first of six water drops (in the sprite VaultCave_SpawnPuffs also
  /// uses for the puffs where one lands) that fall at random, one every 64
  /// frames (VaultCave_UpdateFrostControlledFallingObjects).
  /// ★ Named from what the player sees: blue drops of water above the puddles
  /// (its render)
  LOCALID_FIRST_WATER_DROP,
  /// The six puddles under the water drops, in their order and in the order of
  /// their flags (FLAG_VAULT_CAVE_WEST_UPPER_PUDDLE_FROZEN to
  /// FLAG_VAULT_CAVE_FAR_EAST_PUDDLE_FROZEN). Drawn as the puddle that Frost
  /// freezes into an ice pillar in bilibin_cave.c; frozen, a puddle stops a
  /// water drop 32 pixels up.
  /// ★ Named from what the player sees: six blue puddles on the cave floor,
  /// each under a drop of water (its render)
  LOCALID_WEST_UPPER_PUDDLE = LOCALID_FIRST_WATER_DROP + 6,
  /// ★ Named from what the player sees: the western puddle of the lower row
  /// (its render)
  LOCALID_WEST_LOWER_PUDDLE,
  /// ★ Named from what the player sees: the lower row's puddle just east of it
  /// (its render)
  LOCALID_INNER_WEST_LOWER_PUDDLE,
  /// ★ Named from what the player sees: the eastern puddle of the upper row
  /// (its render)
  LOCALID_EAST_UPPER_PUDDLE,
  /// ★ Named from what the player sees: the eastern puddle of the lower row
  /// (its render)
  LOCALID_EAST_LOWER_PUDDLE,
  /// ★ Named from what the player sees: the easternmost puddle, between the
  /// rows (its render)
  LOCALID_FAR_EAST_PUDDLE,
  /// Ivy (SPRITE_IVY), as in bilibin.c, drawn half as tall again, placed until
  /// Whirlwind blows it away (FLAG_VAULT_CAVE_IVY_BLOWN_AWAY).
  /// ★ Named from what the player sees: leafy green ivy against the cave
  /// wall (its render)
  LOCALID_IVY
};

/// The object events of MapId_VaultCaveMouth, numbering the rows of
/// gVaultCaveMouth_ObjectEventTemplates. The northern switch is map cell (16,
/// 12).
enum VaultCaveMouthLocalId {
  /// A pillar two cells east of the northern switch.
  /// ★ Named from what the player sees: a pink stone pillar with a flame
  /// burning on top (its render)
  LOCALID_PILLAR_EAST_OF_NORTHERN_SWITCH = LOCALID_FIRST_PLACED,
  /// A pillar two cells south of the northern switch, which Move leaves where
  /// it is (EventScript_IgnoreMove).
  /// ★ Named from what the player sees: a pink stone pillar, its flame out
  /// (its render)
  LOCALID_PILLAR_SOUTH_OF_NORTHERN_SWITCH,
  /// Pushed or Moved onto the northern switch.
  /// ★ Named from what the player sees: a pink stone pillar with a flame
  /// burning on top (its render)
  LOCALID_NORTHERN_PILLAR,
  /// The first of four water drops, in the same sprite, that fall on a fixed
  /// schedule (VaultCave_UpdateTimedFallingObjects).
  /// ★ Named from what the player sees: blue drops of water on the cave floor
  /// (its render)
  LOCALID_FIRST_TIMED_WATER_DROP
};

const struct MapWarpDest *Map_GetWarpDests(void) {
  s16 mapId = gSaveBlock.mapId;

  if (mapId == (s32)&MapId_VaultCavePassage) {
    return gVaultCavePassage_WarpDests;
  }
  if (mapId == (s32)&MapId_VaultCaveVambrace) {
    return gVaultCaveVambrace_WarpDests;
  }
  if (mapId == (s32)&MapId_VaultCaveMouth) {
    return gVaultCaveMouth_WarpDests;
  }
  return gVaultCave_FallbackWarpDests;
}

/// Returns the region boxes used by the cave's warps.
const struct MapWarpRegion *Map_GetWarpRegions(void) {
  return gVaultCave_Regions;
}

const u32 *Map_GetWarps(void) {
  s16 mapId = gSaveBlock.mapId;

  if (mapId == (s32)&MapId_VaultCavePassage) {
    return gVaultCavePassage_Warps;
  }
  if (mapId == (s32)&MapId_VaultCaveVambrace) {
    return gVaultCaveVambrace_Warps;
  }
  if (mapId == (s32)&MapId_VaultCaveMouth) {
    return gVaultCaveMouth_Warps;
  }
  return gVaultCave_FallbackWarps;
}

/// Selects the pillars, water drops, puddles and ivy of the room.
const struct ObjectEventTemplate *Map_GetObjectEventTemplates(void) {
  s16 mapId = gSaveBlock.mapId;

  if (mapId == (s32)&MapId_VaultCavePassage) {
    return gVaultCavePassage_ObjectEventTemplates;
  }
  if (mapId == (s32)&MapId_VaultCaveVambrace) {
    return gVaultCaveVambrace_ObjectEventTemplates;
  }
  if (mapId == (s32)&MapId_VaultCaveMouth) {
    return gVaultCaveMouth_ObjectEventTemplates;
  }
  return gVaultCave_FallbackObjectEventTemplates;
}

/// Finds a placed object event with collision enabled at the requested map
/// cell.
static struct ObjectEvent *VaultCave_FindCollidableObjectEventAtCell(s32 column,
                                                                     s32 row) {
  // UB: map creation leaves unused object event slots NULL. The plain safe
  // lookup adds four native bytes in EN; retain the original reads for
  // matching.
#ifdef UBFIX
  struct FieldWork *work = gFieldWork;
  u32 objectEventSlot;

  for (objectEventSlot = LOCALID_FIRST_PLACED; objectEventSlot <= 65;
       objectEventSlot++) {
    struct ObjectEvent *objectEvent = work->objectEvents[objectEventSlot];

    if (objectEvent != NULL && column == (objectEvent->x.fixed >> 20) &&
        row == (objectEvent->z.fixed >> 20) &&
        objectEvent->collisionFlags != 0) {
      return objectEvent;
    }
  }
#else
  s32 **slots = (s32 **)((u8 *)gFieldWork + 0x14);
  u32 objectEventSlot;

  for (objectEventSlot = 8; objectEventSlot <= 65; objectEventSlot++) {
    s32 *objectEventWords = slots[objectEventSlot];

    if (column == (objectEventWords[2] >> 20) &&
        row == (objectEventWords[4] >> 20) &&
        *((u8 *)objectEventWords + 0x59) != 0) {
      return (struct ObjectEvent *)objectEventWords;
    }
  }
#endif
  return NULL;
}

#ifdef UBFIX
/// The table stores Z then X as halfwords. Build its packed word without
/// reading a halfword array through an incompatible, more-aligned struct.
#define DIRECTION_STEP(heading)                                                \
  ((s32)(((u32)(u16)gVaultCave_DirectionSteps[(heading) * 2 + 1] << 16) |      \
         (u16)gVaultCave_DirectionSteps[(heading) * 2]))
#else
/// The step table read as one word per heading: the x step in pixels in the
/// high halfword, the z step in the low. The struct's array reference keeps
/// the native loads; an s32 pointer to the table does not.
struct DirectionSteps {
  s32 xz[16];
};

#define DIRECTION_STEP(heading)                                                \
  (((struct DirectionSteps *)gVaultCave_DirectionSteps)->xz[heading])
#endif

/// Pushes the faced pillar one cell unless another object event or terrain
/// blocks it.
static void VaultCave_PushPillarAhead(void) {
  struct ObjectEvent *leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  s32 direction = leader->facing >> 12;
  struct ObjectEvent *pillar;
  struct ObjectEvent *beyond;
  struct FieldPosition destination;
  s32 fallingIndex;
  s32 column;
  s32 row;

  column = (leader->x.part.pixel + (DIRECTION_STEP(direction) >> 16)) >> 4;
  row = (leader->z.part.pixel + (s16)DIRECTION_STEP(direction)) >> 4;
  pillar = VaultCave_FindCollidableObjectEventAtCell(column, row);
  // UB: the lookup may return NULL. The plain safe guard moves the null
  // branch ahead of the byte load in EN, so matching keeps its original order.
#ifdef UBFIX
  if (pillar == NULL || pillar->collisionFlags == 0) {
#else
  if (pillar->collisionFlags == 0 || pillar == NULL) {
#endif
    return;
  }
  // MapId_VaultCaveMouth's water drops are never pushed; in
  // MapId_VaultCaveVambrace these slots hold water drops too.
  for (fallingIndex = 0; fallingIndex <= 3; fallingIndex++) {
    if (pillar ==
        ObjectEvent_GetById(fallingIndex + LOCALID_FIRST_TIMED_WATER_DROP)) {
      return;
    }
  }
  column = (pillar->x.part.pixel + (DIRECTION_STEP(direction) >> 16)) >> 4;
  row = (pillar->z.part.pixel + (s16)DIRECTION_STEP(direction)) >> 4;
  beyond = VaultCave_FindCollidableObjectEventAtCell(column, row);
  if (beyond != 0 && (beyond->collisionFlags & 1)) {
    return;
  }
  pillar->layer = 2;
  destination.x = pillar->x.fixed + (DIRECTION_STEP(direction) & 0xffff0000);
  destination.y = pillar->y.fixed;
  destination.z = pillar->z.fixed + (s32)((u32)DIRECTION_STEP(direction) << 16);
  if (FieldObject_CheckMovementCollision(pillar, &destination) > 0) {
    return;
  }
  Object_SelectAnimation(leader, ANIM_PUSH);
  WaitFrames(15);
  Sound_PlaySong(SE_PUSH_STEP);
  pillar->speed = 0x3333;
  pillar->acceleration = 0x3333;
  Object_SetPosition(pillar, destination.x, destination.y, destination.z);
  leader->speed = 0x3333;
  leader->acceleration = 0x3333;
  Object_SetPosition(leader, destination.x, destination.y, destination.z);
  Object_WaitForMove(pillar);
  Sound_PlaySong(SE_STOP_ACTION_SE);
  pillar->x.fixed = destination.x;
  pillar->z.fixed = destination.z;
  pillar->velocityX = 0;
  pillar->velocityZ = 0;
  leader->velocityX = 0;
  leader->velocityZ = 0;
  leader->targetX = OBJ_EVENT_NO_TARGET;
  leader->targetZ = OBJ_EVENT_NO_TARGET;
  Object_SelectAnimation(leader, ANIM_STAND);
}

/// Whirlwind blows the ivy away. It is cast on LOCALID_IVY in the puddle room
/// of MapId_VaultCaveVambrace, and the ivy stays gone for as long as the party
/// stays in the cave (FLAG_VAULT_CAVE_IVY_BLOWN_AWAY is a map section flag).
static void EventScript_UseWhirlwind(void) {
  GameFlag_Set(FLAG_VAULT_CAVE_IVY_BLOWN_AWAY);
}

/// In any room of Vault Cave, searching the locked door only says so.
/// "The door is tightly locked."
static void EventScript_ShowLockedDoor(void) {
  FieldMessage_Show(MSGNO(MsgFieldDoorTightlyLocked), 1);
}

/// Selects the cave map's bg events, movement checks and Psynergy callbacks.
const struct MapEvent *Map_GetEvents(void) {
  s16 mapId = gSaveBlock.mapId;

  if (mapId == (s32)&MapId_VaultCavePassage) {
    return gVaultCavePassage_Events;
  }
  if (mapId == (s32)&MapId_VaultCaveVambrace) {
    return gVaultCaveVambrace_Events;
  }
  if (mapId == (s32)&MapId_VaultCaveMouth) {
    return gVaultCaveMouth_Events;
  }
  return gVaultCave_FallbackEvents;
}

/// Tests the southern pillar's cell, rounding fixed coordinates toward zero.
static s32 VaultCave_IsSouthernPillarOnSwitch(void) {
  struct ObjectEvent *pillar = ObjectEvent_GetById(LOCALID_SOUTHERN_PILLAR);
  s32 fixedZ = pillar->z.fixed;
  s32 fixedX;
  s32 row;
  s32 column;

  if (fixedZ < 0) {
    fixedZ += 0x000FFFFF;
  }
  fixedX = pillar->x.fixed;
  row = fixedZ >> 20;
  if (fixedX < 0) {
    fixedX += 0x000FFFFF;
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
    Script_PlaySong(MUS_PUZZLE_SOLVED);
    VaultCave_ApplySouthernSwitchCells();
  }
}

/// In VaultCaveVambrace's southern room, pushing the southern pillar; once it
/// rests on its switch, the puzzle chime plays and the switch opens the way.
static void EventScript_PushSouthernPillar(void) {
  if (Script_GetFlag(FLAG_VAULT_CAVE_SOUTH_PILLAR_ON_SWITCH) == 0) {
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
static void EventScript_MoveSouthernPillar(void) {
  if (Script_GetFlag(FLAG_VAULT_CAVE_SOUTH_PILLAR_ON_SWITCH) == 0) {
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
  struct ObjectEvent *pillar = ObjectEvent_GetById(LOCALID_NORTHERN_PILLAR);
  s32 fixedZ = pillar->z.fixed;
  s32 fixedX;
  s32 row;
  s32 column;

  if (fixedZ < 0) {
    fixedZ += 0x000FFFFF;
  }
  fixedX = pillar->x.fixed;
  row = fixedZ >> 20;
  if (fixedX < 0) {
    fixedX += 0x000FFFFF;
  }
  column = fixedX >> 20;
  if (column == 16 && row == 12) {
    return 1;
  }
  return 0;
}

/// The northern switch also waits until the falling-object guard is clear.
static inline void VaultCave_CompleteNorthernSwitch(void) {
  if (GameFlag_Get(FLAG_VAULT_CAVE_NORTH_PILLAR_ON_SWITCH) == 0) {
    if (VaultCave_IsNorthernPillarOnSwitch() != 0) {
      if (GameFlag_Get(FLAG_VAULT_CAVE_NORTH_PILLAR_STRUCK) == 0) {
        Script_PlaySong(MUS_PUZZLE_SOLVED);
        VaultCave_ApplyNorthernSwitchCells();
        GameFlag_Set(FLAG_VAULT_CAVE_NORTH_PILLAR_ON_SWITCH);
      }
    }
  }
}

/// In VaultCaveMouth's northern room, pushing the northern pillar; it opens its
/// switch only if no water drop has struck it.
static void EventScript_PushNorthernPillar(void) {
  VaultCave_PushPillarAhead();
  VaultCave_CompleteNorthernSwitch();
}

/// In VaultCaveMouth's northern room, Move on
/// LOCALID_PILLAR_SOUTH_OF_NORTHERN_SWITCH does nothing.
static void EventScript_IgnoreMove(void) {}

/// In VaultCaveMouth's northern room, Move on the northern pillar, with the
/// same switch check.
static void EventScript_MoveNorthernPillar(void) {
  VaultCave_CompleteNorthernSwitch();
}

/// Frost freezes the upper western puddle.
/// The puddle is in VaultCaveVambrace's puddle room; freezing it changes its
/// map cell, and arriving again re-freezes every puddle whose flag is set.
static void EventScript_FreezeWestUpperPuddle(void) {
  // An ordinary shared Frost helper keeps the extents but saves r5-r7 instead
  // of r5 alone and changes the map-copy argument registers in native English.
  struct ObjectEvent *target = ObjectEvent_GetById(LOCALID_WEST_UPPER_PUDDLE);

  MapGrid_CopyCellAttributes(26, 30, 1, 1, 23, 32);

  if (target != 0) {

    ObjectEvent_GetById(LOCALID_WEST_UPPER_PUDDLE)->motionFlags = 0;
    target->priorityFlags = OBJ_EVENT_PRIORITY_AUTOMATIC;
  }

  GameFlag_Set(FLAG_VAULT_CAVE_WEST_UPPER_PUDDLE_FROZEN);
}

/// In VaultCaveVambrace's puddle room, Frost on the lower western puddle, as on
/// the upper one.
static void EventScript_FreezeWestLowerPuddle(void) {
  struct ObjectEvent *target = ObjectEvent_GetById(LOCALID_WEST_LOWER_PUDDLE);

  MapGrid_CopyCellAttributes(26, 30, 1, 1, 23, 34);

  if (target != 0) {

    ObjectEvent_GetById(LOCALID_WEST_LOWER_PUDDLE)->motionFlags = 0;
    target->priorityFlags = OBJ_EVENT_PRIORITY_AUTOMATIC;
  }

  GameFlag_Set(FLAG_VAULT_CAVE_WEST_LOWER_PUDDLE_FROZEN);
}

/// In VaultCaveVambrace's puddle room, Frost on the inner lower western puddle.
static void EventScript_FreezeInnerWestLowerPuddle(void) {
  struct ObjectEvent *target =
      ObjectEvent_GetById(LOCALID_INNER_WEST_LOWER_PUDDLE);

  MapGrid_CopyCellAttributes(26, 30, 1, 1, 24, 34);

  if (target != 0) {

    ObjectEvent_GetById(LOCALID_INNER_WEST_LOWER_PUDDLE)->motionFlags = 0;
    target->priorityFlags = OBJ_EVENT_PRIORITY_AUTOMATIC;
  }

  GameFlag_Set(FLAG_VAULT_CAVE_INNER_WEST_LOWER_PUDDLE_FROZEN);
}

/// In VaultCaveVambrace's puddle room, Frost on the upper eastern puddle.
static void EventScript_FreezeEastUpperPuddle(void) {
  struct ObjectEvent *target = ObjectEvent_GetById(LOCALID_EAST_UPPER_PUDDLE);

  MapGrid_CopyCellAttributes(26, 30, 1, 1, 26, 32);

  if (target != 0) {
    ObjectEvent_SetSpriteFlags(target, 0);

    ObjectEvent_GetById(LOCALID_EAST_UPPER_PUDDLE)->motionFlags = 0;
    target->priorityFlags = OBJ_EVENT_PRIORITY_AUTOMATIC;
  }

  GameFlag_Set(FLAG_VAULT_CAVE_EAST_UPPER_PUDDLE_FROZEN);
}

/// In VaultCaveVambrace's puddle room, Frost on the lower eastern puddle.
static void EventScript_FreezeEastLowerPuddle(void) {
  struct ObjectEvent *target = ObjectEvent_GetById(LOCALID_EAST_LOWER_PUDDLE);

  MapGrid_CopyCellAttributes(26, 30, 1, 1, 26, 34);

  if (target != 0) {
    ObjectEvent_SetSpriteFlags(target, 0);

    ObjectEvent_GetById(LOCALID_EAST_LOWER_PUDDLE)->motionFlags = 0;
    target->priorityFlags = OBJ_EVENT_PRIORITY_AUTOMATIC;
  }

  GameFlag_Set(FLAG_VAULT_CAVE_EAST_LOWER_PUDDLE_FROZEN);
}

/// In VaultCaveVambrace's puddle room, Frost on the far eastern puddle.
static void EventScript_FreezeFarEastPuddle(void) {
  struct ObjectEvent *target = ObjectEvent_GetById(LOCALID_FAR_EAST_PUDDLE);

  MapGrid_CopyCellAttributes(26, 30, 1, 1, 28, 33);

  if (target != 0) {
    ObjectEvent_SetSpriteFlags(target, 0);

    ObjectEvent_GetById(LOCALID_FAR_EAST_PUDDLE)->motionFlags = 0;
    target->priorityFlags = OBJ_EVENT_PRIORITY_AUTOMATIC;
  }

  GameFlag_Set(FLAG_VAULT_CAVE_FAR_EAST_PUDDLE_FROZEN);
}

/// Changes the puddles' draw priorities when the leader is above them.
static void VaultCave_UpdatePuddlePriorities(void) {
  struct ObjectEvent *leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);

  if (leader->y.fixed >= PIXELS(16)) {
    VaultCave_UseUnderfootFrostPriorities();
  } else {
    VaultCave_UseAutomaticFrostPriorities();
  }
}

/// Keeps all six puddles beneath the leader's feet.
static void VaultCave_UseUnderfootFrostPriorities(void) {
  s32 targetIndex;
  s32 objectEventSlot;

  objectEventSlot = LOCALID_WEST_UPPER_PUDDLE;
  for (targetIndex = 0; targetIndex < 6; targetIndex++) {
    ObjectEvent_GetById(objectEventSlot)->priorityFlags |=
        OBJ_EVENT_PRIORITY_UNDERFOOT;
    objectEventSlot++;
  }
}

/// Restores automatic draw priority for the same six puddles.
static void VaultCave_UseAutomaticFrostPriorities(void) {
  s32 objectEventSlot = LOCALID_WEST_UPPER_PUDDLE;
  s32 priority = OBJ_EVENT_PRIORITY_AUTOMATIC;
  s32 remaining = 5;

  do {
    struct ObjectEvent *target = ObjectEvent_GetById(objectEventSlot);

    remaining--;
    target->priorityFlags = priority;
    objectEventSlot++;
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

  FieldScript_Start();
  if (gFrameCount & 1) {
    layer->speedX = 1;
    layer->speedY = 1;
  } else {
    layer->speedX = -1;
    layer->speedY = -1;
  }
  MapGrid_SetShake(0x30000, 0x30000, 0x10000);
  MapGrid_SetShake(-1, -1, 0xe666);
  Script_PlaySong(SE_RUSH);
  // Scroll and shake for 480 frames before the final fade.
  for (frame = 479; frame >= 0; frame--) {
    u32 scrollDrift = (u32)(Random_Next() << 11) >> 16;

    layer->phaseY = (double)layer->phaseY - (4718.592 - (double)scrollDrift);
    FieldScript_WaitFrames(1);
  }
  // Keep the packed native value written to BLDALPHA.
  fadeLevel = 6;
  lowFadeLevel = 6;
  frame = 0;
  upperFadeBits = fadeLevel << 10;
  do {
    IoWriteQueue_Store16(0x4000052,
                         upperFadeBits | (fadeLevel << 5) | lowFadeLevel);
    FieldScript_WaitFrames(1);
    if (frame % 20 == 0) {
      lowFadeLevel--;
      fadeLevel--;
    }
    frame++;
  } while (frame <= 69);
  // Replace the mechanism's map cells after the fade completes.
  MapGrid_CopyCells(19, 83, 15, 8, 19, 91);
  Script_PlaySong(SE_STOP_ACTION_SE);
  MapGrid_CenterOnFollowedPoint();
  MapGrid_WaitForShakeToSettle();
  FieldScript_End();
}

/// In VaultCavePassage's pool room, searching the switch flips it.
/// "Isaac flipped the switch." The first time since the party came in from
/// Vault, the switch turns, the cave rumbles and shakes for eight seconds and
/// the map changes as it fades; FLAG_VAULT_CAVE_SWITCH_FLIPPED then keeps the
/// change until the party comes down the ladder from Vault again.
static void EventScript_FlipSwitch(void) {
  s32 destinationRow;
  s32 destinationColumn;

  if (Script_GetFlag(FLAG_VAULT_CAVE_SWITCH_FLIPPED) == 0) {
    FieldMessage_Show(MSGNO(MsgFieldFlippedSwitch), 1);
    GameFlag_Set(FLAG_VAULT_CAVE_SWITCH_FLIPPED);
    // Animate the switch face before the adjacent mechanism starts.
    destinationRow = 27;
    destinationColumn = 92;
    Script_PlaySong(SE_SWITCH);
    MapGrid_CopyCells(107, 27, 1, 1, destinationColumn, destinationRow);
    FieldScript_WaitFrames(39);
    MapGrid_CopyCells(108, 27, 1, 1, destinationColumn, destinationRow);
    FieldScript_WaitFrames(50);
    destinationColumn = 25;
    Script_PlaySong(SE_SWITCH_MECHANISM);
    MapGrid_CopyCells(1, 24, 1, 2, destinationColumn, destinationRow);
    FieldScript_WaitFrames(40);
    MapGrid_CopyCells(2, 24, 1, 2, destinationColumn, destinationRow);
    FieldScript_WaitFrames(40);
    VaultCave_RunRumble();
  }
}

/// Resets a low water drop sharing the subject's cell and reports a hit.
static s32 VaultCave_ResetFallingObjectAtObjectEventCell(s32 subjectLocalId) {
  struct ObjectEvent *subject = ObjectEvent_GetById(subjectLocalId);
  s32 fallingIndex = 0;

  do {
    struct ObjectEvent *fallingObjectEvent =
        ObjectEvent_GetById(fallingIndex + LOCALID_FIRST_TIMED_WATER_DROP);

    if ((u32)(fallingObjectEvent->y.fixed - 1) <= 0x000ffffe) {
      s32 fallingRow = TILE(fallingObjectEvent->z.fixed);
      s32 fallingColumn = TILE(fallingObjectEvent->x.fixed);
      s32 subjectRow = TILE(subject->z.fixed);
      s32 subjectColumn = TILE(subject->x.fixed);
      s32 zDelta = subjectRow - fallingRow;
      s32 xDelta = subjectColumn - fallingColumn;

      if (xDelta == 0 && zDelta == 0) {
        fallingObjectEvent->y.fixed = PIXELS(255);
        fallingObjectEvent->gravity = 0;
        fallingObjectEvent->velocityY = 0;
        return 1;
      }
    }

    fallingIndex++;
  } while (fallingIndex <= 3);

  return 0;
}

/// Drops four objects on a repeating schedule and handles their landings.
static void VaultCave_UpdateTimedFallingObjects(void) {
  s32 fallingIndex;
  struct ObjectEvent *fallingObjectEvent;
  s32 paused;
  s32 scheduleIndex;

  paused = ObjectEvent_GetById(LOCALID_NORTHERN_PILLAR)->paused;
  if (paused == 0) {
    if (++gVaultCave_ScheduleTimer > 190) {
      gVaultCave_ScheduleTimer = 0;
    }
    scheduleIndex = gVaultCave_ScheduleIndex;
    if (gVaultCave_ScheduleFrames[scheduleIndex] == gVaultCave_ScheduleTimer) {
      fallingObjectEvent =
          ObjectEvent_Get((scheduleIndex + LOCALID_FIRST_TIMED_WATER_DROP));
      fallingObjectEvent->gravity = 0xa3d;
      if (++gVaultCave_ScheduleIndex > 3) {
        gVaultCave_ScheduleIndex = 0;
      }
    }
    // Reset objects that have reached the floor and spawn their landing puffs.
    for (fallingIndex = 0; fallingIndex <= 3; fallingIndex++) {
      fallingObjectEvent =
          ObjectEvent_Get((fallingIndex + LOCALID_FIRST_TIMED_WATER_DROP));
      if (fallingObjectEvent->velocityY >= 0) {
        if (fallingObjectEvent->y.fixed <= 0xffff) {
          VaultCave_SpawnPuffs(fallingObjectEvent);
          fallingObjectEvent->y.fixed = PIXELS(255);
          fallingObjectEvent->gravity = 0;
          fallingObjectEvent->velocityY = 0;
          fallingObjectEvent->paused = 0;
          Script_PlaySong(SE_DRIP);
        }
      }
    }
    // A water drop landing on either movable pillar restarts at the top.
    if (VaultCave_ResetFallingObjectAtObjectEventCell(
            LOCALID_NORTHERN_PILLAR) != 0) {
      Import_ObjectEventSetAnimation(LOCALID_NORTHERN_PILLAR, 1);
      if (Script_GetFlag(FLAG_VAULT_CAVE_NORTH_PILLAR_STRUCK) == 0) {
        GameFlag_Set(FLAG_VAULT_CAVE_NORTH_PILLAR_STRUCK);
        Script_PlaySong(SE_DUST_PUFF);
      } else {
        Script_PlaySong(SE_DRIP);
      }
    }
    if (VaultCave_ResetFallingObjectAtObjectEventCell(
            LOCALID_PILLAR_SOUTH_OF_NORTHERN_SWITCH) != 0) {
      Script_PlaySong(SE_DRIP);
    }
  }
}

/// Drops a random object every 64 frames, with reset height set by its puddle
/// (frozen or not).
static void VaultCave_UpdateFrostControlledFallingObjects(void) {

  s32 fallingIndex;
  struct ObjectEvent *fallingObjectEvent;
  s32 stateValue;
  s32 *selectedIndex;

  fallingObjectEvent = ObjectEvent_GetById(LOCALID_FIRST_WATER_DROP);
  if (fallingObjectEvent->paused == 0) {
    // Preserve one random selection per 64 frames.
    if ((++gVaultCave_TickCounter & 63) == 0) {
      selectedIndex = &gVaultCave_TickValue;
      stateValue = Import_RandomNext();
      stateValue = Math_RemainderUnsigned(stateValue, 6);
      *selectedIndex = stateValue;
      fallingObjectEvent =
          ObjectEvent_GetById((stateValue + LOCALID_FIRST_WATER_DROP));
      fallingObjectEvent->gravity = 0xa3d;
    }
    // Each object's paired Frost flag selects its reset height.
    for (fallingIndex = 0; fallingIndex <= 5; fallingIndex++) {
      fallingObjectEvent =
          ObjectEvent_GetById((fallingIndex + LOCALID_FIRST_WATER_DROP));
      stateValue = GameFlag_Get(
          (fallingIndex + FLAG_VAULT_CAVE_WEST_UPPER_PUDDLE_FROZEN));
      if (stateValue != 0) {
        if (fallingObjectEvent->velocityY <= 0) {
          if (fallingObjectEvent->y.fixed > 0x20ffff) {
            continue;
          }
        }
        fallingObjectEvent->y.fixed = PIXELS(255);
        fallingObjectEvent->gravity = 0;
        fallingObjectEvent->velocityY = 0;
        Sound_PlaySong(SE_DRIP);
      } else {
        if (fallingObjectEvent->velocityY <= 0) {
          if (fallingObjectEvent->y.fixed > 0xffff) {
            continue;
          }
        }
        fallingObjectEvent->gravity = stateValue;
        fallingObjectEvent->velocityY = stateValue;
        fallingObjectEvent->y.fixed = PIXELS(255);
        Sound_PlaySong(SE_DRIP);
      }
    }
  }
}

/// Offsets an object event's current position by a polar displacement.
static void VaultCave_OffsetObjectEventRadially(struct ObjectEvent *objectEvent,
                                                s32 radius, s32 angle) {
  s32 destination[3];

  if (objectEvent == NULL) {
    return;
  }
  SET_VECTOR(destination, objectEvent->x.fixed, objectEvent->y.fixed,
             objectEvent->z.fixed);
  Vector_AddPolarOffset(radius, angle, destination);
  Object_SetPosition(objectEvent, destination[0], destination[1],
                     destination[2]);
}

/// Spawns up to four short-lived puffs around a landing object.
static void VaultCave_SpawnPuffs(struct ObjectEvent *source) {
  struct ObjectEvent *puff;
  s32 puffIndex;

  for (puffIndex = 0; puffIndex < 4; puffIndex++) {
    puff = ObjectEvent_Create(240, source->x.fixed, source->y.fixed,
                              source->z.fixed);
    if (puff == 0)
      break;
    puff->scaleY = 0x8ccc;
    puff->scaleX = 0x8ccc;
    puff->motionFlags = OBJ_EVENT_FALLS;
    puff->velocityY = -0x10000;
    puff->speed = Import_RandomNext() + 0xcccc;
    puff->collisionFlags = 1;
    VaultCave_OffsetObjectEventRadially(puff, 0x200000, Import_RandomNext());
    {
      u16 *timer = &puff->wait;
      s32 frames = 8;

      *timer = frames;
    }
    ObjectEvent_SetScript(puff, gVaultCave_PuffScript);
  }
}

/// Initializes six water drops and starts their Frost-controlled updater.
static void VaultCave_StartFrostControlledFalls(void) {
  s32 objectEventSlot = LOCALID_FIRST_WATER_DROP;
  s32 remaining = 5;

  do {
    struct ObjectEvent *fallingObjectEvent;

    ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(objectEventSlot), 0);
    fallingObjectEvent = ObjectEvent_GetById(objectEventSlot);
    fallingObjectEvent->bounce = 0x1999;
    fallingObjectEvent->gravity = 0;
    remaining--;
    fallingObjectEvent->y.fixed = PIXELS(255);
    objectEventSlot++;
  } while (remaining >= 0);

  FieldScheduler_AddCallback(VaultCave_UpdateFrostControlledFallingObjects,
                             TASK_PRIORITY_MAP);
}

/// Initializes four water drops and starts their timed updater.
static void VaultCave_StartTimedFalls(void) {
  s32 objectEventSlot = LOCALID_FIRST_TIMED_WATER_DROP;
  s32 fallingIndex = 0;

  do {
    struct ObjectEvent *fallingObjectEvent;

    ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(objectEventSlot), 0);
    fallingObjectEvent = ObjectEvent_GetById(objectEventSlot);
    fallingObjectEvent->bounce = 0x1999;
    fallingObjectEvent->gravity = 0;
    fallingObjectEvent->y.fixed = PIXELS(255);
    ObjectMotion_SetActionVariant(fallingIndex + LOCALID_FIRST_TIMED_WATER_DROP,
                                  1);
    fallingIndex++;
    objectEventSlot++;
  } while (fallingIndex <= 3);

  FieldScheduler_AddCallback(VaultCave_UpdateTimedFallingObjects,
                             TASK_PRIORITY_MAP);
}

/// Arriving in any room of Vault Cave: restores the room's puzzles and object
/// events, then sets the Retreat destination, the foot of the ladder from
/// Vault.
s32 Map_Initialize(void) {
  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  if (gSaveBlock.mapId == (s32)&MapId_VaultCavePassage) {
    switch (gSaveBlock.warpDest) {
    case VAULT_CAVE_PASSAGE_WATER_ROOM_FROM_NORTH_WEST:
    case VAULT_CAVE_PASSAGE_FROM_VAULT_CAVE_VAMBRACE_EAST:
    case VAULT_CAVE_PASSAGE_FROM_VAULT_CAVE_VAMBRACE_SOUTHEAST:
    case VAULT_CAVE_PASSAGE_WATER_ROOM_FROM_CHEST:
    case VAULT_CAVE_PASSAGE_WATER_ROOM_FROM_NORTH_EAST:
      if (GameFlag_Get(FLAG_VAULT_CAVE_SWITCH_FLIPPED) == 0) {
        MapGrid_CopyCellAttributes(22, 29, 1, 1, 21, 29);
      } else {
        MapGrid_CopyCells(108, 27, 1, 1, 92, 27);
        WaitFrames(1);
        MapGrid_CopyCells(19, 83, 15, 8, 19, 91);
        WaitFrames(1);
        MapGrid_CopyCells(2, 24, 1, 2, 25, 27);
      }
      MapGrid_CenterOnFollowedPoint();
      WaitFrames(1);
      break;
    case VAULT_CAVE_PASSAGE_FROM_VAULT_REVISIT:
      GameFlag_Clear(FLAG_VAULT_CAVE_SWITCH_FLIPPED);
      break;
    }
  }
  if (gSaveBlock.mapId == (s32)&MapId_VaultCaveVambrace) {
    if (GameFlag_Get(FLAG_VAULT_CAVE_IVY_BLOWN_AWAY) == 0) {
      ObjectEvent_GetById(LOCALID_IVY)->scaleY = 0x18000;
    }
    switch (gSaveBlock.warpDest) {
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
        Import_ObjectEventSetAnimation(LOCALID_WEST_UPPER_PUDDLE, 5);
        EventScript_FreezeWestUpperPuddle();
      }
      if (GameFlag_Get(FLAG_VAULT_CAVE_WEST_LOWER_PUDDLE_FROZEN) != 0) {
        Import_ObjectEventSetAnimation(LOCALID_WEST_LOWER_PUDDLE, 5);
        EventScript_FreezeWestLowerPuddle();
      }
      if (GameFlag_Get(FLAG_VAULT_CAVE_INNER_WEST_LOWER_PUDDLE_FROZEN) != 0) {
        Import_ObjectEventSetAnimation(LOCALID_INNER_WEST_LOWER_PUDDLE, 5);
        EventScript_FreezeInnerWestLowerPuddle();
      }
      if (GameFlag_Get(FLAG_VAULT_CAVE_EAST_UPPER_PUDDLE_FROZEN) != 0) {
        Import_ObjectEventSetAnimation(LOCALID_EAST_UPPER_PUDDLE, 5);
        EventScript_FreezeEastUpperPuddle();
      }
      if (GameFlag_Get(FLAG_VAULT_CAVE_EAST_LOWER_PUDDLE_FROZEN) != 0) {
        Import_ObjectEventSetAnimation(LOCALID_EAST_LOWER_PUDDLE, 5);
        EventScript_FreezeEastLowerPuddle();
      }
      if (GameFlag_Get(FLAG_VAULT_CAVE_FAR_EAST_PUDDLE_FROZEN) != 0) {
        Import_ObjectEventSetAnimation(LOCALID_FAR_EAST_PUDDLE, 5);
        EventScript_FreezeFarEastPuddle();
      }
      FieldScheduler_AddCallback(VaultCave_UpdatePuddlePriorities,
                                 TASK_PRIORITY_MAP);
      break;
    case VAULT_CAVE_VAMBRACE_FROM_VAULT_CAVE_PASSAGE_SOUTH:
    case VAULT_CAVE_VAMBRACE_SOUTH_FROM_FROST_ROOM:
      if (GameFlag_Get(FLAG_VAULT_CAVE_SOUTH_PILLAR_ON_SWITCH) != 0) {
        VaultCave_ApplySouthernSwitchCells();
        ObjectEvent_SetPosition(LOCALID_SOUTHERN_PILLAR, PIXELS(248),
                                PIXELS(876));
      }
      ObjectEvent_GetById(LOCALID_PILLAR_EAST_OF_SOUTHERN_SWITCH)
          ->priorityFlags = OBJ_EVENT_PRIORITY_UNDERFOOT;
      break;
    }
    Import_ObjectEventSetAnimation(LOCALID_PILLAR_EAST_OF_SOUTHERN_SWITCH, 2);
    Import_ObjectEventSetAnimation(LOCALID_SOUTHERN_PILLAR, 2);
    ObjectEvent_SetSpriteFlags(
        ObjectEvent_GetById(LOCALID_PILLAR_EAST_OF_SOUTHERN_SWITCH), 0);
    ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_SOUTHERN_PILLAR), 0);
    ObjectEvent_GetById(LOCALID_SOUTHERN_PILLAR)->collisionFlags = 1;
  }
  if (gSaveBlock.mapId == (s32)&MapId_VaultCaveMouth) {
    Import_ObjectEventSetAnimation(LOCALID_PILLAR_EAST_OF_NORTHERN_SWITCH, 2);
    if (GameFlag_Get(FLAG_VAULT_CAVE_NORTH_PILLAR_STRUCK) == 0) {
      Import_ObjectEventSetAnimation(LOCALID_NORTHERN_PILLAR, 2);
    }
    ObjectEvent_SetSpriteFlags(
        ObjectEvent_GetById(LOCALID_PILLAR_EAST_OF_NORTHERN_SWITCH), 0);
    ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_NORTHERN_PILLAR), 0);
    ObjectEvent_SetSpriteFlags(
        ObjectEvent_GetById(LOCALID_PILLAR_SOUTH_OF_NORTHERN_SWITCH), 0);
    ObjectEvent_GetById(LOCALID_NORTHERN_PILLAR)->collisionFlags |= 0x80;
    ObjectEvent_GetById(LOCALID_PILLAR_SOUTH_OF_NORTHERN_SWITCH)
        ->collisionFlags |= 0x80;
    switch (gSaveBlock.warpDest) {
    case VAULT_CAVE_MOUTH_NORTH_FROM_SOUTH:
    case VAULT_CAVE_MOUTH_FROM_VAULT_CAVE_VAMBRACE_NORTH:
      VaultCave_StartTimedFalls();
      ObjectEvent_GetById(LOCALID_FIRST_TIMED_WATER_DROP)->collisionFlags = 2;
      ObjectEvent_GetById(LOCALID_FIRST_TIMED_WATER_DROP + 1)->collisionFlags =
          2;
      ObjectEvent_GetById(LOCALID_FIRST_TIMED_WATER_DROP + 2)->collisionFlags =
          2;
      ObjectEvent_GetById(LOCALID_FIRST_TIMED_WATER_DROP + 3)->collisionFlags =
          2;
      ObjectEvent_GetById(LOCALID_PILLAR_EAST_OF_NORTHERN_SWITCH)
          ->collisionFlags = 1;
      ObjectEvent_GetById(LOCALID_NORTHERN_PILLAR)->collisionFlags = 1;
      ObjectEvent_GetById(LOCALID_PILLAR_SOUTH_OF_NORTHERN_SWITCH)
          ->collisionFlags = 1;
      if (GameFlag_Get(FLAG_VAULT_CAVE_NORTH_PILLAR_ON_SWITCH) != 0) {
        VaultCave_ApplyNorthernSwitchCells();
        ObjectEvent_SetPosition(LOCALID_NORTHERN_PILLAR, PIXELS(264),
                                PIXELS(204));
      }
      break;
    }
  }
  // Retreat always returns to the foot of MapId_VaultCavePassage's ladder
  // from Vault.
  gSaveBlock.retreatWarpDest = VAULT_CAVE_PASSAGE_FROM_VAULT_REVISIT;
  gSaveBlock.retreatMap = (s32)&MapId_VaultCavePassage;
  return 0;
}

// clang-format off
const s16 gVaultCave_DirectionSteps[] = {
    0, 16,
    0, 16,
    0, 16,
    16, 0,
    16, 0,
    16, 0,
    16, 0,
    0, -16,
    0, -16,
    0, -16,
    0, -16,
    -16, 0,
    -16, 0,
    -16, 0,
    -16, 0,
    0, 16,
};

const s32 gVaultCave_PuffScript[] = {
    SC_FREE,
};

struct MapWarpDest gVaultCave_FallbackWarpDests[] = {
    { WARP_DEST_DEFAULT, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { MAP_TABLE_END },
};

struct MapWarpDest gVaultCavePassage_WarpDests[] = {
    { WARP_DEST_DEFAULT, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
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
    { MAP_TABLE_END },
};

struct MapWarpDest gVaultCaveVambrace_WarpDests[] = {
    { WARP_DEST_DEFAULT, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
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
    { MAP_TABLE_END },
};

struct MapWarpDest gVaultCaveMouth_WarpDests[] = {
    { WARP_DEST_DEFAULT, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { VAULT_CAVE_MOUTH_FROM_VAULT_REVISIT, CONDITION_ALWAYS, 312, 0, 664, FACING_NORTH, 0, 200, 520, 440, 680, 0 },
    { VAULT_CAVE_MOUTH_FROM_VAULT_CAVE_PASSAGE, CONDITION_ALWAYS, 312, 0, 600, FACING_SOUTH, 0, 200, 520, 440, 680, 0 },
    { VAULT_CAVE_MOUTH_FROM_VAULT_CAVE_VAMBRACE_SOUTH, CONDITION_ALWAYS, 88, 0, 568, FACING_SOUTH, 0, 8, 472, 248, 632, 0 },
    { VAULT_CAVE_MOUTH_SOUTH_FROM_NORTH, CONDITION_ALWAYS, 168, 0, 544, FACING_SOUTH, 0, 8, 472, 248, 632, 0 },
    { VAULT_CAVE_MOUTH_NORTH_FROM_SOUTH, CONDITION_ALWAYS, 248, 0, 336, FACING_NORTH, 0, 128, 40, 376, 384, 0 },
    { VAULT_CAVE_MOUTH_FROM_VAULT_CAVE_VAMBRACE_NORTH, CONDITION_ALWAYS, 280, 0, 104, FACING_SOUTH, 0, 128, 40, 376, 384, 0 },
    { MAP_TABLE_END },
};

struct MapWarpRegion gVaultCave_Regions[] = {
    { 447, 47, 783, 463, 63, 799, -1, VAULT_CAVE_PASSAGE_TO_VAULT_REVISIT },
    { -1, 0, 0, 0, 0, 0, 0, 0 },
};

u32 gVaultCave_FallbackWarps[] = {
    MAP_WARPS_END,
};

u32 gVaultCavePassage_Warps[] = {
    MAP_WARPS(VaultCavePassage),
    MAP_WARP(VAULT_CAVE_PASSAGE_TO_VAULT_CAVE_MOUTH, VaultCaveMouth, VAULT_CAVE_MOUTH_FROM_VAULT_CAVE_PASSAGE),
    MAP_WARP(VAULT_CAVE_PASSAGE_TO_VAULT_CAVE_VAMBRACE_NORTHWEST, VaultCaveVambrace, VAULT_CAVE_VAMBRACE_FROM_VAULT_CAVE_PASSAGE_NORTH),
    MAP_WARP(VAULT_CAVE_PASSAGE_NORTH_TO_WATER_ROOM_WEST, VaultCavePassage, VAULT_CAVE_PASSAGE_WATER_ROOM_FROM_NORTH_WEST),
    MAP_WARP(VAULT_CAVE_PASSAGE_NORTH_TO_WATER_ROOM_EAST, VaultCavePassage, VAULT_CAVE_PASSAGE_WATER_ROOM_FROM_NORTH_EAST),
    MAP_WARP(VAULT_CAVE_PASSAGE_WATER_ROOM_TO_NORTH_WEST, VaultCavePassage, VAULT_CAVE_PASSAGE_NORTH_FROM_WATER_ROOM_WEST),
    MAP_WARP(VAULT_CAVE_PASSAGE_TO_VAULT_CAVE_VAMBRACE_EAST, VaultCaveVambrace, VAULT_CAVE_VAMBRACE_FROM_VAULT_CAVE_PASSAGE_SOUTHWEST),
    MAP_WARP(VAULT_CAVE_PASSAGE_TO_VAULT_CAVE_VAMBRACE_SOUTHEAST, VaultCaveVambrace, VAULT_CAVE_VAMBRACE_FROM_VAULT_CAVE_PASSAGE_SOUTH),
    MAP_WARP(VAULT_CAVE_PASSAGE_WATER_ROOM_TO_CHEST, VaultCavePassage, VAULT_CAVE_PASSAGE_CHEST_FROM_WATER_ROOM),
    MAP_WARP(VAULT_CAVE_PASSAGE_CHEST_TO_WATER_ROOM, VaultCavePassage, VAULT_CAVE_PASSAGE_WATER_ROOM_FROM_CHEST),
    MAP_WARP(VAULT_CAVE_PASSAGE_TO_VAULT_REVISIT, VaultRevisit, VAULT_REVISIT_FROM_VAULT_CAVE_PASSAGE),
    // A second VAULT_CAVE_PASSAGE_TO_VAULT_REVISIT, to
    // VAULT_CAVE_PASSAGE_LADDER_FROM_CHEST.
    MAP_WARP(VAULT_CAVE_PASSAGE_TO_VAULT_REVISIT, VaultCavePassage, VAULT_CAVE_PASSAGE_LADDER_FROM_CHEST),
    MAP_WARP(VAULT_CAVE_PASSAGE_LADDER_TO_CHEST, VaultCavePassage, VAULT_CAVE_PASSAGE_CHEST_FROM_LADDER),
    MAP_WARP(VAULT_CAVE_PASSAGE_CHEST_TO_LADDER, VaultCavePassage, VAULT_CAVE_PASSAGE_LADDER_FROM_CHEST),
    MAP_WARP(VAULT_CAVE_PASSAGE_WATER_ROOM_TO_NORTH_EAST, VaultCavePassage, VAULT_CAVE_PASSAGE_NORTH_FROM_WATER_ROOM_EAST),
    MAP_WARPS_END,
};

u32 gVaultCaveVambrace_Warps[] = {
    MAP_WARPS(VaultCaveVambrace),
    MAP_WARP(VAULT_CAVE_VAMBRACE_TO_VAULT_CAVE_PASSAGE_NORTH, VaultCavePassage, VAULT_CAVE_PASSAGE_FROM_VAULT_CAVE_VAMBRACE_NORTHWEST),
    MAP_WARP(VAULT_CAVE_VAMBRACE_TO_VAULT_CAVE_MOUTH_NORTH, VaultCaveMouth, VAULT_CAVE_MOUTH_FROM_VAULT_CAVE_VAMBRACE_NORTH),
    MAP_WARP(VAULT_CAVE_VAMBRACE_NORTH_TO_FROST_ROOM, VaultCaveVambrace, VAULT_CAVE_VAMBRACE_FROST_ROOM_FROM_NORTH),
    MAP_WARP(VAULT_CAVE_VAMBRACE_NORTH_TO_WEST, VaultCaveVambrace, VAULT_CAVE_VAMBRACE_WEST_FROM_NORTH),
    MAP_WARP(VAULT_CAVE_VAMBRACE_WEST_TO_NORTH, VaultCaveVambrace, VAULT_CAVE_VAMBRACE_NORTH_FROM_WEST),
    MAP_WARP(VAULT_CAVE_VAMBRACE_TO_VAULT_CAVE_PASSAGE_SOUTHWEST, VaultCavePassage, VAULT_CAVE_PASSAGE_FROM_VAULT_CAVE_VAMBRACE_EAST),
    MAP_WARP(VAULT_CAVE_VAMBRACE_TO_VAULT_CAVE_MOUTH_SOUTH, VaultCaveMouth, VAULT_CAVE_MOUTH_FROM_VAULT_CAVE_VAMBRACE_SOUTH),
    MAP_WARP(VAULT_CAVE_VAMBRACE_FROST_ROOM_TO_NORTH, VaultCaveVambrace, VAULT_CAVE_VAMBRACE_NORTH_FROM_FROST_ROOM),
    MAP_WARP(VAULT_CAVE_VAMBRACE_FROST_ROOM_TO_SOUTH, VaultCaveVambrace, VAULT_CAVE_VAMBRACE_SOUTH_FROM_FROST_ROOM),
    MAP_WARP(VAULT_CAVE_VAMBRACE_TO_VAULT_CAVE_PASSAGE_SOUTH, VaultCavePassage, VAULT_CAVE_PASSAGE_FROM_VAULT_CAVE_VAMBRACE_SOUTHEAST),
    MAP_WARP(VAULT_CAVE_VAMBRACE_SOUTH_TO_FROST_ROOM, VaultCaveVambrace, VAULT_CAVE_VAMBRACE_FROST_ROOM_FROM_SOUTH),
    MAP_WARP(VAULT_CAVE_VAMBRACE_FROST_ROOM_TO_CHEST, VaultCaveVambrace, VAULT_CAVE_VAMBRACE_CHEST_FROM_FROST_ROOM),
    MAP_WARP(VAULT_CAVE_VAMBRACE_CHEST_TO_FROST_ROOM, VaultCaveVambrace, VAULT_CAVE_VAMBRACE_FROST_ROOM_FROM_CHEST),
    MAP_WARPS_END,
};

u32 gVaultCaveMouth_Warps[] = {
    MAP_WARPS(VaultCaveMouth),
    MAP_WARP(VAULT_CAVE_MOUTH_TO_VAULT_REVISIT, VaultRevisit, VAULT_REVISIT_FROM_VAULT_CAVE_MOUTH),
    MAP_WARP(VAULT_CAVE_MOUTH_TO_VAULT_CAVE_PASSAGE, VaultCavePassage, VAULT_CAVE_PASSAGE_FROM_VAULT_CAVE_MOUTH),
    MAP_WARP(VAULT_CAVE_MOUTH_TO_VAULT_CAVE_VAMBRACE_SOUTH, VaultCaveVambrace, VAULT_CAVE_VAMBRACE_FROM_VAULT_CAVE_MOUTH_SOUTH),
    MAP_WARP(VAULT_CAVE_MOUTH_SOUTH_TO_NORTH, VaultCaveMouth, VAULT_CAVE_MOUTH_NORTH_FROM_SOUTH),
    MAP_WARP(VAULT_CAVE_MOUTH_NORTH_TO_SOUTH, VaultCaveMouth, VAULT_CAVE_MOUTH_SOUTH_FROM_NORTH),
    MAP_WARP(VAULT_CAVE_MOUTH_TO_VAULT_CAVE_VAMBRACE_NORTH, VaultCaveVambrace, VAULT_CAVE_VAMBRACE_FROM_VAULT_CAVE_MOUTH_NORTH),
    MAP_WARPS_END,
};

struct ObjectEventTemplate gVaultCave_FallbackObjectEventTemplates[] = {
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gVaultCavePassage_ObjectEventTemplates[] = {
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gVaultCaveVambrace_ObjectEventTemplates[] = {
    { 0x12b, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(280), 0, PIXELS(876), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { 0x12b, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(184), 0, PIXELS(892), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { 0xf0, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(372), 0, PIXELS(512), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { 0xf0, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(372), 0, PIXELS(544), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { 0xf0, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(388), 0, PIXELS(544), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { 0xf0, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(420), 0, PIXELS(512), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { 0xf0, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(420), 0, PIXELS(544), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { 0xf0, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(452), 0, PIXELS(528), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(376), 0, PIXELS(520), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(376), 0, PIXELS(552), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(392), 0, PIXELS(552), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(424), 0, PIXELS(520), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(424), 0, PIXELS(552), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(456), 0, PIXELS(536), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_IVY, FLAG_VAULT_CAVE_IVY_BLOWN_AWAY, MOVEMENT_TYPE_STAND,
        PIXELS(456), 0, PIXELS(456), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gVaultCaveMouth_ObjectEventTemplates[] = {
    { 0x12b, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(296), 0, PIXELS(204), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { 0x12b, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(264), 0, PIXELS(236), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { 0x12b, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(264), 0, PIXELS(300), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { 0xf0, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(248), 0, PIXELS(232), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { 0xf0, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(264), 0, PIXELS(248), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { 0xf0, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(264), 0, PIXELS(264), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { 0xf0, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(280), 0, PIXELS(280), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { MAP_TABLE_END },
};

struct MapEvent gVaultCave_FallbackEvents[] = {
    { MAP_EVENTS_END },
};

struct MapEvent gVaultCavePassage_Events[] = {
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, VAULT_CAVE_PASSAGE_TO_VAULT_CAVE_MOUTH },
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, VAULT_CAVE_PASSAGE_TO_VAULT_CAVE_VAMBRACE_NORTHWEST },
    { MAP_EVENT_WARP, 3, CONDITION_ALWAYS, VAULT_CAVE_PASSAGE_NORTH_TO_WATER_ROOM_WEST },
    { MAP_EVENT_WARP, 4, CONDITION_ALWAYS, VAULT_CAVE_PASSAGE_NORTH_TO_WATER_ROOM_EAST },
    { MAP_EVENT_WARP, 5, CONDITION_ALWAYS, VAULT_CAVE_PASSAGE_WATER_ROOM_TO_NORTH_WEST },
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_DOWN, 6, CONDITION_ALWAYS, VAULT_CAVE_PASSAGE_TO_VAULT_CAVE_VAMBRACE_EAST },
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_DOWN, 7, CONDITION_ALWAYS, VAULT_CAVE_PASSAGE_TO_VAULT_CAVE_VAMBRACE_SOUTHEAST },
    { MAP_EVENT_WARP, 8, CONDITION_ALWAYS, VAULT_CAVE_PASSAGE_WATER_ROOM_TO_CHEST },
    { MAP_EVENT_WARP, 9, CONDITION_ALWAYS, VAULT_CAVE_PASSAGE_CHEST_TO_WATER_ROOM },
    { MAP_EVENT_WARP, 10, CONDITION_ALWAYS, VAULT_CAVE_PASSAGE_TO_VAULT_REVISIT },
    { MAP_EVENT_WARP, 11, CONDITION_ALWAYS, VAULT_CAVE_PASSAGE_LADDER_TO_CHEST },
    { MAP_EVENT_WARP, 12, CONDITION_ALWAYS, VAULT_CAVE_PASSAGE_CHEST_TO_LADDER },
    { MAP_EVENT_WARP, 13, CONDITION_ALWAYS, VAULT_CAVE_PASSAGE_WATER_ROOM_TO_NORTH_EAST },
    { BG_EVENT(BG_EVENT_SILENT), MB_VAULT_CAVE_SWITCH, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_FlipSwitch) },
    { BG_EVENT(BG_EVENT_SILENT), MB_VAULT_CAVE_LOCKED_DOOR, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ShowLockedDoor) },
    { BG_EVENT(BG_EVENT_CHEST), MB_VAULT_CAVE_CHEST, FLAG_VAULT_CAVE_PASSAGE_MIMIC_BEATEN, FIND_MIMIC | 3 },
    { MAP_EVENTS_END },
};

struct MapEvent gVaultCaveVambrace_Events[] = {
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, VAULT_CAVE_VAMBRACE_TO_VAULT_CAVE_PASSAGE_NORTH },
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, VAULT_CAVE_VAMBRACE_TO_VAULT_CAVE_MOUTH_NORTH },
    { MAP_EVENT_WARP, 3, CONDITION_ALWAYS, VAULT_CAVE_VAMBRACE_NORTH_TO_FROST_ROOM },
    { MAP_EVENT_WARP, 4, CONDITION_ALWAYS, VAULT_CAVE_VAMBRACE_NORTH_TO_WEST },
    { MAP_EVENT_WARP, 5, CONDITION_ALWAYS, VAULT_CAVE_VAMBRACE_WEST_TO_NORTH },
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_UP, 6, CONDITION_ALWAYS, VAULT_CAVE_VAMBRACE_TO_VAULT_CAVE_PASSAGE_SOUTHWEST },
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_DOWN, 7, CONDITION_ALWAYS, VAULT_CAVE_VAMBRACE_TO_VAULT_CAVE_MOUTH_SOUTH },
    { MAP_EVENT_WARP, 8, CONDITION_ALWAYS, VAULT_CAVE_VAMBRACE_FROST_ROOM_TO_NORTH },
    { MAP_EVENT_WARP, 9, CONDITION_ALWAYS, VAULT_CAVE_VAMBRACE_FROST_ROOM_TO_SOUTH },
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_UP, 10, CONDITION_ALWAYS, VAULT_CAVE_VAMBRACE_TO_VAULT_CAVE_PASSAGE_SOUTH },
    { MAP_EVENT_WARP, 11, CONDITION_ALWAYS, VAULT_CAVE_VAMBRACE_SOUTH_TO_FROST_ROOM },
    { MAP_EVENT_WARP, 13, CONDITION_ALWAYS, VAULT_CAVE_VAMBRACE_FROST_ROOM_TO_CHEST },
    { MAP_EVENT_WARP, 14, CONDITION_ALWAYS, VAULT_CAVE_VAMBRACE_CHEST_TO_FROST_ROOM },
    { BG_EVENT(BG_EVENT_CHEST), MB_VAULT_CAVE_CHEST, FLAG_VAULT_CAVE_VAMBRACE_CHEST, FIND_ITEM(ITEM_VAMBRACE) },
    { BG_EVENT(BG_EVENT_SILENT), MB_VAULT_CAVE_LOCKED_DOOR, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ShowLockedDoor) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_SOUTHERN_PILLAR, FLAG_VAULT_CAVE_SOUTH_PILLAR_ON_SWITCH, EVENT_SCRIPT(EventScript_MoveSouthernPillar) },
    { MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING, MB_VAULT_CAVE_SOUTHERN_PILLAR, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_PushSouthernPillar) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_FROST), LOCALID_WEST_UPPER_PUDDLE, FLAG_VAULT_CAVE_WEST_UPPER_PUDDLE_FROZEN, EVENT_SCRIPT(EventScript_FreezeWestUpperPuddle) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_FROST), LOCALID_WEST_LOWER_PUDDLE, FLAG_VAULT_CAVE_WEST_LOWER_PUDDLE_FROZEN, EVENT_SCRIPT(EventScript_FreezeWestLowerPuddle) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_FROST), LOCALID_INNER_WEST_LOWER_PUDDLE, FLAG_VAULT_CAVE_INNER_WEST_LOWER_PUDDLE_FROZEN, EVENT_SCRIPT(EventScript_FreezeInnerWestLowerPuddle) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_FROST), LOCALID_EAST_UPPER_PUDDLE, FLAG_VAULT_CAVE_EAST_UPPER_PUDDLE_FROZEN, EVENT_SCRIPT(EventScript_FreezeEastUpperPuddle) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_FROST), LOCALID_EAST_LOWER_PUDDLE, FLAG_VAULT_CAVE_EAST_LOWER_PUDDLE_FROZEN, EVENT_SCRIPT(EventScript_FreezeEastLowerPuddle) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_FROST), LOCALID_FAR_EAST_PUDDLE, FLAG_VAULT_CAVE_FAR_EAST_PUDDLE_FROZEN, EVENT_SCRIPT(EventScript_FreezeFarEastPuddle) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_WHIRLWIND), LOCALID_IVY, FLAG_VAULT_CAVE_IVY_BLOWN_AWAY, EVENT_SCRIPT(EventScript_UseWhirlwind) },
    { MAP_EVENTS_END },
};

struct MapEvent gVaultCaveMouth_Events[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, VAULT_CAVE_MOUTH_TO_VAULT_REVISIT },
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, VAULT_CAVE_MOUTH_TO_VAULT_CAVE_PASSAGE },
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_UP, 3, CONDITION_ALWAYS, VAULT_CAVE_MOUTH_TO_VAULT_CAVE_VAMBRACE_SOUTH },
    { MAP_EVENT_WARP, 4, CONDITION_ALWAYS, VAULT_CAVE_MOUTH_SOUTH_TO_NORTH },
    { MAP_EVENT_WARP, 5, CONDITION_ALWAYS, VAULT_CAVE_MOUTH_NORTH_TO_SOUTH },
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_UP, 6, CONDITION_ALWAYS, VAULT_CAVE_MOUTH_TO_VAULT_CAVE_VAMBRACE_NORTH },
    { MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING, MB_VAULT_CAVE_NORTHERN_PILLAR, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_PushNorthernPillar) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_NORTHERN_PILLAR, FLAG_VAULT_CAVE_NORTH_PILLAR_ON_SWITCH, EVENT_SCRIPT(EventScript_MoveNorthernPillar) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_PILLAR_SOUTH_OF_NORTHERN_SWITCH, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_IgnoreMove) },
    { BG_EVENT(BG_EVENT_SILENT), MB_VAULT_CAVE_LOCKED_DOOR, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ShowLockedDoor) },
    { MAP_EVENTS_END },
};

u32 gVaultCave_ScheduleFrames[] = {
    20, 80, 115, 170,
};
// clang-format on

s32 gVaultCave_ScheduleTimer = 0;

s32 gVaultCave_ScheduleIndex = 0;

s32 gVaultCave_TickCounter = 0;

s32 gVaultCave_TickValue = 0;

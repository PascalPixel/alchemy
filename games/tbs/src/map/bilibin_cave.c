/// @file
/// The Bilibin Cave Entrance and Bilibin Cave, two map sections between Bilibin
/// and Imil that share this overlay: the Bilibin Cave Entrance is
/// MapId_BilibinCaveEntrance, and Bilibin Cave is MapId_BilibinCaveRapier and,
/// beyond it, MapId_BilibinCaveIce, whose far end opens onto the world map
/// farther north. No one speaks here: each map is a run of Psynergy puzzles,
/// the getters pick its tables by gSaveBlock.mapId, and BilibinCave_Initialize
/// restores what the party has already solved in the map being entered.
///
/// At the Bilibin Cave Entrance, entered from the world map, Growth on a sprout
/// (LOCALID_SPROUT) grows it into a vine and changes the metatile behavior of
/// its cell (EventScript_GrowPlant); FLAG_BILIBIN_CAVE_ENTRANCE_SPROUT_GROWN
/// keeps it grown until the party leaves the Bilibin Cave Entrance, into the
/// cave beyond or out to the world map, since both are other map sections. Two
/// warps lead on into Bilibin Cave. The south one's warp cell, at the cave
/// mouth, lies under the ledge path that runs above the mouth, since the map
/// grid has one cell for both: coord event 10, the ledge cells either side of
/// it, turns the warp off so the leader can walk the ledge across it, and coord
/// event 11, a step further out, turns it back on
/// (EventScript_WalkOntoLedgeOverCaveMouth,
/// EventScript_WalkOffLedgeOverCaveMouth; its recording,
/// places/bilibin-cave-entrance/events/EventScript_WalkOntoLedgeOverCaveMouth.gif).
///
/// In MapId_BilibinCaveRapier a fire burns between two torches, the west one
/// cold and the east one lit (LOCALID_RAPIER_ROOM_FIRE), and Douse puts it out
/// for good (FLAG_BILIBIN_CAVE_SECOND_FIRE_DOUSED). Reveal shows a hidden path,
/// a strip of tiles and the cells under them, which
/// EventScript_HideRevealedPath takes away again when Reveal wears off. A chest
/// holds the Elven Rapier.
///
/// MapId_BilibinCaveIce holds the cave's one puzzle of several steps, built
/// from fires, torches and puddles that Frost freezes into ice pillars and the
/// heat of a fire or a torch melts back (PUDDLE_ANIMATION_MELT). While
/// LOCALID_ICE_ROOM_FIRE burns, Frost on the puddle beside it only melts back;
/// Douse puts the fire out (FLAG_BILIBIN_CAVE_FIRST_FIRE_DOUSED), and the
/// puddle freezes into a pillar drawn underfoot, which the party walks over.
/// The second puddle stands beside a torch that cannot be moved, and Frost on
/// it always melts back. The third is an ice pillar: two of the six torches
/// (LOCALID_THIRD_TORCH and LOCALID_FOURTH_TORCH) can be pushed or moved with
/// Move, and while either stands beside the pillar, on cell (35, 23), the
/// pillar melts into a puddle and the cells north and south of it change with
/// it; when both leave it freezes again
/// (BilibinCave_UpdateThirdPuddleFromTorches). Melting turns the metatile
/// behaviors of cells (36, 22) and (36, 24), north and south of the puddle,
/// from MB_ENCOUNTER_GROUND_1 to MB_JUMP_FORWARD
/// (https://alchemy.pascalpixel.com/places/bilibin-cave-ice/third-puddle-melt.gif:
/// the fourth torch, put a cell west of (35, 23), moved onto it with Move, its
/// cell outlined in yellow and the two rewritten cells in red, each labelled
/// with what it does, encounter ground and then a jump across).
/// FLAG_BILIBIN_CAVE_THIRD_TORCH_BY_PILLAR and
/// FLAG_BILIBIN_CAVE_FOURTH_TORCH_BY_PILLAR remember which torch stands there
/// and FLAG_BILIBIN_CAVE_THIRD_PUDDLE_MELTED that the pillar has melted, so
/// coming back from MapId_BilibinCaveRapier puts the torches and the pillar
/// back; leaving Bilibin Cave resets them. Nine more pieces of ice only stand
/// there: five pillars from LOCALID_FIRST_ICE_PILLAR and four lower pieces from
/// LOCALID_FIRST_LOW_ICE. Coord event 20 covers the snow blown in at the cave's
/// mouth to the world map, around BILIBIN_CAVE_ICE_FROM_WORLD_MAP, and makes
/// the leader leave footprints; coord event 21, rimming it, stops them
/// (EventScript_StartFootprints, EventScript_StopFootprints; the snow is in
/// places/bilibin-cave-ice/map-1.png). Its chests hold a Vial, a Psy Crystal
/// and Water of Life.
///
/// Open questions: none.

#include "action_script.h"
#include "save_block.h"
#include "engine_call.h"
#include "map_ids.h"
#include "unit/import.h"
#include "unit/map/bilibin_cave.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/field_services.h"
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
#include "heap_slots.h"
#include "map_grid.h"
#include "types.h"
#include "script_layer.h"
#include "map_tables.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"
#include "object_event.h"
#include "flags.h"
#include "songs.h"
#include "map_grid_scroll.h"

/// In Bilibin Cave's ice room, a movable torch has melted the third puddle's
/// ice pillar.
/// ★ Named from what the player sees: a tall pillar of ice beside the lit
/// torches in the cave's east passage (its render)
#define FLAG_BILIBIN_CAVE_THIRD_PUDDLE_MELTED FLAG_MAPSEC_TEMP_02
/// In Bilibin Cave's ice room, the third torch stands beside the third puddle's
/// ice pillar.
/// ★ Named from what the player sees: golden torches burning on their stands
/// near a pillar of ice (its render)
#define FLAG_BILIBIN_CAVE_THIRD_TORCH_BY_PILLAR FLAG_MAPSEC_TEMP_03
/// In Bilibin Cave's ice room, the fourth torch stands beside the third
/// puddle's ice pillar.
/// ★ Named from what the player sees: golden torches burning on their stands
/// near a pillar of ice (its render)
#define FLAG_BILIBIN_CAVE_FOURTH_TORCH_BY_PILLAR FLAG_MAPSEC_TEMP_04
/// At the Bilibin Cave Entrance, Growth has grown the sprout into a vine.
#define FLAG_BILIBIN_CAVE_ENTRANCE_SPROUT_GROWN FLAG_MAPSEC_TEMP_05

// BilibinCave_Initialize calls one of these for the map being entered.
static void BilibinCave_RestoreEntrance(void);
static void BilibinCave_RestoreRapierRoom(void);
static void BilibinCave_RestoreIceRoom(void);
static void BilibinCave_EmptyObjectEventSetupHook(s32 localId);

/// Object events of the Bilibin Cave Entrance (MapId_BilibinCaveEntrance).
enum {
  /// The sprout Growth grows into a vine.
  /// ★ Named from what the player sees: a green sprout, SPRITE_SPROUT (its
  /// render)
  LOCALID_SPROUT = 8
};

/// Object events of MapId_BilibinCaveRapier, numbered by the rows of
/// gBilibinCaveRapierObjectEventTemplates.
enum {
  /// A torch west of the fire, cold (TORCH_ANIMATION_UNLIT) where its twin
  /// burns.
  /// ★ Named from what the player sees: a golden torch on its stand, its bowl
  /// grey with ash, places/bilibin-cave-rapier/map-5.png (its render)
  LOCALID_RAPIER_ROOM_WEST_TORCH = 8,
  /// A fire between two torches, which Douse puts out.
  /// ★ Named from what the player sees: a flickering flame, SPRITE_FIRE (its
  /// render)
  LOCALID_RAPIER_ROOM_FIRE = 9,
  /// A torch east of the fire.
  /// ★ Named from what the player sees: a golden torch burning on its stand
  /// (its render)
  LOCALID_RAPIER_ROOM_EAST_TORCH = 10
};

/// Object events of MapId_BilibinCaveIce, numbered by the rows of
/// gBilibinCaveIceObjectEventTemplates.
enum {
  /// A fire beside the first puddle, which Douse puts out.
  /// ★ Named from what the player sees: a flickering flame, SPRITE_FIRE (its
  /// render)
  LOCALID_ICE_ROOM_FIRE = 8,
  /// Six torches, 9 to 14; the second stands beside LOCALID_SECOND_PUDDLE.
  /// ★ Named from what the player sees: a golden torch burning on its stand
  /// (its render)
  LOCALID_FIRST_TORCH = 9,
  /// The third torch, moved by Move or pushed.
  /// ★ Named from what the player sees: a golden torch burning on its stand
  /// (its render)
  LOCALID_THIRD_TORCH = 11,
  /// The fourth torch, moved by Move or pushed.
  /// ★ Named from what the player sees: a golden torch burning on its stand
  /// (its render)
  LOCALID_FOURTH_TORCH = 12,
  /// A puddle beside LOCALID_ICE_ROOM_FIRE, which freezes into a pillar the
  /// party walks over once the fire is out.
  /// ★ Named from what the player sees: a puddle of water, SPRITE_PUDDLE (its
  /// render)
  LOCALID_FIRST_PUDDLE = 15,
  /// A puddle beside a torch that stays put, so it never stays frozen.
  /// ★ Named from what the player sees: a puddle of water, SPRITE_PUDDLE (its
  /// render)
  LOCALID_SECOND_PUDDLE = 16,
  /// A puddle frozen into an ice pillar, melted while a torch stands on cell
  /// (35, 23).
  /// ★ Named from what the player sees: a puddle of water, SPRITE_PUDDLE (its
  /// render)
  LOCALID_THIRD_PUDDLE = 17,
  /// Five ice pillars, 18 to 22 (PUDDLE_ANIMATION_PILLAR); nothing acts on
  /// them.
  /// ★ Named from what the player sees: tall pillars of pale blue ice (its
  /// render)
  LOCALID_FIRST_ICE_PILLAR = 18,
  /// Four pieces of ice, 23 to 26, held at the second step of freezing
  /// (PUDDLE_ANIMATION_LOW_ICE); nothing acts on them.
  /// ★ Named from what the player sees: low, jagged lumps of pale blue ice
  /// (its render)
  LOCALID_FIRST_LOW_ICE = 23
};

/// Clears the scripted object event's sprite flags and completes this script
/// command.
static s32
BilibinCave_ClearScriptedObjectEventSpriteFlags(struct Object *interpreter) {
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)interpreter;
  ObjectEvent_SetSpriteFlags(objectEvent, 0);
  return 0;
}

const struct MapWarpDest *BilibinCave_GetWarpDests(void) {
  s16 mapId = gSaveBlock.mapId;

  if (mapId == (s32)&MapId_BilibinCaveEntrance) {
    return gBilibinCaveEntranceWarpDests;
  }
  if (mapId == (s32)&MapId_BilibinCaveRapier) {
    return gBilibinCaveRapierWarpDests;
  }
  if (mapId == (s32)&MapId_BilibinCaveIce) {
    return gBilibinCaveIceWarpDests;
  }
  return gBilibinCaveWarpDestsOther;
}

const struct MapWarpRegion *BilibinCave_GetRegions(void) { return NULL; }

const u32 *BilibinCave_GetWarps(void) { return gBilibinCaveWarps; }

const struct ObjectEventTemplate *BilibinCave_GetObjectEventTemplates(void) {
  s16 mapId = gSaveBlock.mapId;

  if (mapId == (s32)&MapId_BilibinCaveEntrance) {
    return gBilibinCaveEntranceObjectEventTemplates;
  }
  if (mapId == (s32)&MapId_BilibinCaveRapier) {
    return gBilibinCaveRapierObjectEventTemplates;
  }
  if (mapId == (s32)&MapId_BilibinCaveIce) {
    return gBilibinCaveIceObjectEventTemplates;
  }
  return gBilibinCaveObjectEventTemplatesOther;
}

const struct MapEvent *BilibinCave_GetEvents(void) {
  s16 mapId = gSaveBlock.mapId;

  if (mapId == (s32)&MapId_BilibinCaveEntrance) {
    return gBilibinCaveEntranceEvents;
  }
  if (mapId == (s32)&MapId_BilibinCaveRapier) {
    return gBilibinCaveRapierEvents;
  }
  if (mapId == (s32)&MapId_BilibinCaveIce) {
    return gBilibinCaveIceEvents;
  }
  return gBilibinCaveEventsOther;
}

/// Stepping on coord event 10 at the Bilibin Cave Entrance, on either side of
/// the way into Bilibin Cave: the way's cell (21, 14), the warp cell of
/// BILIBIN_CAVE_ENTRANCE_TO_BILIBIN_CAVE_RAPIER_SOUTH, takes the
/// metatile behavior of source cell (1, 0), which has none.
static void EventScript_WalkOntoLedgeOverCaveMouth(void) {
  MapGrid_CopyCellAttributes(1, 0, 1, 1, 21, 14);
}

/// Stepping on coord event 11 at the Bilibin Cave Entrance, a cell further out:
/// the way's cell (21, 14) takes back the warp, from source cell (0, 0).
static void EventScript_WalkOffLedgeOverCaveMouth(void) {
  MapGrid_CopyCellAttributes(0, 0, 1, 1, 21, 14);
}

/// Reveal in Bilibin Cave (MapId_BilibinCaveRapier) shows the hidden path: a
/// strip of tiles and two rows of the cells under it.
static void EventScript_RevealHiddenPath(void) {
  {
    s32 width = 1;
    s32 height = 3;

    MapGrid_CopyCellsTo(111, 37, 97, 21, width, height);
  }
  {
    s32 destinationCellX = 32;
    s32 destinationCellZ = 24;

    MapGrid_CopyCellAttributes(46, 38, 3, 2, destinationCellX,
                               destinationCellZ);
  }
}

/// As Reveal wears off in Bilibin Cave (MapId_BilibinCaveRapier), the path is
/// hidden again: the strip's tiles and one row of its cells are restored.
static void EventScript_HideRevealedPath(void) {
  {
    s32 width = 1;
    s32 height = 3;

    MapGrid_CopyCellsTo(95, 21, 97, 21, width, height);
  }
  {
    s32 destinationCellX = 32;
    s32 destinationCellZ = 25;

    MapGrid_CopyCellAttributes(46, 38, 3, 1, destinationCellX,
                               destinationCellZ);
  }
}

/// Douse on the fire in Bilibin Cave (MapId_BilibinCaveRapier), until it is
/// out: it goes out for good.
static void EventScript_DouseRapierRoomFire(void) {
  FieldScript_Start();
  Import_ObjectEventSetPosition(LOCALID_RAPIER_ROOM_FIRE, 0, 0);
  GameFlag_Set(FLAG_BILIBIN_CAVE_SECOND_FIRE_DOUSED);
  FieldScript_End();
}

/// Douse on the fire in Bilibin Cave (MapId_BilibinCaveIce), until it is out:
/// it goes out, and the puddle beside it freezes into a pillar the party can
/// walk over.
static void EventScript_DouseIceRoomFire(void) {
  FieldScript_Start();
  ObjectEvent_SetPosition(LOCALID_ICE_ROOM_FIRE, 0, 0);
  GameFlag_Set(FLAG_BILIBIN_CAVE_FIRST_FIRE_DOUSED);
  FieldScript_WaitFrames(40);
  // Match the replacement's drawing priority to its metatile behavior.
  ObjectMotion_SetAnimationAndWait(LOCALID_FIRST_PUDDLE,
                                   PUDDLE_ANIMATION_FREEZE);
  ObjectEvent_Get(LOCALID_FIRST_PUDDLE)->motionFlags = 0;
  ObjectEvent_Get(LOCALID_FIRST_PUDDLE)->priorityFlags |=
      OBJ_EVENT_PRIORITY_UNDERFOOT;
  ObjectMotion_SetActionVariant(LOCALID_FIRST_PUDDLE, 2);
  MapGrid_CopyCellAttributes(0, 0, 1, 1, 18, 14);
  FieldScript_End();
}

/// Frost on a puddle in Bilibin Cave (MapId_BilibinCaveIce) that heat keeps
/// from freezing: clears the Psynergy's child effect and plays the ice melting
/// back into a puddle.
static inline void BilibinCave_MeltFrostBack(s32 localId) {
  FieldScript_Start();
  ObjectEvent_ConfigureSpriteColor(localId, 0);
  FieldScript_WaitFrames(40);
  Sound_PlaySong(SE_HIDDEN_PASSAGE_OPEN);
  ObjectMotion_SetAnimationAndWait(localId, PUDDLE_ANIMATION_MELT);
  FieldScript_End();
}

/// Frost on the first puddle in Bilibin Cave (MapId_BilibinCaveIce), while
/// the fire beside it burns.
static void EventScript_FreezeFirstPuddle(void) {
  BilibinCave_MeltFrostBack(LOCALID_FIRST_PUDDLE);
}

/// Frost on the second puddle in Bilibin Cave (MapId_BilibinCaveIce), beside
/// a torch that stays put.
static void EventScript_FreezeSecondPuddle(void) {
  BilibinCave_MeltFrostBack(LOCALID_SECOND_PUDDLE);
}

/// Frost on the third puddle in Bilibin Cave (MapId_BilibinCaveIce), while a
/// torch beside it keeps it melted (FLAG_BILIBIN_CAVE_THIRD_PUDDLE_MELTED).
static void EventScript_FreezeThirdPuddle(void) {
  BilibinCave_MeltFrostBack(LOCALID_THIRD_PUDDLE);
}

/// A movable torch melts the ice pillar beside it. This runs after a push or
/// Move in Bilibin Cave (MapId_BilibinCaveIce): while either movable torch
/// stands on cell (35, 23) (FLAG_BILIBIN_CAVE_THIRD_TORCH_BY_PILLAR and
/// FLAG_BILIBIN_CAVE_FOURTH_TORCH_BY_PILLAR), the ice pillar beside it melts
/// with a sound, and FLAG_BILIBIN_CAVE_THIRD_PUDDLE_MELTED says so; when both
/// leave, it freezes again.
static void BilibinCave_UpdateThirdPuddleFromTorches(void) {
  struct ObjectEvent *firstObjectEvent;
  struct ObjectEvent *secondObjectEvent;
  s32 destinationCellX;

  // Track each movable object event independently before changing the Frost
  // target.
  firstObjectEvent = ObjectEvent_GetById(LOCALID_THIRD_TORCH);
  secondObjectEvent = ObjectEvent_Get(LOCALID_FOURTH_TORCH);
  if ((firstObjectEvent->x.fixed >> 20) == 35 &&
      (firstObjectEvent->z.fixed >> 20) == 23) {
    GameFlag_Set(FLAG_BILIBIN_CAVE_THIRD_TORCH_BY_PILLAR);
  } else {
    GameFlag_Clear(FLAG_BILIBIN_CAVE_THIRD_TORCH_BY_PILLAR);
  }
  if ((secondObjectEvent->x.fixed >> 20) == 35 &&
      (secondObjectEvent->z.fixed >> 20) == 23) {
    GameFlag_Set(FLAG_BILIBIN_CAVE_FOURTH_TORCH_BY_PILLAR);
  } else {
    GameFlag_Clear(FLAG_BILIBIN_CAVE_FOURTH_TORCH_BY_PILLAR);
  }
  if (GameFlag_Get(FLAG_BILIBIN_CAVE_THIRD_TORCH_BY_PILLAR) != 0 ||
      GameFlag_Get(FLAG_BILIBIN_CAVE_FOURTH_TORCH_BY_PILLAR) != 0) {
    if (Script_GetFlag(FLAG_BILIBIN_CAVE_THIRD_PUDDLE_MELTED) == 0) {
      FieldScript_Start();
      FieldScript_WaitFrames(40);
      Script_PlaySong(SE_HIDDEN_PASSAGE_OPEN);
      destinationCellX = 36;
      ObjectMotion_SetAnimationAndWait(LOCALID_THIRD_PUDDLE,
                                       PUDDLE_ANIMATION_MELT);
      MapGrid_CopyCellAttributes(0, 1, 1, 1, destinationCellX, 22);
      MapGrid_CopyCellAttributes(0, 2, 1, 1, destinationCellX, 24);
      FieldScript_End();
    }
    GameFlag_Set(FLAG_BILIBIN_CAVE_THIRD_PUDDLE_MELTED);
  } else {
    if (Script_GetFlag(FLAG_BILIBIN_CAVE_THIRD_PUDDLE_MELTED) != 0) {
      FieldScript_Start();
      FieldScript_WaitFrames(40);
      Script_PlaySong(SE_SHIMMER);
      destinationCellX = 36;
      ObjectMotion_SetAnimationAndWait(LOCALID_THIRD_PUDDLE,
                                       PUDDLE_ANIMATION_FREEZE);
      MapGrid_CopyCellAttributes(1, 1, 1, 1, destinationCellX, 22);
      MapGrid_CopyCellAttributes(1, 2, 1, 1, destinationCellX, 24);
      FieldScript_End();
    }
    GameFlag_Clear(FLAG_BILIBIN_CAVE_THIRD_PUDDLE_MELTED);
  }
}

/// Growth on the sprout at the Bilibin Cave Entrance, until it has grown: it
/// grows into a vine and its cell's metatile behavior changes;
/// FLAG_BILIBIN_CAVE_ENTRANCE_SPROUT_GROWN keeps it grown.
static void EventScript_GrowPlant(void) {

  MapGrid_CopyCellAttributes(31, 0, 1, 1, 8, 13);
  GameFlag_Set(FLAG_BILIBIN_CAVE_ENTRANCE_SPROUT_GROWN);
}

/// Stepping on coord event 20 in Bilibin Cave (MapId_BilibinCaveIce): from here
/// on the leader leaves footprints.
static void EventScript_StartFootprints(void) {
  struct MapGridScrollWork *grid = gMapGridWork[0];
  grid->footprints = 1;
}

/// Stepping on coord event 21 in Bilibin Cave (MapId_BilibinCaveIce): the
/// leader leaves no more footprints.
static void EventScript_StopFootprints(void) {
  struct MapGridScrollWork *grid = gMapGridWork[0];
  grid->footprints = 0;
}

/// Sets up the map being entered, with its puzzles already solved. The map
/// is the Bilibin Cave Entrance or one of Bilibin Cave's two.
s32 BilibinCave_Initialize(void) {
  s16 variant = gSaveBlock.mapId;

  if (variant == (s32)&MapId_BilibinCaveEntrance) {
    BilibinCave_RestoreEntrance();
  } else if (variant == (s32)&MapId_BilibinCaveRapier) {
    BilibinCave_RestoreRapierRoom();
  } else if (variant == (s32)&MapId_BilibinCaveIce) {
    BilibinCave_RestoreIceRoom();
  }
  return 0;
}

/// Sets up the Bilibin Cave Entrance: the vine Growth has grown, with its cell.
static void BilibinCave_RestoreEntrance(void) {
  if (GameFlag_Get(FLAG_BILIBIN_CAVE_ENTRANCE_SPROUT_GROWN) != 0) {

    MapGrid_CopyCellAttributes(31, 0, 1, 1, 8, 13);
    Import_ObjectEventSetAnimation(LOCALID_SPROUT, 0);
  }
}

/// Sets up Bilibin Cave's MapId_BilibinCaveRapier: its two torches, and its
/// fire, gone once Douse has put it out.
static void BilibinCave_RestoreRapierRoom(void) {

  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);

  Import_ObjectEventSetAnimation(LOCALID_RAPIER_ROOM_WEST_TORCH,
                                 TORCH_ANIMATION_UNLIT);
  Import_ObjectEventSetAnimation(LOCALID_RAPIER_ROOM_EAST_TORCH,
                                 TORCH_ANIMATION_LIT);

  if (GameFlag_Get(FLAG_BILIBIN_CAVE_SECOND_FIRE_DOUSED) != 0) {
    Import_ObjectEventSetPosition(LOCALID_RAPIER_ROOM_FIRE, 0, 0);
  } else {
    ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_RAPIER_ROOM_FIRE),
                               0);
  }
}

/// Clears the drawing flags of an object event used by the cave's animations.
static inline void BilibinCave_ClearObjectEventSpriteFlags(s32 localId) {
  struct ObjectEvent *objectEvent = ObjectEvent_Get(localId);
  ObjectEvent_SetSpriteFlags(objectEvent, 0);
}

/// Sets up Bilibin Cave's MapId_BilibinCaveIce: its torches, puddles and
/// pillars, and the steps of its puzzle already solved.
static void BilibinCave_RestoreIceRoom(void) {
  struct ObjectEvent *objectEvent;
  s32 destinationCellX;

  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  // Set up the decorative animations before restoring interactive object
  // events.
  BilibinCave_ClearObjectEventSpriteFlags(LOCALID_FIRST_ICE_PILLAR);
  BilibinCave_ClearObjectEventSpriteFlags(LOCALID_FIRST_ICE_PILLAR + 1);
  BilibinCave_ClearObjectEventSpriteFlags(LOCALID_FIRST_ICE_PILLAR + 2);
  BilibinCave_ClearObjectEventSpriteFlags(LOCALID_FIRST_ICE_PILLAR + 3);
  BilibinCave_ClearObjectEventSpriteFlags(LOCALID_FIRST_ICE_PILLAR + 4);
  BilibinCave_ClearObjectEventSpriteFlags(LOCALID_FIRST_LOW_ICE);
  BilibinCave_ClearObjectEventSpriteFlags(LOCALID_FIRST_LOW_ICE + 1);
  BilibinCave_ClearObjectEventSpriteFlags(LOCALID_FIRST_LOW_ICE + 2);
  BilibinCave_ClearObjectEventSpriteFlags(LOCALID_FIRST_LOW_ICE + 3);
  Import_ObjectEventSetAnimation(LOCALID_FIRST_ICE_PILLAR,
                                 PUDDLE_ANIMATION_PILLAR);
  Import_ObjectEventSetAnimation(LOCALID_FIRST_ICE_PILLAR + 1,
                                 PUDDLE_ANIMATION_PILLAR);
  Import_ObjectEventSetAnimation(LOCALID_FIRST_ICE_PILLAR + 2,
                                 PUDDLE_ANIMATION_PILLAR);
  Import_ObjectEventSetAnimation(LOCALID_FIRST_ICE_PILLAR + 3,
                                 PUDDLE_ANIMATION_PILLAR);
  Import_ObjectEventSetAnimation(LOCALID_FIRST_ICE_PILLAR + 4,
                                 PUDDLE_ANIMATION_PILLAR);
  Import_ObjectEventSetAnimation(LOCALID_FIRST_LOW_ICE,
                                 PUDDLE_ANIMATION_LOW_ICE);
  Import_ObjectEventSetAnimation(LOCALID_FIRST_LOW_ICE + 1,
                                 PUDDLE_ANIMATION_LOW_ICE);
  Import_ObjectEventSetAnimation(LOCALID_FIRST_LOW_ICE + 2,
                                 PUDDLE_ANIMATION_LOW_ICE);
  Import_ObjectEventSetAnimation(LOCALID_FIRST_LOW_ICE + 3,
                                 PUDDLE_ANIMATION_LOW_ICE);
  Import_ObjectEventSetAnimation(LOCALID_FIRST_TORCH, TORCH_ANIMATION_LIT);
  Import_ObjectEventSetAnimation(LOCALID_FIRST_TORCH + 1, TORCH_ANIMATION_LIT);
  Import_ObjectEventSetAnimation(LOCALID_FIRST_TORCH + 2, TORCH_ANIMATION_LIT);
  Import_ObjectEventSetAnimation(LOCALID_FIRST_TORCH + 3, TORCH_ANIMATION_LIT);
  Import_ObjectEventSetAnimation(LOCALID_FIRST_TORCH + 4, TORCH_ANIMATION_LIT);
  Import_ObjectEventSetAnimation(LOCALID_FIRST_TORCH + 5, TORCH_ANIMATION_LIT);
  BilibinCave_EmptyObjectEventSetupHook(LOCALID_FIRST_ICE_PILLAR);
  BilibinCave_EmptyObjectEventSetupHook(LOCALID_FIRST_ICE_PILLAR + 1);
  BilibinCave_EmptyObjectEventSetupHook(LOCALID_FIRST_ICE_PILLAR + 2);
  BilibinCave_EmptyObjectEventSetupHook(LOCALID_FIRST_ICE_PILLAR + 3);
  BilibinCave_EmptyObjectEventSetupHook(LOCALID_FIRST_ICE_PILLAR + 4);
  BilibinCave_EmptyObjectEventSetupHook(LOCALID_FIRST_LOW_ICE);
  BilibinCave_EmptyObjectEventSetupHook(LOCALID_FIRST_LOW_ICE + 1);
  BilibinCave_EmptyObjectEventSetupHook(LOCALID_FIRST_LOW_ICE + 2);
  BilibinCave_EmptyObjectEventSetupHook(LOCALID_FIRST_LOW_ICE + 3);
  BilibinCave_EmptyObjectEventSetupHook(LOCALID_FIRST_TORCH);
  BilibinCave_EmptyObjectEventSetupHook(LOCALID_FIRST_TORCH + 1);
  BilibinCave_EmptyObjectEventSetupHook(LOCALID_FIRST_TORCH + 2);
  BilibinCave_EmptyObjectEventSetupHook(LOCALID_FIRST_TORCH + 3);
  BilibinCave_EmptyObjectEventSetupHook(LOCALID_FIRST_TORCH + 4);
  BilibinCave_EmptyObjectEventSetupHook(LOCALID_FIRST_TORCH + 5);
  // Restore the surface exposed by Douse, including its metatile behavior and
  // priority.
  if (GameFlag_Get(FLAG_BILIBIN_CAVE_FIRST_FIRE_DOUSED) != 0) {
    ObjectEvent_SetPosition(LOCALID_ICE_ROOM_FIRE, 0, 0);
    Import_ObjectEventSetAnimation(LOCALID_FIRST_PUDDLE,
                                   PUDDLE_ANIMATION_PILLAR);
    ObjectEvent_Get(LOCALID_FIRST_PUDDLE)->motionFlags = 0;
    ObjectEvent_Get(LOCALID_FIRST_PUDDLE)->y.fixed = -PIXELS(4);
    ObjectEvent_Get(LOCALID_FIRST_PUDDLE)->priorityFlags |=
        OBJ_EVENT_PRIORITY_UNDERFOOT;
    ObjectMotion_SetActionVariant(LOCALID_FIRST_PUDDLE, 2);
    MapGrid_CopyCellAttributes(0, 0, 1, 1, 18, 14);
  } else {
    Import_ObjectEventSetAnimation(LOCALID_ICE_ROOM_FIRE, 2);
    objectEvent = ObjectEvent_Get(LOCALID_ICE_ROOM_FIRE);
    ObjectEvent_SetSpriteFlags(objectEvent, 0);
    Import_ObjectEventSetAnimation(LOCALID_FIRST_PUDDLE,
                                   PUDDLE_ANIMATION_PUDDLE);
  }
  // Rebuild the Frost target and return moved object events to their saved
  // cells.
  Import_ObjectEventSetAnimation(LOCALID_SECOND_PUDDLE,
                                 PUDDLE_ANIMATION_PUDDLE);
  if (GameFlag_Get(FLAG_BILIBIN_CAVE_THIRD_PUDDLE_MELTED) != 0) {
    destinationCellX = 36;
    Import_ObjectEventSetAnimation(LOCALID_THIRD_PUDDLE,
                                   PUDDLE_ANIMATION_PUDDLE);
    MapGrid_CopyCellAttributes(0, 1, 1, 1, destinationCellX, 22);
    MapGrid_CopyCellAttributes(0, 2, 1, 1, destinationCellX, 24);
  } else {
    destinationCellX = 36;
    Import_ObjectEventSetAnimation(LOCALID_THIRD_PUDDLE,
                                   PUDDLE_ANIMATION_PILLAR);
    MapGrid_CopyCellAttributes(1, 1, 1, 1, destinationCellX, 22);
    MapGrid_CopyCellAttributes(1, 2, 1, 1, destinationCellX, 24);
  }
  if (GameFlag_Get(FLAG_BILIBIN_CAVE_THIRD_TORCH_BY_PILLAR) != 0) {
    ObjectEvent_SetPosition(LOCALID_THIRD_TORCH, PIXELS(570), PIXELS(376));
  }
  if (GameFlag_Get(FLAG_BILIBIN_CAVE_FOURTH_TORCH_BY_PILLAR) != 0) {
    ObjectEvent_SetPosition(LOCALID_FOURTH_TORCH, PIXELS(570), PIXELS(376));
  }
}

// The shared cell lookup retains its position before the push handler.
#include "common/object_findcell.h"

/// Pushing one of the two movable torches in Bilibin Cave
/// (MapId_BilibinCaveIce): it moves only when its next cell and movement
/// probe are clear.
static void EventScript_PushTorch(void) {
  struct ObjectEvent *leader;
  struct ObjectEvent *target;
  struct ObjectEvent *blocker;
  s32 heading;
  s32 nextCellX;
  s32 nextCellZ;
  struct FieldPosition destination;

  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  heading = leader->facing >> 12;

  nextCellX =
      (leader->x.part.pixel + (gBilibinCave_PushSteps[heading] >> 16)) >> 4;
  nextCellZ =
      (leader->z.part.pixel + (s16)gBilibinCave_PushSteps[heading]) >> 4;
  target = ObjectEvent_FindAtCell(nextCellX, nextCellZ);
  if (target == 0)
    return;

  nextCellX =
      (target->x.part.pixel + (gBilibinCave_PushSteps[heading] >> 16)) >> 4;
  nextCellZ =
      (target->z.part.pixel + (s16)gBilibinCave_PushSteps[heading]) >> 4;
  blocker = ObjectEvent_FindAtCell(nextCellX, nextCellZ);
  if (blocker != 0)
    return;

  // Check the destination before moving both object events into their push
  // states.
  target->layer = 2;

  destination.x =
      target->x.fixed + (gBilibinCave_PushSteps[heading] & 0xffff0000);
  destination.y = target->y.fixed;
  destination.z =
      target->z.fixed + (s16)gBilibinCave_PushSteps[heading] * PIXELS(1);

  if (FieldObject_CheckMovementCollision(target, &destination) > 0)
    return;

  Object_SelectAnimation(leader, 8);
  WaitFrames(15);
  Sound_PlaySong(SE_PUSH);

  target->speed = PIXELS(1) / 5;
  target->acceleration = PIXELS(1) / 5;
  Object_SetPosition(target, destination.x, destination.y, destination.z);

  leader->speed = PIXELS(1) / 5;
  leader->acceleration = PIXELS(1) / 5;
  Object_SetPosition(leader, destination.x, destination.y, destination.z);

  // Wait for the target's movement before snapping it to the destination.
  Object_WaitForMove(target);

  target->x.fixed = destination.x;
  target->z.fixed = destination.z;
  target->velocityX = (s32)blocker;
  target->velocityZ = (s32)blocker;

  // Resume the leader and refresh the mechanism affected by the moved object
  // event.
  Object_SelectAnimation(leader, ANIM_STAND);
  BilibinCave_UpdateThirdPuddleFromTorches();
}

/// The original setup invokes this empty hook for every animated object event.
static void BilibinCave_EmptyObjectEventSetupHook(s32 localId) {}

s32 gBilibinCave_ClearSpriteScript[] = {
    SC_CALL(BilibinCave_ClearScriptedObjectEventSpriteFlags),
    SC_END,
};

struct MapWarpDest gBilibinCaveWarpDestsOther[] = {
    {WARP_DEST_DEFAULT, CONDITION_ALWAYS, 72, 0, 488, FACING_NORTH, 0, -1, -1,
     -1, -1, 0},
    {MAP_TABLE_END},
};

struct MapWarpDest gBilibinCaveEntranceWarpDests[] = {
    {WARP_DEST_DEFAULT, CONDITION_ALWAYS, 372, 0, 272, FACING_NORTH, 0, -1, -1,
     -1, -1, 0},
    {BILIBIN_CAVE_ENTRANCE_FROM_WORLD_MAP, CONDITION_ALWAYS, 99, 0, 447,
     FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {BILIBIN_CAVE_ENTRANCE_FROM_BILIBIN_CAVE_RAPIER_SOUTH, CONDITION_ALWAYS,
     342, 0, 254, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {BILIBIN_CAVE_ENTRANCE_FROM_BILIBIN_CAVE_RAPIER_NORTH, CONDITION_ALWAYS,
     440, 0, 136, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {MAP_TABLE_END},
};

struct MapWarpDest gBilibinCaveRapierWarpDests[] = {
    {WARP_DEST_DEFAULT, CONDITION_ALWAYS, 864, 0, 168, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {BILIBIN_CAVE_RAPIER_FROM_BILIBIN_CAVE_ENTRANCE_WEST, CONDITION_ALWAYS, 312,
     0, 756, FACING_NORTH, 0, 40, 60, 630, 840, 0},
    {BILIBIN_CAVE_RAPIER_FROM_BILIBIN_CAVE_ENTRANCE_EAST, CONDITION_ALWAYS, 472,
     0, 680, FACING_NORTH, 0, 40, 60, 630, 840, 0},
    {BILIBIN_CAVE_RAPIER_FROM_BILIBIN_CAVE_ICE_WEST, CONDITION_ALWAYS, 392, 0,
     200, FACING_SOUTH, 0, 40, 60, 630, 840, 0},
    {BILIBIN_CAVE_RAPIER_FROM_TORCHES, CONDITION_ALWAYS, 536, 0, 426,
     FACING_SOUTH, 0, 40, 60, 630, 840, 0},
    {BILIBIN_CAVE_RAPIER_FROM_BILIBIN_CAVE_ICE_EAST, CONDITION_ALWAYS, 856, 0,
     124, FACING_SOUTH, 0, 760, 65, 925, 330, 0},
    {BILIBIN_CAVE_RAPIER_TORCHES_FROM_BILIBIN_CAVE_RAPIER, CONDITION_ALWAYS,
     808, 0, 248, FACING_NORTH, 0, 760, 65, 925, 330, 0},
    {MAP_TABLE_END},
};

struct MapWarpDest gBilibinCaveIceWarpDests[] = {
    {WARP_DEST_DEFAULT, CONDITION_ALWAYS, 172, 0, 300, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {BILIBIN_CAVE_ICE_FROM_BILIBIN_CAVE_RAPIER_WEST, CONDITION_ALWAYS, 120, 0,
     472, FACING_NORTH, 0, 60, 60, 790, 600, 0},
    {BILIBIN_CAVE_ICE_FROM_BILIBIN_CAVE_RAPIER_EAST, CONDITION_ALWAYS, 312, 0,
     524, FACING_NORTH, 0, 60, 60, 790, 600, 0},
    {BILIBIN_CAVE_ICE_FROM_WORLD_MAP, CONDITION_ALWAYS, 538, 0, 548,
     FACING_NORTH, 0, 60, 60, 790, 600, 0},
    {BILIBIN_CAVE_ICE_FROM_CHESTS, CONDITION_ALWAYS, 344, 0, 176, FACING_SOUTH,
     0, 60, 60, 790, 600, 0},
    {BILIBIN_CAVE_ICE_CHESTS_FROM_BILIBIN_CAVE_ICE, CONDITION_ALWAYS, 856, 0,
     632, FACING_NORTH, 0, 740, 510, 980, 690, 0},
    {MAP_TABLE_END},
};

u32 gBilibinCaveWarps[] = {
    MAP_WARPS(BilibinCaveEntrance),
    MAP_WARP(BILIBIN_CAVE_ENTRANCE_TO_WORLD_MAP, WorldMap,
             WORLD_MAP_FROM_BILIBIN_CAVE_ENTRANCE),
    MAP_WARP(BILIBIN_CAVE_ENTRANCE_TO_BILIBIN_CAVE_RAPIER_SOUTH,
             BilibinCaveRapier,
             BILIBIN_CAVE_RAPIER_FROM_BILIBIN_CAVE_ENTRANCE_WEST),
    MAP_WARP(BILIBIN_CAVE_ENTRANCE_TO_BILIBIN_CAVE_RAPIER_NORTH,
             BilibinCaveRapier,
             BILIBIN_CAVE_RAPIER_FROM_BILIBIN_CAVE_ENTRANCE_EAST),
    MAP_WARPS(BilibinCaveRapier),
    MAP_WARP(BILIBIN_CAVE_RAPIER_TO_BILIBIN_CAVE_ENTRANCE_WEST,
             BilibinCaveEntrance,
             BILIBIN_CAVE_ENTRANCE_FROM_BILIBIN_CAVE_RAPIER_SOUTH),
    MAP_WARP(BILIBIN_CAVE_RAPIER_TO_BILIBIN_CAVE_ENTRANCE_EAST,
             BilibinCaveEntrance,
             BILIBIN_CAVE_ENTRANCE_FROM_BILIBIN_CAVE_RAPIER_NORTH),
    MAP_WARP(BILIBIN_CAVE_RAPIER_TO_BILIBIN_CAVE_ICE_WEST, BilibinCaveIce,
             BILIBIN_CAVE_ICE_FROM_BILIBIN_CAVE_RAPIER_WEST),
    MAP_WARP(BILIBIN_CAVE_RAPIER_TO_TORCHES, BilibinCaveRapier,
             BILIBIN_CAVE_RAPIER_TORCHES_FROM_BILIBIN_CAVE_RAPIER),
    MAP_WARP(BILIBIN_CAVE_RAPIER_TO_BILIBIN_CAVE_ICE_EAST, BilibinCaveIce,
             BILIBIN_CAVE_ICE_FROM_BILIBIN_CAVE_RAPIER_EAST),
    MAP_WARP(BILIBIN_CAVE_RAPIER_TORCHES_TO_BILIBIN_CAVE_RAPIER,
             BilibinCaveRapier, BILIBIN_CAVE_RAPIER_FROM_TORCHES),
    MAP_WARPS(BilibinCaveIce),
    MAP_WARP(BILIBIN_CAVE_ICE_TO_BILIBIN_CAVE_RAPIER_WEST, BilibinCaveRapier,
             BILIBIN_CAVE_RAPIER_FROM_BILIBIN_CAVE_ICE_WEST),
    MAP_WARP(BILIBIN_CAVE_ICE_TO_BILIBIN_CAVE_RAPIER_EAST, BilibinCaveRapier,
             BILIBIN_CAVE_RAPIER_FROM_BILIBIN_CAVE_ICE_EAST),
    MAP_WARP(BILIBIN_CAVE_ICE_TO_WORLD_MAP, WorldMap,
             WORLD_MAP_FROM_BILIBIN_CAVE_ICE),
    MAP_WARP(BILIBIN_CAVE_ICE_TO_CHESTS, BilibinCaveIce,
             BILIBIN_CAVE_ICE_CHESTS_FROM_BILIBIN_CAVE_ICE),
    MAP_WARP(BILIBIN_CAVE_ICE_CHESTS_TO_BILIBIN_CAVE_ICE, BilibinCaveIce,
             BILIBIN_CAVE_ICE_FROM_CHESTS),
    MAP_WARPS_END,
};

struct ObjectEventTemplate gBilibinCaveObjectEventTemplatesOther[] = {
    {MAP_TABLE_END},
};

// clang-format off
struct ObjectEventTemplate gBilibinCaveEntranceObjectEventTemplates[] = {
    { SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(136), 0, PIXELS(208), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gBilibinCaveRapierObjectEventTemplates[] = {
    { SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(840), 0, PIXELS(120), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_FIRE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(856), 0, PIXELS(120), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(872), 0, PIXELS(120), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gBilibinCaveIceObjectEventTemplates[] = {
    { SPRITE_FIRE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(280), 0, PIXELS(232), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(152), 0, PIXELS(264), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(520), 0, PIXELS(280), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(504), 0, PIXELS(360), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(536), 0, PIXELS(360), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(504), 0, PIXELS(520), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(584), 0, PIXELS(520), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, (s32)gBilibinCave_ClearSpriteScript,
        PIXELS(296), 0, PIXELS(230), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, (s32)gBilibinCave_ClearSpriteScript,
        PIXELS(504), 0, PIXELS(280), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, (s32)gBilibinCave_ClearSpriteScript,
        PIXELS(584), 0, PIXELS(376), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(344), 0, PIXELS(224), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(392), 0, PIXELS(280), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(424), 0, PIXELS(296), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(456), 0, PIXELS(264), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(328), 0, PIXELS(288), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(200), 0, PIXELS(264), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(248), 0, PIXELS(264), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(136), 0, PIXELS(296), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(184), 0, PIXELS(312), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { MAP_TABLE_END },
};

struct MapEvent gBilibinCaveEventsOther[] = {
    { MAP_EVENTS_END },
};

struct MapEvent gBilibinCaveEntranceEvents[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, BILIBIN_CAVE_ENTRANCE_TO_WORLD_MAP },
    { MAP_EVENT_WARP, 2, CONDITION_ALWAYS, BILIBIN_CAVE_ENTRANCE_TO_BILIBIN_CAVE_RAPIER_SOUTH },
    { MAP_EVENT_WARP, 3, CONDITION_ALWAYS, BILIBIN_CAVE_ENTRANCE_TO_BILIBIN_CAVE_RAPIER_NORTH },
    { MAP_EVENT_COORD, 10, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WalkOntoLedgeOverCaveMouth) },
    { MAP_EVENT_COORD, 11, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WalkOffLedgeOverCaveMouth) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_GROWTH), LOCALID_SPROUT, FLAG_BILIBIN_CAVE_ENTRANCE_SPROUT_GROWN, EVENT_SCRIPT(EventScript_GrowPlant) },
    { MAP_EVENTS_END },
};

struct MapEvent gBilibinCaveRapierEvents[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, BILIBIN_CAVE_RAPIER_TO_BILIBIN_CAVE_ENTRANCE_WEST },
    { MAP_EVENT_WARP, 2, CONDITION_ALWAYS, BILIBIN_CAVE_RAPIER_TO_BILIBIN_CAVE_ENTRANCE_EAST },
    { MAP_EVENT_WARP, 3, CONDITION_ALWAYS, BILIBIN_CAVE_RAPIER_TO_BILIBIN_CAVE_ICE_WEST },
    { MAP_EVENT_WARP, 4, CONDITION_ALWAYS, BILIBIN_CAVE_RAPIER_TO_TORCHES },
    { MAP_EVENT_WARP, 5, CONDITION_ALWAYS, BILIBIN_CAVE_RAPIER_TO_BILIBIN_CAVE_ICE_EAST },
    { MAP_EVENT_WARP, 6, CONDITION_ALWAYS, BILIBIN_CAVE_RAPIER_TORCHES_TO_BILIBIN_CAVE_RAPIER },
    { BG_EVENT(BG_EVENT_CHEST), 100, 0xf5d, FIND_ITEM(ITEM_ELVEN_RAPIER) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ACTS), 0, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_RevealHiddenPath) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ENDS), 0, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_HideRevealedPath) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_DOUSE), LOCALID_RAPIER_ROOM_FIRE, FLAG_BILIBIN_CAVE_SECOND_FIRE_DOUSED, EVENT_SCRIPT(EventScript_DouseRapierRoomFire) },
    { MAP_EVENTS_END },
};

struct MapEvent gBilibinCaveIceEvents[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, BILIBIN_CAVE_ICE_TO_BILIBIN_CAVE_RAPIER_WEST },
    { MAP_EVENT_WARP, 2, CONDITION_ALWAYS, BILIBIN_CAVE_ICE_TO_BILIBIN_CAVE_RAPIER_EAST },
    { MAP_EVENT_WARP, 3, CONDITION_ALWAYS, BILIBIN_CAVE_ICE_TO_WORLD_MAP },
    { MAP_EVENT_WARP, 4, CONDITION_ALWAYS, BILIBIN_CAVE_ICE_TO_CHESTS },
    { MAP_EVENT_WARP, 5, CONDITION_ALWAYS, BILIBIN_CAVE_ICE_CHESTS_TO_BILIBIN_CAVE_ICE },
    { MAP_EVENT_COORD, 20, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_StartFootprints) },
    { MAP_EVENT_COORD, 21, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_StopFootprints) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_DOUSE), LOCALID_ICE_ROOM_FIRE, FLAG_BILIBIN_CAVE_FIRST_FIRE_DOUSED, EVENT_SCRIPT(EventScript_DouseIceRoomFire) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_FROST), LOCALID_FIRST_PUDDLE, FLAG_BILIBIN_CAVE_FIRST_FIRE_DOUSED, EVENT_SCRIPT(EventScript_FreezeFirstPuddle) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_FROST), LOCALID_SECOND_PUDDLE, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_FreezeSecondPuddle) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_FROST), LOCALID_THIRD_PUDDLE, IF_FLAG_SET(FLAG_BILIBIN_CAVE_THIRD_PUDDLE_MELTED), EVENT_SCRIPT(EventScript_FreezeThirdPuddle) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_THIRD_TORCH, CONDITION_ALWAYS, EVENT_SCRIPT(BilibinCave_UpdateThirdPuddleFromTorches) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_FOURTH_TORCH, CONDITION_ALWAYS, EVENT_SCRIPT(BilibinCave_UpdateThirdPuddleFromTorches) },
    { COORD_EVENT_WALKING, 10, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_PushTorch) },
    { COORD_EVENT_WALKING_INTO(FACING_WEST), 11, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_PushTorch) },
    { BG_EVENT(BG_EVENT_CHEST), 100, 0xf5e, FIND_ITEM(ITEM_VIAL) },
    { BG_EVENT(BG_EVENT_CHEST), 101, 0xfc6, FIND_ITEM(ITEM_PSY_CRYSTAL) },
    { BG_EVENT(BG_EVENT_CHEST), 102, 0xfc7, FIND_ITEM(ITEM_WATER_OF_LIFE) },
    { MAP_EVENTS_END },
};
// clang-format on

/// Packs signed pixel steps with X in the upper halfword and Z in the lower.
#define DIRECTION_STEP(dx, dz) (((u32)(dx) << 16) | ((dz) & 0xffff))

s32 gBilibinCave_PushSteps[] = {
    DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(16, 0),
};

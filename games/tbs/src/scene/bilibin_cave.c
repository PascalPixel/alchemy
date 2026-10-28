/// @file
/// The Bilibin Cave Entrance and Bilibin Cave, two areas between Bilibin
/// and Imil that share this overlay: the Bilibin Cave Entrance is
/// SceneId_BilibinCaveEntrance, and Bilibin Cave is SceneId_BilibinCaveRapier
/// and, beyond it, SceneId_BilibinCaveIce, whose far end opens onto the World
/// Map farther north. No one speaks here: each scene is a run of Psynergy
/// puzzles, the getters pick its tables by gAdventure.sceneId, and
/// BilibinCave_Initialize restores what the party has already solved in the
/// scene being entered.
///
/// At the Bilibin Cave Entrance, entered from the World Map, Growth on a sprout
/// (ACTOR_SPROUT) grows it into a vine and changes the cell behavior of
/// its cell (BilibinCave_GrowPlant); FLAG_BILIBIN_CAVE_ENTRANCE_SPROUT_GROWN
/// keeps it grown until the party leaves the Bilibin Cave Entrance, into the
/// cave beyond or out to the World Map, since both are other areas. Two
/// exits lead on into Bilibin Cave. The south one's exit cell, at the cave
/// mouth, lies under the ledge path that runs above the mouth, since the map
/// grid has one cell for both: step trigger 10, the ledge cells either side of
/// it, turns the exit off so the leader can walk the ledge across it, and
/// step trigger 11, a step further out, turns it back on
/// (BilibinCave_WalkOntoLedgeOverCaveMouth,
/// BilibinCave_WalkOffLedgeOverCaveMouth; its recording,
/// places/bilibin-cave-entrance/events/BilibinCave_WalkOntoLedgeOverCaveMouth.gif).
///
/// In SceneId_BilibinCaveRapier a fire burns between two torches, the west one
/// cold and the east one lit (ACTOR_RAPIER_ROOM_FIRE), and Douse puts it out
/// for good (FLAG_BILIBIN_CAVE_SECOND_FIRE_DOUSED). Reveal shows a hidden path,
/// a strip of tiles and the cells under them, which
/// BilibinCave_HideRevealedPath takes away again when Reveal wears off. A chest
/// holds the Elven Rapier.
///
/// SceneId_BilibinCaveIce holds the cave's one puzzle of several steps, built
/// from fires, torches and puddles that Frost freezes into ice pillars and the
/// heat of a fire or a torch melts back (PUDDLE_ANIMATION_MELT). While
/// ACTOR_ICE_ROOM_FIRE burns, Frost on the puddle beside it only melts back;
/// Douse puts the fire out (FLAG_BILIBIN_CAVE_FIRST_FIRE_DOUSED), and the
/// puddle freezes into a pillar drawn underfoot, which the party walks over.
/// The second puddle stands beside a torch that cannot be moved, and Frost on
/// it always melts back. The third is an ice pillar: two of the six torches
/// (ACTOR_THIRD_TORCH and ACTOR_FOURTH_TORCH) can be pushed or moved with
/// Move, and while either stands beside the pillar, on cell (35, 23), the
/// pillar melts into a puddle and the cells north and south of it change with
/// it; when both leave it freezes again
/// (BilibinCave_UpdateThirdPuddleFromTorches). Melting turns the cell
/// behaviors of cells (36, 22) and (36, 24), north and south of the puddle,
/// from CELL_BEHAVIOR_ENCOUNTER_GROUND_1 to CELL_BEHAVIOR_JUMP_FORWARD:
/// encounter ground becomes a jump across.
/// FLAG_BILIBIN_CAVE_THIRD_TORCH_BY_PILLAR and
/// FLAG_BILIBIN_CAVE_FOURTH_TORCH_BY_PILLAR remember which torch stands there
/// and FLAG_BILIBIN_CAVE_THIRD_PUDDLE_MELTED that the pillar has melted, so
/// coming back from SceneId_BilibinCaveRapier puts the torches and the pillar
/// back; leaving Bilibin Cave resets them. Nine more pieces of ice only stand
/// there: five pillars from ACTOR_FIRST_ICE_PILLAR and four lower pieces from
/// ACTOR_FIRST_LOW_ICE. Step trigger 20 covers the snow blown in at the cave's
/// mouth to the World Map, around BILIBIN_CAVE_ICE_FROM_WORLD_MAP, and makes
/// the leader leave footprints; step trigger 21, rimming it, stops them
/// (BilibinCave_StartFootprints, BilibinCave_StopFootprints; the snow is in
/// places/bilibin-cave-ice/map-1.png). Its chests hold a Vial, a Psy Crystal
/// and Water of Life.

#include "script_operands.h"
#include "adventure.h"
#include "system_call.h"
#include "scene_ids.h"
#include "unit/import.h"
#include "unit/scene/bilibin_cave.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/event_commands.h"
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
#include "event.h"
#include "scene_tables.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "actor_script.h"
#include "actor.h"
#include "flags.h"
#include "songs.h"
#include "map_grid_scroll.h"

/// In Bilibin Cave's ice room, a movable torch has melted the third puddle's
/// ice pillar.
/// ★ Named from what the player sees: a tall pillar of ice beside the lit
/// torches in the cave's east passage (its render)
#define FLAG_BILIBIN_CAVE_THIRD_PUDDLE_MELTED FLAG_AREA_TEMP_02
/// In Bilibin Cave's ice room, the third torch stands beside the third puddle's
/// ice pillar.
/// ★ Named from what the player sees: golden torches burning on their stands
/// near a pillar of ice (its render)
#define FLAG_BILIBIN_CAVE_THIRD_TORCH_BY_PILLAR FLAG_AREA_TEMP_03
/// In Bilibin Cave's ice room, the fourth torch stands beside the third
/// puddle's ice pillar.
/// ★ Named from what the player sees: golden torches burning on their stands
/// near a pillar of ice (its render)
#define FLAG_BILIBIN_CAVE_FOURTH_TORCH_BY_PILLAR FLAG_AREA_TEMP_04
/// At the Bilibin Cave Entrance, Growth has grown the sprout into a vine.
#define FLAG_BILIBIN_CAVE_ENTRANCE_SPROUT_GROWN FLAG_AREA_TEMP_05

// BilibinCave_Initialize calls one of these for the scene being entered.
static void BilibinCave_RestoreEntrance(void);
static void BilibinCave_RestoreRapierRoom(void);
static void BilibinCave_RestoreIceRoom(void);
static void BilibinCave_EmptyActorSetupHook(s32 actorId);

/// Actors of the Bilibin Cave Entrance (SceneId_BilibinCaveEntrance).
enum {
  /// The sprout Growth grows into a vine.
  /// ★ Named from what the player sees: a green sprout, SPRITE_SPROUT (its
  /// render)
  ACTOR_SPROUT = 8
};

/// Actors of SceneId_BilibinCaveRapier, numbered by the rows of
/// gBilibinCaveRapierActorPlacements.
enum {
  /// A torch west of the fire, cold (TORCH_ANIMATION_UNLIT) where its twin
  /// burns.
  /// ★ Named from what the player sees: a golden torch on its stand, its bowl
  /// grey with ash, places/bilibin-cave-rapier/map-5.png (its render)
  ACTOR_RAPIER_ROOM_WEST_TORCH = 8,
  /// A fire between two torches, which Douse puts out.
  /// ★ Named from what the player sees: a flickering flame, SPRITE_FIRE (its
  /// render)
  ACTOR_RAPIER_ROOM_FIRE = 9,
  /// A torch east of the fire.
  /// ★ Named from what the player sees: a golden torch burning on its stand
  /// (its render)
  ACTOR_RAPIER_ROOM_EAST_TORCH = 10
};

/// Actors of SceneId_BilibinCaveIce, numbered by the rows of
/// gBilibinCaveIceActorPlacements.
enum {
  /// A fire beside the first puddle, which Douse puts out.
  /// ★ Named from what the player sees: a flickering flame, SPRITE_FIRE (its
  /// render)
  ACTOR_ICE_ROOM_FIRE = 8,
  /// Six torches, 9 to 14; the second stands beside ACTOR_SECOND_PUDDLE.
  /// ★ Named from what the player sees: a golden torch burning on its stand
  /// (its render)
  ACTOR_FIRST_TORCH = 9,
  /// The third torch, moved by Move or pushed.
  /// ★ Named from what the player sees: a golden torch burning on its stand
  /// (its render)
  ACTOR_THIRD_TORCH = 11,
  /// The fourth torch, moved by Move or pushed.
  /// ★ Named from what the player sees: a golden torch burning on its stand
  /// (its render)
  ACTOR_FOURTH_TORCH = 12,
  /// A puddle beside ACTOR_ICE_ROOM_FIRE, which freezes into a pillar the
  /// party walks over once the fire is out.
  /// ★ Named from what the player sees: a puddle of water, SPRITE_PUDDLE (its
  /// render)
  ACTOR_FIRST_PUDDLE = 15,
  /// A puddle beside a torch that stays put, so it never stays frozen.
  /// ★ Named from what the player sees: a puddle of water, SPRITE_PUDDLE (its
  /// render)
  ACTOR_SECOND_PUDDLE = 16,
  /// A puddle frozen into an ice pillar, melted while a torch stands on cell
  /// (35, 23).
  /// ★ Named from what the player sees: a puddle of water, SPRITE_PUDDLE (its
  /// render)
  ACTOR_THIRD_PUDDLE = 17,
  /// Five ice pillars, 18 to 22 (PUDDLE_ANIMATION_PILLAR); nothing acts on
  /// them.
  /// ★ Named from what the player sees: tall pillars of pale blue ice (its
  /// render)
  ACTOR_FIRST_ICE_PILLAR = 18,
  /// Four pieces of ice, 23 to 26, held at the second step of freezing
  /// (PUDDLE_ANIMATION_LOW_ICE); nothing acts on them.
  /// ★ Named from what the player sees: low, jagged lumps of pale blue ice
  /// (its render)
  ACTOR_FIRST_LOW_ICE = 23
};

/// Clears the scripted actor's sprite flags and completes this script
/// command.
static s32
BilibinCave_ClearScriptedActorSpriteFlags(struct Actor *interpreter) {
  struct Actor *actor = interpreter;
  Actor_SetSpriteFlags(actor, 0);
  return 0;
}

const struct SceneEntrance *BilibinCave_GetEntrances(void) {
  s16 sceneId = gAdventure.sceneId;

  if (sceneId == (s32)&SceneId_BilibinCaveEntrance) {
    return gBilibinCaveEntranceEntrances;
  }
  if (sceneId == (s32)&SceneId_BilibinCaveRapier) {
    return gBilibinCaveRapierEntrances;
  }
  if (sceneId == (s32)&SceneId_BilibinCaveIce) {
    return gBilibinCaveIceEntrances;
  }
  return gBilibinCaveEntrancesOther;
}

const struct SceneExitRegion *BilibinCave_GetRegions(void) { return NULL; }

const u32 *BilibinCave_GetExits(void) { return gBilibinCaveExits; }

const struct ActorPlacement *BilibinCave_GetActorPlacements(void) {
  s16 sceneId = gAdventure.sceneId;

  if (sceneId == (s32)&SceneId_BilibinCaveEntrance) {
    return gBilibinCaveEntranceActorPlacements;
  }
  if (sceneId == (s32)&SceneId_BilibinCaveRapier) {
    return gBilibinCaveRapierActorPlacements;
  }
  if (sceneId == (s32)&SceneId_BilibinCaveIce) {
    return gBilibinCaveIceActorPlacements;
  }
  return gBilibinCaveActorPlacementsOther;
}

const struct Trigger *BilibinCave_GetTriggers(void) {
  s16 sceneId = gAdventure.sceneId;

  if (sceneId == (s32)&SceneId_BilibinCaveEntrance) {
    return gBilibinCaveEntranceTriggers;
  }
  if (sceneId == (s32)&SceneId_BilibinCaveRapier) {
    return gBilibinCaveRapierTriggers;
  }
  if (sceneId == (s32)&SceneId_BilibinCaveIce) {
    return gBilibinCaveIceTriggers;
  }
  return gBilibinCaveTriggersOther;
}

/// Stepping on step trigger 10 at the Bilibin Cave Entrance, on either side of
/// the way into Bilibin Cave: the way's cell (21, 14), the exit cell of
/// BILIBIN_CAVE_ENTRANCE_TO_BILIBIN_CAVE_RAPIER_SOUTH, takes the
/// cell behavior of source cell (1, 0), which has none.
static void BilibinCave_WalkOntoLedgeOverCaveMouth(void) {
  MapGrid_CopyCellAttributes(1, 0, 1, 1, 21, 14);
}

/// Stepping on step trigger 11 at the Bilibin Cave Entrance, a cell further
/// out: the way's cell (21, 14) takes back the exit, from source cell (0, 0).
static void BilibinCave_WalkOffLedgeOverCaveMouth(void) {
  MapGrid_CopyCellAttributes(0, 0, 1, 1, 21, 14);
}

/// Reveal in Bilibin Cave (SceneId_BilibinCaveRapier) shows the hidden path: a
/// strip of tiles and two rows of the cells under it.
static void BilibinCave_RevealHiddenPath(void) {
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

/// As Reveal wears off in Bilibin Cave (SceneId_BilibinCaveRapier), the path is
/// hidden again: the strip's tiles and one row of its cells are restored.
static void BilibinCave_HideRevealedPath(void) {
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

/// Douse on the fire in Bilibin Cave (SceneId_BilibinCaveRapier), until it is
/// out: it goes out for good.
static void BilibinCave_DouseRapierRoomFire(void) {
  Event_Start();
  Import_ActorSetPosition(ACTOR_RAPIER_ROOM_FIRE, 0, 0);
  GameFlag_Set(FLAG_BILIBIN_CAVE_SECOND_FIRE_DOUSED);
  Event_End();
}

/// Douse on the fire in Bilibin Cave (SceneId_BilibinCaveIce), until it is out:
/// it goes out, and the puddle beside it freezes into a pillar the party can
/// walk over.
static void BilibinCave_DouseIceRoomFire(void) {
  Event_Start();
  Actor_SetPosition(ACTOR_ICE_ROOM_FIRE, 0, 0);
  GameFlag_Set(FLAG_BILIBIN_CAVE_FIRST_FIRE_DOUSED);
  Event_WaitFrames(40);
  // Match the replacement's drawing priority to its cell behavior.
  Actor_SetAnimationAndWait(ACTOR_FIRST_PUDDLE, PUDDLE_ANIMATION_FREEZE);
  Actor_Get(ACTOR_FIRST_PUDDLE)->motionFlags = 0;
  Actor_Get(ACTOR_FIRST_PUDDLE)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  Actor_SetSpritePriority(ACTOR_FIRST_PUDDLE, 2);
  MapGrid_CopyCellAttributes(0, 0, 1, 1, 18, 14);
  Event_End();
}

/// Frost on a puddle in Bilibin Cave (SceneId_BilibinCaveIce) that heat keeps
/// from freezing: clears the Psynergy's child effect and plays the ice melting
/// back into a puddle.
static inline void BilibinCave_MeltFrostBack(s32 actorId) {
  Event_Start();
  Actor_ConfigureSpriteColor(actorId, SPRITE_COLOR_NORMAL);
  Event_WaitFrames(40);
  Sound_PlaySong(SE_HIDDEN_PASSAGE_OPEN);
  Actor_SetAnimationAndWait(actorId, PUDDLE_ANIMATION_MELT);
  Event_End();
}

/// Frost on the first puddle in Bilibin Cave (SceneId_BilibinCaveIce), while
/// the fire beside it burns.
static void BilibinCave_FreezeFirstPuddle(void) {
  BilibinCave_MeltFrostBack(ACTOR_FIRST_PUDDLE);
}

/// Frost on the second puddle in Bilibin Cave (SceneId_BilibinCaveIce), beside
/// a torch that stays put.
static void BilibinCave_FreezeSecondPuddle(void) {
  BilibinCave_MeltFrostBack(ACTOR_SECOND_PUDDLE);
}

/// Frost on the third puddle in Bilibin Cave (SceneId_BilibinCaveIce), while a
/// torch beside it keeps it melted (FLAG_BILIBIN_CAVE_THIRD_PUDDLE_MELTED).
static void BilibinCave_FreezeThirdPuddle(void) {
  BilibinCave_MeltFrostBack(ACTOR_THIRD_PUDDLE);
}

/// A movable torch melts the ice pillar beside it. This runs after a push or
/// Move in Bilibin Cave (SceneId_BilibinCaveIce): while either movable torch
/// stands on cell (35, 23) (FLAG_BILIBIN_CAVE_THIRD_TORCH_BY_PILLAR and
/// FLAG_BILIBIN_CAVE_FOURTH_TORCH_BY_PILLAR), the ice pillar beside it melts
/// with a sound, and FLAG_BILIBIN_CAVE_THIRD_PUDDLE_MELTED says so; when both
/// leave, it freezes again.
static void BilibinCave_UpdateThirdPuddleFromTorches(void) {
  struct Actor *firstActor;
  struct Actor *secondActor;
  s32 destinationCellX;

  // Track each movable actor independently before changing the Frost
  // target.
  firstActor = Actor_GetById(ACTOR_THIRD_TORCH);
  secondActor = Actor_Get(ACTOR_FOURTH_TORCH);
  if ((firstActor->x.fixed >> 20) == 35 && (firstActor->z.fixed >> 20) == 23) {
    GameFlag_Set(FLAG_BILIBIN_CAVE_THIRD_TORCH_BY_PILLAR);
  } else {
    GameFlag_Clear(FLAG_BILIBIN_CAVE_THIRD_TORCH_BY_PILLAR);
  }
  if ((secondActor->x.fixed >> 20) == 35 &&
      (secondActor->z.fixed >> 20) == 23) {
    GameFlag_Set(FLAG_BILIBIN_CAVE_FOURTH_TORCH_BY_PILLAR);
  } else {
    GameFlag_Clear(FLAG_BILIBIN_CAVE_FOURTH_TORCH_BY_PILLAR);
  }
  if (GameFlag_Get(FLAG_BILIBIN_CAVE_THIRD_TORCH_BY_PILLAR) != 0 ||
      GameFlag_Get(FLAG_BILIBIN_CAVE_FOURTH_TORCH_BY_PILLAR) != 0) {
    if (Event_GetFlag(FLAG_BILIBIN_CAVE_THIRD_PUDDLE_MELTED) == 0) {
      Event_Start();
      Event_WaitFrames(40);
      Event_PlaySong(SE_HIDDEN_PASSAGE_OPEN);
      destinationCellX = 36;
      Actor_SetAnimationAndWait(ACTOR_THIRD_PUDDLE, PUDDLE_ANIMATION_MELT);
      MapGrid_CopyCellAttributes(0, 1, 1, 1, destinationCellX, 22);
      MapGrid_CopyCellAttributes(0, 2, 1, 1, destinationCellX, 24);
      Event_End();
    }
    GameFlag_Set(FLAG_BILIBIN_CAVE_THIRD_PUDDLE_MELTED);
  } else {
    if (Event_GetFlag(FLAG_BILIBIN_CAVE_THIRD_PUDDLE_MELTED) != 0) {
      Event_Start();
      Event_WaitFrames(40);
      Event_PlaySong(SE_SHIMMER);
      destinationCellX = 36;
      Actor_SetAnimationAndWait(ACTOR_THIRD_PUDDLE, PUDDLE_ANIMATION_FREEZE);
      MapGrid_CopyCellAttributes(1, 1, 1, 1, destinationCellX, 22);
      MapGrid_CopyCellAttributes(1, 2, 1, 1, destinationCellX, 24);
      Event_End();
    }
    GameFlag_Clear(FLAG_BILIBIN_CAVE_THIRD_PUDDLE_MELTED);
  }
}

/// Growth on the sprout at the Bilibin Cave Entrance, until it has grown: it
/// grows into a vine and its cell's cell behavior changes;
/// FLAG_BILIBIN_CAVE_ENTRANCE_SPROUT_GROWN keeps it grown.
static void BilibinCave_GrowPlant(void) {

  MapGrid_CopyCellAttributes(31, 0, 1, 1, 8, 13);
  GameFlag_Set(FLAG_BILIBIN_CAVE_ENTRANCE_SPROUT_GROWN);
}

/// Stepping on step trigger 20 in Bilibin Cave (SceneId_BilibinCaveIce): from
/// here on the leader leaves footprints.
static void BilibinCave_StartFootprints(void) {
  struct MapGridScrollWork *grid = gMapGridWork[0];
  grid->footprints = 1;
}

/// Stepping on step trigger 21 in Bilibin Cave (SceneId_BilibinCaveIce): the
/// leader leaves no more footprints.
static void BilibinCave_StopFootprints(void) {
  struct MapGridScrollWork *grid = gMapGridWork[0];
  grid->footprints = 0;
}

/// Sets up the scene being entered, with its puzzles already solved. The scene
/// is the Bilibin Cave Entrance or one of Bilibin Cave's two.
s32 BilibinCave_Initialize(void) {
  s16 variant = gAdventure.sceneId;

  if (variant == (s32)&SceneId_BilibinCaveEntrance) {
    BilibinCave_RestoreEntrance();
  } else if (variant == (s32)&SceneId_BilibinCaveRapier) {
    BilibinCave_RestoreRapierRoom();
  } else if (variant == (s32)&SceneId_BilibinCaveIce) {
    BilibinCave_RestoreIceRoom();
  }
  return 0;
}

/// Sets up the Bilibin Cave Entrance: the vine Growth has grown, with its cell.
static void BilibinCave_RestoreEntrance(void) {
  if (GameFlag_Get(FLAG_BILIBIN_CAVE_ENTRANCE_SPROUT_GROWN) != 0) {

    MapGrid_CopyCellAttributes(31, 0, 1, 1, 8, 13);
    Import_ActorSetAnimation(ACTOR_SPROUT, 0);
  }
}

/// Sets up Bilibin Cave's SceneId_BilibinCaveRapier: its two torches, and its
/// fire, gone once Douse has put it out.
static void BilibinCave_RestoreRapierRoom(void) {

  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);

  Import_ActorSetAnimation(ACTOR_RAPIER_ROOM_WEST_TORCH, TORCH_ANIMATION_UNLIT);
  Import_ActorSetAnimation(ACTOR_RAPIER_ROOM_EAST_TORCH, TORCH_ANIMATION_LIT);

  if (GameFlag_Get(FLAG_BILIBIN_CAVE_SECOND_FIRE_DOUSED) != 0) {
    Import_ActorSetPosition(ACTOR_RAPIER_ROOM_FIRE, 0, 0);
  } else {
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_RAPIER_ROOM_FIRE), 0);
  }
}

/// Clears the drawing flags of an actor used by the cave's animations.
static inline void BilibinCave_ClearActorSpriteFlags(s32 actorId) {
  struct Actor *actor = Actor_Get(actorId);
  Actor_SetSpriteFlags(actor, 0);
}

/// Sets up Bilibin Cave's SceneId_BilibinCaveIce: its torches, puddles and
/// pillars, and the steps of its puzzle already solved.
static void BilibinCave_RestoreIceRoom(void) {
  struct Actor *actor;
  s32 destinationCellX;

  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  // Set up the decorative animations before restoring interactive
  // actors.
  BilibinCave_ClearActorSpriteFlags(ACTOR_FIRST_ICE_PILLAR);
  BilibinCave_ClearActorSpriteFlags(ACTOR_FIRST_ICE_PILLAR + 1);
  BilibinCave_ClearActorSpriteFlags(ACTOR_FIRST_ICE_PILLAR + 2);
  BilibinCave_ClearActorSpriteFlags(ACTOR_FIRST_ICE_PILLAR + 3);
  BilibinCave_ClearActorSpriteFlags(ACTOR_FIRST_ICE_PILLAR + 4);
  BilibinCave_ClearActorSpriteFlags(ACTOR_FIRST_LOW_ICE);
  BilibinCave_ClearActorSpriteFlags(ACTOR_FIRST_LOW_ICE + 1);
  BilibinCave_ClearActorSpriteFlags(ACTOR_FIRST_LOW_ICE + 2);
  BilibinCave_ClearActorSpriteFlags(ACTOR_FIRST_LOW_ICE + 3);
  Import_ActorSetAnimation(ACTOR_FIRST_ICE_PILLAR, PUDDLE_ANIMATION_PILLAR);
  Import_ActorSetAnimation(ACTOR_FIRST_ICE_PILLAR + 1, PUDDLE_ANIMATION_PILLAR);
  Import_ActorSetAnimation(ACTOR_FIRST_ICE_PILLAR + 2, PUDDLE_ANIMATION_PILLAR);
  Import_ActorSetAnimation(ACTOR_FIRST_ICE_PILLAR + 3, PUDDLE_ANIMATION_PILLAR);
  Import_ActorSetAnimation(ACTOR_FIRST_ICE_PILLAR + 4, PUDDLE_ANIMATION_PILLAR);
  Import_ActorSetAnimation(ACTOR_FIRST_LOW_ICE, PUDDLE_ANIMATION_LOW_ICE);
  Import_ActorSetAnimation(ACTOR_FIRST_LOW_ICE + 1, PUDDLE_ANIMATION_LOW_ICE);
  Import_ActorSetAnimation(ACTOR_FIRST_LOW_ICE + 2, PUDDLE_ANIMATION_LOW_ICE);
  Import_ActorSetAnimation(ACTOR_FIRST_LOW_ICE + 3, PUDDLE_ANIMATION_LOW_ICE);
  Import_ActorSetAnimation(ACTOR_FIRST_TORCH, TORCH_ANIMATION_LIT);
  Import_ActorSetAnimation(ACTOR_FIRST_TORCH + 1, TORCH_ANIMATION_LIT);
  Import_ActorSetAnimation(ACTOR_FIRST_TORCH + 2, TORCH_ANIMATION_LIT);
  Import_ActorSetAnimation(ACTOR_FIRST_TORCH + 3, TORCH_ANIMATION_LIT);
  Import_ActorSetAnimation(ACTOR_FIRST_TORCH + 4, TORCH_ANIMATION_LIT);
  Import_ActorSetAnimation(ACTOR_FIRST_TORCH + 5, TORCH_ANIMATION_LIT);
  BilibinCave_EmptyActorSetupHook(ACTOR_FIRST_ICE_PILLAR);
  BilibinCave_EmptyActorSetupHook(ACTOR_FIRST_ICE_PILLAR + 1);
  BilibinCave_EmptyActorSetupHook(ACTOR_FIRST_ICE_PILLAR + 2);
  BilibinCave_EmptyActorSetupHook(ACTOR_FIRST_ICE_PILLAR + 3);
  BilibinCave_EmptyActorSetupHook(ACTOR_FIRST_ICE_PILLAR + 4);
  BilibinCave_EmptyActorSetupHook(ACTOR_FIRST_LOW_ICE);
  BilibinCave_EmptyActorSetupHook(ACTOR_FIRST_LOW_ICE + 1);
  BilibinCave_EmptyActorSetupHook(ACTOR_FIRST_LOW_ICE + 2);
  BilibinCave_EmptyActorSetupHook(ACTOR_FIRST_LOW_ICE + 3);
  BilibinCave_EmptyActorSetupHook(ACTOR_FIRST_TORCH);
  BilibinCave_EmptyActorSetupHook(ACTOR_FIRST_TORCH + 1);
  BilibinCave_EmptyActorSetupHook(ACTOR_FIRST_TORCH + 2);
  BilibinCave_EmptyActorSetupHook(ACTOR_FIRST_TORCH + 3);
  BilibinCave_EmptyActorSetupHook(ACTOR_FIRST_TORCH + 4);
  BilibinCave_EmptyActorSetupHook(ACTOR_FIRST_TORCH + 5);
  // Restore the surface exposed by Douse, including its cell behavior and
  // priority.
  if (GameFlag_Get(FLAG_BILIBIN_CAVE_FIRST_FIRE_DOUSED) != 0) {
    Actor_SetPosition(ACTOR_ICE_ROOM_FIRE, 0, 0);
    Import_ActorSetAnimation(ACTOR_FIRST_PUDDLE, PUDDLE_ANIMATION_PILLAR);
    Actor_Get(ACTOR_FIRST_PUDDLE)->motionFlags = 0;
    Actor_Get(ACTOR_FIRST_PUDDLE)->y.fixed = -PIXELS(4);
    Actor_Get(ACTOR_FIRST_PUDDLE)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    Actor_SetSpritePriority(ACTOR_FIRST_PUDDLE, 2);
    MapGrid_CopyCellAttributes(0, 0, 1, 1, 18, 14);
  } else {
    Import_ActorSetAnimation(ACTOR_ICE_ROOM_FIRE, 2);
    actor = Actor_Get(ACTOR_ICE_ROOM_FIRE);
    Actor_SetSpriteFlags(actor, 0);
    Import_ActorSetAnimation(ACTOR_FIRST_PUDDLE, PUDDLE_ANIMATION_PUDDLE);
  }
  // Rebuild the Frost target and return moved actors to their saved
  // cells.
  Import_ActorSetAnimation(ACTOR_SECOND_PUDDLE, PUDDLE_ANIMATION_PUDDLE);
  if (GameFlag_Get(FLAG_BILIBIN_CAVE_THIRD_PUDDLE_MELTED) != 0) {
    destinationCellX = 36;
    Import_ActorSetAnimation(ACTOR_THIRD_PUDDLE, PUDDLE_ANIMATION_PUDDLE);
    MapGrid_CopyCellAttributes(0, 1, 1, 1, destinationCellX, 22);
    MapGrid_CopyCellAttributes(0, 2, 1, 1, destinationCellX, 24);
  } else {
    destinationCellX = 36;
    Import_ActorSetAnimation(ACTOR_THIRD_PUDDLE, PUDDLE_ANIMATION_PILLAR);
    MapGrid_CopyCellAttributes(1, 1, 1, 1, destinationCellX, 22);
    MapGrid_CopyCellAttributes(1, 2, 1, 1, destinationCellX, 24);
  }
  if (GameFlag_Get(FLAG_BILIBIN_CAVE_THIRD_TORCH_BY_PILLAR) != 0) {
    Actor_SetPosition(ACTOR_THIRD_TORCH, PIXELS(570), PIXELS(376));
  }
  if (GameFlag_Get(FLAG_BILIBIN_CAVE_FOURTH_TORCH_BY_PILLAR) != 0) {
    Actor_SetPosition(ACTOR_FOURTH_TORCH, PIXELS(570), PIXELS(376));
  }
}

// The shared cell lookup retains its position before the push handler.
#include "shared/actor_findcell.h"

/// Pushing one of the two movable torches in Bilibin Cave
/// (SceneId_BilibinCaveIce): it moves only when its next cell and movement
/// probe are clear.
static void BilibinCave_PushTorch(void) {
  struct Actor *leader;
  struct Actor *target;
  struct Actor *blocker;
  s32 heading;
  s32 nextCellX;
  s32 nextCellZ;
  struct FieldPosition destination;

  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  heading = leader->facing >> 12;

  nextCellX =
      (leader->x.part.pixel + (gBilibinCave_PushSteps[heading] >> 16)) >> 4;
  nextCellZ =
      (leader->z.part.pixel + (s16)gBilibinCave_PushSteps[heading]) >> 4;
  target = Actor_FindAtCell(nextCellX, nextCellZ);
  if (target == 0)
    return;

  nextCellX =
      (target->x.part.pixel + (gBilibinCave_PushSteps[heading] >> 16)) >> 4;
  nextCellZ =
      (target->z.part.pixel + (s16)gBilibinCave_PushSteps[heading]) >> 4;
  blocker = Actor_FindAtCell(nextCellX, nextCellZ);
  if (blocker != 0)
    return;

  // Check the destination before moving both actors into their push
  // states.
  target->layer = 2;

  destination.x =
      target->x.fixed + (gBilibinCave_PushSteps[heading] & 0xffff0000);
  destination.y = target->y.fixed;
  destination.z =
      target->z.fixed + (s16)gBilibinCave_PushSteps[heading] * PIXELS(1);

  if (FieldActor_CheckMovementCollision(target, &destination) > 0)
    return;

  Actor_SelectAnimation(leader, 8);
  WaitFrames(15);
  Sound_PlaySong(SE_PUSH);

  target->speedLimit = PIXELS(1) / 5;
  target->acceleration = PIXELS(1) / 5;
  FieldActor_SetMoveTarget(target, destination.x, destination.y, destination.z);

  leader->speedLimit = PIXELS(1) / 5;
  leader->acceleration = PIXELS(1) / 5;
  FieldActor_SetMoveTarget(leader, destination.x, destination.y, destination.z);

  // Wait for the target's movement before snapping it to the destination.
  Actor_WaitForMove(target);

  target->x.fixed = destination.x;
  target->z.fixed = destination.z;
  target->velocityX = (s32)blocker;
  target->velocityZ = (s32)blocker;

  // Resume the leader and refresh the mechanism affected by the moved
  // actor.
  Actor_SelectAnimation(leader, ANIM_STAND);
  BilibinCave_UpdateThirdPuddleFromTorches();
}

/// The original setup invokes this empty hook for every animated actor.
static void BilibinCave_EmptyActorSetupHook(s32 actorId) {}

s32 gBilibinCave_ClearSpriteScript[] = {
    SC_CALL(BilibinCave_ClearScriptedActorSpriteFlags),
    SC_END,
};

struct SceneEntrance gBilibinCaveEntrancesOther[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 72, 0, 488, FACING_NORTH, 0, -1, -1,
     -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gBilibinCaveEntranceEntrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 372, 0, 272, FACING_NORTH, 0, -1, -1,
     -1, -1, 0},
    {BILIBIN_CAVE_ENTRANCE_FROM_WORLD_MAP, CONDITION_ALWAYS, 99, 0, 447,
     FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {BILIBIN_CAVE_ENTRANCE_FROM_BILIBIN_CAVE_RAPIER_SOUTH, CONDITION_ALWAYS,
     342, 0, 254, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {BILIBIN_CAVE_ENTRANCE_FROM_BILIBIN_CAVE_RAPIER_NORTH, CONDITION_ALWAYS,
     440, 0, 136, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gBilibinCaveRapierEntrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 864, 0, 168, FACING_SOUTH, 0, -1, -1,
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
    {SCENE_TABLE_END},
};

struct SceneEntrance gBilibinCaveIceEntrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 172, 0, 300, FACING_SOUTH, 0, -1, -1,
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
    {SCENE_TABLE_END},
};

u32 gBilibinCaveExits[] = {
    SCENE_EXITS(BilibinCaveEntrance),
    SCENE_EXIT(BILIBIN_CAVE_ENTRANCE_TO_WORLD_MAP, WorldMap,
               WORLD_MAP_FROM_BILIBIN_CAVE_ENTRANCE),
    SCENE_EXIT(BILIBIN_CAVE_ENTRANCE_TO_BILIBIN_CAVE_RAPIER_SOUTH,
               BilibinCaveRapier,
               BILIBIN_CAVE_RAPIER_FROM_BILIBIN_CAVE_ENTRANCE_WEST),
    SCENE_EXIT(BILIBIN_CAVE_ENTRANCE_TO_BILIBIN_CAVE_RAPIER_NORTH,
               BilibinCaveRapier,
               BILIBIN_CAVE_RAPIER_FROM_BILIBIN_CAVE_ENTRANCE_EAST),
    SCENE_EXITS(BilibinCaveRapier),
    SCENE_EXIT(BILIBIN_CAVE_RAPIER_TO_BILIBIN_CAVE_ENTRANCE_WEST,
               BilibinCaveEntrance,
               BILIBIN_CAVE_ENTRANCE_FROM_BILIBIN_CAVE_RAPIER_SOUTH),
    SCENE_EXIT(BILIBIN_CAVE_RAPIER_TO_BILIBIN_CAVE_ENTRANCE_EAST,
               BilibinCaveEntrance,
               BILIBIN_CAVE_ENTRANCE_FROM_BILIBIN_CAVE_RAPIER_NORTH),
    SCENE_EXIT(BILIBIN_CAVE_RAPIER_TO_BILIBIN_CAVE_ICE_WEST, BilibinCaveIce,
               BILIBIN_CAVE_ICE_FROM_BILIBIN_CAVE_RAPIER_WEST),
    SCENE_EXIT(BILIBIN_CAVE_RAPIER_TO_TORCHES, BilibinCaveRapier,
               BILIBIN_CAVE_RAPIER_TORCHES_FROM_BILIBIN_CAVE_RAPIER),
    SCENE_EXIT(BILIBIN_CAVE_RAPIER_TO_BILIBIN_CAVE_ICE_EAST, BilibinCaveIce,
               BILIBIN_CAVE_ICE_FROM_BILIBIN_CAVE_RAPIER_EAST),
    SCENE_EXIT(BILIBIN_CAVE_RAPIER_TORCHES_TO_BILIBIN_CAVE_RAPIER,
               BilibinCaveRapier, BILIBIN_CAVE_RAPIER_FROM_TORCHES),
    SCENE_EXITS(BilibinCaveIce),
    SCENE_EXIT(BILIBIN_CAVE_ICE_TO_BILIBIN_CAVE_RAPIER_WEST, BilibinCaveRapier,
               BILIBIN_CAVE_RAPIER_FROM_BILIBIN_CAVE_ICE_WEST),
    SCENE_EXIT(BILIBIN_CAVE_ICE_TO_BILIBIN_CAVE_RAPIER_EAST, BilibinCaveRapier,
               BILIBIN_CAVE_RAPIER_FROM_BILIBIN_CAVE_ICE_EAST),
    SCENE_EXIT(BILIBIN_CAVE_ICE_TO_WORLD_MAP, WorldMap,
               WORLD_MAP_FROM_BILIBIN_CAVE_ICE),
    SCENE_EXIT(BILIBIN_CAVE_ICE_TO_CHESTS, BilibinCaveIce,
               BILIBIN_CAVE_ICE_CHESTS_FROM_BILIBIN_CAVE_ICE),
    SCENE_EXIT(BILIBIN_CAVE_ICE_CHESTS_TO_BILIBIN_CAVE_ICE, BilibinCaveIce,
               BILIBIN_CAVE_ICE_FROM_CHESTS),
    SCENE_EXITS_END,
};

struct ActorPlacement gBilibinCaveActorPlacementsOther[] = {
    {SCENE_TABLE_END},
};

// clang-format off
struct ActorPlacement gBilibinCaveEntranceActorPlacements[] = {
    { SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(136), 0, PIXELS(208), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SCENE_TABLE_END },
};

struct ActorPlacement gBilibinCaveRapierActorPlacements[] = {
    { SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(840), 0, PIXELS(120), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_FIRE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(856), 0, PIXELS(120), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(872), 0, PIXELS(120), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SCENE_TABLE_END },
};

struct ActorPlacement gBilibinCaveIceActorPlacements[] = {
    { SPRITE_FIRE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(280), 0, PIXELS(232), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(152), 0, PIXELS(264), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(520), 0, PIXELS(280), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(504), 0, PIXELS(360), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(536), 0, PIXELS(360), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(504), 0, PIXELS(520), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(584), 0, PIXELS(520), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, (s32)gBilibinCave_ClearSpriteScript,
        PIXELS(296), 0, PIXELS(230), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, (s32)gBilibinCave_ClearSpriteScript,
        PIXELS(504), 0, PIXELS(280), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, (s32)gBilibinCave_ClearSpriteScript,
        PIXELS(584), 0, PIXELS(376), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(344), 0, PIXELS(224), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(392), 0, PIXELS(280), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(424), 0, PIXELS(296), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(456), 0, PIXELS(264), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(328), 0, PIXELS(288), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(200), 0, PIXELS(264), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(248), 0, PIXELS(264), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(136), 0, PIXELS(296), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(184), 0, PIXELS(312), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SCENE_TABLE_END },
};

struct Trigger gBilibinCaveTriggersOther[] = {
    { TRIGGERS_END },
};

struct Trigger gBilibinCaveEntranceTriggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, BILIBIN_CAVE_ENTRANCE_TO_WORLD_MAP },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, BILIBIN_CAVE_ENTRANCE_TO_BILIBIN_CAVE_RAPIER_SOUTH },
    { TRIGGER_EXIT, 3, CONDITION_ALWAYS, BILIBIN_CAVE_ENTRANCE_TO_BILIBIN_CAVE_RAPIER_NORTH },
    { TRIGGER_STEP, 10, CONDITION_ALWAYS, EVENT(BilibinCave_WalkOntoLedgeOverCaveMouth) },
    { TRIGGER_STEP, 11, CONDITION_ALWAYS, EVENT(BilibinCave_WalkOffLedgeOverCaveMouth) },
    { PSYNERGY_ON_ACTOR(ABILITY_GROWTH), ACTOR_SPROUT, FLAG_BILIBIN_CAVE_ENTRANCE_SPROUT_GROWN, EVENT(BilibinCave_GrowPlant) },
    { TRIGGERS_END },
};

struct Trigger gBilibinCaveRapierTriggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, BILIBIN_CAVE_RAPIER_TO_BILIBIN_CAVE_ENTRANCE_WEST },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, BILIBIN_CAVE_RAPIER_TO_BILIBIN_CAVE_ENTRANCE_EAST },
    { TRIGGER_EXIT, 3, CONDITION_ALWAYS, BILIBIN_CAVE_RAPIER_TO_BILIBIN_CAVE_ICE_WEST },
    { TRIGGER_EXIT, 4, CONDITION_ALWAYS, BILIBIN_CAVE_RAPIER_TO_TORCHES },
    { TRIGGER_EXIT, 5, CONDITION_ALWAYS, BILIBIN_CAVE_RAPIER_TO_BILIBIN_CAVE_ICE_EAST },
    { TRIGGER_EXIT, 6, CONDITION_ALWAYS, BILIBIN_CAVE_RAPIER_TORCHES_TO_BILIBIN_CAVE_RAPIER },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 100, 0xf5d, FIND_ITEM(ITEM_ELVEN_RAPIER) },
    { PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(BilibinCave_RevealHiddenPath) },
    { PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(BilibinCave_HideRevealedPath) },
    { PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_RAPIER_ROOM_FIRE, FLAG_BILIBIN_CAVE_SECOND_FIRE_DOUSED, EVENT(BilibinCave_DouseRapierRoomFire) },
    { TRIGGERS_END },
};

struct Trigger gBilibinCaveIceTriggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, BILIBIN_CAVE_ICE_TO_BILIBIN_CAVE_RAPIER_WEST },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, BILIBIN_CAVE_ICE_TO_BILIBIN_CAVE_RAPIER_EAST },
    { TRIGGER_EXIT, 3, CONDITION_ALWAYS, BILIBIN_CAVE_ICE_TO_WORLD_MAP },
    { TRIGGER_EXIT, 4, CONDITION_ALWAYS, BILIBIN_CAVE_ICE_TO_CHESTS },
    { TRIGGER_EXIT, 5, CONDITION_ALWAYS, BILIBIN_CAVE_ICE_CHESTS_TO_BILIBIN_CAVE_ICE },
    { TRIGGER_STEP, 20, CONDITION_ALWAYS, EVENT(BilibinCave_StartFootprints) },
    { TRIGGER_STEP, 21, CONDITION_ALWAYS, EVENT(BilibinCave_StopFootprints) },
    { PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_ICE_ROOM_FIRE, FLAG_BILIBIN_CAVE_FIRST_FIRE_DOUSED, EVENT(BilibinCave_DouseIceRoomFire) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_FIRST_PUDDLE, FLAG_BILIBIN_CAVE_FIRST_FIRE_DOUSED, EVENT(BilibinCave_FreezeFirstPuddle) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SECOND_PUDDLE, CONDITION_ALWAYS, EVENT(BilibinCave_FreezeSecondPuddle) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_THIRD_PUDDLE, IF_FLAG_SET(FLAG_BILIBIN_CAVE_THIRD_PUDDLE_MELTED), EVENT(BilibinCave_FreezeThirdPuddle) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_THIRD_TORCH, CONDITION_ALWAYS, EVENT(BilibinCave_UpdateThirdPuddleFromTorches) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FOURTH_TORCH, CONDITION_ALWAYS, EVENT(BilibinCave_UpdateThirdPuddleFromTorches) },
    { STEP_TRIGGER_WALKING, 10, CONDITION_ALWAYS, EVENT(BilibinCave_PushTorch) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), 11, CONDITION_ALWAYS, EVENT(BilibinCave_PushTorch) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 100, 0xf5e, FIND_ITEM(ITEM_VIAL) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 101, 0xfc6, FIND_ITEM(ITEM_PSY_CRYSTAL) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 102, 0xfc7, FIND_ITEM(ITEM_WATER_OF_LIFE) },
    { TRIGGERS_END },
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

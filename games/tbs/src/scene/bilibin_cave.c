/// @file
/// The Bilibin Cave Entrance and Bilibin Cave are two areas between Bilibin
/// and Imil that share this overlay.
///
/// The Bilibin Cave Entrance is SceneId_BilibinCaveEntrance. Bilibin Cave is
/// SceneId_BilibinCaveRapier and, beyond it, SceneId_BilibinCaveIce, whose far
/// end opens onto the World Map farther north.
///
/// No one speaks here. Each scene is a run of Psynergy puzzles. The getters
/// pick its tables by gMainRoutineWork.next, and Scene_Initialize
/// restores what the party has already solved in the scene being entered.
///
/// The Bilibin Cave Entrance is a clearing below a cliff, entered from the
/// World Map, with a waterfall to the west. The cave has two mouths in the
/// cliff: a low one at ground level (cell (21, 14)) and a high one on a ledge
/// to the north-east (cell (27, 7)). A sprout grows at the foot of the cliff
/// (ACTOR_SPROUT, cell (8, 13)); Growth turns it into a vine Isaac can climb
/// to the ledge, and in by the high mouth (BilibinCave_GrowPlant).
/// FLAG_BILIBIN_CAVE_ENTRANCE_SPROUT_GROWN keeps it grown until the party
/// leaves the Bilibin Cave Entrance, into the cave or out to the World Map,
/// since both are other areas.
///
/// The cliff-top path passes just above the low mouth, and its cells there
/// switch the mouth's way in off and on again
/// (BilibinCave_CloseLowerCaveMouth, BilibinCave_OpenLowerCaveMouth).
///
/// Inside, in SceneId_BilibinCaveRapier, the high mouth leads to the chest of
/// the Elven Rapier. Coming back later with Reveal, the party finds a way
/// through the wall beside that chest
/// (BilibinCave_RevealHiddenPath), and BilibinCave_HideRevealedPath closes it
/// again when Reveal wears off. Beyond is a small room of two torches, the west
/// one cold and the east one lit, with a little fire burning in the doorway
/// between them (ACTOR_RAPIER_ROOM_FIRE). Douse puts it out for good
/// (FLAG_BILIBIN_CAVE_SECOND_FIRE_DOUSED), and the doorway leads to the cave's
/// two last chests, a Psy Crystal and Water of Life.
///
/// SceneId_BilibinCaveIce is the cold part of the cave on the way to Imil,
/// full of ice pillars, with puddles, a fire and burning torches. Frost
/// freezes a puddle into an ice pillar, but next to a fire or a torch the ice
/// melts straight back (BilibinCave_MeltFrostBack).
///
/// The first puddle lies beside a little fire (cell (17, 14)).
/// Douse on the fire puts it out (FLAG_BILIBIN_CAVE_FIRST_FIRE_DOUSED), and
/// the puddle freezes by itself into a pillar, whose top the party hops
/// across.
///
/// The second puddle lies beside a lit torch near the Vial's chest, which
/// cannot be moved, and Frost on it always melts back.
///
/// The third puddle starts as an ice pillar (cell (36, 23)), east of a pair of
/// torches that can be pushed or moved with Move (ACTOR_THIRD_TORCH and
/// ACTOR_FOURTH_TORCH). Set beside it, on cell (35, 23), either torch melts it
/// into a puddle, and the cells north and south of it become hop squares, so
/// the party can jump over the puddle and go on. Take the torch away and it
/// freezes again (BilibinCave_UpdateThirdPuddleFromTorches).
///
/// FLAG_BILIBIN_CAVE_THIRD_TORCH_BY_PILLAR and
/// FLAG_BILIBIN_CAVE_FOURTH_TORCH_BY_PILLAR remember which torch stands there,
/// and FLAG_BILIBIN_CAVE_THIRD_PUDDLE_MELTED that the pillar has melted. So
/// coming back from SceneId_BilibinCaveRapier puts the torches and the pillar
/// back. Leaving Bilibin Cave resets them.
///
/// Nine more pieces of ice only stand there: five pillars from
/// ACTOR_FIRST_ICE_PILLAR and four lower pieces from ACTOR_FIRST_LOW_ICE.
///
/// Snow has blown in at the cave's mouth to the World Map, the way out to
/// Imil, around BILIBIN_CAVE_ICE_FROM_WORLD_MAP. Walking onto it Isaac starts
/// leaving footprints (BilibinCave_StartFootprints), and walking off it stops
/// them (BilibinCave_StopFootprints).
///
/// Its chests hold a Vial, a Psy Crystal and Water of Life.

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "types.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/actor_script.h"
#include "field/event.h"
#include "field/field.h"
#include "field/map_grid.h"
#include "field/map_grid_scroll.h"
#include "field/pushed_actor.h"
#include "field/scene_tables.h"
#include "field/script_operands.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/heap_slots.h"
#include "system/main_routine_work.h"
#include "system/system.h"
#include "system/system_call.h"

extern struct SceneEntrance gBilibinCaveEntrancesOther[];
extern struct SceneEntrance gBilibinCaveEntranceEntrances[];
extern struct SceneEntrance gBilibinCaveRapierEntrances[];
extern struct SceneEntrance gBilibinCaveIceEntrances[];
extern u32 gBilibinCaveExits[];
extern struct ActorPlacement gBilibinCaveActorPlacementsOther[];
extern struct ActorPlacement gBilibinCaveEntranceActorPlacements[];
extern struct ActorPlacement gBilibinCaveRapierActorPlacements[];
extern struct ActorPlacement gBilibinCaveIceActorPlacements[];
extern struct Trigger gBilibinCaveTriggersOther[];
extern struct Trigger gBilibinCaveEntranceTriggers[];
extern struct Trigger gBilibinCaveRapierTriggers[];
extern struct Trigger gBilibinCaveIceTriggers[];
extern s32 gPushedActor_DirectionSteps[];

/// In Bilibin Cave's ice room, a movable torch has melted the third puddle's
/// ice pillar.
#define FLAG_BILIBIN_CAVE_THIRD_PUDDLE_MELTED FLAG_AREA_TEMP_02
/// In Bilibin Cave's ice room, the third torch stands beside the third puddle's
/// ice pillar.
#define FLAG_BILIBIN_CAVE_THIRD_TORCH_BY_PILLAR FLAG_AREA_TEMP_03
/// In Bilibin Cave's ice room, the fourth torch stands beside the third
/// puddle's ice pillar.
#define FLAG_BILIBIN_CAVE_FOURTH_TORCH_BY_PILLAR FLAG_AREA_TEMP_04
/// At the Bilibin Cave Entrance, Growth has grown the sprout into a vine.
#define FLAG_BILIBIN_CAVE_ENTRANCE_SPROUT_GROWN FLAG_AREA_TEMP_05

static void BilibinCave_RestoreEntrance(void);
static void BilibinCave_RestoreRapierRoom(void);
static void BilibinCave_RestoreIceRoom(void);
static void BilibinCave_EmptyActorSetupHook(s32 actorId);

/// Actors of the Bilibin Cave Entrance (SceneId_BilibinCaveEntrance).
enum {
  /// The sprout Growth grows into a vine.
  ACTOR_SPROUT = ACTOR_FIRST_PLACED
};

/// Actors of SceneId_BilibinCaveRapier, numbered by the rows of
/// gBilibinCaveRapierActorPlacements.
enum {
  /// A torch west of the fire. It is cold (TORCH_ANIMATION_UNLIT) where its
  /// twin burns.
  ACTOR_RAPIER_ROOM_WEST_TORCH = ACTOR_FIRST_PLACED,
  /// A little fire in the doorway between the two torches, which Douse puts
  /// out.
  ACTOR_RAPIER_ROOM_FIRE,
  /// A torch east of the fire.
  ACTOR_RAPIER_ROOM_EAST_TORCH
};

/// Actors of SceneId_BilibinCaveIce, numbered by the rows of
/// gBilibinCaveIceActorPlacements.
enum {
  /// A little fire beside the first puddle, which Douse puts out.
  ACTOR_ICE_ROOM_FIRE = ACTOR_FIRST_PLACED,
  /// Six torches, 9 to 14. The second stands beside ACTOR_SECOND_PUDDLE.
  ACTOR_FIRST_TORCH = 9,
  /// The third torch, moved by Move or pushed.
  ACTOR_THIRD_TORCH = 11,
  /// The fourth torch, moved by Move or pushed.
  ACTOR_FOURTH_TORCH = 12,
  /// A puddle beside ACTOR_ICE_ROOM_FIRE. It freezes into a pillar the party
  /// hops across once the fire is out.
  ACTOR_FIRST_PUDDLE = 15,
  /// A puddle beside a torch that stays put, so it never stays frozen.
  ACTOR_SECOND_PUDDLE = 16,
  /// A puddle frozen into an ice pillar, melted while a torch stands on cell
  /// (35, 23).
  ACTOR_THIRD_PUDDLE = 17,
  /// Five ice pillars, 18 to 22 (PUDDLE_ANIMATION_PILLAR). Nothing acts on
  /// them.
  ACTOR_FIRST_ICE_PILLAR = 18,
  /// Four pieces of ice, 23 to 26, held at the second step of freezing
  /// (PUDDLE_ANIMATION_LOW_ICE). Nothing acts on them.
  ACTOR_FIRST_LOW_ICE = 23
};

/// SPRITE_SPROUT's first animation: grown into a vine, held.
enum { SPROUT_ANIMATION_VINE = 0 };
/// SPRITE_FIRE's flickering flame.
enum { FIRE_ANIMATION_BURNING = 2 };

/// Cell behaviors the three maps' triggers answer besides the exits.
enum BilibinCaveCellBehavior {
  /// At the entrance, the cliff-top path just above the low cave mouth,
  /// cells (20, 13..14) and (22, 13..14).
  CELL_BEHAVIOR_BILIBIN_CAVE_ABOVE_LOW_MOUTH = 10,
  /// At the entrance, the path on either side of that stretch, cells
  /// (19, 12..13) and (23, 12..13).
  CELL_BEHAVIOR_BILIBIN_CAVE_BESIDE_LOW_MOUTH = 11,
  /// The Elven Rapier's chest in SceneId_BilibinCaveRapier, cell (35, 28).
  CELL_BEHAVIOR_BILIBIN_CAVE_ELVEN_RAPIER_CHEST = 100,
  /// The chests of SceneId_BilibinCaveIce, at (32, 16), (53, 36) and (52, 37).
  CELL_BEHAVIOR_BILIBIN_CAVE_VIAL_CHEST = 100,
  CELL_BEHAVIOR_BILIBIN_CAVE_PSY_CRYSTAL_CHEST = 101,
  CELL_BEHAVIOR_BILIBIN_CAVE_WATER_OF_LIFE_CHEST = 102,
  /// The dotted track the two movable torches are pushed along, round
  /// (31..33, 19..23).
  CELL_BEHAVIOR_BILIBIN_CAVE_TORCH_TRACK = 10,
  /// The track's east end, cells (34..36, 23), from which walking west pushes
  /// a torch.
  CELL_BEHAVIOR_BILIBIN_CAVE_TORCH_TRACK_END = 11,
  /// The snow blown in at the mouth to the World Map, round (32..34, 32..35).
  CELL_BEHAVIOR_BILIBIN_CAVE_SNOW = 20,
  /// The cells round the snow's edge.
  CELL_BEHAVIOR_BILIBIN_CAVE_SNOW_EDGE = 21
};

/// Spare cells near the top-left corner of the entrance's and the ice room's
/// maps, whose behaviors are copied where the low mouth, the vine and the
/// puddles change, as (column, row).
#define LOW_MOUTH_OPEN_CELL 0, 0   ///< the low mouth's way into the cave
#define LOW_MOUTH_CLOSED_CELL 1, 0 ///< plain ground on the cliff-top path
#define VINE_CELL 31, 0            ///< a vine to climb
#define FIRST_PILLAR_CELL 0, 0     ///< a hop square on the ice pillar's top
#define THIRD_PUDDLE_NORTH_HOP_CELL 0, 1 ///< hop squares beside the puddle
#define THIRD_PUDDLE_SOUTH_HOP_CELL 0, 2
#define THIRD_PUDDLE_NORTH_GROUND_CELL 1, 1 ///< plain ground beside the pillar
#define THIRD_PUDDLE_SOUTH_GROUND_CELL 1, 2

/// A script command that draws the puddle it runs on without a shadow.
static s32 BilibinCave_HideScriptedShadow(struct Actor *actor) {
  Actor_SetSpriteFlags(actor, 0);
  return 0;
}

const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 sceneId = gMainRoutineWork.next;

  if (sceneId == SCENE_BILIBIN_CAVE_ENTRANCE) {
    return gBilibinCaveEntranceEntrances;
  }
  if (sceneId == SCENE_BILIBIN_CAVE_RAPIER) {
    return gBilibinCaveRapierEntrances;
  }
  if (sceneId == SCENE_BILIBIN_CAVE_ICE) {
    return gBilibinCaveIceEntrances;
  }
  return gBilibinCaveEntrancesOther;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) { return gBilibinCaveExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s16 sceneId = gMainRoutineWork.next;

  if (sceneId == SCENE_BILIBIN_CAVE_ENTRANCE) {
    return gBilibinCaveEntranceActorPlacements;
  }
  if (sceneId == SCENE_BILIBIN_CAVE_RAPIER) {
    return gBilibinCaveRapierActorPlacements;
  }
  if (sceneId == SCENE_BILIBIN_CAVE_ICE) {
    return gBilibinCaveIceActorPlacements;
  }
  return gBilibinCaveActorPlacementsOther;
}

const struct Trigger *Scene_GetTriggers(void) {
  s16 sceneId = gMainRoutineWork.next;

  if (sceneId == SCENE_BILIBIN_CAVE_ENTRANCE) {
    return gBilibinCaveEntranceTriggers;
  }
  if (sceneId == SCENE_BILIBIN_CAVE_RAPIER) {
    return gBilibinCaveRapierTriggers;
  }
  if (sceneId == SCENE_BILIBIN_CAVE_ICE) {
    return gBilibinCaveIceTriggers;
  }
  return gBilibinCaveTriggersOther;
}

/// Nothing visible happens when Isaac, on the grassy path along the cliff top
/// from the vine toward the high cave mouth at the Bilibin Cave Entrance,
/// steps onto the stretch drawn just above the low mouth, cells (20, 13..14)
/// and (22, 13..14). The game quietly turns off the low mouth's way into the
/// cave, cell (21, 14), which lies between them in the map, so that walking
/// along the path over it does not take him in.
static void BilibinCave_CloseLowerCaveMouth(void) {
  MapGrid_CopyCellAttributes(LOW_MOUTH_CLOSED_CELL, 1, 1, 21, 14);
}

/// Nothing visible happens when Isaac walks on along the cliff-top path past
/// that stretch at the Bilibin Cave Entrance, cells (19, 12..13) and
/// (23, 12..13). The game turns the low mouth's way into the cave, cell
/// (21, 14), back on.
static void BilibinCave_OpenLowerCaveMouth(void) {
  MapGrid_CopyCellAttributes(LOW_MOUTH_OPEN_CELL, 1, 1, 21, 14);
}

/// Reveal in Bilibin Cave, near the Elven Rapier's chest, shows a way through
/// the wall beside it, cells (32, 24..26), to the room where a little fire
/// burns between two torches.
static void BilibinCave_RevealHiddenPath(void) {
  MapGrid_CopyCellsTo(111, 37, 97, 21, 1, 3);
  {
    s32 destinationCellX = 32;
    s32 destinationCellZ = 24;

    MapGrid_CopyCellAttributes(46, 38, 3, 2, destinationCellX,
                               destinationCellZ);
  }
}

/// As Reveal wears off in Bilibin Cave, the way through the wall beside the
/// Elven Rapier's chest is hidden again.
static void BilibinCave_HideRevealedPath(void) {
  MapGrid_CopyCellsTo(95, 21, 97, 21, 1, 3);
  {
    s32 destinationCellX = 32;
    s32 destinationCellZ = 25;

    MapGrid_CopyCellAttributes(46, 38, 3, 1, destinationCellX,
                               destinationCellZ);
  }
}

/// Douse on the little fire in the doorway of the room of two torches, past the
/// way Reveal shows beside the Elven Rapier's chest, puts it out for good. A
/// rain cloud gathers over the doorway, the flame goes out, and the doorway on
/// to the cave's last two chests is open.
static void BilibinCave_DouseRapierRoomFire(void) {
  Event_Start();
  Actor_SetPosition(ACTOR_RAPIER_ROOM_FIRE, 0, 0);
  GameFlag_Set(FLAG_BILIBIN_CAVE_SECOND_FIRE_DOUSED);
  Event_End();
}

/// Douse on the little fire in the cold part of Bilibin Cave
/// (cell (17, 14)) puts it out, and the puddle beside it freezes by itself into
/// a tall ice pillar, whose top the party hops across.
static void BilibinCave_DouseIceRoomFire(void) {
  Event_Start();
  Actor_SetPosition(ACTOR_ICE_ROOM_FIRE, 0, 0);
  GameFlag_Set(FLAG_BILIBIN_CAVE_FIRST_FIRE_DOUSED);
  Event_Wait(40);
  // The pillar is drawn under the party, and its cell becomes a hop square.
  Actor_SetAnimationAndWait(ACTOR_FIRST_PUDDLE, PUDDLE_ANIMATION_FREEZE);
  Actor_Get(ACTOR_FIRST_PUDDLE)->motionFlags = 0;
  Actor_Get(ACTOR_FIRST_PUDDLE)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  Actor_SetSpritePriority(ACTOR_FIRST_PUDDLE, 2);
  MapGrid_CopyCellAttributes(FIRST_PILLAR_CELL, 1, 1, 18, 14);
  Event_End();
}

/// Frost on a puddle in Bilibin Cave that a fire or torch beside it keeps warm
/// forms ice on it for a moment, which melts straight back into water.
static inline void BilibinCave_MeltFrostBack(s32 actorId) {
  Event_Start();
  Actor_SetSpriteColorById(actorId, SPRITE_COLOR_NORMAL);
  Event_Wait(40);
  Sound_Play(SE_HIDDEN_PASSAGE_OPEN);
  Actor_SetAnimationAndWait(actorId, PUDDLE_ANIMATION_MELT);
  Event_End();
}

/// Frost on the puddle beside the little fire in the cold part of
/// Bilibin Cave (cell (18, 14)), while the fire burns, makes ice that melts
/// straight back. Douse on the fire is what freezes it.
static void BilibinCave_FreezeFirstPuddle(void) {
  BilibinCave_MeltFrostBack(ACTOR_FIRST_PUDDLE);
}

/// Frost on the puddle beside the lit torch near the Vial's chest in the cold
/// part of Bilibin Cave makes ice that melts straight back. The torch cannot
/// be moved, so this puddle never freezes.
static void BilibinCave_FreezeSecondPuddle(void) {
  BilibinCave_MeltFrostBack(ACTOR_SECOND_PUDDLE);
}

/// Frost on the puddle a pushed torch has melted, at cell (36, 23) in the cold
/// part of Bilibin Cave, makes ice that melts straight back while the torch
/// stands beside it (FLAG_BILIBIN_CAVE_THIRD_PUDDLE_MELTED).
static void BilibinCave_FreezeThirdPuddle(void) {
  BilibinCave_MeltFrostBack(ACTOR_THIRD_PUDDLE);
}

/// The cell beside the third puddle's ice pillar where a torch melts it.
#define TORCH_BY_PILLAR_COLUMN 35
#define TORCH_BY_PILLAR_ROW 23

/// Move on either of the pair of torches west of the ice pillar in the cold
/// part of Bilibin Cave slides it a cell. Set beside the pillar, on cell
/// (35, 23), a torch melts the pillar into a puddle, and the
/// cells north and south of it, (36, 22) and (36, 24), become hop squares to
/// jump it by. Taken away again, the puddle freezes back into a pillar, and the
/// hop squares are plain ground once more. The same work follows every push.
static void BilibinCave_UpdateThirdPuddleFromTorches(void) {
  struct Actor *firstActor;
  struct Actor *secondActor;
  s32 destinationCellX;

  firstActor = Actor_Get(ACTOR_THIRD_TORCH);
  secondActor = Actor_Get(ACTOR_FOURTH_TORCH);
  if (FIELD_CELL(firstActor->x.fixed) == TORCH_BY_PILLAR_COLUMN &&
      (FIELD_CELL(firstActor->z.fixed)) == TORCH_BY_PILLAR_ROW) {
    GameFlag_Set(FLAG_BILIBIN_CAVE_THIRD_TORCH_BY_PILLAR);
  } else {
    GameFlag_Clear(FLAG_BILIBIN_CAVE_THIRD_TORCH_BY_PILLAR);
  }
  if (FIELD_CELL(secondActor->x.fixed) == TORCH_BY_PILLAR_COLUMN &&
      (FIELD_CELL(secondActor->z.fixed)) == TORCH_BY_PILLAR_ROW) {
    GameFlag_Set(FLAG_BILIBIN_CAVE_FOURTH_TORCH_BY_PILLAR);
  } else {
    GameFlag_Clear(FLAG_BILIBIN_CAVE_FOURTH_TORCH_BY_PILLAR);
  }
  if (GameFlag_Get(FLAG_BILIBIN_CAVE_THIRD_TORCH_BY_PILLAR) != 0 ||
      GameFlag_Get(FLAG_BILIBIN_CAVE_FOURTH_TORCH_BY_PILLAR) != 0) {
    if (Event_GetFlag(FLAG_BILIBIN_CAVE_THIRD_PUDDLE_MELTED) == 0) {
      Event_Start();
      Event_Wait(40);
      Sound_Play(SE_HIDDEN_PASSAGE_OPEN);
      destinationCellX = 36;
      Actor_SetAnimationAndWait(ACTOR_THIRD_PUDDLE, PUDDLE_ANIMATION_MELT);
      MapGrid_CopyCellAttributes(THIRD_PUDDLE_NORTH_HOP_CELL, 1, 1,
                                 destinationCellX, 22);
      MapGrid_CopyCellAttributes(THIRD_PUDDLE_SOUTH_HOP_CELL, 1, 1,
                                 destinationCellX, 24);
      Event_End();
    }
    GameFlag_Set(FLAG_BILIBIN_CAVE_THIRD_PUDDLE_MELTED);
  } else {
    if (Event_GetFlag(FLAG_BILIBIN_CAVE_THIRD_PUDDLE_MELTED) != 0) {
      Event_Start();
      Event_Wait(40);
      Sound_Play(SE_SHIMMER);
      destinationCellX = 36;
      Actor_SetAnimationAndWait(ACTOR_THIRD_PUDDLE, PUDDLE_ANIMATION_FREEZE);
      MapGrid_CopyCellAttributes(THIRD_PUDDLE_NORTH_GROUND_CELL, 1, 1,
                                 destinationCellX, 22);
      MapGrid_CopyCellAttributes(THIRD_PUDDLE_SOUTH_GROUND_CELL, 1, 1,
                                 destinationCellX, 24);
      Event_End();
    }
    GameFlag_Clear(FLAG_BILIBIN_CAVE_THIRD_PUDDLE_MELTED);
  }
}

/// Growth on the sprout at the foot of the cliff at the Bilibin Cave Entrance
/// (cell (8, 13)) grows it into a vine up the cliff, which Isaac can climb to
/// the ledge and the high cave mouth.
static void BilibinCave_GrowPlant(void) {
  MapGrid_CopyCellAttributes(VINE_CELL, 1, 1, 8, 13);
  GameFlag_Set(FLAG_BILIBIN_CAVE_ENTRANCE_SPROUT_GROWN);
}

/// Walking onto the snow blown in at the cave mouth toward Imil, in the cold
/// part of Bilibin Cave (cells (32..34, 32..35)), Isaac starts leaving
/// footprints.
static void BilibinCave_StartFootprints(void) {
  struct MapGridScrollWork *grid = gMapGridWork[0];
  grid->footprints = 1;
}

/// Walking off the snow at the cave mouth toward Imil, onto the cells round
/// its edge, Isaac stops leaving footprints.
static void BilibinCave_StopFootprints(void) {
  struct MapGridScrollWork *grid = gMapGridWork[0];
  grid->footprints = 0;
}

/// Sets up the scene being entered, with its puzzles already solved.
s32 Scene_Initialize(void) {
  s16 variant = gMainRoutineWork.next;

  if (variant == SCENE_BILIBIN_CAVE_ENTRANCE) {
    BilibinCave_RestoreEntrance();
  } else if (variant == SCENE_BILIBIN_CAVE_RAPIER) {
    BilibinCave_RestoreRapierRoom();
  } else if (variant == SCENE_BILIBIN_CAVE_ICE) {
    BilibinCave_RestoreIceRoom();
  }
  return 0;
}

/// Sets up the Bilibin Cave Entrance, with the vine Growth has grown and its
/// cell.
static void BilibinCave_RestoreEntrance(void) {
  if (GameFlag_Get(FLAG_BILIBIN_CAVE_ENTRANCE_SPROUT_GROWN) != 0) {
    MapGrid_CopyCellAttributes(VINE_CELL, 1, 1, 8, 13);
    Actor_SetAnimation(ACTOR_SPROUT, SPROUT_ANIMATION_VINE);
  }
}

/// Sets up Bilibin Cave's SceneId_BilibinCaveRapier.
static void BilibinCave_RestoreRapierRoom(void) {
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);

  Actor_SetAnimation(ACTOR_RAPIER_ROOM_WEST_TORCH, TORCH_ANIMATION_UNLIT);
  Actor_SetAnimation(ACTOR_RAPIER_ROOM_EAST_TORCH, TORCH_ANIMATION_LIT);

  if (GameFlag_Get(FLAG_BILIBIN_CAVE_SECOND_FIRE_DOUSED) != 0) {
    Actor_SetPosition(ACTOR_RAPIER_ROOM_FIRE, 0, 0);
  } else {
    Actor_SetSpriteFlags(Actor_Get(ACTOR_RAPIER_ROOM_FIRE), 0);
  }
}

/// Draws one of the ice room's torches or pieces of ice without a shadow.
static inline void BilibinCave_HideShadow(s32 actorId) {
  struct Actor *actor = Actor_Get(actorId);
  Actor_SetSpriteFlags(actor, 0);
}

/// Sets up Bilibin Cave's SceneId_BilibinCaveIce.
static void BilibinCave_RestoreIceRoom(void) {
  s32 destinationCellX;

  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  BilibinCave_HideShadow(ACTOR_FIRST_ICE_PILLAR);
  BilibinCave_HideShadow(ACTOR_FIRST_ICE_PILLAR + 1);
  BilibinCave_HideShadow(ACTOR_FIRST_ICE_PILLAR + 2);
  BilibinCave_HideShadow(ACTOR_FIRST_ICE_PILLAR + 3);
  BilibinCave_HideShadow(ACTOR_FIRST_ICE_PILLAR + 4);
  BilibinCave_HideShadow(ACTOR_FIRST_LOW_ICE);
  BilibinCave_HideShadow(ACTOR_FIRST_LOW_ICE + 1);
  BilibinCave_HideShadow(ACTOR_FIRST_LOW_ICE + 2);
  BilibinCave_HideShadow(ACTOR_FIRST_LOW_ICE + 3);
  Actor_SetAnimation(ACTOR_FIRST_ICE_PILLAR, PUDDLE_ANIMATION_PILLAR);
  Actor_SetAnimation(ACTOR_FIRST_ICE_PILLAR + 1, PUDDLE_ANIMATION_PILLAR);
  Actor_SetAnimation(ACTOR_FIRST_ICE_PILLAR + 2, PUDDLE_ANIMATION_PILLAR);
  Actor_SetAnimation(ACTOR_FIRST_ICE_PILLAR + 3, PUDDLE_ANIMATION_PILLAR);
  Actor_SetAnimation(ACTOR_FIRST_ICE_PILLAR + 4, PUDDLE_ANIMATION_PILLAR);
  Actor_SetAnimation(ACTOR_FIRST_LOW_ICE, PUDDLE_ANIMATION_LOW_ICE);
  Actor_SetAnimation(ACTOR_FIRST_LOW_ICE + 1, PUDDLE_ANIMATION_LOW_ICE);
  Actor_SetAnimation(ACTOR_FIRST_LOW_ICE + 2, PUDDLE_ANIMATION_LOW_ICE);
  Actor_SetAnimation(ACTOR_FIRST_LOW_ICE + 3, PUDDLE_ANIMATION_LOW_ICE);
  Actor_SetAnimation(ACTOR_FIRST_TORCH, TORCH_ANIMATION_LIT);
  Actor_SetAnimation(ACTOR_FIRST_TORCH + 1, TORCH_ANIMATION_LIT);
  Actor_SetAnimation(ACTOR_FIRST_TORCH + 2, TORCH_ANIMATION_LIT);
  Actor_SetAnimation(ACTOR_FIRST_TORCH + 3, TORCH_ANIMATION_LIT);
  Actor_SetAnimation(ACTOR_FIRST_TORCH + 4, TORCH_ANIMATION_LIT);
  Actor_SetAnimation(ACTOR_FIRST_TORCH + 5, TORCH_ANIMATION_LIT);
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
  if (GameFlag_Get(FLAG_BILIBIN_CAVE_FIRST_FIRE_DOUSED) != 0) {
    Actor_SetPosition(ACTOR_ICE_ROOM_FIRE, 0, 0);
    Actor_SetAnimation(ACTOR_FIRST_PUDDLE, PUDDLE_ANIMATION_PILLAR);
    Actor_Get(ACTOR_FIRST_PUDDLE)->motionFlags = 0;
    Actor_Get(ACTOR_FIRST_PUDDLE)->y.fixed = -toFIXED(4);
    Actor_Get(ACTOR_FIRST_PUDDLE)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    Actor_SetSpritePriority(ACTOR_FIRST_PUDDLE, 2);
    MapGrid_CopyCellAttributes(FIRST_PILLAR_CELL, 1, 1, 18, 14);
  } else {
    Actor_SetAnimation(ACTOR_ICE_ROOM_FIRE, FIRE_ANIMATION_BURNING);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_ICE_ROOM_FIRE), 0);
    Actor_SetAnimation(ACTOR_FIRST_PUDDLE, PUDDLE_ANIMATION_PUDDLE);
  }
  Actor_SetAnimation(ACTOR_SECOND_PUDDLE, PUDDLE_ANIMATION_PUDDLE);
  if (GameFlag_Get(FLAG_BILIBIN_CAVE_THIRD_PUDDLE_MELTED) != 0) {
    destinationCellX = 36;
    Actor_SetAnimation(ACTOR_THIRD_PUDDLE, PUDDLE_ANIMATION_PUDDLE);
    MapGrid_CopyCellAttributes(THIRD_PUDDLE_NORTH_HOP_CELL, 1, 1,
                               destinationCellX, 22);
    MapGrid_CopyCellAttributes(THIRD_PUDDLE_SOUTH_HOP_CELL, 1, 1,
                               destinationCellX, 24);
  } else {
    destinationCellX = 36;
    Actor_SetAnimation(ACTOR_THIRD_PUDDLE, PUDDLE_ANIMATION_PILLAR);
    MapGrid_CopyCellAttributes(THIRD_PUDDLE_NORTH_GROUND_CELL, 1, 1,
                               destinationCellX, 22);
    MapGrid_CopyCellAttributes(THIRD_PUDDLE_SOUTH_GROUND_CELL, 1, 1,
                               destinationCellX, 24);
  }
  if (GameFlag_Get(FLAG_BILIBIN_CAVE_THIRD_TORCH_BY_PILLAR) != 0) {
    Actor_SetPosition(ACTOR_THIRD_TORCH, 570, 376);
  }
  if (GameFlag_Get(FLAG_BILIBIN_CAVE_FOURTH_TORCH_BY_PILLAR) != 0) {
    Actor_SetPosition(ACTOR_FOURTH_TORCH, 570, 376);
  }
}

#include "shared/actor_find_at_cell.c"

/// Walking into one of the pair of torches west of the ice pillar in the cold
/// part of Bilibin Cave pushes it a cell along the dotted track, if nothing
/// stands in the cell beyond and the ground there lets it go. Then, as after
/// Move, it may melt or refreeze the pillar.
static void BilibinCave_PushTorch(void) {
  struct Actor *leader;
  struct Actor *target;
  struct Actor *blocker;
  s32 heading;
  s32 nextCellX;
  s32 nextCellZ;
  struct FieldPosition destination;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  heading = FACING_INDEX(leader->facing);

  nextCellX = (leader->x.part[0].integer +
               (FIXEDtoINT(gPushedActor_DirectionSteps[heading]))) >>
              4;
  nextCellZ =
      (leader->z.part[0].integer + (s16)gPushedActor_DirectionSteps[heading]) >>
      4;
  target = Actor_FindAtCell(nextCellX, nextCellZ);
  if (target == NULL)
    return;

  nextCellX = (target->x.part[0].integer +
               (FIXEDtoINT(gPushedActor_DirectionSteps[heading]))) >>
              4;
  nextCellZ =
      (target->z.part[0].integer + (s16)gPushedActor_DirectionSteps[heading]) >>
      4;
  blocker = Actor_FindAtCell(nextCellX, nextCellZ);
  if (blocker != NULL)
    return;

  target->layer = 2;

  Actor_StepFrom(&destination, target, gPushedActor_DirectionSteps[heading]);

  if (FieldActor_CheckMovementCollision(target, &destination) > 0)
    return;

  Actor_SelectAnimation(leader, ANIM_PUSH);
  wait(15);
  Sound_Play(SE_PUSH);

  target->speedLimit = toFIXED(1) / 5;
  target->acceleration = toFIXED(1) / 5;
  FieldActor_SetMoveTargetFX(target, destination.x, destination.y,
                             destination.z);

  leader->speedLimit = toFIXED(1) / 5;
  leader->acceleration = toFIXED(1) / 5;
  FieldActor_SetMoveTargetFX(leader, destination.x, destination.y,
                             destination.z);

  Actor_WaitForMove(target);

  target->x.fixed = destination.x;
  target->z.fixed = destination.z;
  // blocker is null here: the torch comes to rest.
  target->velocityX = (s32)blocker;
  target->velocityZ = (s32)blocker;

  Actor_SelectAnimation(leader, ANIM_STAND);
  BilibinCave_UpdateThirdPuddleFromTorches();
}

/// Does nothing: the ice room calls it for each of its torches and pieces of
/// ice.
static void BilibinCave_EmptyActorSetupHook(s32 actorId) {}

s32 gBilibinCave_NoShadowScript[] = {
    SC_CALL(BilibinCave_HideScriptedShadow),
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
    ACTOR_PLACEMENT(SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        136, 0, 208, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gBilibinCaveRapierActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        840, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        856, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        872, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gBilibinCaveIceActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        280, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        152, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        520, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        504, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        536, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        504, 0, 520, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        584, 0, 520, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, (s32)gBilibinCave_NoShadowScript,
        296, 0, 230, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, (s32)gBilibinCave_NoShadowScript,
        504, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, (s32)gBilibinCave_NoShadowScript,
        584, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        344, 0, 224, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        392, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        424, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        456, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        328, 0, 288, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        200, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        248, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        136, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        184, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    { SCENE_TABLE_END },
};

struct Trigger gBilibinCaveTriggersOther[] = {
    { TRIGGER_TABLE_END },
};

struct Trigger gBilibinCaveEntranceTriggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, BILIBIN_CAVE_ENTRANCE_TO_WORLD_MAP },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, BILIBIN_CAVE_ENTRANCE_TO_BILIBIN_CAVE_RAPIER_SOUTH },
    { TRIGGER_EXIT, 3, CONDITION_ALWAYS, BILIBIN_CAVE_ENTRANCE_TO_BILIBIN_CAVE_RAPIER_NORTH },
    { TRIGGER_STEP, CELL_BEHAVIOR_BILIBIN_CAVE_ABOVE_LOW_MOUTH, CONDITION_ALWAYS, EVENT(BilibinCave_CloseLowerCaveMouth) },
    { TRIGGER_STEP, CELL_BEHAVIOR_BILIBIN_CAVE_BESIDE_LOW_MOUTH, CONDITION_ALWAYS, EVENT(BilibinCave_OpenLowerCaveMouth) },
    { PSYNERGY_ON_ACTOR(ABILITY_GROWTH), ACTOR_SPROUT, FLAG_BILIBIN_CAVE_ENTRANCE_SPROUT_GROWN, EVENT(BilibinCave_GrowPlant) },
    { TRIGGER_TABLE_END },
};

struct Trigger gBilibinCaveRapierTriggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, BILIBIN_CAVE_RAPIER_TO_BILIBIN_CAVE_ENTRANCE_WEST },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, BILIBIN_CAVE_RAPIER_TO_BILIBIN_CAVE_ENTRANCE_EAST },
    { TRIGGER_EXIT, 3, CONDITION_ALWAYS, BILIBIN_CAVE_RAPIER_TO_BILIBIN_CAVE_ICE_WEST },
    { TRIGGER_EXIT, 4, CONDITION_ALWAYS, BILIBIN_CAVE_RAPIER_TO_TORCHES },
    { TRIGGER_EXIT, 5, CONDITION_ALWAYS, BILIBIN_CAVE_RAPIER_TO_BILIBIN_CAVE_ICE_EAST },
    { TRIGGER_EXIT, 6, CONDITION_ALWAYS, BILIBIN_CAVE_RAPIER_TORCHES_TO_BILIBIN_CAVE_RAPIER },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), CELL_BEHAVIOR_BILIBIN_CAVE_ELVEN_RAPIER_CHEST, FLAG_BILIBIN_CAVE_ELVEN_RAPIER, FIND_ITEM(ITEM_ELVEN_RAPIER) },
    { PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(BilibinCave_RevealHiddenPath) },
    { PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(BilibinCave_HideRevealedPath) },
    { PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_RAPIER_ROOM_FIRE, FLAG_BILIBIN_CAVE_SECOND_FIRE_DOUSED, EVENT(BilibinCave_DouseRapierRoomFire) },
    { TRIGGER_TABLE_END },
};

struct Trigger gBilibinCaveIceTriggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, BILIBIN_CAVE_ICE_TO_BILIBIN_CAVE_RAPIER_WEST },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, BILIBIN_CAVE_ICE_TO_BILIBIN_CAVE_RAPIER_EAST },
    { TRIGGER_EXIT, 3, CONDITION_ALWAYS, BILIBIN_CAVE_ICE_TO_WORLD_MAP },
    { TRIGGER_EXIT, 4, CONDITION_ALWAYS, BILIBIN_CAVE_ICE_TO_CHESTS },
    { TRIGGER_EXIT, 5, CONDITION_ALWAYS, BILIBIN_CAVE_ICE_CHESTS_TO_BILIBIN_CAVE_ICE },
    { TRIGGER_STEP, CELL_BEHAVIOR_BILIBIN_CAVE_SNOW, CONDITION_ALWAYS, EVENT(BilibinCave_StartFootprints) },
    { TRIGGER_STEP, CELL_BEHAVIOR_BILIBIN_CAVE_SNOW_EDGE, CONDITION_ALWAYS, EVENT(BilibinCave_StopFootprints) },
    { PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_ICE_ROOM_FIRE, FLAG_BILIBIN_CAVE_FIRST_FIRE_DOUSED, EVENT(BilibinCave_DouseIceRoomFire) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_FIRST_PUDDLE, FLAG_BILIBIN_CAVE_FIRST_FIRE_DOUSED, EVENT(BilibinCave_FreezeFirstPuddle) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SECOND_PUDDLE, CONDITION_ALWAYS, EVENT(BilibinCave_FreezeSecondPuddle) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_THIRD_PUDDLE, IF_FLAG_SET(FLAG_BILIBIN_CAVE_THIRD_PUDDLE_MELTED), EVENT(BilibinCave_FreezeThirdPuddle) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_THIRD_TORCH, CONDITION_ALWAYS, EVENT(BilibinCave_UpdateThirdPuddleFromTorches) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FOURTH_TORCH, CONDITION_ALWAYS, EVENT(BilibinCave_UpdateThirdPuddleFromTorches) },
    { STEP_TRIGGER_WALKING, CELL_BEHAVIOR_BILIBIN_CAVE_TORCH_TRACK, CONDITION_ALWAYS, EVENT(BilibinCave_PushTorch) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_BILIBIN_CAVE_TORCH_TRACK_END, CONDITION_ALWAYS, EVENT(BilibinCave_PushTorch) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), CELL_BEHAVIOR_BILIBIN_CAVE_VIAL_CHEST, FLAG_BILIBIN_CAVE_VIAL, FIND_ITEM(ITEM_VIAL) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), CELL_BEHAVIOR_BILIBIN_CAVE_PSY_CRYSTAL_CHEST, FLAG_BILIBIN_CAVE_PSY_CRYSTAL, FIND_ITEM(ITEM_PSY_CRYSTAL) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), CELL_BEHAVIOR_BILIBIN_CAVE_WATER_OF_LIFE_CHEST, FLAG_BILIBIN_CAVE_WATER_OF_LIFE, FIND_ITEM(ITEM_WATER_OF_LIFE) },
    { TRIGGER_TABLE_END },
};
// clang-format on

#include "shared/pushed_actor_direction_steps.c"

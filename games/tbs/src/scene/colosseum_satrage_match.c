/// @file
/// Colosseum Final 2 (SceneId_ColosseumSatrageMatch), the site of the second
/// finals match, where Isaac, having won the first, races Satrage, the
/// fourth-ranked warrior, through five stages to the arena and fights him
/// there.
///
/// It works as Colosseum Final 1 does (colosseum_azart_match.c), and is reached
/// by the Colosseum's west passage. The stages run from east to west, through
/// the Mini-Maze, the shifting floor stage, the Log-Rolling Stage, The Wall and
/// the Board Walk. Isaac comes here three times, each at its own
/// entrance, and ColosseumSatrageMatch_Initialize restores the stages from
/// their temporary flags (FLAG_OBSTACLE_KNOCKED_DOWN to FLAG_WALL_PILLAR_MOVED,
/// and the Mini-Maze pillars' columns from FLAG_MINI_MAZE_FIRST_PILLAR_COLUMN)
/// before setting up the visit its entrance stands for:
///
/// - COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_WEST, the tour
///   (ColosseumSatrageMatch_IntroduceSecondFinals). The guide only greets the
///   party, since the first finals match's guide explained the rules; the five
///   stages' attendants describe their stages and offer a place for each
///   friend to cheer, and the guide then sends Isaac back to the west passage
///   (COLOSSEUM_SATRAGE_MATCH_TO_COLOSSEUM_SOUTH).
/// - COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_EAST, `Lend a Hand`: Garet, Ivan
///   and Mia in turn may use Psynergy on the stages. Move shifts the
///   Mini-Maze's stone pillars and the stone pillar by The Wall, Halt stops the
///   lifts' operator and with him the shifting floor, Frost freezes two puddles
///   on the Log-Rolling Stage, Growth grows a plant up The Wall, and Force
///   knocks a log down onto the Board Walk. After Mia,
///   COLOSSEUM_SATRAGE_MATCH_LEND_A_HAND_TO_RACE starts the race.
/// - COLOSSEUM_SATRAGE_MATCH_RACE_FROM_LEND_A_HAND, the race against Satrage,
///   who follows a recorded path, after the lettering `Match 2` and a count of
///   three. At the arena the first to arrive takes the Chain Mail and the
///   second the Cotton Shirt, and the battle is ENCOUNTER_ZONE_COLOSSO's third
///   or fourth group, the Satrage with or without the mail.
///
/// A win returns to COLOSSEUM_SATRAGE_MATCH_AFTER_MATCH_WON and on to the
/// Colosseum (COLOSSEUM_SATRAGE_MATCH_TO_COLOSSEUM_NORTH, arriving at
/// COLOSSEUM_FROM_COLOSSEUM_SATRAGE_MATCH_NORTH), which sets
/// FLAG_COLOSSO_SECOND_MATCH_WON; a loss returns to
/// COLOSSEUM_SATRAGE_MATCH_AFTER_MATCH_LOST, and
/// COLOSSEUM_SATRAGE_MATCH_TO_BABI_PALACE wakes the party in Babi's Palace from
/// what it takes for a dream.
///
/// The shifting floor's four lifts run on every visit until Halt stops them
/// (ColosseumSatrageMatch_UpdateLifts): every
/// COLOSSEUM_SATRAGE_MATCH_LIFT_CYCLE_FRAMES the phase flips, all four lifts
/// lined up across the water in one phase and two up and two down, by turns,
/// in the other. Each frame the task copies cell behaviors into row 13 from
/// column 58 and into cell (58, 11): one pair of copies by phase once the
/// last lift has stopped, another while it moves. Each copied cell is a wall,
/// a jump across or open ground, as the lifts line up or part.

#include "script_operands.h"
#include "adventure.h"
#include "io_write_queue.h"
#include "scene_ids.h"
#include "vram_cache.h"
#include "module/import.h"
#include "module/scene/colosseum_satrage_match.h"
#include "module/scene/shared/colosso_solo.h"
#include "module/scene/shared/colosso_marker.h"
#include "module/scene/shared/colosso_setup.h"
#include "module/scene/shared/colosso_occupant.h"
#include "module/overworld/overworld.h"
#include "module/overworld/event_commands.h"
#include "module/field/field.h"
#include "module/game/game.h"
#include "module/window/window.h"
#include "module/window/screens.h"
#include "module/menu/menu.h"
#include "module/sound/sound.h"
#include "module/runtime/runtime.h"
#include "module/scene/shared/colosso_item_icon.h"
#include "battle.h"
#include "keystate.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "fx_work.h"
#include "mem.h"
#include "resource.h"
#include "types.h"
#include "event.h"
#include "scene_tables.h"
#include "io_reg.h"
#include "task.h"
#include "resource_ids.h"
#include "event_inline.h"
#include "text/msg_ids.h"
#include "battle_unit.h"
#include "ability_ids.h"
#include "edition.h"
#include "item_ids.h"
#include "actor_script.h"
#include "actor.h"
#include "heap_slots.h"
#include "heap.h"
#include "scene.h"
#include "colosso_gauge.h"
#include "flags.h"
#include "sounds.h"
#include "audio_engine_symbols.h"
#include "macros.h"

#define COLOSSEUM_SATRAGE_MATCH_LIFT_CYCLE_FRAMES 120
#define COLOSSEUM_SATRAGE_MATCH_LIFT_CROSSING_FRAME 75
#define COLOSSEUM_SATRAGE_MATCH_DEMONSTRATION_TIMEOUT_FRAMES 600
#define COLOSSEUM_SATRAGE_MATCH_LOG_TRAVEL_PIXELS 64

// UB: westward log travel passes -64, which cannot be shifted left as signed C.
// Both callers supply only -64, 0 or 64, so fixed-point multiplication fits.
#ifdef UBFIX
#define COLOSSEUM_SATRAGE_MATCH_OFFSET_FIXED(value) ((value) * 0x10000)
#else
#define COLOSSEUM_SATRAGE_MATCH_OFFSET_FIXED(value) ((value) << 16)
#endif

#if LANGUAGE == LANGUAGE_JAPANESE || LANGUAGE == LANGUAGE_ENGLISH
#define COLOSSEUM_SATRAGE_MATCH_ALTERNATE_PORTRAIT_PALETTE_OFFSET 64
#else
#define COLOSSEUM_SATRAGE_MATCH_ALTERNATE_PORTRAIT_PALETTE_OFFSET 160
#endif

#if LANGUAGE == LANGUAGE_JAPANESE
#define COLOSSEUM_SATRAGE_MATCH_INTRO_SCALE 256
#else
#define COLOSSEUM_SATRAGE_MATCH_INTRO_SCALE 384
#endif

#if LANGUAGE == LANGUAGE_FRENCH
#define COLOSSEUM_SATRAGE_MATCH_PULSE_FIRST_SCALE 512
#define COLOSSEUM_SATRAGE_MATCH_PULSE_HIGH_SCALE 512
#define COLOSSEUM_SATRAGE_MATCH_PULSE_LOW_SCALE 384
#elif LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_ITALIAN
#define COLOSSEUM_SATRAGE_MATCH_PULSE_FIRST_SCALE 384
#define COLOSSEUM_SATRAGE_MATCH_PULSE_HIGH_SCALE 384
#define COLOSSEUM_SATRAGE_MATCH_PULSE_LOW_SCALE 256
#else
#define COLOSSEUM_SATRAGE_MATCH_PULSE_FIRST_SCALE 512
#define COLOSSEUM_SATRAGE_MATCH_PULSE_HIGH_SCALE 384
#define COLOSSEUM_SATRAGE_MATCH_PULSE_LOW_SCALE 256
#endif

// UB: each of these calls a void function through an s32 function type, the
// call the bytes show; UBFIX calls it as declared. Each use is tagged.
#ifdef UBFIX
#define COLOSSEUM_SATRAGE_MATCH_SHOW_MESSAGE Event_ShowMessageAndWait
#define COLOSSEUM_SATRAGE_MATCH_MOVE_ACTOR_PAIR                                \
  ColosseumSatrageMatch_MovePusherAndLog
#define COLOSSEUM_SATRAGE_MATCH_START_MODE_TASK                                \
  ColosseumSatrageMatch_StartLetteringAnimation
#define COLOSSEUM_SATRAGE_MATCH_APPLY_MATCHING_SLOT_VALUE Scene_GiveAndEquipItem
#else
#define COLOSSEUM_SATRAGE_MATCH_SHOW_MESSAGE                                   \
  ((s32 (*)())Event_ShowMessageAndWait)
#define COLOSSEUM_SATRAGE_MATCH_MOVE_ACTOR_PAIR                                \
  ((s32 (*)())ColosseumSatrageMatch_MovePusherAndLog)
#define COLOSSEUM_SATRAGE_MATCH_START_MODE_TASK                                \
  ((s32 (*)())ColosseumSatrageMatch_StartLetteringAnimation)
#define COLOSSEUM_SATRAGE_MATCH_APPLY_MATCHING_SLOT_VALUE                      \
  ((s32 (*)())Scene_GiveAndEquipItem)
#endif

// Forward declarations.
static void ColosseumSatrageMatch_IntroduceSecondFinals(s32 attendantId);
static void ColosseumSatrageMatch_ShowRoundStartLettering(s32 mode);
static void ColosseumSatrageMatch_ShowBattleResultLettering(s32 result);
static void MapLocalState_EmptyHook(void);
static void ColosseumSatrageMatch_WaitForOpponentFinish(void);
static s32 ColosseumSatrageMatch_ApproachFinishAndTakeItem(s32 firstItem,
                                                           s32 secondItem);
static void ColosseumSatrageMatch_StartOpponentPath(s32 pathResource);
static void ColosseumSatrageMatch_PushAdjacentObstacle(void);

/// The second finals match's actors, numbered in the order of
/// gColosseumSatrageMatchActorPlacements from ACTOR_FIRST_PLACED; the
/// rows of Isaac, Garet, Ivan and Mia take their own actors. By the
/// guide's and attendants' explanations, the Mini-Maze, the shifting floor
/// stage, the Log-Rolling Stage, The Wall and the Board Walk lead to the arena.
/// The stone pillars are drawn as the stone pillars Move pushes elsewhere.
enum ColosseumSatrageMatchActorId {
  /// Satrage, racing Isaac to the arena.
  /// ★ Named from the game: "My name is Satrage."
  /// (MsgColosseumWhoHaveComeQuestionMe)
  ACTOR_OPPONENT = ACTOR_FIRST_PLACED,
  /// The Board Walk's two stone pillars, on which Move does nothing.
  /// ★ Named from what the player sees: a stone pillar at the Board Walk's
  /// west end (its render)
  ACTOR_BOARD_WALK_WEST_PILLAR,
  /// ★ Named from what the player sees: a stone pillar at the Board Walk's
  /// east end (its render)
  ACTOR_BOARD_WALK_EAST_PILLAR,
  /// The stone pillar by The Wall, pushed or moved with Move.
  /// ★ Named from what the player sees: a stone pillar on the ground by The
  /// Wall (its render)
  ACTOR_WALL_PILLAR,
  /// The Mini-Maze's three stone pillars, pushed or moved with Move.
  /// ColosseumSatrageMatch_Initialize stands them west to east, at columns 73,
  /// 76 and 79, until they have been moved.
  /// ★ Named from what the player sees: three stone pillars in a row in the
  /// Mini-Maze (its render)
  ACTOR_MINI_MAZE_FIRST_PILLAR,
  /// The Board Walk's three logs, in the room of its stone pillars
  /// (https://alchemy.pascalpixel.com/places/colosseum-satrage-match/map-1.png):
  /// the western and eastern logs roll, and the center one never does
  /// (ColosseumSatrageMatch_UnusedRollCenterLogNorth).
  /// ★ Named from the game: "Roll those logs to open a path and follow it to
  /// the next stage." (MsgColossoPlaceCalledBoard)
  ACTOR_ROLLING_LOG_WEST = ACTOR_MINI_MAZE_FIRST_PILLAR + 3,
  /// ★ Named from the game: "Roll those logs to open a path and follow it to
  /// the next stage." (MsgColossoPlaceCalledBoard)
  ACTOR_ROLLING_LOG_EAST,
  /// ★ Named from the game: "The logs here are positioned to block your path."
  /// (MsgColossoPlaceCalledBoard)
  ACTOR_ROLLING_LOG_CENTER,
  /// The log Force strikes to knock ACTOR_FALLING_LOG down.
  /// ★ Named from what the player sees: a log (spr_259.png) above the Board
  /// Walk, which Force strikes (its render)
  ACTOR_FORCE_TARGET,
  /// The plant Growth grows up The Wall.
  /// ★ Named from what the player sees: a small sprout (SPRITE_SPROUT) at the
  /// foot of The Wall (its render)
  ACTOR_GROWTH_PLANT,
  /// The Log-Rolling Stage's two puddles, which Frost freezes; drawn as the
  /// puddle Frost freezes in Lunpa (lunpa.h's SPRITE_PUDDLE).
  /// ★ Named from what the player sees: a small puddle of water on the
  /// Log-Rolling Stage, the western of two (its render)
  ACTOR_PUDDLE_WEST,
  /// ★ Named from what the player sees: a small puddle of water on the
  /// Log-Rolling Stage, the eastern of two (its render)
  ACTOR_PUDDLE_EAST,
  /// The shifting floor stage's four lifts, from west to east, which the
  /// lifts' operator runs.
  /// ★ Named from the game: "The operator of the lifts will cheer for you
  /// here." (MsgColossoOperatorLiftsWillCheerFor)
  ACTOR_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT,
  /// ★ Named from the game: "The operator of the lifts will cheer for you
  /// here." (MsgColossoOperatorLiftsWillCheerFor)
  ACTOR_COLOSSEUM_SATRAGE_MATCH_LAST_LIFT =
          ACTOR_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT +
      3,
  /// Four platforms in the lifts' sprite laid across the water channel at the
  /// Board Walk's south edge, drawn underfoot; nothing but
  /// ColosseumSatrageMatch_Initialize touches them.
  /// ★ Named from what the player sees: four white platforms across a water
  /// channel (its render)
  ACTOR_BOARD_WALK_FIRST_PLATFORM,
  /// Knocked down onto the Board Walk by Force on ACTOR_FORCE_TARGET.
  /// ★ Named from what the player sees: a log (spr_259.png) that spins down
  /// onto the Board Walk when Force strikes (its render)
  ACTOR_FALLING_LOG = ACTOR_BOARD_WALK_FIRST_PLATFORM + 4,
  /// A young man with fair hair who works the shifting floor stage's lifts.
  /// ★ Named from the game: "The operator of the lifts will cheer for you
  /// here." (MsgColossoOperatorLiftsWillCheerFor)
  ACTOR_LIFT_OPERATOR,
  /// The Log-Rolling Stage's two logs.
  /// ★ Named from the game: "Ride the logs across the water to get to the
  /// other side." (MsgColossoLogRollingStage)
  ACTOR_RIDING_LOG_NORTH,
  /// ★ Named from the game: "Ride the logs across the water to get to the
  /// other side." (MsgColossoLogRollingStage)
  ACTOR_RIDING_LOG_SOUTH,
  /// A soldier who greets the party at the second finals match.
  /// ★ Named from the game: "This is the site of the second finals match."
  /// (MsgColossoSiteSecondFinals)
  ACTOR_GUIDE,
  /// The stages' attendants, young men with fair hair in white tunics.
  /// ★ Named from the game: "This stage is dubbed the Mini-Maze."
  /// (MsgColossoStageDubbedMini)
  ACTOR_MINI_MAZE_ATTENDANT,
  /// ★ Named from the game: "This is the shifting floor stage."
  /// (MsgColossoShiftingFloorStage)
  ACTOR_SHIFTING_FLOOR_ATTENDANT,
  /// ★ Named from the game: "Ride the logs across the water to get to the
  /// other side." (MsgColossoLogRollingStage)
  ACTOR_LOG_ROLLING_ATTENDANT,
  /// ★ Named from the game: "The fans just call this The Wall."
  /// (MsgColossoFansJustCallWall)
  ACTOR_WALL_ATTENDANT,
  /// ★ Named from the game: "This place is called the Board Walk."
  /// (MsgColossoPlaceCalledBoard)
  ACTOR_BOARD_WALK_ATTENDANT,
  /// The arena's prizes, drawn as item icons at two blank actors
  /// (Colosso_ShowItemIcon).
  /// ★ Named from what the player sees: the Chain Mail's icon at the arena's
  /// finish, taken by whoever reaches it first (its render)
  ACTOR_PRIZE_A,
  /// ★ Named from what the player sees: the Cotton Shirt's icon at the
  /// arena's finish, taken by whoever reaches it second (its render)
  ACTOR_PRIZE_B
};

/// The temporary flags that keep what the friends changed on the stages, for
/// the race (ColosseumSatrageMatch_Initialize restores them).
enum {
  /// ★ Named from the game: "This place is called the Board Walk."
  /// (MsgColossoPlaceCalledBoard)
  FLAG_OBSTACLE_KNOCKED_DOWN =
      FLAG_AREA_TEMP_30, ///< by Force, on the Board Walk
  /// ★ Named from the game: "Bestows Frost when equipped" (MsgItemPlainName)
  FLAG_WEST_TARGET_FROZEN = FLAG_AREA_TEMP_31, ///< by Frost
  /// ★ Named from the game: "Bestows Frost when equipped" (MsgItemPlainName)
  FLAG_EAST_TARGET_FROZEN = FLAG_AREA_TEMP_32, ///< by Frost
  /// ★ Named from the game: "Growth" (MsgAbilityName)
  FLAG_PLANT_GROWN = FLAG_AREA_TEMP_33, ///< by Growth, up The Wall
  /// ★ Named from the game: "Halt" (MsgAbilityName)
  FLAG_LIFTS_HALTED = FLAG_AREA_TEMP_34, ///< by Halt on ACTOR_LIFT_OPERATOR
  /// The stone pillar by The Wall stands at column 36.
  /// ★ Named from the game: "The fans just call this The Wall."
  /// (MsgColossoFansJustCallWall)
  FLAG_WALL_PILLAR_MOVED = FLAG_AREA_TEMP_35,
  /// The bytes of flags that keep the Mini-Maze's three pillars' columns.
  /// ★ Named from the game: "This stage is dubbed the Mini-Maze."
  /// (MsgColossoStageDubbedMini)
  FLAG_MINI_MAZE_FIRST_PILLAR_COLUMN = FLAG_AREA_TEMP_40,
  /// ★ Named from the game: "This stage is dubbed the Mini-Maze."
  /// (MsgColossoStageDubbedMini)
  FLAG_MINI_MAZE_SECOND_PILLAR_COLUMN = FLAG_AREA_TEMP_48,
  /// ★ Named from the game: "This stage is dubbed the Mini-Maze."
  /// (MsgColossoStageDubbedMini)
  FLAG_MINI_MAZE_THIRD_PILLAR_COLUMN = FLAG_AREA_TEMP_50,
  /// Someone has reached the arena
  /// (ColosseumSatrageMatch_ApproachFinishAndTakeItem).
  /// ★ Named from the game: "You must get through several stages before you
  /// reach the arena." (MsgColossoWarriorsEnterFinalsWithoutAny)
  FLAG_ARENA_REACHED = FLAG_SCENE_TEMP_11
};

/// Sets the party leader's walking speed and acceleration.
static inline void InitializeActorZero(void) {
  Actor_SetSpeedParameters(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
}

/// Sets the specified actor's walking speed and acceleration.
static inline void InitializeSelectedActor(s32 actorId) {
  Actor_SetSpeedParameters(actorId, PIXELS(1), PIXELS(1) / 2);
}

const struct SceneEntrance *Scene_GetEntrances(void) {
  return gColosseumSatrageMatchEntrances;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return 0; }

/// Returns the packed stage and battle-result exits.
const u32 *Scene_GetExits(void) { return gColosseumSatrageMatchExits; }

/// Returns Colosseum Final 2's warriors, obstacles and attendants.
const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gColosseumSatrageMatchActorPlacements;
}

/// Move on the Board Walk's two stone pillars does nothing.
static void ColosseumSatrageMatch_IgnoreMove(void) {}

/// Walking into a stone pillar pushes it. In Colosseum Final 2, walking into
/// one at step trigger 21 pushes the obstacle immediately ahead of the selected
/// actor.
static void ColosseumSatrageMatch_PushPillar(void) {
  ColosseumSatrageMatch_PushAdjacentObstacle();
}

/// Move on the stone pillar by The Wall, and after each push: its cell is
/// updated, and FLAG_WALL_PILLAR_MOVED keeps whether it stands at column 36.
static void ColosseumSatrageMatch_MoveWallPillar(void) {
  struct Actor *obstacle;
  s32 column;
  s32 row;

  obstacle = Actor_GetById(ACTOR_WALL_PILLAR);
  if ((obstacle->x.fixed >> 20) == 36) {
    GameFlag_Set(FLAG_WALL_PILLAR_MOVED);
    column = 0x23;
    row = 0x4D;
    MapGrid_CopyCellAttributes(0x23, 0x4E, 1, 1, column, row);
  } else {
    GameFlag_Clear(FLAG_WALL_PILLAR_MOVED);
    column = 0x23;
    row = 0x4D;
    MapGrid_CopyCellAttributes(0x22, 0x4D, 1, 1, column, row);
  }
}

/// Walking into the stone pillar by The Wall (step triggers 20 and 26) pushes
/// it, then updates its cell.
static void ColosseumSatrageMatch_PushWallPillar(void) {
  ColosseumSatrageMatch_PushAdjacentObstacle();
  ColosseumSatrageMatch_MoveWallPillar();
}

/// Move on a stone pillar in the Mini-Maze, and after each push: the three
/// pillars' columns are kept (FLAG_MINI_MAZE_FIRST_PILLAR_COLUMN and the two
/// after it) and their cells rebuilt.
static void ColosseumSatrageMatch_MoveMiniMazePillar(void) {

  struct Actor *obstacle;
  s32 column;

  MapGrid_CopyCellAttributes(100, 11, 12, 4, 14, 11);

  obstacle = Actor_GetById(ACTOR_MINI_MAZE_FIRST_PILLAR);
  column = obstacle->x.fixed >> 20;
  FieldGameFlag_SetByte(FLAG_MINI_MAZE_FIRST_PILLAR_COLUMN, column);
  MapGrid_CopyCellAttributes(71, 16, 1, 1, column, 16);

  obstacle = Actor_GetById(ACTOR_MINI_MAZE_FIRST_PILLAR + 1);
  column = obstacle->x.fixed >> 20;
  FieldGameFlag_SetByte(FLAG_MINI_MAZE_SECOND_PILLAR_COLUMN, column);
  MapGrid_CopyCellAttributes(71, 16, 1, 1, column, 16);

  obstacle = Actor_GetById(ACTOR_MINI_MAZE_FIRST_PILLAR + 2);
  column = obstacle->x.fixed >> 20;
  FieldGameFlag_SetByte(FLAG_MINI_MAZE_THIRD_PILLAR_COLUMN, column);
  MapGrid_CopyCellAttributes(71, 16, 1, 1, column, 16);
}

/// Force on the target over the Board Walk, before the Psynergy plays: the
/// Psynergy takes its child mode.
static void ColosseumSatrageMatch_PrepareForce(void) {
  u8 *effectWork = gFieldEffectWork;

  ((struct FieldEffectWork *)effectWork)->childMode = 1;
}

/// Force on the target over the Board Walk: the obstacle spins down into place
/// and the way past it opens (FLAG_OBSTACLE_KNOCKED_DOWN).
static void ColosseumSatrageMatch_KnockDownObstacle(void) {
  struct Actor *obstacle;
  s32 fallFrames;
  s32 rotationStep;
  s32 settleFrames;
  s32 settleRotationStep;
  struct Sprite *sprite;

  obstacle = Actor_GetById(ACTOR_FALLING_LOG);
  sprite = obstacle->sprite;
  Event_SetFlag(FLAG_OBSTACLE_KNOCKED_DOWN);
  obstacle->acceleration = 0x1999;
  obstacle->speedLimit = 0x13333;
  Sound_Play(SE_TOPPLE);
  // Spin through the fall, then settle the obstacle at its final height.
  FieldActor_SetMoveTarget(obstacle, PIXELS(336), PIXELS(10), PIXELS(264));
  for (rotationStep = 0, fallFrames = 9; fallFrames >= 0; fallFrames--) {
    sprite->rotation -= rotationStep;
    WaitFrames(1);
    rotationStep += 36;
  }
  FieldActor_SetMoveTarget(obstacle, PIXELS(330), -PIXELS(16), PIXELS(264));
  for (settleRotationStep = 0x168, settleFrames = 21; settleFrames >= 0;
       settleFrames--) {
    sprite->rotation -= settleRotationStep;
    WaitFrames(1);
    settleRotationStep += 36;
  }
  Actor_WaitForMove(obstacle);
  Event_WaitFrames(2);
  Sound_Play(SE_LOG_FALL);
  {
    s32 rotation = 0;

    sprite->rotation = rotation;
  }
  Import_ActorSetAnimation(ACTOR_FALLING_LOG, ANIM_SHAKE_HEAD);
  obstacle->x.fixed = PIXELS(336);
  obstacle->y.fixed = -PIXELS(8);
  obstacle->z.fixed = PIXELS(264);
  obstacle->velocityY = 0;
  obstacle->velocityX = 0;
  // Keep both the stage and its saved cell behavior strip in the new state.
  MapGrid_CopyCellAttributes(19, 16, 1, 1, 20, 16);
  MapGrid_CopyCellAttributes(20, 80, 1, 1, 21, 80);
}

/// Frost on the Log-Rolling Stage's western target: it freezes and its crossing
/// opens (FLAG_WEST_TARGET_FROZEN).
static void ColosseumSatrageMatch_FreezeWestTarget(void) {
  struct Actor *obstacle;

  GameFlag_Set(FLAG_WEST_TARGET_FROZEN);
  obstacle = Actor_GetById(ACTOR_PUDDLE_WEST);
  obstacle->motionFlags = 0;
  {

    MapGrid_CopyCellAttributes(46, 17, 1, 1, 44, 17);
  }
}

/// Frost on the Log-Rolling Stage's eastern target: it freezes and its crossing
/// opens (FLAG_EAST_TARGET_FROZEN).
static void ColosseumSatrageMatch_FreezeEastTarget(void) {
  struct Actor *obstacle;

  GameFlag_Set(FLAG_EAST_TARGET_FROZEN);
  obstacle = Actor_GetById(ACTOR_PUDDLE_EAST);
  obstacle->motionFlags = 0;
  {

    MapGrid_CopyCellAttributes(46, 17, 1, 1, 50, 17);
  }
}

/// Growth on the plant by The Wall: it grows into a strip that can be climbed
/// (FLAG_PLANT_GROWN).
static void ColosseumSatrageMatch_GrowPlant(void) {
  GameFlag_Set(FLAG_PLANT_GROWN);
  {

    MapGrid_CopyCellAttributes(32, 37, 1, 4, 32, 77);
  }
}

/// The selected actor jumps down off the ledge by an obstacle. Walking
/// south into the obstacle at step trigger 30 in Colosseum Final 2 plays the
/// hop the field plays at any ledge (FieldActor_JumpDownLedge) for the actor
/// touching it.
static void ColosseumSatrageMatch_JumpDownLedge(void) {
  FieldActor_JumpDownLedge(gAdventure.leaderActorId, 6, 0);
}

/// Walking west into the obstacle at step trigger 30 in Colosseum Final 2: the
/// leader jumps forward where he can (Field_TryJumpForward).
static void ColosseumSatrageMatch_JumpWestFromLedge(void) {
  Field_TryJumpForward();
}

/// Moves a riding log and its pusher to aligned cell centers.
static void ColosseumSatrageMatch_MovePusherAndLog(s32 logId, s32 offsetX,
                                                   s32 offsetZ) {

  struct Actor *pusher;
  struct Actor *log;
  s32 destinationX;
  s32 destinationZ;

  pusher = Actor_GetById(gAdventure.leaderActorId);
  log = Actor_GetById(logId);
  Event_Start();
  {
    destinationX =
        ((pusher->x.fixed + COLOSSEUM_SATRAGE_MATCH_OFFSET_FIXED(offsetX)) &
         0xFFF00000) +
        0x80000;
    destinationZ =
        ((pusher->z.fixed + COLOSSEUM_SATRAGE_MATCH_OFFSET_FIXED(offsetZ)) &
         0xFFF00000) +
        0x80000;

    pusher->speedLimit = 0x10000;
    pusher->acceleration = 0x8000;
    FieldActor_SetMoveTarget(pusher, destinationX, pusher->y.fixed,
                             destinationZ);
  }
  Actor_SelectAnimation(pusher, 27);
  {
    destinationX =
        ((log->x.fixed + COLOSSEUM_SATRAGE_MATCH_OFFSET_FIXED(offsetX)) &
         0xFFF00000) +
        0x80000;
    destinationZ =
        ((log->z.fixed + COLOSSEUM_SATRAGE_MATCH_OFFSET_FIXED(offsetZ)) &
         0xFFF00000) +
        0x80000;

    log->speedLimit = 0x10000;
    log->acceleration = 0x8000;
    FieldActor_SetMoveTarget(log, destinationX, log->y.fixed, destinationZ);
  }
  if (offsetX < 0 || offsetZ < 0) {
    Actor_SelectAnimation(log, 4);
  } else {
    Actor_SelectAnimation(log, 3);
  }
  Sound_Play(SE_LOG_PUSH);
  Actor_WaitForMove(pusher);
  Sound_Play(SE_STOP_ACTION_SE);
  Actor_SelectAnimation(log, 2);
  Event_End();
}

/// Standing on one of the Log-Rolling Stage's logs (step trigger 22): it
/// carries the rider across the water the way the player holds.
static void ColosseumSatrageMatch_RideLog(void) {
  struct Actor *pusher = Actor_GetById(gAdventure.leaderActorId);
  s32 column = pusher->x.fixed >> 20;
  s32 offsetX = 0;
  s32 logId = ACTOR_RIDING_LOG_NORTH;

  if (pusher->z.fixed >> 20 > 12) {
    logId = ACTOR_RIDING_LOG_SOUTH;
  }
  if (Actor_GetById(logId)->x.fixed >> 20 != column) {
    return;
  }
  if (column > 51) {
    if (gKeysHeld & KEY_LEFT) {
      offsetX = -COLOSSEUM_SATRAGE_MATCH_LOG_TRAVEL_PIXELS;
    }
  } else if (gKeysHeld & KEY_RIGHT) {
    offsetX = COLOSSEUM_SATRAGE_MATCH_LOG_TRAVEL_PIXELS;
  }
  if (offsetX != 0) {
    ColosseumSatrageMatch_MovePusherAndLog(logId, offsetX, 0);
    // Replace both log footprints after the moving pair has stopped.
    MapGrid_CopyCellAttributes(120, 10, 5, 6, 48, 10);
    column = Actor_GetById(ACTOR_RIDING_LOG_NORTH)->x.fixed >> 20;
    MapGrid_CopyCellAttributes(52, 28, 1, 3, column, 10);
    column = Actor_GetById(ACTOR_RIDING_LOG_SOUTH)->x.fixed >> 20;
    MapGrid_CopyCellAttributes(52, 28, 1, 3, column, 13);
  }
}

/// Rolls a log to half-cell coordinates while its pusher follows halfway.
static void ColosseumSatrageMatch_RollLogToCell(s32 logId, s32 halfCellX,
                                                s32 halfCellZ) {
  s32 pusher = gAdventure.leaderActorId;
  struct Actor *pusherActor = Actor_GetById(pusher);
  struct Actor *log = Actor_GetById(logId);
  s32 heading = (pusherActor->facing + 0x1000) & 0xe000;
  s32 alongX = (log->x.fixed >> 20) != halfCellX / 2;
  s32 offsetX;
  s32 offsetZ;

  halfCellX <<= 19;
  halfCellZ <<= 19;
  if (alongX) {
    offsetX = (halfCellX - log->x.fixed) / 2;
    offsetZ = 0;
  } else {
    offsetX = 0;
    offsetZ = (halfCellZ - log->z.fixed) / 2;
  }
  Event_Start();
  Import_ActorSetAnimation(pusher, 8);
  Event_WaitFrames(6);
  log->speedLimit = 0x8000;
  log->acceleration = 0x3333;
  Actor_SelectAnimation(log,
                        gColosseumSatrageMatch_RollLogScript[heading / 0x4000]);
  FieldActor_SetMoveTarget(log, halfCellX, 0, halfCellZ);
  Event_WaitFrames(6);
  Import_ActorSetAnimation(pusher, ANIM_WALK);
  // Follow halfway behind the rolling log while its finish effect runs.
  Actor_StartCamera(((struct FieldWork *)HeapSlot_AllocEwram(
                         HEAP_SLOT_FIELD, sizeof(struct FieldWork)))
                        ->camera,
                    (s32)log);
  Actor_SetSpeedParameters(pusher, PIXELS(1) / 2, PIXELS(1) / 5);
  Actor_SelectAnimation(pusherActor, 2);
  FieldActor_SetMoveTarget(pusherActor, pusherActor->x.fixed + offsetX, 0,
                           pusherActor->z.fixed + offsetZ);
  Sound_Play(SE_HEAVY_MOVE);
  Actor_WaitForMove(pusherActor);
  Actor_SelectAnimation(pusherActor, 1);
  Actor_WaitForMove(log);
  Sound_Play(SE_STOP_ACTION_SE);
  Sound_Play(SE_HEAVY_STOP);
  Actor_SelectAnimation(log, 1);
  Event_WaitFrames(15);
  Event_End();
}

/// Rebuilds the cell behaviors under the three rolling logs at their
/// positions.
static void ColosseumSatrageMatch_UpdateRollingLogCollision(void) {
  s32 cell;

  MapGrid_CopyCellAttributes(100, 11, 12, 4, 14, 11);

  cell = Actor_GetById(ACTOR_ROLLING_LOG_WEST)->x.fixed >> 20;
  MapGrid_CopyCellAttributes(13, 28, 1, 4, cell, 11);

  cell = Actor_GetById(ACTOR_ROLLING_LOG_EAST)->x.fixed >> 20;
  MapGrid_CopyCellAttributes(13, 28, 1, 4, cell, 11);

  cell = Actor_GetById(ACTOR_ROLLING_LOG_CENTER)->z.fixed >> 20;
  MapGrid_CopyCellAttributes(13, 28, 4, 1, 18, cell);
}

/// Walking west into the Board Walk's western log rolls it west. Walking into
/// the western log from the east (step trigger 24) or the west (25), or into
/// the eastern one from the east (28) or the west (27), rolls it the way the
/// leader walks.
static void ColosseumSatrageMatch_RollWestLogWest(void) {
  ColosseumSatrageMatch_RollLogToCell(ACTOR_ROLLING_LOG_WEST, 29, 26);
  ColosseumSatrageMatch_UpdateRollingLogCollision();
}

/// Walking east into the Board Walk's western log (step trigger 25) rolls it
/// east.
static void ColosseumSatrageMatch_RollWestLogEast(void) {
  ColosseumSatrageMatch_RollLogToCell(ACTOR_ROLLING_LOG_WEST, 33, 26);
  ColosseumSatrageMatch_UpdateRollingLogCollision();
}

/// Walking west into the Board Walk's eastern log (step trigger 28) rolls it
/// west.
static void ColosseumSatrageMatch_RollEastLogWest(void) {
  ColosseumSatrageMatch_RollLogToCell(ACTOR_ROLLING_LOG_EAST, 45, 26);
  ColosseumSatrageMatch_UpdateRollingLogCollision();
}

/// Walking east into the Board Walk's eastern log (step trigger 27) rolls it
/// east.
static void ColosseumSatrageMatch_RollEastLogEast(void) {
  ColosseumSatrageMatch_RollLogToCell(ACTOR_ROLLING_LOG_EAST, 49, 26);
  ColosseumSatrageMatch_UpdateRollingLogCollision();
}

/// Unused: no trigger row calls it, so the Board Walk's center log never
/// rolls. Walking into it would have rolled it north, as the western and
/// eastern logs roll west and east (ColosseumSatrageMatch_RollWestLogWest and
/// the rest).
static UNUSED void ColosseumSatrageMatch_UnusedRollCenterLogNorth(void) {
  ColosseumSatrageMatch_RollLogToCell(ACTOR_ROLLING_LOG_CENTER, 40, 23);
  ColosseumSatrageMatch_UpdateRollingLogCollision();
}

/// Unused: as ColosseumSatrageMatch_UnusedRollCenterLogNorth, rolling the
/// center log south.
static UNUSED void ColosseumSatrageMatch_UnusedRollCenterLogSouth(void) {
  ColosseumSatrageMatch_RollLogToCell(ACTOR_ROLLING_LOG_CENTER, 40, 25);
  ColosseumSatrageMatch_UpdateRollingLogCollision();
}

/// The shifting floor's task: carries the selected actor on the
/// lifts and alternates their routes every two seconds, until Halt stops the
/// operator.
static void ColosseumSatrageMatch_UpdateLifts(void) {
  s32 liftId;
  struct Actor *rider;
  struct Actor *lift;
  s32 liftX;
  s32 riderX;
  s32 deltaX;
  s32 liftZ;
  s32 riderZ;
  s32 deltaZ;
  struct Actor *liftActor;

  rider = Actor_GetById(gAdventure.leaderActorId);
  // Carry a rider only while both horizontal distances are less than ten
  // pixels.
  for (liftId = ACTOR_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT;
       liftId <= ACTOR_COLOSSEUM_SATRAGE_MATCH_LAST_LIFT; liftId++) {
    lift = Actor_GetById(liftId);
    lift->paused = 0;
    liftX = lift->x.fixed;
    riderX = rider->x.fixed;
    deltaX = liftX - riderX;
    if (deltaX >= 0) {
      if (deltaX > 0x9ffff) {
        continue;
      }
    } else {
      riderX -= liftX;
      if (riderX > 0x9ffff) {
        continue;
      }
    }
    liftZ = lift->z.fixed;
    riderZ = rider->z.fixed;
    deltaZ = liftZ - riderZ;
    if (deltaZ >= 0) {
      if (deltaZ > 0x9ffff) {
        continue;
      }
    } else {
      riderZ -= liftZ;
      if (riderZ > 0x9ffff) {
        continue;
      }
    }
    if (GameFlag_Get(FLAG_FIELD_HANDLING_REQUESTS) != 0) {
      riderZ = lift->z.fixed;
    } else {
      riderZ = rider->z.fixed;
      deltaZ = lift->velocityZ;
      riderZ += deltaZ;
    }
    rider->z.fixed = riderZ;
  }
  // The final lift's destination selects the cell behaviors for the row.
  if (gColosseumSatrageMatch_LiftTimer != 0 && lift->targetX == 0x80000000) {
    if (gColosseumSatrageMatch_LiftPhase == 0) {
      MapGrid_CopyCellAttributes(58, 28, 7, 1, 58, 13);
    } else {
      MapGrid_CopyCellAttributes(58, 10, 1, 1, 58, 11);
    }
  } else {
    MapGrid_CopyCellAttributes(57, 11, 1, 1, 58, 11);
    MapGrid_CopyCellAttributes(58, 14, 7, 1, 58, 13);
  }
  // Start the next alternating route when the two-second cycle wraps.
  if (gColosseumSatrageMatch_LiftTimer == 0) {
    gColosseumSatrageMatch_LiftPhase ^= 1;
    if (gColosseumSatrageMatch_LiftPhase != 0) {
      liftActor = Actor_GetById(ACTOR_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT);
      FieldActor_SetMoveTarget(liftActor, PIXELS(936), 0, PIXELS(184));
      liftActor = Actor_GetById(ACTOR_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT + 1);
      FieldActor_SetMoveTarget(liftActor, PIXELS(968), 0, PIXELS(248));
      liftActor = Actor_GetById(ACTOR_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT + 2);
      FieldActor_SetMoveTarget(liftActor, PIXELS(1000), 0, PIXELS(184));
      liftActor = Actor_GetById(ACTOR_COLOSSEUM_SATRAGE_MATCH_LAST_LIFT);
      FieldActor_SetMoveTarget(liftActor, PIXELS(1032), 0, PIXELS(248));
      Import_ActorSetAnimation(ACTOR_LIFT_OPERATOR, 11);
    } else {
      liftActor = Actor_GetById(ACTOR_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT);
      FieldActor_SetMoveTarget(liftActor, PIXELS(936), 0, PIXELS(216));
      liftActor = Actor_GetById(ACTOR_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT + 1);
      FieldActor_SetMoveTarget(liftActor, PIXELS(968), 0, PIXELS(216));
      liftActor = Actor_GetById(ACTOR_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT + 2);
      FieldActor_SetMoveTarget(liftActor, PIXELS(1000), 0, PIXELS(216));
      liftActor = Actor_GetById(ACTOR_COLOSSEUM_SATRAGE_MATCH_LAST_LIFT);
      FieldActor_SetMoveTarget(liftActor, PIXELS(1032), 0, PIXELS(216));
      Import_ActorSetAnimation(ACTOR_LIFT_OPERATOR, 10);
    }
  }
  gColosseumSatrageMatch_LiftTimer++;
  if ((u32)gColosseumSatrageMatch_LiftTimer >=
      COLOSSEUM_SATRAGE_MATCH_LIFT_CYCLE_FRAMES) {
    if (GameFlag_Get(FLAG_FIELD_HANDLING_REQUESTS) == 0) {
      gColosseumSatrageMatch_LiftTimer = 0;
    }
  }
}

/// Stops the moving-lift task and restores its starting row.
static void ColosseumSatrageMatch_ResetLifts(void) {
  gColosseumSatrageMatch_LiftTimer = 0;
  gColosseumSatrageMatch_LiftPhase = 0;
  Task_Remove(ColosseumSatrageMatch_UpdateLifts);
  Actor_SetPosition(ACTOR_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT, PIXELS(936),
                    PIXELS(216));
  Actor_SetPosition(ACTOR_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT + 1, PIXELS(968),
                    PIXELS(216));
  Actor_SetPosition(ACTOR_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT + 2, PIXELS(1000),
                    PIXELS(216));
  Actor_SetPosition(ACTOR_COLOSSEUM_SATRAGE_MATCH_LAST_LIFT, PIXELS(1032),
                    PIXELS(216));
  Import_ActorSetAnimation(ACTOR_LIFT_OPERATOR, 10);
}

/// Starts the moving-lift task and releases its pause flag.
static void ColosseumSatrageMatch_StartLifts(void) {
  Task_AddFrame(ColosseumSatrageMatch_UpdateLifts, TASK_PASS_FRAME + 5);
  GameFlag_Clear(FLAG_FIELD_HANDLING_REQUESTS);
}

/// Waits for the demonstration's crossing phase, with a ten-second timeout.
static void ColosseumSatrageMatch_WaitForLiftCrossingPhase(void) {
  s32 elapsedFrames;

  WaitFrames(10);

  elapsedFrames = 0;
  while (gColosseumSatrageMatch_LiftPhase != 0 ||
         gColosseumSatrageMatch_LiftTimer !=
             COLOSSEUM_SATRAGE_MATCH_LIFT_CROSSING_FRAME) {
    WaitFrames(1);
    elapsedFrames++;
    if (elapsedFrames >= COLOSSEUM_SATRAGE_MATCH_DEMONSTRATION_TIMEOUT_FRAMES) {
      return;
    }
  }
}

/// Halt on the shifting floor's lift operator: the lifts stop and leave a
/// crossing (FLAG_LIFTS_HALTED).
static void ColosseumSatrageMatch_HaltLifts(void) {
  FieldPsynergy_HaltActor(ACTOR_LIFT_OPERATOR);
  GameFlag_Set(FLAG_LIFTS_HALTED);

  if (gColosseumSatrageMatch_LiftPhase != 0) {
    gColosseumSatrageMatch_LiftTimer = 0;
  }

  WaitFrames(30);
  WaitFrames(1);

  Task_Remove(ColosseumSatrageMatch_UpdateLifts);

  MapGrid_CopyCellAttributes(58, 28, 7, 1, 58, 13);
  MapGrid_CopyCellAttributes(57, 11, 1, 1, 58, 11);
}

/// Reaching Colosseum Final 2's arena: the leader takes a prize (the Chain Mail
/// if first, the Cotton Shirt if second), and he and Satrage face off and
/// fight; the battle's group follows who took the mail. The arena's step
/// trigger row waits on FLAG_COLOSSEUM_ARENA_CLOSED being clear, which nothing
/// in the game sets (colosseum_azart_match.c's
/// ColosseumSatrageMatch_ReachArena).
static void ColosseumSatrageMatch_ReachArena(void) {
  s32 opponentFinishedFirst;
  s32 refreshCount;
  s32 resultBase;

  resultBase = 2;
  MapLocalState_EmptyHook();
  Event_Start();
  opponentFinishedFirst = ColosseumSatrageMatch_ApproachFinishAndTakeItem(
      ITEM_CHAIN_MAIL, ITEM_COTTON_SHIRT);
  ColosseumSatrageMatch_WaitForOpponentFinish();
  refreshCount = 9;
  do {
    Actor_RefreshSelectorById(ACTOR_OPPONENT);
    refreshCount = (refreshCount - 1);
  } while (refreshCount >= 0);
  // Bring the warriors together before selecting the battle's result bias.
  Actor_SetSpeed(ACTOR_OPPONENT, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkTo(ACTOR_OPPONENT, 88, 256);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 120, 256);
  Import_ActorSetAnimation(ACTOR_OPPONENT, ANIM_STAND);
  Actor_TurnToFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_OPPONENT, 0);
  Event_WaitFrames(10);
  Import_ActorSetAnimation(ACTOR_OPPONENT, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_WaitFrames(20);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(2), PIXELS(1));
  Actor_SetSpeed(ACTOR_OPPONENT, PIXELS(2), PIXELS(1));
  Actor_WalkTo(ACTOR_PARTY_LEADER, 112, 256);
  Actor_SetPositionAndReset(ACTOR_OPPONENT, 96, 0x100);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_STRIDE);
  Import_ActorSetAnimation(ACTOR_OPPONENT, WARRIOR_ANIMATION_FIGHTING_STANCE);
  Event_WaitFrames(10);
  /* FAKEMATCH(temporary): resultBase, set to 2 at the top, keeps the + 1
   * unfolded; as 3 - opponentFinishedFirst, which shares r5's 3 with
   * Battle_SetType(BATTLE_TYPE_SCRIPTED), 3 of 106 instructions differ in
   * tbs-en. */
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_COLOSSO,
                                        resultBase - opponentFinishedFirst + 1);
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  Event_SetAfterBattleDestination((s32)&SceneId_ColosseumSatrageMatch,
                                  COLOSSEUM_SATRAGE_MATCH_AFTER_MATCH_WON);
  Event_SetDefeatDestination((s32)&SceneId_ColosseumSatrageMatch,
                             COLOSSEUM_SATRAGE_MATCH_AFTER_MATCH_LOST);
  GameFlag_Set(FLAG_KEEP_MUSIC_ONCE);
}

/// Returns Colosseum Final 2's obstacle interactions and attendant dialogue.
const struct Trigger *Scene_GetTriggers(void) {
  return gColosseumSatrageMatchTriggers;
}

/// Back from the battle (COLOSSEUM_SATRAGE_MATCH_AFTER_MATCH_WON or _LOST):
/// Isaac and Satrage stand in the arena as the result is shown.
static void ColosseumSatrageMatch_StageBattleResult(s32 result) {

  Actor_ReleaseById(ACTOR_PRIZE_A);
  Actor_ReleaseById(ACTOR_PRIZE_B);
  Party_RestoreHpPp(1);
  Event_Start();
  Actor_SetPosition(ACTOR_OPPONENT, PIXELS(88), PIXELS(256));
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(120), PIXELS(256));
  // BUG: FACING_SOUTH is passed where Actor_FaceActor takes an
  // actor id; Actor_Lookup has no actor past 0xbf, so
  // neither turns.
#ifdef BUGFIX
  Actor_FaceDirection(ACTOR_OPPONENT, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
#else
  Actor_FaceActor(ACTOR_OPPONENT, FACING_SOUTH, 0);
  Actor_FaceActor(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
#endif
  if (result < 0) {
    Import_ActorSetAnimation(ACTOR_OPPONENT, WARRIOR_ANIMATION_ARMS_RAISED);
    Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_CROUCH);
  } else {
    Import_ActorSetAnimation(ACTOR_OPPONENT, WARRIOR_ANIMATION_DEFEATED);
    Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_ARMS_RAISED);
  }
  WaitFrames(1);
  FieldCamera_MoveTo(PIXELS(104), 0, PIXELS(192), 0);
  ColosseumSatrageMatch_ShowBattleResultLettering(result);
  Event_End();
}

/// Sets up Colosseum Final 2: restores obstacle progress, then by the arrival:
/// the race (COLOSSEUM_SATRAGE_MATCH_RACE_FROM_LEND_A_HAND), the friends' turns
/// to lend a hand (COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_EAST), the guide's
/// tour (COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_WEST), or the match won
/// (COLOSSEUM_SATRAGE_MATCH_AFTER_MATCH_WON) or lost
/// (COLOSSEUM_SATRAGE_MATCH_AFTER_MATCH_LOST).
s32 ColosseumSatrageMatch_Initialize(void) {
  struct Actor *actor;
  s32 x;
  s32 obstacleId;
  s32 column;
  s32 liftId;
  s32 columnOffset;
  s32 row;
  s32 zero;

  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_FADE, 0);
  // Restore the cell behavior templates before applying saved puzzle
  // progress.
  GameFlag_Set(FLAG_RETREAT_BLOCKED);
  MapGrid_CopyCellAttributes(14, 11, 12, 4, 100, 11);
  MapGrid_CopyCellAttributes(48, 10, 5, 6, 120, 10);
  for (obstacleId = ACTOR_BOARD_WALK_FIRST_PLATFORM;
       obstacleId <= ACTOR_FALLING_LOG; obstacleId++) {
    actor = Actor_GetById(obstacleId);
    Actor_SelectAnimation(actor, 4);
    actor->motionFlags = 0;
    actor->y.fixed = 0;
    actor->priorityFlags = 2;
  }
  Actor_GetById(ACTOR_FORCE_TARGET)->priorityFlags = 2;
  if (GameFlag_Get(FLAG_OBSTACLE_KNOCKED_DOWN)) {
    actor = Actor_GetById(ACTOR_FALLING_LOG);
    actor->x.fixed = PIXELS(336);
    actor->y.fixed = -PIXELS(8);
    actor->z.fixed = PIXELS(264);
    MapGrid_CopyCellAttributes(19, 16, 1, 1, 20, 16);
    MapGrid_CopyCellAttributes(20, 80, 1, 1, 21, 80);
  } else {
    actor = Actor_GetById(ACTOR_FALLING_LOG);
    Actor_SelectAnimation(actor, 3);
    actor->y.fixed = PIXELS(16);
  }
  Actor_GetById(ACTOR_WALL_PILLAR)->priorityFlags = 2;
  zero = 0;
  if (GameFlag_Get(FLAG_WALL_PILLAR_MOVED)) {
    MapGrid_CopyCellAttributes(35, 78, 1, 1, 35, 77);
  }
  if (GameFlag_Get(FLAG_PLANT_GROWN)) {
    Import_ActorSetAnimation(ACTOR_GROWTH_PLANT, ANIM_SHAKE_HEAD);
    MapGrid_CopyCellAttributes(32, 37, 1, 4, 32, 77);
  }
  if (GameFlag_Get(FLAG_WEST_TARGET_FROZEN)) {
    Actor_GetById(ACTOR_PUDDLE_WEST)->motionFlags = zero;
    Actor_GetById(ACTOR_PUDDLE_WEST)->priorityFlags = 2;
    Import_ActorSetAnimation(ACTOR_PUDDLE_WEST, 5);
    MapGrid_CopyCellAttributes(46, 17, 1, 1, 44, 17);
  }
  if (GameFlag_Get(FLAG_EAST_TARGET_FROZEN)) {
    Actor_GetById(ACTOR_PUDDLE_EAST)->motionFlags = zero;
    Actor_GetById(ACTOR_PUDDLE_EAST)->priorityFlags = 2;
    Import_ActorSetAnimation(ACTOR_PUDDLE_EAST, 5);
    MapGrid_CopyCellAttributes(46, 17, 1, 1, 50, 17);
  }
  actor = Actor_GetById(ACTOR_RIDING_LOG_NORTH);
  column = actor->x.fixed >> 20;
  actor->motionFlags = zero;
  actor->priorityFlags = 2;
  MapGrid_CopyCellAttributes(52, 28, 1, 3, column, 10);
  actor = Actor_GetById(ACTOR_RIDING_LOG_SOUTH);
  column = actor->x.fixed >> 20;
  actor->motionFlags = zero;
  actor->priorityFlags = 2;
  MapGrid_CopyCellAttributes(52, 28, 1, 3, column, 13);

  // Each Mini-Maze pillar returns to the column its area temp flag byte
  // keeps.
  x = FieldGameFlag_GetByte(FLAG_MINI_MAZE_FIRST_PILLAR_COLUMN);
  if (x == 0) {
    x = 73;
  }
  actor = Actor_GetById(ACTOR_MINI_MAZE_FIRST_PILLAR);
  actor->x.fixed = (x << 20) + 0x80000;
  actor->motionFlags = zero;
  actor->priorityFlags = 2;
  MapGrid_CopyCellAttributes(71, 16, 1, 1, x, 16);
  x = FieldGameFlag_GetByte(FLAG_MINI_MAZE_SECOND_PILLAR_COLUMN);
  if (x == 0) {
    x = 76;
  }
  actor = Actor_GetById(ACTOR_MINI_MAZE_FIRST_PILLAR + 1);
  actor->x.fixed = (x << 20) + 0x80000;
  actor->motionFlags = zero;
  actor->priorityFlags = 2;
  MapGrid_CopyCellAttributes(71, 16, 1, 1, x, 16);
  x = FieldGameFlag_GetByte(FLAG_MINI_MAZE_THIRD_PILLAR_COLUMN);
  if (x == 0) {
    x = 79;
  }
  actor = Actor_GetById(ACTOR_MINI_MAZE_FIRST_PILLAR + 2);
  actor->x.fixed = (x << 20) + 0x80000;
  actor->motionFlags = zero;
  actor->priorityFlags = 2;
  MapGrid_CopyCellAttributes(71, 16, 1, 1, x, 16);

  ColosseumSatrageMatch_UpdateRollingLogCollision();
  // With FLAG_LIFTS_HALTED set the lifts stay where they are; otherwise their
  // task resumes.
  Import_ActorSetAnimation(ACTOR_LIFT_OPERATOR, 10);
  if (GameFlag_Get(FLAG_LIFTS_HALTED)) {
    for (liftId = ACTOR_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT, row = 13,
        columnOffset = 58;
         liftId <= ACTOR_COLOSSEUM_SATRAGE_MATCH_LAST_LIFT;
         liftId++, columnOffset += 2) {
      actor = Actor_GetById(liftId);
      actor->priorityFlags = 2;
      Actor_SelectAnimation(actor, 4);
      MapGrid_CopyCellAttributes(56, 13, 1, 1, columnOffset, row);
    }
    Import_ActorSetAnimation(ACTOR_LIFT_OPERATOR, 10);
    FieldPsynergy_HaltActor(ACTOR_LIFT_OPERATOR);
  } else {
    for (liftId = ACTOR_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT;
         liftId <= ACTOR_COLOSSEUM_SATRAGE_MATCH_LAST_LIFT; liftId++) {
      actor = Actor_GetById(liftId);
      actor->priorityFlags = 2;
      Actor_SelectAnimation(actor, 4);
      actor->speedLimit = 0x8000;
      actor->acceleration = 0x3333;
    }
    Task_AddFrame(ColosseumSatrageMatch_UpdateLifts, TASK_PASS_FRAME + 5);
    Task_SetFlags(ColosseumSatrageMatch_UpdateLifts, 1);
  }
  Import_ActorSetAnimation(ACTOR_OPPONENT, WARRIOR_ANIMATION_FIGHTING_STANCE);
  gAdventure.movementMode = LEADER_MOVEMENT_WALK;
  Colosso_ShowItemIcon(ACTOR_PRIZE_B, ITEM_COTTON_SHIRT);
  Colosso_ShowItemIcon(ACTOR_PRIZE_A, ITEM_CHAIN_MAIL);
  Actor_ConfigureSpriteColor(ACTOR_OPPONENT, SPRITE_COLOR_GRB);
  // The race, the friends' turns, the tour, and the battle's result.
  switch (gAdventure.entrance) {
  case COLOSSEUM_SATRAGE_MATCH_RACE_FROM_LEND_A_HAND:
    Scene_BuildDescriptorAndInstallTask(ACTOR_PARTY_LEADER, ACTOR_OPPONENT, 5,
                                        PIXELS(104), PIXELS(256), ACTOR_PRIZE_A,
                                        ACTOR_PRIZE_B);
    MapGrid_CopyCellsLayered(127, 0, 1, 2, 79, 6);
    Actor_ReleaseById(ACTOR_GUIDE);
    Actor_ReleaseById(ACTOR_MINI_MAZE_ATTENDANT);
    Actor_ReleaseById(ACTOR_SHIFTING_FLOOR_ATTENDANT);
    Actor_ReleaseById(ACTOR_LOG_ROLLING_ATTENDANT);
    Actor_ReleaseById(ACTOR_WALL_ATTENDANT);
    Actor_ReleaseById(ACTOR_BOARD_WALK_ATTENDANT);
    if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
      Sound_Play(SOUND_FADE_OUT_BGM);
      Colosso_ControlCharacterAlone(ISAAC);
      Colosso_PlaceFriendsAtCheeringSpots();
      ColosseumSatrageMatch_ShowRoundStartLettering(2);
    }
    Actor_WatchAndSetScript(ACTOR_GARET, ACTOR_PARTY_LEADER);
    Actor_WatchAndSetScript(ACTOR_IVAN, ACTOR_PARTY_LEADER);
    Actor_WatchAndSetScript(ACTOR_MIA, ACTOR_PARTY_LEADER);
    ColosseumSatrageMatch_StartOpponentPath((s32)&ResourceId_OpponentPathB);
    break;
  case COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_EAST:
    Task_AddFrame(Colosso_RaiseTakePlaceOnPsynergy, TASK_PASS_FRAME);
    Actor_ReleaseById(ACTOR_PRIZE_A);
    Actor_ReleaseById(ACTOR_PRIZE_B);
    if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
      Colosso_PlaceFriendsAtCheeringSpots();
      Colosso_ControlCharacterAlone(GARET);
      ColosseumSatrageMatch_ShowRoundStartLettering(0);
    }
    break;
  case COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_WEST:
    if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
      ColosseumSatrageMatch_IntroduceSecondFinals(ACTOR_GUIDE);
      Colosso_ClearCheeringSpots();
    }
    break;
  case COLOSSEUM_SATRAGE_MATCH_AFTER_MATCH_WON:
    ColosseumSatrageMatch_StageBattleResult(2);
    Import_EventSetExitRequest(COLOSSEUM_SATRAGE_MATCH_TO_COLOSSEUM_NORTH);
    break;
  case COLOSSEUM_SATRAGE_MATCH_AFTER_MATCH_LOST:
    ColosseumSatrageMatch_StageBattleResult(-2);
    Import_EventSetExitRequest(COLOSSEUM_SATRAGE_MATCH_TO_BABI_PALACE);
    break;
  }
  return 0;
}

/// The tour of Colosseum Final 2 (COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_WEST):
/// the guide welcomes the party to the second finals match's stages and leaves
/// them to their attendants, then Garet, Ivan and Mia walk back into Isaac.
static void ColosseumSatrageMatch_IntroduceSecondFinals(s32 attendantId) {
  s32 attendantZ;
  s32 attendantX;
  struct Actor *actor;

  actor = Actor_GetById(attendantId);
  attendantX = actor->x.part.pixel;
  attendantZ = actor->z.part.pixel;
  Event_Start();
  Actor_SetSpeed(attendantId, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_IVAN, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_MIA, PIXELS(1), PIXELS(1) / 2);
  Import_ActorSetPosition(ACTOR_PARTY_LEADER, (attendantX << 16),
                          ((attendantZ << 16) - 0x300000));
  Import_ActorSetPosition(ACTOR_GARET, (attendantX << 16) - 0x100000,
                          (attendantZ << 16) - 0x280000);
  Import_ActorSetPosition(ACTOR_IVAN, (attendantX << 16) + 0x100000,
                          (attendantZ << 16) - 0x280000);
  Import_ActorSetPosition(ACTOR_MIA, (attendantX << 16),
                          ((attendantZ << 16) - 0x200000));
  Import_ActorSetPosition(attendantId, (attendantX << 16),
                          ((attendantZ << 16) - 0x500000));
  actor = Actor_GetById(ACTOR_PARTY_LEADER);
  actor->facing = FACING_NORTH;
  FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_SetMessageId(MsgColossoSiteSecondFinals);
  Actor_SetAnimationAndWait(attendantId, ANIM_NOD);
  Event_ShowMessageAndWait(attendantId, 0);
  Actor_RunRepeatedMotion(attendantId, 2);
  Event_ShowMessageAndWait(attendantId, 0);
  Actor_RunRepeatedMotion(attendantId, 2);
  Event_ShowMessageAndWait(attendantId, 0);
  Actor_RunRepeatedMotion(attendantId, 2);
  Event_ShowMessageAndWait(attendantId, 0);
  // Gather the companions into Isaac before the attendant returns to his post.
  Import_ActorSetAnimation(ACTOR_MIA, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_WaitFrames(6);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_WALK);
  actor = Actor_GetById(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Actor_ResetAndSetPosition(ACTOR_GARET, actor->x.part.pixel,
                              actor->z.part.pixel);
  }
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_WALK);
  actor = Actor_GetById(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Actor_ResetAndSetPosition(ACTOR_IVAN, actor->x.part.pixel,
                              actor->z.part.pixel);
  }
  Import_ActorSetAnimation(ACTOR_MIA, ANIM_WALK);
  actor = Actor_GetById(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Actor_ResetAndSetPosition(ACTOR_MIA, actor->x.part.pixel,
                              actor->z.part.pixel);
  }
  Actor_SetPositionAndReset(attendantId, (attendantX - 16), (attendantZ - 64));
  Import_ActorSetPosition(ACTOR_GARET, 0, 0);
  Import_ActorSetPosition(ACTOR_IVAN, 0, 0);
  Import_ActorSetPosition(ACTOR_MIA, 0, 0);
  Actor_SetPositionAndReset(attendantId, (attendantX - 16), (attendantZ - 16));
  Actor_SetPositionAndReset(attendantId, attendantX, attendantZ);
  Actor_ArmCallback(attendantId, FACING_NORTH, 10);
  Event_End();
}

/// The Mini-Maze's attendant, stage 1 of Colosseum Final 2: the first time,
/// Isaac appears and runs the maze; then the objective, and a place for a
/// friend to cheer. With a friend alone on the stages
/// (COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_EAST) he asks whether they are done
/// cheering instead.
static void ColosseumSatrageMatch_TalkToMiniMazeAttendant(s32 attendant) {
  s32 result;
  struct Actor *actor;

  if (gAdventure.entrance == COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_EAST) {
    Colosso_AskDoneCheering(attendant);
  } else {
    Event_Start();
    result = Colosso_GreetAtStage(attendant, 1);
    if (result == COLOSSO_GUIDE_DESCRIBE) {
      Event_SetMessageId(MsgColossoStageDubbedMini);
      FieldCamera_SetSpeedParameters(0x30000, 0x6000);
      FieldCamera_MoveWithinMapBounds(PIXELS(1224), -1, PIXELS(184), 1);
      FieldCamera_WaitForArrival();
      Event_ShowMessageAndWait(attendant, 0);
      Colosso_FadeInWarrior(ACTOR_PARTY_LEADER, 1272, 168);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 2, PIXELS(3) / 4);
      Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 1288, 184);
      Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 1288, 216);
      Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 1224, 216);
      Event_ShowMessageAndWait(attendant, 0);
      Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 1224, 248);
      Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 1192, 248);
      Event_WaitFrames(3);
      actor = Actor_GetById(ACTOR_PARTY_LEADER);
      actor->velocityY = 0x40000;
      Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_ARMS_RAISED);
      FieldEffect_SetAttached(ACTOR_PARTY_LEADER,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      Event_WaitFrames(30);
      Event_ShowMessageAndWait(attendant, 0);
      Colosso_RestoreWarrior(ACTOR_PARTY_LEADER);
      FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
      Colosso_RepeatStageNumber(attendant, 1);
    } else if (result == COLOSSO_GUIDE_OFFER_CHEERING) {
      Event_SetMessageId(MsgColossoMainThingStage);
      Event_ShowMessageAndWait(attendant, 0);
    }
    Colosso_OfferCheeringSpot(result, attendant, 1);
    Event_End();
  }
}

/// The shifting floor stage's attendant, stage 2 of Colosseum Final 2: the
/// first time, Isaac appears and rides the lifts; then, that the lifts'
/// operator will cheer for Isaac, and a place for a friend to cheer.
static void ColosseumSatrageMatch_TalkToShiftingFloorAttendant(s32 attendant) {
  s32 result;

  if (gAdventure.entrance == COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_EAST) {
    Colosso_AskDoneCheering(attendant);
  } else {
    Event_Start();
    result = Colosso_GreetAtStage(attendant, 2);
    if (result == COLOSSO_GUIDE_DESCRIBE) {
      Event_SetMessageId(MsgColossoShiftingFloorStage);
      ColosseumSatrageMatch_ResetLifts();
      FieldCamera_SetSpeed(0x30000, 0x6000);
      FieldCamera_MoveTo(PIXELS(984), -1, PIXELS(232), 1);
      FieldCamera_WaitForArrival();
      /* FAKEMATCH(wrapper): called through a cast to the s32 call the bytes
       * show, which sets r1 before r0; as a direct call, 1 of 104
       * instructions differ in tbs-en. */
      COLOSSEUM_SATRAGE_MATCH_SHOW_MESSAGE(attendant, 0);
      ColosseumSatrageMatch_StartLifts();
      Event_ShowMessage(attendant, 0);
      Colosso_FadeInWarrior(ACTOR_PARTY_LEADER, 1080, 264);
      Event_WaitFrames(15);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 2, PIXELS(3) / 4);
      Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 1080, 216);
      Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 1064, 216);
      ColosseumSatrageMatch_WaitForLiftCrossingPhase();
      Field_TryJumpForward();
      FieldCamera_MoveTo(-1, -1, -1, 0);
      Event_ShowMessage(attendant, 0);
      Colosso_RestoreWarrior(ACTOR_PARTY_LEADER);
      FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
      Colosso_RepeatStageNumber(attendant, 2);
    } else {
      if (result == COLOSSO_GUIDE_OFFER_CHEERING) {
        Event_SetMessageId(MsgColossoOperatorLiftsWillCheerFor);
        Event_ShowMessage(attendant, 0);
      }
    }
    Colosso_OfferCheeringSpot(result, attendant, 2);
    Event_End();
  }
}

/// The Log-Rolling Stage's attendant, stage 3 of Colosseum Final 2: the first
/// time, Isaac appears and rides a log across; then the objective, and a place
/// for a friend to cheer.
static void ColosseumSatrageMatch_TalkToLogRollingAttendant(s32 attendant) {
  s32 result;

  if (gAdventure.entrance == COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_EAST) {
    Colosso_AskDoneCheering(attendant);
  } else {
    Event_Start();
    result = Colosso_GreetAtStage(attendant, 3);
    if (result == COLOSSO_GUIDE_DESCRIBE) {
      Event_SetMessageId(MsgColossoLogRollingStage);
      FieldCamera_SetSpeed(0x30000, 0x6000);
      FieldCamera_MoveTo(PIXELS(752), -1, PIXELS(192), 1);
      FieldCamera_WaitForArrival();
      Event_WaitFrames(60);
      FieldCamera_SetSpeed(0x10000, 0x2000);
      FieldCamera_MoveTo(PIXELS(752), -1, PIXELS(224), 1);
      FieldCamera_WaitForArrival();
      Event_ShowMessage(attendant, 0);
      Colosso_FadeInWarrior(ACTOR_PARTY_LEADER, 856, 264);
      Event_WaitFrames(10);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 2, PIXELS(3) / 4);
      Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 856, 264);
      Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 856, 232);
      Event_ShowMessage(attendant, 0);
      Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 840, 232);
      Event_WaitFrames(10);
      /* FAKEMATCH(wrapper): called through a cast to the s32 call the bytes
       * show, which sets r0 last; as a direct call, 1 of 163 instructions
       * differ in tbs-en. */
      COLOSSEUM_SATRAGE_MATCH_MOVE_ACTOR_PAIR(ACTOR_RIDING_LOG_SOUTH, -64, 0);
      FieldCamera_MoveTo(PIXELS(752), -1, PIXELS(216), 1);
      Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
      Event_WaitFrames(10);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 760, 232);
      Event_WaitFrames(10);
      Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 30);
      Event_ShowMessage(attendant, 0);
      Colosso_RestoreWarrior(ACTOR_PARTY_LEADER);
      FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
      Actor_SetPosition(ACTOR_RIDING_LOG_SOUTH, PIXELS(840), PIXELS(232));
      Colosso_RepeatStageNumber(attendant, 3);
    } else if (result == COLOSSO_GUIDE_OFFER_CHEERING) {
      Event_SetMessageId(MsgColossoHereYourObjectiveRideLogs);
      Event_ShowMessage(attendant, 0);
    }
    Colosso_OfferCheeringSpot(result, attendant, 3);
    Event_End();
  }
}

/// The Wall's attendant, stage 4 of Colosseum Final 2: the first time, Isaac
/// appears and climbs it; then the objective, and a place for a friend to
/// cheer.
static void ColosseumSatrageMatch_TalkToWallAttendant(s32 attendant) {
  s32 z;
  struct Actor *actor;
  s32 result;

  if (gAdventure.entrance == COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_EAST) {
    Colosso_AskDoneCheering(attendant);
    return;
  }
  Event_Start();
  result = Colosso_GreetAtStage(attendant, 4);
  if (result == COLOSSO_GUIDE_DESCRIBE) {
    Event_SetMessageId(MsgColossoFansJustCallWall);
    FieldCamera_SetSpeedParameters(0x30000, 0x6000);
    FieldCamera_MoveTo(PIXELS(536), -1, PIXELS(240), 1);
    FieldCamera_WaitForArrival();
    Event_WaitFrames(45);
    FieldCamera_SetSpeed(0x10000, 0x2000);
    FieldCamera_MoveTo(PIXELS(536), -1, PIXELS(192), 1);
    z = 264;
    FieldCamera_WaitForArrival();
    Event_ShowMessageAndWait(attendant, 0);
    Colosso_FadeInWarrior(ACTOR_PARTY_LEADER, 632, z);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 616, z);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
    Field_ClimbUp();
    FieldCamera_SetSpeed(0x4000, 0x800);
    FieldCamera_MoveTo(PIXELS(536), -1, PIXELS(160), 1);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
    Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_CLIMB);
    actor = Actor_GetById(ACTOR_PARTY_LEADER);
    FieldActor_SetMoveTarget(actor, actor->x.fixed, actor->y.fixed + 0x400000,
                             actor->z.fixed);
    Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
    Field_ClimbUp();
    FieldCamera_MoveTo(-1, -1, -1, 0);
    Event_ShowMessage(attendant, 0);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 2, PIXELS(3) / 4);
    Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 488, 248);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 20);
    FieldActor_JumpDownLedge(ACTOR_PARTY_LEADER, 6, 0);
    FieldCamera_MoveWithinMapBounds(PIXELS(536), -1, PIXELS(160), 1);
    Event_ShowMessageAndWait(attendant, 0);
    Colosso_RestoreWarrior(ACTOR_PARTY_LEADER);
    FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
    Colosso_RepeatStageNumber(attendant, 4);
  } else if (result == COLOSSO_GUIDE_OFFER_CHEERING) {
    Event_SetMessageId(MsgColossoScalingWallQuickly);
    Event_ShowMessageAndWait(attendant, 0);
  }
  Colosso_OfferCheeringSpot(result, attendant, 4);
  Event_End();
}

/// Moves a demonstration obstacle to pixel coordinates while its pusher
/// follows.
static void ColosseumSatrageMatch_PushObstacleToCell(s32 obstacleId, s32 column,
                                                     s32 row) {
  s32 pusher = gAdventure.leaderActorId;
  struct Actor *pusherActor = Actor_GetById(pusher);
  struct Actor *obstacle = Actor_GetById(obstacleId);
  s32 alongX = (obstacle->x.fixed >> 20) != column / 2;
  s32 offsetX;
  s32 offsetZ;

  column <<= 16;
  row <<= 16;
  if (alongX) {
    offsetX = (column - obstacle->x.fixed) / 2;
    offsetZ = 0;
  } else {
    offsetX = 0;
    offsetZ = (row - obstacle->z.fixed) / 2;
  }
  Import_ActorSetAnimation(pusher, 8);
  Event_WaitFrames(6);
  obstacle->speedLimit = 0x8000;
  obstacle->acceleration = 0x3333;
  Sound_Play(SE_HEAVY_MOVE);
  Actor_SelectAnimation(obstacle, 3);
  FieldActor_SetMoveTarget(obstacle, column, 0, row);
  Event_WaitFrames(6);
  Import_ActorSetAnimation(pusher, ANIM_WALK);
  Actor_SetSpeedParameters(pusher, PIXELS(1) / 2, PIXELS(1) / 5);
  Actor_SelectAnimation(pusherActor, 2);
  FieldActor_SetMoveTarget(pusherActor, pusherActor->x.fixed + offsetX, 0,
                           pusherActor->z.fixed + offsetZ);
  Actor_WaitForMove(pusherActor);
  Actor_SelectAnimation(pusherActor, 1);
  Actor_WaitForMove(obstacle);
  Actor_SelectAnimation(obstacle, 1);
  Sound_Play(SE_STOP_ACTION_SE);
  Sound_Play(SE_HEAVY_STOP);
  Event_WaitFrames(15);
}

/// The Board Walk's attendant, stage 5 of Colosseum Final 2: the first time,
/// Isaac appears and rolls a log into a path; then the objective, and a place
/// for a friend to cheer.
static void ColosseumSatrageMatch_TalkToBoardWalkAttendant(s32 attendant) {
  s32 result;

  if (gAdventure.entrance == COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_EAST) {
    Colosso_AskDoneCheering(attendant);
    return;
  }
  Event_Start();
  result = Colosso_GreetAtStage(attendant, 5);
  if (result == COLOSSO_GUIDE_DESCRIBE) {
    Event_SetMessageId(MsgColossoPlaceCalledBoard);
    FieldCamera_SetSpeedParameters(0x20000, 0x4000);
    FieldCamera_MoveTo(PIXELS(328), -1, PIXELS(264), 1);
    FieldCamera_WaitForArrival();
    Event_WaitFrames(30);
    FieldCamera_SetSpeed(0x18000, 0x3000);
    FieldCamera_MoveTo(PIXELS(312), -1, PIXELS(176), 1);
    FieldCamera_WaitForArrival();
    Event_ShowMessageAndWait(attendant, 0);
    Colosso_FadeInWarrior(ACTOR_PARTY_LEADER, 408, 264);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 2, PIXELS(3) / 4);
    Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 408, 216);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 10);
    Event_ShowMessageAndWait(attendant, 0);
    ColosseumSatrageMatch_PushObstacleToCell(ACTOR_ROLLING_LOG_EAST, 360, 208);
    FieldEffect_SpawnAttached(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_SMILE,
                              45);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 2, PIXELS(3) / 4);
    Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 376, 216);
    Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 376, 248);
    Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 312, 248);
    Event_ShowMessageAndWait(attendant, 0);
    Colosso_RestoreWarrior(ACTOR_PARTY_LEADER);
    FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
    Actor_SetPosition(ACTOR_ROLLING_LOG_EAST, PIXELS(392), PIXELS(208));
    Colosso_RepeatStageNumber(attendant, 5);
  } else if (result == COLOSSO_GUIDE_OFFER_CHEERING) {
    Event_SetMessageId(MsgColossoFocusRollingLogs);
    Event_ShowMessageAndWait(attendant, 0);
  }
  Colosso_OfferCheeringSpot(result, attendant, 5);
  Event_End();
}

// This scene's exits for colosso_solo.h.
#define COLOSSO_SOLO_TO_RACE COLOSSEUM_SATRAGE_MATCH_LEND_A_HAND_TO_RACE
#define COLOSSO_SOLO_TO_COLOSSEUM COLOSSEUM_SATRAGE_MATCH_TO_COLOSSEUM_SOUTH
#include "shared/colosso_solo.h"
#include "shared/item_equip.h"
#include "shared/colosso_task.h"

/// Chooses the lettering animation script and resets its task state.
static void ColosseumSatrageMatch_StartLetteringAnimation(u32 mode,
                                                          u32 tileVariant) {
  s32 script;

  gColosso_LetteringMode = mode;
  gColosso_LetteringParameter = (tileVariant << 4);

  {
    s32 priority = TASK_PASS_FRAME;
    Task_AddFrame(Colosso_UpdateLettering, priority);
  }

  script = (s32)gColosseumSatrageMatch_LetteringTitle;
  if (mode == COLOSSO_SHAPE_DIGIT) {
    script = (s32)gColosseumSatrageMatch_LetteringDigit;
  }
  if (mode == COLOSSO_SHAPE_GO) {
    script = (s32)gColosseumSatrageMatch_LetteringGo;
  }
  if (mode == COLOSSO_SHAPE_RESULT) {
    if (tileVariant != 0) {
      script = (s32)gColosseumSatrageMatch_LetteringLose;
    } else {
      script = (s32)gColosseumSatrageMatch_LetteringWon;
    }
  }

  gColosso_LetteringTimer = 0;
  gColosso_LetteringScript = (s16 *)script;
  gColosso_ModeMoveTarget = 0;
  gColosso_ModeMoveDuration = 0;
  gColosso_LetteringPosition = 0;
}

/// The lettering a visit opens with: `Lend a Hand` for the friends' turns
/// (mode 0), or for the race `Match 2`, then 3, 2, 1 and `Go!`.
static void ColosseumSatrageMatch_ShowRoundStartLettering(s32 mode) {
  if (mode == 0) {
    Event_Start();
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Event_WaitFrames(30);
    Sound_Play(MUS_LEVEL_UP);
    Colosso_LoadLettering(COLOSSO_PICTURE_LEND_A_HAND);
    ColosseumSatrageMatch_StartLetteringAnimation(COLOSSO_SHAPE_TITLE, 0);
    Event_WaitFrames(120);
    Event_End();
    return;
  }

  Sound_Play(SE_COLOSSO_LETTERING);
  Event_Start();
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  {
    s16 *openingScript = gColosseumSatrageMatch_LetteringTitle;
    *(s16 *)((u8 *)openingScript + 30) = (mode * 60);
  }
  Event_WaitFrames(30);
  Sound_Play(mode + 0x5a);
  Colosso_LoadLettering(mode);
  ColosseumSatrageMatch_StartLetteringAnimation(COLOSSO_SHAPE_TITLE, 0);
  Event_WaitFrames(120);

  // Finish the opening fanfare before the final lettering pulses and start
  // signal.
  while (Sound_GetBgmRestoreDelay() != 0) {
    WaitFrames(1);
  }

  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  Colosso_LoadLettering(COLOSSO_PICTURE_COUNT_3_2);
  ColosseumSatrageMatch_StartLetteringAnimation(COLOSSO_SHAPE_DIGIT, 0);
  Sound_Play(SE_COLOSSO_COUNTDOWN);
  Event_WaitFrames(60);
  ColosseumSatrageMatch_StartLetteringAnimation(COLOSSO_SHAPE_DIGIT, 1);
  Sound_Play(SE_COLOSSO_COUNTDOWN);
  Event_WaitFrames(60);
  Colosso_LoadLettering(COLOSSO_PICTURE_COUNT_1);
  ColosseumSatrageMatch_StartLetteringAnimation(COLOSSO_SHAPE_DIGIT, 0);
  Sound_Play(SE_COLOSSO_COUNTDOWN);
  Event_WaitFrames(60);
  Colosso_LoadLettering(COLOSSO_PICTURE_GO);
  ColosseumSatrageMatch_StartLetteringAnimation(COLOSSO_SHAPE_GO, 0);
  Sound_Play(SE_COLOSSO_GO);
  Scene_PlayMusic();
  Event_End();
  GameFlag_Set(FLAG_IN_TOWN);
}

/// The result: `Won` or `Lose` in large lettering, and the loser's reaction.
static void ColosseumSatrageMatch_ShowBattleResultLettering(s32 result) {

  s32 reactingActorId;

  Sound_Play(SE_COLOSSO_LETTERING);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  gColosseumSatrageMatch_LetteringWon[13] = result * 60;
  STORE_CONST(gColosseumSatrageMatch_LetteringLose[13]) =
      (result < 0 ? -result : result) * 60;
  if (result < 0) {
    Event_WaitFrames(30);
    Sound_Play(MUS_INN);
    Colosso_LoadLettering(COLOSSO_PICTURE_WON_LOSE_FOR_LOSS);
    /* FAKEMATCH(wrapper): called through a cast to the s32 call the bytes
     * show, which sets r1 before r0; as a direct call, 1 of 72 instructions
     * differ in tbs-en. */
    COLOSSEUM_SATRAGE_MATCH_START_MODE_TASK(3, 1);
    Event_WaitFrames(-result * 60 + 60);
    reactingActorId = 0;
  } else {
    Event_WaitFrames(30);
    Sound_Play(result + 90);
    Colosso_LoadLettering(COLOSSO_PICTURE_WON_LOSE);
    /* FAKEMATCH(wrapper): the same cast as the first arm, which sets r1 before
     * r0; as a direct call, 1 of 72 instructions differ in tbs-en. */
    COLOSSEUM_SATRAGE_MATCH_START_MODE_TASK(3, 0);
    Event_WaitFrames(result * 60 + 60);
    reactingActorId = 8;
  }
  FieldEffect_SpawnAttached(reactingActorId, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                            0);
  while (Sound_GetBgmRestoreDelay() != 0) {
    WaitFrames(1);
  }
  Sound_Play(MUS_SILENCE);
  Event_WaitFrames(30);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
}

#include "shared/colosso_marker.h"
#include "shared/colosso_item_icon.h"

/// Unused: writes eight uppercase hexadecimal digits followed by a terminator;
/// nothing calls it.
static UNUSED void String_WriteU32AsHex(u8 *destination, u32 value) {
  s32 digit;

  destination += 8;
  *destination = 0;
  destination--;
  for (digit = 7; digit >= 0; digit--) {
    *destination = gHexDigits[value & 15];
    value >>= 4;
    destination--;
  }
}

/// Retains the empty hook called before the finish.
static void MapLocalState_EmptyHook(void) {}

/// Unused: marks the opponent's path recorder finished
/// (PATH_RECORDER_FINISHED); nothing calls it.
static UNUSED void ColosseumSatrageMatch_MarkOpponentFinished(void) {
  ((struct PathRecorder *)gSceneLocalState)->mode = PATH_RECORDER_FINISHED;
}

static void ColosseumSatrageMatch_WaitForOpponentFinish(void) {
  s16 *mode = &((struct PathRecorder *)gSceneLocalState)->mode;

  while (*mode != 9) {
    WaitFrames(1);
  }
}

/// Occasionally spawns a falling particle around the attached finish effect.
static void ColosseumSatrageMatch_SpawnRandomMapEffect(struct Actor *actor) {
  struct Actor *source = (struct Actor *)actor;
  s32 position[3];
  u32 chance;

  if (source->velocityY >= -255 && source->velocityY <= 255) {
    source->motionFlags = 0;
  }
  chance = Import_RandomNext();
  if (chance * 100 >> 16 <= 9) {
    struct Actor *particle;
    s32 radius;
    s32 angle;

    SET_VECTOR(position, source->x.fixed, source->y.fixed, source->z.fixed);
    radius = Import_RandomNext();
    angle = Import_RandomNext();
    Vector_AddPolarOffset(radius << 4, angle, position);
    {
      s32 x = position[0];
      s32 y = position[1];
      s32 z = position[2];

      particle = FieldActor_Create(SPRITE_SPARKLE, x, y, z);
    }
    if (particle != 0) {
      particle->motionFlags = 0;
      Actor_SetSpriteFlags(particle, 0);
      Actor_StartScript(particle, gColosseumSatrageMatch_SparkScript);
      Actor_SelectAnimation(particle, 1);
      Actor_SelectAnimation(particle, 0);
    }
  }
}

/// Raises the attached finish effect, starts its script and clears the link.
static s32
ColosseumSatrageMatch_RaiseLinkedMapEffect(struct Actor *interpreter) {
  struct Actor *source = interpreter;
  struct Actor *effect = Actor_GetById(source->timer);

  FieldActor_SetMoveTarget(effect, source->x.fixed,
                           source->y.fixed + PIXELS(36), source->z.fixed);
  effect->motionFlags = 0;
  Actor_StartScript(effect, gColosseumSatrageMatch_RaiseScript);
  Sound_Play(MUS_TREASURE_FOUND);
  source->timer = 0;
  return 0;
}

/// Approaches the finish item on the unused side and shows the item message.
static s32 ColosseumSatrageMatch_ApproachFinishAndTakeItem(s32 firstItem,
                                                           s32 secondItem) {
  struct ColossoGaugeWork *gauge = HEAP_SLOT_BLOCK(HEAP_SLOT_COLOSSO_RACE);
  u8 *gameState;
  struct Actor *warrior;
  s32 finishAlreadyReached;
  s32 x;
  s32 z;
  u16 *itemActorId;
  s16 *timer;

  finishAlreadyReached = GameFlag_Get(FLAG_ARENA_REACHED);

  gameState = (u8 *)&gAdventure;
  warrior = Actor_GetById(((struct Adventure *)gameState)->leaderActorId);

  if (gauge->finishX < warrior->x.fixed) {
    x = gauge->finishX + PIXELS(12);
  } else {
    x = gauge->finishX - PIXELS(12);
  }

  if (finishAlreadyReached != 0) {
    z = gauge->finishZ + PIXELS(16);
    itemActorId = &gauge->finishItemActorIdB;
  } else {
    z = gauge->finishZ - PIXELS(16);
    itemActorId = &gauge->finishItemActorIdA;
  }

  timer = (s16 *)&warrior->timer;
  *timer = *itemActorId;
  warrior->acceleration = PIXELS(1) / 4;
  warrior->speedLimit = PIXELS(1);

  FieldActor_SetMoveTarget(warrior, x, 0, z);
  GameFlag_Set(FLAG_ARENA_REACHED);
  Actor_StartScript(warrior, gColosseumSatrageMatch_ApproachScript);

  while (*timer != 0) {
    WaitFrames(1);
  }

  // Assign the item on the side this warrior reached first.
  if (finishAlreadyReached == 0) {
    /* FAKEMATCH(wrapper): called through a cast to the s32 call the bytes
     * show, which sets r1 before r0; as a direct call, 1 of 110 instructions
     * differ in tbs-en. */
    COLOSSEUM_SATRAGE_MATCH_APPLY_MATCHING_SLOT_VALUE(0, firstItem);
    MessageArgument_Push(firstItem, 2);
  } else {
    /* FAKEMATCH(wrapper): the same cast as the first arm, which sets r1 before
     * r0; as a direct call, 1 of 110 instructions differ in tbs-en. */
    COLOSSEUM_SATRAGE_MATCH_APPLY_MATCHING_SLOT_VALUE(0, secondItem);
    MessageArgument_Push(secondItem, 2);
  }

  gameState = (u8 *)&gAdventure;
  MessageArgument_Push(((struct Adventure *)gameState)->leaderActorId, 1);
  FieldMessage_Show(MsgColossoIsaacGotItem, 3);
  Actor_WaitForScriptEnd(warrior);

  return finishAlreadyReached;
}

#include "shared/colosso_path_opponent.h"

/// Allocates the race work, records both warriors and installs the gauge
/// task.
void Scene_BuildDescriptorAndInstallTask(s32 leaderId, s32 opponentId,
                                         s32 segments, s32 finishX, s32 finishZ,
                                         s32 firstItem, s32 secondItem) {
  struct ColossoRaceWork *work;
  struct Actor *leader;
  struct Actor *opponent;
  void *graphics;
  s32 vramSlot;

  work = HeapSlot_AllocEwram(HEAP_SLOT_COLOSSO_RACE,
                             sizeof(struct ColossoRaceWork));
  graphics = Heap_AllocEwram(512);

  work->gauge.playerActorId = leaderId;
  work->gauge.opponentActorId = opponentId;
  work->gauge.finishItemActorIdA = firstItem;
  work->gauge.finishItemActorIdB = secondItem;
  work->gauge.segments = segments;
  work->gauge.finishX = finishX;
  work->gauge.finishZ = finishZ;

  leader = Actor_GetById(leaderId);
  opponent = Actor_GetById(opponentId);

  // Mirror the recorded opponent start when this is a fresh arrival at the
  // race.
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
    opponent->x.fixed = (finishX << 1) - leader->x.fixed;
    opponent->z.fixed = leader->z.fixed;
  }

  work->gauge.level = 0;
  work->gauge.hold = 0;

  // Upload the gauge graphics and keep only its VRAM slot after freeing
  // scratch.
  FieldDecompress_TaggedLz(gColosso_GaugeGraphics, graphics);

  vramSlot = VramCache_FindFreeEntry();
  work->gauge.vramBlock = vramSlot;
  VramCache_Load((s16)vramSlot, 512, graphics);

  Task_AddFrame(Colosso_DrawGauge, TASK_PASS_FRAME - 10);

  Heap_Release(graphics);
}

/// Decodes the opponent's recorded path and starts its playback task.
static void ColosseumSatrageMatch_StartOpponentPath(s32 pathResource) {
  struct ColossoRaceWork *work = HEAP_SLOT_BLOCK(HEAP_SLOT_COLOSSO_RACE);
  struct PathRecorder *recorder = (struct PathRecorder *)gSceneLocalState;

  FieldDecompress_TaggedLz(Resource_GetTableEntry(pathResource), work->path);
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
    recorder->mode = PATH_RECORDER_PLAY;
    recorder->mirror = 1;
    recorder->actorId = work->gauge.opponentActorId;
    recorder->still = 0;
    recorder->pathIndex = 0;
  }
  Task_AddFrame(Colosso_UpdatePathOpponent, TASK_PASS_FRAME + 5);
}

#include "shared/colosso_map_state.h"
#include "shared/actor_findslot.h"

/// Pushes the adjacent obstacle only when the destination and space above are
/// clear.
static void ColosseumSatrageMatch_PushAdjacentObstacle(void) {

  struct Actor *pusher;
  struct Actor *obstacle;
  struct Actor *blocker;
  u32 packedOffset;
  u32 direction;
  struct FieldPosition destination;
  s32 zero;
  s32 actorId;

  actorId = gAdventure.leaderActorId;
  pusher = Actor_GetById(actorId);

  direction = pusher->facing >> 12;

  packedOffset = gColosseumSatrageMatch_DirectionSteps[direction];
  destination.x = pusher->x.fixed + (packedOffset & 0xffff0000);
  destination.y = pusher->y.fixed;
  packedOffset <<= 16;
  destination.z = pusher->z.fixed + packedOffset;

  obstacle = Actor_FindAtPosition(&destination, pusher);
  if (obstacle == 0) {
    return;
  }

  // Require a free destination cell and free space immediately above the
  // pillar.
  packedOffset = gColosseumSatrageMatch_DirectionSteps[direction];
  destination.x = obstacle->x.fixed + (packedOffset & 0xffff0000);
  destination.y = obstacle->y.fixed;
  packedOffset <<= 16;
  destination.z = obstacle->z.fixed + packedOffset;

  blocker = Actor_FindAtPosition(&destination, obstacle);
  if (blocker != 0 && (blocker->collisionFlags & 1) != 0) {
    return;
  }

  destination.x = obstacle->x.fixed;
  destination.y = obstacle->y.fixed + 0x100000;
  destination.z = obstacle->z.fixed;

  blocker = Actor_FindAtPosition(&destination, obstacle);
  if (blocker != 0 && (blocker->collisionFlags & 1) != 0) {
    return;
  }

  obstacle->layer = 2;
  zero = 0;

  packedOffset = gColosseumSatrageMatch_DirectionSteps[direction];
  destination.x = obstacle->x.fixed + (packedOffset & 0xffff0000);
  destination.y = obstacle->y.fixed;
  packedOffset <<= 16;
  destination.z = obstacle->z.fixed + packedOffset;

  if (FieldActor_CheckMovementCollision(obstacle, &destination) > 0) {
    return;
  }

  // Start the push only after all occupancy and terrain checks have passed.
  Actor_SelectAnimation(pusher, 8);
  WaitFrames(15);

  obstacle->speedLimit = 0x3333;
  obstacle->acceleration = 0x3333;
  FieldActor_SetMoveTarget(obstacle, destination.x, destination.y,
                           destination.z);

  pusher->speedLimit = 0x3333;
  pusher->acceleration = 0x3333;
  FieldActor_SetMoveTarget(pusher, destination.x, destination.y, destination.z);

  Sound_Play(SE_PUSH_STEP);
  Actor_WaitForMove(obstacle);
  Sound_Play(SE_STOP_ACTION_SE);

  obstacle->x.fixed = destination.x;
  obstacle->z.fixed = destination.z;
  obstacle->velocityX = zero;
  obstacle->velocityZ = zero;

  Actor_SelectAnimation(pusher, 1);
}

#include "shared/colosso_occupant.h"

/// Closes the arena's step trigger; nothing in the game sets it.
/// ★ Named from the game: "This is the battle arena. After getting here, the
/// warriors do battle." (MsgColossoWarriorsEnterFinalsWithoutAny)
#define FLAG_COLOSSEUM_ARENA_CLOSED FLAG_SCENE_TEMP_10
/// The chest holding a Smoke Bomb has been opened.
/// ★ Named from the game: "Smoke Bomb" (MsgItemName)
#define FLAG_COLOSSEUM_SATRAGE_MATCH_SMOKE_BOMB_CHEST_OPENED FLAG_AREA_TEMP_39

const u8 gColosseumSatrageMatch_RollLogScript[] = {2, 2, 3, 3};

// clang-format off
const u8 gColosso_LetteringPaletteOffsets[] = {
    0, 32, 32, 32, 96, 64, 64,
    COLOSSEUM_SATRAGE_MATCH_ALTERNATE_PORTRAIT_PALETTE_OFFSET, 128, 0,
};

const s16 gColosseumSatrageMatch_LetteringDigit[] = {
    0x1000, 512, 1, 0x2000, 0, 1, 0x1000, 256,
    6, 0x2000, 16, 6, 0x7fff, 30, 0x2000, 0,
    30, 0x7fff, 30, -1,
};

const s16 gColosseumSatrageMatch_LetteringLose[] = {
    0x1000, 128, 1, 0x2000, 0, 1, 0x1000, 256,
    6, 0x2000, 16, 6, 0x7fff, 60, 0x1000, 384,
    6, 0x7fff, 6, 0x1000, 240, 6, 0x7fff, 6,
    0x1000, 368, 6, 0x7fff, 6, 0x1000, 224, 6,
    0x7fff, 6, 0x2000, 0, 30, 0x1000, 352, 6,
    0x7fff, 6, 0x1000, 208, 6, 0x7fff, 6, 0x1000,
    336, 6, 0x7fff, 6, 0x1000, 192, 6, 0x7fff,
    6, 0x1000, 320, 6, 0x7fff, 6, -1,
};

char gHexDigits[20] = "0123456789ABCDEF";

s32 gColosseumSatrageMatch_SparkScript[] = {
    SC_ADD(SW_Y, -0x4000),
    SC_WAIT(1),
    SC_LOOP(60, SC_TOP),
    SC_RELEASE,
};

s32 gColosseumSatrageMatch_RaiseScript[] = {
    SC_SET(SW_HOOK, (s32)ColosseumSatrageMatch_SpawnRandomMapEffect),
    SC_LABEL(1),
    SC_WAIT(10),
    SC_SET(SW_SCALE_X, 0x16000),
    SC_SET(SW_SCALE_Y, 0x16000),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 0x18000),
    SC_SET(SW_SCALE_Y, 0x18000),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 0x12000),
    SC_SET(SW_SCALE_Y, 0x12000),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 0x10000),
    SC_SET(SW_SCALE_Y, 0x10000),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 0x16000),
    SC_SET(SW_SCALE_Y, 0x16000),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 0x18000),
    SC_SET(SW_SCALE_Y, 0x18000),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 0x12000),
    SC_SET(SW_SCALE_Y, 0x12000),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 0x10000),
    SC_SET(SW_SCALE_Y, 0x10000),
    SC_WAIT(2),
    SC_WAIT(50),
    SC_LOOP(2, SC_TO(1)),
    SC_POSITION(0, 0, 0),
    SC_SET(SW_HOOK, 0),
    SC_END,
};

#define DIRECTION_STEP(x, z) ((u32)(x) << 16 | (u16)(z))

u32 gColosseumSatrageMatch_DirectionSteps[] = {
    DIRECTION_STEP(16, 0),
    DIRECTION_STEP(16, 0),
    DIRECTION_STEP(16, 0),
    DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),
    DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16),
    DIRECTION_STEP(16, 0),
};

struct SceneEntrance gColosseumSatrageMatchEntrances[] = {
    { COLOSSEUM_SATRAGE_MATCH_RACE_FROM_LEND_A_HAND, CONDITION_ALWAYS, 1272, 0, 168, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_EAST, CONDITION_ALWAYS, 1224, 0, 488, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { 10, CONDITION_ALWAYS, 384, 0, 264, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

u32 gColosseumSatrageMatchExits[] = {
    SCENE_EXITS(ColosseumSatrageMatch),
    SCENE_EXIT(COLOSSEUM_SATRAGE_MATCH_LEND_A_HAND_TO_RACE, ColosseumSatrageMatch, COLOSSEUM_SATRAGE_MATCH_RACE_FROM_LEND_A_HAND),
    SCENE_EXIT(COLOSSEUM_SATRAGE_MATCH_TO_COLOSSEUM_SOUTH, Colosseum, COLOSSEUM_FROM_COLOSSEUM_SATRAGE_MATCH_SOUTH),
    SCENE_EXIT(COLOSSEUM_SATRAGE_MATCH_TO_COLOSSEUM_NORTH, Colosseum, COLOSSEUM_FROM_COLOSSEUM_SATRAGE_MATCH_NORTH),
    SCENE_EXIT(COLOSSEUM_SATRAGE_MATCH_TO_BABI_PALACE, BabiPalace, BABI_PALACE_FROM_COLOSSEUM_AZART_MATCH),
    SCENE_EXITS_END,
};

struct ActorPlacement gColosseumSatrageMatchActorPlacements[] = {
    { 0x0, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1272), 0, PIXELS(168), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0 },
    { ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0 },
    { ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0 },
    { SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(264), 0, PIXELS(168), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(376), 0, PIXELS(168), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(568), 0, PIXELS(264), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1272), 0, PIXELS(264), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1224), 0, PIXELS(264), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1176), 0, PIXELS(264), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7,
        PIXELS(264), 0, PIXELS(208), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7,
        PIXELS(392), 0, PIXELS(208), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { 0x103, CONDITION_ALWAYS, 7,
        PIXELS(360), 0, PIXELS(264), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(520), 0, PIXELS(258), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(712), 0, PIXELS(280), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(808), 0, PIXELS(280), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { 0x122, CONDITION_ALWAYS, 7,
        PIXELS(936), 0, PIXELS(216), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { 0x122, CONDITION_ALWAYS, 7,
        PIXELS(968), 0, PIXELS(216), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x122, CONDITION_ALWAYS, 7,
        PIXELS(1000), 0, PIXELS(216), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x122, CONDITION_ALWAYS, 7,
        PIXELS(1032), 0, PIXELS(216), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x122, CONDITION_ALWAYS, 7,
        PIXELS(248), 0, PIXELS(264), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x122, CONDITION_ALWAYS, 7,
        PIXELS(264), 0, PIXELS(264), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x122, CONDITION_ALWAYS, 7,
        PIXELS(296), 0, PIXELS(264), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x122, CONDITION_ALWAYS, 7,
        PIXELS(312), 0, PIXELS(264), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x103, CONDITION_ALWAYS, 7,
        PIXELS(360), 0, PIXELS(264), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1072), 0, PIXELS(280), FACING_WEST, TALK_FACING_KEEP, 0 },
    { SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7,
        PIXELS(840), 0, PIXELS(184), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7,
        PIXELS(840), 0, PIXELS(232), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1224), 0, PIXELS(488), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1208), 0, PIXELS(376), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(984), 0, PIXELS(376), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(760), 0, PIXELS(376), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(536), 0, PIXELS(376), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(312), 0, PIXELS(376), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(104), 0, PIXELS(240), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(104), 0, PIXELS(272), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SCENE_TABLE_END },
};

s32 gColosseumSatrageMatch_LiftPhase = 0;

s32 gColosseumSatrageMatch_LiftTimer = 0;

struct Trigger gColosseumSatrageMatchTriggers[] = {
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 30, CONDITION_ALWAYS, EVENT(ColosseumSatrageMatch_JumpDownLedge) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), 30, CONDITION_ALWAYS, EVENT(ColosseumSatrageMatch_JumpWestFromLedge) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), 28, CONDITION_ALWAYS, EVENT(ColosseumSatrageMatch_RollEastLogWest) },
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), 27, CONDITION_ALWAYS, EVENT(ColosseumSatrageMatch_RollEastLogEast) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), 24, CONDITION_ALWAYS, EVENT(ColosseumSatrageMatch_RollWestLogWest) },
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), 25, CONDITION_ALWAYS, EVENT(ColosseumSatrageMatch_RollWestLogEast) },
    { STEP_TRIGGER_WALKING, 22, CONDITION_ALWAYS, EVENT(ColosseumSatrageMatch_RideLog) },
    { STEP_TRIGGER_WALKING, 21, CONDITION_ALWAYS, EVENT(ColosseumSatrageMatch_PushPillar) },
    { STEP_TRIGGER_WALKING, 20, CONDITION_ALWAYS, EVENT(ColosseumSatrageMatch_PushWallPillar) },
    { STEP_TRIGGER_WALKING, 26, CONDITION_ALWAYS, EVENT(ColosseumSatrageMatch_PushWallPillar) },
    { TRIGGER_STEP, 50, FLAG_COLOSSEUM_ARENA_CLOSED, EVENT(ColosseumSatrageMatch_ReachArena) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 101, FLAG_AREA_TEMP_38, FIND_ITEM(ITEM_NUT) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST) | TRIGGER_FLAG_FACING | FACING_WEST, 102, FLAG_COLOSSEUM_SATRAGE_MATCH_SMOKE_BOMB_CHEST_OPENED, FIND_ITEM(ITEM_SMOKE_BOMB) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 104, FLAG_AREA_TEMP_3B, FIND_ITEM(ITEM_NUT) },
    { PSYNERGY_PHASE_ON_ACTOR(ABILITY_FROST, PSYNERGY_PHASE_ACTS), ACTOR_PUDDLE_WEST, FLAG_WEST_TARGET_FROZEN, EVENT(ColosseumSatrageMatch_FreezeWestTarget) },
    { PSYNERGY_PHASE_ON_ACTOR(ABILITY_FROST, PSYNERGY_PHASE_ACTS), ACTOR_PUDDLE_EAST, FLAG_EAST_TARGET_FROZEN, EVENT(ColosseumSatrageMatch_FreezeEastTarget) },
    { PSYNERGY_PHASE_ON_ACTOR(ABILITY_FORCE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FORCE_TARGET, FLAG_OBSTACLE_KNOCKED_DOWN, EVENT(ColosseumSatrageMatch_PrepareForce) },
    { PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_FORCE_TARGET, FLAG_OBSTACLE_KNOCKED_DOWN, EVENT(ColosseumSatrageMatch_KnockDownObstacle) },
    { PSYNERGY_ON_ACTOR(ABILITY_GROWTH), ACTOR_GROWTH_PLANT, FLAG_PLANT_GROWN, EVENT(ColosseumSatrageMatch_GrowPlant) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_BOARD_WALK_WEST_PILLAR, CONDITION_ALWAYS, EVENT(ColosseumSatrageMatch_IgnoreMove) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_BOARD_WALK_EAST_PILLAR, CONDITION_ALWAYS, EVENT(ColosseumSatrageMatch_IgnoreMove) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_WALL_PILLAR, CONDITION_ALWAYS, EVENT(ColosseumSatrageMatch_MoveWallPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_MINI_MAZE_FIRST_PILLAR, CONDITION_ALWAYS, EVENT(ColosseumSatrageMatch_MoveMiniMazePillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_MINI_MAZE_FIRST_PILLAR + 1, CONDITION_ALWAYS, EVENT(ColosseumSatrageMatch_MoveMiniMazePillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_MINI_MAZE_FIRST_PILLAR + 2, CONDITION_ALWAYS, EVENT(ColosseumSatrageMatch_MoveMiniMazePillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_HALT), ACTOR_LIFT_OPERATOR, FLAG_LIFTS_HALTED, EVENT(ColosseumSatrageMatch_HaltLifts) },
    { TRIGGER_TALK, ACTOR_GUIDE, CONDITION_ALWAYS, EVENT(Colosso_AskReadyToRace) },
    { TRIGGER_TALK, ACTOR_MINI_MAZE_ATTENDANT, CONDITION_ALWAYS, EVENT(ColosseumSatrageMatch_TalkToMiniMazeAttendant) },
    { TRIGGER_TALK, ACTOR_SHIFTING_FLOOR_ATTENDANT, CONDITION_ALWAYS, EVENT(ColosseumSatrageMatch_TalkToShiftingFloorAttendant) },
    { TRIGGER_TALK, ACTOR_LOG_ROLLING_ATTENDANT, CONDITION_ALWAYS, EVENT(ColosseumSatrageMatch_TalkToLogRollingAttendant) },
    { TRIGGER_TALK, ACTOR_WALL_ATTENDANT, CONDITION_ALWAYS, EVENT(ColosseumSatrageMatch_TalkToWallAttendant) },
    { TRIGGER_TALK, ACTOR_BOARD_WALK_ATTENDANT, CONDITION_ALWAYS, EVENT(ColosseumSatrageMatch_TalkToBoardWalkAttendant) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GUIDE, CONDITION_ALWAYS, MSGNO(MsgColossoRiverThisYearsFinalsAreIncredibleThis) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MINI_MAZE_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoWallICantBelieveAnybodyEverGets) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SHIFTING_FLOOR_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoWallHowDoTheyMakeThatFloor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LOG_ROLLING_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoWallImAmazedPeopleCanActuallyCross) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WALL_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoWallBoyThatBitOverThereMust) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOARD_WALK_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoWallIHeardBabiGotTheLog) },
    { TRIGGER_RAISED, 99, CONDITION_ALWAYS, EVENT(Colosso_TakePlaceForMatch) },
    { TRIGGERS_END },
};

s16 gColosso_LetteringSlot = -1;

s16 gColosseumSatrageMatch_LetteringTitle[] = {
    0x4000, -188, 2048, 0x1000, COLOSSEUM_SATRAGE_MATCH_INTRO_SCALE, 1, 0x2000, 16,
    1, 0x7fff, 14, 0x3000, 0, 20, 0x7fff, 60,
    0x3000, 2048, 20, 0x7fff, 60, -1,
};

s16 gColosseumSatrageMatch_LetteringGo[] = {
    0x1000, COLOSSEUM_SATRAGE_MATCH_PULSE_FIRST_SCALE, 1, 0x2000, 0, 1, 0x1000, COLOSSEUM_SATRAGE_MATCH_PULSE_LOW_SCALE,
    6, 0x2000, 16, 6, 0x7fff, 6, 0x1000, COLOSSEUM_SATRAGE_MATCH_PULSE_HIGH_SCALE,
    6, 0x7fff, 6, 0x1000, COLOSSEUM_SATRAGE_MATCH_PULSE_LOW_SCALE, 6, 0x7fff, 6,
    0x1000, COLOSSEUM_SATRAGE_MATCH_PULSE_HIGH_SCALE, 6, 0x7fff, 6, 0x1000, COLOSSEUM_SATRAGE_MATCH_PULSE_LOW_SCALE, 6,
    0x7fff, 6, 0x2000, 0, 30, 0x1000, COLOSSEUM_SATRAGE_MATCH_PULSE_HIGH_SCALE, 6,
    0x7fff, 6, 0x1000, COLOSSEUM_SATRAGE_MATCH_PULSE_LOW_SCALE, 6, 0x7fff, 6, 0x1000,
    COLOSSEUM_SATRAGE_MATCH_PULSE_HIGH_SCALE, 6, 0x7fff, 6, 0x1000, COLOSSEUM_SATRAGE_MATCH_PULSE_LOW_SCALE, 6, 0x7fff,
    6, 0x1000, COLOSSEUM_SATRAGE_MATCH_PULSE_HIGH_SCALE, 6, 0x7fff, 6, -1,

};

s16 gColosseumSatrageMatch_LetteringWon[] = {
    0x1000, 128, 1, 0x2000, 0, 1, 0x1000, 256,
    6, 0x2000, 16, 6, 0x7fff, 60, 0x1000, 384,
    6, 0x7fff, 6, 0x1000, 272, 6, 0x7fff, 6,
    0x1000, 400, 6, 0x7fff, 6, 0x1000, 288, 6,
    0x7fff, 6, 0x2000, 0, 30, 0x1000, 416, 6,
    0x7fff, 6, 0x1000, 304, 6, 0x7fff, 6, 0x1000,
    432, 6, 0x7fff, 6, 0x1000, 320, 6, 0x7fff,
    6, 0x1000, 448, 6, 0x7fff, 6, -1,
};
// clang-format on

s16 gColosso_MarkerSlot = -1;

s32 gColosso_OpponentFinishScript[] = {
    SC_ANIMATION(2),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(6),
    SC_CALL(ColosseumSatrageMatch_RaiseLinkedMapEffect),
    SC_WAIT(3),
    SC_ANIMATION(10),
    SC_WAIT(150),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_ANIMATION(1),
    SC_END,
};

s32 gColosseumSatrageMatch_ApproachScript[] = {
    SC_ANIMATION(2),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(6),
    SC_CALL(ColosseumSatrageMatch_RaiseLinkedMapEffect),
    SC_WAIT(3),
    SC_ANIMATION(28),
    SC_WAIT(150),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_ANIMATION(1),
    SC_END,
};

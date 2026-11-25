/// @file
/// Colosseum Final 1 (SceneId_ColosseumAzartMatch) is the site of the first
/// finals match, where Isaac races Azart through four stages to the arena and
/// fights him there.
///
/// Azart is the second-best of the warriors who cleared the trials. Babi has
/// put Isaac in the finals himself, and only Isaac may race. Garet, Ivan and
/// Mia help from the stages.
///
/// The stages, by their attendants' names, are the Rock Challenge, the
/// Pipeworks, the Broken Bridge and the Lumber Water Crossing Area.
///
/// Isaac comes here three times, each time at its own entrance.
/// Scene_Initialize blocks Retreat and restores the stages from
/// their temporary flags (FLAG_PIPES_MOVED, FLAG_PIPEWORKS_PIPE_PUSHED,
/// FLAG_PLANT_GROWN and the rock's column in FLAG_SECOND_ROCK_COLUMN), so that
/// what the friends changed stays changed for the race. Then it sets up the
/// visit that entrance stands for.
///
/// - COLOSSEUM_AZART_MATCH_FROM_COLOSSEUM, the tour, from the Colosseum's east
///   passage. The guide (ColosseumAzartMatch_ExplainFinalsRules) tells the
///   party that warriors enter without their own equipment and race through the
///   stages to the arena. There the first to arrive takes the better of two
///   prizes and keeps it for the next match. He pans over the stages if asked.
///   Each stage's attendant names it, the first time with a demonstration.
///   Isaac appears on the stage to show how it is crossed, or, at the
///   Pipeworks, a pointer shows how the pipes work. The attendant then offers
///   a place to cheer there. The friend Isaac picks walks to the stage and
///   leaves the party (Colosso_OfferCheeringSpot). When all three are placed,
///   the guide sends Isaac on to the Colosseum's waiting room
///   (COLOSSEUM_AZART_MATCH_TO_COLOSSEUM_WEST).
/// - COLOSSEUM_AZART_MATCH_FROM_COLOSSEUM_WEST, after the warriors have marched
///   out. The lettering reads `Lend a Hand`, and Garet, Ivan and Mia in turn
///   are the party alone, starting where they chose to cheer. Each may use
///   Psynergy on the stages for Isaac. Douse on the Pipeworks' west pipe fills
///   it, Growth grows the plant before the Broken Bridge, and Move shifts the
///   Rock Challenge's rocks. A friend's turn ends when they use Psynergy
///   (Colosso_TakePlaceForMatch) or tell a stage's attendant they are done
///   (Colosso_AskDoneCheering), and where they stand is kept as their place.
///   After Mia, COLOSSEUM_AZART_MATCH_LEND_A_HAND_TO_RACE starts the race.
/// - COLOSSEUM_AZART_MATCH_RACE_FROM_LEND_A_HAND, the race. After the lettering
///   `The 1st Match` and a count of three, Isaac runs alone against Azart, who
///   follows a recorded path (Colosso_StartOpponent). A gauge at the top of the
///   screen shows how near each is to the finish (Colosso_StartRaceGauge).
///   Azart runs east of the finish, which is 1,304 pixels across, where the
///   prizes stand. His path is the mirror of one through Isaac's own stages,
///   so he is seen only as he comes into the arena. At the arena
///   (ColosseumAzartMatch_ReachArena) the first to arrive takes the Iron Shield
///   and the second the Padded Gloves, which Isaac is given and equipped with.
///   Then the two fight. The battle is ENCOUNTER_ZONE_COLOSSO's first or second
///   group, the Azart who has the shield or the one who has not.
///
/// A win returns to COLOSSEUM_AZART_MATCH_AFTER_MATCH_WON, which shows the
/// result and sends Isaac back to the Colosseum
/// (COLOSSEUM_AZART_MATCH_TO_COLOSSEUM_EAST, to
/// COLOSSEUM_FROM_COLOSSEUM_AZART_MATCH_EAST).
///
/// A loss returns to COLOSSEUM_AZART_MATCH_AFTER_MATCH_LOST, and
/// COLOSSEUM_AZART_MATCH_TO_BABI_PALACE wakes the party in Babi's Palace, where
/// Isaac's friends tell him he was only dreaming of the finals.

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "macros.h"
#include "resource_ids.h"
#include "scene_ids.h"
#include "types.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/actor_script.h"
#include "field/event_commands.h"
#include "field/event_inline.h"
#include "field/field.h"
#include "field/field_sequence.h"
#include "field/pushed_actor.h"
#include "field/scene_tables.h"
#include "field/script_operands.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "scene/colosso_gauge.h"
#include "scene/colosso_guide.h"
#include "scene/colosso_lettering.h"
#include "scene/colosso_push.h"
#include "scene/colosso_setup.h"
#include "scene/colosso_solo.h"
#include "scene/import.h"
#include "sound/sound.h"
#include "sound/sound_driver_symbols.h"
#include "system/input.h"
#include "system/io_write_queue.h"
#include "system/main_routine.h"
#include "system/main_routine_state.h"
#include "system/system.h"
#include "system/vram_cache.h"
#include "window/screens.h"
#include "window/window.h"
#include "text/msg_ids.h"

/// The match held here, as the lettering `The 1st Match` reads; the friends'
/// turns open with `Lend a Hand!` instead (Colosso_ShowOpeningLettering).
enum { FINALS_MATCH = 1 };

/// The stages, as their attendants number them: "This is stage 1 of the first
/// finals".
enum ColosseumAzartMatchStage {
  STAGE_ROCK_CHALLENGE = 1,
  STAGE_PIPEWORKS = 2,
  STAGE_BROKEN_BRIDGE = 3,
  STAGE_LUMBER_WATER_CROSSING = 4
};

// The shared Colosso code's state, in the order the scene's memory lays it
// out; the shared files that use it say what each holds: the guide's pointer
// (colosso_guide.c), the lettering (colosso_task.c) and where Isaac stood
// before he was shown on a stage (colosso_guide.c).
s16 gColosso_PointerStep BSS_DATA;
s16 gColosso_LetteringScaleDuration BSS_DATA ALIGNED(4);
s16 gColosso_PointerFlip BSS_DATA ALIGNED(4);
/// Isaac's z before he is shown on the stage, 16.16 fixed-point pixels.
FIXED gColosso_WarriorStartZ BSS_DATA;
s16 gColosso_PointerEndX BSS_DATA;
s16 gColosso_LetteringParameter BSS_DATA ALIGNED(4);
s16 gColosso_LetteringScaleStart BSS_DATA ALIGNED(4);
s16 gColosso_LetteringMoveDuration BSS_DATA ALIGNED(4);
s32 gColosso_LetteringPosition BSS_DATA;
s16 gColosso_PointerBlink BSS_DATA;
s16 gColosso_LetteringScaleTarget BSS_DATA ALIGNED(4);
s16 gColosso_LetteringMoveStep BSS_DATA ALIGNED(4);
s16 gColosso_PointerY BSS_DATA ALIGNED(4);
s16 gColosso_LetteringFadeStep BSS_DATA ALIGNED(4);
/// Isaac's facing before Colosso_FadeInWarrior, which
/// Colosso_RestoreWarrior puts back.
s32 gColosso_WarriorStartAngle BSS_DATA;
s16 gColosso_PointerSteps BSS_DATA;
s16 gColosso_LetteringMode BSS_DATA ALIGNED(4);
s16 gColosso_LetteringFadeTarget BSS_DATA ALIGNED(4);
s16 gColosso_LetteringFadeStart BSS_DATA ALIGNED(4);
s16 gColosso_LetteringTimer BSS_DATA ALIGNED(4);
s16 *gColosso_LetteringScript BSS_DATA;
union PointerCoordinate gColosso_PointerStartX BSS_DATA;
s16 gColosso_LetteringFadeDuration BSS_DATA ALIGNED(4);
u8 gColosseumAzartMatch_UnusedAfterBlendDuration[6] BSS_DATA;
/// The blinking pointer's sprite part: the queue link and attributes 0 to 2.
s32 gColosso_PointerOam[3] BSS_DATA;
union PointerCoordinate gColosso_PointerStartY BSS_DATA;
/// The lettering's sprite parts, three words each, written every frame.
u32 gColosso_LetteringSprites[12] BSS_DATA;
s16 gColosso_LetteringMoveStart BSS_DATA;
s16 gColosso_PointerX BSS_DATA ALIGNED(4);
s16 gColosso_LetteringMoveTarget BSS_DATA ALIGNED(4);
s16 gColosso_LetteringScaleStep BSS_DATA ALIGNED(4);
s16 gColosso_PointerEndY BSS_DATA ALIGNED(4);
/// Isaac's x before he is shown on the stage, 16.16 fixed-point pixels.
FIXED gColosso_WarriorStartX BSS_DATA;

s32 Scene_Initialize(void);
extern const u8 gColosso_LetteringPaletteOffsets[];
extern const s16 gColosso_LetteringDigit[];
extern const s16 gColosso_LetteringLose[];
extern s32 gColosso_PrizeSparkleScript[];
extern s32 gColosso_PrizeScript[];
extern struct SceneEntrance gColosseumAzartMatch_Entrances[];
extern u32 gColosseumAzartMatch_Exits[];
extern struct ActorPlacement gColosseumAzartMatch_ActorPlacements[];
extern u32 gColosseumAzartMatch_BridgeTimer;
extern struct Trigger gColosseumAzartMatch_Triggers[];
extern s16 gColosso_LetteringTitle[];
extern s16 gColosso_LetteringGo[];
extern s16 gColosso_LetteringWin[];
extern s32 gColosso_OpponentFinishScript[];
extern s32 gColosso_LeaderFinishScript[];
extern char gHexDigits[20];
extern s32 gPushedActor_DirectionSteps[];
extern s16 gColosso_LetteringSlot;
extern s16 gColosso_PointerSlot;

// UB: each of these calls a void function through an s32 function type, the
// call the bytes show; UBFIX calls it as declared.
#ifdef UBFIX
#define COLOSSEUM_AZART_MATCH_FACE_DIRECTION Actor_FaceDirectionById
#define COLOSSEUM_AZART_MATCH_PLACE_STAGED_PAIR                                \
  ColosseumAzartMatch_MoveActorPairByOffset
#else
#define COLOSSEUM_AZART_MATCH_FACE_DIRECTION                                   \
  ((s32 (*)())Actor_FaceDirectionById)
#define COLOSSEUM_AZART_MATCH_PLACE_STAGED_PAIR                                \
  ((s32 (*)())ColosseumAzartMatch_MoveActorPairByOffset)
#endif

static void ColosseumAzartMatch_ExplainFinalsRules(s32 guide);
static void Colosso_ShowOpeningLettering(s32 match);
static void Colosso_ShowMatchResult(s32 result);
static void Colosso_NoOp(void);
static void Colosso_WaitForOpponentToFinish(void);
static s32 Colosso_TakePrize(s32 firstItem, s32 secondItem);
static void Colosso_PushActorAhead(void);

typedef void (*MapTask)(void);

/// The first finals match's actors, by the guide's and attendants'
/// explanations.
///
/// Four stages lead to the arena where the two warriors fight: the Rock
/// Challenge, the Pipeworks, the Broken Bridge and the Lumber Water Crossing
/// Area.
enum {
  /// Azart, racing Isaac to the arena.
  ACTOR_OPPONENT = 8,
  /// The Rock Challenge's three rocks, stone pillars the party moves to clear
  /// a path.
  ACTOR_ROCK_A = 9,
  ACTOR_ROCK_B = 10,
  ACTOR_ROCK_C = 11,
  /// A loose length of pipe, pushed west to let water into the Pipeworks'
  /// channel. It is drawn as mercury_lighthouse.c's ACTOR_THIRD_PIPE.
  ACTOR_PIPEWORKS_PIPE = 12,
  /// The white pans hanging under the Pipeworks' two water pipes, which rise
  /// and sink against each other as water fills one, like scales.
  ACTOR_PIPE_WEST = 13,
  ACTOR_PIPE_EAST = 14,
  /// The plant Growth grows into a vine before the Broken Bridge.
  ACTOR_GROWTH_PLANT = 15,
  /// A young man with fair hair who works the Broken Bridge.
  ACTOR_BRIDGE_OPERATOR = 16,
  /// The Lumber Water Crossing Area's two floating logs.
  ACTOR_LOG_NORTH_SOUTH = 17,
  ACTOR_LOG_EAST_WEST = 18,
  /// A soldier who explains the finals.
  ACTOR_GUIDE = 19,
  /// The stages' attendants, young men with fair hair in white tunics.
  ACTOR_ROCK_ATTENDANT = 20,
  ACTOR_PIPEWORKS_ATTENDANT = 21,
  ACTOR_BRIDGE_ATTENDANT = 22,
  ACTOR_LUMBER_ATTENDANT = 23,
  /// The arena's prizes, drawn as item icons at two blank actors
  /// (Colosso_ShowItemIcon).
  ACTOR_PRIZE_A = 24,
  ACTOR_PRIZE_B = 25
};

/// The temporary flags that keep what the friends changed on the stages, for
/// the race.
///
/// Scene_Initialize restores them.
enum {
  FLAG_PIPES_MOVED =
      FLAG_AREA_TEMP_01, ///< the Pipeworks' pipes, as water filled them
  FLAG_PIPEWORKS_PIPE_PUSHED = FLAG_AREA_TEMP_02,
  FLAG_PLANT_GROWN = FLAG_AREA_TEMP_03, ///< by Growth, before the Broken Bridge
  /// The byte of flags that keeps the Rock Challenge's second rock's column.
  FLAG_SECOND_ROCK_COLUMN = FLAG_AREA_TEMP_10
};

/// The chests along the stages have been opened: a Nut, an Oil Drop and a
/// second Nut, from west to east.
#define FLAG_COLOSSEUM_AZART_MATCH_FIRST_NUT_CHEST_OPENED FLAG_AREA_TEMP_08
#define FLAG_COLOSSEUM_AZART_MATCH_OIL_DROP_CHEST_OPENED FLAG_AREA_TEMP_09
#define FLAG_COLOSSEUM_AZART_MATCH_SECOND_NUT_CHEST_OPENED FLAG_AREA_TEMP_0A

/// Cell behaviors the stages' triggers answer.
enum ColosseumAzartMatchCellBehavior {
  /// The Rock Challenge's floor round its pillars, (17..21, 9..11), where
  /// walking into a pillar pushes it.
  CELL_BEHAVIOR_ROCK_CHALLENGE_FLOOR = 10,
  /// Cell (27, 11), at the Rock Challenge's east end, where walking into a
  /// pillar pushes it too.
  CELL_BEHAVIOR_ROCK_CHALLENGE_EAST_END = 11,
  /// The east edge of the Rock Challenge's raised square, cell (22, 12).
  CELL_BEHAVIOR_ROCK_CHALLENGE_SQUARE_EDGE = 90,
  /// East of the Pipeworks' loose pipe, cells (38, 7..10), where walking west
  /// pushes it.
  CELL_BEHAVIOR_PIPEWORKS_PIPE = 12,
  /// On the north-south log, as at (64..66, 24): walking north or south rides
  /// it.
  CELL_BEHAVIOR_LOG_NORTH_SOUTH = 20,
  /// On the east-west log, as at (63, 25..27): walking east or west rides it.
  CELL_BEHAVIOR_LOG_EAST_WEST = 21,
  /// The way into the arena, cells (79, 9..14).
  CELL_BEHAVIOR_ARENA_ENTRY = 50,
  /// The chests: a Nut at (28, 5), an Oil Drop at (48, 6) and a Nut at
  /// (61, 13), opened facing west.
  CELL_BEHAVIOR_FIRST_NUT_CHEST = 100,
  CELL_BEHAVIOR_OIL_DROP_CHEST = 101,
  CELL_BEHAVIOR_SECOND_NUT_CHEST = 103
};

/// Cells kept off the course whose attributes (behaviour and height) the
/// stages' changes copy.
enum ColosseumAzartMatchSpareCell {
  /// A square to hop to (CELL_BEHAVIOR_JUMP_FORWARD), stamped on a pillar's
  /// top.
  HOP_SQUARE_COLUMN = 14,
  HOP_SQUARE_ROW = 13,
  /// Three cells of low ground with no behaviour, laid over the Rock
  /// Challenge's (23..25, 12) before the pillars' tops are stamped again.
  LOW_GROUND_COLUMN = 27,
  LOW_GROUND_ROW = 13,
  /// The grown vine's cell, climbed (CELL_BEHAVIOR_CLIMB).
  VINE_COLUMN = 47,
  VINE_ROW = 24,
  /// The Broken Bridge's row of six, (50..55, 12), with the bridge lying down
  /// and standing up.
  BRIDGE_DOWN_ROW = 25,
  BRIDGE_UP_ROW = 24,
  /// Three cells to ride the north-south log (CELL_BEHAVIOR_LOG_NORTH_SOUTH),
  /// across, and three to ride the east-west one, down.
  LOG_NORTH_SOUTH_COLUMN = 64,
  LOG_NORTH_SOUTH_ROW = 24,
  LOG_EAST_WEST_COLUMN = 63,
  LOG_EAST_WEST_ROW = 25
};

/// Pictures kept on layer 1 (map rows 28 and down) at columns 92 to 100: the
/// Broken Bridge's spans standing up, swinging and lying down, two cells
/// square, drawn at the spans' places (50, 38) and (54, 38); and the five
/// steps of water pouring into the Pipeworks' channel, one cell by three each,
/// drawn at (34, 38).
enum ColosseumAzartMatchPictureCell {
  BRIDGE_PICTURE_COLUMN = 92,
  BRIDGE_UP_PICTURE_ROW = 29,
  BRIDGE_SWINGING_PICTURE_ROW = 31,
  BRIDGE_DOWN_PICTURE_ROW = 33,
  WATER_PICTURE_COLUMN = 96,
  WATER_PICTURE_ROW = 29
};

/// The Broken Bridge's cycle, in frames, counted down: the bridge lies down,
/// walkable, for the last 60 of each.
#define BRIDGE_CYCLE_FRAMES 120

/// The Lumber Water Crossing Area's logs float on fixed lines. The north-south
/// log lies across columns 64 to 66 and floats three cells between row 8 and
/// LOG_SOUTH_ROW, where it bars the way of the east-west log; that one lies
/// across rows 9 to 11 and floats ten cells from the west bank's column 63 to
/// the east bank, or six from column 67, where the other log stops it.
#define LOG_NORTH_ROW 8
#define LOG_SOUTH_ROW 11
#define LOG_NORTH_SOUTH_FLOAT 48
#define LOG_EAST_WEST_FLOAT 160
#define LOG_EAST_WEST_SHORT_FLOAT 96

/// The Broken Bridge's operator's poses as the bridge swings
/// (young_man_with_fair_hair.tsv).
enum BridgeOperatorAnimation {
  BRIDGE_OPERATOR_ANIMATION_SWINGING = 10, ///< as the bridge starts to move
  BRIDGE_OPERATOR_ANIMATION_BRIDGE_DOWN = 11,
  BRIDGE_OPERATOR_ANIMATION_BRIDGE_UP = 12
};

/// SPRITE_PIPE_NORTH_SOUTH's animations, as the Pipeworks' loose pipe is
/// pushed (pipe_north_south.tsv).
enum PipeAnimation {
  PIPE_ANIMATION_STILL = 1,
  PIPE_ANIMATION_ROLLING = 2 ///< its four pictures in turn
};

/// SPRITE_WHITE_PLATFORM's animations for the Pipeworks' pans
/// (white_platform.tsv).
enum PanAnimation {
  PAN_ANIMATION_EMPTY = 2,
  PAN_ANIMATION_FULL = 3 ///< water in it
};

const struct SceneEntrance *Scene_GetEntrances(void) {
  return gColosseumAzartMatch_Entrances;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gColosseumAzartMatch_Exits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gColosseumAzartMatch_ActorPlacements;
}

/// Advances the bridge animation and the cell behaviors that make it
/// walkable.
static void ColosseumAzartMatch_UpdateBrokenBridge(void) {
  switch (gColosseumAzartMatch_BridgeTimer) {
  case 66:
    MapGrid_CopyCellsLayered(BRIDGE_PICTURE_COLUMN, BRIDGE_SWINGING_PICTURE_ROW,
                             2, 2, 50, 38);
    MapGrid_CopyCellsLayered(BRIDGE_PICTURE_COLUMN, BRIDGE_SWINGING_PICTURE_ROW,
                             2, 2, 54, 38);
    Actor_SetAnimation(ACTOR_BRIDGE_OPERATOR,
                       BRIDGE_OPERATOR_ANIMATION_SWINGING);
    break;
  case 60:
    // The bridge lies down, and its cells can be walked on.
    MapGrid_CopyCellsLayered(BRIDGE_PICTURE_COLUMN, BRIDGE_DOWN_PICTURE_ROW, 2,
                             2, 50, 38);
    MapGrid_CopyCellsLayered(BRIDGE_PICTURE_COLUMN, BRIDGE_DOWN_PICTURE_ROW, 2,
                             2, 54, 38);
    MapGrid_CopyCellAttributes(50, BRIDGE_DOWN_ROW, 6, 1, 50, 12);
    Actor_SetAnimation(ACTOR_BRIDGE_OPERATOR,
                       BRIDGE_OPERATOR_ANIMATION_BRIDGE_DOWN);
    break;
  case 6:
    MapGrid_CopyCellsLayered(BRIDGE_PICTURE_COLUMN, BRIDGE_SWINGING_PICTURE_ROW,
                             2, 2, 50, 38);
    MapGrid_CopyCellsLayered(BRIDGE_PICTURE_COLUMN, BRIDGE_SWINGING_PICTURE_ROW,
                             2, 2, 54, 38);
    Actor_SetAnimation(ACTOR_BRIDGE_OPERATOR,
                       BRIDGE_OPERATOR_ANIMATION_SWINGING);
    break;
  case 0:
    // The bridge stands up again, and its cells are a gap once more.
    MapGrid_CopyCellsLayered(BRIDGE_PICTURE_COLUMN, BRIDGE_UP_PICTURE_ROW, 2, 2,
                             50, 38);
    MapGrid_CopyCellsLayered(BRIDGE_PICTURE_COLUMN, BRIDGE_UP_PICTURE_ROW, 2, 2,
                             54, 38);
    Actor_SetAnimation(ACTOR_BRIDGE_OPERATOR,
                       BRIDGE_OPERATOR_ANIMATION_BRIDGE_UP);
    MapGrid_CopyCellAttributes(50, BRIDGE_UP_ROW, 6, 1, 50, 12);
    gColosseumAzartMatch_BridgeTimer = BRIDGE_CYCLE_FRAMES;
    break;
  }
  gColosseumAzartMatch_BridgeTimer--;
}

/// Stops the bridge task and restores its closed frame.
static void ColosseumAzartMatch_ResetBrokenBridge(void) {
  MapTask task;

  gColosseumAzartMatch_BridgeTimer = 0;
  task = ColosseumAzartMatch_UpdateBrokenBridge;
  Task_Remove(task);
  task();
}

/// Starts the broken bridge opening and closing, over and over.
static void ColosseumAzartMatch_StartBrokenBridge(void) {
  gColosseumAzartMatch_BridgeTimer = 66;
  Task_AddFrame(ColosseumAzartMatch_UpdateBrokenBridge, TASK_PASS_FRAME);
}

/// Waits for the crossing window, bounded by one bridge cycle.
static void ColosseumAzartMatch_WaitForBridgeOpening(void) {
  s32 i;

  wait(10);
  for (i = 0; i < BRIDGE_CYCLE_FRAMES && gColosseumAzartMatch_BridgeTimer != 22;
       i++)
    wait(1);
}

/// Move on one of the stone pillars in the Rock Challenge, the first stage,
/// slides it a cell, which can clear the way or give the racers a step.
///
/// The same work follows every push. Once the first pillar has come down to
/// the low ground, level with its floor, it is drawn under the party and its
/// top becomes a square to hop across. The second pillar's top is always such
/// a square, and the column it stands in is kept in FLAG_SECOND_ROCK_COLUMN,
/// so that a friend's Move during Lend a Hand still holds for the race.
static void ColosseumAzartMatch_UpdateRockCells(void) {
  struct Actor *actor;
  FIXED blocked;

  MapGrid_CopyCellAttributes(LOW_GROUND_COLUMN, LOW_GROUND_ROW, 3, 1, 23, 12);
  actor = Actor_Get(ACTOR_ROCK_A);
  blocked = MapGrid_GetTerrainHeight(0, actor->x.fixed, actor->z.fixed);
  if (actor->y.fixed == 0 && blocked == 0) {
    actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    actor->motionFlags = 0;
    {
      s32 x = FIELD_CELL(actor->x.fixed);
      s32 y = FIELD_CELL(actor->z.fixed);

      MapGrid_CopyCellAttributes(HOP_SQUARE_COLUMN, HOP_SQUARE_ROW, 1, 1, x, y);
    }
  }
  actor = Actor_Get(ACTOR_ROCK_B);
  {
    s32 x = FIELD_CELL(actor->x.fixed);

    FieldGameFlag_SetByte(FLAG_SECOND_ROCK_COLUMN, x);
  }
  {
    s32 x = FIELD_CELL(actor->x.fixed);
    s32 y = FIELD_CELL(actor->z.fixed);

    MapGrid_CopyCellAttributes(HOP_SQUARE_COLUMN, HOP_SQUARE_ROW, 1, 1, x, y);
  }
}

/// Walking into a stone pillar in the Rock Challenge pushes it a cell, with
/// Isaac leaning on it, if nothing is in the way.
static void ColosseumAzartMatch_PushRock(void) {
  Colosso_PushActorAhead();
  ColosseumAzartMatch_UpdateRockCells();
}

/// Growth on the sprout before the Broken Bridge, the third stage, grows it
/// into a vine up the wall beside the bridge's scaffold (cell (47, 12)), a
/// shortcut up to the bridge.
static void ColosseumAzartMatch_GrowPlant(void) {
  MapGrid_CopyCellAttributes(VINE_COLUMN, VINE_ROW, 1, 1, 47, 12);
  GameFlag_Set(FLAG_PLANT_GROWN);
}

/// Douse on the white pan under the Pipeworks' west pipe, the second stage,
/// fills it with water. The camera moves over, the full pan sinks and the one
/// under the east pipe rises, as the attendant says the pipes form a kind of
/// scale, and the way on to the next stage opens (cell (41, 12)).
static void ColosseumAzartMatch_DousePipe(void) {
  struct Actor *actor;

  GameFlag_Set(FLAG_PIPES_MOVED);
  actor = Actor_Get(ACTOR_PIPE_WEST);
  Event_Start();
  FieldCamera_SetSpeed(2);
  FieldCamera_MoveTo(600, CAMERA_KEEP, 200, 1);
  Actor_SelectAnimation(actor, PAN_ANIMATION_FULL);
  FieldCamera_WaitForMove();
  actor->motionFlags = 0;
  actor->acceleration = toFIXED(2) / 5;
  actor->speedLimit = toFIXED(4) / 5;
  FieldActor_SetMoveTargetFX(actor, actor->x.fixed, toFIXED(8), actor->z.fixed);
  actor = Actor_Get(ACTOR_PIPE_EAST);
  actor->motionFlags = 0;
  actor->acceleration = toFIXED(2) / 5;
  actor->speedLimit = toFIXED(4) / 5;
  FieldActor_SetMoveTargetFX(actor, actor->x.fixed, toFIXED(32),
                             actor->z.fixed);
  Actor_WaitForMove(actor);
  Event_Wait(45);
  // The way on, (41, 12), becomes a square to hop to, as (43, 12) is.
  MapGrid_CopyCellAttributes(43, 12, 1, 1, 41, 12);
  Event_End();
}

/// Isaac rides one of the Lumber Water Crossing Area's logs dx or dz pixels,
/// from one cell's center to another, as it rolls under him.
static void ColosseumAzartMatch_MoveActorPairByOffset(s32 actorId, s32 dx,
                                                      s32 dz) {
  struct Actor *leader;
  struct Actor *actor;
  FIXED x;
  FIXED z;

  leader = Actor_Get(gMainRoutineState.leaderActorId);
  actor = Actor_Get(actorId);
  Event_Start();
  x = ((leader->x.fixed + toFIXED(dx)) & FIELD_CELL_MASK) + toFIXED(8);
  z = ((leader->z.fixed + toFIXED(dz)) & FIELD_CELL_MASK) + toFIXED(8);

  leader->speedLimit = toFIXED(1);
  leader->acceleration = toFIXED(1) / 2;
  FieldActor_SetMoveTargetFX(leader, x, leader->y.fixed, z);
  Actor_SelectAnimation(leader, ANIM_LEADER_WALK_BACKWARDS);
  x = ((actor->x.fixed + toFIXED(dx)) & FIELD_CELL_MASK) + toFIXED(8);
  z = ((actor->z.fixed + toFIXED(dz)) & FIELD_CELL_MASK) + toFIXED(8);

  actor->speedLimit = toFIXED(1);
  actor->acceleration = toFIXED(1) / 2;
  FieldActor_SetMoveTargetFX(actor, x, actor->y.fixed, z);
  if (dx < 0 || dz < 0) {
    Actor_SelectAnimation(actor, FLOATING_LOG_ANIMATION_ROLL_WEST_OR_NORTH);
  } else {
    Actor_SelectAnimation(actor, FLOATING_LOG_ANIMATION_ROLL_EAST_OR_SOUTH);
  }
  Sound_Play(SE_LOG_PUSH);
  Actor_WaitForMove(leader);
  Actor_SelectAnimation(actor, FLOATING_LOG_ANIMATION_SETTLE);
  Sound_Play(SE_STOP_ACTION_SE);
  Event_End();
}

/// The north-south log carries the leader to the other bank.
static void ColosseumAzartMatch_RollLogNorthSouth(void) {
  struct Actor *actor;
  s32 row;
  s32 offset;

  actor = Actor_Get(gMainRoutineState.leaderActorId);
  row = FIELD_CELL(actor->z.fixed);
  offset = -LOG_NORTH_SOUTH_FLOAT;
  if (row <= LOG_NORTH_ROW) {
    offset = LOG_NORTH_SOUTH_FLOAT;
  }
  // Its row goes back to water, three cells of the pool beside the log's
  // north line, and its new row is the log's.
  MapGrid_CopyCellAttributes(67, 8, 3, 1, 64, row);
  ColosseumAzartMatch_MoveActorPairByOffset(ACTOR_LOG_NORTH_SOUTH, 0, offset);
  actor = Actor_Get(ACTOR_LOG_NORTH_SOUTH);
  row = FIELD_CELL(actor->z.fixed);
  MapGrid_CopyCellAttributes(LOG_NORTH_SOUTH_COLUMN, LOG_NORTH_SOUTH_ROW, 3, 1,
                             64, row);
}

/// The east-west log carries the leader the way the pad is held.
static void ColosseumAzartMatch_RollLogEastWest(void) {
  s32 column;
  s32 offset;
  s32 direction;

  column = FIELD_CELL(Actor_Get(gMainRoutineState.leaderActorId)->x.fixed);
  if ((gKeysHeld & KEY_LEFT) != 0) {
    direction = -1;
  }
  if ((gKeysHeld & KEY_RIGHT) != 0) {
    direction = 1;
  }
  offset = FIELD_CELL(Actor_Get(ACTOR_LOG_NORTH_SOUTH)->z.fixed);
  if (column == 63) {
    if (offset == LOG_SOUTH_ROW) {
      return;
    }
    offset = LOG_EAST_WEST_FLOAT;
  } else if (column == 67) {
    if (offset == LOG_SOUTH_ROW && direction == -1) {
      return;
    }
    offset = LOG_EAST_WEST_SHORT_FLOAT;
  } else {
    if (offset == LOG_SOUTH_ROW) {
      offset = LOG_EAST_WEST_SHORT_FLOAT;
    } else {
      offset = LOG_EAST_WEST_FLOAT;
    }
    offset = -offset;
  }
  // Its column goes back to water, three cells of the pool, and its new
  // column is the log's.
  MapGrid_CopyCellAttributes(72, 9, 1, 3, column, 9);
  ColosseumAzartMatch_MoveActorPairByOffset(ACTOR_LOG_EAST_WEST, offset, 0);
  column = FIELD_CELL(Actor_Get(ACTOR_LOG_EAST_WEST)->x.fixed);
  MapGrid_CopyCellAttributes(LOG_EAST_WEST_COLUMN, LOG_EAST_WEST_ROW, 1, 3,
                             column, 9);
}

/// Runs the five frames that show water entering the Pipeworks channel.
static inline void ColosseumAzartMatch_AnimatePipeworksWater(void) {
  MapGrid_CopyCells(WATER_PICTURE_COLUMN, WATER_PICTURE_ROW, 1, 3, 34, 38);
  Event_Wait(3);
  MapGrid_CopyCells(WATER_PICTURE_COLUMN + 1, WATER_PICTURE_ROW, 1, 3, 34, 38);
  Event_Wait(3);
  MapGrid_CopyCells(WATER_PICTURE_COLUMN + 2, WATER_PICTURE_ROW, 1, 3, 34, 38);
  Event_Wait(3);
  MapGrid_CopyCells(WATER_PICTURE_COLUMN + 3, WATER_PICTURE_ROW, 1, 3, 34, 38);
  Event_Wait(3);
  MapGrid_CopyCells(WATER_PICTURE_COLUMN + 4, WATER_PICTURE_ROW, 1, 3, 34, 38);
}

/// Walking west into the loose length of pipe in the Pipeworks pushes it three
/// cells west, under the spout, and the camera follows it. Water then runs
/// down into the channel, and if the pans have not tipped already, the west
/// pan fills and sinks and the east one rises, opening the way on
/// (cell (41, 12)).
static void ColosseumAzartMatch_PushPipeworksPipe(void) {
  s32 leaderId;
  struct Actor *leader;
  struct Actor *actor;
  s32 none;

  leaderId = gMainRoutineState.leaderActorId;
  leader = Actor_Get(leaderId);
  actor = Actor_Get(ACTOR_PIPEWORKS_PIPE);
  GameFlag_Set(FLAG_PIPEWORKS_PIPE_PUSHED);
  Event_Start();
  Actor_SetAnimation(leaderId, ANIM_PUSH);
  Event_Wait(6);
  actor->speedLimit = toFIXED(1) / 2;
  actor->acceleration = toFIXED(1) / 5;
  Sound_Play(SE_HEAVY_MOVE);
  Actor_SelectAnimation(actor, PIPE_ANIMATION_ROLLING);
  FieldActor_SetMoveTargetFX(actor, actor->x.fixed - toFIXED(48), 0,
                             actor->z.fixed);
  Event_Wait(6);
  Actor_SetAnimation(leaderId, ANIM_WALK);
  Actor_StartCamera(((struct FieldState *)HeapSlot_AllocEwram(
                         HEAP_SLOT_FIELD, sizeof(struct FieldState)))
                        ->camera,
                    (s32)actor);
  Actor_SetSpeedAndAcceleration(leaderId, 0.3, 0.2);
  FieldActor_SetMoveTargetFX(leader, leader->x.fixed - toFIXED(24), 0,
                             leader->z.fixed);
  Actor_WaitForMoveById(leaderId);
  Actor_SetAnimation(leaderId, ANIM_STAND);
  Actor_WaitForMove(actor);
  Actor_SelectAnimation(actor, PIPE_ANIMATION_STILL);
  Sound_Play(SE_STOP_ACTION_SE);
  Sound_Play(SE_HEAVY_STOP);
  Event_Wait(15);
  Event_End();
  // The pipe's column of wall, (37, 7..10), moves to where it now lies, and
  // the floor beside it, column 36, takes its place.
  MapGrid_CopyCellAttributes(37, 7, 1, 4, 34, 7);
  MapGrid_CopyCellAttributes(36, 7, 1, 4, 37, 7);
  none = Event_GetFlag(FLAG_PIPES_MOVED);
  if (none != 0) {
    Event_Start();
    FieldCamera_SetSpeed(2);
    FieldCamera_MoveTo(552, CAMERA_KEEP, 200, 1);
    FieldCamera_WaitForMove();
    ColosseumAzartMatch_AnimatePipeworksWater();
    Event_Wait(15);
    Event_End();
    return;
  }
  GameFlag_Set(FLAG_PIPES_MOVED);
  Event_Start();
  FieldCamera_SetSpeed(2);
  FieldCamera_MoveTo(600, CAMERA_KEEP, 200, 1);
  FieldCamera_WaitForMove();
  actor = Actor_Get(ACTOR_PIPE_WEST);
  actor->motionFlags = none;
  actor->acceleration = toFIXED(2) / 5;
  actor->speedLimit = toFIXED(4) / 5;
  FieldActor_SetMoveTargetFX(actor, actor->x.fixed, toFIXED(8), actor->z.fixed);
  Actor_SelectAnimation(actor, PAN_ANIMATION_FULL);
  ColosseumAzartMatch_AnimatePipeworksWater();
  actor = Actor_Get(ACTOR_PIPE_EAST);
  actor->motionFlags = none;
  actor->acceleration = toFIXED(2) / 5;
  actor->speedLimit = toFIXED(4) / 5;
  FieldActor_SetMoveTargetFX(actor, actor->x.fixed, toFIXED(32),
                             actor->z.fixed);
  Actor_WaitForMove(actor);
  Event_Wait(15);
  Event_End();
  MapGrid_CopyCellAttributes(43, 12, 1, 1, 41, 12);
}

/// Unused: sets FLAG_AREA_TEMP_00. Nothing calls it.
UNUSED static void ColosseumAzartMatch_SetCourseFlag(void) {
  GameFlag_Set(FLAG_AREA_TEMP_00);
}

/// Reaching Colosseum Final 1's arena, the leader takes a prize and fights
/// Azart.
static void ColosseumAzartMatch_ReachArena(void) {
  s32 opponentFirst;
  s32 i;
  s32 opponentFirstSlot = 0;
  Colosso_NoOp();
  Event_Start();
  opponentFirst = Colosso_TakePrize(ITEM_IRON_SHIELD, ITEM_PADDED_GLOVES);
  Colosso_WaitForOpponentToFinish();
  // Up to ten waits of 300 frames while the opponent holds up his prize.
  for (i = 0; i < 10; i++) {
    Actor_WaitForScriptEndById(ACTOR_OPPONENT);
  }
  Actor_SetSpeed(ACTOR_OPPONENT, 1);
  Actor_WalkTo(ACTOR_OPPONENT, 1320, 192);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 1);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 1288, 192);
  Actor_SetAnimation(ACTOR_OPPONENT, ANIM_STAND);
  Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_OPPONENT, 0);
  Event_Wait(10);
  Actor_SetAnimation(ACTOR_OPPONENT, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(20);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 2);
  Actor_SetSpeed(ACTOR_OPPONENT, 2);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 1296, 192);
  Actor_WalkToAndWait(ACTOR_OPPONENT, 1312, 192);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_STRIDE);
  Actor_SetAnimation(ACTOR_OPPONENT, WARRIOR_ANIMATION_FIGHTING_STANCE);
  Event_Wait(10);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_COLOSSO,
                                        opponentFirstSlot - opponentFirst + 1);
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  Event_SetAfterBattleDestination(SCENE_COLOSSEUM_AZART_MATCH,
                                  COLOSSEUM_AZART_MATCH_AFTER_MATCH_WON);
  Event_SetDefeatDestination(SCENE_COLOSSEUM_AZART_MATCH,
                             COLOSSEUM_AZART_MATCH_AFTER_MATCH_LOST);
  GameFlag_Set(FLAG_KEEP_MUSIC_ONCE);
}

/// Walking on from the east edge of the Rock Challenge's raised square (cell
/// (22, 12)) pushes the stone pillar ahead if there is one. With no pillar
/// there, Isaac hops forward across the gap where there is somewhere to land,
/// and otherwise nothing happens.
static void ColosseumAzartMatch_PushRockOrHopAcross(void) {
  if (Colosso_FindActorAhead() == 0) {
    Field_TryJumpForward();
  } else {
    ColosseumAzartMatch_PushRock();
  }
}

const struct Trigger *Scene_GetTriggers(void) {
  return gColosseumAzartMatch_Triggers;
}

/// Back from the battle, Isaac and Azart stand in the arena as the result is
/// shown.
static void ColosseumAzartMatch_RestoreMatchResult(s32 result) {
  Actor_ReleaseById(ACTOR_PRIZE_A);
  Actor_ReleaseById(ACTOR_PRIZE_B);
  Party_RestoreHpPp(1);
  Event_Start();
  Actor_SetPosition(ACTOR_OPPONENT, 1320, 192);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 1288, 192);
  // BUG: FACING_SOUTH is passed where Actor_FaceActor takes an
  // actor id; Actor_Lookup has no actor past 191, so
  // neither turns.
#ifdef BUGFIX
  Actor_FaceDirection(ACTOR_OPPONENT, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
#else
  Actor_FaceActor(ACTOR_OPPONENT, FACING_SOUTH, 0);
  Actor_FaceActor(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
#endif
  if (result < 0) {
    Actor_SetAnimation(ACTOR_OPPONENT, WARRIOR_ANIMATION_ARMS_RAISED);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_CROUCH);
  } else {
    Actor_SetAnimation(ACTOR_OPPONENT, WARRIOR_ANIMATION_DEFEATED);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_ARMS_RAISED);
  }
  wait(1);
  FieldCamera_MoveTo(1304, 0, 128, 0);
  Colosso_ShowMatchResult(result);
  Event_End();
}

/// A friend whose cheering spot is the vine's cell before the Broken Bridge,
/// half cells (94..95, 24..26), is put on layer 1 for the race, the layer the
/// plant itself stands on.
static void ColosseumAzartMatch_SetLayerOnVineCell(s32 actorId) {
  struct Actor *target;
  s32 column;
  s32 row;

  target = Actor_Get(actorId);
  if (target != NULL) {
    column = FIELD_HALF_CELL(target->x.fixed);
    row = FIELD_HALF_CELL(target->z.fixed);
    if (column >= 94 && column <= 95 && row > 23 && row <= 26) {
      target->layer = 1;
    }
  }
}

/// Sets up Colosseum Final 1 for the visit its entrance stands for.
s32 Scene_Initialize(void) {
  struct Actor *actor;
  FIXED hit;
  s32 x;
  s32 col;

  gFieldState->transition = SCENE_TRANSITION(TRANSITION_FADE, 0);
  GameFlag_Set(FLAG_RETREAT_BLOCKED);
  actor = Actor_Get(ACTOR_ROCK_A);
  hit = MapGrid_GetTerrainHeight(0, actor->x.fixed, actor->z.fixed);
  if (actor->y.fixed == 0 && hit == 0) {
    actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    actor->motionFlags = hit;
    MapGrid_CopyCellAttributes(HOP_SQUARE_COLUMN, HOP_SQUARE_ROW, 1, 1,
                               FIELD_CELL(actor->x.fixed),
                               FIELD_CELL(actor->z.fixed));
  }
  x = FieldGameFlag_GetByte(FLAG_SECOND_ROCK_COLUMN);
  if (x == 0) {
    x = 25;
  }
  actor = Actor_Get(ACTOR_ROCK_B);
  actor->x.fixed = FIELD_CELL_CENTER(x);
  actor->motionFlags = 0;
  actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  MapGrid_CopyCellAttributes(HOP_SQUARE_COLUMN, HOP_SQUARE_ROW, 1, 1, x, 12);
  Task_AddFrame(ColosseumAzartMatch_UpdateBrokenBridge, TASK_PASS_FRAME);
  actor = Actor_Get(ACTOR_GROWTH_PLANT);
  actor->layer = 1;
  if (GameFlag_Get(FLAG_PLANT_GROWN)) {
    Actor_SelectAnimation(actor, SPROUT_ANIMATION_GROWN);
    Actor_SetSpriteFlags(actor, 0);
    actor->collisionFlags = 0;
    actor->priorityFlags = ACTOR_PRIORITY_AUTOMATIC | ACTOR_PRIORITY_UNDERFOOT;
    MapGrid_CopyCellAttributes(VINE_COLUMN, VINE_ROW, 1, 1, 47, 12);
  }
  actor = Actor_Get(ACTOR_LOG_NORTH_SOUTH);
  col = FIELD_CELL(actor->z.fixed);
  actor->motionFlags = 0;
  actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  MapGrid_CopyCellAttributes(LOG_NORTH_SOUTH_COLUMN, LOG_NORTH_SOUTH_ROW, 3, 1,
                             64, col);
  actor = Actor_Get(ACTOR_LOG_EAST_WEST);
  col = FIELD_CELL(actor->x.fixed);
  actor->motionFlags = 0;
  actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  MapGrid_CopyCellAttributes(LOG_EAST_WEST_COLUMN, LOG_EAST_WEST_ROW, 1, 3, col,
                             9);
  // With the Pipeworks' loose pipe pushed under the spout, water pours down
  // into the channel.
  if (GameFlag_Get(FLAG_PIPEWORKS_PIPE_PUSHED)) {
    MapGrid_CopyCellAttributes(37, 7, 1, 4, 34, 7);
    MapGrid_CopyCellAttributes(36, 7, 1, 4, 37, 7);
    MapGrid_CopyCells(WATER_PICTURE_COLUMN + 4, WATER_PICTURE_ROW, 1, 3, 34,
                      38); // the water's last step
  }
  actor = Actor_Get(ACTOR_PIPE_WEST);
  if (GameFlag_Get(FLAG_PIPES_MOVED)) {
    MapGrid_CopyCellAttributes(43, 12, 1, 1, 41, 12);
    actor->motionFlags = 0;
    actor->acceleration = toFIXED(2) / 5;
    actor->speedLimit = toFIXED(4) / 5;
    actor->y.fixed = toFIXED(8);
    Actor_SelectAnimation(actor, PAN_ANIMATION_FULL);
  } else {
    Actor_SelectAnimation(actor, PAN_ANIMATION_EMPTY);
  }
  Actor_Get(ACTOR_PIPE_EAST)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  Colosso_ShowItemIcon(ACTOR_PRIZE_A, ITEM_IRON_SHIELD);
  Colosso_ShowItemIcon(ACTOR_PRIZE_B, ITEM_PADDED_GLOVES);
  switch (gMainRoutineState.entrance) {
  case COLOSSEUM_AZART_MATCH_RACE_FROM_LEND_A_HAND:
    Colosso_StartRaceGauge(ACTOR_PARTY_LEADER, ACTOR_OPPONENT, 4, toFIXED(1304),
                           toFIXED(192), ACTOR_PRIZE_A, ACTOR_PRIZE_B);
    // The doorway Isaac sets out from, (19, 2..3), takes the two cells kept at
    // (127, 0..1) for the race.
    MapGrid_CopyCellsLayered(127, 0, 1, 2, 19, 2);
    Actor_ReleaseById(ACTOR_GUIDE);
    Actor_ReleaseById(ACTOR_ROCK_ATTENDANT);
    Actor_ReleaseById(ACTOR_PIPEWORKS_ATTENDANT);
    Actor_ReleaseById(ACTOR_BRIDGE_ATTENDANT);
    Actor_ReleaseById(ACTOR_LUMBER_ATTENDANT);
    if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
      Sound_Play(SOUND_FADE_OUT_BGM);
      Colosso_ControlCharacterAlone(ISAAC);
      Colosso_PlaceFriendsAtCheeringSpots();
      ColosseumAzartMatch_SetLayerOnVineCell(ACTOR_GARET);
      ColosseumAzartMatch_SetLayerOnVineCell(ACTOR_IVAN);
      ColosseumAzartMatch_SetLayerOnVineCell(ACTOR_MIA);
      Colosso_ShowOpeningLettering(FINALS_MATCH);
    }
    Actor_WatchAndSetScript(ACTOR_GARET, ACTOR_PARTY_LEADER);
    Actor_WatchAndSetScript(ACTOR_IVAN, ACTOR_PARTY_LEADER);
    Actor_WatchAndSetScript(ACTOR_MIA, ACTOR_PARTY_LEADER);
    Colosso_StartOpponent(RESOURCE_AZART_OPPONENT_PATH);
    break;
  case COLOSSEUM_AZART_MATCH_FROM_COLOSSEUM_WEST:
    Task_AddFrame(Colosso_RaiseTakePlaceOnPsynergy, TASK_PASS_FRAME);
    Actor_ReleaseById(ACTOR_PRIZE_A);
    Actor_ReleaseById(ACTOR_PRIZE_B);
    if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
      Colosso_PlaceFriendsAtCheeringSpots();
      Colosso_ControlCharacterAlone(GARET);
      Colosso_ShowOpeningLettering(LETTERING_LEND_A_HAND);
    }
    break;
  case COLOSSEUM_AZART_MATCH_FROM_COLOSSEUM:
    if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
      ColosseumAzartMatch_ExplainFinalsRules(ACTOR_GUIDE);
      Colosso_ClearCheeringSpots();
    }
    break;
  case COLOSSEUM_AZART_MATCH_AFTER_MATCH_WON:
    ColosseumAzartMatch_RestoreMatchResult(1);
    Import_EventSetExitRequest(COLOSSEUM_AZART_MATCH_TO_COLOSSEUM_EAST);
    break;
  case COLOSSEUM_AZART_MATCH_AFTER_MATCH_LOST:
    ColosseumAzartMatch_RestoreMatchResult(-1);
    Import_EventSetExitRequest(COLOSSEUM_AZART_MATCH_TO_BABI_PALACE);
    break;
  }
  return 0;
}

/// The guide gives the party the tour of Colosseum Final 1
/// (COLOSSEUM_AZART_MATCH_FROM_COLOSSEUM).
static void ColosseumAzartMatch_ExplainFinalsRules(s32 guide) {
  struct Actor *actor;
  s32 x;
  s32 z;

  actor = Actor_Get(guide);
  x = actor->x.part[0].integer;
  z = actor->z.part[0].integer;
  Event_Start();
  Actor_SetSpeed(guide, 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 1);
  Actor_SetSpeed(ACTOR_GARET, 1);
  Actor_SetSpeed(ACTOR_IVAN, 1);
  Actor_SetSpeed(ACTOR_MIA, 1);
  Actor_SetPositionFX(ACTOR_PARTY_LEADER, toFIXED(x), toFIXED(z) - toFIXED(48));
  Actor_SetPositionFX(ACTOR_GARET, toFIXED(x) - toFIXED(16),
                      toFIXED(z) - toFIXED(40));
  Actor_SetPositionFX(ACTOR_IVAN, toFIXED(x) + toFIXED(16),
                      toFIXED(z) - toFIXED(40));
  Actor_SetPositionFX(ACTOR_MIA, toFIXED(x), toFIXED(z) - toFIXED(32));
  Actor_SetPositionFX(guide, toFIXED(x), toFIXED(z) - toFIXED(80));
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  actor->facing = FACING_NORTH;
  FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_SetMessageId(MsgColossoSiteFirstFinalsBattle);
  // "This is the site of the first finals battle."
  Event_ShowMessage(guide, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  // "So this is where warriors test their mettle against each other."
  Event_ShowMessage(ACTOR_MIA, 0);
  Actor_StartRepeatedMotion(guide, 3);
  // "Well, no... It's actually more than that."
  Event_ShowMessage(guide, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  // "What else is involved?"
  Event_ShowMessage(ACTOR_IVAN, 0);
  Actor_FaceActor(guide, ACTOR_IVAN, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(guide, ANIM_NOD);
  // "The warriors are racing to get here first and get the best..."
  Event_ShowMessage(guide, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  // "So there's an advantage to getting here first? That's quite..."
  Event_ShowMessage(ACTOR_GARET, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  // "That's too hard for Isaac. We must support him as well we can..."
  Event_ShowMessage(ACTOR_MIA, 0);
  Actor_ShowEmote(guide, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  // Pan over each stage while the guide explains the finals rules.
  // "Um, don't you want to hear my explanation?"
  // Yes: "All right. Then let me explain in detail..."
  // No: "Ask the attendants for explanations of the stages."
  if (Event_AskYesNo(guide, 0) == 0) {
    do {
      Event_SetMessageId(MsgColossoWarriorsEnterFinalsWithoutAny);
      Actor_SetAnimation(ACTOR_IVAN, ANIM_NOD);
      Event_Wait(2);
      Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
      Event_Wait(2);
      Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
      Event_Wait(1);
      Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
      Actor_SetAnimationAndWait(guide, ANIM_NOD);
      // "Warriors enter the finals without any of their own equipment."
      Event_ShowMessage(guide, 0);
      Actor_FaceDirection(guide, FACING_NORTHWEST, 0);
      Event_Wait(20);
      // "Please look over there."
      Event_ShowMessage(guide, 0);
      FieldCamera_SetSpeed(3);
      FieldCamera_MoveTo(312, CAMERA_KEEP, 104, 1);
      FieldCamera_WaitForMove();
      // "That is the starting point of the first stage of the finals."
      Event_ShowMessage(guide, 0);
      FieldCamera_SetSpeed(1.5);
      FieldCamera_MoveTo(776, CAMERA_KEEP, 104, 1);
      // "You must get through several stages before you reach the arena."
      Event_ShowMessage(guide, 0);
      FieldCamera_WaitForMove();
      // "The chests along the way contain items that may be useful in..."
      Event_ShowMessage(guide, 0);
      FieldCamera_SetSpeed(3);
      FieldCamera_MoveTo(1240, CAMERA_KEEP, 168, 1);
      FieldCamera_WaitForMove();
      // BUG: FACING_SOUTHWEST is passed where an actor id belongs; no
      // actor has it, so the guide does not turn.
      Actor_FaceActor(guide, FACING_SOUTHWEST, 0);
      // "This is the battle arena. After getting here, the warriors..."
      Event_ShowMessage(guide, 0);
      FieldCamera_MoveTo(1304, CAMERA_KEEP, 168, 1);
      FieldCamera_WaitForMove();
      Actor_FaceActor(guide, ACTOR_PARTY_LEADER, 0);
      // "You can get weapons in the arena, too, but one is weaker..."
      Event_ShowMessage(guide, 0);
      // "So you'll have the advantage if you get here first."
      Event_ShowMessage(guide, 0);
      // "Also, you can use the equipment you get here in your next match."
      Event_ShowMessage(guide, 0);
      FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
      Actor_RunRepeatedMotion(guide, 2);
    } while (Event_AskYesNo(guide, 0) != 0);
    Actor_RunRepeatedMotion(guide, 2);
    Event_SetMessageId(MsgColossoAskAttendantsForExplanationsStages);
    // "Ask the attendants for explanations of the stages."
    Event_ShowMessage(guide, 0);
  }
  Event_SetMessageId(MsgColossoIsaacYoureContestantInFinals);
  Actor_RunRepeatedMotion(guide, 2);
  // "...Isaac, you're a contestant in the finals. Do not forget..."
  Event_ShowMessage(guide, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 0);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  // "Hey, where do we cheer from?"
  Event_ShowMessage(ACTOR_GARET, 0);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  // "Would you like to visit the stages and hear a description..."
  Event_ShowMessage(ACTOR_IVAN, 0);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  // "Yes. We'll decide where to cheer for Isaac after hearing the..."
  Event_ShowMessage(ACTOR_MIA, 0);
  Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
  Event_Wait(1);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Event_Wait(2);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_NOD);
  Event_Wait(1);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(6);
  Actor_SetAnimation(ACTOR_GARET, ANIM_WALK);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetDestination(ACTOR_GARET, actor->x.part[0].integer,
                         actor->z.part[0].integer);
  }
  Actor_SetAnimation(ACTOR_IVAN, ANIM_WALK);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetDestination(ACTOR_IVAN, actor->x.part[0].integer,
                         actor->z.part[0].integer);
  }
  Actor_SetAnimation(ACTOR_MIA, ANIM_WALK);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetDestination(ACTOR_MIA, actor->x.part[0].integer,
                         actor->z.part[0].integer);
  }
  Actor_WalkToAndWait(guide, x - 16, z - 64);
  Actor_SetPosition(ACTOR_GARET, 0, 0);
  Actor_SetPosition(ACTOR_IVAN, 0, 0);
  Actor_SetPosition(ACTOR_MIA, 0, 0);
  Actor_WalkToAndWait(guide, x - 16, z - 16);
  Actor_WalkToAndWait(guide, x, z);
  Actor_FaceDirection(guide, FACING_NORTH, 10);
  Event_End();
}

/// The Rock Challenge's attendant explains stage 1 of Colosseum Final 1.
static void ColosseumAzartMatch_TalkToRockAttendant(s32 actorId) {
  s32 path;

  if (gMainRoutineState.entrance == COLOSSEUM_AZART_MATCH_FROM_COLOSSEUM_WEST) {
    Colosso_AskDoneCheering(actorId);
  } else {
    Event_Start();
    path = Colosso_GreetAtStage(actorId, STAGE_ROCK_CHALLENGE);
    if (path == COLOSSO_GUIDE_DESCRIBE) {
      Event_SetMessageId(MsgColossoCallRockChallenge);
      FieldCamera_SetSpeed(3);
      FieldCamera_MoveTo(328, CAMERA_KEEP, 168, 1);
      FieldCamera_WaitForMove();
      // "They call this the Rock Challenge."
      Event_ShowMessage(actorId, 0);
      Colosso_FadeInWarrior(ACTOR_PARTY_LEADER, 280, 200);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, 1);
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 360, 200);
      Event_Wait(30);
      Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
      // "Warriors must create steps they can use to get through here."
      Event_ShowMessage(actorId, 0);
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 312, 200);
      Event_Wait(30);
      Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 10);
      Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB,
                      60);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, 1.5);
      Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 296, 184);
      Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 296, 152);
      Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 312, 152);
      Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 15);
      Colosso_PushActorAhead();
      Actor_ResetMotionFields(ACTOR_PARTY_LEADER);
      Colosso_PushActorAhead();
      Actor_ResetMotionFields(ACTOR_PARTY_LEADER);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, 1.5);
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 304, 184);
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 296, 192);
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 296, 200);
      Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 15);
      Colosso_PushActorAhead();
      Actor_ResetMotionFields(ACTOR_PARTY_LEADER);
      Colosso_PushActorAhead();
      Actor_ResetMotionFields(ACTOR_PARTY_LEADER);
      Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
      // "Warriors must use only the objects in this stage, along with..."
      Event_ShowMessage(actorId, 0);
      Colosso_RestoreWarrior(ACTOR_PARTY_LEADER);
      FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
      Actor_SetPosition(ACTOR_ROCK_A, 312, 168);
      Colosso_RepeatStageNumber(actorId, STAGE_ROCK_CHALLENGE);
    } else if (path == COLOSSO_GUIDE_OFFER_CHEERING) {
      Event_SetMessageId(MsgColossoObjectiveStageClear);
      // "The objective in this stage is to clear a path as quickly as..."
      Event_ShowMessage(actorId, 0);
    }
    Colosso_OfferCheeringSpot(path, actorId, STAGE_ROCK_CHALLENGE);
    Event_End();
  }
}

/// Moves a Pipeworks actor with the speed used throughout its
/// demonstration.
static inline void ColosseumAzartMatch_SetPipeHeight(struct Actor *actor,
                                                     FIXED height) {
  actor->motionFlags = 0;
  actor->acceleration = toFIXED(2) / 5;
  actor->speedLimit = toFIXED(4) / 5;
  FieldActor_SetMoveTargetFX(actor, actor->x.fixed, height, actor->z.fixed);
}

/// The Pipeworks' attendant explains stage 2 of Colosseum Final 1.
static void ColosseumAzartMatch_TalkToPipeworksAttendant(s32 attendant) {
  s32 path;
  struct Actor *second;

  if (gMainRoutineState.entrance == COLOSSEUM_AZART_MATCH_FROM_COLOSSEUM_WEST) {
    Colosso_AskDoneCheering(attendant);
  } else {
    Event_Start();
    path = Colosso_GreetAtStage(attendant, STAGE_PIPEWORKS);
    if (path == COLOSSO_GUIDE_DESCRIBE) {
      Event_SetMessageId(MsgColossoAreaCalledPipeworks);
      FieldCamera_SetSpeed(3);
      FieldCamera_MoveTo(592, CAMERA_KEEP, 120, 1);
      FieldCamera_WaitForMove();
      Event_Wait(60);
      FieldCamera_SetSpeed(1.5);
      FieldCamera_MoveTo(608, CAMERA_KEEP, 216, 1);
      FieldCamera_WaitForMove();
      // "This area is called the Pipeworks."
      Event_ShowMessage(attendant, 0);
      Colosso_ShowPointer(56, 64, 0);
      Event_Wait(60);
      Colosso_MovePointer(160, 96, 10);
      Event_Wait(70);
      // "See the water pipes? They actually form a kind of scale."
      Event_ShowMessage(attendant, 0);
      Colosso_HidePointer();
      wait(2);
      ColosseumAzartMatch_SetPipeHeight(Actor_Get(ACTOR_PIPE_WEST), toFIXED(8));
      second = Actor_Get(ACTOR_PIPE_EAST);
      ColosseumAzartMatch_SetPipeHeight(second, toFIXED(32));
      Actor_WaitForMove(second);
      Event_Wait(45);
      ColosseumAzartMatch_SetPipeHeight(Actor_Get(ACTOR_PIPE_WEST),
                                        toFIXED(24));
      second = Actor_Get(ACTOR_PIPE_EAST);
      ColosseumAzartMatch_SetPipeHeight(second, 0);
      Actor_WaitForMove(second);
      Event_Wait(15);
      // "When one side lowers, the other side rises."
      Event_ShowMessage(attendant, 0);
      Colosso_ShowPointer(56, 64, 0);
      Event_Wait(30);
      Colosso_MovePointer(160, 96, 10);
      Event_Wait(40);
      Colosso_MovePointer(56, 64, 10);
      Event_Wait(70);
      // "Using this premise, you must find a way to reach the next stage."
      Event_ShowMessage(attendant, 0);
      Colosso_HidePointer();
      wait(2);
      FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
      Colosso_RepeatStageNumber(attendant, STAGE_PIPEWORKS);
    } else if (path == COLOSSO_GUIDE_OFFER_CHEERING) {
      Event_SetMessageId(MsgColossoObjectiveMakeGood);
      // "The objective here is to make good use of the scales."
      Event_ShowMessage(attendant, 0);
    }
    Colosso_OfferCheeringSpot(path, attendant, STAGE_PIPEWORKS);
    Event_End();
  }
}

/// The Broken Bridge's attendant explains stage 3 of Colosseum Final 1.
static void ColosseumAzartMatch_TalkToBridgeAttendant(s32 actorId) {
  s32 path;

  if (gMainRoutineState.entrance == COLOSSEUM_AZART_MATCH_FROM_COLOSSEUM_WEST) {
    Colosso_AskDoneCheering(actorId);
  } else {
    Event_Start();
    path = Colosso_GreetAtStage(actorId, STAGE_BROKEN_BRIDGE);
    if (path == COLOSSO_GUIDE_DESCRIBE) {
      Event_SetMessageId(MsgColossoTheyCallBrokenBridge);
      ColosseumAzartMatch_ResetBrokenBridge();
      FieldCamera_SetSpeed(3);
      FieldCamera_MoveTo(840, CAMERA_KEEP, 216, 1);
      FieldCamera_WaitForMove();
      // "They call this the Broken Bridge."
      Event_ShowMessage(actorId, 0);
      ColosseumAzartMatch_StartBrokenBridge();
      Event_Wait(60);
      // "Carefully time your bridge crossings to reach the next stage."
      Event_ShowMessage(actorId, 0);
      Colosso_FadeInWarrior(ACTOR_PARTY_LEADER, 736, 200);
      COLOSSEUM_AZART_MATCH_FACE_DIRECTION(ACTOR_PARTY_LEADER, 0, 0);
      ColosseumAzartMatch_WaitForBridgeOpening();
      Actor_SetSpeed(ACTOR_PARTY_LEADER, 1);
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 816, 200);
      Event_Wait(30);
      Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
      // "If your timing is off, you'll fall down, which will cost you time."
      Event_ShowMessage(actorId, 0);
      Colosso_RestoreWarrior(ACTOR_PARTY_LEADER);
      FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
      Colosso_RepeatStageNumber(actorId, STAGE_BROKEN_BRIDGE);
    } else if (path == COLOSSO_GUIDE_OFFER_CHEERING) {
      Event_SetMessageId(MsgColossoOperatorBridgeWillAlsoCheer);
      // "The operator of the bridge will also cheer for you in this..."
      Event_ShowMessage(actorId, 0);
    }
    Colosso_OfferCheeringSpot(path, actorId, STAGE_BROKEN_BRIDGE);
    Event_End();
  }
}

/// The Lumber Water Crossing Area's attendant explains stage 4 of Colosseum
/// Final 1.
static void ColosseumAzartMatch_TalkToLumberAttendant(s32 actorId) {
  s32 path;

  if (gMainRoutineState.entrance == COLOSSEUM_AZART_MATCH_FROM_COLOSSEUM_WEST) {
    Colosso_AskDoneCheering(actorId);
    return;
  }
  Event_Start();
  path = Colosso_GreetAtStage(actorId, STAGE_LUMBER_WATER_CROSSING);
  if (path == COLOSSO_GUIDE_DESCRIBE) {
    Event_SetMessageId(MsgColossoPlaceNormallyCalledLumberWater);
    FieldCamera_SetSpeed(3);
    FieldCamera_MoveTo(1088, CAMERA_KEEP, 168, 1);
    FieldCamera_WaitForMove();
    // "This place is normally called the Lumber Water Crossing Area."
    Event_ShowMessage(actorId, 0);
    Colosso_ShowPointer(120, 72, 0);
    // "The obstacle at this stage is that large water tank."
    Event_ShowMessage(actorId, 0);
    Colosso_HidePointer();
    Event_Wait(15);
    Colosso_FadeInWarrior(ACTOR_PARTY_LEADER, 984, 200);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 10);
    // "If you can get all the way across in one go, you don't have to..."
    Event_ShowMessage(actorId, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 30);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 60);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 1.5);
    Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 1000, 192);
    Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 1000, 176);
    Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 1016, 168);
    Event_Wait(15);
    COLOSSEUM_AZART_MATCH_PLACE_STAGED_PAIR(ACTOR_LOG_EAST_WEST,
                                            LOG_EAST_WEST_FLOAT, 0);
    FieldCamera_MoveTo(1088, CAMERA_KEEP, 168, 1);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
    Event_Wait(10);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 1);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 1192, 168);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 30);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
    // "But if you want to reach the treasure box, it'll take a little..."
    Event_ShowMessage(actorId, 0);
    Colosso_RestoreWarrior(ACTOR_PARTY_LEADER);
    FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
    Actor_SetPosition(ACTOR_LOG_EAST_WEST, 1016, 168);
    Colosso_RepeatStageNumber(actorId, STAGE_LUMBER_WATER_CROSSING);
  } else if (path == COLOSSO_GUIDE_OFFER_CHEERING) {
    Event_SetMessageId(MsgColossoLogsKeyClearingStage);
    // "The logs are the key to clearing this stage."
    Event_ShowMessage(actorId, 0);
  }
  Colosso_OfferCheeringSpot(path, actorId, STAGE_LUMBER_WATER_CROSSING);
  Event_End();
}

#include "shared/colosso_solo.c"
#include "shared/item_equip.c"
#include "shared/colosso_task.c"

#include "shared/colosso_lettering.c"

#include "shared/colosso_guide.c"
#include "shared/colosso_item_icon.c"

#include "shared/colosso_hex_string.c"
#include "shared/colosso_finish.c"

#include "shared/colosso_path_opponent.c"
#include "shared/colosso_setup.c"
#include "shared/colosso_map_state.c"
#include "shared/actor_find_at_position.c"

#include "shared/colosso_push.c"

#include "shared/colosso_lettering_tables.c"
// clang-format off

char gHexDigits[20] = "0123456789ABCDEF";

#include "shared/colosso_prize_scripts.c"

#include "shared/pushed_actor_direction_steps.c"

struct SceneEntrance gColosseumAzartMatch_Entrances[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 920, 0, 200, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { COLOSSEUM_AZART_MATCH_RACE_FROM_LEND_A_HAND, CONDITION_ALWAYS, 312, 0, 104, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

u32 gColosseumAzartMatch_Exits[] = {
    SCENE_EXITS(ColosseumAzartMatch),
    SCENE_EXIT(COLOSSEUM_AZART_MATCH_LEND_A_HAND_TO_RACE, ColosseumAzartMatch, COLOSSEUM_AZART_MATCH_RACE_FROM_LEND_A_HAND),
    SCENE_EXIT(COLOSSEUM_AZART_MATCH_TO_COLOSSEUM_WEST, Colosseum, COLOSSEUM_FROM_COLOSSEUM_AZART_MATCH_WEST),
    SCENE_EXIT(COLOSSEUM_AZART_MATCH_TO_COLOSSEUM_EAST, Colosseum, COLOSSEUM_FROM_COLOSSEUM_AZART_MATCH_EAST),
    SCENE_EXIT(COLOSSEUM_AZART_MATCH_TO_BABI_PALACE, BabiPalace, BABI_PALACE_FROM_COLOSSEUM_AZART_MATCH),
    SCENE_EXITS_END,
};

struct ActorPlacement gColosseumAzartMatch_ActorPlacements[] = {
    ACTOR_PLACEMENT(ACTOR_PARTY_LEADER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        312, 0, 104, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        312, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        408, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        456, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, 7,
        601, 0, 146, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, 7,
        552, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, 7,
        664, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        760, 0, 194, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        944, 0, 216, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, 7,
        1048, 0, 136, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7,
        1016, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        360, 0, 424, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        376, 0, 312, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        600, 0, 312, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        824, 0, 312, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        1080, 0, 312, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        1304, 0, 176, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        1304, 0, 208, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

/// The frames left in the broken bridge's cycle (BRIDGE_CYCLE_FRAMES).
u32 gColosseumAzartMatch_BridgeTimer = 0;

struct Trigger gColosseumAzartMatch_Triggers[] = {
    { STEP_TRIGGER_WALKING, CELL_BEHAVIOR_ROCK_CHALLENGE_SQUARE_EDGE, CONDITION_ALWAYS, EVENT(ColosseumAzartMatch_PushRockOrHopAcross) },
    { STEP_TRIGGER_WALKING, CELL_BEHAVIOR_ROCK_CHALLENGE_FLOOR, CONDITION_ALWAYS, EVENT(ColosseumAzartMatch_PushRock) },
    { STEP_TRIGGER_WALKING, CELL_BEHAVIOR_ROCK_CHALLENGE_EAST_END, CONDITION_ALWAYS, EVENT(ColosseumAzartMatch_PushRock) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_PIPEWORKS_PIPE, FLAG_PIPEWORKS_PIPE_PUSHED, EVENT(ColosseumAzartMatch_PushPipeworksPipe) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_LOG_NORTH_SOUTH, CONDITION_ALWAYS, EVENT(ColosseumAzartMatch_RollLogNorthSouth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_LOG_NORTH_SOUTH, CONDITION_ALWAYS, EVENT(ColosseumAzartMatch_RollLogNorthSouth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_LOG_EAST_WEST, CONDITION_ALWAYS, EVENT(ColosseumAzartMatch_RollLogEastWest) },
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_LOG_EAST_WEST, CONDITION_ALWAYS, EVENT(ColosseumAzartMatch_RollLogEastWest) },
    { TRIGGER_STEP, CELL_BEHAVIOR_ARENA_ENTRY, FLAG_COLOSSEUM_ARENA_CLOSED, EVENT(ColosseumAzartMatch_ReachArena) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), CELL_BEHAVIOR_FIRST_NUT_CHEST, FLAG_COLOSSEUM_AZART_MATCH_FIRST_NUT_CHEST_OPENED, FIND_ITEM(ITEM_NUT) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), CELL_BEHAVIOR_OIL_DROP_CHEST, FLAG_COLOSSEUM_AZART_MATCH_OIL_DROP_CHEST_OPENED, FIND_ITEM(ITEM_OIL_DROP) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST) | TRIGGER_FLAG_FACING | FACING_WEST, CELL_BEHAVIOR_SECOND_NUT_CHEST, FLAG_COLOSSEUM_AZART_MATCH_SECOND_NUT_CHEST_OPENED, FIND_ITEM(ITEM_NUT) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ROCK_A, CONDITION_ALWAYS, EVENT(ColosseumAzartMatch_UpdateRockCells) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ROCK_B, CONDITION_ALWAYS, EVENT(ColosseumAzartMatch_UpdateRockCells) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ROCK_C, CONDITION_ALWAYS, EVENT(ColosseumAzartMatch_UpdateRockCells) },
    { PSYNERGY_ON_ACTOR(ABILITY_GROWTH), ACTOR_GROWTH_PLANT, FLAG_PLANT_GROWN, EVENT(ColosseumAzartMatch_GrowPlant) },
    { PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_PIPE_WEST, FLAG_PIPES_MOVED, EVENT(ColosseumAzartMatch_DousePipe) },
    { TRIGGER_TALK, ACTOR_GUIDE, CONDITION_ALWAYS, EVENT(Colosso_AskDoneLookingAround) },
    { TRIGGER_TALK, ACTOR_ROCK_ATTENDANT, CONDITION_ALWAYS, EVENT(ColosseumAzartMatch_TalkToRockAttendant) },
    { TRIGGER_TALK, ACTOR_PIPEWORKS_ATTENDANT, CONDITION_ALWAYS, EVENT(ColosseumAzartMatch_TalkToPipeworksAttendant) },
    { TRIGGER_TALK, ACTOR_BRIDGE_ATTENDANT, CONDITION_ALWAYS, EVENT(ColosseumAzartMatch_TalkToBridgeAttendant) },
    { TRIGGER_TALK, ACTOR_LUMBER_ATTENDANT, CONDITION_ALWAYS, EVENT(ColosseumAzartMatch_TalkToLumberAttendant) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GUIDE, CONDITION_ALWAYS, MSGNO(MsgColossoRiverThisYearsFinalsAreIncredibleThis) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROCK_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoRiverMovingSuchAHugeRockMust) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PIPEWORKS_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoRiverTheresOnlyOneFinalAndThis) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BRIDGE_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoRiverDidYouSeeHisMovesOut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LUMBER_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoRiverHowManyWarriorsHaveSwallowedTheir) },
    { TRIGGER_RAISED, COLOSSO_TAKE_PLACE_TRIGGER, CONDITION_ALWAYS,
      EVENT(Colosso_TakePlaceForMatch) },
    { TRIGGER_TABLE_END },
};

/// The VRAM cache entry holding the lettering's tiles, -1 when none is loaded.
s16 gColosso_LetteringSlot = -1;

#include "shared/colosso_lettering_scripts.c"
// clang-format on

/// The VRAM cache entry holding the marker's tiles, -1 when none is loaded.
s16 gColosso_PointerSlot = -1;

#include "shared/colosso_finish_scripts.c"

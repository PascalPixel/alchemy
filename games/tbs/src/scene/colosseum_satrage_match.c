/// @file
/// Colosseum Final 2 (SceneId_ColosseumSatrageMatch) is the site of the second
/// finals match, where Isaac races Satrage through five stages to the arena
/// and fights him there.
///
/// Isaac has won the first match, and Satrage is the fourth-ranked warrior.
///
/// It works as Colosseum Final 1 does (colosseum_azart_match.c). The stages
/// run from east to west, one after another: the Mini-Maze, where Isaac starts
/// among three stone pillars; the shifting floor stage, four lifts across a
/// channel of water; the Log-Rolling Stage, two logs to ride across a pool;
/// The Wall, a high stone wall to climb, with a row of stone posts at its foot;
/// and the Board Walk, a pool whose logs lie across the way, with a channel of
/// white platforms along its south edge. The arena is in the far west.
///
/// Isaac comes here three times, each at its own entrance.
/// Scene_Initialize restores what the friends changed on the stages from their
/// temporary flags (FLAG_LOG_KNOCKED_DOWN to FLAG_WALL_PILLAR_MOVED, and the
/// Mini-Maze pillars' columns from FLAG_MINI_MAZE_FIRST_PILLAR_COLUMN). Then
/// it sets up the visit its entrance stands for.
///
/// - COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_WEST, the tour
///   (ColosseumSatrageMatch_IntroduceSecondFinals). The guide only greets the
///   party, since the first finals match's guide explained the rules. The five
///   stages' attendants describe their stages and offer a place for each
///   friend to cheer. The guide then sends Isaac back into the Colosseum
///   (COLOSSEUM_SATRAGE_MATCH_TO_COLOSSEUM_SOUTH).
/// - COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_EAST, `Lend a Hand`. Garet, Ivan
///   and Mia in turn may use Psynergy on the stages. Move shifts the
///   Mini-Maze's stone pillars and the stone pillar at the foot of The Wall.
///   Halt stops the lifts' operator and with him the shifting floor. Frost
///   freezes the Log-Rolling Stage's two puddles into pillars of ice. Growth
///   grows the sprout at the foot of The Wall into a vine. Force knocks the
///   tall log by the Board Walk's channel over into the water. After Mia,
///   COLOSSEUM_SATRAGE_MATCH_LEND_A_HAND_TO_RACE starts the race.
/// - COLOSSEUM_SATRAGE_MATCH_RACE_FROM_LEND_A_HAND, the race against Satrage,
///   who follows a recorded path, after the lettering `The 2nd Match` and a
///   count of three. At the arena the first to arrive takes the Chain Mail and
///   the second the Cotton Shirt. The battle is ENCOUNTER_ZONE_COLOSSO's third
///   or fourth group, the Satrage with or without the mail.
///
/// A win returns to COLOSSEUM_SATRAGE_MATCH_AFTER_MATCH_WON and on to the
/// Colosseum (COLOSSEUM_SATRAGE_MATCH_TO_COLOSSEUM_NORTH, arriving at
/// COLOSSEUM_FROM_COLOSSEUM_SATRAGE_MATCH_NORTH), which sets
/// FLAG_COLOSSO_SECOND_MATCH_WON.
///
/// A loss returns to COLOSSEUM_SATRAGE_MATCH_AFTER_MATCH_LOST, and
/// COLOSSEUM_SATRAGE_MATCH_TO_BABI_PALACE wakes the party in Babi's Palace,
/// where Isaac's friends tell him he was only dreaming of the finals.
///
/// The shifting floor's four lifts run on every visit until Halt stops them
/// (ColosseumSatrageMatch_UpdateLifts). Every
/// COLOSSEUM_SATRAGE_MATCH_LIFT_CYCLE_FRAMES they change places: for a while
/// all four stand in a line across the channel, cells (58, 13) to (64, 13),
/// and Isaac can hop from one to the next; then the first and third slide
/// north and the second and fourth south, and the line is broken. Only while
/// they stand still in a line are their cells squares to hop onto
/// (CELL_BEHAVIOR_JUMP_FORWARD); otherwise they are walls.

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
#include "field/event.h"
#include "field/event_commands.h"
#include "field/event_inline.h"
#include "field/field.h"
#include "field/field_psynergy_work.h"
#include "field/field_sequence.h"
#include "field/pushed_actor.h"
#include "field/scene_tables.h"
#include "field/script_operands.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "scene/colosso_gauge.h"
#include "scene/colosso_marker.h"
#include "scene/colosso_occupant.h"
#include "scene/colosso_setup.h"
#include "scene/colosso_solo.h"
#include "scene/import.h"
#include "sound/sound.h"
#include "sound/sound_driver_symbols.h"
#include "system/heap_slots.h"
#include "system/input.h"
#include "system/io_write_queue.h"
#include "system/main_routine.h"
#include "system/main_routine_work.h"
#include "system/system.h"
#include "system/vram_cache.h"
#include "window/screens.h"
#include "window/window.h"
#include "text/msg_ids.h"

s16 gColosso_MarkerStep BSS_DATA;
s16 gColosso_ModeScaleDuration BSS_DATA ALIGNED(4);
s16 gColosso_MarkerFlip BSS_DATA ALIGNED(4);
/// Isaac's z before he is shown on the stage, 16.16 fixed-point pixels.
FIXED gColosso_WarriorStartZ BSS_DATA;
s16 gColosso_MarkerEndX BSS_DATA;
s16 gColosso_LetteringParameter BSS_DATA ALIGNED(4);
s16 gColosso_ModeScaleStart BSS_DATA ALIGNED(4);
s16 gColosso_ModeMoveDuration BSS_DATA ALIGNED(4);
s32 gColosso_LetteringPosition BSS_DATA;
s16 gColosso_MarkerBlink BSS_DATA;
s16 gColosso_ModeScaleTarget BSS_DATA ALIGNED(4);
s16 gColosso_ModeMoveStep BSS_DATA ALIGNED(4);
s16 gColosso_MarkerY BSS_DATA ALIGNED(4);
s16 gColosso_ModeBlendStep BSS_DATA ALIGNED(4);
/// Isaac's facing before Colosso_FadeInWarrior, which
/// Colosso_RestoreWarrior puts back.
s32 gColosso_WarriorStartAngle BSS_DATA;
s16 gColosso_MarkerSteps BSS_DATA;
s16 gColosso_LetteringMode BSS_DATA ALIGNED(4);
s16 gColosso_ModeBlendTarget BSS_DATA ALIGNED(4);
s16 gColosso_ModeBlendStart BSS_DATA ALIGNED(4);
s16 gColosso_LetteringTimer BSS_DATA ALIGNED(4);
s16 *gColosso_LetteringScript BSS_DATA;
union MarkerCoordinate gColosso_MarkerStartX BSS_DATA;
s16 gColosso_ModeBlendDuration BSS_DATA ALIGNED(4);
u8 gColosseumSatrageMatch_UnusedAfterBlendDuration[6] BSS_DATA;
/// The blinking pointer's sprite part: the queue link and attributes 0 to 2.
s32 gColosso_PointerOam[3] BSS_DATA;
union MarkerCoordinate gColosso_MarkerStartY BSS_DATA;
/// The lettering's sprite parts, three words each, written every frame.
u32 gColosso_LetteringSprites[12] BSS_DATA;
s16 gColosso_ModeMoveStart BSS_DATA;
s16 gColosso_MarkerX BSS_DATA ALIGNED(4);
s16 gColosso_ModeMoveTarget BSS_DATA ALIGNED(4);
s16 gColosso_ModeScaleStep BSS_DATA ALIGNED(4);
s16 gColosso_MarkerEndY BSS_DATA ALIGNED(4);
/// Isaac's x before he is shown on the stage, 16.16 fixed-point pixels.
FIXED gColosso_WarriorStartX BSS_DATA;

s32 Scene_Initialize(void);
// UB: the scene calls this before its definition with no prototype, as a
// function returning int.
#ifdef UBFIX
void ColosseumSatrageMatch_StartRaceGauge(s32 leaderId, s32 opponentId,
                                          s32 segments, s32 finishX,
                                          FIXED finishZ, s32 firstItem,
                                          s32 secondItem);
#endif
extern const u8 gColosseumSatrageMatch_RollLogScript[];
extern const u8 gColosso_LetteringPaletteOffsets[];
extern const s16 gColosso_LetteringDigit[];
extern const s16 gColosso_LetteringLose[];
extern s32 gColosso_PrizeSparkleScript[];
extern s32 gColosso_PrizeScript[];
extern struct SceneEntrance gColosseumSatrageMatchEntrances[];
extern u32 gColosseumSatrageMatchExits[];
extern struct ActorPlacement gColosseumSatrageMatchActorPlacements[];
extern s32 gColosseumSatrageMatch_LiftsApart;
extern u32 gColosseumSatrageMatch_LiftTimer;
extern struct Trigger gColosseumSatrageMatchTriggers[];
extern s16 gColosso_LetteringTitle[];
extern s16 gColosso_LetteringGo[];
extern s16 gColosso_LetteringWin[];
extern s32 gColosso_OpponentFinishScript[];
extern s32 gColosso_LeaderFinishScript[];
extern char gHexDigits[20];
extern s32 gPushedActor_DirectionSteps[];
extern s16 gColosso_LetteringSlot;
extern s16 gColosso_MarkerSlot;

#define COLOSSEUM_SATRAGE_MATCH_LIFT_CYCLE_FRAMES 120
#define COLOSSEUM_SATRAGE_MATCH_LIFT_CROSSING_FRAME 75
#define COLOSSEUM_SATRAGE_MATCH_DEMONSTRATION_TIMEOUT_FRAMES 600
#define COLOSSEUM_SATRAGE_MATCH_LOG_TRAVEL_PIXELS 64
/// The map grid rows the two riding logs lie across, from their first row.
#define RIDING_LOG_NORTH_ROW 10
#define RIDING_LOG_SOUTH_ROW 13
/// The falling log's spin, in 1/65536 turns a frame: it speeds up by
/// FALL_SPIN_ACCELERATION each frame, from 0 as it tips and from
/// SETTLE_SPIN_START as it drops into the water.
#define FALL_SPIN_ACCELERATION 36
#define SETTLE_SPIN_START 360
/// The column the stone pillar by The Wall stands in once pushed, a cell east
/// of its place (568, 264).
#define WALL_PILLAR_MOVED_COLUMN 36

// UB: westward log travel passes -64, which cannot be shifted left as signed C.
// Both callers supply only -64, 0 or 64, so fixed-point multiplication fits.
#ifdef UBFIX
#define COLOSSEUM_SATRAGE_MATCH_OFFSET_FIXED(value) ((value) * toFIXED(1))
#else
#define COLOSSEUM_SATRAGE_MATCH_OFFSET_FIXED(value) toFIXED(value)
#endif

// UB: each of these calls a void function through an s32 function type, the
// call the bytes show; UBFIX calls it as declared.
#ifdef UBFIX
#define COLOSSEUM_SATRAGE_MATCH_SHOW_MESSAGE Event_ShowMessageAndWait
#define COLOSSEUM_SATRAGE_MATCH_MOVE_RIDING_LOG                                \
  ColosseumSatrageMatch_MoveRidingLog
#else
#define COLOSSEUM_SATRAGE_MATCH_SHOW_MESSAGE                                   \
  ((s32 (*)())Event_ShowMessageAndWait)
#define COLOSSEUM_SATRAGE_MATCH_MOVE_RIDING_LOG                                \
  ((s32 (*)())ColosseumSatrageMatch_MoveRidingLog)
#endif

static void ColosseumSatrageMatch_IntroduceSecondFinals(s32 attendantId);
static void Colosso_ShowOpeningLettering(s32 match);
static void Colosso_ShowMatchResult(s32 result);
static void Colosso_NoOp(void);
static void Colosso_WaitForOpponentToFinish(void);
static s32 Colosso_TakePrize(s32 firstItem, s32 secondItem);
static void ColosseumSatrageMatch_StartOpponentPath(s32 pathResource);
static void Colosso_PushActorAhead(void);

/// The second finals match's actors, numbered in the order of
/// gColosseumSatrageMatchActorPlacements from ACTOR_FIRST_PLACED.
///
/// The rows of Isaac, Garet, Ivan and Mia take their own actors.
///
/// By the guide's and attendants' explanations, the Mini-Maze, the shifting
/// floor stage, the Log-Rolling Stage, The Wall and the Board Walk lead to the
/// arena. The stone pillars are drawn as the stone pillars Move pushes
/// elsewhere.
enum ColosseumSatrageMatchActorId {
  /// Satrage, racing Isaac to the arena.
  ACTOR_OPPONENT = ACTOR_FIRST_PLACED,
  /// The Board Walk's two stone pillars, on the ledge along the north edge of
  /// its pool. The eastern one stands before the narrow channel that holds a
  /// chest. Move does nothing to them.
  ACTOR_BOARD_WALK_WEST_PILLAR,
  ACTOR_BOARD_WALK_EAST_PILLAR,
  /// The stone pillar on the ledge at the foot of The Wall, beside the chest
  /// holding a Smoke Bomb.
  ACTOR_WALL_PILLAR,
  /// The Mini-Maze's three stone pillars, in a row along the stage's south
  /// edge where the race starts, which Move shifts. Scene_Initialize stands
  /// them west to east at columns 73, 76 and 79 until they have been moved.
  ACTOR_MINI_MAZE_FIRST_PILLAR,
  /// The Board Walk's two logs, the western and the eastern, lying north to
  /// south in its pool below the stone pillars' ledge. A third,
  /// ACTOR_ROLLING_LOG_CENTER, is placed at (0, 0) and never moved there.
  /// ColosseumSatrageMatch_UnusedRollCenterLogNorth, its South twin and its row
  /// in the collision rebuild remain for it.
  ACTOR_ROLLING_LOG_WEST = ACTOR_MINI_MAZE_FIRST_PILLAR + 3,
  ACTOR_ROLLING_LOG_EAST,
  ACTOR_ROLLING_LOG_CENTER,
  /// What Force is cast at to knock ACTOR_FALLING_LOG over: the tall log's
  /// place, at the east end of the Board Walk's channel.
  ACTOR_FORCE_TARGET,
  /// The sprout at the foot of The Wall, which Growth grows into a vine.
  ACTOR_GROWTH_PLANT,
  /// The Log-Rolling Stage's two puddles, which Frost freezes. They are drawn
  /// as the puddle Frost freezes in Lunpa (lunpa.h's SPRITE_PUDDLE).
  ACTOR_PUDDLE_WEST,
  ACTOR_PUDDLE_EAST,
  /// The shifting floor stage's four lifts, from west to east, which the
  /// lifts' operator runs.
  ACTOR_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT,
  ACTOR_COLOSSEUM_SATRAGE_MATCH_LAST_LIFT =
          ACTOR_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT +
      3,
  /// Four platforms in the lifts' sprite laid across the water channel at the
  /// Board Walk's south edge, drawn underfoot. Nothing but
  /// Scene_Initialize touches them.
  ACTOR_BOARD_WALK_FIRST_PLATFORM,
  /// The tall log standing on end at the east end of the Board Walk's
  /// channel, which Force on ACTOR_FORCE_TARGET knocks over into the water.
  ACTOR_FALLING_LOG = ACTOR_BOARD_WALK_FIRST_PLATFORM + 4,
  /// A young man with fair hair who works the shifting floor stage's lifts.
  ACTOR_LIFT_OPERATOR,
  /// The Log-Rolling Stage's two logs.
  ACTOR_RIDING_LOG_NORTH,
  ACTOR_RIDING_LOG_SOUTH,
  /// A soldier who greets the party at the second finals match.
  ACTOR_GUIDE,
  /// The stages' attendants, young men with fair hair in white tunics.
  ACTOR_MINI_MAZE_ATTENDANT,
  ACTOR_SHIFTING_FLOOR_ATTENDANT,
  ACTOR_LOG_ROLLING_ATTENDANT,
  ACTOR_WALL_ATTENDANT,
  ACTOR_BOARD_WALK_ATTENDANT,
  /// The arena's prizes, drawn as item icons at two blank actors
  /// (Colosso_ShowItemIcon).
  ACTOR_PRIZE_A,
  ACTOR_PRIZE_B
};

/// The temporary flags that keep what the friends changed on the stages, for
/// the race.
///
/// Scene_Initialize restores them.
enum {
  FLAG_LOG_KNOCKED_DOWN = FLAG_AREA_TEMP_30,   ///< by Force, on the Board Walk
  FLAG_WEST_PUDDLE_FROZEN = FLAG_AREA_TEMP_31, ///< by Frost
  FLAG_EAST_PUDDLE_FROZEN = FLAG_AREA_TEMP_32, ///< by Frost
  FLAG_PLANT_GROWN = FLAG_AREA_TEMP_33,        ///< by Growth, up The Wall
  FLAG_LIFTS_HALTED = FLAG_AREA_TEMP_34, ///< by Halt on ACTOR_LIFT_OPERATOR
  /// The stone pillar at the foot of The Wall stands a cell east of the chest.
  FLAG_WALL_PILLAR_MOVED = FLAG_AREA_TEMP_35,
  /// The bytes of flags that keep the Mini-Maze's three pillars' columns.
  FLAG_MINI_MAZE_FIRST_PILLAR_COLUMN = FLAG_AREA_TEMP_40,
  FLAG_MINI_MAZE_SECOND_PILLAR_COLUMN = FLAG_AREA_TEMP_48,
  FLAG_MINI_MAZE_THIRD_PILLAR_COLUMN = FLAG_AREA_TEMP_50
};

/// SPRITE_FALLING_LOG's animations (falling_log.tsv).
enum FallingLogAnimation {
  FALLING_LOG_ANIMATION_STANDING = 3, ///< standing on end
  /// Lying across the water, bobbing.
  FALLING_LOG_ANIMATION_FALLEN = 4
};

/// The cell behaviours the stages' step triggers answer.
enum ColosseumSatrageMatchCellBehavior {
  /// The ledge at the foot of The Wall, cell (37, 16), a cell east of where
  /// Move leaves the stone pillar.
  CELL_BEHAVIOR_WALL_LEDGE = 20,
  /// The Board Walk's ledge either side of its two stone pillars, cells
  /// (13, 10) to (17, 10) and (21, 10) to (24, 10).
  CELL_BEHAVIOR_BOARD_WALK_LEDGE = 21,
  /// Under the Log-Rolling Stage's logs, three cells each, moved with them.
  CELL_BEHAVIOR_RIDING_LOG = 22,
  /// Beside the Board Walk's western log: east of it where it starts, cells
  /// (17, 11) and (17, 14), and west of it once rolled, in column 13.
  CELL_BEHAVIOR_WEST_LOG_EAST_SIDE = 24,
  CELL_BEHAVIOR_WEST_LOG_WEST_SIDE = 25,
  /// A second ledge cell for the stone pillar at The Wall. No cell of the map
  /// holds it.
  CELL_BEHAVIOR_WALL_LEDGE_UNUSED = 26,
  /// Beside the Board Walk's eastern log: west of it once rolled, in column
  /// 21, and east of it where it starts, in column 25.
  CELL_BEHAVIOR_EAST_LOG_WEST_SIDE = 27,
  CELL_BEHAVIOR_EAST_LOG_EAST_SIDE = 28,
  /// The top of the stone post at the foot of The Wall just west of the
  /// sprout, cell (31, 16).
  CELL_BEHAVIOR_WALL_POST = 30,
  /// The east edge of the arena, cells (8, 13) to (8, 18).
  CELL_BEHAVIOR_ARENA = 50
};

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

/// Move on either of the Board Walk's two stone pillars, on the ledge along
/// the north edge of its pool, does nothing.
static void ColosseumSatrageMatch_IgnoreMove(void) {}

/// Walking along the Board Walk's ledge, on the north edge of its pool, into
/// one of its two stone pillars pushes the pillar a cell further along, if
/// nothing stands beyond it.
///
/// Pushed from either side, neither pillar moves when tried, so the eastern
/// one keeps closed the narrow channel north of it that leads to a chest
/// holding a Nut, cell (23, 7).
static void ColosseumSatrageMatch_PushBoardWalkPillar(void) {
  Colosso_PushActorAhead();
}

/// Move on the stone pillar at the foot of The Wall slides it a cell east,
/// away from the chest holding a Smoke Bomb, on the ledge at cell (35, 16).
///
/// After Move and after each push, FLAG_WALL_PILLAR_MOVED records whether the
/// pillar stands in WALL_PILLAR_MOVED_COLUMN, and cell (35, 77) is opened or
/// walled to match. Scene_Initialize opens that cell again for the race but
/// does not move the pillar: it is back beside the chest, as a probe of the
/// race with the flag set shows.
///
/// Cell (35, 77) lies below the course the screen shows, whose rows run from
/// 0 to 31; the map stores it as a wall, and the flag makes it open ground
/// (a probe with and without the flag). XXX: what reads cell (35, 77) and
/// what that changes for the player; nothing in this file copies it onward,
/// so a search of the shared Colosso code for row 77, then a race recorded
/// with FLAG_WALL_PILLAR_MOVED set and Isaac climbing at column 35, would
/// settle it.
static void ColosseumSatrageMatch_MoveWallPillar(void) {
  s32 column;
  s32 row;

  if (FIELD_CELL(Actor_Get(ACTOR_WALL_PILLAR)->x.fixed) ==
      WALL_PILLAR_MOVED_COLUMN) {
    GameFlag_Set(FLAG_WALL_PILLAR_MOVED);
    column = 35;
    row = 77;
    MapGrid_CopyCellAttributes(35, 78, 1, 1, column, row);
  } else {
    GameFlag_Clear(FLAG_WALL_PILLAR_MOVED);
    column = 35;
    row = 77;
    MapGrid_CopyCellAttributes(34, 77, 1, 1, column, row);
  }
}

/// Walking west along the ledge at the foot of The Wall, from cell (37, 16),
/// pushes the stone pillar back a cell west, beside the chest, when Move has
/// left it in the next cell.
///
/// With the pillar in its first place the next cell is empty and nothing
/// happens.
static void ColosseumSatrageMatch_PushWallPillar(void) {
  Colosso_PushActorAhead();
  ColosseumSatrageMatch_MoveWallPillar();
}

/// Move on one of the three stone pillars along the Mini-Maze's south edge,
/// where the race starts, slides it a cell. The walkthrough's advice is for
/// Garet to move the middle one, to save Isaac time.
///
/// Each pillar's top is a square to hop onto (CELL_BEHAVIOR_JUMP_FORWARD), and
/// its column is kept in a byte of flags so that the pillars stand where Move
/// left them in the race. The cell a pillar leaves keeps its square until the
/// scene is entered again. The first copy puts back the Board Walk's log cells
/// as the map had them, without the logs' own rows; nothing rolls those logs
/// before the race.
static void ColosseumSatrageMatch_MoveMiniMazePillar(void) {
  s32 column;

  MapGrid_CopyCellAttributes(100, 11, 12, 4, 14, 11);

  column = FIELD_CELL(Actor_Get(ACTOR_MINI_MAZE_FIRST_PILLAR)->x.fixed);
  FieldGameFlag_SetByte(FLAG_MINI_MAZE_FIRST_PILLAR_COLUMN, column);
  MapGrid_CopyCellAttributes(71, 16, 1, 1, column, 16);

  column = FIELD_CELL(Actor_Get(ACTOR_MINI_MAZE_FIRST_PILLAR + 1)->x.fixed);
  FieldGameFlag_SetByte(FLAG_MINI_MAZE_SECOND_PILLAR_COLUMN, column);
  MapGrid_CopyCellAttributes(71, 16, 1, 1, column, 16);

  column = FIELD_CELL(Actor_Get(ACTOR_MINI_MAZE_FIRST_PILLAR + 2)->x.fixed);
  FieldGameFlag_SetByte(FLAG_MINI_MAZE_THIRD_PILLAR_COLUMN, column);
  MapGrid_CopyCellAttributes(71, 16, 1, 1, column, 16);
}

/// Nothing visible happens when Force is cast at the tall log by the Board
/// Walk's channel, just before the fist flies. The scene sets the Psynergy's
/// `childMode`, so that the fist does not shove the log 16 pixels away as it
/// shoves what else it hits, and the log topples from where it stands
/// (ColosseumSatrageMatch_KnockDownLog).
static void ColosseumSatrageMatch_KeepLogStillForForce(void) {
  u8 *effectWork = gFieldEffectWork;

  ((struct FieldEffectWork *)effectWork)->childMode = 1;
}

/// Force on the tall log standing at the east end of the Board Walk's channel
/// knocks it over west into the water, where it lies beside the white
/// platforms as one more step across, cell (20, 16), on the way to the arena.
static void ColosseumSatrageMatch_KnockDownLog(void) {
  struct Actor *obstacle;
  s32 fallFrames;
  s32 rotationStep;
  s32 settleFrames;
  s32 settleRotationStep;
  struct Sprite *sprite;

  obstacle = Actor_Get(ACTOR_FALLING_LOG);
  sprite = obstacle->sprite;
  Event_SetFlag(FLAG_LOG_KNOCKED_DOWN);
  obstacle->acceleration = toFIXED(1) / 10;
  obstacle->speedLimit = toFIXED(6) / 5;
  Sound_Play(SE_TOPPLE);
  FieldActor_SetMoveTarget(obstacle, 336, 10, 264);
  for (rotationStep = 0, fallFrames = 0; fallFrames < 10; fallFrames++) {
    sprite->rotation -= rotationStep;
    wait(1);
    rotationStep += FALL_SPIN_ACCELERATION;
  }
  FieldActor_SetMoveTarget(obstacle, 330, -16, 264);
  for (settleRotationStep = SETTLE_SPIN_START, settleFrames = 0;
       settleFrames < 22; settleFrames++) {
    sprite->rotation -= settleRotationStep;
    wait(1);
    settleRotationStep += FALL_SPIN_ACCELERATION;
  }
  Actor_WaitForMove(obstacle);
  Event_Wait(2);
  Sound_Play(SE_LOG_FALL);
  sprite->rotation = 0;
  Actor_SetAnimation(ACTOR_FALLING_LOG, FALLING_LOG_ANIMATION_FALLEN);
  obstacle->x.fixed = toFIXED(336);
  obstacle->y.fixed = -toFIXED(8);
  obstacle->z.fixed = toFIXED(264);
  obstacle->velocityY = 0;
  obstacle->velocityX = 0;
  MapGrid_CopyCellAttributes(19, 16, 1, 1, 20, 16);
  MapGrid_CopyCellAttributes(20, 80, 1, 1, 21, 80);
}

/// Frost on the western of the Log-Rolling Stage's two puddles, cell (44, 17),
/// freezes it into a pillar of ice, a square Isaac can hop onto
/// (FLAG_WEST_PUDDLE_FROZEN). The walkthrough names this one the useful one.
static void ColosseumSatrageMatch_FreezeWestPuddle(void) {
  GameFlag_Set(FLAG_WEST_PUDDLE_FROZEN);
  Actor_Get(ACTOR_PUDDLE_WEST)->motionFlags = 0;
  MapGrid_CopyCellAttributes(46, 17, 1, 1, 44, 17);
}

/// Frost on the eastern of the Log-Rolling Stage's two puddles, cell (50, 17),
/// freezes it into a pillar of ice, a square Isaac can hop onto
/// (FLAG_EAST_PUDDLE_FROZEN).
static void ColosseumSatrageMatch_FreezeEastPuddle(void) {
  GameFlag_Set(FLAG_EAST_PUDDLE_FROZEN);
  Actor_Get(ACTOR_PUDDLE_EAST)->motionFlags = 0;
  MapGrid_CopyCellAttributes(46, 17, 1, 1, 50, 17);
}

/// Growth on the sprout at the foot of The Wall, cell (32, 16), grows it into a
/// vine up the wall that can be climbed (FLAG_PLANT_GROWN).
static void ColosseumSatrageMatch_GrowPlant(void) {
  GameFlag_Set(FLAG_PLANT_GROWN);
  MapGrid_CopyCellAttributes(32, 37, 1, 4, 32, 77);
}

/// Walking south off the top of the stone post just west of the sprout at the
/// foot of The Wall, cell (31, 16), Isaac jumps down to the ground below.
static void ColosseumSatrageMatch_JumpDownFromPost(void) {
  FieldActor_JumpDownLedge(gMainRoutineWork.leaderActorId, 6, 0);
}

/// Walking west off the top of the stone post just west of the sprout at the
/// foot of The Wall, cell (31, 16), Isaac hops across to the next post west.
static void ColosseumSatrageMatch_HopWestFromPost(void) {
  Field_TryJumpForward();
}

/// Rolls one of the Log-Rolling Stage's logs across the water by the offset,
/// with Isaac on top walking backwards as it turns under him, and stops both
/// in the middle of their cells.
static void ColosseumSatrageMatch_MoveRidingLog(s32 logId, s32 offsetX,
                                                s32 offsetZ) {
  struct Actor *rider;
  struct Actor *log;
  FIXED destinationX;
  FIXED destinationZ;

  rider = Actor_Get(gMainRoutineWork.leaderActorId);
  log = Actor_Get(logId);
  Event_Start();
  {
    destinationX =
        ((rider->x.fixed + COLOSSEUM_SATRAGE_MATCH_OFFSET_FIXED(offsetX)) &
         FIELD_CELL_MASK) +
        toFIXED(8);
    destinationZ =
        ((rider->z.fixed + COLOSSEUM_SATRAGE_MATCH_OFFSET_FIXED(offsetZ)) &
         FIELD_CELL_MASK) +
        toFIXED(8);

    rider->speedLimit = toFIXED(1);
    rider->acceleration = toFIXED(1) / 2;
    FieldActor_SetMoveTargetFX(rider, destinationX, rider->y.fixed,
                               destinationZ);
  }
  Actor_SelectAnimation(rider, ANIM_LEADER_WALK_BACKWARDS);
  {
    destinationX =
        ((log->x.fixed + COLOSSEUM_SATRAGE_MATCH_OFFSET_FIXED(offsetX)) &
         FIELD_CELL_MASK) +
        toFIXED(8);
    destinationZ =
        ((log->z.fixed + COLOSSEUM_SATRAGE_MATCH_OFFSET_FIXED(offsetZ)) &
         FIELD_CELL_MASK) +
        toFIXED(8);

    log->speedLimit = toFIXED(1);
    log->acceleration = toFIXED(1) / 2;
    FieldActor_SetMoveTargetFX(log, destinationX, log->y.fixed, destinationZ);
  }
  if (offsetX < 0 || offsetZ < 0) {
    Actor_SelectAnimation(log, FLOATING_LOG_ANIMATION_ROLL_WEST_OR_NORTH);
  } else {
    Actor_SelectAnimation(log, FLOATING_LOG_ANIMATION_ROLL_EAST_OR_SOUTH);
  }
  Sound_Play(SE_LOG_PUSH);
  Actor_WaitForMove(rider);
  Sound_Play(SE_STOP_ACTION_SE);
  Actor_SelectAnimation(log, FLOATING_LOG_ANIMATION_SETTLE);
  Event_End();
}

/// Standing on one of the Log-Rolling Stage's two logs and holding Left or
/// Right rolls it four cells across the pool with Isaac on top, towards the
/// far bank: west from its east end, east from its west end.
static void ColosseumSatrageMatch_RideLog(void) {
  struct Actor *rider = Actor_Get(gMainRoutineWork.leaderActorId);
  s32 column = FIELD_CELL(rider->x.fixed);
  s32 offsetX = 0;
  s32 logId = ACTOR_RIDING_LOG_NORTH;

  if (FIELD_CELL(rider->z.fixed) >= RIDING_LOG_SOUTH_ROW) {
    logId = ACTOR_RIDING_LOG_SOUTH;
  }
  if (FIELD_CELL(Actor_Get(logId)->x.fixed) != column) {
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
    ColosseumSatrageMatch_MoveRidingLog(logId, offsetX, 0);
    MapGrid_CopyCellAttributes(120, 10, 5, 6, 48, 10);
    column = FIELD_CELL(Actor_Get(ACTOR_RIDING_LOG_NORTH)->x.fixed);
    MapGrid_CopyCellAttributes(52, 28, 1, 3, column, RIDING_LOG_NORTH_ROW);
    column = FIELD_CELL(Actor_Get(ACTOR_RIDING_LOG_SOUTH)->x.fixed);
    MapGrid_CopyCellAttributes(52, 28, 1, 3, column, RIDING_LOG_SOUTH_ROW);
  }
}

/// Isaac leans on one of the Board Walk's logs and it rolls to a place given
/// in half cells, with Isaac walking after it half the way.
static void ColosseumSatrageMatch_RollLogToCell(s32 logId, s32 halfCellX,
                                                s32 halfCellZ) {
  s32 pusher = gMainRoutineWork.leaderActorId;
  struct Actor *pusherActor = Actor_Get(pusher);
  struct Actor *log = Actor_Get(logId);
  s32 heading = FACING_NEAREST_EIGHTH(pusherActor->facing);
  s32 alongX = FIELD_CELL(log->x.fixed) != halfCellX / 2;
  FIXED offsetX;
  FIXED offsetZ;

  halfCellX = FIELD_HALF_CELL_ORIGIN(halfCellX);
  halfCellZ = FIELD_HALF_CELL_ORIGIN(halfCellZ);
  if (alongX) {
    offsetX = (halfCellX - log->x.fixed) / 2;
    offsetZ = 0;
  } else {
    offsetX = 0;
    offsetZ = (halfCellZ - log->z.fixed) / 2;
  }
  Event_Start();
  Actor_SetAnimation(pusher, ANIM_PUSH);
  Event_Wait(6);
  log->speedLimit = toFIXED(1) / 2;
  log->acceleration = toFIXED(1) / 5;
  Actor_SelectAnimation(
      log, gColosseumSatrageMatch_RollLogScript[heading / (DEGtoANG(90))]);
  FieldActor_SetMoveTargetFX(log, halfCellX, 0, halfCellZ);
  Event_Wait(6);
  Actor_SetAnimation(pusher, ANIM_WALK);
  // Follow halfway behind the rolling log while its finish effect runs.
  Actor_StartCamera(((struct FieldWork *)HeapSlot_AllocEwram(
                         HEAP_SLOT_FIELD, sizeof(struct FieldWork)))
                        ->camera,
                    (s32)log);
  Actor_SetSpeedAndAcceleration(pusher, 0.5, 0.2);
  Actor_SelectAnimation(pusherActor, ANIM_WALK);
  FieldActor_SetMoveTargetFX(pusherActor, pusherActor->x.fixed + offsetX, 0,
                             pusherActor->z.fixed + offsetZ);
  Sound_Play(SE_HEAVY_MOVE);
  Actor_WaitForMove(pusherActor);
  Actor_SelectAnimation(pusherActor, ANIM_STAND);
  Actor_WaitForMove(log);
  Sound_Play(SE_STOP_ACTION_SE);
  Sound_Play(SE_HEAVY_STOP);
  Actor_SelectAnimation(log, 1);
  Event_Wait(15);
  Event_End();
}

/// Walls off the cells under the Board Walk's logs where they now lie, after
/// putting back the stage's cells as the map had them.
static void ColosseumSatrageMatch_UpdateRollingLogCollision(void) {
  s32 cell;

  MapGrid_CopyCellAttributes(100, 11, 12, 4, 14, 11);

  cell = FIELD_CELL(Actor_Get(ACTOR_ROLLING_LOG_WEST)->x.fixed);
  MapGrid_CopyCellAttributes(13, 28, 1, 4, cell, 11);

  cell = FIELD_CELL(Actor_Get(ACTOR_ROLLING_LOG_EAST)->x.fixed);
  MapGrid_CopyCellAttributes(13, 28, 1, 4, cell, 11);

  cell = FIELD_CELL(Actor_Get(ACTOR_ROLLING_LOG_CENTER)->z.fixed);
  MapGrid_CopyCellAttributes(13, 28, 4, 1, 18, cell);
}

/// Walking west into the Board Walk's western log, from beside it at cell
/// (17, 11) or (17, 14), rolls it two cells west.
static void ColosseumSatrageMatch_RollWestLogWest(void) {
  ColosseumSatrageMatch_RollLogToCell(ACTOR_ROLLING_LOG_WEST, 29, 26);
  ColosseumSatrageMatch_UpdateRollingLogCollision();
}

/// Walking east into the Board Walk's western log from column 13, once it has
/// been rolled west, rolls it back two cells east.
static void ColosseumSatrageMatch_RollWestLogEast(void) {
  ColosseumSatrageMatch_RollLogToCell(ACTOR_ROLLING_LOG_WEST, 33, 26);
  ColosseumSatrageMatch_UpdateRollingLogCollision();
}

/// Walking west into the Board Walk's eastern log, from beside it in column
/// 25, rolls it two cells west, as the attendant shows on the tour.
static void ColosseumSatrageMatch_RollEastLogWest(void) {
  ColosseumSatrageMatch_RollLogToCell(ACTOR_ROLLING_LOG_EAST, 45, 26);
  ColosseumSatrageMatch_UpdateRollingLogCollision();
}

/// Walking east into the Board Walk's eastern log from column 21, once it has
/// been rolled west, rolls it back two cells east.
static void ColosseumSatrageMatch_RollEastLogEast(void) {
  ColosseumSatrageMatch_RollLogToCell(ACTOR_ROLLING_LOG_EAST, 49, 26);
  ColosseumSatrageMatch_UpdateRollingLogCollision();
}

/// Unused: no trigger row calls it.
///
/// It would have rolled ACTOR_ROLLING_LOG_CENTER, placed at (0, 0) and never
/// moved to the Board Walk, to a target 16 pixels north of its South twin's.
/// The western and eastern logs roll west and east the same way
/// (ColosseumSatrageMatch_RollWestLogWest and the rest).
static UNUSED void ColosseumSatrageMatch_UnusedRollCenterLogNorth(void) {
  ColosseumSatrageMatch_RollLogToCell(ACTOR_ROLLING_LOG_CENTER, 40, 23);
  ColosseumSatrageMatch_UpdateRollingLogCollision();
}

/// Unused: as ColosseumSatrageMatch_UnusedRollCenterLogNorth, rolling
/// ACTOR_ROLLING_LOG_CENTER, which is not on the Board Walk, to the target 16
/// pixels south of that one's.
static UNUSED void ColosseumSatrageMatch_UnusedRollCenterLogSouth(void) {
  ColosseumSatrageMatch_RollLogToCell(ACTOR_ROLLING_LOG_CENTER, 40, 25);
  ColosseumSatrageMatch_UpdateRollingLogCollision();
}

/// Each frame while the shifting floor runs, carries Isaac north or south with
/// the lift he stands on, swaps the lifts between a line across the channel
/// and their broken pattern every COLOSSEUM_SATRAGE_MATCH_LIFT_CYCLE_FRAMES,
/// and makes their cells squares to hop onto only while they stand in line.
static void ColosseumSatrageMatch_UpdateLifts(void) {
  s32 liftId;
  struct Actor *rider;
  struct Actor *lift;
  FIXED liftX;
  FIXED riderX;
  FIXED deltaX;
  FIXED liftZ;
  FIXED riderZ;
  FIXED deltaZ;

  rider = Actor_Get(gMainRoutineWork.leaderActorId);
  // Carry a rider only while both horizontal distances are less than ten
  // pixels.
  for (liftId = ACTOR_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT;
       liftId <= ACTOR_COLOSSEUM_SATRAGE_MATCH_LAST_LIFT; liftId++) {
    lift = Actor_Get(liftId);
    lift->paused = 0;
    liftX = lift->x.fixed;
    riderX = rider->x.fixed;
    deltaX = liftX - riderX;
    if (deltaX >= 0) {
      if (deltaX >= toFIXED(10)) {
        continue;
      }
    } else {
      riderX -= liftX;
      if (riderX >= toFIXED(10)) {
        continue;
      }
    }
    liftZ = lift->z.fixed;
    riderZ = rider->z.fixed;
    deltaZ = liftZ - riderZ;
    if (deltaZ >= 0) {
      if (deltaZ >= toFIXED(10)) {
        continue;
      }
    } else {
      riderZ -= liftZ;
      if (riderZ >= toFIXED(10)) {
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
  if (gColosseumSatrageMatch_LiftTimer != 0 &&
      lift->targetX == ACTOR_NO_TARGET) {
    if (gColosseumSatrageMatch_LiftsApart == 0) {
      MapGrid_CopyCellAttributes(58, 28, 7, 1, 58, 13);
    } else {
      MapGrid_CopyCellAttributes(58, 10, 1, 1, 58, 11);
    }
  } else {
    MapGrid_CopyCellAttributes(57, 11, 1, 1, 58, 11);
    MapGrid_CopyCellAttributes(58, 14, 7, 1, 58, 13);
  }
  if (gColosseumSatrageMatch_LiftTimer == 0) {
    gColosseumSatrageMatch_LiftsApart ^= 1;
    if (gColosseumSatrageMatch_LiftsApart != 0) {
      FieldActor_SetMoveTarget(
          Actor_Get(ACTOR_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT), 936, 0, 184);
      FieldActor_SetMoveTarget(
          Actor_Get(ACTOR_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT + 1), 968, 0, 248);
      FieldActor_SetMoveTarget(
          Actor_Get(ACTOR_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT + 2), 1000, 0,
          184);
      FieldActor_SetMoveTarget(
          Actor_Get(ACTOR_COLOSSEUM_SATRAGE_MATCH_LAST_LIFT), 1032, 0, 248);
      Actor_SetAnimation(ACTOR_LIFT_OPERATOR, 11);
    } else {
      FieldActor_SetMoveTarget(
          Actor_Get(ACTOR_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT), 936, 0, 216);
      FieldActor_SetMoveTarget(
          Actor_Get(ACTOR_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT + 1), 968, 0, 216);
      FieldActor_SetMoveTarget(
          Actor_Get(ACTOR_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT + 2), 1000, 0,
          216);
      FieldActor_SetMoveTarget(
          Actor_Get(ACTOR_COLOSSEUM_SATRAGE_MATCH_LAST_LIFT), 1032, 0, 216);
      Actor_SetAnimation(ACTOR_LIFT_OPERATOR, 10);
    }
  }
  gColosseumSatrageMatch_LiftTimer++;
  if (gColosseumSatrageMatch_LiftTimer >=
      COLOSSEUM_SATRAGE_MATCH_LIFT_CYCLE_FRAMES) {
    if (GameFlag_Get(FLAG_FIELD_HANDLING_REQUESTS) == 0) {
      gColosseumSatrageMatch_LiftTimer = 0;
    }
  }
}

/// Stops the shifting floor and lines its four lifts up across the channel,
/// for the attendant's demonstration.
static void ColosseumSatrageMatch_ResetLifts(void) {
  gColosseumSatrageMatch_LiftTimer = 0;
  gColosseumSatrageMatch_LiftsApart = 0;
  Task_Remove(ColosseumSatrageMatch_UpdateLifts);
  Actor_SetPosition(ACTOR_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT, 936, 216);
  Actor_SetPosition(ACTOR_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT + 1, 968, 216);
  Actor_SetPosition(ACTOR_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT + 2, 1000, 216);
  Actor_SetPosition(ACTOR_COLOSSEUM_SATRAGE_MATCH_LAST_LIFT, 1032, 216);
  Actor_SetAnimation(ACTOR_LIFT_OPERATOR, 10);
}

/// Starts the shifting floor's lifts moving again.
static void ColosseumSatrageMatch_StartLifts(void) {
  Task_AddFrame(ColosseumSatrageMatch_UpdateLifts, TASK_PASS_FRAME + 5);
  GameFlag_Clear(FLAG_FIELD_HANDLING_REQUESTS);
}

/// Waits, for up to ten seconds, until the four lifts stand lined up across
/// the water, for the demonstration to jump onto them.
static void ColosseumSatrageMatch_WaitForLiftsLinedUp(void) {
  s32 elapsedFrames;

  wait(10);

  elapsedFrames = 0;
  while (gColosseumSatrageMatch_LiftsApart != 0 ||
         gColosseumSatrageMatch_LiftTimer !=
             COLOSSEUM_SATRAGE_MATCH_LIFT_CROSSING_FRAME) {
    wait(1);
    elapsedFrames++;
    if (elapsedFrames >= COLOSSEUM_SATRAGE_MATCH_DEMONSTRATION_TIMEOUT_FRAMES) {
      return;
    }
  }
}

/// Halt on the young man who works the shifting floor's lifts freezes him, and
/// the lifts stop in a line across the channel for Isaac to hop over
/// (FLAG_LIFTS_HALTED).
static void ColosseumSatrageMatch_HaltLifts(void) {
  FieldPsynergy_HaltActor(ACTOR_LIFT_OPERATOR);
  GameFlag_Set(FLAG_LIFTS_HALTED);

  if (gColosseumSatrageMatch_LiftsApart != 0) {
    gColosseumSatrageMatch_LiftTimer = 0;
  }

  wait(30);
  wait(1);

  Task_Remove(ColosseumSatrageMatch_UpdateLifts);

  MapGrid_CopyCellAttributes(58, 28, 7, 1, 58, 13);
  MapGrid_CopyCellAttributes(57, 11, 1, 1, 58, 11);
}

/// Reaching the arena at the west end, cells (8, 13) to (8, 18), Isaac takes
/// the Chain Mail if he is first and the Cotton Shirt if not, waits for
/// Satrage, and the two nod and fight.
static void ColosseumSatrageMatch_ReachArena(void) {
  s32 opponentFirst;
  s32 i;
  s32 opponentFirstSlot = 2;
  Colosso_NoOp();
  Event_Start();
  opponentFirst = Colosso_TakePrize(ITEM_CHAIN_MAIL, ITEM_COTTON_SHIRT);
  Colosso_WaitForOpponentToFinish();
  // Up to ten waits of 300 frames while the opponent holds up his prize.
  for (i = 0; i < 10; i++) {
    Actor_WaitForScriptEndById(ACTOR_OPPONENT);
  }
  Actor_SetSpeed(ACTOR_OPPONENT, 1);
  Actor_WalkTo(ACTOR_OPPONENT, 88, 256);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 1);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 120, 256);
  Actor_SetAnimation(ACTOR_OPPONENT, ANIM_STAND);
  Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_OPPONENT, 0);
  Event_Wait(10);
  Actor_SetAnimation(ACTOR_OPPONENT, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(20);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 2);
  Actor_SetSpeed(ACTOR_OPPONENT, 2);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 112, 256);
  Actor_WalkToAndWait(ACTOR_OPPONENT, 96, 256);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_STRIDE);
  Actor_SetAnimation(ACTOR_OPPONENT, WARRIOR_ANIMATION_FIGHTING_STANCE);
  Event_Wait(10);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_COLOSSO,
                                        opponentFirstSlot - opponentFirst + 1);
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  Event_SetAfterBattleDestination(SCENE_COLOSSEUM_SATRAGE_MATCH,
                                  COLOSSEUM_SATRAGE_MATCH_AFTER_MATCH_WON);
  Event_SetDefeatDestination(SCENE_COLOSSEUM_SATRAGE_MATCH,
                             COLOSSEUM_SATRAGE_MATCH_AFTER_MATCH_LOST);
  GameFlag_Set(FLAG_KEEP_MUSIC_ONCE);
}

/// Returns Colosseum Final 2's stages, chests, Psynergy targets and
/// attendants.
const struct Trigger *Scene_GetTriggers(void) {
  return gColosseumSatrageMatchTriggers;
}

/// Back from the battle, Isaac and Satrage stand in the arena as the result is
/// shown.
static void ColosseumSatrageMatch_StageBattleResult(s32 result) {
  Actor_ReleaseById(ACTOR_PRIZE_A);
  Actor_ReleaseById(ACTOR_PRIZE_B);
  Party_RestoreHpPp(1);
  Event_Start();
  Actor_SetPosition(ACTOR_OPPONENT, 88, 256);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 120, 256);
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
  FieldCamera_MoveTo(104, 0, 192, 0);
  Colosso_ShowMatchResult(result);
  Event_End();
}

/// Sets up Colosseum Final 2 for the visit its entrance stands for.
s32 Scene_Initialize(void) {
  struct Actor *actor;
  s32 x;
  s32 obstacleId;
  s32 column;
  s32 liftId;
  s32 columnOffset;
  s32 row;

  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_FADE, 0);
  GameFlag_Set(FLAG_RETREAT_BLOCKED);
  MapGrid_CopyCellAttributes(14, 11, 12, 4, 100, 11);
  MapGrid_CopyCellAttributes(48, 10, 5, 6, 120, 10);
  for (obstacleId = ACTOR_BOARD_WALK_FIRST_PLATFORM;
       obstacleId <= ACTOR_FALLING_LOG; obstacleId++) {
    actor = Actor_Get(obstacleId);
    Actor_SelectAnimation(actor, 4);
    actor->motionFlags = 0;
    actor->y.fixed = 0;
    actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  }
  Actor_Get(ACTOR_FORCE_TARGET)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  if (GameFlag_Get(FLAG_LOG_KNOCKED_DOWN)) {
    actor = Actor_Get(ACTOR_FALLING_LOG);
    actor->x.fixed = toFIXED(336);
    actor->y.fixed = -toFIXED(8);
    actor->z.fixed = toFIXED(264);
    MapGrid_CopyCellAttributes(19, 16, 1, 1, 20, 16);
    MapGrid_CopyCellAttributes(20, 80, 1, 1, 21, 80);
  } else {
    actor = Actor_Get(ACTOR_FALLING_LOG);
    Actor_SelectAnimation(actor, FALLING_LOG_ANIMATION_STANDING);
    actor->y.fixed = toFIXED(16);
  }
  Actor_Get(ACTOR_WALL_PILLAR)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  // With the wall pillar moved, cell (35, 77) takes the open ground of the
  // cell south of it; the map stores it as a wall.
  if (GameFlag_Get(FLAG_WALL_PILLAR_MOVED)) {
    MapGrid_CopyCellAttributes(35, 78, 1, 1, 35, 77);
  }
  if (GameFlag_Get(FLAG_PLANT_GROWN)) {
    Actor_SetAnimation(ACTOR_GROWTH_PLANT, ANIM_SHAKE_HEAD);
    MapGrid_CopyCellAttributes(32, 37, 1, 4, 32, 77);
  }
  if (GameFlag_Get(FLAG_WEST_PUDDLE_FROZEN)) {
    Actor_Get(ACTOR_PUDDLE_WEST)->motionFlags = 0;
    Actor_Get(ACTOR_PUDDLE_WEST)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    Actor_SetAnimation(ACTOR_PUDDLE_WEST, PUDDLE_ANIMATION_PILLAR);
    MapGrid_CopyCellAttributes(46, 17, 1, 1, 44, 17);
  }
  if (GameFlag_Get(FLAG_EAST_PUDDLE_FROZEN)) {
    Actor_Get(ACTOR_PUDDLE_EAST)->motionFlags = 0;
    Actor_Get(ACTOR_PUDDLE_EAST)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    Actor_SetAnimation(ACTOR_PUDDLE_EAST, PUDDLE_ANIMATION_PILLAR);
    MapGrid_CopyCellAttributes(46, 17, 1, 1, 50, 17);
  }
  actor = Actor_Get(ACTOR_RIDING_LOG_NORTH);
  column = FIELD_CELL(actor->x.fixed);
  actor->motionFlags = 0;
  actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  MapGrid_CopyCellAttributes(52, 28, 1, 3, column, 10);
  actor = Actor_Get(ACTOR_RIDING_LOG_SOUTH);
  column = FIELD_CELL(actor->x.fixed);
  actor->motionFlags = 0;
  actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  MapGrid_CopyCellAttributes(52, 28, 1, 3, column, 13);

  // Each Mini-Maze pillar returns to the column its area temp flag byte
  // keeps.
  x = FieldGameFlag_GetByte(FLAG_MINI_MAZE_FIRST_PILLAR_COLUMN);
  if (x == 0) {
    x = 73;
  }
  actor = Actor_Get(ACTOR_MINI_MAZE_FIRST_PILLAR);
  actor->x.fixed = FIELD_CELL_CENTER(x);
  actor->motionFlags = 0;
  actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  MapGrid_CopyCellAttributes(71, 16, 1, 1, x, 16);
  x = FieldGameFlag_GetByte(FLAG_MINI_MAZE_SECOND_PILLAR_COLUMN);
  if (x == 0) {
    x = 76;
  }
  actor = Actor_Get(ACTOR_MINI_MAZE_FIRST_PILLAR + 1);
  actor->x.fixed = FIELD_CELL_CENTER(x);
  actor->motionFlags = 0;
  actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  MapGrid_CopyCellAttributes(71, 16, 1, 1, x, 16);
  x = FieldGameFlag_GetByte(FLAG_MINI_MAZE_THIRD_PILLAR_COLUMN);
  if (x == 0) {
    x = 79;
  }
  actor = Actor_Get(ACTOR_MINI_MAZE_FIRST_PILLAR + 2);
  actor->x.fixed = FIELD_CELL_CENTER(x);
  actor->motionFlags = 0;
  actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  MapGrid_CopyCellAttributes(71, 16, 1, 1, x, 16);

  ColosseumSatrageMatch_UpdateRollingLogCollision();
  // With FLAG_LIFTS_HALTED set the lifts stay where they are; otherwise their
  // task resumes.
  Actor_SetAnimation(ACTOR_LIFT_OPERATOR, 10);
  if (GameFlag_Get(FLAG_LIFTS_HALTED)) {
    for (liftId = ACTOR_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT, row = 13,
        columnOffset = 58;
         liftId <= ACTOR_COLOSSEUM_SATRAGE_MATCH_LAST_LIFT;
         liftId++, columnOffset += 2) {
      actor = Actor_Get(liftId);
      actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      Actor_SelectAnimation(actor, 4);
      MapGrid_CopyCellAttributes(56, 13, 1, 1, columnOffset, row);
    }
    Actor_SetAnimation(ACTOR_LIFT_OPERATOR, 10);
    FieldPsynergy_HaltActor(ACTOR_LIFT_OPERATOR);
  } else {
    for (liftId = ACTOR_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT;
         liftId <= ACTOR_COLOSSEUM_SATRAGE_MATCH_LAST_LIFT; liftId++) {
      actor = Actor_Get(liftId);
      actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      Actor_SelectAnimation(actor, 4);
      actor->speedLimit = toFIXED(1) / 2;
      actor->acceleration = toFIXED(1) / 5;
    }
    Task_AddFrame(ColosseumSatrageMatch_UpdateLifts, TASK_PASS_FRAME + 5);
    Task_SetFlags(ColosseumSatrageMatch_UpdateLifts, 1);
  }
  Actor_SetAnimation(ACTOR_OPPONENT, WARRIOR_ANIMATION_FIGHTING_STANCE);
  gMainRoutineWork.movementMode = LEADER_MOVEMENT_WALK;
  Colosso_ShowItemIcon(ACTOR_PRIZE_B, ITEM_COTTON_SHIRT);
  Colosso_ShowItemIcon(ACTOR_PRIZE_A, ITEM_CHAIN_MAIL);
  Actor_SetSpriteColorById(ACTOR_OPPONENT, SPRITE_COLOR_GRB);
  // The race, the friends' turns, the tour, and the battle's result.
  switch (gMainRoutineWork.entrance) {
  case COLOSSEUM_SATRAGE_MATCH_RACE_FROM_LEND_A_HAND:
    ColosseumSatrageMatch_StartRaceGauge(ACTOR_PARTY_LEADER, ACTOR_OPPONENT, 5,
                                         toFIXED(104), toFIXED(256),
                                         ACTOR_PRIZE_A, ACTOR_PRIZE_B);
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
      Colosso_ShowOpeningLettering(2);
    }
    Actor_WatchAndSetScript(ACTOR_GARET, ACTOR_PARTY_LEADER);
    Actor_WatchAndSetScript(ACTOR_IVAN, ACTOR_PARTY_LEADER);
    Actor_WatchAndSetScript(ACTOR_MIA, ACTOR_PARTY_LEADER);
    ColosseumSatrageMatch_StartOpponentPath(RESOURCE_OPPONENT_PATH_B);
    break;
  case COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_EAST:
    Task_AddFrame(Colosso_RaiseTakePlaceOnPsynergy, TASK_PASS_FRAME);
    Actor_ReleaseById(ACTOR_PRIZE_A);
    Actor_ReleaseById(ACTOR_PRIZE_B);
    if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
      Colosso_PlaceFriendsAtCheeringSpots();
      Colosso_ControlCharacterAlone(GARET);
      Colosso_ShowOpeningLettering(0);
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

/// The guide gives the party the tour of Colosseum Final 2
/// (COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_WEST).
static void ColosseumSatrageMatch_IntroduceSecondFinals(s32 attendantId) {
  s32 attendantZ;
  s32 attendantX;
  struct Actor *actor;

  actor = Actor_Get(attendantId);
  attendantX = actor->x.part[0].integer;
  attendantZ = actor->z.part[0].integer;
  Event_Start();
  Actor_SetSpeed(attendantId, 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 1);
  Actor_SetSpeed(ACTOR_GARET, 1);
  Actor_SetSpeed(ACTOR_IVAN, 1);
  Actor_SetSpeed(ACTOR_MIA, 1);
  Actor_SetPositionFX(ACTOR_PARTY_LEADER, toFIXED(attendantX),
                      (toFIXED(attendantZ) - toFIXED(48)));
  Actor_SetPositionFX(ACTOR_GARET, toFIXED(attendantX) - toFIXED(16),
                      toFIXED(attendantZ) - toFIXED(40));
  Actor_SetPositionFX(ACTOR_IVAN, toFIXED(attendantX) + toFIXED(16),
                      toFIXED(attendantZ) - toFIXED(40));
  Actor_SetPositionFX(ACTOR_MIA, toFIXED(attendantX),
                      (toFIXED(attendantZ) - toFIXED(32)));
  Actor_SetPositionFX(attendantId, toFIXED(attendantX),
                      (toFIXED(attendantZ) - toFIXED(80)));
  Actor_Get(ACTOR_PARTY_LEADER)->facing = FACING_NORTH;
  FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_SetMessageId(MsgColossoSiteSecondFinals);
  Actor_SetAnimationAndWait(attendantId, ANIM_NOD);
  // "This is the site of the second finals match."
  Event_ShowMessage(attendantId, 0);
  Actor_RunRepeatedMotion(attendantId, 2);
  // "As I explained earlier, each stage has its own attendant."
  Event_ShowMessage(attendantId, 0);
  Actor_RunRepeatedMotion(attendantId, 2);
  // "Please ask the attendants directly for descriptions of each..."
  Event_ShowMessage(attendantId, 0);
  Actor_RunRepeatedMotion(attendantId, 2);
  // "...Isaac, that first match was terrific. I'll be right here,..."
  Event_ShowMessage(attendantId, 0);
  Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_NOD);
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
  Actor_WalkToAndWait(attendantId, attendantX - 16, attendantZ - 64);
  Actor_SetPosition(ACTOR_GARET, 0, 0);
  Actor_SetPosition(ACTOR_IVAN, 0, 0);
  Actor_SetPosition(ACTOR_MIA, 0, 0);
  Actor_WalkToAndWait(attendantId, attendantX - 16, attendantZ - 16);
  Actor_WalkToAndWait(attendantId, attendantX, attendantZ);
  Actor_FaceDirection(attendantId, FACING_NORTH, 10);
  Event_End();
}

/// The Mini-Maze's attendant explains stage 1 of Colosseum Final 2.
static void ColosseumSatrageMatch_TalkToMiniMazeAttendant(s32 attendant) {
  s32 result;

  if (gMainRoutineWork.entrance ==
      COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_EAST) {
    Colosso_AskDoneCheering(attendant);
  } else {
    Event_Start();
    result = Colosso_GreetAtStage(attendant, 1);
    if (result == COLOSSO_GUIDE_DESCRIBE) {
      Event_SetMessageId(MsgColossoStageDubbedMini);
      FieldCamera_SetSpeed(3);
      FieldCamera_MoveTo(1224, CAMERA_KEEP, 184, 1);
      FieldCamera_WaitForMove();
      // "This stage is dubbed the Mini-Maze."
      Event_ShowMessage(attendant, 0);
      Colosso_FadeInWarrior(ACTOR_PARTY_LEADER, 1272, 168);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, 1.5);
      Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 1288, 184);
      Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 1288, 216);
      Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 1224, 216);
      Event_ShowMessage(attendant, 0);
      Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 1224, 248);
      Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 1192, 248);
      Event_Wait(3);
      Actor_Get(ACTOR_PARTY_LEADER)->velocityY = toFIXED(4);
      Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_ARMS_RAISED);
      Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      Event_Wait(30);
      Event_ShowMessage(attendant, 0);
      Colosso_RestoreWarrior(ACTOR_PARTY_LEADER);
      FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
      Colosso_RepeatStageNumber(attendant, 1);
    } else if (result == COLOSSO_GUIDE_OFFER_CHEERING) {
      Event_SetMessageId(MsgColossoMainThingStage);
      // "The main thing with this stage is to proceed through the..."
      Event_ShowMessage(attendant, 0);
    }
    Colosso_OfferCheeringSpot(result, attendant, 1);
    Event_End();
  }
}

/// The shifting floor stage's attendant explains stage 2 of Colosseum Final 2.
static void ColosseumSatrageMatch_TalkToShiftingFloorAttendant(s32 attendant) {
  s32 result;

  if (gMainRoutineWork.entrance ==
      COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_EAST) {
    Colosso_AskDoneCheering(attendant);
  } else {
    Event_Start();
    result = Colosso_GreetAtStage(attendant, 2);
    if (result == COLOSSO_GUIDE_DESCRIBE) {
      Event_SetMessageId(MsgColossoShiftingFloorStage);
      ColosseumSatrageMatch_ResetLifts();
      FieldCamera_SetSpeed(3);
      FieldCamera_MoveTo(984, CAMERA_KEEP, 232, 1);
      FieldCamera_WaitForMove();
      COLOSSEUM_SATRAGE_MATCH_SHOW_MESSAGE(attendant, 0);
      ColosseumSatrageMatch_StartLifts();
      Event_ShowMessage(attendant, 0);
      Colosso_FadeInWarrior(ACTOR_PARTY_LEADER, 1080, 264);
      Event_Wait(15);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, 1.5);
      Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 1080, 216);
      Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 1064, 216);
      ColosseumSatrageMatch_WaitForLiftsLinedUp();
      Field_TryJumpForward();
      FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
      Event_ShowMessage(attendant, 0);
      Colosso_RestoreWarrior(ACTOR_PARTY_LEADER);
      FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
      Colosso_RepeatStageNumber(attendant, 2);
    } else {
      if (result == COLOSSO_GUIDE_OFFER_CHEERING) {
        Event_SetMessageId(MsgColossoOperatorLiftsWillCheerFor);
        // "The operator of the lifts will cheer for you here."
        Event_ShowMessage(attendant, 0);
      }
    }
    Colosso_OfferCheeringSpot(result, attendant, 2);
    Event_End();
  }
}

/// The Log-Rolling Stage's attendant explains stage 3 of Colosseum Final 2.
static void ColosseumSatrageMatch_TalkToLogRollingAttendant(s32 attendant) {
  s32 result;

  if (gMainRoutineWork.entrance ==
      COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_EAST) {
    Colosso_AskDoneCheering(attendant);
  } else {
    Event_Start();
    result = Colosso_GreetAtStage(attendant, 3);
    if (result == COLOSSO_GUIDE_DESCRIBE) {
      Event_SetMessageId(MsgColossoLogRollingStage);
      FieldCamera_SetSpeed(3);
      FieldCamera_MoveTo(752, CAMERA_KEEP, 192, 1);
      FieldCamera_WaitForMove();
      Event_Wait(60);
      FieldCamera_SetSpeed(1);
      FieldCamera_MoveTo(752, CAMERA_KEEP, 224, 1);
      FieldCamera_WaitForMove();
      // "This is the Log- Rolling Stage."
      Event_ShowMessage(attendant, 0);
      Colosso_FadeInWarrior(ACTOR_PARTY_LEADER, 856, 264);
      Event_Wait(10);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, 1.5);
      Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 856, 264);
      Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 856, 232);
      Event_ShowMessage(attendant, 0);
      Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 840, 232);
      Event_Wait(10);
      COLOSSEUM_SATRAGE_MATCH_MOVE_RIDING_LOG(
          ACTOR_RIDING_LOG_SOUTH, -COLOSSEUM_SATRAGE_MATCH_LOG_TRAVEL_PIXELS,
          0);
      FieldCamera_MoveTo(752, CAMERA_KEEP, 216, 1);
      Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
      Event_Wait(10);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, 1);
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 760, 232);
      Event_Wait(10);
      Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 30);
      Event_ShowMessage(attendant, 0);
      Colosso_RestoreWarrior(ACTOR_PARTY_LEADER);
      FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
      Actor_SetPosition(ACTOR_RIDING_LOG_SOUTH, 840, 232);
      Colosso_RepeatStageNumber(attendant, 3);
    } else if (result == COLOSSO_GUIDE_OFFER_CHEERING) {
      Event_SetMessageId(MsgColossoHereYourObjectiveRideLogs);
      // "Here, your objective is to ride the logs as quickly as possible."
      Event_ShowMessage(attendant, 0);
    }
    Colosso_OfferCheeringSpot(result, attendant, 3);
    Event_End();
  }
}

/// The Wall's attendant explains stage 4 of Colosseum Final 2.
static void ColosseumSatrageMatch_TalkToWallAttendant(s32 attendant) {
  struct Actor *actor;
  s32 result;

  if (gMainRoutineWork.entrance ==
      COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_EAST) {
    Colosso_AskDoneCheering(attendant);
    return;
  }
  Event_Start();
  result = Colosso_GreetAtStage(attendant, 4);
  if (result == COLOSSO_GUIDE_DESCRIBE) {
    Event_SetMessageId(MsgColossoFansJustCallWall);
    FieldCamera_SetSpeed(3);
    FieldCamera_MoveTo(536, CAMERA_KEEP, 240, 1);
    FieldCamera_WaitForMove();
    Event_Wait(45);
    FieldCamera_SetSpeed(1);
    FieldCamera_MoveTo(536, CAMERA_KEEP, 192, 1);
    {
      FieldCamera_WaitForMove();
      // "The fans just call this The Wall."
      Event_ShowMessage(attendant, 0);
      Colosso_FadeInWarrior(ACTOR_PARTY_LEADER, 632, 264);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, 1);
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 616, 264);
      Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
      Field_ClimbUp();
      FieldCamera_SetSpeed(0.25);
      FieldCamera_MoveTo(536, CAMERA_KEEP, 160, 1);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.5);
      Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_CLIMB);
      actor = Actor_Get(ACTOR_PARTY_LEADER);
      FieldActor_SetMoveTargetFX(actor, actor->x.fixed,
                                 actor->y.fixed + toFIXED(64), actor->z.fixed);
      Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
      Field_ClimbUp();
      FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
      Event_ShowMessage(attendant, 0);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, 1.5);
      Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 488, 248);
      Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 20);
      FieldActor_JumpDownLedge(ACTOR_PARTY_LEADER, 6, 0);
      FieldCamera_MoveTo(536, CAMERA_KEEP, 160, 1);
      Event_ShowMessage(attendant, 0);
      Colosso_RestoreWarrior(ACTOR_PARTY_LEADER);
      FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
      Colosso_RepeatStageNumber(attendant, 4);
    }
  } else if (result == COLOSSO_GUIDE_OFFER_CHEERING) {
    Event_SetMessageId(MsgColossoScalingWallQuickly);
    // "Scaling the wall quickly is priority one in this stage."
    Event_ShowMessage(attendant, 0);
  }
  Colosso_OfferCheeringSpot(result, attendant, 4);
  Event_End();
}

/// Moves a demonstration obstacle to pixel coordinates while its pusher
/// follows.
static void ColosseumSatrageMatch_PushObstacleToCell(s32 obstacleId, s32 column,
                                                     s32 row) {
  s32 pusher = gMainRoutineWork.leaderActorId;
  struct Actor *pusherActor = Actor_Get(pusher);
  struct Actor *obstacle = Actor_Get(obstacleId);
  s32 alongX = FIELD_CELL(obstacle->x.fixed) != column / 2;
  FIXED offsetX;
  FIXED offsetZ;

  column <<= 16;
  row <<= 16;
  if (alongX) {
    offsetX = (column - obstacle->x.fixed) / 2;
    offsetZ = 0;
  } else {
    offsetX = 0;
    offsetZ = (row - obstacle->z.fixed) / 2;
  }
  Actor_SetAnimation(pusher, ANIM_PUSH);
  Event_Wait(6);
  obstacle->speedLimit = toFIXED(1) / 2;
  obstacle->acceleration = toFIXED(1) / 5;
  Sound_Play(SE_HEAVY_MOVE);
  Actor_SelectAnimation(obstacle, 3);
  FieldActor_SetMoveTargetFX(obstacle, column, 0, row);
  Event_Wait(6);
  Actor_SetAnimation(pusher, ANIM_WALK);
  Actor_SetSpeedAndAcceleration(pusher, 0.5, 0.2);
  Actor_SelectAnimation(pusherActor, ANIM_WALK);
  FieldActor_SetMoveTargetFX(pusherActor, pusherActor->x.fixed + offsetX, 0,
                             pusherActor->z.fixed + offsetZ);
  Actor_WaitForMove(pusherActor);
  Actor_SelectAnimation(pusherActor, ANIM_STAND);
  Actor_WaitForMove(obstacle);
  Actor_SelectAnimation(obstacle, 1);
  Sound_Play(SE_STOP_ACTION_SE);
  Sound_Play(SE_HEAVY_STOP);
  Event_Wait(15);
}

/// The Board Walk's attendant explains stage 5 of Colosseum Final 2.
static void ColosseumSatrageMatch_TalkToBoardWalkAttendant(s32 attendant) {
  s32 result;

  if (gMainRoutineWork.entrance ==
      COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_EAST) {
    Colosso_AskDoneCheering(attendant);
    return;
  }
  Event_Start();
  result = Colosso_GreetAtStage(attendant, 5);
  if (result == COLOSSO_GUIDE_DESCRIBE) {
    Event_SetMessageId(MsgColossoPlaceCalledBoard);
    FieldCamera_SetSpeed(2);
    FieldCamera_MoveTo(328, CAMERA_KEEP, 264, 1);
    FieldCamera_WaitForMove();
    Event_Wait(30);
    FieldCamera_SetSpeed(1.5);
    FieldCamera_MoveTo(312, CAMERA_KEEP, 176, 1);
    FieldCamera_WaitForMove();
    // "This place is called the Board Walk."
    Event_ShowMessage(attendant, 0);
    Colosso_FadeInWarrior(ACTOR_PARTY_LEADER, 408, 264);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 1.5);
    Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 408, 216);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 10);
    Event_ShowMessage(attendant, 0);
    ColosseumSatrageMatch_PushObstacleToCell(ACTOR_ROLLING_LOG_EAST, 360, 208);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_SMILE, 45);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 1.5);
    Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 376, 216);
    Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 376, 248);
    Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 312, 248);
    Event_ShowMessage(attendant, 0);
    Colosso_RestoreWarrior(ACTOR_PARTY_LEADER);
    FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
    Actor_SetPosition(ACTOR_ROLLING_LOG_EAST, 392, 208);
    Colosso_RepeatStageNumber(attendant, 5);
  } else if (result == COLOSSO_GUIDE_OFFER_CHEERING) {
    Event_SetMessageId(MsgColossoFocusRollingLogs);
    // "Your focus here will be rolling the logs to form a path."
    Event_ShowMessage(attendant, 0);
  }
  Colosso_OfferCheeringSpot(result, attendant, 5);
  Event_End();
}

#include "shared/colosso_solo.c"
#include "shared/item_equip.c"
#include "shared/colosso_task.c"

#include "shared/colosso_lettering.c"

#include "shared/colosso_marker.c"
#include "shared/colosso_item_icon.c"

#include "shared/colosso_hex_string.c"
#include "shared/colosso_finish.c"

#include "shared/colosso_path_opponent.c"

/// Colosseum Final 2's own copy of Colosso_StartRaceGauge
/// (shared/colosso_setup.c), which passes the gauge routine's address as it is.
void ColosseumSatrageMatch_StartRaceGauge(s32 leaderId, s32 opponentId,
                                          s32 segments, s32 finishX,
                                          FIXED finishZ, s32 firstItem,
                                          s32 secondItem) {
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

  leader = Actor_Get(leaderId);
  opponent = Actor_Get(opponentId);

  // Unless the party returns to where it stood, the opponent starts level
  // with the leader, as far on the other side of the finish.
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
    opponent->x.fixed = (finishX << 1) - leader->x.fixed;
    opponent->z.fixed = leader->z.fixed;
  }

  work->gauge.level = 0;
  work->gauge.hold = 0;

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

#include "shared/colosso_map_state.c"
#include "shared/actor_find_at_position.c"

#include "shared/colosso_occupant.c"

/// The chest holding a Smoke Bomb has been opened.
#define FLAG_COLOSSEUM_SATRAGE_MATCH_SMOKE_BOMB_CHEST_OPENED FLAG_AREA_TEMP_39

const u8 gColosseumSatrageMatch_RollLogScript[] = {2, 2, 3, 3};

#include "shared/colosso_lettering_tables.c"
// clang-format off

char gHexDigits[20] = "0123456789ABCDEF";

#include "shared/colosso_prize_scripts.c"

#include "shared/pushed_actor_direction_steps.c"

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
    ACTOR_PLACEMENT(ACTOR_PARTY_LEADER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        1272, 0, 168, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        264, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        376, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        568, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        1272, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        1224, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        1176, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7,
        264, 0, 208, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7,
        392, 0, 208, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FALLING_LOG, CONDITION_ALWAYS, 7,
        360, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        520, 0, 258, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        712, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        808, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, 7,
        936, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, 7,
        968, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, 7,
        1000, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, 7,
        1032, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, 7,
        248, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, 7,
        264, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, 7,
        296, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, 7,
        312, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FALLING_LOG, CONDITION_ALWAYS, 7,
        360, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        1072, 0, 280, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7,
        840, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7,
        840, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        1224, 0, 488, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        1208, 0, 376, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        984, 0, 376, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        760, 0, 376, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        536, 0, 376, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        312, 0, 376, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        104, 0, 240, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        104, 0, 272, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

s32 gColosseumSatrageMatch_LiftsApart = 0;

u32 gColosseumSatrageMatch_LiftTimer = 0;

struct Trigger gColosseumSatrageMatchTriggers[] = {
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_WALL_POST, CONDITION_ALWAYS, EVENT(ColosseumSatrageMatch_JumpDownFromPost) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_WALL_POST, CONDITION_ALWAYS, EVENT(ColosseumSatrageMatch_HopWestFromPost) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_EAST_LOG_EAST_SIDE, CONDITION_ALWAYS, EVENT(ColosseumSatrageMatch_RollEastLogWest) },
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_EAST_LOG_WEST_SIDE, CONDITION_ALWAYS, EVENT(ColosseumSatrageMatch_RollEastLogEast) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_WEST_LOG_EAST_SIDE, CONDITION_ALWAYS, EVENT(ColosseumSatrageMatch_RollWestLogWest) },
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_WEST_LOG_WEST_SIDE, CONDITION_ALWAYS, EVENT(ColosseumSatrageMatch_RollWestLogEast) },
    { STEP_TRIGGER_WALKING, CELL_BEHAVIOR_RIDING_LOG, CONDITION_ALWAYS, EVENT(ColosseumSatrageMatch_RideLog) },
    { STEP_TRIGGER_WALKING, CELL_BEHAVIOR_BOARD_WALK_LEDGE, CONDITION_ALWAYS, EVENT(ColosseumSatrageMatch_PushBoardWalkPillar) },
    { STEP_TRIGGER_WALKING, CELL_BEHAVIOR_WALL_LEDGE, CONDITION_ALWAYS, EVENT(ColosseumSatrageMatch_PushWallPillar) },
    { STEP_TRIGGER_WALKING, CELL_BEHAVIOR_WALL_LEDGE_UNUSED, CONDITION_ALWAYS, EVENT(ColosseumSatrageMatch_PushWallPillar) },
    { TRIGGER_STEP, CELL_BEHAVIOR_ARENA, FLAG_COLOSSEUM_ARENA_CLOSED, EVENT(ColosseumSatrageMatch_ReachArena) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 101, FLAG_AREA_TEMP_38, FIND_ITEM(ITEM_NUT) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST) | TRIGGER_FLAG_FACING | FACING_WEST, 102, FLAG_COLOSSEUM_SATRAGE_MATCH_SMOKE_BOMB_CHEST_OPENED, FIND_ITEM(ITEM_SMOKE_BOMB) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 104, FLAG_AREA_TEMP_3B, FIND_ITEM(ITEM_NUT) },
    { PSYNERGY_PHASE_ON_ACTOR(ABILITY_FROST, PSYNERGY_PHASE_ACTS), ACTOR_PUDDLE_WEST, FLAG_WEST_PUDDLE_FROZEN, EVENT(ColosseumSatrageMatch_FreezeWestPuddle) },
    { PSYNERGY_PHASE_ON_ACTOR(ABILITY_FROST, PSYNERGY_PHASE_ACTS), ACTOR_PUDDLE_EAST, FLAG_EAST_PUDDLE_FROZEN, EVENT(ColosseumSatrageMatch_FreezeEastPuddle) },
    { PSYNERGY_PHASE_ON_ACTOR(ABILITY_FORCE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FORCE_TARGET, FLAG_LOG_KNOCKED_DOWN, EVENT(ColosseumSatrageMatch_KeepLogStillForForce) },
    { PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_FORCE_TARGET, FLAG_LOG_KNOCKED_DOWN, EVENT(ColosseumSatrageMatch_KnockDownLog) },
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
    { TRIGGER_RAISED, COLOSSO_TAKE_PLACE_TRIGGER, CONDITION_ALWAYS,
      EVENT(Colosso_TakePlaceForMatch) },
    { TRIGGER_TABLE_END },
};

s16 gColosso_LetteringSlot = -1;

#include "shared/colosso_lettering_scripts.c"
// clang-format on

s16 gColosso_MarkerSlot = -1;

#include "shared/colosso_finish_scripts.c"

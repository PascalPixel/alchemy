/// @file
/// Colosseum Final 3 (SceneId_ColosseumNavampaMatch) is the site of the last
/// finals match, where Isaac races Navampa of Gondowan through six stages to
/// the arena and fights him there.
///
/// Isaac has won the first two matches, and Navampa is the sixth-ranked
/// warrior.
///
/// It works as Colosseum Final 1 does (colosseum_azart_match.c). The guide,
/// who did not think Isaac would make it this far, leaves the stages to their
/// attendants. The stages run through the Scales of Justice, the
/// Stepping-Stone Stage, the Wall Wedge, the Free-Climb Area, the Moving
/// Sidewalk Stage and another log-rolling area.
///
/// Scene_Initialize restores the stages from their temporary
/// flags (FLAG_PLANT_GROWN to FLAG_LONG_LOG_IN_PLACE, and the stone's column
/// from FLAG_STONE_COLUMN). Then it sets up the visit its entrance stands for.
///
/// - COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_NORTH, the tour
///   (ColosseumNavampaMatch_IntroduceThirdFinals), from the Colosseum's east
///   passage after the second win. The attendants describe their stages and
///   take the friends to cheer, and the guide sends Isaac back to the passage
///   (COLOSSEUM_NAVAMPA_MATCH_TO_COLOSSEUM).
/// - COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_SOUTH, `Lend a Hand`. Garet, Ivan
///   and Mia in turn may use Psynergy on the stages. Frost freezes the target
///   on the Scales of Justice's eastern ledge so the scales stay tipped. Halt
///   stops the Wall Wedge's operator and with him its pistons. Force knocks the
///   log off the Moving Sidewalk so the belt no longer carries anyone back.
///   Growth grows a plant into a shortcut. Move shifts the weight onto the
///   scales and the stones and the rock on the way. After Mia,
///   COLOSSEUM_NAVAMPA_MATCH_LEND_A_HAND_TO_RACE starts the race.
/// - COLOSSEUM_NAVAMPA_MATCH_RACE_FROM_LEND_A_HAND, the race against Navampa,
///   who follows a recorded path, after the lettering `The 3rd Match` and a
///   count of three. At the arena the first to arrive takes the Claymore and
///   the second the Hunter's Sword. The battle is ENCOUNTER_ZONE_COLOSSO's
///   fifth or sixth group, the Navampa with or without the Claymore.
///
/// Stepping onto the Scales of Justice's western ledge tips the scales
/// (ColosseumNavampaMatch_StepOnScales): the ledge sinks almost to the floor
/// with Isaac on it, and the eastern ledge rises. Stepping off swings them
/// back. The weight Move sets on the western ledge tips them the same way, and
/// once Frost has frozen the puddle on the eastern ledge
/// (ColosseumNavampaMatch_FreezeOnScales) stepping on no longer moves them.
///
/// Win or lose, the third match ends Colosso.
/// COLOSSEUM_NAVAMPA_MATCH_AFTER_MATCH_WON and _LOST set FLAG_COLOSSO_OVER, and
/// a win also FLAG_WON_COLOSSO.
///
/// In the event that follows (ColosseumNavampaMatch_PlayPostBattleReunion),
/// Garet, Ivan and Mia run to Isaac in the arena, but he falls asleep,
/// exhausted, and they leave him to sleep. Both exits lead to Babi's Palace,
/// where he wakes long after the finals.

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
#include "field/field_sequence.h"
#include "field/map_grid.h"
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
#include "scene/sleep_z.h"
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

/// The match held here, as the lettering `The 3rd Match` reads; the friends'
/// turns open with `Lend a Hand!` instead (Colosso_ShowOpeningLettering).
enum { FINALS_MATCH = 3 };

/// The stages, as their attendants number them: "This is stage 1 of the third
/// finals".
enum ColosseumNavampaMatchStage {
  STAGE_SCALES_OF_JUSTICE = 1,
  STAGE_STEPPING_STONE = 2,
  STAGE_WALL_WEDGE = 3,
  STAGE_FREE_CLIMB = 4,
  STAGE_MOVING_SIDEWALK = 5,
  STAGE_LOG_ROLLING = 6
};

/// FieldState.raisedTrigger with nothing raised.
enum { RAISED_TRIGGER_NONE = 0 };

/// The Moving Sidewalk Stage's two tileset animations: the belt's, which runs
/// while it carries the party west, and the one that runs instead once Force
/// has knocked its log off.
enum { SIDEWALK_RUNNING_ANIMATION = 1, SIDEWALK_STOPPED_ANIMATION = 2 };

/// SPRITE_STANDING_LOG's animations (standing_log.tsv) for the Moving
/// Sidewalk's log: it tips over (picture 1) and lies on its side, its cut end
/// facing the viewer (picture 2).
enum StandingLogAnimation {
  STANDING_LOG_ANIMATION_TOPPLE = 2,
  STANDING_LOG_ANIMATION_FALLEN = 3
};

/// The long log's roll through pictures 10 to 13 (log_north_south.tsv), which
/// it plays rolling west, where Vale Cave and Kolima's floodgate play it as
/// LOG_ANIMATION_ROLL_EAST. Rolling east it plays the same pictures backwards,
/// LOG_ANIMATION_ROLL_WEST.
// XXX: which way pictures 10 to 13 turn the log; a recording of the long log
// rolled each way, beside one of Vale Cave's west log, would settle it.
#define LONG_LOG_ANIMATION_ROLL_WEST 2

/// The Wall Wedge's operator's poses (young_man_with_fair_hair.tsv's held
/// pictures 54 to 56), in step with the pistons he works: as the first, third
/// and fifth pistons pull in, as they pass half out, and as they push out.
enum WallOperatorAnimation {
  WALL_OPERATOR_ANIMATION_PISTONS_HALF_OUT = 10,
  WALL_OPERATOR_ANIMATION_ODD_PISTONS_IN = 11,
  WALL_OPERATOR_ANIMATION_ODD_PISTONS_OUT = 12
};

/// Cells the stages' changes copy from, kept off the course: the Scales of
/// Justice's western ledge, cell (9, 12), sunk with someone on it, level
/// (CELL_BEHAVIOR_SCALES_LEDGE) and sunk under the weight; and the store at
/// (120..127, 60..65) where Scene_Initialize keeps layer 2's cells of the long
/// log's lane, (74..81, 60..65), before the log is marked in them.
enum ColosseumNavampaMatchSpareCell {
  SCALES_LEDGE_SUNK_COLUMN = 0,
  SCALES_LEDGE_SUNK_ROW = 24,
  SCALES_LEDGE_LEVEL_COLUMN = 0,
  SCALES_LEDGE_LEVEL_ROW = 25,
  SCALES_LEDGE_WEIGHTED_COLUMN = 1,
  SCALES_LEDGE_WEIGHTED_ROW = 25,
  LOG_LANE_STORE_COLUMN = 120,
  LOG_LANE_STORE_ROW = 60
};

s32 gColosseumNavampaMatch_SleepFrame BSS_DATA ALIGNED(16);
/// Unused: twelve bytes after the sleep frame that nothing reads.
u8 gColosseumNavampaMatch_UnusedAfterSleepFrame[12] BSS_DATA;
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
u8 gColosseumNavampaMatch_UnusedAfterBlendDuration[6] BSS_DATA;
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
extern const s32 gColosseumNavampaMatch_SidewalkLogOffBelt[];
extern const s8 gColosseumNavampaMatch_WallWedgeCycle[][6];
extern const u16 gColosseumNavampaMatch_FacingAngles[];
extern const s32 gColosseumNavampaMatch_SidewalkLogScript[];
extern const u8 gColosso_LetteringPaletteOffsets[];
extern const s16 gColosso_LetteringDigit[];
extern const s16 gColosso_LetteringLose[];
extern s32 gColosso_PrizeSparkleScript[];
extern s32 gColosso_PrizeScript[];
extern struct SceneEntrance gColosseumNavampaMatchEntrances[];
extern u32 gColosseumNavampaMatchExits[];
extern struct ActorPlacement gColosseumNavampaMatchActorPlacements[];
extern s32 gColosseumNavampaMatch_WallWedgeStep;
extern u32 gColosseumNavampaMatch_WallWedgeTimer;
extern struct Trigger gColosseumNavampaMatchTriggers[];
extern s32 gColosseumNavampaMatch_OpponentAfterBattle[];
extern s32 gColosseumNavampaMatch_LeaderActionA[];
extern s32 gColosseumNavampaMatch_LeaderActionB[];
extern s32 gColosseumNavampaMatch_LeaderActionC[];
extern s32 gColosseumNavampaMatch_LeaderScaleScript[];
extern s32 gColosseumNavampaMatch_SleepZScript[];
extern s16 gColosso_LetteringTitle[];
extern s16 gColosso_LetteringGo[];
extern s16 gColosso_LetteringWin[];
extern s32 gColosso_OpponentFinishScript[];
extern s32 gColosso_LeaderFinishScript[];
extern char gHexDigits[20];
extern s32 gPushedActor_DirectionSteps[];
extern s16 gColosso_LetteringSlot;
extern s16 gColosso_PointerSlot;

// UB: this calls a void function through an s32 function type, the call the
// bytes show; UBFIX calls it as declared.
#ifdef UBFIX
#define COLOSSEUM_NAVAMPA_MATCH_FACE_DIRECTION Actor_FaceDirectionById
#else
#define COLOSSEUM_NAVAMPA_MATCH_FACE_DIRECTION                                 \
  ((s32 (*)())Actor_FaceDirectionById)
#endif

static void ColosseumNavampaMatch_PlayPostBattleReunion(s32 battleResult);
static void ColosseumNavampaMatch_IntroduceThirdFinals(s32 guideId);
static void Colosso_ShowOpeningLettering(s32 match);
// Unused: the third match ends with the party's reunion in the arena
// (ColosseumNavampaMatch_PlayPostBattleReunion) instead.
UNUSED static void Colosso_ShowMatchResult(s32 result);
static void Colosso_NoOp(void);
static void Colosso_WaitForOpponentToFinish(void);
static s32 Colosso_TakePrize(s32 firstItem, s32 secondItem);
static void Colosso_PushActorAhead(void);

typedef void (*MapTaskEntry)(void);

/// The third finals match's actors, numbered in the order of
/// gColosseumNavampaMatchActorPlacements from ACTOR_FIRST_PLACED.
///
/// The rows of Isaac, Garet, Ivan and Mia take their own actors.
///
/// By the guide's and attendants' explanations, the Scales of Justice, the
/// Stepping-Stone Stage, the Wall Wedge, the Free-Climb Area, the Moving
/// Sidewalk Stage and another log-rolling area lead to the arena.
///
/// ACTOR_SCALES_WEIGHT to ACTOR_LOG_AREA_STONE_C are all drawn as the rocks
/// of Colosseum Final 1's Rock Challenge. The attendant of this match's
/// log-rolling area calls the ones there stones.
enum {
  /// Navampa, racing Isaac to the arena.
  ACTOR_OPPONENT = 8,
  /// The puddle Frost freezes on the Scales of Justice's eastern ledge.
  ACTOR_PUDDLE = 9,
  /// The Scales of Justice's two ledges.
  ACTOR_SCALES_WEST_LEDGE = 10,
  ACTOR_SCALES_EAST_LEDGE = 11,
  ACTOR_SCALES_WEIGHT = 12,
  ACTOR_STONE = 13,
  /// A stone pillar east of the Wall Wedge's pistons, by the operator. It can
  /// be pushed or moved, and nothing else comes of it
  /// (ColosseumNavampaMatch_JustMoveRock).
  ACTOR_ROCK = 14,
  /// The log-rolling area's three stone pillars. One pushed or moved off the
  /// ledge stays down in the tank as a step
  /// (ColosseumNavampaMatch_UpdateLogAreaStoneCells,
  /// FLAG_FIRST_LOG_AREA_STONE_SETTLED and the two after it).
  ACTOR_LOG_AREA_STONE_A = 15,
  ACTOR_LOG_AREA_STONE_B = 16,
  ACTOR_LOG_AREA_STONE_C = 17,
  /// The Wall Wedge's five pistons, west to east. Each is drawn by two
  /// actors, ACTOR_FIRST_PISTON + n and ACTOR_FIRST_PISTON + PISTON_COUNT
  /// + n.
  ACTOR_FIRST_PISTON = 18,
  ACTOR_LAST_PISTON = 22,
  /// A young man with fair hair who works the Wall Wedge.
  ACTOR_WALL_OPERATOR = 28,
  /// The plant Growth grows into a shortcut.
  ACTOR_GROWTH_PLANT = 29,
  /// The log Force knocks off the Moving Sidewalk Stage's conveyor belt.
  ACTOR_SIDEWALK_LOG = 30,
  /// The log-rolling area's long log.
  ACTOR_LONG_LOG = 31,
  /// A soldier who explains the third finals match.
  ACTOR_GUIDE = 32,
  /// The stages' attendants, young men with fair hair in white tunics.
  ACTOR_SCALES_ATTENDANT = 33,
  ACTOR_STEPPING_STONE_ATTENDANT = 34,
  ACTOR_WALL_WEDGE_ATTENDANT = 35,
  ACTOR_FREE_CLIMB_ATTENDANT = 36,
  ACTOR_MOVING_SIDEWALK_ATTENDANT = 37,
  ACTOR_LOG_ROLLING_ATTENDANT = 38,
  /// The arena's prizes, drawn as item icons at two blank actors
  /// (Colosso_ShowItemIcon).
  ACTOR_PRIZE_A = 39,
  ACTOR_PRIZE_B = 40,
  /// The first of the three letters Z that rise from the sleeping Isaac after
  /// the last match (ColosseumNavampaMatch_SpawnSleepZ).
  ACTOR_FIRST_SLEEP_Z = 41
};

/// The temporary flags that keep what the friends changed on the stages, for
/// the race, and the scales' state.
///
/// Scene_Initialize restores the stages from them.
enum {
  FLAG_PLANT_GROWN = FLAG_AREA_TEMP_60, ///< by Growth, into a shortcut
  FLAG_WALL_WEDGE_HALTED =
      FLAG_AREA_TEMP_61,                  ///< by Halt on ACTOR_WALL_OPERATOR
  FLAG_SCALES_FROZEN = FLAG_AREA_TEMP_62, ///< by Frost on ACTOR_PUDDLE
  /// By Force on ACTOR_SIDEWALK_LOG. The belt carries no one any more.
  FLAG_SIDEWALK_LOG_KNOCKED_OFF = FLAG_AREA_TEMP_63,
  /// Someone stands on the western ledge, which has sunk
  /// (ColosseumNavampaMatch_StepOnScales).
  FLAG_ON_SCALES = FLAG_AREA_TEMP_67,
  FLAG_WEIGHT_ON_SCALES =
      FLAG_AREA_TEMP_68, ///< ACTOR_SCALES_WEIGHT is on the western ledge
  FLAG_LONG_LOG_IN_PLACE =
      FLAG_AREA_TEMP_69, ///< at the east end
                         ///< (ColosseumNavampaMatch_RollLongLog)
  /// The byte of flags that keeps ACTOR_STONE's column.
  FLAG_STONE_COLUMN = FLAG_AREA_TEMP_70,
  /// ACTOR_LOG_AREA_STONE_A has settled. The next two flags are B's and C's.
  FLAG_FIRST_LOG_AREA_STONE_SETTLED = FLAG_SCENE_TEMP_14
};

/// The Wall Wedge's pistons stand in every other column, 32 pixels apart.
enum { PISTON_COUNT = 5, PISTON_SPACING = 32 };

/// The Wall Wedge pistons' animations (piston.png). Each piston is two actors:
/// its body, and PISTON_COUNT actors on, its tip, which shows below the body
/// when it is all the way out and plays the body's animation +
/// PISTON_ANIMATION_TIP.
enum PistonAnimation {
  PISTON_ANIMATION_IN = 1,
  PISTON_ANIMATION_HALF_OUT = 2,
  PISTON_ANIMATION_OUT = 3,
  PISTON_ANIMATION_PUSH_HALF_OUT = 4, ///< from in to half out
  PISTON_ANIMATION_PUSH_OUT = 5,      ///< from half out to all the way out
  PISTON_ANIMATION_PULL_HALF_IN = 6,  ///< from out to half out
  PISTON_ANIMATION_PULL_IN = 7,       ///< from half out to in
  PISTON_ANIMATION_TIP = 8
};

/// Cell behaviors the stages' triggers answer.
enum ColosseumNavampaMatchCellBehavior {
  /// The Moving Sidewalk Stage's belt, cells (62..71, 11) and on.
  CELL_BEHAVIOR_MOVING_SIDEWALK = 11,
  /// The Scales of Justice's western ledge, cell (9, 12), given this behavior
  /// from the spare cell (0, 25) while the scales are level.
  CELL_BEHAVIOR_SCALES_LEDGE = 15,
  /// Beside the ledge, cells (8..10, 13), where stepping off swings the scales
  /// back.
  CELL_BEHAVIOR_OFF_SCALES = 16,
  /// The weight's square by the Scales of Justice, round (4..7, 9..11), where
  /// walking into the weight pushes it.
  CELL_BEHAVIOR_WEIGHT_SQUARE = 20,
  /// The row of the stone pillar by the Wall Wedge's operator, (43..45, 11).
  CELL_BEHAVIOR_ROCK_ROW = 22,
  /// Where the long log's ride starts (ColosseumNavampaMatch_RollLongLog).
  CELL_BEHAVIOR_LONG_LOG = 23,
  /// The way into the arena, cells (92, 9..14).
  CELL_BEHAVIOR_ARENA_ENTRY = 50,
  /// The east end of the log-rolling area, cell (83, 13).
  CELL_BEHAVIOR_LOG_AREA_END = 90,
  /// The east edge of the weight's square, cell (8, 12).
  CELL_BEHAVIOR_WEIGHT_SQUARE_EDGE = 91,
  /// The chests: a Smoke Bomb at (12, 7), a Nut at (44, 6) and a Sleep Bomb at
  /// (75, 13), opened facing west.
  CELL_BEHAVIOR_SMOKE_BOMB_CHEST = 101,
  CELL_BEHAVIOR_NUT_CHEST = 102,
  CELL_BEHAVIOR_SLEEP_BOMB_CHEST = 103
};

const struct SceneEntrance *Scene_GetEntrances(void) {
  return gColosseumNavampaMatchEntrances;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gColosseumNavampaMatchExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gColosseumNavampaMatchActorPlacements;
}

/// The Wall Wedge's task steps its pistons through their cycle, a step every 18
/// frames.
static void ColosseumNavampaMatch_UpdateWallWedge(void) {
  struct FieldState *state = gFieldState;
  struct Actor *actor = Actor_Get(gMainRoutineState.leaderActorId);
  s32 row = FIELD_CELL(actor->z.fixed);
  s32 i;
  s32 kind;

  if (gColosseumNavampaMatch_WallWedgeTimer == 0) {
    gColosseumNavampaMatch_WallWedgeStep =
        (gColosseumNavampaMatch_WallWedgeStep + 1) & 3;
    for (i = ACTOR_FIRST_PISTON; i <= ACTOR_LAST_PISTON; i++) {
      kind = gColosseumNavampaMatch_WallWedgeCycle
          [gColosseumNavampaMatch_WallWedgeStep][i - ACTOR_FIRST_PISTON];
      Actor_SetAnimation(i, kind);
      Actor_SetAnimation(i + PISTON_COUNT, kind + PISTON_ANIMATION_TIP);
      // Each piston's column, (33 + 2n, 11..12), takes the walkway's
      // (32, 11..12), and is walled by (74, 12) while the piston is out.
      MapGrid_CopyCellAttributeRect(32, 11, 1, 2,
                                    (i - ACTOR_FIRST_PISTON) * 2 + 33, 11);
      if (kind != PISTON_ANIMATION_PULL_IN) {
        MapGrid_CopyCellAttributeRect(74, 12, 1, 1,
                                      (i - ACTOR_FIRST_PISTON) * 2 + 33, 11);
      }
    }
    Actor_SetAnimation(
        ACTOR_WALL_OPERATOR,
        gColosseumNavampaMatch_WallWedgeCycle
            [gColosseumNavampaMatch_WallWedgeStep][PISTON_COUNT]);
  } else {
    for (i = ACTOR_FIRST_PISTON; i <= ACTOR_LAST_PISTON; i++) {
      kind = gColosseumNavampaMatch_WallWedgeCycle
          [gColosseumNavampaMatch_WallWedgeStep][i - ACTOR_FIRST_PISTON];
      // The leader stands in the piston's column, which starts 48 pixels
      // west of i * PISTON_SPACING.
      if ((actor->x.fixed - toFIXED(i * PISTON_SPACING) > -toFIXED(50) &&
           actor->x.fixed - toFIXED(i * PISTON_SPACING) <= -toFIXED(30) - 1)) {
        if (row == 11 && kind == PISTON_ANIMATION_PUSH_HALF_OUT)
          state->raisedTrigger = kind;
        if (row == 12 && kind == PISTON_ANIMATION_PUSH_OUT)
          state->raisedTrigger = kind;
      }
    }
  }
  if (++gColosseumNavampaMatch_WallWedgeTimer > 17) {
    gColosseumNavampaMatch_WallWedgeTimer = 0;
  }
}

/// Resets the Wall Wedge's pistons to the first step of their cycle.
static void ColosseumNavampaMatch_ResetWallWedge(void) {
  MapTaskEntry entry;

  gColosseumNavampaMatch_WallWedgeTimer = 0;
  entry = ColosseumNavampaMatch_UpdateWallWedge;
  gColosseumNavampaMatch_WallWedgeStep = 0;
  Task_Remove(entry);
  entry();
}

/// Resumes cycling the Wall Wedge's pistons.
static void ColosseumNavampaMatch_StartWallWedge(void) {
  Task_AddFrame(ColosseumNavampaMatch_UpdateWallWedge, TASK_PASS_FRAME);
}

/// Waits, at most two seconds, for the step of the Wall Wedge's cycle that its
/// attendant's demonstration passes through.
static void ColosseumNavampaMatch_WaitForWallWedgeOpening(void) {
  s32 frames;

  wait(10);

  frames = 0;
  while (gColosseumNavampaMatch_WallWedgeStep != 3 ||
         gColosseumNavampaMatch_WallWedgeTimer != 1) {
    wait(1);
    frames++;
    if (frames > 119) {
      return;
    }
  }
}

/// The Moving Sidewalk Stage's belt carries the party west.
static void ColosseumNavampaMatch_RideMovingSidewalk(void) {
  struct FieldState *fieldState = gFieldState;
  struct MainRoutineState *state = &gMainRoutineState;
  s32 id = state->leaderActorId;
  struct Actor *subject = fieldState->camera;
  struct Actor *actor = Actor_Get(id);
  s32 z = actor->z.part[0].integer;

  if (z >= 183 && z <= 186) {
    subject->x.fixed -= toFIXED(4) / 5;
    actor->x.fixed -= toFIXED(4) / 5;
  }
}

/// Growth on the sprout in the Free-Climb Area, the fourth stage (cell
/// (49, 12)), grows it into a vine, a shortcut that saves the racer time
/// (FLAG_PLANT_GROWN).
static void ColosseumNavampaMatch_GrowPlant(void) {
  GameFlag_Set(FLAG_PLANT_GROWN);
  // Layer 2's wall at (49, 61..64) opens, as the column at (47, 61..64) is.
  MapGrid_CopyCellAttributes(47, 61, 1, 4, 49, 61);
}

/// Moves a scale ledge vertically to a height in whole pixels, at the shared
/// speed and acceleration.
static inline void ColosseumNavampaMatch_MoveOnScales(struct Actor *ledge,
                                                      s32 height) {
  ledge->acceleration = toFIXED(2) / 5;
  ledge->speedLimit = toFIXED(4) / 5;
  FieldActor_SetMoveTargetFX(ledge, ledge->x.fixed, toFIXED(height),
                             ledge->z.fixed);
}

/// Frost on the target on the Scales of Justice's eastern ledge freezes it.
static void ColosseumNavampaMatch_FreezeOnScales(void) {
  struct Actor *actor;

  actor = Actor_Get(ACTOR_PUDDLE);
  actor->scaleX = FIXED_ONE;
  actor->scaleY = FIXED_ONE;

  ColosseumNavampaMatch_MoveOnScales(Actor_Get(ACTOR_SCALES_EAST_LEDGE), 32);

  ColosseumNavampaMatch_MoveOnScales(Actor_Get(ACTOR_SCALES_WEST_LEDGE), 4);

  GameFlag_Set(FLAG_SCALES_FROZEN);
  // The frozen eastern ledge, (13, 12), and the western, (9, 12), become
  // squares to hop to, as (15, 12) and (14, 12) are.
  MapGrid_CopyCellAttributes(15, 12, 1, 1, 13, 12);
  MapGrid_CopyCellAttributes(14, 12, 1, 1, 9, 12);
}

/// Stepping onto the Scales of Justice's western ledge sinks it, unless the
/// scales are frozen.
static void ColosseumNavampaMatch_StepOnScales(void) {
  s32 actorId;
  s32 lowered;
  struct Actor *log;
  struct Actor *rider;

  actorId = gMainRoutineState.leaderActorId;
  lowered = GameFlag_Get(FLAG_SCALES_FROZEN);
  if (lowered == 0) {
    log = Actor_Get(ACTOR_SCALES_WEST_LEDGE);
    if (log != NULL) {
      Actor_SetDestination(actorId, log->x.part[0].integer,
                           log->z.part[0].integer);
    }
    Actor_WaitForMoveById(actorId);
    log = Actor_Get(ACTOR_SCALES_EAST_LEDGE);
    log->motionFlags = lowered;
    ColosseumNavampaMatch_MoveOnScales(log, 32);
    log = Actor_Get(ACTOR_SCALES_WEST_LEDGE);
    log->motionFlags = lowered;
    ColosseumNavampaMatch_MoveOnScales(log, 4);
    rider = Actor_Get(actorId);
    rider->motionFlags = lowered;
    ColosseumNavampaMatch_MoveOnScales(rider, 4);
    Actor_SetSpriteFlags(rider, SPRITE_FLAG_SHADOW);
    Actor_WaitForMoveById(actorId);
    MapGrid_CopyCellAttributes(SCALES_LEDGE_SUNK_COLUMN, SCALES_LEDGE_SUNK_ROW,
                               1, 1, 9, 12);
    wait(2);
    Actor_SetSpriteFlags(rider, SPRITE_FLAG_SHADOW);
    rider->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    rider->groundHeight = rider->y.fixed;
    GameFlag_Set(FLAG_ON_SCALES);
  }
}

/// Stepping off the Scales of Justice's western ledge swings the scales back.
static void ColosseumNavampaMatch_StepOffScales(void) {
  struct Actor *actor;

  Actor_SetAnimation(gMainRoutineState.leaderActorId, ANIM_STAND);

  actor = Actor_Get(ACTOR_SCALES_EAST_LEDGE);
  actor->motionFlags = 0;
  ColosseumNavampaMatch_MoveOnScales(actor, 4);

  actor = Actor_Get(ACTOR_SCALES_WEST_LEDGE);
  actor->motionFlags = 0;
  ColosseumNavampaMatch_MoveOnScales(actor, 32);

  Actor_WaitForMoveById(ACTOR_SCALES_WEST_LEDGE);
  MapGrid_CopyCellAttributes(SCALES_LEDGE_LEVEL_COLUMN, SCALES_LEDGE_LEVEL_ROW,
                             1, 1, 9, 12);
  wait(2);
  GameFlag_Clear(FLAG_ON_SCALES);
}

/// Move on the weight by the Scales of Justice can set it on the scales.
static void ColosseumNavampaMatch_SetWeightOnScales(void) {
  struct Actor *probe;
  struct Actor *log;
  struct Actor *upper;
  s32 x;
  s32 z;

  probe = Actor_Get(ACTOR_SCALES_WEIGHT);
  x = FIELD_CELL(probe->x.fixed);
  if (x == 9) {
    z = FIELD_CELL(probe->z.fixed);
    if (z == 12) {
      log = Actor_Get(ACTOR_SCALES_WEIGHT);
      Actor_SetSpriteFlags(log, 0);
      log->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      log->motionFlags = 0;
      ColosseumNavampaMatch_MoveOnScales(log, 4);
      upper = Actor_Get(ACTOR_SCALES_EAST_LEDGE);
      upper->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      ColosseumNavampaMatch_MoveOnScales(upper, 32);
      ColosseumNavampaMatch_MoveOnScales(Actor_Get(ACTOR_SCALES_WEST_LEDGE), 4);
      GameFlag_Set(FLAG_WEIGHT_ON_SCALES);
      MapGrid_CopyCellAttributes(15, 12, 1, 1, 13, z);
      MapGrid_CopyCellAttributes(SCALES_LEDGE_WEIGHTED_COLUMN,
                                 SCALES_LEDGE_WEIGHTED_ROW, 1, 1, x, z);
    }
  }
}

/// Walking into the weight by the Scales of Justice pushes it, perhaps onto the
/// scales.
static void ColosseumNavampaMatch_PushWeight(void) {
  Colosso_PushActorAhead();
  ColosseumNavampaMatch_SetWeightOnScales();
}

/// Move on the stone pillar of the Stepping-Stone Stage, the second stage,
/// slides it along its row; pushed east, it gives the racer a step across. The
/// same work follows every push.
///
/// The pillar's column is kept in FLAG_STONE_COLUMN, so that a friend's Move
/// during Lend a Hand still holds for the race. Row 11 of the stage is made
/// plain again, and the pillar's own cell is given the top of a pillar.
static void ColosseumNavampaMatch_MoveStone(void) {
  s32 x;

  x = FIELD_CELL(Actor_Get(ACTOR_STONE)->x.fixed);
  FieldGameFlag_SetByte(FLAG_STONE_COLUMN, x);
  // Row 11 takes the plain floor of (18..20, 10), and the pillar's cell the
  // square to hop to at (17, 11).
  MapGrid_CopyCellAttributes(18, 10, 3, 1, 18, 11);
  MapGrid_CopyCellAttributes(17, 11, 1, 1, x, 11);
}

/// Move on the stone pillar by the Wall Wedge's operator, east of the pistons,
/// just slides it; nothing else happens.
static void ColosseumNavampaMatch_JustMoveRock(void) {}

/// Walking into the stone pillar by the Wall Wedge's operator, from the row
/// it stands in (cells (43..45, 11)), pushes it a cell.
static void ColosseumNavampaMatch_PushRock(void) { Colosso_PushActorAhead(); }

/// Move on one of the three stone pillars of the last stage, the log-rolling
/// area, slides it a cell. A pillar that comes down off the ledge into the
/// tank stays there, drawn under the party, as a square to hop across, and
/// the long log can no longer roll over that cell.
///
/// Each pillar that has come down is remembered by its flag
/// (FLAG_FIRST_LOG_AREA_STONE_SETTLED and the two after it), and its cell is
/// marked in both copies of the map, the second 52 rows further down.
static void ColosseumNavampaMatch_UpdateLogAreaStoneCells(void) {
  struct Actor *actor;
  s32 slot;
  s32 x;
  s32 z;
  s32 x2;
  s32 z2;

  for (slot = ACTOR_LOG_AREA_STONE_A; slot <= ACTOR_LOG_AREA_STONE_C; slot++) {
    actor = Actor_Get(slot);
    if (MapGrid_GetTerrainHeight(0, actor->x.fixed, actor->z.fixed) == 0) {
      actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      actor->motionFlags = 0;
      x = FIELD_CELL(actor->x.fixed);
      z = FIELD_CELL(actor->z.fixed);
      // The cell takes the behaviour of the area's east end, (83, 13).
      MapGrid_CopyCellAttributes(83, 13, 1, 1, x, z);
      x2 = FIELD_CELL(actor->x.fixed);
      z2 = FIELD_CELL(actor->z.fixed);
      MapGrid_CopyCellAttributes(83, 13, 1, 1, x2, z2 + 52);
      GameFlag_Set(slot - ACTOR_LOG_AREA_STONE_A +
                   FLAG_FIRST_LOG_AREA_STONE_SETTLED);
    }
  }
}

/// A task for Colosseum Final 3's eastern end.
static void ColosseumNavampaMatch_UpdateEastEndCollision(void) {
  struct Actor *actor;
  s32 x;
  s32 z;
  s32 behavior;

  actor = Actor_Get(gMainRoutineState.leaderActorId);
  x = FIELD_CELL(actor->x.fixed);
  behavior = CELL_BEHAVIOR_LONG_LOG;
  z = FIELD_CELL(actor->z.fixed);
  if (x == 81 && z == 12) {
    if (FACING_EIGHTH(actor->facing) == FACING_SOUTH) {
      behavior = CELL_BEHAVIOR_CLIMB;
    }
    MapGrid_SetCellBehaviorAt(0, FIELD_CELL_ORIGIN(x), FIELD_CELL_ORIGIN(z),
                              behavior);
  }
}

/// Rejects blocked cells and cells occupied by one of the log-rolling area's
/// three stones.
static s32 ColosseumNavampaMatch_CheckLongLogCell(s32 x, s32 z) {
  struct Actor *actor;

  if (MapGrid_GetCellBehaviorAt(0, x, z) == CELL_BEHAVIOR_WALL) {
    return -2;
  }
  actor = Actor_Get(ACTOR_LOG_AREA_STONE_A);
  x = FIELD_CELL(x);
  z = FIELD_CELL(z);
  if (FIELD_CELL(actor->x.fixed) == x && FIELD_CELL(actor->z.fixed) == z) {
    return -1;
  }
  actor = Actor_Get(ACTOR_LOG_AREA_STONE_B);
  if (FIELD_CELL(actor->x.fixed) == x && FIELD_CELL(actor->z.fixed) == z) {
    return -1;
  }
  actor = Actor_Get(ACTOR_LOG_AREA_STONE_C);
  if (FIELD_CELL(actor->x.fixed) == x && FIELD_CELL(actor->z.fixed) == z) {
    return -1;
  }
  return 0;
}

/// Checks all four cells covered by the long rolling log.
static s32 ColosseumNavampaMatch_CheckLongLogPath(s32 x, s32 z) {
  if (ColosseumNavampaMatch_CheckLongLogCell(x, z - toFIXED(24)) != 0 ||
      ColosseumNavampaMatch_CheckLongLogCell(x, z - toFIXED(8)) != 0 ||
      ColosseumNavampaMatch_CheckLongLogCell(x, z + toFIXED(8)) != 0 ||
      ColosseumNavampaMatch_CheckLongLogCell(x, z + toFIXED(24)) != 0) {
    return -1;
  }
  return 0;
}

/// The long log carries the leader until something stops it.
static void ColosseumNavampaMatch_RollLongLog(void) {
  struct Actor *rider;
  struct Actor *log;
  struct FieldState *state;
  s32 angle;
  s32 actorId;
  FIXED x;
  FIXED z;
  s32 steps;
  FIXED nextX;
  s32 column;
  FIXED acceleration;
  s32 rollAnimation;
  s32 step;
  s32 ahead[3];

  rider = Actor_Get(gMainRoutineState.leaderActorId);
  log = Actor_Get(ACTOR_LONG_LOG);
  steps = 0;
  angle =
      gColosseumNavampaMatch_FacingAngles[FACING_EIGHTH_INDEX(rider->facing)];
  actorId = gMainRoutineState.leaderActorId;
  SET_VECTOR(ahead, (rider->x.fixed & FIELD_CELL_MASK) + toFIXED(8),
             rider->y.fixed, (rider->z.fixed & FIELD_CELL_MASK) + toFIXED(8));
  Vector_AddPolarOffset(toFIXED(16), angle, ahead);
  x = log->x.fixed;
  z = log->z.fixed;
  // Only a nearby rider rolls the log; other interactions push the actor
  // ahead.
  if ((ahead[0] - x >= 0 ? ahead[0] - x : x - ahead[0]) > toFIXED(8) ||
      (ahead[2] - z >= 0 ? ahead[2] - z : z - ahead[2]) > toFIXED(32)) {
    goto far;
  }
  if (gKeysHeld & KEY_LEFT) {
    rollAnimation = LONG_LOG_ANIMATION_ROLL_WEST;
    step = -8;
    for (;;) {
      nextX = x - toFIXED(16);
      if (ColosseumNavampaMatch_CheckLongLogPath(nextX, z) == 0) {
        steps++;
        x = nextX;
      } else {
        break;
      }
    }
  } else {
    if (!(gKeysHeld & KEY_RIGHT)) {
      return;
    }
    rollAnimation = LOG_ANIMATION_ROLL_WEST;
    step = 8;
    for (;;) {
      nextX = x + toFIXED(16);
      if (ColosseumNavampaMatch_CheckLongLogPath(nextX, z) == 0) {
        steps++;
        x = nextX;
      } else {
        break;
      }
    }
  }
  if (steps == 0) {
    return;
  }
  // The log's column takes the lane's floor again, (74, 8..11), and layer 2's
  // lane is restored from its store.
  MapGrid_CopyCellAttributes(74, 8, 1, 4, FIELD_CELL(log->x.fixed), 9);
  MapGrid_CopyCellAttributes(LOG_LANE_STORE_COLUMN, LOG_LANE_STORE_ROW, 8, 5,
                             74, 60);
  Event_Start();
  Actor_SetAnimation(actorId, ANIM_PUSH);
  Event_Wait(6);
  log->speedLimit = toFIXED(1) / 2;
  acceleration = toFIXED(1) / 5;
  log->acceleration = acceleration;
  Actor_SelectAnimation(log, rollAnimation);
  FieldActor_SetMoveTargetFX(log, x, 0, z);
  Event_Wait(6);
  Actor_SetAnimation(actorId, ANIM_WALK);
  state = HeapSlot_AllocEwram(HEAP_SLOT_FIELD, sizeof(struct FieldState));
  Actor_StartCamera(state->camera, (s32)log);
  Actor_SetSpeedFX(actorId, toFIXED(1) / 2, acceleration);
  Sound_Play(SE_HEAVY_MOVE);
  Actor_SelectAnimation(rider, ANIM_WALK);
  FieldActor_SetMoveTargetFX(rider, toFIXED(steps * step) + rider->x.fixed, 0,
                             rider->z.fixed);
  Actor_WaitForMove(rider);
  Actor_SelectAnimation(rider, ANIM_STAND);
  Actor_WaitForMove(log);
  // At the eastern end the log settles at the end of its stage.
  if (x >= toFIXED(1328)) {
    GameFlag_Set(FLAG_LONG_LOG_IN_PLACE);
    Actor_SetAnimation(ACTOR_LONG_LOG, LOG_ANIMATION_ROLL_WEST);
    Actor_SetDestinationOffset(ACTOR_LONG_LOG, 18, 6);
    Event_Wait(30);
    Actor_SelectAnimation(log, LOG_ANIMATION_DROP_IN);
    Actor_WaitForMove(log);
    log->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    // Fallen in, its cells (84, 10..12) are squares to hop across, as
    // (86, 9..11) are.
    column = 84;
    MapGrid_CopyCellAttributes(86, 10, 1, 2, column, 10);
    MapGrid_CopyCellAttributes(86, 9, 1, 1, column, 12);
    Sound_Play(SE_STOP_ACTION_SE);
    Sound_Play(SE_LOG_FALL);
  } else {
    Actor_SelectAnimation(log, LOG_ANIMATION_AT_REST);
    Sound_Play(SE_STOP_ACTION_SE);
    Sound_Play(SE_HEAVY_STOP);
    // Its new column, on the ground and on layer 2, is the wall of
    // (85, 9..12).
    column = FIELD_CELL(x);
    MapGrid_CopyCellAttributes(85, 9, 1, 4, column, 9);
    MapGrid_CopyCellAttributes(85, 9, 1, 4, column, 61);
  }
  Event_Wait(15);
  Event_End();
  return;
far:
  Colosso_PushActorAhead();
  ColosseumNavampaMatch_UpdateLogAreaStoneCells();
}

/// Displays the activated moving sidewalk and plays its arrival sounds.
static s32
ColosseumNavampaMatch_ShowActivatedSidewalkLayer(struct Actor *interpreter) {
  TilesetAnim_Pause(SIDEWALK_RUNNING_ANIMATION);
  TilesetAnim_Resume(SIDEWALK_STOPPED_ANIMATION);
  Sound_Play(SE_STOP_ACTION_SE);
  Sound_Play(SE_FLOOR_SWITCH);
  return 0;
}

/// Force on the Moving Sidewalk Stage's log makes it slide away.
static void ColosseumNavampaMatch_ForceSidewalkLog(void) {
  struct Actor *effect;
  FIXED moveRate;

  effect = Actor_Get(ACTOR_SIDEWALK_LOG);
  effect->motionFlags = 0;
  moveRate = toFIXED(8) / 5;
  effect->acceleration = moveRate;
  effect->speedLimit = moveRate;
  Actor_SelectAnimation(effect, STANDING_LOG_ANIMATION_TOPPLE);
  Actor_StartScript(effect, gColosseumNavampaMatch_SidewalkLogScript);
  GameFlag_Set(FLAG_SIDEWALK_LOG_KNOCKED_OFF);
}

/// Halt on the Wall Wedge's operator stops the pistons at their next open step
/// (FLAG_WALL_WEDGE_HALTED).
static void ColosseumNavampaMatch_HaltWall(void) {
  FieldPsynergy_HaltActor(ACTOR_WALL_OPERATOR);
  GameFlag_Set(FLAG_WALL_WEDGE_HALTED);
  wait(10);
  while (gColosseumNavampaMatch_WallWedgeStep != 1 &&
         gColosseumNavampaMatch_WallWedgeStep != 3)
    wait(1);
  wait(1);
  Task_Remove(ColosseumNavampaMatch_UpdateWallWedge);
}

/// A piston of the Wall Wedge, the third stage, punching halfway out while
/// Isaac stands before it on the walkway's north row (row 11) shoves him a
/// cell and a half south.
///
/// ColosseumNavampaMatch_UpdateWallWedge raises this event with the piston's
/// animation number, PISTON_ANIMATION_PUSH_HALF_OUT.
static void ColosseumNavampaMatch_ShovedByPiston(void) {
  struct Actor *actor;
  s32 z;

  actor = Actor_Get(gMainRoutineState.leaderActorId);
  actor->acceleration = toFIXED(1);
  actor->speedLimit = toFIXED(2);
  Actor_SetAttachedEffect(gMainRoutineState.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SelectAnimation(actor, ANIM_LEADER_RUN);
  z = actor->z.fixed & FIELD_CELL_MASK;
  FieldActor_SetMoveTargetFX(actor, actor->x.fixed, actor->y.fixed,
                             z + toFIXED(24));
  Actor_WaitForMove(actor);
}

/// A piston of the Wall Wedge punching all the way out while Isaac stands
/// before it on the walkway's south row (row 12) shoves him off the walkway,
/// and he drops down the ledge below.
///
/// ColosseumNavampaMatch_UpdateWallWedge raises this event with the piston's
/// animation number, PISTON_ANIMATION_PUSH_OUT.
static void ColosseumNavampaMatch_ShovedOffWalkway(void) {
  struct Actor *actor;
  s32 z;

  actor = Actor_Get(gMainRoutineState.leaderActorId);
  if (actor->x.fixed > toFIXED(664)) {
    actor->x.fixed = toFIXED(664);
  }
  actor->acceleration = toFIXED(1);
  actor->speedLimit = toFIXED(2);
  Actor_SelectAnimation(actor, ANIM_LEADER_RUN);
  z = actor->z.fixed & FIELD_CELL_MASK;
  FieldActor_SetMoveTargetFX(actor, actor->x.fixed, actor->y.fixed,
                             z + toFIXED(12));
  Actor_WaitForMove(actor);
  Actor_SetAttachedEffect(gMainRoutineState.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldActor_JumpDownLedge(gMainRoutineState.leaderActorId, 6, 0);
}

/// Unused: an empty function nothing refers to.
UNUSED static void ColosseumNavampaMatch_UnusedNoOp(void) {}

// XXX (no evidence): which lines this question was written for; in the
// Japanese catalog, as in the English one, the lines three apart per character
// from MsgColossoDidntThinkBattles are a spectator's thought and the stage
// announcements, and no catalog older than the released ones survives.
/// Unused: talking to a character at the third finals site would have handed
/// the player control of that character.
///
/// Nothing calls it. Arriving at COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_SOUTH
/// it would ask a yes or no question and, on yes, make the character it names
/// the one the player moves alone, as the friends' turns do.
UNUSED static void
ColosseumNavampaMatch_UnusedAskToControlCharacter(s32 characterId) {
  struct FieldState *state;
  s32 actorId;
  s32 messageOffset;
  s32 entrance;

  state = gFieldState;
  actorId = gMainRoutineState.leaderActorId;
  entrance = gMainRoutineState.entrance;
  if (entrance == COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_SOUTH) {
    Event_Start();
    messageOffset = characterId * 3;
    Event_SetMessageId(messageOffset + MsgColossoDidntThinkBattles);
    Event_OpenMessage(characterId, 0);
    if (Event_AskYesNoWithActorAnimation(actorId, 0) == 0) {
      Event_SetMessageId(messageOffset + MsgColossoDidntThinkBattles + 1);
      Event_ShowMessage(characterId, 0);
      state->transition =
          SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
      state->transitionFrames = 15;
      Event_FinishScreenTransition();
      Event_WaitTransitionFrames();
      Colosso_ControlCharacterAlone(characterId);
      Event_StartScreenTransition();
      Event_WaitTransitionFrames();
    } else {
      Event_SetMessageId(messageOffset + MsgColossoDidntThinkBattles + 2);
      Event_ShowMessage(characterId, 0);
    }
    Event_End();
  }
}

/// Reaching Colosseum Final 3's arena, the leader takes a prize and fights
/// Navampa.
static void ColosseumNavampaMatch_ReachArena(void) {
  s32 opponentFirst;
  s32 i;
  s32 opponentFirstSlot = 4;
  Colosso_NoOp();
  Event_Start();
  opponentFirst = Colosso_TakePrize(ITEM_CLAYMORE, ITEM_HUNTERS_SWORD);
  Colosso_WaitForOpponentToFinish();
  // Up to ten waits of 300 frames while the opponent holds up his prize.
  for (i = 0; i < 10; i++) {
    Actor_WaitForScriptEndById(ACTOR_OPPONENT);
  }
  Actor_SetSpeed(ACTOR_OPPONENT, 1);
  Actor_WalkTo(ACTOR_OPPONENT, 1528, 192);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 1);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 1496, 192);
  Actor_SetAnimation(ACTOR_OPPONENT, ANIM_STAND);
  Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_OPPONENT, 0);
  Event_Wait(10);
  Actor_SetAnimation(ACTOR_OPPONENT, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(20);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 2);
  Actor_SetSpeed(ACTOR_OPPONENT, 2);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 1504, 192);
  Actor_WalkToAndWait(ACTOR_OPPONENT, 1520, 192);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_STRIDE);
  Actor_SetAnimation(ACTOR_OPPONENT, WARRIOR_ANIMATION_FIGHTING_STANCE);
  Event_Wait(10);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_COLOSSO,
                                        opponentFirstSlot - opponentFirst + 1);
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  Event_SetAfterBattleDestination(SCENE_COLOSSEUM_NAVAMPA_MATCH,
                                  COLOSSEUM_NAVAMPA_MATCH_AFTER_MATCH_WON);
  Event_SetDefeatDestination(SCENE_COLOSSEUM_NAVAMPA_MATCH,
                             COLOSSEUM_NAVAMPA_MATCH_AFTER_MATCH_LOST);
  GameFlag_Set(FLAG_KEEP_MUSIC_ONCE);
}

/// Walking on from the east end of the log-rolling area (cell (83, 13)) hops
/// Isaac forward across the gap, if no stone pillar stands in the way and
/// there is somewhere to land.
static void ColosseumNavampaMatch_HopFromLogAreaEnd(void) {
  if (Colosso_FindActorAhead() == 0) {
    Field_TryJumpForward();
  }
}

/// Walking on from the east edge of the weight's square by the Scales of
/// Justice (cell (8, 12)) pushes the weight if it stands ahead, perhaps onto
/// the scales. With nothing there, Isaac hops forward across the gap where
/// there is somewhere to land.
static void ColosseumNavampaMatch_PushWeightOrHopAcross(void) {
  if (Colosso_FindActorAhead() == 0) {
    Field_TryJumpForward();
  } else {
    ColosseumNavampaMatch_PushWeight();
  }
}

const struct Trigger *Scene_GetTriggers(void) {
  return gColosseumNavampaMatchTriggers;
}

/// Sets up Colosseum Final 3 for the visit its entrance stands for.
s32 Scene_Initialize(void) {
  struct Actor *actor;
  s32 i;
  FIXED height;
  s32 column;

  gFieldState->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  GameFlag_Set(FLAG_RETREAT_BLOCKED);
  Task_AddFrame(ColosseumNavampaMatch_UpdateEastEndCollision, TASK_PASS_FRAME);
  MapGrid_CopyCellAttributes(74, 60, 8, 6, LOG_LANE_STORE_COLUMN,
                             LOG_LANE_STORE_ROW);

  Actor_SetSpriteFlags(Actor_Get(ACTOR_PUDDLE), 0);

  actor = Actor_Get(ACTOR_SCALES_WEST_LEDGE);
  Actor_SetSpriteFlags(actor, 0);
  actor->motionFlags = 0;
  actor->y.fixed = toFIXED(32);

  actor = Actor_Get(ACTOR_SCALES_EAST_LEDGE);
  Actor_SetSpriteFlags(actor, 0);
  actor->motionFlags = 0;
  actor->y.fixed = toFIXED(4);

  if (GameFlag_Get(FLAG_SCALES_FROZEN) != 0) {
    Actor_SetAnimation(ACTOR_PUDDLE, PUDDLE_ANIMATION_PILLAR);
    actor = Actor_Get(ACTOR_SCALES_WEST_LEDGE);
    actor->y.fixed = toFIXED(4);
    actor = Actor_Get(ACTOR_SCALES_EAST_LEDGE);
    actor->y.fixed = toFIXED(32);
    MapGrid_CopyCellAttributes(15, 12, 1, 1, 13, 12);
  } else {
    actor = Actor_Get(ACTOR_PUDDLE);
    actor->scaleX = 3 * FIXED_ONE / 2;
    actor->scaleY = 3 * FIXED_ONE / 2;
    // The cells ColosseumNavampaMatch_StepOnScales copies as it sets the flag,
    // redrawn on arrival.
    if (GameFlag_Get(FLAG_ON_SCALES) != 0) {
      MapGrid_CopyCellAttributes(SCALES_LEDGE_SUNK_COLUMN,
                                 SCALES_LEDGE_SUNK_ROW, 1, 1, 9, 12);
    } else {
      MapGrid_CopyCellAttributes(SCALES_LEDGE_LEVEL_COLUMN,
                                 SCALES_LEDGE_LEVEL_ROW, 1, 1, 9, 12);
    }
  }

  // The Scales of Justice are two pans on poles. With the weight, a rock, on
  // the western ledge they have tipped: the west pan hangs low and the east
  // one high, where otherwise the west is high and the east low. The two
  // cells under the pans take the ground that goes with it.
  if (GameFlag_Get(FLAG_WEIGHT_ON_SCALES) != 0) {
    MapGrid_CopyCellAttributes(15, 12, 1, 1, 13, 12);
    MapGrid_CopyCellAttributes(SCALES_LEDGE_WEIGHTED_COLUMN,
                               SCALES_LEDGE_WEIGHTED_ROW, 1, 1, 9, 12);
    actor = Actor_Get(ACTOR_SCALES_WEIGHT);
    Actor_SetSpriteFlags(actor, 0);
    actor->motionFlags = 0;
    actor->y.fixed = toFIXED(2);
    actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    actor = Actor_Get(ACTOR_SCALES_WEST_LEDGE);
    actor->y.fixed = toFIXED(4);
    actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    actor = Actor_Get(ACTOR_SCALES_EAST_LEDGE);
    actor->y.fixed = toFIXED(32);
  }

  column = FieldGameFlag_GetByte(FLAG_STONE_COLUMN);
  if (column == 0) {
    column = 19;
  }
  actor = Actor_Get(ACTOR_STONE);
  actor->x.fixed = FIELD_CELL_CENTER(column);
  actor->motionFlags = 0;
  actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  MapGrid_CopyCellAttributes(18, 10, 3, 1, 18, 11);
  MapGrid_CopyCellAttributes(17, 11, 1, 1, column, 11);

  for (i = ACTOR_LOG_AREA_STONE_A; i <= ACTOR_LOG_AREA_STONE_C; i++) {
    actor = Actor_Get(i);
    height = MapGrid_GetTerrainHeight(0, actor->x.fixed, actor->z.fixed);
    if (actor->y.fixed == 0 && height == 0) {
      actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      actor->motionFlags = height;
      MapGrid_CopyCellAttributes(83, 13, 1, 1, FIELD_CELL(actor->x.fixed),
                                 FIELD_CELL(actor->z.fixed));
      MapGrid_CopyCellAttributes(83, 13, 1, 1, FIELD_CELL(actor->x.fixed),
                                 (FIELD_CELL(actor->z.fixed)) + 52);
    }
  }

  // Halt preserves a stationary wall; otherwise restart its cycle.
  if (GameFlag_Get(FLAG_WALL_WEDGE_HALTED) != 0) {
    for (i = ACTOR_FIRST_PISTON; i <= ACTOR_LAST_PISTON; i++) {
      actor = Actor_Get(i);
      actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      Actor_SelectAnimation(actor, PISTON_ANIMATION_HALF_OUT);
      actor = Actor_Get(i + PISTON_COUNT);
      actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      actor->motionFlags = 0;
      actor->y.fixed = toFIXED(32);
      Actor_SelectAnimation(actor,
                            PISTON_ANIMATION_HALF_OUT + PISTON_ANIMATION_TIP);
      MapGrid_CopyCellAttributes(74, 12, 1, 1, i * 2 - 3, 11);
    }
    Actor_SetAnimation(ACTOR_WALL_OPERATOR,
                       WALL_OPERATOR_ANIMATION_PISTONS_HALF_OUT);
    FieldPsynergy_HaltActor(ACTOR_WALL_OPERATOR);
  } else {
    for (i = ACTOR_FIRST_PISTON; i <= ACTOR_LAST_PISTON; i++) {
      actor = Actor_Get(i);
      actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      actor = Actor_Get(i + PISTON_COUNT);
      actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      actor->motionFlags = 0;
      actor->y.fixed = toFIXED(32);
    }
    Task_AddFrame(ColosseumNavampaMatch_UpdateWallWedge, TASK_PASS_FRAME);
  }

  if (GameFlag_Get(FLAG_PLANT_GROWN) != 0) {
    Actor_SetAnimation(ACTOR_GROWTH_PLANT, ANIM_SHAKE_HEAD);
    MapGrid_CopyCellAttributes(47, 61, 1, 4, 49, 61);
  }

  if (GameFlag_Get(FLAG_SIDEWALK_LOG_KNOCKED_OFF) != 0) {
    TilesetAnim_Pause(SIDEWALK_RUNNING_ANIMATION);
    actor = Actor_Get(ACTOR_SIDEWALK_LOG);
    actor->motionFlags = 0;
    actor->x.fixed = toFIXED(1130);
    actor->z.fixed = toFIXED(184);
    Actor_SetSpriteFlags(actor, 0);
    Actor_SelectAnimation(actor, STANDING_LOG_ANIMATION_FALLEN);
    Actor_StartScript(actor, gColosseumNavampaMatch_SidewalkLogOffBelt);
  } else {
    TilesetAnim_Pause(SIDEWALK_STOPPED_ANIMATION);
  }

  if (GameFlag_Get(FLAG_LONG_LOG_IN_PLACE) != 0) {
    actor = Actor_Get(ACTOR_LONG_LOG);
    Actor_SelectAnimation(actor, LOG_ANIMATION_DROP_IN);
    actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    MapGrid_CopyCellAttributes(86, 10, 1, 2, 84, 10);
    MapGrid_CopyCellAttributes(86, 9, 1, 1, 84, 12);
  } else {
    actor = Actor_Get(ACTOR_LONG_LOG);
    MapGrid_CopyCellAttributes(85, 9, 1, 4, FIELD_CELL(actor->x.fixed), 9);
    MapGrid_CopyCellAttributes(85, 9, 1, 4, FIELD_CELL(actor->x.fixed), 61);
  }

  actor = Actor_Get(ACTOR_PUDDLE);
  actor->motionFlags = 0;
  actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  actor = Actor_Get(ACTOR_SCALES_WEST_LEDGE);
  actor->motionFlags = 0;
  actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  actor = Actor_Get(ACTOR_SCALES_EAST_LEDGE);
  actor->motionFlags = 0;
  actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  Actor_SetAnimation(ACTOR_OPPONENT, WARRIOR_ANIMATION_FIGHTING_STANCE);
  gMainRoutineState.movementMode = LEADER_MOVEMENT_WALK;
  Colosso_ShowItemIcon(ACTOR_PRIZE_A, ITEM_CLAYMORE);
  Colosso_ShowItemIcon(ACTOR_PRIZE_B, ITEM_HUNTERS_SWORD);
  Actor_SetSpriteColorById(ACTOR_OPPONENT, SPRITE_COLOR_RBG);

  // Entrances: the race, the friends' turns, the tour, and the end of Colosso
  // after a win or a loss.
  switch (gMainRoutineState.entrance) {
  case COLOSSEUM_NAVAMPA_MATCH_RACE_FROM_LEND_A_HAND:
    Colosso_StartRaceGauge(ACTOR_PARTY_LEADER, ACTOR_OPPONENT, 6, toFIXED(1512),
                           toFIXED(192), ACTOR_PRIZE_A, ACTOR_PRIZE_B);
    // The doorway Isaac sets out from, (5, 2..3), takes the two cells kept at
    // (127, 0..1) for the race.
    MapGrid_CopyCellsLayered(127, 0, 1, 2, 5, 2);
    Actor_ReleaseById(ACTOR_GUIDE);
    Actor_ReleaseById(ACTOR_SCALES_ATTENDANT);
    Actor_ReleaseById(ACTOR_STEPPING_STONE_ATTENDANT);
    Actor_ReleaseById(ACTOR_WALL_WEDGE_ATTENDANT);
    Actor_ReleaseById(ACTOR_FREE_CLIMB_ATTENDANT);
    Actor_ReleaseById(ACTOR_MOVING_SIDEWALK_ATTENDANT);
    Actor_ReleaseById(ACTOR_LOG_ROLLING_ATTENDANT);
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
      Sound_Play(SOUND_FADE_OUT_BGM);
      Colosso_ControlCharacterAlone(ISAAC);
      Colosso_PlaceFriendsAtCheeringSpots();
      Actor_WatchAndSetScript(ACTOR_GARET, ACTOR_PARTY_LEADER);
      Colosso_ShowOpeningLettering(FINALS_MATCH);
    }
    Actor_WatchAndSetScript(ACTOR_GARET, ACTOR_PARTY_LEADER);
    Actor_WatchAndSetScript(ACTOR_IVAN, ACTOR_PARTY_LEADER);
    Actor_WatchAndSetScript(ACTOR_MIA, ACTOR_PARTY_LEADER);
    Colosso_StartOpponent(RESOURCE_NAVAMPA_OPPONENT_PATH);
    break;

  case COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_SOUTH:
    Task_AddFrame(Colosso_RaiseTakePlaceOnPsynergy, TASK_PASS_FRAME);
    Actor_ReleaseById(ACTOR_PRIZE_A);
    Actor_ReleaseById(ACTOR_PRIZE_B);
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) != 0) {
      break;
    }
    Colosso_PlaceFriendsAtCheeringSpots();
    Colosso_ControlCharacterAlone(GARET);
    Colosso_ShowOpeningLettering(LETTERING_LEND_A_HAND);
    break;

  case COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_NORTH:
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) != 0) {
      break;
    }
    ColosseumNavampaMatch_IntroduceThirdFinals(ACTOR_GUIDE);
    Colosso_ClearCheeringSpots();
    break;

  case COLOSSEUM_NAVAMPA_MATCH_AFTER_MATCH_WON:
    ColosseumNavampaMatch_PlayPostBattleReunion(1);
    Import_EventSetExitRequest(COLOSSEUM_NAVAMPA_MATCH_TO_BABI_PALACE);
    GameFlag_Set(FLAG_COLOSSO_OVER);
    GameFlag_Set(FLAG_WON_COLOSSO);
    break;

  case COLOSSEUM_NAVAMPA_MATCH_AFTER_MATCH_LOST:
    ColosseumNavampaMatch_PlayPostBattleReunion(-1);
    Import_EventSetExitRequest(COLOSSEUM_NAVAMPA_MATCH_TO_BABI_PALACE_LOST);
    GameFlag_Set(FLAG_COLOSSO_OVER);
    break;
  }
  return 0;
}

/// Advances one of the particles that rise from the sleeping Isaac after the
/// last match.
static s32 ColosseumNavampaMatch_AdvanceSleepZ(struct Actor *particle) {
  particle->x.fixed += particle->timer * (toFIXED(1) / SLEEP_Z_STEPS_PER_PIXEL);
  particle->y.fixed +=
      (s16)particle->userValue * (toFIXED(1) / SLEEP_Z_STEPS_PER_PIXEL);
  particle->scaleX += FIXED_ONE / 40;
  particle->scaleY += FIXED_ONE / 40;
  particle->timer += 5;
  particle->userValue = (s16)particle->userValue - 1;
  return 0;
}

/// Shows one of the three particles that rise from the sleeping Isaac, each at
/// its point in a three-second cycle.
static void ColosseumNavampaMatch_SpawnSleepZ(void) {
  struct Actor *particle;
  struct Actor *source;
  FIXED x;
  FIXED y;
  s32 actorId;
  s32 frame;

  particle = Actor_Get(ACTOR_PARTY_LEADER);
  frame = gColosseumNavampaMatch_SleepFrame + 1;
  actorId = ACTOR_FIRST_SLEEP_Z;
  x = particle->x.fixed;
  y = particle->y.fixed;
  gColosseumNavampaMatch_SleepFrame = frame;
  switch (frame % 180) {
  case 10:
    break;
  case 20:
    actorId = ACTOR_FIRST_SLEEP_Z + 1;
    break;
  case 30:
    actorId = ACTOR_FIRST_SLEEP_Z + 2;
    break;
  default:
    return;
  }
  particle = Actor_Get(actorId);
  if (particle == NULL) {
    return;
  }
  source = Actor_Get(ACTOR_PARTY_LEADER);
  if (source != NULL) {
    Actor_SetPositionFX(actorId, source->x.fixed, source->z.fixed);
  }
  Actor_SetSpriteFlags(Actor_Get(actorId), 0);
  particle->motionFlags = 0;
  particle->scaleX = 2 * FIXED_ONE / 5;
  particle->scaleY = 2 * FIXED_ONE / 5;
  {
    FIXED height = toFIXED(4);
    particle->x.fixed = x + height;
    height += y;
    particle->y.fixed = height;
    particle->targetY = height;
  }
  // It drifts east ever faster and rises ever slower, from half a pixel a
  // frame.
  particle->timer = 25;
  particle->userValue = SLEEP_Z_STEPS_PER_PIXEL / 2;
  Actor_SetScript(actorId, gColosseumNavampaMatch_SleepZScript);
}

/// After the last match, Garet, Ivan and Mia run to Isaac in the arena, but he
/// falls asleep.
static void ColosseumNavampaMatch_PlayPostBattleReunion(s32 battleResult) {
  const s32 *leaderScaleScript;
  TaskFunc sleepZTask;

  Actor_ReleaseById(ACTOR_PRIZE_A);
  Actor_ReleaseById(ACTOR_PRIZE_B);
  Party_RestoreHpPp(1);
  Sound_Play(SOUND_FADE_OUT_BGM);
  Event_Start();
  Actor_SetPosition(ACTOR_OPPONENT, 1544, 192);
  if (battleResult < 0) {
    Actor_SetAnimation(ACTOR_OPPONENT, WARRIOR_ANIMATION_ARMS_RAISED);
  } else {
    Actor_SetAnimation(ACTOR_OPPONENT, WARRIOR_ANIMATION_DEFEATED);
  }
  Actor_SetScript(ACTOR_OPPONENT, gColosseumNavampaMatch_OpponentAfterBattle);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 1504, 192);
  Actor_Get(ACTOR_PARTY_LEADER)->facing = FACING_EAST;
  Actor_SetScript(ACTOR_PARTY_LEADER, gColosseumNavampaMatch_LeaderActionA);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_CROUCH);
  Actor_SetSpeed(ACTOR_GARET, 1);
  Actor_SetSpeed(ACTOR_IVAN, 1);
  Actor_SetSpeed(ACTOR_MIA, 1);
  Actor_SetPosition(ACTOR_GARET, 1464, 184);
  Actor_SetPosition(ACTOR_IVAN, 1464, 200);
  Actor_SetPosition(ACTOR_MIA, 1448, 192);
  Actor_Get(ACTOR_GARET)->facing = FACING_EAST;
  Actor_Get(ACTOR_IVAN)->facing = FACING_EAST;
  Actor_Get(ACTOR_MIA)->facing = FACING_EAST;
  wait(1);
  FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
  gFieldState->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  PaletteFade_ApplyTarget(PALETTE_TINT_GRAYSCALE, PALETTES_BG);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_SetMessageId(MsgColossoIsaac);
  Event_Wait(60);
  leaderScaleScript = gColosseumNavampaMatch_LeaderScaleScript;
  Actor_SetScript(ACTOR_PARTY_LEADER, leaderScaleScript);
  Actor_Get(ACTOR_PARTY_LEADER)->scaleX = FIXED_ONE;
  Actor_Get(ACTOR_PARTY_LEADER)->scaleY = FIXED_ONE;
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_LEADER_FALL);
  Actor_Get(ACTOR_PARTY_LEADER)->x.fixed += toFIXED(3);
  Event_Wait(10);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 0);
  Event_Wait(20);
  Actor_SetScript(ACTOR_PARTY_LEADER, gColosseumNavampaMatch_LeaderActionB);
  // Let each companion react before the leader settles back to sleep.
  // "Isaac!!!"
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_Wait(20);
  Actor_WalkToAndWait(ACTOR_GARET, 1504, 176);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 10);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  // "Isaac..."
  Event_ShowMessage(ACTOR_IVAN, 0);
  Actor_WatchAndSetScript(ACTOR_GARET, ACTOR_IVAN);
  Event_Wait(30);
  Actor_WalkToAndWait(ACTOR_IVAN, 1488, 176);
  Actor_WalkTo(ACTOR_GARET, 1520, 184);
  Actor_WalkToAndWait(ACTOR_IVAN, 1504, 176);
  Actor_SetAnimation(ACTOR_GARET, ANIM_STAND);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTH, 10);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_Wait(10);
  Actor_WatchAndSetScript(ACTOR_IVAN, ACTOR_GARET);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
  Event_Wait(30);
  // "Isaac..."
  Event_ShowMessage(ACTOR_MIA, 0);
  Actor_WatchAndSetScript(ACTOR_GARET, ACTOR_MIA);
  Actor_WatchAndSetScript(ACTOR_IVAN, ACTOR_MIA);
  Actor_WalkToAndWait(ACTOR_MIA, 1488, 184);
  Actor_WatchAndSetScript(ACTOR_IVAN, ACTOR_PARTY_LEADER);
  Actor_WatchAndSetScript(ACTOR_GARET, ACTOR_PARTY_LEADER);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Actor_WatchAndSetScript(ACTOR_IVAN, ACTOR_GARET);
  Actor_WatchAndSetScript(ACTOR_GARET, ACTOR_IVAN);
  Event_Wait(40);
  Actor_RunRepeatedMotion(ACTOR_MIA, 2);
  Event_Wait(10);
  Actor_WatchAndSetScript(ACTOR_IVAN, ACTOR_MIA);
  Actor_WatchAndSetScript(ACTOR_GARET, ACTOR_MIA);
  Event_Wait(20);
  // "Isaac..."
  Event_ShowMessage(ACTOR_MIA, 0);
  Actor_SetScript(ACTOR_PARTY_LEADER, gColosseumNavampaMatch_LeaderActionC);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Actor_WatchAndSetScript(ACTOR_GARET, ACTOR_PARTY_LEADER);
  Event_Wait(20);
  // "Isaac..."
  Event_ShowMessage(ACTOR_GARET, 0);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Actor_WatchAndSetScript(ACTOR_IVAN, ACTOR_PARTY_LEADER);
  Event_Wait(20);
  // "Isaac..."
  Event_ShowMessage(ACTOR_IVAN, 0);
  Actor_RunRepeatedMotion(ACTOR_MIA, 2);
  Actor_WatchAndSetScript(ACTOR_MIA, ACTOR_PARTY_LEADER);
  Event_Wait(20);
  // "Isaac..."
  Event_ShowMessage(ACTOR_MIA, 0);
  Actor_SetScript(ACTOR_PARTY_LEADER, leaderScaleScript);
  Event_Wait(60);
  sleepZTask = ColosseumNavampaMatch_SpawnSleepZ;
  // A Z rises from Isaac on the task's first frame (frame 10 of the cycle).
  gColosseumNavampaMatch_SleepFrame = 9;
  Task_AddFrame(sleepZTask, TASK_PASS_FRAME);
  Event_Wait(5);
  Task_Remove(sleepZTask);
  Event_Wait(55);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Task_AddFrame(sleepZTask, TASK_PASS_FRAME);
  Event_Wait(20);
  Task_Remove(sleepZTask);
  Event_Wait(40);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  gColosseumNavampaMatch_SleepFrame = 9;
  Task_AddFrame(sleepZTask, TASK_PASS_FRAME);
  Event_Wait(35);
  Task_Remove(sleepZTask);
  Event_Wait(25);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  gColosseumNavampaMatch_SleepFrame = 9;
  Task_AddFrame(sleepZTask, TASK_PASS_FRAME);
  Event_Wait(35);
  Task_Remove(sleepZTask);
  Event_Wait(25);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Actor_WatchAndSetScript(ACTOR_MIA, ACTOR_IVAN);
  Actor_WatchAndSetScript(ACTOR_IVAN, ACTOR_MIA);
  Event_Wait(60);
  Actor_WatchAndSetScript(ACTOR_MIA, ACTOR_PARTY_LEADER);
  Actor_WatchAndSetScript(ACTOR_IVAN, ACTOR_PARTY_LEADER);
  gColosseumNavampaMatch_SleepFrame = 9;
  Task_AddFrame(sleepZTask, TASK_PASS_FRAME);
  Event_Wait(35);
  Task_Remove(sleepZTask);
  Event_Wait(25);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_SMILE, 60);
  Actor_StartRepeatedMotion(ACTOR_GARET, 3);
  Actor_StartRepeatedMotion(ACTOR_IVAN, 3);
  Actor_RunRepeatedMotion(ACTOR_MIA, 3);
  Actor_WatchAndSetScript(ACTOR_MIA, ACTOR_IVAN);
  Actor_WatchAndSetScript(ACTOR_GARET, ACTOR_IVAN);
  gColosseumNavampaMatch_SleepFrame = 9;
  Task_AddFrame(sleepZTask, TASK_PASS_FRAME);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_NOD);
  Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
  Event_Wait(60);
  // The companions begin to leave, then discuss the sleeping leader off-screen.
  Actor_WalkTo(ACTOR_MIA, 1464, 200);
  Event_Wait(5);
  Actor_WalkTo(ACTOR_IVAN, 1368, 184);
  Event_Wait(3);
  Actor_WalkToAndWait(ACTOR_GARET, 1512, 184);
  Actor_WalkTo(ACTOR_GARET, 1368, 184);
  Actor_WaitForMoveById(ACTOR_MIA);
  Actor_SetAnimation(ACTOR_MIA, ANIM_STAND);
  Actor_WatchAndSetScript(ACTOR_MIA, ACTOR_PARTY_LEADER);
  Event_Wait(60);
  Actor_WalkToAndWait(ACTOR_MIA, 1432, 200);
  Actor_WalkTo(ACTOR_MIA, 1368, 184);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(30);
  Actor_SetPosition(ACTOR_GARET, 1512, 176);
  Actor_SetPosition(ACTOR_IVAN, 1464, 192);
  Actor_SetPosition(ACTOR_MIA, 1560, 200);
  ActorSystem_SuspendProjectionAndShowSprites();
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_OBJECTS);
  PaletteFade_StartInterpolation(1);
  Event_SetMessageId(MsgColossoIsaacFellAsleep);
  // "Isaac fell asleep..."
  Event_ShowMessage(ACTOR_GARET, 0);
  // "He must be exhausted."
  Event_ShowMessage(ACTOR_IVAN, 0);
  // "Let's leave him to sleep for now."
  Event_ShowMessage(ACTOR_MIA, 0);
  Event_Wait(60);
  Event_End();
}

/// The guide gives the party the tour of Colosseum Final 3
/// (COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_NORTH).
static void ColosseumNavampaMatch_IntroduceThirdFinals(s32 guideId) {
  s32 guideZ;
  s32 guideX;
  struct Actor *actor;

  actor = Actor_Get(guideId);
  guideX = actor->x.part[0].integer;
  guideZ = actor->z.part[0].integer;
  Event_Start();
  Actor_SetSpeed(guideId, 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 1);
  Actor_SetSpeed(ACTOR_GARET, 1);
  Actor_SetSpeed(ACTOR_IVAN, 1);
  Actor_SetSpeed(ACTOR_MIA, 1);
  Actor_SetPositionFX(ACTOR_PARTY_LEADER, toFIXED(guideX),
                      (toFIXED(guideZ) - toFIXED(48)));
  Actor_SetPositionFX(ACTOR_GARET, toFIXED(guideX) - toFIXED(16),
                      toFIXED(guideZ) - toFIXED(40));
  Actor_SetPositionFX(ACTOR_IVAN, toFIXED(guideX) + toFIXED(16),
                      toFIXED(guideZ) - toFIXED(40));
  Actor_SetPositionFX(ACTOR_MIA, toFIXED(guideX),
                      (toFIXED(guideZ) - toFIXED(32)));
  Actor_SetPositionFX(guideId, toFIXED(guideX),
                      (toFIXED(guideZ) - toFIXED(80)));
  Actor_Get(ACTOR_PARTY_LEADER)->facing = FACING_NORTH;
  FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_SetMessageId(MsgColossoSiteThirdFinals);
  Actor_SetAnimationAndWait(guideId, ANIM_NOD);
  // "This is the site of the third finals match."
  Event_ShowMessage(guideId, 0);
  Actor_RunRepeatedMotion(guideId, 2);
  // "By now, you know that each stage has its own attendant."
  Event_ShowMessage(guideId, 0);
  Actor_RunRepeatedMotion(guideId, 2);
  // "Ask the attendants descriptions of the stages."
  Event_ShowMessage(guideId, 0);
  Actor_RunRepeatedMotion(guideId, 2);
  // "Honestly, Isaac, I didn't think you'd make it this far! I'll..."
  Event_ShowMessage(guideId, 0);
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
  Actor_WalkToAndWait(guideId, guideX - 16, guideZ - 64);
  Actor_SetPosition(ACTOR_GARET, 0, 0);
  Actor_SetPosition(ACTOR_IVAN, 0, 0);
  Actor_SetPosition(ACTOR_MIA, 0, 0);
  Actor_WalkToAndWait(guideId, guideX - 16, guideZ - 16);
  Actor_WalkToAndWait(guideId, guideX, guideZ);
  Actor_FaceDirection(guideId, FACING_NORTH, 10);
  Event_End();
}

/// The Scales of Justice's attendant explains stage 1 of Colosseum Final 3.
static void ColosseumNavampaMatch_TalkToScalesAttendant(s32 guideId) {
  struct Actor *westPan;
  struct Actor *eastPan;
  struct Actor *westPanAgain;
  struct Actor *eastPanAgain;
  s32 promptResult;

  if (gMainRoutineState.entrance ==
      COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_SOUTH) {
    Colosso_AskDoneCheering(guideId);
    return;
  }
  Event_Start();
  promptResult = Colosso_GreetAtStage(guideId, STAGE_SCALES_OF_JUSTICE);
  if (promptResult == COLOSSO_GUIDE_DESCRIBE) {
    Event_SetMessageId(MsgColossoStageCalledScales);
    FieldCamera_SetSpeed(3);
    FieldCamera_MoveTo(152, CAMERA_KEEP, 200, 1);
    FieldCamera_WaitForMove();
    Event_Wait(30);
    // "This stage is called the Scales of Justice."
    Event_ShowMessage(guideId, 0);
    Colosso_ShowPointer(104, 68, 0);
    Event_Wait(60);
    Colosso_MovePointer(168, 96, 10);
    Event_Wait(70);
    // "There are two ledges that actually form a scale."
    Event_ShowMessage(guideId, 0);
    Colosso_HidePointer();
    wait(2);
    westPan = Actor_Get(ACTOR_SCALES_WEST_LEDGE);
    westPan->motionFlags = 0;
    ColosseumNavampaMatch_MoveOnScales(westPan, 4);
    eastPan = Actor_Get(ACTOR_SCALES_EAST_LEDGE);
    eastPan->motionFlags = 0;
    ColosseumNavampaMatch_MoveOnScales(eastPan, 32);
    Actor_WaitForMove(eastPan);
    Event_Wait(45);
    westPanAgain = Actor_Get(ACTOR_SCALES_WEST_LEDGE);
    westPanAgain->motionFlags = 0;
    ColosseumNavampaMatch_MoveOnScales(westPanAgain, 32);
    eastPanAgain = Actor_Get(ACTOR_SCALES_EAST_LEDGE);
    eastPanAgain->motionFlags = 0;
    ColosseumNavampaMatch_MoveOnScales(eastPanAgain, 4);
    Actor_WaitForMove(eastPanAgain);
    Event_Wait(15);
    // "When one ledge rises, the other falls."
    Event_ShowMessage(guideId, 0);
    Colosso_ShowPointer(104, 68, 0);
    Event_Wait(30);
    Colosso_MovePointer(168, 96, 10);
    Event_Wait(40);
    Colosso_MovePointer(104, 68, 10);
    Event_Wait(70);
    // "Use the scales to cross the gaps and race to the next stage."
    Event_ShowMessage(guideId, 0);
    Colosso_HidePointer();
    wait(2);
    FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
    Colosso_RepeatStageNumber(guideId, STAGE_SCALES_OF_JUSTICE);
  } else if (promptResult == COLOSSO_GUIDE_OFFER_CHEERING) {
    Event_SetMessageId(MsgColossoObjectiveGetAcross);
    // "Here your objective is to get across the gaps as quickly as..."
    Event_ShowMessage(guideId, 0);
  }
  Colosso_OfferCheeringSpot(promptResult, guideId, STAGE_SCALES_OF_JUSTICE);
  Event_End();
}

/// The Stepping-Stone Stage's attendant explains stage 2 of Colosseum Final 3.
static void ColosseumNavampaMatch_TalkToSteppingStoneAttendant(s32 guideId) {
  s32 promptResult;

  if (gMainRoutineState.entrance ==
      COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_SOUTH) {
    Colosso_AskDoneCheering(guideId);
    return;
  }
  Event_Start();
  promptResult = Colosso_GreetAtStage(guideId, STAGE_STEPPING_STONE);
  if (promptResult == COLOSSO_GUIDE_DESCRIBE) {
    Event_SetMessageId(MsgColossoSteppingStoneStage);
    FieldCamera_SetSpeed(3);
    FieldCamera_MoveTo(376, CAMERA_KEEP, 152, 1);
    FieldCamera_WaitForMove();
    Event_Wait(30);
    // "This is the Stepping- Stone Stage."
    Event_ShowMessage(guideId, 0);
    Colosso_FadeInWarrior(ACTOR_PARTY_LEADER, 280, 200);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 1.5);
    Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 280, 152);
    Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 296, 152);
    Event_Wait(10);
    Field_TryJumpForward();
    FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 15);
    Field_TryJumpForward();
    FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 15);
    Field_TryJumpForward();
    FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 15);
    // "You can reach the next stage by jumping from stone to stone."
    Event_ShowMessage(guideId, 0);
    Colosso_ShowPointer(96, 40, 0);
    Colosso_MovePointer(128, 40, 10);
    Event_Wait(30);
    Colosso_MovePointer(160, 40, 10);
    Event_Wait(30);
    Colosso_MovePointer(160, 72, 10);
    Event_Wait(30);
    // "Study the placement of the stones, then choose the shortest path."
    Event_ShowMessage(guideId, 0);
    Colosso_HidePointer();
    Colosso_RestoreWarrior(ACTOR_PARTY_LEADER);
    FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
    Colosso_RepeatStageNumber(guideId, STAGE_STEPPING_STONE);
  } else if (promptResult == COLOSSO_GUIDE_OFFER_CHEERING) {
    Event_SetMessageId(MsgColossoYourGoalInStageSimple);
    // "Your goal in this stage is simple: find the shortest path."
    Event_ShowMessage(guideId, 0);
  }
  Colosso_OfferCheeringSpot(promptResult, guideId, STAGE_STEPPING_STONE);
  Event_End();
}

/// The Wall Wedge's attendant explains stage 3 of Colosseum Final 3.
static void ColosseumNavampaMatch_TalkToWallWedgeAttendant(s32 guideId) {
  struct FieldState *state;
  s32 promptResult;

  if (gMainRoutineState.entrance ==
      COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_SOUTH) {
    Colosso_AskDoneCheering(guideId);
  } else {
    Event_Start();
    promptResult = Colosso_GreetAtStage(guideId, STAGE_WALL_WEDGE);
    if (promptResult == COLOSSO_GUIDE_DESCRIBE) {
      state = gFieldState;
      Event_SetMessageId(MsgColossoPlaceCalledWall);
      ColosseumNavampaMatch_ResetWallWedge();
      FieldCamera_SetSpeed(3);
      FieldCamera_MoveTo(616, CAMERA_KEEP, 184, 1);
      FieldCamera_WaitForMove();
      Event_Wait(30);
      // "This place is called the Wall Wedge."
      Event_ShowMessage(guideId, 0);
      ColosseumNavampaMatch_StartWallWedge();
      Event_Wait(60);
      // "The pistons move back and forth. Avoid them to reach the next stage."
      Event_ShowMessage(guideId, 0);
      Colosso_FadeInWarrior(ACTOR_PARTY_LEADER, 504, 200);
      COLOSSEUM_NAVAMPA_MATCH_FACE_DIRECTION(ACTOR_PARTY_LEADER, 0, 0);
      ColosseumNavampaMatch_WaitForWallWedgeOpening();
      Actor_SetSpeed(ACTOR_PARTY_LEADER, 1.5);
      Colosso_WalkActorTo(ACTOR_PARTY_LEADER, 680, 200);
      for (promptResult = 0; promptResult < 4 * FRAMES_PER_SECOND &&
                             state->raisedTrigger != PISTON_ANIMATION_PUSH_OUT;
           promptResult++)
        wait(1);
      ColosseumNavampaMatch_ShovedOffWalkway();
      Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
      Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN,
                      60);
      // "If your timing is off, a piston might knock you off the ledge."
      Event_ShowMessage(guideId, 0);
      Colosso_RestoreWarrior(ACTOR_PARTY_LEADER);
      FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
      Colosso_RepeatStageNumber(guideId, STAGE_WALL_WEDGE);
      state->raisedTrigger = RAISED_TRIGGER_NONE;
    } else if (promptResult == COLOSSO_GUIDE_OFFER_CHEERING) {
      Event_SetMessageId(MsgColossoOperatorWallsCheer);
      // "The operator of the walls will cheer for you on this stage."
      Event_ShowMessage(guideId, 0);
    }
    Colosso_OfferCheeringSpot(promptResult, guideId, STAGE_WALL_WEDGE);
    Event_End();
  }
}

/// The Free-Climb Area's attendant explains stage 4 of Colosseum Final 3.
static void ColosseumNavampaMatch_TalkToFreeClimbAttendant(s32 guideId) {
  s32 result;
  struct Actor *actor;
  FIXED x;
  FIXED y;
  FIXED raisedHeight;
  FIXED shiftedX;

  if (gMainRoutineState.entrance ==
      COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_SOUTH) {
    Colosso_AskDoneCheering(guideId);
  } else {
    Event_Start();
    result = Colosso_GreetAtStage(guideId, STAGE_FREE_CLIMB);
    if (result == COLOSSO_GUIDE_DESCRIBE) {
      Event_SetMessageId(MsgColossoPlaceNormallyCalledFreeClimb);
      FieldCamera_SetSpeed(3);
      FieldCamera_MoveTo(856, CAMERA_KEEP, 168, 1);
      FieldCamera_WaitForMove();
      Event_Wait(30);
      // "This place is normally called the Free- Climb Area."
      Event_ShowMessage(guideId, 0);
      // "This sheer wall has a bumpy surface."
      Event_ShowMessage(guideId, 0);
      Colosso_FadeInWarrior(ACTOR_PARTY_LEADER, 816, 200);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, 1);
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 840, 200);
      Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
      Field_ClimbUp();
      FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.5);
      actor = Actor_Get(ACTOR_PARTY_LEADER);
      y = actor->y.fixed;
      x = actor->x.fixed;
      Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.5);
      Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_CLIMB);
      raisedHeight = toFIXED(6) + y;
      FieldActor_SetMoveTargetFX(actor, x, raisedHeight, actor->z.fixed);
      Actor_WaitForMove(actor);
      Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_CLIMB_EAST);
      shiftedX = toFIXED(64) + x;
      FieldActor_SetMoveTargetFX(actor, shiftedX, raisedHeight, actor->z.fixed);
      Actor_WaitForMove(actor);
      Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_CLIMB);
      FieldActor_SetMoveTargetFX(actor, shiftedX, y + toFIXED(54),
                                 actor->z.fixed);
      Actor_WaitForMove(actor);
      Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_CLIMB_WEST);
      FieldActor_SetMoveTargetFX(actor, x + toFIXED(48), y + toFIXED(54),
                                 actor->z.fixed);
      Actor_WaitForMove(actor);
      Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_CLING);
      // "Using those bumps, feel your way up the wall and climb to the..."
      Event_ShowMessage(guideId, 0);
      Colosso_RestoreWarrior(ACTOR_PARTY_LEADER);
      FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
      Colosso_RepeatStageNumber(guideId, STAGE_FREE_CLIMB);
    } else if (result == COLOSSO_GUIDE_OFFER_CHEERING) {
      Event_SetMessageId(MsgColossoClearStageMustAbleChange);
      // "To clear this stage, you must be able to change directions..."
      Event_ShowMessage(guideId, 0);
    }
    Colosso_OfferCheeringSpot(result, guideId, STAGE_FREE_CLIMB);
    Event_End();
  }
}

/// Sets an actor running toward (x, z) in pixels, at the height it stands at.
static void ColosseumNavampaMatch_RunActorTo(s32 selector, s32 x, s32 z) {
  struct Actor *actor;

  actor = Actor_Lookup(selector);
  if (actor != NULL) {
    Actor_StartIdle(actor);
    Actor_SelectAnimation(actor, ANIM_LEADER_RUN);
    Actor_SetMoveTarget(actor, toFIXED(x), actor->y.fixed, toFIXED(z));
  }
}

/// Runs an actor to (x, z) in pixels and stands it there once it arrives.
static void ColosseumNavampaMatch_RunActorToAndStand(s32 selector, s32 x,
                                                     s32 z) {
  struct Actor *actor;

  actor = Actor_Lookup(selector);
  if (actor != NULL) {
    Actor_StartIdle(actor);
    Actor_SelectAnimation(actor, ANIM_LEADER_RUN);
    Actor_SetMoveTarget(actor, toFIXED(x), actor->y.fixed, toFIXED(z));
    Actor_WaitForMove(actor);
    Actor_SelectAnimation(actor, ANIM_STAND);
  }
}

/// The Moving Sidewalk Stage's attendant explains stage 5 of Colosseum Final
/// 3.
static void ColosseumNavampaMatch_TalkToMovingSidewalkAttendant(s32 guideId) {
  s32 i;
  s32 result;
  struct Actor *actor;

  if (gMainRoutineState.entrance ==
      COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_SOUTH) {
    Colosso_AskDoneCheering(guideId);
  } else {
    Event_Start();
    result = Colosso_GreetAtStage(guideId, STAGE_MOVING_SIDEWALK);
    if (result == COLOSSO_GUIDE_DESCRIBE) {
      Event_SetMessageId(MsgColossoCalledMovingSidewalkStage);
      FieldCamera_SetSpeed(3);
      FieldCamera_MoveTo(1080, CAMERA_KEEP, 168, 1);
      FieldCamera_WaitForMove();
      Event_Wait(30);
      // "This is called the Moving Sidewalk Stage."
      Event_ShowMessage(guideId, 0);
      // "The conveyor belt moves opposite the direction you need to go."
      Event_ShowMessage(guideId, 0);
      Colosso_FadeInWarrior(ACTOR_PARTY_LEADER, 984, 184);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, 1.5);
      ColosseumNavampaMatch_RunActorToAndStand(ACTOR_PARTY_LEADER, 992, 184);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.3);
      ColosseumNavampaMatch_RunActorTo(ACTOR_PARTY_LEADER, 1120, 184);
      Event_Wait(120);
      Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT |
                                                      ATTACHED_EFFECT_ATTACH);
      Event_Wait(120);
      Actor_ResetMotionFields(ACTOR_PARTY_LEADER);
      Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
#if !TBS_PATCH_3
      Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT |
                                                      ATTACHED_EFFECT_REMOVE);
#endif
      Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
      actor = Actor_Get(ACTOR_PARTY_LEADER);
      for (i = 0; i < 120; i++) {
        if (actor->x.fixed > toFIXED(992)) {
          actor->x.fixed -= toFIXED(6) / 5;
        }
        wait(1);
      }
      Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN,
                      60);
      ColosseumNavampaMatch_RunActorTo(ACTOR_PARTY_LEADER, 1120, 184);
      // "Race against it to reach the next stage."
      Event_ShowMessage(guideId, 0);
      Actor_ResetMotionFields(ACTOR_PARTY_LEADER);
      gMainRoutineState.movementMode = LEADER_MOVEMENT_CLIMB;
      Colosso_RestoreWarrior(ACTOR_PARTY_LEADER);
      FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
      Colosso_RepeatStageNumber(guideId, STAGE_MOVING_SIDEWALK);
    } else if (result == COLOSSO_GUIDE_OFFER_CHEERING) {
      Event_SetMessageId(MsgColossoInStageMustTryOutpace);
      // "In this stage, you must try to outpace the conveyor belt."
      Event_ShowMessage(guideId, 0);
    }
    Colosso_OfferCheeringSpot(result, guideId, STAGE_MOVING_SIDEWALK);
    Event_End();
  }
}

/// The log-rolling area's attendant explains stage 6 of Colosseum Final 3.
static void ColosseumNavampaMatch_TalkToLogRollingAttendant(s32 actorId) {
  s32 state;

  if (gMainRoutineState.entrance ==
      COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_SOUTH) {
    Colosso_AskDoneCheering(actorId);
    return;
  }

  Event_Start();
  state = Colosso_GreetAtStage(actorId, STAGE_LOG_ROLLING);

  if (state == COLOSSO_GUIDE_DESCRIBE) {
    Event_SetMessageId(MsgColossoAnotherLogRollingArea);
    FieldCamera_SetSpeed(3);
    FieldCamera_MoveTo(1288, CAMERA_KEEP, 152, 1);
    FieldCamera_WaitForMove();
    Event_Wait(30);
    // "This is another log-rolling area."
    Event_ShowMessage(actorId, 0);
    Colosso_ShowPointer(180, 88, 0);
    Event_Wait(60);
    // "The obstacle in this stage is the water tank at the exit."
    Event_ShowMessage(actorId, 0);
    Colosso_MovePointer(32, 84, 10);
    Event_Wait(30);
    // "To get across the tank, you'll need to roll the logs."
    Event_ShowMessage(actorId, 0);
    Colosso_MovePointer(96, 84, 30);
    Event_Wait(60);
    // "You can get through quickly by moving the logs and stones."
    Event_ShowMessage(actorId, 0);
    Colosso_HidePointer();
    Event_Wait(2);
    FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
    Colosso_RepeatStageNumber(actorId, STAGE_LOG_ROLLING);
  } else if (state == COLOSSO_GUIDE_OFFER_CHEERING) {
    Event_SetMessageId(MsgColossoHereMustFigureOutHow);
    // "Here you must figure out how the logs and stones need to be..."
    Event_ShowMessage(actorId, 0);
  }

  Colosso_OfferCheeringSpot(state, actorId, STAGE_LOG_ROLLING);
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

/// The chest holding a Smoke Bomb has been opened.
#define FLAG_COLOSSEUM_NAVAMPA_MATCH_SMOKE_BOMB_CHEST_OPENED FLAG_AREA_TEMP_6A
/// The chest holding a Nut has been opened.
#define FLAG_COLOSSEUM_NAVAMPA_MATCH_NUT_CHEST_OPENED FLAG_AREA_TEMP_6B
/// The chest holding a Sleep Bomb has been opened.
#define FLAG_COLOSSEUM_NAVAMPA_MATCH_SLEEP_BOMB_CHEST_OPENED FLAG_AREA_TEMP_6C

/// The Moving Sidewalk Stage's log where Force has knocked it off the belt,
/// bobbing a pixel up and down.
const s32 gColosseumNavampaMatch_SidewalkLogOffBelt[] = {
    SC_SET(SW_Y, toFIXED(1)),    SC_WAIT(10), SC_SET(SW_Y, 0), SC_WAIT(10),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// The Wall Wedge's cycle of four steps.
///
/// Each holds each piston's animation, west to east, then the operator's.
const s8 gColosseumNavampaMatch_WallWedgeCycle[][6] = {
    {PISTON_ANIMATION_PULL_IN, PISTON_ANIMATION_PUSH_OUT,
     PISTON_ANIMATION_PULL_IN, PISTON_ANIMATION_PUSH_OUT,
     PISTON_ANIMATION_PULL_IN, WALL_OPERATOR_ANIMATION_ODD_PISTONS_IN},
    {PISTON_ANIMATION_PUSH_HALF_OUT, PISTON_ANIMATION_PULL_HALF_IN,
     PISTON_ANIMATION_PUSH_HALF_OUT, PISTON_ANIMATION_PULL_HALF_IN,
     PISTON_ANIMATION_PUSH_HALF_OUT, WALL_OPERATOR_ANIMATION_PISTONS_HALF_OUT},
    {PISTON_ANIMATION_PUSH_OUT, PISTON_ANIMATION_PULL_IN,
     PISTON_ANIMATION_PUSH_OUT, PISTON_ANIMATION_PULL_IN,
     PISTON_ANIMATION_PUSH_OUT, WALL_OPERATOR_ANIMATION_ODD_PISTONS_OUT},
    {PISTON_ANIMATION_PULL_HALF_IN, PISTON_ANIMATION_PUSH_HALF_OUT,
     PISTON_ANIMATION_PULL_HALF_IN, PISTON_ANIMATION_PUSH_HALF_OUT,
     PISTON_ANIMATION_PULL_HALF_IN, WALL_OPERATOR_ANIMATION_PISTONS_HALF_OUT},
};

// clang-format off
/// The way the long log rolls for each eighth of its rider's facing.
const u16 gColosseumNavampaMatch_FacingAngles[] = {
    0, 0, FACING_SOUTH, FACING_WEST, FACING_WEST, FACING_WEST, FACING_NORTH, 0,
};

const s32 gColosseumNavampaMatch_SidewalkLogScript[] = {
    SC_MOVE(1088, 0, 184),
    SC_SYNC,
    SC_MOVE(1130, 0, 184),
    SC_SYNC,
    SC_CALL(ColosseumNavampaMatch_ShowActivatedSidewalkLayer),
    SC_LABEL(1),
    SC_SET(SW_Y, toFIXED(1)),
    SC_WAIT(10),
    SC_SET(SW_Y, 0),
    SC_WAIT(10),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
};

#include "shared/colosso_lettering_tables.c"

char gHexDigits[20] = "0123456789ABCDEF";

#include "shared/colosso_prize_scripts.c"

#include "shared/pushed_actor_direction_steps.c"

struct SceneEntrance gColosseumNavampaMatchEntrances[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 88, 0, 104, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { COLOSSEUM_NAVAMPA_MATCH_RACE_FROM_LEND_A_HAND, CONDITION_ALWAYS, 88, 0, 104, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_SOUTH, CONDITION_ALWAYS, 136, 0, 392, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { COLOSSEUM_NAVAMPA_MATCH_AFTER_MATCH_WON, CONDITION_ALWAYS, 88, 0, 104, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { COLOSSEUM_NAVAMPA_MATCH_AFTER_MATCH_LOST, CONDITION_ALWAYS, 88, 0, 104, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

u32 gColosseumNavampaMatchExits[] = {
    SCENE_EXITS(ColosseumNavampaMatch),
    SCENE_EXIT(COLOSSEUM_NAVAMPA_MATCH_LEND_A_HAND_TO_RACE, ColosseumNavampaMatch, COLOSSEUM_NAVAMPA_MATCH_RACE_FROM_LEND_A_HAND),
    SCENE_EXIT(COLOSSEUM_NAVAMPA_MATCH_TO_COLOSSEUM, Colosseum, COLOSSEUM_FROM_COLOSSEUM_NAVAMPA_MATCH),
    SCENE_EXIT(COLOSSEUM_NAVAMPA_MATCH_TO_BABI_PALACE, BabiPalace, BABI_PALACE_FROM_COLOSSEUM_NAVAMPA_MATCH),
    SCENE_EXIT(COLOSSEUM_NAVAMPA_MATCH_TO_BABI_PALACE_LOST, BabiPalace, BABI_PALACE_FROM_COLOSSEUM_NAVAMPA_MATCH),
    SCENE_EXITS_END,
};

struct ActorPlacement gColosseumNavampaMatchActorPlacements[] = {
    ACTOR_PLACEMENT(ACTOR_PARTY_LEADER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        88, 0, 104, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        216, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        152, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        216, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        88, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        312, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        712, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        1240, 0, 136, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        1272, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        1304, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PISTON, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        536, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PISTON, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        568, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PISTON, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        600, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PISTON, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        632, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PISTON, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        664, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PISTON, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        536, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PISTON, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        568, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PISTON, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        600, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PISTON, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        632, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PISTON, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        664, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        720, 0, 216, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        792, 0, 194, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STANDING_LOG, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        1080, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        1208, 0, 176, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        136, 0, 408, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        152, 0, 312, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        376, 0, 312, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        600, 0, 312, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        856, 0, 312, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        1080, 0, 312, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        1304, 0, 312, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        1512, 0, 176, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        1512, 0, 208, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LETTER_Z, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_LETTER_Z, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_LETTER_Z, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    { SCENE_TABLE_END },
};

/// The Wall Wedge's step in its four-step piston cycle.
s32 gColosseumNavampaMatch_WallWedgeStep = 0;

/// The frames into the Wall Wedge's step, 0 to 17.
u32 gColosseumNavampaMatch_WallWedgeTimer = 0;

struct Trigger gColosseumNavampaMatchTriggers[] = {
    { STEP_TRIGGER_WALKING, CELL_BEHAVIOR_LOG_AREA_END, CONDITION_ALWAYS, EVENT(ColosseumNavampaMatch_HopFromLogAreaEnd) },
    { STEP_TRIGGER_WALKING, CELL_BEHAVIOR_WEIGHT_SQUARE_EDGE, CONDITION_ALWAYS, EVENT(ColosseumNavampaMatch_PushWeightOrHopAcross) },
    { TRIGGER_STEP, CELL_BEHAVIOR_MOVING_SIDEWALK, FLAG_SIDEWALK_LOG_KNOCKED_OFF, EVENT(ColosseumNavampaMatch_RideMovingSidewalk) },
    { STEP_TRIGGER_WALKING, CELL_BEHAVIOR_WEIGHT_SQUARE, CONDITION_ALWAYS, EVENT(ColosseumNavampaMatch_PushWeight) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SCALES_WEIGHT, CONDITION_ALWAYS, EVENT(ColosseumNavampaMatch_SetWeightOnScales) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STONE, CONDITION_ALWAYS, EVENT(ColosseumNavampaMatch_MoveStone) },
    { STEP_TRIGGER_WALKING, CELL_BEHAVIOR_ROCK_ROW, CONDITION_ALWAYS, EVENT(ColosseumNavampaMatch_PushRock) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ROCK, CONDITION_ALWAYS, EVENT(ColosseumNavampaMatch_JustMoveRock) },
    { STEP_TRIGGER_WALKING, CELL_BEHAVIOR_LONG_LOG, CONDITION_ALWAYS, EVENT(ColosseumNavampaMatch_RollLongLog) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LOG_AREA_STONE_A, FLAG_FIRST_LOG_AREA_STONE_SETTLED, EVENT(ColosseumNavampaMatch_UpdateLogAreaStoneCells) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LOG_AREA_STONE_B, FLAG_FIRST_LOG_AREA_STONE_SETTLED + 1, EVENT(ColosseumNavampaMatch_UpdateLogAreaStoneCells) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LOG_AREA_STONE_C, FLAG_FIRST_LOG_AREA_STONE_SETTLED + 2, EVENT(ColosseumNavampaMatch_UpdateLogAreaStoneCells) },
    { TRIGGER_STEP, CELL_BEHAVIOR_SCALES_LEDGE, FLAG_ON_SCALES, EVENT(ColosseumNavampaMatch_StepOnScales) },
    { TRIGGER_STEP, CELL_BEHAVIOR_OFF_SCALES, IF_FLAG_SET(FLAG_ON_SCALES), EVENT(ColosseumNavampaMatch_StepOffScales) },
    { TRIGGER_STEP, CELL_BEHAVIOR_ARENA_ENTRY, FLAG_COLOSSEUM_ARENA_CLOSED, EVENT(ColosseumNavampaMatch_ReachArena) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), CELL_BEHAVIOR_SMOKE_BOMB_CHEST, FLAG_COLOSSEUM_NAVAMPA_MATCH_SMOKE_BOMB_CHEST_OPENED, FIND_ITEM(ITEM_SMOKE_BOMB) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), CELL_BEHAVIOR_NUT_CHEST, FLAG_COLOSSEUM_NAVAMPA_MATCH_NUT_CHEST_OPENED, FIND_ITEM(ITEM_NUT) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST) | TRIGGER_FLAG_FACING | FACING_WEST, CELL_BEHAVIOR_SLEEP_BOMB_CHEST, FLAG_COLOSSEUM_NAVAMPA_MATCH_SLEEP_BOMB_CHEST_OPENED, FIND_ITEM(ITEM_SLEEP_BOMB) },
    { PSYNERGY_PHASE_ON_ACTOR(ABILITY_FROST, PSYNERGY_PHASE_ACTS), ACTOR_PUDDLE, FLAG_SCALES_FROZEN, EVENT(ColosseumNavampaMatch_FreezeOnScales) },
    { PSYNERGY_ON_ACTOR(ABILITY_GROWTH), ACTOR_GROWTH_PLANT, FLAG_PLANT_GROWN, EVENT(ColosseumNavampaMatch_GrowPlant) },
    { PSYNERGY_ON_ACTOR(ABILITY_HALT), ACTOR_WALL_OPERATOR, FLAG_WALL_WEDGE_HALTED, EVENT(ColosseumNavampaMatch_HaltWall) },
    { PSYNERGY_PHASE_ON_ACTOR(ABILITY_FORCE, PSYNERGY_PHASE_ACTS), ACTOR_SIDEWALK_LOG, FLAG_SIDEWALK_LOG_KNOCKED_OFF, EVENT(ColosseumNavampaMatch_ForceSidewalkLog) },
    { TRIGGER_RAISED, PISTON_ANIMATION_PUSH_HALF_OUT, CONDITION_ALWAYS, EVENT(ColosseumNavampaMatch_ShovedByPiston) },
    { TRIGGER_RAISED, PISTON_ANIMATION_PUSH_OUT, CONDITION_ALWAYS, EVENT(ColosseumNavampaMatch_ShovedOffWalkway) },
    { TRIGGER_TALK, ACTOR_GUIDE, CONDITION_ALWAYS, EVENT(Colosso_AskDoneLookingAround) },
    { TRIGGER_TALK, ACTOR_SCALES_ATTENDANT, CONDITION_ALWAYS, EVENT(ColosseumNavampaMatch_TalkToScalesAttendant) },
    { TRIGGER_TALK, ACTOR_STEPPING_STONE_ATTENDANT, CONDITION_ALWAYS, EVENT(ColosseumNavampaMatch_TalkToSteppingStoneAttendant) },
    { TRIGGER_TALK, ACTOR_WALL_WEDGE_ATTENDANT, CONDITION_ALWAYS, EVENT(ColosseumNavampaMatch_TalkToWallWedgeAttendant) },
    { TRIGGER_TALK, ACTOR_FREE_CLIMB_ATTENDANT, CONDITION_ALWAYS, EVENT(ColosseumNavampaMatch_TalkToFreeClimbAttendant) },
    { TRIGGER_TALK, ACTOR_MOVING_SIDEWALK_ATTENDANT, CONDITION_ALWAYS, EVENT(ColosseumNavampaMatch_TalkToMovingSidewalkAttendant) },
    { TRIGGER_TALK, ACTOR_LOG_ROLLING_ATTENDANT, CONDITION_ALWAYS, EVENT(ColosseumNavampaMatch_TalkToLogRollingAttendant) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GUIDE, CONDITION_ALWAYS, MSGNO(MsgColossoRiverThisYearsFinalsAreIncredibleThis) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SCALES_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoLogTheStreetVendorsCornOnThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STEPPING_STONE_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoLogICantSeeAnythingWhenIm) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WALL_WEDGE_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoLogItsNotVeryExcitingWorkBut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FREE_CLIMB_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoLogStandingForLongPeriodsOfTime) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MOVING_SIDEWALK_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoLogIWasUpAllNightTrying) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LOG_ROLLING_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoLogSomeTouristToldMeILook) },
    { TRIGGER_RAISED, COLOSSO_TAKE_PLACE_TRIGGER, CONDITION_ALWAYS,
      EVENT(Colosso_TakePlaceForMatch) },
    { TRIGGER_TABLE_END },
};

/// Navampa after the last battle
/// (ColosseumNavampaMatch_PlayPostBattleReunion).
///
/// His sprite grows taller and narrower and back, over and over.
s32 gColosseumNavampaMatch_OpponentAfterBattle[] = {
    SC_ADD(SW_SCALE_Y, 512),
    SC_ADD(SW_SCALE_X, -256),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, 512),
    SC_ADD(SW_SCALE_X, -256),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, 512),
    SC_ADD(SW_SCALE_X, -256),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_Y, -512),
    SC_ADD(SW_SCALE_X, 256),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_Y, -512),
    SC_ADD(SW_SCALE_X, 256),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_Y, -512),
    SC_ADD(SW_SCALE_X, 256),
    SC_WAIT(10),
    SC_LOOP(SC_FOREVER, SC_TOP),
    SC_END,
};

s32 gColosseumNavampaMatch_LeaderActionA[] = {
    SC_ADD(SW_SCALE_Y, 512),
    SC_ADD(SW_SCALE_X, -256),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, 512),
    SC_ADD(SW_SCALE_X, -256),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, 512),
    SC_ADD(SW_SCALE_X, -256),
    SC_WAIT(6),
    SC_ADD(SW_SCALE_Y, -512),
    SC_ADD(SW_SCALE_X, 256),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_Y, -512),
    SC_ADD(SW_SCALE_X, 256),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_Y, -512),
    SC_ADD(SW_SCALE_X, 256),
    SC_WAIT(12),
    SC_LOOP(SC_FOREVER, SC_TOP),
    SC_END,
};

s32 gColosseumNavampaMatch_LeaderActionB[] = {
    SC_SET(SW_SCALE_X, FIXED_ONE),
    SC_SET(SW_SCALE_Y, FIXED_ONE),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_Y, 3 * FIXED_ONE / 64),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, 3 * FIXED_ONE / 64),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, 3 * FIXED_ONE / 64),
    SC_WAIT(16),
    SC_ADD(SW_SCALE_Y, -(3 * FIXED_ONE / 64)),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_Y, -(3 * FIXED_ONE / 64)),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_Y, -(3 * FIXED_ONE / 64)),
    SC_WAIT(32),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};

s32 gColosseumNavampaMatch_LeaderActionC[] = {
    SC_SET(SW_SCALE_X, FIXED_ONE),
    SC_SET(SW_SCALE_Y, FIXED_ONE),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_Y, 3 * FIXED_ONE / 64),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, 3 * FIXED_ONE / 64),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, 3 * FIXED_ONE / 64),
    SC_WAIT(26),
    SC_ADD(SW_SCALE_Y, -(3 * FIXED_ONE / 64)),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_Y, -(3 * FIXED_ONE / 64)),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_Y, -(3 * FIXED_ONE / 64)),
    SC_WAIT(52),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};

s32 gColosseumNavampaMatch_LeaderScaleScript[] = {
    SC_SET(SW_SCALE_X, FIXED_ONE),
    SC_SET(SW_SCALE_Y, FIXED_ONE),
    SC_END,
};

s32 gColosseumNavampaMatch_SleepZScript[] = {
    SC_CALL(ColosseumNavampaMatch_AdvanceSleepZ),
    SC_WAIT(1),
    SC_LOOP(60, SC_TOP),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

s16 gColosso_LetteringSlot = -1;

#include "shared/colosso_lettering_scripts.c"
// clang-format on

s16 gColosso_PointerSlot = -1;

#include "shared/colosso_finish_scripts.c"

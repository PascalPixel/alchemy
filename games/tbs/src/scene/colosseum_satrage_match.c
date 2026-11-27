/* Colosseum Final 2 (SceneId_ColosseumSatrageMatch) is the site of the second
   finals match, where Isaac races Satrage through five stages to the arena and
   fights him there.

   Isaac has won the first match, and Satrage is the fourth-ranked warrior.

   It works as Colosseum Final 1 does (colosseum_azart_match.c). The stages run
   from east to west, one after another: the Mini-Maze, where Isaac starts among
   three stone pillars; the shifting floor stage, four lifts across a channel of
   water; the Log-Rolling Stage, two logs to ride across a pool; The Wall, a
   high stone wall to climb, with a row of stone posts at its foot; and the
   Board Walk, a pool whose logs lie across the way, with a channel of white
   platforms along its south edge. The arena is in the far west.

   Isaac comes here three times, each at its own entrance. scInit restores what
   the friends changed on the stages from their temporary flags
   (FLAG_LOG_KNOCKED_DOWN to FLAG_WALL_PILLAR_MOVED, and the Mini-Maze pillars'
   columns from FLAG_MINI_MAZE_FIRST_PILLAR_COLUMN). Then it sets up the visit
   its entrance stands for.

   - COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_WEST, the tour
	 (scColosseumSatrageMatchIntroduceSecondFinals). The guide only greets the
	 party, since the first finals match's guide explained the rules. The five
	 stages' attendants describe their stages and offer a place for each
	 friend to cheer. The guide then sends Isaac back into the Colosseum
	 (COLOSSEUM_SATRAGE_MATCH_TO_COLOSSEUM_SOUTH).
   - COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_EAST, `Lend a Hand`. Garet, Ivan
	 and Mia in turn may use Psynergy on the stages. Move shifts the
	 Mini-Maze's stone pillars and the stone pillar at the foot of The Wall.
	 Halt stops the lifts' operator and with him the shifting floor. Frost
	 freezes the Log-Rolling Stage's two puddles into pillars of ice. Growth
	 grows the sprout at the foot of The Wall into a vine. Force knocks the
	 tall log by the Board Walk's channel over into the water. After Mia,
	 COLOSSEUM_SATRAGE_MATCH_LEND_A_HAND_TO_RACE starts the race.
   - COLOSSEUM_SATRAGE_MATCH_RACE_FROM_LEND_A_HAND, the race against Satrage,
	 who follows a recorded path, after the lettering `The 2nd Match` and a
	 count of three. At the arena the first to arrive takes the Chain Mail and
	 the second the Cotton Shirt. The battle is ENCOUNTER_ZONE_COLOSSO's third
	 or fourth group, the Satrage with or without the mail.

   A win returns to COLOSSEUM_SATRAGE_MATCH_AFTER_MATCH_WON and on to the
   Colosseum (COLOSSEUM_SATRAGE_MATCH_TO_COLOSSEUM_NORTH, arriving at
   COLOSSEUM_FROM_COLOSSEUM_SATRAGE_MATCH_NORTH), which sets
   FLAG_COLOSSO_SECOND_MATCH_WON.

   A loss returns to COLOSSEUM_SATRAGE_MATCH_AFTER_MATCH_LOST, and
   COLOSSEUM_SATRAGE_MATCH_TO_BABI_PALACE wakes the party in Babi's Palace,
   where Isaac's friends tell him he was only dreaming of the finals.

   The shifting floor's four lifts run on every visit until Halt stops them
   (scColosseumSatrageMatchUpdateLifts). Every
   COLOSSEUM_SATRAGE_MATCH_LIFT_CYCLE_FRAMES they change places: for a while all
   four stand in a line across the channel, metatiles (58, 13) to (64, 13), and
   Isaac can hop from one to the next; then the first and third slide north and
   the second and fourth south, and the line is broken. Only while they stand
   still in a line are their metatiles squares to hop onto
   (METATILE_BEHAVIOR_JUMP_FORWARD); otherwise they are walls. */

#include "ability_ids.h"
#include "item_ids.h"
#include "macros.h"
#include "resource_ids.h"
#include "debug/debug.h"
#include "field/actor_script.h"
#include "field/script_inline.h"
#include "field/field_psynergy_state.h"
#include "field/pushed_actor.h"
#include "graphics/graphics.h"
#include "scene/colosso_gauge.h"
#include "scene/colosso_guide.h"
#include "scene/colosso_lettering.h"
#include "scene/colosso_setup.h"
#include "scene/colosso_solo.h"
#include "sound/sound_driver_symbols.h"
#include "text/msg_ids.h"

/* The match held here, as the lettering `The 2nd Match` reads; the friends'
   turns open with `Lend a Hand!` instead (scColossoShowOpeningLettering). */
enum { FINALS_MATCH = 2 };

/* The stages, as their attendants number them: "This is stage 1 of the second
   finals". */
typedef enum COLOSSEUM_SATRAGE_MATCH_STAGE {
	STAGE_MINI_MAZE = 1,
	STAGE_SHIFTING_FLOOR = 2,
	STAGE_LOG_ROLLING = 3,
	STAGE_WALL = 4,
	STAGE_BOARD_WALK = 5
} COLOSSEUM_SATRAGE_MATCH_STAGE;

/* Where scInit keeps a copy of the metatiles under the logs, off the stages, to
   put back before the logs' metatiles are walled again where they lie: the
   Board Walk's (14..25, 11..14) at column 100, and the Log-Rolling Stage's
   (48..52, 10..15) at column 120, each in the same rows. */
enum { BOARD_WALK_STORE_COLUMN = 100, LOG_ROLLING_STORE_COLUMN = 120 };

/* SPRITE_WHITE_PLATFORM's flat slab at the waterline (white_platform.tsv's
   picture 1), as the lifts and the Board Walk's platforms are drawn. */
#define PLATFORM_ANIMATION_AT_WATERLINE 4

/* The shared Colosso code's state, in the order the scene's memory lays it out;
   the shared files that use it say what each holds: the guide's pointer
   (colosso_guide.c), the lettering (colosso_task.c) and where Isaac stood
   before he was shown on a stage (colosso_guide.c). */
Sint16 Colosso_PointerStep					   BSS_DATA;
Sint16 Colosso_LetteringScaleDuration BSS_DATA ALIGNED(4);
Sint16 Colosso_PointerFlip BSS_DATA			   ALIGNED(4);
/* Isaac's z before he is shown on the stage, 16.16 fixed-point pixels. */
FIXED Colosso_WarriorStartZ					  BSS_DATA;
Sint16 Colosso_PointerEndX					  BSS_DATA;
Sint16 Colosso_LetteringParameter BSS_DATA	  ALIGNED(4);
Sint16 Colosso_LetteringScaleStart BSS_DATA	  ALIGNED(4);
Sint16 Colosso_LetteringMoveDuration BSS_DATA ALIGNED(4);
Sint32 Colosso_LetteringPosition			  BSS_DATA;
Sint16 Colosso_PointerBlink					  BSS_DATA;
Sint16 Colosso_LetteringScaleTarget BSS_DATA  ALIGNED(4);
Sint16 Colosso_LetteringMoveStep BSS_DATA	  ALIGNED(4);
Sint16 Colosso_PointerY BSS_DATA			  ALIGNED(4);
Sint16 Colosso_LetteringFadeStep BSS_DATA	  ALIGNED(4);
/* Isaac's facing before scColossoFadeInWarrior, which scColossoRestoreWarrior
   puts back. */
Sint32 Colosso_WarriorStartAngle			  BSS_DATA;
Sint16 Colosso_PointerSteps					  BSS_DATA;
Sint16 Colosso_LetteringMode BSS_DATA		  ALIGNED(4);
Sint16 Colosso_LetteringFadeTarget BSS_DATA	  ALIGNED(4);
Sint16 Colosso_LetteringFadeStart BSS_DATA	  ALIGNED(4);
Sint16 Colosso_LetteringTimer BSS_DATA		  ALIGNED(4);
Sint16 *Colosso_LetteringScript				  BSS_DATA;
POINTER_COORDINATE Colosso_PointerStartX	  BSS_DATA;
Sint16 Colosso_LetteringFadeDuration BSS_DATA ALIGNED(4);
Uint8 ColosseumSatrageMatch_UnusedAfterBlendDuration[6] BSS_DATA;
/* The blinking pointer's sprite part: the queue link and attributes 0 to 2. */
Sint32									 Colosso_PointerOam[3] BSS_DATA;
POINTER_COORDINATE Colosso_PointerStartY BSS_DATA;
/* The lettering's sprite parts, three words each, written every frame. */
Uint32							  Colosso_LetteringSprites[12] BSS_DATA;
Sint16 Colosso_LetteringMoveStart BSS_DATA;
Sint16 Colosso_PointerX BSS_DATA  ALIGNED(4);
Sint16 Colosso_LetteringMoveTarget BSS_DATA ALIGNED(4);
Sint16 Colosso_LetteringScaleStep BSS_DATA	ALIGNED(4);
Sint16 Colosso_PointerEndY BSS_DATA			ALIGNED(4);
/* Isaac's x before he is shown on the stage, 16.16 fixed-point pixels. */
FIXED Colosso_WarriorStartX BSS_DATA;

Sint32 scInit(void);
/* UB: the scene calls this before its definition with no prototype, as a
   function returning int. */
#if FIX_FUNCTION_SIGNATURES
void scColosseumSatrageMatchStartRaceGauge(Sint32 leaderId, Sint32 opponentId,
	Sint32 segments, Sint32 finishX, FIXED finishZ, Sint32 firstItem,
	Sint32 secondItem);
#endif
extern const Uint8	  ColosseumSatrageMatch_RollLogScript[];
extern SCENE_ENTRANCE ColosseumSatrageMatchEntrances[];
extern Uint32		  ColosseumSatrageMatchExits[];
extern ACTOR_PLACE	  ColosseumSatrageMatchActorPlacements[];
extern Sint32		  ColosseumSatrageMatch_LiftsApart;
extern Uint32		  ColosseumSatrageMatch_LiftTimer;
extern CUE			  ColosseumSatrageMatchCues[];
extern char			  HexDigits[20];

/* The lifts' cycle in frames, the frame of it the demonstration waits for them
   to stand lined up, and how long it waits at most; and how far a riding log
   carries Isaac, in pixels. */
#define COLOSSEUM_SATRAGE_MATCH_LIFT_CYCLE_FRAMES			 120
#define COLOSSEUM_SATRAGE_MATCH_LIFT_CROSSING_FRAME			 75
#define COLOSSEUM_SATRAGE_MATCH_DEMONSTRATION_TIMEOUT_FRAMES 600
#define COLOSSEUM_SATRAGE_MATCH_LOG_TRAVEL_PIXELS			 64
/* The map grid rows the two riding logs lie across, from their first row. */
#define RIDING_LOG_NORTH_ROW								 10
#define RIDING_LOG_SOUTH_ROW								 13
/* The falling log's spin, in 1/65536 turns a frame: it speeds up by
   FALL_SPIN_ACCELERATION each frame, from 0 as it tips and from
   SETTLE_SPIN_START as it drops into the water. */
#define FALL_SPIN_ACCELERATION								 36
#define SETTLE_SPIN_START									 360
/* The column the stone pillar by The Wall stands in once pushed, a metatile
   east of its place (568, 264). */
#define WALL_PILLAR_MOVED_COLUMN							 36

/* A log offset, -64, 0 or 64, in fixed point. */
#define COLOSSEUM_SATRAGE_MATCH_OFFSET_FIXED(value) toFIXED(value)

/* UB: each of these calls a void function through an Sint32 function type, the
   call the bytes show; UBFIX calls it as declared. */
#if FIX_FUNCTION_SIGNATURES
#define COLOSSEUM_SATRAGE_MATCH_SHOW_MESSAGE fdScriptShowMessageAndWait
#define COLOSSEUM_SATRAGE_MATCH_MOVE_RIDING_LOG \
	scColosseumSatrageMatchMoveRidingLog
#else
#define COLOSSEUM_SATRAGE_MATCH_SHOW_MESSAGE \
	((Sint32 (*)())fdScriptShowMessageAndWait)
#define COLOSSEUM_SATRAGE_MATCH_MOVE_RIDING_LOG \
	((Sint32 (*)())scColosseumSatrageMatchMoveRidingLog)
#endif

static void	  scColosseumSatrageMatchIntroduceSecondFinals(Sint32 attendantId);
static void	  scColossoShowOpeningLettering(Sint32 match);
static void	  scColossoShowMatchResult(Sint32 result);
static void	  scColossoNoOperation(void);
static void	  scColossoWaitForOpponentToFinish(void);
static Sint32 scColossoTakePrize(Sint32 firstItem, Sint32 secondItem);
static void	  scColosseumSatrageMatchStartOpponentPath(Sint32 pathResource);
static void	  scColossoPushActorAhead(void);

/* The second finals match's actors, numbered in the order of
   ColosseumSatrageMatchActorPlacements from ACTOR_FIRST_PLACED.

   The rows of Isaac, Garet, Ivan and Mia take their own actors.

   By the guide's and attendants' explanations, the Mini-Maze, the shifting
   floor stage, the Log-Rolling Stage, The Wall and the Board Walk lead to the
   arena. The stone pillars are drawn as the stone pillars Move pushes
   elsewhere. */
typedef enum COLOSSEUM_SATRAGE_MATCH_ACTOR_ID {
	/* Satrage, racing Isaac to the arena. */
	ACTOR_OPPONENT = ACTOR_FIRST_PLACED,
	/* The Board Walk's two stone pillars, on the ledge along the north edge of
	   its pool. The eastern one stands before the narrow channel that holds a
	   chest. Move does nothing to them. */
	ACTOR_BOARD_WALK_WEST_PILLAR,
	ACTOR_BOARD_WALK_EAST_PILLAR,
	/* The stone pillar on the ledge at the foot of The Wall, beside the chest
	   holding a Smoke Bomb. */
	ACTOR_WALL_PILLAR,
	/* The Mini-Maze's three stone pillars, in a row along the stage's south
	   edge where the race starts, which Move shifts. scInit stands them west to
	   east at columns 73, 76 and 79 until they have been moved. */
	ACTOR_MINI_MAZE_FIRST_PILLAR,
	/* The Board Walk's two logs, the western and the eastern, lying north to
	   south in its pool below the stone pillars' ledge. A third,
	   ACTOR_ROLLING_LOG_CENTER, is placed at (0, 0) and never moved there.
	   scColosseumSatrageMatchUnusedRollCenterLogNorth, its South twin and its
	   row in the collision rebuild remain for it. */
	ACTOR_ROLLING_LOG_WEST = ACTOR_MINI_MAZE_FIRST_PILLAR + 3,
	ACTOR_ROLLING_LOG_EAST,
	ACTOR_ROLLING_LOG_CENTER,
	/* What Force is cast at to knock ACTOR_FALLING_LOG over: the tall log's
	   place, at the east end of the Board Walk's channel. */
	ACTOR_FORCE_TARGET,
	/* The sprout at the foot of The Wall, which Growth grows into a vine. */
	ACTOR_GROWTH_PLANT,
	/* The Log-Rolling Stage's two puddles, which Frost freezes. They are drawn
	   as the puddle Frost freezes in Lunpa (lunpa.h's SPRITE_PUDDLE). */
	ACTOR_PUDDLE_WEST,
	ACTOR_PUDDLE_EAST,
	/* The shifting floor stage's four lifts, from west to east, which the
	   lifts' operator runs. */
	ACTOR_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT,
	ACTOR_COLOSSEUM_SATRAGE_MATCH_LAST_LIFT =
		ACTOR_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT + 3,
	/* Four platforms in the lifts' sprite laid across the water channel at the
	   Board Walk's south edge, drawn underfoot. Nothing but scInit touches
	   them. */
	ACTOR_BOARD_WALK_FIRST_PLATFORM,
	/* The tall log standing on end at the east end of the Board Walk's channel,
	   which Force on ACTOR_FORCE_TARGET knocks over into the water. */
	ACTOR_FALLING_LOG = ACTOR_BOARD_WALK_FIRST_PLATFORM + 4,
	/* A young man with fair hair who works the shifting floor stage's lifts. */
	ACTOR_LIFT_OPERATOR,
	/* The Log-Rolling Stage's two logs. */
	ACTOR_RIDING_LOG_NORTH,
	ACTOR_RIDING_LOG_SOUTH,
	/* A soldier who greets the party at the second finals match. */
	ACTOR_GUIDE,
	/* The stages' attendants, young men with fair hair in white tunics. */
	ACTOR_MINI_MAZE_ATTENDANT,
	ACTOR_SHIFTING_FLOOR_ATTENDANT,
	ACTOR_LOG_ROLLING_ATTENDANT,
	ACTOR_WALL_ATTENDANT,
	ACTOR_BOARD_WALK_ATTENDANT,
	/* The arena's prizes, drawn as item icons at two blank actors
	   (scColossoShowItemIcon). */
	ACTOR_PRIZE_A,
	ACTOR_PRIZE_B
} COLOSSEUM_SATRAGE_MATCH_ACTOR_ID;

/* The temporary flags that keep what the friends changed on the stages, for the
   race.

   scInit restores them. */
enum {
	FLAG_LOG_KNOCKED_DOWN = FLAG_AREA_TEMP_30, /* by Force, on the Board Walk */
	FLAG_WEST_PUDDLE_FROZEN = FLAG_AREA_TEMP_31, /* by Frost */
	FLAG_EAST_PUDDLE_FROZEN = FLAG_AREA_TEMP_32, /* by Frost */
	FLAG_PLANT_GROWN = FLAG_AREA_TEMP_33,		 /* by Growth, up The Wall */
	FLAG_LIFTS_HALTED = FLAG_AREA_TEMP_34, /* by Halt on ACTOR_LIFT_OPERATOR */
	/* The stone pillar at the foot of The Wall stands a metatile east of the
	   chest. */
	FLAG_WALL_PILLAR_MOVED = FLAG_AREA_TEMP_35,
	/* The bytes of flags that keep the Mini-Maze's three pillars' columns. */
	FLAG_MINI_MAZE_FIRST_PILLAR_COLUMN = FLAG_AREA_TEMP_40,
	FLAG_MINI_MAZE_SECOND_PILLAR_COLUMN = FLAG_AREA_TEMP_48,
	FLAG_MINI_MAZE_THIRD_PILLAR_COLUMN = FLAG_AREA_TEMP_50
};

/* SPRITE_FALLING_LOG's animations (falling_log.tsv). */
typedef enum FALLING_LOG_ANIMATION {
	FALLING_LOG_ANIMATION_STANDING = 3, /* standing on end */
	/* Lying across the water, bobbing. */
	FALLING_LOG_ANIMATION_FALLEN = 4
} FALLING_LOG_ANIMATION;

/* SPRITE_YOUNG_MAN_WITH_FAIR_HAIR's two poses at the lever of the shifting
   floor, which the lifts' operator takes as he lines the lifts up and as he
   breaks the line. */
typedef enum LIFT_OPERATOR_ANIMATION {
	LIFT_OPERATOR_ANIMATION_LIFTS_LINED_UP = 10,
	LIFT_OPERATOR_ANIMATION_LIFTS_APART = 11
} LIFT_OPERATOR_ANIMATION;

/* The metatile behaviours the stages' step cues answer. */
typedef enum COLOSSEUM_SATRAGE_MATCH_METATILE_BEHAVIOR {
	/* The ledge at the foot of The Wall, metatile (37, 16), a metatile east of
	   where Move leaves the stone pillar. */
	METATILE_BEHAVIOR_WALL_LEDGE = 20,
	/* The Board Walk's ledge either side of its two stone pillars, metatiles
	   (13, 10) to (17, 10) and (21, 10) to (24, 10). */
	METATILE_BEHAVIOR_BOARD_WALK_LEDGE = 21,
	/* Under the Log-Rolling Stage's logs, three metatiles each, moved with
	   them. */
	METATILE_BEHAVIOR_RIDING_LOG = 22,
	/* Beside the Board Walk's western log: east of it where it starts,
	   metatiles (17, 11) and (17, 14), and west of it once rolled, in column
	   13. */
	METATILE_BEHAVIOR_WEST_LOG_EAST_SIDE = 24,
	METATILE_BEHAVIOR_WEST_LOG_WEST_SIDE = 25,
	/* A second ledge metatile for the stone pillar at The Wall. No metatile of
	   the map holds it. */
	METATILE_BEHAVIOR_WALL_LEDGE_UNUSED = 26,
	/* Beside the Board Walk's eastern log: west of it once rolled, in column
	   21, and east of it where it starts, in column 25. */
	METATILE_BEHAVIOR_EAST_LOG_WEST_SIDE = 27,
	METATILE_BEHAVIOR_EAST_LOG_EAST_SIDE = 28,
	/* The top of the stone post at the foot of The Wall just west of the
	   sprout, metatile (31, 16). */
	METATILE_BEHAVIOR_WALL_POST = 30,
	/* The east edge of the arena, metatiles (8, 13) to (8, 18). */
	METATILE_BEHAVIOR_ARENA = 50
} COLOSSEUM_SATRAGE_MATCH_METATILE_BEHAVIOR;

const SCENE_ENTRANCE *scGetEntrances(void)
{
	return ColosseumSatrageMatchEntrances;
}

const SCENE_EXIT_REGION *scGetExitRegions(void)
{
	return 0;
}

/* Returns the packed stage and battle-result exits. */
const Uint32 *scGetExits(void)
{
	return ColosseumSatrageMatchExits;
}

/* Returns Colosseum Final 2's warriors, obstacles and attendants. */
const ACTOR_PLACE *scGetActorPlacements(void)
{
	return ColosseumSatrageMatchActorPlacements;
}

/* Move on either of the Board Walk's two stone pillars, on the ledge along the
   north edge of its pool, does nothing. */
static void scColosseumSatrageMatchIgnoreMove(void)
{
}

/* Walking along the Board Walk's ledge, on the north edge of its pool, into one
   of its two stone pillars pushes the pillar a metatile further along, if
   nothing stands beyond it.

   Pushed from either side, neither pillar moves when tried, so the eastern one
   keeps closed the narrow channel north of it that leads to a chest holding a
   Nut, metatile (23, 7). */
static void scColosseumSatrageMatchPushBoardWalkPillar(void)
{
	scColossoPushActorAhead();
}

/* Move on the stone pillar at the foot of The Wall slides it a metatile east,
   away from the chest holding a Smoke Bomb, on the ledge at metatile (35, 16).

   After Move and after each push, FLAG_WALL_PILLAR_MOVED records whether the
   pillar stands in WALL_PILLAR_MOVED_COLUMN, and metatile (35, 77) is opened or
   walled to match. scInit opens that metatile again for the race but does not
   move the pillar: it is back beside the chest, as a probe of the race with the
   flag set shows.

   Metatile (35, 77) is metatile (35, 13) of the map's third layer, which starts
   at row 64 (the vine Growth raises is copied onto the same layer, at (32,
   77..80)). There it stands walled between open metatiles (35, 12) and (35, 14)
   of column 35; the flag opens it. A walk staged on that layer from (35, 12)
   southward stops there without the flag and goes on with it. Recorded, that
   layer is the face of The Wall above the ledge: Isaac put on it at (35, 12)
   shows over the pillar's top; without the flag he stays there, holding south
   and then west; with it he goes on down past the pillar to the ledge's height
   and west behind the chest. Its rows above, (x, 0..7), are open across, and
   the vine Growth raises opens column 32, so a climb up the vine and down
   column 35 is open on the metatiles. In the race the vine is climbed only from
   the sprout's own metatile. A climb starts from the metatile the leader stands
   on: a climbing metatile (METATILE_BEHAVIOR_CLIMB, read by fdUpdateWalking),
   with his way blocked for more than FIELD_PUSH_FRAMES frames and Up held
   (fdRunPushMetatile). Column 32's climbing metatiles on the ground layer are
   (32, 15) and (32, 16), and the sprout, which blocks the way, stands on (32,
   16). Put at the vine's foot, (32, 17), and holding Up for five seconds, Isaac
   stands against it and does not climb; put on (32, 16) itself, he climbs the
   vine up The Wall's face and walks on north over its top. Going down column 35
   past the opened metatile was not seen. */
static void scColosseumSatrageMatchMoveWallPillar(void)
{
	Sint32 column;
	Sint32 row;

	if(FIELD_METATILE(fdActorGet(ACTOR_WALL_PILLAR)->x.fixed) ==
		WALL_PILLAR_MOVED_COLUMN) {
		gmFlagSet(FLAG_WALL_PILLAR_MOVED);
		column = 35;
		row = 77;
		fdMapGridCopyMetatileAttributes(35, 78, 1, 1, column, row);
	} else {
		gmFlagClear(FLAG_WALL_PILLAR_MOVED);
		column = 35;
		row = 77;
		fdMapGridCopyMetatileAttributes(34, 77, 1, 1, column, row);
	}
}

/* Walking west along the ledge at the foot of The Wall, from metatile (37, 16),
   pushes the stone pillar back a metatile west, beside the chest, when Move has
   left it in the next metatile.

   With the pillar in its first place the next metatile is empty and nothing
   happens. */
static void scColosseumSatrageMatchPushWallPillar(void)
{
	scColossoPushActorAhead();
	scColosseumSatrageMatchMoveWallPillar();
}

/* Move on one of the three stone pillars along the Mini-Maze's south edge,
   where the race starts, slides it a metatile. The walkthrough's advice is for
   Garet to move the middle one, to save Isaac time.

   Each pillar's top is a square to hop onto (METATILE_BEHAVIOR_JUMP_FORWARD),
   and its column is kept in a byte of flags so that the pillars stand where
   Move left them in the race. The metatile a pillar leaves keeps its square
   until the scene is entered again. The first copy puts back the Board Walk's
   log metatiles as the map had them, without the logs' own rows; nothing rolls
   those logs before the race. */
static void scColosseumSatrageMatchMoveMiniMazePillar(void)
{
	Sint32 column;

	fdMapGridCopyMetatileAttributes(BOARD_WALK_STORE_COLUMN, 11, 12, 4, 14, 11);

	column = FIELD_METATILE(fdActorGet(ACTOR_MINI_MAZE_FIRST_PILLAR)->x.fixed);
	syFieldGameFlagSetByte(FLAG_MINI_MAZE_FIRST_PILLAR_COLUMN, column);
	fdMapGridCopyMetatileAttributes(71, 16, 1, 1, column, 16);

	column =
		FIELD_METATILE(fdActorGet(ACTOR_MINI_MAZE_FIRST_PILLAR + 1)->x.fixed);
	syFieldGameFlagSetByte(FLAG_MINI_MAZE_SECOND_PILLAR_COLUMN, column);
	fdMapGridCopyMetatileAttributes(71, 16, 1, 1, column, 16);

	column =
		FIELD_METATILE(fdActorGet(ACTOR_MINI_MAZE_FIRST_PILLAR + 2)->x.fixed);
	syFieldGameFlagSetByte(FLAG_MINI_MAZE_THIRD_PILLAR_COLUMN, column);
	fdMapGridCopyMetatileAttributes(71, 16, 1, 1, column, 16);
}

/* Nothing visible happens when Force is cast at the tall log by the Board
   Walk's channel, just before the fist flies. The scene sets the Psynergy's
   `leaveTgt`, so that the fist does not shove the log 16 pixels away as it
   shoves what else it hits, and the log topples from where it stands
   (scColosseumSatrageMatchKnockDownLog). */
static void scColosseumSatrageMatchKeepLogStillForForce(void)
{
	Uint8 *effectState = FieldEffectStateWork;

	((FIELD_EFFECT_STATE *)effectState)->leaveTgt = 1;
}

/* Force on the tall log standing at the east end of the Board Walk's channel
   knocks it over west into the water, where it lies beside the white platforms
   as one more step across, metatile (20, 16), on the way to the arena. */
static void scColosseumSatrageMatchKnockDownLog(void)
{
	ACTOR *obstacle;
	Sint32 fallFrames;
	Sint32 rotationStep;
	Sint32 settleFrames;
	Sint32 settleRotationStep;
	SPR	  *sprite;

	obstacle = fdActorGet(ACTOR_FALLING_LOG);
	sprite = obstacle->spr;
	fdScriptSetFlag(FLAG_LOG_KNOCKED_DOWN);
	obstacle->acceleration = toFIXED(1) / 10;
	obstacle->speedLimit = toFIXED(6) / 5;
	sySndPlay(SE_TOPPLE);
	syFieldActorSetMoveTarget(obstacle, 336, 10, 264);
	for(rotationStep = 0, fallFrames = 0; fallFrames < 10; fallFrames++) {
		sprite->rotation -= rotationStep;
		sySynch(1);
		rotationStep += FALL_SPIN_ACCELERATION;
	}
	syFieldActorSetMoveTarget(obstacle, 330, -16, 264);
	for(settleRotationStep = SETTLE_SPIN_START, settleFrames = 0;
		settleFrames < 22; settleFrames++) {
		sprite->rotation -= settleRotationStep;
		sySynch(1);
		settleRotationStep += FALL_SPIN_ACCELERATION;
	}
	fdActorWaitForMove(obstacle);
	fdScriptWait(2);
	sySndPlay(SE_LOG_FALL);
	sprite->rotation = 0;
	fdActorSetAnimation(ACTOR_FALLING_LOG, FALLING_LOG_ANIMATION_FALLEN);
	obstacle->x.fixed = toFIXED(336);
	obstacle->y.fixed = -toFIXED(8);
	obstacle->z.fixed = toFIXED(264);
	obstacle->velY = 0;
	obstacle->velX = 0;
	fdMapGridCopyMetatileAttributes(19, 16, 1, 1, 20, 16);
	fdMapGridCopyMetatileAttributes(20, 80, 1, 1, 21, 80);
}

/* Frost on the western of the Log-Rolling Stage's two puddles, metatile (44,
   17), freezes it into a pillar of ice, a square Isaac can hop onto
   (FLAG_WEST_PUDDLE_FROZEN). The walkthrough names this one the useful one. */
static void scColosseumSatrageMatchFreezeWestPuddle(void)
{
	gmFlagSet(FLAG_WEST_PUDDLE_FROZEN);
	fdActorGet(ACTOR_PUDDLE_WEST)->motionFlg = 0;
	fdMapGridCopyMetatileAttributes(46, 17, 1, 1, 44, 17);
}

/* Frost on the eastern of the Log-Rolling Stage's two puddles, metatile (50,
   17), freezes it into a pillar of ice, a square Isaac can hop onto
   (FLAG_EAST_PUDDLE_FROZEN). */
static void scColosseumSatrageMatchFreezeEastPuddle(void)
{
	gmFlagSet(FLAG_EAST_PUDDLE_FROZEN);
	fdActorGet(ACTOR_PUDDLE_EAST)->motionFlg = 0;
	fdMapGridCopyMetatileAttributes(46, 17, 1, 1, 50, 17);
}

/* Growth on the sprout at the foot of The Wall, metatile (32, 16), grows it
   into a vine up the wall that can be climbed (FLAG_PLANT_GROWN). */
static void scColosseumSatrageMatchGrowPlant(void)
{
	gmFlagSet(FLAG_PLANT_GROWN);
	fdMapGridCopyMetatileAttributes(32, 37, 1, 4, 32, 77);
}

/* Walking south off the top of the stone post just west of the sprout at the
   foot of The Wall, metatile (31, 16), Isaac jumps down to the ground below. */
static void scColosseumSatrageMatchJumpDownFromPost(void)
{
	syFieldActorJumpDownLedge(MainRoutineStateWork.leaderActorId, 6, 0);
}

/* Walking west off the top of the stone post just west of the sprout at the
   foot of The Wall, metatile (31, 16), Isaac hops across to the next post
   west. */
static void scColosseumSatrageMatchHopWestFromPost(void)
{
	fdTryJumpForward();
}

/* Rolls one of the Log-Rolling Stage's logs across the water by the offset,
   with Isaac on top walking backwards as it turns under him, and stops both in
   the middle of their metatiles. */
static void scColosseumSatrageMatchMoveRidingLog(
	Sint32 logId, Sint32 offsetX, Sint32 offsetZ)
{
	ACTOR *rider;
	ACTOR *log;
	FIXED  destinationX;
	FIXED  destinationZ;

	rider = fdActorGet(MainRoutineStateWork.leaderActorId);
	log = fdActorGet(logId);
	fdScriptStart();
	{
		destinationX =
			((rider->x.fixed + COLOSSEUM_SATRAGE_MATCH_OFFSET_FIXED(offsetX)) &
				FIELD_METATILE_MASK) +
			toFIXED(8);
		destinationZ =
			((rider->z.fixed + COLOSSEUM_SATRAGE_MATCH_OFFSET_FIXED(offsetZ)) &
				FIELD_METATILE_MASK) +
			toFIXED(8);

		rider->speedLimit = toFIXED(1);
		rider->acceleration = toFIXED(1) / 2;
		syFieldActorSetMoveTargetFX(
			rider, destinationX, rider->y.fixed, destinationZ);
	}
	fdActorSelectAnimation(rider, ANIM_LEADER_WALK_BACKWARDS);
	{
		destinationX =
			((log->x.fixed + COLOSSEUM_SATRAGE_MATCH_OFFSET_FIXED(offsetX)) &
				FIELD_METATILE_MASK) +
			toFIXED(8);
		destinationZ =
			((log->z.fixed + COLOSSEUM_SATRAGE_MATCH_OFFSET_FIXED(offsetZ)) &
				FIELD_METATILE_MASK) +
			toFIXED(8);

		log->speedLimit = toFIXED(1);
		log->acceleration = toFIXED(1) / 2;
		syFieldActorSetMoveTargetFX(
			log, destinationX, log->y.fixed, destinationZ);
	}
	if(offsetX < 0 || offsetZ < 0) {
		fdActorSelectAnimation(log, FLOATING_LOG_ANIMATION_ROLL_WEST_OR_NORTH);
	} else {
		fdActorSelectAnimation(log, FLOATING_LOG_ANIMATION_ROLL_EAST_OR_SOUTH);
	}
	sySndPlay(SE_LOG_PUSH);
	fdActorWaitForMove(rider);
	sySndPlay(SE_STOP_ACTION_SE);
	fdActorSelectAnimation(log, FLOATING_LOG_ANIMATION_SETTLE);
	fdScriptEnd();
}

/* Standing on one of the Log-Rolling Stage's two logs and holding Left or Right
   rolls it four metatiles across the pool with Isaac on top, towards the far
   bank: west from its east end, east from its west end. */
static void scColosseumSatrageMatchRideLog(void)
{
	ACTOR *rider = fdActorGet(MainRoutineStateWork.leaderActorId);
	Sint32 column = FIELD_METATILE(rider->x.fixed);
	Sint32 offsetX = 0;
	Sint32 logId = ACTOR_RIDING_LOG_NORTH;

	if(FIELD_METATILE(rider->z.fixed) >= RIDING_LOG_SOUTH_ROW) {
		logId = ACTOR_RIDING_LOG_SOUTH;
	}
	if(FIELD_METATILE(fdActorGet(logId)->x.fixed) != column) {
		return;
	}
	if(column > 51) {
		if(PerData & KEY_LEFT) {
			offsetX = -COLOSSEUM_SATRAGE_MATCH_LOG_TRAVEL_PIXELS;
		}
	} else if(PerData & KEY_RIGHT) {
		offsetX = COLOSSEUM_SATRAGE_MATCH_LOG_TRAVEL_PIXELS;
	}
	if(offsetX != 0) {
		scColosseumSatrageMatchMoveRidingLog(logId, offsetX, 0);
		fdMapGridCopyMetatileAttributes(
			LOG_ROLLING_STORE_COLUMN, 10, 5, 6, 48, 10);
		column = FIELD_METATILE(fdActorGet(ACTOR_RIDING_LOG_NORTH)->x.fixed);
		fdMapGridCopyMetatileAttributes(
			52, 28, 1, 3, column, RIDING_LOG_NORTH_ROW);
		column = FIELD_METATILE(fdActorGet(ACTOR_RIDING_LOG_SOUTH)->x.fixed);
		fdMapGridCopyMetatileAttributes(
			52, 28, 1, 3, column, RIDING_LOG_SOUTH_ROW);
	}
}

/* Isaac leans on one of the Board Walk's logs and it rolls to a place given in
   half metatiles, with Isaac walking after it half the way. */
static void scColosseumSatrageMatchRollLogToMetatile(
	Sint32 logId, Sint32 halfMetatileX, Sint32 halfMetatileZ)
{
	Sint32 pusher = MainRoutineStateWork.leaderActorId;
	ACTOR *pusherActor = fdActorGet(pusher);
	ACTOR *log = fdActorGet(logId);
	Sint32 heading = FACING_NEAREST_EIGHTH(pusherActor->facing);
	Sint32 alongX = FIELD_METATILE(log->x.fixed) != halfMetatileX / 2;
	FIXED  offsetX;
	FIXED  offsetZ;

	halfMetatileX = FIELD_HALF_METATILE_ORIGIN(halfMetatileX);
	halfMetatileZ = FIELD_HALF_METATILE_ORIGIN(halfMetatileZ);
	if(alongX) {
		offsetX = (halfMetatileX - log->x.fixed) / 2;
		offsetZ = 0;
	} else {
		offsetX = 0;
		offsetZ = (halfMetatileZ - log->z.fixed) / 2;
	}
	fdScriptStart();
	fdActorSetAnimation(pusher, ANIM_PUSH);
	fdScriptWait(6);
	log->speedLimit = toFIXED(1) / 2;
	log->acceleration = toFIXED(1) / 5;
	fdActorSelectAnimation(
		log, ColosseumSatrageMatch_RollLogScript[heading / (DEGtoANG(90))]);
	syFieldActorSetMoveTargetFX(log, halfMetatileX, 0, halfMetatileZ);
	fdScriptWait(6);
	fdActorSetAnimation(pusher, ANIM_WALK);
	/* Follow halfway behind the rolling log while its finish effect runs. */
	fdActorStartCamera(((FIELD_STATE *)syHeapSlotAllocEWRAM(
							HEAP_SLOT_FIELD, sizeof(FIELD_STATE)))
						   ->camera,
		(Sint32)log);
	fdActorSetSpeedAndAcceleration(pusher, 0.5, 0.2);
	fdActorSelectAnimation(pusherActor, ANIM_WALK);
	syFieldActorSetMoveTargetFX(pusherActor, pusherActor->x.fixed + offsetX, 0,
		pusherActor->z.fixed + offsetZ);
	sySndPlay(SE_HEAVY_MOVE);
	fdActorWaitForMove(pusherActor);
	fdActorSelectAnimation(pusherActor, ANIM_STAND);
	fdActorWaitForMove(log);
	sySndPlay(SE_STOP_ACTION_SE);
	sySndPlay(SE_HEAVY_STOP);
	fdActorSelectAnimation(log, LOG_ANIMATION_AT_REST);
	fdScriptWait(15);
	fdScriptEnd();
}

/* Walls off the metatiles under the Board Walk's logs where they now lie, after
   putting back the stage's metatiles as the map had them. */
static void scColosseumSatrageMatchUpdateRollingLogCollision(void)
{
	Sint32 metatile;

	fdMapGridCopyMetatileAttributes(BOARD_WALK_STORE_COLUMN, 11, 12, 4, 14, 11);

	metatile = FIELD_METATILE(fdActorGet(ACTOR_ROLLING_LOG_WEST)->x.fixed);
	fdMapGridCopyMetatileAttributes(13, 28, 1, 4, metatile, 11);

	metatile = FIELD_METATILE(fdActorGet(ACTOR_ROLLING_LOG_EAST)->x.fixed);
	fdMapGridCopyMetatileAttributes(13, 28, 1, 4, metatile, 11);

	metatile = FIELD_METATILE(fdActorGet(ACTOR_ROLLING_LOG_CENTER)->z.fixed);
	fdMapGridCopyMetatileAttributes(13, 28, 4, 1, 18, metatile);
}

/* Walking west into the Board Walk's western log, from beside it at metatile
   (17, 11) or (17, 14), rolls it two metatiles west. */
static void scColosseumSatrageMatchRollWestLogWest(void)
{
	scColosseumSatrageMatchRollLogToMetatile(ACTOR_ROLLING_LOG_WEST, 29, 26);
	scColosseumSatrageMatchUpdateRollingLogCollision();
}

/* Walking east into the Board Walk's western log from column 13, once it has
   been rolled west, rolls it back two metatiles east. */
static void scColosseumSatrageMatchRollWestLogEast(void)
{
	scColosseumSatrageMatchRollLogToMetatile(ACTOR_ROLLING_LOG_WEST, 33, 26);
	scColosseumSatrageMatchUpdateRollingLogCollision();
}

/* Walking west into the Board Walk's eastern log, from beside it in column 25,
   rolls it two metatiles west, as the attendant shows on the tour. */
static void scColosseumSatrageMatchRollEastLogWest(void)
{
	scColosseumSatrageMatchRollLogToMetatile(ACTOR_ROLLING_LOG_EAST, 45, 26);
	scColosseumSatrageMatchUpdateRollingLogCollision();
}

/* Walking east into the Board Walk's eastern log from column 21, once it has
   been rolled west, rolls it back two metatiles east. */
static void scColosseumSatrageMatchRollEastLogEast(void)
{
	scColosseumSatrageMatchRollLogToMetatile(ACTOR_ROLLING_LOG_EAST, 49, 26);
	scColosseumSatrageMatchUpdateRollingLogCollision();
}

/* Unused: no cue row calls it.

   It would have rolled ACTOR_ROLLING_LOG_CENTER, placed at (0, 0) and never
   moved to the Board Walk, to a target 16 pixels north of its South twin's. The
   western and eastern logs roll west and east the same way
   (scColosseumSatrageMatchRollWestLogWest and the rest). */
static UNUSED void scColosseumSatrageMatchUnusedRollCenterLogNorth(void)
{
	scColosseumSatrageMatchRollLogToMetatile(ACTOR_ROLLING_LOG_CENTER, 40, 23);
	scColosseumSatrageMatchUpdateRollingLogCollision();
}

/* Unused: as scColosseumSatrageMatchUnusedRollCenterLogNorth, rolling
   ACTOR_ROLLING_LOG_CENTER, which is not on the Board Walk, to the target 16
   pixels south of that one's. */
static UNUSED void scColosseumSatrageMatchUnusedRollCenterLogSouth(void)
{
	scColosseumSatrageMatchRollLogToMetatile(ACTOR_ROLLING_LOG_CENTER, 40, 25);
	scColosseumSatrageMatchUpdateRollingLogCollision();
}

/* Each frame while the shifting floor runs, carries Isaac north or south with
   the lift he stands on, swaps the lifts between a line across the channel and
   their broken pattern every COLOSSEUM_SATRAGE_MATCH_LIFT_CYCLE_FRAMES, and
   makes their metatiles squares to hop onto only while they stand in line. */
static void scColosseumSatrageMatchUpdateLifts(void)
{
	Sint32 liftId;
	ACTOR *rider;
	ACTOR *lift;
	FIXED  liftX;
	FIXED  riderX;
	FIXED  deltaX;
	FIXED  liftZ;
	FIXED  riderZ;
	FIXED  deltaZ;

	rider = fdActorGet(MainRoutineStateWork.leaderActorId);
	/* Carry a rider only while both horizontal distances are less than ten
	   pixels. */
	for(liftId = ACTOR_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT;
		liftId <= ACTOR_COLOSSEUM_SATRAGE_MATCH_LAST_LIFT; liftId++) {
		lift = fdActorGet(liftId);
		lift->paused = 0;
		liftX = lift->x.fixed;
		riderX = rider->x.fixed;
		deltaX = liftX - riderX;
		if(deltaX >= 0) {
			if(deltaX >= toFIXED(10)) {
				continue;
			}
		} else {
			riderX -= liftX;
			if(riderX >= toFIXED(10)) {
				continue;
			}
		}
		liftZ = lift->z.fixed;
		riderZ = rider->z.fixed;
		deltaZ = liftZ - riderZ;
		if(deltaZ >= 0) {
			if(deltaZ >= toFIXED(10)) {
				continue;
			}
		} else {
			riderZ -= liftZ;
			if(riderZ >= toFIXED(10)) {
				continue;
			}
		}
		if(gmFlagGet(FLAG_FIELD_HANDLING_REQUESTS) != 0) {
			riderZ = lift->z.fixed;
		} else {
			riderZ = rider->z.fixed;
			deltaZ = lift->velZ;
			riderZ += deltaZ;
		}
		rider->z.fixed = riderZ;
	}
	/* The final lift's destination selects the metatile behaviors for the
	   row. */
	if(ColosseumSatrageMatch_LiftTimer != 0 && lift->tgtX == ACTOR_NO_TARGET) {
		if(ColosseumSatrageMatch_LiftsApart == 0) {
			fdMapGridCopyMetatileAttributes(58, 28, 7, 1, 58, 13);
		} else {
			fdMapGridCopyMetatileAttributes(58, 10, 1, 1, 58, 11);
		}
	} else {
		fdMapGridCopyMetatileAttributes(57, 11, 1, 1, 58, 11);
		fdMapGridCopyMetatileAttributes(58, 14, 7, 1, 58, 13);
	}
	if(ColosseumSatrageMatch_LiftTimer == 0) {
		ColosseumSatrageMatch_LiftsApart ^= 1;
		if(ColosseumSatrageMatch_LiftsApart != 0) {
			syFieldActorSetMoveTarget(
				fdActorGet(ACTOR_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT), 936, 0,
				184);
			syFieldActorSetMoveTarget(
				fdActorGet(ACTOR_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT + 1), 968,
				0, 248);
			syFieldActorSetMoveTarget(
				fdActorGet(ACTOR_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT + 2), 1000,
				0, 184);
			syFieldActorSetMoveTarget(
				fdActorGet(ACTOR_COLOSSEUM_SATRAGE_MATCH_LAST_LIFT), 1032, 0,
				248);
			fdActorSetAnimation(
				ACTOR_LIFT_OPERATOR, LIFT_OPERATOR_ANIMATION_LIFTS_APART);
		} else {
			syFieldActorSetMoveTarget(
				fdActorGet(ACTOR_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT), 936, 0,
				216);
			syFieldActorSetMoveTarget(
				fdActorGet(ACTOR_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT + 1), 968,
				0, 216);
			syFieldActorSetMoveTarget(
				fdActorGet(ACTOR_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT + 2), 1000,
				0, 216);
			syFieldActorSetMoveTarget(
				fdActorGet(ACTOR_COLOSSEUM_SATRAGE_MATCH_LAST_LIFT), 1032, 0,
				216);
			fdActorSetAnimation(
				ACTOR_LIFT_OPERATOR, LIFT_OPERATOR_ANIMATION_LIFTS_LINED_UP);
		}
	}
	ColosseumSatrageMatch_LiftTimer++;
	if(ColosseumSatrageMatch_LiftTimer >=
		COLOSSEUM_SATRAGE_MATCH_LIFT_CYCLE_FRAMES) {
		if(gmFlagGet(FLAG_FIELD_HANDLING_REQUESTS) == 0) {
			ColosseumSatrageMatch_LiftTimer = 0;
		}
	}
}

/* Stops the shifting floor and lines its four lifts up across the channel, for
   the attendant's demonstration. */
static void scColosseumSatrageMatchResetLifts(void)
{
	ColosseumSatrageMatch_LiftTimer = 0;
	ColosseumSatrageMatch_LiftsApart = 0;
	syCloseEvent(scColosseumSatrageMatchUpdateLifts);
	fdActorSetPos(ACTOR_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT, 936, 216);
	fdActorSetPos(ACTOR_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT + 1, 968, 216);
	fdActorSetPos(ACTOR_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT + 2, 1000, 216);
	fdActorSetPos(ACTOR_COLOSSEUM_SATRAGE_MATCH_LAST_LIFT, 1032, 216);
	fdActorSetAnimation(
		ACTOR_LIFT_OPERATOR, LIFT_OPERATOR_ANIMATION_LIFTS_LINED_UP);
}

/* Starts the shifting floor's lifts moving again. */
static void scColosseumSatrageMatchStartLifts(void)
{
	sySetFrameEvent(scColosseumSatrageMatchUpdateLifts, EVENT_PASS_FRAME + 5);
	gmFlagClear(FLAG_FIELD_HANDLING_REQUESTS);
}

/* Waits, for up to ten seconds, until the four lifts stand lined up across the
   water, for the demonstration to jump onto them. */
static void scColosseumSatrageMatchWaitForLiftsLinedUp(void)
{
	Sint32 elapsedFrames;

	sySynch(10);

	elapsedFrames = 0;
	while(ColosseumSatrageMatch_LiftsApart != 0 ||
		ColosseumSatrageMatch_LiftTimer !=
			COLOSSEUM_SATRAGE_MATCH_LIFT_CROSSING_FRAME) {
		sySynch(1);
		elapsedFrames++;
		if(elapsedFrames >=
			COLOSSEUM_SATRAGE_MATCH_DEMONSTRATION_TIMEOUT_FRAMES) {
			return;
		}
	}
}

/* Halt on the young man who works the shifting floor's lifts freezes him, and
   the lifts stop in a line across the channel for Isaac to hop over
   (FLAG_LIFTS_HALTED). */
static void scColosseumSatrageMatchHaltLifts(void)
{
	fdPsynergyHaltActor(ACTOR_LIFT_OPERATOR);
	gmFlagSet(FLAG_LIFTS_HALTED);

	if(ColosseumSatrageMatch_LiftsApart != 0) {
		ColosseumSatrageMatch_LiftTimer = 0;
	}

	sySynch(30);
	sySynch(1);

	syCloseEvent(scColosseumSatrageMatchUpdateLifts);

	fdMapGridCopyMetatileAttributes(58, 28, 7, 1, 58, 13);
	fdMapGridCopyMetatileAttributes(57, 11, 1, 1, 58, 11);
}

/* Reaching the arena at the west end, metatiles (8, 13) to (8, 18), Isaac takes
   the Chain Mail if he is first and the Cotton Shirt if not, waits for Satrage,
   and the two nod and fight. */
static void scColosseumSatrageMatchReachArena(void)
{
	Sint32 opponentFirst;
	Sint32 i;
	Sint32 opponentFirstSlot = 2;
	scColossoNoOperation();
	fdScriptStart();
	opponentFirst = scColossoTakePrize(ITEM_CHAIN_MAIL, ITEM_COTTON_SHIRT);
	scColossoWaitForOpponentToFinish();
	/* Up to ten waits of 300 frames while the opponent holds up his prize. */
	for(i = 0; i < 10; i++) {
		fdActorWaitForScriptEndById(ACTOR_OPPONENT);
	}
	fdActorSetSpeed(ACTOR_OPPONENT, 1);
	fdActorWalkTo(ACTOR_OPPONENT, 88, 256);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 1);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 120, 256);
	fdActorSetAnimation(ACTOR_OPPONENT, ANIM_STAND);
	fdActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_OPPONENT, 0);
	fdScriptWait(10);
	fdActorSetAnimation(ACTOR_OPPONENT, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdScriptWait(20);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 2);
	fdActorSetSpeed(ACTOR_OPPONENT, 2);
	fdActorWalkTo(ACTOR_PARTY_LEADER, 112, 256);
	fdActorWalkToAndWait(ACTOR_OPPONENT, 96, 256);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_STRIDE);
	fdActorSetAnimation(ACTOR_OPPONENT, WARRIOR_ANIMATION_FIGHTING_STANCE);
	fdScriptWait(10);
	fdRequestBattleFromEncounterTable(
		ENCOUNTER_ZONE_COLOSSO, opponentFirstSlot - opponentFirst + 1);
	SET_BATTLE_TYPE(BATTLE_TYPE_SCRIPTED);
	fdScriptSetAfterBattleDestination(
		SCENE_COLOSSEUM_SATRAGE_MATCH, COLOSSEUM_SATRAGE_MATCH_AFTER_MATCH_WON);
	fdScriptSetDefeatDestination(SCENE_COLOSSEUM_SATRAGE_MATCH,
		COLOSSEUM_SATRAGE_MATCH_AFTER_MATCH_LOST);
	gmFlagSet(FLAG_KEEP_MUSIC_ONCE);
}

/* Returns Colosseum Final 2's stages, chests, Psynergy targets and
   attendants. */
const CUE *scGetCues(void)
{
	return ColosseumSatrageMatchCues;
}

/* Back from the battle, Isaac and Satrage stand in the arena as the result is
   shown. */
static void scColosseumSatrageMatchStageBattleResult(Sint32 result)
{
	fdActorReleaseById(ACTOR_PRIZE_A);
	fdActorReleaseById(ACTOR_PRIZE_B);
	gmPartyRestoreHpPp(1);
	fdScriptStart();
	fdActorSetPos(ACTOR_OPPONENT, 88, 256);
	fdActorSetPos(ACTOR_PARTY_LEADER, 120, 256);
	/* BUG: FACING_SOUTH is passed where fdActorFaceActor takes an actor id;
	   fdActorLookup has no actor past 191, so neither turns. */
#if FIX_FACING_PASSED_AS_ACTOR_ID
	fdActorFaceDirection(ACTOR_OPPONENT, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
#else
	fdActorFaceActor(ACTOR_OPPONENT, FACING_SOUTH, 0);
	fdActorFaceActor(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
#endif
	if(result < 0) {
		fdActorSetAnimation(ACTOR_OPPONENT, WARRIOR_ANIMATION_ARMS_RAISED);
		fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_CROUCH);
	} else {
		fdActorSetAnimation(ACTOR_OPPONENT, WARRIOR_ANIMATION_DEFEATED);
		fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_ARMS_RAISED);
	}
	sySynch(1);
	fdCameraMoveTo(104, 0, 192, 0);
	scColossoShowMatchResult(result);
	fdScriptEnd();
}

/* Sets up Colosseum Final 2 for the visit its entrance stands for. */
Sint32 scInit(void)
{
	ACTOR *actor;
	Sint32 x;
	Sint32 obstacleId;
	Sint32 column;
	Sint32 liftId;
	Sint32 columnOffset;
	Sint32 row;

	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_FADE, 0);
	gmFlagSet(FLAG_RETREAT_BLOCKED);
	fdMapGridCopyMetatileAttributes(14, 11, 12, 4, BOARD_WALK_STORE_COLUMN, 11);
	fdMapGridCopyMetatileAttributes(48, 10, 5, 6, LOG_ROLLING_STORE_COLUMN, 10);
	for(obstacleId = ACTOR_BOARD_WALK_FIRST_PLATFORM;
		obstacleId <= ACTOR_FALLING_LOG; obstacleId++) {
		actor = fdActorGet(obstacleId);
		/* The falling log, last of these, takes the same number, its
		   FALLING_LOG_ANIMATION_FALLEN, until the lines below stand it up. */
		fdActorSelectAnimation(actor, PLATFORM_ANIMATION_AT_WATERLINE);
		actor->motionFlg = 0;
		actor->y.fixed = 0;
		actor->priFlg = ACTOR_PRIORITY_UNDERFOOT;
	}
	fdActorGet(ACTOR_FORCE_TARGET)->priFlg = ACTOR_PRIORITY_UNDERFOOT;
	if(gmFlagGet(FLAG_LOG_KNOCKED_DOWN)) {
		actor = fdActorGet(ACTOR_FALLING_LOG);
		actor->x.fixed = toFIXED(336);
		actor->y.fixed = -toFIXED(8);
		actor->z.fixed = toFIXED(264);
		fdMapGridCopyMetatileAttributes(19, 16, 1, 1, 20, 16);
		fdMapGridCopyMetatileAttributes(20, 80, 1, 1, 21, 80);
	} else {
		actor = fdActorGet(ACTOR_FALLING_LOG);
		fdActorSelectAnimation(actor, FALLING_LOG_ANIMATION_STANDING);
		actor->y.fixed = toFIXED(16);
	}
	fdActorGet(ACTOR_WALL_PILLAR)->priFlg = ACTOR_PRIORITY_UNDERFOOT;
	/* With the wall pillar moved, metatile (35, 77) takes the open ground of
	   the metatile south of it; the map stores it as a wall. */
	if(gmFlagGet(FLAG_WALL_PILLAR_MOVED)) {
		fdMapGridCopyMetatileAttributes(35, 78, 1, 1, 35, 77);
	}
	if(gmFlagGet(FLAG_PLANT_GROWN)) {
		fdActorSetAnimation(ACTOR_GROWTH_PLANT, ANIM_SHAKE_HEAD);
		fdMapGridCopyMetatileAttributes(32, 37, 1, 4, 32, 77);
	}
	if(gmFlagGet(FLAG_WEST_PUDDLE_FROZEN)) {
		fdActorGet(ACTOR_PUDDLE_WEST)->motionFlg = 0;
		fdActorGet(ACTOR_PUDDLE_WEST)->priFlg = ACTOR_PRIORITY_UNDERFOOT;
		fdActorSetAnimation(ACTOR_PUDDLE_WEST, PUDDLE_ANIMATION_PILLAR);
		fdMapGridCopyMetatileAttributes(46, 17, 1, 1, 44, 17);
	}
	if(gmFlagGet(FLAG_EAST_PUDDLE_FROZEN)) {
		fdActorGet(ACTOR_PUDDLE_EAST)->motionFlg = 0;
		fdActorGet(ACTOR_PUDDLE_EAST)->priFlg = ACTOR_PRIORITY_UNDERFOOT;
		fdActorSetAnimation(ACTOR_PUDDLE_EAST, PUDDLE_ANIMATION_PILLAR);
		fdMapGridCopyMetatileAttributes(46, 17, 1, 1, 50, 17);
	}
	actor = fdActorGet(ACTOR_RIDING_LOG_NORTH);
	column = FIELD_METATILE(actor->x.fixed);
	actor->motionFlg = 0;
	actor->priFlg = ACTOR_PRIORITY_UNDERFOOT;
	fdMapGridCopyMetatileAttributes(52, 28, 1, 3, column, 10);
	actor = fdActorGet(ACTOR_RIDING_LOG_SOUTH);
	column = FIELD_METATILE(actor->x.fixed);
	actor->motionFlg = 0;
	actor->priFlg = ACTOR_PRIORITY_UNDERFOOT;
	fdMapGridCopyMetatileAttributes(52, 28, 1, 3, column, 13);

	/* Each Mini-Maze pillar returns to the column its area temp flag byte
	   keeps. */
	x = syFieldGameFlagGetByte(FLAG_MINI_MAZE_FIRST_PILLAR_COLUMN);
	if(x == 0) {
		x = 73;
	}
	actor = fdActorGet(ACTOR_MINI_MAZE_FIRST_PILLAR);
	actor->x.fixed = FIELD_METATILE_CENTER(x);
	actor->motionFlg = 0;
	actor->priFlg = ACTOR_PRIORITY_UNDERFOOT;
	fdMapGridCopyMetatileAttributes(71, 16, 1, 1, x, 16);
	x = syFieldGameFlagGetByte(FLAG_MINI_MAZE_SECOND_PILLAR_COLUMN);
	if(x == 0) {
		x = 76;
	}
	actor = fdActorGet(ACTOR_MINI_MAZE_FIRST_PILLAR + 1);
	actor->x.fixed = FIELD_METATILE_CENTER(x);
	actor->motionFlg = 0;
	actor->priFlg = ACTOR_PRIORITY_UNDERFOOT;
	fdMapGridCopyMetatileAttributes(71, 16, 1, 1, x, 16);
	x = syFieldGameFlagGetByte(FLAG_MINI_MAZE_THIRD_PILLAR_COLUMN);
	if(x == 0) {
		x = 79;
	}
	actor = fdActorGet(ACTOR_MINI_MAZE_FIRST_PILLAR + 2);
	actor->x.fixed = FIELD_METATILE_CENTER(x);
	actor->motionFlg = 0;
	actor->priFlg = ACTOR_PRIORITY_UNDERFOOT;
	fdMapGridCopyMetatileAttributes(71, 16, 1, 1, x, 16);

	scColosseumSatrageMatchUpdateRollingLogCollision();
	/* With FLAG_LIFTS_HALTED set the lifts stay where they are; otherwise their
	   event resumes. */
	fdActorSetAnimation(
		ACTOR_LIFT_OPERATOR, LIFT_OPERATOR_ANIMATION_LIFTS_LINED_UP);
	if(gmFlagGet(FLAG_LIFTS_HALTED)) {
		for(liftId = ACTOR_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT, row = 13,
		columnOffset = 58;
			liftId <= ACTOR_COLOSSEUM_SATRAGE_MATCH_LAST_LIFT;
			liftId++, columnOffset += 2) {
			actor = fdActorGet(liftId);
			actor->priFlg = ACTOR_PRIORITY_UNDERFOOT;
			fdActorSelectAnimation(actor, PLATFORM_ANIMATION_AT_WATERLINE);
			fdMapGridCopyMetatileAttributes(56, 13, 1, 1, columnOffset, row);
		}
		fdActorSetAnimation(
			ACTOR_LIFT_OPERATOR, LIFT_OPERATOR_ANIMATION_LIFTS_LINED_UP);
		fdPsynergyHaltActor(ACTOR_LIFT_OPERATOR);
	} else {
		for(liftId = ACTOR_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT;
			liftId <= ACTOR_COLOSSEUM_SATRAGE_MATCH_LAST_LIFT; liftId++) {
			actor = fdActorGet(liftId);
			actor->priFlg = ACTOR_PRIORITY_UNDERFOOT;
			fdActorSelectAnimation(actor, PLATFORM_ANIMATION_AT_WATERLINE);
			actor->speedLimit = toFIXED(1) / 2;
			actor->acceleration = toFIXED(1) / 5;
		}
		sySetFrameEvent(
			scColosseumSatrageMatchUpdateLifts, EVENT_PASS_FRAME + 5);
		sySetEventFlags(scColosseumSatrageMatchUpdateLifts, 1);
	}
	fdActorSetAnimation(ACTOR_OPPONENT, WARRIOR_ANIMATION_FIGHTING_STANCE);
	MainRoutineStateWork.movementMode = LEADER_MOVEMENT_WALK;
	scColossoShowItemIcon(ACTOR_PRIZE_B, ITEM_COTTON_SHIRT);
	scColossoShowItemIcon(ACTOR_PRIZE_A, ITEM_CHAIN_MAIL);
	fdActorSetSprColById(ACTOR_OPPONENT, SPRITE_COLOR_GRB);
	/* The race, the friends' turns, the tour, and the battle's result. */
	switch(MainRoutineStateWork.entrance) {
	case COLOSSEUM_SATRAGE_MATCH_RACE_FROM_LEND_A_HAND:
		scColosseumSatrageMatchStartRaceGauge(ACTOR_PARTY_LEADER,
			ACTOR_OPPONENT, 5, toFIXED(104), toFIXED(256), ACTOR_PRIZE_A,
			ACTOR_PRIZE_B);
		fdMapGridCopyMetatilesLayered(127, 0, 1, 2, 79, 6);
		fdActorReleaseById(ACTOR_GUIDE);
		fdActorReleaseById(ACTOR_MINI_MAZE_ATTENDANT);
		fdActorReleaseById(ACTOR_SHIFTING_FLOOR_ATTENDANT);
		fdActorReleaseById(ACTOR_LOG_ROLLING_ATTENDANT);
		fdActorReleaseById(ACTOR_WALL_ATTENDANT);
		fdActorReleaseById(ACTOR_BOARD_WALK_ATTENDANT);
		if(!gmFlagGet(FLAG_KEEP_PARTY_POSITION)) {
			sySndPlay(SOUND_FADE_OUT_BGM);
			scColossoCtrlCharacterAlone(ISAAC);
			scColossoPlaceFriendsAtCheeringSpots();
			scColossoShowOpeningLettering(FINALS_MATCH);
		}
		fdActorWatchAndSetScript(ACTOR_GARET, ACTOR_PARTY_LEADER);
		fdActorWatchAndSetScript(ACTOR_IVAN, ACTOR_PARTY_LEADER);
		fdActorWatchAndSetScript(ACTOR_MIA, ACTOR_PARTY_LEADER);
		scColosseumSatrageMatchStartOpponentPath(
			RESOURCE_SATRAGE_OPPONENT_PATH);
		break;
	case COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_EAST:
		sySetFrameEvent(scColossoRaiseTakePlaceOnPsynergy, EVENT_PASS_FRAME);
		fdActorReleaseById(ACTOR_PRIZE_A);
		fdActorReleaseById(ACTOR_PRIZE_B);
		if(!gmFlagGet(FLAG_KEEP_PARTY_POSITION)) {
			scColossoPlaceFriendsAtCheeringSpots();
			scColossoCtrlCharacterAlone(GARET);
			scColossoShowOpeningLettering(LETTERING_LEND_A_HAND);
		}
		break;
	case COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_WEST:
		if(!gmFlagGet(FLAG_KEEP_PARTY_POSITION)) {
			scColosseumSatrageMatchIntroduceSecondFinals(ACTOR_GUIDE);
			scColossoClearCheeringSpots();
		}
		break;
	case COLOSSEUM_SATRAGE_MATCH_AFTER_MATCH_WON:
		scColosseumSatrageMatchStageBattleResult(2);
		syImportScriptSetExitRequest(
			COLOSSEUM_SATRAGE_MATCH_TO_COLOSSEUM_NORTH);
		break;
	case COLOSSEUM_SATRAGE_MATCH_AFTER_MATCH_LOST:
		scColosseumSatrageMatchStageBattleResult(-2);
		syImportScriptSetExitRequest(COLOSSEUM_SATRAGE_MATCH_TO_BABI_PALACE);
		break;
	}
	return 0;
}

/* The guide gives the party the tour of Colosseum Final 2
   (COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_WEST). */
static void scColosseumSatrageMatchIntroduceSecondFinals(Sint32 attendantId)
{
	Sint32 attendantZ;
	Sint32 attendantX;
	ACTOR *actor;

	actor = fdActorGet(attendantId);
	attendantX = actor->x.part[0].integer;
	attendantZ = actor->z.part[0].integer;
	fdScriptStart();
	fdActorSetSpeed(attendantId, 1);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 1);
	fdActorSetSpeed(ACTOR_GARET, 1);
	fdActorSetSpeed(ACTOR_IVAN, 1);
	fdActorSetSpeed(ACTOR_MIA, 1);
	fdActorSetPosFX(ACTOR_PARTY_LEADER, toFIXED(attendantX),
		(toFIXED(attendantZ) - toFIXED(48)));
	fdActorSetPosFX(ACTOR_GARET, toFIXED(attendantX) - toFIXED(16),
		toFIXED(attendantZ) - toFIXED(40));
	fdActorSetPosFX(ACTOR_IVAN, toFIXED(attendantX) + toFIXED(16),
		toFIXED(attendantZ) - toFIXED(40));
	fdActorSetPosFX(
		ACTOR_MIA, toFIXED(attendantX), (toFIXED(attendantZ) - toFIXED(32)));
	fdActorSetPosFX(
		attendantId, toFIXED(attendantX), (toFIXED(attendantZ) - toFIXED(80)));
	fdActorGet(ACTOR_PARTY_LEADER)->facing = FACING_NORTH;
	fdCameraFollowActor(ACTOR_PARTY_LEADER, 0);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionFrames();
	fdScriptSetMessageId(MsgColossoSiteSecondFinals);
	fdActorSetAnimationAndWait(attendantId, ANIM_NOD);
	/* "This is the site of the second finals match." */
	fdScriptShowMessage(attendantId, 0);
	fdActorRunRepeatedMotion(attendantId, 2);
	/* "As I explained earlier, each stage has its own attendant." */
	fdScriptShowMessage(attendantId, 0);
	fdActorRunRepeatedMotion(attendantId, 2);
	/* "Please ask the attendants directly for descriptions of each..." */
	fdScriptShowMessage(attendantId, 0);
	fdActorRunRepeatedMotion(attendantId, 2);
	/* "...Isaac, that first match was terrific. I'll be right here,..." */
	fdScriptShowMessage(attendantId, 0);
	fdActorSetAnimation(ACTOR_MIA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdScriptWait(6);
	fdActorSetAnimation(ACTOR_GARET, ANIM_WALK);
	actor = fdActorGet(ACTOR_PARTY_LEADER);
	if(actor != NULL) {
		fdActorSetDestination(
			ACTOR_GARET, actor->x.part[0].integer, actor->z.part[0].integer);
	}
	fdActorSetAnimation(ACTOR_IVAN, ANIM_WALK);
	actor = fdActorGet(ACTOR_PARTY_LEADER);
	if(actor != NULL) {
		fdActorSetDestination(
			ACTOR_IVAN, actor->x.part[0].integer, actor->z.part[0].integer);
	}
	fdActorSetAnimation(ACTOR_MIA, ANIM_WALK);
	actor = fdActorGet(ACTOR_PARTY_LEADER);
	if(actor != NULL) {
		fdActorSetDestination(
			ACTOR_MIA, actor->x.part[0].integer, actor->z.part[0].integer);
	}
	fdActorWalkToAndWait(attendantId, attendantX - 16, attendantZ - 64);
	fdActorSetPos(ACTOR_GARET, 0, 0);
	fdActorSetPos(ACTOR_IVAN, 0, 0);
	fdActorSetPos(ACTOR_MIA, 0, 0);
	fdActorWalkToAndWait(attendantId, attendantX - 16, attendantZ - 16);
	fdActorWalkToAndWait(attendantId, attendantX, attendantZ);
	fdActorFaceDirection(attendantId, FACING_NORTH, 10);
	fdScriptEnd();
}

/* The Mini-Maze's attendant explains stage 1 of Colosseum Final 2. */
static void scColosseumSatrageMatchTalkToMiniMazeAttendant(Sint32 attendant)
{
	Sint32 result;

	if(MainRoutineStateWork.entrance ==
		COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_EAST) {
		scColossoAskDoneCheering(attendant);
	} else {
		fdScriptStart();
		result = scColossoGreetAtStage(attendant, STAGE_MINI_MAZE);
		if(result == COLOSSO_GUIDE_DESCRIBE) {
			fdScriptSetMessageId(MsgColossoStageDubbedMini);
			fdCameraSetSpeed(3);
			fdCameraMoveTo(1224, CAMERA_KEEP, 184, 1);
			fdCameraWaitForMove();
			/* "This stage is dubbed the Mini-Maze." */
			fdScriptShowMessage(attendant, 0);
			scColossoFadeInWarrior(ACTOR_PARTY_LEADER, 1272, 168);
			fdActorSetSpeed(ACTOR_PARTY_LEADER, 1.5);
			scColossoWalkActorToAndStand(ACTOR_PARTY_LEADER, 1288, 184);
			scColossoWalkActorToAndStand(ACTOR_PARTY_LEADER, 1288, 216);
			scColossoWalkActorToAndStand(ACTOR_PARTY_LEADER, 1224, 216);
			fdScriptShowMessage(attendant, 0);
			scColossoWalkActorToAndStand(ACTOR_PARTY_LEADER, 1224, 248);
			scColossoWalkActorToAndStand(ACTOR_PARTY_LEADER, 1192, 248);
			fdScriptWait(3);
			fdActorGet(ACTOR_PARTY_LEADER)->velY = toFIXED(4);
			fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_ARMS_RAISED);
			fdActorSetAttachedEffect(ACTOR_PARTY_LEADER,
				ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
			fdScriptWait(30);
			fdScriptShowMessage(attendant, 0);
			scColossoRestoreWarrior(ACTOR_PARTY_LEADER);
			fdCameraFollowActor(ACTOR_PARTY_LEADER, 0);
			scColossoRepeatStageNum(attendant, STAGE_MINI_MAZE);
		} else if(result == COLOSSO_GUIDE_OFFER_CHEERING) {
			fdScriptSetMessageId(MsgColossoMainThingStage);
			/* "The main thing with this stage is to proceed through the..." */
			fdScriptShowMessage(attendant, 0);
		}
		scColossoOfferCheeringSpot(result, attendant, STAGE_MINI_MAZE);
		fdScriptEnd();
	}
}

/* The shifting floor stage's attendant explains stage 2 of Colosseum Final
   2. */
static void scColosseumSatrageMatchTalkToShiftingFloorAttendant(
	Sint32 attendant)
{
	Sint32 result;

	if(MainRoutineStateWork.entrance ==
		COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_EAST) {
		scColossoAskDoneCheering(attendant);
	} else {
		fdScriptStart();
		result = scColossoGreetAtStage(attendant, STAGE_SHIFTING_FLOOR);
		if(result == COLOSSO_GUIDE_DESCRIBE) {
			fdScriptSetMessageId(MsgColossoShiftingFloorStage);
			scColosseumSatrageMatchResetLifts();
			fdCameraSetSpeed(3);
			fdCameraMoveTo(984, CAMERA_KEEP, 232, 1);
			fdCameraWaitForMove();
			COLOSSEUM_SATRAGE_MATCH_SHOW_MESSAGE(attendant, 0);
			scColosseumSatrageMatchStartLifts();
			fdScriptShowMessage(attendant, 0);
			scColossoFadeInWarrior(ACTOR_PARTY_LEADER, 1080, 264);
			fdScriptWait(15);
			fdActorSetSpeed(ACTOR_PARTY_LEADER, 1.5);
			scColossoWalkActorToAndStand(ACTOR_PARTY_LEADER, 1080, 216);
			scColossoWalkActorToAndStand(ACTOR_PARTY_LEADER, 1064, 216);
			scColosseumSatrageMatchWaitForLiftsLinedUp();
			fdTryJumpForward();
			fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
			fdScriptShowMessage(attendant, 0);
			scColossoRestoreWarrior(ACTOR_PARTY_LEADER);
			fdCameraFollowActor(ACTOR_PARTY_LEADER, 0);
			scColossoRepeatStageNum(attendant, STAGE_SHIFTING_FLOOR);
		} else {
			if(result == COLOSSO_GUIDE_OFFER_CHEERING) {
				fdScriptSetMessageId(MsgColossoOperatorLiftsWillCheerFor);
				/* "The operator of the lifts will cheer for you here." */
				fdScriptShowMessage(attendant, 0);
			}
		}
		scColossoOfferCheeringSpot(result, attendant, STAGE_SHIFTING_FLOOR);
		fdScriptEnd();
	}
}

/* The Log-Rolling Stage's attendant explains stage 3 of Colosseum Final 2. */
static void scColosseumSatrageMatchTalkToLogRollingAttendant(Sint32 attendant)
{
	Sint32 result;

	if(MainRoutineStateWork.entrance ==
		COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_EAST) {
		scColossoAskDoneCheering(attendant);
	} else {
		fdScriptStart();
		result = scColossoGreetAtStage(attendant, STAGE_LOG_ROLLING);
		if(result == COLOSSO_GUIDE_DESCRIBE) {
			fdScriptSetMessageId(MsgColossoLogRollingStage);
			fdCameraSetSpeed(3);
			fdCameraMoveTo(752, CAMERA_KEEP, 192, 1);
			fdCameraWaitForMove();
			fdScriptWait(60);
			fdCameraSetSpeed(1);
			fdCameraMoveTo(752, CAMERA_KEEP, 224, 1);
			fdCameraWaitForMove();
			/* "This is the Log- Rolling Stage." */
			fdScriptShowMessage(attendant, 0);
			scColossoFadeInWarrior(ACTOR_PARTY_LEADER, 856, 264);
			fdScriptWait(10);
			fdActorSetSpeed(ACTOR_PARTY_LEADER, 1.5);
			scColossoWalkActorToAndStand(ACTOR_PARTY_LEADER, 856, 264);
			scColossoWalkActorToAndStand(ACTOR_PARTY_LEADER, 856, 232);
			fdScriptShowMessage(attendant, 0);
			scColossoWalkActorToAndStand(ACTOR_PARTY_LEADER, 840, 232);
			fdScriptWait(10);
			COLOSSEUM_SATRAGE_MATCH_MOVE_RIDING_LOG(ACTOR_RIDING_LOG_SOUTH,
				-COLOSSEUM_SATRAGE_MATCH_LOG_TRAVEL_PIXELS, 0);
			fdCameraMoveTo(752, CAMERA_KEEP, 216, 1);
			fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
			fdScriptWait(10);
			fdActorSetSpeed(ACTOR_PARTY_LEADER, 1);
			fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 760, 232);
			fdScriptWait(10);
			fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 30);
			fdScriptShowMessage(attendant, 0);
			scColossoRestoreWarrior(ACTOR_PARTY_LEADER);
			fdCameraFollowActor(ACTOR_PARTY_LEADER, 0);
			fdActorSetPos(ACTOR_RIDING_LOG_SOUTH, 840, 232);
			scColossoRepeatStageNum(attendant, STAGE_LOG_ROLLING);
		} else if(result == COLOSSO_GUIDE_OFFER_CHEERING) {
			fdScriptSetMessageId(MsgColossoHereYourObjectiveRideLogs);
			/* "Here, your objective is to ride the logs as quickly as
			   possible." */
			fdScriptShowMessage(attendant, 0);
		}
		scColossoOfferCheeringSpot(result, attendant, STAGE_LOG_ROLLING);
		fdScriptEnd();
	}
}

/* The Wall's attendant explains stage 4 of Colosseum Final 2. */
static void scColosseumSatrageMatchTalkToWallAttendant(Sint32 attendant)
{
	ACTOR *actor;
	Sint32 result;

	if(MainRoutineStateWork.entrance ==
		COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_EAST) {
		scColossoAskDoneCheering(attendant);
		return;
	}
	fdScriptStart();
	result = scColossoGreetAtStage(attendant, STAGE_WALL);
	if(result == COLOSSO_GUIDE_DESCRIBE) {
		fdScriptSetMessageId(MsgColossoFansJustCallWall);
		fdCameraSetSpeed(3);
		fdCameraMoveTo(536, CAMERA_KEEP, 240, 1);
		fdCameraWaitForMove();
		fdScriptWait(45);
		fdCameraSetSpeed(1);
		fdCameraMoveTo(536, CAMERA_KEEP, 192, 1);
		fdCameraWaitForMove();
		/* "The fans just call this The Wall." */
		fdScriptShowMessage(attendant, 0);
		scColossoFadeInWarrior(ACTOR_PARTY_LEADER, 632, 264);
		fdActorSetSpeed(ACTOR_PARTY_LEADER, 1);
		fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 616, 264);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
		fdClimbUp();
		fdCameraSetSpeed(0.25);
		fdCameraMoveTo(536, CAMERA_KEEP, 160, 1);
		fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.5);
		fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_CLIMB);
		actor = fdActorGet(ACTOR_PARTY_LEADER);
		syFieldActorSetMoveTargetFX(actor, actor->x.fixed,
			actor->y.fixed + toFIXED(64), actor->z.fixed);
		fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
		fdClimbUp();
		fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
		fdScriptShowMessage(attendant, 0);
		fdActorSetSpeed(ACTOR_PARTY_LEADER, 1.5);
		scColossoWalkActorToAndStand(ACTOR_PARTY_LEADER, 488, 248);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 20);
		syFieldActorJumpDownLedge(ACTOR_PARTY_LEADER, 6, 0);
		fdCameraMoveTo(536, CAMERA_KEEP, 160, 1);
		fdScriptShowMessage(attendant, 0);
		scColossoRestoreWarrior(ACTOR_PARTY_LEADER);
		fdCameraFollowActor(ACTOR_PARTY_LEADER, 0);
		scColossoRepeatStageNum(attendant, STAGE_WALL);
	} else if(result == COLOSSO_GUIDE_OFFER_CHEERING) {
		fdScriptSetMessageId(MsgColossoScalingWallQuickly);
		/* "Scaling the wall quickly is priority one in this stage." */
		fdScriptShowMessage(attendant, 0);
	}
	scColossoOfferCheeringSpot(result, attendant, STAGE_WALL);
	fdScriptEnd();
}

/* In the Board Walk attendant's demonstration, Isaac leans on a log and it
   rolls west to (x, z) in pixels, with Isaac walking after it half the way. */
static void scColosseumSatrageMatchDemonstrateLogPush(
	Sint32 obstacleId, Sint32 x, Sint32 z)
{
	Sint32 pusher = MainRoutineStateWork.leaderActorId;
	ACTOR *pusherActor = fdActorGet(pusher);
	ACTOR *obstacle = fdActorGet(obstacleId);
	/* The test is scColosseumSatrageMatchRollLogToMetatile's, made for half
	   metatiles:
	   given pixels it always holds, so the push is along x, as this one is. */
	Sint32 alongX = FIELD_METATILE(obstacle->x.fixed) != x / 2;
	FIXED  offsetX;
	FIXED  offsetZ;

	x <<= 16;
	z <<= 16;
	if(alongX) {
		offsetX = (x - obstacle->x.fixed) / 2;
		offsetZ = 0;
	} else {
		offsetX = 0;
		offsetZ = (z - obstacle->z.fixed) / 2;
	}
	fdActorSetAnimation(pusher, ANIM_PUSH);
	fdScriptWait(6);
	obstacle->speedLimit = toFIXED(1) / 2;
	obstacle->acceleration = toFIXED(1) / 5;
	sySndPlay(SE_HEAVY_MOVE);
	fdActorSelectAnimation(obstacle, LOG_ANIMATION_ROLL_WEST);
	syFieldActorSetMoveTargetFX(obstacle, x, 0, z);
	fdScriptWait(6);
	fdActorSetAnimation(pusher, ANIM_WALK);
	fdActorSetSpeedAndAcceleration(pusher, 0.5, 0.2);
	fdActorSelectAnimation(pusherActor, ANIM_WALK);
	syFieldActorSetMoveTargetFX(pusherActor, pusherActor->x.fixed + offsetX, 0,
		pusherActor->z.fixed + offsetZ);
	fdActorWaitForMove(pusherActor);
	fdActorSelectAnimation(pusherActor, ANIM_STAND);
	fdActorWaitForMove(obstacle);
	fdActorSelectAnimation(obstacle, LOG_ANIMATION_AT_REST);
	sySndPlay(SE_STOP_ACTION_SE);
	sySndPlay(SE_HEAVY_STOP);
	fdScriptWait(15);
}

/* The Board Walk's attendant explains stage 5 of Colosseum Final 2. */
static void scColosseumSatrageMatchTalkToBoardWalkAttendant(Sint32 attendant)
{
	Sint32 result;

	if(MainRoutineStateWork.entrance ==
		COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_EAST) {
		scColossoAskDoneCheering(attendant);
		return;
	}
	fdScriptStart();
	result = scColossoGreetAtStage(attendant, STAGE_BOARD_WALK);
	if(result == COLOSSO_GUIDE_DESCRIBE) {
		fdScriptSetMessageId(MsgColossoPlaceCalledBoard);
		fdCameraSetSpeed(2);
		fdCameraMoveTo(328, CAMERA_KEEP, 264, 1);
		fdCameraWaitForMove();
		fdScriptWait(30);
		fdCameraSetSpeed(1.5);
		fdCameraMoveTo(312, CAMERA_KEEP, 176, 1);
		fdCameraWaitForMove();
		/* "This place is called the Board Walk." */
		fdScriptShowMessage(attendant, 0);
		scColossoFadeInWarrior(ACTOR_PARTY_LEADER, 408, 264);
		fdActorSetSpeed(ACTOR_PARTY_LEADER, 1.5);
		scColossoWalkActorToAndStand(ACTOR_PARTY_LEADER, 408, 216);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 10);
		fdScriptShowMessage(attendant, 0);
		scColosseumSatrageMatchDemonstrateLogPush(
			ACTOR_ROLLING_LOG_EAST, 360, 208);
		fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_SMILE, 45);
		fdActorSetSpeed(ACTOR_PARTY_LEADER, 1.5);
		scColossoWalkActorToAndStand(ACTOR_PARTY_LEADER, 376, 216);
		scColossoWalkActorToAndStand(ACTOR_PARTY_LEADER, 376, 248);
		scColossoWalkActorToAndStand(ACTOR_PARTY_LEADER, 312, 248);
		fdScriptShowMessage(attendant, 0);
		scColossoRestoreWarrior(ACTOR_PARTY_LEADER);
		fdCameraFollowActor(ACTOR_PARTY_LEADER, 0);
		fdActorSetPos(ACTOR_ROLLING_LOG_EAST, 392, 208);
		scColossoRepeatStageNum(attendant, STAGE_BOARD_WALK);
	} else if(result == COLOSSO_GUIDE_OFFER_CHEERING) {
		fdScriptSetMessageId(MsgColossoFocusRollingLogs);
		/* "Your focus here will be rolling the logs to form a path." */
		fdScriptShowMessage(attendant, 0);
	}
	scColossoOfferCheeringSpot(result, attendant, STAGE_BOARD_WALK);
	fdScriptEnd();
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

/* Colosseum Final 2's own copy of scColossoStartRaceGauge
   (shared/colosso_setup.c), which passes the gauge function's address as it
   is. */
void scColosseumSatrageMatchStartRaceGauge(Sint32 leaderId, Sint32 opponentId,
	Sint32 segments, Sint32 finishX, FIXED finishZ, Sint32 firstItem,
	Sint32 secondItem)
{
	COLOSSO_RACE_STATE *state;
	ACTOR			   *leader;
	ACTOR			   *opponent;
	void			   *graphics;
	Sint32				vramSlot;

	state = syHeapSlotAllocEWRAM(
		HEAP_SLOT_COLOSSO_RACE, sizeof(COLOSSO_RACE_STATE));
	graphics = syHeapAllocEWRAM(512);

	state->gauge.playerActorId = leaderId;
	state->gauge.opponentActorId = opponentId;
	state->gauge.finishItemActorIdA = firstItem;
	state->gauge.finishItemActorIdB = secondItem;
	state->gauge.segments = segments;
	state->gauge.finishX = finishX;
	state->gauge.finishZ = finishZ;

	leader = fdActorGet(leaderId);
	opponent = fdActorGet(opponentId);

	/* Unless the party returns to where it stood, the opponent starts level
	   with the leader, as far on the other side of the finish. */
	if(gmFlagGet(FLAG_KEEP_PARTY_POSITION) == 0) {
		opponent->x.fixed = (finishX << 1) - leader->x.fixed;
		opponent->z.fixed = leader->z.fixed;
	}

	state->gauge.level = 0;
	state->gauge.hold = 0;

	syFieldDecompressTaggedLZ(Colosso_GaugeGraphics, graphics);

	vramSlot = syVRAMCacheFindFreeEntry();
	state->gauge.vramBlock = vramSlot;
	syVRAMCacheLoad((Sint16)vramSlot, 512, graphics);

	sySetFrameEvent(scColossoDrawGauge, EVENT_PASS_FRAME - 10);

	syHeapRelease(graphics);
}

/* Decodes the opponent's recorded path and starts its playback event. */
static void scColosseumSatrageMatchStartOpponentPath(Sint32 pathResource)
{
	COLOSSO_RACE_STATE *state = HEAP_SLOT_BLOCK(HEAP_SLOT_COLOSSO_RACE);
	PATH_RECORDER	   *recorder = (PATH_RECORDER *)SceneLocalState;

	syFieldDecompressTaggedLZ(
		syResourceGetTableEntry(pathResource), state->path);
	if(gmFlagGet(FLAG_KEEP_PARTY_POSITION) == 0) {
		recorder->mode = PATH_RECORDER_PLAY;
		recorder->mirror = 1;
		recorder->actorId = state->gauge.opponentActorId;
		recorder->still = 0;
		recorder->pathIdx = 0;
	}
	sySetFrameEvent(scColossoUpdatePathOpponent, EVENT_PASS_FRAME + 5);
}

#include "shared/colosso_map_state.c"
#include "shared/actor_find_at_position.c"

#include "shared/colosso_push.c"

/* The chests have been opened: a Nut on the Board Walk, in the narrow channel
   north of its pool, metatile (23, 7); a Smoke Bomb at the foot of The Wall,
   metatile (34, 16); and a Nut on the shifting floor stage, metatile (56,
   7). */
#define FLAG_COLOSSEUM_SATRAGE_MATCH_BOARD_WALK_NUT_CHEST_OPENED \
	FLAG_AREA_TEMP_38
#define FLAG_COLOSSEUM_SATRAGE_MATCH_SMOKE_BOMB_CHEST_OPENED FLAG_AREA_TEMP_39
#define FLAG_COLOSSEUM_SATRAGE_MATCH_SHIFTING_FLOOR_NUT_CHEST_OPENED \
	FLAG_AREA_TEMP_3B

/* The chests' metatiles, as the flags above. */
enum {
	METATILE_BEHAVIOR_BOARD_WALK_NUT_CHEST = 101,
	METATILE_BEHAVIOR_SMOKE_BOMB_CHEST = 102,
	METATILE_BEHAVIOR_SHIFTING_FLOOR_NUT_CHEST = 104
};

/* The rolling log's animation for each quarter turn of the way it is pushed. */
const Uint8 ColosseumSatrageMatch_RollLogScript[] = {2, 2, 3, 3};

#include "shared/colosso_lettering_tables.c"
/* clang-format off */

char HexDigits[20] = "0123456789ABCDEF";

#include "shared/colosso_prize_scripts.c"

#include "shared/pushed_actor_direction_steps.c"

/* No exit leads to entrance 10, on the Board Walk. */
SCENE_ENTRANCE ColosseumSatrageMatchEntrances[] = {
	{ COLOSSEUM_SATRAGE_MATCH_RACE_FROM_LEND_A_HAND, CONDITION_ALWAYS, 1272, 0, 168, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_EAST, CONDITION_ALWAYS, 1224, 0, 488, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ 10, CONDITION_ALWAYS, 384, 0, 264, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

Uint32 ColosseumSatrageMatchExits[] = {
	SCENE_EXITS(ColosseumSatrageMatch),
	SCENE_EXIT(COLOSSEUM_SATRAGE_MATCH_LEND_A_HAND_TO_RACE, ColosseumSatrageMatch, COLOSSEUM_SATRAGE_MATCH_RACE_FROM_LEND_A_HAND),
	SCENE_EXIT(COLOSSEUM_SATRAGE_MATCH_TO_COLOSSEUM_SOUTH, Colosseum, COLOSSEUM_FROM_COLOSSEUM_SATRAGE_MATCH_SOUTH),
	SCENE_EXIT(COLOSSEUM_SATRAGE_MATCH_TO_COLOSSEUM_NORTH, Colosseum, COLOSSEUM_FROM_COLOSSEUM_SATRAGE_MATCH_NORTH),
	SCENE_EXIT(COLOSSEUM_SATRAGE_MATCH_TO_BABI_PALACE, BabiPalace, BABI_PALACE_FROM_COLOSSEUM_AZART_MATCH),
	SCENE_EXITS_END,
};

ACTOR_PLACE ColosseumSatrageMatchActorPlacements[] = {
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

/* Set while the lifts stand in their broken pattern rather than in a line. */
Sint32 ColosseumSatrageMatch_LiftsApart = 0;

/* The frames into the lifts' cycle */
/* (COLOSSEUM_SATRAGE_MATCH_LIFT_CYCLE_FRAMES). */
Uint32 ColosseumSatrageMatch_LiftTimer = 0;

CUE ColosseumSatrageMatchCues[] = {
	{ STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_BEHAVIOR_WALL_POST, CONDITION_ALWAYS, SCRIPT_REF(scColosseumSatrageMatchJumpDownFromPost) },
	{ STEP_CUE_WALKING_INTO(FACING_WEST), METATILE_BEHAVIOR_WALL_POST, CONDITION_ALWAYS, SCRIPT_REF(scColosseumSatrageMatchHopWestFromPost) },
	{ STEP_CUE_WALKING_INTO(FACING_WEST), METATILE_BEHAVIOR_EAST_LOG_EAST_SIDE, CONDITION_ALWAYS, SCRIPT_REF(scColosseumSatrageMatchRollEastLogWest) },
	{ STEP_CUE_WALKING_INTO(FACING_EAST), METATILE_BEHAVIOR_EAST_LOG_WEST_SIDE, CONDITION_ALWAYS, SCRIPT_REF(scColosseumSatrageMatchRollEastLogEast) },
	{ STEP_CUE_WALKING_INTO(FACING_WEST), METATILE_BEHAVIOR_WEST_LOG_EAST_SIDE, CONDITION_ALWAYS, SCRIPT_REF(scColosseumSatrageMatchRollWestLogWest) },
	{ STEP_CUE_WALKING_INTO(FACING_EAST), METATILE_BEHAVIOR_WEST_LOG_WEST_SIDE, CONDITION_ALWAYS, SCRIPT_REF(scColosseumSatrageMatchRollWestLogEast) },
	{ STEP_CUE_WALKING, METATILE_BEHAVIOR_RIDING_LOG, CONDITION_ALWAYS, SCRIPT_REF(scColosseumSatrageMatchRideLog) },
	{ STEP_CUE_WALKING, METATILE_BEHAVIOR_BOARD_WALK_LEDGE, CONDITION_ALWAYS, SCRIPT_REF(scColosseumSatrageMatchPushBoardWalkPillar) },
	{ STEP_CUE_WALKING, METATILE_BEHAVIOR_WALL_LEDGE, CONDITION_ALWAYS, SCRIPT_REF(scColosseumSatrageMatchPushWallPillar) },
	{ STEP_CUE_WALKING, METATILE_BEHAVIOR_WALL_LEDGE_UNUSED, CONDITION_ALWAYS, SCRIPT_REF(scColosseumSatrageMatchPushWallPillar) },
	{ CUE_STEP, METATILE_BEHAVIOR_ARENA, FLAG_COLOSSEUM_ARENA_CLOSED, SCRIPT_REF(scColosseumSatrageMatchReachArena) },
	{ EXAMINE_CUE(EXAMINE_CHEST), METATILE_BEHAVIOR_BOARD_WALK_NUT_CHEST, FLAG_COLOSSEUM_SATRAGE_MATCH_BOARD_WALK_NUT_CHEST_OPENED, FIND_ITEM(ITEM_NUT) },
	{ EXAMINE_CUE(EXAMINE_CHEST) | CUE_FLAG_FACING | FACING_WEST, METATILE_BEHAVIOR_SMOKE_BOMB_CHEST, FLAG_COLOSSEUM_SATRAGE_MATCH_SMOKE_BOMB_CHEST_OPENED, FIND_ITEM(ITEM_SMOKE_BOMB) },
	{ EXAMINE_CUE(EXAMINE_CHEST), METATILE_BEHAVIOR_SHIFTING_FLOOR_NUT_CHEST, FLAG_COLOSSEUM_SATRAGE_MATCH_SHIFTING_FLOOR_NUT_CHEST_OPENED, FIND_ITEM(ITEM_NUT) },
	{ PSYNERGY_PHASE_ON_ACTOR(ABILITY_FROST, PSYNERGY_PHASE_ACTS), ACTOR_PUDDLE_WEST, FLAG_WEST_PUDDLE_FROZEN, SCRIPT_REF(scColosseumSatrageMatchFreezeWestPuddle) },
	{ PSYNERGY_PHASE_ON_ACTOR(ABILITY_FROST, PSYNERGY_PHASE_ACTS), ACTOR_PUDDLE_EAST, FLAG_EAST_PUDDLE_FROZEN, SCRIPT_REF(scColosseumSatrageMatchFreezeEastPuddle) },
	{ PSYNERGY_PHASE_ON_ACTOR(ABILITY_FORCE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FORCE_TARGET, FLAG_LOG_KNOCKED_DOWN, SCRIPT_REF(scColosseumSatrageMatchKeepLogStillForForce) },
	{ PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_FORCE_TARGET, FLAG_LOG_KNOCKED_DOWN, SCRIPT_REF(scColosseumSatrageMatchKnockDownLog) },
	{ PSYNERGY_ON_ACTOR(ABILITY_GROWTH), ACTOR_GROWTH_PLANT, FLAG_PLANT_GROWN, SCRIPT_REF(scColosseumSatrageMatchGrowPlant) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_BOARD_WALK_WEST_PILLAR, CONDITION_ALWAYS, SCRIPT_REF(scColosseumSatrageMatchIgnoreMove) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_BOARD_WALK_EAST_PILLAR, CONDITION_ALWAYS, SCRIPT_REF(scColosseumSatrageMatchIgnoreMove) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_WALL_PILLAR, CONDITION_ALWAYS, SCRIPT_REF(scColosseumSatrageMatchMoveWallPillar) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_MINI_MAZE_FIRST_PILLAR, CONDITION_ALWAYS, SCRIPT_REF(scColosseumSatrageMatchMoveMiniMazePillar) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_MINI_MAZE_FIRST_PILLAR + 1, CONDITION_ALWAYS, SCRIPT_REF(scColosseumSatrageMatchMoveMiniMazePillar) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_MINI_MAZE_FIRST_PILLAR + 2, CONDITION_ALWAYS, SCRIPT_REF(scColosseumSatrageMatchMoveMiniMazePillar) },
	{ PSYNERGY_ON_ACTOR(ABILITY_HALT), ACTOR_LIFT_OPERATOR, FLAG_LIFTS_HALTED, SCRIPT_REF(scColosseumSatrageMatchHaltLifts) },
	{ CUE_TALK, ACTOR_GUIDE, CONDITION_ALWAYS, SCRIPT_REF(scColossoAskDoneLookingAround) },
	{ CUE_TALK, ACTOR_MINI_MAZE_ATTENDANT, CONDITION_ALWAYS, SCRIPT_REF(scColosseumSatrageMatchTalkToMiniMazeAttendant) },
	{ CUE_TALK, ACTOR_SHIFTING_FLOOR_ATTENDANT, CONDITION_ALWAYS, SCRIPT_REF(scColosseumSatrageMatchTalkToShiftingFloorAttendant) },
	{ CUE_TALK, ACTOR_LOG_ROLLING_ATTENDANT, CONDITION_ALWAYS, SCRIPT_REF(scColosseumSatrageMatchTalkToLogRollingAttendant) },
	{ CUE_TALK, ACTOR_WALL_ATTENDANT, CONDITION_ALWAYS, SCRIPT_REF(scColosseumSatrageMatchTalkToWallAttendant) },
	{ CUE_TALK, ACTOR_BOARD_WALK_ATTENDANT, CONDITION_ALWAYS, SCRIPT_REF(scColosseumSatrageMatchTalkToBoardWalkAttendant) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GUIDE, CONDITION_ALWAYS, MSGNO(MsgColossoRiverThisYearsFinalsAreIncredibleThis) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MINI_MAZE_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoWallICantBelieveAnybodyEverGets) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SHIFTING_FLOOR_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoWallHowDoTheyMakeThatFloor) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LOG_ROLLING_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoWallImAmazedPeopleCanActuallyCross) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WALL_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoWallBoyThatBitOverThereMust) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOARD_WALK_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoWallIHeardBabiGotTheLog) },
	{ CUE_RAISED, COLOSSO_TAKE_PLACE_CUE, CONDITION_ALWAYS,
	  SCRIPT_REF(scColossoTakePlaceForMatch) },
	{ CUE_TABLE_END },
};

Sint16 Colosso_LetteringSlot = -1;

#include "shared/colosso_lettering_scripts.c"
/* clang-format on */

Sint16 Colosso_PointerSlot = -1;

#include "shared/colosso_finish_scripts.c"

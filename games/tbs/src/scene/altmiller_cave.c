/* @file */
/* Altmiller Cave, north of Tolbi, where the party finds Babi while all of */
/* Tolbi searches for him during Colosso (babi_palace.c). Its six scenes share
 */
/* this overlay and the area name Altmiller Cave. In the order the party goes */
/* through them: */
/* */
/* - The winding passage in from the cave mouth (`AltmillerCaveEntrance`). It */
/*   grows darker in steps as the party walks in, until only a circle of light
 */
/*   around them is bright. At its end a hole leads into a small chamber, where
 */
/*   a torch burns beside the doorway down. */
/* - The big dark room below (`AltmillerCaveBabi`). Near its middle an old man
 */
/*   lies on the floor, seen only as a white outline: the invisible Babi */
/*   (scAltmillerCaveMeetInvisibleBabi). He asks the party to fetch his draught
 */
/*   from the bottom of the cave and tells them the order of the five rocks' */
/*   colors. The chest on the room's east side is a Mimic. */
/* - Two dark rooms with a chest each: the Vial (`AltmillerCaveVialChest`) */
/*   and, further down, the Cookie (`AltmillerCaveCookieChest`). */
/* - Between them, a lit hall with a brick floor (`AltmillerCaveSquall`): */
/*   three fallen columns to push, a puddle Frost turns into an ice pillar to */
/*   stop them, Squall, a Jupiter Djinni, in the north-west corner, and a */
/*   stone pillar on a ledge to push into the pit before the way on. */
/* - The lit rooms at the bottom (`AltmillerCaveDraught`): a hall of three more
 */
/*   fallen columns, the room of the five rocks (scAltmillerCaveTurnRock), and
 */
/*   behind the wall the rocks open, the Mystic Draught's chest on a stone */
/*   table. */
/* */
/* Back in Babi's room with the draught, the party watches him drink it and */
/* appear, and two men from Tolbi come to take him home */
/* (scAltmillerCaveRestoreBabiAndInviteParty). Saving him (FLAG_BABI_SAVED) is
 */
/* the story flag the rest of Tolbi waits on. Everything else the cave */
/* remembers is kept in scene temp flags and SceneLocalState, so the puzzles */
/* start over on every ordinary arrival. */
/* */
/* The rooms share one entrance table and one exit table, and the other */
/* getters choose by MainRoutineStateWork.next. scInit sends an entry */
/* from outside the cave (entrance 0) to the room's first entrance, darkens the
 */
/* dark rooms and puts back what the party has done. */
/* */
/* The darkness is BG1 laid over the rest (ALTMILLER_CAVE_DARKNESS_BLEND), and
 */
/* the light around the party is a display window, an ellipse around the */
/* leader inside which the layers are drawn differently */
/* (scAltmillerCaveInitDarkness). In the dark rooms BG1 covers everything */
/* outside the light, so the room is black there but for the glow of */
/* the torches on its walls, and inside it BG1 only dims the ground. */
/* */
/* The passage in darkens by steps instead */
/* (FLAG_ALTMILLER_CAVE_DARKENS_BY_STEPS). Six kinds of cell along it each set
 */
/* a darkness level, kept in the cave's state: at the mouth the passage is */
/* fully lit, and five lines further in each make everything outside the light
 */
/* a little dimmer (scAltmillerCaveApplyFadeBlend). Where one level meets the */
/* next a cell of each lies side by side, so the level is always that of the */
/* last cell stepped on, whichever way the party walks. */
/* */
/* Reveal lights the passage in while it lasts and shows the rocks' colors */
/* (scAltmillerCaveStartReveal, scAltmillerCaveEndReveal). */
/* */
/* The rock puzzle keeps the rounds matched, its state and the light at the top
 */
/* in the cave's state. Turning the leftmost rock and then the rightmost brings
 */
/* out a ring of five lights above the rocks. The ring spins */
/* (scAltmillerCaveSpinLights, a task) and stops with one light at the top. */
/* Turning that light's rock spins it on to a different light. A wrong rock */
/* puts the lights away. The third match bursts the wall behind the rocks open
 */
/* (scAltmillerCaveReleaseWallBurst). Coming up to the rocks from the hall of */
/* fallen columns (ALTMILLER_CAVE_DRAUGHT_ROCKS_FROM_PILLAR_CAVE) starts the */
/* puzzle over. */

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "macros.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/actor_script.h"
#include "field/event_inline.h"
#include "field/field_psynergy_state.h"
#include "graphics/display_transition.h"
#include "graphics/graphics.h"
#include "scene/walk_into_leader.h"
#include "text/msg_ids.h"

struct LIGHT_WHEEL;

extern struct LIGHT_WHEEL *AltmillerCave_LightWheel;
extern SCENE_ENTRANCE AltmillerCave_Entrances[];
extern u32 AltmillerCave_Exits[];
extern ACTOR_PLACE AltmillerCave_ActorPlacementsOther[];
extern ACTOR_PLACE AltmillerCave_BabiActorPlacements[];
extern ACTOR_PLACE AltmillerCave_SquallActorPlacements[];
extern ACTOR_PLACE AltmillerCave_DraughtActorPlacements[];
extern u32 AltmillerCave_PulseScales[];
extern s32 AltmillerCave_GuardsArrivePath[];
extern s32 AltmillerCave_SecondGuardLeavePath[];
extern s32 AltmillerCave_FirstGuardLeavePath[];
extern s32 AltmillerCave_BabiLeavePath[];
extern TRIGGER AltmillerCave_TriggersOther[];
extern TRIGGER AltmillerCave_BabiTriggers[];
extern TRIGGER AltmillerCave_VialChestTriggers[];
extern TRIGGER AltmillerCave_SquallTriggers[];
extern TRIGGER AltmillerCave_CookieChestTriggers[];
extern TRIGGER AltmillerCave_DraughtTriggers[];

/* The actors of Babi's room: Babi while invisible and once seen, and the two */
/* men from Tolbi who come looking for him. Garet, Ivan and Mia are placed */
/* first, in their own party slots. */
enum {
	/* Babi while invisible (SPRITE_INVISIBLE_BABI), seen only as a white */
	/* outline lying on the floor near the middle of his room. */
	ACTOR_INVISIBLE_BABI = ACTOR_FIRST_PLACED,
	/* SPRITE_BABI, the same old man once he has drunk the draught. */
	ACTOR_BABI,
	/* Their sprite is a helmeted soldier carrying a torch */
	/* (SPRITE_SOLDIER_WITH_TORCH). */
	ACTOR_FIRST_SOLDIER,
	ACTOR_SECOND_SOLDIER
};

/* Squall's hall and the hall at the bottom place first the three fallen */
/* columns the party pushes. Squall's hall then places its puddle, the stone */
/* pillar on the ledge and Squall, a Jupiter Djinni. */
enum {
	ACTOR_FIRST_FALLEN_COLUMN = ACTOR_FIRST_PLACED,
	ACTOR_SQUALL_ROOM_PUDDLE = ACTOR_FIRST_FALLEN_COLUMN + 3,
	ACTOR_SQUALL_ROOM_PILLAR,
	ACTOR_SQUALL
};

/* The rock room, after the hall's three fallen columns: the five colored */
/* lights and the five rocks, each in Babi's order, the rocks left to right. */
enum {
	ACTOR_FIRST_LIGHT = ACTOR_FIRST_FALLEN_COLUMN + 3,
	ACTOR_BLUE_LIGHT = ACTOR_FIRST_LIGHT,
	ACTOR_GREEN_LIGHT,
	ACTOR_WHITE_LIGHT,
	ACTOR_YELLOW_LIGHT,
	ACTOR_RED_LIGHT,
	ACTOR_FIRST_ROCK,
	ACTOR_BLUE_ROCK = ACTOR_FIRST_ROCK,
	ACTOR_GREEN_ROCK,
	ACTOR_WHITE_ROCK,
	ACTOR_YELLOW_ROCK,
	ACTOR_RED_ROCK
};

/* The lights, one of each colour. */
enum { LIGHT_COUNT = 5 };

/* The colour mode that draws a light or a rock in each of Babi's five colors,
 */
/* a shuffle or tint of the sprite's own colours, and greyscale for a rock */
/* while Reveal is off. */
enum {
	COLOR_BLUE = SPRITE_COLOR_GRB,
	COLOR_GREEN = SPRITE_COLOR_GBR,
	COLOR_WHITE = SPRITE_COLOR_GREEN_TINT,
	COLOR_YELLOW = SPRITE_COLOR_RBG,
	COLOR_RED = SPRITE_COLOR_BGR,
	COLOR_NONE = SPRITE_COLOR_GREY
};

/* The scene temp flags the cave keeps. */
enum {
	/* The room is dark. It is set on entry everywhere but Squall's hall and the
	 */
	/* lit rooms at the bottom. */
	FLAG_ALTMILLER_CAVE_DARK = FLAG_SCENE_TEMP_00,
	/* The passage in from the cave mouth darkens by steps as the party walks
	   in. */
	FLAG_ALTMILLER_CAVE_DARKENS_BY_STEPS = FLAG_SCENE_TEMP_01,
	/* Frost has turned the puddle in Squall's hall into an ice pillar. */
	FLAG_ALTMILLER_CAVE_PUDDLE_FROZEN = FLAG_SCENE_TEMP_11,
	/* The stone pillar in Squall's hall has dropped into the pit before the way
	 */
	/* on. Only the first patch on keeps it: the pit's cell is then filled again
	 */
	/* when the party comes back, though the pillar is drawn back on its ledge
	 */
	/* either way. */
	FLAG_ALTMILLER_CAVE_PILLAR_IN_PLACE = FLAG_SCENE_TEMP_12
};

/* The cell behaviors the rooms' trigger tables answer, besides their exits, */
/* whose cells carry the exit's own number. Each is a cell of the map named */
/* here. */
typedef enum ALTMILLER_CAVE_TRIGGER_CELL {
	/* The passage in: the first bend north of the cave mouth, on the mouth's */
	/* side, cells (27, 19) to (27, 21). The passage is fully lit. */
	CELL_BEHAVIOR_ALTMILLER_CAVE_FULL_LIGHT = 60,
	/* Each further line across the passage, on its far side: the first bend */
	/* (cells (28, 19) and (28, 20)) and the way north (cell (26, 10)). */
	CELL_BEHAVIOR_ALTMILLER_CAVE_DARKNESS_1 = 61,
	/* North up the passage (cell (26, 9)), and where it turns west along the */
	/* north wall (cells (18, 4) and (18, 5)). */
	CELL_BEHAVIOR_ALTMILLER_CAVE_DARKNESS_2 = 62,
	/* Along the north wall (cells (17, 4) and (17, 5)), and where the passage
	 */
	/* turns south in the north-west (cells (7, 8) and (7, 9)). */
	CELL_BEHAVIOR_ALTMILLER_CAVE_DARKNESS_3 = 63,
	/* The north-west turn (cells (6, 8) and (6, 9)), and down the west side */
	/* (cells (7, 20) to (9, 20)). */
	CELL_BEHAVIOR_ALTMILLER_CAVE_DARKNESS_4 = 64,
	/* The last stretch before the hole into the torch-lit chamber, cells (8,
	   21) */
	/* to (10, 21). */
	CELL_BEHAVIOR_ALTMILLER_CAVE_DARKNESS_5 = 65,
	/* Babi's room: the floor just west of where he lies, on the way to the */
	/* stairs down, cells (11, 9) to (13, 9). */
	CELL_BEHAVIOR_ALTMILLER_CAVE_BESIDE_BABI = 19,
	/* The floor of the fallen columns: in Squall's hall cells (19, 11) to */
	/* (26, 19), and in the hall at the bottom cells (23, 24) to (27, 32). */
	CELL_BEHAVIOR_ALTMILLER_CAVE_DRAUGHT_COLUMN_FLOOR = 70,
	CELL_BEHAVIOR_ALTMILLER_CAVE_SQUALL_COLUMN_FLOOR = 71,
	/* Squall's hall: the four cells around the puddle at (23, 17), north, */
	/* east, south and west of it. */
	CELL_BEHAVIOR_ALTMILLER_CAVE_NORTH_OF_PUDDLE = 72,
	CELL_BEHAVIOR_ALTMILLER_CAVE_EAST_OF_PUDDLE = 73,
	CELL_BEHAVIOR_ALTMILLER_CAVE_SOUTH_OF_PUDDLE = 74,
	CELL_BEHAVIOR_ALTMILLER_CAVE_WEST_OF_PUDDLE = 75,
	/* Squall's hall: the ledge just west of the stone pillar, cell (28, 20). */
	CELL_BEHAVIOR_ALTMILLER_CAVE_WEST_OF_PILLAR = 77,
	/* The chest in each room that has one: the Mimic in Babi's room (cell */
	/* (28, 36)), the Vial (cell (25, 13)), the Cookie (cell (12, 8)) and the */
	/* Mystic Draught on its table (cell (11, 7)). */
	CELL_BEHAVIOR_ALTMILLER_CAVE_CHEST = 100
} ALTMILLER_CAVE_TRIGGER_CELL;

/* The darkness: BG1 blended over every layer, at the weights */
/* BLEND_ALPHA gives each level. */
#define ALTMILLER_CAVE_DARKNESS_BLEND                                          \
	(BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL)
/* A dark room's weights, which the passage's levels do not use. */
#define ALTMILLER_CAVE_DARK_ALPHA BLEND_ALPHA(4, 12)

/* The light around the party is a window, an ellipse a little taller than */
/* wide around the leader (WINDOW_TRANSITION_LEADER_LOW); the transition's */
/* progress sets its size, 5 pixels to each side of the leader for each step. */
/* The light's progress: 25 pixels to each side of the leader. */
#define ALTMILLER_CAVE_LIGHT_STEPS 5
/* The light's progress once Babi is found: wide enough to fill the screen. */
#define ALTMILLER_CAVE_LIGHT_FULL 32
/* Inside the light where the passage darkens by steps: every layer */
/* but BG1, with no blend, so the party's surroundings show at full */
/* brightness. */
#define ALTMILLER_CAVE_STEPS_LIGHT_WININ                                       \
	(WININ_WIN0_BG0 | WININ_WIN0_BG2 | WININ_WIN0_BG3 | WININ_WIN0_OBJ |       \
	 WININ_WIN1_BG0 | WININ_WIN1_BG2 | WININ_WIN1_BG3 | WININ_WIN1_OBJ)
/* Outside that light: everything, blended to the step's darkness. */
#define ALTMILLER_CAVE_STEPS_LIGHT_WINOUT WINOUT_WIN01_ALL
/* Inside the light of an evenly dark room: everything, blended at */
/* ALTMILLER_CAVE_DARK_ALPHA. */
#define ALTMILLER_CAVE_DARK_LIGHT_WININ (WININ_WIN0_ALL | WININ_WIN1_ALL)
/* Outside it: every layer with no blend, so BG1 covers the room. */
#define ALTMILLER_CAVE_DARK_LIGHT_WINOUT                                       \
	(WINOUT_WIN01_BG0 | WINOUT_WIN01_BG1 | WINOUT_WIN01_BG2 |                  \
	 WINOUT_WIN01_BG3 | WINOUT_WIN01_OBJ)
/* Outside the light once Babi is found: everything, blended, so the whole */
/* room brightens as BG1 fades. */
#define ALTMILLER_CAVE_BRIGHTENING_WINOUT (WINOUT_WIN01_ALL | WINOUT_WINOBJ_ALL)

static void scAltmillerCaveReleaseWallBurst(void);

/* The ring of five lights above the rocks as it spins to its next light. */
/* Angles are 16-bit turns, and the light at the top is the chosen one. */
typedef struct LIGHT_WHEEL {
	s16 phase;	/* WHEEL_SPEEDING_UP to WHEEL_STOPPED */
	s16 frames; /* frames into the phase */
	s16 unused; /* nothing reads or writes it */
	u16 ang;
	u16 speed;
	/* How far the ring has turned since its last click. */
	u16 travel;
} LIGHT_WHEEL;

/* The ring speeds up, coasts half a second, slows down, then stops with the */
/* chosen light at the top. */
enum {
	WHEEL_SPEEDING_UP,
	WHEEL_COASTING,
	WHEEL_SLOWING,
	WHEEL_STOPPING,
	WHEEL_STOPPED = 99
};

/* The ring's speeds, in angle a frame. It speeds up by WHEEL_SPEED_STEP a */
/* frame to WHEEL_TOP_SPEED and slows by half a step a frame to */
/* WHEEL_LOW_SPEED, then stops on the chosen light once it is within a frame's
 */
/* turn of the top. */
enum {
	WHEEL_SPEED_STEP = TRIG_TURN / 4096,
	WHEEL_TOP_SPEED = TRIG_TURN * 3 / 64,
	WHEEL_LOW_SPEED = TRIG_TURN * 3 / 256
};

/* The pieces the wall bursts into (scAltmillerCaveReleaseWallBurst). */
enum { WALL_BURST_PIECES = 24 };

/* The cave's own state, bytes of SceneLocalState: the rock puzzle and the */
/* passage's darkness. */
enum {
	CAVE_ROUNDS_MATCHED = 0, /* the rounds of lights matched, up to three */
	CAVE_PUZZLE = 1,		 /* the rock puzzle's state */
	CAVE_CHOSEN_LIGHT =
		2,			  /* the light the ring stops with at the top, 0 to 4 */
	CAVE_DARKNESS = 4 /* the passage's darkness level, 0 to 5 */
};

/* A byte of the cave's state; the chosen light and the darkness are signed. */
#define CAVE_STATE(byte)		(((u8 *)SceneLocalState)[byte])
#define CAVE_SIGNED_STATE(byte) (((s8 *)SceneLocalState)[byte])

/* The rock puzzle's two bytes, from CAVE_PUZZLE. */
typedef struct ROCK_PUZZLE_STATE {
	u8 state;
	s8 chosenLight;
} ROCK_PUZZLE_STATE;

/* The rock puzzle's bytes in the cave's state. */
#define ROCK_PUZZLE ((ROCK_PUZZLE_STATE *)&CAVE_STATE(CAVE_PUZZLE))

/* The puzzle's states. The leftmost rock is turned, then the rightmost, which
 */
/* shows the lights. After three lights matched the wall opens. */
enum {
	PUZZLE_START,
	PUZZLE_LEFT_ROCK_TURNED,
	PUZZLE_LIGHTS_SHOWN,
	PUZZLE_SOLVED = 99
};

/* The ring of lights. */
LIGHT_WHEEL AltmillerCave_LightWheelState __attribute__((nocommon)) ALIGNED(4);

#include "shared/pushed_actor.c"

/* Talking to Squall, the Jupiter Djinni in the north-west corner of the hall */
/* of fallen columns, starts a battle with it. */
static void scAltmillerCaveStartDjinnBattle(void) {
	fdRequestDjinnBattle(ACTOR_SQUALL, DJINNI_SQUALL);
}

/* The six rooms share one entrance table. */
const SCENE_ENTRANCE *scGetEntrances(void) { return AltmillerCave_Entrances; }

const SCENE_EXIT_REGION *scGetExitRegions(void) { return NULL; }

const u32 *scGetExits(void) { return AltmillerCave_Exits; }

const ACTOR_PLACE *scGetActorPlacements(void) {
	s16 sceneId = MainRoutineStateWork.next;

	if(sceneId == SCENE_ALTMILLER_CAVE_BABI) {
		return AltmillerCave_BabiActorPlacements;
	}
	if(sceneId == SCENE_ALTMILLER_CAVE_SQUALL) {
		return AltmillerCave_SquallActorPlacements;
	}
	if(sceneId == SCENE_ALTMILLER_CAVE_DRAUGHT) {
		return AltmillerCave_DraughtActorPlacements;
	}
	return AltmillerCave_ActorPlacementsOther;
}

/* Darkens the passage in to its current level: everything outside the light */
/* around the party is drawn at its full brightness at level 0, and two */
/* sixteenths dimmer at each level after, down to six sixteenths at level 5. */
static void scAltmillerCaveApplyFadeBlend(void) {
	s8 level = CAVE_SIGNED_STATE(CAVE_DARKNESS);

	scIoWrite16(ALTMILLER_CAVE_DARKNESS_BLEND, &REG_BLDCNT);
	if(level == 0) {
		scIoWrite16(BLEND_ALPHA(0, 16), &REG_BLDALPHA);
	}
	else if(level == 1) {
		scIoWrite16(BLEND_ALPHA(0, 14), &REG_BLDALPHA);
	}
	else if(level == 2) {
		scIoWrite16(BLEND_ALPHA(0, 12), &REG_BLDALPHA);
	}
	else if(level == 3) {
		scIoWrite16(BLEND_ALPHA(0, 10), &REG_BLDALPHA);
	}
	else if(level == 4) {
		scIoWrite16(BLEND_ALPHA(0, 8), &REG_BLDALPHA);
	}
	else {
		scIoWrite16(BLEND_ALPHA(0, 6), &REG_BLDALPHA);
	}
}

/* Keeps the passage's darkness level, and shows it unless Reveal is lighting */
/* the passage. */
static void scAltmillerCaveSetFadeLevel(s32 level) {
	FIELD_STATE *state = FieldStateWork;

	CAVE_STATE(CAVE_DARKNESS) = level;
	if(state->revealActive == 0) {
		scAltmillerCaveApplyFadeBlend();
	}
}

/* Walking back toward the cave mouth past the first bend north of it (cells */
/* (27, 19) to (27, 21)) lifts the darkness: the passage is as bright as at the
 */
/* mouth again. */
static void scAltmillerCaveDarkenToLevel0(void) {
	scAltmillerCaveSetFadeLevel(0);
}

/* Everything outside the light around the party grows a little darker as */
/* Isaac walks on east from the first bend north of the cave mouth (cells */
/* (28, 19) and (28, 20)), or comes back down the passage from the north (cell
 */
/* (26, 10)). */
static void scAltmillerCaveDarkenToLevel1(void) {
	scAltmillerCaveSetFadeLevel(1);
}

/* The passage grows darker again as Isaac walks on north up it (cell */
/* (26, 9)), or comes back east along the north wall (cells (18, 4) and */
/* (18, 5)). */
static void scAltmillerCaveDarkenToLevel2(void) {
	scAltmillerCaveSetFadeLevel(2);
}

/* The passage grows darker again as Isaac walks on west along the cave's */
/* north wall (cells (17, 4) and (17, 5)), or comes back from the north-west */
/* turn (cells (7, 8) and (7, 9)). */
static void scAltmillerCaveDarkenToLevel3(void) {
	scAltmillerCaveSetFadeLevel(3);
}

/* The passage grows darker again as Isaac turns south at its north-west end */
/* (cells (6, 8) and (6, 9)), or comes back up the west side (cells (7, 20) to
 */
/* (9, 20)). */
static void scAltmillerCaveDarkenToLevel4(void) {
	scAltmillerCaveSetFadeLevel(4);
}

/* The passage is at its darkest, only the light around the party bright, as */
/* Isaac walks down the west side into the last stretch before the small */
/* torch-lit chamber (cells (8, 21) to (10, 21)). */
static void scAltmillerCaveDarkenToLevel5(void) {
	scAltmillerCaveSetFadeLevel(5);
}

/* Darkens the room on entry, with the light around the party. The passage in */
/* darkens by its levels, the other dark rooms evenly, black outside the */
/* light. */
static void scAltmillerCaveInitDarkness(void) {
	FIELD_STATE *fieldState = FieldStateWork;
	SCR_TRANSITION_STATE *state;

	fieldState->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	fieldState->transitionFrm = 24;
	sySynch(1);
	fdScrTransitionStartHeldWindow(WINDOW_TRANSITION_LEADER_LOW);
	state = ScreenTransitionStateWork;
	state->value = ALTMILLER_CAVE_LIGHT_STEPS;
	if(gmFlagGet(FLAG_ALTMILLER_CAVE_DARKENS_BY_STEPS) != 0) {
		state->insideLayers = ALTMILLER_CAVE_STEPS_LIGHT_WININ;
		state->outsideLayers = ALTMILLER_CAVE_STEPS_LIGHT_WINOUT;
		scAltmillerCaveApplyFadeBlend();
		return;
	}
	state->insideLayers = ALTMILLER_CAVE_DARK_LIGHT_WININ;
	state->outsideLayers = ALTMILLER_CAVE_DARK_LIGHT_WINOUT;
	scIoWrite16(ALTMILLER_CAVE_DARKNESS_BLEND, &REG_BLDCNT);
	scIoWrite16(ALTMILLER_CAVE_DARK_ALPHA, &REG_BLDALPHA);
}

/* Casting Reveal in the passage in lights the whole passage while Reveal */
/* lasts, and in the rock room at the bottom of the cave shows each of the five
 */
/* rocks in its color. The rocks are blue, green, white, yellow and red from */
/* the left, the order Babi gives. */
static void scAltmillerCaveStartReveal(void) {
	if(MainRoutineStateWork.next == SCENE_ALTMILLER_CAVE_ENTRANCE) {
		scIoWrite16(BLEND_ALPHA(0, 16), &REG_BLDALPHA);
	}
	if(MainRoutineStateWork.next == SCENE_ALTMILLER_CAVE_DRAUGHT) {
		fdActorSetSprColById(ACTOR_BLUE_ROCK, COLOR_BLUE);
		fdActorSetSprColById(ACTOR_GREEN_ROCK, COLOR_GREEN);
		fdActorSetSprColById(ACTOR_WHITE_ROCK, COLOR_WHITE);
		fdActorSetSprColById(ACTOR_YELLOW_ROCK, COLOR_YELLOW);
		fdActorSetSprColById(ACTOR_RED_ROCK, COLOR_RED);
	}
}

/* When Reveal wears off, a dark room goes dark again around the light, and the
 */
/* five rocks in the rock room turn back to plain grey. */
static void scAltmillerCaveEndReveal(void) {
	FIELD_EFFECT_STATE *effectState =
		(FIELD_EFFECT_STATE *)FieldEffectStateWork;

	if(gmFlagGet(FLAG_ALTMILLER_CAVE_DARK) != 0) {
		scAltmillerCaveInitDarkness();
		/* Reveal then leaves the light around the party open as it ends. */
		effectState->sceneVariant = 1;
	}
	if(MainRoutineStateWork.next == SCENE_ALTMILLER_CAVE_DRAUGHT) {
		fdActorSetSprColById(ACTOR_BLUE_ROCK, COLOR_NONE);
		fdActorSetSprColById(ACTOR_GREEN_ROCK, COLOR_NONE);
		fdActorSetSprColById(ACTOR_WHITE_ROCK, COLOR_NONE);
		fdActorSetSprColById(ACTOR_YELLOW_ROCK, COLOR_NONE);
		fdActorSetSprColById(ACTOR_RED_ROCK, COLOR_NONE);
	}
}

/* Casting Retreat in the dark rooms of the cave first turns Isaac to face the
 */
/* screen, then lets the spell carry the party out. Retreat plays on a copy of
 */
/* him made after the turn, and the leader himself stops updating. */
static void scAltmillerCaveStartRetreat(void) {
	FIELD_EFFECT_STATE *effectState;
	ACTOR *original;
	ACTOR *replacement;

	effectState = (FIELD_EFFECT_STATE *)FieldEffectStateWork;
	original = effectState->caster;
	fdActorFaceDirection(effectState->casterId, FACING_SOUTH, 0);
	fdActorSetSprColMode(original, SPRITE_COLOR_NORMAL);
	fdEventWait(20);
	replacement = syFieldActorCreate(0, original->x.fixed, original->y.fixed,
									 original->z.fixed);
	if(replacement != NULL) {
		syDMAXCopy(original, replacement, DMA_COPY_WORDS(sizeof(ACTOR) / 4),
				   REG_DMA3);
		original->update = 0;
		effectState->caster = replacement;
		original->sprStorageKind = 0;
	}
}

/* Once the stone pillar on the ledge in Squall's hall has been pushed east */
/* over the edge, with Move or by hand, it drops into the pit below (cell */
/* (30, 20)), and its top fills the gap so the party can cross to the way on. */
/* Pushed anywhere else, nothing more happens. */
static void scAltmillerCaveDropPillarIntoPit(void) {
	ACTOR *actor = fdActorGet(ACTOR_SQUALL_ROOM_PILLAR);

	if(FIELD_CELL(actor->x.fixed) == 30 && FIELD_CELL(actor->z.fixed) == 20) {
		actor->motionFlg = ACTOR_FALLS;
		actor->groundHeight = 0;
		actor->priFlg = ACTOR_PRIORITY_UNDERFOOT;
		fdMapGridCopyCellAttributes(30, 20, 1, 1, 32, 20);
#if TBS_PATCH_1
		gmFlagSet(FLAG_ALTMILLER_CAVE_PILLAR_IN_PLACE);
#endif
	}
}

/* Walking east into the stone pillar on the ledge in Squall's hall, from cell
 */
/* (28, 20), pushes it a cell and over the edge into the pit. */
static void scAltmillerCavePushPillar(void) {
	scPushedActorAdvancePair();
	scAltmillerCaveDropPillarIntoPit();
}

/* Before Frost is cast on the puddle in Squall's hall, the game checks whether
 */
/* anything stands on it. If so, nothing freezes. */
static void scAltmillerCaveCheckPuddleCovered(void) {
	FIELD_EFFECT_STATE *effectState =
		(FIELD_EFFECT_STATE *)FieldEffectStateWork;
	ACTOR *target = fdActorGet(ACTOR_SQUALL_ROOM_PUDDLE);
	FIELD_POS position;

	position.x = target->x.fixed;
	position.y = target->y.fixed;
	position.z = target->z.fixed;

	if(syFieldActorCheckMovementCollision(target, &position) > 0) {
		effectState->leaveTgt = 1;
	}
}

/* Frost on the puddle in Squall's hall (cell (23, 17)) turns it into a pillar
 */
/* of ice. A fallen column pushed toward it stops against it, which is how the
 */
/* party wedges the columns out of the way to reach Squall. The puddle's cell */
/* becomes a wall, copied from a spare cell of the map. */
static void scAltmillerCaveFreezePuddle(void) {
	FIELD_EFFECT_STATE *effectState =
		(FIELD_EFFECT_STATE *)FieldEffectStateWork;
	ACTOR *puddle = fdActorGet(ACTOR_SQUALL_ROOM_PUDDLE);
	s32 blocked;

	blocked = effectState->leaveTgt;
	if(blocked == 0) {
		fdMapGridCopyCellAttributes(76, 16, 1, 1, 73, 17);
		if(puddle != NULL) {
			s32 motionFlags = ACTOR_FALLS;
			puddle->motionFlg = motionFlags;
			/* blocked is 0 here: the pillar is drawn as anything standing is.
			 */
			puddle->priFlg = blocked;
		}
		gmFlagSet(FLAG_ALTMILLER_CAVE_PUDDLE_FROZEN);
	}
}

/* Pushing against the puddle in Squall's hall from any side (cells (23, 16), */
/* (24, 17), (23, 18) and (22, 17)) makes Isaac hop over it, if the ground two
 */
/* cells on is clear. Once the puddle is frozen he can no longer hop it. */
static void scAltmillerCaveHopOverPuddle(void) {
	ACTOR *actor = fdActorGet(MainRoutineStateWork.leaderActorId);
	s32 motionFlags = actor->motionFlg;
	union {
		FIELD_POS pos;
		s32 coordinates[3];
		FIELD_COORDINATE components[3];
	} destination;
	FIELD_COORDINATE *components = destination.components;

	components[0].fixed = actor->x.fixed;
	components[1].fixed = actor->y.fixed;
	components[2].fixed = actor->z.fixed;
	{
		s32 angle = FACING_SIXTEENTH(actor->facing);

		syVectorAddPolarOffset(toFIXED(32), angle, destination.coordinates);
	}
	if(syFieldActorCheckMovementCollision(actor, &destination.pos) == 0) {
		fdEventStart();
		fdActorSelectAnimation(actor, ANIM_LEADER_SPRING);
		sySynch(6);
		sySndPlay(SE_JUMP);
		fdActorSelectAnimation(actor, ANIM_LEADER_LEAP);
		actor->speedLimit = toFIXED(3);
		actor->acceleration = toFIXED(2);
		actor->velY = toFIXED(4);
		actor->motionFlg &= ACTOR_JUMP_KEEPS_MOTION;
		fdActorSetSprFlags(actor, 0);
		fdActorMoveToAndWait(ACTOR_PARTY_LEADER, components[0].part[0].integer,
							 components[2].part[0].integer);
		fdActorSelectAnimation(actor, ANIM_LEADER_SPRING);
		fdActorSetSprFlags(actor, SPRITE_FLAG_SHADOW);
		actor->motionFlg = motionFlags;
		fdEventEnd();
	}
}

#include "shared/pushed_actor_push.c"

/* Update function of the light at the top of the stopped ring. It pulses */
/* through four sizes. */
static void scAltmillerCavePulseLight(ACTOR *light) {
	s32 scale = AltmillerCave_PulseScales[(light->tmr >> 2) & 3];

	light->scaleX = scale;
	light->scaleY = scale;
	light->tmr++;
	light->tmr &= 15;
}

/* Puts a light on the ring, radius pixels from its center above the rocks, at
 */
/* the given angle. */
static void scAltmillerCavePlaceLight(s32 actorId, s32 radius, s32 angle) {
	FIXED centreX = toFIXED(504);
	FIXED centreHeight = toFIXED(24);
	FIXED depth = toFIXED(144);
	ACTOR *light = fdActorGet(actorId);
	s32 coordinates[3];

	coordinates[0] = centreX;
	coordinates[2] = centreHeight;
	syVectorAddPolarOffset(radius, angle, coordinates);
	light->x.fixed = coordinates[0];
	light->y.fixed = coordinates[2];
	light->z.fixed = depth;
}

/* Spaces the five lights evenly around the ring, the first at the given */
/* angle. */
static void scAltmillerCavePlaceLights(s32 angle) {
	s32 i;

	for(i = 0; i < LIGHT_COUNT; i++) {
		scAltmillerCavePlaceLight(ACTOR_FIRST_LIGHT + i, toFIXED(24), angle);
		angle -= DEGtoANG(72);
	}
}

/* Task that spins the ring of lights. */
static void scAltmillerCaveSpinLights(void) {
	LIGHT_WHEEL *wheel = AltmillerCave_LightWheel;
	s32 moving = 1;
	s32 phase = wheel->phase;

	if(phase == WHEEL_SPEEDING_UP) {
		wheel->speed += WHEEL_SPEED_STEP;
		if(wheel->speed > WHEEL_TOP_SPEED - 1) {
			wheel->phase++;
			wheel->frames = 0;
		}
	}
	else if(phase == WHEEL_COASTING) {
		if(wheel->frames == 30) {
			wheel->phase++;
		}
	}
	else if(phase == WHEEL_SLOWING) {
		wheel->speed -= WHEEL_SPEED_STEP / 2;
		if(wheel->speed <= WHEEL_LOW_SPEED - 1) {
			wheel->phase++;
		}
	}
	else if(phase == WHEEL_STOPPING) {
		s32 chosenLight = CAVE_SIGNED_STATE(CAVE_CHOSEN_LIGHT);
		s32 chosenAngle = syMathDivide(chosenLight * TRIG_TURN, LIGHT_COUNT);
		s16 fromTop = wheel->ang - chosenAngle - DEGtoANG(90);

		if(fromTop > -WHEEL_LOW_SPEED && fromTop < WHEEL_LOW_SPEED) {
			wheel->ang = chosenAngle + DEGtoANG(90);
			wheel->phase = WHEEL_STOPPED;
			wheel->speed = 0;
			fdActorGet(ACTOR_FIRST_LIGHT + chosenLight)->update =
				scAltmillerCavePulseLight;
		}
	}
	else if(phase == WHEEL_STOPPED) {
		moving = 0;
	}
	if(moving != 0) {
		AltmillerCave_LightWheel->ang += AltmillerCave_LightWheel->speed;
		scAltmillerCavePlaceLights(AltmillerCave_LightWheel->ang);
		AltmillerCave_LightWheel->travel += AltmillerCave_LightWheel->speed;
		if(AltmillerCave_LightWheel->travel > TRIG_TURN * 3 / 16) {
			AltmillerCave_LightWheel->travel = 0;
			sySndPlay(SE_CLICK);
		}
	}
	AltmillerCave_LightWheel->frames++;
}

/* Pressing A at one of the five rocks in the rock room at the bottom of the */
/* cave turns it: "Isaac rotated the rock." A chime says it was the right */
/* rock and an error sound the wrong one. The leftmost and then the rightmost */
/* bring out the ring of five lights on the wall above the middle rock; after */
/* that, the rock of the light at the top spins the ring on, and the third such
 */
/* match makes the ground shake, the ring spiral into nothing and the wall */
/* behind the rocks burst open. */
static void scAltmillerCaveTurnRock(s32 rock) {
	u8 *puzzle;
	LIGHT_WHEEL *wheel;
	s32 correctRock = 0;
	s32 i;
	ACTOR *actor;
	s32 scale;
	s8 state;
	s32 chosenLight;
	u8 previousLight;
	FIXED radius;
	u16 angle;

	fdEventStart();
	fdEndBusyAndReveal();
	fdEventSetMessageId(MsgAltmillerRotatedRock);
	/* "Isaac rotated the rock." */
	fdEventShowMessage(ACTOR_FIRST_ROCK, 0);
	fdEventEnd();
	for(i = 0; i < LIGHT_COUNT; i++) {
		actor = fdActorGet(ACTOR_FIRST_LIGHT + i);
		actor->update = NULL;
		actor->scaleX = FIXED_ONE;
		actor->scaleY = FIXED_ONE;
	}
	state = ROCK_PUZZLE->state;
	previousLight = ROCK_PUZZLE->chosenLight;
	wheel = AltmillerCave_LightWheel;
	chosenLight = ROCK_PUZZLE->chosenLight;
	wheel->ang = (chosenLight * TRIG_TURN) / LIGHT_COUNT + DEGtoANG(90);
	/* The leftmost rock, then the rightmost, before the rounds of lights. */
	if(state == PUZZLE_START) {
		if(rock == ACTOR_BLUE_ROCK) {
			state = PUZZLE_LEFT_ROCK_TURNED;
			sySndPlay(SE_CHIME);
		}
		else {
			sySndPlay(SE_MENU_ERROR);
		}
		CAVE_STATE(CAVE_ROUNDS_MATCHED) = 0;
	}
	else if(state == PUZZLE_LEFT_ROCK_TURNED) {
		if(rock == ACTOR_BLUE_ROCK) {
			sySndPlay(SE_CHIME);
		}
		else if(rock == ACTOR_RED_ROCK) {
			/* The lights grow in one by one, each with SE_LIGHT_APPEAR. */
			state = PUZZLE_LIGHTS_SHOWN;
			sySndPlay(SE_CHIME);
			sySynch(30);
			angle = AltmillerCave_LightWheel->ang;
			for(i = 0; i < LIGHT_COUNT; i++) {
				scAltmillerCavePlaceLight(ACTOR_FIRST_LIGHT + i, toFIXED(24),
										  angle);
				sySndPlay(SE_LIGHT_APPEAR);
				actor = fdActorGet(ACTOR_FIRST_LIGHT + i);
				actor->scaleX = 0;
				scale = 2 * FIXED_ONE / 5;
				do {
					actor->scaleY = scale;
					actor->scaleX = scale;
					sySynch(1);
					scale += 3 * FIXED_ONE / 64;
				} while(actor->scaleX < FIXED_ONE);
				angle -= DEGtoANG(72);
			}
			sySynch(30);
			correctRock = 1;
		}
		else {
			sySndPlay(SE_MENU_ERROR);
			state = PUZZLE_START;
		}
	}
	else if(state == PUZZLE_LIGHTS_SHOWN) {
		if(rock != ACTOR_FIRST_ROCK + chosenLight) {
			/* The wrong rock: the lights shrink away. */
			state = PUZZLE_START;
			sySndPlay(SE_MENU_ERROR);
			sySynch(30);
			for(i = 0; i < LIGHT_COUNT; i++) {
				actor = fdActorGet(ACTOR_FIRST_LIGHT + i);
				sySndPlay(SE_LIGHT_APPEAR);
				for(scale = actor->scaleX; actor->scaleX > 2 * FIXED_ONE / 5;
					scale -= 3 * FIXED_ONE / 64) {
					actor->scaleY = scale;
					actor->scaleX = scale;
					sySynch(1);
				}
				fdActorSetPos(ACTOR_FIRST_LIGHT + i, 0, 0);
			}
		}
		else {
			sySndPlay(SE_CHIME);
			correctRock = 1;
			sySynch(30);
		}
	}
	puzzle = &CAVE_STATE(CAVE_PUZZLE);
	*puzzle = state;
	/* A light matched: choose a different light and spin the ring to it. */
	if(correctRock) {
		u8 rounds = ++puzzle[CAVE_ROUNDS_MATCHED - CAVE_PUZZLE];

		/* One of the four others: 1 to 4 lights on from the last. */
		puzzle[CAVE_CHOSEN_LIGHT - CAVE_PUZZLE] =
			((s8)((syRandomBelow(4)) + previousLight + 1) + 5) % 5;
		AltmillerCave_LightWheel->phase = WHEEL_SPEEDING_UP;
		AltmillerCave_LightWheel->frames = 0;
		AltmillerCave_LightWheel->speed = TRIG_TURN / 128;
		AltmillerCave_LightWheel->travel = TRIG_TURN * 3 / 16;
		syTaskAddFrame(scAltmillerCaveSpinLights, TASK_PASS_FRAME);
		if(rounds <= 2) {
			while(AltmillerCave_LightWheel->phase != WHEEL_STOPPED) {
				sySynch(1);
			}
			sySynch(10);
			sySndPlay(SE_CHIME);
		}
		else {
			/* The third light matched: the ground shakes, the ring spirals in
			 * to */
			/* nothing, and the wall bursts. */
			*puzzle = PUZZLE_SOLVED;
			while(AltmillerCave_LightWheel->phase != WHEEL_SLOWING) {
				sySynch(1);
			}
			AltmillerCave_LightWheel->phase = WHEEL_SLOWING;
			AltmillerCave_LightWheel->frames = 0;
			fdMapGridSetShake(2, 2, 1);
			fdEventWait(20);
			fdMapGridSetShake(4, 4, 1);
			AltmillerCave_LightWheel->phase = WHEEL_STOPPED;
			sySndPlay(SE_WARP);
			radius = toFIXED(24);
			angle = AltmillerCave_LightWheel->ang;
			do {
				for(i = 0; i < LIGHT_COUNT; i++) {
					actor = fdActorGet(ACTOR_FIRST_LIGHT + i);
					actor->scaleX -= FIXED_ONE / 4096;
					actor->scaleY -= FIXED_ONE / 4096;
					scAltmillerCavePlaceLight(ACTOR_FIRST_LIGHT + i, radius,
											  angle);
					angle -= DEGtoANG(72);
				}
				radius -= toFIXED(1) / 5;
				angle += WHEEL_TOP_SPEED;
				sySynch(1);
			} while(radius > 0);
			for(i = 0; i < LIGHT_COUNT; i++) {
				fdActorSetPos(ACTOR_FIRST_LIGHT + i, 0, 0);
			}
			scAltmillerCaveReleaseWallBurst();
			sySndPlay(MUS_PUZZLE_SOLVED);
		}
		syCloseEvent(scAltmillerCaveSpinLights);
	}
}

/* The party turns north, toward Babi. */
static inline void scAltmillerCaveFacePartyNorth(void) {
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
}

/* Talking to the invisible Babi in his room, or reading his mind, once the */
/* party has the Mystic Draught: he drinks it and appears. Ivan reads his mind
 */
/* and learns who he is, Lord Babi of Tolbi. Two men from Tolbi arrive with */
/* torches to take him home, and he asks the party to enter Colosso's finals. */
/* They decide whether to. */
static void scAltmillerCaveRestoreBabiAndInviteParty(void) {
	SCR_TRANSITION_STATE *state;
	ACTOR *actor;
	s32 i;
	s32 eva;
	s32 evb;

	gmFlagSet(FLAG_BABI_SAVED);
	gmPartyInventoryDiscard(ITEM_MYSTIC_DRAUGHT);
	fdEventStart();
	fdEndBusyAndReveal();
	fdActorRunRepeatedMotion(ACTOR_INVISIBLE_BABI, 2);
	fdEventWait(20);
	fdEventSetMessageId(MsgAltmillerCameBackPleaseDraughtQuickly);
	/* "You... You came back... Please, the draught- quickly." */
	fdEventShowMessage(ACTOR_INVISIBLE_BABI, 0);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.8);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 232, 160);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdEventWait(50);
	fdEventWait(10);
	fdCameraMoveTo(264, CAMERA_KEEP, 200, 1);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.8);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 264, 208);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	syFieldActorLaunchFromLeader(ACTOR_GARET, -16, 16, FACING_NORTH);
	syFieldActorLaunchFromLeader(ACTOR_MIA, 0, 16, FACING_NORTH);
	syFieldActorLaunchFromLeader(ACTOR_IVAN, 16, 16, FACING_NORTH);
	fdActorWaitForMoveById(ACTOR_GARET);
	fdEventWait(20);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
	/* "Oh, thank you! You've saved my life. Squeak... Glug glug glug..." */
	fdEventShowMessage(ACTOR_INVISIBLE_BABI, 0);
	fdEventWait(10);
	fdActorFaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	fdEventWait(20);
	actor = fdActorGet(ACTOR_INVISIBLE_BABI);
	fdEventWait(30);
	fdActorRunRepeatedMotion(ACTOR_INVISIBLE_BABI, 2);
	fdEventWait(30);
	fdActorRunRepeatedMotion(ACTOR_INVISIBLE_BABI, 2);
	fdEventWait(30);
	fdActorRunRepeatedMotion(ACTOR_INVISIBLE_BABI, 2);
	fdEventWait(60);
	sySndPlay(SOUND_FADE_OUT_BGM);
	actor->spr->rotation = 0;
	fdActorJump(ACTOR_INVISIBLE_BABI, 10, 70);
	for(i = 0; i < 30; i++) {
		actor->spr->image.pri = 0;
		fdEventWait(2);
		actor->spr->image.pri = 2;
		fdEventWait(2);
	}
	fdEventWait(40);
	{
		s32 x;
		s32 z;

		x = fdActorGet(ACTOR_INVISIBLE_BABI)->x.part[0].integer;
		z = fdActorGet(ACTOR_INVISIBLE_BABI)->z.part[0].integer;
		fdActorSetPos(ACTOR_INVISIBLE_BABI, 0, 0);
		fdActorSetPos(ACTOR_BABI, x, z);
	}
	actor->spr->flg = 0;
	sySndPlay(MUS_BABIS_THEME);
	fdActorSetSpeed(ACTOR_BABI, 0.8);
	fdActorWalkByAndWait(ACTOR_BABI, 32, 32);
	fdActorFaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
	fdEventWait(20);
	fdActorGet(ACTOR_PARTY_LEADER)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorGet(ACTOR_GARET)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorGet(ACTOR_MIA)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorGet(ACTOR_IVAN)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkBy(ACTOR_PARTY_LEADER, 0, 16);
	fdActorWalkBy(ACTOR_GARET, 0, 16);
	fdActorWalkBy(ACTOR_MIA, 0, 16);
	fdActorWalkByAndWait(ACTOR_IVAN, 0, 16);
	fdActorGet(ACTOR_PARTY_LEADER)->steeringFlg |= ACTOR_FACES_MOTION;
	fdActorGet(ACTOR_GARET)->steeringFlg |= ACTOR_FACES_MOTION;
	fdActorGet(ACTOR_MIA)->steeringFlg |= ACTOR_FACES_MOTION;
	fdActorGet(ACTOR_IVAN)->steeringFlg |= ACTOR_FACES_MOTION;
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	fdActorSetAnimation(ACTOR_GARET, ANIM_STAND);
	fdActorSetAnimation(ACTOR_MIA, ANIM_STAND);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_STAND);
	sySynch(1);
	scAltmillerCaveFacePartyNorth();
	fdEventWait(30);
	fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdActorStartRepeatedMotion(ACTOR_GARET, 2);
	fdActorStartRepeatedMotion(ACTOR_MIA, 2);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
	fdEventWait(20);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
	/* "Thank you for your help. I'll be okay now." */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	fdActorJump(ACTOR_GARET, 4, 13);
	fdActorJump(ACTOR_GARET, 4, 30);
	/* "Look! It's a man!" */
	fdEventShowMessage(ACTOR_GARET, 0);
	fdActorFaceActor(ACTOR_IVAN, ACTOR_GARET, 30);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
	/* "What are you talking about? What did you expect?" */
	fdEventShowMessage(ACTOR_IVAN, 0);
	fdActorFaceActor(ACTOR_GARET, ACTOR_IVAN, 30);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "Well, I mean, we couldn't see who was talking..." */
	fdEventShowMessage(ACTOR_GARET, 0);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
	fdEventWait(30);
	/* "My sudden appearance seems to have startled you." */
	fdEventOpenMessage(ACTOR_BABI, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	if(fdEventAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
		fdEventWait(10);
		fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
		fdEventWait(30);
		fdEventSetMessageId(MsgAltmillerWouldHaveRevealedMyselfSooner);
		/* "I would have revealed myself sooner, but I could not even..." */
		fdEventShowMessage(ACTOR_BABI, 0);
	}
	else {
		fdEventSetMessageId(MsgAltmillerMustTraveledIndeedIfDidnt);
		/* "You must be well traveled indeed if that didn't shock you." */
		fdEventShowMessage(ACTOR_BABI, 0);
	}
	fdEventSetMessageId(MsgAltmillerHowDidBecomeParalyzedWhile);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_MIA, 2);
	fdEventWait(20);
	/* "How did you become paralyzed while you were invisible?" */
	fdEventShowMessage(ACTOR_MIA, 0);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
	fdEventWait(20);
	/* "And that ability to conceal yourself... Are you an Adept?" */
	fdEventShowMessage(ACTOR_IVAN, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	/* "An Adept? What are you talking about?" */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorFaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
	fdActorFaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
	scAltmillerCaveFacePartyNorth();
	fdEventWait(30);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
	fdEventWait(30);
	/* "You used Psynergy to conceal yourself, didn't you? Only an Adept..." */
	fdEventShowMessage(ACTOR_GARET, 0);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
	fdEventWait(30);
	/* "Oh, um... Yes, I was using Psynergy to conceal myself..." */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	/* "Something in his tale doesn't sound right..." */
	fdEventShowMessage(ACTOR_IVAN, 0);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	fdEventWait(30);
	/* "I shall peer into his heart." */
	fdEventShowMessage(ACTOR_IVAN, 0);
	fdEventWait(20);
	fdActorSetSpeed(ACTOR_IVAN, 1);
	fdActorWalkByAndWait(ACTOR_IVAN, 0, -48);
	fdActorFaceDirection(ACTOR_IVAN, FACING_WEST, 0);
	fdEventWait(40);
	/* Ivan reads Babi's mind: he is Babi, the ruler of Tolbi, and hid by the */
	/* power of an ancient civilization. */
	fdPsynergyLoadResources(ABILITY_MIND_READ, 1);
	fdPsynergySetupActorPair(ACTOR_IVAN, ACTOR_BABI);
	fdPsynergyStartCast();
	fdPsynergyRunHandler(1);
	fdEventWait(150);
	fdPsynergyRunHandler(2);
	fdPsynergyStopCasterUpdate();
	fdActorSetAnimation(ACTOR_IVAN, ANIM_STAND);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
	fdEventWait(40);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	/* "What's the matter, Ivan?" */
	fdEventShowMessage(ACTOR_MIA, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "This is Babi, the ruler of Tolbi!" */
	fdEventShowMessage(ACTOR_IVAN, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	fdEventWait(10);
	fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTH, 0);
	fdEventWait(30);
	/* "And that wasn't Psynergy he was using to conceal himself... It was..."
	 */
	fdEventShowMessage(ACTOR_IVAN, 0);
	fdEventWait(10);
	fdActorJump(ACTOR_BABI, 4, 13);
	fdActorJump(ACTOR_BABI, 4, 30);
	fdActorFaceDirection(ACTOR_BABI, FACING_EAST, 0);
	fdEventWait(30);
	/* "I said nothing of the kind! How can you know that?" */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
	fdActorFaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
	fdEventWait(30);
	/* "So, I guess that would make you Adepts..." */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(20);
	fdActorShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	/* "Does that mean... Have I finally found true Lemurians!?" */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
	fdEventWait(30);
	/* "That's what I've been saying! We're Adepts. ...But what's Lemuria?" */
	fdEventShowMessage(ACTOR_GARET, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	/* "Yes, what is this 'Lemuria'?" */
	fdEventShowMessage(ACTOR_MIA, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	/* "What? You're Adepts... but not Lemurians?" */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(50);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
	fdEventWait(10);
	fdActorFaceDirection(ACTOR_IVAN, FACING_WEST, 0);
	fdEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	fdEventWait(30);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_BABI, 2);
	fdEventWait(20);
	fdEventWait(20);
	fdActorSetSpeed(ACTOR_IVAN, 1);
	fdActorWalkByAndWait(ACTOR_IVAN, 0, 48);
	fdEventWait(10);
	fdActorFaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
	fdActorFaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
	scAltmillerCaveFacePartyNorth();
	fdEventWait(50);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdEventWait(30);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
	fdEventWait(30);
	/* "So I was wrong..." */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "Are you searching for these Lemurians, Babi?" */
	fdEventShowMessage(ACTOR_IVAN, 0);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_BABI, 2);
	fdEventWait(30);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
	fdEventWait(30);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
	fdEventWait(20);
	/* "Babi, you're the ruler of Tolbi... You can have anything you want...."
	 */
	fdEventShowMessage(ACTOR_GARET, 0);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
	fdEventWait(30);
	/* "Even the ruler of Tolbi has his limitations." */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 80);
	fdEventWait(20);
	fdActorFaceDirection(ACTOR_BABI, FACING_NORTH, 0);
	fdEventWait(50);
	/* "Every year, I grow older and more frail... This body's had it." */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	/* "And you regret growing older?" */
	fdEventShowMessage(ACTOR_IVAN, 0);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_BABI, 2);
	fdEventWait(20);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	/* "I understand your desire to remain young, but there's nothing anyone..."
	 */
	fdEventShowMessage(ACTOR_MIA, 0);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_BABI, 2);
	fdEventWait(20);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	/* "Wait a sec, Isaac! Do you remember when we were in Sol Sanctum?" */
	fdEventOpenMessage(ACTOR_GARET, 0);
	if(fdEventAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
		fdEventSetMessageId(MsgAltmillerThatsKradenSaidAlchemyCould);
		/* "That's right! Kraden said Alchemy could thwart death itself!" */
		fdEventShowMessage(ACTOR_GARET, 0);
	}
	else {
		fdEventSetMessageId(MsgAltmillerForgotKradenSaidAlchemyCould);
		/* "You forgot? Kraden said Alchemy could be used to thwart death." */
		fdEventShowMessage(ACTOR_GARET, 0);
	}
	fdEventSetMessageId(MsgAltmillerWhatWasThat);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	fdActorFaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
	fdEventWait(10);
	/* "What was that?" */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	fdEventWait(10);
	fdActorFaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 50);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdEventWait(30);
	fdEventWait(10);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
	fdEventWait(30);
	/* "Did you say you know Kraden?" */
	fdEventOpenMessage(ACTOR_BABI, 0);
	fdEventSetMessageId(MsgAltmillerBabi);
	fdEventAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0);
	/* The room brightens as two men from Tolbi arrive: "Babi!" The light */
	/* widens to fill the screen and the darkness (BG1) fades out. */
	state = ScreenTransitionStateWork;
	state->value = ALTMILLER_CAVE_LIGHT_FULL;
	for(eva = 0, evb = BLEND_WEIGHT_MAX; eva <= BLEND_WEIGHT_MAX;
		eva++, evb--) {
		scIoWrite16(ALTMILLER_CAVE_DARKNESS_BLEND, &REG_BLDCNT);
		REG_BLDALPHA = BLEND_ALPHA(evb, eva);
		sySynch(7);
	}
	{
		u16 *slot = (u16 *)&state->outsideLayers;
		s32 value = ALTMILLER_CAVE_BRIGHTENING_WINOUT;

		*slot = value;
	}
	fdActorSetPos(ACTOR_FIRST_SOLDIER, 280, 304);
	fdActorSetPos(ACTOR_SECOND_SOLDIER, 296, 304);
	/* "Babi!" */
	fdEventShowMessage(ACTOR_FIRST_SOLDIER, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTH, 0);
	fdEventWait(10);
	fdActorFaceDirection(ACTOR_MIA, FACING_SOUTH, 0);
	fdEventWait(10);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
	fdEventWait(10);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
	fdEventWait(10);
	fdEventWait(10);
	fdCameraFollowActor(ACTOR_FIRST_SOLDIER, 1);
	fdCameraWaitForMove();
	fdEventWait(70);
	fdActorShowEmote(ACTOR_SECOND_SOLDIER, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "We've been searching everywhere for you, Babi!" */
	fdEventShowMessage(ACTOR_SECOND_SOLDIER, 0);
	fdActorWatchAndSetScript(ACTOR_PARTY_LEADER, ACTOR_FIRST_SOLDIER);
	fdActorWatchAndSetScript(ACTOR_GARET, ACTOR_FIRST_SOLDIER);
	fdActorWatchAndSetScript(ACTOR_MIA, ACTOR_FIRST_SOLDIER);
	fdActorWatchAndSetScript(ACTOR_IVAN, ACTOR_FIRST_SOLDIER);
	/* The two men greet Babi; he invites the party to the finals. */
	fdActorSetSpeed(ACTOR_FIRST_SOLDIER, 1.4);
	fdActorSetSpeed(ACTOR_SECOND_SOLDIER, 1.4);
	fdActorSetScript(ACTOR_FIRST_SOLDIER, AltmillerCave_GuardsArrivePath);
	fdEventWait(3);
	fdActorWalkByAndWait(ACTOR_SECOND_SOLDIER, -16, 0);
	fdActorSetScript(ACTOR_SECOND_SOLDIER, AltmillerCave_GuardsArrivePath);
	fdActorWaitForScriptEndById(ACTOR_FIRST_SOLDIER);
	fdActorWalkByAndWait(ACTOR_FIRST_SOLDIER, 0, -16);
	fdActorWaitForScriptEndById(ACTOR_SECOND_SOLDIER);
	fdActorFaceDirection(ACTOR_FIRST_SOLDIER, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_SECOND_SOLDIER, FACING_EAST, 0);
	fdActorStop(ACTOR_PARTY_LEADER);
	fdActorStop(ACTOR_GARET);
	fdActorStop(ACTOR_MIA);
	fdActorStop(ACTOR_IVAN);
	scAltmillerCaveFacePartyNorth();
	fdActorFaceDirection(ACTOR_BABI, FACING_WEST, 0);
	fdCameraMoveTo(248, CAMERA_KEEP, 216, 1);
	fdCameraWaitForMove();
	fdEventWait(20);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_FIRST_SOLDIER, 2);
	fdEventWait(20);
	/* "Iodem said we would find you in Altmiller Cave!" */
	fdEventShowMessage(ACTOR_FIRST_SOLDIER, 0);
	fdEventWait(10);
	fdActorFaceDirection(ACTOR_BABI, FACING_SOUTHEAST, 0);
	fdEventWait(20);
	fdActorRunRepeatedMotion(ACTOR_BABI, 2);
	fdEventWait(50);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_SECOND_SOLDIER, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	/* "You're...not glad we found you?" */
	fdEventShowMessage(ACTOR_SECOND_SOLDIER, 0);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_BABI, 2);
	fdEventWait(20);
	fdActorFaceDirection(ACTOR_BABI, FACING_WEST, 0);
	fdEventWait(10);
	/* "No, it's not that." */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_FIRST_SOLDIER, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "What are you doing in a cave such as this, my lord?" */
	fdEventShowMessage(ACTOR_FIRST_SOLDIER, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "I have my reasons..." */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdActorWalkByAndWait(ACTOR_SECOND_SOLDIER, 16, 0);
	fdEventWait(20);
	fdActorRunRepeatedMotion(ACTOR_SECOND_SOLDIER, 2);
	fdEventWait(20);
	/* "Everyone in Tolbi has been in a panic over your absence." */
	fdEventShowMessage(ACTOR_SECOND_SOLDIER, 0);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_BABI, 2);
	fdEventWait(20);
	/* "I did not mean to cause anyone any undue concern." */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdActorWalkByAndWait(ACTOR_FIRST_SOLDIER, 16, 0);
	fdEventWait(20);
	/* "We're just happy we found you before the finals began." */
	fdEventShowMessage(ACTOR_FIRST_SOLDIER, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	/* "The trials have already ended?" */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorJump(ACTOR_SECOND_SOLDIER, 4, 13);
	fdActorJump(ACTOR_SECOND_SOLDIER, 4, 30);
	/* "Why have you been in this cave for so long?" */
	fdEventShowMessage(ACTOR_SECOND_SOLDIER, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "I had an important errand, but it has been taken care of." */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_FIRST_SOLDIER, ANIM_NOD);
	fdEventWait(30);
	/* "Splendid! Then you can attend the finals." */
	fdEventShowMessage(ACTOR_FIRST_SOLDIER, 0);
	fdEventWait(10);
	fdActorFaceDirection(ACTOR_BABI, FACING_SOUTHEAST, 0);
	fdEventWait(30);
	/* "I must speak more with these people." */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorFaceDirection(ACTOR_FIRST_SOLDIER, FACING_SOUTHEAST, 0);
	fdEventWait(10);
	fdActorFaceDirection(ACTOR_SECOND_SOLDIER, FACING_SOUTHEAST, 0);
	fdEventWait(70);
	fdActorFaceDirection(ACTOR_SECOND_SOLDIER, FACING_EAST, 0);
	fdEventWait(10);
	fdActorFaceDirection(ACTOR_FIRST_SOLDIER, FACING_EAST, 0);
	fdEventWait(30);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_SECOND_SOLDIER, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	/* "Who are they?" */
	fdEventShowMessage(ACTOR_SECOND_SOLDIER, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	fdEventWait(20);
	fdActorFaceDirection(ACTOR_BABI, FACING_WEST, 0);
	fdEventWait(30);
	/* "I owe my life to them..." */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorFaceEachOther(ACTOR_FIRST_SOLDIER, ACTOR_SECOND_SOLDIER, 70);
	fdActorFaceDirection(ACTOR_FIRST_SOLDIER, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_SECOND_SOLDIER, FACING_SOUTHEAST, 0);
	fdEventWait(30);
	fdActorSetAnimationAndWait(ACTOR_FIRST_SOLDIER, ANIM_NOD);
	fdEventWait(30);
	fdActorSetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_NOD);
	fdEventWait(30);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_SECOND_SOLDIER, 2);
	fdEventWait(20);
	/* "Pardon our rudeness. You have our gratitude for saving Babi's life." */
	fdEventShowMessage(ACTOR_SECOND_SOLDIER, 0);
	fdEventWait(10);
	fdActorFaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
	fdActorFaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
	scAltmillerCaveFacePartyNorth();
	fdEventWait(30);
	fdEventWait(10);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimation(ACTOR_MIA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	fdEventWait(30);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_SECOND_SOLDIER, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
	/* "We must have Iodem grant them a reward." */
	fdEventShowMessage(ACTOR_SECOND_SOLDIER, 0);
	fdEventWait(10);
	fdActorFaceDirection(ACTOR_FIRST_SOLDIER, FACING_EAST, 0);
	fdEventWait(10);
	fdActorFaceDirection(ACTOR_SECOND_SOLDIER, FACING_EAST, 0);
	fdEventWait(30);
	fdActorRunRepeatedMotion(ACTOR_FIRST_SOLDIER, 2);
	fdEventWait(20);
	/* "You can speak with them again after the finals are finished." */
	fdEventShowMessage(ACTOR_FIRST_SOLDIER, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "Oh. I see... If that's what we have to do, then..." */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorSetAnimation(ACTOR_FIRST_SOLDIER, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_NOD);
	fdEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
	fdEventWait(40);
	fdActorWatchAndSetScript(ACTOR_PARTY_LEADER, ACTOR_BABI);
	fdActorWatchAndSetScript(ACTOR_GARET, ACTOR_BABI);
	fdActorWatchAndSetScript(ACTOR_MIA, ACTOR_BABI);
	fdActorWatchAndSetScript(ACTOR_IVAN, ACTOR_BABI);
	fdActorSetSpeed(ACTOR_BABI, 1);
	fdActorWalkByAndWait(ACTOR_BABI, -16, 0);
	fdActorWalkBy(ACTOR_FIRST_SOLDIER, 0, 48);
	fdActorWalkBy(ACTOR_SECOND_SOLDIER, 0, 48);
	fdActorWalkByAndWait(ACTOR_BABI, -16, 0);
	fdActorWalkByAndWait(ACTOR_BABI, 0, 32);
	fdActorSetAnimation(ACTOR_FIRST_SOLDIER, ANIM_STAND);
	fdActorSetAnimation(ACTOR_SECOND_SOLDIER, ANIM_STAND);
	fdActorStop(ACTOR_PARTY_LEADER);
	fdActorStop(ACTOR_GARET);
	fdActorStop(ACTOR_MIA);
	fdActorStop(ACTOR_IVAN);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	fdActorFaceDirection(ACTOR_BABI, FACING_EAST, 0);
	fdEventWait(20);
	/* "I have not heard your names yet." */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdActorFaceDirection(ACTOR_FIRST_SOLDIER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_SECOND_SOLDIER, FACING_NORTH, 0);
	fdEventWait(30);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdEventWait(20);
	/* "I'm Garet." */
	fdEventShowMessage(ACTOR_GARET, 0);
	fdEventWait(10);
	fdActorFaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 30);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdEventWait(20);
	fdActorFaceActor(ACTOR_GARET, ACTOR_BABI, 30);
	/* "This is Isaac." */
	fdEventShowMessage(ACTOR_GARET, 0);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	fdEventWait(30);
	/* "I am Ivan." */
	fdEventShowMessage(ACTOR_IVAN, 0);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
	fdEventWait(30);
	/* "My name is Mia." */
	fdEventShowMessage(ACTOR_MIA, 0);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_BABI, 2);
	fdEventWait(20);
	/* "Come see me at my palace once the finals have ended." */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(20);
	fdActorFaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
	fdEventWait(70);
	fdActorFaceDirection(ACTOR_BABI, FACING_EAST, 0);
	fdEventWait(20);
	/* "I would like to speak with you further." */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(20);
	fdActorFaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
	fdEventWait(30);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
	fdEventWait(30);
	/* Babi and the two men walk back toward the cave entrance. */
	fdActorSetScript(ACTOR_SECOND_SOLDIER, AltmillerCave_SecondGuardLeavePath);
	fdActorSetScript(ACTOR_FIRST_SOLDIER, AltmillerCave_FirstGuardLeavePath);
	fdActorSetScript(ACTOR_BABI, AltmillerCave_BabiLeavePath);
	fdEventWait(10);
	fdActorFaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
	fdActorFaceEachOther(ACTOR_MIA, ACTOR_IVAN, 0);
	fdActorWaitForScriptEndById(ACTOR_BABI);
	/* "Say, I have an idea... Why don't you enter the finals?" */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTH, 0);
	fdEventWait(20);
	fdCameraMoveTo(248, CAMERA_KEEP, 248, 1);
	fdCameraWaitForMove();
	fdEventWait(20);
	fdActorFaceDirection(ACTOR_BABI, FACING_NORTH, 0);
	fdEventWait(20);
	/* "You have some interesting powers." */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_FIRST_SOLDIER, 2);
	fdEventWait(20);
	fdActorFaceDirection(ACTOR_FIRST_SOLDIER, FACING_WEST, 0);
	fdEventWait(10);
	fdActorFaceDirection(ACTOR_SECOND_SOLDIER, FACING_WEST, 0);
	fdEventWait(20);
	/* "But, Lord Babi, they have not cleared the trials." */
	fdEventShowMessage(ACTOR_FIRST_SOLDIER, 0);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_SHAKE_HEAD);
	fdEventWait(30);
	/* "Only warriors who clear the trials can appear in the finals." */
	fdEventShowMessage(ACTOR_SECOND_SOLDIER, 0);
	fdEventWait(20);
	fdActorShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_SCOWL, 40);
	fdActorFaceDirection(ACTOR_BABI, FACING_EAST, 0);
	fdEventWait(20);
	/* "And you don't think that saving my life qualifies as a suitable trial?"
	 */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_FIRST_SOLDIER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(ACTOR_SECOND_SOLDIER, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	fdEventWait(30);
	fdActorFaceActor(ACTOR_FIRST_SOLDIER, ACTOR_SECOND_SOLDIER, 60);
	fdActorFaceDirection(ACTOR_FIRST_SOLDIER, FACING_WEST, 0);
	fdEventWait(30);
	fdActorSetAnimationAndWait(ACTOR_FIRST_SOLDIER, ANIM_NOD);
	fdEventWait(40);
	fdActorFaceDirection(ACTOR_BABI, FACING_NORTH, 0);
	fdEventWait(30);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
	fdEventWait(30);
	/* "I look forward to seeing you battle!" */
	fdEventShowMessage(ACTOR_BABI, 0);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
	fdEventWait(30);
	fdActorFaceDirection(ACTOR_BABI, FACING_EAST, 0);
	fdEventWait(30);
	fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
	fdEventWait(20);
	fdActorSetAnimation(ACTOR_FIRST_SOLDIER, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_NOD);
	fdEventWait(40);
	fdActorFaceDirection(ACTOR_FIRST_SOLDIER, FACING_EAST, 0);
	fdEventWait(20);
	fdActorFaceDirection(ACTOR_SECOND_SOLDIER, FACING_EAST, 0);
	fdEventWait(40);
	fdActorWalkBy(ACTOR_BABI, 32, 0);
	fdActorWalkBy(ACTOR_SECOND_SOLDIER, 0, 64);
	fdActorWalkByAndWait(ACTOR_FIRST_SOLDIER, 16, 0);
	fdActorWalkBy(ACTOR_FIRST_SOLDIER, 0, 64);
	fdActorWaitForMoveById(ACTOR_BABI);
	fdActorWalkByAndWait(ACTOR_BABI, 0, 64);
	fdActorSetPos(ACTOR_BABI, 0, 0);
	fdActorSetPos(ACTOR_FIRST_SOLDIER, 0, 0);
	fdActorSetPos(ACTOR_SECOND_SOLDIER, 0, 0);
	fdEventWait(20);
	/* With Babi gone the darkness returns: BG1 fades back in, then the light */
	/* shrinks around the party again. */
	for(eva = 0, evb = BLEND_WEIGHT_MAX; eva <= BLEND_WEIGHT_MAX;
		eva++, evb--) {
		scIoWrite16(ALTMILLER_CAVE_DARKNESS_BLEND, &REG_BLDCNT);
		REG_BLDALPHA = BLEND_ALPHA(eva, evb);
		sySynch(7);
	}
	state->value = ALTMILLER_CAVE_LIGHT_STEPS;
	{
		u16 *slot = (u16 *)&state->outsideLayers;
		s32 value = ALTMILLER_CAVE_DARK_LIGHT_WINOUT;

		*slot = value;
	}
	sySynch(1);
	scIoWrite16(ALTMILLER_CAVE_DARKNESS_BLEND, &REG_BLDCNT);
	scIoWrite16(ALTMILLER_CAVE_DARK_ALPHA, &REG_BLDALPHA);
	fdCameraMoveToActorById(ACTOR_PARTY_LEADER, 1);
	fdCameraWaitForMove();
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdEventWait(20);
	/* "He's asking us to appear in Colosso, isn't he?" */
	fdEventShowMessage(ACTOR_GARET, 0);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	fdEventWait(30);
	/* "Is he serious?" */
	fdEventShowMessage(ACTOR_IVAN, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "He wants to see us fight using our Psynergy... That's what it is." */
	fdEventShowMessage(ACTOR_MIA, 0);
	fdEventWait(10);
	fdActorFaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
	fdEventWait(20);
	/* "The finals... Should we do it?" */
	fdEventOpenMessage(ACTOR_GARET, 0);
	fdActorFaceActor(ACTOR_MIA, ACTOR_PARTY_LEADER, 0);
	fdActorFaceActor(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
	if(fdEventAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
		fdEventWait(10);
		fdActorRunRepeatedMotion(ACTOR_GARET, 2);
		fdEventWait(20);
		fdEventSetMessageId(MsgAltmillerSoWellDoIt);
		/* "So we'll do it?" */
		fdEventShowMessage(ACTOR_GARET, 0);
	}
	else {
		fdEventWait(10);
		fdActorRunRepeatedMotion(ACTOR_GARET, 2);
		fdEventWait(20);
		fdEventSetMessageId(MsgAltmillerDontWantEitherDo);
		/* "You don't want to either, do you?" */
		fdEventShowMessage(ACTOR_GARET, 0);
	}
	fdEventSetMessageId(MsgAltmillerEitherWayWereStuckHere);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
	fdEventWait(30);
	/* "Either way, we're stuck here in Tolbi until Colosso is over." */
	fdEventShowMessage(ACTOR_IVAN, 0);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
	fdEventWait(30);
	/* "Babi said that he wanted to talk to us, didn't he?" */
	fdEventShowMessage(ACTOR_MIA, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "He seemed to know Kraden." */
	fdEventShowMessage(ACTOR_GARET, 0);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	fdEventWait(30);
	/* "What could be the connection between Lemuria and Adepts?" */
	fdEventShowMessage(ACTOR_IVAN, 0);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_MIA, 2);
	fdEventWait(20);
	/* "I suspect he knows quite a bit about what's going on with us." */
	fdEventShowMessage(ACTOR_MIA, 0);
	fdEventWait(20);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	/* "I guess all we can do is wait around until Colosso is..." */
	fdEventShowMessage(ACTOR_GARET, 0);
	/* "Come on, let's enter Colosso! Can we do it?" */
	fdEventOpenMessage(ACTOR_GARET, 0);
	if(fdEventAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
		fdEventWait(10);
		fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
		fdEventWait(30);
		fdEventSetMessageId(MsgAltmillerGreatItsDecidedJustHave);
		/* "Great! It's decided. We'll just have to do our best!" */
		fdEventShowMessage(ACTOR_GARET, 0);
		fdEventWait(10);
		fdActorFaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
		fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
		fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
		fdEventWait(30);
		/* "If you want to do it, Isaac, we'll go along with you." */
		fdEventShowMessage(ACTOR_IVAN, 0);
		fdEventWait(20);
		fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
		fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
		fdActorSetAnimation(ACTOR_MIA, ANIM_NOD);
		fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
		fdEventWait(30);
	}
	else {
		fdEventWait(10);
		fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
		fdEventWait(30);
		fdEventSetMessageId(MsgAltmillerDontLetMeDownLike);
		/* "What? Don't let me down like this!" */
		fdEventShowMessage(ACTOR_GARET, 0);
		fdEventWait(10);
		fdActorFaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
		fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
		fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
		fdEventWait(30);
		/* "I guess we'll just wait until we reach Tolbi and let Isaac..." */
		fdEventShowMessage(ACTOR_IVAN, 0);
		fdEventWait(10);
		fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
		fdEventWait(20);
		fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
		fdEventWait(20);
		fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
		fdEventWait(30);
		fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 70);
	}
	sySndPlay(SOUND_FADE_OUT_BGM);
	fdActorSetSpeed(ACTOR_GARET, 1.2);
	fdActorSetSpeed(ACTOR_IVAN, 1.2);
	fdActorSetSpeed(ACTOR_MIA, 1.2);
	scWalkIntoLeader(ACTOR_GARET);
	scWalkIntoLeader(ACTOR_IVAN);
	scWalkIntoLeader(ACTOR_MIA);
	scPlayMusic();
	fdEventEnd();
}

/* Talking to the white outline lying on the floor near the middle of Babi's */
/* room, or reading its mind with Mind Read, the first time: the invisible */
/* Babi, too weak to move, asks the party to bring his draught from deep in */
/* the cave and tells them the order of the rocks' colors. */
static void scAltmillerCaveMeetInvisibleBabi(void) {
	ACTOR *leader;
	s32 line;

	gmFlagSet(FLAG_MET_INVISIBLE_BABI);
	sySndPlay(MUS_CAVE_THEME);
	fdEventStart();
	fdEndBusyAndReveal();
	fdEventSetMessageId(MsgAltmillerSee);
	fdActorRunRepeatedMotion(ACTOR_INVISIBLE_BABI, 2);
	fdEventWait(20);
	/* "You... You can see me?" */
	fdEventShowMessage(ACTOR_INVISIBLE_BABI, 0);
	fdEventWait(10);
	fdCameraMoveTo(248, CAMERA_KEEP, 184, 1);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.8);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 248, 192);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	syFieldActorLaunchFromLeader(ACTOR_GARET, -16, 16, FACING_NORTH);
	syFieldActorLaunchFromLeader(ACTOR_MIA, 0, 16, FACING_NORTH);
	syFieldActorLaunchFromLeader(ACTOR_IVAN, 16, 16, FACING_NORTH);
	fdActorWaitForMoveById(ACTOR_GARET);
	fdEventWait(20);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdEventWait(40);
	/* "Someone's there." */
	fdEventShowMessage(ACTOR_IVAN, 0);
	fdActorRunRepeatedMotion(ACTOR_MIA, 2);
	fdEventWait(20);
	/* "Wrapped in Psynergetic light... Who could it be?" */
	fdEventShowMessage(ACTOR_MIA, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
					 40);
	/* "You know about Psynergy? And you can see me?" */
	fdEventShowMessage(ACTOR_INVISIBLE_BABI, 0);
	fdEventWait(10);
	fdActorFaceEachOther(ACTOR_GARET, ACTOR_IVAN, 50);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	fdEventWait(30);
	/* "Come on, mister! Don't hide-show yourself!" */
	fdEventShowMessage(ACTOR_GARET, 0);
	fdEventWait(10);
	fdActorSetAttachedEffect(ACTOR_INVISIBLE_BABI,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorRunRepeatedMotion(ACTOR_INVISIBLE_BABI, 2);
	fdEventWait(30);
	/* "Even if I wanted to show myself... As you can see, I cannot..." */
	fdEventShowMessage(ACTOR_INVISIBLE_BABI, 0);
	fdEventWait(20);
	fdActorFaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	fdEventWait(30);
	/* "What do you mean, 'as you can see'? You're invisible!" */
	fdEventShowMessage(ACTOR_IVAN, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
					 40);
	/* "Oh... That's right." */
	fdEventShowMessage(ACTOR_INVISIBLE_BABI, 0);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
	fdEventWait(20);
	/* "You say you can't move... What happened?" */
	fdEventShowMessage(ACTOR_MIA, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
	/* "Hmmm, yes. I ran out of my draught and collapsed here." */
	fdEventShowMessage(ACTOR_INVISIBLE_BABI, 0);
	fdEventWait(10);
	fdActorFaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 30);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	/* "Draught?" */
	fdEventShowMessage(ACTOR_GARET, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdEventWait(20);
	/* "Exactly what kind of draught is it?" */
	fdEventShowMessage(ACTOR_GARET, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "You couldn't even begin to comprehend if I tried to tell..." */
	fdEventShowMessage(ACTOR_INVISIBLE_BABI, 0);
	fdActorRunRepeatedMotion(ACTOR_INVISIBLE_BABI, 2);
	fdEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
	fdEventWait(20);
	/* "You seem to be in a lot of pain. Your life depends on that..." */
	fdEventShowMessage(ACTOR_MIA, 0);
	fdEventWait(30);
	fdActorShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB,
					 40);
	/* "It does. Could you do me a favor?" */
	fdEventShowMessage(ACTOR_INVISIBLE_BABI, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdEventWait(60);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
	fdEventWait(20);
	/* "A favor? What is it?" */
	fdEventShowMessage(ACTOR_IVAN, 0);
	fdEventWait(20);
	/* "I'd like you to get my draught for me." */
	fdEventShowMessage(ACTOR_INVISIBLE_BABI, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "Get it? Where is it?" */
	fdEventShowMessage(ACTOR_GARET, 0);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_INVISIBLE_BABI, 2);
	fdEventWait(20);
	/* "Deep in this cave..." */
	fdEventShowMessage(ACTOR_INVISIBLE_BABI, 0);
	fdActorRunRepeatedMotion(ACTOR_MIA, 2);
	fdEventWait(20);
	/* "What, is there a pharmacy down there?" */
	fdEventShowMessage(ACTOR_MIA, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "Witty, aren't we? I'll have to tell you how to find it. Can..." */
	fdEventOpenMessage(ACTOR_INVISIBLE_BABI, 0);
	fdActorFaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
	fdActorFaceActor(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
	if(fdEventAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
		fdEventSetMessageId(MsgAltmillerKiddingHaventActually);
		fdEventWait(20);
		fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
		fdActorSetSpeed(ACTOR_GARET, 2);
		fdActorWalkByAndWait(ACTOR_GARET, 0, -16);
		fdEventWait(10);
		fdActorFaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 30);
		/* "Are you kidding, Isaac? We haven't even actually seen this guy!" */
		fdEventShowMessage(ACTOR_GARET, 0);
	}
	else {
		fdEventSetMessageId(MsgAltmillerDontTrustAnyone);
		fdEventWait(10);
		fdActorSetSpeed(ACTOR_GARET, 1);
		fdActorWalkByAndWait(ACTOR_GARET, 0, -16);
		fdEventWait(10);
		fdActorFaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 30);
		fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
		fdEventWait(20);
		/* "I'm with you, Isaac. I don't trust anyone who's afraid to..." */
		fdEventShowMessage(ACTOR_GARET, 0);
	}

	/* Ivan and Mia speak up for helping him, and Garet gives in. */
	line = MsgAltmillerSaidCouldntMove;
	fdEventSetMessageId(line);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
	fdActorSetSpeed(ACTOR_IVAN, 2);
	fdActorWalkByAndWait(ACTOR_IVAN, 0, -16);
	fdEventWait(10);
	fdActorFaceEachOther(ACTOR_IVAN, ACTOR_PARTY_LEADER, 30);
	/* "He said he couldn't move even if he wanted to, right?..." */
	fdEventShowMessage(ACTOR_IVAN, 0);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
	fdEventWait(20);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_MIA, 0);
	fdActorFaceActor(ACTOR_GARET, ACTOR_MIA, 0);
	fdActorFaceActor(ACTOR_IVAN, ACTOR_MIA, 0);
	fdEventWait(20);
	/* "I agree with Ivan- whoever he is, he needs our help." */
	fdEventShowMessage(ACTOR_MIA, 0);
	fdEventWait(20);
	fdActorSetSpeed(ACTOR_GARET, 1);
	fdActorWalkByAndWait(ACTOR_GARET, 0, 16);
	fdEventWait(30);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
	fdEventWait(30);
	fdActorFaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
	fdEventWait(20);
	/* "Fine! Okay! We should help people in need. Is that better!?" */
	fdEventShowMessage(ACTOR_GARET, 0);
	fdEventWait(30);
	fdActorFaceEachOther(ACTOR_MIA, ACTOR_IVAN, 30);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
	fdEventWait(30);
	fdActorSetSpeed(ACTOR_IVAN, 1);
	fdActorWalkByAndWait(ACTOR_IVAN, 0, 16);
	fdEventWait(10);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	line += 7;
	fdEventSetMessageId(line);
	fdEventWait(30);
	fdActorShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
					 40);
	/* "Will you get it for me?!" */
	fdEventShowMessage(ACTOR_INVISIBLE_BABI, 0);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	fdEventWait(30);
	/* "What do we have to do to get your draught?" */
	fdEventShowMessage(ACTOR_IVAN, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
	/* "First, you'll have to go deep into the cave." */
	fdEventShowMessage(ACTOR_INVISIBLE_BABI, 0);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	fdEventWait(30);
	/* "You'll find five rocks sticking up out of the ground." */
	fdEventShowMessage(ACTOR_INVISIBLE_BABI, 0);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
	fdEventWait(30);
	/* "Rotate the rocks on the left and right sides." */
	fdEventShowMessage(ACTOR_INVISIBLE_BABI, 0);
	fdEventWait(10);
	fdActorFaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 50);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdEventWait(30);
	/* "When you turn the rocks in the right order, five colored lights will..."
	 */
	fdEventShowMessage(ACTOR_INVISIBLE_BABI, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	/* "Colored lights!? This sounds weird." */
	fdEventShowMessage(ACTOR_MIA, 0);
	fdEventWait(30);
	/* "That's right. Lights will appear on the wall." */
	fdEventShowMessage(ACTOR_INVISIBLE_BABI, 0);
	fdEventWait(10);
	fdActorFaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 50);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdEventWait(30);
	/* "There will be five colors: blue, green, white, yellow, and red." */
	fdEventShowMessage(ACTOR_INVISIBLE_BABI, 0);
	fdEventWait(10);
	fdActorFaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	fdEventWait(30);
	/* "Turn the five rocks in accordance with those colors." */
	fdEventShowMessage(ACTOR_INVISIBLE_BABI, 0);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdEventWait(20);
	/* "Riiight... Spin the rocks in accordance with the color of the light..."
	 */
	fdEventShowMessage(ACTOR_GARET, 0);
	fdEventWait(30);
	/* "The blue light corresponds to the rock farthest to the left. In..." */
	fdEventShowMessage(ACTOR_INVISIBLE_BABI, 0);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
	fdEventWait(20);
	/* "So, in order, left to right, the colors are blue, green, white,..." */
	fdEventShowMessage(ACTOR_IVAN, 0);
	fdEventWait(20);
	fdActorShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | EMOTE_SMILE, 50);
	/* "Correct. If you do this correctly, a hidden door will open." */
	fdEventShowMessage(ACTOR_INVISIBLE_BABI, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	/* "A door? Somewhere deep in a cave? Who put it there?" */
	fdEventShowMessage(ACTOR_GARET, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "An ancient civilization, long since vanished. Ohhh! Please hurry!" */
	fdEventShowMessage(ACTOR_INVISIBLE_BABI, 0);
	fdActorRunRepeatedMotion(ACTOR_INVISIBLE_BABI, 2);
	fdEventWait(20);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
	fdEventWait(20);
	/* "He seems to be in a lot of pain. We'd better find that cave, Isaac." */
	fdEventShowMessage(ACTOR_MIA, 0);
	fdEventWait(10);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_MIA, 40);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdEventWait(30);
	fdActorFaceActor(ACTOR_GARET, ACTOR_MIA, 0);
	fdActorFaceActor(ACTOR_IVAN, ACTOR_MIA, 0);
	fdEventWait(20);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	fdEventWait(30);
	sySndPlay(SOUND_FADE_OUT_BGM);
	fdActorSetSpeed(ACTOR_GARET, 1.2);
	fdActorSetSpeed(ACTOR_IVAN, 1.2);
	fdActorSetSpeed(ACTOR_MIA, 1.2);
	fdActorSetAnimation(ACTOR_GARET, ANIM_WALK);
	leader = fdActorGet(ACTOR_PARTY_LEADER);
	if(leader != NULL) {
		fdActorSetDestination(ACTOR_GARET, leader->x.part[0].integer,
							  leader->z.part[0].integer);
	}
	fdActorWaitForMoveById(ACTOR_GARET);
	fdActorSetPos(ACTOR_GARET, 0, 0);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_WALK);
	leader = fdActorGet(ACTOR_PARTY_LEADER);
	if(leader != NULL) {
		fdActorSetDestination(ACTOR_IVAN, leader->x.part[0].integer,
							  leader->z.part[0].integer);
	}
	fdActorWaitForMoveById(ACTOR_IVAN);
	fdActorSetPos(ACTOR_IVAN, 0, 0);
	fdActorSetAnimation(ACTOR_MIA, ANIM_WALK);
	leader = fdActorGet(ACTOR_PARTY_LEADER);
	if(leader != NULL) {
		fdActorSetDestination(ACTOR_MIA, leader->x.part[0].integer,
							  leader->z.part[0].integer);
	}
	fdActorWaitForMoveById(ACTOR_MIA);
	fdActorSetPos(ACTOR_MIA, 0, 0);
	scPlayMusic();
	fdEventEnd();
}

/* Talking to the invisible Babi again before the draught is found: he asks */
/* whether the party forgot the order of the rock colors, and tells it again. */
static void scAltmillerCaveRepeatRockOrder(void) {
	s32 question;

	fdEventStart();
	question = MsgAltmillerForgetOrderRock;
	fdEventSetMessageId(question);
	/* "Did you forget the order of rock colors?" */
	fdEventOpenMessage(ACTOR_INVISIBLE_BABI, 0);
	if(fdEventAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
		fdEventWait(20);
		fdEventSetMessageId(question + 1);
		/* "From the left, it's blue, green, white, yellow, and red...." */
		fdEventShowMessage(ACTOR_INVISIBLE_BABI, 0);
	}
	else {
		fdEventWait(20);
		fdEventSetMessageId(question + 2);
		/* "Deep in the cave are five rocks. Turn the ones on the ends..." */
		fdEventShowMessage(ACTOR_INVISIBLE_BABI, 0);
	}
	fdEventEnd();
}

/* As the party walks away from the invisible Babi toward the stairs down in */
/* the north-west of his room (cells (11, 9) to (13, 9)), he calls them back */
/* once: "Wait..." Isaac turns, and Babi tells where the draught is: "The */
/* draught is... on a table behind the door." */
static void scAltmillerCaveBabiCallsPartyBack(void) {
	if(gmFlagGet(FLAG_MET_INVISIBLE_BABI) == 0)
		return;
	if(gmFlagGet(FLAG_BABI_SAVED) != 0)
		return;

	gmFlagSet(FLAG_BABI_CALLED_PARTY_BACK);
	fdEventStart();
	fdEventSetMessageId(MsgAltmillerWait);
	/* "Wait..." */
	fdEventShowMessage(ACTOR_INVISIBLE_BABI, 0);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdEventWait(30);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_INVISIBLE_BABI, 0);
	fdEventWait(30);
	/* "The draught is... on a table behind the door." */
	fdEventShowMessage(ACTOR_INVISIBLE_BABI, 0);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdEventWait(20);
	fdEventEnd();
}

const TRIGGER *scGetTriggers(void) {
	s16 sceneId = MainRoutineStateWork.next;

	if(sceneId == SCENE_ALTMILLER_CAVE_BABI) {
		return AltmillerCave_BabiTriggers;
	}
	if(sceneId == SCENE_ALTMILLER_CAVE_VIAL_CHEST) {
		return AltmillerCave_VialChestTriggers;
	}
	if(sceneId == SCENE_ALTMILLER_CAVE_SQUALL) {
		return AltmillerCave_SquallTriggers;
	}
	if(sceneId == SCENE_ALTMILLER_CAVE_COOKIE_CHEST) {
		return AltmillerCave_CookieChestTriggers;
	}
	if(sceneId == SCENE_ALTMILLER_CAVE_DRAUGHT) {
		return AltmillerCave_DraughtTriggers;
	}
	return AltmillerCave_TriggersOther;
}

/* Update function of the fallen columns in the hall at the bottom of the cave:
 */
/* each is drawn at the leader's own sprite priority. */
static void scAltmillerCaveMatchLeaderPriority(ACTOR *actor) {
	if(actor != NULL) {
		actor->priFlg = 0;
		actor->spr->image.pri = fdActorGet(ACTOR_PARTY_LEADER)->spr->image.pri;
	}
}

/* Walls off the cells the three fallen columns stand on. */
static inline void scAltmillerCaveBlockFallenColumnCells(void) {
	scPushedActorBlockFootprint(ACTOR_FIRST_FALLEN_COLUMN);
	scPushedActorBlockFootprint(ACTOR_FIRST_FALLEN_COLUMN + 1);
	scPushedActorBlockFootprint(ACTOR_FIRST_FALLEN_COLUMN + 2);
}

/* Sets up the room being entered: darkness, the ice pillar in Squall's hall */
/* as the party left it, Babi still lying there or gone, and in the */
/* rock room the rocks and lights as far as the puzzle got. Returns 0. */
s32 scInit(void) {
	ACTOR *actor;
	s32 i;

	if(MainRoutineStateWork.entrance == 0) {
		if(MainRoutineStateWork.next == SCENE_ALTMILLER_CAVE_BABI) {
			MainRoutineStateWork.entrance =
				ALTMILLER_CAVE_BABI_FROM_ALTMILLER_CAVE_ENTRANCE;
		}
		if(MainRoutineStateWork.next == SCENE_ALTMILLER_CAVE_VIAL_CHEST) {
			MainRoutineStateWork.entrance =
				ALTMILLER_CAVE_VIAL_CHEST_FROM_ALTMILLER_CAVE_BABI;
		}
		if(MainRoutineStateWork.next == SCENE_ALTMILLER_CAVE_SQUALL) {
			MainRoutineStateWork.entrance =
				ALTMILLER_CAVE_SQUALL_FROM_ALTMILLER_CAVE_VIAL_CHEST;
		}
		if(MainRoutineStateWork.next == SCENE_ALTMILLER_CAVE_COOKIE_CHEST) {
			MainRoutineStateWork.entrance =
				ALTMILLER_CAVE_COOKIE_CHEST_FROM_ALTMILLER_CAVE_SQUALL;
		}
		if(MainRoutineStateWork.next == SCENE_ALTMILLER_CAVE_DRAUGHT) {
			MainRoutineStateWork.entrance =
				ALTMILLER_CAVE_DRAUGHT_FROM_ALTMILLER_CAVE_COOKIE_CHEST;
		}
	}
	gmFlagSet(FLAG_ALTMILLER_CAVE_DARK);
	fdEventClearFlag(FLAG_ALTMILLER_CAVE_DARKENS_BY_STEPS);
	if(MainRoutineStateWork.next == SCENE_ALTMILLER_CAVE_ENTRANCE) {
		if(MainRoutineStateWork.entrance ==
		   ALTMILLER_CAVE_ENTRANCE_FROM_WORLD_MAP) {
			if(!gmFlagGet(FLAG_KEEP_PARTY_POSITION)) {
				CAVE_STATE(CAVE_DARKNESS) = 0;
			}
			gmFlagSet(FLAG_ALTMILLER_CAVE_DARKENS_BY_STEPS);
		}
		if(MainRoutineStateWork.entrance ==
		   ALTMILLER_CAVE_ENTRANCE_FROM_TORCH_CHAMBER) {
			if(!gmFlagGet(FLAG_KEEP_PARTY_POSITION)) {
				CAVE_STATE(CAVE_DARKNESS) = 5;
			}
			gmFlagSet(FLAG_ALTMILLER_CAVE_DARKENS_BY_STEPS);
		}
	}
	if(MainRoutineStateWork.next == SCENE_ALTMILLER_CAVE_BABI) {
		if(gmFlagGet(FLAG_BABI_SAVED)) {
			fdActorSetPos(ACTOR_INVISIBLE_BABI, 0, 0);
		}
		else {
			actor = fdActorGet(ACTOR_INVISIBLE_BABI);
			actor->spr->image.pri = 1;
			actor->spr->flg = SPRITE_FLAG_OUTLINED;
			actor->spr->rotation = DEGtoANG(90);
		}
	}
	if(MainRoutineStateWork.next == SCENE_ALTMILLER_CAVE_SQUALL) {
		gmFlagClear(FLAG_ALTMILLER_CAVE_DARK);
		scAltmillerCaveBlockFallenColumnCells();
		if(gmFlagGet(FLAG_ALTMILLER_CAVE_PUDDLE_FROZEN)) {
			fdActorSetAnimation(ACTOR_SQUALL_ROOM_PUDDLE,
								PUDDLE_ANIMATION_PILLAR);
			fdMapGridCopyCellAttributes(76, 16, 1, 1, 73, 17);
		}
		else {
			fdActorGet(ACTOR_SQUALL_ROOM_PUDDLE)->priFlg |=
				ACTOR_PRIORITY_UNDERFOOT;
		}
		fdActorSetSprFlags(fdActorGet(ACTOR_SQUALL_ROOM_PUDDLE), 0);
#if TBS_PATCH_1
		/* The cells scAltmillerCaveDropPillarIntoPit copies as it sets the
		 * flag, */
		/* redrawn on arrival. */
		if(gmFlagGet(FLAG_ALTMILLER_CAVE_PILLAR_IN_PLACE)) {
			fdMapGridCopyCellAttributes(30, 20, 1, 1, 32, 20);
		}
#endif
	}
	if(MainRoutineStateWork.next == SCENE_ALTMILLER_CAVE_DRAUGHT) {
		gmFlagClear(FLAG_ALTMILLER_CAVE_DARK);
		scAltmillerCaveBlockFallenColumnCells();
		fdActorGet(ACTOR_FIRST_FALLEN_COLUMN)->update =
			scAltmillerCaveMatchLeaderPriority;
		fdActorGet(ACTOR_FIRST_FALLEN_COLUMN + 1)->update =
			scAltmillerCaveMatchLeaderPriority;
		/* UB: the last is stored through a word view of the function pointer.
		 */
#ifdef UBFIX
		fdActorGet(ACTOR_FIRST_FALLEN_COLUMN + 2)->update =
			scAltmillerCaveMatchLeaderPriority;
#else
		*(s32 *)&fdActorGet(ACTOR_FIRST_FALLEN_COLUMN + 2)->update =
			(s32)scAltmillerCaveMatchLeaderPriority;
#endif
		/* Coming up to the rocks
		 * (ALTMILLER_CAVE_DRAUGHT_ROCKS_FROM_PILLAR_CAVE) */
		/* starts the puzzle over, with the red light first. */
		if(MainRoutineStateWork.entrance ==
		   ALTMILLER_CAVE_DRAUGHT_ROCKS_FROM_PILLAR_CAVE) {
			btDmaFill32(0, AltmillerCave_LightWheel, sizeof(LIGHT_WHEEL));
			if(!gmFlagGet(FLAG_KEEP_PARTY_POSITION)) {
				CAVE_STATE(CAVE_ROUNDS_MATCHED) = 0;
				CAVE_STATE(CAVE_PUZZLE) = PUZZLE_START;
				CAVE_STATE(CAVE_CHOSEN_LIGHT) =
					ACTOR_RED_LIGHT - ACTOR_FIRST_LIGHT;
			}
		}
		if(CAVE_SIGNED_STATE(CAVE_PUZZLE) == PUZZLE_SOLVED) {
			fdMapGridCopyCellsLayered(41, 55, 3, 2, 30, 55);
			fdMapGridCopyCellAttributes(42, 8, 1, 1, 31, 8);
		}
		if(CAVE_SIGNED_STATE(CAVE_PUZZLE) == PUZZLE_LIGHTS_SHOWN) {
			scAltmillerCavePlaceLights(
				(CAVE_SIGNED_STATE(CAVE_CHOSEN_LIGHT) * TRIG_TURN) /
					LIGHT_COUNT +
				DEGtoANG(90));
		}
		for(i = 0; i < LIGHT_COUNT; i++) {
			actor = fdActorGet(ACTOR_FIRST_LIGHT + i);
			actor->motionFlg = 0;
			actor->collisionFlg = 0;
			actor->scaleX = FIXED_ONE;
			actor->scaleY = FIXED_ONE;
			fdActorSetSprFlags(fdActorGet(ACTOR_FIRST_LIGHT + i), 0);
			/* The lights glimmer alike, each starting six frames after the one
			 */
			/* before, so that they do not glimmer in step. */
			fdActorSetAnimation(ACTOR_FIRST_LIGHT + i, i + 1);
		}
		fdActorSetSprColById(ACTOR_BLUE_LIGHT, COLOR_BLUE);
		fdActorSetSprColById(ACTOR_GREEN_LIGHT, COLOR_GREEN);
		fdActorSetSprColById(ACTOR_WHITE_LIGHT, COLOR_WHITE);
		fdActorSetSprColById(ACTOR_YELLOW_LIGHT, COLOR_YELLOW);
		fdActorSetSprColById(ACTOR_RED_LIGHT, COLOR_RED);
		fdActorSetSprColById(ACTOR_BLUE_ROCK, COLOR_NONE);
		fdActorSetSprColById(ACTOR_GREEN_ROCK, COLOR_NONE);
		fdActorSetSprColById(ACTOR_WHITE_ROCK, COLOR_NONE);
		fdActorSetSprColById(ACTOR_YELLOW_ROCK, COLOR_NONE);
		fdActorSetSprColById(ACTOR_RED_ROCK, COLOR_NONE);
		fdActorGet(ACTOR_BLUE_ROCK)->spr->image.pri = 3;
		fdActorGet(ACTOR_RED_ROCK)->spr->image.pri = 3;
		fdActorGet(ACTOR_BLUE_ROCK)->priFlg = ACTOR_PRIORITY_UNDERFOOT;
		fdActorGet(ACTOR_RED_ROCK)->priFlg = ACTOR_PRIORITY_UNDERFOOT;
		fdActorSetSprFlags(fdActorGet(ACTOR_BLUE_ROCK), 0);
		fdActorSetSprFlags(fdActorGet(ACTOR_RED_ROCK), 0);
	}
	if(gmFlagGet(FLAG_ALTMILLER_CAVE_DARK)) {
		scAltmillerCaveInitDarkness();
	}
	else {
		FieldStateWork->transition = SCENE_TRANSITION(
			TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
		FieldStateWork->transitionFrm = 24;
	}
	return 0;
}

/* Update function of a piece of the burst wall. It flies from the wall's */
/* center to a random point 120 pixels away, then starts again from the center.
 */
static void scAltmillerCaveUpdateWallFragment(FIELD_PARTICLE *fragment) {
	FIXED position[3];

	if(fragment->state == 0) {
		FIXED startZ = fragment->originZ;
		s32 startX = fragment->originX;

		fragment->z = startZ;
		position[2] = startZ;
		fragment->x = startX;
		position[0] = startX;
		syVectorAddPolarOffset(toFIXED(120), syRandomNext(), position);
		fragment->tgtX = position[0];
		fragment->tgtZ = position[2];
		fragment->acceleration = toFIXED(5);
		fragment->speedMax = toFIXED(5);
		fragment->turnStepLimited = 0;
		fragment->state++;
		if((VBlankCount & 3) == 0)
			sySndPlay(SE_HEAVY_IMPACT);
	}
	else if(fragment->state == 1) {
		if(fdParticleHasPendingTarget(fragment) == 0) {
			fragment->state--;
		}
	}
	else if(fragment->state == 2) {
		/* Unused: no step leads here. */
		if(fdParticleHasPendingTarget(fragment) == 0)
			fdEffectClearOwnedSlot(fragment);
	}
}

/* The wall above the middle rock bursts open. Pieces of rock fly out from */
/* where the ring was, with heavy thuds, the wall becomes an opening into the */
/* Mystic Draught's room, and the shaking calms. While the WALL_BURST_PIECES */
/* pieces fly, the field's colors are tinted (mnShopSaveAndTintFieldPalette). */
static void scAltmillerCaveReleaseWallBurst(void) {
	FIELD_EFFECT_STATE *effectState;
	VECTOR3 position;
	FIELD_PARTICLE *fragment;
	s32 fragmentIndex;

	fdEffectInitSlots();
	effectState = (FIELD_EFFECT_STATE *)FieldEffectStateWork;
	mnShopSaveAndTintFieldPalette(PALETTE_TINT_CUSTOM | RGB(8, 8, 8));
	position.x = toFIXED(504);
	position.y = toFIXED(24);
	position.z = toFIXED(144);
	fdCameraWorldToScr(&position);
	fragment = effectState->particles;
	for(fragmentIndex = 0; fragmentIndex < WALL_BURST_PIECES; fragmentIndex++) {
		FIXED scale;

		fdParticleInit(fragment, SPRITE_PARTICLE, position.x, position.z);
		fdParticleSetCallback(fragment, scAltmillerCaveUpdateWallFragment);
		fdParticleSetAnimation(fragment, PARTICLE_ANIMATION_FLICKER);
		fdSprSetAnimationColModes(fragment->spr, syRandomBelow(7));
		scale = syMathDivideUnsigned(syRandomNext(), 3) + 3 * FIXED_ONE / 2;
		fragment->scaleY = scale;
		fragment->scaleX = scale;
		sySynch(1);
		fragment++;
	}
	sySynch(80);
	fdMapGridCopyCellsLayered(41, 55, 3, 2, 30, 55);
	fdMapGridCopyCellAttributes(42, 8, 1, 1, 31, 8);
	sySynch(50);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	sySynch(30);
	for(fragmentIndex = 0;
		fragmentIndex < (s32)ARRAY_COUNT(effectState->particles);
		fragmentIndex++) {
		if(effectState->particles[fragmentIndex].active)
			effectState->particles[fragmentIndex].state = 2;
	}
	fdMapGridWaitForShakeToSettle();
	mnShopRestoreFieldPalette();
	fdEffectReleaseSlots();
}

#include "shared/pushed_actor_tables.c"

/* The ring of lights, as the cave's code reaches it. */
LIGHT_WHEEL *AltmillerCave_LightWheel = &AltmillerCave_LightWheelState;

SCENE_ENTRANCE AltmillerCave_Entrances[] = {
	{0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{ALTMILLER_CAVE_ENTRANCE_FROM_WORLD_MAP, CONDITION_ALWAYS, 456, 0, 504,
	 FACING_NORTH, 0, 0, 0, 544, 528, 0},
	{ALTMILLER_CAVE_ENTRANCE_FROM_TORCH_CHAMBER, CONDITION_ALWAYS, 280, 0, 360,
	 FACING_SOUTH, 0, 0, 0, 544, 528, 0},
	{ALTMILLER_CAVE_ENTRANCE_TORCH_CHAMBER_FROM_PASSAGE, CONDITION_ALWAYS, 120,
	 0, 696, FACING_NORTH, 0, 0, 552, 240, 720, 0},
	{ALTMILLER_CAVE_ENTRANCE_FROM_ALTMILLER_CAVE_BABI, CONDITION_ALWAYS, 120, 0,
	 600, FACING_SOUTH, 0, 0, 552, 240, 720, 0},
	{ALTMILLER_CAVE_BABI_FROM_ALTMILLER_CAVE_ENTRANCE, CONDITION_ALWAYS, 392, 0,
	 504, FACING_SOUTH, 0, 48, 16, 496, 672, 0},
	{ALTMILLER_CAVE_BABI_FROM_ALTMILLER_CAVE_VIAL_CHEST, CONDITION_ALWAYS, 136,
	 0, 104, FACING_SOUTH, 0, 48, 16, 496, 672, 0},
	{ALTMILLER_CAVE_VIAL_CHEST_FROM_ALTMILLER_CAVE_BABI, CONDITION_ALWAYS, 232,
	 0, 216, FACING_SOUTH, 0, 64, 48, 592, 576, 0},
	{ALTMILLER_CAVE_VIAL_CHEST_FROM_ALTMILLER_CAVE_SQUALL, CONDITION_ALWAYS,
	 328, 0, 264, FACING_SOUTH, 0, 64, 48, 592, 576, 0},
	{ALTMILLER_CAVE_SQUALL_FROM_ALTMILLER_CAVE_VIAL_CHEST, CONDITION_ALWAYS,
	 200, 0, 424, FACING_SOUTH, 0, 48, 80, 576, 480, 0},
	{ALTMILLER_CAVE_SQUALL_FROM_ALTMILLER_CAVE_COOKIE_CHEST, CONDITION_ALWAYS,
	 520, 0, 328, FACING_SOUTH, 0, 48, 80, 576, 480, 0},
	{ALTMILLER_CAVE_COOKIE_CHEST_FROM_ALTMILLER_CAVE_SQUALL, CONDITION_ALWAYS,
	 680, 0, 152, FACING_SOUTH, 0, 0, 0, 768, 512, 0},
	{ALTMILLER_CAVE_COOKIE_CHEST_FROM_ALTMILLER_CAVE_DRAUGHT, CONDITION_ALWAYS,
	 40, 0, 248, FACING_SOUTH, 0, 0, 0, 768, 512, 0},
	{ALTMILLER_CAVE_DRAUGHT_FROM_ALTMILLER_CAVE_COOKIE_CHEST, CONDITION_ALWAYS,
	 296, 0, 392, FACING_SOUTH, 0, 160, 304, 576, 704, 0},
	{ALTMILLER_CAVE_DRAUGHT_FROM_ROCKS, CONDITION_ALWAYS, 424, 0, 392,
	 FACING_SOUTH, 0, 160, 304, 576, 704, 0},
	{ALTMILLER_CAVE_DRAUGHT_ROCKS_FROM_PILLAR_CAVE, CONDITION_ALWAYS, 504, 0,
	 232, FACING_NORTH, 0, 384, 64, 624, 240, 0},
	{ALTMILLER_CAVE_DRAUGHT_ROCKS_FROM_CHEST, CONDITION_ALWAYS, 504, 0, 152,
	 FACING_SOUTH, 0, 384, 64, 624, 240, 0},
	{ALTMILLER_CAVE_DRAUGHT_CHEST_FROM_ROCKS, CONDITION_ALWAYS, 184, 0, 216,
	 FACING_NORTH, 0, 64, 64, 304, 240, 0},
	{SCENE_TABLE_END},
};

u32 AltmillerCave_Exits[] = {
	SCENE_EXITS(AltmillerCaveEntrance),
	SCENE_EXIT(ALTMILLER_CAVE_ENTRANCE_TO_WORLD_MAP, WorldMap,
			   WORLD_MAP_FROM_ALTMILLER_CAVE_ENTRANCE),
	SCENE_EXIT(ALTMILLER_CAVE_ENTRANCE_TO_TORCH_CHAMBER, AltmillerCaveEntrance,
			   ALTMILLER_CAVE_ENTRANCE_TORCH_CHAMBER_FROM_PASSAGE),
	SCENE_EXIT(ALTMILLER_CAVE_ENTRANCE_TORCH_CHAMBER_TO_PASSAGE,
			   AltmillerCaveEntrance,
			   ALTMILLER_CAVE_ENTRANCE_FROM_TORCH_CHAMBER),
	SCENE_EXIT(ALTMILLER_CAVE_ENTRANCE_TO_ALTMILLER_CAVE_BABI,
			   AltmillerCaveBabi,
			   ALTMILLER_CAVE_BABI_FROM_ALTMILLER_CAVE_ENTRANCE),
	SCENE_EXITS(AltmillerCaveBabi),
	SCENE_EXIT(ALTMILLER_CAVE_BABI_TO_ALTMILLER_CAVE_ENTRANCE,
			   AltmillerCaveEntrance,
			   ALTMILLER_CAVE_ENTRANCE_FROM_ALTMILLER_CAVE_BABI),
	SCENE_EXIT(ALTMILLER_CAVE_BABI_TO_ALTMILLER_CAVE_VIAL_CHEST,
			   AltmillerCaveVialChest,
			   ALTMILLER_CAVE_VIAL_CHEST_FROM_ALTMILLER_CAVE_BABI),
	SCENE_EXITS(AltmillerCaveVialChest),
	SCENE_EXIT(ALTMILLER_CAVE_VIAL_CHEST_TO_ALTMILLER_CAVE_BABI,
			   AltmillerCaveBabi,
			   ALTMILLER_CAVE_BABI_FROM_ALTMILLER_CAVE_VIAL_CHEST),
	SCENE_EXIT(ALTMILLER_CAVE_VIAL_CHEST_TO_ALTMILLER_CAVE_SQUALL,
			   AltmillerCaveSquall,
			   ALTMILLER_CAVE_SQUALL_FROM_ALTMILLER_CAVE_VIAL_CHEST),
	SCENE_EXITS(AltmillerCaveSquall),
	SCENE_EXIT(ALTMILLER_CAVE_SQUALL_TO_ALTMILLER_CAVE_VIAL_CHEST,
			   AltmillerCaveVialChest,
			   ALTMILLER_CAVE_VIAL_CHEST_FROM_ALTMILLER_CAVE_SQUALL),
	SCENE_EXIT(ALTMILLER_CAVE_SQUALL_TO_ALTMILLER_CAVE_COOKIE_CHEST,
			   AltmillerCaveCookieChest,
			   ALTMILLER_CAVE_COOKIE_CHEST_FROM_ALTMILLER_CAVE_SQUALL),
	SCENE_EXITS(AltmillerCaveCookieChest),
	SCENE_EXIT(ALTMILLER_CAVE_COOKIE_CHEST_TO_ALTMILLER_CAVE_SQUALL,
			   AltmillerCaveSquall,
			   ALTMILLER_CAVE_SQUALL_FROM_ALTMILLER_CAVE_COOKIE_CHEST),
	SCENE_EXIT(ALTMILLER_CAVE_COOKIE_CHEST_TO_ALTMILLER_CAVE_DRAUGHT,
			   AltmillerCaveDraught,
			   ALTMILLER_CAVE_DRAUGHT_FROM_ALTMILLER_CAVE_COOKIE_CHEST),
	SCENE_EXITS(AltmillerCaveDraught),
	SCENE_EXIT(ALTMILLER_CAVE_DRAUGHT_TO_ALTMILLER_CAVE_COOKIE_CHEST,
			   AltmillerCaveCookieChest,
			   ALTMILLER_CAVE_COOKIE_CHEST_FROM_ALTMILLER_CAVE_DRAUGHT),
	SCENE_EXIT(ALTMILLER_CAVE_DRAUGHT_TO_ROCKS, AltmillerCaveDraught,
			   ALTMILLER_CAVE_DRAUGHT_ROCKS_FROM_PILLAR_CAVE),
	SCENE_EXIT(ALTMILLER_CAVE_DRAUGHT_ROCKS_TO_PILLAR_CAVE,
			   AltmillerCaveDraught, ALTMILLER_CAVE_DRAUGHT_FROM_ROCKS),
	SCENE_EXIT(ALTMILLER_CAVE_DRAUGHT_ROCKS_TO_CHEST, AltmillerCaveDraught,
			   ALTMILLER_CAVE_DRAUGHT_CHEST_FROM_ROCKS),
	SCENE_EXIT(ALTMILLER_CAVE_DRAUGHT_CHEST_TO_ROCKS, AltmillerCaveDraught,
			   ALTMILLER_CAVE_DRAUGHT_ROCKS_FROM_CHEST),
	SCENE_EXITS_END,
};

ACTOR_PLACE AltmillerCave_ActorPlacementsOther[] = {
	{SCENE_TABLE_END},
};

ACTOR_PLACE AltmillerCave_BabiActorPlacements[] = {
	ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0,
					FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0,
					FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0,
					FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_INVISIBLE_BABI, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 232, 0, 160, FACING_SOUTH,
					TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BABI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -64, 0,
					-64, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SOLDIER_WITH_TORCH, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, -64, 0, -64, FACING_NORTH,
					TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SOLDIER_WITH_TORCH, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, -64, 0, -64, FACING_NORTH,
					TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

ACTOR_PLACE AltmillerCave_SquallActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_NORTH_SOUTH, CONDITION_ALWAYS,
					MOVEMENT_TYPE_RESET_ACTION, 328, 0, 256, FACING_SOUTH,
					TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_WEST_EAST, CONDITION_ALWAYS,
					MOVEMENT_TYPE_RESET_ACTION, 352, 0, 296, FACING_SOUTH,
					TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_WEST_EAST, CONDITION_ALWAYS,
					MOVEMENT_TYPE_RESET_ACTION, 368, 0, 200, FACING_SOUTH,
					TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376,
					0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
					472, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_JUPITER_DJINNI, FLAG_DJINNI(DJINNI_SQUALL),
					MOVEMENT_TYPE_STAND, 264, 0, 200, FACING_SOUTHWEST,
					TALK_FACING_PARTY, 0),
	{SCENE_TABLE_END},
};

ACTOR_PLACE AltmillerCave_DraughtActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_NORTH_SOUTH, CONDITION_ALWAYS,
					MOVEMENT_TYPE_RESET_ACTION, 392, 0, 416, FACING_SOUTH,
					TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_WEST_EAST, CONDITION_ALWAYS,
					MOVEMENT_TYPE_RESET_ACTION, 416, 0, 456, FACING_SOUTH,
					TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_NORTH_SOUTH, CONDITION_ALWAYS,
					MOVEMENT_TYPE_RESET_ACTION, 456, 0, 480, FACING_SOUTH,
					TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_LIGHT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 8,
					1752, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_LIGHT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 472,
					40, 1752, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_LIGHT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504,
					56, 1752, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_LIGHT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536,
					40, 1752, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_LIGHT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 8,
					1752, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_TURNING_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
					440, 0, 190, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_TURNING_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
					472, 0, 174, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_TURNING_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
					504, 0, 174, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_TURNING_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
					536, 0, 174, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_TURNING_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
					568, 0, 190, FACING_SOUTH, TALK_FACING_PARTY, 0),
	{SCENE_TABLE_END},
};

/* The sizes the light at the top pulses through. */
u32 AltmillerCave_PulseScales[] = {
	FIXED_ONE,
	11 * FIXED_ONE / 10,
	6 * FIXED_ONE / 5,
	11 * FIXED_ONE / 10,
};

/* The two men from Tolbi walk in to Babi from the south. */
s32 AltmillerCave_GuardsArrivePath[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE_BY(0, 0, -32),
	SC_SYNC,
	SC_MOVE_BY(-64, 0, 0),
	SC_SYNC,
	SC_MOVE_BY(0, 0, -64),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

/* Babi and the two men walk out of the room, each by his own path. */
s32 AltmillerCave_SecondGuardLeavePath[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE_BY(16, 0, 0),
	SC_SYNC,
	SC_MOVE_BY(0, 0, 32),
	SC_SYNC,
	SC_MOVE_BY(48, 0, 0),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

s32 AltmillerCave_FirstGuardLeavePath[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE_BY(0, 0, 16),
	SC_SYNC,
	SC_MOVE_BY(16, 0, 0),
	SC_SYNC,
	SC_MOVE_BY(0, 0, 32),
	SC_SYNC,
	SC_MOVE_BY(32, 0, 0),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

s32 AltmillerCave_BabiLeavePath[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE_BY(0, 0, 32),
	SC_SYNC,
	SC_MOVE_BY(16, 0, 0),
	SC_SYNC,
	SC_MOVE_BY(0, 0, 32),
	SC_SYNC,
	SC_MOVE_BY(16, 0, 0),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

/* clang-format off */
TRIGGER AltmillerCave_TriggersOther[] = {
    {TRIGGER_EXIT, ALTMILLER_CAVE_ENTRANCE_TO_WORLD_MAP, CONDITION_ALWAYS, ALTMILLER_CAVE_ENTRANCE_TO_WORLD_MAP},
    {TRIGGER_EXIT, ALTMILLER_CAVE_ENTRANCE_TO_TORCH_CHAMBER, CONDITION_ALWAYS, ALTMILLER_CAVE_ENTRANCE_TO_TORCH_CHAMBER},
    {TRIGGER_EXIT, ALTMILLER_CAVE_ENTRANCE_TORCH_CHAMBER_TO_PASSAGE, CONDITION_ALWAYS, ALTMILLER_CAVE_ENTRANCE_TORCH_CHAMBER_TO_PASSAGE},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, ALTMILLER_CAVE_ENTRANCE_TO_ALTMILLER_CAVE_BABI, CONDITION_ALWAYS, ALTMILLER_CAVE_ENTRANCE_TO_ALTMILLER_CAVE_BABI},
    {TRIGGER_STEP, CELL_BEHAVIOR_ALTMILLER_CAVE_FULL_LIGHT, CONDITION_ALWAYS, EVENT(scAltmillerCaveDarkenToLevel0)},
    {TRIGGER_STEP, CELL_BEHAVIOR_ALTMILLER_CAVE_DARKNESS_1, CONDITION_ALWAYS, EVENT(scAltmillerCaveDarkenToLevel1)},
    {TRIGGER_STEP, CELL_BEHAVIOR_ALTMILLER_CAVE_DARKNESS_2, CONDITION_ALWAYS, EVENT(scAltmillerCaveDarkenToLevel2)},
    {TRIGGER_STEP, CELL_BEHAVIOR_ALTMILLER_CAVE_DARKNESS_3, CONDITION_ALWAYS, EVENT(scAltmillerCaveDarkenToLevel3)},
    {TRIGGER_STEP, CELL_BEHAVIOR_ALTMILLER_CAVE_DARKNESS_4, CONDITION_ALWAYS, EVENT(scAltmillerCaveDarkenToLevel4)},
    {TRIGGER_STEP, CELL_BEHAVIOR_ALTMILLER_CAVE_DARKNESS_5, CONDITION_ALWAYS, EVENT(scAltmillerCaveDarkenToLevel5)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS,
        EVENT(scAltmillerCaveStartReveal)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS,
        EVENT(scAltmillerCaveEndReveal)},
    {PSYNERGY_ANYWHERE(ABILITY_RETREAT, PSYNERGY_PHASE_BEFORE_CAST), 0, CONDITION_ALWAYS, EVENT(scAltmillerCaveStartRetreat)},
    {TRIGGER_TABLE_END},
};

TRIGGER AltmillerCave_BabiTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, ALTMILLER_CAVE_BABI_TO_ALTMILLER_CAVE_ENTRANCE, CONDITION_ALWAYS, ALTMILLER_CAVE_BABI_TO_ALTMILLER_CAVE_ENTRANCE},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, ALTMILLER_CAVE_BABI_TO_ALTMILLER_CAVE_VIAL_CHEST, CONDITION_ALWAYS, ALTMILLER_CAVE_BABI_TO_ALTMILLER_CAVE_VIAL_CHEST},
    {TRIGGER_TALK, ACTOR_INVISIBLE_BABI, FLAG_MET_INVISIBLE_BABI, EVENT(scAltmillerCaveMeetInvisibleBabi)},
    {TRIGGER_TALK, ACTOR_INVISIBLE_BABI, FLAG_ALTMILLER_CAVE_MYSTIC_DRAUGHT, EVENT(scAltmillerCaveRepeatRockOrder)},
    {TRIGGER_TALK, ACTOR_INVISIBLE_BABI, FLAG_BABI_SAVED, EVENT(scAltmillerCaveRestoreBabiAndInviteParty)},
    {TRIGGER_STEP, CELL_BEHAVIOR_ALTMILLER_CAVE_BESIDE_BABI, FLAG_BABI_CALLED_PARTY_BACK, EVENT(scAltmillerCaveBabiCallsPartyBack)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TRIGGER_SUBJECT_OWN_EFFECT | ACTOR_INVISIBLE_BABI, FLAG_MET_INVISIBLE_BABI,
        EVENT(scAltmillerCaveMeetInvisibleBabi)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INVISIBLE_BABI, FLAG_ALTMILLER_CAVE_MYSTIC_DRAUGHT, MSGNO(MsgAltmillerNeedDraught)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TRIGGER_SUBJECT_OWN_EFFECT | ACTOR_INVISIBLE_BABI, FLAG_BABI_SAVED,
        EVENT(scAltmillerCaveRestoreBabiAndInviteParty)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS,
        EVENT(scAltmillerCaveEndReveal)},
    {PSYNERGY_ANYWHERE(ABILITY_RETREAT, PSYNERGY_PHASE_BEFORE_CAST), 0, CONDITION_ALWAYS, EVENT(scAltmillerCaveStartRetreat)},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), CELL_BEHAVIOR_ALTMILLER_CAVE_CHEST, FLAG_ALTMILLER_CAVE_MIMIC_BEATEN, FIND_MIMIC | 5},
    {TRIGGER_TABLE_END},
};

TRIGGER AltmillerCave_VialChestTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, ALTMILLER_CAVE_VIAL_CHEST_TO_ALTMILLER_CAVE_BABI, CONDITION_ALWAYS, ALTMILLER_CAVE_VIAL_CHEST_TO_ALTMILLER_CAVE_BABI},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, ALTMILLER_CAVE_VIAL_CHEST_TO_ALTMILLER_CAVE_SQUALL, CONDITION_ALWAYS, ALTMILLER_CAVE_VIAL_CHEST_TO_ALTMILLER_CAVE_SQUALL},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS,
        EVENT(scAltmillerCaveEndReveal)},
    {PSYNERGY_ANYWHERE(ABILITY_RETREAT, PSYNERGY_PHASE_BEFORE_CAST), 0, CONDITION_ALWAYS, EVENT(scAltmillerCaveStartRetreat)},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), CELL_BEHAVIOR_ALTMILLER_CAVE_CHEST, FLAG_ALTMILLER_CAVE_VIAL, FIND_ITEM(ITEM_VIAL)},
    {TRIGGER_TABLE_END},
};

TRIGGER AltmillerCave_SquallTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, ALTMILLER_CAVE_SQUALL_TO_ALTMILLER_CAVE_VIAL_CHEST, CONDITION_ALWAYS, ALTMILLER_CAVE_SQUALL_TO_ALTMILLER_CAVE_VIAL_CHEST},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, ALTMILLER_CAVE_SQUALL_TO_ALTMILLER_CAVE_COOKIE_CHEST, CONDITION_ALWAYS, ALTMILLER_CAVE_SQUALL_TO_ALTMILLER_CAVE_COOKIE_CHEST},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_ALTMILLER_CAVE_NORTH_OF_PUDDLE, FLAG_ALTMILLER_CAVE_PUDDLE_FROZEN, EVENT(scAltmillerCaveHopOverPuddle)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_ALTMILLER_CAVE_EAST_OF_PUDDLE, FLAG_ALTMILLER_CAVE_PUDDLE_FROZEN, EVENT(scAltmillerCaveHopOverPuddle)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_ALTMILLER_CAVE_SOUTH_OF_PUDDLE, FLAG_ALTMILLER_CAVE_PUDDLE_FROZEN, EVENT(scAltmillerCaveHopOverPuddle)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_ALTMILLER_CAVE_WEST_OF_PUDDLE, FLAG_ALTMILLER_CAVE_PUDDLE_FROZEN, EVENT(scAltmillerCaveHopOverPuddle)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, CELL_BEHAVIOR_ALTMILLER_CAVE_SQUALL_COLUMN_FLOOR, CONDITION_ALWAYS, EVENT(scPushedActorPushIfClear)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_ALTMILLER_CAVE_WEST_OF_PILLAR, CONDITION_ALWAYS, EVENT(scAltmillerCavePushPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SQUALL_ROOM_PILLAR, CONDITION_ALWAYS, EVENT(scAltmillerCaveDropPillarIntoPit)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_FROST, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_SQUALL_ROOM_PUDDLE, FLAG_ALTMILLER_CAVE_PUDDLE_FROZEN, EVENT(scAltmillerCaveCheckPuddleCovered)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SQUALL_ROOM_PUDDLE, FLAG_ALTMILLER_CAVE_PUDDLE_FROZEN, EVENT(scAltmillerCaveFreezePuddle)},
    {TRIGGER_TALK, ACTOR_SQUALL, CONDITION_ALWAYS, EVENT(scAltmillerCaveStartDjinnBattle)},
    {TRIGGER_TABLE_END},
};

TRIGGER AltmillerCave_CookieChestTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, ALTMILLER_CAVE_COOKIE_CHEST_TO_ALTMILLER_CAVE_SQUALL, CONDITION_ALWAYS, ALTMILLER_CAVE_COOKIE_CHEST_TO_ALTMILLER_CAVE_SQUALL},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, ALTMILLER_CAVE_COOKIE_CHEST_TO_ALTMILLER_CAVE_DRAUGHT, CONDITION_ALWAYS, ALTMILLER_CAVE_COOKIE_CHEST_TO_ALTMILLER_CAVE_DRAUGHT},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS,
        EVENT(scAltmillerCaveEndReveal)},
    {PSYNERGY_ANYWHERE(ABILITY_RETREAT, PSYNERGY_PHASE_BEFORE_CAST), 0, CONDITION_ALWAYS, EVENT(scAltmillerCaveStartRetreat)},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), CELL_BEHAVIOR_ALTMILLER_CAVE_CHEST, FLAG_ALTMILLER_CAVE_COOKIE, FIND_ITEM(ITEM_COOKIE)},
    {TRIGGER_TABLE_END},
};
/* clang-format on */

TRIGGER AltmillerCave_DraughtTriggers[] = {
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP,
	 ALTMILLER_CAVE_DRAUGHT_TO_ALTMILLER_CAVE_COOKIE_CHEST, CONDITION_ALWAYS,
	 ALTMILLER_CAVE_DRAUGHT_TO_ALTMILLER_CAVE_COOKIE_CHEST},
	{TRIGGER_EXIT, ALTMILLER_CAVE_DRAUGHT_TO_ROCKS, CONDITION_ALWAYS,
	 ALTMILLER_CAVE_DRAUGHT_TO_ROCKS},
	{TRIGGER_EXIT, ALTMILLER_CAVE_DRAUGHT_ROCKS_TO_PILLAR_CAVE,
	 CONDITION_ALWAYS, ALTMILLER_CAVE_DRAUGHT_ROCKS_TO_PILLAR_CAVE},
	{TRIGGER_EXIT, ALTMILLER_CAVE_DRAUGHT_ROCKS_TO_CHEST, CONDITION_ALWAYS,
	 ALTMILLER_CAVE_DRAUGHT_ROCKS_TO_CHEST},
	{TRIGGER_EXIT, ALTMILLER_CAVE_DRAUGHT_CHEST_TO_ROCKS, CONDITION_ALWAYS,
	 ALTMILLER_CAVE_DRAUGHT_CHEST_TO_ROCKS},
	{TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING,
	 CELL_BEHAVIOR_ALTMILLER_CAVE_DRAUGHT_COLUMN_FLOOR, CONDITION_ALWAYS,
	 EVENT(scPushedActorPushIfClear)},
	{TRIGGER_TALK, ACTOR_BLUE_ROCK, CONDITION_ALWAYS,
	 EVENT(scAltmillerCaveTurnRock)},
	{TRIGGER_TALK, ACTOR_GREEN_ROCK, CONDITION_ALWAYS,
	 EVENT(scAltmillerCaveTurnRock)},
	{TRIGGER_TALK, ACTOR_WHITE_ROCK, CONDITION_ALWAYS,
	 EVENT(scAltmillerCaveTurnRock)},
	{TRIGGER_TALK, ACTOR_YELLOW_ROCK, CONDITION_ALWAYS,
	 EVENT(scAltmillerCaveTurnRock)},
	{TRIGGER_TALK, ACTOR_RED_ROCK, CONDITION_ALWAYS,
	 EVENT(scAltmillerCaveTurnRock)},
	{EXAMINE_TRIGGER(EXAMINE_CHEST), CELL_BEHAVIOR_ALTMILLER_CAVE_CHEST,
	 FLAG_ALTMILLER_CAVE_MYSTIC_DRAUGHT, FIND_ITEM(ITEM_MYSTIC_DRAUGHT)},
	{PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
	 CONDITION_ALWAYS, EVENT(scAltmillerCaveStartReveal)},
	{PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
	 CONDITION_ALWAYS, EVENT(scAltmillerCaveEndReveal)},
	{TRIGGER_TABLE_END},
};

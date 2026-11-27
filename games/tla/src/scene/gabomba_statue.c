/* @file */
/* Inside the Gabomba Statue at Kibombo, the seven rooms of this overlay: halls
 */
/* of turning cogs and pinions that Felix rides and hops across. */
/* */
/* From the statue grounds the party comes into the hall of pinions, whose */
/* turning pinions in the south-west pull Felix through to the north. Above */
/* lie the room of corridors with the Bone Armlet's chest, and the room of the
 */
/* flashing cog, where Lash on a rope stops every cog */
/* (scGabombaStatueLashRope) so that Felix can cross to the ladder up to */
/* Steel's room and the mouse's room, where a mouse shows how to hop from cog */
/* to cog (scGabombaStatueMouseShowsTheWay). Back in the room of the flashing */
/* cog, Pound on the two stakes beside it turns all the cogs the other way */
/* (scGabombaStatuePoundStake); a pink drum in the mouse's room then drops */
/* Felix beside the Venus Djinni Steel. The way on leads through the pinions */
/* of the room below the small idol's chamber, up to the chamber and to the */
/* room of the arrow panels behind the statue's eyes. There the party watches */
/* the ceremony's two magical charges stick in a broken channel and mends it */
/* (scGabombaStatueFocusEnergies to scGabombaStatueKradenNoticesTiming): Pound
 */
/* on the stake raises the channel's blocks, the arrow panels turn them, and */
/* Pound sets each back in, until the blue and the red orb statues' charges */
/* reach the statue in the middle at once. */

/* This unit declares its own form of fdActorStartIdle, below. */
#define OWN_Actor_StartIdle

#include "../field/actor_set_sprite_priority.c"

#include "../field/field_effect_move.c"

/* The ROM's callers here are compiled with no declaration in scope, as against
 */
/* this form: no parameter list and an s32 result, where the definition takes */
/* the actor and returns nothing. One of them passes no actor. */
s32 fdActorStartIdle();

/* Both stakes have been Pounded and the cogs turned the other way. */
#define FLAG_GABOMBA_COGS_TURNED_BACK 0x8cd
/* The party has watched how the statue's works run. */
#define FLAG_GABOMBA_SAW_HOW_IT_WORKS 0x8f6
/* Kraden has said it is when the charges arrive that matters ("Maybe we need */
/* to look at when the two magical charges reach the statue in the middle"). */
#define FLAG_GABOMBA_TIMING_NOTICED		  0x9ec
#define FLAG_GABOMBA_FIRST_STAKE_POUNDED  0x8ce /* the west stake */
#define FLAG_GABOMBA_SECOND_STAKE_POUNDED 0x8cf /* the east stake */
/* The mouse that shows the way across the cogs of its room has been seen. */
#define FLAG_GABOMBA_MOUSE_SEEN 0xa6c
/* The two magical charges reached the statue in the middle at different */
/* times. */
#define FLAG_GABOMBA_CHARGES_REACHED_APART 0x9eb

/* The ladder-top blocks are drawn (scGabombaStatueRestoreLadderTopBlocks). */
#define FLAG_LADDER_TOP_BLOCKS_DRAWN  FLAG_SCENE_TEMP_00
#define FLAG_GABOMBA_FIRST_ROPE_TIED  FLAG_SCENE_TEMP_01 /* by fdRopePlace */
#define FLAG_GABOMBA_SECOND_ROPE_TIED FLAG_SCENE_TEMP_02 /* by fdRopePlace */
/* A tied rope has stopped the cogs (scGabombaStatueLashRope). */
#define FLAG_COGS_STOPPED FLAG_SCENE_TEMP_03
/* The party has fallen in beside Steel, who can then be talked to. */
#define FLAG_LANDED_BESIDE_STEEL FLAG_SCENE_TEMP_04
/* The offering below has been heard (scGabombaStatueHearTheOffering). */
#define FLAG_OFFERING_HEARD FLAG_SCENE_TEMP_05

/* The mouse that runs ahead across the cogs of its room and shows the way */
/* (scGabombaStatueMouseShowsTheWay). */
enum { ACTOR_MOUSE = 39 };

/* The arrows room's Kraden, who works out the channel with the party, and */
/* Akafubu, who comes up into the statue once the ceremony has worked */
/* (scGabombaStatueSeeHowItWorks, scGabombaStatueSawEnergyFlow). */
enum { ACTOR_KRADEN = 23, ACTOR_AKAFUBU = 24 };

/* Each room's placed actors. A row of cogs that share one sprite is named by */
/* the first of them: turning its animation turns them all. A Lashed rope runs
 */
/* between two posts, its second post riding a cog's tooth. A ladder-top block
 */
/* is a black square (SPRITE_UNSEEN_BLOCK) at the top of a ladder up to the */
/* room above, held 36 pixels up in SPRITE_COLOR_HIDDEN, so that nothing of */
/* it shows (scGabombaStatueHideActor). */
/* */
/* Drawn or hidden, the ladder-top blocks change nothing on screen: */
/* SPRITE_COLOR_HIDDEN draws every pixel of them transparent, and the display */
/* has no sprite window they could cut. */
enum { ACTOR_BONE_ARMLET_LADDER_TOP_BLOCK = 8 };

/* The Elixir chest's room. */
enum {
	ACTOR_FIRST_ROPE_POST_A = 8,
	ACTOR_FIRST_ROPE_POST_B,
	ACTOR_SECOND_ROPE_POST_A,
	ACTOR_SECOND_ROPE_POST_B,
	ACTOR_FIRST_STAKE,
	ACTOR_SECOND_STAKE,
	ACTOR_ELIXIR_GREEN_COGS,
	ACTOR_ELIXIR_BLUE_COGS = 21,
	/* The blue cog that flashes until both stakes are Pounded. */
	ACTOR_FLASHING_COG = 26,
	ACTOR_ELIXIR_PINION_A,
	ACTOR_ELIXIR_PINION_B,
	ACTOR_ELIXIR_WEST_LADDER_TOP_BLOCK,
	ACTOR_ELIXIR_EAST_LADDER_TOP_BLOCK
};
/* Steel's room. */
enum {
	ACTOR_STEEL_GREEN_COGS = 8,
	ACTOR_STEEL_BLUE_COGS = 15,
	ACTOR_STEEL_RED_COGS = 20,
	ACTOR_STEEL_BLUE_PILLARS = 25,
	ACTOR_STEEL_PINK_DRUMS = 29,
	ACTOR_STEEL_ROPE_POST_A = 31,
	ACTOR_STEEL_ROPE_POST_B
};
/* The mouse's room. */
enum {
	ACTOR_MOUSE_GREEN_COGS = 8,
	ACTOR_MOUSE_BLUE_COGS = 21,
	ACTOR_MOUSE_RED_COGS = 29,
	ACTOR_MOUSE_BLUE_PILLAR_A = 34,
	ACTOR_MOUSE_BLUE_PILLAR_B,
	ACTOR_MOUSE_PINK_DRUM_A = 37,
	ACTOR_MOUSE_PINK_DRUM_B
};
/* The cogs' room. */
enum {
	ACTOR_COGS_ROPE_POST_A = 8,
	ACTOR_COGS_ROPE_POST_B,
	ACTOR_COGS_RED_COGS,
	ACTOR_COGS_PINION_A = 12,
	ACTOR_COGS_PINION_B
};
/* The arrows room: the statue in the middle, where the charges must arrive, */
/* the statues holding the blue and the red orb whose charges run along the */
/* channel, the stake whose Pound raises the blocks, and the seven loose blocks
 */
/* of the channel ("It looks like the energy charge is supposed to run over */
/* these blocks."). */
enum {
	ACTOR_MIDDLE_STATUE = 8,
	ACTOR_BLUE_ORB_STATUE,
	ACTOR_RED_ORB_STATUE,
	ACTOR_CHARGE_A, /* the blue orb's two charges, then the red orb's */
	ACTOR_CHARGE_B,
	ACTOR_CHARGE_C,
	ACTOR_CHARGE_D,
	ACTOR_ARROWS_STAKE,
	ACTOR_BLOCK_A,
	ACTOR_BLOCK_B,
	ACTOR_BLOCK_C,
	ACTOR_BLOCK_D,
	ACTOR_BLOCK_E,
	ACTOR_BLOCK_F,
	ACTOR_BLOCK_G
};
/* The hall of pinions. */
enum {
	ACTOR_PINION_A = 8,
	ACTOR_PINION_B,
	ACTOR_PINION_C,
	ACTOR_PINION_D,
	ACTOR_PINION_E
};

/* The cell behaviors of the rooms' trigger tables. A hole is the top of a */
/* ladder down to the room below, and its number is that of the exit it */
/* leaves by. Between two pinions the leader is pulled through to the */
/* other side, the cells taking the second behavior while the cogs turn back */
/* (scGabombaStatueTurnCogs copies them in). The cells under a cog */
/* (scGabombaStatueDrawCogCells) carry the leader with it, and the cells past a
 */
/* cog's ends hop him across to the next. */
typedef enum GABOMBA_STATUE_CELL_BEHAVIOR {
	CELL_BEHAVIOR_FIRST_LADDER_DOWN = 1,
	CELL_BEHAVIOR_SECOND_LADDER_DOWN,
	CELL_BEHAVIOR_THIRD_LADDER_DOWN,
	CELL_BEHAVIOR_PINION_GAP = 10,
	CELL_BEHAVIOR_PINION_GAP_TURNED_BACK,
	CELL_BEHAVIOR_MOUSE = 77, /* where the mouse pops up */
	CELL_BEHAVIOR_GREEN_COG = 80,
	CELL_BEHAVIOR_BLUE_COG,
	CELL_BEHAVIOR_RED_COG,
	CELL_BEHAVIOR_ROLLER, /* a roller: a blue pillar or a pink drum */
	CELL_BEHAVIOR_COG_END,
	/* The cell past a pink drum's south end, which no trigger reads. */
	CELL_BEHAVIOR_PINK_DRUM_SOUTH_END
} GABOMBA_STATUE_CELL_BEHAVIOR;

/* The arrows room's: the panel with an arrow, and the cells around it that */
/* raise it again once the leader steps off. */
typedef enum GABOMBA_STATUE_ARROW_CELL_BEHAVIOR {
	CELL_BEHAVIOR_AROUND_LEFT_ARROW = 80,
	CELL_BEHAVIOR_LEFT_ARROW,
	CELL_BEHAVIOR_AROUND_RIGHT_ARROW,
	CELL_BEHAVIOR_RIGHT_ARROW
} GABOMBA_STATUE_ARROW_CELL_BEHAVIOR;

/* The triggers this overlay raises: both stakes Pounded, the arrows room's */
/* stake Pounded, and the offering's clock run out. */
enum {
	RAISED_COGS_TURN_BACK = 200,
	RAISED_STAKE_STANDS_UP = 200,
	RAISED_OFFERING_HEARD = 201
};
#include "graphics/palette_buffers.h"
#include "field/event_inline.h"
#include "field/field_sequence.h"
#include "scene/gabomba_statue.h"
#include "scene/scene.h"
#include "game/game.h"
#include "macros.h"
#include "field/object.h"
#include "field/call.h"
#include "field/map.h"
#include "field/actor_script.h"
#include "text/msg_ids.h"

extern const s32 *FieldEffect_DurationScripts[];

#define EFFECT_SET_MODE	  fdActorSelectAnimation
#define EFFECT_SET_SCRIPT fdActorStartScript
#include "../field/field_effect_spawn.c"

/* Steel, the Venus Djinni waiting in Steel's room. */
enum { ACTOR_STEEL = 33 };

/* Talking to Steel, beside whom Felix lands after riding a pink drum off the */
/* mouse's room, starts its battle. */
void scGabombaStatueFightSteel(void) {
	fdRequestDjinnBattle(ACTOR_STEEL, DJINNI_STEEL);
}

/* A member of the party looking about: it turns between its two headings */
/* every 10 to 49 frames. */
s32 scGabombaStatueLookAround(ACTOR *actor) {
	if(actor->lookTmr != 0) {
		actor->lookTmr--;
	}
	else {
		actor->lookTmr = (syRandomBelow(40)) + 10;
		if(actor->lookTurned != 0) {
			actor->lookTurned = 0;
			actor->facing = actor->lookFacing;
		}
		else {
			actor->lookTurned = 1;
			actor->facing = actor->lookOtherFacing;
		}
	}
	return 1;
}

const SCENE_ENTRANCE *scGetEntrances(void) { return SceneEntrances; }

extern SCENE_REGION GabombaStatue_PinionExitRegions[];
extern SCENE_REGION GabombaStatue_BoneArmletChestExitRegions[];
extern SCENE_REGION GabombaStatue_ElixirChestExitRegions[];
extern SCENE_REGION GabombaStatue_SteelExitRegions[];
extern SCENE_REGION GabombaStatue_MouseExitRegions[];
extern SCENE_REGION GabombaStatue_CogsExitRegions[];

const SCENE_REGION *scGetExitRegions(void) {
	s16 scene = MainRoutineStateWork.next;

	if(scene == SCENE_GABOMBA_STATUE_PINION)
		return GabombaStatue_PinionExitRegions;
	if(scene == SCENE_GABOMBA_STATUE_BONE_ARMLET_CHEST)
		return GabombaStatue_BoneArmletChestExitRegions;
	if(scene == SCENE_GABOMBA_STATUE_ELIXIR_CHEST)
		return GabombaStatue_ElixirChestExitRegions;
	if(scene == SCENE_GABOMBA_STATUE_STEEL)
		return GabombaStatue_SteelExitRegions;
	if(scene == SCENE_GABOMBA_STATUE_MOUSE)
		return GabombaStatue_MouseExitRegions;
	if(scene == SCENE_GABOMBA_STATUE_COGS)
		return GabombaStatue_CogsExitRegions;
	return NULL;
}

const u32 *scGetExits(void) { return SceneExits; }

/* The flashing cog's update: its colours swap between two orders every ten */
/* frames. */
s32 scGabombaStatueFlashCog(ACTOR *actor) {
	u32 frame = FrameCount;

	if(frame % 10 == 0)
		((SPR *)actor->spr)->entries[0]->colMode =
			frame % 20 / 10 + SPRITE_COLOR_BGR;
	return 1;
}

/* A room's ropes, by the index the rope functions take; the room of the */
/* flashing cog has two. */
enum { FIRST_ROPE, SECOND_ROPE };

/* The steps of a cog tooth's turn: its phase, kept in the tooth's */
/* sceneCnt, counts them, two degrees each. */
enum { COG_TOOTH_STEPS = 180 };

/* The arrows room's channel: a grid of blocks, eleven columns by eight rows, */
/* from the map's cell (19, 11). The statue in the middle, where the charges */
/* must arrive, stands on its cell (5, 3), and the two orb statues on (1, 7) */
/* and (9, 7). */
enum {
	CHANNEL_COLUMNS = 11,
	CHANNEL_ROWS = 8,
	CHANNEL_CELLS = CHANNEL_COLUMNS * CHANNEL_ROWS,
	CHANNEL_ORIGIN_COLUMN = 19,
	CHANNEL_ORIGIN_ROW = 11,
	CHANNEL_MIDDLE_STATUE_COLUMN = 5,
	CHANNEL_MIDDLE_STATUE_ROW = 3,
	CHANNEL_BLUE_STATUE_COLUMN = 1,
	CHANNEL_RED_STATUE_COLUMN = 9,
	CHANNEL_STATUES_ROW = 7
};

/* The way a charge runs along the channel, and the way a path leaves a cell. */
typedef enum CHANNEL_HEADING {
	HEADING_EAST,
	HEADING_SOUTH,
	HEADING_WEST,
	HEADING_NORTH
} CHANNEL_HEADING;

/* A heading's bit in a set of headings. */
#define HEADING_BIT(heading) (1 << (heading))

/* A path's tried headings once every one has been tried. */
#define HEADING_ALL_TRIED                                                      \
	(HEADING_BIT(HEADING_EAST) | HEADING_BIT(HEADING_SOUTH) |                  \
	 HEADING_BIT(HEADING_WEST) | HEADING_BIT(HEADING_NORTH))

/* The pieces of the channel, by the sides of the cell they join, which are */
/* also the rows of the cell tiles the map keeps for them. */
typedef enum CHANNEL_PIECE {
	PIECE_NONE,
	PIECE_NORTH_SOUTH,
	PIECE_EAST_WEST,
	PIECE_CROSSING,
	PIECE_EAST_SOUTH,
	PIECE_SOUTH_WEST,
	PIECE_WEST_NORTH,
	PIECE_NORTH_EAST,
	PIECE_EAST_SOUTH_AND_WEST_NORTH, /* two curves */
	PIECE_SOUTH_WEST_AND_NORTH_EAST, /* two curves */
	/* No channel: the statues and a block with no piece on it stop a charge. */
	PIECE_STOP,
	PIECE_SOCKET, /* the hole a raised block leaves */
	PIECE_KINDS
} CHANNEL_PIECE;

/* The four charges' paths, by the bit of PUZZLE_CELL.state that marks a cell */
/* one runs through (scGabombaStatueLayPath takes the bit's number). Each orb */
/* statue sends one charge west and one east. */
enum {
	PATH_BIT_BLUE_WEST,
	PATH_BIT_RED_EAST,
	PATH_BIT_BLUE_EAST,
	PATH_BIT_RED_WEST
};

/* The bits of PUZZLE_CELL.state: which of the four charges' paths run through
 */
/* the cell. */
typedef enum CHANNEL_PATH {
	PATH_BLUE_WEST = 1 << PATH_BIT_BLUE_WEST,
	PATH_RED_EAST = 1 << PATH_BIT_RED_EAST,
	PATH_BLUE_EAST = 1 << PATH_BIT_BLUE_EAST,
	PATH_RED_WEST = 1 << PATH_BIT_RED_WEST,
	PATH_BLUE = PATH_BLUE_WEST | PATH_BLUE_EAST,
	PATH_RED = PATH_RED_WEST | PATH_RED_EAST
} CHANNEL_PATH;

/* What stands on a cell of the channel (PUZZLE_CELL.occupied). The map keeps */
/* each piece's cell tiles in a row of their own (CHANNEL_PIECE), a */
/* column for each of these: the column fdMapGridCopyCells copies from is what
 */
/* stands on the cell. */
enum {
	BLOCK_NONE,		/* its piece is part of the floor */
	BLOCK_IN_PLACE, /* a loose block lies in it, to be turned or raised */
	BLOCK_RAISED	/* its block floats over the room, the socket open */
};

/* ACTOR.chargeState. While a charge circles its orb statue before the run, */
/* the same byte holds its actor id (scGabombaStatueSendCharge); the end of its
 */
/* burst or shrink script sets CHARGE_STOPPED. */
enum {
	CHARGE_STOPPED = 0,
	CHARGE_RUNNING = 1,
	CHARGE_LEAVING = 2, /* bursting or shrinking away off the channel */
	CHARGE_ARRIVED = 7	/* at the statue in the middle */
};

/* The blocks of the channel, one for each of the room's block actors. */
enum { CHANNEL_BLOCKS = 7 };

/* A Pound handler runs each frame of the fist's fall with (landed, frame): */
/* landed is 1 on the frame the fist lands, POUND_LAND_FRAME, and what it */
/* strikes sinks a step on each frame before. */
enum { POUND_LAND_FRAME = 8 };

/* The channel blocks' animations, which all the blocks share with the first: */
/* the green crystal spins one way as the right arrow panel turns them */
/* clockwise, and back as the left turns them counterclockwise. */
enum { BLOCK_ANIM_TURN_CLOCKWISE = 2, BLOCK_ANIM_TURN_COUNTERCLOCKWISE = 3 };

/* The two ways a block turns (scGabombaStatueTurnBlockPiece). */
enum { TURN_COUNTERCLOCKWISE, TURN_CLOCKWISE };

/* A charge's step along the channel: where it heads and the piece under it. */
#define CHARGE_STEP(heading, piece) (((heading) << 8) + (piece))
extern s32 GabombaStatue_CogDirection;

/* Where the cog's tooth an actor rides is now: the rope's target hook, so that
 */
/* a rope tied to it follows the turning cog. */
void scGabombaStatueGetToothPos(ACTOR *actor, FIXED *x, FIXED *z) {
	FIXED position[3];
	s32 angle = actor->sceneCnt * TRIG_TURN / COG_TOOTH_STEPS;
	s32 turn;

	SET_VECTOR(position, INTtoFIXED(actor->homeX + 16), toFIXED(16),
			   INTtoFIXED(actor->homeZ));
	turn = GabombaStatue_CogDirection * (TRIG_TURN * 3 / 16) + angle;
	syFieldVectorAddPolarOffset(13, turn + DEGtoANG(180), position);
	*x = position[0];
	*z = position[2];
}

/* A cog tooth's update: it turns about the cog's centre by the cogs' */
/* direction. */
s32 scGabombaStatueTurnTooth(ACTOR *actor) {
	u8 *phase = &actor->sceneCnt;
	s32 angle = *phase * TRIG_TURN / COG_TOOTH_STEPS;
	s32 next;

	actor->priFlg = 0;
	actor->x.fixed = INTtoFIXED(actor->homeX + 16);
	actor->y.fixed = toFIXED(16);
	actor->z.fixed = INTtoFIXED(actor->homeZ);
	syFieldVectorAddPolarOffset(13, angle + DEGtoANG(180), &actor->x.fixed);
	next = *phase + GabombaStatue_CogDirection;
	if(next < 0)
		next = COG_TOOTH_STEPS - 1;
	if(next > COG_TOOTH_STEPS - 1)
		next = 0;
	*phase = next;
	actor->watchedActor = (FIELD_OBJECT *)scGabombaStatueGetToothPos;
	return 1;
}

/* A sprite as the depth queue takes it (QUEUED_SPR), its affine mode
 */
/* and matrix written as bytes. */
typedef struct SPR_VIEW {
	u8 link[4]; /* the depth queue's link to the next sprite */
	u8 y;
	u8 affineMode : 2;
	u8 modeAndShape : 6; /* object mode, mosaic, colours and shape */
	u8 xLow;
	u8 xHigh : 1;
	u8 matrix : 5;
	u8 size : 2;
	u32 attribute2;
} SPR_VIEW;

/* The channel puzzle's state: its grid of pieces, laid from a random seed, and
 */
/* the blocks and arrows drawn over it. */
typedef struct SCENE_STATE {
	PUZZLE_CELL cells[CHANNEL_ROWS][CHANNEL_COLUMNS];
	s32 seed; /* the random seed the channel was laid from */
	s16 firstBlock;
	s16 endBlock;
	s16 blockTileEntry; /* the VRAM cache entry of the blocks' pictures */
	s8 leftArrowDown;
	s8 rightArrowDown;
	s16 originX;
	s16 originZ;
	s16 blocksRaised;
	u8 unusedAfterBlocksRaised[2];
	s32 blocksAng;
	SPR_VIEW oam[10];
} SCENE_STATE;

/* The scene's view of SceneStateWork (system/globals.h). */
#define SceneStateWork (*(SCENE_STATE *)SceneStateWork)
extern s8 GabombaStatue_CogAnimations[];

/* How far a charge circles its orb statue each frame, in 256ths of a turn. */
enum { CIRCLE_STEP = 16 };

/* Each frame, keeps an actor circling the actor it watches. */
s32 scGabombaStatueCircleOwner(ACTOR *actor) {
	ACTOR *owner = &actor->watchedActor->actor;

	if(owner != NULL) {
		u8 *phase = &actor->sceneCnt;

		actor->x.fixed = owner->x.fixed;
		actor->y.fixed = owner->y.fixed + toFIXED(16);
		actor->z.fixed = owner->z.fixed;
		actor->facing = *phase << 8;
		syFieldVectorAddPolarOffset(6, *phase << 8, &actor->x.fixed);
		*phase += CIRCLE_STEP;
	}
	return 1;
}

extern ACTOR_PLACE GabombaStatue_Placements[];
extern ACTOR_PLACE GabombaStatue_BoneArmletChestActorPlacements[];
extern ACTOR_PLACE GabombaStatue_ElixirChestActorPlacements[];
extern ACTOR_PLACE GabombaStatue_SteelActorPlacements[];
extern ACTOR_PLACE GabombaStatue_MouseActorPlacements[];
extern ACTOR_PLACE GabombaStatue_CogsActorPlacements[];
extern ACTOR_PLACE GabombaStatue_ArrowsActorPlacements[];
extern ACTOR_PLACE GabombaStatue_PinionActorPlacements[];

const u8 *scGetActorPlacements(void) {
	s16 scene = MainRoutineStateWork.next;
	const u8 *table = GabombaStatue_Placements;

	if(scene == SCENE_GABOMBA_STATUE_BONE_ARMLET_CHEST)
		table = GabombaStatue_BoneArmletChestActorPlacements;
	else if(scene == SCENE_GABOMBA_STATUE_ELIXIR_CHEST)
		table = GabombaStatue_ElixirChestActorPlacements;
	else if(scene == SCENE_GABOMBA_STATUE_STEEL)
		table = GabombaStatue_SteelActorPlacements;
	else if(scene == SCENE_GABOMBA_STATUE_MOUSE)
		table = GabombaStatue_MouseActorPlacements;
	else if(scene == SCENE_GABOMBA_STATUE_COGS)
		table = GabombaStatue_CogsActorPlacements;
	else if(scene == SCENE_GABOMBA_STATUE_ARROWS)
		table = GabombaStatue_ArrowsActorPlacements;
	else if(scene == SCENE_GABOMBA_STATUE_PINION)
		table = GabombaStatue_PinionActorPlacements;
	return table;
}

extern s8 GabombaStatue_RopeStopSteps[];

/* The frames the cog's tooth takes to settle as the rope pulls taut. */
enum { ROPE_STOP_FRAMES = 25 };

/* Whether a and b are at most limit apart. */
#define DIFF_WITHIN(a, b, limit)                                               \
	((a) - (b) >= 0 ? (a) - (b) <= (limit) : (b) - (a) <= (limit))

/* Lash on the rope by the cogs in the north-west of the room of the flashing */
/* cog, between the posts at cells (18, 5) and (20, 5), ties it to a tooth of */
/* the blue cog. As it pulls taut the tooth rocks to a halt, and every cog in */
/* the room stops, so Felix can cross them to the ladder on the east side. */
void scGabombaStatueLashRope(void) {
	ACTOR *actor = fdActorGet(ACTOR_FIRST_ROPE_POST_B);
	s32 direction;
	s32 i;

	if(fdRopeGetState(FIRST_ROPE) == ROPE_THROWN) {
		direction = GabombaStatue_CogDirection;
		scGabombaStatueTurnCogs(0);
		fdEventSetFlag(FLAG_COGS_STOPPED);
		FieldStateWork->psynergy = 0;
		for(i = 0; i < ROPE_STOP_FRAMES; i++) {
			actor->sceneCnt += GabombaStatue_RopeStopSteps[i] * direction;
			syWait(1);
		}
	}
}

/* The cog of sprite id within 24 pixels of origin, or null. */
ACTOR *scGabombaStatueFindCogUnder(ACTOR *origin, s32 id) {
	s32 i;

	for(i = ACTOR_FIRST_PLACED; i < FIELD_TREASURE_ACTOR; i++) {
		ACTOR *cog = fdActorGet(i);

		if(cog != NULL && cog->sprStorageKind == ACTOR_SPRITE_ONE &&
		   ((SPR *)cog->spr)->entries[0] != NULL &&
		   ((SPR *)cog->spr)->entries[0]->anmId == id) {
			if(DIFF_WITHIN(cog->x.fixed, origin->x.fixed, toFIXED(24) - 1) &&
			   DIFF_WITHIN(cog->z.fixed, origin->z.fixed, toFIXED(24) - 1))
				return cog;
		}
	}
	return NULL;
}

/* As scGabombaStatueFindCogUnder, but around the point 16 pixels ahead of */
/* origin in its facing. */
ACTOR *scGabombaStatueFindCogAhead(ACTOR *origin, s32 id) {
	FIXED position[3];
	s32 i;

	position[0] = origin->x.fixed;
	position[2] = origin->z.fixed;
	syFieldVectorAddPolarOffset(16, origin->facing, position);
	for(i = ACTOR_FIRST_PLACED; i < FIELD_TREASURE_ACTOR; i++) {
		ACTOR *cog = fdActorGet(i);

		if(cog != NULL && cog->sprStorageKind == ACTOR_SPRITE_ONE &&
		   ((SPR *)cog->spr)->entries[0] != NULL &&
		   ((SPR *)cog->spr)->entries[0]->anmId == id) {
			if(DIFF_WITHIN(cog->x.fixed, position[0], toFIXED(24) - 1) &&
			   DIFF_WITHIN(cog->z.fixed, position[2], toFIXED(24) - 1))
				return cog;
		}
	}
	return NULL;
}

/* The leader hops off a cog by dx, dz. */
void scGabombaStatueHopOffCog(ACTOR *actor, s32 dx, s32 dz, s32 velocity) {
	u8 *flags = &actor->motionFlg;
	u8 saved;

	syImportLeaderStopMoving(actor);
	saved = *flags;
	fdActorSelectAnimation(actor, ANIM_LEADER_SPRING);
	syWait(6);
	fdEventPlaySnd(SE_JUMP);
	fdActorSelectAnimation(actor, ANIM_LEADER_LEAP);
	actor->speedLimit = toFIXED(3);
	actor->acceleration = toFIXED(2);
	actor->velY = velocity;
	*flags &= ACTOR_JUMP_KEEPS_MOTION;
	fdActorSetSprFlags(actor, 0);
	scFieldActorSetMoveTargetFX(actor, actor->x.fixed + dx, actor->y.fixed,
								actor->z.fixed + dz);
	scFieldActorWaitForMove(actor);
	*flags = saved;
	fdActorSelectAnimation(actor, ANIM_LEADER_SPRING);
	fdActorSetSprFlags(actor, SPRITE_FLAG_SHADOW);
	syWait(3);
}

/* Standing on a green cog turns the leader with it. */
void scGabombaStatueTurnWithCog(void) {
	s32 step = GabombaStatue_CogDirection;

	if(step != 0) {
		s32 turn = step * DEGtoANG(6);
		s32 delta = -turn;
		ACTOR *leader = fdActorGet(syFieldPartyGetLeaderActor());
		ACTOR *near = scGabombaStatueFindCogUnder(leader, SPRITE_GREEN_COG);

		if(near != NULL) {
			u16 angle = near->facing;

			if(angle == FACING_WEST)
				delta = turn;
		}
		leader->facing += delta;
	}
}

/* Standing on a blue cog carries the leader round with it, unless a wall */
/* stops him. */
void scGabombaStatueRideBlueCog(void) {
	ACTOR *actor, *cog;
	VECTOR3 test;
	s32 step = -DEGtoANG(2);
	s32 turn;
	FIXED x, z, dx, dz;
	s32 back;
	FIXED tx, tz;

	if(GabombaStatue_CogDirection == 0)
		return;
	actor = fdActorGetById(fdPartyGetLeaderActor());
	cog = scGabombaStatueFindCogUnder(actor, SPRITE_BLUE_COG);
	turn = GabombaStatue_CogDirection * step;
	if(cog != 0) {
		if(cog->y.fixed + toFIXED(16) > actor->y.fixed)
			return;
		if(cog->facing == FACING_WEST)
			turn = -turn;
		dx = actor->x.fixed - cog->x.fixed;
		dz = actor->z.fixed - cog->z.fixed;
		back = -turn;
		{
			s32 s = syMathSin(back);
			s32 c = syCos(back);
			x = cog->x.fixed + syMulFX(c, dx) + syMulFX(s, dz);
			z = cog->z.fixed - syMulFX(s, dx) + syMulFX(c, dz);
		}
		test.x = x + actor->velX;
		test.y = cog->y.fixed;
		test.z = z + actor->velZ;
		if(fdMapGridCheckCollision(actor, &test) == 0) {
			s32 s, c, angle;
			angle = DEGtoANG(30) - turn;
			s = syMathSin(angle);
			c = syCos(angle);
			tx = cog->x.fixed + syMulFX(c, dx) + syMulFX(s, dz);
			tz = cog->z.fixed - syMulFX(s, dx) + syMulFX(c, dz);
			test.x = tx + actor->velX;
			test.z = tz + actor->velZ;
			if(fdMapGridCheckCollision(actor, &test) == 0) {
				FIXED s, c;
				s32 angle;
				angle = back - DEGtoANG(30);
				s = syMathSin(angle);
				c = syCos(angle);
				tx = cog->x.fixed + syMulFX(c, dx) + syMulFX(s, dz);
				tz = cog->z.fixed - syMulFX(s, dx) + syMulFX(c, dz);
				test.x = tx + actor->velX;
				test.z = tz + actor->velZ;
				if(fdMapGridCheckCollision(actor, &test) == 0) {
					actor->x.fixed = x;
					actor->z.fixed = z;
				}
			}
		}
	}
	actor->facing += turn;
}

/* Standing on a red cog carries the leader east or west as it slides, kept on
 */
/* its middle; 20 pixels from its centre he hops off the end. */
void scGabombaStatueRideSlidingCog(void) {
	s32 *speed = &GabombaStatue_CogDirection;

	if(*speed != 0) {
		ACTOR *actor = fdActorGet(syFieldPartyGetLeaderActor());
		ACTOR *cog = scGabombaStatueFindCogUnder(actor, SPRITE_RED_COG);
		s32 dx = *speed * toFIXED(0.5);

		if(cog != NULL) {
			FIXED diff, x, cogX;

			if(cog->facing == FACING_WEST)
				dx = -dx;
			if(cog->z.fixed - actor->z.fixed < -toFIXED(2))
				actor->z.fixed = cog->z.fixed + toFIXED(2);
			if(cog->z.fixed - actor->z.fixed > toFIXED(2))
				actor->z.fixed = cog->z.fixed - toFIXED(2);
			x = actor->x.fixed + dx;
			actor->x.fixed = x;
			cogX = cog->x.fixed;
			diff = cogX - x;
			if(diff >= 0 ? diff >= toFIXED(20) : x - cogX >= toFIXED(20)) {
				s32 sign;
				FIXED offset;

				sign = 0;
				offset = actor->x.fixed - cogX;
				if(offset != 0) {
					sign = 1;
					if(offset <= 0)
						sign = -sign;
				}
				scGabombaStatueHopOffCog(actor, sign * toFIXED(12), 0,
										 toFIXED(2));
			}
		}
	}
}

/* The ride on a pink drum of the mouse's room ends at its south end: the */
/* leader drops off it to the room below. */
void scGabombaStatueEndRide(ACTOR *actor) {
	u32 i;
	MAIN_ROUTINE_STATE *routine;

	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	fdActorSetSprFlags(actor, 0);
	actor->motionFlg = 0;
	actor->y.fixed = toFIXED(6);
	actor->z.fixed = toFIXED(247.25707);
	actor->facing = FACING_SOUTH;
	routine = &MainRoutineStateWork;
	fdActorSetAttachedEffect(routine->leaderActorId,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAnimation(routine->leaderActorId, ANIM_LEADER_WALK_BACKWARDS);
	syFieldEventWait(30);
	fdActorSetAnimation(routine->leaderActorId, ANIM_LEADER_ARMS_RAISED);
	syFieldEventWait(10);
	for(i = 0; i < 20; i++) {
		actor->scaleX -= FIXED_ONE * 3 / 256;
		actor->scaleY -= FIXED_ONE * 3 / 256;
		actor->y.fixed -= toFIXED(3) / 2;
		syWait(1);
		if(i == 10)
			fdEventPlaySnd(SE_DUST_PUFF);
		/* DEAD: the loop ends before frame 20, so the screen is never taken out
		 */
		/* here; the ROM keeps the test and the transition. */
		if(i == 20) {
			FieldStateWork->transition =
				SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
			fdEventFinishScrTransition();
		}
	}
}

/* Standing on a roller carries the leader north or south as it rolls, a blue */
/* pillar one way and a pink drum the other, kept on its middle; 20 pixels */
/* from its centre he hops off the end. A drum of the mouse's room carries */
/* him off its south end and down to Steel's room. */
void scGabombaStatueRideRoller(void) {
	ACTOR *actor, *roller;
	s32 speed;
	FIXED d;
	s32 sign, x;
	FIXED z, rollerZ, offset;

	if(GabombaStatue_CogDirection == 0)
		return;
	actor = fdActorGet(syFieldPartyGetLeaderActor());
	speed = GabombaStatue_CogDirection * toFIXED(0.5);
	roller = scGabombaStatueFindCogUnder(actor, SPRITE_BLUE_PILLAR);
	if(roller != 0) {
		if(roller->x.fixed - actor->x.fixed < -toFIXED(2))
			actor->x.fixed = roller->x.fixed + toFIXED(2);
		if(roller->x.fixed - actor->x.fixed > toFIXED(2))
			actor->x.fixed = roller->x.fixed - toFIXED(2);
		z = actor->z.fixed + speed;
		actor->z.fixed = z;
		rollerZ = roller->z.fixed;
		d = rollerZ - z;
		if(d >= 0 ? d >= toFIXED(20) : z - rollerZ >= toFIXED(20)) {
			sign = 0;
			offset = actor->z.fixed - rollerZ;
			if(offset != 0) {
				sign = 1;
				if(offset <= 0)
					sign = -sign;
			}
			scGabombaStatueHopOffCog(actor, 0, sign * toFIXED(12), toFIXED(2));
		}
	}
	roller = scGabombaStatueFindCogUnder(actor, SPRITE_PINK_DRUM);
	if(roller == 0)
		return;
	if(roller->x.fixed - actor->x.fixed < -toFIXED(2))
		actor->x.fixed = roller->x.fixed + toFIXED(2);
	if(roller->x.fixed - actor->x.fixed > toFIXED(2))
		actor->x.fixed = roller->x.fixed - toFIXED(2);
	z = actor->z.fixed - speed;
	actor->z.fixed = z;
	rollerZ = roller->z.fixed;
	d = rollerZ - z;
	if(d >= 0 ? d >= toFIXED(20) : z - rollerZ >= toFIXED(20)) {
		sign = 0;
		offset = actor->z.fixed - rollerZ;
		if(offset != 0) {
			sign = 1;
			if(offset <= 0)
				sign = -sign;
		}
		scGabombaStatueHopOffCog(actor, 0, sign * toFIXED(12), toFIXED(2));
	}
	if(MainRoutineStateWork.next != SCENE_GABOMBA_STATUE_MOUSE)
		return;
	x = FIELD_CELL(actor->x.fixed);
	if(FIELD_CELL(actor->z.fixed) != 15)
		return;
	if(x == 28) {
		scGabombaStatueEndRide(actor);
		syFieldEventSetExitRequest(
			GABOMBA_STATUE_MOUSE_TO_GABOMBA_STATUE_STEEL_SOUTHEAST_A);
	}
	else if(x == 30) {
		scGabombaStatueEndRide(actor);
		syFieldEventSetExitRequest(
			GABOMBA_STATUE_MOUSE_TO_GABOMBA_STATUE_STEEL_SOUTHEAST_B);
	}
}

/* Walking east or west off a sliding cog hops the leader to the next. */
void scGabombaStatueHopAcrossEastOrWest(void) {
	s32 *speed = &GabombaStatue_CogDirection;

	if(*speed != 0) {
		ACTOR *actor = fdActorGet(syFieldPartyGetLeaderActor());
		ACTOR *found = scGabombaStatueFindCogAhead(actor, SPRITE_RED_COG);
		s32 dx = *speed * toFIXED(16);

		if(found != NULL) {
			u16 facing = found->facing;

			if(facing == FACING_WEST)
				dx = -dx;
			facing = actor->facing;
			if((facing == FACING_EAST && dx > 0) ||
			   (facing == FACING_WEST && dx < 0))
				scGabombaStatueHopOffCog(actor, dx, 0, toFIXED(4));
		}
	}
}

/* Walking north or south off a roller hops the leader to the next. */
void scGabombaStatueHopAcrossNorthOrSouth(void) {
	s32 *speed = &GabombaStatue_CogDirection;

	if(*speed != 0) {
		ACTOR *actor = fdActorGet(syFieldPartyGetLeaderActor());
		ACTOR *found = scGabombaStatueFindCogAhead(actor, SPRITE_BLUE_PILLAR);
		s32 dz = *speed * toFIXED(16);
		u16 facing;

		if(found == NULL) {
			found = scGabombaStatueFindCogAhead(actor, SPRITE_PINK_DRUM);
			dz = -dz;
			if(found == NULL)
				return;
		}
		facing = actor->facing;
		if((facing == FACING_SOUTH && dz > 0) ||
		   (facing == FACING_NORTH && dz < 0))
			scGabombaStatueHopOffCog(actor, 0, dz, toFIXED(4));
	}
}

/* When Felix walks to the far west of the mouse's room, cell (14, 10), a */
/* little mouse pops up, squeaks, and hops from cog to cog, showing the way */
/* across. It does so once. */
void scGabombaStatueMouseShowsTheWay(ACTOR *actor) {
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	fdCameraSetSpeed(1);
	fdActorSetPos(ACTOR_MOUSE, 328, 296);
	syFieldEventWait(30);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_SOUTHEAST,
						 0);
	syFieldEventWait(15);
	scFieldActorWatchAndSetScript(MainRoutineStateWork.leaderActorId,
								  ACTOR_MOUSE);
	fdActorShowEmote(MainRoutineStateWork.leaderActorId,
					 EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	syEventCameraFollowActor(ACTOR_MOUSE, 1);
	syFieldEventWait(30);
	fdActorGet(ACTOR_MOUSE)->priFlg |= ACTOR_PRIORITY_IGNORES_RISES;
	fdActorSetAnimation(ACTOR_MOUSE, 1);
	fdActorWalkToAndWait(ACTOR_MOUSE, 320, 240);
	fdActorWalkToAndWait(ACTOR_MOUSE, 304, 216);
	fdActorWalkToAndWait(ACTOR_MOUSE, 280, 216);
	fdActorWalkToAndWait(ACTOR_MOUSE, 280, 184);
	syFieldEventWait(20);
	fdActorFaceActor(ACTOR_MOUSE, MainRoutineStateWork.leaderActorId, 0);
	syFieldEventWait(10);
	fdEventPlaySnd(SE_VOICE_GIRL_C);
	{
		s32 emote = EMOTE_IN_FRONT | EMOTE_EXCLAMATION, frames = 30;

		fdActorShowEmote(ACTOR_MOUSE, emote, frames);
	}
	fdActorWalkToAndWait(ACTOR_MOUSE, 344, 184);
	fdActorWalkToAndWait(ACTOR_MOUSE, 344, 200);
	syFieldEventWait(2);
	fdActorFaceDirection(ACTOR_MOUSE, FACING_WEST, 0);
	syFieldEventWait(15);
	fdEventPlaySnd(SE_JUMP);
	fdActorGet(ACTOR_MOUSE)->velY = toFIXED(4);
	fdActorSetSpeed(ACTOR_MOUSE, 4);
	fdActorWalkToAndWait(ACTOR_MOUSE, 312, 200);
	fdEventPlaySnd(SE_JUMP);
	fdActorGet(ACTOR_MOUSE)->velY = toFIXED(6);
	fdActorSetSpeed(ACTOR_MOUSE, 4);
	fdActorWalkToAndWait(ACTOR_MOUSE, 312, 168);
	fdActorSetSpeed(ACTOR_MOUSE, 2);
	fdActorWalkToAndWait(ACTOR_MOUSE, 328, 152);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	syFieldEventWait(15);
	fdEventPlaySnd(SE_JUMP);
	fdActorGet(ACTOR_MOUSE)->velY = toFIXED(6);
	fdActorSetSpeed(ACTOR_MOUSE, 4);
	fdActorWalkToAndWait(ACTOR_MOUSE, 360, 152);
	syFieldEventWait(15);
	fdEventPlaySnd(SE_JUMP);
	fdActorGet(ACTOR_MOUSE)->velY = toFIXED(6);
	fdActorSetSpeed(ACTOR_MOUSE, 2);
	fdActorWalkToAndWait(ACTOR_MOUSE, 360, 184);
	fdActorSetSpeed(ACTOR_MOUSE, 2);
	fdActorWalkToAndWait(ACTOR_MOUSE, 376, 184);
	fdActorWalkToAndWait(ACTOR_MOUSE, 392, 200);
	fdActorWalkToAndWait(ACTOR_MOUSE, 408, 184);
	syFieldEventWait(15);
	fdEventPlaySnd(SE_JUMP);
	fdActorGet(ACTOR_MOUSE)->velY = toFIXED(6);
	fdActorSetSpeed(ACTOR_MOUSE, 4);
	fdActorWalkToAndWait(ACTOR_MOUSE, 440, 184);
	fdActorSetPos(ACTOR_MOUSE, 0, 0);
	fdEventSetFlag(FLAG_GABOMBA_MOUSE_SEEN);
	fdEventEnd();
}

void scGabombaStatueSendCharge(s32 id, s32 colorMode, s32 statueId, s32 angle);

/* How the charges reached the statue in the middle */
/* (scGabombaStatueRunCharges), which decides how the ceremony outside goes on.
 */
typedef enum CHARGE_ARRIVAL {
	CHARGE_ARRIVAL_TOGETHER =
		0, /* a blue and a red charge at the same moment */
	CHARGE_ARRIVAL_BLUE_FIRST = 1, /* a blue one alone, or before the red */
	CHARGE_ARRIVAL_RED_FIRST = 2,  /* a red one alone, or before the blue */
	CHARGE_ARRIVAL_NONE = 3
} CHARGE_ARRIVAL;

/* Takes charge id off the map, stopped. */
void scGabombaStatueResetCharge(s32 id) {
	fdActorSetPos(id, 0, 0);
	scFieldActorStop(id);
	fdActorSetAnimation(id, 1);
}

/* Sends the four charges circling out of the orb statues, the blue statue's */
/* green-tinted and the red's red-tinted. */
void scGabombaStatueReleaseCharges(void) {
	scGabombaStatueResetCharge(ACTOR_CHARGE_A);
	scGabombaStatueResetCharge(ACTOR_CHARGE_B);
	scGabombaStatueResetCharge(ACTOR_CHARGE_C);
	scGabombaStatueResetCharge(ACTOR_CHARGE_D);
	scGabombaStatueSendCharge(ACTOR_CHARGE_A, SPRITE_COLOR_GREEN_TINT,
							  ACTOR_BLUE_ORB_STATUE, 0);
	scGabombaStatueSendCharge(ACTOR_CHARGE_B, SPRITE_COLOR_GREEN_TINT,
							  ACTOR_BLUE_ORB_STATUE, DEGtoANG(180));
	scGabombaStatueSendCharge(ACTOR_CHARGE_C, SPRITE_COLOR_RED_TINT,
							  ACTOR_RED_ORB_STATUE, DEGtoANG(90));
	scGabombaStatueSendCharge(ACTOR_CHARGE_D, SPRITE_COLOR_RED_TINT,
							  ACTOR_RED_ORB_STATUE, TRIG_TURN * 3 / 4);
}

/* The offering's clock has run out: Akafubu offers the jewel below ("O Great */
/* Gabomba! Please accept this, our humble gift!"), the orb statues send their
 */
/* charges along the channel, and how they reach the statue in the middle */
/* decides how the ceremony outside goes. */
void scGabombaStatueHearTheOffering(void) {
	s32 result;

	fdEventSetFlag(FLAG_OFFERING_HEARD);
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	fdEventPlaySnd(SOUND_FADE_OUT_BGM);
	scGabombaStatueStopClock();
	syFieldEventWait(40);
	syFieldEventSetMessageId(MsgGabombaAcceptOffering);
	/* "O Great Gabomba! Please accept this, our humble gift!" */
	fdEventShowMessage(ACTOR_AKAFUBU, 0);
	fdCameraMoveTo(392, CAMERA_KEEP, 256, 1);
	fdCameraWaitForArrival();
	fdEventPlaySnd(SE_SHIMMER);
	syWait(1);
	fdPaletteFadeApplySource(PALETTE_TINT_NONE, 0);
	fdPaletteFadeApplyTarget(RGB_WHITE, 0);
	fdPaletteFadeInterpolate(40);
	syWait(80);
	fdActorSetSprFlags(fdActorGet(MainRoutineStateWork.leaderActorId), 0);
	scFieldActorSetSprCol(fdActorGet(MainRoutineStateWork.leaderActorId),
						  SPRITE_COLOR_HIDDEN);
	scGabombaStatueReleaseCharges();
	syWait(1);
	fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, 0);
	fdPaletteFadeInterpolate(10);
	syWait(20);
	fdEventPlaySnd(SE_PSYNERGY_START);
	scFieldActorSetSprCol(fdActorGet(ACTOR_BLUE_ORB_STATUE),
						  SPRITE_COLOR_WHITE);
	scFieldActorSetSprCol(fdActorGet(ACTOR_RED_ORB_STATUE), SPRITE_COLOR_WHITE);
	syWait(8);
	scFieldActorSetSprCol(fdActorGet(ACTOR_RED_ORB_STATUE), SPRITE_COLOR_BGR);
	scFieldActorSetSprCol(fdActorGet(ACTOR_BLUE_ORB_STATUE),
						  SPRITE_COLOR_NORMAL);
	syWait(8);
	fdEventPlaySnd(MUS_RITUALS);
	result = scGabombaStatueRunCharges(ACTOR_CHARGE_A, 1);
	fdActorTableSnapshot();
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	switch(result) {
	case CHARGE_ARRIVAL_NONE:
		syFieldEventSetExitRequest(
			GABOMBA_STATUE_ARROWS_TO_KIBOMBO_STATUE_GROUNDS_CEREMONY_NORTH_C);
		break;
	case CHARGE_ARRIVAL_BLUE_FIRST:
		syFieldEventSetExitRequest(
			GABOMBA_STATUE_ARROWS_TO_KIBOMBO_STATUE_GROUNDS_CEREMONY_NORTH_D);
		break;
	case CHARGE_ARRIVAL_RED_FIRST:
		syFieldEventSetExitRequest(
			GABOMBA_STATUE_ARROWS_TO_KIBOMBO_STATUE_GROUNDS_CEREMONY_NORTH_E);
		break;
	case CHARGE_ARRIVAL_TOGETHER:
		syFieldEventSetExitRequest(
			GABOMBA_STATUE_ARROWS_TO_GABOMBA_STATUE_FACE_EAST_A_B);
		break;
	}
}

extern TRIGGER GabombaStatue_Triggers[];
extern TRIGGER GabombaStatue_BoneArmletChestTriggers[];
extern TRIGGER GabombaStatue_ElixirChestTriggers[];
extern TRIGGER GabombaStatue_SteelTriggers[];
extern TRIGGER GabombaStatue_MouseTriggers[];
extern TRIGGER GabombaStatue_CogsTriggers[];
extern TRIGGER GabombaStatue_ArrowsTriggers[];
extern TRIGGER GabombaStatue_PinionTriggers[];

const u8 *scGetTriggers(void) {
	s16 scene = MainRoutineStateWork.next;
	const u8 *table = GabombaStatue_Triggers;

	if(scene == SCENE_GABOMBA_STATUE_BONE_ARMLET_CHEST)
		table = GabombaStatue_BoneArmletChestTriggers;
	else if(scene == SCENE_GABOMBA_STATUE_ELIXIR_CHEST)
		table = GabombaStatue_ElixirChestTriggers;
	else if(scene == SCENE_GABOMBA_STATUE_STEEL)
		table = GabombaStatue_SteelTriggers;
	else if(scene == SCENE_GABOMBA_STATUE_MOUSE)
		table = GabombaStatue_MouseTriggers;
	else if(scene == SCENE_GABOMBA_STATUE_COGS)
		table = GabombaStatue_CogsTriggers;
	else if(scene == SCENE_GABOMBA_STATUE_ARROWS)
		table = GabombaStatue_ArrowsTriggers;
	else if(scene == SCENE_GABOMBA_STATUE_PINION)
		table = GabombaStatue_PinionTriggers;
	return table;
}

/* Felix walks to the top of the ladder at x, z, turns his back and climbs */
/* down it, and the screen closes on him as he leaves for the room below by */
/* the ladder's exit. */
void scGabombaStatueClimbDownLadder(s32 x, s32 z) {
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	ACTOR *actor = fdActorGet(MainRoutineStateWork.leaderActorId);

	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	actor->motionFlg = 0;
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 1);
	fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, x, z);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_NORTH, 0);
	scFieldActorWaitForScriptEndById(MainRoutineStateWork.leaderActorId);
	fdActorSetSprFlags(fdActorGet(MainRoutineStateWork.leaderActorId), 0);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId,
						ANIM_LEADER_CLIMB_DOWN_FROM_TOP);
	scFieldActorSetMoveTarget(actor, x, -12, z);
	scFieldActorWaitForMove(actor);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_LEADER_CLIMB);
	fdEventPlaySnd(SE_MAP_EXIT);
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	syFieldEventSetExitRequest(state->stepTrigger);
}

/* A ladder down that Felix takes only when he walks into it: from the squares
 */
/* beside it on row, pushing east or west, or from the one south of it, pushing
 */
/* north. Brushing past it does nothing. */
#define DEFINE_GABOMBA_STATUE_STEP_WHEN_HELD(                                  \
	name, row, westColumn, eastColumn, southColumn, southRow, holeX, holeZ)    \
	void name(void) {                                                          \
		ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId);        \
		s32 x = FIELD_CELL(leader->x.fixed);                                   \
		s32 z = FIELD_CELL(leader->z.fixed);                                   \
                                                                               \
		if(z == row) {                                                         \
			if(x == westColumn && (InputWork.held & KEY_RIGHT) == 0)           \
				return;                                                        \
			if(x == eastColumn && (InputWork.held & KEY_LEFT) == 0)            \
				return;                                                        \
		}                                                                      \
		else if(x == southColumn && z == southRow &&                           \
				(InputWork.held & KEY_UP) == 0) {                              \
			return;                                                            \
		}                                                                      \
		scGabombaStatueClimbDownLadder(holeX, holeZ);                          \
	}

/* Felix climbs down the ladder in the north of the room of corridors, where */
/* the Mimic and the Bone Armlet's chest are, at cell (19, 11), back to the */
/* hall of pinions below, when he walks into it from the west, the east or the
 */
/* south. */
DEFINE_GABOMBA_STATUE_STEP_WHEN_HELD(
	scGabombaStatueClimbDownBoneArmletChestLadder, 11, 18, 20, 19, 12, 312, 180)

/* The same for a ladder down with a square to walk in from on each of its */
/* four sides. */
#define DEFINE_GABOMBA_STATUE_STEP_WHEN_HELD_FOUR(name, column, northRow,      \
												  southRow, row, westColumn,   \
												  eastColumn, holeX, holeZ)    \
	void name(void) {                                                          \
		ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId);        \
		s32 x = FIELD_CELL(leader->x.fixed);                                   \
		s32 z = FIELD_CELL(leader->z.fixed);                                   \
                                                                               \
		if(x == column) {                                                      \
			if(z == northRow && (InputWork.held & KEY_DOWN) == 0)              \
				return;                                                        \
			if(z == southRow && (InputWork.held & KEY_UP) == 0)                \
				return;                                                        \
		}                                                                      \
		else if(z == row) {                                                    \
			if(x == westColumn && (InputWork.held & KEY_RIGHT) == 0)           \
				return;                                                        \
			if(x == eastColumn && (InputWork.held & KEY_LEFT) == 0)            \
				return;                                                        \
		}                                                                      \
		scGabombaStatueClimbDownLadder(holeX, holeZ);                          \
	}

/* Felix climbs down the ladder in the north of the room of the rope, the */
/* stakes and the flashing cog, at cell (29, 12), back to the room of */
/* corridors below, when he walks into it from any side. */
DEFINE_GABOMBA_STATUE_STEP_WHEN_HELD_FOUR(
	scGabombaStatueClimbDownElixirChestLadder, 29, 11, 13, 12, 28, 30, 472, 196)

/* Felix climbs down the ladder in the north-east of Steel's room, at cell */
/* (33, 7), to the north-east of the room of the rope and the flashing cog */
/* below, when he walks into it from the west or the south. */
void scGabombaStatueClimbDownSteelNortheastLadder(void) {
	ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	s32 x = FIELD_CELL(leader->x.fixed);
	s32 z = FIELD_CELL(leader->z.fixed);

	if(z == 7) {
		if(x == 32 && (InputWork.held & KEY_RIGHT) == 0)
			return;
	}
	else if(x == 33 && z == 8 && (InputWork.held & KEY_UP) == 0) {
		return;
	}
	scGabombaStatueClimbDownLadder(536, 112);
}

/* Felix climbs down the ladder in the west of Steel's room, at cell (6, 13), */
/* to the west of the room of the rope and the flashing cog below, when he */
/* walks into it from any side. */
void scGabombaStatueClimbDownSteelWestLadder(void) {
	ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	s32 x = FIELD_CELL(leader->x.fixed);
	s32 z = FIELD_CELL(leader->z.fixed);

	if(z == 13) {
		if(x == 5 && (InputWork.held & KEY_RIGHT) == 0)
			return;
		if(x == 7 && (InputWork.held & KEY_LEFT) == 0)
			return;
	}
	else if(x == 6) {
		if(z == 12 && (InputWork.held & KEY_DOWN) == 0)
			return;
		if(z == 14 && (InputWork.held & KEY_UP) == 0)
			return;
	}
	scGabombaStatueClimbDownLadder(104, 208);
}

/* Felix climbs down the ladder in the east of Steel's room, at cell (42, 13),
 */
/* to the east of the room of the rope and the flashing cog below, when he */
/* walks into it from the north, the west or the south. */
void scGabombaStatueClimbDownSteelEastLadder(void) {
	ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	s32 x = FIELD_CELL(leader->x.fixed);
	s32 z = FIELD_CELL(leader->z.fixed);

	if(x == 42) {
		if(z == 12 && (InputWork.held & KEY_DOWN) == 0)
			return;
		if(z == 14 && (InputWork.held & KEY_UP) == 0)
			return;
#ifdef BUGFIX
	}
	else if(x == 41 && z == 13 && (InputWork.held & KEY_RIGHT) == 0) {
#else
		/* BUG: the cell tested is east of the hole, where the map has no hole
		 */
		/* cell; from the one west of it, (41, 13), the leader drops in without
		 */
		/* pushing east. */
	}
	else if(x == 43 && z == 13 && (InputWork.held & KEY_RIGHT) == 0) {
#endif
		return;
	}
	scGabombaStatueClimbDownLadder(680, 208);
}

/* Felix climbs down the ladder in the north of the mouse's room, at cell */
/* (24, 7), to Steel's room below, when he walks into it from the west, the */
/* east or the south. */
DEFINE_GABOMBA_STATUE_STEP_WHEN_HELD(scGabombaStatueClimbDownMouseNorthLadder,
									 7, 23, 25, 24, 8, 392, 112)

/* Felix climbs down the ladder in the west of the mouse's room, at cell */
/* (6, 9), to the west of Steel's room below, when he walks into it from the */
/* west, the east or the south. */
DEFINE_GABOMBA_STATUE_STEP_WHEN_HELD(scGabombaStatueClimbDownMouseWestLadder, 9,
									 5, 7, 6, 10, 104, 146)

/* Felix climbs down the ladder in the east of the mouse's room, beside its */
/* east blue cog at cell (42, 9), to the east of Steel's room below, when he */
/* walks into it from the east or the south. */
void scGabombaStatueClimbDownMouseEastLadder(void) {
	ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	s32 x = FIELD_CELL(leader->x.fixed);
	s32 z = FIELD_CELL(leader->z.fixed);

	if(x == 42 && z == 10 && (InputWork.held & KEY_UP) == 0)
		return;
	if(x == 43 && z == 9 && (InputWork.held & KEY_LEFT) == 0)
		return;
	scGabombaStatueClimbDownLadder(680, 146);
}

/* Felix climbs down the ladder in the middle of the room below the small */
/* idol's chamber, at cell (27, 18), back to the mouse's room, when he walks */
/* into it from any side. */
DEFINE_GABOMBA_STATUE_STEP_WHEN_HELD_FOUR(scGabombaStatueClimbDownCogsLadder,
										  27, 17, 19, 18, 26, 28, 440, 288)

/* Felix climbs down the ladder in the north-east of the room of the arrow */
/* panels, at cell (29, 9), back to the small idol's chamber below, when he */
/* walks into it from the west or the south. */
void scGabombaStatueClimbDownArrowsLadder(void) {
	ACTOR *actor = fdActorGet(MainRoutineStateWork.leaderActorId);
	s32 x = FIELD_CELL(actor->x.fixed);
	s32 z = FIELD_CELL(actor->z.fixed);
	ACTOR *leader;
	FIELD_STATE *state;

	if(x == 28 && z == 9 && !(InputWork.held & KEY_RIGHT))
		return;
	if(x == 29 && z == 10 && !(InputWork.held & KEY_UP))
		return;
	state = Ram_HeapSlots->fieldState;
	leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	leader->motionFlg = 0;
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 1);
	fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 472, 144);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_NORTH, 0);
	scFieldActorWaitForScriptEndById(MainRoutineStateWork.leaderActorId);
	fdActorSetSprFlags(fdActorGet(MainRoutineStateWork.leaderActorId), 0);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId,
						ANIM_LEADER_CLIMB_DOWN_FROM_TOP);
	scFieldActorSetMoveTarget(leader, 472, 0, 156);
	scFieldActorWaitForMove(leader);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_LEADER_CLIMB);
	fdEventPlaySnd(SE_MAP_EXIT);
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	syFieldEventSetExitRequest(state->stepTrigger);
}

/* Felix walks into the gap between two turning pinions, which grind and pull */
/* him through to (x, z) on the other side. */
#define DEFINE_GABOMBA_STATUE_WALK_LEADER_TO(name, x, z)                       \
	void name(ACTOR *actor) {                                                  \
		fdEventStart();                                                        \
		syFieldEventEndLastingPsynergy(0);                                     \
		fdEventPlaySnd(SE_GABOMBA_STATUE_HOLE);                                \
		fdActorSetSpeedAndAcceleration(MainRoutineStateWork.leaderActorId,     \
									   1.6, 1.6);                              \
		fdActorMoveToAndWait(MainRoutineStateWork.leaderActorId, x, z);        \
		fdEventEnd();                                                          \
	}

/* Walking into the gap between the pinions in the south-west of the hall of */
/* pinions, cell (9, 28), Felix is pulled through them to the north. */
DEFINE_GABOMBA_STATUE_WALK_LEADER_TO(GabombaStatue_PinionsCarryLeaderNorth, 152,
									 424)

/* Once the cogs turn the other way, the pinions in the south-west of the hall
 */
/* of pinions throw Felix back out to the south when he walks into their gap, */
/* and he lands with a rattle, leaning back. */
void scGabombaStatuePinionsThrowLeaderSouth(void) {
	ACTOR *actor = fdActorGet(MainRoutineStateWork.leaderActorId);

	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	fdEventPlaySnd(SE_GABOMBA_STATUE_HOLE);
	actor->motionFlg = 0;
	fdActorSetSpeedAndAcceleration(MainRoutineStateWork.leaderActorId, 1.6,
								   1.6);
	fdActorMoveToAndWait(MainRoutineStateWork.leaderActorId, 152, 488);
	actor->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	syFieldEventWait(8);
	fdEventPlaySnd(SE_RATTLE);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId,
						ANIM_LEADER_LEAN_BACK);
	syFieldEventWait(20);
	fdEventEnd();
}

/* Walking into the gap between the pinions in the north of the room of the */
/* flashing cog, cell (23, 9), Felix is pulled through them to the south. */
DEFINE_GABOMBA_STATUE_WALK_LEADER_TO(
	GabombaStatue_ElixirPinionsCarryLeaderSouth, 376, 200)

/* Once the cogs turn the other way, the pinions in the south of the room of */
/* the flashing cog, cell (24, 22), pull Felix through them to the north. */
DEFINE_GABOMBA_STATUE_WALK_LEADER_TO(
	GabombaStatue_ElixirPinionsCarryLeaderNorth, 376, 136)

/* Walking into the gap between the pinions in the room below the small */
/* idol's chamber, cell (27, 12), Felix is pulled through them to the south. */
DEFINE_GABOMBA_STATUE_WALK_LEADER_TO(GabombaStatue_CogsPinionsCarryLeaderSouth,
									 440, 248)

/* Once the cogs turn the other way, the pinions east of the first in the room
 */
/* below the small idol's chamber, cell (35, 13), pull Felix through them to */
/* the north. */
DEFINE_GABOMBA_STATUE_WALK_LEADER_TO(GabombaStatue_CogsPinionsCarryLeaderNorth,
									 440, 184)

/* Draws the room's ladder-top blocks, or hides them. */
void scGabombaStatueSetLadderTopBlocksDrawn(s32 show) {
	if(MainRoutineStateWork.next == SCENE_GABOMBA_STATUE_BONE_ARMLET_CHEST)
		scActorsSetDrawn(show, ACTOR_BONE_ARMLET_LADDER_TOP_BLOCK, 1);
	else if(MainRoutineStateWork.next == SCENE_GABOMBA_STATUE_ELIXIR_CHEST)
		scActorsSetDrawn(show, ACTOR_ELIXIR_WEST_LADDER_TOP_BLOCK, 2);
}

extern s16 GabombaStatue_Stakes[];

/* Unused: draws the ladder-top blocks. */
UNUSED void scGabombaStatueDrawLadderTopBlocks(void) {
	scGabombaStatueSetLadderTopBlocksDrawn(1);
	fdEventSetFlag(FLAG_LADDER_TOP_BLOCKS_DRAWN);
}

/* Hides the ladder-top blocks. */
void scGabombaStatueHideLadderTopBlocks(void) {
	scGabombaStatueSetLadderTopBlocksDrawn(0);
	fdEventClearFlag(FLAG_LADDER_TOP_BLOCKS_DRAWN);
}

#include "shared/actors_set_hidden.c"

/* Pound on one of the two stakes either side of the flashing cog in the south
 */
/* of its room, cells (20, 30) and (24, 30), drives it into the floor. Once */
/* both are down, the cogs turn the other way. */
void scGabombaStatuePoundStake(s32 landed, s32 frame) {
	fdPoundSinkTarget(GabombaStatue_Stakes, landed, frame);
	if(fdEventGetFlag(FLAG_GABOMBA_FIRST_STAKE_POUNDED) &&
	   fdEventGetFlag(FLAG_GABOMBA_SECOND_STAKE_POUNDED))
		FieldStateWork->raisedTrigger = RAISED_COGS_TURN_BACK;
}

/* When the second stake by the flashing cog is Pounded, the view pans to the */
/* cog between the two stakes, the cogs stop with a clank, the flashing cog */
/* bobs up and settles, and every cog starts turning the other way. */
void scGabombaStatueCogsTurnBack(void) {
	ACTOR *actor = fdActorGet(ACTOR_FLASHING_COG);

	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	syEventCameraGetActor()->motionFlg = 0;
	fdCameraSetSpeed(0.6);
	fdCameraMoveTo(360, 0, 496, 1);
	fdCameraWaitForArrival();
	fdEventPlaySnd(SE_GABOMBA_STATUE_COGS);
	scGabombaStatueTurnCogs(0);
	syFieldEventWait(40);
	fdEventPlaySnd(SE_LEMURIAN_SHIP_LIFT_FAST);
	actor->motionFlg = 0;
	actor->y.fixed = toFIXED(8);
	syFieldEventWait(10);
	actor->y.fixed = 0;
	syFieldEventWait(10);
	scGabombaStatueTurnCogs(-1);
	fdEventSetFlag(FLAG_GABOMBA_COGS_TURNED_BACK);
	syFieldEventWait(20);
	fdEventEnd();
}

/* Pound on the arrows room's stake raises the channel's blocks. It passes */
/* scGabombaStatueRaiseBlocks no frame, so how far the stake sinks before the */
/* fist lands is left to whatever that argument holds. */
void scGabombaStatuePoundArrowsStake(s32 landed) {
	scGabombaStatueRaiseBlocks(landed);
	if(landed == POUND_STAGE_LANDED)
		FieldStateWork->raisedTrigger = RAISED_STAKE_STANDS_UP;
}

/* Once the blocks have risen, the Pounded stake stands up again to a rising */
/* chime. */
void scGabombaStatueStakeStandsUp(void) {
	ACTOR *actor = fdActorGet(ACTOR_ARROWS_STAKE);

	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	syFieldEventWait(20);
	fdEventPlaySnd(SE_RISING_CHIMES);
	fdActorSelectAnimation(actor, 5);
	actor->scaleY = FIXED_ONE;
	fdEventEnd();
}

/* Each frame: once the offering's clock has run out, the offering is made. */
void scGabombaStatueCheckOfferingClock(void) {
	if(scGabombaStatueGetClock() == 0)
		FieldStateWork->raisedTrigger = RAISED_OFFERING_HEARD;
}

/* Hides actor id (SPRITE_COLOR_HIDDEN) and fixes it in place, sorted behind */
/* the others. */
void scGabombaStatueHideActor(s32 id) {
	fdActorDisableById(id);
	scFieldActorSetSprCol(fdActorGet(id), SPRITE_COLOR_HIDDEN);
	scFieldActorSetSprPriority(id, 1);
	fdActorGet(id)->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
}

/* Coming back to the same spot hides the ladder-top blocks again unless they */
/* were drawn when the party left; otherwise they are drawn. */
void scGabombaStatueRestoreLadderTopBlocks(void) {
	if(fdEventGetFlag(FLAG_KEEP_PARTY_POSITION)) {
		if(!fdEventGetFlag(FLAG_LADDER_TOP_BLOCKS_DRAWN))
			scGabombaStatueHideLadderTopBlocks();
	}
	else {
		fdEventSetFlag(FLAG_LADDER_TOP_BLOCKS_DRAWN);
	}
}

/* Gives a room's ladder-top blocks SPRITE_COLOR_HIDDEN and holds them 36 */
/* pixels up. */
#define DEFINE_GABOMBA_STATUE_HIDE_LADDER_TOP_BLOCKS(name, first, count)       \
	void name(void) {                                                          \
		u32 i;                                                                 \
                                                                               \
		for(i = 0; i < count; i++) {                                           \
			s32 id = i + first;                                                \
			ACTOR *actor = fdActorGet(id);                                     \
                                                                               \
			scGabombaStatueHideActor(id);                                      \
			actor->motionFlg = 0;                                              \
			actor->y.fixed = toFIXED(36);                                      \
		}                                                                      \
		scGabombaStatueRestoreLadderTopBlocks();                               \
	}

/* The Bone Armlet chest's room, as it is set up: its one ladder-top block. */
DEFINE_GABOMBA_STATUE_HIDE_LADDER_TOP_BLOCKS(
	scGabombaStatueHideBoneArmletChestLadderTopBlock,
	ACTOR_BONE_ARMLET_LADDER_TOP_BLOCK, 1)

/* The Elixir chest's room, as it is set up: its two ladder-top blocks. */
DEFINE_GABOMBA_STATUE_HIDE_LADDER_TOP_BLOCKS(
	scGabombaStatueHideElixirChestLadderTopBlocks,
	ACTOR_ELIXIR_WEST_LADDER_TOP_BLOCK, 2)

/* Each frame: while a menu or Psynergy is up, the cogs and the rope posts */
/* stand still. */
void scGabombaStatuePauseCogsInMenus(void) {
	s32 step = ANIM_STEP_NORMAL;
	s32 open = 0;
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	s32 i;

	if(fdEventGetFlag(FLAG_MENU_OPEN) || state->psynergy != 0) {
		open = 1;
		step = 0;
	}
	i = ACTOR_FIRST_PLACED;
	do {
		{
			ACTOR *actor = fdActorGet(i);

			if(actor != NULL && actor->sprStorageKind == ACTOR_SPRITE_ONE) {
				SPR_ANIMATION *part = ((SPR *)actor->spr)->entries[0];

				if(part != NULL) {
					switch(part->anmId) {
					case SPRITE_BLUE_COG:
					case SPRITE_GREEN_COG:
					case SPRITE_RED_COG:
					case SPRITE_BLUE_PILLAR:
					case SPRITE_PINK_DRUM:
					case SPRITE_BROWN_COG:
						fdActorSetAnimationSpeed(actor, step);
						break;
					case SPRITE_TIGHTROPE_POST:
						actor->paused = open;
						break;
					}
				}
			}
		}
		i++;
	} while(i < FIELD_TREASURE_ACTOR);
}

/* The map shapes of the cogs' cells, from COG_SHAPE_FIRST: the scene writes */
/* GabombaStatue_CogShapes over the map's own entries there. */
enum {
	COG_SHAPE_GREEN_COG = 116, /* the green cog's own cell */
	COG_SHAPE_TOP,			   /* the flat top every other cog stands on */
	COG_SHAPE_NORTHWEST,	   /* a blue cog's four corners */
	COG_SHAPE_NORTHEAST,
	COG_SHAPE_SOUTHWEST,
	COG_SHAPE_SOUTHEAST,
	COG_SHAPE_WEST, /* a red cog's sides */
	COG_SHAPE_EAST,
	COG_SHAPE_NORTH, /* a roller's sides */
	COG_SHAPE_SOUTH,
	COG_SHAPE_PINK_DRUM_SOUTH_END,
	COG_SHAPE_END, /* the cells beyond a red cog's or a roller's ends */
	COG_SHAPE_FIRST = COG_SHAPE_GREEN_COG,
	COG_SHAPE_COUNT = COG_SHAPE_END + 1 - COG_SHAPE_FIRST
};

/* A cog's cell word: its collision code (the cell behavior), the attribute */
/* bits above it and the flags of its cell tiles, ORed into the map's. */
#define COG_CELL(behavior, attributes, pictureFlags)                           \
	((u32)(attributes) << 24 | (behavior) << 16 | (pictureFlags))

/* Gives a cell under a cog its shape on both layers and adds bits to both its
 */
/* shape words. */
#define MARK(at, shape, bits)                                                  \
	(cell = (at), layerA[cell] = (shape), layerB[cell] = (shape),              \
	 word = &shapesA[cell], *word |= (bits), word = &shapesB[cell],            \
	 *word |= (bits))

/* Gives the cells under every cog of the room their shapes and the cell */
/* behaviors that carry the leader with it, and fixes the cogs in place. */
void scGabombaStatueDrawCogCells(void) {
	u32 *dest = (u32 *)MapGridShapes + COG_SHAPE_FIRST;
	u32 *src = GabombaStatue_CogShapes;
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	u8 *layerA = map->layers[0].terrainIds;
	u8 *layerB = map->layers[1].terrainIds;
	u32 *shapesA = (u32 *)map->layers[0].cells;
	u32 *shapesB = (u32 *)map->layers[1].cells;
	ACTOR *actor;
	s16 *frames;
	u32 *word;
	u32 i;
	s32 x, z, cell, found;

	for(i = 0; i < COG_SHAPE_COUNT; i++) {
		*dest++ = *src++;
	}
	for(i = ACTOR_FIRST_PLACED; (s32)i < FIELD_TREASURE_ACTOR; i++) {
		actor = fdActorGetById(i);
		if(actor == 0)
			continue;
		if(actor->sprStorageKind != ACTOR_SPRITE_ONE)
			continue;
		frames = (s16 *)((SPR *)actor->spr)->entries[0];
		if(frames == 0)
			continue;
		found = 0;
		x = FIELD_CELL(actor->x.fixed);
		z = FIELD_CELL(actor->z.fixed);
		switch(*frames) {
		case SPRITE_GREEN_COG:
			cell = x + (z << 7);
			layerA[cell] = COG_SHAPE_GREEN_COG;
			layerB[cell] = COG_SHAPE_TOP;
			word = &shapesA[cell];
			*word |= COG_CELL(CELL_BEHAVIOR_GREEN_COG, MAP_CELL_JUMP, 0);
			word = &shapesB[cell];
			*word |= COG_CELL(CELL_BEHAVIOR_GREEN_COG, MAP_CELL_JUMP, 0);
			found = 1;
			break;
		case SPRITE_BLUE_COG:
			MARK(x + (z << 7) - 129, COG_SHAPE_NORTHWEST,
				 COG_CELL(CELL_BEHAVIOR_BLUE_COG, 0, 0));
			MARK(x + (z << 7) - 128, COG_SHAPE_TOP,
				 COG_CELL(CELL_BEHAVIOR_BLUE_COG, MAP_CELL_JUMP, 0));
			MARK(x + (z << 7) - 127, COG_SHAPE_NORTHEAST,
				 COG_CELL(CELL_BEHAVIOR_BLUE_COG, 0, 0));
			MARK(x + (z << 7) - 1, COG_SHAPE_TOP,
				 COG_CELL(CELL_BEHAVIOR_BLUE_COG, MAP_CELL_JUMP, 0));
			MARK(x + (z << 7), COG_SHAPE_TOP,
				 COG_CELL(CELL_BEHAVIOR_BLUE_COG, 0, 0));
			MARK(x + (z << 7) + 1, COG_SHAPE_TOP,
				 COG_CELL(CELL_BEHAVIOR_BLUE_COG, MAP_CELL_JUMP, 0));
			MARK(x + (z << 7) + 127, COG_SHAPE_SOUTHWEST,
				 COG_CELL(CELL_BEHAVIOR_BLUE_COG, 0, 0));
			MARK(x + (z << 7) + 128, COG_SHAPE_TOP,
				 COG_CELL(CELL_BEHAVIOR_BLUE_COG, MAP_CELL_JUMP, 0));
			MARK(x + (z << 7) + 129, COG_SHAPE_SOUTHEAST,
				 COG_CELL(CELL_BEHAVIOR_BLUE_COG, 0, 0));
			found = 1;
			break;
		case SPRITE_RED_COG:
			MARK(x + (z << 7) - 2, COG_SHAPE_END,
				 COG_CELL(CELL_BEHAVIOR_COG_END, 0, 0));
			MARK(x + (z << 7) - 1, COG_SHAPE_WEST,
				 COG_CELL(CELL_BEHAVIOR_RED_COG, 0, CELL_DRAWS_ACTOR(2, 0)));
			MARK(x + (z << 7), COG_SHAPE_TOP,
				 COG_CELL(CELL_BEHAVIOR_RED_COG, MAP_CELL_JUMP,
						  CELL_DRAWS_ACTOR(1, 1)));
			MARK(x + (z << 7) + 1, COG_SHAPE_EAST,
				 COG_CELL(CELL_BEHAVIOR_RED_COG, 0, CELL_DRAWS_ACTOR(2, 0)));
			MARK(x + (z << 7) + 2, COG_SHAPE_END,
				 COG_CELL(CELL_BEHAVIOR_COG_END, 0, 0));
			found = 1;
			break;
		case SPRITE_PINK_DRUM:
			MARK(x + (z << 7) - 256, COG_SHAPE_END,
				 COG_CELL(CELL_BEHAVIOR_COG_END, 0, 0));
			MARK(x + (z << 7) - 128, COG_SHAPE_NORTH,
				 COG_CELL(CELL_BEHAVIOR_ROLLER, 0, CELL_DRAWS_ACTOR(2, 0)));
			MARK(x + (z << 7), COG_SHAPE_TOP,
				 COG_CELL(CELL_BEHAVIOR_ROLLER, 0, CELL_DRAWS_ACTOR(2, 0)));
			MARK(x + (z << 7) + 128, COG_SHAPE_SOUTH,
				 COG_CELL(CELL_BEHAVIOR_ROLLER, 0, CELL_DRAWS_ACTOR(2, 0)));
			MARK(x + (z << 7) + 256, COG_SHAPE_PINK_DRUM_SOUTH_END,
				 COG_CELL(CELL_BEHAVIOR_PINK_DRUM_SOUTH_END, MAP_CELL_JUMP, 0));
			found = 1;
			break;
		case SPRITE_BLUE_PILLAR:
			MARK(x + (z << 7) - 256, COG_SHAPE_END,
				 COG_CELL(CELL_BEHAVIOR_COG_END, 0, 0));
			MARK(x + (z << 7) - 128, COG_SHAPE_NORTH,
				 COG_CELL(CELL_BEHAVIOR_ROLLER, 0, CELL_DRAWS_ACTOR(2, 0)));
			MARK(x + (z << 7), COG_SHAPE_TOP,
				 COG_CELL(CELL_BEHAVIOR_ROLLER, 0, CELL_DRAWS_ACTOR(2, 0)));
			MARK(x + (z << 7) + 128, COG_SHAPE_SOUTH,
				 COG_CELL(CELL_BEHAVIOR_ROLLER, 0, CELL_DRAWS_ACTOR(2, 0)));
			MARK(x + (z << 7) + 256, COG_SHAPE_END,
				 COG_CELL(CELL_BEHAVIOR_COG_END, 0, 0));
			found = 1;
			break;
		case SPRITE_BROWN_COG:
			found = 1;
			break;
		}
		if(found) {
			actor->collisionFlg = 0;
			actor->motionFlg = 0;
			actor->priFlg = 0;
			fdActorSetSprFlags(actor, 0);
		}
	}
	syTaskAddFrame(scGabombaStatuePauseCogsInMenus, TASK_PASS_FRAME);
}

/* Blends the room's layers by how far south the camera is: from 232 pixels, */
/* a step of the blend's sixteen every 8 pixels. */
void scGabombaStatueFadeByDepth(void) {
	s32 blend;
	ACTOR *camera = FieldStateWork->camera;
	s32 level = camera->z.fixed;

	level = FIXEDtoINT(level - toFIXED(232)) >> 3;
	CLAMP(level, 0, BLEND_WEIGHT_MAX);
	blend = ((BLEND_WEIGHT_MAX - level) << 8) + level;
	REG_BLDALPHA = blend;
}

/* Turns cog id by direction, its animation timers set to value. */
void scGabombaStatueSetCogTurn(s32 id, s32 direction, s32 value) {
	ACTOR *actor = fdActorGet(id);
	void *animation = actor->spr;

	fdActorSelectAnimation(actor,
						   GabombaStatue_CogAnimations[(direction + 1) & 3]);
	actor->retainSprCache = ACTOR_COMPOSE_OFF_SCREEN;
	fdSprSetAnimationTimers(animation, value);
}

/* Sets the cogs' direction and turns every cog of the room that way. */
void scGabombaStatueTurnCogs(s32 direction) {
	s16 scene;

	GabombaStatue_CogDirection = direction;
	scene = MainRoutineStateWork.next;
	if(scene == SCENE_GABOMBA_STATUE_ELIXIR_CHEST) {
		scGabombaStatueSetCogTurn(ACTOR_ELIXIR_GREEN_COGS, direction, 0);
		scGabombaStatueSetCogTurn(ACTOR_ELIXIR_BLUE_COGS, direction, 1);
		scGabombaStatueSetCogTurn(ACTOR_FLASHING_COG, direction, 2);
		if(MainRoutineStateWork.entrance <
			   GABOMBA_STATUE_ELIXIR_CHEST_FROM_GABOMBA_STATUE_STEEL_WEST ||
		   MainRoutineStateWork.entrance >
			   GABOMBA_STATUE_ELIXIR_CHEST_FROM_GABOMBA_STATUE_STEEL_EAST)
			scGabombaStatueSetCogTurn(ACTOR_ELIXIR_PINION_A, direction, 3);
		if(direction == 0)
			fdMapGridCopyCellAttributes(23, 21, 1, 3, 23, 9);
		else if(direction == -1)
			fdMapGridCopyCellAttributes(24, 21, 1, 3, 23, 9);
	}
	else if(scene == SCENE_GABOMBA_STATUE_STEEL) {
		scGabombaStatueSetCogTurn(ACTOR_STEEL_GREEN_COGS, direction, 0);
		scGabombaStatueSetCogTurn(ACTOR_STEEL_BLUE_COGS, direction, 1);
		scGabombaStatueSetCogTurn(ACTOR_STEEL_RED_COGS, direction, 2);
		scGabombaStatueSetCogTurn(ACTOR_STEEL_BLUE_PILLARS, direction, 3);
		scGabombaStatueSetCogTurn(ACTOR_STEEL_PINK_DRUMS, direction, 4);
	}
	else if(scene == SCENE_GABOMBA_STATUE_MOUSE) {
		scGabombaStatueSetCogTurn(ACTOR_MOUSE_GREEN_COGS, direction, 0);
		scGabombaStatueSetCogTurn(ACTOR_MOUSE_BLUE_COGS, direction, 1);
		scGabombaStatueSetCogTurn(ACTOR_MOUSE_RED_COGS, direction, 2);
		scGabombaStatueSetCogTurn(ACTOR_MOUSE_BLUE_PILLAR_B, direction, 3);
		scGabombaStatueSetCogTurn(ACTOR_MOUSE_PINK_DRUM_B, direction, 4);
	}
	else if(scene == SCENE_GABOMBA_STATUE_COGS) {
		scGabombaStatueSetCogTurn(ACTOR_COGS_RED_COGS, direction, 0);
		scGabombaStatueSetCogTurn(ACTOR_COGS_PINION_A, direction, 1);
		if(direction == 0)
			fdMapGridCopyCellAttributes(34, 12, 1, 3, 27, 12);
		else if(direction == -1)
			fdMapGridCopyCellAttributes(35, 12, 1, 3, 27, 12);
	}
	else if(scene == SCENE_GABOMBA_STATUE_ARROWS) {
		fdActorSetAnimation(ACTOR_CHARGE_C, 5);
		fdActorSetSprFlags(fdActorGet(ACTOR_CHARGE_C), 0);
	}
	else if(scene == SCENE_GABOMBA_STATUE_PINION) {
		scGabombaStatueSetCogTurn(ACTOR_PINION_A, direction, 0);
		scGabombaStatueSetCogTurn(ACTOR_PINION_B, direction, 1);
		scGabombaStatueSetCogTurn(ACTOR_PINION_C, direction, 2);
		scGabombaStatueSetCogTurn(ACTOR_PINION_D, direction, 3);
		scGabombaStatueSetCogTurn(ACTOR_PINION_E, direction, 4);
		if(direction == 0)
			fdMapGridCopyCellAttributes(0, 0, 1, 3, 9, 27);
		else if(direction == -1)
			fdMapGridCopyCellAttributes(1, 0, 1, 3, 9, 27);
	}
}

extern s32 GabombaStatue_ChargeScript[];

/* Sets charge id circling the orb statue statueId from the angle given, */
/* tinted colorMode. */
void scGabombaStatueSendCharge(s32 id, s32 colorMode, s32 statueId, s32 angle) {
	ACTOR *charge = fdActorGet(id);
	ACTOR *target = fdActorGet(statueId);

	fdActorSelectAnimation(charge, 0);
	fdActorStartScript(charge, GabombaStatue_ChargeScript);
	fdActorSetSprFlags(charge, 0);
	charge->motionFlg = 0;
	charge->watchedActor = (FIELD_OBJECT *)target;
	/* sceneCnt is its place on the circle, in 256ths of a turn, and */
	/* sceneByte its own actor id until the run sets chargeState there. */
	charge->sceneCnt = angle / (TRIG_TURN / 256);
	charge->sceneByte = id;
	fdActorSetSprColMode(charge, colorMode);
}
#undef DIFF_WITHIN
#undef MARK
#undef COG_CELL
#undef DEFINE_GABOMBA_STATUE_STEP_WHEN_HELD
#undef DEFINE_GABOMBA_STATUE_STEP_WHEN_HELD_FOUR
#undef DEFINE_GABOMBA_STATUE_WALK_LEADER_TO
#undef DEFINE_GABOMBA_STATUE_HIDE_LADDER_TOP_BLOCKS

#include "../field/field_effect_advance_motion.c"

/* Felix drops off the south end of a pink drum in the mouse's room above and */
/* falls into Steel's room, landing with a thump in a ring of dust; he sits a */
/* moment as the screen shakes, then stands. One of the two drums lands him */
/* beside Steel, who can then be talked to. */
void scGabombaStatueLandFromFall(void) {
	ACTOR *actor = fdActorGet(MainRoutineStateWork.leaderActorId);
	u32 i;
	EFFECT_OPTIONS options;

	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdMapGridCenterOnFollowedPoint();
	syWait(1);
	actor->y.fixed = toFIXED(130);
	actor->gravity = toFIXED(1) / 2;
	actor->bounce = 0;
	actor->motionFlg = 0;
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	fdEventStartScrTransition();
	fdEventWaitTransitionFrames();
	fdEventPlaySnd(SE_DUST_PUFF);
	actor->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	syFieldEventWait(24);
	options.pal = SPRITE_COLOR_WHITE;
	options.update = fdEffectAdvanceMotion;
	options.startScaleX = FIXED_ONE * 4 / 5;
	options.startScaleY = FIXED_ONE * 4 / 5;
	for(i = 0; i <= DUST_RING_PUFFS; i++) {
		s32 angle;
		FIXED velocity[3];

		angle = i * FACING_STEP;
		velocity[0] = syCos(angle);
		velocity[1] = 0;
		velocity[2] = syMathSin(angle);
		velocity[0] += velocity[0] >> 1;
		velocity[0] += (syRandomBelow(toFIXED(3) / 4)) - toFIXED(3) / 8;
		velocity[2] += (syRandomBelow(toFIXED(3) / 8)) - toFIXED(3) / 16;
		scFieldEffectSpawn(actor->x.fixed, actor->y.fixed, actor->z.fixed,
						   velocity[0], velocity[1], velocity[2],
						   EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE |
							   EFFECT_USE_PALETTE | 1,
						   &options);
	}
	fdEventPlaySnd(SE_THUMP);
	fdActorSetAttachedEffect(MainRoutineStateWork.leaderActorId,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_LEADER_SIT);
	fdMapGridSetShake(5, 5, 1);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	syFieldMapGridWaitForShakeToSettle();
	fdActorSetAttachedEffect(MainRoutineStateWork.leaderActorId,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
	actor->gravity = toFIXED(1);
	actor->bounce = FIXED_ONE / 4;
	syWait(10);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_STAND);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	fdEventEnd();
}

extern u32 GabombaStatue_LeaderFocusScript[];

/* Coming up the hole into the arrows room as the ceremony starts below: */
/* Akafubu calls "Focus your energies!", and Kraden calls the party over to */
/* watch. */
void scGabombaStatueFocusEnergies(void) {
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	syEventCameraGetActor()->motionFlg = 0;
	fdEventStartScrTransition();
	fdEventWaitTransitionFrames();
	syFieldEventWait(40);
	syFieldEventSetMessageId(MsgGabombaFocusEnergies);
	/* "Focus your energies!" */
	fdEventShowMessage(ACTOR_AKAFUBU, 0);
	fdActorPlaceOnActor(ACTOR_KRADEN, MainRoutineStateWork.leaderActorId);
	fdActorPlaceOnActor(ACTOR_JENNA, MainRoutineStateWork.leaderActorId);
	fdActorPlaceOnActor(ACTOR_SHEBA, MainRoutineStateWork.leaderActorId);
	fdActorPlaceOnActor(ACTOR_PIERS, MainRoutineStateWork.leaderActorId);
	syWait(1);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.8);
	fdActorSetSpeed(ACTOR_JENNA, 0.8);
	fdActorSetSpeed(ACTOR_SHEBA, 0.8);
	fdActorSetSpeed(ACTOR_PIERS, 0.8);
	fdActorSetSpeed(ACTOR_KRADEN, 0.8);
	fdActorSetScript(ACTOR_KRADEN, GabombaStatue_KradenFocusScript);
	syWait(10);
	fdActorSetScript(MainRoutineStateWork.leaderActorId,
					 GabombaStatue_LeaderFocusScript);
	fdActorSetScript(ACTOR_JENNA, GabombaStatue_JennaFocusScript);
	fdActorSetScript(ACTOR_PIERS, GabombaStatue_PiersFocusScript);
	scFieldActorWaitForScriptEndById(ACTOR_KRADEN);
	syFieldEventWait(20);
	fdCameraSetSpeed(1.2);
	fdCameraMoveTo(434, CAMERA_KEEP, 286, 1);
	fdActorSetSpeed(ACTOR_KRADEN, 1.2);
	fdActorRunScriptAndWait(ACTOR_KRADEN, GabombaStatue_KradenPointsScript);
	syFieldEventWait(40);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHEAST);
	/* "Everyone! Look over here!" */
	fdEventShowMessage(ACTOR_KRADEN, 0);
	fdCameraSetSpeed(4.8);
	fdCameraMoveTo(434, CAMERA_KEEP, 168, 1);
	fdCameraWaitForArrival();
	syFieldEventWait(20);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_NORTHWEST,
						 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTHEAST, 40);
	fdActorGet(ACTOR_PIERS)->facing = FACING_SOUTH;
	fdActorGet(MainRoutineStateWork.leaderActorId)->facing = FACING_SOUTH;
	fdActorGet(ACTOR_SHEBA)->facing = FACING_SOUTH;
	fdActorGet(ACTOR_JENNA)->facing = FACING_SOUTH;
	fdActorGet(ACTOR_PIERS)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorGet(MainRoutineStateWork.leaderActorId)->steeringFlg &=
		~ACTOR_FACES_MOTION;
	fdActorGet(ACTOR_SHEBA)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorGet(ACTOR_JENNA)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkTo(ACTOR_PIERS, 400, 272);
	fdActorWalkTo(MainRoutineStateWork.leaderActorId, 420, 272);
	fdActorWalkTo(ACTOR_SHEBA, 444, 272);
	fdActorWalkTo(ACTOR_JENNA, 468, 272);
	syFieldEventWait(20);
	fdPaletteFadeApplySource(PALETTE_TINT_NONE, 0);
	fdPaletteFadeApplyTarget(0, 0);
	fdPaletteFadeInterpolate(80);
	syWait(80);
	syFieldEventSetExitRequest(
		GABOMBA_STATUE_ARROWS_TO_GABOMBA_STATUE_FACE_EAST_B);
}

/* The party watches the orb statues' charges run along the broken channel, */
/* and is sent back down as the ceremony fails. */
void scGabombaStatueWatchTheChargesStick(void) {
	s32 south = FACING_SOUTH;

	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdEventPlaySnd(SOUND_FADE_OUT_BGM);
	fdActorSetPosAndFacing(MainRoutineStateWork.leaderActorId, toFIXED(348),
						   toFIXED(312), south);
	fdActorSetPosAndFacing(ACTOR_PIERS, toFIXED(330), toFIXED(312), south);
	fdActorSetPosAndFacing(ACTOR_KRADEN, toFIXED(432), toFIXED(312),
						   FACING_SOUTH_SOUTHEAST);
	fdActorSetPosAndFacing(ACTOR_SHEBA, toFIXED(448), toFIXED(312), south);
	fdActorSetPosAndFacing(ACTOR_JENNA, toFIXED(468), toFIXED(312), south);
	syWait(1);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
	fdEventStartScrTransition();
	fdEventWaitTransitionFrames();
	syFieldEventWait(40);
	fdEventPlaySnd(SE_SHIMMER);
	syWait(1);
	fdPaletteFadeApplySource(PALETTE_TINT_NONE, 0);
	fdPaletteFadeApplyTarget(RGB_WHITE, 0);
	fdPaletteFadeInterpolate(40);
	syWait(80);
	scGabombaStatueReleaseCharges();
	syWait(1);
	fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, 0);
	fdPaletteFadeInterpolate(10);
	syWait(20);
	fdActorShowEmote(MainRoutineStateWork.leaderActorId,
					 EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	scFieldActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	scFieldActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	scFieldActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	scFieldActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 60);
	fdEventPlaySnd(SE_PSYNERGY_START);
	scFieldActorSetSprCol(fdActorGet(ACTOR_BLUE_ORB_STATUE),
						  SPRITE_COLOR_WHITE);
	scFieldActorSetSprCol(fdActorGet(ACTOR_RED_ORB_STATUE), SPRITE_COLOR_WHITE);
	syWait(8);
	scFieldActorSetSprCol(fdActorGet(ACTOR_RED_ORB_STATUE), SPRITE_COLOR_BGR);
	scFieldActorSetSprCol(fdActorGet(ACTOR_BLUE_ORB_STATUE),
						  SPRITE_COLOR_NORMAL);
	syWait(8);
	fdActorGet(MainRoutineStateWork.leaderActorId)->lookTmr =
		(syRandomBelow(40)) + 10;
	fdActorGet(ACTOR_PIERS)->lookTmr = (syRandomBelow(40)) + 10;
	fdActorGet(ACTOR_JENNA)->lookTmr = (syRandomBelow(40)) + 10;
	fdActorGet(ACTOR_SHEBA)->lookTmr = (syRandomBelow(40)) + 10;
	fdActorGet(ACTOR_KRADEN)->lookTmr = (syRandomBelow(40)) + 10;
	fdActorSetScript(MainRoutineStateWork.leaderActorId,
					 GabombaStatue_LookNorthAndEastScript);
	fdActorSetScript(ACTOR_PIERS, GabombaStatue_LookNorthAndEastScript);
	fdActorSetScript(ACTOR_JENNA, GabombaStatue_LookNorthAndWestScript);
	fdActorSetScript(ACTOR_SHEBA, GabombaStatue_LookNorthAndWestScript);
	fdActorSetScript(ACTOR_KRADEN, GabombaStatue_LookNorthAndWestScript);
	fdEventPlaySnd(MUS_RITUALS);
	scGabombaStatueRunCharges(ACTOR_CHARGE_A, 0);
	syFieldEventWait(80);
	fdEventSetFlag(FLAG_GABOMBA_MUSIC_SILENCED);
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	syFieldEventSetExitRequest(
		GABOMBA_STATUE_ARROWS_TO_KIBOMBO_STATUE_GROUNDS_CEREMONY_NORTH_B_B);
}

extern u32 GabombaStatue_ChannelSeeds[16];

/* Kraden sees how the statue works, and the party agrees to mend the channel */
/* for Akafubu's ceremony. */
void scGabombaStatueSeeHowItWorks(void) {
	s32 piers = SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_PIERS;

	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdActorSetPosAndFacing(MainRoutineStateWork.leaderActorId, toFIXED(348),
						   toFIXED(312), FACING_NORTH);
	fdActorSetPosAndFacing(ACTOR_PIERS, toFIXED(330), toFIXED(312),
						   FACING_NORTH);
	fdActorSetPosAndFacing(ACTOR_KRADEN, toFIXED(432), toFIXED(312),
						   FACING_NORTH_NORTHWEST);
	fdActorSetPosAndFacing(ACTOR_SHEBA, toFIXED(448), toFIXED(312),
						   FACING_NORTH);
	fdActorSetPosAndFacing(ACTOR_JENNA, toFIXED(468), toFIXED(312),
						   FACING_NORTH);
	syWait(1);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
	fdEventStartScrTransition();
	fdEventWaitTransitionFrames();
	syFieldEventWait(40);
	scFieldActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	syFieldEventSetMessageId(MsgGabombaUnderstandsChannel);
	/* "Ah! So that's how it works!" */
	fdEventShowMessage(ACTOR_KRADEN, 0);
	syEventCameraGetActor()->motionFlg = 0;
	fdCameraSetSpeed(0.8);
	fdCameraMoveTo(392, CAMERA_KEEP, 292, 1);
	fdCameraWaitForArrival();
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_EAST, 0);
	fdActorFaceDirectionAndWait(ACTOR_SHEBA, FACING_WEST);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
	/* "He's been failing because the conduit that channels his Psynergy is..."
	 */
	fdEventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_SHEBA, 0);
	/* "Should we reconnect the conduit so he can complete the ceremony?" */
	fdEventOpenMessage(
		SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_JENNA, 0);
	if(syFieldEventAskYesNoWithActorAnimation(
		   MainRoutineStateWork.leaderActorId, 0) == 0) {
		scFieldActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
							  20);
		fdActorSetSpeed(ACTOR_PIERS, 2);
		fdActorWalkToAndWait(ACTOR_PIERS, 340, 296);
		fdActorWalkToAndWait(ACTOR_PIERS, 354, 296);
		fdActorWalkToAndWait(ACTOR_PIERS, 364, 308);
		fdActorFaceDirection(MainRoutineStateWork.leaderActorId,
							 FACING_NORTHEAST, 0);
		fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTHWEST);
		/* "I won't help them, Felix! They're the ones who stole my orb from
		 * the..." */
		fdEventShowMessage(
			SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_PIERS, 0);
		FieldStateWork->msg++;
	}
	else {
		FieldStateWork->msg++;
		fdActorSetSpeed(ACTOR_PIERS, 2);
		fdActorWalkToAndWait(ACTOR_PIERS, 340, 296);
		fdActorWalkToAndWait(ACTOR_PIERS, 354, 296);
		fdActorWalkToAndWait(ACTOR_PIERS, 364, 308);
		fdActorFaceDirection(MainRoutineStateWork.leaderActorId,
							 FACING_NORTHEAST, 0);
		fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTHWEST);
		fdActorSetAnimation(ACTOR_PIERS, ANIM_NOD);
		/* "I agree, Felix! Why should we help this pack of thieves?" */
		fdEventShowMessage(piers, 0);
	}
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_WEST);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
	/* "Piers, I understand that you're angry at them, but it really is in..."
	 */
	fdEventShowMessage(
		SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_EAST, 0);
	scFieldActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	/* "Think about it, Piers... Akafubu stole the orb solely to perform..." */
	fdEventShowMessage(
		SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0);
	fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
	scFieldActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
	/* "So, what do you think he intends to do with the orb?" */
	fdEventShowMessage(
		SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0);
	/* "He means to give it to the Great Gabomba..." */
	fdEventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_PIERS, 0);
	scFieldActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	/* "Oh! So...if his ceremony succeeds, the orb will be taken into the..." */
	fdEventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_PIERS, 0);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_NOD);
	/* "And we're INSIDE the statue! If the ceremony succeeds, we can go..." */
	fdEventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0);
	scFieldActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 10);
	/* "I...don't want to help that Akafubu, but..." */
	fdEventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_PIERS, 0);
	fdActorSetSpeed(ACTOR_SHEBA, 0.8);
	fdActorWalkToAndWait(ACTOR_SHEBA, 440, 296);
	fdActorFaceDirectionAndWait(ACTOR_SHEBA, FACING_WEST);
	fdActorSetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
	/* "I don't like the idea of helping him any more than you do, but if we..."
	 */
	fdEventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_SHEBA, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_NORTHEAST, 0);
	scFieldActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_EAST, 80);
	scFieldActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
	fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_EAST);
	/* "You're right, Sheba. Let's see if we can't help him finish his..." */
	fdEventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_PIERS, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 0);
	fdActorFaceDirectionAndWait(ACTOR_SHEBA, FACING_WEST);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_NOD);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_NOD);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHWEST);
	/* "It looks like the energy charge is supposed to run over these blocks."
	 */
	fdEventShowMessage(ACTOR_KRADEN, 0);
	scFieldActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 20);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHWEST);
	/* "If we can just move these around a little, that might be enough to..."
	 */
	fdEventShowMessage(ACTOR_KRADEN, 0);
	fdCameraSetSpeed(4.8);
	fdCameraMoveTo(392, CAMERA_KEEP, 176, 1);
	fdCameraWaitForArrival();
	/* "Ah! There we go!" */
	fdEventShowMessage(ACTOR_KRADEN, 0);
	/* "These panels have arrows on them... What do you think it all means?" */
	fdEventShowMessage(ACTOR_SHEBA, 0);
	/* "Look at the directions they point... I'll bet these will rotate the..."
	 */
	fdEventShowMessage(ACTOR_KRADEN, 0);
	/* "But how do we set the blocks back in place?" */
	fdEventShowMessage(ACTOR_PIERS, 0);
	/* "Just Pound them in!" */
	fdEventShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0);
	fdCameraMoveTo(392, CAMERA_KEEP, 292, 1);
	fdCameraWaitForArrival();
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_NOD);
	/* "Won't that do the trick?" */
	fdEventShowMessage(ACTOR_KRADEN, 0);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_WEST);
	/* "We must watch the path his Psynergy takes and move the blocks..." */
	fdEventShowMessage(ACTOR_KRADEN, 0);
	fdActorSetSpeed(ACTOR_JENNA, 1.6);
	fdActorSetSpeed(ACTOR_SHEBA, 1.6);
	fdActorSetSpeed(ACTOR_KRADEN, 1.6);
	fdActorSetSpeed(ACTOR_PIERS, 0.8);
	fdActorSetScript(ACTOR_PIERS, GabombaStatue_LookAtChannelScript);
	fdActorSetScript(ACTOR_KRADEN, GabombaStatue_LookAtChannelScript);
	fdActorSetScript(ACTOR_SHEBA, GabombaStatue_LookAtChannelScript);
	fdActorRunScriptAndWait(ACTOR_JENNA, GabombaStatue_LookAtChannelScript);
	fdEventSetFlag(FLAG_GABOMBA_SAW_HOW_IT_WORKS);
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	fdActorTableSnapshot();
	syFieldEventSetExitRequest(
		GABOMBA_STATUE_ARROWS_TO_KIBOMBO_STATUE_GROUNDS_CEREMONY_SOUTH);
}

/* Once the Gabomba has responded, Akafubu comes up into the statue, and the */
/* party tells him how it mended the channel and where the orb went. */
void scGabombaStatueSawEnergyFlow(void) {
	s32 index = scGabombaStatueGetPuzzleSeed();
	FIXED x;
	FIXED z;
	ACTOR *hand;
	ACTOR *actor;

	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	scGabombaStatueSetUpChannelPuzzle(CHANNEL_ORIGIN_COLUMN, CHANNEL_ORIGIN_ROW,
									  GabombaStatue_ChannelSeeds[index],
									  ACTOR_BLOCK_A);
	fdActorSetAnimation(ACTOR_ARROWS_STAKE, 5);
	fdActorSetPos(ACTOR_CHARGE_A, 0, 0);
	fdActorSetPos(ACTOR_CHARGE_B, 0, 0);
	fdActorSetPos(ACTOR_CHARGE_C, 0, 0);
	fdActorSetPos(ACTOR_CHARGE_D, 0, 0);
	fdActorSetPos(ACTOR_BLOCK_A, 0, 0);
	fdActorSetPos(ACTOR_BLOCK_B, 0, 0);
	fdActorSetPos(ACTOR_BLOCK_C, 0, 0);
	fdActorSetPos(ACTOR_BLOCK_D, 0, 0);
	fdActorSetPos(ACTOR_BLOCK_E, 0, 0);
	fdActorSetPos(ACTOR_BLOCK_F, 0, 0);
	fdActorSetPos(ACTOR_BLOCK_G, 0, 0);
	syWait(1);
	fdCameraMoveTo(440, CAMERA_KEEP, 182, 0);
	fdActorSetPos(MainRoutineStateWork.leaderActorId, 0, 0);
	fdActorSetPosAndFacing(ACTOR_JENNA, toFIXED(472), toFIXED(216),
						   FACING_SOUTH);
	fdActorSetPosAndFacing(ACTOR_AKAFUBU, toFIXED(472), toFIXED(200),
						   FACING_SOUTH_SOUTHEAST);
	syWait(1);
	fdMapGridCenterOnFollowedPoint();
	syWait(1);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
	fdEventStartScrTransition();
	fdEventWaitTransitionFrames();
	fdCameraSetSpeed(1.2);
	fdCameraMoveTo(440, CAMERA_KEEP, 312, 1);
	fdActorSetSpeed(ACTOR_JENNA, 1);
	fdActorSetSpeed(ACTOR_AKAFUBU, 1);
	fdActorSetScript(ACTOR_JENNA, GabombaStatue_JennaWatchesScript);
	fdActorRunScriptAndWait(ACTOR_AKAFUBU, GabombaStatue_AkafubuComesUpScript);
	syFieldEventSetMessageId(MsgGabombaSawEnergyFlow);
	/* "Yeah, we know... We saw your magic energy flowing past us." */
	fdEventShowMessage(ACTOR_JENNA, 0);
	fdActorFaceDirection(ACTOR_AKAFUBU, FACING_NORTH_NORTHWEST, 0);
	fdActorWalkToAndWait(ACTOR_JENNA, 440, 296);
	fdActorFaceDirectionAndWait(ACTOR_JENNA, FACING_EAST);
	/* "See how this channel runs along the floor?" */
	fdEventShowMessage(ACTOR_JENNA, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
	fdCameraMoveTo(414, CAMERA_KEEP, 272, 1);
	fdCameraWaitForArrival();
	/* "Here, look... It runs all the way over there!" */
	fdEventShowMessage(ACTOR_JENNA, 0);
	scGabombaStatueFindBrokenCell(&x, &z);
	x = FIELD_CELL_ORIGIN(x) + FIELD_CELL_CENTER(CHANNEL_ORIGIN_COLUMN);
	z = FIELD_CELL_ORIGIN(z) + FIELD_CELL_CENTER(CHANNEL_ORIGIN_ROW);
	hand = scFieldActorCreate(SPRITE_POINTING_HAND, x, 0, z);
	hand->motionFlg |= ACTOR_FLOATS;
	fdActorSetSprFlags(hand, 0);
	fdActorSelectAnimation(hand, 1);
	syWait(1);
	actor = fdActorGet(ACTOR_JENNA);
	actor->facing =
		syAtan(hand->z.fixed - actor->z.fixed, hand->x.fixed - actor->x.fixed);
	fdCameraMoveToFX(hand->x.fixed, -1, hand->z.fixed, 1);
	fdCameraWaitForArrival();
	/* "But the thing is, the circuit was broken right there." */
	fdEventShowMessage(ACTOR_JENNA, 0);
	fdActorRelease(hand);
	fdCameraMoveTo(440, CAMERA_KEEP, 304, 1);
	fdCameraWaitForArrival();
	fdActorFaceDirectionAndWait(ACTOR_JENNA, FACING_SOUTHEAST);
	/* "So we completed the circuit so that your magic energy could go where..."
	 */
	fdEventShowMessage(ACTOR_JENNA, 0);
	fdActorSetAttachedEffect(ACTOR_AKAFUBU,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldEventWait(20);
	/* "But...if you truly did this, then why? Why did you help me?" */
	fdEventShowMessage(ACTOR_AKAFUBU, 0);
	fdActorSetPosAndFacing(MainRoutineStateWork.leaderActorId, toFIXED(472),
						   toFIXED(204), FACING_SOUTH);
	fdActorSetPosAndFacing(ACTOR_PIERS, toFIXED(440), toFIXED(204),
						   FACING_SOUTH);
	fdActorSetPosAndFacing(ACTOR_KRADEN, toFIXED(456), toFIXED(204),
						   FACING_SOUTH_SOUTHEAST);
	fdActorSetPosAndFacing(ACTOR_SHEBA, toFIXED(488), toFIXED(204),
						   FACING_SOUTH);
	syWait(1);
	/* "We were sort of hoping that we could take the orb if your ceremony..."
	 */
	fdEventShowMessage(ACTOR_KRADEN, 0);
	fdActorFaceDirection(ACTOR_AKAFUBU, FACING_NORTH_NORTHEAST, 0);
	fdCameraMoveTo(440, CAMERA_KEEP, 264, 1);
	fdCameraWaitForArrival();
	scFieldActorJump(ACTOR_AKAFUBU, 4, 10);
	/* "Ah, so you have taken the orb? Then begone! You have no further..." */
	fdEventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_AKAFUBU, 0);
	fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_SHAKE_HEAD);
	/* "But...we didn't get the orb." */
	fdEventShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_PIERS, 0);
	fdActorShowEmote(ACTOR_AKAFUBU, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	/* "It rolled through this groove and right through this hole." */
	fdEventShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_SHEBA, 0);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
	/* "We saw a light through there, so we suspect there must be another..." */
	fdEventShowMessage(
		SPEAKER_PORTRAIT_LEFT | SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 0);
	syFieldEventWait(20);
	fdPaletteFadeApplySource(PALETTE_TINT_NONE, 0);
	fdPaletteFadeApplyTarget(0, 0);
	fdPaletteFadeInterpolate(60);
	fdEventClearFlag(FLAG_KEEP_PARTY_POSITION);
	syWait(60);
	syFieldEventSetExitRequest(
		GABOMBA_STATUE_ARROWS_TO_GABOMBA_STATUE_CHAMBER_IDOL_WEST);
}

/* Back from a failed ceremony whose charges reached the statue in the middle */
/* apart, Kraden notices it is their timing that matters, and the clock starts
 */
/* again. */
void scGabombaStatueKradenNoticesTiming(void) {
	scGabombaStatueStopClock();
	fdActorPlaceOnActor(ACTOR_KRADEN, MainRoutineStateWork.leaderActorId);
	scFieldActorSetSprCol(fdActorGet(ACTOR_KRADEN), SPRITE_COLOR_HIDDEN);
	syWait(10);
	syFieldEventSetMessageId(MsgGabombaChannelTiming);
	/* "It looks like just fixing the channel should do the trick... Maybe..."
	 */
	fdEventShowMessage(ACTOR_KRADEN, 0);
	fdActorSetPos(ACTOR_KRADEN, 0, 0);
	syWait(1);
	fdEventSetFlag(FLAG_GABOMBA_TIMING_NOTICED);
	scGabombaStatueRestartClock();
}

/* Sets up whichever room of the statue is being entered: its cogs turning, */
/* stopped by the rope or turned the other way by the stakes, then the room's */
/* own things. */
s32 scInit(void) {
	s16 scene;

	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	if(MainRoutineStateWork.next != SCENE_GABOMBA_STATUE_BONE_ARMLET_CHEST) {
		if(fdEventGetFlag(FLAG_COGS_STOPPED))
			scGabombaStatueTurnCogs(0);
		else if(fdEventGetFlag(FLAG_GABOMBA_COGS_TURNED_BACK))
			scGabombaStatueTurnCogs(-1);
		else
			scGabombaStatueTurnCogs(1);
		scGabombaStatueDrawCogCells();
	}
	fdActorGet(syFieldPartyGetLeaderActor())->priFlg |=
		ACTOR_PRIORITY_IGNORES_RISES;
	scene = MainRoutineStateWork.next;
	if(scene == SCENE_GABOMBA_STATUE_BONE_ARMLET_CHEST) {
		scGabombaStatueInitBoneArmletChest();
	}
	else if(scene == SCENE_GABOMBA_STATUE_ELIXIR_CHEST) {
		fdMapGridCopyCellAttributes(15, 68, 8, 3, 15, 4);
		scGabombaStatueInitElixirChest();
	}
	else if(scene == SCENE_GABOMBA_STATUE_STEEL) {
		scGabombaStatueInitSteel();
	}
	else if(scene == SCENE_GABOMBA_STATUE_COGS) {
		scGabombaStatueInitCogs();
	}
	else if(scene == SCENE_GABOMBA_STATUE_ARROWS) {
		scGabombaStatueInitArrows();
	}
	else if(scene == SCENE_GABOMBA_STATUE_PINION) {
		scGabombaStatueInitPinion();
	}
	return 0;
}

/* Coming into the hall of pinions from outside sets the cogs and the stakes */
/* back as they were. */
s32 scPrepareMap(void) {
	if(MainRoutineStateWork.next == SCENE_GABOMBA_STATUE_PINION &&
	   MainRoutineStateWork.entrance ==
		   GABOMBA_STATUE_PINION_FROM_KIBOMBO_STATUE_GROUNDS &&
	   !fdEventGetFlag(FLAG_KEEP_PARTY_POSITION)) {
		fdEventClearFlag(FLAG_GABOMBA_COGS_TURNED_BACK);
		fdEventClearFlag(FLAG_GABOMBA_FIRST_STAKE_POUNDED);
		fdEventClearFlag(FLAG_GABOMBA_SECOND_STAKE_POUNDED);
	}
	return 0;
}

/* Sets up the Bone Armlet chest's room. */
void scGabombaStatueInitBoneArmletChest(void) {
	scGabombaStatueHideBoneArmletChestLadderTopBlock();
}

/* Places rope index between two posts, tied when its flag is set, else coiled:
 */
/* the event inline over the scene's import of fdRopePlace, as event_inline.h */
/* writes them. */
static inline void scFieldRopePlace(s32 index, s32 flag, s32 firstPost,
									s32 secondPost) {
	scGabombaStatuePlaceRope(index, flag, firstPost, secondPost);
}

/* Sets up the Elixir chest's room: its pinions let go of the leader coming */
/* in from the west or the east of Steel's room, the stakes, the flashing */
/* cog raised until both are Pounded, and the two ropes. */
void scGabombaStatueInitElixirChest(void) {
	if(MainRoutineStateWork.entrance >=
		   GABOMBA_STATUE_ELIXIR_CHEST_FROM_GABOMBA_STATUE_STEEL_WEST &&
	   MainRoutineStateWork.entrance <=
		   GABOMBA_STATUE_ELIXIR_CHEST_FROM_GABOMBA_STATUE_STEEL_EAST) {
		fdActorSetScript(ACTOR_ELIXIR_PINION_A,
						 GabombaStatue_PinionReleaseScript);
		fdActorSetScript(ACTOR_ELIXIR_PINION_B,
						 GabombaStatue_PinionReleaseScript);
		fdMapGridCopyCellAttributes(33, 24, 3, 3, 27, 29);
	}
	scGabombaStatuePlaceSwitchActors(GabombaStatue_Stakes);
	if(!fdEventGetFlag(FLAG_GABOMBA_FIRST_STAKE_POUNDED) ||
	   !fdEventGetFlag(FLAG_GABOMBA_SECOND_STAKE_POUNDED))
		fdActorGet(ACTOR_FLASHING_COG)->y.fixed = toFIXED(16);
	fdRopeInit();
	scFieldRopePlace(FIRST_ROPE, FLAG_GABOMBA_FIRST_ROPE_TIED,
					 ACTOR_FIRST_ROPE_POST_A, ACTOR_FIRST_ROPE_POST_B);
	scFieldRopePlace(SECOND_ROPE, FLAG_GABOMBA_SECOND_ROPE_TIED,
					 ACTOR_SECOND_ROPE_POST_A, ACTOR_SECOND_ROPE_POST_B);
	scGabombaStatueHideElixirChestLadderTopBlocks();
}

/* Sets up Steel's room: its rope, and the fall in from the mouse's room. */
void scGabombaStatueInitSteel(void) {
	fdRopeInit();
	scFieldRopePlace(FIRST_ROPE, FLAG_GABOMBA_FIRST_ROPE_TIED,
					 ACTOR_STEEL_ROPE_POST_A, ACTOR_STEEL_ROPE_POST_B);
	if(!fdEventGetFlag(FLAG_KEEP_PARTY_POSITION)) {
		if(MainRoutineStateWork.entrance >=
			   GABOMBA_STATUE_STEEL_FROM_GABOMBA_STATUE_MOUSE_SOUTHEAST_A &&
		   MainRoutineStateWork.entrance <=
			   GABOMBA_STATUE_STEEL_FROM_GABOMBA_STATUE_MOUSE_SOUTHEAST_B)
			scGabombaStatueLandFromFall();
		if(MainRoutineStateWork.entrance ==
		   GABOMBA_STATUE_STEEL_FROM_GABOMBA_STATUE_MOUSE_SOUTHEAST_A)
			fdEventSetFlag(FLAG_LANDED_BESIDE_STEEL);
	}
}

/* Sets up the room below the small idol's chamber: its rope, coiled. */
void scGabombaStatueInitCogs(void) {
	fdRopeInit();
	Value3((s32 (*)())scGabombaStatueCoilRope, FIRST_ROPE,
		   ACTOR_COGS_ROPE_POST_A, ACTOR_COGS_ROPE_POST_B);
}

/* Which of the channel's seeds this game lays it from: the letters of the */
/* leader's name added up. */
s32 scGabombaStatueGetPuzzleSeed(void) {
	UNIT *leader = syUnitGet(MainRoutineStateWork.leaderActorId);
	s32 sum = 0;
	u32 i;

	for(i = 0; i < UNIT_NAME_SIZE - 1; i++)
		sum += leader->name[i];
	return sum & (ARRAY_COUNT(GabombaStatue_ChannelSeeds) - 1);
}

void scGabombaStatueCheckOfferingClock();

/* Sets up the arrows room: its orb statues, its charges and the channel. */
void scGabombaStatueInitArrows(void) {
	ACTOR *leader;
	s32 seed;

	scFieldActorSetSprCol(fdActorGet(ACTOR_RED_ORB_STATUE), SPRITE_COLOR_BGR);
	syTaskAddFrame((s32)scGabombaStatueFadeByDepth, TASK_PASS_FRAME);
	scFieldActorSetSprCol(fdActorGet(ACTOR_CHARGE_A), SPRITE_COLOR_GREEN_TINT);
	scFieldActorSetSprCol(fdActorGet(ACTOR_CHARGE_B), SPRITE_COLOR_GREEN_TINT);
	scFieldActorSetSprCol(fdActorGet(ACTOR_CHARGE_C), SPRITE_COLOR_RED_TINT);
	scFieldActorSetSprCol(fdActorGet(ACTOR_CHARGE_D), SPRITE_COLOR_RED_TINT);
	fdActorSetSprFlags(fdActorGet(ACTOR_BLOCK_A), 0);
	fdActorSetSprFlags(fdActorGet(ACTOR_BLOCK_B), 0);
	fdActorSetSprFlags(fdActorGet(ACTOR_BLOCK_C), 0);
	fdActorSetSprFlags(fdActorGet(ACTOR_BLOCK_D), 0);
	fdActorSetSprFlags(fdActorGet(ACTOR_BLOCK_E), 0);
	fdActorSetSprFlags(fdActorGet(ACTOR_BLOCK_F), 0);
	fdActorSetSprFlags(fdActorGet(ACTOR_BLOCK_G), 0);
	if(fdEventGetFlag(FLAG_GABOMBA_RESPONDED) &&
	   !fdEventGetFlag(FLAG_AKAFUBU_OFFERED_HELP)) {
		scGabombaStatueSawEnergyFlow();
		return;
	}
	if(fdEventGetFlag(FLAG_GABOMBA_BACK_FROM_CEREMONY)) {
		leader = fdActorGet(MainRoutineStateWork.leaderActorId);
		fdActorSetPos(ACTOR_CHARGE_A, 0, 0);
		fdActorSetPos(ACTOR_CHARGE_B, 0, 0);
		fdActorSetPos(ACTOR_CHARGE_C, 0, 0);
		fdActorSetPos(ACTOR_CHARGE_D, 0, 0);
		scGabombaStatueResetClock();
		fdEventStart();
		syFieldEventEndLastingPsynergy(0);
		fdCameraMoveToFX(leader->x.fixed, -1, leader->z.fixed, 0);
		fdMapGridCenterOnFollowedPoint();
		syWait(1);
		fdEventEnd();
	}
	if(fdEventGetFlag(FLAG_GABOMBA_SAW_HOW_IT_WORKS) &&
	   !fdEventGetFlag(FLAG_AKAFUBU_OFFERED_HELP)) {
		scGabombaStatueStartClock(0);
		syTaskAddFrame((s32)scGabombaStatueCheckOfferingClock, TASK_PASS_FRAME);
	}
	seed = scGabombaStatueGetPuzzleSeed();
	scGabombaStatueSetUpChannelPuzzle(CHANNEL_ORIGIN_COLUMN, CHANNEL_ORIGIN_ROW,
									  GabombaStatue_ChannelSeeds[seed],
									  ACTOR_BLOCK_A);
	fdActorSetAnimation(ACTOR_ARROWS_STAKE, 5);
	if(MainRoutineStateWork.entrance ==
	   GABOMBA_STATUE_ARROWS_FROM_KIBOMBO_STATUE_GROUNDS_CEREMONY_NORTH_A) {
		scGabombaStatueWatchTheChargesStick();
		return;
	}
	if(!fdEventGetFlag(FLAG_GABOMBA_SAW_HOW_IT_WORKS)) {
		if(MainRoutineStateWork.entrance ==
		   GABOMBA_STATUE_ARROWS_FROM_GABOMBA_STATUE_CHAMBER_IDOL_EAST)
			scGabombaStatueFocusEnergies();
		else if(
			MainRoutineStateWork.entrance ==
			GABOMBA_STATUE_ARROWS_FROM_KIBOMBO_STATUE_GROUNDS_CEREMONY_NORTH_B_A)
			scGabombaStatueSeeHowItWorks();
	}
	if(fdEventGetFlag(FLAG_GABOMBA_BACK_FROM_CEREMONY)) {
		fdEventStart();
		syFieldEventEndLastingPsynergy(0);
		FieldStateWork->transition =
			SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
		fdEventStartScrTransition();
		fdEventWaitTransitionFrames();
		FieldStateWork->transition =
			SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
		fdEventClearFlag(FLAG_GABOMBA_BACK_FROM_CEREMONY);
		if(fdEventGetFlag(FLAG_GABOMBA_CHARGES_REACHED_APART) &&
		   !fdEventGetFlag(FLAG_GABOMBA_TIMING_NOTICED))
			scGabombaStatueKradenNoticesTiming();
		fdEventClearFlag(FLAG_SHOW_AREA_NAME);
		fdEventEnd();
	}
}

/* Sets up the hall of pinions; coming in from outside, Retreat leads back to */
/* it. */
void scGabombaStatueInitPinion(void) {
	fdActorGet(MainRoutineStateWork.leaderActorId)->priFlg |=
		ACTOR_PRIORITY_IGNORES_RISES;
	if(MainRoutineStateWork.entrance ==
	   GABOMBA_STATUE_PINION_FROM_KIBOMBO_STATUE_GROUNDS) {
		fdEventSetFlag(FLAG_GABOMBA_RETREAT_TO_PINION);
		fdEventClearFlag(FLAG_GABOMBA_RETREAT_TO_IDOL);
	}
}

static __inline__ void *scMemAlloc(s32 size) { return syHeapAlloc(size); }

/* A raised block's picture: 16 pixels square in 256 colours, one for each */
/* piece in CHANNEL_PIECE's order. */
enum { BLOCK_PICTURE_BYTES = 16 * 16 };

/* Loads the blocks' pictures into the VRAM cache. */
void scGabombaStatueLoadBlockPictures(void) {
	SCENE_STATE *state = &SceneStateWork;
	void *buffer = scMemAlloc(PIECE_KINDS * BLOCK_PICTURE_BYTES);
	s32 handle;

	syDecompressTaggedLz(Field_CachedTilesA, buffer);
	handle = syVramCacheFindFreeEntry();
	syVramCacheLoad(handle, PIECE_KINDS * BLOCK_PICTURE_BYTES, buffer);
	state->blockTileEntry = handle;
	syHeapRelease(buffer);
}

/* Whether channel cell (x, z) is taken: blocked, holding a block, or a stop; */
/* 0 outside the grid. */
s32 scGabombaStatueIsCellTaken(u32 x, u32 z) {
	u8 *cells = ((u8 *)&SceneStateWork);

	if(x >= CHANNEL_COLUMNS || z >= CHANNEL_ROWS)
		return 0;
	{
		PUZZLE_CELL *cell = (PUZZLE_CELL *)(cells + (z * CHANNEL_COLUMNS + x) *
														sizeof(PUZZLE_CELL));
		s32 code;

		if(cell->occupied != BLOCK_NONE)
			return 1;
		if(cell->blocked != 0)
			return 1;
		code = cell->kind;
		return code == PIECE_STOP;
	}
}

/* Whether a cell beside channel cell (x, z) is taken. */
s32 scGabombaStatueHasTakenNeighbor(s32 x, s32 z) {
	if(scGabombaStatueIsCellTaken(x, z - 1) ||
	   scGabombaStatueIsCellTaken(x, z + 1) ||
	   scGabombaStatueIsCellTaken(x - 1, z))
		return 1;
	if(scGabombaStatueIsCellTaken(x + 1, z))
		return 1;
	return 0;
}

/* Redraws the channel as the party left it: a raised block's socket open, the
 */
/* other blocks in place, and once the Gabomba has responded every block set */
/* back in. */
void scGabombaStatueRedrawChannel(void) {
	SCENE_STATE *state = &SceneStateWork;
	u32 row;
	u32 col;

	for(row = 0; row < CHANNEL_ROWS; row++)
		for(col = 0; col < CHANNEL_COLUMNS; col++) {
			PUZZLE_CELL *cell = state->cells[0] + (row * CHANNEL_COLUMNS + col);
			s32 occupied = cell->occupied;

			if(fdEventGetFlag(FLAG_GABOMBA_RESPONDED) &&
			   cell->occupied == BLOCK_RAISED) {
				cell->occupied = BLOCK_IN_PLACE;
				occupied = BLOCK_IN_PLACE;
			}
			if(occupied == BLOCK_RAISED)
				fdMapGridCopyCells(BLOCK_NONE, PIECE_SOCKET, 1, 1,
								   state->originX + col, state->originZ + row);
			else if(occupied == BLOCK_IN_PLACE)
				fdMapGridCopyCells(BLOCK_IN_PLACE, state->cells[row][col].kind,
								   1, 1, state->originX + col,
								   state->originZ + row);
			else
				fdMapGridCopyCells(occupied, state->cells[row][col].kind, 1, 1,
								   state->originX + col, state->originZ + row);
		}
}

s32 scGabombaStatueTurnBlockPiece(s32 x, s32 z, s32 direction);

/* The cells of the charges' paths to the statue in the middle: the two sent */
/* east run EAST_PATH_CELLS and the two sent west WEST_PATH_CELLS, so that a */
/* blue and a red charge arrive together. The paths from the statues' inner */
/* sides meet after PATH_TO_CROSSING_CELLS on the crossing's own cell. */
enum { EAST_PATH_CELLS = 17, WEST_PATH_CELLS = 19, PATH_TO_CROSSING_CELLS = 4 };

/* Lays the channel from seed, trying the seeds after it until each charge's */
/* path reaches the statue in the middle as the puzzle needs, then breaks the */
/* circuit and loosens the seven blocks, turned at random. Returns the seed it
 */
/* was laid from. */
s32 scGabombaStatueLayChannel(s32 seed) {
	SCENE_STATE *state = &SceneStateWork;
	s32 row, col, n, blueShared, redShared, crossed, tries, blocksLeft,
		westward;
	s32 piece, paths;
	PUZZLE_CELL *cell;

	state->seed = seed - 1;
retry:
	state->seed += 1;
	RandomSeed = state->seed;
	syWordsClearEntry(state, sizeof(state->cells));
	state->cells[CHANNEL_MIDDLE_STATUE_ROW][CHANNEL_MIDDLE_STATUE_COLUMN].kind =
		PIECE_STOP;
	state->cells[CHANNEL_STATUES_ROW][CHANNEL_BLUE_STATUE_COLUMN].kind =
		PIECE_STOP;
	state->cells[CHANNEL_STATUES_ROW][CHANNEL_RED_STATUE_COLUMN].kind =
		PIECE_STOP;
	state->cells[CHANNEL_STATUES_ROW][CHANNEL_BLUE_STATUE_COLUMN - 1].blocked =
		1;
	state->cells[CHANNEL_STATUES_ROW][CHANNEL_BLUE_STATUE_COLUMN + 1].blocked =
		1;
	state->cells[CHANNEL_STATUES_ROW][CHANNEL_RED_STATUE_COLUMN - 1].blocked =
		1;
	state->cells[CHANNEL_STATUES_ROW][CHANNEL_RED_STATUE_COLUMN + 1].blocked =
		1;
	for(row = 0; row < CHANNEL_ROWS; row++)
		for(col = 0; col < CHANNEL_COLUMNS; col++)
			fdMapGridCopyCells(BLOCK_NONE, state->cells[row][col].kind, 1, 1,
							   state->originX + col, state->originZ + row);
	/* The charges sent east from the blue statue and west from the red cross */
	/* on a piece of two curves between the statues, the crossing's cell counted
	 */
	/* in neither part of their paths. */
	n = 0;
	if(syRandom() < 32768) {
		n |= scGabombaStatueLayPath(2, 7, 5, 7, PATH_TO_CROSSING_CELLS,
									HEADING_EAST, HEADING_NORTH,
									PATH_BIT_BLUE_EAST);
		n |= scGabombaStatueLayPath(8, 7, 6, 6, PATH_TO_CROSSING_CELLS,
									HEADING_WEST, HEADING_WEST,
									PATH_BIT_RED_WEST);
		scGabombaStatueWritePiece(5, 6, PIECE_SOUTH_WEST_AND_NORTH_EAST,
								  PATH_BLUE_EAST | PATH_RED_WEST);
		n |= scGabombaStatueLayPath(
			4, 6, 4, 3, EAST_PATH_CELLS - PATH_TO_CROSSING_CELLS - 1,
			HEADING_WEST, HEADING_EAST, PATH_BIT_BLUE_EAST);
		n |= scGabombaStatueLayPath(
			5, 5, 5, 2, WEST_PATH_CELLS - PATH_TO_CROSSING_CELLS - 1,
			HEADING_NORTH, HEADING_SOUTH, PATH_BIT_RED_WEST);
	}
	else {
		n |= scGabombaStatueLayPath(2, 7, 4, 6, PATH_TO_CROSSING_CELLS,
									HEADING_EAST, HEADING_EAST,
									PATH_BIT_BLUE_EAST);
		n |= scGabombaStatueLayPath(8, 7, 5, 7, PATH_TO_CROSSING_CELLS,
									HEADING_WEST, HEADING_NORTH,
									PATH_BIT_RED_WEST);
		scGabombaStatueWritePiece(5, 6, PIECE_EAST_SOUTH_AND_WEST_NORTH,
								  PATH_BLUE_EAST | PATH_RED_WEST);
		n |= scGabombaStatueLayPath(
			5, 5, 4, 3, EAST_PATH_CELLS - PATH_TO_CROSSING_CELLS - 1,
			HEADING_NORTH, HEADING_EAST, PATH_BIT_BLUE_EAST);
		n |= scGabombaStatueLayPath(
			6, 6, 5, 2, WEST_PATH_CELLS - PATH_TO_CROSSING_CELLS - 1,
			HEADING_EAST, HEADING_SOUTH, PATH_BIT_RED_WEST);
	}
	if(n != 0)
		goto retry;
	n = scGabombaStatueLayPath(0, 7, 5, 4, WEST_PATH_CELLS, HEADING_WEST,
							   HEADING_NORTH, PATH_BIT_BLUE_WEST);
	n |= scGabombaStatueLayPath(10, 7, 6, 3, EAST_PATH_CELLS, HEADING_EAST,
								HEADING_WEST, PATH_BIT_RED_EAST);
	if(n != 0)
		goto retry;
	/* The two blue paths must share a cell, the two red ones too, and the */
	/* crossing must stand. */
	blueShared = 0;
	redShared = 0;
	crossed = 0;
	for(row = 0; row < CHANNEL_ROWS; row++)
		for(col = 0; col < CHANNEL_COLUMNS; col++) {
			piece = state->cells[row][col].kind;
			paths = state->cells[row][col].state;
			if(paths == PATH_BLUE)
				blueShared++;
			if(paths == PATH_RED)
				redShared++;
			if(paths == (PATH_BLUE_EAST | PATH_RED_WEST) &&
			   (u32)(piece - PIECE_EAST_SOUTH_AND_WEST_NORTH) <= 1)
				crossed++;
		}
	if(blueShared == 0 || redShared == 0 || crossed == 0)
		goto retry;
	/* Breaks the circuit: a straight piece near the statues, on the eastward */
	/* paths or the westward ones, becomes a block with no piece on it. */
	westward = syRandomBelow(2);
	blocksLeft = CHANNEL_BLOCKS;
	for(tries = 1; tries <= CHANNEL_CELLS; tries++) {
		col = syRandomBelow(CHANNEL_COLUMNS);
		row = (syRandomBelow(3)) + 5;
		if(scGabombaStatueHasTakenNeighbor(col, row) == 0) {
			piece = state->cells[row][col].kind;
			if((u32)(piece - PIECE_NORTH_SOUTH) <= 1) {
				paths = state->cells[row][col].state;
				if((westward == 0 &&
					(paths & (PATH_RED_EAST | PATH_BLUE_EAST)) != 0) ||
				   (westward == 1 &&
					(paths & (PATH_BLUE_WEST | PATH_RED_WEST)) != 0)) {
					state->cells[row][col].kind = PIECE_STOP;
					state->cells[row][col].occupied = BLOCK_IN_PLACE;
					blocksLeft = CHANNEL_BLOCKS - 1;
					break;
				}
			}
		}
	}
	/* Four crossings or pieces of two curves lose one of their ways as blocks.
	 */
	for(n = 0; n < 4; n++)
		for(tries = 1; tries <= CHANNEL_CELLS; tries++) {
			col = syRandomBelow(CHANNEL_COLUMNS);
			row = syRandomBelow(CHANNEL_ROWS);
			if(scGabombaStatueHasTakenNeighbor(col, row) == 0) {
				piece = state->cells[row][col].kind;
				if(piece == PIECE_CROSSING ||
				   piece == PIECE_EAST_SOUTH_AND_WEST_NORTH ||
				   piece == PIECE_SOUTH_WEST_AND_NORTH_EAST) {
					cell = &state->cells[row][col];
					paths = cell->state;
					if(paths != (PATH_RED_EAST | PATH_BLUE_EAST) &&
					   paths != (PATH_BLUE_WEST | PATH_RED_WEST)) {
						switch(piece) {
						case PIECE_CROSSING:
							cell->kind = (syRandomBelow(2)) + PIECE_NORTH_SOUTH;
							break;
						case PIECE_EAST_SOUTH_AND_WEST_NORTH:
							cell->kind =
								((syRandomBelow(2)) << 1) + PIECE_EAST_SOUTH;
							break;
						case PIECE_SOUTH_WEST_AND_NORTH_EAST:
							cell->kind =
								((syRandomBelow(2)) << 1) + PIECE_SOUTH_WEST;
							break;
						}
						state->cells[row][col].occupied = BLOCK_IN_PLACE;
						blocksLeft--;
						break;
					}
				}
			}
		}
	/* One curve, then straight pieces for the blocks blocksLeft. */
	for(tries = 1; tries <= CHANNEL_CELLS; tries++) {
		col = syRandomBelow(CHANNEL_COLUMNS);
		row = syRandomBelow(CHANNEL_ROWS);
		if(scGabombaStatueHasTakenNeighbor(col, row) == 0) {
			piece = state->cells[row][col].kind;
			if(piece == PIECE_EAST_SOUTH || piece == PIECE_WEST_NORTH ||
			   piece == PIECE_SOUTH_WEST || piece == PIECE_NORTH_EAST) {
				state->cells[row][col].occupied = BLOCK_IN_PLACE;
				blocksLeft--;
				break;
			}
		}
	}
	for(n = 0; n < blocksLeft; n++)
		for(tries = 1; tries <= CHANNEL_CELLS; tries++) {
			col = syRandomBelow(CHANNEL_COLUMNS);
			row = syRandomBelow(CHANNEL_ROWS);
			if(scGabombaStatueHasTakenNeighbor(col, row) == 0) {
				piece = state->cells[row][col].kind;
				if((u32)(piece - PIECE_NORTH_SOUTH) <= 1) {
					state->cells[row][col].occupied = BLOCK_IN_PLACE;
					break;
				}
			}
		}
	/* Turns each block at random, and draws the channel; paths now holds what
	 */
	/* stands on each cell. */
	for(row = 0; row < CHANNEL_ROWS; row++)
		for(col = 0; col < CHANNEL_COLUMNS; col++) {
			piece = state->cells[row][col].kind;
			paths = state->cells[row][col].occupied;
			if(paths != BLOCK_NONE) {
				switch(syRandomBelow(8)) {
				case 0:
				case 1:
				case 2:
					piece = scGabombaStatueTurnBlockPiece(
						col, row, TURN_COUNTERCLOCKWISE);
					break;
				case 3:
				case 4:
				case 5:
					piece =
						scGabombaStatueTurnBlockPiece(col, row, TURN_CLOCKWISE);
					break;
				case 6:
					scGabombaStatueTurnBlockPiece(col, row,
												  TURN_COUNTERCLOCKWISE);
					piece = scGabombaStatueTurnBlockPiece(
						col, row, TURN_COUNTERCLOCKWISE);
					break;
				}
			}
			fdMapGridCopyCells(paths, piece, 1, 1, state->originX + col,
							   state->originZ + row);
		}
	return state->seed;
}

/* How long scGabombaStatueLayPath searches: the steps it takes in all before */
/* it gives up, and the random headings it tries from a cell before backing up.
 */
enum { PATH_ATTEMPTS_MAX = 1280, PATH_TRIES_PER_STEP = 63 };
/* The chance, out of syRandom's 65,536, that a try keeps the path's heading; */
/* the other half of the time it tries a random one. */
#define PATH_KEEP_HEADING_CHANCE 32768

/* scGabombaStatueLayPath's steps so far: each step's heading, cell, piece, */
/* headings tried and the paths through its cell before it was joined. */
u8 GabombaStatue_PathHeadings[32] BSS_DATA;
u8 GabombaStatue_PathColumns[32] BSS_DATA;
u8 GabombaStatue_PathRows[32] BSS_DATA;
u8 GabombaStatue_PathPieces[32] BSS_DATA;
u8 GabombaStatue_PathTried[32] BSS_DATA;
u8 GabombaStatue_PathCellPaths[32] BSS_DATA;
extern s8 GabombaStatue_PathSteps[];
extern u8 GabombaStatue_PieceJoins[];
/* Lays a random path of `length` cells across the channel from (x0, z0) to */
/* (endX, endZ), the pieces it passes joined to it, backing up a step when a */
/* cell has no free way on. Returns 0, or -1 when the distance's parity rules */
/* such a path out or the search gives up. */
s32 scGabombaStatueLayPath(s32 x0, s32 z0, s32 endX, s32 endZ, s32 length,
						   s32 direction, s32 last, s32 bit) {
	u8 *cells, *kinds;
	s32 piece;
	s32 spare, step, dir, used, dz, index, one;
	u32 x, z, nextX, nextZ, tries, attempts;
	u8 tried;

	length--;
	dz = endZ - z0;
	spare = length - abs(endX - x0) - abs(dz);
	syWordsClearEntry(GabombaStatue_PathHeadings,
					  sizeof(GabombaStatue_PathHeadings));
	syWordsClearEntry(GabombaStatue_PathColumns,
					  sizeof(GabombaStatue_PathColumns));
	syWordsClearEntry(GabombaStatue_PathRows, sizeof(GabombaStatue_PathRows));
	syWordsClearEntry(GabombaStatue_PathPieces,
					  sizeof(GabombaStatue_PathPieces));
	syWordsClearEntry(GabombaStatue_PathTried, sizeof(GabombaStatue_PathTried));
	syWordsClearEntry(GabombaStatue_PathCellPaths,
					  sizeof(GabombaStatue_PathCellPaths));
	if(spare & 1)
		return -1;
	x = x0;
	cells = (u8 *)((PUZZLE_CELL *)&SceneStateWork);
	one = 1;
	step = 0;
	attempts = 0;
	z = z0;
	while(1) {
		if(++attempts > PATH_ATTEMPTS_MAX)
			return -1;
		{
			s32 offset = (z * CHANNEL_COLUMNS + x) * sizeof(PUZZLE_CELL);
			GabombaStatue_PathPieces[step] =
				((PUZZLE_CELL *)(cells + offset))->kind;
			GabombaStatue_PathCellPaths[step] =
				((PUZZLE_CELL *)(cells + offset))->state;
		}
		GabombaStatue_PathColumns[step] = x;
		GabombaStatue_PathRows[step] = z;
		GabombaStatue_PathHeadings[step] = direction;
		{
			u8 *joins = GabombaStatue_PieceJoins;
			kinds = GabombaStatue_PathPieces;
			GabombaStatue_PathTried[step] = GabombaStatue_PathTried[step] |
											joins[kinds[step]] |
											1 << ((direction + 2) & 3);
		}
		tries = 0;
		while(1) {
			u32 value;
			if(++tries <= PATH_TRIES_PER_STEP) {
				value = syRandom();
				dir = direction;
				if(value >= PATH_KEEP_HEADING_CHANCE)
					dir = syRandomBelow(4);
				tried = GabombaStatue_PathTried[step];
				if((tried >> dir) & one)
					continue;
				{
					s8 *steps = GabombaStatue_PathSteps;
					index = dir * 2;
					nextX = x + steps[index];
					index += 1;
					nextZ = z + steps[index];
				}
				if(nextX < CHANNEL_COLUMNS && nextZ < CHANNEL_ROWS &&
				   !(step == length - 1 && (nextX != endX || nextZ != endZ))) {
					PUZZLE_CELL *cell =
						(PUZZLE_CELL *)(cells +
										(nextZ * CHANNEL_COLUMNS + nextX) *
											sizeof(PUZZLE_CELL));
					if(cell->blocked == 0 &&
					   (used = (cell->state >> bit) & one) == 0 &&
					   cell->kind != PIECE_STOP) {
						tried |= one << dir;
						GabombaStatue_PathTried[step] = tried;
						kinds = GabombaStatue_PathPieces;
						piece = scGabombaStatuePickPiece(direction, dir,
														 kinds[step]);
						{
							s32 offset =
								(z * CHANNEL_COLUMNS + x) * sizeof(PUZZLE_CELL);
							scGabombaStatueWritePiece(
								x, z, piece,
								((PUZZLE_CELL *)(cells + offset))->state |
									one << bit);
						}
						step++;
						x = nextX;
						z = nextZ;
						direction = dir;
						if(step == length)
							goto done;
						GabombaStatue_PathTried[step] = 0;
						break;
					}
				}
				GabombaStatue_PathTried[step] |= 1 << dir;
				if(GabombaStatue_PathTried[step] != HEADING_ALL_TRIED)
					continue;
			}
			GabombaStatue_PathTried[step] = 0;
			if(step == 0)
				return -1;
			step--;
			x = GabombaStatue_PathColumns[step];
			z = GabombaStatue_PathRows[step];
			direction = GabombaStatue_PathHeadings[step];
			scGabombaStatueWritePiece(x, z, GabombaStatue_PathPieces[step],
									  GabombaStatue_PathCellPaths[step]);
			break;
		}
	}
done: {
	s32 offset = (nextZ * CHANNEL_COLUMNS + nextX) * sizeof(PUZZLE_CELL);
	piece = scGabombaStatuePickPiece(dir, last,
									 ((PUZZLE_CELL *)(cells + offset))->kind);
	scGabombaStatueWritePiece(nextX, nextZ, piece,
							  ((PUZZLE_CELL *)(cells + offset))->state |
								  1 << bit);
}
	return 0;
}

extern u8 GabombaStatue_PieceTable[];
extern u8 GabombaStatue_TurnedPieces[];
/* The piece joining the heading a path enters a cell by to the one it leaves */
/* by, merged with the piece already there. */
s32 scGabombaStatuePickPiece(s32 enter, s32 leave, s32 existing) {
	s32 value = GabombaStatue_PieceTable[enter * 4 + leave];

	if(existing == PIECE_EAST_WEST && value == PIECE_NORTH_SOUTH)
		value = PIECE_CROSSING;
	if(existing == PIECE_NORTH_SOUTH && value == PIECE_EAST_WEST)
		value = PIECE_CROSSING;
	if(existing == PIECE_EAST_SOUTH && value == PIECE_WEST_NORTH)
		value = PIECE_EAST_SOUTH_AND_WEST_NORTH;
	if(existing == PIECE_WEST_NORTH && value == PIECE_EAST_SOUTH)
		value = PIECE_EAST_SOUTH_AND_WEST_NORTH;
	if(existing == PIECE_NORTH_EAST && value == PIECE_SOUTH_WEST)
		value = PIECE_SOUTH_WEST_AND_NORTH_EAST;
	if(existing == PIECE_SOUTH_WEST && value == PIECE_NORTH_EAST)
		value = PIECE_SOUTH_WEST_AND_NORTH_EAST;
	return value;
}

/* Puts piece kind, with the paths state, at channel cell (x, z). */
void scGabombaStatueWritePiece(s32 x, s32 z, s32 kind, s32 state) {

	SceneStateWork.cells[z][x].kind = kind;
	SceneStateWork.cells[z][x].state = state;
}

/* Raises the blocks over the room, their sockets open, or sets them in */
/* place: each block's actor, numbered from id, stands over its cell while it */
/* is raised. */
void scGabombaStatueSetBlocksRaised(s32 id, s32 show) {
	SCENE_STATE *state = &SceneStateWork;
	s32 row;
	s32 column;

	state->blocksRaised = show;
	state->firstBlock = id;
	for(row = 0; row < CHANNEL_ROWS; row++) {
		for(column = 0; column < CHANNEL_COLUMNS; column++) {
			{
				PUZZLE_CELL *cell =
					state->cells[0] + (row * CHANNEL_COLUMNS + column);

				if(cell->occupied != 0) {
					ACTOR *actor = fdActorGet(id);
					s32 mapX;
					s32 mapZ;
					s32 kind;

					if(show != 0) {
						actor->x.fixed =
							FIELD_CELL_CENTER(state->originX + column);
						actor->z.fixed =
							FIELD_CELL_CENTER(state->originZ + row);
						fdMapGridCopyCells(BLOCK_NONE, PIECE_SOCKET, 1, 1,
										   state->originX + column,
										   state->originZ + row);
						cell->occupied = BLOCK_RAISED;
					}
					else {
						actor->x.fixed = 0;
						actor->z.fixed = 0;
						state->cells[row][column].occupied = BLOCK_IN_PLACE;
						fdMapGridCopyCells(BLOCK_IN_PLACE, cell->kind, 1, 1,
										   state->originX + column,
										   state->originZ + row);
					}
					id++;
				}
			}
		}
	}
	state->endBlock = id;
}

/* Turns the block at channel cell (x, z) a quarter turn, and returns its new */
/* piece. */
s32 scGabombaStatueTurnBlockPiece(s32 x, s32 z, s32 direction) {

	SceneStateWork.cells[z][x].kind =
		GabombaStatue_TurnedPieces[SceneStateWork.cells[z][x].kind * 2 +
								   direction];
	return SceneStateWork.cells[z][x].kind;
}

/* A step of the Pound on the arrows room's stake: it sinks, and as it ends the
 */
/* blocks rise. */
void scGabombaStatueRaiseBlocks(s32 landed, s32 frame) {
	ACTOR *actor = fdActorGet(ACTOR_ARROWS_STAKE);

	if(frame < POUND_LAND_FRAME)
		actor->scaleY += -FIXED_ONE * 31 / 256;
	if(landed == POUND_STAGE_LANDED) {
		scGabombaStatueSetBlocksRaised(SceneStateWork.firstBlock, 1);
		fdActorSelectAnimation(actor, 6);
		actor->scaleY = FIXED_ONE;
	}
}

/* Pounding a raised block sets it back into its place in the channel as the */
/* fist lands. */
void scGabombaStatuePoundBlock(s32 landed) {
	SCENE_STATE *state;
	ACTOR *actor;
	PUZZLE_CELL *cell;
	s32 x, z;
	u32 row, col;
	s32 found;

	if(landed != POUND_STAGE_LANDED)
		return;
	state = &SceneStateWork;
	actor =
		fdActorGet(((FIELD_EFFECT_STATE *)Ram_HeapSlots->fieldEffState)->tgtId);
	x = FIELD_CELL(actor->x.fixed) - state->originX;
	z = FIELD_CELL(actor->z.fixed) - state->originZ;
	{
		s32 kind = state->cells[z][x].kind;

		cell = &state->cells[z][x];
		cell->occupied = BLOCK_IN_PLACE;
		fdMapGridCopyCells(BLOCK_IN_PLACE, kind, 1, 1, state->originX + x,
						   state->originZ + z);
	}
	actor->x.fixed = 0;
	actor->z.fixed = 0;
	found = 0;
	row = 0;
	for(row = 0; row < CHANNEL_ROWS; row++) {
		for(col = 0; col < CHANNEL_COLUMNS; col++) {
			if(state->cells[row][col].occupied == BLOCK_RAISED) {
				found = 1;
				break;
			}
		}
		if(found)
			return;
	}
	state->blocksRaised = found;
}

/* Sets the channel blocks' animation (BLOCK_ANIM_TURN_*). */
void scGabombaStatueSetBlockAnimation(s32 animation) {
	fdActorSetAnimationById(SceneStateWork.firstBlock, animation);
}

/* Turns every raised block a quarter turn. */
void scGabombaStatueTurnBlocks(s32 direction) {
	SCENE_STATE *state = &SceneStateWork;
	s32 id;

	for(id = state->firstBlock; id < state->endBlock; id++) {
		ACTOR *actor = fdActorGet(id);

		scGabombaStatueTurnBlockPiece(
			FIELD_CELL(actor->x.fixed) - state->originX,
			FIELD_CELL(actor->z.fixed) - state->originZ, direction);
	}
}

/* The frames the raised blocks take to turn a quarter turn. */
enum { BLOCK_TURN_FRAMES = 20 };

/* The arrow panels' cells, which the map keeps at cells (0, 12) to (1, 13): a
 */
/* column for each panel, a row for each of its two states. */
enum {
	RIGHT_ARROW_PANEL_COLUMN = 0,
	LEFT_ARROW_PANEL_COLUMN = 1,
	ARROW_PANEL_RAISED_ROW = 12,
	ARROW_PANEL_PRESSED_ROW = 13
};

/* Stepping onto the left arrow panel presses it, and with the blocks raised */
/* turns them a quarter turn counterclockwise. */
void scGabombaStatuePressLeftArrowPanel(void) {
	SCENE_STATE *state = &SceneStateWork;
	s32 i;

	if(state->leftArrowDown == 0) {
		fdMapGridCopyCellsLayered(LEFT_ARROW_PANEL_COLUMN,
								  ARROW_PANEL_PRESSED_ROW, 1, 1,
								  state->originX + 3, state->originZ - 2);
		if(state->blocksRaised != 0) {
			fdEventPlaySnd(SE_GABOMBA_STATUE_ARROW);
			fdEventStart();
			syFieldEventEndLastingPsynergy(0);
			scGabombaStatueSetBlockAnimation(BLOCK_ANIM_TURN_COUNTERCLOCKWISE);
			for(i = 0; i < BLOCK_TURN_FRAMES; i++) {
				s32 *offset = &state->blocksAng;

				*offset -= DEGtoANG(90) / BLOCK_TURN_FRAMES;
				syWait(1);
			}
			scGabombaStatueTurnBlocks(0);
			fdEventEnd();
			state->blocksAng = 0;
		}
		state->leftArrowDown = 1;
	}
}

/* Stepping off the left arrow panel raises it again. */
void scGabombaStatueReleaseLeftArrowPanel(void) {
	if(SceneStateWork.leftArrowDown != 0) {
		fdMapGridCopyCellsLayered(
			LEFT_ARROW_PANEL_COLUMN, ARROW_PANEL_RAISED_ROW, 1, 1,
			SceneStateWork.originX + 3, SceneStateWork.originZ - 2);
		SceneStateWork.leftArrowDown = 0;
	}
}

/* The right arrow panel: as the left, turning the blocks clockwise. */
void scGabombaStatuePressRightArrowPanel(void) {
	SCENE_STATE *state = &SceneStateWork;
	s32 i;

	if(state->rightArrowDown == 0) {
		fdMapGridCopyCellsLayered(RIGHT_ARROW_PANEL_COLUMN,
								  ARROW_PANEL_PRESSED_ROW, 1, 1,
								  state->originX + 7, state->originZ - 2);
		if(state->blocksRaised != 0) {
			fdEventPlaySnd(SE_GABOMBA_STATUE_ARROW);
			fdEventStart();
			syFieldEventEndLastingPsynergy(0);
			scGabombaStatueSetBlockAnimation(BLOCK_ANIM_TURN_CLOCKWISE);
			for(i = 0; i < BLOCK_TURN_FRAMES; i++) {
				s32 *offset = &state->blocksAng;

				*offset += DEGtoANG(90) / BLOCK_TURN_FRAMES;
				syWait(1);
			}
			scGabombaStatueTurnBlocks(1);
			fdEventEnd();
			state->blocksAng = 0;
		}
		state->rightArrowDown = 1;
	}
}

/* Stepping off the right arrow panel raises it again. */
void scGabombaStatueReleaseRightArrowPanel(void) {
	if(SceneStateWork.rightArrowDown != 0) {
		fdMapGridCopyCellsLayered(
			RIGHT_ARROW_PANEL_COLUMN, ARROW_PANEL_RAISED_ROW, 1, 1,
			SceneStateWork.originX + 7, SceneStateWork.originZ - 2);
		SceneStateWork.rightArrowDown = 0;
	}
}

/* A raised block's slot in the depth list past its position's, as */
/* fdRenderApplyProjectedPlacement queues an actor's sprite 62 past its own and
 */
/* the actor's shadow 60. */
enum { RAISED_BLOCK_DEPTH = 58 };

/* Each frame: draws the raised blocks over the room, turning while an arrow */
/* panel turns them. */
void scGabombaStatueDrawRaisedBlocks(void) {
	s32 affine = 0;
	SCENE_STATE *state = &SceneStateWork;
	SPR_VIEW *oam = state->oam;
	s32 scrollX = syMapStateView(Ram_HeapSlots->mapGrid)[0] & FIELD_PIXEL_MASK;
	s32 scrollZ = syMapStateView(Ram_HeapSlots->mapGrid)[1] & FIELD_PIXEL_MASK;
	u16 tile = VramCacheEntries[state->blockTileEntry].ofs >> 5;
	AFFINE_TRANSFORM effect;
	s32 id;

	if(state->blocksAng != 0) {
		effect.ang = state->blocksAng;
		effect.scaleX = Q_8_8(1);
		effect.scaleY = Q_8_8(1);
		affine = syOamMatrixAllocFromTransform(&effect);
	}
	for(id = state->firstBlock; id < state->endBlock; id++) {
		ACTOR *piece = fdActorGet(id);
		FIXED px = piece->x.fixed;
		if(px != 0) {
			s32 x = px - scrollX;
			FIXED y = piece->y.fixed + toFIXED(16);
			FIXED pz = piece->z.fixed;
			FIXED z = pz - scrollZ;
			s32 sy = z - y;
			s32 depth = FIXEDtoINT(y + z) + RAISED_BLOCK_DEPTH;
			s32 cellX = FIELD_CELL(px) - state->originX;
			s32 cellZ = FIELD_CELL(pz) - state->originZ;
			s32 kind = state->cells[cellZ][cellX].kind;
			u32 *p;
			x = (FIXEDtoINT(x) - 8) & OBJ_X_MASK;
			sy = (FIXEDtoINT(sy) - 8) & OBJ_Y_MASK;
			p = (u32 *)oam;
			*p++ = 0;
			*p++ = sy | OBJ_X(x) | OBJ_SIZE_16X16 | OBJ_256_COLORS;
			*p = (tile + (kind << 3)) | OBJ_PRIORITY(2);
			if(state->blocksAng != 0) {
				oam->affineMode = 1;
				oam->matrix = affine;
			}
			syOamAddSprAtDepthClamped(oam, depth);
			oam++;
		}
	}
}

/* Sets up the channel puzzle on entering the room, or redraws it when */
/* returning. */
s32 scGabombaStatueSetUpChannelPuzzle(s32 x, s32 z, s32 seed, s32 id) {
	SCENE_STATE *state = &SceneStateWork;
	s32 laidSeed;

	if(fdEventGetFlag(FLAG_KEEP_PARTY_POSITION)) {
		scGabombaStatueLoadBlockPictures();
		scGabombaStatueRedrawChannel();
		laidSeed = state->seed;
		syTaskAddFrame(scGabombaStatueDrawRaisedBlocks, TASK_PASS_FRAME);
	}
	else {
		syWordsClearEntry(state, sizeof(SCENE_STATE));
		state->originX = x;
		state->originZ = z;
		scGabombaStatueLoadBlockPictures();
		laidSeed = scGabombaStatueLayChannel(seed);
		scGabombaStatueSetBlocksRaised(id, 0);
		syTaskAddFrame(scGabombaStatueDrawRaisedBlocks, TASK_PASS_FRAME);
	}
	fdActorGet(id)->retainSprCache = ACTOR_COMPOSE_OFF_SCREEN;
	return laidSeed;
}

extern s32 GabombaStatue_ChargeShrinkScript[];
extern u32 GabombaStatue_ChargeBurstScript[];

/* The frames a charge takes to cross a cell of the channel, and the cells a */
/* run of the charges crosses at most. */
enum { CHARGE_STEPS_PER_CELL = 16, CHARGE_CELLS_MAX = 128 };

/* A frame of a charge's run along the channel: it follows the piece under it,
 */
/* bursts where that piece does not go on its way, and shrinks away where it is
 */
/* stopped short of the statue in the middle or reaches a raised block's */
/* socket. The actor id scGabombaStatueRunCharges passes is never read. */
void scGabombaStatueMoveCharge(ACTOR *charge, s32 step, s32 id) {
	s16 *colp = &charge->chargeColumn;
	s32 col = *colp;
	s16 *rowp = &charge->chargeRow;
	s32 row = *rowp;
	u8 *headingp = &charge->chargeHeading;
	s32 heading = *headingp;
	s32 angle = heading * DEGtoANG(90);
	s32 cell = row * CHANNEL_COLUMNS + col;
	SCENE_STATE *state = &SceneStateWork;
	s32 piece = state->cells[row][col].kind;
	s32 goal = state->cells[row][col].occupied;
	s32 key = CHARGE_STEP(heading, piece);
	s32 d = step - MAPGRID_CELL_PIXELS / 2;
	s32 arc = step * (DEGtoANG(90) / CHARGE_STEPS_PER_CELL);
	FIXED s = syMathMulFX(toFIXED(8), sySin(arc));
	FIXED c = syMathMulFX(toFIXED(8), syCos(arc));
	s32 z;
	s32 x;

	x = FIELD_CELL_CENTER(state->originX + col);
	z = FIELD_CELL_CENTER(state->originZ + row);
	if(goal == BLOCK_RAISED) {
		heading = row = col = 0;
		if(charge->chargeState == CHARGE_RUNNING) {
			sySndPlay(SE_DUST_PUFF);
			fdActorStartScript(charge, GabombaStatue_ChargeShrinkScript);
			charge->chargeState = goal;
		}
	}
	else {
		switch(key) {
		case CHARGE_STEP(HEADING_WEST, PIECE_EAST_WEST):
		case CHARGE_STEP(HEADING_WEST, PIECE_CROSSING):
			col--;
			x -= INTtoFIXED(d);
			break;
		case CHARGE_STEP(HEADING_EAST, PIECE_EAST_WEST):
		case CHARGE_STEP(HEADING_EAST, PIECE_CROSSING):
			col++;
			x += INTtoFIXED(d);
			break;
		case CHARGE_STEP(HEADING_SOUTH, PIECE_NORTH_SOUTH):
		case CHARGE_STEP(HEADING_SOUTH, PIECE_CROSSING):
			row++;
			z += INTtoFIXED(d);
			break;
		case CHARGE_STEP(HEADING_NORTH, PIECE_NORTH_SOUTH):
		case CHARGE_STEP(HEADING_NORTH, PIECE_CROSSING):
			row--;
			z -= INTtoFIXED(d);
			break;
		case CHARGE_STEP(HEADING_WEST, PIECE_EAST_SOUTH):
		case CHARGE_STEP(HEADING_WEST, PIECE_EAST_SOUTH_AND_WEST_NORTH):
			row++;
			heading = HEADING_SOUTH;
			x = x - s + toFIXED(8);
			z = z - c + toFIXED(8);
			angle = DEGtoANG(180) - arc;
			break;
		case CHARGE_STEP(HEADING_NORTH, PIECE_SOUTH_WEST):
		case CHARGE_STEP(HEADING_NORTH, PIECE_SOUTH_WEST_AND_NORTH_EAST):
			col--;
			heading = HEADING_WEST;
			x = x + c - toFIXED(8);
			z = z - s + toFIXED(8);
			angle = TRIG_TURN * 3 / 4 - arc;
			break;
		case CHARGE_STEP(HEADING_EAST, PIECE_WEST_NORTH):
		case CHARGE_STEP(HEADING_EAST, PIECE_EAST_SOUTH_AND_WEST_NORTH):
			row--;
			heading = HEADING_NORTH;
			x = x + s - toFIXED(8);
			z = z + c - toFIXED(8);
			angle = -arc;
			break;
		case CHARGE_STEP(HEADING_SOUTH, PIECE_NORTH_EAST):
		case CHARGE_STEP(HEADING_SOUTH, PIECE_SOUTH_WEST_AND_NORTH_EAST):
			col++;
			heading = HEADING_EAST;
			x = x - c + toFIXED(8);
			z = z + s - toFIXED(8);
			angle = DEGtoANG(90) - arc;
			break;
		case CHARGE_STEP(HEADING_WEST, PIECE_NORTH_EAST):
		case CHARGE_STEP(HEADING_WEST, PIECE_SOUTH_WEST_AND_NORTH_EAST):
			row--;
			heading = HEADING_NORTH;
			x = x - s + toFIXED(8);
			z = z + c - toFIXED(8);
			angle = arc + DEGtoANG(180);
			break;
		case CHARGE_STEP(HEADING_SOUTH, PIECE_WEST_NORTH):
		case CHARGE_STEP(HEADING_SOUTH, PIECE_EAST_SOUTH_AND_WEST_NORTH):
			col--;
			heading = HEADING_WEST;
			x = x + c - toFIXED(8);
			z = z + s - toFIXED(8);
			angle = arc + DEGtoANG(90);
			break;
		case CHARGE_STEP(HEADING_EAST, PIECE_SOUTH_WEST):
		case CHARGE_STEP(HEADING_EAST, PIECE_SOUTH_WEST_AND_NORTH_EAST):
			row++;
			heading = HEADING_SOUTH;
			x = x + s - toFIXED(8);
			z = z - c + toFIXED(8);
			angle = arc;
			break;
		case CHARGE_STEP(HEADING_NORTH, PIECE_EAST_SOUTH):
		case CHARGE_STEP(HEADING_NORTH, PIECE_EAST_SOUTH_AND_WEST_NORTH):
			col++;
			heading = HEADING_EAST;
			x = x - c + toFIXED(8);
			z = z - s + toFIXED(8);
			angle = arc + TRIG_TURN * 3 / 4;
			break;
		case CHARGE_STEP(HEADING_EAST, PIECE_SOCKET):
			heading = row = col = 0;
			break;
		case CHARGE_STEP(HEADING_SOUTH, PIECE_SOCKET):
			heading = row = col = 0;
			break;
		case CHARGE_STEP(HEADING_WEST, PIECE_SOCKET):
			heading = row = col = 0;
			break;
		case CHARGE_STEP(HEADING_EAST, PIECE_STOP):
		case CHARGE_STEP(HEADING_SOUTH, PIECE_STOP):
		case CHARGE_STEP(HEADING_WEST, PIECE_STOP):
		case CHARGE_STEP(HEADING_NORTH, PIECE_STOP):
			if(charge->chargeState == CHARGE_RUNNING) {
				charge->x.fixed = x;
				charge->y.fixed = 0;
				charge->z.fixed = z;
				charge->facing = angle;
				if(FIELD_CELL(x) ==
					   CHANNEL_ORIGIN_COLUMN + CHANNEL_MIDDLE_STATUE_COLUMN &&
				   FIELD_CELL(z) ==
					   CHANNEL_ORIGIN_ROW + CHANNEL_MIDDLE_STATUE_ROW) {
					charge->chargeState = CHARGE_ARRIVED;
				}
				else {
					sySndPlay(SE_DUST_PUFF);
					fdActorStartScript(charge,
									   GabombaStatue_ChargeShrinkScript);
					charge->chargeState = CHARGE_LEAVING;
				}
			}
			break;
		case CHARGE_STEP(HEADING_NORTH, PIECE_SOCKET):
			heading = row = col = 0;
			break;
		default:
			heading = row = col = 0;
			if(charge->chargeState == CHARGE_RUNNING) {
				sySndPlay(SE_WATER);
				fdActorStartScript(charge, GabombaStatue_ChargeBurstScript);
				charge->chargeState = CHARGE_LEAVING;
			}
			break;
		}
	}
	if(step == CHARGE_STEPS_PER_CELL - 1) {
		*colp = col;
		*rowp = row;
		*headingp = heading;
	}
	if(charge->chargeState == CHARGE_RUNNING) {
		charge->y.fixed = 0;
		charge->x.fixed = x;
		charge->z.fixed = z;
		charge->facing = angle;
		/* sceneWord counts the frames the charge has run. */
		charge->sceneWord++;
	}
}

/* Runs the four charges from the orb statues along the channel until each has
 */
/* stopped and, with check, tells how they reached the statue in the middle. */
s32 scGabombaStatueRunCharges(s32 first, s32 check) {
	ACTOR *actor;
	s32 second = first + 1;
	s32 frame, tries, id;
	s32 done;
	s32 result;

	actor = fdActorGet(first);
	actor->chargeColumn = CHANNEL_BLUE_STATUE_COLUMN - 1;
	actor->chargeRow = CHANNEL_STATUES_ROW;
	actor->chargeHeading = HEADING_WEST;
	actor->chargeState = CHARGE_RUNNING;
	actor->sceneWord = 0;
#ifdef UBFIX
	fdActorStartIdle(actor);
#else
	/* UB: called without the actor it starts, which it finds in r0 only because
	 */
	/* fdActorGet left it there. */
	fdActorStartIdle();
#endif
	fdActorSelectAnimation(actor, 2);
	id = second;
	actor = fdActorGet(id++);
	actor->chargeColumn = CHANNEL_BLUE_STATUE_COLUMN + 1;
	actor->chargeRow = CHANNEL_STATUES_ROW;
	actor->chargeHeading = HEADING_EAST;
	actor->chargeState = CHARGE_RUNNING;
	actor->sceneWord = 0;
	fdActorStartIdle(actor);
	fdActorSelectAnimation(actor, 2);
	actor = fdActorGet(id++);
	actor->chargeColumn = CHANNEL_RED_STATUE_COLUMN - 1;
	actor->chargeRow = CHANNEL_STATUES_ROW;
	actor->chargeHeading = HEADING_WEST;
	actor->chargeState = CHARGE_RUNNING;
	actor->sceneWord = 0;
	fdActorStartIdle(actor);
	fdActorSelectAnimation(actor, 2);
	actor = fdActorGet(id);
	actor->chargeColumn = CHANNEL_RED_STATUE_COLUMN + 1;
	actor->chargeRow = CHANNEL_STATUES_ROW;
	actor->chargeHeading = HEADING_EAST;
	actor->chargeState = CHARGE_RUNNING;
	actor->sceneWord = 0;
	fdActorStartIdle(actor);
	fdActorSelectAnimation(actor, 2);
	for(tries = 0; tries < CHARGE_CELLS_MAX; tries++) {
		done = 1;
		for(frame = 0; frame < CHARGE_STEPS_PER_CELL; frame++) {
			for(id = first; id < first + 4; id++) {
				actor = fdActorGet(id);
				if(actor->chargeState != CHARGE_STOPPED &&
				   actor->chargeState != CHARGE_ARRIVED)
					done = 0;
				scGabombaStatueMoveCharge(actor, frame, id);
			}
			syWait(1);
		}
		if(done)
			break;
	}
	syWait(20);
	result = CHARGE_ARRIVAL_NONE;
	if(check) {
		/* A charge still on the map has arrived; sceneWord is its running
		 * frames, */
		/* the sooner of each orb's two. */
		u32 blue = 0;
		u32 red = 0;
		{
			ACTOR *r = fdActorGet(first);
			if(r->x.fixed != 0)
				blue = r->sceneWord;
		}
		{
			ACTOR *r = fdActorGet(second);
			if(r->x.fixed != 0) {
				u32 value = r->sceneWord;
				if(blue == 0 || blue > value)
					blue = value;
			}
		}
		{
			ACTOR *r = fdActorGet(first + 2);
			if(r->x.fixed != 0)
				red = r->sceneWord;
		}
		{
			ACTOR *r = fdActorGet(first + 3);
			if(r->x.fixed != 0) {
				u32 value = r->sceneWord;
				if(red == 0 || red > value)
					red = value;
			}
		}
		if(blue != 0) {
			result = CHARGE_ARRIVAL_BLUE_FIRST;
			if(red != 0) {
				result = CHARGE_ARRIVAL_TOGETHER;
				if(blue != red) {
					if(blue < red) {
						fdEventSetFlag(FLAG_GABOMBA_CHARGES_REACHED_APART);
						result = CHARGE_ARRIVAL_BLUE_FIRST;
					}
					else {
						fdEventSetFlag(FLAG_GABOMBA_CHARGES_REACHED_APART);
						result = CHARGE_ARRIVAL_RED_FIRST;
					}
				}
			}
		}
		else {
			if(red != 0)
				return CHARGE_ARRIVAL_RED_FIRST;
			return CHARGE_ARRIVAL_NONE;
		}
	}
	return result;
}

/* Finds the cell where the circuit is broken: a block with no piece on it. */
void scGabombaStatueFindBrokenCell(u32 *x_out, u32 *z_out) {
	u32 z;
	u32 x;

	for(z = 0; z < CHANNEL_ROWS; z++) {
		for(x = 0; x < CHANNEL_COLUMNS; x++) {
			if(SceneStateWork.cells[z][x].kind == PIECE_STOP &&
			   !(x == CHANNEL_MIDDLE_STATUE_COLUMN &&
				 z == CHANNEL_MIDDLE_STATUE_ROW) &&
			   !(z == CHANNEL_STATUES_ROW &&
				 (x == CHANNEL_BLUE_STATUE_COLUMN ||
				  x == CHANNEL_RED_STATUE_COLUMN))) {
				*x_out = x;
				*z_out = z;
				return;
			}
		}
	}
}

/* The clock's digit sprite sheet: the pictures of 0 to 9 and the colon, 8 by */
/* 16 pixels each, then the five shown. */
enum {
	CLOCK_PICTURE_BYTES = 64,
	CLOCK_SHEET_PICTURES = 11,
	CLOCK_SHOWN_PICTURES = 5
};

/* The depth the clock is drawn at, in front of the room's actors. */
#define CLOCK_DEPTH 236

/* The offering's countdown clock, MM:SS:T drawn as sprites: the digits that */
/* change shrink and grow back each second as it ticks. */
typedef struct CLOCK_WORK {
	u8 *tiles;	/* the digit sprite sheet */
	u8 *digits; /* the five digit pictures, as loaded */
	u8 old[8];
	u8 now[8];
	u8 digit[8];
	SPR_VIEW oam[8];
	s32 tileEntry;
	s32 tmr;
	u8 ringing;
	u8 mode;
} CLOCK_WORK;

/* The clock's state, kept 256 bytes into the inventory snapshot's spare room.
 */
#define CLOCK_STATE                                                            \
	((CLOCK_WORK *)(InventorySnapshot + INVENTORY_SNAPSHOT_SPARE + 256))

/* Converts the remaining frames into minutes, seconds and tenths */
/* (`MM:SS:T`). */
void scGabombaStatueFormatClock(s32 time, u8 *text) {
	if(time < 0)
		time = 0;
	*text = time / (FRAMES_PER_SECOND * 60 * 10) + '0';
	time %= (FRAMES_PER_SECOND * 60 * 10);
	*++text = time / (FRAMES_PER_SECOND * 60) + '0';
	time %= (FRAMES_PER_SECOND * 60);
	*++text = ':';
	*++text = time / (FRAMES_PER_SECOND * 10) + '0';
	time %= (FRAMES_PER_SECOND * 10);
	*++text = time / FRAMES_PER_SECOND + '0';
	time %= FRAMES_PER_SECOND;
	*++text = ':';
	*++text = time / (FRAMES_PER_SECOND / 10) + '0';
	text[1] = 0;
}

/* Plays the clock's tick each second and returns the scale of the changing */
/* digits, which shrink out of sight and grow back over the last third of each
 */
/* second, as the clock counts down to its tick. */
s32 scGabombaStatueTickClock(s32 time) {
	CLOCK_WORK *clock = CLOCK_STATE;
	s32 frame = time % FRAMES_PER_SECOND;
	s32 scale = Q_8_8(2);
	u8 *ringing = &clock->ringing;

	if(frame != 0) {
		*ringing = 1;
	}
	else if(*ringing) {
		if(clock->tmr != 0)
			fdEventPlaySnd(SE_COLOSSO_COUNTDOWN);
		*ringing = frame;
	}
	if(frame < FRAMES_PER_SECOND / 3) {
		FIXED cosine = syCos(frame * (TRIG_TURN / 2) / 10);
		s32 (*mul)(s32 a, s32 b);
		s32 half = Q_8_8(0.5);

		mul = syMulFX;
		scale = mul(half << 2, cosine);
	}
	if(scale <= Q_8_8(1) / 16)
		scale = Q_8_8(1) / 16;
	return scale;
}

/* Draws the clock's minutes and seconds, doubled in size, the colon and each */
/* digit about to change taking the ticking scale. */
void scGabombaStatueDrawClock(void) {
	CLOCK_WORK *clock = CLOCK_STATE;
	u8 *digit = clock->digit;
	u8 *old = clock->tiles;
	u8 *now = clock->digits;
	u32 tile = VramCacheEntries[clock->tileEntry].ofs >> 5;
	SPR_VIEW *oam = clock->oam;
	s32 *timer = &clock->tmr;
	AFFINE_TRANSFORM effect;
	s32 plain, scaled, i;
	s32 scale;
	u32 *p;

	/* The digits shown move on 10 frames before each second runs out, while */
	/* those changing have shrunk out of sight (scGabombaStatueTickClock); the
	 */
	/* ones that differ from the time a second before are the ones that shrink.
	 */
	scGabombaStatueFormatClock(*timer + FRAMES_PER_SECOND - 10, digit);
	scGabombaStatueFormatClock(*timer + FRAMES_PER_SECOND, clock->old);
	scGabombaStatueFormatClock(*timer, clock->now);
	/* old and now walk the sprite sheet and the pictures here, the digit lists
	 */
	/* below. */
	for(i = 0; i < CLOCK_SHOWN_PICTURES; i++) {
		syWordsCopyEntry(
			now, old + *digit * CLOCK_PICTURE_BYTES - '0' * CLOCK_PICTURE_BYTES,
			CLOCK_PICTURE_BYTES);
		now += CLOCK_PICTURE_BYTES;
		digit++;
	}
	mnVramLoad(clock->tileEntry, CLOCK_SHOWN_PICTURES * CLOCK_PICTURE_BYTES,
			   clock->digits);
	effect.ang = 0;
	effect.scaleX = Q_8_8(2);
	effect.scaleY = Q_8_8(2);
	plain = syOamMatrixAllocFromTransform(&effect);
	scale = scGabombaStatueTickClock(*timer);
	effect.ang = 0;
	effect.scaleX = scale;
	effect.scaleY = scale;
	scaled = syOamMatrixAllocFromTransform(&effect);
	old = clock->old;
	now = clock->now;
	for(i = 0; i < CLOCK_SHOWN_PICTURES; i++, tile += 2, oam++) {
		p = (u32 *)oam;
		*p++ = 0;
		*p++ = OBJ_X(i * 16) | OBJ_SIZE_8X16;
		*p = tile | OBJ_PRIORITY(1);
		oam->affineMode = 3;
		if(*old++ != *now++ || i == 2)
			oam->matrix = scaled;
		else
			oam->matrix = plain;
		syOamAddSprAtDepthClamped(oam, CLOCK_DEPTH);
	}
}

/* Counts the clock down while no menu is open, and draws it. */
void scGabombaStatueCountDownShowingClock(void) {
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	CLOCK_WORK *clock = CLOCK_STATE;

	if(state->psynergy == 0 && state->fieldMenu == 0 && state->pauseMenu == 0 &&
	   clock->tmr > 0)
		clock->tmr--;
	scGabombaStatueDrawClock();
}

/* Counts the clock down while no menu is open. */
void scGabombaStatueCountDown(void) {
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	CLOCK_WORK *clock = CLOCK_STATE;

	if(state->psynergy == 0 && state->fieldMenu == 0 && state->pauseMenu == 0 &&
	   clock->tmr > 0)
		clock->tmr--;
}

/* Starts the clock at one minute, shown or hidden. */
void scGabombaStatueStartClock(s32 mode) {
	CLOCK_WORK *clock = CLOCK_STATE;
	u8 *tiles;

	if(!fdEventGetFlag(FLAG_KEEP_PARTY_POSITION)) {
		syWordsClearEntry(clock, sizeof(CLOCK_WORK));
		clock->tmr = FRAMES_PER_SECOND * 60;
		clock->ringing = 1;
	}
	tiles = syHeapAllocEwram(1024);
	syDecompressTaggedLz(syResourceGetTableEntry(RESOURCE_CLOCK_DIGITS), tiles);
	clock->tiles = tiles;
	clock->digits = tiles + CLOCK_SHEET_PICTURES * CLOCK_PICTURE_BYTES;
	clock->tileEntry = syVramCacheFindFreeEntry();
	mnVramLoad(clock->tileEntry, CLOCK_SHOWN_PICTURES * CLOCK_PICTURE_BYTES,
			   NULL);
	clock->mode = mode;
	if(mode)
		syTaskAddFrame(scGabombaStatueCountDownShowingClock, TASK_PASS_FRAME);
	else
		syTaskAddFrame(scGabombaStatueCountDown, TASK_PASS_FRAME);
}

/* Stops the clock's countdown. */
void scGabombaStatueStopClock(void) {
	CLOCK_WORK *clock = CLOCK_STATE;

	if(clock->mode)
		syFieldTaskRemove(scGabombaStatueCountDownShowingClock);
	else
		syFieldTaskRemove(scGabombaStatueCountDown);
}

/* Starts the clock counting down from a minute. */
void scGabombaStatueRestartClock(void) {
	CLOCK_WORK *clock = CLOCK_STATE;

	if(clock->mode)
		syTaskAddFrame(scGabombaStatueCountDownShowingClock, TASK_PASS_FRAME);
	else
		syTaskAddFrame(scGabombaStatueCountDown, TASK_PASS_FRAME);
	clock->tmr = FRAMES_PER_SECOND * 60;
}

/* The frames left on the clock. */
s32 scGabombaStatueGetClock(void) { return CLOCK_STATE->tmr; }

/* Sets the clock back to a minute. */
void scGabombaStatueResetClock(void) {
	CLOCK_STATE->tmr = FRAMES_PER_SECOND * 60;
}

/* Puts out the stakes of a table of rows (actor, kind, flag), ended by -1, */
/* each standing or driven down as its flag says. */
void scGabombaStatuePlaceSwitchActors(const u16 *entry) {
	s32 value;

	for(; (value = *entry) != (u16)-1; entry += 2) {
		u16 id = *entry++;
		ACTOR *actor = fdActorGet(id);
		s16 kind = entry[0];
		s16 flag = entry[1];

		actor->collisionFlg &= ~ACTOR_SOLID;
		fdActorSetSprFlags(actor, 0);
		fdActorSetCellOpen(id, (u16)kind, (u16)flag, actor->layer);
	}
}

/* Finds the current target's table row and shrinks it. */
#include "shared/pound_sink_target.h"

#include "shared/actor_set_cell_open.h"

/* clang-format off */
#include "shared/field_effect_duration_scripts.c"

/* clang-format on */

/* The shapes scGabombaStatueDrawCogCells writes over the map's from */
/* COG_SHAPE_FIRST. */
u32 GabombaStatue_CogShapes[] = {
	[COG_SHAPE_GREEN_COG - COG_SHAPE_FIRST] = MAP_SHAPE(7, 0, 2, 0),
	[COG_SHAPE_TOP - COG_SHAPE_FIRST] = MAP_SHAPE(0, 2, 0, 0),
	[COG_SHAPE_NORTHWEST - COG_SHAPE_FIRST] = MAP_SHAPE(3, 0, 2, 0),
	[COG_SHAPE_NORTHEAST - COG_SHAPE_FIRST] = MAP_SHAPE(4, 0, 2, 0),
	[COG_SHAPE_SOUTHWEST - COG_SHAPE_FIRST] = MAP_SHAPE(4, 2, 0, 0),
	[COG_SHAPE_SOUTHEAST - COG_SHAPE_FIRST] = MAP_SHAPE(3, 2, 0, 0),
	[COG_SHAPE_WEST - COG_SHAPE_FIRST] = MAP_SHAPE(1, 1, 2, 0),
	[COG_SHAPE_EAST - COG_SHAPE_FIRST] = MAP_SHAPE(1, 2, 1, 0),
	[COG_SHAPE_NORTH - COG_SHAPE_FIRST] = MAP_SHAPE(2, 1, 2, 0),
	[COG_SHAPE_SOUTH - COG_SHAPE_FIRST] = MAP_SHAPE(2, 2, 1, 0),
	[COG_SHAPE_PINK_DRUM_SOUTH_END - COG_SHAPE_FIRST] = MAP_SHAPE(0, 248, 0, 0),
	[COG_SHAPE_END - COG_SHAPE_FIRST] = MAP_SHAPE(0, 0, 0, 0),
};

/* Each cog's animation by the cogs' direction + 1: turning back, stopped and */
/* turning. */
s8 GabombaStatue_CogAnimations[] = {
	3,
	1,
	2,
	0,
};

u32 GabombaStatue_PinionReleaseScript[] = {
	SC_RELEASE,
};

/* clang-format off */
/* Unused: GabombaStatue_TurnedPieces from its second byte to its 21st. */
LATE_DATA u8 GabombaStatue_UnusedTurnedPieces[] = {
    PIECE_NONE, PIECE_EAST_WEST, PIECE_EAST_WEST, PIECE_NORTH_SOUTH,
    PIECE_NORTH_SOUTH, PIECE_CROSSING, PIECE_CROSSING, PIECE_NORTH_EAST,
    PIECE_SOUTH_WEST, PIECE_EAST_SOUTH, PIECE_WEST_NORTH, PIECE_SOUTH_WEST,
    PIECE_NORTH_EAST, PIECE_WEST_NORTH, PIECE_EAST_SOUTH,
    PIECE_SOUTH_WEST_AND_NORTH_EAST, PIECE_SOUTH_WEST_AND_NORTH_EAST,
    PIECE_EAST_SOUTH_AND_WEST_NORTH, PIECE_EAST_SOUTH_AND_WEST_NORTH,
    PIECE_STOP, PIECE_STOP,
};

/* The step along each heading as (column, row). */
LATE_DATA s8 GabombaStatue_PathSteps[] = {
    1, 0, 0, 1, -1, 0, 0, -1,
};

/* The headings each piece joins, a bit each; a crossing, two curves and a */
/* stop let no new path in. */
LATE_DATA u8 GabombaStatue_PieceJoins[] = {
    [PIECE_NONE] = 0,
    [PIECE_NORTH_SOUTH] = HEADING_BIT(HEADING_SOUTH) | HEADING_BIT(HEADING_NORTH),
    [PIECE_EAST_WEST] = HEADING_BIT(HEADING_EAST) | HEADING_BIT(HEADING_WEST),
    [PIECE_CROSSING] = HEADING_ALL_TRIED,
    [PIECE_EAST_SOUTH] = HEADING_BIT(HEADING_EAST) | HEADING_BIT(HEADING_SOUTH),
    [PIECE_SOUTH_WEST] = HEADING_BIT(HEADING_SOUTH) | HEADING_BIT(HEADING_WEST),
    [PIECE_WEST_NORTH] = HEADING_BIT(HEADING_WEST) | HEADING_BIT(HEADING_NORTH),
    [PIECE_NORTH_EAST] = HEADING_BIT(HEADING_EAST) | HEADING_BIT(HEADING_NORTH),
    [PIECE_EAST_SOUTH_AND_WEST_NORTH] = HEADING_ALL_TRIED,
    [PIECE_SOUTH_WEST_AND_NORTH_EAST] = HEADING_ALL_TRIED,
    [PIECE_STOP] = HEADING_ALL_TRIED,
};

/* The piece joining each heading a path enters a cell by (a row of four) to */
/* each it leaves by. */
LATE_DATA u8 GabombaStatue_PieceTable[] = {
    /* Heading east, in by the west side; out east, south, back west, north. */
    PIECE_EAST_WEST, PIECE_SOUTH_WEST, PIECE_STOP, PIECE_WEST_NORTH,
    /* Heading south, in by the north side. */
    PIECE_NORTH_EAST, PIECE_NORTH_SOUTH, PIECE_WEST_NORTH, PIECE_STOP,
    /* Heading west, in by the east side. */
    PIECE_STOP, PIECE_EAST_SOUTH, PIECE_EAST_WEST, PIECE_NORTH_EAST,
    /* Heading north, in by the south side. */
    PIECE_EAST_SOUTH, PIECE_STOP, PIECE_SOUTH_WEST, PIECE_NORTH_SOUTH,
};

LATE_DATA const s32 *FieldEffect_DurationScripts[] = {
    FieldEffect_ShortScript,
    FieldEffect_MediumScript,
    FieldEffect_LongScript,
};

/* The way the cogs turn, which each tooth an actor rides steps by. */
LATE_DATA s32 GabombaStatue_CogDirection = 0;

LATE_DATA u32 GabombaStatue_LeaderFocusScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(472, 0, 184),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

LATE_DATA u32 GabombaStatue_JennaFocusScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(488, 0, 176),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(1),
    SC_END,
};

LATE_DATA u32 GabombaStatue_PiersFocusScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(456, 0, 176),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(1),
    SC_END,
};

LATE_DATA u32 GabombaStatue_KradenFocusScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(464, 0, 208),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_SOUTH_SOUTHWEST),
    SC_WAIT(1),
    SC_END,
};

LATE_DATA u32 GabombaStatue_KradenPointsScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(432, 0, 286),
    SC_SYNC,
    SC_MOVE(432, 0, 312),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_SOUTH_SOUTHEAST),
    SC_WAIT(1),
    SC_END,
};

LATE_DATA u32 GabombaStatue_LookAtChannelScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(348, 0, 312),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_END,
};

LATE_DATA s32 GabombaStatue_LookNorthAndEastScript[] = {
    SC_SET(SW_TIMER, FACING_NORTH),
    SC_SET(SW_USER_VALUE, FACING_NORTHEAST),
    SC_SET(SW_SCENE_BYTE, 0),
    SC_CALL(scGabombaStatueLookAround),
};

LATE_DATA s32 GabombaStatue_LookNorthAndWestScript[] = {
    SC_SET(SW_TIMER, FACING_NORTH),
    SC_SET(SW_USER_VALUE, FACING_NORTHWEST),
    SC_SET(SW_SCENE_BYTE, 0),
    SC_CALL(scGabombaStatueLookAround),
};

LATE_DATA u32 GabombaStatue_JennaWatchesScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(472, 0, 308),
    SC_SYNC,
    SC_MOVE(440, 0, 312),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(1),
    SC_END,
};

LATE_DATA u32 GabombaStatue_AkafubuComesUpScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(472, 0, 308),
    SC_SYNC,
    SC_MOVE(460, 0, 312),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_SOUTH_SOUTHWEST),
    SC_WAIT(1),
    SC_END,
};

/* The stakes beside the flashing cog: actor, its look, and the flag Pounding */
/* it sets. */
LATE_DATA s16 GabombaStatue_Stakes[] = {
    ACTOR_FIRST_STAKE, STAKE_MOSSY_STONE, FLAG_GABOMBA_FIRST_STAKE_POUNDED, ACTOR_SECOND_STAKE, STAKE_MOSSY_STONE, FLAG_GABOMBA_SECOND_STAKE_POUNDED, -1, 0,
};

/* The random seeds a game's channel is laid from, one picked by the leader's */
/* name (scGabombaStatueGetPuzzleSeed). */
LATE_DATA u32 GabombaStatue_ChannelSeeds[] = {
    36, 122, 201, 312, 431, 504,
    619, 712, 823, 908, 1054, 1121,
    1207, 1311, 1401, 1555,
};

LATE_DATA SCENE_ENTRANCE SceneEntrances[] = {
    {0, CONDITION_ALWAYS, 392, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

LATE_DATA SCENE_REGION GabombaStatue_PinionExitRegions[] = {
    {784, 16, 80, 800, 32, 96, CONDITION_ALWAYS, GABOMBA_STATUE_PINION_TO_KIBOMBO_STATUE_GROUNDS},
    {80, 48, 368, 96, 64, 384, CONDITION_ALWAYS, GABOMBA_STATUE_PINION_TO_GABOMBA_STATUE_BONE_ARMLET_CHEST},
    {SCENE_TABLE_END},
};

LATE_DATA SCENE_REGION GabombaStatue_BoneArmletChestExitRegions[] = {
    {464, 16, 240, 480, 32, 256, CONDITION_ALWAYS, GABOMBA_STATUE_BONE_ARMLET_CHEST_TO_GABOMBA_STATUE_ELIXIR_CHEST},
    {SCENE_TABLE_END},
};

LATE_DATA SCENE_REGION GabombaStatue_ElixirChestExitRegions[] = {
    {528, 16, 64, 544, 32, 80, CONDITION_ALWAYS, GABOMBA_STATUE_ELIXIR_CHEST_TO_GABOMBA_STATUE_STEEL_NORTHEAST},
    {96, 16, 160, 112, 32, 176, CONDITION_ALWAYS, GABOMBA_STATUE_ELIXIR_CHEST_TO_GABOMBA_STATUE_STEEL_WEST},
    {672, 16, 160, 688, 32, 176, CONDITION_ALWAYS, GABOMBA_STATUE_ELIXIR_CHEST_TO_GABOMBA_STATUE_STEEL_EAST},
    {SCENE_TABLE_END},
};

LATE_DATA SCENE_REGION GabombaStatue_SteelExitRegions[] = {
    {384, 16, 112, 400, 32, 128, CONDITION_ALWAYS, GABOMBA_STATUE_STEEL_TO_GABOMBA_STATUE_MOUSE_NORTHWEST},
    {96, 16, 144, 112, 32, 160, CONDITION_ALWAYS, GABOMBA_STATUE_STEEL_TO_GABOMBA_STATUE_MOUSE_WEST},
    {672, 16, 144, 688, 32, 160, CONDITION_ALWAYS, GABOMBA_STATUE_STEEL_TO_GABOMBA_STATUE_MOUSE_EAST},
    {SCENE_TABLE_END},
};

LATE_DATA SCENE_REGION GabombaStatue_MouseExitRegions[] = {
    {432, 16, 448, 448, 32, 464, CONDITION_ALWAYS, GABOMBA_STATUE_MOUSE_TO_GABOMBA_STATUE_COGS},
    {SCENE_TABLE_END},
};

LATE_DATA SCENE_REGION GabombaStatue_CogsExitRegions[] = {
    {304, 16, 144, 320, 32, 160, CONDITION_ALWAYS, GABOMBA_STATUE_COGS_TO_GABOMBA_STATUE_CHAMBER_IDOL},
    {SCENE_TABLE_END},
};

LATE_DATA u32 SceneExits[] = {
    SCENE_EXITS(GabombaStatuePinion),
    SCENE_EXIT_UNLESS(GABOMBA_STATUE_PINION_TO_KIBOMBO_STATUE_GROUNDS, KibomboStatueGrounds, KIBOMBO_STATUE_GROUNDS_FROM_GABOMBA_STATUE_PINION, FLAG_KIBOMBO_AT_CEREMONY),
    SCENE_EXIT_UNLESS(GABOMBA_STATUE_PINION_TO_KIBOMBO_STATUE_GROUNDS, KibomboStatueGroundsCeremony, KIBOMBO_STATUE_GROUNDS_CEREMONY_FROM_GABOMBA_STATUE_PINION, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GABOMBA_STATUE_PINION_TO_GABOMBA_STATUE_BONE_ARMLET_CHEST, GabombaStatueBoneArmletChest, GABOMBA_STATUE_BONE_ARMLET_CHEST_FROM_GABOMBA_STATUE_PINION, EXIT_FLAG_NONE),
    SCENE_EXITS(GabombaStatueBoneArmletChest),
    SCENE_EXIT(GABOMBA_STATUE_BONE_ARMLET_CHEST_TO_GABOMBA_STATUE_PINION, GabombaStatuePinion, GABOMBA_STATUE_PINION_FROM_GABOMBA_STATUE_BONE_ARMLET_CHEST),
    SCENE_EXIT(GABOMBA_STATUE_BONE_ARMLET_CHEST_TO_GABOMBA_STATUE_ELIXIR_CHEST, GabombaStatueElixirChest, GABOMBA_STATUE_ELIXIR_CHEST_FROM_GABOMBA_STATUE_BONE_ARMLET_CHEST),
    SCENE_EXITS(GabombaStatueElixirChest),
    SCENE_EXIT(GABOMBA_STATUE_ELIXIR_CHEST_TO_GABOMBA_STATUE_BONE_ARMLET_CHEST, GabombaStatueBoneArmletChest, GABOMBA_STATUE_BONE_ARMLET_CHEST_FROM_GABOMBA_STATUE_ELIXIR_CHEST),
    SCENE_EXIT(GABOMBA_STATUE_ELIXIR_CHEST_TO_GABOMBA_STATUE_STEEL_NORTHEAST, GabombaStatueSteel, GABOMBA_STATUE_STEEL_FROM_GABOMBA_STATUE_ELIXIR_CHEST_NORTHEAST),
    SCENE_EXIT(GABOMBA_STATUE_ELIXIR_CHEST_TO_GABOMBA_STATUE_STEEL_WEST, GabombaStatueSteel, GABOMBA_STATUE_STEEL_FROM_GABOMBA_STATUE_ELIXIR_CHEST_WEST),
    SCENE_EXIT(GABOMBA_STATUE_ELIXIR_CHEST_TO_GABOMBA_STATUE_STEEL_EAST, GabombaStatueSteel, GABOMBA_STATUE_STEEL_FROM_GABOMBA_STATUE_ELIXIR_CHEST_EAST),
    SCENE_EXITS(GabombaStatueSteel),
    SCENE_EXIT(GABOMBA_STATUE_STEEL_TO_GABOMBA_STATUE_ELIXIR_CHEST_NORTHEAST, GabombaStatueElixirChest, GABOMBA_STATUE_ELIXIR_CHEST_FROM_GABOMBA_STATUE_STEEL_NORTHEAST),
    SCENE_EXIT(GABOMBA_STATUE_STEEL_TO_GABOMBA_STATUE_ELIXIR_CHEST_WEST, GabombaStatueElixirChest, GABOMBA_STATUE_ELIXIR_CHEST_FROM_GABOMBA_STATUE_STEEL_WEST),
    SCENE_EXIT(GABOMBA_STATUE_STEEL_TO_GABOMBA_STATUE_ELIXIR_CHEST_EAST, GabombaStatueElixirChest, GABOMBA_STATUE_ELIXIR_CHEST_FROM_GABOMBA_STATUE_STEEL_EAST),
    SCENE_EXIT(GABOMBA_STATUE_STEEL_TO_GABOMBA_STATUE_MOUSE_NORTHWEST, GabombaStatueMouse, GABOMBA_STATUE_MOUSE_FROM_GABOMBA_STATUE_STEEL_NORTHWEST),
    SCENE_EXIT(GABOMBA_STATUE_STEEL_TO_GABOMBA_STATUE_MOUSE_WEST, GabombaStatueMouse, GABOMBA_STATUE_MOUSE_FROM_GABOMBA_STATUE_STEEL_WEST),
    SCENE_EXIT(GABOMBA_STATUE_STEEL_TO_GABOMBA_STATUE_MOUSE_EAST, GabombaStatueMouse, GABOMBA_STATUE_MOUSE_FROM_GABOMBA_STATUE_STEEL_EAST),
    SCENE_EXITS(GabombaStatueMouse),
    SCENE_EXIT(GABOMBA_STATUE_MOUSE_TO_GABOMBA_STATUE_STEEL_NORTHWEST, GabombaStatueSteel, GABOMBA_STATUE_STEEL_FROM_GABOMBA_STATUE_MOUSE_NORTHWEST),
    SCENE_EXIT(GABOMBA_STATUE_MOUSE_TO_GABOMBA_STATUE_STEEL_WEST, GabombaStatueSteel, GABOMBA_STATUE_STEEL_FROM_GABOMBA_STATUE_MOUSE_WEST),
    SCENE_EXIT(GABOMBA_STATUE_MOUSE_TO_GABOMBA_STATUE_STEEL_EAST, GabombaStatueSteel, GABOMBA_STATUE_STEEL_FROM_GABOMBA_STATUE_MOUSE_EAST),
    SCENE_EXIT(GABOMBA_STATUE_MOUSE_TO_GABOMBA_STATUE_COGS, GabombaStatueCogs, GABOMBA_STATUE_COGS_FROM_GABOMBA_STATUE_MOUSE),
    SCENE_EXIT(GABOMBA_STATUE_MOUSE_TO_GABOMBA_STATUE_STEEL_SOUTHEAST_A, GabombaStatueSteel, GABOMBA_STATUE_STEEL_FROM_GABOMBA_STATUE_MOUSE_SOUTHEAST_A),
    SCENE_EXIT(GABOMBA_STATUE_MOUSE_TO_GABOMBA_STATUE_STEEL_SOUTHEAST_B, GabombaStatueSteel, GABOMBA_STATUE_STEEL_FROM_GABOMBA_STATUE_MOUSE_SOUTHEAST_B),
    SCENE_EXITS(GabombaStatueCogs),
    SCENE_EXIT(GABOMBA_STATUE_COGS_TO_GABOMBA_STATUE_MOUSE, GabombaStatueMouse, GABOMBA_STATUE_MOUSE_FROM_GABOMBA_STATUE_COGS),
    SCENE_EXIT(GABOMBA_STATUE_COGS_TO_GABOMBA_STATUE_CHAMBER_IDOL, GabombaStatueChamberIdol, GABOMBA_STATUE_CHAMBER_IDOL_FROM_GABOMBA_STATUE_COGS),
    SCENE_EXITS(GabombaStatueArrows),
    SCENE_EXIT(GABOMBA_STATUE_ARROWS_TO_GABOMBA_STATUE_CHAMBER_IDOL_EAST, GabombaStatueChamberIdol, GABOMBA_STATUE_CHAMBER_IDOL_FROM_GABOMBA_STATUE_ARROWS_EAST),
    SCENE_EXIT(GABOMBA_STATUE_ARROWS_TO_GABOMBA_STATUE_FACE_EAST_A_A, GabombaStatueFace, GABOMBA_STATUE_FACE_FROM_KIBOMBO_STATUE_GROUNDS_CEREMONY_EAST_A),
    SCENE_EXIT(GABOMBA_STATUE_ARROWS_TO_GABOMBA_STATUE_FACE_EAST_B, GabombaStatueFace, GABOMBA_STATUE_FACE_FROM_KIBOMBO_STATUE_GROUNDS_CEREMONY_EAST_B),
    SCENE_EXIT(GABOMBA_STATUE_ARROWS_TO_KIBOMBO_STATUE_GROUNDS_CEREMONY_NORTH_B_B, KibomboStatueGroundsCeremony, KIBOMBO_STATUE_GROUNDS_CEREMONY_FROM_GABOMBA_STATUE_ARROWS_NORTH_B),
    SCENE_EXIT(GABOMBA_STATUE_ARROWS_TO_KIBOMBO_STATUE_GROUNDS_CEREMONY_SOUTH, KibomboStatueGroundsCeremony, KIBOMBO_STATUE_GROUNDS_CEREMONY_FROM_GABOMBA_STATUE_ARROWS_SOUTH),
    SCENE_EXIT(GABOMBA_STATUE_ARROWS_TO_KIBOMBO_STATUE_GROUNDS_CEREMONY_NORTH_C, KibomboStatueGroundsCeremony, KIBOMBO_STATUE_GROUNDS_CEREMONY_FROM_GABOMBA_STATUE_CHAMBER_IDOL_NORTH),
    SCENE_EXIT(GABOMBA_STATUE_ARROWS_TO_KIBOMBO_STATUE_GROUNDS_CEREMONY_NORTH_D, KibomboStatueGroundsCeremony, KIBOMBO_STATUE_GROUNDS_CEREMONY_FROM_GABOMBA_STATUE_ARROWS_NORTH_D),
    SCENE_EXIT(GABOMBA_STATUE_ARROWS_TO_KIBOMBO_STATUE_GROUNDS_CEREMONY_NORTH_E, KibomboStatueGroundsCeremony, KIBOMBO_STATUE_GROUNDS_CEREMONY_FROM_GABOMBA_STATUE_ARROWS_NORTH_E),
    SCENE_EXIT(GABOMBA_STATUE_ARROWS_TO_GABOMBA_STATUE_FACE_EAST_A_B, GabombaStatueFace, GABOMBA_STATUE_FACE_FROM_KIBOMBO_STATUE_GROUNDS_CEREMONY_EAST_A),
    SCENE_EXIT(GABOMBA_STATUE_ARROWS_TO_GABOMBA_STATUE_CHAMBER_IDOL_WEST, GabombaStatueChamberIdol, GABOMBA_STATUE_CHAMBER_IDOL_FROM_GABOMBA_STATUE_ARROWS_WEST),
    SCENE_EXITS_END,
};

LATE_DATA s32 GabombaStatue_FlashCogScript[] = {
    SC_CALL(scGabombaStatueFlashCog),
    SC_END,
};

LATE_DATA s32 GabombaStatue_TurnToothScript[] = {
    SC_CALL(scGabombaStatueTurnTooth),
    SC_END,
};

LATE_DATA ACTOR_PLACE GabombaStatue_BoneArmletChestActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 472, 0, 248, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LATE_DATA ACTOR_PLACE GabombaStatue_ElixirChestActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 88, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, (s32)GabombaStatue_TurnToothScript, 328, 0, 88, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 488, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 488, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_GREEN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 72, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_GREEN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 72, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_GREEN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 168, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_GREEN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 248, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_GREEN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 248, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_GREEN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 520, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_GREEN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 552, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BLUE_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 88, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 200, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BLUE_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 216, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BLUE_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 552, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BLUE_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 488, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BLUE_COG, CONDITION_ALWAYS, (s32)GabombaStatue_FlashCogScript, 360, 0, 488, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BROWN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 352, 0, 168, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BROWN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 400, 0, 168, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 680, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {SCENE_TABLE_END},
};

LATE_DATA ACTOR_PLACE GabombaStatue_SteelActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_GREEN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 56, 0, 168, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_GREEN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 152, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_GREEN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 200, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_GREEN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 120, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_GREEN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 216, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_GREEN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 632, 0, 152, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_GREEN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 728, 0, 264, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BLUE_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 56, 0, 232, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 168, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BLUE_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 248, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BLUE_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 168, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BLUE_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 712, 0, 200, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_RED_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 184, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_RED_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 264, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_RED_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 296, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_RED_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 664, 0, 168, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_RED_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 664, 0, 248, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BLUE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 232, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 216, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BLUE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 248, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BLUE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 632, 0, 216, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_PINK_DRUM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 424, 0, 136, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PINK_DRUM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 472, 0, 248, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 424, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_VENUS_DJINNI, FLAG_DJINNI(DJINNI_STEEL), MOVEMENT_TYPE_STAND, 504, 0, 216, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

LATE_DATA ACTOR_PLACE GabombaStatue_MouseActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_GREEN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 56, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_GREEN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 40, 0, 184, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_GREEN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 168, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_GREEN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 120, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_GREEN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 152, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_GREEN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 184, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_GREEN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 184, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_GREEN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 136, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_GREEN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 136, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_GREEN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 312, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_GREEN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 408, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_GREEN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 680, 0, 232, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_GREEN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 728, 0, 248, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BLUE_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 232, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 152, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BLUE_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 184, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BLUE_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 136, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BLUE_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 376, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BLUE_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 424, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BLUE_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 648, 0, 168, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BLUE_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 680, 0, 264, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_RED_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 232, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_RED_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 200, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_RED_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 168, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_RED_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 328, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_RED_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 712, 0, 200, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BLUE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 248, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 200, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BLUE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 568, 0, 216, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_PINK_DRUM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 216, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PINK_DRUM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 216, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_MOUSE, FLAG_GABOMBA_MOUSE_SEEN, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LATE_DATA ACTOR_PLACE GabombaStatue_CogsActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 232, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 232, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_RED_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_RED_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 296, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BROWN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 416, 0, 216, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BROWN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 464, 0, 216, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {SCENE_TABLE_END},
};

LATE_DATA s32 GabombaStatue_ChargeScript[] = {
    SC_CALL(scGabombaStatueCircleOwner),
    SC_END,
};

LATE_DATA ACTOR_PLACE GabombaStatue_ArrowsActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_CARVED_FACE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ORB_PEDESTAL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ORB_PEDESTAL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ENERGY_CHARGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ENERGY_CHARGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ENERGY_CHARGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ENERGY_CHARGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 152, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHANNEL_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 8, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHANNEL_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 8, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_CHANNEL_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 8, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_CHANNEL_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 8, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_CHANNEL_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 8, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_CHANNEL_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 8, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_CHANNEL_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 8, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_AKAFUBU, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 430, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LATE_DATA ACTOR_PLACE GabombaStatue_PinionActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_BROWN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 288, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BROWN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 336, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BROWN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 392, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BROWN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 128, 0, 456, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BROWN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 176, 0, 456, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {SCENE_TABLE_END},
};

LATE_DATA ACTOR_PLACE GabombaStatue_Placements[] = {
    {SCENE_TABLE_END},
};

/* How the tooth the first rope is tied to rocks as the rope pulls taut, a */
/* step of its turn each frame. */
LATE_DATA s8 GabombaStatue_RopeStopSteps[] = {
    1, 1, 1, 0, 0, 0, -1, -1,
    -1, 0, 0, 0, 1, 1, 0, 0,
    -1, -1, 0, 0, 1, 0, -1, 0,
    0, 0, 0, 0,
};

LATE_DATA TRIGGER GabombaStatue_BoneArmletChestTriggers[] = {
    {STEP_TRIGGER_WALKING, CELL_BEHAVIOR_FIRST_LADDER_DOWN, CONDITION_ALWAYS, EVENT(scGabombaStatueClimbDownBoneArmletChestLadder)},
    {TRIGGER_TABLE_END},
};

LATE_DATA TRIGGER GabombaStatue_ElixirChestTriggers[] = {
    {STEP_TRIGGER_WALKING, CELL_BEHAVIOR_FIRST_LADDER_DOWN, CONDITION_ALWAYS, EVENT(scGabombaStatueClimbDownElixirChestLadder)},
    {TRIGGER_STEP, CELL_BEHAVIOR_PINION_GAP, CONDITION_ALWAYS, EVENT(GabombaStatue_ElixirPinionsCarryLeaderSouth)},
    {TRIGGER_STEP, CELL_BEHAVIOR_PINION_GAP_TURNED_BACK, CONDITION_ALWAYS, EVENT(GabombaStatue_ElixirPinionsCarryLeaderNorth)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_FIRST_ROPE_POST_A, FLAG_GABOMBA_FIRST_ROPE_TIED, EVENT(scGabombaStatueLashRope)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_SECOND_ROPE_POST_A, FLAG_GABOMBA_SECOND_ROPE_TIED, 0},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_FIRST_STAKE, FLAG_GABOMBA_FIRST_STAKE_POUNDED, EVENT(scGabombaStatuePoundStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_SECOND_STAKE, FLAG_GABOMBA_SECOND_STAKE_POUNDED, EVENT(scGabombaStatuePoundStake)},
    {TRIGGER_RAISED, RAISED_COGS_TURN_BACK, FLAG_GABOMBA_COGS_TURNED_BACK, EVENT(scGabombaStatueCogsTurnBack)},
    {TRIGGER_TABLE_END},
};

LATE_DATA TRIGGER GabombaStatue_SteelTriggers[] = {
    {STEP_TRIGGER_WALKING, CELL_BEHAVIOR_FIRST_LADDER_DOWN, CONDITION_ALWAYS, EVENT(scGabombaStatueClimbDownSteelNortheastLadder)},
    {STEP_TRIGGER_WALKING, CELL_BEHAVIOR_SECOND_LADDER_DOWN, CONDITION_ALWAYS, EVENT(scGabombaStatueClimbDownSteelWestLadder)},
    {STEP_TRIGGER_WALKING, CELL_BEHAVIOR_THIRD_LADDER_DOWN, CONDITION_ALWAYS, EVENT(scGabombaStatueClimbDownSteelEastLadder)},
    {TRIGGER_STEP, CELL_BEHAVIOR_GREEN_COG, CONDITION_ALWAYS, EVENT(scGabombaStatueTurnWithCog)},
    {TRIGGER_STEP, CELL_BEHAVIOR_BLUE_COG, CONDITION_ALWAYS, EVENT(scGabombaStatueRideBlueCog)},
    {TRIGGER_STEP, CELL_BEHAVIOR_RED_COG, CONDITION_ALWAYS, EVENT(scGabombaStatueRideSlidingCog)},
    {TRIGGER_STEP, CELL_BEHAVIOR_ROLLER, CONDITION_ALWAYS, EVENT(scGabombaStatueRideRoller)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_COG_END, CONDITION_ALWAYS, EVENT(scGabombaStatueHopAcrossEastOrWest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_COG_END, CONDITION_ALWAYS, EVENT(scGabombaStatueHopAcrossEastOrWest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_COG_END, CONDITION_ALWAYS, EVENT(scGabombaStatueHopAcrossNorthOrSouth)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_COG_END, CONDITION_ALWAYS, EVENT(scGabombaStatueHopAcrossNorthOrSouth)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_STEEL_ROPE_POST_A, FLAG_GABOMBA_FIRST_ROPE_TIED, 0},
    {TRIGGER_TALK, ACTOR_STEEL, IF_FLAG_SET(FLAG_LANDED_BESIDE_STEEL), EVENT(scGabombaStatueFightSteel)},
    {TRIGGER_TABLE_END},
};

LATE_DATA TRIGGER GabombaStatue_MouseTriggers[] = {
    {STEP_TRIGGER_WALKING, CELL_BEHAVIOR_FIRST_LADDER_DOWN, CONDITION_ALWAYS, EVENT(scGabombaStatueClimbDownMouseNorthLadder)},
    {STEP_TRIGGER_WALKING, CELL_BEHAVIOR_SECOND_LADDER_DOWN, CONDITION_ALWAYS, EVENT(scGabombaStatueClimbDownMouseWestLadder)},
    {STEP_TRIGGER_WALKING, CELL_BEHAVIOR_THIRD_LADDER_DOWN, CONDITION_ALWAYS, EVENT(scGabombaStatueClimbDownMouseEastLadder)},
    {TRIGGER_STEP, CELL_BEHAVIOR_GREEN_COG, CONDITION_ALWAYS, EVENT(scGabombaStatueTurnWithCog)},
    {TRIGGER_STEP, CELL_BEHAVIOR_BLUE_COG, CONDITION_ALWAYS, EVENT(scGabombaStatueRideBlueCog)},
    {TRIGGER_STEP, CELL_BEHAVIOR_RED_COG, CONDITION_ALWAYS, EVENT(scGabombaStatueRideSlidingCog)},
    {TRIGGER_STEP, CELL_BEHAVIOR_ROLLER, CONDITION_ALWAYS, EVENT(scGabombaStatueRideRoller)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_COG_END, CONDITION_ALWAYS, EVENT(scGabombaStatueHopAcrossEastOrWest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_COG_END, CONDITION_ALWAYS, EVENT(scGabombaStatueHopAcrossEastOrWest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_COG_END, CONDITION_ALWAYS, EVENT(scGabombaStatueHopAcrossNorthOrSouth)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_COG_END, CONDITION_ALWAYS, EVENT(scGabombaStatueHopAcrossNorthOrSouth)},
    {TRIGGER_STEP, CELL_BEHAVIOR_MOUSE, FLAG_GABOMBA_MOUSE_SEEN, EVENT(scGabombaStatueMouseShowsTheWay)},
    {TRIGGER_TABLE_END},
};

LATE_DATA TRIGGER GabombaStatue_CogsTriggers[] = {
    {STEP_TRIGGER_WALKING, CELL_BEHAVIOR_FIRST_LADDER_DOWN, CONDITION_ALWAYS, EVENT(scGabombaStatueClimbDownCogsLadder)},
    {TRIGGER_STEP, CELL_BEHAVIOR_GREEN_COG, CONDITION_ALWAYS, EVENT(scGabombaStatueTurnWithCog)},
    {TRIGGER_STEP, CELL_BEHAVIOR_BLUE_COG, CONDITION_ALWAYS, EVENT(scGabombaStatueRideBlueCog)},
    {TRIGGER_STEP, CELL_BEHAVIOR_RED_COG, CONDITION_ALWAYS, EVENT(scGabombaStatueRideSlidingCog)},
    {TRIGGER_STEP, CELL_BEHAVIOR_ROLLER, CONDITION_ALWAYS, EVENT(scGabombaStatueRideRoller)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_COG_END, CONDITION_ALWAYS, EVENT(scGabombaStatueHopAcrossEastOrWest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_COG_END, CONDITION_ALWAYS, EVENT(scGabombaStatueHopAcrossEastOrWest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_COG_END, CONDITION_ALWAYS, EVENT(scGabombaStatueHopAcrossNorthOrSouth)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_COG_END, CONDITION_ALWAYS, EVENT(scGabombaStatueHopAcrossNorthOrSouth)},
    {TRIGGER_STEP, CELL_BEHAVIOR_PINION_GAP, CONDITION_ALWAYS, EVENT(GabombaStatue_CogsPinionsCarryLeaderSouth)},
    {TRIGGER_STEP, CELL_BEHAVIOR_PINION_GAP_TURNED_BACK, CONDITION_ALWAYS, EVENT(GabombaStatue_CogsPinionsCarryLeaderNorth)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_COGS_ROPE_POST_A, CONDITION_ALWAYS, 0},
    {TRIGGER_TABLE_END},
};

LATE_DATA TRIGGER GabombaStatue_ArrowsTriggers[] = {
    {STEP_TRIGGER_WALKING, CELL_BEHAVIOR_FIRST_LADDER_DOWN, CONDITION_ALWAYS, EVENT(scGabombaStatueClimbDownArrowsLadder)},
    {TRIGGER_STEP, CELL_BEHAVIOR_AROUND_LEFT_ARROW, CONDITION_ALWAYS, EVENT(scGabombaStatueReleaseLeftArrowPanel)},
    {TRIGGER_STEP, CELL_BEHAVIOR_LEFT_ARROW, CONDITION_ALWAYS, EVENT(scGabombaStatuePressLeftArrowPanel)},
    {TRIGGER_STEP, CELL_BEHAVIOR_AROUND_RIGHT_ARROW, CONDITION_ALWAYS, EVENT(scGabombaStatueReleaseRightArrowPanel)},
    {TRIGGER_STEP, CELL_BEHAVIOR_RIGHT_ARROW, CONDITION_ALWAYS, EVENT(scGabombaStatuePressRightArrowPanel)},
    {TRIGGER_RAISED, RAISED_OFFERING_HEARD, FLAG_OFFERING_HEARD, EVENT(scGabombaStatueHearTheOffering)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_ARROWS_STAKE, FLAG_GABOMBA_RESPONDED, EVENT(scGabombaStatuePoundArrowsStake)},
    {TRIGGER_RAISED, RAISED_STAKE_STANDS_UP, CONDITION_ALWAYS, EVENT(scGabombaStatueStakeStandsUp)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_BLOCK_A, CONDITION_ALWAYS, EVENT(scGabombaStatuePoundBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_BLOCK_B, CONDITION_ALWAYS, EVENT(scGabombaStatuePoundBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_BLOCK_C, CONDITION_ALWAYS, EVENT(scGabombaStatuePoundBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_BLOCK_D, CONDITION_ALWAYS, EVENT(scGabombaStatuePoundBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_BLOCK_E, CONDITION_ALWAYS, EVENT(scGabombaStatuePoundBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_BLOCK_F, CONDITION_ALWAYS, EVENT(scGabombaStatuePoundBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_BLOCK_G, CONDITION_ALWAYS, EVENT(scGabombaStatuePoundBlock)},
    {TRIGGER_TABLE_END},
};

LATE_DATA TRIGGER GabombaStatue_PinionTriggers[] = {
    {TRIGGER_STEP, CELL_BEHAVIOR_PINION_GAP, CONDITION_ALWAYS, EVENT(GabombaStatue_PinionsCarryLeaderNorth)},
    {TRIGGER_STEP, CELL_BEHAVIOR_PINION_GAP_TURNED_BACK, CONDITION_ALWAYS, EVENT(scGabombaStatuePinionsThrowLeaderSouth)},
    {TRIGGER_TABLE_END},
};

LATE_DATA TRIGGER GabombaStatue_Triggers[] = {
    {TRIGGER_TABLE_END},
};

/* Each piece turned a quarter turn counterclockwise and clockwise, a pair a */
/* piece in CHANNEL_PIECE's order. */
LATE_DATA u8 GabombaStatue_TurnedPieces[] = {
    PIECE_NONE, PIECE_NONE,
    PIECE_EAST_WEST, PIECE_EAST_WEST,
    PIECE_NORTH_SOUTH, PIECE_NORTH_SOUTH,
    PIECE_CROSSING, PIECE_CROSSING,
    PIECE_NORTH_EAST, PIECE_SOUTH_WEST,
    PIECE_EAST_SOUTH, PIECE_WEST_NORTH,
    PIECE_SOUTH_WEST, PIECE_NORTH_EAST,
    PIECE_WEST_NORTH, PIECE_EAST_SOUTH,
    PIECE_SOUTH_WEST_AND_NORTH_EAST, PIECE_SOUTH_WEST_AND_NORTH_EAST,
    PIECE_EAST_SOUTH_AND_WEST_NORTH, PIECE_EAST_SOUTH_AND_WEST_NORTH,
    PIECE_STOP, PIECE_STOP,
    PIECE_SOCKET, PIECE_SOCKET,
};

LATE_DATA u32 GabombaStatue_ChargeBurstScript[] = {
    SC_ANIMATION(1),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, FIXED_ONE * 5 / 128),
    SC_ADD(SW_SCALE_Y, FIXED_ONE * 5 / 128),
    SC_WAIT(1),
    SC_LOOP(24, SC_TO(1)),
    SC_POSITION(0, 0, 0),
    SC_SET(SW_SCALE_X, FIXED_ONE),
    SC_SET(SW_SCALE_Y, FIXED_ONE),
    SC_SET(SW_SCENE_BYTE, 0),
    SC_WAIT(1),
    SC_END,
};

LATE_DATA s32 GabombaStatue_ChargeShrinkScript[] = {
    SC_ANIMATION(0),
    SC_SET(SW_SCALE_X, FIXED_ONE * 2),
    SC_SET(SW_SCALE_Y, FIXED_ONE * 2),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, -FIXED_ONE * 5 / 128),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE * 5 / 128),
    SC_WAIT(1),
    SC_LOOP(48, SC_TO(1)),
    SC_POSITION(0, 0, 0),
    SC_SET(SW_SCALE_X, FIXED_ONE),
    SC_SET(SW_SCALE_Y, FIXED_ONE),
    SC_SET(SW_SCENE_BYTE, 0),
    SC_WAIT(1),
    SC_END,
};
/* clang-format on */

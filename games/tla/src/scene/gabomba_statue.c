/* Inside the Gabomba Statue at Kibombo, the seven rooms of this overlay: halls
   of turning cogs and pinions that Felix rides and hops across.

   From the statue grounds the party comes into the hall of pinions, whose
   turning pinions in the south-west pull Felix through to the north. Above lie
   the room of corridors with the Bone Armlet's chest, and the room of the
   flashing cog, where Lash on a rope stops every cog (scGabombaStatueLashRope)
   so that Felix can cross to the ladder up to Steel's room and the mouse's
   room, where a mouse shows how to hop from cog to cog
   (scGabombaStatueMouseShowsTheWay). Back in the room of the flashing cog,
   Pound on the two stakes beside it turns all the cogs the other way
   (scGabombaStatuePoundStake); a pink drum in the mouse's room then drops Felix
   beside the Venus Djinni Steel. The way on leads through the pinions of the
   room below the small idol's chamber, up to the chamber and to the room of the
   arrow panels behind the statue's eyes. There the party watches the ceremony's
   two magical charges stick in a broken channel and mends it
   (scGabombaStatueFocusEnergies to scGabombaStatueKradenNoticesTiming): Pound
   on the stake raises the channel's blocks, the arrow panels turn them, and
   Pound sets each back in, until the blue and the red orb statues' charges
   reach the statue in the middle at once. */

/* This unit declares its own form of fdActorStartIdle, below. */
#define OWN_fdActorStartIdle

#include "../field/actor_set_sprite_priority.c"

#include "../field/field_effect_move.c"

/* The ROM's callers here are compiled with no declaration in scope, as against
   this form: no parameter list and an Sint32 result, where the definition takes
   the actor and returns nothing. One of them passes no actor. */
Sint32 fdActorStartIdle();

/* Both stakes have been Pounded and the cogs turned the other way. */
#define FLAG_GABOMBA_COGS_TURNED_BACK	   0x8cd
/* The party has watched how the statue's works run. */
#define FLAG_GABOMBA_SAW_HOW_IT_WORKS	   0x8f6
/* Kraden has said it is when the charges arrive that matters ("Maybe we need to
   look at when the two magical charges reach the statue in the middle"). */
#define FLAG_GABOMBA_TIMING_NOTICED		   0x9ec
#define FLAG_GABOMBA_FIRST_STAKE_POUNDED   0x8ce /* the west stake */
#define FLAG_GABOMBA_SECOND_STAKE_POUNDED  0x8cf /* the east stake */
/* The mouse that shows the way across the cogs of its room has been seen. */
#define FLAG_GABOMBA_MOUSE_SEEN			   0xa6c
/* The two magical charges reached the statue in the middle at different
   times. */
#define FLAG_GABOMBA_CHARGES_REACHED_APART 0x9eb

/* The ladder-top blocks are drawn (scGabombaStatueRestoreLadderTopBlocks). */
#define FLAG_LADDER_TOP_BLOCKS_DRAWN  FLAG_SCENE_TEMP_00
#define FLAG_GABOMBA_FIRST_ROPE_TIED  FLAG_SCENE_TEMP_01 /* by fdRopePlace */
#define FLAG_GABOMBA_SECOND_ROPE_TIED FLAG_SCENE_TEMP_02 /* by fdRopePlace */
/* A tied rope has stopped the cogs (scGabombaStatueLashRope). */
#define FLAG_COGS_STOPPED			  FLAG_SCENE_TEMP_03
/* The party has fallen in beside Steel, who can then be talked to. */
#define FLAG_LANDED_BESIDE_STEEL	  FLAG_SCENE_TEMP_04
/* The offering below has been heard (scGabombaStatueHearTheOffering). */
#define FLAG_OFFERING_HEARD			  FLAG_SCENE_TEMP_05

/* The mouse that runs ahead across the cogs of its room and shows the way
   (scGabombaStatueMouseShowsTheWay). */
enum { ACTOR_MOUSE = 39 };

/* The arrows room's Kraden, who works out the channel with the party, and
   Akafubu, who comes up into the statue once the ceremony has worked
   (scGabombaStatueSeeHowItWorks, scGabombaStatueSawEnergyFlow). */
enum { ACTOR_KRADEN = 23, ACTOR_AKAFUBU = 24 };

/* Each room's placed actors. A row of cogs that share one sprite is named by
   the first of them: turning its animation turns them all. A Lashed rope runs
   between two posts, its second post riding a cog's tooth. A ladder-top block
   is a black square (SPRITE_UNSEEN_BLOCK) at the top of a ladder up to the room
   above, held 36 pixels up in SPRITE_COLOR_HIDDEN, so that nothing of it shows
   (scGabombaStatueHideActor).

   Drawn or hidden, the ladder-top blocks change nothing on screen:
   SPRITE_COLOR_HIDDEN draws every pixel of them transparent, and the display
   has no sprite window they could cut. */
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

/* The arrows room: the statue in the middle, where the charges must arrive, the
   statues holding the blue and the red orb whose charges run along the channel,
   the stake whose Pound raises the blocks, and the seven loose blocks of the
   channel ("It looks like the energy charge is supposed to run over these
   blocks."). */
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

/* The metatile behaviors of the rooms' cue tables. A hole is the top of a
   ladder down to the room below, and its number is that of the exit it leaves
   by. Between two pinions the leader is pulled through to the other side, the
   metatiles taking the second behavior while the cogs turn back
   (scGabombaStatueTurnCogs copies them in). The metatiles under a cog
   (scGabombaStatueDrawCogMetatiles) carry the leader with it, and the metatiles
   past a cog's ends hop him across to the next. */
typedef enum GABOMBA_STATUE_METATILE_BEHAVIOR {
	METATILE_BEHAVIOR_FIRST_LADDER_DOWN = 1,
	METATILE_BEHAVIOR_SECOND_LADDER_DOWN,
	METATILE_BEHAVIOR_THIRD_LADDER_DOWN,
	METATILE_BEHAVIOR_PINION_GAP = 10,
	METATILE_BEHAVIOR_PINION_GAP_TURNED_BACK,
	METATILE_BEHAVIOR_MOUSE = 77, /* where the mouse pops up */
	METATILE_BEHAVIOR_GREEN_COG = 80,
	METATILE_BEHAVIOR_BLUE_COG,
	METATILE_BEHAVIOR_RED_COG,
	METATILE_BEHAVIOR_ROLLER, /* a roller: a blue pillar or a pink drum */
	METATILE_BEHAVIOR_COG_END,
	/* The metatile past a pink drum's south end, which no cue reads. */
	METATILE_BEHAVIOR_PINK_DRUM_SOUTH_END
} GABOMBA_STATUE_METATILE_BEHAVIOR;

/* The arrows room's: the panel with an arrow, and the metatiles around it that
   raise it again once the leader steps off. */
typedef enum GABOMBA_STATUE_ARROW_METATILE_BEHAVIOR {
	METATILE_BEHAVIOR_AROUND_LEFT_ARROW = 80,
	METATILE_BEHAVIOR_LEFT_ARROW,
	METATILE_BEHAVIOR_AROUND_RIGHT_ARROW,
	METATILE_BEHAVIOR_RIGHT_ARROW
} GABOMBA_STATUE_ARROW_METATILE_BEHAVIOR;

/* The cues this overlay raises: both stakes Pounded, the arrows room's stake
   Pounded, and the offering's clock run out. */
enum {
	RAISED_COGS_TURN_BACK = 200,
	RAISED_STAKE_STANDS_UP = 200,
	RAISED_OFFERING_HEARD = 201
};

#include "graphics/palette_buffers.h"
#include "field/script_inline.h"
#include "field/field_mode.h"
#include "scene/gabomba_statue.h"
#include "scene/scene.h"
#include "game/game.h"
#include "macros.h"
#include "field/object.h"
#include "field/call.h"
#include "field/map.h"
#include "field/actor_script.h"
#include "text/msg_ids.h"

extern const Sint32 *FieldEffectDurationScripts[];

#define EFFECT_SET_MODE	  fdActorSelectAnimation
#define EFFECT_SET_SCRIPT fdActorStartScript
#include "../field/field_effect_spawn.c"

/* Steel, the Venus Djinni waiting in Steel's room. */
enum { ACTOR_STEEL = 33 };

/* Talking to Steel, beside whom Felix lands after riding a pink drum off the
   mouse's room, starts its battle. */
void scGabombaStatueFightSteel(void)
{
	fdRequestDjinnBattle(ACTOR_STEEL, DJINNI_STEEL);
}

/* A member of the party looking about: it turns between its two headings every
   10 to 49 ticks. */
Sint32 scGabombaStatueLookAround(ACTOR *actor)
{
	if(actor->lookTimer != 0) {
		actor->lookTimer--;
	} else {
		actor->lookTimer = (syRandomBelow(40)) + 10;
		if(actor->lookTurned != 0) {
			actor->lookTurned = 0;
			actor->facing = actor->lookFacing;
		} else {
			actor->lookTurned = 1;
			actor->facing = actor->lookOtherFacing;
		}
	}
	return 1;
}

const SCENE_ENTRANCE *scGetEntrances(void)
{
	return SceneEntrances;
}

extern SCENE_REGION GabombaStatuePinionExitRegions[];
extern SCENE_REGION GabombaStatueBoneArmletChestExitRegions[];
extern SCENE_REGION GabombaStatueElixirChestExitRegions[];
extern SCENE_REGION GabombaStatueSteelExitRegions[];
extern SCENE_REGION GabombaStatueMouseExitRegions[];
extern SCENE_REGION GabombaStatueCogsExitRegions[];

const SCENE_REGION *scGetExitRegions(void)
{
	Sint16 scene = MainRoutineStateWork.next;

	if(scene == SCENE_GABOMBA_STATUE_PINION)
		return GabombaStatuePinionExitRegions;
	if(scene == SCENE_GABOMBA_STATUE_BONE_ARMLET_CHEST)
		return GabombaStatueBoneArmletChestExitRegions;
	if(scene == SCENE_GABOMBA_STATUE_ELIXIR_CHEST)
		return GabombaStatueElixirChestExitRegions;
	if(scene == SCENE_GABOMBA_STATUE_STEEL)
		return GabombaStatueSteelExitRegions;
	if(scene == SCENE_GABOMBA_STATUE_MOUSE)
		return GabombaStatueMouseExitRegions;
	if(scene == SCENE_GABOMBA_STATUE_COGS)
		return GabombaStatueCogsExitRegions;
	return NULL;
}

const Uint32 *scGetExits(void)
{
	return SceneExits;
}

/* The flashing cog's update: its colours swap between two orders every ten
   ticks. */
Sint32 scGabombaStatueFlashCog(ACTOR *actor)
{
	Uint32 frame = TickCount;

	if(frame % 10 == 0)
		((SPR *)actor->sprite)->entries[0]->colorMode =
			frame % 20 / 10 + SPRITE_COLOR_BGR;
	return 1;
}

/* A room's ropes, by the index the rope functions take; the room of the
   flashing cog has two. */
enum { FIRST_ROPE, SECOND_ROPE };

/* The steps of a cog tooth's turn: its phase, kept in the tooth's sceneCount,
   counts them, two degrees each. */
enum { COG_TOOTH_STEPS = 180 };

/* The arrows room's channel: a grid of blocks, eleven columns by eight rows,
   from the map's metatile (19, 11). The statue in the middle, where the charges
   must arrive, stands on its metatile (5, 3), and the two orb statues on (1, 7)
   and (9, 7). */
enum {
	CHANNEL_COLUMNS = 11,
	CHANNEL_ROWS = 8,
	CHANNEL_METATILES = CHANNEL_COLUMNS * CHANNEL_ROWS,
	CHANNEL_ORIGIN_COLUMN = 19,
	CHANNEL_ORIGIN_ROW = 11,
	CHANNEL_MIDDLE_STATUE_COLUMN = 5,
	CHANNEL_MIDDLE_STATUE_ROW = 3,
	CHANNEL_BLUE_STATUE_COLUMN = 1,
	CHANNEL_RED_STATUE_COLUMN = 9,
	CHANNEL_STATUES_ROW = 7
};

/* The way a charge runs along the channel, and the way a path leaves a
   metatile. */
typedef enum CHANNEL_HEADING {
	HEADING_EAST,
	HEADING_SOUTH,
	HEADING_WEST,
	HEADING_NORTH
} CHANNEL_HEADING;

/* A heading's bit in a set of headings. */
#define HEADING_BIT(heading) (1 << (heading))

/* A path's tried headings once every one has been tried. */
#define HEADING_ALL_TRIED \
	(HEADING_BIT(HEADING_EAST) | HEADING_BIT(HEADING_SOUTH) | \
		HEADING_BIT(HEADING_WEST) | HEADING_BIT(HEADING_NORTH))

/* The pieces of the channel, by the sides of the metatile they join, which are
   also the rows of the metatile tiles the map keeps for them. */
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

/* The four charges' paths, by the bit of PUZZLE_METATILE.state that marks a
   metatile one runs through (scGabombaStatueLayPath takes the bit's number).
   Each orb statue sends one charge west and one east. */
enum {
	PATH_BIT_BLUE_WEST,
	PATH_BIT_RED_EAST,
	PATH_BIT_BLUE_EAST,
	PATH_BIT_RED_WEST
};

/* The bits of PUZZLE_METATILE.state: which of the four charges' paths run
   through the metatile. */
typedef enum CHANNEL_PATH {
	PATH_BLUE_WEST = 1 << PATH_BIT_BLUE_WEST,
	PATH_RED_EAST = 1 << PATH_BIT_RED_EAST,
	PATH_BLUE_EAST = 1 << PATH_BIT_BLUE_EAST,
	PATH_RED_WEST = 1 << PATH_BIT_RED_WEST,
	PATH_BLUE = PATH_BLUE_WEST | PATH_BLUE_EAST,
	PATH_RED = PATH_RED_WEST | PATH_RED_EAST
} CHANNEL_PATH;

/* What stands on a metatile of the channel (PUZZLE_METATILE.occupied). The map
   keeps each piece's metatile tiles in a row of their own (CHANNEL_PIECE), a
   column for each of these: the column fdMapGridCopyMetatiles copies from is
   what stands on the metatile. */
enum {
	BLOCK_NONE,		/* its piece is part of the floor */
	BLOCK_IN_PLACE, /* a loose block lies in it, to be turned or raised */
	BLOCK_RAISED	/* its block floats over the room, the socket open */
};

/* ACTOR.chargeState. While a charge circles its orb statue before the run, the
   same byte holds its actor id (scGabombaStatueSendCharge); the end of its
   burst or shrink script sets CHARGE_STOPPED. */
enum {
	CHARGE_STOPPED = 0,
	CHARGE_RUNNING = 1,
	CHARGE_LEAVING = 2, /* bursting or shrinking away off the channel */
	CHARGE_ARRIVED = 7	/* at the statue in the middle */
};

/* The blocks of the channel, one for each of the room's block actors. */
enum { CHANNEL_BLOCKS = 7 };

/* A Pound handler runs each tick of the fist's fall with (landed, tick):
   landed is 1 on the tick the fist lands, POUND_LAND_TICK, and what it
   strikes sinks a step on each tick before. */
enum { POUND_LAND_TICK = 8 };

/* The channel blocks' animations, which all the blocks share with the first:
   the green crystal spins one way as the right arrow panel turns them
   clockwise, and back as the left turns them counterclockwise. */
enum { BLOCK_ANIM_TURN_CLOCKWISE = 2, BLOCK_ANIM_TURN_COUNTERCLOCKWISE = 3 };

/* The two ways a block turns (scGabombaStatueTurnBlockPiece). */
enum { TURN_COUNTERCLOCKWISE, TURN_CLOCKWISE };

/* A charge's step along the channel: where it heads and the piece under it. */
#define CHARGE_STEP(heading, piece) (((heading) << 8) + (piece))
extern Sint32 GabombaStatueCogDirection;

/* Where the cog's tooth an actor rides is now: the rope's target hook, so that
   a rope tied to it follows the turning cog. */
void scGabombaStatueGetToothPos(ACTOR *actor, FIXED *x, FIXED *z)
{
	FIXED  pos[3];
	Sint32 angle = actor->sceneCount * TRIG_TURN / COG_TOOTH_STEPS;
	Sint32 turn;

	SET_VECTOR(
		pos, INTtoFIXED(actor->homeX + 16), toFIXED(16), toFIXED(actor->homeZ));
	turn = GabombaStatueCogDirection * (TRIG_TURN * 3 / 16) + angle;
	syFieldVectorAddPolarOffset(13, turn + DEGtoANG(180), pos);
	*x = pos[0];
	*z = pos[2];
}

/* A cog tooth's update: it turns about the cog's centre by the cogs'
   direction. */
Sint32 scGabombaStatueTurnTooth(ACTOR *actor)
{
	Uint8 *phase = &actor->sceneCount;
	Sint32 angle = *phase * TRIG_TURN / COG_TOOTH_STEPS;
	Sint32 next;

	actor->priorityFlags = 0;
	actor->x.fixed = INTtoFIXED(actor->homeX + 16);
	actor->y.fixed = toFIXED(16);
	actor->z.fixed = toFIXED(actor->homeZ);
	syFieldVectorAddPolarOffset(13, angle + DEGtoANG(180), &actor->x.fixed);
	next = *phase + GabombaStatueCogDirection;
	if(next < 0)
		next = COG_TOOTH_STEPS - 1;
	if(next > COG_TOOTH_STEPS - 1)
		next = 0;
	*phase = next;
	actor->watchedActor = (FIELD_OBJECT *)scGabombaStatueGetToothPos;
	return 1;
}

/* A sprite as the depth queue takes it (QUEUED_SPR), its affine mode and matrix
   written as bytes. */
typedef struct SPR_VIEW {
	Uint8  link[4]; /* the depth queue's link to the next sprite */
	Uint8  y;
	Uint8  affineMode : 2;
	Uint8  modeAndShape : 6; /* object mode, mosaic, colours and shape */
	Uint8  xLow;
	Uint8  xHigh : 1;
	Uint8  matrix : 5;
	Uint8  size : 2;
	Uint32 attribute2;
} SPR_VIEW;

/* The channel puzzle's state: its grid of pieces, laid from a random seed, and
   the blocks and arrows drawn over it. */
typedef struct SCENE_STATE {
	PUZZLE_METATILE metatiles[CHANNEL_ROWS][CHANNEL_COLUMNS];
	Sint32			seed; /* the random seed the channel was laid from */
	Sint16			firstBlock;
	Sint16			endBlock;
	Sint16	 blockTileEntry; /* the VRAM cache entry of the blocks' pictures */
	Sint8	 leftArrowDown;
	Sint8	 rightArrowDown;
	Sint16	 originX;
	Sint16	 originZ;
	Sint16	 blocksRaised;
	Uint8	 unusedAfterBlocksRaised[2];
	Sint32	 blocksAngle;
	SPR_VIEW oam[10];
} SCENE_STATE;

/* The scene's view of SceneStateWork (system/globals.h). */
#define SceneStateWork (*(SCENE_STATE *)SceneStateWork)
extern Sint8 GabombaStatueCogAnimations[];

/* How far a charge circles its orb statue each tick, in 256ths of a turn. */
enum { CIRCLE_STEP = 16 };

/* Each tick, keeps an actor circling the actor it watches. */
Sint32 scGabombaStatueCircleOwner(ACTOR *actor)
{
	ACTOR *owner = &actor->watchedActor->actor;

	if(owner != NULL) {
		Uint8 *phase = &actor->sceneCount;

		actor->x.fixed = owner->x.fixed;
		actor->y.fixed = owner->y.fixed + toFIXED(16);
		actor->z.fixed = owner->z.fixed;
		actor->facing = *phase << 8;
		syFieldVectorAddPolarOffset(6, *phase << 8, &actor->x.fixed);
		*phase += CIRCLE_STEP;
	}
	return 1;
}

extern ACTOR_PLACE GabombaStatuePlacements[];
extern ACTOR_PLACE GabombaStatueBoneArmletChestActorPlacements[];
extern ACTOR_PLACE GabombaStatueElixirChestActorPlacements[];
extern ACTOR_PLACE GabombaStatueSteelActorPlacements[];
extern ACTOR_PLACE GabombaStatueMouseActorPlacements[];
extern ACTOR_PLACE GabombaStatueCogsActorPlacements[];
extern ACTOR_PLACE GabombaStatueArrowsActorPlacements[];
extern ACTOR_PLACE GabombaStatuePinionActorPlacements[];

const Uint8 *scGetActorPlacements(void)
{
	Sint16		 scene = MainRoutineStateWork.next;
	const Uint8 *table = GabombaStatuePlacements;

	if(scene == SCENE_GABOMBA_STATUE_BONE_ARMLET_CHEST)
		table = GabombaStatueBoneArmletChestActorPlacements;
	else if(scene == SCENE_GABOMBA_STATUE_ELIXIR_CHEST)
		table = GabombaStatueElixirChestActorPlacements;
	else if(scene == SCENE_GABOMBA_STATUE_STEEL)
		table = GabombaStatueSteelActorPlacements;
	else if(scene == SCENE_GABOMBA_STATUE_MOUSE)
		table = GabombaStatueMouseActorPlacements;
	else if(scene == SCENE_GABOMBA_STATUE_COGS)
		table = GabombaStatueCogsActorPlacements;
	else if(scene == SCENE_GABOMBA_STATUE_ARROWS)
		table = GabombaStatueArrowsActorPlacements;
	else if(scene == SCENE_GABOMBA_STATUE_PINION)
		table = GabombaStatuePinionActorPlacements;
	return table;
}

extern Sint8 GabombaStatueRopeStopSteps[];

/* The ticks the cog's tooth takes to settle as the rope pulls taut. */
enum { ROPE_STOP_TICKS = 25 };

/* Whether a and b are at most limit apart. */
#define DIFF_WITHIN(a, b, limit) \
	((a) - (b) >= 0 ? (a) - (b) <= (limit) : (b) - (a) <= (limit))

/* Lash on the rope by the cogs in the north-west of the room of the flashing
   cog, between the posts at metatiles (18, 5) and (20, 5), ties it to a tooth
   of the blue cog. As it pulls taut the tooth rocks to a halt, and every cog in
   the room stops, so Felix can cross them to the ladder on the east side. */
void scGabombaStatueLashRope(void)
{
	ACTOR *actor = fdActorGet(ACTOR_FIRST_ROPE_POST_B);
	Sint32 direction;
	Sint32 i;

	if(fdRopeGetState(FIRST_ROPE) == ROPE_THROWN) {
		direction = GabombaStatueCogDirection;
		scGabombaStatueTurnCogs(0);
		fdScriptSetFlag(FLAG_COGS_STOPPED);
		FieldStateWork->psynergy = 0;
		for(i = 0; i < ROPE_STOP_TICKS; i++) {
			actor->sceneCount += GabombaStatueRopeStopSteps[i] * direction;
			syWait(1);
		}
	}
}

/* The cog of sprite id within 24 pixels of origin, or null. */
ACTOR *scGabombaStatueFindCogUnder(ACTOR *origin, Sint32 id)
{
	Sint32 i;

	for(i = ACTOR_FIRST_PLACED; i < FIELD_TREASURE_ACTOR; i++) {
		ACTOR *cog = fdActorGet(i);

		if(cog != NULL && cog->spriteStorageKind == ACTOR_SPRITE_ONE &&
			((SPR *)cog->sprite)->entries[0] != NULL &&
			((SPR *)cog->sprite)->entries[0]->animationId == id) {
			if(DIFF_WITHIN(cog->x.fixed, origin->x.fixed, toFIXED(24) - 1) &&
				DIFF_WITHIN(cog->z.fixed, origin->z.fixed, toFIXED(24) - 1))
				return cog;
		}
	}
	return NULL;
}

/* As scGabombaStatueFindCogUnder, but around the point 16 pixels ahead of
   origin in its facing. */
ACTOR *scGabombaStatueFindCogAhead(ACTOR *origin, Sint32 id)
{
	FIXED  pos[3];
	Sint32 i;

	pos[0] = origin->x.fixed;
	pos[2] = origin->z.fixed;
	syFieldVectorAddPolarOffset(16, origin->facing, pos);
	for(i = ACTOR_FIRST_PLACED; i < FIELD_TREASURE_ACTOR; i++) {
		ACTOR *cog = fdActorGet(i);

		if(cog != NULL && cog->spriteStorageKind == ACTOR_SPRITE_ONE &&
			((SPR *)cog->sprite)->entries[0] != NULL &&
			((SPR *)cog->sprite)->entries[0]->animationId == id) {
			if(DIFF_WITHIN(cog->x.fixed, pos[0], toFIXED(24) - 1) &&
				DIFF_WITHIN(cog->z.fixed, pos[2], toFIXED(24) - 1))
				return cog;
		}
	}
	return NULL;
}

/* The leader hops off a cog by dx, dz. */
void scGabombaStatueHopOffCog(
	ACTOR *actor, Sint32 dx, Sint32 dz, Sint32 velocity)
{
	Uint8 *flags = &actor->motionFlags;
	Uint8  saved;

	syImportLeaderStopMoving(actor);
	saved = *flags;
	fdActorSelectAnimation(actor, ANIM_LEADER_SPRING);
	syWait(6);
	fdScriptPlaySound(SE_JUMP);
	fdActorSelectAnimation(actor, ANIM_LEADER_LEAP);
	actor->speedLimit = toFIXED(3);
	actor->acceleration = toFIXED(2);
	actor->velocityY = velocity;
	*flags &= ACTOR_JUMP_KEEPS_MOTION;
	fdActorSetSpriteFlags(actor, 0);
	scFieldActorSetMoveTargetFX(
		actor, actor->x.fixed + dx, actor->y.fixed, actor->z.fixed + dz);
	scFieldActorWaitForMove(actor);
	*flags = saved;
	fdActorSelectAnimation(actor, ANIM_LEADER_SPRING);
	fdActorSetSpriteFlags(actor, SPRITE_FLAG_SHADOW);
	syWait(3);
}

/* Standing on a green cog turns the leader with it. */
void scGabombaStatueTurnWithCog(void)
{
	Sint32 step = GabombaStatueCogDirection;

	if(step != 0) {
		Sint32 turn = step * DEGtoANG(6);
		Sint32 delta = -turn;
		ACTOR *leader = fdActorGet(syFieldPartyGetLeaderActor());
		ACTOR *near = scGabombaStatueFindCogUnder(leader, SPRITE_GREEN_COG);

		if(near != NULL) {
			Uint16 angle = near->facing;

			if(angle == FACING_WEST)
				delta = turn;
		}
		leader->facing += delta;
	}
}

/* Standing on a blue cog carries the leader round with it, unless a wall stops
   him. */
void scGabombaStatueRideBlueCog(void)
{
	ACTOR  *actor, *cog;
	VECTOR3 test;
	Sint32	step = -DEGtoANG(2);
	Sint32	turn;
	FIXED	x, z, dx, dz;
	Sint32	back;
	FIXED	tx, tz;

	if(GabombaStatueCogDirection == 0)
		return;
	actor = fdActorGetById(fdPartyGetLeaderActor());
	cog = scGabombaStatueFindCogUnder(actor, SPRITE_BLUE_COG);
	turn = GabombaStatueCogDirection * step;
	if(cog != 0) {
		if(cog->y.fixed + toFIXED(16) > actor->y.fixed)
			return;
		if(cog->facing == FACING_WEST)
			turn = -turn;
		dx = actor->x.fixed - cog->x.fixed;
		dz = actor->z.fixed - cog->z.fixed;
		back = -turn;
		{
			Sint32 s = syMathSin(back);
			Sint32 c = syCos(back);
			x = cog->x.fixed + syMulFX(c, dx) + syMulFX(s, dz);
			z = cog->z.fixed - syMulFX(s, dx) + syMulFX(c, dz);
		}
		test.x = x + actor->velocityX;
		test.y = cog->y.fixed;
		test.z = z + actor->velocityZ;
		if(fdMapGridCheckCollision(actor, &test) == 0) {
			Sint32 s, c, angle;
			angle = DEGtoANG(30) - turn;
			s = syMathSin(angle);
			c = syCos(angle);
			tx = cog->x.fixed + syMulFX(c, dx) + syMulFX(s, dz);
			tz = cog->z.fixed - syMulFX(s, dx) + syMulFX(c, dz);
			test.x = tx + actor->velocityX;
			test.z = tz + actor->velocityZ;
			if(fdMapGridCheckCollision(actor, &test) == 0) {
				FIXED  s, c;
				Sint32 angle;
				angle = back - DEGtoANG(30);
				s = syMathSin(angle);
				c = syCos(angle);
				tx = cog->x.fixed + syMulFX(c, dx) + syMulFX(s, dz);
				tz = cog->z.fixed - syMulFX(s, dx) + syMulFX(c, dz);
				test.x = tx + actor->velocityX;
				test.z = tz + actor->velocityZ;
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
   its middle; 20 pixels from its centre he hops off the end. */
void scGabombaStatueRideSlidingCog(void)
{
	Sint32 *speed = &GabombaStatueCogDirection;

	if(*speed != 0) {
		ACTOR *actor = fdActorGet(syFieldPartyGetLeaderActor());
		ACTOR *cog = scGabombaStatueFindCogUnder(actor, SPRITE_RED_COG);
		Sint32 dx = *speed * toFIXED(0.5);

		if(cog != NULL) {
			FIXED difference, x, cogX;

			if(cog->facing == FACING_WEST)
				dx = -dx;
			if(cog->z.fixed - actor->z.fixed < -toFIXED(2))
				actor->z.fixed = cog->z.fixed + toFIXED(2);
			if(cog->z.fixed - actor->z.fixed > toFIXED(2))
				actor->z.fixed = cog->z.fixed - toFIXED(2);
			x = actor->x.fixed + dx;
			actor->x.fixed = x;
			cogX = cog->x.fixed;
			difference = cogX - x;
			if(difference >= 0 ? difference >= toFIXED(20)
							   : x - cogX >= toFIXED(20)) {
				Sint32 sign;
				FIXED  offset;

				sign = 0;
				offset = actor->x.fixed - cogX;
				if(offset != 0) {
					sign = 1;
					if(offset <= 0)
						sign = -sign;
				}
				scGabombaStatueHopOffCog(
					actor, sign * toFIXED(12), 0, toFIXED(2));
			}
		}
	}
}

/* The ride on a pink drum of the mouse's room ends at its south end: the leader
   drops off it to the room below. */
void scGabombaStatueEndRide(ACTOR *actor)
{
	Uint32				i;
	MAIN_ROUTINE_STATE *routine;

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdActorSetSpriteFlags(actor, 0);
	actor->motionFlags = 0;
	actor->y.fixed = toFIXED(6);
	actor->z.fixed = toFIXED(247.25707);
	actor->facing = FACING_SOUTH;
	routine = &MainRoutineStateWork;
	fdActorSetAttachedEffect(routine->leaderActorId,
		ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAnimation(routine->leaderActorId, ANIM_LEADER_WALK_BACKWARDS);
	syFieldScriptWait(30);
	fdActorSetAnimation(routine->leaderActorId, ANIM_LEADER_ARMS_RAISED);
	syFieldScriptWait(10);
	for(i = 0; i < 20; i++) {
		actor->scaleX -= toFIXED(3) / 256;
		actor->scaleY -= toFIXED(3) / 256;
		actor->y.fixed -= toFIXED(3) / 2;
		syWait(1);
		if(i == 10)
			fdScriptPlaySound(SE_DUST_PUFF);
		/* DEAD: the loop ends before tick 20, so the screen is never taken out
		   here; the ROM keeps the test and the transition. */
		if(i == 20) {
			FieldStateWork->transition =
				SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
			fdScriptFinishScreenTransition();
		}
	}
}

/* Standing on a roller carries the leader north or south as it rolls, a blue
   pillar one way and a pink drum the other, kept on its middle; 20 pixels from
   its centre he hops off the end. A drum of the mouse's room carries him off
   its south end and down to Steel's room. */
void scGabombaStatueRideRoller(void)
{
	ACTOR *actor, *roller;
	Sint32 speed;
	FIXED  d;
	Sint32 sign, x;
	FIXED  z, rollerZ, offset;

	if(GabombaStatueCogDirection == 0)
		return;
	actor = fdActorGet(syFieldPartyGetLeaderActor());
	speed = GabombaStatueCogDirection * toFIXED(0.5);
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
	x = FIELD_METATILE(actor->x.fixed);
	if(FIELD_METATILE(actor->z.fixed) != 15)
		return;
	if(x == 28) {
		scGabombaStatueEndRide(actor);
		syFieldScriptSetExitRequest(
			GABOMBA_STATUE_MOUSE_TO_GABOMBA_STATUE_STEEL_SOUTHEAST_A);
	} else if(x == 30) {
		scGabombaStatueEndRide(actor);
		syFieldScriptSetExitRequest(
			GABOMBA_STATUE_MOUSE_TO_GABOMBA_STATUE_STEEL_SOUTHEAST_B);
	}
}

/* Walking east or west off a sliding cog hops the leader to the next. */
void scGabombaStatueHopAcrossEastOrWest(void)
{
	Sint32 *speed = &GabombaStatueCogDirection;

	if(*speed != 0) {
		ACTOR *actor = fdActorGet(syFieldPartyGetLeaderActor());
		ACTOR *found = scGabombaStatueFindCogAhead(actor, SPRITE_RED_COG);
		Sint32 dx = *speed * toFIXED(16);

		if(found != NULL) {
			Uint16 facing = found->facing;

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
void scGabombaStatueHopAcrossNorthOrSouth(void)
{
	Sint32 *speed = &GabombaStatueCogDirection;

	if(*speed != 0) {
		ACTOR *actor = fdActorGet(syFieldPartyGetLeaderActor());
		ACTOR *found = scGabombaStatueFindCogAhead(actor, SPRITE_BLUE_PILLAR);
		Sint32 dz = *speed * toFIXED(16);
		Uint16 facing;

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

/* When Felix walks to the far west of the mouse's room, metatile (14, 10), a
   little mouse pops up, squeaks, and hops from cog to cog, showing the way
   across. It does so once. */
void scGabombaStatueMouseShowsTheWay(ACTOR *actor)
{
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdCameraSetSpeed(1);
	fdActorSetPos(ACTOR_MOUSE, 328, 296);
	syFieldScriptWait(30);
	fdActorFaceDirection(
		MainRoutineStateWork.leaderActorId, FACING_SOUTHEAST, 0);
	syFieldScriptWait(15);
	scFieldActorWatchAndSetScript(
		MainRoutineStateWork.leaderActorId, ACTOR_MOUSE);
	fdActorShowEmote(MainRoutineStateWork.leaderActorId,
		EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	syScriptCameraFollowActor(ACTOR_MOUSE, 1);
	syFieldScriptWait(30);
	fdActorGet(ACTOR_MOUSE)->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
	fdActorSetAnimation(ACTOR_MOUSE, 1);
	fdActorWalkToAndWait(ACTOR_MOUSE, 320, 240);
	fdActorWalkToAndWait(ACTOR_MOUSE, 304, 216);
	fdActorWalkToAndWait(ACTOR_MOUSE, 280, 216);
	fdActorWalkToAndWait(ACTOR_MOUSE, 280, 184);
	syFieldScriptWait(20);
	fdActorFaceActor(ACTOR_MOUSE, MainRoutineStateWork.leaderActorId, 0);
	syFieldScriptWait(10);
	fdScriptPlaySound(SE_VOICE_GIRL_C);
	{
		Sint32 emote = EMOTE_IN_FRONT | EMOTE_EXCLAMATION, ticks = 30;

		fdActorShowEmote(ACTOR_MOUSE, emote, ticks);
	}
	fdActorWalkToAndWait(ACTOR_MOUSE, 344, 184);
	fdActorWalkToAndWait(ACTOR_MOUSE, 344, 200);
	syFieldScriptWait(2);
	fdActorFaceDirection(ACTOR_MOUSE, FACING_WEST, 0);
	syFieldScriptWait(15);
	fdScriptPlaySound(SE_JUMP);
	fdActorGet(ACTOR_MOUSE)->velocityY = toFIXED(4);
	fdActorSetSpeed(ACTOR_MOUSE, 4);
	fdActorWalkToAndWait(ACTOR_MOUSE, 312, 200);
	fdScriptPlaySound(SE_JUMP);
	fdActorGet(ACTOR_MOUSE)->velocityY = toFIXED(6);
	fdActorSetSpeed(ACTOR_MOUSE, 4);
	fdActorWalkToAndWait(ACTOR_MOUSE, 312, 168);
	fdActorSetSpeed(ACTOR_MOUSE, 2);
	fdActorWalkToAndWait(ACTOR_MOUSE, 328, 152);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	syFieldScriptWait(15);
	fdScriptPlaySound(SE_JUMP);
	fdActorGet(ACTOR_MOUSE)->velocityY = toFIXED(6);
	fdActorSetSpeed(ACTOR_MOUSE, 4);
	fdActorWalkToAndWait(ACTOR_MOUSE, 360, 152);
	syFieldScriptWait(15);
	fdScriptPlaySound(SE_JUMP);
	fdActorGet(ACTOR_MOUSE)->velocityY = toFIXED(6);
	fdActorSetSpeed(ACTOR_MOUSE, 2);
	fdActorWalkToAndWait(ACTOR_MOUSE, 360, 184);
	fdActorSetSpeed(ACTOR_MOUSE, 2);
	fdActorWalkToAndWait(ACTOR_MOUSE, 376, 184);
	fdActorWalkToAndWait(ACTOR_MOUSE, 392, 200);
	fdActorWalkToAndWait(ACTOR_MOUSE, 408, 184);
	syFieldScriptWait(15);
	fdScriptPlaySound(SE_JUMP);
	fdActorGet(ACTOR_MOUSE)->velocityY = toFIXED(6);
	fdActorSetSpeed(ACTOR_MOUSE, 4);
	fdActorWalkToAndWait(ACTOR_MOUSE, 440, 184);
	fdActorSetPos(ACTOR_MOUSE, 0, 0);
	fdScriptSetFlag(FLAG_GABOMBA_MOUSE_SEEN);
	fdScriptEnd();
}

void scGabombaStatueSendCharge(
	Sint32 id, Sint32 colorMode, Sint32 statueId, Sint32 angle);

/* How the charges reached the statue in the middle (scGabombaStatueRunCharges),
   which decides how the ceremony outside goes on. */
typedef enum CHARGE_ARRIVAL {
	/* a blue and a red charge at the same moment */
	CHARGE_ARRIVAL_TOGETHER = 0,
	CHARGE_ARRIVAL_BLUE_FIRST = 1, /* a blue one alone, or before the red */
	CHARGE_ARRIVAL_RED_FIRST = 2,  /* a red one alone, or before the blue */
	CHARGE_ARRIVAL_NONE = 3
} CHARGE_ARRIVAL;

/* Takes charge id off the map, stopped. */
void scGabombaStatueResetCharge(Sint32 id)
{
	fdActorSetPos(id, 0, 0);
	scFieldActorStop(id);
	fdActorSetAnimation(id, 1);
}

/* Sends the four charges circling out of the orb statues, the blue statue's
   green-tinted and the red's red-tinted. */
void scGabombaStatueReleaseCharges(void)
{
	scGabombaStatueResetCharge(ACTOR_CHARGE_A);
	scGabombaStatueResetCharge(ACTOR_CHARGE_B);
	scGabombaStatueResetCharge(ACTOR_CHARGE_C);
	scGabombaStatueResetCharge(ACTOR_CHARGE_D);
	scGabombaStatueSendCharge(
		ACTOR_CHARGE_A, SPRITE_COLOR_GREEN_TINT, ACTOR_BLUE_ORB_STATUE, 0);
	scGabombaStatueSendCharge(ACTOR_CHARGE_B, SPRITE_COLOR_GREEN_TINT,
		ACTOR_BLUE_ORB_STATUE, DEGtoANG(180));
	scGabombaStatueSendCharge(ACTOR_CHARGE_C, SPRITE_COLOR_RED_TINT,
		ACTOR_RED_ORB_STATUE, DEGtoANG(90));
	scGabombaStatueSendCharge(ACTOR_CHARGE_D, SPRITE_COLOR_RED_TINT,
		ACTOR_RED_ORB_STATUE, TRIG_TURN * 3 / 4);
}

/* The offering's clock has run out: Akafubu offers the jewel below ("O Great
   Gabomba! Please accept this, our humble gift!"), the orb statues send their
   charges along the channel, and how they reach the statue in the middle
   decides how the ceremony outside goes. */
void scGabombaStatueHearTheOffering(void)
{
	Sint32 result;

	fdScriptSetFlag(FLAG_OFFERING_HEARD);
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdScriptPlaySound(SOUND_FADE_OUT_BGM);
	scGabombaStatueStopClock();
	syFieldScriptWait(40);
	syFieldScriptSetMessageId(MsgGabombaAcceptOffering);
	/* "O Great Gabomba! Please accept this, our humble gift!" */
	fdScriptShowMessage(ACTOR_AKAFUBU, 0);
	fdCameraMoveTo(392, CAMERA_KEEP, 256, 1);
	fdCameraWaitForArrival();
	fdScriptPlaySound(SE_SHIMMER);
	syWait(1);
	fdPaletteFadeApplySource(PALETTE_TINT_NONE, 0);
	fdPaletteFadeApplyTarget(RGB_WHITE, 0);
	fdPaletteFadeInterpolate(40);
	syWait(80);
	fdActorSetSpriteFlags(fdActorGet(MainRoutineStateWork.leaderActorId), 0);
	scFieldActorSetSpriteColor(
		fdActorGet(MainRoutineStateWork.leaderActorId), SPRITE_COLOR_HIDDEN);
	scGabombaStatueReleaseCharges();
	syWait(1);
	fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, 0);
	fdPaletteFadeInterpolate(10);
	syWait(20);
	fdScriptPlaySound(SE_PSYNERGY_START);
	scFieldActorSetSpriteColor(
		fdActorGet(ACTOR_BLUE_ORB_STATUE), SPRITE_COLOR_WHITE);
	scFieldActorSetSpriteColor(
		fdActorGet(ACTOR_RED_ORB_STATUE), SPRITE_COLOR_WHITE);
	syWait(8);
	scFieldActorSetSpriteColor(
		fdActorGet(ACTOR_RED_ORB_STATUE), SPRITE_COLOR_BGR);
	scFieldActorSetSpriteColor(
		fdActorGet(ACTOR_BLUE_ORB_STATUE), SPRITE_COLOR_NORMAL);
	syWait(8);
	fdScriptPlaySound(MUS_RITUALS);
	result = scGabombaStatueRunCharges(ACTOR_CHARGE_A, 1);
	fdActorTableSnapshot();
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	switch(result) {
	case CHARGE_ARRIVAL_NONE:
		syFieldScriptSetExitRequest(
			GABOMBA_STATUE_ARROWS_TO_KIBOMBO_STATUE_GROUNDS_CEREMONY_NORTH_C);
		break;
	case CHARGE_ARRIVAL_BLUE_FIRST:
		syFieldScriptSetExitRequest(
			GABOMBA_STATUE_ARROWS_TO_KIBOMBO_STATUE_GROUNDS_CEREMONY_NORTH_D);
		break;
	case CHARGE_ARRIVAL_RED_FIRST:
		syFieldScriptSetExitRequest(
			GABOMBA_STATUE_ARROWS_TO_KIBOMBO_STATUE_GROUNDS_CEREMONY_NORTH_E);
		break;
	case CHARGE_ARRIVAL_TOGETHER:
		syFieldScriptSetExitRequest(
			GABOMBA_STATUE_ARROWS_TO_GABOMBA_STATUE_FACE_EAST_A_B);
		break;
	}
}

extern CUE GabombaStatueCues[];
extern CUE GabombaStatueBoneArmletChestCues[];
extern CUE GabombaStatueElixirChestCues[];
extern CUE GabombaStatueSteelCues[];
extern CUE GabombaStatueMouseCues[];
extern CUE GabombaStatueCogsCues[];
extern CUE GabombaStatueArrowsCues[];
extern CUE GabombaStatuePinionCues[];

const Uint8 *scGetCues(void)
{
	Sint16		 scene = MainRoutineStateWork.next;
	const Uint8 *table = GabombaStatueCues;

	if(scene == SCENE_GABOMBA_STATUE_BONE_ARMLET_CHEST)
		table = GabombaStatueBoneArmletChestCues;
	else if(scene == SCENE_GABOMBA_STATUE_ELIXIR_CHEST)
		table = GabombaStatueElixirChestCues;
	else if(scene == SCENE_GABOMBA_STATUE_STEEL)
		table = GabombaStatueSteelCues;
	else if(scene == SCENE_GABOMBA_STATUE_MOUSE)
		table = GabombaStatueMouseCues;
	else if(scene == SCENE_GABOMBA_STATUE_COGS)
		table = GabombaStatueCogsCues;
	else if(scene == SCENE_GABOMBA_STATUE_ARROWS)
		table = GabombaStatueArrowsCues;
	else if(scene == SCENE_GABOMBA_STATUE_PINION)
		table = GabombaStatuePinionCues;
	return table;
}

/* Felix walks to the top of the ladder at x, z, turns his back and climbs down
   it, and the screen closes on him as he leaves for the room below by the
   ladder's exit. */
void scGabombaStatueClimbDownLadder(Sint32 x, Sint32 z)
{
	FIELD_STATE *state = RamHeapSlots->fieldState;
	ACTOR		*actor = fdActorGet(MainRoutineStateWork.leaderActorId);

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	actor->motionFlags = 0;
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 1);
	fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, x, z);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_NORTH, 0);
	scFieldActorWaitForScriptEndById(MainRoutineStateWork.leaderActorId);
	fdActorSetSpriteFlags(fdActorGet(MainRoutineStateWork.leaderActorId), 0);
	fdActorSetAnimation(
		MainRoutineStateWork.leaderActorId, ANIM_LEADER_CLIMB_DOWN_FROM_TOP);
	scFieldActorSetMoveTarget(actor, x, -12, z);
	scFieldActorWaitForMove(actor);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_LEADER_CLIMB);
	fdScriptPlaySound(SE_MAP_EXIT);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	syFieldScriptSetExitRequest(state->stepCue);
}

/* A ladder down that Felix takes only when he walks into it: from the squares
   beside it on row, pushing east or west, or from the one south of it, pushing
   north. Brushing past it does nothing. */
#define DEFINE_GABOMBA_STATUE_STEP_WHEN_HELD( \
	name, row, westColumn, eastColumn, southColumn, southRow, holeX, holeZ) \
	void name(void) \
	{ \
		ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId); \
		Sint32 x = FIELD_METATILE(leader->x.fixed); \
		Sint32 z = FIELD_METATILE(leader->z.fixed); \
\
		if(z == row) { \
			if(x == westColumn && (PerWork.data & KEY_RIGHT) == 0) \
				return; \
			if(x == eastColumn && (PerWork.data & KEY_LEFT) == 0) \
				return; \
		} else if(x == southColumn && z == southRow && \
			(PerWork.data & KEY_UP) == 0) { \
			return; \
		} \
		scGabombaStatueClimbDownLadder(holeX, holeZ); \
	}

/* Felix climbs down the ladder in the north of the room of corridors, where the
   Mimic and the Bone Armlet's chest are, at metatile (19, 11), back to the hall
   of pinions below, when he walks into it from the west, the east or the
   south. */
DEFINE_GABOMBA_STATUE_STEP_WHEN_HELD(
	scGabombaStatueClimbDownBoneArmletChestLadder, 11, 18, 20, 19, 12, 312, 180)

/* The same for a ladder down with a square to walk in from on each of its four
   sides. */
#define DEFINE_GABOMBA_STATUE_STEP_WHEN_HELD_FOUR(name, column, northRow, \
	southRow, row, westColumn, eastColumn, holeX, holeZ) \
	void name(void) \
	{ \
		ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId); \
		Sint32 x = FIELD_METATILE(leader->x.fixed); \
		Sint32 z = FIELD_METATILE(leader->z.fixed); \
\
		if(x == column) { \
			if(z == northRow && (PerWork.data & KEY_DOWN) == 0) \
				return; \
			if(z == southRow && (PerWork.data & KEY_UP) == 0) \
				return; \
		} else if(z == row) { \
			if(x == westColumn && (PerWork.data & KEY_RIGHT) == 0) \
				return; \
			if(x == eastColumn && (PerWork.data & KEY_LEFT) == 0) \
				return; \
		} \
		scGabombaStatueClimbDownLadder(holeX, holeZ); \
	}

/* Felix climbs down the ladder in the north of the room of the rope, the stakes
   and the flashing cog, at metatile (29, 12), back to the room of corridors
   below, when he walks into it from any side. */
DEFINE_GABOMBA_STATUE_STEP_WHEN_HELD_FOUR(
	scGabombaStatueClimbDownElixirChestLadder, 29, 11, 13, 12, 28, 30, 472, 196)

/* Felix climbs down the ladder in the north-east of Steel's room, at metatile
   (33, 7), to the north-east of the room of the rope and the flashing cog
   below, when he walks into it from the west or the south. */
void scGabombaStatueClimbDownSteelNortheastLadder(void)
{
	ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	Sint32 x = FIELD_METATILE(leader->x.fixed);
	Sint32 z = FIELD_METATILE(leader->z.fixed);

	if(z == 7) {
		if(x == 32 && (PerWork.data & KEY_RIGHT) == 0)
			return;
	} else if(x == 33 && z == 8 && (PerWork.data & KEY_UP) == 0) {
		return;
	}
	scGabombaStatueClimbDownLadder(536, 112);
}

/* Felix climbs down the ladder in the west of Steel's room, at metatile (6,
   13), to the west of the room of the rope and the flashing cog below, when he
   walks into it from any side. */
void scGabombaStatueClimbDownSteelWestLadder(void)
{
	ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	Sint32 x = FIELD_METATILE(leader->x.fixed);
	Sint32 z = FIELD_METATILE(leader->z.fixed);

	if(z == 13) {
		if(x == 5 && (PerWork.data & KEY_RIGHT) == 0)
			return;
		if(x == 7 && (PerWork.data & KEY_LEFT) == 0)
			return;
	} else if(x == 6) {
		if(z == 12 && (PerWork.data & KEY_DOWN) == 0)
			return;
		if(z == 14 && (PerWork.data & KEY_UP) == 0)
			return;
	}
	scGabombaStatueClimbDownLadder(104, 208);
}

/* Felix climbs down the ladder in the east of Steel's room, at metatile (42,
   13), to the east of the room of the rope and the flashing cog below, when he
   walks into it from the north, the west or the south. */
void scGabombaStatueClimbDownSteelEastLadder(void)
{
	ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	Sint32 x = FIELD_METATILE(leader->x.fixed);
	Sint32 z = FIELD_METATILE(leader->z.fixed);

	if(x == 42) {
		if(z == 12 && (PerWork.data & KEY_DOWN) == 0)
			return;
		if(z == 14 && (PerWork.data & KEY_UP) == 0)
			return;
#if FIX_GABOMBA_STEEL_EAST_LADDER_CELL
	} else if(x == 41 && z == 13 && (PerWork.data & KEY_RIGHT) == 0) {
#else
		/* BUG: the metatile tested is east of the hole, where the map has no
		   hole metatile; from the one west of it, (41, 13), the leader drops in
		   without pushing east. */
	} else if(x == 43 && z == 13 && (PerWork.data & KEY_RIGHT) == 0) {
#endif
		return;
	}
	scGabombaStatueClimbDownLadder(680, 208);
}

/* Felix climbs down the ladder in the north of the mouse's room, at metatile
   (24, 7), to Steel's room below, when he walks into it from the west, the east
   or the south. */
DEFINE_GABOMBA_STATUE_STEP_WHEN_HELD(
	scGabombaStatueClimbDownMouseNorthLadder, 7, 23, 25, 24, 8, 392, 112)

/* Felix climbs down the ladder in the west of the mouse's room, at metatile (6,
   9), to the west of Steel's room below, when he walks into it from the west,
   the east or the south. */
DEFINE_GABOMBA_STATUE_STEP_WHEN_HELD(
	scGabombaStatueClimbDownMouseWestLadder, 9, 5, 7, 6, 10, 104, 146)

/* Felix climbs down the ladder in the east of the mouse's room, beside its east
   blue cog at metatile (42, 9), to the east of Steel's room below, when he
   walks into it from the east or the south. */
void scGabombaStatueClimbDownMouseEastLadder(void)
{
	ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	Sint32 x = FIELD_METATILE(leader->x.fixed);
	Sint32 z = FIELD_METATILE(leader->z.fixed);

	if(x == 42 && z == 10 && (PerWork.data & KEY_UP) == 0)
		return;
	if(x == 43 && z == 9 && (PerWork.data & KEY_LEFT) == 0)
		return;
	scGabombaStatueClimbDownLadder(680, 146);
}

/* Felix climbs down the ladder in the middle of the room below the small idol's
   chamber, at metatile (27, 18), back to the mouse's room, when he walks into
   it from any side. */
DEFINE_GABOMBA_STATUE_STEP_WHEN_HELD_FOUR(
	scGabombaStatueClimbDownCogsLadder, 27, 17, 19, 18, 26, 28, 440, 288)

/* Felix climbs down the ladder in the north-east of the room of the arrow
   panels, at metatile (29, 9), back to the small idol's chamber below, when he
   walks into it from the west or the south. */
void scGabombaStatueClimbDownArrowsLadder(void)
{
	ACTOR		*actor = fdActorGet(MainRoutineStateWork.leaderActorId);
	Sint32		 x = FIELD_METATILE(actor->x.fixed);
	Sint32		 z = FIELD_METATILE(actor->z.fixed);
	ACTOR		*leader;
	FIELD_STATE *state;

	if(x == 28 && z == 9 && !(PerWork.data & KEY_RIGHT))
		return;
	if(x == 29 && z == 10 && !(PerWork.data & KEY_UP))
		return;
	state = RamHeapSlots->fieldState;
	leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	leader->motionFlags = 0;
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 1);
	fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 472, 144);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_NORTH, 0);
	scFieldActorWaitForScriptEndById(MainRoutineStateWork.leaderActorId);
	fdActorSetSpriteFlags(fdActorGet(MainRoutineStateWork.leaderActorId), 0);
	fdActorSetAnimation(
		MainRoutineStateWork.leaderActorId, ANIM_LEADER_CLIMB_DOWN_FROM_TOP);
	scFieldActorSetMoveTarget(leader, 472, 0, 156);
	scFieldActorWaitForMove(leader);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_LEADER_CLIMB);
	fdScriptPlaySound(SE_MAP_EXIT);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	syFieldScriptSetExitRequest(state->stepCue);
}

/* Felix walks into the gap between two turning pinions, which grind and pull
   him through to (x, z) on the other side. */
#define DEFINE_GABOMBA_STATUE_WALK_LEADER_TO(name, x, z) \
	void name(ACTOR *actor) \
	{ \
		fdScriptStart(); \
		syFieldScriptEndLastingPsynergy(0); \
		fdScriptPlaySound(SE_GABOMBA_STATUE_HOLE); \
		fdActorSetSpeedAndAcceleration( \
			MainRoutineStateWork.leaderActorId, 1.6, 1.6); \
		fdActorMoveToAndWait(MainRoutineStateWork.leaderActorId, x, z); \
		fdScriptEnd(); \
	}

/* Walking into the gap between the pinions in the south-west of the hall of
   pinions, metatile (9, 28), Felix is pulled through them to the north. */
DEFINE_GABOMBA_STATUE_WALK_LEADER_TO(
	GabombaStatuePinionsCarryLeaderNorth, 152, 424)

/* Once the cogs turn the other way, the pinions in the south-west of the hall
   of pinions throw Felix back out to the south when he walks into their gap,
   and he lands with a rattle, leaning back. */
void scGabombaStatuePinionsThrowLeaderSouth(void)
{
	ACTOR *actor = fdActorGet(MainRoutineStateWork.leaderActorId);

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdScriptPlaySound(SE_GABOMBA_STATUE_HOLE);
	actor->motionFlags = 0;
	fdActorSetSpeedAndAcceleration(
		MainRoutineStateWork.leaderActorId, 1.6, 1.6);
	fdActorMoveToAndWait(MainRoutineStateWork.leaderActorId, 152, 488);
	actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	syFieldScriptWait(8);
	fdScriptPlaySound(SE_RATTLE);
	fdActorSetAnimation(
		MainRoutineStateWork.leaderActorId, ANIM_LEADER_LEAN_BACK);
	syFieldScriptWait(20);
	fdScriptEnd();
}

/* Walking into the gap between the pinions in the north of the room of the
   flashing cog, metatile (23, 9), Felix is pulled through them to the south. */
DEFINE_GABOMBA_STATUE_WALK_LEADER_TO(
	GabombaStatueElixirPinionsCarryLeaderSouth, 376, 200)

/* Once the cogs turn the other way, the pinions in the south of the room of the
   flashing cog, metatile (24, 22), pull Felix through them to the north. */
DEFINE_GABOMBA_STATUE_WALK_LEADER_TO(
	GabombaStatueElixirPinionsCarryLeaderNorth, 376, 136)

/* Walking into the gap between the pinions in the room below the small idol's
   chamber, metatile (27, 12), Felix is pulled through them to the south. */
DEFINE_GABOMBA_STATUE_WALK_LEADER_TO(
	GabombaStatueCogsPinionsCarryLeaderSouth, 440, 248)

/* Once the cogs turn the other way, the pinions east of the first in the room
   below the small idol's chamber, metatile (35, 13), pull Felix through them to
   the north. */
DEFINE_GABOMBA_STATUE_WALK_LEADER_TO(
	GabombaStatueCogsPinionsCarryLeaderNorth, 440, 184)

/* Draws the room's ladder-top blocks, or hides them. */
void scGabombaStatueSetLadderTopBlocksDrawn(Sint32 show)
{
	if(MainRoutineStateWork.next == SCENE_GABOMBA_STATUE_BONE_ARMLET_CHEST)
		scActorsSetDrawn(show, ACTOR_BONE_ARMLET_LADDER_TOP_BLOCK, 1);
	else if(MainRoutineStateWork.next == SCENE_GABOMBA_STATUE_ELIXIR_CHEST)
		scActorsSetDrawn(show, ACTOR_ELIXIR_WEST_LADDER_TOP_BLOCK, 2);
}

extern Sint16 GabombaStatueStakes[];

/* Unused: draws the ladder-top blocks. */
UNUSED void scGabombaStatueDrawLadderTopBlocks(void)
{
	scGabombaStatueSetLadderTopBlocksDrawn(1);
	fdScriptSetFlag(FLAG_LADDER_TOP_BLOCKS_DRAWN);
}

/* Hides the ladder-top blocks. */
void scGabombaStatueHideLadderTopBlocks(void)
{
	scGabombaStatueSetLadderTopBlocksDrawn(0);
	fdScriptClearFlag(FLAG_LADDER_TOP_BLOCKS_DRAWN);
}

#include "shared/actors_set_hidden.c"

/* Pound on one of the two stakes either side of the flashing cog in the south
   of its room, metatiles (20, 30) and (24, 30), drives it into the floor. Once
   both are down, the cogs turn the other way. */
void scGabombaStatuePoundStake(Sint32 landed, Sint32 frame)
{
	fdPoundSinkTarget(GabombaStatueStakes, landed, frame);
	if(fdScriptGetFlag(FLAG_GABOMBA_FIRST_STAKE_POUNDED) &&
		fdScriptGetFlag(FLAG_GABOMBA_SECOND_STAKE_POUNDED))
		FieldStateWork->raisedCue = RAISED_COGS_TURN_BACK;
}

/* When the second stake by the flashing cog is Pounded, the view pans to the
   cog between the two stakes, the cogs stop with a clank, the flashing cog bobs
   up and settles, and every cog starts turning the other way. */
void scGabombaStatueCogsTurnBack(void)
{
	ACTOR *actor = fdActorGet(ACTOR_FLASHING_COG);

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	syScriptCameraGetActor()->motionFlags = 0;
	fdCameraSetSpeed(0.6);
	fdCameraMoveTo(360, 0, 496, 1);
	fdCameraWaitForArrival();
	fdScriptPlaySound(SE_GABOMBA_STATUE_COGS);
	scGabombaStatueTurnCogs(0);
	syFieldScriptWait(40);
	fdScriptPlaySound(SE_LEMURIAN_SHIP_LIFT_FAST);
	actor->motionFlags = 0;
	actor->y.fixed = toFIXED(8);
	syFieldScriptWait(10);
	actor->y.fixed = 0;
	syFieldScriptWait(10);
	scGabombaStatueTurnCogs(-1);
	fdScriptSetFlag(FLAG_GABOMBA_COGS_TURNED_BACK);
	syFieldScriptWait(20);
	fdScriptEnd();
}

/* Pound on the arrows room's stake raises the channel's blocks. It passes
   scGabombaStatueRaiseBlocks no tick, so how far the stake sinks before the
   fist lands is left to whatever that argument holds. */
void scGabombaStatuePoundArrowsStake(Sint32 landed)
{
	scGabombaStatueRaiseBlocks(landed);
	if(landed == POUND_STAGE_LANDED)
		FieldStateWork->raisedCue = RAISED_STAKE_STANDS_UP;
}

/* Once the blocks have risen, the Pounded stake stands up again to a rising
   chime. */
void scGabombaStatueStakeStandsUp(void)
{
	ACTOR *actor = fdActorGet(ACTOR_ARROWS_STAKE);

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	syFieldScriptWait(20);
	fdScriptPlaySound(SE_RISING_CHIMES);
	fdActorSelectAnimation(actor, 5);
	actor->scaleY = toFIXED(1);
	fdScriptEnd();
}

/* Each tick: once the offering's clock has run out, the offering is made. */
void scGabombaStatueCheckOfferingClock(void)
{
	if(scGabombaStatueGetClock() == 0)
		FieldStateWork->raisedCue = RAISED_OFFERING_HEARD;
}

/* Hides actor id (SPRITE_COLOR_HIDDEN) and fixes it in place, sorted behind the
   others. */
void scGabombaStatueHideActor(Sint32 id)
{
	fdActorDisableById(id);
	scFieldActorSetSpriteColor(fdActorGet(id), SPRITE_COLOR_HIDDEN);
	scFieldActorSetSpritePriority(id, 1);
	fdActorGet(id)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
}

/* Coming back to the same spot hides the ladder-top blocks again unless they
   were drawn when the party left; otherwise they are drawn. */
void scGabombaStatueRestoreLadderTopBlocks(void)
{
	if(fdScriptGetFlag(FLAG_KEEP_PARTY_POSITION)) {
		if(!fdScriptGetFlag(FLAG_LADDER_TOP_BLOCKS_DRAWN))
			scGabombaStatueHideLadderTopBlocks();
	} else {
		fdScriptSetFlag(FLAG_LADDER_TOP_BLOCKS_DRAWN);
	}
}

/* Gives a room's ladder-top blocks SPRITE_COLOR_HIDDEN and holds them 36 pixels
   up. */
#define DEFINE_GABOMBA_STATUE_HIDE_LADDER_TOP_BLOCKS(name, first, count) \
	void name(void) \
	{ \
		Uint32 i; \
\
		for(i = 0; i < count; i++) { \
			Sint32 id = i + first; \
			ACTOR *actor = fdActorGet(id); \
\
			scGabombaStatueHideActor(id); \
			actor->motionFlags = 0; \
			actor->y.fixed = toFIXED(36); \
		} \
		scGabombaStatueRestoreLadderTopBlocks(); \
	}

/* The Bone Armlet chest's room, as it is set up: its one ladder-top block. */
DEFINE_GABOMBA_STATUE_HIDE_LADDER_TOP_BLOCKS(
	scGabombaStatueHideBoneArmletChestLadderTopBlock,
	ACTOR_BONE_ARMLET_LADDER_TOP_BLOCK, 1)

/* The Elixir chest's room, as it is set up: its two ladder-top blocks. */
DEFINE_GABOMBA_STATUE_HIDE_LADDER_TOP_BLOCKS(
	scGabombaStatueHideElixirChestLadderTopBlocks,
	ACTOR_ELIXIR_WEST_LADDER_TOP_BLOCK, 2)

/* Each tick: while a menu or Psynergy is up, the cogs and the rope posts stand
   still. */
void scGabombaStatuePauseCogsInMenus(void)
{
	Sint32		 step = ANIM_STEP_NORMAL;
	Sint32		 open = 0;
	FIELD_STATE *state = RamHeapSlots->fieldState;
	Sint32		 i;

	if(fdScriptGetFlag(FLAG_MENU_OPEN) || state->psynergy != 0) {
		open = 1;
		step = 0;
	}
	i = ACTOR_FIRST_PLACED;
	do {
		{
			ACTOR *actor = fdActorGet(i);

			if(actor != NULL && actor->spriteStorageKind == ACTOR_SPRITE_ONE) {
				SPR_ANIMATION *part = ((SPR *)actor->sprite)->entries[0];

				if(part != NULL) {
					switch(part->animationId) {
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

/* The map shapes of the cogs' metatiles, from COG_SHAPE_FIRST: the scene writes
   GabombaStatueCogShapes over the map's own entries there. */
enum {
	COG_SHAPE_GREEN_COG = 116, /* the green cog's own metatile */
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
	COG_SHAPE_END, /* the metatiles beyond a red cog's or a roller's ends */
	COG_SHAPE_FIRST = COG_SHAPE_GREEN_COG,
	COG_SHAPE_COUNT = COG_SHAPE_END + 1 - COG_SHAPE_FIRST
};

/* A cog's metatile word: its collision code (the metatile behavior), the
   attribute bits above it and the flags of its metatile tiles, ORed into the
   map's. */
#define COG_METATILE(behavior, attributes, pictureFlags) \
	((Uint32)(attributes) << 24 | (behavior) << 16 | (pictureFlags))

/* Gives a metatile under a cog its shape on both layers and adds bits to both
   its shape words. */
#define MARK(at, shape, bits) \
	(metatile = (at), layerA[metatile] = (shape), layerB[metatile] = (shape), \
		word = &shapesA[metatile], *word |= (bits), word = &shapesB[metatile], \
		*word |= (bits))

/* Gives the metatiles under every cog of the room their shapes and the metatile
   behaviors that carry the leader with it, and fixes the cogs in place. */
void scGabombaStatueDrawCogMetatiles(void)
{
	Uint32	  *dest = (Uint32 *)MapGridShapes + COG_SHAPE_FIRST;
	Uint32	  *src = GabombaStatueCogShapes;
	MAP_STATE *map = RamHeapSlots->mapGrid;
	Uint8	  *layerA = map->layers[0].terrainIds;
	Uint8	  *layerB = map->layers[1].terrainIds;
	Uint32	  *shapesA = (Uint32 *)map->layers[0].metatiles;
	Uint32	  *shapesB = (Uint32 *)map->layers[1].metatiles;
	ACTOR	  *actor;
	Sint16	  *ticks;
	Uint32	  *word;
	Uint32	   i;
	Sint32	   x, z, metatile, found;

	for(i = 0; i < COG_SHAPE_COUNT; i++) {
		*dest++ = *src++;
	}
	for(i = ACTOR_FIRST_PLACED; (Sint32)i < FIELD_TREASURE_ACTOR; i++) {
		actor = fdActorGetById(i);
		if(actor == 0)
			continue;
		if(actor->spriteStorageKind != ACTOR_SPRITE_ONE)
			continue;
		ticks = (Sint16 *)((SPR *)actor->sprite)->entries[0];
		if(ticks == 0)
			continue;
		found = 0;
		x = FIELD_METATILE(actor->x.fixed);
		z = FIELD_METATILE(actor->z.fixed);
		switch(*ticks) {
		case SPRITE_GREEN_COG:
			metatile = x + (z << 7);
			layerA[metatile] = COG_SHAPE_GREEN_COG;
			layerB[metatile] = COG_SHAPE_TOP;
			word = &shapesA[metatile];
			*word |=
				COG_METATILE(METATILE_BEHAVIOR_GREEN_COG, MAP_METATILE_JUMP, 0);
			word = &shapesB[metatile];
			*word |=
				COG_METATILE(METATILE_BEHAVIOR_GREEN_COG, MAP_METATILE_JUMP, 0);
			found = 1;
			break;
		case SPRITE_BLUE_COG:
			MARK(x + (z << 7) - 129, COG_SHAPE_NORTHWEST,
				COG_METATILE(METATILE_BEHAVIOR_BLUE_COG, 0, 0));
			MARK(x + (z << 7) - 128, COG_SHAPE_TOP,
				COG_METATILE(METATILE_BEHAVIOR_BLUE_COG, MAP_METATILE_JUMP, 0));
			MARK(x + (z << 7) - 127, COG_SHAPE_NORTHEAST,
				COG_METATILE(METATILE_BEHAVIOR_BLUE_COG, 0, 0));
			MARK(x + (z << 7) - 1, COG_SHAPE_TOP,
				COG_METATILE(METATILE_BEHAVIOR_BLUE_COG, MAP_METATILE_JUMP, 0));
			MARK(x + (z << 7), COG_SHAPE_TOP,
				COG_METATILE(METATILE_BEHAVIOR_BLUE_COG, 0, 0));
			MARK(x + (z << 7) + 1, COG_SHAPE_TOP,
				COG_METATILE(METATILE_BEHAVIOR_BLUE_COG, MAP_METATILE_JUMP, 0));
			MARK(x + (z << 7) + 127, COG_SHAPE_SOUTHWEST,
				COG_METATILE(METATILE_BEHAVIOR_BLUE_COG, 0, 0));
			MARK(x + (z << 7) + 128, COG_SHAPE_TOP,
				COG_METATILE(METATILE_BEHAVIOR_BLUE_COG, MAP_METATILE_JUMP, 0));
			MARK(x + (z << 7) + 129, COG_SHAPE_SOUTHEAST,
				COG_METATILE(METATILE_BEHAVIOR_BLUE_COG, 0, 0));
			found = 1;
			break;
		case SPRITE_RED_COG:
			MARK(x + (z << 7) - 2, COG_SHAPE_END,
				COG_METATILE(METATILE_BEHAVIOR_COG_END, 0, 0));
			MARK(x + (z << 7) - 1, COG_SHAPE_WEST,
				COG_METATILE(
					METATILE_BEHAVIOR_RED_COG, 0, METATILE_DRAWS_ACTOR(2, 0)));
			MARK(x + (z << 7), COG_SHAPE_TOP,
				COG_METATILE(METATILE_BEHAVIOR_RED_COG, MAP_METATILE_JUMP,
					METATILE_DRAWS_ACTOR(1, 1)));
			MARK(x + (z << 7) + 1, COG_SHAPE_EAST,
				COG_METATILE(
					METATILE_BEHAVIOR_RED_COG, 0, METATILE_DRAWS_ACTOR(2, 0)));
			MARK(x + (z << 7) + 2, COG_SHAPE_END,
				COG_METATILE(METATILE_BEHAVIOR_COG_END, 0, 0));
			found = 1;
			break;
		case SPRITE_PINK_DRUM:
			MARK(x + (z << 7) - 256, COG_SHAPE_END,
				COG_METATILE(METATILE_BEHAVIOR_COG_END, 0, 0));
			MARK(x + (z << 7) - 128, COG_SHAPE_NORTH,
				COG_METATILE(
					METATILE_BEHAVIOR_ROLLER, 0, METATILE_DRAWS_ACTOR(2, 0)));
			MARK(x + (z << 7), COG_SHAPE_TOP,
				COG_METATILE(
					METATILE_BEHAVIOR_ROLLER, 0, METATILE_DRAWS_ACTOR(2, 0)));
			MARK(x + (z << 7) + 128, COG_SHAPE_SOUTH,
				COG_METATILE(
					METATILE_BEHAVIOR_ROLLER, 0, METATILE_DRAWS_ACTOR(2, 0)));
			MARK(x + (z << 7) + 256, COG_SHAPE_PINK_DRUM_SOUTH_END,
				COG_METATILE(METATILE_BEHAVIOR_PINK_DRUM_SOUTH_END,
					MAP_METATILE_JUMP, 0));
			found = 1;
			break;
		case SPRITE_BLUE_PILLAR:
			MARK(x + (z << 7) - 256, COG_SHAPE_END,
				COG_METATILE(METATILE_BEHAVIOR_COG_END, 0, 0));
			MARK(x + (z << 7) - 128, COG_SHAPE_NORTH,
				COG_METATILE(
					METATILE_BEHAVIOR_ROLLER, 0, METATILE_DRAWS_ACTOR(2, 0)));
			MARK(x + (z << 7), COG_SHAPE_TOP,
				COG_METATILE(
					METATILE_BEHAVIOR_ROLLER, 0, METATILE_DRAWS_ACTOR(2, 0)));
			MARK(x + (z << 7) + 128, COG_SHAPE_SOUTH,
				COG_METATILE(
					METATILE_BEHAVIOR_ROLLER, 0, METATILE_DRAWS_ACTOR(2, 0)));
			MARK(x + (z << 7) + 256, COG_SHAPE_END,
				COG_METATILE(METATILE_BEHAVIOR_COG_END, 0, 0));
			found = 1;
			break;
		case SPRITE_BROWN_COG:
			found = 1;
			break;
		}
		if(found) {
			actor->collisionFlags = 0;
			actor->motionFlags = 0;
			actor->priorityFlags = 0;
			fdActorSetSpriteFlags(actor, 0);
		}
	}
	sySetTickEvent(scGabombaStatuePauseCogsInMenus, EVENT_PASS_TICK);
}

/* Blends the room's layers by how far south the camera is: from 232 pixels, a
   step of the blend's sixteen every 8 pixels. */
void scGabombaStatueFadeByDepth(void)
{
	Sint32 blend;
	ACTOR *camera = FieldStateWork->camera;
	Sint32 level = camera->z.fixed;

	level = FIXEDtoINT(level - toFIXED(232)) >> 3;
	CLAMP(level, 0, BLEND_WEIGHT_MAX);
	blend = ((BLEND_WEIGHT_MAX - level) << 8) + level;
	REG_BLDALPHA = blend;
}

/* Turns cog id by direction, its animation timers set to value. */
void scGabombaStatueSetCogTurn(Sint32 id, Sint32 direction, Sint32 value)
{
	ACTOR *actor = fdActorGet(id);
	void  *animation = actor->sprite;

	fdActorSelectAnimation(
		actor, GabombaStatueCogAnimations[(direction + 1) & 3]);
	actor->retainSpriteCache = ACTOR_COMPOSE_OFF_SCREEN;
	fdSpriteSetAnimationTimers(animation, value);
}

/* Sets the cogs' direction and turns every cog of the room that way. */
void scGabombaStatueTurnCogs(Sint32 direction)
{
	Sint16 scene;

	GabombaStatueCogDirection = direction;
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
			fdMapGridCopyMetatileAttributes(23, 21, 1, 3, 23, 9);
		else if(direction == -1)
			fdMapGridCopyMetatileAttributes(24, 21, 1, 3, 23, 9);
	} else if(scene == SCENE_GABOMBA_STATUE_STEEL) {
		scGabombaStatueSetCogTurn(ACTOR_STEEL_GREEN_COGS, direction, 0);
		scGabombaStatueSetCogTurn(ACTOR_STEEL_BLUE_COGS, direction, 1);
		scGabombaStatueSetCogTurn(ACTOR_STEEL_RED_COGS, direction, 2);
		scGabombaStatueSetCogTurn(ACTOR_STEEL_BLUE_PILLARS, direction, 3);
		scGabombaStatueSetCogTurn(ACTOR_STEEL_PINK_DRUMS, direction, 4);
	} else if(scene == SCENE_GABOMBA_STATUE_MOUSE) {
		scGabombaStatueSetCogTurn(ACTOR_MOUSE_GREEN_COGS, direction, 0);
		scGabombaStatueSetCogTurn(ACTOR_MOUSE_BLUE_COGS, direction, 1);
		scGabombaStatueSetCogTurn(ACTOR_MOUSE_RED_COGS, direction, 2);
		scGabombaStatueSetCogTurn(ACTOR_MOUSE_BLUE_PILLAR_B, direction, 3);
		scGabombaStatueSetCogTurn(ACTOR_MOUSE_PINK_DRUM_B, direction, 4);
	} else if(scene == SCENE_GABOMBA_STATUE_COGS) {
		scGabombaStatueSetCogTurn(ACTOR_COGS_RED_COGS, direction, 0);
		scGabombaStatueSetCogTurn(ACTOR_COGS_PINION_A, direction, 1);
		if(direction == 0)
			fdMapGridCopyMetatileAttributes(34, 12, 1, 3, 27, 12);
		else if(direction == -1)
			fdMapGridCopyMetatileAttributes(35, 12, 1, 3, 27, 12);
	} else if(scene == SCENE_GABOMBA_STATUE_ARROWS) {
		fdActorSetAnimation(ACTOR_CHARGE_C, 5);
		fdActorSetSpriteFlags(fdActorGet(ACTOR_CHARGE_C), 0);
	} else if(scene == SCENE_GABOMBA_STATUE_PINION) {
		scGabombaStatueSetCogTurn(ACTOR_PINION_A, direction, 0);
		scGabombaStatueSetCogTurn(ACTOR_PINION_B, direction, 1);
		scGabombaStatueSetCogTurn(ACTOR_PINION_C, direction, 2);
		scGabombaStatueSetCogTurn(ACTOR_PINION_D, direction, 3);
		scGabombaStatueSetCogTurn(ACTOR_PINION_E, direction, 4);
		if(direction == 0)
			fdMapGridCopyMetatileAttributes(0, 0, 1, 3, 9, 27);
		else if(direction == -1)
			fdMapGridCopyMetatileAttributes(1, 0, 1, 3, 9, 27);
	}
}

extern Sint32 GabombaStatueChargeScript[];

/* Sets charge id circling the orb statue statueId from the angle given, tinted
   colorMode. */
void scGabombaStatueSendCharge(
	Sint32 id, Sint32 colorMode, Sint32 statueId, Sint32 angle)
{
	ACTOR *charge = fdActorGet(id);
	ACTOR *target = fdActorGet(statueId);

	fdActorSelectAnimation(charge, 0);
	fdActorStartScript(charge, GabombaStatueChargeScript);
	fdActorSetSpriteFlags(charge, 0);
	charge->motionFlags = 0;
	charge->watchedActor = (FIELD_OBJECT *)target;
	/* sceneCount is its place on the circle, in 256ths of a turn, and sceneByte
	   its own actor id until the run sets chargeState there. */
	charge->sceneCount = angle / (TRIG_TURN / 256);
	charge->sceneByte = id;
	fdActorSetSpriteColorMode(charge, colorMode);
}

#undef DIFF_WITHIN
#undef MARK
#undef COG_METATILE
#undef DEFINE_GABOMBA_STATUE_STEP_WHEN_HELD
#undef DEFINE_GABOMBA_STATUE_STEP_WHEN_HELD_FOUR
#undef DEFINE_GABOMBA_STATUE_WALK_LEADER_TO
#undef DEFINE_GABOMBA_STATUE_HIDE_LADDER_TOP_BLOCKS

#include "../field/field_effect_advance_motion.c"

/* Felix drops off the south end of a pink drum in the mouse's room above and
   falls into Steel's room, landing with a thump in a ring of dust; he sits a
   moment as the screen shakes, then stands. One of the two drums lands him
   beside Steel, who can then be talked to. */
void scGabombaStatueLandFromFall(void)
{
	ACTOR		  *actor = fdActorGet(MainRoutineStateWork.leaderActorId);
	Uint32		   i;
	EFFECT_OPTIONS options;

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdMapGridCenterOnFollowedPoint();
	syWait(1);
	actor->y.fixed = toFIXED(130);
	actor->gravity = toFIXED(1) / 2;
	actor->bounce = 0;
	actor->motionFlags = 0;
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptPlaySound(SE_DUST_PUFF);
	actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	syFieldScriptWait(24);
	options.palette = SPRITE_COLOR_WHITE;
	options.update = fdEffectAdvanceMotion;
	options.startScaleX = toFIXED(4) / 5;
	options.startScaleY = toFIXED(4) / 5;
	for(i = 0; i <= DUST_RING_PUFFS; i++) {
		Sint32 angle;
		FIXED  velocity[3];

		angle = i * FACING_STEP;
		velocity[0] = syCos(angle);
		velocity[1] = 0;
		velocity[2] = syMathSin(angle);
		velocity[0] += velocity[0] >> 1;
		velocity[0] += (syRandomBelow(toFIXED(3) / 4)) - toFIXED(3) / 8;
		velocity[2] += (syRandomBelow(toFIXED(3) / 8)) - toFIXED(3) / 16;
		scFieldEffectSpawn(actor->x.fixed, actor->y.fixed, actor->z.fixed,
			velocity[0], velocity[1], velocity[2],
			EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE | 1,
			&options);
	}
	fdScriptPlaySound(SE_THUMP);
	fdActorSetAttachedEffect(MainRoutineStateWork.leaderActorId,
		ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_LEADER_SIT);
	fdMapGridSetShake(5, 5, 1);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	syFieldMapGridWaitForShakeToSettle();
	fdActorSetAttachedEffect(MainRoutineStateWork.leaderActorId,
		ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
	actor->gravity = toFIXED(1);
	actor->bounce = toFIXED(1) / 4;
	syWait(10);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_STAND);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	fdScriptEnd();
}

extern Uint32 GabombaStatueLeaderFocusScript[];

/* Coming up the hole into the arrows room as the ceremony starts below: Akafubu
   calls "Focus your energies!", and Kraden calls the party over to watch. */
void scGabombaStatueFocusEnergies(void)
{
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	syScriptCameraGetActor()->motionFlags = 0;
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	syFieldScriptWait(40);
	syFieldScriptSetMessageId(MsgGabombaFocusEnergies);
	/* "Focus your energies!" */
	fdScriptShowMessage(ACTOR_AKAFUBU, 0);
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
	fdActorSetScript(ACTOR_KRADEN, GabombaStatueKradenFocusScript);
	syWait(10);
	fdActorSetScript(
		MainRoutineStateWork.leaderActorId, GabombaStatueLeaderFocusScript);
	fdActorSetScript(ACTOR_JENNA, GabombaStatueJennaFocusScript);
	fdActorSetScript(ACTOR_PIERS, GabombaStatuePiersFocusScript);
	scFieldActorWaitForScriptEndById(ACTOR_KRADEN);
	syFieldScriptWait(20);
	fdCameraSetSpeed(1.2);
	fdCameraMoveTo(434, CAMERA_KEEP, 286, 1);
	fdActorSetSpeed(ACTOR_KRADEN, 1.2);
	fdActorRunScriptAndWait(ACTOR_KRADEN, GabombaStatueKradenPointsScript);
	syFieldScriptWait(40);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHEAST);
	/* "Everyone! Look over here!" */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	fdCameraSetSpeed(4.8);
	fdCameraMoveTo(434, CAMERA_KEEP, 168, 1);
	fdCameraWaitForArrival();
	syFieldScriptWait(20);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
	fdActorFaceDirection(
		MainRoutineStateWork.leaderActorId, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTHEAST, 40);
	fdActorGet(ACTOR_PIERS)->facing = FACING_SOUTH;
	fdActorGet(MainRoutineStateWork.leaderActorId)->facing = FACING_SOUTH;
	fdActorGet(ACTOR_SHEBA)->facing = FACING_SOUTH;
	fdActorGet(ACTOR_JENNA)->facing = FACING_SOUTH;
	fdActorGet(ACTOR_PIERS)->steeringFlags &= ~ACTOR_FACES_MOTION;
	fdActorGet(MainRoutineStateWork.leaderActorId)->steeringFlags &=
		~ACTOR_FACES_MOTION;
	fdActorGet(ACTOR_SHEBA)->steeringFlags &= ~ACTOR_FACES_MOTION;
	fdActorGet(ACTOR_JENNA)->steeringFlags &= ~ACTOR_FACES_MOTION;
	fdActorWalkTo(ACTOR_PIERS, 400, 272);
	fdActorWalkTo(MainRoutineStateWork.leaderActorId, 420, 272);
	fdActorWalkTo(ACTOR_SHEBA, 444, 272);
	fdActorWalkTo(ACTOR_JENNA, 468, 272);
	syFieldScriptWait(20);
	fdPaletteFadeApplySource(PALETTE_TINT_NONE, 0);
	fdPaletteFadeApplyTarget(0, 0);
	fdPaletteFadeInterpolate(80);
	syWait(80);
	syFieldScriptSetExitRequest(
		GABOMBA_STATUE_ARROWS_TO_GABOMBA_STATUE_FACE_EAST_B);
}

/* The party watches the orb statues' charges run along the broken channel, and
   is sent back down as the ceremony fails. */
void scGabombaStatueWatchTheChargesStick(void)
{
	Sint32 south = FACING_SOUTH;

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdScriptPlaySound(SOUND_FADE_OUT_BGM);
	fdActorSetPosAndFacing(
		MainRoutineStateWork.leaderActorId, toFIXED(348), toFIXED(312), south);
	fdActorSetPosAndFacing(ACTOR_PIERS, toFIXED(330), toFIXED(312), south);
	fdActorSetPosAndFacing(
		ACTOR_KRADEN, toFIXED(432), toFIXED(312), FACING_SOUTH_SOUTHEAST);
	fdActorSetPosAndFacing(ACTOR_SHEBA, toFIXED(448), toFIXED(312), south);
	fdActorSetPosAndFacing(ACTOR_JENNA, toFIXED(468), toFIXED(312), south);
	syWait(1);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	syFieldScriptWait(40);
	fdScriptPlaySound(SE_SHIMMER);
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
	fdScriptPlaySound(SE_PSYNERGY_START);
	scFieldActorSetSpriteColor(
		fdActorGet(ACTOR_BLUE_ORB_STATUE), SPRITE_COLOR_WHITE);
	scFieldActorSetSpriteColor(
		fdActorGet(ACTOR_RED_ORB_STATUE), SPRITE_COLOR_WHITE);
	syWait(8);
	scFieldActorSetSpriteColor(
		fdActorGet(ACTOR_RED_ORB_STATUE), SPRITE_COLOR_BGR);
	scFieldActorSetSpriteColor(
		fdActorGet(ACTOR_BLUE_ORB_STATUE), SPRITE_COLOR_NORMAL);
	syWait(8);
	fdActorGet(MainRoutineStateWork.leaderActorId)->lookTimer =
		(syRandomBelow(40)) + 10;
	fdActorGet(ACTOR_PIERS)->lookTimer = (syRandomBelow(40)) + 10;
	fdActorGet(ACTOR_JENNA)->lookTimer = (syRandomBelow(40)) + 10;
	fdActorGet(ACTOR_SHEBA)->lookTimer = (syRandomBelow(40)) + 10;
	fdActorGet(ACTOR_KRADEN)->lookTimer = (syRandomBelow(40)) + 10;
	fdActorSetScript(MainRoutineStateWork.leaderActorId,
		GabombaStatueLookNorthAndEastScript);
	fdActorSetScript(ACTOR_PIERS, GabombaStatueLookNorthAndEastScript);
	fdActorSetScript(ACTOR_JENNA, GabombaStatueLookNorthAndWestScript);
	fdActorSetScript(ACTOR_SHEBA, GabombaStatueLookNorthAndWestScript);
	fdActorSetScript(ACTOR_KRADEN, GabombaStatueLookNorthAndWestScript);
	fdScriptPlaySound(MUS_RITUALS);
	scGabombaStatueRunCharges(ACTOR_CHARGE_A, 0);
	syFieldScriptWait(80);
	fdScriptSetFlag(FLAG_GABOMBA_MUSIC_SILENCED);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	syFieldScriptSetExitRequest(
		GABOMBA_STATUE_ARROWS_TO_KIBOMBO_STATUE_GROUNDS_CEREMONY_NORTH_B_B);
}

extern Uint32 GabombaStatueChannelSeeds[16];

/* Kraden sees how the statue works, and the party agrees to mend the channel
   for Akafubu's ceremony. */
void scGabombaStatueSeeHowItWorks(void)
{
	Sint32 piers = SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_PIERS;

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdActorSetPosAndFacing(MainRoutineStateWork.leaderActorId, toFIXED(348),
		toFIXED(312), FACING_NORTH);
	fdActorSetPosAndFacing(
		ACTOR_PIERS, toFIXED(330), toFIXED(312), FACING_NORTH);
	fdActorSetPosAndFacing(
		ACTOR_KRADEN, toFIXED(432), toFIXED(312), FACING_NORTH_NORTHWEST);
	fdActorSetPosAndFacing(
		ACTOR_SHEBA, toFIXED(448), toFIXED(312), FACING_NORTH);
	fdActorSetPosAndFacing(
		ACTOR_JENNA, toFIXED(468), toFIXED(312), FACING_NORTH);
	syWait(1);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	syFieldScriptWait(40);
	scFieldActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	syFieldScriptSetMessageId(MsgGabombaUnderstandsChannel);
	/* "Ah! So that's how it works!" */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	syScriptCameraGetActor()->motionFlags = 0;
	fdCameraSetSpeed(0.8);
	fdCameraMoveTo(392, CAMERA_KEEP, 292, 1);
	fdCameraWaitForArrival();
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_EAST, 0);
	fdActorFaceDirectionAndWait(ACTOR_SHEBA, FACING_WEST);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
	/* "He's been failing because the conduit that channels his Psynergy
	   is..." */
	fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_SHEBA, 0);
	/* "Should we reconnect the conduit so he can complete the ceremony?" */
	fdScriptOpenMessage(
		SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_JENNA, 0);
	if(syFieldScriptAskYesNoWithActorAnimation(
		   MainRoutineStateWork.leaderActorId, 0) == 0) {
		scFieldActorShowEmote(
			ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
		fdActorSetSpeed(ACTOR_PIERS, 2);
		fdActorWalkToAndWait(ACTOR_PIERS, 340, 296);
		fdActorWalkToAndWait(ACTOR_PIERS, 354, 296);
		fdActorWalkToAndWait(ACTOR_PIERS, 364, 308);
		fdActorFaceDirection(
			MainRoutineStateWork.leaderActorId, FACING_NORTHEAST, 0);
		fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTHWEST);
		/* "I won't help them, Felix! They're the ones who stole my orb from
		   the..." */
		fdScriptShowMessage(
			SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_PIERS, 0);
		FieldStateWork->msg++;
	} else {
		FieldStateWork->msg++;
		fdActorSetSpeed(ACTOR_PIERS, 2);
		fdActorWalkToAndWait(ACTOR_PIERS, 340, 296);
		fdActorWalkToAndWait(ACTOR_PIERS, 354, 296);
		fdActorWalkToAndWait(ACTOR_PIERS, 364, 308);
		fdActorFaceDirection(
			MainRoutineStateWork.leaderActorId, FACING_NORTHEAST, 0);
		fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTHWEST);
		fdActorSetAnimation(ACTOR_PIERS, ANIM_NOD);
		/* "I agree, Felix! Why should we help this pack of thieves?" */
		fdScriptShowMessage(piers, 0);
	}
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_WEST);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
	/* "Piers, I understand that you're angry at them, but it really is
	   in..." */
	fdScriptShowMessage(
		SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_EAST, 0);
	scFieldActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	/* "Think about it, Piers... Akafubu stole the orb solely to perform..." */
	fdScriptShowMessage(
		SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0);
	fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
	scFieldActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
	/* "So, what do you think he intends to do with the orb?" */
	fdScriptShowMessage(
		SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0);
	/* "He means to give it to the Great Gabomba..." */
	fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_PIERS, 0);
	scFieldActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	/* "Oh! So...if his ceremony succeeds, the orb will be taken into the..." */
	fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_PIERS, 0);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_NOD);
	/* "And we're INSIDE the statue! If the ceremony succeeds, we can go..." */
	fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0);
	scFieldActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 10);
	/* "I...don't want to help that Akafubu, but..." */
	fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_PIERS, 0);
	fdActorSetSpeed(ACTOR_SHEBA, 0.8);
	fdActorWalkToAndWait(ACTOR_SHEBA, 440, 296);
	fdActorFaceDirectionAndWait(ACTOR_SHEBA, FACING_WEST);
	fdActorSetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
	/* "I don't like the idea of helping him any more than you do, but if
	   we..." */
	fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_SHEBA, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_NORTHEAST, 0);
	scFieldActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_EAST, 80);
	scFieldActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
	fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_EAST);
	/* "You're right, Sheba. Let's see if we can't help him finish his..." */
	fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_PIERS, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 0);
	fdActorFaceDirectionAndWait(ACTOR_SHEBA, FACING_WEST);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_NOD);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_NOD);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHWEST);
	/* "It looks like the energy charge is supposed to run over these
	   blocks." */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	scFieldActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 20);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHWEST);
	/* "If we can just move these around a little, that might be enough
	   to..." */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	fdCameraSetSpeed(4.8);
	fdCameraMoveTo(392, CAMERA_KEEP, 176, 1);
	fdCameraWaitForArrival();
	/* "Ah! There we go!" */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	/* "These panels have arrows on them... What do you think it all means?" */
	fdScriptShowMessage(ACTOR_SHEBA, 0);
	/* "Look at the directions they point... I'll bet these will rotate
	   the..." */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	/* "But how do we set the blocks back in place?" */
	fdScriptShowMessage(ACTOR_PIERS, 0);
	/* "Just Pound them in!" */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0);
	fdCameraMoveTo(392, CAMERA_KEEP, 292, 1);
	fdCameraWaitForArrival();
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_NOD);
	/* "Won't that do the trick?" */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_WEST);
	/* "We must watch the path his Psynergy takes and move the blocks..." */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	fdActorSetSpeed(ACTOR_JENNA, 1.6);
	fdActorSetSpeed(ACTOR_SHEBA, 1.6);
	fdActorSetSpeed(ACTOR_KRADEN, 1.6);
	fdActorSetSpeed(ACTOR_PIERS, 0.8);
	fdActorSetScript(ACTOR_PIERS, GabombaStatueLookAtChannelScript);
	fdActorSetScript(ACTOR_KRADEN, GabombaStatueLookAtChannelScript);
	fdActorSetScript(ACTOR_SHEBA, GabombaStatueLookAtChannelScript);
	fdActorRunScriptAndWait(ACTOR_JENNA, GabombaStatueLookAtChannelScript);
	fdScriptSetFlag(FLAG_GABOMBA_SAW_HOW_IT_WORKS);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	fdActorTableSnapshot();
	syFieldScriptSetExitRequest(
		GABOMBA_STATUE_ARROWS_TO_KIBOMBO_STATUE_GROUNDS_CEREMONY_SOUTH);
}

/* Once the Gabomba has responded, Akafubu comes up into the statue, and the
   party tells him how it mended the channel and where the orb went. */
void scGabombaStatueSawEnergyFlow(void)
{
	Sint32 index = scGabombaStatueGetPuzzleSeed();
	FIXED  x;
	FIXED  z;
	ACTOR *hand;
	ACTOR *actor;

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	scGabombaStatueSetUpChannelPuzzle(CHANNEL_ORIGIN_COLUMN, CHANNEL_ORIGIN_ROW,
		GabombaStatueChannelSeeds[index], ACTOR_BLOCK_A);
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
	fdActorSetPosAndFacing(
		ACTOR_JENNA, toFIXED(472), toFIXED(216), FACING_SOUTH);
	fdActorSetPosAndFacing(
		ACTOR_AKAFUBU, toFIXED(472), toFIXED(200), FACING_SOUTH_SOUTHEAST);
	syWait(1);
	fdMapGridCenterOnFollowedPoint();
	syWait(1);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdCameraSetSpeed(1.2);
	fdCameraMoveTo(440, CAMERA_KEEP, 312, 1);
	fdActorSetSpeed(ACTOR_JENNA, 1);
	fdActorSetSpeed(ACTOR_AKAFUBU, 1);
	fdActorSetScript(ACTOR_JENNA, GabombaStatueJennaWatchesScript);
	fdActorRunScriptAndWait(ACTOR_AKAFUBU, GabombaStatueAkafubuComesUpScript);
	syFieldScriptSetMessageId(MsgGabombaSawEnergyFlow);
	/* "Yeah, we know... We saw your magic energy flowing past us." */
	fdScriptShowMessage(ACTOR_JENNA, 0);
	fdActorFaceDirection(ACTOR_AKAFUBU, FACING_NORTH_NORTHWEST, 0);
	fdActorWalkToAndWait(ACTOR_JENNA, 440, 296);
	fdActorFaceDirectionAndWait(ACTOR_JENNA, FACING_EAST);
	/* "See how this channel runs along the floor?" */
	fdScriptShowMessage(ACTOR_JENNA, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
	fdCameraMoveTo(414, CAMERA_KEEP, 272, 1);
	fdCameraWaitForArrival();
	/* "Here, look... It runs all the way over there!" */
	fdScriptShowMessage(ACTOR_JENNA, 0);
	scGabombaStatueFindBrokenMetatile(&x, &z);
	x = FIELD_METATILE_ORIGIN(x) + FIELD_METATILE_CENTER(CHANNEL_ORIGIN_COLUMN);
	z = FIELD_METATILE_ORIGIN(z) + FIELD_METATILE_CENTER(CHANNEL_ORIGIN_ROW);
	hand = scFieldActorCreate(SPRITE_POINTING_HAND, x, 0, z);
	hand->motionFlags |= ACTOR_FLOATS;
	fdActorSetSpriteFlags(hand, 0);
	fdActorSelectAnimation(hand, 1);
	syWait(1);
	actor = fdActorGet(ACTOR_JENNA);
	actor->facing =
		syAtan(hand->z.fixed - actor->z.fixed, hand->x.fixed - actor->x.fixed);
	fdCameraMoveToFX(hand->x.fixed, -1, hand->z.fixed, 1);
	fdCameraWaitForArrival();
	/* "But the thing is, the circuit was broken right there." */
	fdScriptShowMessage(ACTOR_JENNA, 0);
	fdActorRelease(hand);
	fdCameraMoveTo(440, CAMERA_KEEP, 304, 1);
	fdCameraWaitForArrival();
	fdActorFaceDirectionAndWait(ACTOR_JENNA, FACING_SOUTHEAST);
	/* "So we completed the circuit so that your magic energy could go
	   where..." */
	fdScriptShowMessage(ACTOR_JENNA, 0);
	fdActorSetAttachedEffect(
		ACTOR_AKAFUBU, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldScriptWait(20);
	/* "But...if you truly did this, then why? Why did you help me?" */
	fdScriptShowMessage(ACTOR_AKAFUBU, 0);
	fdActorSetPosAndFacing(MainRoutineStateWork.leaderActorId, toFIXED(472),
		toFIXED(204), FACING_SOUTH);
	fdActorSetPosAndFacing(
		ACTOR_PIERS, toFIXED(440), toFIXED(204), FACING_SOUTH);
	fdActorSetPosAndFacing(
		ACTOR_KRADEN, toFIXED(456), toFIXED(204), FACING_SOUTH_SOUTHEAST);
	fdActorSetPosAndFacing(
		ACTOR_SHEBA, toFIXED(488), toFIXED(204), FACING_SOUTH);
	syWait(1);
	/* "We were sort of hoping that we could take the orb if your
	   ceremony..." */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	fdActorFaceDirection(ACTOR_AKAFUBU, FACING_NORTH_NORTHEAST, 0);
	fdCameraMoveTo(440, CAMERA_KEEP, 264, 1);
	fdCameraWaitForArrival();
	scFieldActorJump(ACTOR_AKAFUBU, 4, 10);
	/* "Ah, so you have taken the orb? Then begone! You have no further..." */
	fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_AKAFUBU, 0);
	fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_SHAKE_HEAD);
	/* "But...we didn't get the orb." */
	fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_PIERS, 0);
	fdActorShowEmote(ACTOR_AKAFUBU, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	/* "It rolled through this groove and right through this hole." */
	fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_SHEBA, 0);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
	/* "We saw a light through there, so we suspect there must be another..." */
	fdScriptShowMessage(
		SPEAKER_PORTRAIT_LEFT | SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 0);
	syFieldScriptWait(20);
	fdPaletteFadeApplySource(PALETTE_TINT_NONE, 0);
	fdPaletteFadeApplyTarget(0, 0);
	fdPaletteFadeInterpolate(60);
	fdScriptClearFlag(FLAG_KEEP_PARTY_POSITION);
	syWait(60);
	syFieldScriptSetExitRequest(
		GABOMBA_STATUE_ARROWS_TO_GABOMBA_STATUE_CHAMBER_IDOL_WEST);
}

/* Back from a failed ceremony whose charges reached the statue in the middle
   apart, Kraden notices it is their timing that matters, and the clock starts
   again. */
void scGabombaStatueKradenNoticesTiming(void)
{
	scGabombaStatueStopClock();
	fdActorPlaceOnActor(ACTOR_KRADEN, MainRoutineStateWork.leaderActorId);
	scFieldActorSetSpriteColor(fdActorGet(ACTOR_KRADEN), SPRITE_COLOR_HIDDEN);
	syWait(10);
	syFieldScriptSetMessageId(MsgGabombaChannelTiming);
	/* "It looks like just fixing the channel should do the trick...
	   Maybe..." */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	fdActorSetPos(ACTOR_KRADEN, 0, 0);
	syWait(1);
	fdScriptSetFlag(FLAG_GABOMBA_TIMING_NOTICED);
	scGabombaStatueRestartClock();
}

/* Sets up whichever room of the statue is being entered: its cogs turning,
   stopped by the rope or turned the other way by the stakes, then the room's
   own things. */
Sint32 scInit(void)
{
	Sint16 scene;

	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	if(MainRoutineStateWork.next != SCENE_GABOMBA_STATUE_BONE_ARMLET_CHEST) {
		if(fdScriptGetFlag(FLAG_COGS_STOPPED))
			scGabombaStatueTurnCogs(0);
		else if(fdScriptGetFlag(FLAG_GABOMBA_COGS_TURNED_BACK))
			scGabombaStatueTurnCogs(-1);
		else
			scGabombaStatueTurnCogs(1);
		scGabombaStatueDrawCogMetatiles();
	}
	fdActorGet(syFieldPartyGetLeaderActor())->priorityFlags |=
		ACTOR_PRIORITY_IGNORES_RISES;
	scene = MainRoutineStateWork.next;
	if(scene == SCENE_GABOMBA_STATUE_BONE_ARMLET_CHEST) {
		scGabombaStatueInitBoneArmletChest();
	} else if(scene == SCENE_GABOMBA_STATUE_ELIXIR_CHEST) {
		fdMapGridCopyMetatileAttributes(15, 68, 8, 3, 15, 4);
		scGabombaStatueInitElixirChest();
	} else if(scene == SCENE_GABOMBA_STATUE_STEEL) {
		scGabombaStatueInitSteel();
	} else if(scene == SCENE_GABOMBA_STATUE_COGS) {
		scGabombaStatueInitCogs();
	} else if(scene == SCENE_GABOMBA_STATUE_ARROWS) {
		scGabombaStatueInitArrows();
	} else if(scene == SCENE_GABOMBA_STATUE_PINION) {
		scGabombaStatueInitPinion();
	}
	return 0;
}

/* Coming into the hall of pinions from outside sets the cogs and the stakes
   back as they were. */
Sint32 scPrepareMap(void)
{
	if(MainRoutineStateWork.next == SCENE_GABOMBA_STATUE_PINION &&
		MainRoutineStateWork.entrance ==
			GABOMBA_STATUE_PINION_FROM_KIBOMBO_STATUE_GROUNDS &&
		!fdScriptGetFlag(FLAG_KEEP_PARTY_POSITION)) {
		fdScriptClearFlag(FLAG_GABOMBA_COGS_TURNED_BACK);
		fdScriptClearFlag(FLAG_GABOMBA_FIRST_STAKE_POUNDED);
		fdScriptClearFlag(FLAG_GABOMBA_SECOND_STAKE_POUNDED);
	}
	return 0;
}

/* Sets up the Bone Armlet chest's room. */
void scGabombaStatueInitBoneArmletChest(void)
{
	scGabombaStatueHideBoneArmletChestLadderTopBlock();
}

/* Places rope index between two posts, tied when its flag is set, else coiled:
   the script inline over the scene's import of fdRopePlace, as script_inline.h
   writes them. */
static inline void scFieldRopePlace(
	Sint32 index, Sint32 flag, Sint32 firstPost, Sint32 secondPost)
{
	scGabombaStatuePlaceRope(index, flag, firstPost, secondPost);
}

/* Sets up the Elixir chest's room: its pinions let go of the leader coming in
   from the west or the east of Steel's room, the stakes, the flashing cog
   raised until both are Pounded, and the two ropes. */
void scGabombaStatueInitElixirChest(void)
{
	if(MainRoutineStateWork.entrance >=
			GABOMBA_STATUE_ELIXIR_CHEST_FROM_GABOMBA_STATUE_STEEL_WEST &&
		MainRoutineStateWork.entrance <=
			GABOMBA_STATUE_ELIXIR_CHEST_FROM_GABOMBA_STATUE_STEEL_EAST) {
		fdActorSetScript(
			ACTOR_ELIXIR_PINION_A, GabombaStatuePinionReleaseScript);
		fdActorSetScript(
			ACTOR_ELIXIR_PINION_B, GabombaStatuePinionReleaseScript);
		fdMapGridCopyMetatileAttributes(33, 24, 3, 3, 27, 29);
	}
	scGabombaStatuePlaceSwitchActors(GabombaStatueStakes);
	if(!fdScriptGetFlag(FLAG_GABOMBA_FIRST_STAKE_POUNDED) ||
		!fdScriptGetFlag(FLAG_GABOMBA_SECOND_STAKE_POUNDED))
		fdActorGet(ACTOR_FLASHING_COG)->y.fixed = toFIXED(16);
	fdRopeInit();
	scFieldRopePlace(FIRST_ROPE, FLAG_GABOMBA_FIRST_ROPE_TIED,
		ACTOR_FIRST_ROPE_POST_A, ACTOR_FIRST_ROPE_POST_B);
	scFieldRopePlace(SECOND_ROPE, FLAG_GABOMBA_SECOND_ROPE_TIED,
		ACTOR_SECOND_ROPE_POST_A, ACTOR_SECOND_ROPE_POST_B);
	scGabombaStatueHideElixirChestLadderTopBlocks();
}

/* Sets up Steel's room: its rope, and the fall in from the mouse's room. */
void scGabombaStatueInitSteel(void)
{
	fdRopeInit();
	scFieldRopePlace(FIRST_ROPE, FLAG_GABOMBA_FIRST_ROPE_TIED,
		ACTOR_STEEL_ROPE_POST_A, ACTOR_STEEL_ROPE_POST_B);
	if(!fdScriptGetFlag(FLAG_KEEP_PARTY_POSITION)) {
		if(MainRoutineStateWork.entrance >=
				GABOMBA_STATUE_STEEL_FROM_GABOMBA_STATUE_MOUSE_SOUTHEAST_A &&
			MainRoutineStateWork.entrance <=
				GABOMBA_STATUE_STEEL_FROM_GABOMBA_STATUE_MOUSE_SOUTHEAST_B)
			scGabombaStatueLandFromFall();
		if(MainRoutineStateWork.entrance ==
			GABOMBA_STATUE_STEEL_FROM_GABOMBA_STATUE_MOUSE_SOUTHEAST_A)
			fdScriptSetFlag(FLAG_LANDED_BESIDE_STEEL);
	}
}

/* Sets up the room below the small idol's chamber: its rope, coiled. */
void scGabombaStatueInitCogs(void)
{
	fdRopeInit();
	Value3((Sint32 (*)())scGabombaStatueCoilRope, FIRST_ROPE,
		ACTOR_COGS_ROPE_POST_A, ACTOR_COGS_ROPE_POST_B);
}

/* Which of the channel's seeds this game lays it from: the letters of the
   leader's name added up. */
Sint32 scGabombaStatueGetPuzzleSeed(void)
{
	UNIT  *leader = syUnitGet(MainRoutineStateWork.leaderActorId);
	Sint32 sum = 0;
	Uint32 i;

	for(i = 0; i < UNIT_NAME_SIZE - 1; i++)
		sum += leader->name[i];
	return sum & (ARRAY_COUNT(GabombaStatueChannelSeeds) - 1);
}

void scGabombaStatueCheckOfferingClock();

/* Sets up the arrows room: its orb statues, its charges and the channel. */
void scGabombaStatueInitArrows(void)
{
	ACTOR *leader;
	Sint32 seed;

	scFieldActorSetSpriteColor(
		fdActorGet(ACTOR_RED_ORB_STATUE), SPRITE_COLOR_BGR);
	sySetTickEvent((Sint32)scGabombaStatueFadeByDepth, EVENT_PASS_TICK);
	scFieldActorSetSpriteColor(
		fdActorGet(ACTOR_CHARGE_A), SPRITE_COLOR_GREEN_TINT);
	scFieldActorSetSpriteColor(
		fdActorGet(ACTOR_CHARGE_B), SPRITE_COLOR_GREEN_TINT);
	scFieldActorSetSpriteColor(
		fdActorGet(ACTOR_CHARGE_C), SPRITE_COLOR_RED_TINT);
	scFieldActorSetSpriteColor(
		fdActorGet(ACTOR_CHARGE_D), SPRITE_COLOR_RED_TINT);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_BLOCK_A), 0);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_BLOCK_B), 0);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_BLOCK_C), 0);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_BLOCK_D), 0);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_BLOCK_E), 0);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_BLOCK_F), 0);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_BLOCK_G), 0);
	if(fdScriptGetFlag(FLAG_GABOMBA_RESPONDED) &&
		!fdScriptGetFlag(FLAG_AKAFUBU_OFFERED_HELP)) {
		scGabombaStatueSawEnergyFlow();
		return;
	}
	if(fdScriptGetFlag(FLAG_GABOMBA_BACK_FROM_CEREMONY)) {
		leader = fdActorGet(MainRoutineStateWork.leaderActorId);
		fdActorSetPos(ACTOR_CHARGE_A, 0, 0);
		fdActorSetPos(ACTOR_CHARGE_B, 0, 0);
		fdActorSetPos(ACTOR_CHARGE_C, 0, 0);
		fdActorSetPos(ACTOR_CHARGE_D, 0, 0);
		scGabombaStatueResetClock();
		fdScriptStart();
		syFieldScriptEndLastingPsynergy(0);
		fdCameraMoveToFX(leader->x.fixed, -1, leader->z.fixed, 0);
		fdMapGridCenterOnFollowedPoint();
		syWait(1);
		fdScriptEnd();
	}
	if(fdScriptGetFlag(FLAG_GABOMBA_SAW_HOW_IT_WORKS) &&
		!fdScriptGetFlag(FLAG_AKAFUBU_OFFERED_HELP)) {
		scGabombaStatueStartClock(0);
		sySetTickEvent(
			(Sint32)scGabombaStatueCheckOfferingClock, EVENT_PASS_TICK);
	}
	seed = scGabombaStatueGetPuzzleSeed();
	scGabombaStatueSetUpChannelPuzzle(CHANNEL_ORIGIN_COLUMN, CHANNEL_ORIGIN_ROW,
		GabombaStatueChannelSeeds[seed], ACTOR_BLOCK_A);
	fdActorSetAnimation(ACTOR_ARROWS_STAKE, 5);
	if(MainRoutineStateWork.entrance ==
		GABOMBA_STATUE_ARROWS_FROM_KIBOMBO_STATUE_GROUNDS_CEREMONY_NORTH_A) {
		scGabombaStatueWatchTheChargesStick();
		return;
	}
	if(!fdScriptGetFlag(FLAG_GABOMBA_SAW_HOW_IT_WORKS)) {
		if(MainRoutineStateWork.entrance ==
			GABOMBA_STATUE_ARROWS_FROM_GABOMBA_STATUE_CHAMBER_IDOL_EAST)
			scGabombaStatueFocusEnergies();
		else if(MainRoutineStateWork.entrance ==
			GABOMBA_STATUE_ARROWS_FROM_KIBOMBO_STATUE_GROUNDS_CEREMONY_NORTH_B_A)
			scGabombaStatueSeeHowItWorks();
	}
	if(fdScriptGetFlag(FLAG_GABOMBA_BACK_FROM_CEREMONY)) {
		fdScriptStart();
		syFieldScriptEndLastingPsynergy(0);
		FieldStateWork->transition =
			SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
		fdScriptStartScreenTransition();
		fdScriptWaitTransitionTicks();
		FieldStateWork->transition =
			SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
		fdScriptClearFlag(FLAG_GABOMBA_BACK_FROM_CEREMONY);
		if(fdScriptGetFlag(FLAG_GABOMBA_CHARGES_REACHED_APART) &&
			!fdScriptGetFlag(FLAG_GABOMBA_TIMING_NOTICED))
			scGabombaStatueKradenNoticesTiming();
		fdScriptClearFlag(FLAG_SHOW_AREA_NAME);
		fdScriptEnd();
	}
}

/* Sets up the hall of pinions; coming in from outside, Retreat leads back to
   it. */
void scGabombaStatueInitPinion(void)
{
	fdActorGet(MainRoutineStateWork.leaderActorId)->priorityFlags |=
		ACTOR_PRIORITY_IGNORES_RISES;
	if(MainRoutineStateWork.entrance ==
		GABOMBA_STATUE_PINION_FROM_KIBOMBO_STATUE_GROUNDS) {
		fdScriptSetFlag(FLAG_GABOMBA_RETREAT_TO_PINION);
		fdScriptClearFlag(FLAG_GABOMBA_RETREAT_TO_IDOL);
	}
}

static __inline__ void *scMemAllocate(Sint32 size)
{
	return syHeapAllocate(size);
}

/* A raised block's picture: 16 pixels square in 256 colours, one for each piece
   in CHANNEL_PIECE's order. */
enum { BLOCK_PICTURE_BYTES = 16 * 16 };

/* Loads the blocks' pictures into the VRAM cache. */
void scGabombaStatueLoadBlockPictures(void)
{
	SCENE_STATE *state = &SceneStateWork;
	void		*buf = scMemAllocate(PIECE_KINDS * BLOCK_PICTURE_BYTES);
	Sint32		 handle;

	syDecompressTaggedLZ(FieldCachedTilesA, buf);
	handle = syVRAMCacheFindFreeEntry();
	syVRAMCacheLoad(handle, PIECE_KINDS * BLOCK_PICTURE_BYTES, buf);
	state->blockTileEntry = handle;
	syHeapRelease(buf);
}

/* Whether channel metatile (x, z) is taken: blocked, holding a block, or a
   stop; 0 outside the grid. */
Sint32 scGabombaStatueIsMetatileTaken(Uint32 x, Uint32 z)
{
	Uint8 *metatiles = ((Uint8 *)&SceneStateWork);

	if(x >= CHANNEL_COLUMNS || z >= CHANNEL_ROWS)
		return 0;
	{
		PUZZLE_METATILE *metatile = (PUZZLE_METATILE *)(metatiles +
			(z * CHANNEL_COLUMNS + x) * sizeof(PUZZLE_METATILE));
		Sint32			 code;

		if(metatile->occupied != BLOCK_NONE)
			return 1;
		if(metatile->blocked != 0)
			return 1;
		code = metatile->kind;
		return code == PIECE_STOP;
	}
}

/* Whether a metatile beside channel metatile (x, z) is taken. */
Bool scGabombaStatueHasTakenNeighbor(Sint32 x, Sint32 z)
{
	if(scGabombaStatueIsMetatileTaken(x, z - 1) ||
		scGabombaStatueIsMetatileTaken(x, z + 1) ||
		scGabombaStatueIsMetatileTaken(x - 1, z))
		return TRUE;
	if(scGabombaStatueIsMetatileTaken(x + 1, z))
		return TRUE;
	return FALSE;
}

/* Redraws the channel as the party left it: a raised block's socket open, the
   other blocks in place, and once the Gabomba has responded every block set
   back in. */
void scGabombaStatueRedrawChannel(void)
{
	SCENE_STATE *state = &SceneStateWork;
	Uint32		 row;
	Uint32		 column;

	for(row = 0; row < CHANNEL_ROWS; row++)
		for(column = 0; column < CHANNEL_COLUMNS; column++) {
			PUZZLE_METATILE *metatile =
				state->metatiles[0] + (row * CHANNEL_COLUMNS + column);
			Sint32 occupied = metatile->occupied;

			if(fdScriptGetFlag(FLAG_GABOMBA_RESPONDED) &&
				metatile->occupied == BLOCK_RAISED) {
				metatile->occupied = BLOCK_IN_PLACE;
				occupied = BLOCK_IN_PLACE;
			}
			if(occupied == BLOCK_RAISED)
				fdMapGridCopyMetatiles(BLOCK_NONE, PIECE_SOCKET, 1, 1,
					state->originX + column, state->originZ + row);
			else if(occupied == BLOCK_IN_PLACE)
				fdMapGridCopyMetatiles(BLOCK_IN_PLACE,
					state->metatiles[row][column].kind, 1, 1,
					state->originX + column, state->originZ + row);
			else
				fdMapGridCopyMetatiles(occupied,
					state->metatiles[row][column].kind, 1, 1,
					state->originX + column, state->originZ + row);
		}
}

Sint32 scGabombaStatueTurnBlockPiece(Sint32 x, Sint32 z, Sint32 direction);

/* The metatiles of the charges' paths to the statue in the middle: the two sent
   east run EAST_PATH_METATILES and the two sent west WEST_PATH_METATILES, so
   that a blue and a red charge arrive together. The paths from the statues'
   inner sides meet after PATH_TO_CROSSING_METATILES on the crossing's own
   metatile. */
enum {
	EAST_PATH_METATILES = 17,
	WEST_PATH_METATILES = 19,
	PATH_TO_CROSSING_METATILES = 4
};

/* Lays the channel from seed, trying the seeds after it until each charge's
   path reaches the statue in the middle as the puzzle needs, then breaks the
   circuit and loosens the seven blocks, turned at random. Returns the seed it
   was laid from. */
Sint32 scGabombaStatueLayChannel(Sint32 seed)
{
	SCENE_STATE *state = &SceneStateWork;
	Sint32 row, column, n, blueShared, redShared, crossed, tries, blocksLeft,
		westward;
	Sint32			 piece, paths;
	PUZZLE_METATILE *metatile;

	state->seed = seed - 1;
retry:
	state->seed += 1;
	RandomSeed = state->seed;
	syWordsClearEntry(state, sizeof(state->metatiles));
	state->metatiles[CHANNEL_MIDDLE_STATUE_ROW][CHANNEL_MIDDLE_STATUE_COLUMN]
		.kind = PIECE_STOP;
	state->metatiles[CHANNEL_STATUES_ROW][CHANNEL_BLUE_STATUE_COLUMN].kind =
		PIECE_STOP;
	state->metatiles[CHANNEL_STATUES_ROW][CHANNEL_RED_STATUE_COLUMN].kind =
		PIECE_STOP;
	state->metatiles[CHANNEL_STATUES_ROW][CHANNEL_BLUE_STATUE_COLUMN - 1]
		.blocked = 1;
	state->metatiles[CHANNEL_STATUES_ROW][CHANNEL_BLUE_STATUE_COLUMN + 1]
		.blocked = 1;
	state->metatiles[CHANNEL_STATUES_ROW][CHANNEL_RED_STATUE_COLUMN - 1]
		.blocked = 1;
	state->metatiles[CHANNEL_STATUES_ROW][CHANNEL_RED_STATUE_COLUMN + 1]
		.blocked = 1;
	for(row = 0; row < CHANNEL_ROWS; row++)
		for(column = 0; column < CHANNEL_COLUMNS; column++)
			fdMapGridCopyMetatiles(BLOCK_NONE,
				state->metatiles[row][column].kind, 1, 1,
				state->originX + column, state->originZ + row);
	/* The charges sent east from the blue statue and west from the red cross on
	   a piece of two curves between the statues, the crossing's metatile
	   counted in neither part of their paths. */
	n = 0;
	if(syRandom() < 32768) {
		n |= scGabombaStatueLayPath(2, 7, 5, 7, PATH_TO_CROSSING_METATILES,
			HEADING_EAST, HEADING_NORTH, PATH_BIT_BLUE_EAST);
		n |= scGabombaStatueLayPath(8, 7, 6, 6, PATH_TO_CROSSING_METATILES,
			HEADING_WEST, HEADING_WEST, PATH_BIT_RED_WEST);
		scGabombaStatueWritePiece(5, 6, PIECE_SOUTH_WEST_AND_NORTH_EAST,
			PATH_BLUE_EAST | PATH_RED_WEST);
		n |= scGabombaStatueLayPath(4, 6, 4, 3,
			EAST_PATH_METATILES - PATH_TO_CROSSING_METATILES - 1, HEADING_WEST,
			HEADING_EAST, PATH_BIT_BLUE_EAST);
		n |= scGabombaStatueLayPath(5, 5, 5, 2,
			WEST_PATH_METATILES - PATH_TO_CROSSING_METATILES - 1, HEADING_NORTH,
			HEADING_SOUTH, PATH_BIT_RED_WEST);
	} else {
		n |= scGabombaStatueLayPath(2, 7, 4, 6, PATH_TO_CROSSING_METATILES,
			HEADING_EAST, HEADING_EAST, PATH_BIT_BLUE_EAST);
		n |= scGabombaStatueLayPath(8, 7, 5, 7, PATH_TO_CROSSING_METATILES,
			HEADING_WEST, HEADING_NORTH, PATH_BIT_RED_WEST);
		scGabombaStatueWritePiece(5, 6, PIECE_EAST_SOUTH_AND_WEST_NORTH,
			PATH_BLUE_EAST | PATH_RED_WEST);
		n |= scGabombaStatueLayPath(5, 5, 4, 3,
			EAST_PATH_METATILES - PATH_TO_CROSSING_METATILES - 1, HEADING_NORTH,
			HEADING_EAST, PATH_BIT_BLUE_EAST);
		n |= scGabombaStatueLayPath(6, 6, 5, 2,
			WEST_PATH_METATILES - PATH_TO_CROSSING_METATILES - 1, HEADING_EAST,
			HEADING_SOUTH, PATH_BIT_RED_WEST);
	}
	if(n != 0)
		goto retry;
	n = scGabombaStatueLayPath(0, 7, 5, 4, WEST_PATH_METATILES, HEADING_WEST,
		HEADING_NORTH, PATH_BIT_BLUE_WEST);
	n |= scGabombaStatueLayPath(10, 7, 6, 3, EAST_PATH_METATILES, HEADING_EAST,
		HEADING_WEST, PATH_BIT_RED_EAST);
	if(n != 0)
		goto retry;
	/* The two blue paths must share a metatile, the two red ones too, and the
	   crossing must stand. */
	blueShared = 0;
	redShared = 0;
	crossed = 0;
	for(row = 0; row < CHANNEL_ROWS; row++)
		for(column = 0; column < CHANNEL_COLUMNS; column++) {
			piece = state->metatiles[row][column].kind;
			paths = state->metatiles[row][column].state;
			if(paths == PATH_BLUE)
				blueShared++;
			if(paths == PATH_RED)
				redShared++;
			if(paths == (PATH_BLUE_EAST | PATH_RED_WEST) &&
				(Uint32)(piece - PIECE_EAST_SOUTH_AND_WEST_NORTH) <= 1)
				crossed++;
		}
	if(blueShared == 0 || redShared == 0 || crossed == 0)
		goto retry;
	/* Breaks the circuit: a straight piece near the statues, on the eastward
	   paths or the westward ones, becomes a block with no piece on it. */
	westward = syRandomBelow(2);
	blocksLeft = CHANNEL_BLOCKS;
	for(tries = 1; tries <= CHANNEL_METATILES; tries++) {
		column = syRandomBelow(CHANNEL_COLUMNS);
		row = (syRandomBelow(3)) + 5;
		if(scGabombaStatueHasTakenNeighbor(column, row) == 0) {
			piece = state->metatiles[row][column].kind;
			if((Uint32)(piece - PIECE_NORTH_SOUTH) <= 1) {
				paths = state->metatiles[row][column].state;
				if((westward == 0 &&
					   (paths & (PATH_RED_EAST | PATH_BLUE_EAST)) != 0) ||
					(westward == 1 &&
						(paths & (PATH_BLUE_WEST | PATH_RED_WEST)) != 0)) {
					state->metatiles[row][column].kind = PIECE_STOP;
					state->metatiles[row][column].occupied = BLOCK_IN_PLACE;
					blocksLeft = CHANNEL_BLOCKS - 1;
					break;
				}
			}
		}
	}
	/* Four crossings or pieces of two curves lose one of their ways as
	   blocks. */
	for(n = 0; n < 4; n++)
		for(tries = 1; tries <= CHANNEL_METATILES; tries++) {
			column = syRandomBelow(CHANNEL_COLUMNS);
			row = syRandomBelow(CHANNEL_ROWS);
			if(scGabombaStatueHasTakenNeighbor(column, row) == 0) {
				piece = state->metatiles[row][column].kind;
				if(piece == PIECE_CROSSING ||
					piece == PIECE_EAST_SOUTH_AND_WEST_NORTH ||
					piece == PIECE_SOUTH_WEST_AND_NORTH_EAST) {
					metatile = &state->metatiles[row][column];
					paths = metatile->state;
					if(paths != (PATH_RED_EAST | PATH_BLUE_EAST) &&
						paths != (PATH_BLUE_WEST | PATH_RED_WEST)) {
						switch(piece) {
						case PIECE_CROSSING:
							metatile->kind =
								(syRandomBelow(2)) + PIECE_NORTH_SOUTH;
							break;
						case PIECE_EAST_SOUTH_AND_WEST_NORTH:
							metatile->kind =
								((syRandomBelow(2)) << 1) + PIECE_EAST_SOUTH;
							break;
						case PIECE_SOUTH_WEST_AND_NORTH_EAST:
							metatile->kind =
								((syRandomBelow(2)) << 1) + PIECE_SOUTH_WEST;
							break;
						}
						state->metatiles[row][column].occupied = BLOCK_IN_PLACE;
						blocksLeft--;
						break;
					}
				}
			}
		}
	/* One curve, then straight pieces for the blocks blocksLeft. */
	for(tries = 1; tries <= CHANNEL_METATILES; tries++) {
		column = syRandomBelow(CHANNEL_COLUMNS);
		row = syRandomBelow(CHANNEL_ROWS);
		if(scGabombaStatueHasTakenNeighbor(column, row) == 0) {
			piece = state->metatiles[row][column].kind;
			if(piece == PIECE_EAST_SOUTH || piece == PIECE_WEST_NORTH ||
				piece == PIECE_SOUTH_WEST || piece == PIECE_NORTH_EAST) {
				state->metatiles[row][column].occupied = BLOCK_IN_PLACE;
				blocksLeft--;
				break;
			}
		}
	}
	for(n = 0; n < blocksLeft; n++)
		for(tries = 1; tries <= CHANNEL_METATILES; tries++) {
			column = syRandomBelow(CHANNEL_COLUMNS);
			row = syRandomBelow(CHANNEL_ROWS);
			if(scGabombaStatueHasTakenNeighbor(column, row) == 0) {
				piece = state->metatiles[row][column].kind;
				if((Uint32)(piece - PIECE_NORTH_SOUTH) <= 1) {
					state->metatiles[row][column].occupied = BLOCK_IN_PLACE;
					break;
				}
			}
		}
	/* Turns each block at random, and draws the channel; paths now holds what
	   stands on each metatile. */
	for(row = 0; row < CHANNEL_ROWS; row++)
		for(column = 0; column < CHANNEL_COLUMNS; column++) {
			piece = state->metatiles[row][column].kind;
			paths = state->metatiles[row][column].occupied;
			if(paths != BLOCK_NONE) {
				switch(syRandomBelow(8)) {
				case 0:
				case 1:
				case 2:
					piece = scGabombaStatueTurnBlockPiece(
						column, row, TURN_COUNTERCLOCKWISE);
					break;
				case 3:
				case 4:
				case 5:
					piece = scGabombaStatueTurnBlockPiece(
						column, row, TURN_CLOCKWISE);
					break;
				case 6:
					scGabombaStatueTurnBlockPiece(
						column, row, TURN_COUNTERCLOCKWISE);
					piece = scGabombaStatueTurnBlockPiece(
						column, row, TURN_COUNTERCLOCKWISE);
					break;
				}
			}
			fdMapGridCopyMetatiles(paths, piece, 1, 1, state->originX + column,
				state->originZ + row);
		}
	return state->seed;
}

/* How long scGabombaStatueLayPath searches: the steps it takes in all before it
   gives up, and the random headings it tries from a metatile before backing
   up. */
enum { PATH_ATTEMPTS_MAX = 1280, PATH_TRIES_PER_STEP = 63 };

/* The chance, out of syRandom's 65,536, that a try keeps the path's heading;
   the other half of the time it tries a random one. */
#define PATH_KEEP_HEADING_CHANCE 32768

/* scGabombaStatueLayPath's steps so far: each step's heading, metatile, piece,
   headings tried and the paths through its metatile before it was joined. */
Uint8		 GabombaStatuePathHeadings[32] BSS_DATA;
Uint8		 GabombaStatuePathColumns[32] BSS_DATA;
Uint8		 GabombaStatuePathRows[32] BSS_DATA;
Uint8		 GabombaStatuePathPieces[32] BSS_DATA;
Uint8		 GabombaStatuePathTried[32] BSS_DATA;
Uint8		 GabombaStatuePathMetatilePaths[32] BSS_DATA;
extern Sint8 GabombaStatuePathSteps[];
extern Uint8 GabombaStatuePieceJoins[];

/* Lays a random path of `length` metatiles across the channel from (x0, z0) to
   (endX, endZ), the pieces it passes joined to it, backing up a step when a
   metatile has no free way on. Returns 0, or -1 when the distance's parity
   rules such a path out or the search gives up. */
Sint32 scGabombaStatueLayPath(Sint32 x0, Sint32 z0, Sint32 endX, Sint32 endZ,
	Sint32 length, Sint32 direction, Sint32 last, Sint32 bit)
{
	Uint8 *metatiles, *kinds;
	Sint32 piece;
	Sint32 spare, step, heading, used, dz, index, one;
	Uint32 x, z, nextX, nextZ, tries, attempts;
	Uint8  tried;

	length--;
	dz = endZ - z0;
	spare = length - abs(endX - x0) - abs(dz);
	syWordsClearEntry(
		GabombaStatuePathHeadings, sizeof(GabombaStatuePathHeadings));
	syWordsClearEntry(
		GabombaStatuePathColumns, sizeof(GabombaStatuePathColumns));
	syWordsClearEntry(GabombaStatuePathRows, sizeof(GabombaStatuePathRows));
	syWordsClearEntry(GabombaStatuePathPieces, sizeof(GabombaStatuePathPieces));
	syWordsClearEntry(GabombaStatuePathTried, sizeof(GabombaStatuePathTried));
	syWordsClearEntry(
		GabombaStatuePathMetatilePaths, sizeof(GabombaStatuePathMetatilePaths));
	if(spare & 1)
		return -1;
	x = x0;
	metatiles = (Uint8 *)((PUZZLE_METATILE *)&SceneStateWork);
	one = 1;
	step = 0;
	attempts = 0;
	z = z0;
	while(1) {
		if(++attempts > PATH_ATTEMPTS_MAX)
			return -1;
		{
			Sint32 offset = (z * CHANNEL_COLUMNS + x) * sizeof(PUZZLE_METATILE);
			GabombaStatuePathPieces[step] =
				((PUZZLE_METATILE *)(metatiles + offset))->kind;
			GabombaStatuePathMetatilePaths[step] =
				((PUZZLE_METATILE *)(metatiles + offset))->state;
		}
		GabombaStatuePathColumns[step] = x;
		GabombaStatuePathRows[step] = z;
		GabombaStatuePathHeadings[step] = direction;
		{
			Uint8 *joins = GabombaStatuePieceJoins;
			kinds = GabombaStatuePathPieces;
			GabombaStatuePathTried[step] = GabombaStatuePathTried[step] |
				joins[kinds[step]] | 1 << ((direction + 2) & 3);
		}
		tries = 0;
		while(1) {
			Uint32 value;
			if(++tries <= PATH_TRIES_PER_STEP) {
				value = syRandom();
				heading = direction;
				if(value >= PATH_KEEP_HEADING_CHANCE)
					heading = syRandomBelow(4);
				tried = GabombaStatuePathTried[step];
				if((tried >> heading) & one)
					continue;
				{
					Sint8 *steps = GabombaStatuePathSteps;
					index = heading * 2;
					nextX = x + steps[index];
					index += 1;
					nextZ = z + steps[index];
				}
				if(nextX < CHANNEL_COLUMNS && nextZ < CHANNEL_ROWS &&
					!(step == length - 1 && (nextX != endX || nextZ != endZ))) {
					PUZZLE_METATILE *metatile = (PUZZLE_METATILE *)(metatiles +
						(nextZ * CHANNEL_COLUMNS + nextX) *
							sizeof(PUZZLE_METATILE));
					if(metatile->blocked == 0 &&
						(used = (metatile->state >> bit) & one) == 0 &&
						metatile->kind != PIECE_STOP) {
						tried |= one << heading;
						GabombaStatuePathTried[step] = tried;
						kinds = GabombaStatuePathPieces;
						piece = scGabombaStatuePickPiece(
							direction, heading, kinds[step]);
						{
							Sint32 offset = (z * CHANNEL_COLUMNS + x) *
								sizeof(PUZZLE_METATILE);
							scGabombaStatueWritePiece(x, z, piece,
								((PUZZLE_METATILE *)(metatiles + offset))
										->state |
									one << bit);
						}
						step++;
						x = nextX;
						z = nextZ;
						direction = heading;
						if(step == length)
							goto done;
						GabombaStatuePathTried[step] = 0;
						break;
					}
				}
				GabombaStatuePathTried[step] |= 1 << heading;
				if(GabombaStatuePathTried[step] != HEADING_ALL_TRIED)
					continue;
			}
			GabombaStatuePathTried[step] = 0;
			if(step == 0)
				return -1;
			step--;
			x = GabombaStatuePathColumns[step];
			z = GabombaStatuePathRows[step];
			direction = GabombaStatuePathHeadings[step];
			scGabombaStatueWritePiece(x, z, GabombaStatuePathPieces[step],
				GabombaStatuePathMetatilePaths[step]);
			break;
		}
	}
done: {
	Sint32 offset = (nextZ * CHANNEL_COLUMNS + nextX) * sizeof(PUZZLE_METATILE);
	piece = scGabombaStatuePickPiece(
		heading, last, ((PUZZLE_METATILE *)(metatiles + offset))->kind);
	scGabombaStatueWritePiece(nextX, nextZ, piece,
		((PUZZLE_METATILE *)(metatiles + offset))->state | 1 << bit);
}
	return 0;
}

extern Uint8 GabombaStatuePieceTable[];
extern Uint8 GabombaStatueTurnedPieces[];

/* The piece joining the heading a path enters a metatile by to the one it
   leaves by, merged with the piece already there. */
Sint32 scGabombaStatuePickPiece(Sint32 enter, Sint32 leave, Sint32 existing)
{
	Sint32 value = GabombaStatuePieceTable[enter * 4 + leave];

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

/* Puts piece kind, with the paths state, at channel metatile (x, z). */
void scGabombaStatueWritePiece(Sint32 x, Sint32 z, Sint32 kind, Sint32 state)
{

	SceneStateWork.metatiles[z][x].kind = kind;
	SceneStateWork.metatiles[z][x].state = state;
}

/* Raises the blocks over the room, their sockets open, or sets them in place:
   each block's actor, numbered from id, stands over its metatile while it is
   raised. */
void scGabombaStatueSetBlocksRaised(Sint32 id, Sint32 show)
{
	SCENE_STATE *state = &SceneStateWork;
	Sint32		 row;
	Sint32		 column;

	state->blocksRaised = show;
	state->firstBlock = id;
	for(row = 0; row < CHANNEL_ROWS; row++) {
		for(column = 0; column < CHANNEL_COLUMNS; column++) {
			{
				PUZZLE_METATILE *metatile =
					state->metatiles[0] + (row * CHANNEL_COLUMNS + column);

				if(metatile->occupied != 0) {
					ACTOR *actor = fdActorGet(id);
					Sint32 mapX;
					Sint32 mapZ;
					Sint32 kind;

					if(show != 0) {
						actor->x.fixed =
							FIELD_METATILE_CENTER(state->originX + column);
						actor->z.fixed =
							FIELD_METATILE_CENTER(state->originZ + row);
						fdMapGridCopyMetatiles(BLOCK_NONE, PIECE_SOCKET, 1, 1,
							state->originX + column, state->originZ + row);
						metatile->occupied = BLOCK_RAISED;
					} else {
						actor->x.fixed = 0;
						actor->z.fixed = 0;
						state->metatiles[row][column].occupied = BLOCK_IN_PLACE;
						fdMapGridCopyMetatiles(BLOCK_IN_PLACE, metatile->kind,
							1, 1, state->originX + column,
							state->originZ + row);
					}
					id++;
				}
			}
		}
	}
	state->endBlock = id;
}

/* Turns the block at channel metatile (x, z) a quarter turn, and returns its
   new piece. */
Sint32 scGabombaStatueTurnBlockPiece(Sint32 x, Sint32 z, Sint32 direction)
{

	SceneStateWork.metatiles[z][x].kind =
		GabombaStatueTurnedPieces[SceneStateWork.metatiles[z][x].kind * 2 +
			direction];
	return SceneStateWork.metatiles[z][x].kind;
}

/* A step of the Pound on the arrows room's stake: it sinks, and as it ends the
   blocks rise. */
void scGabombaStatueRaiseBlocks(Sint32 landed, Sint32 frame)
{
	ACTOR *actor = fdActorGet(ACTOR_ARROWS_STAKE);

	if(frame < POUND_LAND_TICK)
		actor->scaleY += -toFIXED(31) / 256;
	if(landed == POUND_STAGE_LANDED) {
		scGabombaStatueSetBlocksRaised(SceneStateWork.firstBlock, 1);
		fdActorSelectAnimation(actor, 6);
		actor->scaleY = toFIXED(1);
	}
}

/* Pounding a raised block sets it back into its place in the channel as the
   fist lands. */
void scGabombaStatuePoundBlock(Sint32 landed)
{
	SCENE_STATE		*state;
	ACTOR			*actor;
	PUZZLE_METATILE *metatile;
	Sint32			 x, z;
	Uint32			 row, column;
	Sint32			 found;

	if(landed != POUND_STAGE_LANDED)
		return;
	state = &SceneStateWork;
	actor = fdActorGet(
		((FIELD_EFFECT_STATE *)RamHeapSlots->fieldEffectState)->targetId);
	x = FIELD_METATILE(actor->x.fixed) - state->originX;
	z = FIELD_METATILE(actor->z.fixed) - state->originZ;
	{
		Sint32 kind = state->metatiles[z][x].kind;

		metatile = &state->metatiles[z][x];
		metatile->occupied = BLOCK_IN_PLACE;
		fdMapGridCopyMetatiles(
			BLOCK_IN_PLACE, kind, 1, 1, state->originX + x, state->originZ + z);
	}
	actor->x.fixed = 0;
	actor->z.fixed = 0;
	found = 0;
	row = 0;
	for(row = 0; row < CHANNEL_ROWS; row++) {
		for(column = 0; column < CHANNEL_COLUMNS; column++) {
			if(state->metatiles[row][column].occupied == BLOCK_RAISED) {
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
void scGabombaStatueSetBlockAnimation(Sint32 animation)
{
	fdActorSetAnimationById(SceneStateWork.firstBlock, animation);
}

/* Turns every raised block a quarter turn. */
void scGabombaStatueTurnBlocks(Sint32 direction)
{
	SCENE_STATE *state = &SceneStateWork;
	Sint32		 id;

	for(id = state->firstBlock; id < state->endBlock; id++) {
		ACTOR *actor = fdActorGet(id);

		scGabombaStatueTurnBlockPiece(
			FIELD_METATILE(actor->x.fixed) - state->originX,
			FIELD_METATILE(actor->z.fixed) - state->originZ, direction);
	}
}

/* The ticks the raised blocks take to turn a quarter turn. */
enum { BLOCK_TURN_TICKS = 20 };

/* The arrow panels' metatiles, which the map keeps at metatiles (0, 12) to (1,
   13): a column for each panel, a row for each of its two states. */
enum {
	RIGHT_ARROW_PANEL_COLUMN = 0,
	LEFT_ARROW_PANEL_COLUMN = 1,
	ARROW_PANEL_RAISED_ROW = 12,
	ARROW_PANEL_PRESSED_ROW = 13
};

/* Stepping onto the left arrow panel presses it, and with the blocks raised
   turns them a quarter turn counterclockwise. */
void scGabombaStatuePressLeftArrowPanel(void)
{
	SCENE_STATE *state = &SceneStateWork;
	Sint32		 i;

	if(state->leftArrowDown == 0) {
		fdMapGridCopyMetatilesLayered(LEFT_ARROW_PANEL_COLUMN,
			ARROW_PANEL_PRESSED_ROW, 1, 1, state->originX + 3,
			state->originZ - 2);
		if(state->blocksRaised != 0) {
			fdScriptPlaySound(SE_GABOMBA_STATUE_ARROW);
			fdScriptStart();
			syFieldScriptEndLastingPsynergy(0);
			scGabombaStatueSetBlockAnimation(BLOCK_ANIM_TURN_COUNTERCLOCKWISE);
			for(i = 0; i < BLOCK_TURN_TICKS; i++) {
				Sint32 *offset = &state->blocksAngle;

				*offset -= DEGtoANG(90) / BLOCK_TURN_TICKS;
				syWait(1);
			}
			scGabombaStatueTurnBlocks(0);
			fdScriptEnd();
			state->blocksAngle = 0;
		}
		state->leftArrowDown = 1;
	}
}

/* Stepping off the left arrow panel raises it again. */
void scGabombaStatueReleaseLeftArrowPanel(void)
{
	if(SceneStateWork.leftArrowDown != 0) {
		fdMapGridCopyMetatilesLayered(LEFT_ARROW_PANEL_COLUMN,
			ARROW_PANEL_RAISED_ROW, 1, 1, SceneStateWork.originX + 3,
			SceneStateWork.originZ - 2);
		SceneStateWork.leftArrowDown = 0;
	}
}

/* The right arrow panel: as the left, turning the blocks clockwise. */
void scGabombaStatuePressRightArrowPanel(void)
{
	SCENE_STATE *state = &SceneStateWork;
	Sint32		 i;

	if(state->rightArrowDown == 0) {
		fdMapGridCopyMetatilesLayered(RIGHT_ARROW_PANEL_COLUMN,
			ARROW_PANEL_PRESSED_ROW, 1, 1, state->originX + 7,
			state->originZ - 2);
		if(state->blocksRaised != 0) {
			fdScriptPlaySound(SE_GABOMBA_STATUE_ARROW);
			fdScriptStart();
			syFieldScriptEndLastingPsynergy(0);
			scGabombaStatueSetBlockAnimation(BLOCK_ANIM_TURN_CLOCKWISE);
			for(i = 0; i < BLOCK_TURN_TICKS; i++) {
				Sint32 *offset = &state->blocksAngle;

				*offset += DEGtoANG(90) / BLOCK_TURN_TICKS;
				syWait(1);
			}
			scGabombaStatueTurnBlocks(1);
			fdScriptEnd();
			state->blocksAngle = 0;
		}
		state->rightArrowDown = 1;
	}
}

/* Stepping off the right arrow panel raises it again. */
void scGabombaStatueReleaseRightArrowPanel(void)
{
	if(SceneStateWork.rightArrowDown != 0) {
		fdMapGridCopyMetatilesLayered(RIGHT_ARROW_PANEL_COLUMN,
			ARROW_PANEL_RAISED_ROW, 1, 1, SceneStateWork.originX + 7,
			SceneStateWork.originZ - 2);
		SceneStateWork.rightArrowDown = 0;
	}
}

/* A raised block's slot in the depth list past its position's, as
   fdRenderApplyProjectedPlacement queues an actor's sprite 62 past its own and
   the actor's shadow 60. */
enum { RAISED_BLOCK_DEPTH = 58 };

/* Each tick: draws the raised blocks over the room, turning while an arrow
   panel turns them. */
void scGabombaStatueDrawRaisedBlocks(void)
{
	Sint32		 affine = 0;
	SCENE_STATE *state = &SceneStateWork;
	SPR_VIEW	*oam = state->oam;
	Sint32		 scrollX =
		syMapStateView(RamHeapSlots->mapGrid)[0] & FIELD_PIXEL_MASK;
	Sint32 scrollZ =
		syMapStateView(RamHeapSlots->mapGrid)[1] & FIELD_PIXEL_MASK;
	Uint16			 tile = VramCacheEntries[state->blockTileEntry].offset >> 5;
	AFFINE_TRANSFORM effect;
	Sint32			 id;

	if(state->blocksAngle != 0) {
		effect.angle = state->blocksAngle;
		effect.scaleX = Q_8_8(1);
		effect.scaleY = Q_8_8(1);
		affine = syOAMMatrixAllocateFromTransform(&effect);
	}
	for(id = state->firstBlock; id < state->endBlock; id++) {
		ACTOR *piece = fdActorGet(id);
		FIXED  px = piece->x.fixed;
		if(px != 0) {
			Sint32	x = px - scrollX;
			FIXED	y = piece->y.fixed + toFIXED(16);
			FIXED	pz = piece->z.fixed;
			FIXED	z = pz - scrollZ;
			Sint32	sy = z - y;
			Sint32	depth = FIXEDtoINT(y + z) + RAISED_BLOCK_DEPTH;
			Sint32	metatileX = FIELD_METATILE(px) - state->originX;
			Sint32	metatileZ = FIELD_METATILE(pz) - state->originZ;
			Sint32	kind = state->metatiles[metatileZ][metatileX].kind;
			Uint32 *p;
			x = (FIXEDtoINT(x) - 8) & OBJ_X_MASK;
			sy = (FIXEDtoINT(sy) - 8) & OBJ_Y_MASK;
			p = (Uint32 *)oam;
			*p++ = 0;
			*p++ = sy | OBJ_X(x) | OBJ_SIZE_16X16 | OBJ_256_COLORS;
			*p = (tile + (kind << 3)) | OBJ_PRIORITY(2);
			if(state->blocksAngle != 0) {
				oam->affineMode = 1;
				oam->matrix = affine;
			}
			syOAMAddSpriteAtDepthClamped(oam, depth);
			oam++;
		}
	}
}

/* Sets up the channel puzzle on entering the room, or redraws it when
   returning. */
Sint32 scGabombaStatueSetUpChannelPuzzle(
	Sint32 x, Sint32 z, Sint32 seed, Sint32 id)
{
	SCENE_STATE *state = &SceneStateWork;
	Sint32		 laidSeed;

	if(fdScriptGetFlag(FLAG_KEEP_PARTY_POSITION)) {
		scGabombaStatueLoadBlockPictures();
		scGabombaStatueRedrawChannel();
		laidSeed = state->seed;
		sySetTickEvent(scGabombaStatueDrawRaisedBlocks, EVENT_PASS_TICK);
	} else {
		syWordsClearEntry(state, sizeof(SCENE_STATE));
		state->originX = x;
		state->originZ = z;
		scGabombaStatueLoadBlockPictures();
		laidSeed = scGabombaStatueLayChannel(seed);
		scGabombaStatueSetBlocksRaised(id, 0);
		sySetTickEvent(scGabombaStatueDrawRaisedBlocks, EVENT_PASS_TICK);
	}
	fdActorGet(id)->retainSpriteCache = ACTOR_COMPOSE_OFF_SCREEN;
	return laidSeed;
}

extern Sint32 GabombaStatueChargeShrinkScript[];
extern Uint32 GabombaStatueChargeBurstScript[];

/* The ticks a charge takes to cross a metatile of the channel, and the
   metatiles a run of the charges crosses at most. */
enum { CHARGE_STEPS_PER_METATILE = 16, CHARGE_METATILES_MAX = 128 };

/* A tick of a charge's run along the channel: it follows the piece under it,
   bursts where that piece does not go on its way, and shrinks away where it is
   stopped short of the statue in the middle or reaches a raised block's socket.
   The actor id scGabombaStatueRunCharges passes is never read. */
void scGabombaStatueMoveCharge(ACTOR *charge, Sint32 step, Sint32 id)
{
	Sint16		*colp = &charge->chargeColumn;
	Sint32		 column = *colp;
	Sint16		*rowp = &charge->chargeRow;
	Sint32		 row = *rowp;
	Uint8		*headingp = &charge->chargeHeading;
	Sint32		 heading = *headingp;
	Sint32		 angle = heading * DEGtoANG(90);
	Sint32		 metatile = row * CHANNEL_COLUMNS + column;
	SCENE_STATE *state = &SceneStateWork;
	Sint32		 piece = state->metatiles[row][column].kind;
	Sint32		 goal = state->metatiles[row][column].occupied;
	Sint32		 key = CHARGE_STEP(heading, piece);
	Sint32		 d = step - MAPGRID_METATILE_PIXELS / 2;
	Sint32		 arc = step * (DEGtoANG(90) / CHARGE_STEPS_PER_METATILE);
	FIXED		 s = syMathMulFX(toFIXED(8), sySin(arc));
	FIXED		 c = syMathMulFX(toFIXED(8), syCos(arc));
	Sint32		 z;
	Sint32		 x;

	x = FIELD_METATILE_CENTER(state->originX + column);
	z = FIELD_METATILE_CENTER(state->originZ + row);
	if(goal == BLOCK_RAISED) {
		heading = row = column = 0;
		if(charge->chargeState == CHARGE_RUNNING) {
			sySoundPlay(SE_DUST_PUFF);
			fdActorStartScript(charge, GabombaStatueChargeShrinkScript);
			charge->chargeState = goal;
		}
	} else {
		switch(key) {
		case CHARGE_STEP(HEADING_WEST, PIECE_EAST_WEST):
		case CHARGE_STEP(HEADING_WEST, PIECE_CROSSING):
			column--;
			x -= toFIXED(d);
			break;
		case CHARGE_STEP(HEADING_EAST, PIECE_EAST_WEST):
		case CHARGE_STEP(HEADING_EAST, PIECE_CROSSING):
			column++;
			x += toFIXED(d);
			break;
		case CHARGE_STEP(HEADING_SOUTH, PIECE_NORTH_SOUTH):
		case CHARGE_STEP(HEADING_SOUTH, PIECE_CROSSING):
			row++;
			z += toFIXED(d);
			break;
		case CHARGE_STEP(HEADING_NORTH, PIECE_NORTH_SOUTH):
		case CHARGE_STEP(HEADING_NORTH, PIECE_CROSSING):
			row--;
			z -= toFIXED(d);
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
			column--;
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
			column++;
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
			column--;
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
			column++;
			heading = HEADING_EAST;
			x = x - c + toFIXED(8);
			z = z - s + toFIXED(8);
			angle = arc + TRIG_TURN * 3 / 4;
			break;
		case CHARGE_STEP(HEADING_EAST, PIECE_SOCKET):
			heading = row = column = 0;
			break;
		case CHARGE_STEP(HEADING_SOUTH, PIECE_SOCKET):
			heading = row = column = 0;
			break;
		case CHARGE_STEP(HEADING_WEST, PIECE_SOCKET):
			heading = row = column = 0;
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
				if(FIELD_METATILE(x) ==
						CHANNEL_ORIGIN_COLUMN + CHANNEL_MIDDLE_STATUE_COLUMN &&
					FIELD_METATILE(z) ==
						CHANNEL_ORIGIN_ROW + CHANNEL_MIDDLE_STATUE_ROW) {
					charge->chargeState = CHARGE_ARRIVED;
				} else {
					sySoundPlay(SE_DUST_PUFF);
					fdActorStartScript(charge, GabombaStatueChargeShrinkScript);
					charge->chargeState = CHARGE_LEAVING;
				}
			}
			break;
		case CHARGE_STEP(HEADING_NORTH, PIECE_SOCKET):
			heading = row = column = 0;
			break;
		default:
			heading = row = column = 0;
			if(charge->chargeState == CHARGE_RUNNING) {
				sySoundPlay(SE_WATER);
				fdActorStartScript(charge, GabombaStatueChargeBurstScript);
				charge->chargeState = CHARGE_LEAVING;
			}
			break;
		}
	}
	if(step == CHARGE_STEPS_PER_METATILE - 1) {
		*colp = column;
		*rowp = row;
		*headingp = heading;
	}
	if(charge->chargeState == CHARGE_RUNNING) {
		charge->y.fixed = 0;
		charge->x.fixed = x;
		charge->z.fixed = z;
		charge->facing = angle;
		/* sceneWord counts the ticks the charge has run. */
		charge->sceneWord++;
	}
}

/* Runs the four charges from the orb statues along the channel until each has
   stopped and, with check, tells how they reached the statue in the middle. */
Sint32 scGabombaStatueRunCharges(Sint32 first, Sint32 check)
{
	ACTOR *actor;
	Sint32 second = first + 1;
	Sint32 tick, tries, id;
	Bool   done;
	Sint32 result;

	actor = fdActorGet(first);
	actor->chargeColumn = CHANNEL_BLUE_STATUE_COLUMN - 1;
	actor->chargeRow = CHANNEL_STATUES_ROW;
	actor->chargeHeading = HEADING_WEST;
	actor->chargeState = CHARGE_RUNNING;
	actor->sceneWord = 0;
#if FIX_FUNCTION_SIGNATURES
	fdActorStartIdle(actor);
#else
	/* UB: called without the actor it starts, which it finds in r0 only because
	   fdActorGet left it there. */
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
	for(tries = 0; tries < CHARGE_METATILES_MAX; tries++) {
		done = TRUE;
		for(tick = 0; tick < CHARGE_STEPS_PER_METATILE; tick++) {
			for(id = first; id < first + 4; id++) {
				actor = fdActorGet(id);
				if(actor->chargeState != CHARGE_STOPPED &&
					actor->chargeState != CHARGE_ARRIVED)
					done = FALSE;
				scGabombaStatueMoveCharge(actor, tick, id);
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
		   ticks, the sooner of each orb's two. */
		Uint32 blue = 0;
		Uint32 red = 0;
		{
			ACTOR *r = fdActorGet(first);
			if(r->x.fixed != 0)
				blue = r->sceneWord;
		}
		{
			ACTOR *r = fdActorGet(second);
			if(r->x.fixed != 0) {
				Uint32 value = r->sceneWord;
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
				Uint32 value = r->sceneWord;
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
						fdScriptSetFlag(FLAG_GABOMBA_CHARGES_REACHED_APART);
						result = CHARGE_ARRIVAL_BLUE_FIRST;
					} else {
						fdScriptSetFlag(FLAG_GABOMBA_CHARGES_REACHED_APART);
						result = CHARGE_ARRIVAL_RED_FIRST;
					}
				}
			}
		} else {
			if(red != 0)
				return CHARGE_ARRIVAL_RED_FIRST;
			return CHARGE_ARRIVAL_NONE;
		}
	}
	return result;
}

/* Finds the metatile where the circuit is broken: a block with no piece on
   it. */
void scGabombaStatueFindBrokenMetatile(Uint32 *x_out, Uint32 *z_out)
{
	Uint32 z;
	Uint32 x;

	for(z = 0; z < CHANNEL_ROWS; z++) {
		for(x = 0; x < CHANNEL_COLUMNS; x++) {
			if(SceneStateWork.metatiles[z][x].kind == PIECE_STOP &&
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

/* The clock's digit sprite sheet: the pictures of 0 to 9 and the colon, 8 by 16
   pixels each, then the five shown. */
enum {
	CLOCK_PICTURE_BYTES = 64,
	CLOCK_SHEET_PICTURES = 11,
	CLOCK_SHOWN_PICTURES = 5
};

/* The depth the clock is drawn at, in front of the room's actors. */
#define CLOCK_DEPTH 236

/* The offering's countdown clock, MM:SS:T drawn as sprites: the digits that
   change shrink and grow back each second as it ticks. */
typedef struct CLOCK_WORK {
	Uint8	*tiles;	 /* the digit sprite sheet */
	Uint8	*digits; /* the five digit pictures, as loaded */
	Uint8	 old[8];
	Uint8	 now[8];
	Uint8	 digit[8];
	SPR_VIEW oam[8];
	Sint32	 tileEntry;
	Sint32	 timer;
	Uint8	 ringing;
	Uint8	 mode;
} CLOCK_WORK;

/* The clock's state, kept 256 bytes into the inventory snapshot's spare
   room. */
#define CLOCK_STATE \
	((CLOCK_WORK *)(InventorySnapshot + INVENTORY_SNAPSHOT_SPARE + 256))

/* Converts the remaining ticks into minutes, seconds and tenths
   (`MM:SS:T`). */
void scGabombaStatueFormatClock(Sint32 time, Uint8 *text)
{
	if(time < 0)
		time = 0;
	*text = time / (TICKS_PER_SECOND * 60 * 10) + '0';
	time %= (TICKS_PER_SECOND * 60 * 10);
	*++text = time / (TICKS_PER_SECOND * 60) + '0';
	time %= (TICKS_PER_SECOND * 60);
	*++text = ':';
	*++text = time / (TICKS_PER_SECOND * 10) + '0';
	time %= (TICKS_PER_SECOND * 10);
	*++text = time / TICKS_PER_SECOND + '0';
	time %= TICKS_PER_SECOND;
	*++text = ':';
	*++text = time / (TICKS_PER_SECOND / 10) + '0';
	text[1] = 0;
}

/* Plays the clock's tick each second and returns the scale of the changing
   digits, which shrink out of sight and grow back over the last third of each
   second, as the clock counts down to its tick. */
Sint32 scGabombaStatueTickClock(Sint32 time)
{
	CLOCK_WORK *clock = CLOCK_STATE;
	Sint32		tick = time % TICKS_PER_SECOND;
	Sint32		scale = Q_8_8(2);
	Uint8	   *ringing = &clock->ringing;

	if(tick != 0) {
		*ringing = 1;
	} else if(*ringing) {
		if(clock->timer != 0)
			fdScriptPlaySound(SE_COLOSSO_COUNTDOWN);
		*ringing = tick;
	}
	if(tick < TICKS_PER_SECOND / 3) {
		FIXED cosine = syCos(tick * (TRIG_TURN / 2) / 10);
		Sint32 (*mul)(Sint32 a, Sint32 b);
		Sint32 half = Q_8_8(0.5);

		mul = syMulFX;
		scale = mul(half << 2, cosine);
	}
	if(scale <= Q_8_8(1) / 16)
		scale = Q_8_8(1) / 16;
	return scale;
}

/* Draws the clock's minutes and seconds, doubled in size, the colon and each
   digit about to change taking the ticking scale. */
void scGabombaStatueDrawClock(void)
{
	CLOCK_WORK		*clock = CLOCK_STATE;
	Uint8			*digit = clock->digit;
	Uint8			*old = clock->tiles;
	Uint8			*now = clock->digits;
	Uint32			 tile = VramCacheEntries[clock->tileEntry].offset >> 5;
	SPR_VIEW		*oam = clock->oam;
	Sint32			*timer = &clock->timer;
	AFFINE_TRANSFORM effect;
	Sint32			 plain, scaled, i;
	Sint32			 scale;
	Uint32			*p;

	/* The digits shown move on 10 ticks before each second runs out, while
	   those changing have shrunk out of sight (scGabombaStatueTickClock); the
	   ones that differ from the time a second before are the ones that
	   shrink. */
	scGabombaStatueFormatClock(*timer + TICKS_PER_SECOND - 10, digit);
	scGabombaStatueFormatClock(*timer + TICKS_PER_SECOND, clock->old);
	scGabombaStatueFormatClock(*timer, clock->now);
	/* old and now walk the sprite sheet and the pictures here, the digit lists
	   below. */
	for(i = 0; i < CLOCK_SHOWN_PICTURES; i++) {
		syWordsCopyEntry(now,
			old + *digit * CLOCK_PICTURE_BYTES - '0' * CLOCK_PICTURE_BYTES,
			CLOCK_PICTURE_BYTES);
		now += CLOCK_PICTURE_BYTES;
		digit++;
	}
	mnVRAMLoad(clock->tileEntry, CLOCK_SHOWN_PICTURES * CLOCK_PICTURE_BYTES,
		clock->digits);
	effect.angle = 0;
	effect.scaleX = Q_8_8(2);
	effect.scaleY = Q_8_8(2);
	plain = syOAMMatrixAllocateFromTransform(&effect);
	scale = scGabombaStatueTickClock(*timer);
	effect.angle = 0;
	effect.scaleX = scale;
	effect.scaleY = scale;
	scaled = syOAMMatrixAllocateFromTransform(&effect);
	old = clock->old;
	now = clock->now;
	for(i = 0; i < CLOCK_SHOWN_PICTURES; i++, tile += 2, oam++) {
		p = (Uint32 *)oam;
		*p++ = 0;
		*p++ = OBJ_X(i * 16) | OBJ_SIZE_8X16;
		*p = tile | OBJ_PRIORITY(1);
		oam->affineMode = 3;
		if(*old++ != *now++ || i == 2)
			oam->matrix = scaled;
		else
			oam->matrix = plain;
		syOAMAddSpriteAtDepthClamped(oam, CLOCK_DEPTH);
	}
}

/* Counts the clock down while no menu is open, and draws it. */
void scGabombaStatueCountDownShowingClock(void)
{
	FIELD_STATE *state = RamHeapSlots->fieldState;
	CLOCK_WORK	*clock = CLOCK_STATE;

	if(state->psynergy == 0 && state->fieldMenu == 0 && state->pauseMenu == 0 &&
		clock->timer > 0)
		clock->timer--;
	scGabombaStatueDrawClock();
}

/* Counts the clock down while no menu is open. */
void scGabombaStatueCountDown(void)
{
	FIELD_STATE *state = RamHeapSlots->fieldState;
	CLOCK_WORK	*clock = CLOCK_STATE;

	if(state->psynergy == 0 && state->fieldMenu == 0 && state->pauseMenu == 0 &&
		clock->timer > 0)
		clock->timer--;
}

/* Starts the clock at one minute, shown or hidden. */
void scGabombaStatueStartClock(Sint32 mode)
{
	CLOCK_WORK *clock = CLOCK_STATE;
	Uint8	   *tiles;

	if(!fdScriptGetFlag(FLAG_KEEP_PARTY_POSITION)) {
		syWordsClearEntry(clock, sizeof(CLOCK_WORK));
		clock->timer = TICKS_PER_SECOND * 60;
		clock->ringing = 1;
	}
	tiles = syHeapAllocateEWRAM(1024);
	syDecompressTaggedLZ(syResourceGetTableEntry(RESOURCE_CLOCK_DIGITS), tiles);
	clock->tiles = tiles;
	clock->digits = tiles + CLOCK_SHEET_PICTURES * CLOCK_PICTURE_BYTES;
	clock->tileEntry = syVRAMCacheFindFreeEntry();
	mnVRAMLoad(
		clock->tileEntry, CLOCK_SHOWN_PICTURES * CLOCK_PICTURE_BYTES, NULL);
	clock->mode = mode;
	if(mode)
		sySetTickEvent(scGabombaStatueCountDownShowingClock, EVENT_PASS_TICK);
	else
		sySetTickEvent(scGabombaStatueCountDown, EVENT_PASS_TICK);
}

/* Stops the clock's countdown. */
void scGabombaStatueStopClock(void)
{
	CLOCK_WORK *clock = CLOCK_STATE;

	if(clock->mode)
		syFieldCloseEvent(scGabombaStatueCountDownShowingClock);
	else
		syFieldCloseEvent(scGabombaStatueCountDown);
}

/* Starts the clock counting down from a minute. */
void scGabombaStatueRestartClock(void)
{
	CLOCK_WORK *clock = CLOCK_STATE;

	if(clock->mode)
		sySetTickEvent(scGabombaStatueCountDownShowingClock, EVENT_PASS_TICK);
	else
		sySetTickEvent(scGabombaStatueCountDown, EVENT_PASS_TICK);
	clock->timer = TICKS_PER_SECOND * 60;
}

/* The ticks left on the clock. */
Sint32 scGabombaStatueGetClock(void)
{
	return CLOCK_STATE->timer;
}

/* Sets the clock back to a minute. */
void scGabombaStatueResetClock(void)
{
	CLOCK_STATE->timer = TICKS_PER_SECOND * 60;
}

/* Puts out the stakes of a table of rows (actor, kind, flag), ended by -1, each
   standing or driven down as its flag says. */
void scGabombaStatuePlaceSwitchActors(const Uint16 *entry)
{
	Sint32 value;

	for(; (value = *entry) != (Uint16)-1; entry += 2) {
		Uint16 id = *entry++;
		ACTOR *actor = fdActorGet(id);
		Sint16 kind = entry[0];
		Sint16 flag = entry[1];

		actor->collisionFlags &= ~ACTOR_SOLID;
		fdActorSetSpriteFlags(actor, 0);
		fdActorSetMetatileOpen(id, (Uint16)kind, (Uint16)flag, actor->layer);
	}
}

/* Finds the current target's table row and shrinks it. */
#include "shared/pound_sink_target.h"

#include "shared/actor_set_metatile_open.h"

/* clang-format off */
#include "shared/field_effect_duration_scripts.c"

/* clang-format on */

/* The shapes scGabombaStatueDrawCogMetatiles writes over the map's from
   COG_SHAPE_FIRST. */
Uint32 GabombaStatueCogShapes[] = {
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

/* Each cog's animation by the cogs' direction + 1: turning back, stopped and
   turning. */
Sint8 GabombaStatueCogAnimations[] = {
	3,
	1,
	2,
	0,
};

Uint32 GabombaStatuePinionReleaseScript[] = {
	SC_RELEASE,
};

/* clang-format off */
/* Unused: GabombaStatueTurnedPieces from its second byte to its 21st. */
LATE_DATA Uint8 GabombaStatueUnusedTurnedPieces[] = {
	PIECE_NONE, PIECE_EAST_WEST, PIECE_EAST_WEST, PIECE_NORTH_SOUTH,
	PIECE_NORTH_SOUTH, PIECE_CROSSING, PIECE_CROSSING, PIECE_NORTH_EAST,
	PIECE_SOUTH_WEST, PIECE_EAST_SOUTH, PIECE_WEST_NORTH, PIECE_SOUTH_WEST,
	PIECE_NORTH_EAST, PIECE_WEST_NORTH, PIECE_EAST_SOUTH,
	PIECE_SOUTH_WEST_AND_NORTH_EAST, PIECE_SOUTH_WEST_AND_NORTH_EAST,
	PIECE_EAST_SOUTH_AND_WEST_NORTH, PIECE_EAST_SOUTH_AND_WEST_NORTH,
	PIECE_STOP, PIECE_STOP,
};

/* The step along each heading as (column, row). */
LATE_DATA Sint8 GabombaStatuePathSteps[] = {
	1, 0, 0, 1, -1, 0, 0, -1,
};

/* The headings each piece joins, a bit each; a crossing, two curves and a */
/* stop let no new path in. */
LATE_DATA Uint8 GabombaStatuePieceJoins[] = {
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

/* The piece joining each heading a path enters a metatile by (a row of four) */
/* to each it leaves by. */
LATE_DATA Uint8 GabombaStatuePieceTable[] = {
	/* Heading east, in by the west side; out east, south, back west, north. */
	PIECE_EAST_WEST, PIECE_SOUTH_WEST, PIECE_STOP, PIECE_WEST_NORTH,
	/* Heading south, in by the north side. */
	PIECE_NORTH_EAST, PIECE_NORTH_SOUTH, PIECE_WEST_NORTH, PIECE_STOP,
	/* Heading west, in by the east side. */
	PIECE_STOP, PIECE_EAST_SOUTH, PIECE_EAST_WEST, PIECE_NORTH_EAST,
	/* Heading north, in by the south side. */
	PIECE_EAST_SOUTH, PIECE_STOP, PIECE_SOUTH_WEST, PIECE_NORTH_SOUTH,
};

LATE_DATA const Sint32 *FieldEffectDurationScripts[] = {
	FieldEffectShortScript,
	FieldEffectMediumScript,
	FieldEffectLongScript,
};

/* The way the cogs turn, which each tooth an actor rides steps by. */
LATE_DATA Sint32 GabombaStatueCogDirection = 0;

LATE_DATA Uint32 GabombaStatueLeaderFocusScript[] = {
	SC_ANIMATION(2),
	SC_MOVE(472, 0, 184),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_END,
};

LATE_DATA Uint32 GabombaStatueJennaFocusScript[] = {
	SC_ANIMATION(2),
	SC_MOVE(488, 0, 176),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_WAIT(1),
	SC_SET(SW_FACING, FACING_SOUTH),
	SC_WAIT(1),
	SC_END,
};

LATE_DATA Uint32 GabombaStatuePiersFocusScript[] = {
	SC_ANIMATION(2),
	SC_MOVE(456, 0, 176),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_WAIT(1),
	SC_SET(SW_FACING, FACING_SOUTH),
	SC_WAIT(1),
	SC_END,
};

LATE_DATA Uint32 GabombaStatueKradenFocusScript[] = {
	SC_ANIMATION(2),
	SC_MOVE(464, 0, 208),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_WAIT(1),
	SC_SET(SW_FACING, FACING_SOUTH_SOUTHWEST),
	SC_WAIT(1),
	SC_END,
};

LATE_DATA Uint32 GabombaStatueKradenPointsScript[] = {
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

LATE_DATA Uint32 GabombaStatueLookAtChannelScript[] = {
	SC_ANIMATION(2),
	SC_MOVE(348, 0, 312),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_POSITION_ORIGIN,
	SC_WAIT(1),
	SC_END,
};

LATE_DATA Sint32 GabombaStatueLookNorthAndEastScript[] = {
	SC_SET(SW_TIMER, FACING_NORTH),
	SC_SET(SW_USER_VALUE, FACING_NORTHEAST),
	SC_SET(SW_SCENE_BYTE, 0),
	SC_CALL(scGabombaStatueLookAround),
};

LATE_DATA Sint32 GabombaStatueLookNorthAndWestScript[] = {
	SC_SET(SW_TIMER, FACING_NORTH),
	SC_SET(SW_USER_VALUE, FACING_NORTHWEST),
	SC_SET(SW_SCENE_BYTE, 0),
	SC_CALL(scGabombaStatueLookAround),
};

LATE_DATA Uint32 GabombaStatueJennaWatchesScript[] = {
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

LATE_DATA Uint32 GabombaStatueAkafubuComesUpScript[] = {
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
LATE_DATA Sint16 GabombaStatueStakes[] = {
	ACTOR_FIRST_STAKE, STAKE_MOSSY_STONE, FLAG_GABOMBA_FIRST_STAKE_POUNDED, ACTOR_SECOND_STAKE, STAKE_MOSSY_STONE, FLAG_GABOMBA_SECOND_STAKE_POUNDED, -1, 0,
};

/* The random seeds a game's channel is laid from, one picked by the leader's */
/* name (scGabombaStatueGetPuzzleSeed). */
LATE_DATA Uint32 GabombaStatueChannelSeeds[] = {
	36, 122, 201, 312, 431, 504,
	619, 712, 823, 908, 1054, 1121,
	1207, 1311, 1401, 1555,
};

LATE_DATA SCENE_ENTRANCE SceneEntrances[] = {
	{0, CONDITION_ALWAYS, 392, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

LATE_DATA SCENE_REGION GabombaStatuePinionExitRegions[] = {
	{784, 16, 80, 800, 32, 96, CONDITION_ALWAYS, GABOMBA_STATUE_PINION_TO_KIBOMBO_STATUE_GROUNDS},
	{80, 48, 368, 96, 64, 384, CONDITION_ALWAYS, GABOMBA_STATUE_PINION_TO_GABOMBA_STATUE_BONE_ARMLET_CHEST},
	{SCENE_TABLE_END},
};

LATE_DATA SCENE_REGION GabombaStatueBoneArmletChestExitRegions[] = {
	{464, 16, 240, 480, 32, 256, CONDITION_ALWAYS, GABOMBA_STATUE_BONE_ARMLET_CHEST_TO_GABOMBA_STATUE_ELIXIR_CHEST},
	{SCENE_TABLE_END},
};

LATE_DATA SCENE_REGION GabombaStatueElixirChestExitRegions[] = {
	{528, 16, 64, 544, 32, 80, CONDITION_ALWAYS, GABOMBA_STATUE_ELIXIR_CHEST_TO_GABOMBA_STATUE_STEEL_NORTHEAST},
	{96, 16, 160, 112, 32, 176, CONDITION_ALWAYS, GABOMBA_STATUE_ELIXIR_CHEST_TO_GABOMBA_STATUE_STEEL_WEST},
	{672, 16, 160, 688, 32, 176, CONDITION_ALWAYS, GABOMBA_STATUE_ELIXIR_CHEST_TO_GABOMBA_STATUE_STEEL_EAST},
	{SCENE_TABLE_END},
};

LATE_DATA SCENE_REGION GabombaStatueSteelExitRegions[] = {
	{384, 16, 112, 400, 32, 128, CONDITION_ALWAYS, GABOMBA_STATUE_STEEL_TO_GABOMBA_STATUE_MOUSE_NORTHWEST},
	{96, 16, 144, 112, 32, 160, CONDITION_ALWAYS, GABOMBA_STATUE_STEEL_TO_GABOMBA_STATUE_MOUSE_WEST},
	{672, 16, 144, 688, 32, 160, CONDITION_ALWAYS, GABOMBA_STATUE_STEEL_TO_GABOMBA_STATUE_MOUSE_EAST},
	{SCENE_TABLE_END},
};

LATE_DATA SCENE_REGION GabombaStatueMouseExitRegions[] = {
	{432, 16, 448, 448, 32, 464, CONDITION_ALWAYS, GABOMBA_STATUE_MOUSE_TO_GABOMBA_STATUE_COGS},
	{SCENE_TABLE_END},
};

LATE_DATA SCENE_REGION GabombaStatueCogsExitRegions[] = {
	{304, 16, 144, 320, 32, 160, CONDITION_ALWAYS, GABOMBA_STATUE_COGS_TO_GABOMBA_STATUE_CHAMBER_IDOL},
	{SCENE_TABLE_END},
};

LATE_DATA Uint32 SceneExits[] = {
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

LATE_DATA Sint32 GabombaStatueFlashCogScript[] = {
	SC_CALL(scGabombaStatueFlashCog),
	SC_END,
};

LATE_DATA Sint32 GabombaStatueTurnToothScript[] = {
	SC_CALL(scGabombaStatueTurnTooth),
	SC_END,
};

LATE_DATA ACTOR_PLACE GabombaStatueBoneArmletChestActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 472, 0, 248, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

LATE_DATA ACTOR_PLACE GabombaStatueElixirChestActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 88, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, (Sint32)GabombaStatueTurnToothScript, 328, 0, 88, FACING_EAST, TALK_FACING_KEEP, 0),
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
	ACTOR_PLACEMENT(SPRITE_BLUE_COG, CONDITION_ALWAYS, (Sint32)GabombaStatueFlashCogScript, 360, 0, 488, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BROWN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 352, 0, 168, FACING_WEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BROWN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 400, 0, 168, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 680, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	{SCENE_TABLE_END},
};

LATE_DATA ACTOR_PLACE GabombaStatueSteelActorPlacements[] = {
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

LATE_DATA ACTOR_PLACE GabombaStatueMouseActorPlacements[] = {
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

LATE_DATA ACTOR_PLACE GabombaStatueCogsActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 232, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 232, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_RED_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_RED_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 296, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_BROWN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 416, 0, 216, FACING_WEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BROWN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 464, 0, 216, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	{SCENE_TABLE_END},
};

LATE_DATA Sint32 GabombaStatueChargeScript[] = {
	SC_CALL(scGabombaStatueCircleOwner),
	SC_END,
};

LATE_DATA ACTOR_PLACE GabombaStatueArrowsActorPlacements[] = {
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

LATE_DATA ACTOR_PLACE GabombaStatuePinionActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_BROWN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 288, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BROWN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 336, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_BROWN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 392, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_BROWN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 128, 0, 456, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_BROWN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 176, 0, 456, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	{SCENE_TABLE_END},
};

LATE_DATA ACTOR_PLACE GabombaStatuePlacements[] = {
	{SCENE_TABLE_END},
};

/* How the tooth the first rope is tied to rocks as the rope pulls taut, a */
/* step of its turn each tick. */
LATE_DATA Sint8 GabombaStatueRopeStopSteps[] = {
	1, 1, 1, 0, 0, 0, -1, -1,
	-1, 0, 0, 0, 1, 1, 0, 0,
	-1, -1, 0, 0, 1, 0, -1, 0,
	0, 0, 0, 0,
};

LATE_DATA CUE GabombaStatueBoneArmletChestCues[] = {
	{STEP_CUE_WALKING, METATILE_BEHAVIOR_FIRST_LADDER_DOWN, CONDITION_ALWAYS, SCRIPT_REF(scGabombaStatueClimbDownBoneArmletChestLadder)},
	{CUE_TABLE_END},
};

LATE_DATA CUE GabombaStatueElixirChestCues[] = {
	{STEP_CUE_WALKING, METATILE_BEHAVIOR_FIRST_LADDER_DOWN, CONDITION_ALWAYS, SCRIPT_REF(scGabombaStatueClimbDownElixirChestLadder)},
	{CUE_STEP, METATILE_BEHAVIOR_PINION_GAP, CONDITION_ALWAYS, SCRIPT_REF(GabombaStatueElixirPinionsCarryLeaderSouth)},
	{CUE_STEP, METATILE_BEHAVIOR_PINION_GAP_TURNED_BACK, CONDITION_ALWAYS, SCRIPT_REF(GabombaStatueElixirPinionsCarryLeaderNorth)},
	{PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_FIRST_ROPE_POST_A, FLAG_GABOMBA_FIRST_ROPE_TIED, SCRIPT_REF(scGabombaStatueLashRope)},
	{PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_SECOND_ROPE_POST_A, FLAG_GABOMBA_SECOND_ROPE_TIED, 0},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_FIRST_STAKE, FLAG_GABOMBA_FIRST_STAKE_POUNDED, SCRIPT_REF(scGabombaStatuePoundStake)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_SECOND_STAKE, FLAG_GABOMBA_SECOND_STAKE_POUNDED, SCRIPT_REF(scGabombaStatuePoundStake)},
	{CUE_RAISED, RAISED_COGS_TURN_BACK, FLAG_GABOMBA_COGS_TURNED_BACK, SCRIPT_REF(scGabombaStatueCogsTurnBack)},
	{CUE_TABLE_END},
};

LATE_DATA CUE GabombaStatueSteelCues[] = {
	{STEP_CUE_WALKING, METATILE_BEHAVIOR_FIRST_LADDER_DOWN, CONDITION_ALWAYS, SCRIPT_REF(scGabombaStatueClimbDownSteelNortheastLadder)},
	{STEP_CUE_WALKING, METATILE_BEHAVIOR_SECOND_LADDER_DOWN, CONDITION_ALWAYS, SCRIPT_REF(scGabombaStatueClimbDownSteelWestLadder)},
	{STEP_CUE_WALKING, METATILE_BEHAVIOR_THIRD_LADDER_DOWN, CONDITION_ALWAYS, SCRIPT_REF(scGabombaStatueClimbDownSteelEastLadder)},
	{CUE_STEP, METATILE_BEHAVIOR_GREEN_COG, CONDITION_ALWAYS, SCRIPT_REF(scGabombaStatueTurnWithCog)},
	{CUE_STEP, METATILE_BEHAVIOR_BLUE_COG, CONDITION_ALWAYS, SCRIPT_REF(scGabombaStatueRideBlueCog)},
	{CUE_STEP, METATILE_BEHAVIOR_RED_COG, CONDITION_ALWAYS, SCRIPT_REF(scGabombaStatueRideSlidingCog)},
	{CUE_STEP, METATILE_BEHAVIOR_ROLLER, CONDITION_ALWAYS, SCRIPT_REF(scGabombaStatueRideRoller)},
	{STEP_CUE_WALKING_INTO(FACING_EAST), METATILE_BEHAVIOR_COG_END, CONDITION_ALWAYS, SCRIPT_REF(scGabombaStatueHopAcrossEastOrWest)},
	{STEP_CUE_WALKING_INTO(FACING_WEST), METATILE_BEHAVIOR_COG_END, CONDITION_ALWAYS, SCRIPT_REF(scGabombaStatueHopAcrossEastOrWest)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_COG_END, CONDITION_ALWAYS, SCRIPT_REF(scGabombaStatueHopAcrossNorthOrSouth)},
	{STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_BEHAVIOR_COG_END, CONDITION_ALWAYS, SCRIPT_REF(scGabombaStatueHopAcrossNorthOrSouth)},
	{PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_STEEL_ROPE_POST_A, FLAG_GABOMBA_FIRST_ROPE_TIED, 0},
	{CUE_TALK, ACTOR_STEEL, IF_FLAG_SET(FLAG_LANDED_BESIDE_STEEL), SCRIPT_REF(scGabombaStatueFightSteel)},
	{CUE_TABLE_END},
};

LATE_DATA CUE GabombaStatueMouseCues[] = {
	{STEP_CUE_WALKING, METATILE_BEHAVIOR_FIRST_LADDER_DOWN, CONDITION_ALWAYS, SCRIPT_REF(scGabombaStatueClimbDownMouseNorthLadder)},
	{STEP_CUE_WALKING, METATILE_BEHAVIOR_SECOND_LADDER_DOWN, CONDITION_ALWAYS, SCRIPT_REF(scGabombaStatueClimbDownMouseWestLadder)},
	{STEP_CUE_WALKING, METATILE_BEHAVIOR_THIRD_LADDER_DOWN, CONDITION_ALWAYS, SCRIPT_REF(scGabombaStatueClimbDownMouseEastLadder)},
	{CUE_STEP, METATILE_BEHAVIOR_GREEN_COG, CONDITION_ALWAYS, SCRIPT_REF(scGabombaStatueTurnWithCog)},
	{CUE_STEP, METATILE_BEHAVIOR_BLUE_COG, CONDITION_ALWAYS, SCRIPT_REF(scGabombaStatueRideBlueCog)},
	{CUE_STEP, METATILE_BEHAVIOR_RED_COG, CONDITION_ALWAYS, SCRIPT_REF(scGabombaStatueRideSlidingCog)},
	{CUE_STEP, METATILE_BEHAVIOR_ROLLER, CONDITION_ALWAYS, SCRIPT_REF(scGabombaStatueRideRoller)},
	{STEP_CUE_WALKING_INTO(FACING_EAST), METATILE_BEHAVIOR_COG_END, CONDITION_ALWAYS, SCRIPT_REF(scGabombaStatueHopAcrossEastOrWest)},
	{STEP_CUE_WALKING_INTO(FACING_WEST), METATILE_BEHAVIOR_COG_END, CONDITION_ALWAYS, SCRIPT_REF(scGabombaStatueHopAcrossEastOrWest)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_COG_END, CONDITION_ALWAYS, SCRIPT_REF(scGabombaStatueHopAcrossNorthOrSouth)},
	{STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_BEHAVIOR_COG_END, CONDITION_ALWAYS, SCRIPT_REF(scGabombaStatueHopAcrossNorthOrSouth)},
	{CUE_STEP, METATILE_BEHAVIOR_MOUSE, FLAG_GABOMBA_MOUSE_SEEN, SCRIPT_REF(scGabombaStatueMouseShowsTheWay)},
	{CUE_TABLE_END},
};

LATE_DATA CUE GabombaStatueCogsCues[] = {
	{STEP_CUE_WALKING, METATILE_BEHAVIOR_FIRST_LADDER_DOWN, CONDITION_ALWAYS, SCRIPT_REF(scGabombaStatueClimbDownCogsLadder)},
	{CUE_STEP, METATILE_BEHAVIOR_GREEN_COG, CONDITION_ALWAYS, SCRIPT_REF(scGabombaStatueTurnWithCog)},
	{CUE_STEP, METATILE_BEHAVIOR_BLUE_COG, CONDITION_ALWAYS, SCRIPT_REF(scGabombaStatueRideBlueCog)},
	{CUE_STEP, METATILE_BEHAVIOR_RED_COG, CONDITION_ALWAYS, SCRIPT_REF(scGabombaStatueRideSlidingCog)},
	{CUE_STEP, METATILE_BEHAVIOR_ROLLER, CONDITION_ALWAYS, SCRIPT_REF(scGabombaStatueRideRoller)},
	{STEP_CUE_WALKING_INTO(FACING_EAST), METATILE_BEHAVIOR_COG_END, CONDITION_ALWAYS, SCRIPT_REF(scGabombaStatueHopAcrossEastOrWest)},
	{STEP_CUE_WALKING_INTO(FACING_WEST), METATILE_BEHAVIOR_COG_END, CONDITION_ALWAYS, SCRIPT_REF(scGabombaStatueHopAcrossEastOrWest)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_COG_END, CONDITION_ALWAYS, SCRIPT_REF(scGabombaStatueHopAcrossNorthOrSouth)},
	{STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_BEHAVIOR_COG_END, CONDITION_ALWAYS, SCRIPT_REF(scGabombaStatueHopAcrossNorthOrSouth)},
	{CUE_STEP, METATILE_BEHAVIOR_PINION_GAP, CONDITION_ALWAYS, SCRIPT_REF(GabombaStatueCogsPinionsCarryLeaderSouth)},
	{CUE_STEP, METATILE_BEHAVIOR_PINION_GAP_TURNED_BACK, CONDITION_ALWAYS, SCRIPT_REF(GabombaStatueCogsPinionsCarryLeaderNorth)},
	{PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_COGS_ROPE_POST_A, CONDITION_ALWAYS, 0},
	{CUE_TABLE_END},
};

LATE_DATA CUE GabombaStatueArrowsCues[] = {
	{STEP_CUE_WALKING, METATILE_BEHAVIOR_FIRST_LADDER_DOWN, CONDITION_ALWAYS, SCRIPT_REF(scGabombaStatueClimbDownArrowsLadder)},
	{CUE_STEP, METATILE_BEHAVIOR_AROUND_LEFT_ARROW, CONDITION_ALWAYS, SCRIPT_REF(scGabombaStatueReleaseLeftArrowPanel)},
	{CUE_STEP, METATILE_BEHAVIOR_LEFT_ARROW, CONDITION_ALWAYS, SCRIPT_REF(scGabombaStatuePressLeftArrowPanel)},
	{CUE_STEP, METATILE_BEHAVIOR_AROUND_RIGHT_ARROW, CONDITION_ALWAYS, SCRIPT_REF(scGabombaStatueReleaseRightArrowPanel)},
	{CUE_STEP, METATILE_BEHAVIOR_RIGHT_ARROW, CONDITION_ALWAYS, SCRIPT_REF(scGabombaStatuePressRightArrowPanel)},
	{CUE_RAISED, RAISED_OFFERING_HEARD, FLAG_OFFERING_HEARD, SCRIPT_REF(scGabombaStatueHearTheOffering)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_ARROWS_STAKE, FLAG_GABOMBA_RESPONDED, SCRIPT_REF(scGabombaStatuePoundArrowsStake)},
	{CUE_RAISED, RAISED_STAKE_STANDS_UP, CONDITION_ALWAYS, SCRIPT_REF(scGabombaStatueStakeStandsUp)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_BLOCK_A, CONDITION_ALWAYS, SCRIPT_REF(scGabombaStatuePoundBlock)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_BLOCK_B, CONDITION_ALWAYS, SCRIPT_REF(scGabombaStatuePoundBlock)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_BLOCK_C, CONDITION_ALWAYS, SCRIPT_REF(scGabombaStatuePoundBlock)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_BLOCK_D, CONDITION_ALWAYS, SCRIPT_REF(scGabombaStatuePoundBlock)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_BLOCK_E, CONDITION_ALWAYS, SCRIPT_REF(scGabombaStatuePoundBlock)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_BLOCK_F, CONDITION_ALWAYS, SCRIPT_REF(scGabombaStatuePoundBlock)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_BLOCK_G, CONDITION_ALWAYS, SCRIPT_REF(scGabombaStatuePoundBlock)},
	{CUE_TABLE_END},
};

LATE_DATA CUE GabombaStatuePinionCues[] = {
	{CUE_STEP, METATILE_BEHAVIOR_PINION_GAP, CONDITION_ALWAYS, SCRIPT_REF(GabombaStatuePinionsCarryLeaderNorth)},
	{CUE_STEP, METATILE_BEHAVIOR_PINION_GAP_TURNED_BACK, CONDITION_ALWAYS, SCRIPT_REF(scGabombaStatuePinionsThrowLeaderSouth)},
	{CUE_TABLE_END},
};

LATE_DATA CUE GabombaStatueCues[] = {
	{CUE_TABLE_END},
};

/* Each piece turned a quarter turn counterclockwise and clockwise, a pair a */
/* piece in CHANNEL_PIECE's order. */
LATE_DATA Uint8 GabombaStatueTurnedPieces[] = {
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

LATE_DATA Uint32 GabombaStatueChargeBurstScript[] = {
	SC_ANIMATION(1),
	SC_LABEL(1),
	SC_ADD(SW_SCALE_X, toFIXED(5) / 128),
	SC_ADD(SW_SCALE_Y, toFIXED(5) / 128),
	SC_WAIT(1),
	SC_LOOP(24, SC_TO(1)),
	SC_POSITION(0, 0, 0),
	SC_SET(SW_SCALE_X, toFIXED(1)),
	SC_SET(SW_SCALE_Y, toFIXED(1)),
	SC_SET(SW_SCENE_BYTE, 0),
	SC_WAIT(1),
	SC_END,
};

LATE_DATA Sint32 GabombaStatueChargeShrinkScript[] = {
	SC_ANIMATION(0),
	SC_SET(SW_SCALE_X, toFIXED(2)),
	SC_SET(SW_SCALE_Y, toFIXED(2)),
	SC_LABEL(1),
	SC_ADD(SW_SCALE_X, -toFIXED(5) / 128),
	SC_ADD(SW_SCALE_Y, -toFIXED(5) / 128),
	SC_WAIT(1),
	SC_LOOP(48, SC_TO(1)),
	SC_POSITION(0, 0, 0),
	SC_SET(SW_SCALE_X, toFIXED(1)),
	SC_SET(SW_SCALE_Y, toFIXED(1)),
	SC_SET(SW_SCENE_BYTE, 0),
	SC_WAIT(1),
	SC_END,
};
/* clang-format on */

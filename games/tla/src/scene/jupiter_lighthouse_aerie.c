/* @file */
/* Jupiter Lighthouse's aerie, the open top of the tower between its two */
/* archer statues, with the lid over the well guarded by two angels. A block */
/* Move pushes into the gap at the end of each archer's path lights the path */
/* (scJupiterLighthousePlacePathBlock), the archer shoots a golden trident at */
/* its angel (scJupiterLighthouseStartBeam), and the angel rises and tilts the
 */
/* lid (scJupiterLighthouseRaiseAngels); with both, the lid is lifted off */
/* the well (FLAG_JUPITER_LIGHTHOUSE_LID_LIFTED). */

/* This unit declares its own form of fdMapGridGetCellBehavior, below. */
#define OWN_MapGrid_GetCellBehavior

#include "scene/event_layer.h"
#include "field/field_sequence.h"
#include "game/game.h"
#include "system/field_effect.h"
#include "macros.h"
#include "field/event_inline.h"
#include "shared/jupiter_lighthouse_aerie.h"
#include "scene/scene.h"
#include "field/call.h"
#include "field/actor_script.h"
#include "scene/lighthouse_palettes.h"

/* The placed actors between the archers and the lid (ACTOR_RIGHT_ARCHER and */
/* ACTOR_LID, shared/jupiter_lighthouse_aerie.h). */
typedef enum JUPITER_LIGHTHOUSE_AERIE_ACTOR_ID {
	/* The blocks Move pushes into the gaps at the ends of the archers' paths.
	 */
	ACTOR_LEFT_PATH_BLOCK = ACTOR_RIGHT_ARCHER + 1,
	ACTOR_RIGHT_PATH_BLOCK,
	/* The chains hanging down the wall below the well (fdHangingVinePlace). */
	ACTOR_WEST_CHAIN,
	ACTOR_EAST_CHAIN,
	/* The six platforms floating over the shafts either side of the well, west
	 */
	/* to east. */
	ACTOR_FLOATING_PLATFORM_A,
	ACTOR_FLOATING_PLATFORM_B,
	ACTOR_FLOATING_PLATFORM_C,
	ACTOR_FLOATING_PLATFORM_D,
	ACTOR_FLOATING_PLATFORM_E,
	ACTOR_FLOATING_PLATFORM_F
} JUPITER_LIGHTHOUSE_AERIE_ACTOR_ID;

/* What scJupiterLighthouseRaiseAngels raises: one angel, or the second of */
/* the two and then the lid with both angels. */
typedef enum ANGEL_RISE {
	RISE_LEFT_ANGEL,
	RISE_RIGHT_ANGEL,
	RISE_LEFT_ANGEL_AND_LID,
	RISE_RIGHT_ANGEL_AND_LID
} ANGEL_RISE;

/* The archer an archer's beam comes from (scJupiterLighthouseStartBeam). */
typedef enum BEAM_SIDE { BEAM_LEFT, BEAM_RIGHT } BEAM_SIDE;

/* The path blocks' glows, pictures of scLightSprsInitB's set: the lit */
/* channel of the path piece each block carries, from east round to south on */
/* the left block and from west round to south on the right. */
enum { GLOW_PATH_EAST_SOUTH = 3, GLOW_PATH_WEST_SOUTH = 5 };

/* The cells of the gaps at the ends of the archers' paths, which a path block
 */
/* fills. */
#define LEFT_GAP_CELL_X	 6
#define RIGHT_GAP_CELL_X 57
#define GAP_CELL_Z		 12

/* A path block's cell heights, in 8-pixel steps: the ledge it stands on, and */
/* its top, 16 pixels higher, which keeps the leader from walking through it. */
#define LEDGE_HEIGHT		  (32 / 8)
#define PATH_BLOCK_TOP_HEIGHT (48 / 8)
/* The frames a path block set down in its gap is given to land. */
#define PATH_BLOCK_LANDING_FRAMES 30

/* A path block dropped into its gap: the gap's cells become the path's, in */
/* their pictures and as floor Felix can walk on. */
#define scJupiterLighthouseFillLeftGap()                                       \
	(fdMapGridCopyCellsLayered(66, 39, 1, 1, 70, 10),                          \
	 fdMapGridCopyCellsLayered(5, 75, 1, 1, 6, 75),                            \
	 fdMapGridCopyCellAttributes(5, 12, 1, 1, 6, 12))
#define scJupiterLighthouseFillRightGap()                                      \
	(fdMapGridCopyCellsLayered(68, 39, 1, 1, 121, 10),                         \
	 fdMapGridCopyCellsLayered(58, 75, 1, 1, 57, 75),                          \
	 fdMapGridCopyCellAttributes(58, 12, 1, 1, 57, 12))
/* The path to an archer lit, once its gap is filled. */
#define scJupiterLighthouseLightLeftPath()                                     \
	fdMapGridCopyCellsLayered(70, 39, 1, 1, 71, 10)
#define scJupiterLighthouseLightRightPath()                                    \
	fdMapGridCopyCellsLayered(72, 39, 1, 1, 120, 10)

extern s32 PaletteCycleFrame;

/* Sets the palette cycle's frame (PaletteCycleFrame). */
static inline void scPaletteCycleSetFrame(s32 value) {
	PaletteCycleFrame = value;
}
extern s32 PaletteCyclePausesInEvents;
s32 WhirlwindRiding BSS_DATA;
/* A copy of the room's cycling palette, BG palette CYCLE_PALETTE, which the */
/* cycle takes its colours from (scJupiterLighthouseLoadPalettes). */
u16 PaletteCycleColors[16] BSS_DATA;
/* Palette 13's first two colours; it is copied whole, with the unlit colours */
/* after them. */
u16 PaletteCycleUnlitPalette[2] BSS_DATA;
u16 PaletteCycleUnlitColors[14] BSS_DATA;
s16 LightSpriteTile BSS_DATA;
s16 LightSpriteCount BSS_DATA;
ACTOR *LightSpriteActors[16] BSS_DATA;
LIGHT_OAM LightSpriteOams[16] BSS_DATA;
s16 LightSpritesLarge BSS_DATA;
s16 LightSpritePriority BSS_DATA;
CELL_TARGET CrackedTiles BSS_DATA;

struct EFFECT_OPTIONS;
struct EFFECT_SPR;
struct LIGHT_OAM;

void scLightSprsSetPriority(s32 value);

void scJupiterLighthouseStartBeamFlicker(ACTOR *actor);
void scJupiterLighthouseStopBeamFlicker(ACTOR *actor);

void scJupiterLighthouseStartBeam(s32 archerId, s32 angelId, s32 side);
void scLightSprsInitB(void);
void scLightSprsAdd(s32 id, s32 value);

void scFieldEffectSpawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
						s32 velocityZ, u32 flags,
						const EFFECT_OPTIONS *options);

void scJupiterLighthouseUpdateBeam(void);

extern u32 FieldEffect_MediumScript[];
extern u32 FieldEffect_LongScript[];

struct CELL_TARGET;
struct CELL_TARGET_ENTRY;

void scJupiterLighthousePlaceRaisedActors(void);

void fdLeaderCenterInCell(void);

void scCrackedTileCrack(CELL_TARGET *state, CELL_TARGET_ENTRY *entry);

void scCrackedTileFall(CELL_TARGET *state, CELL_TARGET_ENTRY *entry);

void scCrackedTileDropIn(void);

struct BEAM_SPARK;
struct FIELD_EFFECT;

/* Draws an effect sprite (fdEffectSprDraw, through the overlay's import). */
void syImportEffectSprDraw(EFFECT_SPR *sprite);

const SCENE_ENTRANCE *scGetEntrances(void) { return SceneEntrances; }

const SCENE_REGION *scGetExitRegions(void) { return 0; }

const u32 *scGetExits(void) { return SceneExits; }

const ACTOR_PLACE *scGetActorPlacements(void) { return ActorPlacements; }

/* A shot angel rises and tilts the lid away from its side; the second */
/* of the two then lifts both angels and the lid off the well, to the puzzle's
 */
/* jingle. */
void scJupiterLighthouseRaiseAngels(s32 mode) {
	ACTOR *left = fdActorGet(ACTOR_LEFT_ANGEL);
	ACTOR *right = fdActorGet(ACTOR_RIGHT_ANGEL);
	ACTOR *lid = fdActorGet(ACTOR_LID);
	FIELD_SPR *sprite = lid->spr;
	s32 i;

	scFieldActorSetPosAndResetMotion(lid, lid->x.fixed, lid->y.fixed,
									 lid->z.fixed);
	if(mode == RISE_LEFT_ANGEL || mode == RISE_LEFT_ANGEL_AND_LID) {
		fdEventPlaySnd(SE_RISE_WITH_SPARKS_HIGH);
		for(i = 0; i < ANGEL_RISE_FRAMES; i++) {
			left->y.fixed += toFIXED(1) / 10;
			lid->y.fixed += toFIXED(3) / 100;
			lid->x.fixed -= toFIXED(1) / 25;
			sprite->rotation += TRIG_TURN / 32 / ANGEL_RISE_FRAMES;
			syWait(1);
		}
	}
	else {
		fdEventPlaySnd(SE_RISE_WITH_SPARKS_HIGH);
		for(i = 0; i < ANGEL_RISE_FRAMES; i++) {
			right->y.fixed += toFIXED(1) / 10;
			lid->y.fixed += toFIXED(3) / 100;
			lid->x.fixed += toFIXED(1) / 25;
			sprite->rotation -= TRIG_TURN / 32 / ANGEL_RISE_FRAMES;
			syWait(1);
		}
	}
	if(mode != RISE_LEFT_ANGEL_AND_LID && mode != RISE_RIGHT_ANGEL_AND_LID)
		return;
	syFieldEventWait(30);
	scJupiterLighthouseStartBeamFlicker(left);
	scJupiterLighthouseStartBeamFlicker(right);
	fdEventPlaySnd(SE_JUPITER_LIGHTHOUSE_BEAMS);
	for(i = 0; i < LID_LIFT_FRAMES; i++) {
		fdActorGet(ACTOR_LEFT_ANGEL)->y.fixed += toFIXED(1) / 10;
		fdActorGet(ACTOR_RIGHT_ANGEL)->y.fixed += toFIXED(1) / 10;
		lid->y.fixed += toFIXED(1) / 10;
		syWait(1);
	}
	fdEventPlaySnd(SOUND_FADE_OUT_ACTION_SE);
	syFieldEventWait(40);
	fdEventPlaySnd(MUS_PUZZLE_SOLVED);
	syFieldEventWait(10);
	mnSndWaitForJingle();
	left->motionFlg = ACTOR_FLOATS;
	right->motionFlg = ACTOR_FLOATS;
	lid->motionFlg = ACTOR_FLOATS;
}

/* The left archer shoots its beam at the left angel, which rises; the second */
/* angel of the two lifts the lid off. */
void scJupiterLighthouseShootAtLeftAngel(void) {
	fdEventSetFlag(FLAG_JUPITER_LIGHTHOUSE_LEFT_ANGEL_SHOT);
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_STAND);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_EAST, 0);
	scJupiterLighthouseStartBeam(ACTOR_LEFT_ARCHER, ACTOR_LEFT_ANGEL,
								 BEAM_LEFT);
	syFieldEventWait(40);
	if(fdEventGetFlag(FLAG_JUPITER_LIGHTHOUSE_RIGHT_ANGEL_SHOT) == 0) {
		scJupiterLighthouseRaiseAngels(RISE_LEFT_ANGEL);
		syFieldEventWait(40);
	}
	else {
		scJupiterLighthouseRaiseAngels(RISE_LEFT_ANGEL_AND_LID);
	}
	syEventCameraFollowActor(MainRoutineStateWork.leaderActorId, 1);
	syFieldEventWait(40);
	scJupiterLighthouseStopBeamFlicker(fdActorGet(ACTOR_LEFT_ANGEL));
	scJupiterLighthouseStopBeamFlicker(fdActorGet(ACTOR_RIGHT_ANGEL));
	scJupiterLighthouseStopBeamFlicker(fdActorGet(ACTOR_LEFT_ARCHER));
	scJupiterLighthouseStopBeamFlicker(fdActorGet(ACTOR_RIGHT_ARCHER));
	fdCameraWaitForArrival();
	fdEventEnd();
}

/* The right archer shoots its beam at the right angel, which rises; the second
 */
/* angel of the two lifts the lid off. */
void scJupiterLighthouseShootAtRightAngel(void) {
	fdEventSetFlag(FLAG_JUPITER_LIGHTHOUSE_RIGHT_ANGEL_SHOT);
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_STAND);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_WEST, 0);
	scJupiterLighthouseStartBeam(ACTOR_RIGHT_ARCHER, ACTOR_RIGHT_ANGEL,
								 BEAM_RIGHT);
	syFieldEventWait(40);
	if(fdEventGetFlag(FLAG_JUPITER_LIGHTHOUSE_LEFT_ANGEL_SHOT) == 0) {
		scJupiterLighthouseRaiseAngels(RISE_RIGHT_ANGEL);
		syFieldEventWait(40);
	}
	else {
		scJupiterLighthouseRaiseAngels(RISE_RIGHT_ANGEL_AND_LID);
	}
	syEventCameraFollowActor(MainRoutineStateWork.leaderActorId, 1);
	syFieldEventWait(40);
	scJupiterLighthouseStopBeamFlicker(fdActorGet(ACTOR_LEFT_ANGEL));
	scJupiterLighthouseStopBeamFlicker(fdActorGet(ACTOR_RIGHT_ANGEL));
	scJupiterLighthouseStopBeamFlicker(fdActorGet(ACTOR_LEFT_ARCHER));
	scJupiterLighthouseStopBeamFlicker(fdActorGet(ACTOR_RIGHT_ARCHER));
	fdCameraWaitForArrival();
	fdEventEnd();
}

/* Move (or a push) takes hold of the block on the ledge by an archer statue: */
/* the cell it leaves drops back to the ledge's height. */
void scJupiterLighthouseLiftPathBlock(s32 unused, s32 actorId) {
	ACTOR *actor = fdActorGet(actorId);

	fdMapGridSetCellHeight(actor->x.fixed, actor->z.fixed, 0, LEDGE_HEIGHT);
}

/* Move sets down the block on the ledge by the archer statue at the west or */
/* east end of the aerie. Pushed into the gap at the end of the archer's path */
/* (cell (6, 12) in the west, (57, 12) in the east), it drops in with a heavy */
/* thud, the path lights up, and the archer shoots at the angel on its side of
 */
/* the well, which rises and lifts its half of the lid; once both angels have */
/* risen the lid is off the well. Set down anywhere else, the block stands in */
/* Felix's way. */
void scJupiterLighthousePlacePathBlock(s32 unused, s32 id) {
	ACTOR *actor = fdActorGet(id);
	s32 i;

	if(id == ACTOR_LEFT_PATH_BLOCK) {
		if(FIELD_CELL(actor->x.fixed) == LEFT_GAP_CELL_X &&
		   FIELD_CELL(actor->z.fixed) == GAP_CELL_Z) {
			fdEventSetFlag(FLAG_JUPITER_LIGHTHOUSE_LEFT_ANGEL_SHOT);
			actor->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
			syWait(2);
			i = 0;
			if(actor->y.fixed > actor->groundHeight || actor->velY != 0) {
				do {
					syWait(1);
					i++;
					if(i > PATH_BLOCK_LANDING_FRAMES - 1)
						break;
				} while(actor->y.fixed > actor->groundHeight ||
						actor->velY != 0);
			}
			actor->motionFlg = 0;
			scFieldActorStop(MainRoutineStateWork.leaderActorId);
			fdActorSetPos(id, 0, 0);
			fdEventPlaySnd(SE_HEAVY_IMPACT);
			scJupiterLighthouseFillLeftGap();
			syFieldEventWait(30);
			fdEventPlaySnd(SE_MAP_TRANSITION);
			scJupiterLighthouseLightLeftPath();
			syFieldEventWait(20);
			scJupiterLighthouseShootAtLeftAngel();
		}
		else {
			fdMapGridSetCellHeight(actor->x.fixed, actor->z.fixed, 0,
								   PATH_BLOCK_TOP_HEIGHT);
		}
	}
	if(id == ACTOR_RIGHT_PATH_BLOCK) {
		if(FIELD_CELL(actor->x.fixed) == RIGHT_GAP_CELL_X &&
		   FIELD_CELL(actor->z.fixed) == GAP_CELL_Z) {
			fdEventSetFlag(FLAG_JUPITER_LIGHTHOUSE_RIGHT_ANGEL_SHOT);
			actor->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
			syWait(2);
			i = 0;
			if(actor->y.fixed > actor->groundHeight || actor->velY != 0) {
				do {
					syWait(1);
					i++;
					if(i > PATH_BLOCK_LANDING_FRAMES - 1)
						break;
				} while(actor->y.fixed > actor->groundHeight ||
						actor->velY != 0);
			}
			actor->motionFlg = 0;
			scFieldActorStop(MainRoutineStateWork.leaderActorId);
			fdActorSetPos(id, 0, 0);
			fdEventPlaySnd(SE_HEAVY_IMPACT);
			scJupiterLighthouseFillRightGap();
			syFieldEventWait(30);
			fdEventPlaySnd(SE_MAP_TRANSITION);
			scJupiterLighthouseLightRightPath();
			syFieldEventWait(20);
			scJupiterLighthouseShootAtRightAngel();
		}
		else {
			fdMapGridSetCellHeight(actor->x.fixed, actor->z.fixed, 0,
								   PATH_BLOCK_TOP_HEIGHT);
		}
	}
	if(fdEventGetFlag(FLAG_JUPITER_LIGHTHOUSE_LEFT_ANGEL_SHOT) &&
	   fdEventGetFlag(FLAG_JUPITER_LIGHTHOUSE_RIGHT_ANGEL_SHOT))
		fdEventSetFlag(FLAG_JUPITER_LIGHTHOUSE_LID_LIFTED);
}

const TRIGGER *scGetTriggers(void) { return SceneEvents; }

/* Sets up the aerie as the party enters: the window in, the palettes, the */
/* chains, the floating platforms the leader may step onto, and the angels and
 */
/* the lid as far as they have risen. Returns 0. */
s32 scInit(void) {
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
	if(MainRoutineStateWork.entrance ==
		   JUPITER_LIGHTHOUSE_AERIE_FROM_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_WEST ||
	   MainRoutineStateWork.entrance ==
		   JUPITER_LIGHTHOUSE_AERIE_FROM_JUPITER_LIGHTHOUSE_ROOM_PHAETONS_BLADE_CHEST)
		fdActorGet(MainRoutineStateWork.leaderActorId)->priFlg |=
			ACTOR_PRIORITY_IGNORES_RISES;
	scJupiterLighthouseLoadPalettes(LIGHTHOUSE_PALETTES_BY_FLAG);
	syImportHangingVinePlace(ACTOR_WEST_CHAIN, 1);
	syImportHangingVinePlace(ACTOR_EAST_CHAIN, 1);
	fdActorGet(MainRoutineStateWork.leaderActorId)->priFlg |=
		ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
	fdActorGet(ACTOR_FLOATING_PLATFORM_A)->priFlg |= ACTOR_PRIORITY_PLATFORM;
	fdActorGet(ACTOR_FLOATING_PLATFORM_B)->priFlg |= ACTOR_PRIORITY_PLATFORM;
	fdActorGet(ACTOR_FLOATING_PLATFORM_C)->priFlg |= ACTOR_PRIORITY_PLATFORM;
	fdActorGet(ACTOR_FLOATING_PLATFORM_D)->priFlg |= ACTOR_PRIORITY_PLATFORM;
	fdActorGet(ACTOR_FLOATING_PLATFORM_E)->priFlg |= ACTOR_PRIORITY_PLATFORM;
	fdActorGet(ACTOR_FLOATING_PLATFORM_F)->priFlg |= ACTOR_PRIORITY_PLATFORM;
	scJupiterLighthousePlaceRaisedActors();
	return 0;
}

/* Lays out the path blocks: gone into their gaps, their paths lit, for each */
/* angel already shot, and glowing where they stand otherwise. */
s32 scPrepareMap(void) {
	ACTOR *actor;

	scSetArrivalFlags(0);
	scLightSprsInitB();
	if(fdEventGetFlag(FLAG_JUPITER_LIGHTHOUSE_LEFT_ANGEL_SHOT)) {
		fdActorSetPos(ACTOR_LEFT_PATH_BLOCK, 0, 0);
		scJupiterLighthouseFillLeftGap();
		scJupiterLighthouseLightLeftPath();
	}
	else {
		scLightSprsAdd(ACTOR_LEFT_PATH_BLOCK, GLOW_PATH_EAST_SOUTH);
		fdActorGet(ACTOR_LEFT_PATH_BLOCK)->priFlg |=
			ACTOR_PRIORITY_IGNORES_RISES;
	}
	actor = fdActorGet(ACTOR_LEFT_PATH_BLOCK);
	actor->motionFlg = 0;
	fdMapGridSetCellHeight(actor->x.fixed, actor->z.fixed, 0,
						   PATH_BLOCK_TOP_HEIGHT);
	if(fdEventGetFlag(FLAG_JUPITER_LIGHTHOUSE_RIGHT_ANGEL_SHOT)) {
		fdActorSetPos(ACTOR_RIGHT_PATH_BLOCK, 0, 0);
		scJupiterLighthouseFillRightGap();
		scJupiterLighthouseLightRightPath();
	}
	else {
		scLightSprsAdd(ACTOR_RIGHT_PATH_BLOCK, GLOW_PATH_WEST_SOUTH);
		fdActorGet(ACTOR_RIGHT_PATH_BLOCK)->priFlg |=
			ACTOR_PRIORITY_IGNORES_RISES;
	}
	actor = fdActorGet(ACTOR_RIGHT_PATH_BLOCK);
	actor->motionFlg = 0;
	fdMapGridSetCellHeight(actor->x.fixed, actor->z.fixed, 0,
						   PATH_BLOCK_TOP_HEIGHT);
	fdActorGet(ACTOR_LEFT_ARCHER)->sceneCnt = 1;
	fdActorGet(ACTOR_RIGHT_ARCHER)->sceneCnt = 1;
	return 0;
}

#include "shared/scene_set_arrival_flags.c"

#include "shared/palette_cycle_set_pauses_in_events.h"

#include "shared/palette_cycle_step.h"

#include "shared/jupiter_lighthouse_load_palettes.h"

#include "shared/actor_raise_from_cell.h"

#include "shared/whirlwind_carry_leader.h"

#include "shared/position_flat_distance.h"

extern s32 WhirlwindEndScript[];

/* The ROM's callers here are compiled against this form, which returns an s32
 */
/* where the definition returns a u8. */
s32 fdMapGridGetCellBehavior(s32 x, s32 z, s32 layer);
#include "shared/whirlwind_update.h"

#include "shared/whirlwind_spawn.h"

#include "shared/actor_emit_smoke.h"

#include "shared/jupiter_lighthouse_place_raised_actors.h"

/* An actor's update: flickers its sprite between filled and its beam's */
/* colours, a step a frame. */
void scJupiterLighthouseFlickerBeam(ACTOR *actor) {
	u8 *fill = &DecodeFillByte;
	u8 *step = &actor->sceneByte;
	s32 phase = *step & 3;

	*fill = 2;
	switch(phase) {
	case 1:
		fdActorSetSprColMode(actor, SPRITE_COLOR_FILL);
		break;
	case 0:
	case 2:
		fdActorSetSprColMode(actor, SPRITE_COLOR_GBR);
		break;
	case 3:
		fdActorSetSprColMode(actor, SPRITE_COLOR_GBR);
		break;
	}
	(*step)++;
}

/* Starts an actor's flicker (scJupiterLighthouseFlickerBeam). */
void scJupiterLighthouseStartBeamFlicker(ACTOR *actor) {
	actor->sceneByte = 0;
	actor->update = (OBJECT_UPDATE)scJupiterLighthouseFlickerBeam;
}

/* Stops an actor's flicker and leaves it in its beam's colours. */
void scJupiterLighthouseStopBeamFlicker(ACTOR *actor) {
	actor->update = 0;
	fdActorSetSprColMode(actor, SPRITE_COLOR_GBR);
}

/* The beam's pictures, at a byte a pixel: four 8 by 8 sparkles, and the 32 by
 */
/* 16 golden trident. */
#define SPARKLE_BYTES (4 * 8 * 8)
#define TRIDENT_BYTES (32 * 16)

/* An archer's beam (scJupiterLighthouseStartBeam), in the menu's heap slot: */
/* the sparks' and the trident's pictures and sprites, the beam's path, and */
/* where its steps have reached. */
typedef struct BEAM_STATE {
	u8 tiles[TRIDENT_BYTES];
	EFFECT_SPR sprs[32];
	BEAM_SPARK sparks[32];
	EFFECT_SPR large;
	u8 unusedAfterLarge[28];
	s32 from[3];
	s32 to[3];
	s32 carried[3];
	s32 pointA[3];
	s32 pointB[3];
	s16 step;
	s16 frm;
	u16 largeTile;
	u16 smallTile;
	s8 duration;
	s8 loop;
	s8 still;
	s8 side;
	ACTOR *archer;
	ACTOR *angel;
} BEAM_STATE;

/* The value t of n steps of the way from from to to. */
#define LERP(from, to, t, n) ((from) + ((to) - (from)) * (t) / (n))

/* The steps of an archer's beam (BEAM_STATE.step). */
typedef enum BEAM_STEP {
	BEAM_GATHER, /* the sparks close in on the archer */
	BEAM_GROW,	 /* the trident grows there */
	BEAM_SPREAD, /* the sparks spread out round it */
	BEAM_CARRY,	 /* the trident flies to the angel, the camera with it */
	BEAM_SHRINK, /* it shrinks into the angel, which starts to flicker */
	BEAM_FADE,	 /* the last sparks die out */
	BEAM_DONE = 999
} BEAM_STEP;

/* The frames the trident takes to fly from the archer to the angel. */
#define BEAM_CARRY_FRAMES 96
/* The frames a spark lives, as it fades through its four pictures. */
#define BEAM_SPARK_FRAMES 16

/* A task: one frame of an archer's beam, step by step: its sparks gather, */
/* the golden trident grows and is carried to the angel, and shrinks there */
/* as the angel starts to flicker. */
void scJupiterLighthouseUpdateBeam(void) {
	BEAM_STATE *state = Ram_HeapSlots->menuState;
	ACTOR *carrier = FieldStateWork->camera;
	s32 t = state->frm;
	s32 position[3];
	FIXED a[3];
	FIXED b[3];
	FIXED aside;
	s32 scale;
	s32 *pos;
	BEAM_SPARK *spark;
	EFFECT_SPR *sprite;
	EFFECT_SPR *large;
	s32 i;

	switch(state->step) {
	case BEAM_GATHER:
		state->duration = 30;
		aside = state->side == BEAM_LEFT ? toFIXED(10) : -toFIXED(10);
		a[0] = state->from[0] - aside;
		a[1] = state->from[1] + toFIXED(24);
		a[2] = state->from[2];
		b[0] = state->from[0];
		b[1] = state->from[1];
		b[2] = state->from[2];
		state->pointA[0] = LERP(a[0], b[0], t, state->duration);
		state->pointA[1] = LERP(a[1], b[1], t, state->duration);
		state->pointA[2] = LERP(a[2], b[2], t, state->duration);
		a[1] = state->from[1] - toFIXED(24);
		state->pointB[0] = LERP(a[0], b[0], t, state->duration);
		state->pointB[1] = LERP(a[1], b[1], t, state->duration);
		state->pointB[2] = LERP(a[2], b[2], t, state->duration);
		if(state->frm == state->duration) {
			state->step++;
			state->frm = -1;
		}
		break;
	case BEAM_GROW:
		state->duration = 60;
		aside = state->side == BEAM_LEFT ? toFIXED(30) : -toFIXED(30);
		a[0] = state->from[0];
		a[1] = state->from[1];
		a[2] = state->from[2];
		b[0] = state->from[0] - aside;
		b[1] = state->from[1];
		b[2] = state->from[2];
		state->pointA[0] = LERP(a[0], b[0], t, state->duration);
		state->pointA[1] = LERP(a[1], b[1], t, state->duration);
		state->pointA[2] = LERP(a[2], b[2], t, state->duration);
		state->pointB[0] = state->pointA[0];
		state->pointB[1] = state->pointA[1];
		state->pointB[2] = state->pointA[2];
		large = &state->large;
		large->scaleY = large->scaleX = FIXED_ONE * state->frm / 60;
		if(state->frm == state->duration) {
			state->step++;
			state->frm = -1;
		}
		break;
	case BEAM_SPREAD:
		if(state->frm == 0)
			state->still = 1;
		aside = state->side == BEAM_LEFT ? toFIXED(1) / 2 : -toFIXED(1) / 2;
		state->pointA[0] += aside;
		state->pointB[0] += aside;
		if(state->frm == 14) {
			state->step++;
			state->frm = -1;
		}
		break;
	case BEAM_CARRY:
		if(t == 0)
			sySndPlay(SE_PSYNERGY_SPARKLE);
		state->carried[0] = ((state->from[0] >> 2) +
							 ((state->to[0] >> 2) - (state->from[0] >> 2)) * t /
								 BEAM_CARRY_FRAMES)
							<< 2;
		state->carried[1] =
			LERP(state->from[1], state->to[1], t, BEAM_CARRY_FRAMES);
		state->carried[2] =
			LERP(state->from[2], state->to[2], t, BEAM_CARRY_FRAMES);
		fdActorSetPosAndResetMotion(carrier, state->carried[0],
									state->carried[1], state->carried[2]);
		aside = state->side == BEAM_LEFT ? toFIXED(16) : -toFIXED(16);
		state->pointA[0] = state->carried[0] - aside;
		state->pointA[1] = state->carried[1];
		state->pointA[2] = state->carried[2];
		state->pointB[0] = state->pointA[0];
		state->pointB[1] = state->pointA[1];
		state->pointB[2] = state->pointA[2];
		if(state->frm == BEAM_CARRY_FRAMES) {
			state->step++;
			state->frm = -1;
		}
		break;
	case BEAM_SHRINK:
		if(state->frm == 0) {
			sySndPlay(SE_SHIMMER);
			scJupiterLighthouseStartBeamFlicker(state->angel);
		}
		large = &state->large;
		large->scaleY = large->scaleX = large->scaleX - toFIXED(1) / 32;
		if(large->scaleX < toFIXED(1) / 5) {
			large->scaleX = 0;
			state->step++;
			state->frm = -1;
		}
		break;
	case BEAM_FADE:
		if(state->frm == 0)
			state->loop = 0;
		if(state->frm == BEAM_SPARK_FRAMES)
			state->step = BEAM_DONE;
		break;
	}
	large = &state->large;
	scale = large->scaleX;
	pos = position;
	if(state->side == BEAM_LEFT)
		large->scaleX = scale < 0 ? -scale : scale;
	else
		large->scaleX = -(scale < 0 ? -scale : scale);
	pos[0] = state->carried[0];
	pos[1] = state->carried[1];
	pos[2] = state->carried[2];
	fdCameraWorldToScr(pos);
	large->x = pos[0];
	large->y = pos[2];
	syImportEffectSprDraw(large);
	for(i = 0, spark = state->sparks, sprite = state->sprs; i <= 7;
		i++, sprite++, spark++) {
		if(spark->age == 0) {
			if(i & 1) {
				spark->pos[0] = state->pointA[0];
				spark->pos[1] = state->pointA[1];
				spark->pos[2] = state->pointA[2];
			}
			else {
				spark->pos[0] = state->pointB[0];
				spark->pos[1] = state->pointB[1];
				spark->pos[2] = state->pointB[2];
			}
			if(state->still == 0) {
				spark->pos[1] += sySin(syRandom()) << 3;
				spark->drift = toFIXED(3) / 10;
			}
			else {
				spark->pos[1] += sySin(syRandom()) << 1;
				spark->pos[0] += syCos(syRandom()) * 10;
				spark->drift = (syRandom() - 32768) << 1;
			}
		}
		if(spark->age < BEAM_SPARK_FRAMES) {
			sprite->oam.idx = state->smallTile + (spark->age & 3) * 2;
			fdEffectSprDrawAtWorldPos(sprite, spark);
			spark->pos[1] -= spark->drift;
		}
		spark->age++;
		if(state->loop != 0 && spark->age == BEAM_SPARK_FRAMES)
			spark->age = 0;
	}
	state->frm++;
}
#undef LERP

/* An archer shoots its beam at an angel: loads the sparks and the trident */
/* into VRAM and holds the event until the beam's task has finished. */
void scJupiterLighthouseStartBeam(s32 archerId, s32 angelId, s32 side) {
	BEAM_STATE *state = syHeapSlotAlloc(HEAP_SLOT_MENU, sizeof(BEAM_STATE));
	ACTOR *carrier = FieldStateWork->camera;
	EFFECT_SPR *sprite;
	BEAM_SPARK *spark;
	s32 smallEntry;
	s32 largeEntry;
	s32 smallTile;
	s32 largeTile;
	s32 i;
	s32 age;

	state->archer = fdActorGet(archerId);
	state->angel = fdActorGet(angelId);
	syDecompressTaggedLz(syResourceGetTableEntry(RESOURCE_STAR_SPARKLE_FADE),
						 state);
	smallEntry = syVramCacheFindFreeEntry();
	smallTile = mnVramLoad(smallEntry, SPARKLE_BYTES, state);
	state->smallTile = smallTile;
	spark = state->sparks;
	age = 0;
	for(i = 0; i < 32; age -= 2, i++, spark++) {
		sprite = &state->sprs[i];
		fdEffectSprInit(sprite, 4, 4, OBJ_SIZE_8X8, smallTile);
		sprite->oam.fullCol = 1;
		sprite->oam.pal = 0;
		sprite->oam.pri = 0;
		sprite->depth = OAM_DEPTH_WINDOW;
		spark->age = age;
	}
	syDecompressTaggedLz(syResourceGetTableEntry(RESOURCE_GOLDEN_TRIDENT),
						 state);
	largeEntry = syVramCacheFindFreeEntry();
	largeTile = mnVramLoad(largeEntry, TRIDENT_BYTES, state);
	state->largeTile = largeTile;
	sprite = &state->large;
	syFieldEffectSprInit(sprite, 30, 7, OBJ_SIZE_32X16, largeTile);
	sprite->depth = OAM_DEPTH_WINDOW;
	sprite->oam.pri = 1;
	sprite->oam.fullCol = 1;
	sprite->oam.pal = 0;
	sprite->scaleX = 0;
	sprite->scaleY = 0;
	if(side == BEAM_LEFT) {
		state->from[0] = toFIXED(152);
		state->from[1] = toFIXED(32);
		state->from[2] = toFIXED(164);
		state->to[0] = toFIXED(470);
		state->to[1] = toFIXED(32);
		state->to[2] = toFIXED(164);
	}
	else {
		state->from[0] = toFIXED(872);
		state->from[1] = toFIXED(32);
		state->from[2] = toFIXED(164);
		state->to[0] = toFIXED(550);
		state->to[1] = toFIXED(32);
		state->to[2] = toFIXED(164);
	}
	state->carried[0] = state->from[0];
	state->carried[1] = state->from[1];
	state->carried[2] = state->from[2];
	carrier->speedLimit = toFIXED(1) / 2;
	carrier->acceleration = toFIXED(1) / 2;
	fdActorStartIdle(carrier);
	scFieldActorSetMoveTargetFX(carrier, state->carried[0], state->carried[1],
								state->carried[2]);
	fdEventPlaySnd(SE_SWIRL);
	scJupiterLighthouseStartBeamFlicker(state->archer);
	syFieldEventWait(40);
	state->side = side;
	state->step = BEAM_GATHER;
	state->frm = 0;
	state->loop = 1;
	state->still = 0;
	syTaskAddFrame(scJupiterLighthouseUpdateBeam, TASK_PASS_FRAME);
	while(state->step != BEAM_DONE)
		syWait(1);
	syFieldTaskRemove(scJupiterLighthouseUpdateBeam);
	syVramCacheFreeEntry(largeEntry);
	syVramCacheFreeEntry(smallEntry);
	syHeapSlotRelease(HEAP_SLOT_MENU);
}

#include "shared/light_sprites_draw.h"

#include "shared/light_sprites_init_a.h"

#include "shared/light_sprites_init_b.h"

#include "shared/light_sprites_init_c.h"

#include "shared/light_sprites_add.h"

/* Sets the priority the lit actors' sprites are drawn at. */
void scLightSprsSetPriority(s32 value) { LightSpritePriority = value; }

#include "../field/actor_set_sprite_priority.c"

#include "../field/field_effect_move.c"

extern const s32 *FieldEffect_DurationScripts[];

#define EFFECT_SET_MODE	  fdActorSelectAnimation
#define EFFECT_SET_SCRIPT fdActorStartScript
#include "../field/field_effect_spawn.c"

#define CELL_PUSH_SLOT_ENTRY syOamAddSprAtDepthClamped
#include "../field/cracked_tile_mask.c"

#include "shared/cracked_tile_find.h"

#include "shared/leader_center_in_cell.h"

#include "shared/cracked_tile_crack.h"

#include "shared/cracked_tile_fall.h"

#include "../field/field_effect_advance_motion.c"

#undef CELL_PUSH_SLOT_ENTRY

#include "shared/cracked_tile_drop_in.h"

#include "shared/cracked_tile_step.h"

/* Unused here: on landing below a cracked tile, puts the leader on the cell */
/* the flag bytes saved, offset by (dx, dz), and starts the screen transition,
 */
/* as the rooms with cracked floors do (scCrackedTilePlaceLanding); the aerie */
/* has none to fall through. */
UNUSED void scCrackedTilePlaceLanding(s32 dx, s32 dz, s32 transition) {
	ACTOR *actor = fdActorGet(MainRoutineStateWork.leaderActorId);
	FIELD_STATE *field = Ram_HeapSlots->fieldState;
	CELL_TARGET *state = &CrackedTiles;
	s32 x = scFieldGameFlagGetByte(state->flgByte);
	s32 z = scFieldGameFlagGetByte(state->flgByte + 8);

	if(x != 0 || z != 0) {
		ACTOR *camera;

		syGameFlagSetByte(state->flgByte, 0);
		syGameFlagSetByte(state->flgByte + 8, 0);
		actor->x.fixed = FIELD_CELL_CENTER(x + dx);
		actor->z.fixed = FIELD_CELL_CENTER(z + dz);
		camera = field->camera;
		camera->x.fixed = actor->x.fixed;
		camera->z.fixed = actor->z.fixed;
		fdMapGridCenterOnFollowedPoint();
		scCrackedTileDropIn();
		FieldStateWork->transition = transition;
	}
}

extern u32 FieldEffect_ShortScript[];

/* clang-format off */
#include "shared/field_effect_duration_scripts.c"

SCENE_ENTRANCE SceneEntrances[] = {
    {0, CONDITION_ALWAYS, 560, 96, 264, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 SceneExits[] = {
    SCENE_EXITS(JupiterLighthouseAerie),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_AERIE_TO_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_SOUTHEAST, JupiterLighthouseRoomCrackedFloor, JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_FROM_JUPITER_LIGHTHOUSE_AERIE_SOUTHEAST),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_AERIE_TO_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_EAST, JupiterLighthouseRoomCrackedFloor, JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_FROM_JUPITER_LIGHTHOUSE_AERIE_EAST),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_AERIE_TO_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_WEST, JupiterLighthouseRoomCrackedFloor, JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_FROM_JUPITER_LIGHTHOUSE_AERIE_WEST),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_AERIE_TO_JUPITER_LIGHTHOUSE_ROOM_PHAETONS_BLADE_CHEST, JupiterLighthouseRoomPhaetonsBladeChest, JUPITER_LIGHTHOUSE_ROOM_PHAETONS_BLADE_CHEST_FROM_JUPITER_LIGHTHOUSE_AERIE),
    SCENE_EXITS_END,
};

ACTOR_PLACE ActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_ANGEL_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 472, 0, 256, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ANGEL_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 552, 0, 256, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ARCHER_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 200, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ARCHER_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 888, 0, 200, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 904, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SEEDLING, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SEEDLING, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 568, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 312, 16, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 344, 16, 312, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 376, 16, 312, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 648, 16, 312, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 680, 16, 312, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 712, 16, 312, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_BASIN, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_SPRITE_FLAGS, 512, 0, 248, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_BASIN, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_SPRITE_FLAGS, 512, 0, 248, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {SCENE_TABLE_END},
};

TRIGGER SceneEvents[] = {
    {TRIGGER_EXIT, JUPITER_LIGHTHOUSE_AERIE_TO_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_SOUTHEAST, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_AERIE_TO_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_SOUTHEAST},
    {TRIGGER_EXIT, JUPITER_LIGHTHOUSE_AERIE_TO_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_EAST, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_AERIE_TO_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_EAST},
    {TRIGGER_EXIT, JUPITER_LIGHTHOUSE_AERIE_TO_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_WEST, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_AERIE_TO_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_WEST},
    {TRIGGER_EXIT, JUPITER_LIGHTHOUSE_AERIE_TO_JUPITER_LIGHTHOUSE_ROOM_PHAETONS_BLADE_CHEST, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_AERIE_TO_JUPITER_LIGHTHOUSE_ROOM_PHAETONS_BLADE_CHEST},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_LEFT_PATH_BLOCK, CONDITION_ALWAYS, EVENT(scJupiterLighthouseLiftPathBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LEFT_PATH_BLOCK, CONDITION_ALWAYS, EVENT(scJupiterLighthousePlacePathBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_RIGHT_PATH_BLOCK, CONDITION_ALWAYS, EVENT(scJupiterLighthouseLiftPathBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_RIGHT_PATH_BLOCK, CONDITION_ALWAYS, EVENT(scJupiterLighthousePlacePathBlock)},
    {TRIGGER_PUSH, -1, CONDITION_ALWAYS, EVENT(scJupiterLighthouseLiftPathBlock)},
    {TRIGGER_PUSHED, -1, CONDITION_ALWAYS, EVENT(scJupiterLighthousePlacePathBlock)},
    {TRIGGER_TABLE_END},
};

/* The palette cycle's frame, -1 before it starts, and whether events hold it */
/* (fdPaletteCycleStep). */
s32 PaletteCycleFrame = -1;
s32 PaletteCyclePausesInEvents = 1;

s32 WhirlwindEndScript[] = {
    SC_ANIMATION(5),
    SC_WAIT(12),
    SC_RELEASE,
};
/* clang-format on */

const s32 *FieldEffect_DurationScripts[] = {
	FieldEffect_ShortScript,
	FieldEffect_MediumScript,
	FieldEffect_LongScript,
};

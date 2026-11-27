/* Aqua Rock's outside, the island south of the Apojii Islands, in two rooms of
   this overlay. On the slopes, red moai pour streams that stop now and then, a
   pillar Moved before one stops it for good, Douse fills a dry moai and turns a
   Water Stone's streams back uphill, and the guardian moai step aside once the
   Apojii's Aqua Stone is Doused. In the Mist Sabre chest's room, Frost freezes
   puddles into steps, Move drops two rocks into the stream, and Douse on its
   Water Stone opens the way inside (aqua_rock_inside.c). */

#include "../field/actor_set_sprite_priority.c"
#include "field/field_mode.h"
#include "field/actor_script.h"
#include "scene/dust.h"

#include "../field/actor_create_silhouette.c"

#include "graphics/palette_buffers.h"

/* The bubbles a stream carries along at once. */
#define STREAM_BUBBLE_COUNT 64

/* A bubble a stream carries: its place, its age in ticks and its sprite's
   attributes. */
typedef struct BUBBLE {
	Sint32			x;
	Sint32			y;
	Sint32			z;
	volatile Sint32 age;
	Uint32			oam[3];
} BUBBLE;

/* The bubbles' state: the actor they come from, their VRAM entry, the next free
   bubble, which way they go (up the stream when positive) and the bubbles. */
typedef struct AQUA_ROCK_BUBBLES {
	ACTOR *anchor;
	Uint16 vramEntry;
	Uint16 next;
	Sint16 dir;
	Uint8  unusedAfterDir[2]; /* no code names it */
	BUBBLE bubbles[STREAM_BUBBLE_COUNT];
} AQUA_ROCK_BUBBLES;

Sint32 scActorEmitMist(ACTOR *actor);

void scAquaRockTurnFish(ACTOR *actor);
void scAquaRockFlowSparkleUpStream(ACTOR *actor);
void scAquaRockDrawRisingBubbles(void);
void scAquaRockDrawFallingBubbles(void);
void scAquaRockDrawBubbles(void);

extern Uint32 WadingSplashScript[];
extern Uint32 RippleScript[];
/* The bubbles' four ticks, compressed (graphics/aqua_rock/tiles.s). */
extern const Uint8	AquaRock_BubbleTiles[];
extern SCENE_REGION AquaRockSlopesRegions[];
extern SCENE_REGION AquaRockMistSabreChestRegions[];
extern ACTOR_PLACE	AquaRockPlacements[];
extern ACTOR_PLACE	AquaRockSlopesPlacements[];
extern ACTOR_PLACE	AquaRockMistSabreChestPlacements[];

/* Aqua Rock's lasting flags outside: the slopes' second pillar Moved into the
   hole in its ledge, the two rocks Moved into the Mist Sabre chest's room's
   stream, the guardian moai stepped aside, the streams the slopes' Water Stone
   has turned back (each Douse turns them again), the Mist Sabre chest's room's
   Water Stone Doused, and the dry moai filled. */
#define FLAG_AQUA_ROCK_PILLAR_IN_HOLE					   0x950
#define FLAG_AQUA_ROCK_FIRST_ROCK_DROPPED				   0x951
#define FLAG_AQUA_ROCK_SECOND_ROCK_DROPPED				   0x952
#define FLAG_AQUA_ROCK_MOAI_STEPPED_ASIDE				   0x955
#define FLAG_AQUA_ROCK_STREAMS_TURNED_BACK				   0xa68
#define FLAG_AQUA_ROCK_MIST_SABRE_CHEST_WATER_STONE_DOUSED 0xa69
#define FLAG_AQUA_ROCK_DRY_MOAI_FILLED					   0xa6b
/* While the party is on the island: the metatile of the pillar Move sets before
   the top's red moai, kept as two bytes, and whether it stands before the west
   or the east one, which stops that moai's stream. */
#define FLAG_PILLAR_COLUMN								   FLAG_AREA_TEMP_10
#define FLAG_PILLAR_ROW									   FLAG_AREA_TEMP_18
#define FLAG_PILLAR_BEFORE_WEST_MOAI					   FLAG_AREA_TEMP_20
#define FLAG_PILLAR_BEFORE_EAST_MOAI					   FLAG_AREA_TEMP_21
/* This visit's flags. Each of the four red moai's streams, stopped
   (scAquaRockTimeStreams). */
#define FLAG_WEST_MOAI_STREAM_STOPPED					   FLAG_SCENE_TEMP_00
#define FLAG_EAST_MOAI_STREAM_STOPPED					   FLAG_SCENE_TEMP_01
#define FLAG_LOWER_WEST_MOAI_STREAM_STOPPED				   FLAG_SCENE_TEMP_02
#define FLAG_LOWER_EAST_MOAI_STREAM_STOPPED				   FLAG_SCENE_TEMP_03
/* Frost on each of the Mist Sabre chest's room's three puddles. */
#define FLAG_PUDDLE_A_FROZEN							   FLAG_SCENE_TEMP_21
#define FLAG_PUDDLE_B_FROZEN							   FLAG_SCENE_TEMP_22
#define FLAG_PUDDLE_C_FROZEN							   FLAG_SCENE_TEMP_23

/* The metatiles before the top's west and east red moai, where the pillar stops
   a stream; the column of the hole in the ledge; and the column of the Mist
   Sabre chest's room's stream, where a rock Moved drops in. */
#define WEST_MOAI_COLUMN   7
#define EAST_MOAI_COLUMN   15
#define MOAI_FRONT_ROW	   35
#define PILLAR_HOLE_COLUMN 32
#define ROCK_STREAM_COLUMN 53

/* The cue StepIntoCurrent raises for a leader the current carries over the edge
   of the falls (scAquaRockDropDownFalls). */
#define CUE_DROP_DOWN_FALLS 44

/* The slopes' map animations of the streams their Water Stone turns, two each:
   running downhill, and turned back to run uphill. */
enum { STREAMS_DOWNHILL_ANIMATION = 5, STREAMS_UPHILL_ANIMATION = 7 };

/* The metatile behaviours the rooms' cue tables answer: the streams the top's
   west and east red moai pour, the heads of the two lower red moai's streams,
   the streams that only ever run downhill, the streams the slopes' Water Stone
   turns back, and the mouth of the sparkling passage. */
enum {
	METATILE_BEHAVIOR_WEST_MOAI_STREAM = 30,
	METATILE_BEHAVIOR_EAST_MOAI_STREAM = 31,
	METATILE_BEHAVIOR_LOWER_WEST_MOAI_STREAM = 32,
	METATILE_BEHAVIOR_LOWER_EAST_MOAI_STREAM = 33,
	METATILE_BEHAVIOR_DOWNHILL_STREAM = 40,
	METATILE_BEHAVIOR_TURNING_STREAM = 41,
	METATILE_BEHAVIOR_SPARKLING_PASSAGE = 4
};

/* The slopes (AquaRockSlopesPlacements). */
typedef enum AQUA_ROCK_SLOPES_ACTOR_ID {
	/* The slopes' sixteen moai: four red ones, the first two at the top, then
	   the dry one, two green ones and nine green ones lower down that share one
	   sprite. */
	ACTOR_FIRST_MOAI = ACTOR_FIRST_PLACED,
	/* The grey moai Douse fills, so that it pours a stream
	   (scAquaRockDouseDryMoai). */
	ACTOR_DRY_MOAI = ACTOR_FIRST_MOAI + 4,
	ACTOR_FIRST_BOTTOM_MOAI = ACTOR_FIRST_MOAI + 7,
	/* The five guardian moai that stand in the way in, until they step aside
	   together (scAquaRockMoaiStepAside). */
	ACTOR_FIRST_GUARDIAN_MOAI = ACTOR_FIRST_MOAI + 9,
	ACTOR_LAST_MOAI = ACTOR_FIRST_MOAI + 15,
	/* The pillar Move sets before the top's red moai, and the pillar on the
	   ledge Move pushes into its hole. */
	ACTOR_MOAI_PILLAR,
	ACTOR_LEDGE_PILLAR,
	/* The Water Stone whose Douse turns the streams back uphill. */
	ACTOR_WATER_STONE,
	/* The three fish of the pools (AquaRockFishScript and
	   AquaRockSlowFishScript). */
	ACTOR_FIRST_FISH
} AQUA_ROCK_SLOPES_ACTOR_ID;

/* The Mist Sabre chest's room (AquaRockMistSabreChestPlacements). */
typedef enum AQUA_ROCK_MIST_SABRE_CHEST_ACTOR_ID {
	/* The Water Stone whose Douse opens the way inside. */
	ACTOR_MIST_SABRE_WATER_STONE = ACTOR_FIRST_PLACED,
	/* The three puddles Frost freezes into steps. */
	ACTOR_PUDDLE_A,
	ACTOR_PUDDLE_B,
	ACTOR_PUDDLE_C,
	/* The two rocks Move pushes into the stream. */
	ACTOR_FIRST_ROCK,
	ACTOR_SECOND_ROCK,
	/* An unseen marker the bubbles and sparkles rise from once the streams are
	   turned back. */
	ACTOR_BUBBLE_MARKER,
	/* An unseen marker in the pool where, once its Water Stone is Doused, rings
	   of water rise and the leader sinks inside (scAquaRockSinkInside). */
	ACTOR_WAY_INSIDE
} AQUA_ROCK_MIST_SABRE_CHEST_ACTOR_ID;

/* The Mist Sabre chest's room's ways between its west and east halves, which
   its one map holds, each named by the half it leaves; a way's exit and the
   entrance at its foot share its number. */
typedef enum AQUA_ROCK_MIST_SABRE_CHEST_WAY {
	WAY_MIST_SABRE_CHEST_WEST_TO_EAST = 4,
	WAY_MIST_SABRE_CHEST_EAST_TO_WEST
} AQUA_ROCK_MIST_SABRE_CHEST_WAY;

extern CUE AquaRockCues[];
extern CUE AquaRockSlopesCues[];
extern CUE AquaRockMistSabreChestCues[];

#include "game/game.h"
#include "scene/scene.h"
#include "scene/ground_effects.h"
#include "field/script_inline.h"
#include "field/call.h"
#include "macros.h"

/* Unused: creates an unseen, still actor of `kind` at a point, untargeted and
   drawn underfoot. */
UNUSED ACTOR *scAquaRockCreateActor(Sint32 x, Sint32 y, Sint32 z, Sint32 kind)
{
	ACTOR *actor = syImportActorCreate(kind, x, y, z);

	if(actor != 0) {
		fdSetActorSprPriority(actor, 1);
		actor->motionFlg = 0;
		actor->collisionFlg = ACTOR_NOT_TARGETED;
		fdActorSetSprFlags(actor, 0);
		fdActorSetSprCol(actor, SPRITE_COLOR_HIDDEN);
		actor->priFlg |=
			ACTOR_PRIORITY_UNDERFOOT | ACTOR_PRIORITY_IGNORES_RISES;
		return actor;
	}
	return 0;
}

#include "../field/field_effect_move.c"

#define EFFECT_SET_MODE	  fdActorSelectAnimation
#define EFFECT_SET_SCRIPT fdActorStartScript
#define EFFECT_SPRITE_PRIORITY
#include "../field/field_effect_spawn.c"

struct ACTOR;

void scGroundParticlesSetAnchor(Sint32 id);
void scAquaRockSetBubbleAnchor(Sint32 id);
void scLeaderFlowWithStream(void);
void scActorSpawnWadingSplash(ACTOR *actor);
void scAquaRockAddMoaiWater(Sint32 id);
void scActorEmitRipples(ACTOR *actor);

void   scAquaRockKeepLeaderPriority(void);
Sint32 scAquaRockStartBubbles(void);
void   scAquaRockMoaiStepAside(void);
void   scAquaRockSetUpStreams(void);
void   scAquaRockArriveFromPassage(void);
void   scAquaRockArriveFromInside(void);
void   scAquaRockArriveUpStream(void);

extern Sint16 GroundParticleFrames[];

Sint32 scAquaRockStartFish(ACTOR *actor);
void   scAquaRockRideStreamUp(void);
void   scAquaRockStepIntoCurrent(void);

void scAquaRockDouseDryMoai(void);

void scAquaRockDropDownFalls(void);

/* One of the streams the moai pour, which stops and starts with its flag: each
   change runs down it as a wave, copying three metatiles a row from the flowing
   or stopped pictures beside it, a row every four ticks. */
typedef struct AQUA_ROCK_STREAM {
	Sint16 pictureSrcX, pictureSrcY; /* its pictures, flowing then stopped */
	/* its metatiles, flowing then stopped */
	Sint16 metatileSrcX, metatileSrcY;
	Sint16 pictureX, pictureY;	 /* where its pictures are copied */
	Sint16 metatileX, metatileY; /* where its metatiles are copied */
	Sint16 height;				 /* its rows */
	/* The flag that stops it, or IF_FLAG_SET of one that sets it flowing. */
	Sint16 flg;
	Sint16 shown;		 /* the flag's value it was last told */
	Sint16 waveRow[2];	 /* each wave's row times four, or STREAM_NO_WAVE */
	Sint16 waveStops[2]; /* whether each wave stops the stream or starts it */
	Sint16 nextWave;
} AQUA_ROCK_STREAM;

/* AQUA_ROCK_STREAM.waveRow for no wave. */
#define STREAM_NO_WAVE 9999

/* The five moai streams scAquaRockUpdateStreams stops and starts: the four red
   moai's and the dry moai's. */
AQUA_ROCK_STREAM AquaRockStreams[5] BSS_DATA ALIGNED(16);
AQUA_ROCK_BUBBLES AquaRockBubbles			 BSS_DATA;

const SCENE_ENTRANCE *scGetEntrances(void)
{
	return SceneEntrances;
}

const SCENE_REGION *scGetExitRegions(void)
{
	Sint16 scene = MainRoutineStateWork.next;

	if(scene == SCENE_AQUA_ROCK_MOAI)
		return AquaRockSlopesRegions;
	if(scene == SCENE_AQUA_ROCK_MIST_SABRE_CHEST)
		return AquaRockMistSabreChestRegions;
	return 0;
}

const Uint32 *scGetExits(void)
{
	return SceneExits;
}

/* A fish's script call: picks a random spot ahead within 16 pixels of its home
   and swims there; after three misses it turns around. */
Sint32 scAquaRockPickRandomSpot(ACTOR *actor)
{
	VECTOR3 spot;
	/* LEFTOVER(frame): an unused local the ROM's frame reserves and never
	 * touches. */
	VECTOR3 unused;
	Sint32	x;
	FIXED	coordinate;
	Sint32	tries = 0;
	Sint32	maxDistanceSquared = 16 * 16;
	FIXED	distance;
	Sint32	dx, dz;
	Uint32	a, b;

again:
	tries++;
	if(tries <= 3) {
		spot.x = actor->x.fixed;
		spot.y = actor->y.fixed;
		spot.z = actor->z.fixed;
		distance = syMulFX(syRandom(), toFIXED(8)) + toFIXED(4);
		a = syRandom();
		b = syRandom();
		syVectorAddPolarOffset(
			distance, actor->facing + (a >> 2) - (b >> 2), &spot);
		/* The spot's distance from home in whole pixels, rounded toward
		   zero. */
		/* FAKEMATCH(temporary): x, a copy of spot.x on which the division by
		 * toFIXED(1) is written out by hand; as spot.x / toFIXED(1). */
		x = spot.x;
		if(x < 0)
			x += toFIXED(1) - 1;
		dx = FIXEDtoINT(x) - actor->tmr;
		/* FAKEMATCH(temporary): coordinate, a copy of spot.z on which the
		 * division by toFIXED(1) is written out by hand; as spot.z /
		 * toFIXED(1). */
		coordinate = spot.z;
		if(coordinate < 0)
			coordinate += toFIXED(1) - 1;
		dz = FIXEDtoINT(coordinate) - actor->userValue;
		if(dx * dx + dz * dz > maxDistanceSquared)
			goto again;
		goto found;
	}
	actor->facing += DEGtoANG(180);
	actor->wait = 1;
	goto end;
found:
	fdActorSetMoveTarget(actor, spot.x, spot.y, spot.z);
end:
	/* FAKEMATCH(dead): x set to 0, a store nothing reads. */
	x = 0;
	/* FAKEMATCH(dead): coordinate set to 0, a store nothing reads. */
	coordinate = 0;
	return 0;
}

/* A fish's update: its sprite turns with its facing. */
void scAquaRockTurnFish(ACTOR *actor)
{
	FIELD_SPR *spr;
	Sint32	   ang;

	ang = actor->facing;
	spr = actor->spr;
	spr->rotation = ang + DEGtoANG(90);
}

/* A fish's script call: takes away its shadow and gives it its speed and its
   turning update. */
Sint32 scAquaRockStartFish(ACTOR *actor)
{
	fdActorSetSprFlags(actor, 0);
	actor->speedLimit = toFIXED(1);
	actor->acceleration = toFIXED(1) / 20;
	actor->collisionFlg = 0;
	actor->update = (OBJECT_UPDATE)scAquaRockTurnFish;
	return 0;
}

const ACTOR_PLACE *scGetActorPlacements(void)
{
	Sint16 scene = MainRoutineStateWork.next;

	if(scene == SCENE_AQUA_ROCK_MOAI)
		return AquaRockSlopesPlacements;
	if(scene == SCENE_AQUA_ROCK_MIST_SABRE_CHEST)
		return AquaRockMistSabreChestPlacements;
	return AquaRockPlacements;
}

/* Unused: a sparkle's update. It rises to the water's surface and then flows
   north up the stream, until it reaches the top or the stream's end. */
void scAquaRockFlowSparkleUpStream(ACTOR *actor)
{
	if(fdMapGetTerrainHeight(0, actor->x.fixed, actor->z.fixed) >
		actor->y.fixed)
		actor->y.fixed += toFIXED(2);
	else
		actor->z.fixed -= toFIXED(2);
	if(FIELD_METATILE(actor->y.fixed) == 23)
		fdActorRelease(actor);
	if(FIELD_METATILE(actor->z.fixed) == 48)
		fdActorRelease(actor);
}

/* Unused: now and then a sparkle starts up the stream from somewhere along the
   actor's 48-pixel width. */
UNUSED void scAquaRockSpawnSparkleAtRandom(ACTOR *actor)
{
	if((Uint32)syRandom() < 4096) {
		ACTOR *child;
		Sint32 spread = syRandom();
		FIXED  x = actor->x.fixed;

		x += spread * 48;
		child = syImportActorCreate(
			SPRITE_GLINT, x - toFIXED(24), actor->y.fixed, actor->z.fixed);
		if(child != 0) {
			fdActorSetSprFlags(child, 0);
			child->update = (OBJECT_UPDATE)scAquaRockFlowSparkleUpStream;
			child->motionFlg = 0;
		}
	}
}

/* A moai's stream stops or starts as a wave running down it from the moai's
   mouth, a row every four ticks. It runs every tick, sending a wave down each
   stream whose flag has changed. */
void scAquaRockUpdateStreams(void)
{
	AQUA_ROCK_STREAM *entry;
	Sint32			  n, i, flg;

	entry = AquaRockStreams;
	for(n = 0; n <= 4; n++, entry++) {
		flg = syGameFlagGet(entry->flg & (CONDITION_FLAG_SET - 1));
		if(entry->flg & CONDITION_FLAG_SET)
			flg ^= 1;
		if(entry->shown != flg) {
			entry->waveRow[entry->nextWave] = 0;
			entry->waveStops[entry->nextWave] = flg;
			if(++entry->nextWave > 1)
				entry->nextWave = 0;
			entry->shown = flg;
		}
		for(i = 0; i < 2; i++) {
			Sint32 row = entry->waveRow[i];
			if(row < entry->height * 4) {
				Sint32 inverse;
				row /= 4;
				flg = entry->waveStops[i];
				inverse = flg ^ 1;
				if(entry->waveRow[i] & 1) {
					fdMapGridCopyMetatilesLayered(flg * 3 + entry->pictureSrcX,
						entry->pictureSrcY + row, 3, 1, entry->pictureX,
						entry->pictureY + row);
					fdMapGridCopyMetatiles(flg * 3 + entry->metatileSrcX,
						entry->metatileSrcY + row, 3, 1, entry->metatileX,
						entry->metatileY + row);
				} else {
					fdMapGridCopyMetatilesLayered(
						inverse * 3 + entry->pictureSrcX,
						entry->pictureSrcY + row, 3, 1, entry->pictureX,
						entry->pictureY + row);
					fdMapGridCopyMetatiles(inverse * 3 + entry->metatileSrcX,
						entry->metatileSrcY + row, 3, 1, entry->metatileX,
						entry->metatileY + row);
				}
				entry->waveRow[i]++;
			}
		}
	}
}

extern Sint32 AquaRockFirstStreamTimer;
extern Sint32 AquaRockSecondStreamTimer;
extern Sint32 AquaRockThirdStreamTimer;
extern Sint32 AquaRockFourthStreamTimer;

/* The ticks of a red moai's stream's cycle, and the last of them it stops
   for. */
#define STREAM_CYCLE_TICKS 240
#define STREAM_STOP_TICKS  90

/* The four red moai's streams flow for two and a half seconds and stop for one
   and a half, over and over, and the top two stay stopped while the pillar
   stands before them. It runs every tick while no script runs, setting each
   stream's stopped flag, which scAquaRockUpdateStreams turns into the wave down
   the stream. */
void scAquaRockTimeStreams(void)
{
	if(syGameFlagGet(FLAG_FIELD_HANDLING_REQUESTS))
		return;
	if(AquaRockFirstStreamTimer < STREAM_CYCLE_TICKS - STREAM_STOP_TICKS) {
		if(syGameFlagGet(FLAG_PILLAR_BEFORE_WEST_MOAI))
			syGameFlagSet(FLAG_WEST_MOAI_STREAM_STOPPED);
		else
			syGameFlagClear(FLAG_WEST_MOAI_STREAM_STOPPED);
	} else {
		syGameFlagSet(FLAG_WEST_MOAI_STREAM_STOPPED);
	}
	if(++AquaRockFirstStreamTimer >= STREAM_CYCLE_TICKS)
		AquaRockFirstStreamTimer = 0;
	if(AquaRockSecondStreamTimer < STREAM_CYCLE_TICKS - STREAM_STOP_TICKS) {
		if(syGameFlagGet(FLAG_PILLAR_BEFORE_EAST_MOAI))
			syGameFlagSet(FLAG_EAST_MOAI_STREAM_STOPPED);
		else
			syGameFlagClear(FLAG_EAST_MOAI_STREAM_STOPPED);
	} else {
		syGameFlagSet(FLAG_EAST_MOAI_STREAM_STOPPED);
	}
	if(++AquaRockSecondStreamTimer >= STREAM_CYCLE_TICKS)
		AquaRockSecondStreamTimer = 0;
	if(AquaRockThirdStreamTimer < STREAM_CYCLE_TICKS - STREAM_STOP_TICKS)
		syGameFlagClear(FLAG_LOWER_WEST_MOAI_STREAM_STOPPED);
	else
		syGameFlagSet(FLAG_LOWER_WEST_MOAI_STREAM_STOPPED);
	if(++AquaRockThirdStreamTimer >= STREAM_CYCLE_TICKS)
		AquaRockThirdStreamTimer = 0;
	if(AquaRockFourthStreamTimer < STREAM_CYCLE_TICKS - STREAM_STOP_TICKS)
		syGameFlagClear(FLAG_LOWER_EAST_MOAI_STREAM_STOPPED);
	else
		syGameFlagSet(FLAG_LOWER_EAST_MOAI_STREAM_STOPPED);
	if(++AquaRockFourthStreamTimer >= STREAM_CYCLE_TICKS)
		AquaRockFourthStreamTimer = 0;
}

/* Sets up a stream (AQUA_ROCK_STREAM) to show its flag's opposite, so that its
   first update runs a wave to the flag's state. */
#define STREAM_SET(stream, pictureSourceX_, pictureSourceY_, metatileSourceX_, \
	metatileSourceY_, pictureX_, pictureY_, metatileX_, metatileY_, height_, \
	flag_) \
	(stream)->pictureSrcX = (pictureSourceX_); \
	(stream)->pictureSrcY = (pictureSourceY_); \
	(stream)->metatileSrcX = (metatileSourceX_); \
	(stream)->metatileSrcY = (metatileSourceY_); \
	(stream)->pictureX = (pictureX_); \
	(stream)->pictureY = (pictureY_); \
	(stream)->metatileX = (metatileX_); \
	(stream)->metatileY = (metatileY_); \
	(stream)->height = (height_); \
	(stream)->flg = (flag_); \
	(stream)->shown = syGameFlagGet(flag_) ^ 1; \
	(stream)->nextWave = 0; \
	(stream)->waveRow[1] = STREAM_NO_WAVE; \
	(stream)->waveRow[0] = STREAM_NO_WAVE

/* Sets the five streams with their pictures, metatiles and flags, stops at once
   the top two the pillar stands before, and starts the streams' events. */
void scAquaRockSetUpStreams(void)
{
	AQUA_ROCK_STREAM *entry = AquaRockStreams;

	STREAM_SET(entry, 64, 74, 76, 74, 70, 10, 6, 74, 36,
		FLAG_WEST_MOAI_STREAM_STOPPED);
	entry++;
	STREAM_SET(entry, 70, 74, 82, 74, 78, 10, 14, 74, 36,
		FLAG_EAST_MOAI_STREAM_STOPPED);
	entry++;
	STREAM_SET(entry, 90, 93, 102, 93, 103, 26, 39, 90, 15,
		FLAG_LOWER_WEST_MOAI_STREAM_STOPPED);
	entry++;
	STREAM_SET(entry, 96, 93, 108, 93, 107, 26, 43, 90, 15,
		FLAG_LOWER_EAST_MOAI_STREAM_STOPPED);
	entry++;
	STREAM_SET(entry, 116, 95, 122, 95, 96, 32, 32, 96, 13,
		IF_FLAG_SET(FLAG_AQUA_ROCK_DRY_MOAI_FILLED));
	if(syGameFlagGet(FLAG_PILLAR_BEFORE_WEST_MOAI))
		syGameFlagSet(FLAG_WEST_MOAI_STREAM_STOPPED);
	else
		syGameFlagClear(FLAG_WEST_MOAI_STREAM_STOPPED);
	if(syGameFlagGet(FLAG_PILLAR_BEFORE_EAST_MOAI))
		syGameFlagSet(FLAG_EAST_MOAI_STREAM_STOPPED);
	else
		syGameFlagClear(FLAG_EAST_MOAI_STREAM_STOPPED);
	if(syGameFlagGet(FLAG_WEST_MOAI_STREAM_STOPPED)) {
		AQUA_ROCK_STREAM *stream = &AquaRockStreams[0];
		fdMapGridCopyMetatilesLayered(stream->pictureSrcX + 3,
			stream->pictureSrcY, 3, stream->height, stream->pictureX,
			stream->pictureY);
		fdMapGridCopyMetatiles(stream->metatileSrcX + 3, stream->metatileSrcY,
			3, stream->height, stream->metatileX, stream->metatileY);
		stream->waveRow[1] = STREAM_NO_WAVE;
		stream->waveRow[0] = STREAM_NO_WAVE;
		stream->shown = 1;
	}
	if(syGameFlagGet(FLAG_EAST_MOAI_STREAM_STOPPED)) {
		AQUA_ROCK_STREAM *stream = &AquaRockStreams[1];
		fdMapGridCopyMetatilesLayered(stream->pictureSrcX + 3,
			stream->pictureSrcY, 3, stream->height, stream->pictureX,
			stream->pictureY);
		fdMapGridCopyMetatiles(stream->metatileSrcX + 3, stream->metatileSrcY,
			3, stream->height, stream->metatileX, stream->metatileY);
		stream->waveRow[1] = STREAM_NO_WAVE;
		stream->waveRow[0] = STREAM_NO_WAVE;
		stream->shown = 1;
	}
	sySetTickEvent(scAquaRockTimeStreams, EVENT_PASS_TICK);
	sySetEvent(scAquaRockUpdateStreams, EVENT_PASS_TICK);
}

#include "shared/actor_flash_white.h"

/* Unused: a moai's update while it flashes, grey every other two ticks. */
UNUSED void scAquaRockFlashMoai(ACTOR *actor)
{
	if(TickCount & 2)
		fdActorSetSprColMode(actor, SPRITE_COLOR_GREY);
	else
		fdActorSetSprColMode(actor, SPRITE_COLOR_NORMAL);
}

/* A flashing actor's last update: its sprite's colour back to normal. */
void scAquaRockStopFlashing(ACTOR *actor)
{
	fdActorSetSprColMode(actor, SPRITE_COLOR_NORMAL);
	actor->update = 0;
}

/* Douse on the slopes' Water Stone: it flickers, the screen flashes four
   colours and turns white, and the streams turn back to run uphill, bubbles
   rising from the stone; Doused again, they run downhill once more. */
void scAquaRockDouseWaterStone(Sint32 unused, Sint32 id)
{
	ACTOR *actor = fdActorGetById(id);
	Sint32 i;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	actor->update = (OBJECT_UPDATE)scActorFlashWhite;
	sySynch(30);
	if(syGameFlagGet(FLAG_AQUA_ROCK_STREAMS_TURNED_BACK) == 0)
		scGroundParticlesSetAnchor(ACTOR_WATER_STONE);
	else
		scGroundParticlesSetAnchor(0);
	for(i = 0; i < 4; i++) {
		fdPaletteFadeSetTarget(syRandom() & 0x7fff, 0);
		fdPaletteFadeStartInterpolation(15);
		sySynch(10);
		fdPaletteFadeSetTarget(PALETTE_TINT_NONE, PALETTES_ALL);
		fdPaletteFadeStartInterpolation(10);
		sySynch(10);
	}
	fdPaletteFadeSetTarget(RGB_WHITE, PALETTES_ALL);
	fdPaletteFadeStartInterpolation(20);
	fdScriptWait(20);
	if(fdScriptGetFlag(FLAG_AQUA_ROCK_STREAMS_TURNED_BACK) == 0) {
		syGameFlagSet(FLAG_AQUA_ROCK_STREAMS_TURNED_BACK);
		fdMapGridCopyMetatiles(64, 64, 1, 2, 51, 82);
		fdMapGridCopyMetatiles(66, 64, 1, 2, 115, 18);
		fdTilesetAnimPause(STREAMS_DOWNHILL_ANIMATION);
		fdTilesetAnimPause(STREAMS_DOWNHILL_ANIMATION + 1);
		fdTilesetAnimResume(STREAMS_UPHILL_ANIMATION);
		fdTilesetAnimResume(STREAMS_UPHILL_ANIMATION + 1);
		actor->update = (OBJECT_UPDATE)scAquaRockStopFlashing;
		scAquaRockSetBubbleAnchor(ACTOR_WATER_STONE);
	} else {
		syGameFlagClear(FLAG_AQUA_ROCK_STREAMS_TURNED_BACK);
		fdMapGridCopyMetatiles(65, 64, 1, 2, 51, 82);
		fdMapGridCopyMetatiles(67, 64, 1, 2, 115, 18);
		fdTilesetAnimPause(STREAMS_UPHILL_ANIMATION);
		fdTilesetAnimPause(STREAMS_UPHILL_ANIMATION + 1);
		fdTilesetAnimResume(STREAMS_DOWNHILL_ANIMATION);
		fdTilesetAnimResume(STREAMS_DOWNHILL_ANIMATION + 1);
		actor->update = (OBJECT_UPDATE)scAquaRockStopFlashing;
		scAquaRockSetBubbleAnchor(0);
	}
	fdScriptWait(30);
	fdPaletteFadeSetTarget(PALETTE_TINT_NONE, PALETTES_ALL);
	fdPaletteFadeStartInterpolation(30);
	fdScriptWait(15);
	fdScriptEnd();
}

void syGameFlagSetByte(Sint32 flg, Sint32 value);

/* Move on a pillar. The pillar by the top's red moai keeps its metatile in the
   area's flags, and stopped before one of them stops that moai's stream; the
   ledge's pillar, pushed onto its hole, drops into it. */
void scAquaRockMovePillar(Sint32 unused, Sint32 id)
{

	if(id == ACTOR_MOAI_PILLAR) {
		Sint32 column;
		Sint32 row;

		ACTOR *pillar = fdActorGetById(ACTOR_MOAI_PILLAR);

		column = FIELD_METATILE(pillar->x.fixed);
		row = FIELD_METATILE(pillar->z.fixed);
		syGameFlagSetByte(FLAG_PILLAR_COLUMN, column);
		syGameFlagSetByte(FLAG_PILLAR_ROW, row);
		fdScriptClearFlag(FLAG_PILLAR_BEFORE_WEST_MOAI);
		syGameFlagClear(FLAG_PILLAR_BEFORE_EAST_MOAI);
		if(column == WEST_MOAI_COLUMN && row == MOAI_FRONT_ROW)
			syGameFlagSet(FLAG_PILLAR_BEFORE_WEST_MOAI);
		if(column == EAST_MOAI_COLUMN && row == MOAI_FRONT_ROW)
			syGameFlagSet(FLAG_PILLAR_BEFORE_EAST_MOAI);
	} else if(id == ACTOR_LEDGE_PILLAR) {
		ACTOR *actor = fdActorGetById(ACTOR_LEDGE_PILLAR);

		if(FIELD_METATILE(actor->x.fixed) == PILLAR_HOLE_COLUMN) {
			sySynch(30);
			actor->z.fixed += toFIXED(1);
			syGameFlagSet(FLAG_AQUA_ROCK_PILLAR_IN_HOLE);
		}
	}
}

/* Unused: waits until the actor has come down to `height`, a second at most. */
UNUSED void scActorWaitBelowHeight(ACTOR *actor, FIXED height)
{
	Sint32 ticks = 60;

	while(ticks != 0) {
		sySynch(1);
		ticks--;
		if(actor->y.fixed <= height)
			break;
	}
}

/* Felix, walking into one of the streams the slopes' Water Stone turns back, is
   carried up it, splashing, once the stone has been Doused; at the head of the
   slopes' stream he spins up out of the water and away into the Mist Sabre
   chest's room. Until then, or once Doused again, the stream carries him down
   instead (scLeaderFlowWithStream). It runs every tick he stands on the
   stream's metatiles, moving him and the camera two pixels north, and leaping
   him up each ledge the stream climbs. */
void scAquaRockRideStreamUp(void)
{
	ACTOR		*leader, *carried;
	VECTOR3		 ahead;
	Sint32		 owner;
	FIELD_STATE *state;
	Sint32		 result, i;

	if(syGameFlagGet(FLAG_AQUA_ROCK_STREAMS_TURNED_BACK)) {
		state = Ram_HeapSlots->fieldState;
		owner = MainRoutineStateWork.leaderActorId;
		carried = state->camera;
		leader = fdActorGetById(owner);
		if(MainRoutineStateWork.next == SCENE_AQUA_ROCK_MOAI &&
			leader->groundHeight == 0)
			return;
		ahead.x = leader->x.fixed;
		ahead.y = leader->y.fixed;
		ahead.z = leader->z.fixed + toFIXED(2);
		result = fdMapGridCheckCollision(leader, &ahead);
		if((TickCount & 4) == 0)
			scActorSpawnWadingSplash(leader);
		if(result < 0) {
			fdActorSetAttachedEffect(
				owner, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
			fdActorSetMoveTarget(leader, leader->x.fixed, leader->y.fixed,
				leader->z.fixed - toFIXED(8));
			fdActorSelectAnimation(leader, ANIM_LEADER_LEAP);
			fdActorWaitForMove(leader);
			do
				sySynch(1);
			while(leader->y.fixed != leader->groundHeight);
			scActorSpawnWadingSplash(leader);
			fdActorSelectAnimation(leader, ANIM_LEADER_SPRING);
			sySynch(3);
			return;
		}
		ahead.x = leader->x.fixed;
		ahead.y = leader->y.fixed;
		ahead.z = leader->z.fixed - toFIXED(8);
		result = fdMapGridCheckCollision(leader, &ahead);
		if(result > 0) {
			if(MainRoutineStateWork.next != SCENE_AQUA_ROCK_MOAI)
				return;
			fdScriptStart();
			fdScriptEndLastingPsynergy(0);
			leader->motionFlg = 0;
			fdActorSetSprFlags(leader, 0);
			MainRoutineStateWork.movementMode = LEADER_MOVEMENT_KEYS;
			scActorSpawnWadingSplash(leader);
			fdScriptWait(5);
			for(i = 0; i < 60; i++) {
				leader->y.fixed += toFIXED(2);
				leader->facing += TRIG_TURN / 16;
				if(syRandom() <= 4095)
					scActorSpawnWadingSplash(leader);
				sySynch(1);
			}
			MainRoutineStateWork.movementMode = LEADER_MOVEMENT_WALK;
			fdScriptEnd();
			syImportScriptSetExitRequest(
				AQUA_ROCK_MOAI_TO_AQUA_ROCK_MIST_SABRE_CHEST_C);
			return;
		}
		ahead.x = leader->x.fixed + (toFIXED(57) / 10);
		ahead.y = leader->y.fixed;
		ahead.z = leader->z.fixed - (toFIXED(57) / 10);
		result = fdMapGridCheckCollision(leader, &ahead);
		if(result > 0)
			return;
		ahead.x = leader->x.fixed - (toFIXED(57) / 10);
		ahead.y = leader->y.fixed;
		ahead.z = leader->z.fixed - (toFIXED(57) / 10);
		result = fdMapGridCheckCollision(leader, &ahead);
		if(result > 0)
			return;
		carried->z.fixed -= toFIXED(2);
		leader->z.fixed -= toFIXED(2);
	} else
		scLeaderFlowWithStream();
}

/* The attribute byte of a metatile of a layer. */
static inline Uint8 MetatileAttribute(
	MAP_METATILE *metatiles, Sint32 row, Sint32 col)
{
	metatiles += row * MAPGRID_SIZE + col;
	return metatiles->attributeB;
}

/* Felix, stepping into the stream one of the red moai pours, is swept away down
   the slopes, tumbling over the ledges, and where it carries him over the edge
   of the falls he drops down them, flailing, to the pool below
   (scAquaRockDropDownFalls). It runs every tick he stands on the stream's
   metatiles while it flows; the top two moai's streams take him only while the
   pillar does not stop them. */
void scAquaRockStepIntoCurrent(void)
{
	MAP_STATE	*map = Ram_HeapSlots->mapGrid;
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	ACTOR		*actor = fdActorGetById(MainRoutineStateWork.leaderActorId);
	Sint32		 col;
	Sint32		 row;

	if(MainRoutineStateWork.movementMode == LEADER_MOVEMENT_KEYS) {
		col = FIELD_METATILE(actor->x.fixed);
		row = FIELD_METATILE(actor->z.fixed - actor->y.fixed - toFIXED(12));
		if(MetatileAttribute(map->layers[1].metatiles, row, col) != 0)
			state->raisedCue = CUE_DROP_DOWN_FALLS;
	} else {
		col = FIELD_METATILE(actor->x.fixed);
		row = FIELD_METATILE(actor->z.fixed - actor->y.fixed);
		if(MetatileAttribute(map->layers[1].metatiles, row, col) != 0)
			scLeaderFlowWithStream();
	}
}

/* Douse on the dry moai: it fills, turns blue and pours, and its stream down
   the slopes starts to flow. */
void scAquaRockDouseDryMoai(void)
{
	fdMapGridCopyMetatileAttributes(62, 41, 1, 2, 33, 41);
	fdMapGridCopyMetatileTilesMetatilesRect(125, 32, 3, 13, 96, 32);
	fdMapGridCopyMetatileTilesMetatilesRect(61, 96, 3, 13, 32, 96);
	fdActorSetSprCol(fdActorGetById(ACTOR_DRY_MOAI), SPRITE_COLOR_BRG);
	scAquaRockAddMoaiWater(ACTOR_DRY_MOAI);
	syGameFlagSet(FLAG_AQUA_ROCK_DRY_MOAI_FILLED);
}

/* The leader drops down the falls, flailing, and lands, for at most three
   seconds. */
void scAquaRockDropDownFalls(void)
{
	ACTOR *actor = fdActorGetById(MainRoutineStateWork.leaderActorId);
	Sint32 saved = actor->gravity;
	Sint32 i;
	MAIN_ROUTINE_STATE *state;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdEffectSetAttached(MainRoutineStateWork.leaderActorId,
		ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
	fdActorSelectAnimation(actor, ANIM_LEADER_FLAIL);
	actor->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	for(i = 0; i < 180; i++) {
		sySynch(1);
		if(actor->y.fixed <= actor->groundHeight)
			break;
	}
	state = &MainRoutineStateWork;
	actor->gravity = saved;
	fdEffectSetAttached(state->leaderActorId, ATTACHED_EFFECT_REMOVE);
	fdActorSetSprFlags(actor, SPRITE_FLAG_SHADOW);
	fdActorSelectAnimation(actor, ANIM_STAND);
	state->movementMode = LEADER_MOVEMENT_WALK;
	fdScriptEnd();
}

/* Defines Frost on a puddle: it freezes into a step, its metatile taking the
   attributes of the frozen metatile at (sourceX, row), and its flag is set. */
#define DEFINE_AQUA_ROCK_FREEZE_PUDDLE(name, sourceX, row, column, flag) \
	void name(Sint32 unused, Sint32 id) \
	{ \
		ACTOR *actor = fdActorGetById(id); \
\
		actor->priFlg = 0; \
		actor->motionFlg = 0; \
		fdMapGridCopyMetatileAttributes(sourceX, row, 1, 1, column, row); \
		syGameFlagSet(flag); \
	}

/* Frost on the Mist Sabre chest's room's first puddle: it freezes into a
   step. */
DEFINE_AQUA_ROCK_FREEZE_PUDDLE(
	AquaRock_FreezeFirstPuddle, 42, 32, 44, FLAG_PUDDLE_A_FROZEN)

/* Frost on the second puddle: it freezes into a step. */
DEFINE_AQUA_ROCK_FREEZE_PUDDLE(
	AquaRock_FreezeSecondPuddle, 48, 32, 46, FLAG_PUDDLE_B_FROZEN)

/* Frost on the third puddle: it freezes into a step. */
DEFINE_AQUA_ROCK_FREEZE_PUDDLE(
	AquaRock_FreezeThirdPuddle, 44, 44, 46, FLAG_PUDDLE_C_FROZEN)

/* Defines Move on a rock: pushed to the stream's column it drops in, its flag
   is set and its metatile in `row` takes the attributes of the stream's
   rock-filled metatile. */
#define DEFINE_AQUA_ROCK_DROP_AT_COLUMN(name, flag, row) \
	void name(Sint32 unused, Sint32 id) \
	{ \
		ACTOR *actor = fdActorGetById(id); \
		Sint32 column = FIELD_METATILE(actor->x.fixed); \
\
		if(column == ROCK_STREAM_COLUMN) { \
			sySynch(30); \
			actor->z.fixed += toFIXED(1); \
			syGameFlagSet(flag); \
			fdMapCopyMetatileAttributeRect(54, 58, 1, 1, column, row); \
		} \
	}

/* Move on the first rock to the stream: it drops in. */
DEFINE_AQUA_ROCK_DROP_AT_COLUMN(
	AquaRock_DropFirstRock, FLAG_AQUA_ROCK_FIRST_ROCK_DROPPED, 52)

/* Move on the second rock to the stream: it drops in. */
DEFINE_AQUA_ROCK_DROP_AT_COLUMN(
	AquaRock_DropSecondRock, FLAG_AQUA_ROCK_SECOND_ROCK_DROPPED, 58)

/* Douse on the Mist Sabre chest's room's Water Stone: the screen flashes four
   colours and turns white, the stone's bubbles start, the pool's metatiles
   change and the camera pans down to the pool, where rings of water start to
   rise over the way inside. */
void scAquaRockDouseMistSabreChestWaterStone(Sint32 unused, Sint32 id)
{
	ACTOR *actor = fdActorGetById(id);
	Sint32 i;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	actor->update = (OBJECT_UPDATE)scActorFlashWhite;
	sySynch(30);
	scGroundParticlesSetAnchor(ACTOR_MIST_SABRE_WATER_STONE);
	sySndPlay(SE_SHIMMER);
	for(i = 0; i < 4; i++) {
		fdPaletteFadeSetTarget(syRandom() & 0x7fff, 0);
		fdPaletteFadeStartInterpolation(15);
		sySynch(10);
		fdPaletteFadeSetTarget(PALETTE_TINT_NONE, PALETTES_ALL);
		fdPaletteFadeStartInterpolation(10);
		sySynch(10);
	}
	fdPaletteFadeSetTarget(RGB_WHITE, PALETTES_ALL);
	fdPaletteFadeStartInterpolation(20);
	fdScriptWait(20);
	syGameFlagSet(FLAG_AQUA_ROCK_MIST_SABRE_CHEST_WATER_STONE_DOUSED);
	fdMapCopyMetatileAttributeRect(15, 48, 1, 1, 16, 48);
	fdMapCopyMetatileAttributeRect(17, 50, 1, 1, 16, 50);
	actor->update = (OBJECT_UPDATE)scAquaRockStopFlashing;
	scAquaRockSetBubbleAnchor(ACTOR_MIST_SABRE_WATER_STONE);
	fdMapGridCopyMetatileTilesMetatilesRect(64, 68, 5, 13, 14, 94);
	fdMapCopyMetatileAttributeRect(64, 68, 5, 8, 14, 34);
	fdScriptWait(30);
	fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
	fdPaletteFadeStartInterpolation(30);
	fdScriptWait(15);
	fdCameraSetSpeed(1);
	fdCameraMoveTo(248, -32, 760, 1);
	fdCameraWaitForArrival();
	fdActorSetPos(ACTOR_WAY_INSIDE, 264, 792);
	fdActorSetSprFlags(fdActorGetById(ACTOR_WAY_INSIDE), 0);
	fdActorGetById(ACTOR_WAY_INSIDE)->collisionFlg = 0;
	fdActorGetById(ACTOR_WAY_INSIDE)->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
	fdActorGetById(ACTOR_WAY_INSIDE)->update =
		(OBJECT_UPDATE)scActorEmitRipples;
	fdScriptWait(30);
	for(i = 0; i < 6; i++) {
		sySndPlay(SE_WADE);
		sySynch(10);
	}
	fdScriptWait(30);
	fdScriptEnd();
}

const CUE *scGetCues(void)
{
	Sint16 scene = MainRoutineStateWork.next;

	if(scene == SCENE_AQUA_ROCK_MOAI)
		return AquaRockSlopesCues;
	if(scene == SCENE_AQUA_ROCK_MIST_SABRE_CHEST)
		return AquaRockMistSabreChestCues;
	return AquaRockCues;
}

/* On the slopes Felix is drawn behind the falls while a current carries him,
   and in front of them otherwise. It runs every tick, giving his sprite
   priority 2 while he is carried and 1 otherwise. */
void scAquaRockKeepLeaderPriority(void)
{
	ACTOR *actor = fdActorGetById(fdPartyGetLeaderActor());

	if(actor != 0 &&
		(actor->sprStorageKind & ACTOR_SPRITE_KIND_MASK) == ACTOR_SPRITE_ONE) {
		FIELD_SPR *spr;
		Sint32	   pri;

		spr = actor->spr;
		pri = MainRoutineStateWork.movementMode == LEADER_MOVEMENT_KEYS ? 2 : 1;
		spr->pri = pri;
		spr->partPri = pri;
		actor->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;
	}
}

/* Arriving from inside, the Vial chest's room below: the leader spins up out of
   the pool and lands, going on north or south by the key held. */
void scAquaRockArriveFromInside(void)
{
	ACTOR *actor = fdActorGetById(MainRoutineStateWork.leaderActorId);
	Sint32 saved = actor->motionFlg;
	Sint32 i;
	Sint32 ang;
	Sint32 up;
	MAIN_ROUTINE_STATE *state;

	sySndPlay(SE_SPIN_AWAY);
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdScriptStartScreenTransition();
	fdActorSetSprFlags(actor, 0);
	actor->motionFlg = 0;
	actor->y.fixed = -toFIXED(16);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 2);
	for(i = 0; i < 16; i++) {
		actor->y.fixed += toFIXED(1);
		ang = actor->facing;
		ang += DEGtoANG(45);
		actor->facing = ang;
		sySynch(1);
	}
	scActorSpawnWadingSplash(actor);
	actor->motionFlg = saved;
	actor->velY = toFIXED(8);
	fdActorSetSprFlags(actor, SPRITE_FLAG_SHADOW);
	up = PerWork.data & KEY_UP;
	state = &MainRoutineStateWork;
	if(up)
		fdActorMoveToAndWaitById(state->leaderActorId, 264, 760);
	else
		fdActorMoveToAndWaitById(state->leaderActorId, 264, 824);
	fdScriptEnd();
}

/* Arriving by the slopes' stream turned back: it throws the leader spinning up
   out of the water, and he walks out onto the bank. */
void scAquaRockArriveUpStream(void)
{
	ACTOR *actor = fdActorGetById(MainRoutineStateWork.leaderActorId);
	Sint32 saved = actor->motionFlg;
	Sint32 i;
	Sint32 ang;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdScriptStartScreenTransition();
	fdActorSetSprFlags(actor, 0);
	actor->motionFlg = 0;
	actor->y.fixed = -toFIXED(64);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 2);
	for(i = 0; i < 24; i++) {
		actor->y.fixed += toFIXED(2);
		ang = actor->facing;
		ang += DEGtoANG(45);
		actor->facing = ang;
		sySynch(1);
	}
	scActorSpawnWadingSplash(actor);
	actor->motionFlg = saved;
	actor->velY = toFIXED(12);
	fdActorSetSprFlags(actor, SPRITE_FLAG_SHADOW);
	fdActorMoveToAndWaitById(MainRoutineStateWork.leaderActorId, 680, 912);
	fdScriptEnd();
}

/* The farthest a guardian moai rocks either way as it shakes. */
#define MOAI_SHAKE (TRIG_TURN / 128)

/* A guardian moai's update as it moves aside: it shakes, the swing growing over
   16 ticks to MOAI_SHAKE. */
void scAquaRockShakeMoai(ACTOR *actor)
{
	FIELD_SPR *spr = actor->spr;

	/* timer is the phase of the shake and userValue its swing. */
	actor->tmr += DEGtoANG(45);
	actor->userValue += MOAI_SHAKE / 16;
	if(actor->userValue > MOAI_SHAKE)
		actor->userValue = MOAI_SHAKE;
	spr->rotation = syMulFX(sySin(actor->tmr), actor->userValue);
}

/* A guardian moai's update once it has moved: the swing dies away over 32
   ticks. */
void scAquaRockStopShakingMoai(ACTOR *actor)
{
	FIELD_SPR *sprite = actor->spr;

	/* timer is the phase of the shake and userValue its swing. */
	actor->tmr += DEGtoANG(45);
	actor->userValue -= MOAI_SHAKE / 32;
	if(actor->userValue < 0) {
		actor->userValue = 0;
	}
	sprite->rotation = syMulFX(sySin(actor->tmr), actor->userValue);
}

/* The last guardian moai rocks a sixty-fourth of a turn either way: its sine,
   whose one is a whole turn, divided by this. */
#define MOAI_ROCK_DIVISOR 64
/* The last guardian moai's lean, about which it rocks. */
#define MOAI_LEAN		  (TRIG_TURN * 3 / 128)

/* The last guardian moai's update: it keeps rocking gently, leaning a
   little. */
void scAquaRockRockMoai(ACTOR *actor)
{
	FIELD_SPR *sprite = actor->spr;

	actor->tmr += TRIG_TURN / 256;
	sprite->rotation = sySin(actor->tmr) / MOAI_ROCK_DIVISOR + MOAI_LEAN;
}

/* Arriving by the north way, as Douse wakes the Apojii Islands' Aqua Stone: the
   five guardian moai shake and move aside into a row clear of the way in, the
   last left rocking, and the party goes back to the Apojii Islands where it
   stood. */
void scAquaRockMoaiStepAside(void)
{
	Sint32 i;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdActorSetPos(MainRoutineStateWork.leaderActorId, 0, 0);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	sySndPlay(SE_POP_UP);
	for(i = ACTOR_FIRST_GUARDIAN_MOAI; i <= ACTOR_FIRST_GUARDIAN_MOAI + 4;
		i++) {
		{
			ACTOR *actor = fdActorGetById(i);

			actor->speedLimit = toFIXED(1) / 4;
			actor->acceleration = toFIXED(1) / 64;
			actor->update = (OBJECT_UPDATE)scAquaRockShakeMoai;
			actor->tmr = syRandom();
			actor->userValue = 0;
		}
	}
	fdScriptWait(60);
	fdMapCopyMetatilesRect(64, 69, 11, 4, 87, 53);
	fdActorSetDestination(ACTOR_FIRST_GUARDIAN_MOAI, 392, 824);
	fdActorSetDestination(ACTOR_FIRST_GUARDIAN_MOAI + 1, 424, 824);
	fdActorSetDestination(ACTOR_FIRST_GUARDIAN_MOAI + 2, 456, 824);
	fdActorSetDestination(ACTOR_FIRST_GUARDIAN_MOAI + 3, 488, 824);
	fdActorSetDestinationById(ACTOR_FIRST_GUARDIAN_MOAI + 4, 520, 824);
	fdScriptWait(240);
	fdMapCopyMetatilesRect(64, 66, 11, 7, 87, 50);
	for(i = ACTOR_FIRST_GUARDIAN_MOAI; i <= ACTOR_FIRST_GUARDIAN_MOAI + 4;
		i++) {
		fdActorGetById(i)->update = (OBJECT_UPDATE)scAquaRockStopShakingMoai;
	}
	sySndPlay(SE_STOP_EXIT_SE);
	fdScriptWait(60);
	{
		ACTOR *actor = fdActorGetById(ACTOR_FIRST_GUARDIAN_MOAI + 4);

		actor->update = (OBJECT_UPDATE)scAquaRockRockMoai;
		actor->tmr = 0;
	}
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	syGameFlagSet(FLAG_KEEP_PARTY_POSITION);
	syGameFlagClear(FLAG_SHOW_AREA_NAME);
	syImportScriptSetExitRequest(AQUA_ROCK_MOAI_TO_APOJII_ISLANDS);
	fdScriptEnd();
	syGameFlagSet(FLAG_AQUA_ROCK_MOAI_STEPPED_ASIDE);
}

/* Sets up the room the party comes into, Retreat leading back to the slopes'
   south shore. On the slopes: the moai's colours and water, the pillars where
   they were left, the guardian moai aside once they have moved, the streams,
   and the bubbles and sparkles of a Doused Water Stone. In the Mist Sabre
   chest's room: its frozen puddles and dropped rocks, its Water Stone's bubbles
   and the way inside it opens, and the party's arrival up the stream or from
   inside. */
Sint32 scInit(void)
{
	Sint32					  scene = MainRoutineStateWork.next;
	MAP_STATE_GROUND_EFFECTS *map =
		(MAP_STATE_GROUND_EFFECTS *)Ram_HeapSlots->mapGrid;
	Sint32	id, x, z;
	Sint16 *entrance;

	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	MainRoutineStateWork.retreatScene = SCENE_AQUA_ROCK_MOAI;
	MainRoutineStateWork.retreatEntrance = AQUA_ROCK_MOAI_FROM_WORLD_MAP_SOUTH;
	map->wading = MAP_WADING_UPPER_LEVEL;
	fdQueueMusic(SE_AMBIENT_WATER);
	if(scene == SCENE_AQUA_ROCK_MOAI) {
		fdActorSetSprCol(fdActorGetById(ACTOR_FIRST_MOAI), SPRITE_COLOR_GBR);
		fdActorSetSprCol(
			fdActorGetById(ACTOR_FIRST_MOAI + 1), SPRITE_COLOR_GBR);
		fdActorSetSprCol(
			fdActorGetById(ACTOR_FIRST_MOAI + 2), SPRITE_COLOR_GBR);
		fdActorSetSprCol(
			fdActorGetById(ACTOR_FIRST_MOAI + 3), SPRITE_COLOR_GBR);
		if(syGameFlagGet(FLAG_AQUA_ROCK_DRY_MOAI_FILLED))
			fdActorSetSprCol(fdActorGetById(ACTOR_DRY_MOAI), SPRITE_COLOR_BRG);
		else
			fdActorSetSprCol(fdActorGetById(ACTOR_DRY_MOAI), SPRITE_COLOR_GREY);
		scAquaRockAddMoaiWater(ACTOR_FIRST_MOAI);
		scAquaRockAddMoaiWater(ACTOR_FIRST_MOAI + 1);
		scAquaRockAddMoaiWater(ACTOR_FIRST_MOAI + 2);
		scAquaRockAddMoaiWater(ACTOR_FIRST_MOAI + 3);
		scAquaRockAddMoaiWater(ACTOR_FIRST_MOAI + 5);
		scAquaRockAddMoaiWater(ACTOR_FIRST_MOAI + 6);
		for(id = ACTOR_FIRST_BOTTOM_MOAI; id <= ACTOR_LAST_MOAI; id++)
			scAquaRockAddMoaiWater(id);
		for(id = ACTOR_FIRST_MOAI; id < ACTOR_FIRST_BOTTOM_MOAI; id++)
			fdActorSetSprPriority(id, 3);
		for(id = ACTOR_FIRST_BOTTOM_MOAI; id <= ACTOR_LAST_MOAI; id++)
			fdActorSetSprPriority(id, 1);
		fdActorGetById(ACTOR_FIRST_BOTTOM_MOAI)->retainSprCache =
			ACTOR_COMPOSE_OFF_SCREEN;
		fdActorSetAnimationById(ACTOR_FIRST_BOTTOM_MOAI, 3);
		for(id = ACTOR_FIRST_BOTTOM_MOAI + 1; id <= ACTOR_LAST_MOAI; id++) {
			fdActorSetAnimationById(id, 1);
			fdActorSetAnimationById(id, 0);
		}
		if(syGameFlagGet(FLAG_AQUA_ROCK_PILLAR_IN_HOLE))
			fdActorSetPos(ACTOR_LEDGE_PILLAR, 520, 584);
		x = syGameFlagGetByte(FLAG_PILLAR_COLUMN);
		z = syGameFlagGetByte(FLAG_PILLAR_ROW);
		if(z != 0)
			fdActorSetHorizontalPosWithTerrain(ACTOR_MOAI_PILLAR,
				FIELD_METATILE_CENTER(x), FIELD_METATILE_CENTER(z));
		fdActorGetById(fdPartyGetLeaderActor())->priFlg |=
			ACTOR_PRIORITY_IGNORES_RISES;
		sySetEvent(scAquaRockKeepLeaderPriority, EVENT_PASS_TICK);
		fdActorSetSprPriority(ACTOR_FIRST_FISH + 1, 1);
		fdActorSetSprPriority(ACTOR_MOAI_PILLAR, 1);
		scAquaRockStartBubbles();
		fdGroundParticlesStart();
		if(syGameFlagGet(FLAG_AQUA_ROCK_STREAMS_TURNED_BACK)) {
			scAquaRockSetBubbleAnchor(ACTOR_WATER_STONE);
			scGroundParticlesSetAnchor(ACTOR_WATER_STONE);
			fdTilesetAnimPause(STREAMS_DOWNHILL_ANIMATION);
			fdTilesetAnimPause(STREAMS_DOWNHILL_ANIMATION + 1);
			fdTilesetAnimResume(STREAMS_UPHILL_ANIMATION);
			fdTilesetAnimResume(STREAMS_UPHILL_ANIMATION + 1);
			fdActorGetById(ACTOR_WATER_STONE)->update = scAquaRockStopFlashing;
		}
		fdActorSetSprPriority(ACTOR_WATER_STONE, 1);
		if(MainRoutineStateWork.entrance == AQUA_ROCK_MOAI_FROM_WORLD_MAP_NORTH)
			scAquaRockMoaiStepAside();
		if(syGameFlagGet(FLAG_AQUA_ROCK_MOAI_STEPPED_ASIDE)) {
			fdActorSetPos(ACTOR_FIRST_GUARDIAN_MOAI, 392, 824);
			fdActorSetPos(ACTOR_FIRST_GUARDIAN_MOAI + 1, 424, 824);
			fdActorSetPos(ACTOR_FIRST_GUARDIAN_MOAI + 2, 456, 824);
			fdActorSetPos(ACTOR_FIRST_GUARDIAN_MOAI + 3, 488, 824);
			fdActorSetPos(ACTOR_FIRST_GUARDIAN_MOAI + 4, 520, 824);
			fdMapGridCopyMetatileAttributes(64, 66, 9, 3, 24, 53);
			fdMapGridCopyMetatiles(64, 66, 11, 7, 87, 50);
			fdActorGetById(ACTOR_FIRST_GUARDIAN_MOAI + 4)->update =
				scAquaRockRockMoai;
		}
		if(syGameFlagGet(FLAG_AQUA_ROCK_DRY_MOAI_FILLED)) {
			fdMapGridCopyMetatileAttributes(62, 41, 1, 2, 33, 41);
			fdMapGridCopyMetatileTilesMetatilesRect(116, 95, 3, 13, 96, 32);
			fdMapGridCopyMetatilesLayered(122, 95, 3, 13, 32, 96);
			scAquaRockAddMoaiWater(ACTOR_DRY_MOAI);
		}
		scAquaRockSetUpStreams();
		if(MainRoutineStateWork.entrance ==
			AQUA_ROCK_MOAI_FROM_AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST)
			scAquaRockArriveFromPassage();
	} else if(scene == SCENE_AQUA_ROCK_MIST_SABRE_CHEST) {
		fdActorSetSprFlags(fdActorGetById(ACTOR_BUBBLE_MARKER), 0);
		if(syGameFlagGet(FLAG_PUDDLE_A_FROZEN))
			fdMapGridCopyMetatileAttributes(42, 32, 1, 1, 44, 32);
		if(syGameFlagGet(FLAG_PUDDLE_B_FROZEN))
			fdMapGridCopyMetatileAttributes(48, 32, 1, 1, 46, 32);
		if(syGameFlagGet(FLAG_PUDDLE_C_FROZEN))
			fdMapGridCopyMetatileAttributes(44, 44, 1, 1, 46, 44);
		fdActorSetAnimationById(ACTOR_FIRST_ROCK, 2);
		if(syGameFlagGet(FLAG_AQUA_ROCK_FIRST_ROCK_DROPPED)) {
			fdActorSetPos(ACTOR_FIRST_ROCK, 856, 840);
			fdMapGridCopyMetatileAttributes(54, 58, 1, 1, 53, 52);
		}
		fdActorSetAnimationById(ACTOR_SECOND_ROCK, 2);
		if(syGameFlagGet(FLAG_AQUA_ROCK_SECOND_ROCK_DROPPED)) {
			fdActorSetPos(ACTOR_SECOND_ROCK, 856, 936);
			fdMapGridCopyMetatileAttributes(54, 58, 1, 1, 53, 58);
		}
		/* Arriving in the east half from the west. The map has no entrance 7;
		   the east half's other entrance, from the Moai's spring
		   (AQUA_ROCK_MIST_SABRE_CHEST_FROM_AQUA_ROCK_MOAI_C), gets the west
		   half's set-up. */
		if(MainRoutineStateWork.entrance == WAY_MIST_SABRE_CHEST_EAST_TO_WEST ||
			MainRoutineStateWork.entrance == 7) {
			scAquaRockStartBubbles();
			fdGroundParticlesStart();
			if(syGameFlagGet(FLAG_AQUA_ROCK_STREAMS_TURNED_BACK)) {
				scAquaRockSetBubbleAnchor(ACTOR_BUBBLE_MARKER);
				scGroundParticlesSetAnchor(ACTOR_BUBBLE_MARKER);
			} else {
				fdTilesetAnimPause(STREAMS_UPHILL_ANIMATION);
				fdTilesetAnimPause(STREAMS_UPHILL_ANIMATION + 1);
				fdTilesetAnimResume(STREAMS_DOWNHILL_ANIMATION);
				fdTilesetAnimResume(STREAMS_DOWNHILL_ANIMATION + 1);
			}
			fdActorSetSprFlags(fdActorGetById(ACTOR_BUBBLE_MARKER), 0);
		} else {
			scAquaRockStartBubbles();
			fdGroundParticlesStart();
			fdActorGetById(ACTOR_MIST_SABRE_WATER_STONE)->update =
				scAquaRockStopFlashing;
			if(syGameFlagGet(
				   FLAG_AQUA_ROCK_MIST_SABRE_CHEST_WATER_STONE_DOUSED)) {
				scAquaRockSetBubbleAnchor(ACTOR_MIST_SABRE_WATER_STONE);
				scGroundParticlesSetAnchor(ACTOR_MIST_SABRE_WATER_STONE);
				fdActorSetPos(ACTOR_WAY_INSIDE, 264, 792);
				fdActorSetSprFlags(fdActorGetById(ACTOR_WAY_INSIDE), 0);
				fdActorGetById(ACTOR_WAY_INSIDE)->collisionFlg = 0;
				fdActorGetById(ACTOR_WAY_INSIDE)->priFlg |=
					ACTOR_PRIORITY_UNDERFOOT;
				fdActorGetById(ACTOR_WAY_INSIDE)->update = scActorEmitRipples;
				fdMapCopyMetatileAttributeRect(15, 48, 1, 1, 16, 48);
				fdMapCopyMetatileAttributeRect(17, 50, 1, 1, 16, 50);
				fdMapGridCopyMetatileTilesMetatilesRect(64, 68, 5, 13, 14, 94);
				fdMapCopyMetatileAttributeRect(64, 68, 5, 8, 14, 34);
			}
		}
		if(!syGameFlagGet(FLAG_KEEP_PARTY_POSITION)) {
			if(MainRoutineStateWork.entrance ==
				AQUA_ROCK_MIST_SABRE_CHEST_FROM_AQUA_ROCK_INSIDE_VIAL_CHEST)
				scAquaRockArriveFromInside();
			entrance = &MainRoutineStateWork.entrance;
			if(*entrance == AQUA_ROCK_MIST_SABRE_CHEST_FROM_AQUA_ROCK_MOAI_C)
				scAquaRockArriveUpStream();
		}
	}
	return 0;
}

Sint32 scPrepareMap(void)
{
	return 0;
}

#include "shared/actor_spawn_wading_splash.h"

#include "shared/actor_emit_ripples.h"

#include "shared/leader_flow_with_stream.h"

/* Gives a moai its water: the pool in the top of its head, pouring from its
   mouth. */
void scAquaRockAddMoaiWater(Sint32 id)
{
	ACTOR *actor = fdActorGetById(id);

	if(actor != 0) {
		SPR_ANIMATION *water =
			(SPR_ANIMATION *)fdSprAddAnimation(actor->spr, SPRITE_MOAI_WATER);

		water->colMode = SPRITE_COLOR_NORMAL;
		water->pri = 0;
		fdActorSelectAnimation(actor, 0);
		fdActorSelectAnimation(actor, 2);
	}
}

/* Once the Water Stone is Doused, stepping on the way inside: the leader
   tumbles and spins down into the pool and leaves by `exit`, to the Vial
   chest's room inside. */
void scAquaRockSinkInside(Sint32 exit)
{
	ACTOR *actor = fdActorGetById(MainRoutineStateWork.leaderActorId);
	Sint32 i;
	Sint32 ang;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdActorSetSprPriority(MainRoutineStateWork.leaderActorId, 1);
	actor->motionFlg = 0;
	actor->priFlg |= ACTOR_PRIORITY_IGNORES_RISES;
	sySndPlay(SE_SPLASH_SMALL);
	fdActorSelectAnimation(actor, ANIM_LEADER_TUMBLE_IN_WATER);
	sySndPlay(SE_SPIN_AWAY);
	for(i = 0; i < 60; i++) {
		if(i == 30)
			fdScriptFinishScreenTransition();
		actor->y.fixed -= toFIXED(1) / 4;
		ang = actor->facing;
		ang += TRIG_TURN / 16;
		actor->facing = ang;
		if((i & 7) == 0)
			scActorSpawnWadingSplash(fdActorGetById(ACTOR_WAY_INSIDE));
		sySynch(1);
	}
	fdScriptEnd();
	syImportScriptSetExitRequest(exit);
}

#include "shared/actor_emit_mist.h"

/* The leader walks into the sparkling passage in a mist and leaves by
   `entrance`. */
void scAquaRockEnterSparklingPassage(Sint32 entrance)
{
	ACTOR  *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	Sint32 *owner = &MainRoutineStateWork.leaderActorId;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	sySndPlay(SE_SPARKLING_PASSAGE);
	leader->update = (OBJECT_UPDATE)scActorEmitMist;
	leader->motionFlg = 0;
	leader->speedLimit = toFIXED(1) / 5;
	fdActorSetAnimationById(*owner, 2);
	fdActorSetDestinationOffsetById(*owner, 0, -8);
	fdActorWaitForMoveById(*owner);
	fdActorSetSprCol(fdActorGetById(*owner), SPRITE_COLOR_BLUE_TINT);
	fdActorSetSprFlags(fdActorGetById(*owner), 0);
	leader->update = 0;
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	syImportScriptSetExitRequest(entrance);
	fdScriptEnd();
}

/* Arriving from the sparkling passage, unless the party's place is kept: the
   leader walks out of the mist. */
void scAquaRockArriveFromPassage(void)
{
	ACTOR  *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	Sint32	flg = syGameFlagGet(FLAG_KEEP_PARTY_POSITION);
	Sint32 *owner = &MainRoutineStateWork.leaderActorId;

	if(flg == 0) {
		fdScriptStart();
		fdScriptEndLastingPsynergy(0);
		fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
		leader->motionFlg = flg;
		fdActorSetHorizontalPosWithTerrain(*owner,
			toFIXED(leader->x.part[0].integer),
			toFIXED(leader->z.part[0].integer) - toFIXED(16));
		fdActorSetSprCol(fdActorGetById(*owner), SPRITE_COLOR_BLUE_TINT);
		fdActorSetSprFlags(fdActorGetById(*owner), 0);
		fdScriptStartScreenTransition();
		sySndPlay(SE_SPARKLING_PASSAGE);
		leader->update = (OBJECT_UPDATE)scActorEmitMist;
		fdActorSetSpeed(*owner, 0.4);
		fdActorWalkByAndWaitById(*owner, 0, 8);
		fdActorSetSprCol(fdActorGetById(*owner), SPRITE_COLOR_NORMAL);
		fdActorSetSprFlags(fdActorGetById(*owner), SPRITE_FLAG_SHADOW);
		((FIELD_SPR *)leader->spr)->pri = 1;
		fdActorWalkByAndWaitById(*owner, 0, 10);
		leader->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
		leader->update = (OBJECT_UPDATE)flg;
		fdScriptNoOperation();
		fdScriptWaitTransitionTicks();
		fdScriptEnd();
	}
}

/* Each tick, for bubbles going up: now and then one starts from the actor they
   come from, and each rises up the stream, north, until it reaches the top;
   those in view are drawn. */
void scAquaRockDrawRisingBubbles(void)
{
	MAP_STATE		  *map = Ram_HeapSlots->mapGrid;
	Sint32			   viewX = syMapStateView(map)[0] & FIELD_PIXEL_MASK;
	Sint32			   viewZ = syMapStateView(map)[1] & FIELD_PIXEL_MASK;
	Sint32			   lift = map->origin[1];
	AQUA_ROCK_BUBBLES *state = &AquaRockBubbles;
	Uint32			   tile = VramCacheEntries[state->vramEntry].ofs >> 5;
	ACTOR			  *src;
	BUBBLE			  *entry;
	Sint32			   i, x, y, z, screenX, screenY;

	if(syRandom() < 4096 && (src = state->anchor) != 0) {
		entry = &state->bubbles[state->next];
		entry->x = src->x.fixed + syRandom() * 3 * 16 - toFIXED(24);
		entry->y = src->y.fixed;
		entry->z = src->z.fixed;
		entry->age = 0;
		if(++state->next > STREAM_BUBBLE_COUNT - 1)
			state->next = 0;
	}
	entry = state->bubbles;
	for(i = 0; i < STREAM_BUBBLE_COUNT; i++, entry++) {
		if(entry->x != 0) {
			/* screenX holds the ground's height here, before its own value. */
			screenX = fdMapGetTerrainHeight(0, entry->x, entry->z);
			if(screenX > entry->y)
				entry->y += toFIXED(2);
			else
				entry->z -= toFIXED(2);
			if(FIELD_METATILE(entry->y) == 23)
				entry->x = 0;
			if(FIELD_METATILE(entry->z) == 48)
				entry->x = 0;
			entry->age++;
			x = entry->x - viewX;
			y = entry->y;
			z = entry->z;
			y -= lift;
			z -= viewZ;
			z -= lift;
			screenY = z - y;
			y += z;
			y = FIXEDtoINT(y);
			screenX = FIXEDtoINT(x);
			x = screenX - 4;
			screenY = FIXEDtoINT(screenY);
			screenY -= 4;
			z = y + 58;
			/* screenX now holds its offset into the range test (on screen from
			   -11 to 323). */
			screenX += 11;
			if((Uint32)screenX <= 334 && screenY > -16 && screenY <= 239) {
				x &= OBJ_X_MASK;
				screenY &= OBJ_Y_MASK;
				entry->oam[0] = 0;
				entry->oam[1] = screenY | OBJ_X(x) | OBJ_256_COLORS;
				entry->oam[2] = (tile + (entry->age & 3) * 2) | OBJ_PRIORITY(1);
				syOAMAddSprAtDepth(entry->oam, z);
			}
		}
	}
}

/* Each tick, for bubbles going down: now and then one starts from the actor
   they come from, and each falls down the stream, south, until it reaches the
   bottom; those in view are drawn. */
void scAquaRockDrawFallingBubbles(void)
{
	MAP_STATE		  *map = Ram_HeapSlots->mapGrid;
	Sint32			   viewX = syMapStateView(map)[0] & FIELD_PIXEL_MASK;
	Sint32			   viewZ = syMapStateView(map)[1] & FIELD_PIXEL_MASK;
	Sint32			   lift = map->origin[1];
	AQUA_ROCK_BUBBLES *state = &AquaRockBubbles;
	Uint32			   tile = VramCacheEntries[state->vramEntry].ofs >> 5;
	ACTOR			  *src;
	BUBBLE			  *bubble;
	Sint32			   i, x, y, depth, screenX, screenY, z, row;

	if(syRandom() < 4096 && (src = state->anchor) != 0) {
		bubble = &state->bubbles[state->next];
		bubble->x = src->x.fixed + syRandom() * 3 * 8 - toFIXED(12);
		bubble->y = src->y.fixed;
		bubble->z = src->z.fixed;
		bubble->age = 0;
		if(++state->next > STREAM_BUBBLE_COUNT - 1)
			state->next = 0;
	}
	bubble = state->bubbles;
	for(i = 0; i < STREAM_BUBBLE_COUNT; i++, bubble++) {
		if(bubble->x != 0) {
			if(fdMapGetTerrainHeight(0, bubble->x, bubble->z) < bubble->y)
				bubble->y -= toFIXED(2);
			else
				bubble->z += toFIXED(2);
			if(FIELD_METATILE(bubble->y) == -4)
				bubble->x = 0;
			if(FIELD_METATILE(bubble->z) == 50)
				bubble->x = 0;
			bubble->age++;
			z = bubble->z;
			x = bubble->x - viewX;
			y = bubble->y - lift;
			z -= viewZ;
			z -= lift;
			row = z - y;
			screenX = FIXEDtoINT(x);
			y += z;
			x = screenX - 4;
			screenY = FIXEDtoINT(row) - 4;
			y = FIXEDtoINT(y);
			depth = y + 58;
			if((Uint32)(screenX + 11) <= 334 && screenY > -16 &&
				screenY <= 239) {
				x &= OBJ_X_MASK;
				screenY &= OBJ_Y_MASK;
				bubble->oam[0] = 0;
				bubble->oam[1] = screenY | OBJ_X(x) | OBJ_256_COLORS;
				bubble->oam[2] =
					(tile + (bubble->age & 3) * 2) | OBJ_PRIORITY(1);
				syOAMAddSprAtDepth(bubble->oam, depth);
			}
		}
	}
}

/* Each tick: the bubbles go up or down the stream, as their direction says. */
void scAquaRockDrawBubbles(void)
{
	if(AquaRockBubbles.dir > 0)
		scAquaRockDrawRisingBubbles();
	else
		scAquaRockDrawFallingBubbles();
}

/* Clears the bubbles, loads their tiles into VRAM and starts their drawing;
   returns the drawing event (sySetEvent). */
Sint32 scAquaRockStartBubbles(void)
{
	AQUA_ROCK_BUBBLES *state = &AquaRockBubbles;
	void			  *buf;

	syWordsClearEntry(state, sizeof(AQUA_ROCK_BUBBLES));
	buf = syHeapCallAlternatePool(syHeapAlloc, 256);
	syDecompressTaggedLZ(AquaRock_BubbleTiles, buf);
	state->vramEntry = syVRAMCacheFindFreeEntry();
	syVRAMCacheLoad(state->vramEntry, 256, buf);
	syHeapRelease(buf);
	return sySetEvent(scAquaRockDrawBubbles, EVENT_PASS_TICK);
}

/* Sets the actor the bubbles come from, or with 0 none: they fall from the Mist
   Sabre chest's room's Water Stone and rise from any other. */
void scAquaRockSetBubbleAnchor(Sint32 id)
{
	AQUA_ROCK_BUBBLES *state = &AquaRockBubbles;

	if(id == 0) {
		state->anchor = 0;
	} else {
		state->anchor = fdActorGetById(id);
		if(id == ACTOR_MIST_SABRE_WATER_STONE)
			state->dir = -1;
		else
			state->dir = 1;
	}
}

#include "shared/ground_particles_draw.h"

#include "shared/ground_particles_start.h"

#include "shared/ground_particles_set_anchor.h"

/* clang-format off */
#include "shared/field_effect_duration_scripts.c"

/* A fish's script: it swims to random spots, resting between. */
Sint32 AquaRockFishScript[] = {
	SC_CALL((Sint32)scAquaRockStartFish),
	SC_LABEL(1),
	SC_ANIMATION(2),
	SC_CALL((Sint32)scAquaRockPickRandomSpot),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_WAIT(120),
	SC_ANIMATION(2),
	SC_CALL((Sint32)scAquaRockPickRandomSpot),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_WAIT(20),
	SC_ANIMATION(2),
	SC_CALL((Sint32)scAquaRockPickRandomSpot),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_WAIT(180),
	SC_LOOP(SC_FOREVER, SC_TO(1)),
	SC_END,
};

/* A fish's script: it swims to random spots, resting longer between. */
Sint32 AquaRockSlowFishScript[] = {
	SC_CALL((Sint32)scAquaRockStartFish),
	SC_LABEL(1),
	SC_WAIT(60),
	SC_ANIMATION(2),
	SC_CALL((Sint32)scAquaRockPickRandomSpot),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_WAIT(120),
	SC_ANIMATION(2),
	SC_CALL((Sint32)scAquaRockPickRandomSpot),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_WAIT(15),
	SC_ANIMATION(2),
	SC_CALL((Sint32)scAquaRockPickRandomSpot),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_WAIT(180),
	SC_LOOP(SC_FOREVER, SC_TO(1)),
	SC_END,
};

/* A splash's script: it shows for 20 ticks, then is released. */
Uint32 WadingSplashScript[] = {
	SC_WAIT(20),
	SC_RELEASE,
};

#include "shared/ripple_script.c"
/* clang-format on */

#undef DEFINE_AQUA_ROCK_FREEZE_PUDDLE
#undef DEFINE_AQUA_ROCK_DROP_AT_COLUMN
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef EFFECT_SPRITE_PRIORITY

/* clang-format off */
#define GROUND_PARTICLE_FRAMES_SECTION LATE_DATA
#include "shared/ground_particle_frames.c"

LATE_DATA const Sint32 *FieldEffectDurationScripts[] = {
	FieldEffectShortScript,
	FieldEffectMediumScript,
	FieldEffectLongScript,
};

#define SCENE_ENTRANCES_SECTION LATE_DATA
#include "shared/scene_entrances_default.c"

/* The slopes' exit regions: the two ways up into the Mist Sabre chest's */
/* room. */
LATE_DATA SCENE_REGION AquaRockSlopesRegions[] = {
	{176, 424, 544, 192, 440, 560, CONDITION_ALWAYS,
	 AQUA_ROCK_MOAI_TO_AQUA_ROCK_MIST_SABRE_CHEST_A},
	{272, 424, 544, 288, 440, 560, CONDITION_ALWAYS,
	 AQUA_ROCK_MOAI_TO_AQUA_ROCK_MIST_SABRE_CHEST_B},
	{SCENE_TABLE_END},
};

/* The Mist Sabre chest's room's exit regions: the ways back down to the */
/* slopes. */
LATE_DATA SCENE_REGION AquaRockMistSabreChestRegions[] = {
	{48, -56, 960, 64, -40, 976, CONDITION_ALWAYS,
	 AQUA_ROCK_MIST_SABRE_CHEST_TO_AQUA_ROCK_MOAI_A},
	{144, -56, 960, 160, -40, 976, CONDITION_ALWAYS,
	 AQUA_ROCK_MIST_SABRE_CHEST_TO_AQUA_ROCK_MOAI_B},
	{236, -108, 956, 260, -84, 980, CONDITION_ALWAYS,
	 AQUA_ROCK_MIST_SABRE_CHEST_TO_AQUA_ROCK_MOAI_D},
	{268, -108, 956, 292, -84, 980, CONDITION_ALWAYS,
	 AQUA_ROCK_MIST_SABRE_CHEST_TO_AQUA_ROCK_MOAI_D},
	{SCENE_TABLE_END},
};

LATE_DATA Uint32 SceneExits[] = {
	SCENE_EXITS(AquaRockMoai),
	SCENE_EXIT(AQUA_ROCK_MOAI_TO_WORLD_MAP_SOUTH, WorldMap, WORLD_MAP_FROM_AQUA_ROCK_MOAI_SOUTH),
	SCENE_EXIT(AQUA_ROCK_MOAI_TO_AQUA_ROCK_MIST_SABRE_CHEST_A, AquaRockMistSabreChest, AQUA_ROCK_MIST_SABRE_CHEST_FROM_AQUA_ROCK_MOAI_A),
	SCENE_EXIT(AQUA_ROCK_MOAI_TO_AQUA_ROCK_MIST_SABRE_CHEST_B, AquaRockMistSabreChest, AQUA_ROCK_MIST_SABRE_CHEST_FROM_AQUA_ROCK_MOAI_B),
	SCENE_EXIT(AQUA_ROCK_MOAI_TO_AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST, AquaRockInsideRustySwordChest, AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST_FROM_AQUA_ROCK_MOAI),
	SCENE_EXIT(AQUA_ROCK_MOAI_TO_AQUA_ROCK_MIST_SABRE_CHEST_C, AquaRockMistSabreChest, AQUA_ROCK_MIST_SABRE_CHEST_FROM_AQUA_ROCK_MOAI_C),
	SCENE_EXIT(AQUA_ROCK_MOAI_TO_APOJII_ISLANDS, ApojiiIslands, APOJII_ISLANDS_FROM_WORLD_MAP_A),
	SCENE_EXITS(AquaRockMistSabreChest),
	SCENE_EXIT(AQUA_ROCK_MIST_SABRE_CHEST_TO_AQUA_ROCK_INSIDE_VIAL_CHEST, AquaRockInsideVialChest, AQUA_ROCK_INSIDE_VIAL_CHEST_FROM_AQUA_ROCK_MIST_SABRE_CHEST),
	SCENE_EXIT(AQUA_ROCK_MIST_SABRE_CHEST_TO_AQUA_ROCK_MOAI_A, AquaRockMoai, AQUA_ROCK_MOAI_FROM_AQUA_ROCK_MIST_SABRE_CHEST_A),
	SCENE_EXIT(AQUA_ROCK_MIST_SABRE_CHEST_TO_AQUA_ROCK_MOAI_B, AquaRockMoai, AQUA_ROCK_MOAI_FROM_AQUA_ROCK_MIST_SABRE_CHEST_B),
	SCENE_EXIT(WAY_MIST_SABRE_CHEST_WEST_TO_EAST, AquaRockMistSabreChest, WAY_MIST_SABRE_CHEST_EAST_TO_WEST),
	SCENE_EXIT(WAY_MIST_SABRE_CHEST_EAST_TO_WEST, AquaRockMistSabreChest, WAY_MIST_SABRE_CHEST_WEST_TO_EAST),
	SCENE_EXIT(AQUA_ROCK_MIST_SABRE_CHEST_TO_AQUA_ROCK_MOAI_D, AquaRockMoai, AQUA_ROCK_MOAI_FROM_AQUA_ROCK_MIST_SABRE_CHEST_D),
	SCENE_EXITS_END,
};

/* The scene's other rooms' actors. */
LATE_DATA ACTOR_PLACE AquaRockPlacements[] = {
	{SCENE_TABLE_END},
};

/* The slopes' actors, in AQUA_ROCK_SLOPES_ACTOR_ID's order. */
LATE_DATA ACTOR_PLACE AquaRockSlopesPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_MOAI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 551, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_MOAI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 551, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_MOAI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 648, 0, 615, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_MOAI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 712, 0, 615, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_MOAI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 647, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_MOAI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 647, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_MOAI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 56, 0, 679, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_MOAI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 823, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_MOAI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 927, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_MOAI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 879, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_MOAI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 424, 0, 863, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_MOAI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 847, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_MOAI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 863, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_MOAI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 879, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_MOAI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 712, 0, 847, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_MOAI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 840, 0, 959, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 552, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 584, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WATER_DROP_LARGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 840, 0, 600, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FISH, CONDITION_ALWAYS, (Sint32)AquaRockFishScript, 376, 0, 960, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FISH, CONDITION_ALWAYS, (Sint32)AquaRockFishScript, 136, 0, 904, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FISH, CONDITION_ALWAYS, (Sint32)AquaRockSlowFishScript, 144, 0, 912, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

/* The Mist Sabre chest's room's actors, in */
/* AQUA_ROCK_MIST_SABRE_CHEST_ACTOR_ID's order. */
LATE_DATA ACTOR_PLACE AquaRockMistSabreChestPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_WATER_DROP_LARGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 584, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 712, 0, 520, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 744, 0, 520, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 744, 0, 712, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_EMBER_STONE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 840, 0, 840, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_EMBER_STONE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 824, 0, 936, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 712, 0, 936, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

/* The tick counts of the four red moai's streams' cycles */
/* (scAquaRockTimeStreams). */
LATE_DATA Sint32 AquaRockFirstStreamTimer = 0;
LATE_DATA Sint32 AquaRockSecondStreamTimer = 0;
LATE_DATA Sint32 AquaRockThirdStreamTimer = 0;
LATE_DATA Sint32 AquaRockFourthStreamTimer = 0;

/* The scene's other rooms' cues. */
LATE_DATA CUE AquaRockCues[] = {
	{CUE_TABLE_END},
};

/* The slopes' exits, currents and streams, Move on the pillars and Douse on */
/* the dry moai and the Water Stone. */
LATE_DATA CUE AquaRockSlopesCues[] = {
	{CUE_EXIT, 1, CONDITION_ALWAYS, AQUA_ROCK_MOAI_TO_WORLD_MAP_SOUTH},
	{STEP_CUE_FACING(FACING_NORTH), METATILE_BEHAVIOR_SPARKLING_PASSAGE, CONDITION_ALWAYS, SCRIPT_REF(scAquaRockEnterSparklingPassage)},
	{CUE_STEP, METATILE_BEHAVIOR_WEST_MOAI_STREAM, FLAG_PILLAR_BEFORE_WEST_MOAI, SCRIPT_REF(scAquaRockStepIntoCurrent)},
	{CUE_STEP, METATILE_BEHAVIOR_EAST_MOAI_STREAM, FLAG_PILLAR_BEFORE_EAST_MOAI, SCRIPT_REF(scAquaRockStepIntoCurrent)},
	{CUE_STEP, METATILE_BEHAVIOR_LOWER_WEST_MOAI_STREAM, CONDITION_ALWAYS, SCRIPT_REF(scAquaRockStepIntoCurrent)},
	{CUE_STEP, METATILE_BEHAVIOR_LOWER_EAST_MOAI_STREAM, CONDITION_ALWAYS, SCRIPT_REF(scAquaRockStepIntoCurrent)},
	{CUE_STEP, METATILE_BEHAVIOR_DOWNHILL_STREAM, CONDITION_ALWAYS, SCRIPT_REF(scLeaderFlowWithStream)},
	{CUE_STEP, METATILE_BEHAVIOR_TURNING_STREAM, CONDITION_ALWAYS, SCRIPT_REF(scAquaRockRideStreamUp)},
	{CUE_RAISED, CUE_DROP_DOWN_FALLS, CONDITION_ALWAYS, SCRIPT_REF(scAquaRockDropDownFalls)},
	{PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_DRY_MOAI, FLAG_AQUA_ROCK_DRY_MOAI_FILLED, SCRIPT_REF(scAquaRockDouseDryMoai)},
	{PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_WATER_STONE, FLAG_AQUA_ROCK_STREAMS_TURNED_BACK, SCRIPT_REF(scAquaRockDouseWaterStone)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_MOAI_PILLAR, CONDITION_ALWAYS, SCRIPT_REF(scAquaRockMovePillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LEDGE_PILLAR, FLAG_AQUA_ROCK_PILLAR_IN_HOLE, SCRIPT_REF(scAquaRockMovePillar)},
	{CUE_PUSHED, 0, CONDITION_ALWAYS, SCRIPT_REF(scAquaRockMovePillar)},
	{CUE_TABLE_END},
};

/* The Mist Sabre chest's room's way inside, exits, streams, puddles to */
/* Frost, rocks to drop and Water Stone to Douse. */
LATE_DATA CUE AquaRockMistSabreChestCues[] = {
	{CUE_STEP, AQUA_ROCK_MIST_SABRE_CHEST_TO_AQUA_ROCK_INSIDE_VIAL_CHEST, CONDITION_ALWAYS, SCRIPT_REF(scAquaRockSinkInside)},
	{CUE_EXIT, 2, CONDITION_ALWAYS, AQUA_ROCK_MIST_SABRE_CHEST_TO_AQUA_ROCK_MOAI_A},
	{CUE_EXIT, 3, CONDITION_ALWAYS, AQUA_ROCK_MIST_SABRE_CHEST_TO_AQUA_ROCK_MOAI_B},
	{CUE_EXIT, 4, CONDITION_ALWAYS, 4},
	{CUE_EXIT, 5, CONDITION_ALWAYS, 5},
	{CUE_STEP, METATILE_BEHAVIOR_DOWNHILL_STREAM, CONDITION_ALWAYS, SCRIPT_REF(scLeaderFlowWithStream)},
	{CUE_STEP, METATILE_BEHAVIOR_TURNING_STREAM, CONDITION_ALWAYS, SCRIPT_REF(scAquaRockRideStreamUp)},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PUDDLE_A, FLAG_PUDDLE_A_FROZEN, SCRIPT_REF(AquaRock_FreezeFirstPuddle)},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PUDDLE_B, FLAG_PUDDLE_B_FROZEN, SCRIPT_REF(AquaRock_FreezeSecondPuddle)},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PUDDLE_C, FLAG_PUDDLE_C_FROZEN, SCRIPT_REF(AquaRock_FreezeThirdPuddle)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_ROCK, FLAG_AQUA_ROCK_FIRST_ROCK_DROPPED, SCRIPT_REF(AquaRock_DropFirstRock)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SECOND_ROCK, FLAG_AQUA_ROCK_SECOND_ROCK_DROPPED, SCRIPT_REF(AquaRock_DropSecondRock)},
	{CUE_PUSHED, 0, FLAG_AQUA_ROCK_SECOND_ROCK_DROPPED, SCRIPT_REF(AquaRock_DropSecondRock)},
	{PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_MIST_SABRE_WATER_STONE, FLAG_AQUA_ROCK_MIST_SABRE_CHEST_WATER_STONE_DOUSED, SCRIPT_REF(scAquaRockDouseMistSabreChestWaterStone)},
	{CUE_TABLE_END},
};

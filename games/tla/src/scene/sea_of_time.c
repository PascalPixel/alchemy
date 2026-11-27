/* @file */
/* The Sea of Time, the purple sea the Lemurian ship must cross to reach */
/* Lemuria, in two scenes: the great sea of swirls the ship sails into from */
/* the world map, and the last stretch before Lemuria, where Poseidon waits. */
/* */
/* The great sea is full of swirls, each a ring of current round a dark eye */
/* that turns the ship round and round, and of rapids, straight channels of */
/* striped water that carry the ship along. Some run off the edge of the sea */
/* and sweep the ship out of it: the first time back aboard the Lemurian ship,
 */
/* where the party talks over how to find the way, later straight out to the */
/* world map. The way is in Yepp's songs, which the children of Yallam sing: */
/* pass so many swirls, then sail round a trunk, one of the little volcanoes */
/* in the sea, two or three times the same way round until stars rise about */
/* it. Then come the waves: the sea's patterns go still, and until the waves */
/* stop the channels carry nothing. Bands of rough water in the north turn the
 */
/* ship back south. The sea's north-west corner leads to the last stretch, */
/* where Poseidon rises from the sea. Once he is beaten, four rocks rise out */
/* of the water for Grind, and the way north to Lemuria is open. */

#include "graphics/palette_buffers.h"
#include "system/field_effect.h"
#include "field/actor_script.h"
#include "../field/actor_set_sprite_priority.c"

#include "../field/actor_create_silhouette.c"
#include "macros.h"
#include "field/event_inline.h"
#include "scene/scene.h"
#include "game/game.h"

extern volatile u32 SeaOfTime_WaveStartFrame;
extern u32 SeaOfTime_WaveState;
/* The scene's view of SceneStateWork (system/globals.h): its words. */
#define SceneStateWork ((u32 *)SceneStateWork)
extern MAP_CELL MapGridCellBuffer[];

void scSetActorSprPriority(ACTOR *actor, s32 priority);

void scFieldEffectSpawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
						s32 velocityZ, u32 flags,
						const EFFECT_OPTIONS *options);
s32 fdMapCellGetAttributes(u32 layer, s32 x, s32 y);
s32 scMapCellSetAttributes(u32 layer, s32 x, s32 y, s32 value);
s32 scPosFlatDistance(FIXED *a, FIXED *b);
void scSeaOfTimeRemoveRock(s32 x, s32 z, s32 actor);
void scSeaOfTimePlaceRock(s32 x, s32 z, s32 actor);

/* syCos, through an inline as the swirl's turn calls it. */
static inline s32 scSeaOfTimeCos(s32 angle) { return syCos(angle); }

#include "shared/scene_create_imported_actor.h"

#include "../field/field_effect_move.c"

#define EFFECT_SET_MODE	  fdActorSelectAnimation
#define EFFECT_SET_SCRIPT fdActorStartScript
#define EFFECT_SPRITE_PRIORITY
#include "../field/field_effect_spawn.c"

extern ACTOR_PLACE SeaOfTime_ToLemuriaPlacements[];
extern u32 SeaOfTime_LastLapFrame;

void scSeaOfTimeTimeWaves(void);

extern TRIGGER SeaOfTime_SwirlsTriggers[];

extern TRIGGER SeaOfTime_NorthPartTriggers[];
extern TRIGGER SeaOfTime_SouthPartTriggers[];
extern s16 SeaOfTime_RedPulseStep[1];
void scSeaOfTimeRideCurrent(void);

void scSeaOfTimeCountLaps(void);

extern u32 FieldEffect_ShortScript[];
extern u32 FieldEffect_MediumScript[];
extern u32 FieldEffect_LongScript[];
extern s32 SeaOfTime_ShipRests[];

/* The sea's horizontal offset for each screen line as it ripples sideways */
/* (scSeaOfTimeAnimateSea). */
u16 SeaOfTime_RippleLines[DISPLAY_HEIGHT] BSS_DATA ALIGNED(16);

#include "shared/ship_drop_shadow.h"

#include "../field/map_cell_get.c"

#include "shared/map_cell_set.c"

#include "shared/position_flat_distance.h"

const SCENE_ENTRANCE *scGetEntrances(void) { return SceneEntrances; }

const SCENE_REGION *scGetExitRegions(void) { return 0; }

const u32 *scGetExits(void) { return SceneExits; }

extern ACTOR_PLACE SeaOfTime_SwirlsPlacements[];

/* The Lemurian ship the party sails, and the rocks that rise in the last */
/* stretch. The ship's row places it blank, and it wears the ship's sprite */
/* sheet (`SPRITE_WINGED_SHIP`) on the water. */
enum {
	ACTOR_SHIP = ACTOR_FIRST_PLACED,
	/* The grey rocks that rise out of the last stretch's north part once */
	/* Poseidon is beaten, for Grind to clear: A on cells (10, 11) to (11, 12),
	 */
	/* B on (18, 8) to (19, 9), D on (23, 13) to (24, 14) and E on (29, 9) to */
	/* (30, 10). Rock C is never placed. In the south part, rock A's row stands
	 */
	/* for one more rock, on (44, 42) to (45, 43), under its own flag. */
	ACTOR_ROCK_A,
	ACTOR_ROCK_B,
	ACTOR_ROCK_C,
	ACTOR_ROCK_D,
	ACTOR_ROCK_E
};

/* Set as Grind clears each rock; a cleared rock is not placed again. */
enum {
	FLAG_ROCK_A_CLEARED = 0xa90,
	FLAG_ROCK_B_CLEARED,
	FLAG_ROCK_C_CLEARED,
	FLAG_ROCK_D_CLEARED,
	FLAG_ROCK_E_CLEARED,
	FLAG_ROCK_SOUTH_CLEARED
};

/* Set while the waves run, from the stars over a trunk until the sea's */
/* patterns come back; meanwhile the channels carry nothing and no laps are */
/* counted. */
#define FLAG_WAVES_RUNNING FLAG_SCENE_TEMP_00
/* Set the first time a channel sweeps the ship out of the sea, which puts the
 */
/* party back aboard the Lemurian ship; later sweeps land on the world map. */
/* Facing Poseidon sets it too. */
#define FLAG_SWEPT_BACK_ONCE 0x8dd

/* The kinds of water the ship sails over. A swirl is six cells square: a */
/* ring of eight 2 by 2 squares of current, each named for where it lies on */
/* the ring, round a 2 by 2 eye that turns clockwise or counterclockwise. */
/* Rough water turns the ship back; a channel carries it straight on, east, */
/* south, west or north; the cells at the sea's edge where the channels run */
/* out sweep it out of the sea; and the eight 2 by 2 lap squares round each */
/* trunk, numbered in order round it from `CELL_BEHAVIOR_LAP_FIRST`, count its
 */
/* laps. The sea's cells sit on its own layer, which the map keeps 64 columns */
/* east of the ground's; the cells named below are the ground's. */
typedef enum SEA_OF_TIME_CELL_BEHAVIOR {
	CELL_BEHAVIOR_SWIRL_RING_SOUTH = 30,
	CELL_BEHAVIOR_SWIRL_RING_SOUTHEAST,
	CELL_BEHAVIOR_SWIRL_RING_EAST,
	CELL_BEHAVIOR_SWIRL_RING_NORTHEAST,
	CELL_BEHAVIOR_SWIRL_RING_NORTH,
	CELL_BEHAVIOR_SWIRL_RING_NORTHWEST,
	CELL_BEHAVIOR_SWIRL_RING_WEST,
	CELL_BEHAVIOR_SWIRL_RING_SOUTHWEST,
	CELL_BEHAVIOR_SWIRL_COUNTERCLOCKWISE = 40,
	CELL_BEHAVIOR_SWIRL_CLOCKWISE,
	CELL_BEHAVIOR_ROUGH_WATER = 44,
	CELL_BEHAVIOR_CHANNEL_EAST,
	CELL_BEHAVIOR_CHANNEL_SOUTH,
	CELL_BEHAVIOR_CHANNEL_WEST,
	CELL_BEHAVIOR_CHANNEL_NORTH,
	CELL_BEHAVIOR_SWEPT_BACK,
	CELL_BEHAVIOR_LAP_FIRST
} SEA_OF_TIME_CELL_BEHAVIOR;

/* The raised triggers of the waves: the laps round a trunk are done, and the */
/* waves are over. */
typedef enum SEA_OF_TIME_RAISED_TRIGGER {
	RAISED_START_WAVES = 200,
	RAISED_STOP_WAVES
} SEA_OF_TIME_RAISED_TRIGGER;

/* The last stretch's step triggers: in the north part, cells (20, 18) to */
/* (23, 20), where the ship meets Poseidon; in the south part, the way off its
 */
/* west edge to the world map and the way off its east edge back north. */
enum {
	TRIGGER_TO_WORLD_MAP = 1,
	TRIGGER_BACK_NORTH = 2,
	TRIGGER_BEFORE_POSEIDON = 5
};
/* The last stretch's north part is reached by entrances 1 to */
/* `NORTH_PART_LAST_ENTRANCE`; the last is where the ship waits after */
/* Poseidon's battle. The north part's way down to the south part, at its west
 */
/* edge, is its exit 2, arriving by entrance 6, and the south part's way back */
/* north is its exit 5, arriving by entrance 2. */
enum {
	NORTH_PART_LAST_ENTRANCE = 4,
	ENTRANCE_FROM_SOUTH_PART = 2,
	ENTRANCE_AFTER_POSEIDON = 4,
	ENTRANCE_FROM_NORTH_PART = 6,
	EXIT_TO_SOUTH_PART = 2,
	EXIT_BACK_NORTH = 5
};

const ACTOR_PLACE *scGetActorPlacements(void) {
	if(MainRoutineStateWork.next == SCENE_SEA_OF_TIME_TO_LEMURIA)
		return SeaOfTime_ToLemuriaPlacements;
	return SeaOfTime_SwirlsPlacements;
}

/* The 32-pixel square, two cells across, that a 16.16 coordinate lies in, and
 */
/* a square's corner: a swirl's eye is one such square, and its ring the eight
 */
/* round it. */
#define SWIRL_SQUARE(coordinate)	((coordinate) >> 21)
#define SWIRL_SQUARE_CORNER(square) ((square) << 21)
#define SWIRL_SQUARE_ORIGIN(coordinate)                                        \
	SWIRL_SQUARE_CORNER(SWIRL_SQUARE(coordinate))
/* How far, in pixels, the ship may drift from a swirl's eye before the ring's
 */
/* current pulls it back in. */
#define CURRENT_HOLD_DISTANCE 46
/* How far either side of the ship its spray is thrown, in pixels: by a */
/* swirl's ring, and by a channel or rough water. */
#define CURRENT_SPRAY_REACH 10
#define CHANNEL_SPRAY_REACH 12
/* n quarter turns as an angle, DEGtoANG(90) apiece. */
#define QUARTER_TURNS(n) ((n) << 14)

/* The sea's background palette, whose colours 1 to SEA_COLORS the waves tint */
/* and the sea's frame callback turns round; the next palette's turn as the */
/* water flows. */
#define SEA_PALETTE	 3
#define SEA_COLORS	 14
#define FLOW_PALETTE (SEA_PALETTE + 1)
/* The colour the waves tint the sea's colours toward, a step a channel at a */
/* time. */
#define WAVE_RED   11
#define WAVE_GREEN 6
#define WAVE_BLUE  23
/* The frames between the sea's colour steps while the waves run. */
#define WAVE_COLOR_STEP_FRAMES 360
/* The background colour whose red pulses while no waves run, between */
/* `PULSE_RED_MIN` and full: colour 15 of background palette 0. */
#define PULSE_COLOR	  BG_PLTT_COLOR(0, 15)
#define PULSE_RED_MIN 18
/* The ripple's sine shifted down to a swing of two pixels each way. */
#define RIPPLE_SHIFT 15
/* The shower of water over Poseidon: PUFF_SHOWER_FRAMES long, a puff every 16
 */
/* frames, from PUFF_SHOWER_FASTER every 8 and from PUFF_SHOWER_FASTEST every */
/* other frame, each somewhere in a PUFF_SPREAD-pixel square; BIG_PUFFS larger
 */
/* ones follow, one a frame. */
#define PUFF_SHOWER_FRAMES	360
#define PUFF_SHOWER_FASTER	120
#define PUFF_SHOWER_FASTEST 240
#define PUFF_SPREAD			32
#define BIG_PUFFS			16

/* The Lemurian ship sails into the dark eye of a swirl, and the swirl pulls it
 */
/* into the very middle and spins it round there, clockwise or */
/* counterclockwise as the swirl turns, until the player steers it out. Each */
/* eye is 2 by 2 cells, such as cells (48, 6) to (49, 7). */
void scSeaOfTimeCircleSwirl(void) {
	ACTOR *actor = fdActorGetById(ACTOR_SHIP);
	s32 turn[1];
	FIXED center[3];
	s32 *pos;
	FIXED sine;
	FIXED dx;
	FIXED dz;
	FIXED x;
	FIXED z;

	center[0] = SWIRL_SQUARE_ORIGIN(actor->x.fixed) + toFIXED(16);
	center[2] = SWIRL_SQUARE_ORIGIN(actor->z.fixed) + toFIXED(16);
	pos = &actor->x.fixed;
	if(scPosFlatDistance(pos, center) > 4) {
		if(*pos < center[0])
			*pos += toFIXED(0.2);
		else
			*pos -= toFIXED(0.2);
		if(actor->z.fixed < center[2])
			actor->z.fixed += toFIXED(0.2);
		else
			actor->z.fixed -= toFIXED(0.2);
		if(fdMapCellGetAttributes(MAPGRID_LAYER_MIDDLE, FIELD_CELL(center[0]),
								  FIELD_CELL(center[2])) >>
			   8 ==
		   CELL_BEHAVIOR_SWIRL_COUNTERCLOCKWISE) {
			turn[0] = scSeaOfTimeCos(-TRIG_TURN / 128);
			sine = sySin(-TRIG_TURN / 128);
		}
		else {
			turn[0] = scSeaOfTimeCos(TRIG_TURN / 128);
			sine = sySin(TRIG_TURN / 128);
		}
		dx = actor->x.fixed - center[0];
		dz = actor->z.fixed - center[2];
		x = syMulFX(dx, turn[0]) - syMulFX(dz, sine);
		z = syMulFX(dx, sine) + syMulFX(dz, turn[0]);
		x += center[0];
		z += center[2];
		actor->x.fixed = x;
		actor->z.fixed = z;
	}
}

/* The Lemurian ship sails into the ring of current round a swirl's eye, and */
/* the current carries it round the eye, clockwise or counterclockwise as the */
/* swirl turns, with a rushing sound and white spray thrown up on both sides. A
 */
/* ship that drifts too far out is pulled back toward the eye. */
/* */
/* Each ring cell's name says where it lies about the eye, so the eye is one */
/* 32-pixel square (`SWIRL_SQUARE`) the other way. */
void scSeaOfTimeRideCurrent(void) {
	FIELD_STATE *state = (FIELD_STATE *)Ram_HeapSlots->fieldState;
	ACTOR *actor = fdActorGetById(ACTOR_SHIP);
	FIXED center[3];
	FIXED ends[4];
	EFFECT_OPTIONS spark;
	s32 turn[1];
	s32 *pos;
	FIXED dx;
	FIXED dz;
	s32 x;
	s32 z;
	s32 phase;
	s32 frame;
	s32 *counter;
	FIXED c;
	FIXED s;
	s32 flip;
	s32 speed;
	FIXED px;
	FIXED py;
	FIXED pz;

	switch(state->stepTrigger) {
	case CELL_BEHAVIOR_SWIRL_RING_SOUTH:
		center[0] = 0;
		center[2] = -1;
		break;
	case CELL_BEHAVIOR_SWIRL_RING_EAST:
		center[0] = -1;
		center[2] = 0;
		break;
	case CELL_BEHAVIOR_SWIRL_RING_NORTH:
		center[0] = 0;
		center[2] = 1;
		break;
	case CELL_BEHAVIOR_SWIRL_RING_WEST:
		center[0] = 1;
		center[2] = 0;
		break;
	case CELL_BEHAVIOR_SWIRL_RING_SOUTHEAST:
		center[0] = -1;
		center[2] = -1;
		break;
	case CELL_BEHAVIOR_SWIRL_RING_NORTHEAST:
		center[0] = -1;
		center[2] = 1;
		break;
	case CELL_BEHAVIOR_SWIRL_RING_NORTHWEST:
		center[0] = 1;
		center[2] = 1;
		break;
	case CELL_BEHAVIOR_SWIRL_RING_SOUTHWEST:
		center[0] = 1;
		center[2] = -1;
		break;
	}
	center[0] = SWIRL_SQUARE_CORNER(SWIRL_SQUARE(actor->x.fixed) + center[0]) +
				toFIXED(16);
	center[2] = SWIRL_SQUARE_CORNER(SWIRL_SQUARE(actor->z.fixed) + center[2]) +
				toFIXED(16);
	pos = &actor->x.fixed;
	if(scPosFlatDistance(pos, center) > CURRENT_HOLD_DISTANCE) {
		if(*pos < center[0])
			*pos += toFIXED(1) / 2;
		else
			*pos -= toFIXED(1) / 2;
		if(actor->z.fixed < center[2])
			actor->z.fixed += toFIXED(1) / 2;
		else
			actor->z.fixed -= toFIXED(1) / 2;
	}
	if(fdMapCellGetAttributes(MAPGRID_LAYER_MIDDLE, FIELD_CELL(center[0]),
							  FIELD_CELL(center[2])) >>
		   8 ==
	   CELL_BEHAVIOR_SWIRL_COUNTERCLOCKWISE) {
		turn[0] = scSeaOfTimeCos(-TRIG_TURN / 128);
		s = sySin(-TRIG_TURN / 128);
	}
	else {
		turn[0] = scSeaOfTimeCos(TRIG_TURN / 128);
		s = sySin(TRIG_TURN / 128);
	}
	dx = actor->x.fixed - center[0];
	dz = actor->z.fixed - center[2];
	x = syMulFX(dx, turn[0]);
	x -= syMulFX(dz, s);
	z = syMulFX(dx, s);
	z += syMulFX(dz, turn[0]);
	x += center[0];
	z += center[2];
	actor->x.fixed = x;
	actor->z.fixed = z;

	counter = &FrameCount;
	frame = *counter;
	phase = 3;
	phase &= frame;
	if(phase == 0) {
		if((frame & 10) == 0)
			fdEventPlaySnd(SE_CURRENT);
		spark.pri = 2;
		spark.pal = SPRITE_COLOR_WHITE;
		spark.startScaleX = FIXED_ONE * 4 / 5;
		spark.startScaleY = FIXED_ONE * 4 / 5;
		c = syCos(actor->facing);
		{
			FIXED sine = sySin(actor->facing);
			ends[0] = c * CURRENT_SPRAY_REACH;
			ends[1] = sine * CURRENT_SPRAY_REACH;
			ends[2] = -(c * CURRENT_SPRAY_REACH);
			ends[3] = -(sine * CURRENT_SPRAY_REACH);
		}
		ends[0] += actor->x.fixed;
		ends[1] += actor->z.fixed;
		speed = toFIXED(1) / 2;
		ends[2] += actor->x.fixed;
		ends[3] += actor->z.fixed;
		if(ends[1] != actor->z.fixed) {
			ends[1] -= toFIXED(12);
			ends[3] -= toFIXED(12);
		}
		flip = *counter & 4;
		px = ends[0];
		py = actor->y.fixed;
		pz = ends[1];
		if(flip) {
			scFieldEffectSpawn(px + toFIXED(3), py, pz, speed, 0, 0,
							   EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY |
								   EFFECT_USE_PALETTE,
							   &spark);
			scFieldEffectSpawn(ends[2] - toFIXED(3), actor->y.fixed, ends[3],
							   -speed, 0, 0,
							   EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY |
								   EFFECT_USE_PALETTE,
							   &spark);
		}
		else {
			scFieldEffectSpawn(px - toFIXED(3), py, pz, -speed, 0, 0,
							   EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY |
								   EFFECT_USE_PALETTE,
							   &spark);
			scFieldEffectSpawn(ends[2] + toFIXED(3), actor->y.fixed, ends[3],
							   speed, 0, 0,
							   EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY |
								   EFFECT_USE_PALETTE,
							   &spark);
		}
	}
}

/* The Lemurian ship sails onto a channel, a straight strip of striped rushing
 */
/* water, and is carried fast along it to its end, east, south, west or */
/* north, spray flying from both sides. Some channels run off the edge of the */
/* sea, where it is swept out. While the waves run, the channels carry */
/* nothing. */
void scSeaOfTimeFollowChannel(void) {
	FIXED pos[4];
	EFFECT_OPTIONS spark;
	FIELD_STATE *state;
	ACTOR *actor;
	s16 angle;
	s32 x, z, dx, dz;
	FIXED targetX, targetZ;
	FIXED cosine, sine;
	FIXED x0, y0, z0;
	s32 flip;

	state = Ram_HeapSlots->fieldState;
	actor = fdActorGetById(ACTOR_SHIP);
	angle = QUARTER_TURNS(state->stepTrigger - CELL_BEHAVIOR_CHANNEL_EAST);
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	x = FIELD_CELL(actor->x.fixed);
	z = FIELD_CELL(actor->z.fixed);
	dx = FIXEDtoINT(syCos(angle));
	dz = FIXEDtoINT(sySin(angle));
	actor->speedLimit = toFIXED(4);
	actor->acceleration = toFIXED(4);
	targetZ = 0;
	targetX = 0;
	if(fdMapCellGetAttributes(MAPGRID_LAYER_MIDDLE, x + (dz < 0 ? -dz : dz),
							  z + (dx < 0 ? -dx : dx)) >>
		   8 ==
	   state->stepTrigger) {
		targetX = (dz < 0 ? -dz : dz) * toFIXED(15);
		targetZ = (dx < 0 ? -dx : dx) * toFIXED(15);
	}
	targetX += FIELD_CELL_ORIGIN(x);
	targetZ += FIELD_CELL_ORIGIN(z);
	while(fdMapCellGetAttributes(MAPGRID_LAYER_MIDDLE, x, z) >> 8 ==
		  state->stepTrigger) {
		x += dx;
		z += dz;
		targetX += FIELD_CELL_ORIGIN(dx);
		targetZ += FIELD_CELL_ORIGIN(dz);
		fdActorSetMoveTarget(actor, targetX, actor->y.fixed, targetZ);
		while(fdActorHasArrived(actor) == 0) {
			s32 phase = 3;
			phase &= FrameCount;
			if(phase == 0) {
				if((FrameCount & 10) == 0) {
					sySndPlay(SE_CURRENT);
				}
				/* The current's spray (scSeaOfTimeRideCurrent), larger, from
				 * further */
				/* out and thrown 4 pixels wide; each function writes its own.
				 */
				spark.pri = 2;
				spark.pal = SPRITE_COLOR_WHITE;
				spark.startScaleX = FIXED_ONE * 7 / 5;
				spark.startScaleY = FIXED_ONE * 6 / 5;
				cosine = syCos(actor->facing);
				sine = sySin(actor->facing);
				pos[0] = cosine * CHANNEL_SPRAY_REACH;
				pos[1] = sine * CHANNEL_SPRAY_REACH;
				pos[2] = -(cosine * CHANNEL_SPRAY_REACH);
				pos[3] = -(sine * CHANNEL_SPRAY_REACH);
				pos[0] += actor->x.fixed;
				pos[1] += actor->z.fixed;
				pos[2] += actor->x.fixed;
				pos[3] += actor->z.fixed;
				if(pos[1] != actor->z.fixed) {
					pos[1] -= toFIXED(12);
					pos[3] -= toFIXED(12);
				}
				flip = FrameCount & 4;
				y0 = actor->y.fixed;
				x0 = pos[0];
				z0 = pos[1];
				if(flip) {
					scFieldEffectSpawn(
						x0 + toFIXED(4), y0, z0, toFIXED(1) / 2, 0, 0,
						EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY |
							EFFECT_USE_PALETTE,
						&spark);
					scFieldEffectSpawn(pos[2] - toFIXED(4), actor->y.fixed,
									   pos[3], -toFIXED(1) / 2, 0, 0,
									   EFFECT_USE_START_SCALE |
										   EFFECT_USE_PRIORITY |
										   EFFECT_USE_PALETTE,
									   &spark);
				}
				else {
					scFieldEffectSpawn(
						x0 - toFIXED(4), y0, z0, -toFIXED(1) / 2, 0, 0,
						EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY |
							EFFECT_USE_PALETTE,
						&spark);
					scFieldEffectSpawn(pos[2] + toFIXED(4), actor->y.fixed,
									   pos[3], toFIXED(1) / 2, 0, 0,
									   EFFECT_USE_START_SCALE |
										   EFFECT_USE_PRIORITY |
										   EFFECT_USE_PALETTE,
									   &spark);
				}
			}
			sySynch(1);
		}
	}
	fdEventEnd();
}

extern u16 SeaOfTime_LapCount;
extern s16 SeaOfTime_LapTurn;
extern u16 SeaOfTime_LastLapCell;
extern SPOTS SeaOfTime_LapSpots;
extern u8 SeaOfTime_LapsNeeded[];

/* The lap squares round each trunk, in order round it. */
#define LAP_CELLS 8
/* The frames the ship may take from one lap square to the next before its */
/* count starts again. */
#define LAP_STEP_FRAMES 50
/* The squares round after which stars rise about the trunk, somewhere in a */
/* `SPARKLE_SPREAD`-pixel square. */
#define LAP_STEPS_BEFORE_SPARKLES 3
#define SPARKLE_SPREAD			  32

/* The Lemurian ship sails round one of the trunks, the little volcanoes in the
 */
/* great sea, and keeps going the same way round. Soon stars rise glittering */
/* about the trunk, and after two laps or three, as the trunk needs, the waves
 */
/* start. Turning back or dawdling starts the count again. */
/* */
/* Each of the trunk's eight lap squares counts one step when the ship enters */
/* it from the one before, within `LAP_STEP_FRAMES`; the trunks, numbered on */
/* the ground layer, need `SeaOfTime_LapsNeeded` steps. The trunk on cells */
/* (4, 8) to (9, 13), by the way to the last stretch, is one of them. */
void scSeaOfTimeCountLaps(void) {
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	ACTOR *actor = fdActorGetById(ACTOR_SHIP);
	s16 index;
	s32 now;
	s32 last;
	s32 nowRaw;
	u32 phase;
	u32 *frame;
	u32 *stamp;

	index = (fdMapCellGetAttributes(MAPGRID_LAYER_GROUND,
									FIELD_CELL(actor->x.fixed),
									FIELD_CELL(actor->z.fixed)) >>
			 8) -
			1;
	frame = &FrameCount;
	stamp = &SeaOfTime_LastLapFrame;

	if(*frame - *stamp > LAP_STEP_FRAMES) {
		SeaOfTime_LapCount = 0;
		SeaOfTime_LapTurn = 0;
	}
	last = SeaOfTime_LastLapCell;
	now = state->stepTrigger;
	nowRaw = (u16)state->stepTrigger;
	if(last != now) {
		s16 *delta = &SeaOfTime_LapTurn;
		s32 raw;
		s32 diff;
		u16 *count;
		s32 step;

		if(*delta == 0) {
			*delta = last - nowRaw;
		}
		if(*delta == LAP_CELLS - 1) {
			*delta = -1;
		}
		raw = (u16)*delta;
		if((s16)raw == -(LAP_CELLS - 1)) {
			*delta = 1;
		}
		diff = last - now;
		raw = (u16)*delta;
		step = (s16)raw;
		count = &SeaOfTime_LapCount;
		if(diff == step || now - last == step * (LAP_CELLS - 1)) {
			(*count)++;
			*stamp = *frame;
		}
		else {
			*count = 0;
			*delta = 0;
		}
	}
	SeaOfTime_LastLapCell = state->stepTrigger;
	if(SeaOfTime_LapCount > LAP_STEPS_BEFORE_SPARKLES) {
		phase = FrameCount & 3;
		if(phase == 0) {
			SPOTS spots = SeaOfTime_LapSpots;
			EFFECT_OPTIONS params;
			u32 z;
			u32 x;

			params.type = SPRITE_SPARKLE;
			params.pri = 2;
			x = syRandom() & (SPARKLE_SPREAD - 1);
			z = syRandom() & (SPARKLE_SPREAD - 1);
			fdEventPlaySnd(SE_GLITTER);
			x <<= 16;
			z <<= 16;
			scFieldEffectSpawn(
				spots.spot[(u16)index].x + x, 0, spots.spot[(u16)index].z + z,
				0, toFIXED(1.2), phase,
				EFFECT_USE_TYPE | EFFECT_USE_PRIORITY | EFFECT_SCRIPT_MEDIUM,
				&params);
		}
	}
	if(SeaOfTime_LapCount == SeaOfTime_LapsNeeded[(u16)index]) {

		state->raisedTrigger = RAISED_START_WAVES;
		SeaOfTime_LapCount = 0;
		SeaOfTime_LapTurn = 0;
	}
}

/* The frames the waves run. */
#define WAVE_FRAMES 120

/* While the waves run, ends them `WAVE_FRAMES` after the sea has gone still, */
/* and keeps their start, the sea's colour step and the frame count in case */
/* the scene is left and entered again. */
void scSeaOfTimeTimeWaves(void) {
	u32 *state = SceneStateWork;

	if(FrameCount - SeaOfTime_WaveStartFrame > WAVE_FRAMES)
		FieldStateWork->raisedTrigger = RAISED_STOP_WAVES;
	*state++ = SeaOfTime_WaveStartFrame;
	*state++ = SeaOfTime_WaveState;
	*state = FrameCount;
}

extern u32 SeaOfTime_WaveState;

/* Steps one of a colour's channels a unit toward its target, setting changed */
/* if it moved: the sea's colours fade a step at a time this way, toward the */
/* waves' colour and back to the colours saved on arrival. */
#define STEP_TOWARD(channel, target, changed)                                  \
	{                                                                          \
		if((channel) < (target)) {                                             \
			(channel)++;                                                       \
			(changed) = 1;                                                     \
		}                                                                      \
		if((channel) > (target)) {                                             \
			(channel)--;                                                       \
			(changed) = 1;                                                     \
		}                                                                      \
	}

/* One step of the sea's colours (SEA_PALETTE from colour 1) toward the waves'
 */
/* colour, every channel of each a unit nearer. */
#define STEP_SEA_TOWARD_WAVES(palette, i, changed)                             \
	for(i = 1, palette = &BG_PLTT_COLOR(SEA_PALETTE, 1); i != SEA_COLORS;      \
		i++, palette++) {                                                      \
		u16 color = *palette;                                                  \
		u16 r = RGB_RED(color);                                                \
		u16 g = RGB_GREEN(color);                                              \
		u16 b = RGB_BLUE(color);                                               \
                                                                               \
		STEP_TOWARD(r, WAVE_RED, changed);                                     \
		STEP_TOWARD(g, WAVE_GREEN, changed);                                   \
		STEP_TOWARD(b, WAVE_BLUE, changed);                                    \
		*palette = RGB(r, g, b);                                               \
	}

/* One step of the sea's colours back toward those saved on arrival */
/* (SeaOfTime_SavedColors, the sea's from colour 1 on). */
#define STEP_SEA_TOWARD_SAVED_COLORS(palette, saved, i, j, changed)            \
	palette = &BG_PLTT_COLOR(SEA_PALETTE, 0);                                  \
	for(i = 1, saved = &SeaOfTime_SavedColors, j = 0; i != SEA_COLORS + 1;     \
		i++, j++) {                                                            \
		u16 color = palette[i];                                                \
		u16 target = saved->cols[j];                                           \
		u16 r = RGB_RED(color);                                                \
		u16 g = RGB_GREEN(color);                                              \
		u16 b = RGB_BLUE(color);                                               \
		u16 tr = RGB_RED(target);                                              \
		u16 tg = RGB_GREEN(target);                                            \
		u16 tb = RGB_BLUE(target);                                             \
                                                                               \
		STEP_TOWARD(r, tr, changed);                                           \
		STEP_TOWARD(g, tg, changed);                                           \
		STEP_TOWARD(b, tb, changed);                                           \
		palette[i] = RGB(r, g, b);                                             \
	}

/* The waves start once the ship has circled a trunk enough times: with a */
/* rising sound, the stripes and swirls in the water fade over a few seconds */
/* until the sea lies flat, and the channels stop carrying the ship. They last
 */
/* two seconds once the sea is still. */
/* */
/* The sea's colours step toward one colour (`WAVE_RED`, `WAVE_GREEN`, */
/* `WAVE_BLUE`) every 10 frames, and its colour cycle slows to a step every */
/* `WAVE_COLOR_STEP_FRAMES`; `scSeaOfTimeTimeWaves` ends them. */
void scSeaOfTimeStartWaves(void) {
	volatile u16 *palette;
	s32 changed;
	s32 i;
	s32 frame;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdEventSetFlag(FLAG_WAVES_RUNNING);
	fdEventPlaySnd(SE_RISE_WITH_SPARKS_LOW);
	do {
		sySynch(10);
		changed = 0;
		STEP_SEA_TOWARD_WAVES(palette, i, changed);
		frame = FrameCount;
		SeaOfTime_WaveState += frame & 1;
	} while(changed);
	SeaOfTime_WaveState = WAVE_COLOR_STEP_FRAMES;
	SeaOfTime_WaveStartFrame = frame;
	sySetEvent(scSeaOfTimeTimeWaves, TASK_PASS_FRAME);
	fdEventEnd();
}

SAVED_COL SeaOfTime_SavedColors BSS_DATA;

/* The waves stop: with a higher sound the stripes and swirls come back into */
/* the water over a few seconds, and the channels carry the ship again. */
/* */
/* The sea's colours step back to the ones saved on arrival every 10 */
/* frames. */
void scSeaOfTimeStopWaves(void) {
	s32 changed;
	volatile u16 *palette;
	SAVED_COL *saved;
	u32 i;
	u32 j;

	syCloseEvent(scSeaOfTimeTimeWaves);
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	sySndPlay(SE_RISE_WITH_SPARKS_HIGH);
	SeaOfTime_WaveState = 1;
	/* Each of the sea's colours steps one unit per channel toward its saved */
	/* colour every 10 frames, until none changes. */
	do {
		sySynch(10);
		changed = 0;
		STEP_SEA_TOWARD_SAVED_COLORS(palette, saved, i, j, changed);
	} while(changed);
	SeaOfTime_WaveState = 1;
	fdEventClearFlag(FLAG_WAVES_RUNNING);
	fdEventEnd();
}

void scFieldEffectSpawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
						s32 velocityZ, u32 flags,
						const EFFECT_OPTIONS *options);

/* The Lemurian ship sails into the rough water of the great sea's north, the */
/* band along the north edge at cells (46, 1) to (61, 3) or the one at (18, 8)
 */
/* to (35, 9), and the water pushes it back south a little, spray flying */
/* from both sides, every time it tries. */
void scSeaOfTimePushBackInRoughWater(void) {
	FIXED sides[4];
	EFFECT_OPTIONS options;
	ACTOR *actor;
	FIXED cosine, sine;
	FIXED x, y, z;
	FIXED velocity, reverseVelocity;
	s32 alternate;

	actor = fdActorGetById(ACTOR_SHIP);
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdActorSetMoveTarget(actor, actor->x.fixed, actor->y.fixed,
						 actor->z.fixed + toFIXED(8));
	while(fdActorHasArrived(actor) == 0) {
		if((FrameCount & 3) == 0) {
			/* The channels' spray (scSeaOfTimeFollowChannel), written out again
			 * here. */
			options.pri = 2;
			options.pal = SPRITE_COLOR_WHITE;
			options.startScaleX = FIXED_ONE * 7 / 5;
			options.startScaleY = FIXED_ONE * 6 / 5;
			cosine = syCos(actor->facing);
			sine = sySin(actor->facing);
			sides[0] = cosine * CHANNEL_SPRAY_REACH;
			sides[1] = sine * CHANNEL_SPRAY_REACH;
			sides[2] = -(cosine * CHANNEL_SPRAY_REACH);
			sides[3] = -(sine * CHANNEL_SPRAY_REACH);
			sides[0] += actor->x.fixed;
			sides[1] += actor->z.fixed;
			sides[2] += actor->x.fixed;
			sides[3] += actor->z.fixed;
			velocity = toFIXED(1) / 2;
			if(sides[1] != actor->z.fixed) {
				sides[1] -= toFIXED(12);
				sides[3] -= toFIXED(12);
			}
			alternate = FrameCount & 4;
			reverseVelocity = -velocity;
			x = sides[0];
			y = actor->y.fixed;
			z = sides[1];
			if(alternate) {
				scFieldEffectSpawn(x + toFIXED(4), y, z, velocity, 0, 0,
								   EFFECT_USE_PALETTE | EFFECT_USE_PRIORITY |
									   EFFECT_USE_START_SCALE,
								   &options);
				scFieldEffectSpawn(sides[2] - toFIXED(4), actor->y.fixed,
								   sides[3], reverseVelocity, 0, 0,
								   EFFECT_USE_PALETTE | EFFECT_USE_PRIORITY |
									   EFFECT_USE_START_SCALE,
								   &options);
			}
			else {
				scFieldEffectSpawn(x - toFIXED(4), y, z, reverseVelocity, 0, 0,
								   EFFECT_USE_PALETTE | EFFECT_USE_PRIORITY |
									   EFFECT_USE_START_SCALE,
								   &options);
				scFieldEffectSpawn(sides[2] + toFIXED(4), actor->y.fixed,
								   sides[3], velocity, 0, 0,
								   EFFECT_USE_PALETTE | EFFECT_USE_PRIORITY |
									   EFFECT_USE_START_SCALE,
								   &options);
			}
		}
		sySynch(1);
	}
	fdEventEnd();
}

/* A channel runs the Lemurian ship off the edge of the great sea, at cells */
/* (16, 63), (32, 63) or (48, 63) in the south or (62, 16) in the east, and the
 */
/* screen fades. The first time the party finds itself back aboard the ship, */
/* talking over how to find the way; after that the ship comes out on the */
/* world map. */
void scSeaOfTimeSweptBack(void) {
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	if(!fdEventGetFlag(FLAG_SWEPT_BACK_ONCE)) {
		syImportEventSetExitRequest(SEA_OF_TIME_SWIRLS_TO_LEMURIAN_SHIP_TILLER);
		fdEventSetFlag(FLAG_SWEPT_BACK_ONCE);
	}
	else {
		syImportEventSetExitRequest(SEA_OF_TIME_SWIRLS_TO_WORLD_MAP);
	}
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	fdEventEnd();
}

/* Each frame Poseidon stands over the sea, water splashes up at his feet, */
/* every other frame somewhere across his width. */
void scSeaOfTimeUpdateWater(ACTOR *actor) {
	EFFECT_OPTIONS spark;
	s32 even = FrameCount & 1;

	if(even == 0) {
		FIXED dx = toFIXED(syRandom() & 31) - toFIXED(14);

		spark.startScaleX = FIXED_ONE * 7 / 10;
		spark.startScaleY = FIXED_ONE * 7 / 10;
		spark.pal = SPRITE_COLOR_WHITE;
		spark.pri = 1;
		scFieldEffectSpawn(
			actor->x.fixed + dx, 0, actor->z.fixed + toFIXED(16), 0, even, even,
			EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
			&spark);
	}
}

/* Poseidon's battle, in the story battles' encounter table. */
#define ENCOUNTER_SLOT_POSEIDON 6

/* The Lemurian ship sails up the last stretch to cells (20, 18) to (23, 20), */
/* and Poseidon is met: the ship sails north on its own as the camera rises, */
/* the music stops, the water goes flat and its patterns slowly return, a */
/* shower of water bursts over the sea, faster and faster, and Poseidon rises */
/* out of it. His battle starts at once, with no running away, and after it */
/* the ship is back here, at `ENTRANCE_AFTER_POSEIDON`. */
void scSeaOfTimeFacePoseidon(void) {
	volatile u16 fill;
	EFFECT_OPTIONS burst;
	EFFECT_OPTIONS rain;
	ACTOR *actor;
	s32 changed;
	volatile u16 *palette;
	SAVED_COL *saved;
	u32 i;
	u32 j;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	syGameFlagSet(FLAG_SWEPT_BACK_ONCE);
	fdActorSetSpeed(ACTOR_SHIP, 1);
	fdActorGetById(ACTOR_SHIP)->facing = FACING_NORTH;
	fdActorWalkTo(ACTOR_SHIP, 352, 264);
	fdCameraSetSpeed(1);
	fdCameraMoveTo(352, CAMERA_KEEP, 196, 1);
	fdCameraWaitForArrival();
	fdActorWaitForMoveById(ACTOR_SHIP);
	sySndPlay(SOUND_FADE_OUT_BGM);
	sySndPlay(SE_RISE_WITH_SPARKS_HIGH);
	/* The sea's colours all take the waves' colour at once. */
	fill = RGB(WAVE_RED, WAVE_GREEN, WAVE_BLUE);
	syDMAXCopy((const void *)&fill, (void *)&BG_PLTT_COLOR(SEA_PALETTE, 1),
			   DMA_FILL_HALFWORDS(SEA_COLORS), REG_DMA3);
	fdMapGridCopyCellTilesIndicesRect(49, 70, 19, 73, 6, 6);
	/* Steps each colour's red, green and blue by one toward the saved palette
	 */
	/* every 10 frames until none changes. */
	do {
		sySynch(10);
		changed = 0;
		STEP_SEA_TOWARD_SAVED_COLORS(palette, saved, i, j, changed);
	} while(changed);
	fdEventWait(30);
	/* The shower of water, with SE_SCATTER every 64 frames. */
	for(i = 0; i != PUFF_SHOWER_FRAMES; i++) {
		if(i % 64 == 0)
			sySndPlay(SE_SCATTER);
		if(i < PUFF_SHOWER_FASTER) {
			if(i % 16)
				continue;
		}
		else if(i < PUFF_SHOWER_FASTEST) {
			if(i % 8)
				continue;
		}
		else if(i % 2)
			continue;
		{
			s32 x = syRandom();
			s32 mask = PUFF_SPREAD - 1;
			s32 z = syRandom();

			burst.pal = SPRITE_COLOR_WHITE;
			scFieldEffectSpawn(toFIXED(x & mask) + toFIXED(336), 0,
							   toFIXED(z & mask) + toFIXED(176), 0, 0, 0,
							   EFFECT_USE_PALETTE, &burst);
			sySynch(1);
		}
	}
	/* Then the larger puffs, one a frame. */
	for(i = 0; i != BIG_PUFFS; i++) {
		{
			s32 x = syRandom();
			s32 mask = PUFF_SPREAD - 1;
			s32 z = syRandom();
			FIXED offsetZ = toFIXED(z & mask);
			FIXED offsetX = toFIXED(x & mask);

			rain.startScaleX = FIXED_ONE * 3 / 2;
			rain.startScaleY = FIXED_ONE * 3 / 2;
			rain.pal = SPRITE_COLOR_WHITE;
			rain.pri = 1;
			scFieldEffectSpawn(offsetX + toFIXED(336), 0,
							   offsetZ + toFIXED(176), 0, toFIXED(1), 0,
							   EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY |
								   EFFECT_USE_PALETTE,
							   &rain);
			sySynch(1);
		}
	}
	actor = syImportActorCreate(SPRITE_POSEIDON, toFIXED(348), 0, toFIXED(184));
	actor->update = (OBJECT_UPDATE)scSeaOfTimeUpdateWater;
	fdEventWait(60);
	fdActorSelectAnimation(actor, 1);
	fdEventWait(40);
	MainRoutineStateWork.battleType = BATTLE_TYPE_NO_ESCAPE;
	fdEventSetAfterBattleDestination(SCENE_SEA_OF_TIME_TO_LEMURIA,
									 ENTRANCE_AFTER_POSEIDON);
	fdRequestBattleFromEncounterTable(ENCOUNTER_ZONE_SERPENT,
									  ENCOUNTER_SLOT_POSEIDON);
	fdEventEnd();
}

/* A rock Grind has broken is gone from the sea, and the ship can sail over its
 */
/* 2 by 2 cells. */
void scSeaOfTimeRemoveRock(s32 x, s32 z, s32 actor) {
	s32 cx = FIELD_CELL(x);
	s32 cz = FIELD_CELL(z);

	scMapCellSetAttributes(MAPGRID_LAYER_MIDDLE, cx - 1, cz - 1, 0);
	scMapCellSetAttributes(MAPGRID_LAYER_MIDDLE, cx, cz - 1, 0);
	scMapCellSetAttributes(MAPGRID_LAYER_MIDDLE, cx - 1, cz, 0);
	scMapCellSetAttributes(MAPGRID_LAYER_MIDDLE, cx, cz, 0);
	fdActorSetPos(actor, 0, 0);
}

/* Defines what Grind on a rock does: the rock is ground away */
/* (scSeaOfTimeRemoveRock) and its flag set, so it is gone for good. */
#define DEFINE_GRIND_ROCK(name, rock, flag)                                    \
	void name(void) {                                                          \
		ACTOR *actor;                                                          \
                                                                               \
		fdEventStart();                                                        \
		fdEventEndLastingPsynergy(0);                                          \
		actor = fdActorGetById(rock);                                          \
		scSeaOfTimeRemoveRock(actor->x.fixed, fdActorGetById(rock)->z.fixed,   \
							  rock);                                           \
		syGameFlagSet(flag);                                                   \
		fdEventEnd();                                                          \
	}

/* Felix casts Grind on one of the rocks that rose in the last stretch's north
 */
/* part after Poseidon's battle, the one on cells (10, 11) to (11, 12), and it
 */
/* is gone for good. The other rocks, on (18, 8), (23, 13) and (29, 9), have */
/* events of their own, made the same way. */
DEFINE_GRIND_ROCK(SeaOfTime_GrindFirstRock, ACTOR_ROCK_A, FLAG_ROCK_A_CLEARED)

/* As SeaOfTime_GrindFirstRock, for the rock on cells (18, 8) to (19, 9). */
DEFINE_GRIND_ROCK(SeaOfTime_GrindSecondRock, ACTOR_ROCK_B, FLAG_ROCK_B_CLEARED)

/* Unused: Grind on a third rock, ACTOR_ROCK_C, which no arrival raises or */
/* places, so its trigger is never met. */
DEFINE_GRIND_ROCK(SeaOfTime_GrindThirdRock, ACTOR_ROCK_C, FLAG_ROCK_C_CLEARED)

/* As SeaOfTime_GrindFirstRock, for the rock on cells (23, 13) to (24, 14). */
DEFINE_GRIND_ROCK(SeaOfTime_GrindFourthRock, ACTOR_ROCK_D, FLAG_ROCK_D_CLEARED)

/* As SeaOfTime_GrindFirstRock, for the rock on cells (29, 9) to (30, 10). */
DEFINE_GRIND_ROCK(SeaOfTime_GrindFifthRock, ACTOR_ROCK_E, FLAG_ROCK_E_CLEARED)

/* Felix casts Grind on the rock in the last stretch's south part, on cells */
/* (44, 42) to (45, 43), and it is gone for good. */
DEFINE_GRIND_ROCK(SeaOfTime_GrindSouthRock, ACTOR_ROCK_A,
				  FLAG_ROCK_SOUTH_CLEARED)

/* Defines a step trigger that leaves the scene by exit, the screen closing. */
#define DEFINE_LEAVE_BY_EXIT(name, exit)                                       \
	void name(void) {                                                          \
		fdEventStart();                                                        \
		fdEventEndLastingPsynergy(0);                                          \
		syImportEventSetExitRequest(exit);                                     \
		fdEventFinishScrTransition();                                          \
		fdEventWaitTransitionFrames();                                         \
		fdEventEnd();                                                          \
	}

/* The Lemurian ship sails off the west edge of the last stretch's south part,
 */
/* cells (1, 45) to (3, 50), and the screen fades to the world map. */
DEFINE_LEAVE_BY_EXIT(SeaOfTime_LeaveForWorldMap,
					 SEA_OF_TIME_TO_LEMURIA_TO_WORLD_MAP_NORTHEAST)

/* The Lemurian ship sails off the east edge of the last stretch's south part,
 */
/* cells (55, 44) and (55, 45), and the screen fades to its north part. */
DEFINE_LEAVE_BY_EXIT(SeaOfTime_ReturnNorth, EXIT_BACK_NORTH)

const TRIGGER *scGetTriggers(void) {
	if(MainRoutineStateWork.next == SCENE_SEA_OF_TIME_SWIRLS)
		return SeaOfTime_SwirlsTriggers;
	if(MainRoutineStateWork.entrance <= NORTH_PART_LAST_ENTRANCE)
		return SeaOfTime_NorthPartTriggers;
	return SeaOfTime_SouthPartTriggers;
}

/* Each frame, keeps the Sea of Time moving: the swirls turn and the channels'
 */
/* stripes flow as their colours are cycled, one colour's red pulses (fading */
/* out as the waves start and back as they stop), and the whole sea ripples */
/* sideways line by line. */
/* */
/* The swirls' colours turn a step at the pace `SeaOfTime_WaveState` sets and */
/* the channels' every other frame. */
void scSeaOfTimeAnimateSea(void) {
	u16 *line = SeaOfTime_RippleLines;
	MAP_LAYER *layer = fdMapGridGetLayer(2);
	PALETTE_FADE *fade = Ram_HeapSlots->palFade;
	FIELD_STATE *event = Ram_HeapSlots->fieldState;
	s32 i;
	s16 color;

	if(event->psynergy != 0)
		return;
	if(fade->duration != 0)
		return;
	if(event->transitionActive == 0)
		return;

	if(FrameCount - SeaOfTime_WaveStartFrame > SeaOfTime_WaveState) {
		s16 target;

		color = BG_PLTT_COLOR(SEA_PALETTE, SEA_COLORS);
		target = SeaOfTime_SavedColors.cols[SEA_COLORS - 1];

		for(i = SEA_COLORS; i != 0; i--) {
			BG_PLTT_COLOR(SEA_PALETTE, i) = BG_PLTT_COLOR(SEA_PALETTE, i - 1);
			/* UB: the last pass reads the halfword before colors[0], which the
			 * store */
			/* after the loop overwrites. */
#ifdef UBFIX
			if(i != 1)
#endif
				SeaOfTime_SavedColors.cols[i - 1] =
					SeaOfTime_SavedColors.cols[i - 2];
		}
		BG_PLTT_COLOR(SEA_PALETTE, 1) = color;
		SeaOfTime_SavedColors.cols[0] = target;
		SeaOfTime_WaveStartFrame = FrameCount;
	}

	if((FrameCount & 1) == 0) {
		color = BG_PLTT_COLOR(FLOW_PALETTE, SEA_COLORS);

		for(i = SEA_COLORS; i != 0; i--)
			BG_PLTT_COLOR(FLOW_PALETTE, i) = BG_PLTT_COLOR(FLOW_PALETTE, i - 1);
		BG_PLTT_COLOR(FLOW_PALETTE, 1) = color;

		if((FrameCount & 3) == 0) {
			u16 color = PULSE_COLOR;
			u16 red = RGB_RED(color);
			u16 green = RGB_GREEN(color);
			u16 blue = RGB_BLUE(color);

			if(syGameFlagGet(FLAG_WAVES_RUNNING) == 0) {
				red += SeaOfTime_RedPulseStep[0];
				if(red < PULSE_RED_MIN) {
					red = PULSE_RED_MIN;
					SeaOfTime_RedPulseStep[0] = 1;
				}
				else if(red > RGB_LEVEL_MAX) {
					red = RGB_LEVEL_MAX;
					SeaOfTime_RedPulseStep[0] = -1;
				}
			}
			else if(FieldStateWork->raisedTrigger == RAISED_START_WAVES) {
				if(red != 0)
					red--;
			}
			else if(FieldStateWork->raisedTrigger == RAISED_STOP_WAVES) {
				if(red < RGB_LEVEL_MAX - 1)
					red++;
			}
			PULSE_COLOR = RGB(red & RGB_LEVEL_MAX, green, blue);
		}
	}

	syDMAXCopy((const void *)BG_PLTT, Ram_HeapSlots->palFade,
			   DMA_COPY_WORDS(PALETTE_FADE_BG_COLORS / 2), REG_DMA3);
	fdPaletteFadeSetTarget(PALETTE_TINT_NONE, PALETTES_ALL);
	layer->speedY = 0;
	for(i = 0; i != DISPLAY_HEIGHT; i++)
		*line++ =
			(s16)FIXEDtoINT(layer->x) +
			(sySin((i + *(u8 *)&FrameCount + (s16)FIXEDtoINT(layer->y)) << 9) >>
			 RIPPLE_SHIFT);
	line = SeaOfTime_RippleLines;
	REG_BG1HOFS = *line;
	syDMAXCopy(line + 1, (void *)&REG_BG1HOFS, DMA_HBLANK_HALFWORD_PER_LINE,
			   (volatile u32 *)REG_DMA0);
}

/* The frames a rock takes to grow to its height out of the sea. */
#define ROCK_RISE_FRAMES 10

/* Puts a rock in the last stretch, which the ship cannot sail through. `x` */
/* and `z` count half cells of 8 pixels, from the map's top left, and the */
/* rock covers the 2 by 2 cells round that point. */
void scSeaOfTimePlaceRock(s32 x, s32 z, s32 actor) {
	fdActorGetById(actor)->collisionFlg |= ACTOR_NEAR_PSYNERGY;
	fdActorSetPosFX(actor, FIELD_HALF_CELL_ORIGIN(x) + toFIXED(8),
					FIELD_HALF_CELL_ORIGIN(z) + toFIXED(8));
	x >>= 1;
	z >>= 1;
	scMapCellSetAttributes(MAPGRID_LAYER_MIDDLE, x, z, CELL_BEHAVIOR_WALL << 8);
	scMapCellSetAttributes(MAPGRID_LAYER_MIDDLE, x + 1, z,
						   CELL_BEHAVIOR_WALL << 8);
	scMapCellSetAttributes(MAPGRID_LAYER_MIDDLE, x, z + 1,
						   CELL_BEHAVIOR_WALL << 8);
	scMapCellSetAttributes(MAPGRID_LAYER_MIDDLE, x + 1, z + 1,
						   CELL_BEHAVIOR_WALL << 8);
}

/* A rock rises out of the sea: the camera goes to it, the screen shakes with */
/* a clatter, water splashes and the rock grows up out of the splash. */
void scSeaOfTimeRaiseRock(s32 x, s32 z, s32 actor) {
	EFFECT_OPTIONS spark;
	FIXED px;
	FIXED pz;
	s32 i;

	fdCameraSetSpeed(6);
	px = FIELD_HALF_CELL_ORIGIN(x) + toFIXED(8);
	pz = FIELD_HALF_CELL_ORIGIN(z) + toFIXED(8);
	fdCameraMoveToFX(px, CAMERA_KEEP, pz, 1);
	fdCameraWaitForArrival();
	fdEventPlaySnd(SE_CLATTER);
	fdMapGridSetShake(0, 16, 1);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	spark.startScaleX = FIXED_ONE * 6 / 5;
	spark.startScaleY = FIXED_ONE * 3 / 2;
	spark.pri = 1;
	spark.type = SPRITE_SPLASH;
	scFieldEffectSpawn(
		px, 0, pz, 0, 0, 0,
		EFFECT_USE_TYPE | EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY, &spark);
	fdActorGetById(actor)->scaleY = FIXED_ONE / ROCK_RISE_FRAMES;
	scSeaOfTimePlaceRock(x, z, actor);
	for(i = 0; i != ROCK_RISE_FRAMES; i++) {
		fdActorGetById(actor)->scaleY += FIXED_ONE / ROCK_RISE_FRAMES;
		fdEventWait(1);
	}
	fdActorGetById(actor)->scaleY = FIXED_ONE;
	fdEventWait(20);
}

/* After Poseidon's battle, back in the last stretch, Poseidon stands over the
 */
/* sea. Four rocks rise out of the water one by one, a shower of water bursts */
/* over him until he is gone, the sea's patterns fade away for good, the music
 */
/* comes back, and the way north to Lemuria is open. */
void scSeaOfTimeOpen(void) {
	ACTOR *actor;
	u32 i;
	s32 changed;
	volatile u16 *palette;
	EFFECT_OPTIONS params;
	EFFECT_OPTIONS burst;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdEventPlaySnd(MUS_SILENCE);
	fdMapGridCopyCellsTo(49, 70, 19, 73, 6, 6);
	actor = scFieldActorCreate(SPRITE_POSEIDON, toFIXED(348), 0, toFIXED(184));
	actor->update = (OBJECT_UPDATE)scSeaOfTimeUpdateWater;
	fdCameraMoveTo(352, CAMERA_KEEP, 196, 0);
	fdEventStartScrTransition();
	sySynch(1);
	sySetEvent(scSeaOfTimeAnimateSea, TASK_PASS_VBLANK);
	fdEventWaitTransitionFrames();
	fdEventWait(60);
	scSeaOfTimeRaiseRock(21, 23, ACTOR_ROCK_A);
	scSeaOfTimeRaiseRock(37, 17, ACTOR_ROCK_B);
	scSeaOfTimeRaiseRock(47, 27, ACTOR_ROCK_D);
	scSeaOfTimeRaiseRock(59, 19, ACTOR_ROCK_E);
	fdMapGridWaitForShakeToSettle();
	fdCameraMoveTo(352, CAMERA_KEEP, 196, 1);
	fdCameraWaitForArrival();
	fdEventSetFlag(FLAG_WAVES_RUNNING);
	FieldStateWork->raisedTrigger = RAISED_START_WAVES;
	fdEventPlaySnd(SE_SEA_OF_TIME_OPENS);
	fdEventWait(40);
	for(i = 0; i != PUFF_SHOWER_FRAMES; i++) {
		if(i % 32 == 0)
			fdEventPlaySnd(SE_SCATTER);
		if(i < PUFF_SHOWER_FASTER) {
			if(i % 16)
				continue;
		}
		else if(i < PUFF_SHOWER_FASTEST) {
			if(i % 8)
				continue;
		}
		else if(i % 2)
			continue;
		{
			s32 mask = PUFF_SPREAD - 1;
			s32 x = syRandom();
			s32 y = syRandom();

			params.startScaleX = FIXED_ONE * 4 / 5;
			params.startScaleY = FIXED_ONE * 4 / 5;
			params.pri = 1;
			params.pal = SPRITE_COLOR_WHITE;
			x &= mask;
			x <<= 16;
			x += toFIXED(336);
			scFieldEffectSpawn(x, 0, toFIXED(y & mask) + toFIXED(176), 0, 0, 0,
							   EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY |
								   EFFECT_USE_PALETTE,
							   &params);
			sySynch(1);
		}
	}
	for(i = 0; i != BIG_PUFFS; i++) {
		s32 mask = PUFF_SPREAD - 1;
		s32 x = syRandom();
		s32 y = syRandom();

		burst.startScaleX = FIXED_ONE * 3 / 2;
		burst.startScaleY = FIXED_ONE * 3 / 2;
		burst.pal = SPRITE_COLOR_WHITE;
		burst.pri = 1;
		x &= mask;
		x <<= 16;
		x += toFIXED(336);
		scFieldEffectSpawn(
			x, 0, toFIXED(y & mask) + toFIXED(176), 0, toFIXED(1), 0,
			EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
			&burst);
		sySynch(1);
	}
	fdActorRelease(actor);
	fdEventPlaySnd(SE_RISE_WITH_SPARKS_LOW);
	do {
		sySynch(10);
		changed = 0;
		STEP_SEA_TOWARD_WAVES(palette, i, changed);
		SeaOfTime_WaveState += FrameCount & 1;
	} while(changed);
	syCloseEvent(scSeaOfTimeAnimateSea);
	scPlayMusic();
	fdEventSetFlag(FLAG_SEA_OF_TIME_OPENED);
	fdEventEnd();
}

/* Sets up the sea. In the great sea, the sea's colours are saved, and if the */
/* waves were running when the party left, they are still running. In the */
/* last stretch's north part, once Poseidon is beaten, the rocks Grind has not
 */
/* broken are back and the pulsing red is gone; on the arrival from his battle,
 */
/* the sea opens (`scSeaOfTimeOpen`). In its south part the south rock is */
/* placed until it is broken. */
s32 scInit(void) {
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	if(MainRoutineStateWork.next == SCENE_SEA_OF_TIME_SWIRLS) {
		SeaOfTime_WaveStartFrame = FrameCount;
		SeaOfTime_WaveState = 1;
		syDMAXCopy((const void *)&BG_PLTT_COLOR(SEA_PALETTE, 1),
				   SeaOfTime_SavedColors.cols, DMA_COPY_HALFWORDS(SEA_COLORS),
				   REG_DMA3);
		if(syGameFlagGet(FLAG_WAVES_RUNNING)) {
			u32 *state = SceneStateWork;
			u32 offset, i;

			SeaOfTime_WaveStartFrame = *state++;
			SeaOfTime_WaveState = *state++;
			FrameCount = *state;
			/* The sea's colours but the last, by their byte offsets. */
			i = 1;
			offset = (SEA_PALETTE * PALETTE_COLORS + 1) * sizeof(u16);
			do {
				*(volatile u16 *)((u32)BG_PLTT + offset) =
					RGB(WAVE_RED, WAVE_GREEN, WAVE_BLUE);
				i++;
				offset += 2;
			} while(i != SEA_COLORS);
			sySetEvent(scSeaOfTimeTimeWaves, TASK_PASS_FRAME);
		}
	}
	else if(MainRoutineStateWork.next == SCENE_SEA_OF_TIME_TO_LEMURIA) {
		SeaOfTime_WaveStartFrame = FrameCount;
		SeaOfTime_WaveState = 0;
		if(MainRoutineStateWork.entrance <= NORTH_PART_LAST_ENTRANCE) {
			if(!syGameFlagGet(FLAG_SEA_OF_TIME_OPENED)) {
				syDMAXCopy((const void *)&BG_PLTT_COLOR(SEA_PALETTE, 1),
						   SeaOfTime_SavedColors.cols,
						   DMA_COPY_HALFWORDS(SEA_COLORS), REG_DMA3);
			}
			else {
				if(!syGameFlagGet(FLAG_ROCK_A_CLEARED))
					scSeaOfTimePlaceRock(21, 23, ACTOR_ROCK_A);
				if(!syGameFlagGet(FLAG_ROCK_B_CLEARED))
					scSeaOfTimePlaceRock(37, 17, ACTOR_ROCK_B);
				if(!syGameFlagGet(FLAG_ROCK_D_CLEARED))
					scSeaOfTimePlaceRock(47, 27, ACTOR_ROCK_D);
				if(!syGameFlagGet(FLAG_ROCK_E_CLEARED))
					scSeaOfTimePlaceRock(59, 19, ACTOR_ROCK_E);
				{
					void *block = Ram_HeapSlots->palFade;

					PULSE_COLOR &= ~RGB(RGB_LEVEL_MAX, 0, 0);
					syDMAXCopy((const void *)BG_PLTT, block,
							   DMA_COPY_WORDS(PALETTE_FADE_BG_COLORS / 2),
							   REG_DMA3);
				}
				fdPaletteFadeSetTarget(PALETTE_TINT_NONE, PALETTES_ALL);
			}
			if(MainRoutineStateWork.entrance == ENTRANCE_AFTER_POSEIDON &&
			   !syGameFlagGet(FLAG_KEEP_PARTY_POSITION))
				scSeaOfTimeOpen();
		}
		else {
			if(!syGameFlagGet(FLAG_ROCK_SOUTH_CLEARED))
				scSeaOfTimePlaceRock(89, 85, ACTOR_ROCK_A);
		}
	}
	{
		s32 priority = TASK_PASS_VBLANK;

		sySetEvent(scSeaOfTimeAnimateSea, priority);
	}
	return 0;
}

s32 scPrepareMap(void) {
	void *block = Ram_HeapSlots->palFade;

	btDmaFill32(-1, block, PALETTE_FADE_BG_COLORS * sizeof(u16));
	fdPaletteFadeSetSource(PALETTE_TINT_NONE, PALETTES_ALL);
	return 0;
}

/* clang-format off */
#include "shared/field_effect_duration_scripts.c"

/* The steps round each trunk the ship must sail before the waves start. */
u8 SeaOfTime_LapsNeeded[] = {
    2 * LAP_CELLS, 3 * LAP_CELLS, 2 * LAP_CELLS,
    3 * LAP_CELLS, 2 * LAP_CELLS, 3 * LAP_CELLS,
};

/* Where the sparkles rise round each trunk as the ship's laps add up, in 16.16 */
/* pixels. */
SPOTS SeaOfTime_LapSpots = {{
    {toFIXED(672), toFIXED(704)},
    {toFIXED(896), toFIXED(320)},
    {toFIXED(512), toFIXED(192)},
    {toFIXED(320), toFIXED(416)},
    {toFIXED(320), toFIXED(832)},
    {toFIXED(96), toFIXED(160)},
}};
/* clang-format on */

const s32 *FieldEffect_DurationScripts[] = {
	FieldEffect_ShortScript,
	FieldEffect_MediumScript,
	FieldEffect_LongScript,
};

s32 SeaOfTime_ShipRests[] = {
	SC_CALL((s32)syShipDropShadow),
	SC_END,
};

#include "shared/scene_entrances_default.c"

u32 SceneExits[] = {
	SCENE_EXITS(SeaOfTimeSwirls),
	SCENE_EXIT_UNLESS(SEA_OF_TIME_SWIRLS_TO_WORLD_MAP, WorldMap,
					  WORLD_MAP_FROM_SEA_OF_TIME_SWIRLS, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(
		SEA_OF_TIME_SWIRLS_TO_SEA_OF_TIME_TO_LEMURIA, SeaOfTimeToLemuria,
		SEA_OF_TIME_TO_LEMURIA_FROM_SEA_OF_TIME_SWIRLS, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(3, SeaOfTimeSwirls, SEA_OF_TIME_SWIRLS_FROM_WORLD_MAP,
					  EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(
		SEA_OF_TIME_SWIRLS_TO_LEMURIAN_SHIP_TILLER, LemurianShipTiller,
		LEMURIAN_SHIP_TILLER_FROM_SEA_OF_TIME_SWIRLS, EXIT_FLAG_NONE),
	SCENE_EXITS(SeaOfTimeToLemuria),
	SCENE_EXIT_UNLESS(
		SEA_OF_TIME_TO_LEMURIA_TO_SEA_OF_TIME_SWIRLS, SeaOfTimeSwirls,
		SEA_OF_TIME_SWIRLS_FROM_SEA_OF_TIME_TO_LEMURIA, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(EXIT_TO_SOUTH_PART, SeaOfTimeToLemuria,
					  ENTRANCE_FROM_NORTH_PART, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(SEA_OF_TIME_TO_LEMURIA_TO_WORLD_MAP_NORTH, WorldMap,
					  WORLD_MAP_FROM_SEA_OF_TIME_TO_LEMURIA_NORTH,
					  EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(SEA_OF_TIME_TO_LEMURIA_TO_WORLD_MAP_NORTHEAST, WorldMap,
					  WORLD_MAP_FROM_SEA_OF_TIME_TO_LEMURIA_NORTHEAST,
					  EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(EXIT_BACK_NORTH, SeaOfTimeToLemuria,
					  ENTRANCE_FROM_SOUTH_PART, EXIT_FLAG_NONE),
	SCENE_EXITS_END,
};

ACTOR_PLACE SeaOfTime_SwirlsPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, (s32)SeaOfTime_ShipRests,
					120, 0, 280, FACING_NORTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

ACTOR_PLACE SeaOfTime_ToLemuriaPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, (s32)SeaOfTime_ShipRests,
					120, 0, 280, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SEA_ROCK, CONDITION_ALWAYS,
					MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_NORTH,
					TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SEA_ROCK, CONDITION_ALWAYS,
					MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_NORTH,
					TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SEA_ROCK, CONDITION_ALWAYS,
					MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_NORTH,
					TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SEA_ROCK, CONDITION_ALWAYS,
					MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_NORTH,
					TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SEA_ROCK, CONDITION_ALWAYS,
					MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_NORTH,
					TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

/* The ship's laps round a trunk: the last step trigger it crossed, the steps */
/* counted, the way it turns and the frame of its last step; then the waves' */
/* start frame and state. */
u16 SeaOfTime_LastLapCell = 0;
u16 SeaOfTime_LapCount = 0;
s16 SeaOfTime_LapTurn = 0;
u32 SeaOfTime_LastLapFrame = 0;
volatile u32 SeaOfTime_WaveStartFrame = 0;
u32 SeaOfTime_WaveState = 0;

TRIGGER SeaOfTime_SwirlsTriggers[] = {
	{TRIGGER_EXIT, SEA_OF_TIME_SWIRLS_TO_WORLD_MAP, CONDITION_ALWAYS,
	 SEA_OF_TIME_SWIRLS_TO_WORLD_MAP},
	{TRIGGER_EXIT, SEA_OF_TIME_SWIRLS_TO_SEA_OF_TIME_TO_LEMURIA,
	 CONDITION_ALWAYS, SEA_OF_TIME_SWIRLS_TO_SEA_OF_TIME_TO_LEMURIA},
	{TRIGGER_STEP, CELL_BEHAVIOR_SWEPT_BACK, CONDITION_ALWAYS,
	 EVENT(scSeaOfTimeSweptBack)},
	{TRIGGER_STEP, CELL_BEHAVIOR_SWIRL_RING_SOUTH, CONDITION_ALWAYS,
	 EVENT(scSeaOfTimeRideCurrent)},
	{TRIGGER_STEP, CELL_BEHAVIOR_SWIRL_RING_SOUTHEAST, CONDITION_ALWAYS,
	 EVENT(scSeaOfTimeRideCurrent)},
	{TRIGGER_STEP, CELL_BEHAVIOR_SWIRL_RING_EAST, CONDITION_ALWAYS,
	 EVENT(scSeaOfTimeRideCurrent)},
	{TRIGGER_STEP, CELL_BEHAVIOR_SWIRL_RING_NORTHEAST, CONDITION_ALWAYS,
	 EVENT(scSeaOfTimeRideCurrent)},
	{TRIGGER_STEP, CELL_BEHAVIOR_SWIRL_RING_NORTH, CONDITION_ALWAYS,
	 EVENT(scSeaOfTimeRideCurrent)},
	{TRIGGER_STEP, CELL_BEHAVIOR_SWIRL_RING_NORTHWEST, CONDITION_ALWAYS,
	 EVENT(scSeaOfTimeRideCurrent)},
	{TRIGGER_STEP, CELL_BEHAVIOR_SWIRL_RING_WEST, CONDITION_ALWAYS,
	 EVENT(scSeaOfTimeRideCurrent)},
	{TRIGGER_STEP, CELL_BEHAVIOR_SWIRL_RING_SOUTHWEST, CONDITION_ALWAYS,
	 EVENT(scSeaOfTimeRideCurrent)},
	{TRIGGER_STEP, CELL_BEHAVIOR_SWIRL_COUNTERCLOCKWISE, CONDITION_ALWAYS,
	 EVENT(scSeaOfTimeCircleSwirl)},
	{TRIGGER_STEP, CELL_BEHAVIOR_SWIRL_CLOCKWISE, CONDITION_ALWAYS,
	 EVENT(scSeaOfTimeCircleSwirl)},
	{TRIGGER_STEP, CELL_BEHAVIOR_ROUGH_WATER, CONDITION_ALWAYS,
	 EVENT(scSeaOfTimePushBackInRoughWater)},
	{TRIGGER_STEP, CELL_BEHAVIOR_CHANNEL_EAST, FLAG_WAVES_RUNNING,
	 EVENT(scSeaOfTimeFollowChannel)},
	{TRIGGER_STEP, CELL_BEHAVIOR_CHANNEL_SOUTH, FLAG_WAVES_RUNNING,
	 EVENT(scSeaOfTimeFollowChannel)},
	{TRIGGER_STEP, CELL_BEHAVIOR_CHANNEL_WEST, FLAG_WAVES_RUNNING,
	 EVENT(scSeaOfTimeFollowChannel)},
	{TRIGGER_STEP, CELL_BEHAVIOR_CHANNEL_NORTH, FLAG_WAVES_RUNNING,
	 EVENT(scSeaOfTimeFollowChannel)},
	{TRIGGER_STEP, CELL_BEHAVIOR_LAP_FIRST, FLAG_WAVES_RUNNING,
	 EVENT(scSeaOfTimeCountLaps)},
	{TRIGGER_STEP, CELL_BEHAVIOR_LAP_FIRST + 1, FLAG_WAVES_RUNNING,
	 EVENT(scSeaOfTimeCountLaps)},
	{TRIGGER_STEP, CELL_BEHAVIOR_LAP_FIRST + 2, FLAG_WAVES_RUNNING,
	 EVENT(scSeaOfTimeCountLaps)},
	{TRIGGER_STEP, CELL_BEHAVIOR_LAP_FIRST + 3, FLAG_WAVES_RUNNING,
	 EVENT(scSeaOfTimeCountLaps)},
	{TRIGGER_STEP, CELL_BEHAVIOR_LAP_FIRST + 4, FLAG_WAVES_RUNNING,
	 EVENT(scSeaOfTimeCountLaps)},
	{TRIGGER_STEP, CELL_BEHAVIOR_LAP_FIRST + 5, FLAG_WAVES_RUNNING,
	 EVENT(scSeaOfTimeCountLaps)},
	{TRIGGER_STEP, CELL_BEHAVIOR_LAP_FIRST + 6, FLAG_WAVES_RUNNING,
	 EVENT(scSeaOfTimeCountLaps)},
	{TRIGGER_STEP, CELL_BEHAVIOR_LAP_FIRST + 7, FLAG_WAVES_RUNNING,
	 EVENT(scSeaOfTimeCountLaps)},
	{TRIGGER_RAISED, RAISED_START_WAVES, CONDITION_ALWAYS,
	 EVENT(scSeaOfTimeStartWaves)},
	{TRIGGER_RAISED, RAISED_STOP_WAVES, CONDITION_ALWAYS,
	 EVENT(scSeaOfTimeStopWaves)},
	{TRIGGER_TABLE_END},
};

TRIGGER SeaOfTime_NorthPartTriggers[] = {
	{TRIGGER_EXIT, SEA_OF_TIME_TO_LEMURIA_TO_SEA_OF_TIME_SWIRLS,
	 CONDITION_ALWAYS, SEA_OF_TIME_TO_LEMURIA_TO_SEA_OF_TIME_SWIRLS},
	{TRIGGER_EXIT, EXIT_TO_SOUTH_PART, CONDITION_ALWAYS, EXIT_TO_SOUTH_PART},
	{TRIGGER_EXIT, SEA_OF_TIME_TO_LEMURIA_TO_WORLD_MAP_NORTH, CONDITION_ALWAYS,
	 SEA_OF_TIME_TO_LEMURIA_TO_WORLD_MAP_NORTH},
	{TRIGGER_STEP, TRIGGER_BEFORE_POSEIDON, FLAG_SEA_OF_TIME_OPENED,
	 EVENT(scSeaOfTimeFacePoseidon)},
	{TRIGGER_STEP, CELL_BEHAVIOR_SWIRL_RING_SOUTH, CONDITION_ALWAYS,
	 EVENT(scSeaOfTimeRideCurrent)},
	{TRIGGER_STEP, CELL_BEHAVIOR_SWIRL_RING_SOUTHEAST, CONDITION_ALWAYS,
	 EVENT(scSeaOfTimeRideCurrent)},
	{TRIGGER_STEP, CELL_BEHAVIOR_SWIRL_RING_EAST, CONDITION_ALWAYS,
	 EVENT(scSeaOfTimeRideCurrent)},
	{TRIGGER_STEP, CELL_BEHAVIOR_SWIRL_RING_NORTHEAST, CONDITION_ALWAYS,
	 EVENT(scSeaOfTimeRideCurrent)},
	{TRIGGER_STEP, CELL_BEHAVIOR_SWIRL_RING_NORTH, CONDITION_ALWAYS,
	 EVENT(scSeaOfTimeRideCurrent)},
	{TRIGGER_STEP, CELL_BEHAVIOR_SWIRL_RING_NORTHWEST, CONDITION_ALWAYS,
	 EVENT(scSeaOfTimeRideCurrent)},
	{TRIGGER_STEP, CELL_BEHAVIOR_SWIRL_RING_WEST, CONDITION_ALWAYS,
	 EVENT(scSeaOfTimeRideCurrent)},
	{TRIGGER_STEP, CELL_BEHAVIOR_SWIRL_RING_SOUTHWEST, CONDITION_ALWAYS,
	 EVENT(scSeaOfTimeRideCurrent)},
	{TRIGGER_STEP, CELL_BEHAVIOR_CHANNEL_NORTH, CONDITION_ALWAYS,
	 EVENT(scSeaOfTimeFollowChannel)},
	{TRIGGER_STEP, CELL_BEHAVIOR_SWIRL_CLOCKWISE, CONDITION_ALWAYS,
	 EVENT(scSeaOfTimeCircleSwirl)},
	{PSYNERGY_ON_ACTOR(ABILITY_GRIND), ACTOR_ROCK_A, CONDITION_ALWAYS,
	 EVENT(SeaOfTime_GrindFirstRock)},
	{PSYNERGY_ON_ACTOR(ABILITY_GRIND), ACTOR_ROCK_B, CONDITION_ALWAYS,
	 EVENT(SeaOfTime_GrindSecondRock)},
	{PSYNERGY_ON_ACTOR(ABILITY_GRIND), ACTOR_ROCK_C, CONDITION_ALWAYS,
	 EVENT(SeaOfTime_GrindThirdRock)},
	{PSYNERGY_ON_ACTOR(ABILITY_GRIND), ACTOR_ROCK_D, CONDITION_ALWAYS,
	 EVENT(SeaOfTime_GrindFourthRock)},
	{PSYNERGY_ON_ACTOR(ABILITY_GRIND), ACTOR_ROCK_E, CONDITION_ALWAYS,
	 EVENT(SeaOfTime_GrindFifthRock)},
	{TRIGGER_TABLE_END},
};

TRIGGER SeaOfTime_SouthPartTriggers[] = {
	{TRIGGER_STEP, TRIGGER_TO_WORLD_MAP, CONDITION_ALWAYS,
	 EVENT(SeaOfTime_LeaveForWorldMap)},
	{TRIGGER_STEP, TRIGGER_BACK_NORTH, CONDITION_ALWAYS,
	 EVENT(SeaOfTime_ReturnNorth)},
	{TRIGGER_STEP, CELL_BEHAVIOR_CHANNEL_EAST, CONDITION_ALWAYS,
	 EVENT(scSeaOfTimeFollowChannel)},
	{TRIGGER_STEP, CELL_BEHAVIOR_CHANNEL_SOUTH, CONDITION_ALWAYS,
	 EVENT(scSeaOfTimeFollowChannel)},
	{TRIGGER_STEP, CELL_BEHAVIOR_CHANNEL_WEST, CONDITION_ALWAYS,
	 EVENT(scSeaOfTimeFollowChannel)},
	{TRIGGER_STEP, CELL_BEHAVIOR_CHANNEL_NORTH, CONDITION_ALWAYS,
	 EVENT(scSeaOfTimeFollowChannel)},
	{PSYNERGY_ON_ACTOR(ABILITY_GRIND), ACTOR_ROCK_A, CONDITION_ALWAYS,
	 EVENT(SeaOfTime_GrindSouthRock)},
	{TRIGGER_TABLE_END},
};

/* The step the sea's red pulse takes each time it brightens or dims. */
s16 SeaOfTime_RedPulseStep[] = {
	1,
};
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef EFFECT_SPRITE_PRIORITY
#undef SceneStateWork

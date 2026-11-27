/* The Yampi Desert Cave, the hidden dungeon behind the desert's sand slide: a
   Teleport circle leads on from the room the party comes in by, and in the
   rooms beyond the party pushes logs, sinks a stake with Pound, bursts pillars,
   slips under walls with Sand, Scoops up the Venus Djinni Crystal racing
   through the sand, and meets Valukar, the guardian of fire. */
#include "macros.h"
#include "edition.h"
#include "field/actor_script.h"
#include "field/script_inline.h"
#include "field/field_mode.h"
#include "field/field_mode_script.h"
#include "game/game.h"
#include "graphics/palette_buffers.h"
#include "scene/scene.h"
#include "text/msg_ids.h"

#include "shared/tables.h"

/* Set once Burst has broken the pillar in the Water of Life chest's room. */
#define FLAG_YAMPI_DESERT_CAVE_CHEST_ROOM_PILLAR_BURST	 FLAG_AREA_TEMP_00
/* Set once Burst has broken the pillar in Crystal's room. */
#define FLAG_YAMPI_DESERT_CAVE_CRYSTAL_ROOM_PILLAR_BURST FLAG_AREA_TEMP_01
/* Set once Scoop has dug up the Mythril Silver in Valukar's room. */
#define FLAG_YAMPI_DESERT_CAVE_MYTHRIL_SILVER_DUG_UP	 FLAG_AREA_TEMP_02
/* Set as Move leaves each pillar where it opens the way: Valukar's room's, and
   Crystal's room's west and east pillars. */
#define FLAG_YAMPI_DESERT_CAVE_GUARDIAN_PILLAR_MOVED	 FLAG_AREA_TEMP_03
#define FLAG_YAMPI_DESERT_CAVE_WEST_PILLAR_MOVED		 FLAG_AREA_TEMP_04
#define FLAG_YAMPI_DESERT_CAVE_EAST_PILLAR_MOVED		 FLAG_AREA_TEMP_05
/* Set once Pound has sunk the stake in Crystal's room. */
#define FLAG_YAMPI_DESERT_CAVE_STAKE_POUNDED			 FLAG_SCENE_TEMP_06
/* Set while the sand ripple runs, the Djinni Crystal racing under it. */
#define FLAG_YAMPI_DESERT_CAVE_RIPPLE_RUNNING			 FLAG_SCENE_TEMP_70
/* Set once Scoop has dug Crystal out of the sand. */
#define FLAG_YAMPI_DESERT_CAVE_CRYSTAL_DUG_UP			 FLAG_SCENE_TEMP_71
/* The ripple's step count, the byte of flags from FLAG_SCENE_TEMP_78: each
   RIPPLE_STEPS_PER_POINT steps it moves on one point of
   YampiDesertCaveRipplePath. */
#define FLAG_BYTE_YAMPI_DESERT_CAVE_RIPPLE_STEP			 FLAG_SCENE_TEMP_78
/* The steps the ripple takes at each point of its path. */
#define RIPPLE_STEPS_PER_POINT							 32

/* Set once Valukar, the guardian of fire, has been beaten; he is gone. */
#define FLAG_VALUKAR_BEATEN 0x9cd

/* The Water of Life chest's room's actors
   (SceneIdYampiDesertCaveWaterOfLifeChest), the rows of
   YampiDesertCavePlacements. */
typedef enum YAMPI_DESERT_CAVE_CHEST_ROOM_ACTOR_ID {
	/* The three fallen columns pushed as logs (PushKinds). */
	ACTOR_CHEST_ROOM_FIRST_LOG = ACTOR_FIRST_PLACED,
	ACTOR_CHEST_ROOM_SECOND_LOG,
	ACTOR_CHEST_ROOM_THIRD_LOG,
	/* The pillar Burst breaks (SPRITE_BURST_PILLAR). */
	ACTOR_CHEST_ROOM_BURST_PILLAR
} YAMPI_DESERT_CAVE_CHEST_ROOM_ACTOR_ID;

/* The chest room's two Teleport circles, the north one where the party comes in
   from the desert and the south one: the collision code of each circle's
   metatiles, and the entrance on it. Each circle sends the party to the
   other. */
typedef enum YAMPI_DESERT_CAVE_TELEPORT_CIRCLE {
	NORTH_CIRCLE_METATILES = 30,
	SOUTH_CIRCLE_METATILES = 31,
	NORTH_CIRCLE_ENTRANCE = 9,
	SOUTH_CIRCLE_ENTRANCE = 10
} YAMPI_DESERT_CAVE_TELEPORT_CIRCLE;

/* The ways between two parts of one room: each leads to its partner's entrance
   and back. */
typedef enum YAMPI_DESERT_CAVE_WAY {
	/* The Water of Life chest's room: the middle and west passages north out of
	   its south part, to the south-east and south-west feet of its north
	   part. */
	CHEST_ROOM_MIDDLE_PASSAGE = 3,
	CHEST_ROOM_WEST_PASSAGE = 4,
	CHEST_ROOM_NORTH_PART_WEST_FOOT = 5,
	CHEST_ROOM_NORTH_PART_EAST_FOOT = 6,
	/* Valukar's rooms: the stairs down in the north wall of the south cave, to
	   the stairs up at the north-east of the room with the ladder. */
	GUARDIAN_SOUTH_CAVE_STAIRS = 2,
	GUARDIAN_LADDER_ROOM_STAIRS = 17,
	/* Crystal's rooms: the west doorway of the room with the ladder and the
	   chest, to the foot of the room of fallen columns; that room's north
	   doorway and its doorway on the ledge, to the north-west room's south-east
	   foot and the north-east room's east end; the north-east room's north
	   doorway, to the north-west room's south-west foot. */
	CRYSTAL_LADDER_ROOM_WEST_DOORWAY = 2,
	CRYSTAL_LOG_ROOM_FOOT = 3,
	CRYSTAL_LOG_ROOM_NORTH_DOORWAY = 4,
	CRYSTAL_LOG_ROOM_LEDGE_DOORWAY = 5,
	CRYSTAL_NORTHEAST_ROOM_EAST_END = 6,
	CRYSTAL_NORTHWEST_ROOM_EAST_FOOT = 7,
	CRYSTAL_NORTHEAST_ROOM_NORTH_DOORWAY = 9,
	CRYSTAL_NORTHWEST_ROOM_WEST_FOOT = 10
} YAMPI_DESERT_CAVE_WAY;

/* The metatiles the rooms' cue tables answer, by the behaviour the maps give
   them: round the logs, the Water of Life chest room's pillar Burst breaks, the
   stretch of sand in Crystal's room the ripple races over (metatiles (46, 14)
   on, which Burst and Scoop both answer on), before Valukar, and over the
   buried Mythril Silver. */
typedef enum YAMPI_DESERT_CAVE_METATILE {
	METATILE_AROUND_LOG = 42,
	METATILE_BURST_PILLAR = 33,
	METATILE_RIPPLE_SAND = 33,
	METATILE_BEFORE_VALUKAR = 52,
	METATILE_MYTHRIL_SILVER = 100
} YAMPI_DESERT_CAVE_METATILE;

/* Valukar's room's actors (SceneIdYampiDesertCaveGuardian). */
typedef enum YAMPI_DESERT_CAVE_GUARDIAN_ACTOR_ID {
	/* The stone pillar Move pushes (SPRITE_STONE_PILLAR). */
	ACTOR_GUARDIAN_ROOM_PILLAR = ACTOR_FIRST_PLACED,
	/* Valukar, the guardian of fire: "I am the..." (MsgIAmThe). */
	ACTOR_VALUKAR
} YAMPI_DESERT_CAVE_GUARDIAN_ACTOR_ID;

/* Crystal's room's actors (SceneIdYampiDesertCaveCrystal). */
typedef enum YAMPI_DESERT_CAVE_CRYSTAL_ACTOR_ID {
	ACTOR_CRYSTAL_ROOM_FIRST_LOG = ACTOR_FIRST_PLACED,
	ACTOR_CRYSTAL_ROOM_SECOND_LOG,
	ACTOR_CRYSTAL_ROOM_THIRD_LOG,
	ACTOR_CRYSTAL_ROOM_FOURTH_LOG,
	/* The two stone pillars Move pushes, east and west. */
	ACTOR_EAST_PILLAR,
	ACTOR_WEST_PILLAR,
	/* The stake Pound sinks. */
	ACTOR_STAKE,
	/* The pillar Burst breaks. */
	ACTOR_CRYSTAL_ROOM_BURST_PILLAR,
	/* The ripple of sand the Djinni Crystal races under
	   (scYampiDesertCaveWonderAtRipple). */
	ACTOR_SAND_RIPPLE,
	/* Crystal, the Venus Djinni, dug out of the sand
	   (scYampiDesertCaveFightCrystal). */
	ACTOR_CRYSTAL
} YAMPI_DESERT_CAVE_CRYSTAL_ACTOR_ID;

/* The Mythril Silver buried in Valukar's room, its first scene item's treasure
   spot. */
enum { ACTOR_MYTHRIL_SILVER = FIELD_TREASURE_ACTOR };

extern Uint16 PushKinds[];

void scYampiDesertCaveStopLeaderAtLog(ACTOR *actor);

void scTeleportArriveWhirling(void);

extern Uint16 TeleportCircleShimmerSlots[];

void scYampiDesertCaveFightCrystal(void);

void scYampiDesertCaveWonderAtRipple(void);

/* Walking against one of the logs lying in the inner cave or in Crystal's room
   pushes it, and it rolls on until it meets a wall or another log. */
void scYampiDesertCaveTryPushLog(void);

void scYampiDesertCaveTeleportFromNorthCircle(void);

void scYampiDesertCaveTeleportFromSouthCircle(void);

void scYampiDesertCaveCastSand(void);
void scYampiDesertCaveEndSand(void);

extern Uint32 YampiDesertCaveSettle[];

/* The Teleport circles' shimmer (shared/teleport_shimmer.c). */
Uint16 PaletteCycleBright		  BSS_DATA;
Uint16 *PaletteCycleSlots		  BSS_DATA;
Uint16 PaletteCycleTimer		  BSS_DATA;
Uint16 PaletteCycleDelay BSS_DATA ALIGNED(4);
Uint16 *PaletteSlots			  BSS_DATA;
Uint16 *PaletteColors			  BSS_DATA;
Uint16 *PaletteCycleColors		  BSS_DATA;
Uint16 PaletteCycleLength		  BSS_DATA;
Uint16 PaletteCycleIndex BSS_DATA ALIGNED(4);

extern Sint8 PushRectangles[][4];

/* Makes a log's metatiles solid where it lies: its rectangle's metatiles take
   the raised layers' attributes and block the ground; returns 0 for an actor
   that is no log. */
Bool scYampiDesertCaveSetLogMetatiles(Sint32 id)
{
	MAP_STATE *map = RamHeapSlots->mapGrid;
	ACTOR	  *actor = fdActorGetById(id);
	Sint16	   sprite = ((SPR *)actor->sprite)->entries[0]->animationId;
	Sint16	   i;
	Uint32	   kind;
	Sint32	   w, h, x, z;
	Sint32	   az;
	Sint32	   bx, bz;

	i = 0;
	if((Uint16)sprite != PushKinds[i]) {
		do {
			/* FAKEMATCH(dead): the counter's halfword step written out by hand;
			 * as a Uint16 counter in for (i = 0; i < PUSH_KINDS; i++) with the
			 * rectangle's top and the middle layer's x in an Sint16 of their
			 * own, 22 lines off. */
			kind = (Uint16)NEXT_HALFWORD(i);
			i = kind;
		} while(kind < PUSH_KINDS && (Uint16)sprite != PushKinds[kind]);
	}
	kind = (Uint16)i;
	if(kind == PUSH_KINDS)
		return FALSE;
	w = (ABS(PushRectangles[kind][0]) + ABS(PushRectangles[kind][2])) >> 4;
	/* i is reused: the rectangle's top here, the middle layer's x offset in
	   metatiles below. */
	i = PushRectangles[kind][1];
	h = (ABS(i) + ABS(PushRectangles[kind][3])) >> 4;
	x = FIELD_METATILE(actor->x.fixed + toFIXED(PushRectangles[kind][0]));
	z = FIELD_METATILE(actor->z.fixed + toFIXED(i));
	i = FIELD_METATILE(map->layers[MAPGRID_LAYER_MIDDLE].offsetX);
	az = FIELD_METATILE(map->layers[MAPGRID_LAYER_MIDDLE].offsetY);
	bx = map->layers[MAPGRID_LAYER_OVERHEAD].offsetX;
	bz = map->layers[MAPGRID_LAYER_OVERHEAD].offsetY;
	fdMapGridCopyMetatileAttributes(x, z, w, h, (Uint16)i + x, (Uint16)az + z);
	fdMapGridCopyMetatileAttributes((Uint16)FIELD_METATILE(bx) + x,
		(Uint16)FIELD_METATILE(bz) + z, w, h, x + 64, z + 64);
	fdMapGridFillCollisionRectangle(
		MAPGRID_LAYER_GROUND, x, z, w, h, METATILE_BEHAVIOR_WALL);
	fdMapGridFillCollisionRectangle(
		MAPGRID_LAYER_OVERHEAD, x, z, w, h, METATILE_BEHAVIOR_WALL);
	return TRUE;
}

#include "../field/map_metatile_fill.c"

extern Sint32 PushLeaderSteps[];

/* Where a pushed log comes to rest, found before the leader pushes it. */
typedef struct LOG_ROLL {
	FIXED  x, y, z;	 /* the log's position at the end of the roll */
	Sint32 kind;	 /* its row of PushRectangles */
	Sint32 slot;	 /* its actor id */
	void  *onPushed; /* run as the log rolls, if not NULL */
} LOG_ROLL;

/* Finds the log the leader faces and can push: its kind, its actor slot and the
   direction, or NULL. */
ACTOR *scYampiDesertCaveFindPushedLog(
	Sint32 *direction, Sint32 *slot, Sint32 *kind);

/* What fdMapGridCheckCollision says of a step to a position: the system call
   layer's inline, as The Broken Seal's system_call.h has it, which this game's
   does not yet. */
static inline Sint32 scFieldActorCheckMovementCollision(
	ACTOR *actor, Sint32 *pos)
{
	return fdMapGridCheckCollision(actor, pos);
}

/* Finds how far a pushed object can roll in the leader's direction, probing its
   rectangle metatile by metatile until fdMapGridCheckCollision reports a wall.
   Returns 1 with the log's end position in roll, or 0 if it cannot move. */
Bool scYampiDesertCaveFindLogRoll(LOG_ROLL *roll)
{
	FIXED  probe[3];
	Sint32 direction;
	ACTOR *actor;
	Sint32 kind, w, h, dx;
	FIXED  dz;
	Sint32 count, i, j;
	Uint8 *layer;

	actor =
		scYampiDesertCaveFindPushedLog(&direction, &roll->slot, &roll->kind);
	if(actor != NULL) {
		kind = roll->kind;
		w = (ABS(PushRectangles[kind][0]) + ABS(PushRectangles[kind][2])) >> 4;
		h = (ABS(PushRectangles[kind][1]) + ABS(PushRectangles[kind][3])) >> 4;
		dx = PushLeaderSteps[direction] & FIELD_PIXEL_MASK;
		dz = toFIXED(PushLeaderSteps[direction]);
		roll->onPushed = NULL;
		count = 0;
		layer = &actor->layer;
		*layer = MAPGRID_LAYER_OVERHEAD;
		roll->x = actor->x.fixed + dx;
		roll->z = actor->z.fixed + dz;
		probe[1] = actor->y.fixed;
		do {
			probe[2] = roll->z + toFIXED(PushRectangles[kind][1]);
			for(j = 0; j < h; j++) {
				probe[0] = roll->x + toFIXED(PushRectangles[kind][0]);
				for(i = 0; i < w; i++) {
					if(scFieldActorCheckMovementCollision(actor, probe) ==
						COLLISION_IMPASSABLE)
						goto blocked;
					probe[0] += toFIXED(MAPGRID_METATILE_PIXELS);
				}
				probe[2] += toFIXED(MAPGRID_METATILE_PIXELS);
			}
			roll->x += dx;
			count++;
			roll->z += dz;
		} while(1);
	blocked:
		*layer = MAPGRID_LAYER_GROUND;
		if(count == 0)
			return FALSE;
		roll->x = actor->x.fixed + dx * count;
		roll->y = actor->y.fixed;
		roll->z = actor->z.fixed + dz * count;
		return TRUE;
	} else {
		return FALSE;
	}
}

ACTOR *scYampiDesertCaveFindPushedLog(
	Sint32 *direction, Sint32 *slot, Sint32 *kind)
{
	FIELD_STATE *state = RamHeapSlots->fieldState;
	ACTOR		*leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	ACTOR	   **entry;
	ACTOR		*actor;
	Uint32		 i, k;
	Sint32		 x, z, left, top, right, bottom;

	*direction = FACING_INDEX(leader->facing);
	i = ACTOR_FIRST_PLACED;
	entry = &state->actors[ACTOR_FIRST_PLACED];
	for(; i < FIELD_TREASURE_ACTOR; i++) {
		actor = *entry;
		for(k = 0; k < PUSH_KINDS; k++) {
			if(((SPR *)actor->sprite)->entries[0]->animationId ==
				PushKinds[k]) {
				x = (leader->x.part[0].integer +
						PUSH_STEP_X(PushLeaderSteps[*direction])) >>
					4;
				z = (leader->z.part[0].integer +
						PUSH_STEP_Z(PushLeaderSteps[*direction])) >>
					4;
				left = (actor->x.part[0].integer + PushRectangles[k][0]) >> 4;
				top = (actor->z.part[0].integer + PushRectangles[k][1]) >> 4;
				right = (actor->x.part[0].integer + PushRectangles[k][2]) >> 4;
				bottom = (actor->z.part[0].integer + PushRectangles[k][3]) >> 4;
				if(left <= x && x < right && top <= z && z < bottom) {
					*kind = k;
					if(k & 1) {
						if(left != FIELD_METATILE(leader->x.fixed)) {
							*slot = i;
							return actor;
						}
					} else {
						if(top != FIELD_METATILE(leader->z.fixed)) {
							*slot = i;
							return actor;
						}
					}
				}
			}
		}
		entry++;
	}
	return NULL;
}

extern ACTOR_PLACE YampiDesertCavePlacements[];
extern ACTOR_PLACE YampiDesertCaveGuardianPlacements[];
extern ACTOR_PLACE YampiDesertCaveCrystalPlacements[];
extern Uint16	   YampiDesertCaveCrystalStakes[];
extern CUE		   YampiDesertCaveCues[];
extern CUE		   YampiDesertCaveGuardianCues[];
extern CUE		   YampiDesertCaveCrystalCues[];

Bool scYampiDesertCaveFindLogRoll(LOG_ROLL *roll);
/* The leader pushes a log to where scYampiDesertCaveFindLogRoll stopped it, and
   its metatiles move with it. */
void   scYampiDesertCavePushLog(LOG_ROLL roll);
void   scTeleportStartShimmer(void);
void   scTeleportStopShimmer(void);
Sint32 syListCountEntries(Uint16 *list);
void   fdPaletteCycleFade(void);
void   fdPoundSinkTarget(Uint16 *list, Sint32 mode, Sint32 count);
void   fdTeleportPlaySparks(void);

void scYampiDesertCavePushLog(LOG_ROLL roll)
{
	MAP_STATE *map = RamHeapSlots->mapGrid;
	Sint32	   w, h;
	ACTOR	  *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	ACTOR	  *actor = fdActorGetById(roll.slot);
	/* BUG: the leader's facing is taken as a 16.16 number, whose whole part is
	   always 0 for this 16-bit angle, where its sixteenth (FACING_INDEX) was
	   meant: every push takes direction 0 (east), so the log rolls with
	   animation 2 whichever way it goes and the leader's step after it is
	   none. */
#if FIX_PUSH_FACING_AND_LEADER_STEP
	Sint32 direction = FACING_INDEX(leader->facing);
#else
	Sint32 direction = FIXEDtoINT(leader->facing);
#endif
	Sint32 kind = roll.kind;
	Sint32 speed = leader->speedLimit;
	Sint32 acceleration = leader->acceleration;
	Sint32 metatile[3];
	FIXED  from[3];
	Sint32 ax, az, bx, bz;

	from[0] = actor->x.fixed;
	from[2] = actor->z.fixed;
	metatile[0] =
		FIELD_METATILE(actor->x.fixed + toFIXED(PushRectangles[kind][0]));
	metatile[2] =
		FIELD_METATILE(actor->z.fixed + toFIXED(PushRectangles[kind][1]));
	w = (ABS(PushRectangles[kind][0]) + ABS(PushRectangles[kind][2])) >> 4;
	h = (ABS(PushRectangles[kind][1]) + ABS(PushRectangles[kind][3])) >> 4;
	fdMapGridFillCollisionRectangle(MAPGRID_LAYER_GROUND, metatile[0],
		metatile[2], w, h, METATILE_BEHAVIOR_NORMAL);
	fdActorSetSpeedAndAcceleration(
		MainRoutineStateWork.leaderActorId, 0.5, 0.1);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_PUSH);
	sySynch(15);
	/* The leader follows the log half its way. */
	fdActorSetDestinationOffset(MainRoutineStateWork.leaderActorId,
		FIXEDtoINT(roll.x - from[0]) >> 1, FIXEDtoINT(roll.z - from[2]) >> 1);
	fdActorGetById(MainRoutineStateWork.leaderActorId)->update =
		scYampiDesertCaveStopLeaderAtLog;
	sySynch(4);
	if(direction >= FACING_INDEX(FACING_SOUTHWEST) &&
		direction <= FACING_INDEX(FACING_NORTH_NORTHEAST))
		fdActorSelectAnimation(actor, 3);
	else
		fdActorSelectAnimation(actor, 2);
	fdScriptPlaySound(SE_HEAVY_MOVE);
	fdActorSetSpeedAndAcceleration(roll.slot, 0.5, 0.1);
	fdActorSetMoveTarget(actor, roll.x, roll.y, roll.z);
	fdActorWaitForMoveById(MainRoutineStateWork.leaderActorId);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_WALK);
	fdActorSetSpeedAndAcceleration(
		MainRoutineStateWork.leaderActorId, 0.3, 0.1);
	/* BUG: the leader's step after the push reads both halves from the step's
	   z: x is its sign and z its half (half of PUSH_STEP_X and PUSH_STEP_Z was
	   meant). With direction 0 both are 0. */
	{
		Sint32 step = PushLeaderSteps[direction];

#if FIX_PUSH_FACING_AND_LEADER_STEP
		fdActorSetDestinationOffset(MainRoutineStateWork.leaderActorId,
			PUSH_STEP_X(step) / 2, PUSH_STEP_Z(step) / 2);
#else
		fdActorSetDestinationOffset(MainRoutineStateWork.leaderActorId,
			toFIXED(step) >> 31, FIXEDtoINT(toFIXED(step)) >> 1);
#endif
	}
	if(roll.onPushed != NULL)
		((void (*)(void))roll.onPushed)();
	fdActorWaitForMoveById(MainRoutineStateWork.leaderActorId);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_STAND);
	leader->update = NULL;
	leader->speedLimit = speed;
	leader->acceleration = acceleration;
	scFieldActorWaitForMove(actor);
	fdScriptPlaySound(SE_STOP_EXIT_SE);
	fdScriptPlaySound(SE_HEAVY_STOP);
	fdActorSetPosAndResetMotion(actor, roll.x, actor->y.fixed, roll.z);
	fdActorSelectAnimation(actor, 1);
	roll.x = FIELD_METATILE(roll.x + toFIXED(PushRectangles[kind][0]));
	roll.z = FIELD_METATILE(roll.z + toFIXED(PushRectangles[kind][1]));
	ax = FIELD_METATILE(map->layers[MAPGRID_LAYER_MIDDLE].offsetX);
	az = FIELD_METATILE(map->layers[MAPGRID_LAYER_MIDDLE].offsetY);
	bx = FIELD_METATILE(map->layers[MAPGRID_LAYER_OVERHEAD].offsetX);
	bz = FIELD_METATILE(map->layers[MAPGRID_LAYER_OVERHEAD].offsetY);
	fdMapGridCopyMetatileAttributes(
		roll.x, roll.z, w, h, ax + roll.x, az + roll.z);
	fdMapGridCopyMetatileAttributes(
		roll.x + bx, roll.z + bz, w, h, roll.x + 64, roll.z + 64);
	fdMapGridFillCollisionRectangle(
		MAPGRID_LAYER_GROUND, roll.x, roll.z, w, h, METATILE_BEHAVIOR_WALL);
	fdMapGridFillCollisionRectangle(
		MAPGRID_LAYER_OVERHEAD, roll.x, roll.z, w, h, METATILE_BEHAVIOR_WALL);
	metatile[0] = FIELD_METATILE(from[0] + toFIXED(PushRectangles[kind][0]));
	metatile[2] = FIELD_METATILE(from[2] + toFIXED(PushRectangles[kind][1]));
	fdMapGridCopyMetatileAttributes(
		ax + metatile[0], az + metatile[2], w, h, metatile[0], metatile[2]);
	fdMapGridCopyMetatileAttributes(metatile[0] + 64, metatile[2] + 64, w, h,
		metatile[0] + bx, metatile[2] + bz);
	fdScriptNoOperation();
}

/* The leader's update while pushing: it stops at a wall or another log. */
void scYampiDesertCaveStopLeaderAtLog(ACTOR *actor)
{
	FIXED  pos[3];
	ACTOR *hit;
	Uint32 i;
	Sint32 angle = FACING_INDEX(actor->facing);

	/* The facing rounded to the nearest of east, south, west and north: two
	   sixteenths on, whole quarters kept, and back from sixteenths to a
	   facing. */
	angle = FACING_FROM_INDEX((angle + 2) & 0xc);
	pos[0] = actor->x.fixed;
	pos[1] = actor->y.fixed;
	pos[2] = actor->z.fixed;
	syVectorAddPolarOffset(toFIXED(MAPGRID_METATILE_PIXELS), angle, pos);
	hit = fdActorFindAtMetatile(pos, 1);
	if(hit != NULL) {
		for(i = 0; i < PUSH_KINDS; i++) {
			if(((SPR *)hit->sprite)->entries[0]->animationId == PushKinds[i])
				return;
		}
		fdActorSetPosAndResetMotion(
			actor, actor->x.fixed, actor->y.fixed, actor->z.fixed);
	}
	pos[0] = actor->x.fixed;
	pos[1] = actor->y.fixed;
	pos[2] = actor->z.fixed;
	if(fdMapGridCheckCollision(actor, pos) > 0)
		fdActorSetPosAndResetMotion(
			actor, actor->x.fixed, actor->y.fixed, actor->z.fixed);
}

#include "shared/teleport_shimmer.c"

void fdActorSetMetatileOpen(Sint32 id, Uint32 kind, Sint32 flag, Sint32 layer);

#include "shared/pillar_place.h"

/* Finds the current target's table row and shrinks it. */
#include "shared/pound_sink_target.h"

#include "shared/actor_set_metatile_open.h"

/* Talking to Crystal, the Venus Djinni Scoop has dug out of the sand, starts
   its battle. */
void scYampiDesertCaveFightCrystal(void)
{
	fdScriptSetFlag(FLAG_YAMPI_DESERT_CAVE_CRYSTAL_DUG_UP);
	fdScriptRequestDjinnBattle(ACTOR_CRYSTAL, DJINNI_CRYSTAL);
	return;
}

/* Talking to the ripple racing through the sand of Crystal's room only puts a
   question mark over Felix's head. */
void scYampiDesertCaveWonderAtRipple(void)
{
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	scFieldActorShowEmote(MainRoutineStateWork.leaderActorId,
		EMOTE_IN_FRONT | EMOTE_QUESTION, 32);
	fdScriptEnd();
	return;
}

const SCENE_ENTRANCE *scGetEntrances(void)
{
	return SceneEntrances;
}

const SCENE_REGION *scGetExitRegions(void)
{
	return NULL;
}

const Uint32 *scGetExits(void)
{
	return SceneExits;
}

const ACTOR_PLACE *scGetActorPlacements(void)
{
	if(MainRoutineStateWork.next == SCENE_YAMPI_DESERT_CAVE_GUARDIAN)
		return YampiDesertCaveGuardianPlacements;
	if(MainRoutineStateWork.next == SCENE_YAMPI_DESERT_CAVE_CRYSTAL)
		return YampiDesertCaveCrystalPlacements;
	return YampiDesertCavePlacements;
}

void scYampiDesertCaveTryPushLog(void)
{
	LOG_ROLL roll;

	if(scYampiDesertCaveFindLogRoll(&roll))
		scYampiDesertCavePushLog(roll);
}

/* Teleport on one of the chest room's circles: the circle shimmers and the
   party arrives on the other, by its entrance. */
#define DEFINE_TELEPORT_FROM_CIRCLE(name, entrance) \
	void name(void) \
	{ \
		scTeleportStartShimmer(); \
		fdTeleportPlaySparks(); \
		MainRoutineStateWork.teleportState = TELEPORT_ARRIVING; \
		fdScriptSetDestination( \
			SCENE_YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST, entrance); \
		fdScriptWait(40); \
		scTeleportStopShimmer(); \
	}

/* Teleport cast on the pale carved circle in the small round chamber at the
   north of the cave's first room (metatiles (14, 7) to (17, 9)) makes the
   circle shimmer and carries the party to the circle in the inner cave. */
DEFINE_TELEPORT_FROM_CIRCLE(
	scYampiDesertCaveTeleportFromNorthCircle, SOUTH_CIRCLE_ENTRANCE)

/* Teleport cast on the carved circle in the inner cave (metatiles (21, 56) to
   (24, 58)) makes it shimmer and carries the party back to the circle in the
   small chamber of the cave's first room, near the Water of Life's chest. */
DEFINE_TELEPORT_FROM_CIRCLE(
	scYampiDesertCaveTeleportFromSouthCircle, NORTH_CIRCLE_ENTRANCE)

/* Burst breaks a pillar for good (flag) as it acts (phase 3). */
#define DEFINE_BURST_PILLAR(name, flag, actorId) \
	void name(Sint32 phase) \
	{ \
		if(!fdScriptGetFlag(flag) && phase == 3) { \
			fdActorSetPos(actorId, 0, 0); \
			syGameFlagSet(flag); \
		} \
	}

/* Burst on the chest room's pillar breaks it for good. The pillar is taken away
   and `FLAG_YAMPI_DESERT_CAVE_CHEST_ROOM_PILLAR_BURST` is set. */
DEFINE_BURST_PILLAR(YampiDesertCaveBurstChestRoomPillar,
	FLAG_YAMPI_DESERT_CAVE_CHEST_ROOM_PILLAR_BURST,
	ACTOR_CHEST_ROOM_BURST_PILLAR)

/* Burst on the pillar in Crystal's room breaks it for good. The pillar is taken
   away and `FLAG_YAMPI_DESERT_CAVE_CRYSTAL_ROOM_PILLAR_BURST` is set. */
DEFINE_BURST_PILLAR(YampiDesertCaveBurstCrystalRoomPillar,
	FLAG_YAMPI_DESERT_CAVE_CRYSTAL_ROOM_PILLAR_BURST,
	ACTOR_CRYSTAL_ROOM_BURST_PILLAR)

/* Move takes a pillar: the metatiles it stood on open, as the room has them
   without it. */
void scYampiDesertCaveStartMovingPillar(Sint32 unused, Sint32 id)
{
	if(MainRoutineStateWork.next == SCENE_YAMPI_DESERT_CAVE_GUARDIAN) {
		Sint32 row = 12;

		fdMapGridCopyMetatileAttributes(98, 3, 3, 3, 97, row);
		fdMapGridCopyMetatileAttributes(33, 12, 1, 1, 34, row);
	}
	if(MainRoutineStateWork.next == SCENE_YAMPI_DESERT_CAVE_CRYSTAL) {
		if(id == ACTOR_EAST_PILLAR) {
			fdMapGridCopyMetatileAttributes(102, 15, 1, 1, 103, 15);
			fdMapGridCopyMetatileAttributes(43, 14, 1, 1, 39, 16);
		}
		if(id == ACTOR_WEST_PILLAR) {
			ACTOR *actor = fdActorGetById(ACTOR_WEST_PILLAR);

			Sint32 column = FIELD_METATILE(actor->x.fixed);
			Sint32 row = FIELD_METATILE(actor->z.fixed);

			fdMapGridCopyMetatileAttributes(7, 10, 1, 1, column, row);
		}
	}
}

/* Move sets a pillar down: where it opens the way it stays for good, and the
   metatiles under it close again. */
void scYampiDesertCaveFinishMovingPillar(Sint32 unused, Sint32 id)
{
	ACTOR *actor = fdActorGetById(id);

	if(MainRoutineStateWork.next == SCENE_YAMPI_DESERT_CAVE_GUARDIAN) {
		Sint32 row = FIELD_METATILE(actor->z.fixed);

		if(row == 13) {
			actor->motionFlags = 0;
			fdScriptSetFlag(FLAG_YAMPI_DESERT_CAVE_GUARDIAN_PILLAR_MOVED);
			fdMapGridCopyMetatileAttributes(32, 13, 1, 1, 34, row);
			fdMapGridCopyMetatileAttributes(95, 3, 3, 3, 97, 12);
		} else {
			row = 12;
			fdMapGridCopyMetatileAttributes(92, 3, 3, 3, 97, row);
			fdMapGridCopyMetatileAttributes(32, 13, 1, 1, 34, row);
		}
	}
	if(MainRoutineStateWork.next == SCENE_YAMPI_DESERT_CAVE_CRYSTAL) {
		if(id == ACTOR_EAST_PILLAR) {
			Sint32 column;

			fdMapGridCopyMetatileAttributes(103, 16, 1, 1, 103, 15);
			column = FIELD_METATILE(actor->x.fixed);
			if(column == 38) {
				fdScriptSetFlag(FLAG_YAMPI_DESERT_CAVE_EAST_PILLAR_MOVED);
				fdMapGridCopyMetatileAttributes(38, 15, 1, 1, column, 16);
			} else {
				fdMapGridCopyMetatileAttributes(38, 15, 1, 1, 39, 16);
			}
		}
		if(id == ACTOR_WEST_PILLAR) {
			if(FIELD_METATILE(actor->x.fixed) == 10 &&
				FIELD_METATILE(actor->z.fixed) == 11)
				fdScriptSetFlag(FLAG_YAMPI_DESERT_CAVE_WEST_PILLAR_MOVED);
			fdMapGridCopyMetatileAttributes(12, 11, 1, 1,
				FIELD_METATILE(actor->x.fixed), FIELD_METATILE(actor->z.fixed));
		}
	}
}

/* As Sand melts Felix into the sand, the ways only a sunk Felix can take open
   in the Water of Life chest's room and in Crystal's room. */
void scYampiDesertCaveCastSand(void)
{
	fdActorGetById(MainRoutineStateWork.leaderActorId)->layer =
		MAPGRID_LAYER_OVERHEAD;
	if(MainRoutineStateWork.next ==
		SCENE_YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST) {
		fdMapGridCopyMetatileAttributes(55, 71, 4, 1, 43, 70);
		fdMapGridCopyMetatileAttributes(43, 71, 4, 1, 55, 71);
	}
	if(MainRoutineStateWork.next == SCENE_YAMPI_DESERT_CAVE_CRYSTAL) {
		fdMapGridCopyMetatileAttributes(110, 45, 5, 2, 90, 50);
		fdMapGridCopyMetatileAttributes(113, 47, 2, 1, 90, 52);
		fdMapGridCopyMetatileAttributes(107, 48, 3, 2, 90, 53);
		fdMapGridCopyMetatileAttributes(97, 58, 5, 2, 110, 45);
		fdMapGridCopyMetatileAttributes(97, 58, 2, 1, 113, 47);
		fdMapGridCopyMetatileAttributes(97, 58, 3, 2, 107, 48);
	}
}

/* As Sand fades and Felix rises out of the sand, those ways close again. */
void scYampiDesertCaveEndSand(void)
{
	fdActorGetById(MainRoutineStateWork.leaderActorId)->layer =
		MAPGRID_LAYER_GROUND;
	if(MainRoutineStateWork.next == SCENE_YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST)
		fdMapGridCopyMetatileAttributes(43, 70, 4, 1, 55, 71);
	if(MainRoutineStateWork.next == SCENE_YAMPI_DESERT_CAVE_CRYSTAL) {
		fdMapGridCopyMetatileAttributes(90, 50, 5, 2, 110, 45);
		fdMapGridCopyMetatileAttributes(90, 52, 2, 1, 113, 47);
		fdMapGridCopyMetatileAttributes(90, 53, 3, 2, 107, 48);
	}
}

/* Pound on the wooden stake in Crystal's room hammers it into the ground and
   opens the way past it. */
void scYampiDesertCavePoundStake(Sint32 mode, Sint32 count)
{
	fdPoundSinkTarget(YampiDesertCaveCrystalStakes, mode, count);
	fdMapGridCopyMetatileAttributes(47, 12, 1, 1, 46, 12);
}

extern Sint16 YampiDesertCaveRipplePath[];

/* Moves the ripple along its path, a row of YampiDesertCaveRipplePath for
   every RIPPLE_STEPS_PER_POINT steps of its counter, which counts on while the
   player has control, whether the party walks or stands. */
void scYampiDesertCaveMoveSandRipple(void)
{
	Sint32 step =
		scFieldGameFlagGetByte(FLAG_BYTE_YAMPI_DESERT_CAVE_RIPPLE_STEP);
	Sint32 index = (step / RIPPLE_STEPS_PER_POINT) * 2;
	Sint32 column = YampiDesertCaveRipplePath[index];
	Sint32 row = YampiDesertCaveRipplePath[index + 1];

	if(fdActorGetById(ACTOR_SAND_RIPPLE)->paused == 0) {
		fdActorSetPosFX(ACTOR_SAND_RIPPLE, FIELD_HALF_METATILE_ORIGIN(column),
			FIELD_HALF_METATILE_ORIGIN(row));
		if(fdScriptGetFlag(FLAG_FIELD_HANDLING_REQUESTS) == 0)
			step++;
		syGameFlagSetByte(FLAG_BYTE_YAMPI_DESERT_CAVE_RIPPLE_STEP, step);
	}
}

/* Scoop cast right beside the ripple racing through the sand of Crystal's room
   stops it and digs up Crystal, the Venus Djinni, who drops to the ground. The
   ripple is stopped as the ground is dug (SCOOP_STAGE_DUG) and Crystal set down
   as she comes up (SCOOP_STAGE_DUG_UP). */
void scYampiDesertCaveScoopDjinni(Sint32 mode)
{
	Sint32 index =
		(((Sint32)syGameFlagGetByte(FLAG_BYTE_YAMPI_DESERT_CAVE_RIPPLE_STEP)) /
			RIPPLE_STEPS_PER_POINT) *
		2;
	Sint16		 column = YampiDesertCaveRipplePath[index];
	Sint32		 px = column * (MAPGRID_METATILE_PIXELS / 2);
	Sint16		 row = YampiDesertCaveRipplePath[index + 1];
	Sint32		 pz = row * (MAPGRID_METATILE_PIXELS / 2);
	FIELD_STATE *state = RamHeapSlots->fieldState;
	/* The Scoop's hand, where it digs. */
	ACTOR *scoopHand = ((SCOOP_STATE *)RamHeapSlots->battleEffect)->actor;
	Sint32 x = 0, z = 0;

	if(scoopHand != NULL) {
		x = FIELD_METATILE(scoopHand->x.fixed);
		z = FIELD_METATILE(scoopHand->z.fixed);
	}
	if(px / MAPGRID_METATILE_PIXELS != x || pz / MAPGRID_METATILE_PIXELS != z)
		return;
	if(mode == SCOOP_STAGE_DUG) {
		syCloseEvent(scYampiDesertCaveMoveSandRipple);
		fdActorSetPos(ACTOR_SAND_RIPPLE, 0, 0);
	}
	if(mode == SCOOP_STAGE_DUG_UP) {
		ACTOR *crystal = fdActorGetById(ACTOR_CRYSTAL);

		fdActorSetPosFX(ACTOR_CRYSTAL, FIELD_HALF_METATILE_ORIGIN(column),
			FIELD_HALF_METATILE_ORIGIN(row));
		crystal->y.fixed = toFIXED(32);
		crystal->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
		crystal->gravity = toFIXED(0.09);
		sySynch(50);
		/* Felix speaks to Crystal at once, which starts its battle
		   (scYampiDesertCaveFightCrystal). */
		{
			Sint16 *talk = &state->talk;
			Sint32	value = ACTOR_CRYSTAL;

			*talk = value;
		}
#if SCOOP_DIGS_UP_CRYSTAL_ONCE
		fdScriptClearFlag(FLAG_YAMPI_DESERT_CAVE_RIPPLE_RUNNING);
#endif
	}
}

/* Stepping up to Valukar in his chamber (metatiles (11, 11) to (13, 11)), the
   screen glows warm as he says "I am the guardian of fire. If you want my
   power, prove yourself in my crucible of flame.", then he comes at Felix and
   his battle begins, one the party cannot run from. */
void scYampiDesertCaveMeetGuardianOfFire(void)
{
	ACTOR *leader;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdActorStartRepeatedMotion(ACTOR_VALUKAR, 2);
	fdScriptWait(20);
	fdPaletteFadeApplySource(PALETTE_TINT_NONE, 0);
	fdPaletteFadeSetTarget(PALETTE_TINT_WARM, 1);
	fdPaletteFadeStartInterpolation(60);
	fdScriptWait(60);
	/* "I am the guardian of fire. If you want my power, prove yourself in my
	   crucible of flame." */
	syScriptMessageShowCentered((Sint32)MsgIAmThe, 0, 6);
	fdPaletteFadeSetTarget(PALETTE_TINT_NONE, 0);
	fdPaletteFadeStartInterpolation(60);
	fdScriptWait(60);
	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	if(leader != 0)
		fdActorSetDestination(ACTOR_VALUKAR, leader->x.part[0].integer,
			leader->z.part[0].integer);
	fdScriptWait(20);
	MainRoutineStateWork.pendingDjinni = IF_FLAG_SET(FLAG_VALUKAR_BEATEN);
	SET_BATTLE_TYPE(BATTLE_TYPE_NO_ESCAPE);
	MainRoutineStateWork.battleBg = (Sint32)&ResourceIdLavaCaveBackdrop;
	fdRequestBattleFromEncounterTable(
		ENCOUNTER_ZONE_HIDDEN_BOSSES, HIDDEN_BOSS_VALUKAR);
}

/* Scoop at the west end of a passage in the part of the inner cave below
   Valukar's chamber (metatile (5, 38)) digs up the Mythril Silver: "Felix got
   some Mythril Silver." */
void scYampiDesertCaveDigUpMythrilSilver(Sint32 mode)
{
	fdScriptDigUpTreasure(mode, ACTOR_MYTHRIL_SILVER,
		FLAG_YAMPI_DESERT_CAVE_MYTHRIL_SILVER_DUG_UP);
}

const CUE *scGetCues(void)
{
	if(MainRoutineStateWork.next == SCENE_YAMPI_DESERT_CAVE_GUARDIAN)
		return YampiDesertCaveGuardianCues;
	if(MainRoutineStateWork.next == SCENE_YAMPI_DESERT_CAVE_CRYSTAL)
		return YampiDesertCaveCrystalCues;
	return YampiDesertCaveCues;
}

extern Uint16 TeleportCirclePaletteSlots[];
extern Uint16 TeleportCircleColors[];
extern Uint16 TeleportCircleShimmerColors[];

/* Sets up Yampi Desert Cave's three rooms as each one loads: the first room's
   palette cycle and Retreat destination, the third room's background
   priorities, and in each room the objects its flags place. */
Sint32 scInit(void)
{
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	fdEffectStartBlankOverlay(0);
	if(MainRoutineStateWork.next ==
		SCENE_YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST) {
		scPaletteSetColorsFade(TeleportCirclePaletteSlots, TeleportCircleColors,
			TeleportCircleShimmerColors, TeleportCircleShimmerSlots);
		fdActorDisableById(ACTOR_CHEST_ROOM_FIRST_LOG);
		fdActorDisableById(ACTOR_CHEST_ROOM_SECOND_LOG);
		fdActorDisableById(ACTOR_CHEST_ROOM_THIRD_LOG);
		scYampiDesertCaveSetLogMetatiles(ACTOR_CHEST_ROOM_FIRST_LOG);
		scYampiDesertCaveSetLogMetatiles(ACTOR_CHEST_ROOM_SECOND_LOG);
		scYampiDesertCaveSetLogMetatiles(ACTOR_CHEST_ROOM_THIRD_LOG);
		if(fdScriptGetFlag(FLAG_YAMPI_DESERT_CAVE_CHEST_ROOM_PILLAR_BURST)) {
			fdActorSetPos(ACTOR_CHEST_ROOM_BURST_PILLAR, 0, 0);
			fdMapGridCopyMetatileAttributes(32, 13, 1, 1, 34, 13);
		}
		if(MainRoutineStateWork.entrance ==
			YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST_FROM_YAMPI_DESERT_TRAINERS_WHIP_CHEST) {
			if(fdScriptGetFlag(
				   FLAG_YAMPI_DESERT_ENTERED_BY_GUARDIAN_RING_CHEST)) {
				MainRoutineStateWork.retreatScene =
					SCENE_YAMPI_DESERT_GUARDIAN_RING_CHEST;
				MainRoutineStateWork.retreatEntrance =
					YAMPI_DESERT_GUARDIAN_RING_CHEST_FROM_WORLD_MAP;
			} else if(fdScriptGetFlag(
						  FLAG_YAMPI_DESERT_ENTERED_BY_TRAINERS_WHIP_CHEST)) {
				MainRoutineStateWork.retreatScene =
					SCENE_YAMPI_DESERT_TRAINERS_WHIP_CHEST;
				MainRoutineStateWork.retreatEntrance =
					YAMPI_DESERT_TRAINERS_WHIP_CHEST_FROM_WORLD_MAP;
			} else if(fdScriptGetFlag(
						  FLAG_YAMPI_DESERT_ENTERED_BY_HIDDEN_PSYNERGY_STONE)) {
				MainRoutineStateWork.retreatScene =
					SCENE_YAMPI_DESERT_HIDDEN_PSYNERGY_STONE;
				MainRoutineStateWork.retreatEntrance =
					YAMPI_DESERT_HIDDEN_PSYNERGY_STONE_FROM_WORLD_MAP;
			}
		}
	}
	if(MainRoutineStateWork.next == SCENE_YAMPI_DESERT_CAVE_GUARDIAN) {
		if(fdScriptGetFlag(FLAG_YAMPI_DESERT_CAVE_GUARDIAN_PILLAR_MOVED)) {
			fdActorSetPos(ACTOR_GUARDIAN_ROOM_PILLAR, 552, 216);
			fdMapGridCopyMetatileAttributes(32, 13, 1, 1, 34, 13);
			fdMapGridCopyMetatileAttributes(95, 3, 3, 3, 98, 13);
		} else {
			fdMapGridCopyMetatileAttributes(32, 13, 1, 1, 34, 12);
		}
		if(!fdScriptGetFlag(FLAG_YAMPI_DESERT_CAVE_MYTHRIL_SILVER_DUG_UP))
			fdActorSetWideSprite(ACTOR_MYTHRIL_SILVER, TREASURE_SPOT_WIDE);
	}
	if(MainRoutineStateWork.next == SCENE_YAMPI_DESERT_CAVE_CRYSTAL) {
		/* In the room of fallen columns, by any of its three ways in, the
		   walls' layer (BG2) sinks to the ground's priority, under every actor,
		   and the overhead layer stays over them. */
		if(MainRoutineStateWork.entrance >= CRYSTAL_LOG_ROOM_FOOT &&
			MainRoutineStateWork.entrance <= CRYSTAL_LOG_ROOM_LEDGE_DOORWAY) {
			REG_BG1CNT &= ~BGCNT_PRIORITY(3);
			REG_BG1CNT |= BGCNT_PRIORITY(1);
			REG_BG2CNT &= ~BGCNT_PRIORITY(3);
			REG_BG2CNT |= BGCNT_PRIORITY(3);
			REG_BG3CNT &= ~BGCNT_PRIORITY(3);
			REG_BG3CNT |= BGCNT_PRIORITY(3);
		}
		fdActorDisableById(ACTOR_CRYSTAL_ROOM_FIRST_LOG);
		fdActorDisableById(ACTOR_CRYSTAL_ROOM_SECOND_LOG);
		fdActorDisableById(ACTOR_CRYSTAL_ROOM_THIRD_LOG);
		fdActorDisableById(ACTOR_CRYSTAL_ROOM_FOURTH_LOG);
		scYampiDesertCaveSetLogMetatiles(ACTOR_CRYSTAL_ROOM_FIRST_LOG);
		scYampiDesertCaveSetLogMetatiles(ACTOR_CRYSTAL_ROOM_SECOND_LOG);
		scYampiDesertCaveSetLogMetatiles(ACTOR_CRYSTAL_ROOM_THIRD_LOG);
		scYampiDesertCaveSetLogMetatiles(ACTOR_CRYSTAL_ROOM_FOURTH_LOG);
		scPillarPlace(YampiDesertCaveCrystalStakes);
		if(!fdScriptGetFlag(FLAG_YAMPI_DESERT_CAVE_STAKE_POUNDED))
			fdMapGridCopyMetatileAttributes(48, 12, 1, 1, 46, 12);
		if(fdScriptGetFlag(FLAG_YAMPI_DESERT_CAVE_CRYSTAL_ROOM_PILLAR_BURST))
			fdActorSetPos(ACTOR_CRYSTAL_ROOM_BURST_PILLAR, 0, 0);
		if(fdScriptGetFlag(FLAG_YAMPI_DESERT_CAVE_WEST_PILLAR_MOVED)) {
			fdActorSetPos(ACTOR_WEST_PILLAR, 168, 184);
			fdMapGridCopyMetatileAttributes(12, 11, 1, 1, 10, 11);
		} else {
#if YAMPI_WEST_PILLAR_BLOCKS_ITS_OWN_CELL
			ACTOR *actor = fdActorGetById(ACTOR_WEST_PILLAR);

			fdMapGridCopyMetatileAttributes(12, 11, 1, 1,
				FIELD_METATILE(actor->x.fixed), FIELD_METATILE(actor->z.fixed));
#else
			fdMapGridCopyMetatileAttributes(12, 11, 1, 1, 8, 10);
#endif
		}
		if(fdScriptGetFlag(FLAG_YAMPI_DESERT_CAVE_EAST_PILLAR_MOVED)) {
			fdActorSetPos(ACTOR_EAST_PILLAR, 616, 264);
			fdMapGridCopyMetatileAttributes(38, 15, 1, 1, 38, 16);
		} else {
			fdMapGridCopyMetatileAttributes(38, 15, 1, 1, 39, 16);
		}
		if(!fdScriptGetFlag(FLAG_DJINNI(DJINNI_CRYSTAL)) &&
			!fdScriptGetFlag(FLAG_DJINNI_HIDING(DJINNI_CRYSTAL))) {
			Sint32 step =
				scFieldGameFlagGetByte(FLAG_BYTE_YAMPI_DESERT_CAVE_RIPPLE_STEP);
			Sint32 placed =
				fdScriptGetFlag(FLAG_YAMPI_DESERT_CAVE_CRYSTAL_DUG_UP);

			if(placed) {
				step = step / RIPPLE_STEPS_PER_POINT * 2;
				fdActorSetPos(ACTOR_SAND_RIPPLE, 0, 0);
				fdActorSetPosFX(ACTOR_CRYSTAL,
					FIELD_HALF_METATILE_ORIGIN(YampiDesertCaveRipplePath[step]),
					FIELD_HALF_METATILE_ORIGIN(
						YampiDesertCaveRipplePath[step + 1]));
			} else {
				if(fdScriptGetFlag(FLAG_KEEP_PARTY_POSITION))
					syGameFlagSetByte(
						FLAG_BYTE_YAMPI_DESERT_CAVE_RIPPLE_STEP, 0);
				sySetEvent(scYampiDesertCaveMoveSandRipple, EVENT_PASS_TICK);
				fdScriptSetFlag(FLAG_YAMPI_DESERT_CAVE_RIPPLE_RUNNING);
				fdActorSetSpritePriority(ACTOR_SAND_RIPPLE, 3);
				fdActorGetById(ACTOR_SAND_RIPPLE)->collisionFlags = 0;
			}
		}
	}
	fdEffectStartBlankOverlay(0);
	return 0;
}

Sint32 scPrepareMap(void)
{
	return 0;
}

#undef DEFINE_TELEPORT_FROM_CIRCLE
#undef DEFINE_BURST_PILLAR

void scYampiDesertCaveMeetGuardianOfFire(void);

/* clang-format off */
#include "shared/push_kinds.c"

#include "shared/push_leader_steps.c"

/* The ripple's path, a column and a row in half metatiles (8 pixels) for */
/* each thirty-two steps: it shows at four places in turn, and between them */
/* lies at the origin, out of sight. */
Sint16 YampiDesertCaveRipplePath[] = {
	101, 39, 0, 0, 97, 35, 0, 0,
	99, 37, 0, 0, 95, 33, 0, 0,
};

#define TELEPORT_CIRCLE_FIRST_ENTRY 9
#include "shared/teleport_circle_palette.c"

#include "shared/scene_entrances_default.c"

Uint32 SceneExits[] = {
	SCENE_EXITS(YampiDesertCaveWaterOfLifeChest),
	SCENE_EXIT(YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST_TO_YAMPI_DESERT_TRAINERS_WHIP_CHEST, YampiDesertTrainersWhipChest, YAMPI_DESERT_TRAINERS_WHIP_CHEST_FROM_YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST),
	SCENE_EXIT(YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST_TO_YAMPI_DESERT_CAVE_CRYSTAL_SOUTH, YampiDesertCaveCrystal, YAMPI_DESERT_CAVE_CRYSTAL_FROM_YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST_SOUTH),
	SCENE_EXIT(CHEST_ROOM_MIDDLE_PASSAGE, YampiDesertCaveWaterOfLifeChest, CHEST_ROOM_NORTH_PART_EAST_FOOT),
	SCENE_EXIT(CHEST_ROOM_WEST_PASSAGE, YampiDesertCaveWaterOfLifeChest, CHEST_ROOM_NORTH_PART_WEST_FOOT),
	SCENE_EXIT(CHEST_ROOM_NORTH_PART_WEST_FOOT, YampiDesertCaveWaterOfLifeChest, CHEST_ROOM_WEST_PASSAGE),
	SCENE_EXIT(CHEST_ROOM_NORTH_PART_EAST_FOOT, YampiDesertCaveWaterOfLifeChest, CHEST_ROOM_MIDDLE_PASSAGE),
	SCENE_EXIT(YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST_TO_YAMPI_DESERT_CAVE_CRYSTAL_NORTH, YampiDesertCaveCrystal, YAMPI_DESERT_CAVE_CRYSTAL_FROM_YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST_NORTH),
	SCENE_EXIT(YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST_TO_YAMPI_DESERT_CAVE_GUARDIAN, YampiDesertCaveGuardian, YAMPI_DESERT_CAVE_GUARDIAN_FROM_YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST),
	SCENE_EXITS(YampiDesertCaveGuardian),
	SCENE_EXIT(YAMPI_DESERT_CAVE_GUARDIAN_TO_YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST, YampiDesertCaveWaterOfLifeChest, YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST_FROM_YAMPI_DESERT_CAVE_GUARDIAN),
	SCENE_EXIT(GUARDIAN_SOUTH_CAVE_STAIRS, YampiDesertCaveGuardian, GUARDIAN_LADDER_ROOM_STAIRS),
	SCENE_EXIT(YAMPI_DESERT_CAVE_GUARDIAN_TO_YAMPI_DESERT_CAVE_CRYSTAL_NORTHWEST, YampiDesertCaveCrystal, YAMPI_DESERT_CAVE_CRYSTAL_FROM_YAMPI_DESERT_CAVE_GUARDIAN_NORTHWEST),
	SCENE_EXIT(YAMPI_DESERT_CAVE_GUARDIAN_TO_YAMPI_DESERT_CAVE_CRYSTAL_SOUTH, YampiDesertCaveCrystal, YAMPI_DESERT_CAVE_CRYSTAL_FROM_YAMPI_DESERT_CAVE_GUARDIAN_SOUTH),
	SCENE_EXIT(YAMPI_DESERT_CAVE_GUARDIAN_TO_YAMPI_DESERT_CAVE_CRYSTAL_SOUTHEAST, YampiDesertCaveCrystal, YAMPI_DESERT_CAVE_CRYSTAL_FROM_YAMPI_DESERT_CAVE_GUARDIAN_SOUTHEAST),
	SCENE_EXIT(GUARDIAN_LADDER_ROOM_STAIRS, YampiDesertCaveGuardian, GUARDIAN_SOUTH_CAVE_STAIRS),
	SCENE_EXITS(YampiDesertCaveCrystal),
	SCENE_EXIT(YAMPI_DESERT_CAVE_CRYSTAL_TO_YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST_SOUTH, YampiDesertCaveWaterOfLifeChest, YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST_FROM_YAMPI_DESERT_CAVE_CRYSTAL_SOUTH),
	SCENE_EXIT(CRYSTAL_LADDER_ROOM_WEST_DOORWAY, YampiDesertCaveCrystal, CRYSTAL_LOG_ROOM_FOOT),
	SCENE_EXIT(CRYSTAL_LOG_ROOM_FOOT, YampiDesertCaveCrystal, CRYSTAL_LADDER_ROOM_WEST_DOORWAY),
	SCENE_EXIT(CRYSTAL_LOG_ROOM_NORTH_DOORWAY, YampiDesertCaveCrystal, CRYSTAL_NORTHWEST_ROOM_EAST_FOOT),
	SCENE_EXIT(CRYSTAL_LOG_ROOM_LEDGE_DOORWAY, YampiDesertCaveCrystal, CRYSTAL_NORTHEAST_ROOM_EAST_END),
	SCENE_EXIT(CRYSTAL_NORTHEAST_ROOM_EAST_END, YampiDesertCaveCrystal, CRYSTAL_LOG_ROOM_LEDGE_DOORWAY),
	SCENE_EXIT(CRYSTAL_NORTHWEST_ROOM_EAST_FOOT, YampiDesertCaveCrystal, CRYSTAL_LOG_ROOM_NORTH_DOORWAY),
	SCENE_EXIT(YAMPI_DESERT_CAVE_CRYSTAL_TO_YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST_NORTH, YampiDesertCaveWaterOfLifeChest, YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST_FROM_YAMPI_DESERT_CAVE_CRYSTAL_NORTH),
	SCENE_EXIT(CRYSTAL_NORTHEAST_ROOM_NORTH_DOORWAY, YampiDesertCaveCrystal, CRYSTAL_NORTHWEST_ROOM_WEST_FOOT),
	SCENE_EXIT(CRYSTAL_NORTHWEST_ROOM_WEST_FOOT, YampiDesertCaveCrystal, CRYSTAL_NORTHEAST_ROOM_NORTH_DOORWAY),
	SCENE_EXIT(YAMPI_DESERT_CAVE_CRYSTAL_TO_YAMPI_DESERT_CAVE_GUARDIAN_SOUTHEAST, YampiDesertCaveGuardian, YAMPI_DESERT_CAVE_GUARDIAN_FROM_YAMPI_DESERT_CAVE_CRYSTAL_SOUTHEAST),
	SCENE_EXIT(YAMPI_DESERT_CAVE_CRYSTAL_TO_YAMPI_DESERT_CAVE_GUARDIAN_SOUTH, YampiDesertCaveGuardian, YAMPI_DESERT_CAVE_GUARDIAN_FROM_YAMPI_DESERT_CAVE_CRYSTAL_SOUTH),
	SCENE_EXIT(YAMPI_DESERT_CAVE_CRYSTAL_TO_YAMPI_DESERT_CAVE_GUARDIAN_NORTHWEST, YampiDesertCaveGuardian, YAMPI_DESERT_CAVE_GUARDIAN_FROM_YAMPI_DESERT_CAVE_CRYSTAL_NORTHWEST),
	SCENE_EXITS_END,
};

/* A pushed pillar or stake stands still where it was placed. */
Uint32 YampiDesertCaveSettle[] = {
	SC_SET(SW_MOTION, 0),
	SC_END,
};

/* The Water of Life chest's room: three logs and the pillar Burst breaks. */
ACTOR_PLACE YampiDesertCavePlacements[] = {
	ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 888, 0, 336, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 832, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 912, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BURST_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 712, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};
/* clang-format on */

/* Valukar's room: the pillar Move pushes, and Valukar until he is beaten. */
ACTOR_PLACE YampiDesertCaveGuardianPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS,
		(Sint32)YampiDesertCaveSettle, 552, 0, 200, FACING_SOUTH,
		TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_VALUKAR, FLAG_VALUKAR_BEATEN, MOVEMENT_TYPE_STAND,
		200, 0, 152, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

/* Crystal's room, in YAMPI_DESERT_CAVE_CRYSTAL_ACTOR_ID order. */
ACTOR_PLACE YampiDesertCaveCrystalPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_NORTH_SOUTH, CONDITION_ALWAYS,
		MOVEMENT_TYPE_CLEAR_COLLISION, 712, 0, 752, FACING_SOUTH,
		TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_WEST_EAST, CONDITION_ALWAYS,
		MOVEMENT_TYPE_CLEAR_COLLISION, 752, 0, 744, FACING_SOUTH,
		TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_NORTH_SOUTH, CONDITION_ALWAYS,
		MOVEMENT_TYPE_CLEAR_COLLISION, 728, 0, 784, FACING_SOUTH,
		TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_WEST_EAST, CONDITION_ALWAYS,
		MOVEMENT_TYPE_CLEAR_COLLISION, 800, 0, 760, FACING_SOUTH,
		TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS,
		(Sint32)YampiDesertCaveSettle, 632, 0, 264, FACING_SOUTH,
		TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS,
		(Sint32)YampiDesertCaveSettle, 136, 0, 168, FACING_SOUTH,
		TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS,
		(Sint32)YampiDesertCaveSettle, 744, 0, 200, FACING_SOUTH,
		TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BURST_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		264, 0, 632, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SAND_RIPPLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_VENUS_DJINNI, FLAG_DJINNI(DJINNI_CRYSTAL),
		MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	{SCENE_TABLE_END},
};

/* The stake Pound sinks in Crystal's room: its actor, its kind and the flag set
   once it is sunk, as fdPoundSinkTarget reads them. */
Uint16 YampiDesertCaveCrystalStakes[] ALIGNED(4) = {
	ACTOR_STAKE,
	0,
	FLAG_YAMPI_DESERT_CAVE_STAKE_POUNDED,
	ACTOR_LIST_END,
};

/* The Water of Life chest's room: its exits, the logs, the pillar, the two
   Teleport circles and Sand. */
CUE YampiDesertCaveCues[] = {
	{CUE_EXIT,
		YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST_TO_YAMPI_DESERT_TRAINERS_WHIP_CHEST,
		CONDITION_ALWAYS,
		YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST_TO_YAMPI_DESERT_TRAINERS_WHIP_CHEST},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN,
		YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST_TO_YAMPI_DESERT_CAVE_CRYSTAL_SOUTH,
		CONDITION_ALWAYS,
		YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST_TO_YAMPI_DESERT_CAVE_CRYSTAL_SOUTH},
	{CUE_EXIT, CHEST_ROOM_MIDDLE_PASSAGE, CONDITION_ALWAYS,
		CHEST_ROOM_MIDDLE_PASSAGE},
	{CUE_EXIT, CHEST_ROOM_WEST_PASSAGE, CONDITION_ALWAYS,
		CHEST_ROOM_WEST_PASSAGE},
	{CUE_EXIT, CHEST_ROOM_NORTH_PART_WEST_FOOT, CONDITION_ALWAYS,
		CHEST_ROOM_NORTH_PART_WEST_FOOT},
	{CUE_EXIT, CHEST_ROOM_NORTH_PART_EAST_FOOT, CONDITION_ALWAYS,
		CHEST_ROOM_NORTH_PART_EAST_FOOT},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN,
		YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST_TO_YAMPI_DESERT_CAVE_CRYSTAL_NORTH,
		CONDITION_ALWAYS,
		YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST_TO_YAMPI_DESERT_CAVE_CRYSTAL_NORTH},
	{CUE_EXIT,
		YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST_TO_YAMPI_DESERT_CAVE_GUARDIAN,
		CONDITION_ALWAYS,
		YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST_TO_YAMPI_DESERT_CAVE_GUARDIAN},
	{STEP_CUE_WALKING, METATILE_AROUND_LOG, CONDITION_ALWAYS,
		SCRIPT_REF(scYampiDesertCaveTryPushLog)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
		METATILE_BURST_PILLAR, FLAG_YAMPI_DESERT_CAVE_CHEST_ROOM_PILLAR_BURST,
		SCRIPT_REF(YampiDesertCaveBurstChestRoomPillar)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_TELEPORT, PSYNERGY_PHASE_AFTER_CAST),
		NORTH_CIRCLE_METATILES, CONDITION_ALWAYS,
		SCRIPT_REF(scYampiDesertCaveTeleportFromNorthCircle)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_TELEPORT, PSYNERGY_PHASE_AFTER_CAST),
		SOUTH_CIRCLE_METATILES, CONDITION_ALWAYS,
		SCRIPT_REF(scYampiDesertCaveTeleportFromSouthCircle)},
	{PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0,
		CONDITION_ALWAYS, SCRIPT_REF(scYampiDesertCaveCastSand)},
	{PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS,
		SCRIPT_REF(scYampiDesertCaveEndSand)},
	{CUE_TABLE_END},
};

/* Valukar's room: its exits, Valukar's step, the pillar, Sand and Scoop on the
   buried Mythril Silver. */
CUE YampiDesertCaveGuardianCues[] = {
	{CUE_EXIT,
		YAMPI_DESERT_CAVE_GUARDIAN_TO_YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST,
		CONDITION_ALWAYS,
		YAMPI_DESERT_CAVE_GUARDIAN_TO_YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, GUARDIAN_SOUTH_CAVE_STAIRS,
		CONDITION_ALWAYS, GUARDIAN_SOUTH_CAVE_STAIRS},
	{CUE_EXIT,
		YAMPI_DESERT_CAVE_GUARDIAN_TO_YAMPI_DESERT_CAVE_CRYSTAL_NORTHWEST,
		CONDITION_ALWAYS,
		YAMPI_DESERT_CAVE_GUARDIAN_TO_YAMPI_DESERT_CAVE_CRYSTAL_NORTHWEST},
	{CUE_EXIT, YAMPI_DESERT_CAVE_GUARDIAN_TO_YAMPI_DESERT_CAVE_CRYSTAL_SOUTH,
		CONDITION_ALWAYS,
		YAMPI_DESERT_CAVE_GUARDIAN_TO_YAMPI_DESERT_CAVE_CRYSTAL_SOUTH},
	{CUE_EXIT,
		YAMPI_DESERT_CAVE_GUARDIAN_TO_YAMPI_DESERT_CAVE_CRYSTAL_SOUTHEAST,
		CONDITION_ALWAYS,
		YAMPI_DESERT_CAVE_GUARDIAN_TO_YAMPI_DESERT_CAVE_CRYSTAL_SOUTHEAST},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, GUARDIAN_LADDER_ROOM_STAIRS,
		CONDITION_ALWAYS, GUARDIAN_LADDER_ROOM_STAIRS},
	{CUE_STEP, METATILE_BEFORE_VALUKAR, FLAG_VALUKAR_BEATEN,
		SCRIPT_REF(scYampiDesertCaveMeetGuardianOfFire)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
		ACTOR_GUARDIAN_ROOM_PILLAR,
		FLAG_YAMPI_DESERT_CAVE_GUARDIAN_PILLAR_MOVED,
		SCRIPT_REF(scYampiDesertCaveStartMovingPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_GUARDIAN_ROOM_PILLAR,
		FLAG_YAMPI_DESERT_CAVE_GUARDIAN_PILLAR_MOVED,
		SCRIPT_REF(scYampiDesertCaveFinishMovingPillar)},
	{PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0,
		CONDITION_ALWAYS, SCRIPT_REF(scYampiDesertCaveCastSand)},
	{PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS,
		SCRIPT_REF(scYampiDesertCaveEndSand)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS),
		METATILE_MYTHRIL_SILVER, FLAG_YAMPI_DESERT_CAVE_MYTHRIL_SILVER_DUG_UP,
		SCRIPT_REF(scYampiDesertCaveDigUpMythrilSilver)},
	{CUE_TABLE_END},
};

/* Crystal's room: its exits, the ripple and Crystal, the logs, the pillars, the
   stake, Sand and Scoop on the ripple. */
CUE YampiDesertCaveCrystalCues[] = {
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP,
		YAMPI_DESERT_CAVE_CRYSTAL_TO_YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST_SOUTH,
		CONDITION_ALWAYS,
		YAMPI_DESERT_CAVE_CRYSTAL_TO_YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST_SOUTH},
	{CUE_EXIT, CRYSTAL_LADDER_ROOM_WEST_DOORWAY, CONDITION_ALWAYS,
		CRYSTAL_LADDER_ROOM_WEST_DOORWAY},
	{CUE_EXIT, CRYSTAL_LOG_ROOM_FOOT, CONDITION_ALWAYS, CRYSTAL_LOG_ROOM_FOOT},
	{CUE_EXIT, CRYSTAL_LOG_ROOM_NORTH_DOORWAY, CONDITION_ALWAYS,
		CRYSTAL_LOG_ROOM_NORTH_DOORWAY},
	{CUE_EXIT, CRYSTAL_LOG_ROOM_LEDGE_DOORWAY, CONDITION_ALWAYS,
		CRYSTAL_LOG_ROOM_LEDGE_DOORWAY},
	{CUE_EXIT, CRYSTAL_NORTHEAST_ROOM_EAST_END, CONDITION_ALWAYS,
		CRYSTAL_NORTHEAST_ROOM_EAST_END},
	{CUE_EXIT, CRYSTAL_NORTHWEST_ROOM_EAST_FOOT, CONDITION_ALWAYS,
		CRYSTAL_NORTHWEST_ROOM_EAST_FOOT},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP,
		YAMPI_DESERT_CAVE_CRYSTAL_TO_YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST_NORTH,
		CONDITION_ALWAYS,
		YAMPI_DESERT_CAVE_CRYSTAL_TO_YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST_NORTH},
	{CUE_EXIT, CRYSTAL_NORTHEAST_ROOM_NORTH_DOORWAY, CONDITION_ALWAYS,
		CRYSTAL_NORTHEAST_ROOM_NORTH_DOORWAY},
	{CUE_EXIT, CRYSTAL_NORTHWEST_ROOM_WEST_FOOT, CONDITION_ALWAYS,
		CRYSTAL_NORTHWEST_ROOM_WEST_FOOT},
	{CUE_EXIT,
		YAMPI_DESERT_CAVE_CRYSTAL_TO_YAMPI_DESERT_CAVE_GUARDIAN_SOUTHEAST,
		CONDITION_ALWAYS,
		YAMPI_DESERT_CAVE_CRYSTAL_TO_YAMPI_DESERT_CAVE_GUARDIAN_SOUTHEAST},
	{CUE_EXIT, YAMPI_DESERT_CAVE_CRYSTAL_TO_YAMPI_DESERT_CAVE_GUARDIAN_SOUTH,
		CONDITION_ALWAYS,
		YAMPI_DESERT_CAVE_CRYSTAL_TO_YAMPI_DESERT_CAVE_GUARDIAN_SOUTH},
	{CUE_EXIT,
		YAMPI_DESERT_CAVE_CRYSTAL_TO_YAMPI_DESERT_CAVE_GUARDIAN_NORTHWEST,
		CONDITION_ALWAYS,
		YAMPI_DESERT_CAVE_CRYSTAL_TO_YAMPI_DESERT_CAVE_GUARDIAN_NORTHWEST},
	{CUE_TALK, ACTOR_SAND_RIPPLE, CONDITION_ALWAYS,
		SCRIPT_REF(scYampiDesertCaveWonderAtRipple)},
	{CUE_TALK, ACTOR_CRYSTAL, CONDITION_ALWAYS,
		SCRIPT_REF(scYampiDesertCaveFightCrystal)},
	{STEP_CUE_WALKING, METATILE_AROUND_LOG, CONDITION_ALWAYS,
		SCRIPT_REF(scYampiDesertCaveTryPushLog)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
		METATILE_RIPPLE_SAND, FLAG_YAMPI_DESERT_CAVE_CRYSTAL_ROOM_PILLAR_BURST,
		SCRIPT_REF(YampiDesertCaveBurstCrystalRoomPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
		ACTOR_EAST_PILLAR, FLAG_YAMPI_DESERT_CAVE_EAST_PILLAR_MOVED,
		SCRIPT_REF(scYampiDesertCaveStartMovingPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_EAST_PILLAR,
		FLAG_YAMPI_DESERT_CAVE_EAST_PILLAR_MOVED,
		SCRIPT_REF(scYampiDesertCaveFinishMovingPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
		ACTOR_WEST_PILLAR, FLAG_YAMPI_DESERT_CAVE_WEST_PILLAR_MOVED,
		SCRIPT_REF(scYampiDesertCaveStartMovingPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_WEST_PILLAR,
		FLAG_YAMPI_DESERT_CAVE_WEST_PILLAR_MOVED,
		SCRIPT_REF(scYampiDesertCaveFinishMovingPillar)},
	{CUE_PUSH, 0, CONDITION_ALWAYS,
		SCRIPT_REF(scYampiDesertCaveStartMovingPillar)},
	{CUE_PUSHED, 0, CONDITION_ALWAYS,
		SCRIPT_REF(scYampiDesertCaveFinishMovingPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_STAKE,
		FLAG_YAMPI_DESERT_CAVE_STAKE_POUNDED,
		SCRIPT_REF(scYampiDesertCavePoundStake)},
	{PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0,
		CONDITION_ALWAYS, SCRIPT_REF(scYampiDesertCaveCastSand)},
	{PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS,
		SCRIPT_REF(scYampiDesertCaveEndSand)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS),
		METATILE_RIPPLE_SAND,
		IF_FLAG_SET(FLAG_YAMPI_DESERT_CAVE_RIPPLE_RUNNING),
		SCRIPT_REF(scYampiDesertCaveScoopDjinni)},
	{CUE_TABLE_END},
};

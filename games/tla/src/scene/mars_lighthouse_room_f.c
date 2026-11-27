/* The Mars Lighthouse's four towers, one scene each in this overlay. Each is
   reached from the central hall by its ledge in the rain and comes out on the
   beacon ledge where its element's flame is lit. Every tower has white dragon
   heads on its walls whose flames knock Felix back, and white urn torches that,
   once fire reaches them, send a fireball rolling.

   The Mars tower: Burst breaks its cracked walls. Stepping on a button in the
   lava sets a dragon head sliding along the north wall until its flame lights
   an urn torch; Blaze at that torch carries on to the one below, whose fireball
   cracks the centre wall for Burst. In its pipe room, pipes pushed into line
   turn the fire from one dragon head to another until the way is clear.

   The Mercury tower: across black ice, frozen puddles and pushed pipes mend the
   pipeline until no fire bursts from the pipes' ends; then Blaze lights a
   dragon head whose flame reaches an urn torch, and its fireball melts the
   icebergs.

   The Jupiter tower: Cyclone on its circles lifts the party from room to room,
   Move drops a block to bridge a gap and pushes a white dragon statue into the
   flames of the big room, Reveal shows stepping stones, and a hole drops the
   party back to the first hall.

   The Venus tower: gaps jumped and filled with stones Carry lifts, a robed
   statue pushed off a ledge, a tightrope, and statues that hide dragon heads or
   the way on. */

/* This unit declares its own form of fdMapGridGetMetatileBehavior, below. */
#define OWN_fdMapGridGetMetatileBehavior

#include "scene/flame_throwers.h"
#include "field/field_mode.h"
#include "system/field_effect.h"
#include "scene/scene.h"
#include "field/script_inline.h"
#include "edition.h"
#include "macros.h"
#include "field/call.h"
#include "field/actor_script.h"
#include "scene/lighthouse_palettes.h"

/* Move set the white dragon statue under the dragon head. */
#define FLAG_JUPITER_STATUE_BLOCKS_FLAMES  0xa37
/* The robed statue dropped off the upper ledge. */
#define FLAG_VENUS_STATUE_PUSHED_OFF_LEDGE 0xa38
/* The urn torch's fireball melted the icebergs. */
#define FLAG_ICEBERGS_MELTED			   0xa40
/* The button's flame thrower has reached the Mars tower's torch. */
#define FLAG_MARS_TORCH_LIT				   0xa42
#define FLAG_CENTER_WALL_CRACKED		   0xa43
#define FLAG_WEST_WALL_BURST			   0xa44
#define FLAG_EAST_WALL_BURST			   0xa45
#define FLAG_CENTER_WALL_BURST			   0xa46
#define FLAG_BUTTON_PRESSED				   FLAG_SCENE_TEMP_20
#define FLAG_UPPER_PUDDLE_FROZEN		   FLAG_SCENE_TEMP_20
#define FLAG_CENTER_PUDDLE_FROZEN		   FLAG_SCENE_TEMP_21
/* The Mercury tower's pipes all in line: no fire bursts from the pipes'
   ends. */
#define FLAG_PIPELINE_MENDED			   FLAG_AREA_TEMP_50
/* The camera has been shown the mended pipeline. */
#define FLAG_SAW_PIPELINE_MENDED		   FLAG_AREA_TEMP_51
#define FLAG_BLOCK_DROPPED				   FLAG_AREA_TEMP_58

/* The Jupiter tower's exit and entrance by its hole, which drops the party back
   to the tower's first room. */
#define JUPITER_TOWER_THROUGH_HOLE 16

/* The Mercury tower's exit back into itself, and the entrance it lands at, once
   its pipeline is first mended: the tower is loaded afresh and the torch the
   pipeline feeds flares up. */
#define MERCURY_TOWER_PIPELINE_MENDED 9

/* The doorways inside the Mars and Mercury towers between two of their parts:
   each is an entrance, and the exit of its number goes through it to the other.
   The Mars tower's join its first rooms, by the lava and the button, to its
   pipe room; the Mercury tower's join its hall of pipes to the room of the
   dragon head Blaze lights. */
enum {
	MARS_TOWER_FIRST_ROOMS_DOORWAY = 2,
	MARS_TOWER_PIPE_ROOM_DOORWAY = 3,
	MERCURY_TOWER_PIPE_HALL_DOORWAY = 2,
	MERCURY_TOWER_DRAGON_HEAD_ROOM_DOORWAY = 3
};

/* The Jupiter tower's Cyclone circles, by the exit each one's whirlwind takes,
   the number its metatiles carry. A circle the party also lands on shares its
   number with that landing (below). */
enum {
	CYCLONE_FROM_FIRST_HALL_WEST_CIRCLE = 2,
	CYCLONE_FROM_CORRIDOR_FOOT_CIRCLE = 3,
	CYCLONE_FROM_FIRST_HALL_EAST_CIRCLE = 4,
	CYCLONE_FROM_NARROW_ROOM_FIRST_CIRCLE = 5,
	CYCLONE_FROM_CORRIDOR_HEAD_EAST_CIRCLE = 6,
	CYCLONE_FROM_CORRIDOR_HEAD_WEST_CIRCLE = 7,
	CYCLONE_FROM_BIG_ROOM_RAISED_CIRCLE = 8,
	CYCLONE_FROM_TWO_CIRCLE_ROOM_FIRST_CIRCLE = 9,
	CYCLONE_FROM_TWO_CIRCLE_ROOM_SECOND_CIRCLE = 10,
	CYCLONE_FROM_BIG_ROOM_EAST_CIRCLE = 11,
	CYCLONE_FROM_NARROW_ROOM_SECOND_CIRCLE = 12,
	CYCLONE_FROM_BIG_ROOM_WEST_CIRCLE = 13,
	CYCLONE_FROM_CARVED_DRAGON_CIRCLE = 14
};

/* The Jupiter tower's landings from Cyclone's whirlwinds, each where an exit of
   a Cyclone circle sets the party down: most on the circle that takes it back,
   the rest in the big room of the white dragon statue. */
enum {
	JUPITER_TOWER_FIRST_HALL_WEST_CIRCLE = 2,
	JUPITER_TOWER_CORRIDOR_FOOT_CIRCLE = 3,
	JUPITER_TOWER_FIRST_HALL_EAST_CIRCLE = 4,
	JUPITER_TOWER_NARROW_ROOM_FIRST_CIRCLE = 5,
	JUPITER_TOWER_BIG_ROOM_SOUTHWEST = 6,
	JUPITER_TOWER_BIG_ROOM_FAR_WEST = 7,
	JUPITER_TOWER_BIG_ROOM_RAISED_CIRCLE = 8,
	JUPITER_TOWER_TWO_CIRCLE_ROOM_FIRST_CIRCLE = 9,
	JUPITER_TOWER_BIG_ROOM_NORTHEAST_LEDGE = 10,
	JUPITER_TOWER_BIG_ROOM_EAST_CIRCLE = 11,
	JUPITER_TOWER_NARROW_ROOM_SECOND_CIRCLE = 12,
	JUPITER_TOWER_BIG_ROOM_WEST_CIRCLE = 13,
	JUPITER_TOWER_CARVED_DRAGON_CIRCLE = 14
};

/* The Venus tower's doorways the party comes through from the small rooms
   between its halls: the west and east doorways of the hall of the flame
   throwers and stones; the statue hall's west doorway (the upper doorway, whose
   statue stands aside once moved) and east doorway; the statue hall's south
   passage, from the lower hall; and the lower hall's doorway in its row of
   statues (the lower doorway). */
enum {
	VENUS_TOWER_HALL_WEST_DOORWAY = 6,
	VENUS_TOWER_HALL_EAST_DOORWAY = 8,
	VENUS_TOWER_UPPER_DOORWAY = 11,
	VENUS_TOWER_STATUE_HALL_EAST_DOORWAY = 13,
	VENUS_TOWER_STATUE_HALL_SOUTH_PASSAGE = 14,
	VENUS_TOWER_LOWER_DOORWAY = 15
};

/* The Mars tower. Its first rooms: the flame thrower the button sets moving,
   the torch its flame lights (two actors, lit and unlit), the torch below it
   that shoots at the center wall, its glint and fireball, and the unseen point
   at the wall's foot. Its pipe room: two pipes that can be pushed, the four
   places its fire comes from, and a dragon statue. */
enum {
	ACTOR_BUTTON_FLAME_THROWER = 8,
	ACTOR_MARS_TORCH = 9,
	ACTOR_UNLIT_MARS_TORCH = 10,
	ACTOR_WALL_TORCH = 11,
	ACTOR_WALL_SPARKLE = 12,  /* the glint that leaves the torch */
	ACTOR_WALL_FIREBALL = 13, /* the fireball the glint sets rolling */
	ACTOR_CENTER_WALL = 14,
	ACTOR_MARS_PIPE_A = 15,
	ACTOR_MARS_PIPE_B = 16,
	/* Where the fire bursts out: unseen points at the pipes' open ends, or,
	   once a pipe is pushed into line, the flame thrower it feeds. */
	ACTOR_PIPE_FLAME_A = 18,
	ACTOR_PIPE_FLAME_B,
	ACTOR_MARS_PIPE_A_FLAME_THROWER, /* a dragon head seen from the side */
	ACTOR_MARS_PIPE_B_FLAME_THROWER, /* a white dragon head */
	ACTOR_PIPE_STATUE
};

/* The Mercury tower: three pushable pipes, two puddles for Frost, the unseen
   points at the pipes' ends where the fire bursts out, the flame thrower Blaze
   lights, the torch the mended pipeline lights (lit and unlit), and the torch
   that shoots at the icebergs, with its glint and fireball. */
enum {
	ACTOR_MERCURY_PIPE_A = 8,
	ACTOR_MERCURY_PIPE_B = 9,
	ACTOR_MERCURY_PIPE_C = 10,
	ACTOR_UPPER_PUDDLE = 11,
	ACTOR_CENTER_PUDDLE = 12,
	ACTOR_MERCURY_PIPE_FLAME_A = 13, /* unseen, at a pipe's end */
	ACTOR_MERCURY_PIPE_FLAME_B,
	ACTOR_MERCURY_PIPE_FLAME_C,
	ACTOR_MERCURY_FLAME_THROWER = 16,
	ACTOR_MERCURY_TORCH = 17,
	ACTOR_UNLIT_MERCURY_TORCH = 18,
	ACTOR_ICEBERG_TORCH = 19,
	ACTOR_ICEBERG_SPARKLE = 21, /* the glint that leaves the torch */
	ACTOR_ICEBERG_FIREBALL = 22 /* the fireball the glint sets rolling */
};

/* The Jupiter tower: its flame throwers, the purple block Move drops into the
   gap, the white dragon statue Move pushes into the flames, and the torch above
   the great carved dragon, with its glint and fireball. */
enum {
	ACTOR_JUPITER_FLAME_THROWER_A = 8,
	ACTOR_JUPITER_FLAME_THROWER_B,
	ACTOR_JUPITER_FLAME_THROWER_C,
	ACTOR_DROPPING_BLOCK = 11,
	ACTOR_JUPITER_STATUE = 12,
	ACTOR_UPPER_FLAME_THROWER = 13,
	ACTOR_DROP_TORCH = 14,
	ACTOR_DROP_SPARKLE = 15, /* the glint that leaves the torch */
	ACTOR_DROP_FIREBALL = 16 /* the fireball that drops where the glint stops */
};

/* The Venus tower: the tightrope's posts, the three stones Carry lifts, its
   flame throwers, the robed stone statue pushed off the ledge, the statues
   before the two doorways, which stand aside once moved, the five that hide
   flame throwers, and those three hidden flame throwers. */
enum {
	ACTOR_TIGHTROPE_NEAR_POST = 8,
	ACTOR_TIGHTROPE_FAR_POST = 9,
	ACTOR_FIRST_STONE = 10,
	ACTOR_SECOND_STONE = 11,
	ACTOR_THIRD_STONE = 12,
	ACTOR_VENUS_FLAME_THROWER_A = 13,
	ACTOR_VENUS_FLAME_THROWER_B,
	ACTOR_VENUS_FLAME_THROWER_C,
	ACTOR_PUSHED_STATUE = 16,
	ACTOR_UPPER_DOORWAY_STATUE = 17,
	ACTOR_TRAP_STATUE_A = 18,
	ACTOR_TRAP_STATUE_B,
	ACTOR_TRAP_STATUE_C,
	ACTOR_TRAP_STATUE_D,
	ACTOR_TRAP_STATUE_E,
	ACTOR_LOWER_DOORWAY_STATUE = 23,
	ACTOR_HIDDEN_FLAME_THROWER_A = 24,
	ACTOR_HIDDEN_FLAME_THROWER_B,
	ACTOR_HIDDEN_FLAME_THROWER_C
};

/* Where a pushed object comes to rest, read by the push that follows. */
typedef struct PUSH_TARGET {
	FIXED  x; /* 16.16 fixed-point pixels */
	Sint32 y;
	Sint32 z;
	Sint32 rectangle; /* index into the rectangle table */
	Sint32 id;
	void  *func;
} PUSH_TARGET;

Sint32 fdMapGetTerrainHeight(Sint32 layer, Sint32 x, Sint32 z);

#ifdef MODERN
/* scFlameThrowerSet, defined below, is called with no prototype in scope. */
void scFlameThrowerSet(Sint32 index, Sint32 value);
#endif
Sint32 scFlameThrowerGetFlags(Sint32 index);
Bool   scPushTargetFindStop(PUSH_TARGET *target);
void   scPushTargetPush(PUSH_TARGET target);

void scActorSetupScriptedPart(ACTOR *actor, Sint32 part);
void scMarsLighthouseRoomFFillGapsWithStones(void);
void scMarsLighthouseRoomFShootFireballAtWall(void);

#include "shared/jupiter_lighthouse_aerie.h"
#include "shared/tables.h"

struct EFFECT_OPTIONS;
struct LIGHT_OAM;

extern Sint16 SlideOnIce_SteerAngles[];
/* The first of SlideOnIce_SteerAngles' second row, which keeps only the four
   straight headings: the first row has an entry for each of the sixteen
   combinations of the pad's four direction keys. */
#define STEER_STRAIGHT_ROW 16

void scMarsLighthouseRoomFMeltPuddle(Sint32 id);

void scLeaderSetSlideStopped(Sint32 value);

void scFlameThrowersCreate(void);

void scFlameThrowersReleaseFlame(void);

void scFlameThrowerSetPuffsOffScreen(Sint32 index, Sint16 value);

void scCrackedTileDropIn(void);

Bool scPushTargetFillRectangle(Sint32 id);

void scLightSprsInitB(void);

void scLightSprsAdd(Sint32 id, Sint32 value);

void scLightSprsSetPriority(Sint32 value);

void scMarsLighthouseRoomFShootFireballAtIcebergs(void);

void scMarsLighthouseRoomFDropFireball(void);

void scWhirlwindUpdate(ACTOR *self);

void scFieldEffectSpawn(Sint32 x, Sint32 y, Sint32 z, Sint32 velocityX,
	Sint32 velocityY, Sint32 velocityZ, Uint32 flags,
	const EFFECT_OPTIONS *options);

Bool fdActorIsTouching(ACTOR *a, ACTOR *b);
Bool fdActorIsBeside(ACTOR *a, ACTOR *b);
void scFlameUpdateForward(ACTOR *actor);

void scFlameUpdateSideways(ACTOR *actor);

void scFlameThrowerSendForward(
	Sint32 *position, Sint32 speed, Sint32 offScreen);
void scFlameThrowerSendBackward(
	Sint32 *position, Sint32 speed, Sint32 offScreen);
void scFlameThrowerSendEast(Sint32 *position, Sint32 speed, Sint32 offScreen);
void scFlameThrowerSendWest(Sint32 *position, Sint32 speed, Sint32 offScreen);
void scFlameThrowersPuff(void);

extern Uint32 MarsLighthouseRoomF_MercuryFlameThrowerRoute[];

void gmPartyTakeFallDamage(void);

void scLeaderCenterInMetatile(void);

/* 0 when two spheres overlap, else -1 (fdCheckSpheresOverlap, through the
   overlay's own import row). */
Sint32 syImportFieldCheckSpheresOverlap(
	Sint32 *a, Sint32 radiusA, struct POSITION *b, Sint32 radiusB);

Sint32 WhirlwindRiding BSS_DATA;
Uint16				   PaletteCycleColors[16] BSS_DATA;
/* Palette 13's first two colours; it is copied whole, with the unlit colours
   after them. */
Uint16							   PaletteCycleUnlitPalette[2] BSS_DATA;
Uint16							   PaletteCycleUnlitColors[14] BSS_DATA;
METATILE_TARGET CrackedTiles	   BSS_DATA;
volatile Sint16 SlideOnIce_Stopped BSS_DATA;
Sint16 LightSpriteTile BSS_DATA	   ALIGNED(4);
Sint16 LightSpriteCount			   BSS_DATA;
ACTOR							  *LightSpriteActors[16] BSS_DATA;
LIGHT_OAM						   LightSpriteOams[16] BSS_DATA;
Sint16 LightSpritesLarge		   BSS_DATA;
Sint16 LightSpritePriority		   BSS_DATA;

/* Whether two spheres overlap (fdCheckSpheresOverlap), from a script. */
static inline Sint32 scScriptCheckSpheresOverlap(
	Sint32 *a, Sint32 radiusA, Sint32 *b, Sint32 radiusB)
{
	return syImportFieldCheckSpheresOverlap(
		a, radiusA, (struct POSITION *)b, radiusB);
}

void fdLeaderStopIfPending(void);
void fdActorSteerByPad(ACTOR *actor);
/* Sets byte 2 of a rectangle of one layer's map metatiles. */
void   fdMapGridFillCollisionRectangle(Sint32 layer, Sint32 x, Sint32 z,
	  Uint32 width, Uint32 height, Sint32 value);
ACTOR *scPushTargetFindAhead(Sint32 *direction, Sint32 *slot, Sint32 *kind);

void scActorStopAtPushTarget(ACTOR *actor);

void scMarsLighthouseRoomFMeltIcebergs(void);
/* The ROM's callers here are compiled against this form, which returns an
   Sint32 where the definition returns a Uint8. */
Sint32 fdMapGridGetMetatileBehavior(Sint32 x, Sint32 z, Sint32 layer);

const SCENE_ENTRANCE *scGetEntrances(void)
{
	return SceneEntrances;
}

const SCENE_REGION *scGetExitRegions(void)
{
	return 0;
}

/* The four towers share one exit table. */
const Uint32 *scGetExits(void)
{
	return SceneExits;
}

extern ACTOR_PLACE MarsLighthouseRoomF_Placements[];
extern ACTOR_PLACE MarsLighthouseRoomF_VenusTowerPlacements[];
extern ACTOR_PLACE MarsLighthouseRoomF_MarsTowerPlacements[];
extern ACTOR_PLACE MarsLighthouseRoomF_JupiterTowerPlacements[];
extern ACTOR_PLACE MarsLighthouseRoomF_MercuryTowerPlacements[];

/* A torch's script call: the towers' torches stand without a shadow. */
Sint32 scMarsLighthouseRoomFSetUpTorch(ACTOR *actor)
{
	fdActorSetSprFlags(actor, 0);
	return 0;
}

/* The current tower's actors. */
const ACTOR_PLACE *scGetActorPlacements(void)
{
	if(MainRoutineStateWork.next == SCENE_MARS_LIGHTHOUSE_VENUS_TOWER)
		return MarsLighthouseRoomF_VenusTowerPlacements;
	if(MainRoutineStateWork.next == SCENE_MARS_LIGHTHOUSE_MERCURY_TOWER)
		return MarsLighthouseRoomF_MercuryTowerPlacements;
	if(MainRoutineStateWork.next == SCENE_MARS_LIGHTHOUSE_MARS_TOWER)
		return MarsLighthouseRoomF_MarsTowerPlacements;
	if(MainRoutineStateWork.next == SCENE_MARS_LIGHTHOUSE_JUPITER_TOWER)
		return MarsLighthouseRoomF_JupiterTowerPlacements;
	return MarsLighthouseRoomF_Placements;
}

/* Nothing visible happens yet when Felix casts Blaze at the lit urn torch of
   the Mars tower or the Mercury tower; the game only makes this cast's jet the
   faster, longer one (BLAZE_LONG_SPRAY), so that it carries past the torch to
   the torch or dragon head beyond. Each cast starts with the field state's
   `blazeLongSpray` clear. */
void scMarsLighthouseRoomFStartBlazeOnTorch(void)
{
	FIELD_STATE *event = Ram_HeapSlots->fieldState;

	event->blazeLongSpray = 1;
}

/* Walking east or west at one of the gaps along the ledge in the Venus tower's
   first hall (metatiles (37, 8), (39, 8), (42, 8) and (44, 8)), Felix hops
   across it, unless one of the grey stones stands in his way, which he pushes
   instead. */
void scMarsLighthouseRoomFJumpGapOrPushStone(void)
{
	ACTOR *actor = fdActorGet(MainRoutineStateWork.leaderActorId);
	Uint32 facing = actor->facing;

	if(facing == FACING_EAST || facing == FACING_WEST) {
		Sint32 x = FIELD_METATILE(actor->x.fixed);
		Sint32 z = FIELD_METATILE(actor->z.fixed);

		if(facing != FACING_EAST)
			x--;
		else
			x++;
		actor = fdActorGet(ACTOR_FIRST_STONE);
		if(FIELD_METATILE(actor->x.fixed) != x ||
			FIELD_METATILE(actor->z.fixed) != z) {
			actor = fdActorGet(ACTOR_SECOND_STONE);
			if(FIELD_METATILE(actor->x.fixed) != x ||
				FIELD_METATILE(actor->z.fixed) != z) {
				actor = fdActorGet(ACTOR_THIRD_STONE);
				if(FIELD_METATILE(actor->x.fixed) != x ||
					FIELD_METATILE(actor->z.fixed) != z) {
					fdTryJumpForward();
					return;
				}
			}
		}
	}
	if(fdFindNearbyTalkTarget(MainRoutineStateWork.leaderActorId) != -1)
		fdLeaderPushObjectAhead();
}

/* Walking west from metatile (38, 11) in the Venus tower's first hall, Felix
   hops across the gap before him. */
void scMarsLighthouseRoomFJumpGap(void)
{
	fdTryJumpForward();
}

#if MARS_LIGHTHOUSE_LEDGE_PUSH_BLOCKED_BY_STONE

/* Walking into a grey stone on metatile (38, 11) of the Venus tower's first
   hall, Felix pushes it along the ledge; in every edition but the Japanese it
   moves only while no other stone stands beyond it. */
void scMarsLighthouseRoomFPushStoneAlongLedge(void)
{
	Sint32 position[3];
	ACTOR *actor = fdActorGet(MainRoutineStateWork.leaderActorId);
	Sint32 angle = FACING_INDEX(actor->facing);

	/* The facing rounded to the nearest of east, south, west and north: two
	   sixteenths on, whole quarters kept, and back from sixteenths to a
	   facing. */
	angle = FACING_FROM_INDEX((angle + 2) & 0xc);
	position[0] = actor->x.fixed;
	position[1] = (actor->y.fixed + toFIXED(4)) & ~(toFIXED(8) - 1);
	position[2] = actor->z.fixed;
	syFieldVectorAddPolarOffset(16, angle, position);
	actor = fdActorFindAtMetatile(position, 1);
	if(actor != NULL) {
		position[0] = actor->x.fixed;
		position[1] = actor->y.fixed;
		position[2] = actor->z.fixed;
		syFieldVectorAddPolarOffset(16, angle, position);
		if(fdActorFindAtMetatile(position, 1) != NULL)
			return;
	}
	fdLeaderPushObjectAhead();
}

#else

/* Walking into a grey stone on metatile (38, 11) of the Venus tower's first
   hall, Felix pushes it along the ledge. */
void scMarsLighthouseRoomFPushStoneAlongLedge(void)
{
	fdLeaderPushObjectAhead();
}

#endif

extern Sint16 MarsLighthouseRoomF_StatueFlameThrowers[];

/* Felix Moves the robed statue on the Venus tower's upper ledge: once it
   reaches the ledge's east end (metatile column 50), it drops off to the floor
   below, where it stays. */
void scMarsLighthouseRoomFMoveVenusStatueOffLedge(Sint32 unused, Sint32 id)
{
	ACTOR *actor = fdActorGet(id);

	if(FIELD_METATILE(actor->x.fixed) == 50) {
		fdScriptSetFlag(FLAG_VENUS_STATUE_PUSHED_OFF_LEDGE);
		actor->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	}
}

/* Walking east into the robed statue on the Venus tower's upper ledge (from
   metatile (48, 19)), Felix pushes it; once it reaches the ledge's east end
   (metatile column 50), it drops off to the floor below, where it stays. */
void scMarsLighthouseRoomFPushVenusStatueOffLedge(void)
{
	ACTOR *actor = fdActorGet(ACTOR_PUSHED_STATUE);

	fdLeaderPushObjectAhead();
	if(FIELD_METATILE(actor->x.fixed) == 50) {
		fdScriptSetFlag(FLAG_VENUS_STATUE_PUSHED_OFF_LEDGE);
		actor->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	}
}

/* Nothing visible happens as a push or Move of the first grey stone starts; the
   game frees it from the ground's height so that it can slide over the gaps. */
void scMarsLighthouseRoomFStartMovingStone(Sint32 unused, Sint32 id)
{
	if(id == ACTOR_FIRST_STONE)
		fdActorGet(ACTOR_FIRST_STONE)->motionFlg = 0;
}

/* When a push or Move ends, the robed statue drops if it is at the ledge's end,
   and the first grey stone settles back onto the ground (in every edition but
   the Japanese, into a gap it fills, which Felix can then walk over). */
void scMarsLighthouseRoomFStopMovingStone(Sint32 unused, Sint32 id)
{
	if(id == ACTOR_PUSHED_STATUE) {
		scMarsLighthouseRoomFMoveVenusStatueOffLedge(
			unused, ACTOR_PUSHED_STATUE);
	} else if(id == ACTOR_FIRST_STONE) {
		fdActorGet(ACTOR_FIRST_STONE)->motionFlg =
			ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
#if MARS_LIGHTHOUSE_FILLED_GAPS_BECOME_FLOOR
		scMarsLighthouseRoomFFillGapsWithStones();
#endif
	}
}

/* Felix Moves one of the five trap statues of the Venus tower: as it slides
   aside, a dragon head that was hidden behind it shows and breathes fire. */
void scMarsLighthouseRoomFUncoverFlameThrower(Sint32 unused, Sint32 id)
{
	ACTOR *actor = fdActorGet(id);
	Sint32 flameThrower;

	flameThrower =
		MarsLighthouseRoomF_StatueFlameThrowers[id - ACTOR_TRAP_STATUE_A];
	fdActorSetPosFX(flameThrower, toFIXED(actor->homeX),
		(toFIXED(actor->homeZ)) - toFIXED(8));
}

/* Moved back to where it stood, a trap statue hides its dragon head again. */
void scMarsLighthouseRoomFCoverFlameThrower(Sint32 unused, Sint32 id)
{
	ACTOR *actor = fdActorGet(id);
	FIXED  x;
	Sint32 flameThrower;

	x = actor->x.fixed;
	flameThrower =
		MarsLighthouseRoomF_StatueFlameThrowers[id - ACTOR_TRAP_STATUE_A];
	if(FIELD_METATILE(x) == actor->tmr >> 4)
		fdActorSetPos(flameThrower, 0, 0);
}

#if MARS_LIGHTHOUSE_FILLED_GAPS_BECOME_FLOOR
/* In every edition but the Japanese: the gaps at metatiles (38, 8) and (43, 8),
   and (47, 8) for the first stone, are floor wherever a stone fills them. */
void scMarsLighthouseRoomFFillGapsWithStones(void)
{
	ACTOR *actor;
	Sint32 id;
	Sint32 x;
	Sint32 z;

	fdMapGridCopyMetatileAttributes(53, 72, 1, 1, 38, 72);
	fdMapGridCopyMetatileAttributes(53, 72, 1, 1, 43, 72);
	fdMapGridCopyMetatileAttributes(53, 8, 1, 1, 47, 8);
	for(id = ACTOR_FIRST_STONE; id <= ACTOR_THIRD_STONE; id++) {
		actor = fdActorGet(id);
		x = FIELD_METATILE(actor->x.fixed);
		z = FIELD_METATILE(actor->z.fixed);
		if(x == 38 && z == 8)
			fdMapGridCopyMetatileAttributes(54, 72, 1, 1, 38, 72);
	}
	for(id = ACTOR_FIRST_STONE; id <= ACTOR_THIRD_STONE; id++) {
		actor = fdActorGet(id);
		x = FIELD_METATILE(actor->x.fixed);
		z = FIELD_METATILE(actor->z.fixed);
		if(x == 43 && z == 8)
			fdMapGridCopyMetatileAttributes(54, 72, 1, 1, 43, 72);
	}
	actor = fdActorGet(ACTOR_FIRST_STONE);
	x = FIELD_METATILE(actor->x.fixed);
	z = FIELD_METATILE(actor->z.fixed);
	if(x == 47 && z == 8)
		fdMapGridCopyMetatileAttributes(54, 8, 1, 1, 47, 8);
}
#endif

/* Where the two pipes stand decides which of the pipe room's dragon heads
   breathe fire and which pipe ends spout it. */
void scMarsLighthouseRoomFRouteFireThroughPipes(void)
{
	if(FIELD_METATILE(fdActorGet(ACTOR_MARS_PIPE_A)->z.fixed) == 14) {
		scFlameThrowerSet(0, FLAME_THROWER_DRY | 1);
		scFlameThrowerSet(2, FLAME_EAST | 3);
	} else {
		scFlameThrowerSet(0, FLAME_EAST | 1);
		scFlameThrowerSet(2, FLAME_THROWER_DRY | 3);
	}
	if(FIELD_METATILE(fdActorGet(ACTOR_MARS_PIPE_B)->x.fixed) == 6) {
		scFlameThrowerSet(1, FLAME_THROWER_DRY | 1);
		scFlameThrowerSet(3, FLAME_FORWARD | 3);
	} else {
		scFlameThrowerSet(1, FLAME_BACKWARD | 1);
		scFlameThrowerSet(3, FLAME_THROWER_DRY | 3);
	}
}

/* Walking into one of the two pipes in the Mars tower's pipe room (the
   metatiles round them, (7, 12) to (10, 16) and (4, 20) to (7, 23)), Felix
   pushes it along, and the fire takes its new way: a pipe pushed into line
   feeds a dragon head, and one pushed out of line leaves its fire spouting from
   the open end. */
void scMarsLighthouseRoomFPushPipe(void)
{
	PUSH_TARGET box;

	if(scPushTargetFindStop(&box)) {
		scPushTargetPush(box);
		scMarsLighthouseRoomFRouteFireThroughPipes();
	}
}

/* Defines what Burst does to a cracked wall: it breaks open, drawn from the
   spare metatiles at x wallX on the ground layer and upperX on the upper layer,
   row 50, and its flag is set. */
#define DEFINE_BURST_WALL(name, wallX, upperX, flag) \
	void name(Sint32 state, Sint32 unused) \
	{ \
		if(state == 1) { \
			fdMapGridCopyMetatiles(29, 56, 3, 4, wallX, 50); \
			fdMapGridCopyMetatilesLayered(93, 56, 3, 4, upperX, 50); \
			fdScriptSetFlag(flag); \
		} \
	}

/* Felix casts Burst on the cracked west wall at the start of the Mars tower
   (metatile (11, 52)): the wall breaks open, and stays broken. */
DEFINE_BURST_WALL(
	scMarsLighthouseRoomFBurstWestWall, 10, 74, FLAG_WEST_WALL_BURST)

/* Felix casts Burst on the cracked east wall at the start of the Mars tower
   (metatile (19, 52)): the wall breaks open, and stays broken. */
DEFINE_BURST_WALL(
	scMarsLighthouseRoomFBurstEastWall, 18, 82, FLAG_EAST_WALL_BURST)

/* Felix casts Burst on the centre wall at the start of the Mars tower, once the
   urn torch's fireball has cracked it (metatile (15, 52)): the wall breaks
   open, and stays broken. */
DEFINE_BURST_WALL(
	scMarsLighthouseRoomFBurstCenterWall, 14, 78, FLAG_CENTER_WALL_BURST)

/* Felix steps on the button in the corner of the lava room of the Mars tower
   (metatile (6, 36)): with a click, the dragon head on the north wall starts
   sliding east and west along it, breathing fire down at the lava, until its
   flame lights the urn torch. */
void scMarsLighthouseRoomFPressButton(void)
{
	fdScriptPlaySnd(SE_BUTTON_PRESS);
	scFlameThrowerSet(0, FLAME_FORWARD | 3);
	fdScriptSetFlag(FLAG_BUTTON_PRESSED);
}

/* The sliding dragon head's flame reaches the unlit urn torch in the Mars
   tower, which flares up with a whoosh and stays lit from now on. */
void scMarsLighthouseRoomFLightTorch(void)
{
	ACTOR *actor = fdActorGet(ACTOR_MARS_TORCH);

	fdActorSetPos(ACTOR_UNLIT_MARS_TORCH, 0, 0);
	fdActorSetPos(ACTOR_MARS_TORCH, actor->homeX, actor->homeZ);
	fdScriptSetFlag(FLAG_MARS_TORCH_LIT);
	fdScriptPlaySnd(SE_WHOOSH);
}

/* Felix's long Blaze from the lit urn torch in the Mars tower reaches the torch
   below it, which sends a fireball rolling south toward the centre wall. */
void scMarsLighthouseRoomFBlazeTorch(void)
{
	scMarsLighthouseRoomFShootFireballAtWall();
}

extern Uint32 Debris_Script[];
extern Uint32 MarsLighthouseRoomF_RubbleScriptB[];

/* The pieces of wall scMarsLighthouseRoomFScatterRubble throws. */
#define RUBBLE_PIECES 12

/* Twelve green pieces of wall burst from an actor, fly up and bounce away down
   over the lava, then flicker out: as the Mars tower's fireball cracks the
   centre wall (scMarsLighthouseRoomFCrackCenterWall). Each piece flickers out
   after Debris_Script's wait or, at random, two frames later
   (MarsLighthouseRoomF_RubbleScriptB). */
void scMarsLighthouseRoomFScatterRubble(Sint32 id)
{
	ACTOR *origin = fdActorGet(id);
	Sint32 i;

	for(i = 0; i < RUBBLE_PIECES; i++) {
		Sint32 a = syRandom();
		FIXED  x = origin->x.fixed + (a << 4) - (syRandom() << 4);
		Sint32 b = syRandom();
		Sint32 c = syRandom();
		ACTOR *actor = scFieldActorCreate(SPRITE_ROUND_BOULDER, x,
			(b << 3) + origin->y.fixed, (c << 4) + origin->z.fixed);

		if(actor != NULL) {
			Sint32 angle = (syRandom() >> 2) + DEGtoANG(315);
			Sint32 speed = syRandom() << 3;
			Sint32 scale = (syRandom() >> 1) + toFIXED(1) / 4;

			if(syRandom() <= 32767)
				fdActorStartScript(actor, Debris_Script);
			else
				fdActorStartScript(actor, MarsLighthouseRoomF_RubbleScriptB);
			fdActorSetSprFlags(actor, 0);
			actor->velY = (angle << 2) + toFIXED(2);
			actor->velZ = syMulFX(speed, syCos(angle));
			{
				FIXED vx = syMulFX(speed, syMathSin(angle));

				actor->acceleration = 0;
				actor->scaleX = scale;
				actor->scaleY = scale;
				actor->gravity = toFIXED(33) / 100;
				actor->velX = vx;
				actor->bounce = toFIXED(1) / 2;
			}
			fdActorSetSprColMode(actor, 1);
		}
	}
}

/* The camera follows the fireball from the Mars tower's lower torch south to
   the centre wall and holds there for two seconds. */
void scMarsLighthouseRoomFWatchFireballCrackWall(void)
{
	FieldStateWork->psynergy = 0;
	fdScriptClearFlag(FLAG_MENU_OPEN);
	fdScriptClearFlag(FLAG_FIELD_HANDLING_REQUESTS);
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdCameraSetSpeed(2);
	fdCameraMoveTo(248, CAMERA_KEEP, 712, 1);
	fdCameraWaitForArrival();
	syWait(20);
	fdCameraSetSpeed(5);
	fdCameraMoveTo(248, CAMERA_KEEP, 872, 1);
	fdCameraWaitForArrival();
	syFieldScriptWait(120);
	fdScriptEnd();
}

/* The fireball cracks the Mars tower's centre wall, ready for Burst, with
   pieces of wall flying, unless Burst has broken it already. */
void scMarsLighthouseRoomFCrackCenterWall(Sint32 burst)
{
	if(!fdScriptGetFlag(FLAG_CENTER_WALL_BURST)) {
		fdMapGridCopyMetatiles(29, 60, 3, 4, 14, 50);
		fdMapGridCopyMetatilesLayered(93, 60, 3, 4, 78, 50);
		if(burst && !fdScriptGetFlag(FLAG_CENTER_WALL_CRACKED))
			scMarsLighthouseRoomFScatterRubble(ACTOR_CENTER_WALL);
		fdScriptSetFlag(FLAG_CENTER_WALL_CRACKED);
	}
}

/* Felix casts Cyclone on the west circle of the Jupiter tower's first hall
   (metatile (3, 57)): a whirlwind lifts the party spinning and sets them down
   on the circle of the small room at the foot of the long corridor. Each circle
   of the Jupiter tower has its own exit, of the number its metatiles carry. */
void scMarsLighthouseRoomFCycloneOnFirstHallWestCircle(void)
{
	syImportScriptSetExitRequest(CYCLONE_FROM_FIRST_HALL_WEST_CIRCLE);
}

/* Felix casts Cyclone on the circle of the small room at the foot of the long
   corridor (metatile (34, 16)): a whirlwind lifts the party spinning and sets
   them down on the west circle of the first hall. */
void scMarsLighthouseRoomFCycloneOnCorridorFootCircle(void)
{
	syImportScriptSetExitRequest(CYCLONE_FROM_CORRIDOR_FOOT_CIRCLE);
}

/* Felix casts Cyclone on the east circle of the Jupiter tower's first hall
   (metatile (21, 57)): a whirlwind lifts the party spinning and sets them down
   on the first circle of the narrow room. */
void scMarsLighthouseRoomFCycloneOnFirstHallEastCircle(void)
{
	syImportScriptSetExitRequest(CYCLONE_FROM_FIRST_HALL_EAST_CIRCLE);
}

/* Felix casts Cyclone on the first circle of the narrow room (metatile (41,
   40)):
   a whirlwind lifts the party spinning and sets them down on the east circle of
   the first hall. */
void scMarsLighthouseRoomFCycloneOnNarrowRoomFirstCircle(void)
{
	syImportScriptSetExitRequest(CYCLONE_FROM_NARROW_ROOM_FIRST_CIRCLE);
}

/* Felix casts Cyclone on the east circle at the head of the long corridor
   (metatile (37, 5)): a whirlwind lifts the party spinning and sets them down
   in the south-west of the big room of the white dragon statue. */
void scMarsLighthouseRoomFCycloneOnCorridorHeadEastCircle(void)
{
	syImportScriptSetExitRequest(CYCLONE_FROM_CORRIDOR_HEAD_EAST_CIRCLE);
}

/* Felix casts Cyclone on the west circle at the head of the long corridor
   (metatile (32, 5)): a whirlwind lifts the party spinning and sets them down
   in the far west of the big room. */
void scMarsLighthouseRoomFCycloneOnCorridorHeadWestCircle(void)
{
	syImportScriptSetExitRequest(CYCLONE_FROM_CORRIDOR_HEAD_WEST_CIRCLE);
}

/* Felix casts Cyclone on the raised circle in the middle of the big room
   (metatile (18, 24)): a whirlwind lifts the party spinning and sets them down
   on the first circle of the room with two circles. */
void scMarsLighthouseRoomFCycloneOnBigRoomRaisedCircle(void)
{
	syImportScriptSetExitRequest(CYCLONE_FROM_BIG_ROOM_RAISED_CIRCLE);
}

/* Felix casts Cyclone on the first circle of the room with two circles
   (metatile (31, 60)): a whirlwind lifts the party spinning and sets them down
   on the raised circle in the middle of the big room. */
void scMarsLighthouseRoomFCycloneOnTwoCircleRoomFirstCircle(void)
{
	syImportScriptSetExitRequest(CYCLONE_FROM_TWO_CIRCLE_ROOM_FIRST_CIRCLE);
}

/* Felix casts Cyclone on the second circle of the room with two circles
   (metatile (36, 57)): a whirlwind lifts the party spinning and sets them down
   on a ledge in the north-east of the big room. */
void scMarsLighthouseRoomFCycloneOnTwoCircleRoomSecondCircle(void)
{
	syImportScriptSetExitRequest(CYCLONE_FROM_TWO_CIRCLE_ROOM_SECOND_CIRCLE);
}

/* Felix casts Cyclone on the circle at the east of the big room (metatile (24,
   19)): a whirlwind lifts the party spinning and sets them down on the second
   circle of the narrow room. */
void scMarsLighthouseRoomFCycloneOnBigRoomEastCircle(void)
{
	syImportScriptSetExitRequest(CYCLONE_FROM_BIG_ROOM_EAST_CIRCLE);
}

/* Felix casts Cyclone on the second circle of the narrow room (metatile (43,
   29)):
   a whirlwind lifts the party spinning and sets them down on the circle at the
   east of the big room. */
void scMarsLighthouseRoomFCycloneOnNarrowRoomSecondCircle(void)
{
	syImportScriptSetExitRequest(CYCLONE_FROM_NARROW_ROOM_SECOND_CIRCLE);
}

/* Felix casts Cyclone on the circle west of the big room's middle (metatile
   (13, 14)): a whirlwind lifts the party spinning and sets them down on the
   circle under the great carved dragon. */
void scMarsLighthouseRoomFCycloneOnBigRoomWestCircle(void)
{
	syImportScriptSetExitRequest(CYCLONE_FROM_BIG_ROOM_WEST_CIRCLE);
}

/* Felix casts Cyclone on the circle under the great carved dragon (metatile
   (54, 12)): a whirlwind lifts the party spinning and sets them down on the
   circle west of the big room's middle. */
void scMarsLighthouseRoomFCycloneOnCarvedDragonCircle(void)
{
	syImportScriptSetExitRequest(CYCLONE_FROM_CARVED_DRAGON_CIRCLE);
}

/* Felix Moves the purple block in the Jupiter tower: once it reaches metatile
   column 13 it drops into the gap with a thud, bridging it for good. */
void scMarsLighthouseRoomFDropBlock(Sint32 unused, Sint32 id)
{
	ACTOR *actor = fdActorGet(id);

	if(FIELD_METATILE(actor->x.fixed) == 13) {
		actor->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
		if(actor->groundHeight !=
			fdMapGetTerrainHeight(
				actor->layer, actor->x.fixed, actor->z.fixed)) {
			Sint32 frames;

			syWait(2);
			for(frames = 0; frames < 30 && actor->y.fixed > actor->groundHeight;
				frames++)
				syWait(1);
			fdScriptPlaySnd(SE_IMPACT);
		}
		fdMapGridCopyMetatilesLayered(71, 24, 7, 1, 71, 5);
		fdMapGridCopyMetatileAttributes(7, 24, 7, 1, 7, 7);
		fdMapGridCopyMetatileAttributes(7, 88, 7, 1, 7, 71);
		fdScriptPlaySnd(SE_MAP_TRANSITION);
		fdActorSetPos(id, 0, 0);
		fdScriptSetFlag(FLAG_BLOCK_DROPPED);
	}
}

/* Felix Moves the white dragon statue in the Jupiter tower's big room: once it
   stands at metatile column 7, under the dragon head, it blocks the flames, and
   stays there. */
void scMarsLighthouseRoomFMoveJupiterStatueIntoFlames(Sint32 unused, Sint32 id)
{
	if(FIELD_METATILE(fdActorGet(id)->x.fixed) == 7)
		fdScriptSetFlag(FLAG_JUPITER_STATUE_BLOCKS_FLAMES);
}

/* While Felix's Reveal lasts in the Jupiter tower, the hidden stepping stones
   show, the secret way east. */
void scMarsLighthouseRoomFRevealSteppingStones(void)
{
	fdMapGridCopyMetatiles(17, 24, 3, 1, 17, 13);
	fdMapGridCopyMetatileAttributes(20, 24, 1, 1, 18, 77);
	fdMapGridCopyMetatiles(62, 20, 1, 2, 54, 20);
}

/* As Reveal fades, the Jupiter tower's stepping stones are hidden again. */
void scMarsLighthouseRoomFHideSteppingStones(void)
{
	fdMapGridCopyMetatiles(17, 25, 3, 1, 17, 13);
	fdMapGridCopyMetatileAttributes(20, 25, 1, 1, 18, 77);
	fdMapGridCopyMetatiles(62, 22, 1, 2, 54, 20);
}

/* Felix steps into the hole in the floor of the room under the great carved
   dragon in the Jupiter tower (metatiles (52, 19) to (56, 22)): he raises his
   arms, sinks out of sight with a puff of dust, and drops back to the tower's
   first hall. */
void scMarsLighthouseRoomFFallIntoHole(void)
{
	ACTOR *actor = fdActorGet(MainRoutineStateWork.leaderActorId);
	Sint32 frame;

	fdLeaderStopMoving();
	fdActorSetSprFlags(actor, 0);
	fdActorSetAnimation(
		MainRoutineStateWork.leaderActorId, ANIM_LEADER_ARMS_RAISED);
	syFieldScriptWait(16);
	for(frame = 0; frame <= 39; frame++) {
		if(frame == 5)
			fdScriptPlaySnd(SE_DUST_PUFF);
		actor->motionFlg = 0;
		actor->scaleX += -(toFIXED(1) / 64);
		actor->scaleY += -(toFIXED(3) / 256);
		actor->y.fixed += -(toFIXED(6) / 10);
		syWait(1);
	}
	fdActorGet(MainRoutineStateWork.leaderActorId)->sprStorageKind = 0;
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionFrames();
	syFieldScriptSetExitRequest(JUPITER_TOWER_THROUGH_HOLE);
}

/* Where the Mercury tower's three pipes stand decides which open pipe end
   spouts fire; with all three in line none does, and the first time that
   happens the camera pans north to show the pipeline mended and the tower is
   loaded afresh. */
void scMarsLighthouseRoomFRouteFireThroughPipeline(void)
{
	scFlameThrowerSet(2, FLAME_THROWER_DRY | 1);
	scFlameThrowerSet(1, FLAME_THROWER_DRY | 1);
	scFlameThrowerSet(0, FLAME_THROWER_DRY | 1);
	fdScriptClearFlag(FLAG_PIPELINE_MENDED);
	if(FIELD_METATILE(fdActorGet(ACTOR_MERCURY_PIPE_C)->z.fixed) != 15 &&
		FIELD_METATILE(fdActorGet(ACTOR_MERCURY_PIPE_B)->z.fixed) != 15) {
		scFlameThrowerSet(1, FLAME_WEST | 1);
		return;
	}
	if(FIELD_METATILE(fdActorGet(ACTOR_MERCURY_PIPE_B)->z.fixed) != 13) {
		scFlameThrowerSet(0, FLAME_EAST | 1);
		return;
	}
	if(FIELD_METATILE(fdActorGet(ACTOR_MERCURY_PIPE_A)->x.fixed) != 37) {
		scFlameThrowerSet(2, FLAME_BACKWARD | 1);
		return;
	}
	fdScriptSetFlag(FLAG_PIPELINE_MENDED);
	if(!fdScriptGetFlag(FLAG_SAW_PIPELINE_MENDED)) {
		fdCameraSetSpeed(2);
		fdCameraMoveTo(616, CAMERA_KEEP, 24, 1);
		syFieldScriptWait(60);
		fdActorTableSnapshot();
		syFieldScriptSetExitRequest(MERCURY_TOWER_PIPELINE_MENDED);
	}
}

/* Felix's long Blaze from the lit urn torch in the Mercury tower reaches the
   dragon head beyond it, which starts breathing fire, and the camera begins to
   move south. */
void scMarsLighthouseRoomFLightFlameThrower(void)
{
	scFlameThrowerSet(3, FLAME_FORWARD | 3);
	fdCameraSetSpeed(1);
	fdCameraMoveTo(184, CAMERA_KEEP, 560, 1);
}

/* The camera follows the Mercury tower's newly lit dragon head's flame south
   toward the urn torch by the icebergs. */
void scMarsLighthouseRoomFWatchFlameThrower(void)
{
	FIELD_STATE *state = Ram_HeapSlots->fieldState;

	fdScriptSetFlag(FLAG_MENU_OPEN);
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	state->psynergy = 0;
	fdScriptClearFlag(FLAG_MENU_OPEN);
	fdScriptClearFlag(FLAG_FIELD_HANDLING_REQUESTS);
	syFieldScriptWait(60);
	fdCameraSetSpeed(3);
	fdCameraMoveTo(184, CAMERA_KEEP, 744, 1);
	fdCameraWaitForArrival();
	syFieldScriptWait(60);
	fdScriptEnd();
}

/* Walking into one of the three pipes of the Mercury tower's pipe room (the
   metatiles round them, about (34, 7) to (40, 10)), Felix pushes it along; a
   frozen puddle the fire now reaches melts back into water. Where no pipe
   stands he hops across the gap ahead, if there is ground beyond. */
void scMarsLighthouseRoomFPushPipeOrJump(void)
{
	PUSH_TARGET box;

	if(scPushTargetFindStop(&box)) {
		scPushTargetPush(box);
		scMarsLighthouseRoomFRouteFireThroughPipeline();
		if(scFlameThrowerGetFlags(0) != FLAME_THROWER_DRY &&
			fdScriptGetFlag(FLAG_CENTER_PUDDLE_FROZEN)) {
			ACTOR *actor = fdActorGet(ACTOR_CENTER_PUDDLE);

			actor->update = NULL;
			fdActorSetSprColMode(actor, 0);
			fdActorSetAnimation(ACTOR_CENTER_PUDDLE, 6);
			fdScriptClearFlag(FLAG_CENTER_PUDDLE_FROZEN);
			scMarsLighthouseRoomFMeltPuddle(ACTOR_CENTER_PUDDLE);
		}
		if(scFlameThrowerGetFlags(2) != FLAME_THROWER_DRY &&
			fdScriptGetFlag(FLAG_UPPER_PUDDLE_FROZEN)) {
			ACTOR *actor = fdActorGet(ACTOR_UPPER_PUDDLE);

			actor->update = NULL;
			fdActorSetSprColMode(actor, 0);
			fdActorSetAnimation(ACTOR_UPPER_PUDDLE, 6);
			fdScriptClearFlag(FLAG_UPPER_PUDDLE_FROZEN);
			scMarsLighthouseRoomFMeltPuddle(ACTOR_UPPER_PUDDLE);
		}
	} else {
		Sint32		  angle;
		const Sint16 *table;
		Uint32		 *keys;

		fdActorGet(MainRoutineStateWork.leaderActorId);
		keys = &PerWork.data;
		table = &SlideOnIce_SteerAngles[STEER_STRAIGHT_ROW];
		angle = *(Sint16 *)((Uint8 *)table + (((*keys >> 4) & 0xf) << 1));
		if(angle != -1) {
			MAP_STATE *map = Ram_HeapSlots->mapGrid;
			ACTOR	  *leader = fdActorGet(MainRoutineStateWork.leaderActorId);
			Sint32	   position[3];
			Sint32	   x;
			Sint32	   z;
			Uint8	  *metatiles;

			SET_VECTOR(position,
				(leader->x.fixed & FIELD_METATILE_MASK) + toFIXED(8),
				leader->y.fixed,
				(leader->z.fixed & FIELD_METATILE_MASK) + toFIXED(8));
			syFieldVectorAddPolarOffset(16, angle, position);
			x = FIELD_METATILE(position[0]);
			z = FIELD_METATILE(position[2]);
			metatiles = Ram_MapMetatileBuffer;
			if(map != NULL)
				metatiles = (Uint8 *)map->layers[2].metatiles;
			metatiles += (x + (z << 7)) << 2;
			if(metatiles[2] != METATILE_BEHAVIOR_WALL)
				fdTryJumpForward();
		}
	}
}

extern CUE MarsLighthouseRoomF_Cues[];
extern CUE MarsLighthouseRoomF_VenusTowerCues[];
extern CUE MarsLighthouseRoomF_MarsTowerCues[];
extern CUE MarsLighthouseRoomF_JupiterTowerCues[];
extern CUE MarsLighthouseRoomF_MercuryTowerCues[];
void	   scLeaderSlideOnIceFromEdge(void);

/* Frost turns a puddle into a pillar of ice that blocks the fire's way. */
void scMarsLighthouseRoomFFreezePuddle(Sint32 id)
{
	ACTOR	  *actor = fdActorGet(id);
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	Sint32	   row = FIELD_METATILE(actor->z.fixed) << 7;
	FIXED	   x = actor->x.fixed;
	Uint8	  *metatile = (Uint8 *)map->layers[2].metatiles;
	Sint32	   offset = FIELD_METATILE(x) + row;

	metatile += offset << 2;
	metatile[2] = METATILE_BEHAVIOR_WALL;
	fdActorGet(id)->priFlg = 0;
}

/* A thawed puddle is water again. */
void scMarsLighthouseRoomFMeltPuddle(Sint32 id)
{
	ACTOR	  *actor = fdActorGet(id);
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	Sint32	   row = FIELD_METATILE(actor->z.fixed) << 7;
	FIXED	   x = actor->x.fixed;
	Uint8	  *metatile = (Uint8 *)map->layers[2].metatiles;
	Sint32	   offset = FIELD_METATILE(x) + row;

	metatile += offset << 2;
	metatile[2] = 0;
	syActorFlagsSet(&fdActorGet(id)->priFlg, ACTOR_PRIORITY_UNDERFOOT);
}

/* Defines what Frost does to a puddle: it freezes and its flag is set; unless
   the flame thrower flameThrower stands dry, the flame melts it again and the
   flag is cleared. */
#define DEFINE_FROST_PUDDLE(name, flag, flameThrower) \
	void name(Sint32 unused, Sint32 id) \
	{ \
		scMarsLighthouseRoomFFreezePuddle(id); \
		fdScriptSetFlag(flag); \
		if(scFlameThrowerGetFlags(flameThrower) != FLAME_THROWER_DRY) { \
			ACTOR *actor = fdActorGet(id); \
\
			actor->update = NULL; \
			fdActorSetSprColMode(actor, 0); \
			fdActorSetAnimation(id, 6); \
			fdScriptClearFlag(flag); \
			scMarsLighthouseRoomFMeltPuddle(id); \
		} \
	}

/* Felix casts Frost on the upper right puddle of the Mercury tower's pipe room:
   it rises into a pillar of ice, unless fire is spouting onto it, which melts
   it at once. */
DEFINE_FROST_PUDDLE(
	MarsLighthouseRoomF_FrostUpperPuddle, FLAG_UPPER_PUDDLE_FROZEN, 2)

/* Felix casts Frost on the centre puddle of the Mercury tower's pipe room: it
   rises into a pillar of ice, unless fire is spouting onto it, which melts it
   at once. */
DEFINE_FROST_PUDDLE(
	MarsLighthouseRoomF_FrostCenterPuddle, FLAG_CENTER_PUDDLE_FROZEN, 0)

/* The fireball from the Mercury tower's lower urn torch melts the icebergs in
   its way, opening the path over the black ice. */
void scMarsLighthouseRoomFMeltIcebergs(void)
{
	fdScriptSetFlag(FLAG_ICEBERGS_MELTED);
	fdMapGridCopyMetatileAttributes(24, 38, 3, 9, 10, 38);
	fdMapGridCopyMetatilesLayered(64, 64, 3, 9, 74, 36);
	fdMapGridCopyMetatilesLayered(64, 64, 3, 9, 10, 100);
	fdMapGridCopyMetatileAttributes(64, 64, 3, 9, 74, 38);
	fdMapGridCopyMetatileAttributes(64, 64, 3, 9, 10, 102);
}

/* Unused: slides on from the edge of the ice, then carries on while hit. */
UNUSED void scMarsLighthouseRoomFSlideFromEdge(void)
{
	scLeaderSlideOnIceFromEdge();
	scFireballCarryLeaderIfHit();
}

/* The current tower's cues. */
const CUE *scGetCues(void)
{
	if(MainRoutineStateWork.next == SCENE_MARS_LIGHTHOUSE_VENUS_TOWER)
		return MarsLighthouseRoomF_VenusTowerCues;
	if(MainRoutineStateWork.next == SCENE_MARS_LIGHTHOUSE_MERCURY_TOWER)
		return MarsLighthouseRoomF_MercuryTowerCues;
	if(MainRoutineStateWork.next == SCENE_MARS_LIGHTHOUSE_MARS_TOWER)
		return MarsLighthouseRoomF_MarsTowerCues;
	if(MainRoutineStateWork.next == SCENE_MARS_LIGHTHOUSE_JUPITER_TOWER)
		return MarsLighthouseRoomF_JupiterTowerCues;
	return MarsLighthouseRoomF_Cues;
}

/* Each frame in the Mercury tower: a hit by the flames stops Felix's slide. */
void scMarsLighthouseRoomFStopSlideWhileHit(void)
{
	if(scFlameThrowersHasHitLeader())
		scLeaderSetSlideStopped(1);
}

extern Uint16 MarsLighthouseRoomF_MarsButtonFlameThrowers[];
extern Uint16 MarsLighthouseRoomF_MarsButtonFlameStops[];
extern Uint16 MarsLighthouseRoomF_MarsPipeFlameThrowers[];
extern Uint16 MarsLighthouseRoomF_MarsPipeFlameStops[];
extern Uint16 MarsLighthouseRoomF_MercuryFlameThrowers[];
extern Uint16 MarsLighthouseRoomF_MercuryFlameStops[];
extern Uint16 MarsLighthouseRoomF_JupiterUpperFlameThrowers[];
extern Uint16 MarsLighthouseRoomF_JupiterUpperFlameStops[];
extern Uint16 MarsLighthouseRoomF_JupiterFlameThrowers[];
extern Uint16 MarsLighthouseRoomF_JupiterFlameStops[];
extern Uint16 MarsLighthouseRoomF_VenusFlameThrowers[];
extern Uint16 MarsLighthouseRoomF_VenusFlameStops[];
extern Uint16 MarsLighthouseRoomF_VenusStatueFlameThrowers[];
extern Uint16 MarsLighthouseRoomF_VenusStatueFlameStops[];

/* Sets up the tower being entered: the flame throwers of the part of it
   entered, and what the flags have changed (lit torches, broken walls, pipes in
   line, puddles frozen, the dropped block, statues in the flames). Returns
   0. */
Sint32 scInit(void)
{
	MAIN_ROUTINE_STATE *state = &MainRoutineStateWork;
	Sint32				scene = state->next;
	Sint32				entrance = state->entrance;
	HEAP_SLOTS		   *heap = Ram_HeapSlots;
	Sint32				value;

	((FIELD_STATE *)heap->fieldState)->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	value =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	fdScriptSetFlag(FLAG_IN_MARS_LIGHTHOUSE);
	scFlameThrowersCreate();
	if(scene == SCENE_MARS_LIGHTHOUSE_MARS_TOWER) {
		fdEffectStartBlankOverlay(0);
		((FIELD_STATE *)heap->fieldState)->transition = value;
		if(entrance ==
				MARS_LIGHTHOUSE_MARS_TOWER_FROM_MARS_LIGHTHOUSE_MARS_LEDGE ||
			entrance == MARS_TOWER_FIRST_ROOMS_DOORWAY) {
			scFlameThrowersStart(MarsLighthouseRoomF_MarsButtonFlameThrowers,
				MarsLighthouseRoomF_MarsButtonFlameStops,
				scMarsLighthouseRoomFLightTorch, STEP_CUE_FLAME_HIT);
			if(fdScriptGetFlag(FLAG_MARS_TORCH_LIT))
				fdActorSetPos(ACTOR_UNLIT_MARS_TORCH, 0, 0);
			else
				fdActorSetPos(ACTOR_MARS_TORCH, 0, 0);
			if(fdScriptGetFlag(FLAG_CENTER_WALL_CRACKED))
				scMarsLighthouseRoomFCrackCenterWall(0);
			if(fdScriptGetFlag(FLAG_WEST_WALL_BURST))
				scMarsLighthouseRoomFBurstWestWall(1, 0);
			if(fdScriptGetFlag(FLAG_EAST_WALL_BURST))
				scMarsLighthouseRoomFBurstEastWall(1, 0);
			if(fdScriptGetFlag(FLAG_CENTER_WALL_BURST))
				scMarsLighthouseRoomFBurstCenterWall(1, 0);
		} else {
			scFlameThrowersStart(MarsLighthouseRoomF_MarsPipeFlameThrowers,
				MarsLighthouseRoomF_MarsPipeFlameStops, 0, STEP_CUE_FLAME_HIT);
			fdActorDisableById(ACTOR_MARS_PIPE_A);
			fdActorDisableById(ACTOR_MARS_PIPE_B);
			scPushTargetFillRectangle(ACTOR_MARS_PIPE_A);
			scPushTargetFillRectangle(ACTOR_MARS_PIPE_B);
			scMarsLighthouseRoomFRouteFireThroughPipes();
		}
	} else if(scene == SCENE_MARS_LIGHTHOUSE_MERCURY_TOWER) {
		fdEffectStartBlankOverlay(0);
		((FIELD_STATE *)heap->fieldState)->transition = value;
		scFlameThrowersStart(MarsLighthouseRoomF_MercuryFlameThrowers,
			MarsLighthouseRoomF_MercuryFlameStops,
			scMarsLighthouseRoomFShootFireballAtIcebergs, STEP_CUE_FLAME_HIT);
		scFlameThrowerSetPuffsOffScreen(3, 1);
		if(entrance == MERCURY_TOWER_PIPE_HALL_DOORWAY)
			scFlameThrowersReleaseFlame();
		if(entrance == MERCURY_TOWER_PIPELINE_MENDED) {
			fdScriptSetFlag(FLAG_SAW_PIPELINE_MENDED);
			fdScriptStart();
			fdScriptEndLastingPsynergy(0);
			fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
			fdActorSetPos(state->leaderActorId, 0, 0);
			fdActorSetPos(ACTOR_MERCURY_TORCH, 0, 0);
			fdScriptStartScreenTransition();
			fdScriptWaitTransitionFrames();
			fdScriptWait(30);
			fdActorSetPos(ACTOR_UNLIT_MERCURY_TORCH, 0, 0);
			fdActorSetPos(ACTOR_MERCURY_TORCH, 264, 520);
			fdScriptPlaySnd(SE_WHOOSH);
			fdScriptWait(60);
			fdScriptSetFlag(FLAG_KEEP_PARTY_POSITION);
			return Value1((Sint32 (*)())syImportScriptSetExitRequest,
				MERCURY_TOWER_DRAGON_HEAD_ROOM_DOORWAY);
		}
		if(fdScriptGetFlag(FLAG_PIPELINE_MENDED)) {
			fdActorSetPos(ACTOR_MERCURY_PIPE_A, 600, 144);
			fdActorSetPos(ACTOR_MERCURY_PIPE_B, 544, 208);
			fdActorSetPos(ACTOR_MERCURY_PIPE_C, 544, 240);
			fdScriptSetFlag(FLAG_UPPER_PUDDLE_FROZEN);
			fdScriptSetFlag(FLAG_CENTER_PUDDLE_FROZEN);
		}
		fdActorDisableById(ACTOR_MERCURY_PIPE_A);
		fdActorDisableById(ACTOR_MERCURY_PIPE_B);
		fdActorDisableById(ACTOR_MERCURY_PIPE_C);
		scPushTargetFillRectangle(ACTOR_MERCURY_PIPE_A);
		scPushTargetFillRectangle(ACTOR_MERCURY_PIPE_B);
		scPushTargetFillRectangle(ACTOR_MERCURY_PIPE_C);
		fdActorGetById(ACTOR_UPPER_PUDDLE)->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
		fdActorGetById(ACTOR_CENTER_PUDDLE)->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
		if(fdScriptGetFlag(FLAG_UPPER_PUDDLE_FROZEN))
			scMarsLighthouseRoomFFreezePuddle(ACTOR_UPPER_PUDDLE);
		if(fdScriptGetFlag(FLAG_CENTER_PUDDLE_FROZEN))
			scMarsLighthouseRoomFFreezePuddle(ACTOR_CENTER_PUDDLE);
		scMarsLighthouseRoomFRouteFireThroughPipeline();
		if(fdScriptGetFlag(FLAG_PIPELINE_MENDED))
			fdActorSetPos(ACTOR_UNLIT_MERCURY_TORCH, 0, 0);
		else
			fdActorSetPos(ACTOR_MERCURY_TORCH, 0, 0);
		if(fdScriptGetFlag(FLAG_ICEBERGS_MELTED))
			scMarsLighthouseRoomFMeltIcebergs();
		sySetEvent(scMarsLighthouseRoomFStopSlideWhileHit, EVENT_PASS_FRAME);
	} else if(scene == SCENE_MARS_LIGHTHOUSE_JUPITER_TOWER) {
		fdEffectStartBlankOverlay(0);
		((FIELD_STATE *)heap->fieldState)->transition = value;
		scJupiterLighthouseLoadPalettes(LIGHTHOUSE_PALETTES_LIT);
		scLightSprsInitB();
		scLightSprsSetPriority(2);
		scLightSprsAdd(ACTOR_DROPPING_BLOCK, 6);
		if(fdScriptGetFlag(FLAG_BLOCK_DROPPED)) {
			fdMapGridCopyMetatilesLayered(71, 24, 7, 1, 71, 5);
			fdMapGridCopyMetatileAttributes(7, 24, 7, 1, 7, 7);
			fdMapGridCopyMetatileAttributes(7, 88, 7, 1, 7, 71);
			fdActorSetPos(ACTOR_DROPPING_BLOCK, 0, 0);
		}
		if(fdScriptGetFlag(FLAG_JUPITER_STATUE_BLOCKS_FLAMES))
			fdActorSetPos(ACTOR_JUPITER_STATUE, 120, 296);
		if(entrance == JUPITER_TOWER_CARVED_DRAGON_CIRCLE ||
			entrance ==
				MARS_LIGHTHOUSE_JUPITER_TOWER_FROM_MARS_LIGHTHOUSE_BEACON_LEDGE) {
			scFlameThrowersStart(MarsLighthouseRoomF_JupiterUpperFlameThrowers,
				MarsLighthouseRoomF_JupiterUpperFlameStops,
				scMarsLighthouseRoomFDropFireball, STEP_CUE_FLAME_HIT);
			scFlameThrowerSetPuffsOffScreen(0, 1);
		} else {
			scFlameThrowersStart(MarsLighthouseRoomF_JupiterFlameThrowers,
				MarsLighthouseRoomF_JupiterFlameStops, 0, STEP_CUE_FLAME_HIT);
		}
		/* On some of Cyclone's landings the party spins down from above
		   (fdActorDepartInRing), on the others it shows in a widening whirlwind
		   (fdActorArriveInRing). */
		if(!fdScriptGetFlag(FLAG_KEEP_PARTY_POSITION)) {
			if(entrance == JUPITER_TOWER_FIRST_HALL_WEST_CIRCLE ||
				entrance == JUPITER_TOWER_FIRST_HALL_EAST_CIRCLE ||
				entrance == JUPITER_TOWER_BIG_ROOM_SOUTHWEST ||
				entrance == JUPITER_TOWER_BIG_ROOM_FAR_WEST ||
				entrance == JUPITER_TOWER_BIG_ROOM_RAISED_CIRCLE ||
				entrance == JUPITER_TOWER_BIG_ROOM_NORTHEAST_LEDGE ||
				entrance == JUPITER_TOWER_BIG_ROOM_EAST_CIRCLE ||
				entrance == JUPITER_TOWER_BIG_ROOM_WEST_CIRCLE)
				fdActorDepartInRing(MainRoutineStateWork.leaderActorId);
			if(entrance == JUPITER_TOWER_CORRIDOR_FOOT_CIRCLE ||
				entrance == JUPITER_TOWER_NARROW_ROOM_FIRST_CIRCLE ||
				entrance == JUPITER_TOWER_TWO_CIRCLE_ROOM_FIRST_CIRCLE ||
				entrance == JUPITER_TOWER_NARROW_ROOM_SECOND_CIRCLE ||
				entrance == JUPITER_TOWER_CARVED_DRAGON_CIRCLE)
				fdActorArriveInRing(MainRoutineStateWork.leaderActorId);
			if(entrance == JUPITER_TOWER_THROUGH_HOLE &&
				!fdScriptGetFlag(FLAG_KEEP_PARTY_POSITION))
				scCrackedTileDropIn();
		}
	} else if(scene == SCENE_MARS_LIGHTHOUSE_VENUS_TOWER) {
		fdEffectStartBlankOverlay(0);
		((FIELD_STATE *)heap->fieldState)->transition = value;
		fdTightropeInit();
		fdTightropePlace(
			0, ACTOR_TIGHTROPE_NEAR_POST, ACTOR_TIGHTROPE_FAR_POST);
		fdActorGetById(ACTOR_TIGHTROPE_NEAR_POST)->sceneCnt = 1;
		fdActorGetById(ACTOR_TIGHTROPE_FAR_POST)->sceneCnt = 1;
		if(entrance ==
				MARS_LIGHTHOUSE_VENUS_TOWER_FROM_MARS_LIGHTHOUSE_BEACON_LEDGE ||
			entrance == VENUS_TOWER_HALL_WEST_DOORWAY ||
			entrance == VENUS_TOWER_HALL_EAST_DOORWAY) {
			scFlameThrowersStart(MarsLighthouseRoomF_VenusFlameThrowers,
				MarsLighthouseRoomF_VenusFlameStops, 0, STEP_CUE_FLAME_HIT);
#if MARS_LIGHTHOUSE_FILLED_GAPS_BECOME_FLOOR
			scMarsLighthouseRoomFFillGapsWithStones();
#endif
		} else if(entrance == VENUS_TOWER_UPPER_DOORWAY ||
			entrance == VENUS_TOWER_STATUE_HALL_EAST_DOORWAY ||
			entrance == VENUS_TOWER_STATUE_HALL_SOUTH_PASSAGE ||
			entrance ==
				MARS_LIGHTHOUSE_VENUS_TOWER_FROM_MARS_LIGHTHOUSE_VENUS_LEDGE ||
			entrance == VENUS_TOWER_LOWER_DOORWAY) {
			scFlameThrowersStart(MarsLighthouseRoomF_VenusStatueFlameThrowers,
				MarsLighthouseRoomF_VenusStatueFlameStops, 0,
				STEP_CUE_FLAME_HIT);
		}
		if(!fdScriptGetFlag(FLAG_KEEP_PARTY_POSITION)) {
			if(entrance == VENUS_TOWER_LOWER_DOORWAY)
				fdActorSetPos(ACTOR_LOWER_DOORWAY_STATUE, 664, 712);
			if(entrance == VENUS_TOWER_UPPER_DOORWAY)
				fdActorSetPos(ACTOR_UPPER_DOORWAY_STATUE, 536, 552);
		}
		fdActorGetById(ACTOR_PUSHED_STATUE)->motionFlg = 0;
		if(fdScriptGetFlag(FLAG_VENUS_STATUE_PUSHED_OFF_LEDGE)) {
			fdActorSetPos(ACTOR_PUSHED_STATUE, 808, 312);
			fdActorGetById(ACTOR_PUSHED_STATUE)->y.fixed = 0;
			fdActorGetById(ACTOR_PUSHED_STATUE)->groundHeight = 0;
		}
		fdActorGetById(MainRoutineStateWork.leaderActorId)->priFlg |=
			ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
		fdActorGetById(ACTOR_FIRST_STONE)->priFlg |=
			ACTOR_PRIORITY_WALKS_ON_PLATFORMS | ACTOR_PRIORITY_PLATFORM;
		fdActorGetById(ACTOR_SECOND_STONE)->priFlg |= ACTOR_PRIORITY_PLATFORM;
		fdActorGetById(ACTOR_THIRD_STONE)->priFlg |= ACTOR_PRIORITY_PLATFORM;
	}
	return 0;
}

extern Sint32 MarsLighthouseRoomF_FireballToWall[];
extern Sint32 MarsLighthouseRoomF_FireballToIcebergs[];
extern Sint32 MarsLighthouseRoomF_FireballDrop[];

Sint32 scPrepareMap(void)
{
	MAIN_ROUTINE_STATE *party;
	Sint32				flag = FLAG_MARS_LIGHTHOUSE_THAWED;

	party = &MainRoutineStateWork;
	if(fdScriptGetFlag(flag))
		fdMapCopyPalette(
			party->mapId + MARS_LIGHTHOUSE_THAWED_MAPS, (Sint32)BG_PLTT);
	return 0;
}

/* The Mars tower's lower urn torch sends its fireball south at the centre
   wall. */
void scMarsLighthouseRoomFShootFireballAtWall(void)
{
	scActorAttach(ACTOR_WALL_TORCH, ACTOR_WALL_SPARKLE, ACTOR_WALL_FIREBALL,
		FACING_SOUTH, MarsLighthouseRoomF_FireballToWall);
}

/* The dragon head's flame reaches the Mercury tower's lower urn torch, which
   sends its fireball south at the icebergs. */
void scMarsLighthouseRoomFShootFireballAtIcebergs(void)
{
	scActorAttach(ACTOR_ICEBERG_TORCH, ACTOR_ICEBERG_SPARKLE,
		ACTOR_ICEBERG_FIREBALL, FACING_SOUTH,
		MarsLighthouseRoomF_FireballToIcebergs);
	scFlameThrowerSet(3, FLAME_SPARK | 3);
}

/* The flame reaches the Jupiter tower's urn torch above the great carved
   dragon, whose fireball drops to the floor below. */
void scMarsLighthouseRoomFDropFireball(void)
{
	scActorAttach(ACTOR_DROP_TORCH, ACTOR_DROP_SPARKLE, ACTOR_DROP_FIREBALL,
		FACING_SOUTH, MarsLighthouseRoomF_FireballDrop);
}

#include "shared/scene_set_arrival_flags.c"

extern Sint32 PaletteCyclePausesInScripts;

#include "shared/palette_cycle_set_pauses_in_scripts.h"

extern Sint32 PaletteCycleFrame;

/* Sets the palette cycle's frame. */
static inline void scPaletteCycleSetFrame(Sint32 value)
{
	PaletteCycleFrame = value;
}

#include "shared/palette_cycle_step.h"

#include "shared/jupiter_lighthouse_load_palettes.h"

#include "shared/actor_raise_from_metatile.h"

#include "shared/whirlwind_carry_leader.h"

#include "shared/position_flat_distance.h"

extern Sint32 WhirlwindEndScript[];

#include "shared/whirlwind_update.h"

#include "shared/whirlwind_spawn.h"

#include "shared/actor_emit_smoke_b.h"

#include "shared/jupiter_lighthouse_place_raised_actors.h"

#include "shared/actor_is_touching.h"

#include "shared/actor_is_beside.h"

#include "shared/flame_update_forward.h"

extern Sint32 ScriptedPart_Script[];

#include "shared/actor_reset_scripted_for.h"

#include "shared/flame_thrower_send_forward.c"

#include "shared/flame_update_sideways.h"

void scFlameThrowerUpdateSpark(ACTOR *actor);

#include "shared/actor_setup_scripted_part.h"

#include "shared/flame_thrower_send_backward.c"

#include "shared/flame_thrower_send_east.c"

#include "shared/flame_thrower_send_west.c"

#include "shared/actor_is_above.h"

#include "shared/flame_thrower_update_spark.h"

#include "shared/actor_setup_scripted_for.h"

#include "shared/flame_thrower_send_spark.c"

#include "shared/flame_throwers_puff.c"

#include "shared/flame_throwers_create.c"

void scActorSetupScriptedFor(ACTOR *actor, Sint32 unused);

#include "shared/flame_throwers_release_flame.h"

UNUSED void scFlameThrowersSetMode(Sint16 value)
{
	FLAME_THROWERS_STATE->runsDuringPsynergy = value;
}

#include "shared/beam_start.h"

#include "shared/flame_thrower_is_active.c"

#include "shared/flame_thrower_set_active.c"

UNUSED Sint32 scFlameThrowersGetCounter(void)
{
	return FLAME_THROWERS_STATE->cnt;
}

#include "shared/flame_throwers_advance_counter.h"

#include "shared/flame_thrower_set.h"

#include "shared/flame_thrower_set_puffs_off_screen.c"

#include "shared/flame_thrower_get_flags.c"

#include "shared/flame_throwers_set_stop.h"

#include "shared/party_take_fall_damage.h"

#include "../field/field_effect_advance_spin.c"

#include "shared/actor_blink.h"

#include "shared/leader_knock_back.c"

/* Hit by the flames, Felix is knocked back. */
void scMarsLighthouseRoomFKnockBackIfHit(void)
{
	if(FLAME_THROWERS_STATE->active)
		fdLeaderKnockBack();
}

/* A rolling fireball hits Felix in the Jupiter tower: the party takes damage,
   and he sits blinking on the fireball and is carried along with it until it
   stops. */
Sint32 scFireballCarryLeader(void)
{
	FIELD_STATE		*event = Ram_HeapSlots->fieldState;
	FLAME_HIT_STATE *ride;
	ACTOR			*leader;
	/* LEFTOVER(frame): an unused local that only sizes the frame; as no local,
	 * the frame is smaller. */
	UNUSED Sint32 unused[3];

	if(event->transitionActive == 0) {
		fdScreenTransitionStart(event->transition, event->transitionFrm);
		event->transitionActive = 1;
	}
	fdScriptClearFlag(FLAG_MENU_OPEN);
	event->psynergy = 0;
	event->fieldMenu = 0;
	event->pauseMenu = 0;
	event->aButton = 0;
	event->encounter = 0;
	/* UB: gmPartyTakeFallDamage takes no arguments and returns nothing; it is
	   called with two through an int-returning type. */
#if FIX_FUNCTION_SIGNATURES
	gmPartyTakeFallDamage();
#else
	((Sint32 (*)(Sint32, Sint32))gmPartyTakeFallDamage)(-FALL_DAMAGE, 0);
#endif
	fdCameraSetSpeed(7);
	leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	ride = FLAME_THROWERS_STATE;
	fdScriptSetFlag(FLAG_MENU_OPEN);
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdScriptClearFlag(FLAG_MENU_OPEN);
	leader->update = (OBJECT_UPDATE)fdActorBlink;
	leader->facing = FACING_SOUTH;
	fdActorSelectAnimation(leader, ANIM_LEADER_SIT);
	while(ride->car->tmr == 0) {
		if(fdMapGridGetTerrainKindOrBumpAt(
			   0, leader->x.fixed, leader->z.fixed) != TERRAIN_ROUNDED_SQUARE) {
			leader->x.fixed += ride->car->stepX;
			leader->z.fixed += ride->car->stepZ;
		} else {
			if(ride->car->stepX != 0)
				leader->x.fixed =
					(leader->x.fixed & FIELD_METATILE_MASK) + toFIXED(8);
			if(ride->car->stepZ != 0)
				leader->z.fixed =
					(leader->z.fixed & FIELD_METATILE_MASK) + toFIXED(8);
		}
		ride->active = 0;
		syWait(1);
	}
	syFieldScriptWait(30);
	leader->update = NULL;
	fdActorSetSprColMode(leader, 0);
	ride->active = 0;
#if FIX_FUNCTION_SIGNATURES
	return 0;
#endif
}

#include "shared/fireball_carry_leader_if_hit.h"

/* Whether Felix has been hit by the flames. */
Sint32 scFlameThrowersHasHitLeader(void)
{
	return FLAME_THROWERS_STATE->active;
}

extern Uint32 Crumbs_Script[];

#include "shared/actor_scatter_crumbs.c"

extern Uint32 IceChips_Script[];

#include "shared/ice_chips_spawn.h"

#include "shared/actor_hold_during_script.c"
#undef position
#undef STATE

void scActorScatterCrumbs(ACTOR *actor);
void scIceChipsSpawn(Sint32 x, Sint32 y, Sint32 z, Sint32 speed);

#include "shared/actor_roll_and_crumble.c"

void scActorHoldDuringScript(ACTOR *actor);

#include "shared/actor_attach.c"

void scActorRollAndCrumble(ACTOR *actor);

#include "shared/actor_launch_ahead.h"

#include "shared/actor_pop_up_slow.h"

#include "shared/actor_pop_up.h"

void scActorSetupScriptedFor(ACTOR *actor, Sint32 unused);

#include "shared/actor_park.h"

#include "../field/actor_set_sprite_priority.c"

#include "../field/field_effect_move.c"

#undef DEFINE_BURST_WALL
#undef DEFINE_FROST_PUDDLE
#undef IWRAM
#undef position
#undef STATE
#undef MapLayer
extern const Sint32 *FieldEffect_DurationScripts[];
#define EFFECT_SET_MODE	  fdActorSelectAnimation
#define EFFECT_SET_SCRIPT fdActorStartScript
#include "../field/field_effect_spawn.c"
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT

#define METATILE_PUSH_SLOT_ENTRY syOAMAddSprAtDepthClamped
#include "../field/cracked_tile_mask.c"

#include "shared/cracked_tile_find.h"

#include "shared/leader_center_in_metatile.h"

#include "shared/cracked_tile_crack.h"

#include "shared/cracked_tile_fall.h"

#include "../field/field_effect_advance_motion.c"

#undef METATILE_PUSH_SLOT_ENTRY

#include "shared/cracked_tile_drop_in.h"

void syGameFlagSetByte(Sint32 flag, Sint32 value);

#include "shared/cracked_tile_step.h"

#include "shared/cracked_tile_place_landing.h"

extern Sint16 SlideOnIce_SteerAngles[];

#include "shared/actor_steer_by_pad.h"

#include "shared/leader_stop_if_pending.h"

extern Sint16 SlideOnIce_AnglesByKey[];

/* This room's slides reach fdCheckSpheresOverlap through the overlay's first
   import of it, syImportFieldCheckSpheresOverlap. */
#define fdCheckSpheresOverlap syImportFieldCheckSpheresOverlap
#include "shared/leader_slide_on_ice.h"

#include "shared/slide_on_ice_from_edge.c"

/* Stops the leader's slide on the ice where it is (nonzero), or lets it run. */
void scLeaderSetSlideStopped(Sint32 value)
{
	SlideOnIce_Stopped = value;
}

#include "shared/leader_slide_on_ice_ahead.h"
#undef fdCheckSpheresOverlap

extern Uint16 PushKinds[PUSH_KINDS];
extern Sint8  PushRectangles[][4];

#include "shared/push_target_fill_rectangle.h"

/* Sets the metatile behavior of a rectangle of one layer's metatiles. */
void fdMapGridFillCollisionRectangle(
	Sint32 layer, Sint32 x, Sint32 z, Uint32 width, Uint32 height, Sint32 value)
{
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	Uint8	  *metatiles = (Uint8 *)map->layers[layer].metatiles;
	Uint32	   row;
	Uint32	   column;

	metatiles += (x + (z << 7)) << 2;
	for(row = 0; row < height; row++) {
		Uint8 *metatile = metatiles + (row << 9);

		for(column = 0; column < width; column++) {
			metatile[2] = value;
			metatile += 4;
		}
	}
}

extern Sint32 PushLeaderSteps[];

static inline Sint32 scPushTargetCheckCollision(ACTOR *actor, Sint32 *position)
{
	return fdMapGridCheckCollision(actor, position);
}

#include "shared/push_target_find_stop.h"

#include "shared/push_target_find_ahead.h"

#include "shared/push_target_push.h"

#include "shared/actor_stop_at_push_target.h"

#include "shared/light_sprites_draw.h"

#include "shared/light_sprites_init_a.h"

#include "shared/light_sprites_init_b.h"

#include "shared/light_sprites_init_c.h"

#include "shared/light_sprites_add.h"

/* Sets the priority the glow sprites are drawn at. */
void scLightSprsSetPriority(Sint32 value)
{
	LightSpritePriority = value;
}

/* The towers' torches' script. */
Sint32 MarsLighthouseRoomF_TorchScript[] = {
	SC_CALL(scMarsLighthouseRoomFSetUpTorch),
	SC_END,
};

/* clang-format off */
/* The Venus tower's moving flame throwers: each slides east and west along */
/* the wall, pausing at each end. */
Sint32 MarsLighthouseRoomF_VenusEastFlameThrowerRoute[] = {
	SC_SET(SW_STEERING, 0),
	SC_SET(SW_MOTION, ACTOR_FALLS),
	SC_SPEED(0.5),
	SC_ACCELERATION(0.03125),
	SC_MOVE(680, 0, 304),
	SC_SYNC,
	SC_WAIT(20),
	SC_MOVE(760, 0, 304),
	SC_SYNC,
	SC_WAIT(20),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

Sint32 MarsLighthouseRoomF_VenusWestFlameThrowerRoute[] = {
	SC_SET(SW_STEERING, 0),
	SC_SET(SW_MOTION, ACTOR_FALLS),
	SC_SPEED(0.5),
	SC_ACCELERATION(0.03125),
	SC_MOVE(584, 0, 304),
	SC_SYNC,
	SC_WAIT(20),
	SC_MOVE(664, 0, 304),
	SC_SYNC,
	SC_WAIT(20),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

/* The Mars tower's button flame thrower: raised on the wall, it waits for */
/* the button (FLAG_BUTTON_PRESSED), then slides east and west for good. */
Sint32 MarsLighthouseRoomF_ButtonFlameThrowerRoute[] = {
	SC_SET(SW_STEERING, 0),
	SC_SET(SW_MOTION, ACTOR_FALLS),
	SC_SET(SW_Y, toFIXED(48)),
	SC_SET(SW_GROUND, toFIXED(48)),
	SC_SPEED(0.5),
	SC_ACCELERATION(0.03125),
	SC_LABEL(1),
	SC_WAIT(1),
	SC_FLAG_CHECK(FLAG_BUTTON_PRESSED),
	SC_IFNOT(SC_TO(1)),
	SC_LABEL(2),
	SC_MOVE(176, 0, 576),
	SC_SYNC,
	SC_MOVE(248, 0, 576),
	SC_SYNC,
	SC_WAIT(20),
	SC_MOVE(176, 0, 576),
	SC_SYNC,
	SC_MOVE(104, 0, 576),
	SC_SYNC,
	SC_WAIT(20),
	SC_LOOP(SC_FOREVER, SC_TO(2)),
};

/* The Jupiter tower's moving flame throwers, each back and forth along its */
/* wall; the east one also rises and drops. */
Sint32 MarsLighthouseRoomF_JupiterWestFlameThrowerRoute[] = {
	SC_SET(SW_STEERING, 0),
	SC_SET(SW_MOTION, ACTOR_FALLS),
	SC_SET(SW_Y, toFIXED(32)),
	SC_SET(SW_GROUND, toFIXED(32)),
	SC_SPEED(0.5),
	SC_ACCELERATION(0.03125),
	SC_MOVE(88, 0, 112),
	SC_SYNC,
	SC_WAIT(20),
	SC_LABEL(1),
	SC_MOVE(168, 0, 112),
	SC_SYNC,
	SC_WAIT(20),
	SC_MOVE(88, 0, 112),
	SC_SYNC,
	SC_WAIT(20),
	SC_LOOP(SC_FOREVER, SC_TO(1)),
};

Sint32 MarsLighthouseRoomF_JupiterEastFlameThrowerRoute[] = {
	SC_SET(SW_STEERING, 0),
	SC_SET(SW_MOTION, 0),
	SC_SET(SW_Y, toFIXED(32)),
	SC_SET(SW_GROUND, toFIXED(32)),
	SC_SPEED(0.5),
	SC_ACCELERATION(0.03125),
	SC_LABEL(1),
	SC_MOVE(264, 32, 112),
	SC_SYNC,
	SC_WAIT(20),
	SC_MOVE(264, 80, 112),
	SC_SYNC,
	SC_WAIT(20),
	SC_MOVE(232, 80, 112),
	SC_SYNC,
	SC_WAIT(20),
	SC_MOVE(264, 80, 112),
	SC_SYNC,
	SC_WAIT(20),
	SC_MOVE(264, 32, 112),
	SC_SYNC,
	SC_WAIT(20),
	SC_MOVE(360, 32, 112),
	SC_SYNC,
	SC_WAIT(20),
	SC_LOOP(SC_FOREVER, SC_TO(1)),
};

/* The Jupiter tower's flame thrower that stays put, raised on its wall. */
Sint32 MarsLighthouseRoomF_JupiterLowerFlameThrowerSetUp[] = {
	SC_SET(SW_STEERING, 0),
	SC_SET(SW_MOTION, ACTOR_FALLS),
	SC_SET(SW_Y, toFIXED(32)),
	SC_SET(SW_GROUND, toFIXED(32)),
	SC_SPEED(0.5),
	SC_ACCELERATION(0.03125),
	SC_END,
};

Sint32 MarsLighthouseRoomF_JupiterFarFlameThrowerRoute[] = {
	SC_SET(SW_STEERING, 0),
	SC_SET(SW_MOTION, ACTOR_FALLS),
	SC_SET(SW_Y, toFIXED(48)),
	SC_SET(SW_GROUND, toFIXED(48)),
	SC_SPEED(0.5),
	SC_ACCELERATION(0.03125),
	SC_MOVE(968, 0, 80),
	SC_SYNC,
	SC_WAIT(20),
	SC_LABEL(1),
	SC_MOVE(872, 0, 80),
	SC_SYNC,
	SC_MOVE(776, 0, 80),
	SC_SYNC,
	SC_WAIT(20),
	SC_MOVE(872, 0, 80),
	SC_SYNC,
	SC_MOVE(968, 0, 80),
	SC_SYNC,
	SC_WAIT(20),
	SC_LOOP(SC_FOREVER, SC_TO(1)),
};

/* The flame thrower each of the Venus tower's five trap statues hides. */
Sint16 MarsLighthouseRoomF_StatueFlameThrowers[] = {
	ACTOR_HIDDEN_FLAME_THROWER_A, ACTOR_HIDDEN_FLAME_THROWER_B,
	ACTOR_HIDDEN_FLAME_THROWER_C, ACTOR_HIDDEN_FLAME_THROWER_A,
	ACTOR_HIDDEN_FLAME_THROWER_B, 0,
};

#include "shared/debris_script.c"

/* As Debris_Script, two frames later. */
Uint32 MarsLighthouseRoomF_RubbleScriptB[] = {
	SC_WAIT(42),
	SC_LABEL(1),
	SC_SET(SW_SPRITE_KIND, ACTOR_SPRITE_NONE),
	SC_WAIT(1),
	SC_SET(SW_SPRITE_KIND, ACTOR_SPRITE_ONE),
	SC_WAIT(1),
	SC_LOOP(10, SC_TO(1)),
	SC_RELEASE,
};

/* Each part of the towers' flame throwers for scFlameThrowersStart, each */
/* with its flame's kind and reach, and the actors their flames stop at, 1 */
/* where reaching it calls the function scFlameThrowersStart is given. */
Uint16 MarsLighthouseRoomF_MarsButtonFlameThrowers[] = {
	ACTOR_BUTTON_FLAME_THROWER, FLAME_THROWER_DRY | 3, 0,
};
Uint16 MarsLighthouseRoomF_MarsButtonFlameStops[] = {
	ACTOR_UNLIT_MARS_TORCH, 1, 0,
};
Uint16 MarsLighthouseRoomF_MarsPipeFlameThrowers[] = {
	ACTOR_PIPE_FLAME_A, FLAME_EAST | 1,
	ACTOR_PIPE_FLAME_B, FLAME_BACKWARD | 1,
	ACTOR_MARS_PIPE_A_FLAME_THROWER, FLAME_EAST | 3,
	ACTOR_MARS_PIPE_B_FLAME_THROWER, FLAME_FORWARD | 3,
	0,
};
Uint16 MarsLighthouseRoomF_MarsPipeFlameStops[] = {
	ACTOR_PIPE_STATUE, 0, 0,
};
Uint16 MarsLighthouseRoomF_MercuryFlameThrowers[] = {
	ACTOR_MERCURY_PIPE_FLAME_A, FLAME_EAST | 1,
	ACTOR_MERCURY_PIPE_FLAME_B, FLAME_WEST | 1,
	ACTOR_MERCURY_PIPE_FLAME_C, FLAME_BACKWARD | 1,
	ACTOR_MERCURY_FLAME_THROWER, FLAME_SPARK | 3,
	0,
};
Uint16 MarsLighthouseRoomF_MercuryFlameStops[] = {
	ACTOR_ICEBERG_TORCH, 1, 0,
};
Uint16 MarsLighthouseRoomF_JupiterUpperFlameThrowers[] = {
	ACTOR_UPPER_FLAME_THROWER, FLAME_FORWARD | 3, 0,
};
Uint16 MarsLighthouseRoomF_JupiterUpperFlameStops[] = {
	ACTOR_DROP_TORCH, 1, 0,
};
Uint16 MarsLighthouseRoomF_JupiterFlameThrowers[] = {
	ACTOR_JUPITER_FLAME_THROWER_A, FLAME_FORWARD | 3,
	ACTOR_JUPITER_FLAME_THROWER_B, FLAME_FORWARD | 3,
	ACTOR_JUPITER_FLAME_THROWER_C, FLAME_FORWARD | 3,
	0,
};
Uint16 MarsLighthouseRoomF_JupiterFlameStops[] = {
	ACTOR_JUPITER_STATUE, 0, 0,
};
Uint16 MarsLighthouseRoomF_VenusFlameThrowers[] = {
	ACTOR_VENUS_FLAME_THROWER_A, FLAME_FORWARD | 3,
	ACTOR_VENUS_FLAME_THROWER_B, FLAME_FORWARD | 3,
	ACTOR_VENUS_FLAME_THROWER_C, FLAME_FORWARD | 3,
	0,
};
Uint16 MarsLighthouseRoomF_VenusFlameStops[] = {
	ACTOR_FIRST_STONE, 0, 0,
};
Uint16 MarsLighthouseRoomF_VenusStatueFlameThrowers[] = {
	ACTOR_HIDDEN_FLAME_THROWER_A, FLAME_FORWARD | 3,
	ACTOR_HIDDEN_FLAME_THROWER_B, FLAME_FORWARD | 3,
	ACTOR_HIDDEN_FLAME_THROWER_C, FLAME_FORWARD | 3,
	0,
};
Uint16 MarsLighthouseRoomF_VenusStatueFlameStops[] = {
	ACTOR_TRAP_STATUE_A, 0, ACTOR_TRAP_STATUE_B, 0, ACTOR_TRAP_STATUE_C, 0,
	ACTOR_TRAP_STATUE_D, 0, ACTOR_TRAP_STATUE_E, 0, 0,
};

/* The fireballs' scripts: each pops up off its torch and drops just ahead of */
/* it. */
Sint32 MarsLighthouseRoomF_FireballToWall[] = {
	SC_CALL(scActorPopUp),
	SC_MOVE(248, 32, 672),
	SC_SYNC,
	SC_MOVE(248, 22, 672),
	SC_SYNC,
	SC_MOVE(248, 22, 688),
	SC_SYNC,
	SC_SET(SW_WATCHED_ACTOR, (Sint32)scMarsLighthouseRoomFCrackCenterWall),
	SC_CALL(scActorPark),
	SC_END,
};

Sint32 MarsLighthouseRoomF_FireballToIcebergs[] = {
	SC_CALL(scActorPopUp),
	SC_MOVE(184, 64, 560),
	SC_SYNC,
	SC_MOVE(184, 54, 560),
	SC_SYNC,
	SC_MOVE(184, 54, 576),
	SC_SYNC,
	SC_SET(SW_WATCHED_ACTOR, (Sint32)scMarsLighthouseRoomFMeltIcebergs),
	SC_CALL(scActorPark),
	SC_END,
};

Sint32 MarsLighthouseRoomF_FireballDrop[] = {
	SC_CALL(scActorPopUp),
	SC_MOVE(872, 32, 128),
	SC_SYNC,
	SC_MOVE(872, 22, 128),
	SC_SYNC,
	SC_MOVE(872, 22, 144),
	SC_SYNC,
	SC_CALL(scActorPark),
	SC_END,
};

#include "shared/mars_lighthouse_part_scripts.c"

#include "shared/field_effect_duration_scripts.c"

#include "shared/slide_on_ice_angles.c"

#include "shared/push_kinds.c"

#include "shared/push_leader_steps.c"
/* clang-format on */

#undef MapLayer

/* The stones fill the gaps in every edition but the Japanese; in the Japanese
   edition their rows run nothing. */
#if !MARS_LIGHTHOUSE_FILLED_GAPS_BECOME_FLOOR
#define MARS_LIGHTHOUSE_ROOM_F_FILL_GAPS_WITH_STONES 0
#else
#define MARS_LIGHTHOUSE_ROOM_F_FILL_GAPS_WITH_STONES \
	((Sint32)scMarsLighthouseRoomFFillGapsWithStones)
#endif

LATE_DATA SCENE_ENTRANCE SceneEntrances[] = {
	{0, CONDITION_ALWAYS, 192, 0, 192, FACING_WEST, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

LATE_DATA Uint32 SceneExits[] = {
	SCENE_EXITS(MarsLighthouseMarsTower),
	SCENE_EXIT(MARS_LIGHTHOUSE_MARS_TOWER_TO_MARS_LIGHTHOUSE_MARS_LEDGE,
		MarsLighthouseMarsLedge,
		MARS_LIGHTHOUSE_MARS_LEDGE_FROM_MARS_LIGHTHOUSE_MARS_TOWER),
	SCENE_EXIT(MARS_TOWER_FIRST_ROOMS_DOORWAY, MarsLighthouseMarsTower,
		MARS_TOWER_PIPE_ROOM_DOORWAY),
	SCENE_EXIT(MARS_TOWER_PIPE_ROOM_DOORWAY, MarsLighthouseMarsTower,
		MARS_TOWER_FIRST_ROOMS_DOORWAY),
	SCENE_EXIT(MARS_LIGHTHOUSE_MARS_TOWER_TO_MARS_LIGHTHOUSE_BEACON_LEDGE,
		MarsLighthouseBeaconLedge,
		MARS_LIGHTHOUSE_BEACON_LEDGE_FROM_MARS_LIGHTHOUSE_MARS_TOWER),
	SCENE_EXITS(MarsLighthouseMercuryTower),
	SCENE_EXIT(MARS_LIGHTHOUSE_MERCURY_TOWER_TO_MARS_LIGHTHOUSE_MERCURY_LEDGE,
		MarsLighthouseMercuryLedge,
		MARS_LIGHTHOUSE_MERCURY_LEDGE_FROM_MARS_LIGHTHOUSE_MERCURY_TOWER),
	SCENE_EXIT(MERCURY_TOWER_PIPE_HALL_DOORWAY, MarsLighthouseMercuryTower,
		MERCURY_TOWER_DRAGON_HEAD_ROOM_DOORWAY),
	SCENE_EXIT(MERCURY_TOWER_DRAGON_HEAD_ROOM_DOORWAY,
		MarsLighthouseMercuryTower, MERCURY_TOWER_PIPE_HALL_DOORWAY),
	SCENE_EXIT(4, MarsLighthouseMercuryTower, 5),
	SCENE_EXIT(5, MarsLighthouseMercuryTower, 4),
	SCENE_EXIT(6, MarsLighthouseMercuryTower, 7),
	SCENE_EXIT(7, MarsLighthouseMercuryTower, 6),
	SCENE_EXIT(MARS_LIGHTHOUSE_MERCURY_TOWER_TO_MARS_LIGHTHOUSE_BEACON_LEDGE,
		MarsLighthouseBeaconLedge,
		MARS_LIGHTHOUSE_BEACON_LEDGE_FROM_MARS_LIGHTHOUSE_MERCURY_TOWER),
	SCENE_EXIT(MERCURY_TOWER_PIPELINE_MENDED, MarsLighthouseMercuryTower,
		MERCURY_TOWER_PIPELINE_MENDED),
	SCENE_EXITS(MarsLighthouseJupiterTower),
	SCENE_EXIT(MARS_LIGHTHOUSE_JUPITER_TOWER_TO_MARS_LIGHTHOUSE_JUPITER_LEDGE,
		MarsLighthouseJupiterLedge,
		MARS_LIGHTHOUSE_JUPITER_LEDGE_FROM_MARS_LIGHTHOUSE_JUPITER_TOWER),
	SCENE_EXIT(CYCLONE_FROM_FIRST_HALL_WEST_CIRCLE, MarsLighthouseJupiterTower,
		JUPITER_TOWER_CORRIDOR_FOOT_CIRCLE),
	SCENE_EXIT(CYCLONE_FROM_CORRIDOR_FOOT_CIRCLE, MarsLighthouseJupiterTower,
		JUPITER_TOWER_FIRST_HALL_WEST_CIRCLE),
	SCENE_EXIT(CYCLONE_FROM_FIRST_HALL_EAST_CIRCLE, MarsLighthouseJupiterTower,
		JUPITER_TOWER_NARROW_ROOM_FIRST_CIRCLE),
	SCENE_EXIT(CYCLONE_FROM_NARROW_ROOM_FIRST_CIRCLE,
		MarsLighthouseJupiterTower, JUPITER_TOWER_FIRST_HALL_EAST_CIRCLE),
	SCENE_EXIT(CYCLONE_FROM_CORRIDOR_HEAD_EAST_CIRCLE,
		MarsLighthouseJupiterTower, JUPITER_TOWER_BIG_ROOM_SOUTHWEST),
	SCENE_EXIT(CYCLONE_FROM_CORRIDOR_HEAD_WEST_CIRCLE,
		MarsLighthouseJupiterTower, JUPITER_TOWER_BIG_ROOM_FAR_WEST),
	SCENE_EXIT(CYCLONE_FROM_BIG_ROOM_RAISED_CIRCLE, MarsLighthouseJupiterTower,
		JUPITER_TOWER_TWO_CIRCLE_ROOM_FIRST_CIRCLE),
	SCENE_EXIT(CYCLONE_FROM_TWO_CIRCLE_ROOM_FIRST_CIRCLE,
		MarsLighthouseJupiterTower, JUPITER_TOWER_BIG_ROOM_RAISED_CIRCLE),
	SCENE_EXIT(CYCLONE_FROM_TWO_CIRCLE_ROOM_SECOND_CIRCLE,
		MarsLighthouseJupiterTower, JUPITER_TOWER_BIG_ROOM_NORTHEAST_LEDGE),
	SCENE_EXIT(CYCLONE_FROM_BIG_ROOM_EAST_CIRCLE, MarsLighthouseJupiterTower,
		JUPITER_TOWER_NARROW_ROOM_SECOND_CIRCLE),
	SCENE_EXIT(CYCLONE_FROM_NARROW_ROOM_SECOND_CIRCLE,
		MarsLighthouseJupiterTower, JUPITER_TOWER_BIG_ROOM_EAST_CIRCLE),
	SCENE_EXIT(CYCLONE_FROM_BIG_ROOM_WEST_CIRCLE, MarsLighthouseJupiterTower,
		JUPITER_TOWER_CARVED_DRAGON_CIRCLE),
	SCENE_EXIT(CYCLONE_FROM_CARVED_DRAGON_CIRCLE, MarsLighthouseJupiterTower,
		JUPITER_TOWER_BIG_ROOM_WEST_CIRCLE),
	SCENE_EXIT(MARS_LIGHTHOUSE_JUPITER_TOWER_TO_MARS_LIGHTHOUSE_BEACON_LEDGE,
		MarsLighthouseBeaconLedge,
		MARS_LIGHTHOUSE_BEACON_LEDGE_FROM_MARS_LIGHTHOUSE_JUPITER_TOWER),
	SCENE_EXIT(JUPITER_TOWER_THROUGH_HOLE, MarsLighthouseJupiterTower,
		JUPITER_TOWER_THROUGH_HOLE),
	SCENE_EXITS(MarsLighthouseVenusTower),
	SCENE_EXIT(MARS_LIGHTHOUSE_VENUS_TOWER_TO_MARS_LIGHTHOUSE_VENUS_LEDGE,
		MarsLighthouseVenusLedge,
		MARS_LIGHTHOUSE_VENUS_LEDGE_FROM_MARS_LIGHTHOUSE_VENUS_TOWER),
	SCENE_EXIT(6, MarsLighthouseVenusTower, 7),
	SCENE_EXIT(7, MarsLighthouseVenusTower, VENUS_TOWER_HALL_WEST_DOORWAY),
	SCENE_EXIT(8, MarsLighthouseVenusTower, 9),
	SCENE_EXIT(9, MarsLighthouseVenusTower, VENUS_TOWER_HALL_EAST_DOORWAY),
	SCENE_EXIT(10, MarsLighthouseVenusTower, VENUS_TOWER_UPPER_DOORWAY),
	SCENE_EXIT(11, MarsLighthouseVenusTower, 10),
	SCENE_EXIT(
		12, MarsLighthouseVenusTower, VENUS_TOWER_STATUE_HALL_EAST_DOORWAY),
	SCENE_EXIT(13, MarsLighthouseVenusTower, 12),
	SCENE_EXIT(14, MarsLighthouseVenusTower, VENUS_TOWER_LOWER_DOORWAY),
	SCENE_EXIT(
		15, MarsLighthouseVenusTower, VENUS_TOWER_STATUE_HALL_SOUTH_PASSAGE),
	SCENE_EXIT(MARS_LIGHTHOUSE_VENUS_TOWER_TO_MARS_LIGHTHOUSE_BEACON_LEDGE,
		MarsLighthouseBeaconLedge,
		MARS_LIGHTHOUSE_BEACON_LEDGE_FROM_MARS_LIGHTHOUSE_VENUS_TOWER),
	SCENE_EXITS_END,
};

/* clang-format off */
LATE_DATA ACTOR_PLACE MarsLighthouseRoomF_Placements[] = {
	{SCENE_TABLE_END},
};

/* Each tower's actors, in its enum's order. */
LATE_DATA ACTOR_PLACE MarsLighthouseRoomF_VenusTowerPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 776, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 776, 0, 136, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SPIRAL_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 600, 0, 136, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SPIRAL_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 632, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SPIRAL_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 680, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_DRAGON_HEAD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 744, 0, 128, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_DRAGON_HEAD, CONDITION_ALWAYS, MarsLighthouseRoomF_VenusWestFlameThrowerRoute, 664, 0, 304, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_DRAGON_HEAD, CONDITION_ALWAYS, MarsLighthouseRoomF_VenusEastFlameThrowerRoute, 760, 0, 304, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 792, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 552, 0, 552, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 552, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 680, 0, 552, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 744, 0, 552, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 552, 0, 712, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 712, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 680, 0, 712, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_DRAGON_HEAD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_DRAGON_HEAD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_DRAGON_HEAD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

LATE_DATA ACTOR_PLACE MarsLighthouseRoomF_MarsTowerPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_DRAGON_HEAD, CONDITION_ALWAYS, MarsLighthouseRoomF_ButtonFlameThrowerRoute, 104, 0, 576, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_MARS_TORCH_LIT, CONDITION_ALWAYS, (Sint32)MarsLighthouseRoomF_TorchScript, 248, 0, 616, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_MARS_TORCH, CONDITION_ALWAYS, (Sint32)MarsLighthouseRoomF_TorchScript, 248, 0, 616, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_MARS_TORCH, CONDITION_ALWAYS, (Sint32)MarsLighthouseRoomF_TorchScript, 248, 0, 664, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SPARKLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FIREBALL, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 248, 0, 848, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PIPE_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 144, 0, 256, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 104, 0, 352, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PIPE_END, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 104, 0, 408, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 112, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_DRAGON_HEAD_SIDE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 368, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_DRAGON_HEAD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 304, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_DRAGON_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 312, FACING_WEST, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

LATE_DATA ACTOR_PLACE MarsLighthouseRoomF_JupiterTowerPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_DRAGON_HEAD, CONDITION_ALWAYS, MarsLighthouseRoomF_JupiterWestFlameThrowerRoute, 88, 0, 112, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_DRAGON_HEAD, CONDITION_ALWAYS, MarsLighthouseRoomF_JupiterEastFlameThrowerRoute, 360, 0, 112, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_DRAGON_HEAD, CONDITION_ALWAYS, MarsLighthouseRoomF_JupiterLowerFlameThrowerSetUp, 120, 0, 288, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_DRAGON_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 296, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_DRAGON_HEAD, CONDITION_ALWAYS, MarsLighthouseRoomF_JupiterFarFlameThrowerRoute, 968, 0, 80, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_MARS_TORCH, CONDITION_ALWAYS, (Sint32)MarsLighthouseRoomF_TorchScript, 872, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SPARKLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FIREBALL, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

LATE_DATA Uint32 MarsLighthouseRoomF_MercuryFlameThrowerRoute[] = {
	SC_SET(SW_STEERING, 0),
	SC_SET(SW_MOTION, ACTOR_FALLS),
	SC_SET(SW_Y, toFIXED(80)),
	SC_SET(SW_GROUND, toFIXED(80)),
	SC_SPEED(0.5),
	SC_ACCELERATION(0.03125),
	SC_MOVE(216, 0, 512),
	SC_SYNC,
	SC_WAIT(20),
	SC_MOVE(152, 0, 512),
	SC_SYNC,
	SC_WAIT(20),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

LATE_DATA ACTOR_PLACE MarsLighthouseRoomF_MercuryTowerPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 568, 0, 144, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PIPE_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 544, 0, 240, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PIPE_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 544, 0, 272, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 512, 0, 200, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 576, 0, 232, FACING_WEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 600, 0, 176, FACING_WEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_DRAGON_HEAD, CONDITION_ALWAYS, (Sint32)MarsLighthouseRoomF_MercuryFlameThrowerRoute, 152, 0, 512, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_MARS_TORCH_LIT, CONDITION_ALWAYS, (Sint32)MarsLighthouseRoomF_TorchScript, 264, 0, 520, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_MARS_TORCH, CONDITION_ALWAYS, (Sint32)MarsLighthouseRoomF_TorchScript, 264, 0, 520, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_MARS_TORCH, CONDITION_ALWAYS, (Sint32)MarsLighthouseRoomF_TorchScript, 184, 0, 552, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PIPE_END, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 600, 0, 204, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SPARKLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FIREBALL, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

LATE_DATA CUE MarsLighthouseRoomF_Cues[] = {
	{CUE_TABLE_END},
};

/* Each tower's cues. */
LATE_DATA CUE MarsLighthouseRoomF_VenusTowerCues[] = {
	{STEP_CUE_WALKING, 30, CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseRoomFJumpGapOrPushStone)},
	{STEP_CUE_WALKING_INTO(FACING_WEST), 31, CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseRoomFJumpGap)},
	{STEP_CUE_WALKING, 31, CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseRoomFPushStoneAlongLedge)},
	{CUE_EXIT, 1, CONDITION_ALWAYS, 1},
	{CUE_EXIT, 5, CONDITION_ALWAYS, 5},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 6, CONDITION_ALWAYS, 6},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 7, CONDITION_ALWAYS, 7},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 8, CONDITION_ALWAYS, 8},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 9, CONDITION_ALWAYS, 9},
	{CUE_EXIT, 10, CONDITION_ALWAYS, 10},
	{CUE_EXIT | EXIT_STYLE_HIDDEN_LAYER, 11, CONDITION_ALWAYS, 11},
	{CUE_EXIT, 12, CONDITION_ALWAYS, 12},
	{CUE_EXIT | EXIT_STYLE_HIDDEN_LAYER, 13, CONDITION_ALWAYS, 13},
	{CUE_EXIT, 14, CONDITION_ALWAYS, 14},
	{CUE_EXIT | EXIT_STYLE_HIDDEN_LAYER, 15, CONDITION_ALWAYS, 15},
	{CUE_PUSH, 0, CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseRoomFStartMovingStone)},
	{CUE_PUSHED, 0, CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseRoomFStopMovingStone)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FIRST_STONE, CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseRoomFStartMovingStone)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_STONE, CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseRoomFStopMovingStone)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SECOND_STONE, CONDITION_ALWAYS, MARS_LIGHTHOUSE_ROOM_F_FILL_GAPS_WITH_STONES},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_THIRD_STONE, CONDITION_ALWAYS, MARS_LIGHTHOUSE_ROOM_F_FILL_GAPS_WITH_STONES},
	{PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_FIRST_STONE, CONDITION_ALWAYS, MARS_LIGHTHOUSE_ROOM_F_FILL_GAPS_WITH_STONES},
	{PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_SECOND_STONE, CONDITION_ALWAYS, MARS_LIGHTHOUSE_ROOM_F_FILL_GAPS_WITH_STONES},
	{PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_THIRD_STONE, CONDITION_ALWAYS, MARS_LIGHTHOUSE_ROOM_F_FILL_GAPS_WITH_STONES},
	{STEP_CUE_WALKING_INTO(FACING_EAST), 33, FLAG_VENUS_STATUE_PUSHED_OFF_LEDGE, SCRIPT_REF(scMarsLighthouseRoomFPushVenusStatueOffLedge)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PUSHED_STATUE, FLAG_VENUS_STATUE_PUSHED_OFF_LEDGE, SCRIPT_REF(scMarsLighthouseRoomFMoveVenusStatueOffLedge)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_UPPER_DOORWAY_STATUE, CONDITION_ALWAYS, 0},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_TRAP_STATUE_A, CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseRoomFUncoverFlameThrower)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_TRAP_STATUE_B, CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseRoomFUncoverFlameThrower)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_TRAP_STATUE_C, CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseRoomFUncoverFlameThrower)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_TRAP_STATUE_A, CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseRoomFCoverFlameThrower)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_TRAP_STATUE_B, CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseRoomFCoverFlameThrower)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_TRAP_STATUE_C, CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseRoomFCoverFlameThrower)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_TRAP_STATUE_D, CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseRoomFUncoverFlameThrower)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_TRAP_STATUE_E, CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseRoomFUncoverFlameThrower)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_TRAP_STATUE_D, CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseRoomFCoverFlameThrower)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_TRAP_STATUE_E, CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseRoomFCoverFlameThrower)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LOWER_DOORWAY_STATUE, CONDITION_ALWAYS, 0},
	{CUE_STEP, STEP_CUE_FLAME_HIT, CONDITION_ALWAYS, SCRIPT_REF(fdLeaderKnockBack)},
	{CUE_TABLE_END},
};

LATE_DATA CUE MarsLighthouseRoomF_MarsTowerCues[] = {
	{CUE_EXIT, 1, CONDITION_ALWAYS, 1},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 3, CONDITION_ALWAYS, 3},
	{CUE_EXIT, 4, CONDITION_ALWAYS, 4},
	{CUE_STEP, 33, FLAG_BUTTON_PRESSED, SCRIPT_REF(scMarsLighthouseRoomFPressButton)},
	{STEP_CUE_WALKING, 30, CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseRoomFPushPipe)},
	{CUE_STEP, 30, CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseRoomFKnockBackIfHit)},
	{CUE_STEP, STEP_CUE_FLAME_HIT, CONDITION_ALWAYS, SCRIPT_REF(fdLeaderKnockBack)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PIPE_STATUE, CONDITION_ALWAYS, 0},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 20, FLAG_WEST_WALL_BURST, SCRIPT_REF(scMarsLighthouseRoomFBurstWestWall)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 21, FLAG_EAST_WALL_BURST, SCRIPT_REF(scMarsLighthouseRoomFBurstEastWall)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 22, FLAG_CENTER_WALL_BURST, SCRIPT_REF(scMarsLighthouseRoomFBurstCenterWall)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_MARS_TORCH, CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseRoomFStartBlazeOnTorch)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_HITS), ACTOR_WALL_TORCH, CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseRoomFBlazeTorch)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_AFTER_END), ACTOR_WALL_TORCH, CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseRoomFWatchFireballCrackWall)},
	{CUE_TABLE_END},
};

LATE_DATA CUE MarsLighthouseRoomF_JupiterTowerCues[] = {
	{CUE_STEP, 20, CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseRoomFFallIntoHole)},
	{CUE_EXIT, 1, CONDITION_ALWAYS, 1},
	{CUE_EXIT, 15, CONDITION_ALWAYS, 15},
	{CUE_EXIT, 16, CONDITION_ALWAYS, 16},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 2, CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseRoomFCycloneOnFirstHallWestCircle)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 3, CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseRoomFCycloneOnCorridorFootCircle)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 4, CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseRoomFCycloneOnFirstHallEastCircle)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 5, CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseRoomFCycloneOnNarrowRoomFirstCircle)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 6, CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseRoomFCycloneOnCorridorHeadEastCircle)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 7, CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseRoomFCycloneOnCorridorHeadWestCircle)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 8, CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseRoomFCycloneOnBigRoomRaisedCircle)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 9, CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseRoomFCycloneOnTwoCircleRoomFirstCircle)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 10, CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseRoomFCycloneOnTwoCircleRoomSecondCircle)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 11, CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseRoomFCycloneOnBigRoomEastCircle)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 12, CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseRoomFCycloneOnNarrowRoomSecondCircle)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 13, CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseRoomFCycloneOnBigRoomWestCircle)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 14, CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseRoomFCycloneOnCarvedDragonCircle)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_DROPPING_BLOCK, FLAG_BLOCK_DROPPED, SCRIPT_REF(scMarsLighthouseRoomFDropBlock)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_JUPITER_STATUE, FLAG_JUPITER_STATUE_BLOCKS_FLAMES, SCRIPT_REF(scMarsLighthouseRoomFMoveJupiterStatueIntoFlames)},
	{PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseRoomFRevealSteppingStones)},
	{PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseRoomFHideSteppingStones)},
	{CUE_STEP, STEP_CUE_FLAME_HIT, CONDITION_ALWAYS, SCRIPT_REF(fdLeaderKnockBack)},
	{CUE_STEP, STEP_CUE_FIREBALL_HIT, CONDITION_ALWAYS, SCRIPT_REF(scFireballCarryLeader)},
	{CUE_STEP, 14, CONDITION_ALWAYS, SCRIPT_REF(scFireballCarryLeaderIfHit)},
	{CUE_TABLE_END},
};

LATE_DATA CUE MarsLighthouseRoomF_MercuryTowerCues[] = {
	{CUE_EXIT, 1, CONDITION_ALWAYS, 1},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 3, CONDITION_ALWAYS, 3},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 4, CONDITION_ALWAYS, 4},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 5, CONDITION_ALWAYS, 5},
	{CUE_EXIT, 6, CONDITION_ALWAYS, 6},
	{CUE_EXIT | EXIT_STYLE_HIDDEN_LAYER, 7, CONDITION_ALWAYS, 7},
	{CUE_EXIT, 8, CONDITION_ALWAYS, 8},
	{CUE_STEP, 30, CONDITION_ALWAYS, SCRIPT_REF(fdLeaderSlideOnIce)},
	{STEP_CUE_WALKING, 33, CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseRoomFPushPipeOrJump)},
	{CUE_STEP, 33, CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseRoomFKnockBackIfHit)},
	{CUE_STEP, STEP_CUE_FLAME_HIT, CONDITION_ALWAYS, SCRIPT_REF(fdLeaderKnockBack)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_MERCURY_TORCH, CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseRoomFStartBlazeOnTorch)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_HITS), ACTOR_MERCURY_FLAME_THROWER, CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseRoomFLightFlameThrower)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_AFTER_END), ACTOR_MERCURY_FLAME_THROWER, CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseRoomFWatchFlameThrower)},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_UPPER_PUDDLE, FLAG_UPPER_PUDDLE_FROZEN, SCRIPT_REF(MarsLighthouseRoomF_FrostUpperPuddle)},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_CENTER_PUDDLE, FLAG_CENTER_PUDDLE_FROZEN, SCRIPT_REF(MarsLighthouseRoomF_FrostCenterPuddle)},
	{CUE_TABLE_END},
};

LATE_DATA Sint32 PaletteCycleFrame = -1;

LATE_DATA Sint32 PaletteCyclePausesInScripts = 1;

LATE_DATA Sint32 WhirlwindEndScript[] = {
	SC_ANIMATION(5),
	SC_WAIT(12),
	SC_RELEASE,
};
/* clang-format on */

LATE_DATA const Sint32 *FieldEffect_DurationScripts[] = {
	FieldEffect_ShortScript,
	FieldEffect_MediumScript,
	FieldEffect_LongScript,
};

/* The Anemos Inner Sanctum, the hidden dungeon under Contigo, reached by
   Teleport from the circle in the middle of the town.

   The party arrives in a room with four element symbols on its floor. Stepping
   onto each, Felix lights a plate for every Djinni of its element the party
   has; with all eighteen of an element, its torch over the north door lights,
   and with all four lit the door opens (scAnemosInnerSanctumLightSymbol).
   Beyond it, Charon's slate speaks as the party comes up to it
   (scAnemosInnerSanctumApproachCharonSlate). Below, mimic stones copy Felix's
   steps, mirrored, until they reach a button that opens a door
   (scAnemosInnerSanctumMimicLeader); seven round switches make a lift float,
   ready to carry the party up (scAnemosInnerSanctumPressRoundSwitch); pillars
   are moved onto their places with Move, and a switch holds a door open while
   something stands on it; cracked floors give way under the party, and Sand
   lets Felix pass along stretches of sand. The last room, with its six symbol
   blocks and the Dullahan, is anemos_inner_sanctum_entrance.c. */
#include "scene/script_layer.h"
#include "field/field_mode.h"
#include "field/script_inline.h"
#include "field/field_mode_script.h"
#include "macros.h"
#include "game/game.h"
#include "field/call.h"
#include "scene/scene.h"
#include "field/actor_script.h"
#include "scene/mars_lighthouse_entrance.h"
#include "graphics/palette_buffers.h"

struct ACTOR;
struct METATILE_TARGET;
struct METATILE_TARGET_ENTRY;

/* A spark sent spiralling in to a Djinni's plate, as an actor's motion fields
   hold it: the plate it closes on (originX, originZ), the radius, angle and
   spin of its spiral, and its height, which drops by `fall` each step. */
typedef struct SPARK {
	const Sint32 *script;
	Sint16		  scriptCursor;
	Uint16		  facing;
	Sint32		  x;
	Sint32		  y;
	Sint32		  z;
	Sint32		  fall;
	Sint32		  scaleX, scaleY;
	Uint16		  collisionRadius;
	Uint8		  layer;
	Uint8		  priFlg;
	Sint32		  velX, velY, velZ;
	Sint32		  step;
	Sint32		  height;
	Sint32		  tgtX, tgtY, tgtZ;
	Sint32		  originX;
	Sint32		  originZ;
	Sint32		  radius;
	void		 *spr;
	Uint8		  sprStorageKind;
	Uint8		  motionFlg;
	Uint8		  arrivalAxis, conditionResult, snapToTgt, collisionFlg;
	Uint8		  steeringFlg, paused, retainSprCache, repeatCnt;
	Sint16		  wait;
	Uint8		  refused, frozen;
	Uint8		  pal; /* the palette of its sparks */
	Uint8		  sceneByte;
	Sint16		  ang;
	Sint16		  spin;
} SPARK;

Uint16 PaletteCycleBright		  BSS_DATA;
Uint16 *PaletteCycleSlots		  BSS_DATA;
Uint16 PaletteCycleTimer		  BSS_DATA;
Uint16 PaletteCycleDelay BSS_DATA ALIGNED(4);
Uint16 *PaletteSlots			  BSS_DATA;
Uint16 *PaletteColors			  BSS_DATA;
Uint16 *PaletteCycleColors		  BSS_DATA;
Uint16 PaletteCycleLength		  BSS_DATA;
Uint16 PaletteCycleIndex BSS_DATA ALIGNED(4);

void scTeleportArriveWhirling(void);

void scTeleportStopShimmer(void);

void fdTeleportPlaySparks(void);

void scLeaderCenterInMetatile(void);

METATILE_TARGET CrackedTiles BSS_DATA;

void scAnemosInnerSanctumLightSymbol(void);
void scAnemosInnerSanctumStepOffSymbol(void);
void scAnemosInnerSanctumStartMovingBlock(void);
void scAnemosInnerSanctumFinishMovingBlock(void);
void scAnemosInnerSanctumPressRoundSwitch(void);
void scAnemosInnerSanctumRideLiftUp(void);
void scAnemosInnerSanctumSinkIntoSand(void);
void scAnemosInnerSanctumEndSand(void);
void scAnemosInnerSanctumPressSwitch(void);
void scAnemosInnerSanctumStepOffSwitch(void);
void scAnemosInnerSanctumFallThroughFirstCracks(void);
void scAnemosInnerSanctumFallThroughSecondCracks(void);

void scAnemosInnerSanctumStartMimicStone(void);
void scAnemosInnerSanctumDropMimicStone(void);
void scAnemosInnerSanctumOpenMimicStoneDoor(void);
void scAnemosInnerSanctumApproachCharonSlate(void);
void scAnemosInnerSanctumTeleportOut(void);

extern Uint32 AnemosInnerSanctumShortScript[];
extern Uint32 AnemosInnerSanctumMediumScript[];
extern Uint32 AnemosInnerSanctumLongScript[];
extern Uint32 FieldEffectLongScript[];
extern Sint32 AnemosInnerSanctumNoShadowScript[];
extern Sint16 AnemosInnerSanctumMimicStoneDoorFrames[];
extern Sint32 AnemosInnerSanctumOrbScript[];

/* The actors of the rooms Move and the lifts work in, numbering each room's
   placement table: stone pillars (SPRITE_STONE_PILLAR) Moved onto their places,
   and the lifts (SPRITE_ARROW_PANEL) the round switches ready. */
typedef enum ANEMOS_INNER_SANCTUM_ELEVATOR_ACTOR_ID {
	ACTOR_ELEVATOR_FIRST_PILLAR = ACTOR_FIRST_PLACED,
	ACTOR_ELEVATOR_LIFT,
	ACTOR_ELEVATOR_SECOND_PILLAR
} ANEMOS_INNER_SANCTUM_ELEVATOR_ACTOR_ID;

/* The round switches' room. */
typedef enum ANEMOS_INNER_SANCTUM_SWITCHES_ACTOR_ID {
	ACTOR_SWITCHES_FIRST_PILLAR = ACTOR_FIRST_PLACED,
	ACTOR_SWITCHES_BOULDER, /* SPRITE_ROUND_BOULDER */
	ACTOR_SWITCHES_SECOND_PILLAR,
	ACTOR_SWITCHES_PILLAR_A,
	ACTOR_SWITCHES_PILLAR_B,
	ACTOR_SWITCHES_THIRD_PILLAR,
	/* An unseen marker standing on the switch, 14 pixels down, as a platform:
	   taken away as the switch is pressed (scAnemosInnerSanctumPressSwitch),
	   put back as the leader steps off it
	   (scAnemosInnerSanctumStepOffSwitch). */
	ACTOR_SWITCHES_SWITCH_MARKER
} ANEMOS_INNER_SANCTUM_SWITCHES_ACTOR_ID;

/* The Orihalcon chest's room. */
typedef enum ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_ACTOR_ID {
	ACTOR_ORIHALCON_CHEST_LIFT = ACTOR_FIRST_PLACED,
	ACTOR_ORIHALCON_CHEST_FIRST_PILLAR,
	ACTOR_ORIHALCON_CHEST_SECOND_PILLAR
} ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_ACTOR_ID;

/* The guardian room's three mimic stones (SPRITE_MIMIC_STONE), one in each room
   off its north side: entered from the south by entrance 1, 3 or 5, that room's
   stone mirrors the leader's steps (scAnemosInnerSanctumMimicLeader) until it
   reaches its button or falls into a hole. */
#define ACTOR_MIMIC_STONE_A ACTOR_FIRST_PLACED
#define ACTOR_MIMIC_STONE_B (ACTOR_FIRST_PLACED + 1)
#define ACTOR_MIMIC_STONE_C (ACTOR_FIRST_PLACED + 2)

/* The guardian room's ways in from the south, one below each mimic stone. */
enum {
	ENTRANCE_BY_MIMIC_STONE_A = 1,
	ENTRANCE_BY_MIMIC_STONE_B = 3,
	ENTRANCE_BY_MIMIC_STONE_C = 5
};

/* Set as each mimic stone reaches its button and opens its room's north door
   (scAnemosInnerSanctumOpenMimicStoneDoor), so that the set-up leaves it
   there. */
#define FLAG_MIMIC_STONE_A_ON_BUTTON FLAG_AREA_TEMP_01
#define FLAG_MIMIC_STONE_B_ON_BUTTON FLAG_AREA_TEMP_02
#define FLAG_MIMIC_STONE_C_ON_BUTTON FLAG_AREA_TEMP_03

/* Set while the leader stands on one of the Dragon Skin room's element symbols,
   so that it lights its plates once a visit to it. */
#define FLAG_ON_SYMBOL			 FLAG_SCENE_TEMP_00
/* Set while the switches room's switch is held down by the leader. */
#define FLAG_SWITCH_PRESSED		 FLAG_SCENE_TEMP_00
/* Set while the guardian room's mimic stone follows the leader or rests on its
   button, so that stepping onto its floor starts it no more. */
#define FLAG_MIMIC_STONE_STARTED FLAG_SCENE_TEMP_00

/* The step cues of the Dragon Skin room: its four element symbols, whose plates
   light for the Djinn of the element the party has
   (scAnemosInnerSanctumLightSymbol), the floor between them, and the Teleport
   circle in its middle. */
typedef enum ANEMOS_INNER_SANCTUM_SYMBOL_CUE {
	CUE_TELEPORT_CIRCLE = 30,
	CUE_VENUS_SYMBOL = 50,
	CUE_MERCURY_SYMBOL,
	CUE_MARS_SYMBOL,
	CUE_JUPITER_SYMBOL,
	CUE_FLOOR_BETWEEN_SYMBOLS
} ANEMOS_INNER_SANCTUM_SYMBOL_CUE;

/* The first of the seven round switches' step cues
   (scAnemosInnerSanctumPressRoundSwitch). */
enum { CUE_FIRST_ROUND_SWITCH = 50, ROUND_SWITCHES = 7 };

/* Set as each round switch is pressed, lighting it; the macro takes the
   switch's place from the first. */
#define FLAG_ROUND_SWITCH_PRESSED(n) (FLAG_SCENE_TEMP_00 + (n))
/* Set once all seven round switches are pressed: the room's lift floats, ready
   to carry the party. */
#define FLAG_LIFT_READY				 FLAG_SCENE_TEMP_07

/* The ready lift's step cue, in the elevator and Orihalcon chest rooms. */
enum { CUE_LIFT = 11 };

/* The switches room's step cues: the switch, and the metatile east of it that a
   leader stepping off it crosses. */
enum { CUE_SWITCH = 50, CUE_EAST_OF_SWITCH };

/* The step cue before Charon's slate. */
enum { CUE_BEFORE_CHARON_SLATE = 30 };

/* The sanctum's lasting flags: each pillar Moved onto its place, each element's
   torch lit, the Dragon Skin room's north door opened by all four, and Charon's
   slate's words heard. */
#define FLAG_ANEMOS_ELEVATOR_FIRST_PILLAR_PLACED		0xa80
#define FLAG_ANEMOS_ELEVATOR_SECOND_PILLAR_PLACED		0xa81
#define FLAG_ANEMOS_SWITCHES_FIRST_PILLAR_PLACED		0xa82
#define FLAG_ANEMOS_SWITCHES_SECOND_PILLAR_PLACED		0xa83
#define FLAG_ANEMOS_SWITCHES_THIRD_PILLAR_PLACED		0xa84
#define FLAG_ANEMOS_SWITCHES_PILLAR_B_PLACED			0xa85
#define FLAG_ANEMOS_ORIHALCON_CHEST_FIRST_PILLAR_PLACED 0xa86
#define FLAG_ANEMOS_VENUS_TORCH_LIT						0xa87
#define FLAG_ANEMOS_MERCURY_TORCH_LIT					0xa88
#define FLAG_ANEMOS_MARS_TORCH_LIT						0xa89
#define FLAG_ANEMOS_JUPITER_TORCH_LIT					0xa8a
#define FLAG_ANEMOS_NORTH_DOOR_OPEN						0xa8b
#define FLAG_ANEMOS_CHARON_SLATE_SPOKE					0xa8c

/* The guardian room's lift (SPRITE_ARROW_PANEL), which brings the party up from
   the Orihalcon chest room (scAnemosInnerSanctumArriveOnGuardianLift). */
#define ACTOR_GUARDIAN_LIFT (ACTOR_FIRST_PLACED + 3)

/* The first column of the middle layer's metatiles in these rooms' maps (their
   map_layer 128, 0): the Dragon Skin room draws its plates, torches' light and
   pillar of light there. */
#define MIDDLE_LAYER_COLUMN (MAPGRID_SIZE / 2)
/* The first row of the overhead layer's metatiles in these rooms' maps (their
   map_layer 0, 128). */
#define OVERHEAD_LAYER_ROW	(MAPGRID_SIZE / 2)
/* How far below a plate or the pillar of light its lit picture is kept, in
   metatiles. */
#define LIT_PICTURE_ROWS	26

/* The four torches over the Dragon Skin room's north door, on the wall's row z
   152: each element's x and the height its flame burns at. */
#define VENUS_TORCH_X	520
#define VENUS_TORCH_Y	48
#define MERCURY_TORCH_X 552
#define MERCURY_TORCH_Y 48
#define MARS_TORCH_X	504
#define MARS_TORCH_Y	40
#define JUPITER_TORCH_X 568
#define JUPITER_TORCH_Y 40
#define TORCH_ROW_Z		152

/* A spark ring's sixteenths of a turn; an orb starts at every second one. */
#define SPARK_RING_STEPS 16

/* The ways between the rooms each of these maps holds, named by the room a way
   leaves and the room it leads to, each room by where it lies on its map; a
   way's exit and the entrance at its foot share its number, so the way back is
   the entrance the exit leads to.

   The elevator's: the north-west and south-west halls, the room above the lift,
   the round switches' room below it, and the east and south-east rooms. */
typedef enum ANEMOS_INNER_SANCTUM_ELEVATOR_WAY {
	WAY_ELEVATOR_NORTHWEST_TO_SOUTHWEST = 1,
	WAY_ELEVATOR_NORTHWEST_TO_EAST,
	WAY_ELEVATOR_NORTHWEST_TO_ABOVE_THE_LIFT,
	WAY_ELEVATOR_SOUTHWEST_TO_NORTHWEST = 5,
	WAY_ELEVATOR_SOUTHWEST_TO_EAST = 7,
	WAY_ELEVATOR_SOUTHWEST_TO_SOUTHEAST = 9,
	WAY_ELEVATOR_ROUND_SWITCHES_TO_EAST,
	/* Up the lift, to the room above. */
	EXIT_UP_THE_LIFT,
	WAY_ELEVATOR_EAST_TO_NORTHWEST,
	WAY_ELEVATOR_EAST_TO_SOUTHWEST,
	WAY_ELEVATOR_EAST_TO_ROUND_SWITCHES,
	WAY_ELEVATOR_SOUTHEAST_TO_SOUTHWEST,
	WAY_ELEVATOR_ABOVE_THE_LIFT_TO_NORTHWEST = 17,
	/* Where the lift arrives in the room above. */
	ENTRANCE_UP_THE_LIFT
} ANEMOS_INNER_SANCTUM_ELEVATOR_WAY;

/* The round switches' room's: its north, north-east, west, middle and south
   rooms, the north-east one with two ways down, west and east. */
typedef enum ANEMOS_INNER_SANCTUM_SWITCHES_WAY {
	WAY_SWITCHES_NORTH_TO_MIDDLE = 1,
	WAY_SWITCHES_NORTHEAST_TO_MIDDLE_WEST = 3,
	WAY_SWITCHES_NORTHEAST_TO_MIDDLE_EAST,
	WAY_SWITCHES_WEST_TO_MIDDLE,
	WAY_SWITCHES_MIDDLE_TO_WEST = 10,
	WAY_SWITCHES_MIDDLE_TO_SOUTH,
	WAY_SWITCHES_MIDDLE_TO_NORTH = 13,
	WAY_SWITCHES_MIDDLE_TO_NORTHEAST_WEST,
	WAY_SWITCHES_MIDDLE_TO_NORTHEAST_EAST,
	WAY_SWITCHES_SOUTH_TO_MIDDLE = 17
} ANEMOS_INNER_SANCTUM_SWITCHES_WAY;

/* The Orihalcon chest room's: its north-east and south-east rooms, and the
   second cracked floor's room west of them. */
typedef enum ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_WAY {
	WAY_ORIHALCON_CHEST_NORTHEAST_TO_SOUTHEAST = 3,
	WAY_ORIHALCON_CHEST_CRACKED_FLOOR_TO_SOUTHEAST,
	WAY_ORIHALCON_CHEST_SOUTHEAST_TO_CRACKED_FLOOR = 8,
	WAY_ORIHALCON_CHEST_SOUTHEAST_TO_NORTHEAST
} ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_WAY;

/* The guardian room's: its south and south-east rooms. */
typedef enum ANEMOS_INNER_SANCTUM_GUARDIAN_WAY {
	WAY_GUARDIAN_SOUTH_TO_SOUTHEAST = 8,
	WAY_GUARDIAN_SOUTHEAST_TO_SOUTH = 11
} ANEMOS_INNER_SANCTUM_GUARDIAN_WAY;

/* Set when the elevator room's lift has carried the party up; it stays
   above. */
#define FLAG_ELEVATOR_LIFT_RISEN FLAG_AREA_TEMP_00
/* Set when the Orihalcon chest room's lift has carried the party up to the
   guardian room; it stays there, and the round switches below stay pressed. */
#define FLAG_GUARDIAN_LIFT_RISEN FLAG_AREA_TEMP_04

/* The Orihalcon chest room's two cracked floors: a metatile's step cue is its
   floor's when whole, the next when cracked. */
enum {
	CUE_FIRST_CRACKED_FLOOR = 33,
	CUE_SECOND_CRACKED_FLOOR = CUE_FIRST_CRACKED_FLOOR + 2
};

/* The Orihalcon chest room's ways down through its cracked floors, each an exit
   and the entrance below of one number. */
enum { THROUGH_FIRST_CRACKED_FLOOR = 11, THROUGH_SECOND_CRACKED_FLOOR = 12 };

/* The row the mimic stones start on: one follows the leader's steps only while
   he is north of it, and never goes south of it. */
#define MIMIC_STONE_LINE_Z 248

/* The guardian room's step cues by the mimic stones: the floor where the
   leader's steps start one following, and the metatile behaviors a following
   stone stops on, a hole it falls into and the button that opens its door. */
typedef enum ANEMOS_INNER_SANCTUM_MIMIC_STONE_CUE {
	CUE_MIMIC_STONE_START = 50,
	CUE_MIMIC_STONE_HOLE,
	CUE_MIMIC_STONE_BUTTON,
	/* Raised when the stone falls into a hole
	   (scAnemosInnerSanctumDropMimicStone). */
	CUE_MIMIC_STONE_IN_HOLE = 200,
	/* Raised when the stone reaches the button
	   (scAnemosInnerSanctumOpenMimicStoneDoor). */
	CUE_MIMIC_STONE_ON_BUTTON
} ANEMOS_INNER_SANCTUM_MIMIC_STONE_CUE;

#include "shared/teleport_shimmer.c"

#include "../field/actor_set_sprite_priority.c"

#include "../field/actor_create_silhouette.c"

#include "../field/field_effect_move.c"

extern Sint32 AnemosInnerSanctumDurationScripts[];

/* Spawns a field effect at x, y, z moving at the velocity given, its type,
   scale, palette, priority, script, rotation, spin and update chosen by the
   EFFECT_USE_ flags from the options.

   This overlay's own copy of ../field/field_effect_spawn.c, which it does not
   include: this one takes actor 0's sprite priority rather than the leader's,
   picks from the overlay's own duration scripts, and sets a requested priority
   on the sprite while creating the actor through the import. */
void scFieldEffectSpawn(Sint32 x, Sint32 y, Sint32 z, Sint32 velX, Sint32 velY,
	Sint32 velZ, Uint32 flg, const EFFECT_OPTIONS *options)
{
	FIELD_EFFECT *party = ((FIELD_EFFECT *)fdActorGetById(0));
	FIELD_EFFECT *effect;
	FIELD_SPR	 *spr;
	const Sint32 *script;
	Uint32		  start;

	if((flg & EFFECT_USE_TYPE) != 0 && options != 0)
		effect = (FIELD_EFFECT *)syImportActorCreate(options->type, x, y, z);
	else
		effect =
			(FIELD_EFFECT *)syImportActorCreate(SPRITE_DUST_CLOUD, x, y, z);
	if(effect == 0)
		return;

	spr = effect->spr;
	fdActorSelectAnimation(effect, (flg + 1) & EFFECT_SCRIPT_MASK);
	fdActorStartScript(
		effect, AnemosInnerSanctumDurationScripts[flg & EFFECT_SCRIPT_MASK]);
	effect->motionFlg = 0;
	fdActorSetSprFlags(effect, 0);
	effect->update = fdEffectUpdateMotionScaleAndSpin;
	effect->vel.x = velX;
	effect->vel.y = velY;
	effect->vel.z = velZ;
	fdSetActorSprPriority(effect, party->spr->pri);
	effect->scaleRateX = 0;
	effect->scaleRateY = 0;
	effect->spin = 0;

	if((flg & EFFECT_OPTION_MASK) == 0 || options == 0)
		return;
	if((flg & EFFECT_USE_PALETTE) != 0)
		scFieldActorSetSprCol(effect, options->pal);
	if((flg & EFFECT_USE_PRIORITY) != 0) {
		effect->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;
		spr->pri = options->pri;
		fdSetActorSprPriority(effect, options->pri);
	}
	start = flg & EFFECT_USE_START_SCALE;
	if(start != 0) {
		effect->scaleX = options->startScaleX;
		effect->scaleY = options->startScaleY;
	}
	if((flg & EFFECT_SCALE_TO_TARGET) != 0) {
		FIXED  tgtX;
		Sint32 cnt;

		script = AnemosInnerSanctumDurationScripts[flg & EFFECT_SCRIPT_MASK];
		tgtX = options->tgtScaleX;
		cnt = script[EFFECT_SCRIPT_REPEAT_COUNT];
		if(start != 0) {
			effect->scaleRateX = (tgtX - effect->scaleX) / cnt;
			effect->scaleRateY = (options->tgtScaleY - effect->scaleY) /
				script[EFFECT_SCRIPT_REPEAT_COUNT];
		} else {
			effect->scaleRateX = (tgtX - toFIXED(1)) / cnt;
			effect->scaleRateY = (options->tgtScaleY - toFIXED(1)) /
				script[EFFECT_SCRIPT_REPEAT_COUNT];
		}
	}
	if((flg & EFFECT_USE_SCRIPT) != 0) {
		fdActorSelectAnimation(effect, 1);
		fdActorStartScript(effect, options->script);
	}
	if((flg & EFFECT_USE_ROTATION) != 0)
		spr->rotation = options->rotation;
	if((flg & EFFECT_USE_SPIN) != 0)
		effect->spin = options->spin;
	if((flg & EFFECT_USE_UPDATE) != 0)
		effect->update = options->update;
}

/* Clears the actor's sprite flags, as a script's call; always 0. */
Sint32 scAnemosInnerSanctumClearScriptedActorSprFlags(Sint32 actorId)
{
	fdActorSetSprFlags(actorId, 0);
	return 0;
}

/* A spark ring's orb: it circles its centre at its own speed while the radius
   shrinks a pixel a tick; 0 once it reaches the centre. */
Bool scAnemosInnerSanctumCircleOrb(ACTOR *actor)
{
	FIXED radius;

	actor->tmr += actor->userValue;
	SET_ACTOR_POSITION(
		actor, actor->bounce, actor->gravity, toFIXED(TORCH_ROW_Z));
	syFieldVectorAddPolarOffsetFX(
		actor->sceneWord, actor->tmr, &actor->x.fixed);
	radius = actor->sceneWord - toFIXED(1);
	actor->sceneWord = radius;
	if(radius)
		return TRUE;
	return FALSE;
}

extern Uint32 AnemosInnerSanctumFadingSparkScript[];

/* Leaves a fading spark of the source's palette where the source is. */
void scAnemosInnerSanctumLeaveSpark(ACTOR *src)
{
	ACTOR *actor = (scFieldActorCreate(
		SPRITE_PARTICLE, src->x.fixed, src->y.fixed, src->z.fixed));

	actor->motionFlg = 0;
	scFieldActorSetSprCol(actor, src->sceneCnt);
	fdSetActorSprPriority(actor, 2);
	fdActorSelectAnimation(actor, 1);
	fdActorSetSprFlags(actor, 0);
	fdActorStartScript(actor, AnemosInnerSanctumFadingSparkScript);
	fdActorSetBlendMode(actor, 1);
}

/* A spark's update: two steps a tick along its spiral, closing in and falling,
   leaving a fading spark at each; 0 once it reaches its plate. */
Bool scAnemosInnerSanctumSpiralIn(SPARK *spark)
{
	if(spark->radius < spark->step)
		return FALSE;
	spark->ang += spark->spin >> 1;
	spark->x = spark->originX;
	spark->y = spark->height;
	spark->z = spark->originZ;
	syFieldVectorAddPolarOffsetFX(spark->radius, spark->ang, &spark->x);
	scAnemosInnerSanctumLeaveSpark(spark);
	spark->radius -= spark->step >> 1;
	spark->ang += spark->spin >> 1;
	spark->x = spark->originX;
	spark->y = spark->height;
	spark->z = spark->originZ;
	syFieldVectorAddPolarOffsetFX(spark->radius, spark->ang, &spark->x);
	scAnemosInnerSanctumLeaveSpark(spark);
	spark->radius -= spark->step >> 1;
	spark->height -= spark->fall;
	return TRUE;
}

/* A spark reaches its plate: a puff of sparks of its palette, and the plate
   shows its lit picture. */
Sint32 scAnemosInnerSanctumLightPlate(ACTOR *actor)
{
	EFFECT_OPTIONS options;
	Sint32		   x;
	Sint32		   y;

	options.pri = 2;
	options.pal = actor->sceneCnt;
	scFieldEffectSpawn(actor->x.fixed, actor->y.fixed, actor->z.fixed, 0,
		toFIXED(1), 0, EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE, &options);
	x = FIELD_METATILE(actor->bounce) + MIDDLE_LAYER_COLUMN;
	y = FIELD_METATILE(actor->gravity);
	fdMapGridCopyMetatilesTo(x, y + LIT_PICTURE_ROWS, x, y, 1, 1);
	return 0;
}

#include "../field/actor_wait_until_landed.c"

const SCENE_ENTRANCE *scGetEntrances(void)
{
	return SceneEntrances;
}

const SCENE_REGION *scGetExitRegions(void)
{
	return 0;
}

const Uint32 *scGetExits(void)
{
	return SceneExits;
}

extern ACTOR_PLACE AnemosInnerSanctumPlacements[];
extern ACTOR_PLACE AnemosInnerSanctumDragonSkinChestPlacements[];
extern ACTOR_PLACE AnemosInnerSanctumElevatorPlacements[];
extern ACTOR_PLACE AnemosInnerSanctumSwitchesPlacements[];
extern ACTOR_PLACE AnemosInnerSanctumOrihalconChestPlacements[];
extern ACTOR_PLACE AnemosInnerSanctumGuardianPlacements[];

const ACTOR_PLACE *scGetActorPlacements(void)
{
	if(MainRoutineStateWork.next ==
		SCENE_ANEMOS_INNER_SANCTUM_DRAGON_SKIN_CHEST)
		return AnemosInnerSanctumDragonSkinChestPlacements;
	if(MainRoutineStateWork.next == SCENE_ANEMOS_INNER_SANCTUM_ELEVATOR)
		return AnemosInnerSanctumElevatorPlacements;
	if(MainRoutineStateWork.next == SCENE_ANEMOS_INNER_SANCTUM_SWITCHES)
		return AnemosInnerSanctumSwitchesPlacements;
	if(MainRoutineStateWork.next == SCENE_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST)
		return AnemosInnerSanctumOrihalconChestPlacements;
	if(MainRoutineStateWork.next == SCENE_ANEMOS_INNER_SANCTUM_GUARDIAN)
		return AnemosInnerSanctumGuardianPlacements;
	return AnemosInnerSanctumPlacements;
}

extern Sint32 AnemosInnerSanctumSparkScript[];

/* Sends a spark of the given palette spiralling from one point to another. */
void	 scAnemosInnerSanctumSendSpark(from, to, palette) VECTOR3 *from;
VECTOR3 *to;
Sint16	 palette;
{
	SPARK *spark;
	Sint32 x, z, distance;

	spark = ((SPARK *)scFieldActorCreate(
		SPRITE_PARTICLE, from->x, from->y, from->z));
	x = FIXEDtoINT(from->x - to->x);
	z = FIXEDtoINT(from->z - to->z);
	spark->ang = syAtan(from->z - to->z, from->x - to->x);
	spark->spin = (((syRandom() & 2) - 1) * (syRandom() & 15)) << 8;
	spark->pal = palette;
	spark->motionFlg = 0;
	spark->originX = to->x;
	spark->originZ = to->z;
	distance = toFIXED(sySquartIWRAM(x * x + z * z));
	spark->radius = distance;
	spark->step = distance / 16;
	spark->height = from->y;
	spark->fall = (from->y - to->y) / 16;
	fdActorSetBlendMode(spark, 1);
	scFieldActorSetSprCol(spark, (Uint16)palette);
	fdSetActorSprPriority(spark, 2);
	fdActorSelectAnimation(spark, 7);
	fdActorSetSprFlags(spark, 0);
	syFieldVectorAddPolarOffsetFX(spark->radius, spark->ang, &spark->x);
	fdActorStartScript(spark, AnemosInnerSanctumSparkScript);
	scAnemosInnerSanctumSpiralIn(spark);
}

/* Rings a torch at x, y (16.16) with eight orbs of the given palette that
   circle in on it at the given speed. Each orb keeps its circle in its actor's
   fields: its centre in bounce (x) and gravity (y), its radius in sceneWord,
   its angle in timer and its speed in userValue. */
void   scAnemosInnerSanctumSparkRing(x, y, palette, speed) Sint32 x;
Sint32 y;
Sint16 palette;
Sint16 speed;
{
	Sint32 i;

	for(i = 0; i != SPARK_RING_STEPS;) {
		ACTOR *orb =
			(scFieldActorCreate(SPRITE_PARTICLE, x, y, toFIXED(TORCH_ROW_Z)));
		Sint32 ang = i * (TRIG_TURN / SPARK_RING_STEPS);

		syFieldVectorAddPolarOffset(32, ang, &orb->x.fixed);
		orb->tmr = ang;
		orb->userValue = speed;
		orb->sceneCnt = i;
		orb->sceneByte = palette;
		orb->bounce = x;
		orb->sceneWord = toFIXED(32);
		orb->gravity = y;
		orb->motionFlg = 0;
		fdActorSetBlendMode(orb, 1);
		scFieldActorSetSprCol(orb, (Uint16)palette);
		fdSetActorSprPriority(orb, 2);
		fdActorSelectAnimation(orb, 7);
		fdActorSetSprFlags(orb, 0);
		fdActorStartScript(orb, AnemosInnerSanctumOrbScript);
		i += 2;
	}
}

/* Leaves a flame of the given palette burning on the torch at x and height y on
   the torches' row. */
void   scAnemosInnerSanctumLightTorch(x, y, palette) Sint32 x;
Sint32 y;
Sint16 palette;
{
	ACTOR *flame;

	flame = scFieldActorCreate(
		SPRITE_PARTICLE, toFIXED(x), INTtoFIXED(y + 3), toFIXED(TORCH_ROW_Z));
	flame->motionFlg = ACTOR_FLOATS;
	scFieldActorSetSprCol(flame, (Uint16)palette);
	fdSetActorSprPriority(flame, 2);
	fdActorSelectAnimation(flame, 7);
	fdActorSetSprFlags(flame, 0);
}

extern Uint32 AnemosInnerSanctumDjinniRisesScript[];
extern Sint32 AnemosInnerSanctumDjinniSinksScript[];
extern Sint32 AnemosInnerSanctumPlateOffsets[][2];

/* Stepping onto an element's symbol in the Dragon Skin room: the leader raises
   his arms in a pillar of light and a Djinni of the element rises over him, and
   each of the element's Djinn the party has found sends a spark to its own
   plate. Short of all eighteen, the plates and the light go out again; with all
   eighteen, the element's torch over the north door is lit, and with all four
   lit the door opens. Nothing if the torch is lit already or the party has none
   of the element's Djinn. */
void scAnemosInnerSanctumLightSymbol(void)
{
	FIELD_STATE *state = RamHeapSlots->fieldState;
	ACTOR		*leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	Uint16		 alpha;
	Sint32		 x, z;
	Sint32		 tgtX;
	Sint32		 tgtY;
	Sint16		 type;
	Sint16		 cue;
	Sint32		 idx;
	Sint32		 base;
	Uint8		 cnt;
	Sint32		 palette;
	Sint32		 column, row, metatileX, metatileZ;
	Sint32		 i;
	Uint8		 last;
	ACTOR		*djinni;

	syIORead16(&REG_BLDALPHA, &alpha);
	tgtX = 0;
	tgtY = 0;
	type = state->stepCue - CUE_VENUS_SYMBOL + SPRITE_VENUS_DJINNI;
	cue = state->stepCue;
	idx = cue - CUE_VENUS_SYMBOL;
	x = ((idx & 1) << 8) + 408;
	base = 0;
	cnt = 0;
	palette = SPRITE_COLOR_NORMAL;
	z = (idx & 2) * 3 * 32 + 168;
	switch(cue) {
	case CUE_VENUS_SYMBOL:
		base = FLAG_DJINNI(DJINNI_FLINT);
		tgtX = VENUS_TORCH_X;
		tgtY = VENUS_TORCH_Y;
		if(syGameFlagGet(FLAG_ANEMOS_VENUS_TORCH_LIT)) {
			syGameFlagSet(FLAG_ON_SYMBOL);
			return;
		}
		break;
	case CUE_MERCURY_SYMBOL:
		palette = SPRITE_COLOR_BGR;
		base = FLAG_DJINNI(DJINNI_FIZZ);
		tgtX = MERCURY_TORCH_X;
		tgtY = MERCURY_TORCH_Y;
		if(syGameFlagGet(FLAG_ANEMOS_MERCURY_TORCH_LIT)) {
			syGameFlagSet(FLAG_ON_SYMBOL);
			return;
		}
		break;
	case CUE_MARS_SYMBOL:
		palette = SPRITE_COLOR_RED_TINT;
		base = FLAG_DJINNI(DJINNI_FORGE);
		tgtX = MARS_TORCH_X;
		tgtY = MARS_TORCH_Y;
		if(syGameFlagGet(FLAG_ANEMOS_MARS_TORCH_LIT)) {
			syGameFlagSet(FLAG_ON_SYMBOL);
			return;
		}
		break;
	case CUE_JUPITER_SYMBOL:
		palette = SPRITE_COLOR_RBG;
		base = FLAG_DJINNI(DJINNI_GUST);
		tgtX = JUPITER_TORCH_X;
		tgtY = JUPITER_TORCH_Y;
		if(syGameFlagGet(FLAG_ANEMOS_JUPITER_TORCH_LIT)) {
			syGameFlagSet(FLAG_ON_SYMBOL);
			return;
		}
		break;
	}

	{
		Uint8	found[20];
		Uint8	order[20];
		VECTOR3 from;
		VECTOR3 to;

		for(i = (Uint16)base; i != (Uint16)base + DJINNI_SPARE_FIRST; i++) {
			if(syGameFlagGet(i)) {
				found[cnt] = i - base;
				cnt++;
			}
		}
		if(cnt == 0)
			return;

		fdScriptStart();
		syFieldScriptEndLastingPsynergy(0);
		fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.8);
		fdCameraSetSpeed(0.8);
		fdActorWalkTo(MainRoutineStateWork.leaderActorId, x, z);
		fdCameraMoveToFX(toFIXED(x), -1, toFIXED(z), 1);
		scFieldActorWaitForMoveById(MainRoutineStateWork.leaderActorId);
		fdActorFaceDirectionAndWait(
			MainRoutineStateWork.leaderActorId, FACING_SOUTH);
		fdActorSetAnimation(
			MainRoutineStateWork.leaderActorId, ANIM_LEADER_ARMS_RAISED);
		djinni = (scFieldActorCreate((Uint16)type, leader->x.fixed,
			leader->y.fixed + toFIXED(8), leader->z.fixed));
		djinni->motionFlg = 0;
		djinni->scaleY = toFIXED(1) / 10;
		djinni->scaleX = toFIXED(1) / 10;
		fdActorSetSprFlags((Sint32)djinni, 0);
		fdActorStartScript(djinni, AnemosInnerSanctumDjinniRisesScript);
		fdActorSetBlendMode(djinni, 1);
		fdPaletteFadeApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
		fdPaletteFadeApplyTarget(PALETTE_TINT_WARM, PALETTES_BG);
		fdPaletteFadeInterpolate(60);
		fdBlendScriptSuspend();
		alpha = alpha & BLEND_ALPHA_TOP_MASK;
		REG_BLDALPHA = BLEND_ALPHA(0, BLEND_WEIGHT_MAX);
		metatileX = x >> 4;
		metatileZ = z >> 4;
		column = metatileX;
		row = metatileZ;
		fdMapGridCopyMetatilesTo(MIDDLE_LAYER_COLUMN + metatileX - 1,
			metatileZ - 3 + LIT_PICTURE_ROWS,
			MIDDLE_LAYER_COLUMN + metatileX - 1, metatileZ - 3, 3, 5);
		for(i = 0; i != alpha; i++) {
			REG_BLDALPHA = BLEND_ALPHA(i, BLEND_WEIGHT_MAX);
			syWait(8);
		}
		fdBlendScriptResume();
		fdScriptPlaySnd(SE_GLITTER);
		syFieldScriptWait(48);

		last = cnt - 1;
		for(i = 0; i != cnt; i++) {
			Sint32 pick = syRandom();
			pick &= last;

			order[i] = found[pick];
			found[pick] = found[last];
			last--;
		}
		for(i = 0; i != cnt;) {
			Sint32 slot;

			from.x = leader->x.fixed + toFIXED(syRandom() & 31) - toFIXED(16);
			from.y = leader->y.fixed + toFIXED(syRandom() & 15) + toFIXED(8);
			from.z = leader->z.fixed + toFIXED(syRandom() & 15) - toFIXED(8);
			slot = order[i];
			to.x = leader->x.fixed +
				toFIXED(AnemosInnerSanctumPlateOffsets[slot][0]);
			to.y = leader->y.fixed;
			to.z = leader->z.fixed +
				toFIXED(AnemosInnerSanctumPlateOffsets[slot][1]);
			scAnemosInnerSanctumSendSpark(&from, &to, palette);
			if(i & 1)
				fdScriptPlaySnd(SE_SPARKS);
			i++;
			syFieldScriptWait(8);
		}

		if(cnt != DJINNI_SPARE_FIRST) {
			syFieldScriptWait(96);
			fdActorStartScript(djinni, AnemosInnerSanctumDjinniSinksScript);
			fdBlendScriptSuspend();
			alpha = REG_BLDALPHA & BLEND_ALPHA_TOP_MASK;
			fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
			fdPaletteFadeInterpolate(60);
			for(i = alpha; i != 0;) {
				REG_BLDALPHA = BLEND_ALPHA(i, BLEND_WEIGHT_MAX);
				i--;
				syWait(8);
			}
			for(i = 0; i != cnt;) {
				Sint32 slot = order[i];

				i++;
				fdMapGridCopyMetatilesTo(MIDDLE_LAYER_COLUMN, 0,
					FIELD_METATILE(leader->x.fixed +
						toFIXED(AnemosInnerSanctumPlateOffsets[slot][0])) +
						MIDDLE_LAYER_COLUMN,
					FIELD_METATILE(leader->z.fixed +
						toFIXED(AnemosInnerSanctumPlateOffsets[slot][1])),
					1, 1);
			}
			fdMapGridCopyMetatilesTo(MIDDLE_LAYER_COLUMN, 0,
				MIDDLE_LAYER_COLUMN + column - 1, row - 3, 3, 5);
			fdBlendScriptResume();
			fdScriptWaitTransitionTicks();
		} else {
			fdScriptSetFlag((Sint16)state->stepCue - CUE_VENUS_SYMBOL +
				FLAG_ANEMOS_VENUS_TORCH_LIT);
			fdActorStartScript(djinni, AnemosInnerSanctumDjinniSinksScript);
			syFieldScriptWait(60);
			fdCameraSetSpeed(4);
			fdCameraMoveTo(536, CAMERA_KEEP, 120, 1);
			fdCameraWaitForArrival();
			fdScriptPlaySnd(SE_SHIMMER);
			{
				FIXED ringX = toFIXED(tgtX);
				FIXED ringY = toFIXED(tgtY);

				scAnemosInnerSanctumSparkRing(
					ringX, ringY, palette, TRIG_TURN * 8 / 256);
				syFieldScriptWait(10);
				scAnemosInnerSanctumSparkRing(
					ringX, ringY, palette, TRIG_TURN * 9 / 256);
				syFieldScriptWait(10);
				scAnemosInnerSanctumSparkRing(
					ringX, ringY, palette, TRIG_TURN * 10 / 256);
			}
			syFieldScriptWait(12);
			syWait(16);
			fdPaletteFadeApplyTarget(RGB_WHITE, PALETTES_ALL);
			fdPaletteFadeInterpolate(1);
			syWait(1);
			fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
			fdPaletteFadeInterpolate(32);
			fdScriptPlaySnd(SE_FLASH);
			scAnemosInnerSanctumLightTorch(tgtX, tgtY, palette);
			syFieldScriptWait(80);
			if(syGameFlagGet(FLAG_ANEMOS_VENUS_TORCH_LIT) &&
				syGameFlagGet(FLAG_ANEMOS_MERCURY_TORCH_LIT) &&
				syGameFlagGet(FLAG_ANEMOS_MARS_TORCH_LIT) &&
				syGameFlagGet(FLAG_ANEMOS_JUPITER_TORCH_LIT)) {
				/* The north door opens. */
				fdScriptSetFlag(FLAG_ANEMOS_NORTH_DOOR_OPEN);
				fdScriptPlaySnd(SE_THUMP);
				fdMapGridCopyMetatilesTo(52, 7, 33, 7, 1, 2);
				syWait(5);
				fdMapGridCopyMetatilesTo(54, 7, 33, 7, 1, 2);
				fdMapGridSetMetatileHeight(toFIXED(536), toFIXED(136), 0, 0);
				syWait(60);
			}
		}

		fdActorRelease(djinni);
		fdScriptSetFlag(FLAG_ON_SYMBOL);
		syScriptCameraFollowActor(MainRoutineStateWork.leaderActorId, 1);
		fdCameraWaitForArrival();
		syFieldScriptWait(60);
		fdScriptEnd();
	}
}

/* Nothing visible happens as Felix steps off a symbol onto the floor between
   them; the game lets the symbols be stepped onto again. */
void scAnemosInnerSanctumStepOffSymbol(void)
{
	syGameFlagClear(FLAG_ON_SYMBOL);
}

extern Sint16 AnemosInnerSanctumElevatorBlocks[];
extern Sint16 AnemosInnerSanctumSwitchesBlocks[];
extern Sint16 AnemosInnerSanctumOrihalconChestBlocks[];

/* Nothing visible of its own happens as Felix starts to move a pillar with
   Move; in the switches' and the Orihalcon chest's rooms, the game closes the
   stretch of sand by the pillar's place while it is moving, so that a mound of
   sand cannot pass under it, and opens a metatile or two beside it. */
void scAnemosInnerSanctumStartMovingBlock(void)
{
	if(MainRoutineStateWork.next == SCENE_ANEMOS_INNER_SANCTUM_ELEVATOR) {
		fdActorLiftTargetBlock((Sint32)AnemosInnerSanctumElevatorBlocks);
	} else {
		if(MainRoutineStateWork.next == SCENE_ANEMOS_INNER_SANCTUM_SWITCHES) {
			fdActorLiftTargetBlock((Sint32)AnemosInnerSanctumSwitchesBlocks);
			fdSetMetatileBehaviorAt(MAPGRID_LAYER_OVERHEAD, toFIXED(72),
				toFIXED(440), METATILE_BEHAVIOR_WALL);
		} else {
			if(MainRoutineStateWork.next ==
				SCENE_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST) {
				fdActorLiftTargetBlock(
					(Sint32)AnemosInnerSanctumOrihalconChestBlocks);
				fdSetMetatileBehaviorAt(MAPGRID_LAYER_OVERHEAD, toFIXED(280),
					toFIXED(344), METATILE_BEHAVIOR_WALL);
				fdSetMetatileBehaviorAt(MAPGRID_LAYER_OVERHEAD, toFIXED(280),
					toFIXED(376), METATILE_BEHAVIOR_WALL);
				fdSetMetatileBehaviorAt(MAPGRID_LAYER_OVERHEAD, toFIXED(296),
					toFIXED(360), METATILE_BEHAVIOR_WALL);
				fdSetMetatileBehaviorAt(MAPGRID_LAYER_OVERHEAD, toFIXED(280),
					toFIXED(360), METATILE_BEHAVIOR_SAND);
				fdSetMetatileBehaviorAt(MAPGRID_LAYER_OVERHEAD, toFIXED(520),
					toFIXED(632), METATILE_BEHAVIOR_WALL);
				fdSetMetatileBehaviorAt(MAPGRID_LAYER_OVERHEAD, toFIXED(520),
					toFIXED(616), METATILE_BEHAVIOR_SAND);
			}
		}
	}
}

/* Once Felix has moved a pillar with Move: a pillar left in the place it
   belongs stays there on later visits, as a shortcut. In the switches' room,
   the pillar moved onto the switch presses it with a click, and the door west
   of it opens with a thump, for good.

   The stretches of sand scAnemosInnerSanctumStartMovingBlock closed are opened
   again. Each place is checked in half metatiles. In the switches' room
   whichever pillar is set down on pillar B's place counts as pillar B, the game
   trading the two actors' places while the third pillar is not on its own. */
void scAnemosInnerSanctumFinishMovingBlock(void)
{
	ACTOR *block = FieldStateWork->movedBlock;
	Sint32 x;
	Sint32 z;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdActorPlaceMovedBlock();
	x = FIELD_HALF_METATILE(block->x.fixed);
	z = FIELD_HALF_METATILE(block->z.fixed);
	if(MainRoutineStateWork.next == SCENE_ANEMOS_INNER_SANCTUM_SWITCHES) {
		fdSetMetatileBehaviorAt(MAPGRID_LAYER_OVERHEAD, toFIXED(72),
			toFIXED(440), METATILE_BEHAVIOR_SAND);
		if(x != 8)
			fdSetMetatileBehaviorAt(MAPGRID_LAYER_OVERHEAD, toFIXED(72),
				toFIXED(424), METATILE_BEHAVIOR_SAND);
	}
	if(MainRoutineStateWork.next ==
		SCENE_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST) {
		fdSetMetatileBehaviorAt(MAPGRID_LAYER_OVERHEAD, toFIXED(280),
			toFIXED(344), METATILE_BEHAVIOR_NORMAL);
		fdSetMetatileBehaviorAt(MAPGRID_LAYER_OVERHEAD, toFIXED(280),
			toFIXED(376), METATILE_BEHAVIOR_SAND);
		fdSetMetatileBehaviorAt(MAPGRID_LAYER_OVERHEAD, toFIXED(296),
			toFIXED(360), METATILE_BEHAVIOR_NORMAL);
		fdSetMetatileBehaviorAt(MAPGRID_LAYER_OVERHEAD, toFIXED(520),
			toFIXED(632), METATILE_BEHAVIOR_SAND);
	}
	if(MainRoutineStateWork.next == SCENE_ANEMOS_INNER_SANCTUM_ELEVATOR) {
		if(x == 37 && z == 31) {
			syGameFlagSet(FLAG_ANEMOS_ELEVATOR_FIRST_PILLAR_PLACED);
			fdActorSetSprPriority(ACTOR_ELEVATOR_FIRST_PILLAR, 2);
		}
		if(x == 55 && z == 95)
			syGameFlagSet(FLAG_ANEMOS_ELEVATOR_SECOND_PILLAR_PLACED);
	} else if(MainRoutineStateWork.next ==
		SCENE_ANEMOS_INNER_SANCTUM_SWITCHES) {
		if(x == 7 && z == 53)
			syGameFlagSet(FLAG_ANEMOS_SWITCHES_FIRST_PILLAR_PLACED);
		if(x == 67 && z == 45) {
			fdMapGridCopyMetatilesTo(50, 22, 33, 22, 1, 1);
			fdActorSetPos(ACTOR_SWITCHES_SWITCH_MARKER, 0, 0);
			fdScriptPlaySnd(SE_BUTTON_PRESS);
			syFieldScriptWait(30);
			fdScriptPlaySnd(SE_THUMP);
			fdMapGridCopyMetatilesTo(50, 20, 31, 20, 1, 2);
			syFieldScriptWait(5);
			fdMapGridCopyMetatilesTo(51, 20, 31, 20, 1, 2);
			syFieldScriptWait(5);
			fdMapGridSetMetatileHeight(toFIXED(496), toFIXED(336), 0, 0);
			syGameFlagSet(FLAG_ANEMOS_SWITCHES_SECOND_PILLAR_PLACED);
		}
		if(x == 93 && z == 57)
			syGameFlagSet(FLAG_ANEMOS_SWITCHES_THIRD_PILLAR_PLACED);
		if(x == 91 && z == 59) {
			if(!fdScriptGetFlag(FLAG_ANEMOS_SWITCHES_THIRD_PILLAR_PLACED)) {
				Sint32 fromX = toFIXED(
					FIXEDtoINT(fdActorGet(ACTOR_SWITCHES_PILLAR_B)->x.fixed));

				fdActorSetPosFX(ACTOR_SWITCHES_THIRD_PILLAR, fromX,
					toFIXED(FIXEDtoINT(
						fdActorGet(ACTOR_SWITCHES_PILLAR_B)->z.fixed)));
				fdActorSetPos(ACTOR_SWITCHES_PILLAR_B, 728, 472);
			}
			syGameFlagSet(FLAG_ANEMOS_SWITCHES_PILLAR_B_PLACED);
		}
	}
	if(MainRoutineStateWork.next ==
			SCENE_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST &&
		x == 33 && z == 45)
		syGameFlagSet(FLAG_ANEMOS_ORIHALCON_CHEST_FIRST_PILLAR_PLACED);
	fdScriptEnd();
}

extern Sint32 AnemosInnerSanctumSwitchGlowScript[];

/* A round switch stepped on: it lights under the leader in a burst of blue
   rings and its scene flag is set; once all seven are pressed, the room's lift
   floats up with its arrow pointing up (FLAG_LIFT_READY), ready to carry the
   party. */
void scAnemosInnerSanctumPressRoundSwitch(void)
{
	FIELD_STATE *state = RamHeapSlots->fieldState;
	ACTOR		*leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	ACTOR		*actor;
	Sint32		 all;
	Sint32		 i;
	Sint32		 id;

	if(MainRoutineStateWork.next == SCENE_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST)
		fdMapGridCopyMetatilesTo(17, 71, FIELD_METATILE(leader->x.fixed),
			FIELD_METATILE(leader->z.fixed) + OVERHEAD_LAYER_ROW, 1, 1);
	else
		fdMapGridCopyMetatilesTo(54, 87, FIELD_METATILE(leader->x.fixed),
			FIELD_METATILE(leader->z.fixed) + OVERHEAD_LAYER_ROW, 1, 1);
	fdScriptPlaySnd(SE_DRIP);
	actor = scFieldActorCreate(SPRITE_ENERGY_CHARGE,
		FIELD_METATILE_ORIGIN(FIELD_METATILE(leader->x.fixed)) + toFIXED(8), 0,
		FIELD_METATILE_ORIGIN(FIELD_METATILE(leader->z.fixed)) + toFIXED(8));
	actor->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
	actor->motionFlg = 0;
	scFieldActorSetSprCol(actor, SPRITE_COLOR_GBR);
	actor->scaleY = toFIXED(11) / 8;
	actor->scaleX = toFIXED(11) / 8;
	fdSetActorSprPriority(actor, 2);
	fdActorSelectAnimation(actor, ENERGY_CHARGE_ANIMATION_BURST);
	fdActorSetSprFlags(actor, 0);
	fdActorStartScript(actor, AnemosInnerSanctumSwitchGlowScript);
	fdScriptSetFlag(
		FLAG_ROUND_SWITCH_PRESSED(state->stepCue - CUE_FIRST_ROUND_SWITCH));
	all = fdScriptGetFlag(FLAG_ROUND_SWITCH_PRESSED(0));
	for(i = 1; i != ROUND_SWITCHES; i++)
		all &= fdScriptGetFlag(FLAG_ROUND_SWITCH_PRESSED(i));
	if(all) {
		fdScriptStart();
		syFieldScriptEndLastingPsynergy(0);
		syFieldScriptWait(10);
		fdCameraSetSpeed(0.8);
		id = ACTOR_ELEVATOR_LIFT;
		if(MainRoutineStateWork.next ==
			SCENE_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST)
			id = ACTOR_ORIHALCON_CHEST_LIFT;
		fdCameraMoveToActorById(id, 1);
		fdCameraWaitForArrival();
		syFieldScriptWait(10);
		fdScriptPlaySnd(SE_LEMURIAN_SHIP_LIFT_FAST);
		fdActorSetAnimation(id, ARROW_PANEL_ANIMATION_UP);
		fdScriptSetFlag(FLAG_LIFT_READY);
		fdActorGet(id)->motionFlg = ACTOR_FLOATS;
		syFieldScriptWait(80);
		fdScriptEnd();
	}
}

/* Felix steps onto the lift once it floats ready: he walks to its middle, turns
   to face south, and it carries the party up out of the room to the room
   above. */
void scAnemosInnerSanctumRideLiftUp(void)
{
	ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	ACTOR *lift;

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	if(MainRoutineStateWork.next ==
		SCENE_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST) {
		lift = fdActorGet(ACTOR_ORIHALCON_CHEST_LIFT);
		syFieldScriptSetExitRequest(
			ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_TO_ANEMOS_INNER_SANCTUM_GUARDIAN_SOUTHWEST);
	} else {
		lift = fdActorGet(ACTOR_ELEVATOR_LIFT);
		syFieldScriptSetExitRequest(EXIT_UP_THE_LIFT);
	}
	{
		Sint32 x = (FIELD_METATILE(leader->x.fixed) << 4) + 8;
		Sint32 z = (FIELD_METATILE(leader->z.fixed) << 4) + 8;

		fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, x, z);
	}
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_SOUTH, 10);
	syFieldScriptWait(30);
	lift->motionFlg = 0;
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	lift->speedLimit = toFIXED(2);
	lift->acceleration = toFIXED(1) / 32;
	fdScriptPlaySnd(SE_KOLIMA_BRIDGE_POST);
	scFieldActorSetMoveTargetFX(
		lift, lift->x.fixed, lift->y.fixed + toFIXED(4), lift->z.fixed);
	scFieldActorWaitForMove(lift);
	fdSetActorSprPriority(leader, 1);
	fdSetActorSprPriority(lift, 1);
	scFieldActorSetMoveTargetFX(
		lift, lift->x.fixed, lift->y.fixed + toFIXED(128), lift->z.fixed);
	syFieldScriptWait(80);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptEnd();
}

/* Felix casts Sand standing on sand and sinks into it; as a mound of sand he
   can then travel along the stretch of sand, where on foot he cannot pass,
   until it wears off.

   While it lasts the leader moves on the overhead layer, where the sand's own
   metatiles are. */
void scAnemosInnerSanctumSinkIntoSand(void)
{
	ACTOR *actor;
	Sint32 behavior;

	actor = fdActorGet(MainRoutineStateWork.leaderActorId);
	behavior = syFieldMapGridGetMetatileBehaviorAt(
		MAPGRID_LAYER_OVERHEAD, actor->x.fixed, actor->z.fixed);
	if(behavior == METATILE_BEHAVIOR_SAND) {
		actor->layer = MAPGRID_LAYER_OVERHEAD;
	}
}

/* Nothing visible of its own: as Sand wears off and Felix rises from the sand,
   the game sets him walking on the ground's metatiles again. */
void scAnemosInnerSanctumEndSand(void)
{
	fdActorGet(MainRoutineStateWork.leaderActorId)->layer =
		MAPGRID_LAYER_GROUND;
}

/* Stepping onto the switch: the leader walks onto its middle, it sinks with a
   click and the door west of it opens with a thump. */
void scAnemosInnerSanctumPressSwitch(void)
{
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 536, 360);
	syFieldScriptWait(8);
	fdMapGridCopyMetatilesTo(50, 22, 33, 22, 1, 1);
	fdActorSetPos(ACTOR_SWITCHES_SWITCH_MARKER, 0, 0);
	fdScriptPlaySnd(SE_BUTTON_PRESS);
	syFieldScriptWait(30);
	fdScriptPlaySnd(SE_THUMP);
	fdMapGridCopyMetatilesTo(50, 20, 31, 20, 1, 2);
	syFieldScriptWait(5);
	fdMapGridCopyMetatilesTo(51, 20, 31, 20, 1, 2);
	syFieldScriptWait(5);
	fdScriptSetFlag(FLAG_SWITCH_PRESSED);
	fdScriptEnd();
}

/* Stepping off the pressed switch to the east: it rises again with a click and
   its door shuts with a thump. */
void scAnemosInnerSanctumStepOffSwitch(void)
{
	ACTOR *platform;

	platform = fdActorGet(ACTOR_SWITCHES_SWITCH_MARKER);
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	platform->x.fixed = toFIXED(536);
	platform->z.fixed = toFIXED(360);
	platform->groundHeight = -toFIXED(14);
	platform->y.fixed = -toFIXED(14);
	fdMapGridCopyMetatilesTo(51, 22, 33, 22, 1, 1);
	fdScriptPlaySnd(SE_BUTTON_PRESS);
	syFieldScriptWait(30);
	fdScriptPlaySnd(SE_THUMP);
	fdMapGridCopyMetatilesTo(50, 20, 31, 20, 1, 2);
	syFieldScriptWait(5);
	fdMapGridCopyMetatilesTo(52, 20, 31, 20, 1, 2);
	syFieldScriptWait(5);
	fdScriptClearFlag(FLAG_SWITCH_PRESSED);
	fdScriptEnd();
}

Bool scCrackedTileStep();

/* Defines a step cue on a cracked floor: when a tile gives way under the
   leader, the party leaves by exit `exit`, through the floor. */
#define DEFINE_ANEMOS_INNER_SANCTUM_EXIT_IF(name, exit) \
	void name(void) \
	{ \
		Sint32 fell; \
\
		fell = scCrackedTileStep(); \
		if(fell != 0) { \
			syFieldScriptSetExitRequest(exit); \
		} \
	}

/* Stepping onto the first cracked floor: once a tile gives way, the party falls
   through to the part of the room below. */
DEFINE_ANEMOS_INNER_SANCTUM_EXIT_IF(
	scAnemosInnerSanctumFallThroughFirstCracks, THROUGH_FIRST_CRACKED_FLOOR)

/* As scAnemosInnerSanctumFallThroughFirstCracks, for the second floor. */
DEFINE_ANEMOS_INNER_SANCTUM_EXIT_IF(
	scAnemosInnerSanctumFallThroughSecondCracks, THROUGH_SECOND_CRACKED_FLOOR)

extern Sint32 AnemosInnerSanctumDustScript[];

/* A mimic stone's update: each step the leader takes, it takes too, mirrored
   east for west, scraping up dust, unless a wall stands in its way; on its
   button or over a hole it stops and raises that cue. Once the leader goes back
   south of its row, it settles on its metatile and may be started again.
   sceneWord and acceleration hold the leader's last x and z. */
void scAnemosInnerSanctumMimicLeader(ACTOR *self)
{
	ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	FIXED  next[2];
	FIXED  ahead[3];
	ACTOR *dust;
	Sint32 behavior;

	if(FIXEDtoINT(fdActorGet(MainRoutineStateWork.leaderActorId)->z.fixed) >
		MIMIC_STONE_LINE_Z) {
		self->update = 0;
		fdScriptClearFlag(FLAG_MIMIC_STONE_STARTED);
		self->x.fixed = (self->x.fixed & FIELD_METATILE_MASK) + toFIXED(8);
		self->z.fixed = (self->z.fixed & FIELD_METATILE_MASK) + toFIXED(8);
		return;
	}
	ahead[0] = next[0] = self->x.fixed - (leader->x.fixed - self->sceneWord);
	ahead[1] = leader->y.fixed;
	ahead[2] = next[1] = self->z.fixed + (leader->z.fixed - self->acceleration);
	if(self->sceneWord != leader->x.fixed ||
		self->acceleration != leader->z.fixed) {
		dust = (scFieldActorCreate(
			SPRITE_PARTICLE, self->x.fixed, self->y.fixed, self->z.fixed));
		dust->x.fixed += toFIXED((syRandom() & 15) - 8);
		dust->y.fixed += toFIXED((syRandom() & 31) - 4);
		dust->motionFlg = 0;
		scFieldActorSetSprCol(dust, self->sceneCnt);
		fdSetActorSprPriority(dust, 2);
		fdActorSelectAnimation(dust, 1);
		fdActorSetSprFlags(dust, 0);
		fdActorStartScript(dust, AnemosInnerSanctumDustScript);
		self->sceneWord = leader->x.fixed;
		self->acceleration = leader->z.fixed;
		if((TickCount & 3) == 0)
			fdScriptPlaySnd(SE_DRIP);
	}
	if(FIXEDtoINT(ahead[2]) > MIMIC_STONE_LINE_Z)
		return;
	syFieldVectorAddPolarOffset(7, DEGtoANG(180) - leader->facing, ahead);
	if(syFieldMapGridGetMetatileBehaviorAt(self->layer, ahead[0], ahead[2]) ==
		METATILE_BEHAVIOR_WALL)
		return;
	{
		FIXED x = next[0];

		self->x.fixed = x;
		self->z.fixed = next[1];
		behavior = syFieldMapGridGetMetatileBehaviorAt(self->layer, x, next[1]);
	}
	if(behavior == CUE_MIMIC_STONE_BUTTON) {
		FieldStateWork->raisedCue = CUE_MIMIC_STONE_ON_BUTTON;
		self->update = 0;
	} else if(behavior == CUE_MIMIC_STONE_HOLE) {
		FieldStateWork->raisedCue = CUE_MIMIC_STONE_IN_HOLE;
		self->update = 0;
	}
}

#include "text/msg_ids.h"

void scTeleportStartFastShimmer(void);

/* Stepping onto the floor before a mimic stone: the stone of the room the party
   came into starts to mirror the leader's steps. */
void scAnemosInnerSanctumStartMimicStone(void)
{
	ACTOR *actor;

	if(MainRoutineStateWork.entrance == ENTRANCE_BY_MIMIC_STONE_A)
		actor = fdActorGet(ACTOR_MIMIC_STONE_A);
	else if(MainRoutineStateWork.entrance == ENTRANCE_BY_MIMIC_STONE_B)
		actor = fdActorGet(ACTOR_MIMIC_STONE_B);
	else
		actor = fdActorGet(ACTOR_MIMIC_STONE_C);
	if(fdActorGet(MainRoutineStateWork.leaderActorId)->z.part[0].integer <
		MIMIC_STONE_LINE_Z) {
		actor->motionFlg = 0;
		actor->update = scAnemosInnerSanctumMimicLeader;
		actor->sceneWord =
			fdActorGet(MainRoutineStateWork.leaderActorId)->x.fixed;
		actor->acceleration =
			fdActorGet(MainRoutineStateWork.leaderActorId)->z.fixed;
		fdScriptSetFlag(FLAG_MIMIC_STONE_STARTED);
	}
}

/* A mimic stone falls into a hole: it slides to the hole's middle and drops in
   with a puff of dust, hidden below the hole's edge by a crate's silhouette
   drawn as an object window, and is taken away. */
void scAnemosInnerSanctumDropMimicStone(void)
{
	ACTOR	  *stone;
	ACTOR	  *mask;
	FIELD_SPR *spr;
	FIXED	   pos[3];

	if(MainRoutineStateWork.entrance == ENTRANCE_BY_MIMIC_STONE_A)
		stone = fdActorGet(ACTOR_MIMIC_STONE_A);
	else if(MainRoutineStateWork.entrance == ENTRANCE_BY_MIMIC_STONE_B)
		stone = fdActorGet(ACTOR_MIMIC_STONE_B);
	else
		stone = fdActorGet(ACTOR_MIMIC_STONE_C);
	pos[0] = (stone->x.fixed & FIELD_METATILE_MASK) + toFIXED(8);
	pos[2] = (stone->z.fixed & FIELD_METATILE_MASK) + toFIXED(8);
	stone->acceleration = toFIXED(1);
	stone->speedLimit = toFIXED(1);
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	scFieldActorSetMoveTargetFX(stone, pos[0], 0, pos[2]);
	mask = fdActorCreateSilhouette(
		stone->x.fixed, 0, pos[2] + toFIXED(59), SPRITE_CRATE);
	mask->scaleX = toFIXED(3) / 2;
	mask->scaleY = toFIXED(3) / 2;
	spr = mask->spr;
	spr->objMode = OBJ_MODE_WINDOW;
	REG_WINOUT = WINOUT_WIN01_ALL | WINOUT_WINOBJ_BG0 | WINOUT_WINOBJ_BG1 |
		WINOUT_WINOBJ_BG2 | WINOUT_WINOBJ_BG3 | WINOUT_WINOBJ_CLR;
	REG_DISPCNT |= DISPCNT_OBJWIN_ON;
	fdScriptPlaySnd(SE_DUST_PUFF);
	stone->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	fdActorWaitUntilLanded(stone);
	stone->x.fixed = 0;
	stone->y.fixed = 0;
	stone->z.fixed = 0;
	/* Stone A goes too, whichever stone fell; the room places it again when it
	   is next entered. */
	fdActorSetPos(ACTOR_MIMIC_STONE_A, 0, 0);
	fdActorRelease(mask);
	fdScriptEnd();
}

/* A mimic stone reaches its button: it settles on the button's middle with a
   thump, and its room's north door opens, its flag keeping both so. */
void scAnemosInnerSanctumOpenMimicStoneDoor(void)
{
	ACTOR *stone;
	Sint32 column;
	FIXED  tgt[3];

	if(MainRoutineStateWork.entrance == ENTRANCE_BY_MIMIC_STONE_A) {
		stone = fdActorGet(ACTOR_MIMIC_STONE_A);
		fdScriptSetFlag(FLAG_MIMIC_STONE_A_ON_BUTTON);
		column = 11;
	} else if(MainRoutineStateWork.entrance == ENTRANCE_BY_MIMIC_STONE_B) {
		stone = fdActorGet(ACTOR_MIMIC_STONE_B);
		fdScriptSetFlag(FLAG_MIMIC_STONE_B_ON_BUTTON);
		column = 26;
	} else {
		stone = fdActorGet(ACTOR_MIMIC_STONE_C);
		fdScriptSetFlag(FLAG_MIMIC_STONE_C_ON_BUTTON);
		column = 53;
	}
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	tgt[0] = (stone->x.fixed & FIELD_METATILE_MASK) + toFIXED(8);
	tgt[2] = (stone->z.fixed & FIELD_METATILE_MASK) + toFIXED(8);
	stone->acceleration = toFIXED(1);
	stone->speedLimit = toFIXED(1);
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	scFieldActorSetMoveTargetFX(stone, tgt[0], 0, tgt[2]);
	scFieldActorWaitForMove(stone);
	fdScriptPlaySnd(SE_THUMP);
	fdMapGridCopyMetatilesTo(18, 4, column, 4, 1, 2);
	syWait(5);
	fdMapGridCopyMetatilesTo(18, 7, column, 4, 1, 2);
	syWait(5);
	fdMapGridSetMetatileHeight(
		FIELD_METATILE_ORIGIN(column), toFIXED(88), 0, 0);
	fdScriptEnd();
}

/* The first time Felix comes up to Charon's slate, the room's colours warm and
   the slate speaks: the power of the land to create a great darkness is for
   whoever attacks it. */
void scAnemosInnerSanctumApproachCharonSlate(void)
{
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdPaletteFadeApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
	fdPaletteFadeApplyTarget(PALETTE_TINT_WARM, PALETTES_BG);
	fdPaletteFadeInterpolate(60);
	syFieldScriptWait(60);
	/* "We inherited the power of the land to create a great darkness. If you
	   crave this power, attack this slate!" */
	mnFieldMessageShowCentered((Sint32)MsgSummonInheritedEarthDarkness, 0, 6);
	fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
	fdPaletteFadeInterpolate(60);
	syFieldScriptWait(60);
	fdScriptSetFlag(FLAG_ANEMOS_CHARON_SLATE_SPOKE);
	fdScriptEnd();
}

extern CUE AnemosInnerSanctumCues[];
extern CUE AnemosInnerSanctumDragonSkinChestCues[];
extern CUE AnemosInnerSanctumElevatorCues[];
extern CUE AnemosInnerSanctumSwitchesCues[];
extern CUE AnemosInnerSanctumOrihalconChestCues[];
extern CUE AnemosInnerSanctumGuardianCues[];

const CUE *scGetCues(void)
{
	if(MainRoutineStateWork.next ==
		SCENE_ANEMOS_INNER_SANCTUM_DRAGON_SKIN_CHEST)
		return AnemosInnerSanctumDragonSkinChestCues;
	if(MainRoutineStateWork.next == SCENE_ANEMOS_INNER_SANCTUM_ELEVATOR)
		return AnemosInnerSanctumElevatorCues;
	if(MainRoutineStateWork.next == SCENE_ANEMOS_INNER_SANCTUM_SWITCHES)
		return AnemosInnerSanctumSwitchesCues;
	if(MainRoutineStateWork.next == SCENE_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST)
		return AnemosInnerSanctumOrihalconChestCues;
	if(MainRoutineStateWork.next == SCENE_ANEMOS_INNER_SANCTUM_GUARDIAN)
		return AnemosInnerSanctumGuardianCues;
	return AnemosInnerSanctumCues;
}

/* Felix casts Teleport on the circle in the middle of the symbols' room: sparks
   fly as the colours cycle, and the party teleports back to Contigo. */
void scAnemosInnerSanctumTeleportOut(void)
{
	scTeleportStartShimmer();
	fdTeleportPlaySparks();
	MainRoutineStateWork.teleportState = TELEPORT_ARRIVING;
	fdScriptSetDestination(
		SCENE_CONTIGO, CONTIGO_FROM_ANEMOS_INNER_SANCTUM_DRAGON_SKIN_CHEST);
	syFieldScriptWait(40);
	scTeleportStopShimmer();
}

extern Sint16					   AnemosInnerSanctumNorthDoorFrames[];
extern MAP_GRID_METATILE_ANIMATION AnemosInnerSanctumMimicStoneDoors[];

/* Plays a door's map edit at x, y. */
static __inline__ void scAnemosInnerSanctumAnimateDoor(
	const void *steps, Sint32 x, Sint32 y)
{
	fdMapGridPlayMapEdit(steps, x, y);
}

/* Unused: stills every placed actor, opens the north door or a mimic stone's
   door by the step cue's number, and walks the leader out by the exit of that
   number. */
UNUSED void scAnemosInnerSanctumEnterTheRoom(void)
{
	FIELD_STATE *state = RamHeapSlots->fieldState;
	Sint32		 i;

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	for(i = ACTOR_FIRST_PLACED; (Uint32)i < FIELD_TREASURE_ACTOR; i++) {
		ACTOR *actor = fdActorGet(i);

		if(actor != NULL)
			actor->motionFlg = 0;
	}
	fdScriptPlaySnd(SE_DOOR_OPEN);
	if(MainRoutineStateWork.next ==
		SCENE_ANEMOS_INNER_SANCTUM_DRAGON_SKIN_CHEST) {
		fdMapGridAnimateMetatiles(AnemosInnerSanctumNorthDoorFrames, 33, 7);
	} else {
		Sint32 idx = state->stepCue / 2 - 1;

		scAnemosInnerSanctumAnimateDoor(
			AnemosInnerSanctumMimicStoneDoors[idx].steps,
			AnemosInnerSanctumMimicStoneDoors[idx].x,
			AnemosInnerSanctumMimicStoneDoors[idx].y);
	}
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_WALK);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.5);
	fdActorSetDestinationOffset(MainRoutineStateWork.leaderActorId, 0, -4);
	syFieldScriptWait(6);
	syFieldScriptSetExitRequest(state->stepCue);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptEnd();
}

/* The party arrives up the lift in the elevator room: it rises the last of the
   way into the room with Felix on it and stays there. */
void scAnemosInnerSanctumArriveOnElevatorLift(void)
{
	ACTOR *lift = fdActorGet(ACTOR_ELEVATOR_LIFT);

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdActorSetAnimation(ACTOR_ELEVATOR_LIFT, ARROW_PANEL_ANIMATION_UP);
	lift->y.fixed = -toFIXED(64);
	fdActorSetPos(ACTOR_ELEVATOR_LIFT, 728, 120);
	fdActorSetPos(MainRoutineStateWork.leaderActorId, 728, 120);
	fdActorGet(MainRoutineStateWork.leaderActorId)->y.fixed = -toFIXED(48);
	fdScriptStartScreenTransition();
	lift->acceleration = toFIXED(1);
	lift->speedLimit = toFIXED(1);
	scFieldActorSetMoveTargetFX(
		lift, lift->x.fixed, lift->y.fixed + toFIXED(48), lift->z.fixed);
	scFieldActorWaitForMove(lift);
	fdScriptSetFlag(FLAG_ELEVATOR_LIFT_RISEN);
	fdScriptEnd();
}

/* As scAnemosInnerSanctumArriveOnElevatorLift, on the guardian room's lift. */
void scAnemosInnerSanctumArriveOnGuardianLift(void)
{
	ACTOR *lift = fdActorGet(ACTOR_GUARDIAN_LIFT);

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	lift->motionFlg = 0;
	lift->y.fixed = -toFIXED(64);
	fdActorGet(MainRoutineStateWork.leaderActorId)->y.fixed = -toFIXED(48);
	fdScriptStartScreenTransition();
	lift->acceleration = toFIXED(1);
	lift->speedLimit = toFIXED(1);
	scFieldActorSetMoveTargetFX(
		lift, lift->x.fixed, lift->y.fixed + toFIXED(48), lift->z.fixed);
	scFieldActorWaitForMove(lift);
	fdScriptSetFlag(FLAG_GUARDIAN_LIFT_RISEN);
	fdScriptEnd();
}

Sint32		  scCrackedTileFind();
void		  scCrackedTilePlaceLanding(Sint32 x, Sint32 z, Sint32 effect);
extern Uint16 TeleportCirclePaletteSlots[];
extern Uint16 TeleportCircleColors[];
extern Uint16 TeleportCircleShimmerColors[];
extern Uint16 TeleportCircleShimmerSlots[];

/* Sets up the room the party comes into: the Dragon Skin room's lit torches and
   opened north door; the pillars left in their places; the lifts, below or
   above as they last carried the party, and the round switches already pressed;
   the cracked floors; and the mimic stones resting on their buttons, their
   doors open. */
Sint32 scInit(void)
{
	Sint32 scene;
	Sint32 entrance;

	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	scene = MainRoutineStateWork.next;
	if(scene == SCENE_ANEMOS_INNER_SANCTUM_DRAGON_SKIN_CHEST) {
		scPaletteSetColsFade((Sint32)TeleportCirclePaletteSlots,
			(Sint32)TeleportCircleColors, (Sint32)TeleportCircleShimmerColors,
			(Sint32)TeleportCircleShimmerSlots);
		if(fdScriptGetFlag(FLAG_ANEMOS_VENUS_TORCH_LIT)) {
			scAnemosInnerSanctumLightTorch(
				VENUS_TORCH_X, VENUS_TORCH_Y, SPRITE_COLOR_NORMAL);
			fdMapGridCopyMetatilesTo(85, 34, 85, 8, 9, 8);
		}
		if(fdScriptGetFlag(FLAG_ANEMOS_MERCURY_TORCH_LIT)) {
			scAnemosInnerSanctumLightTorch(
				MERCURY_TORCH_X, MERCURY_TORCH_Y, SPRITE_COLOR_BGR);
			fdMapGridCopyMetatilesTo(101, 34, 101, 8, 9, 8);
		}
		if(fdScriptGetFlag(FLAG_ANEMOS_MARS_TORCH_LIT)) {
			scAnemosInnerSanctumLightTorch(
				MARS_TORCH_X, MARS_TORCH_Y, SPRITE_COLOR_RED_TINT);
			fdMapGridCopyMetatilesTo(85, 46, 85, 20, 9, 8);
		}
		if(fdScriptGetFlag(FLAG_ANEMOS_JUPITER_TORCH_LIT)) {
			scAnemosInnerSanctumLightTorch(
				JUPITER_TORCH_X, JUPITER_TORCH_Y, SPRITE_COLOR_RBG);
			fdMapGridCopyMetatilesTo(101, 46, 101, 20, 9, 8);
		}
		if(fdScriptGetFlag(FLAG_ANEMOS_NORTH_DOOR_OPEN)) {
			fdMapGridCopyMetatilesTo(54, 7, 33, 7, 1, 2);
			fdMapGridSetMetatileHeight(toFIXED(536), toFIXED(136), 0, 0);
		}
	} else if(scene == SCENE_ANEMOS_INNER_SANCTUM_ELEVATOR) {
		scFieldActorSetSprPriority(ACTOR_ELEVATOR_FIRST_PILLAR, 1);
		if(fdScriptGetFlag(FLAG_ANEMOS_ELEVATOR_FIRST_PILLAR_PLACED)) {
			fdActorSetPos(ACTOR_ELEVATOR_FIRST_PILLAR, 296, 248);
			scFieldActorSetSprPriority(ACTOR_ELEVATOR_FIRST_PILLAR, 2);
		}
		if(fdScriptGetFlag(FLAG_ANEMOS_ELEVATOR_SECOND_PILLAR_PLACED))
			fdActorSetPos(ACTOR_ELEVATOR_SECOND_PILLAR, 440, 760);
		Call1((void (*)())fdActorPlaceBlocks,
			(Sint32)AnemosInnerSanctumElevatorBlocks);
		fdActorGet(MainRoutineStateWork.leaderActorId)->priFlg |=
			ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
		fdActorGet(ACTOR_ELEVATOR_LIFT)->motionFlg = 0;
		fdActorGet(ACTOR_ELEVATOR_LIFT)->priFlg |= ACTOR_PRIORITY_PLATFORM;
		if(MainRoutineStateWork.entrance ==
				WAY_ELEVATOR_ABOVE_THE_LIFT_TO_NORTHWEST &&
			fdScriptGetFlag(FLAG_ELEVATOR_LIFT_RISEN)) {
			fdActorSetAnimation(ACTOR_ELEVATOR_LIFT, ARROW_PANEL_ANIMATION_UP);
			fdActorSetPos(ACTOR_ELEVATOR_LIFT, 728, 120);
		}
		if(MainRoutineStateWork.entrance == ENTRANCE_UP_THE_LIFT &&
			syGameFlagGet(FLAG_ELEVATOR_LIFT_RISEN) == 0)
			scAnemosInnerSanctumArriveOnElevatorLift();
		if(MainRoutineStateWork.entrance ==
			WAY_ELEVATOR_ROUND_SWITCHES_TO_EAST) {
			if(fdScriptGetFlag(FLAG_ELEVATOR_LIFT_RISEN))
				fdActorSetPos(ACTOR_ELEVATOR_LIFT, 0, 0);
			if(fdScriptGetFlag(FLAG_LIFT_READY))
				fdActorGet(ACTOR_ELEVATOR_LIFT)->motionFlg = ACTOR_FLOATS;
			if(fdScriptGetFlag(FLAG_ROUND_SWITCH_PRESSED(0)))
				fdMapGridCopyMetatilesTo(54, 87, 44, 86, 1, 1);
			if(fdScriptGetFlag(FLAG_ROUND_SWITCH_PRESSED(1)))
				fdMapGridCopyMetatilesTo(54, 87, 46, 86, 1, 1);
			if(fdScriptGetFlag(FLAG_ROUND_SWITCH_PRESSED(2)))
				fdMapGridCopyMetatilesTo(54, 87, 43, 87, 1, 1);
			if(fdScriptGetFlag(FLAG_ROUND_SWITCH_PRESSED(3)))
				fdMapGridCopyMetatilesTo(54, 87, 45, 87, 1, 1);
			if(fdScriptGetFlag(FLAG_ROUND_SWITCH_PRESSED(4)))
				fdMapGridCopyMetatilesTo(54, 87, 47, 87, 1, 1);
			if(fdScriptGetFlag(FLAG_ROUND_SWITCH_PRESSED(5)))
				fdMapGridCopyMetatilesTo(54, 87, 44, 88, 1, 1);
			if(fdScriptGetFlag(FLAG_ROUND_SWITCH_PRESSED(6)))
				fdMapGridCopyMetatilesTo(54, 87, 46, 88, 1, 1);
			fdActorGet(MainRoutineStateWork.leaderActorId)->priFlg |=
				ACTOR_PRIORITY_IGNORES_RISES;
		}
	} else if(scene == SCENE_ANEMOS_INNER_SANCTUM_SWITCHES) {
		ACTOR *marker = fdActorGet(ACTOR_SWITCHES_SWITCH_MARKER);

		marker->motionFlg = ACTOR_FALLS;
		marker->groundHeight = -toFIXED(14);
		marker->y.fixed = -toFIXED(14);
		marker->priFlg |= ACTOR_PRIORITY_PLATFORM;
		fdActorGet(MainRoutineStateWork.leaderActorId)->priFlg |=
			ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
		fdActorGet(ACTOR_SWITCHES_BOULDER)->sceneCnt = 1;
		if(fdScriptGetFlag(FLAG_ANEMOS_SWITCHES_FIRST_PILLAR_PLACED))
			fdActorSetPos(ACTOR_SWITCHES_FIRST_PILLAR, 56, 424);
		if(fdScriptGetFlag(FLAG_ANEMOS_SWITCHES_SECOND_PILLAR_PLACED)) {
			fdActorSetPos(ACTOR_SWITCHES_SECOND_PILLAR, 536, 360);
			fdMapGridCopyMetatilesTo(51, 20, 31, 20, 1, 2);
			fdMapGridSetMetatileHeight(toFIXED(496), toFIXED(336), 0, 0);
		}
		if(fdScriptGetFlag(FLAG_ANEMOS_SWITCHES_THIRD_PILLAR_PLACED))
			fdActorSetPos(ACTOR_SWITCHES_THIRD_PILLAR, 744, 456);
		if(fdScriptGetFlag(FLAG_ANEMOS_SWITCHES_PILLAR_B_PLACED)) {
			if(!fdScriptGetFlag(FLAG_KEEP_PARTY_POSITION) &&
				!fdScriptGetFlag(FLAG_ANEMOS_SWITCHES_THIRD_PILLAR_PLACED))
				fdActorSetPosFX(ACTOR_SWITCHES_THIRD_PILLAR,
					toFIXED(
						fdActorGet(ACTOR_SWITCHES_PILLAR_B)->x.part[0].integer),
					toFIXED(fdActorGet(ACTOR_SWITCHES_PILLAR_B)
							->z.part[0]
							.integer));
			fdActorSetPos(ACTOR_SWITCHES_PILLAR_B, 728, 472);
		}
		Call1((void (*)())fdActorPlaceBlocks,
			(Sint32)AnemosInnerSanctumSwitchesBlocks);
	} else if(scene == SCENE_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST) {
		if(fdScriptGetFlag(FLAG_ANEMOS_ORIHALCON_CHEST_FIRST_PILLAR_PLACED))
			fdActorSetPos(ACTOR_ORIHALCON_CHEST_FIRST_PILLAR, 264, 360);
		Call1((void (*)())fdActorPlaceBlocks,
			(Sint32)AnemosInnerSanctumOrihalconChestBlocks);
		entrance = MainRoutineStateWork.entrance;
		if(entrance ==
			ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_FROM_ANEMOS_INNER_SANCTUM_SWITCHES_NORTHWEST) {
			scCrackedTileFind(CUE_FIRST_CRACKED_FLOOR, FLAG_SCENE_TEMP_80,
				FLAG_AREA_TEMP_80, 18, 68);
			if(fdScriptGetFlag(FLAG_GUARDIAN_LIFT_RISEN) == 0) {
				fdActorGet(MainRoutineStateWork.leaderActorId)->priFlg |=
					ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
				fdActorGet(ACTOR_ORIHALCON_CHEST_LIFT)->motionFlg = 0;
				fdActorGet(ACTOR_ORIHALCON_CHEST_LIFT)->priFlg |=
					ACTOR_PRIORITY_PLATFORM;
			} else {
				fdActorSetPos(ACTOR_ORIHALCON_CHEST_LIFT, 0, 0);
				fdScriptSetFlag(FLAG_ROUND_SWITCH_PRESSED(0));
				fdScriptSetFlag(FLAG_ROUND_SWITCH_PRESSED(1));
				fdScriptSetFlag(FLAG_ROUND_SWITCH_PRESSED(2));
				fdScriptSetFlag(FLAG_ROUND_SWITCH_PRESSED(3));
				fdScriptSetFlag(FLAG_ROUND_SWITCH_PRESSED(4));
				fdScriptSetFlag(FLAG_ROUND_SWITCH_PRESSED(5));
				fdScriptSetFlag(FLAG_ROUND_SWITCH_PRESSED(6));
			}
			if(fdScriptGetFlag(FLAG_LIFT_READY))
				fdActorGet(ACTOR_ORIHALCON_CHEST_LIFT)->motionFlg =
					ACTOR_FLOATS;
			if(fdScriptGetFlag(FLAG_ROUND_SWITCH_PRESSED(0)))
				fdMapGridCopyMetatilesTo(17, 71, 8, 69, 1, 1);
			if(fdScriptGetFlag(FLAG_ROUND_SWITCH_PRESSED(5)))
				fdMapGridCopyMetatilesTo(17, 71, 6, 71, 1, 1);
			if(fdScriptGetFlag(FLAG_ROUND_SWITCH_PRESSED(1)))
				fdMapGridCopyMetatilesTo(17, 71, 5, 75, 1, 1);
			if(fdScriptGetFlag(FLAG_ROUND_SWITCH_PRESSED(2)))
				fdMapGridCopyMetatilesTo(17, 71, 8, 75, 1, 1);
			if(fdScriptGetFlag(FLAG_ROUND_SWITCH_PRESSED(3)))
				fdMapGridCopyMetatilesTo(17, 71, 11, 75, 1, 1);
			if(fdScriptGetFlag(FLAG_ROUND_SWITCH_PRESSED(4)))
				fdMapGridCopyMetatilesTo(17, 71, 8, 73, 1, 1);
			if(fdScriptGetFlag(FLAG_ROUND_SWITCH_PRESSED(6)))
				fdMapGridCopyMetatilesTo(17, 71, 10, 71, 1, 1);
		} else if(entrance == THROUGH_FIRST_CRACKED_FLOOR) {
			scCrackedTileFind(CUE_FIRST_CRACKED_FLOOR, FLAG_SCENE_TEMP_80,
				FLAG_AREA_TEMP_80, 18, 68);
			scCrackedTilePlaceLanding(22, 0,
				SCENE_TRANSITION(
					TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE));
		} else if(entrance == THROUGH_SECOND_CRACKED_FLOOR) {
			scCrackedTileFind(CUE_SECOND_CRACKED_FLOOR, FLAG_SCENE_TEMP_80,
				FLAG_AREA_TEMP_80, 18, 68);
			if(fdScriptGetFlag(FLAG_KEEP_PARTY_POSITION) == 0)
				scCrackedTilePlaceLanding(36, -36,
					SCENE_TRANSITION(
						TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE));
		} else if(MainRoutineStateWork.entrance >=
				WAY_ORIHALCON_CHEST_CRACKED_FLOOR_TO_SOUTHEAST &&
			MainRoutineStateWork.entrance <=
				WAY_ORIHALCON_CHEST_CRACKED_FLOOR_TO_SOUTHEAST + 1) {
			scCrackedTileFind(CUE_SECOND_CRACKED_FLOOR, FLAG_SCENE_TEMP_80,
				FLAG_AREA_TEMP_80, 18, 68);
		}
	} else if(scene == SCENE_ANEMOS_INNER_SANCTUM_GUARDIAN) {
		MAP_STATE *map = RamHeapSlots->mapGrid;

		switch(MainRoutineStateWork.entrance) {
		case ANEMOS_INNER_SANCTUM_GUARDIAN_FROM_ANEMOS_INNER_SANCTUM_SWITCHES_SOUTHWEST:
			if(fdScriptGetFlag(FLAG_GUARDIAN_LIFT_RISEN) == 0)
				fdActorSetPos(ACTOR_GUARDIAN_LIFT, 0, 0);
		case ANEMOS_INNER_SANCTUM_GUARDIAN_FROM_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_SOUTHWEST:
			fdActorGet(MainRoutineStateWork.leaderActorId)->priFlg |=
				ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
			fdActorSetAnimation(ACTOR_GUARDIAN_LIFT, ARROW_PANEL_ANIMATION_UP);
			fdActorGet(ACTOR_GUARDIAN_LIFT)->motionFlg = 0;
			fdActorGet(ACTOR_GUARDIAN_LIFT)->priFlg |= ACTOR_PRIORITY_PLATFORM;
			fdActorSetAnimation(ACTOR_GUARDIAN_LIFT, ARROW_PANEL_ANIMATION_UP);
			if(fdScriptGetFlag(FLAG_GUARDIAN_LIFT_RISEN) == 0 &&
				MainRoutineStateWork.entrance ==
					ANEMOS_INNER_SANCTUM_GUARDIAN_FROM_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_SOUTHWEST)
				scAnemosInnerSanctumArriveOnGuardianLift();
			else
				fdActorGet(ACTOR_GUARDIAN_LIFT)->y.fixed = -toFIXED(16);
			break;
		case ENTRANCE_BY_MIMIC_STONE_A:
		case ANEMOS_INNER_SANCTUM_GUARDIAN_FROM_ANEMOS_INNER_SANCTUM_ELEVATOR_NORTHWEST:
			if(fdScriptGetFlag(FLAG_MIMIC_STONE_A_ON_BUTTON)) {
				fdActorSetPos(ACTOR_MIMIC_STONE_A, 120, 120);
				fdScriptSetFlag(FLAG_MIMIC_STONE_STARTED);
			} else {
				fdMapGridCopyMetatilesTo(18, 1, 11, 4, 1, 2);
				fdMapGridSetMetatileHeight(toFIXED(184), toFIXED(88), 0, 2);
			}
			break;
		case ENTRANCE_BY_MIMIC_STONE_B:
		case ANEMOS_INNER_SANCTUM_GUARDIAN_FROM_ANEMOS_INNER_SANCTUM_SWITCHES_NORTHEAST:
			if(fdScriptGetFlag(FLAG_MIMIC_STONE_B_ON_BUTTON)) {
				fdActorSetPos(ACTOR_MIMIC_STONE_B, 552, 120);
				fdScriptSetFlag(FLAG_MIMIC_STONE_STARTED);
			} else {
				fdMapGridCopyMetatilesTo(18, 1, 26, 4, 1, 2);
				fdMapGridSetMetatileHeight(toFIXED(424), toFIXED(88), 0, 2);
			}
			break;
		case ENTRANCE_BY_MIMIC_STONE_C:
		case ANEMOS_INNER_SANCTUM_GUARDIAN_FROM_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_NORTHEAST:
			if(fdScriptGetFlag(FLAG_MIMIC_STONE_C_ON_BUTTON)) {
				fdActorSetPos(ACTOR_MIMIC_STONE_C, 792, 120);
				fdScriptSetFlag(FLAG_MIMIC_STONE_STARTED);
			} else {
				fdMapGridCopyMetatilesTo(18, 1, 53, 4, 1, 2);
				fdMapGridSetMetatileHeight(toFIXED(856), toFIXED(88), 0, 2);
			}
			map->minX += toFIXED(8);
			map->maxX += toFIXED(8);
			break;
		}
	}
	return 0;
}

Sint32 scPrepareMap(void)
{
	return 0;
}

#include "../field/field_effect_set_sprite_priority.c"

/* An effect's update: moves it by its velocity, grows it by its scale rates and
   spins its sprite. */
void scEffectMoveLocal(FIELD_OBJECT *object)
{
	Sint32	   x;
	Sint32	   velX;
	FIELD_SPR *spr;

	x = object->eff.x;
	velX = object->eff.vel.x;
	spr = object->eff.spr;

	object->eff.x = x + velX;
	object->eff.y += object->eff.vel.y;
	object->eff.z += object->eff.vel.z;
	object->eff.scaleX += object->eff.scaleRateX;
	object->eff.scaleY += object->eff.scaleRateY;
	spr->rotation += object->eff.spin;
}

extern Sint32 FieldEffectDurationScripts[];
void		  fdEffectSetSprPriority(ACTOR *actor, Sint32 pri);

/* As scFieldEffectSpawn, the effect taking the leader's sprite priority and
   moved by scEffectMoveLocal: the shared spawner's update and priority setter
   are fixed, so this copy is written out too. */
void scFieldEffectSpawnLocal(Sint32 x, Sint32 y, Sint32 z, Sint32 velX,
	Sint32 velY, Sint32 velZ, Uint32 flg, const EFFECT_OPTIONS *options)
{
	FIELD_EFFECT *party =
		((FIELD_EFFECT *)fdActorGetById(MainRoutineStateWork.leaderActorId));
	FIELD_EFFECT *effect;
	FIELD_SPR	 *spr;
	const Sint32 *script;
	Uint32		  start;

	if((flg & EFFECT_USE_TYPE) != 0 && options != 0)
		effect = (FIELD_EFFECT *)syImportActorCreate(options->type, x, y, z);
	else
		effect =
			(FIELD_EFFECT *)syImportActorCreate(SPRITE_DUST_CLOUD, x, y, z);
	if(effect == 0)
		return;

	spr = effect->spr;
	fdActorSelectAnimation(effect, (flg + 1) & EFFECT_SCRIPT_MASK);
	fdActorStartScript(
		effect, FieldEffectDurationScripts[flg & EFFECT_SCRIPT_MASK]);
	effect->motionFlg = 0;
	fdActorSetSprFlags(effect, 0);
	effect->update = scEffectMoveLocal;
	effect->vel.x = velX;
	effect->vel.y = velY;
	effect->vel.z = velZ;
	fdEffectSetSprPriority(effect, party->spr->pri);
	effect->scaleRateX = 0;
	effect->scaleRateY = 0;
	effect->spin = 0;

	if((flg & EFFECT_OPTION_MASK) == 0 || options == 0)
		return;
	if((flg & EFFECT_USE_PALETTE) != 0)
		scFieldActorSetSprCol(effect, options->pal);
	if((flg & EFFECT_USE_PRIORITY) != 0) {
		effect->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;
		fdEffectSetSprPriority(effect, options->pri);
	}
	start = flg & EFFECT_USE_START_SCALE;
	if(start != 0) {
		effect->scaleX = options->startScaleX;
		effect->scaleY = options->startScaleY;
	}
	if((flg & EFFECT_SCALE_TO_TARGET) != 0) {
		FIXED  tgtX;
		Sint32 cnt;

		script = FieldEffectDurationScripts[flg & EFFECT_SCRIPT_MASK];
		tgtX = options->tgtScaleX;
		cnt = script[EFFECT_SCRIPT_REPEAT_COUNT];
		if(start != 0) {
			effect->scaleRateX = (tgtX - effect->scaleX) / cnt;
			effect->scaleRateY = (options->tgtScaleY - effect->scaleY) /
				script[EFFECT_SCRIPT_REPEAT_COUNT];
		} else {
			effect->scaleRateX = (tgtX - toFIXED(1)) / cnt;
			effect->scaleRateY = (options->tgtScaleY - toFIXED(1)) /
				script[EFFECT_SCRIPT_REPEAT_COUNT];
		}
	}
	if((flg & EFFECT_USE_SCRIPT) != 0) {
		fdActorSelectAnimation(effect, 1);
		fdActorStartScript(effect, options->script);
	}
	if((flg & EFFECT_USE_ROTATION) != 0)
		spr->rotation = options->rotation;
	if((flg & EFFECT_USE_SPIN) != 0)
		effect->spin = options->spin;
	if((flg & EFFECT_USE_UPDATE) != 0)
		effect->update = options->update;
}

#define METATILE_PUSH_SLOT_ENTRY syOAMAddSprAtDepth
#include "../field/cracked_tile_mask.c"

#include "shared/cracked_tile_find.h"

#include "shared/leader_center_in_metatile.h"

#include "shared/cracked_tile_crack.h"

#include "shared/cracked_tile_fall.h"

#include "../field/field_effect_advance_motion.c"

#undef DEFINE_ANEMOS_INNER_SANCTUM_EXIT_IF
#undef METATILE_PUSH_SLOT_ENTRY

/* The leader lands from the cracked floor above in a ring of dust, as Mars
   Lighthouse's lands from its cracks (MarsLighthouseEntranceLandFromCrack). */
void scCrackedTileDropIn(void)
{
	ACTOR		  *leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	Uint32		   i;
	EFFECT_OPTIONS options;

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdMapGridCenterOnFollowedPoint();
	syWait(1);
	leader->y.fixed = toFIXED(130);
	leader->gravity = toFIXED(1) / 2;
	leader->bounce = 0;
	leader->motionFlg = 0;
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptPlaySnd(SE_DUST_PUFF);
	leader->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	syFieldScriptWait(24);
	options.pal = SPRITE_COLOR_WHITE;
	options.update = fdEffectAdvanceMotion;
	options.startScaleX = toFIXED(16) / 25;
	options.startScaleY = toFIXED(16) / 25;
	for(i = 0; i <= DUST_RING_PUFFS; i++) {
		Sint32 ang;
		Sint32 vel[3];
		FIXED  cosine;

		ang = i * (TRIG_TURN / DUST_RING_PUFFS);
		cosine = syCos(ang);
		vel[0] = cosine;
		vel[1] = 0;
		vel[2] = syMathSin(ang);
		vel[0] += vel[0] >> 1;
		vel[0] += (syRandom() * (toFIXED(3) / 4) >> 16) - (toFIXED(3) / 8);
		vel[2] += (syRandom() * (toFIXED(3) / 8) >> 16) - (toFIXED(3) / 16);
		scFieldEffectSpawnLocal(leader->x.fixed, leader->y.fixed,
			leader->z.fixed, vel[0], vel[1], vel[2],
			EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE |
				EFFECT_SCRIPT_MEDIUM,
			&options);
	}
	fdScriptPlaySnd(SE_THUMP);
	fdActorSetAttachedEffect(MainRoutineStateWork.leaderActorId,
		ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_LEADER_SIT);
	fdMapGridSetShake(5, 5, 1);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	syFieldMapGridWaitForShakeToSettle();
	fdActorSetAttachedEffect(MainRoutineStateWork.leaderActorId,
		ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
	leader->gravity = toFIXED(1);
	leader->bounce = toFIXED(1) / 4;
	syFieldScriptWait(10);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_STAND);
	fdScriptEnd();
}

void syGameFlagSetByte(Sint32 flg, Sint32 value);
#include "shared/cracked_tile_step.h"

#include "shared/cracked_tile_place_landing.h"

extern Uint32 FieldEffectShortScript[];
extern Uint32 FieldEffectMediumScript[];

/* clang-format off */
/* The same three lifetimes as FieldEffectShortScript, MediumScript and */
/* LongScript (shared/field_effect_duration_scripts.c), under other names. */
Uint32 AnemosInnerSanctumShortScript[] = {
	SC_WAIT(1),
	SC_LOOP(22, SC_TOP),
	SC_WAIT(1),
	SC_POSITION(0, 0, 0),
	SC_WAIT(1),
	SC_RELEASE,
	SC_END,
};

Uint32 AnemosInnerSanctumMediumScript[] = {
	SC_WAIT(1),
	SC_LOOP(44, SC_TOP),
	SC_WAIT(1),
	SC_POSITION(0, 0, 0),
	SC_WAIT(1),
	SC_RELEASE,
	SC_END,
};

Uint32 AnemosInnerSanctumLongScript[] = {
	SC_WAIT(1),
	SC_LOOP(126, SC_TOP),
	SC_WAIT(1),
	SC_POSITION(0, 0, 0),
	SC_WAIT(1),
	SC_RELEASE,
	SC_END,
};

Sint32 AnemosInnerSanctumSwitchGlowScript[] = {
	SC_WAIT(20),
	SC_RELEASE,
	SC_END,
};

Sint32 AnemosInnerSanctumOrbScript[] = {
	SC_CALL(scAnemosInnerSanctumCircleOrb),
	SC_RELEASE,
	SC_END,
};

Sint32 AnemosInnerSanctumDustScript[] = {
	SC_LABEL(1),
	SC_ADD(SW_SCALE_X, -toFIXED(1) / 10),
	SC_ADD(SW_SCALE_Y, -toFIXED(1) / 10),
	SC_WAIT(1),
	SC_LOOP(8, SC_TO(1)),
	SC_RELEASE,
	SC_END,
};

Uint32 AnemosInnerSanctumDjinniRisesScript[] = {
	SC_WAIT(50),
	SC_LABEL(1),
	SC_ADD(SW_SCALE_X, toFIXED(1) / 10),
	SC_ADD(SW_SCALE_Y, toFIXED(1) / 10),
	SC_ADD(SW_Y, toFIXED(3)),
	SC_WAIT(2),
	SC_LOOP(10, SC_TO(1)),
	SC_SET(SW_MOTION, ACTOR_FLOATS),
	SC_END,
};

Sint32 AnemosInnerSanctumDjinniSinksScript[] = {
	SC_WAIT(30),
	SC_SET(SW_MOTION, 0),
	SC_LABEL(1),
	SC_ADD(SW_SCALE_X, -toFIXED(1) / 10),
	SC_ADD(SW_SCALE_Y, -toFIXED(1) / 10),
	SC_ADD(SW_Y, -toFIXED(3)),
	SC_WAIT(2),
	SC_LOOP(10, SC_TO(1)),
	SC_END,
};

Uint32 AnemosInnerSanctumFadingSparkScript[] = {
	SC_WAIT(3),
	SC_RELEASE,
	SC_END,
};

Sint32 AnemosInnerSanctumSparkScript[] = {
	SC_SET(SW_SCALE_X, toFIXED(2)),
	SC_SET(SW_SCALE_Y, toFIXED(2)),
	SC_LABEL(1),
	SC_ADD(SW_SCALE_X, -toFIXED(1) / 10),
	SC_ADD(SW_SCALE_Y, -toFIXED(1) / 10),
	SC_WAIT(2),
	SC_LOOP(10, SC_TO(1)),
	SC_CALL((Sint32)scAnemosInnerSanctumSpiralIn),
	SC_CALL((Sint32)scAnemosInnerSanctumLightPlate),
	SC_RELEASE,
	SC_END,
};

/* The blocks Move pushes in each room (fdActorLiftTargetBlock), ended by -1: */
/* the elevator room's two pillars. */
Sint16 AnemosInnerSanctumElevatorBlocks[] = {
	ACTOR_ELEVATOR_FIRST_PILLAR, ACTOR_ELEVATOR_SECOND_PILLAR, -1,
};

/* The switches room's five pillars. */
Sint16 AnemosInnerSanctumSwitchesBlocks[] = {
	ACTOR_SWITCHES_FIRST_PILLAR, ACTOR_SWITCHES_SECOND_PILLAR,
	ACTOR_SWITCHES_PILLAR_A,     ACTOR_SWITCHES_PILLAR_B,
	ACTOR_SWITCHES_THIRD_PILLAR, -1,
};

/* The Orihalcon chest room's two pillars. */
Sint16 AnemosInnerSanctumOrihalconChestBlocks[] = {
	ACTOR_ORIHALCON_CHEST_FIRST_PILLAR, ACTOR_ORIHALCON_CHEST_SECOND_PILLAR, -1,
};

/* Where the sparks fly to round the leader as a symbol lights, in pixels */
/* from him in x and z. */
Sint32 AnemosInnerSanctumPlateOffsets[][2] = {
	{-48, -32}, {48, -32}, {-64, -16}, {64, -16}, {-32, -16}, {32, -16},
	{-48, 0}, {48, 0}, {-32, 0}, {32, 0}, {-64, 16}, {64, 16}, {-32, 16},
	{32, 16}, {-48, 32}, {48, 32}, {-16, 32}, {16, 32},
};

#include "shared/field_effect_duration_scripts.c"

/* The field effects' scripts by how long they last, which scFieldEffectSpawn */
/* picks by its flags (EFFECT_SCRIPT_MASK). */
Sint32 AnemosInnerSanctumDurationScripts[] = {
	(Sint32)AnemosInnerSanctumShortScript,
	(Sint32)AnemosInnerSanctumMediumScript,
	(Sint32)AnemosInnerSanctumLongScript,
};

#define TELEPORT_CIRCLE_FIRST_ENTRY 89
#include "shared/teleport_circle_palette.c"

Sint32 AnemosInnerSanctumNoShadowScript[] = {
	SC_CALL((Sint32)scAnemosInnerSanctumClearScriptedActorSprFlags),
	SC_END,
};

#include "shared/scene_entrances_default.c"

Uint32 SceneExits[] = {
	SCENE_EXITS(AnemosInnerSanctumDragonSkinChest),
	SCENE_EXIT_UNLESS(ANEMOS_INNER_SANCTUM_DRAGON_SKIN_CHEST_TO_ANEMOS_INNER_SANCTUM_ENTRANCE, AnemosInnerSanctumEntrance, ANEMOS_INNER_SANCTUM_ENTRANCE_FROM_ANEMOS_INNER_SANCTUM_DRAGON_SKIN_CHEST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(ANEMOS_INNER_SANCTUM_DRAGON_SKIN_CHEST_TO_ANEMOS_INNER_SANCTUM_GUARDIAN, AnemosInnerSanctumGuardian, ANEMOS_INNER_SANCTUM_GUARDIAN_FROM_ANEMOS_INNER_SANCTUM_DRAGON_SKIN_CHEST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(ANEMOS_INNER_SANCTUM_DRAGON_SKIN_CHEST_TO_CONTIGO, Contigo, CONTIGO_FROM_ANEMOS_INNER_SANCTUM_DRAGON_SKIN_CHEST, EXIT_FLAG_NONE),
	SCENE_EXITS(AnemosInnerSanctumElevator),
	SCENE_EXIT_UNLESS(WAY_ELEVATOR_NORTHWEST_TO_SOUTHWEST, AnemosInnerSanctumElevator, WAY_ELEVATOR_SOUTHWEST_TO_NORTHWEST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(WAY_ELEVATOR_NORTHWEST_TO_EAST, AnemosInnerSanctumElevator, WAY_ELEVATOR_EAST_TO_NORTHWEST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(WAY_ELEVATOR_NORTHWEST_TO_ABOVE_THE_LIFT, AnemosInnerSanctumElevator, WAY_ELEVATOR_ABOVE_THE_LIFT_TO_NORTHWEST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(ANEMOS_INNER_SANCTUM_ELEVATOR_TO_ANEMOS_INNER_SANCTUM_SWITCHES, AnemosInnerSanctumSwitches, ANEMOS_INNER_SANCTUM_SWITCHES_FROM_ANEMOS_INNER_SANCTUM_ELEVATOR, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(WAY_ELEVATOR_SOUTHWEST_TO_NORTHWEST, AnemosInnerSanctumElevator, WAY_ELEVATOR_NORTHWEST_TO_SOUTHWEST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(ANEMOS_INNER_SANCTUM_ELEVATOR_TO_ANEMOS_INNER_SANCTUM_GUARDIAN_NORTHWEST, AnemosInnerSanctumGuardian, ANEMOS_INNER_SANCTUM_GUARDIAN_FROM_ANEMOS_INNER_SANCTUM_ELEVATOR_NORTHWEST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(WAY_ELEVATOR_SOUTHWEST_TO_EAST, AnemosInnerSanctumElevator, WAY_ELEVATOR_EAST_TO_SOUTHWEST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(ANEMOS_INNER_SANCTUM_ELEVATOR_TO_ANEMOS_INNER_SANCTUM_GUARDIAN_WEST, AnemosInnerSanctumGuardian, ANEMOS_INNER_SANCTUM_GUARDIAN_FROM_ANEMOS_INNER_SANCTUM_ELEVATOR_WEST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(WAY_ELEVATOR_SOUTHWEST_TO_SOUTHEAST, AnemosInnerSanctumElevator, WAY_ELEVATOR_SOUTHEAST_TO_SOUTHWEST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(WAY_ELEVATOR_ROUND_SWITCHES_TO_EAST, AnemosInnerSanctumElevator, WAY_ELEVATOR_EAST_TO_ROUND_SWITCHES, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(EXIT_UP_THE_LIFT, AnemosInnerSanctumElevator, ENTRANCE_UP_THE_LIFT, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(WAY_ELEVATOR_EAST_TO_NORTHWEST, AnemosInnerSanctumElevator, WAY_ELEVATOR_NORTHWEST_TO_EAST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(WAY_ELEVATOR_EAST_TO_SOUTHWEST, AnemosInnerSanctumElevator, WAY_ELEVATOR_SOUTHWEST_TO_EAST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(WAY_ELEVATOR_EAST_TO_ROUND_SWITCHES, AnemosInnerSanctumElevator, WAY_ELEVATOR_ROUND_SWITCHES_TO_EAST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(WAY_ELEVATOR_SOUTHEAST_TO_SOUTHWEST, AnemosInnerSanctumElevator, WAY_ELEVATOR_SOUTHWEST_TO_SOUTHEAST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(ANEMOS_INNER_SANCTUM_ELEVATOR_TO_ANEMOS_INNER_SANCTUM_GUARDIAN_EAST, AnemosInnerSanctumGuardian, ANEMOS_INNER_SANCTUM_GUARDIAN_FROM_ANEMOS_INNER_SANCTUM_ELEVATOR_EAST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(WAY_ELEVATOR_ABOVE_THE_LIFT_TO_NORTHWEST, AnemosInnerSanctumElevator, WAY_ELEVATOR_NORTHWEST_TO_ABOVE_THE_LIFT, EXIT_FLAG_NONE),
	SCENE_EXITS(AnemosInnerSanctumSwitches),
	SCENE_EXIT_UNLESS(WAY_SWITCHES_NORTH_TO_MIDDLE, AnemosInnerSanctumSwitches, WAY_SWITCHES_MIDDLE_TO_NORTH, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(ANEMOS_INNER_SANCTUM_SWITCHES_TO_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_SOUTHEAST, AnemosInnerSanctumOrihalconChest, ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_FROM_ANEMOS_INNER_SANCTUM_SWITCHES_SOUTHEAST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(WAY_SWITCHES_NORTHEAST_TO_MIDDLE_WEST, AnemosInnerSanctumSwitches, WAY_SWITCHES_MIDDLE_TO_NORTHEAST_WEST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(WAY_SWITCHES_NORTHEAST_TO_MIDDLE_EAST, AnemosInnerSanctumSwitches, WAY_SWITCHES_MIDDLE_TO_NORTHEAST_EAST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(WAY_SWITCHES_WEST_TO_MIDDLE, AnemosInnerSanctumSwitches, WAY_SWITCHES_MIDDLE_TO_WEST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(ANEMOS_INNER_SANCTUM_SWITCHES_TO_ANEMOS_INNER_SANCTUM_GUARDIAN_SOUTHWEST, AnemosInnerSanctumGuardian, ANEMOS_INNER_SANCTUM_GUARDIAN_FROM_ANEMOS_INNER_SANCTUM_SWITCHES_SOUTHWEST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(ANEMOS_INNER_SANCTUM_SWITCHES_TO_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_NORTH, AnemosInnerSanctumOrihalconChest, ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_FROM_ANEMOS_INNER_SANCTUM_SWITCHES_NORTH, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(ANEMOS_INNER_SANCTUM_SWITCHES_TO_ANEMOS_INNER_SANCTUM_ELEVATOR, AnemosInnerSanctumElevator, ANEMOS_INNER_SANCTUM_ELEVATOR_FROM_ANEMOS_INNER_SANCTUM_SWITCHES, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(ANEMOS_INNER_SANCTUM_SWITCHES_TO_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_NORTHWEST, AnemosInnerSanctumOrihalconChest, ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_FROM_ANEMOS_INNER_SANCTUM_SWITCHES_NORTHWEST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(WAY_SWITCHES_MIDDLE_TO_WEST, AnemosInnerSanctumSwitches, WAY_SWITCHES_WEST_TO_MIDDLE, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(WAY_SWITCHES_MIDDLE_TO_SOUTH, AnemosInnerSanctumSwitches, WAY_SWITCHES_SOUTH_TO_MIDDLE, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(ANEMOS_INNER_SANCTUM_SWITCHES_TO_ANEMOS_INNER_SANCTUM_GUARDIAN_NORTHEAST, AnemosInnerSanctumGuardian, ANEMOS_INNER_SANCTUM_GUARDIAN_FROM_ANEMOS_INNER_SANCTUM_SWITCHES_NORTHEAST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(WAY_SWITCHES_MIDDLE_TO_NORTH, AnemosInnerSanctumSwitches, WAY_SWITCHES_NORTH_TO_MIDDLE, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(WAY_SWITCHES_MIDDLE_TO_NORTHEAST_WEST, AnemosInnerSanctumSwitches, WAY_SWITCHES_NORTHEAST_TO_MIDDLE_WEST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(WAY_SWITCHES_MIDDLE_TO_NORTHEAST_EAST, AnemosInnerSanctumSwitches, WAY_SWITCHES_NORTHEAST_TO_MIDDLE_EAST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(ANEMOS_INNER_SANCTUM_SWITCHES_TO_ANEMOS_INNER_SANCTUM_GUARDIAN_EAST, AnemosInnerSanctumGuardian, ANEMOS_INNER_SANCTUM_GUARDIAN_FROM_ANEMOS_INNER_SANCTUM_SWITCHES_EAST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(WAY_SWITCHES_SOUTH_TO_MIDDLE, AnemosInnerSanctumSwitches, WAY_SWITCHES_MIDDLE_TO_SOUTH, EXIT_FLAG_NONE),
	SCENE_EXITS(AnemosInnerSanctumOrihalconChest),
	SCENE_EXIT_UNLESS(ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_TO_ANEMOS_INNER_SANCTUM_SWITCHES_NORTHWEST, AnemosInnerSanctumSwitches, ANEMOS_INNER_SANCTUM_SWITCHES_FROM_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_NORTHWEST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_TO_ANEMOS_INNER_SANCTUM_SWITCHES_NORTH, AnemosInnerSanctumSwitches, ANEMOS_INNER_SANCTUM_SWITCHES_FROM_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_NORTH, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(WAY_ORIHALCON_CHEST_NORTHEAST_TO_SOUTHEAST, AnemosInnerSanctumOrihalconChest, WAY_ORIHALCON_CHEST_SOUTHEAST_TO_NORTHEAST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(WAY_ORIHALCON_CHEST_CRACKED_FLOOR_TO_SOUTHEAST, AnemosInnerSanctumOrihalconChest, WAY_ORIHALCON_CHEST_SOUTHEAST_TO_CRACKED_FLOOR, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_TO_ANEMOS_INNER_SANCTUM_ENTRANCE, AnemosInnerSanctumEntrance, ANEMOS_INNER_SANCTUM_ENTRANCE_FROM_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_TO_ANEMOS_INNER_SANCTUM_GUARDIAN_EAST, AnemosInnerSanctumGuardian, ANEMOS_INNER_SANCTUM_GUARDIAN_FROM_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_EAST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_TO_ANEMOS_INNER_SANCTUM_SWITCHES_SOUTHEAST, AnemosInnerSanctumSwitches, ANEMOS_INNER_SANCTUM_SWITCHES_FROM_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_SOUTHEAST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(WAY_ORIHALCON_CHEST_SOUTHEAST_TO_CRACKED_FLOOR, AnemosInnerSanctumOrihalconChest, WAY_ORIHALCON_CHEST_CRACKED_FLOOR_TO_SOUTHEAST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(WAY_ORIHALCON_CHEST_SOUTHEAST_TO_NORTHEAST, AnemosInnerSanctumOrihalconChest, WAY_ORIHALCON_CHEST_NORTHEAST_TO_SOUTHEAST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_TO_ANEMOS_INNER_SANCTUM_GUARDIAN_NORTHEAST, AnemosInnerSanctumGuardian, ANEMOS_INNER_SANCTUM_GUARDIAN_FROM_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_NORTHEAST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(THROUGH_FIRST_CRACKED_FLOOR, AnemosInnerSanctumOrihalconChest, THROUGH_FIRST_CRACKED_FLOOR, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(THROUGH_SECOND_CRACKED_FLOOR, AnemosInnerSanctumOrihalconChest, THROUGH_SECOND_CRACKED_FLOOR, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_TO_ANEMOS_INNER_SANCTUM_GUARDIAN_SOUTHWEST, AnemosInnerSanctumGuardian, ANEMOS_INNER_SANCTUM_GUARDIAN_FROM_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_SOUTHWEST, EXIT_FLAG_NONE),
	SCENE_EXITS(AnemosInnerSanctumGuardian),
	SCENE_EXIT_UNLESS(ANEMOS_INNER_SANCTUM_GUARDIAN_TO_ANEMOS_INNER_SANCTUM_ELEVATOR_WEST, AnemosInnerSanctumElevator, ANEMOS_INNER_SANCTUM_ELEVATOR_FROM_ANEMOS_INNER_SANCTUM_GUARDIAN_WEST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(ANEMOS_INNER_SANCTUM_GUARDIAN_TO_ANEMOS_INNER_SANCTUM_ELEVATOR_NORTHWEST, AnemosInnerSanctumElevator, ANEMOS_INNER_SANCTUM_ELEVATOR_FROM_ANEMOS_INNER_SANCTUM_GUARDIAN_NORTHWEST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(ANEMOS_INNER_SANCTUM_GUARDIAN_TO_ANEMOS_INNER_SANCTUM_SWITCHES_EAST, AnemosInnerSanctumSwitches, ANEMOS_INNER_SANCTUM_SWITCHES_FROM_ANEMOS_INNER_SANCTUM_GUARDIAN_EAST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(ANEMOS_INNER_SANCTUM_GUARDIAN_TO_ANEMOS_INNER_SANCTUM_SWITCHES_NORTHEAST, AnemosInnerSanctumSwitches, ANEMOS_INNER_SANCTUM_SWITCHES_FROM_ANEMOS_INNER_SANCTUM_GUARDIAN_NORTHEAST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(ANEMOS_INNER_SANCTUM_GUARDIAN_TO_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_EAST, AnemosInnerSanctumOrihalconChest, ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_FROM_ANEMOS_INNER_SANCTUM_GUARDIAN_EAST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(ANEMOS_INNER_SANCTUM_GUARDIAN_TO_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_NORTHEAST, AnemosInnerSanctumOrihalconChest, ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_FROM_ANEMOS_INNER_SANCTUM_GUARDIAN_NORTHEAST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(ANEMOS_INNER_SANCTUM_GUARDIAN_TO_ANEMOS_INNER_SANCTUM_SWITCHES_SOUTHWEST, AnemosInnerSanctumSwitches, ANEMOS_INNER_SANCTUM_SWITCHES_FROM_ANEMOS_INNER_SANCTUM_GUARDIAN_SOUTHWEST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(WAY_GUARDIAN_SOUTH_TO_SOUTHEAST, AnemosInnerSanctumGuardian, WAY_GUARDIAN_SOUTHEAST_TO_SOUTH, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(ANEMOS_INNER_SANCTUM_GUARDIAN_TO_ANEMOS_INNER_SANCTUM_ELEVATOR_EAST, AnemosInnerSanctumElevator, ANEMOS_INNER_SANCTUM_ELEVATOR_FROM_ANEMOS_INNER_SANCTUM_GUARDIAN_EAST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(ANEMOS_INNER_SANCTUM_GUARDIAN_TO_ANEMOS_INNER_SANCTUM_DRAGON_SKIN_CHEST, AnemosInnerSanctumDragonSkinChest, ANEMOS_INNER_SANCTUM_DRAGON_SKIN_CHEST_FROM_ANEMOS_INNER_SANCTUM_GUARDIAN, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(WAY_GUARDIAN_SOUTHEAST_TO_SOUTH, AnemosInnerSanctumGuardian, WAY_GUARDIAN_SOUTH_TO_SOUTHEAST, EXIT_FLAG_NONE),
	SCENE_EXITS_END,
};

ACTOR_PLACE AnemosInnerSanctumPlacements[] = {
	{SCENE_TABLE_END},
};

ACTOR_PLACE AnemosInnerSanctumDragonSkinChestPlacements[] = {
	{SCENE_TABLE_END},
};

ACTOR_PLACE AnemosInnerSanctumElevatorPlacements[] = {
	{SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 1, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_ARROW_PANEL, CONDITION_ALWAYS, (Sint32)AnemosInnerSanctumNoShadowScript, 2, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 3, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SCENE_TABLE_END},
};

ACTOR_PLACE AnemosInnerSanctumSwitchesPlacements[] = {
	{SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 1, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 2, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 3, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 4, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 5, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 6, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 7, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SCENE_TABLE_END},
};

ACTOR_PLACE AnemosInnerSanctumOrihalconChestPlacements[] = {
	{SPRITE_ARROW_PANEL, CONDITION_ALWAYS, (Sint32)AnemosInnerSanctumNoShadowScript, 3, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 4, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 5, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SCENE_TABLE_END},
};

ACTOR_PLACE AnemosInnerSanctumGuardianPlacements[] = {
	{SPRITE_MIMIC_STONE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 1, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_MIMIC_STONE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 2, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_MIMIC_STONE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 3, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_ARROW_PANEL, CONDITION_ALWAYS, (Sint32)AnemosInnerSanctumNoShadowScript, 4, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	ACTOR_PLACEMENT(SPRITE_COILED_DRAGON_STATUE, CONDITION_ALWAYS, (Sint32)AnemosInnerSanctumNoShadowScript, 584, 0, 582, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_COILED_DRAGON_STATUE, CONDITION_ALWAYS, (Sint32)AnemosInnerSanctumNoShadowScript, 584, 0, 630, FACING_WEST, TALK_FACING_PARTY_AND_BACK, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_COILED_DRAGON_STATUE, CONDITION_ALWAYS, (Sint32)AnemosInnerSanctumNoShadowScript, 680, 0, 582, FACING_EAST, TALK_FACING_PARTY_AND_BACK, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_COILED_DRAGON_STATUE, CONDITION_ALWAYS, (Sint32)AnemosInnerSanctumNoShadowScript, 680, 0, 630, FACING_EAST, TALK_FACING_PARTY_AND_BACK, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	{SCENE_TABLE_END},
};

CUE AnemosInnerSanctumCues[] = {
	{CUE_TABLE_END},
};

CUE AnemosInnerSanctumDragonSkinChestCues[] = {
	{CUE_EXIT, 1, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_DRAGON_SKIN_CHEST_TO_ANEMOS_INNER_SANCTUM_ENTRANCE},
	{CUE_EXIT, 2, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_DRAGON_SKIN_CHEST_TO_ANEMOS_INNER_SANCTUM_GUARDIAN},
	{CUE_STEP, CUE_FLOOR_BETWEEN_SYMBOLS, CONDITION_ALWAYS, SCRIPT_REF(scAnemosInnerSanctumStepOffSymbol)},
	{CUE_STEP, CUE_VENUS_SYMBOL, FLAG_ON_SYMBOL, SCRIPT_REF(scAnemosInnerSanctumLightSymbol)},
	{CUE_STEP, CUE_MERCURY_SYMBOL, FLAG_ON_SYMBOL, SCRIPT_REF(scAnemosInnerSanctumLightSymbol)},
	{CUE_STEP, CUE_MARS_SYMBOL, FLAG_ON_SYMBOL, SCRIPT_REF(scAnemosInnerSanctumLightSymbol)},
	{CUE_STEP, CUE_JUPITER_SYMBOL, FLAG_ON_SYMBOL, SCRIPT_REF(scAnemosInnerSanctumLightSymbol)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_TELEPORT, PSYNERGY_PHASE_AFTER_CAST), CUE_TELEPORT_CIRCLE, CONDITION_ALWAYS, SCRIPT_REF(scAnemosInnerSanctumTeleportOut)},
	{CUE_TABLE_END},
};

CUE AnemosInnerSanctumElevatorCues[] = {
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 1, CONDITION_ALWAYS, 1},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
	{CUE_EXIT, 3, CONDITION_ALWAYS, 3},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 4, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_ELEVATOR_TO_ANEMOS_INNER_SANCTUM_SWITCHES},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 5, CONDITION_ALWAYS, 5},
	{CUE_EXIT, 6, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_ELEVATOR_TO_ANEMOS_INNER_SANCTUM_GUARDIAN_NORTHWEST},
	{CUE_EXIT, 7, CONDITION_ALWAYS, 7},
	{CUE_EXIT, 8, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_ELEVATOR_TO_ANEMOS_INNER_SANCTUM_GUARDIAN_WEST},
	{CUE_EXIT, 9, CONDITION_ALWAYS, 9},
	{CUE_EXIT, 10, CONDITION_ALWAYS, 10},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 12, CONDITION_ALWAYS, 12},
	{CUE_EXIT, 13, CONDITION_ALWAYS, 13},
	{CUE_EXIT, 14, CONDITION_ALWAYS, 14},
	{CUE_EXIT, 15, CONDITION_ALWAYS, 15},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 16, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_ELEVATOR_TO_ANEMOS_INNER_SANCTUM_GUARDIAN_EAST},
	{CUE_EXIT, 17, CONDITION_ALWAYS, 17},
	{CUE_STEP, CUE_LIFT, IF_FLAG_SET(FLAG_LIFT_READY), SCRIPT_REF(scAnemosInnerSanctumRideLiftUp)},
	{CUE_STEP, CUE_FIRST_ROUND_SWITCH, FLAG_ROUND_SWITCH_PRESSED(0), SCRIPT_REF(scAnemosInnerSanctumPressRoundSwitch)},
	{CUE_STEP, CUE_FIRST_ROUND_SWITCH + 1, FLAG_ROUND_SWITCH_PRESSED(1), SCRIPT_REF(scAnemosInnerSanctumPressRoundSwitch)},
	{CUE_STEP, CUE_FIRST_ROUND_SWITCH + 2, FLAG_ROUND_SWITCH_PRESSED(2), SCRIPT_REF(scAnemosInnerSanctumPressRoundSwitch)},
	{CUE_STEP, CUE_FIRST_ROUND_SWITCH + 3, FLAG_ROUND_SWITCH_PRESSED(3), SCRIPT_REF(scAnemosInnerSanctumPressRoundSwitch)},
	{CUE_STEP, CUE_FIRST_ROUND_SWITCH + 4, FLAG_ROUND_SWITCH_PRESSED(4), SCRIPT_REF(scAnemosInnerSanctumPressRoundSwitch)},
	{CUE_STEP, CUE_FIRST_ROUND_SWITCH + 5, FLAG_ROUND_SWITCH_PRESSED(5), SCRIPT_REF(scAnemosInnerSanctumPressRoundSwitch)},
	{CUE_STEP, CUE_FIRST_ROUND_SWITCH + 6, FLAG_ROUND_SWITCH_PRESSED(6), SCRIPT_REF(scAnemosInnerSanctumPressRoundSwitch)},
	{CUE_PUSH, 0, CONDITION_ALWAYS, SCRIPT_REF(scAnemosInnerSanctumStartMovingBlock)},
	{CUE_PUSHED, 0, CONDITION_ALWAYS, SCRIPT_REF(scAnemosInnerSanctumFinishMovingBlock)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_ELEVATOR_FIRST_PILLAR, FLAG_ANEMOS_ELEVATOR_FIRST_PILLAR_PLACED, SCRIPT_REF(scAnemosInnerSanctumStartMovingBlock)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ELEVATOR_FIRST_PILLAR, FLAG_ANEMOS_ELEVATOR_FIRST_PILLAR_PLACED, SCRIPT_REF(scAnemosInnerSanctumFinishMovingBlock)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_ELEVATOR_SECOND_PILLAR, FLAG_ANEMOS_ELEVATOR_SECOND_PILLAR_PLACED, SCRIPT_REF(scAnemosInnerSanctumStartMovingBlock)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ELEVATOR_SECOND_PILLAR, FLAG_ANEMOS_ELEVATOR_SECOND_PILLAR_PLACED, SCRIPT_REF(scAnemosInnerSanctumFinishMovingBlock)},
	{CUE_TABLE_END},
};

CUE AnemosInnerSanctumSwitchesCues[] = {
	{CUE_EXIT, 1, CONDITION_ALWAYS, 1},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_SWITCHES_TO_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_SOUTHEAST},
	{CUE_EXIT, 3, CONDITION_ALWAYS, 3},
	{CUE_EXIT, 4, CONDITION_ALWAYS, 4},
	{CUE_EXIT, 5, CONDITION_ALWAYS, 5},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 6, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_SWITCHES_TO_ANEMOS_INNER_SANCTUM_GUARDIAN_SOUTHWEST},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 7, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_SWITCHES_TO_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_NORTH},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 8, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_SWITCHES_TO_ANEMOS_INNER_SANCTUM_ELEVATOR},
	{CUE_EXIT, 9, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_SWITCHES_TO_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_NORTHWEST},
	{CUE_EXIT, 10, CONDITION_ALWAYS, 10},
	{CUE_EXIT, 11, CONDITION_ALWAYS, 11},
	{CUE_EXIT, 12, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_SWITCHES_TO_ANEMOS_INNER_SANCTUM_GUARDIAN_NORTHEAST},
	{CUE_EXIT, 13, CONDITION_ALWAYS, 13},
	{CUE_EXIT, 14, CONDITION_ALWAYS, 14},
	{CUE_EXIT, 15, CONDITION_ALWAYS, 15},
	{CUE_EXIT, 16, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_SWITCHES_TO_ANEMOS_INNER_SANCTUM_GUARDIAN_EAST},
	{CUE_EXIT, 17, CONDITION_ALWAYS, 17},
	{PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, SCRIPT_REF(scAnemosInnerSanctumSinkIntoSand)},
	{PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, SCRIPT_REF(scAnemosInnerSanctumEndSand)},
	{CUE_STEP, CUE_SWITCH, FLAG_SWITCH_PRESSED, SCRIPT_REF(scAnemosInnerSanctumPressSwitch)},
	{CUE_STEP, CUE_EAST_OF_SWITCH, IF_FLAG_SET(FLAG_SWITCH_PRESSED), SCRIPT_REF(scAnemosInnerSanctumStepOffSwitch)},
	{PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_SWITCHES_BOULDER, CONDITION_ALWAYS, 0},
	{CUE_PUSH, 0, CONDITION_ALWAYS, SCRIPT_REF(scAnemosInnerSanctumStartMovingBlock)},
	{CUE_PUSHED, 0, CONDITION_ALWAYS, SCRIPT_REF(scAnemosInnerSanctumFinishMovingBlock)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_SWITCHES_FIRST_PILLAR, FLAG_ANEMOS_SWITCHES_FIRST_PILLAR_PLACED, SCRIPT_REF(scAnemosInnerSanctumStartMovingBlock)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SWITCHES_FIRST_PILLAR, FLAG_ANEMOS_SWITCHES_FIRST_PILLAR_PLACED, SCRIPT_REF(scAnemosInnerSanctumFinishMovingBlock)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_SWITCHES_SECOND_PILLAR, FLAG_ANEMOS_SWITCHES_SECOND_PILLAR_PLACED, SCRIPT_REF(scAnemosInnerSanctumStartMovingBlock)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SWITCHES_SECOND_PILLAR, FLAG_ANEMOS_SWITCHES_SECOND_PILLAR_PLACED, SCRIPT_REF(scAnemosInnerSanctumFinishMovingBlock)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_SWITCHES_PILLAR_A, CONDITION_ALWAYS, SCRIPT_REF(scAnemosInnerSanctumStartMovingBlock)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SWITCHES_PILLAR_A, CONDITION_ALWAYS, SCRIPT_REF(scAnemosInnerSanctumFinishMovingBlock)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_SWITCHES_PILLAR_B, FLAG_ANEMOS_SWITCHES_PILLAR_B_PLACED, SCRIPT_REF(scAnemosInnerSanctumStartMovingBlock)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SWITCHES_PILLAR_B, FLAG_ANEMOS_SWITCHES_PILLAR_B_PLACED, SCRIPT_REF(scAnemosInnerSanctumFinishMovingBlock)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_SWITCHES_THIRD_PILLAR, FLAG_ANEMOS_SWITCHES_THIRD_PILLAR_PLACED, SCRIPT_REF(scAnemosInnerSanctumStartMovingBlock)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SWITCHES_THIRD_PILLAR, FLAG_ANEMOS_SWITCHES_THIRD_PILLAR_PLACED, SCRIPT_REF(scAnemosInnerSanctumFinishMovingBlock)},
	{CUE_TABLE_END},
};

CUE AnemosInnerSanctumOrihalconChestCues[] = {
	{CUE_EXIT, 1, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_TO_ANEMOS_INNER_SANCTUM_SWITCHES_NORTHWEST},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_TO_ANEMOS_INNER_SANCTUM_SWITCHES_NORTH},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 3, CONDITION_ALWAYS, 3},
	{CUE_EXIT, 4, CONDITION_ALWAYS, 4},
	{CUE_EXIT, 5, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_TO_ANEMOS_INNER_SANCTUM_ENTRANCE},
	{CUE_EXIT, 6, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_TO_ANEMOS_INNER_SANCTUM_GUARDIAN_EAST},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 7, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_TO_ANEMOS_INNER_SANCTUM_SWITCHES_SOUTHEAST},
	{CUE_EXIT, 8, CONDITION_ALWAYS, 8},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 9, CONDITION_ALWAYS, 9},
	{CUE_EXIT, 10, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_TO_ANEMOS_INNER_SANCTUM_GUARDIAN_NORTHEAST},
	{CUE_STEP, CUE_LIFT, IF_FLAG_SET(FLAG_LIFT_READY), SCRIPT_REF(scAnemosInnerSanctumRideLiftUp)},
	{CUE_STEP, CUE_FIRST_ROUND_SWITCH, FLAG_ROUND_SWITCH_PRESSED(0), SCRIPT_REF(scAnemosInnerSanctumPressRoundSwitch)},
	{CUE_STEP, CUE_FIRST_ROUND_SWITCH + 1, FLAG_ROUND_SWITCH_PRESSED(1), SCRIPT_REF(scAnemosInnerSanctumPressRoundSwitch)},
	{CUE_STEP, CUE_FIRST_ROUND_SWITCH + 2, FLAG_ROUND_SWITCH_PRESSED(2), SCRIPT_REF(scAnemosInnerSanctumPressRoundSwitch)},
	{CUE_STEP, CUE_FIRST_ROUND_SWITCH + 3, FLAG_ROUND_SWITCH_PRESSED(3), SCRIPT_REF(scAnemosInnerSanctumPressRoundSwitch)},
	{CUE_STEP, CUE_FIRST_ROUND_SWITCH + 4, FLAG_ROUND_SWITCH_PRESSED(4), SCRIPT_REF(scAnemosInnerSanctumPressRoundSwitch)},
	{CUE_STEP, CUE_FIRST_ROUND_SWITCH + 5, FLAG_ROUND_SWITCH_PRESSED(5), SCRIPT_REF(scAnemosInnerSanctumPressRoundSwitch)},
	{CUE_STEP, CUE_FIRST_ROUND_SWITCH + 6, FLAG_ROUND_SWITCH_PRESSED(6), SCRIPT_REF(scAnemosInnerSanctumPressRoundSwitch)},
	{CUE_STEP, CUE_FIRST_CRACKED_FLOOR, CONDITION_ALWAYS, SCRIPT_REF(scAnemosInnerSanctumFallThroughFirstCracks)},
	{CUE_STEP, CUE_FIRST_CRACKED_FLOOR + 1, CONDITION_ALWAYS, SCRIPT_REF(scAnemosInnerSanctumFallThroughFirstCracks)},
	{CUE_STEP, CUE_SECOND_CRACKED_FLOOR, CONDITION_ALWAYS, SCRIPT_REF(scAnemosInnerSanctumFallThroughSecondCracks)},
	{CUE_STEP, CUE_SECOND_CRACKED_FLOOR + 1, CONDITION_ALWAYS, SCRIPT_REF(scAnemosInnerSanctumFallThroughSecondCracks)},
	{PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, SCRIPT_REF(scAnemosInnerSanctumSinkIntoSand)},
	{PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, SCRIPT_REF(scAnemosInnerSanctumEndSand)},
	{CUE_PUSH, 0, CONDITION_ALWAYS, SCRIPT_REF(scAnemosInnerSanctumStartMovingBlock)},
	{CUE_PUSHED, 0, CONDITION_ALWAYS, SCRIPT_REF(scAnemosInnerSanctumFinishMovingBlock)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_ORIHALCON_CHEST_FIRST_PILLAR, FLAG_ANEMOS_ORIHALCON_CHEST_FIRST_PILLAR_PLACED, SCRIPT_REF(scAnemosInnerSanctumStartMovingBlock)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ORIHALCON_CHEST_FIRST_PILLAR, FLAG_ANEMOS_ORIHALCON_CHEST_FIRST_PILLAR_PLACED, SCRIPT_REF(scAnemosInnerSanctumFinishMovingBlock)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_ORIHALCON_CHEST_SECOND_PILLAR, CONDITION_ALWAYS, SCRIPT_REF(scAnemosInnerSanctumStartMovingBlock)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ORIHALCON_CHEST_SECOND_PILLAR, CONDITION_ALWAYS, SCRIPT_REF(scAnemosInnerSanctumFinishMovingBlock)},
	{CUE_TABLE_END},
};

CUE AnemosInnerSanctumGuardianCues[] = {
	{CUE_EXIT, 1, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_GUARDIAN_TO_ANEMOS_INNER_SANCTUM_ELEVATOR_WEST},
	{CUE_EXIT, 2, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_GUARDIAN_TO_ANEMOS_INNER_SANCTUM_ELEVATOR_NORTHWEST},
	{CUE_EXIT, 3, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_GUARDIAN_TO_ANEMOS_INNER_SANCTUM_SWITCHES_EAST},
	{CUE_EXIT, 4, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_GUARDIAN_TO_ANEMOS_INNER_SANCTUM_SWITCHES_NORTHEAST},
	{CUE_EXIT, 5, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_GUARDIAN_TO_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_EAST},
	{CUE_EXIT, 6, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_GUARDIAN_TO_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_NORTHEAST},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 7, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_GUARDIAN_TO_ANEMOS_INNER_SANCTUM_SWITCHES_SOUTHWEST},
	{CUE_EXIT, 8, CONDITION_ALWAYS, 8},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 9, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_GUARDIAN_TO_ANEMOS_INNER_SANCTUM_ELEVATOR_EAST},
	{CUE_EXIT, 10, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_GUARDIAN_TO_ANEMOS_INNER_SANCTUM_DRAGON_SKIN_CHEST},
	{CUE_EXIT, 11, CONDITION_ALWAYS, 11},
	{CUE_STEP, CUE_MIMIC_STONE_START, FLAG_MIMIC_STONE_STARTED, SCRIPT_REF(scAnemosInnerSanctumStartMimicStone)},
	{CUE_STEP, CUE_MIMIC_STONE_HOLE, FLAG_MIMIC_STONE_STARTED, SCRIPT_REF(scAnemosInnerSanctumStartMimicStone)},
	{CUE_STEP, CUE_MIMIC_STONE_BUTTON, FLAG_MIMIC_STONE_STARTED, SCRIPT_REF(scAnemosInnerSanctumStartMimicStone)},
	{CUE_STEP, CUE_BEFORE_CHARON_SLATE, FLAG_ANEMOS_CHARON_SLATE_SPOKE, SCRIPT_REF(scAnemosInnerSanctumApproachCharonSlate)},
	{CUE_RAISED, CUE_MIMIC_STONE_IN_HOLE, CONDITION_ALWAYS, SCRIPT_REF(scAnemosInnerSanctumDropMimicStone)},
	{CUE_RAISED, CUE_MIMIC_STONE_ON_BUTTON, CONDITION_ALWAYS, SCRIPT_REF(scAnemosInnerSanctumOpenMimicStoneDoor)},
	{CUE_TABLE_END},
	/* Unused: rows past the table's end. */
	{CUE_EXIT, 1, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_GUARDIAN_TO_ANEMOS_INNER_SANCTUM_ELEVATOR_WEST},
	{CUE_EXIT, 2, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_GUARDIAN_TO_ANEMOS_INNER_SANCTUM_ELEVATOR_NORTHWEST},
	{CUE_EXIT, 3, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_GUARDIAN_TO_ANEMOS_INNER_SANCTUM_SWITCHES_EAST},
	{CUE_EXIT, 4, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_GUARDIAN_TO_ANEMOS_INNER_SANCTUM_SWITCHES_NORTHEAST},
	{CUE_EXIT, 5, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_GUARDIAN_TO_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_EAST},
	{CUE_TABLE_END},
};

/* The north door's opening, as MAPGRID_ANIMATION_STEP rows: two steps of two */
/* ticks. */
Sint16 AnemosInnerSanctumNorthDoorFrames[] = {
	52, 7, 1, 2, 2, 54, 7, 1,
	2, 2, -1,
};

/* A mimic stone room's door opening, the same. */
Sint16 AnemosInnerSanctumMimicStoneDoorFrames[] = {
	18, 4, 1, 2, 2, 18, 7, 1,
	2, 2, -1,
};
/* clang-format on */

/* The three mimic stone rooms' doors, by their step cue's number. */
MAP_GRID_METATILE_ANIMATION AnemosInnerSanctumMimicStoneDoors[] = {
	{AnemosInnerSanctumMimicStoneDoorFrames, 11, 4},
	{AnemosInnerSanctumMimicStoneDoorFrames, 26, 4},
	{AnemosInnerSanctumMimicStoneDoorFrames, 53, 4},
};

Sint32 FieldEffectDurationScripts[] = {
	(Sint32)FieldEffectShortScript,
	(Sint32)FieldEffectMediumScript,
	(Sint32)FieldEffectLongScript,
};

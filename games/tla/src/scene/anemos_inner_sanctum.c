/* @file */
/* The Anemos Inner Sanctum, the hidden dungeon under Contigo, reached by */
/* Teleport from the circle in the middle of the town. */
/* */
/* The party arrives in a room with four element symbols on its floor. */
/* Stepping onto each, Felix lights a plate for every Djinni of its element */
/* the party has; with all eighteen of an element, its torch over the north */
/* door lights, and with all four lit the door opens */
/* (scAnemosInnerSanctumLightSymbol). Beyond it, Charon's slate speaks as the */
/* party comes up to it (scAnemosInnerSanctumApproachCharonSlate). Below, mimic
 */
/* stones copy Felix's steps, mirrored, until they reach a button that opens */
/* a door (scAnemosInnerSanctumMimicLeader); seven round switches make a */
/* lift float, ready to carry the party up */
/* (scAnemosInnerSanctumPressRoundSwitch); pillars are moved onto their */
/* places with Move, and a switch holds a door open while something stands */
/* on it; cracked floors give way under the party, and Sand lets Felix pass */
/* along stretches of sand. The last room, with its six symbol blocks and */
/* the Dullahan, is anemos_inner_sanctum_entrance.c. */
#include "scene/event_layer.h"
#include "field/field_sequence.h"
#include "field/event_inline.h"
#include "field/field_sequence_event.h"
#include "macros.h"
#include "game/game.h"
#include "field/call.h"
#include "scene/scene.h"
#include "field/actor_script.h"
#include "scene/mars_lighthouse_entrance.h"
#include "graphics/palette_buffers.h"

struct ACTOR;
struct CELL_TARGET;
struct CELL_TARGET_ENTRY;

/* A spark sent spiralling in to a Djinni's plate, as an actor's motion fields
 */
/* hold it: the plate it closes on (originX, originZ), the radius, angle and */
/* spin of its spiral, and its height, which drops by `fall` each step. */
typedef struct SPARK {
	const s32 *script;
	s16 scriptCursor;
	u16 facing;
	s32 x;
	s32 y;
	s32 z;
	s32 fall;
	s32 scaleX, scaleY;
	u16 collisionRadius;
	u8 layer;
	u8 priFlg;
	s32 velX, velY, velZ;
	s32 step;
	s32 height;
	s32 tgtX, tgtY, tgtZ;
	s32 originX;
	s32 originZ;
	s32 radius;
	void *spr;
	u8 sprStorageKind;
	u8 motionFlg;
	u8 arrivalAxis, conditionResult, snapToTgt, collisionFlg;
	u8 steeringFlg, paused, retainSprCache, repeatCnt;
	s16 wait;
	u8 refused, frozen;
	u8 pal; /* the palette of its sparks */
	u8 sceneByte;
	s16 ang;
	s16 spin;
} SPARK;

u16 PaletteCycleBright BSS_DATA;
u16 *PaletteCycleSlots BSS_DATA;
u16 PaletteCycleTimer BSS_DATA;
u16 PaletteCycleDelay BSS_DATA ALIGNED(4);
u16 *PaletteSlots BSS_DATA;
u16 *PaletteColors BSS_DATA;
u16 *PaletteCycleColors BSS_DATA;
u16 PaletteCycleLength BSS_DATA;
u16 PaletteCycleIndex BSS_DATA ALIGNED(4);

void scTeleportArriveWhirling(void);

void scTeleportStopShimmer(void);

void fdTeleportPlaySparks(void);

void fdLeaderCenterInCell(void);

CELL_TARGET CrackedTiles BSS_DATA;

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

extern u32 AnemosInnerSanctum_ShortScript[];
extern u32 AnemosInnerSanctum_MediumScript[];
extern u32 AnemosInnerSanctum_LongScript[];
extern u32 FieldEffect_LongScript[];
extern s32 AnemosInnerSanctum_NoShadowScript[];
extern s16 AnemosInnerSanctum_MimicStoneDoorFrames[];
extern s32 AnemosInnerSanctum_OrbScript[];

/* The actors of the rooms Move and the lifts work in, numbering each room's */
/* placement table: stone pillars (SPRITE_STONE_PILLAR) Moved onto their */
/* places, and the lifts (SPRITE_ARROW_PANEL) the round switches ready. */
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
	 */
	/* taken away as the switch is pressed (scAnemosInnerSanctumPressSwitch),
	   put */
	/* back as the leader steps off it (scAnemosInnerSanctumStepOffSwitch). */
	ACTOR_SWITCHES_SWITCH_MARKER
} ANEMOS_INNER_SANCTUM_SWITCHES_ACTOR_ID;

/* The Orihalcon chest's room. */
typedef enum ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_ACTOR_ID {
	ACTOR_ORIHALCON_CHEST_LIFT = ACTOR_FIRST_PLACED,
	ACTOR_ORIHALCON_CHEST_FIRST_PILLAR,
	ACTOR_ORIHALCON_CHEST_SECOND_PILLAR
} ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_ACTOR_ID;

/* The guardian room's three mimic stones (SPRITE_MIMIC_STONE), one in each */
/* room off its north side: entered from the south by entrance 1, 3 or 5, that
 */
/* room's stone mirrors the leader's steps (scAnemosInnerSanctumMimicLeader) */
/* until it reaches its button or falls into a hole. */
#define ACTOR_MIMIC_STONE_A ACTOR_FIRST_PLACED
#define ACTOR_MIMIC_STONE_B (ACTOR_FIRST_PLACED + 1)
#define ACTOR_MIMIC_STONE_C (ACTOR_FIRST_PLACED + 2)

/* The guardian room's ways in from the south, one below each mimic stone. */
enum {
	ENTRANCE_BY_MIMIC_STONE_A = 1,
	ENTRANCE_BY_MIMIC_STONE_B = 3,
	ENTRANCE_BY_MIMIC_STONE_C = 5
};

/* Set as each mimic stone reaches its button and opens its room's north door */
/* (scAnemosInnerSanctumOpenMimicStoneDoor), so that the set-up leaves it */
/* there. */
#define FLAG_MIMIC_STONE_A_ON_BUTTON FLAG_AREA_TEMP_01
#define FLAG_MIMIC_STONE_B_ON_BUTTON FLAG_AREA_TEMP_02
#define FLAG_MIMIC_STONE_C_ON_BUTTON FLAG_AREA_TEMP_03

/* Set while the leader stands on one of the Dragon Skin room's element */
/* symbols, so that it lights its plates once a visit to it. */
#define FLAG_ON_SYMBOL FLAG_SCENE_TEMP_00
/* Set while the switches room's switch is held down by the leader. */
#define FLAG_SWITCH_PRESSED FLAG_SCENE_TEMP_00
/* Set while the guardian room's mimic stone follows the leader or rests on */
/* its button, so that stepping onto its floor starts it no more. */
#define FLAG_MIMIC_STONE_STARTED FLAG_SCENE_TEMP_00

/* The step triggers of the Dragon Skin room: its four element symbols, whose */
/* plates light for the Djinn of the element the party has */
/* (scAnemosInnerSanctumLightSymbol), the floor between them, and the Teleport
 */
/* circle in its middle. */
typedef enum ANEMOS_INNER_SANCTUM_SYMBOL_TRIGGER {
	TRIGGER_TELEPORT_CIRCLE = 30,
	TRIGGER_VENUS_SYMBOL = 50,
	TRIGGER_MERCURY_SYMBOL,
	TRIGGER_MARS_SYMBOL,
	TRIGGER_JUPITER_SYMBOL,
	TRIGGER_FLOOR_BETWEEN_SYMBOLS
} ANEMOS_INNER_SANCTUM_SYMBOL_TRIGGER;

/* The first of the seven round switches' step triggers */
/* (scAnemosInnerSanctumPressRoundSwitch). */
enum { TRIGGER_FIRST_ROUND_SWITCH = 50, ROUND_SWITCHES = 7 };
/* Set as each round switch is pressed, lighting it; the macro takes the */
/* switch's place from the first. */
#define FLAG_ROUND_SWITCH_PRESSED(n) (FLAG_SCENE_TEMP_00 + (n))
/* Set once all seven round switches are pressed: the room's lift floats, */
/* ready to carry the party. */
#define FLAG_LIFT_READY FLAG_SCENE_TEMP_07
/* The ready lift's step trigger, in the elevator and Orihalcon chest rooms. */
enum { TRIGGER_LIFT = 11 };
/* The switches room's step triggers: the switch, and the cell east of it */
/* that a leader stepping off it crosses. */
enum { TRIGGER_SWITCH = 50, TRIGGER_EAST_OF_SWITCH };
/* The step trigger before Charon's slate. */
enum { TRIGGER_BEFORE_CHARON_SLATE = 30 };

/* The sanctum's lasting flags: each pillar Moved onto its place, each */
/* element's torch lit, the Dragon Skin room's north door opened by all four, */
/* and Charon's slate's words heard. */
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

/* The guardian room's lift (SPRITE_ARROW_PANEL), which brings the party up */
/* from the Orihalcon chest room (scAnemosInnerSanctumArriveOnGuardianLift). */
#define ACTOR_GUARDIAN_LIFT (ACTOR_FIRST_PLACED + 3)

/* The first column of the middle layer's cells in these rooms' maps (their */
/* map_layer 128, 0): the Dragon Skin room draws its plates, torches' light and
 */
/* pillar of light there. */
#define MIDDLE_LAYER_COLUMN (MAPGRID_SIZE / 2)
/* The first row of the overhead layer's cells in these rooms' maps (their */
/* map_layer 0, 128). */
#define OVERHEAD_LAYER_ROW (MAPGRID_SIZE / 2)
/* How far below a plate or the pillar of light its lit picture is kept, in */
/* cells. */
#define LIT_PICTURE_ROWS 26

/* The four torches over the Dragon Skin room's north door, on the wall's row */
/* z 152: each element's x and the height its flame burns at. */
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

/* The ways between the rooms each of these maps holds, named by the room a */
/* way leaves and the room it leads to, each room by where it lies on its map;
 */
/* a way's exit and the entrance at its foot share its number, so the way back
 */
/* is the entrance the exit leads to. */
/* */
/* The elevator's: the north-west and south-west halls, the room above the */
/* lift, the round switches' room below it, and the east and south-east rooms.
 */
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
/* The round switches' room's: its north, north-east, west, middle and south */
/* rooms, the north-east one with two ways down, west and east. */
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
/* The Orihalcon chest room's: its north-east and south-east rooms, and the */
/* second cracked floor's room west of them. */
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
/* Set when the elevator room's lift has carried the party up; it stays above.
 */
#define FLAG_ELEVATOR_LIFT_RISEN FLAG_AREA_TEMP_00
/* Set when the Orihalcon chest room's lift has carried the party up to the */
/* guardian room; it stays there, and the round switches below stay pressed. */
#define FLAG_GUARDIAN_LIFT_RISEN FLAG_AREA_TEMP_04

/* The Orihalcon chest room's two cracked floors: a cell's step trigger is its
 */
/* floor's when whole, the next when cracked. */
enum {
	TRIGGER_FIRST_CRACKED_FLOOR = 33,
	TRIGGER_SECOND_CRACKED_FLOOR = TRIGGER_FIRST_CRACKED_FLOOR + 2
};
/* The Orihalcon chest room's ways down through its cracked floors, each an */
/* exit and the entrance below of one number. */
enum { THROUGH_FIRST_CRACKED_FLOOR = 11, THROUGH_SECOND_CRACKED_FLOOR = 12 };

/* The row the mimic stones start on: one follows the leader's steps only */
/* while he is north of it, and never goes south of it. */
#define MIMIC_STONE_LINE_Z 248
/* The guardian room's step triggers by the mimic stones: the floor where the */
/* leader's steps start one following, and the cell behaviors a following */
/* stone stops on, a hole it falls into and the button that opens its door. */
typedef enum ANEMOS_INNER_SANCTUM_MIMIC_STONE_TRIGGER {
	TRIGGER_MIMIC_STONE_START = 50,
	TRIGGER_MIMIC_STONE_HOLE,
	TRIGGER_MIMIC_STONE_BUTTON,
	/* Raised when the stone falls into a hole */
	/* (scAnemosInnerSanctumDropMimicStone). */
	TRIGGER_MIMIC_STONE_IN_HOLE = 200,
	/* Raised when the stone reaches the button */
	/* (scAnemosInnerSanctumOpenMimicStoneDoor). */
	TRIGGER_MIMIC_STONE_ON_BUTTON
} ANEMOS_INNER_SANCTUM_MIMIC_STONE_TRIGGER;

#include "shared/teleport_shimmer.c"

#include "../field/actor_set_sprite_priority.c"

#include "../field/actor_create_silhouette.c"

#include "../field/field_effect_move.c"

extern s32 AnemosInnerSanctum_DurationScripts[];

/* Spawns a field effect at x, y, z moving at the velocity given, its type, */
/* scale, palette, priority, script, rotation, spin and update chosen by the */
/* EFFECT_USE_ flags from the options. */
/* */
/* This overlay's own copy of ../field/field_effect_spawn.c, which it does not
 */
/* include: this one takes actor 0's sprite priority rather than the leader's,
 */
/* picks from the overlay's own duration scripts, and sets a requested */
/* priority on the sprite while creating the actor through the import. */
void scFieldEffectSpawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
						s32 velocityZ, u32 flags,
						const EFFECT_OPTIONS *options) {
	FIELD_EFFECT *party = ((FIELD_EFFECT *)fdActorGetById(0));
	FIELD_EFFECT *effect;
	FIELD_SPR *sprite;
	const s32 *script;
	u32 start;

	if((flags & EFFECT_USE_TYPE) != 0 && options != 0)
		effect = (FIELD_EFFECT *)syImportActorCreate(options->type, x, y, z);
	else
		effect =
			(FIELD_EFFECT *)syImportActorCreate(SPRITE_DUST_CLOUD, x, y, z);
	if(effect == 0)
		return;

	sprite = effect->spr;
	fdActorSelectAnimation(effect, (flags + 1) & EFFECT_SCRIPT_MASK);
	fdActorStartScript(
		effect, AnemosInnerSanctum_DurationScripts[flags & EFFECT_SCRIPT_MASK]);
	effect->motionFlg = 0;
	fdActorSetSprFlags(effect, 0);
	effect->update = fdEffectUpdateMotionScaleAndSpin;
	effect->vel.x = velocityX;
	effect->vel.y = velocityY;
	effect->vel.z = velocityZ;
	scSetActorSprPriority(effect, party->spr->pri);
	effect->scaleRateX = 0;
	effect->scaleRateY = 0;
	effect->spin = 0;

	if((flags & EFFECT_OPTION_MASK) == 0 || options == 0)
		return;
	if((flags & EFFECT_USE_PALETTE) != 0)
		scFieldActorSetSprCol(effect, options->pal);
	if((flags & EFFECT_USE_PRIORITY) != 0) {
		effect->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;
		sprite->pri = options->pri;
		scSetActorSprPriority(effect, options->pri);
	}
	start = flags & EFFECT_USE_START_SCALE;
	if(start != 0) {
		effect->scaleX = options->startScaleX;
		effect->scaleY = options->startScaleY;
	}
	if((flags & EFFECT_SCALE_TO_TARGET) != 0) {
		FIXED targetX;
		s32 count;

		script = AnemosInnerSanctum_DurationScripts[flags & EFFECT_SCRIPT_MASK];
		targetX = options->tgtScaleX;
		count = script[EFFECT_SCRIPT_REPEAT_COUNT];
		if(start != 0) {
			effect->scaleRateX = (targetX - effect->scaleX) / count;
			effect->scaleRateY = (options->tgtScaleY - effect->scaleY) /
								 script[EFFECT_SCRIPT_REPEAT_COUNT];
		}
		else {
			effect->scaleRateX = (targetX - FIXED_ONE) / count;
			effect->scaleRateY = (options->tgtScaleY - FIXED_ONE) /
								 script[EFFECT_SCRIPT_REPEAT_COUNT];
		}
	}
	if((flags & EFFECT_USE_SCRIPT) != 0) {
		fdActorSelectAnimation(effect, 1);
		fdActorStartScript(effect, options->script);
	}
	if((flags & EFFECT_USE_ROTATION) != 0)
		sprite->rotation = options->rotation;
	if((flags & EFFECT_USE_SPIN) != 0)
		effect->spin = options->spin;
	if((flags & EFFECT_USE_UPDATE) != 0)
		effect->update = options->update;
}

/* Clears the actor's sprite flags, as a script's call; always 0. */
s32 scAnemosInnerSanctumClearScriptedActorSprFlags(s32 actorId) {
	fdActorSetSprFlags(actorId, 0);
	return 0;
}

/* A spark ring's orb: it circles its centre at its own speed while the */
/* radius shrinks a pixel a frame; 0 once it reaches the centre. */
s32 scAnemosInnerSanctumCircleOrb(ACTOR *actor) {
	FIXED radius;

	actor->tmr += actor->userValue;
	SET_ACTOR_POSITION(actor, actor->bounce, actor->gravity,
					   toFIXED(TORCH_ROW_Z));
	syFieldVectorAddPolarOffsetFX(actor->sceneWord, actor->tmr,
								  &actor->x.fixed);
	radius = actor->sceneWord - toFIXED(1);
	actor->sceneWord = radius;
	if(radius)
		return 1;
	return 0;
}

extern u32 AnemosInnerSanctum_FadingSparkScript[];

/* Leaves a fading spark of the source's palette where the source is. */
void scAnemosInnerSanctumLeaveSpark(ACTOR *source) {
	ACTOR *actor = (scFieldActorCreate(SPRITE_PARTICLE, source->x.fixed,
									   source->y.fixed, source->z.fixed));

	actor->motionFlg = 0;
	scFieldActorSetSprCol(actor, source->sceneCnt);
	scSetActorSprPriority(actor, 2);
	fdActorSelectAnimation(actor, 1);
	fdActorSetSprFlags(actor, 0);
	fdActorStartScript(actor, AnemosInnerSanctum_FadingSparkScript);
	fdActorSetBlendMode(actor, 1);
}

/* A spark's update: two steps a frame along its spiral, closing in and */
/* falling, leaving a fading spark at each; 0 once it reaches its plate. */
s32 scAnemosInnerSanctumSpiralIn(SPARK *spark) {
	if(spark->radius < spark->step)
		return 0;
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
	return 1;
}

/* A spark reaches its plate: a puff of sparks of its palette, and the plate */
/* shows its lit picture. */
s32 scAnemosInnerSanctumLightPlate(ACTOR *actor) {
	EFFECT_OPTIONS options;
	s32 x;
	s32 y;

	options.pri = 2;
	options.pal = actor->sceneCnt;
	scFieldEffectSpawn(actor->x.fixed, actor->y.fixed, actor->z.fixed, 0,
					   toFIXED(1), 0, EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
					   &options);
	x = FIELD_CELL(actor->bounce) + MIDDLE_LAYER_COLUMN;
	y = FIELD_CELL(actor->gravity);
	fdMapGridCopyCellsTo(x, y + LIT_PICTURE_ROWS, x, y, 1, 1);
	return 0;
}

#include "../field/actor_wait_until_landed.c"

const SCENE_ENTRANCE *scGetEntrances(void) { return SceneEntrances; }

const SCENE_REGION *scGetExitRegions(void) { return 0; }

const u32 *scGetExits(void) { return SceneExits; }

extern ACTOR_PLACE AnemosInnerSanctum_Placements[];
extern ACTOR_PLACE AnemosInnerSanctum_DragonSkinChestPlacements[];
extern ACTOR_PLACE AnemosInnerSanctum_ElevatorPlacements[];
extern ACTOR_PLACE AnemosInnerSanctum_SwitchesPlacements[];
extern ACTOR_PLACE AnemosInnerSanctum_OrihalconChestPlacements[];
extern ACTOR_PLACE AnemosInnerSanctum_GuardianPlacements[];

const ACTOR_PLACE *scGetActorPlacements(void) {
	if(MainRoutineStateWork.next ==
	   SCENE_ANEMOS_INNER_SANCTUM_DRAGON_SKIN_CHEST)
		return AnemosInnerSanctum_DragonSkinChestPlacements;
	if(MainRoutineStateWork.next == SCENE_ANEMOS_INNER_SANCTUM_ELEVATOR)
		return AnemosInnerSanctum_ElevatorPlacements;
	if(MainRoutineStateWork.next == SCENE_ANEMOS_INNER_SANCTUM_SWITCHES)
		return AnemosInnerSanctum_SwitchesPlacements;
	if(MainRoutineStateWork.next == SCENE_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST)
		return AnemosInnerSanctum_OrihalconChestPlacements;
	if(MainRoutineStateWork.next == SCENE_ANEMOS_INNER_SANCTUM_GUARDIAN)
		return AnemosInnerSanctum_GuardianPlacements;
	return AnemosInnerSanctum_Placements;
}

extern s32 AnemosInnerSanctum_SparkScript[];

/* Sends a spark of the given palette spiralling from one point to another. */
void scAnemosInnerSanctumSendSpark(from, to, palette) VECTOR3 *from;
VECTOR3 *to;
s16 palette;
{
	SPARK *spark;
	s32 x, z, distance;

	spark = ((SPARK *)scFieldActorCreate(SPRITE_PARTICLE, from->x, from->y,
										 from->z));
	x = FIXEDtoINT(from->x - to->x);
	z = FIXEDtoINT(from->z - to->z);
	spark->ang = syAtan(from->z - to->z, from->x - to->x);
	spark->spin = (((syRandom() & 2) - 1) * (syRandom() & 15)) << 8;
	spark->pal = palette;
	spark->motionFlg = 0;
	spark->originX = to->x;
	spark->originZ = to->z;
	distance = INTtoFIXED(sySquart(x * x + z * z));
	spark->radius = distance;
	spark->step = distance / 16;
	spark->height = from->y;
	spark->fall = (from->y - to->y) / 16;
	fdActorSetBlendMode(spark, 1);
	scFieldActorSetSprCol(spark, (u16)palette);
	scSetActorSprPriority(spark, 2);
	fdActorSelectAnimation(spark, 7);
	fdActorSetSprFlags(spark, 0);
	syFieldVectorAddPolarOffsetFX(spark->radius, spark->ang, &spark->x);
	fdActorStartScript(spark, AnemosInnerSanctum_SparkScript);
	scAnemosInnerSanctumSpiralIn(spark);
}

/* Rings a torch at x, y (16.16) with eight orbs of the given palette that */
/* circle in on it at the given speed. Each orb keeps its circle in its actor's
 */
/* fields: its centre in bounce (x) and gravity (y), its radius in sceneWord, */
/* its angle in timer and its speed in userValue. */
void scAnemosInnerSanctumSparkRing(x, y, palette, speed) s32 x;
s32 y;
s16 palette;
s16 speed;
{
	s32 i;

	for(i = 0; i != SPARK_RING_STEPS;) {
		ACTOR *orb =
			(scFieldActorCreate(SPRITE_PARTICLE, x, y, toFIXED(TORCH_ROW_Z)));
		s32 angle = i * (TRIG_TURN / SPARK_RING_STEPS);

		syFieldVectorAddPolarOffset(32, angle, &orb->x.fixed);
		orb->tmr = angle;
		orb->userValue = speed;
		orb->sceneCnt = i;
		orb->sceneByte = palette;
		orb->bounce = x;
		orb->sceneWord = toFIXED(32);
		orb->gravity = y;
		orb->motionFlg = 0;
		fdActorSetBlendMode(orb, 1);
		scFieldActorSetSprCol(orb, (u16)palette);
		scSetActorSprPriority(orb, 2);
		fdActorSelectAnimation(orb, 7);
		fdActorSetSprFlags(orb, 0);
		fdActorStartScript(orb, AnemosInnerSanctum_OrbScript);
		i += 2;
	}
}

/* Leaves a flame of the given palette burning on the torch at x and height y */
/* on the torches' row. */
void scAnemosInnerSanctumLightTorch(x, y, palette) s32 x;
s32 y;
s16 palette;
{
	ACTOR *flame;

	flame = scFieldActorCreate(SPRITE_PARTICLE, INTtoFIXED(x),
							   INTtoFIXED(y + 3), toFIXED(TORCH_ROW_Z));
	flame->motionFlg = ACTOR_FLOATS;
	scFieldActorSetSprCol(flame, (u16)palette);
	scSetActorSprPriority(flame, 2);
	fdActorSelectAnimation(flame, 7);
	fdActorSetSprFlags(flame, 0);
}

extern u32 AnemosInnerSanctum_DjinniRisesScript[];
extern s32 AnemosInnerSanctum_DjinniSinksScript[];
extern s32 AnemosInnerSanctum_PlateOffsets[][2];

/* Stepping onto an element's symbol in the Dragon Skin room: the leader */
/* raises his arms in a pillar of light and a Djinni of the element rises over
 */
/* him, and each of the element's Djinn the party has found sends a spark to */
/* its own plate. Short of all eighteen, the plates and the light go out */
/* again; with all eighteen, the element's torch over the north door is lit, */
/* and with all four lit the door opens. Nothing if the torch is lit already */
/* or the party has none of the element's Djinn. */
void scAnemosInnerSanctumLightSymbol(void) {
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	u16 alpha;
	s32 x, z;
	s32 targetX;
	s32 targetY;
	s16 type;
	s16 trigger;
	s32 index;
	s32 base;
	u8 count;
	s32 palette;
	s32 column, row, cellX, cellZ;
	s32 i;
	u8 last;
	ACTOR *djinni;

	syIoRead16(&REG_BLDALPHA, &alpha);
	targetX = 0;
	targetY = 0;
	type = state->stepTrigger - TRIGGER_VENUS_SYMBOL + SPRITE_VENUS_DJINNI;
	trigger = state->stepTrigger;
	index = trigger - TRIGGER_VENUS_SYMBOL;
	x = ((index & 1) << 8) + 408;
	base = 0;
	count = 0;
	palette = SPRITE_COLOR_NORMAL;
	z = (index & 2) * 3 * 32 + 168;
	switch(trigger) {
	case TRIGGER_VENUS_SYMBOL:
		base = FLAG_DJINNI(DJINNI_FLINT);
		targetX = VENUS_TORCH_X;
		targetY = VENUS_TORCH_Y;
		if(syGameFlagGet(FLAG_ANEMOS_VENUS_TORCH_LIT)) {
			syGameFlagSet(FLAG_ON_SYMBOL);
			return;
		}
		break;
	case TRIGGER_MERCURY_SYMBOL:
		palette = SPRITE_COLOR_BGR;
		base = FLAG_DJINNI(DJINNI_FIZZ);
		targetX = MERCURY_TORCH_X;
		targetY = MERCURY_TORCH_Y;
		if(syGameFlagGet(FLAG_ANEMOS_MERCURY_TORCH_LIT)) {
			syGameFlagSet(FLAG_ON_SYMBOL);
			return;
		}
		break;
	case TRIGGER_MARS_SYMBOL:
		palette = SPRITE_COLOR_RED_TINT;
		base = FLAG_DJINNI(DJINNI_FORGE);
		targetX = MARS_TORCH_X;
		targetY = MARS_TORCH_Y;
		if(syGameFlagGet(FLAG_ANEMOS_MARS_TORCH_LIT)) {
			syGameFlagSet(FLAG_ON_SYMBOL);
			return;
		}
		break;
	case TRIGGER_JUPITER_SYMBOL:
		palette = SPRITE_COLOR_RBG;
		base = FLAG_DJINNI(DJINNI_GUST);
		targetX = JUPITER_TORCH_X;
		targetY = JUPITER_TORCH_Y;
		if(syGameFlagGet(FLAG_ANEMOS_JUPITER_TORCH_LIT)) {
			syGameFlagSet(FLAG_ON_SYMBOL);
			return;
		}
		break;
	}

	{
		u8 found[20];
		u8 order[20];
		VECTOR3 from;
		VECTOR3 to;

		for(i = (u16)base; i != (u16)base + DJINNI_SPARE_FIRST; i++) {
			if(syGameFlagGet(i)) {
				found[count] = i - base;
				count++;
			}
		}
		if(count == 0)
			return;

		fdEventStart();
		syFieldEventEndLastingPsynergy(0);
		fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.8);
		fdCameraSetSpeed(0.8);
		fdActorWalkTo(MainRoutineStateWork.leaderActorId, x, z);
		fdCameraMoveToFX(INTtoFIXED(x), -1, INTtoFIXED(z), 1);
		scFieldActorWaitForMoveById(MainRoutineStateWork.leaderActorId);
		fdActorFaceDirectionAndWait(MainRoutineStateWork.leaderActorId,
									FACING_SOUTH);
		fdActorSetAnimation(MainRoutineStateWork.leaderActorId,
							ANIM_LEADER_ARMS_RAISED);
		djinni =
			(scFieldActorCreate((u16)type, leader->x.fixed,
								leader->y.fixed + toFIXED(8), leader->z.fixed));
		djinni->motionFlg = 0;
		djinni->scaleY = FIXED_ONE / 10;
		djinni->scaleX = FIXED_ONE / 10;
		fdActorSetSprFlags((s32)djinni, 0);
		fdActorStartScript(djinni, AnemosInnerSanctum_DjinniRisesScript);
		fdActorSetBlendMode(djinni, 1);
		fdPaletteFadeApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
		fdPaletteFadeApplyTarget(PALETTE_TINT_WARM, PALETTES_BG);
		fdPaletteFadeInterpolate(60);
		fdBlendScriptSuspend();
		alpha = alpha & BLEND_ALPHA_TOP_MASK;
		REG_BLDALPHA = BLEND_ALPHA(0, BLEND_WEIGHT_MAX);
		cellX = x >> 4;
		cellZ = z >> 4;
		column = cellX;
		row = cellZ;
		fdMapGridCopyCellsTo(MIDDLE_LAYER_COLUMN + cellX - 1,
							 cellZ - 3 + LIT_PICTURE_ROWS,
							 MIDDLE_LAYER_COLUMN + cellX - 1, cellZ - 3, 3, 5);
		for(i = 0; i != alpha; i++) {
			REG_BLDALPHA = BLEND_ALPHA(i, BLEND_WEIGHT_MAX);
			syWait(8);
		}
		fdBlendScriptResume();
		fdEventPlaySnd(SE_GLITTER);
		syFieldEventWait(48);

		last = count - 1;
		for(i = 0; i != count; i++) {
			s32 pick = syRandom();
			pick &= last;

			order[i] = found[pick];
			found[pick] = found[last];
			last--;
		}
		for(i = 0; i != count;) {
			s32 slot;

			from.x =
				leader->x.fixed + INTtoFIXED(syRandom() & 31) - toFIXED(16);
			from.y = leader->y.fixed + INTtoFIXED(syRandom() & 15) + toFIXED(8);
			from.z = leader->z.fixed + INTtoFIXED(syRandom() & 15) - toFIXED(8);
			slot = order[i];
			to.x = leader->x.fixed +
				   INTtoFIXED(AnemosInnerSanctum_PlateOffsets[slot][0]);
			to.y = leader->y.fixed;
			to.z = leader->z.fixed +
				   INTtoFIXED(AnemosInnerSanctum_PlateOffsets[slot][1]);
			scAnemosInnerSanctumSendSpark(&from, &to, palette);
			if(i & 1)
				fdEventPlaySnd(SE_SPARKS);
			i++;
			syFieldEventWait(8);
		}

		if(count != DJINNI_SPARE_FIRST) {
			syFieldEventWait(96);
			fdActorStartScript(djinni, AnemosInnerSanctum_DjinniSinksScript);
			fdBlendScriptSuspend();
			alpha = REG_BLDALPHA & BLEND_ALPHA_TOP_MASK;
			fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
			fdPaletteFadeInterpolate(60);
			for(i = alpha; i != 0;) {
				REG_BLDALPHA = BLEND_ALPHA(i, BLEND_WEIGHT_MAX);
				i--;
				syWait(8);
			}
			for(i = 0; i != count;) {
				s32 slot = order[i];

				i++;
				fdMapGridCopyCellsTo(
					MIDDLE_LAYER_COLUMN, 0,
					FIELD_CELL(
						leader->x.fixed +
						INTtoFIXED(AnemosInnerSanctum_PlateOffsets[slot][0])) +
						MIDDLE_LAYER_COLUMN,
					FIELD_CELL(
						leader->z.fixed +
						INTtoFIXED(AnemosInnerSanctum_PlateOffsets[slot][1])),
					1, 1);
			}
			fdMapGridCopyCellsTo(MIDDLE_LAYER_COLUMN, 0,
								 MIDDLE_LAYER_COLUMN + column - 1, row - 3, 3,
								 5);
			fdBlendScriptResume();
			fdEventWaitTransitionFrames();
		}
		else {
			fdEventSetFlag((s16)state->stepTrigger - TRIGGER_VENUS_SYMBOL +
						   FLAG_ANEMOS_VENUS_TORCH_LIT);
			fdActorStartScript(djinni, AnemosInnerSanctum_DjinniSinksScript);
			syFieldEventWait(60);
			fdCameraSetSpeed(4);
			fdCameraMoveTo(536, CAMERA_KEEP, 120, 1);
			fdCameraWaitForArrival();
			fdEventPlaySnd(SE_SHIMMER);
			{
				FIXED ringX = INTtoFIXED(targetX);
				FIXED ringY = INTtoFIXED(targetY);

				scAnemosInnerSanctumSparkRing(ringX, ringY, palette,
											  TRIG_TURN * 8 / 256);
				syFieldEventWait(10);
				scAnemosInnerSanctumSparkRing(ringX, ringY, palette,
											  TRIG_TURN * 9 / 256);
				syFieldEventWait(10);
				scAnemosInnerSanctumSparkRing(ringX, ringY, palette,
											  TRIG_TURN * 10 / 256);
			}
			syFieldEventWait(12);
			syWait(16);
			fdPaletteFadeApplyTarget(RGB_WHITE, PALETTES_ALL);
			fdPaletteFadeInterpolate(1);
			syWait(1);
			fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
			fdPaletteFadeInterpolate(32);
			fdEventPlaySnd(SE_FLASH);
			scAnemosInnerSanctumLightTorch(targetX, targetY, palette);
			syFieldEventWait(80);
			if(syGameFlagGet(FLAG_ANEMOS_VENUS_TORCH_LIT) &&
			   syGameFlagGet(FLAG_ANEMOS_MERCURY_TORCH_LIT) &&
			   syGameFlagGet(FLAG_ANEMOS_MARS_TORCH_LIT) &&
			   syGameFlagGet(FLAG_ANEMOS_JUPITER_TORCH_LIT)) {
				/* The north door opens. */
				fdEventSetFlag(FLAG_ANEMOS_NORTH_DOOR_OPEN);
				fdEventPlaySnd(SE_THUMP);
				fdMapGridCopyCellsTo(52, 7, 33, 7, 1, 2);
				syWait(5);
				fdMapGridCopyCellsTo(54, 7, 33, 7, 1, 2);
				fdMapGridSetCellHeight(toFIXED(536), toFIXED(136), 0, 0);
				syWait(60);
			}
		}

		fdActorRelease(djinni);
		fdEventSetFlag(FLAG_ON_SYMBOL);
		syEventCameraFollowActor(MainRoutineStateWork.leaderActorId, 1);
		fdCameraWaitForArrival();
		syFieldEventWait(60);
		fdEventEnd();
	}
}

/* Nothing visible happens as Felix steps off a symbol onto the floor */
/* between them; the game lets the symbols be stepped onto again. */
void scAnemosInnerSanctumStepOffSymbol(void) {
	syGameFlagClear(FLAG_ON_SYMBOL);
}

extern s16 AnemosInnerSanctum_ElevatorBlocks[];
extern s16 AnemosInnerSanctum_SwitchesBlocks[];
extern s16 AnemosInnerSanctum_OrihalconChestBlocks[];

/* Nothing visible of its own happens as Felix starts to move a pillar with */
/* Move; in the switches' and the Orihalcon chest's rooms, the game closes */
/* the stretch of sand by the pillar's place while it is moving, so that a */
/* mound of sand cannot pass under it, and opens a cell or two beside it. */
void scAnemosInnerSanctumStartMovingBlock(void) {
	if(MainRoutineStateWork.next == SCENE_ANEMOS_INNER_SANCTUM_ELEVATOR) {
		fdActorLiftTargetBlock((s32)AnemosInnerSanctum_ElevatorBlocks);
	}
	else {
		if(MainRoutineStateWork.next == SCENE_ANEMOS_INNER_SANCTUM_SWITCHES) {
			fdActorLiftTargetBlock((s32)AnemosInnerSanctum_SwitchesBlocks);
			fdSetCellBehaviorAt(MAPGRID_LAYER_OVERHEAD, toFIXED(72),
								toFIXED(440), CELL_BEHAVIOR_WALL);
		}
		else {
			if(MainRoutineStateWork.next ==
			   SCENE_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST) {
				fdActorLiftTargetBlock(
					(s32)AnemosInnerSanctum_OrihalconChestBlocks);
				fdSetCellBehaviorAt(MAPGRID_LAYER_OVERHEAD, toFIXED(280),
									toFIXED(344), CELL_BEHAVIOR_WALL);
				fdSetCellBehaviorAt(MAPGRID_LAYER_OVERHEAD, toFIXED(280),
									toFIXED(376), CELL_BEHAVIOR_WALL);
				fdSetCellBehaviorAt(MAPGRID_LAYER_OVERHEAD, toFIXED(296),
									toFIXED(360), CELL_BEHAVIOR_WALL);
				fdSetCellBehaviorAt(MAPGRID_LAYER_OVERHEAD, toFIXED(280),
									toFIXED(360), CELL_BEHAVIOR_SAND);
				fdSetCellBehaviorAt(MAPGRID_LAYER_OVERHEAD, toFIXED(520),
									toFIXED(632), CELL_BEHAVIOR_WALL);
				fdSetCellBehaviorAt(MAPGRID_LAYER_OVERHEAD, toFIXED(520),
									toFIXED(616), CELL_BEHAVIOR_SAND);
			}
		}
	}
}

/* Once Felix has moved a pillar with Move: a pillar left in the place it */
/* belongs stays there on later visits, as a shortcut. In the switches' room, */
/* the pillar moved onto the switch presses it with a click, and the door */
/* west of it opens with a thump, for good. */
/* */
/* The stretches of sand scAnemosInnerSanctumStartMovingBlock closed are */
/* opened again. Each place is checked in half cells. In the switches' room */
/* whichever pillar is set down on pillar B's place counts as pillar B, the */
/* game trading the two actors' places while the third pillar is not on its */
/* own. */
void scAnemosInnerSanctumFinishMovingBlock(void) {
	ACTOR *block = FieldStateWork->movedBlock;
	s32 x;
	s32 z;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdActorPlaceMovedBlock();
	x = FIELD_HALF_CELL(block->x.fixed);
	z = FIELD_HALF_CELL(block->z.fixed);
	if(MainRoutineStateWork.next == SCENE_ANEMOS_INNER_SANCTUM_SWITCHES) {
		fdSetCellBehaviorAt(MAPGRID_LAYER_OVERHEAD, toFIXED(72), toFIXED(440),
							CELL_BEHAVIOR_SAND);
		if(x != 8)
			fdSetCellBehaviorAt(MAPGRID_LAYER_OVERHEAD, toFIXED(72),
								toFIXED(424), CELL_BEHAVIOR_SAND);
	}
	if(MainRoutineStateWork.next ==
	   SCENE_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST) {
		fdSetCellBehaviorAt(MAPGRID_LAYER_OVERHEAD, toFIXED(280), toFIXED(344),
							CELL_BEHAVIOR_NORMAL);
		fdSetCellBehaviorAt(MAPGRID_LAYER_OVERHEAD, toFIXED(280), toFIXED(376),
							CELL_BEHAVIOR_SAND);
		fdSetCellBehaviorAt(MAPGRID_LAYER_OVERHEAD, toFIXED(296), toFIXED(360),
							CELL_BEHAVIOR_NORMAL);
		fdSetCellBehaviorAt(MAPGRID_LAYER_OVERHEAD, toFIXED(520), toFIXED(632),
							CELL_BEHAVIOR_SAND);
	}
	if(MainRoutineStateWork.next == SCENE_ANEMOS_INNER_SANCTUM_ELEVATOR) {
		if(x == 37 && z == 31) {
			syGameFlagSet(FLAG_ANEMOS_ELEVATOR_FIRST_PILLAR_PLACED);
			fdActorSetSprPriority(ACTOR_ELEVATOR_FIRST_PILLAR, 2);
		}
		if(x == 55 && z == 95)
			syGameFlagSet(FLAG_ANEMOS_ELEVATOR_SECOND_PILLAR_PLACED);
	}
	else if(MainRoutineStateWork.next == SCENE_ANEMOS_INNER_SANCTUM_SWITCHES) {
		if(x == 7 && z == 53)
			syGameFlagSet(FLAG_ANEMOS_SWITCHES_FIRST_PILLAR_PLACED);
		if(x == 67 && z == 45) {
			fdMapGridCopyCellsTo(50, 22, 33, 22, 1, 1);
			fdActorSetPos(ACTOR_SWITCHES_SWITCH_MARKER, 0, 0);
			fdEventPlaySnd(SE_BUTTON_PRESS);
			syFieldEventWait(30);
			fdEventPlaySnd(SE_THUMP);
			fdMapGridCopyCellsTo(50, 20, 31, 20, 1, 2);
			syFieldEventWait(5);
			fdMapGridCopyCellsTo(51, 20, 31, 20, 1, 2);
			syFieldEventWait(5);
			fdMapGridSetCellHeight(toFIXED(496), toFIXED(336), 0, 0);
			syGameFlagSet(FLAG_ANEMOS_SWITCHES_SECOND_PILLAR_PLACED);
		}
		if(x == 93 && z == 57)
			syGameFlagSet(FLAG_ANEMOS_SWITCHES_THIRD_PILLAR_PLACED);
		if(x == 91 && z == 59) {
			if(!fdEventGetFlag(FLAG_ANEMOS_SWITCHES_THIRD_PILLAR_PLACED)) {
				s32 fromX = INTtoFIXED(
					FIXEDtoINT(fdActorGet(ACTOR_SWITCHES_PILLAR_B)->x.fixed));

				fdActorSetPosFX(
					ACTOR_SWITCHES_THIRD_PILLAR, fromX,
					INTtoFIXED(FIXEDtoINT(
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
	fdEventEnd();
}

extern s32 AnemosInnerSanctum_SwitchGlowScript[];

/* A round switch stepped on: it lights under the leader in a burst of blue */
/* rings and its scene flag is set; once all seven are pressed, the room's */
/* lift floats up with its arrow pointing up (FLAG_LIFT_READY), ready to */
/* carry the party. */
void scAnemosInnerSanctumPressRoundSwitch(void) {
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	ACTOR *actor;
	s32 all;
	s32 i;
	s32 id;

	if(MainRoutineStateWork.next == SCENE_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST)
		fdMapGridCopyCellsTo(17, 71, FIELD_CELL(leader->x.fixed),
							 FIELD_CELL(leader->z.fixed) + OVERHEAD_LAYER_ROW,
							 1, 1);
	else
		fdMapGridCopyCellsTo(54, 87, FIELD_CELL(leader->x.fixed),
							 FIELD_CELL(leader->z.fixed) + OVERHEAD_LAYER_ROW,
							 1, 1);
	fdEventPlaySnd(SE_DRIP);
	actor = scFieldActorCreate(
		SPRITE_ENERGY_CHARGE,
		FIELD_CELL_ORIGIN(FIELD_CELL(leader->x.fixed)) + toFIXED(8), 0,
		FIELD_CELL_ORIGIN(FIELD_CELL(leader->z.fixed)) + toFIXED(8));
	actor->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
	actor->motionFlg = 0;
	scFieldActorSetSprCol(actor, SPRITE_COLOR_GBR);
	actor->scaleY = FIXED_ONE * 11 / 8;
	actor->scaleX = FIXED_ONE * 11 / 8;
	scSetActorSprPriority(actor, 2);
	fdActorSelectAnimation(actor, ENERGY_CHARGE_ANIMATION_BURST);
	fdActorSetSprFlags(actor, 0);
	fdActorStartScript(actor, AnemosInnerSanctum_SwitchGlowScript);
	fdEventSetFlag(FLAG_ROUND_SWITCH_PRESSED(state->stepTrigger -
											 TRIGGER_FIRST_ROUND_SWITCH));
	all = fdEventGetFlag(FLAG_ROUND_SWITCH_PRESSED(0));
	for(i = 1; i != ROUND_SWITCHES; i++)
		all &= fdEventGetFlag(FLAG_ROUND_SWITCH_PRESSED(i));
	if(all) {
		fdEventStart();
		syFieldEventEndLastingPsynergy(0);
		syFieldEventWait(10);
		fdCameraSetSpeed(0.8);
		id = ACTOR_ELEVATOR_LIFT;
		if(MainRoutineStateWork.next ==
		   SCENE_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST)
			id = ACTOR_ORIHALCON_CHEST_LIFT;
		fdCameraMoveToActorById(id, 1);
		fdCameraWaitForArrival();
		syFieldEventWait(10);
		fdEventPlaySnd(SE_LEMURIAN_SHIP_LIFT_FAST);
		fdActorSetAnimation(id, ARROW_PANEL_ANIMATION_UP);
		fdEventSetFlag(FLAG_LIFT_READY);
		fdActorGet(id)->motionFlg = ACTOR_FLOATS;
		syFieldEventWait(80);
		fdEventEnd();
	}
}

/* Felix steps onto the lift once it floats ready: he walks to its middle, */
/* turns to face south, and it carries the party up out of the room to the */
/* room above. */
void scAnemosInnerSanctumRideLiftUp(void) {
	ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	ACTOR *lift;

	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	if(MainRoutineStateWork.next ==
	   SCENE_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST) {
		lift = fdActorGet(ACTOR_ORIHALCON_CHEST_LIFT);
		syFieldEventSetExitRequest(
			ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_TO_ANEMOS_INNER_SANCTUM_GUARDIAN_SOUTHWEST);
	}
	else {
		lift = fdActorGet(ACTOR_ELEVATOR_LIFT);
		syFieldEventSetExitRequest(EXIT_UP_THE_LIFT);
	}
	{
		s32 x = (FIELD_CELL(leader->x.fixed) << 4) + 8;
		s32 z = (FIELD_CELL(leader->z.fixed) << 4) + 8;

		fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, x, z);
	}
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_SOUTH, 10);
	syFieldEventWait(30);
	lift->motionFlg = 0;
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	lift->speedLimit = toFIXED(2);
	lift->acceleration = toFIXED(1) / 32;
	fdEventPlaySnd(SE_KOLIMA_BRIDGE_POST);
	scFieldActorSetMoveTargetFX(lift, lift->x.fixed, lift->y.fixed + toFIXED(4),
								lift->z.fixed);
	scFieldActorWaitForMove(lift);
	scSetActorSprPriority(leader, 1);
	scSetActorSprPriority(lift, 1);
	scFieldActorSetMoveTargetFX(lift, lift->x.fixed,
								lift->y.fixed + toFIXED(128), lift->z.fixed);
	syFieldEventWait(80);
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	fdEventEnd();
}

/* Felix casts Sand standing on sand and sinks into it; as a mound of sand */
/* he can then travel along the stretch of sand, where on foot he cannot */
/* pass, until it wears off. */
/* */
/* While it lasts the leader moves on the overhead layer, where the sand's */
/* own cells are. */
void scAnemosInnerSanctumSinkIntoSand(void) {
	ACTOR *actor;
	s32 behavior;

	actor = fdActorGet(MainRoutineStateWork.leaderActorId);
	behavior = syFieldMapGridGetCellBehaviorAt(MAPGRID_LAYER_OVERHEAD,
											   actor->x.fixed, actor->z.fixed);
	if(behavior == CELL_BEHAVIOR_SAND) {
		actor->layer = MAPGRID_LAYER_OVERHEAD;
	}
}

/* Nothing visible of its own: as Sand wears off and Felix rises from the */
/* sand, the game sets him walking on the ground's cells again. */
void scAnemosInnerSanctumEndSand(void) {
	fdActorGet(MainRoutineStateWork.leaderActorId)->layer =
		MAPGRID_LAYER_GROUND;
}

/* Stepping onto the switch: the leader walks onto its middle, it sinks with a
 */
/* click and the door west of it opens with a thump. */
void scAnemosInnerSanctumPressSwitch(void) {
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 536, 360);
	syFieldEventWait(8);
	fdMapGridCopyCellsTo(50, 22, 33, 22, 1, 1);
	fdActorSetPos(ACTOR_SWITCHES_SWITCH_MARKER, 0, 0);
	fdEventPlaySnd(SE_BUTTON_PRESS);
	syFieldEventWait(30);
	fdEventPlaySnd(SE_THUMP);
	fdMapGridCopyCellsTo(50, 20, 31, 20, 1, 2);
	syFieldEventWait(5);
	fdMapGridCopyCellsTo(51, 20, 31, 20, 1, 2);
	syFieldEventWait(5);
	fdEventSetFlag(FLAG_SWITCH_PRESSED);
	fdEventEnd();
}

/* Stepping off the pressed switch to the east: it rises again with a click */
/* and its door shuts with a thump. */
void scAnemosInnerSanctumStepOffSwitch(void) {
	ACTOR *platform;

	platform = fdActorGet(ACTOR_SWITCHES_SWITCH_MARKER);
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	platform->x.fixed = toFIXED(536);
	platform->z.fixed = toFIXED(360);
	platform->groundHeight = -toFIXED(14);
	platform->y.fixed = -toFIXED(14);
	fdMapGridCopyCellsTo(51, 22, 33, 22, 1, 1);
	fdEventPlaySnd(SE_BUTTON_PRESS);
	syFieldEventWait(30);
	fdEventPlaySnd(SE_THUMP);
	fdMapGridCopyCellsTo(50, 20, 31, 20, 1, 2);
	syFieldEventWait(5);
	fdMapGridCopyCellsTo(52, 20, 31, 20, 1, 2);
	syFieldEventWait(5);
	fdEventClearFlag(FLAG_SWITCH_PRESSED);
	fdEventEnd();
}

s32 scCrackedTileStep();

/* Defines a step trigger on a cracked floor: when a tile gives way under the */
/* leader, the party leaves by exit `exit`, through the floor. */
#define DEFINE_ANEMOS_INNER_SANCTUM_EXIT_IF(name, exit)                        \
	void name(void) {                                                          \
		s32 fell;                                                              \
                                                                               \
		fell = scCrackedTileStep();                                            \
		if(fell != 0) {                                                        \
			syFieldEventSetExitRequest(exit);                                  \
		}                                                                      \
	}

/* Stepping onto the first cracked floor: once a tile gives way, the party */
/* falls through to the part of the room below. */
DEFINE_ANEMOS_INNER_SANCTUM_EXIT_IF(scAnemosInnerSanctumFallThroughFirstCracks,
									THROUGH_FIRST_CRACKED_FLOOR)

/* As scAnemosInnerSanctumFallThroughFirstCracks, for the second floor. */
DEFINE_ANEMOS_INNER_SANCTUM_EXIT_IF(scAnemosInnerSanctumFallThroughSecondCracks,
									THROUGH_SECOND_CRACKED_FLOOR)

extern s32 AnemosInnerSanctum_DustScript[];

/* A mimic stone's update: each step the leader takes, it takes too, mirrored */
/* east for west, scraping up dust, unless a wall stands in its way; on its */
/* button or over a hole it stops and raises that trigger. Once the leader */
/* goes back south of its row, it settles on its cell and may be started */
/* again. sceneWord and acceleration hold the leader's last x and z. */
void scAnemosInnerSanctumMimicLeader(ACTOR *self) {
	ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	FIXED next[2];
	FIXED ahead[3];
	ACTOR *dust;
	s32 behavior;

	if(FIXEDtoINT(fdActorGet(MainRoutineStateWork.leaderActorId)->z.fixed) >
	   MIMIC_STONE_LINE_Z) {
		self->update = 0;
		fdEventClearFlag(FLAG_MIMIC_STONE_STARTED);
		self->x.fixed = (self->x.fixed & FIELD_CELL_MASK) + toFIXED(8);
		self->z.fixed = (self->z.fixed & FIELD_CELL_MASK) + toFIXED(8);
		return;
	}
	ahead[0] = next[0] = self->x.fixed - (leader->x.fixed - self->sceneWord);
	ahead[1] = leader->y.fixed;
	ahead[2] = next[1] = self->z.fixed + (leader->z.fixed - self->acceleration);
	if(self->sceneWord != leader->x.fixed ||
	   self->acceleration != leader->z.fixed) {
		dust = (scFieldActorCreate(SPRITE_PARTICLE, self->x.fixed,
								   self->y.fixed, self->z.fixed));
		dust->x.fixed += INTtoFIXED((syRandom() & 15) - 8);
		dust->y.fixed += INTtoFIXED((syRandom() & 31) - 4);
		dust->motionFlg = 0;
		scFieldActorSetSprCol(dust, self->sceneCnt);
		scSetActorSprPriority(dust, 2);
		fdActorSelectAnimation(dust, 1);
		fdActorSetSprFlags(dust, 0);
		fdActorStartScript(dust, AnemosInnerSanctum_DustScript);
		self->sceneWord = leader->x.fixed;
		self->acceleration = leader->z.fixed;
		if((FrameCount & 3) == 0)
			fdEventPlaySnd(SE_DRIP);
	}
	if(FIXEDtoINT(ahead[2]) > MIMIC_STONE_LINE_Z)
		return;
	syFieldVectorAddPolarOffset(7, DEGtoANG(180) - leader->facing, ahead);
	if(syFieldMapGridGetCellBehaviorAt(self->layer, ahead[0], ahead[2]) ==
	   CELL_BEHAVIOR_WALL)
		return;
	{
		FIXED x = next[0];

		self->x.fixed = x;
		self->z.fixed = next[1];
		behavior = syFieldMapGridGetCellBehaviorAt(self->layer, x, next[1]);
	}
	if(behavior == TRIGGER_MIMIC_STONE_BUTTON) {
		FieldStateWork->raisedTrigger = TRIGGER_MIMIC_STONE_ON_BUTTON;
		self->update = 0;
	}
	else if(behavior == TRIGGER_MIMIC_STONE_HOLE) {
		FieldStateWork->raisedTrigger = TRIGGER_MIMIC_STONE_IN_HOLE;
		self->update = 0;
	}
}

#include "text/msg_ids.h"

void scTeleportStartFastShimmer(void);

/* Stepping onto the floor before a mimic stone: the stone of the room the */
/* party came into starts to mirror the leader's steps. */
void scAnemosInnerSanctumStartMimicStone(void) {
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
		fdEventSetFlag(FLAG_MIMIC_STONE_STARTED);
	}
}

/* A mimic stone falls into a hole: it slides to the hole's middle and drops in
 */
/* with a puff of dust, hidden below the hole's edge by a crate's silhouette */
/* drawn as an object window, and is taken away. */
void scAnemosInnerSanctumDropMimicStone(void) {
	ACTOR *stone;
	ACTOR *mask;
	FIELD_SPR *sprite;
	FIXED position[3];

	if(MainRoutineStateWork.entrance == ENTRANCE_BY_MIMIC_STONE_A)
		stone = fdActorGet(ACTOR_MIMIC_STONE_A);
	else if(MainRoutineStateWork.entrance == ENTRANCE_BY_MIMIC_STONE_B)
		stone = fdActorGet(ACTOR_MIMIC_STONE_B);
	else
		stone = fdActorGet(ACTOR_MIMIC_STONE_C);
	position[0] = (stone->x.fixed & FIELD_CELL_MASK) + toFIXED(8);
	position[2] = (stone->z.fixed & FIELD_CELL_MASK) + toFIXED(8);
	stone->acceleration = toFIXED(1);
	stone->speedLimit = toFIXED(1);
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	scFieldActorSetMoveTargetFX(stone, position[0], 0, position[2]);
	mask = fdActorCreateSilhouette(stone->x.fixed, 0, position[2] + toFIXED(59),
								   SPRITE_CRATE);
	mask->scaleX = FIXED_ONE * 3 / 2;
	mask->scaleY = FIXED_ONE * 3 / 2;
	sprite = mask->spr;
	sprite->objMode = OBJ_MODE_WINDOW;
	REG_WINOUT = WINOUT_WIN01_ALL | WINOUT_WINOBJ_BG0 | WINOUT_WINOBJ_BG1 |
				 WINOUT_WINOBJ_BG2 | WINOUT_WINOBJ_BG3 | WINOUT_WINOBJ_CLR;
	REG_DISPCNT |= DISPCNT_OBJWIN_ON;
	fdEventPlaySnd(SE_DUST_PUFF);
	stone->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	fdActorWaitUntilLanded(stone);
	stone->x.fixed = 0;
	stone->y.fixed = 0;
	stone->z.fixed = 0;
	/* Stone A goes too, whichever stone fell; the room places it again when it
	 */
	/* is next entered. */
	fdActorSetPos(ACTOR_MIMIC_STONE_A, 0, 0);
	fdActorRelease(mask);
	fdEventEnd();
}

/* A mimic stone reaches its button: it settles on the button's middle with a */
/* thump, and its room's north door opens, its flag keeping both so. */
void scAnemosInnerSanctumOpenMimicStoneDoor(void) {
	ACTOR *stone;
	s32 column;
	FIXED target[3];

	if(MainRoutineStateWork.entrance == ENTRANCE_BY_MIMIC_STONE_A) {
		stone = fdActorGet(ACTOR_MIMIC_STONE_A);
		fdEventSetFlag(FLAG_MIMIC_STONE_A_ON_BUTTON);
		column = 11;
	}
	else if(MainRoutineStateWork.entrance == ENTRANCE_BY_MIMIC_STONE_B) {
		stone = fdActorGet(ACTOR_MIMIC_STONE_B);
		fdEventSetFlag(FLAG_MIMIC_STONE_B_ON_BUTTON);
		column = 26;
	}
	else {
		stone = fdActorGet(ACTOR_MIMIC_STONE_C);
		fdEventSetFlag(FLAG_MIMIC_STONE_C_ON_BUTTON);
		column = 53;
	}
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	target[0] = (stone->x.fixed & FIELD_CELL_MASK) + toFIXED(8);
	target[2] = (stone->z.fixed & FIELD_CELL_MASK) + toFIXED(8);
	stone->acceleration = toFIXED(1);
	stone->speedLimit = toFIXED(1);
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	scFieldActorSetMoveTargetFX(stone, target[0], 0, target[2]);
	scFieldActorWaitForMove(stone);
	fdEventPlaySnd(SE_THUMP);
	fdMapGridCopyCellsTo(18, 4, column, 4, 1, 2);
	syWait(5);
	fdMapGridCopyCellsTo(18, 7, column, 4, 1, 2);
	syWait(5);
	fdMapGridSetCellHeight(FIELD_CELL_ORIGIN(column), toFIXED(88), 0, 0);
	fdEventEnd();
}

/* The first time Felix comes up to Charon's slate, the room's colours warm */
/* and the slate speaks: the power of the land to create a great darkness */
/* is for whoever attacks it. */
void scAnemosInnerSanctumApproachCharonSlate(void) {
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	fdPaletteFadeApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
	fdPaletteFadeApplyTarget(PALETTE_TINT_WARM, PALETTES_BG);
	fdPaletteFadeInterpolate(60);
	syFieldEventWait(60);
	/* "We inherited the power of the land to create a great darkness. If you */
	/* crave this power, attack this slate!" */
	mnFieldMessageShowCentered((s32)MsgSummonInheritedEarthDarkness, 0, 6);
	fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
	fdPaletteFadeInterpolate(60);
	syFieldEventWait(60);
	fdEventSetFlag(FLAG_ANEMOS_CHARON_SLATE_SPOKE);
	fdEventEnd();
}

extern TRIGGER AnemosInnerSanctum_Triggers[];
extern TRIGGER AnemosInnerSanctum_DragonSkinChestTriggers[];
extern TRIGGER AnemosInnerSanctum_ElevatorTriggers[];
extern TRIGGER AnemosInnerSanctum_SwitchesTriggers[];
extern TRIGGER AnemosInnerSanctum_OrihalconChestTriggers[];
extern TRIGGER AnemosInnerSanctum_GuardianTriggers[];

const TRIGGER *scGetTriggers(void) {
	if(MainRoutineStateWork.next ==
	   SCENE_ANEMOS_INNER_SANCTUM_DRAGON_SKIN_CHEST)
		return AnemosInnerSanctum_DragonSkinChestTriggers;
	if(MainRoutineStateWork.next == SCENE_ANEMOS_INNER_SANCTUM_ELEVATOR)
		return AnemosInnerSanctum_ElevatorTriggers;
	if(MainRoutineStateWork.next == SCENE_ANEMOS_INNER_SANCTUM_SWITCHES)
		return AnemosInnerSanctum_SwitchesTriggers;
	if(MainRoutineStateWork.next == SCENE_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST)
		return AnemosInnerSanctum_OrihalconChestTriggers;
	if(MainRoutineStateWork.next == SCENE_ANEMOS_INNER_SANCTUM_GUARDIAN)
		return AnemosInnerSanctum_GuardianTriggers;
	return AnemosInnerSanctum_Triggers;
}

/* Felix casts Teleport on the circle in the middle of the symbols' room: */
/* sparks fly as the colours cycle, and the party teleports back to */
/* Contigo. */
void scAnemosInnerSanctumTeleportOut(void) {
	scTeleportStartShimmer();
	fdTeleportPlaySparks();
	MainRoutineStateWork.teleportState = TELEPORT_ARRIVING;
	fdEventSetDestination(SCENE_CONTIGO,
						  CONTIGO_FROM_ANEMOS_INNER_SANCTUM_DRAGON_SKIN_CHEST);
	syFieldEventWait(40);
	scTeleportStopShimmer();
}

extern s16 AnemosInnerSanctum_NorthDoorFrames[];
extern MAP_GRID_CELL_ANIMATION AnemosInnerSanctum_MimicStoneDoors[];

/* Plays a door's map edit at x, y. */
static __inline__ void scAnemosInnerSanctumAnimateDoor(const void *steps, s32 x,
													   s32 y) {
	fdMapGridPlayMapEdit(steps, x, y);
}

/* Unused: stills every placed actor, opens the north door or a mimic stone's */
/* door by the step trigger's number, and walks the leader out by the exit of */
/* that number. */
UNUSED void scAnemosInnerSanctumEnterTheRoom(void) {
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	s32 i;

	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	for(i = ACTOR_FIRST_PLACED; (u32)i < FIELD_TREASURE_ACTOR; i++) {
		ACTOR *actor = fdActorGet(i);

		if(actor != NULL)
			actor->motionFlg = 0;
	}
	fdEventPlaySnd(SE_DOOR_OPEN);
	if(MainRoutineStateWork.next ==
	   SCENE_ANEMOS_INNER_SANCTUM_DRAGON_SKIN_CHEST) {
		fdMapGridAnimateCells(AnemosInnerSanctum_NorthDoorFrames, 33, 7);
	}
	else {
		s32 index = state->stepTrigger / 2 - 1;

		scAnemosInnerSanctumAnimateDoor(
			AnemosInnerSanctum_MimicStoneDoors[index].steps,
			AnemosInnerSanctum_MimicStoneDoors[index].x,
			AnemosInnerSanctum_MimicStoneDoors[index].y);
	}
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_WALK);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.5);
	fdActorSetDestinationOffset(MainRoutineStateWork.leaderActorId, 0, -4);
	syFieldEventWait(6);
	syFieldEventSetExitRequest(state->stepTrigger);
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	fdEventEnd();
}

/* The party arrives up the lift in the elevator room: it rises the last of */
/* the way into the room with Felix on it and stays there. */
void scAnemosInnerSanctumArriveOnElevatorLift(void) {
	ACTOR *lift = fdActorGet(ACTOR_ELEVATOR_LIFT);

	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	fdActorSetAnimation(ACTOR_ELEVATOR_LIFT, ARROW_PANEL_ANIMATION_UP);
	lift->y.fixed = -toFIXED(64);
	fdActorSetPos(ACTOR_ELEVATOR_LIFT, 728, 120);
	fdActorSetPos(MainRoutineStateWork.leaderActorId, 728, 120);
	fdActorGet(MainRoutineStateWork.leaderActorId)->y.fixed = -toFIXED(48);
	fdEventStartScrTransition();
	lift->acceleration = toFIXED(1);
	lift->speedLimit = toFIXED(1);
	scFieldActorSetMoveTargetFX(lift, lift->x.fixed,
								lift->y.fixed + toFIXED(48), lift->z.fixed);
	scFieldActorWaitForMove(lift);
	fdEventSetFlag(FLAG_ELEVATOR_LIFT_RISEN);
	fdEventEnd();
}

/* As scAnemosInnerSanctumArriveOnElevatorLift, on the guardian room's lift. */
void scAnemosInnerSanctumArriveOnGuardianLift(void) {
	ACTOR *lift = fdActorGet(ACTOR_GUARDIAN_LIFT);

	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	lift->motionFlg = 0;
	lift->y.fixed = -toFIXED(64);
	fdActorGet(MainRoutineStateWork.leaderActorId)->y.fixed = -toFIXED(48);
	fdEventStartScrTransition();
	lift->acceleration = toFIXED(1);
	lift->speedLimit = toFIXED(1);
	scFieldActorSetMoveTargetFX(lift, lift->x.fixed,
								lift->y.fixed + toFIXED(48), lift->z.fixed);
	scFieldActorWaitForMove(lift);
	fdEventSetFlag(FLAG_GUARDIAN_LIFT_RISEN);
	fdEventEnd();
}

s32 scCrackedTileFind();
void scCrackedTilePlaceLanding(s32 x, s32 z, s32 effect);
extern u16 TeleportCircle_PaletteSlots[];
extern u16 TeleportCircle_Colors[];
extern u16 TeleportCircle_ShimmerColors[];
extern u16 TeleportCircle_ShimmerSlots[];

/* Sets up the room the party comes into: the Dragon Skin room's lit torches */
/* and opened north door; the pillars left in their places; the lifts, below */
/* or above as they last carried the party, and the round switches already */
/* pressed; the cracked floors; and the mimic stones resting on their */
/* buttons, their doors open. */
s32 scInit(void) {
	s32 scene;
	s32 entrance;

	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	scene = MainRoutineStateWork.next;
	if(scene == SCENE_ANEMOS_INNER_SANCTUM_DRAGON_SKIN_CHEST) {
		scPaletteSetColsFade((s32)TeleportCircle_PaletteSlots,
							 (s32)TeleportCircle_Colors,
							 (s32)TeleportCircle_ShimmerColors,
							 (s32)TeleportCircle_ShimmerSlots);
		if(fdEventGetFlag(FLAG_ANEMOS_VENUS_TORCH_LIT)) {
			scAnemosInnerSanctumLightTorch(VENUS_TORCH_X, VENUS_TORCH_Y,
										   SPRITE_COLOR_NORMAL);
			fdMapGridCopyCellsTo(85, 34, 85, 8, 9, 8);
		}
		if(fdEventGetFlag(FLAG_ANEMOS_MERCURY_TORCH_LIT)) {
			scAnemosInnerSanctumLightTorch(MERCURY_TORCH_X, MERCURY_TORCH_Y,
										   SPRITE_COLOR_BGR);
			fdMapGridCopyCellsTo(101, 34, 101, 8, 9, 8);
		}
		if(fdEventGetFlag(FLAG_ANEMOS_MARS_TORCH_LIT)) {
			scAnemosInnerSanctumLightTorch(MARS_TORCH_X, MARS_TORCH_Y,
										   SPRITE_COLOR_RED_TINT);
			fdMapGridCopyCellsTo(85, 46, 85, 20, 9, 8);
		}
		if(fdEventGetFlag(FLAG_ANEMOS_JUPITER_TORCH_LIT)) {
			scAnemosInnerSanctumLightTorch(JUPITER_TORCH_X, JUPITER_TORCH_Y,
										   SPRITE_COLOR_RBG);
			fdMapGridCopyCellsTo(101, 46, 101, 20, 9, 8);
		}
		if(fdEventGetFlag(FLAG_ANEMOS_NORTH_DOOR_OPEN)) {
			fdMapGridCopyCellsTo(54, 7, 33, 7, 1, 2);
			fdMapGridSetCellHeight(toFIXED(536), toFIXED(136), 0, 0);
		}
	}
	else if(scene == SCENE_ANEMOS_INNER_SANCTUM_ELEVATOR) {
		scFieldActorSetSprPriority(ACTOR_ELEVATOR_FIRST_PILLAR, 1);
		if(fdEventGetFlag(FLAG_ANEMOS_ELEVATOR_FIRST_PILLAR_PLACED)) {
			fdActorSetPos(ACTOR_ELEVATOR_FIRST_PILLAR, 296, 248);
			scFieldActorSetSprPriority(ACTOR_ELEVATOR_FIRST_PILLAR, 2);
		}
		if(fdEventGetFlag(FLAG_ANEMOS_ELEVATOR_SECOND_PILLAR_PLACED))
			fdActorSetPos(ACTOR_ELEVATOR_SECOND_PILLAR, 440, 760);
		Call1((void (*)())fdActorPlaceBlocks,
			  (s32)AnemosInnerSanctum_ElevatorBlocks);
		fdActorGet(MainRoutineStateWork.leaderActorId)->priFlg |=
			ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
		fdActorGet(ACTOR_ELEVATOR_LIFT)->motionFlg = 0;
		fdActorGet(ACTOR_ELEVATOR_LIFT)->priFlg |= ACTOR_PRIORITY_PLATFORM;
		if(MainRoutineStateWork.entrance ==
			   WAY_ELEVATOR_ABOVE_THE_LIFT_TO_NORTHWEST &&
		   fdEventGetFlag(FLAG_ELEVATOR_LIFT_RISEN)) {
			fdActorSetAnimation(ACTOR_ELEVATOR_LIFT, ARROW_PANEL_ANIMATION_UP);
			fdActorSetPos(ACTOR_ELEVATOR_LIFT, 728, 120);
		}
		if(MainRoutineStateWork.entrance == ENTRANCE_UP_THE_LIFT &&
		   syGameFlagGet(FLAG_ELEVATOR_LIFT_RISEN) == 0)
			scAnemosInnerSanctumArriveOnElevatorLift();
		if(MainRoutineStateWork.entrance ==
		   WAY_ELEVATOR_ROUND_SWITCHES_TO_EAST) {
			if(fdEventGetFlag(FLAG_ELEVATOR_LIFT_RISEN))
				fdActorSetPos(ACTOR_ELEVATOR_LIFT, 0, 0);
			if(fdEventGetFlag(FLAG_LIFT_READY))
				fdActorGet(ACTOR_ELEVATOR_LIFT)->motionFlg = ACTOR_FLOATS;
			if(fdEventGetFlag(FLAG_ROUND_SWITCH_PRESSED(0)))
				fdMapGridCopyCellsTo(54, 87, 44, 86, 1, 1);
			if(fdEventGetFlag(FLAG_ROUND_SWITCH_PRESSED(1)))
				fdMapGridCopyCellsTo(54, 87, 46, 86, 1, 1);
			if(fdEventGetFlag(FLAG_ROUND_SWITCH_PRESSED(2)))
				fdMapGridCopyCellsTo(54, 87, 43, 87, 1, 1);
			if(fdEventGetFlag(FLAG_ROUND_SWITCH_PRESSED(3)))
				fdMapGridCopyCellsTo(54, 87, 45, 87, 1, 1);
			if(fdEventGetFlag(FLAG_ROUND_SWITCH_PRESSED(4)))
				fdMapGridCopyCellsTo(54, 87, 47, 87, 1, 1);
			if(fdEventGetFlag(FLAG_ROUND_SWITCH_PRESSED(5)))
				fdMapGridCopyCellsTo(54, 87, 44, 88, 1, 1);
			if(fdEventGetFlag(FLAG_ROUND_SWITCH_PRESSED(6)))
				fdMapGridCopyCellsTo(54, 87, 46, 88, 1, 1);
			fdActorGet(MainRoutineStateWork.leaderActorId)->priFlg |=
				ACTOR_PRIORITY_IGNORES_RISES;
		}
	}
	else if(scene == SCENE_ANEMOS_INNER_SANCTUM_SWITCHES) {
		ACTOR *marker = fdActorGet(ACTOR_SWITCHES_SWITCH_MARKER);

		marker->motionFlg = ACTOR_FALLS;
		marker->groundHeight = -toFIXED(14);
		marker->y.fixed = -toFIXED(14);
		marker->priFlg |= ACTOR_PRIORITY_PLATFORM;
		fdActorGet(MainRoutineStateWork.leaderActorId)->priFlg |=
			ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
		fdActorGet(ACTOR_SWITCHES_BOULDER)->sceneCnt = 1;
		if(fdEventGetFlag(FLAG_ANEMOS_SWITCHES_FIRST_PILLAR_PLACED))
			fdActorSetPos(ACTOR_SWITCHES_FIRST_PILLAR, 56, 424);
		if(fdEventGetFlag(FLAG_ANEMOS_SWITCHES_SECOND_PILLAR_PLACED)) {
			fdActorSetPos(ACTOR_SWITCHES_SECOND_PILLAR, 536, 360);
			fdMapGridCopyCellsTo(51, 20, 31, 20, 1, 2);
			fdMapGridSetCellHeight(toFIXED(496), toFIXED(336), 0, 0);
		}
		if(fdEventGetFlag(FLAG_ANEMOS_SWITCHES_THIRD_PILLAR_PLACED))
			fdActorSetPos(ACTOR_SWITCHES_THIRD_PILLAR, 744, 456);
		if(fdEventGetFlag(FLAG_ANEMOS_SWITCHES_PILLAR_B_PLACED)) {
			if(!fdEventGetFlag(FLAG_KEEP_PARTY_POSITION) &&
			   !fdEventGetFlag(FLAG_ANEMOS_SWITCHES_THIRD_PILLAR_PLACED))
				fdActorSetPosFX(
					ACTOR_SWITCHES_THIRD_PILLAR,
					INTtoFIXED(
						fdActorGet(ACTOR_SWITCHES_PILLAR_B)->x.part[0].integer),
					INTtoFIXED(fdActorGet(ACTOR_SWITCHES_PILLAR_B)
								   ->z.part[0]
								   .integer));
			fdActorSetPos(ACTOR_SWITCHES_PILLAR_B, 728, 472);
		}
		Call1((void (*)())fdActorPlaceBlocks,
			  (s32)AnemosInnerSanctum_SwitchesBlocks);
	}
	else if(scene == SCENE_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST) {
		if(fdEventGetFlag(FLAG_ANEMOS_ORIHALCON_CHEST_FIRST_PILLAR_PLACED))
			fdActorSetPos(ACTOR_ORIHALCON_CHEST_FIRST_PILLAR, 264, 360);
		Call1((void (*)())fdActorPlaceBlocks,
			  (s32)AnemosInnerSanctum_OrihalconChestBlocks);
		entrance = MainRoutineStateWork.entrance;
		if(entrance ==
		   ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_FROM_ANEMOS_INNER_SANCTUM_SWITCHES_NORTHWEST) {
			scCrackedTileFind(TRIGGER_FIRST_CRACKED_FLOOR, FLAG_SCENE_TEMP_80,
							  FLAG_AREA_TEMP_80, 18, 68);
			if(fdEventGetFlag(FLAG_GUARDIAN_LIFT_RISEN) == 0) {
				fdActorGet(MainRoutineStateWork.leaderActorId)->priFlg |=
					ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
				fdActorGet(ACTOR_ORIHALCON_CHEST_LIFT)->motionFlg = 0;
				fdActorGet(ACTOR_ORIHALCON_CHEST_LIFT)->priFlg |=
					ACTOR_PRIORITY_PLATFORM;
			}
			else {
				fdActorSetPos(ACTOR_ORIHALCON_CHEST_LIFT, 0, 0);
				fdEventSetFlag(FLAG_ROUND_SWITCH_PRESSED(0));
				fdEventSetFlag(FLAG_ROUND_SWITCH_PRESSED(1));
				fdEventSetFlag(FLAG_ROUND_SWITCH_PRESSED(2));
				fdEventSetFlag(FLAG_ROUND_SWITCH_PRESSED(3));
				fdEventSetFlag(FLAG_ROUND_SWITCH_PRESSED(4));
				fdEventSetFlag(FLAG_ROUND_SWITCH_PRESSED(5));
				fdEventSetFlag(FLAG_ROUND_SWITCH_PRESSED(6));
			}
			if(fdEventGetFlag(FLAG_LIFT_READY))
				fdActorGet(ACTOR_ORIHALCON_CHEST_LIFT)->motionFlg =
					ACTOR_FLOATS;
			if(fdEventGetFlag(FLAG_ROUND_SWITCH_PRESSED(0)))
				fdMapGridCopyCellsTo(17, 71, 8, 69, 1, 1);
			if(fdEventGetFlag(FLAG_ROUND_SWITCH_PRESSED(5)))
				fdMapGridCopyCellsTo(17, 71, 6, 71, 1, 1);
			if(fdEventGetFlag(FLAG_ROUND_SWITCH_PRESSED(1)))
				fdMapGridCopyCellsTo(17, 71, 5, 75, 1, 1);
			if(fdEventGetFlag(FLAG_ROUND_SWITCH_PRESSED(2)))
				fdMapGridCopyCellsTo(17, 71, 8, 75, 1, 1);
			if(fdEventGetFlag(FLAG_ROUND_SWITCH_PRESSED(3)))
				fdMapGridCopyCellsTo(17, 71, 11, 75, 1, 1);
			if(fdEventGetFlag(FLAG_ROUND_SWITCH_PRESSED(4)))
				fdMapGridCopyCellsTo(17, 71, 8, 73, 1, 1);
			if(fdEventGetFlag(FLAG_ROUND_SWITCH_PRESSED(6)))
				fdMapGridCopyCellsTo(17, 71, 10, 71, 1, 1);
		}
		else if(entrance == THROUGH_FIRST_CRACKED_FLOOR) {
			scCrackedTileFind(TRIGGER_FIRST_CRACKED_FLOOR, FLAG_SCENE_TEMP_80,
							  FLAG_AREA_TEMP_80, 18, 68);
			scCrackedTilePlaceLanding(
				22, 0,
				SCENE_TRANSITION(TRANSITION_WINDOW,
								 WINDOW_TRANSITION_INTERLACED_WIPE));
		}
		else if(entrance == THROUGH_SECOND_CRACKED_FLOOR) {
			scCrackedTileFind(TRIGGER_SECOND_CRACKED_FLOOR, FLAG_SCENE_TEMP_80,
							  FLAG_AREA_TEMP_80, 18, 68);
			if(fdEventGetFlag(FLAG_KEEP_PARTY_POSITION) == 0)
				scCrackedTilePlaceLanding(
					36, -36,
					SCENE_TRANSITION(TRANSITION_WINDOW,
									 WINDOW_TRANSITION_INTERLACED_WIPE));
		}
		else if(MainRoutineStateWork.entrance >=
					WAY_ORIHALCON_CHEST_CRACKED_FLOOR_TO_SOUTHEAST &&
				MainRoutineStateWork.entrance <=
					WAY_ORIHALCON_CHEST_CRACKED_FLOOR_TO_SOUTHEAST + 1) {
			scCrackedTileFind(TRIGGER_SECOND_CRACKED_FLOOR, FLAG_SCENE_TEMP_80,
							  FLAG_AREA_TEMP_80, 18, 68);
		}
	}
	else if(scene == SCENE_ANEMOS_INNER_SANCTUM_GUARDIAN) {
		MAP_STATE *map = Ram_HeapSlots->mapGrid;

		switch(MainRoutineStateWork.entrance) {
		case ANEMOS_INNER_SANCTUM_GUARDIAN_FROM_ANEMOS_INNER_SANCTUM_SWITCHES_SOUTHWEST:
			if(fdEventGetFlag(FLAG_GUARDIAN_LIFT_RISEN) == 0)
				fdActorSetPos(ACTOR_GUARDIAN_LIFT, 0, 0);
		case ANEMOS_INNER_SANCTUM_GUARDIAN_FROM_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_SOUTHWEST:
			fdActorGet(MainRoutineStateWork.leaderActorId)->priFlg |=
				ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
			fdActorSetAnimation(ACTOR_GUARDIAN_LIFT, ARROW_PANEL_ANIMATION_UP);
			fdActorGet(ACTOR_GUARDIAN_LIFT)->motionFlg = 0;
			fdActorGet(ACTOR_GUARDIAN_LIFT)->priFlg |= ACTOR_PRIORITY_PLATFORM;
			fdActorSetAnimation(ACTOR_GUARDIAN_LIFT, ARROW_PANEL_ANIMATION_UP);
			if(fdEventGetFlag(FLAG_GUARDIAN_LIFT_RISEN) == 0 &&
			   MainRoutineStateWork.entrance ==
				   ANEMOS_INNER_SANCTUM_GUARDIAN_FROM_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_SOUTHWEST)
				scAnemosInnerSanctumArriveOnGuardianLift();
			else
				fdActorGet(ACTOR_GUARDIAN_LIFT)->y.fixed = -toFIXED(16);
			break;
		case ENTRANCE_BY_MIMIC_STONE_A:
		case ANEMOS_INNER_SANCTUM_GUARDIAN_FROM_ANEMOS_INNER_SANCTUM_ELEVATOR_NORTHWEST:
			if(fdEventGetFlag(FLAG_MIMIC_STONE_A_ON_BUTTON)) {
				fdActorSetPos(ACTOR_MIMIC_STONE_A, 120, 120);
				fdEventSetFlag(FLAG_MIMIC_STONE_STARTED);
			}
			else {
				fdMapGridCopyCellsTo(18, 1, 11, 4, 1, 2);
				fdMapGridSetCellHeight(toFIXED(184), toFIXED(88), 0, 2);
			}
			break;
		case ENTRANCE_BY_MIMIC_STONE_B:
		case ANEMOS_INNER_SANCTUM_GUARDIAN_FROM_ANEMOS_INNER_SANCTUM_SWITCHES_NORTHEAST:
			if(fdEventGetFlag(FLAG_MIMIC_STONE_B_ON_BUTTON)) {
				fdActorSetPos(ACTOR_MIMIC_STONE_B, 552, 120);
				fdEventSetFlag(FLAG_MIMIC_STONE_STARTED);
			}
			else {
				fdMapGridCopyCellsTo(18, 1, 26, 4, 1, 2);
				fdMapGridSetCellHeight(toFIXED(424), toFIXED(88), 0, 2);
			}
			break;
		case ENTRANCE_BY_MIMIC_STONE_C:
		case ANEMOS_INNER_SANCTUM_GUARDIAN_FROM_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_NORTHEAST:
			if(fdEventGetFlag(FLAG_MIMIC_STONE_C_ON_BUTTON)) {
				fdActorSetPos(ACTOR_MIMIC_STONE_C, 792, 120);
				fdEventSetFlag(FLAG_MIMIC_STONE_STARTED);
			}
			else {
				fdMapGridCopyCellsTo(18, 1, 53, 4, 1, 2);
				fdMapGridSetCellHeight(toFIXED(856), toFIXED(88), 0, 2);
			}
			map->minX += toFIXED(8);
			map->maxX += toFIXED(8);
			break;
		}
	}
	return 0;
}

s32 scPrepareMap(void) { return 0; }

#include "../field/field_effect_set_sprite_priority.c"

/* An effect's update: moves it by its velocity, grows it by its scale rates */
/* and spins its sprite. */
void scEffectMoveLocal(FIELD_OBJECT *object) {
	s32 x;
	s32 velocityX;
	FIELD_SPR *sprite;

	x = object->eff.x;
	velocityX = object->eff.vel.x;
	sprite = object->eff.spr;

	object->eff.x = x + velocityX;
	object->eff.y += object->eff.vel.y;
	object->eff.z += object->eff.vel.z;
	object->eff.scaleX += object->eff.scaleRateX;
	object->eff.scaleY += object->eff.scaleRateY;
	sprite->rotation += object->eff.spin;
}

extern s32 FieldEffect_DurationScripts[];
void fdEffectSetSprPriority(ACTOR *actor, s32 priority);
/* As scFieldEffectSpawn, the effect taking the leader's sprite priority and */
/* moved by scEffectMoveLocal: the shared spawner's update and priority setter
 */
/* are fixed, so this copy is written out too. */
void scFieldEffectSpawnLocal(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
							 s32 velocityZ, u32 flags,
							 const EFFECT_OPTIONS *options) {
	FIELD_EFFECT *party =
		((FIELD_EFFECT *)fdActorGetById(MainRoutineStateWork.leaderActorId));
	FIELD_EFFECT *effect;
	FIELD_SPR *sprite;
	const s32 *script;
	u32 start;

	if((flags & EFFECT_USE_TYPE) != 0 && options != 0)
		effect = (FIELD_EFFECT *)syImportActorCreate(options->type, x, y, z);
	else
		effect =
			(FIELD_EFFECT *)syImportActorCreate(SPRITE_DUST_CLOUD, x, y, z);
	if(effect == 0)
		return;

	sprite = effect->spr;
	fdActorSelectAnimation(effect, (flags + 1) & EFFECT_SCRIPT_MASK);
	fdActorStartScript(effect,
					   FieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK]);
	effect->motionFlg = 0;
	fdActorSetSprFlags(effect, 0);
	effect->update = scEffectMoveLocal;
	effect->vel.x = velocityX;
	effect->vel.y = velocityY;
	effect->vel.z = velocityZ;
	fdEffectSetSprPriority(effect, party->spr->pri);
	effect->scaleRateX = 0;
	effect->scaleRateY = 0;
	effect->spin = 0;

	if((flags & EFFECT_OPTION_MASK) == 0 || options == 0)
		return;
	if((flags & EFFECT_USE_PALETTE) != 0)
		scFieldActorSetSprCol(effect, options->pal);
	if((flags & EFFECT_USE_PRIORITY) != 0) {
		effect->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;
		fdEffectSetSprPriority(effect, options->pri);
	}
	start = flags & EFFECT_USE_START_SCALE;
	if(start != 0) {
		effect->scaleX = options->startScaleX;
		effect->scaleY = options->startScaleY;
	}
	if((flags & EFFECT_SCALE_TO_TARGET) != 0) {
		FIXED targetX;
		s32 count;

		script = FieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK];
		targetX = options->tgtScaleX;
		count = script[EFFECT_SCRIPT_REPEAT_COUNT];
		if(start != 0) {
			effect->scaleRateX = (targetX - effect->scaleX) / count;
			effect->scaleRateY = (options->tgtScaleY - effect->scaleY) /
								 script[EFFECT_SCRIPT_REPEAT_COUNT];
		}
		else {
			effect->scaleRateX = (targetX - FIXED_ONE) / count;
			effect->scaleRateY = (options->tgtScaleY - FIXED_ONE) /
								 script[EFFECT_SCRIPT_REPEAT_COUNT];
		}
	}
	if((flags & EFFECT_USE_SCRIPT) != 0) {
		fdActorSelectAnimation(effect, 1);
		fdActorStartScript(effect, options->script);
	}
	if((flags & EFFECT_USE_ROTATION) != 0)
		sprite->rotation = options->rotation;
	if((flags & EFFECT_USE_SPIN) != 0)
		effect->spin = options->spin;
	if((flags & EFFECT_USE_UPDATE) != 0)
		effect->update = options->update;
}

#define CELL_PUSH_SLOT_ENTRY syOamAddSprAtDepth
#include "../field/cracked_tile_mask.c"

#include "shared/cracked_tile_find.h"

#include "shared/leader_center_in_cell.h"

#include "shared/cracked_tile_crack.h"

#include "shared/cracked_tile_fall.h"

#include "../field/field_effect_advance_motion.c"

#undef DEFINE_ANEMOS_INNER_SANCTUM_EXIT_IF
#undef CELL_PUSH_SLOT_ENTRY
/* The leader lands from the cracked floor above in a ring of dust, as Mars */
/* Lighthouse's lands from its cracks (MarsLighthouseEntrance_LandFromCrack). */
void scCrackedTileDropIn(void) {
	ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	u32 i;
	EFFECT_OPTIONS options;

	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdMapGridCenterOnFollowedPoint();
	syWait(1);
	leader->y.fixed = toFIXED(130);
	leader->gravity = toFIXED(1) / 2;
	leader->bounce = 0;
	leader->motionFlg = 0;
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	fdEventStartScrTransition();
	fdEventWaitTransitionFrames();
	fdEventPlaySnd(SE_DUST_PUFF);
	leader->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	syFieldEventWait(24);
	options.pal = SPRITE_COLOR_WHITE;
	options.update = fdEffectAdvanceMotion;
	options.startScaleX = FIXED_ONE * 16 / 25;
	options.startScaleY = FIXED_ONE * 16 / 25;
	for(i = 0; i <= DUST_RING_PUFFS; i++) {
		s32 angle;
		s32 velocity[3];
		FIXED cosine;

		angle = i * (TRIG_TURN / DUST_RING_PUFFS);
		cosine = syCos(angle);
		velocity[0] = cosine;
		velocity[1] = 0;
		velocity[2] = syMathSin(angle);
		velocity[0] += velocity[0] >> 1;
		velocity[0] += (syRandom() * (toFIXED(3) / 4) >> 16) - (toFIXED(3) / 8);
		velocity[2] +=
			(syRandom() * (toFIXED(3) / 8) >> 16) - (toFIXED(3) / 16);
		scFieldEffectSpawnLocal(leader->x.fixed, leader->y.fixed,
								leader->z.fixed, velocity[0], velocity[1],
								velocity[2],
								EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE |
									EFFECT_USE_PALETTE | EFFECT_SCRIPT_MEDIUM,
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
	leader->gravity = toFIXED(1);
	leader->bounce = FIXED_ONE / 4;
	syFieldEventWait(10);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_STAND);
	fdEventEnd();
}

void syGameFlagSetByte(s32 flag, s32 value);
#include "shared/cracked_tile_step.h"

#include "shared/cracked_tile_place_landing.h"

extern u32 FieldEffect_ShortScript[];
extern u32 FieldEffect_MediumScript[];

/* clang-format off */
/* The same three lifetimes as FieldEffect_ShortScript, MediumScript and */
/* LongScript (shared/field_effect_duration_scripts.c), under other names. */
u32 AnemosInnerSanctum_ShortScript[] = {
    SC_WAIT(1),
    SC_LOOP(22, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
    SC_END,
};

u32 AnemosInnerSanctum_MediumScript[] = {
    SC_WAIT(1),
    SC_LOOP(44, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
    SC_END,
};

u32 AnemosInnerSanctum_LongScript[] = {
    SC_WAIT(1),
    SC_LOOP(126, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
    SC_END,
};

s32 AnemosInnerSanctum_SwitchGlowScript[] = {
    SC_WAIT(20),
    SC_RELEASE,
    SC_END,
};

s32 AnemosInnerSanctum_OrbScript[] = {
    SC_CALL(scAnemosInnerSanctumCircleOrb),
    SC_RELEASE,
    SC_END,
};

s32 AnemosInnerSanctum_DustScript[] = {
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, -FIXED_ONE / 10),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 10),
    SC_WAIT(1),
    SC_LOOP(8, SC_TO(1)),
    SC_RELEASE,
    SC_END,
};

u32 AnemosInnerSanctum_DjinniRisesScript[] = {
    SC_WAIT(50),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 10),
    SC_ADD(SW_SCALE_Y, FIXED_ONE / 10),
    SC_ADD(SW_Y, toFIXED(3)),
    SC_WAIT(2),
    SC_LOOP(10, SC_TO(1)),
    SC_SET(SW_MOTION, ACTOR_FLOATS),
    SC_END,
};

s32 AnemosInnerSanctum_DjinniSinksScript[] = {
    SC_WAIT(30),
    SC_SET(SW_MOTION, 0),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, -FIXED_ONE / 10),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 10),
    SC_ADD(SW_Y, -toFIXED(3)),
    SC_WAIT(2),
    SC_LOOP(10, SC_TO(1)),
    SC_END,
};

u32 AnemosInnerSanctum_FadingSparkScript[] = {
    SC_WAIT(3),
    SC_RELEASE,
    SC_END,
};

s32 AnemosInnerSanctum_SparkScript[] = {
    SC_SET(SW_SCALE_X, FIXED_ONE * 2),
    SC_SET(SW_SCALE_Y, FIXED_ONE * 2),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, -FIXED_ONE / 10),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 10),
    SC_WAIT(2),
    SC_LOOP(10, SC_TO(1)),
    SC_CALL((s32)scAnemosInnerSanctumSpiralIn),
    SC_CALL((s32)scAnemosInnerSanctumLightPlate),
    SC_RELEASE,
    SC_END,
};

/* The blocks Move pushes in each room (fdActorLiftTargetBlock), ended by -1: */
/* the elevator room's two pillars. */
s16 AnemosInnerSanctum_ElevatorBlocks[] = {
    ACTOR_ELEVATOR_FIRST_PILLAR, ACTOR_ELEVATOR_SECOND_PILLAR, -1,
};

/* The switches room's five pillars. */
s16 AnemosInnerSanctum_SwitchesBlocks[] = {
    ACTOR_SWITCHES_FIRST_PILLAR, ACTOR_SWITCHES_SECOND_PILLAR,
    ACTOR_SWITCHES_PILLAR_A,     ACTOR_SWITCHES_PILLAR_B,
    ACTOR_SWITCHES_THIRD_PILLAR, -1,
};

/* The Orihalcon chest room's two pillars. */
s16 AnemosInnerSanctum_OrihalconChestBlocks[] = {
    ACTOR_ORIHALCON_CHEST_FIRST_PILLAR, ACTOR_ORIHALCON_CHEST_SECOND_PILLAR, -1,
};

/* Where the sparks fly to round the leader as a symbol lights, in pixels from */
/* him in x and z. */
s32 AnemosInnerSanctum_PlateOffsets[][2] = {
    {-48, -32}, {48, -32}, {-64, -16}, {64, -16}, {-32, -16}, {32, -16},
    {-48, 0}, {48, 0}, {-32, 0}, {32, 0}, {-64, 16}, {64, 16}, {-32, 16},
    {32, 16}, {-48, 32}, {48, 32}, {-16, 32}, {16, 32},
};

#include "shared/field_effect_duration_scripts.c"

/* The field effects' scripts by how long they last, which scFieldEffectSpawn */
/* picks by its flags (EFFECT_SCRIPT_MASK). */
s32 AnemosInnerSanctum_DurationScripts[] = {
    (s32)AnemosInnerSanctum_ShortScript,
    (s32)AnemosInnerSanctum_MediumScript,
    (s32)AnemosInnerSanctum_LongScript,
};

#define TELEPORT_CIRCLE_FIRST_ENTRY 89
#include "shared/teleport_circle_palette.c"

s32 AnemosInnerSanctum_NoShadowScript[] = {
    SC_CALL((s32)scAnemosInnerSanctumClearScriptedActorSprFlags),
    SC_END,
};

#include "shared/scene_entrances_default.c"

u32 SceneExits[] = {
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

ACTOR_PLACE AnemosInnerSanctum_Placements[] = {
    {SCENE_TABLE_END},
};

ACTOR_PLACE AnemosInnerSanctum_DragonSkinChestPlacements[] = {
    {SCENE_TABLE_END},
};

ACTOR_PLACE AnemosInnerSanctum_ElevatorPlacements[] = {
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 1, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_ARROW_PANEL, CONDITION_ALWAYS, (s32)AnemosInnerSanctum_NoShadowScript, 2, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 3, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SCENE_TABLE_END},
};

ACTOR_PLACE AnemosInnerSanctum_SwitchesPlacements[] = {
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 1, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 2, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 3, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 4, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 5, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 6, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 7, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SCENE_TABLE_END},
};

ACTOR_PLACE AnemosInnerSanctum_OrihalconChestPlacements[] = {
    {SPRITE_ARROW_PANEL, CONDITION_ALWAYS, (s32)AnemosInnerSanctum_NoShadowScript, 3, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 4, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 5, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SCENE_TABLE_END},
};

ACTOR_PLACE AnemosInnerSanctum_GuardianPlacements[] = {
    {SPRITE_MIMIC_STONE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 1, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_MIMIC_STONE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 2, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_MIMIC_STONE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 3, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_ARROW_PANEL, CONDITION_ALWAYS, (s32)AnemosInnerSanctum_NoShadowScript, 4, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    ACTOR_PLACEMENT(SPRITE_COILED_DRAGON_STATUE, CONDITION_ALWAYS, (s32)AnemosInnerSanctum_NoShadowScript, 584, 0, 582, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_COILED_DRAGON_STATUE, CONDITION_ALWAYS, (s32)AnemosInnerSanctum_NoShadowScript, 584, 0, 630, FACING_WEST, TALK_FACING_PARTY_AND_BACK, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_COILED_DRAGON_STATUE, CONDITION_ALWAYS, (s32)AnemosInnerSanctum_NoShadowScript, 680, 0, 582, FACING_EAST, TALK_FACING_PARTY_AND_BACK, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_COILED_DRAGON_STATUE, CONDITION_ALWAYS, (s32)AnemosInnerSanctum_NoShadowScript, 680, 0, 630, FACING_EAST, TALK_FACING_PARTY_AND_BACK, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {SCENE_TABLE_END},
};

TRIGGER AnemosInnerSanctum_Triggers[] = {
    {TRIGGER_TABLE_END},
};

TRIGGER AnemosInnerSanctum_DragonSkinChestTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_DRAGON_SKIN_CHEST_TO_ANEMOS_INNER_SANCTUM_ENTRANCE},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_DRAGON_SKIN_CHEST_TO_ANEMOS_INNER_SANCTUM_GUARDIAN},
    {TRIGGER_STEP, TRIGGER_FLOOR_BETWEEN_SYMBOLS, CONDITION_ALWAYS, EVENT(scAnemosInnerSanctumStepOffSymbol)},
    {TRIGGER_STEP, TRIGGER_VENUS_SYMBOL, FLAG_ON_SYMBOL, EVENT(scAnemosInnerSanctumLightSymbol)},
    {TRIGGER_STEP, TRIGGER_MERCURY_SYMBOL, FLAG_ON_SYMBOL, EVENT(scAnemosInnerSanctumLightSymbol)},
    {TRIGGER_STEP, TRIGGER_MARS_SYMBOL, FLAG_ON_SYMBOL, EVENT(scAnemosInnerSanctumLightSymbol)},
    {TRIGGER_STEP, TRIGGER_JUPITER_SYMBOL, FLAG_ON_SYMBOL, EVENT(scAnemosInnerSanctumLightSymbol)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_TELEPORT, PSYNERGY_PHASE_AFTER_CAST), TRIGGER_TELEPORT_CIRCLE, CONDITION_ALWAYS, EVENT(scAnemosInnerSanctumTeleportOut)},
    {TRIGGER_TABLE_END},
};

TRIGGER AnemosInnerSanctum_ElevatorTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 4, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_ELEVATOR_TO_ANEMOS_INNER_SANCTUM_SWITCHES},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_ELEVATOR_TO_ANEMOS_INNER_SANCTUM_GUARDIAN_NORTHWEST},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_ELEVATOR_TO_ANEMOS_INNER_SANCTUM_GUARDIAN_WEST},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 16, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_ELEVATOR_TO_ANEMOS_INNER_SANCTUM_GUARDIAN_EAST},
    {TRIGGER_EXIT, 17, CONDITION_ALWAYS, 17},
    {TRIGGER_STEP, TRIGGER_LIFT, IF_FLAG_SET(FLAG_LIFT_READY), EVENT(scAnemosInnerSanctumRideLiftUp)},
    {TRIGGER_STEP, TRIGGER_FIRST_ROUND_SWITCH, FLAG_ROUND_SWITCH_PRESSED(0), EVENT(scAnemosInnerSanctumPressRoundSwitch)},
    {TRIGGER_STEP, TRIGGER_FIRST_ROUND_SWITCH + 1, FLAG_ROUND_SWITCH_PRESSED(1), EVENT(scAnemosInnerSanctumPressRoundSwitch)},
    {TRIGGER_STEP, TRIGGER_FIRST_ROUND_SWITCH + 2, FLAG_ROUND_SWITCH_PRESSED(2), EVENT(scAnemosInnerSanctumPressRoundSwitch)},
    {TRIGGER_STEP, TRIGGER_FIRST_ROUND_SWITCH + 3, FLAG_ROUND_SWITCH_PRESSED(3), EVENT(scAnemosInnerSanctumPressRoundSwitch)},
    {TRIGGER_STEP, TRIGGER_FIRST_ROUND_SWITCH + 4, FLAG_ROUND_SWITCH_PRESSED(4), EVENT(scAnemosInnerSanctumPressRoundSwitch)},
    {TRIGGER_STEP, TRIGGER_FIRST_ROUND_SWITCH + 5, FLAG_ROUND_SWITCH_PRESSED(5), EVENT(scAnemosInnerSanctumPressRoundSwitch)},
    {TRIGGER_STEP, TRIGGER_FIRST_ROUND_SWITCH + 6, FLAG_ROUND_SWITCH_PRESSED(6), EVENT(scAnemosInnerSanctumPressRoundSwitch)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(scAnemosInnerSanctumStartMovingBlock)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(scAnemosInnerSanctumFinishMovingBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_ELEVATOR_FIRST_PILLAR, FLAG_ANEMOS_ELEVATOR_FIRST_PILLAR_PLACED, EVENT(scAnemosInnerSanctumStartMovingBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ELEVATOR_FIRST_PILLAR, FLAG_ANEMOS_ELEVATOR_FIRST_PILLAR_PLACED, EVENT(scAnemosInnerSanctumFinishMovingBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_ELEVATOR_SECOND_PILLAR, FLAG_ANEMOS_ELEVATOR_SECOND_PILLAR_PLACED, EVENT(scAnemosInnerSanctumStartMovingBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ELEVATOR_SECOND_PILLAR, FLAG_ANEMOS_ELEVATOR_SECOND_PILLAR_PLACED, EVENT(scAnemosInnerSanctumFinishMovingBlock)},
    {TRIGGER_TABLE_END},
};

TRIGGER AnemosInnerSanctum_SwitchesTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_SWITCHES_TO_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_SOUTHEAST},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 6, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_SWITCHES_TO_ANEMOS_INNER_SANCTUM_GUARDIAN_SOUTHWEST},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 7, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_SWITCHES_TO_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_NORTH},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 8, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_SWITCHES_TO_ANEMOS_INNER_SANCTUM_ELEVATOR},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_SWITCHES_TO_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_NORTHWEST},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_SWITCHES_TO_ANEMOS_INNER_SANCTUM_GUARDIAN_NORTHEAST},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_EXIT, 16, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_SWITCHES_TO_ANEMOS_INNER_SANCTUM_GUARDIAN_EAST},
    {TRIGGER_EXIT, 17, CONDITION_ALWAYS, 17},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(scAnemosInnerSanctumSinkIntoSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(scAnemosInnerSanctumEndSand)},
    {TRIGGER_STEP, TRIGGER_SWITCH, FLAG_SWITCH_PRESSED, EVENT(scAnemosInnerSanctumPressSwitch)},
    {TRIGGER_STEP, TRIGGER_EAST_OF_SWITCH, IF_FLAG_SET(FLAG_SWITCH_PRESSED), EVENT(scAnemosInnerSanctumStepOffSwitch)},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_SWITCHES_BOULDER, CONDITION_ALWAYS, 0},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(scAnemosInnerSanctumStartMovingBlock)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(scAnemosInnerSanctumFinishMovingBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_SWITCHES_FIRST_PILLAR, FLAG_ANEMOS_SWITCHES_FIRST_PILLAR_PLACED, EVENT(scAnemosInnerSanctumStartMovingBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SWITCHES_FIRST_PILLAR, FLAG_ANEMOS_SWITCHES_FIRST_PILLAR_PLACED, EVENT(scAnemosInnerSanctumFinishMovingBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_SWITCHES_SECOND_PILLAR, FLAG_ANEMOS_SWITCHES_SECOND_PILLAR_PLACED, EVENT(scAnemosInnerSanctumStartMovingBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SWITCHES_SECOND_PILLAR, FLAG_ANEMOS_SWITCHES_SECOND_PILLAR_PLACED, EVENT(scAnemosInnerSanctumFinishMovingBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_SWITCHES_PILLAR_A, CONDITION_ALWAYS, EVENT(scAnemosInnerSanctumStartMovingBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SWITCHES_PILLAR_A, CONDITION_ALWAYS, EVENT(scAnemosInnerSanctumFinishMovingBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_SWITCHES_PILLAR_B, FLAG_ANEMOS_SWITCHES_PILLAR_B_PLACED, EVENT(scAnemosInnerSanctumStartMovingBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SWITCHES_PILLAR_B, FLAG_ANEMOS_SWITCHES_PILLAR_B_PLACED, EVENT(scAnemosInnerSanctumFinishMovingBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_SWITCHES_THIRD_PILLAR, FLAG_ANEMOS_SWITCHES_THIRD_PILLAR_PLACED, EVENT(scAnemosInnerSanctumStartMovingBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SWITCHES_THIRD_PILLAR, FLAG_ANEMOS_SWITCHES_THIRD_PILLAR_PLACED, EVENT(scAnemosInnerSanctumFinishMovingBlock)},
    {TRIGGER_TABLE_END},
};

TRIGGER AnemosInnerSanctum_OrihalconChestTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_TO_ANEMOS_INNER_SANCTUM_SWITCHES_NORTHWEST},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_TO_ANEMOS_INNER_SANCTUM_SWITCHES_NORTH},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_TO_ANEMOS_INNER_SANCTUM_ENTRANCE},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_TO_ANEMOS_INNER_SANCTUM_GUARDIAN_EAST},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 7, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_TO_ANEMOS_INNER_SANCTUM_SWITCHES_SOUTHEAST},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_TO_ANEMOS_INNER_SANCTUM_GUARDIAN_NORTHEAST},
    {TRIGGER_STEP, TRIGGER_LIFT, IF_FLAG_SET(FLAG_LIFT_READY), EVENT(scAnemosInnerSanctumRideLiftUp)},
    {TRIGGER_STEP, TRIGGER_FIRST_ROUND_SWITCH, FLAG_ROUND_SWITCH_PRESSED(0), EVENT(scAnemosInnerSanctumPressRoundSwitch)},
    {TRIGGER_STEP, TRIGGER_FIRST_ROUND_SWITCH + 1, FLAG_ROUND_SWITCH_PRESSED(1), EVENT(scAnemosInnerSanctumPressRoundSwitch)},
    {TRIGGER_STEP, TRIGGER_FIRST_ROUND_SWITCH + 2, FLAG_ROUND_SWITCH_PRESSED(2), EVENT(scAnemosInnerSanctumPressRoundSwitch)},
    {TRIGGER_STEP, TRIGGER_FIRST_ROUND_SWITCH + 3, FLAG_ROUND_SWITCH_PRESSED(3), EVENT(scAnemosInnerSanctumPressRoundSwitch)},
    {TRIGGER_STEP, TRIGGER_FIRST_ROUND_SWITCH + 4, FLAG_ROUND_SWITCH_PRESSED(4), EVENT(scAnemosInnerSanctumPressRoundSwitch)},
    {TRIGGER_STEP, TRIGGER_FIRST_ROUND_SWITCH + 5, FLAG_ROUND_SWITCH_PRESSED(5), EVENT(scAnemosInnerSanctumPressRoundSwitch)},
    {TRIGGER_STEP, TRIGGER_FIRST_ROUND_SWITCH + 6, FLAG_ROUND_SWITCH_PRESSED(6), EVENT(scAnemosInnerSanctumPressRoundSwitch)},
    {TRIGGER_STEP, TRIGGER_FIRST_CRACKED_FLOOR, CONDITION_ALWAYS, EVENT(scAnemosInnerSanctumFallThroughFirstCracks)},
    {TRIGGER_STEP, TRIGGER_FIRST_CRACKED_FLOOR + 1, CONDITION_ALWAYS, EVENT(scAnemosInnerSanctumFallThroughFirstCracks)},
    {TRIGGER_STEP, TRIGGER_SECOND_CRACKED_FLOOR, CONDITION_ALWAYS, EVENT(scAnemosInnerSanctumFallThroughSecondCracks)},
    {TRIGGER_STEP, TRIGGER_SECOND_CRACKED_FLOOR + 1, CONDITION_ALWAYS, EVENT(scAnemosInnerSanctumFallThroughSecondCracks)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(scAnemosInnerSanctumSinkIntoSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(scAnemosInnerSanctumEndSand)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(scAnemosInnerSanctumStartMovingBlock)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(scAnemosInnerSanctumFinishMovingBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_ORIHALCON_CHEST_FIRST_PILLAR, FLAG_ANEMOS_ORIHALCON_CHEST_FIRST_PILLAR_PLACED, EVENT(scAnemosInnerSanctumStartMovingBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ORIHALCON_CHEST_FIRST_PILLAR, FLAG_ANEMOS_ORIHALCON_CHEST_FIRST_PILLAR_PLACED, EVENT(scAnemosInnerSanctumFinishMovingBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_ORIHALCON_CHEST_SECOND_PILLAR, CONDITION_ALWAYS, EVENT(scAnemosInnerSanctumStartMovingBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ORIHALCON_CHEST_SECOND_PILLAR, CONDITION_ALWAYS, EVENT(scAnemosInnerSanctumFinishMovingBlock)},
    {TRIGGER_TABLE_END},
};

TRIGGER AnemosInnerSanctum_GuardianTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_GUARDIAN_TO_ANEMOS_INNER_SANCTUM_ELEVATOR_WEST},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_GUARDIAN_TO_ANEMOS_INNER_SANCTUM_ELEVATOR_NORTHWEST},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_GUARDIAN_TO_ANEMOS_INNER_SANCTUM_SWITCHES_EAST},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_GUARDIAN_TO_ANEMOS_INNER_SANCTUM_SWITCHES_NORTHEAST},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_GUARDIAN_TO_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_EAST},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_GUARDIAN_TO_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_NORTHEAST},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 7, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_GUARDIAN_TO_ANEMOS_INNER_SANCTUM_SWITCHES_SOUTHWEST},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 9, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_GUARDIAN_TO_ANEMOS_INNER_SANCTUM_ELEVATOR_EAST},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_GUARDIAN_TO_ANEMOS_INNER_SANCTUM_DRAGON_SKIN_CHEST},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_STEP, TRIGGER_MIMIC_STONE_START, FLAG_MIMIC_STONE_STARTED, EVENT(scAnemosInnerSanctumStartMimicStone)},
    {TRIGGER_STEP, TRIGGER_MIMIC_STONE_HOLE, FLAG_MIMIC_STONE_STARTED, EVENT(scAnemosInnerSanctumStartMimicStone)},
    {TRIGGER_STEP, TRIGGER_MIMIC_STONE_BUTTON, FLAG_MIMIC_STONE_STARTED, EVENT(scAnemosInnerSanctumStartMimicStone)},
    {TRIGGER_STEP, TRIGGER_BEFORE_CHARON_SLATE, FLAG_ANEMOS_CHARON_SLATE_SPOKE, EVENT(scAnemosInnerSanctumApproachCharonSlate)},
    {TRIGGER_RAISED, TRIGGER_MIMIC_STONE_IN_HOLE, CONDITION_ALWAYS, EVENT(scAnemosInnerSanctumDropMimicStone)},
    {TRIGGER_RAISED, TRIGGER_MIMIC_STONE_ON_BUTTON, CONDITION_ALWAYS, EVENT(scAnemosInnerSanctumOpenMimicStoneDoor)},
    {TRIGGER_TABLE_END},
    /* Unused: rows past the table's end. */
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_GUARDIAN_TO_ANEMOS_INNER_SANCTUM_ELEVATOR_WEST},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_GUARDIAN_TO_ANEMOS_INNER_SANCTUM_ELEVATOR_NORTHWEST},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_GUARDIAN_TO_ANEMOS_INNER_SANCTUM_SWITCHES_EAST},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_GUARDIAN_TO_ANEMOS_INNER_SANCTUM_SWITCHES_NORTHEAST},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_GUARDIAN_TO_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_EAST},
    {TRIGGER_TABLE_END},
};

/* The north door's opening, as MAPGRID_ANIMATION_STEP rows: two steps of two */
/* frames. */
s16 AnemosInnerSanctum_NorthDoorFrames[] = {
    52, 7, 1, 2, 2, 54, 7, 1,
    2, 2, -1,
};

/* A mimic stone room's door opening, the same. */
s16 AnemosInnerSanctum_MimicStoneDoorFrames[] = {
    18, 4, 1, 2, 2, 18, 7, 1,
    2, 2, -1,
};
/* clang-format on */

/* The three mimic stone rooms' doors, by their step trigger's number. */
MAP_GRID_CELL_ANIMATION AnemosInnerSanctum_MimicStoneDoors[] = {
	{AnemosInnerSanctum_MimicStoneDoorFrames, 11, 4},
	{AnemosInnerSanctum_MimicStoneDoorFrames, 26, 4},
	{AnemosInnerSanctum_MimicStoneDoorFrames, 53, 4},
};

s32 FieldEffect_DurationScripts[] = {
	(s32)FieldEffect_ShortScript,
	(s32)FieldEffect_MediumScript,
	(s32)FieldEffect_LongScript,
};

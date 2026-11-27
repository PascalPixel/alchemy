/* @file */
/* Tundaria Tower, the frozen tower that holds the Center Prong, in seven maps
 */
/* that share this overlay (SceneId_TundariaTowerEntrance to */
/* SceneId_TundariaTowerBurstBrooch). */
/* */
/* In the first hall the middle way leads to an ice crystal with something */
/* frozen inside, and Parch dries the pools left and right of it, uncovering a
 */
/* door in each. The right door leads to fields of black ice to slide across */
/* and up to a hall with three ways: Pound flattens the ice columns on the left
 */
/* way for its chests, a black ice slide on the lower way drops to the sliding
 */
/* puzzle where the Mars Djinni Reflux appears, and the right way leads to a */
/* room where Pound and Move set two pillars against a ledge to climb. Beyond */
/* it Reveal opens a way through an icy wall, and outside Felix finds the Burst
 */
/* Brooch; Burst on the brown part of the outer wall brings its rocks down for
 */
/* a shortcut. With Burst the crystal breaks to free the Center Prong, and the
 */
/* icy wall at the end of the left door's way breaks to reach four more chests.
 */
#include "item_ids.h"
#include "macros.h"
#include "version.h"
#include "field/actor_script.h"
#include "field/call.h"
#include "field/event_inline.h"
#include "field/field_sequence.h"
#include "game/game.h"
#include "scene/falling_rocks.h"
#include "scene/scene.h"
#include "text/msg_ids.h"

void fdLeaderSlideOnIce(void);
void fdActorSetCellShape(s32 level, s32 layer, s32 owner);

#include "../field/actor_set_sprite_priority.c"

/* fdActorStartScript, through the stub this overlay's import table */
/* names for it. */
void scTundariaTowerStartEffectScript(struct FIELD_EFFECT *effect,
									  const s32 *script);

/* The Center Prong frozen in the crystal (SceneId_TundariaTowerCenterProng), */
/* an item icon (ITEM_CENTER_PRONG) until the crystal breaks. */
#define ACTOR_FROZEN_CENTER_PRONG ACTOR_FIRST_PLACED
/* The Center Prong where it falls when the crystal breaks, glinting */
/* (scTundariaTowerSparkleCenterProng). */
#define ACTOR_FALLEN_CENTER_PRONG (FIELD_TREASURE_ACTOR + 4)
/* Reflux, the Mars Djinni (SceneId_TundariaTowerReflux). */
#define ACTOR_REFLUX ACTOR_FIRST_PLACED
/* The two stone pillars on the ledge in the room of pillars */
/* (SceneId_TundariaTowerPillarPuzzle), which Move pulls down and a push sets */
/* against the ledge as steps; the second is first in the table. The two that */
/* slide over the ice there, and the ice columns before them. */
#define ACTOR_SECOND_LEDGE_PILLAR	(ACTOR_FIRST_PLACED + 4)
#define ACTOR_FIRST_LEDGE_PILLAR	(ACTOR_FIRST_PLACED + 5)
#define ACTOR_FIRST_SLIDING_PILLAR	(ACTOR_FIRST_PLACED + 3)
#define ACTOR_SECOND_SLIDING_PILLAR (ACTOR_FIRST_PLACED + 6)
#define ACTOR_FIRST_ICE_COLUMN		ACTOR_FIRST_PLACED
/* The stone pillar in the Burst Brooch's room. */
#define ACTOR_BROOCH_ROOM_PILLAR ACTOR_FIRST_PLACED

/* The ways between two parts of one map: each leads to its partner's */
/* entrance and back. */
typedef enum TUNDARIA_TOWER_WAY {
	/* The Center Prong's first hall: its north doorway to the crystal's */
	/* chamber, and the doors behind its left and right pools */
	/* (scTundariaTowerOpenDoor), to the ends of the west and east rooms. */
	CENTER_PRONG_HALL_NORTH_DOORWAY = 2,
	CENTER_PRONG_LEFT_POOL_DOOR = 3,
	CENTER_PRONG_RIGHT_POOL_DOOR = 4,
	CENTER_PRONG_WEST_ROOM_NORTH_DOORWAY = 6,
	CENTER_PRONG_WEST_ROOM_EAST_END = 7,
	CENTER_PRONG_EAST_ROOM_WEST_END = 8,
	CENTER_PRONG_EAST_ROOM_NORTH_DOORWAY = 9,
	CENTER_PRONG_CRYSTAL_CHAMBER = 11,
	/* The north corridor's two passages south, into the back of the two chest
	 */
	/* rooms, whose front halves the west and east rooms' doorways reach. */
	CENTER_PRONG_CORRIDOR_MIDDLE_PASSAGE = 13,
	CENTER_PRONG_CORRIDOR_EAST_PASSAGE = 14,
	CENTER_PRONG_WEST_CHESTS_BACK = 15,
	CENTER_PRONG_WEST_CHESTS_FRONT = 16,
	CENTER_PRONG_EAST_CHESTS_BACK = 17,
	CENTER_PRONG_EAST_CHESTS_FRONT = 18,
	/* The Ice Slide: the feet of the icy hall west and east, to the north */
	/* doorways of the rooms below, whose feet lead up the three ladders of the
	 */
	/* ice floor at the bottom. */
	ICE_SLIDE_HALL_WEST_FOOT = 1,
	ICE_SLIDE_HALL_EAST_FOOT = 2,
	ICE_SLIDE_WEST_ROOM_DOORWAY = 3,
	ICE_SLIDE_WEST_ROOM_FOOT = 4,
	ICE_SLIDE_STAIR_ROOM_FOOT = 6,
	ICE_SLIDE_EAST_ROOM_DOORWAY = 7,
	ICE_SLIDE_EAST_ROOM_FOOT = 8,
	ICE_SLIDE_ICE_FLOOR_WEST_LADDER = 10,
	ICE_SLIDE_ICE_FLOOR_MIDDLE_LADDER = 11,
	ICE_SLIDE_ICE_FLOOR_EAST_LADDER = 12,
	/* Reflux's floor: the south-west foot of the hall of stairs, to the north
	 */
	/* doorway of the sliding puzzle's room, where Reflux waits. */
	REFLUX_HALL_SOUTHWEST_FOOT = 5,
	REFLUX_PUZZLE_ROOM_DOORWAY = 6,
	/* Where the party lands in the sliding puzzle's room from the slide. */
	REFLUX_FROM_SLIDE = 7,
	/* The ice columns' floor: the foot of the room of chests and ice columns,
	 */
	/* to the north room's east doorway; the great hall's west doorway, to the
	 */
	/* corridor's south one. */
	ICE_COLUMNS_CHEST_ROOM_FOOT = 1,
	ICE_COLUMNS_NORTH_ROOM_EAST_DOORWAY = 3,
	ICE_COLUMNS_HALL_WEST_DOORWAY = 4,
	ICE_COLUMNS_CORRIDOR_SOUTH_DOORWAY = 8,
	/* The room of pillars' floor: the foot of the room whose ice floor */
	/* reflects Felix, to the */
	/* lower corridor's middle doorway; and the hole in the floor of the room */
	/* by the Burst Brooch's south door, cell (29, 7), down a black ice slide */
	/* to the lower corridor's west end. */
	PILLAR_PUZZLE_MIRROR_ROOM_FOOT = 7,
	PILLAR_PUZZLE_CORRIDOR_DOORWAY = 8,
	PILLAR_PUZZLE_FROM_SLIDE = 10,
	PILLAR_PUZZLE_SLIDE_HOLE = 30
} TUNDARIA_TOWER_WAY;

#include "../field/field_effect_move.c"
#define EFFECT_SET_MODE	  fdActorSelectAnimation
#define EFFECT_SET_SCRIPT scTundariaTowerStartEffectScript
#include "../field/field_effect_spawn.c"

#undef EFFECT_SET_SCRIPT

#include "shared/tundaria_tower.h"
#include "shared/event_check_spheres_overlap.h"

/* Parch dried the pool left of the middle way. */
#define FLAG_LEFT_WATER_PARCHED 0x942
/* Parch dried the pool right of the middle way. */
#define FLAG_RIGHT_WATER_PARCHED 0x943
#define FLAG_CRYSTAL_BROKEN                                                    \
	0x944 /* Burst broke the crystal round the prong                           \
		   */
/* The second ledge pillar dropped beside the first: the steps are made. */
#define FLAG_SECOND_LEDGE_PILLAR_SET 0x946
/* A ledge pillar dropped to the ledge's foot. */
#define FLAG_FIRST_LEDGE_PILLAR_SET 0x947
/* Burst opened the brown wall outside, in the Burst Brooch's room. */
#define FLAG_BROWN_WALL_BURST 0x948
/* Burst broke the icy wall at the end of the left door's way, in the room */
/* whose ice floor reflects Felix. */
#define FLAG_MIRROR_WALL_BURST 0x94b
#define FLAG_REFLUX_APPEARED   FLAG_SCENE_TEMP_00
#define FLAG_FIGHTING_REFLUX   FLAG_SCENE_TEMP_01
/* The first of four: Pound flattened ice column n. */
#define FLAG_ICE_COLUMN_POUNDED FLAG_SCENE_TEMP_00
/* Reveal shows the hidden way through the icy wall at the dead end beyond the
 */
/* room of pillars. */
#define FLAG_HIDDEN_WAY_REVEALED FLAG_SCENE_TEMP_12

/* How far a standing ice column's top rises over the ground under it, in */
/* height steps of 8 pixels. */
#define ICE_COLUMN_HEIGHT_STEPS 4

/* A pillar's starting cell, kept in its timer while it slides: its column */
/* over its row's byte. */
#define START_CELL_ROW_MASK 0xff

/* The tower's own cell behaviors, the cells its step and Psynergy triggers */
/* name; a cell's attributes hold its behavior in their high byte */
/* (scMapCellSetAttributes). */
typedef enum TUNDARIA_TOWER_CELL_BEHAVIOR {
	/* Where the leader's slide over the ice ends */
	/* (scTundariaTowerStopSliding). */
	CELL_BEHAVIOR_TUNDARIA_TOWER_SLIDE_END = 31,
	/* Black ice, slid across (scTundariaTowerSlideOnIce). */
	CELL_BEHAVIOR_TUNDARIA_TOWER_ICE = 50,
	/* In the Burst Brooch's room, the cell where Burst opens the brown wall */
	/* (scTundariaTowerBurstBrownWall). */
	CELL_BEHAVIOR_TUNDARIA_TOWER_BROWN_WALL = 50,
	/* The step on which Reflux appears (scTundariaTowerRefluxAppears). */
	CELL_BEHAVIOR_TUNDARIA_TOWER_REFLUX_APPEARS = 51,
	/* The first hall's doors behind the left and right pools, each the number
	 */
	/* of the exit it leads through (scTundariaTowerOpenDoor). */
	CELL_BEHAVIOR_TUNDARIA_TOWER_LEFT_DOOR = CENTER_PRONG_LEFT_POOL_DOOR,
	CELL_BEHAVIOR_TUNDARIA_TOWER_RIGHT_DOOR = CENTER_PRONG_RIGHT_POOL_DOOR,
	/* The first hall's left and right pools, which Parch dries. */
	CELL_BEHAVIOR_TUNDARIA_TOWER_LEFT_POOL = 50,
	CELL_BEHAVIOR_TUNDARIA_TOWER_RIGHT_POOL = 51,
	/* The crystal holding the Center Prong, and the icy wall Burst breaks at */
	/* the end of the left door's way. */
	CELL_BEHAVIOR_TUNDARIA_TOWER_CRYSTAL = 52,
	CELL_BEHAVIOR_TUNDARIA_TOWER_ICY_WALL = 52,
	/* The top of the black ice slide down to Reflux's sliding puzzle */
	/* (scTundariaTowerJumpDownSlide). */
	CELL_BEHAVIOR_TUNDARIA_TOWER_SLIDE_TOP = 11
} TUNDARIA_TOWER_CELL_BEHAVIOR;

void scTundariaTowerAdvanceIceChip(FIELD_OBJECT *object);
void scTundariaTowerScatterIceChips(VECTOR3 *position, s32 count, FIXED scale);
void btParticleUpdateFalling(void);
void scTundariaTowerStartFallingRocks(void);
void scTundariaTowerEndFallingRocks(void);
void scTundariaTowerUpdateBrownWallCell(void);
void scTundariaTowerSparkleCenterProng(ACTOR *actor);
extern s16 TundariaTowerPillarPuzzlePillars[];
extern s16 TundariaTowerBurstBroochPillar[];
extern TRIGGER TundariaTowerEntranceTriggers[];
extern TRIGGER TundariaTowerCenterProngTriggers[];
extern TRIGGER TundariaTowerIceSlideTriggers[];
extern TRIGGER TundariaTowerRefluxTriggers[];
extern TRIGGER TundariaTowerIceColumnsTriggers[];
extern TRIGGER TundariaTowerPillarPuzzleTriggers[];
extern TRIGGER TundariaTowerBurstBroochTriggers[];

/* Talking to Reflux, once he has hopped down before the party, starts his */
/* battle. */
void scTundariaTowerFightReflux(void) {
	syGameFlagSet(FLAG_FIGHTING_REFLUX);
	fdEventRequestDjinnBattle(ACTOR_REFLUX, DJINNI_REFLUX);
}

#include "../field/actor_wait_until_landed.c"

#include "../field/map_cell_get.c"

#include "shared/map_cell_set.c"

/* Makes the ground under an ice column match it, by its Pound flag (kept in */
/* its timer): a standing column blocks the way and Felix can jump onto its */
/* top, ICE_COLUMN_HEIGHT_STEPS up; one pounded flat is walked over and drawn */
/* beneath him. */
void scTundariaTowerSetColumnCell(ACTOR *actor) {
	s32 x = FIELD_CELL(actor->x.fixed);
	s32 z = FIELD_CELL(actor->z.fixed);
	s32 attr0 = fdMapCellGetAttributes(MAPGRID_LAYER_GROUND, x, z);
	s32 attr2 = fdMapCellGetAttributes(MAPGRID_LAYER_OVERHEAD, x, z);
	s32 height = CELL_HEIGHT_STEPS(
		fdMapGetTerrainHeight(actor->layer, actor->x.fixed, actor->z.fixed));

	if(syGameFlagGet(actor->tmr)) {
		attr2 &= ~CELL_ATTRIBUTES(CELL_BEHAVIOR_WALL, 0);
		attr0 &= ~MAP_CELL_JUMP;
		if(CELL_HEIGHT_STEPS(actor->groundHeight) != height)
			height -= ICE_COLUMN_HEIGHT_STEPS;
		scSetActorSprPriority(actor, 3);
	}
	else {
		attr2 =
			CELL_ATTRIBUTES(CELL_BEHAVIOR_WALL, attr2 & CELL_ATTRIBUTE_BITS);
		attr0 |= MAP_CELL_JUMP;
		height += ICE_COLUMN_HEIGHT_STEPS;
		scSetActorSprPriority(actor, 2);
	}
	scMapCellSetAttributes(MAPGRID_LAYER_GROUND, x, z, attr0);
	scMapCellSetAttributes(MAPGRID_LAYER_OVERHEAD, x, z, attr2);
	fdMapGridSetCellHeight(FIELD_CELL_ORIGIN(x), FIELD_CELL_ORIGIN(z),
						   actor->layer, height);
}

/* Sets each listed ice column, by its row of (actor, animation, flag), */
/* standing or pounded flat as its flag says (scTundariaTowerSetColumnCell). */
void scTundariaTowerSetUpIceColumns(u16 *table) {
	u16 *row;
	FIELD_ACTOR *object;
	s16 flag;
	s16 animation;

	if(table[0] == ACTOR_LIST_END)
		return;
	for(row = table; row[0] != ACTOR_LIST_END; row += 3) {
		object = ((FIELD_ACTOR *)fdActorGetById(row[0]));
		animation = row[1];
		flag = row[2];

		fdActorSetSprFlags(object, 0);
		fdActorSelectAnimation(object,
							   (u16)animation + syGameFlagGet((u16)flag));
		object->motionFlg = 0;
		object->collisionFlg &= ~ACTOR_SOLID;
		object->tmr = flag;
		scTundariaTowerSetColumnCell(object);
	}
}

const SCENE_ENTRANCE *scGetEntrances(void) { return SceneEntrances; }

extern SCENE_REGION TundariaTowerPillarPuzzleExitRegions[];
extern SCENE_REGION TundariaTowerBurstBroochExitRegions[];

const SCENE_REGION *scGetExitRegions(void) {
	if(MainRoutineStateWork.next == SCENE_TUNDARIA_TOWER_PILLAR_PUZZLE)
		return TundariaTowerPillarPuzzleExitRegions;
	if(MainRoutineStateWork.next == SCENE_TUNDARIA_TOWER_BURST_BROOCH)
		return TundariaTowerBurstBroochExitRegions;
	return 0;
}

const u32 *scGetExits(void) { return SceneExits; }

extern ACTOR_PLACE TundariaTowerCenterProngActorPlacements[];
extern ACTOR_PLACE TundariaTowerIceSlideActorPlacements[];
extern ACTOR_PLACE TundariaTowerRefluxActorPlacements[];
extern ACTOR_PLACE TundariaTowerIceColumnsActorPlacements[];
extern ACTOR_PLACE TundariaTowerPillarPuzzleActorPlacements[];
extern ACTOR_PLACE TundariaTowerBurstBroochActorPlacements[];

const TRIGGER *scGetActorPlacements(void) {
	if(MainRoutineStateWork.next == SCENE_TUNDARIA_TOWER_CENTER_PRONG)
		return (const TRIGGER *)TundariaTowerCenterProngActorPlacements;
	if(MainRoutineStateWork.next == SCENE_TUNDARIA_TOWER_ICE_SLIDE)
		return (const TRIGGER *)TundariaTowerIceSlideActorPlacements;
	if(MainRoutineStateWork.next == SCENE_TUNDARIA_TOWER_REFLUX)
		return (const TRIGGER *)TundariaTowerRefluxActorPlacements;
	if(MainRoutineStateWork.next == SCENE_TUNDARIA_TOWER_ICE_COLUMNS)
		return (const TRIGGER *)TundariaTowerIceColumnsActorPlacements;
	if(MainRoutineStateWork.next == SCENE_TUNDARIA_TOWER_PILLAR_PUZZLE)
		return (const TRIGGER *)TundariaTowerPillarPuzzleActorPlacements;
	if(MainRoutineStateWork.next == SCENE_TUNDARIA_TOWER_BURST_BROOCH)
		return (const TRIGGER *)TundariaTowerBurstBroochActorPlacements;
	return (const TRIGGER *)TundariaTowerCenterProngActorPlacements;
}

/* Parch on the pool left of the first hall's middle way dries it, uncovering */
/* a green door in the wall behind it. */
void scTundariaTowerParchLeftWater(void) {
	fdEventStart();
	fdMapGridCopyCellAttributes(19, 55, 7, 8, 24, 46);
	fdMapGridCopyCellsTo(24, 102, 24, 110, 7, 8);
	syGameFlagSet(FLAG_LEFT_WATER_PARCHED);
	fdEventEnd();
}

/* Parch on the pool right of the first hall's middle way dries it, */
/* uncovering a door in the wall behind it. */
void scTundariaTowerParchRightWater(void) {
	fdEventStart();
	fdMapGridCopyCellAttributes(37, 55, 7, 8, 32, 46);
	fdMapGridCopyCellsTo(32, 102, 32, 110, 7, 8);
	syGameFlagSet(FLAG_RIGHT_WATER_PARCHED);
	fdEventEnd();
}

/* Walking north into the door behind one of the first hall's dried pools */
/* opens it, and Felix walks through: the left door leads to the way that */
/* ends at an icy wall, the right one to the black ice fields. */
void scTundariaTowerOpenDoor(void) {
	FIELD_STATE *fieldState = Ram_HeapSlots->fieldState;
	s16 *choice;
	s32 x = 37;
	MAIN_ROUTINE_STATE *state;
	s32 cell;

	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	choice = &fieldState->stepTrigger;
	if(*choice == CELL_BEHAVIOR_TUNDARIA_TOWER_LEFT_DOOR)
		x = 25;
	fdEventPlaySnd(SE_DOOR_OPEN);
	fdMapGridCopyCellsTo(30, 38, x, 49, 1, 2);
	syFieldEventWait(10);
	fdMapGridCopyCellsTo(32, 38, x, 49, 1, 2);
	syFieldEventWait(10);
	state = &MainRoutineStateWork;
	fdActorSetSpeed(state->leaderActorId, 0.5);
	fdActorGetById(state->leaderActorId)->motionFlg = 0;
	cell = FIELD_HALF_CELL(fdActorGetById(state->leaderActorId)->x.fixed) | 1;
	fdActorWalkToAndWait(
		state->leaderActorId, cell << 3,
		FIXEDtoINT(fdActorGetById(state->leaderActorId)->z.fixed) | 1);
	fdEventPlaySnd(SE_MAP_EXIT);
	cell = fdActorGetById(state->leaderActorId)->x.fixed;
	fdActorWalkTo(
		state->leaderActorId, FIELD_HALF_CELL(cell) << 3,
		(FIXEDtoINT(fdActorGetById(state->leaderActorId)->z.fixed) | 1) - 16);
	syFieldEventWait(5);
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	fdActorWaitForMoveById(state->leaderActorId);
	syImportEventSetExitRequest(*choice);
	fdEventEnd();
}

/* Each frame of a chip of ice: it moves and scales by its rates and falls. */
void scTundariaTowerAdvanceIceChip(FIELD_OBJECT *object) {
	object->eff.x += object->eff.vel.x;
	object->eff.y += object->eff.vel.y;
	object->eff.z += object->eff.vel.z;
	object->eff.scaleX += object->eff.scaleRateX;
	object->eff.scaleY += object->eff.scaleRateY;
	object->eff.vel.y -= toFIXED(3) / 10;
}

/* Throws chips of ice (SPRITE_BLUE_CRYSTAL) off a point, each turned at */
/* random, to fall under scTundariaTowerAdvanceIceChip. */
void scTundariaTowerScatterIceChips(VECTOR3 *position, s32 count, FIXED scale) {
	EFFECT_OPTIONS options;
	s32 i;
	s32 velocityX;
	s32 velocityY;
	FIXED x;

	options.pri = 1;
	options.type = SPRITE_BLUE_CRYSTAL;
	options.tgtScaleY = FIXED_ONE * 8 / 10;
	options.tgtScaleX = FIXED_ONE * 8 / 10;
	options.startScaleY = scale;
	options.startScaleX = scale;
	options.update = scTundariaTowerAdvanceIceChip;
	for(i = 0; i != count; i++) {
		options.rotation = (syRandom() & 15) << 12;
		velocityX = syRandom() & 15;
		velocityX -= 8;
		velocityX *= toFIXED(1) / 4;
		velocityY = syRandom() & 15;
		velocityY += 8;
		velocityY *= toFIXED(1) / 4;
		x = position->x - INTtoFIXED(syRandom() & 31) + toFIXED(15);
		scFieldEffectSpawn(x, position->y + INTtoFIXED(syRandom() & 15),
						   position->z, velocityX, velocityY, 0,
						   EFFECT_USE_UPDATE | EFFECT_USE_ROTATION |
							   EFFECT_USE_TYPE | EFFECT_USE_START_SCALE |
							   EFFECT_SCALE_TO_TARGET | EFFECT_USE_PRIORITY,
						   &options);
	}
}

/* Burst on the ice crystal in the Center Prong's chamber, cell (21, 23): it */
/* cracks and throws off chips of ice. */
void scTundariaTowerBurstCrystal(s32 step) {
	VECTOR3 position;

	switch(step) {
	case 1:
		break;
	case 2:
		position.x = toFIXED(347);
		position.y = 0;
		position.z = toFIXED(376);
		scTundariaTowerScatterIceChips(&position, 1, FIXED_ONE * 8 / 10);
		break;
	case 3:
		fdMapGridCopyCellTilesIndicesRect(31, 85, 20, 85, 3, 4);
		position.x = toFIXED(347);
		position.y = 0;
		position.z = toFIXED(376);
		scTundariaTowerScatterIceChips(&position, 8, FIXED_ONE * 3 / 2);
		break;
	}
}

/* The fallen Center Prong's update: a sparkle near it every eighth frame. */
void scTundariaTowerSparkleCenterProng(ACTOR *actor) {
	EFFECT_OPTIONS params;
	u32 phase = FrameCount & 7;
	s32 dx;
	s32 dy;

	if(phase == 0) {
		dx = (syRandom() & 15) - 8;
		dy = syRandom() & 7;
		params.type = SPRITE_SPARKLE;
		scFieldEffectSpawn(actor->x.fixed + (INTtoFIXED(dx)),
						   actor->y.fixed + (INTtoFIXED(dy - 8)),
						   actor->z.fixed, 0, toFIXED(1) / 2, phase,
						   EFFECT_USE_TYPE, &params);
	}
}

/* As Burst ends, the crystal is broken and the Center Prong drops from it to */
/* the floor, where it glints until Felix picks it up. */
void scTundariaTowerFreeCenterProng(void) {
	ACTOR *actor = fdActorGetById(ACTOR_FROZEN_CENTER_PRONG);

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	syGameFlagSet(FLAG_CRYSTAL_BROKEN);
	fdMapGridCopyCellAttributes(11, 22, 3, 2, 20, 22);
	actor->gravity = toFIXED(1) / 2;
	actor->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	fdActorWaitUntilLanded(actor);
	fdActorSetPos(ACTOR_FROZEN_CENTER_PRONG, 0, 0);
	fdActorSetPos(ACTOR_FALLEN_CENTER_PRONG, 344, 372);
	fdActorGetById(ACTOR_FALLEN_CENTER_PRONG)->update =
		(OBJECT_UPDATE)scTundariaTowerSparkleCenterProng;
	fdEventEnd();
}

/* Searching the ice crystal in the Center Prong's chamber, cell (21, 23), */
/* says "There's something inside the cracked ice...". Once */
/* FLAG_TUNDARIA_TOWER_BURST_BROOCH is set, the ordinary field menu opens by */
/* itself as the message closes (`fieldMenu`): the party's HP and PP at the */
/* top right and the command icons at the bottom, Psynergy picked, waiting for
 */
/* the player. */
void scTundariaTowerExamineCrystal(void) {
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	mnFieldMessageShow(MsgThereSSomething, 1);
	if(syGameFlagGet(FLAG_TUNDARIA_TOWER_BURST_BROOCH)) {
		FIELD_STATE *state = Ram_HeapSlots->fieldState;

		state->fieldMenu = 1;
	}
	fdEventEnd();
}

/* Stepping onto black ice sets Felix sliding straight on until something */
/* stops him. */
void scTundariaTowerSlideOnIce(void) { fdLeaderSlideOnIce(); }

extern s32 TundariaTower_RefluxSpinScript[];

/* Sliding onto cell (21, 33) in Reflux's sliding puzzle: the screen flashes */
/* white with a splash and a shake, and Reflux hops down before the party and */
/* waits to be talked to. */
void scTundariaTowerRefluxAppears(void) {
	ACTOR *actor = fdActorGetById(ACTOR_REFLUX);

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	scTundariaTowerFadeFrom(FIXED_ONE, 0);
	fdPaletteFadeSetTarget(RGB_WHITE, 0);
	fdPaletteFadeStartInterpolation(2);
	sySndPlay(SE_WATER);
	fdMapGridSetShake(5, 7, 1);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	fdEventWait(2);
	fdPaletteFadeSetTarget(FIXED_ONE, 0);
	fdPaletteFadeStartInterpolation(2);
	actor->steeringFlg = 0;
	actor->update = 0;
	fdActorSetDestination(ACTOR_REFLUX, 424, 536);
	scTundariaTowerStartEffectScript(actor, TundariaTower_RefluxSpinScript);
	fdActorWaitForMoveById(ACTOR_REFLUX);
	syGameFlagSet(FLAG_REFLUX_APPEARED);
	fdActorWatchAndSetScript(ACTOR_REFLUX, MainRoutineStateWork.leaderActorId);
	scMapCellSetAttributes(
		MAPGRID_LAYER_GROUND, 21, 33,
		CELL_ATTRIBUTES(CELL_BEHAVIOR_TUNDARIA_TOWER_ICE, 0));
	scMapCellSetAttributes(
		MAPGRID_LAYER_GROUND, 22, 33,
		CELL_ATTRIBUTES(CELL_BEHAVIOR_TUNDARIA_TOWER_ICE, 0));
	scMapCellSetAttributes(MAPGRID_LAYER_GROUND, 26, 33,
						   CELL_ATTRIBUTES(CELL_BEHAVIOR_WALL, 0));
	fdEventEnd();
}

/* Pound on an ice column: chips of ice fly off as it is squashed, and it */
/* ends flat in the floor, to be walked over. */
void scTundariaTowerShrinkIceColumn(s32 phase, s32 step) {
	EFFECT_OPTIONS options;
	FIELD_EFFECT_STATE *effectState = Ram_HeapSlots->fieldEffState;
	ACTOR *actor = fdActorGetById(effectState->tgtId);
	s32 velocityX;

	if(step < 8) {
		options.type = SPRITE_BLUE_CRYSTAL;
		options.startScaleY = FIXED_ONE;
		options.startScaleX = FIXED_ONE;
		options.pri = 0;
		options.tgtScaleY = FIXED_ONE / 2;
		options.tgtScaleX = FIXED_ONE / 2;
		options.rotation = (syRandom() & 15) << 12;
		velocityX = ((syRandom() & 15) - 8) << 13;
		scFieldEffectSpawn(actor->x.fixed,
						   actor->y.fixed - INTtoFIXED(step * 3) + toFIXED(16),
						   actor->z.fixed, velocityX, 0, 0,
						   EFFECT_USE_ROTATION | EFFECT_USE_TYPE |
							   EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY,
						   &options);
		actor->scaleY -= FIXED_ONE * 31 / 256;
	}
	if(phase == 1) {
		actor->scaleY = FIXED_ONE;
		syGameFlagSet(actor->tmr);
		scTundariaTowerSetColumnCell(fdActorGetById(effectState->tgtId));
		fdMapGridCopyCellTilesIndicesRect(67, 23,
										  (FIELD_CELL(actor->x.fixed)) + 64,
										  FIELD_CELL(actor->z.fixed), 1, 1);
		actor->z.fixed = 0;
		actor->y.fixed = 0;
		actor->x.fixed = 0;
	}
}

/* Pound on one of the ice columns, on the hall's left way or in the room of */
/* pillars, flattens it into the floor. */
void scTundariaTowerPoundIceColumn(s32 phase, s32 step) {
	scTundariaTowerShrinkIceColumn(phase, step);
}

/* Stepping onto the top of the black ice slide on the lower way from the */
/* hall of three ways, cell (19, 57): Felix leans back, then runs and goes */
/* down the slide to Reflux's sliding puzzle. */
void scTundariaTowerJumpDownSlide(void) {
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdActorSetSprFlags(fdActorGetById(MainRoutineStateWork.leaderActorId), 0);
	leader->facing = FACING_SOUTH;
	fdEventWait(10);
	fdActorSetAnimationById(MainRoutineStateWork.leaderActorId,
							ANIM_LEADER_LEAN_BACK);
	fdEventWait(30);
	fdEffectSetAttached(MainRoutineStateWork.leaderActorId,
						ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorRunRepeatedMotion(MainRoutineStateWork.leaderActorId, 2);
	fdEventWait(20);
	leader->facing = FACING_NORTH;
	fdActorSetAnimationById(MainRoutineStateWork.leaderActorId,
							ANIM_LEADER_RUN);
	fdActorSetAnimationSpeedById(MainRoutineStateWork.leaderActorId,
								 ANIM_STEP_NORMAL * 3 / 2);
	fdEventWait(40);
	fdMapGridSetCellHeight(FIELD_CELL_ORIGIN(19), FIELD_CELL_ORIGIN(57), 0,
						   -12);
	fdActorGetById(MainRoutineStateWork.leaderActorId)->bounce = 0;
	syImportEventSetExitRequest(
		TUNDARIA_TOWER_ICE_COLUMNS_TO_TUNDARIA_TOWER_REFLUX_SOUTH);
	fdEventEnd();
}

/* As Felix starts to push or Move a stone pillar in the room of pillars or the
 */
/* Burst Brooch's room, it is taken off the map grid; in the room of pillars */
/* the cell it started from is kept, so that Move can send it on sliding the */
/* same way. */
void scTundariaTowerStartPillarMove(void) {
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	MAIN_ROUTINE_STATE *party = &MainRoutineStateWork;

	if(party->next == SCENE_TUNDARIA_TOWER_PILLAR_PUZZLE) {
		ACTOR *actor;

		fdActorLiftTargetBlock(TundariaTowerPillarPuzzlePillars);
		actor = state->movedBlock;
		actor->tmr = ((FIELD_CELL(actor->x.fixed)) << 8) |
					 ((FIELD_CELL(actor->z.fixed)) & START_CELL_ROW_MASK);
	}
	else if(party->next == SCENE_TUNDARIA_TOWER_BURST_BROOCH) {
		fdActorLiftTargetBlock(TundariaTowerBurstBroochPillar);
	}
}

/* Move on one of the two stone pillars on the ice in the room of pillars */
/* sends it sliding on, a cell at a time, until the ice ends or a wall stops */
/* it, the camera following. */
void scTundariaTowerSlidePillar(s32 unused, s32 actorArg) {
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	ACTOR *actor;
	u16 actorId;
	s32 dx;
	s32 dz;

	actorId = actorArg;
	actor = state->movedBlock;
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	dx = (FIELD_CELL(actor->x.fixed)) - (actor->tmr >> 8);
	dz = (FIELD_CELL(actor->z.fixed)) - (actor->tmr & START_CELL_ROW_MASK);
	actor->speedLimit = toFIXED(1) / 2;
	while((fdMapCellGetAttributes(MAPGRID_LAYER_MIDDLE,
								  (FIELD_CELL(actor->x.fixed)) + dx,
								  (FIELD_CELL(actor->z.fixed)) + dz) >>
		   8) == CELL_BEHAVIOR_TUNDARIA_TOWER_ICE &&
		  (fdMapCellGetAttributes(MAPGRID_LAYER_OVERHEAD,
								  (FIELD_CELL(actor->x.fixed)) + dx,
								  (FIELD_CELL(actor->z.fixed)) + dz) >>
		   8) != CELL_BEHAVIOR_WALL) {
		fdActorSetMoveTarget(actor, INTtoFIXED(dx * 16) + actor->x.fixed,
							 actor->y.fixed,
							 INTtoFIXED(dz * 16) + actor->z.fixed);
		fdCameraFollowActor(actorId, 1);
		fdActorWaitForMove(actor);
		sySynch(1);
	}
	actor->x.fixed =
		FIELD_HALF_CELL_ORIGIN(FIELD_HALF_CELL(actor->x.fixed) | 1);
	actor->z.fixed =
		FIELD_HALF_CELL_ORIGIN(FIELD_HALF_CELL(actor->z.fixed) | 1);
	fdActorPlaceMovedBlock();
	fdEventEnd();
}

/* A pillar pushed or Moved off the edge of a ledge drops to the floor below */
/* with a thump. In the room of pillars the two pillars from the ledge, set */
/* one beside the other at its foot, make the steps up it. */
void scTundariaTowerSetDownPillar(void) {
	FIELD_STATE *event = Ram_HeapSlots->fieldState;
	ACTOR *actor;

	actor = event->movedBlock;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	if(fdMapGetTerrainHeight(2, actor->x.fixed, actor->z.fixed) !=
	   actor->y.fixed) {
		actor->layer = 2;
		actor->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
		fdActorWaitUntilLanded(actor);
		sySndPlay(SE_THUMP);
		fdActorWaitUntilLanded(actor);
		actor->motionFlg = 0;
		actor->layer = 0;
	}
	fdActorPlaceMovedBlock();
	if(MainRoutineStateWork.next == SCENE_TUNDARIA_TOWER_PILLAR_PUZZLE) {
		if(FIELD_HALF_CELL(actor->x.fixed) == 27)
			syGameFlagSet(FLAG_FIRST_LEDGE_PILLAR_SET);
		if(FIELD_HALF_CELL(actor->x.fixed) == 25 &&
		   syGameFlagGet(FLAG_FIRST_LEDGE_PILLAR_SET))
			syGameFlagSet(FLAG_SECOND_LEDGE_PILLAR_SET);
	}
	fdEventEnd();
}

/* When Reveal wears off, the way through the icy wall at the dead end beyond */
/* the room of pillars closes again; Felix, if he stands in it, is put out on */
/* the side he faces. */
void scTundariaTowerEndRevealHiddenWay(void) {
	fdBlendScriptResume();
	syGameFlagClear(FLAG_HIDDEN_WAY_REVEALED);
	/* The icy wall's part of the map is entered by 6, from the Burst Brooch's
	 */
	/* room, or 7. */
	if(MainRoutineStateWork.entrance >= 6 &&
	   MainRoutineStateWork.entrance <= 7) {
		fdMapGridCopyCellTilesIndicesRect(39, 100, 53, 100, 3, 3);
		fdMapGridCopyCellAttributes(43, 41, 3, 3, 53, 37);
		if(FIELD_CELL(
			   fdActorGetById(MainRoutineStateWork.leaderActorId)->z.fixed) ==
		   38) {
			s16 angle =
				fdActorGetById(MainRoutineStateWork.leaderActorId)->facing;
			s32 owner = MainRoutineStateWork.leaderActorId;

			if(angle < 0)
				fdActorSetPos(owner, 872, 600);
			else
				fdActorSetPos(owner, 872, 632);
		}
	}
}

/* Reveal at the icy wall at the dead end beyond the room of pillars shows a */
/* way through it, cells (53, 37) to (55, 39). */
void scTundariaTowerRevealHiddenWay(void) {
	fdBlendScriptSuspend();
	syGameFlagSet(FLAG_HIDDEN_WAY_REVEALED);
	if(MainRoutineStateWork.entrance >= 6 &&
	   MainRoutineStateWork.entrance <= 7) {
		fdMapGridCopyCellTilesIndicesRect(43, 100, 53, 100, 3, 3);
		fdMapGridCopyCellAttributes(43, 37, 3, 3, 53, 37);
		if(FIELD_CELL(
			   fdActorGetById(MainRoutineStateWork.leaderActorId)->z.fixed) >
		   38)
			scMapCellSetAttributes(
				0, 54, 37,
				CELL_ATTRIBUTES(CELL_BEHAVIOR_TUNDARIA_TOWER_SLIDE_END, 0));
		else
			scMapCellSetAttributes(
				0, 54, 39,
				CELL_ATTRIBUTES(CELL_BEHAVIOR_TUNDARIA_TOWER_SLIDE_END, 0));
	}
}

/* The cells where the ice ends in the room of pillars stop Felix sliding. */
void scTundariaTowerStopSliding(void) { fdEventEndLastingPsynergy(0); }

/* Burst on the icy wall at the end of the left door's way, cell (7, 35), in */
/* the room whose ice floor reflects Felix: chips of ice fly and it breaks */
/* open onto the rooms of chests beyond. */
void scTundariaTowerBurstMirrorWall(s32 step) {
	VECTOR3 position;

	switch(step) {
	case 1:
		fdMapGridCopyCellTilesIndicesRect(18, 97, 6, 97, 3, 4);
		fdMapGridCopyCellTilesIndicesRect(82, 34, 70, 34, 3, 2);
		fdMapGridCopyCellAttributes(7, 36, 1, 1, 7, 35);
		syGameFlagSet(FLAG_MIRROR_WALL_BURST);
		break;
	case 2:
		position.x = toFIXED(120);
		position.y = 0;
		position.z = toFIXED(568);
		scTundariaTowerScatterIceChips(&position, 1, FIXED_ONE * 8 / 10);
		break;
	case 3:
		position.x = toFIXED(120);
		position.y = 0;
		position.z = toFIXED(568);
		scTundariaTowerScatterIceChips(&position, 8, FIXED_ONE * 3 / 2);
		break;
	}
}

/* As Felix starts to cast Move on a sliding pillar or Burst on the icy wall, */
/* whatever he was doing, such as sliding, stops first. */
void scTundariaTowerResolveBeforePsynergy(void) {
	fdEventEndLastingPsynergy(0);
}

/* Burst on the brown part of the outer wall in the Burst Brooch's room, from */
/* the ground below it: rocks fall as it breaks open into a shortcut. */
void scTundariaTowerBurstBrownWall(s32 shake, s32 frame) {
	if(frame == 0) {
		scTundariaTowerStartFallingRocks();
		syCloseEvent(scTundariaTowerUpdateBrownWallCell);
	}
	if(shake == 1) {
		fdMapGridCopyCellTilesIndicesRect(13, 26, 13, 16, 3, 3);
		fdMapGridCopyCellAttributes(13, 26, 3, 3, 13, 16);
		syGameFlagSet(FLAG_BROWN_WALL_BURST);
	}
	if(shake == 2)
		syRockSpawn(toFIXED(232), toFIXED(16), toFIXED(280), 2);
	if(shake == 3)
		syRockSpawn(toFIXED(232), toFIXED(16), toFIXED(280), 30);
	if(frame == FIELD_PSYNERGY_FINISHED)
		scTundariaTowerEndFallingRocks();
}

const TRIGGER *scGetTriggers(void) {
	if(MainRoutineStateWork.next == SCENE_TUNDARIA_TOWER_ENTRANCE)
		return TundariaTowerEntranceTriggers;
	if(MainRoutineStateWork.next == SCENE_TUNDARIA_TOWER_CENTER_PRONG)
		return TundariaTowerCenterProngTriggers;
	if(MainRoutineStateWork.next == SCENE_TUNDARIA_TOWER_ICE_SLIDE)
		return TundariaTowerIceSlideTriggers;
	if(MainRoutineStateWork.next == SCENE_TUNDARIA_TOWER_REFLUX)
		return TundariaTowerRefluxTriggers;
	if(MainRoutineStateWork.next == SCENE_TUNDARIA_TOWER_ICE_COLUMNS)
		return TundariaTowerIceColumnsTriggers;
	if(MainRoutineStateWork.next == SCENE_TUNDARIA_TOWER_PILLAR_PUZZLE)
		return TundariaTowerPillarPuzzleTriggers;
	if(MainRoutineStateWork.next == SCENE_TUNDARIA_TOWER_BURST_BROOCH)
		return TundariaTowerBurstBroochTriggers;
	return TundariaTowerCenterProngTriggers;
}

/* Felix's reflection in the ice floor of two rooms, the one before the icy */
/* wall Burst breaks and the one before the icy wall Reveal opens, mirrored */
/* across the floor's edge. */
void scTundariaTowerReflectLeader(ACTOR *actor) {
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	SPR *leaderSprite = leader->spr;
	SPR *reflection = actor->spr;

	if(leader->z.fixed <= actor->sceneWord) {
		actor->z.fixed = 0;
		actor->x.fixed = 0;
		REG_BG2CNT &= (u16)~BGCNT_PRIORITY(3);
		REG_BG2CNT |= BGCNT_PRIORITY(1);
	}
	else {
		REG_BG2CNT &= (u16)~BGCNT_PRIORITY(3);
		REG_BG2CNT |= BGCNT_PRIORITY(2);
		if(syGameFlagGet(FLAG_HIDDEN_WAY_REVEALED)) {
			actor->z.fixed = 0;
			actor->x.fixed = 0;
		}
		else {
			SPR_ANIMATION *animation = reflection->entries[0];
			SPR_ANIMATION *source;

			actor->x.fixed = leader->x.fixed;
			actor->y.fixed = leader->y.fixed;
			actor->z.fixed =
				actor->sceneWord - (leader->z.fixed - actor->sceneWord);
			actor->facing = ~leader->facing;
			source = leaderSprite->entries[0];
			animation->script = source->script;
			animation->tmr = source->tmr;
			animation->scriptOfs = source->scriptOfs;
		}
	}
}

#include "../field/field_effect_advance_motion.c"

struct ACTOR;
struct PARTICLE;
struct PARTICLE_SPR;
union FIELD_OBJECT;

void fdLeaderStopIfPending(void);

void fdActorSetCellOpen(s32 id, u32 kind, s32 flag, s32 layer);

void scTundariaTowerScatterDust(s32 spread);
void fdLeaderSetCellShapeAt(s32 a, s32 b);

void scTundariaTowerSpawnDustRing(void);

ACTOR *TundariaTower_Reflection BSS_DATA;
volatile s16 SlideOnIce_Stopped BSS_DATA;

void scTundariaTowerParchLeftWater(void);
void scTundariaTowerParchRightWater(void);

extern s16 SlideOnIce_SteerAngles[];
extern s16 SlideOnIce_AnglesByKey[];

extern u32 Dust_ScatterScript[];

extern u32 Dust_BurstScript[];

void scTundariaTowerExamineCrystal(void);
void scTundariaTowerSlideOnIce(void);
void scTundariaTowerRefluxAppears(void);
void scTundariaTowerPoundIceColumn(s32 phase, s32 step);
void scTundariaTowerSetDownPillar(void);
void scTundariaTowerEndRevealHiddenWay(void);
void scTundariaTowerRevealHiddenWay(void);
void scTundariaTowerStopSliding(void);
void scTundariaTowerResolveBeforePsynergy(void);

void scFieldEffectSpawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
						s32 velocityZ, u32 flags,
						const EFFECT_OPTIONS *options);

/* At the foot of a black ice slide, Felix drops in from above and lands in a */
/* puff of dust with a thump that shakes the screen. */
void scTundariaTowerLandFromSlide(void) {
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	EFFECT_OPTIONS options;
	s32 angle;
	u32 i;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);
	leader->y.fixed = toFIXED(130);
	leader->gravity = toFIXED(1) / 2;
	leader->bounce = 0;
	leader->motionFlg = 0;
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	fdEventStartScrTransition();
	fdEventWaitTransitionFrames();
	sySndPlay(SE_DUST_PUFF);
	leader->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	fdEventWait(24);
	options.pal = 7;
	options.update = (void *)fdEffectAdvanceMotion;
	options.startScaleX = FIXED_ONE * 8 / 10;
	options.startScaleY = FIXED_ONE * 8 / 10;
	i = 0;
	do {
		FIXED velocity[3];

		angle = i * (TRIG_TURN / DUST_RING_PUFFS);
		velocity[0] = syCos(angle);
		velocity[1] = 0;
		velocity[2] = sySin(angle);
		velocity[0] += velocity[0] >> 1;
		velocity[0] += syRandomBelow(toFIXED(3) / 4) - toFIXED(3) / 8;
		velocity[2] += syRandomBelow(toFIXED(3) / 8) - toFIXED(3) / 16;
		scFieldEffectSpawn(leader->x.fixed, leader->y.fixed, leader->z.fixed,
						   velocity[0], velocity[1], velocity[2],
						   EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE |
							   EFFECT_USE_PALETTE | 1,
						   &options);
		i++;
	} while(i <= DUST_RING_PUFFS);
	sySndPlay(SE_THUMP);
	fdEffectSetAttached(MainRoutineStateWork.leaderActorId,
						ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
	fdActorSetAnimationById(MainRoutineStateWork.leaderActorId,
							ANIM_LEADER_LEAN_BACK);
	fdMapGridSetShake(5, 5, 1);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	fdMapGridWaitForShakeToSettle();
	fdEffectSetAttached(MainRoutineStateWork.leaderActorId,
						ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
	leader->gravity = toFIXED(1);
	leader->bounce = FIXED_ONE / 4;
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	fdEventEnd();
}

/* Each frame in the Burst Brooch's room, the cell before the brown wall */
/* answers Burst only while Felix stands on the ground below it, not on the */
/* ladders above. */
void scTundariaTowerUpdateBrownWallCell(void) {
	if(FIELD_HALF_CELL(
		   fdActorGetById(MainRoutineStateWork.leaderActorId)->y.fixed) != 0)
		scMapCellSetAttributes(MAPGRID_LAYER_GROUND, 14, 17, 0);
	else
		scMapCellSetAttributes(
			MAPGRID_LAYER_GROUND, 14, 17,
			CELL_ATTRIBUTES(CELL_BEHAVIOR_TUNDARIA_TOWER_BROWN_WALL, 0));
}

extern s16 TundariaTowerIceColumnsColumns[];
extern s16 TundariaTowerPillarPuzzleColumns[];

s32 scInit(void) {
	ACTOR *actor;
	s32 scene;
	s32 flag;

	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	scene = MainRoutineStateWork.next;
	if(scene == SCENE_TUNDARIA_TOWER_ENTRANCE) {
	}
	else if(scene == SCENE_TUNDARIA_TOWER_CENTER_PRONG) {
		switch(MainRoutineStateWork.entrance) {
		/* The first hall, by its four ways in. */
		case TUNDARIA_TOWER_CENTER_PRONG_FROM_TUNDARIA_TOWER_ENTRANCE:
		case CENTER_PRONG_HALL_NORTH_DOORWAY:
		case CENTER_PRONG_LEFT_POOL_DOOR:
		case CENTER_PRONG_RIGHT_POOL_DOOR:
			if(syGameFlagGet(FLAG_LEFT_WATER_PARCHED))
				scTundariaTowerParchLeftWater();
			if(syGameFlagGet(FLAG_RIGHT_WATER_PARCHED))
				scTundariaTowerParchRightWater();
			break;
		case CENTER_PRONG_CRYSTAL_CHAMBER:
			flag = syGameFlagGet(FLAG_CENTER_PRONG_TAKEN);
			if(flag == 0) {
				actor = fdActorGetById(ACTOR_FROZEN_CENTER_PRONG);
				fdActorSetSprFlags(fdActorGetById(ACTOR_FROZEN_CENTER_PRONG),
								   0);
				actor->collisionFlg = 0;
				actor->motionFlg = 0;
				actor->groundHeight = toFIXED(8);
				actor->y.fixed = toFIXED(8);
				fdActorSetItemIcon(actor, ITEM_CENTER_PRONG);
				if(syGameFlagGet(FLAG_CRYSTAL_BROKEN)) {
					fdMapGridCopyCellsTo(31, 85, 20, 85, 3, 4);
					fdMapGridCopyCellAttributes(11, 22, 3, 2, 20, 22);
					fdActorSetPos(ACTOR_FALLEN_CENTER_PRONG, 344, 372);
					fdActorGetById(ACTOR_FALLEN_CENTER_PRONG)->update =
						(OBJECT_UPDATE)scTundariaTowerSparkleCenterProng;
					fdActorSetPos(ACTOR_FROZEN_CENTER_PRONG, 0, 0);
				}
			}
			else if(syGameFlagGet(FLAG_CRYSTAL_BROKEN)) {
				fdMapGridCopyCellsTo(31, 85, 20, 85, 3, 4);
				fdMapGridCopyCellAttributes(11, 22, 3, 2, 20, 22);
				fdActorSetPos(ACTOR_FROZEN_CENTER_PRONG, 0, 0);
			}
			break;
		}
	}
	else if(scene == SCENE_TUNDARIA_TOWER_REFLUX) {
		switch(MainRoutineStateWork.entrance) {
		/* The sliding puzzle's part of the map. */
		case REFLUX_PUZZLE_ROOM_DOORWAY:
		case REFLUX_FROM_SLIDE:
			if(!syGameFlagGet(FLAG_DJINNI(DJINNI_REFLUX))) {
				if(!syGameFlagGet(FLAG_REFLUX_APPEARED)) {
					fdActorSetPos(ACTOR_REFLUX, 344, 536);
					scMapCellSetAttributes(
						MAPGRID_LAYER_GROUND, 21, 33,
						CELL_ATTRIBUTES(
							CELL_BEHAVIOR_TUNDARIA_TOWER_REFLUX_APPEARS, 0));
					scMapCellSetAttributes(
						MAPGRID_LAYER_GROUND, 22, 33,
						CELL_ATTRIBUTES(CELL_BEHAVIOR_WALL, 0));
				}
				else if(!syGameFlagGet(FLAG_FIGHTING_REFLUX)) {
					scMapCellSetAttributes(
						MAPGRID_LAYER_GROUND, 26, 33,
						CELL_ATTRIBUTES(CELL_BEHAVIOR_WALL, 0));
				}
				fdActorWatchAndSetScript(ACTOR_REFLUX,
										 MainRoutineStateWork.leaderActorId);
			}
			if(!syGameFlagGet(FLAG_KEEP_PARTY_POSITION) &&
			   MainRoutineStateWork.entrance == REFLUX_FROM_SLIDE)
				scTundariaTowerLandFromSlide();
			break;
		}
	}
	else if(scene == SCENE_TUNDARIA_TOWER_ICE_COLUMNS) {
		scTundariaTowerSetUpIceColumns(TundariaTowerIceColumnsColumns);
	}
	else if(scene == SCENE_TUNDARIA_TOWER_PILLAR_PUZZLE) {
		switch(MainRoutineStateWork.entrance) {
		case TUNDARIA_TOWER_PILLAR_PUZZLE_FROM_TUNDARIA_TOWER_BURST_BROOCH_SOUTH:
			actor =
				scFieldActorCreate(SPRITE_CRATE, toFIXED(472), 0, toFIXED(200));
			if(actor != NULL) {
				scSetActorSprPriority((s32)actor, 1);
				actor->motionFlg = 0;
				fdActorSetSprFlags(actor, 0);
				fdActorSetSprCol(actor, SPRITE_COLOR_BLACK);
				fdActorSetBlendMode(actor, 1);
			}
			break;
		case TUNDARIA_TOWER_PILLAR_PUZZLE_FROM_TUNDARIA_TOWER_ICE_COLUMNS_NORTH:
		case TUNDARIA_TOWER_PILLAR_PUZZLE_FROM_TUNDARIA_TOWER_ICE_COLUMNS_SOUTHWEST:
			scTundariaTowerSetUpIceColumns(TundariaTowerPillarPuzzleColumns);
			if(syGameFlagGet(FLAG_FIRST_LEDGE_PILLAR_SET)) {
				fdActorSetPos(ACTOR_FIRST_LEDGE_PILLAR, 216, 136);
				if(syGameFlagGet(FLAG_SECOND_LEDGE_PILLAR_SET))
					fdActorSetPos(ACTOR_SECOND_LEDGE_PILLAR, 200, 136);
			}
			fdActorPlaceBlocks(TundariaTowerPillarPuzzlePillars);
			fdActorSetSprPriority(ACTOR_SECOND_LEDGE_PILLAR, 2);
			break;
		case TUNDARIA_TOWER_PILLAR_PUZZLE_FROM_TUNDARIA_TOWER_CENTER_PRONG_NORTH:
		case TUNDARIA_TOWER_PILLAR_PUZZLE_FROM_TUNDARIA_TOWER_CENTER_PRONG_SOUTH:
			if(syGameFlagGet(FLAG_MIRROR_WALL_BURST)) {
				fdMapGridCopyCellsTo(18, 97, 6, 97, 3, 4);
				fdMapGridCopyCellsTo(82, 34, 70, 34, 3, 2);
				fdMapGridCopyCellAttributes(7, 36, 1, 1, 7, 35);
			}
			{
				ACTOR *effect;

				TundariaTower_Reflection = effect =
					scFieldActorCreate(SPRITE_FELIX, 0, 0, 0);
				effect->sceneWord = toFIXED(576);
				effect->update = scTundariaTowerReflectLeader;
				effect->motionFlg = 0;
				scSetActorSprPriority(effect, 3);
			}
			break;
		case TUNDARIA_TOWER_PILLAR_PUZZLE_FROM_TUNDARIA_TOWER_BURST_BROOCH_NORTH:
		case PILLAR_PUZZLE_MIRROR_ROOM_FOOT: {
			ACTOR *effect;

			TundariaTower_Reflection = effect =
				scFieldActorCreate(SPRITE_FELIX, 0, 0, 0);
			effect->sceneWord = toFIXED(624);
			effect->update = scTundariaTowerReflectLeader;
			effect->motionFlg = 0;
			scSetActorSprPriority(effect, 3);
		} break;
		case PILLAR_PUZZLE_FROM_SLIDE:
			if(!syGameFlagGet(FLAG_KEEP_PARTY_POSITION))
				scTundariaTowerLandFromSlide();
			break;
		}
	}
	else if(scene == SCENE_TUNDARIA_TOWER_BURST_BROOCH) {
		syGameFlagSet(FLAG_RETREAT_BLOCKED);
		if(syGameFlagGet(FLAG_BROWN_WALL_BURST)) {
			fdMapGridCopyCellsTo(13, 26, 13, 16, 3, 3);
			fdMapGridCopyCellAttributes(13, 26, 3, 3, 13, 16);
		}
		else {
			sySetEvent(scTundariaTowerUpdateBrownWallCell, TASK_PASS_FRAME);
		}
		fdActorPlaceBlocks(TundariaTowerBurstBroochPillar);
	}
	return 0;
}

s32 scPrepareMap(void) { return 0; }

#include "shared/actor_steer_by_pad.h"

#include "shared/leader_stop_if_pending.h"

#include "shared/leader_slide_on_ice.h"

extern s16 SlideOnIce_AnglesByKey[];
void fdActorSteerByPad(ACTOR *actor);
void fdLeaderStopIfPending(void);

#include "shared/slide_on_ice_from_edge.c"

/* Stops the leader's slide on the ice where it is (nonzero), or lets it run. */
void scLeaderSetSlideStopped(s32 value) { SlideOnIce_Stopped = value; }

#include "shared/leader_slide_on_ice_ahead.h"

/* Unused: places each listed stake at its cell, as Yampi Desert's do. */
UNUSED void scTundariaTowerPlaceStakes(u16 *table) {
	u16 *row;
	s32 id;
	ACTOR *actor;
	s16 x, z;

	if(table[0] == ACTOR_LIST_END)
		return;
	for(row = table; row[0] != ACTOR_LIST_END; row += 3) {
		id = row[0];
		actor = fdActorGetById(id);
		x = row[1];
		z = row[2];

		actor->collisionFlg &= ~ACTOR_SOLID;
		fdActorSetSprFlags(actor, 0);
		fdActorSetCellOpen(id, (u16)x, (u16)z, actor->layer);
	}
}

#include "shared/pound_sink_target.h"

#include "shared/actor_set_cell_open.h"

/* Unused: Magma Rock's crumbling pillars on entering: each listed pillar */
/* from its two flags (fallen: moved away; else its cell raised and its shape */
/* redrawn), then the leader lifted onto the ground under him. */
UNUSED void scTundariaTowerRestoreCrumblingPillars(const u16 *table) {
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	ACTOR *actor;
	s32 id;
	s32 flag;
	s32 nextFlag;
	MAP_CELL *cells;
	u8 *shapes;
	MAP_CELL *cell;
	s32 index;
	ACTOR *leader;
	s32 ground;

	if(*table != ACTOR_LIST_END) {
		for(; *table != ACTOR_LIST_END; table += 2) {
			id = table[0];
			actor = fdActorGetById(id);
			flag = table[1];
			nextFlag = flag + 1;
			cells = map->layers[actor->layer].cells;
			shapes = Ram_MapShapeGrid + (cells - (MAP_CELL *)Ram_MapCellBuffer);
			if(syGameFlagGet(nextFlag)) {
				fdActorSetPos(id, 0, 0);
			}
			else {
				s32 shape;
				u8 *slot;
				s32 height;

				index = fdActorGetCellIndex(actor);
				cell = &cells[index];
				slot = shapes + index;
				shape = fdMapGridGetTerrainKind(actor->layer, actor->x.fixed,
												actor->z.fixed);
				height = fdMapGetTerrainHeight(actor->layer, actor->x.fixed,
											   actor->z.fixed);
				fdActorDisableById(id);
				cell->attributeB = MAP_CELL_JUMP;
				*slot = fdMapShapeFindOrAdd(actor->layer, shape,
											FIELD_HALF_CELL(height) + 4);
				if(syGameFlagGet(flag))
					fdActorSelectAnimation(actor, 0);
			}
		}
	}
	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	ground =
		fdMapGetTerrainHeight(leader->layer, leader->x.fixed, leader->z.fixed);
	if(leader->y.fixed < ground) {
		leader->groundHeight = ground;
		leader->y.fixed = ground;
	}
}

/* Unused: Magma Rock's crumbling floor: a listed pillar stepped on flashes */
/* the first time and crumbles under the leader the second. */
UNUSED void scTundariaTowerCheckCrumblingStep(const u16 *table) {
	ACTOR *leader;
	ACTOR *trap;
	FIXED position[3];
	s32 id;
	s32 flag;
	s32 second;

	if(fdTryJumpForward() != 0)
		return;
	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	position[0] = leader->x.fixed;
	position[1] = leader->y.fixed;
	position[2] = leader->z.fixed;
	trap = fdActorFindAtCell(position, 0);
	if(trap == 0)
		return;
	while(*table != ACTOR_LIST_END) {
		id = table[0];
		if(fdActorGetById(id) != trap) {
			table += 2;
			continue;
		}
		flag = table[1];
		fdEventStart();
		if(!syGameFlagGet(flag)) {
			sySndPlay(SE_CHEST_CLOSE);
			fdActorSetSprCol(fdActorGetById(id), SPRITE_COLOR_WHITE);
			sySynch(2);
			fdActorSelectAnimation(trap, 0);
			fdActorSetSprCol(fdActorGetById(id), SPRITE_COLOR_NORMAL);
			sySynch(2);
			fdActorSetSprCol(fdActorGetById(id), SPRITE_COLOR_WHITE);
			sySynch(4);
			fdActorSetSprCol(fdActorGetById(id), SPRITE_COLOR_NORMAL);
			scTundariaTowerScatterDust(0);
			syGameFlagSet(flag);
		}
		else {
			second = flag + 1;
			if(!syGameFlagGet(second)) {
				leader->motionFlg = 0;
				sySndPlay(SE_PUSH);
				fdMapGridSetShake(1, 4, 1);
				scTundariaTowerScatterDust(0);
				sySynch(8);
				fdActorSelectAnimation(trap, 2);
				fdLeaderSetCellShapeAt(-2, trap->layer);
				scTundariaTowerScatterDust(1);
				sySynch(16);
				fdLeaderSetCellShapeAt(-2, trap->layer);
				sySynch(4);
				fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
				sySynch(8);
				leader->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
				sySynch(5);
				fdActorSetPosAndResetMotion(trap, 0, 0, 0);
				sySynch(2);
				sySndPlay(SE_THUMP);
				scTundariaTowerSpawnDustRing();
				sySynch(20);
				syGameFlagSet(second);
			}
		}
		fdEventEnd();
		return;
	}
}

/* Unused: Magma Rock's crumbling pillar: the listed pillar shakes, crumbles */
/* and falls away. */
UNUSED void scTundariaTowerCrumblePillar(u16 *row, s32 id) {
	ACTOR *actor = fdActorGetById(id);
	s32 flag;
	s32 nextFlag;

	if(*row == ACTOR_LIST_END)
		return;
	do {
		if(*row != id) {
			row += 2;
			continue;
		}
		flag = row[1];
		fdEventStart();
		nextFlag = flag + 1;
		if(!syGameFlagGet(nextFlag)) {
			sySndPlay(SE_PUSH);
			fdMapGridSetShake(1, 4, 1);
			scTundariaTowerScatterDust(0);
			sySynch(8);
			fdActorSelectAnimation(actor, 2);
			fdActorSetCellShape(-4, actor->layer, id);
			scTundariaTowerScatterDust(1);
			sySynch(16);
			fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
			sySynch(8);
			fdActorSetPosAndResetMotion(actor, 0, 0, 0);
			sySynch(2);
			sySndPlay(SE_THUMP);
			scTundariaTowerSpawnDustRing();
			sySynch(20);
			syGameFlagSet(flag);
			syGameFlagSet(nextFlag);
		}
		fdEventEnd();
		return;
	} while(*row != ACTOR_LIST_END);
}

/* Keeps the pillar's sprite cached and stops it. */
void scTundariaTowerFreezePillar(s32 id) {
	fdActorGetById(id)->retainSprCache =
		ACTOR_KEEP_SPRITE_VRAM | ACTOR_COMPOSE_OFF_SCREEN;
	fdActorDisableById(id);
}

/* Unused: Magma Rock's trap floor on entering: the first three placed actors */
/* gather at the leader and are put away, then each listed (actor, flag) pair */
/* is set from its two flags, as scTundariaTowerRestoreCrumblingPillars does, */
/* a flashed trap taking the second actor's pictures. */
UNUSED void scTundariaTowerRestorePillars(u16 *list) {
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	ACTOR *actor;
	s32 id;
	s32 flag;
	s32 nextFlag;
	MAP_CELL *cells;
	u8 *shapes;
	MAP_CELL *cell;
	s32 index;
	u8 *grid = MapShapeGrid;

	fdActorPlaceOnActor(ACTOR_FIRST_PLACED, MainRoutineStateWork.leaderActorId);
	fdActorPlaceOnActor(ACTOR_FIRST_PLACED + 1,
						MainRoutineStateWork.leaderActorId);
	fdActorPlaceOnActor(ACTOR_FIRST_PLACED + 2,
						MainRoutineStateWork.leaderActorId);
	sySynch(1);
	scTundariaTowerFreezePillar(ACTOR_FIRST_PLACED);
	scTundariaTowerFreezePillar(ACTOR_FIRST_PLACED + 1);
	scTundariaTowerFreezePillar(ACTOR_FIRST_PLACED + 2);
	fdActorSetAnimationById(ACTOR_FIRST_PLACED + 1, 0);
	sySynch(1);
	fdActorSetPos(ACTOR_FIRST_PLACED, 0, 0);
	fdActorSetPos(ACTOR_FIRST_PLACED + 1, 0, 0);
	fdActorSetPos(ACTOR_FIRST_PLACED + 2, 0, 0);
	sySynch(1);
	if(*list != ACTOR_LIST_END) {
		for(; *list != ACTOR_LIST_END; list += 2) {
			id = list[0];
			actor = fdActorGetById(id);
			flag = list[1];
			nextFlag = flag + 1;
			cells = map->layers[actor->layer].cells;
			shapes = grid + (cells - (MAP_CELL *)Ram_MapCellBuffer);
			actor->retainSprCache =
				ACTOR_KEEP_SPRITE_VRAM | ACTOR_COMPOSE_OFF_SCREEN;
			if(syGameFlagGet(nextFlag)) {
				fdActorSetPos(id, 0, 0);
			}
			else {
				s32 shape;
				u8 *slot;
				s32 height;

				index = fdActorGetCellIndex(actor);
				cell = &cells[index];
				slot = shapes + index;
				shape = fdMapGridGetTerrainKind(actor->layer, actor->x.fixed,
												actor->z.fixed);
				height = fdMapGetTerrainHeight(actor->layer, actor->x.fixed,
											   actor->z.fixed);
				fdActorDisableById(id);
				cell->attributeB = MAP_CELL_JUMP;
				*slot = fdMapShapeFindOrAdd(actor->layer, shape,
											FIELD_HALF_CELL(height) + 4);
				if(syGameFlagGet(flag))
					fdActorCopyGraphics(id, ACTOR_FIRST_PLACED + 1);
			}
		}
	}
	sySynch(10);
}

#include "shared/actor_take_place_of.h"

/* Unused: Magma Rock's trap floor: a listed pillar stepped on flashes the */
/* first time and gives way under the leader the second. */
UNUSED void scTundariaTowerStepOnTrap(const u16 *table) {
	ACTOR *leader;
	ACTOR *partner;
	ACTOR *trap;
	FIXED position[3];
	s32 id;
	s32 flag;
	s32 second;
	u8 *motion;

	if(fdTryJumpForward() != 0)
		return;
	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	partner = fdActorGetById(ACTOR_FIRST_PLACED);
	position[0] = leader->x.fixed;
	position[1] = leader->y.fixed;
	position[2] = leader->z.fixed;
	trap = fdActorFindAtCell(position, 0);
	if(trap != 0) {
		while(*table != ACTOR_LIST_END) {
			id = table[0];
			if(fdActorGetById(id) != trap) {
				table += 2;
				continue;
			}
			flag = table[1];
			fdEventStart();
			if(!syGameFlagGet(flag)) {
				fdActorSelectAnimation(partner, 1);
				fdActorTakePlaceOf(partner, trap);
				sySynch(1);
				sySndPlay(SE_CHEST_CLOSE);
				fdActorSetSprCol(fdActorGetById(ACTOR_FIRST_PLACED),
								 SPRITE_COLOR_WHITE);
				sySynch(2);
				fdActorSelectAnimation(partner, 0);
				fdActorCopyGraphics(id, ACTOR_FIRST_PLACED + 1);
				fdActorSetSprCol(fdActorGetById(ACTOR_FIRST_PLACED),
								 SPRITE_COLOR_NORMAL);
				sySynch(2);
				fdActorSetSprCol(fdActorGetById(ACTOR_FIRST_PLACED),
								 SPRITE_COLOR_WHITE);
				sySynch(4);
				fdActorSetSprCol(fdActorGetById(ACTOR_FIRST_PLACED),
								 SPRITE_COLOR_NORMAL);
				scTundariaTowerScatterDust(0);
				fdActorTakePlaceOf(trap, partner);
				sySynch(1);
				syGameFlagSet(flag);
			}
			else {
				second = flag + 1;
				if(!syGameFlagGet(second)) {
					fdActorSelectAnimation(partner, 0);
					fdActorTakePlaceOf(partner, trap);
					motion = &leader->motionFlg;
					sySynch(1);
					*motion = 0;
					sySndPlay(SE_PUSH);
					fdMapGridSetShake(1, 4, 1);
					scTundariaTowerScatterDust(0);
					sySynch(8);
					fdActorSelectAnimation(partner, 2);
					fdLeaderSetCellShapeAt(-2, trap->layer);
					scTundariaTowerScatterDust(1);
					sySynch(16);
					fdLeaderSetCellShapeAt(-2, trap->layer);
					sySynch(4);
					fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
					sySynch(8);
					*motion = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
					sySynch(5);
					fdActorSetPosAndResetMotion(partner, 0, 0, 0);
					sySynch(2);
					sySndPlay(SE_THUMP);
					scTundariaTowerSpawnDustRing();
					sySynch(20);
					syGameFlagSet(second);
				}
			}
			fdEventEnd();
			return;
		}
	}
}

#include "../field/field_effect_advance_motion_priority.c"

/* Scatters eight clouds of dust round the leader's feet, outwards when spread
 */
/* is set. */
void scTundariaTowerScatterDust(s32 spread) {
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	FIELD_EFFECT *effect;
	u32 frame = 0;
	u32 i;

	for(i = 0; i <= 7; i++) {
		{
			/* Each piece starts 0 to 13 pixels lower than the leader. */
			FIXED y = leader->y.fixed - ((syRandomBelow(14)) << 16);

			effect = (FIELD_EFFECT *)syImportActorCreate(
				SPRITE_DUST_CLOUD,
				i * toFIXED(2) + leader->x.fixed - toFIXED(8), y - toFIXED(2),
				leader->z.fixed);
		}
		if(effect != 0) {
			frame = fdSprShareVramCacheEntry(effect->spr, frame);
			effect->motionFlg = 0;
			effect->collisionFlg = 0;
			fdActorSetSprFlags(effect, 0);
			fdActorSelectAnimation(effect, 2);
			scTundariaTowerStartEffectScript(effect, Dust_ScatterScript);
			effect->scaleY = effect->scaleX = FIXED_ONE * 7 / 10;
			effect->spr->pri = 1;
			if(spread != 0) {
				effect->vel.x =
					syMulFX(syCos(i * DEGtoANG(45)), FIXED_ONE * 6 / 5);
				effect->vel.z = sySin(i * DEGtoANG(45));
			}
			else {
				effect->vel.x = spread;
				effect->vel.z = spread;
			}
			effect->vel.y = -(syRandomBelow(16384) + (toFIXED(3) / 8));
			effect->spin = syRandomBelow(512) - (TRIG_TURN / 256);
			effect->scaleRateX = -(FIXED_ONE / 32);
			effect->scaleRateY = -(FIXED_ONE * 3 / 128);
			effect->update = fdEffectAdvanceMotionAtLeaderPriority;
		}
	}
}

/* Throws a ring of dust clouds out from the leader as something lands. */
void scTundariaTowerSpawnDustRing(void) {
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	s32 state;
	u32 i;

	if(leader != NULL) {
		state = 0;
		for(i = 0; i <= DUST_RING_PUFFS; i++) {
			FIELD_EFFECT *effect = ((FIELD_EFFECT *)syImportActorCreate(
				SPRITE_DUST_CLOUD, leader->x.fixed, leader->y.fixed,
				leader->z.fixed));

			if(effect != NULL) {
				s32 angle;

				state = fdSprShareVramCacheEntry(effect->spr, state);
				effect->motionFlg = 0;
				effect->collisionFlg = 0;
				fdActorSetSprFlags(effect, 0);
				fdActorSelectAnimation(effect, 2);
				scTundariaTowerStartEffectScript(effect, Dust_BurstScript);
				angle = i * (TRIG_TURN / DUST_RING_PUFFS);
				effect->vel.x = syCos(angle);
				effect->vel.y = 0;
				effect->vel.z = sySin(angle);
				effect->vel.x += effect->vel.x >> 1;
				effect->vel.x += syRandomBelow(toFIXED(3) / 4) - toFIXED(3) / 8;
				effect->vel.z +=
					syRandomBelow(toFIXED(3) / 8) - toFIXED(3) / 16;
				effect->spin = syRandomBelow(4096) - (TRIG_TURN / 32);
				effect->scaleRateX = 0;
				effect->scaleRateY = 0;
				effect->update = fdEffectAdvanceMotionAtLeaderPriority;
				effect->spr->pri = ((FIELD_SPR *)leader->spr)->pri;
			}
		}
	}
}

#include "shared/actor_set_cell_shape.h"

#include "shared/leader_set_cell_shape.h"

#include "shared/particle_update_falling.h"

/* Spawns count rocks around (x, y, z), each pushed out at a random angle and */
/* thrown up and out along it. */
#include "shared/rock_spawn.h"

/* The falling rocks' setup: loads their picture, sets up ROCK_COUNT small */
/* sprites, marks each rock idle and starts the task that spawns them. */
void scTundariaTowerStartFallingRocks(void) {
	ROCK_STATE *state = syHeapSlotAlloc(HEAP_SLOT_MENU, sizeof(ROCK_STATE));
	s32 resource;
	s32 tile;
	ROCK *rock;
	EFFECT_SPR *sprite;
	s32 i;

	syDecompressTaggedLz(syResourceGetTableEntry(RESOURCE_FALLING_ROCK), state);
	resource = syVramCacheFindFreeEntry();
	tile = mnVramLoad(resource, sizeof(state->tiles), state);
	state->tileBase = tile;
	state->resource = resource;
	rock = state->rocks;
	for(i = 0; i < ROCK_COUNT; i++) {
		sprite = &state->sprs[i];
		fdEffectSprInit(sprite, 8, 8, OBJ_SIZE_16X16, tile);
		sprite->oam.fullCol = 1;
		sprite->oam.pal = 0;
		sprite->oam.pri = 0;
		sprite->depth = OAM_DEPTH_WINDOW;
		rock->tmr = -1;
		rock++;
	}
	state->next = 0;
	syTaskAddFrame(btParticleUpdateFalling, TASK_PASS_FRAME);
}

/* Stops the falling rocks and frees their pictures and state. */
void scTundariaTowerEndFallingRocks(void) {
	ROCK_STATE *state = Ram_HeapSlots->menuState;

	syCloseEvent(btParticleUpdateFalling);
	syVramCacheFreeEntry(state->resource);
	syHeapSlotRelease(HEAP_SLOT_MENU);
}

void scTundariaTowerFightReflux(void);
void scTundariaTowerOpenDoor(void);
void scTundariaTowerFreeCenterProng(void);
void scTundariaTowerJumpDownSlide(void);
void scTundariaTowerStartPillarMove(void);

/* clang-format off */
#include "shared/field_effect_duration_scripts.c"

/* The blocks Move pushes, ended by -1: the pillar puzzle's four, and the one */
/* in the Burst Brooch's room. */
s16 TundariaTowerPillarPuzzlePillars[] = {
    ACTOR_FIRST_SLIDING_PILLAR, ACTOR_SECOND_LEDGE_PILLAR,
    ACTOR_FIRST_LEDGE_PILLAR, ACTOR_SECOND_SLIDING_PILLAR, -1,
};
s16 TundariaTowerBurstBroochPillar[] = {
    ACTOR_BROOCH_ROOM_PILLAR, -1,
};

#include "shared/slide_on_ice_angles.c"

#include "shared/dust_scatter_script.c"

#include "shared/dust_burst_script.c"

const s32 *FieldEffect_DurationScripts[] = {
    FieldEffect_ShortScript,
    FieldEffect_MediumScript,
    FieldEffect_LongScript,
};

/* The ice columns of the room of ice columns: each one's actor, its standing */
/* column's animation (the flattened one follows it) and its Pound flag, */
/* ending -1. */
s16 TundariaTowerIceColumnsColumns[] = {
    ACTOR_FIRST_ICE_COLUMN, 7, FLAG_ICE_COLUMN_POUNDED,
    ACTOR_FIRST_ICE_COLUMN + 1, 7, FLAG_ICE_COLUMN_POUNDED + 1,
    ACTOR_FIRST_ICE_COLUMN + 2, 7, FLAG_ICE_COLUMN_POUNDED + 2,
    ACTOR_FIRST_ICE_COLUMN + 3, 7, FLAG_ICE_COLUMN_POUNDED + 3,
    -1,
};

/* The ice columns of the room of pillars, as TundariaTowerIceColumnsColumns. */
s16 TundariaTowerPillarPuzzleColumns[] = {
    ACTOR_FIRST_ICE_COLUMN, 7, FLAG_ICE_COLUMN_POUNDED,
    ACTOR_FIRST_ICE_COLUMN + 1, 7, FLAG_ICE_COLUMN_POUNDED + 1,
    ACTOR_FIRST_ICE_COLUMN + 2, 7, FLAG_ICE_COLUMN_POUNDED + 2,
    -1, 0,
};

/* Reflux turning round a sixteenth of a turn each frame as he comes down. */
s32 TundariaTower_RefluxSpinScript[] = {
    SC_SET(SW_FACING, 0),
    SC_LABEL(1),
    SC_WAIT(1),
    SC_ADD(SW_FACING, TRIG_TURN / 16),
    SC_LOOP(SC_FOREVER, SC_LABEL(1)),
};

#include "shared/scene_entrances_default.c"

SCENE_REGION TundariaTowerPillarPuzzleExitRegions[] = {
    {816, 80, 528, 832, 96, 544, CONDITION_ALWAYS,
     TUNDARIA_TOWER_PILLAR_PUZZLE_TO_TUNDARIA_TOWER_BURST_BROOCH_NORTH},
    {SCENE_TABLE_END},
};

SCENE_REGION TundariaTowerBurstBroochExitRegions[] = {
    {176, 15, 192, 192, 33, 208, CONDITION_ALWAYS,
     TUNDARIA_TOWER_BURST_BROOCH_TO_TUNDARIA_TOWER_PILLAR_PUZZLE_NORTH},
    {SCENE_TABLE_END},
};

u32 SceneExits[] = {
    SCENE_EXITS(TundariaTowerEntrance),
    SCENE_EXIT_UNLESS(TUNDARIA_TOWER_ENTRANCE_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_TUNDARIA_TOWER_ENTRANCE, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(TUNDARIA_TOWER_ENTRANCE_TO_TUNDARIA_TOWER_CENTER_PRONG, TundariaTowerCenterProng, TUNDARIA_TOWER_CENTER_PRONG_FROM_TUNDARIA_TOWER_ENTRANCE, EXIT_FLAG_NONE),
    SCENE_EXITS(TundariaTowerCenterProng),
    SCENE_EXIT_UNLESS(TUNDARIA_TOWER_CENTER_PRONG_TO_TUNDARIA_TOWER_ENTRANCE, TundariaTowerEntrance, TUNDARIA_TOWER_ENTRANCE_FROM_TUNDARIA_TOWER_CENTER_PRONG, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(CENTER_PRONG_HALL_NORTH_DOORWAY, TundariaTowerCenterProng, CENTER_PRONG_CRYSTAL_CHAMBER, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(CENTER_PRONG_LEFT_POOL_DOOR, TundariaTowerCenterProng, CENTER_PRONG_WEST_ROOM_EAST_END, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(CENTER_PRONG_RIGHT_POOL_DOOR, TundariaTowerCenterProng, CENTER_PRONG_EAST_ROOM_WEST_END, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(TUNDARIA_TOWER_CENTER_PRONG_TO_TUNDARIA_TOWER_PILLAR_PUZZLE_SOUTH, TundariaTowerPillarPuzzle, TUNDARIA_TOWER_PILLAR_PUZZLE_FROM_TUNDARIA_TOWER_CENTER_PRONG_SOUTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(CENTER_PRONG_WEST_ROOM_NORTH_DOORWAY, TundariaTowerCenterProng, CENTER_PRONG_WEST_CHESTS_FRONT, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(CENTER_PRONG_WEST_ROOM_EAST_END, TundariaTowerCenterProng, CENTER_PRONG_LEFT_POOL_DOOR, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(CENTER_PRONG_EAST_ROOM_WEST_END, TundariaTowerCenterProng, CENTER_PRONG_RIGHT_POOL_DOOR, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(CENTER_PRONG_EAST_ROOM_NORTH_DOORWAY, TundariaTowerCenterProng, CENTER_PRONG_EAST_CHESTS_FRONT, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(TUNDARIA_TOWER_CENTER_PRONG_TO_TUNDARIA_TOWER_ICE_SLIDE, TundariaTowerIceSlide, TUNDARIA_TOWER_ICE_SLIDE_FROM_TUNDARIA_TOWER_CENTER_PRONG, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(CENTER_PRONG_CRYSTAL_CHAMBER, TundariaTowerCenterProng, CENTER_PRONG_HALL_NORTH_DOORWAY, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(TUNDARIA_TOWER_CENTER_PRONG_TO_TUNDARIA_TOWER_PILLAR_PUZZLE_NORTH, TundariaTowerPillarPuzzle, TUNDARIA_TOWER_PILLAR_PUZZLE_FROM_TUNDARIA_TOWER_CENTER_PRONG_NORTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(CENTER_PRONG_CORRIDOR_MIDDLE_PASSAGE, TundariaTowerCenterProng, CENTER_PRONG_WEST_CHESTS_BACK, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(CENTER_PRONG_CORRIDOR_EAST_PASSAGE, TundariaTowerCenterProng, CENTER_PRONG_EAST_CHESTS_BACK, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(CENTER_PRONG_WEST_CHESTS_BACK, TundariaTowerCenterProng, CENTER_PRONG_CORRIDOR_MIDDLE_PASSAGE, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(CENTER_PRONG_WEST_CHESTS_FRONT, TundariaTowerCenterProng, CENTER_PRONG_WEST_ROOM_NORTH_DOORWAY, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(CENTER_PRONG_EAST_CHESTS_BACK, TundariaTowerCenterProng, CENTER_PRONG_CORRIDOR_EAST_PASSAGE, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(CENTER_PRONG_EAST_CHESTS_FRONT, TundariaTowerCenterProng, CENTER_PRONG_EAST_ROOM_NORTH_DOORWAY, EXIT_FLAG_NONE),
    SCENE_EXITS(TundariaTowerIceSlide),
    SCENE_EXIT_UNLESS(ICE_SLIDE_HALL_WEST_FOOT, TundariaTowerIceSlide, ICE_SLIDE_WEST_ROOM_DOORWAY, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(ICE_SLIDE_HALL_EAST_FOOT, TundariaTowerIceSlide, ICE_SLIDE_EAST_ROOM_DOORWAY, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(ICE_SLIDE_WEST_ROOM_DOORWAY, TundariaTowerIceSlide, ICE_SLIDE_HALL_WEST_FOOT, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(ICE_SLIDE_WEST_ROOM_FOOT, TundariaTowerIceSlide, ICE_SLIDE_ICE_FLOOR_WEST_LADDER, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(TUNDARIA_TOWER_ICE_SLIDE_TO_TUNDARIA_TOWER_REFLUX, TundariaTowerReflux, TUNDARIA_TOWER_REFLUX_FROM_TUNDARIA_TOWER_ICE_SLIDE, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(ICE_SLIDE_STAIR_ROOM_FOOT, TundariaTowerIceSlide, ICE_SLIDE_ICE_FLOOR_MIDDLE_LADDER, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(ICE_SLIDE_EAST_ROOM_DOORWAY, TundariaTowerIceSlide, ICE_SLIDE_HALL_EAST_FOOT, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(ICE_SLIDE_EAST_ROOM_FOOT, TundariaTowerIceSlide, ICE_SLIDE_ICE_FLOOR_EAST_LADDER, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(TUNDARIA_TOWER_ICE_SLIDE_TO_TUNDARIA_TOWER_CENTER_PRONG, TundariaTowerCenterProng, TUNDARIA_TOWER_CENTER_PRONG_FROM_TUNDARIA_TOWER_ICE_SLIDE, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(ICE_SLIDE_ICE_FLOOR_WEST_LADDER, TundariaTowerIceSlide, ICE_SLIDE_WEST_ROOM_FOOT, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(ICE_SLIDE_ICE_FLOOR_MIDDLE_LADDER, TundariaTowerIceSlide, ICE_SLIDE_STAIR_ROOM_FOOT, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(ICE_SLIDE_ICE_FLOOR_EAST_LADDER, TundariaTowerIceSlide, ICE_SLIDE_EAST_ROOM_FOOT, EXIT_FLAG_NONE),
    SCENE_EXITS(TundariaTowerReflux),
    SCENE_EXIT_UNLESS(TUNDARIA_TOWER_REFLUX_TO_TUNDARIA_TOWER_ICE_COLUMNS_NORTHWEST, TundariaTowerIceColumns, TUNDARIA_TOWER_ICE_COLUMNS_FROM_TUNDARIA_TOWER_REFLUX_NORTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(TUNDARIA_TOWER_REFLUX_TO_TUNDARIA_TOWER_ICE_SLIDE, TundariaTowerIceSlide, TUNDARIA_TOWER_ICE_SLIDE_FROM_TUNDARIA_TOWER_REFLUX, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(TUNDARIA_TOWER_REFLUX_TO_TUNDARIA_TOWER_ICE_COLUMNS_NORTHEAST, TundariaTowerIceColumns, TUNDARIA_TOWER_ICE_COLUMNS_FROM_TUNDARIA_TOWER_REFLUX_NORTHEAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(TUNDARIA_TOWER_REFLUX_TO_TUNDARIA_TOWER_ICE_COLUMNS_NORTH, TundariaTowerIceColumns, TUNDARIA_TOWER_ICE_COLUMNS_FROM_TUNDARIA_TOWER_REFLUX_NORTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(REFLUX_HALL_SOUTHWEST_FOOT, TundariaTowerReflux, REFLUX_PUZZLE_ROOM_DOORWAY, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(REFLUX_PUZZLE_ROOM_DOORWAY, TundariaTowerReflux, REFLUX_HALL_SOUTHWEST_FOOT, EXIT_FLAG_NONE),
    SCENE_EXITS(TundariaTowerIceColumns),
    SCENE_EXIT_UNLESS(ICE_COLUMNS_CHEST_ROOM_FOOT, TundariaTowerIceColumns, ICE_COLUMNS_NORTH_ROOM_EAST_DOORWAY, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(TUNDARIA_TOWER_ICE_COLUMNS_TO_TUNDARIA_TOWER_REFLUX_NORTHWEST, TundariaTowerReflux, TUNDARIA_TOWER_REFLUX_FROM_TUNDARIA_TOWER_ICE_COLUMNS_NORTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(ICE_COLUMNS_NORTH_ROOM_EAST_DOORWAY, TundariaTowerIceColumns, ICE_COLUMNS_CHEST_ROOM_FOOT, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(ICE_COLUMNS_HALL_WEST_DOORWAY, TundariaTowerIceColumns, ICE_COLUMNS_CORRIDOR_SOUTH_DOORWAY, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(TUNDARIA_TOWER_ICE_COLUMNS_TO_TUNDARIA_TOWER_REFLUX_NORTH, TundariaTowerReflux, TUNDARIA_TOWER_REFLUX_FROM_TUNDARIA_TOWER_ICE_COLUMNS_NORTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(TUNDARIA_TOWER_ICE_COLUMNS_TO_TUNDARIA_TOWER_REFLUX_NORTHEAST, TundariaTowerReflux, TUNDARIA_TOWER_REFLUX_FROM_TUNDARIA_TOWER_ICE_COLUMNS_NORTHEAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(TUNDARIA_TOWER_ICE_COLUMNS_TO_TUNDARIA_TOWER_PILLAR_PUZZLE_NORTH, TundariaTowerPillarPuzzle, TUNDARIA_TOWER_PILLAR_PUZZLE_FROM_TUNDARIA_TOWER_ICE_COLUMNS_NORTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(ICE_COLUMNS_CORRIDOR_SOUTH_DOORWAY, TundariaTowerIceColumns, ICE_COLUMNS_HALL_WEST_DOORWAY, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(TUNDARIA_TOWER_ICE_COLUMNS_TO_TUNDARIA_TOWER_PILLAR_PUZZLE_SOUTHWEST, TundariaTowerPillarPuzzle, TUNDARIA_TOWER_PILLAR_PUZZLE_FROM_TUNDARIA_TOWER_ICE_COLUMNS_SOUTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(TUNDARIA_TOWER_ICE_COLUMNS_TO_TUNDARIA_TOWER_PILLAR_PUZZLE_SOUTH, TundariaTowerPillarPuzzle, TUNDARIA_TOWER_PILLAR_PUZZLE_FROM_TUNDARIA_TOWER_ICE_COLUMNS_SOUTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(TUNDARIA_TOWER_ICE_COLUMNS_TO_TUNDARIA_TOWER_REFLUX_SOUTH, TundariaTowerReflux, TUNDARIA_TOWER_REFLUX_FROM_TUNDARIA_TOWER_ICE_COLUMNS_SOUTH, EXIT_FLAG_NONE),
    SCENE_EXITS(TundariaTowerPillarPuzzle),
    SCENE_EXIT_UNLESS(TUNDARIA_TOWER_PILLAR_PUZZLE_TO_TUNDARIA_TOWER_ICE_COLUMNS_NORTH, TundariaTowerIceColumns, TUNDARIA_TOWER_ICE_COLUMNS_FROM_TUNDARIA_TOWER_PILLAR_PUZZLE_NORTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(TUNDARIA_TOWER_PILLAR_PUZZLE_TO_TUNDARIA_TOWER_ICE_COLUMNS_SOUTHWEST, TundariaTowerIceColumns, TUNDARIA_TOWER_ICE_COLUMNS_FROM_TUNDARIA_TOWER_PILLAR_PUZZLE_SOUTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(TUNDARIA_TOWER_PILLAR_PUZZLE_TO_TUNDARIA_TOWER_BURST_BROOCH_SOUTH, TundariaTowerBurstBrooch, TUNDARIA_TOWER_BURST_BROOCH_FROM_TUNDARIA_TOWER_PILLAR_PUZZLE_SOUTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(TUNDARIA_TOWER_PILLAR_PUZZLE_TO_TUNDARIA_TOWER_CENTER_PRONG_NORTH, TundariaTowerCenterProng, TUNDARIA_TOWER_CENTER_PRONG_FROM_TUNDARIA_TOWER_PILLAR_PUZZLE_NORTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(TUNDARIA_TOWER_PILLAR_PUZZLE_TO_TUNDARIA_TOWER_CENTER_PRONG_SOUTH, TundariaTowerCenterProng, TUNDARIA_TOWER_CENTER_PRONG_FROM_TUNDARIA_TOWER_PILLAR_PUZZLE_SOUTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(TUNDARIA_TOWER_PILLAR_PUZZLE_TO_TUNDARIA_TOWER_BURST_BROOCH_NORTH, TundariaTowerBurstBrooch, TUNDARIA_TOWER_BURST_BROOCH_FROM_TUNDARIA_TOWER_PILLAR_PUZZLE_NORTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(PILLAR_PUZZLE_MIRROR_ROOM_FOOT, TundariaTowerPillarPuzzle, PILLAR_PUZZLE_CORRIDOR_DOORWAY, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(PILLAR_PUZZLE_CORRIDOR_DOORWAY, TundariaTowerPillarPuzzle, PILLAR_PUZZLE_MIRROR_ROOM_FOOT, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(TUNDARIA_TOWER_PILLAR_PUZZLE_TO_TUNDARIA_TOWER_ICE_COLUMNS_SOUTH, TundariaTowerIceColumns, TUNDARIA_TOWER_ICE_COLUMNS_FROM_TUNDARIA_TOWER_PILLAR_PUZZLE_SOUTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(PILLAR_PUZZLE_FROM_SLIDE, TundariaTowerPillarPuzzle, PILLAR_PUZZLE_FROM_SLIDE, EXIT_FLAG_NONE),
    SCENE_EXITS(TundariaTowerBurstBrooch),
    SCENE_EXIT_UNLESS(TUNDARIA_TOWER_BURST_BROOCH_TO_TUNDARIA_TOWER_PILLAR_PUZZLE_NORTH, TundariaTowerPillarPuzzle, TUNDARIA_TOWER_PILLAR_PUZZLE_FROM_TUNDARIA_TOWER_BURST_BROOCH_NORTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(TUNDARIA_TOWER_BURST_BROOCH_TO_TUNDARIA_TOWER_PILLAR_PUZZLE_SOUTH, TundariaTowerPillarPuzzle, TUNDARIA_TOWER_PILLAR_PUZZLE_FROM_TUNDARIA_TOWER_BURST_BROOCH_SOUTH, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

ACTOR_PLACE TundariaTowerCenterProngActorPlacements[] = {
    {SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, 1, 1, 0, 0, FACING_EAST, 2, 2},
    {SCENE_TABLE_END},
};

ACTOR_PLACE TundariaTowerIceSlideActorPlacements[] = {
    {SCENE_TABLE_END},
};

ACTOR_PLACE TundariaTowerRefluxActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, FLAG_DJINNI(DJINNI_REFLUX), 7, 424, 0, 536, FACING_WEST, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

ACTOR_PLACE TundariaTowerIceColumnsActorPlacements[] = {                                             
      {SPRITE_STAKE, CONDITION_ALWAYS, 1, 1, 0, 0, FACING_SOUTH, 2, 2},                
      {SPRITE_STAKE, CONDITION_ALWAYS, 1, 2, 0, 0, FACING_SOUTH, 2, 2},                
      {SPRITE_STAKE, CONDITION_ALWAYS, 1, 3, 0, 0, FACING_SOUTH, 2, 2},                
      {SPRITE_STAKE, CONDITION_ALWAYS, 1, 4, 0, 0, FACING_SOUTH, 2, 2},                
      {SCENE_TABLE_END},                                                       
  };

ACTOR_PLACE TundariaTowerPillarPuzzleActorPlacements[] = {                                             
      {SPRITE_STAKE, CONDITION_ALWAYS, 1, 1, 0, 0, FACING_SOUTH, 2, 2},                
      {SPRITE_STAKE, CONDITION_ALWAYS, 1, 2, 0, 0, FACING_SOUTH, 2, 2},                
      {SPRITE_STAKE, CONDITION_ALWAYS, 1, 3, 0, 0, FACING_SOUTH, 2, 2},                
      {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, 1, 4, 0, 0, FACING_SOUTH, 2, 2},                
      {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, 1, 5, 0, 0, FACING_SOUTH, 2, 2},                
      {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, 1, 6, 0, 0, FACING_SOUTH, 2, 2},                
      {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, 1, 7, 0, 0, FACING_SOUTH, 2, 2},                
      {SCENE_TABLE_END},                                                       
  };

ACTOR_PLACE TundariaTowerBurstBroochActorPlacements[] = {
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, 1, 2, 0, 0, FACING_SOUTH, 2, 2},
    {SCENE_TABLE_END},
};

TRIGGER TundariaTowerEntranceTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, TUNDARIA_TOWER_ENTRANCE_TO_WORLD_MAP},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, TUNDARIA_TOWER_ENTRANCE_TO_TUNDARIA_TOWER_CENTER_PRONG},
    {TRIGGER_TABLE_END},
};
/* clang-format on */

TRIGGER TundariaTowerCenterProngTriggers[] = {
	{TRIGGER_EXIT, 1, CONDITION_ALWAYS,
	 TUNDARIA_TOWER_CENTER_PRONG_TO_TUNDARIA_TOWER_ENTRANCE},
	{TRIGGER_EXIT, CENTER_PRONG_HALL_NORTH_DOORWAY, CONDITION_ALWAYS,
	 CENTER_PRONG_HALL_NORTH_DOORWAY},
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
	 CELL_BEHAVIOR_TUNDARIA_TOWER_LEFT_DOOR, CONDITION_ALWAYS,
	 EVENT(scTundariaTowerOpenDoor)},
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
	 CELL_BEHAVIOR_TUNDARIA_TOWER_RIGHT_DOOR, CONDITION_ALWAYS,
	 EVENT(scTundariaTowerOpenDoor)},
	{TRIGGER_EXIT, 5, CONDITION_ALWAYS,
	 TUNDARIA_TOWER_CENTER_PRONG_TO_TUNDARIA_TOWER_PILLAR_PUZZLE_SOUTH},
	{TRIGGER_EXIT, CENTER_PRONG_WEST_ROOM_NORTH_DOORWAY, CONDITION_ALWAYS,
	 CENTER_PRONG_WEST_ROOM_NORTH_DOORWAY},
	{TRIGGER_EXIT, CENTER_PRONG_WEST_ROOM_EAST_END, CONDITION_ALWAYS,
	 CENTER_PRONG_WEST_ROOM_EAST_END},
	{TRIGGER_EXIT, CENTER_PRONG_EAST_ROOM_WEST_END, CONDITION_ALWAYS,
	 CENTER_PRONG_EAST_ROOM_WEST_END},
	{TRIGGER_EXIT, CENTER_PRONG_EAST_ROOM_NORTH_DOORWAY, CONDITION_ALWAYS,
	 CENTER_PRONG_EAST_ROOM_NORTH_DOORWAY},
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 10, CONDITION_ALWAYS,
	 TUNDARIA_TOWER_CENTER_PRONG_TO_TUNDARIA_TOWER_ICE_SLIDE},
	{TRIGGER_EXIT, CENTER_PRONG_CRYSTAL_CHAMBER, CONDITION_ALWAYS,
	 CENTER_PRONG_CRYSTAL_CHAMBER},
	{TRIGGER_EXIT, 12, CONDITION_ALWAYS,
	 TUNDARIA_TOWER_CENTER_PRONG_TO_TUNDARIA_TOWER_PILLAR_PUZZLE_NORTH},
	{TRIGGER_EXIT, CENTER_PRONG_CORRIDOR_MIDDLE_PASSAGE, CONDITION_ALWAYS,
	 CENTER_PRONG_CORRIDOR_MIDDLE_PASSAGE},
	{TRIGGER_EXIT, CENTER_PRONG_CORRIDOR_EAST_PASSAGE, CONDITION_ALWAYS,
	 CENTER_PRONG_CORRIDOR_EAST_PASSAGE},
	{TRIGGER_EXIT, CENTER_PRONG_WEST_CHESTS_BACK, CONDITION_ALWAYS,
	 CENTER_PRONG_WEST_CHESTS_BACK},
	{TRIGGER_EXIT, CENTER_PRONG_WEST_CHESTS_FRONT, CONDITION_ALWAYS,
	 CENTER_PRONG_WEST_CHESTS_FRONT},
	{TRIGGER_EXIT, CENTER_PRONG_EAST_CHESTS_BACK, CONDITION_ALWAYS,
	 CENTER_PRONG_EAST_CHESTS_BACK},
	{TRIGGER_EXIT, CENTER_PRONG_EAST_CHESTS_FRONT, CONDITION_ALWAYS,
	 CENTER_PRONG_EAST_CHESTS_FRONT},
	{TRIGGER_EXAMINE, CELL_BEHAVIOR_TUNDARIA_TOWER_CRYSTAL, CONDITION_ALWAYS,
	 EVENT(scTundariaTowerExamineCrystal)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_PARCH, PSYNERGY_PHASE_ACTS),
	 CELL_BEHAVIOR_TUNDARIA_TOWER_LEFT_POOL, FLAG_LEFT_WATER_PARCHED,
	 EVENT(scTundariaTowerParchLeftWater)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_PARCH, PSYNERGY_PHASE_ACTS),
	 CELL_BEHAVIOR_TUNDARIA_TOWER_RIGHT_POOL, FLAG_RIGHT_WATER_PARCHED,
	 EVENT(scTundariaTowerParchRightWater)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
	 CELL_BEHAVIOR_TUNDARIA_TOWER_CRYSTAL, FLAG_CRYSTAL_BROKEN,
	 EVENT(scTundariaTowerBurstCrystal)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST),
	 CELL_BEHAVIOR_TUNDARIA_TOWER_CRYSTAL, FLAG_CRYSTAL_BROKEN,
	 EVENT(scTundariaTowerFreeCenterProng)},
	{TRIGGER_TABLE_END},
};

TRIGGER TundariaTowerIceSlideTriggers[] = {
	{TRIGGER_EXIT, ICE_SLIDE_HALL_WEST_FOOT, CONDITION_ALWAYS,
	 ICE_SLIDE_HALL_WEST_FOOT},
	{TRIGGER_EXIT, ICE_SLIDE_HALL_EAST_FOOT, CONDITION_ALWAYS,
	 ICE_SLIDE_HALL_EAST_FOOT},
	{TRIGGER_EXIT, ICE_SLIDE_WEST_ROOM_DOORWAY, CONDITION_ALWAYS,
	 ICE_SLIDE_WEST_ROOM_DOORWAY},
	{TRIGGER_EXIT, ICE_SLIDE_WEST_ROOM_FOOT, CONDITION_ALWAYS,
	 ICE_SLIDE_WEST_ROOM_FOOT},
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 5, CONDITION_ALWAYS,
	 TUNDARIA_TOWER_ICE_SLIDE_TO_TUNDARIA_TOWER_REFLUX},
	{TRIGGER_EXIT, ICE_SLIDE_STAIR_ROOM_FOOT, CONDITION_ALWAYS,
	 ICE_SLIDE_STAIR_ROOM_FOOT},
	{TRIGGER_EXIT, ICE_SLIDE_EAST_ROOM_DOORWAY, CONDITION_ALWAYS,
	 ICE_SLIDE_EAST_ROOM_DOORWAY},
	{TRIGGER_EXIT, ICE_SLIDE_EAST_ROOM_FOOT, CONDITION_ALWAYS,
	 ICE_SLIDE_EAST_ROOM_FOOT},
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 9, CONDITION_ALWAYS,
	 TUNDARIA_TOWER_ICE_SLIDE_TO_TUNDARIA_TOWER_CENTER_PRONG},
	{TRIGGER_EXIT, ICE_SLIDE_ICE_FLOOR_WEST_LADDER, CONDITION_ALWAYS,
	 ICE_SLIDE_ICE_FLOOR_WEST_LADDER},
	{TRIGGER_EXIT, ICE_SLIDE_ICE_FLOOR_MIDDLE_LADDER, CONDITION_ALWAYS,
	 ICE_SLIDE_ICE_FLOOR_MIDDLE_LADDER},
	{TRIGGER_EXIT, ICE_SLIDE_ICE_FLOOR_EAST_LADDER, CONDITION_ALWAYS,
	 ICE_SLIDE_ICE_FLOOR_EAST_LADDER},
	{TRIGGER_STEP, CELL_BEHAVIOR_TUNDARIA_TOWER_ICE, CONDITION_ALWAYS,
	 EVENT(scTundariaTowerSlideOnIce)},
	{TRIGGER_TABLE_END},
};

TRIGGER TundariaTowerRefluxTriggers[] = {
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS,
	 TUNDARIA_TOWER_REFLUX_TO_TUNDARIA_TOWER_ICE_COLUMNS_NORTHWEST},
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS,
	 TUNDARIA_TOWER_REFLUX_TO_TUNDARIA_TOWER_ICE_SLIDE},
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 3, CONDITION_ALWAYS,
	 TUNDARIA_TOWER_REFLUX_TO_TUNDARIA_TOWER_ICE_COLUMNS_NORTHEAST},
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 4, CONDITION_ALWAYS,
	 TUNDARIA_TOWER_REFLUX_TO_TUNDARIA_TOWER_ICE_COLUMNS_NORTH},
	{TRIGGER_EXIT, REFLUX_HALL_SOUTHWEST_FOOT, CONDITION_ALWAYS,
	 REFLUX_HALL_SOUTHWEST_FOOT},
	{TRIGGER_EXIT, REFLUX_PUZZLE_ROOM_DOORWAY, CONDITION_ALWAYS,
	 REFLUX_PUZZLE_ROOM_DOORWAY},
	{TRIGGER_STEP, CELL_BEHAVIOR_TUNDARIA_TOWER_ICE, CONDITION_ALWAYS,
	 EVENT(scTundariaTowerSlideOnIce)},
	{TRIGGER_STEP, CELL_BEHAVIOR_TUNDARIA_TOWER_REFLUX_APPEARS,
	 FLAG_REFLUX_APPEARED, EVENT(scTundariaTowerRefluxAppears)},
	{TRIGGER_TALK, ACTOR_REFLUX, CONDITION_ALWAYS,
	 EVENT(scTundariaTowerFightReflux)},
	{TRIGGER_TABLE_END},
};

TRIGGER TundariaTowerIceColumnsTriggers[] = {
	{TRIGGER_EXIT, ICE_COLUMNS_CHEST_ROOM_FOOT, CONDITION_ALWAYS,
	 ICE_COLUMNS_CHEST_ROOM_FOOT},
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS,
	 TUNDARIA_TOWER_ICE_COLUMNS_TO_TUNDARIA_TOWER_REFLUX_NORTHWEST},
	{TRIGGER_EXIT, ICE_COLUMNS_NORTH_ROOM_EAST_DOORWAY, CONDITION_ALWAYS,
	 ICE_COLUMNS_NORTH_ROOM_EAST_DOORWAY},
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, ICE_COLUMNS_HALL_WEST_DOORWAY,
	 CONDITION_ALWAYS, ICE_COLUMNS_HALL_WEST_DOORWAY},
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 5, CONDITION_ALWAYS,
	 TUNDARIA_TOWER_ICE_COLUMNS_TO_TUNDARIA_TOWER_REFLUX_NORTH},
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 6, CONDITION_ALWAYS,
	 TUNDARIA_TOWER_ICE_COLUMNS_TO_TUNDARIA_TOWER_REFLUX_NORTHEAST},
	{TRIGGER_EXIT, 7, CONDITION_ALWAYS,
	 TUNDARIA_TOWER_ICE_COLUMNS_TO_TUNDARIA_TOWER_PILLAR_PUZZLE_NORTH},
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, ICE_COLUMNS_CORRIDOR_SOUTH_DOORWAY,
	 CONDITION_ALWAYS, ICE_COLUMNS_CORRIDOR_SOUTH_DOORWAY},
	{TRIGGER_EXIT, 9, CONDITION_ALWAYS,
	 TUNDARIA_TOWER_ICE_COLUMNS_TO_TUNDARIA_TOWER_PILLAR_PUZZLE_SOUTHWEST},
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 10, CONDITION_ALWAYS,
	 TUNDARIA_TOWER_ICE_COLUMNS_TO_TUNDARIA_TOWER_PILLAR_PUZZLE_SOUTH},
	{TRIGGER_STEP, CELL_BEHAVIOR_TUNDARIA_TOWER_SLIDE_TOP, CONDITION_ALWAYS,
	 EVENT(scTundariaTowerJumpDownSlide)},
	{TRIGGER_STEP, CELL_BEHAVIOR_TUNDARIA_TOWER_ICE, CONDITION_ALWAYS,
	 EVENT(scTundariaTowerSlideOnIce)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS),
	 ACTOR_FIRST_ICE_COLUMN, FLAG_ICE_COLUMN_POUNDED,
	 EVENT(scTundariaTowerPoundIceColumn)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS),
	 ACTOR_FIRST_ICE_COLUMN + 1, FLAG_ICE_COLUMN_POUNDED + 1,
	 EVENT(scTundariaTowerPoundIceColumn)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS),
	 ACTOR_FIRST_ICE_COLUMN + 2, FLAG_ICE_COLUMN_POUNDED + 2,
	 EVENT(scTundariaTowerPoundIceColumn)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS),
	 ACTOR_FIRST_ICE_COLUMN + 3, FLAG_ICE_COLUMN_POUNDED + 3,
	 EVENT(scTundariaTowerPoundIceColumn)},
	{TRIGGER_TABLE_END},
};

TRIGGER TundariaTowerPillarPuzzleTriggers[] = {
	{TRIGGER_EXIT, 1, CONDITION_ALWAYS,
	 TUNDARIA_TOWER_PILLAR_PUZZLE_TO_TUNDARIA_TOWER_ICE_COLUMNS_NORTH},
	{TRIGGER_EXIT, 2, CONDITION_ALWAYS,
	 TUNDARIA_TOWER_PILLAR_PUZZLE_TO_TUNDARIA_TOWER_ICE_COLUMNS_SOUTHWEST},
	{TRIGGER_EXIT, 3, CONDITION_ALWAYS,
	 TUNDARIA_TOWER_PILLAR_PUZZLE_TO_TUNDARIA_TOWER_BURST_BROOCH_SOUTH},
	{TRIGGER_EXIT, 4, CONDITION_ALWAYS,
	 TUNDARIA_TOWER_PILLAR_PUZZLE_TO_TUNDARIA_TOWER_CENTER_PRONG_NORTH},
	{TRIGGER_EXIT, 5, CONDITION_ALWAYS,
	 TUNDARIA_TOWER_PILLAR_PUZZLE_TO_TUNDARIA_TOWER_CENTER_PRONG_SOUTH},
	{TRIGGER_EXIT, PILLAR_PUZZLE_MIRROR_ROOM_FOOT, CONDITION_ALWAYS,
	 PILLAR_PUZZLE_MIRROR_ROOM_FOOT},
	{TRIGGER_EXIT, PILLAR_PUZZLE_CORRIDOR_DOORWAY, CONDITION_ALWAYS,
	 PILLAR_PUZZLE_CORRIDOR_DOORWAY},
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 9, CONDITION_ALWAYS,
	 TUNDARIA_TOWER_PILLAR_PUZZLE_TO_TUNDARIA_TOWER_ICE_COLUMNS_SOUTH},
	{TRIGGER_EXIT, PILLAR_PUZZLE_SLIDE_HOLE, CONDITION_ALWAYS,
	 PILLAR_PUZZLE_FROM_SLIDE},
	{TRIGGER_STEP, CELL_BEHAVIOR_TUNDARIA_TOWER_SLIDE_END, CONDITION_ALWAYS,
	 EVENT(scTundariaTowerStopSliding)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS),
	 ACTOR_FIRST_ICE_COLUMN, FLAG_ICE_COLUMN_POUNDED,
	 EVENT(scTundariaTowerPoundIceColumn)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS),
	 ACTOR_FIRST_ICE_COLUMN + 1, FLAG_ICE_COLUMN_POUNDED + 1,
	 EVENT(scTundariaTowerPoundIceColumn)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS),
	 ACTOR_FIRST_ICE_COLUMN + 2, FLAG_ICE_COLUMN_POUNDED + 2,
	 EVENT(scTundariaTowerPoundIceColumn)},
	{TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(scTundariaTowerStartPillarMove)},
	{TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(scTundariaTowerSetDownPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_FIRST),
	 ACTOR_FIRST_SLIDING_PILLAR, CONDITION_ALWAYS,
	 EVENT(scTundariaTowerResolveBeforePsynergy)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
	 ACTOR_FIRST_SLIDING_PILLAR, CONDITION_ALWAYS,
	 EVENT(scTundariaTowerStartPillarMove)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_ACTS),
	 ACTOR_FIRST_SLIDING_PILLAR, CONDITION_ALWAYS,
	 EVENT(scTundariaTowerSlidePillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
	 ACTOR_SECOND_LEDGE_PILLAR, FLAG_SECOND_LEDGE_PILLAR_SET,
	 EVENT(scTundariaTowerStartPillarMove)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SECOND_LEDGE_PILLAR,
	 FLAG_SECOND_LEDGE_PILLAR_SET, EVENT(scTundariaTowerSetDownPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
	 ACTOR_FIRST_LEDGE_PILLAR, FLAG_FIRST_LEDGE_PILLAR_SET,
	 EVENT(scTundariaTowerStartPillarMove)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_LEDGE_PILLAR,
	 FLAG_FIRST_LEDGE_PILLAR_SET, EVENT(scTundariaTowerSetDownPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_FIRST),
	 ACTOR_SECOND_SLIDING_PILLAR, CONDITION_ALWAYS,
	 EVENT(scTundariaTowerResolveBeforePsynergy)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
	 ACTOR_SECOND_SLIDING_PILLAR, CONDITION_ALWAYS,
	 EVENT(scTundariaTowerStartPillarMove)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_ACTS),
	 ACTOR_SECOND_SLIDING_PILLAR, CONDITION_ALWAYS,
	 EVENT(scTundariaTowerSlidePillar)},
	{PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
	 CONDITION_ALWAYS, EVENT(scTundariaTowerRevealHiddenWay)},
	{PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
	 CONDITION_ALWAYS, EVENT(scTundariaTowerEndRevealHiddenWay)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_FIRST),
	 CELL_BEHAVIOR_TUNDARIA_TOWER_ICY_WALL, CONDITION_ALWAYS,
	 EVENT(scTundariaTowerResolveBeforePsynergy)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
	 CELL_BEHAVIOR_TUNDARIA_TOWER_ICY_WALL, FLAG_MIRROR_WALL_BURST,
	 EVENT(scTundariaTowerBurstMirrorWall)},
	{TRIGGER_TABLE_END},
};

TRIGGER TundariaTowerBurstBroochTriggers[] = {
	{TRIGGER_EXIT, 2, CONDITION_ALWAYS,
	 TUNDARIA_TOWER_BURST_BROOCH_TO_TUNDARIA_TOWER_PILLAR_PUZZLE_SOUTH},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
	 ACTOR_BROOCH_ROOM_PILLAR, CONDITION_ALWAYS,
	 EVENT(scTundariaTowerStartPillarMove)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_BROOCH_ROOM_PILLAR,
	 CONDITION_ALWAYS, EVENT(scTundariaTowerSetDownPillar)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
	 CELL_BEHAVIOR_TUNDARIA_TOWER_BROWN_WALL, FLAG_BROWN_WALL_BURST,
	 EVENT(scTundariaTowerBurstBrownWall)},
	{TRIGGER_TABLE_END},
};
#undef EFFECT_SET_MODE

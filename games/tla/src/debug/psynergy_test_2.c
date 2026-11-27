/* @file */
/* The second debug room of Psynergy tests, on the east side of the Psynergy */
/* test's map, where a fire, a stake sunk flat and two posts stand: Douse and */
/* Pound take the fire away, Grind stands the stake up and Tremor on it does */
/* nothing, talking to it scatters rocks, and a rope is coiled between the */
/* posts. Its Burst row answers the Psynergy test's first Scoop spot, west of */
/* this room's view, and its battle row a cell behavior no cell holds. */

#include "field/field.h"
#include "game/game.h"
#include "field/event_inline.h"
#include "field/field_sequence_event.h"
#include "field/call.h"
#include "window/message_speed.h"

#include "scene/falling_rocks.h"

/* Set once the step onto CELL_BEHAVIOR_BATTLE_STEP has started its battle. */
#define FLAG_BATTLE_FOUGHT FLAG_SCENE_TEMP_20

/* The cell behaviors the room's triggers answer. */
typedef enum PSYNERGY_TEST2_CELL_BEHAVIOR {
	/* A step here starts a battle; no cell of the map holds it. */
	CELL_BEHAVIOR_BATTLE_STEP = 30,
	/* The Psynergy test's first Scoop spot, cell (10, 10), west of this room.
	 */
	CELL_BEHAVIOR_HOLE_SPOT = 50
} PSYNERGY_TEST2_CELL_BEHAVIOR;

/* The room's actors, numbered in the order of ActorPlacements. */
typedef enum PSYNERGY_TEST2_ACTOR_ID {
	ACTOR_FIRE = ACTOR_FIRST_PLACED, /* Douse and Pound take it off the map */
	ACTOR_STAKE, /* Grind and Tremor are tried on it; talked to, rocks fly */
	ACTOR_FIRST_POST,  /* the rope's first end; Lash at it does nothing */
	ACTOR_SECOND_POST, /* the rope's other end */
} PSYNERGY_TEST2_ACTOR_ID;

#define ROCK_FRAMES 20 /* how long a rock flies */

/* The rocks a talk scatters, in the menu's heap slot: the scenes' struct */
/* ROCK_STATE with each rock read as a particle. */
typedef struct ROCK_PARTICLE_STATE {
	u8 tiles[ROCK_SHEET_BYTES];
	PARTICLE rocks[ROCK_COUNT];
	EFFECT_SPR sprs[ROCK_COUNT];
	s16 next;
	u16 tileBase;
	s16 resource;
} ROCK_PARTICLE_STATE;

void dbPsynergyTest2SpawnRocks(s32 x, s32 y, s32 z, s32 count);

void dbPsynergyTest2StartRocks(void);
void dbPsynergyTest2EndRocks(void);

const SCENE_ENTRANCE *scGetEntrances(void) { return SceneEntrances; }

const SCENE_REGION *scGetExitRegions(void) { return 0; }

const u32 *scGetExits(void) { return SceneExits; }

const ACTOR_PLACE *scGetActorPlacements(void) { return ActorPlacements; }

/* Douse on ACTOR_FIRE takes it off the map. */
void dbPsynergyTest2DouseFire(void) { fdActorSetPos(ACTOR_FIRE, 0, 0); }

/* So does Pound, as its fist lands. */
void dbPsynergyTest2PoundFire(s32 stage) {
	if(stage == POUND_STAGE_LANDED)
		fdActorSetPos(ACTOR_FIRE, 0, 0);
}

/* The event for Tremor on ACTOR_STAKE, which does nothing. */
void dbPsynergyTest2IgnoreTremor(void) {}

/* Grind on ACTOR_STAKE stands the stone stake up again. */
void dbPsynergyTest2GrindStake(void) {
	fdActorSetAnimationById(ACTOR_STAKE, 3);
}

/* Burst at CELL_BEHAVIOR_HOLE_SPOT, as its great blast dies down, copies the */
/* 1 by 2 block of cells at (49, 1) over (34, 1), and its cell behaviors over */
/* (34, 3). */
void dbPsynergyTest2BurstCells(s32 stage) {
	if(stage == BURST_STAGE_DYING_DOWN) {
		fdMapGridCopyCellsLayered(49, 1, 1, 2, 34, 1);
		fdMapGridCopyCellAttributes(49, 1, 1, 2, 34, 3);
	}
}

/* Stepping onto CELL_BEHAVIOR_BATTLE_STEP starts a battle, the first time */
/* only; no cell holds it, so it never runs. */
void dbPsynergyTest2StepIntoBattle(void) {
	if(!fdEventGetFlag(FLAG_BATTLE_FOUGHT)) {
		fdEventSetFlag(FLAG_BATTLE_FOUGHT);
		fdRequestBattleFromEncounterTable(
			ENCOUNTER_ZONE_WORLD_MAP_BEACH_AND_GRASSLAND, 0);
	}
}

/* Talking to ACTOR_STAKE: twenty rocks burst out around it and fall for 20 */
/* frames. */
void dbPsynergyTest2ScatterRocks(s32 id) {
	ACTOR *actor = fdActorGet(id);

	dbPsynergyTest2StartRocks();
	dbPsynergyTest2SpawnRocks(actor->x.fixed, actor->y.fixed, actor->z.fixed,
							  20);
	syWait(20);
	dbPsynergyTest2EndRocks();
}

const TRIGGER *scGetTriggers(void) { return SceneEvents; }

/* Fades the room in over the backdrop, sets the fast message speed, coils the
 */
/* rope and shows the stake sunk flat. */
s32 scInit(void) {
	FIELD_STATE *state = Ram_HeapSlots->fieldState;

	state->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	state->sceneAttributeHigh = 0;
	state->sceneAttributeLow = 0;
	MainRoutineStateWork.msgSpeed = MESSAGE_SPEED_FAST;
	fdRopeInit();
	/* Coils rope 0 between the two posts. */
	Value3((s32 (*)())fdRopeCoil, 0, ACTOR_FIRST_POST, ACTOR_SECOND_POST);
	fdActorSetAnimation(ACTOR_STAKE, 4);
	return 0;
}

s32 scPrepareMap(void) { return 0; }

/* Each frame of the rocks: each rock younger than ROCK_FRAMES is drawn, its */
/* picture one of eight as it ages, and falls. */
void dbPsynergyTest2UpdateRocks(void) {
	ROCK_PARTICLE_STATE *state = Ram_HeapSlots->menuState;
	PARTICLE *particle = state->rocks;
	EFFECT_SPR *sprite = state->sprs;
	s32 i;
	s32 step = -toFIXED(1) / 2;

	for(i = 0; i < ROCK_COUNT; i++) {
		if((u32)particle->variant < ROCK_FRAMES) {
			sprite->oam.idx =
				state->tileBase + ((particle->variant >> 2) & 7) * 8;
			fdEffectSprDrawAtWorldPos(sprite, particle);
			fdBattleEffectStepVector3(particle, 63, step);
			particle->variant++;
		}
		sprite++;
		particle++;
	}
}

SCENE_ENTRANCE SceneEntrances[] = {
	{0, CONDITION_ALWAYS, 408, 0, 88, FACING_SOUTH, 0, 280, 0, 780, 464, 0},
	{1, CONDITION_ALWAYS, 408, 0, 408, FACING_NORTH, 0, 280, 0, 780, 464, 0},
	{2, CONDITION_ALWAYS, 504, 0, 232, FACING_SOUTH, 0, 280, 0, 780, 464, 0},
	{SCENE_TABLE_END},
};

#include "../scene/shared/psynergy_test_exits.c"

ACTOR_PLACE ActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0,
					104, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0,
					248, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 360, 0, 104, FACING_SOUTH,
					TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 488, 0, 136, FACING_SOUTH,
					TALK_FACING_KEEP, 1),
	{SCENE_TABLE_END},
};

TRIGGER SceneEvents[] = {
	{TRIGGER_TALK, ACTOR_STAKE, CONDITION_ALWAYS,
	 EVENT(dbPsynergyTest2ScatterRocks)},
	{TRIGGER_STEP, CELL_BEHAVIOR_BATTLE_STEP, CONDITION_ALWAYS,
	 EVENT(dbPsynergyTest2StepIntoBattle)},
	{PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_FIRE, CONDITION_ALWAYS,
	 EVENT(dbPsynergyTest2DouseFire)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_FIRE,
	 CONDITION_ALWAYS, EVENT(dbPsynergyTest2PoundFire)},
	{PSYNERGY_ON_ACTOR(ABILITY_TREMOR), ACTOR_STAKE, CONDITION_ALWAYS,
	 EVENT(dbPsynergyTest2IgnoreTremor)},
	{PSYNERGY_ON_ACTOR(ABILITY_GRIND), ACTOR_STAKE, CONDITION_ALWAYS,
	 EVENT(dbPsynergyTest2GrindStake)},
	{PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_FIRST_POST, CONDITION_ALWAYS, 0},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
	 CELL_BEHAVIOR_HOLE_SPOT, CONDITION_ALWAYS,
	 EVENT(dbPsynergyTest2BurstCells)},
	{TRIGGER_TABLE_END},
};

/* Throws `count` rocks out of (x, y, z): each starts 8 pixels or more away */
/* at a random angle and flies up and outward. */
void dbPsynergyTest2SpawnRocks(s32 x, s32 y, s32 z, s32 count) {
	ROCK_PARTICLE_STATE *state = Ram_HeapSlots->menuState;
	PARTICLE *rock;
	s32 angle;
	s32 i;

	if(count > 0) {
		i = count;
		do {
			angle = syRandom();
			rock = &state->rocks[state->next];
			rock->variant = 0;
			rock->x = x;
			rock->y = y;
			rock->z = z;
			syFieldVectorAddPolarOffsetFX((syRandom() << 3) + toFIXED(8), angle,
										  &rock->x);
			rock->velX = 0;
			rock->velY = toFIXED(5);
			rock->velZ = 0;
			syFieldVectorAddPolarOffsetFX((syRandom() << 1) + toFIXED(2), angle,
										  &rock->velX);
			state->next++;
			state->next &= ROCK_COUNT - 1;
			i--;
		} while(i != 0);
	}
}

/* Loads the rocks' tiles, hides every rock and starts */
/* dbPsynergyTest2UpdateRocks. */
void dbPsynergyTest2StartRocks(void) {
	ROCK_PARTICLE_STATE *state =
		syHeapSlotAlloc(HEAP_SLOT_MENU, sizeof(ROCK_PARTICLE_STATE));
	s32 entry;
	s32 tile;
	s32 i;
	PARTICLE *rock;
	EFFECT_SPR *sprite;

	syDecompressTaggedLz(syResourceGetTableEntry(RESOURCE_FALLING_ROCK), state);
	entry = syVramCacheFindFreeEntry();
	tile = mnVramLoad(entry, ROCK_SHEET_BYTES, state);
	state->tileBase = tile;
	state->resource = entry;
	rock = state->rocks;
	for(i = 0; i < ROCK_COUNT; i++) {
		sprite = &state->sprs[i];
		fdEffectSprInit(sprite, 8, 8, OBJ_SIZE_16X16, tile);
		sprite->oam.fullCol = 1;
		sprite->oam.pal = 0;
		sprite->oam.pri = 0;
		sprite->depth = OAM_DEPTH_WINDOW;
		rock->variant = -1;
		rock++;
	}
	state->next = 0;
	syFieldTaskAddFrame(dbPsynergyTest2UpdateRocks, TASK_PASS_FRAME);
}

/* Stops the rocks and frees their tiles and state. */
void dbPsynergyTest2EndRocks(void) {
	ROCK_PARTICLE_STATE *state = Ram_HeapSlots->menuState;

	syFieldTaskRemove(dbPsynergyTest2UpdateRocks);
	syVramCacheFreeEntry(state->resource);
	syHeapSlotRelease(HEAP_SLOT_MENU);
}

/* Unused: the Psynergy test's dbPsynergyTestDigCells again. */
UNUSED void dbPsynergyTest2DigCells(s32 stage, s32 sourceX, s32 sourceY,
									s32 width, s32 height, s32 destinationX,
									s32 destinationY) {
	if(stage == SCOOP_STAGE_DUG)
		fdMapGridCopyCells(sourceX, sourceY, width, height, destinationX,
						   destinationY);
}

/* Unused: the Psynergy test's dbPsynergyTestGrowActor again. */
UNUSED void dbPsynergyTest2GrowActor(s32 stage, s32 id, s32 x, s32 z) {
	ACTOR *actor;
	s32 scale;

	actor = fdActorGet(id);
	if(stage == SCOOP_STAGE_DUG) {
		fdActorSetPos(id, x, z);
		actor->scaleX = 0;
		actor->scaleY = 0;
	}
	if(stage == SCOOP_STAGE_DUG_UP) {
		while(actor->scaleX < FIXED_ONE) {
			syWait(1);
			scale = actor->scaleX;
			scale += toFIXED(0.02);
			actor->scaleX = scale;
			actor->scaleY = scale;
		}
		actor->scaleX = FIXED_ONE;
		actor->scaleY = FIXED_ONE;
	}
}

/* Unused: the Psynergy test's dbPsynergyTestDropActor again. */
UNUSED void dbPsynergyTest2DropActor(s32 stage, s32 id, s32 x, s32 z) {
	ACTOR *actor;

	actor = fdActorGet(id);
	if(stage == SCOOP_STAGE_DUG_UP) {
		fdActorSetPos(id, x, z);
		actor->y.fixed = toFIXED(32);
		actor->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
		actor->gravity = toFIXED(0.09);
		syWait(50);
	}
}

/* Unused: the Psynergy test's dbPsynergyTestRaiseStatue again. */
UNUSED void dbPsynergyTest2RaiseStatue(s32 stage, s32 x, s32 z, s32 row,
									   s32 column) {
	ACTOR *actor;

	if(stage == SCOOP_STAGE_DUG) {
		actor =
			scFieldActorCreate(SPRITE_BROWN_BOULDER, toFIXED(x), 0, toFIXED(z));
		if(actor != NULL) {
			fdActorSetSprFlags(actor, 0);
			actor->priFlg = ACTOR_PRIORITY_UNDERFOOT;
		}
	}
	if(stage == SCOOP_STAGE_DUG_UP) {
		actor = (scFieldActorCreate(SPRITE_WINGED_STATUE, toFIXED(x), 0,
									toFIXED(z)));
		if(actor != NULL) {
			s32 scale;

			fdActorSetSprFlags(actor, 0);
			actor->velY = toFIXED(6);
			actor->scaleX = FIXED_ONE * 2 / 5;
			actor->scaleY = FIXED_ONE * 2 / 5;
			while(actor->scaleX < FIXED_ONE) {
				syWait(1);
				scale = actor->scaleX + toFIXED(0.02);
				actor->facing += FACING_STEP;
				actor->scaleX = scale;
				actor->scaleY = scale;
			}
			actor->scaleX = FIXED_ONE;
			actor->scaleY = FIXED_ONE;
			actor->facing =
				fdActorGet(MainRoutineStateWork.leaderActorId)->facing +
				DEGtoANG(180);
			fdRequestBattleFromEncounterTable(row, column);
		}
	}
}

/* The second debug room of Psynergy tests, on the east side of the Psynergy
   test's map, where a fire, a stake sunk flat and two posts stand: Douse and
   Pound take the fire away, Grind stands the stake up and Tremor on it does
   nothing, talking to it scatters rocks, and a rope is coiled between the
   posts. Its Burst row answers the Psynergy test's first Scoop spot, west of
   this room's view, and its battle row a metatile behavior no metatile
   holds. */

#include "field/field.h"
#include "game/game.h"
#include "field/script_inline.h"
#include "field/field_mode_script.h"
#include "field/call.h"
#include "window/message_speed.h"

#include "scene/falling_rocks.h"

/* Set once the step onto METATILE_BEHAVIOR_BATTLE_STEP has started its
   battle. */
#define FLAG_BATTLE_FOUGHT FLAG_SCENE_TEMP_20

/* The metatile behaviors the room's cues answer. */
typedef enum PSYNERGY_TEST2_METATILE_BEHAVIOR {
	/* A step here starts a battle; no metatile of the map holds it. */
	METATILE_BEHAVIOR_BATTLE_STEP = 30,
	/* The Psynergy test's first Scoop spot, metatile (10, 10), west of this
	   room. */
	METATILE_BEHAVIOR_HOLE_SPOT = 50
} PSYNERGY_TEST2_METATILE_BEHAVIOR;

/* The room's actors, numbered in the order of ActorPlacements. */
typedef enum PSYNERGY_TEST2_ACTOR_ID {
	ACTOR_FIRE = ACTOR_FIRST_PLACED, /* Douse and Pound take it off the map */
	ACTOR_STAKE, /* Grind and Tremor are tried on it; talked to, rocks fly */
	ACTOR_FIRST_POST,  /* the rope's first end; Lash at it does nothing */
	ACTOR_SECOND_POST, /* the rope's other end */
} PSYNERGY_TEST2_ACTOR_ID;

#define ROCK_TICKS 20 /* how long a rock flies */

/* The rocks a talk scatters, in the menu's heap slot: the scenes' struct
   ROCK_STATE with each rock read as a particle. */
typedef struct ROCK_PARTICLE_STATE {
	Uint8	   tiles[ROCK_SHEET_BYTES];
	PARTICLE   rocks[ROCK_COUNT];
	EFFECT_SPR sprites[ROCK_COUNT];
	Sint16	   next;
	Uint16	   tileBase;
	Sint16	   resource;
} ROCK_PARTICLE_STATE;

void dbPsynergyTest2SpawnRocks(Sint32 x, Sint32 y, Sint32 z, Sint32 count);

void dbPsynergyTest2StartRocks(void);
void dbPsynergyTest2EndRocks(void);

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

const ACTOR_PLACE *scGetActorPlacements(void)
{
	return ActorPlacements;
}

/* Douse on ACTOR_FIRE takes it off the map. */
void dbPsynergyTest2DouseFire(void)
{
	fdActorSetPos(ACTOR_FIRE, 0, 0);
}

/* So does Pound, as its fist lands. */
void dbPsynergyTest2PoundFire(Sint32 stage)
{
	if(stage == POUND_STAGE_LANDED)
		fdActorSetPos(ACTOR_FIRE, 0, 0);
}

/* The script for Tremor on ACTOR_STAKE, which does nothing. */
void dbPsynergyTest2IgnoreTremor(void)
{
}

/* Grind on ACTOR_STAKE stands the stone stake up again. */
void dbPsynergyTest2GrindStake(void)
{
	fdActorSetAnimationById(ACTOR_STAKE, 3);
}

/* Burst at METATILE_BEHAVIOR_HOLE_SPOT, as its great blast dies down, copies
   the 1 by 2 block of metatiles at (49, 1) over (34, 1), and its metatile
   behaviors over (34, 3). */
void dbPsynergyTest2BurstMetatiles(Sint32 stage)
{
	if(stage == BURST_STAGE_DYING_DOWN) {
		fdMapGridCopyMetatilesLayered(49, 1, 1, 2, 34, 1);
		fdMapGridCopyMetatileAttributes(49, 1, 1, 2, 34, 3);
	}
}

/* Stepping onto METATILE_BEHAVIOR_BATTLE_STEP starts a battle, the first time
   only; no metatile holds it, so it never runs. */
void dbPsynergyTest2StepIntoBattle(void)
{
	if(!fdScriptGetFlag(FLAG_BATTLE_FOUGHT)) {
		fdScriptSetFlag(FLAG_BATTLE_FOUGHT);
		fdRequestBattleFromEncounterTable(
			ENCOUNTER_ZONE_WORLD_MAP_BEACH_AND_GRASSLAND, 0);
	}
}

/* Talking to ACTOR_STAKE: twenty rocks burst out around it and fall for 20
   ticks. */
void dbPsynergyTest2ScatterRocks(Sint32 id)
{
	ACTOR *actor = fdActorGet(id);

	dbPsynergyTest2StartRocks();
	dbPsynergyTest2SpawnRocks(
		actor->x.fixed, actor->y.fixed, actor->z.fixed, 20);
	syWait(20);
	dbPsynergyTest2EndRocks();
}

const CUE *scGetCues(void)
{
	return SceneCues;
}

/* Fades the room in over the backdrop, sets the fast message speed, coils the
   rope and shows the stake sunk flat. */
Sint32 scInit(void)
{
	FIELD_STATE *state = RamHeapSlots->fieldState;

	state->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	state->sceneAttributeHigh = 0;
	state->sceneAttributeLow = 0;
	MainRoutineStateWork.msgSpeed = MESSAGE_SPEED_FAST;
	fdRopeInit();
	/* Coils rope 0 between the two posts. */
	Value3((Sint32 (*)())fdRopeCoil, 0, ACTOR_FIRST_POST, ACTOR_SECOND_POST);
	fdActorSetAnimation(ACTOR_STAKE, 4);
	return 0;
}

Sint32 scPrepareMap(void)
{
	return 0;
}

/* Each tick of the rocks: each rock younger than ROCK_TICKS is drawn, its
   picture one of eight as it ages, and falls. */
void dbPsynergyTest2UpdateRocks(void)
{
	ROCK_PARTICLE_STATE *state = RamHeapSlots->menuState;
	PARTICLE			*particle = state->rocks;
	EFFECT_SPR			*sprite = state->sprites;
	Sint32				 i;
	Sint32				 step = -toFIXED(1) / 2;

	for(i = 0; i < ROCK_COUNT; i++) {
		if((Uint32)particle->variant < ROCK_TICKS) {
			sprite->oam.index =
				state->tileBase + ((particle->variant >> 2) & 7) * 8;
			fdEffectSpriteDrawAtWorldPos(sprite, particle);
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
		MOVEMENT_TYPE_STAND, 360, 0, 104, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS,
		MOVEMENT_TYPE_STAND, 488, 0, 136, FACING_SOUTH, TALK_FACING_KEEP, 1),
	{SCENE_TABLE_END},
};

CUE SceneCues[] = {
	{CUE_TALK, ACTOR_STAKE, CONDITION_ALWAYS,
		SCRIPT_REF(dbPsynergyTest2ScatterRocks)},
	{CUE_STEP, METATILE_BEHAVIOR_BATTLE_STEP, CONDITION_ALWAYS,
		SCRIPT_REF(dbPsynergyTest2StepIntoBattle)},
	{PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_FIRE, CONDITION_ALWAYS,
		SCRIPT_REF(dbPsynergyTest2DouseFire)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_FIRE,
		CONDITION_ALWAYS, SCRIPT_REF(dbPsynergyTest2PoundFire)},
	{PSYNERGY_ON_ACTOR(ABILITY_TREMOR), ACTOR_STAKE, CONDITION_ALWAYS,
		SCRIPT_REF(dbPsynergyTest2IgnoreTremor)},
	{PSYNERGY_ON_ACTOR(ABILITY_GRIND), ACTOR_STAKE, CONDITION_ALWAYS,
		SCRIPT_REF(dbPsynergyTest2GrindStake)},
	{PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_FIRST_POST, CONDITION_ALWAYS, 0},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
		METATILE_BEHAVIOR_HOLE_SPOT, CONDITION_ALWAYS,
		SCRIPT_REF(dbPsynergyTest2BurstMetatiles)},
	{CUE_TABLE_END},
};

/* Throws `count` rocks out of (x, y, z): each starts 8 pixels or more away at a
   random angle and flies up and outward. */
void dbPsynergyTest2SpawnRocks(Sint32 x, Sint32 y, Sint32 z, Sint32 count)
{
	ROCK_PARTICLE_STATE *state = RamHeapSlots->menuState;
	PARTICLE			*rock;
	Sint32				 angle;
	Sint32				 i;

	if(count > 0) {
		i = count;
		do {
			angle = syRandom();
			rock = &state->rocks[state->next];
			rock->variant = 0;
			rock->x = x;
			rock->y = y;
			rock->z = z;
			syFieldVectorAddPolarOffsetFX(
				(syRandom() << 3) + toFIXED(8), angle, &rock->x);
			rock->velocityX = 0;
			rock->velocityY = toFIXED(5);
			rock->velocityZ = 0;
			syFieldVectorAddPolarOffsetFX(
				(syRandom() << 1) + toFIXED(2), angle, &rock->velocityX);
			state->next++;
			state->next &= ROCK_COUNT - 1;
			i--;
		} while(i != 0);
	}
}

/* Loads the rocks' tiles, hides every rock and starts
   dbPsynergyTest2UpdateRocks. */
void dbPsynergyTest2StartRocks(void)
{
	ROCK_PARTICLE_STATE *state =
		syHeapSlotAllocate(HEAP_SLOT_MENU, sizeof(ROCK_PARTICLE_STATE));
	Sint32		entry;
	Sint32		tile;
	Sint32		i;
	PARTICLE   *rock;
	EFFECT_SPR *sprite;

	syDecompressTaggedLZ(syResourceGetTableEntry(RESOURCE_FALLING_ROCK), state);
	entry = syVRAMCacheFindFreeEntry();
	tile = mnVRAMLoad(entry, ROCK_SHEET_BYTES, state);
	state->tileBase = tile;
	state->resource = entry;
	rock = state->rocks;
	for(i = 0; i < ROCK_COUNT; i++) {
		sprite = &state->sprites[i];
		fdEffectSpriteInit(sprite, 8, 8, OBJ_SIZE_16X16, tile);
		sprite->oam.fullColor = 1;
		sprite->oam.palette = 0;
		sprite->oam.priority = 0;
		sprite->depth = OAM_DEPTH_WINDOW;
		rock->variant = -1;
		rock++;
	}
	state->next = 0;
	syFieldSetTickEvent(dbPsynergyTest2UpdateRocks, EVENT_PASS_TICK);
}

/* Stops the rocks and frees their tiles and state. */
void dbPsynergyTest2EndRocks(void)
{
	ROCK_PARTICLE_STATE *state = RamHeapSlots->menuState;

	syFieldCloseEvent(dbPsynergyTest2UpdateRocks);
	syVRAMCacheFreeEntry(state->resource);
	syHeapSlotRelease(HEAP_SLOT_MENU);
}

/* Unused: the Psynergy test's dbPsynergyTestDigMetatiles again. */
UNUSED void dbPsynergyTest2DigMetatiles(Sint32 stage, Sint32 srcX, Sint32 srcY,
	Sint32 width, Sint32 height, Sint32 destX, Sint32 destY)
{
	if(stage == SCOOP_STAGE_DUG)
		fdMapGridCopyMetatiles(srcX, srcY, width, height, destX, destY);
}

/* Unused: the Psynergy test's dbPsynergyTestGrowActor again. */
UNUSED void dbPsynergyTest2GrowActor(
	Sint32 stage, Sint32 id, Sint32 x, Sint32 z)
{
	ACTOR *actor;
	Sint32 scale;

	actor = fdActorGet(id);
	if(stage == SCOOP_STAGE_DUG) {
		fdActorSetPos(id, x, z);
		actor->scaleX = 0;
		actor->scaleY = 0;
	}
	if(stage == SCOOP_STAGE_DUG_UP) {
		while(actor->scaleX < toFIXED(1)) {
			syWait(1);
			scale = actor->scaleX;
			scale += toFIXED(0.02);
			actor->scaleX = scale;
			actor->scaleY = scale;
		}
		actor->scaleX = toFIXED(1);
		actor->scaleY = toFIXED(1);
	}
}

/* Unused: the Psynergy test's dbPsynergyTestDropActor again. */
UNUSED void dbPsynergyTest2DropActor(
	Sint32 stage, Sint32 id, Sint32 x, Sint32 z)
{
	ACTOR *actor;

	actor = fdActorGet(id);
	if(stage == SCOOP_STAGE_DUG_UP) {
		fdActorSetPos(id, x, z);
		actor->y.fixed = toFIXED(32);
		actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
		actor->gravity = toFIXED(0.09);
		syWait(50);
	}
}

/* Unused: the Psynergy test's dbPsynergyTestRaiseStatue again. */
UNUSED void dbPsynergyTest2RaiseStatue(
	Sint32 stage, Sint32 x, Sint32 z, Sint32 row, Sint32 column)
{
	ACTOR *actor;

	if(stage == SCOOP_STAGE_DUG) {
		actor =
			scFieldActorCreate(SPRITE_BROWN_BOULDER, toFIXED(x), 0, toFIXED(z));
		if(actor != NULL) {
			fdActorSetSpriteFlags(actor, 0);
			actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
		}
	}
	if(stage == SCOOP_STAGE_DUG_UP) {
		actor = (scFieldActorCreate(
			SPRITE_WINGED_STATUE, toFIXED(x), 0, toFIXED(z)));
		if(actor != NULL) {
			Sint32 scale;

			fdActorSetSpriteFlags(actor, 0);
			actor->velocityY = toFIXED(6);
			actor->scaleX = toFIXED(2) / 5;
			actor->scaleY = toFIXED(2) / 5;
			while(actor->scaleX < toFIXED(1)) {
				syWait(1);
				scale = actor->scaleX + toFIXED(0.02);
				actor->facing += FACING_STEP;
				actor->scaleX = scale;
				actor->scaleY = scale;
			}
			actor->scaleX = toFIXED(1);
			actor->scaleY = toFIXED(1);
			actor->facing =
				fdActorGet(MainRoutineStateWork.leaderActorId)->facing +
				DEGtoANG(180);
			fdRequestBattleFromEncounterTable(row, column);
		}
	}
}

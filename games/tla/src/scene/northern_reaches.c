/* The Northern Reaches, the icy sea where a great wall of ice blocks the ship's
   way north to Prox.

   Sailing up to the wall before the ship carries the cannon from Loho, the crew
   calls Kraden to see it. With the cannon aboard, Kraden says to use it and to
   take the ship between the two blocks of ice before the wall for shelter.
   There, Felix fires the Magma Ball, the wall bursts in fire and is gone, and
   the ship sails off north. */

#include "field/field_sprite.h"
#include "graphics/palette_buffers.h"
#include "field/field.h"
#include "game/game.h"
#include "field/field_mode_script.h"
#include "field/script_inline.h"
#include "scene/scene.h"
#include "text/msg_ids.h"
#include "field/actor_script.h"

/* The Magma Ball has shattered the ice wall. */
#define FLAG_ICE_WALL_SHATTERED 0x90e

/* Piers has said the ship is at a safe distance. */
#define FLAG_SAFE_DISTANCE_TOLD 0x90d

/* The crew has called Kraden to see the ice wall, for this visit. */
#define FLAG_ICE_WALL_SIGHTED FLAG_SCENE_TEMP_00

/* The crew has talked the ice wall over. */
#define FLAG_ICE_WALL_DISCUSSED 0x90c

/* The metatiles of the sea before the ice wall that start its scripts: a row
   close under the wall, (90, 11) to (96, 11); the gap between the two blocks of
   ice, (92, 13) and (93, 13); and a row further south, (89, 15) to (91, 15). */
enum {
	METATILE_UNDER_ICE_WALL = 22,
	METATILE_BETWEEN_ICE_BLOCKS = 21,
	METATILE_SOUTH_OF_ICE_WALL = 20
};

/* Ways in that no exit reaches. */
enum {
	/* Unused: the party below the ice wall, which still stands. */
	ENTRANCE_BELOW_ICE_WALL = 2,
	/* Unused: the party below the ice wall, drawn already gone. */
	ENTRANCE_ICE_WALL_GONE = 3,
	/* Unused: loads the cannon aboard, hands the ship to the player and enters
	   again by the north way in. */
	ENTRANCE_ABOARD_SHIP_WITH_CANNON = 90
};

/* The Northern Reaches, by the rows of `ActorPlacements`; the rows for Jenna,
   Sheba and Piers go to their party slots. */
typedef enum NORTHERN_REACHES_ACTOR_ID {
	/* The party's ship, placed blank; it sails off north once the wall is
	   gone. */
	ACTOR_SHIP = ACTOR_FIRST_PLACED,
	ACTOR_MAGMA_BALL_SHOT, /* the Magma Ball fired at the ice wall */
	ACTOR_CANNON_SMOKE,	   /* a puff of smoke at the bow as the cannon fires */
	ACTOR_KRADEN
} NORTHERN_REACHES_ACTOR_ID;

void scNorthernReachesShatterIceWall(
	FIXED *blast, FIXED *pos, void (*func)(void));

extern Sint32 NorthernReachesShipAtRest[];

/* One of the ice wall's shards: its place and speed in 16.16 pixels, as a
   PARTICLE has them, and how many ticks it has flown, counted unsigned. */
typedef struct ICE_SHARD_OBJ {
	FIXED  x;
	FIXED  y;
	FIXED  z;
	FIXED  velX;
	FIXED  velY;
	FIXED  velZ;
	Uint32 age;
} ICE_SHARD_OBJ;

/* One of the sparks the Magma Ball's explosion throws over the wall: the link
   the sprite list threads it by, its sprite, and its age in ticks, negative
   while it waits to appear. */
typedef struct EXPLOSION_SPARK {
	struct EXPLOSION_SPARK *next;
	SPARK_OAM				oam;
	Sint32					age;
} EXPLOSION_SPARK;

/* The wall breaks into 54 shards, nine columns of six, and the explosion's
   sparks (`EXPLOSION_SPARK_COUNT`) fly about it. */
#define ICE_SHARD_COUNT	  54
/* How long a shard flies, in ticks. */
#define ICE_SHARD_TICKS	  60
/* The buffer a sprite sheet is unpacked into. */
#define SHEET_BUFFER_SIZE 4096

/* The shattering's state: the shards' sprite sheet, and each shard's sprite and
   motion. */
typedef struct ICE_WALL_STATE {
	Uint8		  sheet[SHEET_BUFFER_SIZE];
	EFFECT_SPR	  sprs[ICE_SHARD_COUNT];
	ICE_SHARD_OBJ shards[ICE_SHARD_COUNT];
} ICE_WALL_STATE;

#include "shared/ship_drop_shadow.h"

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

/* Draws the ice wall gone, as the Magma Ball leaves it and on every later
   visit. */
void scNorthernReachesClearIceWall(void)
{
	fdMapGridCopyMetatilesLayered(47, 9, 9, 4, 25, 9);
	fdMapGridCopyMetatilesLayered(47, 69, 9, 10, 25, 69);
	fdMapGridCopyMetatileAttributes(111, 8, 9, 5, 89, 8);
}

/* The ship sails up to the ice wall, metatiles (90, 11) to (96, 11), before it
   carries the cannon: Jenna calls "Kraden, come look at this... There's a huge
   ice wall blocking our path," and Kraden says they will go no farther unless
   they do something about it. Once a visit. */
void scNorthernReachesSightIceWall(void)
{
	if(fdScriptGetFlag(FLAG_ICE_WALL_SIGHTED) != 0)
		return;
	fdScriptSetFlag(FLAG_ICE_WALL_SIGHTED);
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	syFieldScriptSetMessageId((Sint32)MsgShipIceWallFirstApproach);
	scFieldActorWaitForMove(fdActorGet(ACTOR_SHIP));
	fdActorFaceDirectionAndWait(ACTOR_SHIP, FACING_NORTH);
	syFieldScriptWait(5);
	/* "Kraden, come look at this... There's a huge ice wall blocking our..." */
	syFieldScriptShowMessageThenWait(ACTOR_JENNA, 0, 5);
	/* "Hm... If we can't do something about that wall, we're not going
	   any..." */
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 5);
	fdScriptEnd();
}

/* The ship, carrying the cannon, sails north toward the ice wall, metatiles
   (89, 15) to (91, 15): the view turns to the wall, and Kraden says to use the
   cannon they got in Loho and to take the ship between the two blocks of ice
   before it for shelter. Only once. */
void scNorthernReachesDiscussIceWall(void)
{
	if(fdScriptGetFlag(FLAG_ICE_WALL_DISCUSSED) != 0)
		return;
	fdScriptSetFlag(FLAG_ICE_WALL_DISCUSSED);
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	syFieldScriptSetMessageId((Sint32)MsgShipIceWallCrewReposition);
	scFieldActorWaitForMove(fdActorGet(ACTOR_SHIP));
	syFieldScriptWait(5);
	/* "Kraden, come look at this... There's a huge ice wall blocking our..." */
	syFieldScriptShowMessageThenWait(ACTOR_JENNA, 0, 5);
	fdCameraMoveTo(464, CAMERA_KEEP, 176, 1);
	fdCameraWaitForArrival();
	syFieldScriptWait(5);
	fdActorSetPos(ACTOR_KRADEN, 408, 288);
	fdActorSetPos(ACTOR_JENNA, 408, 288);
	fdActorSetPos(ACTOR_SHEBA, 408, 288);
	/* "Nothing to worry about, Jenna. We'll just have to use that cannon
	   we..." */
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 5);
	/* "That's right! If the cannon can blast away a stone wall, think
	   what..." */
	syFieldScriptShowMessageThenWait(ACTOR_SHEBA, 0, 5);
	/* "Felix, take us between those two blocks of ice for shelter when
	   you..." */
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 5);
	fdActorSetPos(ACTOR_KRADEN, 0, 0);
	fdActorSetPos(ACTOR_JENNA, 0, 0);
	fdActorSetPos(ACTOR_SHEBA, 0, 0);
	fdScriptEnd();
}

/* Shrinks an actor evenly by step, as the ship sails off into the distance. */
#define fdActorShrinkBy(actor, step) \
	{ \
		Sint32 scale = (actor)->scaleY; \
\
		(actor)->scaleY = scale - (step); \
		(actor)->scaleX = scale - (step); \
	}

/* The Magma Ball grows from half its size to full, a thirty-second a tick,
   over its first MAGMA_BALL_GROW_TICKS ticks of flight. */
#define MAGMA_BALL_GROW_TICKS 16
/* The ship's last shrink as it sails off: SHIP_SHRINK_STEPS steps of seven
   ticks. */
#define SHIP_SHRINK_STEPS	  20

/* The ship, carrying the cannon, comes between the two blocks of ice before the
   ice wall, metatile (92, 13) or (93, 13). Piers says they should be safe
   firing from here, and Kraden asks Felix to aim at the wall and fire when
   ready. With a Yes the ship jolts back as the cannon fires, the Magma Ball
   flies into the wall, which bursts in flames and white light and is gone, and
   after the crew's cheers the ship sails off north. With a No it pulls back to
   try again. */
void scNorthernReachesFireMagmaBall(void)
{
	FIXED  from[3];
	ACTOR *ship;
	ACTOR *ball;
	ACTOR *smoke;
	FIXED  to[3];
	Sint32 cnt, msg;
	Sint32 told;

	ball = fdActorGet(ACTOR_MAGMA_BALL_SHOT);
	smoke = fdActorGet(ACTOR_CANNON_SMOKE);
	ship = fdActorGet(ACTOR_SHIP);
	cnt = fdScriptGetFlag(FLAG_ICE_WALL_SHATTERED);
	if(cnt != 0)
		return;
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	syFieldScriptSetMessageId((Sint32)MsgShipSafeFiringDistance);
	fdActorMoveToAndWait(ACTOR_SHIP, 464, 216);
	fdActorFaceDirectionAndWait(ACTOR_SHIP, FACING_NORTH);
	syFieldScriptWait(5);
	fdActorGet(ACTOR_KRADEN)->scaleX = 0;
	fdActorGet(ACTOR_PIERS)->scaleX = 0;
	fdActorGet(ACTOR_JENNA)->scaleX = 0;
	fdActorGet(ACTOR_SHEBA)->scaleX = 0;
	fdActorSetSprFlags(fdActorGet(ACTOR_KRADEN), 0);
	fdActorSetSprFlags(fdActorGet(ACTOR_PIERS), 0);
	fdActorSetSprFlags(fdActorGet(ACTOR_JENNA), 0);
	fdActorSetSprFlags(fdActorGet(ACTOR_SHEBA), 0);
	fdActorSetPos(ACTOR_KRADEN, 464, 216);
	fdActorSetPos(ACTOR_PIERS, 464, 216);
	fdActorSetPos(ACTOR_JENNA, 464, 216);
	fdActorSetPos(ACTOR_SHEBA, 464, 216);
	told = fdScriptGetFlag(FLAG_SAFE_DISTANCE_TOLD);
	msg = (Sint32)MsgShipSafeFiringDistance;
	if(told == 0) {
		fdScriptSetFlag(FLAG_SAFE_DISTANCE_TOLD);
		/* "We should be safe if we fire from this distance." */
		syFieldScriptShowMessageThenWait(
			SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0, 5);
	}
	syFieldScriptSetMessageId((msg + 1));
	/* "OK, here we go! Felix, aim right at the ice wall and fire when ready!"
	   Yes: fire; No: wait. */
	fdScriptOpenMessage(ACTOR_KRADEN, 0);
	if(syFieldScriptAskYesNoWithActorAnimation(ACTOR_FELIX, 0)) {
		/* "What? You're not ready yet? Take your time and get into
		   position..." */
		syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 5);
		fdActorSetDestinationOffset(ACTOR_SHIP, 0, 16);
		scFieldActorWaitForMove(ship);
		syFieldScriptWait(5);
	} else {
		FieldStateWork->msg++;
		/* "Fire the Magma Ball!" */
		syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 5);
		fdActorSetSpeed(ACTOR_SHIP, 3);
		fdActorGet(ACTOR_SHIP)->steeringFlg &= ~ACTOR_FACES_MOTION;
		fdActorWalkBy(ACTOR_SHIP, 0, 16);
		fdScriptPlaySnd(SE_CLATTER);
		syFieldScriptWait(4);
		smoke->scaleX = toFIXED(3) / 2;
		smoke->scaleY = toFIXED(3) / 2;
		fdActorSetSprColMode(fdActorGet(ACTOR_CANNON_SMOKE), SPRITE_COLOR_GREY);
		fdActorSetPos(ACTOR_CANNON_SMOKE, 464, 200);
		fdActorSetAnimation(ACTOR_CANNON_SMOKE, 1);
		ball->scaleX = toFIXED(1) / 2;
		ball->scaleY = toFIXED(1) / 2;
		fdActorSetSpeed(ACTOR_MAGMA_BALL_SHOT, 2);
		fdActorSetPos(ACTOR_MAGMA_BALL_SHOT, 464, 216);
		fdActorWalkTo(ACTOR_MAGMA_BALL_SHOT, 464, 144);
		for(cnt = MAGMA_BALL_GROW_TICKS - 1; cnt >= 0; cnt--) {
			ball->scaleX += toFIXED(1) / 32;
			ball->scaleY += toFIXED(1) / 32;
			syWait(1);
		}
		fdCameraMoveTo(472, CAMERA_KEEP, 176, 1);
		fdCameraWaitForArrival();
		scFieldActorWaitForMoveById(ACTOR_MAGMA_BALL_SHOT);
		fdScriptPlaySnd(SE_CRASH);
		from[0] = ((ball->x.fixed) - toFIXED(2));
		from[1] = (ball->y.fixed);
		from[2] = ((ball->z.fixed) - toFIXED(8));
		to[0] = toFIXED(400);
		to[1] = 0;
		to[2] = toFIXED(80);
		scNorthernReachesShatterIceWall(
			from, to, scNorthernReachesClearIceWall);
		syFieldScriptWait(20);
		fdActorSetPos(ACTOR_CANNON_SMOKE, 0, 0);
		fdActorSetPos(ACTOR_MAGMA_BALL_SHOT, 0, 0);
		FieldStateWork->transitionFrm = 100;
		fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
		fdScriptStartScreenTransition();
		fdScriptWaitTransitionTicks();
		FieldStateWork->transitionFrm = 8;
		/* "Great! We did it! We can keep going now!" */
		syFieldScriptShowMessageThenWait(
			SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0, 5);
		/* "All right, let's head for the Northern Seas." */
		syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 5);
		/* "Let's go!" */
		syFieldScriptShowMessageThenWait(
			SPEAKER_PORTRAIT_RIGHT | ACTOR_SHEBA, 0, 5);
		fdActorSetSpeed(ACTOR_SHIP, 0.8);
		fdActorMoveToAndWait(ACTOR_SHIP, 464, 176);
		syScriptCameraFollowActor(ACTOR_SHIP, 1);
		fdActorMoveToAndWait(ACTOR_SHIP, 464, 152);
		fdActorSetSpeed(ACTOR_SHIP, 0.6);
		/* Off toward the horizon: five steps of 16 pixels north, written out as
		   the script's other steps are, the ship a sixteenth smaller at
		   each. */
		fdActorShrinkBy(ship, toFIXED(1) / 16);
		fdActorSetDestinationOffset(ACTOR_SHIP, 0, -16);
		scFieldActorWaitForMoveById(ACTOR_SHIP);
		fdActorShrinkBy(ship, toFIXED(1) / 16);
		fdActorSetDestinationOffset(ACTOR_SHIP, 0, -16);
		scFieldActorWaitForMoveById(ACTOR_SHIP);
		fdActorShrinkBy(ship, toFIXED(1) / 16);
		fdActorSetDestinationOffset(ACTOR_SHIP, 0, -16);
		scFieldActorWaitForMoveById(ACTOR_SHIP);
		fdActorShrinkBy(ship, toFIXED(1) / 16);
		fdActorSetDestinationOffset(ACTOR_SHIP, 0, -16);
		scFieldActorWaitForMoveById(ACTOR_SHIP);
		fdActorShrinkBy(ship, toFIXED(1) / 16);
		fdActorSetDestinationOffset(ACTOR_SHIP, 0, -16);
		scFieldActorWaitForMoveById(ACTOR_SHIP);
		fdActorSetSpeed(ACTOR_SHIP, 0.2);
		fdActorSetDestinationOffset(ACTOR_SHIP, 0, -12);
		for(cnt = 0; cnt < SHIP_SHRINK_STEPS; cnt++) {
			fdActorShrinkBy(ship, toFIXED(9) / 256);
			syWait(7);
		}
		ship->scaleY = 0;
		ship->scaleX = 0;
		syFieldScriptWait(10);
		fdScriptSetFlag(FLAG_ICE_WALL_SHATTERED);
		syFieldScriptSetExitRequest(NORTHERN_REACHES_TO_WORLD_MAP_NORTH);
	}
	fdActorSetPos(ACTOR_KRADEN, 0, 0);
	fdActorSetPos(ACTOR_JENNA, 0, 0);
	fdActorSetPos(ACTOR_SHEBA, 0, 0);
	fdActorSetPos(ACTOR_PIERS, 0, 0);
	fdScriptEnd();
}

const CUE *scGetCues(void)
{
	return SceneCues;
}

Sint32 scInit(void)
{
	MAIN_ROUTINE_STATE *state;

	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	state = &MainRoutineStateWork;
	fdActorGet(state->leaderActorId)->priFlg |= ACTOR_PRIORITY_IGNORES_RISES;
	if(state->entrance == ENTRANCE_ABOARD_SHIP_WITH_CANNON) {
		fdScriptSetFlag(FLAG_SHIP_CARRIES_CANNON);
		/* The player steers the ship, actor 8, in place of the leader. */
		state->ownerOverride = 1;
		fdScriptSetDestination(
			SCENE_NORTHERN_REACHES, NORTHERN_REACHES_FROM_WORLD_MAP_NORTH);
	}
	fdRisingParticlesStart();
	return 0;
}

Sint32 scPrepareMap(void)
{
	if(MainRoutineStateWork.entrance == ENTRANCE_ICE_WALL_GONE)
		scNorthernReachesClearIceWall();
	if(fdScriptGetFlag(FLAG_ICE_WALL_SHATTERED))
		scNorthernReachesClearIceWall();
	return 0;
}

/* The Magma Ball's explosion over the ice wall: sparks fly, the screen fades to
   white, `func` draws the wall gone and its shards fly apart from
   `pos`. The caller passes where the ball struck as `blast`, but nothing
   reads it: the sparks burst at random over a fixed stretch of the screen, 52
   to 179 pixels across and 16 to 79 down, wherever the ball landed. */
void scNorthernReachesShatterIceWall(
	FIXED *blast, FIXED *pos, void (*func)(void))
{
	ICE_WALL_STATE	*state;
	ICE_SHARD_OBJ	*shard;
	EFFECT_SPR		*spr;
	EFFECT_SPR		*flySpr;
	EXPLOSION_SPARK *sparks;
	EXPLOSION_SPARK *spark;
	/* LEFTOVER(frame): an unused local the ROM's frame reserves and never
	 * touches. */
	Uint8  frmSlot[4];
	void  *sheet;
	Sint32 i, frm, column, entry, sparkEntry, tiles, flying, sparkTiles;
	Sint32 size;

	state = syHeapSlotAlloc(HEAP_SLOT_MENU, sizeof(ICE_WALL_STATE));
	flying = 0;
	syDecompressTaggedLZ(
		syResourceGetTableEntry(RESOURCE_FIELD_EFFECT_ICE_SHARD), state);
	entry = syVRAMCacheFindFreeEntry();
	tiles = syVRAMCacheLoad(entry, 1280, state);
	i = 0;
	shard = state->shards;
	spr = state->sprs;
	for(; i < ICE_SHARD_COUNT; i++) {
		fdEffectSprInit(spr, 0, 0, OBJ_SIZE_16X16, (i & 3) * 8 + tiles);
		spr->depth = EFFECT_SPRITE_FRONT_DEPTH;
		spr->oam.pri = 0;
		spr->oam.fullCol = 1;
		spr->oam.pal = 0;
		column = i / 6;
		shard->x = pos[0] + FIELD_METATILE_ORIGIN(column);
		shard->y = pos[1] - FIELD_METATILE_ORIGIN(i % 6);
		column -= 4;
		spr++;
		shard->z = pos[2];
		shard->velX = (syRandom() * 3 >> 1) * column;
		shard->velY = (syRandom() << 2) + toFIXED(2);
		shard->velZ = 0;
		shard->age = 0;
		shard++;
	}
	/* FAKEMATCH(temporary): size, the sprite sheet's size held in a local
	 * before the sparks are allocated; as SHEET_BUFFER_SIZE at the
	 * allocation. */
	size = SHEET_BUFFER_SIZE;
	sparks = syHeapAllocEWRAM(EXPLOSION_SPARK_COUNT * sizeof(EXPLOSION_SPARK));
	sheet = syHeapAlloc(size);
	syDecompressTaggedLZ(
		syResourceGetTableEntry(RESOURCE_FIELD_EFFECT_EXPLOSION), sheet);
	sparkEntry = syVRAMCacheFindFreeEntry();
	sparkTiles = syVRAMCacheLoad(sparkEntry, 1024, sheet);
	syHeapRelease(sheet);
	for(i = 0; i < EXPLOSION_SPARK_COUNT; i++)
		sparks[i].age = -(syRandom() << 5 >> 16);
	syDMAXCopy(FieldExplosionPalette,
		(void *)&OBJ_PLTT_COLOR(EXPLOSION_SPARK_PALETTE, 0),
		DMA_COPY_HALFWORDS(PALETTE_COLORS), REG_DMA3);
	for(frm = 0; frm <= 149; frm++) {
		if((frm & 31) == 0) {
			fdScriptPlaySnd(SE_CRASH);
		}
		if(frm == 60) {
			fdPaletteFadeSetTarget(RGB_WHITE, PALETTES_ALL);
			fdPaletteFadeStartInterpolation(90);
			fdScriptPlaySnd(SE_RUMBLE);
		}
		if(frm == 105) {
			func();
			flying = 1;
			fdMapGridSetShake(6, 6, 1);
			fdScriptPlaySnd(SE_SPARKLING_PASSAGE);
		}
		if(frm == 130) {
			flying = 1;
		}
		if(flying != 0) {
			shard = state->shards;
			flySpr = state->sprs;
			for(i = ICE_SHARD_COUNT - 1; i >= 0; i--) {
				if(shard->age < ICE_SHARD_TICKS) {
					fdEffectSprDrawAtWorldPos(flySpr, &shard->x);
					fdBattleEffectStepVector3(&shard->x, 63, -toFIXED(3) / 16);
					shard->age++;
				}
				flySpr++;
				shard++;
			}
		}
		spark = sparks;
		do {
			if(spark->age == 0) {
				spark->oam.words.first = OBJ_SIZE_16X16;
				spark->oam.words.second = 0;
				spark->oam.oam.x = (syRandom() << 7 >> 16) + 52;
				spark->oam.oam.y = (syRandom() << 6 >> 16) + 16;
				spark->oam.oam.pal = EXPLOSION_SPARK_PALETTE;
				spark->oam.oam.pri = 0;
			}
			if(spark->age >= 0) {
				spark->oam.oam.idx = sparkTiles + (spark->age >> 2) * 4;
				syOAMAddSprAtDepthClamped(spark, EXPLOSION_SPARK_DEPTH);
			}
			spark->age++;
			if(spark->age >= EXPLOSION_SPARK_TICKS) {
				spark->age = -(syRandomBelow(7));
			}
			spark++;
		} while((Sint32)spark <= (Sint32)&sparks[EXPLOSION_SPARK_COUNT - 1]);
		sySynch(1);
	}
	syVRAMCacheFreeEntry(sparkEntry);
	syHeapRelease(sparks);
	syFieldScriptWait(40);
	fdMapGridSetShake(0, 0, 0);
	syVRAMCacheFreeEntry(entry);
	syHeapSlotRelease(HEAP_SLOT_MENU);
}

/* The ship at rest on the sea, without its shadow, on the middle layer. */
Sint32 NorthernReachesShipAtRest[] = {
	SC_CALL((Sint32)scShipDropShadow),
	SC_ANIMATION(7),
	SC_END,
};

SCENE_ENTRANCE SceneEntrances[] = {
	{0, CONDITION_ALWAYS, 440, 0, 520, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{ENTRANCE_BELOW_ICE_WALL, CONDITION_ALWAYS, 470, 0, 192, FACING_NORTH, 0,
		-1, -1, -1, -1, 0},
	{ENTRANCE_ICE_WALL_GONE, CONDITION_ALWAYS, 470, 0, 192, FACING_NORTH, 0, -1,
		-1, -1, -1, 0},
	{SCENE_TABLE_END},
};

Uint32 SceneExits[] = {
	SCENE_EXITS(NorthernReaches),
	SCENE_EXIT_UNLESS(NORTHERN_REACHES_TO_WORLD_MAP_SOUTH, WorldMap,
		WORLD_MAP_FROM_NORTHERN_REACHES_SOUTH, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(NORTHERN_REACHES_TO_WORLD_MAP_NORTH, WorldMap,
		WORLD_MAP_FROM_NORTHERN_REACHES_NORTH, EXIT_FLAG_NONE),
	SCENE_EXITS_END,
};

ACTOR_PLACE ActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS,
		(Sint32)NorthernReachesShipAtRest, 120, 0, 280, FACING_SOUTH,
		TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_EXPLOSION, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		-56, 0, -56, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_DUST_CLOUD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		-56, 0, -56, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56,
		0, -56, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0,
		-56, FACING_NORTHEAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0,
		-56, FACING_NORTHEAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0,
		-56, FACING_NORTHEAST, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

CUE SceneCues[] = {
	{CUE_EXIT, NORTHERN_REACHES_TO_WORLD_MAP_SOUTH, CONDITION_ALWAYS,
		NORTHERN_REACHES_TO_WORLD_MAP_SOUTH},
	{CUE_EXIT, NORTHERN_REACHES_TO_WORLD_MAP_NORTH, CONDITION_ALWAYS,
		NORTHERN_REACHES_TO_WORLD_MAP_NORTH},
	{CUE_STEP, METATILE_SOUTH_OF_ICE_WALL,
		IF_FLAG_SET(FLAG_SHIP_CARRIES_CANNON),
		SCRIPT_REF(scNorthernReachesDiscussIceWall)},
	{CUE_STEP, METATILE_BETWEEN_ICE_BLOCKS,
		IF_FLAG_SET(FLAG_SHIP_CARRIES_CANNON),
		SCRIPT_REF(scNorthernReachesFireMagmaBall)},
	{CUE_STEP, METATILE_UNDER_ICE_WALL, FLAG_SHIP_CARRIES_CANNON,
		SCRIPT_REF(scNorthernReachesSightIceWall)},
	{CUE_TABLE_END},
};

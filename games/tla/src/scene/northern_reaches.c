/* @file */
/* The Northern Reaches, the icy sea where a great wall of ice blocks the */
/* ship's way north to Prox. */
/* */
/* Sailing up to the wall before the ship carries the cannon from Loho, the */
/* crew calls Kraden to see it. With the cannon aboard, Kraden says to use it */
/* and to take the ship between the two blocks of ice before the wall for */
/* shelter. There, Felix fires the Magma Ball, the wall bursts in fire */
/* and is gone, and the ship sails off north. */

#include "field/field_sprite.h"
#include "graphics/palette_buffers.h"
#include "field/field.h"
#include "game/game.h"
#include "field/field_sequence_event.h"
#include "field/event_inline.h"
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

/* The cells of the sea before the ice wall that start its events: a row */
/* close under the wall, (90, 11) to (96, 11); the gap between the two blocks */
/* of ice, (92, 13) and (93, 13); and a row further south, (89, 15) to */
/* (91, 15). */
enum {
	CELL_UNDER_ICE_WALL = 22,
	CELL_BETWEEN_ICE_BLOCKS = 21,
	CELL_SOUTH_OF_ICE_WALL = 20
};

/* Ways in that no exit reaches. */
enum {
	/* Unused: the party below the ice wall, which still stands. */
	ENTRANCE_BELOW_ICE_WALL = 2,
	/* Unused: the party below the ice wall, drawn already gone. */
	ENTRANCE_ICE_WALL_GONE = 3,
	/* Unused: loads the cannon aboard, hands the ship to the player and enters
	 */
	/* again by the north way in. */
	ENTRANCE_ABOARD_SHIP_WITH_CANNON = 90
};

/* The Northern Reaches, by the rows of `ActorPlacements`; the rows for */
/* Jenna, Sheba and Piers go to their party slots. */
typedef enum NORTHERN_REACHES_ACTOR_ID {
	/* The party's ship, placed blank; it sails off north once the wall is gone.
	 */
	ACTOR_SHIP = ACTOR_FIRST_PLACED,
	ACTOR_MAGMA_BALL_SHOT, /* the Magma Ball fired at the ice wall */
	ACTOR_CANNON_SMOKE,	   /* a puff of smoke at the bow as the cannon fires */
	ACTOR_KRADEN
} NORTHERN_REACHES_ACTOR_ID;

void scNorthernReachesShatterIceWall(FIXED *blast, FIXED *position,
									 void (*callback)(void));

extern s32 NorthernReaches_ShipAtRest[];

/* One of the ice wall's shards: its place and speed in 16.16 pixels, as a */
/* PARTICLE has them, and how many frames it has flown, counted */
/* unsigned. */
typedef struct ICE_SHARD_OBJ {
	FIXED x;
	FIXED y;
	FIXED z;
	FIXED velX;
	FIXED velY;
	FIXED velZ;
	u32 age;
} ICE_SHARD_OBJ;

/* One of the sparks the Magma Ball's explosion throws over the wall: the link
 */
/* the sprite list threads it by, its sprite, and its age in frames, negative */
/* while it waits to appear. */
typedef struct EXPLOSION_SPARK {
	struct EXPLOSION_SPARK *next;
	SPARK_OAM oam;
	s32 age;
} EXPLOSION_SPARK;

/* The wall breaks into 54 shards, nine columns of six, and the explosion's */
/* sparks (`EXPLOSION_SPARK_COUNT`) fly about it. */
#define ICE_SHARD_COUNT 54
/* How long a shard flies, in frames. */
#define ICE_SHARD_FRAMES 60
/* The buffer a sprite sheet is unpacked into. */
#define SHEET_BUFFER_SIZE 4096

/* The shattering's state: the shards' sprite sheet, and each shard's sprite */
/* and motion. */
typedef struct ICE_WALL_STATE {
	u8 sheet[SHEET_BUFFER_SIZE];
	EFFECT_SPR sprs[ICE_SHARD_COUNT];
	ICE_SHARD_OBJ shards[ICE_SHARD_COUNT];
} ICE_WALL_STATE;

#include "shared/ship_drop_shadow.h"

const SCENE_ENTRANCE *scGetEntrances(void) { return SceneEntrances; }

const SCENE_REGION *scGetExitRegions(void) { return 0; }

const u32 *scGetExits(void) { return SceneExits; }

const ACTOR_PLACE *scGetActorPlacements(void) { return ActorPlacements; }

/* Draws the ice wall gone, as the Magma Ball leaves it and on every later */
/* visit. */
void scNorthernReachesClearIceWall(void) {
	fdMapGridCopyCellsLayered(47, 9, 9, 4, 25, 9);
	fdMapGridCopyCellsLayered(47, 69, 9, 10, 25, 69);
	fdMapGridCopyCellAttributes(111, 8, 9, 5, 89, 8);
}

/* The ship sails up to the ice wall, cells (90, 11) to (96, 11), before it */
/* carries the cannon: Jenna calls "Kraden, come look at this... There's a huge
 */
/* ice wall blocking our path," and Kraden says they will go no farther unless
 */
/* they do something about it. Once a visit. */
void scNorthernReachesSightIceWall(void) {
	if(fdEventGetFlag(FLAG_ICE_WALL_SIGHTED) != 0)
		return;
	fdEventSetFlag(FLAG_ICE_WALL_SIGHTED);
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	syFieldEventSetMessageId((s32)MsgShipIceWallFirstApproach);
	scFieldActorWaitForMove(fdActorGet(ACTOR_SHIP));
	fdActorFaceDirectionAndWait(ACTOR_SHIP, FACING_NORTH);
	syFieldEventWait(5);
	/* "Kraden, come look at this... There's a huge ice wall blocking our..." */
	syFieldEventShowMessageThenWait(ACTOR_JENNA, 0, 5);
	/* "Hm... If we can't do something about that wall, we're not going any..."
	 */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 5);
	fdEventEnd();
}

/* The ship, carrying the cannon, sails north toward the ice wall, cells */
/* (89, 15) to (91, 15): the view turns to the wall, and Kraden says to use the
 */
/* cannon they got in Loho and to take the ship between the two blocks of ice */
/* before it for shelter. Only once. */
void scNorthernReachesDiscussIceWall(void) {
	if(fdEventGetFlag(FLAG_ICE_WALL_DISCUSSED) != 0)
		return;
	fdEventSetFlag(FLAG_ICE_WALL_DISCUSSED);
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	syFieldEventSetMessageId((s32)MsgShipIceWallCrewReposition);
	scFieldActorWaitForMove(fdActorGet(ACTOR_SHIP));
	syFieldEventWait(5);
	/* "Kraden, come look at this... There's a huge ice wall blocking our..." */
	syFieldEventShowMessageThenWait(ACTOR_JENNA, 0, 5);
	fdCameraMoveTo(464, CAMERA_KEEP, 176, 1);
	fdCameraWaitForArrival();
	syFieldEventWait(5);
	fdActorSetPos(ACTOR_KRADEN, 408, 288);
	fdActorSetPos(ACTOR_JENNA, 408, 288);
	fdActorSetPos(ACTOR_SHEBA, 408, 288);
	/* "Nothing to worry about, Jenna. We'll just have to use that cannon we..."
	 */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 5);
	/* "That's right! If the cannon can blast away a stone wall, think what..."
	 */
	syFieldEventShowMessageThenWait(ACTOR_SHEBA, 0, 5);
	/* "Felix, take us between those two blocks of ice for shelter when you..."
	 */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 5);
	fdActorSetPos(ACTOR_KRADEN, 0, 0);
	fdActorSetPos(ACTOR_JENNA, 0, 0);
	fdActorSetPos(ACTOR_SHEBA, 0, 0);
	fdEventEnd();
}

/* Shrinks an actor evenly by step, as the ship sails off into the distance. */
#define fdActorShrinkBy(actor, step)                                           \
	{                                                                          \
		s32 scale = (actor)->scaleY;                                           \
                                                                               \
		(actor)->scaleY = scale - (step);                                      \
		(actor)->scaleX = scale - (step);                                      \
	}

/* The Magma Ball grows from half its size to full, a thirty-second a frame, */
/* over its first MAGMA_BALL_GROW_FRAMES frames of flight. */
#define MAGMA_BALL_GROW_FRAMES 16
/* The ship's last shrink as it sails off: SHIP_SHRINK_STEPS steps of seven */
/* frames. */
#define SHIP_SHRINK_STEPS 20

/* The ship, carrying the cannon, comes between the two blocks of ice before */
/* the ice wall, cell (92, 13) or (93, 13). Piers says they should be safe */
/* firing from here, and Kraden asks Felix to aim at the wall and fire when */
/* ready. With a Yes the ship jolts back as the cannon fires, the Magma Ball */
/* flies into the wall, which bursts in flames and white light and is gone, */
/* and after the crew's cheers the ship sails off north. With a No it pulls */
/* back to try again. */
void scNorthernReachesFireMagmaBall(void) {
	FIXED from[3];
	ACTOR *ship;
	ACTOR *ball;
	ACTOR *smoke;
	FIXED to[3];
	s32 count, message;
	s32 told;

	ball = fdActorGet(ACTOR_MAGMA_BALL_SHOT);
	smoke = fdActorGet(ACTOR_CANNON_SMOKE);
	ship = fdActorGet(ACTOR_SHIP);
	count = fdEventGetFlag(FLAG_ICE_WALL_SHATTERED);
	if(count != 0)
		return;
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	syFieldEventSetMessageId((s32)MsgShipSafeFiringDistance);
	fdActorMoveToAndWait(ACTOR_SHIP, 464, 216);
	fdActorFaceDirectionAndWait(ACTOR_SHIP, FACING_NORTH);
	syFieldEventWait(5);
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
	told = fdEventGetFlag(FLAG_SAFE_DISTANCE_TOLD);
	message = (s32)MsgShipSafeFiringDistance;
	if(told == 0) {
		fdEventSetFlag(FLAG_SAFE_DISTANCE_TOLD);
		/* "We should be safe if we fire from this distance." */
		syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0,
										5);
	}
	syFieldEventSetMessageId((message + 1));
	/* "OK, here we go! Felix, aim right at the ice wall and fire when ready!"
	 */
	/* Yes: fire; No: wait. */
	fdEventOpenMessage(ACTOR_KRADEN, 0);
	if(syFieldEventAskYesNoWithActorAnimation(ACTOR_FELIX, 0)) {
		/* "What? You're not ready yet? Take your time and get into position..."
		 */
		syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 5);
		fdActorSetDestinationOffset(ACTOR_SHIP, 0, 16);
		scFieldActorWaitForMove(ship);
		syFieldEventWait(5);
	}
	else {
		FieldStateWork->msg++;
		/* "Fire the Magma Ball!" */
		syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 5);
		fdActorSetSpeed(ACTOR_SHIP, 3);
		fdActorGet(ACTOR_SHIP)->steeringFlg &= ~ACTOR_FACES_MOTION;
		fdActorWalkBy(ACTOR_SHIP, 0, 16);
		fdEventPlaySnd(SE_CLATTER);
		syFieldEventWait(4);
		smoke->scaleX = FIXED_ONE * 3 / 2;
		smoke->scaleY = FIXED_ONE * 3 / 2;
		fdActorSetSprColMode(fdActorGet(ACTOR_CANNON_SMOKE), SPRITE_COLOR_GREY);
		fdActorSetPos(ACTOR_CANNON_SMOKE, 464, 200);
		fdActorSetAnimation(ACTOR_CANNON_SMOKE, 1);
		ball->scaleX = FIXED_ONE / 2;
		ball->scaleY = FIXED_ONE / 2;
		fdActorSetSpeed(ACTOR_MAGMA_BALL_SHOT, 2);
		fdActorSetPos(ACTOR_MAGMA_BALL_SHOT, 464, 216);
		fdActorWalkTo(ACTOR_MAGMA_BALL_SHOT, 464, 144);
		for(count = MAGMA_BALL_GROW_FRAMES - 1; count >= 0; count--) {
			ball->scaleX += FIXED_ONE / 32;
			ball->scaleY += FIXED_ONE / 32;
			syWait(1);
		}
		fdCameraMoveTo(472, CAMERA_KEEP, 176, 1);
		fdCameraWaitForArrival();
		scFieldActorWaitForMoveById(ACTOR_MAGMA_BALL_SHOT);
		fdEventPlaySnd(SE_CRASH);
		from[0] = ((ball->x.fixed) - toFIXED(2));
		from[1] = (ball->y.fixed);
		from[2] = ((ball->z.fixed) - toFIXED(8));
		to[0] = toFIXED(400);
		to[1] = 0;
		to[2] = toFIXED(80);
		scNorthernReachesShatterIceWall(from, to,
										scNorthernReachesClearIceWall);
		syFieldEventWait(20);
		fdActorSetPos(ACTOR_CANNON_SMOKE, 0, 0);
		fdActorSetPos(ACTOR_MAGMA_BALL_SHOT, 0, 0);
		FieldStateWork->transitionFrm = 100;
		fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
		fdEventStartScrTransition();
		fdEventWaitTransitionFrames();
		FieldStateWork->transitionFrm = 8;
		/* "Great! We did it! We can keep going now!" */
		syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0,
										5);
		/* "All right, let's head for the Northern Seas." */
		syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 5);
		/* "Let's go!" */
		syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_SHEBA, 0,
										5);
		fdActorSetSpeed(ACTOR_SHIP, 0.8);
		fdActorMoveToAndWait(ACTOR_SHIP, 464, 176);
		syEventCameraFollowActor(ACTOR_SHIP, 1);
		fdActorMoveToAndWait(ACTOR_SHIP, 464, 152);
		fdActorSetSpeed(ACTOR_SHIP, 0.6);
		/* Off toward the horizon: five steps of 16 pixels north, written out as
		 */
		/* the event's other steps are, the ship a sixteenth smaller at each. */
		fdActorShrinkBy(ship, FIXED_ONE / 16);
		fdActorSetDestinationOffset(ACTOR_SHIP, 0, -16);
		scFieldActorWaitForMoveById(ACTOR_SHIP);
		fdActorShrinkBy(ship, FIXED_ONE / 16);
		fdActorSetDestinationOffset(ACTOR_SHIP, 0, -16);
		scFieldActorWaitForMoveById(ACTOR_SHIP);
		fdActorShrinkBy(ship, FIXED_ONE / 16);
		fdActorSetDestinationOffset(ACTOR_SHIP, 0, -16);
		scFieldActorWaitForMoveById(ACTOR_SHIP);
		fdActorShrinkBy(ship, FIXED_ONE / 16);
		fdActorSetDestinationOffset(ACTOR_SHIP, 0, -16);
		scFieldActorWaitForMoveById(ACTOR_SHIP);
		fdActorShrinkBy(ship, FIXED_ONE / 16);
		fdActorSetDestinationOffset(ACTOR_SHIP, 0, -16);
		scFieldActorWaitForMoveById(ACTOR_SHIP);
		fdActorSetSpeed(ACTOR_SHIP, 0.2);
		fdActorSetDestinationOffset(ACTOR_SHIP, 0, -12);
		for(count = 0; count < SHIP_SHRINK_STEPS; count++) {
			fdActorShrinkBy(ship, FIXED_ONE * 9 / 256);
			syWait(7);
		}
		ship->scaleY = 0;
		ship->scaleX = 0;
		syFieldEventWait(10);
		fdEventSetFlag(FLAG_ICE_WALL_SHATTERED);
		syFieldEventSetExitRequest(NORTHERN_REACHES_TO_WORLD_MAP_NORTH);
	}
	fdActorSetPos(ACTOR_KRADEN, 0, 0);
	fdActorSetPos(ACTOR_JENNA, 0, 0);
	fdActorSetPos(ACTOR_SHEBA, 0, 0);
	fdActorSetPos(ACTOR_PIERS, 0, 0);
	fdEventEnd();
}

const TRIGGER *scGetTriggers(void) { return SceneEvents; }

s32 scInit(void) {
	MAIN_ROUTINE_STATE *state;

	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	state = &MainRoutineStateWork;
	fdActorGet(state->leaderActorId)->priFlg |= ACTOR_PRIORITY_IGNORES_RISES;
	if(state->entrance == ENTRANCE_ABOARD_SHIP_WITH_CANNON) {
		fdEventSetFlag(FLAG_SHIP_CARRIES_CANNON);
		/* The player steers the ship, actor 8, in place of the leader. */
		state->ownerOverride = 1;
		fdEventSetDestination(SCENE_NORTHERN_REACHES,
							  NORTHERN_REACHES_FROM_WORLD_MAP_NORTH);
	}
	fdRisingParticlesStart();
	return 0;
}

s32 scPrepareMap(void) {
	if(MainRoutineStateWork.entrance == ENTRANCE_ICE_WALL_GONE)
		scNorthernReachesClearIceWall();
	if(fdEventGetFlag(FLAG_ICE_WALL_SHATTERED))
		scNorthernReachesClearIceWall();
	return 0;
}

/* The Magma Ball's explosion over the ice wall: sparks fly, the screen fades */
/* to white, `callback` draws the wall gone and its shards fly apart from */
/* `position`. The caller passes where the ball struck as `blast`, but nothing
 */
/* reads it: the sparks burst at random over a fixed stretch of the screen, 52
 */
/* to 179 pixels across and 16 to 79 down, wherever the ball landed. */
void scNorthernReachesShatterIceWall(FIXED *blast, FIXED *position,
									 void (*callback)(void)) {
	ICE_WALL_STATE *state;
	ICE_SHARD_OBJ *shard;
	EFFECT_SPR *sprite;
	EFFECT_SPR *flySprite;
	EXPLOSION_SPARK *sparks;
	EXPLOSION_SPARK *spark;
	/* LEFTOVER(frame): an unused local the ROM's frame reserves and never
	 * touches. */
	u8 frameSlot[4];
	void *sheet;
	s32 i, frame, column, entry, sparkEntry, tiles, flying, sparkTiles;
	s32 size;

	state = syHeapSlotAlloc(HEAP_SLOT_MENU, sizeof(ICE_WALL_STATE));
	flying = 0;
	syDecompressTaggedLz(
		syResourceGetTableEntry(RESOURCE_FIELD_EFFECT_ICE_SHARD), state);
	entry = syVramCacheFindFreeEntry();
	tiles = syVramCacheLoad(entry, 1280, state);
	i = 0;
	shard = state->shards;
	sprite = state->sprs;
	for(; i < ICE_SHARD_COUNT; i++) {
		fdEffectSprInit(sprite, 0, 0, OBJ_SIZE_16X16, (i & 3) * 8 + tiles);
		sprite->depth = EFFECT_SPRITE_FRONT_DEPTH;
		sprite->oam.pri = 0;
		sprite->oam.fullCol = 1;
		sprite->oam.pal = 0;
		column = i / 6;
		shard->x = position[0] + FIELD_CELL_ORIGIN(column);
		shard->y = position[1] - FIELD_CELL_ORIGIN(i % 6);
		column -= 4;
		sprite++;
		shard->z = position[2];
		shard->velX = (syRandom() * 3 >> 1) * column;
		shard->velY = (syRandom() << 2) + toFIXED(2);
		shard->velZ = 0;
		shard->age = 0;
		shard++;
	}
	/* FAKEMATCH(temporary): size, the sprite sheet's size held in a local
	 * before the sparks are allocated; as SHEET_BUFFER_SIZE at the allocation.
	 */
	size = SHEET_BUFFER_SIZE;
	sparks = syHeapAllocEwram(EXPLOSION_SPARK_COUNT * sizeof(EXPLOSION_SPARK));
	sheet = syHeapAlloc(size);
	syDecompressTaggedLz(
		syResourceGetTableEntry(RESOURCE_FIELD_EFFECT_EXPLOSION), sheet);
	sparkEntry = syVramCacheFindFreeEntry();
	sparkTiles = syVramCacheLoad(sparkEntry, 1024, sheet);
	syHeapRelease(sheet);
	for(i = 0; i < EXPLOSION_SPARK_COUNT; i++)
		sparks[i].age = -(syRandom() << 5 >> 16);
	syDMAXCopy(Field_ExplosionPalette,
			   (void *)&OBJ_PLTT_COLOR(EXPLOSION_SPARK_PALETTE, 0),
			   DMA_COPY_HALFWORDS(PALETTE_COLORS), REG_DMA3);
	for(frame = 0; frame <= 149; frame++) {
		if((frame & 31) == 0) {
			fdEventPlaySnd(SE_CRASH);
		}
		if(frame == 60) {
			fdPaletteFadeSetTarget(RGB_WHITE, PALETTES_ALL);
			fdPaletteFadeStartInterpolation(90);
			fdEventPlaySnd(SE_RUMBLE);
		}
		if(frame == 105) {
			callback();
			flying = 1;
			fdMapGridSetShake(6, 6, 1);
			fdEventPlaySnd(SE_SPARKLING_PASSAGE);
		}
		if(frame == 130) {
			flying = 1;
		}
		if(flying != 0) {
			shard = state->shards;
			flySprite = state->sprs;
			for(i = ICE_SHARD_COUNT - 1; i >= 0; i--) {
				if(shard->age < ICE_SHARD_FRAMES) {
					fdEffectSprDrawAtWorldPos(flySprite, &shard->x);
					fdBattleEffectStepVector3(&shard->x, 63, -toFIXED(3) / 16);
					shard->age++;
				}
				flySprite++;
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
				syOamAddSprAtDepthClamped(spark, EXPLOSION_SPARK_DEPTH);
			}
			spark->age++;
			if(spark->age >= EXPLOSION_SPARK_FRAMES) {
				spark->age = -(syRandomBelow(7));
			}
			spark++;
		} while((s32)spark <= (s32)&sparks[EXPLOSION_SPARK_COUNT - 1]);
		sySynch(1);
	}
	syVramCacheFreeEntry(sparkEntry);
	syHeapRelease(sparks);
	syFieldEventWait(40);
	fdMapGridSetShake(0, 0, 0);
	syVramCacheFreeEntry(entry);
	syHeapSlotRelease(HEAP_SLOT_MENU);
}

/* The ship at rest on the sea, without its shadow, on the middle layer. */
s32 NorthernReaches_ShipAtRest[] = {
	SC_CALL((s32)syShipDropShadow),
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

u32 SceneExits[] = {
	SCENE_EXITS(NorthernReaches),
	SCENE_EXIT_UNLESS(NORTHERN_REACHES_TO_WORLD_MAP_SOUTH, WorldMap,
					  WORLD_MAP_FROM_NORTHERN_REACHES_SOUTH, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(NORTHERN_REACHES_TO_WORLD_MAP_NORTH, WorldMap,
					  WORLD_MAP_FROM_NORTHERN_REACHES_NORTH, EXIT_FLAG_NONE),
	SCENE_EXITS_END,
};

ACTOR_PLACE ActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS,
					(s32)NorthernReaches_ShipAtRest, 120, 0, 280, FACING_SOUTH,
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

TRIGGER SceneEvents[] = {
	{TRIGGER_EXIT, NORTHERN_REACHES_TO_WORLD_MAP_SOUTH, CONDITION_ALWAYS,
	 NORTHERN_REACHES_TO_WORLD_MAP_SOUTH},
	{TRIGGER_EXIT, NORTHERN_REACHES_TO_WORLD_MAP_NORTH, CONDITION_ALWAYS,
	 NORTHERN_REACHES_TO_WORLD_MAP_NORTH},
	{TRIGGER_STEP, CELL_SOUTH_OF_ICE_WALL,
	 IF_FLAG_SET(FLAG_SHIP_CARRIES_CANNON),
	 EVENT(scNorthernReachesDiscussIceWall)},
	{TRIGGER_STEP, CELL_BETWEEN_ICE_BLOCKS,
	 IF_FLAG_SET(FLAG_SHIP_CARRIES_CANNON),
	 EVENT(scNorthernReachesFireMagmaBall)},
	{TRIGGER_STEP, CELL_UNDER_ICE_WALL, FLAG_SHIP_CARRIES_CANNON,
	 EVENT(scNorthernReachesSightIceWall)},
	{TRIGGER_TABLE_END},
};

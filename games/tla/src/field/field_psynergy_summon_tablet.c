/* A summon tablet read: its pictures cycling, its pebbles and runes flying, and
   placing the flag blocks. Part of field_psynergy.c. */

#include "field/actor_script.h"

/* The summon tablet's update while its runes fly: on the actor's timer, its
   sprite flashes white and outlined, then outlined, white again, and plain,
   redrawn each tick; the fourth step leaves the outline as it was. */
void fdSummonTabletCyclePictures(ACTOR *actor)
{
	SPR			  *sprite = actor->sprite;
	SPR_ANIMATION *layer = sprite->entries[0];

	switch(actor->timer & 3) {
	case 0:
		layer->colorMode = SPRITE_COLOR_WHITE;
		sprite->dirty = 1;
		sprite->flags = SPRITE_FLAG_OUTLINED;
		break;
	case 1:
		sprite->dirty = 1;
		sprite->flags = 0;
		break;
	case 2:
		layer->colorMode = SPRITE_COLOR_WHITE;
		sprite->dirty = 1;
		sprite->flags = 0;
		break;
	case 3:
		layer->colorMode = SPRITE_COLOR_NORMAL;
		sprite->dirty = 1;
		break;
	}
	actor->timer++;
}

/* A point on the screen chasing its target, 16.16 pixels, as
   fdCameraWorldToScreen gives one: z down the screen. */
typedef struct CHASE {
	FIXED x;
	FIXED z;
	FIXED unusedAfterZ; /* no code names it */
	FIXED targetX;
	FIXED targetZ;
} CHASE;

/* The sign bit of a 32-bit number: two numbers' signs differ where their
   exclusive or has it. */
#define SIGN_BIT 0x80000000

/* Moves toward the target by speed, stopping on it on overshoot. */
void fdMotionStepTowardTarget(CHASE *chase, FIXED speed)
{
	Sint32 dx = chase->targetX - chase->x;
	Sint32 dz = chase->targetZ - chase->z;
	Uint16 angle = syAtan(dz, dx);

	chase->x += syMulFX(syCos(angle), speed);
	chase->z += syMulFX(sySin(angle), speed);
	{
		Sint32 nextDx = chase->targetX - chase->x;
		Sint32 nextDz = chase->targetZ - chase->z;

		if((nextDx ^ dx) & SIGN_BIT)
			chase->x = chase->targetX;
		if((dz ^ nextDz) & SIGN_BIT)
			chase->z = chase->targetZ;
	}
}

/* One rune or pebble and a tick count, negative before it starts. */
typedef struct TABLET_PIECE {
	union {
		/* A rune: a point on the screen chasing its target. */
		CHASE rune;

		/* A pebble: a position in the world and its velocity
		   (fdAdvanceWithGravity3D's vector). */
		struct {
			FIXED pos[3];
			FIXED velocity[3];
		} pebble;
	};

	Sint32 timer;
} TABLET_PIECE;

/* The pebbles that fly off the tablet, a power of two: the next wraps by a
   mask. */
#define TABLET_PEBBLE_COUNT		 32
/* The pebbles thrown off at once by the last, heavy blow (SE_HEAVY_IMPACT). */
#define TABLET_PEBBLES_ON_IMPACT 25
/* A pebble's four pictures (RESOURCE_PEBBLES, 16 by 16 and shrinking), five
   ticks each; a picture of 256 colours is 8 tile numbers. */
#define PEBBLE_PICTURES			 4
#define PEBBLE_PICTURE_TICKS	 5
#define PEBBLE_PICTURE_TILES	 8
/* The tablet's runes, in three rows of 16. */
#define TABLET_RUNE_COUNT		 48
#define TABLET_RUNE_ROW			 16
/* RESOURCE_RUNES' 16 grey runes, 8 by 8, and after them the same runes lit in
   blue and green; an 8 by 8 tile of 256 colours is two tile numbers. */
#define TABLET_RUNE_PICTURES	 16
#define RUNE_LIT				 (TABLET_RUNE_PICTURES * 2)

/* What the runes do (SUMMON_TABLET_STATE.state). */
typedef enum RUNE_MOTION {
	RUNES_LINE_UP = 0, /* each flies from the tablet to its place in the rows */
	RUNES_CIRCLE = 2,  /* each chases the one before round the circle */
	RUNES_GATHER = 3   /* each flies to the leader and is drawn till it lands */
} RUNE_MOTION;

/* The summon tablet's state (heap slot battleEffect): the decoded pictures, the
   pebbles and runes that fly between the tablet and the leader, and the circle
   the runes gather on. */
typedef struct SUMMON_TABLET_STATE {
	/* The runes' pictures, and then the pebbles', decoded to be loaded into
	   VRAM. */
	Uint8		 pictures[2048];
	TABLET_PIECE pebbles[TABLET_PEBBLE_COUNT];
	EFFECT_SPR	 pebbleSprites[TABLET_PEBBLE_COUNT];
	TABLET_PIECE runes[TABLET_RUNE_COUNT];
	EFFECT_SPR	 runeSprites[TABLET_RUNE_COUNT];
	Sint32		 originX;
	Sint32		 originZ;
	Sint32		 angle;
	Sint32		 radius;
	Sint32		 spacing;
	Sint8		 state; /* RUNE_MOTION */
	Sint8		 moving;
	Uint16		 pebbleTile;
	Sint32		 targetX;
	Sint32		 targetZ;
} SUMMON_TABLET_STATE;

/* Event of fdSummonTabletPlayRunes: moves and draws the runes and pebbles. */
void fdSummonTabletUpdateRunes(void)
{
	SUMMON_TABLET_STATE *state = RamHeapSlots->battleEffect;
	Sint32				 pos[3];
	TABLET_PIECE		*piece;
	EFFECT_SPR			*sprite;
	Sint32				 i;
	Uint16				*tile;

	if(state->moving != 0) {
		pos[0] = state->originX;
		pos[2] = state->originZ;
		syVectorAddPolarOffset(state->radius, state->angle, pos);
		piece = state->runes;
		piece->rune.targetX = pos[0];
		piece->rune.targetZ = pos[2];
		state->angle += TRIG_TURN * 3 / 128;
		for(i = 0; i < TABLET_RUNE_COUNT; i++, piece++) {
			if(piece->timer >= 0) {
				if(i != 0) {
					piece->rune.targetX = pos[0];
					piece->rune.targetZ = pos[2];
				}
				pos[0] = piece->rune.x;
				pos[2] = piece->rune.z;
				syVectorAddPolarOffset(state->spacing, 0, pos);
			}
			piece->timer++;
		}
	}

	piece = state->runes;
	sprite = state->runeSprites;
	for(i = 0; i < TABLET_RUNE_COUNT; i++, sprite++, piece++) {
		if(state->state == RUNES_LINE_UP) {
			if(piece->timer >= 0) {
				fdMotionStepTowardTarget(&piece->rune, toFIXED(4));
				sprite->x = piece->rune.x;
				sprite->y = piece->rune.z;
				fdEffectSpriteDraw(sprite);
			}
			piece->timer++;
		} else if(state->state == RUNES_CIRCLE) {
			fdMotionStepTowardTarget(&piece->rune, toFIXED(6));
			sprite->x = piece->rune.x;
			sprite->y = piece->rune.z;
			fdEffectSpriteDraw(sprite);
		} else if(state->state == RUNES_GATHER) {
			if(piece->rune.x != state->targetX ||
				piece->rune.z != state->targetZ) {
				fdMotionStepTowardTarget(&piece->rune, toFIXED(6));
				sprite->x = piece->rune.x;
				sprite->y = piece->rune.z;
				fdEffectSpriteDraw(sprite);
			}
		}
	}

	piece = state->pebbles;
	tile = &state->pebbleTile;
	sprite = state->pebbleSprites;
	for(i = 0; i < TABLET_PEBBLE_COUNT; i++, sprite++, piece++) {
		if((Uint32)piece->timer < PEBBLE_PICTURES * PEBBLE_PICTURE_TICKS) {
			sprite->oam.index = *tile +
				(piece->timer / PEBBLE_PICTURE_TICKS & 7) *
					PEBBLE_PICTURE_TILES;
			fdEffectSpriteDrawAtWorldPos(sprite, piece->pebble.pos);
			fdAdvanceWithGravity3D(
				piece->pebble.pos, DAMPING_SCALE - 1, -(toFIXED(1) / 2));
			piece->timer++;
		}
	}
}

/* The stages of a summon tablet's reading. */
typedef enum TABLET_STAGE {
	TABLET_STRUCK = 0,		/* pebbles fly off it, then it vanishes */
	TABLET_RUNES_LIGHT = 1, /* the runes, lined up, light up in a wave */
	TABLET_LEADER_TURNS = 2,
	TABLET_ARMS_RAISED = 3,
	TABLET_RUNES_CIRCLE = 4,	/* the runes stream round in a circle */
	TABLET_RUNES_TO_LEADER = 5, /* the circle moves over the leader */
	TABLET_RUNES_CLOSE_IN = 6,	/* and shrinks to nothing on it */
	TABLET_LEADER_SQUASHED = 7
} TABLET_STAGE;

extern const Sint32 FieldPsynergyCastColors[];
extern const Sint32 SummonTabletSquashScript[];
extern const Uint8	MovementTypeSquashScript[];

/* Plays a summon tablet's reading (a CONTAINER_STONE_TABLET treasure, "examined
   the stone tablet"): pebbles fly off the tablet as it is struck, and its runes
   light up and fly round the leader, with their sounds, before
   mnSummonShowLearnedMessage. */
void fdSummonTabletPlayRunes(Sint32 targetId)
{
	SUMMON_TABLET_STATE *summonTabletState = syHeapSlotAllocate(
		HEAP_SLOT_BATTLE_EFFECT, sizeof(SUMMON_TABLET_STATE));
	Sint32		  ownerId = fdPartyGetLeaderActor();
	ACTOR		 *owner = fdActorLookup(ownerId);
	ACTOR		 *target = fdActorLookup(targetId);
	Sint32		  state;
	Sint32		  runeEntry;
	Sint32		  pebbleEntry;
	FIXED		  ownerX;
	FIXED		  ownerZ;
	FIXED		  targetX;
	FIXED		  targetZ;
	Sint32		  newPebbles = 0;
	Sint32		  nextPebble = 0;
	FIXED		  pos[3];
	EFFECT_SPR	 *sprite;
	TABLET_PIECE *piece;
	Sint32		  runeTile;
	Sint32		  pebbleTile;
	Sint32		  timer;
	Sint32		  runeFrame;
	Sint32		  delay;
	Sint32		  angle;
	Sint32		  i;
	FIXED		  startX = toFIXED(130);
	FIXED		  startZ = toFIXED(30);

	fdStartColorTransition(8, FieldPsynergyCastColors[TickCount & 7]);
	pos[0] = owner->x.fixed;
	pos[1] = owner->y.fixed + toFIXED(24);
	pos[2] = owner->z.fixed;
	fdCameraWorldToScreen(pos);
	ownerX = pos[0];
	ownerZ = pos[2];
	summonTabletState->targetX = ownerX;
	summonTabletState->targetZ = ownerZ;
	pos[0] = target->x.fixed;
	pos[1] = target->y.fixed;
	pos[2] = target->z.fixed;
	fdCameraWorldToScreen(pos);
	targetX = pos[0];
	targetZ = pos[2];
	syDecompressTaggedLZ(
		syResourceGetTableEntry(RESOURCE_RUNES), summonTabletState->pictures);
	runeEntry = syVRAMCacheFindFreeEntry();
	runeTile = syVRAMCacheLoad(runeEntry, sizeof(summonTabletState->pictures),
		summonTabletState->pictures);
	piece = summonTabletState->runes;
	i = 0;
	sprite = summonTabletState->runeSprites;
	for(; i < TABLET_RUNE_COUNT; i++) {
		fdEffectSpriteInit(sprite, 4, 4, 0,
			runeTile + (syRandom() * TABLET_RUNE_PICTURES >> 16) * 2);
		sprite->oam.fullColor = 1;
		sprite->oam.palette = 0;
		sprite->oam.priority = 1;
		sprite->depth = OAM_DEPTH_WINDOW;
		piece->rune.x = targetX;
		piece->rune.z = targetZ;
		/* The runes line up on the screen in rows 12 pixels apart, the rows 24
		   pixels apart, from (30, 30). */
		piece->rune.targetX = (i % TABLET_RUNE_ROW) * toFIXED(12) + toFIXED(30);
		piece->rune.targetZ = (i / TABLET_RUNE_ROW) * toFIXED(24) + toFIXED(30);
		piece->timer = -i;
		sprite++;
		piece++;
	}
	syDecompressTaggedLZ(
		syResourceGetTableEntry(RESOURCE_PEBBLES), summonTabletState->pictures);
	pebbleEntry = syVRAMCacheFindFreeEntry();
	pebbleTile = syVRAMCacheLoad(pebbleEntry,
		sizeof(summonTabletState->pictures), summonTabletState->pictures);
	summonTabletState->pebbleTile = pebbleTile;
	piece = summonTabletState->pebbles;
	sprite = summonTabletState->pebbleSprites;
	for(i = 0; i < TABLET_PEBBLE_COUNT; i++) {
		fdEffectSpriteInit(sprite, 8, 8, OBJ_SIZE_16X16, pebbleTile);
		sprite->oam.fullColor = 1;
		sprite->oam.palette = 0;
		sprite->oam.priority = 0;
		sprite->depth = OAM_DEPTH_WINDOW;
		piece->timer = -1;
		sprite++;
		piece++;
	}
	sySynch(1);
	sySoundPlayStub(SE_DJINNI_SET);
	fdActorSelectAnimationStub(target, 2);
	fdScriptWait(60);
	target->timer = 0;
	target->update = fdSummonTabletCyclePictures;
	sySoundPlayStub(SE_SWIRL);
	fdScriptWait(80);
	fdActorStartScriptStub(target, SummonTabletSquashScript);
	summonTabletState->moving = 0;
	sySetEvent(fdSummonTabletUpdateRunes, EVENT_PASS_TICK);
	state = TABLET_STRUCK;
	timer = 0;
	do {
		switch(state) {
		case TABLET_STRUCK:
			summonTabletState->state = RUNES_LINE_UP;
			if(timer <= 39) {
				if((timer & 7) == 0)
					sySoundPlayStub(SE_IMPACT);
				newPebbles = 1;
			}
			if(timer == 40) {
				sySoundPlayStub(SE_HEAVY_IMPACT);
				newPebbles = TABLET_PEBBLES_ON_IMPACT;
				fdActorStartIdleStub(target);
				target->scaleX = 0;
			}
			if(timer == 80) {
				state++;
				timer = -1;
			}
			break;
		case TABLET_RUNES_LIGHT:
			/* Each rune lights up three times, a tick later for each place
			   across or down the rows. */
			for(i = 0; i < TABLET_RUNE_COUNT; i++) {
				EFFECT_SPR *rune;
				runeFrame = i % TABLET_RUNE_ROW + i / TABLET_RUNE_ROW;
				runeFrame = timer - runeFrame;
				rune = &summonTabletState->runeSprites[i];
				if(runeFrame == 0)
					rune->oam.index += RUNE_LIT;
				if(runeFrame == 3)
					rune->oam.index += -RUNE_LIT;
				if(runeFrame == 8)
					rune->oam.index += RUNE_LIT;
				if(runeFrame == 10)
					rune->oam.index += -RUNE_LIT;
				if(runeFrame == 16)
					rune->oam.index += RUNE_LIT;
				if(runeFrame == 18)
					rune->oam.index += -RUNE_LIT;
			}
			if(timer == 0)
				sySoundPlayStub(SE_GLITTER);
			if(timer == 56) {
				state++;
				timer = -1;
			}
			break;
		case TABLET_LEADER_TURNS:
			if(timer == 0) {
				fdActorFaceDirectionById(ownerId, FACING_SOUTH, 0);
				break;
			}
			if(owner->script[owner->scriptCursor] == SCRIPT_COMMAND_END) {
				state++;
				timer = -1;
			}
			if(timer == 60) {
				state++;
				timer = -1;
			}
			break;
		case TABLET_ARMS_RAISED:
			if(timer == 0)
				fdActorSelectAnimationStub(owner, ANIM_LEADER_ARMS_RAISED);
			if(timer == 10) {
				state++;
				timer = -1;
			}
			break;
		case TABLET_RUNES_CIRCLE:
			if(timer == 0) {
				piece = summonTabletState->runes;
				delay = 0;
				for(i = 0; i < TABLET_RUNE_COUNT; i++) {
					piece->timer = delay;
					delay -= 3;
					piece++;
				}
				summonTabletState->radius = toFIXED(60);
				summonTabletState->angle = TRIG_TURN * 3 / 4;
				summonTabletState->originX = startX;
				summonTabletState->originZ = startZ;
				summonTabletState->moving = 1;
				summonTabletState->spacing = toFIXED(4);
				summonTabletState->state = RUNES_CIRCLE;
			}
			if(timer == 0)
				sySoundPlayStub(SE_FALLING_LIGHT);
			if(timer == 48)
				sySoundPlayStub(SE_FALLING_LIGHT);
			if(timer == 96)
				sySoundPlayStub(SE_FALLING_LIGHT);
			if(timer == 100) {
				state++;
				timer = -1;
			}
			break;
		case TABLET_RUNES_TO_LEADER:
			summonTabletState->originX =
				timer * (ownerX - startX) / 30 + startX;
			summonTabletState->originZ =
				timer * (ownerZ - startZ) / 30 + startZ;
			if(timer == 30) {
				state++;
				timer = -1;
			}
			break;
		case TABLET_RUNES_CLOSE_IN:
			if(timer == 0)
				summonTabletState->spacing = 0;
			summonTabletState->radius += -toFIXED(2);
			if(summonTabletState->radius < 0) {
				summonTabletState->radius = 0;
				state++;
				timer = -1;
			}
			break;
		case TABLET_LEADER_SQUASHED:
			if(timer == 0)
				fdActorStartScriptStub(owner, MovementTypeSquashScript);
			if((timer & 3) == 0)
				sySoundPlayStub(SE_MAP_TRANSITION);
			summonTabletState->state = RUNES_GATHER;
			if(timer == 60)
				state = FIELD_PSYNERGY_FINISHED;
			break;
		}
		for(i = 0; i < newPebbles; i++) {
			angle = syRandom();
			piece = &summonTabletState->pebbles[nextPebble];
			piece->timer = 0;
			piece->pebble.pos[0] = target->x.fixed;
			piece->pebble.pos[1] = target->y.fixed + toFIXED(12);
			piece->pebble.pos[2] = target->z.fixed;
			syVectorAddPolarOffset(
				syRandom() * 4 + toFIXED(2), angle, piece->pebble.pos);
			piece->pebble.velocity[0] = 0;
			piece->pebble.velocity[1] = toFIXED(9) / 2;
			piece->pebble.velocity[2] = 0;
			syVectorAddPolarOffset(syRandom() * 2 + toFIXED(2), angle,
				summonTabletState->pebbles[nextPebble].pebble.velocity);
			nextPebble = (nextPebble + 1) & (TABLET_PEBBLE_COUNT - 1);
		}
		newPebbles = 0;
		sySynch(1);
		timer++;
	} while(state != FIELD_PSYNERGY_FINISHED);
	fdActorStartIdleStub(owner);
	owner->scaleX = toFIXED(1);
	owner->scaleY = toFIXED(1);
	fdScriptWait(10);
	syCloseEvent(fdSummonTabletUpdateRunes);
	fdPsynergyFadeFromCastColor();
	syVRAMCacheFreeEntry(runeEntry);
	syVRAMCacheFreeEntry(pebbleEntry);
	syHeapSlotRelease(HEAP_SLOT_BATTLE_EFFECT);
}

/* A flag block: a puddle's actor and the flag that says it is frozen, in a list
   ended by ACTOR_LIST_END (fdFlagBlockPlaceAll). */
typedef struct ACTOR_FLAG {
	Uint16 actor;
	Sint16 flags;
} ACTOR_FLAG;

/* Sets up a scene's flag blocks, the puddles Frost turns into pillars of ice
   (actor and flag pairs, ended by ACTOR_LIST_END): each stops being solid and
   blocks its metatile; one whose flag is set shows the ice pillar and has the
   flag cleared, the others the puddle. Then fdFlagBlockApplyFlags raises the
   frozen ones and sets their flags again, and the leader is lifted onto the
   ground if it now lies below it. A scene with such puddles calls it as it
   starts. */
void fdFlagBlockPlaceAll(ACTOR_FLAG *actors)
{
	MAP_STATE  *map = RamHeapSlots->mapGrid;
	ACTOR_FLAG *list = actors;
	ACTOR	   *leader;
	Sint32		height;

	for(; actors->actor != ACTOR_LIST_END; actors++) {
		ACTOR		 *actor = fdActorGetById(actors->actor);
		Uint16		  flag = actors->flags;
		Sint32		  slot = fdActorGetMetatileIndex(actor);
		MAP_METATILE *metatile = &map->layers[actor->layer].metatiles[slot];

		actor->motionFlags = 0;
		actor->collisionFlags &= ~ACTOR_SOLID;
		fdActorSetSpriteFlagsStub(actor, 0);
		metatile->collisionCode = METATILE_BEHAVIOR_WALL;
		if(syGameFlagGet(flag)) {
			fdActorSelectAnimationStub(actor, PUDDLE_ANIMATION_FROZEN);
			syGameFlagClear(flag);
		} else {
			fdActorSelectAnimationStub(actor, PUDDLE_ANIMATION_PUDDLE);
		}
	}
	fdFlagBlockApplyFlags(list);
	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	height = fdMapGridGetTerrainHeightStub(
		leader->layer, leader->x.fixed, leader->z.fixed);
	if(leader->y.fixed < height)
		leader->y.fixed = leader->groundHeight = height;
}

/* Marks the metatile at (x, z) MAP_METATILE_JUMP, a gap the leader jumps
   across, where no ground lies above height base. */
void fdMapGridMarkJumpIfOpen(FIXED x, FIXED z, Sint32 layer, FIXED base)
{
	if(fdMapGridGetTerrainHeightAboveStub(x, z, layer, base) == 0)
		fdMapGridSetMetatileAttributesAt(x, z, layer,
			fdMapGridGetMetatileAttributesAt(x, z, layer) | MAP_METATILE_JUMP);
}

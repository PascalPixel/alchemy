/* A summon tablet read: its pictures cycling, its pebbles and runes flying, and
   placing the flag blocks. Part of field_psynergy.c. */

#include "field/actor_script.h"

/* The summon tablet's update while its runes fly: on the actor's timer, its
   sprite flashes white and outlined, then outlined, white again, and plain,
   redrawn each tick; the fourth step leaves the outline as it was. */
void fdSummonTabletCyclePictures(ACTOR *actor)
{
	SPR			  *spr = actor->spr;
	SPR_ANIMATION *layer = spr->entries[0];

	switch(actor->tmr & 3) {
	case 0:
		layer->colMode = SPRITE_COLOR_WHITE;
		spr->dirty = 1;
		spr->flg = SPRITE_FLAG_OUTLINED;
		break;
	case 1:
		spr->dirty = 1;
		spr->flg = 0;
		break;
	case 2:
		layer->colMode = SPRITE_COLOR_WHITE;
		spr->dirty = 1;
		spr->flg = 0;
		break;
	case 3:
		layer->colMode = SPRITE_COLOR_NORMAL;
		spr->dirty = 1;
		break;
	}
	actor->tmr++;
}

/* A point on the screen chasing its target, 16.16 pixels, as
   fdCameraWorldToScreen gives one: z down the screen. */
typedef struct CHASE {
	FIXED x;
	FIXED z;
	FIXED unusedAfterZ; /* no code names it */
	FIXED tgtX;
	FIXED tgtZ;
} CHASE;

/* The sign bit of a 32-bit number: two numbers' signs differ where their
   exclusive or has it. */
#define SIGN_BIT 0x80000000

/* Moves toward the target by speed, stopping on it on overshoot. */
void fdMotionStepTowardTarget(CHASE *chase, FIXED speed)
{
	Sint32 dx = chase->tgtX - chase->x;
	Sint32 dz = chase->tgtZ - chase->z;
	Uint16 angle = syAtan(dz, dx);

	chase->x += syMulFX(syCos(angle), speed);
	chase->z += syMulFX(sySin(angle), speed);
	{
		Sint32 nextDx = chase->tgtX - chase->x;
		Sint32 nextDz = chase->tgtZ - chase->z;

		if((nextDx ^ dx) & SIGN_BIT)
			chase->x = chase->tgtX;
		if((dz ^ nextDz) & SIGN_BIT)
			chase->z = chase->tgtZ;
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
			FIXED vel[3];
		} pebble;
	};

	Sint32 tmr;
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
	EFFECT_SPR	 pebbleSprs[TABLET_PEBBLE_COUNT];
	TABLET_PIECE runes[TABLET_RUNE_COUNT];
	EFFECT_SPR	 runeSprs[TABLET_RUNE_COUNT];
	Sint32		 originX;
	Sint32		 originZ;
	Sint32		 ang;
	Sint32		 radius;
	Sint32		 spacing;
	Sint8		 state; /* RUNE_MOTION */
	Sint8		 moving;
	Uint16		 pebbleTile;
	Sint32		 tgtX;
	Sint32		 tgtZ;
} SUMMON_TABLET_STATE;

/* Event of fdSummonTabletPlayRunes: moves and draws the runes and pebbles. */
void fdSummonTabletUpdateRunes(void)
{
	SUMMON_TABLET_STATE *state = Ram_HeapSlots->battleEff;
	Sint32				 pos[3];
	TABLET_PIECE		*piece;
	EFFECT_SPR			*spr;
	Sint32				 i;
	Uint16				*tile;

	if(state->moving != 0) {
		pos[0] = state->originX;
		pos[2] = state->originZ;
		syVectorAddPolarOffset(state->radius, state->ang, pos);
		piece = state->runes;
		piece->rune.tgtX = pos[0];
		piece->rune.tgtZ = pos[2];
		state->ang += TRIG_TURN * 3 / 128;
		for(i = 0; i < TABLET_RUNE_COUNT; i++, piece++) {
			if(piece->tmr >= 0) {
				if(i != 0) {
					piece->rune.tgtX = pos[0];
					piece->rune.tgtZ = pos[2];
				}
				pos[0] = piece->rune.x;
				pos[2] = piece->rune.z;
				syVectorAddPolarOffset(state->spacing, 0, pos);
			}
			piece->tmr++;
		}
	}

	piece = state->runes;
	spr = state->runeSprs;
	for(i = 0; i < TABLET_RUNE_COUNT; i++, spr++, piece++) {
		if(state->state == RUNES_LINE_UP) {
			if(piece->tmr >= 0) {
				fdMotionStepTowardTarget(&piece->rune, toFIXED(4));
				spr->x = piece->rune.x;
				spr->y = piece->rune.z;
				fdEffectSprDraw(spr);
			}
			piece->tmr++;
		} else if(state->state == RUNES_CIRCLE) {
			fdMotionStepTowardTarget(&piece->rune, toFIXED(6));
			spr->x = piece->rune.x;
			spr->y = piece->rune.z;
			fdEffectSprDraw(spr);
		} else if(state->state == RUNES_GATHER) {
			if(piece->rune.x != state->tgtX || piece->rune.z != state->tgtZ) {
				fdMotionStepTowardTarget(&piece->rune, toFIXED(6));
				spr->x = piece->rune.x;
				spr->y = piece->rune.z;
				fdEffectSprDraw(spr);
			}
		}
	}

	piece = state->pebbles;
	tile = &state->pebbleTile;
	spr = state->pebbleSprs;
	for(i = 0; i < TABLET_PEBBLE_COUNT; i++, spr++, piece++) {
		if((Uint32)piece->tmr < PEBBLE_PICTURES * PEBBLE_PICTURE_TICKS) {
			spr->oam.idx = *tile +
				(piece->tmr / PEBBLE_PICTURE_TICKS & 7) * PEBBLE_PICTURE_TILES;
			fdEffectSprDrawAtWorldPos(spr, piece->pebble.pos);
			fdAdvanceWithGravity3D(
				piece->pebble.pos, DAMPING_SCALE - 1, -(toFIXED(1) / 2));
			piece->tmr++;
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
void fdSummonTabletPlayRunes(Sint32 tgtId)
{
	SUMMON_TABLET_STATE *summonTabletState =
		syHeapSlotAlloc(HEAP_SLOT_BATTLE_EFFECT, sizeof(SUMMON_TABLET_STATE));
	Sint32		  ownerId = fdPartyGetLeaderActor();
	ACTOR		 *owner = fdActorLookup(ownerId);
	ACTOR		 *tgt = fdActorLookup(tgtId);
	Sint32		  state;
	Sint32		  runeEntry;
	Sint32		  pebbleEntry;
	FIXED		  ownerX;
	FIXED		  ownerZ;
	FIXED		  tgtX;
	FIXED		  tgtZ;
	Sint32		  newPebbles = 0;
	Sint32		  nextPebble = 0;
	FIXED		  pos[3];
	EFFECT_SPR	 *spr;
	TABLET_PIECE *piece;
	Sint32		  runeTile;
	Sint32		  pebbleTile;
	Sint32		  tmr;
	Sint32		  runeFrm;
	Sint32		  delay;
	Sint32		  ang;
	Sint32		  i;
	FIXED		  startX = toFIXED(130);
	FIXED		  startZ = toFIXED(30);

	fdStartColTransition(8, FieldPsynergyCastColors[TickCount & 7]);
	pos[0] = owner->x.fixed;
	pos[1] = owner->y.fixed + toFIXED(24);
	pos[2] = owner->z.fixed;
	fdCameraWorldToScreen(pos);
	ownerX = pos[0];
	ownerZ = pos[2];
	summonTabletState->tgtX = ownerX;
	summonTabletState->tgtZ = ownerZ;
	pos[0] = tgt->x.fixed;
	pos[1] = tgt->y.fixed;
	pos[2] = tgt->z.fixed;
	fdCameraWorldToScreen(pos);
	tgtX = pos[0];
	tgtZ = pos[2];
	syDecompressTaggedLZ(
		syResourceGetTableEntry(RESOURCE_RUNES), summonTabletState->pictures);
	runeEntry = syVRAMCacheFindFreeEntry();
	runeTile = syVRAMCacheLoad(runeEntry, sizeof(summonTabletState->pictures),
		summonTabletState->pictures);
	piece = summonTabletState->runes;
	i = 0;
	spr = summonTabletState->runeSprs;
	for(; i < TABLET_RUNE_COUNT; i++) {
		fdEffectSprInit(spr, 4, 4, 0,
			runeTile + (syRandom() * TABLET_RUNE_PICTURES >> 16) * 2);
		spr->oam.fullCol = 1;
		spr->oam.pal = 0;
		spr->oam.pri = 1;
		spr->depth = OAM_DEPTH_WINDOW;
		piece->rune.x = tgtX;
		piece->rune.z = tgtZ;
		/* The runes line up on the screen in rows 12 pixels apart, the rows 24
		   pixels apart, from (30, 30). */
		piece->rune.tgtX = (i % TABLET_RUNE_ROW) * toFIXED(12) + toFIXED(30);
		piece->rune.tgtZ = (i / TABLET_RUNE_ROW) * toFIXED(24) + toFIXED(30);
		piece->tmr = -i;
		spr++;
		piece++;
	}
	syDecompressTaggedLZ(
		syResourceGetTableEntry(RESOURCE_PEBBLES), summonTabletState->pictures);
	pebbleEntry = syVRAMCacheFindFreeEntry();
	pebbleTile = syVRAMCacheLoad(pebbleEntry,
		sizeof(summonTabletState->pictures), summonTabletState->pictures);
	summonTabletState->pebbleTile = pebbleTile;
	piece = summonTabletState->pebbles;
	spr = summonTabletState->pebbleSprs;
	for(i = 0; i < TABLET_PEBBLE_COUNT; i++) {
		fdEffectSprInit(spr, 8, 8, OBJ_SIZE_16X16, pebbleTile);
		spr->oam.fullCol = 1;
		spr->oam.pal = 0;
		spr->oam.pri = 0;
		spr->depth = OAM_DEPTH_WINDOW;
		piece->tmr = -1;
		spr++;
		piece++;
	}
	sySynch(1);
	sySndPlayStub(SE_DJINNI_SET);
	fdActorSelectAnimationStub(tgt, 2);
	fdScriptWait(60);
	tgt->tmr = 0;
	tgt->update = fdSummonTabletCyclePictures;
	sySndPlayStub(SE_SWIRL);
	fdScriptWait(80);
	fdActorStartScriptStub(tgt, SummonTabletSquashScript);
	summonTabletState->moving = 0;
	sySetEvent(fdSummonTabletUpdateRunes, EVENT_PASS_TICK);
	state = TABLET_STRUCK;
	tmr = 0;
	do {
		switch(state) {
		case TABLET_STRUCK:
			summonTabletState->state = RUNES_LINE_UP;
			if(tmr <= 39) {
				if((tmr & 7) == 0)
					sySndPlayStub(SE_IMPACT);
				newPebbles = 1;
			}
			if(tmr == 40) {
				sySndPlayStub(SE_HEAVY_IMPACT);
				newPebbles = TABLET_PEBBLES_ON_IMPACT;
				fdActorStartIdleStub(tgt);
				tgt->scaleX = 0;
			}
			if(tmr == 80) {
				state++;
				tmr = -1;
			}
			break;
		case TABLET_RUNES_LIGHT:
			/* Each rune lights up three times, a tick later for each place
			   across or down the rows. */
			for(i = 0; i < TABLET_RUNE_COUNT; i++) {
				EFFECT_SPR *rune;
				runeFrm = i % TABLET_RUNE_ROW + i / TABLET_RUNE_ROW;
				runeFrm = tmr - runeFrm;
				rune = &summonTabletState->runeSprs[i];
				if(runeFrm == 0)
					rune->oam.idx += RUNE_LIT;
				if(runeFrm == 3)
					rune->oam.idx += -RUNE_LIT;
				if(runeFrm == 8)
					rune->oam.idx += RUNE_LIT;
				if(runeFrm == 10)
					rune->oam.idx += -RUNE_LIT;
				if(runeFrm == 16)
					rune->oam.idx += RUNE_LIT;
				if(runeFrm == 18)
					rune->oam.idx += -RUNE_LIT;
			}
			if(tmr == 0)
				sySndPlayStub(SE_GLITTER);
			if(tmr == 56) {
				state++;
				tmr = -1;
			}
			break;
		case TABLET_LEADER_TURNS:
			if(tmr == 0) {
				fdActorFaceDirectionById(ownerId, FACING_SOUTH, 0);
				break;
			}
			if(owner->script[owner->scriptCursor] == SCRIPT_COMMAND_END) {
				state++;
				tmr = -1;
			}
			if(tmr == 60) {
				state++;
				tmr = -1;
			}
			break;
		case TABLET_ARMS_RAISED:
			if(tmr == 0)
				fdActorSelectAnimationStub(owner, ANIM_LEADER_ARMS_RAISED);
			if(tmr == 10) {
				state++;
				tmr = -1;
			}
			break;
		case TABLET_RUNES_CIRCLE:
			if(tmr == 0) {
				piece = summonTabletState->runes;
				delay = 0;
				for(i = 0; i < TABLET_RUNE_COUNT; i++) {
					piece->tmr = delay;
					delay -= 3;
					piece++;
				}
				summonTabletState->radius = toFIXED(60);
				summonTabletState->ang = TRIG_TURN * 3 / 4;
				summonTabletState->originX = startX;
				summonTabletState->originZ = startZ;
				summonTabletState->moving = 1;
				summonTabletState->spacing = toFIXED(4);
				summonTabletState->state = RUNES_CIRCLE;
			}
			if(tmr == 0)
				sySndPlayStub(SE_FALLING_LIGHT);
			if(tmr == 48)
				sySndPlayStub(SE_FALLING_LIGHT);
			if(tmr == 96)
				sySndPlayStub(SE_FALLING_LIGHT);
			if(tmr == 100) {
				state++;
				tmr = -1;
			}
			break;
		case TABLET_RUNES_TO_LEADER:
			summonTabletState->originX = tmr * (ownerX - startX) / 30 + startX;
			summonTabletState->originZ = tmr * (ownerZ - startZ) / 30 + startZ;
			if(tmr == 30) {
				state++;
				tmr = -1;
			}
			break;
		case TABLET_RUNES_CLOSE_IN:
			if(tmr == 0)
				summonTabletState->spacing = 0;
			summonTabletState->radius += -toFIXED(2);
			if(summonTabletState->radius < 0) {
				summonTabletState->radius = 0;
				state++;
				tmr = -1;
			}
			break;
		case TABLET_LEADER_SQUASHED:
			if(tmr == 0)
				fdActorStartScriptStub(owner, MovementTypeSquashScript);
			if((tmr & 3) == 0)
				sySndPlayStub(SE_MAP_TRANSITION);
			summonTabletState->state = RUNES_GATHER;
			if(tmr == 60)
				state = FIELD_PSYNERGY_FINISHED;
			break;
		}
		for(i = 0; i < newPebbles; i++) {
			ang = syRandom();
			piece = &summonTabletState->pebbles[nextPebble];
			piece->tmr = 0;
			piece->pebble.pos[0] = tgt->x.fixed;
			piece->pebble.pos[1] = tgt->y.fixed + toFIXED(12);
			piece->pebble.pos[2] = tgt->z.fixed;
			syVectorAddPolarOffset(
				syRandom() * 4 + toFIXED(2), ang, piece->pebble.pos);
			piece->pebble.vel[0] = 0;
			piece->pebble.vel[1] = toFIXED(9) / 2;
			piece->pebble.vel[2] = 0;
			syVectorAddPolarOffset(syRandom() * 2 + toFIXED(2), ang,
				summonTabletState->pebbles[nextPebble].pebble.vel);
			nextPebble = (nextPebble + 1) & (TABLET_PEBBLE_COUNT - 1);
		}
		newPebbles = 0;
		sySynch(1);
		tmr++;
	} while(state != FIELD_PSYNERGY_FINISHED);
	fdActorStartIdleStub(owner);
	owner->scaleX = toFIXED(1);
	owner->scaleY = toFIXED(1);
	fdScriptWait(10);
	syCloseEvent(fdSummonTabletUpdateRunes);
	fdPsynergyFadeFromCastCol();
	syVRAMCacheFreeEntry(runeEntry);
	syVRAMCacheFreeEntry(pebbleEntry);
	syHeapSlotRelease(HEAP_SLOT_BATTLE_EFFECT);
}

/* A flag block: a puddle's actor and the flag that says it is frozen, in a list
   ended by ACTOR_LIST_END (fdFlagBlockPlaceAll). */
typedef struct ACTOR_FLAG {
	Uint16 actor;
	Sint16 flg;
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
	MAP_STATE  *map = Ram_HeapSlots->mapGrid;
	ACTOR_FLAG *list = actors;
	ACTOR	   *leader;
	Sint32		height;

	for(; actors->actor != ACTOR_LIST_END; actors++) {
		ACTOR		 *actor = fdActorGetById(actors->actor);
		Uint16		  flg = actors->flg;
		Sint32		  slot = fdActorGetMetatileIndex(actor);
		MAP_METATILE *metatile = &map->layers[actor->layer].metatiles[slot];

		actor->motionFlg = 0;
		actor->collisionFlg &= ~ACTOR_SOLID;
		fdActorSetSprFlagsStub(actor, 0);
		metatile->collisionCode = METATILE_BEHAVIOR_WALL;
		if(syGameFlagGet(flg)) {
			fdActorSelectAnimationStub(actor, PUDDLE_ANIMATION_FROZEN);
			syGameFlagClear(flg);
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

/* Blaze, and the check whether an actor stands where the map or another actor
   blocks it. Part of field_psynergy.c. */

extern const Sint32 BlazeTintColors[];

/* The first layer of an actor's sprite. */
#define ACTOR_FIRST_LAYER(actor) (((SPR *)(actor)->spr)->entries[0])

/* The jet's longest spray, in ticks; B ends it sooner. */
#define BLAZE_SPRAY_TICKS 80
/* How far in front of the caster the jet's flames are drawn. */
#define BLAZE_FLAME_DEPTH 10

/* The flame's colour, after the torch it is cast at: an index of
   BlazeTintColors. */
typedef enum BLAZE_LOOK {
	BLAZE_RED,
	BLAZE_VIOLET,
	BLAZE_YELLOW,
	BLAZE_BLUE
} BLAZE_LOOK;

/* Blaze: a jet of flame sprays from the caster, with smoke, until B is pressed
   or for BLAZE_SPRAY_TICKS at most: red, or violet, yellow or blue to match
   the torch (SPRITE_MARS_TORCH_LIT) it is cast at. Each actor it reaches runs
   its Blaze script, and one that is lit by it ends the jet, its actor id kept
   in the field state. Cast at nothing, the nearest fire in reach is the target:
   a brazier, a lit candlestick, a lit Mars Lighthouse torch, or an actor
   wearing SPRITE_EXCLAMATION_BUBBLE (the Gondowan Cliffs hideout's unseen
   marker at its fire). */
void fdPsynergyBlaze(void)
{
	BLAZE_STATE		   *state;
	FIELD_STATE		   *event;
	FIELD_EFFECT_STATE *scene;
	ACTOR			   *caster;
	ACTOR			   *actor;
	EFFECT_SPR		   *spr;

	union {
		ACTOR					*actor;
		FIELD_PSYNERGY_PARTICLE *particle;
	} cursor;

	void  *pending;
	Sint32 pos[3];
	Sint32 kind;
	Sint32 look;
	Sint32 best;
	Sint32 distance;
	FIXED  height;
	Sint32 id;
	Sint32 resourceId;
	Sint32 flameSlot;
	Sint32 puffSlot;
	Sint32 tile;
	Sint32 i;
	Sint32 j;
	Sint32 k;
	Sint32 mode;

	state = syHeapSlotAlloc(HEAP_SLOT_BATTLE_EFFECT, sizeof(BLAZE_STATE));
	scene = RamHeapSlots->fieldEffState;
	event = RamHeapSlots->fieldState;
	caster = scene->caster;
	look = BLAZE_RED;
	actor = scene->tgt;
	if(fdActorGetSprIdStub(actor) == SPRITE_MARS_TORCH_LIT) {
		kind = ACTOR_FIRST_LAYER(actor)->colMode;
		if(kind == SPRITE_COLOR_GBR)
			look = BLAZE_VIOLET;
		if(kind == SPRITE_COLOR_GRB)
			look = BLAZE_YELLOW;
		if(kind == SPRITE_COLOR_BGR)
			look = BLAZE_BLUE;
	}
	fdStartColTransition(8, BlazeTintColors[look]);
	state->mode = BLAZE_FLICKER;
	if(scene->aimAhead != 0) {
		state->origin[0] = scene->x;
		state->origin[1] = scene->y + toFIXED(16);
		state->origin[2] = scene->z;
		best = BEYOND_REACH;
		for(i = 0; i < FIELD_ACTOR_COUNT; i++) {
			cursor.actor = event->actors[i];
			id = ACTOR_FIRST_LAYER(cursor.actor)->anmId;
			if(cursor.actor != NULL && cursor.actor->script != NULL &&
				(id == SPRITE_EXCLAMATION_BUBBLE || id == SPRITE_BRAZIER ||
					id == SPRITE_LIT_CANDLESTICK ||
					id == SPRITE_MARS_TORCH_LIT)) {
				distance = fdMeasurePsynergyReach(
					scene->fieldPsynergy, scene->casterId, i);
				if(distance != -1 && best > distance) {
					height = toFIXED((Sint8)fdResourceGetMetadataRecordStubStub(
						ACTOR_FIRST_LAYER(cursor.actor)->anmId)
							->sprTop);
					state->mode = BLAZE_LIGHT_TORCH;
					state->origin[0] = cursor.actor->x.fixed;
					state->origin[1] = cursor.actor->y.fixed + height;
					state->origin[2] = cursor.actor->z.fixed;
					scene->tgtId = i;
					scene->tgt = cursor.actor;
					best = distance;
				}
			}
		}
	} else {
		if(event->blazeLongSpray == 0)
			state->mode = BLAZE_SPRAY;
		else
			state->mode = BLAZE_LONG_SPRAY;
		if(scene->tgt != NULL)
			height = toFIXED((Sint8)fdResourceGetMetadataRecordStubStub(
				ACTOR_FIRST_LAYER(scene->tgt)->anmId)
					->sprTop);
		else
			height = toFIXED(20);
		state->origin[0] = scene->x;
		state->origin[1] = scene->y + height;
		state->origin[2] = scene->z;
	}

	mode = state->mode;
	if(mode == BLAZE_LIGHT_TORCH) {
		pos[0] = state->origin[0];
		pos[1] = state->origin[1];
		pos[2] = state->origin[2];
		actor = fdActorSpawn(SPRITE_BLAZE_FLAME, pos[0], pos[1], pos[2]);
		if(actor != NULL) {
			actor->motionFlg = 0;
			actor->scaleX = toFIXED(5) / 4;
			actor->scaleY = toFIXED(5) / 4;
			fdActorSelectAnimationStub(actor, 2);
			sySndPlayStub(SE_SOL_SANCTUM_LIGHT_UP);
			sySynch(50);
			fdActorReleaseStub(actor);
			sySynch(10);
		}
	} else if(mode == BLAZE_FLICKER) {
		pos[0] = state->origin[0];
		pos[1] = state->origin[1];
		pos[2] = state->origin[2];
		actor = fdActorSpawn(SPRITE_BLAZE_FLICKER, pos[0], pos[1], pos[2]);
		if(actor != NULL) {
			actor->motionFlg = mode;
			fdActorSelectAnimationStub(actor, 2);
			sySndPlayStub(SE_SOL_SANCTUM_LIGHT_UP);
			sySynch(45);
			fdActorReleaseStub(actor);
			sySynch(10);
		}
	} else {

		sySndPlayStub(SE_RUSH);
		fdBattleEffectSetObjectUpdateMode(scene->caster, CASTER_LOOK_NORMAL);
		state->ang = scene->ang8;
		state->tgtAng = scene->ang8;
		switch(look) {
		case BLAZE_RED:
			resourceId = RESOURCE_RED_FLAME_BURST;
			break;
		case BLAZE_VIOLET:
			resourceId = RESOURCE_VIOLET_FLAME_BURST;
			break;
		case BLAZE_YELLOW:
			resourceId = RESOURCE_YELLOW_FLAME_BURST;
			break;
		case BLAZE_BLUE:
			resourceId = RESOURCE_BLUE_FLAME_BURST;
			break;
		default:
			resourceId = RESOURCE_RED_FLAME_BURST;
			break;
		}
		syDecompressTaggedLZ(syResourceGetTableEntry(resourceId), state);
		flameSlot = syVRAMCacheFindFreeEntry();
		tile = syVRAMCacheLoad(flameSlot, 4096, state);
		state->flameTile = tile;
		cursor.particle = state->flameMotion;
		for(j = 0; j < BLAZE_FLAMES; j++) {
			spr = &state->flames[j];

			fdEffectSprInit(spr, 15, 24, OBJ_SIZE_32X32, tile);
			spr->depth = fdActorGetSprDepth(caster) + BLAZE_FLAME_DEPTH;
			spr->oam.pri = fdActorGetSprPriority(caster);
			spr->oam.fullCol = 1;
			spr->oam.pal = 0;
			cursor.particle->tmr = -1;
			cursor.particle++;
		}
		syDecompressTaggedLZ(
			syResourceGetTableEntry(RESOURCE_DUST_PUFF_A), state);
		puffSlot = syVRAMCacheFindFreeEntry();
		tile = syVRAMCacheLoad(puffSlot, 2048, state);
		state->puffTile = tile;
		cursor.particle = state->puffMotion;
		for(j = 0; j < BLAZE_FLAMES; j++) {
			spr = &state->puffs[j];

			fdEffectSprInit(spr, 8, 15, OBJ_SIZE_16X16, tile);
			spr->scaleY = spr->scaleX = toFIXED(3) / 4;
			spr->depth = EFFECT_SPRITE_FRONT_DEPTH;
			spr->oam.pri = 0;
			spr->oam.fullCol = 1;
			spr->oam.pal = 0;
			cursor.particle->tmr = -1;
			cursor.particle++;
		}
		sySynch(1);
		state->emitting = 1;
		sySetEvent(fdBlazeUpdateSpray, EVENT_PASS_TICK);
		for(k = 0; k < BLAZE_SPRAY_TICKS; k++) {
			sySynch(1);
			if(state->hit != 0) {
				pending = fdFindPsynergyCue(PSYNERGY_CUE(PSYNERGY_PHASE_HITS),
					scene->fieldPsynergy,
					(Sint16)(state->hitObj | PSYNERGY_TARGET_ACTOR));
				if(pending != NULL)
					fdRunPsynergyScript(
						pending, scene->casterId, state->hitObj);
				if(fdFindPsynergyCue(PSYNERGY_CUE(PSYNERGY_PHASE_AFTER_END),
					   scene->fieldPsynergy,
					   (Sint16)(state->hitObj | PSYNERGY_TARGET_ACTOR)) !=
					NULL) {
					event->psynergyHitObj = state->hitObj;
					break;
				}
			}
			if(PerWork.push & KEY_B)
				break;
		}
		sySndPlayStub(SOUND_FADE_OUT_EXIT_SE);
		state->emitting = 0;
		for(k = 0; k < 60; k++) {
			cursor.particle = state->flameMotion;
			for(j = 0; j < BLAZE_FLAMES && cursor.particle->tmr == -1;
				j++, cursor.particle++)
				;
			if(j == BLAZE_FLAMES)
				break;
			sySynch(1);
		}
		syCloseEvent(fdBlazeUpdateSpray);
		syVRAMCacheFreeEntry(flameSlot);
		syVRAMCacheFreeEntry(puffSlot);
	}
	fdPsynergyFadeFromCastCol();
	syHeapSlotRelease(HEAP_SLOT_BATTLE_EFFECT);
}

/* Nonzero when the map blocks an actor at (x, z) on a layer at a height. */
Sint32 fdMapGridCheckCollisionAtStub(
	Sint32 x, Sint32 z, Sint32 layer, Sint32 height);

/* Says whether an actor stands where it may not: 1 when, outside the World Map,
   fdMapGridCheckCollisionAtStub reports its metatile, ORed with 2 when it
   overlaps any other solid actor (each radius less 2 pixels). */
Sint32 fdActorCheckBlocked(ACTOR *actor)
{
	Sint32 result;
	ACTOR *other;
	Sint32 i;

	FIELD_STATE *state = RamHeapSlots->fieldState;
	result = 0;
	if(state->mode != ACTOR_SYSTEM_WORLD_MAP) {
		if(fdMapGridCheckCollisionAtStub(
			   actor->x.fixed, actor->z.fixed, 0, actor->groundHeight) != 0)
			result = 1;
	}
	other = RamHeapSlots->actors;
	for(i = 0; i < ACTOR_COUNT; i++, other++) {
		if(other->script != NULL && (other->collisionFlg & ACTOR_SOLID) &&
			other != actor) {
			if(fdCheckSpheresOverlapStub(&actor->x.fixed, actor->radius - 2,
				   &other->x.fixed, other->radius - 2) >= 0)
				result |= 2;
		}
	}
	return result;
}

#undef BLAZE_SPRAY_TICKS
#undef BLAZE_FLAME_DEPTH

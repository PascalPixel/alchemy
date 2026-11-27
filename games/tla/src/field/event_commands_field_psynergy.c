/* @file */
/* Helpers the field Psynergy kinds share: map cells, spawned actors and */
/* particles, and the field's tint. */
/* Part of event_commands.c. */

#include "field/field_effect.h"

/* If the scene's sprite is active, stops its update callback */
/* (fdPsynergyDrawCanvasSpr) and frees its VRAM cache entry. */
void fdPsynergyReleaseSceneSpr(void) {
	FIELD_EFFECT_STATE *effectState = Ram_HeapSlots->fieldEffState;

	if(effectState->sprActive != 0) {
		effectState->sprActive = 0;
		syCloseEvent(fdPsynergyDrawCanvasSpr);
		syVramCacheFreeEntry(effectState->sprVramSlot);
	}
}

/* Returns the map cell at position x, z in the given layer, read from the */
/* world map's blocks or the cell buffer by the event state's mode. */
MAP_CELL *fdMapGridGetCellAt(s32 x, s32 z, s32 layer) {
	FIELD_STATE *event = Ram_HeapSlots->fieldState;
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	MAP_CELL *cells;

	if(event->mode == ACTOR_SYSTEM_WORLD_MAP) {
		cells = &(
			(MAP_CELL *)syRamMapBlocks)[((x / toFIXED(WORLD_MAP_CELL_PIXELS)) &
										 (WORLD_MAP_VIEW_CELLS - 1)) +
										((z / toFIXED(WORLD_MAP_CELL_PIXELS)) &
										 (WORLD_MAP_VIEW_CELLS - 1)) *
											WORLD_MAP_VIEW_CELLS];
	}
	else {
		if(layer <= MAPGRID_LAYER_OVERHEAD)
			cells = map->layers[layer].cells;
		else
			cells = (MAP_CELL *)Ram_MapCellBuffer;
		cells = &cells[x / toFIXED(MAPGRID_CELL_PIXELS) +
					   (z / toFIXED(MAPGRID_CELL_PIXELS)) * MAPGRID_SIZE];
	}
	return cells;
}

/* Whether the Scoop can dig at (x, z): on the world map, sand; elsewhere not */
/* on a Cyclone cell, water or raised ground, and then on sand, or on a */
/* MAP_CELL_SCOOP cell (any other, where FIELD_STATE.sceneAttributeLow is set).
 */
s32 fdMapGridCanScoop(s32 x, s32 z, s32 layer) {
	FIELD_STATE *event = Ram_HeapSlots->fieldState;

	if(event->mode == ACTOR_SYSTEM_WORLD_MAP)
		return fdMapGridIsSandCell(x, z, layer);
	if(fdMapGridGetCycloneCellKind(x, z, layer) != CYCLONE_CELL_NONE ||
	   fdMapGridIsWaterCell(x, z, layer) != 0 ||
	   fdMapGridGetTerrainKindAtStub(layer, x, z) != 0)
		return 0;
	if(fdMapGridIsSandCell(x, z, layer) != 0)
		return 1;
	if(event->sceneAttributeLow == 0)
		return fdMapGridGetCellAt(x, z, layer)->attributeB & MAP_CELL_SCOOP;
	return ~fdMapGridGetCellAt(x, z, layer)->attributeB & MAP_CELL_SCOOP;
}

/* Whether Cloak can hide the leader on the cell at (x, z): the overhead */
/* layer's collision code is CELL_BEHAVIOR_CLOAK, whatever layer is passed. */
s32 fdMapGridIsCloakCell(s32 x, s32 z, s32 layer) {
	return fdMapGridGetCellAt(x, z, MAPGRID_LAYER_OVERHEAD)->collisionCode ==
		   CELL_BEHAVIOR_CLOAK;
}

/* Whether Hover holds up over the cell at (x, z): the overhead layer's */
/* collision code is CELL_BEHAVIOR_HOVER, whatever layer is passed. */
s32 fdMapGridIsHoverCell(s32 x, s32 z, s32 layer) {
	return fdMapGridGetCellAt(x, z, MAPGRID_LAYER_OVERHEAD)->collisionCode ==
		   CELL_BEHAVIOR_HOVER;
}

/* Whether the leader can move as sand on the cell at (x, z): the overhead */
/* layer's CELL_BEHAVIOR_SAND, or on the world map collision */
/* WORLD_MAP_COLLISION_WALKABLE_FIRST or shallow water; */
/* FIELD_STATE.sceneAttributeHigh turns the answer round. */
s32 fdMapGridIsSandCell(s32 x, s32 z, s32 layer) {
	FIELD_STATE *event = Ram_HeapSlots->fieldState;
	s32 column = x;
	if(event->mode == ACTOR_SYSTEM_WORLD_MAP) {
		s32 position[3];

		SET_VECTOR(position, column, 0, z);
		column = (u32)(scWorldMapGetCollisionStub(position) -
					   WORLD_MAP_COLLISION_WALKABLE_FIRST) <=
				 WORLD_MAP_COLLISION_SHALLOW_WATER -
					 WORLD_MAP_COLLISION_WALKABLE_FIRST;
	}
	else {
		s32 code = fdMapGridGetCellAt(column, z, MAPGRID_LAYER_OVERHEAD)
					   ->collisionCode;

		column = code == CELL_BEHAVIOR_SAND;
	}
	if(event->sceneAttributeHigh != 0)
		column = column == 0;
	return column;
}

/* Whether the cell at (x, z) can be climbed (MAP_CELL_CLIMBABLE): Sand starts
 */
/* up a wall face only there (fdActorMoveAsSand) and ends its climb off one. */
u8 fdMapGridIsClimbableCell(s32 x, s32 z, s32 layer) {
	return fdMapGridGetCellAt(x, z, layer)->attributeB & MAP_CELL_CLIMBABLE;
}

/* Whether the cell at (x, z) is water: MAP_CELL_WATER, or on the world map */
/* collision WORLD_MAP_COLLISION_WATER_FIRST to WORLD_MAP_COLLISION_SEA_LAST. */
s32 fdMapGridIsWaterCell(s32 x, s32 z, s32 layer) {
	FIELD_STATE *event = Ram_HeapSlots->fieldState;

	if(event->mode == ACTOR_SYSTEM_WORLD_MAP) {
		s32 position[3];
		s32 collision;

		position[1] = 0;
		position[0] = x;
		position[2] = z;
		collision = scWorldMapGetCollisionStub(position);
		return (u32)(collision - WORLD_MAP_COLLISION_WATER_FIRST) <=
			   WORLD_MAP_COLLISION_SEA_LAST - WORLD_MAP_COLLISION_WATER_FIRST;
	}
	return (u8)(fdMapGridGetCellAt(x, z, layer)->attributeB & MAP_CELL_WATER);
}

/* The Cyclone's kind of the cell at (x, z), from the overhead layer's */
/* collision code (CYCLONE_CELL_KIND). */
/* fdPsynergyGatherNearbyCells lists them. */
s32 fdMapGridGetCycloneCellKind(s32 x, s32 z, s32 layer) {
	u8 code = fdMapGridGetCellAt(x, z, MAPGRID_LAYER_OVERHEAD)->collisionCode;

	if(code == CELL_BEHAVIOR_CYCLONE)
		return CYCLONE_CELL_BURST;
	if(code == CELL_BEHAVIOR_CYCLONE_CARRY)
		return CYCLONE_CELL_CARRY;
	if(code == CELL_BEHAVIOR_CYCLONE_LIFT)
		return CYCLONE_CELL_LIFT;
	return CYCLONE_CELL_NONE;
}

/* Returns the behaviour (collision code) of the map cell at x, z in layer. */
u8 fdMapGridGetCellBehavior(s32 x, s32 z, s32 layer) {
	return fdMapGridGetCellAt(x, z, layer)->collisionCode;
}

/* Sets the cell behavior of the map cell at x, z in layer, and returns it. */
u8 fdMapGridSetCellBehavior(s32 x, s32 z, s32 layer, s32 value) {
	fdMapGridGetCellAt(x, z, layer)->collisionCode = value;
	return value;
}

/* Returns the attribute byte of the map cell at x, z in layer. */
s32 fdMapGridGetCellAttributesAt(s32 x, s32 z, s32 layer) {
	return fdMapGridGetCellAt(x, z, layer)->attributeB;
}

/* Sets the attribute byte of the map cell at x, z in layer. */
u8 fdMapGridSetCellAttributesAt(s32 x, s32 z, s32 layer, s32 value) {
	fdMapGridGetCellAt(x, z, layer)->attributeB = value;
	return value;
}

/* Sets bits in the attribute byte of the map cell at x, z in layer. */
u8 fdMapGridSetCellAttributeBitsAt(s32 x, s32 z, s32 layer, s32 bits) {
	s32 flags = fdMapGridGetCellAttributesAt(x, z, layer);
	MAP_CELL *cell = fdMapGridGetCellAt(x, z, layer);

	flags |= bits;
	cell->attributeB = flags;
	return flags;
}

/* Clears bits in the attribute byte of the map cell at x, z in layer. */
u8 fdMapGridClearAttributeBBitsAt(s32 x, s32 z, s32 layer, s32 bits) {
	s32 flags = fdMapGridGetCellAttributesAt(x, z, layer);
	MAP_CELL *cell = fdMapGridGetCellAt(x, z, layer);

	flags &= ~bits;
	cell->attributeB = flags;
	return flags;
}

/* Returns 1 when two upright cylinders overlap, their centres closer on the */
/* ground than the sum of radiusA and radiusB; else -1. */
s32 fdPsynergyCheckOverlap(s32 *a, s32 radiusA, s32 *b, s32 radiusB) {
	s32 dx = FIXEDtoINT(*a++ - *b++);
	s32 dy = FIXEDtoINT(*a++ - *b++);
	s32 dz = FIXEDtoINT(*a - *b);
	s32 radius = radiusA + radiusB;

	/* The box and height bounds are 16.16 against whole-pixel differences, so
	 */
	/* on a map they never reject a pair and the ground circle alone decides. */
	if(dx > toFIXED(64) || dz > toFIXED(64))
		return -1;
	if(dy > INTtoFIXED(radius))
		return -1;
	if(dx * dx + dz * dz < radius * radius)
		return 1;
	return -1;
}

/* Moves the actor from 16 pixels above the caster to (x, y, z), growing it */
/* from a quarter to full size. */
void fdPsynergyGrowFromCaster(ACTOR *actor, s32 x, s32 y, s32 z) {
	FIELD_EFFECT_STATE *scene = Ram_HeapSlots->fieldEffState;
	ACTOR *mainActor = scene->caster;
	FIXED start[3];
	s32 i = 0;

	if(scene->ang != TRIG_TURN * 3 / 4)
		fdActorSetSprPriorityDirectStub(actor, 0);
	start[0] = mainActor->x.fixed;
	start[1] = mainActor->y.fixed + toFIXED(16);
	start[2] = mainActor->z.fixed;
	for(; i <= GROW_STEPS; i++) {
		SET_ACTOR_POSITION(actor, start[0] + (x - start[0]) * i / GROW_STEPS,
						   start[1] + (y - start[1]) * i / GROW_STEPS,
						   start[2] + (z - start[2]) * i / GROW_STEPS);
		actor->scaleY = actor->scaleX =
			i * (FIXED_ONE * 3 / 4) / GROW_STEPS + FIXED_ONE / 4;
		sySynch(1);
	}
}

/* fdPsynergyGrowFromCaster backwards: moves the actor from (x, y, z) to */
/* 16 pixels above the caster, shrinking it from full size to a quarter. */
void fdPsynergyShrinkToCaster(ACTOR *actor, s32 x, s32 y, s32 z) {
	ACTOR *mainActor =
		((FIELD_EFFECT_STATE *)Ram_HeapSlots->fieldEffState)->caster;
	FIXED start[3];
	s32 i;

	i = 0;
	start[0] = mainActor->x.fixed;
	start[1] = mainActor->y.fixed + toFIXED(16);
	start[2] = mainActor->z.fixed;
	for(; i <= GROW_STEPS; i++) {
		SET_ACTOR_POSITION(actor, x + (start[0] - x) * i / GROW_STEPS,
						   y + (start[1] - y) * i / GROW_STEPS,
						   z + (start[2] - z) * i / GROW_STEPS);
		actor->scaleY = actor->scaleX =
			i * -(FIXED_ONE * 3 / 4) / GROW_STEPS + FIXED_ONE;
		sySynch(1);
	}
}

/* Spawns a sparkle at x, y, z that hops off in a random */
/* direction 8 to 32 pixels away at 1 to 2 pixels a frame. */
ACTOR *fdBattleEffectSpawnScatterParticle(s32 x, s32 y, s32 z) {
	ACTOR *actor = fdActorSpawn(SPRITE_SPARKLE, x, y, z);
	s32 magnitude;

	if(actor != NULL) {
		fdActorStartScriptStub(actor, FieldEffect_FragmentScript);
		actor->speedLimit = syRandom() + FIXED_ONE;
		actor->acceleration = FIXED_ONE;
		actor->motionFlg = ACTOR_FALLS;
		actor->gravity = toFIXED(1) / 25;
		magnitude = syRandom();
		magnitude -= syRandom();
		actor->velY = magnitude;
		fdActorSetTargetPosFromMagnitudeAng(actor, syRandom() * 24 + toFIXED(8),
											syRandom());
	}
	return actor;
}

/* Counts the free blocks of the sprite VRAM cache. */
s32 fdResourceTableCountFreeBlocks(void) {
	u8 *marker = VramCacheBlockOwners;
	s32 freeCount;
	s32 remaining;

	freeCount = 0;
	remaining = VRAM_BLOCK_COUNT;
	do {
		if(*marker++ == VRAM_BLOCK_OWNER_FREE) {
			freeCount++;
		}
		remaining--;
	} while(remaining != 0);
	return freeCount;
}

/* Makes sourceSprite share sharedSprite's VRAM block, releasing its own; with
 */
/* no sharedSprite it keeps its block. */
void *fdSprShareVramCacheEntry(void *sourceSprite, void *sharedSprite) {
	SPR *shared;
	SPR *sprite;

	sprite = sourceSprite;
	shared = NULL;
	if(sprite != NULL) {
		if(sharedSprite == NULL) {
			sprite->retainCacheEntry = 1;
		}
		else {
			syVramCacheFreeEntry(sprite->slot);
			sprite->slot = ((SPR *)sharedSprite)->slot;
			sprite->retainCacheEntry = 1;
			sprite = sharedSprite;
		}
		shared = sprite;
	}
	return shared;
}

#define fdActorSetAnimationSpeedStub                                           \
	((void (*)(ACTOR *, s32))fdActorSetAnimationSpeedStub)

/* Creates an actor for a field Psynergy, at the caster's ground height, with */
/* sprite priority 0 and animation 1. */
ACTOR *fdActorSpawn(s32 kind, s32 x, s32 y, s32 z) {
	FIELD_EFFECT_STATE *scene = Ram_HeapSlots->fieldEffState;
	ACTOR *actor;

	actor = fdActorCreateStub(kind, x, y, z);
	if(actor != NULL) {
		if(actor->sprStorageKind == ACTOR_SPRITE_NONE) {
			fdActorReleaseStub(actor);
			return NULL;
		}
		actor->groundHeight = scene->caster->groundHeight;
		actor->motionFlg = ACTOR_FLOATS;
		actor->priFlg = ACTOR_PRIORITY_OVERHEAD;
		((SPR *)actor->spr)->image.pri = 0;
		fdActorSetSprFlagsStub(actor, 0);
		fdActorSelectAnimationStub(actor, 1);
	}
	return actor;
}

/* Pauses or resumes every actor but the Halted one and excluded, and sets */
/* their animation speed, as Sand stills the field around the leader. */
void fdActorSetOthersPaused(ACTOR *excluded, s32 paused, s32 animationSpeed) {
	s16 *haltedId;
	ACTOR *actor;
	s32 actorId;

	actorId = 0;
	haltedId = &MainRoutineStateWork.haltedActor;
	do {
		actor = fdActorLookup(actorId);
		if(actorId != *haltedId && actor != NULL && actor != excluded) {
			actor->paused = paused;
			fdActorSetAnimationSpeedStub(actor, animationSpeed);
		}
		actorId++;
		/* BUG: one past the actor table: fdActorLookup(FIELD_ACTOR_COUNT) reads
		 * the */
		/* step trigger and push cell requests as an actor. */
#ifdef BUGFIX
	} while(actorId < FIELD_ACTOR_COUNT);
#else
	} while(actorId <= FIELD_ACTOR_COUNT);
#endif
}

/* The channels of those colours, three a colour, before the OBJ colours'. */
#define BG_CHANNELS (PALETTE_FADE_BG_COLORS * 3)

/* Fades the palettes to color over frames frames, keeping the field's colours
 */
/* for fdStartPaletteTransition; FLAG_PSYNERGY_TINTED is set while it */
/* holds. */
void fdStartColTransition(s32 frames, s32 color) {
	FIELD_STATE *field = Ram_HeapSlots->fieldState;
	PALETTE_FADE *state = Ram_HeapSlots->palFade;

	/* The OBJ colours are kept always, the BG colours only while Reveal has not
	 */
	/* greyed them. */
	syDMAXCopy(&state->tgt[BG_CHANNELS], &field->savedFadeTgt[BG_CHANNELS],
			   DMA_COPY_WORDS((PALETTE_FADE_CHANNELS - BG_CHANNELS) / 2),
			   REG_DMA3);
	if(field->revealActive == 0)
		syDMAXCopy(state->tgt, field->savedFadeTgt,
				   DMA_COPY_WORDS(BG_CHANNELS / 2), REG_DMA3);
	syDMAXCopy(state->tgt, state->cur, DMA_COPY_WORDS(sizeof state->cur / 4),
			   REG_DMA3);
	fdPaletteFadeSetTarget(PALETTE_TINT_CUSTOM | color, PALETTES_BG);
	syGameFlagSet(FLAG_PSYNERGY_TINTED);
	fdPaletteFadeStartInterpolation(frames);
}

/* Fades the palettes back from fdStartColTransition's colour over frames */
/* frames, the BG half to grey while Reveal is active; clears */
/* FLAG_PSYNERGY_TINTED. */
void fdStartPaletteTransition(s32 frames) {
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	s32 savedPalettes = (s32)state->savedFadeTgt;

	fdPaletteFadeSetTarget(savedPalettes, PALETTES_OBJECTS);
	if(state->revealActive != 0)
		fdPaletteFadeSetTarget(PALETTE_TINT_GRAYSCALE, PALETTES_BG);
	else
		fdPaletteFadeSetTarget(savedPalettes, PALETTES_BG);
	syGameFlagClear(FLAG_PSYNERGY_TINTED);
	fdPaletteFadeStartInterpolation(frames);
}

/* Tints the field for a Psynergy animation: a tint chosen by the frame */
/* counter (so each cast looks a little different), fixed for some */
/* animations, and forced by the tint flags; then fades towards it over eight */
/* frames. The palettes are copied by fdStartColTransition. */
void fdPsynergyFadeToCastCol(void) {
	FIELD_EFFECT_STATE *scene = Ram_HeapSlots->fieldEffState;
	s32 tint;
	s32 animation;

	tint = FrameCount & 7;
	animation = scene->fieldPsynergy;
	if(animation == FIELD_PSYNERGY_POUND)
		tint = PSYNERGY_TINT_BLUE;
	if(animation == FIELD_PSYNERGY_BLAZE)
		tint = PSYNERGY_TINT_RED;
	if(animation == FIELD_PSYNERGY_SAND)
		tint = PSYNERGY_TINT_GREY;
	if(animation == FIELD_PSYNERGY_CYCLONE)
		tint = PSYNERGY_TINT_BLUE;
	if(animation == FIELD_PSYNERGY_SCOOP)
		tint = PSYNERGY_TINT_YELLOW;
	if(animation == FIELD_PSYNERGY_ARROW)
		tint = PSYNERGY_TINT_BLUE;
	if(animation == FIELD_PSYNERGY_PARCH)
		tint = PSYNERGY_TINT_GREY;
	if(animation == FIELD_PSYNERGY_TREMOR)
		tint = PSYNERGY_TINT_DARK_GREY;
	if(animation == FIELD_PSYNERGY_GRIND)
		tint = PSYNERGY_TINT_BLUE;
	if(animation == FIELD_PSYNERGY_BURST)
		tint = PSYNERGY_TINT_GREY;
	if(scene->fieldPsynergy == FIELD_PSYNERGY_TELEPORT)
		tint = PSYNERGY_TINT_DARK_GREY;

	if(syGameFlagGet(FLAG_PSYNERGY_TINT(PSYNERGY_TINT_BLUE)))
		tint = PSYNERGY_TINT_BLUE;
	if(syGameFlagGet(FLAG_PSYNERGY_TINT(PSYNERGY_TINT_GREEN)))
		tint = PSYNERGY_TINT_GREEN;
	if(syGameFlagGet(FLAG_PSYNERGY_TINT(PSYNERGY_TINT_RED)))
		tint = PSYNERGY_TINT_RED;
	if(syGameFlagGet(FLAG_PSYNERGY_TINT(PSYNERGY_TINT_CYAN)))
		tint = PSYNERGY_TINT_CYAN;
	if(syGameFlagGet(FLAG_PSYNERGY_TINT(PSYNERGY_TINT_MAGENTA)))
		tint = PSYNERGY_TINT_MAGENTA;
	if(syGameFlagGet(FLAG_PSYNERGY_TINT(PSYNERGY_TINT_YELLOW)))
		tint = PSYNERGY_TINT_YELLOW;
	if(syGameFlagGet(FLAG_PSYNERGY_TINT(PSYNERGY_TINT_DARK_GREY)))
		tint = PSYNERGY_TINT_DARK_GREY;
	if(syGameFlagGet(FLAG_PSYNERGY_TINT(PSYNERGY_TINT_GREY)))
		tint = PSYNERGY_TINT_GREY;
	fdStartColTransition(8, FieldPsynergy_CastColors[tint]);
}
#undef fdActorSetAnimationSpeedStub
#undef BG_CHANNELS

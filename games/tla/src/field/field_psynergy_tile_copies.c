/* @file */
/* The Cyclone's helpers: copying the cells it touched, its arcs and its */
/* burst, and a Mad Plant leaping at the leader. */
/* Part of field_psynergy.c. */

/* The frames of a Mad Plant's leap at the leader. */
#define MAD_PLANT_LEAP_FRAMES 20
/* The pixels of one of the Cyclone's arcs, and the angle from one to the */
/* next, about two degrees. */
#define CYCLONE_ARC_PIXELS	   30
#define CYCLONE_ARC_PIXEL_STEP 368

/* Clears the weeds the Cyclone blew away: for each cell it touched (the field
 */
/* state's list from fdPsynergyGatherNearbyCells) inside the width by */
/* height rectangle at (x, z) of the layer, copies the matching cell tiles */
/* from the rectangle at (toX, toZ) over it, opens it on layer 2 and records */
/* the copy in TouchCopiesWork, up to 64. Gaia Rock calls it after a Cyclone. */
void fdCycloneCopyTouchedCells(s32 x, s32 z, s16 layerIndex, s16 width,
							   s16 height, s32 toX, s32 toZ) {
	FIELD_STATE *field = Ram_HeapSlots->fieldState;
	ACTOR *owner = fdActorGetById(fdPartyGetLeaderActor());
	MAP_LAYER *layer = ((MAP_STATE *)Ram_HeapSlots->mapGrid)->layers;
	s16 *cell = field->psynergyCells;
	s32 lift;
	s32 scrollX = FIELD_CELL(layer->ofsX);
	s32 scrollZ = FIELD_CELL(layer->ofsY);
	s32 layerX, layerZ;
	s32 left, top, i;

	if(layerIndex <= 2)
		layer += layerIndex;
	layerX = FIELD_CELL(layer->ofsX);
	layerZ = FIELD_CELL(layer->ofsY);
	left = x - layerX;
	top = z - layerZ;
	lift = -(FIELD_CELL(owner->groundHeight));

	for(i = 0; cell[i * 2] != -1; i++) {
		s32 cz, cx, cellX, cellZ;

		cx = cell[i * 2];
		cellX = cx - scrollX;
		cz = cell[i * 2 + 1];
		cellZ = cz + lift - scrollZ;

		if(cellX >= left && cellX < left + width && cellZ >= top &&
		   cellZ < top + height) {
			s32 dx = cellX - left;
			s32 dz = cellZ - top;
			s32 n;

			fdMapGridCopyCellTilesCellsRectStub(toX + dx, toZ + dz, 1, 1,
												x + dx, z + dz);
			fdMapGridGetCellAt(FIELD_CELL_ORIGIN(cx), FIELD_CELL_ORIGIN(cz), 2)
				->collisionCode = 0;
			n = TouchCopiesWork.cnt;
			if(n < (s32)ARRAY_COUNT(TouchCopiesWork.copies)) {
				TouchCopiesWork.copies[n].toX = toX + dx;
				TouchCopiesWork.copies[n].toZ = toZ + dz;
				TouchCopiesWork.copies[n].fromX = x + dx;
				TouchCopiesWork.copies[n].fromZ = z + dz;
				TouchCopiesWork.copies[n].cellX = cell[i * 2];
				TouchCopiesWork.copies[n].cellZ = cell[i * 2 + 1];
				TouchCopiesWork.cnt++;
			}
		}
	}
}

/* A Mad Plant (sprite) springs out of the bushes cleared at the cell (x, z): */
/* it grows, turns to the leader, hops twice and leaps at the leader in an arc
 */
/* before the battle. The Gabomba Catacombs, Taopo Swamp and Gaia Rock each */
/* hide one. */
void fdMadPlantLeapAtLeader(s32 sprite, s32 x, s32 z) {
	s32 ownerId = fdPartyGetLeaderActor();
	ACTOR *owner = fdActorGetById(ownerId);
	ACTOR *actor = fdActorCreateStub(sprite, toFIXED(x), 0, toFIXED(z));
	FIXED from[3];
	FIXED to[3];
	s32 i;
	s32 steps;

	if(actor == NULL)
		return;
	fdActorSetSprFlagsStub(actor, 0);
	actor->gravity = toFIXED(7) / 20;
	actor->velY = toFIXED(5);
	actor->scaleY = actor->scaleX = FIXED_ONE * 2 / 5;
	do {
		sySynch(1);
		actor->scaleY = actor->scaleX = actor->scaleX + FIXED_ONE * 3 / 200;
		actor->facing += FACING_STEP;
	} while(actor->scaleX < FIXED_ONE);
	actor->scaleY = actor->scaleX = FIXED_ONE;
	fdActorSelectAnimationStub(owner, 1);
	fdActorTurnPairToFaceEachOther(actor, owner);
	owner->tmr = owner->facing;
	fdEventWait(10);
	fdActorStartRepeatedMotion(ownerId, 3);
	fdEffectSetAttached(ownerId,
						ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdEventWait(32);
	actor->gravity = toFIXED(1);
	actor->velY = toFIXED(6);
	sySndPlayStub(SE_JUMP);
	fdEventWait(15);
	actor->velY = toFIXED(6);
	sySndPlayStub(SE_JUMP);
	fdEventWait(23);
	sySndPlayStub(SE_ENEMY_CRY);

	from[0] = actor->x.fixed;
	from[1] = actor->y.fixed;
	from[2] = actor->z.fixed;
	to[0] = owner->x.fixed;
	to[1] = owner->y.fixed;
	to[2] = owner->z.fixed;
	/* A straight line, lifted by half a sine wave 40 pixels high. */
	steps = MAD_PLANT_LEAP_FRAMES;
	for(i = 0; i < steps; i++) {
		actor->x.fixed =
			from[0] + i * (to[0] - from[0]) / MAD_PLANT_LEAP_FRAMES;
		actor->y.fixed =
			from[1] + i * (to[1] - from[1]) / MAD_PLANT_LEAP_FRAMES +
			sySin(i * (TRIG_TURN / 2) / MAD_PLANT_LEAP_FRAMES) * 40;
		actor->z.fixed =
			from[2] + i * (to[2] - from[2]) / MAD_PLANT_LEAP_FRAMES;
		sySynch(1);
	}
}

/* Plots an arc of CYCLONE_ARC_PIXELS pixels at radius (at most 31) from the */
/* middle of the canvas, from angle on, shading through the canvas palette's */
/* colours 1 to 15; offsets maps each point to its byte. The Cyclone draws its
 */
/* swirl with it. */
void fdCyclonePlotArc(s32 radius, s32 angle, u16 *offsets, u8 *pixels) {
	s32 i;

	if(radius > CANVAS_WIDTH / 2 - 1)
		radius = CANVAS_WIDTH / 2 - 1;
	for(i = 0; i < CYCLONE_ARC_PIXELS; i++) {
		s32 x = FIXEDtoINT(radius * syCos(angle)) + CANVAS_WIDTH / 2;
		s32 y = FIXEDtoINT(radius * sySin(angle)) + CANVAS_WIDTH / 2;
		s32 value = i / 2 + CANVAS_PALETTE * PALETTE_COLORS + 1;

		pixels[offsets[y * CANVAS_WIDTH + x]] = value;
		angle += CYCLONE_ARC_PIXEL_STEP;
	}
}

/* Starts Cyclone's 96 leaves, scattered round the caster and spinning, to */
/* blow toward the first cell of FIELD_STATE.psynergyCells */
/* (fdParticleBurstUpdate moves them). */
void fdParticleBurstStart(void) {
	PARTICLE_BURST_STATE *state =
		(PARTICLE_BURST_STATE *)Ram_HeapSlots->battleEff;
	FIELD_EFFECT_STATE *effect = Ram_HeapSlots->fieldEffState;
	FIELD_STATE *field = Ram_HeapSlots->fieldState;
	ACTOR *owner = effect->caster;
	BURST_CELL *cell = (BURST_CELL *)field->psynergyCells;
	FIXED dx = FIELD_CELL_ORIGIN(cell->x) - owner->x.fixed + toFIXED(8);
	FIXED dz = FIELD_CELL_ORIGIN(cell->z) - owner->z.fixed + toFIXED(8);
	s32 heading = (u16)syAtan(dz, dx);
	EFFECT_SPR *sprite;
	BURST_PARTICLE *particle;
	s32 resource;
	s32 vram;
	s32 i;

	state->frm = 0;
	state->active = 1;
	syDecompressTaggedLz(syResourceGetTableEntry(RESOURCE_LEAVES), state);
	resource = syVramCacheFindFreeEntry();
	vram = syVramCacheLoad(resource, 384, state);
	state->cacheEntry = resource;
	state->tile = vram;

	for(i = 0, particle = state->particles; i < PARTICLE_BURST_COUNT;
		i++, particle++) {
		sprite = &state->sprs[i];
		fdEffectSprInit(sprite, 4, 4, 0, vram);
		sprite->depth = EFFECT_SPRITE_FRONT_DEPTH;
		sprite->oam.pri = 0;
		sprite->oam.fullCol = 1;
		sprite->oam.pal = 0;
		SET_POSITION(particle, owner->x.fixed, owner->y.fixed, owner->z.fixed);
		syVectorAddPolarOffset(syRandom() * 8 + toFIXED(8), syRandom(),
							   &particle->x);
		particle->heading = heading;
		particle->speed = syRandom() + toFIXED(1);
		particle->spin = (syRandom() >> 6) + 384;
		if(i & 1)
			particle->spin = -particle->spin;
		particle->delay = -i / 2;
	}
	sySndPlayStub(SE_SHIMMER);
	sySetEvent(fdParticleBurstUpdate, TASK_PASS_FRAME);
}

#undef MAD_PLANT_LEAP_FRAMES
#undef CYCLONE_ARC_PIXELS
#undef CYCLONE_ARC_PIXEL_STEP

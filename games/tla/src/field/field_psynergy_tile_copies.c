/* The Cyclone's helpers: copying the metatiles it touched, its arcs and its
   burst, and a Mad Plant leaping at the leader. Part of field_psynergy.c. */

/* The frames of a Mad Plant's leap at the leader. */
#define MAD_PLANT_LEAP_FRAMES  20
/* The pixels of one of the Cyclone's arcs, and the angle from one to the next,
   about two degrees. */
#define CYCLONE_ARC_PIXELS	   30
#define CYCLONE_ARC_PIXEL_STEP 368

/* Clears the weeds the Cyclone blew away: for each metatile it touched (the
   field state's list from fdPsynergyGatherNearbyMetatiles) inside the width by
   height rectangle at (x, z) of the layer, copies the matching metatile tiles
   from the rectangle at (toX, toZ) over it, opens it on layer 2 and records the
   copy in TouchCopiesWork, up to 64. Gaia Rock calls it after a Cyclone. */
void fdCycloneCopyTouchedMetatiles(Sint32 x, Sint32 z, Sint16 layerIndex,
	Sint16 width, Sint16 height, Sint32 toX, Sint32 toZ)
{
	FIELD_STATE *field = Ram_HeapSlots->fieldState;
	ACTOR		*owner = fdActorGetById(fdPartyGetLeaderActor());
	MAP_LAYER	*layer = ((MAP_STATE *)Ram_HeapSlots->mapGrid)->layers;
	Sint16		*metatile = field->psynergyMetatiles;
	Sint32		 lift;
	Sint32		 scrollX = FIELD_METATILE(layer->ofsX);
	Sint32		 scrollZ = FIELD_METATILE(layer->ofsY);
	Sint32		 layerX, layerZ;
	Sint32		 left, top, i;

	if(layerIndex <= 2)
		layer += layerIndex;
	layerX = FIELD_METATILE(layer->ofsX);
	layerZ = FIELD_METATILE(layer->ofsY);
	left = x - layerX;
	top = z - layerZ;
	lift = -(FIELD_METATILE(owner->groundHeight));

	for(i = 0; metatile[i * 2] != -1; i++) {
		Sint32 cz, cx, metatileX, metatileZ;

		cx = metatile[i * 2];
		metatileX = cx - scrollX;
		cz = metatile[i * 2 + 1];
		metatileZ = cz + lift - scrollZ;

		if(metatileX >= left && metatileX < left + width && metatileZ >= top &&
			metatileZ < top + height) {
			Sint32 dx = metatileX - left;
			Sint32 dz = metatileZ - top;
			Sint32 n;

			fdMapGridCopyMetatileTilesMetatilesRectStub(
				toX + dx, toZ + dz, 1, 1, x + dx, z + dz);
			fdMapGridGetMetatileAt(
				FIELD_METATILE_ORIGIN(cx), FIELD_METATILE_ORIGIN(cz), 2)
				->collisionCode = 0;
			n = TouchCopiesWork.cnt;
			if(n < (Sint32)ARRAY_COUNT(TouchCopiesWork.copies)) {
				TouchCopiesWork.copies[n].toX = toX + dx;
				TouchCopiesWork.copies[n].toZ = toZ + dz;
				TouchCopiesWork.copies[n].fromX = x + dx;
				TouchCopiesWork.copies[n].fromZ = z + dz;
				TouchCopiesWork.copies[n].metatileX = metatile[i * 2];
				TouchCopiesWork.copies[n].metatileZ = metatile[i * 2 + 1];
				TouchCopiesWork.cnt++;
			}
		}
	}
}

/* A Mad Plant (sprite) springs out of the bushes cleared at the metatile (x,
   z):
   it grows, turns to the leader, hops twice and leaps at the leader in an arc
   before the battle. The Gabomba Catacombs, Taopo Swamp and Gaia Rock each hide
   one. */
void fdMadPlantLeapAtLeader(Sint32 sprite, Sint32 x, Sint32 z)
{
	Sint32 ownerId = fdPartyGetLeaderActor();
	ACTOR *owner = fdActorGetById(ownerId);
	ACTOR *actor = fdActorCreateStub(sprite, toFIXED(x), 0, toFIXED(z));
	FIXED  from[3];
	FIXED  to[3];
	Sint32 i;
	Sint32 steps;

	if(actor == NULL)
		return;
	fdActorSetSprFlagsStub(actor, 0);
	actor->gravity = toFIXED(7) / 20;
	actor->velY = toFIXED(5);
	actor->scaleY = actor->scaleX = toFIXED(2) / 5;
	do {
		sySynch(1);
		actor->scaleY = actor->scaleX = actor->scaleX + toFIXED(3) / 200;
		actor->facing += FACING_STEP;
	} while(actor->scaleX < toFIXED(1));
	actor->scaleY = actor->scaleX = toFIXED(1);
	fdActorSelectAnimationStub(owner, 1);
	fdActorTurnPairToFaceEachOther(actor, owner);
	owner->tmr = owner->facing;
	fdScriptWait(10);
	fdActorStartRepeatedMotion(ownerId, 3);
	fdEffectSetAttached(
		ownerId, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(32);
	actor->gravity = toFIXED(1);
	actor->velY = toFIXED(6);
	sySndPlayStub(SE_JUMP);
	fdScriptWait(15);
	actor->velY = toFIXED(6);
	sySndPlayStub(SE_JUMP);
	fdScriptWait(23);
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
		actor->y.fixed = from[1] +
			i * (to[1] - from[1]) / MAD_PLANT_LEAP_FRAMES +
			sySin(i * (TRIG_TURN / 2) / MAD_PLANT_LEAP_FRAMES) * 40;
		actor->z.fixed =
			from[2] + i * (to[2] - from[2]) / MAD_PLANT_LEAP_FRAMES;
		sySynch(1);
	}
}

/* Plots an arc of CYCLONE_ARC_PIXELS pixels at radius (at most 31) from the
   middle of the canvas, from angle on, shading through the canvas palette's
   colours 1 to 15; offsets maps each point to its byte. The Cyclone draws its
   swirl with it. */
void fdCyclonePlotArc(
	Sint32 radius, Sint32 angle, Uint16 *offsets, Uint8 *pixels)
{
	Sint32 i;

	if(radius > CANVAS_WIDTH / 2 - 1)
		radius = CANVAS_WIDTH / 2 - 1;
	for(i = 0; i < CYCLONE_ARC_PIXELS; i++) {
		Sint32 x = FIXEDtoINT(radius * syCos(angle)) + CANVAS_WIDTH / 2;
		Sint32 y = FIXEDtoINT(radius * sySin(angle)) + CANVAS_WIDTH / 2;
		Sint32 value = i / 2 + CANVAS_PALETTE * PALETTE_COLORS + 1;

		pixels[offsets[y * CANVAS_WIDTH + x]] = value;
		angle += CYCLONE_ARC_PIXEL_STEP;
	}
}

/* Starts Cyclone's 96 leaves, scattered round the caster and spinning, to blow
   toward the first metatile of FIELD_STATE.psynergyMetatiles
   (fdParticleBurstUpdate moves them). */
void fdParticleBurstStart(void)
{
	PARTICLE_BURST_STATE *state =
		(PARTICLE_BURST_STATE *)Ram_HeapSlots->battleEff;
	FIELD_EFFECT_STATE *effect = Ram_HeapSlots->fieldEffState;
	FIELD_STATE		   *field = Ram_HeapSlots->fieldState;
	ACTOR			   *owner = effect->caster;
	BURST_METATILE	   *metatile = (BURST_METATILE *)field->psynergyMetatiles;
	FIXED dx = FIELD_METATILE_ORIGIN(metatile->x) - owner->x.fixed + toFIXED(8);
	FIXED dz = FIELD_METATILE_ORIGIN(metatile->z) - owner->z.fixed + toFIXED(8);
	Sint32			heading = (Uint16)syAtan(dz, dx);
	EFFECT_SPR	   *sprite;
	BURST_PARTICLE *particle;
	Sint32			resource;
	Sint32			vram;
	Sint32			i;

	state->frm = 0;
	state->active = 1;
	syDecompressTaggedLZ(syResourceGetTableEntry(RESOURCE_LEAVES), state);
	resource = syVRAMCacheFindFreeEntry();
	vram = syVRAMCacheLoad(resource, 384, state);
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
		syVectorAddPolarOffset(
			syRandom() * 8 + toFIXED(8), syRandom(), &particle->x);
		particle->heading = heading;
		particle->speed = syRandom() + toFIXED(1);
		particle->spin = (syRandom() >> 6) + 384;
		if(i & 1)
			particle->spin = -particle->spin;
		particle->delay = -i / 2;
	}
	sySndPlayStub(SE_SHIMMER);
	sySetEvent(fdParticleBurstUpdate, EVENT_PASS_FRAME);
}

#undef MAD_PLANT_LEAP_FRAMES
#undef CYCLONE_ARC_PIXELS
#undef CYCLONE_ARC_PIXEL_STEP

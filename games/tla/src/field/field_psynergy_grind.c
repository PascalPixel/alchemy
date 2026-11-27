/* Grind: the screen shakes up and down while spray rises round the target, a
   sea rock is ground away, and the plain sway Grind makes at nothing. Part of
   field_psynergy.c. */

/* The splashes of spray Grind keeps. */
#define GRIND_SPLASH_COUNT		 64
/* The largest picture Grind grinds down, in bytes, a byte a pixel. */
#define GRIND_PICTURE_SIZE		 2048
/* The tiles of each frame of the splash picture (RESOURCE_WATER_SPLASH), 16 by
   16 pixels in 256 colours. */
#define GRIND_SPLASH_FRAME_TILES SPRITE_TILES_8BPP(16, 16)
/* How far in front of or behind the target the spray is drawn. */
#define GRIND_SPRAY_DEPTH		 15

/* Grind's state (heap slot HEAP_SLOT_BATTLE_EFFECT): the splash picture and the
   splash particles. */
typedef struct GRIND_STATE {
	Uint8					unused[32]; /* no code names it */
	Uint8					picture[GRIND_PICTURE_SIZE];
	FIELD_PSYNERGY_PARTICLE particles[GRIND_SPLASH_COUNT];
	EFFECT_SPR				sprs[GRIND_SPLASH_COUNT];
} GRIND_STATE;

/* What Grind does. */
typedef enum GRIND_MODE {
	GRIND_ON_TARGET =
		/* the target's Grind script runs as the shaking peaks */
	0,
	GRIND_SEA_ROCK = 1,	 /* a sea rock is ground down row by row */
	GRIND_ON_NOTHING = 2 /* spray rises before the caster */
} GRIND_MODE;

/* Grind ("Pulverize large objects."): the screen shakes harder and harder while
   spray rises round the target, or round the spot 32 pixels before the caster.
   A sea rock (SPRITE_SEA_ROCK) is ground down row by row; otherwise the place's
   Grind script runs while the shaking is at its height. */
void fdPsynergyGrindOnTarget(void)
{
	GRIND_STATE *state =
		syHeapSlotAlloc(HEAP_SLOT_BATTLE_EFFECT, sizeof(GRIND_STATE));
	FIELD_EFFECT_STATE		*scene = Ram_HeapSlots->fieldEffState;
	ACTOR					*caster = scene->caster;
	ACTOR					*child = scene->tgt;
	SPR						*anm = child->spr;
	EFFECT_SPR				*spr;
	FIELD_PSYNERGY_PARTICLE *particle;
	void					*src;
	Uint8					*dst;
	Sint32					 ang;
	Sint32					 i;
	Sint32					 entry;
	Sint32					 tile;
	Sint32					 phase;
	Sint32					 tmr;
	Sint32					 shifts;
	Sint32					 animating;
	Sint32					 spraying;
	Sint32					 next;
	Sint32					 isSeaRock;
	Sint32					 mode;
	FIXED					 radius;
	Sint32					 selector;
	CUE						*pending;
	Uint8					*picture;
	Sint32					 counter;
	FIXED					 amplitude;

	shifts = 0;
	radius = toFIXED(14);
	animating = 0;
	spraying = 0;
	next = 0;
	mode = GRIND_ON_TARGET;
	selector = fdSelectPsynergyTarget(scene->fieldPsynergy);
	pending = fdFindPsynergyCue(
		PSYNERGY_CUE(PSYNERGY_PHASE_ACTS), scene->fieldPsynergy, selector);
	if(child != NULL) {
		if(anm != NULL) {
			if(anm->width * anm->height <= GRIND_PICTURE_SIZE) {
			} else
				goto end;
		}
	} else {
		scene->x = caster->x.fixed;
		scene->z = caster->z.fixed + toFIXED(8);
		scene->y = caster->y.fixed;
		syVectorAddPolarOffset(toFIXED(32), scene->ang, &scene->x);
	}
	isSeaRock = 0;
	if(fdActorGetSprIdStub(child) == SPRITE_SEA_ROCK) {
		isSeaRock = 1;
		mode = GRIND_SEA_ROCK;
	}
	if(child == NULL)
		mode = GRIND_ON_NOTHING;
	if(mode == GRIND_SEA_ROCK)
		radius = toFIXED(14);
	if(mode == GRIND_ON_NOTHING)
		radius = toFIXED(21);

	fdPsynergyFadeToCastCol();
	src = syResourceGetTableEntry(RESOURCE_WATER_SPLASH);
	picture = state->picture;
	syDecompressTaggedLZ(src, picture);
	entry = syVRAMCacheFindFreeEntry();
	tile = syVRAMCacheLoad(entry, 1024, picture);
	particle = state->particles;
	for(i = 0; i < GRIND_SPLASH_COUNT; i++) {
		spr = &state->sprs[i];
		fdEffectSprInit(spr, 8, 8, OBJ_SIZE_16X16, tile);
		spr->oam.fullCol = 1;
		spr->oam.pal = 0;
		spr->oam.pri = 0;
		spr->depth = OAM_DEPTH_WINDOW;
		particle->tmr = -1;
		particle++;
	}
	if(child != NULL && anm != NULL && isSeaRock == 1) {
		fdSprSetAnimationSpeedStub(anm, 0);
		fdPsynergyDecodePicture((PICTURE_SOURCE *)anm->entries[0], picture,
			anm->width * anm->height);
	}
	fdViewMoveStart(VIEW_MOVE_Z);
	sySynch(1);
	sySndPlayStub(SE_PUSH_STEP);
	counter = 0;
	amplitude = toFIXED(2);
	phase = SWAY_GROW;
	tmr = 0;
	do {
		if(spraying) {
			if(mode == GRIND_SEA_ROCK) {
				if(radius > toFIXED(5))
					radius += -(toFIXED(1) / 25);
			} else if(radius > toFIXED(5)) {
				radius += -(toFIXED(2) / 25);
			}
		}
		for(i = 0; i < spraying; i++) {
			ang = syRandom();
			if(mode == GRIND_ON_NOTHING)
				ang = ((syRandom() * 10) >> 4) - TRIG_TURN / 16;
			spr = &state->sprs[next];
			particle = &state->particles[next];
			if(child != NULL) {
				spr->oam.pri = fdActorGetSprPriority(child);
				if(ang < TRIG_TURN / 2)
					spr->depth = fdActorGetSprDepth(child) + GRIND_SPRAY_DEPTH;
				else
					spr->depth = fdActorGetSprDepth(child) - GRIND_SPRAY_DEPTH;
			}
			particle->tmr = 0;
			SET_POSITION(particle, scene->x, scene->y + toFIXED(5), scene->z);
			if(radius > toFIXED(5))
				syVectorAddPolarOffset(
					radius + syRandom() * 5, ang, &particle->x);
			else
				syVectorAddPolarOffset(
					radius - (syRandom() << 2), ang, &particle->x);
			next++;
			next %= GRIND_SPLASH_COUNT;
		}
		particle = state->particles;
		for(i = 0; i < GRIND_SPLASH_COUNT; i++, particle++) {
			spr = &state->sprs[i];
			if((Uint32)particle->tmr <= 23) {
				if(particle->tmr == 0)
					spr->oam.idx = tile;
				if(particle->tmr == 18)
					spr->oam.idx = tile + GRIND_SPLASH_FRAME_TILES;
				if(particle->tmr == 20)
					spr->oam.idx = tile + 2 * GRIND_SPLASH_FRAME_TILES;
				if(particle->tmr == 22)
					spr->oam.idx = tile + 3 * GRIND_SPLASH_FRAME_TILES;
				fdEffectSprDrawAtWorldPos(spr, particle);
				particle->tmr++;
			}
		}
		if(child != NULL && anm != NULL && animating) {
			Sint32 size = anm->width * anm->height;
			dst = (Uint8 *)OBJ_VRAM0 +
				syVRAMCacheLoad(anm->slot, size, NULL) * TILE_SIZE_4BPP;
			if(shifts % 3 == 0)
				fdBattleEffectShiftRowsDown(picture, anm->width, anm->height);
			shifts++;
			((void (*)(
				void *, Sint32, Sint32, void *))Ram_HeapSlots->pictureUploader)(
				picture, anm->width, anm->height, dst);
		}
		fdViewMoveSwing(VIEW_MOVE_Z, counter++, amplitude);
		switch(phase) {
		case SWAY_GROW:
			if(tmr == 40 && mode != GRIND_ON_TARGET)
				spraying = 1;
			amplitude += toFIXED(1) / 10;
			if(amplitude > toFIXED(12) - 1) {
				phase++;
				tmr = -1;
			}
			break;
		case SWAY_HOLD:
			if(isSeaRock == 0) {
				if(tmr == 50) {
					phase = SWAY_SHRINK;
					tmr = -1;
				}
				if(pending != NULL)
					((void (*)(Sint32, Sint32, Sint32))pending->value)(
						0, tmr, selector);
			} else {
				if(tmr == anm->height * 3) {
					phase = SWAY_SHRINK;
					tmr = -1;
				}
				animating = 1;
			}
			break;
		case SWAY_SHRINK:
			if(isSeaRock == 1)
				animating = 0;
			if(tmr == 5 && mode != GRIND_ON_TARGET)
				spraying = 0;
			amplitude += -(toFIXED(3) / 20);
			if(amplitude <= toFIXED(2)) {
				phase++;
				tmr = -1;
			}
			break;
		case SWAY_SETTLE:
			amplitude = 0;
			if(tmr == 0)
				phase = SWAY_DONE;
			break;
		}
		sySynch(1);
		tmr++;
	} while(phase != SWAY_DONE);
	sySndPlayStub(SOUND_FADE_OUT_ACTION_SE);
	fdViewMoveEnd();
	fdPsynergyFadeFromCastCol();
	syVRAMCacheFreeEntry(entry);
end:
	syHeapSlotRelease(HEAP_SLOT_BATTLE_EFFECT);
}

/* FIELD_PSYNERGY_GRIND as fdPsynergyRun casts it: the view sways. */

/* Sways the view up and down the screen: the swing grows from SWAY_START to
   SWAY_PEAK, holds for SWAY_HOLD_TICKS, shrinks back and stops; then the map's
   view bounds come back. */
void fdPsynergyGrind(void)
{
	FIELD_EFFECT_STATE *scene;
	Sint32				phase;
	Sint32				amplitude;
	Sint32				state;
	Sint32				ticks;

	/* Grind's state's size, though the sway holds only the view move. */
	syHeapSlotAlloc(HEAP_SLOT_BATTLE_EFFECT, sizeof(GRIND_STATE));
	scene = Ram_HeapSlots->fieldEffState;
	fdPsynergyFadeToCastCol();
	fdBattleEffectSetObjectUpdateMode(scene->caster, CASTER_LOOK_NORMAL);
	fdViewMoveStart(VIEW_MOVE_Z);
	sySynch(10);

	phase = 0;
	amplitude = SWAY_START;
	state = SWAY_GROW;
	ticks = 0;
	do {
		fdViewMoveSwing(VIEW_MOVE_Z, phase++, amplitude);
		switch(state) {
		case SWAY_GROW:
			amplitude += SWAY_RAMP;
			if(amplitude >= SWAY_PEAK) {
				state = SWAY_HOLD;
				ticks = -1;
			}
			break;
		case SWAY_HOLD:
			if(ticks == SWAY_HOLD_TICKS) {
				state = SWAY_SHRINK;
				ticks = -1;
			}
			break;
		case SWAY_SHRINK:
			amplitude -= SWAY_RAMP;
			if(amplitude <= SWAY_START) {
				state = SWAY_SETTLE;
				ticks = -1;
			}
			break;
		case SWAY_SETTLE:
			amplitude = 0;
			if(ticks == 0)
				state = SWAY_DONE;
			break;
		}
		sySynch(1);
		ticks++;
	} while(state != SWAY_DONE);

	fdViewMoveEnd();
	fdPsynergyFadeFromCastCol();
	syHeapSlotRelease(HEAP_SLOT_BATTLE_EFFECT);
}

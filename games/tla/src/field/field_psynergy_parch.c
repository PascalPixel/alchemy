/* Parch, with the canvas pixels and clipped 16 by 16 BG map copies it draws.
   Part of field_psynergy.c. */

/* Parch cast on a target: runs fdPsynergyParch. */
void fdPsynergyParchOnTarget(void)
{
	fdPsynergyParch();
}

/* A puff of Parch's steam: its place, its sway and rise, and its age. */
typedef struct PARCH_PUFF {
	Sint32 x;
	Sint32 y;
	Sint32 z;
	Sint32 sway;
	Sint32 rise;
	Sint32 swayRate;
	Sint32 tmr;
} PARCH_PUFF;

/* The puffs of steam Parch keeps, a power of two: the next wraps by a mask. */
#define PARCH_PUFF_COUNT	   64
/* The disc's tiles, this many tiles after the window sprites' glyph tiles. */
#define PARCH_DISC_TILE_OFFSET 128
/* The disc's colours, the last 32 of the sprite palette, which glow as the
   Ring's do (GLOW_RED and the rest); the white flash of drying water raises
   them all. */
#define PARCH_GLOW_COLORS	   32

/* What Parch finds before the caster. */
typedef enum PARCH_RESULT {
	PARCH_NOTHING = 0, /* no water: the disc swells and shrinks */
	PARCH_WATER = 1,   /* water with no Parch script: steam rises from it */
	/* A puddle, or water with a Parch script: it dries up in a white flash. */
	PARCH_DRIES = 2
} PARCH_RESULT;

/* The stages of Parch's disc, which differ by what it finds (PARCH_RESULT).
   Every result starts with PARCH_SWELLS. */
typedef enum PARCH_PHASE {
	/* the disc grows, to three quarters (a quarter on nothing) */
	PARCH_SWELLS = 0,
	/* On nothing: the disc holds for 30 frames, then shrinks away. */
	PARCH_HOLDS = 1,
	PARCH_EMPTY_SHRINKS = 2,
	/* On water: steam rises two puffs a frame, then one, and the disc shrinks
	   away. Water that dries flashes white 40 frames into the steam, and the
	   cast ends with the thinner steam. */
	PARCH_STEAMS = 1,
	PARCH_STEAM_THINS = 2,
	PARCH_STEAM_SHRINKS = 3
} PARCH_PHASE;

/* Parch's state (heap slot HEAP_SLOT_BATTLE_EFFECT): the glow disc over the
   target and the 64 puffs of steam that rise as it dries. */
typedef struct PARCH_STATE {
	Uint8	   tiles[4096];
	EFFECT_SPR disc;
	Uint8	   unusedAfterDisc[40]; /* no code names it */
	Sint32	   discX;
	Sint32	   discY;
	Sint32	   discScaleX;
	Sint32	   discScaleY;
	EFFECT_SPR sprs[PARCH_PUFF_COUNT];
	PARCH_PUFF particles[PARCH_PUFF_COUNT];
} PARCH_STATE;

void fdBattleEffectFadeToSavedPalettes(Sint32 frames);

/* Parch ("Evaporate standing water."): a glowing disc spreads 32 pixels before
   the caster and white puffs rise from it. On a puddle (SPRITE_PUDDLE), or on
   water with a Parch script, the water there dries up and the script runs. */
void fdPsynergyParch(void)
{
	PARCH_STATE *state =
		syHeapSlotAlloc(HEAP_SLOT_BATTLE_EFFECT, sizeof(PARCH_STATE));
	HEAP_SLOTS		   *heap;
	FIELD_EFFECT_STATE *scene = Ram_HeapSlots->fieldEffState;
	WIN_STATE		   *windowState = (WIN_STATE *)Ram_HeapSlots->winState;
	Sint32				offset; /* into the field state's actor list */
	ACTOR			   *caster;
	ACTOR			   *actor;
	SPR				   *actorSprite;
	EFFECT_SPR		   *sprite;
	EFFECT_SPR		   *fresh;
	PARCH_PUFF		   *particle;
	FIXED				screen[3];
	FIXED				pos[3];
	Sint32				phase;
	Sint32				timer;
	Sint32				frame;
	Sint32				entry;
	Sint32				tile;
	Sint32				next;
	Sint32				spawn;
	Sint32				fade;
	Sint32				result;
	Sint32				found;
	Sint32				kind;
	void			   *pending;
	Sint32				scaleY;
	Sint32				scaleX;
	Sint32				y;
	Sint32				x;
	Sint32				red;
	Sint32				green;
	Sint32				blue;
	Sint32				add;
	Sint32				packed;
	Sint32				i;
	Uint8			   *dst;

	caster = scene->caster;
	found = -1;
	fade = -1;
	pending = NULL;
	fdPsynergyFadeToCastCol();
	pos[0] = caster->x.fixed;
	pos[1] = caster->y.fixed;
	pos[2] = caster->z.fixed;
	syVectorAddPolarOffset(toFIXED(32), scene->ang, pos);

	i = 0;
	kind = SPRITE_PUDDLE;
	heap = Ram_HeapSlots;
	for(offset = offsetof(FIELD_STATE, actors); i < FIELD_ACTOR_COUNT;
		i++, offset += sizeof(ACTOR *)) {
		actor = *(ACTOR **)((Uint8 *)heap->fieldState + offset);
		actorSprite = actor->spr;
		if(actor->script != NULL && actorSprite->entries[0]->anmId == kind &&
			actorSprite->lastNo == PUDDLE_ANIMATION_PUDDLE &&
			fdPsynergyCheckOverlap(
				pos, 4, &actor->x.fixed, actor->radius - 2) >= 0) {
			result = PARCH_DRIES;
			found = i;
			fdActorGetById(i)->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
			goto loaded;
		}
	}
	if(fdMeasurePsynergyHeight(scene->fieldPsynergy, caster, pos) == -1) {
		result = PARCH_NOTHING;
		/* DEAD: pending is already NULL; the ROM keeps the second store. */
		pending = NULL;
	} else if(fdMapGridIsWaterMetatile(pos[0], pos[2], caster->layer) != 0) {
		pending = fdFindPsynergyCue(PSYNERGY_CUE(PSYNERGY_PHASE_ACTS),
			scene->fieldPsynergy,
			fdMapGridGetMetatileBehavior(pos[0], pos[2], caster->layer));
		result = PARCH_DRIES;
		if(pending == NULL)
			result = PARCH_WATER;
	} else {
		result = PARCH_NOTHING;
	}
loaded: {
	void *source = syResourceGetTableEntry(RESOURCE_GLOW_DISC);
	dst = (Uint8 *)OBJ_VRAM0 + PARCH_DISC_TILE_OFFSET * TILE_SIZE_4BPP +
		windowState->glyphTiles * TILE_SIZE_4BPP;
	syDecompressTaggedLZ(source, state);
}
	syDMAXCopy(state, dst, DMA_COPY_WORDS(1024), REG_DMA3);
	screen[0] = pos[0];
	screen[1] = pos[1];
	screen[2] = pos[2];
	fdCameraWorldToScreen(screen);
	state->discX = screen[0];
	state->discY = screen[2];
	state->discScaleX = 0;
	state->discScaleY = 0;
	syDecompressTaggedLZ(syResourceGetTableEntry(RESOURCE_WHITE_PUFF), state);
	entry = syVRAMCacheFindFreeEntry();
	tile = syVRAMCacheLoad(entry, 2048, state);
	particle = state->particles;
	for(i = 0; i < PARCH_PUFF_COUNT; i++) {
		fresh = &state->sprs[i];
		fdEffectSprInit(fresh, 8, 15, OBJ_SIZE_16X16, tile);
		fresh->depth = EFFECT_SPRITE_FRONT_DEPTH;
		fresh->oam.pri = 0;
		fresh->oam.fullCol = 1;
		fresh->oam.pal = 0;
		particle->tmr = -1;
		particle++;
	}
	sySndPlayStub(SE_PSYNERGY_START);
	sySynch(1);
	next = 0;
	spawn = 0;
	phase = PARCH_SWELLS;
	timer = 0;
	frame = 0;
	do {
		for(i = spawn; i != 0; i--) {
			Sint32 slot = next & (PARCH_PUFF_COUNT - 1);
			sprite = &state->sprs[slot];
			sprite->scaleX = toFIXED(1) / 5;
			sprite->scaleY = toFIXED(1) / 5;
			particle = &state->particles[slot];
			SET_POSITION(particle, pos[0], pos[1], pos[2]);
			syVectorAddPolarOffset(
				syRandom() * 12 + toFIXED(9), syRandom(), &particle->x);
			particle->sway = syRandomBelow(32);
			particle->swayRate = syRandomBelow(512) + 256;
			particle->rise = syRandom() + toFIXED(1) / 5;
			particle->tmr = 0;
			next = (next + 1) & (PARCH_PUFF_COUNT - 1);
		}
		for(i = 0, particle = state->particles; i < PARCH_PUFF_COUNT;
			i++, particle++) {
			if(particle->tmr >= 0) {
				Sint32 grown;

				sprite = &state->sprs[i];
				sprite->oam.idx = tile + ((particle->tmr >> 2) & 7) * 8;
				grown = sprite->scaleX + toFIXED(1) / 10;
				/* DEAD: stored again after the clamp; the ROM keeps both
				   stores. */
				sprite->scaleX = grown;
				if(grown > toFIXED(1))
					grown = toFIXED(1);
				sprite->scaleX = grown;
				sprite->scaleY = grown;
				screen[0] = particle->x;
				screen[1] = particle->y;
				screen[2] = particle->z;
				fdCameraWorldToScreen(screen);
				sprite->x = screen[0] +
					(sySin(particle->sway * particle->swayRate) << 3);
				sprite->y = screen[2];
				fdEffectSprDraw(sprite);
				particle->sway++;
				particle->y += particle->rise;
				if(++particle->tmr == 32)
					particle->tmr = -1;
			}
		}

		for(i = 0; i < PARCH_GLOW_COLORS; i++) {
			if(fade == -1)
				add = 0;
			else
				add = fade / 4;
			blue = sySin((frame / 2 + i) * (TRIG_TURN / 32)); /* green's sine */
			green = FIXEDtoINT(blue * GLOW_GREEN_SWING) + GLOW_GREEN;
			blue =
				sySin((frame / 4 + i) * (TRIG_TURN / 32) + TRIG_TURN * 9 / 32);
			blue = FIXEDtoINT(blue * GLOW_BLUE_SWING) + GLOW_BLUE;
			red = add + GLOW_RED;
			if(red > RGB_LEVEL_MAX)
				red = RGB_LEVEL_MAX;
			green += add;
			if(green > RGB_LEVEL_MAX)
				green = RGB_LEVEL_MAX;
			blue += add;
			if(blue > RGB_LEVEL_MAX)
				blue = RGB_LEVEL_MAX;
			syIOWrite16(
				RGB(red, green, blue), &OBJ_PLTT[OBJ_PLTT_SIZE / 2 - 1 - i]);
		}
		if(fade != -1 && fade <= 126)
			fade++;

		x = state->discX;
		y = state->discY;
		scaleX = state->discScaleX;
		scaleY = state->discScaleY;
		packed = windowState->glyphTiles + PARCH_DISC_TILE_OFFSET;
		sprite = &state->disc;
		fdEffectSprInit(sprite, 31, 31, OBJ_SIZE_64X64, packed);
		sprite->oam.fullCol = 1;
		sprite->oam.pal = 0;
		sprite->oam.pri =
			fdPsynergyGetCanvasPriority(scene->caster, scene->ang);
		sprite->depth = 1;
		sprite->x = x;
		sprite->y = y;
		sprite->scaleX = scaleX;
		sprite->scaleY = scaleY;
		fdEffectSprDraw(sprite);

		if(result == PARCH_NOTHING) {
			switch(phase) {
			case PARCH_SWELLS:
				if(timer == 10)
					sySndPlayStub(SE_PARCH);
				state->discScaleX += toFIXED(3) / 256;
				state->discScaleY = state->discScaleX;
				if(state->discScaleX > toFIXED(0.25) - 1) {
					phase = PARCH_HOLDS;
					timer = -1;
				}
				break;
			case PARCH_HOLDS:
				if(timer == 30) {
					phase = PARCH_EMPTY_SHRINKS;
					timer = -1;
				}
				break;
			case PARCH_EMPTY_SHRINKS:
				spawn = 0;
				state->discScaleX += -(toFIXED(3) / 256);
				state->discScaleY = state->discScaleX;
				if(state->discScaleX <= 0)
					phase = FIELD_PSYNERGY_FINISHED;
				break;
			}
		}
		if(result == PARCH_WATER) {
			switch(phase) {
			case PARCH_SWELLS:
				if(timer == 30)
					sySndPlayStub(SE_SPIN);
				state->discScaleX += toFIXED(3) / 256;
				state->discScaleY = state->discScaleX;
				if(state->discScaleX > toFIXED(0.75) - 1) {
					phase = PARCH_STEAMS;
					timer = -1;
				}
				break;
			case PARCH_STEAMS:
				spawn = 2;
				if(timer == 40) {
					phase = PARCH_STEAM_THINS;
					timer = -1;
				}
				break;
			case PARCH_STEAM_THINS:
				spawn = 1;
				if(timer == 40) {
					phase = PARCH_STEAM_SHRINKS;
					timer = -1;
				}
				break;
			case PARCH_STEAM_SHRINKS:
				spawn = 0;
				state->discScaleX += -(toFIXED(1) / 64);
				state->discScaleY = state->discScaleX;
				if(state->discScaleX <= 0)
					phase = FIELD_PSYNERGY_FINISHED;
				break;
			}
		}
		if(result == PARCH_DRIES) {
			switch(phase) {
			case PARCH_SWELLS:
				if(timer == 40)
					sySndPlayStub(SE_SPIN);
				state->discScaleX += toFIXED(3) / 256;
				state->discScaleY = state->discScaleX;
				if(state->discScaleX > toFIXED(0.75) - 1) {
					phase = PARCH_STEAMS;
					timer = -1;
				}
				break;
			case PARCH_STEAMS:
				if(timer == 40) {
					fade = 0;
					fdPaletteFadeSetTarget(RGB_WHITE, 0);
					fdPaletteFadeStartInterpolation(100);
				}
				spawn = 2;
				if(timer == 100) {
					phase = PARCH_STEAM_THINS;
					timer = -1;
				}
				break;
			case PARCH_STEAM_THINS:
				spawn = 1;
				if(timer == 80)
					phase = FIELD_PSYNERGY_FINISHED;
				break;
			}
		}
		sySynch(1);
		timer++;
		frame++;
	} while(phase != FIELD_PSYNERGY_FINISHED);

	if(found != -1) {
		ACTOR *marked = fdActorGetById(found);
		fdMapGridSetMetatileBehavior(
			marked->x.fixed, marked->z.fixed, marked->layer, 0);
		fdActorSetHorizontalPosWithTerrain(found, 0, 0);
	}
	if(pending != NULL)
		fdRunPsynergyScript(pending, scene->casterId, scene->tgtId);
	syVRAMCacheFreeEntry(entry);
	if(result == PARCH_DRIES)
		fdBattleEffectFadeToSavedPalettes(100);
	else
		fdPsynergyFadeFromCastCol();
	syHeapSlotRelease(HEAP_SLOT_BATTLE_EFFECT);
}

void fdBattleEffectPlotCanvasPixel(Sint32 x, Sint32 y, Sint32 color);

/* Plots one pixel of the 128x64 canvas on a circle around its bottom middle
   (63, 63): a radius (16.16) and an angle, the cosine leftward and the sine
   upward. */
void fdCanvasPlotPolarPixel(
	Sint32 unused, FIXED radius, Sint32 angle, Sint32 color)
{
	fdBattleEffectPlotCanvasPixel(
		63 - FIXEDtoINT(syMulFX(syCos(angle), radius)),
		63 - FIXEDtoINT(syMulFX(sySin(angle), radius)), color);
}

extern const Uint16 Parch_PictureTiles[];

/* The picture fdBgMapDrawClipped16x16 draws, 16 tiles square. */
#define CLIPPED_PICTURE_TILES 16

/* Copies Parch's 16x16-tile picture (Parch_PictureTiles) to BG0's map, leaving
   out the tiles that would fall outside the 30x20-tile screen at (x, y). */
void fdBgMapDrawClipped16x16(Sint32 x, Sint32 y)
{
	const Uint16 *tiles = Parch_PictureTiles;
	Sint32		  row;
	Sint32		  column;
	Sint32		  sx;
	Sint32		  sy;

	x /= 8;
	y /= 8;
	IWRAM_FILL_WORDS((void *)BG_SCREEN_BLOCK(4), 1024, 0);
	for(row = 0, sy = y; row < CLIPPED_PICTURE_TILES; row++, sy++) {
		if(sy < 0)
			continue;
		if(sy > DISPLAY_TILE_HEIGHT - 1)
			break;
		for(sx = x, column = 0; column < CLIPPED_PICTURE_TILES;
			column++, sx++) {
			if(sx < 0)
				continue;
			if(sx > DISPLAY_TILE_WIDTH - 1)
				break;
			((Uint16 *)BG_SCREEN_BLOCK(4))[row * BG_SCREEN_WIDTH + column] =
				tiles[row * CLIPPED_PICTURE_TILES + column];
		}
	}
}

#undef PARCH_DISC_TILE_OFFSET
#undef PARCH_GLOW_COLORS
#undef CLIPPED_PICTURE_TILES

// Gaia, and the effect sprites with tiles of their own. Part of battle_effect.c.

#define GAIA_TICKS             192 // ticks of Gaia
// The shaft of light widens into the wall a row every three ticks from GAIA_WALL_WIDEN_TICK, stands
// once it has risen, and sinks away a row every three ticks.
#define GAIA_WALL_WIDEN_TICK   64
#define GAIA_WALL_STAND_TICK   88
#define GAIA_WALL_SINK_TICK    160

#define GAIA_ROCKS             11 // rock sprites tumbling up through the wall
// The rocks that fly from the start; the rest join them from tick 72.
#define GAIA_EARLY_ROCKS       6
// The rocks' motion is state->particles[GAIA_FIRST_ROCK] on.
#define GAIA_FIRST_ROCK        10
// The columns GaiaLateRockColumns gives the rocks after the early ones.
#define GAIA_LATE_ROCK_COLUMNS 6
// A rock waits this many ticks longer than the rock after it before it first flies.
#define GAIA_ROCK_STAGGER      4
#define GAIA_DUST              256 // dust in PARTICLES
// The dust drawn: the rest is set out and never shown.
#define GAIA_DUST_SHOWN        64

// The pictures Gaia rebuilds from the earth wall sprite sheet: the rising wall's rows, the standing
// wall's top and foot, then the flames.
#define GAIA_ROW_HEIGHT        32
// The rising wall's rows, narrowest first, and the flames at its foot.
#define GAIA_ROW_PICTURES      9
#define GAIA_FLAME_PICTURES    3
// The standing wall's top, stretched from a 48x32 row, and its foot.
#define GAIA_WALL_TOP_WIDTH    48
#define GAIA_WALL_TOP_HEIGHT   96
#define GAIA_WALL_FOOT_WIDTH   48
#define GAIA_WALL_FOOT_HEIGHT  21

// Where each picture starts in EarthWallSheet, in bytes.
typedef enum EARTH_WALL_SHEET_PICTURE
{
	GAIA_WALL_TOP  = GAIA_ROW_HEIGHT * (5 + 6 + 9 + 12 + 15 + 18 + 21 + 32 + 40),
	GAIA_WALL_FOOT = GAIA_WALL_TOP + PICTURE_BYTES(GAIA_WALL_TOP)
} EARTH_WALL_SHEET_PICTURE;

// Flames flicker at the targets' feet under a narrow shaft of orange light, which widens into a
// glowing wall and stands while rocks tumble up through it and the targets are battered, then
// narrows away again.
//
// Abilities: Gaia, Mother Gaia, Grand Gaia, Rockfall, Rockslide, Avalanche.
void btEffectRunGaia(BATTLE_EFFECT_PARM *args)
{
	BATTLE_EFFECT_STATE *state;
	void                *canvas;
	Sint32               frame;
	BLITTER_PAIR         draw;
	Uint8               *sprites;
	Sint32               wallX;
	Sint32               rockX;
	SCALE                scale;
	FIXED                pos[4];
	Uint8               *src;
	Uint16              *color;
	void                *palette = (void *)BG_PLTT;
	Sint32               sheetIndex;
	Sint32               i;
	Sint32               j;
	Sint32               k;

	state   = RamHeapSlots->battleEffect;
	canvas  = RamHeapSlots->battleCanvas;
	sprites = RamHeapSlots->battleSheet;
	btEffectStartCanvasLayer(0);

	// The higher the variant, the wider the wall.
	if (args->variant == EFFECT_VARIANT_BASIC)
		REG_BG2PA = Q_8_8(0.8);
	else if (args->variant == EFFECT_VARIANT_STRONGER)
		REG_BG2PA = Q_8_8(1.0 / 1.5);
	if (args->side == EFFECT_SIDE_PARTY)
	{
		wallX = 8;
		if (args->variant == EFFECT_VARIANT_BASIC)
			rockX = 40;
		else if (args->variant == EFFECT_VARIANT_STRONGER)
			rockX = 36;
		else
			rockX = 40;
	}
	else
	{
		wallX = -16;
		rockX = -12;
	}
	REG_BG2X = GaiaBg2Shifts[args->side][args->variant] << 8;

	btResourceLoadAndDecompress(RESOURCE_EARTH_WALL_SHEET, RamMapMetatileBuffer, 1, 0);
	syWordsCopyEntry(
		palette, syResourceGetTableEntry(RESOURCE_ORANGE_PALETTE_A), SHEET_PALETTE_BYTES);
	color = (Uint16 *)&BG_PLTT_COLOR(0, 1);
	for (i = 0; i != SHEET_PALETTE_COLORS - 1; i++)
	{
		Sint32 c      = *color;
		Sint32 blue   = RGB_BLUE((Uint16)c);
		Sint32 green  = RGB_GREEN((Uint16)c);
		Sint32 red    = RGB_LEVEL_MAX;

		red          &= c;
		blue         -= 8;
		green        -= 8;
		red          -= 8;

		if (blue < 0)
			blue = 0;
		if (green < 0)
			green = 0;
		if (red < 0)
			red = 0;
		*color++ = RGB(red, green, blue);
	}

	src        = RamMapMetatileBuffer;
	sheetIndex = 0;
	for (j = 0; j != GAIA_ROW_PICTURES; j++)
	{
		for (i = 0; i != GAIA_ROW_HEIGHT; i++)
		{
			for (k = 0; k != GaiaRowWidths[j]; k++, sheetIndex++)
				state->sheet[sheetIndex] = src[k];
		}
		src += GaiaRowWidths[j];
	}
	for (j = 0; j != GAIA_ROW_HEIGHT; j++)
	{
		for (i = 0; i != GAIA_WALL_TOP_HEIGHT / GAIA_ROW_HEIGHT; i++)
		{
			for (k = 0; k != GAIA_WALL_TOP_WIDTH; k++, sheetIndex++)
				state->sheet[sheetIndex] = src[k];
		}
		src += GAIA_WALL_TOP_WIDTH;
	}
	for (i = 0; i != GAIA_WALL_FOOT_WIDTH * GAIA_WALL_FOOT_HEIGHT; i++)
		state->sheet[sheetIndex++] = *src++;
	for (i = 0; i != GAIA_FLAME_PICTURES; i++)
	{
		for (k = 0; k != GaiaFlameWidths[i] * GaiaFlameHeights[i]; k++)
			state->sheet[sheetIndex++] = *src++;
	}
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sprites, 0, 0);

	for (i = 0; i != GAIA_ROCKS; i++)
	{
		SPR *rock         = fdSpriteCreateStub(SPRITE_BATTLE_ROCKS);

		state->sprites[i] = rock;
		if (rock != 0)
		{
			rock->flags = 0;
			fdSpriteStartAnimationStub(rock, i / 4);
			((SPR *)state->sprites[i])->image.priority = 1;
		}
	}
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
	draw[0] = RamHeapSlots->ramCode;
	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
	draw[1]      = RamHeapSlots->ramCodePaired;
	REG_BLDCNT   = BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

	// Rocks wait their turn (variant counts the ticks) and tumble up through the wall, y counting
	// down from 128 by z a tick.
	for (i = 0; i != GAIA_ROCKS; i++)
	{
		PARTICLE *rock = &state->particles[GAIA_FIRST_ROCK + i];

		SET_POSITION(rock, (syRandom() & 15) + 88, 128, (syRandom() & 15) + 2);
		SET_VELOCITY(rock, i, 1, toFIXED(1) / 2);
		rock->variant = GAIA_ROCKS * GAIA_ROCK_STAGGER - i * GAIA_ROCK_STAGGER;
	}
	// DEAD: the sixth column lands on the particle after the last rock, which nothing reads; the
	// ROM keeps the store.
	for (i = 0; i != GAIA_LATE_ROCK_COLUMNS; i++)
		state->particles[GAIA_FIRST_ROCK + GAIA_EARLY_ROCKS + i].x = GaiaLateRockColumns[i];

	// Dust rises from the foot of the wall.
	for (i = 0; i != GAIA_DUST; i++)
	{
		PARTICLE *dust  = &PARTICLES[i];

		dust->x         = INTtoFIXED((syRandom() & 63) + wallX + 32);
		dust->y         = INTtoFIXED((syRandom() & 7) + 96);
		dust->velocityY = ((syRandom() & 63) + 32) << 13;
		dust->variant   = syRandom() & 31;
	}
	REG_BG2CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
	// The screen shakes throughout.
	state->shakeFrame = 250;
	for (frame = 0; frame != GAIA_TICKS; frame++)
	{
		if (frame == 0)
			sySoundPlayStub(SE_POWER_UP);
		if (frame == 40)
			sySoundPlayStub(SE_RUMBLE);
		if (frame == 96)
			sySoundPlayStub(SE_CRASH);
		if (frame == 120)
			EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);

		// The flames flicker at the wall's foot.
		if (frame <= 81)
		{
			Sint32 picture = frame / 4;

			if (picture > 2)
				picture = (picture & 1) + 1;
			draw[0](canvas,
			        state->sheet + GaiaFlamePictures[picture],
			        wallX - GaiaFlameWidths[picture] + 64,
			        116 - GaiaFlameHeights[picture],
			        GaiaFlameWidths[picture],
			        GaiaFlameHeights[picture]);
			draw[1](canvas,
			        state->sheet + GaiaFlamePictures[picture],
			        wallX + 64,
			        116 - GaiaFlameHeights[picture],
			        GaiaFlameWidths[picture],
			        GaiaFlameHeights[picture]);
		}

		// The rising wall widens a row at a time.
		if (frame >= 12 && frame < GAIA_WALL_STAND_TICK)
		{
			Sint32 row = (frame - GAIA_WALL_WIDEN_TICK) / 3;

			CLAMP(row, 0, 7);
			for (i = 0; i != 4; i++)
			{
				draw[0](canvas,
				        state->sheet + GaiaRowPictures[row],
				        wallX - GaiaRowWidths[row] + 64,
				        i * GAIA_ROW_HEIGHT - 12,
				        GaiaRowWidths[row],
				        GAIA_ROW_HEIGHT);
				draw[1](canvas,
				        state->sheet + GaiaRowPictures[row],
				        wallX + 64,
				        i * GAIA_ROW_HEIGHT - 12,
				        GaiaRowWidths[row],
				        GAIA_ROW_HEIGHT);
			}
		}

		// The wall sinks away.
		if (frame >= GAIA_WALL_SINK_TICK && frame < GAIA_WALL_SINK_TICK + 24)
		{
			Sint32 row = 7 - (frame - GAIA_WALL_SINK_TICK) / 3;

			CLAMP(row, 0, 7);
			for (i = 0; i != 4; i++)
			{
				draw[0](canvas,
				        state->sheet + GaiaRowPictures[row],
				        wallX - GaiaRowWidths[row] + 64,
				        i * GAIA_ROW_HEIGHT - 12,
				        GaiaRowWidths[row],
				        GAIA_ROW_HEIGHT);
				draw[1](canvas,
				        state->sheet + GaiaRowPictures[row],
				        wallX + 64,
				        i * GAIA_ROW_HEIGHT - 12,
				        GaiaRowWidths[row],
				        GAIA_ROW_HEIGHT);
			}
		}

		// The standing wall.
		if (frame >= GAIA_WALL_STAND_TICK && frame < GAIA_WALL_SINK_TICK)
		{
			draw[0](canvas,
			        &state->sheet[GAIA_WALL_TOP],
			        wallX + 16,
			        0,
			        GAIA_WALL_TOP_WIDTH,
			        GAIA_WALL_TOP_HEIGHT);
			draw[1](canvas,
			        &state->sheet[GAIA_WALL_TOP],
			        wallX + 64,
			        0,
			        GAIA_WALL_TOP_WIDTH,
			        GAIA_WALL_TOP_HEIGHT);
			draw[0](canvas,
			        &state->sheet[GAIA_WALL_FOOT],
			        wallX + 16,
			        GAIA_WALL_TOP_HEIGHT,
			        GAIA_WALL_FOOT_WIDTH,
			        GAIA_WALL_FOOT_HEIGHT);
			draw[1](canvas,
			        &state->sheet[GAIA_WALL_FOOT],
			        wallX + 64,
			        GAIA_WALL_TOP_HEIGHT,
			        GAIA_WALL_FOOT_WIDTH,
			        GAIA_WALL_FOOT_HEIGHT);
		}

		// Dust keeps rising up the wall.
		if (frame >= GAIA_WALL_STAND_TICK)
		{
			for (i = 0; i != GAIA_DUST_SHOWN; i++)
			{
				PARTICLE *dust = &PARTICLES[i];

				if (dust->variant == 0)
				{
					Sint32 size = (i & 3) + 5;

					draw[0](canvas,
					        sprites + BattleEffectMotePictures[size - 1],
					        FIXED_INTEGER_PART(dust->x) - size / 2,
					        FIXED_INTEGER_PART(dust->y) - size,
					        size,
					        size * 2);
					dust->y -= dust->velocityY;
					if (dust->y < 0 && frame < GAIA_WALL_SINK_TICK)
						dust->y = toFIXED(96);
				}
				else
				{
					dust->variant--;
				}
			}
		}

		// Rocks tumble up through the wall, larger with the variant.
		if (frame > 4)
		{
			Sint32 rocks = GAIA_EARLY_ROCKS;

			if (frame > 71)
				rocks = GAIA_ROCKS;
			for (k = 0; k != rocks; k++)
			{
				PARTICLE *rock = &state->particles[GAIA_FIRST_ROCK + k];
				Sint32    slot;

				if (rock->variant == 0)
				{
					// DEAD: scale copied from GaiaUnitScale before both its fields are set; the ROM
					// keeps the copy.
					scale = GaiaUnitScale;
					if (frame > 71)
					{
						scale.x  = k * (toFIXED(1) / 16) + toFIXED(1) / 2;
						scale.x += args->variant * (toFIXED(1) / 4);
					}
					else
					{
						scale.x = toFIXED(1) / 2;
					}
					scale.y = scale.x;
					pos[3]  = 0;
					pos[0]  = toFIXED(rock->x + rockX * 2);
					pos[1]  = toFIXED(512) - toFIXED(rock->y);
					pos[2]  = toFIXED(512);
					slot    = (frame / 2 + k) % GAIA_ROCKS;
					// DEAD: slot != -1 always holds, slot being a remainder of a positive count;
					// the ROM keeps the test.
					if (slot != -1)
						fdRenderApplyProjectedPlacementStub(state->sprites[slot], pos, &scale, 0);
					rock->y         -= rock->z;
					// DEAD: velocityX steps from 1 to 12 by velocityY and nothing reads it; the ROM
					// keeps the count.
					rock->velocityX += rock->velocityY;
					if (rock->velocityX > 12)
						rock->velocityX -= 12;
					if (rock->y < 0)
					{
						if (frame >= GAIA_WALL_SINK_TICK)
						{
							rock->variant = -1;
						}
						else
						{
							if (frame >= GAIA_WALL_STAND_TICK)
							{
								rock->z = (syRandom() & 7) + 8;
								if (args->variant == EFFECT_VARIANT_BASIC)
									rock->x = syRandom() % 96 + 42;
								else if (args->variant == EFFECT_VARIANT_STRONGER)
									rock->x = syRandom() % 112 + 34;
								else
									rock->x = syRandom() % 160 + 10;
							}
							rock->y       = 128;
							rock->variant = 8;
						}
					}
				}
				else
				{
					rock->variant--;
				}
			}
		}

		// The targets flash and recoil.
		if (frame <= 158)
		{
			EFFECT_FOR_EACH_TARGET(i, args)
			{
				if (frame > 85)
				{
					if (frame % 12 == 0)
						btEffectTintBattler(
							args->targets[i], SPRITE_COLOR_WHITE, POSE_DAMAGE, i, 6);
					if ((frame & 3) == 0)
						btMotionApplyKnockbackStub(args->targets[i], KNOCKBACK_SKID);
				}
			}
		}
		// While the wall stands the screen shakes hard, otherwise softly.
		if (frame < 90 || frame > GAIA_WALL_SINK_TICK)
			btCameraShake(2, 2);
		else
			btCameraShake(8, 8);
		EFFECT_END_TICK(state);
	}
	syCloseEvent(btEffectTransferCanvas);
	EFFECT_RELEASE_BLITTERS();
	for (i = 0; i != GAIA_ROCKS; i++)
		fdSpriteDestroyStub(state->sprites[i]);
	btEffectEndCanvasLayer();
}

// Takes the first free animation entry of the 64 and clears it for use. With none free it clears
// the entry at address 0.
SPR_ANIMATION *btAnimationObjectAllocateEmpty(void)
{
	SPR_ANIMATION *entry = RamHeapSlots->spriteAnimations;
	SPR_ANIMATION *found = NULL;
	Sint32         i     = 0;
	void          *none  = NULL;

	while (i < ANIM_OBJECT_COUNT)
	{
		if (entry->kind == 0)
		{
			found = entry;
			break;
		}
		i++;
		entry++;
	}
	found->animationId  = 0;
	found->scripts      = NULL;
	found->pictures     = NULL;
	found->script       = none;
	found->mode         = 0;
	found->picture      = 0;
	found->scriptOffset = 0;
	found->kind         = 1;
	found->colorMode    = 0;
	return found;
}

// A SPR as btEffectSpriteCreateWithTiles searches and fills it: its OAM attributes and its shadow's
// written as words.
typedef struct SPR_WORDS
{
	Sint32 link; // the link to the next part at its depth, as
	// ANIMATION_SPR_PART's
	Sint32 attributes;

	union
	{
		Uint32 word;

		struct
		{
			Uint32 tile : 10;
			Uint32 rest : 22;
		} bits;
	} oam;

	Sint32         scale;
	Uint8          slot;
	Uint8          retainCacheEntry : 1;
	Uint8          useOwnScale      : 1;
	Uint8          sunkRows         : 6;
	Uint16         rotation;
	Uint8          width;
	Uint8          height;
	Uint8          offsetX;
	Uint8          offsetY;
	Uint8          lastNo;
	Uint8          dirty;
	Uint8          flags;
	Uint8          count;
	// The shadow's link, attributes 0 and 1, and attribute 2.
	Sint32         shadowLink;
	Sint32         shadowAttributes;
	Sint32         shadowTile;
	SPR_ANIMATION *entry; // its first layer
	SPR_ANIMATION *otherEntries[3];
} SPR_WORDS;

// Makes an effect sprite with tiles of its own in object VRAM: a free VRAM cache entry and the
// first free effect sprite of the 64, as btEffectSpriteAcquire takes, set up for
// fdSpriteStartAnimationStub. The Lost Age's own.
void *btEffectSpriteCreateWithTiles(Sint32 width, Sint32 height, Sint32 attributes, Sint32 flags)
{
	SPR_WORDS          *found     = NULL;
	Sint32              vramEntry = syVRAMCacheFindFreeEntry();
	SPR_WORDS          *sprite    = RamHeapSlots->sprites;
	Sint32              i         = 0;
	ACTOR_SYSTEM_STATE *actorSystem;
	Sint32              size;
	Sint32              tile;
	VRAM_CACHE_ENTRY   *entries;

	while (i < ANIM_OBJECT_COUNT)
	{
		if (sprite->width == 0)
		{
			found = sprite;
			break;
		}
		i++;
		sprite++;
	}
	tile                    = syVRAMCacheLoad(vramEntry, 0, 0);
	found->slot             = vramEntry;
	found->rotation         = 0;
	found->flags            = 0;
	found->retainCacheEntry = 0;
	{
		Uint32 *words = (Uint32 *)found;

		*words++      = 0;
		*words++      = attributes;
		*words        = flags | tile | OBJ_PRIORITY(2);
	}
	{
		Sint32 *p = &found->shadowLink;

		*p        = 0;
	}
	// The shadow, as the field engine's sprites take it from the shared shadow tiles: the table
	// read as halfwords, the shadow entry's second its offset.
	found->shadowAttributes = OBJ_SIZE_16X8 | OBJ_256_COLORS;
	entries                 = VramCacheEntries;
	found->shadowTile =
		(((Uint16 *)entries)[VRAM_CACHE_ENTRY_ACTOR_SHADOW * 2 + 1] >> 5) | OBJ_PRIORITY(2);
	sprite->entry   = btAnimationObjectAllocateEmpty();
	sprite->width   = width;
	sprite->scale   = toFIXED(1);
	sprite->height  = height;
	sprite->offsetY = 0;
	sprite->offsetX = 0;
	sprite->count   = 1;
	sprite->flags   = 0;
	fdSpriteStartAnimationStub(sprite, 0);

	actorSystem = RamHeapSlots->actorSystem;
	size        = sprite->width * sprite->height;
	if (!(attributes & OBJ_256_COLORS))
		size = (Uint32)size >> 1;
	syVRAMCacheLoad(sprite->slot, size, 0);
	actorSystem->used     += size;
	sprite->oam.bits.tile  = entries[sprite->slot].offset >> 5;
	return found;
}

#undef GAIA_TICKS
#undef GAIA_WALL_WIDEN_TICK
#undef GAIA_WALL_STAND_TICK
#undef GAIA_WALL_SINK_TICK
#undef GAIA_ROCKS
#undef GAIA_EARLY_ROCKS
#undef GAIA_FIRST_ROCK
#undef GAIA_LATE_ROCK_COLUMNS
#undef GAIA_ROCK_STAGGER
#undef GAIA_DUST
#undef GAIA_DUST_SHOWN
#undef GAIA_ROW_HEIGHT
#undef GAIA_ROW_PICTURES
#undef GAIA_FLAME_PICTURES
#undef GAIA_WALL_TOP_WIDTH
#undef GAIA_WALL_TOP_HEIGHT
#undef GAIA_WALL_FOOT_WIDTH
#undef GAIA_WALL_FOOT_HEIGHT

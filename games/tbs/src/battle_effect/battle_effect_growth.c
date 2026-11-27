// Growth, Punji, Vine and Thorn. Part of battle_effect.c.

#define GROWTH_DUST                          512 // puffs of dust in PARTICLES
#define THORN_DUST                           64  // puffs of dust in PARTICLES for the thorns
// The ticks a puff of dust lasts: its seven pictures, two ticks each.
#define DUST_PUFF_TICKS                      14
// The columns set out, of which GrowthColumnCounts picks how many grow.
#define GROWING_COLUMNS                      16

// Growing columns and thorns spring up one after another, each shaking the screen and throwing up
// dust from its root, and the targets react a few ticks later.

// The dust pictures in the vine sprite sheet, after the vines.
#define GROWING_COLUMNS_VINE_DUST_PICTURES   7769
// The dust pictures in the pillar sprite sheet, after the pillars.
#define GROWING_COLUMNS_PILLAR_DUST_PICTURES 2815
// ThornSheet: the big thorn, then the small one.
#define THORN_BIG_WIDTH                      48
#define THORN_BIG_HEIGHT                     80
#define THORN_SMALL_PICTURE_WIDTH            32
#define THORN_SMALL_PICTURE_HEIGHT           64
#define THORN_SMALL_PICTURE                  PICTURE_BYTES(THORN_BIG)
// The pixels a big and a small thorn lean sideways at most as they grow.
#define THORN_BIG_LEAN                       30
#define THORN_SMALL_LEAN                     8

// What the growing columns grow (btEffectRunGrowingColumns).
typedef enum GROWING_COLUMNS_MODE
{
	GROWING_COLUMNS_VINES,  // Growth
	GROWING_COLUMNS_PILLARS // Punji's green pillars
} GROWING_COLUMNS_MODE;

// One growing column, laid over a PARTICLE in state->particles.
typedef struct GROWING_COLUMN
{
	Sint32 x;               // the root, in canvas pixels
	Sint32 y;               // the root's row: 108 to 115, lower for later columns
	Sint32 unused[2];
	Sint32 height;          // the full length, 55 to 118 capped by its shape
	Sint32 unusedVelocityZ; // the Particle's velocityZ, which the columns leave
	                        // alone
	// The tick it starts to grow; set, but the tick loop computes its own.
	Sint32 start;
} GROWING_COLUMN;

// Vines grow from the ground, scattering dust from each root.
//
// Abilities: Growth, Mad Growth, Wild Growth.
void btEffectRunGrowth(BATTLE_EFFECT_PARM *args)
{
	btEffectRunGrowingColumns(args, GROWING_COLUMNS_VINES);
}

// Green pillars grow from the ground, scattering dust from each root.
//
// Abilities: Punji, Punji Trap, Punji Strike.
void btEffectRunPunji(BATTLE_EFFECT_PARM *args)
{
	btEffectRunGrowingColumns(args, GROWING_COLUMNS_PILLARS);
}

// Vines or green pillars grow one after another, alternately mirrored, each throwing up dust from
// its root.
void btEffectRunGrowingColumns(BATTLE_EFFECT_PARM *args, Sint32 mode)
{
	BATTLE_EFFECT_STATE *state;
	void                *canvas;
	BLITTER_PAIR         rectangle;

	Sint32               frame;
	Sint32               duration;
	DRAW_RECTANGLE      *draw;
	Sint32               start;

	GROWING_COLUMN      *setup;
	Sint32               x;
	Sint32               height;
	Sint32               limit;

	Sint32               puffPictures;
	GROWING_COLUMN      *column;
	Sint32               length;
	Sint32               reach;
	Sint8                pillarHeight;

	Sint32               n; // a target that reacts, then a free slot for the dust
	Sint32               reactFrame;

	Sint32               i;
	PARTICLE            *puff;
	Sint32               puffAge;
	Sint32               image;

	state       = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	canvas      = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
	state->args = args;
	btEffectStartCanvasLayer(BGCNT_PRIORITY(1));
	REG_BG2PA  = Q_8_8(1); // the canvas at its own width
	REG_BLDCNT = 0;
	if (mode == GROWING_COLUMNS_PILLARS)
	{
		btResourceLoadAndDecompress(RESOURCE_GREEN_PILLAR_SHEET, state->sheet, 1, 1);
	}
	else
	{
		btResourceLoadAndDecompress(RESOURCE_VINE_SHEET, state->sheet, 1, 1);
	}
	if (state->args->side == EFFECT_SIDE_PARTY)
	{
		REG_BG2X = -(NARROW_CANVAS_PARTY_X << 8);
	}
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, COMPOSITE_TRANSPARENT);
	rectangle[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_TRANSPARENT);
	{
		DRAW_RECTANGLE mirroredBlitter = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);

		draw                           = rectangle;
		rectangle[1]                   = mirroredBlitter;
	}

	duration = GrowthColumnCounts[state->args->variant] * 4 + 56;

	for (i = 0; i != PARTICLE_POOL_COUNT; i++)
		PARTICLES[i].variant = -1;

	for (i = 0; i != GROWING_COLUMNS; i++)
	{
		setup         = &((GROWING_COLUMN *)state->particles)[i];
		x             = (GrowthColumnX[i] + (7 & syRandom())) - 4;
		setup->y      = i / 2 + 108;
		setup->x      = x;
		height        = (63 & syRandom()) + 55;
		setup->height = height;
		limit         = GrowthHeightLimits[i % 3];
		if (limit < height)
		{
			setup->height = limit;
		}
		limit        = i * 4 + 8;
		setup->start = limit;
	}

	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);

	for (frame = 0; frame != duration; frame++)
	{
		if (frame == duration - 64)
		{
			EFFECT_SHOW_DAMAGE(SE_WATER);
		}
		if (frame >= duration - 20 && frame < duration - 4)
		{
			// Fade out over the last 16 ticks.
			REG_BLDCNT   = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
			REG_BLDALPHA = EFFECT_BLEND_CANVAS((duration - frame) - 5);
		}
		if (frame < duration - 4)
		{
			for (i = 0; i != GrowthColumnCounts[state->args->variant]; i++)
			{
				column = &((GROWING_COLUMN *)state->particles)[i];
				start  = i * 4 + 8;
				if (frame == i * 4 + 9)
				{
					state->shakeFrame = 2;
				}

				// A column grows from its root.
				if (frame > start)
				{
					Uint32 shape = i % 3;

					length       = (frame - start) * 8;
					reach        = column->height;
					if (length > reach)
					{
						length = reach;
					}
					if (mode == GROWING_COLUMNS_VINES)
					{
						{
							Sint32 side    = 1 & i;
							Sint32 picture = GrowthVinePictures[shape];
							Uint32 width   = GrowthVineWidths[shape];
							draw[side](canvas,
							           state->sheet + picture,
							           column->x - (width >> 1),
							           column->y - length,
							           width,
							           length);
						}
					}
					else
					{
						pillarHeight = GrowthPillarHeights[7 & i];
						if (length > pillarHeight)
						{
							length = pillarHeight;
						}
						{
							Sint32 side    = 1 & i;
							Sint32 picture = GrowthPillarPictures[shape];
							Uint32 width   = GrowthPillarWidths[shape];
							draw[side](canvas,
							           state->sheet + picture,
							           column->x - (width >> 1),
							           column->y - length,
							           width,
							           length);
						}
					}
				}

				// The targets react as it grows.
				n = 0;
				if (state->args->targetCount != 0)
				{
					reactFrame = start + 4;
					for (; n != state->args->targetCount; n++)
					{
						if (frame == reactFrame)
						{
							if (!(i & 1))
							{
								sySoundPlayStub(SE_IMPACT);
							}
							btEffectTintBattler(
								state->args->targets[n], SPRITE_COLOR_WHITE, POSE_DAMAGE, n, 3);
						}
					}
				}
				else
				{
					reactFrame = start + 4;
				}

				// Dust puffs from the root.
				if ((frame == reactFrame) || (frame == (start + 8)))
				{
					for (n = 0; n != GROWTH_DUST; n++)
					{
						PARTICLE *dust = &PARTICLES[n];

						if (dust->variant == -1)
						{
							dust->x       = ((syRandom() & 0xf) + column->x) - 8;
							dust->y       = (syRandom() & 0xf) + 80;
							dust->variant = 0;
							break;
						}
					}
				}
			}
		}

		// Dust, over the columns.
		for (i = 0; i != GROWTH_DUST; i++)
		{
			puff    = &PARTICLES[i];
			puffAge = puff->variant;
			if (puffAge >= 0)
			{
				image        = puffAge / 2;
				puffPictures = GROWING_COLUMNS_VINE_DUST_PICTURES;
				if (mode != GROWING_COLUMNS_VINES)
				{
					puffPictures = GROWING_COLUMNS_PILLAR_DUST_PICTURES;
				}
				rectangle[0](canvas,
				             state->sheet + (GrowthPuffPictures[image] + puffPictures),
				             puff->x - GrowthPuffWidths[image],
				             puff->y - (Sint8)GrowthPuffHeights[image] / 2,
				             GrowthPuffWidths[image],
				             (Sint8)GrowthPuffHeights[image]);
				rectangle[1](canvas,
				             state->sheet + (GrowthPuffPictures[image] + puffPictures),
				             puff->x,
				             puff->y - (Sint8)GrowthPuffHeights[image] / 2,
				             GrowthPuffWidths[image],
				             (Sint8)GrowthPuffHeights[image]);
				puff->variant++;
				if (puff->variant == DUST_PUFF_TICKS)
				{
					puff->variant = -1;
				}
			}
		}

		btCameraShake(4, 4);
		EFFECT_END_TICK(state);
	}
	EFFECT_FINISH_CANVAS();
}

#define VINE_COLUMNS        16 // columns that rise
#define VINE_TICKS          70
// GreenVineSheet: five 32 by 32 pictures of a vine, drawn in turn.
#define GREEN_VINE_WIDTH    32
#define GREEN_VINE_HEIGHT   32
#define GREEN_VINE_PICTURES 5

// Sixteen green vines grow from the middle of the row of targets, 2 ticks apart, and shrink back.
//
// Abilities: Vine.
void btEffectRunVine(BATTLE_EFFECT_PARM *args)
{
	BATTLE_EFFECT_STATE *state;
	void                *canvas;
	BLITTER_PAIR         draw;
	Sint32               originX, originY;
	SCREEN_POINT         first, last;
	Sint32               i, frame, picture, height, offset, middle;
	PARTICLE            *column;

	state       = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	canvas      = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
	state->args = args;
	btEffectRunDjinniEntrance(args,
	                          ELEMENT_VENUS + DJINNI_ENTRANCE_LONG,
	                          args->side,
	                          DJINNI_ANCHOR_CENTRE,
	                          &originX,
	                          &originY);
	btEffectStartCanvasLayer(BGCNT_PRIORITY(1));
	REG_BG2PA  = Q_8_8(1); // the canvas at its own width
	REG_BLDCNT = 0;
	btResourceLoadAndDecompress(RESOURCE_GREEN_VINE_SHEET, state->sheet, 1, 1);

	// Centre the vines between the first and last targets.
	btProjectionScaledUnit(state->args->targets[0], &first);
	btProjectionScaledUnit(state->args->targets[state->args->targetCount - 1], &last);
	middle    = first.x;
	middle   += (last.x - middle) / 2;
	first.x   = middle;
	REG_BG2X  = (CANVAS_SIZE / 2 - first.x) << 8;
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, COMPOSITE_TRANSPARENT);
	draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_TRANSPARENT);
	draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
	column  = state->particles;
	for (i = 0; i != VINE_COLUMNS; i++)
	{
		column[i].x = VineColumnOffsets[i] + CANVAS_SIZE / 2;
	}
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
	for (frame = 0; frame != VINE_TICKS; frame++)
	{
		if (frame == 32)
		{
			sySoundPlayStub(SE_CRACK);
			EFFECT_FOR_EACH_TARGET(i, state->args)
			btEffectTintBattler(state->args->targets[i], SPRITE_COLOR_WHITE, POSE_DAMAGE, i, 16);
		}
		for (i = 0; i != VINE_COLUMNS; i++)
		{
			column = &state->particles[i];
			if (frame == i * 4 + 5)
				state->shakeFrame = 2;

			// Each vine grows, then shrinks back.
			offset = i * 2;
			if (frame > offset + 4)
			{
				picture = (frame / 4 + i) % GREEN_VINE_PICTURES;
				if (frame < offset + 32)
				{
					height = (frame - offset) * 4 - 16;
					if (height > GREEN_VINE_HEIGHT)
						height = GREEN_VINE_HEIGHT;
				}
				else
				{
					height = 160 - (frame - offset) * 4;
				}
				if (height > 0)
					draw[i & 1](canvas,
					            state->sheet + picture * PICTURE_BYTES(GREEN_VINE),
					            column->x - GREEN_VINE_WIDTH / 2,
					            (i & 7) - height + 104,
					            GREEN_VINE_WIDTH,
					            height);
			}
		}
		btCameraShake(4, 4);
		EFFECT_END_TICK(state);
	}
	EFFECT_FINISH_CANVAS();
}

// Thorns burst from the ground one every 8 ticks, throwing up dust from their roots.
//
// Abilities: Thorn, Briar, Nettle.
void btEffectRunThorn(BATTLE_EFFECT_PARM *args)
{
	void                *canvas;
	Sint32               frame;
	DRAW_RECTANGLE       drawFlip;
	DRAW_RECTANGLE       draw;
	Sint32               frameCount;
	BATTLE_EFFECT_STATE *state;
	Sint32               i;
	Sint32               j;
	Sint32               age;
	Sint32               start;

	state       = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	canvas      = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
	state->args = args;
	btEffectStartCanvasLayer(BGCNT_PRIORITY(1));
	REG_BG2PA    = Q_8_8(1); // the canvas at its own width
	REG_BLDCNT   = 0;
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	btResourceLoadAndDecompress(RESOURCE_THORN_SHEET, state->sheet, 1, 1);
	if (state->args->side == EFFECT_SIDE_PARTY)
	{
		REG_BG2X = -(NARROW_CANVAS_PARTY_X << 8);
	}
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, COMPOSITE_TRANSPARENT);
	draw = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_TRANSPARENT);
	// clear the canvas every tick
	state->transferMode  = CANVAS_COPY_AND_CLEAR;
	state->transferValue = 0;
	drawFlip             = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
	sySetEvent(btEffectTransferCanvas, EVENT_PASS_VBLANK);
	frameCount = ThornCounts[state->args->variant] * 8 + 56;

	for (i = 0; i != PARTICLE_POOL_COUNT; i++)
	{
		PARTICLES[i].variant = -1;
	}

	for (frame = 0; frame != frameCount; frame++)
	{
		if (frame == frameCount - 64)
		{
			EFFECT_SHOW_DAMAGE(SE_WATER);
		}
		if (frame >= frameCount - 16)
		{
			// Fade out over the last 16 ticks.
			REG_BLDCNT   = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
			REG_BLDALPHA = EFFECT_BLEND_CANVAS(frameCount - frame - 1);
		}

		for (i = 0; i != ThornCounts[state->args->variant]; i++)
		{
			start = i * 8 + 8;
			age   = frame - (i * 8 + 8);
			if (frame > start)
			{
				Sint32 height;
				Sint32 lean;

				// A thorn leans sideways as it grows.
				if (ThornKinds[i] < THORN_SMALL)
				{
					height = age * 16;
					lean   = age * 6;
					if (height > THORN_BIG_HEIGHT)
					{
						height = THORN_BIG_HEIGHT;
					}
					if (lean > THORN_BIG_LEAN)
					{
						lean = THORN_BIG_LEAN;
					}
					if (ThornKinds[i] & THORN_MIRRORED)
					{
						drawFlip(canvas,
						         state->sheet,
						         ThornX[i] - lean,
						         108 - height,
						         THORN_BIG_WIDTH,
						         height);
					}
					else
					{
						draw(canvas,
						     state->sheet,
						     ThornX[i] + lean,
						     108 - height,
						     THORN_BIG_WIDTH,
						     height);
					}
				}
				else
				{
					height = age * 8;
					lean   = age;
					if (height > THORN_SMALL_PICTURE_HEIGHT)
					{
						height = THORN_SMALL_PICTURE_HEIGHT;
					}
					if (age > THORN_SMALL_LEAN)
					{
						lean = THORN_SMALL_LEAN;
					}
					if (ThornKinds[i] & THORN_MIRRORED)
					{
						drawFlip(canvas,
						         state->sheet + THORN_SMALL_PICTURE,
						         ThornX[i] - lean,
						         108 - height,
						         THORN_SMALL_PICTURE_WIDTH,
						         height);
					}
					else
					{
						draw(canvas,
						     state->sheet + THORN_SMALL_PICTURE,
						     ThornX[i] + lean,
						     108 - height,
						     THORN_SMALL_PICTURE_WIDTH,
						     height);
					}
				}
				if (frame == start + 1)
				{
					state->shakeFrame = 3;
				}

				// Dust puffs beside the thorn.
				if (frame < start + 3)
				{
					Sint32    y = (syRandom() & 31) + 72;
					PARTICLE *puff;
					for (j = 0; j != THORN_DUST; j++)
					{
						puff = &PARTICLES[j];
						if (puff->variant == -1)
						{
							puff->x = ThornX[i] + (syRandom() & 31) + 32;
							if (puff->x > 96)
							{
								puff->x = 96;
							}
							puff->y       = y;
							puff->variant = 0;
							break;
						}
					}
				}
			}
			EFFECT_FOR_EACH_TARGET(j, state->args)
			{
				if (frame == start + 4)
				{
					sySoundPlayStub(SE_WATER);
					btEffectTintBattler(
						state->args->targets[j], SPRITE_COLOR_WHITE, POSE_DAMAGE, j, 3);
				}
			}
		}

		// Dust, over the thorns.
		for (i = 0; i != THORN_DUST; i++)
		{
			if (PARTICLES[i].variant >= 0)
			{
				Sint32 picture = PARTICLES[i].variant / 2;
				draw(canvas,
				     state->sheet + ThornPuffPictures[picture],
				     PARTICLES[i].x - ThornPuffWidths[picture],
				     PARTICLES[i].y - (Sint8)ThornPuffHeights[picture] / 2,
				     ThornPuffWidths[picture],
				     (Sint8)ThornPuffHeights[picture]);
				drawFlip(canvas,
				         state->sheet + ThornPuffPictures[picture],
				         PARTICLES[i].x,
				         PARTICLES[i].y - (Sint8)ThornPuffHeights[picture] / 2,
				         ThornPuffWidths[picture],
				         (Sint8)ThornPuffHeights[picture]);
				if (++PARTICLES[i].variant == DUST_PUFF_TICKS)
				{
					PARTICLES[i].variant = -1;
				}
			}
		}
		btCameraShake(8, 8);
		EFFECT_END_TICK(state);
	}

	EFFECT_FINISH_CANVAS();
}

#undef GROWING_COLUMNS_VINE_DUST_PICTURES
#undef GROWING_COLUMNS_PILLAR_DUST_PICTURES
#undef THORN_SMALL_PICTURE
#undef VINE_COLUMNS
#undef VINE_TICKS
#undef GREEN_VINE_WIDTH
#undef GREEN_VINE_HEIGHT
#undef GREEN_VINE_PICTURES
#undef GROWTH_DUST
#undef THORN_DUST
#undef DUST_PUFF_TICKS
#undef GROWING_COLUMNS
#undef THORN_BIG_LEAN
#undef THORN_SMALL_LEAN
#undef THORN_BIG_WIDTH
#undef THORN_BIG_HEIGHT
#undef THORN_SMALL_PICTURE_WIDTH
#undef THORN_SMALL_PICTURE_HEIGHT

// The Judgment summon. Part of battle_effect.c.

// Judgment's sprite scale, one to one.
extern const SCALE JudgmentUnitScale;

// The gathering's length and the sweep's, in ticks.
#define JUDGMENT_GATHER_TICKS             160
#define JUDGMENT_SWEEP_TICKS              336
// The gathering's crash: the canvas fills to JUDGMENT_CRASH_LEVEL the tick before, then the beams
// fall.
#define JUDGMENT_BEAM_TICK                144
#define JUDGMENT_CRASH_LEVEL              42
#define JUDGMENT_FORMATION_OBJECTS        16 // sprites in the formation, 4x4
#define JUDGMENT_FORMATION_COLUMNS        4
#define JUDGMENT_FORMATION_SPRITE_SIZE    32
// The formation's last pose, its lion shield held before it (WingedKnightCTiles), draws
// JUDGMENT_SHIELD_FORWARD_OBJECTS of the sprites and leaves the three JUDGMENT_SHIELD_FORWARD_EMPTY
// says empty.
#define JUDGMENT_SHIELD_FORWARD_OBJECTS   13
#define JUDGMENT_SHIELD_FORWARD_EMPTY(i)  ((i) == 8 || (i) == 12 || (i) == 15)
#define JUDGMENT_DEBRIS_OBJECTS           16 // debris sprites of the sweep

// Where the motes gather on the canvas, at Judgment's shield: the glow, its shards and the falling
// beams are drawn about it.
#define JUDGMENT_BURST_X                  60
#define JUDGMENT_BURST_Y                  80
// How far from the burst's centre its shards flicker as the glow swells, out to twice as far.
#define JUDGMENT_SHARD_RADIUS             16
// The point the gathering's motes circle in to, just below the burst.
#define JUDGMENT_MOTES_X                  56
#define JUDGMENT_MOTES_Y                  96
// The gathering motes' depths either side of their centre; a mote is drawn a size larger for each
// JUDGMENT_MOTE_SIZE_DEPTH nearer, from size 2.
#define JUDGMENT_MOTE_DEPTH               60
#define JUDGMENT_MOTE_SIZE_DEPTH          20
// The pink burst's beam picture, at the sprite sheet's start.
#define JUDGMENT_BEAM_WIDTH               24
#define JUDGMENT_BEAM_HEIGHT              64
// The tornado sprite sheet, after the beam; its six glint pictures are the glow's shards
// (JudgmentShardPictures counts from the sprite sheet's start).
#define JUDGMENT_SHEET_TORNADO            PICTURE_BYTES(JUDGMENT_BEAM)
// The glow, a shaded disc up to JUDGMENT_GLOW_SIZE pixels tall, drawn over the tornado sprite
// sheet's strips after its six glints.
#define JUDGMENT_SHEET_GLOW               (JUDGMENT_SHEET_TORNADO + GLINT_SET_BYTES)
#define JUDGMENT_GLOW_SIZE                80
// The explosion's four pictures, loaded over the flares on the sweep's tick 64.
#define JUDGMENT_SHEET_EXPLOSION          16384
// The two flare pictures, from the crescent sprite sheet the sweep loads.
#define JUDGMENT_FLARE_WIDTH              24
#define JUDGMENT_FLARE_HEIGHT             24
#define JUDGMENT_SHEET_FLARE_A            16384
#define JUDGMENT_SHEET_FLARE_B            (JUDGMENT_SHEET_FLARE_A + PICTURE_BYTES(JUDGMENT_FLARE))
// JudgmentFlashSheet's three flash pictures, a byte a pixel, one after another in the sprite sheet:
// the small flash, the large flash and the glow.
#define JUDGMENT_SHEET_FLASH_SMALL_WIDTH  16
#define JUDGMENT_SHEET_FLASH_SMALL_HEIGHT 17
#define JUDGMENT_SHEET_FLASH_LARGE_WIDTH  24
#define JUDGMENT_SHEET_FLASH_LARGE_HEIGHT 41
#define JUDGMENT_SHEET_FLASH_GLOW_WIDTH   22
#define JUDGMENT_SHEET_FLASH_GLOW_HEIGHT  44
#define JUDGMENT_SHEET_FLASH_SMALL        24576
#define JUDGMENT_SHEET_FLASH_LARGE                                                                 \
	(JUDGMENT_SHEET_FLASH_SMALL + PICTURE_BYTES(JUDGMENT_SHEET_FLASH_SMALL))
#define JUDGMENT_SHEET_FLASH_GLOW                                                                  \
	(JUDGMENT_SHEET_FLASH_LARGE + PICTURE_BYTES(JUDGMENT_SHEET_FLASH_LARGE))
#define JUDGMENT_EXPLOSION_WIDTH         32
#define JUDGMENT_EXPLOSION_HEIGHT        54
#define JUDGMENT_EXPLOSION_PICTURE_BYTES PICTURE_BYTES(JUDGMENT_EXPLOSION)

// FireRingTexture, in the map cell buffer: the rings of light that spread over the ground at the
// crash and at the flare's explosion, tilted JUDGMENT_CRASH_RING_TILT and JUDGMENT_FLASH_RING_TILT.
#define JUDGMENT_FIRE_RING               16384
// The ticks from which the crash's ring and the explosion's ring fade, eight levels a tick.
#define JUDGMENT_FIRE_RING_FADE_TICK     152
#define JUDGMENT_FLASH_RING_FADE_TICK    80
#define JUDGMENT_CRASH_RING_TILT         3000
#define JUDGMENT_FLASH_RING_TILT         5000

// The sweep's scenes, by the tick each opens on: the flare lands on the aerial view and it
// explodes, the wave crosses the beach, the debris rises in the forest, and the battle returns. A
// or B skips to the next scene from five ticks into one.
#define JUDGMENT_SWEEP_EXPLOSION         64
#define JUDGMENT_SWEEP_BEACH             150
#define JUDGMENT_SWEEP_FOREST            214
#define JUDGMENT_SWEEP_RETURN            280
#define JUDGMENT_WAVE_SIZE               120
// The level the wave's rings are drawn at.
#define JUDGMENT_RING_LEVEL              20
// Where the canvas sits on the screen in the sweep: the wave picture centred.
#define JUDGMENT_SWEEP_CANVAS_X          ((DISPLAY_WIDTH - JUDGMENT_WAVE_SIZE) / 2)
// The column of the wave picture the flare falls down and the aerial view's rings spread from, at
// JUDGMENT_AERIAL_RINGS_Y; the beach's rings spread from further right and higher.
#define JUDGMENT_FLARE_X                 (JUDGMENT_WAVE_SIZE / 2)
#define JUDGMENT_AERIAL_RINGS_Y          44
#define JUDGMENT_BEACH_RINGS_X           112
#define JUDGMENT_BEACH_RINGS_Y           32
// Where the blast and its debris sweep back in from as the battle returns, beyond the screen's
// right edge.
#define JUDGMENT_RETURN_BLAST_X          (DISPLAY_WIDTH + 32)

// Sets a ring's four mirrored points (left or right, top or bottom) of the wave picture to
// JUDGMENT_RING_LEVEL.
#define JUDGMENT_PLOT_RING_POINTS(left, right, top, bottom)                                        \
	(state->sheet[(bottom) * JUDGMENT_WAVE_SIZE + (right)] = JUDGMENT_RING_LEVEL,                  \
	 state->sheet[(top) * JUDGMENT_WAVE_SIZE + (right)]    = JUDGMENT_RING_LEVEL,                  \
	 state->sheet[(bottom) * JUDGMENT_WAVE_SIZE + (left)]  = JUDGMENT_RING_LEVEL,                  \
	 state->sheet[(top) * JUDGMENT_WAVE_SIZE + (left)]     = JUDGMENT_RING_LEVEL)

// The battle wipes away to a violet sky where Judgment, a winged knight with a lion shield, flies
// forward while motes gather into its shield; a glow swells there and bursts, beams fall and a ring
// of light spreads below, then a flare falls on the land below and explodes in rings of light, and
// the blast sweeps over a beach and through a forest, throwing up rocks, before the battle returns.
//
// Abilities: Judgment.
void btEffectRunJudgment(BATTLE_EFFECT_PARM *args)
{
	Sint32                   anchor[3];
	SCREEN_POINT             pos;
	SPR_PLACEMENT            place;
	SPR_PLACEMENT            debrisPlace;
	SCALE                    pair;
	MODEL_TEXTURE            texture;
	MODEL_TEXTURE            glare;
	SPR_SCALE                scale;
	Uint8                   *canvas;
	BATTLE_EFFECT_STATE     *state;
	BATTLE_BACKGROUND_STATE *view;
	DRAW_RECTANGLE           draw[2];
	Sint32                   frame;
	Uint8                   *motes;
	FIXED                    depth;
	Sint32                   zoom;
	Sint32                   addR, addG, addB;
	Sint32                   step;
	Sint32                   cx, cy, squash, sx, ring, rings, shift;
	Sint32                   i;
	Uint8                   *cells;
	Uint8                   *palette;

	canvas = RamHeapSlots->battleCanvas;
	state  = RamHeapSlots->battleEffect;
	view   = RamHeapSlots->battleBg;
	motes  = RamHeapSlots->battleSheet;
	cells  = RamMapMetatileBuffer;
	EFFECT_START_NARROW_CANVAS(BGCNT_WRAP);
	btEffectConfigureEffectDisplay();
	PLTT_BACKDROP       = 0;
	BG_PLTT_COLOR(0, 1) = 0;
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
	state->transferMode = CANVAS_COPY;
	draw[0]             = RamHeapSlots->ramCode;
	sySetEvent(btEffectTransferCanvas, EVENT_PASS_VBLANK);
	// Wipe the battle away, change the battle background behind it and set out Judgment's sprites
	// in its first pose, then wipe back.
	btEffectWipeCanvas(CANVAS_WIPE_DOWN, CANVAS_WIPE_FILL);
	btEffectShowLivingTargets(state->args);
	btBackgroundLoadStub(BATTLE_BACKGROUND_FIXED, RESOURCE_VIOLET_SKY_BACKDROP, 0);

	palette = syResourceGetTableEntry(RESOURCE_WINGED_KNIGHT_A_TILES);
	syWordsCopy((void *)OBJ_PLTT, palette, OBJ_PLTT_SIZE);
	palette += OBJ_PLTT_SIZE;
	syDecompressTaggedLZ(palette, cells);
	for (i = 0; i != JUDGMENT_FORMATION_OBJECTS; i++)
	{
		SPR *object            = btEffectSpriteCreateWithTiles(JUDGMENT_FORMATION_SPRITE_SIZE,
                                                    JUDGMENT_FORMATION_SPRITE_SIZE,
                                                    OBJ_SIZE_32X32 | OBJ_256_COLORS,
                                                    0);

		object->image.priority = 2;
		state->sprites[i]      = object;
		syWordsCopyEntry(
			(void *)OBJ_TILES(object), cells + i * OBJ_32X32_8BPP_BYTES, OBJ_32X32_8BPP_BYTES);
	}
	ProjectionWork.centerY = SUMMON_PROJECTION_CENTER_Y;
	sySynch(1);
	view->busy        = 1;
	BgScrollWork[1].x = 0;
	btEffectWipeCanvas(CANVAS_WIPE_DOWN, CANVAS_WIPE_CLEAR);
	EFFECT_SHOW_WIDE_CANVAS(BLEND_WEIGHT_MAX);
	depth = -toFIXED(64);
	zoom  = toFIXED(8);
	btResourceLoadAndDecompress(RESOURCE_PINK_BURST_SHEET, state->sheet, 1, 1);
	btResourceLoadAndDecompress(
		RESOURCE_TORNADO_SHEET, state->sheet + JUDGMENT_SHEET_TORNADO, 1, 0);
	btResourceLoadAndDecompress(
		RESOURCE_FIRE_RING_TEXTURE, RamMapMetatileBuffer + JUDGMENT_FIRE_RING, 0, 0);
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, motes, 0, 0);
	palette = syResourceGetTableEntry(RESOURCE_BLUE_CRESCENT_SHEET);
	syWordsCopyEntry((void *)BG_PLTT, palette, SHEET_PALETTE_BYTES);
	EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
	REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);

	// The gathering.
	frame      = 0;
	do
	{
		// The motes' places on their spheres, and Judgment's first pose again.
		if (frame == 0)
		{
			PARTICLE *p = state->particles;

			for (i = 0; i != ARRAY_COUNT(state->particles); i++, p++)
			{
				p->x         = (syRandom() & 15) + 72;
				p->velocityX = syRandom() & TRIG_ANGLE_MASK;
				// velocityY is drawn twice; the second draw is the one kept.
				p->velocityY = syRandom() & TRIG_ANGLE_MASK;
				p->velocityY = syRandom() & TRIG_ANGLE_MASK;
			}
			palette = syResourceGetTableEntry(RESOURCE_WINGED_KNIGHT_A_TILES);
			syWordsCopyEntry((void *)OBJ_PLTT, palette, OBJ_PLTT_SIZE);
			palette += OBJ_PLTT_SIZE;
			syDecompressTaggedLZ(palette, RamMapMetatileBuffer);
			for (i = 0; i != JUDGMENT_FORMATION_OBJECTS; i++)
			{
				SPR *object = state->sprites[i];

				syWordsCopyEntry((void *)OBJ_TILES(object),
				                 RamMapMetatileBuffer + i * OBJ_32X32_8BPP_BYTES,
				                 OBJ_32X32_8BPP_BYTES);
			}
			depth = -toFIXED(64);
			zoom  = toFIXED(8);
		}
		if (frame == JUDGMENT_BEAM_TICK - 1)
		{
			IWRAM_FILL_WORDS(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(JUDGMENT_CRASH_LEVEL));
			sySoundPlayStub(SE_CRASH);
		}
		if (frame == 80)
		{
			sySoundPlayStub(SE_SWIRL);
		}
		if (frame == 72)
		{
			state->fadeFrame = BACKGROUND_DARKEN_TICKS;
			state->fadeStep  = 0;
		}

		// The motes circle in to Judgment's shield, four more each tick from tick 80.
		anchor[2] = 0;
		anchor[1] = 0;
		{
			PARTICLE *p = state->particles;

			for (i = 0; i != ARRAY_COUNT(state->particles); i++, p++)
			{
				Sint32 size;

				if (frame > i / 4 + 80 && p->x > 0)
				{
					syInitMatrix();
					syRotZ(p->velocityY);
					syRotX(p->velocityX);
					syRotY(p->velocityY + frame * (TRIG_TURN / 128));
					anchor[0]  = p->x;
					p->x      -= 2;
					if (p->x < 0)
					{
						p->x = 0;
					}
					btEffectPosProjectPoint(anchor, &pos);
					CLAMP(pos.depth, -JUDGMENT_MOTE_DEPTH, JUDGMENT_MOTE_DEPTH);
					pos.depth += JUDGMENT_MOTE_DEPTH;
					size       = pos.depth / JUDGMENT_MOTE_SIZE_DEPTH + 2;
					pos.x     += JUDGMENT_MOTES_X;
					pos.y     += JUDGMENT_MOTES_Y;
					EFFECT_DRAW_MOTE(draw[0], canvas, motes, pos.x, pos.y, size);
				}
			}
		}

		// The formation slows to a stop, jumps back and drifts forward again.
		pair               = JudgmentUnitScale;
		place.groundHeight = 0;
		place.height       = 0;
		for (i = 0; i != JUDGMENT_FORMATION_OBJECTS; i++)
		{
			if (frame > 75 && JUDGMENT_SHIELD_FORWARD_EMPTY(i))
			{
				continue;
			}
			place.x = toFIXED((i % JUDGMENT_FORMATION_COLUMNS) * JUDGMENT_FORMATION_SPRITE_SIZE)
			        + toFIXED(76);
			place.y =
				toFIXED((i / JUDGMENT_FORMATION_COLUMNS) * JUDGMENT_FORMATION_SPRITE_SIZE) + depth;
			fdRenderApplyProjectedPlacementStub(
				state->sprites[i], sySpritePlacementValues(&place), (Sint32 *)&pair, 0);
		}
		depth += zoom;
		if (frame <= 47)
		{
			zoom -= toFIXED(1) / 4;
		}
		if (frame > 32)
		{
			zoom = zoom * 60 / DAMPING_SCALE;
		}
		if (frame == JUDGMENT_BEAM_TICK)
		{
			zoom = -toFIXED(8);
		}
		if (frame == 146)
		{
			zoom = toFIXED(1);
		}

		// Judgment swings its shield round, then holds it before it.
		if (frame == 72)
		{
			palette = syResourceGetTableEntry(RESOURCE_WINGED_KNIGHT_B_TILES);
			syWordsCopyEntry((void *)OBJ_PLTT, palette, OBJ_PLTT_SIZE);
			palette += OBJ_PLTT_SIZE;
			syDecompressTaggedLZ(palette, RamMapMetatileBuffer);
			for (i = 0; i != JUDGMENT_FORMATION_OBJECTS; i++)
			{
				IO_WRITE_QUEUE_PUSH(OBJ_TILES((SPR *)state->sprites[i]),
				                    (Uint32)(cells + i * OBJ_32X32_8BPP_BYTES),
				                    DMA_COPY_WORDS(OBJ_32X32_8BPP_BYTES / 4));
			}
		}
		if (frame == 76)
		{
			palette = syResourceGetTableEntry(RESOURCE_WINGED_KNIGHT_C_TILES);
			syWordsCopyEntry((void *)OBJ_PLTT, palette, OBJ_PLTT_SIZE);
			palette += OBJ_PLTT_SIZE;
			syDecompressTaggedLZ(palette, RamMapMetatileBuffer);
			for (i = 0; i != JUDGMENT_SHIELD_FORWARD_OBJECTS; i++)
			{
				IO_WRITE_QUEUE_PUSH(OBJ_TILES((SPR *)state->sprites[JudgmentShieldSprites[i]]),
				                    (Uint32)(cells + i * OBJ_32X32_8BPP_BYTES),
				                    DMA_COPY_WORDS(OBJ_32X32_8BPP_BYTES / 4));
			}
		}

		// The glow swells at the shield, 3 pixels a tick.
		if (frame >= 116 && frame < JUDGMENT_BEAM_TICK)
		{
			Sint32 size = (frame - 116) * 3 + 2;

			if (size > JUDGMENT_GLOW_SIZE)
			{
				size = JUDGMENT_GLOW_SIZE;
			}
			else
			{
				btEffectDrawShadedCircles(state->sheet + JUDGMENT_SHEET_GLOW, size, toFIXED(1));
			}
			btCanvasBlitMirroredQuarters(
				state->sheet + JUDGMENT_SHEET_GLOW, JUDGMENT_BURST_X, JUDGMENT_BURST_Y, size);
		}
		// Shards flicker about it, ever further out.
		if (frame >= 116 && frame < JUDGMENT_BEAM_TICK)
		{
			Sint32 radius = (frame - 116) / 3 + JUDGMENT_SHARD_RADIUS;

			// DEAD: never true, as the radius reaches only 25 by the crash; the ROM keeps the test
			// and the store.
			if (radius > 2 * JUDGMENT_SHARD_RADIUS)
			{
				radius = 2 * JUDGMENT_SHARD_RADIUS;
			}
			for (i = 0; i != 6; i++)
			{
				Sint32 k     = i & 3;
				Sint32 angle = syRandom() & TRIG_ANGLE_MASK;
				Sint32 x     = (FIXEDtoINT(radius * sySin(angle))) - (JudgmentShardWidths[k] >> 1);
				Sint32 y     = (FIXEDtoINT(radius * syCos(angle))) - (JudgmentShardHeights[k] / 2);

				draw[0](canvas,
				        state->sheet + JudgmentShardPictures[k],
				        x + JUDGMENT_BURST_X,
				        y + JUDGMENT_BURST_Y,
				        JudgmentShardWidths[k],
				        JudgmentShardHeights[k]);
			}
		}

		// Judgment's colours brighten toward a pale blue as the glow swells, turn white at the
		// crash and come back over the next 8 ticks.
		{
			Sint32 flag = 0;

			if (frame >= 108 && frame <= 142)
			{
				Sint32 t = (frame - 108) / 2;

				addR     = t / 4;
				addG     = t / 2;
				addB     = t;
				flag     = 1;
			}
			if (frame == JUDGMENT_BEAM_TICK - 1)
			{
				addB = addG = addR = RGB_LEVEL_MAX;
				flag               = 1;
			}
			if (frame >= JUDGMENT_BEAM_TICK && frame <= JUDGMENT_BEAM_TICK + 8)
			{
				// FAKEMATCH(dead): step assigned to itself as it is set.
				step = step = 304 - frame * 2;
				addB = addR = addG = step;
				flag               = 1;
			}
			if (flag == 1)
			{
				Uint16          *src  = syResourceGetTableEntry(RESOURCE_WINGED_KNIGHT_C_TILES);
				volatile Uint16 *dest = (volatile Uint16 *)OBJ_PLTT;

				for (i = 0; i != OBJ_PLTT_COLORS; i++, src++, dest++)
				{
					Sint32 r = RGB_RED(*src) + addR;
					Sint32 g = RGB_GREEN(*src) + addG;
					Sint32 b = RGB_BLUE(*src) + addB;

					if (r > RGB_LEVEL_MAX)
						r = RGB_LEVEL_MAX;
					if (g > RGB_LEVEL_MAX)
						g = RGB_LEVEL_MAX;
					if (b > RGB_LEVEL_MAX)
						b = RGB_LEVEL_MAX;
					if (r < 0)
						r = 0;
					if (g < 0)
						g = 0;
					if (b < 0)
						b = 0;
					*dest = RGB(r, g, b);
				}
			}
		}

		// The beams fall from the burst, 16 pixels a tick from 32 pixels down.
		if (frame >= JUDGMENT_BEAM_TICK)
		{
			Sint32 y = frame * 16 - (JUDGMENT_BEAM_TICK - 2) * 16;
			Sint32 flag;

			flag = EFFECT_CLIP_BOUNDS;
			EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, flag, BLITTER_ADDITIVE);
			(RamHeapSlots->ramCodePaired)(canvas,
			                              state->sheet,
			                              JUDGMENT_BURST_X - JUDGMENT_BEAM_WIDTH,
			                              y,
			                              JUDGMENT_BEAM_WIDTH,
			                              JUDGMENT_BEAM_HEIGHT);
			syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
			flag = EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X;
			EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, flag, BLITTER_ADDITIVE);
			(RamHeapSlots->ramCodePaired)(canvas,
			                              state->sheet,
			                              JUDGMENT_BURST_X,
			                              y,
			                              JUDGMENT_BEAM_WIDTH,
			                              JUDGMENT_BEAM_HEIGHT);
			syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
		}

		// A ring of light spreads over the ground below and fades.
		if (frame >= JUDGMENT_BEAM_TICK && frame < JUDGMENT_GATHER_TICKS)
		{
			Sint32        s;
			Sint32        shade;
			void         *verts;
			RENDER_MODEL *model;

			verts             = syHeapAllocateEWRAM(32);
			model             = btRenderAllocateModelList(1);
			shade             = 0;
			s                 = (frame - JUDGMENT_BEAM_TICK + 1) * (toFIXED(1) / 8);
			texture.logWidth  = 7;
			texture.logHeight = 7;
			texture.pixels    = RamMapMetatileBuffer + JUDGMENT_FIRE_RING;
			model->texture    = &texture;
			model->kind       = MODEL_SHADED_ADDITIVE;
			model->mesh       = BattleEffectLargeSquareMesh;
			model->vertices   = verts;
			if (frame >= JUDGMENT_FIRE_RING_FADE_TICK)
			{
				shade = frame * -8 + JUDGMENT_FIRE_RING_FADE_TICK * 8;
			}
			model->shade = shade;
			syInitMatrix();
			syTranslate(-toFIXED(4), toFIXED(24), 0);
			syMatrixScaleUniform(s);
			syRotX(JUDGMENT_CRASH_RING_TILT);
			btRenderTransformVertices(BattleEffectGroundSquareCorners, verts, 4);
			btRenderDrawModelList(model);
			syHeapRelease(model);
			syHeapRelease(verts);
		}

		EFFECT_PRESENT_CANVAS(state);
		frame++;
	} while (frame != JUDGMENT_GATHER_TICKS && (frame <= 4 || !(PerWork.repeat & (KEY_A | KEY_B))));

	// The sweep's setup.
	syWordsFill(canvas, CANVAS_BYTES, 0);
	for (i = 0; i != JUDGMENT_FORMATION_OBJECTS; i++)
	{
		fdSpriteDestroyStub(state->sprites[i]);
	}
	sySynch(1);
	syPaletteLoadBasicSpriteColors();
	syDMAXCopy((const void *)OBJ_PLTT,
	           (void *)&BG_PLTT_COLOR(BASIC_COLORS_PALETTE, 0),
	           DMA_COPY_HALFWORDS(PALETTE_COLORS),
	           REG_DMA3);
	BG_PLTT_COLOR(BASIC_COLORS_PALETTE, TEXT_BACKGROUND_COLOR) =
		BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR);
	for (i = 0; i != JUDGMENT_DEBRIS_OBJECTS; i++)
	{
		SPR *object       = fdSpriteCreateStub(SPRITE_BATTLE_ROCKS);

		state->sprites[i] = object;
		if (object)
		{
			object->flags = 0;
			fdSpriteStartAnimationStub(object, i % 3);
			((SPR *)state->sprites[i])->image.priority = 0;
		}
	}
	syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
	draw[0] = RamHeapSlots->ramCode;
	btResourceLoadAndDecompress(
		RESOURCE_BLUE_CRESCENT_SHEET, state->sheet + JUDGMENT_SHEET_FLARE_A, 1, 1);
	// The motes are dimmed, though the sweep draws none: a level in the brighter half drops by half
	// the levels, and any other but 0 becomes 1.
	{
		Uint8 *c = motes;

		for (i = 0; i != MOTE_SET_BYTES; i++, c++)
		{
			if (*c > (CANVAS_LEVEL_BRIGHTEST + 1) / 2)
			{
				*c -= (CANVAS_LEVEL_BRIGHTEST + 1) / 2;
			}
			else if (*c != 0)
			{
				*c = 1;
			}
		}
	}
	// Stop the battle background fade.
	state->fadeFrame = 0;
	state->fadeStep  = 0;
	btBackgroundLoadStub(BATTLE_BACKGROUND_FIXED, RESOURCE_AERIAL_VIEW_BACKDROP, 0);
	REG_BG2PA  = Q_8_8(1);
	REG_BG2X   = -(JUDGMENT_SWEEP_CANVAS_X << 8);
	REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
	IWRAM_FILL_WORDS(canvas, CANVAS_BYTES, 0);
	cx     = JUDGMENT_FLARE_X;
	cy     = JUDGMENT_AERIAL_RINGS_Y;
	squash = 6;
	ring   = 0;
	shift  = 0;
	sx     = 8;
	rings  = 2;
	EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
	syWordsFillEntry(state->sheet, JUDGMENT_WAVE_SIZE * JUDGMENT_WAVE_SIZE, 0);

	// The sweep, on its own tick clock.
	for (frame = 0; frame != JUDGMENT_SWEEP_TICKS; frame++)
	{
		if (frame == 66)
		{
			sySoundPlayStub(SE_BANE_ESCAPE);
		}
		if (frame == 88)
		{
			sySoundPlayStub(SE_WATER_DRAIN);
		}
		if (frame == 155)
		{
			sySoundPlayStub(SE_WAVE);
		}
		if (frame == 217)
		{
			sySoundPlayStub(SE_SWITCH_MECHANISM);
		}
		if (frame == JUDGMENT_SWEEP_RETURN)
		{
			sySoundPlayStub(SE_GATE_MOVE);
		}
		if (frame == 300)
		{
			for (i = 0; i != args->targetCount; i++)
			{
				EFFECT_STRIKE_TARGET(args, i, 8, KNOCKBACK_HOP);
			}
			EFFECT_SHOW_DAMAGE(SE_CRASH);
		}
		if (PerWork.repeat & (KEY_A | KEY_B))
		{
			if (frame >= 5 && frame < JUDGMENT_SWEEP_BEACH)
			{
				frame = JUDGMENT_SWEEP_BEACH;
				IO_WRITE_QUEUE_PUSH((Uint32)&REG_BG2PA, Q_8_8(0.5), IO_WRITE_HALFWORD);
				IO_WRITE_QUEUE_PUSH((Uint32)&REG_BG2X, 0, IO_WRITE_WORD);
				btResourceLoadAndDecompress(
					RESOURCE_EXPLOSION_SHEET, state->sheet + JUDGMENT_SHEET_EXPLOSION, 1, 0);
			}
			else if (frame >= JUDGMENT_SWEEP_BEACH + 5 && frame < JUDGMENT_SWEEP_FOREST)
			{
				frame = JUDGMENT_SWEEP_FOREST;
			}
			else if (frame >= JUDGMENT_SWEEP_FOREST + 5 && frame < JUDGMENT_SWEEP_RETURN)
			{
				frame = JUDGMENT_SWEEP_RETURN;
			}
		}
		if (frame == 318)
		{
			IWRAM_FILL_WORDS(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
		}
		if (frame == 319)
		{
			btGraphicsLoadGreyRamp();
			EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(4));
		}
		if (frame == JUDGMENT_SWEEP_EXPLOSION)
		{
			IO_WRITE_QUEUE_PUSH((Uint32)&REG_BG2PA, Q_8_8(0.5), IO_WRITE_HALFWORD);
			IO_WRITE_QUEUE_PUSH((Uint32)&REG_BG2X, 0, IO_WRITE_WORD);
			IWRAM_FILL_WORDS(canvas, CANVAS_BYTES, 0);
			btResourceLoadAndDecompress(
				RESOURCE_EXPLOSION_SHEET, state->sheet + JUDGMENT_SHEET_EXPLOSION, 1, 0);
			btResourceLoadAndDecompress(
				RESOURCE_JUDGMENT_FLASH_SHEET, state->sheet + JUDGMENT_SHEET_FLASH_SMALL, 0, 0);
		}
		if (frame == 66)
		{
			// Invert the battle background's colours, background palettes 6 to 13.
			volatile Uint16 *c = &BG_PLTT_COLOR(6, 0);

			for (i = 0; i != BATTLE_BACKGROUND_COLORS; i++, c++)
			{
				*c ^= RGB_WHITE;
			}
		}
		if (frame == 69)
		{
			IWRAM_FILL_WORDS(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
		}
		// Undo the inversion, rewriting the battle background's palette from the battle's copy with
		// every channel 7 levels darker.
		if (frame == 70)
		{
			btBackgroundSetModeStub(BATTLE_BACKGROUND_FIXED, RESOURCE_AERIAL_VIEW_BACKDROP, 7);
		}
		// The beach: the rings restart, four a tick.
		if (frame == JUDGMENT_SWEEP_BEACH)
		{
			cx     = JUDGMENT_BEACH_RINGS_X;
			cy     = JUDGMENT_BEACH_RINGS_Y;
			ring   = 0;
			rings  = 4;
			squash = 8;
			IWRAM_FILL_WORDS(state->sheet, JUDGMENT_WAVE_SIZE * JUDGMENT_WAVE_SIZE, 0);
			IWRAM_FILL_WORDS(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
			btBackgroundLoadStub(BATTLE_BACKGROUND_FIXED, RESOURCE_BEACH_BACKDROP, 0);
		}
		// The forest.
		if (frame == JUDGMENT_SWEEP_FOREST)
		{
			IWRAM_FILL_WORDS(state->sheet, JUDGMENT_WAVE_SIZE * JUDGMENT_WAVE_SIZE, 0);
			IWRAM_FILL_WORDS(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
			btBackgroundLoadStub(BATTLE_BACKGROUND_FIXED, RESOURCE_FOREST_BACKDROP, 0);
			{
				PARTICLE *p = state->particles;

				for (i = 0; i != JUDGMENT_DEBRIS_OBJECTS; i++, p++)
				{
					p->x         = INTtoFIXED((syRandom() & 63) + 32);
					p->y         = INTtoFIXED((syRandom() & 31) + 96);
					p->velocityY = ((syRandom() & 15) + 16) << 15;
					p->z         = toFIXED(1) / 2 - (p->x >> 7);
					p->variant   = 1;
				}
			}
		}
		if (frame == JUDGMENT_SWEEP_RETURN)
		{
			// Back to the battle.
			btEffectSetupBlendedDisplay();
			IWRAM_FILL_WORDS(state->sheet, JUDGMENT_WAVE_SIZE * JUDGMENT_WAVE_SIZE, 0);
			IWRAM_FILL_WORDS(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(1));
			view->busy   = 0;
			REG_BLDALPHA = EFFECT_BLEND_ADD;
			{
				PARTICLE *p = state->particles;

				for (i = 0; i != JUDGMENT_DEBRIS_OBJECTS; i++, p++)
				{
					p->x         = INTtoFIXED((syRandom() & 63) + 256);
					p->y         = INTtoFIXED((syRandom() & 31) + 96);
					p->velocityY = ((syRandom() & 15) + 16) << 15;
					p->z         = 0;
				}
			}
			shift = JUDGMENT_RETURN_BLAST_X;
			EFFECT_SET_TRANSFER(state, CANVAS_COPY, 0);
		}
		// The canvas's colours brighten to white.
		if (frame >= JUDGMENT_SWEEP_RETURN && frame <= 318)
		{
			volatile Uint16 *c = &BG_PLTT_COLOR(0, 1);

			for (i = 0; i != SHEET_PALETTE_COLORS - 1; i++, c++)
			{
				Sint32 v = *c;
				Sint32 b = RGB_BLUE((Uint16)v);
				Sint32 g = RGB_GREEN((Uint16)v);
				Sint32 r = RGB_RED(v);

				b++;
				g++;
				r++;

				if (b > RGB_LEVEL_MAX)
					b = RGB_LEVEL_MAX;
				if (g > RGB_LEVEL_MAX)
					g = RGB_LEVEL_MAX;
				if (r > RGB_LEVEL_MAX)
					r = RGB_LEVEL_MAX;
				*c = RGB(r, g, b);
			}
		}
		if (frame == 182)
		{
			IWRAM_FILL_WORDS(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
		}

		// A flare falls onto the aerial view, drawn as four mirrored quarters, its picture
		// flickering every 4 ticks.
		if (frame < JUDGMENT_SWEEP_EXPLOSION)
		{
			Uint8 *sheet;
			Sint32 top = frame - 4;
			Sint32 flag;

			if ((frame & 7) < 4)
			{
				sheet = state->sheet + JUDGMENT_SHEET_FLARE_A;
			}
			else
			{
				sheet = state->sheet + JUDGMENT_SHEET_FLARE_B;
			}
			flag = EFFECT_CLIP_BOUNDS;
			EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, flag, BLITTER_MAX);
			(RamHeapSlots->ramCodePaired)(canvas,
			                              sheet,
			                              JUDGMENT_FLARE_X - JUDGMENT_FLARE_WIDTH,
			                              top - JUDGMENT_FLARE_HEIGHT,
			                              JUDGMENT_FLARE_WIDTH,
			                              JUDGMENT_FLARE_HEIGHT);
			syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
			flag = EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X;
			EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, flag, BLITTER_MAX);
			(RamHeapSlots->ramCodePaired)(canvas,
			                              sheet,
			                              JUDGMENT_FLARE_X - 1,
			                              top - JUDGMENT_FLARE_HEIGHT,
			                              JUDGMENT_FLARE_WIDTH,
			                              JUDGMENT_FLARE_HEIGHT);
			syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
			flag = EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y;
			EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, flag, BLITTER_MAX);
			(RamHeapSlots->ramCodePaired)(canvas,
			                              sheet,
			                              JUDGMENT_FLARE_X - JUDGMENT_FLARE_WIDTH,
			                              top - 1,
			                              JUDGMENT_FLARE_WIDTH,
			                              JUDGMENT_FLARE_HEIGHT);
			syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
			flag = EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP;
			EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, flag, BLITTER_MAX);
			(RamHeapSlots->ramCodePaired)(canvas,
			                              sheet,
			                              JUDGMENT_FLARE_X - 1,
			                              top - 1,
			                              JUDGMENT_FLARE_WIDTH,
			                              JUDGMENT_FLARE_HEIGHT);
			syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
		}
		// It explodes in flashes.
		if (frame >= JUDGMENT_SWEEP_EXPLOSION && frame < 66)
		{
			EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet, JUDGMENT_SHEET_FLASH_SMALL, 52, 51);
		}
		if (frame >= 66 && frame < 68)
		{
			EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet, JUDGMENT_SHEET_FLASH_LARGE, 48, 40);
		}
		if (frame >= 68 && frame < 76)
		{
			Sint32 d = 76 - frame;

			EFFECT_DRAW_PICTURE(
				draw[0], canvas, state->sheet, JUDGMENT_SHEET_FLASH_GLOW, 49 - d / 2, 38 - d);
			EFFECT_DRAW_PICTURE(
				draw[0], canvas, state->sheet, JUDGMENT_SHEET_FLASH_GLOW, 49 - d / 2, d + 38);
			EFFECT_DRAW_PICTURE(
				draw[0], canvas, state->sheet, JUDGMENT_SHEET_FLASH_GLOW, d / 2 + 49, 38 - d);
			EFFECT_DRAW_PICTURE(
				draw[0], canvas, state->sheet, JUDGMENT_SHEET_FLASH_GLOW, d / 2 + 49, d + 38);
		}
		if (frame >= 78 && frame < 80)
		{
			EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet, JUDGMENT_SHEET_FLASH_SMALL, 52, 51);
		}
		if (frame >= 80 && frame < 82)
		{
			EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet, JUDGMENT_SHEET_FLASH_LARGE, 48, 40);
		}
		if (frame >= 82 && frame < 86)
		{
			Sint32 d = frame * 2 - 164;

			EFFECT_DRAW_PICTURE(
				draw[0], canvas, state->sheet, JUDGMENT_SHEET_FLASH_GLOW, 49 - d / 4, 38 - d);
			EFFECT_DRAW_PICTURE(
				draw[0], canvas, state->sheet, JUDGMENT_SHEET_FLASH_GLOW, 49 - d / 4, d + 38);
			EFFECT_DRAW_PICTURE(
				draw[0], canvas, state->sheet, JUDGMENT_SHEET_FLASH_GLOW, d / 4 + 49, 38 - d);
			EFFECT_DRAW_PICTURE(
				draw[0], canvas, state->sheet, JUDGMENT_SHEET_FLASH_GLOW, d / 4 + 49, d + 38);
		}

		// A ring of light spreads over the land from the explosion and fades.
		if (frame >= 72 && frame < 88)
		{
			Sint32        s;
			Sint32        shade;
			void         *verts;
			RENDER_MODEL *model;

			verts           = syHeapAllocateEWRAM(32);
			model           = btRenderAllocateModelList(1);
			s               = (frame - 72) * (toFIXED(1) / 8) + toFIXED(1) / 32;
			glare.logWidth  = 7;
			glare.logHeight = 7;
			glare.pixels    = RamMapMetatileBuffer + JUDGMENT_FIRE_RING;
			shade           = 0;
			model->kind     = MODEL_SHADED_ADDITIVE;
			model->texture  = &glare;
			model->mesh     = BattleEffectLargeSquareMesh;
			model->vertices = verts;
			if (frame >= JUDGMENT_FLASH_RING_FADE_TICK)
			{
				shade = frame * -8 + JUDGMENT_FLASH_RING_FADE_TICK * 8;
			}
			model->shade = shade;
			syInitMatrix();
			syTranslate(-toFIXED(4), toFIXED(8), 0);
			syMatrixScaleUniform(s);
			syRotX(JUDGMENT_FLASH_RING_TILT);
			btRenderTransformVertices(BattleEffectGroundSquareCorners, verts, 4);
			btRenderDrawModelList(model);
			syHeapRelease(model);
			syHeapRelease(verts);
		}

		if (frame > 85)
		{
			// More rings each tick, each one pixel wider than the last.
			if (frame < JUDGMENT_SWEEP_FOREST)
			{
				for (i = 0; i != rings; i++, ring++)
				{
					Sint32 radius = ring;
					Sint32 slope  = 0;
					Sint32 err    = ring;

					while (radius >= slope)
					{
						Sint32 half, halfSlope, top, bottom, left, right, dx;

						half = radius / 2;
						dx   = (half * sx) / 8;
						// FAKEMATCH(dead): a store top's own line
						// * overwrites.
						if (dx > 1000)
							top = 0;
						right = cx + dx;
						// FAKEMATCH(dead): a branch whose store the next line
						// * overwrites.
						if (dx > cx)
							left = 0;
						left   = cx - dx;
						bottom = cy + (slope * squash) / 8;
						top    = cy - (slope * sx) / 8;
						if (top < 0)
							top = 0;
						if (bottom > JUDGMENT_WAVE_SIZE - 1)
							bottom = JUDGMENT_WAVE_SIZE - 1;
						if (left < 0)
							left = 0;
						if (right > JUDGMENT_WAVE_SIZE - 1)
							right = JUDGMENT_WAVE_SIZE - 1;
						JUDGMENT_PLOT_RING_POINTS(left, right, top, bottom);
						right = cx + half + 1;
						left  = cx - half + 1;
						if (left < 0)
							left = 0;
						if (right > JUDGMENT_WAVE_SIZE - 1)
							right = JUDGMENT_WAVE_SIZE - 1;
						JUDGMENT_PLOT_RING_POINTS(left, right, top, bottom);

						halfSlope = slope / 2;
						dx        = (halfSlope * sx) / 8;
						// FAKEMATCH(dead): as above, the pick before cx's
						// * reload in the second half.
						if (dx > 1000)
							top = 0;
						right = cx + dx;
						// FAKEMATCH(dead): a branch whose store the next line
						// * overwrites.
						if (dx > cx)
							left = 0;
						left   = cx - dx;
						bottom = cy + (radius * squash) / 8;
						top    = cy - (radius * sx) / 8;
						if (left < 0)
							left = 0;
						if (right > JUDGMENT_WAVE_SIZE - 1)
							right = JUDGMENT_WAVE_SIZE - 1;
						if (top < 0)
							top = 0;
						if (bottom > JUDGMENT_WAVE_SIZE - 1)
							bottom = JUDGMENT_WAVE_SIZE - 1;
						JUDGMENT_PLOT_RING_POINTS(left, right, top, bottom);
						right = cx + halfSlope + 1;
						left  = cx - halfSlope + 1;
						if (left < 0)
							left = 0;
						if (right > JUDGMENT_WAVE_SIZE - 1)
							right = JUDGMENT_WAVE_SIZE - 1;
						JUDGMENT_PLOT_RING_POINTS(left, right, top, bottom);

						err = err - slope * 2 - 1;
						if (err < 0)
						{
							err = err + radius * 2 - 2;
							radius--;
						}
						slope++;
					}
				}
			}
		}

		if (frame >= 86 && frame < JUDGMENT_SWEEP_BEACH)
		{
			Sint32 angle = -DEGtoANG(90);

			for (i = 0; i != 9; i++, angle += TRIG_TURN / 16)
			{
				Sint32 x = FIXEDtoINT((frame - 86) * sySin(angle));
				Sint32 y = FIXEDtoINT((frame - 70) * syCos(angle));

				draw[0](canvas,
				        state->sheet + ((frame / 2 + i) % 4) * JUDGMENT_EXPLOSION_PICTURE_BYTES
				            + JUDGMENT_SHEET_EXPLOSION,
				        x + 44,
				        y + 17,
				        JUDGMENT_EXPLOSION_WIDTH,
				        JUDGMENT_EXPLOSION_HEIGHT);
			}
		}

		EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
		draw[1] = RamHeapSlots->ramCodePaired;
		if (frame > 85)
		{
			draw[1](canvas, state->sheet, 0, 0, JUDGMENT_WAVE_SIZE, JUDGMENT_WAVE_SIZE);
		}
		if (frame >= JUDGMENT_SWEEP_RETURN)
		{
			shift -= 8;
		}
		if (frame == 238)
		{
			IWRAM_FILL_WORDS(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
		}
		if (frame >= JUDGMENT_SWEEP_FOREST && frame < JUDGMENT_SWEEP_RETURN)
		{
			BgScrollWork[1].x += 8;
		}
		if (frame >= 246 && frame < JUDGMENT_SWEEP_RETURN)
		{
			shift += 8;
			EFFECT_SET_TRANSFER(state, CANVAS_COPY, CANVAS_FADE_THREE_QUARTERS);
		}

		// Debris rises from the field.
		if (frame >= JUDGMENT_SWEEP_FOREST)
		{
			PARTICLE *p;

			debrisPlace.groundHeight = 0;
			debrisPlace.height       = 0;
			for (i = 0; i != JUDGMENT_DEBRIS_OBJECTS; i++)
			{
				p = &state->particles[i];
				if (p->variant != 0)
				{
					scale.x = p->z + (p->y >> 7);
					if (scale.x < toFIXED(1) / 32)
					{
						scale.x = toFIXED(1) / 32;
					}
					scale.y            = scale.x;
					debrisPlace.x      = p->x;
					debrisPlace.height = toFIXED(255);
					debrisPlace.y      = p->y + toFIXED(255);
					fdRenderApplyProjectedPlacementStub(
						state->sprites[i], sySpritePlacementValues(&debrisPlace), scale.values, 0);
					p->y -= p->velocityY;
					if (p->y < toFIXED(16))
					{
						if (frame < JUDGMENT_SWEEP_RETURN)
						{
							p->x = INTtoFIXED((syRandom() & 63) + shift + 32);
						}
						else
						{
							p->x = toFIXED((syRandom() & 63) + shift - 32);
						}
						if (frame > 301)
						{
							p->variant = 0;
						}
						else if (frame > 245)
						{
							p->y = toFIXED(96);
						}
						else
						{
							p->y = toFIXED(192) - p->x;
						}
					}
				}
			}
			if (frame < JUDGMENT_SWEEP_RETURN)
			{
				for (i = 0; i != 8; i++)
				{
					Sint32 x = (i & 1) * 20 - (syRandom() & 15) + (i / 2) * 4 + shift / 2 - 16;
					Sint32 y = (i / 2) * 32 - (syRandom() & 15);

					draw[1](canvas,
					        state->sheet + ((frame / 2 + i) % 4) * JUDGMENT_EXPLOSION_PICTURE_BYTES
					            + JUDGMENT_SHEET_EXPLOSION,
					        x,
					        y,
					        JUDGMENT_EXPLOSION_WIDTH,
					        JUDGMENT_EXPLOSION_HEIGHT);
				}
			}
			else
			{
				for (i = 0; i != 8; i++)
				{
					Sint32 x = (i & 1) * 20 + (syRandom() & 15) - (i / 2) * 4 + shift / 2 - 16;
					Sint32 y = (i / 2) * 32 - (syRandom() & 15);

					draw[1](canvas,
					        state->sheet + ((frame / 2 + i) % 4) * JUDGMENT_EXPLOSION_PICTURE_BYTES
					            + JUDGMENT_SHEET_EXPLOSION,
					        x,
					        y,
					        JUDGMENT_EXPLOSION_WIDTH,
					        JUDGMENT_EXPLOSION_HEIGHT);
				}
			}
		}
		syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
		// The battle background shakes up and down.
		if (frame >= JUDGMENT_SWEEP_EXPLOSION)
		{
			Sint32 y          = syRandom() % 4 + 30;

			BgScrollWork[1].y = y;
		}
		EFFECT_END_TICK(state);
	}

	for (i = 0; i != JUDGMENT_DEBRIS_OBJECTS; i++)
	{
		fdSpriteDestroyStub(state->sprites[i]);
	}
	EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

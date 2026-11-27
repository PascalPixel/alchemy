// Frost. Part of battle_effect.c.

// The variant's row of FrostTimings, read by column (FROST_TIMING_MOTES, FROST_TIMING_COLUMNS,
// FROST_TIMING_BURST_TICK).
#define FROST_TIMING(variant, column) FrostTimings[(variant) * 3 + (column)]

// A row of FrostMoundSpots.
enum
{
	FROST_MOUND_X,   // canvas x of the mound's left edge
	FROST_MOUND_FOOT // canvas y of its foot
};

// Mound i's FROST_MOUND_X or FROST_MOUND_FOOT.
#define FROST_MOUND_SPOT(i, column) FrostMoundSpots[(i) * 2 + (column)]

// The ice mounds' pictures in IceChipSheet are all as wide; their heights are FrostMoundHeights.
#define FROST_MOUND_WIDTH           32

// The motes still drawn once they burst, of the variant's FROST_TIMING_MOTES.
#define FROST_BURST_MOTES           16
// How far below the centre the motes sink before the burst, in pixels; they are drawn only between
// this depth and as high above.
#define FROST_MOTE_DEPTH            48
// The ice chips thrown up at the burst; only the first FROST_CHIPS_DRAWN are drawn.
#define FROST_CHIPS                 32
#define FROST_CHIPS_DRAWN           24
// The sparkles that rise over the burst.
#define FROST_SPARKLES              32
// A sparkle's ticks: two for each of BattleEffectSparklePictures' nine, counted down.
#define FROST_SPARKLE_LIFE          18

// Motes of ice spiral down and in while mounds of ice spring up; at the burst the motes spread and
// rise, chips fly up and sparkles rise.
//
// Abilities: Frost, Tundra, Glacier.
void btEffectRunFrost(BATTLE_EFFECT_PARM *args)
{
	BATTLE_EFFECT_STATE *state;
	void                *canvas;
	Sint32               frame;
	Uint8               *sparkleSheet;
	FIXED                xyz[3];
	SCREEN_POINT         screen;
	FIXED                point[3];
	DRAW_RECTANGLE       pair[2];
	PARTICLE            *mote;
	Sint32               i;

	state        = RamHeapSlots->battleEffect;
	canvas       = RamHeapSlots->battleCanvas;
	sparkleSheet = RamHeapSlots->battleSheet;
	EFFECT_START_NARROW_CANVAS(BGCNT_PRIORITY(1));
	btResourceLoadAndDecompress(RESOURCE_ICE_CHIP_SHEET, state->sheet, 1, 1);
	btResourceLoadAndDecompress(RESOURCE_SPARKLE_DOTS, sparkleSheet, 0, 0);
	btEffectBuildCanvasBlitters(0, pair);

	// A mote: x holds its angle, z its radius, y its height.
	for (i = 0; i != ARRAY_COUNT(state->particles); i++)
	{
		PARTICLE *seed = &state->particles[i];

		seed->x        = syRandom() & TRIG_ANGLE_MASK;
		seed->z        = (syRandom() & 63) + 56;
		seed->y        = toFIXED((syRandom() & 31) - 64);
	}

	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
	if (args->side == EFFECT_SIDE_PARTY)
		REG_BG2X = -(NARROW_CANVAS_PARTY_X << 8);

	for (frame = 0; frame != FROST_TIMING(args->variant, FROST_TIMING_BURST_TICK) + 50; frame++)
	{
		Sint32 count;
		Sint32 lowest;
		Sint32 highest;

		lowest  = toFIXED(120);
		highest = 0;
		if (frame == FROST_TIMING(args->variant, FROST_TIMING_BURST_TICK) + 11)
			EFFECT_SHOW_DAMAGE(SE_IMPACT);
		SET_VECTOR(point, 0, 0, toFIXED(512));
		syInitMatrix();
		syMatrixTranslateVector(point);
		if (frame >= 36 && frame < 64 && (frame & 3) == 0)
			sySoundPlayStub(SE_ICE);
		if (frame == 85)
			sySoundPlayStub(SE_WHOOSH);
		EFFECT_FOR_EACH_TARGET(i, args)
		{
			if (frame == i * 4 + 40)
				btEffectTintBattler(
					args->targets[i], SPRITE_COLOR_BLUE_TINT, POSE_DAMAGE, TINT_KEEP, 0);
		}

		// Motes spiral in and sink.
		if (frame < FROST_TIMING(args->variant, FROST_TIMING_BURST_TICK))
			count = FROST_TIMING(args->variant, FROST_TIMING_MOTES);
		else
			count = FROST_BURST_MOTES;
		if (frame < FROST_TIMING(args->variant, FROST_TIMING_BURST_TICK) + 35)
		{
			for (i = 0; i != count; i++)
			{
				if (frame > i)
				{
					Sint32 picture = i % 8;

					mote           = &state->particles[i];
					if (mote->y < toFIXED(FROST_MOTE_DEPTH - i / 2)
					    && mote->y > -toFIXED(FROST_MOTE_DEPTH))
					{
						Uint32 width;
						Uint32 height;

						SET_VECTOR(
							xyz, mote->z * sySin(mote->x), mote->y, mote->z * syCos(mote->x));
						btEffectPosProjectPoint(xyz, &screen);
						screen.x = (FIXEDtoINT(screen.x) >> 1) + 64;
						screen.y = FIXEDtoINT(screen.y) + 60;
						// UB: width is assigned in one argument and read in another.
#if FIX_UNSEQUENCED_SIDE_EFFECT
						width  = FrostMoteWidths[picture];
						height = FrostMoteHeights[picture];
						pair[1](canvas,
						        state->sheet + FrostMoteCells[picture],
						        screen.x - (width >> 1),
						        screen.y - (height >> 1),
						        width,
						        height);
#else
						pair[1](canvas,
						        state->sheet + FrostMoteCells[picture],
						        screen.x - ((width = FrostMoteWidths[picture]) >> 1),
						        screen.y - ((height = FrostMoteHeights[picture]) >> 1),
						        width,
						        height);
#endif
					}
					if (frame < FROST_TIMING(args->variant, FROST_TIMING_BURST_TICK))
					{
						if (frame > i + 16)
						{
							if (mote->z > 4)
								mote->z -= 2;
							if (mote->y < toFIXED(FROST_MOTE_DEPTH))
								mote->y += toFIXED(5);
							mote->x += TRIG_TURN / 128;
						}
					}
					else
					{
						mote->z += 8;
						mote->y -= INTtoFIXED(i % 5 + 2);
						if (lowest > mote->y)
							lowest = mote->y;
						if (highest < mote->y)
							highest = mote->y;
					}
				}
			}
		}
		lowest  += toFIXED(64);
		highest += toFIXED(64);

		// Ice mounds spring up.
		if (frame < FROST_TIMING(args->variant, FROST_TIMING_BURST_TICK))
		{
			for (i = 0; i != FROST_TIMING(args->variant, FROST_TIMING_COLUMNS); i++)
			{
				if (i < (frame - 36) / 3)
				{
					Sint32 picture = i % 3;
					Uint32 height;

					if (frame >= FROST_TIMING(args->variant, FROST_TIMING_BURST_TICK) - 7)
					{
#if FIX_UNSEQUENCED_SIDE_EFFECT
						height = FrostMoundHeights[picture];
						pair[1](canvas,
						        state->sheet + FrostMoundPictures[picture],
						        FROST_MOUND_SPOT(i, FROST_MOUND_X),
						        FROST_MOUND_SPOT(i, FROST_MOUND_FOOT) - height,
						        FROST_MOUND_WIDTH,
						        height);
#else
						pair[1](canvas,
						        state->sheet + FrostMoundPictures[picture],
						        FROST_MOUND_SPOT(i, FROST_MOUND_X),
						        FROST_MOUND_SPOT(i, FROST_MOUND_FOOT)
						            - (height = FrostMoundHeights[picture]),
						        FROST_MOUND_WIDTH,
						        height);
#endif
					}
					else
					{
#if FIX_UNSEQUENCED_SIDE_EFFECT
						height = FrostMoundHeights[picture];
						pair[0](canvas,
						        state->sheet + FrostMoundPictures[picture],
						        FROST_MOUND_SPOT(i, FROST_MOUND_X),
						        FROST_MOUND_SPOT(i, FROST_MOUND_FOOT) - height,
						        FROST_MOUND_WIDTH,
						        height);
#else
						pair[0](canvas,
						        state->sheet + FrostMoundPictures[picture],
						        FROST_MOUND_SPOT(i, FROST_MOUND_X),
						        FROST_MOUND_SPOT(i, FROST_MOUND_FOOT)
						            - (height = FrostMoundHeights[picture]),
						        FROST_MOUND_WIDTH,
						        height);
#endif
					}
				}
			}
		}

		// Chips fly up at the burst. BUG: velocityX is never set, so a chip drifts sideways by
		// whatever word its PARTICLES slot last held in the map grid's cell buffer: the field's
		// cells, or an earlier effect's particles or pictures.
		if (frame == FROST_TIMING(args->variant, FROST_TIMING_BURST_TICK))
		{
			for (i = 0; i != FROST_CHIPS; i++)
			{
				PARTICLE *chip = &PARTICLES[i];

				SET_POSITION(chip,
				             toFIXED(syRandom() & 127),
				             INTtoFIXED((syRandom() & 15) + 80),
				             ((syRandom() & 63) - 32) << 12);
#if FIX_FROST_CHIP_DRIFT
				chip->velocityX = 0;
#endif
				chip->velocityY = ((-syRandom() & 15) - 16) << 13;
				chip->variant   = (syRandom() & 15) + 16;
			}
		}
		if (frame >= FROST_TIMING(args->variant, FROST_TIMING_BURST_TICK))
		{
			for (i = 0; i != FROST_CHIPS_DRAWN; i++)
			{
				PARTICLE *chip = &PARTICLES[i];

				if (chip->variant >= 0)
				{
					Sint32 picture = i % 8;

					pair[1](canvas,
					        state->sheet + FrostChipCells[picture],
					        FIXEDtoINT(chip->x),
					        FIXEDtoINT(chip->y),
					        FrostChipWidths[picture],
					        FrostChipHeights[picture]);
					chip->x += chip->velocityX;
					chip->y += chip->velocityY;
					chip->variant--;
				}
				if (lowest > chip->y)
					lowest = chip->y;
				if (highest < chip->y)
					highest = chip->y;
			}
		}
		lowest  = FIXEDtoINT(lowest);
		highest = FIXEDtoINT(highest);
		if (highest <= lowest)
			highest = lowest + 1;

		// Sparkles rise over the motes and chips.
		if (frame == FROST_TIMING(args->variant, FROST_TIMING_BURST_TICK))
		{
			for (i = 0; i != FROST_SPARKLES; i++)
			{
				PARTICLE *sparkle  = &state->particles[i];

				sparkle->velocityX = toFIXED(syRandom() & 127);
				// DEAD: never true, as highest was just kept above lowest; the ROM keeps the
				// compare and its arm.
				if (highest == lowest)
					sparkle->velocityY = toFIXED(lowest);
				else
					sparkle->velocityY = toFIXED(syRandom() % (highest - lowest) + lowest);
				sparkle->variant = (syRandom() & 15) + 20;
			}
		}
		if (frame >= FROST_TIMING(args->variant, FROST_TIMING_BURST_TICK))
		{
			Sint32 rise = (frame - FROST_TIMING(args->variant, FROST_TIMING_BURST_TICK)) / 2;

			for (i = 0; i != FROST_SPARKLES; i++)
			{
				PARTICLE *sparkle = &state->particles[i];

				if (sparkle->variant >= 0 && sparkle->variant < FROST_SPARKLE_LIFE)
				{
					Sint32 picture = (FROST_SPARKLE_LIFE - 1 - sparkle->variant) / 2;
					Uint32 size;

#if FIX_UNSEQUENCED_SIDE_EFFECT
					size = BattleEffectSparkleSizes[picture];
					pair[1](canvas,
					        sparkleSheet + BattleEffectSparklePictures[picture],
					        FIXEDtoINT(sparkle->velocityX) - (size >> 1),
					        FIXEDtoINT(sparkle->velocityY) - (size >> 1) - rise,
					        size,
					        size);
#else
					pair[1](canvas,
					        sparkleSheet + BattleEffectSparklePictures[picture],
					        FIXEDtoINT(sparkle->velocityX)
					            - ((size = BattleEffectSparkleSizes[picture]) >> 1),
					        FIXEDtoINT(sparkle->velocityY) - (size >> 1) - rise,
					        size,
					        size);
#endif
				}
				sparkle->variant--;
				if ((sparkle->variant == -1 || sparkle->variant == FROST_SPARKLE_LIFE - 1)
				    && frame < FROST_TIMING(args->variant, FROST_TIMING_BURST_TICK) + 35)
				{
					sparkle->variant   = FROST_SPARKLE_LIFE - 1;
					sparkle->velocityX = toFIXED(syRandom() & 127);
					sparkle->velocityY = toFIXED(syRandom() % (highest - lowest) + lowest);
				}
			}
		}
		EFFECT_PRESENT_CANVAS(state);
	}

	EFFECT_RELEASE_BLITTERS();
	syCloseEvent(btEffectTransferCanvas);
	btEffectEndCanvasLayer();
}

#undef FROST_TIMING
#undef FROST_MOUND_SPOT
#undef FROST_MOUND_WIDTH
#undef FROST_BURST_MOTES
#undef FROST_MOTE_DEPTH
#undef FROST_CHIPS
#undef FROST_CHIPS_DRAWN
#undef FROST_SPARKLES
#undef FROST_SPARKLE_LIFE

// Ray. Part of battle_effect.c.

#define RAY_TICKS             64                  // ticks of the bolts' approach

#define RAY_SPARK_COUNT       PARTICLE_POOL_COUNT // sparks in PARTICLES

#define RAY_PICTURES          GLOW_SHEET_ROOM     // where the bolt pictures start in the sheet
#define RAY_COUNT             4                   // bolts
// The bolts turn a 32nd of a turn a tick, each a quarter turn more than the one before.
#define RAY_BOLT_TURN         (TRIG_TURN / 32)
// The strongest variant shows the canvas twice as wide, scrolled this many canvas pixels toward the
// targets' side.
#define RAY_WIDE_CANVAS_SHIFT 16
// Where the canvas is shown over targets on the party's side: from screen x 128.
#define RAY_PARTY_CANVAS_X    128
// The column of a RaySparks row that says how many sparks each bolt throws a tick; nothing reads
// the first.
#define RAY_SPARKS_PER_BOLT   1

// Four bolts close in, turning towards the centre and scattering sparks from their roots.
//
// Abilities: Ray, Storm Ray, Destruct Ray.
void btEffectRunRay(BATTLE_EFFECT_PARM *args)
{
	BATTLE_EFFECT_STATE *state;
	void                *canvas;
	Sint32               frame;
	Uint8               *motes;
	Sint32               bolt;
	Sint32               thrown;
	BLITTER_PAIR         draw;
	Sint32               i;

	state       = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	canvas      = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
	motes       = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
	state->args = args;
	btEffectStartTiledCanvas(1);
	if (state->args->variant == EFFECT_VARIANT_STRONGEST)
		REG_BG2PA = Q_8_8(0.5);
	else
		REG_BG2PA = Q_8_8(1);
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, COMPOSITE_LIGHTEN);
	draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_ADDITIVE);
	draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
	btResourceLoadAndDecompress(RESOURCE_LIGHTNING_BOLT_SHEET, state->sheet + RAY_PICTURES, 1, 1);
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, motes, 0, 0);
	if (state->args->variant == EFFECT_VARIANT_STRONGEST)
	{
		if (state->args->side == EFFECT_SIDE_PARTY)
			REG_BG2X = -(RAY_WIDE_CANVAS_SHIFT << 8);
		else
			REG_BG2X = RAY_WIDE_CANVAS_SHIFT << 8;
	}
	else
	{
		if (state->args->side == EFFECT_SIDE_PARTY)
			REG_BG2X = -(RAY_PARTY_CANVAS_X << 8);
	}

	for (i = 0; i != RAY_SPARK_COUNT; i++)
	{
		PARTICLE *spark = &PARTICLES[i];
		Sint32    speed;
		Sint32    angle;

		speed = (syRandom() & 0x3ff) + 256;
		angle = (syRandom() & 0x7fff) - DEGtoANG(90);
		SET_POSITION(spark, 64 << 8, 112 << 8, FIXEDtoINT(sySin(angle) * speed));
		spark->velocityZ = FIXEDtoINT(-(syCos(angle) * speed * 2));
		spark->variant   = 0;
	}

	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
	sySoundPlayStub(SE_ITEM_BREAK);

	for (frame = 0; frame != RAY_TICKS; frame++)
	{
		if (frame == 20)
			EFFECT_SHOW_DAMAGE(SE_IMPACT);
		// The bolts, over the first 16 ticks.
		if (frame < 16)
		{
			if (frame % 5 == 2)
				syWordsFill(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(16));
			for (bolt = 0; bolt != RAY_COUNT; bolt++)
			{
				Sint32 angle;
				Sint32 x;
				Sint32 y;

				thrown = 0;
				angle  = (frame * RAY_BOLT_TURN + DEGtoANG(90)) * bolt;
				x      = FIXEDtoINT((32 - frame) * sySin(angle)) + 64;
				y      = -FIXEDtoINT(syCos(angle) * 8) - 8;
				if (state->args->variant == EFFECT_VARIANT_BASIC)
					draw[0](canvas,
					        state->sheet + (syRandom() & 3) * LIGHTNING_BOLT_PICTURE_BYTES
					            + RAY_PICTURES,
					        x + (syRandom() & 7) - 16,
					        y,
					        LIGHTNING_BOLT_WIDTH,
					        LIGHTNING_BOLT_HEIGHT);
				else
					draw[bolt & 1](canvas,
					               state->sheet + (syRandom() & 3) * LIGHTNING_BOLT_PICTURE_BYTES
					                   + RAY_PICTURES,
					               x + (syRandom() & 7) - 16,
					               y,
					               LIGHTNING_BOLT_WIDTH,
					               LIGHTNING_BOLT_HEIGHT);

				// Sparks fly from the bolt's root.
				for (i = 0; i != RAY_SPARK_COUNT; i++)
				{
					PARTICLE *spark = &PARTICLES[i];

					if (spark->variant == 0)
					{
						Sint32 speed;
						Sint32 spread;

						speed            = (syRandom() & 0x1ff) + 128;
						spread           = (syRandom() & 0x7fff) - DEGtoANG(90);
						spark->x         = toFIXED(x);
						spark->y         = INTtoFIXED(y + 112);
						spark->velocityX = (sySin(spread) * speed) >> 9;
						spark->velocityY = -(syCos(spread) * speed * 2) >> 7;
						spark->variant   = (syRandom() & 7) + 32;
						thrown++;
						if (thrown == RaySparks[state->args->variant][RAY_SPARKS_PER_BOLT])
							break;
					}
				}
			}
			state->shakeFrame = 1;
		}

		// Sparks bounce off the bottom.
		for (i = 0; i != RAY_SPARK_COUNT; i++)
		{
			PARTICLE *spark = &PARTICLES[i];

			if (spark->variant > 0)
			{
				spark->variant--;
				btParticleAdvanceWithGravity2D(spark, 60, -toFIXED(1) / 32);
				if (spark->y > toFIXED(120))
				{
					spark->velocityY = -spark->velocityY / 2;
				}
				else if (spark->x >= 0 && spark->x < toFIXED(127) && spark->y >= 0)
				{
					Sint32 x    = FIXEDtoINT(spark->x);
					Sint32 y    = FIXEDtoINT(spark->y);
					Sint32 size = spark->variant / 8 + 1;

					EFFECT_DRAW_MOTE(draw[0], canvas, motes, x, y, size);
				}
			}
		}

		// The targets are hit 4 ticks apart.
		if (frame >= 4 && frame < 96)
		{
			EFFECT_FOR_EACH_TARGET(i, state->args)
			{
				if (frame == i * 4 + 4)
					btEffectTintBattler(
						state->args->targets[i], SPRITE_COLOR_WHITE, POSE_DAMAGE, i, 10);
			}
		}
		btCameraShake(2, 4);
		EFFECT_END_TICK(state);
	}

	EFFECT_FINISH_CANVAS();
}

#undef RAY_SPARK_COUNT
#undef RAY_PICTURES
#undef RAY_COUNT
#undef RAY_BOLT_TURN
#undef RAY_WIDE_CANVAS_SHIFT
#undef RAY_PARTY_CANVAS_X
#undef RAY_TICKS
#undef RAY_SPARKS_PER_BOLT

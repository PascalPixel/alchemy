/* Bolt, Flash Bolt and Blue Bolt: lightning strikes each target in turn. Part
   of battle_effect.c. */

#define BOLT_INTERVAL		   8 /* frames between one target's bolt and the next's */
/* The run of PARTICLES each target's sparks are thrown from. */
#define BOLT_SPARKS_PER_TARGET 128
/* The canvas line where the bolt ends and the sparks bounce. */
#define BOLT_GROUND_Y		   104
/* The level the canvas flashes to as each bolt starts. */
#define BOLT_FLASH_LEVEL	   16
/* The frames a spark keeps each size as it burns out. */
#define BOLT_SPARK_SIZE_FRAMES 16

/* The second blitter, made anew for each glint with its random flip. */
#define BOLT_GLINT_BLITTER (Ram_HeapSlots->ramCodePaired)

/* The columns of a Bolt_Counts row, one row a variant. */
enum {
	/* sparks thrown from each bolt's foot, at most the BOLT_SPARKS_PER_TARGET
	   of its target's run */
	BOLT_SPARKS,
	BOLT_GLINTS,	   /* glints a frame around each target */
	BOLT_GLINT_SPREAD, /* how high and wide the glints spread, in pixels */
	BOLT_STRIKES,	   /* bolts drawn over each other */
	BOLT_COUNT_COLUMNS
};

/* A variant's count in column column of Bolt_Counts. */
#define BOLT_COUNT(variant, column) \
	Bolt_Counts[(variant) * BOLT_COUNT_COLUMNS + (column)]

/* Where the bolt pictures start in the sprite sheet, after the six glints. */
#define BOLT_PICTURES GLINT_SET_BYTES

/* A bolt of lightning strikes each target in turn, and sparks fly up from where
   it lands while glints flicker there.

   Abilities: Bolt, Flash Bolt, Blue Bolt. */
void btEffectRunBolt(BATTLE_EFFECT_PARM *args)
{
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	Sint32				 i;
	Sint32				 frame;
	/* LEFTOVER(frame): draw[1], the pair's idle second blitter, which the ROM's
	 * frame reserves and never touches; as one DRAW_RECTANGLE. */
	BLITTER_PAIR draw;
	Uint8		*sprites;
	SCREEN_POINT screen;
	Sint32		 j;

	state = Ram_HeapSlots->battleEff;
	canvas = Ram_HeapSlots->battleCanvas;
	sprites = Ram_HeapSlots->battleSheet;
	btEffectStartCanvasLayer(BGCNT_PRIORITY(1));
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
	draw[0] = Ram_HeapSlots->ramCode;
	/* The tornado sprite sheet only for its glints. */
	btResourceLoadAndDecompress(RESOURCE_TORNADO_SHEET, state->sheet, 1, 0);
	btResourceLoadAndDecompress(
		RESOURCE_LIGHTNING_BOLT_SHEET, state->sheet + BOLT_PICTURES, 1, 1);
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sprites, 0, 0);
	for(j = 0; j != PARTICLE_POOL_COUNT; j++)
		PARTICLES[j].variant = 0;
	EFFECT_FREE_PARTICLES(j, state->particles, 64);
	EFFECT_START_TRANSFER(
		state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
	sySndPlayStub(SE_ITEM_BREAK);

	for(frame = 0; frame != args->tgtCnt * BOLT_INTERVAL + 40; frame++) {
		if(frame == 24)
			EFFECT_SHOW_DAMAGE(SE_IMPACT);

		/* The canvas flashes as each target's turn starts. */
		EFFECT_FOR_EACH_TARGET(i, args) {
			if(frame == i * BOLT_INTERVAL)
				syWordsFillEntry(
					canvas, CANVAS_BYTES, CANVAS_FILL_WORD(BOLT_FLASH_LEVEL));
		}

		EFFECT_FOR_EACH_TARGET(i, args) {
			Sint32 start = i * BOLT_INTERVAL;

			btEffectPosProjectBattler(args->tgts[i], &screen);
			screen.x /= 2;
			if(frame == start + 1)
				state->shakeFrm = 4;
			if(frame == start + 4) {
				EFFECT_STRIKE_TARGET(args, i, 6, KNOCKBACK_BOUNCE);
			}

			/* The bolt drops from the top, 64 pixels a frame, to the ground. */
			if(frame >= start && frame < start + 16) {
				Sint32 height = (frame - start) << 6;

				if(height > BOLT_GROUND_Y)
					height = BOLT_GROUND_Y;
				for(j = 0; j != BOLT_COUNT(args->variant, BOLT_STRIKES); j++) {
					draw[0](canvas,
						state->sheet +
							(((i + frame + j) / 2) & 3) *
								LIGHTNING_BOLT_PICTURE_BYTES +
							BOLT_PICTURES,
						screen.x - LIGHTNING_BOLT_WIDTH / 2, 0,
						LIGHTNING_BOLT_WIDTH, height);
				}

				/* Sparks fly up from its foot, within 90 degrees of straight
				   up. */
				if(frame == start + 2) {
					for(j = 0; j != BOLT_COUNT(args->variant, BOLT_SPARKS);
						j++) {
						PARTICLE *spark =
							&PARTICLES[i * BOLT_SPARKS_PER_TARGET + j];
						Sint32 speed = (syRandom() & 0x1ff) + 64;
						Sint32 angle = (syRandom() & 0x7fff) - DEGtoANG(90);

						spark->x = toFIXED(screen.x);
						spark->y = toFIXED(BOLT_GROUND_Y);
						spark->velX = (sySin(angle) * speed) >> 5;
						spark->velY = -(syCos(angle) * speed) >> 6;
						spark->variant = (syRandom() & 7) + 32;
					}
				}
			}

			/* Glints flicker at the foot, each at a random place with a random
			   flip. */
			if(frame >= start + 2 && frame < start + 24) {
				for(j = 0; j != BOLT_COUNT(args->variant, BOLT_GLINTS); j++) {
					Sint32 kind = j & 3;
					Sint32 drop = syRandom() %
						BOLT_COUNT(args->variant, BOLT_GLINT_SPREAD);
					Sint32 spread =
						BOLT_COUNT(args->variant, BOLT_GLINT_SPREAD) - drop + 1;
					Sint32 y = screen.y - drop -
						(BattleEffect_GlintPictureHeights[kind] >> 1) + 8;
					Uint32 random = syRandom();
					Sint32 x = screen.x;
					Uint32 flip;

					x += random % spread;
					x -= spread / 2;
					x -= BattleEffect_GlintPictureWidths[kind] >> 1;
					flip = syRandom();
					EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
						Bolt_GlintFlips[flip & 3] | EFFECT_CLIP_BOUNDS,
						Bolt_GlintModes[args->variant]);
					BOLT_GLINT_BLITTER(canvas,
						state->sheet + BattleEffect_GlintPictures[kind], x, y,
						BattleEffect_GlintPictureWidths[kind],
						BattleEffect_GlintPictureHeights[kind]);
					syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
				}
			}
		}

		/* Sparks fall, bounce off the ground at half speed, and shrink as they
		   burn out. */
		for(j = 0; j != PARTICLE_POOL_COUNT; j++) {
			PARTICLE *spark = &PARTICLES[j];

			if(spark->variant > 0) {
				spark->variant--;
				btParticleAdvanceWithGravity2D(spark, 60, toFIXED(1) / 16);
				if(spark->y > toFIXED(BOLT_GROUND_Y)) {
					spark->velY = -spark->velY / 2;
				} else if((spark->x >= 0 && spark->x < toFIXED(127)) &&
					spark->y >= 0) {
					Sint32 size = spark->variant / BOLT_SPARK_SIZE_FRAMES + 1;
					Sint32 x = FIXEDtoINT(spark->x);
					Sint32 y = FIXEDtoINT(spark->y);

					EFFECT_DRAW_MOTE(draw[0], canvas, sprites, x, y, size);
				}
			}
		}
		btCameraShake(2, 8);
		EFFECT_END_FRAME(state);
	}
	EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

#undef BOLT_INTERVAL
#undef BOLT_SPARKS_PER_TARGET
#undef BOLT_GROUND_Y
#undef BOLT_FLASH_LEVEL
#undef BOLT_SPARK_SIZE_FRAMES
#undef BOLT_GLINT_BLITTER
#undef BOLT_COUNT
#undef BOLT_PICTURES

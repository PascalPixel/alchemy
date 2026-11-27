/* Bolt. Part of battle_effect.c. */

/* The second blitter, made anew for each glint with its random flip. */
#define BOLT_GLINT_BLITTER EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED)

/* The columns of a BoltCounts row. */
enum {
	/* sparks thrown from each bolt's foot, at most BOLT_SPARKS_PER_TARGET */
	BOLT_SPARKS,
	BOLT_GLINTS,	   /* glints a tick around each target */
	BOLT_GLINT_SPREAD, /* how high and wide the glints spread, in pixels */
	BOLT_STRIKES	   /* bolts drawn over each other */
};

/* Where the bolt pictures start in the sprite sheet, after the six glints. */
#define BOLT_PICTURES			GLINT_SET_BYTES
/* The targets are struck this many ticks apart. */
#define BOLT_INTERVAL			8
/* Each target's run of PARTICLES for its sparks. */
#define BOLT_SPARKS_PER_TARGET	128
/* The canvas row the bolts reach down to and the sparks bounce off. */
#define BOLT_GROUND_Y			104
/* A spark is drawn a size smaller for every this many ticks less it has to
   live. */
#define BOLT_SPARK_SHRINK_TICKS 16

/* A bolt of lightning strikes each target in turn, and sparks fly up from where
   it lands while glints flicker there.

   Abilities: Bolt, Flash Bolt, Blue Bolt. */
void btEffectRunBolt(BATTLE_EFFECT_PARM *args)
{
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	Sint32				 i;
	Sint32				 frm;
	/* LEFTOVER(frame): draw[1], the idle half of the pair. */
	BLITTER_PAIR draw;
	Uint8		*sprs;
	SCREEN_POINT screen;
	Sint32		 j;

	state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
	sprs = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
	state->args = args;
	btEffectStartCanvasLayer(BGCNT_PRIORITY(1));
	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, COMPOSITE_LIGHTEN);
	draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
	/* The tornado sprite sheet only for its glints. */
	btResourceLoadAndDecompress(RESOURCE_TORNADO_SHEET, state->sheet, 1, 0);
	btResourceLoadAndDecompress(
		RESOURCE_LIGHTNING_BOLT_SHEET, state->sheet + BOLT_PICTURES, 1, 1);
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sprs, 0, 0);
	for(j = 0; j != PARTICLE_POOL_COUNT; j++)
		PARTICLES[j].variant = 0;
	EFFECT_FREE_PARTICLES(j, state->particles, 64);
	EFFECT_START_TRANSFER(
		state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
	sySndPlayStub(SE_ITEM_BREAK);
	for(frm = 0; frm != state->args->tgtCnt * BOLT_INTERVAL + 40; frm++) {
		if(frm == 24)
			EFFECT_SHOW_DAMAGE(SE_IMPACT);
		/* The canvas flashes as each target's turn starts. */
		EFFECT_FOR_EACH_TARGET(i, state->args) {
			if(frm == i * BOLT_INTERVAL)
				syWordsFill(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(16));
		}
		EFFECT_FOR_EACH_TARGET(i, state->args) {
			Sint32 start = i * BOLT_INTERVAL;

			btProjectionUnit(state->args->tgts[i], &screen);
			screen.x /= 2;
			if(frm == start + 1)
				state->shakeFrm = 4;
			if(frm == start + 4) {
				btEffectTintBattler(state->args->tgts[i], SPRITE_COLOR_WHITE,
					POSE_DAMAGE, i, 6);
				btMotionApplyKnockbackStub(
					state->args->tgts[i], KNOCKBACK_BOUNCE);
			}

			/* The bolt drops from the top. */
			if(frm >= start && frm < start + 16) {
				Sint32 height = (frm - start) << 6;

				if(height > BOLT_GROUND_Y)
					height = BOLT_GROUND_Y;
				for(j = 0; j != BoltCounts[state->args->variant][BOLT_STRIKES];
					j++) {
					draw[0](canvas,
						state->sheet +
							(((i + frm + j) / 2) & 3) *
								LIGHTNING_BOLT_PICTURE_BYTES +
							BOLT_PICTURES,
						screen.x - LIGHTNING_BOLT_WIDTH / 2, 0,
						LIGHTNING_BOLT_WIDTH, height);
				}

				/* Sparks fly up from its foot. */
				if(frm == start + 2) {
					for(j = 0;
						j != BoltCounts[state->args->variant][BOLT_SPARKS];
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

			/* Glints flicker at the foot. */
			if(frm >= start + 2 && frm < start + 24) {
				for(j = 0; j != BoltCounts[state->args->variant][BOLT_GLINTS];
					j++) {
					Sint32 kind = j & 3;
					Sint32 drop = syRandom() %
						BoltCounts[state->args->variant][BOLT_GLINT_SPREAD];
					Sint32 spread =
						BoltCounts[state->args->variant][BOLT_GLINT_SPREAD] -
						drop + 1;
					Sint32 y = screen.y - drop -
						(BattleEffectGlintPictureHeights[kind] >> 1) + 8;
					Uint32 random = syRandom();
					Sint32 x = screen.x;

					x += random % spread;
					x -= spread / 2;
					x -= (BattleEffectGlintPictureWidths[kind] >> 1);

					EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
						BoltGlintFlips[syRandom() & 3] | EFFECT_CLIP_BOUNDS,
						BoltGlintModes[state->args->variant]);
					BOLT_GLINT_BLITTER(canvas,
						state->sheet + BattleEffectGlintPictures[kind], x, y,
						BattleEffectGlintPictureWidths[kind],
						BattleEffectGlintPictureHeights[kind]);
					syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
				}
			}
		}

		/* Sparks fall, bounce and shrink as they die. */
		for(j = 0; j != PARTICLE_POOL_COUNT; j++) {
			PARTICLE *spark = &PARTICLES[j];

			if(spark->variant > 0) {
				spark->variant--;
				btParticleAdvanceWithGravity2D(spark, 60, toFIXED(1) / 16);
				if(spark->y > toFIXED(BOLT_GROUND_Y)) {
					spark->velY = -spark->velY / 2;
				} else if((spark->x >= 0 && spark->x < toFIXED(127)) &&
					spark->y >= 0) {
					Sint32 size = spark->variant / BOLT_SPARK_SHRINK_TICKS + 1;
					Sint32 x = FIXEDtoINT(spark->x);
					Sint32 y = FIXEDtoINT(spark->y);

					EFFECT_DRAW_MOTE(draw[0], canvas, sprs, x, y, size);
				}
			}
		}
		btCameraShake(2, 8);
		EFFECT_END_TICK(state);
	}
	EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

#undef BOLT_GLINT_BLITTER
#undef BOLT_PICTURES
#undef BOLT_INTERVAL
#undef BOLT_SPARKS_PER_TARGET
#undef BOLT_GROUND_Y
#undef BOLT_SPARK_SHRINK_TICKS

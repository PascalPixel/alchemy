/* Raging Heat and Juggle. Part of battle_effect.c. */

/* The ring of fire's texture, 128x128, after the quarter of the circle of runes
   (64x64) that the pillar's picture then replaces. */
#define FIRE_RING_PIXELS		(Ram_MapMetatileBuffer + 64 * 64)
/* The embers, after the ring of fire's texture. */
#define EMBERS					((PARTICLE *)(FIRE_RING_PIXELS + 128 * 128))
#define RAGING_HEAT_EMBER_COUNT 256

#define RAGING_HEAT_TICKS				 96
/* The pillar starts to fall and the ring of fire to spread. */
#define RAGING_HEAT_PILLAR_TICK			 56
/* FlashBurstSheet's pictures, after the circle of runes (128x128) in the
   effect's sprite sheet. */
#define RAGING_HEAT_FLASH_BURST_PICTURES (128 * 128)
/* The bursts the pillar sets out where it lands; the variant draws the first
   RAGING_HEAT_BURSTS of them. */
#define RAGING_HEAT_BURSTS_SET_OUT		 32
/* A burst shows each of FlashBurstSheet's six pictures for four ticks. */
#define RAGING_HEAT_BURST_TICKS			 24
/* The rings' tilt toward the viewer. */
#define RAGING_HEAT_TILT				 2000
/* Until this tick the circle of runes is tipped further and rights itself. */
#define RUNE_CIRCLE_RIGHTED_TICK		 48

/* RagingHeatStyles[variant]: four bytes for each strength. */
enum {
	RAGING_HEAT_BURSTS,	   /* bursts of fire drawn where the pillar lands */
	RAGING_HEAT_SPREAD,	   /* how far apart they fly */
	RAGING_HEAT_RING_SIZE, /* the rings' size, in 64ths of their textures' */
	RAGING_HEAT_EMBERS	   /* embers that fly from the landing */
};

/* A circle of runes spins over the targets' middle and swells; from tick 56 a
   pillar of flame (two mirrored halves) falls from it onto them and bursts into
   fire and embers where it lands, under a ring of fire that spreads and fades.

   Abilities: Raging Heat. */
void btEffectRunRagingHeat(BATTLE_EFFECT_PARM *args)
{
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	Sint32				 frm;
	BLITTER_PAIR		 draw;
	Uint8				*sprs;
	SCREEN_POINT		 tgts[8];
	MODEL_TEXTURE		 texture;
	Uint8				*buf;
	Sint32				 centerX;
	Sint32				 baseY;
	RENDER_MODEL		*model;
	PARTICLE			*burst;
	PARTICLE			*particle;
	Sint32				 i;

	buf = Ram_MapMetatileBuffer;
	state = Ram_HeapSlots->battleEff;
	canvas = Ram_HeapSlots->battleCanvas;
	sprs = Ram_HeapSlots->battleSheet;
	btEffectStartCanvasLayer(BGCNT_PRIORITY(1));
	REG_BLDALPHA = BLEND_ALPHA(16, 10);
	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
	draw[0] = Ram_HeapSlots->ramCode;
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
		EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_ADDITIVE);
	draw[1] = Ram_HeapSlots->ramCodePaired;
	for(i = 0; i != args->tgtCnt; i++) {
		btEffectPosProjectScaledBattler(args->tgts[i], &tgts[i]);
	}
	centerX = tgts[0].x + (tgts[args->tgtCnt - 1].x - tgts[0].x) / 2;
	baseY = tgts[0].y - 48;
	btResourceLoadAndDecompress(RESOURCE_CORNER_RUNE_B_SHEET, buf, 1, 1);
	btPictureMirrorIntoQuadrants(buf, state->sheet, 64, 64);
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sprs, 0, 0);
	btResourceLoadAndDecompress(RESOURCE_FLASH_BURST_SHEET,
		state->sheet + RAGING_HEAT_FLASH_BURST_PICTURES, 1, 0);
	btResourceLoadAndDecompress(RESOURCE_PINK_BURST_SHEET, buf, 1, 0);
	btResourceLoadAndDecompress(
		RESOURCE_FIRE_RING_TEXTURE, FIRE_RING_PIXELS, 0, 0);
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

	for(frm = 0; frm != RAGING_HEAT_TICKS; frm++) {
		if(frm == 0) {
			for(i = 0; i != ARRAY_COUNT(state->particles); i++) {
				state->particles[i].variant = RAGING_HEAT_BURST_TICKS;
			}
			EFFECT_FREE_PARTICLES(i, EMBERS, RAGING_HEAT_EMBER_COUNT);
			sySndPlayStub(SE_SPIN);
		}
		/* The colours turn to the orange ramp over the 16 ticks before the
		   fall. */
		if((Uint32)(frm - (RAGING_HEAT_PILLAR_TICK - 16)) < 16) {
			btEffectStepPaletteToResource(RESOURCE_ORANGE_PALETTE_B);
		}
		if(frm == RAGING_HEAT_PILLAR_TICK) {
			sySndPlayStub(SE_CLATTER);
		}

		/* The pillar falls eight lines a tick and bursts where it lands. */
		if(frm >= RAGING_HEAT_PILLAR_TICK) {
			Sint32 y = baseY + frm * 8 - 480;

			if((Uint32)(y - 52) <= 7) {
				for(i = 0, burst = state->particles;
					i != RAGING_HEAT_BURSTS_SET_OUT; i++, burst++) {
					Uint32 random;
					Uint32 spread;

					burst->variant = -i / 4 + 2;
					burst->x = toFIXED(centerX / 2);
					burst->y = INTtoFIXED((syRandom() & 15) + y + 16);
					random = syRandom();
					spread =
						RagingHeatStyles[args->variant][RAGING_HEAT_SPREAD];
					burst->velX = ((random % spread) - (spread >> 1)) << 12;
					burst->velY = (192 - (syRandom() & 255)) << 10;
				}
				for(i = 0;
					i != RagingHeatStyles[args->variant][RAGING_HEAT_EMBERS];
					i++) {
					Sint32 ang;
					Sint32 speed;

					EMBERS[i].x = toFIXED(centerX / 2);
					EMBERS[i].y = INTtoFIXED((syRandom() & 15) + y + 24);
					ang = (syRandom() & 0x7fff) + DEGtoANG(90);
					speed = (syRandom() & 255) + 127;
					EMBERS[i].velX = (sySin(ang) * speed) >> 5;
					EMBERS[i].velY = (syCos(ang) * speed) >> 5;
					EMBERS[i].variant = (syRandom() & 15) + 16;
				}
				EFFECT_SHOW_DAMAGE(SE_CRASH);
				for(i = 0; i != args->tgtCnt; i++) {
					btMotionApplyKnockbackStub(args->tgts[i], KNOCKBACK_NUDGE);
					btEffectTintBattler(
						args->tgts[i], SPRITE_COLOR_WHITE, POSE_DAMAGE, i, 16);
				}
				state->shakeFrm = 16;
			}
			if(y <= 52) {
				draw[0](canvas, buf, centerX / 2 - 24, y, 24, 64);
				draw[1](canvas, buf, centerX / 2, y, 24, 64);
			}
			i = 0;
			/* FAKEMATCH(dead): the test repeats the loop's own. */
			if(i != RagingHeatStyles[args->variant][RAGING_HEAT_BURSTS]) {
				for(particle = state->particles;
					i != RagingHeatStyles[args->variant][RAGING_HEAT_BURSTS];
					i++, particle++) {
					if((Uint32)particle->variant < RAGING_HEAT_BURST_TICKS) {
						if(particle->variant > 1) {
							draw[0](canvas,
								state->sheet +
									RAGING_HEAT_FLASH_BURST_PICTURES +
									(particle->variant / 4) *
										PICTURE_BYTES(FLASH_BURST),
								FIXED_INTEGER_PART(particle->x) -
									FLASH_BURST_WIDTH / 2,
								FIXED_INTEGER_PART(particle->y) -
									FLASH_BURST_HEIGHT / 2,
								FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
						}
						btParticleAdvanceWithGravity2D(
							particle, 60, -toFIXED(1) / 16);
					}
					particle->variant++;
				}
			}
			i = 0;
			if(i != RagingHeatStyles[args->variant][RAGING_HEAT_EMBERS]) {
				particle = EMBERS;
				do {
					if(particle->variant >= 0) {
						Sint32 size = (particle->variant >> 3) + 1;

						EFFECT_DRAW_MOTE(draw[0], canvas, sprs,
							FIXED_INTEGER_PART(particle->x),
							FIXED_INTEGER_PART(particle->y), size);
						btParticleAdvanceWithGravity2D(
							particle, 60, toFIXED(1) / 16);
						particle->variant--;
					}
					i++;
					particle++;
				} while(
					i != RagingHeatStyles[args->variant][RAGING_HEAT_EMBERS]);
			}
		}

		/* The circle of runes, and from the pillar's fall the ring of fire. */
		{
			void  *vertices;
			Sint32 shade;

			vertices = syHeapAllocEWRAM(4 * sizeof(PROJECTED_VERTEX));
			model = btRenderAllocateModelList(1);
			shade = 0;
			model->kind = MODEL_SHADED_ADDITIVE;
			model->texture = &texture;
			model->mesh = BattleEffectLargeSquareMesh;
			model->vertices = vertices;
			model->shade = shade;
			texture.logWidth = 7;
			texture.logHeight = 7;
			for(i = 0; i != 2; i++) {
				Sint32 size =
					RagingHeatStyles[args->variant][RAGING_HEAT_RING_SIZE];
				Sint32 largest = size * (toFIXED(1) / 64);
				Sint32 step = size << 5;
				Sint32 scale;

				syInitMatrix();
				syScale(toFIXED(1) / 2, toFIXED(1), toFIXED(1) / 2);
				if(i == 0) {
					syTranslate(toFIXED(centerX) - toFIXED(128),
						toFIXED(baseY) - toFIXED(64), 0);
				} else {
					syTranslate(toFIXED(centerX) - toFIXED(128),
						INTtoFIXED(baseY + 8) - toFIXED(64), 0);
				}
				syRotX(RAGING_HEAT_TILT);
				if(frm < RUNE_CIRCLE_RIGHTED_TICK) {
					syRotX((RUNE_CIRCLE_RIGHTED_TICK - frm) << 9);
				}
				if(i == 0) {
					scale = (frm - 16) * step;
					if(scale > largest) {
						scale = largest;
					}
					if(frm > 60) {
						scale = largest - step * (frm * 2 - 120);
					}
					texture.pixels = state->sheet;
					syRotY(-frm * 3 << 8);
				} else {
					scale = (frm - RAGING_HEAT_PILLAR_TICK) * 4 * step;
					if(scale < 0) {
						continue;
					}
					scale += toFIXED(1) / 4;
					shade = 512 - frm * 8;
					model->shade = shade;
					texture.pixels = FIRE_RING_PIXELS;
				}
				if(scale > 0 && shade > MODEL_SHADE_INVISIBLE) {
					syMatrixScaleUniform(scale);
					btRenderTransformVertices(
						BattleEffectGroundSquareCorners, vertices, 4);
					btRenderDrawModelList(model);
				}
			}
			syHeapRelease(model);
			syHeapRelease(vertices);
		}
		btCameraShake(8, 8);
		EFFECT_END_TICK(state);
	}
	EFFECT_FINISH_CANVAS();
}

#undef RAGING_HEAT_EMBER_COUNT
#undef RAGING_HEAT_PILLAR_TICK
#undef RAGING_HEAT_FLASH_BURST_PICTURES
#undef RAGING_HEAT_BURSTS_SET_OUT
#undef RAGING_HEAT_BURST_TICKS
#undef RAGING_HEAT_TILT
#undef RUNE_CIRCLE_RIGHTED_TICK

#define JUGGLE_TICKS			 122
#define JUGGLE_RING_RISE_TICK	 8 /* the ring starts to rise over the caster */
#define JUGGLE_RING_SOUND_TICK	 16 /* the launch is heard */
#define JUGGLE_RING_THROW_TICK	 64 /* it flies at the targets */
#define JUGGLE_RING_HIT_TICK	 88 /* it strikes; sparks burst */
#define JUGGLE_SPARKS_PER_TARGET 128
#define JUGGLE_SPARK_COUNT		 768
/* The ring's tilt toward the viewer, beside its turn. */
#define JUGGLE_TILT				 500
/* SmallFlameSheet's four pictures of a flame. */
#define JUGGLE_FLAME_WIDTH		 16
#define JUGGLE_FLAME_HEIGHT		 64

/* JuggleStyles[variant]: seven bytes for each strength. */
enum {
	JUGGLE_RING_LIFT_STEP,	   /* pixels the ring rises each tick */
	JUGGLE_RING_LIFT_MAX,	   /* and its highest */
	JUGGLE_RING_FLAMES,		   /* flames in the ring */
	JUGGLE_RING_FLAME_SPACING, /* their spacing round it */
	JUGGLE_RING_SPARKS,		   /* sparks at each target */
	JUGGLE_RING_SHAKE,		   /* ticks the screen shakes at the hit */
	JUGGLE_RING_BLITTER		   /* which of the pair draws the flames */
};

/* A ring of small flames. The flames circle the caster in a ring that rises
   over its head from tick 8, each turning its own way through four pictures.
   At tick 64 the ring flies at the targets' midpoint, slowing as it nears; at
   tick 88 it strikes: every target is knocked back and flashes, sparks burst
   from each, and the screen shakes. The variant sets the ring's rise, its
   flames and sparks and the shake; variant 2 also takes the orange palette.

   Abilities: Juggle, Heat Juggle, Fiery Juggle. */
void btEffectRunJuggle(BATTLE_EFFECT_PARM *args)
{
	SCREEN_POINT		 caster;
	SCREEN_POINT		 midpoint;
	Sint32				 vector[3];
	SCREEN_POINT		 tgt;
	DRAW_RECTANGLE		 draw[2];
	BATTLE_EFFECT_STATE *state = Ram_HeapSlots->battleEff;
	void				*canvas = Ram_HeapSlots->battleCanvas;
	Sint32				 frm;
	Uint8				*sparkPictures = Ram_HeapSlots->battleSheet;
	PARTICLE			*ring = state->particles;
	DRAW_RECTANGLE		*blitters;
	Sint32				 lift;
	Sint32				 j;
	Sint32				 i;

	btEffectStartCanvasLayer(BGCNT_PRIORITY(1));
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	btResourceLoadAndDecompress(RESOURCE_SMALL_FLAME_SHEET, state->sheet, 1, 1);
	btResourceLoadAndDecompress(
		RESOURCE_YELLOW_GLOW_SHEET, sparkPictures, 0, 0);
	if(args->variant == EFFECT_VARIANT_STRONGEST) {
		LOAD_BG_PALETTE(RESOURCE_ORANGE_PALETTE_B);
	}
	blitters = draw;
	btEffectBuildCanvasBlitters(0, blitters);
	EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
	sySetEvent((Sint32)btEffectTransferCanvas, EVENT_PASS_VBLANK);
	lift = 0;
	{
		Sint32 *life = &((PARTICLE *)Ram_MapMetatileBuffer)->variant;

		for(i = 0; i != JUGGLE_SPARK_COUNT; i++) {
			*life = -1;
			life += sizeof(PARTICLE) / sizeof(Sint32);
		}
	}

	for(frm = 0; frm != JUGGLE_TICKS; frm++) {
		if(frm == JUGGLE_RING_SOUND_TICK)
			sySndPlayStub(SE_LAUNCH);
		if(frm == 0)
			lift = 0;

		/* The ring rises over the caster, and follows it until it is thrown. */
		btEffectPosProjectBattler(args->user, &caster);
		if(frm >= JUGGLE_RING_RISE_TICK)
			lift += JuggleStyles[args->variant][JUGGLE_RING_LIFT_STEP];
		if(lift > JuggleStyles[args->variant][JUGGLE_RING_LIFT_MAX])
			lift = JuggleStyles[args->variant][JUGGLE_RING_LIFT_MAX];
		if(frm < JUGGLE_RING_THROW_TICK) {
			ring->x = toFIXED(caster.x);
			ring->y = INTtoFIXED(caster.y - lift - 32);
		} else {
			/* The throw: z and velocityZ hold the pull toward the midpoint, and
			   the ring keeps three quarters of its speed each tick. */
			if(frm == JUGGLE_RING_THROW_TICK) {
				btEffectGetTargetsMidpoint(&midpoint);
				ring->x = toFIXED(caster.x);
				ring->y = INTtoFIXED(caster.y - lift - 32);
				ring->velX = 0;
				ring->velY = 0;
				ring->z = (toFIXED(midpoint.x) - ring->x) / 64;
				ring->velZ = (INTtoFIXED(midpoint.y - 32) - ring->y) / 64;
			}
			ring->x += ring->velX;
			ring->y += ring->velY;
			ring->velX += ring->z;
			ring->velY += ring->velZ;
			ring->velX = ring->velX * 48 / DAMPING_SCALE;
			ring->velY = ring->velY * 48 / DAMPING_SCALE;
		}

		/* The flames: each one's place on the ring turned into the canvas, the
		   second eight on a ring tipped on its side. A flame stands centred on
		   its place, three quarters of its height above it. */
		for(i = 0; i != JuggleStyles[args->variant][JUGGLE_RING_FLAMES]; i++) {
			Sint32	picture = (frm / 2 + i * 4) & 3;
			Sint32 *place;
			Sint32	x;
			Sint32	y;

			syInitMatrix();
			syTranslate(ring->x - toFIXED(128), ring->y - toFIXED(64), 0);
			syRotZ(frm * (TRIG_TURN / 256));
			syRotX(frm * (TRIG_TURN / 256) + JUGGLE_TILT);
			if(i > 7)
				syRotX(DEGtoANG(90));
			syRotY(
				(frm * 4 +
					i * JuggleStyles[args->variant][JUGGLE_RING_FLAME_SPACING])
				<< 8);
			place = vector;
			place[0] = 0;
			place[1] = 0;
			place[2] = toFIXED(lift);
			syCalcPoint(place, place);
			/* DEAD: place keeps the flame's place on the canvas, never read;
			   the ROM keeps the two stores. */
			x = FIXEDtoINT(place[0]) >> 1;
			place[0] = x + 64;
			y = FIXED_INTEGER_PART(place[1]);
			place[1] = y + 64;
			blitters[JuggleStyles[args->variant][JUGGLE_RING_BLITTER]](canvas,
				state->sheet + picture * PICTURE_BYTES(JUGGLE_FLAME),
				x + CANVAS_SIZE / 2 - JUGGLE_FLAME_WIDTH / 2,
				y + CANVAS_SIZE / 2 - JUGGLE_FLAME_HEIGHT * 3 / 4,
				JUGGLE_FLAME_WIDTH, JUGGLE_FLAME_HEIGHT);
		}

		/* The strike: sparks burst from every target. */
		if(frm == JUGGLE_RING_HIT_TICK) {
			EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
			for(i = 0; i != args->tgtCnt; i++) {
				btEffectPosProjectSpr(args->tgts[i], &tgt);
				btMotionApplyKnockbackStub(args->tgts[i], KNOCKBACK_HURL);
				btEffectTintBattler(
					args->tgts[i], SPRITE_COLOR_WHITE, POSE_DAMAGE, i, 16);
				for(j = 0; j != JuggleStyles[args->variant][JUGGLE_RING_SPARKS];
					j++) {
					PARTICLE *spark = &((PARTICLE *)Ram_MapMetatileBuffer)[i *
							JUGGLE_SPARKS_PER_TARGET +
						j];
					Sint32	  ang;
					Sint32	  speed;

					spark->x = toFIXED(tgt.x / 2);
					spark->y = toFIXED(tgt.y);
					ang = syRandom();
					speed = (syRandom() & 255) + 127;
					spark->velX = (sySin(ang) * speed) >> 6;
					spark->velY = (syCos(ang) * speed) >> 6;
					spark->variant = (syRandom() & 15) + 24;
				}
			}
			state->shakeFrm = JuggleStyles[args->variant][JUGGLE_RING_SHAKE];
		}

		/* The sparks fly and burn down, drawn by turns with the two
		   blitters. */
		for(i = 0; i != JUGGLE_SPARK_COUNT; i++) {
			PARTICLE *spark = &((PARTICLE *)Ram_MapMetatileBuffer)[i];

			if(spark->variant >= 0) {
				Sint32 size = (spark->variant >> 3) + 1;

				blitters[i & 1](canvas,
					sparkPictures + BattleEffectMotePictures[size - 1],
					FIXED_INTEGER_PART(spark->x) - size / 2,
					FIXED_INTEGER_PART(spark->y) - size, size, size * 2);
				btParticleAdvanceWithGravity2D(spark, 60, -toFIXED(1) / 4);
				spark->variant--;
			}
		}

		btCameraShake(8, 8);
		EFFECT_END_TICK(state);
	}

	EFFECT_RELEASE_BLITTERS();
	syCloseEvent((Sint32)btEffectTransferCanvas);
	btEffectEndCanvasLayer();
}

/* BG2PA for the white band: the canvas's 128 columns stretched across about 420
   screen pixels. */
#define WHITE_BAND_STRETCH Q_8_8(78.0 / 256)

/* Loads the grey ramp and fills the canvas with a white band, its left and
   right 32 columns fading out, stretched across the screen on BG2: the summons
   sweep it in from the right to white the battle out. */
void btEffectFillWhiteBand(void)
{
	Uint8 *canvas = Ram_HeapSlots->battleCanvas;
	Sint32 x;
	Sint32 y;

	btGraphicsLoadGreyRamp();
	REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
	PLTT_BACKDROP = 0;
	REG_BG2PA = WHITE_BAND_STRETCH;
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
	for(x = 0; x != CANVAS_SIZE; x++) {
		for(y = 0; y != CANVAS_SIZE; y++) {
			Sint32 level;

			if(x < 32)
				level = x * 2;
			else if(x > 95)
				level = 255 - x * 2;
			else
				level = CANVAS_LEVEL_BRIGHTEST;
			canvas[CANVAS_PIXEL_OFFSET(x, y)] = level;
		}
	}
}

#undef WHITE_BAND_STRETCH
#undef FIRE_RING_PIXELS
#undef EMBERS
#undef RAGING_HEAT_TICKS
#undef JUGGLE_TICKS
#undef JUGGLE_RING_SOUND_TICK
#undef JUGGLE_RING_RISE_TICK
#undef JUGGLE_RING_THROW_TICK
#undef JUGGLE_RING_HIT_TICK
#undef JUGGLE_SPARKS_PER_TARGET
#undef JUGGLE_SPARK_COUNT
#undef JUGGLE_TILT
#undef JUGGLE_FLAME_WIDTH
#undef JUGGLE_FLAME_HEIGHT

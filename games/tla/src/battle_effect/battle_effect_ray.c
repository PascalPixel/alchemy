/* Ray. Part of battle_effect.c. */

#define RAY_TICKS 64

#define RAY_SPARK_COUNT PARTICLE_POOL_COUNT /* sparks in PARTICLES */

/* The columns of a RaySparks row, one row a variant. */
enum {
	RAY_SPARKS_UNREAD,	 /* Unused: nothing reads it */
	RAY_SPARKS_PER_BOLT, /* sparks each bolt throws a tick */
};

#define RAY_PICTURES 1550 /* the bolt pictures in the sprite sheet */
#define RAY_COUNT	 4	  /* bolts */

/* The level the canvas flashes to every five ticks while the bolts strike. */
#define RAY_FLASH_LEVEL		  16
/* How far Destruct Ray's canvas, at twice its width, is scrolled towards the
   targets' side, in canvas pixels. */
#define RAY_WIDE_CANVAS_SHIFT 16

/* Four bolts close in, turning towards the centre and scattering sparks from
   their roots, while the canvas flashes.

   Abilities: Ray, Storm Ray, Destruct Ray. */
void btEffectRunRay(BATTLE_EFFECT_PARM *args)
{
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	Sint32				 frame;
	Uint8				*sheet;
	Sint32				 bolt;
	Sint32				 edgeShift;
	Sint32				 thrown;
	BLITTER_PAIR		 draw;
	SCREEN_POINT		 midpoint;
	Sint32				 i;

	state = RamHeapSlots->battleEffect;
	canvas = RamHeapSlots->battleCanvas;
	sheet = RamHeapSlots->battleSheet;
	edgeShift = 0;
	if(args->side == EFFECT_SIDE_ENEMIES)
		btEffectStartTiledCanvasFilled(BGCNT_WRAP | BGCNT_PRIORITY(1));
	else
		btEffectStartTiledCanvas(BGCNT_WRAP | BGCNT_PRIORITY(1));
	/* twice as wide */
	if(args->variant == EFFECT_VARIANT_STRONGEST)
		REG_BG2PA = Q_8_8(0.5);
	else
		REG_BG2PA = Q_8_8(1);
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
	draw[0] = RamHeapSlots->ramCode;
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
		EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_ADDITIVE);
	draw[1] = RamHeapSlots->ramCodePaired;
	btResourceLoadAndDecompress(
		RESOURCE_LIGHTNING_BOLT_SHEET, state->sheet + RAY_PICTURES, 1, 1);
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sheet, 0, 0);

	/* Destruct Ray's wide canvas is scrolled towards the targets' side; the
	   others' is centred on the targets and kept on the screen, and where it
	   meets an edge the bolts shift by what is left over. */
	if(args->variant == EFFECT_VARIANT_STRONGEST) {
		if(state->args->side == EFFECT_SIDE_PARTY)
			REG_BG2X = -(RAY_WIDE_CANVAS_SHIFT << 8);
		else
			REG_BG2X = RAY_WIDE_CANVAS_SHIFT << 8;
	} else {
		btEffectGetTargetsMidpoint(&midpoint);
		midpoint.x -= CANVAS_SIZE / 2;
		if(midpoint.x < 0) {
			edgeShift = midpoint.x;
			midpoint.x = 0;
		} else if(midpoint.x > NARROW_CANVAS_PARTY_X) {
			edgeShift = midpoint.x - NARROW_CANVAS_PARTY_X;
			midpoint.x = NARROW_CANVAS_PARTY_X;
		}
		REG_BG2X = -midpoint.x << 8;
	}

	/* Every spark waits to be thrown. The place and speeds set here are never
	   read: a spark is given its own as it is thrown. */
	for(i = 0; i != RAY_SPARK_COUNT; i++) {
		PARTICLE *spark = &PARTICLES[i];
		Sint32	  speed;
		Sint32	  angle;

		speed = (syRandom() & 0x3ff) + 256;
		angle = (syRandom() & 0x7fff) - DEGtoANG(90);
		SET_POSITION(spark, toFIXED(1) / 4, toFIXED(7) / 16,
			FIXEDtoINT(sySin(angle) * speed));
		spark->velocityZ = FIXEDtoINT(-(syCos(angle) * speed * 2));
		spark->variant = 0;
	}

	EFFECT_START_TRANSFER(
		state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
	sySoundPlayStub(SE_ITEM_BREAK);

	for(frame = 0; frame != RAY_TICKS; frame++) {
		if(frame == 20)
			EFFECT_SHOW_DAMAGE(SE_IMPACT);
		/* The bolts, over the first 16 ticks. */
		if(frame < 16) {
			if(frame % 5 == 2)
				syWordsFillEntry(
					canvas, CANVAS_BYTES, CANVAS_FILL_WORD(RAY_FLASH_LEVEL));
			for(bolt = 0; bolt != RAY_COUNT; bolt++) {
				Sint32 angle;
				Sint32 x;
				Sint32 y;

				/* The bolts close in from 32 pixels either side, a pixel a
				   tick, each turning at its own rate. */
				thrown = 0;
				angle = (frame * (TRIG_TURN / 32) + DEGtoANG(90)) * bolt;
				x = FIXEDtoINT((32 - frame) * sySin(angle)) + 64;
				y = -FIXEDtoINT(syCos(angle) * 8) - 8;
				if(args->variant == EFFECT_VARIANT_BASIC)
					draw[0](canvas,
						state->sheet +
							(syRandom() & 3) * LIGHTNING_BOLT_PICTURE_BYTES +
							RAY_PICTURES,
						edgeShift + x + (syRandom() & 7) - 16, y,
						LIGHTNING_BOLT_WIDTH, LIGHTNING_BOLT_HEIGHT);
				else
					draw[bolt & 1](canvas,
						state->sheet +
							(syRandom() & 3) * LIGHTNING_BOLT_PICTURE_BYTES +
							RAY_PICTURES,
						edgeShift + x + (syRandom() & 7) - 16, y,
						LIGHTNING_BOLT_WIDTH, LIGHTNING_BOLT_HEIGHT);

				/* Sparks fly from the bolt's root. */
				for(i = 0; i != RAY_SPARK_COUNT; i++) {
					PARTICLE *spark = &PARTICLES[i];

					if(spark->variant == 0) {
						Sint32 speed;
						Sint32 spread;

						speed = (syRandom() & 0x1ff) + 128;
						spread = (syRandom() & 0x7fff) - DEGtoANG(90);
						spark->x = toFIXED(x);
						spark->y = INTtoFIXED(y + 112);
						spark->velocityX = (sySin(spread) * speed) >> 9;
						spark->velocityY = -(syCos(spread) * speed * 2) >> 7;
						spark->variant = (syRandom() & 7) + 32;
						thrown++;
						if(thrown ==
							RaySparks[args->variant][RAY_SPARKS_PER_BOLT])
							break;
					}
				}
			}
			state->shakeFrame = 1;
		}

		/* Sparks bounce off the bottom. */
		for(i = 0; i != RAY_SPARK_COUNT; i++) {
			PARTICLE *spark = &PARTICLES[i];

			if(spark->variant > 0) {
				spark->variant--;
				btParticleAdvanceWithGravity2D(spark, 60, -toFIXED(1) / 32);
				if(spark->y > toFIXED(120)) {
					spark->velocityY = -spark->velocityY / 2;
				} else if(spark->x >= 0 && spark->x < toFIXED(127) &&
					spark->y >= 0) {
					Sint32 x = FIXEDtoINT(spark->x);
					Sint32 y = FIXEDtoINT(spark->y);
					Sint32 size = spark->variant / 8 + 1;

					EFFECT_DRAW_MOTE(
						draw[0], canvas, sheet, edgeShift + x, y, size);
				}
			}
		}

		if(frame >= 4 && frame < 96) {
			EFFECT_FOR_EACH_TARGET(i, args) {
				if(frame == i * 4 + 4)
					btEffectTintBattler(args->targets[i], SPRITE_COLOR_WHITE,
						POSE_DAMAGE, i, 10);
			}
		}
		btCameraShake(2, 4);
		EFFECT_END_TICK(state);
	}

	EFFECT_FINISH_CANVAS();
}

#undef RAY_TICKS
#undef RAY_SPARK_COUNT
#undef RAY_PICTURES
#undef RAY_COUNT
#undef RAY_FLASH_LEVEL
#undef RAY_WIDE_CANVAS_SHIFT

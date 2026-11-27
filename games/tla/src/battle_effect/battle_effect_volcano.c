/* @file */
/* Volcano: pillars of fire scroll up among the targets over a flickering */
/* flame, each erupting in a fountain of sparks and spitting more. */
/* Part of battle_effect.c. */

#define VOLCANO_FRAMES 96
/* The frames the pillars burn, and the strongest variant's camera turns. */
#define VOLCANO_PILLAR_FRAMES 64
/* The camera's turn each of those frames: three sixteenths of a turn in all. */
#define VOLCANO_CAMERA_SPIN (TRIG_TURN * 3 / 16 / VOLCANO_PILLAR_FRAMES)
#define VOLCANO_ERUPTION	200 /* sparks thrown as a pillar starts */
#define VOLCANO_TRICKLE		4	/* and each frame after */
#define VOLCANO_SPARK_POOL	PARTICLE_POOL_COUNT /* sparks in PARTICLES */
/* The flame on odd frames, then on even frames, after the 34x104 column. */
#define VOLCANO_FLAME_PICTURE_ODD PICTURE_BYTES(FIRE_PILLAR)
#define VOLCANO_FLAME_PICTURE_EVEN                                             \
	(VOLCANO_FLAME_PICTURE_ODD + PICTURE_BYTES(FIRE_PILLAR_FLAME))
/* Sparks are thrown from this far below the column's foot. */
#define VOLCANO_SPARK_DROP 16
/* How much faster a spark falls each frame: a sixteenth of a pixel a frame. */
#define VOLCANO_SPARK_GRAVITY (Q_8_8(1) / 16)
/* The mote size a spark is drawn at: a size smaller every 8 frames of its */
/* life, and the smallest for its last 32, which is all of a trickled spark's.
 */
#define VOLCANO_SPARK_SIZE(life) (((life) - 17) / 8)

/* One, two or four columns of fire rise among the targets, each erupting in a
 */
/* fountain of sparks and knocking the targets back. */
/* */
/* Abilities: Volcano, Eruption, Pyroclasm, Lava Shower, Molten Bath, Magma */
/* Storm. */
void btEffectRunVolcano(BATTLE_EFFECT_PARM *args) {
	BATTLE_EFFECT_STATE *state;
	void *canvas;
	s32 frame;
	s32 pillar;
	u8 *sheet;
	SCR_POINT screen;
	DRAW_RECTANGLE draw[2];
	MOTION_OBJECT *object;
	BATTLE_CAMERA *camera;
	s32 i;

	state = Ram_HeapSlots->battleEff;
	canvas = Ram_HeapSlots->battleCanvas;
	sheet = Ram_HeapSlots->battleSheet;
	btEffectStartCanvasLayer(BGCNT_PRIORITY(1));
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	btResourceLoadAndDecompress(RESOURCE_FIRE_PILLAR_SHEET_A, state->sheet, 1,
								1);
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sheet, 0, 0);
	if(args->variant == EFFECT_VARIANT_BASIC) {
		LOAD_BG_PALETTE(RESOURCE_RED_PALETTE_B);
	}
	else if(args->variant == EFFECT_VARIANT_STRONGEST) {
		LOAD_BG_PALETTE(RESOURCE_ORANGE_PALETTE_B);
	}
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
	draw[0] = Ram_HeapSlots->ramCode;
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
						EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
	draw[1] = Ram_HeapSlots->ramCodePaired;

	for(i = 0; i != VOLCANO_SPARK_POOL; i++)
		PARTICLES[i].variant = 0;

	/* The pillars stand on the first target's side of the field, at the */
	/* variant's depths. */
	object = btObjectGetStub(args->tgts[0])->obj;
	for(i = 0; i != 4; i++) {
		PARTICLE *base = &state->particles[i];
		s32 x;

		x = INTtoFIXED((syRandom() & 15) + 72);
		SET_POSITION(base, x, 0, INTtoFIXED(Volcano_Depths[args->variant][i]));
		if(object->x < 0)
			base->x = -x;
	}

	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

	for(frame = 0; frame != VOLCANO_FRAMES; frame++) {
		camera = Ram_HeapSlots->cameraState;
		if(args->variant == EFFECT_VARIANT_STRONGEST &&
		   frame < VOLCANO_PILLAR_FRAMES) {
			if(args->side == EFFECT_SIDE_ENEMIES)
				camera->yaw += VOLCANO_CAMERA_SPIN;
			else
				camera->yaw -= VOLCANO_CAMERA_SPIN;
		}
		if(frame == 16)
			EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
		EFFECT_VIEW_FROM_CAMERA(camera);

		if(frame < VOLCANO_PILLAR_FRAMES) {
			for(pillar = 0; pillar != Volcano_Counts[args->variant]; pillar++) {
				btEffectPosProjectPoint((s32 *)&state->particles[pillar],
										&screen);
				screen.x /= 2;
				screen.y -= 8;
				if(frame == Volcano_StartFrames[pillar])
					sySndPlayStub(SE_CRASH);

				/* The column scrolls up over its flickering flame. */
				if(frame >= Volcano_StartFrames[pillar] + 4) {
					s32 scroll =
						(frame * 16 + pillar * 25) % FIRE_PILLAR_HEIGHT;

					draw[pillar & 1](canvas, state->sheet,
									 screen.x - FIRE_PILLAR_WIDTH / 2,
									 screen.y - scroll - FIRE_PILLAR_HEIGHT,
									 FIRE_PILLAR_WIDTH, FIRE_PILLAR_HEIGHT);
					draw[pillar & 1](
						canvas, state->sheet, screen.x - FIRE_PILLAR_WIDTH / 2,
						screen.y - scroll, FIRE_PILLAR_WIDTH, scroll);
					if(frame & 1) {
						draw[0](
							canvas, state->sheet + VOLCANO_FLAME_PICTURE_ODD,
							screen.x - FIRE_PILLAR_FLAME_WIDTH, screen.y - 24,
							FIRE_PILLAR_FLAME_WIDTH, FIRE_PILLAR_FLAME_HEIGHT);
						draw[1](
							canvas, state->sheet + VOLCANO_FLAME_PICTURE_ODD,
							screen.x, screen.y - 24, FIRE_PILLAR_FLAME_WIDTH,
							FIRE_PILLAR_FLAME_HEIGHT);
					}
					else {
						draw[0](
							canvas, state->sheet + VOLCANO_FLAME_PICTURE_EVEN,
							screen.x - FIRE_PILLAR_FLAME_WIDTH, screen.y - 24,
							FIRE_PILLAR_FLAME_WIDTH, FIRE_PILLAR_FLAME_HEIGHT);
						draw[1](
							canvas, state->sheet + VOLCANO_FLAME_PICTURE_EVEN,
							screen.x, screen.y - 24, FIRE_PILLAR_FLAME_WIDTH,
							FIRE_PILLAR_FLAME_HEIGHT);
					}
				}

				/* Sparks: a fountain at the start, then a few a frame. */
				if(frame == Volcano_StartFrames[pillar] ||
				   frame >= Volcano_StartFrames[pillar] + 16) {
					s32 spawned = 0;

					for(i = 0; i != VOLCANO_SPARK_POOL; i++) {
						PARTICLE *spark = &PARTICLES[i];

						if(spark->variant == 0) {
							s32 speed;
							s32 angle;

							speed = (syRandom() & 0x3ff) + 32;
							angle = (syRandom() & 0x7fff) - DEGtoANG(90);
							SET_POSITION(
								spark, screen.x << 8,
								(screen.y << 8) + Q_8_8(VOLCANO_SPARK_DROP),
								FIXEDtoINT_TWICE(sySin(angle) * speed));
							spark->velY =
								FIXEDtoINT_TWICE(-(syCos(angle) * speed * 2));
							spawned++;
							if(frame == Volcano_StartFrames[pillar]) {
								spark->variant = (syRandom() & 7) + 48;
								if(spawned == VOLCANO_ERUPTION)
									break;
							}
							else {
								spark->variant = (syRandom() & 7) + 24;
								if(spawned == VOLCANO_TRICKLE)
									break;
							}
						}
					}
				}

				if(frame == Volcano_StartFrames[pillar]) {
					state->shakeFrm = 2;
					EFFECT_FOR_EACH_TARGET(i, args) {
						btEffectTintBattler(args->tgts[i],
											SPRITE_COLOR_RED_TINT, POSE_DAMAGE,
											i, 8);
						btMotionApplyKnockbackStub(args->tgts[i],
												   KNOCKBACK_HOP);
					}
				}
			}
		}

		/* The sparks, in 24.8 fixed point (z holds the speed across). */
		for(i = 0; i != VOLCANO_SPARK_POOL; i++) {
			PARTICLE *spark = &PARTICLES[i];
			s32 life = spark->variant;

			if(life > 0) {
				FIXED x;
				s32 y;
				s32 drawY;

				spark->variant = life - 1;
				x = spark->x += spark->z;
				y = spark->y += spark->velY;
				spark->z = spark->z * 60 / DAMPING_SCALE;
				spark->velY =
					spark->velY * 60 / DAMPING_SCALE - VOLCANO_SPARK_GRAVITY;
				drawY = y / 256;
				/* A spark bounces at half its speed off the foot of the battle
				 * view. */
				if(drawY > BATTLE_VIEW_HEIGHT) {
					spark->velY = -spark->velY / 2;
				}
				else if(x >= 0 && x >> 8 <= 126 && y >= 0) {
					s32 drawX = x >> 8;
					s32 size = VOLCANO_SPARK_SIZE(life);

					if(size <= 0)
						size = 1;
					EFFECT_DRAW_MOTE(draw[i & 1], canvas, sheet, drawX, drawY,
									 size);
				}
			}
		}

		btCameraShake(16, 16);
		EFFECT_END_FRAME(state);
	}

	EFFECT_RELEASE_BLITTERS();
	syCloseEvent(btEffectTransferCanvas);
	btEffectEndCanvasLayer();
}

#undef VOLCANO_FRAMES
#undef VOLCANO_PILLAR_FRAMES
#undef VOLCANO_CAMERA_SPIN
#undef VOLCANO_ERUPTION
#undef VOLCANO_TRICKLE
#undef VOLCANO_SPARK_POOL
#undef VOLCANO_FLAME_PICTURE_ODD
#undef VOLCANO_FLAME_PICTURE_EVEN
#undef VOLCANO_SPARK_DROP
#undef VOLCANO_SPARK_GRAVITY
#undef VOLCANO_SPARK_SIZE

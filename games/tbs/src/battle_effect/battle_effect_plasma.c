/* @file */
/* Plasma, and the spark groups of Blast and Dynamite. */
/* Part of battle_effect.c. */

/* The lightning pillars throw their sparks into one pool in PARTICLES, as */
/* Volcano's fire pillars do, and the sparks bounce off the ground. */

#define PLASMA_SPARK_COUNT                                                     \
	PARTICLE_POOL_COUNT /* sparks in PARTICLES, shared by every pillar */

#define PLASMA_SPACING 8 /* frames between pillars */
/* The second picture in the sprite sheet, after the first 48x112 one. */
#define PLASMA_SECOND_PICTURE PICTURE_BYTES(LIGHTNING_PILLAR)

/* Plasma_Columns has seven pillars for each side. */
#define PLASMA_COLUMN_X(side, pillar) Plasma_Columns[(side)][(pillar)]

/* Pillars of lightning strike one after another, each with a white flash, */
/* throwing sparks up from the ground and knocking the targets back. */
/* */
/* Abilities: Plasma, Shine Plasma, Spark Plasma, Thunderclap, Thunderbolt, */
/* Thunderstorm. */
void btEffectRunPlasma(BATTLE_EFFECT_PARM *args) {
	BLITTER_PAIR draw;
	BATTLE_EFFECT_STATE *state;
	void *canvas;
	s32 i;
	u8 *motes;
	s32 count;
	s32 frame;
	s32 k;
	s32 start;
	PARTICLE *spark;
	PARTICLE *sparks;

	state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
	motes = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
	state->args = args;
	btEffectStartCanvasLayer(BGCNT_PRIORITY(1));
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
						COMPOSITE_ADDITIVE);
	draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
						EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
						COMPOSITE_LIGHTEN);
	draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
	btResourceLoadAndDecompress(RESOURCE_LIGHTNING_PILLAR_SHEET, state->sheet,
								1, 1);
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, motes, 0, 0);
	if(state->args->variant != EFFECT_VARIANT_STRONGEST) {
		void *palette = syResourceGetTableEntry(RESOURCE_PLASMA_VIOLET_PALETTE);
		syWordsCopy(BG_PLTT, palette, SHEET_PALETTE_BYTES);
	}

	for(k = 0; k != PLASMA_SPARK_COUNT; k++)
		PARTICLES[k].variant = 0;
	sparks = PARTICLES;
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
	count = Plasma_Counts[state->args->variant];

	/* Seven frames a pillar and 48 more: after the last strike, 49 frames at
	 * the */
	/* least, time for its sparks (41 frames at the most) to die out. */
	for(frame = 0; frame != count * 7 + 48; frame++) {
		if(state->args->variant == EFFECT_VARIANT_STRONGEST && frame <= 63) {
			BATTLE_CAMERA *camera = BattleCameraWork;
			s32 speed;

			/* A 256th of a turn a frame, slowing from frame 56. */
			if(frame <= 55)
				speed = TRIG_TURN / 256;
			else
				speed = TRIG_TURN / 256 - (frame - 56) * 8;
			if(state->args->side == EFFECT_SIDE_PARTY)
				camera->yaw -= speed;
			else
				camera->yaw += speed;
		}
		if(frame == 32)
			EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);

		/* Each pillar strikes with a flash. */
		for(i = 0; i != count; i++) {
			start = i * PLASMA_SPACING;
			if(frame == start) {
				sySndPlayStub(SE_HEAVY_IMPACT);
				syWordsFill(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(16));
			}
			if(frame >= start && frame < start + 9) {
				if(frame >= start + 1 && frame < start + 2)
					draw[0](canvas, state->sheet,
							PLASMA_COLUMN_X(state->args->side, i) -
								LIGHTNING_PILLAR_WIDTH / 2,
							0, LIGHTNING_PILLAR_WIDTH, LIGHTNING_PILLAR_HEIGHT);
				if(frame >= start + 2 && frame < start + 4)
					draw[0](canvas, state->sheet + PLASMA_SECOND_PICTURE,
							PLASMA_COLUMN_X(state->args->side, i) -
								LIGHTNING_PILLAR_WIDTH / 2,
							0, LIGHTNING_PILLAR_WIDTH, LIGHTNING_PILLAR_HEIGHT);

				/* Sparks fly up from the pillar's foot, and the screen shakes.
				 */
				if(frame == start + 2) {
					s32 spawned = 0;

					for(k = 0; k != PLASMA_SPARK_COUNT; k++) {
						spark = &sparks[k];
						if(spark->variant == 0) {
							s32 speed;
							s32 angle;

							speed = (syRandom() & 0x3ff) + 32;
							angle = (syRandom() & 0x7fff) - DEGtoANG(90);

							spark->x = INTtoFIXED(
								PLASMA_COLUMN_X(state->args->side, i));
							spark->y = toFIXED(104);
							spark->velX = (sySin(angle) * speed) >> 7;
							spark->velY = -((syCos(angle) * speed) << 1) >> 7;
							spark->variant = (syRandom() & 7) + 32;
							spawned++;
							if(spawned == Plasma_Sparks[state->args->variant]
													   [PLASMA_SPARKS])
								break;
						}
					}
					state->shakeFrm = Plasma_Sparks[state->args->variant]
												   [PLASMA_SHAKE_FRAMES];
				}
			}
			/* The targets recoil. */
			if(frame == start + 4) {
				EFFECT_FOR_EACH_TARGET(k, state->args) {
					btMotionApplyKnockbackStub(state->args->tgts[k],
											   KNOCKBACK_HOP);
					btEffectTintBattler(state->args->tgts[k],
										SPRITE_COLOR_WHITE, POSE_DAMAGE, k, 8);
				}
			}
		}

		/* Sparks fall, bounce and shrink as they die. */
		for(k = 0; k != PLASMA_SPARK_COUNT; k++) {
			spark = &sparks[k];
			if(spark->variant > 0) {
				spark->variant--;
				btParticleAdvanceWithGravity2D(spark, 60, toFIXED(1) / 16);
				if(spark->y > toFIXED(104)) {
					spark->velY = -spark->velY / 2;
				}
				else {
					s32 x = FIXEDtoINT(spark->x);
					s32 y;
					s32 size;

					if(spark->x >= 0 && x <= 119 && spark->y >= 0) {
						y = FIXEDtoINT(spark->y);
						size = spark->variant / 8 + 1;
						EFFECT_DRAW_MOTE(draw[k & 1], canvas, motes, x, y,
										 size);
					}
				}
			}
		}
		btCameraShake(8, 16);
		EFFECT_END_FRAME(state);
	}

	EFFECT_FINISH_CANVAS();
}

/* Bursts of light go off among the targets, each ringed by fading flashes */
/* and throwing sparks. */
/* */
/* Abilities: Blast, Nova, Supernova, Fire Bomb, Cluster Bomb, Carpet Bomb. */
void btEffectRunNova(BATTLE_EFFECT_PARM *args) {
	btEffectRunSparkGroups(args, SPARK_GROUPS_NOVA);
}
/* Orange bursts go off in front of the battle. */
/* */
/* Abilities: Dynamite. */
void btEffectRunDynamite(BATTLE_EFFECT_PARM *args) {
	btEffectRunSparkGroups(args, SPARK_GROUPS_DYNAMITE);
}
#define SPARK_GROUPS_FLASHES 16 /* flashes kept per burst */
#define SPARK_GROUPS_SPACING 8	/* frames between bursts */
#define SPARK_GROUPS_RING	 12 /* of which drawn as the ring */
#define SPARK_GROUPS_FLASH_DRAWN                                               \
	18 /* a flash is drawn for its last 18 frames */
/* A ring flash's frames, a frame more for every second flash out. */
#define SPARK_GROUPS_FLASH_LIFE 25
/* Where a burst goes off: this far across from the origin, mirrored on the */
/* enemies' side, less its SPARK_GROUPS_OFFSET. The weaker strengths' one burst
 */
/* (offset 16) goes off 12 pixels across, the strongest's three (offsets 0, 24
 */
/* and 48) 28, 4 and -20. */
#define SPARK_GROUPS_BURST_REACH 28

/* One to three bursts go off: each flashes, rings itself with smaller flashes
 */
/* that fade, and throws out sparks that bounce on the ground, knocking the */
/* targets back. */
void btEffectRunSparkGroups(BATTLE_EFFECT_PARM *args, s32 mode) {
	SCR_POINT screenPosition;
	BLITTER_PAIR draw;
	BATTLE_EFFECT_STATE *state;
	void *canvas;
	s32 i;
	s32 frame;
	u8 *motes;
	s32 originX;
	s32 originY;
	s32 j;
	s32 start;
	PARTICLE *point;
	PARTICLE *spark;
	PARTICLE *sparks;

	state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
	motes = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
	state->args = args;
	if(mode == SPARK_GROUPS_NOVA) {
		btEffectStartCanvasLayer(BGCNT_PRIORITY(1));
		originX = 60;
		originY = 48;
	}
	else if(mode == SPARK_GROUPS_DYNAMITE) {
		btEffectStartCanvasLayer(0);
		originX = 60;
		originY = 64;
	}
	else {
		btEffectStartCanvasLayer(0);
		btProjectionScaledUnit(state->args->user, &screenPosition);
		originX = screenPosition.x / 2;
		originY = screenPosition.y + 48;
	}
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
						COMPOSITE_LIGHTEN);
	draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
						COMPOSITE_ADDITIVE);
	draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
	btResourceLoadAndDecompress(RESOURCE_FLASH_BURST_SHEET, state->sheet, 1, 1);
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, motes, 0, 0);
	if(mode == SPARK_GROUPS_DYNAMITE) {
		syWordsCopy(BG_PLTT,
					syResourceGetTableEntry(RESOURCE_VOLCANO_ORANGE_PALETTE),
					SHEET_PALETTE_BYTES);
	}
	else if(mode == SPARK_GROUPS_RISING) {
		syWordsCopy(BG_PLTT,
					syResourceGetTableEntry(RESOURCE_LIGHTNING_BOLT_SHEET),
					SHEET_PALETTE_BYTES);
	}
	sparks = PARTICLES;

	for(i = 0;
		i != SparkGroups_Shapes[state->args->variant][SPARK_GROUPS_BURSTS];
		i++) {
		for(j = 0; j != SPARK_GROUPS_FLASHES; j++) {
			s32 radius;
			s32 angle;

			point = &state->particles[i * SPARK_GROUPS_FLASHES + j];
			radius = j * 2;
			angle = syRandom() & TRIG_ANGLE_MASK;
			point->x = sySin(angle) * radius;
			point->y = -(syCos(angle) * radius);
			point->variant = j / 2 + SPARK_GROUPS_FLASH_LIFE;
		}
		for(j = 0;
			j != SparkGroups_Shapes[state->args->variant][SPARK_GROUPS_SPARKS];
			j++) {
			s32 speed;
			s32 angle;

			spark = &sparks[i * SparkGroups_Shapes[state->args->variant]
												  [SPARK_GROUPS_SPARKS]] +
					j;
			speed = (syRandom() & 0x3ff) + 32;
			angle = syRandom() & TRIG_ANGLE_MASK;
			if(state->args->side == EFFECT_SIDE_PARTY)
				spark->x =
					INTtoFIXED(originX -
							   SparkGroups_Shapes[state->args->variant]
												 [SPARK_GROUPS_OFFSET + i] +
							   SPARK_GROUPS_BURST_REACH);
			else
				spark->x =
					INTtoFIXED(originX +
							   SparkGroups_Shapes[state->args->variant]
												 [SPARK_GROUPS_OFFSET + i] -
							   SPARK_GROUPS_BURST_REACH);
			spark->y = INTtoFIXED(originY);
			spark->velX = (sySin(angle) * speed) >> 6;
			spark->velY = -((syCos(angle) * speed) << 1) >> 6;
			spark->variant = (syRandom() & 7) + 32;
		}
	}
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE,
						  CANVAS_FADE_THREE_QUARTERS);

	for(frame = 0;
		frame != SparkGroups_Shapes[state->args->variant][SPARK_GROUPS_BURSTS] *
						 SPARK_GROUPS_SPACING +
					 56;
		frame++) {
		BATTLE_CAMERA *camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);

		/* The strongest variant turns the camera. */
		if(state->args->variant == EFFECT_VARIANT_STRONGEST && frame <= 51) {
			if(state->args->side == EFFECT_SIDE_ENEMIES)
				camera->yaw += TRIG_TURN / 256;
			else
				camera->yaw -= TRIG_TURN / 256;
		}
		/* Severe Blow's flash. */
		if(state->args->variant == SPARK_GROUPS_SEVERE_BLOW && frame == 4)
			syWordsFill(canvas, CANVAS_BYTES,
						CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
		if(mode == SPARK_GROUPS_DYNAMITE || mode == SPARK_GROUPS_RISING) {
			if(frame == 2)
				EFFECT_SHOW_DAMAGE(SE_CRASH);
		}
		else {
			if(frame == 2)
				sySndPlayStub(SE_CRASH);
			if(frame == 24)
				EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
		}

		/* Each burst flashes, then its ring and sparks, and the targets reel.
		 */
		for(i = 0;
			i != SparkGroups_Shapes[state->args->variant][SPARK_GROUPS_BURSTS];
			i++) {
			start = i * SPARK_GROUPS_SPACING;
			if(frame == start)
				state->shakeFrm = 12;
			if(frame >= start && frame < start + 2) {
				if(state->args->side == EFFECT_SIDE_PARTY)
					draw[0](canvas, state->sheet,
							originX -
								SparkGroups_Shapes[state->args->variant]
												  [SPARK_GROUPS_OFFSET + i] +
								SPARK_GROUPS_BURST_REACH -
								FLASH_BURST_WIDTH / 2,
							originY - FLASH_BURST_HEIGHT / 2, FLASH_BURST_WIDTH,
							FLASH_BURST_HEIGHT);
				else
					draw[0](canvas, state->sheet,
							originX +
								SparkGroups_Shapes[state->args->variant]
												  [SPARK_GROUPS_OFFSET + i] -
								SPARK_GROUPS_BURST_REACH -
								FLASH_BURST_WIDTH / 2,
							originY - FLASH_BURST_HEIGHT / 2, FLASH_BURST_WIDTH,
							FLASH_BURST_HEIGHT);
			}
			if(frame >= start) {
				for(j = 0; j != SPARK_GROUPS_RING; j++) {
					s32 x;
					s32 y;

					point = &state->particles[i * SPARK_GROUPS_FLASHES] + j;
					y = FIXED_INTEGER_PART(point->y) + originY;
					if(state->args->side == EFFECT_SIDE_PARTY)
						x = FIXED_INTEGER_PART(point->x) + originX -
							SparkGroups_Shapes[state->args->variant]
											  [SPARK_GROUPS_OFFSET + i] +
							SPARK_GROUPS_BURST_REACH;
					else
						x = FIXED_INTEGER_PART(point->x) + originX +
							SparkGroups_Shapes[state->args->variant]
											  [SPARK_GROUPS_OFFSET + i] -
							SPARK_GROUPS_BURST_REACH;
					if(point->variant >= 0 &&
					   point->variant < SPARK_GROUPS_FLASH_DRAWN)
						draw[0](canvas,
								state->sheet +
									Plasma_FlashPictures[point->variant / 3] *
										PICTURE_BYTES(FLASH_BURST),
								x - FLASH_BURST_WIDTH / 2,
								y - FLASH_BURST_HEIGHT / 2, FLASH_BURST_WIDTH,
								FLASH_BURST_HEIGHT);
					if(point->variant > 0)
						point->variant--;
					else
						point->variant = -1;
				}
			}
			/* The sparks fly and fall (or rise, for SPARK_GROUPS_RISING). */
			if(frame > start + 5) {
				FIXED gravity;

				if(mode == SPARK_GROUPS_RISING)
					gravity = -toFIXED(1) / 16;
				else
					gravity = toFIXED(1) / 16;
				for(j = 0; j != SparkGroups_Shapes[state->args->variant]
												  [SPARK_GROUPS_SPARKS];
					j++) {
					spark =
						&sparks[i * SparkGroups_Shapes[state->args->variant]
													  [SPARK_GROUPS_SPARKS]] +
						j;
					if(spark->variant > 0) {
						btParticleAdvanceWithGravity2D(spark, 60, gravity);
						spark->variant--;
						if(spark->y > toFIXED(108)) {
							spark->velY = -spark->velY / 2;
						}
						else if(spark->x >= 0 && spark->x < toFIXED(127) &&
								spark->y >= 0) {
							s16 x = FIXEDtoINT(spark->x);
							s16 y = FIXEDtoINT(spark->y);
							s32 size = spark->variant / 5 + 1;

							EFFECT_DRAW_MOTE(draw[j & 1], canvas, motes, x, y,
											 size);
						}
					}
				}
			}
			EFFECT_FOR_EACH_TARGET(j, state->args) {
				if(frame == start + 6) {
					btEffectTintBattler(state->args->tgts[j],
										SPRITE_COLOR_WHITE, POSE_DAMAGE, j, 10);
					btMotionApplyKnockbackStub(state->args->tgts[j],
											   KNOCKBACK_HURL);
				}
			}
		}
		btCameraShake(16, 16);
		EFFECT_END_FRAME(state);
	}

	EFFECT_FINISH_CANVAS();
}

#undef PLASMA_SPARK_COUNT
#undef PLASMA_SPACING
#undef PLASMA_SECOND_PICTURE
#undef PLASMA_COLUMN_X
#undef SPARK_GROUPS_FLASHES
#undef SPARK_GROUPS_SPACING
#undef SPARK_GROUPS_RING
#undef SPARK_GROUPS_FLASH_DRAWN
#undef SPARK_GROUPS_FLASH_LIFE
#undef SPARK_GROUPS_BURST_REACH

/* Plasma, and the spark groups of Blast and Dynamite. Part of
   battle_effect.c. */

/* The lightning pillars throw their sparks into one pool in PARTICLES, as
   Volcano's fire pillars do, and the sparks bounce off the ground. */

#define PLASMA_SPARK_COUNT \
	PARTICLE_POOL_COUNT /* sparks in PARTICLES, shared by every pillar */

#define PLASMA_SPACING		  8 /* ticks between pillars */
/* The second picture in the sprite sheet, after the first 48x112 one. */
#define PLASMA_SECOND_PICTURE PICTURE_BYTES(LIGHTNING_PILLAR)

/* PlasmaColumns has seven pillars for each side. */
#define PLASMA_COLUMN_X(side, pillar) PlasmaColumns[(side)][(pillar)]

/* Pillars of lightning strike one after another, each with a white flash,
   throwing sparks up from the ground and knocking the targets back.

   Abilities: Plasma, Shine Plasma, Spark Plasma, Thunderclap, Thunderbolt,
   Thunderstorm. */
void btEffectRunPlasma(BATTLE_EFFECT_PARM *args)
{
	BLITTER_PAIR		 draw;
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	Sint32				 i;
	Uint8				*motes;
	Sint32				 cnt;
	Sint32				 frm;
	Sint32				 k;
	Sint32				 start;
	PARTICLE			*spark;
	PARTICLE			*sparks;

	state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
	motes = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
	state->args = args;
	btEffectStartCanvasLayer(BGCNT_PRIORITY(1));
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, COMPOSITE_ADDITIVE);
	draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
		EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_LIGHTEN);
	draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
	btResourceLoadAndDecompress(
		RESOURCE_LIGHTNING_PILLAR_SHEET, state->sheet, 1, 1);
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, motes, 0, 0);
	if(state->args->variant != EFFECT_VARIANT_STRONGEST) {
		void *palette = syResourceGetTableEntry(RESOURCE_PLASMA_VIOLET_PALETTE);
		syWordsCopy(BG_PLTT, palette, SHEET_PALETTE_BYTES);
	}

	for(k = 0; k != PLASMA_SPARK_COUNT; k++)
		PARTICLES[k].variant = 0;
	sparks = PARTICLES;
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
	cnt = PlasmaCounts[state->args->variant];

	/* Seven ticks a pillar and 48 more: after the last strike, 49 ticks at
	   the least, time for its sparks (41 ticks at the most) to die out. */
	for(frm = 0; frm != cnt * 7 + 48; frm++) {
		if(state->args->variant == EFFECT_VARIANT_STRONGEST && frm <= 63) {
			BATTLE_CAMERA *camera = BattleCameraWork;
			Sint32		   speed;

			/* A 256th of a turn a tick, slowing from tick 56. */
			if(frm <= 55)
				speed = TRIG_TURN / 256;
			else
				speed = TRIG_TURN / 256 - (frm - 56) * 8;
			if(state->args->side == EFFECT_SIDE_PARTY)
				camera->yaw -= speed;
			else
				camera->yaw += speed;
		}
		if(frm == 32)
			EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);

		/* Each pillar strikes with a flash. */
		for(i = 0; i != cnt; i++) {
			start = i * PLASMA_SPACING;
			if(frm == start) {
				sySndPlayStub(SE_HEAVY_IMPACT);
				syWordsFill(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(16));
			}
			if(frm >= start && frm < start + 9) {
				if(frm >= start + 1 && frm < start + 2)
					draw[0](canvas, state->sheet,
						PLASMA_COLUMN_X(state->args->side, i) -
							LIGHTNING_PILLAR_WIDTH / 2,
						0, LIGHTNING_PILLAR_WIDTH, LIGHTNING_PILLAR_HEIGHT);
				if(frm >= start + 2 && frm < start + 4)
					draw[0](canvas, state->sheet + PLASMA_SECOND_PICTURE,
						PLASMA_COLUMN_X(state->args->side, i) -
							LIGHTNING_PILLAR_WIDTH / 2,
						0, LIGHTNING_PILLAR_WIDTH, LIGHTNING_PILLAR_HEIGHT);

				/* Sparks fly up from the pillar's foot, and the screen
				   shakes. */
				if(frm == start + 2) {
					Sint32 spawned = 0;

					for(k = 0; k != PLASMA_SPARK_COUNT; k++) {
						spark = &sparks[k];
						if(spark->variant == 0) {
							Sint32 speed;
							Sint32 ang;

							speed = (syRandom() & 0x3ff) + 32;
							ang = (syRandom() & 0x7fff) - DEGtoANG(90);

							spark->x =
								toFIXED(PLASMA_COLUMN_X(state->args->side, i));
							spark->y = toFIXED(104);
							spark->velX = (sySin(ang) * speed) >> 7;
							spark->velY = -((syCos(ang) * speed) << 1) >> 7;
							spark->variant = (syRandom() & 7) + 32;
							spawned++;
							if(spawned ==
								PlasmaSparks[state->args->variant]
											[PLASMA_SPARKS])
								break;
						}
					}
					state->shakeFrm =
						PlasmaSparks[state->args->variant][PLASMA_SHAKE_TICKS];
				}
			}
			/* The targets recoil. */
			if(frm == start + 4) {
				EFFECT_FOR_EACH_TARGET(k, state->args) {
					btMotionApplyKnockbackStub(
						state->args->tgts[k], KNOCKBACK_HOP);
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
				} else {
					Sint32 x = FIXEDtoINT(spark->x);
					Sint32 y;
					Sint32 size;

					if(spark->x >= 0 && x <= 119 && spark->y >= 0) {
						y = FIXEDtoINT(spark->y);
						size = spark->variant / 8 + 1;
						EFFECT_DRAW_MOTE(
							draw[k & 1], canvas, motes, x, y, size);
					}
				}
			}
		}
		btCameraShake(8, 16);
		EFFECT_END_TICK(state);
	}

	EFFECT_FINISH_CANVAS();
}

/* Bursts of light go off among the targets, each ringed by fading flashes and
   throwing sparks.

   Abilities: Blast, Nova, Supernova, Fire Bomb, Cluster Bomb, Carpet Bomb. */
void btEffectRunNova(BATTLE_EFFECT_PARM *args)
{
	btEffectRunSparkGroups(args, SPARK_GROUPS_NOVA);
}

/* Orange bursts go off in front of the battle.

   Abilities: Dynamite. */
void btEffectRunDynamite(BATTLE_EFFECT_PARM *args)
{
	btEffectRunSparkGroups(args, SPARK_GROUPS_DYNAMITE);
}

#define SPARK_GROUPS_FLASHES 16 /* flashes kept per burst */
#define SPARK_GROUPS_SPACING 8	/* ticks between bursts */
#define SPARK_GROUPS_RING	 12 /* of which drawn as the ring */
#define SPARK_GROUPS_FLASH_DRAWN \
	18 /* a flash is drawn over its final 18 ticks */
/* A ring flash's ticks, a tick more for every second flash out. */
#define SPARK_GROUPS_FLASH_LIFE	 25
/* Where a burst goes off: this far across from the origin, mirrored on the
   enemies' side, less its SPARK_GROUPS_OFFSET. The weaker strengths' one burst
   (offset 16) goes off 12 pixels across, the strongest's three (offsets 0, 24
   and 48) 28, 4 and -20. */
#define SPARK_GROUPS_BURST_REACH 28

/* One to three bursts go off: each flashes, rings itself with smaller flashes
   that fade, and throws out sparks that bounce on the ground, knocking the
   targets back. */
void btEffectRunSparkGroups(BATTLE_EFFECT_PARM *args, Sint32 mode)
{
	SCREEN_POINT		 screenPos;
	BLITTER_PAIR		 draw;
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	Sint32				 i;
	Sint32				 frm;
	Uint8				*motes;
	Sint32				 originX;
	Sint32				 originY;
	Sint32				 j;
	Sint32				 start;
	PARTICLE			*point;
	PARTICLE			*spark;
	PARTICLE			*sparks;

	state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
	motes = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
	state->args = args;
	if(mode == SPARK_GROUPS_NOVA) {
		btEffectStartCanvasLayer(BGCNT_PRIORITY(1));
		originX = 60;
		originY = 48;
	} else if(mode == SPARK_GROUPS_DYNAMITE) {
		btEffectStartCanvasLayer(0);
		originX = 60;
		originY = 64;
	} else {
		btEffectStartCanvasLayer(0);
		btProjectionScaledUnit(state->args->user, &screenPos);
		originX = screenPos.x / 2;
		originY = screenPos.y + 48;
	}
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, COMPOSITE_LIGHTEN);
	draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS, COMPOSITE_ADDITIVE);
	draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
	btResourceLoadAndDecompress(RESOURCE_FLASH_BURST_SHEET, state->sheet, 1, 1);
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, motes, 0, 0);
	if(mode == SPARK_GROUPS_DYNAMITE) {
		syWordsCopy(BG_PLTT,
			syResourceGetTableEntry(RESOURCE_VOLCANO_ORANGE_PALETTE),
			SHEET_PALETTE_BYTES);
	} else if(mode == SPARK_GROUPS_RISING) {
		syWordsCopy(BG_PLTT,
			syResourceGetTableEntry(RESOURCE_LIGHTNING_BOLT_SHEET),
			SHEET_PALETTE_BYTES);
	}
	sparks = PARTICLES;

	for(i = 0;
		i != SparkGroupsShapes[state->args->variant][SPARK_GROUPS_BURSTS];
		i++) {
		for(j = 0; j != SPARK_GROUPS_FLASHES; j++) {
			Sint32 radius;
			Sint32 ang;

			point = &state->particles[i * SPARK_GROUPS_FLASHES + j];
			radius = j * 2;
			ang = syRandom() & TRIG_ANGLE_MASK;
			point->x = sySin(ang) * radius;
			point->y = -(syCos(ang) * radius);
			point->variant = j / 2 + SPARK_GROUPS_FLASH_LIFE;
		}
		for(j = 0;
			j != SparkGroupsShapes[state->args->variant][SPARK_GROUPS_SPARKS];
			j++) {
			Sint32 speed;
			Sint32 ang;

			spark = &sparks[i *
						SparkGroupsShapes[state->args->variant]
										 [SPARK_GROUPS_SPARKS]] +
				j;
			speed = (syRandom() & 0x3ff) + 32;
			ang = syRandom() & TRIG_ANGLE_MASK;
			if(state->args->side == EFFECT_SIDE_PARTY)
				spark->x = INTtoFIXED(originX -
					SparkGroupsShapes[state->args->variant]
									 [SPARK_GROUPS_OFFSET + i] +
					SPARK_GROUPS_BURST_REACH);
			else
				spark->x = INTtoFIXED(originX +
					SparkGroupsShapes[state->args->variant]
									 [SPARK_GROUPS_OFFSET + i] -
					SPARK_GROUPS_BURST_REACH);
			spark->y = toFIXED(originY);
			spark->velX = (sySin(ang) * speed) >> 6;
			spark->velY = -((syCos(ang) * speed) << 1) >> 6;
			spark->variant = (syRandom() & 7) + 32;
		}
	}
	EFFECT_START_TRANSFER(
		state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

	for(frm = 0; frm !=
		SparkGroupsShapes[state->args->variant][SPARK_GROUPS_BURSTS] *
				SPARK_GROUPS_SPACING +
			56;
		frm++) {
		BATTLE_CAMERA *camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);

		/* The strongest variant turns the camera. */
		if(state->args->variant == EFFECT_VARIANT_STRONGEST && frm <= 51) {
			if(state->args->side == EFFECT_SIDE_ENEMIES)
				camera->yaw += TRIG_TURN / 256;
			else
				camera->yaw -= TRIG_TURN / 256;
		}
		/* Severe Blow's flash. */
		if(state->args->variant == SPARK_GROUPS_SEVERE_BLOW && frm == 4)
			syWordsFill(
				canvas, CANVAS_BYTES, CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
		if(mode == SPARK_GROUPS_DYNAMITE || mode == SPARK_GROUPS_RISING) {
			if(frm == 2)
				EFFECT_SHOW_DAMAGE(SE_CRASH);
		} else {
			if(frm == 2)
				sySndPlayStub(SE_CRASH);
			if(frm == 24)
				EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
		}

		/* Each burst flashes, then its ring and sparks, and the targets
		   reel. */
		for(i = 0;
			i != SparkGroupsShapes[state->args->variant][SPARK_GROUPS_BURSTS];
			i++) {
			start = i * SPARK_GROUPS_SPACING;
			if(frm == start)
				state->shakeFrm = 12;
			if(frm >= start && frm < start + 2) {
				if(state->args->side == EFFECT_SIDE_PARTY)
					draw[0](canvas, state->sheet,
						originX -
							SparkGroupsShapes[state->args->variant]
											 [SPARK_GROUPS_OFFSET + i] +
							SPARK_GROUPS_BURST_REACH - FLASH_BURST_WIDTH / 2,
						originY - FLASH_BURST_HEIGHT / 2, FLASH_BURST_WIDTH,
						FLASH_BURST_HEIGHT);
				else
					draw[0](canvas, state->sheet,
						originX +
							SparkGroupsShapes[state->args->variant]
											 [SPARK_GROUPS_OFFSET + i] -
							SPARK_GROUPS_BURST_REACH - FLASH_BURST_WIDTH / 2,
						originY - FLASH_BURST_HEIGHT / 2, FLASH_BURST_WIDTH,
						FLASH_BURST_HEIGHT);
			}
			if(frm >= start) {
				for(j = 0; j != SPARK_GROUPS_RING; j++) {
					Sint32 x;
					Sint32 y;

					point = &state->particles[i * SPARK_GROUPS_FLASHES] + j;
					y = FIXED_INTEGER_PART(point->y) + originY;
					if(state->args->side == EFFECT_SIDE_PARTY)
						x = FIXED_INTEGER_PART(point->x) + originX -
							SparkGroupsShapes[state->args->variant]
											 [SPARK_GROUPS_OFFSET + i] +
							SPARK_GROUPS_BURST_REACH;
					else
						x = FIXED_INTEGER_PART(point->x) + originX +
							SparkGroupsShapes[state->args->variant]
											 [SPARK_GROUPS_OFFSET + i] -
							SPARK_GROUPS_BURST_REACH;
					if(point->variant >= 0 &&
						point->variant < SPARK_GROUPS_FLASH_DRAWN)
						draw[0](canvas,
							state->sheet +
								PlasmaFlashPictures[point->variant / 3] *
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
			if(frm > start + 5) {
				FIXED gravity;

				if(mode == SPARK_GROUPS_RISING)
					gravity = -toFIXED(1) / 16;
				else
					gravity = toFIXED(1) / 16;
				for(j = 0; j !=
					SparkGroupsShapes[state->args->variant]
									 [SPARK_GROUPS_SPARKS];
					j++) {
					spark = &sparks[i *
								SparkGroupsShapes[state->args->variant]
												 [SPARK_GROUPS_SPARKS]] +
						j;
					if(spark->variant > 0) {
						btParticleAdvanceWithGravity2D(spark, 60, gravity);
						spark->variant--;
						if(spark->y > toFIXED(108)) {
							spark->velY = -spark->velY / 2;
						} else if(spark->x >= 0 && spark->x < toFIXED(127) &&
							spark->y >= 0) {
							Sint16 x = FIXEDtoINT(spark->x);
							Sint16 y = FIXEDtoINT(spark->y);
							Sint32 size = spark->variant / 5 + 1;

							EFFECT_DRAW_MOTE(
								draw[j & 1], canvas, motes, x, y, size);
						}
					}
				}
			}
			EFFECT_FOR_EACH_TARGET(j, state->args) {
				if(frm == start + 6) {
					btEffectTintBattler(state->args->tgts[j],
						SPRITE_COLOR_WHITE, POSE_DAMAGE, j, 10);
					btMotionApplyKnockbackStub(
						state->args->tgts[j], KNOCKBACK_HURL);
				}
			}
		}
		btCameraShake(16, 16);
		EFFECT_END_TICK(state);
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

/* Dragon Cloud. Part of battle_effect.c. */

#define DRAGON_CLOUD_TICKS		  150
#define DRAGON_CLOUD_STRIKE_TICK  83 /* the flame strikes the first target */
/* The size of the trail's picture, in pixels. */
#define DRAGON_CLOUD_TRAIL_WIDTH  24
#define DRAGON_CLOUD_TRAIL_HEIGHT 39
#define DRAGON_CLOUD_TRAIL_PICTURE_BYTES \
	PICTURE_BYTES(DRAGON_CLOUD_TRAIL) /* bytes in each trail copy */
/* The first of the particles the embers take. */
#define DRAGON_CLOUD_FIRST_EMBER		8
/* FirePillarSheetB: five flames, then the trail picture. */
#define DRAGON_CLOUD_FLAME_WIDTH		24
#define DRAGON_CLOUD_FLAME_HEIGHT		48
#define DRAGON_CLOUD_TRAIL_PICTURE		(5 * PICTURE_BYTES(DRAGON_CLOUD_FLAME))
#define DRAGON_CLOUD_PUFF_PICTURES		15360
#define DRAGON_CLOUD_PUFFS				8 /* puffs, particles 0 to 7 */
#define DRAGON_CLOUD_EMBERS				40
#define DRAGON_CLOUD_FLASHES			16 /* flashes, after the embers */
/* The trail's copies of the flame's path, the older ones from
   DRAGON_CLOUD_TRAIL_BRIGHT on drawn darker, four levels a copy. */
#define DRAGON_CLOUD_TRAIL_LENGTH		20
#define DRAGON_CLOUD_TRAIL_BRIGHT		10
/* The puffs' arc: this many pixels across and down from its centre. */
#define DRAGON_CLOUD_PUFF_RADIUS_X		24
#define DRAGON_CLOUD_PUFF_RADIUS_Y		16
/* An ember is drawn at size DRAGON_CLOUD_EMBER_SIZE_MAX at depth
   DRAGON_CLOUD_EMBER_DEPTH_NEAR or nearer, one size smaller every
   DRAGON_CLOUD_EMBER_DEPTH_STEP farther, its depth held below
   DRAGON_CLOUD_EMBER_DEPTH_FAR. */
#define DRAGON_CLOUD_EMBER_SIZE_MAX		9
#define DRAGON_CLOUD_EMBER_DEPTH_NEAR	160
#define DRAGON_CLOUD_EMBER_DEPTH_STEP	64
#define DRAGON_CLOUD_EMBER_DEPTH_FAR	799
/* The flash and puff animations: six pictures, two ticks each (Epicenter's
   flashes, four ticks each). */
#define DRAGON_CLOUD_ANIMATION_PICTURES 6

/* Epicenter's sparks, EPICENTER_SPARK_COUNT of EXTRA_PARTICLES, come in three
   batches: at the target, thrown up from the user's side, and shed by the
   flame. The sparks thrown up start on EPICENTER_SPARK_GROUND and bounce
   there. */
#define EPICENTER_SPARK_COUNT  512
#define EPICENTER_SPARK_BATCH  128
#define EPICENTER_SPARK_GROUND 108

/* Draws one puff picture sized by the puff tables. */
#define DRAGON_CLOUD_DRAW_PUFF(blit, canvas, sheet, picture, x, y) \
	blit(canvas, \
		&(sheet)[BattleEffectEmberStreakPictures[picture] + \
			DRAGON_CLOUD_PUFF_PICTURES], \
		x, y, BattleEffectEmberStreakPictureWidths[picture], \
		BattleEffectEmberStreakPictureHeights[picture])

/* A flame leaves the user along a stored path with a fading trail behind it,
   strikes the first target and bursts into embers and flashes. Epicenter's
   burns orange and throws sparks: up from the user's side at the start, off the
   flame as it flies, and over the target at the strike.

   Abilities: Dragon Cloud, Epicenter. */
void btEffectRunDragonCloud(BATTLE_EFFECT_PARM *args)
{
	POINT2_D			 trail[DRAGON_CLOUD_TRAIL_LENGTH];
	SCREEN_POINT		 target;
	SCREEN_POINT		 pos;
	DRAW_RECTANGLE		 draw[2];
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	Sint32				 frame;
	Sint32				 drift;
	Sint8				*path;
	Sint32				 x;
	Sint32				 y;
	Uint8				*motes;
	ACTOR				*actor;
	PARTICLE			*particle;
	Sint32				 i;
	Sint32				 j;
	Sint32				 angle;

	state = RamHeapSlots->battleEffect;
	canvas = RamHeapSlots->battleCanvas;
	drift = 0;
	x = 0;
	y = 0;
	motes = RamHeapSlots->battleSheet;
	btEffectStartCanvasLayer(0);
	REG_BLDALPHA = EFFECT_BLEND_ADD;

	btResourceLoadAndDecompress(
		RESOURCE_FIRE_PILLAR_SHEET_B, state->sheet, 1, 1);
	/* Darker copies of the trail picture, for the trail's tail. */
	for(i = 1; i != DRAGON_CLOUD_TRAIL_LENGTH; i++) {
		for(j = 0; j != DRAGON_CLOUD_TRAIL_PICTURE_BYTES; j++) {
			Sint32 shade = state->sheet[DRAGON_CLOUD_TRAIL_PICTURE + j];

			if(i > DRAGON_CLOUD_TRAIL_BRIGHT) {
				shade = shade - (i - DRAGON_CLOUD_TRAIL_BRIGHT) * 4;
				if(shade < 0)
					shade = 0;
				state->sheet[DRAGON_CLOUD_TRAIL_PICTURE +
					(i - DRAGON_CLOUD_TRAIL_BRIGHT) *
						DRAGON_CLOUD_TRAIL_PICTURE_BYTES +
					j] = shade;
			}
		}
	}
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, motes, 0, 0);
	btResourceLoadAndDecompress(RESOURCE_EMBER_STREAK_SHEET,
		&state->sheet[DRAGON_CLOUD_PUFF_PICTURES], 1, 1);
	btResourceLoadAndDecompress(
		RESOURCE_FLASH_BURST_SHEET, RamMapMetatileBuffer, 1, 0);
	if(args->variant == EFFECT_VARIANT_STRONGER) {
		LOAD_BG_PALETTE(RESOURCE_ORANGE_PALETTE_B);
	}
	btEffectBuildCanvasBlitters(args->side, draw);
	EFFECT_START_TRANSFER(
		state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

	/* The embers and flashes thrown at the strike wait at the target: the first
	   DragonCloudSparkCounts[variant] are drawn as embers, the rest play the
	   flash pictures and rise more slowly. */
	actor = (ACTOR *)btObjectGetStub(args->targets[0])->object;
	for(i = 0; i != DRAGON_CLOUD_EMBERS + DRAGON_CLOUD_FLASHES; i++) {
		particle = &state->particles[DRAGON_CLOUD_FIRST_EMBER + i];
		SET_POSITION(particle, actor->x.fixed, 0, actor->z.fixed);
		if(i >= DragonCloudSparkCounts[args->variant])
			SET_VELOCITY(particle, ((syRandom() & 63) - 32) << 14,
				(syRandom() & 63) << 12, ((syRandom() & 63) - 32) << 14);
		else
			SET_VELOCITY(particle, ((syRandom() & 63) - 32) << 14,
				(syRandom() & 63) << 13, ((syRandom() & 63) - 32) << 14);
		particle->variant = 1;
	}

	/* Epicenter's sparks: all asleep but the first batch, at the target (only
	   Epicenter draws them). */
	for(i = 0; i != EPICENTER_SPARK_COUNT; i++)
		EXTRA_PARTICLES[i].variant = -1;
	for(i = 0; i != EPICENTER_SPARK_BATCH; i++) {
		particle = &EXTRA_PARTICLES[i];
		SET_POSITION(particle, actor->x.fixed, 0, actor->z.fixed);
		particle->velocityX = ((syRandom() & 63) - 32) << 14;
		particle->velocityY = (syRandom() & 63) << 12;
		particle->velocityZ = ((syRandom() & 63) - 32) << 14;
		particle->variant = (syRandom() & 15) + 24;
	}

	/* The eight puffs, on an arc over the user, each two ticks after the
	   last. */
	for(i = 0, angle = -DEGtoANG(90); i != DRAGON_CLOUD_PUFFS;
		angle += TRIG_TURN / 16, i++) {
		particle = &state->particles[i];
		if(args->side == EFFECT_SIDE_PARTY) {
			particle->x =
				FIXEDtoINT(sySin(angle) * DRAGON_CLOUD_PUFF_RADIUS_X) + 88;
			particle->y =
				FIXEDtoINT(syCos(angle) * DRAGON_CLOUD_PUFF_RADIUS_Y) + 40;
		} else {
			particle->x =
				FIXEDtoINT(-(sySin(angle) * DRAGON_CLOUD_PUFF_RADIUS_X)) + 16;
			particle->y =
				FIXEDtoINT(syCos(angle) * DRAGON_CLOUD_PUFF_RADIUS_Y) + 40;
		}
		particle->variant = -(i * 2);
	}
	path = syResourceGetTableEntry(RESOURCE_DRAGON_CLOUD_FLAME_PATH);

	for(frame = 0; frame != DRAGON_CLOUD_TICKS; frame++) {
		BATTLE_CAMERA *camera = RamHeapSlots->cameraState;

		if(frame == DRAGON_CLOUD_STRIKE_TICK) {
			if(args->variant == EFFECT_VARIANT_BASIC)
				EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
			else
				EFFECT_SHOW_DAMAGE(SE_CRASH);
		}
		if(frame == 0)
			sySoundPlayStub(SE_WHOOSH);
		if(frame == 50)
			sySoundPlayStub(SE_WHOOSH);

		/* For 64 ticks the camera turns away from the user's side. */
		if(args->side == EFFECT_SIDE_ENEMIES) {
			if(frame <= 63)
				camera->yaw -= TRIG_TURN / 256;
		} else {
			if(frame <= 63)
				camera->yaw += TRIG_TURN / 256;
		}
		btCameraSetViewStub(0, 0, 0, BATTLE_ZOOM_NORMAL);

		/* Frames 0-17: the puff arc rises, one picture every three ticks. */
		if(frame <= 17) {
			Sint32 picture = frame / 3;
			Sint32 width;
			Sint32 offset;

			offset = BattleEffectEmberStreakPictures[picture];
			/* LEFTOVER(template): width, read from the puff table as
			 * btEffectRunHaures and btEffectRunUlysses read it to draw
			 * with, and unused here; as no width at all. */
			width = BattleEffectEmberStreakPictureWidths[picture];
			draw[0](canvas, &state->sheet[offset + DRAGON_CLOUD_PUFF_PICTURES],
				56 - BattleEffectEmberStreakPictureWidths[picture] / 2,
				BattleEffectEmberStreakPictureBiasY[picture] + 60,
				BattleEffectEmberStreakPictureWidths[picture],
				BattleEffectEmberStreakPictureHeights[picture]);
			DRAGON_CLOUD_DRAW_PUFF(draw[1], canvas, state->sheet, picture,
				64 - BattleEffectEmberStreakPictureWidths[picture] / 2,
				BattleEffectEmberStreakPictureBiasY[picture] + 60);
		}

		/* Epicenter: a batch of sparks thrown up from the user's side at the
		   start. */
		if(args->variant == EFFECT_VARIANT_STRONGER) {
			if(frame == 0) {
				for(i = 0; i != EPICENTER_SPARK_BATCH; i++) {
					Sint32 angle;
					Sint32 force;

					particle = &EXTRA_PARTICLES[EPICENTER_SPARK_BATCH + i];
					particle->x = toFIXED(60);
					particle->y = toFIXED(EPICENTER_SPARK_GROUND);
					angle = (syRandom() & 0x7fff) + DEGtoANG(90);
					force = (syRandom() & 255) + 127;
					particle->velocityX = (sySin(angle) * force) >> 7;
					particle->velocityY = (syCos(angle) * force) >> 5;
					particle->variant = (syRandom() & 15) + 24;
				}
			}
			/* Only the first half of them is drawn. */
			for(i = 0; i != EPICENTER_SPARK_BATCH / 2; i++) {
				particle = &EXTRA_PARTICLES[EPICENTER_SPARK_BATCH + i];
				if(particle->variant >= 0 && frame >= i / 8) {
					Sint32 size = (particle->variant >> 4) + 1;

					draw[i & 1](canvas,
						motes + BattleEffectMotePictures[size - 1],
						FIXED_INTEGER_PART(particle->x) - size / 2,
						FIXED_INTEGER_PART(particle->y) - size, size, size * 2);
					btParticleAdvanceWithGravity2D(
						particle, 62, toFIXED(1) / 4);
					if(particle->y >
						INTtoFIXED((i & 3) + EPICENTER_SPARK_GROUND))
						particle->velocityY = -particle->velocityY / 2;
					particle->variant--;
				}
			}
		}

		/* The flame flies along its path, a 16-bit start and then steps; from
		   tick 78 it dives from above the target. */
		if(frame >= 18 && frame <= 58) {
			if(frame == 18) {
				x = (path[0] << 8) + (Uint8)path[1];
				y = (path[2] << 8) + (Uint8)path[3] + 16;
				path += 4;
			} else {
				x += path[0];
				y += path[1];
				path += 2;
			}
		}
		if(frame >= 78 && frame <= 118) {
			btEffectPosProjectBattler(args->targets[0], &target);
			if(frame == 78) {
				x = target.x / 2;
				y = 48;
			} else {
				y -= 16;
			}
		}

		/* The trail: each copy takes the place of the one before, the older
		   ones darker. */
		for(j = DRAGON_CLOUD_TRAIL_LENGTH - 1; j != 0; j--) {
			if(frame > j + 18 && frame <= j + 83) {
				trail[j].x = trail[j - 1].x;
				trail[j].y = trail[j - 1].y;
				if(j > DRAGON_CLOUD_TRAIL_BRIGHT)
					draw[0](canvas,
						&state->sheet[DRAGON_CLOUD_TRAIL_PICTURE +
							(j - DRAGON_CLOUD_TRAIL_BRIGHT) *
								DRAGON_CLOUD_TRAIL_PICTURE_BYTES],
						trail[j].x - DRAGON_CLOUD_TRAIL_WIDTH / 2, trail[j].y,
						DRAGON_CLOUD_TRAIL_WIDTH, DRAGON_CLOUD_TRAIL_HEIGHT);
				else
					draw[0](canvas, &state->sheet[DRAGON_CLOUD_TRAIL_PICTURE],
						trail[j].x - DRAGON_CLOUD_TRAIL_WIDTH / 2, trail[j].y,
						DRAGON_CLOUD_TRAIL_WIDTH, DRAGON_CLOUD_TRAIL_HEIGHT);
			}
		}
		syInitMatrix();
		syLookAtLoad(camera->eye, camera->focus);

		/* The flame leans as it rises and falls. */
		if(frame >= 18 && frame <= DRAGON_CLOUD_STRIKE_TICK) {
			Sint32 lift;

			if(frame >= 78 && frame <= 118)
				pos.x = x;
			else if(args->side == EFFECT_SIDE_PARTY)
				pos.x = 64 - x / 2;
			else
				pos.x = x / 2 + 64;
			pos.y = 60 - y;
			lift = (pos.y - trail[0].y - 24) / 2;
			if(lift > 2)
				lift = 2;
			if(lift < -2)
				lift = -2;
			drift += lift;
			if(drift > 8)
				drift = 8;
			if(drift < -8)
				drift = -8;
			lift = drift / 4 + 2;
			trail[0].x = pos.x;
			trail[0].y = pos.y - 20;
			draw[0](canvas,
				&state->sheet[lift * PICTURE_BYTES(DRAGON_CLOUD_FLAME)],
				trail[0].x - DRAGON_CLOUD_FLAME_WIDTH / 2, trail[0].y - 2,
				DRAGON_CLOUD_FLAME_WIDTH, DRAGON_CLOUD_FLAME_HEIGHT);

			/* The flame sheds four sparks a tick (only Epicenter draws
			   them). */
			for(i = 0; i != 4; i++) {
				Sint32 angle;
				Sint32 force;
				Sint32 index;

				index = ((frame * 4 + i) & (EPICENTER_SPARK_BATCH - 1)) +
					EPICENTER_SPARK_BATCH * 2;
				particle = &EXTRA_PARTICLES[index];
				particle->x = toFIXED(pos.x);
				particle->y = toFIXED(pos.y);
				angle = syRandom();
				force = (syRandom() & 255) + 127;
				particle->velocityX = (sySin(angle) * force) >> 7;
				particle->velocityY = (syCos(angle) * force) >> 5;
				particle->variant = (syRandom() & 15) + 24;
			}
		}

		/* After the strike the embers rise and fall back, sized by depth, and
		   the flashes play the flash pictures. */
		if(frame >= DRAGON_CLOUD_STRIKE_TICK) {
			for(i = 0; i != DRAGON_CLOUD_EMBERS + DRAGON_CLOUD_FLASHES; i++) {
				particle = &state->particles[DRAGON_CLOUD_FIRST_EMBER + i];
				if(particle->y >= 0) {
					Sint32 size;

					btEffectPosProjectPoint(&particle->x, &pos);
					pos.x >>= 1;
					if(pos.depth < DRAGON_CLOUD_EMBER_DEPTH_NEAR)
						pos.depth = DRAGON_CLOUD_EMBER_DEPTH_NEAR;
					if(pos.depth > DRAGON_CLOUD_EMBER_DEPTH_FAR)
						pos.depth = DRAGON_CLOUD_EMBER_DEPTH_FAR;
					size = DRAGON_CLOUD_EMBER_SIZE_MAX -
						(pos.depth - DRAGON_CLOUD_EMBER_DEPTH_NEAR) /
							DRAGON_CLOUD_EMBER_DEPTH_STEP;
					if(i >= DragonCloudSparkCounts[args->variant]) {
						Sint32 picture;

						if(args->variant == EFFECT_VARIANT_BASIC)
							picture = particle->variant / 2;
						else
							picture = particle->variant / 4;
						if(picture < DRAGON_CLOUD_ANIMATION_PICTURES) {
							draw[args->variant](canvas,
								RamMapMetatileBuffer +
									picture * PICTURE_BYTES(FLASH_BURST),
								pos.x - FLASH_BURST_WIDTH / 2, pos.y - 40,
								FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
							particle->variant++;
							btParticleAdvanceWithGravity3D(
								particle, 62, toFIXED(1) / 8);
						}
					} else {
						draw[0](canvas,
							motes + BattleEffectMotePictures[size - 1],
							pos.x - size / 2, pos.y - size, size, size * 2);
						btParticleAdvanceWithGravity3D(
							particle, PARTICLE_DAMPING_NONE, -toFIXED(1) / 8);
					}
				}
			}

			/* Epicenter's first batch of sparks, at the target. */
			if(args->variant == EFFECT_VARIANT_STRONGER) {
				for(i = 0; i != EPICENTER_SPARK_BATCH; i++) {
					Sint32 size;

					particle = &EXTRA_PARTICLES[i];
					size = particle->variant;
					if(size >= 0) {
						btEffectPosProjectPoint(&particle->x, &pos);
						size >>= 3;
						size += 1;
						pos.x >>= 1;
						draw[i & 1](canvas,
							motes + BattleEffectMotePictures[size - 1],
							pos.x - size / 2, pos.y - size, size, size * 2);
						btParticleAdvanceWithGravity3D(
							particle, 62, toFIXED(1) / 8);
						particle->variant--;
					}
				}
			}
		}

		/* Epicenter: the sparks the flame shed. */
		if(args->variant == EFFECT_VARIANT_STRONGER) {
			PARTICLE *spark = &EXTRA_PARTICLES[EPICENTER_SPARK_BATCH * 2];

			for(i = 0; i != EPICENTER_SPARK_BATCH; i++, spark++) {
				if(spark->variant >= 0) {
					Sint32 size = (spark->variant >> 4) + 1;

					draw[i & 1](canvas,
						motes + BattleEffectMotePictures[size - 1],
						FIXED_INTEGER_PART(spark->x) - size / 2,
						FIXED_INTEGER_PART(spark->y) - size, size, size * 2);
					btParticleAdvanceWithGravity2D(spark, 60, -toFIXED(1) / 4);
					spark->variant--;
				}
			}
		}

		if(frame == DRAGON_CLOUD_STRIKE_TICK) {
			state->shakeFrame = 8;
			EFFECT_STRIKE_TARGET(args, 0, 8, KNOCKBACK_HOP);
		}
		if(frame == 50) {
			state->shakeFrame = 12;
			btEffectTintBattler(
				args->targets[0], SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 8);
		}
		/* The puffs. */
		if(frame > 49) {
			for(i = 0; i != DRAGON_CLOUD_PUFFS; i++) {
				particle = &state->particles[i];
				if(particle->variant >= 0 &&
					particle->variant < 2 * DRAGON_CLOUD_ANIMATION_PICTURES) {
					Sint32 picture = particle->variant / 2;

					DRAGON_CLOUD_DRAW_PUFF(draw[1], canvas, state->sheet,
						picture,
						particle->x -
							BattleEffectEmberStreakPictureWidths[picture] / 2,
						particle->y +
							BattleEffectEmberStreakPictureBiasY[picture]);
				}
				particle->variant++;
			}
		}
		btCameraShake(8, 8);
		EFFECT_END_TICK(state);
	}

	EFFECT_FINISH_CANVAS();
}

#undef DRAGON_CLOUD_TICKS
#undef DRAGON_CLOUD_STRIKE_TICK
#undef DRAGON_CLOUD_TRAIL_WIDTH
#undef DRAGON_CLOUD_TRAIL_HEIGHT
#undef DRAGON_CLOUD_TRAIL_PICTURE_BYTES
#undef DRAGON_CLOUD_FIRST_EMBER
#undef DRAGON_CLOUD_FLAME_WIDTH
#undef DRAGON_CLOUD_FLAME_HEIGHT
#undef DRAGON_CLOUD_TRAIL_PICTURE
#undef DRAGON_CLOUD_PUFF_PICTURES
#undef DRAGON_CLOUD_PUFFS
#undef DRAGON_CLOUD_EMBERS
#undef DRAGON_CLOUD_FLASHES
#undef DRAGON_CLOUD_TRAIL_LENGTH
#undef DRAGON_CLOUD_TRAIL_BRIGHT
#undef DRAGON_CLOUD_PUFF_RADIUS_X
#undef DRAGON_CLOUD_PUFF_RADIUS_Y
#undef DRAGON_CLOUD_EMBER_SIZE_MAX
#undef DRAGON_CLOUD_EMBER_DEPTH_NEAR
#undef DRAGON_CLOUD_EMBER_DEPTH_STEP
#undef DRAGON_CLOUD_EMBER_DEPTH_FAR
#undef DRAGON_CLOUD_ANIMATION_PICTURES
#undef EPICENTER_SPARK_COUNT
#undef EPICENTER_SPARK_BATCH
#undef EPICENTER_SPARK_GROUND
#undef DRAGON_CLOUD_DRAW_PUFF

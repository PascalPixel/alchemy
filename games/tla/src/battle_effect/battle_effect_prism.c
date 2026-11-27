/* Prism. Part of battle_effect.c. */

/* One of PrismCounts' three numbers for a variant. */
#define PRISM_COUNT(variant, column) PrismCounts[(variant) * 3 + (column)]
#define PRISM_CRYSTAL_COUNT			 0 /* the crystals that fall */
#define PRISM_TICKS					 1 /* the effect's ticks */
#define PRISM_DAMAGE_TICK			 2 /* the tick the damage is shown */
/* The camera's turn a tick in the strongest variant, until it slows from
   PRISM_CAMERA_SLOWING to a stop at PRISM_CAMERA_STOP. */
#define PRISM_CAMERA_TURN			 192
#define PRISM_CAMERA_SLOWING		 96
#define PRISM_CAMERA_STOP			 104
/* The x in PrismShardStarts under the middle of the crystal. */
#define PRISM_SHARD_MIDDLE_X		 40
#define PRISM_GROUND_Y				 toFIXED(56) /* y 56, where the crystals shatter */
#define PRISM_SHARD_PICTURES \
	PICTURE_BYTES(CRYSTAL)			/* the shards, after the crystal */
#define PRISM_SHATTERED			  1 /* a crystal's z once it has hit the ground */
/* The shard pictures that PrismShardPictures, PrismShardWidths and
   PrismShardHeights hold. */
#define PRISM_SHARD_PICTURE_COUNT 12

/* Crystals drop out of the sky onto the targets' side and shatter on the ground
   into spinning shards, flashing the targets and shaking the screen.

   Abilities: Prism, Hail Prism, Freeze Prism. */
void btEffectRunPrism(BATTLE_EFFECT_PARM *args)
{
	BLITTER_PAIR		 draw;
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	Sint32				 frame;
	PARTICLE			*seed;
	PARTICLE			*crystal;
	PARTICLE			*shards;
	PARTICLE			*shard;
	Sint32				 x;
	Sint32				 picture;
	Sint32				 i;
	Sint32				 j;
	Sint32				 k;

	shards = PARTICLES;
	state = RamHeapSlots->battleEffect;
	canvas = RamHeapSlots->battleCanvas;
	btEffectStartCanvasLayer(0);
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	/* The boulder sprite sheet is unpacked first and the crystal sprite sheet
	   over it; Prism draws only the crystal's pictures. */
	btResourceLoadAndDecompress(RESOURCE_BOULDER_SHEET, state->sheet, 1, 0);
	btResourceLoadAndDecompress(RESOURCE_CRYSTAL_SHEET, state->sheet, 1, 1);
	btEffectBuildCanvasBlitters(args->side, draw);
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

	/* The crystals wait above the screen, each eight ticks behind the last,
	   and drift sideways so that they land over the targets. */
	for(k = 0; k != PRISM_COUNT(args->variant, PRISM_CRYSTAL_COUNT); k++) {
		seed = &state->particles[k];
		/* A first roll, never read. */
		x = syRandom() & 31;
		seed->y = -toFIXED(64);
		if(args->side == EFFECT_SIDE_PARTY) {
			x = INTtoFIXED((syRandom() & 31) + 80);
			seed->velocityX = (syRandom() & 63) << 12;
		} else {
			x = INTtoFIXED((syRandom() & 31) + 8);
			seed->velocityX = -(syRandom() & 63) << 12;
		}
		seed->x = x - seed->velocityX * 18;
		seed->velocityY = 0;
		seed->z = 0;
		seed->variant = k * 8;
	}

	for(frame = 0; frame != PRISM_COUNT(args->variant, PRISM_TICKS); frame++) {
		/* The strongest variant turns the camera. */
		if(args->variant == EFFECT_VARIANT_STRONGEST &&
			frame < PRISM_CAMERA_STOP) {
			BATTLE_CAMERA *camera = RamHeapSlots->cameraState;
			Sint32		   speed;

			if(frame < PRISM_CAMERA_SLOWING)
				speed = PRISM_CAMERA_TURN;
			else
				speed = PRISM_CAMERA_TURN - (frame - PRISM_CAMERA_SLOWING) * 24;
			if(args->side == EFFECT_SIDE_ENEMIES)
				camera->yaw -= speed;
			else
				camera->yaw += speed;
		}
		if(frame == PRISM_COUNT(args->variant, PRISM_DAMAGE_TICK))
			EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
		if(frame == PRISM_COUNT(args->variant, PRISM_TICKS) - 8) {
			EFFECT_SET_TRANSFER(
				state, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(6));
		}
		if(frame <= PRISM_COUNT(args->variant, PRISM_TICKS) - 8) {
			for(i = 0; i != PRISM_COUNT(args->variant, PRISM_CRYSTAL_COUNT);
				i++) {
				crystal = &state->particles[i];
				if(crystal->z == PRISM_SHATTERED) {
					/* A shattered crystal's shards fly and spin, slowing
					   down. */
					for(j = 0; j != SHARDS_PER_CRYSTAL; j++) {
						picture = (j % 5) * 3 +
							(shards[i * SHARDS_PER_CRYSTAL + j].variant /
								SHARD_TURN) %
								3;
						shard = &shards[i * SHARDS_PER_CRYSTAL + j];
						/* UB: kind 4 (j % 5 == 4) asks for pictures 12 to 14,
						   past the ends of PrismShardWidths and
						   PrismShardPictures, and picture 14 past
						   PrismShardHeights'; the heights it finds are 0, so
						   those three shards of every crystal never show. */
#if FIX_CRYSTAL_SHARD_TABLE_OVERRUN
						if(picture < PRISM_SHARD_PICTURE_COUNT)
#endif
							draw[j <= 2](canvas,
								state->sheet + PrismShardPictures[picture] +
									PRISM_SHARD_PICTURES,
								FIXED_INTEGER_PART(shard->x) -
									PrismShardWidths[picture] / 2,
								FIXED_INTEGER_PART(shard->y) -
									PrismShardHeights[picture] / 2,
								PrismShardWidths[picture],
								PrismShardHeights[picture]);
						btParticleAdvanceWithGravity2D(
							shard, PARTICLE_DAMPING_NONE, toFIXED(1) / 8);
						shard->variant += shard->z;
						if(shard->z > 1 && (frame & 1))
							shard->z--;
					}
				} else if(frame >= crystal->variant) {
					draw[i & 1](canvas, state->sheet,
						FIXED_INTEGER_PART(crystal->x) - CRYSTAL_WIDTH / 2,
						FIXED_INTEGER_PART(crystal->y), CRYSTAL_WIDTH,
						CRYSTAL_HEIGHT);
					btParticleAdvanceWithGravity2D(
						crystal, PARTICLE_DAMPING_NONE, toFIXED(1));
					if(crystal->y > PRISM_GROUND_Y) {
						crystal->z = PRISM_SHATTERED;
						crystal->y = PRISM_GROUND_Y;
						for(j = 0; j != SHARDS_PER_CRYSTAL; j++) {
							shard = &shards[i * SHARDS_PER_CRYSTAL + j];
							shard->x = INTtoFIXED(PrismShardStarts[j * 2] -
										   PRISM_SHARD_MIDDLE_X) +
								crystal->x;
							shard->y = toFIXED(PrismShardStarts[j * 2 + 1]);
							shard->velocityX = ((syRandom() & 127) - 64) << 11;
							shard->velocityY = -(syRandom() & 127) << 11;
							if(i & 1) {
								shard->velocityX *= 2;
								shard->velocityY *= 2;
							}
							shard->z = 32;
							shard->variant = 0;
						}
						state->shakeFrame = 8;
						sySoundPlayStub(SE_CLATTER);
						EFFECT_FOR_EACH_TARGET(j, args)
							btEffectTintBattler(args->targets[j],
								SPRITE_COLOR_WHITE, POSE_DAMAGE, j, 4);
					}
				}
			}
		}
		btCameraShake(args->variant * 2 + 4, args->variant * 4 + 8);
		EFFECT_END_TICK(state);
	}

	EFFECT_FINISH_CANVAS();
}

#undef PRISM_CRYSTAL_COUNT
#undef PRISM_TICKS
#undef PRISM_DAMAGE_TICK
#undef PRISM_COUNT
#undef PRISM_CAMERA_TURN
#undef PRISM_CAMERA_SLOWING
#undef PRISM_CAMERA_STOP
#undef PRISM_SHARD_MIDDLE_X
#undef PRISM_GROUND_Y
#undef PRISM_SHATTERED

/* The ramp's colours, one a canvas level. */
#define PHASE_RAMP_COLORS (CANVAS_LEVEL_BRIGHTEST + 1)
/* A channel's level at the top of the ramp, about which the sine sways it by up
   to twice as much either way. */
#define PHASE_PEAK		  16

/* Rebuilds the background palette's first ramp as a grey whose channels sway
   with the tick, so the effect shimmers through colours. */
void btEffectUpdatePhasePalette(
	Sint32 frame, Sint32 redPhase, Sint32 greenPhase, Sint32 bluePhase)
{
	Uint16 palette[PHASE_RAMP_COLORS];
	Sint32 phase;
	Sint32 redOffset;
	Sint32 greenOffset;
	Sint32 blueOffset;
	Sint32 index;

	phase = frame * (TRIG_TURN / 64);
	redOffset = FIXEDtoINT_TWICE(sySin(phase + redPhase) * PHASE_PEAK);
	greenOffset = FIXEDtoINT_TWICE(sySin(phase + greenPhase) * PHASE_PEAK);
	blueOffset = FIXEDtoINT_TWICE(sySin(phase + bluePhase) * PHASE_PEAK);

	palette[0] = 0;
	for(index = 1; index != PHASE_RAMP_COLORS; index++) {
		Sint32 red;
		Sint32 green;
		Sint32 blue;

		red = (redOffset + PHASE_PEAK) * index / PHASE_RAMP_COLORS;
		green = (greenOffset + PHASE_PEAK) * index / PHASE_RAMP_COLORS;
		blue = (blueOffset + PHASE_PEAK) * index / PHASE_RAMP_COLORS;

		CLAMP(red, 0, RGB_LEVEL_MAX);
		CLAMP(green, 0, RGB_LEVEL_MAX);
		CLAMP(blue, 0, RGB_LEVEL_MAX);

		palette[index] = RGB(red, green, blue);
	}

	/* Entry 0, black, goes to colour 1, so canvas level n shows entry n - 1. */
	syWordsCopy((void *)&BG_PLTT_COLOR(0, 1), palette, sizeof(palette));
}

#undef PHASE_RAMP_COLORS
#undef PHASE_PEAK
#undef PRISM_SHARD_PICTURE_COUNT

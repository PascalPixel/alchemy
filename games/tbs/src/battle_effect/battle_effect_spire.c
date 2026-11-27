/* @file */
/* Spire. */
/* Part of battle_effect.c. */

#define SPIRE_ROCK_WIDTH                                                       \
	34 /* the boulder picture, the first in the sprite sheet */
#define SPIRE_ROCK_HEIGHT 62 /* the boulder picture's height */
/* The shard pictures in the sprite sheet, after the boulder. */
#define SPIRE_SHARD_PICTURES (SPIRE_ROCK_WIDTH * SPIRE_ROCK_HEIGHT)
#define SPIRE_SHARDS		 21 /* shards per rock */
/* The shard shapes, each drawn in SPIRE_SHARD_TUMBLE_PICTURES as it tumbles. */
#define SPIRE_SHARD_SHAPES			5
#define SPIRE_SHARD_TUMBLE_PICTURES 3
/* How far a shard turns between its pictures; it turns by its spin (its z, */
/* from 32 slowing to 1) each frame. */
#define SPIRE_SHARD_TUMBLE_STEP 96
/* Frames a boulder falls before it lands and strikes the targets. */
#define SPIRE_FALL_FRAMES 18

/* Boulders drop along the row of targets, centred on it, and shatter into */
/* tumbling shards, hitting every target as each lands. */
/* */
/* Abilities: Spire, Clay Spire, Stone Spire. */
void btEffectRunSpire(BATTLE_EFFECT_PARM *args) {
	BATTLE_EFFECT_STATE *state;
	void *canvas;
	DRAW_RECTANGLE blit;
	s32 rockCount;
	SCR_POINT first;
	SCR_POINT last;
	PARTICLE *rock;
	PARTICLE *shard;
	s32 frame;
	s32 i;
	s32 j;
	s32 picture;
	u32 width;
	u32 height;

	state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
	state->args = args;
	EFFECT_START_NARROW_CANVAS(BGCNT_PRIORITY(1));
	REG_BLDCNT = 0;
	btResourceLoadAndDecompress(RESOURCE_BOULDER_SHEET, state->sheet, 1, 1);
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
						COMPOSITE_TRANSPARENT);
	blit = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
	btProjectionScaledUnit(state->args->tgts[0], &first);
	btProjectionScaledUnit(state->args->tgts[state->args->tgtCnt - 1], &last);
	first.x += (last.x - first.x) / 2;
	REG_BG2X = (CANVAS_SIZE / 2 - first.x) << 8;
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
	rockCount = Spire_RockCounts[state->args->variant];
	for(i = 0; i != rockCount; i++) {
		rock = &state->particles[i];
		rock->y = -toFIXED(64);
		rock->velY = 0;
	}
	for(i = 0; i != rockCount; i++) {
		for(j = 0; j != SPIRE_SHARDS; j++) {
			PARTICLE *shard = &PARTICLES[i * SPIRE_SHARDS + j];

			shard->x = INTtoFIXED(Spire_ShardOffsets[j][SPIRE_SHARD_X] +
								  Spire_RockX[i]);
			shard->y = INTtoFIXED(Spire_ShardOffsets[j][SPIRE_SHARD_Y]);
			shard->velX = ((syRandom() % 96) - 48) << 10;
			shard->velY = -((syRandom() & 127) + 32) << 11;
			shard->z = 32;
			shard->variant = 0;
		}
	}
	for(frame = 0; frame != Spire_DropFrames[rockCount - 1] + 80; frame++) {
		if(frame == Spire_DropFrames[rockCount - 1] + 48)
			EFFECT_SHOW_DAMAGE(SE_WATER);
		for(i = 0; i != rockCount; i++) {
			rock = &state->particles[i];
			if(frame == Spire_DropFrames[i] + SPIRE_FALL_FRAMES) {
				sySndPlayStub(SE_HEAVY_IMPACT);
				state->shakeFrm = 4;
			}
			if(frame >= Spire_DropFrames[i] + SPIRE_FALL_FRAMES) {
				for(j = 0; j != SPIRE_SHARDS; j++) {
					shard = &PARTICLES[i * SPIRE_SHARDS + j];
					picture =
						(j % SPIRE_SHARD_SHAPES) * SPIRE_SHARD_TUMBLE_PICTURES +
						shard->variant / SPIRE_SHARD_TUMBLE_STEP %
							SPIRE_SHARD_TUMBLE_PICTURES;
					/* UB: width is assigned in one argument and read in
					 * another. */
#ifdef UBFIX
					width = Spire_ShardWidths[picture];
					height = Spire_ShardHeights[picture];
					blit(canvas,
						 state->sheet + Spire_ShardPictures[picture] +
							 SPIRE_SHARD_PICTURES,
						 FIXEDtoINT(shard->x) - (width >> 1),
						 FIXEDtoINT(shard->y) - (height >> 1), width, height);
#else
					blit(canvas,
						 state->sheet + Spire_ShardPictures[picture] +
							 SPIRE_SHARD_PICTURES,
						 FIXEDtoINT(shard->x) -
							 ((width = Spire_ShardWidths[picture]) >> 1),
						 FIXEDtoINT(shard->y) -
							 ((height = Spire_ShardHeights[picture]) >> 1),
						 width, height);
#endif
					btParticleAdvanceWithGravity2D(shard, PARTICLE_DAMPING_NONE,
												   toFIXED(1) / 4);
					shard->variant += shard->z;
					if(shard->z > 1 && (frame & 1))
						shard->z--;
				}
			}
			/* The boulder falls over its column. */
			if(frame < Spire_DropFrames[i] + SPIRE_FALL_FRAMES) {
				if(frame >= Spire_DropFrames[i])
					blit(canvas, state->sheet,
						 Spire_RockX[i] + CANVAS_SIZE / 2 -
							 SPIRE_ROCK_WIDTH / 2,
						 FIXEDtoINT(rock->y), SPIRE_ROCK_WIDTH,
						 SPIRE_ROCK_HEIGHT);
				rock->y += rock->velY;
				if(frame > Spire_DropFrames[i])
					rock->velY += toFIXED(1);
				if(rock->y > toFIXED(50))
					rock->y = toFIXED(50);
			}
			if(frame == Spire_DropFrames[i] + SPIRE_FALL_FRAMES) {
				EFFECT_FOR_EACH_TARGET(j, state->args)
					btEffectTintBattler(state->args->tgts[j],
										SPRITE_COLOR_WHITE, POSE_DAMAGE, j, 8);
			}
		}
		btCameraShake(2, 4);
		EFFECT_END_FRAME(state);
	}
	EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

#undef SPIRE_ROCK_WIDTH
#undef SPIRE_ROCK_HEIGHT
#undef SPIRE_SHARD_PICTURES
#undef SPIRE_SHARDS
#undef SPIRE_SHARD_SHAPES
#undef SPIRE_SHARD_TUMBLE_PICTURES
#undef SPIRE_SHARD_TUMBLE_STEP
#undef SPIRE_FALL_FRAMES

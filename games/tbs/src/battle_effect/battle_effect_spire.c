/* Spire. Part of battle_effect.c. */

#define SPIRE_ROCK_WIDTH \
	34 /* the boulder picture, the first in the sprite sheet */
#define SPIRE_ROCK_HEIGHT			62 /* the boulder picture's height */
/* The shard pictures in the sprite sheet, after the boulder. */
#define SPIRE_SHARD_PICTURES		(SPIRE_ROCK_WIDTH * SPIRE_ROCK_HEIGHT)
#define SPIRE_SHARDS				21 /* shards per rock */
/* The shard shapes, each drawn in SPIRE_SHARD_TUMBLE_PICTURES as it tumbles. */
#define SPIRE_SHARD_SHAPES			5
#define SPIRE_SHARD_TUMBLE_PICTURES 3
/* How far a shard turns between its pictures; it turns by its spin (its z, from
   32 slowing to 1) each tick. */
#define SPIRE_SHARD_TUMBLE_STEP		96
/* Ticks a boulder falls before it lands and strikes the targets. */
#define SPIRE_FALL_TICKS			18

/* Boulders drop along the row of targets, centred on it, and shatter into
   tumbling shards, hitting every target as each lands.

   Abilities: Spire, Clay Spire, Stone Spire. */
void btEffectRunSpire(BATTLE_EFFECT_PARM *args)
{
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	DRAW_RECTANGLE		 blit;
	Sint32				 rockCount;
	SCREEN_POINT		 first;
	SCREEN_POINT		 last;
	PARTICLE			*rock;
	PARTICLE			*shard;
	Sint32				 frame;
	Sint32				 i;
	Sint32				 j;
	Sint32				 picture;
	Uint32				 width;
	Uint32				 height;

	state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
	state->args = args;
	EFFECT_START_NARROW_CANVAS(BGCNT_PRIORITY(1));
	REG_BLDCNT = 0;
	btResourceLoadAndDecompress(RESOURCE_BOULDER_SHEET, state->sheet, 1, 1);
	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, COMPOSITE_TRANSPARENT);
	blit = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
	btProjectionScaledUnit(state->args->targets[0], &first);
	btProjectionScaledUnit(
		state->args->targets[state->args->targetCount - 1], &last);
	first.x += (last.x - first.x) / 2;
	REG_BG2X = (CANVAS_SIZE / 2 - first.x) << 8;
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
	rockCount = SpireRockCounts[state->args->variant];
	for(i = 0; i != rockCount; i++) {
		rock = &state->particles[i];
		rock->y = -toFIXED(64);
		rock->velocityY = 0;
	}
	for(i = 0; i != rockCount; i++) {
		for(j = 0; j != SPIRE_SHARDS; j++) {
			PARTICLE *shard = &PARTICLES[i * SPIRE_SHARDS + j];

			shard->x =
				toFIXED(SpireShardOffsets[j][SPIRE_SHARD_X] + SpireRockX[i]);
			shard->y = toFIXED(SpireShardOffsets[j][SPIRE_SHARD_Y]);
			shard->velocityX = ((syRandom() % 96) - 48) << 10;
			shard->velocityY = -((syRandom() & 127) + 32) << 11;
			shard->z = 32;
			shard->variant = 0;
		}
	}
	for(frame = 0; frame != SpireDropTicks[rockCount - 1] + 80; frame++) {
		if(frame == SpireDropTicks[rockCount - 1] + 48)
			EFFECT_SHOW_DAMAGE(SE_WATER);
		for(i = 0; i != rockCount; i++) {
			rock = &state->particles[i];
			if(frame == SpireDropTicks[i] + SPIRE_FALL_TICKS) {
				sySoundPlayStub(SE_HEAVY_IMPACT);
				state->shakeFrame = 4;
			}
			if(frame >= SpireDropTicks[i] + SPIRE_FALL_TICKS) {
				for(j = 0; j != SPIRE_SHARDS; j++) {
					shard = &PARTICLES[i * SPIRE_SHARDS + j];
					picture =
						(j % SPIRE_SHARD_SHAPES) * SPIRE_SHARD_TUMBLE_PICTURES +
						shard->variant / SPIRE_SHARD_TUMBLE_STEP %
							SPIRE_SHARD_TUMBLE_PICTURES;
					/* UB: width is assigned in one argument and read in
					   another. */
#if FIX_UNSEQUENCED_SIDE_EFFECT
					width = SpireShardWidths[picture];
					height = SpireShardHeights[picture];
					blit(canvas,
						state->sheet + SpireShardPictures[picture] +
							SPIRE_SHARD_PICTURES,
						FIXEDtoINT(shard->x) - (width >> 1),
						FIXEDtoINT(shard->y) - (height >> 1), width, height);
#else
					blit(canvas,
						state->sheet + SpireShardPictures[picture] +
							SPIRE_SHARD_PICTURES,
						FIXEDtoINT(shard->x) -
							((width = SpireShardWidths[picture]) >> 1),
						FIXEDtoINT(shard->y) -
							((height = SpireShardHeights[picture]) >> 1),
						width, height);
#endif
					btParticleAdvanceWithGravity2D(
						shard, PARTICLE_DAMPING_NONE, toFIXED(1) / 4);
					shard->variant += shard->z;
					if(shard->z > 1 && (frame & 1))
						shard->z--;
				}
			}
			/* The boulder falls over its column. */
			if(frame < SpireDropTicks[i] + SPIRE_FALL_TICKS) {
				if(frame >= SpireDropTicks[i])
					blit(canvas, state->sheet,
						SpireRockX[i] + CANVAS_SIZE / 2 - SPIRE_ROCK_WIDTH / 2,
						FIXEDtoINT(rock->y), SPIRE_ROCK_WIDTH,
						SPIRE_ROCK_HEIGHT);
				rock->y += rock->velocityY;
				if(frame > SpireDropTicks[i])
					rock->velocityY += toFIXED(1);
				if(rock->y > toFIXED(50))
					rock->y = toFIXED(50);
			}
			if(frame == SpireDropTicks[i] + SPIRE_FALL_TICKS) {
				EFFECT_FOR_EACH_TARGET(j, state->args)
					btEffectTintBattler(state->args->targets[j],
						SPRITE_COLOR_WHITE, POSE_DAMAGE, j, 8);
			}
		}
		btCameraShake(2, 4);
		EFFECT_END_TICK(state);
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
#undef SPIRE_FALL_TICKS

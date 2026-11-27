/* Spire. Part of battle_effect.c. */

#define SPIRE_SHARDS	 21 /* shards per rock */
/* Ticks a boulder falls before it lands and strikes the targets. */
#define SPIRE_FALL_TICKS 18

/* Boulders drop along the row of targets, centred on it, and shatter into
   tumbling shards, hitting every target as each lands.

   Abilities: Spire, Clay Spire, Stone Spire. */
void btEffectRunSpire(BATTLE_EFFECT_PARM *args)
{
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	DRAW_RECTANGLE		 blit;
	Sint32				 rockCnt;
	SCREEN_POINT		 first;
	SCREEN_POINT		 last;
	PARTICLE			*rock;
	PARTICLE			*shard;
	Sint32				 frm;
	Sint32				 i;
	Sint32				 j;
	Sint32				 picture;
	Uint32				 width;
	Uint32				 height;

	state = Ram_HeapSlots->battleEff;
	canvas = Ram_HeapSlots->battleCanvas;
	EFFECT_START_NARROW_CANVAS(BGCNT_PRIORITY(1));
	REG_BLDCNT = 0;
	btResourceLoadAndDecompress(RESOURCE_BOULDER_SHEET, state->sheet, 1, 1);
	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_TRANSPARENT);
	blit = Ram_HeapSlots->ramCode;

	/* The canvas is centred between the first and the last target. */
	btEffectPosProjectScaledBattler(args->tgts[0], &first);
	btEffectPosProjectScaledBattler(args->tgts[args->tgtCnt - 1], &last);
	first.x += (last.x - first.x) / 2;
	REG_BG2X = (CANVAS_SIZE / 2 - first.x) << 8;
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);

	/* Each boulder waits 64 pixels above the canvas; each shard sits at its
	   place in its boulder, ready to fly up at random. */
	rockCnt = SpireRockCounts[args->variant];
	for(i = 0; i != rockCnt; i++) {
		rock = &state->particles[i];
		rock->y = -toFIXED(64);
		rock->velY = 0;
	}
	for(i = 0; i != rockCnt; i++) {
		for(j = 0; j != SPIRE_SHARDS; j++) {
			PARTICLE *shard = &PARTICLES[i * SPIRE_SHARDS + j];

			shard->x = toFIXED(SpireShardOffsets[j * 2] + SpireRockX[i]);
			shard->y = toFIXED(SpireShardOffsets[j * 2 + 1]);
			shard->velX = ((syRandom() % 96) - 48) << 10;
			shard->velY = -((syRandom() & 127) + 32) << 11;
			shard->z = 32;
			shard->variant = 0;
		}
	}

	for(frm = 0; frm != SpireDropTicks[rockCnt - 1] + 65; frm++) {
		/* DEAD: the first boulder's crash has already let the battle show the
		   damage, so this second use changes nothing; the ROM keeps the test
		   and the call. */
		if(frm == SpireDropTicks[rockCnt - 1] + 48)
			EFFECT_SHOW_DAMAGE(SE_IMPACT);
		for(i = 0; i != rockCnt; i++) {
			rock = &state->particles[i];
			/* Each boulder crashes and shakes the screen; the first lets the
			   battle show the damage. */
			if(frm == SpireDropTicks[i] + SPIRE_FALL_TICKS) {
				if(i == 0)
					EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
				else
					sySndPlayStub(SE_HEAVY_IMPACT);
				state->shakeFrm = 4;
			}
			/* Its shards fly, each turning through its kind's three pictures,
			   more slowly every other tick. */
			if(frm >= SpireDropTicks[i] + SPIRE_FALL_TICKS) {
				for(j = 0; j != SPIRE_SHARDS; j++) {
					shard = &PARTICLES[i * SPIRE_SHARDS + j];
					picture = (j % 5) * 3 + shard->variant / SHARD_TURN % 3;
					/* UB: width is assigned in one argument and read in
					   another. */
#if FIX_UNSEQUENCED_SIDE_EFFECT
					width = SpireShardWidths[picture];
					height = SpireShardHeights[picture];
					blit(canvas,
						state->sheet + SpireShardPictures[picture] +
							BOULDER_SHARD_PICTURES,
						FIXED_INTEGER_PART(shard->x) - (width >> 1),
						FIXED_INTEGER_PART(shard->y) - (height >> 1), width,
						height);
#else
					blit(canvas,
						state->sheet + SpireShardPictures[picture] +
							BOULDER_SHARD_PICTURES,
						FIXED_INTEGER_PART(shard->x) -
							((width = SpireShardWidths[picture]) >> 1),
						FIXED_INTEGER_PART(shard->y) -
							((height = SpireShardHeights[picture]) >> 1),
						width, height);
#endif
					btParticleAdvanceWithGravity2D(
						shard, PARTICLE_DAMPING_NONE, toFIXED(1) / 4);
					shard->variant += shard->z;
					if(shard->z > 1 && (frm & 1))
						shard->z--;
				}
			}
			/* Until then the boulder falls, a pixel a tick faster each tick,
			   and rests at y 50. */
			if(frm < SpireDropTicks[i] + SPIRE_FALL_TICKS) {
				if(frm >= SpireDropTicks[i])
					blit(canvas, state->sheet,
						SpireRockX[i] + CANVAS_SIZE / 2 - BOULDER_WIDTH / 2,
						FIXED_INTEGER_PART(rock->y), BOULDER_WIDTH,
						BOULDER_HEIGHT);
				rock->y += rock->velY;
				if(frm > SpireDropTicks[i])
					rock->velY += toFIXED(1);
				if(rock->y > toFIXED(50))
					rock->y = toFIXED(50);
			}
			if(frm == SpireDropTicks[i] + SPIRE_FALL_TICKS) {
				EFFECT_FOR_EACH_TARGET(j, args)
					btEffectTintBattler(
						args->tgts[j], SPRITE_COLOR_WHITE, POSE_DAMAGE, j, 8);
			}
		}
		btCameraShake(2, 4);
		EFFECT_END_TICK(state);
	}

	EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

#undef SPIRE_SHARDS
#undef SPIRE_FALL_TICKS

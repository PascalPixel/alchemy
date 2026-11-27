/* @file */
/* Break and Drain. */
/* Part of battle_effect.c. */

/* Break and Drain take the targets in turn, this many frames apart. */
#define DRAIN_TARGET_FRAMES 20

#define BREAK_SHARDS_PER_TARGET 128
/* Break runs DRAIN_TARGET_FRAMES a target and then this many while the last */
/* shards fall. */
#define BREAK_FALL_FRAMES 56
#define BREAK_HIT_FRAME	  32
/* The canvas row below which a falling shard goes out. */
#define BREAK_SHARD_GROUND 112

/* Ice shards burst up from each target in turn, tinting it, and fall away. */
/* */
/* Abilities: Break. */
void btEffectRunBreak(BATTLE_EFFECT_PARM *args) {
	BATTLE_EFFECT_STATE *state;
	void *canvas;
	DRAW_RECTANGLE draw;
	u8 *sheet;
	BATTLE_CAMERA *camera;
	s32 point[3];
	SCR_POINT screen;
	s32 member;
	s32 frame;
	s32 i;

	state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
	sheet = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
	camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
	state->args = args;
	btEffectStartCanvasLayer(BGCNT_PRIORITY(1));
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sheet, 0, 0);
	syWordsCopy(BG_PLTT, syResourceGetTableEntry(RESOURCE_ICE_BLOCK_SHEET),
				SHEET_PALETTE_BYTES);
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
						COMPOSITE_LIGHTEN);
	draw = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);

	EFFECT_FREE_PARTICLES(i, PARTICLES, 8 * BREAK_SHARDS_PER_TARGET);

	EFFECT_VIEW_FROM_CAMERA(camera);

	/* Scatter the shards around the middle of each body. */
	EFFECT_FOR_EACH_TARGET(member, state->args) {
		ACTOR *actor;
		s32 offset;
		s32 height;
		PARTICLE *shard;

		offset = member * BREAK_SHARDS_PER_TARGET * sizeof(PARTICLE);
		actor = btObjectGetStub(state->args->tgts[member])->actor;
		height = btUnitGetEffectHeightStub(state->args->tgts[member]) / 2;
		SET_VECTOR(point, actor->x.fixed, height, actor->z.fixed);
		btProjectionPoint(point, &screen);
		screen.x >>= 1;
		for(i = 0, shard = (PARTICLE *)(Ram_MapGridCellBuffer + offset);
			i != BREAK_SHARDS_PER_TARGET; i++) {
			s32 radius;
			s32 angle;
			s32 lift;

			radius = syRandom() & 0xff;
			angle = syRandom() & TRIG_ANGLE_MASK;
			shard->x = ((sySin(angle) * radius) >> 7) + INTtoFIXED(screen.x);
			shard->y = ((syCos(angle) * radius) >> 3) + INTtoFIXED(screen.y);
			/* Thrown up at 2 to 6 pixels a frame, and up to a pixel a frame
			 * aside. */
			shard->velX = (128 - (syRandom() & 0xff)) << 9;
			lift = syRandom() & 0xff;
			shard->variant = 0;
			shard->velY = (-lift - 128) << 10;
			shard++;
		}
	}

	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

	for(frame = 0;
		frame != state->args->tgtCnt * DRAIN_TARGET_FRAMES + BREAK_FALL_FRAMES;
		frame++) {
		if(frame == BREAK_HIT_FRAME)
			EFFECT_SHOW_DAMAGE(0);
		EFFECT_FOR_EACH_TARGET(member, state->args) {
			s32 offset;

			offset = member * BREAK_SHARDS_PER_TARGET * sizeof(PARTICLE);
			if(frame == member * DRAIN_TARGET_FRAMES) {
				sySndPlayStub(SE_CRACK);
				btEffectTintBattler(state->args->tgts[member],
									SPRITE_COLOR_WHITE, TINT_KEEP, member, 20);
			}
			/* A shard falls and goes out below the ground. */
			if(frame > member * DRAIN_TARGET_FRAMES) {
				PARTICLE *shard;

				for(i = 0, shard = (PARTICLE *)(Ram_MapGridCellBuffer + offset);
					i != BREAK_SHARDS_PER_TARGET; i++) {
					if(shard->variant >= 0) {
						s32 size;

						size = i % 3 + 1;
						EFFECT_DRAW_MOTE(draw, canvas, sheet,
										 FIXEDtoINT(shard->x),
										 FIXEDtoINT(shard->y), size);
						btParticleAdvanceWithGravity2D(shard, 62,
													   Drain_Gravities[i & 3]);
						shard->variant++;
						if(shard->velY > 0 &&
						   FIXEDtoINT(shard->y) > BREAK_SHARD_GROUND)
							shard->variant = -1;
					}
					shard++;
				}
			}
		}
		EFFECT_END_FRAME(state);
	}

	EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

#define DRAIN_MOTES_PER_TARGET 128
#define DRAIN_DRAWN			   32 /* motes drawn for each target */
#define DRAIN_MOTE_SIZE		   6  /* a mote's size in pixels */
/* Drain runs DRAIN_TARGET_FRAMES a target and then this many; the user glows */
/* from DRAIN_GLOW_FRAME until DRAIN_GLOW_END_FRAMES before the end. */
#define DRAIN_END_FRAMES	  72
#define DRAIN_GLOW_FRAME	  40
#define DRAIN_GLOW_END_FRAMES 20
#define DRAIN_HIT_FRAME		  64
/* A mote homes in on the user this many frames after its target's turn, */
/* each mote a frame after the one before, gaining a 256th of the distance a */
/* frame. */
#define DRAIN_PULL_DELAY	 10
#define DRAIN_PULL(distance) ((distance) >> 8)
/* A mote goes out within this many pixels of the middle of the user, across */
/* and in depth. */
#define DRAIN_ARRIVAL_DISTANCE 16

/* Motes of light burst from each target in turn, and are */
/* pulled into the centre of the user's body. */
/* */
/* Abilities: Drain, Psy Drain. */
void btEffectRunDrain(BATTLE_EFFECT_PARM *args) {
	BATTLE_EFFECT_STATE *state;
	void *canvas;
	DRAW_RECTANGLE draw;
	u8 *sheet;
	BATTLE_CAMERA *camera;
	SCR_POINT screen;
	s32 alternate;
	s32 member;
	s32 frame;
	s32 i;

	state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
	sheet = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
	camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
	if(args->variant == EFFECT_VARIANT_BASIC)
		alternate = 0;
	else
		alternate = 1;
	state->args = args;
	btEffectStartCanvasLayer(BGCNT_PRIORITY(1));
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sheet, 0, 0);
	syWordsCopy(BG_PLTT,
				syResourceGetTableEntry(alternate == 0
											? RESOURCE_ICE_BLOCK_SHEET
											: RESOURCE_BLAST_SHEET),
				SHEET_PALETTE_BYTES);

	/* The motes fly out of each target's body. */
	EFFECT_FREE_PARTICLES(i, PARTICLES, 8 * DRAIN_MOTES_PER_TARGET);

	EFFECT_FOR_EACH_TARGET(member, state->args) {
		ACTOR *actor;
		s32 halfHeight;

		actor = btObjectGetStub(state->args->tgts[member])->actor;
		halfHeight = btUnitGetEffectHeightStub(state->args->tgts[member]) / 2;
		for(i = 0; i != DRAIN_MOTES_PER_TARGET; i++) {
			PARTICLE *mote = &PARTICLES[member * DRAIN_MOTES_PER_TARGET + i];

			SET_POSITION(mote, actor->x.fixed, actor->y.fixed + halfHeight,
						 actor->z.fixed);
			SET_VELOCITY(mote, ((syRandom() & 0xff) - 128) << 10,
						 ((syRandom() & 0xff) - 128) << 10,
						 ((syRandom() & 0xff) - 128) << 10);
			mote->variant = 0;
		}
	}

	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
						COMPOSITE_LIGHTEN);
	draw = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(4));
	sySndPlayStub(SE_SWIRL);

	for(frame = 0;
		frame != state->args->tgtCnt * DRAIN_TARGET_FRAMES + DRAIN_END_FRAMES;
		frame++) {
		ACTOR *user;
		s32 halfHeight;

		user = btObjectGetStub(state->args->user)->actor;
		halfHeight = btUnitGetEffectHeightStub(state->args->user) / 2;
		if(frame == DRAIN_HIT_FRAME)
			EFFECT_SHOW_DAMAGE(SE_IMPACT);
		EFFECT_VIEW_FROM_CAMERA(camera);
		if(frame == DRAIN_GLOW_FRAME)
			btEffectTintBattler(state->args->user, SPRITE_COLOR_WHITE,
								TINT_KEEP, TINT_KEEP, 0);
		if(frame == state->args->tgtCnt * DRAIN_TARGET_FRAMES +
						DRAIN_END_FRAMES - DRAIN_GLOW_END_FRAMES)
			btEffectTintBattler(state->args->user, SPRITE_COLOR_NORMAL,
								TINT_KEEP, TINT_KEEP, 0);
		EFFECT_FOR_EACH_TARGET(member, state->args) {
			if(frame == member * DRAIN_TARGET_FRAMES)
				btEffectTintBattler(state->args->tgts[member],
									SPRITE_COLOR_WHITE, POSE_DAMAGE, member,
									42);
			if(frame > member * DRAIN_TARGET_FRAMES) {
				for(i = 0; i != DRAIN_DRAWN; i++) {
					PARTICLE *mote =
						&PARTICLES[member * DRAIN_MOTES_PER_TARGET + i];

					if(mote->variant >= 0) {
						s32 size = DRAIN_MOTE_SIZE;

						btProjectionPoint(mote, &screen);
						screen.x >>= 1;
						EFFECT_DRAW_MOTE(draw, canvas, sheet, screen.x,
										 screen.y, size);
						btParticleAdvanceWithGravity3D(mote, 62, 0);

						/* After a moment a mote is pulled into the user. */
						if(frame > member * DRAIN_TARGET_FRAMES + i +
									   DRAIN_PULL_DELAY) {
							s32 dx;
							s32 dy;
							s32 dz;

							dx = DRAIN_PULL(user->x.fixed - mote->x);
							dy = DRAIN_PULL(user->y.fixed + halfHeight -
											mote->y);
							dz = DRAIN_PULL(user->z.fixed - mote->z);
							mote->velX += dx;
							mote->velY += dy;
							mote->velZ += dz;
							if((dx > -DRAIN_PULL(
										 toFIXED(DRAIN_ARRIVAL_DISTANCE)) &&
								dx < DRAIN_PULL(
										 toFIXED(DRAIN_ARRIVAL_DISTANCE))) &&
							   (dz > -DRAIN_PULL(
										 toFIXED(DRAIN_ARRIVAL_DISTANCE)) &&
								dz < DRAIN_PULL(
										 toFIXED(DRAIN_ARRIVAL_DISTANCE))))
								mote->variant = -1;
						}
					}
				}
			}
		}
		EFFECT_END_FRAME(state);
	}

	EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

#undef DRAIN_TARGET_FRAMES
#undef BREAK_SHARDS_PER_TARGET
#undef BREAK_FALL_FRAMES
#undef BREAK_HIT_FRAME
#undef BREAK_SHARD_GROUND
#undef DRAIN_MOTES_PER_TARGET
#undef DRAIN_DRAWN
#undef DRAIN_MOTE_SIZE
#undef DRAIN_END_FRAMES
#undef DRAIN_GLOW_FRAME
#undef DRAIN_GLOW_END_FRAMES
#undef DRAIN_HIT_FRAME
#undef DRAIN_PULL_DELAY
#undef DRAIN_PULL
#undef DRAIN_ARRIVAL_DISTANCE

/* @file */
/* Swirling stars, orbiting skulls and target bursts. */
/* Part of battle_effect.c. */

/* None of the three effects here is called or named by any table: nothing in */
/* the game reaches them. */

#define SWIRLING_STARS_FRAMES 160
#define SWIRLING_STARS_COUNT  64 /* stars that swirl */
#define SWIRLING_STARS_PLACED                                                  \
	256 /* stars given a place, the first to swirl                             \
		 */
/* A star is drawn SWIRLING_STAR_LARGEST pixels wide (and twice as tall) at */
/* the nearest depth, a pixel narrower for every SWIRLING_STAR_DEPTH_STEP */
/* farther, its depth clamped from SWIRLING_STAR_DEPTH_NEAR to */
/* SWIRLING_STAR_DEPTH_FAR. */
#define SWIRLING_STAR_LARGEST	 9
#define SWIRLING_STAR_DEPTH_NEAR 250
#define SWIRLING_STAR_DEPTH_FAR	 634
#define SWIRLING_STAR_DEPTH_STEP 64
/* The angle star `i` turns each frame about its axis: each later star faster.
 */
#define SWIRLING_STAR_SPIN(i) (((i) + 32) * 8)

/* Swirling stars: stars spin about a point above the field, each on its own */
/* axis, then are drawn back towards it. */
UNUSED void btEffectRunSwirlingStars(BATTLE_EFFECT_PARM *args) {
	BATTLE_EFFECT_STATE *state;
	void *canvas;
	s32 center[3];
	SCR_POINT screen;
	BLITTER_PAIR draw;
	PARTICLE *star;
	s32 i;
	s32 frame;

	state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
	state->args = args;
	btEffectStartCanvasLayer(0);
	btResourceLoadAndDecompress(RESOURCE_STAR_DOT_SHEET, state->sheet, 1, 1);
	btEffectBuildCanvasBlitters(state->args->side ^ 1, draw);
	for(i = 0; i != SWIRLING_STARS_PLACED; i++) {
		star = &PARTICLES[i];
		SET_POSITION(star, INTtoFIXED((syRandom() & 0xff) - 127),
					 INTtoFIXED((syRandom() & 0xff) - 127),
					 INTtoFIXED((syRandom() & 0xff) - 127));
		SET_VELOCITY(star, 0, 0, 0);
		star->variant = 0;
	}
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
	SET_VECTOR(center, 0, toFIXED(80), 0);
	for(frame = 0; frame != SWIRLING_STARS_FRAMES; frame++) {
		BATTLE_CAMERA *camera = BattleCameraWork;

		EFFECT_VIEW_FROM_CAMERA(camera);
		syMatrixTranslateVector(center);
		for(i = 0; i != SWIRLING_STARS_COUNT; i++) {
			star = &PARTICLES[i];
			if(frame > i / 4 && star->variant == 0) {
				s32 size;

				syPushMatrix();
				switch(i & 3) {
				case SPIN_YAW:
					syRotY(frame * SWIRLING_STAR_SPIN(i));
					break;
				case SPIN_PITCH:
					syRotX(-frame * SWIRLING_STAR_SPIN(i));
					break;
				case SPIN_ROLL:
					syRotZ(-frame * SWIRLING_STAR_SPIN(i));
					break;
				case SPIN_PITCH_AND_ROLL:
					syRotX(-frame * SWIRLING_STAR_SPIN(i));
					syRotZ(-frame * SWIRLING_STAR_SPIN(i));
					break;
				}
				btProjectionPoint(star, &screen);
				screen.x >>= 1;
				syPopMatrix();
				CLAMP(screen.depth, SWIRLING_STAR_DEPTH_NEAR,
					  SWIRLING_STAR_DEPTH_FAR);
				size = SWIRLING_STAR_LARGEST -
					   (screen.depth - SWIRLING_STAR_DEPTH_NEAR) /
						   SWIRLING_STAR_DEPTH_STEP;
				draw[0](canvas,
						state->sheet + ((i & 3) * MOTE_SET_BYTES +
										BattleEffect_MotePictures[size - 1]),
						screen.x - size / 2, screen.y - size, size, size * 2);
				btParticleAdvanceWithGravity3D(star, 60, 0);
				/* After 30 frames a star springs back towards the centre. */
				if(frame > i / 4 + 30) {
					s32 dx = -star->x >> 8;
					s32 dy = -star->y >> 8;
					s32 dz = -star->z >> 8;

					star->velX += dx;
					star->velY += dy;
					star->velZ += dz;
				}
			}
		}
		EFFECT_PRESENT_CANVAS(state);
	}
	EFFECT_FINISH_CANVAS();
}

/* Orbiting skulls: three skulls circle each target in turn. */
UNUSED void btEffectRunOrbitingSkulls(void) {
	BATTLE_EFFECT_STATE *state;
	void *canvas;
	s32 point[3];
	SCR_POINT center;
	SCR_POINT screen;
	s32 frame;
	/* LEFTOVER(frame): draw[1], the idle half of the pair. */
	BLITTER_PAIR draw;
	s32 member;
	BATTLE_CAMERA *camera;
	u8 *skulls;
	s32 xOffset;
	s32 i;

	state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
	camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
	EFFECT_START_NARROW_CANVAS(BGCNT_PRIORITY(1));
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	skulls = syResourceGetTableEntry(RESOURCE_SKULL_SHEET);
	syWordsCopy(BG_PLTT, skulls, SHEET_PALETTE_BYTES);
	fdDecompressTaggedLz(skulls + SHEET_PALETTE_BYTES, state->sheet);
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
						COMPOSITE_LIGHTEN);
	EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
	EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE,
						CANVAS_FADE_THREE_QUARTERS);
	draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
	sySetEvent(btEffectTransferCanvas, TASK_PASS_VBLANK);
	if(state->args->side == EFFECT_SIDE_ENEMIES)
		xOffset = 0;
	else
		xOffset = -NARROW_CANVAS_PARTY_X;
	/* FAKEMATCH(dead): the scroll's store in a once-only block. */
	do {
		REG_BG2X = xOffset << 8;
	} while(0);
	for(i = 0; i != ARRAY_COUNT(state->particles); i++) {
		PARTICLE *skull = &state->particles[i];

		skull->x = 0;
		skull->y = 0;
		skull->velX = 0;
		skull->z = 4;
	}
	for(frame = 0; frame != state->args->tgtCnt * 16 + 64; frame++) {
		EFFECT_FOR_EACH_TARGET(member, state->args) {
			ACTOR *actor;
			u32 memberFrame;

			actor = btObjectGetStub(state->args->tgts[member])->actor;
			btProjectionScaledUnit(state->args->tgts[member], &screen);
			memberFrame = frame - member * 16;
			screen.x += xOffset;
			if(memberFrame < 64) {
				/* Default: NULL until it is pointed at the point below. */
				/* FAKEMATCH(temporary): rec, a pointer standing in for &point.
				 */
				s32 *rec = NULL;

				EFFECT_VIEW_FROM_CAMERA(camera);
				rec = point;
				SET_VECTOR(rec, actor->x.fixed, actor->y.fixed, actor->z.fixed);
				btProjectionPoint(rec, &center);
				center.x += xOffset;
				/* FAKEMATCH(dead): an empty once-only loop. */
				do {
				} while(0);
				for(i = 0; i != 3; i++) {
					/* A skull's velocityX is its angle round the target. */
					PARTICLE *skull = &state->particles[member * 3 + i];
					s32 x;
					s32 y;

					x = screen.x +
						FIXEDtoINT(
							(sySin(skull->velX + i * (DEGtoANG(120))) * 8));
					y = screen.y +
						FIXEDtoINT(
							(syCos(skull->velX + i * (DEGtoANG(120))) * 8));
					skull->velX += TRIG_TURN / 128;
					draw[0](canvas, state->sheet + i * PICTURE_BYTES(SKULL),
							x - SKULL_WIDTH / 2, y - 28, SKULL_WIDTH,
							SKULL_HEIGHT);
				}
			}
		}
		EFFECT_PRESENT_CANVAS(state);
	}
	EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

/* The burst's six pictures in the sprite sheet, after the smoke's six. */
#define TARGET_BURST_PICTURES (6 * PICTURE_BYTES(SMOKE_PUFF))

#define TARGET_BURSTS_FRAMES 96

/* Target bursts: on each target in turn a burst flares while puffs of smoke */
/* scatter around it. */
UNUSED void btEffectRunTargetBursts(BATTLE_EFFECT_PARM *args) {
	BATTLE_EFFECT_STATE *state;
	void *canvas;
	void *palette;
	s32 frame;
	DRAW_RECTANGLE drawSmoke;
	DRAW_RECTANGLE drawBurst;
	PARTICLE *puff;
	s32 i;
	SCR_POINT screen;
	s32 point[3];

	state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
	state->args = args;
	btEffectStartCanvasLayer(0);
	btResourceLoadAndDecompress(RESOURCE_SMOKE_SHEET, state->sheet, 1, 1);
	btResourceLoadAndDecompress(RESOURCE_TARGET_BURST_IMAGE,
								state->sheet + TARGET_BURST_PICTURES, 0, 0);
	palette = syResourceGetTableEntry(RESOURCE_PINK_BURST_SHEET);
	syWordsCopy(BG_PLTT, palette, SHEET_PALETTE_BYTES);
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
						COMPOSITE_ADDITIVE);
	drawBurst = HEAP_SLOT_BLOCK(HEAP_SLOT_RAM_CODE);
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
						COMPOSITE_LIGHTEN);
	drawSmoke = HEAP_SLOT_BLOCK(HEAP_SLOT_RAM_CODE_PAIRED);

	for(i = 0; i != ARRAY_COUNT(state->particles); i++) {
		s32 angle;
		s32 speed;

		puff = &state->particles[i];
		angle = syRandom() & TRIG_ANGLE_MASK;
		speed = syRandom() & 0xff;
		SET_POSITION(puff, 0, 0, 0);
		if(i % 6 == 5) {
			puff->velX = 0;
			puff->velY = 0;
		}
		else {
			puff->velX = (sySin(angle) * speed) >> 7;
			puff->velY = (syCos(angle) * speed) >> 9;
		}
		puff->velZ = 0;
		puff->variant = 0;
	}

	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE,
						  CANVAS_FADE_THREE_QUARTERS);

	for(frame = 0; frame != TARGET_BURSTS_FRAMES; frame++) {
		s32 member;
		BATTLE_CAMERA *camera = BattleCameraWork;

		EFFECT_FOR_EACH_TARGET(member, state->args) {
			ACTOR *actor;

			actor = btObjectGetStub(state->args->tgts[member])->actor;
			EFFECT_VIEW_FROM_CAMERA(camera);
			SET_VECTOR(point, actor->x.fixed, toFIXED(20), actor->z.fixed);
			syMatrixTranslateVector(point);
			if(frame >= member * 8 && frame < member * 8 + 40) {
				PARTICLE *puff;

				for(i = 0; i != 6; i++) {
					s32 picture;

					puff = &state->particles[member * 6 + i];
					picture = puff->variant / 6;
					if(picture > 5)
						picture = 5;
					btProjectionPoint(puff, &screen);
					screen.x >>= 1;
					if(i == 5) {
						btParticleAdvanceWithGravity3D(puff, 62,
													   toFIXED(1) / 32);
						drawBurst(
							canvas,
							state->sheet + picture * PICTURE_BYTES(SMOKE_PUFF) +
								TARGET_BURST_PICTURES,
							screen.x - SMOKE_PUFF_WIDTH / 2, screen.y - 36,
							SMOKE_PUFF_WIDTH, SMOKE_PUFF_HEIGHT);
					}
					else {
						btParticleAdvanceWithGravity3D(puff, 60,
													   toFIXED(1) / 128);
						drawSmoke(
							canvas,
							state->sheet + picture * PICTURE_BYTES(SMOKE_PUFF),
							screen.x - SMOKE_PUFF_WIDTH / 2, screen.y - 36,
							SMOKE_PUFF_WIDTH, SMOKE_PUFF_HEIGHT);
					}
					puff->variant++;
				}
			}
		}
		EFFECT_PRESENT_CANVAS(state);
	}
	EFFECT_FINISH_CANVAS();
}

#undef SWIRLING_STARS_FRAMES
#undef SWIRLING_STARS_COUNT
#undef SWIRLING_STARS_PLACED
#undef SWIRLING_STAR_LARGEST
#undef SWIRLING_STAR_DEPTH_NEAR
#undef SWIRLING_STAR_DEPTH_FAR
#undef SWIRLING_STAR_DEPTH_STEP
#undef TARGET_BURST_PICTURES
#undef TARGET_BURSTS_FRAMES

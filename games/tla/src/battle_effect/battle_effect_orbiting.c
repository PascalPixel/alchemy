/* Swirling stars and target bursts, two effects no table names and nothing in
   the game reaches. Part of battle_effect.c. */

#define SWIRLING_STARS_TICKS	 160
#define SWIRLING_STARS_COUNT	 64 /* stars that swirl */
/* stars given a place, the first to swirl */
#define SWIRLING_STARS_PLACED	 256
/* A star is drawn SWIRLING_STAR_LARGEST pixels wide (and twice as tall) at the
   nearest depth, a pixel narrower for every SWIRLING_STAR_DEPTH_STEP farther,
   its depth clamped from SWIRLING_STAR_DEPTH_NEAR to
   SWIRLING_STAR_DEPTH_FAR. */
#define SWIRLING_STAR_LARGEST	 9
#define SWIRLING_STAR_DEPTH_NEAR 250
#define SWIRLING_STAR_DEPTH_FAR	 634
#define SWIRLING_STAR_DEPTH_STEP 64
/* The angle star `i` turns each tick about its axis: each later star
   faster. */
#define SWIRLING_STAR_SPIN(i)	 (((i) + 32) * 8)

/* Unused: nothing calls it. Stars spin about a point above the field, each on
   its own axis, then are drawn back towards it. */
UNUSED void btEffectRunSwirlingStars(BATTLE_EFFECT_PARM *args)
{
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	FIXED				 center[3];
	SCREEN_POINT		 screen;
	DRAW_RECTANGLE		 draw[2];
	PARTICLE			*star;
	Sint32				 i;
	Sint32				 frm;

	state = Ram_HeapSlots->battleEff;
	canvas = Ram_HeapSlots->battleCanvas;
	btEffectStartCanvasLayer(0);
	btResourceLoadAndDecompress(RESOURCE_STAR_DOT_SHEET, state->sheet, 1, 1);
	btEffectBuildCanvasBlitters(args->side ^ 1, draw);
	for(i = 0; i != SWIRLING_STARS_PLACED; i++) {
		star = &PARTICLES[i];
		SET_POSITION(star, toFIXED((syRandom() & 0xff) - 127),
			toFIXED((syRandom() & 0xff) - 127),
			toFIXED((syRandom() & 0xff) - 127));
		SET_VELOCITY(star, 0, 0, 0);
		star->variant = 0;
	}
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
	SET_VECTOR(center, 0, toFIXED(80), 0);
	for(frm = 0; frm != SWIRLING_STARS_TICKS; frm++) {
		BATTLE_CAMERA *camera = Ram_HeapSlots->cameraState;

		EFFECT_VIEW_FROM_CAMERA(camera);
		syMatrixTranslateVector(center);
		for(i = 0; i != SWIRLING_STARS_COUNT; i++) {
			star = &PARTICLES[i];
			if(frm > i / 4 && star->variant == 0) {
				Sint32 size;

				syPushMatrix();
				switch(i & 3) {
				case 0:
					syRotY(frm * SWIRLING_STAR_SPIN(i));
					break;
				case 1:
					syRotX(-frm * SWIRLING_STAR_SPIN(i));
					break;
				case 2:
					syRotZ(-frm * SWIRLING_STAR_SPIN(i));
					break;
				case 3:
					syRotX(-frm * SWIRLING_STAR_SPIN(i));
					syRotZ(-frm * SWIRLING_STAR_SPIN(i));
					break;
				}
				btEffectPosProjectPoint(star, &screen);
				screen.x >>= 1;
				syPopMatrix();
				CLAMP(screen.depth, SWIRLING_STAR_DEPTH_NEAR,
					SWIRLING_STAR_DEPTH_FAR);
				size = SWIRLING_STAR_LARGEST -
					(screen.depth - SWIRLING_STAR_DEPTH_NEAR) /
						SWIRLING_STAR_DEPTH_STEP;
				draw[0](canvas,
					state->sheet +
						((i & 3) * MOTE_SET_BYTES +
							BattleEffectMotePictures[size - 1]),
					screen.x - size / 2, screen.y - size, size, size * 2);
				btParticleAdvanceWithGravity3D(star, 60, 0);
				/* After 30 ticks a star springs back towards the centre. */
				if(frm > i / 4 + 30) {
					Sint32 dx = -star->x >> 8;
					Sint32 dy = -star->y >> 8;
					Sint32 dz = -star->z >> 8;

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

/* The burst's six pictures in the sprite sheet, after the smoke's six. */
#define TARGET_BURST_PICTURES (6 * PICTURE_BYTES(SMOKE_PUFF))

#define TARGET_BURSTS_TICKS 96

/* Unused: nothing calls it. On each target in turn a burst flares while puffs
   of smoke scatter around it. */
UNUSED void btEffectRunTargetBursts(BATTLE_EFFECT_PARM *args)
{
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	void				*palette;
	Sint32				 frm;
	DRAW_RECTANGLE		 drawSmoke;
	DRAW_RECTANGLE		 drawBurst;
	PARTICLE			*puff;
	Sint32				 i;
	SCREEN_POINT		 screen;
	FIXED				 point[3];

	state = Ram_HeapSlots->battleEff;
	canvas = Ram_HeapSlots->battleCanvas;
	btEffectStartCanvasLayer(0);
	btResourceLoadAndDecompress(RESOURCE_SMOKE_SHEET, state->sheet, 1, 1);
	btResourceLoadAndDecompress(RESOURCE_TARGET_BURST_IMAGE,
		state->sheet + TARGET_BURST_PICTURES, 0, 0);
	palette = syResourceGetTableEntry(RESOURCE_PINK_BURST_SHEET);
	syWordsCopyEntry((void *)BG_PLTT, palette, SHEET_PALETTE_BYTES);
	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
	drawBurst = Ram_HeapSlots->ramCode;
	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
	drawSmoke = Ram_HeapSlots->ramCodePaired;

	for(i = 0; i != ARRAY_COUNT(state->particles); i++) {
		Sint32 ang;
		Sint32 speed;

		puff = &state->particles[i];
		ang = syRandom() & TRIG_ANGLE_MASK;
		speed = syRandom() & 0xff;
		SET_POSITION(puff, 0, 0, 0);
		if(i % 6 == 5) {
			puff->velX = 0;
			puff->velY = 0;
		} else {
			puff->velX = (sySin(ang) * speed) >> 7;
			puff->velY = (syCos(ang) * speed) >> 9;
		}
		puff->velZ = 0;
		puff->variant = 0;
	}

	EFFECT_START_TRANSFER(
		state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

	for(frm = 0; frm != TARGET_BURSTS_TICKS; frm++) {
		Sint32		   member;
		BATTLE_CAMERA *camera = Ram_HeapSlots->cameraState;

		EFFECT_FOR_EACH_TARGET(member, args) {
			MOTION_OBJECT *tgt;

			tgt = btObjectGetStub(args->tgts[member])->obj;
			EFFECT_VIEW_FROM_CAMERA(camera);
			SET_VECTOR(point, tgt->x, toFIXED(20), tgt->z);
			syMatrixTranslateVector(point);
			if(frm >= member * 8 && frm < member * 8 + 40) {
				for(i = 0; i != 6; i++) {
					Sint32 picture;

					/* A picture every six ticks, holding the last. */
					puff = &state->particles[member * 6 + i];
					picture = puff->variant / 6;
					if(picture > 5)
						picture = 5;
					btEffectPosProjectPoint(puff, &screen);
					screen.x >>= 1;
					if(i == 5) {
						btParticleAdvanceWithGravity3D(
							puff, 62, toFIXED(1) / 32);
						drawBurst(canvas,
							state->sheet + picture * PICTURE_BYTES(SMOKE_PUFF) +
								TARGET_BURST_PICTURES,
							screen.x - SMOKE_PUFF_WIDTH / 2, screen.y - 36,
							SMOKE_PUFF_WIDTH, SMOKE_PUFF_HEIGHT);
					} else {
						btParticleAdvanceWithGravity3D(
							puff, 60, toFIXED(1) / 128);
						drawSmoke(canvas,
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

#undef SWIRLING_STARS_TICKS
#undef SWIRLING_STARS_COUNT
#undef SWIRLING_STARS_PLACED
#undef SWIRLING_STAR_LARGEST
#undef SWIRLING_STAR_DEPTH_NEAR
#undef SWIRLING_STAR_DEPTH_FAR
#undef SWIRLING_STAR_DEPTH_STEP
#undef SWIRLING_STAR_SPIN
#undef TARGET_BURST_PICTURES
#undef TARGET_BURSTS_TICKS

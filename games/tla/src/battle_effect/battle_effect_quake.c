/* @file */
/* Quake. */
/* Part of battle_effect.c. */

/* The pillars stand side by side from QUAKE_PILLARS_LEFT on the canvas, each */
/* rising from the ground line QUAKE_GROUND_Y and capped with a strip of the */
/* background's ground QUAKE_STRIP_HEIGHT high. */
#define QUAKE_PILLARS	   3
#define QUAKE_PILLAR_WIDTH 40
#define QUAKE_PILLARS_LEFT 8
#define QUAKE_GROUND_Y	   112
#define QUAKE_STRIP_HEIGHT 16
/* The rock picture in the sheet; the sheet starts with the strip of ground. */
#define QUAKE_WALL_PICTURE 7680

/* The byte of pixel (x, y) in the battle background's tiles, laid out 32 to a
 */
/* row in 256 colours. */
#define BACKGROUND_PIXEL_OFFSET(x, y)                                          \
	(((x) & 7) + (((x) / 8) << 6) + (((y) & 7) << 3) + (((y) / 8) << 11))

/* Three rock pillars rise from the left, 4 frames apart, and sink again, */
/* throwing the targets standing on them into the air. */
/* */
/* Abilities: Quake, Earthquake, Quake Sphere. */
void btEffectRunQuake(BATTLE_EFFECT_PARM *args) {
	BATTLE_EFFECT_STATE *battleEffectState;
	void *canvas;
	s32 frame;
	s32 column;
	DRAW_RECTANGLE draw;
	BATTLE_CAMERA *camera;
	s32 xOffset;
	ROCK_WALL_HEIGHTS heights;
	s32 point[3];
	SCR_POINT screen;
	s32 member;
	s32 row;
	s32 col;
	s32 y;

	battleEffectState = Ram_HeapSlots->battleEff;
	canvas = Ram_HeapSlots->battleCanvas;
	camera = Ram_HeapSlots->cameraState;
	/* The canvas at its own width, opaque: the pillars hide what is behind
	 * them. */
	EFFECT_START_NARROW_CANVAS(BGCNT_PRIORITY(1));
	REG_BLDCNT = 0;
	btResourceLoadAndDecompress(RESOURCE_ROCK_WALL_SHEET,
								battleEffectState->sheet + QUAKE_WALL_PICTURE,
								1, 1);

	/* Cut the strip of ground that caps each pillar from the battle background.
	 */
	syWordsCopy(Ram_MapCellBuffer, (void *)BG_CHAR_BLOCK(2), 2 * BG_CHAR_SIZE);
	for(row = 0; row != QUAKE_STRIP_HEIGHT; row++) {
		y = row + QUAKE_GROUND_Y - QUAKE_STRIP_HEIGHT;
		for(col = 0; col != QUAKE_PILLAR_WIDTH; col++) {
			s32 pixel = BACKGROUND_PIXEL_OFFSET(col + 32, y);

			battleEffectState->sheet[row * QUAKE_PILLAR_WIDTH + col] =
				Ram_MapCellBuffer[pixel];
		}
	}

	if(args->side == EFFECT_SIDE_PARTY) {
		REG_BG2X = -(NARROW_CANVAS_PARTY_X << 8);
		xOffset = -NARROW_CANVAS_PARTY_X;
	}
	else {
		xOffset = 0;
	}
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
						BLITTER_TRANSPARENT);
	draw = Ram_HeapSlots->ramCode;
	EFFECT_FOR_EACH_TARGET(member, args)
		battleEffectState->particles[member].variant = 0;
	EFFECT_START_TRANSFER(battleEffectState, CANVAS_COPY_AND_CLEAR, 0);
	heights = RockWall_Heights;
	battleEffectState->shakeFrm = 128;
	sySndPlayStub(SE_RUMBLE);

	for(frame = 0; frame != Quake_Timings[args->variant][QUAKE_TIMING_FRAMES];
		frame++) {
		EFFECT_VIEW_FROM_CAMERA(camera);
		if(frame == Quake_Timings[args->variant][QUAKE_TIMING_FRAMES] - 16)
			EFFECT_SHOW_DAMAGE(SE_IMPACT);

		for(column = 0; column != QUAKE_PILLARS; column++) {
			s32 riseFrame = column * 4 + 16;

			/* DEAD: never true, as the left side is 32 or more and riseFrame at
			 */
			/* most 24; the ROM keeps the test and its store. */
			if((frame & 31) + 32 == riseFrame)
				heights.column[column] += 32;
			if(frame >= riseFrame &&
			   frame <
				   riseFrame +
					   Quake_Timings[args->variant][QUAKE_TIMING_RISE_FRAMES]) {
				s32 height;
				s32 top;

				/* A pillar rises and sinks along a sine, once, twice or three
				 * times by */
				/* variant. */
				height =
					FIXEDtoINT(heights.column[column] *
							   sySin((frame - riseFrame) * (TRIG_TURN / 64)));
				if(height < 0)
					height = -height;
				top = QUAKE_GROUND_Y - height;
				draw(canvas, battleEffectState->sheet + QUAKE_WALL_PICTURE,
					 column * QUAKE_PILLAR_WIDTH + QUAKE_PILLARS_LEFT, top,
					 QUAKE_PILLAR_WIDTH, height);
				draw(canvas, battleEffectState->sheet,
					 column * QUAKE_PILLAR_WIDTH + QUAKE_PILLARS_LEFT,
					 QUAKE_GROUND_Y - QUAKE_STRIP_HEIGHT - height,
					 QUAKE_PILLAR_WIDTH, QUAKE_STRIP_HEIGHT);

				/* Throw up the targets standing on the pillar. */
				EFFECT_FOR_EACH_TARGET(member, args) {
					MOTION_OBJECT *object;

					object = btObjectGetStub(args->tgts[member])->obj;
					SET_VECTOR(point, object->x, object->y, object->z);
					btEffectPosProjectPoint(point, &screen);
					screen.x += xOffset;
					if(screen.x >=
						   column * QUAKE_PILLAR_WIDTH + QUAKE_PILLARS_LEFT &&
					   screen.x <= column * QUAKE_PILLAR_WIDTH +
									   QUAKE_PILLARS_LEFT +
									   QUAKE_PILLAR_WIDTH &&
					   screen.y >= top) {
						object->velY = toFIXED(12);
						object->verticalMotionStrength = BATTLE_UNIT_GRAVITY;
					}
					if(object->y < 0)
						btEffectTintBattler(args->tgts[member],
											SPRITE_COLOR_NORMAL, POSE_DAMAGE,
											TINT_KEEP, 0);
				}
			}
		}

		/* Each target flashes white once, as it lands. */
		EFFECT_FOR_EACH_TARGET(member, args) {
			MOTION_OBJECT *object;
			PARTICLE *state;

			object = btObjectGetStub(args->tgts[member])->obj;
			state = &battleEffectState->particles[member];
			if(state->variant == 0 && object->y <= 0 && object->velY < 0) {
				state->variant = 1;
				btEffectTintBattler(args->tgts[member], SPRITE_COLOR_WHITE,
									POSE_DAMAGE, member, 5);
			}
		}
		btCameraShake(Quake_Timings[args->variant][QUAKE_TIMING_SHAKE],
					  Quake_Timings[args->variant][QUAKE_TIMING_SHAKE]);
		EFFECT_END_FRAME(battleEffectState);
	}

	EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

#undef QUAKE_PILLARS
#undef QUAKE_PILLAR_WIDTH
#undef QUAKE_PILLARS_LEFT
#undef QUAKE_GROUND_Y
#undef QUAKE_STRIP_HEIGHT
#undef QUAKE_WALL_PICTURE
#undef BACKGROUND_PIXEL_OFFSET

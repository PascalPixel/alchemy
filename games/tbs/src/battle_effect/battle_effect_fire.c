/* @file */
/* Fire. */
/* Part of battle_effect.c. */

/* The embers set up in PARTICLES: no row of Fire_Counts may ask for more. */
#define FIRE_EMBERS 64
/* The frames of an impact flare: its six pictures, two frames each. */
#define FIRE_FLARE_FRAMES 12

/* Embers burst from the top of the user and home in on the targets, and each */
/* that lands shows an impact flare as its target flinches. */
/* */
/* Abilities: Fire, Fireball, Inferno. */
void btEffectRunFire(BATTLE_EFFECT_PARM *args) {
	BATTLE_EFFECT_STATE *state;
	void *canvas;
	s32 frame;
	BATTLE_CAMERA *camera;
	u8 *sheet;
	FIXED bodyTop;
	SCR_POINT screen;
	BLITTER_PAIR draw;
	ACTOR *user;
	s32 i;

	state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
	camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
	sheet = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
	state->args = args;
	if(args->side == EFFECT_SIDE_PARTY)
		btEffectStartCanvasLayer(BGCNT_PRIORITY(1));
	else
		btEffectStartCanvasLayer(0);
	btResourceLoadAndDecompress(RESOURCE_EMBER_STREAK_SHEET, state->sheet, 1,
								1);
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sheet, 0, 0);
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
						COMPOSITE_ADDITIVE);
	draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
						COMPOSITE_LIGHTEN);
	draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
	REG_BLDALPHA = EFFECT_BLEND_ADD;

	/* The embers leave the top of the user in a random direction. */
	user = btObjectGetStub(state->args->user)->actor;
	bodyTop = user->y.fixed + btUnitGetEffectHeightStub(state->args->user);
	for(i = 0; i != FIRE_EMBERS; i++) {
		PARTICLE *ember = &PARTICLES[i];
		s32 angle;
		s32 speed;

		angle = syRandom();
		speed = (syRandom() & 127) + 127;
		SET_VELOCITY(ember, (sySin(angle) * speed) >> 6,
					 (s32)INTtoFIXED((syRandom() & 127) - 16) >> 6,
					 (syCos(angle) * speed) >> 6);
		SET_POSITION(ember, user->x.fixed, bodyTop, user->z.fixed);
		ember->variant = -1;
	}

	state->cameraDelta = 0;
	state->cameraPhase = CAMERA_PHASE_IDLE;
	sySetEvent(btCameraApplyPhasedDelta, TASK_PASS_VBLANK);
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE,
						  CANVAS_FADE_THREE_QUARTERS);

	for(frame = 0; frame != (Fire_Counts[state->args->variant] >> 1) + 132;
		frame++) {
		/* The camera turns while the embers fly. */
		if(frame > 16 && frame < 80)
			state->cameraDelta = TRIG_TURN / 256;
		else
			state->cameraDelta = 0;
		if(frame == (Fire_Counts[state->args->variant] >> 1) + 108)
			EFFECT_SHOW_DAMAGE(SE_IMPACT);
		btCameraSetViewStub(0, 0, 0, BATTLE_ZOOM_NORMAL);
		EFFECT_VIEW_FROM_CAMERA(camera);

		/* The embers in flight. */
		for(i = 0; i != Fire_Counts[state->args->variant]; i++) {
			PARTICLE *ember = &PARTICLES[i];
			s32 launchFrame;

			launchFrame = i / 2;
			if(frame > launchFrame && ember->variant == -1) {
				s32 size;

				btProjectionPoint(ember, &screen);
				screen.x >>= 1;
				CLAMP(screen.depth, MOTE_DEPTH_NEAR, MOTE_DEPTH_FAR);
				size = MOTE_SIZE_MAX -
					   (screen.depth - MOTE_DEPTH_NEAR) / MOTE_DEPTH_STEP;
				EFFECT_DRAW_MOTE(draw[frame < launchFrame + 48 ? 1 : 0], canvas,
								 sheet, screen.x, screen.y, size);
				ember->x += ember->velX;
				ember->y += ember->velY;
				ember->z += ember->velZ;
			}

			/* An ember homes in on its target. */
			if(frame > launchFrame + 48 && ember->variant == -1) {
				ACTOR *target;
				s32 vx;
				s32 vy;
				s32 vz;

				target =
					btObjectGetStub(state->args->tgts[i % state->args->tgtCnt])
						->actor;
				vx = ember->velX += (target->x.fixed - ember->x) >> 9;
				vy = ember->velY += (target->y.fixed - ember->y) >> 9;
				vz = ember->velZ += (target->z.fixed - ember->z) >> 9;
				if(frame < launchFrame + 85) {
					SET_VELOCITY(ember, vx * 60 / DAMPING_SCALE,
								 vy * 60 / DAMPING_SCALE,
								 vz * 60 / DAMPING_SCALE);
				}

				/* Below the ground it hits. */
				if(ember->y < 0) {
					ember->variant = 0;
					ember->x = screen.x;
					ember->y = screen.y;
					sySndPlayStub(SE_WHOOSH);
					btEffectTintBattler(
						state->args->tgts[i % state->args->tgtCnt],
						SPRITE_COLOR_RED_TINT, POSE_DAMAGE,
						i % state->args->tgtCnt, 4);
					state->shakeFrm = 2;
				}
			}
		}

		/* The impact flares. */
		for(i = 0; i != Fire_Counts[state->args->variant]; i++) {
			PARTICLE *ember = &PARTICLES[i];

			if(ember->variant >= 0 && ember->variant < FIRE_FLARE_FRAMES) {
				s32 picture;
				u32 width;

				picture = ember->variant / 2;
				/* UB: width is assigned in one argument and read in another. */
#ifdef UBFIX
				width = Fire_FlareWidths[picture];
				draw[1](canvas, state->sheet + Fire_FlarePictures[picture],
						ember->x - (width >> 1),
						ember->y + Fire_FlareBiasY[picture] - 56, width,
						Fire_FlareHeights[picture]);
#else
				draw[1](canvas, state->sheet + Fire_FlarePictures[picture],
						ember->x - ((width = Fire_FlareWidths[picture]) >> 1),
						ember->y + Fire_FlareBiasY[picture] - 56, width,
						Fire_FlareHeights[picture]);
#endif
				ember->variant++;
			}
		}

		if(state->cameraPhase == CAMERA_PHASE_IDLE)
			state->cameraPhase = CAMERA_PHASE_FRAME_DONE;
		btCameraShake(8, 8);
		EFFECT_END_FRAME(state);
	}

	syCloseEvent(btEffectTransferCanvas);
	syCloseEvent(btCameraApplyPhasedDelta);
	EFFECT_RELEASE_BLITTERS();
	btEffectEndCanvasLayer();
}

#undef FIRE_EMBERS
#undef FIRE_FLARE_FRAMES

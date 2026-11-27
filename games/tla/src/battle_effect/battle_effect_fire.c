/* Fire. Part of battle_effect.c. */

/* The embers set up in PARTICLES: no row of FireCounts may ask for more. */
#define FIRE_EMBERS			  64
/* An ember's variant while it flies; from its hit on, it counts the ticks of
   its impact flare. */
#define FIRE_EMBER_IN_FLIGHT  (-1)
/* An ember in flight is drawn at its largest mote (MOTE_PICTURES) at depth
   FIRE_EMBER_DEPTH_NEAR or nearer, one size smaller every FIRE_EMBER_DEPTH_STEP
   farther, and no smaller than at FIRE_EMBER_DEPTH_FAR. */
#define FIRE_EMBER_DEPTH_NEAR 160
#define FIRE_EMBER_DEPTH_STEP 64
#define FIRE_EMBER_DEPTH_FAR  799
/* The ticks of an impact flare: its six pictures, two ticks each. */
#define FIRE_FLARE_TICKS	  12

/* Embers burst from the top of the user and home in on the targets, and each
   that lands shows an impact flare as its target flinches.

   Abilities: Fire, Fireball, Inferno. */
void btEffectRunFire(BATTLE_EFFECT_PARM *args)
{
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	Sint32				 frame;
	BATTLE_CAMERA		*camera;
	Uint8				*sheet;
	FIXED				 bodyTop;
	SCREEN_POINT		 screen;
	DRAW_RECTANGLE		 draw[2];
	MOTION_OBJECT		*user;
	Sint32				 i;

	state = RamHeapSlots->battleEffect;
	canvas = RamHeapSlots->battleCanvas;
	camera = RamHeapSlots->cameraState;
	sheet = RamHeapSlots->battleSheet;
	if(args->side == EFFECT_SIDE_PARTY)
		btEffectStartCanvasLayer(BGCNT_PRIORITY(1));
	else
		btEffectStartCanvasLayer(0);
	btResourceLoadAndDecompress(
		RESOURCE_EMBER_STREAK_SHEET, state->sheet, 1, 1);
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sheet, 0, 0);
	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
	draw[0] = RamHeapSlots->ramCode;
	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
	draw[1] = RamHeapSlots->ramCodePaired;
	REG_BLDALPHA = EFFECT_BLEND_ADD;

	/* The embers leave the top of the user in a random direction. */
	user = btObjectGetStub(args->user)->object;
	bodyTop = user->y + btUnitGetEffectHeightStub(args->user);
	for(i = 0; i != FIRE_EMBERS; i++) {
		PARTICLE *ember = &PARTICLES[i];
		Sint32	  angle;
		Sint32	  speed;

		angle = syRandom();
		speed = (syRandom() & 127) + 127;
		SET_VELOCITY(ember, (sySin(angle) * speed) >> 6,
			(Sint32)toFIXED((syRandom() & 127) - 16) >> 6,
			(syCos(angle) * speed) >> 6);
		SET_POSITION(ember, user->x, bodyTop, user->z);
		ember->variant = FIRE_EMBER_IN_FLIGHT;
	}

	state->cameraDelta = 0;
	state->cameraPhase = CAMERA_PHASE_IDLE;
	sySetEvent(btCameraApplyPhasedDelta, EVENT_PASS_VBLANK);
	EFFECT_START_TRANSFER(
		state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

	for(frame = 0; frame != (FireCounts[args->variant] >> 1) + 132; frame++) {
		/* The camera turns while the embers fly. */
		if(frame > 16 && frame < 80)
			state->cameraDelta = TRIG_TURN / 256;
		else
			state->cameraDelta = 0;
		if(frame == (FireCounts[args->variant] >> 1) + 108)
			EFFECT_SHOW_DAMAGE(SE_IMPACT);
		btCameraSetViewStub(0, 0, 0, BATTLE_ZOOM_NORMAL);
		EFFECT_VIEW_FROM_CAMERA(camera);

		/* The embers in flight, two more each tick. */
		for(i = 0; i != FireCounts[args->variant]; i++) {
			PARTICLE *ember = &PARTICLES[i];
			Sint32	  launchFrame;

			launchFrame = i / 2;
			if(frame > launchFrame && ember->variant == FIRE_EMBER_IN_FLIGHT) {
				Sint32 size;

				btEffectPosProjectPoint(ember, &screen);
				screen.x >>= 1;
				CLAMP(
					screen.depth, FIRE_EMBER_DEPTH_NEAR, FIRE_EMBER_DEPTH_FAR);
				size = MOTE_PICTURES -
					(screen.depth - FIRE_EMBER_DEPTH_NEAR) /
						FIRE_EMBER_DEPTH_STEP;
				EFFECT_DRAW_MOTE(draw[frame < launchFrame + 48 ? 1 : 0], canvas,
					sheet, screen.x, screen.y, size);
				ember->x += ember->velocityX;
				ember->y += ember->velocityY;
				ember->z += ember->velocityZ;
			}

			/* An ember homes in on its target. */
			if(frame > launchFrame + 48 &&
				ember->variant == FIRE_EMBER_IN_FLIGHT) {
				MOTION_OBJECT *target;
				Sint32		   vx;
				Sint32		   vy;
				Sint32		   vz;

				target = btObjectGetStub(args->targets[i % args->targetCount])
							 ->object;
				vx = ember->velocityX += (target->x - ember->x) >> 9;
				vy = ember->velocityY += (target->y - ember->y) >> 9;
				vz = ember->velocityZ += (target->z - ember->z) >> 9;
				if(frame < launchFrame + 85) {
					SET_VELOCITY(ember, vx * 60 / DAMPING_SCALE,
						vy * 60 / DAMPING_SCALE, vz * 60 / DAMPING_SCALE);
				}

				/* Below the ground it hits, and its flare shows where it was on
				   screen. */
				if(ember->y < 0) {
					ember->variant = 0;
					ember->x = screen.x;
					ember->y = screen.y;
					if(args->variant == EFFECT_VARIANT_STRONGEST)
						sySoundPlayStub(SE_HEAVY_IMPACT);
					else
						sySoundPlayStub(SE_WHOOSH);
					btEffectTintBattler(args->targets[i % args->targetCount],
						SPRITE_COLOR_RED_TINT, POSE_DAMAGE,
						i % args->targetCount, 4);
					state->shakeFrame = 2;
				}
			}
		}

		/* The impact flares. */
		for(i = 0; i != FireCounts[args->variant]; i++) {
			PARTICLE *ember = &PARTICLES[i];

			if(ember->variant >= 0 && ember->variant < FIRE_FLARE_TICKS) {
				Sint32 picture;
				Uint32 width;

				picture = ember->variant / 2;
				/* UB: width is assigned in one argument and read in another. */
#if FIX_UNSEQUENCED_SIDE_EFFECT
				width = FireEmberStreakWidths[picture];
				draw[1](canvas, state->sheet + FireEmberStreakPictures[picture],
					ember->x - (width >> 1),
					ember->y + FireEmberStreakBiasY[picture] - 56, width,
					FireEmberStreakHeights[picture]);
#else
				draw[1](canvas, state->sheet + FireEmberStreakPictures[picture],
					ember->x - ((width = FireEmberStreakWidths[picture]) >> 1),
					ember->y + FireEmberStreakBiasY[picture] - 56, width,
					FireEmberStreakHeights[picture]);
#endif
				ember->variant++;
			}
		}

		if(state->cameraPhase == CAMERA_PHASE_IDLE)
			state->cameraPhase = CAMERA_PHASE_TICK_DONE;
		btCameraShake(8, 8);
		EFFECT_END_TICK(state);
	}

	syCloseEvent(btEffectTransferCanvas);
	syCloseEvent(btCameraApplyPhasedDelta);
	EFFECT_RELEASE_BLITTERS();
	btEffectEndCanvasLayer();
}

#undef FIRE_EMBERS
#undef FIRE_EMBER_IN_FLIGHT
#undef FIRE_EMBER_DEPTH_NEAR
#undef FIRE_EMBER_DEPTH_STEP
#undef FIRE_EMBER_DEPTH_FAR
#undef FIRE_FLARE_TICKS

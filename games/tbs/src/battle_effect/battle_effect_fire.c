// Fire. Part of battle_effect.c.

// The embers set up in PARTICLES: no row of FireCounts may ask for more.
#define FIRE_EMBERS      64
// The ticks of an impact flare: its six pictures, two ticks each.
#define FIRE_FLARE_TICKS 12

// Embers burst from the top of the user and home in on the targets, and each that lands shows an
// impact flare as its target flinches.
//
// Abilities: Fire, Fireball, Inferno.
void btEffectRunFire(BATTLE_EFFECT_PARM *args)
{
	BATTLE_EFFECT_STATE *state;
	void                *canvas;
	Sint32               frame;
	BATTLE_CAMERA       *camera;
	Uint8               *sheet;
	FIXED                bodyTop;
	SCREEN_POINT         screen;
	BLITTER_PAIR         draw;
	ACTOR               *user;
	Sint32               i;

	state       = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	canvas      = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
	camera      = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
	sheet       = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
	state->args = args;
	if (args->side == EFFECT_SIDE_PARTY)
		btEffectStartCanvasLayer(BGCNT_PRIORITY(1));
	else
		btEffectStartCanvasLayer(0);
	btResourceLoadAndDecompress(RESOURCE_EMBER_STREAK_SHEET, state->sheet, 1, 1);
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sheet, 0, 0);
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, COMPOSITE_ADDITIVE);
	draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS, COMPOSITE_LIGHTEN);
	draw[1]      = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
	REG_BLDALPHA = EFFECT_BLEND_ADD;

	// The embers leave the top of the user in a random direction.
	user         = btObjectGetStub(state->args->user)->actor;
	bodyTop      = user->y.fixed + btUnitGetEffectHeightStub(state->args->user);
	for (i = 0; i != FIRE_EMBERS; i++)
	{
		PARTICLE *ember = &PARTICLES[i];
		Sint32    angle;
		Sint32    speed;

		angle = syRandom();
		speed = (syRandom() & 127) + 127;
		SET_VELOCITY(ember,
		             (sySin(angle) * speed) >> 6,
		             (Sint32)toFIXED((syRandom() & 127) - 16) >> 6,
		             (syCos(angle) * speed) >> 6);
		SET_POSITION(ember, user->x.fixed, bodyTop, user->z.fixed);
		ember->variant = -1;
	}

	state->cameraDelta = 0;
	state->cameraPhase = CAMERA_PHASE_IDLE;
	sySetEvent(btCameraApplyPhasedDelta, EVENT_PASS_VBLANK);
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

	for (frame = 0; frame != (FireCounts[state->args->variant] >> 1) + 132; frame++)
	{
		// The camera turns while the embers fly.
		if (frame > 16 && frame < 80)
			state->cameraDelta = TRIG_TURN / 256;
		else
			state->cameraDelta = 0;
		if (frame == (FireCounts[state->args->variant] >> 1) + 108)
			EFFECT_SHOW_DAMAGE(SE_IMPACT);
		btCameraSetViewStub(0, 0, 0, BATTLE_ZOOM_NORMAL);
		EFFECT_VIEW_FROM_CAMERA(camera);

		// The embers in flight.
		for (i = 0; i != FireCounts[state->args->variant]; i++)
		{
			PARTICLE *ember = &PARTICLES[i];
			Sint32    launchFrame;

			launchFrame = i / 2;
			if (frame > launchFrame && ember->variant == -1)
			{
				Sint32 size;

				btProjectionPoint(ember, &screen);
				screen.x >>= 1;
				CLAMP(screen.depth, MOTE_DEPTH_NEAR, MOTE_DEPTH_FAR);
				size = MOTE_SIZE_MAX - (screen.depth - MOTE_DEPTH_NEAR) / MOTE_DEPTH_STEP;
				EFFECT_DRAW_MOTE(draw[frame < launchFrame + 48 ? 1 : 0],
				                 canvas,
				                 sheet,
				                 screen.x,
				                 screen.y,
				                 size);
				ember->x += ember->velocityX;
				ember->y += ember->velocityY;
				ember->z += ember->velocityZ;
			}

			// An ember homes in on its target.
			if (frame > launchFrame + 48 && ember->variant == -1)
			{
				ACTOR *target;
				Sint32 vx;
				Sint32 vy;
				Sint32 vz;

				target = btObjectGetStub(state->args->targets[i % state->args->targetCount])->actor;
				vx = ember->velocityX += (target->x.fixed - ember->x) >> 9;
				vy = ember->velocityY += (target->y.fixed - ember->y) >> 9;
				vz = ember->velocityZ += (target->z.fixed - ember->z) >> 9;
				if (frame < launchFrame + 85)
				{
					SET_VELOCITY(ember,
					             vx * 60 / DAMPING_SCALE,
					             vy * 60 / DAMPING_SCALE,
					             vz * 60 / DAMPING_SCALE);
				}

				// Below the ground it hits.
				if (ember->y < 0)
				{
					ember->variant = 0;
					ember->x       = screen.x;
					ember->y       = screen.y;
					sySoundPlayStub(SE_WHOOSH);
					btEffectTintBattler(state->args->targets[i % state->args->targetCount],
					                    SPRITE_COLOR_RED_TINT,
					                    POSE_DAMAGE,
					                    i % state->args->targetCount,
					                    4);
					state->shakeFrame = 2;
				}
			}
		}

		// The impact flares.
		for (i = 0; i != FireCounts[state->args->variant]; i++)
		{
			PARTICLE *ember = &PARTICLES[i];

			if (ember->variant >= 0 && ember->variant < FIRE_FLARE_TICKS)
			{
				Sint32 picture;
				Uint32 width;

				picture = ember->variant / 2;
				// UB: width is assigned in one argument and read in another.
#if FIX_UNSEQUENCED_SIDE_EFFECT
				width = FireFlareWidths[picture];
				draw[1](canvas,
				        state->sheet + FireFlarePictures[picture],
				        ember->x - (width >> 1),
				        ember->y + FireFlareBiasY[picture] - 56,
				        width,
				        FireFlareHeights[picture]);
#else
				draw[1](canvas,
				        state->sheet + FireFlarePictures[picture],
				        ember->x - ((width = FireFlareWidths[picture]) >> 1),
				        ember->y + FireFlareBiasY[picture] - 56,
				        width,
				        FireFlareHeights[picture]);
#endif
				ember->variant++;
			}
		}

		if (state->cameraPhase == CAMERA_PHASE_IDLE)
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
#undef FIRE_FLARE_TICKS

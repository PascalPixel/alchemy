/* Streak volleys (Sonic Wave, Shriek, War Cry ...) and mote fountains (Sleep
   Star, Soothing Star). Part of battle_effect.c. */

/* Column `n` of style `mode`'s row of StreakStyles, four flags a style. */
#define STREAK_VALUE(mode, n) StreakStyles[(mode)][(n)]
#define STREAK_ANIMATE		  0 /* nonzero: the streak cycles through three images */
#define STREAK_HIT			  1 /* nonzero: play the hit sound and flash the target */
/* The target's knockback on the hit (BATTLE_KNOCKBACK), or -1 for none. */
#define STREAK_MOTION		  2
/* Nonzero: the struck target trembles from side to side. */
#define STREAK_SHAKE		  3

#define STREAK_MAX 3 /* streaks fired at each target */

/* The streak volley's styles: its colour and its row of StreakStyles. */
typedef enum STREAK_STYLE {
	STREAK_STYLE_EMBER,	 /* Sonic Wave */
	STREAK_STYLE_LIME,	 /* Shriek, and the unused btEffectRunLimeStreaks */
	STREAK_STYLE_VIOLET, /* Banshee Howl */
	STREAK_STYLE_RED,	 /* Wicked Howl */
	STREAK_STYLE_CYAN	 /* War Cry */
} STREAK_STYLE;

/* Three streaks of ember light fly from the user at each target in turn; each
   target flashes and recoils.

   Abilities: Sonic Wave. */
void btEffectRunSonicWave(BATTLE_EFFECT_PARM *args)
{
	btEffectRunStreakVolley(args, STREAK_STYLE_EMBER);
}

/* Three streaks of lime light fly from the user at each target in turn; each
   target flashes and recoils.

   Abilities: Shriek. */
void btEffectRunShriek(BATTLE_EFFECT_PARM *args)
{
	btEffectRunStreakVolley(args, STREAK_STYLE_LIME);
}

/* Three streaks of violet light fly from the user at each target in turn; each
   target flashes and trembles.

   Abilities: Banshee Howl. */
void btEffectRunBansheeHowl(BATTLE_EFFECT_PARM *args)
{
	btEffectRunStreakVolley(args, STREAK_STYLE_VIOLET);
}

/* Three flickering cyan streaks, cycling through three colour rows, fly from
   the user at each target in turn; each target flashes and recoils.

   Abilities: War Cry. */
void btEffectRunWarCry(BATTLE_EFFECT_PARM *args)
{
	btEffectRunStreakVolley(args, STREAK_STYLE_CYAN);
}

/* Three flickering red streaks, cycling through three colour rows, fly from the
   user at each target in turn; each target flashes and trembles.

   Abilities: Wicked Howl. */
void btEffectRunWickedHowl(BATTLE_EFFECT_PARM *args)
{
	btEffectRunStreakVolley(args, STREAK_STYLE_RED);
}

/* Unused: no entry of the effect table points here. It would have played the
   same lime streaks as Shriek. */
UNUSED void btEffectRunLimeStreaks(BATTLE_EFFECT_PARM *args)
{
	btEffectRunStreakVolley(args, STREAK_STYLE_LIME);
}

/* FlameArcSheet's streaks: three animation frames, each six pictures that grow;
   the lower half is the upper drawn flipped. */
#define STREAK_WIDTH		20
#define STREAK_HEIGHT		40
#define STREAK_FRAME_BYTES	(6 * PICTURE_BYTES(STREAK))
#define STREAK_SPACING		6  /* ticks between streaks */
#define STREAK_FLIGHT		24 /* ticks from user to target */
#define STREAK_TARGET_DELAY 32 /* ticks between targets */

/* Fires the streaks at each target in turn from above the user; each leaves a
   fading trail. */
void btEffectRunStreakVolley(BATTLE_EFFECT_PARM *args, Sint32 mode)
{
	SCREEN_POINT		 pos;
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	Sint32				 k;
	DRAW_RECTANGLE		 drawLower;
	DRAW_RECTANGLE		 drawUpper;
	BATTLE_CAMERA		*camera;
	ACTOR				*src;
	ACTOR				*target;
	Sint32				 resource;
	Sint32				 frame;
	Sint32				 streakIndex;
	Sint32				 streaks = STREAK_MAX;
	FIXED lift = toFIXED(20); /* the streaks fly 20 pixels above the ground */

	state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
	camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
	state->args = args;
	btEffectStartCanvasLayer(BGCNT_PRIORITY(1));

	if(state->args->side == EFFECT_SIDE_ENEMIES) {
		EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
			EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y, COMPOSITE_LIGHTEN);
		drawUpper = HEAP_SLOT_BLOCK(HEAP_SLOT_RAM_CODE);
		EFFECT_LOAD_BLITTER(
			HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS, COMPOSITE_LIGHTEN);
		drawLower = HEAP_SLOT_BLOCK(HEAP_SLOT_RAM_CODE_PAIRED);
	} else {
		EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
			EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP, COMPOSITE_LIGHTEN);
		drawUpper = HEAP_SLOT_BLOCK(HEAP_SLOT_RAM_CODE);
		EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
			EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_LIGHTEN);
		drawLower = HEAP_SLOT_BLOCK(HEAP_SLOT_RAM_CODE_PAIRED);
	}

	btResourceLoadAndDecompress(RESOURCE_FLAME_ARC_SHEET, state->sheet, 0, 0);
	switch(mode) {
	case STREAK_STYLE_EMBER:
		resource = RESOURCE_EMBER_STREAK_SHEET;
		break;
	case STREAK_STYLE_LIME:
		resource = RESOURCE_LIME_PALETTE;
		break;
	case STREAK_STYLE_VIOLET:
		resource = RESOURCE_JUPITER_STAR_FIELD_PALETTE;
		break;
	case STREAK_STYLE_RED:
		resource = RESOURCE_VOLCANO_RED_PALETTE;
		break;
	case STREAK_STYLE_CYAN:
	default:
		resource = RESOURCE_CYAN_PALETTE;
		break;
	}
	syWordsCopy(
		BG_PLTT, syResourceGetTableEntry(resource), SHEET_PALETTE_BYTES);
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

	src = btObjectGetStub(state->args->user)->actor;
	{
		/* The loop counts 6 ticks for each streak though targets are 32 ticks
		   apart, so with three or four targets it stops before the last one's
		   second flash. */
		Sint32 totalTicks =
			streaks * state->args->targetCount * STREAK_SPACING + 48;

		EFFECT_FOR_EACH_TARGET(k, state->args) {
			target = btObjectGetStub(state->args->targets[k])->actor;
			for(streakIndex = 0; streakIndex != streaks; streakIndex++) {
				PARTICLE *streak = &state->particles[k * streaks + streakIndex];

				SET_POSITION(
					streak, src->x.fixed, src->y.fixed + lift, src->z.fixed);
				SET_VELOCITY(streak,
					(target->x.fixed - streak->x) / STREAK_FLIGHT,
					(target->y.fixed + lift - streak->y) / STREAK_FLIGHT,
					(target->z.fixed - streak->z) / STREAK_FLIGHT);
				streak->variant = 0; /* ticks since the streak set off */
			}
		}

		for(frame = 0; frame != totalTicks; frame++) {
			Sint32 *row = state->bg2xLines;

			/* A sine wave of BG2X offsets for the wave HBlank DMA, which
			   nothing arms here, so the canvas does not ripple. */
			for(k = 0; k != DISPLAY_HEIGHT; k++)
				*row++ = CANVAS_WAVE_BG2X(
					toFIXED(8) - sySin((frame + k) * (TRIG_TURN / 16)) * 2);
			if(frame > totalTicks - 16)
				REG_BLDALPHA = EFFECT_BLEND_CANVAS(totalTicks - frame);
			EFFECT_VIEW_FROM_CAMERA(camera);
			EFFECT_FOR_EACH_TARGET(k, state->args) {
				if(frame >= k * STREAK_TARGET_DELAY) {
					for(streakIndex = 0; streakIndex != streaks;
						streakIndex++) {
						if(frame >= k * STREAK_TARGET_DELAY +
								streakIndex * STREAK_SPACING) {
							PARTICLE *streak =
								&state->particles[k * streaks + streakIndex];
							Sint32 step;

							btProjectionPoint(streak, &pos);
							pos.x >>= 1;
							step = streak->variant / 8;
							if(step > 5)
								step = 5;
							if(STREAK_VALUE(mode, STREAK_ANIMATE)) {
								Uint8 *picture =
									&state->sheet[step * PICTURE_BYTES(STREAK) +
										frame / 2 % 3 * STREAK_FRAME_BYTES];

								drawUpper(canvas, picture,
									pos.x - STREAK_WIDTH / 2,
									pos.y - STREAK_HEIGHT, STREAK_WIDTH,
									STREAK_HEIGHT);
								drawLower(canvas, picture,
									pos.x - STREAK_WIDTH / 2, pos.y,
									STREAK_WIDTH, STREAK_HEIGHT);
							} else {
								Uint8 *picture =
									&state->sheet[step * PICTURE_BYTES(STREAK) +
										2 * STREAK_FRAME_BYTES];

								drawUpper(canvas, picture,
									pos.x - STREAK_WIDTH / 2,
									pos.y - STREAK_HEIGHT, STREAK_WIDTH,
									STREAK_HEIGHT);
								drawLower(canvas, picture,
									pos.x - STREAK_WIDTH / 2, pos.y,
									STREAK_WIDTH, STREAK_HEIGHT);
							}
							btParticleAdvanceWithGravity3D(
								streak, PARTICLE_DAMPING_NONE, 0);
							streak->variant++;
						}
					}

					/* The target trembles. */
					if(STREAK_VALUE(mode, STREAK_SHAKE) &&
						frame >= k * STREAK_TARGET_DELAY + 30 &&
						frame < k * STREAK_TARGET_DELAY + 62) {
						ACTOR *struck =
							btObjectGetStub(state->args->targets[k])->actor;

						struck->x.fixed += toFIXED(StreakTremble
								[(frame - k * STREAK_TARGET_DELAY - 30) % 8]);
						if(struck->x.fixed > 0)
							struck->x.fixed += toFIXED(1) / 2;
						else
							struck->x.fixed -= toFIXED(1) / 2;
						btEffectTintBattler(state->args->targets[k], TINT_KEEP,
							POSE_DAMAGE, TINT_KEEP, 0);
					}

					/* The hit: the target flashes, and again 16 ticks
					   later. */
					if(STREAK_VALUE(mode, STREAK_HIT)) {
						if(frame == k * STREAK_TARGET_DELAY + 24) {
							sySoundPlayStub(SE_IMPACT);
							/* the first hit lets the battle show the damage */
							if(k == 0)
								EFFECT_SHOW_DAMAGE(-1);
							btEffectTintBattler(state->args->targets[k],
								SPRITE_COLOR_WHITE, POSE_DAMAGE, k, 8);
						}
						if(frame == k * STREAK_TARGET_DELAY + 40)
							btEffectTintBattler(state->args->targets[k],
								SPRITE_COLOR_WHITE, POSE_DAMAGE, k, 8);
					}
					if(STREAK_VALUE(mode, STREAK_MOTION) != -1 &&
						frame == k * STREAK_TARGET_DELAY + 24) {
						state->shakeFrame = 4;
						btMotionApplyKnockbackStub(state->args->targets[k],
							STREAK_VALUE(mode, STREAK_MOTION));
					}
				}
			}
			btCameraShake(8, 8);
			EFFECT_END_TICK(state);
		}
	}

	EFFECT_FINISH_CANVAS();
}

/* The mote fountain's modes (btEffectRunMoteFountain). */
typedef enum MOTE_FOUNTAIN_MODE {
	MOTE_FOUNTAIN_HEALING, /* Soothing Star */
	MOTE_FOUNTAIN_DRIFTING /* Sleep Star */
} MOTE_FOUNTAIN_MODE;

/* The motes the fountains set out, and the ones drawn. */
enum { MOTE_FOUNTAIN_MOTES = 256, MOTE_FOUNTAIN_MOTES_DRAWN = 128 };

/* Motes burst out of the user and float up, wobbling, as they drift across the
   field onto the targets, and each target flinches in turn.

   Abilities: Sleep Star. */
void btEffectRunSleepStar(BATTLE_EFFECT_PARM *args)
{
	btEffectRunMoteFountain(args, MOTE_FOUNTAIN_DRIFTING);
}

/* Orange healing motes burst out of the user and float up close to it, and each
   target glows in turn with the recovery sound.

   Abilities: Soothing Star. */
void btEffectRunSoothingStar(BATTLE_EFFECT_PARM *args)
{
	btEffectRunMoteFountain(args, MOTE_FOUNTAIN_HEALING);
}

#define MOTE_FOUNTAIN_TICKS 128

/* The projected depths over which a mote is sized, nearest to farthest: it is
   drawn MOTE_FOUNTAIN_LARGEST pixels wide (and twice as tall) at
   MOTE_FOUNTAIN_DEPTH_NEAR or nearer, a pixel narrower for every
   MOTE_FOUNTAIN_DEPTH_STEP farther. */
#define MOTE_FOUNTAIN_DEPTH_NEAR 314
#define MOTE_FOUNTAIN_DEPTH_FAR	 634
#define MOTE_FOUNTAIN_LARGEST	 6
#define MOTE_FOUNTAIN_DEPTH_STEP 64

/* Motes burst out of the user and float upwards, wobbling, and each target
   reacts in turn. */
void btEffectRunMoteFountain(BATTLE_EFFECT_PARM *args, Sint32 mode)
{
	Sint32				 worldPoint[3];
	SCREEN_POINT		 pos;
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	BLITTER_PAIR		 draw;
	BATTLE_CAMERA		*camera;
	Uint8				*motes;
	ACTOR				*src;
	PARTICLE			*mote;
	Sint32				 resource;
	Sint32				 frame;
	Sint32				 i;

	state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
	camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
	motes = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
	state->args = args;
	if(mode == MOTE_FOUNTAIN_HEALING)
		btEffectStartCanvasLayer(0);
	else
		btEffectStartCanvasLayer(BGCNT_PRIORITY(1));
	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, COMPOSITE_LIGHTEN);
	draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
		EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y, COMPOSITE_LIGHTEN);
	draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, motes, 0, 0);
	if(mode == MOTE_FOUNTAIN_HEALING)
		resource = RESOURCE_GAIA_ORANGE_PALETTE;
	else
		resource = RESOURCE_EARTH_WALL_SHEET;
	syWordsCopy(
		BG_PLTT, syResourceGetTableEntry(resource), SHEET_PALETTE_BYTES);
	EFFECT_START_TRANSFER(
		state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

	src = btObjectGetStub(state->args->user)->actor;
	for(i = 0; i != MOTE_FOUNTAIN_MOTES; i++) {
		Sint32 speed;
		Sint32 angle;

		mote = &PARTICLES[i];

		speed = (syRandom() & 0x3ff) + 32;
		angle = syRandom() & TRIG_ANGLE_MASK;
		SET_POSITION(
			mote, src->x.fixed, src->y.fixed + toFIXED(5), src->z.fixed);
		SET_VELOCITY(mote, sySin(angle) * speed >> 8,
			((syRandom() & 255) - 32) << 9, -(syCos(angle) * speed << 1) >> 8);
		mote->variant = (syRandom() & 31) + 48;
		if(mode == MOTE_FOUNTAIN_HEALING) {
			mote->velocityX /= 2;
			mote->velocityZ /= 2;
		}
	}

	for(frame = 0; frame != MOTE_FOUNTAIN_TICKS; frame++) {
		EFFECT_VIEW_FROM_CAMERA(camera);
		for(i = 0; i != MOTE_FOUNTAIN_MOTES_DRAWN; i++) {
			mote = &PARTICLES[i];
			if(frame >= i / 32 * 8 && mote->variant >= 0) {
				Sint32 size;

				/* Motes wobble and grow as they near the camera. */
				SET_VECTOR(worldPoint,
					mote->x +
						sySin((i * 4 + mote->variant) * (TRIG_TURN / 64)) * 16,
					mote->y, mote->z);
				btProjectionPoint(worldPoint, &pos);
				pos.x >>= 1;
				if(pos.depth < MOTE_FOUNTAIN_DEPTH_NEAR)
					pos.depth = MOTE_FOUNTAIN_DEPTH_NEAR;
				if(pos.depth > MOTE_FOUNTAIN_DEPTH_FAR)
					pos.depth = MOTE_FOUNTAIN_DEPTH_FAR;
				size = MOTE_FOUNTAIN_LARGEST -
					(pos.depth - MOTE_FOUNTAIN_DEPTH_NEAR) /
						MOTE_FOUNTAIN_DEPTH_STEP;
				EFFECT_DRAW_MOTE(draw[0], canvas, motes, pos.x, pos.y, size);
				btParticleAdvanceWithGravity3D(mote, 62, toFIXED(1) / 64);
				/* The drifting mode pushes the motes across the field. */
				if(mode == MOTE_FOUNTAIN_DRIFTING) {
					if(src->x.fixed < 0)
						mote->velocityX += toFIXED(1) / 8;
					else
						mote->velocityX -= toFIXED(1) / 8;
				}
				mote->variant--;
			}
		}
		if(mode == MOTE_FOUNTAIN_DRIFTING) {
			EFFECT_FOR_EACH_TARGET(i, state->args) {
				if(frame == i * 8 + 48) {
					EFFECT_SHOW_DAMAGE(-1);
					btEffectTintBattler(state->args->targets[i],
						SPRITE_COLOR_WHITE, POSE_DAMAGE, i, 8);
				}
			}
		} else {
			EFFECT_FOR_EACH_TARGET(i, state->args) {
				if(frame == i * 8 + 48) {
					sySoundPlayStub(SE_RECOVERY);
					EFFECT_SHOW_DAMAGE(-1);
					btEffectTintBattler(state->args->targets[i],
						SPRITE_COLOR_WHITE, TINT_KEEP, i, 8);
				}
			}
		}
		EFFECT_END_TICK(state);
	}

	EFFECT_FINISH_CANVAS();
}

#undef STREAK_VALUE
#undef STREAK_ANIMATE
#undef STREAK_HIT
#undef STREAK_MOTION
#undef STREAK_SHAKE
#undef STREAK_MAX
#undef STREAK_SPACING
#undef STREAK_FLIGHT
#undef STREAK_TARGET_DELAY
#undef MOTE_FOUNTAIN_TICKS
#undef MOTE_FOUNTAIN_DEPTH_NEAR
#undef MOTE_FOUNTAIN_DEPTH_FAR
#undef MOTE_FOUNTAIN_LARGEST
#undef MOTE_FOUNTAIN_DEPTH_STEP

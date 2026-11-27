/* An unused Mercury Djinni heal, and the camera turn Fire and Froth share. Part
   of battle_effect.c. */

#define DJINN_HEAL_SPARKS			 512 /* sparks in PARTICLES, 64 for each target */
#define DJINN_HEAL_SPARKS_PER_TARGET 64

#define DJINN_HEAL_SWIRL_TICKS		  98
#define DJINN_HEAL_SWIRL_MOTES		  32 /* motes swirled in */
/* The motes set out to swirl, of which DJINN_HEAL_SWIRL_MOTES are drawn. */
#define DJINN_HEAL_SWIRL_SLOTS		  64
/* A swirling mote is drawn at size DJINN_HEAL_MOTE_SIZE_MAX at depth
   DJINN_HEAL_MOTE_DEPTH_NEAR or nearer, one size smaller every
   DJINN_HEAL_MOTE_DEPTH_STEP farther, its depth held below
   DJINN_HEAL_MOTE_DEPTH_FAR. */
#define DJINN_HEAL_MOTE_SIZE_MAX	  9
#define DJINN_HEAL_MOTE_DEPTH_NEAR	  250
#define DJINN_HEAL_MOTE_DEPTH_STEP	  64
#define DJINN_HEAL_MOTE_DEPTH_FAR	  634
/* The depths the sparkles' idle size by depth is worked out over, as the
   healing sparkles' are: three sizes, DJINN_HEAL_SPARKLE_DEPTH_STEP apart. */
#define DJINN_HEAL_SPARKLE_DEPTH_NEAR 314
#define DJINN_HEAL_SPARKLE_DEPTH_STEP 128
#define DJINN_HEAL_SPARKLE_DEPTH_FAR  634
/* The angle mote `i` turns each tick about its axis: each later mote
   faster. */
#define DJINN_HEAL_MOTE_SPIN(i)		  ((i) * 32 + 256)

/* A small Mercury Djinni heals the party: motes swirl up from the user, the
   Djinni appears where they gather, and sparkles spiral in on each target in
   turn. Nothing reaches it. */
UNUSED void btEffectRunDjinnHeal(BATTLE_EFFECT_PARM *args)
{
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	Sint32				 member;
	Uint8				*sheet;
	Sint32				 frame;
	Sint32				 i;
	Sint32				 pos[3];
	FIXED				 target[3];
	Sint32				 delta[3];
	Sint32				 djinniPoint[3];
	SCREEN_POINT		 screen;
	Sint32				 point[3];
	SCREEN_POINT		 view;
	BLITTER_PAIR		 draw;

	state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
	sheet = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
	state->args = args;
	btEffectStartCanvasLayer(0);
	btResourceLoadAndDecompress(
		RESOURCE_MERCURY_DJINN_SMALL_SHEET, state->sheet, 1, 1);
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sheet, 0, 0);
	btEffectBuildCanvasBlitters(state->args->side ^ 1, draw);
	{
		ACTOR *actor = btObjectGetStub(state->args->user)->actor;
		ACTOR *goal = btObjectGetStub(state->args->targets[0])->actor;

		for(i = 0; i != DJINN_HEAL_SWIRL_SLOTS; i++) {
			PARTICLE *spark = &PARTICLES[i];
			Sint32	  heading = syRandom() & TRIG_ANGLE_MASK;
			Sint32	  speed = (syRandom() & 0xff) + 128;

			SET_POSITION(spark, 0, INTtoFIXED((syRandom() & 31) + 20), 0);
			SET_VELOCITY(spark, (sySin(heading) * speed) >> 5, 0,
				(syCos(heading) * speed) >> 5);
			spark->variant = 0;
		}
		EFFECT_START_TRANSFER(
			state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

		/* The swirl. */
		SET_VECTOR(pos, actor->x.fixed, 0, actor->z.fixed);
		SET_VECTOR(target, goal->x.fixed, toFIXED(90), 0);
	}
	SET_VECTOR(delta, (target[0] - pos[0]) / 40, (target[1] - pos[1]) / 40,
		(target[2] - pos[2]) / 40);
	for(frame = 0; frame != DJINN_HEAL_SWIRL_TICKS; frame++) {
		BATTLE_CAMERA *camera = BattleCameraWork;
		if(frame == 8)
			sySoundPlayStub(SE_POWER_UP);
		if(frame == 80)
			sySoundPlayStub(SE_SWIRL);
		EFFECT_VIEW_FROM_CAMERA(camera);
		if(frame >= 30 && frame < 70) {
			pos[0] += delta[0];
			pos[1] += delta[1];
			pos[2] += delta[2];
		}
		syMatrixTranslateVector(pos);
		if(frame == 0)
			btEffectTintBattler(
				state->args->user, SPRITE_COLOR_WHITE, TINT_KEEP, TINT_KEEP, 0);
		if(frame == 24)
			btEffectTintBattler(state->args->user, SPRITE_COLOR_NORMAL,
				TINT_KEEP, TINT_KEEP, 0);
		for(i = 0; i != DJINN_HEAL_SWIRL_MOTES; i++) {
			PARTICLE *spark = &PARTICLES[i];
			if(frame > i && spark->variant == 0) {
				Sint32 size;
				syPushMatrix();
				switch(i & 3) {
				case SPIN_YAW:
					syRotY(frame * DJINN_HEAL_MOTE_SPIN(i));
					break;
				case SPIN_PITCH:
					syRotX(-frame * DJINN_HEAL_MOTE_SPIN(i));
					break;
				case SPIN_ROLL:
					syRotZ(-frame * DJINN_HEAL_MOTE_SPIN(i));
					break;
				case SPIN_PITCH_AND_ROLL:
					syRotX(-frame * DJINN_HEAL_MOTE_SPIN(i));
					syRotZ(-frame * DJINN_HEAL_MOTE_SPIN(i));
					break;
				}
				btProjectionPoint(spark, &screen);
				screen.x >>= 1;
				syPopMatrix();
				CLAMP(screen.depth, DJINN_HEAL_MOTE_DEPTH_NEAR,
					DJINN_HEAL_MOTE_DEPTH_FAR);
				size = DJINN_HEAL_MOTE_SIZE_MAX -
					(screen.depth - DJINN_HEAL_MOTE_DEPTH_NEAR) /
						DJINN_HEAL_MOTE_DEPTH_STEP;
				EFFECT_DRAW_MOTE(
					draw[0], canvas, sheet, screen.x, screen.y, size);
				btParticleAdvanceWithGravity3D(spark, 60, 0);
				if(frame > i + 30) {
					Sint32 dx = -spark->x >> 8;
					Sint32 dy = -spark->y >> 8;
					Sint32 dz = -spark->z >> 8;

					spark->velocityX += dx;
					spark->velocityY += dy;
					spark->velocityZ += dz;
				}
			}
		}
		/* The Djinni appears and bobs. */
		if(frame > 82) {
			SET_VECTOR(
				djinniPoint, 0, (sySin(frame * (TRIG_TURN / 64)) * 4), 0);
			btProjectionPoint(djinniPoint, &screen);
			screen.x >>= 1;
			draw[1](canvas, state->sheet, screen.x - SMALL_DJINNI_WIDTH / 2,
				screen.y - MERCURY_SMALL_DJINNI_HEIGHT / 2, SMALL_DJINNI_WIDTH,
				MERCURY_SMALL_DJINNI_HEIGHT);
		}
		EFFECT_PRESENT_CANVAS(state);
	}
	/* The healing. */
	for(i = 0; i != DJINN_HEAL_SPARKS; i++) {
		PARTICLE *spark = &PARTICLES[i];
		SET_POSITION(spark, ((syRandom() & 0xff) - 127) << 15,
			((syRandom() & 0x7f) + 64) << 15,
			((syRandom() & 0xff) - 127) << 15);
		spark->variant = 0;
	}
	btResourceLoadAndDecompress(RESOURCE_SPARKLE_DOTS, sheet, 0, 0);
	for(frame = 0; frame != state->args->targetCount * 8 + 72; frame++) {
		BATTLE_CAMERA *camera = BattleCameraWork;
		EFFECT_VIEW_FROM_CAMERA(camera);
		/* The Djinni sways, then flies off. */
		if(frame >= state->args->targetCount * 8 + 40)
			target[1] += toFIXED(4);
		SET_VECTOR(point, target[0], target[1],
			target[2] + sySin(frame * (TRIG_TURN / 32)) * 40);
		btProjectionPoint(point, &view);
		view.x >>= 1;
		draw[0](canvas, state->sheet, view.x - SMALL_DJINNI_WIDTH / 2,
			view.y - MERCURY_SMALL_DJINNI_HEIGHT / 2, SMALL_DJINNI_WIDTH,
			MERCURY_SMALL_DJINNI_HEIGHT);
		EFFECT_FOR_EACH_TARGET(member, state->args) {
			ACTOR *object =
				btObjectGetStub(state->args->targets[member])->actor;
			Sint32 start = member * 8;
			EFFECT_VIEW_FROM_CAMERA(camera);
			SET_VECTOR(pos, object->x.fixed, toFIXED(40), object->z.fixed);
			syMatrixTranslateVector(pos);
			/* Each target glows with the recovery sound. */
			if(frame == start + 30)
				sySoundPlayStub(SE_RECOVERY);
			if(frame == start + 40)
				btEffectTintBattler(state->args->targets[member],
					SPRITE_COLOR_WHITE, TINT_KEEP, TINT_KEEP, 0);
			if(frame == start + 64)
				btEffectTintBattler(state->args->targets[member],
					SPRITE_COLOR_NORMAL, TINT_KEEP, TINT_KEEP, 0);
			if(frame > start) {
				syRotY((frame - start) << 9);
				for(i = 0; i != DJINN_HEAL_SPARKS_PER_TARGET; i++) {
					PARTICLE *spark =
						&PARTICLES[member * DJINN_HEAL_SPARKS_PER_TARGET + i];

					if(frame > start + i / 2) {
						Sint32 xx = (spark->x >> 8) * (spark->x >> 8);
						Sint32 yy = (spark->y >> 8) * (spark->y >> 8);
						Sint32 zz = (spark->z >> 8) * (spark->z >> 8);
						Sint32 distance = sySquartIWRAM(xx + yy + zz) >> 9;
						if(distance != 0) {
							Sint32 pictureIndex;

							btProjectionPoint(spark, &view);
							view.x >>= 1;
							/* LEFTOVER(template): the clamp and size by depth,
							 * idle here, as btEffectRunHealingMotes
							 * (battle_effect_healing.c) clamps the depth and
							 * sizes its motes by it in every mode but Ply,
							 * which overwrites the size as this does; as both
							 * lines removed. */
							CLAMP(view.depth, DJINN_HEAL_SPARKLE_DEPTH_NEAR,
								DJINN_HEAL_SPARKLE_DEPTH_FAR);
							pictureIndex = 3 -
								(view.depth - DJINN_HEAL_SPARKLE_DEPTH_NEAR) /
									DJINN_HEAL_SPARKLE_DEPTH_STEP;
							pictureIndex = (i * 4 + frame) % 9;
							{
								Uint8 *picture = sheet +
									BattleEffectSparklePictures[pictureIndex];
								Uint32 edge =
									BattleEffectSparkleSizes[pictureIndex];
								Uint32 half = edge >> 1;

								draw[1](canvas, picture, view.x - half,
									view.y - half, edge, edge);
							}
							spark->x -= spark->x / distance;
							spark->y -= spark->y / distance;
							spark->z -= spark->z / distance;
						}
					}
				}
			}
		}
		EFFECT_PRESENT_CANVAS(state);
	}
	EFFECT_FINISH_CANVAS();
}

/* Turns the battle camera by the effect's delta each VBlank: the whole delta on
   the VBlank after one of the effect's ticks ends, half of it on the others,
   so the camera keeps turning while a tick runs long. */
void btCameraApplyPhasedDelta(void)
{
	BATTLE_EFFECT_STATE *state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	BATTLE_CAMERA		*camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
	Sint32				*phase = &state->cameraPhase;

	/* XXX (no evidence): whether the half turns are meant to keep the turn
	   smooth when an effect's tick runs long; the code shows the effect, not
	   the reason. */
	if(*phase == CAMERA_PHASE_TICK_DONE) {
		camera->yaw += state->cameraDelta;
		*phase = CAMERA_PHASE_IDLE;
	} else {
		camera->yaw += state->cameraDelta / 2;
		if(*phase == CAMERA_PHASE_HALF_TURNED)
			*phase = CAMERA_PHASE_IDLE;
		else
			*phase = CAMERA_PHASE_HALF_TURNED;
	}
}

#undef DJINN_HEAL_SWIRL_TICKS
#undef DJINN_HEAL_SWIRL_MOTES
#undef DJINN_HEAL_SWIRL_SLOTS
#undef DJINN_HEAL_MOTE_SIZE_MAX
#undef DJINN_HEAL_MOTE_DEPTH_NEAR
#undef DJINN_HEAL_MOTE_DEPTH_STEP
#undef DJINN_HEAL_MOTE_DEPTH_FAR
#undef DJINN_HEAL_SPARKLE_DEPTH_NEAR
#undef DJINN_HEAL_SPARKLE_DEPTH_STEP
#undef DJINN_HEAL_SPARKLE_DEPTH_FAR

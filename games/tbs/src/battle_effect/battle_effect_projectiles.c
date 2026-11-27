/* The Djinni entrance that opens the Djinn's effects, and the projectile
   volleys (Slash, Shuriken, Wing Flutter, Blizzard ...). Part of
   battle_effect.c. */

/* The sparks the Djinni entrance sets out round the user, of which the first
   ENTRANCE_SPARKS spin and gather. */
#define ENTRANCE_SPARK_SLOTS	  64
#define ENTRANCE_SPARKS			  32
/* A spinning spark is drawn at size ENTRANCE_SPARK_SIZE_MAX at depth
   ENTRANCE_SPARK_DEPTH_NEAR or nearer, one size smaller every
   ENTRANCE_SPARK_DEPTH_STEP farther, its depth held below
   ENTRANCE_SPARK_DEPTH_FAR. */
#define ENTRANCE_SPARK_SIZE_MAX	  8
#define ENTRANCE_SPARK_DEPTH_NEAR 250
#define ENTRANCE_SPARK_DEPTH_STEP 64
#define ENTRANCE_SPARK_DEPTH_FAR  634
/* The entrance's ticks, and the long entrance's, which ends in a burst of
   motes. */
#define ENTRANCE_TICKS			  64
#define ENTRANCE_LONG_TICKS		  84

/* Sparks swirl around a point that travels from the user to the goal and gather
   into a Djinni, which sways for 16 ticks and bursts into motes. */
void btEffectRunDjinniEntrance(BATTLE_EFFECT_PARM *args, Sint32 element,
	Sint32 side, Sint32 anchor, Sint32 *outX, Sint32 *outY)
{
	Sint32				 center[3];
	Sint32				 goal[3];
	Sint32				 step[3];
	Sint32				 point[3];
	SCREEN_POINT		 pos;
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	BLITTER_PAIR		 draw;
	Uint8				*motes;
	Sint32				 frmCnt;
	ACTOR				*tgt;
	ACTOR				*user;
	Uint8				*resource;
	Sint32				 sheetId;
	Sint32				 frm;
	Sint32				 i;

	state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
	motes = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
	state->args = args;
	btEffectStartCanvasLayer(0);
	state->fadeFrm = BACKGROUND_DARKEN_TICKS;
	state->fadeStep = 0;
	if(element > ELEMENT_JUPITER) {
		element -= DJINNI_ENTRANCE_LONG;
		frmCnt = ENTRANCE_LONG_TICKS;
	} else {
		frmCnt = ENTRANCE_TICKS;
	}
	switch(element) {
	case ELEMENT_VENUS:
		sheetId = RESOURCE_VENUS_DJINN_SMALL_SHEET;
		break;
	case ELEMENT_MERCURY:
		sheetId = RESOURCE_MERCURY_DJINN_SMALL_SHEET;
		break;
	case ELEMENT_MARS:
		sheetId = RESOURCE_MARS_DJINN_SMALL_SHEET;
		break;
	default:
		sheetId = RESOURCE_JUPITER_DJINN_SMALL_SHEET;
		break;
	}
	resource = syResourceGetTableEntry(sheetId);
	syWordsCopy(BG_PLTT, resource, SHEET_PALETTE_BYTES);
	syDecompressTaggedLZ(resource + SHEET_PALETTE_BYTES, state->sheet);
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, motes, 0, 0);
	if(side == EFFECT_SIDE_PARTY) {
		EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
			EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_ADDITIVE);
		EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
			EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_LIGHTEN);
	} else {
		EFFECT_LOAD_BLITTER(
			HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, COMPOSITE_ADDITIVE);
		EFFECT_LOAD_BLITTER(
			HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS, COMPOSITE_LIGHTEN);
	}
	draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
	draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
	user = btObjectGetStub(state->args->user)->actor;
	tgt = btObjectGetStub(state->args->tgts[0])->actor;
	for(i = 0; i != ENTRANCE_SPARK_SLOTS; i++) {
		PARTICLE *spark = &PARTICLES[i];
		Sint32	  angle = syRandom() & TRIG_ANGLE_MASK;
		Sint32	  speed = (syRandom() & 255) + 128;

		SET_POSITION(spark, 0, INTtoFIXED((syRandom() & 31) + 20), 0);
		SET_VELOCITY(
			spark, (sySin(angle) * speed) >> 5, 0, (syCos(angle) * speed) >> 5);
		spark->variant = 0;
	}
	EFFECT_START_TRANSFER(
		state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
	SET_VECTOR(center, user->x.fixed, 0, user->z.fixed);
	switch(anchor) {
	case DJINNI_ANCHOR_OVER_TARGET:
		SET_VECTOR(goal, tgt->x.fixed, toFIXED(60), tgt->z.fixed);
		break;
	case DJINNI_ANCHOR_TARGET_X:
		SET_VECTOR(goal, tgt->x.fixed, toFIXED(60), 0);
		break;
	case DJINNI_ANCHOR_OVER_USER:
		SET_VECTOR(goal, user->x.fixed, toFIXED(60), user->z.fixed);
		break;
	case DJINNI_ANCHOR_USER_X:
		SET_VECTOR(goal, user->x.fixed, toFIXED(60), 0);
		break;
	case DJINNI_ANCHOR_CENTRE:
		SET_VECTOR(goal, 0, toFIXED(60), 0);
		break;
	}
	SET_VECTOR(step, (goal[0] - center[0]) / 40, (goal[1] - center[1]) / 40,
		(goal[2] - center[2]) / 40);
	for(frm = 0; frm != frmCnt; frm++) {
		BATTLE_CAMERA *camera = BattleCameraWork;

		if(frm > 75)
			REG_BLDALPHA = EFFECT_BLEND_CANVAS(168 - frm * 2);
		if(frm == 8)
			sySndPlayStub(SE_POWER_UP);
		EFFECT_VIEW_FROM_CAMERA(camera);
		if(frm >= 6 && frm <= 45) {
			center[0] += step[0];
			center[1] += step[1];
			center[2] += step[2];
		}
		syMatrixTranslateVector(center);
		if(frm == 0)
			btEffectTintBattler(
				state->args->user, SPRITE_COLOR_WHITE, TINT_KEEP, TINT_KEEP, 0);
		if(frm == 24)
			btEffectTintBattler(state->args->user, SPRITE_COLOR_NORMAL,
				TINT_KEEP, TINT_KEEP, 0);
		for(i = 0; i != ENTRANCE_SPARKS; i++) {
			PARTICLE *spark = &PARTICLES[i];

			if(frm >= i / 8 && spark->variant == 0) {
				Sint32 size;

				syPushMatrix();
				switch(i & 3) {
				case SPIN_YAW:
					syRotY(frm * ((i + 8) * (TRIG_TURN / 2048)));
					break;
				case SPIN_PITCH:
					syRotX(-frm * ((i + 8) * (TRIG_TURN / 2048)));
					break;
				case SPIN_ROLL:
					syRotZ(-frm * ((i + 8) * (TRIG_TURN / 2048)));
					break;
				case SPIN_PITCH_AND_ROLL:
					syRotX(-frm * ((i + 8) * (TRIG_TURN / 2048)));
					syRotZ(-frm * ((i + 8) * (TRIG_TURN / 2048)));
					break;
				}
				btProjectionPoint(&spark->x, &pos);
				pos.x >>= 1;
				syPopMatrix();
				CLAMP(pos.depth, ENTRANCE_SPARK_DEPTH_NEAR,
					ENTRANCE_SPARK_DEPTH_FAR);
				size = ENTRANCE_SPARK_SIZE_MAX -
					(pos.depth - ENTRANCE_SPARK_DEPTH_NEAR) /
						ENTRANCE_SPARK_DEPTH_STEP;
				EFFECT_DRAW_MOTE(draw[1], canvas, motes, pos.x, pos.y, size);
				btParticleAdvanceWithGravity3D(spark, 60, 0);
				if(frm >= i / 8 + 24) {
					Sint32 pullX = -spark->x >> 7;
					Sint32 pullY = -spark->y >> 7;
					Sint32 pullZ = -spark->z >> 7;

					spark->velX += pullX;
					spark->velY += pullY;
					spark->velZ += pullZ;
					SET_VELOCITY(spark, spark->velX * 62 / DAMPING_SCALE,
						spark->velY * 62 / DAMPING_SCALE,
						spark->velZ * 62 / DAMPING_SCALE);
					if((pullX > -(toFIXED(1) / 32) &&
						   pullX < toFIXED(1) / 32) &&
						(pullZ > -(toFIXED(1) / 32) && pullZ < toFIXED(1) / 32))
						spark->variant = -1;
				}
			}
		}
		/* The Djinni sways over the goal. */
		if(frm >= 54 && frm <= 69) {
			SET_VECTOR(point, (sySin(frm * (TRIG_TURN / 64)) * 4), 0, 0);
			btProjectionPoint(point, &pos);
			*outX = pos.x;
			*outY = pos.y;
			pos.x >>= 1;
			draw[0](canvas, state->sheet, pos.x - SMALL_DJINNI_WIDTH / 2,
				pos.y - SMALL_DJINNI_HEIGHT / 2, SMALL_DJINNI_WIDTH,
				SMALL_DJINNI_HEIGHT);
		}
		/* The long version's Djinni bursts into motes. */
		if(frm == ENTRANCE_TICKS) {
			for(i = 0; i != ARRAY_COUNT(state->particles); i++) {
				PARTICLE *mote = &state->particles[i];
				Sint32	  angle = syRandom() & TRIG_ANGLE_MASK;
				Sint32	  speed = (syRandom() & 255) + 128;

				mote->x = *outX << 15;
				mote->y = toFIXED(*outY);
				mote->velX = (sySin(angle) * speed) >> 6;
				mote->velY = (syCos(angle) * speed) >> 5;
				mote->variant = (syRandom() & 15) + 8;
			}
		}
		if(frm >= ENTRANCE_TICKS) {
			for(i = 0; i != ARRAY_COUNT(state->particles); i++) {
				PARTICLE *mote = &state->particles[i];

				if(mote->variant >= 0) {
					Sint32 size = (mote->variant >> 3) + 2;

					EFFECT_DRAW_MOTE(draw[0], canvas, motes,
						FIXEDtoINT(mote->x), FIXEDtoINT(mote->y), size);
					btParticleAdvanceWithGravity2D(mote, 60, 0);
					mote->variant--;
				}
			}
		}
		EFFECT_PRESENT_CANVAS(state);
	}
	syCloseEvent(btEffectTransferCanvas);
	EFFECT_RELEASE_BLITTERS();
	syCloseEvent(btEffectDarkenBackgroundStep);
	syWordsClear(BG_CHAR_BLOCK(1), CANVAS_BYTES);
	syWordsClear(canvas, CANVAS_BYTES);
	REG_BLDALPHA = EFFECT_BLEND_ADD;
}

#define PROJECTILE_VOLLEY_ANIMATION_STEP_FAST \
	(ANIM_STEP_NORMAL * 3) /* the user's, while firing */

/* Six comets fly at each target, three ticks apart, and burst into a flash and
   sparks; the effect's variant picks one of three comets.

   Abilities: Slash, Wind Slash, Sonic Slash. */
void btEffectRunSlash(BATTLE_EFFECT_PARM *args)
{
	btEffectRunProjectileVolley(args, PROJECTILE_VOLLEY_SLASH);
}

/* Four spinning shurikens fly at each target, five ticks apart, and vanish
   once past it.

   Abilities: Shuriken. */
void btEffectRunShuriken(BATTLE_EFFECT_PARM *args)
{
	btEffectRunProjectileVolley(args, PROJECTILE_VOLLEY_SHURIKEN);
}

/* Ten pellets fly slowly at each target and bounce off it, up or down at
   random.

   Abilities: Crazy Voice. */
void btEffectRunCrazyVoice(BATTLE_EFFECT_PARM *args)
{
	btEffectRunProjectileVolley(args, PROJECTILE_VOLLEY_CRAZY_VOICE);
}

/* Five yellow sparks fly level at each target, five ticks apart.

   Abilities: Wing Flutter. */
void btEffectRunWingFlutter(BATTLE_EFFECT_PARM *args)
{
	btEffectRunProjectileVolley(args, PROJECTILE_VOLLEY_WING_FLUTTER);
}

/* Ten large yellow sparks stream level at each target, one a tick, spread up
   and down.

   Abilities: Wing Stroke. */
void btEffectRunWingStroke(BATTLE_EFFECT_PARM *args)
{
	btEffectRunProjectileVolley(args, PROJECTILE_VOLLEY_WING_STROKE);
}

/* Ten small yellow sparks stream level at each target, one a tick.

   Abilities: Wing Beat. */
void btEffectRunWingBeat(BATTLE_EFFECT_PARM *args)
{
	btEffectRunProjectileVolley(args, PROJECTILE_VOLLEY_WING_BEAT);
}

/* Fires the volley from the user's leap pose. */
static inline void btEffectRunVolleyWithAction(
	BATTLE_EFFECT_PARM *args, Sint32 mode)
{
	ACTOR *user;

	user = btObjectGetStub(args->user)->actor;
	fdActorSelectAnimationStub(user, POSE_LEAP);
	fdActorSetAnimationSpeedStub(user, PROJECTILE_VOLLEY_ANIMATION_STEP_FAST);
	btEffectRunProjectileVolley(args, mode);
	fdActorSetAnimationSpeedStub(user, ANIM_STEP_NORMAL);
}

/* From the user's action pose, one large comet in ice colours flies at each
   target and bursts into 80 sparks.

   Abilities: Cutting Edge. */
void btEffectRunCuttingEdge(BATTLE_EFFECT_PARM *args)
{
	btEffectRunVolleyWithAction(args, PROJECTILE_VOLLEY_CUTTING_EDGE);
}

/* From the user's action pose, seven fireballs trailing embers fly at each
   target, two ticks apart, each bursting into 40 sparks.

   Abilities: Heat Wave. */
void btEffectRunHeatWave(BATTLE_EFFECT_PARM *args)
{
	btEffectRunVolleyWithAction(args, PROJECTILE_VOLLEY_HEAT_WAVE);
}

/* Thirty ice chips fall from high above the user onto each target.

   Abilities: Sleet. */
void btEffectRunSleet(BATTLE_EFFECT_PARM *args)
{
	btEffectRunProjectileVolley(args, PROJECTILE_VOLLEY_SLEET);
}

/* Sixty ice chips stream at each target, one a tick, without knocking it back.

   Abilities: Ice Blessing. */
void btEffectRunIceBlessing(BATTLE_EFFECT_PARM *args)
{
	btEffectRunProjectileVolley(args, PROJECTILE_VOLLEY_ICE_BLESSING);
}

/* From the user's action pose, sixty ice chips stream at each target, one a
   tick, scattered up and down, without knocking it back.

   Abilities: Blizzard. */
void btEffectRunBlizzard(BATTLE_EFFECT_PARM *args)
{
	btEffectRunVolleyWithAction(args, PROJECTILE_VOLLEY_BLIZZARD);
}

#define PROJECTILE_VOLLEY_PARTICLE_COUNT PARTICLE_POOL_COUNT

/* The bytes of a ProjectileVolleyModes row. */
typedef enum VOLLEY_FIELD {
	PROJECTILE_VOLLEY_SHOTS,   /* shots at each target */
	PROJECTILE_VOLLEY_STEPS,   /* ticks a shot takes to reach its target */
	PROJECTILE_VOLLEY_STAGGER, /* ticks between shots */
	PROJECTILE_VOLLEY_DELAY,   /* ticks between targets */
	PROJECTILE_VOLLEY_SPARKS   /* sparks thrown on each hit */
} VOLLEY_FIELD;

/* comets: the top half of each */
#define PROJECTILE_VOLLEY_COMET_PICTURES  4096
#define PROJECTILE_VOLLEY_STREAK_PICTURES 8192 /* wind streaks */
/* The hit flashes turned on their side, after the upright ones. */
#define PROJECTILE_VOLLEY_FLASH_TURNED \
	(12 * 10 + 18 * 15 + 24 * 18 + 30 * 22 + 36 * 27 + 39 * 28 + 40 * 30)
/* The yellow spark sprite sheet: the small spark first, then the medium one,
   then the large one. */
#define PROJECTILE_VOLLEY_SMALL_SPARK_PICTURE_WIDTH	  12
#define PROJECTILE_VOLLEY_SMALL_SPARK_PICTURE_HEIGHT  8
#define PROJECTILE_VOLLEY_MEDIUM_SPARK_PICTURE_WIDTH  18
#define PROJECTILE_VOLLEY_MEDIUM_SPARK_PICTURE_HEIGHT 13
#define PROJECTILE_VOLLEY_LARGE_SPARK_PICTURE_WIDTH	  29
#define PROJECTILE_VOLLEY_LARGE_SPARK_PICTURE_HEIGHT  23
#define PROJECTILE_VOLLEY_SMALL_SPARK_PICTURE		  0
#define PROJECTILE_VOLLEY_MEDIUM_SPARK_PICTURE \
	PICTURE_BYTES(PROJECTILE_VOLLEY_SMALL_SPARK_PICTURE)
#define PROJECTILE_VOLLEY_LARGE_SPARK_PICTURE \
	(PROJECTILE_VOLLEY_MEDIUM_SPARK_PICTURE + \
		PICTURE_BYTES(PROJECTILE_VOLLEY_MEDIUM_SPARK_PICTURE))
/* The comets' width, and the shuriken's six pictures. */
#define PROJECTILE_VOLLEY_COMET_WIDTH		32
#define SHURIKEN_WIDTH						32
#define SHURIKEN_HEIGHT						24
#define PROJECTILE_VOLLEY_DJINNI_PICTURE	26048 /* the small Mercury Djinni */
#define PROJECTILE_VOLLEY_FIRST_SPARK		100 /* the first spark in PARTICLES */
#define PROJECTILE_VOLLEY_FIRST_TRAIL		512 /* the first trail in PARTICLES */
/* The most shots a volley fires at all its targets together, one fewer than the
   effect's particles; past it, the volley fires at the first target alone. */
#define PROJECTILE_VOLLEY_SHOTS_MAX			63
/* A hit's sparks fly at up to 4 pixels a tick each way, as a random -128 to
   127 shifted up; Cutting Edge's at up to 8. */
#define PROJECTILE_VOLLEY_SPARK_SPEED_SHIFT 11
#define CUTTING_EDGE_SPARK_SPEED_SHIFT		12

/* Shots fly from the user to each target and burst into a flash and sparks when
   they pass it. The hit sound plays at most once every 8 ticks. */
void btEffectRunProjectileVolley(BATTLE_EFFECT_PARM *args, Sint32 mode)
{
	SCREEN_POINT		 pos;
	Sint32				 screenX;
	Sint32				 screenY;
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	Sint32				 i;
	Sint32				 j;
	Sint32				 frm;
	BLITTER_PAIR		 draw;
	BATTLE_CAMERA		*camera;
	Uint8				*motes;
	Sint32				 sndCooldown;
	Sint32				 total;
	Sint32				 comet;
	Sint32				 shots;
	Sint32				 steps;
	ACTOR				*user;
	Sint32				 k;
	Sint32				 trailsOrShift;
	Sint32				 hit;
	Sint32				 x;

	state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
	camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
	motes = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
	sndCooldown = 0;
	state->args = args;
	comet = args->variant;
	if(mode == PROJECTILE_VOLLEY_ICE_BLESSING)
		btEffectStartCanvasLayer(0);
	else
		btEffectStartCanvasLayer(BGCNT_PRIORITY(1));
	if(state->args->prelude == 1)
		btEffectRunDjinniEntrance(args, ELEMENT_MERCURY, state->args->side,
			DJINNI_ANCHOR_OVER_USER, &screenX, &screenY);

	if(mode == PROJECTILE_VOLLEY_CUTTING_EDGE) {
		if(state->args->side == EFFECT_SIDE_PARTY)
			EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
				EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_LIGHTEN);
		else
			EFFECT_LOAD_BLITTER(
				HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, COMPOSITE_LIGHTEN);
	} else {
		if(state->args->side == EFFECT_SIDE_PARTY)
			EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
				EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_ADDITIVE);
		else
			EFFECT_LOAD_BLITTER(
				HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, COMPOSITE_ADDITIVE);
	}
	draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);

	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, motes, 0, 0);
	if(mode == PROJECTILE_VOLLEY_SLASH ||
		mode == PROJECTILE_VOLLEY_CUTTING_EDGE ||
		mode == PROJECTILE_VOLLEY_IMPACT_COMETS) {
		if(mode == PROJECTILE_VOLLEY_CUTTING_EDGE)
			comet = PROJECTILE_VOLLEY_COMET_C;
		if(mode == PROJECTILE_VOLLEY_IMPACT_COMETS)
			comet = PROJECTILE_VOLLEY_COMET_A;
		if(comet == PROJECTILE_VOLLEY_COMET_A)
			btResourceLoadAndDecompress(RESOURCE_SLASH_COMET_SHEET,
				state->sheet + PROJECTILE_VOLLEY_COMET_PICTURES, 1, 1);
		else if(comet == PROJECTILE_VOLLEY_COMET_B)
			btResourceLoadAndDecompress(RESOURCE_WIND_SLASH_COMET_SHEET,
				state->sheet + PROJECTILE_VOLLEY_COMET_PICTURES, 1, 1);
		else
			btResourceLoadAndDecompress(RESOURCE_SONIC_SLASH_COMET_SHEET,
				state->sheet + PROJECTILE_VOLLEY_COMET_PICTURES, 1, 1);
		if(mode == PROJECTILE_VOLLEY_CUTTING_EDGE) {
			const void *pal = syResourceGetTableEntry(RESOURCE_ICE_BLOCK_SHEET);
			syWordsCopy(BG_PLTT, pal, SHEET_PALETTE_BYTES);
		}
		btResourceLoadAndDecompress(RESOURCE_WIND_STREAK_SHEET,
			state->sheet + PROJECTILE_VOLLEY_STREAK_PICTURES, 1, 0);
		/* The ice colours again, though the streaks left them as they were. */
		if(mode == PROJECTILE_VOLLEY_CUTTING_EDGE) {
			const void *pal = syResourceGetTableEntry(RESOURCE_ICE_BLOCK_SHEET);
			syWordsCopy(BG_PLTT, pal, SHEET_PALETTE_BYTES);
		}
		EFFECT_SET_TRANSFER(
			state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
	} else if(mode == PROJECTILE_VOLLEY_SHURIKEN) {
		btResourceLoadAndDecompress(
			RESOURCE_SHURIKEN_SHEET, state->sheet, 1, 1);
		REG_BLDCNT = 0;
		EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
	} else if(mode == PROJECTILE_VOLLEY_CRAZY_VOICE) {
		syWordsCopy(BG_PLTT,
			syResourceGetTableEntry(RESOURCE_SLASH_COMET_SHEET),
			SHEET_PALETTE_BYTES);
		btResourceLoadAndDecompress(
			RESOURCE_PROJECTILE_VOLLEY_IMAGE, state->sheet, 0, 0);
		EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
	} else if(mode == PROJECTILE_VOLLEY_WING_FLUTTER ||
		mode == PROJECTILE_VOLLEY_WING_BEAT ||
		mode == PROJECTILE_VOLLEY_WING_STROKE) {
		btResourceLoadAndDecompress(
			RESOURCE_YELLOW_SPARK_SHEET, state->sheet, 1, 1);
		EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
	} else if(mode == PROJECTILE_VOLLEY_HEAT_WAVE) {
		btResourceLoadAndDecompress(
			RESOURCE_FIREBALL_SHEET, state->sheet, 1, 1);
		EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
	} else {
		btResourceLoadAndDecompress(
			RESOURCE_ICE_CHIP_SHEET, state->sheet, 1, 1);
		btResourceLoadAndDecompress(RESOURCE_MERCURY_DJINN_SMALL_SHEET,
			state->sheet + PROJECTILE_VOLLEY_DJINNI_PICTURE, 1, 0);
		EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
	}
	sySetEvent(btEffectTransferCanvas, EVENT_PASS_VBLANK);

	user = btObjectGetStub(state->args->user)->actor;
	shots = (ProjectileVolleyModes[mode][PROJECTILE_VOLLEY_SHOTS]);
	steps = (ProjectileVolleyModes[mode][PROJECTILE_VOLLEY_STEPS]);
	{
		Sint32 stagger =
			(ProjectileVolleyModes[mode][PROJECTILE_VOLLEY_STAGGER]);
		Sint32 delay = (ProjectileVolleyModes[mode][PROJECTILE_VOLLEY_DELAY]);
		Sint32 height = btUnitGetEffectHeightStub(state->args->user);

		if(state->args->tgtCnt * shots > PROJECTILE_VOLLEY_SHOTS_MAX)
			state->args->tgtCnt = 1;

		/* Each shot starts at the user and is aimed at its target. */

		EFFECT_FOR_EACH_TARGET(i, state->args) {
			ACTOR *tgt = btObjectGetStub(state->args->tgts[i])->actor;
			Sint32 tgtHeight = btUnitGetEffectHeightStub(state->args->tgts[i]);

			for(j = 0; j != shots; j++) {
				PARTICLE *shot = &state->particles[i * shots + j];

				shot->x = user->x.fixed;
				if(mode == PROJECTILE_VOLLEY_SLEET)
					shot->y =
						user->y.fixed + toFIXED(syRandom() & 15) + toFIXED(58);
				else if(mode == PROJECTILE_VOLLEY_ICE_BLESSING)
					shot->y = user->y.fixed + height / 2;
				else if(mode == PROJECTILE_VOLLEY_HEAT_WAVE)
					shot->y = user->y.fixed + height / 2;
				else if(mode == PROJECTILE_VOLLEY_BLIZZARD)
					shot->y = user->y.fixed + height / 2 +
						toFIXED(16 - (syRandom() & 31));
				else if(mode == PROJECTILE_VOLLEY_WING_FLUTTER ||
					mode == PROJECTILE_VOLLEY_WING_BEAT)
					shot->y = user->y.fixed + height / 2 +
						toFIXED(16 - (syRandom() & 31));
				else if(mode == PROJECTILE_VOLLEY_WING_STROKE)
					shot->y = user->y.fixed + height / 2 +
						toFIXED(32 - (syRandom() & 63));
				else if(mode == PROJECTILE_VOLLEY_CUTTING_EDGE)
					shot->y = user->y.fixed + height / 2;
				else
					shot->y = user->y.fixed + height;
				shot->z = user->z.fixed;
				shot->velX = (tgt->x.fixed - shot->x) / steps;
				if(mode == PROJECTILE_VOLLEY_SLEET)
					shot->velY =
						(Sint32)(tgt->y.fixed + toFIXED(syRandom() & 63) -
							shot->y - toFIXED(12)) /
						steps;
				else if(mode == PROJECTILE_VOLLEY_IMPACT_COMETS)
					shot->velY =
						(Sint32)(tgt->y.fixed + toFIXED(syRandom() & 7) -
							shot->y + toFIXED(22)) /
						steps;
				else if(mode == PROJECTILE_VOLLEY_BLIZZARD)
					shot->velY =
						(Sint32)(tgt->y.fixed +
							toFIXED(64 - (syRandom() & 63)) - shot->y) /
						steps;
				else if(mode == PROJECTILE_VOLLEY_ICE_BLESSING)
					shot->velY =
						(Sint32)(tgt->y.fixed + toFIXED(syRandom() & 31) -
							shot->y + toFIXED(4)) /
						steps;
				else if(mode == PROJECTILE_VOLLEY_WING_FLUTTER ||
					mode == PROJECTILE_VOLLEY_WING_BEAT ||
					mode == PROJECTILE_VOLLEY_WING_STROKE ||
					mode == PROJECTILE_VOLLEY_CUTTING_EDGE)
					shot->velY = 0;
				else if(mode == PROJECTILE_VOLLEY_HEAT_WAVE)
					shot->velY = (Sint32)(tgt->y.fixed + tgtHeight / 2 +
									 toFIXED(syRandom() & 15) - shot->y) /
						steps;
				else
					shot->velY = (Sint32)(tgt->y.fixed + tgtHeight -
									 toFIXED(syRandom() & 15) - shot->y) /
						steps;
				shot->velZ = (tgt->z.fixed - shot->z) / steps;
				shot->variant = 0;
			}
		}
		for(i = 0; i != PROJECTILE_VOLLEY_PARTICLE_COUNT; i++)
			PARTICLES[i].variant = 0;
		if(mode == PROJECTILE_VOLLEY_HEAT_WAVE)
			total = state->args->tgtCnt * delay + shots * stagger + 32;
		else
			total = state->args->tgtCnt * delay + shots * stagger + 16;

		for(frm = 0; frm != total; frm++) {
			Sint32 idx;

			if(sndCooldown > 0)
				sndCooldown--;
			if(mode == PROJECTILE_VOLLEY_HEAT_WAVE) {
				if(frm == 4)
					sySndPlayStub(SE_WHOOSH);
				if(frm == 32)
					EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
			} else if(mode == PROJECTILE_VOLLEY_SLEET) {
				if(frm == 48)
					EFFECT_SHOW_DAMAGE(SE_IMPACT);
			} else if(mode != PROJECTILE_VOLLEY_CUTTING_EDGE) {
				if(frm == 16)
					EFFECT_SHOW_DAMAGE(SE_IMPACT);
			}

			/* The prelude's small Djinni circles, then rises away. */
			if(state->args->prelude == 1) {
				Sint32 x = FIXEDtoINT(-sySin(frm * (TRIG_TURN / 32)) * 4) +
					screenX / 2 - 10;
				Sint32 y = FIXEDtoINT(syCos(frm * (TRIG_TURN / 32)) * 2) +
					screenY - 24;

				if(frm > 69)
					y = y - frm * 2 + 138;
				if(state->args->side == EFFECT_SIDE_PARTY)
					EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
						EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
						COMPOSITE_ADDITIVE);
				else
					EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
						EFFECT_CLIP_BOUNDS, COMPOSITE_ADDITIVE);
				draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
				/* Drawn twice, so brighter, over its first four ticks. */
				if(frm <= 3)
					draw[1](canvas,
						state->sheet + PROJECTILE_VOLLEY_DJINNI_PICTURE, x, y,
						SMALL_DJINNI_WIDTH, SMALL_DJINNI_HEIGHT);
				draw[1](canvas, state->sheet + PROJECTILE_VOLLEY_DJINNI_PICTURE,
					x, y, SMALL_DJINNI_WIDTH, SMALL_DJINNI_HEIGHT);
				syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
			}

			EFFECT_VIEW_FROM_CAMERA(camera);
			idx = 0;
			EFFECT_FOR_EACH_TARGET(i, state->args) {
				ACTOR *tgt = btObjectGetStub(state->args->tgts[i])->actor;

				for(j = 0; j != shots; j++, idx++) {
					if(j * stagger + i * delay < frm) {
						PARTICLE *shot = &state->particles[i * shots + j];

						btProjectionPoint(shot, &pos);
						pos.x >>= 1;
						shot->x += shot->velX;
						shot->y += shot->velY;
						shot->z += shot->velZ;
						if(mode == PROJECTILE_VOLLEY_HEAT_WAVE) {
							/* A fireball leaves a trail of embers. */
							trailsOrShift = 0;
							for(k = PROJECTILE_VOLLEY_FIRST_TRAIL;
								k != PROJECTILE_VOLLEY_PARTICLE_COUNT; k++) {
								PARTICLE *particle = &PARTICLES[k];

								if(particle->variant == 0) {
									particle->x = toFIXED(pos.x);
									particle->y = toFIXED(pos.y);
									particle->velX = ((syRandom() & 255) - 128)
										<< 11;
									particle->velY = ((syRandom() & 255) - 128)
										<< 11;
									particle->variant = (syRandom() & 7) + 32;
									trailsOrShift++;
									if(trailsOrShift == 2)
										break;
								}
							}
						}
						if(shot->variant == 0) {
							/* A shot hits once it passes its target. */
							x = shot->x;
							if(tgt->x.fixed < 0) {
								hit = x < 0;
								/* FAKEMATCH(dead): two updates that cancel,
								 * under a test that does nothing. */
								if(tgt) {
									hit ^= x;
									hit ^= x;
								}
								if(!hit)
									goto drawShot;
							} else {
								hit = x >= 0;
								/* FAKEMATCH(dead): the same pair for the other
								 * side. */
								if(tgt) {
									hit ^= x;
									hit ^= x;
								}
								if(!hit)
									goto drawShot;
							}
							if(ABS(x) >= ABS(tgt->x.fixed)) {
								Sint32	  spawned = 0;
								PARTICLE *flash = &PARTICLES[idx];

								shot->variant = 1;
								if(mode == PROJECTILE_VOLLEY_CUTTING_EDGE) {
									EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
								} else if(mode != PROJECTILE_VOLLEY_HEAT_WAVE &&
									sndCooldown == 0) {
									sndCooldown = 8;
									sySndPlayStub(SE_WATER);
								}
								/* The hit: a flash, a shake, and the target
								   flinches. */
								if(mode == PROJECTILE_VOLLEY_CRAZY_VOICE) {
									/* Pellets bounce off, up or down. */
									Sint32 kick = ((syRandom() & 31) + 32)
										<< 12;

									if(syRandom() & 1)
										shot->velY += kick;
									else
										shot->velY -= kick;
									shot->velX = -shot->velX;
								}
								flash->variant = 1;
								SET_POSITION(flash, pos.x, pos.y, 0);
								if(mode != PROJECTILE_VOLLEY_SLEET)
									state->shakeFrm = 2;
								btEffectTintBattler(state->args->tgts[i],
									SPRITE_COLOR_WHITE, POSE_DAMAGE, i, 8);
								if(mode != PROJECTILE_VOLLEY_SLEET &&
									mode != PROJECTILE_VOLLEY_BLIZZARD &&
									mode != PROJECTILE_VOLLEY_ICE_BLESSING) {
									if(mode == PROJECTILE_VOLLEY_CUTTING_EDGE)
										btMotionApplyKnockbackStub(
											state->args->tgts[i],
											KNOCKBACK_HURL);
									else
										btMotionApplyKnockbackStub(
											state->args->tgts[i],
											KNOCKBACK_SKID);
								}
								/* Sparks fly from the hit. */
								trailsOrShift =
									mode == PROJECTILE_VOLLEY_CUTTING_EDGE
									? CUTTING_EDGE_SPARK_SPEED_SHIFT
									: PROJECTILE_VOLLEY_SPARK_SPEED_SHIFT;
								for(k = PROJECTILE_VOLLEY_FIRST_SPARK;
									k != PROJECTILE_VOLLEY_FIRST_TRAIL; k++) {
									PARTICLE *particle = &PARTICLES[k];

									if(particle->variant == 0) {
										particle->x = toFIXED(pos.x);
										particle->y = toFIXED(pos.y);
										particle->velX =
											((syRandom() & 255) - 128)
											<< trailsOrShift;
										particle->velY =
											((syRandom() & 255) - 128)
											<< trailsOrShift;
										particle->variant =
											(syRandom() & 7) + 16;
										spawned++;
										if(spawned ==
											(ProjectileVolleyModes
													[mode]
													[PROJECTILE_VOLLEY_SPARKS]))
											break;
									}
								}
							}
						}
					drawShot:
						if(mode == PROJECTILE_VOLLEY_SLASH ||
							mode == PROJECTILE_VOLLEY_CUTTING_EDGE ||
							mode == PROJECTILE_VOLLEY_IMPACT_COMETS) {
							if(state->args->side == EFFECT_SIDE_PARTY)
								EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
									EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
									COMPOSITE_LIGHTEN);
							else
								EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
									EFFECT_CLIP_BOUNDS, COMPOSITE_LIGHTEN);
							EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED)(canvas,
								state->sheet + PROJECTILE_VOLLEY_COMET_PICTURES,
								pos.x - PROJECTILE_VOLLEY_COMET_WIDTH / 2,
								pos.y - ProjectileVolleyCometHeights[comet],
								PROJECTILE_VOLLEY_COMET_WIDTH,
								ProjectileVolleyCometHeights[comet]);
							syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
							if(state->args->side == EFFECT_SIDE_PARTY)
								EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
									EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP,
									COMPOSITE_LIGHTEN);
							else
								EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
									EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y,
									COMPOSITE_LIGHTEN);
							EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED)(canvas,
								state->sheet + PROJECTILE_VOLLEY_COMET_PICTURES,
								pos.x - PROJECTILE_VOLLEY_COMET_WIDTH / 2,
								pos.y, PROJECTILE_VOLLEY_COMET_WIDTH,
								ProjectileVolleyCometHeights[comet]);
							syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
						} else if(mode == PROJECTILE_VOLLEY_SHURIKEN) {
							if(ABS(shot->x) <= ABS(tgt->x.fixed))
								draw[0](canvas,
									state->sheet +
										frm % 6 * PICTURE_BYTES(SHURIKEN),
									pos.x - SHURIKEN_WIDTH / 2,
									pos.y - SHURIKEN_HEIGHT / 2, SHURIKEN_WIDTH,
									SHURIKEN_HEIGHT);
						} else if(mode == PROJECTILE_VOLLEY_SLEET ||
							mode == PROJECTILE_VOLLEY_BLIZZARD ||
							mode == PROJECTILE_VOLLEY_ICE_BLESSING) {
							Sint32 picture = j & 3;
							draw[0](canvas,
								state->sheet +
									ProjectileVolleyPelletPictures[picture],
								pos.x - 4, pos.y - 4, 8, 8);
						} else if(mode == PROJECTILE_VOLLEY_CRAZY_VOICE) {
							draw[0](canvas, state->sheet + j % 6 * (8 * 16),
								pos.x - 4, pos.y - 8, 8, 16);
						} else if(mode == PROJECTILE_VOLLEY_WING_FLUTTER) {
							EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet,
								PROJECTILE_VOLLEY_MEDIUM_SPARK_PICTURE,
								pos.x - 9, pos.y - 7);
						} else if(mode == PROJECTILE_VOLLEY_WING_BEAT) {
							EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet,
								PROJECTILE_VOLLEY_SMALL_SPARK_PICTURE,
								pos.x - 6, pos.y - 4);
						} else if(mode == PROJECTILE_VOLLEY_WING_STROKE) {
							EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet,
								PROJECTILE_VOLLEY_LARGE_SPARK_PICTURE,
								pos.x - 15, pos.y - 12);
						} else {
							draw[0](canvas, state->sheet,
								pos.x - FLAMEBALL_WIDTH / 2,
								pos.y - FLAMEBALL_HEIGHT / 2, FLAMEBALL_WIDTH,
								FLAMEBALL_HEIGHT);
						}
					}
				}
			}

			if(mode == PROJECTILE_VOLLEY_SLASH ||
				mode == PROJECTILE_VOLLEY_CUTTING_EDGE ||
				mode == PROJECTILE_VOLLEY_IMPACT_COMETS) {
				for(i = 0; i != state->args->tgtCnt * shots; i++) {
					PARTICLE *flash = &PARTICLES[i];

					if(flash->variant == 1) {
						Sint32 step = flash->z / 2;

						if(!(i & 1)) {
							EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
								EFFECT_CLIP_BOUNDS, COMPOSITE_LIGHTEN);
							EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED)(canvas,
								state->sheet +
									ProjectileVolleyFlashPictures[step],
								flash->x - ProjectileVolleyFlashWidths[step],
								flash->y - ProjectileVolleyFlashRises[step],
								ProjectileVolleyFlashWidths[step],
								ProjectileVolleyFlashHeights[step]);
							syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
							EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
								EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP,
								COMPOSITE_LIGHTEN);
							EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED)(canvas,
								state->sheet +
									ProjectileVolleyFlashPictures[step],
								flash->x,
								flash->y + ProjectileVolleyFlashSteps[step] -
									ProjectileVolleyFlashRises[step],
								ProjectileVolleyFlashWidths[step],
								ProjectileVolleyFlashHeights[step]);
							syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
						} else {
							EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
								EFFECT_CLIP_BOUNDS, COMPOSITE_LIGHTEN);
							EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED)(canvas,
								state->sheet +
									ProjectileVolleyFlashPictures[step] +
									PROJECTILE_VOLLEY_FLASH_TURNED,
								flash->x - ProjectileVolleyFlashRises[step],
								flash->y - ProjectileVolleyFlashWidths[step],
								ProjectileVolleyFlashHeights[step],
								ProjectileVolleyFlashWidths[step]);
							syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
							EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
								EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP,
								COMPOSITE_LIGHTEN);
							EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED)(canvas,
								state->sheet +
									ProjectileVolleyFlashPictures[step] +
									PROJECTILE_VOLLEY_FLASH_TURNED,
								flash->x + ProjectileVolleyFlashSteps[step] -
									ProjectileVolleyFlashRises[step],
								flash->y, ProjectileVolleyFlashHeights[step],
								ProjectileVolleyFlashWidths[step]);
							syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
						}
						flash->z++;
						if(flash->z == 12)
							flash->variant = 0;
					}
				}
			}

			/* Sparks fall and bounce, shrinking as they die. */
			for(i = PROJECTILE_VOLLEY_FIRST_SPARK;
				i != PROJECTILE_VOLLEY_FIRST_TRAIL; i++) {
				PARTICLE *particle = &PARTICLES[i];

				if(particle->variant > 0) {
					Sint32 size = (particle->variant >> 3) + 1;

					EFFECT_DRAW_MOTE(draw[0], canvas, motes,
						FIXEDtoINT(particle->x), FIXEDtoINT(particle->y), size);
					btParticleAdvanceWithGravity2D(
						particle, 60, toFIXED(1) / 16);
					if(particle->y > toFIXED(112))
						particle->velY = -particle->velY / 2;
					particle->variant--;
				}
			}

			/* Fireball trails rise and fade. */
			for(i = PROJECTILE_VOLLEY_FIRST_TRAIL;
				i != PROJECTILE_VOLLEY_PARTICLE_COUNT; i++) {
				PARTICLE *particle = &PARTICLES[i];

				if(particle->variant > 0) {
					Sint32 size = (particle->variant >> 4) + 1;

					EFFECT_DRAW_MOTE(draw[0], canvas, motes,
						FIXEDtoINT(particle->x), FIXEDtoINT(particle->y), size);
					btParticleAdvanceWithGravity2D(
						particle, 60, -toFIXED(1) / 4);
					particle->variant--;
				}
			}
			btCameraShake(4, 4);
			EFFECT_END_TICK(state);
		}
	}

	EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

/* Sends a unit back to its place (its anchor) with a hop. */
UNUSED void btEffectHopBackToSpot(Sint32 id)
{
	BATTLE_OBJECT *slot = btObjectGetStub(id);
	ACTOR		  *actor = slot->actor;

	actor->acceleration = toFIXED(2);
	actor->speedLimit = toFIXED(8);
	actor->velY = toFIXED(4);
	actor->gravity = BATTLE_UNIT_GRAVITY;
	actor->bounce = 0;
	actor->steeringFlg = 0;
	actor->snapToTgt = 1;
	fdActorStartIdleStub(actor);
	fdActorSetMoveTargetStub(actor, slot->anchorX, 0, slot->anchorZ);
	fdActorSelectAnimationStub(actor, POSE_STANDING);
}

/* How far btEffectSetApproachMotion takes a unit toward the other. */
#define APPROACH_PERCENT 80

/* Moves a unit APPROACH_PERCENT of the way to another one over ticks, in its
   leap pose. */
void btEffectSetApproachMotion(Sint32 id, Sint32 tgtId, Sint32 ticks)
{
	BATTLE_OBJECT *slot = btObjectGetStub(id);
	BATTLE_OBJECT *tgtSlot = btObjectGetStub(tgtId);
	ACTOR		  *actor = slot->actor;
	ACTOR		  *tgt = tgtSlot->actor;
	Sint32		   rate = APPROACH_PERCENT;
	FIXED		   diffX = tgt->x.fixed - actor->x.fixed;
	FIXED		   startX = actor->x.fixed;
	FIXED		   dx = rate * diffX / 100;
	FIXED		   diffZ = tgt->z.fixed - actor->z.fixed;
	FIXED		   startZ = actor->z.fixed;
	FIXED		   dz = rate * diffZ / 100;
	FIXED		   x = startX + dx;
	FIXED		   z = startZ + dz;
	Sint32		   stepX = dx >> 8;
	Sint32		   stepZ = dz >> 8;
	Sint32		   speed;

	speed = sySquartIWRAM(stepX * stepX + stepZ * stepZ);
	speed = (speed << 8) / ticks;
	actor->acceleration = speed;
	actor->speedLimit = speed;
	actor->snapToTgt = 1;
	actor->gravity = BATTLE_UNIT_GRAVITY;
	actor->velY = 0;
	actor->bounce = 0;
	actor->steeringFlg = 1;
	fdActorStartIdleStub(actor);
	fdActorSetMoveTargetStub(actor, x, 0, z);
	fdActorSelectAnimationStub(actor, POSE_LEAP);
}

#undef PROJECTILE_VOLLEY_ANIMATION_STEP_FAST
#undef PROJECTILE_VOLLEY_PARTICLE_COUNT
#undef PROJECTILE_VOLLEY_COMET_PICTURES
#undef PROJECTILE_VOLLEY_STREAK_PICTURES
#undef PROJECTILE_VOLLEY_FLASH_TURNED
#undef PROJECTILE_VOLLEY_MEDIUM_SPARK_PICTURE
#undef PROJECTILE_VOLLEY_LARGE_SPARK_PICTURE
#undef PROJECTILE_VOLLEY_DJINNI_PICTURE
#undef PROJECTILE_VOLLEY_FIRST_SPARK
#undef PROJECTILE_VOLLEY_FIRST_TRAIL
#undef PROJECTILE_VOLLEY_SHOTS_MAX
#undef PROJECTILE_VOLLEY_SPARK_SPEED_SHIFT
#undef CUTTING_EDGE_SPARK_SPEED_SHIFT
#undef APPROACH_PERCENT
#undef ENTRANCE_SPARK_SLOTS
#undef ENTRANCE_SPARKS
#undef ENTRANCE_SPARK_SIZE_MAX
#undef ENTRANCE_SPARK_DEPTH_NEAR
#undef ENTRANCE_SPARK_DEPTH_STEP
#undef ENTRANCE_SPARK_DEPTH_FAR
#undef ENTRANCE_TICKS
#undef ENTRANCE_LONG_TICKS

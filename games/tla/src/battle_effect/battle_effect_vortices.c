/* Froth's bubbles that burst into vortices, and Whirlwind's tornadoes. Part of
   battle_effect.c. */

#define WHIRLWIND_TICKS 80 /* ticks of the tornadoes */

/* Froth's sheet: the vortex's three pictures, each its left half (the right is
   drawn mirrored), then the bubble. */
#define FROTH_VORTEX_HALF_WIDTH		16
#define FROTH_VORTEX_HALF_HEIGHT	64
#define FROTH_BUBBLE_PICTURE_WIDTH	12
#define FROTH_BUBBLE_PICTURE_HEIGHT 24
#define FROTH_BUBBLE_PICTURE		(3 * PICTURE_BYTES(FROTH_VORTEX_HALF))
/* The bubbles set out in the effect's particles, of which each variant flies
   its FROTH_BUBBLE_COUNT; the targets' places follow from
   FROTH_FIRST_TARGET. */
#define FROTH_BUBBLE_SLOTS			30
#define FROTH_FIRST_TARGET			32
/* The depths a bubble's projection is held between. */
#define FROTH_BUBBLE_DEPTH_NEAR		160
#define FROTH_BUBBLE_DEPTH_FAR		799
/* The ticks a vortex turns where a bubble landed. */
#define FROTH_VORTEX_TICKS			16

/* The glints that sparkle on the tornadoes, four on each. */
#define WHIRLWIND_GLINTS	 16
/* The ticks a glint lives: three pictures, two ticks each. */
#define WHIRLWIND_GLINT_LIFE 6
/* The sweep sways this far either way, a sway every 64 ticks, and each tornado
   turns round it every 32. */
#define WHIRLWIND_SWEEP_SWAY 16

/* The tornado pictures, after the six glint pictures in the sprite sheet. */
#define WHIRLWIND_TORNADO_PICTURES GLINT_SET_BYTES
#define WHIRLWIND_SECTION_BYTES	   PICTURE_BYTES(TORNADO_STRIP)

/* Bubbles fly out of the top of the user, hang, and home in on the targets;
   each that lands bursts into a swirling vortex, knocking back its target,
   while the camera swings round.

   Abilities: Froth, Froth Sphere, Froth Spiral. */
void btEffectRunFroth(BATTLE_EFFECT_PARM *args)
{
	SCREEN_POINT		 pos;
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	BLITTER_PAIR		 draw;
	MOTION_OBJECT		*src;
	MOTION_OBJECT		*object;
	PARTICLE			*point;
	PARTICLE			*seat;
	PARTICLE			*tgt;
	BATTLE_CAMERA		*camera;
	Sint32				 frm;
	Sint32				 y;
	Sint32				 i;
	Sint8				*landed;

	/* Set up: the canvas layer, the vortex sprite sheet and two blitters, the
	   one on the right mirrored. landed is nonzero once a bubble has struck. */
	landed = (Sint8 *)Ram_MapMetatileBuffer;
	state = Ram_HeapSlots->battleEff;
	canvas = Ram_HeapSlots->battleCanvas;
	btEffectStartCanvasLayer(BGCNT_PRIORITY(1));
	btResourceLoadAndDecompress(RESOURCE_VORTEX_SHEET, state->sheet, 1, 1);
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
	draw[0] = Ram_HeapSlots->ramCode;
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
		EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP, BLITTER_MAX);
	REG_BLDALPHA = BLEND_ALPHA(15, 15);
	draw[1] = Ram_HeapSlots->ramCodePaired;

	/* The bubbles start at the top of the user, at random velocities. */
	src = btObjectGetStub(args->user)->obj;
	y = src->y + btUnitGetEffectHeightStub(args->user);
	for(i = 0; i != FROTH_BUBBLE_SLOTS; i++) {
		point = &state->particles[i];
		SET_POSITION(point, src->x, y, src->z);
		SET_VELOCITY(point, toFIXED((Sint32)((syRandom() & 255) - 127)) >> 5,
			toFIXED((Sint32)((syRandom() & 127) - 16)) >> 6,
			toFIXED((Sint32)((syRandom() & 255) - 127)) >> 5);
		point->variant = -1;
		landed[i] = 0;
	}

	/* Each target's seat on the ground. */
	EFFECT_FOR_EACH_TARGET(i, args) {
		seat = &state->particles[FROTH_FIRST_TARGET + i];
		object = btObjectGetStub(args->tgts[i])->obj;
		SET_POSITION(seat, object->x, 0, object->z);
	}

	state->cameraDelta = 0;
	state->cameraPhase = CAMERA_PHASE_IDLE;
	sySetEvent(btCameraApplyPhasedDelta, EVENT_PASS_VBLANK);
	EFFECT_START_TRANSFER(
		state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
	sySndPlayStub(SE_SCATTER);

	for(frm = 0; frm != VorticesCounts[args->variant][FROTH_TICKS]; frm++) {
		/* The camera turns between ticks 17 and 63. */
		camera = Ram_HeapSlots->cameraState;
		if(frm >= 17 && frm < 64)
			state->cameraDelta = TRIG_TURN * 3 / 512;
		else
			state->cameraDelta = 0;
		if(frm == VorticesCounts[args->variant][FROTH_TICKS] - 16)
			EFFECT_SHOW_DAMAGE(SE_IMPACT);
		EFFECT_VIEW_FROM_CAMERA(camera);

		for(i = 0; i != VorticesCounts[args->variant][FROTH_BUBBLE_COUNT];
			i++) {
			point = &state->particles[i];

			/* A bubble leaves the user two ticks after the one before it. */
			if(frm > i * 2 && landed[i] == 0) {
				btEffectPosProjectPoint((Sint32 *)point, &pos);
				pos.x >>= 1;
				/* DEAD: the clamped depth is never read; the ROM keeps the
				   clamp. */
				CLAMP(
					pos.depth, FROTH_BUBBLE_DEPTH_NEAR, FROTH_BUBBLE_DEPTH_FAR);
				draw[0](canvas, state->sheet + FROTH_BUBBLE_PICTURE,
					pos.x - FROTH_BUBBLE_PICTURE_WIDTH / 2,
					pos.y - FROTH_BUBBLE_PICTURE_HEIGHT / 2,
					FROTH_BUBBLE_PICTURE_WIDTH, FROTH_BUBBLE_PICTURE_HEIGHT);
				point->x += point->velX;
				point->y += point->velY;
				point->z += point->velZ;
			}

			/* After 48 ticks it steers toward its target's seat, damped for
			   the first 37 of them, and lands. */
			if(frm > i * 2 + 48 && landed[i] == 0) {
				tgt = &state->particles[FROTH_FIRST_TARGET + i % args->tgtCnt];
				point->velX += (tgt->x - point->x) >> 9;
				point->velY += (tgt->y - point->y) >> 9;
				point->velZ += (tgt->z - point->z) >> 9;
				if(frm < i * 2 + 85) {
					SET_VELOCITY(point, point->velX * 60 / DAMPING_SCALE,
						point->velY * 60 / DAMPING_SCALE,
						point->velZ * 60 / DAMPING_SCALE);
				}
				/* On the ground it strikes. */
				if(point->y < 0) {
					landed[i] = 1;
					point->variant = 0;
					point->x = pos.x;
					point->y = pos.y + (syRandom() & 31) - 16;
					btEffectTintBattler(args->tgts[i % args->tgtCnt],
						SPRITE_COLOR_WHITE, POSE_DAMAGE, i % args->tgtCnt, 4);
					btMotionApplyKnockbackStub(
						args->tgts[i % args->tgtCnt], KNOCKBACK_NUDGE);
					state->shakeFrm = 4;
					sySndPlayStub(SE_WATER);
				}
			}

			/* The vortex where a bubble landed: three pictures, two ticks
			   each, drawn as mirrored halves. */
			if(point->variant >= 0 && point->variant < FROTH_VORTEX_TICKS) {
				draw[0](canvas,
					state->sheet +
						(point->variant / 2) % 3 *
							PICTURE_BYTES(FROTH_VORTEX_HALF),
					point->x - FROTH_VORTEX_HALF_WIDTH, point->y - 56,
					FROTH_VORTEX_HALF_WIDTH, FROTH_VORTEX_HALF_HEIGHT);
				draw[1](canvas,
					state->sheet +
						(point->variant / 2) % 3 *
							PICTURE_BYTES(FROTH_VORTEX_HALF),
					point->x, point->y - 56, FROTH_VORTEX_HALF_WIDTH,
					FROTH_VORTEX_HALF_HEIGHT);
				point->variant++;
			}
		}

		btCameraShake(args->variant * 2 + 2, args->variant * 2 + 2);
		if(state->cameraPhase == CAMERA_PHASE_IDLE)
			state->cameraPhase = CAMERA_PHASE_TICK_DONE;
		EFFECT_END_TICK(state);
	}

	/* Tear down. */
	syCloseEvent(btCameraApplyPhasedDelta);
	EFFECT_FINISH_CANVAS();
}

/* Tornadoes sweep across the targets, sparkling with glints, and the targets
   reel again and again.

   Abilities: Whirlwind, Tornado, Tempest, Gale, Typhoon, Hurricane. */
void btEffectRunWhirlwind(BATTLE_EFFECT_PARM *args)
{
	BLITTER_PAIR		 draw;
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	FIXED				 wave;
	PARTICLE			*glint;
	Sint32				 frm;
	Sint32				 x;
	Sint32				 y;
	Sint32				 segment;
	Sint32				 picture;
	Sint32				 i;
	Sint32				 j;

	/* Set up: the canvas layer, the tornado sprite sheet and two max blitters,
	   the second mirrored. */
	state = Ram_HeapSlots->battleEff;
	canvas = Ram_HeapSlots->battleCanvas;
	btEffectStartCanvasLayer(BGCNT_PRIORITY(1));
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	btResourceLoadAndDecompress(RESOURCE_TORNADO_SHEET, state->sheet, 1, 1);
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
	draw[0] = Ram_HeapSlots->ramCode;
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
		EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
	draw[1] = Ram_HeapSlots->ramCodePaired;

	/* The glints start at random places in their funnel, staggered in time. */
	for(j = 0; j != WHIRLWIND_GLINTS; j++) {
		glint = &state->particles[j];
		glint->x = syRandom() & 31;
		glint->y = (syRandom() & 63) + 16;
		glint->variant = -(syRandom() & 15);
	}
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
	REG_BLDALPHA = EFFECT_BLEND_CANVAS(0); /* invisible to start with */
	sySynch(1);
	sySndPlayStub(SE_RUMBLE);

	for(frm = 0; frm != WHIRLWIND_TICKS; frm++) {
		wave = sySin(frm * (TRIG_TURN / 64)) * WHIRLWIND_SWEEP_SWAY;
		if(frm == 32)
			EFFECT_SHOW_DAMAGE(SE_IMPACT);

		/* The canvas flashes every eight ticks from tick 16. */
		for(i = 0; i != 7; i++) {
			if(frm == i * 8 + 16)
				syWordsFillEntry(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(8));
		}

		/* The sweep sways from side to side over the targets; it fades in over
		   16 ticks and out over the last 16. */
		if(args->side == EFFECT_SIDE_PARTY)
			wave += toFIXED(32);
		else
			wave -= toFIXED(32);
		if(frm <= 16)
			REG_BLDALPHA = EFFECT_BLEND_CANVAS(frm);
		if(frm > 63)
			REG_BLDALPHA = EFFECT_BLEND_CANVAS(79 - frm);

		for(i = 0; i != VorticesShapes[args->variant][WHIRLWIND_TORNADOES];
			i++) {
			/* Each tornado circles its own phase, a quarter turn from the
			   next. */
			x = FIXEDtoINT(VorticesShapes[args->variant][WHIRLWIND_SWAY] *
						sySin(frm * (TRIG_TURN / 32) + i * DEGtoANG(90)) +
					wave) +
				40;
			y = FIXEDtoINT(
				syCos(frm * (TRIG_TURN / 32) + i * DEGtoANG(90)) * 2);
			segment = (frm / 2) % 3;
			draw[0](canvas,
				state->sheet + segment * (2 * WHIRLWIND_SECTION_BYTES) +
					WHIRLWIND_TORNADO_PICTURES,
				x, y + 16, TORNADO_STRIP_WIDTH, TORNADO_STRIP_HEIGHT);
			draw[0](canvas,
				state->sheet + segment * WHIRLWIND_SECTION_BYTES +
					(WHIRLWIND_TORNADO_PICTURES + 6 * WHIRLWIND_SECTION_BYTES),
				x, y + 48, TORNADO_STRIP_WIDTH, TORNADO_STRIP_HEIGHT);
			draw[0](canvas,
				state->sheet + segment * (2 * WHIRLWIND_SECTION_BYTES) +
					(WHIRLWIND_TORNADO_PICTURES + WHIRLWIND_SECTION_BYTES),
				x, y + 80, TORNADO_STRIP_WIDTH, TORNADO_STRIP_HEIGHT);

			/* Its four glints, the second of each pair mirrored; each restarts
			   at a new place when its pictures run out. */
			for(j = 0; j != 4; j++) {
				glint = &state->particles[i * 4 + j];
				if(glint->variant >= 0) {
					picture = glint->variant / 2 + (j / 2) * 3;
					draw[j & 1](canvas,
						state->sheet + BattleEffectGlintPictures[picture],
						glint->x + x, glint->y + y,
						BattleEffectGlintPictureWidths[picture],
						BattleEffectGlintPictureHeights[picture]);
				}
				glint->variant++;
				if(glint->variant == WHIRLWIND_GLINT_LIFE) {
					glint->x = syRandom() & 31;
					glint->y = (syRandom() & 63) + 16;
					glint->variant = 0;
				}
			}
		}

		/* Strike each target seven times, every eight ticks, three ticks
		   after the target before it. */
		EFFECT_FOR_EACH_TARGET(j, args) {
			for(i = 0; i != 7; i++) {
				if(frm == i * 8 + j * 3 + 16) {
					EFFECT_STRIKE_TARGET(args, j, 4, KNOCKBACK_BOUNCE);
				}
			}
		}

		state->shakeFrm = 1;
		btCameraShake(VorticesShapes[args->variant][WHIRLWIND_SHAKE],
			VorticesShapes[args->variant][WHIRLWIND_SHAKE] * 2);
		EFFECT_END_TICK(state);
	}

	/* Tear down. */
	EFFECT_RELEASE_BLITTERS();
	syCloseEvent(btEffectTransferCanvas);
	btEffectEndCanvasLayer();
}

#undef WHIRLWIND_TICKS
#undef FROTH_VORTEX_HALF_WIDTH
#undef FROTH_VORTEX_HALF_HEIGHT
#undef FROTH_BUBBLE_PICTURE_WIDTH
#undef FROTH_BUBBLE_PICTURE_HEIGHT
#undef FROTH_BUBBLE_PICTURE
#undef FROTH_BUBBLE_SLOTS
#undef FROTH_FIRST_TARGET
#undef FROTH_BUBBLE_DEPTH_NEAR
#undef FROTH_BUBBLE_DEPTH_FAR
#undef FROTH_VORTEX_TICKS
#undef WHIRLWIND_GLINTS
#undef WHIRLWIND_GLINT_LIFE
#undef WHIRLWIND_SWEEP_SWAY
#undef WHIRLWIND_TORNADO_PICTURES
#undef WHIRLWIND_SECTION_BYTES

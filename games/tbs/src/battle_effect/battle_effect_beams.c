/* The plain attack's slash, and the critical hit's gathering light and
   crescent. Part of battle_effect.c. */

#define BEAM_FRAMES				32 /* the slash's and the critical hit's length */
#define BEAM_SPARKS				64 /* sparks in state->particles, drawn in pairs */
/* The frames the first spark lives, shrinking as it goes; each later one lives
   a frame longer. */
#define BEAM_SPARK_LIFE			16
/* BG1's scroll that shows the canvas, 96 pixels down BG1's map
   (btEffectSetupCanvasTileMap), from the battle view's top line. */
#define BEAM_CANVAS_SCROLL_Y	(96 - BATTLE_VIEW_TOP)
/* The critical hit's camera zoom, in percent, held while it gathers. */
#define CRITICAL_HIT_ZOOM		130
/* The motes that close in on the attacker, and how far either side of the
   centre their depth is clamped. */
#define CRITICAL_HIT_MOTES		64
#define CRITICAL_HIT_MOTE_DEPTH 60

/* The plain attack's pictures, by effect kind: the attacker's attack effect. */
typedef enum BEAM_KIND {
	BEAM_PLAIN_ATTACK,	/* drawn as BEAM_ARC_A; no attack has kind 0 */
	BEAM_ARC_A,			/* one blue slash: Isaac's, Garet's and Jenna's */
	BEAM_ARC_B,			/* three blue slashes side by side */
	BEAM_IMAGE,			/* a streak of light, growing */
	BEAM_WAVE,			/* three short blue arcs, backwards: Ivan's and Mia's */
	BEAM_ARC_A_INVERTED /* one blue slash, upside down */
} BEAM_KIND;

/* A plain attack: a slash or streak sweeps from the user over its first target,
   a yellow orb flashes over the target and sparks fly off. */
void btEffectRunAttackSlash(BATTLE_EFFECT_PARM *args)
{
	SCREEN_POINT		 screenPosition;
	SCREEN_POINT		 anchor;
	SCREEN_POINT		 screen;
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	Sint32				 scrollX;
	Uint8				*sheet;
	BATTLE_CAMERA		*camera;
	Sint32				 kind;
	UNIT				*unit;
	BLITTER_PAIR		 draw;
	ACTOR				*target;
	Sint32				 half;
	Sint32				 frame;
	Sint32				 i;

	state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
	sheet = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
	camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
	kind = args->kind;
	state->args = args;
	unit = gmUnitGetStub(args->user);
	sySynch(1);
	btEffectConfigureEffectDisp();
	btEffectSetupCanvasTileMap();
	REG_BG1CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(0) | BGCNT_256_COLORS |
		BGCNT_SCREEN_BASE(31);
	sySynch(1);
	if(kind == BEAM_ARC_A_INVERTED) {
		if(state->args->side == EFFECT_SIDE_ENEMIES) {
			EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
				EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y, COMPOSITE_ADDITIVE);
			EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
				EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y, COMPOSITE_LIGHTEN);
		} else {
			EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
				EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X | COMPOSITE_FLIP_Y,
				COMPOSITE_ADDITIVE);
			EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
				EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X | COMPOSITE_FLIP_Y,
				COMPOSITE_LIGHTEN);
		}
	} else {
		if(state->args->side == EFFECT_SIDE_ENEMIES) {
			EFFECT_LOAD_BLITTER(
				HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, COMPOSITE_ADDITIVE);
			EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
				COMPOSITE_LIGHTEN);
		} else {
			EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
				EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_ADDITIVE);
			EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
				EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_LIGHTEN);
		}
	}
	{
		DRAW_RECTANGLE *blitters = EFFECT_BLITTERS;

		draw[0] = blitters[HEAP_SLOT_RAM_CODE];
		draw[1] = blitters[HEAP_SLOT_RAM_CODE_PAIRED];
	}
	sySynch(1);
	if(kind == BEAM_WAVE) {
		btResourceLoadAndDecompress(RESOURCE_WAVE_SHEET, state->sheet, 1, 1);
	} else if(kind == BEAM_IMAGE) {
		btResourceLoadAndDecompress(RESOURCE_BEAM_IMAGE, state->sheet, 0, 0);
	} else {
		switch(kind) {
		case BEAM_PLAIN_ATTACK:
		case BEAM_ARC_A:
		case BEAM_ARC_A_INVERTED:
			btResourceLoadAndDecompress(
				RESOURCE_BLUE_ARC_SHEET_A, state->sheet, 1, 1);
			break;
		case BEAM_ARC_B:
			btResourceLoadAndDecompress(
				RESOURCE_BLUE_ARC_SHEET_B, state->sheet, 1, 1);
			break;
		}
	}
	/* A monster's slash takes the Mars Djinni's colours, the party's the
	   crescent's. */
	if(state->args->user > PIERS)
		syWordsCopy(BG_PLTT,
			syResourceGetTableEntry(RESOURCE_MARS_DJINN_SMALL_SHEET),
			SHEET_PALETTE_BYTES);
	else
		syWordsCopy(BG_PLTT, syResourceGetTableEntry(RESOURCE_CRESCENT_SHEET),
			SHEET_PALETTE_BYTES);
	sySynch(1);
	btResourceLoadAndDecompress(RESOURCE_BLUE_GLOW_SHEET, sheet, 0, 0);
	btResourceLoadAndDecompress(
		RESOURCE_YELLOW_ORB_SHEET, Ram_MapGridMetatileBuffer, 1, 0);
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
	REG_BG1CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(0) | BGCNT_256_COLORS |
		BGCNT_SCREEN_BASE(31);
	/* Scroll BG1 so the first target falls on the canvas. */
	btProjectionSpr(state->args->tgts[0], &anchor);
	if(state->args->side == EFFECT_SIDE_ENEMIES)
		scrollX = 96 - anchor.x;
	else
		scrollX = 32 - anchor.x;
	if(scrollX > 0)
		scrollX = 0;
	if(scrollX < -128)
		scrollX = -128;
	anchor.x += scrollX;
	BgScrollWork[1].x = scrollX;
	BgScrollWork[1].y = BEAM_CANVAS_SCROLL_Y;
	sySynch(1);
	/* Sparks fly from the target, mostly rising. */
	target = btObjectGetStub(state->args->tgts[0])->actor;
	half = btUnitGetEffectHeightStub(state->args->tgts[0]) / 2;
	for(i = 0; i != BEAM_SPARKS; i++) {
		PARTICLE *spark = &state->particles[i];

		SET_POSITION(
			spark, target->x.fixed, target->y.fixed + half, target->z.fixed);
		SET_VELOCITY(spark, (syRandom() & 255) << 10,
			((syRandom() & 255) - 32) << 10, ((syRandom() & 255) - 127) << 10);
		if(spark->x > 0)
			spark->velX = -spark->velX;
		spark->velX = -spark->velX;
		spark->variant = i + BEAM_SPARK_LIFE;
	}

	for(frame = 0; frame != BEAM_FRAMES; frame++) {
		if(frame == 5) {
			if(btSprHasHeavyImpactStub(unit->id))
				EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
			else
				EFFECT_SHOW_DAMAGE(SE_IMPACT);
		}
		if(frame == 4)
			btMotionApplyKnockbackStub(state->args->tgts[0], KNOCKBACK_NUDGE);
		/* The slash or streak plays from the user. */
		btProjectionScaledUnit(state->args->user, &screenPosition);
		screenPosition.y += 16;
		if(kind == BEAM_WAVE) {
			if(frame <= 11) {
				if(state->args->side == EFFECT_SIDE_ENEMIES)
					draw[1](canvas,
						state->sheet + (5 - frame / 2) * PICTURE_BYTES(WAVE),
						screenPosition.x + scrollX - WAVE_WIDTH,
						screenPosition.y - WAVE_HEIGHT / 2, WAVE_WIDTH,
						WAVE_HEIGHT);
				else
					draw[1](canvas,
						state->sheet + (5 - frame / 2) * PICTURE_BYTES(WAVE),
						screenPosition.x + scrollX,
						screenPosition.y - WAVE_HEIGHT / 2, WAVE_WIDTH,
						WAVE_HEIGHT);
			}
		} else if((kind >= BEAM_PLAIN_ATTACK && kind <= BEAM_ARC_B) ||
			kind == BEAM_ARC_A_INVERTED) {
			if(frame <= 11) {
				if(state->args->side == EFFECT_SIDE_ENEMIES)
					draw[1](canvas,
						state->sheet + frame / 2 * PICTURE_BYTES(BLUE_ARC),
						screenPosition.x + scrollX - BLUE_ARC_WIDTH,
						screenPosition.y - 40, BLUE_ARC_WIDTH, BLUE_ARC_HEIGHT);
				else
					draw[1](canvas,
						state->sheet + frame / 2 * PICTURE_BYTES(BLUE_ARC),
						screenPosition.x + scrollX, screenPosition.y - 40,
						BLUE_ARC_WIDTH, BLUE_ARC_HEIGHT);
			}
		} else {
			if(frame <= 17) {
				Sint32 picture = frame / 3;

				if(state->args->side == EFFECT_SIDE_ENEMIES)
					draw[1](canvas, state->sheet + Beam_Pictures[picture],
						screenPosition.x + Beam_PictureReaches[picture] +
							scrollX - 58,
						screenPosition.y - Beam_PictureHeights[picture] / 2,
						Beam_PictureWidths[picture],
						Beam_PictureHeights[picture]);
				else
					draw[1](canvas, state->sheet + Beam_Pictures[picture],
						screenPosition.x - Beam_PictureReaches[picture] +
							scrollX - Beam_PictureWidths[picture] + 58,
						screenPosition.y - Beam_PictureHeights[picture] / 2,
						Beam_PictureWidths[picture],
						Beam_PictureHeights[picture]);
			}
		}
		/* The yellow orb flashes over the target. */
		if(frame >= 4 && frame <= 15)
			draw[0](canvas,
				Ram_MapGridMetatileBuffer +
					(frame - 4) / 2 * PICTURE_BYTES(YELLOW_ORB),
				anchor.x - 16, anchor.y - YELLOW_ORB_HEIGHT / 2,
				YELLOW_ORB_WIDTH, YELLOW_ORB_HEIGHT);
		EFFECT_VIEW_FROM_CAMERA(camera);
		/* The sparks fly. */
		if(frame >= 4 && frame <= 31) {
			for(i = 0; i != BEAM_SPARKS; i++) {
				Sint32	  pair = i / 2;
				PARTICLE *spark = &state->particles[pair];
				Sint32	  size = spark->variant;

				if(size > 0) {
					syConvert3Dto2D(
						&spark->x, syScreenPointCoordinates(&screen));
					screen.x += scrollX;
					size >>= 3;
					size += 2;
					EFFECT_DRAW_FLARE(draw[pair & 1], canvas, sheet, screen.x,
						screen.y, size);
					btParticleAdvanceWithGravity3D(spark, 60, -toFIXED(1) / 64);
					spark->variant--;
				}
			}
		}
		EFFECT_PRESENT_CANVAS(state);
	}

	syCloseEvent(btEffectTransferCanvas);
	EFFECT_RELEASE_BLITTERS();
	BgScrollWork[1].y = BATTLE_BACKGROUND_SCROLL_Y;
	btEffectHideBg1AndRestoreBackdrop();
}

/* A critical hit: motes of light close in on the attacker as it hangs in its
   leap, then a crescent slash sweeps across the first target and sparks fly
   from it. */
void btEffectRunCriticalHit(BATTLE_EFFECT_PARM *args)
{
	SCREEN_POINT		 screenPosition;
	SCREEN_POINT		 anchor;
	SCREEN_POINT		 screen;
	SCREEN_POINT		 spot;
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	Sint32				 scrollX;
	Uint8				*sheet;
	BATTLE_CAMERA		*camera;
	Sint32				 velocityX;
	Sint32				 velocityY;
	Sint32				 velocityZ;
	Sint32				 acceleration;
	Sint32				 gravity;
	BLITTER_PAIR		 draw;
	ACTOR				*actor;
	ACTOR				*target;
	BATTLE_STATE		*battle;
	Sint32				 gathers;
	Sint32				 half;
	Sint32				 frame;
	Sint32				 i;

	state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
	sheet = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
	camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
	/* DEAD: never true, as only kinds from EFFECT_RANGE_RING_AND_SPARK reach
	   this runner (btEffectRunByIdRange), so gathers stays 1; the ROM keeps the
	   compare and the store. */
	gathers = 1;
	if(args->kind < EFFECT_RANGE_RING_AND_SPARK)
		gathers = 0;
	state->args = args;
	btCameraSetViewAtPairMidpointStub(
		args->user, args->firstTgt, CRITICAL_HIT_ZOOM);
	sySynch(1);
	btEffectSetupCanvasTileMap();
	REG_BG1CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(0) | BGCNT_256_COLORS |
		BGCNT_SCREEN_BASE(31);
	if(state->args->side == EFFECT_SIDE_ENEMIES) {
		EFFECT_LOAD_BLITTER(
			HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, COMPOSITE_ADDITIVE);
		EFFECT_LOAD_BLITTER(
			HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS, COMPOSITE_LIGHTEN);
	} else {
		EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
			EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_ADDITIVE);
		EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
			EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_LIGHTEN);
	}
	{
		DRAW_RECTANGLE *blitters = EFFECT_BLITTERS;

		draw[0] = blitters[HEAP_SLOT_RAM_CODE];
		draw[1] = blitters[HEAP_SLOT_RAM_CODE_PAIRED];
	}
	btCameraSetViewAtPairMidpointStub(
		state->args->user, state->args->firstTgt, CRITICAL_HIT_ZOOM);
	sySynch(1);
	btResourceLoadAndDecompress(
		RESOURCE_STAR_BURST_SHEET, state->imageStorage, 1, 0);
	btCameraSetViewAtPairMidpointStub(
		state->args->user, state->args->firstTgt, CRITICAL_HIT_ZOOM);
	sySynch(1);
	btResourceLoadAndDecompress(
		RESOURCE_CRESCENT_SHEET, Ram_MapGridMetatileBuffer, 1, 1);
	/* A monster's crescent takes the Mars Djinni's colours. */
	if(state->args->user > PIERS)
		syWordsCopy(BG_PLTT,
			syResourceGetTableEntry(RESOURCE_MARS_DJINN_SMALL_SHEET),
			SHEET_PALETTE_BYTES);
	btCameraSetViewAtPairMidpointStub(
		state->args->user, state->args->firstTgt, CRITICAL_HIT_ZOOM);
	sySynch(1);
	btResourceLoadAndDecompress(RESOURCE_BLUE_GLOW_SHEET, sheet, 0, 0);
	btCameraSetViewAtPairMidpointStub(
		state->args->user, state->args->firstTgt, CRITICAL_HIT_ZOOM);
	sySynch(1);
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);

	/* The gathering. */
	if(gathers == 1) {
		actor = btObjectGetStub(state->args->user)->actor;
		for(i = 0; i != CRITICAL_HIT_MOTES; i++) {
			PARTICLE *mote = &state->particles[i];

			SET_POSITION(mote, (syRandom() & 63) + 16, 0, 0);
			SET_VELOCITY(mote, syRandom() & TRIG_ANGLE_MASK,
				syRandom() & TRIG_ANGLE_MASK, syRandom() & TRIG_ANGLE_MASK);
		}
		fdActorSetAnimationSpeedStub(actor, 0);
		velocityX = actor->velX;
		velocityY = actor->velY;
		velocityZ = actor->velZ;
		gravity = actor->gravity;
		acceleration = actor->acceleration;
		SET_VELOCITY(actor, 0, 0, 0);
		actor->acceleration = 0;
		actor->gravity = 0;
		btProjectionScaledUnit(state->args->user, &screenPosition);
		scrollX = 64 - screenPosition.x;
		BgScrollWork[1].x = scrollX;
		BgScrollWork[1].y = BEAM_CANVAS_SCROLL_Y;
		state->fadeFrm = BACKGROUND_DARKEN_FRAMES;
		state->fadeStep = 0;
		sySetEvent(btEffectDarkenBackgroundStep, EVENT_PASS_FRAME);
		sySndPlayStub(SE_POWER_UP);
		for(frame = 0; frame != BEAM_FRAMES; frame++) {
			btCameraSetViewAtPairMidpointStub(
				state->args->user, state->args->firstTgt, CRITICAL_HIT_ZOOM);
			for(i = 0; i != CRITICAL_HIT_MOTES; i++) {
				PARTICLE *mote = &state->particles[i];

				if(mote->x >= 0 && frame >= i / 4) {
					Sint32 size = (i & 1) + 5;

					syInitMatrix();
					syRotZ(mote->velZ);
					syRotX(mote->velX);
					syRotY(mote->velY);
					btProjectionPoint(&mote->x, &screen);
					screen.x += 64;
					screen.y = screen.y + screenPosition.y + 24;
					/* DEAD: the depth is clamped and moved to start at 0 but
					   never read; the ROM keeps the clamp and the move. */
					CLAMP(screen.depth, -CRITICAL_HIT_MOTE_DEPTH,
						CRITICAL_HIT_MOTE_DEPTH);
					screen.depth += CRITICAL_HIT_MOTE_DEPTH;
					EFFECT_DRAW_FLARE(
						draw[0], canvas, sheet, screen.x, screen.y, size);
					mote->x -= 4;
				}
			}
			EFFECT_PRESENT_CANVAS(state);
		}
		syCloseEvent(btEffectDarkenBackgroundStep);
		fdActorSetAnimationSpeedStub(actor, ANIM_STEP_NORMAL);
		SET_VELOCITY(actor, velocityX, velocityY, velocityZ);
		actor->acceleration = acceleration;
		actor->gravity = gravity;
	}

	/* The strike. */
	syWordsClear(canvas, CANVAS_BYTES);
	syWordsClear((void *)BG_CHAR_BLOCK(1), CANVAS_BYTES);
	EFFECT_SET_TRANSFER(
		state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
	REG_BG1CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(0) | BGCNT_256_COLORS |
		BGCNT_SCREEN_BASE(31);
	btProjectionScaledUnit(state->args->tgts[0], &anchor);
	if(state->args->side == EFFECT_SIDE_ENEMIES)
		scrollX = 32 - anchor.x;
	else
		scrollX = 96 - anchor.x;
	if(scrollX > 0)
		scrollX = 0;
	if(scrollX < -128)
		scrollX = -128;
	anchor.x += scrollX;
	BgScrollWork[1].y = BEAM_CANVAS_SCROLL_Y;
	BgScrollWork[1].x = scrollX;
	target = btObjectGetStub(state->args->tgts[0])->actor;
	half = btUnitGetEffectHeightStub(state->args->tgts[0]) / 2;
	for(i = 0; i != BEAM_SPARKS; i++) {
		PARTICLE *spark = &state->particles[i];

		SET_POSITION(
			spark, target->x.fixed, target->y.fixed + half, target->z.fixed);
		SET_VELOCITY(spark, (syRandom() & 255) << 10, (syRandom() & 255) << 10,
			((syRandom() & 255) - 127) << 10);
		if(spark->x > 0)
			spark->velX = -spark->velX;
		spark->variant = i + BEAM_SPARK_LIFE;
	}

	for(frame = 0; frame != BEAM_FRAMES; frame++) {
		if(frame == 5)
			EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
		if(frame == 4)
			btMotionApplyKnockbackStub(state->args->tgts[0], KNOCKBACK_NUDGE);
		btProjectionScaledUnit(state->args->user, &screenPosition);
		screenPosition.y += 16;
		/* The slash: a star, an arc, then two crescents. */
		if(frame <= 1)
			draw[0](canvas, state->imageStorage, 0, 0, SLASH_PICTURE_SIZE,
				SLASH_PICTURE_SIZE);
		else if(frame <= 3)
			draw[0](canvas,
				state->imageStorage + SLASH_PICTURE_SIZE * SLASH_PICTURE_SIZE,
				0, 0, SLASH_PICTURE_SIZE, SLASH_PICTURE_SIZE);
		else if(frame <= 5)
			draw[0](canvas, Ram_MapGridMetatileBuffer, 0, 0, SLASH_PICTURE_SIZE,
				SLASH_PICTURE_SIZE);
		else if(frame <= 7)
			draw[0](canvas,
				Ram_MapGridMetatileBuffer +
					SLASH_PICTURE_SIZE * SLASH_PICTURE_SIZE,
				0, 0, SLASH_PICTURE_SIZE, SLASH_PICTURE_SIZE);
		EFFECT_VIEW_FROM_CAMERA(camera);
		/* The sparks fly. */
		if(frame >= 4 && frame <= 31) {
			for(i = 0; i != BEAM_SPARKS; i++) {
				Sint32	  pair = i / 2;
				PARTICLE *spark = &state->particles[pair];
				Sint32	  size = spark->variant;

				if(size > 0) {
					btProjectionPoint(&spark->x, &spot);
					spot.x += scrollX;
					size >>= 3;
					size += 2;
					spot.y += 16;
					EFFECT_DRAW_FLARE(
						draw[pair & 1], canvas, sheet, spot.x, spot.y, size);
					btParticleAdvanceWithGravity3D(spark, 60, -toFIXED(1) / 64);
					spark->variant--;
				}
			}
		}
		EFFECT_PRESENT_CANVAS(state);
	}

	syCloseEvent(btEffectTransferCanvas);
	EFFECT_RELEASE_BLITTERS();
	BgScrollWork[1].y = BATTLE_BACKGROUND_SCROLL_Y;
	battle = BattleStateWork;
	for(frame = 0; frame != 7; frame++) {
		btBackgroundSetPaletteLevelStub(battle->bg, 6 - frame);
		sySynch(1);
	}
	btEffectHideBg1AndRestoreBackdrop();
}

#undef BEAM_FRAMES
#undef BEAM_SPARKS
#undef BEAM_SPARK_LIFE
#undef BEAM_CANVAS_SCROLL_Y
#undef CRITICAL_HIT_ZOOM
#undef CRITICAL_HIT_MOTES
#undef CRITICAL_HIT_MOTE_DEPTH

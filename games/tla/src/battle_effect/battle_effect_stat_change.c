/* Stat boosts and reductions (Impact, Guard, Ward, Dull, Weaken ...). Part of
   battle_effect.c. */

/* The stat boosts' modes (btEffectRunStatBoost), each named for the effect that
   plays it. */
typedef enum STAT_BOOST_MODE {
	STAT_BOOST_IMPACT,	/* a dagger, in Mars colours */
	STAT_BOOST_GUARD,	/* a shield, in Mars colours */
	STAT_BOOST_WARD,	/* a shield, in Jupiter colours */
	STAT_BOOST_GRANITE, /* a pane of light, in Venus colours */
	STAT_BOOST_ZEPHYR,	/* a gust of wind, keeping the palette */
	STAT_BOOST_FLASH,	/* a pane of light, in Mars colours */
	STAT_BOOST_IRON, /* a shield, in Venus colours, the backdrop turning grey */
	/* a shield, in Mercury colours; no ability's effect plays it */
	STAT_BOOST_UNUSED,
	STAT_BOOST_SHADE,		  /* a pane of light, in Mercury colours */
	STAT_BOOST_COAL,		  /* a gust of wind, in Mars colours */
	STAT_BOOST_CRUCIBLE_FIRE, /* Impact's burst without the swirling flares */
	/* a shield, in Mercury colours, the backdrop turning to Jupiter's */
	STAT_BOOST_STEAM,
	STAT_BOOST_GUARD_AURA /* a pane of light, in grey */
} STAT_BOOST_MODE;

/* Light swirls into each target and bursts, then a dagger appears in Mars
   colours.

   Abilities: Impact, High Impact, Demon Spear, Angel Spear; Forge; Kindle. */
void btEffectRunImpact(BATTLE_EFFECT_PARM *args)
{
	btEffectRunStatBoost(args, STAT_BOOST_IMPACT);
}

/* Light swirls into each target and bursts, then a shield appears in Mars
   colours.

   Abilities: Guard, Protect, Guardian, Protector; Corona. */
void btEffectRunGuard(BATTLE_EFFECT_PARM *args)
{
	btEffectRunStatBoost(args, STAT_BOOST_GUARD);
}

/* Light swirls into each target and bursts, then a shield appears in Venus
   colours as the backdrop turns grey.

   Abilities: Iron. */
void btEffectRunIron(BATTLE_EFFECT_PARM *args)
{
	btEffectRunStatBoost(args, STAT_BOOST_IRON);
}

/* Plays the stat boost's unused mode; no ability's effect names this one. */
void btEffectRunUnusedStatBoost(BATTLE_EFFECT_PARM *args)
{
	btEffectRunStatBoost(args, STAT_BOOST_UNUSED);
}

/* Light swirls into each target and bursts, then a shield appears in Jupiter
   colours.

   Abilities: Ward, Resist, Magic Shell, Magic Shield; Breeze. */
void btEffectRunWard(BATTLE_EFFECT_PARM *args)
{
	btEffectRunStatBoost(args, STAT_BOOST_WARD);
}

/* Light swirls into each target and bursts, then a shield appears in Mercury
   colours as the backdrop turns to Jupiter's colours.

   Abilities: Steam. */
void btEffectRunSteam(BATTLE_EFFECT_PARM *args)
{
	btEffectRunStatBoost(args, STAT_BOOST_STEAM);
}

/* Light swirls into each target and bursts, then a pane of light appears in
   Venus colours.

   Abilities: Granite. */
void btEffectRunGranite(BATTLE_EFFECT_PARM *args)
{
	btEffectRunStatBoost(args, STAT_BOOST_GRANITE);
}

/* Light swirls into each target and bursts, then a pane of light appears in
   Mars colours.

   Abilities: Flash. */
void btEffectRunFlash(BATTLE_EFFECT_PARM *args)
{
	btEffectRunStatBoost(args, STAT_BOOST_FLASH);
}

/* Light swirls into each target and bursts, then a pane of light appears in
   Mercury colours.

   Abilities: Shade. */
void btEffectRunShade(BATTLE_EFFECT_PARM *args)
{
	btEffectRunStatBoost(args, STAT_BOOST_SHADE);
}

/* Light swirls into each target and bursts, then a pane of light appears in
   grey.

   Abilities: Guard Aura. */
void btEffectRunGuardAura(BATTLE_EFFECT_PARM *args)
{
	btEffectRunStatBoost(args, STAT_BOOST_GUARD_AURA);
}

/* Light swirls into each target and bursts, then a gust of wind sweeps across
   it in the colours already loaded, as this is the one boost that keeps the
   palette.

   Abilities: Zephyr. */
void btEffectRunZephyr(BATTLE_EFFECT_PARM *args)
{
	btEffectRunStatBoost(args, STAT_BOOST_ZEPHYR);
}

/* Light swirls into each target and bursts, then a gust of wind sweeps across
   it in Mars colours.

   Abilities: Coal. */
void btEffectRunCoal(BATTLE_EFFECT_PARM *args)
{
	btEffectRunStatBoost(args, STAT_BOOST_COAL);
}

/* The stat boost's burst with no swirling flares (STAT_BOOST_FLARES), with no
   prelude Djinni and the fade step cleared.

   Abilities: Crucible Fire. */
void btEffectRunCrucibleFire(BATTLE_EFFECT_PARM *args)
{
	BATTLE_EFFECT_STATE *state = Ram_HeapSlots->battleEff;

	state->fadeStep = 0;
	args->prelude = 0;
	btEffectRunStatBoost(args, STAT_BOOST_CRUCIBLE_FIRE);
}

/* Where each picture sits in the sprite sheet. */
typedef enum STAT_BOOST_SHEET {
	STAT_BOOST_FLARES = 0,		/* flares (BlueGlowSheet) */
	STAT_BOOST_BURST = 1550,	/* six bursts of light (GlowOrb) */
	STAT_BOOST_PICTURE = 11150, /* the picture the mode picks */
	STAT_BOOST_DJINNI = 26048	/* the Djinni */
} STAT_BOOST_SHEET;

/* When the first target's flares burst and its sprite appears, with the damage;
   each later target follows STAT_BOOST_TARGET_DELAY ticks behind. Crucible
   Fire starts at STAT_BOOST_CRUCIBLE_FIRE_TICK, after the flares. */
#define STAT_BOOST_BURST_TICK		  52
#define STAT_BOOST_SPRITE_TICK		  80
#define STAT_BOOST_TARGET_DELAY		  8
#define STAT_BOOST_CRUCIBLE_FIRE_TICK (STAT_BOOST_BURST_TICK + 1)

/* The particles both kinds of stat change set out: the first half of the
   pool. */
#define STAT_CHANGE_PARTICLES		(PARTICLE_POOL_COUNT / 2)
/* Each target swirls in STAT_BOOST_TARGET_FLARES flares from its own run of
   STAT_BOOST_TARGET_PARTICLES particles. */
#define STAT_BOOST_TARGET_PARTICLES 64
#define STAT_BOOST_TARGET_FLARES	32
/* How far the flares turn about the target each tick. */
#define STAT_BOOST_FLARE_SPIN		(TRIG_TURN / 64)
/* A flare is drawn at size STAT_BOOST_FLARE_SIZE_MAX at depth
   STAT_BOOST_FLARE_DEPTH_NEAR or nearer, one size smaller every
   STAT_BOOST_FLARE_DEPTH_STEP farther, and at size 1 from
   STAT_BOOST_FLARE_DEPTH_FAR on. */
#define STAT_BOOST_FLARE_SIZE_MAX	6
#define STAT_BOOST_FLARE_DEPTH_NEAR 314
#define STAT_BOOST_FLARE_DEPTH_STEP 64
#define STAT_BOOST_FLARE_DEPTH_FAR \
	(STAT_BOOST_FLARE_DEPTH_NEAR + \
		(STAT_BOOST_FLARE_SIZE_MAX - 1) * STAT_BOOST_FLARE_DEPTH_STEP)
/* How far the prelude's Djinni sways to either side, in pixels. */
#define STAT_BOOST_DJINNI_SWAY	  20
/* The shift that sets the canvas, shown 1.25 times as wide, against the
   screen's right edge over the party. */
#define STAT_BOOST_PARTY_CANVAS_X (DISPLAY_WIDTH * 4 / 5 - CANVAS_SIZE)

/* Flares of light swirl in to the centre of each target in turn and burst, and
   the mode's picture plays over it. */
void btEffectRunStatBoost(BATTLE_EFFECT_PARM *args, Uint32 mode)
{
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	DRAW_RECTANGLE		 draw[2];
	Sint32				 member;
	Sint32				 canvasShiftX;
	Sint32				 drawFlg;
	Sint32				 start;
	Sint32				 additive = BLITTER_ADDITIVE;
	/* FAKEMATCH(temporary): 0 held in a local for the center's coordinates; as
	 * 0. */
	Sint32		 zero = 0;
	Sint32		 flipX = COMPOSITE_FLIP_X;
	Sint32		 frm;
	Sint32		 i;
	Sint32		 origin[3];
	Sint32		 tgt[3];
	SCREEN_POINT base;
	Sint32		 center[3];
	SCREEN_POINT screen;
	Sint32		 point[3];

	state = Ram_HeapSlots->battleEff;
	canvas = Ram_HeapSlots->battleCanvas;
	start = 0;
	btEffectStartCanvasLayer(0);

	/* The prelude: the Djinni appears on the far side; origin receives
	   where. */
	if(args->prelude == 1) {
		if(mode == STAT_BOOST_GRANITE || mode == STAT_BOOST_IRON)
			btEffectRunDjinniEntrance(args,
				DJINNI_ENTRANCE(
					ELEMENT_VENUS, args->side ^ 1, DJINNI_ANCHOR_TARGET_X),
				origin, tgt);
		else if(mode == STAT_BOOST_UNUSED || mode == STAT_BOOST_SHADE ||
			mode == STAT_BOOST_STEAM)
			btEffectRunDjinniEntrance(args,
				DJINNI_ENTRANCE(
					ELEMENT_MERCURY, args->side ^ 1, DJINNI_ANCHOR_TARGET_X),
				origin, tgt);
		else if(mode == STAT_BOOST_WARD || mode == STAT_BOOST_ZEPHYR)
			btEffectRunDjinniEntrance(args,
				DJINNI_ENTRANCE(
					ELEMENT_JUPITER, args->side ^ 1, DJINNI_ANCHOR_TARGET_X),
				origin, tgt);
		else if(mode != STAT_BOOST_CRUCIBLE_FIRE)
			btEffectRunDjinniEntrance(args,
				DJINNI_ENTRANCE(
					ELEMENT_MARS, args->side ^ 1, DJINNI_ANCHOR_TARGET_X),
				origin, tgt);
		origin[0] = origin[0] * 4 / 5;
	}

	/* Crucible Fire is Impact joined after the flares, with a fade. */
	if(mode == STAT_BOOST_CRUCIBLE_FIRE) {
		state->fadeFrm = BACKGROUND_DARKEN_TICKS;
		mode = STAT_BOOST_IMPACT;
		start = STAT_BOOST_CRUCIBLE_FIRE_TICK;
	}
	REG_BG2PA = Q_8_8(0.8); /* 1.25 times the width */

	/* The pictures and the colours. */
	btResourceLoadAndDecompress(
		RESOURCE_BLUE_GLOW_SHEET, state->sheet + STAT_BOOST_FLARES, 0, 0);
	btResourceLoadAndDecompress(
		RESOURCE_GLOW_ORB_SHEET, state->sheet + STAT_BOOST_BURST, 1, 1);
	if(mode == STAT_BOOST_GRANITE || mode == STAT_BOOST_FLASH ||
		mode == STAT_BOOST_SHADE || mode == STAT_BOOST_GUARD_AURA) {
		btResourceLoadAndDecompress(RESOURCE_VIOLET_STREAK_SHEET,
			state->sheet + STAT_BOOST_PICTURE, 1, 1);
		if(mode == STAT_BOOST_GRANITE) {
			LOAD_BG_PALETTE(RESOURCE_VENUS_DJINN_SHEET);
		} else if(mode == STAT_BOOST_FLASH) {
			LOAD_BG_PALETTE(RESOURCE_MARS_DJINN_SHEET);
		} else {
			LOAD_BG_PALETTE(RESOURCE_MERCURY_DJINN_SHEET);
		}
	} else if(mode == STAT_BOOST_ZEPHYR) {
		btResourceLoadAndDecompress(
			RESOURCE_WISP_SHEET, state->sheet + STAT_BOOST_PICTURE, 1, 1);
	} else if(mode == STAT_BOOST_COAL) {
		btResourceLoadAndDecompress(
			RESOURCE_WISP_SHEET, state->sheet + STAT_BOOST_PICTURE, 1, 0);
		LOAD_BG_PALETTE(RESOURCE_MARS_DJINN_SHEET);
	} else {
		void *palette;

		if(mode == STAT_BOOST_IMPACT)
			btResourceLoadAndDecompress(
				RESOURCE_DAGGER_SHEET, state->sheet + STAT_BOOST_PICTURE, 1, 0);
		else
			btResourceLoadAndDecompress(
				RESOURCE_SHIELD_SHEET, state->sheet + STAT_BOOST_PICTURE, 1, 0);
		if(mode == STAT_BOOST_IMPACT)
			palette = syResourceGetTableEntry(RESOURCE_MARS_DJINN_SHEET);
		else if(mode == STAT_BOOST_WARD || mode == STAT_BOOST_ZEPHYR)
			palette = syResourceGetTableEntry(RESOURCE_JUPITER_DJINN_SHEET);
		else if(mode == STAT_BOOST_IRON)
			palette = syResourceGetTableEntry(RESOURCE_VENUS_DJINN_SHEET);
		else if(mode == STAT_BOOST_UNUSED || mode == STAT_BOOST_STEAM)
			palette = syResourceGetTableEntry(RESOURCE_MERCURY_DJINN_SHEET);
		else if(mode == STAT_BOOST_GUARD)
			palette = syResourceGetTableEntry(RESOURCE_MARS_DJINN_SHEET);
		else
			palette = syResourceGetTableEntry(RESOURCE_PINK_BURST_SHEET);
		syWordsCopy((void *)BG_PLTT, palette, SHEET_PALETTE_BYTES);
	}
	if(mode == STAT_BOOST_GRANITE || mode == STAT_BOOST_IRON)
		btResourceLoadAndDecompress(
			RESOURCE_VENUS_DJINN_SHEET, state->sheet + STAT_BOOST_DJINNI, 1, 0);
	else if(mode == STAT_BOOST_GUARD_AURA)
		btGraphicsLoadGreyRamp();
	else if(mode == STAT_BOOST_WARD || mode == STAT_BOOST_ZEPHYR)
		btResourceLoadAndDecompress(RESOURCE_JUPITER_DJINN_SHEET,
			state->sheet + STAT_BOOST_DJINNI, 1, 0);
	else if(mode == STAT_BOOST_UNUSED || mode == STAT_BOOST_SHADE ||
		mode == STAT_BOOST_STEAM)
		btResourceLoadAndDecompress(RESOURCE_MERCURY_DJINN_SHEET,
			state->sheet + STAT_BOOST_DJINNI, 1, 0);
	else
		btResourceLoadAndDecompress(
			RESOURCE_MARS_DJINN_SHEET, state->sheet + STAT_BOOST_DJINNI, 1, 0);

	/* The flares: 25 pixels across and deep, 50 up and down. */
	for(i = 0; i != STAT_CHANGE_PARTICLES; i++) {
		PARTICLE *flare = &PARTICLES[i];

		SET_POSITION(flare, (syRandom() % 200 - 100) << 14,
			(syRandom() % 200 - 100) << 15, (syRandom() % 200 - 100) << 14);
		flare->variant = 0;
	}

	/* A lone target is moved to the canvas's middle. */
	if(args->tgtCnt == 1) {
		btEffectPosProjectScaledBattler(args->tgts[0], &base);
		canvasShiftX = -base.x * 4 / 5 + CANVAS_SIZE / 2;
	} else {
		if(args->side == EFFECT_SIDE_PARTY)
			canvasShiftX = -STAT_BOOST_PARTY_CANVAS_X;
		else
			canvasShiftX = 0;
	}
	REG_BG2X = canvasShiftX << 8;
	state->transferMode = CANVAS_COPY_AND_FADE;
	state->transferValue = CANVAS_FADE_HALF;
	sySetEvent(btEffectTransferCanvas, EVENT_PASS_VBLANK);
	if(args->side == EFFECT_SIDE_PARTY)
		drawFlg = EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X;
	else
		drawFlg = EFFECT_CLIP_BOUNDS;

	for(frm = start; frm !=
		args->tgtCnt * STAT_BOOST_TARGET_DELAY + STAT_BOOST_SPRITE_TICK + 28;
		frm++) {
		BATTLE_CAMERA *camera = Ram_HeapSlots->cameraState;
		Sint32		   prelude;

		if(frm == 0)
			sySndPlayStub(SE_SWIRL);
		if(mode == STAT_BOOST_IRON && (frm & 3) == 0)
			btEffectStepPaletteToResource(RESOURCE_GREY_PALETTE_A);
		if(mode == STAT_BOOST_STEAM && (frm & 3) == 0)
			btEffectStepPaletteToResource(RESOURCE_JUPITER_DJINN_SHEET);
		if(frm == STAT_BOOST_SPRITE_TICK)
			EFFECT_SHOW_DAMAGE(0);

		/* The prelude's Djinni sways, and from tick 32 rises two pixels a
		   tick; for its first four ticks it is drawn added as well. */
		prelude = args->prelude;
		if(prelude == 1) {
			Sint32 x = (FIXEDtoINT(sySin(frm * (TRIG_TURN / 32)) *
						   STAT_BOOST_DJINNI_SWAY)) +
				origin[0] + canvasShiftX - DJINNI_WIDTH / 2;
			Sint32 y = (FIXEDtoINT(syCos(frm * (TRIG_TURN / 32)) * 4)) +
				origin[1] - 24;

			EFFECT_LOAD_BLITTER(
				HEAP_SLOT_RAM_CODE, drawFlg ^ flipX, BLITTER_MAX);
			draw[0] = Ram_HeapSlots->ramCode;
			EFFECT_LOAD_BLITTER(
				HEAP_SLOT_RAM_CODE_PAIRED, drawFlg ^ flipX, additive);
			draw[1] = Ram_HeapSlots->ramCodePaired;

			if(frm > 32)
				y = y - frm * 2 + 64;
			draw[0](canvas, state->sheet + STAT_BOOST_DJINNI, x, y,
				DJINNI_WIDTH, DJINNI_HEIGHT);
			if(frm <= 3)
				draw[1](canvas, state->sheet + STAT_BOOST_DJINNI, x, y,
					DJINNI_WIDTH, DJINNI_HEIGHT);
			EFFECT_RELEASE_BLITTERS();
		}

		for(member = 0; member != args->tgtCnt; member++) {
			MOTION_OBJECT *object = btObjectGetStub(args->tgts[member])->obj;
			Sint32		   height =
				btUnitGetEffectHeightStub(args->tgts[member]) * 2 / 3;

			if(frm == member * STAT_BOOST_TARGET_DELAY + STAT_BOOST_SPRITE_TICK)
				sySndPlayStub(SE_POWER_UP);
			/* The flares turn about two thirds of the target's height. */
			EFFECT_VIEW_FROM_CAMERA(camera);
			SET_VECTOR(point, object->x, height, object->z);
			syMatrixTranslateVector(point);
			if(start != 0) {
				if(frm ==
					member * STAT_BOOST_TARGET_DELAY +
						STAT_BOOST_CRUCIBLE_FIRE_TICK)
					btEffectTintBattler(args->tgts[member], SPRITE_COLOR_WHITE,
						TINT_KEEP, member, 16);
			} else {
				if(frm == member * STAT_BOOST_TARGET_DELAY + 48)
					btEffectTintBattler(args->tgts[member], SPRITE_COLOR_WHITE,
						TINT_KEEP, member, 16);

				/* Flares turn and close in on the centre. */
				if(frm > member * STAT_BOOST_TARGET_DELAY) {
					EFFECT_LOAD_BLITTER(
						HEAP_SLOT_RAM_CODE, drawFlg, BLITTER_MAX);
					draw[0] = Ram_HeapSlots->ramCode;
					EFFECT_LOAD_BLITTER(
						HEAP_SLOT_RAM_CODE_PAIRED, drawFlg, additive);
					{
						/* FAKEMATCH(dead): slot set to NULL first, never
						 * read. */
						DRAW_RECTANGLE *slot = NULL;

						slot = &Ram_HeapSlots->ramCodePaired;
						draw[1] = *slot;
					}
					if(mode == STAT_BOOST_IMPACT) {
						syRotX(-frm * STAT_BOOST_FLARE_SPIN);
					} else if(mode == STAT_BOOST_GUARD) {
					} else if(mode == STAT_BOOST_WARD) {
						syRotY(frm * STAT_BOOST_FLARE_SPIN);
					} else {
						syRotY(frm * STAT_BOOST_FLARE_SPIN);
						syRotZ(frm * STAT_BOOST_FLARE_SPIN);
					}
					for(i = 0; i != STAT_BOOST_TARGET_FLARES; i++) {
						PARTICLE *flare =
							&PARTICLES[member * STAT_BOOST_TARGET_PARTICLES +
								i];

						if(frm > member * STAT_BOOST_TARGET_DELAY + i) {
							Sint32 xx = (flare->x >> 8) * (flare->x >> 8);
							Sint32 yy = (flare->y >> 8) * (flare->y >> 8);
							Sint32 zz = (flare->z >> 8) * (flare->z >> 8);
							Sint32 distance = sySquartIWRAM(xx + yy + zz) >> 9;

							if(distance != 0) {
								Sint32 size;

								btEffectPosProjectPoint(
									(Sint32 *)flare, &screen);
								screen.x = screen.x * 4 / 5 + canvasShiftX;
								CLAMP(screen.depth, STAT_BOOST_FLARE_DEPTH_NEAR,
									STAT_BOOST_FLARE_DEPTH_FAR);
								size = STAT_BOOST_FLARE_SIZE_MAX -
									(screen.depth -
										STAT_BOOST_FLARE_DEPTH_NEAR) /
										STAT_BOOST_FLARE_DEPTH_STEP;
								EFFECT_DRAW_FLARE(draw[1], canvas, state->sheet,
									screen.x, screen.y, size);
								flare->x -= flare->x / distance;
								flare->y -= flare->y / distance;
								flare->z -= flare->z / distance;
							}
						}
					}
					EFFECT_RELEASE_BLITTERS();
				}
			}

			SET_VECTOR(center, zero, zero, zero);
			btEffectPosProjectPoint(center, &screen);
			screen.x = screen.x * 4 / 5 + canvasShiftX;

			/* The burst: six pictures four ticks apart. */
			if(frm >=
					member * STAT_BOOST_TARGET_DELAY + STAT_BOOST_BURST_TICK &&
				frm < member * STAT_BOOST_TARGET_DELAY + STAT_BOOST_BURST_TICK +
						24) {
				Sint32 picture;

				picture = (frm - member * STAT_BOOST_TARGET_DELAY -
							  STAT_BOOST_BURST_TICK) /
					4 % 6;

				EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, drawFlg, BLITTER_MAX);
				draw[0] = Ram_HeapSlots->ramCode;
				draw[0](canvas,
					state->sheet + picture * PICTURE_BYTES(GLOW_ORB) +
						STAT_BOOST_BURST,
					screen.x - GLOW_ORB_WIDTH / 2,
					screen.y - GLOW_ORB_HEIGHT / 2, GLOW_ORB_WIDTH,
					GLOW_ORB_HEIGHT);
				syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
			}

			/* The mode's picture. */
			if(mode == STAT_BOOST_IMPACT) {
				/* Seven 24x40 pictures, four ticks apart. */
				if(frm >= member * STAT_BOOST_TARGET_DELAY +
							STAT_BOOST_SPRITE_TICK &&
					frm < member * STAT_BOOST_TARGET_DELAY +
							STAT_BOOST_SPRITE_TICK + 28) {
					Sint32 picture = (frm - member * STAT_BOOST_TARGET_DELAY -
										 STAT_BOOST_SPRITE_TICK) /
						4 % 7;

					EFFECT_LOAD_BLITTER(
						HEAP_SLOT_RAM_CODE, drawFlg, BLITTER_MAX);
					draw[0] = Ram_HeapSlots->ramCode;
					draw[0](canvas,
						state->sheet + picture * PICTURE_BYTES(DAGGER) +
							STAT_BOOST_PICTURE,
						screen.x - DAGGER_WIDTH / 2,
						screen.y - DAGGER_HEIGHT / 2, DAGGER_WIDTH,
						DAGGER_HEIGHT);
					syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
				}
			} else if(mode == STAT_BOOST_GRANITE || mode == STAT_BOOST_FLASH ||
				mode == STAT_BOOST_SHADE || mode == STAT_BOOST_GUARD_AURA) {
				/* Six 32x64 pictures, four ticks apart. */
				if(frm >= member * STAT_BOOST_TARGET_DELAY +
							STAT_BOOST_SPRITE_TICK &&
					frm < member * STAT_BOOST_TARGET_DELAY +
							STAT_BOOST_SPRITE_TICK + 24) {
					Sint32 picture = (frm - member * STAT_BOOST_TARGET_DELAY -
										 STAT_BOOST_SPRITE_TICK) /
						4 % 6;

					EFFECT_LOAD_BLITTER(
						HEAP_SLOT_RAM_CODE, drawFlg, BLITTER_MAX);
					draw[0] = Ram_HeapSlots->ramCode;
					draw[0](canvas,
						state->sheet + picture * PICTURE_BYTES(VIOLET_STREAK) +
							STAT_BOOST_PICTURE,
						screen.x - VIOLET_STREAK_WIDTH / 2,
						screen.y - VIOLET_STREAK_HEIGHT / 2,
						VIOLET_STREAK_WIDTH, VIOLET_STREAK_HEIGHT);
					syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
				}
			} else if(mode == STAT_BOOST_ZEPHYR || mode == STAT_BOOST_COAL) {
				/* Six 64x32 pictures two ticks apart, mirrored below into
				   64x64. */
				if(frm >= member * STAT_BOOST_TARGET_DELAY +
							STAT_BOOST_SPRITE_TICK &&
					frm < member * STAT_BOOST_TARGET_DELAY +
							STAT_BOOST_SPRITE_TICK + 24) {
					Sint32 picture = (frm - member * STAT_BOOST_TARGET_DELAY -
										 STAT_BOOST_SPRITE_TICK) /
						2 % 6;
					/* drawn as its upper and mirrored lower half */
					Uint8 *wisp;
					Sint32 lowerFlg; /* the lower half drawn flipped */

					EFFECT_LOAD_BLITTER(
						HEAP_SLOT_RAM_CODE, drawFlg, BLITTER_MAX);
					lowerFlg = drawFlg | COMPOSITE_FLIP_Y;
					draw[0] = Ram_HeapSlots->ramCode;
					/* Built for the 128 by 128 canvas, 7 the shift of each
					   side. */
					btEffectBuildBlitter(
						HEAP_SLOT_RAM_CODE_PAIRED, 7, 7, lowerFlg, BLITTER_MAX);
					wisp = state->sheet + picture * PICTURE_BYTES(WISP) +
						STAT_BOOST_PICTURE;
					draw[1] = Ram_HeapSlots->ramCodePaired;
					draw[0](canvas, wisp, screen.x - WISP_WIDTH / 2,
						screen.y - 24, WISP_WIDTH, WISP_HEIGHT);
					draw[1](canvas, wisp, screen.x - WISP_WIDTH / 2,
						screen.y + 8, WISP_WIDTH, WISP_HEIGHT);
					EFFECT_RELEASE_BLITTERS();
				}
			} else {
				/* Six 40x40 pictures four ticks apart, added to the canvas. */
				if(frm >= member * STAT_BOOST_TARGET_DELAY +
							STAT_BOOST_SPRITE_TICK &&
					frm < member * STAT_BOOST_TARGET_DELAY +
							STAT_BOOST_SPRITE_TICK + 24) {
					Sint32 picture = (frm - member * STAT_BOOST_TARGET_DELAY -
										 STAT_BOOST_SPRITE_TICK) /
						4 % 6;

					EFFECT_LOAD_BLITTER(
						HEAP_SLOT_RAM_CODE, drawFlg, BLITTER_ADDITIVE);
					draw[0] = Ram_HeapSlots->ramCode;
					draw[0](canvas,
						state->sheet + picture * PICTURE_BYTES(SHIELD) +
							STAT_BOOST_PICTURE,
						screen.x - SHIELD_WIDTH / 2,
						screen.y - SHIELD_HEIGHT / 2, SHIELD_WIDTH,
						SHIELD_HEIGHT);
					syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
				}
			}
		}
		EFFECT_END_TICK(state);
	}

	syCloseEvent(btEffectTransferCanvas);
	btEffectEndCanvasLayer();
}

/* The stat reductions' modes (btEffectRunStatReduction). */
typedef enum STAT_REDUCTION_MODE {
	STAT_REDUCTION_UNUSED, /* a dagger, in the pink burst's colours; unused */
	STAT_REDUCTION_IMPAIR, /* a shield, in the glow orb's colours */
	STAT_REDUCTION_WEAKEN, /* a shield, in the pink burst's colours */
	STAT_REDUCTION_DULL	   /* a dagger, in the glow orb's colours */
} STAT_REDUCTION_MODE;

/* Breaks the dagger sprite into fading shards as Dull does, in the pink burst's
   colours; no effect table entry reaches it. */
UNUSED void btEffectRunUnusedStatReduction(void *action)
{
	btEffectRunStatReduction(action, STAT_REDUCTION_UNUSED);
}

/* A dagger appears over each target and breaks into shards that fade through
   darker copies, in the glow orb's colours.

   Abilities: Dull, Blunt. */
void btEffectRunDull(BATTLE_EFFECT_PARM *args)
{
	btEffectRunStatReduction(args, STAT_REDUCTION_DULL);
}

/* A shield appears over each target and breaks into shards that fade through
   darker copies, in the glow orb's colours.

   Abilities: Impair, Debilitate. */
void btEffectRunImpair(BATTLE_EFFECT_PARM *args)
{
	btEffectRunStatReduction(args, STAT_REDUCTION_IMPAIR);
}

/* A shield appears over each target and breaks into fading shards, in the pink
   burst's colours.

   Abilities: Weaken, Enfeeble. */
void btEffectRunWeaken(BATTLE_EFFECT_PARM *args)
{
	btEffectRunStatReduction(args, STAT_REDUCTION_WEAKEN);
}

/* Each target throws STAT_REDUCTION_TARGET_SHARDS shards from its own run of
   STAT_REDUCTION_TARGET_PARTICLES particles, STAT_REDUCTION_TARGET_DELAY ticks
   after the target before it. */
#define STAT_REDUCTION_TARGET_PARTICLES 32
#define STAT_REDUCTION_TARGET_SHARDS	24
#define STAT_REDUCTION_TARGET_DELAY		4

/* A dagger (Dull) or a shield (Impair and Weaken) appears over each target and
   breaks into shards that fly off and fade. */
void btEffectRunStatReduction(BATTLE_EFFECT_PARM *args, Sint32 mode)
{
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	Sint32				 frm;
	Sint32				 j;
	Sint32				 xOfs;
	Sint32				 origin[3];
	SCREEN_POINT		 screen;
	FIXED				 point[3];
	DRAW_RECTANGLE		 draw[2];
	Sint32				 i;
	void				*palette;

	state = Ram_HeapSlots->battleEff;
	canvas = Ram_HeapSlots->battleCanvas;
	EFFECT_START_NARROW_CANVAS(0);
	if(mode == STAT_REDUCTION_UNUSED || mode == STAT_REDUCTION_DULL)
		btResourceLoadAndDecompress(RESOURCE_DAGGER_SHEET, state->sheet, 1, 1);
	else
		btResourceLoadAndDecompress(RESOURCE_SHIELD_SHEET, state->sheet, 1, 1);
	if(mode == STAT_REDUCTION_UNUSED)
		palette = syResourceGetTableEntry(RESOURCE_PINK_BURST_SHEET);
	else if(mode == STAT_REDUCTION_DULL)
		palette = syResourceGetTableEntry(RESOURCE_GLOW_ORB_SHEET);
	else if(mode == STAT_REDUCTION_IMPAIR)
		palette = syResourceGetTableEntry(RESOURCE_GLOW_ORB_SHEET);
	else
		palette = syResourceGetTableEntry(RESOURCE_PINK_BURST_SHEET);
	syWordsCopyEntry((void *)BG_PLTT, palette, SHEET_PALETTE_BYTES);
	btResourceLoadAndDecompress(RESOURCE_PALETTE_RAMP_IMAGE,
		state->sheet + STAT_REDUCTION_SHARD_PICTURES, 0, 0);

	for(i = 1; i != DIMMED_COPIES; i++) {
		for(j = 0; j != STAT_REDUCTION_SHARD_BYTES; j++) {
			Sint32 level = state->sheet[STAT_REDUCTION_SHARD_PICTURES + j];

			EFFECT_CAP_LEVEL(level, DIMMED_COPY_CAP(i));
			state->sheet[STAT_REDUCTION_SHARD_PICTURES +
				i * STAT_REDUCTION_SHARD_BYTES + j] = level;
		}
	}

	if(args->side == EFFECT_SIDE_PARTY) {
		REG_BG2X = -(NARROW_CANVAS_PARTY_X << 8);
		xOfs = -NARROW_CANVAS_PARTY_X;
	} else {
		REG_BG2X = 0;
		xOfs = 0;
	}

	/* The shards start in a grid where the sprite was, in rows 6 pixels apart
	   up from 10 below its centre, four to a row 2 pixels apart from 2 to one
	   side (dagger) or 8 apart from 16 (shield), and fly away from the user's
	   side, scattering and rising. */
	for(i = 0; i != STAT_CHANGE_PARTICLES; i++) {
		PARTICLE *shard = &PARTICLES[i];
		Sint32	  radius = 192;
		Sint32	  ang;

		ang = syRandom() & TRIG_ANGLE_MASK;
		shard->x = 0;
		if(mode == STAT_REDUCTION_UNUSED || mode == STAT_REDUCTION_DULL) {
			shard->y = toFIXED(
				(i & (STAT_REDUCTION_TARGET_PARTICLES - 1)) / 4 * 6 - 10);
			shard->z = toFIXED(i % 4 * 2 - 2);
		} else {
			shard->y = toFIXED(
				(i & (STAT_REDUCTION_TARGET_PARTICLES - 1)) / 4 * 6 - 10);
			shard->z = toFIXED(i % 4 * 8 - 16);
		}
		if(args->side == EFFECT_SIDE_PARTY)
			shard->velX = toFIXED(2);
		else
			shard->velX = -toFIXED(2);
		shard->velY = ((syCos(ang) * radius) >> 6) + toFIXED(1);
		shard->velZ = (sySin(ang) * radius) >> 6;
		shard->variant = syRandom() & 0xff;
	}

	btEffectBuildCanvasBlitters(args->side, draw);
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

	for(frm = 0; frm != args->tgtCnt * STAT_REDUCTION_TARGET_DELAY + 64;
		frm++) {
		BATTLE_CAMERA *camera = Ram_HeapSlots->cameraState;

		/* The loop runs count * STAT_REDUCTION_TARGET_DELAY + 64 ticks, so
		   tick 72 comes only with three or more targets. With one or two,
		   btPlaybackWaitForReady starts playback once the effect returns, so
		   the messages follow the shards instead of overlapping them. */
		if(frm == 72)
			EFFECT_SHOW_DAMAGE(0);
		for(j = 0; j != args->tgtCnt; j++) {
			MOTION_OBJECT *object;
			Sint32		   tgtFrm;
			Sint32		   x;
			Sint32		   y;

			object = btObjectGetStub(args->tgts[j])->obj;
			tgtFrm = frm - j * STAT_REDUCTION_TARGET_DELAY;
			if(tgtFrm > 0) {
				/* DEAD: the view set from the camera a first time, which the
				   second call repeats; the ROM keeps both calls. */
				EFFECT_VIEW_FROM_CAMERA(camera);
				SET_VECTOR(point, object->x, toFIXED(20), object->z);
				EFFECT_VIEW_FROM_CAMERA(camera);
				syMatrixTranslateVector(point);
				SET_VECTOR(origin, 0, 0, 0);
				btEffectPosProjectPoint(origin, &screen);
				x = screen.x + xOfs;
				y = screen.y;
				if(mode == STAT_REDUCTION_UNUSED ||
					mode == STAT_REDUCTION_DULL) {
					if(tgtFrm <= 26)
						draw[0](canvas,
							state->sheet +
								tgtFrm / 4 % 7 * PICTURE_BYTES(DAGGER),
							x - DAGGER_WIDTH / 2, y - DAGGER_HEIGHT / 2,
							DAGGER_WIDTH, DAGGER_HEIGHT);
				} else {
					if(tgtFrm <= 23)
						draw[1](canvas,
							state->sheet +
								tgtFrm / 4 % 6 * PICTURE_BYTES(SHIELD),
							x - SHIELD_WIDTH / 2, y - SHIELD_HEIGHT / 2,
							SHIELD_WIDTH, SHIELD_HEIGHT);
				}
				/* The chime as the sprite breaks. */
				if(tgtFrm == 24)
					sySndPlayStub(SE_CRACK);

				/* The shards fly for 36 ticks, a shade darker every four. */
				if(tgtFrm >= 24 && tgtFrm <= 60) {
					Sint32 level;

					level = 0;
					if(tgtFrm > 28) {
						level = (tgtFrm - 24) / 4;
						if(level > DIMMED_COPIES - 1)
							level = DIMMED_COPIES - 1;
					}
					for(i = 0; i != STAT_REDUCTION_TARGET_SHARDS; i++) {
						PARTICLE *shard =
							&PARTICLES[j * STAT_REDUCTION_TARGET_PARTICLES + i];
						Sint32 picture;

						picture = i % 4 * 3 + (shard->variant + tgtFrm) / 8 % 3;
						btEffectPosProjectPoint((Sint32 *)shard, &screen);
						x = screen.x + xOfs;
						y = screen.y;
						draw[0](canvas,
							state->sheet +
								(level * STAT_REDUCTION_SHARD_BYTES +
									StatChangeShardPictures[picture]) +
								STAT_REDUCTION_SHARD_PICTURES,
							x, y, StatChangeShardWidths[picture],
							StatChangeShardHeights[picture]);
						btParticleAdvanceWithGravity3D(shard, 60, 0);
					}
				}
			}
		}
		EFFECT_PRESENT_CANVAS(state);
	}

	EFFECT_FINISH_CANVAS();
}

#undef STAT_BOOST_BURST_TICK
#undef STAT_BOOST_SPRITE_TICK
#undef STAT_BOOST_TARGET_DELAY
#undef STAT_BOOST_CRUCIBLE_FIRE_TICK
#undef STAT_CHANGE_PARTICLES
#undef STAT_BOOST_TARGET_PARTICLES
#undef STAT_BOOST_TARGET_FLARES
#undef STAT_BOOST_FLARE_SPIN
#undef STAT_BOOST_FLARE_SIZE_MAX
#undef STAT_BOOST_FLARE_DEPTH_NEAR
#undef STAT_BOOST_FLARE_DEPTH_STEP
#undef STAT_BOOST_FLARE_DEPTH_FAR
#undef STAT_BOOST_DJINNI_SWAY
#undef STAT_BOOST_PARTY_CANVAS_X
#undef STAT_REDUCTION_TARGET_PARTICLES
#undef STAT_REDUCTION_TARGET_SHARDS
#undef STAT_REDUCTION_TARGET_DELAY

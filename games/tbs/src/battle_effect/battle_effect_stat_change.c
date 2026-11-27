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
	STAT_BOOST_FLASH	/* a pane of light, in Mars colours */
} STAT_BOOST_MODE;

/* The stat reductions' modes (btEffectRunStatReduction). */
typedef enum STAT_REDUCTION_MODE {
	STAT_REDUCTION_DULL,   /* a dagger, in the pink burst's colours */
	STAT_REDUCTION_IMPAIR, /* a shield, in the glow orb's colours */
	STAT_REDUCTION_WEAKEN, /* a shield, in the pink burst's colours */
	STAT_REDUCTION_UNUSED  /* played as STAT_REDUCTION_WEAKEN; unused */
} STAT_REDUCTION_MODE;

/* Light swirls into each target and bursts, then a dagger appears in Mars
   colours.

   Abilities: Impact, High Impact, Demon Spear, Angel Spear; Forge. */
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

/* Light swirls into each target and bursts, then a shield appears in Jupiter
   colours.

   Abilities: Ward, Resist, Magic Shell, Magic Shield; Breeze. */
void btEffectRunWard(BATTLE_EFFECT_PARM *args)
{
	btEffectRunStatBoost(args, STAT_BOOST_WARD);
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

/* Light swirls into each target and bursts, then a gust of wind sweeps across
   it in the colours already loaded, as this is the one boost that keeps the
   palette.

   Abilities: Zephyr. */
void btEffectRunZephyr(BATTLE_EFFECT_PARM *args)
{
	btEffectRunStatBoost(args, STAT_BOOST_ZEPHYR);
}

/* Where each picture sits in the sprite sheet. */
typedef enum STAT_BOOST_SHEET {
	STAT_BOOST_FLARES = 0,				/* flares (BlueGlowSheet) */
	STAT_BOOST_BURST = GLOW_SHEET_ROOM, /* six bursts of light (GlowOrb) */
	STAT_BOOST_PICTURE = 11150,			/* the picture the mode picks */
	STAT_BOOST_DJINNI = 26048			/* the Djinni */
} STAT_BOOST_SHEET;

/* When the first target's flares burst and its sprite appears, with the damage;
   each later target follows STAT_BOOST_TARGET_DELAY ticks behind. */
#define STAT_BOOST_BURST_TICK	52
#define STAT_BOOST_SPRITE_TICK	80
#define STAT_BOOST_TARGET_DELAY 8

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
	BLITTER_PAIR		 draw;
	Sint32				 member;
	Sint32				 canvasShiftX;
	Sint32				 drawFlg;
	Sint32				 frm;
	Sint32				 i;
	SCREEN_POINT		 base;
	Sint32				 center[3];
	SCREEN_POINT		 screen;
	Sint32				 point[3];
	Sint32				 originX;
	Sint32				 originY;

	state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
	state->args = args;
	btEffectStartCanvasLayer(0);

	if(state->args->prelude == 1) {
		if(mode == STAT_BOOST_GRANITE)
			btEffectRunDjinniEntrance(args, ELEMENT_VENUS,
				state->args->side ^ 1, DJINNI_ANCHOR_TARGET_X, &originX,
				&originY);
		else if(mode == STAT_BOOST_WARD || mode == STAT_BOOST_ZEPHYR)
			btEffectRunDjinniEntrance(args, ELEMENT_JUPITER,
				state->args->side ^ 1, DJINNI_ANCHOR_TARGET_X, &originX,
				&originY);
		else
			btEffectRunDjinniEntrance(args, ELEMENT_MARS, state->args->side ^ 1,
				DJINNI_ANCHOR_TARGET_X, &originX, &originY);
		originX = originX * 4 / 5;
	}
	REG_BG2PA = Q_8_8(0.8); /* 1.25 times the width */

	btResourceLoadAndDecompress(
		RESOURCE_BLUE_GLOW_SHEET, state->sheet + STAT_BOOST_FLARES, 0, 0);
	btResourceLoadAndDecompress(
		RESOURCE_GLOW_ORB_SHEET, state->sheet + STAT_BOOST_BURST, 1, 1);
	if(mode == STAT_BOOST_GRANITE || mode == STAT_BOOST_FLASH) {
		btResourceLoadAndDecompress(RESOURCE_VIOLET_STREAK_SHEET,
			state->sheet + STAT_BOOST_PICTURE, 1, 1);
		if(mode == STAT_BOOST_GRANITE)
			syWordsCopy(BG_PLTT,
				syResourceGetTableEntry(RESOURCE_VENUS_DJINN_SHEET),
				SHEET_PALETTE_BYTES);
		else
			syWordsCopy(BG_PLTT,
				syResourceGetTableEntry(RESOURCE_MARS_DJINN_SHEET),
				SHEET_PALETTE_BYTES);
	} else if(mode == STAT_BOOST_ZEPHYR) {
		btResourceLoadAndDecompress(
			RESOURCE_WISP_SHEET, state->sheet + STAT_BOOST_PICTURE, 1, 1);
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
		else if(mode == STAT_BOOST_GUARD)
			palette = syResourceGetTableEntry(RESOURCE_MARS_DJINN_SHEET);
		else
			palette = syResourceGetTableEntry(RESOURCE_PINK_BURST_SHEET);
		syWordsCopy(BG_PLTT, palette, SHEET_PALETTE_BYTES);
	}
	if(mode == STAT_BOOST_GRANITE)
		btResourceLoadAndDecompress(
			RESOURCE_VENUS_DJINN_SHEET, state->sheet + STAT_BOOST_DJINNI, 1, 0);
	else if(mode == STAT_BOOST_WARD || mode == STAT_BOOST_ZEPHYR)
		btResourceLoadAndDecompress(RESOURCE_JUPITER_DJINN_SHEET,
			state->sheet + STAT_BOOST_DJINNI, 1, 0);
	else
		btResourceLoadAndDecompress(
			RESOURCE_MARS_DJINN_SHEET, state->sheet + STAT_BOOST_DJINNI, 1, 0);

	for(i = 0; i != STAT_CHANGE_PARTICLES; i++) {
		PARTICLE *flare = &PARTICLES[i];

		SET_POSITION(flare, (syRandom() % 200 - 100) << 14,
			(syRandom() % 200 - 100) << 15, (syRandom() % 200 - 100) << 14);
		flare->variant = 0;
	}

	if(state->args->tgtCnt == 1) {
		btProjectionScaledUnit(state->args->tgts[0], &base);
		canvasShiftX = -base.x * 4 / 5 + CANVAS_SIZE / 2;
	} else {
		if(state->args->side == EFFECT_SIDE_PARTY)
			canvasShiftX = -STAT_BOOST_PARTY_CANVAS_X;
		else
			canvasShiftX = 0;
	}
	REG_BG2X = canvasShiftX << 8;
	state->transferMode = CANVAS_COPY_AND_FADE;
	state->transferValue = CANVAS_FADE_HALF;
	sySetEvent(btEffectTransferCanvas, EVENT_PASS_VBLANK);
	if(state->args->side == EFFECT_SIDE_PARTY)
		drawFlg = EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X;
	else
		drawFlg = EFFECT_CLIP_BOUNDS;
	sySndPlayStub(SE_SWIRL);

	for(frm = 0; frm !=
		state->args->tgtCnt * STAT_BOOST_TARGET_DELAY + STAT_BOOST_SPRITE_TICK +
			28;
		frm++) {
		BATTLE_CAMERA *camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);

		if(frm == STAT_BOOST_SPRITE_TICK)
			EFFECT_SHOW_DAMAGE(0);

		/* The prelude's Djinni sways, and from tick 32 rises two pixels a
		   tick; for its first four ticks it is drawn added as well. */
		if(state->args->prelude == 1) {
			Sint32 x = FIXEDtoINT(sySin(frm * (TRIG_TURN / 32)) *
						   STAT_BOOST_DJINNI_SWAY) +
				originX + canvasShiftX - DJINNI_WIDTH / 2;
			Sint32 y =
				FIXEDtoINT(syCos(frm * (TRIG_TURN / 32)) * 4) + originY - 24;

			EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, drawFlg ^ COMPOSITE_FLIP_X,
				COMPOSITE_LIGHTEN);
			draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
			EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
				drawFlg ^ COMPOSITE_FLIP_X, COMPOSITE_ADDITIVE);
			draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
			if(frm > 32)
				y = y - frm * 2 + 64;
			draw[0](canvas, state->sheet + STAT_BOOST_DJINNI, x, y,
				DJINNI_WIDTH, DJINNI_HEIGHT);
			if(frm <= 3)
				draw[1](canvas, state->sheet + STAT_BOOST_DJINNI, x, y,
					DJINNI_WIDTH, DJINNI_HEIGHT);
			EFFECT_RELEASE_BLITTERS();
		}

		EFFECT_FOR_EACH_TARGET(member, state->args) {
			ACTOR *actor = btObjectGetStub(state->args->tgts[member])->actor;
			Sint32 height =
				btUnitGetEffectHeightStub(state->args->tgts[member]) * 2 / 3;

			if(frm == member * STAT_BOOST_TARGET_DELAY + STAT_BOOST_SPRITE_TICK)
				sySndPlayStub(SE_POWER_UP);
			EFFECT_VIEW_FROM_CAMERA(camera);
			SET_VECTOR(point, actor->x.fixed, height, actor->z.fixed);
			syMatrixTranslateVector(point);
			if(frm == member * STAT_BOOST_TARGET_DELAY + 48)
				btEffectTintBattler(state->args->tgts[member],
					SPRITE_COLOR_WHITE, TINT_KEEP, member, 16);

			/* Flares turn and close in on the centre. */
			if(frm > member * STAT_BOOST_TARGET_DELAY) {
				EFFECT_LOAD_BLITTER(
					HEAP_SLOT_RAM_CODE, drawFlg, COMPOSITE_LIGHTEN);
				draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
				EFFECT_LOAD_BLITTER(
					HEAP_SLOT_RAM_CODE_PAIRED, drawFlg, COMPOSITE_ADDITIVE);
				draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
				if(mode == STAT_BOOST_IMPACT) {
					syRotX(-frm * STAT_BOOST_FLARE_SPIN);
				} else if(mode == STAT_BOOST_GUARD) {
					/* Guard's flares close in without turning. */
				} else if(mode == STAT_BOOST_WARD) {
					syRotY(frm * STAT_BOOST_FLARE_SPIN);
				} else {
					syRotY(frm * STAT_BOOST_FLARE_SPIN);
					syRotZ(frm * STAT_BOOST_FLARE_SPIN);
				}
				for(i = 0; i != STAT_BOOST_TARGET_FLARES; i++) {
					PARTICLE *flare =
						&PARTICLES[member * STAT_BOOST_TARGET_PARTICLES + i];

					if(frm > member * STAT_BOOST_TARGET_DELAY + i) {
						Sint32 xx = (flare->x >> 8) * (flare->x >> 8);
						Sint32 yy = (flare->y >> 8) * (flare->y >> 8);
						Sint32 zz = (flare->z >> 8) * (flare->z >> 8);
						Sint32 distance = sySquartIWRAM(xx + yy + zz) >> 9;

						if(distance != 0) {
							Sint32 size;

							btProjectionPoint(flare, &screen);
							screen.x = screen.x * 4 / 5 + canvasShiftX;
							CLAMP(screen.depth, STAT_BOOST_FLARE_DEPTH_NEAR,
								STAT_BOOST_FLARE_DEPTH_FAR);
							size = STAT_BOOST_FLARE_SIZE_MAX -
								(screen.depth - STAT_BOOST_FLARE_DEPTH_NEAR) /
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

			SET_VECTOR(center, 0, 0, 0);
			btProjectionPoint(center, &screen);
			screen.x = screen.x * 4 / 5 + canvasShiftX;

			if(frm >=
					member * STAT_BOOST_TARGET_DELAY + STAT_BOOST_BURST_TICK &&
				frm < member * STAT_BOOST_TARGET_DELAY + STAT_BOOST_BURST_TICK +
						24) {
				Sint32 picture = (frm - member * STAT_BOOST_TARGET_DELAY -
									 STAT_BOOST_BURST_TICK) /
					4 % 6;

				EFFECT_LOAD_BLITTER(
					HEAP_SLOT_RAM_CODE, drawFlg, COMPOSITE_LIGHTEN);
				draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
				draw[0](canvas,
					state->sheet + picture * PICTURE_BYTES(GLOW_ORB) +
						STAT_BOOST_BURST,
					screen.x - GLOW_ORB_WIDTH / 2,
					screen.y - GLOW_ORB_HEIGHT / 2, GLOW_ORB_WIDTH,
					GLOW_ORB_HEIGHT);
				syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
			}

			if(mode == STAT_BOOST_IMPACT) {
				if(frm >= member * STAT_BOOST_TARGET_DELAY +
							STAT_BOOST_SPRITE_TICK &&
					frm < member * STAT_BOOST_TARGET_DELAY +
							STAT_BOOST_SPRITE_TICK + 28) {
					Sint32 picture = (frm - member * STAT_BOOST_TARGET_DELAY -
										 STAT_BOOST_SPRITE_TICK) /
						4 % 7;

					EFFECT_LOAD_BLITTER(
						HEAP_SLOT_RAM_CODE, drawFlg, COMPOSITE_LIGHTEN);
					draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
					draw[0](canvas,
						state->sheet + picture * PICTURE_BYTES(DAGGER) +
							STAT_BOOST_PICTURE,
						screen.x - DAGGER_WIDTH / 2,
						screen.y - DAGGER_HEIGHT / 2, DAGGER_WIDTH,
						DAGGER_HEIGHT);
					syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
				}
			} else if(mode == STAT_BOOST_GRANITE || mode == STAT_BOOST_FLASH) {
				if(frm >= member * STAT_BOOST_TARGET_DELAY +
							STAT_BOOST_SPRITE_TICK &&
					frm < member * STAT_BOOST_TARGET_DELAY +
							STAT_BOOST_SPRITE_TICK + 24) {
					Sint32 picture = (frm - member * STAT_BOOST_TARGET_DELAY -
										 STAT_BOOST_SPRITE_TICK) /
						4 % 6;

					EFFECT_LOAD_BLITTER(
						HEAP_SLOT_RAM_CODE, drawFlg, COMPOSITE_LIGHTEN);
					draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
					draw[0](canvas,
						state->sheet + picture * PICTURE_BYTES(VIOLET_STREAK) +
							STAT_BOOST_PICTURE,
						screen.x - VIOLET_STREAK_WIDTH / 2,
						screen.y - VIOLET_STREAK_HEIGHT / 2,
						VIOLET_STREAK_WIDTH, VIOLET_STREAK_HEIGHT);
					syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
				}
			} else if(mode == STAT_BOOST_ZEPHYR) {
				if(frm >= member * STAT_BOOST_TARGET_DELAY +
							STAT_BOOST_SPRITE_TICK &&
					frm < member * STAT_BOOST_TARGET_DELAY +
							STAT_BOOST_SPRITE_TICK + 24) {
					Sint32 picture = (frm - member * STAT_BOOST_TARGET_DELAY -
										 STAT_BOOST_SPRITE_TICK) /
						2 % 6;

					EFFECT_LOAD_BLITTER(
						HEAP_SLOT_RAM_CODE, drawFlg, COMPOSITE_LIGHTEN);
					draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
					EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
						drawFlg | COMPOSITE_FLIP_Y, COMPOSITE_LIGHTEN);
					draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
					draw[0](canvas,
						state->sheet + picture * PICTURE_BYTES(WISP) +
							STAT_BOOST_PICTURE,
						screen.x - WISP_WIDTH / 2, screen.y - 24, WISP_WIDTH,
						WISP_HEIGHT);
					draw[1](canvas,
						state->sheet + picture * PICTURE_BYTES(WISP) +
							STAT_BOOST_PICTURE,
						screen.x - WISP_WIDTH / 2, screen.y + 8, WISP_WIDTH,
						WISP_HEIGHT);
					EFFECT_RELEASE_BLITTERS();
				}
			} else {
				if(frm >= member * STAT_BOOST_TARGET_DELAY +
							STAT_BOOST_SPRITE_TICK &&
					frm < member * STAT_BOOST_TARGET_DELAY +
							STAT_BOOST_SPRITE_TICK + 24) {
					Sint32 picture = (frm - member * STAT_BOOST_TARGET_DELAY -
										 STAT_BOOST_SPRITE_TICK) /
						4 % 6;

					EFFECT_LOAD_BLITTER(
						HEAP_SLOT_RAM_CODE, drawFlg, COMPOSITE_ADDITIVE);
					draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
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

/* A dagger appears over each target and breaks into shards that fade through
   darker copies, in the pink burst's colours.

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

/* Breaks the shield sprite into fading shards as Weaken does; no effect table
   entry reaches it. */
UNUSED void btEffectRunUnusedStatReduction(BATTLE_EFFECT_PARM *args)
{
	btEffectRunStatReduction(args, STAT_REDUCTION_UNUSED);
}

/* The shard picture in the sprite sheet, then its DIMMED_COPIES - 1 dimmed
   copies. */
#define STAT_REDUCTION_SHARD_PICTURES	9600
#define STAT_REDUCTION_SHARD_BYTES		696 /* bytes in each copy */
/* Each target throws STAT_REDUCTION_TARGET_SHARDS shards from its own run of
   STAT_REDUCTION_TARGET_PARTICLES particles, STAT_REDUCTION_TARGET_DELAY ticks
   after the target before it. */
#define STAT_REDUCTION_TARGET_PARTICLES 32
#define STAT_REDUCTION_TARGET_SHARDS	24
#define STAT_REDUCTION_TARGET_DELAY		4

/* A dagger (Dull) or a shield (the others) appears over each target and breaks
   into shards that fly off and fade. */
void btEffectRunStatReduction(BATTLE_EFFECT_PARM *args, Sint32 mode)
{
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	Sint32				 frm;
	Sint32				 j;
	Sint32				 xOfs;
	Sint32				 origin[3];
	SCREEN_POINT		 screen;
	Sint32				 point[3];
	BLITTER_PAIR		 blitters;
	Sint32				 i;
	Sint32				 paletteId;

	state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
	state->args = args;
	EFFECT_START_NARROW_CANVAS(0);
	if(mode == STAT_REDUCTION_DULL)
		btResourceLoadAndDecompress(RESOURCE_DAGGER_SHEET, state->sheet, 1, 1);
	else
		btResourceLoadAndDecompress(RESOURCE_SHIELD_SHEET, state->sheet, 1, 1);
	if(mode == STAT_REDUCTION_DULL)
		paletteId = RESOURCE_PINK_BURST_SHEET;
	else if(mode == STAT_REDUCTION_IMPAIR)
		paletteId = RESOURCE_GLOW_ORB_SHEET;
	else
		paletteId = RESOURCE_PINK_BURST_SHEET;
	syWordsCopy(
		BG_PLTT, syResourceGetTableEntry(paletteId), SHEET_PALETTE_BYTES);
	btResourceLoadAndDecompress(RESOURCE_STAT_REDUCTION_SHARD_SHEET,
		state->sheet + STAT_REDUCTION_SHARD_PICTURES, 0, 0);

	for(i = 1; i != DIMMED_COPIES; i++) {
		for(j = 0; j != STAT_REDUCTION_SHARD_BYTES; j++) {
			Sint32 level = state->sheet[STAT_REDUCTION_SHARD_PICTURES + j];

			EFFECT_CAP_LEVEL(level, DIMMED_COPY_CAP(i));
			state->sheet[STAT_REDUCTION_SHARD_PICTURES +
				i * STAT_REDUCTION_SHARD_BYTES + j] = level;
		}
	}

	if(state->args->side == EFFECT_SIDE_PARTY) {
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
		if(mode == STAT_REDUCTION_DULL) {
			shard->y = toFIXED(
				(i & (STAT_REDUCTION_TARGET_PARTICLES - 1)) / 4 * 6 - 10);
			shard->z = toFIXED(i % 4 * 2 - 2);
		} else {
			shard->y = toFIXED(
				(i & (STAT_REDUCTION_TARGET_PARTICLES - 1)) / 4 * 6 - 10);
			shard->z = toFIXED(i % 4 * 8 - 16);
		}
		if(state->args->side == EFFECT_SIDE_PARTY)
			shard->velX = toFIXED(2);
		else
			shard->velX = -toFIXED(2);
		shard->velY = ((syCos(ang) * radius) >> 6) + toFIXED(1);
		shard->velZ = (sySin(ang) * radius) >> 6;
		shard->variant = syRandom() & 0xff;
	}

	btEffectBuildCanvasBlitters(state->args->side, blitters);
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

	for(frm = 0; frm != state->args->tgtCnt * STAT_REDUCTION_TARGET_DELAY + 64;
		frm++) {
		BATTLE_CAMERA *camera = BattleCameraWork;

		/* The loop runs count * STAT_REDUCTION_TARGET_DELAY + 64 ticks, so
		   tick 72 comes only with three or more targets. With one or two,
		   btPlaybackWaitForReady starts playback once the effect returns, so
		   the messages follow the shards instead of overlapping them. XXX (no
		   evidence): whether that difference was meant; both orders show the
		   same messages, so only the author's intent would say. */
		if(frm == 72)
			EFFECT_SHOW_DAMAGE(0);
		EFFECT_FOR_EACH_TARGET(j, state->args) {
			ACTOR *actor;
			Sint32 tgtFrm;
			Sint32 x;
			Sint32 y;

			tgtFrm = frm - j * STAT_REDUCTION_TARGET_DELAY;
			actor = btObjectGetStub(state->args->tgts[j])->actor;
			if(tgtFrm > 0) {
				/* DEAD: the view set from the camera a first time, which the
				   second call repeats; the ROM keeps both calls. */
				EFFECT_VIEW_FROM_CAMERA(camera);
				SET_VECTOR(point, actor->x.fixed, toFIXED(20), actor->z.fixed);
				EFFECT_VIEW_FROM_CAMERA(camera);
				syMatrixTranslateVector(point);
				SET_VECTOR(origin, 0, 0, 0);
				btProjectionPoint(origin, &screen);
				x = screen.x + xOfs;
				y = screen.y;
				if(mode == STAT_REDUCTION_DULL) {
					if(tgtFrm <= 26)
						blitters[0](canvas,
							state->sheet +
								tgtFrm / 4 % 7 * PICTURE_BYTES(DAGGER),
							x - DAGGER_WIDTH / 2, y - DAGGER_HEIGHT / 2,
							DAGGER_WIDTH, DAGGER_HEIGHT);
				} else {
					if(tgtFrm <= 23)
						blitters[1](canvas,
							state->sheet +
								tgtFrm / 4 % 6 * PICTURE_BYTES(SHIELD),
							x - SHIELD_WIDTH / 2, y - SHIELD_HEIGHT / 2,
							SHIELD_WIDTH, SHIELD_HEIGHT);
				}
				if(tgtFrm == 24)
					sySndPlayStub(SE_CRACK);
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
						btProjectionPoint(shard, &screen);
						x = screen.x + xOfs;
						y = screen.y;
						blitters[0](canvas,
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
#undef STAT_REDUCTION_SHARD_PICTURES
#undef STAT_REDUCTION_SHARD_BYTES
#undef STAT_REDUCTION_TARGET_PARTICLES
#undef STAT_REDUCTION_TARGET_SHARDS
#undef STAT_REDUCTION_TARGET_DELAY

/* Haunt, the orbiting spirals, and the evil spirits of Demon Night, Rumble,
   Haunting and Thorny Grave. Part of battle_effect.c. */

/* The spirits scattered at the start, up to 64 pixels from their centre. */
#define HAUNT_SCATTERED 512
#define HAUNT_SPIRITS	8  /* evil spirits drawn around each target */
#define HAUNT_SLOTS		64 /* particles reserved for each target */

/* Evil spirits (SkullSheet's skulls, smeared pink by the ripple) close in on
   each target in turn.

   Abilities: Haunt. */
void btEffectRunHaunt(BATTLE_EFFECT_PARM *args)
{
	Sint32				 k;
	SCREEN_POINT		 screenPos;
	FIXED				 point[3];
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	Sint32				 frm;
	DRAW_RECTANGLE		 draw[2];
	BATTLE_CAMERA		*camera;
	Sint32				 i;

	state = RamHeapSlots->battleEff;
	canvas = RamHeapSlots->battleCanvas;
	btEffectStartCanvasLayer(0);
	btResourceLoadAndDecompress(RESOURCE_SKULL_SHEET, state->sheet, 1, 1);
	syWordsCopyEntry((void *)BG_PLTT,
		syResourceGetTableEntry(RESOURCE_PINK_BURST_SHEET),
		SHEET_PALETTE_BYTES);
	btEffectBuildCanvasBlitters(0, draw);
	sySetEvent(btEffectArmBg2AffineHBlankDMA, EVENT_PASS_VBLANK);
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(4));

	/* Scatter every spirit up to 64 pixels from its centre. */
	for(i = 0; i != HAUNT_SCATTERED; i++) {
		SET_POSITION(&PARTICLES[i], ((syRandom() & 255) - 127) << 15,
			((syRandom() & 255) - 127) << 15, ((syRandom() & 255) - 127) << 15);
	}
	sySndPlayStub(SE_SWIRL);

	for(frm = 0; frm != args->tgtCnt * 32 + 64; frm++) {
		Sint32 *row;

		camera = RamHeapSlots->cameraState;
		if(frm == 92)
			EFFECT_SHOW_DAMAGE(0);

		/* Ripple the canvas line by line. */
		row = state->bg2xLines;
		if(args->side == EFFECT_SIDE_ENEMIES) {
			for(i = 0; i != DISPLAY_HEIGHT; i++)
				*row++ = CANVAS_WAVE_BG2X(
					toFIXED(6) - sySin((frm + i) * (TRIG_TURN / 32)) * 6);
		} else {
			for(i = 0; i != DISPLAY_HEIGHT; i++)
				*row++ =
					CANVAS_WAVE_BG2X(sySin((frm + i) * (TRIG_TURN / 32)) * 6);
		}

		EFFECT_FOR_EACH_TARGET(k, args) {
			MOTION_OBJECT *tgt = btObjectGetStub(args->tgts[k])->obj;

			/* Spin the spirits about a point 20 pixels above the target. */
			EFFECT_VIEW_FROM_CAMERA(camera);
			SET_VECTOR(point, tgt->x, toFIXED(20), tgt->z);
			syMatrixTranslateVector(point);
			if(frm > k * 32) {
				syRotX(frm << 9);
				if(frm == k * 32 + 32)
					btEffectTintBattler(
						args->tgts[k], SPRITE_COLOR_WHITE, POSE_DAMAGE, k, 32);

				/* Each spirit steps a part of its distance nearer the centre,
				   and is gone once it arrives. */
				for(i = 0; i != HAUNT_SPIRITS; i++) {
					PARTICLE *spirit = &PARTICLES[k * HAUNT_SLOTS + i];

					if(frm > (k * 8 + i) * 4) {
						Sint32 x = (spirit->x >> 8) * (spirit->x >> 8);
						Sint32 y = (spirit->y >> 8) * (spirit->y >> 8);
						Sint32 z = (spirit->z >> 8) * (spirit->z >> 8);
						Sint32 dist = sySquartIWRAM(x + y + z) >> 8;

						if(dist != 0) {
							btEffectPosProjectPoint(
								(Sint32 *)spirit, &screenPos);
							screenPos.x >>= 1;
							draw[0](canvas,
								state->sheet + i % 3 * PICTURE_BYTES(SKULL),
								screenPos.x - SKULL_WIDTH / 2,
								screenPos.y - SKULL_HEIGHT / 2, SKULL_WIDTH,
								SKULL_HEIGHT);
							spirit->x -= spirit->x / dist;
							spirit->y -= spirit->y / dist;
							spirit->z -= spirit->z / dist;
							spirit->variant++;
						}
					}
				}
			}
		}
		EFFECT_END_TICK(state);
	}

	syCloseEvent(btEffectTransferCanvas);
	syCloseEvent(btEffectArmBg2AffineHBlankDMA);
	EFFECT_RELEASE_BLITTERS();
	btEffectEndCanvasLayer();
}

/* The spirals: ORBITING_SPIRALS a quarter turn apart, circling
   ORBITING_SPIRALS_RADIUS pixels out, each target's ORBITING_SPIRALS_INTERVAL
   ticks after the one before and for ORBITING_SPIRALS_LIFE ticks less one. */
#define ORBITING_SPIRALS			   4
#define ORBITING_SPIRALS_RADIUS		   16
#define ORBITING_SPIRALS_INTERVAL	   16
#define ORBITING_SPIRALS_LIFE		   60
/* The canvas's scroll on the party side until the first tick's wave rows
   replace it, 8 pixels short of NARROW_CANVAS_PARTY_X. */
#define ORBITING_SPIRALS_PARTY_START_X (NARROW_CANVAS_PARTY_X - 8)

/* Unused: no row of AbilityBattleEffects in either game names its kind, and the
   handler table is reached with no other kind (btEffectRunFromTable), so the
   player never sees it. Its code draws the spirals about the point 40 pixels
   above each target, one target after another, turning a 128th of a turn a
   tick; each target takes its damage pose 32 ticks into its turn. */
void btEffectRunUnusedOrbitingSpirals(BATTLE_EFFECT_PARM *args)
{
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	DRAW_RECTANGLE		 draw[2];
	FIXED				 point[3];
	SCREEN_POINT		 screen;
	Sint32				 member;
	Sint32				 ofsX;
	Sint32				 frm;

	state = RamHeapSlots->battleEff;
	canvas = RamHeapSlots->battleCanvas;
	EFFECT_START_NARROW_CANVAS(0);
	btResourceLoadAndDecompress(RESOURCE_SPIRAL_SHEET, state->sheet, 1, 1);
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
	draw[0] = RamHeapSlots->ramCode;
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
		EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP, BLITTER_MAX);
	draw[1] = RamHeapSlots->ramCodePaired;
	sySetEvent(btEffectArmBg2AffineHBlankDMA, EVENT_PASS_VBLANK);
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
	if(args->side == EFFECT_SIDE_PARTY) {
		REG_BG2X = -(ORBITING_SPIRALS_PARTY_START_X << 8);
		ofsX = -NARROW_CANVAS_PARTY_X;
	} else {
		ofsX = 0;
	}

	for(frm = 0; frm != (args->tgtCnt * ORBITING_SPIRALS_INTERVAL) + 48;
		frm++) {
		BATTLE_CAMERA *camera;
		Sint32		  *row;
		Sint32		   i;

		camera = RamHeapSlots->cameraState;
		row = state->bg2xLines;
		if(args->side == EFFECT_SIDE_ENEMIES) {
			for(i = 0; i != DISPLAY_HEIGHT; i++)
				*row++ = CANVAS_WAVE_BG2X(
					toFIXED(8) - sySin((frm + i) * (TRIG_TURN / 64)) * 8);
		} else {
			for(i = 0; i != DISPLAY_HEIGHT; i++)
				*row++ =
					CANVAS_WAVE_BG2X(sySin((frm + i) * (TRIG_TURN / 64)) * 8) -
					(NARROW_CANVAS_PARTY_X << 8);
		}

		EFFECT_VIEW_FROM_CAMERA(camera);
		EFFECT_FOR_EACH_TARGET(member, args) {
			MOTION_OBJECT *tgt;

			tgt = btObjectGetStub(args->tgts[member])->obj;
			if(frm > member * ORBITING_SPIRALS_INTERVAL &&
				frm < (member * ORBITING_SPIRALS_INTERVAL) +
						ORBITING_SPIRALS_LIFE) {
				Sint32 spin;

				if(frm == (member * ORBITING_SPIRALS_INTERVAL) + 32)
					btEffectTintBattler(args->tgts[member], SPRITE_COLOR_NORMAL,
						POSE_DAMAGE, TINT_KEEP, 0);
				SET_VECTOR(point, tgt->x, toFIXED(40), tgt->z);
				btEffectPosProjectPoint(point, &screen);

				for(i = 0; i != ORBITING_SPIRALS; i++) {
					Sint32 x;
					Sint32 y;
					Sint32 half;

					spin = frm * (TRIG_TURN / 128) +
						i * (TRIG_TURN / ORBITING_SPIRALS);
					x = (screen.x +
							FIXEDtoINT(sySin(spin) * ORBITING_SPIRALS_RADIUS)) +
						ofsX;
					y = screen.y +
						FIXEDtoINT(syCos(spin) * ORBITING_SPIRALS_RADIUS);
					half = frm / 16;
					draw[half & 1](canvas,
						state->sheet +
							(frm / 4 - half * 4) * PICTURE_BYTES(SPIRAL),
						x - SPIRAL_WIDTH / 2, y - SPIRAL_HEIGHT / 2,
						SPIRAL_WIDTH, SPIRAL_HEIGHT);
				}
			}
		}
		EFFECT_PRESENT_CANVAS(state);
	}

	syCloseEvent(btEffectTransferCanvas);
	syCloseEvent(btEffectArmBg2AffineHBlankDMA);
	EFFECT_RELEASE_BLITTERS();
	btEffectEndCanvasLayer();
}

/* The evil spirits' modes (btEffectRunGatheringLights), each named for the
   effect that plays it, and its row of GatheringLightsCounts. */
typedef enum GATHERING_SPIRITS_MODE {
	GATHERING_SPIRITS_DEMON_NIGHT, /* pink */
	GATHERING_SPIRITS_RUMBLE,	   /* Mars Djinn colours */
	GATHERING_SPIRITS_HAUNTING,	   /* Mercury Djinn colours */
	GATHERING_SPIRITS_THORNY_GRAVE /* pink, the ramp reversed */
} GATHERING_SPIRITS_MODE;

/* Sixteen pink spirits burst out of the user and are pulled into each target.

   Abilities: Demon Night. */
void btEffectRunDemonNight(BATTLE_EFFECT_PARM *args)
{
	btEffectRunGatheringLights(args, GATHERING_SPIRITS_DEMON_NIGHT);
}

/* Eight spirits in Mars Djinn colours burst out of the user and are pulled into
   each target.

   Abilities: Rumble. */
void btEffectRunRumble(BATTLE_EFFECT_PARM *args)
{
	btEffectRunGatheringLights(args, GATHERING_SPIRITS_RUMBLE);
}

/* Twelve spirits in Mercury Djinn colours burst out of the user and are pulled
   into each target.

   Abilities: Haunting. */
void btEffectRunHaunting(BATTLE_EFFECT_PARM *args)
{
	btEffectRunGatheringLights(args, GATHERING_SPIRITS_HAUNTING);
}

/* Sixteen dark spirits edged in white burst out of the user, are pulled into
   each target and push it back as they arrive.

   Abilities: Thorny Grave. */
void btEffectRunThornyGrave(BATTLE_EFFECT_PARM *args)
{
	btEffectRunGatheringLights(args, GATHERING_SPIRITS_THORNY_GRAVE);
}

#define GATHERING_SPIRITS_SLOTS			 128 /* particles reserved for each target */
/* The targets' turns, this many ticks apart: each is struck by its spirits
   from GATHERING_SPIRITS_HIT_TICK. */
#define GATHERING_SPIRITS_TARGET_TICKS	 20
#define GATHERING_SPIRITS_HIT_TICK		 70
/* The background's widest sway, in pixels, at the effect's midpoint. */
#define GATHERING_SPIRITS_SWAY			 16
/* A layer's BGnHOFS and BGnVOFS as one word, as an HBlank DMA writes them. */
#define BG_OFFSETS(horizontal, vertical) ((horizontal) | (vertical) << 16)
/* The spirits' pictures, three of the sprite sheet's four drawn. */
#define SPIRIT_WIDTH					 20
#define SPIRIT_HEIGHT					 32

/* For each target in turn, spirits burst out of the user every way and from 30
   ticks on are pulled toward the middle of the target's body, vanishing once
   they have crossed it, in the mode's colours. The battle background sways
   sideways, swelling and dying away over the effect. In Thorny Grave every
   spirit that arrives also pushes its target back. */
void btEffectRunGatheringLights(BATTLE_EFFECT_PARM *args, Sint32 mode)
{
	SCREEN_POINT		 screenPos;
	DRAW_RECTANGLE		 draw[2];
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	Sint32				 frm;
	Sint32				 k;
	BATTLE_CAMERA		*camera;
	Sint32				 paletteId;
	Sint32				 i;

	state = RamHeapSlots->battleEff;
	canvas = RamHeapSlots->battleCanvas;
	camera = RamHeapSlots->cameraState;
	btEffectStartCanvasLayer(BGCNT_PRIORITY(1));
	btResourceLoadAndDecompress(
		RESOURCE_MEMBER_BURST_IMAGE, state->sheet, 0, 0);
	if(mode == GATHERING_SPIRITS_DEMON_NIGHT ||
		mode == GATHERING_SPIRITS_THORNY_GRAVE)
		paletteId = RESOURCE_PINK_BURST_SHEET;
	else if(mode == GATHERING_SPIRITS_RUMBLE)
		paletteId = RESOURCE_MARS_DJINN_SHEET;
	else
		paletteId = RESOURCE_MERCURY_DJINN_SHEET;
	LOAD_BG_PALETTE(paletteId);
	if(mode == GATHERING_SPIRITS_THORNY_GRAVE)
		btPaletteMirrorBgCols();
	btEffectBuildCanvasBlitters(args->side, draw);

	/* The spirits fly off the user's body every way, at up to two pixels a
	   tick. */
	EFFECT_FREE_PARTICLES(i, PARTICLES, PARTICLE_POOL_COUNT);
	EFFECT_FOR_EACH_TARGET(k, args) {
		MOTION_OBJECT *src = btObjectGetStub(args->user)->obj;
		Sint32		   height = btUnitGetEffectHeightStub(args->user);

		for(i = 0; i != GATHERING_SPIRITS_SLOTS; i++) {
			PARTICLE *spirit = &PARTICLES[k * GATHERING_SPIRITS_SLOTS + i];

			SET_POSITION(spirit, src->x, height, src->z);
			SET_VELOCITY(spirit, ((syRandom() & 255) - 128) << 10,
				((syRandom() & 255) - 128) << 10,
				((syRandom() & 255) - 128) << 10);
			spirit->variant = 0;
		}
	}

	sySetEvent(btEffectArmBg1HBlankDMA, EVENT_PASS_VBLANK);
	EFFECT_START_TRANSFER(
		state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
	sySndPlayStub(SE_ENEMY_CRY);

	for(frm = 0; frm !=
		GatheringLightsCounts[mode * 2 + 1] +
			args->tgtCnt * GATHERING_SPIRITS_TARGET_TICKS;
		frm++) {
		Sint32 *row;
		Sint32	sway;
		Sint32	scrollX;

		if(frm == 80) {
			if(mode == GATHERING_SPIRITS_DEMON_NIGHT)
				EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
			else if(mode == GATHERING_SPIRITS_THORNY_GRAVE)
				EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
			else
				EFFECT_SHOW_DAMAGE(SE_IMPACT);
		}
		EFFECT_VIEW_FROM_CAMERA(camera);

		/* The battle background sways line by line, by up to
		   GATHERING_SPIRITS_SWAY pixels at the effect's midpoint. A negative
		   scroll wraps into the halfword one short of 65536. */
		row = state->bg2xLines;
		sway =
			FIXEDtoINT(sySin(frm * (TRIG_TURN / 2) /
						   (GatheringLightsCounts[mode * 2 + 1] +
							   args->tgtCnt * GATHERING_SPIRITS_TARGET_TICKS)) *
				GATHERING_SPIRITS_SWAY);
		scrollX = BgScrollWork[1].x;
		for(i = 0; i != DISPLAY_HEIGHT; i++) {
			Sint32 x =
				(FIXEDtoINT(sySin((frm + i) * (TRIG_TURN / 64)) * sway)) +
				scrollX;

			if(x < 0)
				x += 65535;
			*row++ = BG_OFFSETS(x, BATTLE_BACKGROUND_SCROLL_Y);
		}

		EFFECT_FOR_EACH_TARGET(k, args) {
			MOTION_OBJECT *tgt = btObjectGetStub(args->tgts[k])->obj;
			Sint32		   half = btUnitGetEffectHeightStub(args->tgts[k]) / 2;

			if(frm ==
				k * GATHERING_SPIRITS_TARGET_TICKS +
					GATHERING_SPIRITS_HIT_TICK + 1) {
				if(mode == GATHERING_SPIRITS_DEMON_NIGHT)
					sySndPlayStub(SE_HEAVY_IMPACT);
				else if(mode != GATHERING_SPIRITS_THORNY_GRAVE)
					sySndPlayStub(SE_IMPACT);
			}
			if(frm ==
				k * GATHERING_SPIRITS_TARGET_TICKS + GATHERING_SPIRITS_HIT_TICK)
				btEffectTintBattler(
					args->tgts[k], SPRITE_COLOR_WHITE, POSE_DAMAGE, k, 26);
			if(frm > k * GATHERING_SPIRITS_TARGET_TICKS) {
				for(i = 0; i != GatheringLightsCounts[mode * 2]; i++) {
					PARTICLE *spirit =
						&PARTICLES[k * GATHERING_SPIRITS_SLOTS + i];

					if(frm > k * GATHERING_SPIRITS_TARGET_TICKS + i * 2 &&
						spirit->variant >= 0) {
						btEffectPosProjectPoint((Sint32 *)spirit, &screenPos);
						screenPos.x >>= 1;
						draw[0](canvas,
							state->sheet + i % 3 * PICTURE_BYTES(SPIRIT),
							screenPos.x - SPIRIT_WIDTH / 2,
							screenPos.y - SPIRIT_HEIGHT / 2, SPIRIT_WIDTH,
							SPIRIT_HEIGHT);
						btParticleAdvanceWithGravity3D(spirit, 62, 0);

						if(frm > k * GATHERING_SPIRITS_TARGET_TICKS + i + 30) {
							Sint32 x = spirit->x;
							Sint32 dx = (tgt->x - x) >> 8;
							Sint32 dy = (tgt->y + half - spirit->y) >> 9;
							Sint32 dz = (tgt->z - spirit->z) >> 8;
							Sint32 tgtDistance;
							Sint32 spiritDistance;
							Sint32 crossed;

							spirit->velX += dx;
							spirit->velY += dy;
							spirit->velZ += dz;
							/* It has crossed once it is as far out from the
							   middle of the field as its target is, on the
							   target's side. */
							tgtDistance = tgt->x < 0 ? -tgt->x : tgt->x;
							spiritDistance = x;
							if(spiritDistance < 0)
								spiritDistance = -spiritDistance;
							/* DEAD: each arm stores its sign test in crossed,
							   which is never read; the ROM keeps the 0/1 value
							   each store makes. */
							if(tgtDistance <= spiritDistance &&
								((x < 0 && (crossed = tgt->x < 0)) ||
									(x >= 0 && (crossed = tgt->x >= 0)))) {
								spirit->variant = -1;
								if(mode == GATHERING_SPIRITS_THORNY_GRAVE) {
									btEffectStartActorMotion(args->tgts[k], 1,
										toFIXED(2), toFIXED(2), toFIXED(2),
										i * 8 + 100);
									/* FAKEMATCH(dead): spiritDistance set to 0,
									 * never read again. */
									spiritDistance = 0;
									if((i & 1) && frm != 80)
										sySndPlayStub(SE_HEAVY_IMPACT);
								}
							}
						}
					}
				}
			}
		}
		EFFECT_END_TICK(state);
	}

	syCloseEvent(btEffectTransferCanvas);
	EFFECT_RELEASE_BLITTERS();
	syCloseEvent(btEffectArmBg1HBlankDMA);
	btEffectEndCanvasLayer();
}

#undef HAUNT_SCATTERED
#undef HAUNT_SPIRITS
#undef HAUNT_SLOTS
#undef ORBITING_SPIRALS
#undef ORBITING_SPIRALS_RADIUS
#undef ORBITING_SPIRALS_INTERVAL
#undef ORBITING_SPIRALS_LIFE
#undef ORBITING_SPIRALS_PARTY_START_X
#undef GATHERING_SPIRITS_SLOTS
#undef GATHERING_SPIRITS_TARGET_TICKS
#undef GATHERING_SPIRITS_HIT_TICK
#undef GATHERING_SPIRITS_SWAY
#undef BG_OFFSETS
#undef SPIRIT_WIDTH
#undef SPIRIT_HEIGHT

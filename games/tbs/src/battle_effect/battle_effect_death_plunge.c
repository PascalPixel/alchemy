/* Death Plunge, the blade rain, the element pillar, the element burst and the
   shattering opening. Part of battle_effect.c. */

/* A word of two palette entries, both `color`. */
#define COLOR_PAIR(color) ((color) << 16 | (color))

#define ELEMENT_BURST_TICKS		  56 /* ticks of the element burst */
/* The battle background's first BG palette, of the eight its colours take. */
#define BATTLE_BACKGROUND_PALETTE 6

/* The element runners, the streaks and the shattering opening are reached from
   the battle through stub calls, not the handler table, and take their own
   state, canvas and sprite sheet. */

#define DEATH_PLUNGE_TICKS		  96
/* The plunge strikes: the target flashes, the user flashes and the damage
   shows. */
#define DEATH_PLUNGE_STRIKE_TICK  54
/* How far toward the target the user runs, in hundredths of the way. */
#define DEATH_PLUNGE_RUN_PERCENT  90
/* The flashes thrown in a ring after the strike. */
#define DEATH_PLUNGE_RING_FLASHES 16

/* The user runs at the first target, both leap and turn upside down, and on the
   hit they bounce apart while a ball of fire swells over them.

   Abilities: Death Plunge. */
void btEffectRunDeathPlunge(BATTLE_EFFECT_PARM *args)
{
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	/* LEFTOVER(frame): draw[1], the idle half of the pair. */
	BLITTER_PAIR   draw;
	BATTLE_OBJECT *first;
	BATTLE_OBJECT *second;
	ACTOR		  *user;
	ACTOR		  *tgt;
	FIXED		   x;
	FIXED		   z;
	Sint32		   userHeight;
	Sint32		   tgtHeight;
	Sint32		   point[3];
	SCREEN_POINT   screen;
	Sint32		   frm;

	state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
	state->args = args;
	btEffectStartCanvasLayer(0);
	btResourceLoadAndDecompress(RESOURCE_FLASH_BURST_SHEET, state->sheet, 1, 1);
	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, COMPOSITE_LIGHTEN);
	state->transferMode = CANVAS_COPY_AND_FADE;
	state->transferValue = CANVAS_FADE_THREE_QUARTERS;
	draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
	sySetEvent(btEffectTransferCanvas, EVENT_PASS_VBLANK);
	/* The run, most of the way to the target. */
	first = btObjectGetStub(state->args->user);
	second = btObjectGetStub(state->args->tgts[0]);
	user = first->actor;
	tgt = second->actor;
	{
		Sint32 percent = DEATH_PLUNGE_RUN_PERCENT;

		x = user->x.fixed + percent * (tgt->x.fixed - user->x.fixed) / 100;
		z = user->z.fixed + percent * (tgt->z.fixed - user->z.fixed) / 100;
	}
	userHeight = btUnitGetEffectHeightStub(state->args->user);
	tgtHeight = btUnitGetEffectHeightStub(state->args->tgts[0]);
	fdActorStartIdleStub(user);
	fdActorSetMoveTargetStub(user, x, 0, z);
	fdActorSelectAnimationStub(user, POSE_LEAP);
	user->snapToTgt = 1;
	user->steeringFlg = 1;
	user->acceleration = toFIXED(2);
	user->speedLimit = toFIXED(8);
	sySynch(20);
	for(frm = 0; frm != DEATH_PLUNGE_TICKS; frm++) {
		BATTLE_CAMERA *camera = BattleCameraWork;

		EFFECT_VIEW_FROM_CAMERA(camera);

		/* Both leap at 15 pixels a tick. */
		if(frm == 0) {
			tgt->velY = toFIXED(15);
			tgt->gravity = toFIXED(57) / 100;
			user->velY = toFIXED(15);
			user->gravity = toFIXED(57) / 100;
		}
		if(frm == 11) {
			tgt->scaleY = -tgt->scaleY;
			user->scaleY = -user->scaleY;
			user->y.fixed += userHeight;
			tgt->y.fixed += tgtHeight;
		}

		/* The target flashes and both bounce; the user heads back towards the
		   middle of the field. */
		if(frm == DEATH_PLUNGE_STRIKE_TICK) {
			btEffectTintBattler(
				state->args->tgts[0], SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 10);
			tgt->velY = toFIXED(8);
			tgt->gravity = BATTLE_UNIT_GRAVITY;
			user->velY = toFIXED(5);
			user->gravity = toFIXED(47) / 100;
			user->acceleration = toFIXED(1);
			user->speedLimit = toFIXED(2);
			user->steeringFlg = 0;
			fdActorStartIdleStub(user);
			fdActorSetMoveTargetStub(user, 0, 0, user->z.fixed);
		}
		EFFECT_VIEW_FROM_CAMERA(camera);
		SET_VECTOR(point, user->x.fixed, user->y.fixed, user->z.fixed);
		btProjectionPoint(point, &screen);
		screen.x >>= 1;

		/* One big flash on the user. */
		if(frm == DEATH_PLUNGE_STRIKE_TICK ||
			frm == DEATH_PLUNGE_STRIKE_TICK + 1)
			draw[0](canvas, state->sheet, screen.x - FLASH_BURST_WIDTH / 2,
				screen.y - 16, FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
		if(frm >= 56 && frm <= 67) {
			Sint32 i;

			for(i = 0; i != DEATH_PLUNGE_RING_FLASHES; i++) {
				Sint32 ang;
				Sint32 rx;
				Sint32 ry;

				ang = i * (TRIG_TURN / DEATH_PLUNGE_RING_FLASHES);
				rx = screen.x + FIXEDtoINT(sySin(ang) * (frm - 46));
				ry = FIXEDtoINT(syCos(ang) * (frm - 46)) - frm + 100;
				draw[0](canvas,
					state->sheet + (frm - 56) / 2 * PICTURE_BYTES(FLASH_BURST),
					rx - FLASH_BURST_WIDTH / 2, ry, FLASH_BURST_WIDTH,
					FLASH_BURST_HEIGHT);
			}
		}

		/* Both turn right side up again. */
		if(frm == 64) {
			tgt->scaleY = -tgt->scaleY;
			user->scaleY = -user->scaleY;
			user->y.fixed -= userHeight;
			tgt->y.fixed -= tgtHeight;
			fdActorSelectAnimationStub(user, POSE_STILL);
		}
		if(frm == DEATH_PLUNGE_STRIKE_TICK)
			EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
		if(frm == 0) {
			sySndPlayStub(SE_WHOOSH);
			state->shakeFrm = 6;
		}
		if(frm == 53)
			state->shakeFrm = 6;
		btCameraShake(16, 16);
		EFFECT_PRESENT_CANVAS(state);
	}
	EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

#define BLADE_RAIN_TICKS	  120
#define BLADE_RAIN_SWORDS	  16
/* A sword drifts sideways 0 to 3 steps of two pixels a tick, and its picture
   is its step's: velocityX is the step << BLADE_RAIN_STEP_SHIFT. */
#define BLADE_RAIN_STEP_SHIFT 17
/* Where on the canvas the first sword waits for its turn; each four after it
   wait a pixel higher. */
#define BLADE_RAIN_WAIT_Y	  24

/* Swords fall slanting in on the first target, one every 4 ticks, and stick in
   the ground. */
UNUSED void btEffectRunBladeRain(BATTLE_EFFECT_PARM *args)
{
	SCREEN_POINT		 screenPos;
	BLITTER_PAIR		 draw;
	void			   **slots;
	void			   **cursor;
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	DRAW_RECTANGLE		*blit;
	PARTICLE			*blade;
	Sint32				 frm;
	Sint32				 i;

	slots = &HeapSlots[HEAP_SLOT_BATTLE_EFFECT];
	cursor = slots;
	state = *cursor++;
	canvas = *cursor;
	state->args = args;
	EFFECT_START_NARROW_CANVAS(BGCNT_PRIORITY(1));
	REG_BLDALPHA = EFFECT_BLEND_CANVAS(0);

	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, COMPOSITE_TRANSPARENT);
	draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
		EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_TRANSPARENT);
	draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
	blit = draw;
	btResourceLoadAndDecompress(RESOURCE_SWORD_SLASH_SHEET, state->sheet, 1, 1);
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
	btProjectionScaledUnit(state->args->tgts[0], &screenPos);
	REG_BG2X = (CANVAS_SIZE / 2 - screenPos.x) << 8;

	/* Each sword waits near the top of the canvas until its turn, then falls,
	   drifting towards the middle. */
	for(i = 0; i != ARRAY_COUNT(state->particles); i++) {
		Sint32 column;

		blade = &state->particles[i];
		column = syRandom() % 96 + 16;
		blade->x = column;
		blade->y = toFIXED(BLADE_RAIN_WAIT_Y - i / 4);
		if(column <= 43)
			blade->velX = 3;
		else if(column <= 51)
			blade->velX = 2;
		else if(column <= 59)
			blade->velX = 1;
		else if(column <= 67)
			blade->velX = 0;
		else if(column <= 75)
			blade->velX = -1;
		else if(column <= 83)
			blade->velX = -2;
		else
			blade->velX = -3;
		blade->velX <<= BLADE_RAIN_STEP_SHIFT;
		blade->velY = toFIXED(8);
		blade->x <<= 16;
	}
	sySndPlayStub(SE_POWER_UP);
	for(frm = 0; frm != BLADE_RAIN_TICKS; frm++) {
		if(frm <= 16) {
			REG_BLDALPHA = EFFECT_BLEND_CANVAS(frm);
			if(frm == 16)
				REG_BLDCNT = 0;
		}
		if(frm > 103) {
			REG_BLDALPHA = EFFECT_BLEND_CANVAS(120 - frm);
			if(frm == 104)
				REG_BLDCNT =
					BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
		}

		/* The picture follows how steeply the sword falls; one drifting left is
		   mirrored. */
		for(i = BLADE_RAIN_SWORDS - 1; i != -1; i--) {
			Sint32 drift;
			Sint32 speed;
			Sint32 picture;
			Sint32 mirrored;

			blade = &state->particles[i];
			drift = blade->velX;
			speed = drift;
			if(drift < 0)
				speed = -drift;
			picture = speed >> BLADE_RAIN_STEP_SHIFT;
			if(frm < i * 4 + 25) {
				Uint32 width;
				Uint32 height;

				mirrored = drift < 0;
				/* UB: width is assigned in one argument and read in another. */
#if FIX_UNSEQUENCED_SIDE_EFFECT
				width = BladeRainSwordWidths[picture];
				height = BladeRainSwordHeights[picture];
				blit[mirrored](canvas,
					state->sheet + BladeRainSwordPictures[picture],
					FIXEDtoINT(blade->x) - (width >> 1),
					FIXEDtoINT(blade->y) - (height >> 1), width, height);
#else
				blit[mirrored](canvas,
					state->sheet + BladeRainSwordPictures[picture],
					FIXEDtoINT(blade->x) -
						((width = BladeRainSwordWidths[picture]) >> 1),
					FIXEDtoINT(blade->y) -
						((height = BladeRainSwordHeights[picture]) >> 1),
					width, height);
#endif
				if(frm >= i * 4 + 16) {
					blade->x += blade->velX;
					blade->y += blade->velY;
				}
			} else {
				Uint32 width;
				Uint32 height;

				mirrored = drift < 0;
#if FIX_UNSEQUENCED_SIDE_EFFECT
				width = BladeRainSwordWidths[picture];
				height = BladeRainSwordHeights[picture];
				blit[mirrored](canvas,
					state->sheet + BladeRainSwordPictures[picture],
					FIXEDtoINT(blade->x) - (width >> 1),
					FIXEDtoINT(blade->y) - (height >> 1), width, height - 4);
#else
				blit[mirrored](canvas,
					state->sheet + BladeRainSwordPictures[picture],
					FIXEDtoINT(blade->x) -
						((width = BladeRainSwordWidths[picture]) >> 1),
					FIXEDtoINT(blade->y) -
						((height = BladeRainSwordHeights[picture]) >> 1),
					width, height - 4);
#endif
			}
		}

		/* The target flashes and the screen shakes every 4 ticks. */
		if(frm >= 23 && frm <= 87 && (frm & 3) == 0) {
			btEffectTintBattler(
				state->args->tgts[0], SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 2);
			state->shakeFrm = 1;
			if((frm & 7) == 0)
				sySndPlayStub(SE_IMPACT);
		}
		btCameraShake(8, 8);
		EFFECT_END_TICK(state);
	}
	EFFECT_FINISH_CANVAS();
}

#define ELEMENT_PILLAR_TICKS	   56
/* IceTileSheet: the pillar's half in four pictures, then its glow in three. */
#define ELEMENT_PILLAR_HALF_WIDTH  17
#define ELEMENT_PILLAR_HALF_HEIGHT 64
#define ELEMENT_PILLAR_GLOW_WIDTH  24
#define ELEMENT_PILLAR_GLOW_HEIGHT 43
#define ELEMENT_PILLAR_GLOW		   (4 * PICTURE_BYTES(ELEMENT_PILLAR_HALF))
/* The element pillar's and burst's canvas weight over the battle, in
   sixteenths. */
#define ELEMENT_BLEND_LEVEL		   12
/* The motes both set up in PARTICLES; the pillar spirals the first
   ELEMENT_PILLAR_MOTES up around it, each circling once in
   ELEMENT_PILLAR_TURN_TICKS. */
#define ELEMENT_SPIRAL_MOTES	   128
#define ELEMENT_PILLAR_MOTES	   32
#define ELEMENT_PILLAR_TURN_TICKS  64

/* A pillar of light in an element's colours stands at a unit's feet while motes
   spiral up around it. */
void btEffectRunElementPillar(BATTLE_EFFECT_PARM *args)
{
	SCREEN_POINT		 ground;
	SCREEN_POINT		 screenPos;
	Sint32				 point[3];
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	BLITTER_PAIR		 draw;
	Uint8				*sheet;
	BATTLE_CAMERA		*camera;
	Sint32				 frm;
	void				*palette;
	Sint32				 i;

	state = syHeapSlotAlloc(HEAP_SLOT_BATTLE_EFFECT, sizeof *state);
	canvas = syHeapSlotAlloc(HEAP_SLOT_BATTLE_CANVAS, CANVAS_BYTES);
	sheet = syHeapSlotAlloc(HEAP_SLOT_BATTLE_SHEET, GLOW_SHEET_ROOM);
	camera = BattleCameraWork;
	state->args = args;
	btEffectStartCanvasLayer(0);
	state->fadeFrm = BACKGROUND_DARKEN_TICKS;
	state->fadeStep = 0;
	REG_BLDALPHA = EFFECT_BLEND_CANVAS(ELEMENT_BLEND_LEVEL);
	REG_BG2PA = Q_8_8(1);
	btResourceLoadAndDecompress(RESOURCE_ICE_TILE_SHEET, state->sheet, 1, 0);
	btResourceLoadAndDecompress(RESOURCE_BLUE_GLOW_SHEET, sheet, 0, 0);
	switch(state->args->kind) {
	case ELEMENT_VENUS:
		palette = syResourceGetTableEntry(RESOURCE_DEATH_PLUNGE_YELLOW_PALETTE);
		break;
	case ELEMENT_MERCURY:
		palette = syResourceGetTableEntry(RESOURCE_ICE_TILE_SHEET);
		break;
	case ELEMENT_MARS:
		palette = syResourceGetTableEntry(RESOURCE_DEATH_PLUNGE_RED_PALETTE);
		break;
	default:
		palette = syResourceGetTableEntry(RESOURCE_DEATH_PLUNGE_VIOLET_PALETTE);
		break;
	}
	syWordsCopy(BG_PLTT, palette, SHEET_PALETTE_BYTES);

	for(i = 0; i != ELEMENT_SPIRAL_MOTES; i++) {
		PARTICLES[i].y = 0;
		PARTICLES[i].x = syRandom() & TRIG_ANGLE_MASK;
		PARTICLES[i].z = (syRandom() & 0x1ff) + i * 2;
		PARTICLES[i].variant = -i;
	}
	EFFECT_START_TRANSFER(
		state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, COMPOSITE_ADDITIVE);
	draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
	state->args->variant++;
	if(state->args->variant <= 0)
		state->args->variant = 1;
	if(state->args->variant > 4)
		state->args->variant = 4;
	sySndPlayStub(SE_POWER_UP);

	for(frm = 0; frm != ELEMENT_PILLAR_TICKS; frm++) {
		btProjectionUnit(state->args->user, &ground);
		REG_BG2X = (CANVAS_SIZE / 2 - ground.x) << 8;
		if(frm > 49)
			REG_BLDALPHA = EFFECT_BLEND_CANVAS(112 - frm * 2);
		if(frm == 16)
			btEffectTintBattler(
				state->args->tgts[0], SPRITE_COLOR_WHITE, TINT_KEEP, 0, 20);

		/* The pillar, its right half mirrored. */
		if(frm <= 55) {
			Sint32 n;
			Uint8 *glow;

			n = frm / 2 % 4;
			EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
				COMPOSITE_LIGHTEN);
			draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
			draw[1](canvas,
				state->sheet + n * PICTURE_BYTES(ELEMENT_PILLAR_HALF), 47,
				ground.y - ELEMENT_PILLAR_HALF_HEIGHT,
				ELEMENT_PILLAR_HALF_WIDTH, ELEMENT_PILLAR_HALF_HEIGHT);
			n = frm / 4 % 3;
			glow = state->sheet + n * PICTURE_BYTES(ELEMENT_PILLAR_GLOW) +
				ELEMENT_PILLAR_GLOW;
			draw[1](canvas, glow, 40, ground.y - 36, ELEMENT_PILLAR_GLOW_WIDTH,
				ELEMENT_PILLAR_GLOW_HEIGHT);
			syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
			EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
				EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_LIGHTEN);
			draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
			n = frm / 2 % 4;
			draw[1](canvas,
				state->sheet + n * PICTURE_BYTES(ELEMENT_PILLAR_HALF), 64,
				ground.y - ELEMENT_PILLAR_HALF_HEIGHT,
				ELEMENT_PILLAR_HALF_WIDTH, ELEMENT_PILLAR_HALF_HEIGHT);
			draw[1](canvas, glow, 64, ground.y - 36, ELEMENT_PILLAR_GLOW_WIDTH,
				ELEMENT_PILLAR_GLOW_HEIGHT);
			syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
		}
		/* DEAD: a lookup whose result is dropped; the ROM keeps the call. */
		btObjectGetStub(state->args->user);
		EFFECT_VIEW_FROM_CAMERA(camera);

		for(i = 0; i != ELEMENT_PILLAR_MOTES; i++) {
			PARTICLE *fleck = &PARTICLES[i];

			if(fleck->variant >= 0) {
				Sint32 size;
				Sint32 turnSteps = ELEMENT_PILLAR_TURN_TICKS;

				point[0] = fleck->z * sySin(fleck->x) >> 4;
				point[2] = -(fleck->z * syCos(fleck->x) >> 4);
				point[1] = fleck->y;
				fleck->x += TRIG_TURN / turnSteps;
				fleck->y += toFIXED(5);
				fleck->z += 64;
				btProjectionPoint(point, &screenPos);
				screenPos.x /= 2;
				size = (i & 1) + state->args->variant;
				EFFECT_DRAW_FLARE(
					draw[0], canvas, sheet, screenPos.x, screenPos.y, size);
			}
			fleck->variant++;
		}
		EFFECT_END_TICK(state);
	}

	syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
	syCloseEvent(btEffectTransferCanvas);
	btEffectEndCanvasLayer();
	syHeapSlotRelease(HEAP_SLOT_BATTLE_SHEET);
	syHeapSlotRelease(HEAP_SLOT_BATTLE_CANVAS);
	syHeapSlotRelease(HEAP_SLOT_BATTLE_EFFECT);
}

/* Sets one pixel of the disc's mask to colour 2. */
#define SHATTERING_OPENING_PLOT_PIXEL(x, y) \
	(*(RamMapGridMetatileBuffer + ((x) & 7) + (x) / 8 * 64 + ((y) & 7) * 8 + \
		 (y) / 8 * 2048) = 2)

/* Plots one point of the disc's circle in four octants. */
#define SHATTERING_OPENING_PLOT_CORNERS(x1, x2, y1, y2) \
	(SHATTERING_OPENING_PLOT_PIXEL(x2, y2), \
		SHATTERING_OPENING_PLOT_PIXEL(x2, y1), \
		SHATTERING_OPENING_PLOT_PIXEL(x1, y2), \
		SHATTERING_OPENING_PLOT_PIXEL(x1, y1))

#define SHATTERING_OPENING_TICKS	   128
/* flashes thrown as the screen breaks */
#define SHATTERING_OPENING_FLASHES	   32
/* How long a flash floats up, its six pictures four ticks each and the last
   held. */
#define SHATTERING_OPENING_FLASH_TICKS 40
#define SHATTERING_OPENING_PIECES	   33 /* pieces the screen breaks into */
#define SHATTERING_OPENING_DISC_X	   96 /* the centre of the opening disc */
#define SHATTERING_OPENING_DISC_Y	   60 /* the centre of the opening disc */

/* As a battle opens on the storm at Venus Lighthouse's aerie, the screen turns
   white and breaks into pieces that fly apart and fall, while a grey disc grows
   over it and the battle background's colours return. */
void btEffectRunShatteringOpening(BATTLE_EFFECT_PARM *args)
{
	/* LEFTOVER(frame): an unused local the ROM's frame reserves and never
	 * touches. */
	UNUSED SCREEN_POINT		 pos;
	BATTLE_EFFECT_STATE		*state;
	void					*canvas;
	Sint32					 frm;
	BLITTER_PAIR			 draw;
	BATTLE_STATE			*battle;
	BATTLE_BACKGROUND_STATE *view;
	Sint32					 tgtRadius;
	PARTICLE				*piece;
	Sint32					 radius;
	Sint32					 i;
	Sint32					 octantX;
	Sint32					 j;
	Sint32					 k;
	Sint32					 error;
	Sint32					 octantY;
	Sint32					 x1;
	Sint32					 leftX;
	Sint32					 innerLeftX;
	Sint32					 y1;
	Sint32					 x2;
	Sint32					 y2;
	Uint8					*startY;
	Uint8					*startX;
	Sint32					 centreX = SHATTERING_OPENING_DISC_X;
	Sint32					 centreY = SHATTERING_OPENING_DISC_Y;

	state = syHeapSlotAlloc(HEAP_SLOT_BATTLE_EFFECT, sizeof(*state));
	canvas = syHeapSlotAlloc(HEAP_SLOT_BATTLE_CANVAS, CANVAS_BYTES);
	syHeapSlotAlloc(HEAP_SLOT_BATTLE_SHEET, MOTE_SET_BYTES);
	battle = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE);
	view = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_BACKGROUND);
	state->args = args;

	btEffectClearTintTimers();
	view->secondMode = BATTLE_BACKGROUND_FIXED;
	BgScrollWork[1].y = BATTLE_BACKGROUND_SCROLL_Y;
	btBackgroundSetModeStub(BATTLE_BACKGROUND_FIXED, battle->bg, 0);
	REG_BG2CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
		BGCNT_SCREEN_BASE(7);
	btCameraSetViewStub(0, 0, 0, BATTLE_ZOOM_NORMAL);
	view->secondMode = BATTLE_BACKGROUND_NONE;
	EFFECT_SET_CANVAS_AFFINE();
	EFFECT_SET_BATTLE_WINDOWS();
	REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG2 |
		WININ_WIN1_OBJ | WININ_WIN1_CLR;
	REG_WINOUT = EFFECT_WINOUT;

	i = 0;
	for(k = 0; k != CANVAS_TILES; k++) {
		for(j = 0; j != 8; j++, i += 2) {
			Sint16 entry;

			entry = ((k * CANVAS_TILES + j * 2 + 1) << 8) | ((k * 8 + j) * 2);
			*(volatile Sint16 *)((Uint8 *)BG_CANVAS_MAP + i) = entry;
		}
	}

	syWordsClear(canvas, CANVAS_BYTES);
	syWordsClear((void *)BG_CHAR_BLOCK(1), CANVAS_BYTES);
	IO_WRITE_QUEUE_PUSH(
		(Uint32)&REG_DISPCNT, EFFECT_DISPCNT, IO_WRITE_HALFWORD);
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	REG_BLDCNT = 0;
	btResourceLoadAndDecompress(
		RESOURCE_SHATTERING_OPENING_SHEET, state->imageStorage, 1, 1);
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, COMPOSITE_TRANSPARENT);
	draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS, COMPOSITE_LIGHTEN);
	draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);

	/* The pieces fly out from the middle. */
	startY = ShatteringOpeningPieceStartY;
	startX = ShatteringOpeningPieceStartX;
	radius = 0;
	piece = state->particles;
	for(i = 0; i != SHATTERING_OPENING_PIECES; i++) {
		FIXED  x, y;
		Sint32 vx, vy;

		x = toFIXED(*startX++);
		y = toFIXED(*startY++);
		piece->x = x;
		piece->y = y;
		vx = (x - toFIXED(32)) >> 2;
		vy = (y - toFIXED(60)) >> 2;
		piece->velX = vx;
		piece->velY = vy;
		piece++;
	}

	syWordsCopy(
		RamMapGridMetatileBuffer, BG_CHAR_BLOCK(2), BATTLE_BACKGROUND_BYTES);
	syWordsFill(
		RamMapGridMetatileBuffer, BATTLE_BACKGROUND_BYTES, CANVAS_FILL_WORD(1));
	view->busy = 1;
	state->savedBg1X = BgScrollWork[1].x;
	state->savedBg1Y = BgScrollWork[1].y;
	BgScrollWork[1].x = 0;
	IO_WRITE_QUEUE_PUSH((Uint32)&REG_BG1CNT,
		BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(0) | BGCNT_256_COLORS |
			BGCNT_SCREEN_BASE(31),
		IO_WRITE_HALFWORD);
	syWordsFill((void *)&BG_PLTT_COLOR(BATTLE_BACKGROUND_PALETTE, 0),
		BATTLE_BACKGROUND_COLORS * sizeof(Uint16), COLOR_PAIR(RGB_WHITE));
	sySndPlayStub(SE_POWER_UP);
	btEffectTintBattler(
		state->args->tgts[0], SPRITE_COLOR_WHITE, POSE_CAST, 0, 30);

	for(frm = 0; frm != SHATTERING_OPENING_TICKS; frm++) {
		if(frm == 2)
			sySndPlayStub(SE_POWER_UP);
		if(frm == 3)
			sySndPlayStub(SE_POWER_UP);
		if(frm == 28)
			btEffectTintBattler(
				state->args->tgts[0], TINT_KEEP, POSE_CAST, TINT_KEEP, 0);
		if(frm == 32)
			sySndPlayStub(SE_MONSTER_ROAR);
		if(frm == 5)
			sySndPlayStub(SE_CRASH);
		/* Bring the battle background back on BG1. */
		if(frm == 5) {
			BgScrollWork[1].x = state->savedBg1X;
			btBackgroundLoadStub(BATTLE_BACKGROUND_FIXED, battle->bg, -1);
		}

		/* Ease the white back to the battle background's colours. */
		if(frm > 7) {
			Uint16 *col;

			col = (Uint16 *)&BG_PLTT_COLOR(BATTLE_BACKGROUND_PALETTE, 0);
			for(i = 0; i != BATTLE_BACKGROUND_COLORS; i++) {
				Sint32 red = RGB_RED(*col);
				Sint32 green = RGB_GREEN(*col);
				Sint32 blue = RGB_BLUE(*col);
				Sint32 goalRed = RGB_RED(battle->pal[i]);
				Sint32 goalGreen = RGB_GREEN(battle->pal[i]);
				Sint32 goalBlue = RGB_BLUE(battle->pal[i]);

				if(red < goalRed)
					red++;
				else if(red > goalRed)
					red--;
				if(green < goalGreen)
					green++;
				else if(green > goalGreen)
					green--;
				if(blue < goalBlue)
					blue++;
				else if(blue > goalBlue)
					blue--;
				*col++ = RGB(red, green, blue);
			}
		}

		/* The whole screen turns the disc's grey. */
		if(frm == 4)
			syWordsFill((void *)BG_CHAR_BLOCK(2), BATTLE_BACKGROUND_BYTES,
				CANVAS_FILL_WORD(2));

		/* Grow the grey disc. */
		if(frm <= 3) {
			Sint32 v;

			v = frm * 4 + 8;
			tgtRadius = frm * 32;
			BG_PLTT[2] = RGB(v, v, v);
			for(; radius != tgtRadius; radius++) {
				octantX = radius;
				octantY = 0;
				error = radius;
				while(octantX >= octantY) {
					x2 = octantX + centreX;
					x1 = centreX - octantX;
					leftX = x1;
					y1 = centreY - octantY;
					y2 = octantY + centreY;
					if(y1 < 0)
						y1 = 0;
					if(y2 > BATTLE_BACKGROUND_HEIGHT - 1)
						y2 = BATTLE_BACKGROUND_HEIGHT - 1;
					if(x1 < 0)
						x1 = 0;
					if(x2 > BATTLE_BACKGROUND_WIDTH - 1)
						x2 = BATTLE_BACKGROUND_WIDTH - 1;
					SHATTERING_OPENING_PLOT_CORNERS(x1, x2, y1, y2);
					x1 = leftX + 1;
					x2 = octantX + SHATTERING_OPENING_DISC_X + 1;
					if(x1 < 0)
						x1 = 0;
					if(x2 > BATTLE_BACKGROUND_WIDTH - 1)
						x2 = BATTLE_BACKGROUND_WIDTH - 1;
					SHATTERING_OPENING_PLOT_CORNERS(x1, x2, y1, y2);

					x2 = octantY + centreX;
					x1 = centreX - octantY;
					innerLeftX = x1;
					y2 = octantX + centreY;
					y1 = centreY - octantX;
					if(x1 < 0)
						x1 = 0;
					if(x2 > BATTLE_BACKGROUND_WIDTH - 1)
						x2 = BATTLE_BACKGROUND_WIDTH - 1;
					if(y1 < 0)
						y1 = 0;
					if(y2 > BATTLE_BACKGROUND_HEIGHT - 1)
						y2 = BATTLE_BACKGROUND_HEIGHT - 1;
					SHATTERING_OPENING_PLOT_CORNERS(x1, x2, y1, y2);
					x1 = innerLeftX + 1;
					x2 = octantY + SHATTERING_OPENING_DISC_X + 1;
					if(x1 < 0)
						x1 = 0;
					if(x2 > BATTLE_BACKGROUND_WIDTH - 1)
						x2 = BATTLE_BACKGROUND_WIDTH - 1;
					SHATTERING_OPENING_PLOT_CORNERS(x1, x2, y1, y2);

					error -= octantY * 2 + 1;
					if(error < 0) {
						error += octantX * 2 - 2;
						octantX--;
					}
					octantY++;
				}
			}
			syWordsCopy(BG_CHAR_BLOCK(2), RamMapGridMetatileBuffer,
				BATTLE_BACKGROUND_BYTES);
		}

		/* The pieces fly and fall. */
		if(frm <= 50) {
			for(i = 0; i != SHATTERING_OPENING_PIECES; i++) {
				piece = &state->particles[i];
				draw[0](canvas,
					state->imageStorage + ShatteringOpeningPiecePictures[i],
					FIXEDtoINT(piece->x), FIXEDtoINT(piece->y),
					ShatteringOpeningPieceWidths[i],
					ShatteringOpeningPieceHeights[i]);
				if(frm > 3)
					btParticleAdvanceWithGravity2D(
						piece, PARTICLE_DAMPING_NONE, toFIXED(1) / 4);
			}
		}

		if(frm >= 8 && frm <= 50) {
			Sint32 v;

			v = frm - 8;
			if(v > RGB_LEVEL_MAX)
				v = RGB_LEVEL_MAX;
			BG_PLTT[1] = RGB(v, v, v);
		}

		if(frm == 51) {
			btResourceLoadAndDecompress(
				RESOURCE_FLASH_BURST_SHEET, state->sheet, 1, 0);
			for(i = 1; i != SHEET_PALETTE_COLORS; i++) {
				Sint32 v;

				v = i / 2;
				/* DEAD: v is never below 0; the ROM keeps the test. */
				if(v < 0)
					v = 0;
				BG_PLTT[i] = RGB(v, v / 2, v);
			}
			REG_BLDCNT =
				BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
			for(i = 0; i != SHATTERING_OPENING_FLASHES; i++) {
				piece = &state->particles[i];
				piece->x = INTtoFIXED((syRandom() & 31) + 32);
				piece->y = INTtoFIXED((syRandom() & 31) + 80);
				piece->velX = ((syRandom() & 0x1ff) - 256) << 12;
				piece->velY = 0;
				piece->variant = 0;
			}
			state->transferMode = CANVAS_COPY_AND_FADE;
			state->transferValue = CANVAS_FADE_HALF;
		}

		/* Flashes float up and fade. */
		if(frm > 52) {
			for(i = 0; i != SHATTERING_OPENING_FLASHES; i++) {
				piece = &state->particles[i];
				if(frm >= i / 4 + 52 &&
					piece->variant < SHATTERING_OPENING_FLASH_TICKS) {
					Sint32 picture;

					picture = piece->variant / 4;
					if(picture > 5)
						picture = 5;
					draw[1](canvas,
						state->sheet + picture * PICTURE_BYTES(FLASH_BURST),
						FIXEDtoINT(piece->x) - FLASH_BURST_WIDTH / 2,
						FIXEDtoINT(piece->y) - FLASH_BURST_HEIGHT / 2,
						FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
					btParticleAdvanceWithGravity2D(piece, 60, -toFIXED(1) / 16);
					piece->variant++;
				}
			}
		}

		EFFECT_END_TICK(state);
	}

	EFFECT_RELEASE_BLITTERS();
	syCloseEvent(btEffectTransferCanvas);
	btEffectTintBattler(
		state->args->tgts[0], TINT_KEEP, POSE_STANDING, TINT_KEEP, 0);
	BgScrollWork[1].x = state->savedBg1X;
	BgScrollWork[1].y = BATTLE_BACKGROUND_SCROLL_Y;
	btBackgroundSetModeStub(BATTLE_BACKGROUND_ZOOMED, battle->bg, 0);
	sySynch(1);
	IO_WRITE_QUEUE_PUSH((Uint32)&REG_DISPCNT,
		DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG2_ON |
			DISPCNT_OBJ_ON | DISPCNT_WIN0_ON | DISPCNT_WIN1_ON,
		IO_WRITE_HALFWORD);
	syHeapSlotRelease(HEAP_SLOT_BATTLE_SHEET);
	syHeapSlotRelease(HEAP_SLOT_BATTLE_CANVAS);
	syHeapSlotRelease(HEAP_SLOT_BATTLE_EFFECT);
}

/* The element streaks seeded at the start, and those drawn. */
#define STREAK_SEEDS		  256
#define STREAKS				  64
/* A streak's head starts 48 to 63 from the centre, its tail STREAK_LENGTH
   behind it. */
#define STREAK_START_DISTANCE 48
#define STREAK_LENGTH		  8

/* One streak of light of the element streak burst, over a particle's entry. */
typedef struct PLUNGE_STREAK {
	Sint32 head;	/* the head's distance from the centre */
	Sint32 tail;	/* the tail's distance; both close in 4 a tick */
	Sint32 unusedZ; /* the Particle's z, which the streaks leave alone */
	Sint32 pitch;	/* the axis's turn, a random 16-bit angle */
	Sint32 yaw;		/* the axis's turn, a random 16-bit angle */
	Sint32 roll;	/* the axis's turn, a random 16-bit angle */
	/* the Particle's variant, which the streaks leave alone */
	Sint32 unusedVariant;
} PLUNGE_STREAK;

/* CyanSparkSheet: five small sparks, then the burst's pictures. */
#define ELEMENT_BURST_SPARK_WIDTH  32
#define ELEMENT_BURST_SPARK_HEIGHT 32
#define ELEMENT_BURST_BURST_WIDTH  48
#define ELEMENT_BURST_BURST_HEIGHT 48
#define ELEMENT_BURST_BURST		   (5 * PICTURE_BYTES(ELEMENT_BURST_SPARK))
/* A burst mote's shortest life in ticks; by its index a mote lives up to twice
   as long, less one. */
#define ELEMENT_BURST_MOTE_LIFE	   16

/* Sparks in an element's colours flare at four corners around a unit and motes
   spiral in, then a burst of light plays over it. */
void btEffectRunElementBurst(BATTLE_EFFECT_PARM *args)
{
	SCREEN_POINT		 ground;
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	BLITTER_PAIR		 draw;
	Uint8				*sheet;
	void				*palette;
	Sint32				 frm;
	Sint32				 i;

	state = syHeapSlotAlloc(HEAP_SLOT_BATTLE_EFFECT, sizeof *state);
	canvas = syHeapSlotAlloc(HEAP_SLOT_BATTLE_CANVAS, CANVAS_BYTES);
	sheet = syHeapSlotAlloc(HEAP_SLOT_BATTLE_SHEET, GLOW_SHEET_ROOM);
	state->args = args;
	btEffectStartCanvasLayer(0);
	/* Setup, as for the element pillar. */
	state->fadeFrm = BACKGROUND_DARKEN_TICKS;
	state->fadeStep = 0;
	REG_BLDALPHA = EFFECT_BLEND_CANVAS(ELEMENT_BLEND_LEVEL);
	REG_BG2PA = Q_8_8(1);
	btResourceLoadAndDecompress(RESOURCE_CYAN_SPARK_SHEET, state->sheet, 1, 0);
	btResourceLoadAndDecompress(RESOURCE_BLUE_GLOW_SHEET, sheet, 0, 0);
	switch(state->args->kind) {
	case ELEMENT_VENUS:
		palette = syResourceGetTableEntry(RESOURCE_DEATH_PLUNGE_YELLOW_PALETTE);
		break;
	case ELEMENT_MERCURY:
		palette = syResourceGetTableEntry(RESOURCE_ICE_TILE_SHEET);
		break;
	case ELEMENT_MARS:
		palette = syResourceGetTableEntry(RESOURCE_DEATH_PLUNGE_RED_PALETTE);
		break;
	default:
		palette = syResourceGetTableEntry(RESOURCE_DEATH_PLUNGE_VIOLET_PALETTE);
		break;
	}
	syWordsCopy(BG_PLTT, palette, SHEET_PALETTE_BYTES);

	for(i = 0; i != ELEMENT_SPIRAL_MOTES; i++) {
		PARTICLES[i].y = toFIXED(128);
		PARTICLES[i].x = syRandom() & TRIG_ANGLE_MASK;
		PARTICLES[i].z = (syRandom() & 0x1ff) + 1024;
		PARTICLES[i].variant = -i;
	}
	/* A mote's x is its angle and y its distance from the user; its variant
	   counts down its life. */
	for(i = 0; i != ARRAY_COUNT(state->particles); i++) {
		state->particles[i].x = syRandom() & TRIG_ANGLE_MASK;
		state->particles[i].y = (syRandom() & 31) + 16;
		state->particles[i].variant =
			(i & (ELEMENT_BURST_MOTE_LIFE - 1)) + ELEMENT_BURST_MOTE_LIFE;
	}
	EFFECT_START_TRANSFER(
		state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
		EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_ADDITIVE);
	draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
	sySndPlayStub(SE_SPIN);

	for(frm = 0; frm != ELEMENT_BURST_TICKS; frm++) {
		btProjectionScaledUnit(args->user, &ground);
		REG_BG2X = (CANVAS_SIZE / 2 - ground.x) << 8;
		if(frm > 49)
			REG_BLDALPHA = EFFECT_BLEND_CANVAS(112 - frm * 2);
		if(frm == 26) {
			sySndPlayStub(SE_POWER_UP);
			btEffectTintBattler(
				state->args->tgts[0], SPRITE_COLOR_WHITE, TINT_KEEP, 0, 20);
		}
		if(frm >= 28 && frm <= 48)
			draw[0](canvas,
				state->sheet +
					(frm - 28) / 3 * PICTURE_BYTES(ELEMENT_BURST_BURST) +
					ELEMENT_BURST_BURST,
				40, ground.y - 24, ELEMENT_BURST_BURST_WIDTH,
				ELEMENT_BURST_BURST_HEIGHT);
		if(frm >= 0 && frm <= 14) {
			Sint32 picture = frm / 3 % 5;

			for(i = 0; i != 4; i++) {
				EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
					ElementBurstSparkFlips[i] | EFFECT_CLIP_BOUNDS,
					COMPOSITE_LIGHTEN);
				draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
				draw[1](canvas,
					state->sheet + picture * PICTURE_BYTES(ELEMENT_BURST_SPARK),
					ElementBurstSparkX[i] + 32,
					ground.y + ElementBurstSparkY[i] - 32,
					ELEMENT_BURST_SPARK_WIDTH, ELEMENT_BURST_SPARK_HEIGHT);
				syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
			}
		}
		if(frm >= 0) {
			for(i = 0; i != ARRAY_COUNT(state->particles); i++) {
				PARTICLE *mote = &state->particles[i];

				if(mote->variant >= 0 && mote->y > 0) {
					Sint32 size = (mote->variant >> 3) + 1;
					Sint32 x = FIXEDtoINT(mote->y * sySin(mote->x)) + 64;
					Sint32 y = FIXEDtoINT(mote->y * syCos(mote->x)) + ground.y;

					/* DEAD: never true, as the variant is not negative here;
					   the ROM keeps the test and the store. */
					if(size <= 0)
						size = 1;
					EFFECT_DRAW_FLARE(draw[0], canvas, sheet, x, y, size);
					mote->y -= 2;
					mote->variant--;
				}
			}
		}
		EFFECT_END_TICK(state);
	}

	/* Finish, as for the element pillar. */
	syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
	syCloseEvent(btEffectTransferCanvas);
	btEffectEndCanvasLayer();
	syHeapSlotRelease(HEAP_SLOT_BATTLE_SHEET);
	syHeapSlotRelease(HEAP_SLOT_BATTLE_CANVAS);
	syHeapSlotRelease(HEAP_SLOT_BATTLE_EFFECT);
}

/* Draws a tick of the element streaks: lines of light closing in on the centre
   of the canvas. */
void btEffectDrawStreaks(void)
{
	BATTLE_EFFECT_STATE *state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	Sint32				 frm;
	Sint32				 i;
	PLUNGE_STREAK		*streak;
	VECTOR3				 point;
	SCREEN_POINT		 head;
	SCREEN_POINT		 tail;

	frm = (state->frm)++;
	if(frm == 0) {
		for(i = 0; i != STREAK_SEEDS; i++) {
			PLUNGE_STREAK *seed =
				&((PLUNGE_STREAK *)RamMapGridMetatileBuffer)[i];
			Sint32 r = syRandom() & 15;

			seed->head = r + STREAK_START_DISTANCE;
			seed->tail = r + STREAK_START_DISTANCE - STREAK_LENGTH;
			seed->pitch = syRandom() & TRIG_ANGLE_MASK;
			seed->yaw = syRandom() & TRIG_ANGLE_MASK;
			seed->roll = syRandom() & TRIG_ANGLE_MASK;
		}
	}
	point.y = 0;
	point.z = 0;
	for(i = 0; i != STREAKS; i++) {
		streak = &((PLUNGE_STREAK *)RamMapGridMetatileBuffer)[i];
		if(frm > i / 4 && streak->head > 0) {
			Sint32 fade;

			syInitMatrix();
			syRotZ(streak->roll);
			syRotX(streak->pitch);
			syRotY(streak->yaw);
			point.x = streak->head;
			btProjectionPoint(&point, &head);
			head.x += 64;
			head.y += 80;
			point.x = streak->tail;
			btProjectionPoint(&point, &tail);
			tail.x += 64;
			tail.y += 80;
			streak->tail -= 4;
			streak->head -= 4;
			if(streak->tail < 0)
				streak->tail = 0;
			/* The tail stops at the centre, and the head goes out once it
			   passes it. */
			fade = -streak->tail / 2;
			btEffectDrawClippedCanvasLine(
				tail.x - 1, tail.y, head.x - 1, head.y, fade + 48);
			btEffectDrawClippedCanvasLine(
				tail.x, tail.y - 1, head.x, head.y - 1, fade + 48);
			btEffectDrawClippedCanvasLine(
				tail.x, tail.y, head.x, head.y, fade + 56);
		}
	}
	state->transferPending = 1;
}

/* Starts the element streaks, which then draw by themselves while the battle
   runs on. */
void btEffectStartElementStreaks(Sint32 element)
{
	BATTLE_EFFECT_STATE *state;
	void				*palette;
	Sint32				 id;

	state = syHeapSlotAlloc(HEAP_SLOT_BATTLE_EFFECT, sizeof *state);
	syHeapSlotAlloc(HEAP_SLOT_BATTLE_CANVAS, CANVAS_BYTES);
	btEffectStartCanvasLayer(0);
	state->fadeFrm = BACKGROUND_DARKEN_TICKS;
	REG_BG2PA = Q_8_8(1);
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	switch(element) {
	case ELEMENT_VENUS:
		id = RESOURCE_VENUS_STAR_FIELD_PALETTE;
		break;
	case ELEMENT_MERCURY:
		id = RESOURCE_CRYSTAL_SHEET;
		break;
	case ELEMENT_MARS:
		id = RESOURCE_EMBER_STREAK_SHEET;
		break;
	case ELEMENT_JUPITER:
		id = RESOURCE_JUPITER_STAR_FIELD_PALETTE;
		break;
	case ELEMENT_NONE:
	default:
		id = RESOURCE_DUST_PUFF_SHEET;
		break;
	}
	palette = syResourceGetTableEntry(id);
	syWordsCopy(BG_PLTT, palette, SHEET_PALETTE_BYTES);
	state->frm = 0;
	state->transferMode = CANVAS_COPY_AND_DARKEN;
	state->transferValue = CANVAS_FILL_WORD(6);
	sySetEvent(btEffectDrawStreaks, EVENT_PASS_TICK);
	sySetEvent(btEffectTransferCanvas, EVENT_PASS_VBLANK);
}

/* Ends the element streaks and frees what they took. */
void btEffectEndElementStreaks(void)
{
	syCloseEvent(btEffectDrawStreaks);
	syCloseEvent(btEffectTransferCanvas);
	syWordsClear((void *)BG_CHAR_BLOCK(1), CANVAS_BYTES);
	syCloseEvent(btEffectDarkenBackgroundStep);
	syHeapSlotRelease(HEAP_SLOT_BATTLE_CANVAS);
	syHeapSlotRelease(HEAP_SLOT_BATTLE_EFFECT);
}

#undef COLOR_PAIR
#undef BATTLE_BACKGROUND_PALETTE
#undef ELEMENT_BURST_TICKS
#undef DEATH_PLUNGE_STRIKE_TICK
#undef DEATH_PLUNGE_TICKS
#undef DEATH_PLUNGE_RUN_PERCENT
#undef DEATH_PLUNGE_RING_FLASHES
#undef BLADE_RAIN_TICKS
#undef BLADE_RAIN_SWORDS
#undef BLADE_RAIN_STEP_SHIFT
#undef BLADE_RAIN_WAIT_Y
#undef ELEMENT_PILLAR_TICKS
#undef ELEMENT_BLEND_LEVEL
#undef ELEMENT_SPIRAL_MOTES
#undef ELEMENT_PILLAR_MOTES
#undef ELEMENT_PILLAR_TURN_TICKS
#undef SHATTERING_OPENING_PLOT_PIXEL
#undef SHATTERING_OPENING_PLOT_CORNERS
#undef SHATTERING_OPENING_TICKS
#undef SHATTERING_OPENING_FLASHES
#undef SHATTERING_OPENING_FLASH_TICKS
#undef SHATTERING_OPENING_PIECES
#undef SHATTERING_OPENING_DISC_X
#undef SHATTERING_OPENING_DISC_Y
#undef STREAK_SEEDS
#undef STREAKS
#undef STREAK_START_DISTANCE
#undef STREAK_LENGTH
#undef ELEMENT_BURST_MOTE_LIFE

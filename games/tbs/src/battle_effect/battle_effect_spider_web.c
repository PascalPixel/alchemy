/* Poison Beat, Spinning Beat and Spider Web. Part of battle_effect.c. */

/* The swirl swipe's modes (btEffectRunSwirlSwipe), each named for the effect
   that plays it. */
typedef enum SWIRL_SWIPE_MODE {
	SWIRL_SWIPE_POISON_BEAT,  /* lime; the target recoils at once, 48 ticks */
	SWIRL_SWIPE_SPINNING_BEAT /* magenta; the target is thrown up, 74 ticks */
} SWIRL_SWIPE_MODE;

/* A lime swirl sweeps across the first target and knocks it back.

   Abilities: Poison Beat. */
void btEffectRunPoisonBeat(BATTLE_EFFECT_PARM *args)
{
	btEffectRunSwirlSwipe(args, SWIRL_SWIPE_POISON_BEAT);
}

/* A magenta swirl sweeps across the first target and throws it into the air.

   Abilities: Spinning Beat. */
void btEffectRunSpinningBeat(BATTLE_EFFECT_PARM *args)
{
	btEffectRunSwirlSwipe(args, SWIRL_SWIPE_SPINNING_BEAT);
}

/* A swirl sweeps across the first target from the user's side. */
void btEffectRunSwirlSwipe(BATTLE_EFFECT_PARM *args, Sint32 mode)
{
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	ACTOR				*actor;
	SCREEN_POINT		 pos;
	/* LEFTOVER(frame): draw[1], the idle half of the pair. */
	BLITTER_PAIR draw;
	Sint32		 ticks;
	Sint32		 frm;
	Sint32		 picture;

	state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
	state->args = args;
	EFFECT_START_NARROW_CANVAS(0);
	if(state->args->side == EFFECT_SIDE_PARTY) {
		EFFECT_LOAD_BLITTER(
			HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, COMPOSITE_TRANSPARENT);
		draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
	} else {
		EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
			EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_TRANSPARENT);
		draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
	}
	btResourceLoadAndDecompress(
		RESOURCE_MAGENTA_SWIRL_SHEET, state->sheet, 1, 1);
	btResourceLoadAndDecompress(
		RESOURCE_MAGENTA_TAIL_SHEET, Ram_MapGridMetatileBuffer, 1, 0);
	if(mode == SWIRL_SWIPE_POISON_BEAT) {
		void *palette = syResourceGetTableEntry(RESOURCE_LIME_PALETTE);
		syWordsCopy(BG_PLTT, palette, SHEET_PALETTE_BYTES);
	}
	EFFECT_START_TRANSFER(
		state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
	actor = btObjectGetStub(state->args->tgts[0])->actor;
	btProjectionScaledUnit(state->args->tgts[0], &pos);
	if(state->args->side == EFFECT_SIDE_ENEMIES)
		REG_BG2X = (16 - pos.x) << 8;
	else
		REG_BG2X = (112 - pos.x) << 8;
	ticks = 74;
	if(mode != SWIRL_SWIPE_SPINNING_BEAT)
		ticks = 48;
	for(frm = 0; frm != ticks; frm++) {
		/* The swirl's four pictures, then the two of its tail, four ticks
		   each. */
		picture = frm / 4;
		if(picture <= 5) {
			if(picture <= 3)
				draw[0](canvas, state->sheet + SwirlSwipePictures[picture],
					SwirlSwipePictureX[picture + state->args->side * 6],
					SwirlSwipePictureBiasY[picture] + 32,
					SwirlSwipePictureWidths[picture],
					SwirlSwipePictureHeights[picture]);
			else
				draw[0](canvas,
					Ram_MapGridMetatileBuffer + SwirlSwipePictures[picture],
					SwirlSwipePictureX[picture + state->args->side * 6],
					SwirlSwipePictureBiasY[picture] + 32,
					SwirlSwipePictureWidths[picture],
					SwirlSwipePictureHeights[picture]);
		}
		if(frm == 8) {
			if(mode == SWIRL_SWIPE_POISON_BEAT) {
				EFFECT_SHOW_DAMAGE(SE_IMPACT);
				btMotionApplyKnockbackStub(state->args->tgts[0], KNOCKBACK_HOP);
			} else {
				sySndPlayStub(SE_HEAVY_IMPACT);
				btEffectTintBattler(state->args->tgts[0], SPRITE_COLOR_WHITE,
					POSE_DAMAGE, 0, 4);
			}
			state->shakeFrm = 8;
		}
		/* Spinning Beat throws the target into the air. */
		if(mode == SWIRL_SWIPE_SPINNING_BEAT) {
			if(frm == 13) {
				actor->velY = toFIXED(12);
				actor->gravity = toFIXED(47) / 100;
				actor->bounce = toFIXED(1) / 4;
			}
			if(frm == 65) {
				state->shakeFrm = 4;
				EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
			}
		}
		btCameraShake(8, 8);
		EFFECT_END_TICK(state);
	}
	EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

#define SPIDER_WEB_TICKS		  63
/* SpiderWebSheet's quarter of the web, drawn four times mirrored. */
#define SPIDER_WEB_QUARTER_WIDTH  32
#define SPIDER_WEB_QUARTER_HEIGHT 32
/* The web's middle on the canvas, which the canvas puts over the targets'
   middle; the quarters to its left and above take its column and row too. */
#define SPIDER_WEB_CENTER_X		  (CANVAS_SIZE / 2)
#define SPIDER_WEB_CENTER_Y		  72
#define SPIDER_WEB_LEFT			  (SPIDER_WEB_CENTER_X - SPIDER_WEB_QUARTER_WIDTH + 1)
#define SPIDER_WEB_TOP			  (SPIDER_WEB_CENTER_Y - SPIDER_WEB_QUARTER_HEIGHT + 1)

/* A spider web spreads over the targets in four mirrored quarters.

   Abilities: Spider Web. */
void btEffectRunSpiderWeb(BATTLE_EFFECT_PARM *args)
{
	SCREEN_POINT		 first;
	SCREEN_POINT		 last;
	void				*canvas;
	BATTLE_EFFECT_STATE *state;
	Sint32				 frm;
	Sint32				 i;

	canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
	state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	state->args = args;
	btEffectStartCanvasLayer(BGCNT_PRIORITY(2));
	REG_BG2PA = Q_8_8(1);
	REG_BLDALPHA = EFFECT_BLEND_CANVAS(0); /* invisible to start with */
	btProjectionScaledUnit(state->args->tgts[0], &first);
	btProjectionScaledUnit(state->args->tgts[state->args->tgtCnt - 1], &last);
	first.x += (last.x - first.x) / 2;
	REG_BG2X = (SPIDER_WEB_CENTER_X - first.x) << 8;
	btResourceLoadAndDecompress(RESOURCE_SPIDER_WEB_SHEET, state->sheet, 1, 1);
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
	sySndPlayStub(SE_CRACK);
	for(frm = 0; frm != SPIDER_WEB_TICKS; frm++) {
		if(frm <= 8)
			REG_BLDALPHA = EFFECT_BLEND_CANVAS(frm << 1);
		if(frm > 53)
			REG_BLDALPHA = EFFECT_BLEND_CANVAS(124 - (frm << 1));
		EFFECT_LOAD_BLITTER(
			HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, COMPOSITE_TRANSPARENT);
		EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE](canvas, state->sheet,
			SPIDER_WEB_LEFT, SPIDER_WEB_TOP, SPIDER_WEB_QUARTER_WIDTH,
			SPIDER_WEB_QUARTER_HEIGHT);
		syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
		EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
			EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_TRANSPARENT);
		EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE](canvas, state->sheet,
			SPIDER_WEB_CENTER_X, SPIDER_WEB_TOP, SPIDER_WEB_QUARTER_WIDTH,
			SPIDER_WEB_QUARTER_HEIGHT);
		syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
		EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
			EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y, COMPOSITE_TRANSPARENT);
		EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE](canvas, state->sheet,
			SPIDER_WEB_LEFT, SPIDER_WEB_CENTER_Y, SPIDER_WEB_QUARTER_WIDTH,
			SPIDER_WEB_QUARTER_HEIGHT);
		syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
		EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
			EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP, COMPOSITE_TRANSPARENT);
		EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE](canvas, state->sheet,
			SPIDER_WEB_CENTER_X, SPIDER_WEB_CENTER_Y, SPIDER_WEB_QUARTER_WIDTH,
			SPIDER_WEB_QUARTER_HEIGHT);
		syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
		if(frm == 32)
			EFFECT_SHOW_DAMAGE(SE_CRACK);
		EFFECT_FOR_EACH_TARGET(i, state->args) {
			if(frm == 10)
				btEffectTintBattler(
					state->args->tgts[i], SPRITE_COLOR_WHITE, TINT_KEEP, i, 8);
		}
		EFFECT_END_TICK(state);
	}
	syCloseEvent(btEffectTransferCanvas);
	btEffectEndCanvasLayer();
}

#undef SPIDER_WEB_TICKS
#undef SPIDER_WEB_CENTER_X
#undef SPIDER_WEB_CENTER_Y
#undef SPIDER_WEB_LEFT
#undef SPIDER_WEB_TOP
#undef SPIDER_WEB_QUARTER_WIDTH
#undef SPIDER_WEB_QUARTER_HEIGHT

/* The battle background, its palette and modes, the camera's zoom and the
   units' highlights. Part of battle.c. */

#include "battle_internal.h"

/* The value Select picks in the debug value editor. */
#define DEBUG_VALUE_SELECT		WHEN(DEBUG_VALUE_EDITOR_NEW_SELECT_PRESET, 399, 480)
/* The debug value editor's step for up and down; left and right step by one. */
#define DEBUG_VALUE_COARSE_STEP 100

/* The zoomed background's BG2CNT for the first 256 picture tiles: a 256 by 256
   affine map in screen block 7, 256 colours, priority 2. */
#define BACKGROUND_PLANE_UPPER \
	(BGCNT_SIZE(1) | BGCNT_SCREEN_BASE(7) | BGCNT_256_COLORS | \
		BGCNT_CHAR_BASE(2) | BGCNT_PRIORITY(2))
/* The same map, reading the remaining picture tiles from character block 3. */
#define BACKGROUND_PLANE_LOWER \
	(BGCNT_SIZE(1) | BGCNT_SCREEN_BASE(7) | BGCNT_256_COLORS | \
		BGCNT_CHAR_BASE(3) | BGCNT_PRIORITY(2))
/* Lines outside the plane read the padded map in this screen block. */
#define BACKGROUND_PADDED_SCREEN 31
#define BACKGROUND_PLANE_OFF \
	(BGCNT_WRAP | BGCNT_SCREEN_BASE(BACKGROUND_PADDED_SCREEN) | \
		BGCNT_256_COLORS | BGCNT_CHAR_BASE(3) | BGCNT_PRIORITY(2))

/* The screen lines the plane covers, 0 to 135, above the status window. */
#define BACKGROUND_PLANE_LINES		  136
/* The plane's top lines, left off except while commands are chosen. */
#define BACKGROUND_PLANE_HIDDEN_LINES 16
/* The picture's lines that BACKGROUND_PLANE_UPPER reads: its first 256 tiles,
   32 to a row. */
#define BACKGROUND_UPPER_HALF_LINES	  64

/* A battle background resource: its BATTLE_BACKGROUND_COLORS colours, then its
   packed picture. */
#define BACKGROUND_PALETTE_BYTES (BATTLE_BACKGROUND_COLORS * 2)

/* The fixed background's sideways pan in 8.8 pixels while the zoom is below 1;
   from 1 on it moves three 8.8 pixels a step of the camera's yaw, so the
   picture turns with the camera, and is the same when the camera faces the
   party's actions (YAW_PARTY_ACTION). */
#define BACKGROUND_PAN_X Q_8_8(8)

/* The steps of each half of BattleHighlightBlendRamp: the first half plays
   once, then the second over and over. */
#define HIGHLIGHT_RAMP_HALF 16

/* Darkens the background by level, 0 for its own colours. */
void btBackgroundSetPaletteLevel(Sint32 unused, Sint32 level)
{
	BATTLE_STATE   *battle = BattleStateWork;
	Uint16		   *palette = battle->pal;
	volatile Uint32 ime;

	syIRQLock(&ime);

	if(level == 0) {
		syDMAXCopy(palette, BATTLE_BG_PALETTE,
			DMA_COPY_HALFWORDS(BATTLE_BACKGROUND_COLORS), REG_DMA3);
	} else {
		btGraphicsScaleRGB555Clamped(palette, BATTLE_BG_PALETTE,
			battle->brightness = toFIXED(1) - level * BACKGROUND_LEVEL_STEP,
			BATTLE_BACKGROUND_COLORS);
	}
	syIRQUnlock(&ime);
}

/* Switches the background's display mode, optionally fades its palette by fade,
   and rebuilds both its maps. */
void btBackgroundSetMode(Sint32 mode, Sint32 unusedResource, Sint32 fade)
{
	BATTLE_BACKGROUND_STATE *view = BattleBackgroundStateWork;

	if(view->mode == BATTLE_BACKGROUND_NONE) {
		sySetEvent(btBackgroundShowZoomedPage, EVENT_PASS_VBLANK_FIRST);
	}
	view->mode = mode;

	if(mode == BATTLE_BACKGROUND_FIXED) {
		IO_WRITE_QUEUE_PUSH(
			(Uint32)&REG_BG1CNT, BATTLE_BG1CNT, IO_WRITE_HALFWORD);
	}

	/* The battle windows' second text palette takes the first sprite palette's
	   colours, over the window's background colour. */
	syDMAXCopy(OBJ_PLTT, &BG_PLTT_COLOR(BASIC_COLORS_PALETTE, 0),
		DMA_COPY_HALFWORDS(16), REG_DMA3);
	BG_PLTT_COLOR(BASIC_COLORS_PALETTE, TEXT_BACKGROUND_COLOR) =
		BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR);

	if(fade == BATTLE_BACKGROUND_FADE_RESTORE) {
		syDMAXCopy(BattleStateWork->pal, BATTLE_BG_PALETTE,
			DMA_COPY_HALFWORDS(BATTLE_BACKGROUND_COLORS), REG_DMA3);
	} else if(fade != 0) {
		Uint16 *src = BattleStateWork->pal;
		Uint16 *destination = BATTLE_BG_PALETTE;
		Sint32	colIdx;

		for(colIdx = 0; colIdx != BATTLE_BACKGROUND_COLORS; colIdx++) {
			Sint32 red = RGB_RED(src[colIdx]);
			Sint32 green = RGB_GREEN(src[colIdx]);
			Sint32 blue = RGB_BLUE(src[colIdx]);

			if(red > fade) {
				red -= fade;
			} else {
				red = 0;
			}
			if(green > fade) {
				green -= fade;
			} else {
				green = 0;
			}
			if(blue > fade) {
				blue -= fade;
			} else {
				blue = 0;
			}
			destination[colIdx] = RGB(red, green, blue);
		}
	}

	btTransitionBuildAffineTilemap(BG_SCREEN_BLOCK(7));
	btTransitionBuildFlatTilemap(BG_SCREEN_BLOCK(BACKGROUND_PADDED_SCREEN));
}

/* Allocates the zoomed background's scanline state. */
void btBackgroundInit(void)
{
	BATTLE_AFFINE_HDMA		*scanlines;
	BATTLE_BACKGROUND_STATE *view;
	scanlines =
		syHeapSlotAllocEWRAM(HEAP_SLOT_BATTLE_SCANLINES, sizeof *scanlines);
	view = BattleBackgroundStateWork;
	DMA_FILL32(0, scanlines, sizeof *scanlines);
	view->mode = BATTLE_BACKGROUND_NONE;
}

/* Frees the zoomed background's scanline state. */
void btBackgroundFree(void)
{
	syHeapSlotRelease(HEAP_SLOT_BATTLE_SCANLINES);
}

/* The bit decoder btBackgroundLoad copies to RAM and runs. It returns the
   source after the last halfword it read. */
typedef const Uint8 *(*BIT_DECODER)(const Uint8 *source, void *destination);

/* Loads a battle background, its picture, its palette darkened by level (none
   when negative) and both its maps. */
void btBackgroundLoad(Sint32 mode, Sint32 resourceId, Sint32 level)
{
	BATTLE_BACKGROUND_STATE *view =
		HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_BACKGROUND);
	Uint8		 *backgroundData = syResourceGetTableEntry(resourceId);
	BATTLE_STATE *battle = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE);
	Uint16		 *palette;

	RAM_CODE_LOAD_SLOT(
		HEAP_SLOT_RAM_CODE_TEMP, btBitDecoderDecodeImage, BitDecoder_Size);
	((BIT_DECODER)HEAP_SLOT_BLOCK(HEAP_SLOT_RAM_CODE_TEMP))(
		backgroundData + BACKGROUND_PALETTE_BYTES, BG_CHAR_BLOCK(2));
	syHeapSlotRelease(HEAP_SLOT_RAM_CODE_TEMP);
	palette = battle->pal;
	syDMAXCopy(backgroundData, palette,
		DMA_COPY_WORDS(BACKGROUND_PALETTE_BYTES / 4), REG_DMA3);
	if(level >= 0)
		btGraphicsScaleRGB555Clamped(palette, BATTLE_BG_PALETTE,
			battle->brightness = toFIXED(1) - level * BACKGROUND_LEVEL_STEP,
			BATTLE_BACKGROUND_COLORS);
	syDMAXCopy(OBJ_PLTT, &BG_PLTT_COLOR(BASIC_COLORS_PALETTE, 0),
		DMA_COPY_HALFWORDS(16), REG_DMA3);
	BG_PLTT_COLOR(BASIC_COLORS_PALETTE, TEXT_BACKGROUND_COLOR) =
		BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR);
	btTransitionBuildAffineTilemap(BG_SCREEN_BLOCK(7));
	btTransitionBuildFlatTilemap(BG_SCREEN_BLOCK(BACKGROUND_PADDED_SCREEN));
	/* BG1's last row is left clear. */
	syWordsClear((BG_SCREEN_BLOCK(BACKGROUND_PADDED_SCREEN) + BG_SCREEN_SIZE -
					 BG_SCREEN_WIDTH * 2),
		BG_SCREEN_WIDTH * 2);
	if(view->mode == BATTLE_BACKGROUND_NONE)
		sySetEvent(btBackgroundShowZoomedPage, EVENT_PASS_VBLANK_FIRST);
	view->mode = mode;
	if(mode == BATTLE_BACKGROUND_FIXED)
		REG_BG1CNT = BATTLE_BG1CNT;
}

/* Builds the zoomed background's next page for a zoom of scale about centerX
   and centerY, shifted by ofsX and ofsY, and shows it: below a zoom of 1
   the plane lies flat, from 1 it wraps and turns with the camera. */
void btCameraSetRange(
	FIXED centerX, FIXED centerY, Sint32 ofsX, Sint32 ofsY, FIXED scale)
{
	BATTLE_BACKGROUND_STATE *view =
		HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_BACKGROUND);
	BATTLE_AFFINE_HDMA *scanlines = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SCANLINES);
	BATTLE_CAMERA	   *camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
	Uint16				wrap;
	Sint32				panX;
	Uint16			   *line;
	BATTLE_AFFINE	   *affine;
	FIXED				ratio;
	Sint32				step;
	FIXED				x;
	Sint32				y;
	Uint32				scanline;
	Uint32				lowerHalfLine;

	panX = BACKGROUND_PAN_X;
	wrap = 0;
	if(scale >= toFIXED(1)) {
		wrap = BGCNT_WRAP;
		panX = BACKGROUND_PAN_X + YAW_PARTY_ACTION * 3 + -camera->yaw * 3;
	}
	if(scanlines == NULL)
		return;
	if((view->mode == BATTLE_BACKGROUND_FIXED ||
		   view->secondMode == BATTLE_BACKGROUND_FIXED) &&
		view->busy == 0)
		BgScrollWork[1].x = panX >> 8;
	if(view->mode != BATTLE_BACKGROUND_ZOOMED)
		return;

	/* The affine parameters: a plain zoom, the reference point moved so the
	   centre stays put and the picture's top row is the first line below the
	   plane's hidden lines. */
	line = scanlines->lines[scanlines->page ^ 1];
	ratio = syDivFX(scale, toFIXED(1));
	step = ratio >> 8;
	affine = &scanlines->affine;
	affine->pa = step;
	affine->pb = 0;
	affine->pc = 0;
	affine->pd = step;
	x = syMulFX(ratio, syMulFX(centerX, scale - toFIXED(1)));
	y = syMulFX(ratio, syMulFX(centerY, scale - toFIXED(1)));
	affine->x = ((x + toFIXED(0.5) - 1) >> 8) + ofsX + panX;
	y = ((y + toFIXED(0.5) - 1) >> 8) + ofsY -
		Q_8_8(BACKGROUND_PLANE_HIDDEN_LINES);
	affine->y = y;

	/* The first line that reaches the picture's lower half at this zoom. */
	lowerHalfLine = FIXEDtoINT(syDivFX(
						(Sint16)step, (BACKGROUND_UPPER_HALF_LINES << 8) - y)) +
		1;

	scanline = 0;
	if(!gmFlagGetStub(FLAG_CHOOSING_COMMANDS)) {
		for(; scanline < BACKGROUND_PLANE_HIDDEN_LINES; scanline++)
			*line++ = BACKGROUND_PLANE_OFF;
	}
	if(lowerHalfLine > BACKGROUND_PLANE_LINES)
		lowerHalfLine = BACKGROUND_PLANE_LINES;
	for(; scanline < lowerHalfLine; scanline++)
		*line++ = wrap | BACKGROUND_PLANE_UPPER;
	for(; scanline < BACKGROUND_PLANE_LINES; scanline++)
		*line++ = wrap | BACKGROUND_PLANE_LOWER;
	for(; scanline < DISPLAY_HEIGHT; scanline++)
		*line++ = BACKGROUND_PLANE_OFF;
	scanlines->page ^= 1;
}

/* Unused: btCameraSetView with the eye at the camera's own distance rather than
   BATTLE_CAMERA_DISTANCE, and no zoom taken; written as a call to it, the eye
   would stay at BATTLE_CAMERA_DISTANCE. */
#if FIX_UNINITIALIZED_READ
UNUSED void btCameraSetViewAtDepth(
	Sint32 x, Sint32 depth, Sint32 y, Sint32 zoom)
{
#else
UNUSED void btCameraSetViewAtDepth(Sint32 x, Sint32 depth, Sint32 y)
{
#endif
	FIXED distance = BATTLE_CAMERA_DISTANCE;
	/* UB: zoom is never set, so the scale is whatever it happens to hold; UBFIX
	   takes it as a parameter. */
#if !FIX_UNINITIALIZED_READ
	Sint32 zoom;
#endif
	BATTLE_CAMERA *camera = BattleCameraWork;
	Sint32		  *focus = camera->focus;
	FIXED		   scale = toFIXED(zoom) / 100;
	Sint32		   eyeOfs[3];
	Sint32		   originOnScreen[3];
	Sint32		   origin[3];
	FIXED		   nearShare;
	FIXED		   halfFocal;
	FIXED		   focal;
	Uint32		   nearDepth;

	SET_VECTOR(focus, x, depth, y);
	origin[0] = 0;
	nearShare = toFIXED(3) / 4;
	origin[1] = 0;
	origin[2] = 0;

	nearDepth = syDivFX(distance, nearShare);
	sySetScreenDist(distance, nearDepth, distance * 2);
	syInitMatrix();
	syMatrixTranslateVector(focus);
	syRotY(camera->yaw);
	syRotX(camera->pitch);
	eyeOfs[0] = 0;
	eyeOfs[1] = 0;
	eyeOfs[2] = camera->distance;
	syCalcPoint(eyeOfs, (Sint32 *)camera);
	ProjectionWork.centerX = BATTLE_PROJECTION_CENTER_X;
	ProjectionWork.centerY = BATTLE_PROJECTION_CENTER_Y;
	syInitMatrix();
	syLookAtLoad(camera->eye, focus);
	syConvert3Dto2D(origin, originOnScreen);

	btCameraSetRange(toFIXED(BATTLE_PROJECTION_CENTER_X),
		toFIXED(BATTLE_PROJECTION_CENTER_Y),
		(BATTLE_PROJECTION_CENTER_X - originOnScreen[0]) << 8,
		(BATTLE_PROJECTION_CENTER_Y - originOnScreen[1]) << 8, scale);
	halfFocal = scale * (FIXEDtoINT(BATTLE_CAMERA_DISTANCE) / 2);
	focal = halfFocal * 2;
	nearDepth = syDivFX(focal, nearShare);
	sySetScreenDist(focal, nearDepth, halfFocal * 4);
}

/* Points the battle camera at (x, depth, y) with a zoom in percent, and lines
   the zoomed background up with it. */
void btCameraSetView(Sint32 x, Sint32 depth, Sint32 y, Sint32 zoom)
{
	BATTLE_CAMERA *camera = BattleCameraWork;
	Sint32		  *focus = camera->focus;
	FIXED		   scale = toFIXED(zoom) / 100;
	FIXED		   eyeOfs[3];
	Sint32		   originOnScreen[3];
	Sint32		   origin[3];
	FIXED		   distance = BATTLE_CAMERA_DISTANCE;
	FIXED		   nearShare;
	FIXED		   halfFocal;
	FIXED		   focal;
	Uint32		   nearDepth;

	SET_VECTOR(focus, x, depth, y);
	origin[0] = 0;
	nearShare = toFIXED(3) / 4;
	origin[1] = 0;
	origin[2] = 0;

	/* The eye BATTLE_CAMERA_DISTANCE back from the focus. */
	nearDepth = syDivFX(distance, nearShare);
	sySetScreenDist(distance, nearDepth, distance * 2);
	syInitMatrix();
	syMatrixTranslateVector(focus);
	syRotY(camera->yaw);
	syRotX(camera->pitch);
	eyeOfs[0] = 0;
	eyeOfs[1] = 0;
	eyeOfs[2] = distance;
	syCalcPoint(eyeOfs, (Sint32 *)camera);
	/* Where the battlefield's origin falls on the screen moves the plane as
	   much. */
	ProjectionWork.centerX = BATTLE_PROJECTION_CENTER_X;
	ProjectionWork.centerY = BATTLE_PROJECTION_CENTER_Y;
	syInitMatrix();
	syLookAtLoad(camera->eye, focus);
	syConvert3Dto2D(origin, originOnScreen);

	btCameraSetRange(toFIXED(BATTLE_PROJECTION_CENTER_X),
		toFIXED(BATTLE_PROJECTION_CENTER_Y),
		(BATTLE_PROJECTION_CENTER_X - originOnScreen[0]) << 8,
		(BATTLE_PROJECTION_CENTER_Y - originOnScreen[1]) << 8, scale);
	/* The zoom scales the focal length and both depths. */
	halfFocal = scale * (FIXEDtoINT(BATTLE_CAMERA_DISTANCE) / 2);
	focal = halfFocal * 2;
	nearDepth = syDivFX(focal, nearShare);
	sySetScreenDist(focal, nearDepth, halfFocal * 4);
}

/* Points the battle camera at the ground between two units, with a zoom in
   percent. */
void btCameraSetViewAtPairMidpoint(
	Sint32 firstUnitId, Sint32 secondUnitId, Sint32 zoom)
{
	ACTOR *firstActor = btObjectGet(firstUnitId)->actor;
	ACTOR *secondActor = btObjectGet(secondUnitId)->actor;
	FIXED  firstX = firstActor->x.fixed;
	FIXED  secondX = secondActor->x.fixed;
	FIXED  firstZ = firstActor->z.fixed;
	FIXED  secondZ = secondActor->z.fixed;
	FIXED  x = (secondX + firstX) / 2;
	FIXED  z = (secondZ + firstZ) / 2;

	btCameraSetView(x, 0, z, zoom);
}

/* Fades BG2 out over the backdrop in eight ticks. */
void btBackgroundFadeAlphaOut(void)
{
	Sint32 alphaStep;

	REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BD;
	for(alphaStep = 1; alphaStep <= BLEND_WEIGHT_MAX; alphaStep += 2) {
		REG_BLDALPHA =
			BLEND_ALPHA(BLEND_WEIGHT_MAX, BLEND_WEIGHT_MAX) - alphaStep;
		sySynch(1);
	}
}

/* Fades BG2 in over the backdrop in eight ticks. */
void btBackgroundFadeAlphaIn(void)
{
	Sint32 step;

	REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BD;
	for(step = 1; step <= BLEND_WEIGHT_MAX; step += 2) {
		REG_BLDALPHA = step + BLEND_ALPHA(0, BLEND_WEIGHT_MAX);
		sySynch(1);
	}
}

/* What btTransitionInitEntries treats as an array of these; syUnitMatrix writes
   over it as a matrix. */
typedef struct BATTLE_TRANSITION_ENTRY {
	/* the first word, read before and after the identity is set */
	Uint32 value;
	Uint32 sum;		/* set to the first word's old value plus its new one */
	Uint32 rest[2]; /* never read */
} BATTLE_TRANSITION_ENTRY;

/* Unused: lightens every layer by REG_BLDY. */
UNUSED void btBackgroundSetBlendCtrl(void)
{
	REG_BLDCNT = BLDCNT_TGT1_ALL | BLDCNT_EFFECT_LIGHTEN;
}

/* Unused: sets entries to the identity transform, then stores the first word's
   old value plus its new one in the second. */
UNUSED void btTransitionInitEntries(BATTLE_TRANSITION_ENTRY *entries)
{
	/* XXX (no evidence): what the sum is for; nothing calls this in either
	   game. */
	Uint32 prev = entries[0].value;

	syUnitMatrix((FIXED *)entries);
	entries[0].sum = prev + entries[0].value;
}

/* Unused: returns value divided by 16. */
UNUSED Sint32 btBackgroundDivideBy16(Sint32 value)
{
	return value / 16;
}

/* Unused debug aid: while Start is held, edits a value with the d-pad until A
   takes it; with Select held, returns DEBUG_VALUE_SELECT. */
UNUSED Sint32 btAdjustDebugValue(Sint32 selectedValue)
{
	BATTLE_STATE *battle;

	if(PerData & KEY_START) {
		for(;;) {
			battle = BattleStateWork;
			if(PerRepeat & KEY_LEFT)
				battle->debugValue--;
			if(PerRepeat & KEY_RIGHT)
				battle->debugValue++;
			if(PerRepeat & KEY_UP)
				battle->debugValue -= DEBUG_VALUE_COARSE_STEP;
			if(PerRepeat & KEY_DOWN)
				battle->debugValue += DEBUG_VALUE_COARSE_STEP;
			if(PerPush & KEY_A)
				selectedValue = battle->debugValue;
			else {
				sySynch(1);
				continue;
			}
			break;
		}
	}
	if(PerData & KEY_SELECT)
		selectedValue = DEBUG_VALUE_SELECT;
	return selectedValue;
}

/* Sets the sprite mode of a unit's picture and its shadow. */
void btHighlightSetSprMode(Sint32 unitId, Sint32 mode)
{
	BATTLE_OBJECT *slot = btObjectGet(unitId);
	ACTOR		  *actor;
	SPR			  *spr;
	SPR			 **sprs;
	Sint32		   sprIdx;

	if(slot == 0)
		return;
	actor = slot->actor;
	if(actor == 0)
		return;
	switch(actor->sprStorageKind & ACTOR_SPRITE_KIND_MASK) {
	case ACTOR_SPRITE_ONE: {
		spr = actor->spr;
		spr->image.objMode = mode;
		spr->shadow.objMode = mode;
		break;
	}
	case ACTOR_SPRITE_GROUP: {
		sprs = (SPR **)actor->spr;
		sprIdx = 0;
		do {
			spr = *sprs++;
			if(spr == 0)
				break;
			spr->image.objMode = mode;
			spr->shadow.objMode = mode;
		} while(++sprIdx < ACTOR_SPRITE_GROUP_SIZE);
		break;
	}
	}
}

/* Unused: makes every unit's sprites semi-transparent but selectedUnitId's. */
UNUSED void btHighlightSetOtherSprModes(Sint32 selectedUnitId)
{
	Sint16 unitIds[UNIT_MAX];
	Sint32 unitCnt = btUnitListIds(BATTLE_SIDE_MASK_BOTH, unitIds);
	Sint32 unitIdx;

	for(unitIdx = 0; unitIdx < unitCnt; unitIdx++) {
		if(unitIds[unitIdx] != selectedUnitId)
			btHighlightSetSprMode(unitIds[unitIdx], SPRITE_SEMI_TRANSPARENT);
	}
}

/* Returns every unit's sprites to SPRITE_OPAQUE. */
void btHighlightClearAllSprModes(void)
{
	Sint16 unitIds[UNIT_MAX];
	Sint32 unitCnt = btUnitListIds(BATTLE_SIDE_MASK_BOTH, unitIds);
	Sint32 unitIdx;

	for(unitIdx = 0; unitIdx < unitCnt; unitIdx++)
		btHighlightSetSprMode(unitIds[unitIdx], SPRITE_OPAQUE);
}

/* Lightens the highlighted units by the next step of the blend ramp: the first
   sixteen steps once, then the second sixteen over and over. */
void btBackgroundStepHighlightBlend(void)
{
	BATTLE_STATE *battle = BattleStateWork;
	Sint8		 *blendRamp;
	Uint16		  rampIdx;
	Sint32		  nextIdx;

	if(battle != NULL && battle->highlightMode != HIGHLIGHT_OFF) {
		REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_EFFECT_LIGHTEN | BLDCNT_TGT2_ALL;
		REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX, 0);
		blendRamp = BattleHighlightBlendRamp;
		REG_BLDY = blendRamp[battle->highlightStep];
		/* From the first half's last step on, the next step stays in the
		   second. */
		rampIdx = battle->highlightStep;
		nextIdx = (rampIdx + 1) & (HIGHLIGHT_RAMP_HALF - 1);
		if(rampIdx >= HIGHLIGHT_RAMP_HALF - 1)
			nextIdx |= HIGHLIGHT_RAMP_HALF;
		battle->highlightStep = nextIdx;
	}
}

/* Highlights the units in selectedUnits, the others turning semi-transparent,
   or with HIGHLIGHT_OFF stops the highlight. */
void btHighlightSetModes(Uint16 *selectedUnits, Sint32 mode)
{
	Sint16		  unitIds[UNIT_MAX];
	BATTLE_STATE *battle = BattleStateWork;
	Uint32		  unitCnt;
	Uint32		  unitIdx;

	if(mode == HIGHLIGHT_OFF) {
		syCloseEvent(btBackgroundStepHighlightBlend);
		REG_BLDY = mode;
		btHighlightClearAllSprModes();
		sySynch(1);
		syIOWriteQueueStore16((Uint32)&REG_BLDCNT, 0);
	}
	if(battle != 0 && mode != HIGHLIGHT_OFF) {
		battle->highlightMode = mode;
		battle->highlightStep = 0;
		REG_BLDY = 0;
		IO_WRITE16(BLEND_ALPHA(BLEND_WEIGHT_MAX, 0), &REG_BLDALPHA);

		unitCnt = btUnitListIds(BATTLE_SIDE_MASK_BOTH, unitIds);
		for(unitIdx = 0; unitIdx < unitCnt; unitIdx++)
			btHighlightSetSprMode(unitIds[unitIdx], mode & 1);

		if(selectedUnits != 0) {
			Uint32 unitId;

			for(unitIdx = 0; unitIdx < UNIT_MAX; unitIdx++) {
				unitId = *selectedUnits++;
				if(unitId == BATTLE_UNIT_LIST_END)
					break;
				btHighlightSetSprMode(unitId, (mode & 1) ^ 1);
			}
		}
		sySynch(1);
		syIOWriteQueueStore16((Uint32)&REG_BLDCNT, 0);
		sySetEvent(btBackgroundStepHighlightBlend, EVENT_PASS_VBLANK);
	}
}

#undef BACKGROUND_PLANE_LINES
#undef BACKGROUND_PLANE_HIDDEN_LINES
#undef BACKGROUND_PLANE_UPPER
#undef BACKGROUND_PLANE_LOWER
#undef BACKGROUND_PLANE_OFF
#undef BACKGROUND_UPPER_HALF_LINES
#undef BACKGROUND_PALETTE_BYTES
#undef BACKGROUND_PAN_X
#undef HIGHLIGHT_RAMP_HALF
#undef DEBUG_VALUE_SELECT
#undef DEBUG_VALUE_COARSE_STEP

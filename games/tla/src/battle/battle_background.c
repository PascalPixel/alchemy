/* The battle background, its palette and modes, the camera's zoom and the
   units' highlights, and a unit's element glow: the star field of a cast, the
   streaks, the pillar and the burst. Part of battle.c. */

#include "battle/battle_effect_state.h"

/* The zoomed background's BG2CNT for the first 256 picture tiles: a 256 by 256
   affine map in screen block 7, 256 colours, priority 2. */
#define BACKGROUND_PLANE_UPPER \
	(BGCNT_SIZE(1) | BGCNT_SCREEN_BASE(7) | BGCNT_256_COLORS | \
		BGCNT_CHAR_BASE(2) | BGCNT_PRIORITY(2))
/* The same map, reading the remaining picture tiles from character block 3. */
#define BACKGROUND_PLANE_LOWER \
	(BGCNT_SIZE(1) | BGCNT_SCREEN_BASE(7) | BGCNT_256_COLORS | \
		BGCNT_CHAR_BASE(3) | BGCNT_PRIORITY(2))
#define BACKGROUND_PLANE_OFF \
	(BGCNT_WRAP | BGCNT_SCREEN_BASE(BATTLE_BG1_SCREEN_BLOCK) | \
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

/* Darkens the battle background's palette by level steps of a sixtieth; level 0
   restores its saved colours. */
void btBackgroundSetPaletteLevel(Sint32 unused, Sint32 level)
{
	BATTLE_STATE   *battle = RamHeapSlots->battleState;
	Uint16		   *palette = battle->pal;
	volatile Uint32 ime;

	IRQ_LOCK(ime);

	if(level == 0) {
		syDMAXCopy(palette, (void *)BATTLE_BG_PALETTE,
			DMA_COPY_HALFWORDS(BATTLE_BACKGROUND_COLORS), REG_DMA3);
	} else {
		btColBufferScale(palette, BATTLE_BG_PALETTE,
			battle->brightness = toFIXED(1) - level * BACKGROUND_LEVEL_STEP,
			BATTLE_BACKGROUND_COLORS);
	}
	syIRQUnlock(&ime);
}

/* Switches the battle background's display mode and rebuilds both its maps. A
   nonzero fade darkens the saved palette by taking fade from each colour
   channel, and BATTLE_BACKGROUND_FADE_RESTORE puts it back undarkened; the
   background's id is not read. */
void btBackgroundSetMode(Sint32 mode, Sint32 backgroundId, Sint32 fade)
{
	BATTLE_BACKGROUND_STATE *background = RamHeapSlots->battleBg;

	if(background->mode == BATTLE_BACKGROUND_NONE)
		sySetEvent(btBackgroundShowZoomedPage, EVENT_PASS_VBLANK + 127);
	background->mode = mode;

	if(mode == BATTLE_BACKGROUND_FIXED)
		IO_WRITE_QUEUE_PUSH(
			(Uint32)&REG_BG1CNT, BATTLE_BG1CNT, IO_WRITE_HALFWORD);

	/* The battle windows' second text palette takes the first sprite palette's
	   colours, over the window's background colour. */
	syDMAXCopy((void *)OBJ_PLTT,
		(void *)&BG_PLTT_COLOR(BASIC_COLORS_PALETTE, 0), DMA_COPY_HALFWORDS(16),
		REG_DMA3);
	BG_PLTT_COLOR(BASIC_COLORS_PALETTE, TEXT_BACKGROUND_COLOR) =
		BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR);

	if(fade == BATTLE_BACKGROUND_FADE_RESTORE) {
		BATTLE_STATE *battle;
		Uint16		 *palette;

		battle = RamHeapSlots->battleState;
		palette = battle->pal;
		syDMAXCopy(palette, (void *)BATTLE_BG_PALETTE,
			DMA_COPY_HALFWORDS(BATTLE_BACKGROUND_COLORS), REG_DMA3);
	} else if(fade != 0) {
		BATTLE_STATE *battle;
		Uint16		 *src;
		Uint16		 *dst;
		Sint32		  i;

		battle = RamHeapSlots->battleState;
		src = battle->pal;
		dst = BATTLE_BG_PALETTE;

		for(i = 0; i != BATTLE_BACKGROUND_COLORS; i++) {
			Sint32 red = RGB_RED(src[i]);
			Sint32 green = RGB_GREEN(src[i]);
			Sint32 blue = RGB_BLUE(src[i]);

			if(red > fade)
				red -= fade;
			else
				red = 0;
			if(green > fade)
				green -= fade;
			else
				green = 0;
			if(blue > fade)
				blue -= fade;
			else
				blue = 0;
			dst[i] = RGB(red, green, blue);
		}
	}
	btTransitionBuildAffineTilemap((void *)BG_SCREEN_BLOCK(7));
	btTransitionBuildFlatTilemap(
		(void *)BG_SCREEN_BLOCK(BATTLE_BG1_SCREEN_BLOCK));
}

/* Allocates the zoomed background's scanline state. */
void btBackgroundInit(void)
{
	void *block = syHeapSlotAllocEWRAM(
		HEAP_SLOT_BATTLE_SCANLINES, sizeof(BATTLE_AFFINE_HDMA));
	BATTLE_BACKGROUND_STATE *background = RamHeapSlots->battleBg;

	IWRAM_FILL_WORDS(block, sizeof(BATTLE_AFFINE_HDMA), 0);
	background->mode = BATTLE_BACKGROUND_NONE;
}

#include "battle/background_free.c"

/* Loads a battle background: its picture decoded into BG VRAM, its palette
   darkened by level steps of 1/60 (none when level is negative), its tile table
   and tilemap, and the HBlank scroll the first time. */
void btBackgroundLoad(Sint32 mode, Sint32 resource, Sint32 level)
{
	BATTLE_BACKGROUND_STATE *background = RamHeapSlots->battleBg;
	Uint8					*data = syResourceGetTableEntry(resource);
	BATTLE_STATE			*battle = (BATTLE_STATE *)RamHeapSlots->battleState;
	MODEL_DECODER			 decode;
	Uint16					*palette;

	RAM_CODE_LOAD(decode, btModelDecodeCode, ModelDecodeCodeSize);
	decode(data + BACKGROUND_PALETTE_BYTES, (void *)BG_CHAR_BLOCK(2));
	syHeapRelease(decode);
	palette = battle->pal;
	syDMAXCopy(
		data, palette, DMA_COPY_WORDS(BACKGROUND_PALETTE_BYTES / 4), REG_DMA3);
	if(level >= 0)
		btColBufferScale(palette, BATTLE_BG_PALETTE,
			battle->brightness = toFIXED(1) - level * BACKGROUND_LEVEL_STEP,
			BATTLE_BACKGROUND_COLORS);
	if(battle->keepBackdropPal == 0) {
		syDMAXCopy((void *)OBJ_PLTT,
			(void *)&BG_PLTT_COLOR(BASIC_COLORS_PALETTE, 0),
			DMA_COPY_HALFWORDS(16), REG_DMA3);
		BG_PLTT_COLOR(BASIC_COLORS_PALETTE, TEXT_BACKGROUND_COLOR) =
			BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR);
	}
	btTransitionBuildAffineTilemap((void *)BG_SCREEN_BLOCK(7));
	btTransitionBuildFlatTilemap(
		(void *)BG_SCREEN_BLOCK(BATTLE_BG1_SCREEN_BLOCK));
	/* BG1's last row is left clear. */
	syWordsClearEntry((void *)(BG_SCREEN_BLOCK(BATTLE_BG1_SCREEN_BLOCK) +
						  BG_SCREEN_SIZE - BG_SCREEN_WIDTH * 2),
		BG_SCREEN_WIDTH * 2);
	if(background->mode == BATTLE_BACKGROUND_NONE)
		sySetEvent(btBackgroundShowZoomedPage, EVENT_PASS_VBLANK + 127);
	background->mode = mode;
	if(mode == BATTLE_BACKGROUND_FIXED)
		REG_BG1CNT = BATTLE_BG1CNT;
}

/* Builds the zoomed background's next page for a zoom of scale about centerX
   and centerY, shifted by ofsX and ofsY, and shows it: below a zoom of 1
   the plane lies flat, from 1 it wraps and turns with the camera. */
void btCameraSetRange(
	FIXED centerX, FIXED centerY, Sint32 ofsX, Sint32 ofsY, FIXED scale)
{
	BATTLE_BACKGROUND_STATE *background = RamHeapSlots->battleBg;
	BATTLE_AFFINE_HDMA		*scanlines = RamHeapSlots->battleScanlines;
	BATTLE_CAMERA			*camera = RamHeapSlots->cameraState;
	Sint32					 panX;
	Uint16					 wrap;
	Uint16					*line;
	BATTLE_AFFINE			*affine;
	FIXED					 ratio;
	Sint32					 step;
	FIXED					 x;
	FIXED					 y;
	Uint32					 scanline;
	Uint32					 lowerHalfLine;

	panX = BACKGROUND_PAN_X;
	wrap = 0;
	ratio = toFIXED(1);
	if(scale >= ratio) {
		wrap = BGCNT_WRAP;
		panX = BACKGROUND_PAN_X + YAW_PARTY_ACTION * 3 + -camera->yaw * 3;
	}
	if(scanlines == NULL)
		return;
	if((background->mode == BATTLE_BACKGROUND_FIXED ||
		   background->bg1ScrollMode == BATTLE_BACKGROUND_FIXED) &&
		background->busy == 0)
		BgScrollWork[1].x = panX >> 8;
	if(background->mode != BATTLE_BACKGROUND_ZOOMED)
		return;

	/* The affine parameters: a plain zoom, the reference point moved so the
	   centre stays put and the picture's top row is the first line below the
	   plane's hidden lines. */
	line = scanlines->lines[scanlines->page ^ 1];
	ratio = syDivFX(scale, ratio);
	step = ratio >> 8;
	affine = &scanlines->affine;
	affine->pa = step;
	affine->pb = 0;
	affine->pc = 0;
	affine->pd = step;
	x = syMulFX(ratio, syMulFX(centerX, scale - toFIXED(1)));
	y = syMulFX(ratio, syMulFX(centerY, scale - toFIXED(1)));
	affine->x = ((x + toFIXED(0.5) - 1) >> 8) + ofsX + panX;
	affine->y = ((y + toFIXED(0.5) - 1) >> 8) + ofsY -
		Q_8_8(BACKGROUND_PLANE_HIDDEN_LINES);
	/* The first line that reaches the picture's lower half at this zoom. */
	lowerHalfLine = FIXEDtoINT(syDivFX((Sint16)affine->pd,
						(BACKGROUND_UPPER_HALF_LINES << 8) - affine->y)) +
		1;
	scanline = 0;
	if(!syGameFlagGet(FLAG_CHOOSING_COMMANDS)) {
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

/* Unused: sets the camera's eye at its stored distance without taking a
   zoom. */
#if FIX_UNINITIALIZED_READ
UNUSED void btCameraSetViewAtDepth(
	Sint32 x, Sint32 depth, Sint32 y, Sint32 zoom)
{
#else
UNUSED void btCameraSetViewAtDepth(Sint32 x, Sint32 depth, Sint32 y)
{
#endif
	FIXED focal = BATTLE_CAMERA_DISTANCE;
	/* UB: zoom is never set, so the scale is whatever its register held; UBFIX
	   takes it as a parameter. */
#if !FIX_UNINITIALIZED_READ
	Sint32 zoom;
#endif
	BATTLE_CAMERA *camera = RamHeapSlots->cameraState;
	Sint32		  *focus = camera->focus;
	FIXED		   scale = toFIXED(zoom) / 100;
	Sint32		   eyeOfs[3];
	Sint32		   originOnScreen[3];
	Sint32		   origin[3];
	FIXED		   nearShare;
	FIXED		   halfFocal;
	FIXED		   zoomedFocal;
	Uint32		   nearDepth;

	focus[0] = x;
	focus[1] = depth;
	focus[2] = y;
	origin[0] = 0;
	nearShare = toFIXED(3) / 4;
	origin[1] = 0;
	origin[2] = 0;

	nearDepth = syDivFX(focal, nearShare);
	sySetScreenDist(focal, nearDepth, focal * 2);
	syInitMatrix();
	syMatrixTranslateVector(focus);
	syRotY(camera->yaw);
	syRotX(camera->pitch);
	eyeOfs[0] = 0;
	eyeOfs[1] = 0;
	eyeOfs[2] = camera->distance;
	syCalcPoint(eyeOfs, camera->eye);
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
	zoomedFocal = halfFocal * 2;
	nearDepth = syDivFX(zoomedFocal, nearShare);
	sySetScreenDist(zoomedFocal, nearDepth, halfFocal * 4);
}

/* Points the battle camera at (x, depth, y) with a zoom in percent, and lines
   the zoomed background up with it: where the battlefield's origin falls on the
   screen moves the plane as much, and the zoom scales the focal length and the
   far depth. */
void btCameraSetView(Sint32 x, Sint32 depth, Sint32 y, Sint32 zoom)
{
	BATTLE_CAMERA *camera = RamHeapSlots->cameraState;
	Sint32		  *focus = camera->focus;
	FIXED		   scale = toFIXED(zoom) / 100;
	FIXED		   eyeOfs[3];
	Sint32		   originOnScreen[3];
	Sint32		   origin[3];
	FIXED		   focal = BATTLE_CAMERA_DISTANCE;
	FIXED		   nearShare;
	FIXED		   halfFocal;
	FIXED		   zoomedFocal;
	Uint32		   nearDepth;

	focus[0] = x;
	focus[1] = depth;
	focus[2] = y;
	origin[0] = 0;
	nearShare = toFIXED(3) / 4;
	origin[1] = 0;
	origin[2] = 0;

	nearDepth = syDivFX(focal, nearShare);
	sySetScreenDist(focal, nearDepth, focal * 2);
	syInitMatrix();
	syMatrixTranslateVector(focus);
	syRotY(camera->yaw);
	syRotX(camera->pitch);
	eyeOfs[0] = 0;
	eyeOfs[1] = 0;
	eyeOfs[2] = focal;
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
	zoomedFocal = halfFocal * 2;
	nearDepth = syDivFX(zoomedFocal, nearShare);
	sySetScreenDist(zoomedFocal, nearDepth, halfFocal * 4);
}

/* Points the battle camera at the ground between two units, with a zoom in
   percent. */
void btCameraSetViewAtPairMidpoint(
	Sint32 firstUnitId, Sint32 secondUnitId, Sint32 zoom)
{
	MOTION_OBJECT *first = btObjectGet(firstUnitId)->obj;
	MOTION_OBJECT *second = btObjectGet(secondUnitId)->obj;
	FIXED		   firstX = first->x;
	FIXED		   secondX = second->x;
	FIXED		   firstZ = first->z;
	FIXED		   secondZ = second->z;
	FIXED		   x = (secondX + firstX) / 2;
	FIXED		   z = (secondZ + firstZ) / 2;

	btCameraSetView(x, 0, z, zoom);
}

/* Fades BG2 out over the backdrop in eight ticks, its blend weight falling by
   2 a tick from 15, while the backdrop's stays 16. Leaves REG_BLDCNT blending
   BG2 over the backdrop. */
void btBackgroundFadeAlphaOut(void)
{
	Sint32 step;

	REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BD;
	for(step = 1; step <= BLEND_WEIGHT_MAX; step += 2) {
		REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX, BLEND_WEIGHT_MAX) - step;
		sySynch(1);
	}
}

/* Fades BG2 in over the backdrop in eight ticks, its blend weight rising by 2
   a tick from 1, while the backdrop's stays 16. Leaves REG_BLDCNT blending BG2
   over the backdrop. */
void btBackgroundFadeAlphaIn(void)
{
	Sint32 step;

	REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BD;
	for(step = 1; step <= BLEND_WEIGHT_MAX; step += 2) {
		REG_BLDALPHA = step + BLEND_ALPHA(0, BLEND_WEIGHT_MAX);
		sySynch(1);
	}
}

/* Unused: sets BLDCNT to brighten every layer and the backdrop. */
UNUSED void btBrightenAllLayers(void)
{
	REG_BLDCNT = BLDCNT_TGT1_ALL | BLDCNT_EFFECT_LIGHTEN;
}

/* Unused: returns the 16.16 product of a and b. */
UNUSED FIXED btMulFX(FIXED a, FIXED b)
{
	return syMulFX(a, b);
}

/* What btTransitionInitEntries treats as an array of these; syUnitMatrix writes
   over it as a matrix. */
struct BATTLE_TRANSITION_ENTRY {
	/* the first word, read before and after the identity is set */
	Uint32 value;
	Uint32 sum;		/* set to the first word's old value plus its new one */
	Uint32 rest[2]; /* never read */
};

/* Unused: sets entries to the identity transform, then stores the first word's
   old value plus its new one in the second. */
UNUSED void btTransitionInitEntries(struct BATTLE_TRANSITION_ENTRY *entries)
{
	Uint32 prev = entries[0].value;

	syUnitMatrix((FIXED *)entries);
	entries[0].sum = prev + entries[0].value;
}

/* Unused: returns value divided by 16. */
UNUSED Sint32 btBackgroundDivideBy16(Sint32 value)
{
	return value / 16;
}

/* What Select picks: another value in the Japanese release. */
#define DEBUG_VALUE_SELECT	 WHEN(DEBUG_VALUE_EDITOR_NEW_SELECT_PRESET, 544, 411)
/* What R picks, in every release. */
#define DEBUG_VALUE_R		 399
/* The digits the debug value steps: left and right step its ones, up and down
   its tens, L and R its hundreds. */
#define DEBUG_VALUE_TENS	 10
#define DEBUG_VALUE_HUNDREDS 100

/* Unused debug aid: with Start held, the d-pad and L/R step the battle state's
   debugValue by its ones, tens and hundreds until A takes it; Select held picks
   DEBUG_VALUE_SELECT and R held DEBUG_VALUE_R; otherwise value comes back
   unchanged. */
UNUSED Sint32 btAdjustDebugValue(Sint32 value)
{
	if(PerWork.data & KEY_START) {
		while(1) {
			BATTLE_STATE *battle = RamHeapSlots->battleState;

			if(PerWork.repeat & KEY_LEFT)
				battle->debugValue--;
			if(PerWork.repeat & KEY_RIGHT)
				battle->debugValue++;
			if(PerWork.repeat & KEY_L)
				battle->debugValue -= DEBUG_VALUE_HUNDREDS;
			if(PerWork.repeat & KEY_R)
				battle->debugValue += DEBUG_VALUE_HUNDREDS;
			if(PerWork.repeat & KEY_UP)
				battle->debugValue -= DEBUG_VALUE_TENS;
			if(PerWork.repeat & KEY_DOWN)
				battle->debugValue += DEBUG_VALUE_TENS;
			if(PerWork.push & KEY_A) {
				value = battle->debugValue;
				break;
			}
			sySynch(1);
		}
	}
	if(PerWork.data & KEY_SELECT)
		value = DEBUG_VALUE_SELECT;
	if(PerWork.data & KEY_R)
		value = DEBUG_VALUE_R;
	return value;
}

/* Sets the sprite mode of a unit's picture and its shadow. */
void btHighlightSetSprMode(Sint32 unitId, Sint32 mode)
{
	BATTLE_OBJECT *slot = btObjectGet(unitId);
	MOTION_OBJECT *object;
	SPR			  *record;
	SPR			 **records;
	Sint32		   recordIdx;

	if(slot == 0)
		return;
	object = slot->obj;
	if(object == 0)
		return;
	switch(object->recordStorageKind & ACTOR_SPRITE_KIND_MASK) {
	case ACTOR_SPRITE_ONE: {
		record = object->records;
		record->image.objMode = mode;
		record->shadow.objMode = mode;
		break;
	}
	case ACTOR_SPRITE_GROUP: {
		records = object->records;
		recordIdx = 0;
		do {
			record = *records++;
			if(record == 0)
				break;
			record->image.objMode = mode;
			record->shadow.objMode = mode;
			recordIdx++;
		} while(recordIdx < ACTOR_SPRITE_GROUP_SIZE);
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

extern Sint8 BattleHighlightBlendRamp[];

/* The steps of each half of BattleHighlightBlendRamp: the first half plays
   once, then the second over and over. */
#define HIGHLIGHT_RAMP_HALF 16

/* Steps the highlight's blend: HIGHLIGHT_FADE_IN and HIGHLIGHT_FADE_OUT fade
   the units listed in or out over 32 frames, HIGHLIGHT_PULSE blends them by the
   ramp, and HIGHLIGHT_SELECTED lightens them by it: the first sixteen steps
   once, then the second sixteen over and over. */
void btBackgroundStepHighlightBlend(void)
{
	BATTLE_STATE *battle = RamHeapSlots->battleState;

	if(battle != NULL) {
		if(battle->highlightMode == HIGHLIGHT_FADE_IN) {
			Uint32 level = battle->highlightStep >> 1;

			if(level > BLEND_WEIGHT_MAX)
				level = BLEND_WEIGHT_MAX;
			REG_BLDCNT = BLEND_SPRITES_OVER_ALL;
			REG_BLDALPHA = ((BLEND_WEIGHT_MAX - level) << 8) | level;
			battle->highlightStep++;
		} else if(battle->highlightMode == HIGHLIGHT_FADE_OUT) {
			Uint32 level = battle->highlightStep >> 1;

			if(level > BLEND_WEIGHT_MAX)
				level = BLEND_WEIGHT_MAX;
			REG_BLDCNT = BLEND_SPRITES_OVER_ALL;
			REG_BLDALPHA = (level << 8) | (BLEND_WEIGHT_MAX - level);
			battle->highlightStep++;
		} else if(battle->highlightMode == HIGHLIGHT_PULSE) {
			REG_BLDCNT = BLEND_SPRITES_OVER_ALL;
			REG_BLDALPHA =
				(BattleHighlightBlendRamp[battle->highlightStep] << 8) |
				(BLEND_WEIGHT_MAX -
					BattleHighlightBlendRamp[battle->highlightStep]);
			battle->highlightStep =
				((battle->highlightStep + 1) & (HIGHLIGHT_RAMP_HALF - 1)) |
				(battle->highlightStep >= HIGHLIGHT_RAMP_HALF - 1
						? HIGHLIGHT_RAMP_HALF
						: 0);
		} else if(battle->highlightMode != HIGHLIGHT_OFF) {
			REG_BLDCNT =
				BLDCNT_TGT1_OBJ | BLDCNT_EFFECT_LIGHTEN | BLDCNT_TGT2_ALL;
			REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX, 0);
			REG_BLDY = BattleHighlightBlendRamp[battle->highlightStep];
			battle->highlightStep =
				((battle->highlightStep + 1) & (HIGHLIGHT_RAMP_HALF - 1)) |
				(battle->highlightStep >= HIGHLIGHT_RAMP_HALF - 1
						? HIGHLIGHT_RAMP_HALF
						: 0);
		}
	}
}

/* Highlights the units in actors (up to UNIT_MAX, ended by
   BATTLE_UNIT_LIST_END), the others taking the other blend, or with
   HIGHLIGHT_OFF stops the highlight. */
void btHighlightSetModes(Uint16 *actors, Sint32 mode)
{
	Sint16		  activeActors[UNIT_MAX];
	BATTLE_STATE *battle = RamHeapSlots->battleState;
	Uint32		  cnt;
	Uint32		  i;

	if(mode == HIGHLIGHT_OFF) {
		syCloseEvent(btBackgroundStepHighlightBlend);
		syIOWrite16(mode, &REG_BLDY);
		btHighlightClearAllSprModes();
		sySynch(1);
		syIOWriteQueueStore16((Uint32)&REG_BLDCNT, 0);
	}
	if(battle != NULL && mode != HIGHLIGHT_OFF) {
		battle->highlightMode = mode;
		battle->highlightStep = 0;
		REG_BLDY = 0;
		syIOWrite16(BLEND_ALPHA(BLEND_WEIGHT_MAX, 0), &REG_BLDALPHA);

		cnt = btUnitListIds(BATTLE_SIDE_MASK_BOTH, (Uint16 *)activeActors);
		for(i = 0; i < cnt; i++)
			btHighlightSetSprMode(activeActors[i], mode & 1);

		if(actors != NULL) {
			Uint32 actor = *actors;

			i = 0;
			actors++;
			if(actor != BATTLE_UNIT_LIST_END) {
				do {
					btHighlightSetSprMode(actor, (mode & 1) ^ 1);
					i++;
					if(i >= UNIT_MAX)
						break;
					actor = *actors++;
				} while(actor != BATTLE_UNIT_LIST_END);
			}
		}
		sySynch(1);
		syIOWriteQueueStore16((Uint32)&REG_BLDCNT, 0);
		sySetEvent(btBackgroundStepHighlightBlend, EVENT_PASS_VBLANK);
	}
}

/* One of the star field's sparks, drawn inward to the centre. Sixteen live in
   STAR_FIELD_STATE.sparks; a spent one starts again while the field is
   young. */
struct SPARK {
	Sint32
		/* x, y, z, then their velocities, in STAR_FIELD_PIXEL units */
		   motion[6];
	Sint32 life; /* ticks left */
};

/* A ring of the star field leaving its centre: its place, its speed and its
   age. */
struct RING {
	Sint32 x;
	Sint32 y;
	Sint32 velX;
	Sint32 velY;
	Sint32 age;
};

/* The star field's sparks and rings, and the ticks in which a spent spark
   starts again. */
#define STAR_FIELD_SPARKS		16
#define STAR_FIELD_RINGS		3
#define STAR_FIELD_SPAWN_TICKS	25
#define STAR_FIELD_SIZE			128 /* the canvas's width and height in pixels */
#define STAR_FIELD_CENTER		(STAR_FIELD_SIZE / 2)
#define STAR_FIELD_RING_SIZE	32
#define STAR_FIELD_TICKS		45 /* the ticks a cast's glow lasts */
/* The ticks the background darkens through as the star field gathers, a
   BACKGROUND_LEVEL_STEP each. */
#define STAR_FIELD_DARKEN_TICKS 25
/* The ticks an enemy's cast shows its element's streaks. */
#define ELEMENT_STREAK_TICKS	40
/* The blue ring sprite sheet's colours, before its pictures; an element's
   palette replaces them. */
#define STAR_FIELD_COLORS		64
/* The sparks and rings move in 1/1024 canvas pixels: STAR_FIELD_PIXEL to a
   pixel. */
#define STAR_FIELD_PIXEL_SHIFT	10
#define STAR_FIELD_PIXEL		(1 << STAR_FIELD_PIXEL_SHIFT)

/* The star field's state, for the 45 ticks of a cast's glow. */
struct STAR_FIELD_STATE {
	/* The blue ring sprite sheet's pictures, the sparks' and the rings'. */
	Uint8		 sheet[4544];
	struct SPARK sparks[STAR_FIELD_SPARKS];
	struct RING	 rings[STAR_FIELD_RINGS];
	/* ticks drawn; new sparks start only in the first
	   STAR_FIELD_SPAWN_TICKS */
	Sint32 ticks;
	/* 1 when a drawn tick waits for btElementGlowUploadStarField */
	Sint32 ready;
	/* Where BG2 shows the 128 by 128 canvas, in 8.8 pixels: btElementGlowPlay
	   keeps it on the unit. */
	Sint32 originX;
	Sint32 originY;
	Sint32 originPending; /* 1 once the origin is queued for REG_BG2X */
};

/* The star tables: each picture's size and sprite sheet offset, and the ring's
   four pictures. */
extern const Uint8	ElementGlowSparkSizes[];
extern const Sint32 ElementGlowSparkPictures[];
extern const Sint32 ElementGlowRingPictures[];

/* Draws the star field's next tick: each spark falls towards the centre and
   shrinks as its life runs out, a spent one starting again while the field is
   young, and the three rings move outward. */
void btElementGlowUpdateStarField(void)
{
	Sint32					 ringsLeft;
	Sint32					 frm;
	Uint32					 size;
	Sint32					 ringX;
	Sint32					 ringY;
	Sint32					 phase;
	void					*dest;
	Sint32					 i;
	struct STAR_FIELD_STATE *state;
	DRAW_RECTANGLE			 draw;
	struct SPARK			*spark;
	struct RING				*ring;
	Sint32					*axis;
	Sint32					 axisIdx;
	FIXED					 distance;
	FIXED					 scale;
	Sint32					 step;
	Uint32					 radius;
	Sint32					 ang;
	Uint32					 half;
	Sint32					 x;
	Sint32					 y;
	Sint32					 life;
	Sint32 (*mulFX)(Sint32 a, Sint32 b);

	dest = RamHeapSlots->battleCanvas;
	state = (struct STAR_FIELD_STATE *)RamHeapSlots->battleEff;
	state->ready = 0;
	draw = RamHeapSlots->ramCode;
	for(i = 0; i < STAR_FIELD_SPARKS; i++) {
		spark = &state->sparks[i];
		life = spark->life;
		if(life != 0) {
			/* The motion's first three words are the spark's place, the last
			   three its velocity: each tick it is pulled toward the centre by
			   the same amount whatever its distance, and its velocity loses
			   1/128. */
			distance =
				sySquartFX((spark->motion[0] >> 8) * (spark->motion[0] >> 8) +
					(spark->motion[1] >> 8) * (spark->motion[1] >> 8) +
					(spark->motion[2] >> 8) * (spark->motion[2] >> 8));
			if(distance <= toFIXED(1) / 16 - 1) {
				spark->life = 0;
			} else {
				scale = syDivFX(distance, toFIXED(1));
				spark->life--;
				mulFX = syMulFX;
				axis = spark->motion;
				for(axisIdx = 0; axisIdx < 3; axisIdx++) {
					step = mulFX(mulFX(-*axis >> 8, scale), toFIXED(19) / 16);
					axis[3] = axis[3] - (axis[3] >> 7) + step;
					*axis += axis[3];
					axis++;
				}
			}
			life = spark->life;
		}
		if(life == 0 && state->ticks < STAR_FIELD_SPAWN_TICKS) {
			ang = syRandom();
			radius = syRandom() + toFIXED(1);
			half = radius >> 1;
			spark->motion[0] = syMulFX(syCos(ang), half);
			spark->motion[1] = syMulFX(sySin(ang), half);
			if(spark->motion[0] & 1)
				spark->motion[0] = -spark->motion[0];
			if(spark->motion[1] & 1)
				spark->motion[1] = -spark->motion[1];
			spark->motion[2] = (Uint32)(syRandom() + toFIXED(0.5)) >> 2;
			spark->motion[3] =
				(-spark->motion[0] >> 7) + (spark->motion[1] >> 8);
			spark->motion[4] =
				(-spark->motion[1] >> 7) + (-spark->motion[0] >> 8);
			spark->motion[5] = 0;
			life = spark->life = radius / (8 * STAR_FIELD_PIXEL) + 1;
		}
		if(life != 0) {
			x = (spark->motion[0] >> STAR_FIELD_PIXEL_SHIFT) +
				STAR_FIELD_CENTER;
			y = (spark->motion[1] >> STAR_FIELD_PIXEL_SHIFT) +
				STAR_FIELD_CENTER;
			frm = life;
			if(frm < 0)
				frm = 0;
			else if(frm > 6)
				frm = 6;
			size = ElementGlowSparkSizes[frm];
			draw(dest, (Uint8 *)state + ElementGlowSparkPictures[frm],
				x - (size >> 1), y - (size >> 1), size, size);
		}
	}
	draw = RamHeapSlots->ramCodePaired;
	for(ring = state->rings, ringsLeft = STAR_FIELD_RINGS - 1; ringsLeft >= 0;
		ringsLeft--, ring++) {
		ring->x += ring->velX;
		ring->y += ring->velY;
		ringX = ring->x >> STAR_FIELD_PIXEL_SHIFT;
		ringY = ring->y >> STAR_FIELD_PIXEL_SHIFT;
		phase = 3 - ring->age / 8;
		if(phase >= 0) {
			ring->age++;
			/* FAKEMATCH(dead): an empty once-only block after the age's store;
			 * as the store alone. */
			do {
			} while(0);
			draw(dest, (Uint8 *)state + ElementGlowRingPictures[phase],
				ringX + STAR_FIELD_CENTER - STAR_FIELD_RING_SIZE / 2,
				ringY + STAR_FIELD_CENTER - STAR_FIELD_RING_SIZE / 2,
				STAR_FIELD_RING_SIZE, STAR_FIELD_RING_SIZE);
		}
	}
	state->ticks++;
	state->ready = 1;
}

/* Copies the drawn canvas to BG2's tiles, at three quarters of its
   brightness. */
Sint32 btElementGlowUploadStarField(void)
{
	void					*src = RamHeapSlots->battleCanvas;
	struct STAR_FIELD_STATE *state = RamHeapSlots->battleEff;

	if(src != 0) {
		if(state->ready != 0) {
			state->ready = 0;
			return syColBufferScaleThreeQuarters(
				src, (void *)BG_CHAR_BLOCK(1), BG_CHAR_SIZE);
		}
	}
	return (Sint32)src;
}

/* Returns the x of the point at angle and radius from the origin. */
static __inline__ Sint32 PolarX(Sint32 ang, Sint32 radius)
{
	return syMulFX(syCos(ang), radius);
}

/* Returns the y of the point at angle and radius from the origin. */
static __inline__ Sint32 PolarY(Sint32 ang, Sint32 radius)
{
	return syMulFX(sySin(ang), radius);
}

/* Starts a star field of the element: sixteen sparks on random rings and three
   rings 120 degrees apart, in the element's colours. Returns the upload event's
   index. */
Sint32 btElementGlowInitStarField(Sint32 element)
{
	struct STAR_FIELD_STATE *state;
	struct SPARK			*spark;
	struct RING				*ring;
	Sint32					 i;
	Uint8					*data;

	RamHeapSlots->battleBg->mode = BATTLE_BACKGROUND_FIXED;
	state = syHeapSlotAllocEWRAM(HEAP_SLOT_BATTLE_EFFECT, sizeof *state);
	syHeapSlotAlloc(HEAP_SLOT_BATTLE_CANVAS, CANVAS_BYTES);
	for(i = 0; i < STAR_FIELD_SPARKS; i++) {
		Sint32 ang = syRandom();
		Uint32 radius = syRandom() + toFIXED(1);
		Uint32 half = radius >> 1;

		spark = &state->sparks[i];
		spark->motion[0] = syMulFX(syCos(ang), half);
		spark->motion[1] = syMulFX(sySin(ang), half);
		if(spark->motion[0] & 1)
			spark->motion[0] = -spark->motion[0];
		if(spark->motion[1] & 1)
			spark->motion[1] = -spark->motion[1];
		spark->motion[2] = (Uint32)(syRandom() + toFIXED(0.5)) >> 2;
		spark->motion[3] = (-spark->motion[0] >> 7) + (spark->motion[1] >> 8);
		spark->motion[4] = (-spark->motion[1] >> 7) + (-spark->motion[0] >> 8);
		spark->motion[5] = 0;
		spark->life = radius / (8 * STAR_FIELD_PIXEL) + 1;
	}
	for(i = 0; i < STAR_FIELD_RINGS; i++) {
		Sint32 radius = 4 * STAR_FIELD_PIXEL;
		Sint32 ang;

		ring = &state->rings[i];
		ang = i * (DEGtoANG(120));
		ring->x = PolarX(ang, radius);
		ring->y = PolarY(ang, radius);
		ring->velX = PolarX(ang, STAR_FIELD_PIXEL / 2);
		ring->velY = PolarY(ang, STAR_FIELD_PIXEL / 2);
		ring->age = 0;
	}
	state->ticks = 0;
	state->ready = 0;
	state->originPending = 0;
	syWordsClearEntry(
		RamHeapSlots->battleCanvas, STAR_FIELD_SIZE * STAR_FIELD_SIZE);
	data = syResourceGetTableEntry(RESOURCE_BLUE_RING_SHEET);
	syWordsCopyEntry((void *)BG_PLTT, data, STAR_FIELD_COLORS * 2);
	data += STAR_FIELD_COLORS * 2;
	syDecompressTaggedLZ(data, state);
	switch(element) {
	case ELEMENT_VENUS:
		data = syResourceGetTableEntry(RESOURCE_YELLOW_PALETTE_B);
		break;
	case ELEMENT_MERCURY:
		data = syResourceGetTableEntry(RESOURCE_BLUE_RING_SHEET);
		break;
	case ELEMENT_MARS:
		data = syResourceGetTableEntry(RESOURCE_RED_PALETTE_C);
		break;
	default:
		data = syResourceGetTableEntry(RESOURCE_VIOLET_PALETTE_D);
		break;
	}
	syDMAXCopy(
		data, (void *)BG_PLTT, DMA_COPY_WORDS(STAR_FIELD_COLORS / 2), REG_DMA3);
	syIOWrite32(0, &REG_BG2X);
	syIOWrite32(0, &REG_BG2Y);
	syIOWrite16(Q_8_8(1), &REG_BG2PA);
	syIOWrite16(0, &REG_BG2PB);
	syIOWrite16(0, &REG_BG2PC);
	syIOWrite16(Q_8_8(1), &REG_BG2PD);
	/* LEFTOVER(frame): the twin's five-argument btEffectBuildBlitterStub,
	 * never taken here; the ROM's frame still reserves its fifth argument and
	 * never touches it; as the call left out. */
	if(0)
		btEffectBuildBlitterStub(HEAP_SLOT_RAM_CODE, 7, 7, 3, 3);
	/* Clipped to the canvas, the additive blitter and then the one that keeps
	   the brighter colour, packed as btEffectLoadBlitter takes them. */
	btEffectLoadBlitterStub(HEAP_SLOT_RAM_CODE,
		COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | BLITTER_ADDITIVE << 4);
	btEffectLoadBlitterStub(HEAP_SLOT_RAM_CODE_PAIRED,
		COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | BLITTER_MAX << 4);
	sySetEvent(btElementGlowUpdateStarField, EVENT_PASS_TICK);
	return sySetEvent(btElementGlowUploadStarField, EVENT_PASS_TICK);
}

/* Ends a star field and turns BG2 off. */
Sint32 btElementGlowEnd(Sint32 unused)
{
	syWordsClearEntry((void *)BG_CHAR_BLOCK(1), BG_CHAR_SIZE);
	syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
	syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
	syHeapSlotRelease(HEAP_SLOT_BATTLE_CANVAS);
	syHeapSlotRelease(HEAP_SLOT_BATTLE_EFFECT);
	REG_DISPCNT = DISPCNT_OBJ_ON | DISPCNT_BG1_ON | DISPCNT_BG0_ON |
		DISPCNT_OBJ_1D_MAP | DISPCNT_MODE_1;
	syCloseEvent(btElementGlowUploadStarField);
	return syCloseEvent(btElementGlowUpdateStarField);
}

/* Unused: does nothing. */
UNUSED void btElementGlowReservedNoOperation(void)
{
}

/* Plays a glow of the element about a unit, and returns when it is over: the
   star field, the streaks, the pillar or the burst. */
void btElementGlowPlay(
	Sint32 unitId, Sint32 element, Sint32 mode, Sint32 variant)
{
	SCREEN_POINT			  starPos;
	SCREEN_POINT			  streakPos;
	BATTLE_PRESENTATION_STATE pillarLaunch;
	BATTLE_PRESENTATION_STATE burstLaunch;
	Sint32					  origin[2];
	BATTLE_STATE			 *battle = RamHeapSlots->battleState;
	volatile Uint16			 *ime;
	struct STAR_FIELD_STATE	 *state;
	Sint32					  frm;
	Sint32					 *brightness;
	SCREEN_POINT			 *pos;

	sySynch(1);
	btBackgroundSetMode(BATTLE_BACKGROUND_FIXED, battle->bg, 0);
	syWordsClearEntry((void *)BG_CHAR_BLOCK(1), BG_CHAR_SIZE);
	syIOWriteQueueStore16((Uint32)&REG_DISPCNT,
		DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG1_ON |
			DISPCNT_BG2_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON);
	syIOWriteQueueStore16((Uint32)&REG_BG2CNT,
		BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7));
	syIOWriteQueueStore16((Uint32)&REG_BLDCNT,
		BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL);
	sySynch(1);
	syIOWrite16(DISPLAY_WIDTH, &REG_WIN0H);
	syIOWrite16(WIN_RANGE(16, 136), &REG_WIN0V);
	syIOWrite16(WININ_WIN0_ALL, &REG_WININ);
	syIOWrite16(WINOUT_WIN01_BG0 | WINOUT_WIN01_OBJ, &REG_WINOUT);
	if(mode == GLOW_STAR_FIELD) {
		/* The star field, the background darkening as it starts. */
		syIOWriteQueueStore16(
			(Uint32)&REG_BLDALPHA, BLEND_ALPHA(14, BLEND_WEIGHT_MAX));
		btElementGlowInitStarField(element);
		for(frm = 0, pos = &starPos; frm < STAR_FIELD_TICKS; frm++) {
			brightness = &battle->brightness;
			ime = &REG_IME;
			state = (struct STAR_FIELD_STATE *)RamHeapSlots->battleEff;
			if(frm < STAR_FIELD_DARKEN_TICKS)
				btColBufferScale(battle->pal, BATTLE_BG_PALETTE,
					*brightness = toFIXED(1) - frm * BACKGROUND_LEVEL_STEP,
					BATTLE_BACKGROUND_COLORS);
			btMotionProjectScaledPos(unitId, (Sint32 *)pos);
			state->originX = (STAR_FIELD_CENTER - pos->x) << 8;
			state->originY = (STAR_FIELD_CENTER - pos->y) << 8;
			IO_WRITE_QUEUE_PUSH_IME(ime, (Uint32)&REG_BG2X,
				(Uint32)&state->originX, DMA_COPY_WORDS(2));
			state->originPending = 1;
			sySynch(1);
		}
		btElementGlowEnd(element);
	} else if(mode == GLOW_STREAKS) {
		/* The streaks, their canvas centred on the unit likewise. */
		btEffectStartElementStreaksStub(element);
		for(frm = 0; frm < ELEMENT_STREAK_TICKS; frm++) {
			btMotionProjectScaledPos(unitId, (Sint32 *)&streakPos);
			origin[0] = (STAR_FIELD_CENTER - streakPos.x) << 8;
			origin[1] = (STAR_FIELD_CENTER - streakPos.y) << 8;
			IO_WRITE_QUEUE_PUSH(
				(Uint32)&REG_BG2X, (Uint32)origin, DMA_COPY_WORDS(2));
			sySynch(1);
		}
		btEffectEndElementStreaksStub();
	} else if(mode == GLOW_PILLAR) {
		/* The pillar and the burst run a battle effect with the unit as its
		   user and only target. */
		pillarLaunch.prelude = 0;
		pillarLaunch.kind = element;
		pillarLaunch.variant = variant;
		pillarLaunch.actor = unitId;
		pillarLaunch.actors[0] = unitId;
		pillarLaunch.firstTgt = unitId;
		pillarLaunch.cnt = 1;
		pillarLaunch.range = 1;
		btEffectRunElementPillarStub(&pillarLaunch);
	} else {
		burstLaunch.prelude = 0;
		burstLaunch.kind = element;
		burstLaunch.variant = 0;
		burstLaunch.actor = unitId;
		burstLaunch.firstTgt = unitId;
		burstLaunch.actors[0] = unitId;
		burstLaunch.cnt = 1;
		burstLaunch.range = 1;
		btEffectRunElementBurstStub(&burstLaunch);
	}
}

/* Copies the background's saved palette back undarkened, as the star field of
   btElementGlowPlay leaves it dark. */
void btBackgroundRestorePalette(void)
{
	btBackgroundSetPaletteLevel(0, 0);
}

/* Unused: removes the VCount interrupt's handler, as btTransitionRun installs
   one. */
UNUSED void btIRQRemoveVCountHandler(void)
{
	syIntFunction(IRQ_VCOUNT, 0, 0);
}

/* The average level of a formation's enemy kinds, counting only kinds the
   player has defeated (ENEMY_DEFEATED_FLAG, or all with
   FLAG_MONSTER_BATTLE_ANY_ENEMY); -1 for an empty formation, -2 when one
   present kind is undefeated, -3 when none counts. */
Sint32 btFormationAverageLevel(Sint32 recordId)
{
	FORMATION			   *record;
	const ENEMY_DEFINITION *enemy;
	Uint32					memberIdx;
	Sint32					enemyId;
	Sint32					levelTotal;
	Sint32					enemyCnt;
	/* LEFTOVER(frame): an unused local the ROM's frame reserves and never
	 * touches. */
	UNUSED Sint32 levels[7];

	enemyCnt = 0;
	levelTotal = 0;
	record = &Formations[recordId];
	for(memberIdx = 0; memberIdx < FORMATION_MEMBERS; memberIdx++) {
		if(record->minCnts[memberIdx] != 0)
			break;
	}
	if(memberIdx == FORMATION_MEMBERS)
		return -1;
	for(memberIdx = 0; memberIdx < FORMATION_MEMBERS; memberIdx++) {
		if(record->maxCnts[memberIdx] == 0)
			continue;
		enemyId = record->memberIds[memberIdx];
		enemy = gmEnemyGetDefinitionStub(enemyId + ENEMY_ID_FIRST);
		if(enemy == 0)
			continue;
		if(!syGameFlagGet(FLAG_MONSTER_BATTLE_ANY_ENEMY) &&
			!syGameFlagGet(ENEMY_DEFEATED_FLAG(enemyId + ENEMY_ID_FIRST)))
			return -2;
		levelTotal += enemy->level;
		enemyCnt++;
	}
	if(enemyCnt == 0)
		return -3;
	return levelTotal / enemyCnt;
}

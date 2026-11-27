/* @file */
/* A unit's elemental glow: the star field and the other glows. */
/* Part of battle.c. */

#include "battle_internal.h"

/* One of the star field's sparks, drawn inward to the centre. Sixteen live */
/* in SPARK_STATE.sparks; a spent one starts again while the field is young. */
typedef struct SPARK {
	s32 motion[6]; /* x, y, z, then their velocities, in STAR_FIELD_PIXEL units
					*/
	s32 life;	   /* frames left */
} SPARK;

/* The star field's sparks and rings, and the frames in which a spent spark */
/* starts again. */
#define STAR_FIELD_SPARKS		16
#define STAR_FIELD_RINGS		3
#define STAR_FIELD_SPAWN_FRAMES 25
#define STAR_FIELD_SIZE			128 /* the canvas's width and height in pixels */
#define STAR_FIELD_CENTER		(STAR_FIELD_SIZE / 2)
#define STAR_FIELD_RING_SIZE	32
#define STAR_FIELD_FRAMES		45 /* the frames a cast's glow lasts */
/* The blue ring sprite sheet's colours, before its pictures; an element's */
/* palette replaces them. */
#define STAR_FIELD_COLORS 64
/* The sparks and rings move in 1/1024 canvas pixels: STAR_FIELD_PIXEL to a */
/* pixel. */
#define STAR_FIELD_PIXEL_SHIFT 10
#define STAR_FIELD_PIXEL	   (1 << STAR_FIELD_PIXEL_SHIFT)

/* The star field's state, for the 45 frames of a cast's glow. */
typedef struct SPARK_STATE {
	/* The blue ring sprite sheet's pictures, the sparks' and the rings'. */
	u8 sheet[4544];
	SPARK sparks[STAR_FIELD_SPARKS];
	RING rings[STAR_FIELD_RINGS];
	s32 frames; /* frames drawn; new sparks start only in the first */
				/* STAR_FIELD_SPAWN_FRAMES */
	s32 ready; /* 1 when a drawn frame waits for btElementGlowUploadStarField */
	/* Where BG2 shows the 128 by 128 canvas: btElementGlowPlay */
	/* keeps it on the unit, as a battle effect's tracked canvas would be. */
	BATTLE_BG2_ORIGIN canvasOrigin;
} SPARK_STATE;

STATIC_ASSERT(offsetof(SPARK_STATE, canvasOrigin) ==
			  offsetof(BATTLE_EFFECT_STATE, trackedCanvas.origin));

/* Draws the star field's next frame: each spark falls towards the centre and */
/* shrinks as its life runs out, and the three rings move outward. */
UNREAD_RESULT btElementGlowUpdateStarField(void) {
	s32 ringIndex;
	s32 picture;
	u32 size;
	s32 ringX;
	s32 ringY;
	s32 phase;
	void *canvas;
	s32 sparkIndex;
	SPARK_STATE *state;
	DRAW_RECTANGLE draw;
	SPARK *spark;
	RING *ring;
	s32 *axisPosition;
	s32 axis;
	FIXED distance;
	FIXED scale;
	s32 coordinate;
	FIXED acceleration;
	u32 radius;
	s32 angle;
	u32 halfRadius;
	s32 x;
	s32 y;
	s32 life;

	canvas = HeapSlots[HEAP_SLOT_BATTLE_CANVAS];
	state = HeapSlots[HEAP_SLOT_BATTLE_EFFECT];
	state->ready = 0;
	draw = (DRAW_RECTANGLE)HeapSlots[HEAP_SLOT_RAM_CODE];
	for(sparkIndex = 0; sparkIndex < STAR_FIELD_SPARKS; sparkIndex++) {
		spark = &state->sparks[sparkIndex];
		life = spark->life;
		/* A spark ends within 4 pixels of the centre (the distance is in */
		/* STAR_FIELD_PIXEL units). Until then it is pulled straight at the
		 * centre, */
		/* 19/64 of a pixel a frame faster each frame, and its speed loses a
		 * 128th */
		/* a frame. */
		if(life != 0) {
			distance =
				sySquartFX((spark->motion[0] >> 8) * (spark->motion[0] >> 8) +
						   (spark->motion[1] >> 8) * (spark->motion[1] >> 8) +
						   (spark->motion[2] >> 8) * (spark->motion[2] >> 8));
			if(distance <= 4 * STAR_FIELD_PIXEL - 1) {
				spark->life = 0;
			}
			else {
				scale = syDivFX(distance, FIXED_ONE);
				spark->life--;
				axisPosition = spark->motion;
				for(axis = 0; axis < 3; axis++) {
					coordinate = *axisPosition;
					acceleration = syMulFX(syMulFX(-coordinate >> 8, scale),
										   FIXED_ONE * 19 / 16);
					axisPosition[3] =
						axisPosition[3] - (axisPosition[3] >> 7) + acceleration;
					*axisPosition = coordinate + axisPosition[3];
					axisPosition++;
				}
			}
			life = spark->life;
		}
		/* A spent spark starts again, as btElementGlowInitStarField starts */
		/* them, while the field is young. */
		if(life == 0 && state->frames < STAR_FIELD_SPAWN_FRAMES) {
			angle = syRandom();
			radius = syRandom() + FIXED_ONE;
			halfRadius = radius >> 1;
			spark->motion[0] = syMulFX(syCos(angle), halfRadius);
			spark->motion[1] = syMulFX(sySin(angle), halfRadius);
			if(spark->motion[0] & 1)
				spark->motion[0] = -spark->motion[0];
			if(spark->motion[1] & 1)
				spark->motion[1] = -spark->motion[1];
			spark->motion[2] = (syRandom() + toFIXED(0.5)) >> 2;
			spark->motion[3] =
				(-spark->motion[0] >> 7) + (spark->motion[1] >> 8);
			spark->motion[4] =
				(-spark->motion[1] >> 7) + (-spark->motion[0] >> 8);
			spark->motion[5] = 0;
			life = spark->life = radius / (8 * STAR_FIELD_PIXEL) + 1;
		}
		/* Drawn about the canvas's centre by its life left: 2 pixels square on
		 */
		/* its last frame, 3 on the one before, 4 until then. */
		if(life != 0) {
			x = (spark->motion[0] >> STAR_FIELD_PIXEL_SHIFT) +
				STAR_FIELD_CENTER;
			y = (spark->motion[1] >> STAR_FIELD_PIXEL_SHIFT) +
				STAR_FIELD_CENTER;
			picture = life;
			if(picture < 0)
				picture = 0;
			else if(picture > 6)
				picture = 6;
			size = ElementGlow_SparkSizes[picture];
			draw(canvas, (u8 *)state + ElementGlow_SparkPictures[picture],
				 x - (size >> 1), y - (size >> 1), size, size);
		}
	}
	/* The rings, through the blitter that keeps the brighter colour: each shows
	 */
	/* its four pictures eight frames apiece, then is no longer drawn. */
	draw = (DRAW_RECTANGLE)HeapSlots[HEAP_SLOT_RAM_CODE_PAIRED];
	for(ring = state->rings, ringIndex = STAR_FIELD_RINGS - 1; ringIndex >= 0;
		ringIndex--, ring++) {
		ring->x += ring->velX;
		ring->y += ring->velY;
		ringX = ring->x >> STAR_FIELD_PIXEL_SHIFT;
		ringY = ring->y >> STAR_FIELD_PIXEL_SHIFT;
		phase = 3 - ring->age / 8;
		if(phase >= 0) {
			ring->age++;
			draw(canvas, (u8 *)state + ElementGlow_RingPictures[phase],
				 ringX + STAR_FIELD_CENTER - STAR_FIELD_RING_SIZE / 2,
				 ringY + STAR_FIELD_CENTER - STAR_FIELD_RING_SIZE / 2,
				 STAR_FIELD_RING_SIZE, STAR_FIELD_RING_SIZE);
		}
	}
	state->frames++;
	state->ready = 1;
}

/* Copies the drawn canvas to BG2's tiles and dims the canvas to three */
/* quarters, so the next frame draws over fading trails. */
/* UB: the task runner invokes a void task. */
#ifdef UBFIX
void btElementGlowUploadStarField(void) {
#else
u32 btElementGlowUploadStarField(void) {
#endif
	void *source = HeapSlots[HEAP_SLOT_BATTLE_CANVAS];
	SPARK_STATE *state = HeapSlots[HEAP_SLOT_BATTLE_EFFECT];

	if(source != 0) {
		if(state->ready != 0) {
			state->ready = 0;
			syCanvasCopyAndScaleThreeQuarters(source, BG_CHAR_BLOCK(1),
											  BG_CHAR_SIZE);
		}
	}
}

/* The two tasks as the task runner takes them: without UBFIX they are */
/* declared returning a value, and are cast to TASK_FUNC. */
#ifdef UBFIX
#define STAR_FIELD_UPDATE_FUNC btElementGlowUpdateStarField
#define STAR_FIELD_UPLOAD_FUNC btElementGlowUploadStarField
#else
#define STAR_FIELD_UPDATE_FUNC ((TASK_FUNC)btElementGlowUpdateStarField)
#define STAR_FIELD_UPLOAD_FUNC ((TASK_FUNC)btElementGlowUploadStarField)
#endif

/* Starts a star field of the element: sixteen sparks on random rings and three
 */
/* rings 120 degrees apart. Returns the upload task's index. */
s32 btElementGlowInitStarField(s32 element) {
	SPARK_STATE *state;
	SPARK *spark;
	RING *ring;
	s32 effectIndex;
	u8 *graphics;
	/* LEFTOVER(frame): an unused local the ROM's frame reserves and never
	 * touches. */
	UNUSED SPARK unused;

	BattleBackgroundStateWork->mode = BATTLE_BACKGROUND_FIXED;
	state = syHeapSlotAllocEwram(HEAP_SLOT_BATTLE_EFFECT, sizeof *state);
	syHeapSlotAlloc(HEAP_SLOT_BATTLE_CANVAS, STAR_FIELD_SIZE * STAR_FIELD_SIZE);
	/* FAKEMATCH(dead): an empty once-only loop. */
	do {
	} while(0);
	/* Each spark at a random angle and height, swirling inward; it lives a
	 * frame */
	/* for every four pixels it starts out from the centre, and one more. */
	for(effectIndex = 0; effectIndex < STAR_FIELD_SPARKS; effectIndex++) {
		s32 angle = syRandom();
		u32 radius = syRandom() + FIXED_ONE;
		u32 halfRadius = radius >> 1;

		spark = &state->sparks[effectIndex];
		spark->motion[0] = syMulFX(syCos(angle), halfRadius);
		spark->motion[1] = syMulFX(sySin(angle), halfRadius);
		if(spark->motion[0] & 1)
			spark->motion[0] = -spark->motion[0];
		if(spark->motion[1] & 1)
			spark->motion[1] = -spark->motion[1];
		spark->motion[2] = (syRandom() + toFIXED(0.5)) >> 2;
		spark->motion[3] = (-spark->motion[0] >> 7) + (spark->motion[1] >> 8);
		spark->motion[4] = (-spark->motion[1] >> 7) + (-spark->motion[0] >> 8);
		spark->motion[5] = 0;
		spark->life = radius / (8 * STAR_FIELD_PIXEL) + 1;
	}
	for(effectIndex = 0; effectIndex < STAR_FIELD_RINGS; effectIndex++) {
		s32 radius = 4 * STAR_FIELD_PIXEL;
		s32 angle;

		ring = &state->rings[effectIndex];
		angle = effectIndex * (DEGtoANG(120));
		ring->x = syMulFX(syCos(angle), radius);
		ring->y = syMulFX(sySin(angle), radius);
		ring->velX = syMulFX(syCos(angle), STAR_FIELD_PIXEL / 2);
		ring->velY = syMulFX(sySin(angle), STAR_FIELD_PIXEL / 2);
		ring->age = 0;
	}
	state->frames = 0;
	state->ready = 0;
	state->canvasOrigin.queued = 0;
	syWordsClear(HeapSlots[HEAP_SLOT_BATTLE_CANVAS],
				 STAR_FIELD_SIZE * STAR_FIELD_SIZE);
	/* The sprite sheet's colours, then its pictures; then the element's own */
	/* colours over them. */
	graphics = syResourceGetTableEntry(RESOURCE_BLUE_RING_SHEET);
	syWordsCopy(BG_PLTT, graphics, STAR_FIELD_COLORS * 2);
	graphics += STAR_FIELD_COLORS * 2;
	fdDecompressTaggedLz(graphics, state);
	switch(element) {
	case ELEMENT_VENUS:
		graphics = syResourceGetTableEntry(RESOURCE_VENUS_STAR_FIELD_PALETTE);
		break;
	case ELEMENT_MERCURY:
		graphics = syResourceGetTableEntry(RESOURCE_BLUE_RING_SHEET);
		break;
	case ELEMENT_MARS:
		graphics = syResourceGetTableEntry(RESOURCE_MARS_STAR_FIELD_PALETTE);
		break;
	default:
		graphics = syResourceGetTableEntry(RESOURCE_JUPITER_STAR_FIELD_PALETTE);
		break;
	}
	syDMAXCopy(graphics, BG_PLTT, DMA_COPY_WORDS(STAR_FIELD_COLORS / 2),
			   REG_DMA3);
	scIoWrite32(0, &REG_BG2X);
	scIoWrite32(0, &REG_BG2Y);
	scIoWrite16(Q_8_8(1), &REG_BG2PA);
	scIoWrite16(0, &REG_BG2PB);
	scIoWrite16(0, &REG_BG2PC);
	scIoWrite16(Q_8_8(1), &REG_BG2PD);
	btEffectBuildBlitterStub(HEAP_SLOT_RAM_CODE, 7, 7,
							 BLITTER_CLIP_X | BLITTER_CLIP_Y,
							 COMPOSITE_ADDITIVE);
	btEffectBuildBlitterStub(HEAP_SLOT_RAM_CODE_PAIRED, 7, 7,
							 BLITTER_CLIP_X | BLITTER_CLIP_Y,
							 COMPOSITE_LIGHTEN);
	sySetEvent(STAR_FIELD_UPDATE_FUNC, TASK_PASS_FRAME);
	return sySetEvent(STAR_FIELD_UPLOAD_FUNC, TASK_PASS_FRAME);
}

/* Ends a star field and turns BG2 off. */
s32 btElementGlowEnd(s32 unused) {
	syWordsClear(BG_CHAR_BLOCK(1), BG_CHAR_SIZE);
	syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
	syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
	syHeapSlotRelease(HEAP_SLOT_BATTLE_CANVAS);
	syHeapSlotRelease(HEAP_SLOT_BATTLE_EFFECT);
	REG_DISPCNT = DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
				  DISPCNT_BG1_ON | DISPCNT_OBJ_ON;
	syCloseEvent(STAR_FIELD_UPLOAD_FUNC);
	return syCloseEvent(STAR_FIELD_UPDATE_FUNC);
}

/* Copies count colours from source to destination, each channel scaled by */
/* scale (16.16, at most 1), and returns 0. */
s32 btGraphicsScaleRgb555Clamped(u16 *source, u16 *destination, FIXED scale,
								 s32 count) {
	s32 i;

	if(scale > FIXED_ONE)
		scale = FIXED_ONE;
	/* Each channel scaled in place: a 16.16 scale of at most 1 keeps it inside
	 */
	/* its own five bits. */
	for(i = 0; i < count; i++) {
		u32 redMask = RGB(RGB_LEVEL_MAX, 0, 0);
		u32 greenMask = RGB(0, RGB_LEVEL_MAX, 0);
		u32 blueMask = RGB(0, 0, RGB_LEVEL_MAX);
		u32 color = *source;
		u32 red = color & redMask;
		u32 green = color & greenMask;
		u32 blue = blueMask & color;

		red *= scale;
		green *= scale;
		blue *= scale;
		color = (FIXEDtoINT(red) & redMask) | (FIXEDtoINT(green) & greenMask);
		color |= FIXEDtoINT(blue) & blueMask;
		*destination = color;
		source++;
		destination++;
	}
	return 0;
}

/* Plays a glow of the element about a unit, and returns when it is over: the */
/* star field, the streaks, the pillar or the burst. */
void btElementGlowPlay(s32 unitId, s32 element, s32 mode, s32 variant) {
	/* LEFTOVER(frame): an unused local the ROM's frame reserves and never
	 * touches. */
	UNUSED s32 spare[10];
	SCR_POINT starFieldPosition;
	SCR_POINT streakPosition;
	BATTLE_STAGING_STATE pillarLaunch;
	BATTLE_STAGING_STATE burstLaunch;
	/* The heap slots read from the camera's, as the battle effects read theirs
	 */
	/* from the effect's (EFFECT_SLOT). */
	void **slots = &HeapSlots[HEAP_SLOT_CAMERA];
	volatile u16 *ime;
	BATTLE_STATE *battle =
		HEAP_SLOT_FROM(slots, HEAP_SLOT_CAMERA, HEAP_SLOT_BATTLE);
	SPARK_STATE *state;
	s32 frame;
	s32 *brightness;

	sySynch(1);
	btBackgroundSetMode(BATTLE_BACKGROUND_FIXED, battle->bg, 0);
	syWordsClear(BG_CHAR_BLOCK(1), BG_CHAR_SIZE);
	syIoWriteQueueStore16((u32)&REG_DISPCNT,
						  DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
							  DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_OBJ_ON |
							  DISPCNT_WIN0_ON);
	syIoWriteQueueStore16((u32)&REG_BG2CNT, BGCNT_CHAR_BASE(1) |
												BGCNT_256_COLORS |
												BGCNT_SCREEN_BASE(7));
	syIoWriteQueueStore16((u32)&REG_BLDCNT, BLDCNT_TGT1_BG2 |
												BLDCNT_EFFECT_BLEND |
												BLDCNT_TGT2_ALL);
	sySynch(1);
	scIoWrite16(WIN_RANGE(0, DISPLAY_WIDTH), &REG_WIN0H);
	scIoWrite16(WIN_RANGE(16, 136), &REG_WIN0V);
	scIoWrite16(WININ_WIN0_ALL, &REG_WININ);
	scIoWrite16(WINOUT_WIN01_BG0 | WINOUT_WIN01_OBJ, &REG_WINOUT);
	if(mode == GLOW_STAR_FIELD) {
		/* The star field for 45 frames, the background darkening for the
		 * first 25. */
		syIoWriteQueueStore16((u32)&REG_BLDALPHA,
							  BLEND_ALPHA(14, BLEND_WEIGHT_MAX));
		btElementGlowInitStarField(element);
		for(frame = 0; frame < STAR_FIELD_FRAMES; frame++) {
			brightness = &battle->brightness;
			ime = &REG_IME;
			state = HeapSlots[HEAP_SLOT_BATTLE_EFFECT];
			if(frame <= 24)
				btGraphicsScaleRgb555Clamped(
					battle->pal, BATTLE_BG_PALETTE,
					*brightness = FIXED_ONE - frame * BACKGROUND_LEVEL_STEP,
					BATTLE_BACKGROUND_COLORS);
			btMotionProjectScaledPos(unitId,
									 syScrPointCoordinates(&starFieldPosition));
			state->canvasOrigin.x =
				(STAR_FIELD_CENTER - (u32)starFieldPosition.x) << 8;
			state->canvasOrigin.y =
				(STAR_FIELD_CENTER - (u32)starFieldPosition.y) << 8;
			IO_WRITE_QUEUE_PUSH_IME(ime, (u32)&REG_BG2X,
									(u32)&state->canvasOrigin.x,
									DMA_COPY_WORDS(2));
			state->canvasOrigin.queued = 1;
			sySynch(1);
		}
		btElementGlowEnd(element);
	}
	else if(mode == GLOW_STREAKS) {
		/* The streaks for 40 frames, their canvas centred on the unit likewise.
		 */
		BATTLE_EFFECT_STATE *effectState;

		/* The effect uses its image area to stage the tracked canvas origin. */
		btEffectStartElementStreaksStub(element);
		for(frame = 0; frame < 40; frame++) {
			effectState = HeapSlots[HEAP_SLOT_BATTLE_EFFECT];
			btMotionProjectScaledPos(unitId,
									 syScrPointCoordinates(&streakPosition));
			effectState->trackedCanvas.origin.x =
				(STAR_FIELD_CENTER - (u32)streakPosition.x) << 8;
			effectState->trackedCanvas.origin.y =
				(STAR_FIELD_CENTER - (u32)streakPosition.y) << 8;
			IO_WRITE_QUEUE_PUSH((u32)&REG_BG2X,
								(u32)&effectState->trackedCanvas.origin.x,
								DMA_COPY_WORDS(2));
			effectState->trackedCanvas.origin.queued = 1;
			sySynch(1);
		}
		btEffectEndElementStreaksStub();
	}
	else if(mode == GLOW_PILLAR) {
		/* The pillar and the burst run a battle effect with the unit as its
		 * user */
		/* and only target. */
		pillarLaunch.eff.prelude = 0;
		pillarLaunch.eff.kind = element;
		pillarLaunch.eff.variant = variant;
		pillarLaunch.eff.user = unitId;
		pillarLaunch.eff.tgts[0] = unitId;
		pillarLaunch.eff.firstTgt = unitId;
		pillarLaunch.eff.tgtCnt = 1;
		pillarLaunch.eff.range = 1;
		btEffectRunElementPillarStub(&pillarLaunch.eff);
	}
	else {
		burstLaunch.eff.kind = element;
		burstLaunch.eff.prelude = 0;
		burstLaunch.eff.variant = 0;
		burstLaunch.eff.user = unitId;
		burstLaunch.eff.tgts[0] = unitId;
		burstLaunch.eff.firstTgt = unitId;
		burstLaunch.eff.tgtCnt = 1;
		burstLaunch.eff.range = 1;
		btEffectRunElementBurstStub(&burstLaunch.eff);
	}
}

/* Copies the background's saved palette back undarkened, as the star field of
 */
/* btElementGlowPlay leaves it dark. */
void btBackgroundRestorePalette(void) { btBackgroundSetPaletteLevel(0, 0); }

/* Removes the VCount interrupt's handler, as */
/* btTransitionRun installs one; nothing calls it. */
void btIrqRemoveVCountHandler(void) { syIntFunction(IRQ_VCOUNT, 0, 0); }

/* A unit's elemental glow: the star field and the other glows. Part of
   battle.c. */

#include "battle_internal.h"

/* One of the star field's sparks, drawn inward to the centre. Sixteen live in
   SPARK_STATE.sparks; a spent one starts again while the field is young. */
typedef struct SPARK {
	Sint32
		/* x, y, z, then their velocities, in STAR_FIELD_PIXEL units */
		   motion[6];
	Sint32 life; /* ticks left */
} SPARK;

/* The star field's sparks and rings, and the ticks in which a spent spark
   starts again. */
#define STAR_FIELD_SPARKS	   16
#define STAR_FIELD_RINGS	   3
#define STAR_FIELD_SPAWN_TICKS 25
#define STAR_FIELD_SIZE		   128 /* the canvas's width and height in pixels */
#define STAR_FIELD_CENTER	   (STAR_FIELD_SIZE / 2)
#define STAR_FIELD_RING_SIZE   32
#define STAR_FIELD_TICKS	   45 /* the ticks a cast's glow lasts */
/* The blue ring sprite sheet's colours, before its pictures; an element's
   palette replaces them. */
#define STAR_FIELD_COLORS	   64
/* The sparks and rings move in 1/1024 canvas pixels: STAR_FIELD_PIXEL to a
   pixel. */
#define STAR_FIELD_PIXEL_SHIFT 10
#define STAR_FIELD_PIXEL	   (1 << STAR_FIELD_PIXEL_SHIFT)

/* The star field's state, for the 45 ticks of a cast's glow. */
typedef struct SPARK_STATE {
	/* The blue ring sprite sheet's pictures, the sparks' and the rings'. */
	Uint8 sheet[4544];
	SPARK sparks[STAR_FIELD_SPARKS];
	RING  rings[STAR_FIELD_RINGS];
	/* ticks drawn; new sparks start only in the first
	   STAR_FIELD_SPAWN_TICKS */
	Sint32 ticks;
	/* 1 when a drawn tick waits for btElementGlowUploadStarField */
	Sint32 ready;
	/* Where BG2 shows the 128 by 128 canvas: btElementGlowPlay keeps it on the
	   unit, as a battle effect's tracked canvas would be. */
	BATTLE_BG2_ORIGIN canvasOrigin;
} SPARK_STATE;

STATIC_ASSERT(offsetof(SPARK_STATE, canvasOrigin) ==
	offsetof(BATTLE_EFFECT_STATE, trackedCanvas.origin));

/* Draws the star field's next tick: each spark falls towards the centre and
   shrinks as its life runs out, and the three rings move outward. */
UNREAD_RESULT btElementGlowUpdateStarField(void)
{
	Sint32		   ringIndex;
	Sint32		   picture;
	Uint32		   size;
	Sint32		   ringX;
	Sint32		   ringY;
	Sint32		   phase;
	void		  *canvas;
	Sint32		   sparkIndex;
	SPARK_STATE	  *state;
	DRAW_RECTANGLE draw;
	SPARK		  *spark;
	RING		  *ring;
	Sint32		  *axisPos;
	Sint32		   axis;
	FIXED		   distance;
	FIXED		   scale;
	Sint32		   coordinate;
	FIXED		   acceleration;
	Uint32		   radius;
	Sint32		   angle;
	Uint32		   halfRadius;
	Sint32		   x;
	Sint32		   y;
	Sint32		   life;

	canvas = HeapSlots[HEAP_SLOT_BATTLE_CANVAS];
	state = HeapSlots[HEAP_SLOT_BATTLE_EFFECT];
	state->ready = 0;
	draw = (DRAW_RECTANGLE)HeapSlots[HEAP_SLOT_RAM_CODE];
	for(sparkIndex = 0; sparkIndex < STAR_FIELD_SPARKS; sparkIndex++) {
		spark = &state->sparks[sparkIndex];
		life = spark->life;
		/* A spark ends within 4 pixels of the centre (the distance is in
		   STAR_FIELD_PIXEL units). Until then it is pulled straight at the
		   centre, 19/64 of a pixel a tick faster each tick, and its speed
		   loses a 128th a tick. */
		if(life != 0) {
			distance =
				sySquartFX((spark->motion[0] >> 8) * (spark->motion[0] >> 8) +
					(spark->motion[1] >> 8) * (spark->motion[1] >> 8) +
					(spark->motion[2] >> 8) * (spark->motion[2] >> 8));
			if(distance <= 4 * STAR_FIELD_PIXEL - 1) {
				spark->life = 0;
			} else {
				scale = syDivFX(distance, toFIXED(1));
				spark->life--;
				axisPos = spark->motion;
				for(axis = 0; axis < 3; axis++) {
					coordinate = *axisPos;
					acceleration = syMulFX(
						syMulFX(-coordinate >> 8, scale), toFIXED(19) / 16);
					axisPos[3] = axisPos[3] - (axisPos[3] >> 7) + acceleration;
					*axisPos = coordinate + axisPos[3];
					axisPos++;
				}
			}
			life = spark->life;
		}
		/* A spent spark starts again, as btElementGlowInitStarField starts
		   them, while the field is young. */
		if(life == 0 && state->ticks < STAR_FIELD_SPAWN_TICKS) {
			angle = syRandom();
			radius = syRandom() + toFIXED(1);
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
		   its last tick, 3 on the one before, 4 until then. */
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
			size = ElementGlowSparkSizes[picture];
			draw(canvas, (Uint8 *)state + ElementGlowSparkPictures[picture],
				x - (size >> 1), y - (size >> 1), size, size);
		}
	}
	/* The rings, through the blitter that keeps the brighter colour: each shows
	   its four pictures eight ticks apiece, then is no longer drawn. */
	draw = (DRAW_RECTANGLE)HeapSlots[HEAP_SLOT_RAM_CODE_PAIRED];
	for(ring = state->rings, ringIndex = STAR_FIELD_RINGS - 1; ringIndex >= 0;
		ringIndex--, ring++) {
		ring->x += ring->velocityX;
		ring->y += ring->velocityY;
		ringX = ring->x >> STAR_FIELD_PIXEL_SHIFT;
		ringY = ring->y >> STAR_FIELD_PIXEL_SHIFT;
		phase = 3 - ring->age / 8;
		if(phase >= 0) {
			ring->age++;
			draw(canvas, (Uint8 *)state + ElementGlowRingPictures[phase],
				ringX + STAR_FIELD_CENTER - STAR_FIELD_RING_SIZE / 2,
				ringY + STAR_FIELD_CENTER - STAR_FIELD_RING_SIZE / 2,
				STAR_FIELD_RING_SIZE, STAR_FIELD_RING_SIZE);
		}
	}
	state->ticks++;
	state->ready = 1;
}

/* Copies the drawn canvas to BG2's tiles and dims the canvas to three quarters,
   so the next tick draws over fading trails. */
/* UB: the event runner invokes a void event. */
#if FIX_FUNCTION_SIGNATURES
void btElementGlowUploadStarField(void)
{
#else
Uint32 btElementGlowUploadStarField(void)
{
#endif
	void		*src = HeapSlots[HEAP_SLOT_BATTLE_CANVAS];
	SPARK_STATE *state = HeapSlots[HEAP_SLOT_BATTLE_EFFECT];

	if(src != 0) {
		if(state->ready != 0) {
			state->ready = 0;
			syCanvasCopyAndScaleThreeQuarters(
				src, BG_CHAR_BLOCK(1), BG_CHAR_SIZE);
		}
	}
}

/* The two events as the event runner takes them: without UBFIX they are
   declared returning a value, and are cast to EVENT_FUNC. */
#define STAR_FIELD_UPDATE_FUNC \
	WHEN(FIX_FUNCTION_SIGNATURES, btElementGlowUpdateStarField, \
		((EVENT_FUNC)btElementGlowUpdateStarField))
#define STAR_FIELD_UPLOAD_FUNC \
	WHEN(FIX_FUNCTION_SIGNATURES, btElementGlowUploadStarField, \
		((EVENT_FUNC)btElementGlowUploadStarField))

/* Starts a star field of the element: sixteen sparks on random rings and three
   rings 120 degrees apart. Returns the upload event's index. */
Sint32 btElementGlowInitStarField(Sint32 element)
{
	SPARK_STATE *state;
	SPARK		*spark;
	RING		*ring;
	Sint32		 effectIndex;
	Uint8		*graphics;
	/* LEFTOVER(frame): an unused local the ROM's frame reserves and never
	 * touches. */
	UNUSED SPARK unused;

	BattleBackgroundStateWork->mode = BATTLE_BACKGROUND_FIXED;
	state = syHeapSlotAllocateEWRAM(HEAP_SLOT_BATTLE_EFFECT, sizeof *state);
	syHeapSlotAllocate(
		HEAP_SLOT_BATTLE_CANVAS, STAR_FIELD_SIZE * STAR_FIELD_SIZE);
	/* FAKEMATCH(dead): an empty once-only loop. */
	do {
	} while(0);
	/* Each spark at a random angle and height, swirling inward; it lives a
	   tick for every four pixels it starts out from the centre, and one
	   more. */
	for(effectIndex = 0; effectIndex < STAR_FIELD_SPARKS; effectIndex++) {
		Sint32 angle = syRandom();
		Uint32 radius = syRandom() + toFIXED(1);
		Uint32 halfRadius = radius >> 1;

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
		Sint32 radius = 4 * STAR_FIELD_PIXEL;
		Sint32 angle;

		ring = &state->rings[effectIndex];
		angle = effectIndex * (DEGtoANG(120));
		ring->x = syMulFX(syCos(angle), radius);
		ring->y = syMulFX(sySin(angle), radius);
		ring->velocityX = syMulFX(syCos(angle), STAR_FIELD_PIXEL / 2);
		ring->velocityY = syMulFX(sySin(angle), STAR_FIELD_PIXEL / 2);
		ring->age = 0;
	}
	state->ticks = 0;
	state->ready = 0;
	state->canvasOrigin.queued = 0;
	syWordsClear(
		HeapSlots[HEAP_SLOT_BATTLE_CANVAS], STAR_FIELD_SIZE * STAR_FIELD_SIZE);
	/* The sprite sheet's colours, then its pictures; then the element's own
	   colours over them. */
	graphics = syResourceGetTableEntry(RESOURCE_BLUE_RING_SHEET);
	syWordsCopy(BG_PLTT, graphics, STAR_FIELD_COLORS * 2);
	graphics += STAR_FIELD_COLORS * 2;
	syDecompressTaggedLZ(graphics, state);
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
	syDMAXCopy(
		graphics, BG_PLTT, DMA_COPY_WORDS(STAR_FIELD_COLORS / 2), REG_DMA3);
	IO_WRITE32(0, &REG_BG2X);
	IO_WRITE32(0, &REG_BG2Y);
	IO_WRITE16(Q_8_8(1), &REG_BG2PA);
	IO_WRITE16(0, &REG_BG2PB);
	IO_WRITE16(0, &REG_BG2PC);
	IO_WRITE16(Q_8_8(1), &REG_BG2PD);
	btEffectBuildBlitterStub(HEAP_SLOT_RAM_CODE, 7, 7,
		BLITTER_CLIP_X | BLITTER_CLIP_Y, COMPOSITE_ADDITIVE);
	btEffectBuildBlitterStub(HEAP_SLOT_RAM_CODE_PAIRED, 7, 7,
		BLITTER_CLIP_X | BLITTER_CLIP_Y, COMPOSITE_LIGHTEN);
	sySetEvent(STAR_FIELD_UPDATE_FUNC, EVENT_PASS_TICK);
	return sySetEvent(STAR_FIELD_UPLOAD_FUNC, EVENT_PASS_TICK);
}

/* Ends a star field and turns BG2 off. */
Sint32 btElementGlowEnd(Sint32 unused)
{
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

/* Copies count colours from source to destination, each channel scaled by scale
   (16.16, at most 1), and returns 0. */
Sint32 btGraphicsScaleRGB555Clamped(
	Uint16 *src, Uint16 *dest, FIXED scale, Sint32 count)
{
	Sint32 i;

	if(scale > toFIXED(1))
		scale = toFIXED(1);
	/* Each channel scaled in place: a 16.16 scale of at most 1 keeps it inside
	   its own five bits. */
	for(i = 0; i < count; i++) {
		Uint32 redMask = RGB(RGB_LEVEL_MAX, 0, 0);
		Uint32 greenMask = RGB(0, RGB_LEVEL_MAX, 0);
		Uint32 blueMask = RGB(0, 0, RGB_LEVEL_MAX);
		Uint32 color = *src;
		Uint32 red = color & redMask;
		Uint32 green = color & greenMask;
		Uint32 blue = blueMask & color;

		red *= scale;
		green *= scale;
		blue *= scale;
		color = (FIXEDtoINT(red) & redMask) | (FIXEDtoINT(green) & greenMask);
		color |= FIXEDtoINT(blue) & blueMask;
		*dest = color;
		src++;
		dest++;
	}
	return 0;
}

/* Plays a glow of the element about a unit, and returns when it is over: the
   star field, the streaks, the pillar or the burst. */
void btElementGlowPlay(
	Sint32 unitId, Sint32 element, Sint32 mode, Sint32 variant)
{
	/* LEFTOVER(frame): an unused local the ROM's frame reserves and never
	 * touches. */
	UNUSED Sint32		 spare[10];
	SCREEN_POINT		 starFieldPos;
	SCREEN_POINT		 streakPos;
	BATTLE_STAGING_STATE pillarLaunch;
	BATTLE_STAGING_STATE burstLaunch;
	/* The heap slots read from the camera's, as the battle effects read theirs
	   from the effect's (EFFECT_SLOT). */
	void		   **slots = &HeapSlots[HEAP_SLOT_CAMERA];
	volatile Uint16 *ime;
	BATTLE_STATE	*battle =
		HEAP_SLOT_FROM(slots, HEAP_SLOT_CAMERA, HEAP_SLOT_BATTLE);
	SPARK_STATE *state;
	Sint32		 frame;
	Sint32		*brightness;

	sySynch(1);
	btBackgroundSetMode(BATTLE_BACKGROUND_FIXED, battle->bg, 0);
	syWordsClear(BG_CHAR_BLOCK(1), BG_CHAR_SIZE);
	syIOWriteQueueStore16((Uint32)&REG_DISPCNT,
		DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG1_ON |
			DISPCNT_BG2_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON);
	syIOWriteQueueStore16((Uint32)&REG_BG2CNT,
		BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7));
	syIOWriteQueueStore16((Uint32)&REG_BLDCNT,
		BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL);
	sySynch(1);
	IO_WRITE16(WIN_RANGE(0, DISPLAY_WIDTH), &REG_WIN0H);
	IO_WRITE16(WIN_RANGE(16, 136), &REG_WIN0V);
	IO_WRITE16(WININ_WIN0_ALL, &REG_WININ);
	IO_WRITE16(WINOUT_WIN01_BG0 | WINOUT_WIN01_OBJ, &REG_WINOUT);
	if(mode == GLOW_STAR_FIELD) {
		/* The star field for 45 ticks, the background darkening for the first
		   25. */
		syIOWriteQueueStore16(
			(Uint32)&REG_BLDALPHA, BLEND_ALPHA(14, BLEND_WEIGHT_MAX));
		btElementGlowInitStarField(element);
		for(frame = 0; frame < STAR_FIELD_TICKS; frame++) {
			brightness = &battle->brightness;
			ime = &REG_IME;
			state = HeapSlots[HEAP_SLOT_BATTLE_EFFECT];
			if(frame <= 24)
				btGraphicsScaleRGB555Clamped(battle->palette, BATTLE_BG_PALETTE,
					*brightness = toFIXED(1) - frame * BACKGROUND_LEVEL_STEP,
					BATTLE_BACKGROUND_COLORS);
			btMotionProjectScaledPos(
				unitId, syScreenPointCoordinates(&starFieldPos));
			state->canvasOrigin.x = (STAR_FIELD_CENTER - (Uint32)starFieldPos.x)
				<< 8;
			state->canvasOrigin.y = (STAR_FIELD_CENTER - (Uint32)starFieldPos.y)
				<< 8;
			IO_WRITE_QUEUE_PUSH_IME(ime, (Uint32)&REG_BG2X,
				(Uint32)&state->canvasOrigin.x, DMA_COPY_WORDS(2));
			state->canvasOrigin.queued = 1;
			sySynch(1);
		}
		btElementGlowEnd(element);
	} else if(mode == GLOW_STREAKS) {
		/* The streaks for 40 ticks, their canvas centred on the unit
		   likewise. */
		BATTLE_EFFECT_STATE *effectState;

		/* The effect uses its image area to stage the tracked canvas origin. */
		btEffectStartElementStreaksStub(element);
		for(frame = 0; frame < 40; frame++) {
			effectState = HeapSlots[HEAP_SLOT_BATTLE_EFFECT];
			btMotionProjectScaledPos(
				unitId, syScreenPointCoordinates(&streakPos));
			effectState->trackedCanvas.origin.x =
				(STAR_FIELD_CENTER - (Uint32)streakPos.x) << 8;
			effectState->trackedCanvas.origin.y =
				(STAR_FIELD_CENTER - (Uint32)streakPos.y) << 8;
			IO_WRITE_QUEUE_PUSH((Uint32)&REG_BG2X,
				(Uint32)&effectState->trackedCanvas.origin.x,
				DMA_COPY_WORDS(2));
			effectState->trackedCanvas.origin.queued = 1;
			sySynch(1);
		}
		btEffectEndElementStreaksStub();
	} else if(mode == GLOW_PILLAR) {
		/* The pillar and the burst run a battle effect with the unit as its
		   user and only target. */
		pillarLaunch.effect.prelude = 0;
		pillarLaunch.effect.kind = element;
		pillarLaunch.effect.variant = variant;
		pillarLaunch.effect.user = unitId;
		pillarLaunch.effect.targets[0] = unitId;
		pillarLaunch.effect.firstTarget = unitId;
		pillarLaunch.effect.targetCount = 1;
		pillarLaunch.effect.range = 1;
		btEffectRunElementPillarStub(&pillarLaunch.effect);
	} else {
		burstLaunch.effect.kind = element;
		burstLaunch.effect.prelude = 0;
		burstLaunch.effect.variant = 0;
		burstLaunch.effect.user = unitId;
		burstLaunch.effect.targets[0] = unitId;
		burstLaunch.effect.firstTarget = unitId;
		burstLaunch.effect.targetCount = 1;
		burstLaunch.effect.range = 1;
		btEffectRunElementBurstStub(&burstLaunch.effect);
	}
}

/* Copies the background's saved palette back undarkened, as the star field of
   btElementGlowPlay leaves it dark. */
void btBackgroundRestorePalette(void)
{
	btBackgroundSetPaletteLevel(0, 0);
}

/* Removes the VCount interrupt's handler, as btTransitionRun installs one;
   nothing calls it. */
void btIRQRemoveVCountHandler(void)
{
	syIntFunction(IRQ_VCOUNT, 0, 0);
}

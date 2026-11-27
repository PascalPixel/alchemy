// Effect positions, projection and id dispatch helpers: a unit projected onto the screen, the
// dispatch of the second range of effect kinds, the plain attack's slash and the critical hit's
// gathering light and crescent, and a unit pushed back by an effect. Part of battle_effect.c.

#include "enemy_ids.h"

// Projects a unit onto the screen, raised by half its height, from the battle camera.
Sint32 btUnitProjectToScreen(Sint32 unit, Sint32 *screen)
{
	BATTLE_CAMERA *camera;
	MOTION_OBJECT *object;
	SPR           *info;
	FIXED          scale;

	camera = RamHeapSlots->cameraState;
	object = btObjectGetStub(unit)->object;
	info   = btActorGetSpriteStub(object, 0);
	EFFECT_VIEW_FROM_CAMERA(camera);
	scale      = syMulFX(syConvert3Dto2D(&object->x, screen), info->scale);
	screen[1] -= syMulFX(scale, FIXEDtoINT(btUnitGetEffectHeightStub(unit)) >> 1);
	return 0;
}

// Unused: reads the held keys when the first target is an enemy and does nothing with them.
UNUSED void btEffectReadKeysForEnemyTarget(void)
{
	BATTLE_EFFECT_STATE *state;
	BATTLE_EFFECT_PARM  *args;

	state = RamHeapSlots->battleEffect;
	args  = state->args;

	// DEAD: nothing uses the held keys read here; the ROM keeps the test and the load.
	if (args->targets[0] >= FIRST_ENEMY_UNIT)
		(void)PerWork.data;
}

// Where the casting impacts' effect kinds start, CASTING_IMPACT_KINDS of them. Below them are the
// classes' plain attacks (ATTACK_EFFECT), and from EFFECT_ID_CRITICAL_OFFSET on the same attacks
// landing as critical hits.
#define EFFECT_RANGE_CASTING_IMPACT 100
#define CASTING_IMPACT_KINDS        101

// Runs a battle effect from the second range of kinds: the casting impacts, the plain attacks'
// beams and their critical hits' ring and sparks, and the canvas burst.
void btEffectRunByIdRange(BATTLE_EFFECT_PARM *args)
{
	BATTLE_EFFECT_STATE *state;
	Sint32               kind;
	Sint32               impact;

	syHeapSlotAllocateEWRAM(HEAP_SLOT_BATTLE_SHEET, BATTLE_SHEET_BYTES);
	syHeapSlotAllocate(HEAP_SLOT_BATTLE_EFFECT, BATTLE_EFFECT_STATE_SIZE);
	syHeapSlotAllocate(HEAP_SLOT_BATTLE_CANVAS, CANVAS_BYTES);
	state                      = RamHeapSlots->battleEffect;
	state->args                = args;
	state->framesSinceTransfer = 1;
	kind                       = args->kind;
	impact                     = kind - EFFECT_RANGE_CASTING_IMPACT;
	if ((Uint32)impact < CASTING_IMPACT_KINDS)
	{
		btEffectRunCastingImpact(args, impact);
	}
	else if (kind == EFFECT_ID_CRITICAL_OFFSET + ATTACK_EFFECT_BURST || kind == ATTACK_EFFECT_BURST)
	{
		btEffectRunBallAttack(args);
	}
	else if (kind >= EFFECT_ID_CRITICAL_OFFSET)
	{
		btEffectRunCriticalHit(args);
	}
	else
	{
		btEffectRunAttackSlash(args);
	}
	syHeapSlotRelease(HEAP_SLOT_BATTLE_CANVAS);
	syHeapSlotRelease(HEAP_SLOT_BATTLE_EFFECT);
	syHeapSlotRelease(HEAP_SLOT_BATTLE_SHEET);
}

#define BEAM_TICKS              32 // the slash's and the critical hit's length
#define BEAM_SPARKS             64 // sparks in state->particles, drawn in pairs
// The ticks the first spark lives, shrinking as it goes; each later one lives a tick longer.
#define BEAM_SPARK_LIFE         16
// BG1's scroll that shows the canvas, 96 pixels down BG1's map (btEffectSetupCanvasTileMap), from
// the battle view's top line.
#define BEAM_CANVAS_SCROLL_Y    (96 - BATTLE_VIEW_TOP)
// The critical hit's camera zoom, in percent, held while it gathers.
#define CRITICAL_HIT_ZOOM       130
// The motes that close in on the attacker, and how far either side of the centre their depth is
// clamped.
#define CRITICAL_HIT_MOTES      64
#define CRITICAL_HIT_MOTE_DEPTH 60

// A plain attack: a slash or streak sweeps from the user over its first target, a yellow orb
// flashes over the target and sparks fly off.
void btEffectRunAttackSlash(BATTLE_EFFECT_PARM *args)
{
	SCREEN_POINT         screenPos;
	SCREEN_POINT         anchor;
	SCREEN_POINT         screen;
	BATTLE_EFFECT_STATE *state;
	void                *canvas;
	Sint32               scrollX;
	Uint8               *sheet;
	BATTLE_CAMERA       *camera;
	Sint32               kind;
	UNIT                *unit;
	DRAW_RECTANGLE       draw[2];
	MOTION_OBJECT       *target;
	Sint32               half;
	Sint32               frame;
	Sint32               i;

	state  = RamHeapSlots->battleEffect;
	canvas = RamHeapSlots->battleCanvas;
	sheet  = RamHeapSlots->battleSheet;
	camera = RamHeapSlots->cameraState;
	kind   = args->kind;
	unit   = syUnitGet(args->user);
	sySynch(1);
	btEffectConfigureEffectDisplay();
	btEffectSetupCanvasTileMap();
	REG_BG1CNT = BGCNT_256_COLORS | BGCNT_SCREEN_BASE(31);
	sySynch(1);
	if (kind == ATTACK_EFFECT_RISING_SLASH || kind == ATTACK_EFFECT_RISING_SLASH_LOW)
	{
		if (args->side == EFFECT_SIDE_ENEMIES)
		{
			EFFECT_LOAD_BLITTER(
				HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y, BLITTER_ADDITIVE);
			EFFECT_LOAD_BLITTER(
				HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y, BLITTER_MAX);
		}
		else
		{
			EFFECT_LOAD_BLITTER(
				HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP, BLITTER_ADDITIVE);
			EFFECT_LOAD_BLITTER(
				HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP, BLITTER_MAX);
		}
	}
	else
	{
		if (args->side == EFFECT_SIDE_ENEMIES)
		{
			EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
			EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
		}
		else
		{
			EFFECT_LOAD_BLITTER(
				HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_ADDITIVE);
			EFFECT_LOAD_BLITTER(
				HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
		}
	}
	draw[0] = RamHeapSlots->ramCode;
	draw[1] = RamHeapSlots->ramCodePaired;
	sySynch(1);
	if (kind == ATTACK_EFFECT_WAVE)
	{
		btResourceLoadAndDecompress(RESOURCE_WAVE_SHEET, state->sheet, 1, 1);
	}
	else if (kind == ATTACK_EFFECT_THRUST)
	{
		btResourceLoadAndDecompress(RESOURCE_BEAM_IMAGE, state->sheet, 0, 0);
	}
	else
	{
		switch (kind)
		{
			case 0: // a class with no attack effect slashes
			case ATTACK_EFFECT_SLASH:
			case ATTACK_EFFECT_RISING_SLASH:
			case ATTACK_EFFECT_RISING_SLASH_LOW:
				btResourceLoadAndDecompress(RESOURCE_BLUE_ARC_SHEET_A, state->sheet, 1, 1);
				break;
			case ATTACK_EFFECT_CLAW:
				btResourceLoadAndDecompress(RESOURCE_BLUE_ARC_SHEET_B, state->sheet, 1, 1);
				break;
		}
	}
	// An enemy's sparks take the Mars Djinn's colours, a party member's the star crescent's.
	syDMAXCopy(syResourceGetTableEntry(args->user > ALLY_MAX ? RESOURCE_MARS_DJINN_SMALL_SHEET
	                                                         : RESOURCE_STAR_CRESCENT_SHEET),
	           (void *)BG_PLTT,
	           DMA_COPY_WORDS(SHEET_PALETTE_BYTES / 4),
	           REG_DMA3);
	sySynch(1);
	btResourceLoadAndDecompress(RESOURCE_BLUE_GLOW_SHEET, sheet, 0, 0);
	btResourceLoadAndDecompress(RESOURCE_YELLOW_ORB_SHEET, RamMapMetatileBuffer, 1, 0);
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
	REG_BG1CNT = BGCNT_PRIORITY(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(31);
	// Scroll BG1 so the first target falls on the canvas.
	btEffectPosProjectSprite(args->targets[0], &anchor);
	if (args->side == EFFECT_SIDE_ENEMIES)
		scrollX = 96 - anchor.x;
	else
		scrollX = 32 - anchor.x;
	if (scrollX > 0)
		scrollX = 0;
	if (scrollX < -128)
		scrollX = -128;
	anchor.x          += scrollX;
	BgScrollWork[1].x  = scrollX;
	BgScrollWork[1].y  = BEAM_CANVAS_SCROLL_Y;
	sySynch(1);
	// Sparks fly from the target, mostly rising.
	target = btObjectGetStub(args->targets[0])->object;
	half   = btUnitGetEffectHeightStub(args->targets[0]) / 2;
	for (i = 0; i != BEAM_SPARKS; i++)
	{
		PARTICLE *spark = &state->particles[i];

		SET_POSITION(spark, target->x, target->y + half, target->z);
		spark->velocityX = (syRandom() & 255) << 10;
		spark->velocityY = ((syRandom() & 255) - 32) << 10;
		spark->velocityZ = ((syRandom() & 255) - 127) << 10;
		if (spark->x > 0)
			spark->velocityX = -spark->velocityX;
		spark->velocityX = -spark->velocityX;
		spark->variant   = i + BEAM_SPARK_LIFE;
	}

	for (frame = 0; frame != BEAM_TICKS; frame++)
	{
		if (frame == 5)
		{
			if (btSpriteHasHeavyImpactStub(unit->enemyId))
				EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
			else
				EFFECT_SHOW_DAMAGE(SE_IMPACT);
		}
		// The target recoils: hurled, hopping, shoved or nudged by the kind.
		if (frame == 4)
		{
			if (kind == ATTACK_EFFECT_HURL)
				btMotionApplyKnockbackStub(args->targets[0], KNOCKBACK_HURL);
			else if (kind == ATTACK_EFFECT_RISING_SLASH || kind == ATTACK_EFFECT_RISING_SLASH_LOW)
				btMotionApplyKnockbackStub(args->targets[0], KNOCKBACK_HOP);
			else if (kind == ATTACK_EFFECT_SHOVE)
				btEffectStartActorMotion(
					args->targets[0], 1, toFIXED(1), toFIXED(4), toFIXED(3), 150);
			else
				btMotionApplyKnockbackStub(args->targets[0], KNOCKBACK_NUDGE);
		}
		// The slash or streak plays from the user.
		btEffectPosProjectScaledBattler(args->user, &screenPos);
		screenPos.y += 16;
		if (kind == ATTACK_EFFECT_RISING_SLASH_LOW)
			screenPos.y += 16;
		if (kind == ATTACK_EFFECT_WAVE)
		{
			// Six pictures, two ticks each, played backwards.
			if (frame <= 11)
			{
				if (args->side == EFFECT_SIDE_ENEMIES)
					draw[0](canvas,
					        state->sheet + (5 - frame / 2) * PICTURE_BYTES(WAVE),
					        screenPos.x + scrollX - WAVE_WIDTH,
					        screenPos.y - WAVE_HEIGHT / 2,
					        WAVE_WIDTH,
					        WAVE_HEIGHT);
				else
					draw[0](canvas,
					        state->sheet + (5 - frame / 2) * PICTURE_BYTES(WAVE),
					        screenPos.x + scrollX,
					        screenPos.y - WAVE_HEIGHT / 2,
					        WAVE_WIDTH,
					        WAVE_HEIGHT);
			}
		}
		else if ((Uint32)kind <= ATTACK_EFFECT_CLAW || kind == ATTACK_EFFECT_RISING_SLASH
		         || kind == ATTACK_EFFECT_RISING_SLASH_LOW)
		{
			// Six pictures, two ticks each.
			if (frame <= 11)
			{
				if (args->side == EFFECT_SIDE_ENEMIES)
					draw[0](canvas,
					        state->sheet + frame / 2 * PICTURE_BYTES(BLUE_ARC),
					        screenPos.x + scrollX - BLUE_ARC_WIDTH,
					        screenPos.y - 40,
					        BLUE_ARC_WIDTH,
					        BLUE_ARC_HEIGHT);
				else
					draw[0](canvas,
					        state->sheet + frame / 2 * PICTURE_BYTES(BLUE_ARC),
					        screenPos.x + scrollX,
					        screenPos.y - 40,
					        BLUE_ARC_WIDTH,
					        BLUE_ARC_HEIGHT);
			}
		}
		else if (kind == ATTACK_EFFECT_THRUST)
		{
			// Six pictures, three ticks each, each reaching further.
			if (frame <= 17)
			{
				Sint32 picture = frame / 3;

				if (args->side == EFFECT_SIDE_ENEMIES)
					draw[0](canvas,
					        state->sheet + BeamPictures[picture],
					        screenPos.x + BeamPictureReaches[picture] + scrollX - 58,
					        screenPos.y - BeamPictureHeights[picture] / 2,
					        BeamPictureWidths[picture],
					        BeamPictureHeights[picture]);
				else
					draw[0](canvas,
					        state->sheet + BeamPictures[picture],
					        screenPos.x - BeamPictureReaches[picture] + scrollX
					            - BeamPictureWidths[picture] + 58,
					        screenPos.y - BeamPictureHeights[picture] / 2,
					        BeamPictureWidths[picture],
					        BeamPictureHeights[picture]);
			}
		}
		// The yellow orb flashes over the target, six pictures, two ticks each.
		if (frame >= 4 && frame <= 15)
			draw[0](canvas,
			        RamMapMetatileBuffer + (frame - 4) / 2 * PICTURE_BYTES(YELLOW_ORB),
			        anchor.x - 16,
			        anchor.y - YELLOW_ORB_HEIGHT / 2,
			        YELLOW_ORB_WIDTH,
			        YELLOW_ORB_HEIGHT);
		EFFECT_VIEW_FROM_CAMERA(camera);
		// The sparks fly, two to each particle, through the two blitters in turn.
		if (frame >= 4 && frame <= 31)
		{
			for (i = 0; i != BEAM_SPARKS; i++)
			{
				Sint32    pair  = i / 2;
				PARTICLE *spark = &state->particles[pair];
				Sint32    size  = spark->variant;

				if (size > 0)
				{
					syConvert3Dto2D(&spark->x, &screen.x);
					screen.x  += scrollX;
					size     >>= 3;
					size      += 2;
					EFFECT_DRAW_FLARE(draw[pair & 1], canvas, sheet, screen.x, screen.y, size);
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

// A critical hit: motes of light close in on the attacker as it hangs in its leap, then a crescent
// slash sweeps across the first target and sparks fly from it.
void btEffectRunCriticalHit(BATTLE_EFFECT_PARM *args)
{
	SCREEN_POINT         screenPos;
	SCREEN_POINT         anchor;
	SCREEN_POINT         screen;
	SCREEN_POINT         spot;
	BATTLE_EFFECT_STATE *state;
	void                *canvas;
	Sint32               scrollX;
	Uint8               *sheet;
	BATTLE_CAMERA       *camera;
	Sint32               velocityX;
	Sint32               velocityY;
	Sint32               velocityZ;
	Sint32               acceleration;
	Sint32               strength;
	DRAW_RECTANGLE       draw[2];
	HEAP_SLOTS          *heap;
	MOTION_OBJECT       *object;
	MOTION_OBJECT       *target;
	Uint16              *background;
	Sint32               bytes = CANVAS_VIEW_BYTES;
	Sint32               big;
	Sint32               half;
	Sint32               frame;
	Sint32               i;

	state  = RamHeapSlots->battleEffect;
	canvas = RamHeapSlots->battleCanvas;
	sheet  = RamHeapSlots->battleSheet;
	camera = RamHeapSlots->cameraState;
	// DEAD: never true, as only kinds from EFFECT_ID_CRITICAL_OFFSET reach this runner
	// (btEffectRunByIdRange), so big stays 1; the ROM keeps the compare and the store.
	big    = 1;
	if (args->kind < EFFECT_ID_CRITICAL_OFFSET)
		big = 0;
	btCameraSetViewAtPairMidpointStub(args->user, args->firstTarget, CRITICAL_HIT_ZOOM);
	sySynch(1);
	btEffectSetupCanvasTileMap();
	REG_BG1CNT = BGCNT_256_COLORS | BGCNT_SCREEN_BASE(31);
	if (args->side == EFFECT_SIDE_ENEMIES)
	{
		EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
		EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
	}
	else
	{
		EFFECT_LOAD_BLITTER(
			HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_ADDITIVE);
		EFFECT_LOAD_BLITTER(
			HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
	}
	heap    = RamHeapSlots;
	draw[0] = heap->ramCode;
	draw[1] = heap->ramCodePaired;
	btCameraSetViewAtPairMidpointStub(args->user, args->firstTarget, CRITICAL_HIT_ZOOM);
	sySynch(1);
	btResourceLoadAndDecompress(RESOURCE_STAR_CRESCENT_SHEET, state->sheet, 1, 0);
	btCameraSetViewAtPairMidpointStub(args->user, args->firstTarget, CRITICAL_HIT_ZOOM);
	sySynch(1);
	btResourceLoadAndDecompress(RESOURCE_GREY_CRESCENT_SHEET, RamMapMetatileBuffer, 1, 1);
	// An enemy's sparks take the Mars Djinn's colours.
	if (args->user > ALLY_MAX)
		syDMAXCopy(syResourceGetTableEntry(RESOURCE_MARS_DJINN_SMALL_SHEET),
		           (void *)BG_PLTT,
		           DMA_COPY_WORDS(SHEET_PALETTE_BYTES / 4),
		           REG_DMA3);
	btCameraSetViewAtPairMidpointStub(args->user, args->firstTarget, CRITICAL_HIT_ZOOM);
	sySynch(1);
	btResourceLoadAndDecompress(RESOURCE_BLUE_GLOW_SHEET, sheet, 0, 0);
	btCameraSetViewAtPairMidpointStub(args->user, args->firstTarget, CRITICAL_HIT_ZOOM);
	sySynch(1);
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);

	// The gathering.
	if (big == 1)
	{
		object = btObjectGetStub(args->user)->object;
		for (i = 0; i != CRITICAL_HIT_MOTES; i++)
		{
			PARTICLE *mote = &state->particles[i];

			SET_POSITION(mote, (syRandom() & 63) + 16, 0, 0);
			mote->velocityX = syRandom() & TRIG_ANGLE_MASK;
			mote->velocityY = syRandom() & TRIG_ANGLE_MASK;
			mote->velocityZ = syRandom() & TRIG_ANGLE_MASK;
		}
		fdActorSetAnimationSpeedStub(object, 0);
		velocityX                      = object->velocityX;
		velocityY                      = object->velocityY;
		velocityZ                      = object->velocityZ;
		strength                       = object->verticalMotionStrength;
		acceleration                   = object->acceleration;
		object->velocityX              = 0;
		object->velocityY              = 0;
		object->velocityZ              = 0;
		object->acceleration           = 0;
		object->verticalMotionStrength = 0;
		btEffectPosProjectScaledBattler(args->user, &screenPos);
		scrollX           = 64 - screenPos.x;
		BgScrollWork[1].x = scrollX;
		BgScrollWork[1].y = BEAM_CANVAS_SCROLL_Y;
		state->fadeFrame  = BACKGROUND_DARKEN_TICKS;
		state->fadeStep   = 0;
		sySetEvent(btEffectDarkenBackgroundStep, EVENT_PASS_TICK);
		sySoundPlayStub(SE_POWER_UP);
		for (frame = 0; frame != BEAM_TICKS; frame++)
		{
			btCameraSetViewAtPairMidpointStub(args->user, args->firstTarget, CRITICAL_HIT_ZOOM);
			// Mote i starts at tick i / 4, 16 to 79 pixels out on its own axes, and moves in 4
			// pixels a tick until it reaches the attacker.
			for (i = 0; i != CRITICAL_HIT_MOTES; i++)
			{
				PARTICLE *mote = &state->particles[i];

				if (mote->x >= 0 && frame >= i / 4)
				{
					Sint32 size = (i & 1) + 5;

					syInitMatrix();
					syRotZ(mote->velocityZ);
					syRotX(mote->velocityX);
					syRotY(mote->velocityY);
					btEffectPosProjectPoint(&mote->x, &screen);
					screen.x += 64;
					screen.y  = screen.y + screenPos.y + 24;
					// DEAD: the depth is clamped and moved to start at 0 but never read; the ROM
					// keeps the clamp and the move.
					CLAMP(screen.depth, -CRITICAL_HIT_MOTE_DEPTH, CRITICAL_HIT_MOTE_DEPTH);
					screen.depth += CRITICAL_HIT_MOTE_DEPTH;
					EFFECT_DRAW_FLARE(draw[0], canvas, sheet, screen.x, screen.y, size);
					mote->x -= 4;
				}
			}
			EFFECT_PRESENT_CANVAS(state);
		}
		syCloseEvent(btEffectDarkenBackgroundStep);
		fdActorSetAnimationSpeedStub(object, ANIM_STEP_NORMAL);
		object->velocityX              = velocityX;
		object->velocityY              = velocityY;
		object->velocityZ              = velocityZ;
		object->acceleration           = acceleration;
		object->verticalMotionStrength = strength;
	}

	// The strike.
	syWordsClearEntry(canvas, bytes);
	syWordsClearEntry((void *)BG_CHAR_BLOCK(1), bytes);
	EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
	REG_BG1CNT = BGCNT_PRIORITY(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(31);
	btEffectPosProjectScaledBattler(args->targets[0], &anchor);
	if (args->side == EFFECT_SIDE_ENEMIES)
		scrollX = 32 - anchor.x;
	else
		scrollX = 96 - anchor.x;
	if (scrollX > 0)
		scrollX = 0;
	if (scrollX < -128)
		scrollX = -128;
	anchor.x          += scrollX;
	BgScrollWork[1].x  = scrollX;
	BgScrollWork[1].y  = BEAM_CANVAS_SCROLL_Y;
	target             = btObjectGetStub(args->targets[0])->object;
	half               = btUnitGetEffectHeightStub(args->targets[0]) / 2;
	for (i = 0; i != BEAM_SPARKS; i++)
	{
		PARTICLE *spark = &state->particles[i];

		SET_POSITION(spark, target->x, target->y + half, target->z);
		spark->velocityX = (syRandom() & 255) << 10;
		spark->velocityY = (syRandom() & 255) << 10;
		spark->velocityZ = ((syRandom() & 255) - 127) << 10;
		if (spark->x > 0)
			spark->velocityX = -spark->velocityX;
		spark->variant = i + BEAM_SPARK_LIFE;
	}

	for (frame = 0; frame != BEAM_TICKS; frame++)
	{
		if (frame == 5)
			EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
		if (frame == 4)
			btMotionApplyKnockbackStub(args->targets[0], KNOCKBACK_NUDGE);
		btEffectPosProjectScaledBattler(args->user, &screenPos);
		screenPos.y += 16;
		// The slash: a star, an arc, then two crescents.
		if (frame <= 1)
			draw[0](canvas, state->sheet, 0, 0, SLASH_PICTURE_SIZE, SLASH_PICTURE_SIZE);
		else if (frame <= 3)
			draw[0](canvas, RamMapMetatileBuffer, -4, 0, CANVAS_SIZE, CANVAS_SIZE);
		else if (frame <= 5)
			draw[0](canvas, RamMapMetatileBuffer + CANVAS_BYTES, -4, 0, CANVAS_SIZE, CANVAS_SIZE);
		else if (frame <= 7)
			draw[0](canvas,
			        state->sheet + SLASH_PICTURE_SIZE * SLASH_PICTURE_SIZE,
			        0,
			        0,
			        SLASH_PICTURE_SIZE,
			        SLASH_PICTURE_SIZE);
		EFFECT_VIEW_FROM_CAMERA(camera);
		// The sparks fly, two to each particle, through the two blitters in turn.
		if (frame >= 4 && frame <= 31)
		{
			for (i = 0; i != BEAM_SPARKS; i++)
			{
				Sint32    pair  = i / 2;
				PARTICLE *spark = &state->particles[pair];
				Sint32    size  = spark->variant;

				if (size > 0)
				{
					btEffectPosProjectPoint(&spark->x, &spot);
					spot.x  += scrollX;
					size   >>= 3;
					size    += 2;
					spot.y  += 16;
					EFFECT_DRAW_FLARE(draw[pair & 1], canvas, sheet, spot.x, spot.y, size);
					btParticleAdvanceWithGravity3D(spark, 60, -toFIXED(1) / 64);
					spark->variant--;
				}
			}
		}
		EFFECT_PRESENT_CANVAS(state);
	}

	// The background's palette steps back over seven ticks.
	syCloseEvent(btEffectTransferCanvas);
	EFFECT_RELEASE_BLITTERS();
	BgScrollWork[1].y = BATTLE_BACKGROUND_SCROLL_Y;
	for (frame = 0, background = &RamHeapSlots->battleState->bg; frame != 7; frame++)
	{
		btBackgroundSetPaletteLevelStub(*background, 6 - frame);
		sySynch(1);
	}
	btEffectHideBg1AndRestoreBackdrop();
}

// Knocks a unit back in its damage pose with the caller's acceleration, top speed, hop and reach in
// percent, as btMotionApplyKnockback does from its tables; a unit already moving up or down always
// takes the hop, a still one only when `force` is set. The Fusion Dragon and the Doom Dragon's
// three forms only take the pose.
void btEffectStartActorMotion(Sint32 id,
                              Sint32 force,
                              Sint32 acceleration,
                              Sint32 speedLimit,
                              Sint32 velocityY,
                              Sint32 percent)
{
	BATTLE_OBJECT *slot;
	MOTION_OBJECT *object;
	Uint32         enemy;

	slot   = btObjectGetStub(id);
	object = slot->object;
	enemy  = syUnitGet(id)->enemyId;
	if (enemy != ENEMY_FUSION_DRAGON && enemy != ENEMY_DOOM_DRAGON_A && enemy != ENEMY_DOOM_DRAGON_E
	    && enemy != ENEMY_DOOM_DRAGON_H)
	{
		object->acceleration = acceleration;
		object->speedLimit   = speedLimit;
		if (force != 0 || object->velocityY != 0)
			object->velocityY = velocityY;
		fdActorStartIdleStub(object);
		fdActorSetMoveTargetStub(object, slot->anchorX * percent / 100, 0, slot->anchorZ);
	}
	object->verticalMotionStrength = toFIXED(3) / 5;
	object->verticalMotionPhase    = 0;
	object->autoFaceMotion         = 0;
	fdActorSelectAnimationStub(object, POSE_DAMAGE);
}

#undef EFFECT_RANGE_CASTING_IMPACT
#undef CASTING_IMPACT_KINDS
#undef BEAM_TICKS
#undef BEAM_SPARKS
#undef BEAM_SPARK_LIFE
#undef BEAM_CANVAS_SCROLL_Y
#undef CRITICAL_HIT_ZOOM
#undef CRITICAL_HIT_MOTES
#undef CRITICAL_HIT_MOTE_DEPTH

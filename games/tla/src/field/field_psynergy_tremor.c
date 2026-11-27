/* @file */
/* Tremor: the view swayed from side to side. */
/* Part of field_psynergy.c. */

/* Tremor cast at a chosen object (fdPsynergyDispatch): */
/* fdPsynergyTremor's sideways sway, stronger, with the object's picture */
/* held in the state. */

/* The sprites of RESOURCE_SHAKE_TILES' two tiles the state sets up. */
#define SHAKE_SPRITE_COUNT 64
/* The OBJ palette the shake sprites take. */
#define SHAKE_SPRITE_PALETTE 14
#define PICTURE_SIZE		 4096
#define PICTURE_MAX_PIXELS	 2048

/* How far round the nearby objects' shaking goes each frame: 3/32 of a turn. */
#define SHAKE_STEP (TRIG_TURN * 3 / 32)
/* The objects shake as far as a whole turn for every */
/* STRONG_SHAKE_PIXELS_PER_TURN pixels the view sways past SWAY_SHAKE. */
#define STRONG_SHAKE_PIXELS_PER_TURN 128

/* The stronger sway at an object (16.16 pixels): from SWAY_START up to */
/* STRONG_SWAY_PEAK, ramping more slowly and holding longer. */
#define STRONG_SWAY_PEAK		toFIXED(12)
#define STRONG_SWAY_RAMP		(toFIXED(1) / 10) /* 0.1 pixels a frame */
#define SWAY_SHAKE				toFIXED(4) /* sprites shake only past this */
#define STRONG_SWAY_HOLD_FRAMES 40

/* The stages of a sway of the view (Tremor's and Grind's): it grows, holds, */
/* shrinks and settles, and SWAY_DONE ends it. */
enum {
	SWAY_GROW,
	SWAY_HOLD,
	SWAY_SHRINK,
	SWAY_SETTLE,
	SWAY_DONE = 999,
};

/* Tremor's state (heap slot HEAP_SLOT_BATTLE_EFFECT): the view's sway, the */
/* picture of the shaken actor and its falling pieces. */
typedef struct TREMOR_STATE {
	VIEW_MOVE_STATE view;
	u8 picture[PICTURE_SIZE];
	EFFECT_SPR sprs[SHAKE_SPRITE_COUNT];
	FIELD_PSYNERGY_PARTICLE particles[SHAKE_SPRITE_COUNT];
} TREMOR_STATE;

/* Sets the step of each of a sprite's layers: 0 stops its animation, */
/* ANIM_STEP_NORMAL runs it at normal speed. */
void fdSprSetAnimationSpeedStub(void *sprite, s32 value);

/* Tremor ("Shake objects left and right.") on the chosen object: sways the */
/* view from side to side, as fdPsynergyTremor does with no object: the */
/* swing grows from SWAY_START to STRONG_SWAY_PEAK, holds */
/* STRONG_SWAY_HOLD_FRAMES, shrinks back and stops. Past SWAY_SHAKE the nearby
 */
/* objects' sprites shake with it. The state also takes the object's picture */
/* and SHAKE_SPRITE_COUNT sprites, which nothing here draws. An object whose */
/* picture is larger than PICTURE_MAX_PIXELS gets no sway. */
void fdPsynergyTremorOnTarget(void) {
	TREMOR_STATE *tremorState =
		syHeapSlotAlloc(HEAP_SLOT_BATTLE_EFFECT, sizeof(*tremorState));
	FIELD_EFFECT_STATE *scene = Ram_HeapSlots->fieldEffState;
	FIELD_STATE *field = Ram_HeapSlots->fieldState;
	ACTOR *target = scene->tgt;
	SPR *targetSprite = target->spr;
	s32 tiles;
	s32 shown;
	s32 width;
	s32 showing;
	s32 frameCount;
	s32 phase;
	s32 i;
	s32 tile;
	s32 amplitude;
	s32 state;
	s32 frames;
	FIELD_PSYNERGY_PARTICLE *particle;

	if(target == NULL || targetSprite == NULL ||
	   targetSprite->width * targetSprite->height <= PICTURE_MAX_PIXELS) {
		fdPsynergyFadeToCastCol();
		syDecompressTaggedLz(syResourceGetTableEntry(RESOURCE_SHAKE_TILES),
							 tremorState->picture);
		tiles = syVramCacheFindFreeEntry();
		tile = syVramCacheLoad(tiles, 64, tremorState->picture);
		for(i = 0, particle = tremorState->particles; i < SHAKE_SPRITE_COUNT;
			i++, particle++) {
			EFFECT_SPR *sprite = &tremorState->sprs[i];

			fdEffectSprInit(sprite, 4, 4, 0, tile + (i & 1));
			sprite->oam.fullCol = 0;
			sprite->oam.pal = SHAKE_SPRITE_PALETTE;
			sprite->oam.pri = 0;
			sprite->depth = OAM_DEPTH_WINDOW;
			particle->tmr = -1;
		}
		fdBattleEffectSetObjectUpdateMode(scene->caster, CASTER_LOOK_NORMAL);
		if(target != NULL && targetSprite != NULL) {
			fdSprSetAnimationSpeedStub(targetSprite, 0);
			fdPsynergyDecodePicture((PICTURE_SOURCE *)targetSprite->entries[0],
									tremorState->picture,
									targetSprite->width * targetSprite->height);
		}
		fdViewMoveStart(VIEW_MOVE_X);
		fdViewMoveSaveNearbySprs();
		sySynch(10);

		shown = -1;
		width = targetSprite->width;
		showing = 0;
		phase = 0;
		frameCount = 0;
		amplitude = SWAY_START;
		state = SWAY_GROW;
		frames = 0;
		do {
			if((frameCount & 15) == 15)
				sySndPlayStub(SE_JUMP);

			/* Shake the gathered objects' sprites with the view. */
			if(field->mode == ACTOR_SYSTEM_MAP) {
				for(i = 0; i < tremorState->view.cnt; i++) {
					SPR *sprite = tremorState->view.actors[i]->spr;
					s32 rotation = tremorState->view.sprRotations[i];

					if(amplitude > SWAY_SHAKE)
						sprite->rotation =
							rotation +
							syMulFX(sySin(phase * SHAKE_STEP),
									(amplitude - SWAY_SHAKE) /
										STRONG_SHAKE_PIXELS_PER_TURN);
					else
						sprite->rotation = rotation;
				}
			}

			fdViewMoveSwing(VIEW_MOVE_X, phase++, amplitude);
			switch(state) {
			case SWAY_GROW:
				if(frames == 40)
					showing = 1;
				amplitude += STRONG_SWAY_RAMP;
				if(amplitude >= STRONG_SWAY_PEAK) {
					state = SWAY_HOLD;
					frames = -1;
				}
				break;
			case SWAY_HOLD:
				if(frames == STRONG_SWAY_HOLD_FRAMES) {
					state = SWAY_SHRINK;
					frames = -1;
				}
				break;
			case SWAY_SHRINK:
				if(frames == 50)
					showing = -1;
				amplitude -= STRONG_SWAY_RAMP;
				if(amplitude <= SWAY_START) {
					state = SWAY_SETTLE;
					frames = -1;
				}
				break;
			case SWAY_SETTLE:
				amplitude = 0;
				if(frames == 0)
					state = SWAY_DONE;
				break;
			}

			/* A count up to the picture's width and back that nothing reads. */
			if(showing == 1 && shown <= width)
				shown++;
			if(showing == -1 && shown >= 0)
				shown -= 2;

			sySynch(1);
			frames++;
			frameCount++;
		} while(state != SWAY_DONE);

		if(target != NULL && targetSprite != NULL) {
			fdSprSetAnimationSpeedStub(targetSprite, ANIM_STEP_NORMAL);
			((SPR *)target->spr)->dirty = 1;
		}
		fdViewMoveEnd();
		fdViewMoveRestoreNearbySprs();
		syVramCacheFreeEntry(tiles);
		fdPsynergyFadeFromCastCol();
	}
	syHeapSlotRelease(HEAP_SLOT_BATTLE_EFFECT);
}

#undef STRONG_SWAY_PEAK
#undef STRONG_SWAY_RAMP
#undef STRONG_SWAY_HOLD_FRAMES
#undef STRONG_SHAKE_PIXELS_PER_TURN

/* Tremor as fdPsynergyRun casts it, at no object: the view sways. */

/* The sway's hold while nearby objects shake with it, in place of */
/* SWAY_HOLD_FRAMES. */
#define SWAY_SHAKE_FRAMES 50
/* The objects shake as far as a whole turn for every SHAKE_PIXELS_PER_TURN */
/* pixels the view sways. */
#define SHAKE_PIXELS_PER_TURN 384

/* Sways the view from side to side, with a sound every 16 frames: the swing */
/* grows from SWAY_START to SWAY_PEAK, holds, shrinks back and stops. On a */
/* town or dungeon map the nearby objects' sprites shake with it; when there */
/* are any, the hold lasts SWAY_SHAKE_FRAMES instead of SWAY_HOLD_FRAMES. Then
 */
/* the view bounds and the sprites come back. */
void fdPsynergyTremor(void) {
	/* Tremor's state's size, though the sway holds only the view move. */
	VIEW_MOVE_STATE *viewMoveState =
		syHeapSlotAlloc(HEAP_SLOT_BATTLE_EFFECT, sizeof(TREMOR_STATE));
	FIELD_EFFECT_STATE *scene = Ram_HeapSlots->fieldEffState;
	FIELD_STATE *field = Ram_HeapSlots->fieldState;
	s32 frameCount;
	s32 phase;
	s32 i;
	s32 amplitude;
	s32 state;
	s32 frames;

	fdPsynergyFadeToCastCol();
	fdBattleEffectSetObjectUpdateMode(scene->caster, CASTER_LOOK_NORMAL);
	fdViewMoveStart(VIEW_MOVE_X);
	fdViewMoveSaveNearbySprs();
	sySynch(10);

	phase = 0;
	frameCount = 0;
	amplitude = SWAY_START;
	state = SWAY_GROW;
	frames = 0;
	do {
		if((frameCount & 15) == 15)
			sySndPlayStub(SE_JUMP);

		/* Shake the gathered objects' sprites with the view. */
		if(field->mode == ACTOR_SYSTEM_MAP) {
			for(i = 0; i < viewMoveState->cnt; i++) {
				SPR *sprite = viewMoveState->actors[i]->spr;
				s32 rotation = viewMoveState->sprRotations[i];

				if(amplitude > 0)
					sprite->rotation =
						rotation + syMulFX(sySin(phase * SHAKE_STEP),
										   amplitude / SHAKE_PIXELS_PER_TURN);
				else
					sprite->rotation = rotation;
			}
		}

		fdViewMoveSwing(VIEW_MOVE_X, phase++, amplitude);
		switch(state) {
		case SWAY_GROW:
			amplitude += SWAY_RAMP;
			if(amplitude >= SWAY_PEAK) {
				state = SWAY_HOLD;
				frames = -1;
			}
			break;
		case SWAY_HOLD:
			if(viewMoveState->cnt != 0) {
				if(frames == SWAY_SHAKE_FRAMES) {
					state = SWAY_SHRINK;
					frames = -1;
				}
			}
			else if(frames == SWAY_HOLD_FRAMES) {
				state = SWAY_SHRINK;
				frames = -1;
			}
			break;
		case SWAY_SHRINK:
			amplitude -= SWAY_RAMP;
			if(amplitude <= SWAY_START) {
				state = SWAY_SETTLE;
				frames = -1;
			}
			break;
		case SWAY_SETTLE:
			amplitude = 0;
			if(frames == 0)
				state = SWAY_DONE;
			break;
		}
		sySynch(1);
		frames++;
		frameCount++;
	} while(state != SWAY_DONE);

	fdViewMoveEnd();
	fdViewMoveRestoreNearbySprs();
	fdPsynergyFadeFromCastCol();
	syHeapSlotRelease(HEAP_SLOT_BATTLE_EFFECT);
}

/* Moves rows 1 to rows - 2 of a buffer down one row by DMA, dropping the */
/* last, then clears row 0 (stride bytes per row). */
void fdBattleEffectShiftRowsDown(u8 *base, s32 stride, s32 rows) {
	u8 *source = base + (rows - 2) * stride;
	u8 *destination = base + (rows - 1) * stride;
	s32 i;

	for(i = rows - 1; i != 1; i--) {
		syDMAXCopy(source, destination, DMA_COPY_WORDS(stride / 4), REG_DMA3);
		source -= stride;
		destination -= stride;
	}
	syWordsFillEntry(base, stride, 0);
}
#undef SHAKE_SPRITE_COUNT
#undef SHAKE_SPRITE_PALETTE
#undef PICTURE_SIZE
#undef PICTURE_MAX_PIXELS
#undef SWAY_SHAKE
#undef SWAY_SHAKE_FRAMES
#undef SHAKE_STEP
#undef SHAKE_PIXELS_PER_TURN

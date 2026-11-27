/* @file */
/* A debug sprite viewer. */
/* Part of field.c. */

extern const u16 SpriteViewer_BackdropColors[8];

void fdSprViewerSetLayerPriority(s32 slot, s32 value);
void fdSprViewerSetLayerColMode(s32 slot, s32 value);
s32 fdSprViewerStartStaggeredAnimations(s32 slot, s32 value);
void fdSprViewerSetLayerSpr(s32 slot, s32 value);
void fdSprViewerLayOutRing(s32 x, s32 z, SPR_PLACEMENT *out);

s32 fdSprViewerStepSprId(s32 value, s32 delta);
void fdSprViewerPlaceRing(void);

enum {
	/* The sprite copies and their ring positions; a facing kind places 4, 6, 8
	 */
	/* or 10 of them. */
	UI_SPRITE_VIEWER_COPY_COUNT = 10,
	UI_SPRITE_VIEWER_SPRITE_LIMIT = 716, /* sprite ids 0 to 715 are offered */
	UI_SPRITE_VIEWER_ANIMATION_LIMIT = 100, /* animations 0 to 99 */
	/* A sprite animation priority above 3, which fdSprComposePicture skips. */
	UI_SPRITE_VIEWER_HIDDEN_PRIORITY = 8,
	/* The sprite ids L and R step by while Start is held: any step past one */
	/* goes to the next stop of SpriteViewer_SkipStops. */
	UI_SPRITE_VIEWER_FAST_STEP = 10,
	/* SpriteViewer_SkipStops' entries, its end marker the last. */
	UI_SPRITE_VIEWER_SKIP_STOPS = 10,
	/* The colour SPRITE_COLOR_FILL paints in the viewer (DecodeFillByte): */
	/* colour 3 of the basic sprite palette, orange. */
	UI_SPRITE_VIEWER_FILL_COLOR = 3,
	/* DebugMode while the viewer runs. Every reader tests it only for */
	/* nonzero, so 2 turns on the same debug keys any other value would. */
	UI_SPRITE_VIEWER_DEBUG_MODE = 2
};

/* An angle's bits: shifted up and back down, a value keeps only them. */
#define ANGLE_BITS 16

/* What left and right choose for L and R to change on the selected sprite */
/* animation. */
typedef enum UI_SPR_VIEWER_EDIT_MODE {
	UI_SPRITE_VIEWER_EDIT_ANIMATION,
	UI_SPRITE_VIEWER_EDIT_SPRITE,
	UI_SPRITE_VIEWER_EDIT_PRIORITY,
	UI_SPRITE_VIEWER_EDIT_COLOR,
	UI_SPRITE_VIEWER_EDIT_COUNT
} UI_SPR_VIEWER_EDIT_MODE;

/* One sprite animation the sprite viewer edits. */
typedef struct UI_SPR_VIEWER_LAYER {
	s16 sprId;	/* metadata record selected for this layer */
	s16 anmIdx; /* animation script selected from that metadata */
	u8 enabled; /* zero hides it by a priority above the drawing range */
	s8 pri;		/* drawing order, 0 to 3 */
	s8 colMode; /* SPR_COL_MODE; the viewer cycles through 0 to 15 */
} UI_SPR_VIEWER_LAYER;

/* The debug sprite viewer, in The Broken Seal's form with this game's */
/* changes; it never returns. */
void fdSprViewerRun(void) {
	UI_SPR_VIEWER_LAYER layers[SPRITE_ANIMATION_COUNT];
	volatile u32 zero;
	s32 x;
	s32 y;
	u32 editMode;
	s32 shadowsEnabled;
	u32 backdrop;
	void *ring;
	u32 selectedLayer;
	u32 i;
	s32 spriteId;
	s32 changed;

	x = 144;
	y = 96;
	editMode = UI_SPRITE_VIEWER_EDIT_SPRITE;
	selectedLayer = 0;
	shadowsEnabled = 1;
	backdrop = 0;
	ring = syHeapSlotAllocEwram(HEAP_SLOT_BATTLE, 160);
	DecodeFillByte = UI_SPRITE_VIEWER_FILL_COLOR;
	btDmaFill32Cell(zero, 0, ring, 4);
	btDmaFill32Cell(zero, 0, layers, sizeof(layers));
	spriteId = fdSprViewerStepSprId(-1, 1);
	for(i = 0; i < SPRITE_ANIMATION_COUNT; i++) {
		layers[i].anmIdx = 1;
		layers[i].pri = 1;
		layers[i].sprId = spriteId;
	}
	layers[0].enabled = 1;
	DebugMode = UI_SPRITE_VIEWER_DEBUG_MODE;
	REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
	*(volatile u16 *)BG_PLTT = RGB(0, 0, 31);
	REG_DISPCNT = DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_OBJ_ON;
	syScrFadeFromBlack(1);

restart:
	syHeapInit();
	syInitEvent();
	ring = syHeapSlotAllocEwram(HEAP_SLOT_BATTLE, 160);
	syVramCacheReset();
	fdActorSystemInit(ACTOR_SYSTEM_MAP);
	fdSprPictureCacheLoad(0, MapGridCellBuffer, layers[0].sprId, 0);
	if(fdResourceGetMetadataRecordStub(layers[0].sprId)->drawKind ==
	   ANIM_FACING_TWO_PART)
		fdSprPictureCacheLoad(1, ActorSpriteSlots, layers[0].sprId + 1, 0);
	for(i = 0; i < UI_SPRITE_VIEWER_COPY_COUNT; i++) {
		SPR *sprite;
		s32 part = 0;

		if(fdResourceGetMetadataRecordStub(layers[0].sprId)->drawKind ==
			   ANIM_FACING_TWO_PART &&
		   (i & 1))
			part = 1;
		sprite = fdSprCreate(SPRITE_PICTURE_TAG(part, layers[0].sprId + part));
		fdSprAddAnimation(sprite, layers[1].sprId);
		fdSprAddAnimation(sprite, layers[2].sprId);
		fdSprAddAnimation(sprite, layers[3].sprId);
		sprite->flg = shadowsEnabled;
	}
	for(i = 0; i < SPRITE_ANIMATION_COUNT; i++) {
		if(layers[i].enabled != 0)
			fdSprViewerSetLayerPriority(i, layers[i].pri);
		else
			fdSprViewerSetLayerPriority(i, UI_SPRITE_VIEWER_HIDDEN_PRIORITY);
		fdSprViewerSetLayerColMode(i, layers[i].colMode);
		fdSprViewerStartStaggeredAnimations(i, layers[i].anmIdx);
	}
	fdSprViewerLayOutRing(x, y, ring);
	sySetEvent(fdSprViewerPlaceRing, TASK_PASS_FRAME);

next:
	sySynch(1);
	for(;;) {
		if(InputWork.held & KEY_START) {
			if(InputWork.held & KEY_LEFT)
				x--;
			if(InputWork.held & KEY_RIGHT)
				x++;
			if(InputWork.held & KEY_UP)
				y--;
			if(InputWork.held & KEY_DOWN)
				y++;
			fdSprViewerLayOutRing(x, y, ring);
		}
		else {
			/* DEAD: a read of the repeated keys nothing uses; the ROM keeps the
			 * load. */
			(void)InputWork.repeat;
			if(InputWork.repeat & KEY_UP)
				selectedLayer =
					(selectedLayer - 1) & (SPRITE_ANIMATION_COUNT - 1);
			if(InputWork.repeat & KEY_DOWN)
				selectedLayer =
					(selectedLayer + 1) & (SPRITE_ANIMATION_COUNT - 1);
			if(InputWork.repeat & KEY_LEFT)
				editMode = (editMode - 1) & (UI_SPRITE_VIEWER_EDIT_COUNT - 1);
			if(InputWork.repeat & KEY_RIGHT)
				editMode = (editMode + 1) & (UI_SPRITE_VIEWER_EDIT_COUNT - 1);
		}

		if(InputWork.pressed & KEY_START) {
			SPR *sprites = Ram_HeapSlots->sprs;

			shadowsEnabled ^= 1;
			for(i = 0; i < UI_SPRITE_VIEWER_COPY_COUNT; i++) {
				sprites->flg = shadowsEnabled;
				sprites++;
			}
		}

		switch(editMode) {
		case UI_SPRITE_VIEWER_EDIT_ANIMATION:
			/* The second sprite animation follows the first's animation. */
			if(selectedLayer == 1)
				break;
			if(InputWork.repeat & KEY_L) {
				layers[selectedLayer].anmIdx--;
				if(layers[selectedLayer].anmIdx < 0)
					layers[selectedLayer].anmIdx = 0;
				if(selectedLayer == 0)
					layers[1].anmIdx = layers[0].anmIdx;
				goto next;
			}
			if(!(InputWork.repeat & KEY_R))
				break;
			layers[selectedLayer].anmIdx++;
			if(layers[selectedLayer].anmIdx >
			   UI_SPRITE_VIEWER_ANIMATION_LIMIT - 1)
				layers[selectedLayer].anmIdx =
					UI_SPRITE_VIEWER_ANIMATION_LIMIT - 1;
			if(selectedLayer == 0)
				layers[1].anmIdx = layers[0].anmIdx;
			goto next;

		case UI_SPRITE_VIEWER_EDIT_SPRITE: {
			s32 steps;

			changed = 0;
			steps = 1;
			if(InputWork.held & KEY_START)
				steps = UI_SPRITE_VIEWER_FAST_STEP;
			if(InputWork.repeat & KEY_L) {
				layers[selectedLayer].sprId =
					fdSprViewerStepSprId(layers[selectedLayer].sprId, -steps);
				changed = 1;
			}
			if(InputWork.repeat & KEY_R) {
				layers[selectedLayer].sprId =
					fdSprViewerStepSprId(layers[selectedLayer].sprId, steps);
				changed = 1;
			}
			if(changed == 0)
				break;
			if(selectedLayer == 0)
				goto restart;
			if(layers[selectedLayer].enabled != 0) {
				fdSprViewerSetLayerSpr(selectedLayer,
									   layers[selectedLayer].sprId);
				fdSprViewerSetLayerPriority(selectedLayer,
											layers[selectedLayer].pri);
				fdSprViewerSetLayerColMode(selectedLayer,
										   layers[selectedLayer].colMode);
				fdSprViewerStartStaggeredAnimations(
					selectedLayer, layers[selectedLayer].anmIdx);
			}
			goto next;
		}

		case UI_SPRITE_VIEWER_EDIT_PRIORITY:
			changed = 0;
			if(InputWork.repeat & KEY_L) {
				layers[selectedLayer].pri--;
				if(layers[selectedLayer].pri < 0)
					layers[selectedLayer].pri = 3;
				changed = 1;
			}
			if(InputWork.repeat & KEY_R) {
				layers[selectedLayer].pri++;
				if(layers[selectedLayer].pri > 3)
					layers[selectedLayer].pri = 0;
				changed = 1;
			}
			if(changed == 0)
				break;
			if(layers[selectedLayer].enabled != 0)
				fdSprViewerSetLayerPriority(selectedLayer,
											layers[selectedLayer].pri);
			goto next;

		case UI_SPRITE_VIEWER_EDIT_COLOR:
			changed = 0;
			if(InputWork.repeat & KEY_L) {
				layers[selectedLayer].colMode--;
				if(layers[selectedLayer].colMode < 0)
					layers[selectedLayer].colMode = SPRITE_COLOR_HIDDEN;
				changed = 1;
			}
			if(InputWork.repeat & KEY_R) {
				layers[selectedLayer].colMode++;
				if(layers[selectedLayer].colMode > SPRITE_COLOR_HIDDEN)
					layers[selectedLayer].colMode = 0;
				changed = 1;
			}
			if(changed == 0)
				break;
			if(layers[selectedLayer].enabled != 0)
				fdSprViewerSetLayerColMode(selectedLayer,
										   layers[selectedLayer].colMode);
			goto next;
		}

		if(InputWork.repeat & KEY_A) {
			if(editMode == UI_SPRITE_VIEWER_EDIT_ANIMATION) {
				if(selectedLayer != 1) {
					if(layers[selectedLayer].enabled != 0) {
						fdSprViewerStartStaggeredAnimations(
							selectedLayer, layers[selectedLayer].anmIdx);
						if(selectedLayer == 0)
							fdSprViewerStartStaggeredAnimations(
								1, layers[1].anmIdx);
					}
					goto next;
				}
			}
			else if(selectedLayer != 0) {
				layers[selectedLayer].enabled ^= 1;
				fdSprViewerSetLayerPriority(
					selectedLayer, layers[selectedLayer].enabled != 0
									   ? layers[selectedLayer].pri
									   : UI_SPRITE_VIEWER_HIDDEN_PRIORITY);
				goto next;
			}
		}
		if(InputWork.repeat & KEY_SELECT) {
			syInitEvent();
			syDummyReturnZero(RESOURCE_SPRITE_METADATA,
							  Sprite_AnimationMetadata);
			goto restart;
		}
		if(InputWork.pressed & KEY_B) {
			volatile u16 *palette = (volatile u16 *)BG_PLTT;
			const u16 *colors = SpriteViewer_BackdropColors;
			backdrop++;
			backdrop &= 7;
			/* B steps the backdrop through its eight colours. */
			*palette = *(const u16 *)((u32)colors + (backdrop << 1));
		}
		sySynch(1);
	}
}

extern const u16 SpriteViewer_SkipStops[];

/* Steps a sprite id (0 to 715) by delta: a step of one wraps around the */
/* range, a larger step forward goes to the next stop above value and one back
 */
/* to the last stop below it (SpriteViewer_SkipStops, ended by (u16)-1, from */
/* which a step forward wraps to 0). A delta of 0 returns value unchanged. */
s32 fdSprViewerStepSprId(s32 value, s32 delta) {
	s32 step;

	if(delta != 0) {
		step = delta;
		if(delta < 0)
			step = -delta;
		if(step == 1) {
			value += delta;
			if(value < 0)
				value = UI_SPRITE_VIEWER_SPRITE_LIMIT - 1;
			if(value >= UI_SPRITE_VIEWER_SPRITE_LIMIT)
				value = 0;
		}
		else if(delta > 0) {
			u32 i;

			for(i = 0; i < UI_SPRITE_VIEWER_SKIP_STOPS; i++) {
				if(value < SpriteViewer_SkipStops[i]) {
					value = SpriteViewer_SkipStops[i];
					if(value == (u16)-1)
						value = 0;
					break;
				}
			}
		}
		else if(value == 0) {
			value = SpriteViewer_SkipStops[UI_SPRITE_VIEWER_SKIP_STOPS - 2];
		}
		else {
			s32 i;

			for(i = UI_SPRITE_VIEWER_SKIP_STOPS - 2; i >= 0; i--) {
				if(value > SpriteViewer_SkipStops[i]) {
					value = SpriteViewer_SkipStops[i];
					break;
				}
			}
		}
		return value;
	}
#ifdef UBFIX
	return value;
#else
	/* UB: no return statement for a delta of 0; UBFIX returns value unchanged.
	 */
#endif
}

/* Lays out the sprite viewer's ring around pixel (x, z): one position per */
/* facing of the first sprite's facing kind (6, 8, 10 or 4, or two opposite */
/* pairs for ANIM_FACING_TWO_PART), 56 pixels out (40 for a two-part */
/* sprite). B held turns the ring. Called by fdSprViewerRun when it starts */
/* and while Start moves the ring. */
void fdSprViewerLayOutRing(s32 centerX, s32 centerY, SPR_PLACEMENT *positions) {
	SPR_PLACEMENT *pair;
	s32 angle;
	s32 i;
	u16 turn;
	u8 kind;

	angle = 0;
	kind = ((SPR *)Ram_HeapSlots->sprs)->entries[0]->kind;
	if((InputWork.held & KEY_B) != 0)
		angle = (s16)(VBlankCount << 8);
	switch((u32)kind) {
	case ANIM_FACING_THREE_PICTURES:
		for(i = 0; i < 6; i++) {
			angle <<= ANGLE_BITS;
			positions->x = toFIXED(centerX);
			angle = (u32)angle >> ANGLE_BITS;
			positions->height = 0;
			positions->y = toFIXED(centerY);
			syVectorAddPolarOffset(toFIXED(56), (u16)angle, &positions->x);
			positions++;
			angle = (s16)(angle + DEGtoANG(60));
		}
		for(; i < UI_SPRITE_VIEWER_COPY_COUNT; i++)
			positions++;
		break;
	case ANIM_FACING_FIVE_PICTURES:
	case ANIM_FACING_EIGHT_PICTURES:
	case ANIM_FACING_EIGHT_NO_TABLE:
	case ANIM_FACING_EIGHT_PICTURES_MIRRORED:
		for(i = 0; i < 8; i++) {
			angle = (u32)angle << ANGLE_BITS;
			positions->x = toFIXED(centerX);
			angle = (u32)angle >> ANGLE_BITS;
			positions->height = 0;
			positions->y = toFIXED(centerY);
			syVectorAddPolarOffset(toFIXED(56), (u16)angle, &positions->x);
			positions++;
			angle = (s16)(angle + DEGtoANG(45));
		}
		for(; i < UI_SPRITE_VIEWER_COPY_COUNT; i++)
			positions++;
		break;
	case ANIM_FACING_FOUR_PICTURES:
	case ANIM_FACING_SIX_PICTURES:
		for(i = 0; i < UI_SPRITE_VIEWER_COPY_COUNT; i++) {
			angle <<= ANGLE_BITS;
			positions->x = toFIXED(centerX);
			angle = (u32)angle >> ANGLE_BITS;
			positions->height = 0;
			positions->y = toFIXED(centerY);
			syVectorAddPolarOffset(toFIXED(56), (u16)angle, &positions->x);
			positions++;
			angle = (s16)(angle + DEGtoANG(36));
		}
		break;
	case ANIM_FACING_TWO_PART:
		turn = angle;
		angle = (s16)(turn + DEGtoANG(90));
		i = 0;
		pair = positions;
		while(i < 2) {
			pair[i * 2].x = toFIXED(centerX);
			pair[i * 2].height = 0;
			pair[i * 2].y = toFIXED(centerY);
			angle <<= ANGLE_BITS;
			angle = (u32)angle >> ANGLE_BITS;
			syVectorAddPolarOffset(toFIXED(40), angle, &positions[0].x);
			pair[i * 2 + 1].x = toFIXED(centerX);
			{
				s32 *point = sySprPlacementValues(&positions[1]);
				pair[i * 2 + 1].height = 0;
				pair[i * 2 + 1].y = toFIXED(centerY);
				syVectorAddPolarOffset(toFIXED(40), (u16)angle, point);
			}
			angle = ((u32)angle + DEGtoANG(180)) << ANGLE_BITS;
			i++;
			positions += 2;
			angle >>= ANGLE_BITS;
		}
		break;
	default:
		angle = (s32)(((u32)angle << ANGLE_BITS) +
					  ((u32)DEGtoANG(45) << ANGLE_BITS)) >>
				ANGLE_BITS;
		for(i = 0; i < 4; i++) {
			angle = (u32)angle << ANGLE_BITS;
			angle = (u32)angle >> ANGLE_BITS;
			positions->x = toFIXED(centerX);
			positions->height = 0;
			positions->y = toFIXED(centerY);
			syVectorAddPolarOffset(toFIXED(56), (u16)angle, &positions->x);
			positions++;
			angle = (s16)(angle + DEGtoANG(90));
		}
		for(i = 4; i < UI_SPRITE_VIEWER_COPY_COUNT; i++)
			positions++;
		break;
	}
}

/* Gives sprite animation slot of each of the viewer's ten sprites the colour */
/* mode value. */
void fdSprViewerSetLayerColMode(s32 slot, s32 value) {
	SPR *sprites = Ram_HeapSlots->sprs;
	s32 count;

	for(count = 0; count < 10; count++) {
		sprites->entries[slot & 3]->colMode = value;
		sprites++;
	}
}

/* Gives sprite animation slot of each of the viewer's ten sprites the drawing
 */
/* order value. */
void fdSprViewerSetLayerPriority(s32 slot, s32 value) {
	SPR *sprites = Ram_HeapSlots->sprs;
	s32 count;

	for(count = 0; count < 10; count++) {
		sprites->entries[slot & 3]->pri = value;
		sprites++;
	}
}

/* Starts animation value on sprite animation slot of each of the viewer's ten
 */
/* sprites, each one a frame (16 timer units) behind the last, so the copies */
/* are staggered. */
s32 fdSprViewerStartStaggeredAnimations(s32 slot, s32 value) {
	s32 zero;
	s32 count;
	s32 offset;
	ANIMATION_METADATA *metadata;
	SPR_ANIMATION *entry;
	u8 *state;

	state = Ram_HeapSlots->sprs;
	count = 0;
	offset = (3 & slot) * sizeof(SPR_ANIMATION *) + offsetof(SPR, entries);
	zero = 0;
	do {
		entry = *(SPR_ANIMATION **)(state + offset);
		if(entry->scripts != 0) {
			metadata = fdResourceGetMetadataRecordStub(entry->anmId);
			if(value < metadata->anmCnt) {
				entry->kind = metadata->drawKind;
				entry->script = entry->scripts[value];
				entry->tmr = count * SPRITE_ANIMATION_NORMAL_STEP;
				entry->step = SPRITE_ANIMATION_NORMAL_STEP;
				entry->scriptOfs = zero;
				entry->pictureBase = zero;
				entry->picture = SPRITE_PICTURE_NONE;
			}
			((SPR *)state)->ofsY = metadata->adjustY;
			((SPR *)state)->rotation = zero;
		}
		count += 1;
		state += sizeof(SPR);
	} while(count <= 9);
#ifdef UBFIX
	return 0;
#else
	/* UB: an int function with no return statement. fdSprViewerRun's calls */
	/* treat r0 as set by it, which is how the ROM schedules their arguments. */
#endif
}

/* Sets sprite no on sprite animation slot of each of the viewer's ten sprites,
 */
/* and resets each sprite animation from its new metadata. */
void fdSprViewerSetLayerSpr(s32 slot, s32 no) {
	SPR *sprites = Ram_HeapSlots->sprs;
	s32 count;

	fdResourceGetMetadataRecordStub(no);
	for(count = 0; count < 10; count++) {
		SPR_ANIMATION *entry = sprites->entries[slot & 3];

		entry->anmId = no;
		fdSprAnimationReset(entry);
		sprites++;
	}
}

extern const SPR_SCALE SpriteViewer_Scale;

/* Each frame in the sprite viewer: places each copy of the sprite at its ring
 */
/* position, facing the way that position stands for. */
void fdSprViewerPlaceRing(void) {
	SPR *sprites = Ram_HeapSlots->sprs;
	u32 kind = sprites->entries[0]->kind;
	SPR_SCALE scale;
	u8 *table;
	u16 angle;
	u16 step;
	u16 odd = 0;
	u32 count;
	u32 i;

	scale = SpriteViewer_Scale;
	table = (u8 *)Ram_HeapSlots->battleState;

	switch(kind) {
	case ANIM_FACING_THREE_PICTURES:
		angle = 0;
		step = DEGtoANG(60);
		count = 6;
		break;
	case ANIM_FACING_FIVE_PICTURES:
	case ANIM_FACING_EIGHT_PICTURES:
	case ANIM_FACING_EIGHT_NO_TABLE:
	case ANIM_FACING_EIGHT_PICTURES_MIRRORED:
		angle = 0;
		step = DEGtoANG(45);
		count = 8;
		break;
	case ANIM_FACING_FOUR_PICTURES:
	case ANIM_FACING_SIX_PICTURES:
		angle = 0;
		step = DEGtoANG(36);
		count = UI_SPRITE_VIEWER_COPY_COUNT;
		break;
	case ANIM_FACING_TWO_PART:
		angle = 0;
		step = 0;
		odd = DEGtoANG(180);
		count = 4;
		break;
	default:
		angle = DEGtoANG(45);
		step = DEGtoANG(90);
		count = 4;
		break;
	}

	for(i = 0; i < count; i++) {
		fdRenderApplyProjectedPlacement(
			sprites, (s32 *)(table + i * sizeof(SPR_PLACEMENT)), (s32 *)&scale,
			angle);
		sprites++;
		angle += step;
		if(i & 1)
			angle += odd;
	}
}

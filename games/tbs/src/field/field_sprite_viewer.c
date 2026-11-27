/* @file */
/* The debug sprite viewer: one sprite shown once for each way it can face, */
/* in a ring, with up to four sprite animations laid over it and the keys to */
/* change each one's sprite, animation, priority and colour mode. */
/* Part of field.c. */

#include "field_internal.h"

enum {
	/* The copies of the sprite and their places on the ring; a facing kind */
	/* uses 4, 6, 8 or 10 of them (fdSprViewerLayOutRing). */
	SPRITE_VIEWER_COPY_COUNT = 10,
	SPRITE_VIEWER_SPRITE_LIMIT = 512,	 /* sprite ids 0 to 511 are offered */
	SPRITE_VIEWER_ANIMATION_LIMIT = 100, /* animations 0 to 99 */
	/* A sprite animation priority above 3, which fdSprComposePicture skips. */
	SPRITE_VIEWER_HIDDEN_PRIORITY = 8,
	/* The sprites with an image L and R step over while Start is held. */
	SPRITE_VIEWER_FAST_STEP = 10
};

/* An angle's bits: shifted up and back down, a value keeps only them. */
#define ANGLE_BITS 16

/* What left and right choose for L and R to change on the selected sprite */
/* animation. */
typedef enum SPR_VIEWER_EDIT_MODE {
	SPRITE_VIEWER_EDIT_ANIMATION,
	SPRITE_VIEWER_EDIT_SPRITE,
	SPRITE_VIEWER_EDIT_PRIORITY,
	SPRITE_VIEWER_EDIT_COLOR,
	SPRITE_VIEWER_EDIT_COUNT
} SPR_VIEWER_EDIT_MODE;

/* One sprite animation the sprite viewer edits. */
typedef struct SPR_VIEWER_LAYER {
	s16 sprId;	/* the animation id (the sprite number) it shows */
	s16 anmIdx; /* which of that sprite's animations plays */
	u8 enabled; /* shown; hidden by a priority above the drawn range */
	s8 pri;		/* drawing order, 0 to 3 */
	s8 colMode; /* SPR_COL_MODE; the viewer cycles through 0 to 15 */
} SPR_VIEWER_LAYER;

/* A debug sprite viewer, which never returns. It shows a sprite (with up to */
/* three more sprite animations) once for each of its facings, in a ring. */
void fdSprViewerRun(void) {
	SPR_VIEWER_LAYER layers[ANIM_LAYER_COUNT];
	ANIMATION_METADATA *spriteMetadata;
	SPR *sprites;
	volatile s32 fillWord;
	s32 spriteId;
	s32 x;
	s32 y;
	u32 editMode;
	s32 shadowsEnabled;
	SPR_PLACEMENT *ring;
	s32 selectedLayer;
	u32 spriteSteps;
	s32 changed;
	/* The edit loop reads the held and repeated keys through these pointers. */
	volatile u32 *heldKeys;
	volatile u32 *repeatedKeys;
	u32 i;

	/* The ring's center, in pixels. */
	x = 144;
	y = 96;
	editMode = SPRITE_VIEWER_EDIT_SPRITE;
	shadowsEnabled = 1;
	selectedLayer = 0;
	ring = syHeapSlotAllocEwram(HEAP_SLOT_BATTLE,
								SPRITE_VIEWER_COPY_COUNT * sizeof(*ring));
	/* A sprite animation in SPRITE_COLOR_FILL is painted in colour 3. */
	DecodeFillByte = 3;

	/* Only the ring's first word is cleared; fdSprViewerLayOutRing fills */
	/* it. */
	btDmaFill32Cell(fillWord, 0, ring, 4);
	btDmaFill32Cell(fillWord, 0, layers, sizeof(layers));

	/* All four sprite animations start on the first sprite with an image, */
	/* animation 1, priority 1; only sprite animation 0 is shown. */
	spriteId = fdSprViewerStepSprId(-1, 1);
	for(i = 0; i < ANIM_LAYER_COUNT; i++) {
		layers[i].anmIdx = 1;
		layers[i].pri = 1;
		layers[i].sprId = spriteId;
	}
	layers[0].enabled = 1;
	/* Any nonzero value turns the debug mode on. */
	DebugMode = 2;
	/* Nothing visible comes of this blend: BG1 is never turned on below, and */
	/* the viewer's sprites are all in normal mode (fdSprCreate); only a */
	/* semi-transparent sprite blends whatever BLDCNT's first target is (GBATEK,
	 */
	/* "Semi-Transparent OBJs"). */
	REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
	/* A green backdrop behind BG0 and the sprites, the only layers shown. */
	PLTT_BACKDROP = RGB(0, 15, 0);
	REG_DISPCNT = DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_OBJ_ON;
	syScrFadeFromBlack(1);

	/* Rebuilt from here whenever sprite animation 0's sprite changes or Select
	 * is */
	/* pressed: a fresh heap, task table and VRAM cache each time. */
restart:
	syHeapInit();
	syInitEvent();
	ring = syHeapSlotAllocEwram(HEAP_SLOT_BATTLE,
								SPRITE_VIEWER_COPY_COUNT * sizeof(*ring));
	syVramCacheReset();
	fdActorSystemInit(ACTOR_SYSTEM_MAP);
	/* With no map loaded, the map cell buffer holds sprite animation 0's */
	/* pictures; a two-part sprite (ANIM_FACING_TWO_PART) puts its second part
	 * in */
	/* the actor sprite buffer, as sprite picture cache 1. */
	fdSprPictureCacheLoad(0, (u32 *)Ram_MapGridCellBuffer, layers[0].sprId, 0);
	spriteMetadata = syResourceGetMetadataRecordStub(layers[0].sprId);
	if(spriteMetadata->drawKind == ANIM_FACING_TWO_PART) {
		fdSprPictureCacheLoad(1, (u32 *)Ram_EnemySpriteSlots,
							  layers[0].sprId + 1, 0);
	}

	/* Ten copies of the sprite; for a two-part sprite every other copy */
	/* shows its second part, number + 1, from sprite picture cache 1. */
	for(i = 0; i < SPRITE_VIEWER_COPY_COUNT; i++) {
		SPR *sprite;
		s32 part = 0;

		spriteMetadata = syResourceGetMetadataRecordStub(layers[0].sprId);
		if(spriteMetadata->drawKind == ANIM_FACING_TWO_PART && (i & 1) != 0) {
			part = 1;
		}
		sprite = fdSprCreate(SPRITE_PICTURE_TAG(part, layers[0].sprId + part));
		/* Sprite animations 1 to 3 are added to every copy whether shown or
		 * not; a */
		/* hidden sprite animation is kept out of the drawing by its priority
		 * below. */
		fdSprAddAnimation(sprite, layers[1].sprId);
		fdSprAddAnimation(sprite, layers[2].sprId);
		fdSprAddAnimation(sprite, layers[3].sprId);
		sprite->flg = shadowsEnabled;
	}

	/* Priority 8 hides a sprite animation: fdSprComposePicture draws only */
	/* priorities 0 to 3. */
	for(i = 0; i < ANIM_LAYER_COUNT; i++) {
		if(layers[i].enabled != 0) {
			fdSprViewerSetLayerPriority(i, layers[i].pri);
		}
		else {
			fdSprViewerSetLayerPriority(i, SPRITE_VIEWER_HIDDEN_PRIORITY);
		}
		fdSprViewerSetLayerColMode(i, layers[i].colMode);
		fdSprViewerStartStaggeredAnimations(i, layers[i].anmIdx);
	}
	/* The copies are drawn each frame by the task below, from the ring's */
	/* positions. */
	fdSprViewerLayOutRing(x, y, ring);
	sySetEvent(fdSprViewerPlaceRing, TASK_PASS_FRAME);

next:
	sySynch(1);
	heldKeys = &KeysHeld;
	repeatedKeys = &KeysRepeat;
	for(;;) {
		/* Start held: the direction pad moves the ring a pixel a frame; else up
		 */
		/* and down choose a sprite animation and left and right what L and R */
		/* change on it. */
		if((*heldKeys & KEY_START) != 0) {
			if((*heldKeys & KEY_LEFT) != 0) {
				x--;
			}
			if((*heldKeys & KEY_RIGHT) != 0) {
				x++;
			}
			if((*heldKeys & KEY_UP) != 0) {
				y--;
			}
			if((*heldKeys & KEY_DOWN) != 0) {
				y++;
			}
			fdSprViewerLayOutRing(x, y, ring);
		}
		else {
			/* DEAD: a read of KeysRepeat whose value goes unused; the ROM keeps
			 */
			/* the load. */
			(void)KeysRepeat;
			if((*repeatedKeys & KEY_UP) != 0) {
				selectedLayer = (selectedLayer - 1) & (ANIM_LAYER_COUNT - 1);
			}
			if((KeysRepeat & KEY_DOWN) != 0) {
				selectedLayer = (selectedLayer + 1) & (ANIM_LAYER_COUNT - 1);
			}
			if((KeysRepeat & KEY_LEFT) != 0) {
				editMode = (editMode - 1) & (SPRITE_VIEWER_EDIT_COUNT - 1);
			}
			if((KeysRepeat & KEY_RIGHT) != 0) {
				editMode++;
				editMode &= SPRITE_VIEWER_EDIT_COUNT - 1;
			}
		}

		/* A press of Start also toggles the shadows (SPRITE_FLAG_SHADOW) of the
		 */
		/* ten copies. */
		if((KeysPressed & KEY_START) != 0) {
			sprites = SpritesWork;
			shadowsEnabled ^= 1;
			for(i = 0; i < SPRITE_VIEWER_COPY_COUNT; i++) {
				sprites->flg = shadowsEnabled;
				sprites++;
			}
		}

		switch(editMode) {
		/* The animation: sprite animation 1 follows sprite animation 0's and is
		 * not */
		/* edited on its own; 0 to 99. */
		case SPRITE_VIEWER_EDIT_ANIMATION:
			if(selectedLayer == 1) {
				break;
			}
			if((KeysRepeat & KEY_L) != 0) {
				layers[selectedLayer].anmIdx--;
				if(layers[selectedLayer].anmIdx < 0) {
					layers[selectedLayer].anmIdx = 0;
				}
				if(selectedLayer == 0) {
					layers[1].anmIdx = layers[0].anmIdx;
				}
				goto next;
			}
			if((KeysRepeat & KEY_R) == 0) {
				break;
			}
			layers[selectedLayer].anmIdx++;
			if(layers[selectedLayer].anmIdx >= SPRITE_VIEWER_ANIMATION_LIMIT) {
				layers[selectedLayer].anmIdx =
					SPRITE_VIEWER_ANIMATION_LIMIT - 1;
			}
			if(selectedLayer == 0) {
				layers[1].anmIdx = layers[0].anmIdx;
			}
			goto next;

		/* The sprite number: one sprite with an image at a time, or ten */
		/* with Start held. */
		case SPRITE_VIEWER_EDIT_SPRITE:
			changed = 0;
			spriteSteps = 1;
			if((*heldKeys & KEY_START) != 0) {
				spriteSteps = SPRITE_VIEWER_FAST_STEP;
			}
			if((*repeatedKeys & KEY_L) != 0) {
				for(i = 0; i < spriteSteps; i++) {
					layers[selectedLayer].sprId =
						fdSprViewerStepSprId(layers[selectedLayer].sprId, -1);
				}
				changed = 1;
			}
			if((*repeatedKeys & KEY_R) != 0) {
				for(i = 0; i < spriteSteps; i++) {
					layers[selectedLayer].sprId =
						fdSprViewerStepSprId(layers[selectedLayer].sprId, 1);
				}
				changed = 1;
			}
			if(changed == 0) {
				break;
			}
			/* A new sprite on sprite animation 0 changes the ring and the
			 * copies: */
			/* start over. */
			if(selectedLayer == 0) {
				goto restart;
			}
			/* A hidden sprite animation only remembers the number until it is
			 * shown */
			/* again. */
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

		/* The sprite animation's draw priority, 0 to 3 and wrapping. */
		case SPRITE_VIEWER_EDIT_PRIORITY:
			changed = 0;
			if((*repeatedKeys & KEY_L) != 0) {
				layers[selectedLayer].pri--;
				if(layers[selectedLayer].pri < 0) {
					layers[selectedLayer].pri = 3;
				}
				changed = 1;
			}
			if((*repeatedKeys & KEY_R) != 0) {
				layers[selectedLayer].pri++;
				if(layers[selectedLayer].pri > 3) {
					layers[selectedLayer].pri = 0;
				}
				changed = 1;
			}
			if(changed == 0) {
				break;
			}
			if(layers[selectedLayer].enabled != 0) {
				fdSprViewerSetLayerPriority(selectedLayer,
											layers[selectedLayer].pri);
			}
			goto next;

		/* The draw parameter (SPR_COL_MODE), 0 to 15 and wrapping; */
		/* SPRITE_COLOR_HIDDEN is the value fdSprSetAnimationColModes */
		/* leaves alone. */
		case SPRITE_VIEWER_EDIT_COLOR:
			changed = 0;
			if((*repeatedKeys & KEY_L) != 0) {
				layers[selectedLayer].colMode--;
				if(layers[selectedLayer].colMode < 0) {
					layers[selectedLayer].colMode = SPRITE_COLOR_HIDDEN;
				}
				changed = 1;
			}
			if((*repeatedKeys & KEY_R) != 0) {
				layers[selectedLayer].colMode++;
				if(layers[selectedLayer].colMode > SPRITE_COLOR_HIDDEN) {
					layers[selectedLayer].colMode = 0;
				}
				changed = 1;
			}
			if(changed == 0) {
				break;
			}
			if(layers[selectedLayer].enabled != 0) {
				fdSprViewerSetLayerColMode(selectedLayer,
										   layers[selectedLayer].colMode);
			}
			goto next;

		default:
			break;
		}

		/* A, editing the animation: restart the selected sprite animation
		 * (sprite */
		/* animation 0 also restarts sprite animation 1, which follows it). */
		if((*repeatedKeys & KEY_A) != 0) {
			if(editMode == SPRITE_VIEWER_EDIT_ANIMATION) {
				if(selectedLayer != 1) {
					if(layers[selectedLayer].enabled != 0) {
						fdSprViewerStartStaggeredAnimations(
							selectedLayer, layers[selectedLayer].anmIdx);
						if(selectedLayer == 0) {
							fdSprViewerStartStaggeredAnimations(
								1, layers[1].anmIdx);
						}
					}
					goto next;
				}
			}
			else if(selectedLayer != 0) {
				layers[selectedLayer].enabled ^= 1;
				fdSprViewerSetLayerPriority(
					selectedLayer, layers[selectedLayer].enabled != 0
									   ? layers[selectedLayer].pri
									   : SPRITE_VIEWER_HIDDEN_PRIORITY);
				goto next;
			}
		}

		/* syDummyReturnZero ignores its arguments, so the resource set */
		/* chosen here changes nothing in this build. */
		if((*repeatedKeys & KEY_SELECT) != 0) {
			syInitEvent();
			if((*heldKeys & KEY_B) != 0) {
				syDummyReturnZero(RESOURCE_RESOURCE_STUBS, Stubs_Resource);
			}
			else {
				syDummyReturnZero(RESOURCE_RESOURCE_FAR_CALLS_ALTERNATE,
								  Stubs_Resource);
			}
			goto restart;
		}
		sySynch(1);
	}
}

/* Returns the next sprite id after spriteId, stepping by direction (1 or -1) */
/* and wrapping within 0-511, whose metadata has an image (a nonzero width). */
s32 fdSprViewerStepSprId(s32 spriteId, s32 direction) {
	for(;;) {
		spriteId += direction;
		/* Past either end, jump to just beyond the other, so the next step */
		/* lands on 511 or 0. */
		if(spriteId < 0) {
			spriteId = SPRITE_VIEWER_SPRITE_LIMIT;
			continue;
		}
		{
			s32 spriteLimit = SPRITE_VIEWER_SPRITE_LIMIT;
			if(spriteId >= spriteLimit) {
				spriteId = -1;
				continue;
			}
		}
		if(syResourceGetMetadataRecordStub(spriteId)->width) {
			return spriteId;
		}
	}
}

/* Lays out the sprite viewer's ring around (centerX, centerY), in pixels: one
 */
/* position per facing of the first sprite's facing kind (6, 8, 10 or 4, or */
/* two opposite pairs for ANIM_FACING_TWO_PART), 56 pixels out (40 for a */
/* two-part sprite). */
void fdSprViewerLayOutRing(s32 centerX, s32 centerY, SPR_PLACEMENT *positions) {
	SPR_PLACEMENT *pairedPositions;
	s32 angle;
	s32 facingIndex;
	u16 initialHeading;
	u8 facingKind;

	angle = 0;
	facingKind = SpritesWork->entries[0]->kind;
	/* With B held the ring starts at an angle that turns with the frame */
	/* counter, a full turn every 256 frames. */
	if((KeysHeld & KEY_B) != 0)
		angle = (s16)(VBlankCount << 8);
	/* The steps are a sixth, an eighth, a tenth and a quarter of a turn. */
	switch((u32)facingKind) {
	case ANIM_FACING_THREE_PICTURES:
		for(facingIndex = 0; facingIndex < 6; facingIndex++) {
			angle = (u32)angle << ANGLE_BITS;
			positions->x = toFIXED(centerX);
			angle = (u32)angle >> ANGLE_BITS;
			positions->height = 0;
			positions->y = toFIXED(centerY);
			syVectorAddPolarOffset(toFIXED(56), (u16)angle,
								   sySprPlacementValues(positions));
			positions++;
			angle = (s16)(angle + DEGtoANG(60));
		}
		for(; facingIndex < SPRITE_VIEWER_COPY_COUNT; facingIndex++)
			positions++;
		break;
	case ANIM_FACING_FIVE_PICTURES:
	case ANIM_FACING_EIGHT_PICTURES:
	case ANIM_FACING_EIGHT_NO_TABLE:
	case ANIM_FACING_EIGHT_PICTURES_MIRRORED:
		for(facingIndex = 0; facingIndex < 8; facingIndex++) {
			angle = (u32)angle << ANGLE_BITS;
			positions->x = toFIXED(centerX);
			angle = (u32)angle >> ANGLE_BITS;
			positions->height = 0;
			positions->y = toFIXED(centerY);
			syVectorAddPolarOffset(toFIXED(56), (u16)angle,
								   sySprPlacementValues(positions));
			positions++;
			angle = (s16)(angle + DEGtoANG(45));
		}
		for(; facingIndex < SPRITE_VIEWER_COPY_COUNT; facingIndex++)
			positions++;
		break;
	case ANIM_FACING_FOUR_PICTURES:
	case ANIM_FACING_SIX_PICTURES:
		for(facingIndex = 0; facingIndex < SPRITE_VIEWER_COPY_COUNT;
			facingIndex++) {
			angle = (u32)angle << ANGLE_BITS;
			positions->x = toFIXED(centerX);
			angle = (u32)angle >> ANGLE_BITS;
			positions->height = 0;
			positions->y = toFIXED(centerY);
			syVectorAddPolarOffset(toFIXED(56), (u16)angle,
								   sySprPlacementValues(positions));
			positions++;
			angle = (s16)(angle + DEGtoANG(36));
		}
		break;
	/* Two places 40 pixels out, a quarter turn round and then */
	/* opposite it, each holding both of the sprite's parts. */
	case ANIM_FACING_TWO_PART:
		initialHeading = angle;
		angle = (s16)(initialHeading + DEGtoANG(90));
		facingIndex = 0;
		pairedPositions = positions;
		while(facingIndex < 2) {
			pairedPositions[facingIndex * 2].x = toFIXED(centerX);
			pairedPositions[facingIndex * 2].height = 0;
			pairedPositions[facingIndex * 2].y = toFIXED(centerY);
			angle = (u32)angle << ANGLE_BITS;
			angle = (u32)angle >> ANGLE_BITS;
			syVectorAddPolarOffset(toFIXED(40), angle,
								   sySprPlacementValues(&positions[0]));
			pairedPositions[facingIndex * 2 + 1].x = toFIXED(centerX);
			{
				s32 *point = sySprPlacementValues(&positions[1]);
				pairedPositions[facingIndex * 2 + 1].height = 0;
				pairedPositions[facingIndex * 2 + 1].y = toFIXED(centerY);
				syVectorAddPolarOffset(toFIXED(40), (u16)angle, point);
			}
			angle = ((u32)angle + DEGtoANG(180)) << ANGLE_BITS;
			facingIndex++;
			positions += 2;
			angle >>= ANGLE_BITS;
		}
		break;
	/* Any other kind: four places, starting an eighth of a turn round. */
	default:
		angle = (s32)(((u32)angle << ANGLE_BITS) +
					  ((u32)DEGtoANG(45) << ANGLE_BITS)) >>
				ANGLE_BITS;
		for(facingIndex = 0; facingIndex < 4; facingIndex++) {
			angle = (u32)angle << ANGLE_BITS;
			angle = (u32)angle >> ANGLE_BITS;
			positions->x = toFIXED(centerX);
			positions->height = 0;
			positions->y = toFIXED(centerY);
			syVectorAddPolarOffset(toFIXED(56), (u16)angle,
								   sySprPlacementValues(positions));
			positions++;
			angle = (s16)(angle + DEGtoANG(90));
		}
		for(facingIndex = 4; facingIndex < SPRITE_VIEWER_COPY_COUNT;
			facingIndex++)
			positions++;
		break;
	}
}

/* Defines name(layerIndex, value), which sets `field` of sprite animation */
/* layerIndex (0 to 3) of each of the viewer's ten sprites to value. */
#define SPRITE_VIEWER_DEFINE_SET_LAYER(name, field)                            \
	void name(s32 layerIndex, s32 value) {                                     \
		SPR *spriteGroup = SpritesWork;                                        \
		s32 spriteIndex;                                                       \
                                                                               \
		layerIndex &= ANIM_LAYER_COUNT - 1;                                    \
		for(spriteIndex = 0; spriteIndex < SPRITE_VIEWER_COPY_COUNT;           \
			spriteIndex++) {                                                   \
			spriteGroup->entries[layerIndex]->field = value;                   \
			spriteGroup++;                                                     \
		}                                                                      \
	}

/* Sets the colour mode (SPR_COL_MODE, the sprite animation's draw */
/* parameter) or the priority of one sprite animation of the viewer's ten */
/* sprites. */
SPRITE_VIEWER_DEFINE_SET_LAYER(fdSprViewerSetLayerColMode, colMode)
SPRITE_VIEWER_DEFINE_SET_LAYER(fdSprViewerSetLayerPriority, pri)

/* Starts animationIndex on layerIndex of each of the viewer's ten sprites, */
/* each one a frame (16 timer units) behind the last, so the copies are */
/* staggered. Also sets each copy's offsetY and clears its rotation. */
void fdSprViewerStartStaggeredAnimations(s32 layerIndex, s32 animationIndex) {
	s32 spriteIndex;
	SPR *spriteGroup;

	spriteGroup = SpritesWork;
	spriteIndex = 0;
	do {
		SPR_ANIMATION *entry =
			spriteGroup->entries[layerIndex & (ANIM_LAYER_COUNT - 1)];
		ANIMATION_METADATA *metadata;
		s32 start = 0;

		if(entry->scripts != 0) {
			metadata = syResourceGetMetadataRecordStub(entry->anmId);
			if(animationIndex < metadata->anmCnt) {
				/* Copy n starts n frames (16 timer units each) late, at normal
				 */
				/* speed, with no frame drawn yet. */
				entry->kind = metadata->drawKind;
				entry->script = entry->scripts[animationIndex];
				entry->tmr = spriteIndex * SPRITE_ANIMATION_NORMAL_STEP;
				entry->step = SPRITE_ANIMATION_NORMAL_STEP;
				entry->scriptOfs = start;
				entry->pictureBase = start;
				entry->picture = SPRITE_PICTURE_NONE;
			}
			spriteGroup->ofsY = metadata->adjustY;
			spriteGroup->rotation = start;
		}
		spriteIndex++;
		spriteGroup++;
	} while(spriteIndex < SPRITE_VIEWER_COPY_COUNT);
}

/* Sets spriteId on layerIndex of each of the viewer's ten sprites. Resets each
 */
/* sprite animation from its new metadata. */
void fdSprViewerSetLayerSpr(s32 layerIndex, s32 spriteId) {
	SPR *spriteGroup = SpritesWork;
	s32 spriteIndex;

	/* DEAD: the sprite's metadata looked up and not used; the ROM keeps the
	 * call. */
	syResourceGetMetadataRecordStub(spriteId);
	layerIndex &= ANIM_LAYER_COUNT - 1;
	for(spriteIndex = 0; spriteIndex < SPRITE_VIEWER_COPY_COUNT;
		spriteIndex++) {
		SPR_ANIMATION *entry = spriteGroup->entries[layerIndex];

		entry->anmId = spriteId;
		fdSprAnimationReset(entry);
		spriteGroup++;
	}
}

/* Each frame in the sprite viewer: places each copy of the sprite at its ring
 */
/* position, facing the way that position stands for. */
void fdSprViewerPlaceRing(void) {
	SPR *sprite = SpritesWork;
	u32 facingKind = sprite->entries[0]->kind;
	SPR_SCALE scale;
	SPR_PLACEMENT *facingPositions;
	u16 angle;
	u16 step;
	u16 pairedFacingStep = 0;
	u32 count;
	u32 spriteIndex;

	scale = SpriteViewer_Scale;
	facingPositions = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE);

	switch(facingKind) {
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
		count = SPRITE_VIEWER_COPY_COUNT;
		break;
	/* The two parts at each place face the same way; the second place faces */
	/* half a turn from the first. */
	case ANIM_FACING_TWO_PART:
		angle = 0;
		step = 0;
		pairedFacingStep = DEGtoANG(180);
		count = 4;
		break;
	/* The eighth-turn start matches fdSprViewerLayOutRing's default ring. */
	default:
		angle = DEGtoANG(45);
		step = DEGtoANG(90);
		count = 4;
		break;
	}

	/* Copy n faces n steps round, the way its ring position lies. */
	for(spriteIndex = 0; spriteIndex < count; spriteIndex++) {
		fdRenderApplyProjectedPlacement(sprite, &facingPositions[spriteIndex],
										&scale, angle);
		sprite++;
		angle += step;
		if(spriteIndex & 1)
			angle += pairedFacingStep;
	}
}

#undef SPRITE_VIEWER_DEFINE_SET_LAYER
#undef ANGLE_BITS

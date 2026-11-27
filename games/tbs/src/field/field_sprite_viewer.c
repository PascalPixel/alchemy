/* The debug sprite viewer: one sprite shown once for each way it can face, in a
   ring, with up to four sprite animations laid over it and the keys to change
   each one's sprite, animation, priority and colour mode. Part of field.c. */

#include "field_internal.h"

enum {
	/* The copies of the sprite and their places on the ring; a facing kind uses
	   4, 6, 8 or 10 of them (fdSprViewerLayOutRing). */
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

/* What left and right choose for L and R to change on the selected sprite
   animation. */
typedef enum SPR_VIEWER_EDIT_MODE {
	SPRITE_VIEWER_EDIT_ANIMATION,
	SPRITE_VIEWER_EDIT_SPRITE,
	SPRITE_VIEWER_EDIT_PRIORITY,
	SPRITE_VIEWER_EDIT_COLOR,
	SPRITE_VIEWER_EDIT_COUNT
} SPR_VIEWER_EDIT_MODE;

/* One sprite animation the sprite viewer edits. */
typedef struct SPR_VIEWER_LAYER {
	Sint16 sprId;	/* the animation id (the sprite number) it shows */
	Sint16 anmIdx;	/* which of that sprite's animations plays */
	Uint8  enabled; /* shown; hidden by a priority above the drawn range */
	Sint8  pri;		/* drawing order, 0 to 3 */
	Sint8  colMode; /* SPR_COL_MODE; the viewer cycles through 0 to 15 */
} SPR_VIEWER_LAYER;

/* A debug sprite viewer, which never returns. It shows a sprite (with up to
   three more sprite animations) once for each of its facings, in a ring. */
void fdSprViewerRun(void)
{
	SPR_VIEWER_LAYER	layers[ANIM_LAYER_COUNT];
	ANIMATION_METADATA *sprMetadata;
	SPR				   *sprs;
	volatile Sint32		fillWord;
	Sint32				sprId;
	Sint32				x;
	Sint32				y;
	Uint32				editMode;
	Sint32				shadowsEnabled;
	SPR_PLACEMENT	   *ring;
	Sint32				selectedLayer;
	Uint32				sprSteps;
	Sint32				changed;
	/* The edit loop reads the held and repeated keys through these pointers. */
	volatile Uint32 *heldKeys;
	volatile Uint32 *repeatedKeys;
	Uint32			 i;

	/* The ring's center, in pixels. */
	x = 144;
	y = 96;
	editMode = SPRITE_VIEWER_EDIT_SPRITE;
	shadowsEnabled = 1;
	selectedLayer = 0;
	ring = syHeapSlotAllocEWRAM(
		HEAP_SLOT_BATTLE, SPRITE_VIEWER_COPY_COUNT * sizeof(*ring));
	/* A sprite animation in SPRITE_COLOR_FILL is painted in colour 3. */
	DecodeFillByte = 3;

	/* Only the ring's first word is cleared; fdSprViewerLayOutRing fills it. */
	DMA_FILL32_CELL(fillWord, 0, ring, 4);
	DMA_FILL32_CELL(fillWord, 0, layers, sizeof(layers));

	/* All four sprite animations start on the first sprite with an image,
	   animation 1, priority 1; only sprite animation 0 is shown. */
	sprId = fdSprViewerStepSprId(-1, 1);
	for(i = 0; i < ANIM_LAYER_COUNT; i++) {
		layers[i].anmIdx = 1;
		layers[i].pri = 1;
		layers[i].sprId = sprId;
	}
	layers[0].enabled = 1;
	/* Any nonzero value turns the debug mode on. */
	DebugMode = 2;
	/* Nothing visible comes of this blend: BG1 is never turned on below, and
	   the viewer's sprites are all in normal mode (fdSprCreate); only a
	   semi-transparent sprite blends whatever BLDCNT's first target is (GBATEK,
	   "Semi-Transparent OBJs"). */
	REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
	/* A green backdrop behind BG0 and the sprites, the only layers shown. */
	PLTT_BACKDROP = RGB(0, 15, 0);
	REG_DISPCNT = DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_OBJ_ON;
	syScreenFadeFromBlack(1);

	/* Rebuilt from here whenever sprite animation 0's sprite changes or Select
	   is pressed: a fresh heap, event table and VRAM cache each time. */
restart:
	syHeapInit();
	syInitEvent();
	ring = syHeapSlotAllocEWRAM(
		HEAP_SLOT_BATTLE, SPRITE_VIEWER_COPY_COUNT * sizeof(*ring));
	syVRAMCacheReset();
	fdActorSystemInit(ACTOR_SYSTEM_MAP);
	/* With no map loaded, the map metatile buffer holds sprite animation 0's
	   pictures; a two-part sprite (ANIM_FACING_TWO_PART) puts its second part
	   in the actor sprite buffer, as sprite picture cache 1. */
	fdSprPictureCacheLoad(
		0, (Uint32 *)RamMapGridMetatileBuffer, layers[0].sprId, 0);
	sprMetadata = syResourceGetMetadataRecordStub(layers[0].sprId);
	if(sprMetadata->drawKind == ANIM_FACING_TWO_PART) {
		fdSprPictureCacheLoad(
			1, (Uint32 *)RamEnemySpriteSlots, layers[0].sprId + 1, 0);
	}

	/* Ten copies of the sprite; for a two-part sprite every other copy shows
	   its second part, number + 1, from sprite picture cache 1. */
	for(i = 0; i < SPRITE_VIEWER_COPY_COUNT; i++) {
		SPR	  *spr;
		Sint32 part = 0;

		sprMetadata = syResourceGetMetadataRecordStub(layers[0].sprId);
		if(sprMetadata->drawKind == ANIM_FACING_TWO_PART && (i & 1) != 0) {
			part = 1;
		}
		spr = fdSprCreate(SPRITE_PICTURE_TAG(part, layers[0].sprId + part));
		/* Sprite animations 1 to 3 are added to every copy whether shown or
		   not; a hidden sprite animation is kept out of the drawing by its
		   priority below. */
		fdSprAddAnimation(spr, layers[1].sprId);
		fdSprAddAnimation(spr, layers[2].sprId);
		fdSprAddAnimation(spr, layers[3].sprId);
		spr->flg = shadowsEnabled;
	}

	/* Priority 8 hides a sprite animation: fdSprComposePicture draws only
	   priorities 0 to 3. */
	for(i = 0; i < ANIM_LAYER_COUNT; i++) {
		if(layers[i].enabled != 0) {
			fdSprViewerSetLayerPriority(i, layers[i].pri);
		} else {
			fdSprViewerSetLayerPriority(i, SPRITE_VIEWER_HIDDEN_PRIORITY);
		}
		fdSprViewerSetLayerColMode(i, layers[i].colMode);
		fdSprViewerStartStaggeredAnimations(i, layers[i].anmIdx);
	}
	/* The copies are drawn each tick by the event below, from the ring's
	   positions. */
	fdSprViewerLayOutRing(x, y, ring);
	sySetEvent(fdSprViewerPlaceRing, EVENT_PASS_TICK);

next:
	sySynch(1);
	heldKeys = &PerData;
	repeatedKeys = &PerRepeat;
	for(;;) {
		/* Start held: the direction pad moves the ring a pixel a tick; else up
		   and down choose a sprite animation and left and right what L and R
		   change on it. */
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
		} else {
			/* DEAD: a read of PerRepeat whose value goes unused; the ROM keeps
			   the load. */
			(void)PerRepeat;
			if((*repeatedKeys & KEY_UP) != 0) {
				selectedLayer = (selectedLayer - 1) & (ANIM_LAYER_COUNT - 1);
			}
			if((PerRepeat & KEY_DOWN) != 0) {
				selectedLayer = (selectedLayer + 1) & (ANIM_LAYER_COUNT - 1);
			}
			if((PerRepeat & KEY_LEFT) != 0) {
				editMode = (editMode - 1) & (SPRITE_VIEWER_EDIT_COUNT - 1);
			}
			if((PerRepeat & KEY_RIGHT) != 0) {
				editMode++;
				editMode &= SPRITE_VIEWER_EDIT_COUNT - 1;
			}
		}

		/* A press of Start also toggles the shadows (SPRITE_FLAG_SHADOW) of the
		   ten copies. */
		if((PerPush & KEY_START) != 0) {
			sprs = SpritesWork;
			shadowsEnabled ^= 1;
			for(i = 0; i < SPRITE_VIEWER_COPY_COUNT; i++) {
				sprs->flg = shadowsEnabled;
				sprs++;
			}
		}

		switch(editMode) {
		/* The animation: sprite animation 1 follows sprite animation 0's and is
		   not edited on its own; 0 to 99. */
		case SPRITE_VIEWER_EDIT_ANIMATION:
			if(selectedLayer == 1) {
				break;
			}
			if((PerRepeat & KEY_L) != 0) {
				layers[selectedLayer].anmIdx--;
				if(layers[selectedLayer].anmIdx < 0) {
					layers[selectedLayer].anmIdx = 0;
				}
				if(selectedLayer == 0) {
					layers[1].anmIdx = layers[0].anmIdx;
				}
				goto next;
			}
			if((PerRepeat & KEY_R) == 0) {
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

		/* The sprite number: one sprite with an image at a time, or ten with
		   Start held. */
		case SPRITE_VIEWER_EDIT_SPRITE:
			changed = 0;
			sprSteps = 1;
			if((*heldKeys & KEY_START) != 0) {
				sprSteps = SPRITE_VIEWER_FAST_STEP;
			}
			if((*repeatedKeys & KEY_L) != 0) {
				for(i = 0; i < sprSteps; i++) {
					layers[selectedLayer].sprId =
						fdSprViewerStepSprId(layers[selectedLayer].sprId, -1);
				}
				changed = 1;
			}
			if((*repeatedKeys & KEY_R) != 0) {
				for(i = 0; i < sprSteps; i++) {
					layers[selectedLayer].sprId =
						fdSprViewerStepSprId(layers[selectedLayer].sprId, 1);
				}
				changed = 1;
			}
			if(changed == 0) {
				break;
			}
			/* A new sprite on sprite animation 0 changes the ring and the
			   copies:
			   start over. */
			if(selectedLayer == 0) {
				goto restart;
			}
			/* A hidden sprite animation only remembers the number until it is
			   shown again. */
			if(layers[selectedLayer].enabled != 0) {
				fdSprViewerSetLayerSpr(
					selectedLayer, layers[selectedLayer].sprId);
				fdSprViewerSetLayerPriority(
					selectedLayer, layers[selectedLayer].pri);
				fdSprViewerSetLayerColMode(
					selectedLayer, layers[selectedLayer].colMode);
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
				fdSprViewerSetLayerPriority(
					selectedLayer, layers[selectedLayer].pri);
			}
			goto next;

		/* The draw parameter (SPR_COL_MODE), 0 to 15 and wrapping;
		   SPRITE_COLOR_HIDDEN is the value fdSprSetAnimationColModes leaves
		   alone. */
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
				fdSprViewerSetLayerColMode(
					selectedLayer, layers[selectedLayer].colMode);
			}
			goto next;

		default:
			break;
		}

		/* A, editing the animation: restart the selected sprite animation
		   (sprite animation 0 also restarts sprite animation 1, which follows
		   it). */
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
			} else if(selectedLayer != 0) {
				layers[selectedLayer].enabled ^= 1;
				fdSprViewerSetLayerPriority(selectedLayer,
					layers[selectedLayer].enabled != 0
						? layers[selectedLayer].pri
						: SPRITE_VIEWER_HIDDEN_PRIORITY);
				goto next;
			}
		}

		/* syDummyReturnZero ignores its arguments, so the resource set chosen
		   here changes nothing in this build. */
		if((*repeatedKeys & KEY_SELECT) != 0) {
			syInitEvent();
			if((*heldKeys & KEY_B) != 0) {
				syDummyReturnZero(RESOURCE_RESOURCE_STUBS, StubsResource);
			} else {
				syDummyReturnZero(
					RESOURCE_RESOURCE_FAR_CALLS_ALTERNATE, StubsResource);
			}
			goto restart;
		}
		sySynch(1);
	}
}

/* Returns the next sprite id after sprId, stepping by direction (1 or -1)
   and wrapping within 0-511, whose metadata has an image (a nonzero width). */
Sint32 fdSprViewerStepSprId(Sint32 sprId, Sint32 direction)
{
	for(;;) {
		sprId += direction;
		/* Past either end, jump to just beyond the other, so the next step
		   lands on 511 or 0. */
		if(sprId < 0) {
			sprId = SPRITE_VIEWER_SPRITE_LIMIT;
			continue;
		}
		{
			Sint32 sprLimit = SPRITE_VIEWER_SPRITE_LIMIT;
			if(sprId >= sprLimit) {
				sprId = -1;
				continue;
			}
		}
		if(syResourceGetMetadataRecordStub(sprId)->width) {
			return sprId;
		}
	}
}

/* Lays out the sprite viewer's ring around (centerX, centerY), in pixels: one
   position per facing of the first sprite's facing kind (6, 8, 10 or 4, or two
   opposite pairs for ANIM_FACING_TWO_PART), 56 pixels out (40 for a two-part
   sprite). */
void fdSprViewerLayOutRing(
	Sint32 centerX, Sint32 centerY, SPR_PLACEMENT *positions)
{
	SPR_PLACEMENT *pairedPositions;
	Sint32		   ang;
	Sint32		   facingIdx;
	Uint16		   initHeading;
	Uint8		   facingKind;

	ang = 0;
	facingKind = SpritesWork->entries[0]->kind;
	/* With B held the ring starts at an angle that turns with the tick
	   counter, a full turn every 256 ticks. */
	if((PerData & KEY_B) != 0)
		ang = (Sint16)(FrameCount << 8);
	/* The steps are a sixth, an eighth, a tenth and a quarter of a turn. */
	switch((Uint32)facingKind) {
	case ANIM_FACING_THREE_PICTURES:
		for(facingIdx = 0; facingIdx < 6; facingIdx++) {
			ang = (Uint32)ang << ANGLE_BITS;
			positions->x = toFIXED(centerX);
			ang = (Uint32)ang >> ANGLE_BITS;
			positions->height = 0;
			positions->y = toFIXED(centerY);
			syVectorAddPolarOffset(
				toFIXED(56), (Uint16)ang, sySprPlacementValues(positions));
			positions++;
			ang = (Sint16)(ang + DEGtoANG(60));
		}
		for(; facingIdx < SPRITE_VIEWER_COPY_COUNT; facingIdx++)
			positions++;
		break;
	case ANIM_FACING_FIVE_PICTURES:
	case ANIM_FACING_EIGHT_PICTURES:
	case ANIM_FACING_EIGHT_NO_TABLE:
	case ANIM_FACING_EIGHT_PICTURES_MIRRORED:
		for(facingIdx = 0; facingIdx < 8; facingIdx++) {
			ang = (Uint32)ang << ANGLE_BITS;
			positions->x = toFIXED(centerX);
			ang = (Uint32)ang >> ANGLE_BITS;
			positions->height = 0;
			positions->y = toFIXED(centerY);
			syVectorAddPolarOffset(
				toFIXED(56), (Uint16)ang, sySprPlacementValues(positions));
			positions++;
			ang = (Sint16)(ang + DEGtoANG(45));
		}
		for(; facingIdx < SPRITE_VIEWER_COPY_COUNT; facingIdx++)
			positions++;
		break;
	case ANIM_FACING_FOUR_PICTURES:
	case ANIM_FACING_SIX_PICTURES:
		for(facingIdx = 0; facingIdx < SPRITE_VIEWER_COPY_COUNT; facingIdx++) {
			ang = (Uint32)ang << ANGLE_BITS;
			positions->x = toFIXED(centerX);
			ang = (Uint32)ang >> ANGLE_BITS;
			positions->height = 0;
			positions->y = toFIXED(centerY);
			syVectorAddPolarOffset(
				toFIXED(56), (Uint16)ang, sySprPlacementValues(positions));
			positions++;
			ang = (Sint16)(ang + DEGtoANG(36));
		}
		break;
	/* Two places 40 pixels out, a quarter turn round and then opposite it, each
	   holding both of the sprite's parts. */
	case ANIM_FACING_TWO_PART:
		initHeading = ang;
		ang = (Sint16)(initHeading + DEGtoANG(90));
		facingIdx = 0;
		pairedPositions = positions;
		while(facingIdx < 2) {
			pairedPositions[facingIdx * 2].x = toFIXED(centerX);
			pairedPositions[facingIdx * 2].height = 0;
			pairedPositions[facingIdx * 2].y = toFIXED(centerY);
			ang = (Uint32)ang << ANGLE_BITS;
			ang = (Uint32)ang >> ANGLE_BITS;
			syVectorAddPolarOffset(
				toFIXED(40), ang, sySprPlacementValues(&positions[0]));
			pairedPositions[facingIdx * 2 + 1].x = toFIXED(centerX);
			{
				Sint32 *point = sySprPlacementValues(&positions[1]);
				pairedPositions[facingIdx * 2 + 1].height = 0;
				pairedPositions[facingIdx * 2 + 1].y = toFIXED(centerY);
				syVectorAddPolarOffset(toFIXED(40), (Uint16)ang, point);
			}
			ang = ((Uint32)ang + DEGtoANG(180)) << ANGLE_BITS;
			facingIdx++;
			positions += 2;
			ang >>= ANGLE_BITS;
		}
		break;
	/* Any other kind: four places, starting an eighth of a turn round. */
	default:
		ang = (Sint32)(((Uint32)ang << ANGLE_BITS) +
				  ((Uint32)DEGtoANG(45) << ANGLE_BITS)) >>
			ANGLE_BITS;
		for(facingIdx = 0; facingIdx < 4; facingIdx++) {
			ang = (Uint32)ang << ANGLE_BITS;
			ang = (Uint32)ang >> ANGLE_BITS;
			positions->x = toFIXED(centerX);
			positions->height = 0;
			positions->y = toFIXED(centerY);
			syVectorAddPolarOffset(
				toFIXED(56), (Uint16)ang, sySprPlacementValues(positions));
			positions++;
			ang = (Sint16)(ang + DEGtoANG(90));
		}
		for(facingIdx = 4; facingIdx < SPRITE_VIEWER_COPY_COUNT; facingIdx++)
			positions++;
		break;
	}
}

/* Defines name(layerIdx, value), which sets `field` of sprite animation
   layerIdx (0 to 3) of each of the viewer's ten sprites to value. */
#define SPRITE_VIEWER_DEFINE_SET_LAYER(name, field) \
	void name(Sint32 layerIdx, Sint32 value) \
	{ \
		SPR	  *spriteGroup = SpritesWork; \
		Sint32 sprIdx; \
\
		layerIdx &= ANIM_LAYER_COUNT - 1; \
		for(sprIdx = 0; sprIdx < SPRITE_VIEWER_COPY_COUNT; sprIdx++) { \
			spriteGroup->entries[layerIdx]->field = value; \
			spriteGroup++; \
		} \
	}

/* Sets the colour mode (SPR_COL_MODE, the sprite animation's draw parameter) or
   the priority of one sprite animation of the viewer's ten sprites. */
SPRITE_VIEWER_DEFINE_SET_LAYER(fdSprViewerSetLayerColMode, colMode)
SPRITE_VIEWER_DEFINE_SET_LAYER(fdSprViewerSetLayerPriority, pri)

/* Starts anmIdx on layerIdx of each of the viewer's ten sprites, each
   one a tick (16 timer units) behind the last, so the copies are staggered.
   Also sets each copy's offsetY and clears its rotation. */
void fdSprViewerStartStaggeredAnimations(Sint32 layerIdx, Sint32 anmIdx)
{
	Sint32 sprIdx;
	SPR	  *sprGroup;

	sprGroup = SpritesWork;
	sprIdx = 0;
	do {
		SPR_ANIMATION *entry =
			sprGroup->entries[layerIdx & (ANIM_LAYER_COUNT - 1)];
		ANIMATION_METADATA *metadata;
		Sint32				start = 0;

		if(entry->scripts != 0) {
			metadata = syResourceGetMetadataRecordStub(entry->anmId);
			if(anmIdx < metadata->anmCnt) {
				/* Copy n starts n ticks (16 timer units each) late, at normal
				   speed, with no tick drawn yet. */
				entry->kind = metadata->drawKind;
				entry->script = entry->scripts[anmIdx];
				entry->tmr = sprIdx * SPRITE_ANIMATION_NORMAL_STEP;
				entry->step = SPRITE_ANIMATION_NORMAL_STEP;
				entry->scriptOfs = start;
				entry->pictureBase = start;
				entry->picture = SPRITE_PICTURE_NONE;
			}
			sprGroup->ofsY = metadata->adjustY;
			sprGroup->rotation = start;
		}
		sprIdx++;
		sprGroup++;
	} while(sprIdx < SPRITE_VIEWER_COPY_COUNT);
}

/* Sets sprId on layerIdx of each of the viewer's ten sprites. Resets each
   sprite animation from its new metadata. */
void fdSprViewerSetLayerSpr(Sint32 layerIdx, Sint32 sprId)
{
	SPR	  *spriteGroup = SpritesWork;
	Sint32 sprIdx;

	/* DEAD: the sprite's metadata looked up and not used; the ROM keeps the
	   call. */
	syResourceGetMetadataRecordStub(sprId);
	layerIdx &= ANIM_LAYER_COUNT - 1;
	for(sprIdx = 0; sprIdx < SPRITE_VIEWER_COPY_COUNT; sprIdx++) {
		SPR_ANIMATION *entry = spriteGroup->entries[layerIdx];

		entry->anmId = sprId;
		fdSprAnimationReset(entry);
		spriteGroup++;
	}
}

/* Each tick in the sprite viewer: places each copy of the sprite at its ring
   position, facing the way that position stands for. */
void fdSprViewerPlaceRing(void)
{
	SPR			  *sprite = SpritesWork;
	Uint32		   facingKind = sprite->entries[0]->kind;
	SPR_SCALE	   scale;
	SPR_PLACEMENT *facingPositions;
	Uint16		   ang;
	Uint16		   step;
	Uint16		   pairedFacingStep = 0;
	Uint32		   cnt;
	Uint32		   sprIdx;

	scale = SpriteViewerScale;
	facingPositions = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE);

	switch(facingKind) {
	case ANIM_FACING_THREE_PICTURES:
		ang = 0;
		step = DEGtoANG(60);
		cnt = 6;
		break;
	case ANIM_FACING_FIVE_PICTURES:
	case ANIM_FACING_EIGHT_PICTURES:
	case ANIM_FACING_EIGHT_NO_TABLE:
	case ANIM_FACING_EIGHT_PICTURES_MIRRORED:
		ang = 0;
		step = DEGtoANG(45);
		cnt = 8;
		break;
	case ANIM_FACING_FOUR_PICTURES:
	case ANIM_FACING_SIX_PICTURES:
		ang = 0;
		step = DEGtoANG(36);
		cnt = SPRITE_VIEWER_COPY_COUNT;
		break;
	/* The two parts at each place face the same way; the second place faces
	   half a turn from the first. */
	case ANIM_FACING_TWO_PART:
		ang = 0;
		step = 0;
		pairedFacingStep = DEGtoANG(180);
		cnt = 4;
		break;
	/* The eighth-turn start matches fdSprViewerLayOutRing's default ring. */
	default:
		ang = DEGtoANG(45);
		step = DEGtoANG(90);
		cnt = 4;
		break;
	}

	/* Copy n faces n steps round, the way its ring position lies. */
	for(sprIdx = 0; sprIdx < cnt; sprIdx++) {
		fdRenderApplyProjectedPlacement(
			sprite, &facingPositions[sprIdx], &scale, ang);
		sprite++;
		ang += step;
		if(sprIdx & 1)
			ang += pairedFacingStep;
	}
}

#undef SPRITE_VIEWER_DEFINE_SET_LAYER
#undef ANGLE_BITS

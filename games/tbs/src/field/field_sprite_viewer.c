// The debug sprite viewer: one sprite shown once for each way it can face, in a ring, with up to
// four sprite animations laid over it and the keys to change each one's sprite, animation, priority
// and colour mode. Part of field.c.

#include "field_internal.h"

enum
{
	// The copies of the sprite and their places on the ring; a facing kind uses 4, 6, 8 or 10 of
	// them (fdSpriteViewerLayOutRing).
	SPRITE_VIEWER_COPY_COUNT   = 10,
	SPRITE_VIEWER_SPRITE_LIMIT = 512, // sprite ids 0 to 511 are offered
	SPRITE_VIEWER_ANIMATION_LIMIT =
		100,                          // animations 0 to 99
	         // A sprite animation priority above 3, which fdSpriteComposePicture skips.
	SPRITE_VIEWER_HIDDEN_PRIORITY = 8,
	// The sprites with an image L and R step over while Start is held.
	SPRITE_VIEWER_FAST_STEP       = 10
};

// An angle's bits: shifted up and back down, a value keeps only them.
#define ANGLE_BITS 16

// What left and right choose for L and R to change on the selected sprite animation.
typedef enum SPR_VIEWER_EDIT_MODE
{
	SPRITE_VIEWER_EDIT_ANIMATION,
	SPRITE_VIEWER_EDIT_SPRITE,
	SPRITE_VIEWER_EDIT_PRIORITY,
	SPRITE_VIEWER_EDIT_COLOR,
	SPRITE_VIEWER_EDIT_COUNT
} SPR_VIEWER_EDIT_MODE;

// One sprite animation the sprite viewer edits.
typedef struct SPR_VIEWER_LAYER
{
	Sint16 spriteId;       // the animation id (the sprite number) it shows
	Sint16 animationIndex; // which of that sprite's animations plays
	Uint8  enabled;        // shown; hidden by a priority above the drawn range
	Sint8  priority;       // drawing order, 0 to 3
	Sint8  colorMode;      // SPR_COL_MODE; the viewer cycles through 0 to 15
} SPR_VIEWER_LAYER;

// A debug sprite viewer, which never returns. It shows a sprite (with up to three more sprite
// animations) once for each of its facings, in a ring.
void fdSpriteViewerRun(void)
{
	SPR_VIEWER_LAYER    layers[ANIM_LAYER_COUNT];
	ANIMATION_METADATA *spriteMetadata;
	SPR                *sprites;
	volatile Sint32     fillWord;
	Sint32              spriteId;
	Sint32              x;
	Sint32              y;
	Uint32              editMode;
	Sint32              shadowsEnabled;
	SPR_PLACEMENT      *ring;
	Sint32              selectedLayer;
	Uint32              spriteSteps;
	Sint32              changed;
	// The edit loop reads the held and repeated keys through these pointers.
	volatile Uint32    *heldKeys;
	volatile Uint32    *repeatedKeys;
	Uint32              i;

	// The ring's center, in pixels.
	x              = 144;
	y              = 96;
	editMode       = SPRITE_VIEWER_EDIT_SPRITE;
	shadowsEnabled = 1;
	selectedLayer  = 0;
	ring = syHeapSlotAllocateEWRAM(HEAP_SLOT_BATTLE, SPRITE_VIEWER_COPY_COUNT * sizeof(*ring));
	// A sprite animation in SPRITE_COLOR_FILL is painted in colour 3.
	DecodeFillByte = 3;

	// Only the ring's first word is cleared; fdSpriteViewerLayOutRing fills it.
	DMA_FILL32_CELL(fillWord, 0, ring, 4);
	DMA_FILL32_CELL(fillWord, 0, layers, sizeof(layers));

	// All four sprite animations start on the first sprite with an image, animation 1, priority 1;
	// only sprite animation 0 is shown.
	spriteId = fdSpriteViewerStepSpriteId(-1, 1);
	for (i = 0; i < ANIM_LAYER_COUNT; i++)
	{
		layers[i].animationIndex = 1;
		layers[i].priority       = 1;
		layers[i].spriteId       = spriteId;
	}
	layers[0].enabled = 1;
	// Any nonzero value turns the debug mode on.
	DebugMode         = 2;
	// Nothing visible comes of this blend: BG1 is never turned on below, and the viewer's sprites
	// are all in normal mode (fdSpriteCreate); only a semi-transparent sprite blends whatever
	// BLDCNT's first target is (GBATEK, "Semi-Transparent OBJs").
	REG_BLDCNT        = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
	// A green backdrop behind BG0 and the sprites, the only layers shown.
	PLTT_BACKDROP     = RGB(0, 15, 0);
	REG_DISPCNT       = DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_OBJ_ON;
	syScreenFadeFromBlack(1);

	// Rebuilt from here whenever sprite animation 0's sprite changes or Select is pressed: a fresh
	// heap, event table and VRAM cache each time.
restart:
	syHeapInit();
	syInitEvent();
	ring = syHeapSlotAllocateEWRAM(HEAP_SLOT_BATTLE, SPRITE_VIEWER_COPY_COUNT * sizeof(*ring));
	syVRAMCacheReset();
	fdActorSystemInit(ACTOR_SYSTEM_MAP);
	// With no map loaded, the map metatile buffer holds sprite animation 0's pictures; a two-part
	// sprite (ANIM_FACING_TWO_PART) puts its second part in the actor sprite buffer, as sprite
	// picture cache 1.
	fdSpritePictureCacheLoad(0, (Uint32 *)RamMapGridMetatileBuffer, layers[0].spriteId, 0);
	spriteMetadata = syResourceGetMetadataRecordStub(layers[0].spriteId);
	if (spriteMetadata->drawKind == ANIM_FACING_TWO_PART)
	{
		fdSpritePictureCacheLoad(1, (Uint32 *)RamEnemySpriteSlots, layers[0].spriteId + 1, 0);
	}

	// Ten copies of the sprite; for a two-part sprite every other copy shows its second part,
	// number + 1, from sprite picture cache 1.
	for (i = 0; i < SPRITE_VIEWER_COPY_COUNT; i++)
	{
		SPR   *sprite;
		Sint32 part    = 0;

		spriteMetadata = syResourceGetMetadataRecordStub(layers[0].spriteId);
		if (spriteMetadata->drawKind == ANIM_FACING_TWO_PART && (i & 1) != 0)
		{
			part = 1;
		}
		sprite = fdSpriteCreate(SPRITE_PICTURE_TAG(part, layers[0].spriteId + part));
		// Sprite animations 1 to 3 are added to every copy whether shown or not; a hidden sprite
		// animation is kept out of the drawing by its priority below.
		fdSpriteAddAnimation(sprite, layers[1].spriteId);
		fdSpriteAddAnimation(sprite, layers[2].spriteId);
		fdSpriteAddAnimation(sprite, layers[3].spriteId);
		sprite->flags = shadowsEnabled;
	}

	// Priority 8 hides a sprite animation: fdSpriteComposePicture draws only priorities 0 to 3.
	for (i = 0; i < ANIM_LAYER_COUNT; i++)
	{
		if (layers[i].enabled != 0)
		{
			fdSpriteViewerSetLayerPriority(i, layers[i].priority);
		}
		else
		{
			fdSpriteViewerSetLayerPriority(i, SPRITE_VIEWER_HIDDEN_PRIORITY);
		}
		fdSpriteViewerSetLayerColorMode(i, layers[i].colorMode);
		fdSpriteViewerStartStaggeredAnimations(i, layers[i].animationIndex);
	}
	// The copies are drawn each tick by the event below, from the ring's positions.
	fdSpriteViewerLayOutRing(x, y, ring);
	sySetEvent(fdSpriteViewerPlaceRing, EVENT_PASS_TICK);

next:
	sySynch(1);
	heldKeys     = &PerData;
	repeatedKeys = &PerRepeat;
	for (;;)
	{
		// Start held: the direction pad moves the ring a pixel a tick; else up and down choose a
		// sprite animation and left and right what L and R change on it.
		if ((*heldKeys & KEY_START) != 0)
		{
			if ((*heldKeys & KEY_LEFT) != 0)
			{
				x--;
			}
			if ((*heldKeys & KEY_RIGHT) != 0)
			{
				x++;
			}
			if ((*heldKeys & KEY_UP) != 0)
			{
				y--;
			}
			if ((*heldKeys & KEY_DOWN) != 0)
			{
				y++;
			}
			fdSpriteViewerLayOutRing(x, y, ring);
		}
		else
		{
			// DEAD: a read of PerRepeat whose value goes unused; the ROM keeps the load.
			(void)PerRepeat;
			if ((*repeatedKeys & KEY_UP) != 0)
			{
				selectedLayer = (selectedLayer - 1) & (ANIM_LAYER_COUNT - 1);
			}
			if ((PerRepeat & KEY_DOWN) != 0)
			{
				selectedLayer = (selectedLayer + 1) & (ANIM_LAYER_COUNT - 1);
			}
			if ((PerRepeat & KEY_LEFT) != 0)
			{
				editMode = (editMode - 1) & (SPRITE_VIEWER_EDIT_COUNT - 1);
			}
			if ((PerRepeat & KEY_RIGHT) != 0)
			{
				editMode++;
				editMode &= SPRITE_VIEWER_EDIT_COUNT - 1;
			}
		}

		// A press of Start also toggles the shadows (SPRITE_FLAG_SHADOW) of the ten copies.
		if ((PerPush & KEY_START) != 0)
		{
			sprites         = SpritesWork;
			shadowsEnabled ^= 1;
			for (i = 0; i < SPRITE_VIEWER_COPY_COUNT; i++)
			{
				sprites->flags = shadowsEnabled;
				sprites++;
			}
		}

		switch (editMode)
		{
			// The animation: sprite animation 1 follows sprite animation 0's and is not edited on
			// its own; 0 to 99.
			case SPRITE_VIEWER_EDIT_ANIMATION:
				if (selectedLayer == 1)
				{
					break;
				}
				if ((PerRepeat & KEY_L) != 0)
				{
					layers[selectedLayer].animationIndex--;
					if (layers[selectedLayer].animationIndex < 0)
					{
						layers[selectedLayer].animationIndex = 0;
					}
					if (selectedLayer == 0)
					{
						layers[1].animationIndex = layers[0].animationIndex;
					}
					goto next;
				}
				if ((PerRepeat & KEY_R) == 0)
				{
					break;
				}
				layers[selectedLayer].animationIndex++;
				if (layers[selectedLayer].animationIndex >= SPRITE_VIEWER_ANIMATION_LIMIT)
				{
					layers[selectedLayer].animationIndex = SPRITE_VIEWER_ANIMATION_LIMIT - 1;
				}
				if (selectedLayer == 0)
				{
					layers[1].animationIndex = layers[0].animationIndex;
				}
				goto next;

			// The sprite number: one sprite with an image at a time, or ten with Start held.
			case SPRITE_VIEWER_EDIT_SPRITE:
				changed     = 0;
				spriteSteps = 1;
				if ((*heldKeys & KEY_START) != 0)
				{
					spriteSteps = SPRITE_VIEWER_FAST_STEP;
				}
				if ((*repeatedKeys & KEY_L) != 0)
				{
					for (i = 0; i < spriteSteps; i++)
					{
						layers[selectedLayer].spriteId =
							fdSpriteViewerStepSpriteId(layers[selectedLayer].spriteId, -1);
					}
					changed = 1;
				}
				if ((*repeatedKeys & KEY_R) != 0)
				{
					for (i = 0; i < spriteSteps; i++)
					{
						layers[selectedLayer].spriteId =
							fdSpriteViewerStepSpriteId(layers[selectedLayer].spriteId, 1);
					}
					changed = 1;
				}
				if (changed == 0)
				{
					break;
				}
				// A new sprite on sprite animation 0 changes the ring and the copies: start over.
				if (selectedLayer == 0)
				{
					goto restart;
				}
				// A hidden sprite animation only remembers the number until it is shown again.
				if (layers[selectedLayer].enabled != 0)
				{
					fdSpriteViewerSetLayerSprite(selectedLayer, layers[selectedLayer].spriteId);
					fdSpriteViewerSetLayerPriority(selectedLayer, layers[selectedLayer].priority);
					fdSpriteViewerSetLayerColorMode(selectedLayer, layers[selectedLayer].colorMode);
					fdSpriteViewerStartStaggeredAnimations(selectedLayer,
					                                       layers[selectedLayer].animationIndex);
				}
				goto next;

			// The sprite animation's draw priority, 0 to 3 and wrapping.
			case SPRITE_VIEWER_EDIT_PRIORITY:
				changed = 0;
				if ((*repeatedKeys & KEY_L) != 0)
				{
					layers[selectedLayer].priority--;
					if (layers[selectedLayer].priority < 0)
					{
						layers[selectedLayer].priority = 3;
					}
					changed = 1;
				}
				if ((*repeatedKeys & KEY_R) != 0)
				{
					layers[selectedLayer].priority++;
					if (layers[selectedLayer].priority > 3)
					{
						layers[selectedLayer].priority = 0;
					}
					changed = 1;
				}
				if (changed == 0)
				{
					break;
				}
				if (layers[selectedLayer].enabled != 0)
				{
					fdSpriteViewerSetLayerPriority(selectedLayer, layers[selectedLayer].priority);
				}
				goto next;

			// The draw parameter (SPR_COL_MODE), 0 to 15 and wrapping; SPRITE_COLOR_HIDDEN is the
			// value fdSpriteSetAnimationColorModes leaves alone.
			case SPRITE_VIEWER_EDIT_COLOR:
				changed = 0;
				if ((*repeatedKeys & KEY_L) != 0)
				{
					layers[selectedLayer].colorMode--;
					if (layers[selectedLayer].colorMode < 0)
					{
						layers[selectedLayer].colorMode = SPRITE_COLOR_HIDDEN;
					}
					changed = 1;
				}
				if ((*repeatedKeys & KEY_R) != 0)
				{
					layers[selectedLayer].colorMode++;
					if (layers[selectedLayer].colorMode > SPRITE_COLOR_HIDDEN)
					{
						layers[selectedLayer].colorMode = 0;
					}
					changed = 1;
				}
				if (changed == 0)
				{
					break;
				}
				if (layers[selectedLayer].enabled != 0)
				{
					fdSpriteViewerSetLayerColorMode(selectedLayer, layers[selectedLayer].colorMode);
				}
				goto next;

			default: break;
		}

		// A, editing the animation: restart the selected sprite animation (sprite animation 0 also
		// restarts sprite animation 1, which follows it).
		if ((*repeatedKeys & KEY_A) != 0)
		{
			if (editMode == SPRITE_VIEWER_EDIT_ANIMATION)
			{
				if (selectedLayer != 1)
				{
					if (layers[selectedLayer].enabled != 0)
					{
						fdSpriteViewerStartStaggeredAnimations(
							selectedLayer, layers[selectedLayer].animationIndex);
						if (selectedLayer == 0)
						{
							fdSpriteViewerStartStaggeredAnimations(1, layers[1].animationIndex);
						}
					}
					goto next;
				}
			}
			else if (selectedLayer != 0)
			{
				layers[selectedLayer].enabled ^= 1;
				fdSpriteViewerSetLayerPriority(selectedLayer,
				                               layers[selectedLayer].enabled != 0
				                                   ? layers[selectedLayer].priority
				                                   : SPRITE_VIEWER_HIDDEN_PRIORITY);
				goto next;
			}
		}

		// syDummyReturnZero ignores its arguments, so the resource set chosen here changes nothing
		// in this build.
		if ((*repeatedKeys & KEY_SELECT) != 0)
		{
			syInitEvent();
			if ((*heldKeys & KEY_B) != 0)
			{
				syDummyReturnZero(RESOURCE_RESOURCE_STUBS, StubsResource);
			}
			else
			{
				syDummyReturnZero(RESOURCE_RESOURCE_FAR_CALLS_ALTERNATE, StubsResource);
			}
			goto restart;
		}
		sySynch(1);
	}
}

// Returns the next sprite id after spriteId, stepping by direction (1 or -1) and wrapping within
// 0-511, whose metadata has an image (a nonzero width).
Sint32 fdSpriteViewerStepSpriteId(Sint32 spriteId, Sint32 direction)
{
	for (;;)
	{
		spriteId += direction;
		// Past either end, jump to just beyond the other, so the next step lands on 511 or 0.
		if (spriteId < 0)
		{
			spriteId = SPRITE_VIEWER_SPRITE_LIMIT;
			continue;
		}
		{
			Sint32 spriteLimit = SPRITE_VIEWER_SPRITE_LIMIT;
			if (spriteId >= spriteLimit)
			{
				spriteId = -1;
				continue;
			}
		}
		if (syResourceGetMetadataRecordStub(spriteId)->width)
		{
			return spriteId;
		}
	}
}

// Lays out the sprite viewer's ring around (centerX, centerY), in pixels: one position per facing
// of the first sprite's facing kind (6, 8, 10 or 4, or two opposite pairs for
// ANIM_FACING_TWO_PART), 56 pixels out (40 for a two-part sprite).
void fdSpriteViewerLayOutRing(Sint32 centerX, Sint32 centerY, SPR_PLACEMENT *positions)
{
	SPR_PLACEMENT *pairedPositions;
	Sint32         angle;
	Sint32         facingIndex;
	Uint16         initHeading;
	Uint8          facingKind;

	angle      = 0;
	facingKind = SpritesWork->entries[0]->kind;
	// With B held the ring starts at an angle that turns with the tick counter, a full turn every
	// 256 ticks.
	if ((PerData & KEY_B) != 0)
		angle = (Sint16)(FrameCount << 8);
	// The steps are a sixth, an eighth, a tenth and a quarter of a turn.
	switch ((Uint32)facingKind)
	{
		case ANIM_FACING_THREE_PICTURES:
			for (facingIndex = 0; facingIndex < 6; facingIndex++)
			{
				angle             = (Uint32)angle << ANGLE_BITS;
				positions->x      = toFIXED(centerX);
				angle             = (Uint32)angle >> ANGLE_BITS;
				positions->height = 0;
				positions->y      = toFIXED(centerY);
				syVectorAddPolarOffset(
					toFIXED(56), (Uint16)angle, sySpritePlacementValues(positions));
				positions++;
				angle = (Sint16)(angle + DEGtoANG(60));
			}
			for (; facingIndex < SPRITE_VIEWER_COPY_COUNT; facingIndex++)
				positions++;
			break;
		case ANIM_FACING_FIVE_PICTURES:
		case ANIM_FACING_EIGHT_PICTURES:
		case ANIM_FACING_EIGHT_NO_TABLE:
		case ANIM_FACING_EIGHT_PICTURES_MIRRORED:
			for (facingIndex = 0; facingIndex < 8; facingIndex++)
			{
				angle             = (Uint32)angle << ANGLE_BITS;
				positions->x      = toFIXED(centerX);
				angle             = (Uint32)angle >> ANGLE_BITS;
				positions->height = 0;
				positions->y      = toFIXED(centerY);
				syVectorAddPolarOffset(
					toFIXED(56), (Uint16)angle, sySpritePlacementValues(positions));
				positions++;
				angle = (Sint16)(angle + DEGtoANG(45));
			}
			for (; facingIndex < SPRITE_VIEWER_COPY_COUNT; facingIndex++)
				positions++;
			break;
		case ANIM_FACING_FOUR_PICTURES:
		case ANIM_FACING_SIX_PICTURES:
			for (facingIndex = 0; facingIndex < SPRITE_VIEWER_COPY_COUNT; facingIndex++)
			{
				angle             = (Uint32)angle << ANGLE_BITS;
				positions->x      = toFIXED(centerX);
				angle             = (Uint32)angle >> ANGLE_BITS;
				positions->height = 0;
				positions->y      = toFIXED(centerY);
				syVectorAddPolarOffset(
					toFIXED(56), (Uint16)angle, sySpritePlacementValues(positions));
				positions++;
				angle = (Sint16)(angle + DEGtoANG(36));
			}
			break;
		// Two places 40 pixels out, a quarter turn round and then opposite it, each holding both of
		// the sprite's parts.
		case ANIM_FACING_TWO_PART:
			initHeading     = angle;
			angle           = (Sint16)(initHeading + DEGtoANG(90));
			facingIndex     = 0;
			pairedPositions = positions;
			while (facingIndex < 2)
			{
				pairedPositions[facingIndex * 2].x      = toFIXED(centerX);
				pairedPositions[facingIndex * 2].height = 0;
				pairedPositions[facingIndex * 2].y      = toFIXED(centerY);
				angle                                   = (Uint32)angle << ANGLE_BITS;
				angle                                   = (Uint32)angle >> ANGLE_BITS;
				syVectorAddPolarOffset(toFIXED(40), angle, sySpritePlacementValues(&positions[0]));
				pairedPositions[facingIndex * 2 + 1].x = toFIXED(centerX);
				{
					Sint32 *point = sySpritePlacementValues(&positions[1]);
					pairedPositions[facingIndex * 2 + 1].height = 0;
					pairedPositions[facingIndex * 2 + 1].y      = toFIXED(centerY);
					syVectorAddPolarOffset(toFIXED(40), (Uint16)angle, point);
				}
				angle = ((Uint32)angle + DEGtoANG(180)) << ANGLE_BITS;
				facingIndex++;
				positions  += 2;
				angle     >>= ANGLE_BITS;
			}
			break;
		// Any other kind: four places, starting an eighth of a turn round.
		default:
			angle = (Sint32)(((Uint32)angle << ANGLE_BITS) + ((Uint32)DEGtoANG(45) << ANGLE_BITS))
			     >> ANGLE_BITS;
			for (facingIndex = 0; facingIndex < 4; facingIndex++)
			{
				angle             = (Uint32)angle << ANGLE_BITS;
				angle             = (Uint32)angle >> ANGLE_BITS;
				positions->x      = toFIXED(centerX);
				positions->height = 0;
				positions->y      = toFIXED(centerY);
				syVectorAddPolarOffset(
					toFIXED(56), (Uint16)angle, sySpritePlacementValues(positions));
				positions++;
				angle = (Sint16)(angle + DEGtoANG(90));
			}
			for (facingIndex = 4; facingIndex < SPRITE_VIEWER_COPY_COUNT; facingIndex++)
				positions++;
			break;
	}
}

// Defines name(layerIndex, value), which sets `field` of sprite animation layerIndex (0 to 3) of
// each of the viewer's ten sprites to value.
#define SPRITE_VIEWER_DEFINE_SET_LAYER(name, field)                                                \
	void name(Sint32 layerIndex, Sint32 value)                                                     \
	{                                                                                              \
		SPR   *spriteGroup = SpritesWork;                                                          \
		Sint32 spriteIndex;                                                                        \
                                                                                                   \
		layerIndex &= ANIM_LAYER_COUNT - 1;                                                        \
		for (spriteIndex = 0; spriteIndex < SPRITE_VIEWER_COPY_COUNT; spriteIndex++)               \
		{                                                                                          \
			spriteGroup->entries[layerIndex]->field = value;                                       \
			spriteGroup++;                                                                         \
		}                                                                                          \
	}

// Sets the colour mode (SPR_COL_MODE, the sprite animation's draw parameter) or the priority of one
// sprite animation of the viewer's ten sprites.
SPRITE_VIEWER_DEFINE_SET_LAYER(fdSpriteViewerSetLayerColorMode, colorMode)
SPRITE_VIEWER_DEFINE_SET_LAYER(fdSpriteViewerSetLayerPriority, priority)

// Starts animationIndex on layerIndex of each of the viewer's ten sprites, each one a tick (16
// timer units) behind the last, so the copies are staggered. Also sets each copy's offsetY and
// clears its rotation.
void fdSpriteViewerStartStaggeredAnimations(Sint32 layerIndex, Sint32 animationIndex)
{
	Sint32 spriteIndex;
	SPR   *spriteGroup;

	spriteGroup = SpritesWork;
	spriteIndex = 0;
	do
	{
		SPR_ANIMATION      *entry = spriteGroup->entries[layerIndex & (ANIM_LAYER_COUNT - 1)];
		ANIMATION_METADATA *metadata;
		Sint32              start = 0;

		if (entry->scripts != 0)
		{
			metadata = syResourceGetMetadataRecordStub(entry->animationId);
			if (animationIndex < metadata->animationCount)
			{
				// Copy n starts n ticks (16 timer units each) late, at normal speed, with no tick
				// drawn yet.
				entry->kind         = metadata->drawKind;
				entry->script       = entry->scripts[animationIndex];
				entry->timer        = spriteIndex * SPRITE_ANIMATION_NORMAL_STEP;
				entry->step         = SPRITE_ANIMATION_NORMAL_STEP;
				entry->scriptOffset = start;
				entry->pictureBase  = start;
				entry->picture      = SPRITE_PICTURE_NONE;
			}
			spriteGroup->offsetY  = metadata->adjustY;
			spriteGroup->rotation = start;
		}
		spriteIndex++;
		spriteGroup++;
	} while (spriteIndex < SPRITE_VIEWER_COPY_COUNT);
}

// Sets spriteId on layerIndex of each of the viewer's ten sprites. Resets each sprite animation
// from its new metadata.
void fdSpriteViewerSetLayerSprite(Sint32 layerIndex, Sint32 spriteId)
{
	SPR   *spriteGroup = SpritesWork;
	Sint32 spriteIndex;

	// DEAD: the sprite's metadata looked up and not used; the ROM keeps the call.
	syResourceGetMetadataRecordStub(spriteId);
	layerIndex &= ANIM_LAYER_COUNT - 1;
	for (spriteIndex = 0; spriteIndex < SPRITE_VIEWER_COPY_COUNT; spriteIndex++)
	{
		SPR_ANIMATION *entry = spriteGroup->entries[layerIndex];

		entry->animationId   = spriteId;
		fdSpriteAnimationReset(entry);
		spriteGroup++;
	}
}

// Each tick in the sprite viewer: places each copy of the sprite at its ring position, facing the
// way that position stands for.
void fdSpriteViewerPlaceRing(void)
{
	SPR           *sprite     = SpritesWork;
	Uint32         facingKind = sprite->entries[0]->kind;
	SPR_SCALE      scale;
	SPR_PLACEMENT *facingPositions;
	Uint16         angle;
	Uint16         step;
	Uint16         pairedFacingStep = 0;
	Uint32         count;
	Uint32         spriteIndex;

	scale           = SpriteViewerScale;
	facingPositions = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE);

	switch (facingKind)
	{
		case ANIM_FACING_THREE_PICTURES:
			angle = 0;
			step  = DEGtoANG(60);
			count = 6;
			break;
		case ANIM_FACING_FIVE_PICTURES:
		case ANIM_FACING_EIGHT_PICTURES:
		case ANIM_FACING_EIGHT_NO_TABLE:
		case ANIM_FACING_EIGHT_PICTURES_MIRRORED:
			angle = 0;
			step  = DEGtoANG(45);
			count = 8;
			break;
		case ANIM_FACING_FOUR_PICTURES:
		case ANIM_FACING_SIX_PICTURES:
			angle = 0;
			step  = DEGtoANG(36);
			count = SPRITE_VIEWER_COPY_COUNT;
			break;
		// The two parts at each place face the same way; the second place faces half a turn from
		// the first.
		case ANIM_FACING_TWO_PART:
			angle            = 0;
			step             = 0;
			pairedFacingStep = DEGtoANG(180);
			count            = 4;
			break;
		// The eighth-turn start matches fdSpriteViewerLayOutRing's default ring.
		default:
			angle = DEGtoANG(45);
			step  = DEGtoANG(90);
			count = 4;
			break;
	}

	// Copy n faces n steps round, the way its ring position lies.
	for (spriteIndex = 0; spriteIndex < count; spriteIndex++)
	{
		fdRenderApplyProjectedPlacement(sprite, &facingPositions[spriteIndex], &scale, angle);
		sprite++;
		angle += step;
		if (spriteIndex & 1)
			angle += pairedFacingStep;
	}
}

#undef SPRITE_VIEWER_DEFINE_SET_LAYER
#undef ANGLE_BITS

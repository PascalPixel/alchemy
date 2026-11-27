// A debug sprite viewer. Part of field.c.

extern const Uint16 SpriteViewerBackdropColors[8];

void                fdSpriteViewerSetLayerPriority(Sint32 slot, Sint32 value);
void                fdSpriteViewerSetLayerColorMode(Sint32 slot, Sint32 value);
Sint32              fdSpriteViewerStartStaggeredAnimations(Sint32 slot, Sint32 value);
void                fdSpriteViewerSetLayerSprite(Sint32 slot, Sint32 value);
void                fdSpriteViewerLayOutRing(Sint32 x, Sint32 z, SPR_PLACEMENT *out);

Sint32              fdSpriteViewerStepSpriteId(Sint32 value, Sint32 delta);
void                fdSpriteViewerPlaceRing(void);

enum
{
    // The sprite copies and their ring positions; a facing kind places 4, 6, 8 or 10 of them.
    UI_SPRITE_VIEWER_COPY_COUNT   = 10,
    UI_SPRITE_VIEWER_SPRITE_LIMIT = 716, // sprite ids 0 to 715 are offered
    UI_SPRITE_VIEWER_ANIMATION_LIMIT =
        100,                             // animations 0 to 99
             // A sprite animation priority above 3, which fdSpriteComposePicture skips.
    UI_SPRITE_VIEWER_HIDDEN_PRIORITY = 8,
    // The sprite ids L and R step by while Start is held: any step past one goes to the next stop
    // of SpriteViewerSkipStops.
    UI_SPRITE_VIEWER_FAST_STEP       = 10,
    // SpriteViewerSkipStops' entries, its end marker the last.
    UI_SPRITE_VIEWER_SKIP_STOPS      = 10,
    // The colour SPRITE_COLOR_FILL paints in the viewer (DecodeFillByte): colour 3 of the basic
    // sprite palette, orange.
    UI_SPRITE_VIEWER_FILL_COLOR      = 3,
    // DebugMode while the viewer runs. Every reader tests it only for nonzero, so 2 turns on the
    // same debug keys any other value would.
    UI_SPRITE_VIEWER_DEBUG_MODE      = 2
};

// An angle's bits: shifted up and back down, a value keeps only them.
#define ANGLE_BITS 16

// What left and right choose for L and R to change on the selected sprite animation.
typedef enum UI_SPR_VIEWER_EDIT_MODE
{
    UI_SPRITE_VIEWER_EDIT_ANIMATION,
    UI_SPRITE_VIEWER_EDIT_SPRITE,
    UI_SPRITE_VIEWER_EDIT_PRIORITY,
    UI_SPRITE_VIEWER_EDIT_COLOR,
    UI_SPRITE_VIEWER_EDIT_COUNT
} UI_SPR_VIEWER_EDIT_MODE;

// One sprite animation the sprite viewer edits.
typedef struct UI_SPR_VIEWER_LAYER
{
    Sint16 spriteId;       // metadata record selected for this layer
    Sint16 animationIndex; // animation script selected from that metadata
    Uint8  enabled;        // zero hides it by a priority above the drawing range
    Sint8  priority;       // drawing order, 0 to 3
    Sint8  colorMode;      // SPR_COL_MODE; the viewer cycles through 0 to 15
} UI_SPR_VIEWER_LAYER;

// The debug sprite viewer, in The Broken Seal's form with this game's changes; it never returns.
void fdSpriteViewerRun(void)
{
    UI_SPR_VIEWER_LAYER layers[SPRITE_ANIMATION_COUNT];
    volatile Uint32     zero;
    Sint32              x;
    Sint32              y;
    Uint32              editMode;
    Sint32              shadowsEnabled;
    Uint32              backdrop;
    void               *ring;
    Uint32              selectedLayer;
    Uint32              i;
    Sint32              spriteId;
    Sint32              changed;

    x              = 144;
    y              = 96;
    editMode       = UI_SPRITE_VIEWER_EDIT_SPRITE;
    selectedLayer  = 0;
    shadowsEnabled = 1;
    backdrop       = 0;
    ring           = syHeapSlotAllocateEWRAM(HEAP_SLOT_BATTLE, 160);
    DecodeFillByte = UI_SPRITE_VIEWER_FILL_COLOR;
    DMA_FILL32_CELL(zero, 0, ring, 4);
    DMA_FILL32_CELL(zero, 0, layers, sizeof(layers));
    spriteId = fdSpriteViewerStepSpriteId(-1, 1);
    for (i = 0; i < SPRITE_ANIMATION_COUNT; i++)
    {
        layers[i].animationIndex = 1;
        layers[i].priority       = 1;
        layers[i].spriteId       = spriteId;
    }
    layers[0].enabled           = 1;
    DebugMode                   = UI_SPRITE_VIEWER_DEBUG_MODE;
    REG_BLDCNT                  = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
    *(volatile Uint16 *)BG_PLTT = RGB(0, 0, 31);
    REG_DISPCNT                 = DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_OBJ_ON;
    syScreenFadeFromBlack(1);

restart:
    syHeapInit();
    syInitEvent();
    ring = syHeapSlotAllocateEWRAM(HEAP_SLOT_BATTLE, 160);
    syVRAMCacheReset();
    fdActorSystemInit(ACTOR_SYSTEM_MAP);
    fdSpritePictureCacheLoad(0, MapGridMetatileBuffer, layers[0].spriteId, 0);
    if (fdResourceGetMetadataRecordStub(layers[0].spriteId)->drawKind == ANIM_FACING_TWO_PART)
        fdSpritePictureCacheLoad(1, ActorSpriteSlots, layers[0].spriteId + 1, 0);
    for (i = 0; i < UI_SPRITE_VIEWER_COPY_COUNT; i++)
    {
        SPR   *sprite;
        Sint32 part = 0;

        if (fdResourceGetMetadataRecordStub(layers[0].spriteId)->drawKind == ANIM_FACING_TWO_PART
            && (i & 1))
            part = 1;
        sprite = fdSpriteCreate(SPRITE_PICTURE_TAG(part, layers[0].spriteId + part));
        fdSpriteAddAnimation(sprite, layers[1].spriteId);
        fdSpriteAddAnimation(sprite, layers[2].spriteId);
        fdSpriteAddAnimation(sprite, layers[3].spriteId);
        sprite->flags = shadowsEnabled;
    }
    for (i = 0; i < SPRITE_ANIMATION_COUNT; i++)
    {
        if (layers[i].enabled != 0)
            fdSpriteViewerSetLayerPriority(i, layers[i].priority);
        else
            fdSpriteViewerSetLayerPriority(i, UI_SPRITE_VIEWER_HIDDEN_PRIORITY);
        fdSpriteViewerSetLayerColorMode(i, layers[i].colorMode);
        fdSpriteViewerStartStaggeredAnimations(i, layers[i].animationIndex);
    }
    fdSpriteViewerLayOutRing(x, y, ring);
    sySetEvent(fdSpriteViewerPlaceRing, EVENT_PASS_TICK);

next:
    sySynch(1);
    for (;;)
    {
        if (PerWork.data & KEY_START)
        {
            if (PerWork.data & KEY_LEFT)
                x--;
            if (PerWork.data & KEY_RIGHT)
                x++;
            if (PerWork.data & KEY_UP)
                y--;
            if (PerWork.data & KEY_DOWN)
                y++;
            fdSpriteViewerLayOutRing(x, y, ring);
        }
        else
        {
            // DEAD: a read of the repeated keys nothing uses; the ROM keeps the load.
            (void)PerWork.repeat;
            if (PerWork.repeat & KEY_UP)
                selectedLayer = (selectedLayer - 1) & (SPRITE_ANIMATION_COUNT - 1);
            if (PerWork.repeat & KEY_DOWN)
                selectedLayer = (selectedLayer + 1) & (SPRITE_ANIMATION_COUNT - 1);
            if (PerWork.repeat & KEY_LEFT)
                editMode = (editMode - 1) & (UI_SPRITE_VIEWER_EDIT_COUNT - 1);
            if (PerWork.repeat & KEY_RIGHT)
                editMode = (editMode + 1) & (UI_SPRITE_VIEWER_EDIT_COUNT - 1);
        }

        if (PerWork.push & KEY_START)
        {
            SPR *sprites    = RamHeapSlots->sprites;

            shadowsEnabled ^= 1;
            for (i = 0; i < UI_SPRITE_VIEWER_COPY_COUNT; i++)
            {
                sprites->flags = shadowsEnabled;
                sprites++;
            }
        }

        switch (editMode)
        {
            case UI_SPRITE_VIEWER_EDIT_ANIMATION:
                // The second sprite animation follows the first's animation.
                if (selectedLayer == 1)
                    break;
                if (PerWork.repeat & KEY_L)
                {
                    layers[selectedLayer].animationIndex--;
                    if (layers[selectedLayer].animationIndex < 0)
                        layers[selectedLayer].animationIndex = 0;
                    if (selectedLayer == 0)
                        layers[1].animationIndex = layers[0].animationIndex;
                    goto next;
                }
                if (!(PerWork.repeat & KEY_R))
                    break;
                layers[selectedLayer].animationIndex++;
                if (layers[selectedLayer].animationIndex > UI_SPRITE_VIEWER_ANIMATION_LIMIT - 1)
                    layers[selectedLayer].animationIndex = UI_SPRITE_VIEWER_ANIMATION_LIMIT - 1;
                if (selectedLayer == 0)
                    layers[1].animationIndex = layers[0].animationIndex;
                goto next;

            case UI_SPRITE_VIEWER_EDIT_SPRITE:
            {
                Sint32 steps;

                changed = 0;
                steps   = 1;
                if (PerWork.data & KEY_START)
                    steps = UI_SPRITE_VIEWER_FAST_STEP;
                if (PerWork.repeat & KEY_L)
                {
                    layers[selectedLayer].spriteId =
                        fdSpriteViewerStepSpriteId(layers[selectedLayer].spriteId, -steps);
                    changed = 1;
                }
                if (PerWork.repeat & KEY_R)
                {
                    layers[selectedLayer].spriteId =
                        fdSpriteViewerStepSpriteId(layers[selectedLayer].spriteId, steps);
                    changed = 1;
                }
                if (changed == 0)
                    break;
                if (selectedLayer == 0)
                    goto restart;
                if (layers[selectedLayer].enabled != 0)
                {
                    fdSpriteViewerSetLayerSprite(selectedLayer, layers[selectedLayer].spriteId);
                    fdSpriteViewerSetLayerPriority(selectedLayer, layers[selectedLayer].priority);
                    fdSpriteViewerSetLayerColorMode(selectedLayer, layers[selectedLayer].colorMode);
                    fdSpriteViewerStartStaggeredAnimations(selectedLayer,
                                                           layers[selectedLayer].animationIndex);
                }
                goto next;
            }

            case UI_SPRITE_VIEWER_EDIT_PRIORITY:
                changed = 0;
                if (PerWork.repeat & KEY_L)
                {
                    layers[selectedLayer].priority--;
                    if (layers[selectedLayer].priority < 0)
                        layers[selectedLayer].priority = 3;
                    changed = 1;
                }
                if (PerWork.repeat & KEY_R)
                {
                    layers[selectedLayer].priority++;
                    if (layers[selectedLayer].priority > 3)
                        layers[selectedLayer].priority = 0;
                    changed = 1;
                }
                if (changed == 0)
                    break;
                if (layers[selectedLayer].enabled != 0)
                    fdSpriteViewerSetLayerPriority(selectedLayer, layers[selectedLayer].priority);
                goto next;

            case UI_SPRITE_VIEWER_EDIT_COLOR:
                changed = 0;
                if (PerWork.repeat & KEY_L)
                {
                    layers[selectedLayer].colorMode--;
                    if (layers[selectedLayer].colorMode < 0)
                        layers[selectedLayer].colorMode = SPRITE_COLOR_HIDDEN;
                    changed = 1;
                }
                if (PerWork.repeat & KEY_R)
                {
                    layers[selectedLayer].colorMode++;
                    if (layers[selectedLayer].colorMode > SPRITE_COLOR_HIDDEN)
                        layers[selectedLayer].colorMode = 0;
                    changed = 1;
                }
                if (changed == 0)
                    break;
                if (layers[selectedLayer].enabled != 0)
                    fdSpriteViewerSetLayerColorMode(selectedLayer, layers[selectedLayer].colorMode);
                goto next;
        }

        if (PerWork.repeat & KEY_A)
        {
            if (editMode == UI_SPRITE_VIEWER_EDIT_ANIMATION)
            {
                if (selectedLayer != 1)
                {
                    if (layers[selectedLayer].enabled != 0)
                    {
                        fdSpriteViewerStartStaggeredAnimations(
                            selectedLayer, layers[selectedLayer].animationIndex);
                        if (selectedLayer == 0)
                            fdSpriteViewerStartStaggeredAnimations(1, layers[1].animationIndex);
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
                                                   : UI_SPRITE_VIEWER_HIDDEN_PRIORITY);
                goto next;
            }
        }
        if (PerWork.repeat & KEY_SELECT)
        {
            syInitEvent();
            syDummyReturnZero(RESOURCE_SPRITE_METADATA, SpriteAnimationMetadata);
            goto restart;
        }
        if (PerWork.push & KEY_B)
        {
            volatile Uint16 *palette = (volatile Uint16 *)BG_PLTT;
            const Uint16    *colors  = SpriteViewerBackdropColors;
            backdrop++;
            backdrop &= 7;
            // B steps the backdrop through its eight colours.
            *palette  = *(const Uint16 *)((Uint32)colors + (backdrop << 1));
        }
        sySynch(1);
    }
}

extern const Uint16 SpriteViewerSkipStops[];

// Steps a sprite id (0 to 715) by delta: a step of one wraps around the range, a larger step
// forward goes to the next stop above value and one back to the last stop below it
// (SpriteViewerSkipStops, ended by (Uint16)-1, from which a step forward wraps to 0). A delta of 0
// returns value unchanged.
Sint32              fdSpriteViewerStepSpriteId(Sint32 value, Sint32 delta)
{
    Sint32 step;

    if (delta != 0)
    {
        step = delta;
        if (delta < 0)
            step = -delta;
        if (step == 1)
        {
            value += delta;
            if (value < 0)
                value = UI_SPRITE_VIEWER_SPRITE_LIMIT - 1;
            if (value >= UI_SPRITE_VIEWER_SPRITE_LIMIT)
                value = 0;
        }
        else if (delta > 0)
        {
            Uint32 i;

            for (i = 0; i < UI_SPRITE_VIEWER_SKIP_STOPS; i++)
            {
                if (value < SpriteViewerSkipStops[i])
                {
                    value = SpriteViewerSkipStops[i];
                    if (value == (Uint16)-1)
                        value = 0;
                    break;
                }
            }
        }
        else if (value == 0)
        {
            value = SpriteViewerSkipStops[UI_SPRITE_VIEWER_SKIP_STOPS - 2];
        }
        else
        {
            Sint32 i;

            for (i = UI_SPRITE_VIEWER_SKIP_STOPS - 2; i >= 0; i--)
            {
                if (value > SpriteViewerSkipStops[i])
                {
                    value = SpriteViewerSkipStops[i];
                    break;
                }
            }
        }
        return value;
    }
#if FIX_FUNCTION_SIGNATURES
    return value;
#else
    // UB: no return statement for a delta of 0; UBFIX returns value unchanged.
#endif
}

// Lays out the sprite viewer's ring around pixel (x, z): one position per facing of the first
// sprite's facing kind (6, 8, 10 or 4, or two opposite pairs for ANIM_FACING_TWO_PART), 56 pixels
// out (40 for a two-part sprite). B held turns the ring. Called by fdSpriteViewerRun when it starts
// and while Start moves the ring.
void fdSpriteViewerLayOutRing(Sint32 centerX, Sint32 centerY, SPR_PLACEMENT *positions)
{
    SPR_PLACEMENT *pair;
    Sint32         angle;
    Sint32         i;
    Uint16         turn;
    Uint8          kind;

    angle = 0;
    kind  = ((SPR *)RamHeapSlots->sprites)->entries[0]->kind;
    if ((PerWork.data & KEY_B) != 0)
        angle = (Sint16)(FrameCount << 8);
    switch ((Uint32)kind)
    {
        case ANIM_FACING_THREE_PICTURES:
            for (i = 0; i < 6; i++)
            {
                angle             <<= ANGLE_BITS;
                positions->x        = toFIXED(centerX);
                angle               = (Uint32)angle >> ANGLE_BITS;
                positions->height   = 0;
                positions->y        = toFIXED(centerY);
                syVectorAddPolarOffset(toFIXED(56), (Uint16)angle, &positions->x);
                positions++;
                angle = (Sint16)(angle + DEGtoANG(60));
            }
            for (; i < UI_SPRITE_VIEWER_COPY_COUNT; i++)
                positions++;
            break;
        case ANIM_FACING_FIVE_PICTURES:
        case ANIM_FACING_EIGHT_PICTURES:
        case ANIM_FACING_EIGHT_NO_TABLE:
        case ANIM_FACING_EIGHT_PICTURES_MIRRORED:
            for (i = 0; i < 8; i++)
            {
                angle             = (Uint32)angle << ANGLE_BITS;
                positions->x      = toFIXED(centerX);
                angle             = (Uint32)angle >> ANGLE_BITS;
                positions->height = 0;
                positions->y      = toFIXED(centerY);
                syVectorAddPolarOffset(toFIXED(56), (Uint16)angle, &positions->x);
                positions++;
                angle = (Sint16)(angle + DEGtoANG(45));
            }
            for (; i < UI_SPRITE_VIEWER_COPY_COUNT; i++)
                positions++;
            break;
        case ANIM_FACING_FOUR_PICTURES:
        case ANIM_FACING_SIX_PICTURES:
            for (i = 0; i < UI_SPRITE_VIEWER_COPY_COUNT; i++)
            {
                angle             <<= ANGLE_BITS;
                positions->x        = toFIXED(centerX);
                angle               = (Uint32)angle >> ANGLE_BITS;
                positions->height   = 0;
                positions->y        = toFIXED(centerY);
                syVectorAddPolarOffset(toFIXED(56), (Uint16)angle, &positions->x);
                positions++;
                angle = (Sint16)(angle + DEGtoANG(36));
            }
            break;
        case ANIM_FACING_TWO_PART:
            turn  = angle;
            angle = (Sint16)(turn + DEGtoANG(90));
            i     = 0;
            pair  = positions;
            while (i < 2)
            {
                pair[i * 2].x        = toFIXED(centerX);
                pair[i * 2].height   = 0;
                pair[i * 2].y        = toFIXED(centerY);
                angle              <<= ANGLE_BITS;
                angle                = (Uint32)angle >> ANGLE_BITS;
                syVectorAddPolarOffset(toFIXED(40), angle, &positions[0].x);
                pair[i * 2 + 1].x = toFIXED(centerX);
                {
                    Sint32 *point          = sySpritePlacementValues(&positions[1]);
                    pair[i * 2 + 1].height = 0;
                    pair[i * 2 + 1].y      = toFIXED(centerY);
                    syVectorAddPolarOffset(toFIXED(40), (Uint16)angle, point);
                }
                angle = ((Uint32)angle + DEGtoANG(180)) << ANGLE_BITS;
                i++;
                positions  += 2;
                angle     >>= ANGLE_BITS;
            }
            break;
        default:
            angle = (Sint32)(((Uint32)angle << ANGLE_BITS) + ((Uint32)DEGtoANG(45) << ANGLE_BITS))
                 >> ANGLE_BITS;
            for (i = 0; i < 4; i++)
            {
                angle             = (Uint32)angle << ANGLE_BITS;
                angle             = (Uint32)angle >> ANGLE_BITS;
                positions->x      = toFIXED(centerX);
                positions->height = 0;
                positions->y      = toFIXED(centerY);
                syVectorAddPolarOffset(toFIXED(56), (Uint16)angle, &positions->x);
                positions++;
                angle = (Sint16)(angle + DEGtoANG(90));
            }
            for (i = 4; i < UI_SPRITE_VIEWER_COPY_COUNT; i++)
                positions++;
            break;
    }
}

// Gives sprite animation slot of each of the viewer's ten sprites the colour mode value.
void fdSpriteViewerSetLayerColorMode(Sint32 slot, Sint32 value)
{
    SPR   *sprites = RamHeapSlots->sprites;
    Sint32 count;

    for (count = 0; count < 10; count++)
    {
        sprites->entries[slot & 3]->colorMode = value;
        sprites++;
    }
}

// Gives sprite animation slot of each of the viewer's ten sprites the drawing order value.
void fdSpriteViewerSetLayerPriority(Sint32 slot, Sint32 value)
{
    SPR   *sprites = RamHeapSlots->sprites;
    Sint32 count;

    for (count = 0; count < 10; count++)
    {
        sprites->entries[slot & 3]->priority = value;
        sprites++;
    }
}

// Starts animation value on sprite animation slot of each of the viewer's ten sprites, each one a
// tick (16 timer units) behind the last, so the copies are staggered.
Sint32 fdSpriteViewerStartStaggeredAnimations(Sint32 slot, Sint32 value)
{
    Sint32              zero;
    Sint32              count;
    Sint32              offset;
    ANIMATION_METADATA *metadata;
    SPR_ANIMATION      *entry;
    Uint8              *state;

    state  = RamHeapSlots->sprites;
    count  = 0;
    offset = (3 & slot) * sizeof(SPR_ANIMATION *) + offsetof(SPR, entries);
    zero   = 0;
    do
    {
        entry = *(SPR_ANIMATION **)(state + offset);
        if (entry->scripts != 0)
        {
            metadata = fdResourceGetMetadataRecordStub(entry->animationId);
            if (value < metadata->animationCount)
            {
                entry->kind         = metadata->drawKind;
                entry->script       = entry->scripts[value];
                entry->timer        = count * SPRITE_ANIMATION_NORMAL_STEP;
                entry->step         = SPRITE_ANIMATION_NORMAL_STEP;
                entry->scriptOffset = zero;
                entry->pictureBase  = zero;
                entry->picture      = SPRITE_PICTURE_NONE;
            }
            ((SPR *)state)->offsetY  = metadata->adjustY;
            ((SPR *)state)->rotation = zero;
        }
        count += 1;
        state += sizeof(SPR);
    } while (count <= 9);
#if FIX_FUNCTION_SIGNATURES
    return 0;
#else
    // UB: an int function with no return statement. fdSpriteViewerRun's calls treat r0 as set by
    // it, which is how the ROM schedules their arguments.
#endif
}

// Sets sprite no on sprite animation slot of each of the viewer's ten sprites, and resets each
// sprite animation from its new metadata.
void fdSpriteViewerSetLayerSprite(Sint32 slot, Sint32 no)
{
    SPR   *sprites = RamHeapSlots->sprites;
    Sint32 count;

    fdResourceGetMetadataRecordStub(no);
    for (count = 0; count < 10; count++)
    {
        SPR_ANIMATION *entry = sprites->entries[slot & 3];

        entry->animationId   = no;
        fdSpriteAnimationReset(entry);
        sprites++;
    }
}

extern const SPR_SCALE SpriteViewerScale;

// Each tick in the sprite viewer: places each copy of the sprite at its ring position, facing the
// way that position stands for.
void                   fdSpriteViewerPlaceRing(void)
{
    SPR      *sprites = RamHeapSlots->sprites;
    Uint32    kind    = sprites->entries[0]->kind;
    SPR_SCALE scale;
    Uint8    *table;
    Uint16    angle;
    Uint16    step;
    Uint16    odd = 0;
    Uint32    count;
    Uint32    i;

    scale = SpriteViewerScale;
    table = (Uint8 *)RamHeapSlots->battleState;

    switch (kind)
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
            count = UI_SPRITE_VIEWER_COPY_COUNT;
            break;
        case ANIM_FACING_TWO_PART:
            angle = 0;
            step  = 0;
            odd   = DEGtoANG(180);
            count = 4;
            break;
        default:
            angle = DEGtoANG(45);
            step  = DEGtoANG(90);
            count = 4;
            break;
    }

    for (i = 0; i < count; i++)
    {
        fdRenderApplyProjectedPlacement(
            sprites, (Sint32 *)(table + i * sizeof(SPR_PLACEMENT)), (Sint32 *)&scale, angle);
        sprites++;
        angle += step;
        if (i & 1)
            angle += odd;
    }
}

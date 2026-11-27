// Growth, Punji, Vine and Thorn. Part of battle_effect.c.

#define GROWTH_FLARES                         512 // impact flares in PARTICLES
#define THORN_FLARES                          64  // impact flares in PARTICLES for the thorns
// The ticks an impact flare lasts: its seven pictures, two ticks each.
#define GROWTH_FLARE_TICKS                    14
// The columns set out, of which GrowthColumnCounts picks how many grow.
#define GROWING_COLUMNS                       16
// The ticks from one column's start to the next's.
#define COLUMN_INTERVAL                       4

// Growing columns and thorns spring up one after another, each shaking the screen and flaring at
// its root, and the targets react a few ticks later.

// The impact flare's pictures in the vine sprite sheet, after the vines.
#define GROWING_COLUMNS_VINE_FLARE_PICTURES   7769
// The impact flare's pictures in the pillar sprite sheet, after the pillars.
#define GROWING_COLUMNS_PILLAR_FLARE_PICTURES 2815

// What the growing columns grow (btEffectRunGrowingColumns).
typedef enum GROWING_COLUMNS_MODE
{
    GROWING_COLUMNS_VINES,  // Growth
    GROWING_COLUMNS_PILLARS // Punji's green pillars
} GROWING_COLUMNS_MODE;

// Vines grow from the ground, flaring at each root.
//
// Abilities: Growth, Mad Growth, Wild Growth.
void btEffectRunGrowth(BATTLE_EFFECT_PARM *args)
{
    btEffectRunGrowingColumns(args, GROWING_COLUMNS_VINES);
}

// Green pillars grow from the ground, flaring at each root.
//
// Abilities: Punji, Punji Trap, Punji Strike.
void btEffectRunPunji(BATTLE_EFFECT_PARM *args)
{
    btEffectRunGrowingColumns(args, GROWING_COLUMNS_PILLARS);
}

// Vines or green pillars grow one after another, alternately mirrored, each flaring at its root.
void btEffectRunGrowingColumns(BATTLE_EFFECT_PARM *args, Sint32 mode)
{
    BATTLE_EFFECT_STATE *state;
    void                *canvas;
    BLITTER_PAIR         rectangle;
    Sint32               frame;
    Sint32               start;
    DUAL_COLUMN         *setup;
    Sint32               x;
    Sint32               height;
    Sint32               limit;
    Sint32               flarePictures;
    DUAL_COLUMN         *column;
    Sint32               length;
    Sint32               reach;
    Sint8                pillarHeight;
    Sint32               n;
    Sint32               reactFrame;
    Sint32               i;
    PARTICLE            *flare;
    Sint32               flareAge;
    Sint32               picture;
    Sint32               duration;

    state  = RamHeapSlots->battleEffect;
    canvas = RamHeapSlots->battleCanvas;
    EFFECT_START_NARROW_CANVAS(BGCNT_PRIORITY(1));
    REG_BLDCNT = 0;
    if (mode == GROWING_COLUMNS_PILLARS)
    {
        btResourceLoadAndDecompress(RESOURCE_GREEN_PILLAR_SHEET, state->sheet, 1, 1);
    }
    else
    {
        btResourceLoadAndDecompress(RESOURCE_VINE_SHEET, state->sheet, 1, 1);
    }
    if (args->side == EFFECT_SIDE_PARTY)
    {
        REG_BG2X = -(NARROW_CANVAS_PARTY_X << 8);
    }
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_TRANSPARENT);
    rectangle[0] = RamHeapSlots->ramCode;
    EFFECT_LOAD_BLITTER(
        HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_TRANSPARENT);
    rectangle[1] = RamHeapSlots->ramCodePaired;

    duration     = GrowthColumnCounts[args->variant] * COLUMN_INTERVAL + 56;

    EFFECT_FREE_PARTICLES(i, PARTICLES, PARTICLE_POOL_COUNT);

    for (i = 0; i != GROWING_COLUMNS; i++)
    {
        setup         = &((DUAL_COLUMN *)state->particles)[i];
        x             = (GrowthColumnX[i] + (7 & syRandom())) - 4;
        setup->y      = i / 2 + 108;
        setup->x      = x;
        height        = (63 & syRandom()) + 55;
        setup->height = height;
        limit         = GrowthHeightLimits[i % 3];
        if (limit < height)
        {
            setup->height = limit;
        }
        limit        = i * COLUMN_INTERVAL + 8;
        setup->start = limit;
    }

    EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);

    for (frame = 0; frame != duration; frame++)
    {
        Sint32 damageFrame = duration - 64;
        Sint32 fadeStart   = duration - 20;
        Sint32 fadeEnd     = duration - 4;

        if (frame == damageFrame)
        {
            EFFECT_SHOW_DAMAGE(SE_IMPACT);
        }
        if (frame >= fadeStart && frame < fadeEnd)
        {
            // Fade out over the last 16 ticks.
            REG_BLDCNT   = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
            REG_BLDALPHA = EFFECT_BLEND_CANVAS((duration - frame) - 5);
        }
        if (frame < fadeEnd)
        {
            for (i = 0; i != GrowthColumnCounts[args->variant]; i++)
            {
                column = &((DUAL_COLUMN *)state->particles)[i];
                start  = i * COLUMN_INTERVAL + 8;
                if (frame == i * COLUMN_INTERVAL + 9)
                {
                    state->shakeFrame = 2;
                }

                // A column grows from its root.
                if (frame > start)
                {
                    Uint32 shape = i % 3;
                    Sint32 side;

                    length = (frame - start) * 8;
                    reach  = column->height;
                    if (length > reach)
                    {
                        length = reach;
                    }
                    if (mode == GROWING_COLUMNS_VINES)
                    {
                        Uint8 *src;
                        Uint32 width;

                        // FAKEMATCH(temporary): side holds 1 before it is
                        // * masked with i, here and for the pillars; as side = 1
                        // * & i.
                        side   = 1;
                        side  &= i;
                        src    = state->sheet + GrowthVinePictures[shape];
                        width  = GrowthVineWidths[shape];
                        rectangle[side](canvas,
                                        src,
                                        column->x - (width >> 1),
                                        column->y - length,
                                        width,
                                        length);
                    }
                    else
                    {
                        Uint8 *src;
                        Uint32 width;

                        pillarHeight = GrowthPillarHeights[7 & i];
                        if (length > pillarHeight)
                        {
                            length = pillarHeight;
                        }
                        side   = 1;
                        side  &= i;
                        src    = state->sheet + GrowthPillarPictures[shape];
                        width  = GrowthPillarWidths[shape];
                        rectangle[side](canvas,
                                        src,
                                        column->x - (width >> 1),
                                        column->y - length,
                                        width,
                                        length);
                    }
                }

                // The targets react as it grows.
                n = 0;
                if (args->targetCount != 0)
                {
                    reactFrame = start + 4;
                    do
                    {
                        if (frame == reactFrame)
                        {
                            if (!(i & 1))
                            {
                                sySoundPlayStub(SE_IMPACT);
                            }
                            btEffectTintBattler(
                                args->targets[n], SPRITE_COLOR_WHITE, POSE_DAMAGE, n, 3);
                        }
                        n++;
                    } while (n != args->targetCount);
                }
                else
                {
                    reactFrame = start + 4;
                }

                // Impact flares at the root.
                if ((frame == reactFrame) || (frame == (start + 8)))
                {
                    for (n = 0, flare = PARTICLES; n != GROWTH_FLARES; flare++, n++)
                    {
                        if (flare->variant == -1)
                        {
                            flare->x       = ((syRandom() & 15) + column->x) - 8;
                            flare->y       = (syRandom() & 15) + 80;
                            flare->variant = 0;
                            break;
                        }
                    }
                }
            }
        }

        // The flares, over the columns, each beside its mirror image.
        for (i = 0; i != GROWTH_FLARES; i++)
        {
            flare    = &PARTICLES[i];
            flareAge = flare->variant;
            if (flareAge >= 0)
            {
                picture       = flareAge / 2;
                flarePictures = GROWING_COLUMNS_VINE_FLARE_PICTURES;
                if (mode != GROWING_COLUMNS_VINES)
                {
                    flarePictures = GROWING_COLUMNS_PILLAR_FLARE_PICTURES;
                }
                rectangle[0](canvas,
                             state->sheet + (GrowthPuffPictures[picture] + flarePictures),
                             flare->x - GrowthPuffWidths[picture],
                             flare->y - (Sint8)GrowthPuffHeights[picture] / 2,
                             GrowthPuffWidths[picture],
                             (Sint8)GrowthPuffHeights[picture]);
                rectangle[1](canvas,
                             state->sheet + (GrowthPuffPictures[picture] + flarePictures),
                             flare->x,
                             flare->y - (Sint8)GrowthPuffHeights[picture] / 2,
                             GrowthPuffWidths[picture],
                             (Sint8)GrowthPuffHeights[picture]);
                flare->variant++;
                if (flare->variant == GROWTH_FLARE_TICKS)
                {
                    flare->variant = -1;
                }
            }
        }

        btCameraShake(4, 4);
        EFFECT_END_TICK(state);
    }
    EFFECT_FINISH_CANVAS();
}

#define VINE_COLUMNS        16 // columns that rise
#define VINE_TICKS          70
// The tick the vines reach the targets, which flash.
#define VINE_REACH_TICK     32
// GreenVineSheet: five 32 by 32 pictures of a vine, drawn in turn.
#define GREEN_VINE_WIDTH    32
#define GREEN_VINE_HEIGHT   32
#define GREEN_VINE_PICTURES 5

// Sixteen green vines grow from the middle of the row of targets, 2 ticks apart, and shrink back.
//
// Abilities: Vine.
void btEffectRunVine(BATTLE_EFFECT_PARM *args)
{
    HEAP_SLOTS          *heap;
    BATTLE_EFFECT_STATE *state;
    void                *canvas;
    BLITTER_PAIR         draw;
    Sint32               origin[3];
    Sint32               originWorld[3];
    SCREEN_POINT         first;
    SCREEN_POINT         last;
    Sint32               i;
    Sint32               frame;
    Sint32               picture;
    Sint32               height;
    Sint32               offset;
    Sint32               middle;
    PARTICLE            *column;

    heap   = RamHeapSlots;
    state  = (BATTLE_EFFECT_STATE *)heap->battleEffect;
    canvas = heap->battleCanvas;
    btEffectStartCanvasLayer(0);
    btEffectRunDjinniEntrance(
        args,
        DJINNI_ENTRANCE(ELEMENT_VENUS + DJINNI_ENTRANCE_LONG, args->side, DJINNI_ANCHOR_CENTRE),
        origin,
        originWorld);
    REG_BG2CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
    REG_BG2PA  = Q_8_8(1);
    REG_BLDCNT = 0;
    btResourceLoadAndDecompress(RESOURCE_GREEN_VINE_SHEET, state->sheet, 1, 1);

    // Centre the vines between the first and last targets.
    btEffectPosProjectScaledBattler(args->targets[0], &first);
    btEffectPosProjectScaledBattler(args->targets[args->targetCount - 1], &last);
    middle    = first.x;
    middle   += (last.x - middle) / 2;
    first.x   = middle;
    REG_BG2X  = (CANVAS_SIZE / 2 - first.x) << 8;
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_TRANSPARENT);
    draw[0] = heap->ramCode;
    EFFECT_LOAD_BLITTER(
        HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_TRANSPARENT);
    draw[1] = heap->ramCodePaired;
    column  = state->particles;
    for (i = 0; i != VINE_COLUMNS; i++)
    {
        column[i].x = VineColumnOffsets[i] + CANVAS_SIZE / 2;
    }
    EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
    for (frame = 0; frame != VINE_TICKS; frame++)
    {
        if (frame == VINE_REACH_TICK)
        {
            sySoundPlayStub(SE_CRACK);
            for (i = 0; i != args->targetCount; i++)
                btEffectTintBattler(args->targets[i], SPRITE_COLOR_WHITE, POSE_DAMAGE, i, 16);
        }
        for (i = 0, column = state->particles; i != VINE_COLUMNS; i++, column++)
        {
            if (frame == i * 4 + 5)
                state->shakeFrame = 2;

            // Each vine grows, then shrinks back.
            offset = i * 2;
            if (frame > offset + 4)
            {
                picture = (frame / 4 + i) % GREEN_VINE_PICTURES;
                if (frame < offset + 32)
                {
                    height = (frame - offset) * 4 - 16;
                    if (height > GREEN_VINE_HEIGHT)
                        height = GREEN_VINE_HEIGHT;
                }
                else
                {
                    height = 160 - (frame - offset) * 4;
                }
                if (height > 0)
                    draw[i & 1](canvas,
                                state->sheet + picture * PICTURE_BYTES(GREEN_VINE),
                                column->x - GREEN_VINE_WIDTH / 2,
                                (i & 7) - height + 104,
                                GREEN_VINE_WIDTH,
                                height);
            }
        }
        btCameraShake(4, 4);
        EFFECT_END_TICK(state);
    }
    EFFECT_FINISH_CANVAS();
}

// The ticks from one thorn's start to the next's.
#define THORN_INTERVAL             8
// ThornSheet: the big thorn, then the small one.
#define THORN_BIG_WIDTH            48
#define THORN_BIG_HEIGHT           80
#define THORN_SMALL_PICTURE_WIDTH  32
#define THORN_SMALL_PICTURE_HEIGHT 64
#define THORN_SMALL_PICTURE        PICTURE_BYTES(THORN_BIG)
// The pixels a big and a small thorn lean sideways at most as they grow.
#define THORN_BIG_LEAN             30
#define THORN_SMALL_LEAN           8

// Thorns burst from the ground one after another, flaring at their roots.
//
// Abilities: Thorn, Briar, Nettle.
void btEffectRunThorn(BATTLE_EFFECT_PARM *args)
{
    void                *canvas;
    Sint32               frame;
    DRAW_RECTANGLE       drawFlip;
    DRAW_RECTANGLE       draw;
    Sint32               frameCount;
    HEAP_SLOTS          *heap;
    BATTLE_EFFECT_STATE *state;
    Sint32               i;
    Sint32               j;
    Sint32               age;
    Sint32               start;

    // FAKEMATCH(dead): heap set through its own address; as heap =
    // * RamHeapSlots.
    *&heap = RamHeapSlots;
    state  = RamHeapSlots->battleEffect;
    canvas = RamHeapSlots->battleCanvas;
    EFFECT_START_NARROW_CANVAS(BGCNT_PRIORITY(1));
    REG_BLDCNT   = 0;
    REG_BLDALPHA = EFFECT_BLEND_ADD;
    btResourceLoadAndDecompress(RESOURCE_THORN_SHEET, state->sheet, 1, 1);
    if (args->side == EFFECT_SIDE_PARTY)
    {
        REG_BG2X = -(NARROW_CANVAS_PARTY_X << 8);
    }
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_TRANSPARENT);
    draw = heap->ramCode;
    EFFECT_LOAD_BLITTER(
        HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_TRANSPARENT);
    drawFlip = heap->ramCodePaired;
    EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
    frameCount = ThornCounts[args->variant] * THORN_INTERVAL + 56;

    EFFECT_FREE_PARTICLES(i, PARTICLES, PARTICLE_POOL_COUNT);

    for (frame = 0; frame != frameCount; frame++)
    {
        if (frame == frameCount - 64)
        {
            EFFECT_SHOW_DAMAGE(SE_IMPACT);
        }
        if (frame >= frameCount - 16)
        {
            // Fade out over the last 16 ticks.
            REG_BLDCNT   = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
            REG_BLDALPHA = EFFECT_BLEND_CANVAS(frameCount - frame - 1);
        }

        for (i = 0; i != ThornCounts[args->variant]; i++)
        {
            start = i * THORN_INTERVAL + 8;
            age   = frame - start;
            if (frame > start)
            {
                Sint32 height;
                Sint32 lean;

                // A thorn leans sideways as it grows.
                if (ThornKinds[i] < THORN_SMALL)
                {
                    height = age * 16;
                    lean   = (age * 2 + age) * 2;
                    if (height > THORN_BIG_HEIGHT)
                    {
                        height = THORN_BIG_HEIGHT;
                    }
                    if (lean > THORN_BIG_LEAN)
                    {
                        lean = THORN_BIG_LEAN;
                    }
                    if (ThornKinds[i] & THORN_MIRRORED)
                    {
                        drawFlip(canvas,
                                 state->sheet,
                                 ThornX[i] - lean,
                                 108 - height,
                                 THORN_BIG_WIDTH,
                                 height);
                    }
                    else
                    {
                        draw(canvas,
                             state->sheet,
                             ThornX[i] + lean,
                             108 - height,
                             THORN_BIG_WIDTH,
                             height);
                    }
                }
                else
                {
                    height = age * 8;
                    lean   = age;
                    if (height > THORN_SMALL_PICTURE_HEIGHT)
                    {
                        height = THORN_SMALL_PICTURE_HEIGHT;
                    }
                    if (age > THORN_SMALL_LEAN)
                    {
                        lean = THORN_SMALL_LEAN;
                    }
                    if (ThornKinds[i] & THORN_MIRRORED)
                    {
                        drawFlip(canvas,
                                 state->sheet + THORN_SMALL_PICTURE,
                                 ThornX[i] - lean,
                                 108 - height,
                                 THORN_SMALL_PICTURE_WIDTH,
                                 height);
                    }
                    else
                    {
                        draw(canvas,
                             state->sheet + THORN_SMALL_PICTURE,
                             ThornX[i] + lean,
                             108 - height,
                             THORN_SMALL_PICTURE_WIDTH,
                             height);
                    }
                }
                if (frame == start + 1)
                {
                    state->shakeFrame = 3;
                }

                // Impact flares beside the thorn.
                if (frame < start + 3)
                {
                    Sint32    y = (syRandom() & 31) + 72;
                    PARTICLE *flare;

                    for (j = 0; j != THORN_FLARES; j++)
                    {
                        flare = &PARTICLES[j];
                        if (flare->variant == -1)
                        {
                            flare->x = ThornX[i] + (syRandom() & 31) + 32;
                            if (flare->x > 96)
                            {
                                flare->x = 96;
                            }
                            flare->y       = y;
                            flare->variant = 0;
                            break;
                        }
                    }
                }
            }

            // The targets react as the thorn reaches them.
            for (j = 0; j != args->targetCount; j++)
            {
                if (frame == start + 4)
                {
                    sySoundPlayStub(SE_IMPACT);
                    btEffectTintBattler(args->targets[j], SPRITE_COLOR_WHITE, POSE_DAMAGE, j, 3);
                }
            }
        }

        // The flares, over the thorns, each beside its mirror image.
        for (i = 0; i != THORN_FLARES; i++)
        {
            if (PARTICLES[i].variant >= 0)
            {
                Sint32 picture = PARTICLES[i].variant / 2;

                draw(canvas,
                     state->sheet + ThornPuffPictures[picture],
                     PARTICLES[i].x - ThornPuffWidths[picture],
                     PARTICLES[i].y - (Sint8)ThornPuffHeights[picture] / 2,
                     ThornPuffWidths[picture],
                     (Sint8)ThornPuffHeights[picture]);
                drawFlip(canvas,
                         state->sheet + ThornPuffPictures[picture],
                         PARTICLES[i].x,
                         PARTICLES[i].y - (Sint8)ThornPuffHeights[picture] / 2,
                         ThornPuffWidths[picture],
                         (Sint8)ThornPuffHeights[picture]);
                if (++PARTICLES[i].variant == GROWTH_FLARE_TICKS)
                {
                    PARTICLES[i].variant = -1;
                }
            }
        }
        btCameraShake(8, 8);
        EFFECT_END_TICK(state);
    }

    EFFECT_FINISH_CANVAS();
}

#undef GROWTH_FLARES
#undef THORN_FLARES
#undef GROWTH_FLARE_TICKS
#undef GROWING_COLUMNS
#undef COLUMN_INTERVAL
#undef GROWING_COLUMNS_VINE_FLARE_PICTURES
#undef GROWING_COLUMNS_PILLAR_FLARE_PICTURES
#undef VINE_COLUMNS
#undef VINE_TICKS
#undef VINE_REACH_TICK
#undef GREEN_VINE_WIDTH
#undef GREEN_VINE_HEIGHT
#undef GREEN_VINE_PICTURES
#undef THORN_INTERVAL
#undef THORN_BIG_WIDTH
#undef THORN_BIG_HEIGHT
#undef THORN_SMALL_PICTURE_WIDTH
#undef THORN_SMALL_PICTURE_HEIGHT
#undef THORN_SMALL_PICTURE
#undef THORN_BIG_LEAN
#undef THORN_SMALL_LEAN

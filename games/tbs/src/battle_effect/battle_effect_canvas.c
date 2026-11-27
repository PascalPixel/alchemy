// The canvas layer: opening and closing it, its transfers, the battle background's darkening behind
// it, the targets' flash timers, and lines. Part of battle_effect.c.

// How far a row of btEffectWipeCanvas's wipe lags behind its front, at most: each row lags a random
// 0 to WIPE_LAG_MAX pixels.
#define WIPE_LAG_MAX    63
// Where the across wipe's front stops, three ticks past

// CANVAS_SIZE + WIPE_LAG_MAX, where the wipe is finished.
#define WIPE_ACROSS_END 256

// Wipes the whole canvas to one colour with a ragged edge, a tick at a time: 23 ticks across, 15
// down.
void btEffectWipeCanvas(Sint32 direction, Sint32 fill)
{
    BATTLE_EFFECT_STATE *state;
    Uint8               *canvas;
    Uint8                delay[CANVAS_SIZE];
    Sint32               x;
    Sint32               y;
    Sint32               wipePos;

    canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
    state  = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
    for (x = 0; x != CANVAS_SIZE; x++)
        delay[x] = syRandom() & WIPE_LAG_MAX;

    if (direction == CANVAS_WIPE_ACROSS)
    {
        Sint32 front;
        Sint32 speed;

        front = 0;
        speed = 1;
        x     = 0;
        do
        {
            front += speed;
            speed++;
            for (; x != front; x++)
            {
                for (y = 0; y != CANVAS_SIZE; y++)
                {
                    wipePos = x - delay[y];
                    if (wipePos >= 0)
                        if (wipePos < CANVAS_SIZE)
                            canvas[CANVAS_PIXEL_OFFSET(wipePos, y)] = 1 - fill;
                }
            }
            EFFECT_PRESENT_CANVAS(state);
        } while (front <= WIPE_ACROSS_END);
    }
    else
    {
        Sint32 front;
        Sint32 speed;

        front = 0;
        speed = 1;
        y     = 0;
        do
        {
            front += speed / 2;
            speed += 4;
            for (; y != front; y++)
            {
                for (x = 0; x != CANVAS_SIZE; x++)
                {
                    wipePos = y - delay[x];
                    if (wipePos >= 0)
                        if (wipePos < CANVAS_SIZE)
                            canvas[CANVAS_PIXEL_OFFSET(x, wipePos)] = 1 - fill;
                }
            }
            EFFECT_PRESENT_CANVAS(state);
        } while (front <= CANVAS_SIZE + WIPE_LAG_MAX);
    }
}

// Copies the canvas to VRAM once the effect has finished a tick, then clears, fades, darkens or
// brightens it as transferMode says.
void btEffectTransferCanvas(void)
{
    BATTLE_EFFECT_STATE *state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
    void                *src;
    Sint32              *counter;
    Sint32               nextCounter;

    if (state->transferPending == 1)
    {
        src = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
        switch (state->transferMode)
        {
            case CANVAS_COPY: syWordsCopy(BG_CHAR_BLOCK(1), src, CANVAS_BYTES); break;
            case CANVAS_COPY_AND_CLEAR:
                syWordsCopy(BG_CHAR_BLOCK(1), src, CANVAS_BYTES);
                syWordsFill(src, CANVAS_BYTES, state->transferValue);
                break;
            case CANVAS_COPY_AND_FADE:
                if (state->transferValue == CANVAS_FADE_HALF)
                {
                    syCanvasCopyAndHalve(src, (void *)BG_CHAR_BLOCK(1), CANVAS_BYTES);
                }
                else
                {
                    syCanvasCopyAndScaleThreeQuarters(src, (void *)BG_CHAR_BLOCK(1), CANVAS_BYTES);
                }
                break;
            case CANVAS_COPY_AND_DARKEN:
                syCanvasCopyAndDarken(
                    src, state->transferValue, (void *)BG_CHAR_BLOCK(1), CANVAS_BYTES);
                break;
            case CANVAS_COPY_AND_BRIGHTEN:
                syCanvasCopyAndBrighten(
                    src, state->transferValue, (void *)BG_CHAR_BLOCK(1), CANVAS_BYTES);
                break;
        }
        state->transferPending = 0;
        counter                = &state->framesSinceTransfer;
        nextCounter            = 1;
    }
    else
    {
        counter     = &state->framesSinceTransfer;
        nextCounter = *counter + 1;
    }
    *counter = nextCounter;
}

// Copies a full-screen image to VRAM once the effect has finished a tick, as transferMode says.
void btEffectTransferFullScreenImage(void)
{
    BATTLE_EFFECT_STATE *state;
    void                *src;
    Sint32               transferMode;

    state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
    src   = RamMapGridMetatileBuffer;
    if (state->transferPending != 1)
        return;

    transferMode = state->transferMode;
    switch (transferMode)
    {
        case CANVAS_COPY: syWordsCopy(BG_CHAR_BLOCK(2), src, BATTLE_BACKGROUND_BYTES); break;
        case CANVAS_COPY_AND_CLEAR:
            syWordsCopy(BG_CHAR_BLOCK(2), src, BATTLE_BACKGROUND_BYTES);
            syWordsFill(src, BATTLE_BACKGROUND_BYTES, state->transferValue);
            break;
        case CANVAS_COPY_AND_FADE:
            if (state->transferValue == CANVAS_FADE_HALF)
            {
                syCanvasCopyAndHalve(src, (void *)BG_CHAR_BLOCK(2), BATTLE_BACKGROUND_BYTES);
            }
            else
            {
                syCanvasCopyAndScaleThreeQuarters(
                    src, (void *)BG_CHAR_BLOCK(2), BATTLE_BACKGROUND_BYTES);
            }
            break;
        case CANVAS_COPY_AND_DARKEN:
            syCanvasCopyAndDarken(
                src, state->transferValue, (void *)BG_CHAR_BLOCK(2), BATTLE_BACKGROUND_BYTES);
            break;
    }

    state->transferPending = 0;
}

// The battle background's first BG palette, of the eight its colours take.
#define BATTLE_BACKGROUND_PALETTE 6

// Writes the window, display and blend registers the effect keeps in its state, all at once.
void btEffectApplyWindowBlendRegisters(void)
{
    BATTLE_EFFECT_STATE *state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);

    REG_WIN0H                  = state->win0h;
    REG_WIN0V                  = state->win0v;
    REG_WIN1H                  = state->win1h;
    REG_WIN1V                  = state->win1v;
    REG_WININ                  = state->winin;
    REG_WINOUT                 = state->winout;
    REG_DISPCNT                = state->dispcnt;
    REG_BLDCNT                 = state->bldcnt;
    REG_BLDALPHA               = state->bldalpha;
}

// Writes the effect's BG2 reference point to BG2X and BG2Y.
void btEffectApplyBg2Reference(void)
{
    BATTLE_EFFECT_STATE *state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);

    REG_BG2X                   = state->bg2x;
    REG_BG2Y                   = state->bg2y;
}

// Darkens the battle background a step each tick while the canvas is open, so the effect stands
// out.
void btEffectDarkenBackgroundStep(void)
{
    BATTLE_STATE        *battle = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE);
    BATTLE_EFFECT_STATE *state  = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);

    if (state->fadeFrame > 0)
    {
        Sint32 step = ++state->fadeStep;

        btGraphicsScaleRGB555ClampedStub(battle->palette,
                                         (void *)&BG_PLTT_COLOR(BATTLE_BACKGROUND_PALETTE, 0),
                                         toFIXED(1) - step * BACKGROUND_LEVEL_STEP,
                                         BATTLE_BACKGROUND_COLORS);
        state->fadeFrame--;
    }
}

// Clears every target's flash timer.
void btEffectClearTintTimers(void)
{
    BATTLE_EFFECT_STATE *state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);

    syWordsClear(state->actorTimers, sizeof state->actorTimers);
}

// Counts down each target's flash timer and gives it its own palette back when one runs out.
void btEffectCountDownBattlerTints(void)
{
    BATTLE_EFFECT_STATE *state;
    Sint32               actorIndex;

    state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
    for (actorIndex = 0; actorIndex != ARRAY_COUNT(state->actorTimers); actorIndex++)
    {
        if (state->actorTimers[actorIndex] != 0)
        {
            if ((state->actorTimers[actorIndex] = state->actorTimers[actorIndex] - 1) == 0)
            {
                btEffectTintBattler(
                    state->args->targets[actorIndex], SPRITE_COLOR_NORMAL, TINT_KEEP, TINT_KEEP, 0);
            }
        }
    }
}

// Opens the canvas layer over the battle, darkening the battle background behind it.
void btEffectStartCanvasLayer(Sint32 bgControl)
{
    BATTLE_EFFECT_STATE     *state   = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
    BATTLE_STATE            *battle  = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE);
    void                    *canvas  = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
    BATTLE_BACKGROUND_STATE *display = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_BACKGROUND);
    Sint32                   row;
    Sint32                   column;

    btEffectClearTintTimers();
    display->secondMode = BATTLE_BACKGROUND_FIXED;
    sySynch(1);
    REG_BLDCNT = 0;
    IO_WRITE_QUEUE_PUSH((Uint32)&REG_DISPCNT, EFFECT_DISPCNT, IO_WRITE_HALFWORD);
    BgScrollWork[1].y = BATTLE_BACKGROUND_SCROLL_Y;
    sySynch(1);

    btBackgroundSetModeStub(BATTLE_BACKGROUND_FIXED, battle->bg, 0);
    state->fadeFrame = 0;
    state->fadeStep  = 0;
    sySetEvent(btEffectDarkenBackgroundStep, EVENT_PASS_TICK);

    // Change BG2's settings while it is hidden.
    IO_WRITE_QUEUE_PUSH((Uint32)&REG_DISPCNT,
                        DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG1_ON
                            | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON | DISPCNT_WIN1_ON,
                        IO_WRITE_HALFWORD);
    sySynch(1);
    REG_BG2CNT = bgControl | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
    IO_WRITE_QUEUE_PUSH((Uint32)&REG_DISPCNT,
                        DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG1_ON
                            | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON | DISPCNT_WIN1_ON,
                        IO_WRITE_HALFWORD);
    btCameraSetViewStub(0, 0, 0, BATTLE_ZOOM_NORMAL);
    display->secondMode = BATTLE_BACKGROUND_NONE;
    sySynch(1);
    REG_BLDCNT   = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
    REG_BLDALPHA = EFFECT_BLEND_CANVAS(CANVAS_BLEND_LEVEL);
    EFFECT_SET_CANVAS_AFFINE();
    EFFECT_SET_BATTLE_WINDOWS();
    REG_WININ =
        EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG2 | WININ_WIN1_OBJ | WININ_WIN1_CLR;
    REG_WINOUT = EFFECT_WINOUT;
    IO_WRITE_QUEUE_PUSH((Uint32)&REG_DISPCNT, EFFECT_DISPCNT, IO_WRITE_HALFWORD);

    {
        Sint32 offset = 0;

        for (row = 0; row != CANVAS_TILES; row++)
        {
            EFFECT_WRITE_CANVAS_MAP_ROW(row, column, offset);
        }

        syWordsClear(canvas, CANVAS_BYTES);
        syWordsClear(BG_CHAR_BLOCK(1), CANVAS_BYTES);
        state->shakeFrame = 0;
        state->savedBg1X  = BgScrollWork[1].x;
        state->savedBg1Y  = BgScrollWork[1].y;
        sySynch(1);
    }
}

// Opens the canvas layer as btEffectStartCanvasLayer does, with the windows off and a half-faded
// background.
void btEffectOpenCanvasLayer(Sint32 bgControl)
{
    BATTLE_EFFECT_STATE     *state   = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
    BATTLE_STATE            *battle  = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE);
    void                    *canvas  = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
    BATTLE_BACKGROUND_STATE *display = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_BACKGROUND);
    Sint32                   row;
    Sint32                   column;

    btEffectClearTintTimers();
    display->secondMode = BATTLE_BACKGROUND_FIXED;
    sySynch(1);
    REG_BLDCNT = 0;
    IO_WRITE_QUEUE_PUSH((Uint32)&REG_DISPCNT,
                        DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG1_ON
                            | DISPCNT_BG2_ON | DISPCNT_OBJ_ON,
                        IO_WRITE_HALFWORD);
    BgScrollWork[1].y = BATTLE_BACKGROUND_SCROLL_Y;
    sySynch(1);
    btBackgroundSetModeStub(BATTLE_BACKGROUND_FIXED, battle->bg, BATTLE_BACKGROUND_FADE_RESTORE);
    state->fadeFrame = BACKGROUND_DARKEN_TICKS;
    state->fadeStep  = 0;
    sySetEvent(btEffectDarkenBackgroundStep, EVENT_PASS_TICK);
    IO_WRITE_QUEUE_PUSH((Uint32)&REG_DISPCNT,
                        DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG1_ON
                            | DISPCNT_OBJ_ON,
                        IO_WRITE_HALFWORD);
    sySynch(1);
    REG_BG2CNT = bgControl | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
    IO_WRITE_QUEUE_PUSH((Uint32)&REG_DISPCNT,
                        DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG1_ON
                            | DISPCNT_OBJ_ON,
                        IO_WRITE_HALFWORD);
    btCameraSetViewStub(0, 0, 0, BATTLE_ZOOM_NORMAL);
    display->secondMode = BATTLE_BACKGROUND_NONE;
    sySynch(1);

    REG_BLDCNT   = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
    REG_BLDALPHA = EFFECT_BLEND_CANVAS(CANVAS_BLEND_LEVEL);
    EFFECT_SET_CANVAS_AFFINE();
    EFFECT_SET_BATTLE_WINDOWS();
    REG_WININ =
        EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG2 | WININ_WIN1_OBJ | WININ_WIN1_CLR;
    REG_WINOUT = EFFECT_WINOUT;
    IO_WRITE_QUEUE_PUSH((Uint32)&REG_DISPCNT, EFFECT_DISPCNT, IO_WRITE_HALFWORD);
    sySynch(1);

    {
        Sint32 offset = 0;

        for (row = 0; row != CANVAS_TILES; row++)
        {
            EFFECT_WRITE_CANVAS_MAP_ROW(row, column, offset);
        }

        syWordsClear(canvas, CANVAS_BYTES);
        syWordsClear(BG_CHAR_BLOCK(1), CANVAS_BYTES);
        sySynch(1);
    }
}

// Opens the canvas layer on a 256-pixel map that wraps: the canvas fills the left half of the map's
// top rows, and the right half repeats the canvas's first tile.
void btEffectStartTiledCanvas(Sint32 bgControl)
{
    Sint32 row;
    Sint32 column;
    Sint32 offset;

    btEffectStartCanvasLayer(bgControl);
    REG_BG2CNT = bgControl | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7)
               | BGCNT_WRAP | BGCNT_SIZE(1);
    offset = 0;
    for (row = 0; row != CANVAS_TILES; row++)
    {
        EFFECT_WRITE_CANVAS_MAP_ROW(row, column, offset);
        for (column = 0; column != 8; column++, offset += 2)
        {
            *(volatile Uint16 *)((Uint8 *)BG_CANVAS_MAP + offset) = 0;
        }
    }
}

// The levels the battle background brightens by each tick as the canvas closes, back to its own
// colours.
#define BACKGROUND_RESTORE_STEP 3

// Closes the canvas layer and brings the battle background's colours back.
void btEffectEndCanvasLayer(void)
{
    BATTLE_EFFECT_STATE *state  = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
    BATTLE_STATE        *battle = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE);
    Sint32               i;

    sySoundPlayStub(SOUND_FADE_OUT_ACTION_SE);
    BgScrollWork[1].x      = state->savedBg1X;
    BgScrollWork[1].y      = state->savedBg1Y;
    ProjectionWork.centerX = BATTLE_PROJECTION_CENTER_X;
    ProjectionWork.centerY = BATTLE_PROJECTION_CENTER_Y;
    REG_BG2CNT = BGCNT_PRIORITY(3) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
    syWordsClear(BG_CHAR_BLOCK(1), CANVAS_BYTES);
    syCloseEvent(btEffectDarkenBackgroundStep);
    // This overwrites the saved y restored above, so BG1 is always left at
    // BATTLE_BACKGROUND_SCROLL_Y.
    BgScrollWork[1].y = BATTLE_BACKGROUND_SCROLL_Y;
    IO_WRITE_QUEUE_PUSH((Uint32)&REG_DISPCNT,
                        DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG1_ON
                            | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON | DISPCNT_WIN1_ON,
                        IO_WRITE_HALFWORD);
    REG_BLDCNT = 0;
    sySynch(1);

    btBackgroundSetModeStub(BATTLE_BACKGROUND_ZOOMED, battle->bg, 7);
    sySynch(1);
    // The background brightens from the darkened level back to its own colours over eight ticks.
    for (i = 0; i != BACKGROUND_DARKENED_LEVEL / BACKGROUND_RESTORE_STEP; i++)
    {
        btBackgroundSetPaletteLevelStub(battle->bg,
                                        BACKGROUND_DARKENED_LEVEL - BACKGROUND_RESTORE_STEP
                                            - i * BACKGROUND_RESTORE_STEP);
        sySynch(1);
    }

    IO_WRITE_QUEUE_PUSH((Uint32)&REG_DISPCNT,
                        DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG2_ON
                            | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON | DISPCNT_WIN1_ON,
                        IO_WRITE_HALFWORD);
    sySynch(1);
}

// Hides BG1 and the windows and starts bringing the battle background's colours back.
void btEffectHideBg1AndRestoreBackdrop(void)
{
    BATTLE_STATE *battle;
    Sint32       *secondMode;

    secondMode =
        &((BATTLE_BACKGROUND_STATE *)HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_BACKGROUND))->secondMode;
    battle      = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE);
    *secondMode = BATTLE_BACKGROUND_FIXED;
    syIOWriteQueueStore16((Uint32)&REG_DISPCNT,
                          DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG2_ON
                              | DISPCNT_OBJ_ON);
    sySynch(1);
    btBackgroundSetModeStub(BATTLE_BACKGROUND_ZOOMED, battle->bg, 0);
    sySynch(1);
}

// BG1's map while it shows the canvas at its own size: the battle's own BG1 map, in screen
// block 31.
#define CANVAS_BG1_SCREEN     31
// Row `row` of BG1's map.
#define BG1_MAP_ROW(row)      (BG_SCREEN_BLOCK(CANVAS_BG1_SCREEN) + (row) * MAP_ROW_BYTES)
// The tile, cleared, that BG1 shows around the canvas.
#define CANVAS_BG1_BLANK_TILE 255
// The canvas's first tile counted from BG1's character base: the canvas is character block 1.
#define CANVAS_BG1_FIRST_TILE (BG_CHAR_SIZE / TILE_SIZE_8BPP)
// The map row where the canvas starts on BG1, 96 pixels down: the beams scroll BG1 to y 80 to show
// it from the battle view's top line.
#define CANVAS_BG1_TOP_ROW    12

// Shows the canvas on BG1 instead of BG2, at its own size.
void btEffectSetupCanvasTileMap(void)
{
    // UB: indexing past BattleStateWork reaches the heap slots after it.
#if FIX_POINTER_OUTSIDE_OBJECT
    void                    *canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
    BATTLE_STATE            *screen = BattleStateWork;
    BATTLE_BACKGROUND_STATE *layer  = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_BACKGROUND);
#else
    Uint8                  **cells  = (Uint8 **)&BattleStateWork;
    void                    *canvas = cells[HEAP_SLOT_BATTLE_CANVAS - HEAP_SLOT_BATTLE];
    BATTLE_STATE            *screen = (BATTLE_STATE *)cells[0];
    BATTLE_BACKGROUND_STATE *layer =
        (BATTLE_BACKGROUND_STATE *)cells[HEAP_SLOT_BATTLE_BACKGROUND - HEAP_SLOT_BATTLE];
#endif
    Sint32 row;
    Sint32 column;
    Sint32 byteOffset;

    btEffectClearTintTimers();
    btBackgroundSetModeStub(BATTLE_BACKGROUND_ZOOMED, screen->bg, 0);
    BgScrollWork[1].y = BATTLE_BACKGROUND_SCROLL_Y;
    layer->secondMode = BATTLE_BACKGROUND_NONE;
    // A blank tile, then the rows above the canvas, from the first one BG1 shows, which show
    // nothing.
    syWordsClear(BG_CHAR_BLOCK(0) + CANVAS_BG1_BLANK_TILE * TILE_SIZE_8BPP, TILE_SIZE_8BPP);
    syWordsFill(BG1_MAP_ROW(BATTLE_BACKGROUND_SCROLL_Y / 8),
                (CANVAS_BG1_TOP_ROW - BATTLE_BACKGROUND_SCROLL_Y / 8) * MAP_ROW_BYTES,
                -1);
    // The canvas's tiles, then the blank tile across the rest of each row.
    byteOffset = 0;
    for (row = 0; row != CANVAS_TILES; row++)
    {
        for (column = 0; column != BG_SCREEN_WIDTH; column++, byteOffset += 2)
        {
            if (column >= CANVAS_TILES)
                *(volatile Uint16 *)(BG1_MAP_ROW(CANVAS_BG1_TOP_ROW) + byteOffset) =
                    CANVAS_BG1_BLANK_TILE;
            else
                *(volatile Uint16 *)(BG1_MAP_ROW(CANVAS_BG1_TOP_ROW) + byteOffset) =
                    row * CANVAS_TILES + CANVAS_BG1_FIRST_TILE + column;
        }
    }
    REG_DISPCNT = EFFECT_DISPCNT;
    REG_BG1CNT  = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(0) | BGCNT_256_COLORS
               | BGCNT_SCREEN_BASE(CANVAS_BG1_SCREEN);
    REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
    EFFECT_SET_BATTLE_WINDOWS();
    REG_WININ =
        EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG2 | WININ_WIN1_OBJ | WININ_WIN1_CLR;
    REG_WINOUT   = EFFECT_WINOUT;
    REG_BLDALPHA = EFFECT_BLEND_CANVAS(CANVAS_BLEND_LEVEL);
    syWordsClear(canvas, CANVAS_BYTES);
    syWordsClear(BG_CHAR_BLOCK(1), CANVAS_BYTES);
    sySynch(1);
}

// Draws a line on the canvas, keeping the brighter colour where it crosses what is drawn; the
// line's ends are clipped to the canvas's rows only.
void btEffectDrawClippedCanvasLine(Sint32 x0, Sint32 y0, Sint32 x1, Sint32 y1, Sint32 color)
{
    Sint32 dx     = x1 - x0;
    Sint32 dy     = y1 - y0;
    // The slower axis is followed in 8.8 fixed point, a pixel each time the fraction carries into
    // the whole part.
    Sint32 frac   = Q_8_8(0.5);
    Uint8 *canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
    Sint32 step;
    Sint32 i;
    Sint32 x;
    Sint32 y;
    Sint32 tmp;
    Uint32 offset;

    CLAMP(y0, 0, CANVAS_SIZE - 1);
    CLAMP(y1, 0, CANVAS_SIZE - 1);

    if (ABS(dx) < ABS(dy))
    {
        if (dy < 0)
        {
            tmp = x0;
            x0  = x1;
            x1  = tmp;
            tmp = y0;
            y0  = y1;
            y1  = tmp;
            dx  = x1 - x0;
            // LEFTOVER(template): dy recomputed after the swap and never read
            // * again, as the shallow arm's swap below recomputes both deltas and
            // * reads dy for its y step; as the line removed.
            dy  = y1 - y0;
        }
        step = ABS(x1 - x0) * Q_8_8(1) / ABS(y1 - y0);
        x    = x0;
        for (i = y0; i != y1; i++)
        {
            offset = CANVAS_PIXEL_OFFSET((Uint32)x, (Uint32)i);
            if (canvas[offset] < color)
                canvas[offset] = color;
            frac += step;
            if (frac & Q_8_8(1))
            {
                if (dx > 0)
                    x++;
                else
                    x--;
                frac &= ~Q_8_8(1);
            }
        }
    }
    else
    {
        if (dx < 0)
        {
            tmp = x0;
            x0  = x1;
            x1  = tmp;
            tmp = y0;
            y0  = y1;
            y1  = tmp;
            // LEFTOVER(template): dx recomputed after the swap and never read
            // * again, as the steep arm's swap above recomputes both deltas and
            // * reads dx for its x step; as the line removed.
            dx  = x1 - x0;
            dy  = y1 - y0;
        }
        step = ABS(y1 - y0) * Q_8_8(1) / ABS(x1 - x0);
        y    = y0;
        for (i = x0; i != x1; i++)
        {
            offset = CANVAS_PIXEL_OFFSET((Uint32)i, (Uint32)y);
            if (canvas[offset] < color)
                canvas[offset] = color;
            frac += step;
            if (frac & Q_8_8(1))
            {
                if (dy > 0)
                    y++;
                else
                    y--;
                frac &= ~Q_8_8(1);
            }
        }
    }
}

#undef WIPE_LAG_MAX
#undef WIPE_ACROSS_END
#undef BATTLE_BACKGROUND_PALETTE
#undef BACKGROUND_RESTORE_STEP
#undef CANVAS_BG1_SCREEN
#undef BG1_MAP_ROW
#undef CANVAS_BG1_BLANK_TILE
#undef CANVAS_BG1_FIRST_TILE
#undef CANVAS_BG1_TOP_ROW

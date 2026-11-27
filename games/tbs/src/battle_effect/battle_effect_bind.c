// Bind and Triangle Strike. Part of battle_effect.c.

// Bind's sprite sheet: the rune, the flare pictures, then the Djinni.
#define BIND_FLARE_PICTURES        4096
#define BIND_DJINNI_PICTURE        8192
// The particles each target's triangle keeps, and the flares along each edge.
#define BIND_SLOTS_PER_TARGET      32
#define BIND_EDGE_FLARES           24
// The ticks the triangle of flares shrinks and turns before the rune shows, with the damage.
#define BIND_RUNE_TICK             64
// The Djinni of Bind's prelude sways this far either way, a sway every 32 ticks.
#define BIND_DJINNI_SWAY           20
// The ticks Triangle Strike's user takes to leap at the target.
#define TRIANGLE_STRIKE_LEAP_TICKS 16

// The blue ramp: one colour for each blue level from BG palette BLUE_RAMP_PALETTE (later from
// BLUE_RAMP_SECOND_PALETTE), each colour's red and green half its blue. A pixel takes the colour
// for RGB_LEVEL_MAX less its brightest channel, so the background turns blue with its light and
// dark swapped.
#define BLUE_RAMP_PALETTE          2
#define BLUE_RAMP_SECOND_PALETTE   6
#define BLUE_RAMP_COLORS           (RGB_LEVEL_MAX + 1)
// The ramp's edge moves down a quarter of its speed in rows a tick, from this speed and a row
// faster every four ticks, for at most BLUE_RAMP_TICKS or until it is past BLUE_RAMP_LAST_ROW.
#define BLUE_RAMP_START_SPEED      16
#define BLUE_RAMP_TICKS            27
#define BLUE_RAMP_LAST_ROW         248

// Turns the screen into shades of blue, top down, with a ragged edge; nothing calls it.
UNUSED void btEffectConvertBackgroundToBlueRamp(void)
{
    Sint32  i, x, y, row, end, speed;
    Sint32  red, green, blue;
    Uint16  color;
    Uint8  *delays, *pixel;
    Uint16 *palette;
    palette = (Uint16 *)&BG_PLTT_COLOR(BLUE_RAMP_PALETTE, 0);
    for (i = 0; i != BLUE_RAMP_COLORS; i++)
    {
        Sint32 half;
        half       = i / 2;
        *palette++ = RGB(half, half, i);
    }
    delays = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
    end    = 0;
    speed  = BLUE_RAMP_START_SPEED;
    syWordsCopy(BLUE_RAMP_PIXELS, BG_CHAR_BLOCK(2), BATTLE_BACKGROUND_BYTES);
    for (x = 0; x != BATTLE_BACKGROUND_WIDTH; x++)
    {
        delays[x] = syRandom() & 63;
    }
    row = 0;
    for (i = 0; i != BLUE_RAMP_TICKS; i++)
    {
        end += speed / 4;
        speed++;
        for (; row != end; row++)
        {
            for (x = 0; x != BATTLE_BACKGROUND_WIDTH; x++)
            {
                y = row - delays[x];
                if (y >= 0)
                {
                    if (y < BATTLE_BACKGROUND_HEIGHT)
                    {
                        pixel = BLUE_RAMP_PIXELS + BACKGROUND_PIXEL_OFFSET(x, y);
                        color = ((Sint16 *)BG_PLTT)[*pixel];
                        red   = RGB_RED(color);
                        green = RGB_GREEN(color);
                        blue  = RGB_BLUE(color);
                        if (red < green)
                            red = green;
                        if (red < blue)
                            red = blue;
                        *pixel = BLUE_RAMP_PALETTE * PALETTE_COLORS + RGB_LEVEL_MAX - red;
                    }
                }
            }
        }
        syWordsCopy(BG_CHAR_BLOCK(2), BLUE_RAMP_PIXELS, BATTLE_BACKGROUND_BYTES);
        sySynch(1);
        if (end > BLUE_RAMP_LAST_ROW)
            break;
    }
    // Then onto a second blue ramp.
    palette = (Uint16 *)&BG_PLTT_COLOR(BLUE_RAMP_SECOND_PALETTE, 0);
    for (i = 0; i != BLUE_RAMP_COLORS; i++)
    {
        Sint32 half;
        half       = i / 2;
        *palette++ = RGB(half, half, i);
    }
    for (i = 0; i != BATTLE_BACKGROUND_BYTES; i++)
    {
        BLUE_RAMP_PIXELS[i] += (BLUE_RAMP_SECOND_PALETTE - BLUE_RAMP_PALETTE) * PALETTE_COLORS;
    }
    syWordsCopy(BG_CHAR_BLOCK(2), BLUE_RAMP_PIXELS, BATTLE_BACKGROUND_BYTES);
    sySynch(1);
}

// A triangle of light spins and shrinks onto the first target, changing colour and leaving a fading
// trail, then a rune stands on the target. The triangle's corners are projected onto the canvas and
// each edge is stamped with flares.
//
// Abilities: Bind; Luff.
void btEffectRunBind(BATTLE_EFFECT_PARM *args)
{
    SCREEN_POINT         anchor;
    // The target's position, then each corner projected around it.
    SCREEN_POINT         corner;
    SCREEN_POINT         screen;
    VECTOR3              scale;
    Sint32               originX;
    Sint32               originY;
    BLITTER_PAIR         draw;
    BATTLE_EFFECT_STATE *state;
    void                *canvas;
    Sint32               frame;
    Sint32               member;
    BATTLE_CAMERA       *camera;
    Sint32               shift;
    Uint8               *graphics;
    Uint8               *palette;
    ACTOR               *actor;
    Sint32               memberFrame;
    PARTICLE            *point;
    Sint32               size;
    Sint32               j;
    Sint32               k;

    state       = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
    canvas      = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
    graphics    = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
    camera      = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
    state->args = args;
    btEffectStartCanvasLayer(BGCNT_PRIORITY(1));
    if (state->args->prelude == 1)
        btEffectRunDjinniEntrance(args,
                                  ELEMENT_JUPITER,
                                  state->args->side,
                                  DJINNI_ANCHOR_OVER_TARGET,
                                  &originX,
                                  &originY);
    REG_BG2PA = Q_8_8(1);

    palette   = syResourceGetTableEntry(RESOURCE_RUNE_SHEET);
    syWordsCopy(BG_PLTT, palette, SHEET_PALETTE_BYTES);
    palette += SHEET_PALETTE_BYTES;
    syDecompressTaggedLZ(palette, state->sheet);
    syDecompressTaggedLZ(syResourceGetTableEntry(RESOURCE_YELLOW_GLOW_SHEET), graphics);
    syDecompressTaggedLZ(syResourceGetTableEntry(RESOURCE_BLUE_GLOW_SHEET),
                         state->sheet + BIND_FLARE_PICTURES);
    palette  = syResourceGetTableEntry(RESOURCE_JUPITER_DJINN_SHEET);
    palette += SHEET_PALETTE_BYTES;
    syDecompressTaggedLZ(palette, state->sheet + BIND_DJINNI_PICTURE);
    EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(4));
    btProjectionScaledUnit(state->args->targets[0], &anchor);
    shift    = CANVAS_SIZE / 2 - anchor.x;
    REG_BG2X = shift << 8;
    sySoundPlayStub(SE_SWIRL);

    for (frame = 0; frame != state->args->targetCount * 20 + 72; frame++)
    {
        if (frame == BIND_RUNE_TICK)
            EFFECT_SHOW_DAMAGE(0);
        // The colours cycle through the hues.
        btEffectUpdatePhasePalette(frame, TRIG_TURN - DEGtoANG(120), DEGtoANG(120), 0);

        // The Djinni sways, then rises away.
        if (state->args->prelude == 1)
        {
            Sint32 x;
            Sint32 y;

            x = FIXEDtoINT(sySin(frame * (TRIG_TURN / 32)) * BIND_DJINNI_SWAY) + originX + shift
              - DJINNI_WIDTH / 2;
            y = FIXEDtoINT(syCos(frame * (TRIG_TURN / 32)) * 4) + originY - 24;
            btEffectBuildCanvasBlitters(state->args->side, draw);
            if (frame > 32)
                y = y - frame * 2 + 64;
            draw[0](canvas, state->sheet + BIND_DJINNI_PICTURE, x, y, DJINNI_WIDTH, DJINNI_HEIGHT);
            if (frame <= 3)
                draw[1](
                    canvas, state->sheet + BIND_DJINNI_PICTURE, x, y, DJINNI_WIDTH, DJINNI_HEIGHT);
            EFFECT_RELEASE_BLITTERS();
        }
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, COMPOSITE_LIGHTEN);
        draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
        EFFECT_LOAD_BLITTER(
            HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_LIGHTEN);
        draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
        if (frame > 16 && (frame & 15) == 0)
            state->transferValue += CANVAS_FILL_WORD(1);

        // Only the first target's triangle is drawn, though the ticks are counted for every target.
        for (member = 0; member != 1; member++)
        {
            memberFrame = frame - member * 8;
            actor       = btObjectGetStub(state->args->targets[member])->actor;
            if (memberFrame >= 0 && memberFrame < 96)
            {
                EFFECT_VIEW_FROM_CAMERA(camera);
                SET_VECTOR(syScreenPointCoordinates(&corner),
                           actor->x.fixed,
                           actor->y.fixed,
                           actor->z.fixed);
                btProjectionPoint(&corner, &screen);
                screen.x  = anchor.x + shift;
                screen.y -= 24;
                // The triangle.
                if (memberFrame <= 67)
                {
                    for (j = 0; j != 3; j++)
                    {
                        point = &state->particles[member * BIND_SLOTS_PER_TARGET + j];
                        syInitMatrix();
                        if (memberFrame < BIND_RUNE_TICK)
                        {
                            scale.x = toFIXED(21) / 8 - (memberFrame * 3 << 9);
                            scale.y = toFIXED(21) / 8 - (memberFrame * 3 << 9);
                            scale.z = toFIXED(21) / 8 - (memberFrame * 3 << 9);
                            syMatrixScaleVector(syVector3Coordinates(&scale));
                            syRotZ((BIND_RUNE_TICK - memberFrame) << 9);
                            syRotY((BIND_RUNE_TICK - memberFrame) << 9);
                        }
                        syRotZ(j * (DEGtoANG(120)));
                        btProjectionPoint(BindVertex, &corner);
                        point->velocityX = corner.x + screen.x;
                        point->velocityY = corner.y + screen.y + 16;
                    }
                    for (j = 0; j != 3; j++)
                    {
                        PARTICLE *from = &state->particles[j + member * BIND_SLOTS_PER_TARGET];
                        PARTICLE *to =
                            &state->particles[(j + 1) % 3 + member * BIND_SLOTS_PER_TARGET];

                        size = 5 - memberFrame / 16;
                        for (k = 0; k != BIND_EDGE_FLARES; k++)
                        {
                            Sint32 x = from->velocityX
                                     + k * (to->velocityX - from->velocityX) / BIND_EDGE_FLARES;
                            Sint32 y = from->velocityY
                                     + k * (to->velocityY - from->velocityY) / BIND_EDGE_FLARES;

                            EFFECT_DRAW_FLARE(
                                draw[0], canvas, state->sheet + BIND_FLARE_PICTURES, x, y, size);
                        }
                    }
                }
                // Then the rune, in two mirrored halves.
                if (memberFrame >= BIND_RUNE_TICK)
                {
                    draw[0](canvas,
                            state->sheet,
                            screen.x - RUNE_HALF_WIDTH,
                            screen.y - RUNE_HALF_HEIGHT / 2,
                            RUNE_HALF_WIDTH,
                            RUNE_HALF_HEIGHT);
                    draw[1](canvas,
                            state->sheet,
                            screen.x,
                            screen.y - RUNE_HALF_HEIGHT / 2,
                            RUNE_HALF_WIDTH,
                            RUNE_HALF_HEIGHT);
                }
            }
        }
        EFFECT_RELEASE_BLITTERS();
        EFFECT_PRESENT_CANVAS(state);
    }

    syCloseEvent(btEffectTransferCanvas);
    btEffectEndCanvasLayer();
}

// Unused: no row of AbilityBattleEffects in either game names its kind, and the handler table is
// reached with no other kind (btEffectRunFromTable), so the player never sees it. Its tick loop is
// btEffectRunBind's, without the Djinni prelude, and on tick 46 it starts the user's approach to
// the first target.
void btEffectRunUnusedTriangleStrike(BATTLE_EFFECT_PARM *args)
{
    SCREEN_POINT         anchor;
    // The target's position, then each corner projected around it.
    SCREEN_POINT         corner;
    SCREEN_POINT         screen;
    VECTOR3              scale;
    BATTLE_EFFECT_STATE *state;
    void                *canvas;
    Sint32               frame;
    BLITTER_PAIR         draw;
    Sint32               member;
    BATTLE_CAMERA       *camera;
    Sint32               shift;
    Uint8               *graphics;
    Uint8               *palette;
    ACTOR               *actor;
    Sint32               memberFrame;
    PARTICLE            *point;
    Sint32               size;
    Sint32               j;
    Sint32               k;

    state       = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
    canvas      = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
    graphics    = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
    camera      = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
    state->args = args;
    EFFECT_START_NARROW_CANVAS(0);

    palette = syResourceGetTableEntry(RESOURCE_RUNE_SHEET);
    syWordsCopy(BG_PLTT, palette, SHEET_PALETTE_BYTES);
    palette += SHEET_PALETTE_BYTES;
    syDecompressTaggedLZ(palette, state->sheet);
    syDecompressTaggedLZ(syResourceGetTableEntry(RESOURCE_YELLOW_GLOW_SHEET), graphics);
    palette = syResourceGetTableEntry(RESOURCE_BLUE_GLOW_SHEET);
    syDecompressTaggedLZ(palette, state->sheet + BIND_FLARE_PICTURES);
    EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(4));
    btProjectionScaledUnit(state->args->targets[0], &anchor);
    shift    = CANVAS_SIZE / 2 - anchor.x;
    REG_BG2X = shift << 8;
    sySoundPlayStub(SE_SWIRL);

    for (frame = 0; frame != state->args->targetCount * 20 + 72; frame++)
    {
        if (frame == BIND_RUNE_TICK)
            EFFECT_SHOW_DAMAGE(0);
        // The user sets off towards the target.
        if (frame == 46)
            btMotionApproachTargetStub(
                state->args->user, state->args->targets[0], TRIANGLE_STRIKE_LEAP_TICKS, 0);
        btEffectUpdatePhasePalette(frame, TRIG_TURN - DEGtoANG(120), DEGtoANG(120), 0);
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, COMPOSITE_LIGHTEN);
        draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
        EFFECT_LOAD_BLITTER(
            HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_LIGHTEN);
        draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
        if (frame > 16 && (frame & 15) == 0)
            state->transferValue += CANVAS_FILL_WORD(1);

        // Only the first target's triangle is drawn, though the ticks are counted for every target.
        for (member = 0; member != 1; member++)
        {
            memberFrame = frame - member * 8;
            actor       = btObjectGetStub(state->args->targets[member])->actor;
            if (memberFrame >= 0 && memberFrame < 96)
            {
                EFFECT_VIEW_FROM_CAMERA(camera);
                SET_VECTOR(syScreenPointCoordinates(&corner),
                           actor->x.fixed,
                           actor->y.fixed,
                           actor->z.fixed);
                btProjectionPoint(&corner, &screen);
                screen.x  = anchor.x + shift;
                screen.y -= 24;
                if (memberFrame <= 67)
                {
                    for (j = 0; j != 3; j++)
                    {
                        point = &state->particles[member * BIND_SLOTS_PER_TARGET + j];
                        syInitMatrix();
                        if (memberFrame < BIND_RUNE_TICK)
                        {
                            scale.x = toFIXED(21) / 8 - (memberFrame * 3 << 9);
                            scale.y = toFIXED(21) / 8 - (memberFrame * 3 << 9);
                            scale.z = toFIXED(21) / 8 - (memberFrame * 3 << 9);
                            syMatrixScaleVector(syVector3Coordinates(&scale));
                            syRotZ((BIND_RUNE_TICK - memberFrame) << 9);
                            syRotY((BIND_RUNE_TICK - memberFrame) << 9);
                        }
                        syRotZ(j * (DEGtoANG(120)));
                        btProjectionPoint(BindTriangleStrikeVertex, &corner);
                        point->velocityX = corner.x + screen.x;
                        point->velocityY = corner.y + screen.y + 16;
                    }
                    for (j = 0; j != 3; j++)
                    {
                        PARTICLE *from = &state->particles[j + member * BIND_SLOTS_PER_TARGET];
                        PARTICLE *to =
                            &state->particles[(j + 1) % 3 + member * BIND_SLOTS_PER_TARGET];

                        size = 5 - memberFrame / 16;
                        for (k = 0; k != BIND_EDGE_FLARES; k++)
                        {
                            Sint32 x = from->velocityX
                                     + k * (to->velocityX - from->velocityX) / BIND_EDGE_FLARES;
                            Sint32 y = from->velocityY
                                     + k * (to->velocityY - from->velocityY) / BIND_EDGE_FLARES;

                            EFFECT_DRAW_FLARE(
                                draw[0], canvas, state->sheet + BIND_FLARE_PICTURES, x, y, size);
                        }
                    }
                }
                // Then the rune, in two mirrored halves.
                if (memberFrame >= BIND_RUNE_TICK)
                {
                    draw[0](canvas,
                            state->sheet,
                            screen.x - RUNE_HALF_WIDTH,
                            screen.y - RUNE_HALF_HEIGHT / 2,
                            RUNE_HALF_WIDTH,
                            RUNE_HALF_HEIGHT);
                    draw[1](canvas,
                            state->sheet,
                            screen.x,
                            screen.y - RUNE_HALF_HEIGHT / 2,
                            RUNE_HALF_WIDTH,
                            RUNE_HALF_HEIGHT);
                }
            }
        }
        EFFECT_RELEASE_BLITTERS();
        EFFECT_PRESENT_CANVAS(state);
    }

    syCloseEvent(btEffectTransferCanvas);
    btEffectEndCanvasLayer();
}

#undef BIND_FLARE_PICTURES
#undef BIND_DJINNI_PICTURE
#undef BIND_SLOTS_PER_TARGET
#undef BIND_EDGE_FLARES
#undef BIND_RUNE_TICK
#undef BIND_DJINNI_SWAY
#undef TRIANGLE_STRIKE_LEAP_TICKS
#undef BLUE_RAMP_PALETTE
#undef BLUE_RAMP_SECOND_PALETTE
#undef BLUE_RAMP_COLORS
#undef BLUE_RAMP_START_SPEED
#undef BLUE_RAMP_TICKS
#undef BLUE_RAMP_LAST_ROW

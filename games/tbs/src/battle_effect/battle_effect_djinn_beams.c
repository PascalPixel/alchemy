// Revive, Water of Life, Quartz and Dew. Part of battle_effect.c.

#define DJINN_BEAMS_MOTES             512 // motes in PARTICLES
// Each target's own run of the motes, of which the first eight are drawn.
#define DJINN_BEAMS_TARGET_MOTES      64
// Each target's motes and column start this many ticks after the last's.
#define DJINN_BEAMS_TARGET_TICKS      16
// How far a mote sways either side, and the Djinni across and up and down, in pixels.
#define DJINN_BEAMS_MOTE_SWAY         16
#define DJINN_BEAMS_DJINNI_SWAY_X     20
#define DJINN_BEAMS_DJINNI_SWAY_Y     4

// The motes (CrescentMoonSheet), then the Djinni.
#define DJINN_BEAMS_MOTE_PICTURES     10000
#define DJINN_BEAMS_MOTE_WIDTH        24
#define DJINN_BEAMS_MOTE_HEIGHT       24
#define DJINN_BEAMS_DJINNI_PICTURE    26048

// The column of light is drawn a row at a time from EarthWallSheet's second picture, one row of six
// pixels brightening from the edge inward (the first, before it, is five), and mirrored for its
// right half. As the column narrows it takes fewer of them, so it dims too.
#define DJINN_BEAMS_COLUMN_RAMP       5
#define DJINN_BEAMS_COLUMN_HALF_WIDTH 6

// Motes drift down over each target and a column of light falls on it, in the Venus Djinni's
// colours; with a prelude, a Venus Djinni appears first.
//
// Abilities: Revive, Water of Life; Quartz.
void btEffectRunRevive(BATTLE_EFFECT_PARM *args)
{
    btEffectRunDjinnBeams(args, ELEMENT_VENUS);
}

// Dew plays Revive's beams of light in the Mercury Djinni's colours, and with a prelude a Mercury
// Djinni appears first.
//
// Abilities: Dew.
void btEffectRunDew(BATTLE_EFFECT_PARM *args)
{
    btEffectRunDjinnBeams(args, ELEMENT_MERCURY);
}

// When there is a prelude a Djinni appears and rises away; then over each target in turn motes
// drift down and a column of light falls on it.
void btEffectRunDjinnBeams(BATTLE_EFFECT_PARM *args, Sint32 element)
{
    SCREEN_POINT         onlyTarget;
    SCREEN_POINT         screenPos;
    Sint32               ground[3];
    Sint32               point[3];
    Sint32               djinniX;
    Sint32               djinniY;
    BLITTER_PAIR         draw;
    BATTLE_EFFECT_STATE *state;
    void                *canvas;
    Sint32               frame;
    Sint32               offsetX;
    Uint8               *palette;
    Sint32               i;
    Sint32               k;

    state       = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
    canvas      = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
    state->args = args;
    btEffectStartCanvasLayer(0);
    if (state->args->prelude == 1)
        btEffectRunDjinniEntrance(args,
                                  element,
                                  state->args->side ^ state->args->prelude,
                                  DJINNI_ANCHOR_OVER_TARGET,
                                  &djinniX,
                                  &djinniY);
    palette = syResourceGetTableEntry(RESOURCE_EARTH_WALL_SHEET);
    syWordsCopy(BG_PLTT, palette, SHEET_PALETTE_BYTES);
    palette += SHEET_PALETTE_BYTES;
    syDecompressTaggedLZ(palette, state->sheet);
    palette = syResourceGetTableEntry(RESOURCE_CRESCENT_MOON_SHEET);
    syWordsCopy(BG_PLTT, palette, SHEET_PALETTE_BYTES);
    palette += SHEET_PALETTE_BYTES;
    syDecompressTaggedLZ(palette, state->sheet + DJINN_BEAMS_MOTE_PICTURES);
    if (element == ELEMENT_VENUS)
        palette = syResourceGetTableEntry(RESOURCE_VENUS_DJINN_SHEET);
    else
        palette = syResourceGetTableEntry(RESOURCE_MERCURY_DJINN_SHEET);
    syWordsCopy(BG_PLTT, palette, SHEET_PALETTE_BYTES);
    palette += SHEET_PALETTE_BYTES;
    syDecompressTaggedLZ(palette, state->sheet + DJINN_BEAMS_DJINNI_PICTURE);

    for (i = 0; i != DJINN_BEAMS_MOTES; i++)
    {
        PARTICLES[i].x       = 0;
        PARTICLES[i].y       = toFIXED(80);
        PARTICLES[i].z       = (syRandom() | -32) << 14;
        PARTICLES[i].variant = syRandom() & 255;
    }

    REG_BG2PA = Q_8_8(1);
    if (state->args->targetCount == 1)
    {
        btProjectionScaledUnit(state->args->targets[0], &onlyTarget);
        offsetX = CANVAS_SIZE / 2 - onlyTarget.x;
    }
    else
    {
        if (state->args->side == EFFECT_SIDE_PARTY)
            offsetX = -NARROW_CANVAS_PARTY_X;
        else
            offsetX = 0;
    }
    REG_BG2X = offsetX << 8;
    EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

    for (frame = 0; frame != state->args->targetCount * DJINN_BEAMS_TARGET_TICKS + 116; frame++)
    {
        BATTLE_CAMERA *camera = BattleCameraWork;

        if (frame == 64)
            sySoundPlayStub(SE_POWER_UP);
        if (frame == 80)
            EFFECT_SHOW_DAMAGE(0);

        // The Djinni sways, then rises away.
        if (state->args->prelude == 1)
        {
            Sint32 angle = frame * (TRIG_TURN / 32);
            Sint32 x     = FIXEDtoINT(-sySin(angle) * DJINN_BEAMS_DJINNI_SWAY_X) + djinniX + offsetX
                     - DJINNI_WIDTH / 2;
            Sint32 y = FIXEDtoINT(syCos(angle) * DJINN_BEAMS_DJINNI_SWAY_Y) + djinniY - 24;

            btEffectBuildCanvasBlitters(state->args->side ^ 1, draw);
            if (frame > 32)
                y = y - frame * 2 + 64;
            draw[1](canvas,
                    state->sheet + DJINN_BEAMS_DJINNI_PICTURE,
                    x,
                    y,
                    DJINNI_WIDTH,
                    DJINNI_HEIGHT);
            if (frame <= 3)
                draw[1](canvas,
                        state->sheet + DJINN_BEAMS_DJINNI_PICTURE,
                        x,
                        y,
                        DJINNI_WIDTH,
                        DJINNI_HEIGHT);
            EFFECT_RELEASE_BLITTERS();
        }
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, COMPOSITE_LIGHTEN);
        draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
        EFFECT_LOAD_BLITTER(
            HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_LIGHTEN);
        draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);

        // Motes sink, swaying.
        EFFECT_FOR_EACH_TARGET(k, state->args)
        {
            ACTOR *target = btObjectGetStub(state->args->targets[k])->actor;
            Sint32 start  = k * DJINN_BEAMS_TARGET_TICKS;

            EFFECT_VIEW_FROM_CAMERA(camera);
            SET_VECTOR(point, target->x.fixed, 0, target->z.fixed);
            syMatrixTranslateVector(point);
            if (frame > start)
            {
                for (i = 0; i != 8; i++)
                {
                    PARTICLE *mote = &PARTICLES[k * DJINN_BEAMS_TARGET_MOTES + i];

                    if (frame > i * 8 + start && mote->y > toFIXED(8))
                    {
                        Sint32 sway;

                        btProjectionPoint(mote, &screenPos);
                        screenPos.x += offsetX;
                        sway         = FIXEDtoINT(sySin(mote->variant * (TRIG_TURN / 64))
                                          * DJINN_BEAMS_MOTE_SWAY);
                        if (i & 1)
                            screenPos.x -= sway;
                        else
                            screenPos.x += sway;
                        draw[i & 1](canvas,
                                    state->sheet
                                        + DjinnBeamsMotePictures[mote->variant / 8 & 7]
                                              * PICTURE_BYTES(DJINN_BEAMS_MOTE)
                                        + DJINN_BEAMS_MOTE_PICTURES,
                                    screenPos.x - DJINN_BEAMS_MOTE_WIDTH / 2,
                                    screenPos.y - DJINN_BEAMS_MOTE_HEIGHT / 2,
                                    DJINN_BEAMS_MOTE_WIDTH,
                                    DJINN_BEAMS_MOTE_HEIGHT);
                        mote->y -= toFIXED(1);
                        mote->variant++;
                    }
                }
            }
        }

        // The column of light.
        EFFECT_FOR_EACH_TARGET(i, state->args)
        {
            Sint32 start = i * DJINN_BEAMS_TARGET_TICKS;

            if (frame >= start + 72)
            {
                ACTOR *target = btObjectGetStub(state->args->targets[i])->actor;

                EFFECT_VIEW_FROM_CAMERA(camera);
                if (frame == start + 72)
                    btEffectTintBattler(
                        state->args->targets[i], SPRITE_COLOR_GRB, TINT_KEEP, TINT_KEEP, 0);
                if (frame == start + 88)
                    btEffectTintBattler(
                        state->args->targets[i], SPRITE_COLOR_NORMAL, TINT_KEEP, TINT_KEEP, 0);
                SET_VECTOR(ground, target->x.fixed, 0, target->z.fixed);
                btProjectionPoint(ground, &screenPos);
                screenPos.x += offsetX;
                if (frame < start + 104)
                {
                    Sint32 picture = frame / 4;
                    Sint32 width   = DJINN_BEAMS_COLUMN_HALF_WIDTH;

                    if (frame > start + 88)
                        width = DJINN_BEAMS_COLUMN_HALF_WIDTH - (frame - start - 88) / 3;
                    if (picture > 2)
                        picture = (picture & 1) + 1;
                    if (frame < start + 100)
                    {
                        draw[0](canvas,
                                state->sheet + DjinnBeamsHeadPictures[picture],
                                screenPos.x - DjinnBeamsHeadWidths[picture],
                                screenPos.y - DjinnBeamsHeadHeights[picture] + 8,
                                DjinnBeamsHeadWidths[picture],
                                DjinnBeamsHeadHeights[picture]);
                        draw[1](canvas,
                                state->sheet + DjinnBeamsHeadPictures[picture],
                                screenPos.x,
                                screenPos.y - DjinnBeamsHeadHeights[picture] + 8,
                                DjinnBeamsHeadWidths[picture],
                                DjinnBeamsHeadHeights[picture]);
                    }
                    for (k = 0; k != screenPos.y; k++)
                    {
                        draw[0](canvas,
                                state->sheet + DJINN_BEAMS_COLUMN_RAMP,
                                screenPos.x - width,
                                k,
                                width,
                                1);
                        draw[1](canvas,
                                state->sheet + DJINN_BEAMS_COLUMN_RAMP,
                                screenPos.x,
                                k,
                                width,
                                1);
                    }
                }
            }
        }
        EFFECT_RELEASE_BLITTERS();
        EFFECT_PRESENT_CANVAS(state);
    }

    syCloseEvent(btEffectTransferCanvas);
    btEffectEndCanvasLayer();
}

#undef DJINN_BEAMS_MOTE_PICTURES
#undef DJINN_BEAMS_DJINNI_PICTURE
#undef DJINN_BEAMS_MOTES
#undef DJINN_BEAMS_MOTE_WIDTH
#undef DJINN_BEAMS_MOTE_HEIGHT
#undef DJINN_BEAMS_TARGET_MOTES
#undef DJINN_BEAMS_TARGET_TICKS
#undef DJINN_BEAMS_MOTE_SWAY
#undef DJINN_BEAMS_DJINNI_SWAY_X
#undef DJINN_BEAMS_DJINNI_SWAY_Y
#undef DJINN_BEAMS_COLUMN_RAMP
#undef DJINN_BEAMS_COLUMN_HALF_WIDTH

// The twisting column of Whorl, Gale, Lethe Albion, Poison Flow and Raging Flood, and the Djinn
// Core and Cannon. Part of battle_effect.c.

// The ticks Whorl and Gale last, and the others.
#define TWISTING_COLUMN_DJINN_TICKS                  38
#define TWISTING_COLUMN_TICKS                        52

// The column's rows of corners, two (x, y, z, pad) each, as the strip's quads join them:
// TWISTING_COLUMN_QUADS quads, one row more. They are built at the start of the map cell buffer,
// which the transform reads as MapGridMetatileBuffer.
#define TWISTING_COLUMN_CORNERS                      ((Sint8 (*)[2][4])RamMapMetatileBuffer)
#define TWISTING_COLUMN_QUADS                        32
#define TWISTING_COLUMN_ROWS                         (TWISTING_COLUMN_QUADS + 1)
#define TWISTING_COLUMN_VERTICES                     (TWISTING_COLUMN_ROWS * 2)

// The column's transformed corners, its strip's mesh and the second sprite sheet, in the map cell
// buffer.
#define TWISTING_COLUMN_TRANSFORMED                  (RamMapMetatileBuffer + 4096)
#define TWISTING_COLUMN_MESH                         (RamMapMetatileBuffer + 8192)
#define TWISTING_COLUMN_SHEET                        (RamMapMetatileBuffer + 16384)

// Where JupiterDjinnSmallSheet is decoded in the effect's pictures.
#define TWISTING_COLUMN_JUPITER_DJINN_SMALL_PICTURES 8192

// Where FlashBurstSheet is decoded in the map cell buffer.
#define TWISTING_COLUMN_FLASH_BURST                  20480

// The last particle keeps the target's position, and its scale in velocityX and velocityY, to
// restore when the column ends; the particles before it are the motes and flashes.
#define TWISTING_COLUMN_SAVED_TARGET                 63

// The motes that trail the funnel of Whorl and Gale.
#define TWISTING_COLUMN_MOTES                        16

// The angle a tick the Jupiter Djinni's bob turns by as it rides the funnel, about 18 degrees: a
// bob every 20 ticks.
#define TWISTING_COLUMN_BOB_STEP                     3304

// How far the Jupiter Djinni sways to either side as it rides the funnel, a sway every 32 ticks.
#define TWISTING_COLUMN_DJINNI_SWAY                  16

// Whorl draws its target toward the Djinni, 1/128 of the way a tick at first, then 1/16 a tick as
// it shrinks toward a quarter of its size.
#define TWISTING_COLUMN_DRIFT_STEPS                  128
#define TWISTING_COLUMN_LIFT_STEPS                   16

// Lethe Albion's and Raging Flood's bands of water: 128 pixels long and 48 wide, rippling up to 28
// pixels from their axis, and stretched to about 2.09 times their length, wider than the screen.
// Their swirls are squares stretched to about 1.31 across and flattened to half their height.
#define TWISTING_COLUMN_BAND_HALF_LENGTH             64
#define TWISTING_COLUMN_BAND_WIDTH                   48
#define TWISTING_COLUMN_BAND_RIPPLE                  28
#define TWISTING_COLUMN_BAND_STRETCH                 137072
#define TWISTING_COLUMN_SWIRL_STRETCH                85536

// Poison Flow's stream ripples up to 64 pixels from its axis.
#define TWISTING_COLUMN_STREAM_RIPPLE                64

// Whorl's and Gale's funnel widens to 80 pixels and ripples up to 96 from its axis.
#define TWISTING_COLUMN_FUNNEL_HALF_WIDTH            40
#define TWISTING_COLUMN_FUNNEL_RIPPLE                96

// Lethe Albion's and Raging Flood's three bands of water rise TWISTING_COLUMN_WAVE_RISE pixels and
// settle again over the cast, half a turn of a sine.
#define TWISTING_COLUMN_WAVE_RISE                    64
#define TWISTING_COLUMN_WAVE_RISE_STEP               (TRIG_TURN / 2 / TWISTING_COLUMN_TICKS)

// How far Whorl's and Gale's funnel turns about its length from the upright, about 17 degrees: it
// slants down from the top right onto the target.
#define TWISTING_COLUMN_LEAN                         3096

// The column's brightest shades (RENDER_MODEL.shade), and the ticks it starts to fade out.
#define TWISTING_COLUMN_BAND_SHADE                   (-20)
#define TWISTING_COLUMN_FLOOD_SHADE                  (-8)
#define TWISTING_COLUMN_FADE_TICK                    46
#define TWISTING_COLUMN_FUNNEL_FADE_TICK             30

// The twisting column shared by the Djinn Whorl and Gale, Lethe Albion, Poison Flow and Raging
// Flood, a textured strip turned about its length. For the Djinn the Jupiter Djinni rides the head
// of a pink funnel of wind that sweeps down on the target, carrying it up (Whorl) or blowing it
// away (Gale); Poison Flow's red stream sweeps out from the caster through the targets; and Lethe
// Albion's and Raging Flood's three bands of water surge up over the targets' side and settle, as
// clouds of spray burst from the target and two swirls (Lethe Albion's runes) spin out over it.
void btEffectRunTwistingColumn(BATTLE_EFFECT_PARM *args, Uint32 mode)
{
    BATTLE_EFFECT_STATE *state;
    Uint8               *canvas;
    // LEFTOVER(frame): blit[1], the pair's idle second blitter, which the ROM's
    // * tick reserves and never touches; as one DRAW_RECTANGLE.
    DRAW_RECTANGLE       blit[2];
    Sint32               frame;
    BATTLE_CAMERA       *camera;
    Uint8               *sprites;
    Sint32               ticks;
    UNIT                *unit;
    MOTION_OBJECT       *target;
    RENDER_MODEL        *model;
    MODEL_TEXTURE        texture;
    Sint32               columns;
    Sint32               shade;
    Sint32               i;
    Sint32               j;
    // Where the sprite sheets' palettes are copied.
    void                *palette  = (void *)BG_PLTT;
    void                *vertices = TWISTING_COLUMN_TRANSFORMED;
    Uint8               *sheet    = TWISTING_COLUMN_SHEET;
    void                *mesh     = TWISTING_COLUMN_MESH;
    // Where the Jupiter Djinni's entrance leaves it, in the world and on the screen, and where it
    // rides the funnel this tick. Lethe Albion and Raging Flood set djinniPlace to the caster's
    // place, which they never read.
    Sint32               djinniStart[3];
    Sint32               djinniScreen[3];
    Sint32               djinniPlace[3];
    SCREEN_POINT         moteScreen;
    SCREEN_POINT         targetScreen;
    SCREEN_POINT         casterScreen;

    state   = RamHeapSlots->battleEffect;
    canvas  = RamHeapSlots->battleCanvas;
    camera  = RamHeapSlots->cameraState;
    sprites = RamHeapSlots->battleSheet;
    unit    = syUnitGet(args->targets[0]);
    btEffectStartCanvasLayer(0);
    if (mode <= TWISTING_COLUMN_GALE)
    {
        ticks = TWISTING_COLUMN_DJINN_TICKS;
        btEffectRunDjinniEntrance(
            args,
            DJINNI_ENTRANCE(ELEMENT_JUPITER, args->side, DJINNI_ANCHOR_HIGH_USER_X),
            djinniScreen,
            djinniStart);
        REG_BG2CNT =
            BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
    }
    else if (mode == TWISTING_COLUMN_LETHE_ALBION || mode == TWISTING_COLUMN_RAGING_FLOOD)
    {
        MOTION_OBJECT *caster = btObjectGetStub(args->user)->object;

        if (mode == TWISTING_COLUMN_LETHE_ALBION)
        {
            Sint32 row;
            Sint32 k;
            Sint32 size = RUNE_TEXTURE_SIZE * RUNE_TEXTURE_SIZE;

            // The rune's half, mirrored into a whole rune on the texture.
            btResourceLoadAndDecompress(RESOURCE_RUNE_SHEET, sheet + size, 1, 0);
            syWordsClearEntry(sheet, size);
            for (row = 0; row != RUNE_HALF_HEIGHT; row++)
            {
                for (k = 0; k != RUNE_HALF_WIDTH; k++)
                {
                    (TWISTING_COLUMN_SHEET + RUNE_TEXTURE_RUNE)[row * RUNE_TEXTURE_SIZE + k] =
                        sheet[row * RUNE_HALF_WIDTH + RUNE_TEXTURE_SIZE * RUNE_TEXTURE_SIZE + k];
                    sheet[row * RUNE_TEXTURE_SIZE + (RUNE_TEXTURE_RUNE + 2 * RUNE_HALF_WIDTH - 1)
                          - k] =
                        sheet[row * RUNE_HALF_WIDTH + RUNE_TEXTURE_SIZE * RUNE_TEXTURE_SIZE + k];
                }
            }
            syWordsCopyEntry(
                palette, syResourceGetTableEntry(RESOURCE_BLUE_FLAME_SHEET), SHEET_PALETTE_BYTES);
        }
        else
        {
            btResourceLoadAndDecompress(RESOURCE_SWIRL_TEXTURE, sheet, 0, 0);
        }
        djinniPlace[0] = caster->x;
        djinniPlace[1] = caster->y;
        djinniPlace[2] = caster->z;
        btResourceLoadAndDecompress(
            RESOURCE_FLASH_BURST_SHEET, RamMapMetatileBuffer + TWISTING_COLUMN_FLASH_BURST, 1, 0);
        ticks = TWISTING_COLUMN_TICKS;
    }
    else
    {
        REG_BG2CNT =
            BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
        ticks = TWISTING_COLUMN_TICKS;
    }
    if (!args->side)
    {
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
    }
    else
    {
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
    }
    blit[0]      = RamHeapSlots->ramCode;
    REG_BLDALPHA = EFFECT_BLEND_ADD;
    EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
    target                                           = btObjectGetStub(args->targets[0])->object;
    state->particles[TWISTING_COLUMN_SAVED_TARGET].x = target->x;
    state->particles[TWISTING_COLUMN_SAVED_TARGET].y = target->y;
    state->particles[TWISTING_COLUMN_SAVED_TARGET].z = target->z;
    state->particles[TWISTING_COLUMN_SAVED_TARGET].velocityX = target->scaleX;
    state->particles[TWISTING_COLUMN_SAVED_TARGET].velocityY = target->scaleY;
    if (mode == TWISTING_COLUMN_LETHE_ALBION || mode == TWISTING_COLUMN_RAGING_FLOOD)
    {
        btResourceLoadAndDecompress(RESOURCE_EMBER_TEXTURE, state->sheet, 0, 0);
    }
    else
    {
        btResourceLoadAndDecompress(RESOURCE_STREAK_TEXTURE, state->sheet, 0, 0);
    }
    btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sprites, 0, 0);
    btResourceLoadAndDecompress(RESOURCE_JUPITER_DJINN_SMALL_SHEET,
                                state->sheet + TWISTING_COLUMN_JUPITER_DJINN_SMALL_PICTURES,
                                1,
                                1);
    if (mode == TWISTING_COLUMN_POISON_FLOW)
    {
        syWordsCopyEntry(
            palette, syResourceGetTableEntry(RESOURCE_MARS_DJINN_SHEET), SHEET_PALETTE_BYTES);
    }
    if (mode == TWISTING_COLUMN_RAGING_FLOOD || mode == TWISTING_COLUMN_LETHE_ALBION)
    {
        syWordsCopyEntry(
            palette, syResourceGetTableEntry(RESOURCE_WATER_SPRAY_SHEET), SHEET_PALETTE_BYTES);
    }
    columns =
        (mode == TWISTING_COLUMN_LETHE_ALBION || mode == TWISTING_COLUMN_RAGING_FLOOD) ? 8 : 4;
    btRenderBuildQuadStrip(mesh, columns, 32, TWISTING_COLUMN_QUADS);

    frame = 0;
    if (ticks != 0)
    {
        do
        {
            if (frame == 0)
            {
                sySoundPlayStub(SE_WATER_COLUMN);
            }
            if (frame == 32)
            {
                sySoundPlayStub(SE_CURSE_FALL);
            }
            if (frame == 0)
            {
                target->x      = state->particles[TWISTING_COLUMN_SAVED_TARGET].x;
                target->y      = state->particles[TWISTING_COLUMN_SAVED_TARGET].y;
                target->z      = state->particles[TWISTING_COLUMN_SAVED_TARGET].z;
                target->scaleX = state->particles[TWISTING_COLUMN_SAVED_TARGET].velocityX;
                target->scaleY = state->particles[TWISTING_COLUMN_SAVED_TARGET].velocityY;
                if (mode == TWISTING_COLUMN_WHORL)
                {
                    for (i = 0; i != TWISTING_COLUMN_SAVED_TARGET; i++)
                    {
                        PARTICLE *mote = &state->particles[i];

                        if (args->side == EFFECT_SIDE_PARTY)
                        {
                            mote->x = toFIXED(200);
                        }
                        else
                        {
                            mote->x = -toFIXED(200);
                        }
                        mote->y         = toFIXED((syRandom() & 127) - 64);
                        mote->z         = toFIXED((syRandom() & 127) - 64);
                        mote->velocityX = mote->x;
                        mote->velocityZ = mote->z;
                        mote->velocityY = mote->y;
                    }
                }
                else if (mode == TWISTING_COLUMN_GALE)
                {
                    {
                        PARTICLE *mote = state->particles;

                        for (i = 0; i != TWISTING_COLUMN_SAVED_TARGET; i++, mote++)
                        {
                            SET_POSITION(
                                mote, djinniStart[0], toFIXED(syRandom() & 31) + toFIXED(64), 0);
                            mote->velocityX = ((syRandom() & 63) + 64) << 13;
                            if (mote->x > 0)
                            {
                                mote->velocityX = -mote->velocityX;
                            }
                            mote->velocityY = (syRandom() | -128) << 11;
                            mote->velocityZ = 0;
                        }
                    }
                }
            }
            syInitMatrix();
            syLookAtLoad(camera->eye, camera->focus);
            if (mode <= TWISTING_COLUMN_GALE)
            {
                Sint32 k;

                djinniPlace[0] =
                    djinniStart[0] + sySin(frame * (TRIG_TURN / 32)) * TWISTING_COLUMN_DJINNI_SWAY;
                k = 4;
                if (frame > 39)
                {
                    k = frame - 36;
                }
                djinniPlace[1] =
                    djinniStart[1] + toFIXED(k) - syCos(frame * TWISTING_COLUMN_BOB_STEP) * k;
                if (frame > 27)
                {
                    djinniPlace[1] = djinniPlace[1] + frame * toFIXED(4) - toFIXED(112);
                }
                djinniPlace[2] = djinniStart[2];
                btEffectPosProjectPoint(djinniPlace, (SCREEN_POINT *)djinniScreen);
                blit[0](canvas,
                        state->sheet + TWISTING_COLUMN_JUPITER_DJINN_SMALL_PICTURES,
                        djinniScreen[0] / 2 - 10,
                        djinniScreen[1] - 20,
                        SMALL_DJINNI_WIDTH,
                        SMALL_DJINNI_HEIGHT);
            }
            if (frame > 7)
            {
                if (mode <= TWISTING_COLUMN_GALE && frame <= 47)
                {
                    state->shakeFrame = 3;
                }
                if (mode == TWISTING_COLUMN_WHORL)
                {
                    if (frame == 32)
                    {
                        if (args->variant == EFFECT_VARIANT_STRONGER)
                        {
                            target->y = toFIXED(124);
                        }
                        else
                        {
                            state->shakeFrame = 8;
                            EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
                            target->scaleX =
                                state->particles[TWISTING_COLUMN_SAVED_TARGET].velocityX;
                            target->scaleY =
                                state->particles[TWISTING_COLUMN_SAVED_TARGET].velocityY;
                            btEffectStartActorMotion(
                                args->targets[0], 1, toFIXED(2), toFIXED(8), toFIXED(4), 150);
                        }
                    }
                    else if (frame <= 19)
                    {
                        target->verticalMotionStrength = 0;
                        target->x +=
                            (djinniPlace[0] - state->particles[TWISTING_COLUMN_SAVED_TARGET].x)
                            / TWISTING_COLUMN_DRIFT_STEPS;
                        target->y +=
                            (djinniPlace[1] - state->particles[TWISTING_COLUMN_SAVED_TARGET].y)
                            / TWISTING_COLUMN_DRIFT_STEPS;
                        target->z +=
                            (djinniPlace[2] - state->particles[TWISTING_COLUMN_SAVED_TARGET].z)
                            / TWISTING_COLUMN_DRIFT_STEPS;
                    }
                    else if (frame <= 31
                             && (args->variant == EFFECT_VARIANT_STRONGER || frame <= 23))
                    {
                        target->x +=
                            (djinniPlace[0] - state->particles[TWISTING_COLUMN_SAVED_TARGET].x)
                            / TWISTING_COLUMN_LIFT_STEPS;
                        target->y +=
                            (djinniPlace[1] - state->particles[TWISTING_COLUMN_SAVED_TARGET].y)
                            / TWISTING_COLUMN_LIFT_STEPS;
                        target->z +=
                            (djinniPlace[2] - state->particles[TWISTING_COLUMN_SAVED_TARGET].z)
                            / TWISTING_COLUMN_LIFT_STEPS;
                        target->scaleX +=
                            (toFIXED(1) / 4
                             - state->particles[TWISTING_COLUMN_SAVED_TARGET].velocityX)
                            / TWISTING_COLUMN_LIFT_STEPS;
                        target->scaleY +=
                            (toFIXED(1) / 4
                             - state->particles[TWISTING_COLUMN_SAVED_TARGET].velocityY)
                            / TWISTING_COLUMN_LIFT_STEPS;
                    }
                }
                else if (mode == TWISTING_COLUMN_GALE)
                {
                    target->verticalMotionStrength = 0;
                    if (frame > 55)
                    {
                        target->y = toFIXED(144);
                    }
                    else if (frame <= 31)
                    {
                        if (target->x > 0)
                        {
                            target->x += toFIXED(1) / 2;
                        }
                        else
                        {
                            target->x -= toFIXED(1) / 2;
                        }
                    }
                    else
                    {
                        if (target->x > 0)
                        {
                            target->x += toFIXED(16);
                        }
                        else
                        {
                            target->x -= toFIXED(16);
                        }
                    }
                }
            }
            if (frame > 0 && mode <= TWISTING_COLUMN_GALE)
            {
                {
                    PARTICLE *mote = state->particles;

                    for (i = 0; i != TWISTING_COLUMN_MOTES; i++, mote++)
                    {
                        if (frame >= i && frame < i + 30)
                        {
                            Sint32 size = (i & 3) + 1;

                            if (mode == TWISTING_COLUMN_WHORL)
                            {
                                mote->x += (djinniPlace[0] - mote->velocityX) / 32;
                                mote->y += (djinniPlace[1] - mote->velocityY) / 32;
                                mote->z += (djinniPlace[2] - mote->velocityZ) / 32;
                            }
                            else
                            {
                                btParticleAdvanceWithGravity3D(mote, 62, toFIXED(1) / 64);
                            }
                            btEffectPosProjectPoint(mote, &moteScreen);
                            EFFECT_DRAW_MOTE(
                                blit[0], canvas, sprites, moteScreen.x / 2, moteScreen.y, size);
                        }
                    }
                }
            }

            if (mode == TWISTING_COLUMN_LETHE_ALBION || mode == TWISTING_COLUMN_RAGING_FLOOD)
            {
                for (j = 0; j != TWISTING_COLUMN_ROWS; j++)
                {
                    Sint32 ripple = j;

                    if (ripple > TWISTING_COLUMN_BAND_RIPPLE)
                    {
                        ripple = TWISTING_COLUMN_BAND_RIPPLE;
                    }
                    for (i = 0; i != 2; i++)
                    {
                        Sint32 width = TWISTING_COLUMN_BAND_WIDTH;

                        TWISTING_COLUMN_CORNERS[j][i][0] =
                            -j * 4 + TWISTING_COLUMN_BAND_HALF_LENGTH;
                        TWISTING_COLUMN_CORNERS[j][i][1] =
                            (FIXEDtoINT(syCos(j * (TRIG_TURN / 64) - frame * (TRIG_TURN / 32))
                                        * ripple))
                            + i * width - width / 2;
                        TWISTING_COLUMN_CORNERS[j][i][2] = 0;
                    }
                }
            }
            else if (mode == TWISTING_COLUMN_POISON_FLOW)
            {
                for (j = 0; j != TWISTING_COLUMN_ROWS; j++)
                {
                    Sint32 ripple = j * 4;
                    Sint32 w      = j * 2 + 4;

                    if (ripple > TWISTING_COLUMN_STREAM_RIPPLE)
                    {
                        ripple = TWISTING_COLUMN_STREAM_RIPPLE;
                    }
                    for (i = 0; i != 2; i++)
                    {
                        TWISTING_COLUMN_CORNERS[j][i][0] = -j * 3;
                        TWISTING_COLUMN_CORNERS[j][i][1] =
                            (FIXEDtoINT(syCos(j * (TRIG_TURN / 64) - frame * (TRIG_TURN / 32))
                                        * ripple))
                            + (i * w << 1) - w;
                        TWISTING_COLUMN_CORNERS[j][i][2] = 0;
                    }
                }
            }
            else
            {
                for (j = 0; j != TWISTING_COLUMN_ROWS; j++)
                {
                    Sint32 ripple = j * 3;
                    Sint32 w      = j * 2;

                    if (ripple > TWISTING_COLUMN_FUNNEL_RIPPLE)
                    {
                        ripple = TWISTING_COLUMN_FUNNEL_RIPPLE;
                    }
                    if (w > TWISTING_COLUMN_FUNNEL_HALF_WIDTH)
                    {
                        w = TWISTING_COLUMN_FUNNEL_HALF_WIDTH;
                    }
                    for (i = 0; i != 2; i++)
                    {
                        TWISTING_COLUMN_CORNERS[j][i][0] = -(j * 2) << 1;
                        TWISTING_COLUMN_CORNERS[j][i][1] =
                            (FIXEDtoINT(syCos((frame + j * 2) * (TRIG_TURN / 64)) * ripple))
                            + (i * w << 1) - w;
                        TWISTING_COLUMN_CORNERS[j][i][2] = 0;
                    }
                }
            }

            model = btRenderAllocateModelList(1);
            // The bands fade in four levels a tick from MODEL_SHADE_INVISIBLE to their brightest,
            // Lethe Albion's TWISTING_COLUMN_BAND_SHADE and Raging Flood's
            // TWISTING_COLUMN_FLOOD_SHADE. From TWISTING_COLUMN_FADE_TICK Lethe Albion's fade out
            // eight levels a tick; Raging Flood's count up from beyond their brightest, and so stay
            // at it.
            if (mode == TWISTING_COLUMN_RAGING_FLOOD)
            {
                if (frame < TWISTING_COLUMN_FADE_TICK)
                {
                    shade = frame * 4 - 64;
                }
                else
                {
                    shade = (frame - (TWISTING_COLUMN_FADE_TICK + 1)) * 8;
                }
                if (shade > TWISTING_COLUMN_FLOOD_SHADE)
                {
                    shade = TWISTING_COLUMN_FLOOD_SHADE;
                }
            }
            else
            {
                if (frame < TWISTING_COLUMN_FADE_TICK)
                {
                    shade = frame * 4 - 64;
                }
                else
                {
                    shade = TWISTING_COLUMN_BAND_SHADE - (frame - TWISTING_COLUMN_FADE_TICK) * 8;
                }
                if (shade > TWISTING_COLUMN_BAND_SHADE)
                {
                    shade = TWISTING_COLUMN_BAND_SHADE;
                }
            }
            if (shade < MODEL_SHADE_INVISIBLE)
            {
                shade = -64;
            }
            if (mode == TWISTING_COLUMN_LETHE_ALBION || mode == TWISTING_COLUMN_RAGING_FLOOD)
            {
                texture.logWidth = 5;
            }
            else
            {
                texture.logWidth = 7;
            }
            texture.logHeight = 5;
            texture.pixels    = state->sheet;
            model->kind       = MODEL_SHADED_ADDITIVE;
            model->texture    = &texture;
            model->mesh       = mesh;
            model->vertices   = vertices;
            model->shade      = shade;
            if (mode == TWISTING_COLUMN_LETHE_ALBION || mode == TWISTING_COLUMN_RAGING_FLOOD)
            {
                btEffectPosProjectBattler(args->targets[0], &targetScreen);
                if (frame == 16)
                {
                    EFFECT_SHOW_DAMAGE(SE_CLATTER);
                    btEffectStartActorMotion(args->targets[0], 1, 0, 0, toFIXED(7), 100);
                    state->shakeFrame = 8;
                    btEffectTintBattler(args->targets[0], SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 8);
                }
                if (frame == 0)
                {
                    {
                        PARTICLE *spray = state->particles;

                        for (i = 0; i != TWISTING_COLUMN_SAVED_TARGET; i++, spray++)
                        {
                            Sint32 speed     = (syRandom() & 255) + 256;
                            Sint32 angle     = (syRandom() & 0x7fff) + DEGtoANG(90);

                            spray->velocityX = (sySin(angle) * speed) >> 7;
                            spray->velocityY = (syCos(angle) * speed) >> 7;
                            spray->x         = toFIXED(targetScreen.x / 2);
                            spray->y         = INTtoFIXED(targetScreen.y - 8);
                            spray->variant   = 0;
                        }
                    }
                }
                // Clouds of spray: FlashBurstSheet's bursts in the water spray's colours, four
                // ticks a picture.
                if (frame > 15)
                {
                    {
                        PARTICLE *spray = state->particles;

                        for (i = 0; i != 8; i++, spray++)
                        {
                            if (spray->variant < FLASH_BURST_PICTURE_COUNT * 4)
                            {
                                blit[0](canvas,
                                        (RamMapMetatileBuffer + TWISTING_COLUMN_FLASH_BURST)
                                            + spray->variant / 4 * PICTURE_BYTES(FLASH_BURST),
                                        (Sint16)(FIXEDtoINT(spray->x)) - FLASH_BURST_WIDTH / 2,
                                        (Sint16)(FIXEDtoINT(spray->y)) - FLASH_BURST_HEIGHT / 2,
                                        FLASH_BURST_WIDTH,
                                        FLASH_BURST_HEIGHT);
                                btParticleAdvanceWithGravity2D(spray, 60, -toFIXED(1) / 8);
                                spray->variant++;
                            }
                        }
                    }
                }
                model->scrollU = ~(frame << 3) & TEXTURE_SCROLL_MASK;
                model->scrollV = 0;
                for (i = 0; i != 3; i++)
                {
                    Sint32 y = TWISTING_COLUMN_WAVE_RISE
                             - FIXEDtoINT(sySin(frame * TWISTING_COLUMN_WAVE_RISE_STEP)
                                          * TWISTING_COLUMN_WAVE_RISE);

                    syPushMatrix();
                    syTranslate(0, toFIXED(y - i * 8) + toFIXED(48), 0);
                    if (args->side == EFFECT_SIDE_PARTY)
                    {
                        syRotY(DEGtoANG(180));
                    }
                    syRotZ(i * (TRIG_TURN / 64) + TRIG_TURN / 256);
                    syRotX(i * (TRIG_TURN / 8) - frame * (TRIG_TURN / 32));
                    syScale(TWISTING_COLUMN_BAND_STRETCH, toFIXED(1), toFIXED(1));
                    btRenderTransformVertices(
                        MapGridMetatileBuffer, vertices, TWISTING_COLUMN_VERTICES);
                    btRenderDrawModelList(model);
                    syPopMatrix();
                }
                texture.logWidth  = 6;
                texture.logHeight = 6;
                model->mesh       = BattleEffectSquareMesh;
                model->vertices   = vertices;
                model->shade      = shade;
                model->scrollU    = 0;
                model->scrollV    = 0;
                texture.pixels    = sheet;
                for (i = 0; i != 2; i++)
                {
                    Sint32 age = frame - i * 4 - 16;

                    if (age >= 0)
                    {
                        Sint32 scale = age * 3 * (toFIXED(1) / 16);

                        shade        = 0;
                        if (age > 7)
                        {
                            shade = (8 - age) * 8;
                        }
                        model->shade = shade;
                        if (shade > MODEL_SHADE_INVISIBLE)
                        {
                            syInitMatrix();
                            syTranslate(INTtoFIXED(targetScreen.x / 2 - 64),
                                        INTtoFIXED(targetScreen.y - 64),
                                        0);
                            syScale(toFIXED(1) / 2, toFIXED(1), toFIXED(1) / 2);
                            syRotZ(i * (DEGtoANG(90)) + DEGtoANG(45));
                            syRotX(-TRIG_TURN / 16);
                            syRotY(frame * (TRIG_TURN / 16));
                            syMatrixScaleUniform(scale);
                            btRenderTransformVertices(BattleEffectGroundSquareCorners, vertices, 4);
                            btRenderDrawModelList(model);
                        }
                    }
                }
            }
            else if (mode == TWISTING_COLUMN_POISON_FLOW)
            {
                btEffectPosProjectScaledBattler(args->user, &casterScreen);
                if (frame == 8)
                {
                    sySoundPlayStub(SE_CLATTER);
                }
                if (frame == 16)
                {
                    EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
                }
                for (i = 0; i != args->targetCount; i++)
                {
                    if (frame == i * 4 + 16)
                    {
                        sySoundPlayStub(SE_HEAVY_IMPACT);
                        btEffectStartActorMotion(args->targets[i], 1, 0, 0, toFIXED(7), 100);
                        btEffectTintBattler(
                            args->targets[i], SPRITE_COLOR_WHITE, POSE_DAMAGE, i, 8);
                        state->shakeFrame = 8;
                    }
                }
                model->scrollV = 0;
                // A once-only loop: its i terms are the other modes' ring index, here always 0.
                for (i = 0; i != 1; i++)
                {
                    Sint32 scroll   = ~((frame + i * 6) << 3);

                    scroll         &= TEXTURE_SCROLL_MASK;
                    model->scrollU  = scroll;
                    syInitMatrix();
                    syTranslate(INTtoFIXED(casterScreen.x / 2 - 64),
                                toFIXED(casterScreen.y) - toFIXED(52),
                                0);
                    if (args->side == EFFECT_SIDE_PARTY)
                    {
                        syRotY(DEGtoANG(180));
                    }
                    syRotX(i * (TRIG_TURN / 2));
                    syRotZ(
                        FIXEDtoINT(sySin((frame + i * 8) * (TRIG_TURN / 32)) * (TRIG_TURN / 32)));
                    syScale(TWISTING_COLUMN_SWIRL_STRETCH, toFIXED(1) / 2, toFIXED(1));
                    btRenderTransformVertices(
                        MapGridMetatileBuffer, vertices, TWISTING_COLUMN_VERTICES);
                    btRenderDrawModelList(model);
                }
            }
            else
            {
                // The funnel and Poison Flow's stream fade in alike, and out from
                // TWISTING_COLUMN_FUNNEL_FADE_TICK.
                if (frame < TWISTING_COLUMN_FUNNEL_FADE_TICK)
                {
                    shade = frame * 4 - 64;
                }
                else
                {
                    shade =
                        TWISTING_COLUMN_BAND_SHADE - (frame - TWISTING_COLUMN_FUNNEL_FADE_TICK) * 8;
                }
                if (shade > TWISTING_COLUMN_BAND_SHADE)
                {
                    shade = TWISTING_COLUMN_BAND_SHADE;
                }
                if (shade > MODEL_SHADE_INVISIBLE)
                {
                    model->shade = shade;
                    syScale(toFIXED(1) / 2, toFIXED(1), toFIXED(1));
                    ProjectionWork.centerX = 60;
                    if (mode == TWISTING_COLUMN_WHORL)
                    {
                        model->scrollU = (frame << 3) & TEXTURE_SCROLL_MASK;
                    }
                    else
                    {
                        model->scrollU = ~(frame << 3) & TEXTURE_SCROLL_MASK;
                    }
                    model->scrollV = 0;
                    syMatrixTranslateVector(djinniPlace);
                    syTranslate(0, toFIXED(16), 0);
                    for (i = 0; i != 3; i++)
                    {
                        syPushMatrix();
                        if (args->side == EFFECT_SIDE_PARTY)
                        {
                            syRotY(DEGtoANG(180));
                        }
                        syRotZ(i * (TRIG_TURN / 64) + TWISTING_COLUMN_LEAN);
                        if (mode != TWISTING_COLUMN_GALE)
                        {
                            syRotX((frame + i * 4) * (TRIG_TURN / 32));
                        }
                        else
                        {
                            syRotX(i * (TRIG_TURN / 8) - frame * (TRIG_TURN / 32));
                        }
                        syScale(toFIXED(4), toFIXED(1), toFIXED(1));
                        btRenderProjectVertices(
                            MapGridMetatileBuffer, vertices, TWISTING_COLUMN_VERTICES);
                        btRenderDrawModelList(model);
                        syPopMatrix();
                    }
                }
                ProjectionWork.centerX = BATTLE_PROJECTION_CENTER_X;
            }
            syHeapRelease(model);
            if (mode == TWISTING_COLUMN_LETHE_ALBION || mode == TWISTING_COLUMN_RAGING_FLOOD)
            {
                btCameraShake(8, 8);
            }
            else
            {
                btCameraShake(4, 4);
            }
            EFFECT_END_TICK(state);
            frame++;
        } while (frame != ticks);
    }

    if (unit->classId != CLASS_NONE && mode == TWISTING_COLUMN_WHORL
        && args->variant == EFFECT_VARIANT_STRONGER)
    {
        target->x      = state->particles[TWISTING_COLUMN_SAVED_TARGET].x;
        target->y      = state->particles[TWISTING_COLUMN_SAVED_TARGET].y;
        target->z      = state->particles[TWISTING_COLUMN_SAVED_TARGET].z;
        target->scaleX = state->particles[TWISTING_COLUMN_SAVED_TARGET].velocityX;
        target->scaleY = state->particles[TWISTING_COLUMN_SAVED_TARGET].velocityY;
    }
    syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
    syCloseEvent(btEffectTransferCanvas);
    btEffectEndCanvasLayer();
}

// Abilities: Core.
void btEffectRunCore(BATTLE_EFFECT_PARM *args)
{
    btEffectRunDjinniCharge(args, DJINNI_CHARGE_CORE);
}

// Abilities: Cannon.
void btEffectRunCannon(BATTLE_EFFECT_PARM *args)
{
    btEffectRunDjinniCharge(args, DJINNI_CHARGE_CANNON);
}

#undef TWISTING_COLUMN_DJINN_TICKS
#undef TWISTING_COLUMN_TICKS
#undef TWISTING_COLUMN_CORNERS
#undef TWISTING_COLUMN_QUADS
#undef TWISTING_COLUMN_ROWS
#undef TWISTING_COLUMN_VERTICES
#undef TWISTING_COLUMN_TRANSFORMED
#undef TWISTING_COLUMN_MESH
#undef TWISTING_COLUMN_SHEET
#undef TWISTING_COLUMN_JUPITER_DJINN_SMALL_PICTURES
#undef TWISTING_COLUMN_FLASH_BURST
#undef TWISTING_COLUMN_SAVED_TARGET
#undef TWISTING_COLUMN_MOTES
#undef TWISTING_COLUMN_BOB_STEP
#undef TWISTING_COLUMN_DJINNI_SWAY
#undef TWISTING_COLUMN_DRIFT_STEPS
#undef TWISTING_COLUMN_LIFT_STEPS
#undef TWISTING_COLUMN_BAND_HALF_LENGTH
#undef TWISTING_COLUMN_BAND_WIDTH
#undef TWISTING_COLUMN_BAND_RIPPLE
#undef TWISTING_COLUMN_STREAM_RIPPLE
#undef TWISTING_COLUMN_FUNNEL_HALF_WIDTH
#undef TWISTING_COLUMN_FUNNEL_RIPPLE
#undef TWISTING_COLUMN_WAVE_RISE
#undef TWISTING_COLUMN_WAVE_RISE_STEP
#undef TWISTING_COLUMN_LEAN
#undef TWISTING_COLUMN_BAND_STRETCH
#undef TWISTING_COLUMN_SWIRL_STRETCH
#undef TWISTING_COLUMN_BAND_SHADE
#undef TWISTING_COLUMN_FLOOD_SHADE
#undef TWISTING_COLUMN_FADE_TICK
#undef TWISTING_COLUMN_FUNNEL_FADE_TICK

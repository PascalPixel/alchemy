// Rising Dragon and Heartbreak. Part of battle_effect.c.

// On every strike after the first, the target's motion goes to this percent of its anchor's x,
// lowered in every edition but the Japanese.
#define TARGET_ANCHOR_PERCENT  WHEN(RISING_DRAGON_SHORTER_KNOCKBACK, 200, 300)

#define DRAGON_SEGMENTS        33
// The corners of one dragon's strip, two to a segment.
#define DRAGON_CORNERS         (DRAGON_SEGMENTS * 2)
// The sparks each strike scatters.
#define DRAGON_SPARKS          32
// How far round its arc a dragon's head and tail travel, DRAGON_ARC_SPEED a tick.
#define DRAGON_ARC_END         950
#define DRAGON_ARC_SPEED       44
// The strips' widths, as a fraction of their rows' widths, and the heads' scale.
#define DRAGON_SCALE           toFIXED(0.3169)
// How far the second dragon's arc is turned from the first's.
#define DRAGON_SECOND_TURN     14192
// A head's turn at the start of its arc, less 32 for each unit of the arc.
#define DRAGON_HEAD_TURN       12384
// The dragons' buffers in the map cell buffer: the strips' corners, then their transformed
// vertices, then the strip mesh.
#define RISING_DRAGON_VERTICES 4096
#define RISING_DRAGON_STRIP    8192

// A strip's vertex, as btRenderTransformVertices reads it.
typedef struct STRIP_VERTEX
{
    Sint8 x;
    Sint8 y;
    Sint8 z;
    Sint8 pad;
} STRIP_VERTEX;

// Where ClawSlashSheet, a dragon's head of flame, is decoded in the effect's pictures.
#define RISING_DRAGON_FLAME_DRAGON_HEAD_PICTURES 16384

// A dragon of blue flame (two at the next variant) rises from the caster and arcs onto the target:
// a scrolling band of flame of DRAGON_SEGMENTS segments, widening to its head, a dragon's head of
// flame. Each strikes ten ticks after the last, scattering DRAGON_SPARKS sparks.
//
// Abilities: Rising Dragon (the Masamune's unleash).
void btEffectRunRisingDragon(BATTLE_EFFECT_PARM *args)
{
    BATTLE_EFFECT_STATE *state;
    void                *canvas;
    DRAW_RECTANGLE       blit[2];
    Sint32               frame;
    BATTLE_CAMERA       *camera;
    Uint8               *sprites;
    void                *caster;
    Sint32               arcs[4];
    SCREEN_POINT         src;
    SCREEN_POINT         target;
    SCREEN_POINT         hit;
    MODEL_TEXTURE        texture;
    RENDER_MODEL        *model;
    void                *vertices;
    PARTICLE            *spark;
    Sint32               i;
    Sint32               j;

    state   = RamHeapSlots->battleEffect;
    canvas  = RamHeapSlots->battleCanvas;
    camera  = RamHeapSlots->cameraState;
    sprites = RamHeapSlots->battleSheet;
    btEffectStartCanvasLayer(BGCNT_PRIORITY(1));
    if (args->side == EFFECT_SIDE_ENEMIES)
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
    {
        Sint32 endX[2] = {0, 0};
        Sint32 endY[2] = {0, 0};
        caster         = btObjectGetStub(args->user)->object;
        fdActorSetAnimationSpeedStub(caster, ANIM_STEP_NORMAL * 3);
        fdActorSelectAnimationStub(caster, POSE_LEAP);
        btEffectPosProjectBattler(args->user, &src);
        btEffectPosProjectBattler(args->targets[0], &target);
        btResourceLoadAndDecompress(RESOURCE_SMOKE_BAND_SHEET, state->sheet, 1, 1);
        btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sprites, 0, 0);
        btResourceLoadAndDecompress(RESOURCE_CLAW_SLASH_SHEET,
                                    state->sheet + RISING_DRAGON_FLAME_DRAGON_HEAD_PICTURES,
                                    1,
                                    1);
        syWordsCopyEntry((void *)BG_PLTT,
                         syResourceGetTableEntry(RESOURCE_BLUE_FLAME_SHEET),
                         SHEET_PALETTE_BYTES);
        REG_BG2PA = Q_8_8(0.5);
        btRenderBuildQuadStrip(
            MapGridMetatileBuffer + RISING_DRAGON_STRIP, 4, 32, DRAGON_SEGMENTS - 1);

        for (frame = 0; frame != args->variant * 10 + 60; frame++)
        {
            EFFECT_VIEW_FROM_CAMERA(camera);
            if (frame == 0)
            {
                arcs[0] = 0;
                arcs[1] = 0;
                arcs[2] = 0;
                arcs[3] = 0;
                EFFECT_FREE_PARTICLES(i, state->particles, ARRAY_COUNT(state->particles));
            }

            // Each dragon's strike.
            for (i = 0; i != args->variant + 1; i++)
            {
                if (frame != i * 10 + 28)
                {
                    continue;
                }
                btEffectPosProjectBattler(args->targets[0], &hit);
                for (j = 0; j != DRAGON_SPARKS; j++)
                {
                    Sint32 angle;
                    Sint32 speed;

                    spark            = &state->particles[i * DRAGON_SPARKS + j];

                    spark->x         = toFIXED(hit.x / 2);
                    spark->y         = INTtoFIXED(hit.y - 16);
                    angle            = syRandom();
                    speed            = (syRandom() & 255) + 127;
                    spark->velocityX = (sySin(angle) * speed) >> 6;
                    spark->velocityY = (syCos(angle) * speed) >> 6;
                    spark->variant   = (syRandom() & 15) + 16;
                }
                syWordsFillEntry(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
                state->shakeFrame = 8;
                if (frame == 28)
                {
                    if (args->variant == EFFECT_VARIANT_BASIC)
                    {
                        EFFECT_SHOW_DAMAGE(SE_CLATTER);
                    }
                    else
                    {
                        sySoundPlayStub(SE_CLATTER);
                    }
                    btEffectStartActorMotion(
                        args->targets[0], 1, toFIXED(2), toFIXED(4), toFIXED(4), 150);
                }
                else
                {
                    EFFECT_SHOW_DAMAGE(SE_CLATTER);
                    btEffectStartActorMotion(args->targets[0],
                                             1,
                                             toFIXED(2),
                                             toFIXED(4),
                                             toFIXED(7),
                                             TARGET_ANCHOR_PERCENT);
                }
                btEffectTintBattler(args->targets[0], SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 8);
            }
            for (i = 0; i != ARRAY_COUNT(state->particles); i++)
            {
                spark = &state->particles[i];
                if (spark->variant >= 0)
                {
                    Sint32 size = (spark->variant >> 3) + 2;

                    blit[0](canvas,
                            sprites + BattleEffectMotePictures[size - 1],
                            FIXED_INTEGER_PART(spark->x) - size / 2,
                            FIXED_INTEGER_PART(spark->y) - size,
                            size,
                            size * 2);
                    btParticleAdvanceWithGravity2D(spark, 60, toFIXED(1) / 16);
                    spark->variant--;
                }
            }

            if (frame > 15)
            {
                STRIP_VERTEX *stripA    = (STRIP_VERTEX *)MapGridMetatileBuffer;
                Sint32        scale     = DRAGON_SCALE;
                const Uint8  *stripMesh = MapGridMetatileBuffer + RISING_DRAGON_STRIP;

                // The arcs' heads and tails: the first dragon's head sets out at tick 16 and its
                // tail at 32, the second's at 30 and 46.
                arcs[0]                 = (frame - 16) * DRAGON_ARC_SPEED;
                if (arcs[0] > DRAGON_ARC_END)
                {
                    arcs[0] = DRAGON_ARC_END;
                }
                if (frame > 31)
                {
                    arcs[1] = (frame - 32) * DRAGON_ARC_SPEED;
                    if (arcs[1] > DRAGON_ARC_END)
                    {
                        arcs[1] = DRAGON_ARC_END;
                    }
                }
                if (frame > 29)
                {
                    arcs[2] = (frame - 30) * DRAGON_ARC_SPEED;
                    if (arcs[2] > DRAGON_ARC_END)
                    {
                        arcs[2] = DRAGON_ARC_END;
                    }
                }
                if (frame > 45)
                {
                    arcs[3] = (frame - 46) * DRAGON_ARC_SPEED;
                    if (arcs[3] > DRAGON_ARC_END)
                    {
                        arcs[3] = DRAGON_ARC_END;
                    }
                }

                // The first dragon arcs over, the second under.
                for (j = 0; j != DRAGON_SEGMENTS; j++)
                {
                    Sint32 distance = src.x / 4 - target.x / 4;
                    Sint32 angle    = j * (arcs[0] - arcs[1]) + arcs[1] * DRAGON_SEGMENTS;
                    Sint32 height;
                    Sint32 width;
                    Sint32 start;

                    if (distance < 0)
                    {
                        distance = -distance;
                    }
                    height = distance / 3;
                    width  = j * 2 + 16;
                    if (width > 32)
                    {
                        width = 32;
                    }
                    width = width * scale / (toFIXED(1));
                    start = -(width / 2);
                    for (i = 0; i != 2; i++)
                    {
                        STRIP_VERTEX *corner = stripA + (j * 2 + i);

                        corner->x =
                            (FIXEDtoINT(syCos(angle) * (distance + start + i * width))) - distance;
                        corner->y = -(FIXEDtoINT(sySin(angle) * (height + start + i * width)));
                        corner->z = 0;
                    }
                    if (j == DRAGON_SEGMENTS - 1)
                    {
                        endX[0] = (FIXEDtoINT(syCos(angle) * distance)) - distance;
                        endY[0] = -(FIXEDtoINT(sySin(angle) * height));
                    }
                }
                for (j = 0; j != DRAGON_SEGMENTS; j++)
                {
                    Sint32 distance = src.x / 4 - target.x / 4;
                    Sint32 angle    = j * (arcs[2] - arcs[3]) + arcs[3] * DRAGON_SEGMENTS;
                    Sint32 height;
                    Sint32 width;
                    Sint32 start;

                    if (distance < 0)
                    {
                        distance = -distance;
                    }
                    height = distance * 32 / 36;
                    width  = j * 4;
                    if (width > 24)
                    {
                        width = 24;
                    }
                    width = width * scale / (toFIXED(1));
                    start = -(width / 2);
                    for (i = 0; i != 2; i++)
                    {
                        STRIP_VERTEX *stripB =
                            (STRIP_VERTEX *)RamMapMetatileBuffer + DRAGON_CORNERS;
                        STRIP_VERTEX *corner = stripB + (j * 2 + i);

                        corner->x =
                            (FIXEDtoINT(syCos(angle) * (distance + start + i * width))) - distance;
                        corner->y = FIXEDtoINT(sySin(angle) * (height + start + i * width));
                        corner->z = 0;
                    }
                    if (j == DRAGON_SEGMENTS - 1)
                    {
                        endX[1] = (FIXEDtoINT(syCos(angle) * distance)) - distance;
                        endY[1] = FIXEDtoINT(sySin(angle) * height);
                    }
                }

                model           = btRenderAllocateModelList(1);
                vertices        = MapGridMetatileBuffer + RISING_DRAGON_VERTICES;
                model->texture  = &texture;
                model->shade    = 0;
                model->kind     = MODEL_SHADED_MAX;
                model->scrollV  = 0;
                model->vertices = vertices;
                for (i = 0; i != args->variant + 1; i++)
                {
                    if (arcs[i * 2 + 1] >= arcs[i * 2])
                    {
                        continue;
                    }
                    texture.logWidth  = 6;
                    texture.logHeight = 5;
                    texture.pixels    = state->sheet;
                    model->mesh       = stripMesh;
                    model->scrollU    = TEXTURE_SCROLL_MASK & ~(frame << 3);
                    syInitMatrix();
                    if (args->side == EFFECT_SIDE_ENEMIES)
                    {
                        syTranslate(INTtoFIXED(src.x / 2 - 68), toFIXED(src.y) - toFIXED(96), 0);
                    }
                    else
                    {
                        syTranslate(INTtoFIXED(src.x / 2 - 52), toFIXED(src.y) - toFIXED(96), 0);
                    }
                    syScale(toFIXED(1) / 2, toFIXED(1), toFIXED(1) / 2);
                    if (args->side == EFFECT_SIDE_PARTY)
                    {
                        syRotY(DEGtoANG(180));
                    }
                    syRotZ(i * DRAGON_SECOND_TURN - TRIG_TURN / 16);
                    syMatrixScaleUniform(toFIXED(4));
                    btRenderTransformVertices(
                        stripA + i * DRAGON_CORNERS, vertices, DRAGON_CORNERS);
                    btRenderDrawModelList(model);
                    if (frame < i * 16 + 40)
                    {
                        texture.logWidth  = 6;
                        texture.logHeight = 6;
                        model->scrollU    = 0;
                        model->mesh       = BattleEffectSquareMesh;
                        texture.pixels    = state->sheet + RISING_DRAGON_FLAME_DRAGON_HEAD_PICTURES;
                        syTranslate(toFIXED(endX[i]), toFIXED(endY[i]), 0);
                        syRotZ(DRAGON_HEAD_TURN - (arcs[i * 2] << 5));
                        syMatrixScaleUniform(DRAGON_SCALE);
                        btRenderTransformVertices(BattleEffectSquareCorners, vertices, 4);
                        btRenderDrawModelList(model);
                    }
                }
                syHeapRelease(model);
            }
            btEffectCountDownBattlerTints();
            btCameraShake(2, 2);
            EFFECT_PRESENT_CANVAS(state);
        }
        fdActorSetAnimationSpeedStub(caster, ANIM_STEP_NORMAL);
        syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
        syCloseEvent(btEffectTransferCanvas);
    }
    btEffectEndCanvasLayer();
}

#define FIGURE_TICKS               108
// The burst at its hand flashes the screen.
#define FIGURE_SPARK_BURST_TICK    30
#define FIGURE_HIT_TICK            80 // the blow, which starts the result phase

// The parts of the figure in BattleEffectHeartbreakSheet, unpacked into the map cells.
#define FIGURE_BODY                0
#define FIGURE_BODY_WIDTH          96
#define FIGURE_BODY_HEIGHT         83
#define FIGURE_ARM_CLENCHED        7968
#define FIGURE_ARM_CLENCHED_WIDTH  42
#define FIGURE_ARM_CLENCHED_HEIGHT 35
#define FIGURE_ARM_REACHING        9438
#define FIGURE_ARM_REACHING_WIDTH  48
#define FIGURE_ARM_REACHING_HEIGHT 51
// A burst of sparks growing, four pictures (HeartbreakSparkBurstPictures).
#define FIGURE_SPARK_BURST         11886
// A heart, then seven pictures of green flame burning it away (HeartbreakHeartBurnPictures); the
// first is also the heart held up.
#define FIGURE_HEART               20125
#define FIGURE_HEART_WIDTH         32
#define FIGURE_HEART_HEIGHT        29
// The figure sways on a circle of this radius, a 256th of a turn a tick.
#define FIGURE_SWAY                8

// A great cloaked figure with a horned skull for a head sweeps down on the target from above the
// far side, swaying and fading in through the blend, its clawed hand reaching out. From tick 28 a
// burst of sparks grows at its hand and the screen flashes white; from tick 40 a heart is held up,
// from tick 48 the hand closes into a fist, and from tick 62 green flame burns the heart away. At
// tick 80 the blow lands and the result phase starts; from tick 92 the figure fades out and rises
// away, and the battle background is reloaded at the end. On the far side the whole figure is drawn
// mirrored.
//
// Abilities: Heartbreak (the Thanatos Mace's unleash).
void btEffectRunHeartbreak(BATTLE_EFFECT_PARM *args)
{
    HEAP_SLOTS          *heap;
    BATTLE_EFFECT_STATE *state;
    Uint8               *canvas;
    Uint8               *parts;
    DRAW_RECTANGLE       draw[2];
    SCREEN_POINT         target;
    Sint32               frame;
    Sint32               x;
    Sint32               y;

    heap   = RamHeapSlots;
    state  = (BATTLE_EFFECT_STATE *)heap->battleEffect;
    canvas = heap->battleCanvas;
    parts  = RamMapMetatileBuffer;
    btEffectStartTiledCanvas(BGCNT_WRAP | BGCNT_PRIORITY(1));
    REG_BG2PA = Q_8_8(1);
    btResourceLoadAndDecompress(RESOURCE_HEARTBREAK_SHEET, parts, 1, 1);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_TRANSPARENT);
    draw[0] = heap->ramCode;
    EFFECT_LOAD_BLITTER(
        HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_TRANSPARENT);
    draw[1] = heap->ramCodePaired;
    EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
    btEffectPosProjectScaledBattler(args->targets[0], &target);

    for (frame = 0; frame != FIGURE_TICKS; frame++)
    {
        // It starts 100 pixels above the canvas and to the target's far side, and comes down at it
        // for 32 ticks.
        if (frame == 0)
        {
            if (args->side == EFFECT_SIDE_ENEMIES)
                state->particles[0].x = INTtoFIXED(target.x + 40);
            else
                state->particles[0].x = INTtoFIXED(target.x - 168);
            state->particles[0].y = -toFIXED(100);
            if (args->side == EFFECT_SIDE_ENEMIES)
                state->particles[0].velocityX = -toFIXED(6);
            else
                state->particles[0].velocityX = toFIXED(6);
            state->particles[0].velocityY = toFIXED(8);
            REG_BLDALPHA                  = EFFECT_BLEND_CANVAS(0);
        }
        if (frame < 32)
            btParticleAdvanceWithGravity2D(&state->particles[0], 60, 0);

        // In over 32 ticks, out over the last 16, rising away.
        if (frame <= 32)
        {
            Sint32 level = frame / 2;

            REG_BLDALPHA = ((BLEND_WEIGHT_MAX - level / 2) << 8) | level;
        }
        if (frame > 91)
        {
            Sint32 level = FIGURE_TICKS - frame;

            REG_BLDALPHA = ((BLEND_WEIGHT_MAX - level / 2) << 8) | level;
            btParticleAdvanceWithGravity2D(&state->particles[0], 60, -toFIXED(1) / 2);
        }

        // The sway, and the canvas moved with the figure.
        x = FIXED_INTEGER_PART(state->particles[0].x)
          + (FIXEDtoINT(sySin(frame * (TRIG_TURN / 256)) * FIGURE_SWAY));
        y = FIXED_INTEGER_PART(state->particles[0].y)
          + (FIXEDtoINT(syCos(frame * (TRIG_TURN / 256)) * FIGURE_SWAY));
        REG_BG2X = -x << 8;

        // The body, and the arm reaching until tick 48, then clenched.
        if (args->side == EFFECT_SIDE_ENEMIES)
        {
            draw[args->side](canvas,
                             RamMapMetatileBuffer + FIGURE_BODY,
                             25,
                             y + 2,
                             FIGURE_BODY_WIDTH,
                             FIGURE_BODY_HEIGHT);
            if ((Uint32)frame < 48)
                draw[args->side](canvas,
                                 RamMapMetatileBuffer + FIGURE_ARM_REACHING,
                                 31,
                                 y + 22,
                                 FIGURE_ARM_REACHING_WIDTH,
                                 FIGURE_ARM_REACHING_HEIGHT);
            else
                draw[args->side](canvas,
                                 RamMapMetatileBuffer + FIGURE_ARM_CLENCHED,
                                 64,
                                 y + 16,
                                 FIGURE_ARM_CLENCHED_WIDTH,
                                 FIGURE_ARM_CLENCHED_HEIGHT);
        }
        else
        {
            draw[args->side](canvas,
                             RamMapMetatileBuffer + FIGURE_BODY,
                             7,
                             y + 2,
                             FIGURE_BODY_WIDTH,
                             FIGURE_BODY_HEIGHT);
            if ((Uint32)frame < 48)
                draw[args->side](canvas,
                                 RamMapMetatileBuffer + FIGURE_ARM_REACHING,
                                 49,
                                 y + 22,
                                 FIGURE_ARM_REACHING_WIDTH,
                                 FIGURE_ARM_REACHING_HEIGHT);
            else
                draw[args->side](canvas,
                                 RamMapMetatileBuffer + FIGURE_ARM_CLENCHED,
                                 22,
                                 y + 16,
                                 FIGURE_ARM_CLENCHED_WIDTH,
                                 FIGURE_ARM_CLENCHED_HEIGHT);
        }

        // The burst of sparks, three ticks a picture from tick 28.
        if (frame >= 28 && frame < 40)
        {
            Sint32 picture = (frame - 28) / 3;

            if (args->side == EFFECT_SIDE_ENEMIES)
            {
                draw[args->side](canvas,
                                 RamMapMetatileBuffer + FIGURE_SPARK_BURST
                                     + HeartbreakSparkBurstPictures[picture],
                                 46 - HeartbreakSparkBurstWidths[picture] / 2,
                                 y - HeartbreakSparkBurstHeights[picture] / 2 + 61,
                                 HeartbreakSparkBurstWidths[picture],
                                 HeartbreakSparkBurstHeights[picture]);
            }
            else
            {
                draw[args->side](canvas,
                                 RamMapMetatileBuffer + FIGURE_SPARK_BURST
                                     + HeartbreakSparkBurstPictures[picture],
                                 HeartbreakSparkBurstWidths[picture] / 2
                                     - HeartbreakSparkBurstWidths[picture] + 82,
                                 y - HeartbreakSparkBurstHeights[picture] / 2 + 61,
                                 HeartbreakSparkBurstWidths[picture],
                                 HeartbreakSparkBurstHeights[picture]);
            }
        }

        // The heart: held up until tick 47, then in the closed fist, where from tick 62 it burns
        // away, a picture every six ticks.
        if (frame > 39)
        {
            if (frame <= 47)
            {
                if (args->side == EFFECT_SIDE_ENEMIES)
                    draw[args->side](canvas,
                                     RamMapMetatileBuffer + FIGURE_HEART,
                                     30,
                                     y + 46,
                                     FIGURE_HEART_WIDTH,
                                     FIGURE_HEART_HEIGHT);
                else
                    draw[args->side](canvas,
                                     RamMapMetatileBuffer + FIGURE_HEART,
                                     66,
                                     y + 46,
                                     FIGURE_HEART_WIDTH,
                                     FIGURE_HEART_HEIGHT);
            }
            else
            {
                Sint32 picture = (frame - 56) / 6;

                if (picture < 0)
                    picture = 0;
                if (picture <= 7)
                {
                    if (args->side == EFFECT_SIDE_ENEMIES)
                        draw[args->side](canvas,
                                         RamMapMetatileBuffer + FIGURE_HEART
                                             + HeartbreakHeartBurnPictures[picture],
                                         63,
                                         y + HeartbreakHeartBurnY[picture] - 8,
                                         FIGURE_HEART_WIDTH,
                                         HeartbreakHeartBurnHeights[picture]);
                    else
                        draw[args->side](canvas,
                                         RamMapMetatileBuffer + FIGURE_HEART
                                             + HeartbreakHeartBurnPictures[picture],
                                         33,
                                         y + HeartbreakHeartBurnY[picture] - 8,
                                         FIGURE_HEART_WIDTH,
                                         HeartbreakHeartBurnHeights[picture]);
                }
            }
        }

        if (frame == FIGURE_HIT_TICK)
        {
            state->shakeFrame = 6;
            EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
            btEffectTintBattler(args->targets[0], SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 16);
        }
        if (frame == FIGURE_SPARK_BURST_TICK)
        {
            syWordsFillEntry(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
            state->shakeFrame = 3;
            sySoundPlayStub(SE_POWER_UP);
            btEffectTintBattler(args->targets[0], SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 16);
        }

        btCameraShake(4, 4);
        EFFECT_END_TICK(state);
    }

    syCloseEvent(btEffectTransferCanvas);
    btBackgroundLoadStub(
        BATTLE_BACKGROUND_FIXED, RamHeapSlots->battleState->bg, BACKGROUND_DARKEN_TICKS);
    EFFECT_RELEASE_BLITTERS();
    btEffectEndCanvasLayer();
}

#undef TARGET_ANCHOR_PERCENT
#undef DRAGON_SEGMENTS
#undef DRAGON_CORNERS
#undef DRAGON_SPARKS
#undef DRAGON_ARC_END
#undef DRAGON_ARC_SPEED
#undef DRAGON_SCALE
#undef DRAGON_SECOND_TURN
#undef DRAGON_HEAD_TURN
#undef RISING_DRAGON_VERTICES
#undef RISING_DRAGON_STRIP
#undef RISING_DRAGON_FLAME_DRAGON_HEAD_PICTURES
#undef FIGURE_TICKS
#undef FIGURE_SPARK_BURST_TICK
#undef FIGURE_HIT_TICK
#undef FIGURE_BODY
#undef FIGURE_BODY_WIDTH
#undef FIGURE_BODY_HEIGHT
#undef FIGURE_ARM_CLENCHED
#undef FIGURE_ARM_CLENCHED_WIDTH
#undef FIGURE_ARM_CLENCHED_HEIGHT
#undef FIGURE_ARM_REACHING
#undef FIGURE_ARM_REACHING_WIDTH
#undef FIGURE_ARM_REACHING_HEIGHT
#undef FIGURE_SPARK_BURST
#undef FIGURE_HEART
#undef FIGURE_HEART_WIDTH
#undef FIGURE_HEART_HEIGHT
#undef FIGURE_SWAY

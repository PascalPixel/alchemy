// Helm Splitter. Part of battle_effect.c.

#define HELM_SPLITTER_TICKS          70
#define HELM_SPLITTER_LEAP_TICKS     16 // the user's leap at the target
#define HELM_SPLITTER_STRIKE_TICK    18 // the swirl strikes the target
// FireSwirlSheet's bytes; the sparks' pictures, PinkStarSheet's stars or SkullSheet's skulls,
// follow them.
#define HELM_SPLITTER_SWIRL_BYTES    16000
#define HELM_SPLITTER_SPARK_PICTURES HELM_SPLITTER_SWIRL_BYTES
#define HELM_SPLITTER_SPARKS         16 // sparks the strike scatters, a variant
#define HELM_SPLITTER_SPARK_SLOTS    64 // spark slots moved and drawn
// The side of each of PinkStarSheet's six square stars.
#define HELM_SPLITTER_STAR_SIZE      16
#define HELM_SPLITTER_STARS          6
// FireSwirlSheet's pictures, the swirl growing; the last one holds.
#define HELM_SPLITTER_SWIRL_PICTURES 7

// The user leaps at the first target and strikes through a fire swirl; the target reels and sparks
// scatter, bouncing off the ground: stars for Helm Splitter, and for Skull Splitter twice as many
// skulls from a swirl of blue flame.
//
// Abilities: Helm Splitter, Skull Splitter.
void btEffectRunHelmSplitter(BATTLE_EFFECT_PARM *args)
{
    BATTLE_EFFECT_STATE *state  = RamHeapSlots->battleEffect;
    void                *canvas = RamHeapSlots->battleCanvas;
    DRAW_RECTANGLE       draw[2];
    SCREEN_POINT         pos;
    PARTICLE            *sparks;
    PARTICLE            *spark;
    Sint32              *life;
    Sint32               frame;
    Sint32               i;

    // Set up the canvas and load the swirl and the spark sprite sheets.
    btEffectStartCanvasLayer(BGCNT_PRIORITY(1));
    REG_BLDALPHA = EFFECT_BLEND_ADD;
    if (args->variant == EFFECT_VARIANT_BASIC)
    {
        REG_BG2PA = Q_8_8(1);
    }
    btResourceLoadAndDecompress(RESOURCE_FIRE_SWIRL_SHEET, state->sheet, 1, 1);
    if (args->variant == EFFECT_VARIANT_BASIC)
    {
        btResourceLoadAndDecompress(
            RESOURCE_PINK_STAR_SHEET, state->sheet + HELM_SPLITTER_SPARK_PICTURES, 1, 0);
        state->transferValue = CANVAS_FADE_HALF;
    }
    else
    {
        // Lift every drawn pixel of the swirl into the upper half of the palette, where the blue
        // flame's colours go.
        Uint8 *pixels = state->sheet;

        for (i = 0; i != HELM_SPLITTER_SWIRL_BYTES; i++)
        {
            if (pixels[i] != 0)
            {
                pixels[i] = pixels[i] * 2 + (CANVAS_LEVEL_BRIGHTEST + 1) / 2;
                if (pixels[i] > CANVAS_LEVEL_BRIGHTEST)
                    pixels[i] = CANVAS_LEVEL_BRIGHTEST;
            }
        }
        btResourceLoadAndDecompress(
            RESOURCE_SKULL_SHEET, state->sheet + HELM_SPLITTER_SPARK_PICTURES, 1, 1);
        syWordsCopyEntry((void *)BG_PLTT,
                         syResourceGetTableEntry(RESOURCE_BLUE_FLAME_SHEET),
                         SHEET_PALETTE_BYTES);
        state->transferValue = CANVAS_FADE_THREE_QUARTERS;
    }
    state->transferMode = CANVAS_COPY_AND_FADE;
    sySetEvent(btEffectTransferCanvas, EVENT_PASS_VBLANK);
    btEffectBuildCanvasBlitters(args->side, draw);

    // No spark is alive yet.
    for (i = 0, life = &state->particles[0].variant; i != HELM_SPLITTER_SPARK_SLOTS; i++)
    {
        *life  = 0;
        life  += sizeof(PARTICLE) / sizeof(*life);
    }
    sparks = state->particles;

    // The user leaps at the target.
    btMotionApproachTargetStub(args->user, args->targets[0], HELM_SPLITTER_LEAP_TICKS, toFIXED(7));

    for (frame = 0; frame != HELM_SPLITTER_TICKS; frame++)
    {
        // The swirl grows, a picture every second tick.
        if (args->variant == EFFECT_VARIANT_BASIC)
        {
            btEffectPosProjectBattler(args->user, &pos);
            if (args->side == EFFECT_SIDE_ENEMIES)
                REG_BG2X = (80 - pos.x) << 8;
            else
                REG_BG2X = (32 - pos.x) << 8;
            if (frame >= 8 && frame <= 23)
            {
                Sint32 picture = (frame - 8) / 2;

                if (picture > HELM_SPLITTER_SWIRL_PICTURES - 1)
                    picture = HELM_SPLITTER_SWIRL_PICTURES - 1;
                if (args->side == EFFECT_SIDE_ENEMIES)
                {
                    draw[0](canvas,
                            state->sheet + HelmSplitterPictures[picture],
                            HelmSplitterPictureBiasX[picture] + 30,
                            HelmSplitterPictureBiasY[picture] + pos.y - 60,
                            HelmSplitterPictureWidths[picture],
                            HelmSplitterPictureHeights[picture]);
                }
                else
                {
                    draw[0](canvas,
                            state->sheet + HelmSplitterPictures[picture],
                            -HelmSplitterPictureBiasX[picture] - HelmSplitterPictureWidths[picture]
                                + 98,
                            HelmSplitterPictureBiasY[picture] + pos.y - 60,
                            HelmSplitterPictureWidths[picture],
                            HelmSplitterPictureHeights[picture]);
                }
            }
        }
        else
        {
            btEffectPosProjectBattler(args->targets[0], &pos);
            if (frame >= 8 && frame <= 23)
            {
                Sint32 picture = (frame - 8) / 2;

                if (picture > HELM_SPLITTER_SWIRL_PICTURES - 1)
                    picture = HELM_SPLITTER_SWIRL_PICTURES - 1;
                if (args->side == EFFECT_SIDE_ENEMIES)
                {
                    draw[0](canvas,
                            state->sheet + HelmSplitterPictures[picture],
                            pos.x / 2 + HelmSplitterPictureBiasX[picture] - 16,
                            HelmSplitterPictureBiasY[picture] + pos.y - 92,
                            HelmSplitterPictureWidths[picture],
                            HelmSplitterPictureHeights[picture]);
                }
                else
                {
                    draw[0](canvas,
                            state->sheet + HelmSplitterPictures[picture],
                            pos.x / 2 - HelmSplitterPictureBiasX[picture]
                                - HelmSplitterPictureWidths[picture] + 16,
                            HelmSplitterPictureBiasY[picture] + pos.y - 92,
                            HelmSplitterPictureWidths[picture],
                            HelmSplitterPictureHeights[picture]);
                }
            }
        }

        // The strike: the target flashes and reels, the screen shakes and the swirl bursts into
        // sparks.
        if (frame == HELM_SPLITTER_STRIKE_TICK)
        {
            btEffectPosProjectBattler(args->targets[0], &pos);
            EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
            EFFECT_STRIKE_TARGET(args, 0, 8, KNOCKBACK_BOUNCE);
            state->shakeFrame = 4;
            for (i = 0; i != args->variant * HELM_SPLITTER_SPARKS + HELM_SPLITTER_SPARKS; i++)
            {
                Sint32 force = (syRandom() & 255) + 256;
                Sint32 angle = syRandom() & TRIG_ANGLE_MASK;

                spark        = &sparks[i];
                if (args->variant == EFFECT_VARIANT_BASIC)
                    spark->x = toFIXED(64);
                else
                    spark->x = toFIXED(pos.x / 2);
                spark->y         = toFIXED(80);
                spark->velocityX = (sySin(angle) * force) >> 7;
                spark->velocityY = -(syCos(angle) * force) >> 6;
                spark->variant   = (syRandom() & 15) + 16;
            }
        }

        // The sparks fall, bounce off the ground at y = 104 and fade out.
        for (i = 0; i != HELM_SPLITTER_SPARK_SLOTS; i++)
        {
            spark = &sparks[i];
            if (spark->variant > 0)
            {
                spark->variant--;
                btParticleAdvanceWithGravity2D(spark, 60, 0);
                if (spark->y > toFIXED(104))
                {
                    spark->velocityY = -spark->velocityY / 2;
                }
                else if ((spark->x >= 0 && spark->x < toFIXED(127)) && spark->y >= 0)
                {
                    Sint32 sx = FIXEDtoINT(spark->x);
                    Sint32 sy = FIXEDtoINT(spark->y);

                    if (args->variant == EFFECT_VARIANT_BASIC)
                        draw[0](canvas,
                                state->sheet + HELM_SPLITTER_SPARK_PICTURES
                                    + ((frame + i) / 4 % HELM_SPLITTER_STARS)
                                          * HELM_SPLITTER_STAR_SIZE * HELM_SPLITTER_STAR_SIZE,
                                sx - HELM_SPLITTER_STAR_SIZE / 2,
                                sy - HELM_SPLITTER_STAR_SIZE / 2,
                                HELM_SPLITTER_STAR_SIZE,
                                HELM_SPLITTER_STAR_SIZE);
                    else
                        draw[0](canvas,
                                state->sheet + HELM_SPLITTER_SPARK_PICTURES
                                    + i % 3 * PICTURE_BYTES(SKULL),
                                sx - SKULL_WIDTH / 2,
                                sy - SKULL_HEIGHT / 2,
                                SKULL_WIDTH,
                                SKULL_HEIGHT);
                }
            }
        }

        btCameraShake(8, 8);
        EFFECT_END_TICK(state);
    }

    EFFECT_FINISH_CANVAS();
}

#undef HELM_SPLITTER_TICKS
#undef HELM_SPLITTER_LEAP_TICKS
#undef HELM_SPLITTER_STRIKE_TICK
#undef HELM_SPLITTER_SWIRL_BYTES
#undef HELM_SPLITTER_SPARK_PICTURES
#undef HELM_SPLITTER_SPARKS
#undef HELM_SPLITTER_SPARK_SLOTS
#undef HELM_SPLITTER_STAR_SIZE
#undef HELM_SPLITTER_STARS
#undef HELM_SPLITTER_SWIRL_PICTURES

// Flare. Part of battle_effect.c.

#define FLARE_TICKS          80 // ticks of the flare

#define FLARE_ARC_COUNT      9  // flames on the arc
// The half ellipse's radii, across and up and down, in canvas pixels.
#define FLARE_ARC_RADIUS_X   32
#define FLARE_ARC_RADIUS_Y   16
// A flame's six pictures, four ticks each, before it goes out.
#define FLARE_FLAME_PICTURES 6
#define FLARE_PICTURE_TICKS  4

// Nine flames rise one after another along half an ellipse.
//
// Abilities: Flare, Flare Wall, Flare Storm.
void btEffectRunFlare(BATTLE_EFFECT_PARM *args)
{
    BATTLE_EFFECT_STATE *state;
    void                *canvas;
    Sint32               frame;
    DRAW_RECTANGLE       draw;
    Sint32               i;
    Sint32               picture;
    Uint32               width;
    Sint32               direction;
    Sint32               originX;
    PARTICLE            *arcFlame;
    PARTICLE            *flame;

    state       = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
    canvas      = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
    state->args = args;
    btEffectStartCanvasLayer(0);
    REG_BLDCNT   = BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
    REG_BLDALPHA = EFFECT_BLEND_CANVAS(CANVAS_BLEND_LEVEL);
    btResourceLoadAndDecompress(RESOURCE_EMBER_STREAK_SHEET, state->sheet, 1, 1);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, COMPOSITE_ADDITIVE);
    draw = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);

    // The arc of flames.
    if (state->args->targets[0] >= FIRST_ENEMY_UNIT)
    {
        originX   = 0;
        direction = 1;
    }
    else
    {
        originX   = 64;
        direction = -1;
    }
    for (i = 0; i != FLARE_ARC_COUNT; i++)
    {
        Sint32 angle = i * (TRIG_TURN / 16) - DEGtoANG(90);

        arcFlame     = &state->particles[i];
        arcFlame->x  = ((direction * FIXEDtoINT(sySin(angle) * FLARE_ARC_RADIUS_X)) + originX) + 20;
        arcFlame->y  = FIXEDtoINT(syCos(angle) * FLARE_ARC_RADIUS_Y) + 40;
        arcFlame->variant = -i * 4;
    }

    state->transferMode = CANVAS_COPY_AND_FADE;
    if (state->args->variant == EFFECT_VARIANT_STRONGEST)
    {
        state->transferValue = CANVAS_FADE_THREE_QUARTERS;
    }
    else
    {
        state->transferValue = CANVAS_FADE_HALF;
    }
    sySetEvent(btEffectTransferCanvas, EVENT_PASS_VBLANK);
    sySoundPlayStub(SE_WHOOSH);
    for (frame = 0; frame != FLARE_TICKS; frame++)
    {
        if (frame == 24)
        {
            EFFECT_SHOW_DAMAGE(SE_IMPACT);
        }

        // Each flame plays its pictures and goes out.
        for (i = 0; i != FLARE_ARC_COUNT; i++)
        {
            flame = &state->particles[i];
            if (flame->variant >= 0 && flame->variant < FLARE_FLAME_PICTURES * FLARE_PICTURE_TICKS)
            {
                picture = flame->variant / FLARE_PICTURE_TICKS;
                // UB: width is assigned in one argument and read in another.
#if FIX_UNSEQUENCED_SIDE_EFFECT
                width = BattleEffectPuffArcPictureWidths[picture];
                draw(canvas,
                     state->sheet + BattleEffectPuffArcPictures[picture],
                     flame->x - (width >> 1),
                     flame->y + BattleEffectPuffArcPictureBiasY[picture],
                     width,
                     BattleEffectPuffArcPictureHeights[picture]);
#else
                draw(canvas,
                     state->sheet + BattleEffectPuffArcPictures[picture],
                     flame->x - ((width = BattleEffectPuffArcPictureWidths[picture]) >> 1),
                     flame->y + BattleEffectPuffArcPictureBiasY[picture],
                     width,
                     BattleEffectPuffArcPictureHeights[picture]);
#endif
                // Flare Wall stacks a flame 16 pixels higher, and Flare Storm another 16 above
                // that.
                if (state->args->variant != EFFECT_VARIANT_BASIC)
                {
#if FIX_UNSEQUENCED_SIDE_EFFECT
                    width = BattleEffectPuffArcPictureWidths[picture];
                    draw(canvas,
                         state->sheet + BattleEffectPuffArcPictures[picture],
                         flame->x - (width >> 1),
                         (flame->y + BattleEffectPuffArcPictureBiasY[picture]) - 16,
                         width,
                         BattleEffectPuffArcPictureHeights[picture]);
#else
                    draw(canvas,
                         state->sheet + BattleEffectPuffArcPictures[picture],
                         flame->x - ((width = BattleEffectPuffArcPictureWidths[picture]) >> 1),
                         (flame->y + BattleEffectPuffArcPictureBiasY[picture]) - 16,
                         width,
                         BattleEffectPuffArcPictureHeights[picture]);
#endif
                }
                if (state->args->variant == EFFECT_VARIANT_STRONGEST)
                {
#if FIX_UNSEQUENCED_SIDE_EFFECT
                    width = BattleEffectPuffArcPictureWidths[picture];
                    draw(canvas,
                         state->sheet + BattleEffectPuffArcPictures[picture],
                         flame->x - (width >> 1),
                         (flame->y + BattleEffectPuffArcPictureBiasY[picture]) - 32,
                         width,
                         BattleEffectPuffArcPictureHeights[picture]);
#else
                    draw(canvas,
                         state->sheet + BattleEffectPuffArcPictures[picture],
                         flame->x - ((width = BattleEffectPuffArcPictureWidths[picture]) >> 1),
                         (flame->y + BattleEffectPuffArcPictureBiasY[picture]) - 32,
                         width,
                         BattleEffectPuffArcPictureHeights[picture]);
#endif
                }
            }
            flame->variant++;
        }

        // The targets flash red.
        for (i = 0; i != state->args->targetCount; i++)
        {
            if (frame == (i * 8) + 16)
            {
                btEffectTintBattler(
                    state->args->targets[i], SPRITE_COLOR_RED_TINT, POSE_DAMAGE, i, 12);
            }
        }
        EFFECT_END_TICK(state);
    }
    syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
    syCloseEvent(btEffectTransferCanvas);
    btEffectEndCanvasLayer();
}

// Lists the indices within indexRadius of where currentValue sits in values, up to entryLimit, and
// returns how many.
UNUSED Sint32 btEffectBuildCenteredIndexList(BATTLE_EFFECT_INDEX_WINDOW *window, Sint16 *output)
{
    Sint32 windowLength = window->indexRadius * 2 + 1;
    Sint32 outputCount  = 0;
    Sint32 centerIndex  = 0;
    Sint32 windowIndex;

    for (windowIndex = 0; windowIndex != windowLength; windowIndex++)
    {
        if (window->currentValue == window->values[windowIndex])
        {
            centerIndex = windowIndex;
            break;
        }
    }
    for (windowIndex = 0; windowIndex != windowLength; windowIndex++)
    {
        Sint32 index = centerIndex + windowIndex - window->indexRadius;

        if (index >= 0 && index < window->entryLimit)
        {
            output[outputCount++] = index;
        }
    }
    return outputCount;
}

#undef FLARE_TICKS
#undef FLARE_ARC_COUNT
#undef FLARE_ARC_RADIUS_X
#undef FLARE_ARC_RADIUS_Y
#undef FLARE_FLAME_PICTURES
#undef FLARE_PICTURE_TICKS

// Monster bites and slashes on one target, and Mad Zephyr; then the unused Blade Rain, the element
// pillar the battle raises at a unit's feet, and an unused fade-in of the battle background. Part
// of battle_effect.c.

// One slash strikes the first target among glints, in lightning colours.
//
// Abilities: Numbing Sting, Paralytail.
void btEffectRunNumbingSting(BATTLE_EFFECT_PARM *args)
{
    btEffectRunTargetStrike(args, TARGET_STRIKE_STYLE_LIGHTNING_GLINT_SLASH);
}

// Fangs bite the first target three times while motes stream out of it and back into the user.
//
// Abilities: Cannibal Fang.
void btEffectRunCannibalFang(BATTLE_EFFECT_PARM *args)
{
    btEffectRunTargetStrike(args, TARGET_STRIKE_STYLE_EMBER_DRAIN_BITES);
}

// Fangs bite the first target once, in lime, with smoke.
//
// Abilities: Poisonous Bite; Poison Fang.
void btEffectRunPoisonousBite(BATTLE_EFFECT_PARM *args)
{
    btEffectRunTargetStrike(args, TARGET_STRIKE_STYLE_LIME_SMOKE_BITE);
}

// Fangs bite the first target once, with smoke, in Mars Djinn colours.
//
// Abilities: Rabid Fang.
void btEffectRunRabidFang(BATTLE_EFFECT_PARM *args)
{
    btEffectRunTargetStrike(args, TARGET_STRIKE_STYLE_MARS_SMOKE_BITE);
}

// Fangs bite the first target three times among glints, in lightning colours.
//
// Abilities: Electric Bite.
void btEffectRunElectricBite(BATTLE_EFFECT_PARM *args)
{
    btEffectRunTargetStrike(args, TARGET_STRIKE_STYLE_LIGHTNING_GLINT_BITES);
}

// One lime slash strikes the first target, with smoke.
//
// Abilities: Poison Tail; Poison Sting.
void btEffectRunPoisonTail(BATTLE_EFFECT_PARM *args)
{
    btEffectRunTargetStrike(args, TARGET_STRIKE_STYLE_LIME_SMOKE_SLASH);
}

// Fangs bite the first target three times, draining motes back into the user, in Mars Djinn
// colours.
//
// Abilities: Vampiric Fang.
void btEffectRunVampiricFang(BATTLE_EFFECT_PARM *args)
{
    btEffectRunTargetStrike(args, TARGET_STRIKE_STYLE_MARS_DRAIN_BITES);
}

// Fangs bite the first target twice, in blue, with sparks.
//
// Abilities: Double Fang.
void btEffectRunDoubleFang(BATTLE_EFFECT_PARM *args)
{
    btEffectRunTargetStrike(args, TARGET_STRIKE_STYLE_BLUE_SPARK_BITES);
}

// One heavy slash strikes the first target in Mars Djinn colours, throwing sparks.
//
// Abilities: Mortal Blow.
void btEffectRunMortalBlow(BATTLE_EFFECT_PARM *args)
{
    btEffectRunTargetStrike(args, TARGET_STRIKE_STYLE_MARS_HEAVY_SLASH);
}

// Two slashes strike the first target, throwing sparks, in lightning colours.
//
// Abilities: Twin Beaks, Gryphon.
void btEffectRunTwinBeaks(BATTLE_EFFECT_PARM *args)
{
    btEffectRunTargetStrike(args, TARGET_STRIKE_STYLE_LIGHTNING_SPARK_SLASHES);
}

// Fangs bite the first target three times, in pink, with smoke.
//
// Abilities: Acid Bite.
void btEffectRunAcidBite(BATTLE_EFFECT_PARM *args)
{
    btEffectRunTargetStrike(args, TARGET_STRIKE_STYLE_PINK_SMOKE_BITES);
}

// Fangs bite the first target three times, draining motes back into the user, added in Mars Djinn
// colours.
//
// Abilities: Drain Fang.
void btEffectRunDrainFang(BATTLE_EFFECT_PARM *args)
{
    btEffectRunTargetStrike(args, TARGET_STRIKE_STYLE_MARS_ADDITIVE_DRAIN_BITES);
}

// One orange slash strikes the first target with a flash and sparks.
//
// Abilities: Angle Spike.
void btEffectRunAngleSpike(BATTLE_EFFECT_PARM *args)
{
    btEffectRunTargetStrike(args, TARGET_STRIKE_STYLE_ORANGE_FLASH_SPARK_SLASH);
}

// One orange slash strikes the first target with a flash, glints and sparks.
//
// Abilities: Heat Stun.
void btEffectRunHeatStun(BATTLE_EFFECT_PARM *args)
{
    btEffectRunTargetStrike(args, TARGET_STRIKE_STYLE_ORANGE_FLASH_GLINT_SLASH);
}

// Fangs bite the first target three times, draining motes back into the user, added in Mars Djinn
// colours.
//
// Abilities: Maneater.
void btEffectRunManeater(BATTLE_EFFECT_PARM *args)
{
    btEffectRunTargetStrike(args, TARGET_STRIKE_STYLE_MANEATER);
}

// Unused: no ability plays it, so the player never sees it: fangs bite the first target once, hard,
// throwing sparks, in Mars Djinn colours.
void btEffectRunUnusedHeavyBite(BATTLE_EFFECT_PARM *args)
{
    btEffectRunTargetStrike(args, TARGET_STRIKE_STYLE_MARS_HEAVY_BITE);
}

// Unused: no ability plays it, so the player never sees it: fangs bite the first target once with a
// flash and sparks, in orange.
void btEffectRunUnusedFlashBite(BATTLE_EFFECT_PARM *args)
{
    btEffectRunTargetStrike(args, TARGET_STRIKE_STYLE_ORANGE_FLASH_BITE);
}

// One slash strikes the first target while a star swells on it, in Mars Djinn colours.
//
// Abilities: Kill Sting.
void btEffectRunKillSting(BATTLE_EFFECT_PARM *args)
{
    btEffectRunTargetStrike(args, TARGET_STRIKE_STYLE_MARS_STAR_SLASH);
}

// Unused: no ability plays it, so the player never sees it: fangs bite the first target once, in
// blue, with sparks.
void btEffectRunUnusedSparkBite(BATTLE_EFFECT_PARM *args)
{
    btEffectRunTargetStrike(args, TARGET_STRIKE_STYLE_BLUE_SPARK_BITE);
}

// The user lunges a fifth of the way at the first target, then four slashes strike it, throwing
// sparks, in lightning colours; eight in its stronger variant.
//
// Abilities: Mad Zephyr.
void btEffectRunMadZephyr(BATTLE_EFFECT_PARM *args)
{
    btEffectSetApproachMotion(args->user, args->targets[0], 4, 20);
    sySynch(2);
    if (args->variant == EFFECT_VARIANT_BASIC)
        btEffectRunTargetStrike(args, TARGET_STRIKE_STYLE_LIGHTNING_FOUR_SPARK_SLASHES);
    else
        btEffectRunTargetStrike(args, TARGET_STRIKE_STYLE_LIGHTNING_EIGHT_SPARK_SLASHES);
}

// The blitter the effect draws with, made in the heap's RAM code slot.
#define BLITTER                          (RamHeapSlots->ramCode)

// Column `field` of style `mode`'s row of TargetStrikeStyles.
#define TARGET_STRIKE_VALUE(mode, field) TargetStrikeStyles[(mode)][(field)]
// The slash a tick falls in, a slash lasting six pictures. The bites count their sparks' and puffs'
// slots by it too, though a bite lasts four, so the second and third bites share theirs.
#define TARGET_STRIKE_SLASH(mode, tick)                                                            \
    ((tick) / (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_PICTURE_TICKS) * 6))

// The motes in PARTICLES, 128 for each target, then the strike sparks.
#define TARGET_STRIKE_MOTES_PER_TARGET  128
#define TARGET_STRIKE_MOTE_SLOTS        512
#define TARGET_STRIKE_SPARK_COUNT       256
#define TARGET_STRIKE_SPARKS            (PARTICLES + TARGET_STRIKE_MOTE_SLOTS)
// The sparks a slash's hit or a flash sets flying, and a bite's hit; a heavy hit sets them all
// flying.
#define TARGET_STRIKE_HIT_SPARKS        32
#define TARGET_STRIKE_BITE_SPARKS       64
// The motes a bite's hit sends out; the first 64 are drawn.
#define TARGET_STRIKE_BITE_MOTES        16
#define TARGET_STRIKE_DRAWN_MOTES       64
// A flash lives through FlashBurstSheet's six pictures, four ticks each.
#define TARGET_STRIKE_FLASH_TICKS       (FLASH_BURST_PICTURE_COUNT * 4)
// The ticks a drained mote flies out before the user pulls it in.
#define TARGET_STRIKE_MOTE_FLIGHT_TICKS 10
// The star's sine steps a 32nd of a turn a tick, and its scale rises to TARGET_STRIKE_STAR_SCALE
// 256ths, about 2.4 times.
#define TARGET_STRIKE_STAR_STEP_SHIFT   11
#define TARGET_STRIKE_STAR_SCALE        612
// The hit's pictures in the sprite sheet, by style: SmokeSheet's six 24x48 puffs, FlashBurstSheet's
// six 32x64 bursts, or StarCrossSheet's star.
#define TARGET_STRIKE_HIT_PICTURES      12800
// The bite's stages after the tornado sprite sheet: BeamImage for a slash, BlueDropletSheet
// otherwise.
#define TARGET_STRIKE_BITE_PICTURES     3200

// Strikes the first target with slashes or a bite of closing fangs, with the extras its style's
// flags choose (TARGET_STRIKE_STYLE): smoke, motes that stream back into the user, glints, sparks,
// a flash or a star. Each hit flashes and shakes the target, and the battle shows the damage at the
// last.
void btEffectRunTargetStrike(BATTLE_EFFECT_PARM *args, Sint32 mode)
{
    SCREEN_POINT         ground;
    SCREEN_POINT         targetPos;
    SCREEN_POINT         alternate;
    SCREEN_POINT         center;
    SCREEN_POINT         pos;
    BLITTER_PAIR         draw;
    MODEL_TEXTURE        texture;
    BATTLE_EFFECT_STATE *state;
    void                *canvas;
    Sint32               frame;
    Sint32               k;
    Uint8               *sheet;
    BATTLE_CAMERA       *camera;
    MOTION_OBJECT       *target;
    MOTION_OBJECT       *src;
    Sint32               resource;
    Sint32               i;

    state  = RamHeapSlots->battleEffect;
    canvas = RamHeapSlots->battleCanvas;
    sheet  = RamHeapSlots->battleSheet;
    camera = RamHeapSlots->cameraState;
    if (mode == TARGET_STRIKE_STYLE_MARS_HEAVY_SLASH)
        btEffectStartCanvasLayer(0);
    else
        btEffectStartCanvasLayer(BGCNT_PRIORITY(1));
    REG_BLDALPHA = EFFECT_BLEND_ADD;
    if (mode == TARGET_STRIKE_STYLE_MARS_HEAVY_SLASH)
    {
        state->fadeFrame = BACKGROUND_DARKEN_TICKS;
        state->fadeStep  = 0;
    }

    // The sprite sheets: motes, the tornado, the slashes or fangs, the style's colours and the
    // hit's pictures (smoke, flashes or the star).
    btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sheet, 0, 0);
    btResourceLoadAndDecompress(RESOURCE_TORNADO_SHEET, state->sheet, 1, 0);
    if (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_KIND) == STRIKE_KIND_SLASH)
        btResourceLoadAndDecompress(
            RESOURCE_BEAM_IMAGE, state->sheet + TARGET_STRIKE_BITE_PICTURES, 0, 0);
    else
        btResourceLoadAndDecompress(
            RESOURCE_BLUE_DROPLET_SHEET, state->sheet + TARGET_STRIKE_BITE_PICTURES, 0, 0);
    switch (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_COLOR))
    {
        case STRIKE_COLOR_BLUE     : resource = RESOURCE_BLUE_BEAM_SHEET; break;
        case STRIKE_COLOR_LIME     : resource = RESOURCE_LIME_PALETTE; break;
        case STRIKE_COLOR_PINK     : resource = RESOURCE_PINK_PALETTE; break;
        case STRIKE_COLOR_EMBER    : resource = RESOURCE_EMBER_STREAK_SHEET; break;
        case STRIKE_COLOR_MARS     : resource = RESOURCE_MARS_DJINN_SHEET; break;
        case STRIKE_COLOR_LIGHTNING:
        default                    : resource = RESOURCE_LIGHTNING_BOLT_SHEET; break;
        case STRIKE_COLOR_ORANGE   : resource = RESOURCE_ORANGE_PALETTE_B; break;
    }
    syWordsCopyEntry((void *)BG_PLTT, syResourceGetTableEntry(resource), SHEET_PALETTE_BYTES);
    if (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_FLAGS) & STRIKE_FLASH)
        btResourceLoadAndDecompress(
            RESOURCE_FLASH_BURST_SHEET, state->sheet + TARGET_STRIKE_HIT_PICTURES, 1, 0);
    else if (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_FLAGS) & STRIKE_STAR)
        btResourceLoadAndDecompress(
            RESOURCE_STAR_CROSS_SHEET, state->sheet + TARGET_STRIKE_HIT_PICTURES, 1, 0);
    else
        btResourceLoadAndDecompress(
            RESOURCE_SMOKE_SHEET, state->sheet + TARGET_STRIKE_HIT_PICTURES, 1, 0);

    // LEFTOVER(template): a per-target loop whose count is 1, the first target
    // * only, as btEffectRunDrainMotes (battle_effect_drain.c) seeds each
    // * target's motes over every target; as its body once with k 0.
    for (k = 0; k != 1; k++)
    {
        // The smoke or flashes start at the target's feet, waiting (-1) until a hit.
        target = btObjectGetStub(args->targets[k])->object;
        btEffectPosProjectBattler(args->targets[k], &ground);
        for (i = 0; i != ARRAY_COUNT(state->particles); i++)
        {
            PARTICLE *puff = &state->particles[i];

            SET_POSITION(puff, toFIXED(ground.x / 2), toFIXED(80), 0);
            puff->velocityX = ((syRandom() & 255) - 128) << 9;
            puff->velocityY = ((syRandom() & 255) - 128) << 9;
            puff->velocityZ = 0;
            puff->variant   = -1;
        }

        // The motes burst from 20 pixels above the target.
        for (i = 0; i != TARGET_STRIKE_MOTES_PER_TARGET; i++)
        {
            PARTICLE *mote = &PARTICLES[k * TARGET_STRIKE_MOTES_PER_TARGET + i];

            SET_POSITION(mote, target->x, toFIXED(20), target->z);
            mote->velocityX = ((syRandom() & 255) - 128) << 11;
            mote->velocityY = ((syRandom() & 255) - 128) << 11;
            mote->velocityZ = ((syRandom() & 255) - 128) << 11;
            mote->variant   = -1;
        }

        // The sparks fly away from the hit: heavy hits throw them in a fan away from the user, the
        // others towards the middle of the field.
        for (i = 0; i != TARGET_STRIKE_SPARK_COUNT; i++)
        {
            SET_POSITION(&TARGET_STRIKE_SPARKS[i], target->x, toFIXED(20), target->z);
            if (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_FLAGS) & STRIKE_HEAVY)
            {
                Sint32 angle                      = syRandom();
                Sint32 speed                      = syRandom() & 255;

                TARGET_STRIKE_SPARKS[i].velocityX = (syRandom() & 0x1ff) << 11;
                if (args->side == EFFECT_SIDE_PARTY)
                    TARGET_STRIKE_SPARKS[i].velocityX = -TARGET_STRIKE_SPARKS[i].velocityX;
                TARGET_STRIKE_SPARKS[i].velocityY = sySin(angle) * speed >> 4;
                TARGET_STRIKE_SPARKS[i].velocityZ = syCos(angle) * speed >> 4;
            }
            else
            {
                if (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_KIND) == STRIKE_KIND_BITE)
                {
                    TARGET_STRIKE_SPARKS[i].velocityX = ((syRandom() & 255) - 128) << 11;
                    TARGET_STRIKE_SPARKS[i].velocityY = ((syRandom() & 255) - 128) << 11;
                    TARGET_STRIKE_SPARKS[i].velocityZ = ((syRandom() & 255) - 128) << 11;
                }
                else
                {
                    TARGET_STRIKE_SPARKS[i].velocityX = (syRandom() & 255) << 11;
                    TARGET_STRIKE_SPARKS[i].velocityY = ((syRandom() & 255) - 128) << 11;
                    TARGET_STRIKE_SPARKS[i].velocityZ = ((syRandom() & 255) - 128) << 11;
                }
                if (TARGET_STRIKE_SPARKS[i].x > 0)
                    TARGET_STRIKE_SPARKS[i].velocityX = -TARGET_STRIKE_SPARKS[i].velocityX;
            }
            TARGET_STRIKE_SPARKS[i].variant = -1;
        }
    }

    state->transferMode = CANVAS_COPY_AND_FADE;
    if (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_FLAGS) & STRIKE_STAR)
        state->transferValue = CANVAS_FADE_HALF;
    else
        state->transferValue = CANVAS_FADE_THREE_QUARTERS;
    sySetEvent(btEffectTransferCanvas, EVENT_PASS_VBLANK);

    // Aim at the middle of the first target.
    btEffectPosProjectScaledBattler(args->targets[0], &targetPos);
    btEffectPosProjectBattler(args->targets[0], &alternate);
    targetPos.y += (alternate.y - targetPos.y) / 2;

    for (frame = 0; frame != TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_TICKS); frame++)
    {
        Sint32 span = TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_HITS)
                    * TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_PICTURE_TICKS);
        Sint32 length = span * 4;
        Sint32 glint  = 0;

        if (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_KIND) == STRIKE_KIND_SLASH)
        {
            // Slashes: six pictures each, sweeping across the target from the user's side.
            if (frame < span * 6)
            {
                Sint32 picture = frame / TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_PICTURE_TICKS) % 6;

                // Mad Zephyr aims each slash a little higher or lower.
                if (frame % (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_PICTURE_TICKS) * 6) == 0
                    && (mode == TARGET_STRIKE_STYLE_LIGHTNING_FOUR_SPARK_SLASHES
                        || mode == TARGET_STRIKE_STYLE_LIGHTNING_EIGHT_SPARK_SLASHES))
                {
                    btEffectPosProjectScaledBattler(args->targets[0], &targetPos);
                    btEffectPosProjectBattler(args->targets[0], &alternate);
                    targetPos.y += (alternate.y - targetPos.y) / 2;
                    targetPos.y += (syRandom() & 15) - 8;
                }
                if (args->side == EFFECT_SIDE_PARTY)
                {
                    EFFECT_LOAD_BLITTER(
                        HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
                    BLITTER(canvas,
                            state->sheet + TargetStrikePictures[picture],
                            targetPos.x / 2 - (TargetStrikeReach[picture] / 2)
                                - TargetStrikeWidths[picture] + 8,
                            targetPos.y - (TargetStrikeHeights[picture] / 2),
                            TargetStrikeWidths[picture],
                            TargetStrikeHeights[picture]);
                }
                else
                {
                    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
                    BLITTER(canvas,
                            state->sheet + TargetStrikePictures[picture],
                            targetPos.x / 2 + (TargetStrikeReach[picture] / 2) - 8,
                            targetPos.y - (TargetStrikeHeights[picture] / 2),
                            TargetStrikeWidths[picture],
                            TargetStrikeHeights[picture]);
                }
                syHeapSlotRelease(HEAP_SLOT_RAM_CODE);

                // A star sounds four ticks before its hit.
                if (frame % (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_PICTURE_TICKS) * 6)
                        == TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_PICTURE_TICKS) * 4 - 4
                    && (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_FLAGS) & STRIKE_STAR))
                    sySoundPlayStub(SE_POWER_UP);

                // The hit, on the slash's fifth picture.
                if (frame % (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_PICTURE_TICKS) * 6)
                    == TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_PICTURE_TICKS) * 4)
                {
                    if (mode == TARGET_STRIKE_STYLE_LIGHTNING_FOUR_SPARK_SLASHES)
                    {
                        if (frame / TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_PICTURE_TICKS) / 6 <= 2)
                            sySoundPlayStub(SE_IMPACT);
                        else
                            EFFECT_SHOW_DAMAGE(SE_IMPACT);
                    }
                    else if (mode == TARGET_STRIKE_STYLE_LIGHTNING_SPARK_SLASHES)
                    {
                        if (frame <= 8)
                            sySoundPlayStub(SE_IMPACT);
                        else
                            EFFECT_SHOW_DAMAGE(SE_IMPACT);
                    }
                    else if (mode == TARGET_STRIKE_STYLE_LIGHTNING_EIGHT_SPARK_SLASHES)
                    {
                        if (frame / TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_PICTURE_TICKS) / 6 > 6)
                            EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
                        else
                            sySoundPlayStub(SE_HEAVY_IMPACT);
                    }
                    else if ((TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_FLAGS) & STRIKE_STAR)
                             || (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_FLAGS) & STRIKE_FLASH))
                    {
                        EFFECT_SHOW_DAMAGE(SE_CLATTER);
                    }
                    else if (mode == TARGET_STRIKE_STYLE_MARS_HEAVY_SLASH)
                    {
                        EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
                    }
                    else
                    {
                        EFFECT_SHOW_DAMAGE(SE_IMPACT);
                    }

                    if (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_FLAGS) & STRIKE_FLASH)
                    {
                        state->shakeFrame = 8;
                        EFFECT_STRIKE_TARGET(args, 0, 12, KNOCKBACK_HURL);
                        for (i = 0; i != TARGET_STRIKE_HIT_SPARKS; i++)
                            TARGET_STRIKE_SPARKS[i].variant = (syRandom() & 15) + 15;
                    }
                    else if (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_FLAGS) & STRIKE_HEAVY)
                    {
                        state->shakeFrame = 8;
                        EFFECT_STRIKE_TARGET(args, 0, 12, KNOCKBACK_HURL);
                        for (i = 0; i != TARGET_STRIKE_SPARK_COUNT; i++)
                            TARGET_STRIKE_SPARKS[i].variant = (syRandom() & 15) + 15;
                    }
                    else
                    {
                        state->shakeFrame = 4;
                        btEffectTintBattler(
                            args->targets[0], SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 8);
                        for (i = 0; i != TARGET_STRIKE_HIT_SPARKS; i++)
                        {
                            PARTICLE *spark = &TARGET_STRIKE_SPARKS[TARGET_STRIKE_SLASH(mode, frame)
                                                                        * TARGET_STRIKE_HIT_SPARKS
                                                                    + i];

                            spark->variant  = (syRandom() & 15) + 7;
                        }
                    }
                    if (mode == TARGET_STRIKE_STYLE_ORANGE_FLASH_SPARK_SLASH)
                    {
                        for (i = 0; i != 4; i++)
                        {
                            PARTICLE *puff =
                                &state->particles[TARGET_STRIKE_SLASH(mode, frame) * 16 + i];

                            puff->variant = 0;
                        }
                    }
                    else
                    {
                        for (i = 0; i != 8; i++)
                        {
                            PARTICLE *puff =
                                &state->particles[TARGET_STRIKE_SLASH(mode, frame) * 16 + i];

                            puff->variant = 0;
                        }
                    }
                }
            }
            if (frame >= 12 && frame <= 31)
                glint = 1;
        }
        else if (frame < length + 4)
        {
            // Bites: rows of fangs close above and below the target, its left half drawn and then
            // mirrored.
            Sint32 stage;

            if (frame < length)
            {
                Sint32 picture = frame / TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_PICTURE_TICKS);

                while (picture > 4)
                    picture -= 4;
                stage = TargetStrikeBiteStages[picture];
            }
            else
            {
                stage = 3;
            }
            EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                                EFFECT_CLIP_BOUNDS,
                                TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_COMPOSITE));
            BLITTER(canvas,
                    state->sheet + stage * PICTURE_BYTES(TARGET_STRIKE_BITE)
                        + TARGET_STRIKE_BITE_PICTURES,
                    targetPos.x / 2 - TARGET_STRIKE_BITE_WIDTH,
                    TARGET_STRIKE_BITE_Y,
                    TARGET_STRIKE_BITE_WIDTH,
                    TARGET_STRIKE_BITE_HEIGHT);
            syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
            EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                                EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                                TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_COMPOSITE));
            BLITTER(canvas,
                    state->sheet + stage * PICTURE_BYTES(TARGET_STRIKE_BITE)
                        + TARGET_STRIKE_BITE_PICTURES,
                    targetPos.x / 2,
                    TARGET_STRIKE_BITE_Y,
                    TARGET_STRIKE_BITE_WIDTH,
                    TARGET_STRIKE_BITE_HEIGHT);
            syHeapSlotRelease(HEAP_SLOT_RAM_CODE);

            // The hit, on each bite's fourth picture.
            if (frame % (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_PICTURE_TICKS) * 4)
                == TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_PICTURE_TICKS) * 3)
            {
                btEffectTintBattler(args->targets[0], SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 8);
                state->shakeFrame = 4;
                if ((TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_FLAGS) & STRIKE_FLASH)
                    || (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_FLAGS) & STRIKE_HEAVY))
                    EFFECT_SHOW_DAMAGE(SE_CLATTER);
                else if (frame > (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_HITS) * 4 - 4)
                                     * TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_PICTURE_TICKS))
                    EFFECT_SHOW_DAMAGE(SE_IMPACT);
                else
                    sySoundPlayStub(SE_IMPACT);
                if (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_FLAGS) & STRIKE_FLASH)
                {
                    state->shakeFrame = 8;
                    EFFECT_STRIKE_TARGET(args, 0, 12, KNOCKBACK_HURL);
                    for (i = 0; i != TARGET_STRIKE_HIT_SPARKS; i++)
                        TARGET_STRIKE_SPARKS[i].variant = (syRandom() & 15) + 15;
                }
                else if (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_FLAGS) & STRIKE_HEAVY)
                {
                    state->shakeFrame = 8;
                    EFFECT_STRIKE_TARGET(args, 0, 12, KNOCKBACK_HURL);
                    for (i = 0; i != TARGET_STRIKE_SPARK_COUNT; i++)
                        TARGET_STRIKE_SPARKS[i].variant = (syRandom() & 15) + 15;
                }
                for (i = 0; i != TARGET_STRIKE_BITE_SPARKS; i++)
                {
                    PARTICLE *spark = &TARGET_STRIKE_SPARKS[TARGET_STRIKE_SLASH(mode, frame)
                                                                * TARGET_STRIKE_BITE_SPARKS
                                                            + i];

                    spark->variant  = (syRandom() & 15) + 7;
                }
                for (i = 0; i != 8; i++)
                {
                    PARTICLE *puff = &state->particles[TARGET_STRIKE_SLASH(mode, frame) * 16 + i];

                    puff->variant  = 0;
                }
                for (i = 0; i != TARGET_STRIKE_BITE_MOTES; i++)
                    PARTICLES[TARGET_STRIKE_SLASH(mode, frame) * TARGET_STRIKE_BITE_MOTES + i]
                        .variant = 0;
            }
            if (stage == 3)
                glint = 1;
        }

        // Three glints a tick around the target's middle.
        if ((TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_FLAGS) & STRIKE_GLINTS) && glint)
        {
            btEffectPosProjectScaledBattler(args->targets[0], &center);
            btEffectPosProjectBattler(args->targets[0], &alternate);
            center.y += (alternate.y - center.y) / 2;
            for (i = 0; i != 3; i++)
            {
                Sint32 image  = frame & 3;
                Sint32 angle  = syRandom() & TRIG_ANGLE_MASK;
                Sint32 radius = (syRandom() & 31) + 4;
                Sint32 x;
                Sint32 y;
                Uint32 flip;

                x = center.x / 2 + (FIXEDtoINT(sySin(angle) * radius) >> 1)
                  - (BattleEffectGlintPictureWidths[image] / 2);
                y = center.y - (FIXEDtoINT(syCos(angle) * radius) >> 1)
                  - (BattleEffectGlintPictureHeights[image] / 2);
                flip = syRandom();
                EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                                    TargetStrikeGlintDrawFlags[flip & 3] | EFFECT_CLIP_BOUNDS,
                                    BLITTER_ADDITIVE);
                ALT_BLITTER(canvas,
                            state->sheet + BattleEffectGlintPictures[image],
                            x,
                            y,
                            BattleEffectGlintPictureWidths[image],
                            BattleEffectGlintPictureHeights[image]);
                syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
            }
        }

        syInitMatrix();
        syLookAtLoad(camera->eye, camera->focus);
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
        draw[0] = BLITTER;
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
        draw[1] = ALT_BLITTER;
        // DEAD: a loop that runs once, for the user; the ROM keeps its counter, its test against 1
        // and its branch back.
        for (k = 0; k != 1; k++)
        {
            src = btObjectGetStub(args->user)->object;

            // Wisps of smoke rise and spread for SMOKE_PUFF_TICKS.
            if (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_FLAGS) & STRIKE_SMOKE)
            {
                for (i = 0; i != ARRAY_COUNT(state->particles); i++)
                {
                    PARTICLE *puff = &state->particles[i];

                    if (puff->variant >= 0 && puff->variant < SMOKE_PUFF_TICKS)
                    {
                        Sint32 picture = puff->variant / 4;

                        draw[i & 1](canvas,
                                    state->sheet + picture * PICTURE_BYTES(SMOKE_PUFF)
                                        + TARGET_STRIKE_HIT_PICTURES,
                                    FIXED_INTEGER_PART(puff->x) - SMOKE_PUFF_WIDTH / 2,
                                    FIXED_INTEGER_PART(puff->y) - SMOKE_PUFF_HEIGHT / 2,
                                    SMOKE_PUFF_WIDTH,
                                    SMOKE_PUFF_HEIGHT);
                        btParticleAdvanceWithGravity2D(puff, 62, -toFIXED(1) / 64);
                        puff->variant++;
                    }
                }
            }

            // Flashes rise twice as fast as smoke.
            if (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_FLAGS) & STRIKE_FLASH)
            {
                for (i = 0; i != ARRAY_COUNT(state->particles); i++)
                {
                    PARTICLE *flash = &state->particles[i];

                    if (flash->variant >= 0 && flash->variant < TARGET_STRIKE_FLASH_TICKS)
                    {
                        draw[0](canvas,
                                state->sheet + flash->variant / 4 * PICTURE_BYTES(FLASH_BURST)
                                    + TARGET_STRIKE_HIT_PICTURES,
                                FIXED_INTEGER_PART(flash->x) - FLASH_BURST_WIDTH / 2,
                                FIXED_INTEGER_PART(flash->y) - FLASH_BURST_HEIGHT / 2,
                                FLASH_BURST_WIDTH,
                                FLASH_BURST_HEIGHT);
                        btParticleAdvanceWithGravity2D(flash, 62, -toFIXED(1) / 32);
                        btParticleAdvanceWithGravity2D(flash, 62, -toFIXED(1) / 32);
                        flash->variant++;
                    }
                }
            }

            // Motes burst out, then stream back to the user, who flashes.
            if (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_FLAGS) & STRIKE_ABSORB)
            {
                Sint32 size = 3;

                if (mode == TARGET_STRIKE_STYLE_MARS_ADDITIVE_DRAIN_BITES)
                    size = 8;
                if (frame == 55)
                    btEffectTintBattler(args->user, SPRITE_COLOR_WHITE, TINT_KEEP, TINT_KEEP, 0);
                if (frame == 90)
                    btEffectTintBattler(args->user, SPRITE_COLOR_NORMAL, TINT_KEEP, TINT_KEEP, 0);
                for (i = 0; i != TARGET_STRIKE_DRAWN_MOTES; i++)
                {
                    PARTICLE *mote = &PARTICLES[i];

                    if (mote->variant >= 0)
                    {
                        btEffectPosProjectPoint((Sint32 *)mote, &pos);
                        pos.x >>= 1;
                        EFFECT_DRAW_MOTE(draw[1], canvas, sheet, pos.x, pos.y, size);
                        btParticleAdvanceWithGravity3D(mote, 60, 0);
                        mote->variant++;
                        if (mote->variant > TARGET_STRIKE_MOTE_FLIGHT_TICKS)
                        {
                            Sint32 dx        = (src->x - mote->x) >> 8;
                            Sint32 dy        = (src->y - mote->y + toFIXED(20)) >> 8;
                            Sint32 dz        = (src->z - mote->z) >> 8;

                            mote->velocityX += dx;
                            mote->velocityY += dy;
                            mote->velocityZ += dz;
                            if ((dx > -(toFIXED(1) / 16) && dx < toFIXED(1) / 16)
                                && (dz > -(toFIXED(1) / 16) && dz < toFIXED(1) / 16))
                                mote->variant = -1;
                        }
                    }
                }
            }

            // Sparks fall away, shrinking.
            if (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_FLAGS) & STRIKE_SPARKS)
            {
                for (i = 0; i != TARGET_STRIKE_SPARK_COUNT; i++)
                {
                    PARTICLE *spark = &TARGET_STRIKE_SPARKS[i];
                    Sint32    size  = spark->variant;

                    if (size >= 0)
                    {
                        btEffectPosProjectPoint((Sint32 *)spark, &pos);
                        pos.x >>= 1;
                        btParticleAdvanceWithGravity3D(spark, 60, 0);
                        size >>= 3;
                        size++;
                        EFFECT_DRAW_MOTE(draw[1], canvas, sheet, pos.x, pos.y, size);
                        spark->variant--;
                    }
                }
            }
        }
        EFFECT_RELEASE_BLITTERS();

        // The star: StarCrossSheet's star of light on a square over the target's middle swells and
        // shrinks from tick 8 to 17, as the canvas flashes to its brightest.
        if (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_FLAGS) & STRIKE_STAR)
        {
            void         *corners = syHeapAllocateEWRAM(32);
            RENDER_MODEL *model   = btRenderAllocateModelList(1);

            model->shade          = 0;
            texture.logWidth      = 6;
            texture.logHeight     = 6;
            texture.pixels        = state->sheet + TARGET_STRIKE_HIT_PICTURES;
            model->kind           = MODEL_SHADED_MAX;
            model->mesh           = BattleEffectSquareMesh;
            model->texture        = &texture;
            model->vertices       = corners;
            if (frame == 8)
                syWordsFillEntry(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
            if (frame >= 8 && frame <= 17)
            {
                Sint32 scale =
                    sySin(((frame - 8) << TARGET_STRIKE_STAR_STEP_SHIFT) + TRIG_TURN * 3 / 16)
                        * TARGET_STRIKE_STAR_SCALE
                    >> 8;

                syInitMatrix();
                syTranslate(INTtoFIXED(targetPos.x / 2 - 64), INTtoFIXED(targetPos.y - 64), 0);
                syRotX(DEGtoANG(90));
                syRotY(DEGtoANG(45));
                syScale(scale, scale / 2, scale / 2);
                btRenderTransformVertices(BattleEffectGroundSquareCorners, corners, 4);
                btRenderDrawModelList(model);
            }
            syHeapRelease(model);
            syHeapRelease(corners);
        }

        if (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_FLAGS) & STRIKE_HEAVY)
            btCameraShake(8, 8);
        else
            btCameraShake(2, 4);
        EFFECT_END_TICK(state);
    }

    syCloseEvent(btEffectTransferCanvas);
    btEffectEndCanvasLayer();
}

#define BLADE_RAIN_TICKS        120
#define BLADE_RAIN_SWORDS       16
// A sword drifts sideways 0 to 3 steps of two pixels a tick, and its picture is its step's:
// velocityX is the step << BLADE_RAIN_STEP_SHIFT.
#define BLADE_RAIN_STEP_SHIFT   17
// Where on the canvas the first sword waits for its turn; each four after it wait a pixel higher.
#define BLADE_RAIN_WAIT_Y       24
// CrossedDaggersSheet: four 32x32 pictures of a sword falling point down, each slanting more than
// the last.
#define BLADE_RAIN_SWORD_WIDTH  32
#define BLADE_RAIN_SWORD_HEIGHT 32

// Unused: nothing calls it. Swords fall slanting in on the first target, one every 4 ticks, and
// stick in the ground.
UNUSED void btEffectRunBladeRain(BATTLE_EFFECT_PARM *args)
{
    SCREEN_POINT         screenPos;
    BLITTER_PAIR         draw;
    BATTLE_EFFECT_STATE *state;
    void                *canvas;
    DRAW_RECTANGLE      *blit;
    PARTICLE            *blade;
    Sint32               frame;
    Sint32               i;

    state  = RamHeapSlots->battleEffect;
    canvas = RamHeapSlots->battleCanvas;
    EFFECT_START_NARROW_CANVAS(BGCNT_PRIORITY(1));
    REG_BLDALPHA = EFFECT_BLEND_CANVAS(0);

    // [0] as drawn, [1] mirrored left to right; both skip clear pixels.
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_TRANSPARENT);
    draw[0] = RamHeapSlots->ramCode;
    EFFECT_LOAD_BLITTER(
        HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_TRANSPARENT);
    draw[1] = RamHeapSlots->ramCodePaired;
    blit    = draw;
    btResourceLoadAndDecompress(RESOURCE_CROSSED_DAGGERS_SHEET, state->sheet, 1, 1);
    // each transfer clears the canvas
    state->transferMode  = CANVAS_COPY_AND_CLEAR;
    state->transferValue = 0;
    sySetEvent(btEffectTransferCanvas, EVENT_PASS_VBLANK);

    // Centre BG2 on the first target.
    btEffectPosProjectScaledBattler(args->targets[0], &screenPos);
    REG_BG2X = (CANVAS_SIZE / 2 - screenPos.x) << 8;

    // Swords start in columns 16 to 111 near the top of the canvas; the further from the middle,
    // the faster they drift towards it.
    for (i = 0; i != ARRAY_COUNT(state->particles); i++)
    {
        Sint32 column;

        blade    = &state->particles[i];
        column   = syRandom() % 96 + 16;
        blade->x = column;
        blade->y = toFIXED(BLADE_RAIN_WAIT_Y - i / 4);
        if (column <= 43)
            blade->velocityX = 3;
        else if (column <= 51)
            blade->velocityX = 2;
        else if (column <= 59)
            blade->velocityX = 1;
        else if (column <= 67)
            blade->velocityX = 0;
        else if (column <= 75)
            blade->velocityX = -1;
        else if (column <= 83)
            blade->velocityX = -2;
        else
            blade->velocityX = -3;
        blade->velocityX <<= BLADE_RAIN_STEP_SHIFT;
        blade->velocityY   = toFIXED(8);
        blade->x         <<= 16;
    }
    sySoundPlayStub(SE_POWER_UP);

    for (frame = 0; frame != BLADE_RAIN_TICKS; frame++)
    {
        if (frame <= 16)
        {
            REG_BLDALPHA = EFFECT_BLEND_CANVAS(frame);
            if (frame == 16)
                REG_BLDCNT = 0;
        }
        if (frame > 103)
        {
            REG_BLDALPHA = EFFECT_BLEND_CANVAS(BLADE_RAIN_TICKS - frame);
            if (frame == 104)
                REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
        }

        // The faster a sword drifts, the more its picture slants, and one drifting left is
        // mirrored. Sword i falls from tick i * 4 + 16 until it sticks in the ground at i * 4 + 25.
        for (i = BLADE_RAIN_SWORDS - 1; i != -1; i--)
        {
            Sint32 drift;
            Sint32 speed;
            Sint32 picture;
            Sint32 mirrored;

            blade = &state->particles[i];
            drift = blade->velocityX;
            speed = drift;
            if (drift < 0)
                speed = -drift;
            picture = speed >> BLADE_RAIN_STEP_SHIFT;
            if (frame < i * 4 + 25)
            {
                mirrored = drift < 0;
                blit[mirrored](canvas,
                               state->sheet + picture * PICTURE_BYTES(BLADE_RAIN_SWORD),
                               FIXEDtoINT(blade->x) - BLADE_RAIN_SWORD_WIDTH / 2,
                               FIXEDtoINT(blade->y) - BLADE_RAIN_SWORD_HEIGHT / 2,
                               BLADE_RAIN_SWORD_WIDTH,
                               BLADE_RAIN_SWORD_HEIGHT);
                if (frame >= i * 4 + 16)
                {
                    blade->x += blade->velocityX;
                    blade->y += blade->velocityY;
                }
            }
            else
            {
                mirrored = drift < 0;
                blit[mirrored](canvas,
                               state->sheet + picture * PICTURE_BYTES(BLADE_RAIN_SWORD),
                               FIXEDtoINT(blade->x) - BLADE_RAIN_SWORD_WIDTH / 2,
                               FIXEDtoINT(blade->y) - BLADE_RAIN_SWORD_HEIGHT / 2,
                               BLADE_RAIN_SWORD_WIDTH,
                               BLADE_RAIN_SWORD_HEIGHT);
            }
        }

        // From tick 24 to 84, every 4 ticks, the target flashes for 2 ticks and the screen shakes;
        // every 8, a blow sounds.
        if (frame >= 23 && frame <= 87 && (frame & 3) == 0)
        {
            btEffectTintBattler(args->targets[0], SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 2);
            state->shakeFrame = 1;
            if ((frame & 7) == 0)
                sySoundPlayStub(SE_IMPACT);
        }
        btCameraShake(8, 8);
        EFFECT_END_TICK(state);
    }

    EFFECT_FINISH_CANVAS();
}

#define ELEMENT_PILLAR_TICKS       56
// IceTileSheet: the pillar's half in four pictures, then its glow in three.
#define ELEMENT_PILLAR_HALF_WIDTH  17
#define ELEMENT_PILLAR_HALF_HEIGHT 64
#define ELEMENT_PILLAR_GLOW_WIDTH  24
#define ELEMENT_PILLAR_GLOW_HEIGHT 43
#define ELEMENT_PILLAR_GLOW        (4 * PICTURE_BYTES(ELEMENT_PILLAR_HALF))
// The pillar spirals the first ELEMENT_PILLAR_MOTES of its ELEMENT_SPIRAL_MOTES up around it, each
// circling once in ELEMENT_PILLAR_TURN_TICKS.
#define ELEMENT_PILLAR_MOTES       32
#define ELEMENT_PILLAR_TURN_TICKS  64
// The bytes of BlueGlowSheet, the motes' sheet, which the pillar allocates for itself.
#define ELEMENT_PILLAR_SHEET_BYTES 1550

// A pillar of light in an element's colours stands at a unit's feet while motes spiral up around
// it. The battle's element glow and the auto battle run it, and it sets up its own heap blocks.
void btEffectRunElementPillar(BATTLE_EFFECT_PARM *args)
{
    SCREEN_POINT         ground;
    SCREEN_POINT         screenPos;
    Sint32               point[3];
    BATTLE_EFFECT_STATE *state;
    void                *canvas;
    BLITTER_PAIR         draw;
    Uint8               *sheet;
    BATTLE_CAMERA       *camera;
    Sint32               frame;
    void                *palette;
    Sint32               i;

    state  = syHeapSlotAllocate(HEAP_SLOT_BATTLE_EFFECT, BATTLE_EFFECT_STATE_SIZE);
    canvas = syHeapSlotAllocate(HEAP_SLOT_BATTLE_CANVAS, CANVAS_BYTES);
    sheet  = syHeapSlotAllocate(HEAP_SLOT_BATTLE_SHEET, ELEMENT_PILLAR_SHEET_BYTES);
    camera = RamHeapSlots->cameraState;
    state->framesSinceTransfer = 1;
    state->args                = args;
    btEffectStartCanvasLayer(0);
    state->fadeFrame = BACKGROUND_DARKEN_TICKS;
    state->fadeStep  = 0;
    REG_BLDALPHA     = EFFECT_BLEND_CANVAS(ELEMENT_BLEND_LEVEL);
    REG_BG2PA        = Q_8_8(1);
    btResourceLoadAndDecompress(RESOURCE_ICE_TILE_SHEET, state->sheet, 1, 0);
    btResourceLoadAndDecompress(RESOURCE_BLUE_GLOW_SHEET, sheet, 0, 0);
    switch (args->kind)
    {
        case ELEMENT_VENUS  : palette = syResourceGetTableEntry(RESOURCE_YELLOW_PALETTE_A); break;
        case ELEMENT_MERCURY: palette = syResourceGetTableEntry(RESOURCE_ICE_TILE_SHEET); break;
        case ELEMENT_MARS   : palette = syResourceGetTableEntry(RESOURCE_RED_PALETTE_A); break;
        default             : palette = syResourceGetTableEntry(RESOURCE_VIOLET_PALETTE_A); break;
    }
    syWordsCopyEntry((void *)BG_PLTT, palette, SHEET_PALETTE_BYTES);

    // A mote's x is its angle and z its distance from the unit, in sixteenths of a pixel; mote i
    // waits i ticks before it rises.
    for (i = 0; i != ELEMENT_SPIRAL_MOTES; i++)
    {
        PARTICLES[i].y       = 0;
        PARTICLES[i].x       = syRandom() & TRIG_ANGLE_MASK;
        PARTICLES[i].z       = (syRandom() & 0x1ff) + i * 2;
        PARTICLES[i].variant = -i;
    }
    EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
    draw[0] = RamHeapSlots->ramCode;
    // The motes' size grows with the variant, from 1 to 4.
    args->variant++;
    if (args->variant <= 0)
        args->variant = 1;
    if (args->variant > 4)
        args->variant = 4;
    sySoundPlayStub(SE_POWER_UP);

    for (frame = 0; frame != ELEMENT_PILLAR_TICKS; frame++)
    {
        btEffectPosProjectBattler(args->user, &ground);
        REG_BG2X = (CANVAS_SIZE / 2 - ground.x) << 8;
        if (frame > 49)
            REG_BLDALPHA = EFFECT_BLEND_CANVAS(112 - frame * 2);
        if (frame == 16)
            btEffectTintBattler(args->targets[0], SPRITE_COLOR_WHITE, TINT_KEEP, 0, 20);

        // The pillar: the left half at x 47, then the right half mirrored at x 64, each with its
        // glow.
        if (frame <= 55)
        {
            Sint32 n;
            Sint32 flags;

            n     = frame / 2 % 4; // a pillar picture every 2 ticks
            flags = EFFECT_CLIP_BOUNDS;
            EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, flags, BLITTER_MAX);
            draw[1] = RamHeapSlots->ramCodePaired;
            draw[1](canvas,
                    state->sheet + n * PICTURE_BYTES(ELEMENT_PILLAR_HALF),
                    47,
                    ground.y - ELEMENT_PILLAR_HALF_HEIGHT,
                    ELEMENT_PILLAR_HALF_WIDTH,
                    ELEMENT_PILLAR_HALF_HEIGHT);
            n = frame / 4 % 3; // a glow picture every 4 ticks
            draw[1](canvas,
                    state->sheet + n * PICTURE_BYTES(ELEMENT_PILLAR_GLOW) + ELEMENT_PILLAR_GLOW,
                    40,
                    ground.y - 36,
                    ELEMENT_PILLAR_GLOW_WIDTH,
                    ELEMENT_PILLAR_GLOW_HEIGHT);
            syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
            flags = EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X;
            EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, flags, BLITTER_MAX);
            draw[1] = RamHeapSlots->ramCodePaired;
            n       = frame / 2 % 4;
            draw[1](canvas,
                    state->sheet + n * PICTURE_BYTES(ELEMENT_PILLAR_HALF),
                    64,
                    ground.y - ELEMENT_PILLAR_HALF_HEIGHT,
                    ELEMENT_PILLAR_HALF_WIDTH,
                    ELEMENT_PILLAR_HALF_HEIGHT);
            n = frame / 4 % 3;
            draw[1](canvas,
                    state->sheet + n * PICTURE_BYTES(ELEMENT_PILLAR_GLOW) + ELEMENT_PILLAR_GLOW,
                    64,
                    ground.y - 36,
                    ELEMENT_PILLAR_GLOW_WIDTH,
                    ELEMENT_PILLAR_GLOW_HEIGHT);
            syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
        }
        syInitMatrix();
        syLookAtLoad(camera->eye, camera->focus);

        // Each mote circles, rising 5 pixels and widening 4 pixels a tick.
        for (i = 0; i != ELEMENT_PILLAR_MOTES; i++)
        {
            PARTICLE *fleck = &PARTICLES[i];

            if (fleck->variant >= 0)
            {
                Sint32 size;

                point[0]  = fleck->z * sySin(fleck->x) >> 4;
                point[2]  = -(fleck->z * syCos(fleck->x) >> 4);
                point[1]  = fleck->y;
                fleck->x += TRIG_TURN / ELEMENT_PILLAR_TURN_TICKS;
                fleck->y += toFIXED(5);
                fleck->z += 64;
                btEffectPosProjectPoint(point, &screenPos);
                screenPos.x /= 2;
                size         = (i & 1) + args->variant;
                EFFECT_DRAW_FLARE(draw[0], canvas, sheet, screenPos.x, screenPos.y, size);
            }
            fleck->variant++;
        }
        EFFECT_END_TICK(state);
    }

    syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
    syCloseEvent(btEffectTransferCanvas);
    btEffectEndCanvasLayer();
    syHeapSlotRelease(HEAP_SLOT_BATTLE_SHEET);
    syHeapSlotRelease(HEAP_SLOT_BATTLE_CANVAS);
    syHeapSlotRelease(HEAP_SLOT_BATTLE_EFFECT);
}

// Unused: nothing refers to it. Shows the cloud swirl (btEffectRunCloudSwirl), then the battle
// background fading in, until A.
void btEffectRunBackgroundFadeIn(BATTLE_EFFECT_PARM *args)
{
    BATTLE_EFFECT_STATE     *state;
    void                    *canvas;
    BATTLE_STATE            *battle;
    BATTLE_BACKGROUND_STATE *view;
    Sint32                   frame;
    Sint32                   i;
    Sint32                   j;
    Sint32                   k;

    state       = syHeapSlotAllocate(HEAP_SLOT_BATTLE_EFFECT, BATTLE_EFFECT_STATE_SIZE);
    canvas      = syHeapSlotAllocate(HEAP_SLOT_BATTLE_CANVAS, CANVAS_BYTES);
    battle      = RamHeapSlots->battleState;
    view        = RamHeapSlots->battleBg;
    state->args = args;
    state->framesSinceTransfer = 1;
    btEffectRunCloudSwirl(args);

    // Make BG2 a 128x128 canvas, as btEffectStartCanvasLayer does.
    btEffectClearTintTimers();
    view->bg1ScrollMode = BATTLE_BACKGROUND_FIXED;
    BgScrollWork[1].y   = BATTLE_BACKGROUND_SCROLL_Y;
    btBackgroundSetModeStub(BATTLE_BACKGROUND_FIXED, battle->bg, 0);
    REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
    btCameraSetViewStub(0, 0, 0, BATTLE_ZOOM_NORMAL);
    view->bg1ScrollMode = BATTLE_BACKGROUND_NONE;
    REG_BG2X            = 0;
    REG_BG2Y            = -(BATTLE_VIEW_TOP << 8);
    REG_BG2PA           = Q_8_8(0.5);
    REG_BG2PB           = 0;
    REG_BG2PC           = 0;
    REG_BG2PD           = Q_8_8(1);
    EFFECT_SET_BATTLE_WINDOWS();
    REG_WININ =
        EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG2 | WININ_WIN1_OBJ | WININ_WIN1_CLR;
    REG_WINOUT = EFFECT_WINOUT;

    i          = 0;
    for (k = 0; k != CANVAS_TILES; k++)
        EFFECT_WRITE_CANVAS_MAP_ROW(k, j, i);

    syWordsClear(canvas, CANVAS_BYTES);
    syWordsClear((void *)BG_CHAR_BLOCK(1), CANVAS_BYTES);
    IO_WRITE_QUEUE_PUSH((Uint32)&REG_DISPCNT, EFFECT_DISPCNT, IO_WRITE_HALFWORD);
    REG_BLDALPHA = EFFECT_BLEND_ADD;
    REG_BLDCNT   = 0;
    EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_TRANSPARENT);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS, BLITTER_MAX);

    // Keep the battle background's image aside, show the battle background with its colours black,
    // and flash the first target.
    syWordsCopy(RamMapMetatileBuffer, (void *)BG_CHAR_BLOCK(2), BATTLE_BACKGROUND_BYTES);
    syWordsFill(RamMapMetatileBuffer, BATTLE_BACKGROUND_BYTES, CANVAS_FILL_WORD(1));
    state->savedBg1X = BgScrollWork[1].x;
    state->savedBg1Y = BgScrollWork[1].y;
    btBackgroundLoadStub(BATTLE_BACKGROUND_FIXED, battle->bg, -1);
    btPaletteBrightenSavedBattleBg(-RGB_LEVEL_MAX, -RGB_LEVEL_MAX, -RGB_LEVEL_MAX);
    btEffectTintBattler(args->targets[0], SPRITE_COLOR_WHITE, POSE_CAST, 0, 30);

    // Fade the colours up a shade every two ticks; B starts again, A ends.
    for (frame = 0; frame != 32096; frame++)
    {
        if (PerWork.data & KEY_B)
            frame = 0;
        if (PerWork.data & KEY_A)
            break;
        if (frame <= 63)
            btPaletteBrightenSavedBattleBg(
                frame / 2 - RGB_LEVEL_MAX, frame / 2 - RGB_LEVEL_MAX, frame / 2 - RGB_LEVEL_MAX);
        EFFECT_END_TICK(state);
    }

    // Put the background and the display back.
    EFFECT_RELEASE_BLITTERS();
    syCloseEvent(btEffectTransferCanvas);
    btEffectTintBattler(args->targets[0], TINT_KEEP, POSE_STANDING, TINT_KEEP, 0);
    BgScrollWork[1].x = state->savedBg1X;
    BgScrollWork[1].y = BATTLE_BACKGROUND_SCROLL_Y;
    btBackgroundSetModeStub(BATTLE_BACKGROUND_ZOOMED, battle->bg, 0);
    sySynch(1);
    IO_WRITE_QUEUE_PUSH((Uint32)&REG_DISPCNT,
                        DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG2_ON
                            | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON | DISPCNT_WIN1_ON,
                        IO_WRITE_HALFWORD);
    syHeapSlotRelease(HEAP_SLOT_BATTLE_SHEET);
    syHeapSlotRelease(HEAP_SLOT_BATTLE_CANVAS);
    syHeapSlotRelease(HEAP_SLOT_BATTLE_EFFECT);
}

#undef BLITTER
#undef TARGET_STRIKE_VALUE
#undef TARGET_STRIKE_SLASH
#undef TARGET_STRIKE_MOTES_PER_TARGET
#undef TARGET_STRIKE_MOTE_SLOTS
#undef TARGET_STRIKE_SPARK_COUNT
#undef TARGET_STRIKE_HIT_SPARKS
#undef TARGET_STRIKE_BITE_SPARKS
#undef TARGET_STRIKE_BITE_MOTES
#undef TARGET_STRIKE_DRAWN_MOTES
#undef TARGET_STRIKE_FLASH_TICKS
#undef TARGET_STRIKE_STAR_STEP_SHIFT
#undef TARGET_STRIKE_STAR_SCALE
#undef TARGET_STRIKE_SPARKS
#undef ELEMENT_PILLAR_SHEET_BYTES
#undef TARGET_STRIKE_MOTE_FLIGHT_TICKS
#undef TARGET_STRIKE_HIT_PICTURES
#undef TARGET_STRIKE_BITE_PICTURES
#undef BLADE_RAIN_TICKS
#undef BLADE_RAIN_SWORDS
#undef BLADE_RAIN_STEP_SHIFT
#undef BLADE_RAIN_WAIT_Y
#undef BLADE_RAIN_SWORD_WIDTH
#undef BLADE_RAIN_SWORD_HEIGHT
#undef ELEMENT_PILLAR_TICKS
#undef ELEMENT_PILLAR_HALF_WIDTH
#undef ELEMENT_PILLAR_HALF_HEIGHT
#undef ELEMENT_PILLAR_GLOW_WIDTH
#undef ELEMENT_PILLAR_GLOW_HEIGHT
#undef ELEMENT_PILLAR_GLOW
#undef ELEMENT_PILLAR_MOTES
#undef ELEMENT_PILLAR_TURN_TICKS

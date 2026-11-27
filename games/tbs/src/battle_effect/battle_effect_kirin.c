// The Kirin summon. Part of battle_effect.c.

#define KIRIN_CHARGE_TICKS       96  // ticks of the charge and the wall of fire
#define KIRIN_SWEEP_TICKS        120 // ticks of Kirin's bound through the hoop

#define KIRIN_BLOB_BYTES         PICTURE_BYTES(FIRE_BLOB)

// The sprites Kirin is drawn from, each at its KirinColumns and KirinRows.
#define KIRIN_SPRITES            9
// The forest's scroll speeds up a pixel a tick to this, in pixels a tick.
#define KIRIN_FOREST_SPEED_MAX   24
// How high Kirin rises in each bound, in its last leap away, and in the leap that opens its charge,
// in pixels.
#define KIRIN_BOUND_HEIGHT       32
#define KIRIN_LEAP_AWAY_HEIGHT   96
#define KIRIN_CHARGE_LEAP_HEIGHT 64
// The hoop's motes, spaced evenly round it.
#define KIRIN_HOOP_MOTES         64
// The fire trail: the first of state->particles.
#define KIRIN_TRAIL_MOTES        48
// How far the column of fire blobs slides in, in pixels.
#define KIRIN_BLOB_SLIDE_MAX     40
// The motes a target bursts into, at PARTICLES[target * KIRIN_BURST_MOTES].
#define KIRIN_BURST_MOTES        32
// The charge frees the motes of ten bursts and draws those of the first six targets.
#define KIRIN_BURSTS_FREED       10
#define KIRIN_BURSTS_DRAWN       6

// Over a scrolling forest Kirin bounds in through a hoop of light, trailing fire; then, unseen
// behind the battle background, it charges across the targets, each bursting into motes as it
// passes, and a wall of fire sweeps over them and leaves them black.
//
// Abilities: Kirin.
void btEffectRunKirin(BATTLE_EFFECT_PARM *args)
{
    static SPR_SCALE         sweepScale  = {{{toFIXED(1), toFIXED(1)}}};
    static SPR_SCALE         chargeScale = {{{toFIXED(1), toFIXED(1)}}};
    SPR_PLACEMENT            place;
    SCREEN_POINT             unit;
    Uint8                    wallJitter[16];
    Uint8                    passed[14];
    SPR_PLACEMENT            chargePlace;
    void                    *canvas;
    BATTLE_EFFECT_STATE     *state;
    DRAW_RECTANGLE           blit;
    Uint8                   *sheet;
    Sint32                   kirinX;
    Sint32                   scrolled;
    Sint32                   savedX;
    BATTLE_BACKGROUND_STATE *view;
    Sint32                   forestSpeed;
    Sint32                   bounce;
    Sint32                   frame;
    Sint32                   i;

    canvas      = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
    state       = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
    sheet       = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
    state->args = args;
    btEffectStartCanvasLayer(0);
    btEffectConfigureEffectDisplay();
    REG_BG2CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
    PLTT_BACKDROP       = 0;
    BG_PLTT_COLOR(0, 1) = 0;
    state->transferMode = CANVAS_COPY;
    sySetEvent(btEffectTransferCanvas, EVENT_PASS_VBLANK);
    // Setup: wipe the battle away, swap in the forest and wipe back.
    btEffectWipeCanvas(CANVAS_WIPE_ACROSS, CANVAS_WIPE_FILL);
    btEffectCreateSprites(KIRIN_SPRITES, SPRITE_KIRIN, 1);
    ProjectionWork.centerY = SUMMON_PROJECTION_CENTER_Y;
    btEffectShowLivingTargets(state->args);
    REG_WININ =
        EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG1 | WININ_WIN1_BG2 | WININ_WIN1_CLR;
    REG_WIN0H = WIN_RANGE(0, 202);
    sySynch(1);
    btBackgroundLoadStub(BATTLE_BACKGROUND_FIXED, RESOURCE_FOREST_BACKDROP, 0);
    btEffectWipeCanvas(CANVAS_WIPE_ACROSS, CANVAS_WIPE_CLEAR);
    btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sheet, 0, 0);
    btResourceLoadAndDecompress(RESOURCE_FIRE_BLOB_SHEET, state->sheet, 1, 1);
    EFFECT_SHOW_WIDE_CANVAS(CANVAS_BLEND_LEVEL);

    kirinX      = 0;
    scrolled    = 0;
    savedX      = BgScrollWork[1].x;
    view        = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_BACKGROUND);
    forestSpeed = 0;
    EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
    view->busy = 1;

    for (i = 0; i != ARRAY_COUNT(state->particles); i++)
    {
        PARTICLE *fire  = &state->particles[i];

        fire->x         = (syRandom() & 31) + 16;
        fire->y         = INTtoFIXED((syRandom() & 31) + 48);
        fire->velocityY = toFIXED((syRandom() & 31) - 16);
        fire->variant   = syRandom() % 48 + 2;
    }

    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, COMPOSITE_ADDITIVE);
    blit       = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
    REG_BG2CNT = BGCNT_PRIORITY(2) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);

    // The sweep: Kirin bounds in over the forest (A or B ends it).
    for (frame = 0; frame != KIRIN_SWEEP_TICKS; frame++)
    {
        bounce = 0;
        if (frame == 0)
        {
            sySoundPlayStub(SE_WHOOSH);
        }
        if (frame == 26)
        {
            sySoundPlayStub(SE_RUMBLE);
        }
        if (frame == 40)
        {
            sySoundPlayStub(SE_CAST);
        }
        if (frame == 72)
        {
            sySoundPlayStub(SE_CAST);
        }
        if (frame == 104)
        {
            sySoundPlayStub(SE_CAST);
        }
        if ((PerRepeat & (KEY_A | KEY_B)) != 0 && frame > 16)
        {
            break;
        }

        if (frame >= 24 && frame <= 55)
        {
            forestSpeed++;
        }
        if (forestSpeed > KIRIN_FOREST_SPEED_MAX)
        {
            forestSpeed = KIRIN_FOREST_SPEED_MAX;
        }
        // The forest scrolls the whole bound: this test and the one below are always true, as the
        // bound ends at KIRIN_SWEEP_TICKS (120).
        if (frame <= 135)
        {
            BgScrollWork[1].x -= forestSpeed;
            scrolled          += forestSpeed;
        }

        // Kirin bounds in, the screen shaking at each landing, then lifts away to the left.
        if (frame <= 149)
        {
            SPR_SCALE pair = sweepScale;
            Sint32    lift;

            lift = 0;
            if (frame > 103)
            {
                lift = (frame - 104) * 16;
            }
            if (frame >= 8 && frame <= 31)
            {
                kirinX = kirinX + forestSpeed - 8;
            }
            if (frame > 7)
            {
                Sint32 height;
                Sint32 angle;

                height = KIRIN_LEAP_AWAY_HEIGHT;
                if (frame <= 104)
                {
                    height = KIRIN_BOUND_HEIGHT;
                }
                // A bound every 32 ticks: half a sine wave, repeated.
                angle = (frame * (TRIG_TURN / 64) - DEGtoANG(45)) & TRIG_ANGLE_MASK;
                if (angle > DEGtoANG(180))
                {
                    angle -= DEGtoANG(180);
                }
                bounce = FIXEDtoINT(sySin(angle) * height);
                if ((frame & 31) == 8)
                {
                    state->shakeFrame = 4;
                }
            }
            place.groundHeight = 0;
            place.height       = toFIXED(255);
            for (i = 0; i != KIRIN_SPRITES; i++)
            {
                place.x = toFIXED(kirinX + KirinColumns[i] - lift) + toFIXED(224);
                place.y = toFIXED(KirinRows[i] - bounce) + toFIXED(72);
                fdRenderApplyProjectedPlacementStub(state->sprites[i], &place, &pair, 0);
            }
        }

        // A hoop of motes opens in front of Kirin.
        if (frame <= 26)
        {
            Sint32 size  = scrolled + 4;
            Sint32 count = frame * 8;

            if (size > MOTE_SIZE_MAX)
            {
                size = MOTE_SIZE_MAX;
            }
            if (count > KIRIN_HOOP_MOTES)
            {
                count = KIRIN_HOOP_MOTES;
            }
            // A tall ellipse, its half-width twice the motes' size before the cap and its
            // half-height twelve times it, swelling and moving right as the forest starts to
            // scroll.
            for (i = 0; i != count; i++)
            {
                Sint32 angle = i * (TRIG_TURN / KIRIN_HOOP_MOTES);
                Sint32 x     = FIXEDtoINT(sySin(angle) * ((scrolled + 4) * 2)) + scrolled + 96;
                Sint32 y     = FIXEDtoINT(syCos(angle) * ((scrolled + 4) * 12)) + 64;

                EFFECT_DRAW_MOTE(blit, canvas, sheet, x, y, size);
            }
        }

        if (frame == 24)
        {
            EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
        }
        if (frame == 28)
        {
            REG_BG2CNT =
                BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
        }

        // Fire trails Kirin.
        if (frame > 17)
        {
            for (i = 0; i != KIRIN_TRAIL_MOTES; i++)
            {
                PARTICLE *fire = &state->particles[i];

                if (fire->variant == 0)
                {
                    Sint32 size = i % 3 + 1;

                    blit(canvas,
                         sheet + BattleEffectMotePictures[size - 1],
                         fire->x,
                         FIXEDtoINT(fire->y) - size,
                         size,
                         size * 2);
                    fire->x         += 2;
                    fire->y         += fire->velocityY;
                    fire->velocityY  = fire->velocityY * 48 / DAMPING_SCALE;
                }
                else
                {
                    fire->variant--;
                }
                if (fire->x > 128 || fire->variant == 1)
                {
                    fire->x         = (syRandom() & 31) + kirinX + 172;
                    fire->y         = INTtoFIXED((syRandom() & 31) - bounce + 56);
                    fire->velocityY = ((syRandom() & 31) - 16) * (toFIXED(1) / 2);
                }
            }
        }

        // A column of fire blobs slides left.
        if (frame > 31)
        {
            Sint32 slide = (frame - 32) / 2;

            if (slide > KIRIN_BLOB_SLIDE_MAX)
            {
                slide = KIRIN_BLOB_SLIDE_MAX;
            }
            for (i = 0; i != 6; i++)
                blit(canvas,
                     state->sheet + (syRandom() & 3) * KIRIN_BLOB_BYTES,
                     120 - slide,
                     i * 18,
                     FIRE_BLOB_WIDTH,
                     FIRE_BLOB_HEIGHT);
        }

        if (state->shakeFrame > 0)
        {
            state->shakeFrame--;
            BgScrollWork[1].y = (syRandom() & 7) + BATTLE_BACKGROUND_SCROLL_Y - 4;
        }
        else
        {
            BgScrollWork[1].y = BATTLE_BACKGROUND_SCROLL_Y;
        }
        EFFECT_PRESENT_CANVAS(state);
    }

    // The charge: Kirin runs across the targets behind the battle background.
    BgScrollWork[1].x = savedX;
    view->busy        = 0;
    btEffectSetupBlendedDisplay();
    REG_WIN0H = WIN_RANGE(0, DISPLAY_WIDTH);

    for (i = 0; i != KIRIN_SPRITES; i++)
    {
        SPR *sprite            = state->sprites[i];

        sprite->image.priority = 3;
    }

    {
        Sint32 chargeX;

        chargeX = 224;
        for (i = 0; i != ARRAY_COUNT(passed); i++)
        {
            passed[i] = 0;
        }
        for (i = 0; i != ARRAY_COUNT(wallJitter); i++)
        {
            wallJitter[i] = syRandom() & 31;
        }
        for (i = 0; i != KIRIN_BURSTS_FREED * KIRIN_BURST_MOTES; i++)
        {
            PARTICLES[i].variant = 0;
        }

        EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
        REG_BG2CNT =
            BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
        REG_BLDALPHA = EFFECT_BLEND_ADD;

        for (frame = 0; frame != KIRIN_CHARGE_TICKS; frame++)
        {
            if (frame <= 23)
            {
                SPR_SCALE chargeScaleCopy = chargeScale;
                Sint32    angle;
                Sint32    rise;

                chargeX -= 16;
                // A bound every 16 ticks, the first higher.
                if (frame <= 8)
                {
                    angle = frame * (TRIG_TURN / 32) + DEGtoANG(90);
                    if (angle > DEGtoANG(180))
                    {
                        angle -= DEGtoANG(180);
                    }
                    rise = FIXEDtoINT(sySin(angle) * KIRIN_CHARGE_LEAP_HEIGHT);
                }
                else
                {
                    angle = frame * (TRIG_TURN / 32) + DEGtoANG(90);
                    if (angle > DEGtoANG(180))
                    {
                        angle -= DEGtoANG(180);
                    }
                    rise = FIXEDtoINT(sySin(angle) * KIRIN_BOUND_HEIGHT);
                }
                chargePlace.groundHeight = 0;
                chargePlace.height       = toFIXED(255);
                for (i = 0; i != KIRIN_SPRITES; i++)
                {
                    chargePlace.x = toFIXED(chargeX + KirinColumns[i]);
                    chargePlace.y = toFIXED(KirinRows[i] - rise) + toFIXED(72);
                    fdRenderApplyProjectedPlacementStub(
                        state->sprites[i], &chargePlace, &chargeScaleCopy, 0);
                }
            }

            if (frame == 8)
            {
                state->shakeFrame = 8;
                sySoundPlayStub(SE_CRASH);
            }
            if (frame == 11)
            {
                sySoundPlayStub(SE_CRASH);
            }
            if (frame == 46)
            {
                sySoundPlayStub(SE_KIRIN);
            }

            EFFECT_FOR_EACH_TARGET(i, state->args)
            {
                // A target Kirin passes bursts into motes and reels.
                if (passed[i] == 0)
                {
                    btProjectionUnit(state->args->targets[i], &unit);
                    if (unit.x > chargeX)
                    {
                        Sint32 k;

                        passed[i] = 1;
                        for (k = 0; k != KIRIN_BURST_MOTES; k++)
                        {
                            PARTICLE *burst = &PARTICLES[i * KIRIN_BURST_MOTES + k];
                            Sint32    up;

                            // From the target's place on the wide canvas, 16 pixels up.
                            burst->x          = unit.x * (toFIXED(1) / 2);
                            burst->y          = INTtoFIXED(unit.y - 16);
                            burst->velocityX  = ((syRandom() & 255) - 128) * (toFIXED(1) / 64);
                            up                = (syRandom() & 255) - 192;
                            burst->velocityY  = up * (toFIXED(1) / 32);
                            burst->x         += burst->velocityX * 4;
                            burst->y         += up * (toFIXED(1) / 8);
                            burst->variant    = (syRandom() & 15) + 8;
                        }
                        btMotionApplyKnockbackStub(state->args->targets[i], KNOCKBACK_HOP);
                        sySoundPlayStub(SE_HEAVY_IMPACT);
                    }
                }
            }

            for (i = 0; i != KIRIN_BURSTS_DRAWN * KIRIN_BURST_MOTES; i++)
            {
                PARTICLE *burst = &PARTICLES[i];

                if (burst->variant > 0)
                {
                    // The mote's size.
                    Sint32 size = 3;

                    EFFECT_DRAW_MOTE(
                        blit, canvas, sheet, FIXEDtoINT(burst->x), FIXEDtoINT(burst->y), size);
                    burst->x += burst->velocityX;
                    burst->y += burst->velocityY;
                    burst->variant--;
                }
            }

            if (frame == 48)
            {
                sySoundPlayStub(SE_WHOOSH);
            }
            // The wall of fire slides off to the left and its smear dies away.
            if (frame > 40)
            {
                Sint32 slide = (frame - 40) * 12;

                EFFECT_SET_TRANSFER(state, CANVAS_COPY, CANVAS_FADE_THREE_QUARTERS);
                for (i = 0; i != ARRAY_COUNT(wallJitter); i++)
                    blit(canvas,
                         state->sheet + (syRandom() & 3) * KIRIN_BLOB_BYTES,
                         wallJitter[i] - slide + 120,
                         i * 8 - 8,
                         FIRE_BLOB_WIDTH,
                         FIRE_BLOB_HEIGHT);
            }
            if (frame > 64)
            {
                state->transferMode = CANVAS_COPY_AND_FADE;
            }
            // Every target, burnt by the wall of fire, turns black.
            if (frame == 58)
            {
                EFFECT_FOR_EACH_TARGET(i, state->args)
                {
                    btEffectTintBattler(
                        state->args->targets[i], SPRITE_COLOR_BLACK, POSE_DAMAGE, TINT_KEEP, 0);
                }
            }

            btCameraShake(8, 8);
            EFFECT_END_TICK(state);
        }
    }

    EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
    for (i = 0; i != KIRIN_SPRITES; i++)
    {
        fdSpriteDestroyStub(state->sprites[i]);
    }
    EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

#undef KIRIN_BLOB_BYTES
#undef KIRIN_SPRITES
#undef KIRIN_FOREST_SPEED_MAX
#undef KIRIN_BOUND_HEIGHT
#undef KIRIN_LEAP_AWAY_HEIGHT
#undef KIRIN_CHARGE_LEAP_HEIGHT
#undef KIRIN_HOOP_MOTES
#undef KIRIN_TRAIL_MOTES
#undef KIRIN_BLOB_SLIDE_MAX
#undef KIRIN_BURST_MOTES
#undef KIRIN_BURSTS_FREED
#undef KIRIN_BURSTS_DRAWN

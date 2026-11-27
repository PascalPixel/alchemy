// Demon Night, Rumble and Haunting. Part of battle_effect.c.

// Sixteen pink lights burst out of the user and are pulled into each target, with the heavy impact
// sound.
//
// Abilities: Demon Night.
void btEffectRunDemonNight(BATTLE_EFFECT_PARM *args)
{
    btEffectRunGatheringLights(args, GATHERING_LIGHTS_DEMON_NIGHT);
}

// Eight lights in Mars Djinn colours burst out of the user and are pulled into each target.
//
// Abilities: Rumble.
void btEffectRunRumble(BATTLE_EFFECT_PARM *args)
{
    btEffectRunGatheringLights(args, GATHERING_LIGHTS_RUMBLE);
}

// Twelve lights in Mercury Djinn colours burst out of the user and are pulled into each target.
//
// Abilities: Haunting.
void btEffectRunHaunting(BATTLE_EFFECT_PARM *args)
{
    btEffectRunGatheringLights(args, GATHERING_LIGHTS_HAUNTING);
}

#define GATHERING_LIGHTS_SLOTS        128 // particles reserved for each target
// The targets' turns, this many ticks apart: each is struck by its lights from
// GATHERING_LIGHTS_HIT_TICK.
#define GATHERING_LIGHTS_TARGET_TICKS 20
#define GATHERING_LIGHTS_HIT_TICK     70
// GatheringLightsImage's three lights.
#define GATHERING_LIGHT_WIDTH         20
#define GATHERING_LIGHT_HEIGHT        32
// How far the canvas's ripple sways each line either side of its middle, in CANVAS_WAVE_BG2X's
// quarter canvas pixels (4 canvas pixels, 8 on screen); the wave moves a 64th of a turn a line and
// a tick.
#define GATHERING_LIGHTS_RIPPLE       16

// For each target in turn, lights burst out of the user and are pulled into the middle of the
// target's body, which is struck as they arrive; the effect layer ripples throughout.
void btEffectRunGatheringLights(BATTLE_EFFECT_PARM *args, Sint32 mode)
{
    SCREEN_POINT         screenPos;
    BLITTER_PAIR         draw;
    BATTLE_EFFECT_STATE *state;
    void                *canvas;
    Sint32               frame;
    Sint32               k;
    BATTLE_CAMERA       *camera;
    Sint32               paletteId;
    Sint32               i;

    state       = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
    canvas      = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
    camera      = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
    state->args = args;
    btEffectStartCanvasLayer(BGCNT_PRIORITY(1));
    syDecompressTaggedLZ(syResourceGetTableEntry(RESOURCE_GATHERING_LIGHTS_IMAGE), state);
    if (mode == GATHERING_LIGHTS_DEMON_NIGHT)
        paletteId = RESOURCE_PINK_BURST_SHEET;
    else if (mode == GATHERING_LIGHTS_RUMBLE)
        paletteId = RESOURCE_MARS_DJINN_SHEET;
    else
        paletteId = RESOURCE_MERCURY_DJINN_SHEET;
    syWordsCopy(BG_PLTT, syResourceGetTableEntry(paletteId), SHEET_PALETTE_BYTES);
    btEffectBuildCanvasBlitters(state->args->side, draw);

    // The lights fly off the user in every direction.
    EFFECT_FREE_PARTICLES(i, PARTICLES, PARTICLE_POOL_COUNT);
    EFFECT_FOR_EACH_TARGET(k, state->args)
    {
        ACTOR *user   = btObjectGetStub(state->args->user)->actor;
        Sint32 height = btUnitGetEffectHeightStub(state->args->user);

        for (i = 0; i != GATHERING_LIGHTS_SLOTS; i++)
        {
            PARTICLE *mote = &PARTICLES[k * GATHERING_LIGHTS_SLOTS + i];

            SET_POSITION(mote, user->x.fixed, height, user->z.fixed);
            SET_VELOCITY(mote,
                         ((syRandom() & 255) - 128) << 10,
                         ((syRandom() & 255) - 128) << 10,
                         ((syRandom() & 255) - 128) << 10);
            mote->variant = 0;
        }
    }

    sySetEvent(btEffectArmBg2AffineHBlankDMA, EVENT_PASS_VBLANK);
    EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
    sySoundPlayStub(SE_ENEMY_CRY);

    for (frame = 0; frame
                    != GatheringLightsCounts[mode][GATHERING_LIGHTS_TICKS]
                           + state->args->targetCount * GATHERING_LIGHTS_TARGET_TICKS;
         frame++)
    {
        Sint32 *row;

        if (frame == 80)
        {
            if (mode == GATHERING_LIGHTS_DEMON_NIGHT)
                EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
            else
                EFFECT_SHOW_DAMAGE(SE_IMPACT);
        }
        EFFECT_VIEW_FROM_CAMERA(camera);

        // Ripple the canvas line by line.
        row = state->bg2xLines;
        for (i = 0; i != DISPLAY_HEIGHT; i++)
            *row++ =
                CANVAS_WAVE_BG2X(toFIXED(GATHERING_LIGHTS_RIPPLE)
                                 - sySin((frame + i) * (TRIG_TURN / 64)) * GATHERING_LIGHTS_RIPPLE);

        EFFECT_FOR_EACH_TARGET(k, state->args)
        {
            ACTOR *target = btObjectGetStub(state->args->targets[k])->actor;
            Sint32 half   = btUnitGetEffectHeightStub(state->args->targets[k]) / 2;

            if (frame == k * GATHERING_LIGHTS_TARGET_TICKS + GATHERING_LIGHTS_HIT_TICK + 1)
            {
                if (mode == GATHERING_LIGHTS_DEMON_NIGHT)
                    sySoundPlayStub(SE_HEAVY_IMPACT);
                else
                    sySoundPlayStub(SE_IMPACT);
            }
            if (frame == k * GATHERING_LIGHTS_TARGET_TICKS + GATHERING_LIGHTS_HIT_TICK)
                btEffectTintBattler(
                    state->args->targets[k], SPRITE_COLOR_WHITE, POSE_DAMAGE, k, 26);
            if (frame > k * GATHERING_LIGHTS_TARGET_TICKS)
            {
                for (i = 0; i != GatheringLightsCounts[mode][GATHERING_LIGHTS_COUNT]; i++)
                {
                    PARTICLE *mote = &PARTICLES[k * GATHERING_LIGHTS_SLOTS + i];

                    // Each light leaves two ticks after the one before it.
                    if (frame > (k * (GATHERING_LIGHTS_TARGET_TICKS / 2) + i) * 2
                        && mote->variant >= 0)
                    {
                        btProjectionPoint(mote, &screenPos);
                        screenPos.x >>= 1;
                        draw[0](canvas,
                                state->sheet + i % 3 * PICTURE_BYTES(GATHERING_LIGHT),
                                screenPos.x - GATHERING_LIGHT_WIDTH / 2,
                                screenPos.y - GATHERING_LIGHT_HEIGHT / 2,
                                GATHERING_LIGHT_WIDTH,
                                GATHERING_LIGHT_HEIGHT);
                        btParticleAdvanceWithGravity3D(mote, 62, 0);
                        // Thirty ticks on, the light is pulled toward the middle of its target's
                        // body, its speed gaining a 512th of the distance a tick, and is gone once
                        // within 32 pixels of it across and in depth.
                        if (frame > k * GATHERING_LIGHTS_TARGET_TICKS + i + 30)
                        {
                            Sint32 dx        = (target->x.fixed - mote->x) >> 9;
                            Sint32 dy        = (target->y.fixed + half - mote->y) >> 9;
                            Sint32 dz        = (target->z.fixed - mote->z) >> 9;

                            mote->velocityX += dx;
                            mote->velocityY += dy;
                            mote->velocityZ += dz;
                            if ((dx >= -4095 && dx <= 4095) && (dz >= -4095 && dz <= 4095))
                                mote->variant = -1;
                        }
                    }
                }
            }
        }
        EFFECT_END_TICK(state);
    }

    syCloseEvent(btEffectTransferCanvas);
    EFFECT_RELEASE_BLITTERS();
    syCloseEvent(btEffectArmBg2AffineHBlankDMA);
    btEffectEndCanvasLayer();
}

#undef GATHERING_LIGHTS_SLOTS
#undef GATHERING_LIGHTS_RIPPLE
#undef GATHERING_LIGHTS_TARGET_TICKS
#undef GATHERING_LIGHTS_HIT_TICK
#undef GATHERING_LIGHT_WIDTH
#undef GATHERING_LIGHT_HEIGHT

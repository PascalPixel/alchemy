// Smoke Bomb and Blast, and the function that creates an effect's sprites. Part of battle_effect.c.

// The eruption's modes (btEffectRunEruption).
typedef enum ERUPTION_MODE
{
    ERUPTION_BLAST, // fire streak colours; the variant picks the counts and
    // the targets recoil
    ERUPTION_SMOKE_BOMB // greys, with Mad Blast's counts
} ERUPTION_MODE;

// A fountain of grey motes and puffs of smoke bursts up beside the targets and falls back, flashing
// each target.
//
// Abilities: Smoke Bomb, Sleep Bomb.
void btEffectRunSmokeBomb(BATTLE_EFFECT_PARM *args)
{
    btEffectRunEruption(args, ERUPTION_SMOKE_BOMB);
}

// A fountain of fire-coloured motes and puffs of smoke bursts up beside the targets and falls back,
// flashing each target and making it recoil.
//
// Abilities: Blast, Mad Blast, Fiery Blast.
void btEffectRunBlast(BATTLE_EFFECT_PARM *args)
{
    btEffectRunEruption(args, ERUPTION_BLAST);
}

// The mote pictures in the sprite sheet: two sets, MOTE_SET_BYTES apart.
#define ERUPTION_MOTE_PICTURES   12800

// The puffs of smoke given a start, of which the variant's count in EruptionCounts are thrown.
#define ERUPTION_PUFFS           32
// A puff shows each of its seven pictures (EruptionPuffPictures) for three ticks, then is gone.
#define ERUPTION_PUFF_TICKS      21
// A mote is drawn ERUPTION_MOTE_LARGEST pixels wide (and twice as tall) at the nearest depth, a
// pixel narrower for every ERUPTION_MOTE_DEPTH_STEP farther, its depth clamped from
// ERUPTION_MOTE_DEPTH_NEAR to ERUPTION_MOTE_DEPTH_FAR.
#define ERUPTION_MOTE_LARGEST    9
#define ERUPTION_MOTE_DEPTH_NEAR 160
#define ERUPTION_MOTE_DEPTH_FAR  799
#define ERUPTION_MOTE_DEPTH_STEP 64

// A fountain of motes and puffs of smoke bursts up beside the targets and falls back, hitting the
// targets one after another.
void btEffectRunEruption(BATTLE_EFFECT_PARM *args, Sint32 mode)
{
    BATTLE_EFFECT_STATE *state;
    void                *canvas;
    // LEFTOVER(frame): draw[1], the idle half of the pair.
    BLITTER_PAIR         draw;
    Sint32               variant;
    SCREEN_POINT         screen;
    Sint32               frame;
    Sint32               i;

    state  = RamHeapSlots->battleEffect;
    canvas = RamHeapSlots->battleCanvas;
    btEffectStartCanvasLayer(BGCNT_PRIORITY(1));
    REG_BLDALPHA = EFFECT_BLEND_ADD;
    btResourceLoadAndDecompress(RESOURCE_BLAST_SHEET, state->sheet, 1, 0);
    // Smoke Bomb's colours are a ramp of greys; Blast takes the fire streak's.
    if (mode == ERUPTION_SMOKE_BOMB)
    {
        for (i = 0; i != SHEET_PALETTE_COLORS; i++)
        {
            Sint32 level = i / 2;

            BG_PLTT[i]   = RGB(level, level, level);
        }
        variant = EFFECT_VARIANT_STRONGER;
    }
    else
    {
        syWordsCopyEntry((void *)BG_PLTT,
                         syResourceGetTableEntry(RESOURCE_FIRE_STREAK_SHEET),
                         SHEET_PALETTE_BYTES);
        variant = args->variant;
    }

    // The puffs and motes burst up from beside the targets.
    for (i = 0; i != ERUPTION_PUFFS; i++)
    {
        PARTICLE *puff = &state->particles[i];

        if (args->side == EFFECT_SIDE_PARTY)
            puff->x = toFIXED(50);
        else
            puff->x = -toFIXED(50);
        puff->y = 0;
        puff->z = 0;
        SET_VELOCITY(puff,
                     ((syRandom() & 63) - 32) << 13,
                     ((syRandom() & 63) + 16) << 12,
                     ((syRandom() & 63) - 32) << 13);
        puff->variant = 0;
    }
    for (i = 0; i != PARTICLE_POOL_COUNT; i++)
    {
        PARTICLE *mote = &PARTICLES[i];

        if (args->side == EFFECT_SIDE_PARTY)
            mote->x = toFIXED(50);
        else
            mote->x = -toFIXED(50);
        mote->y = 0;
        mote->z = 0;
        SET_VELOCITY(mote,
                     ((syRandom() & 63) - 32) << 13,
                     ((syRandom() & 31) + 8) << 13,
                     ((syRandom() & 63) - 32) << 13);
        mote->variant = 0;
    }

    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
    draw[0] = RamHeapSlots->ramCode;
    EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

    for (frame = 0; frame != EruptionCounts[variant][ERUPTION_COUNT_TICKS]; frame++)
    {
        BATTLE_CAMERA *camera;

        camera = RamHeapSlots->cameraState;
        EFFECT_VIEW_FROM_CAMERA(camera);
        if (frame == 2)
            sySoundPlayStub(SE_CLATTER);
        if (frame == EruptionCounts[variant][ERUPTION_COUNT_TICKS] - 48)
            EFFECT_SHOW_DAMAGE(SE_IMPACT);

        // The motes fall.
        for (i = 0; i != EruptionCounts[variant][ERUPTION_COUNT_MOTES]; i++)
        {
            PARTICLE *mote = &PARTICLES[i];

            if (mote->y >= 0)
            {
                Sint32 size;

                btEffectPosProjectPoint((Sint32 *)mote, &screen);
                screen.x >>= 1;
                screen.x  += args->side * 32 - 16;
                CLAMP(screen.depth, ERUPTION_MOTE_DEPTH_NEAR, ERUPTION_MOTE_DEPTH_FAR);
                size = ERUPTION_MOTE_LARGEST
                     - (screen.depth - ERUPTION_MOTE_DEPTH_NEAR) / ERUPTION_MOTE_DEPTH_STEP;
                draw[0](canvas,
                        state->sheet
                            + (BattleEffectMotePictures[size - 1] + (i & 1) * MOTE_SET_BYTES)
                            + ERUPTION_MOTE_PICTURES,
                        screen.x - size / 2,
                        screen.y - size,
                        size,
                        size * 2);
                btParticleAdvanceWithGravity3D(mote, PARTICLE_DAMPING_NONE, -toFIXED(1) / 8);
            }
        }

        // The puffs, one more set out each tick.
        if (frame > 2)
        {
            for (i = 0; i != EruptionCounts[variant][ERUPTION_COUNT_PUFFS]; i++)
            {
                PARTICLE *puff = &state->particles[i];

                if (i < frame && puff->y >= 0)
                {
                    btEffectPosProjectPoint((Sint32 *)puff, &screen);
                    screen.x >>= 1;
                    screen.x  += args->side * 32 - 16;
                    if (puff->variant >= 0 && puff->variant < ERUPTION_PUFF_TICKS)
                    {
                        Sint32 picture = puff->variant / 3;
                        Uint32 width;

                        // UB: width is assigned in one argument and read in another.
#if FIX_UNSEQUENCED_SIDE_EFFECT
                        width = EruptionPuffSizes[picture];
                        draw[0](canvas,
                                state->sheet + EruptionPuffPictures[picture],
                                screen.x - (width >> 1),
                                screen.y - (width >> 1),
                                width,
                                width);
#else
                        draw[0](canvas,
                                state->sheet + EruptionPuffPictures[picture],
                                screen.x - ((width = EruptionPuffSizes[picture]) >> 1),
                                screen.y - (width >> 1),
                                width,
                                width);
#endif
                    }
                    if (puff->variant < ERUPTION_PUFF_TICKS)
                        puff->variant++;
                    btParticleAdvanceWithGravity3D(puff, PARTICLE_DAMPING_NONE, -toFIXED(1) / 8);
                }
            }
        }

        // The hits, one target a tick; Blast's targets also recoil.
        if (mode == ERUPTION_BLAST)
        {
            EFFECT_FOR_EACH_TARGET(i, args)
            {
                if (frame == i + 6)
                {
                    EFFECT_STRIKE_TARGET(args, i, 10, KNOCKBACK_LEAP);
                }
            }
        }
        else
        {
            EFFECT_FOR_EACH_TARGET(i, args)
            {
                if (frame == i + 6)
                    btEffectTintBattler(args->targets[i], SPRITE_COLOR_WHITE, POSE_DAMAGE, i, 10);
            }
        }
        if (frame == 2)
            state->shakeFrame = 6;
        btCameraShake(16, 16);
        EFFECT_END_TICK(state);
    }

    EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

// Creates the effect's sprites in its state: count sprites of spriteId, the nth showing its
// animation n, all at the given priority.
void btEffectCreateSprites(Sint32 count, Sint32 spriteId, Uint32 priority)
{
    BATTLE_EFFECT_STATE *state = RamHeapSlots->battleEffect;
    Sint32               i     = 0;

    if (count == 0)
        return;
    for (; i != count; i++)
    {
        FIELD_SPR *sprite = (FIELD_SPR *)fdSpriteCreateStub(spriteId);

        state->sprites[i] = sprite;
        if (sprite != NULL)
        {
            sprite->flags = 0;
            fdSpriteStartAnimationStub(sprite, i);
            ((FIELD_SPR *)state->sprites[i])->priority = priority;
        }
    }
}

// Does nothing; btEffectCallNoOperation calls it in a busy wait.
void btEffectNoOperation(void)
{
}

#undef ERUPTION_MOTE_PICTURES
#undef ERUPTION_PUFFS
#undef ERUPTION_PUFF_TICKS
#undef ERUPTION_MOTE_LARGEST
#undef ERUPTION_MOTE_DEPTH_NEAR
#undef ERUPTION_MOTE_DEPTH_FAR
#undef ERUPTION_MOTE_DEPTH_STEP

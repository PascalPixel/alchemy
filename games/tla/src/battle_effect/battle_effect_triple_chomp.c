// Triple Chomp, and the entry points of Beam, Rolling Flame, Ghost Soldier and Blast Breath. Part
// of battle_effect.c.

#define TRIPLE_CHOMP_TICKS            50
// The bites, closing at TripleChompBiteTicks' three ticks.
#define TRIPLE_CHOMP_BITES            3
// The canvas line each bite closes on, this far above the target's place.
#define TRIPLE_CHOMP_BITE_HEIGHT      24

// The sparks, TRIPLE_CHOMP_SPARK_COUNT of EXTRA_PARTICLES: each bite throws
// TRIPLE_CHOMP_BITE_SPARKS from the start of its own run of TRIPLE_CHOMP_BITE_SPARK_SLOTS.
#define TRIPLE_CHOMP_SPARK_COUNT      512
#define TRIPLE_CHOMP_BITE_SPARKS      32
#define TRIPLE_CHOMP_BITE_SPARK_SLOTS 128

// Each bite's ring, FireRingTexture on a square: the first three particles hold where each is
// spark. A ring swells from a quarter of its size by TRIPLE_CHOMP_RING_GROWTH a tick for
// TRIPLE_CHOMP_RING_TICKS, spark whole for the first half and then losing TRIPLE_CHOMP_RING_FADE of
// its shade a tick.
#define BITE_RING(bite)               (&state->particles[bite])
#define TRIPLE_CHOMP_RING_TICKS       16
#define TRIPLE_CHOMP_RING_GROWTH      toFIXED(0.15552)
#define TRIPLE_CHOMP_RING_FADE        8

// The user lunges most of the way to the target, and three bites close on it, TripleChompBiteTicks
// apart. Each throws blue sparks that fly off away from the user, jolts the target back and flashes
// it, and swells a ring of light over it, tilted back and turned a quarter further each bite, that
// fades as it grows. The last bite shows the damage.
//
// Abilities: Triple Chomp.
void btEffectRunTripleChomp(BATTLE_EFFECT_PARM *args)
{
    BATTLE_EFFECT_STATE *state;
    void                *canvas;
    Sint32               frame;
    // LEFTOVER(frame): draw[1], the idle half of the pair.
    BLITTER_PAIR         draw;
    Uint8               *graphics;
    void                *vertices;
    RENDER_MODEL        *model;
    MODEL_TEXTURE        texture;
    SCREEN_POINT         pos;
    PARTICLE            *spark;
    Sint32               i;
    Sint32               j;
    Uint8               *pixels = RamMapMetatileBuffer;

    state                       = RamHeapSlots->battleEffect;
    canvas                      = RamHeapSlots->battleCanvas;
    graphics                    = RamHeapSlots->battleSheet;

    // Set up the canvas, then let the user lunge.
    btEffectStartCanvasLayer(BGCNT_PRIORITY(1));
    REG_BLDALPHA = EFFECT_BLEND_ADD;
    EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
    btEffectSetApproachMotion(args->user, args->targets[0], 8, 80);
    sySynch(8);

    // A clipped, mirrored blitter keeping the brighter for the sparks; the ring's texture, the
    // spark sprite sheet and the blue flame's colours.
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
    draw[0] = RamHeapSlots->ramCode;
    btResourceLoadAndDecompress(RESOURCE_FIRE_RING_TEXTURE, pixels, 0, 0);
    btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, graphics, 0, 0);
    syWordsCopyEntry(
        (void *)BG_PLTT, syResourceGetTableEntry(RESOURCE_BLUE_FLAME_SHEET), SHEET_PALETTE_BYTES);

    for (frame = 0; frame != TRIPLE_CHOMP_TICKS; frame++)
    {
        if (frame == 0)
        {
            for (i = 0; i != TRIPLE_CHOMP_SPARK_COUNT; i++)
                EXTRA_PARTICLES[i].variant = -1;
        }

        // This tick's ring: one model drawing the square with the 128x128 texture.
        vertices = syHeapAllocateEWRAM(32);
        model    = btRenderAllocateModelList(1);
        btEffectPosProjectBattler(args->targets[0], &pos);
        texture.logWidth  = 7;
        texture.logHeight = 7;
        texture.pixels    = pixels;
        model->mesh       = BattleEffectLargeSquareMesh;
        model->kind       = MODEL_SHADED_ADDITIVE;
        model->texture    = &texture;
        model->vertices   = vertices;

        for (i = 0; i != TRIPLE_CHOMP_BITES; i++)
        {
            // The bite closes: its sparks fly from the target's middle, and the target jolts and
            // flashes. The last bite shows the damage.
            if (frame == TripleChompBiteTicks[i])
            {
                for (j = 0; j != TRIPLE_CHOMP_BITE_SPARKS; j++)
                {
                    PARTICLE *spark = &EXTRA_PARTICLES[i * TRIPLE_CHOMP_BITE_SPARK_SLOTS + j];
                    Sint32    angle;
                    Sint32    force;

                    spark->x         = toFIXED(pos.x / 2);
                    spark->y         = INTtoFIXED(pos.y - TRIPLE_CHOMP_BITE_HEIGHT);
                    angle            = syRandom();
                    force            = (syRandom() & 255) + 32;
                    spark->velocityX = (sySin(angle) * force) >> 8;
                    spark->velocityY = (syCos(angle) * force) >> 6;
                    if (args->side == EFFECT_SIDE_ENEMIES)
                        spark->velocityX += -toFIXED(1) / 2;
                    else
                        spark->velocityX += toFIXED(1) / 2;
                    BITE_RING(i)->x = pos.x;
                    BITE_RING(i)->y = pos.y - TRIPLE_CHOMP_BITE_HEIGHT;
                    spark->variant  = (syRandom() & 7) + 16;
                }
                if (i == TRIPLE_CHOMP_BITES - 1)
                    EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
                else
                    sySoundPlayStub(SE_HEAVY_IMPACT);
                btEffectStartActorMotion(
                    args->targets[0], 1, toFIXED(2), toFIXED(8), 0, i * 20 + 120);
                btEffectTintBattler(args->targets[0], SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 8);
                state->shakeFrame = 8;
            }

            // The bite's ring swells and fades.
            if (frame >= TripleChompBiteTicks[i]
                && frame < TripleChompBiteTicks[i] + TRIPLE_CHOMP_RING_TICKS)
            {
                Sint32 age   = frame - TripleChompBiteTicks[i];
                Sint32 shade = TRIPLE_CHOMP_RING_TICKS / 2 * TRIPLE_CHOMP_RING_FADE
                             - age * TRIPLE_CHOMP_RING_FADE;
                FIXED scale = age * TRIPLE_CHOMP_RING_GROWTH + toFIXED(1) / 4;

                if (shade > 0)
                    shade = 0;
                if (shade > MODEL_SHADE_INVISIBLE)
                {
                    model->shade = shade;
                    syInitMatrix();
                    syScale(toFIXED(1) / 2, toFIXED(1), toFIXED(1) / 2);
                    syTranslate(
                        INTtoFIXED(BITE_RING(i)->x - 128), INTtoFIXED(BITE_RING(i)->y - 64), 0);
                    syRotZ(i * (DEGtoANG(90)) + DEGtoANG(45));
                    syRotX(-TRIG_TURN / 16);
                    syMatrixScaleUniform(scale);
                    btRenderTransformVertices(BattleEffectGroundSquareCorners, vertices, 4);
                    btRenderDrawModelList(model);
                }
            }
        }

        // Each spark is spark and moved twice a tick, shrinking as it dies.
        spark = EXTRA_PARTICLES;
        for (i = 0; i != TRIPLE_CHOMP_SPARK_COUNT; i++)
        {
            Sint32 life = spark->variant;

            if (life >= 0)
            {
                Sint32 size = (life >> 2) + 3;

                EFFECT_DRAW_MOTE(draw[0],
                                 canvas,
                                 graphics,
                                 FIXED_INTEGER_PART(spark->x),
                                 FIXED_INTEGER_PART(spark->y),
                                 size);
                btParticleAdvanceWithGravity2D(spark, 62, 0);
                EFFECT_DRAW_MOTE(draw[0],
                                 canvas,
                                 graphics,
                                 FIXED_INTEGER_PART(spark->x),
                                 FIXED_INTEGER_PART(spark->y),
                                 size);
                btParticleAdvanceWithGravity2D(spark, 62, 0);
                spark->variant--;
            }
            spark++;
        }

        syHeapRelease(model);
        syHeapRelease(vertices);
        btCameraShake(8, 8);
        EFFECT_END_TICK(state);
    }

    syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
    syCloseEvent(btEffectTransferCanvas);
    btEffectEndCanvasLayer();
}

// Motes of fire gather on the user and a beam of fire shoots from it across the targets, a ring of
// light flying along it.
//
// Abilities: Beam, Cycle Beam, Searing Beam.
void btEffectRunBeam(BATTLE_EFFECT_PARM *args)
{
    btEffectRunBreaths(args, BREATH_BEAM);
}

// A ball of flame rolls from the user onto the targets and fire billows over them, always at the
// strongest variant.
//
// Abilities: Rolling Flame.
void btEffectRunRollingFlame(BATTLE_EFFECT_PARM *args)
{
    args->variant = EFFECT_VARIANT_STRONGEST;
    btEffectRunBreaths(args, BREATH_ROLLING_FLAME);
}

// A ghostly soldier with a pike appears in the user's stead, gathers violet light and fires a blast
// of blue light and a ring at the target, always at the strongest variant.
//
// Abilities: Ghost Soldier, Pike Assault.
void btEffectRunGhostSoldier(BATTLE_EFFECT_PARM *args)
{
    args->variant = EFFECT_VARIANT_STRONGEST;
    btEffectRunBreaths(args, BREATH_PIKE);
}

// A jet of fire streams from the user onto the targets, crossed rings of fire flaring where it
// lands.
//
// Abilities: Blast Breath.
void btEffectRunBlastBreath(BATTLE_EFFECT_PARM *args)
{
    btEffectRunBreaths(args, BREATH_BLAST);
}

#undef TRIPLE_CHOMP_TICKS
#undef TRIPLE_CHOMP_BITES
#undef TRIPLE_CHOMP_BITE_HEIGHT
#undef TRIPLE_CHOMP_SPARK_COUNT
#undef TRIPLE_CHOMP_BITE_SPARKS
#undef TRIPLE_CHOMP_BITE_SPARK_SLOTS
#undef BITE_RING
#undef TRIPLE_CHOMP_RING_TICKS
#undef TRIPLE_CHOMP_RING_GROWTH
#undef TRIPLE_CHOMP_RING_FADE

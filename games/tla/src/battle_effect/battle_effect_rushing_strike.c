// Rushing strikes (Bone Charge, Brute Force, Onslaught, Headbutt, Stun Muscle, Living Armor), and
// the handlers that play the mirrored wall. Part of battle_effect.c.

#define RUSHING_STRIKE_TICKS                   60
// the user strikes; the ring and sparks start
#define RUSHING_STRIKE_TICK                    8
// the burst hides the user rushing in
#define RUSHING_STRIKE_BURST_TICKS             15
#define RUSHING_STRIKE_RING_TICKS              12 // the ring where it hits swells
#define RUSHING_STRIKE_SPARK_COUNT             64 // in EXTRA_PARTICLES
// The first two sparks' ticks to live; each pair after lives one more.
#define RUSHING_STRIKE_SPARK_LIFE              16
#define RUSHING_STRIKE_EXPLOSIONS              16 // Living Armor's
// The explosions set up at the hit, of which RUSHING_STRIKE_EXPLOSIONS are drawn.
#define RUSHING_STRIKE_EXPLOSION_SLOTS         64
#define RUSHING_STRIKE_EXPLOSION_PICTURE_TICKS 4
// FlashBurstSheet's six pictures, four ticks apiece.
#define RUSHING_STRIKE_EXPLOSION_TICKS         24

// The burst: FlamePlumeSheet's 32x32 plume above the same plume upside down.
#define RUSHING_STRIKE_BURST_WIDTH             32
#define RUSHING_STRIKE_BURST_HEIGHT            64
#define RUSHING_STRIKE_PLUME_SIZE              32
// Where FlashBurstSheet is decoded in the effect's pictures, after the burst.
#define RUSHING_STRIKE_EXPLOSION_PICTURES      PICTURE_BYTES(RUSHING_STRIKE_BURST)

// The user rushes at the first target in a burst of light; a ring flares where it hits and sparks
// scatter, and Living Armor's hit raises explosions.
void btEffectRunRushingStrike(BATTLE_EFFECT_PARM *args, Sint32 mode)
{
    MODEL_TEXTURE        texture;
    DRAW_RECTANGLE       draw[2];
    Uint8               *ringPixels = RamMapMetatileBuffer;
    Sint32               height;
    SCREEN_POINT         target;
    SCREEN_POINT         caster;
    SCREEN_POINT         screen;
    BATTLE_EFFECT_STATE *state  = RamHeapSlots->battleEffect;
    void                *canvas = RamHeapSlots->battleCanvas;
    Sint32               frame;
    BATTLE_CAMERA       *camera       = RamHeapSlots->cameraState;
    Uint8               *motePictures = RamHeapSlots->battleSheet;
    MOTION_OBJECT       *targetObject;
    RENDER_MODEL        *quad;
    void                *vertices;
    Sint32               i;
    Sint32               row;
    Sint32               column;
    Sint32               scale;
    Sint32               ringFrame;
    Sint32               brightness;
    Sint32               resource;

    btEffectStartCanvasLayer(0);
    REG_BLDALPHA = EFFECT_BLEND_ADD;
    btEffectBuildCanvasBlitters(0, draw);
    btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, motePictures, 0, 0);
    btResourceLoadAndDecompress(RESOURCE_FIRE_RING_TEXTURE, RamMapMetatileBuffer, 0, 0);

    // The burst: the plume, then the same plume upside down below it.
    btResourceLoadAndDecompress(RESOURCE_FLAME_PLUME_SHEET, state->sheet, 0, 0);
    for (row = 0; row != RUSHING_STRIKE_PLUME_SIZE; row++)
        for (column = 0; column != RUSHING_STRIKE_PLUME_SIZE; column++)
            state->sheet[RUSHING_STRIKE_PLUME_SIZE * RUSHING_STRIKE_PLUME_SIZE
                         + (RUSHING_STRIKE_PLUME_SIZE - 1 - row) * RUSHING_STRIKE_PLUME_SIZE
                         + column] = state->sheet[row * RUSHING_STRIKE_PLUME_SIZE + column];
    btResourceLoadAndDecompress(
        RESOURCE_FLASH_BURST_SHEET, state->sheet + RUSHING_STRIKE_EXPLOSION_PICTURES, 1, 0);
    switch (mode)
    {
        case RUSHING_STRIKE_UNUSED      : resource = RESOURCE_FIRE_PILLAR_SHEET_B; break;
        case RUSHING_STRIKE_BRUTE_FORCE : resource = RESOURCE_ICE_BLOCK_SHEET; break;
        case RUSHING_STRIKE_ONSLAUGHT   : resource = RESOURCE_PINK_BURST_SHEET; break;
        case RUSHING_STRIKE_HEADBUTT    :
        case RUSHING_STRIKE_LIVING_ARMOR: resource = RESOURCE_BLAST_SHEET; break;
        default                         : resource = RESOURCE_ORANGE_PALETTE_B; break;
    }
    syWordsCopyEntry((void *)BG_PLTT, syResourceGetTableEntry(resource), SHEET_PALETTE_BYTES);
    EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
    sySetEvent((Sint32)btEffectTransferCanvas, EVENT_PASS_VBLANK);

    // The user rushes at the target.
    btEffectSetApproachMotion(args->user, args->targets[0], 10, 80);
    fdActorSetAnimationSpeedStub(btObjectGetStub(args->user)->object, ANIM_STEP_NORMAL * 3);
    targetObject = btObjectGetStub(args->targets[0])->object;
    btEffectPosProjectBattler(args->targets[0], &target);

    for (frame = 0; frame != RUSHING_STRIKE_TICKS; frame++)
    {
        // The first tick readies the sparks and Living Armor's explosions at the target, 8 pixels
        // out from it, flying back the way the user came.
        if (frame == 0)
        {
            PARTICLE *spark = EXTRA_PARTICLES;

            for (i = 0; i != RUSHING_STRIKE_SPARK_COUNT; i++, spark++)
            {
                spark->x = targetObject->x;
                if (spark->x < 0)
                    spark->x += -toFIXED(8);
                else
                    spark->x += toFIXED(8);
                spark->y         = targetObject->y + toFIXED(14);
                spark->z         = targetObject->z;
                spark->velocityX = (syRandom() & 511) << 11;
                spark->velocityY = ((syRandom() & 255) - 64) << 11;
                spark->velocityZ = ((syRandom() & 255) - 128) << 11;
                if (spark->x > 0)
                    spark->velocityX = -spark->velocityX;
                spark->variant = i / 2 + RUSHING_STRIKE_SPARK_LIFE;
            }
            for (i = 0; i != RUSHING_STRIKE_EXPLOSION_SLOTS; i++)
            {
                PARTICLE *explosion = &state->particles[i];
                explosion->x        = targetObject->x;
                if (explosion->x < 0)
                    explosion->x += -toFIXED(8);
                else
                    explosion->x += toFIXED(8);
                explosion->y         = targetObject->y + toFIXED(14);
                explosion->z         = targetObject->z;
                explosion->velocityX = ((syRandom() & 511) - 256) << 10;
                explosion->velocityY = ((syRandom() & 255) - 64) << 10;
                explosion->velocityZ = ((syRandom() & 255) - 128) << 10;
                if (explosion->x > 0)
                    explosion->velocityX = -explosion->velocityX;
                explosion->variant = 0;
            }
        }

        // The burst at the user as it rushes in.
        btEffectPosProjectBattler(args->user, &caster);
        vertices          = syHeapAllocateEWRAM(32);
        quad              = btRenderAllocateModelList(1);
        texture.logWidth  = 5;
        brightness        = 0;
        texture.logHeight = 6;
        texture.pixels    = state->sheet;
        quad->texture     = &texture;
        quad->mesh        = BattleEffectTallMesh;
        quad->kind        = MODEL_SHADED_MAX;
        quad->vertices    = vertices;
        quad->shade       = brightness;
        if (frame < RUSHING_STRIKE_BURST_TICKS)
        {
            syInitMatrix();
            syScale(toFIXED(1) / 2, toFIXED(1), toFIXED(1) / 2);
            syTranslate(INTtoFIXED(caster.x - 128), INTtoFIXED(caster.y - 88), 0);
            syScale(RushingStrikeScales[mode] << 1,
                    RushingStrikeScales[mode] << 1,
                    RushingStrikeScales[mode]);
            if (args->side == EFFECT_SIDE_PARTY)
                syRotY(DEGtoANG(180));
            btRenderTransformVertices(BattleEffectSquareCorners, vertices, 4);
            btRenderDrawModelList(quad);
        }

        // The ring where it hits swells, four times as tall as it is wide, and darkens after its
        // fifth tick.
        ringFrame = frame - RUSHING_STRIKE_TICK;
        if (frame >= RUSHING_STRIKE_TICK && frame < RUSHING_STRIKE_TICK + RUSHING_STRIKE_RING_TICKS)
        {
            if (mode == RUSHING_STRIKE_STUN_MUSCLE)
                scale = ringFrame * (toFIXED(1) / 16) + toFIXED(1) / 16;
            else
                scale = ringFrame * 3048 + 3048;
            brightness = 32 - ringFrame * 8;
            if (brightness > 0)
                brightness = 0;
            quad->shade       = brightness;
            texture.logWidth  = 7;
            texture.logHeight = 7;
            texture.pixels    = ringPixels;
            quad->kind        = MODEL_SHADED_ADDITIVE;
            quad->mesh        = BattleEffectLargeSquareMesh;
            syInitMatrix();
            syScale(toFIXED(1) / 2, toFIXED(1), toFIXED(1) / 2);
            if (args->side == EFFECT_SIDE_ENEMIES)
                syTranslate(INTtoFIXED(target.x - 144), INTtoFIXED(target.y - 84), 0);
            else
                syTranslate(INTtoFIXED(target.x - 112), INTtoFIXED(target.y - 84), 0);
            syScale(scale, scale << 2, scale);
            btRenderTransformVertices(BattleEffectSquareCorners, vertices, 4);
            btRenderDrawModelList(quad);
        }
        syHeapRelease(quad);
        syHeapRelease(vertices);

        if (frame == RUSHING_STRIKE_TICK)
        {
            EFFECT_STRIKE_TARGET(args, 0, 8, KNOCKBACK_HURL);
            if (mode == RUSHING_STRIKE_LIVING_ARMOR)
                EFFECT_SHOW_DAMAGE(SE_CRASH);
            else
                EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
            state->shakeFrame = 8;
        }

        // From the hit on, the sparks fly and burn down, drawn by turns with the two blitters;
        // their sprites shrink as they burn. DEAD: tick < 64 is always true, as the loop ends at
        // RUSHING_STRIKE_TICKS; the ROM keeps the compare.
        if (frame >= RUSHING_STRIKE_TICK && frame < 64)
        {
            PARTICLE *spark;

            syInitMatrix();
            syLookAtLoad(camera->eye, camera->focus);
            spark = EXTRA_PARTICLES;
            for (i = 0; i != RUSHING_STRIKE_SPARK_COUNT; i++, spark++)
            {
                Sint32 life = spark->variant;

                if (life >= 0)
                {
                    Sint32 size = (life >> 4) + 2;

                    btEffectPosProjectPoint(&spark->x, &screen);
                    screen.x >>= 1;
                    height     = size * 2;
                    draw[i & 1](canvas,
                                motePictures + BattleEffectMotePictures[size - 1],
                                screen.x - size / 2,
                                screen.y - size,
                                size,
                                height);
                    btParticleAdvanceWithGravity3D(spark, 60, -toFIXED(1) / 128);
                    spark->variant--;
                }
            }

            // Living Armor's explosions rise, flashing and burning out.
            if (mode == RUSHING_STRIKE_LIVING_ARMOR)
            {
                for (i = 0; i != RUSHING_STRIKE_EXPLOSIONS; i++)
                {
                    PARTICLE *explosion = &state->particles[i];
                    Sint32    life      = explosion->variant;

                    if (life >= 0 && life < RUSHING_STRIKE_EXPLOSION_TICKS)
                    {
                        Sint32 picture = life / RUSHING_STRIKE_EXPLOSION_PICTURE_TICKS;

                        btEffectPosProjectPoint(&explosion->x, &screen);
                        screen.x >>= 1;
                        draw[0](canvas,
                                state->sheet + RUSHING_STRIKE_EXPLOSION_PICTURES
                                    + picture * PICTURE_BYTES(FLASH_BURST),
                                screen.x - FLASH_BURST_WIDTH / 2,
                                screen.y - FLASH_BURST_HEIGHT / 2,
                                FLASH_BURST_WIDTH,
                                FLASH_BURST_HEIGHT);
                        btParticleAdvanceWithGravity3D(explosion, 60, toFIXED(1) / 8);
                        explosion->variant++;
                    }
                }
            }
        }

        btCameraShake(8, 8);
        EFFECT_END_TICK(state);
    }

    syCloseEvent((Sint32)btEffectTransferCanvas);
    EFFECT_RELEASE_BLITTERS();
    btEffectEndCanvasLayer();
}

// Abilities: Spasm.
void btEffectRunSpasm(BATTLE_EFFECT_PARM *args)
{
    btEffectRunMirroredWall(args, MIRRORED_WALL_SPASM);
}

// Abilities: Berserk.
void btEffectRunBerserk(BATTLE_EFFECT_PARM *args)
{
    btEffectRunMirroredWall(args, MIRRORED_WALL_BERSERK);
}

// Abilities: Recovery.
void btEffectRunRecovery(BATTLE_EFFECT_PARM *args)
{
    btEffectRunMirroredWall(args, MIRRORED_WALL_RECOVERY);
}

// Abilities: Sack.
void btEffectRunSack(BATTLE_EFFECT_PARM *args)
{
    btEffectRunMirroredWall(args, MIRRORED_WALL_SACK);
}

#undef RUSHING_STRIKE_TICKS
#undef RUSHING_STRIKE_TICK
#undef RUSHING_STRIKE_BURST_TICKS
#undef RUSHING_STRIKE_RING_TICKS
#undef RUSHING_STRIKE_SPARK_COUNT
#undef RUSHING_STRIKE_SPARK_LIFE
#undef RUSHING_STRIKE_EXPLOSIONS
#undef RUSHING_STRIKE_EXPLOSION_SLOTS
#undef RUSHING_STRIKE_EXPLOSION_PICTURE_TICKS
#undef RUSHING_STRIKE_EXPLOSION_TICKS
#undef RUSHING_STRIKE_BURST_WIDTH
#undef RUSHING_STRIKE_BURST_HEIGHT
#undef RUSHING_STRIKE_PLUME_SIZE
#undef RUSHING_STRIKE_EXPLOSION_PICTURES

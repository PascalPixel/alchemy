// Core and Cannon, a Mars Djinni charging the target, and the entries of Rime and Haze. Part of
// battle_effect.c.

#define DJINNI_CHARGE_DJINNI_PICTURE         4096 // the small Mars Djinni

// The last two particles: the one that keeps the camera's focus at the start, and the one that
// holds the Djinni's offset from it and its step toward the target. The debris are the particles
// before them.
#define DJINNI_CHARGE_START                  62
#define DJINNI_CHARGE_STEP                   63
// Cannon's eight flash bursts, in the particles after its debris.
#define DJINNI_CHARGE_FIRST_BURST            54
// Where GlowOrbSheet (Core) or FlashBurstSheet (Cannon) is decoded in the effect's pictures.
#define DJINNI_CHARGE_BURST_PICTURES         8192
// Cannon's blast trailing from the Djinni: PetalSheet's four cones of light, decoded into the map
// cell buffer.
#define DJINNI_CHARGE_BLAST_WIDTH            60
#define DJINNI_CHARGE_BLAST_HEIGHT           100

// While the Djinni pulls back, its step toward the target grows by a 1024th of the way a tick; as
// it drives in, by a 128th.
#define DJINNI_CHARGE_PULL_BACK_SHIFT        10
#define DJINNI_CHARGE_DRIVE_SHIFT            7
// The debris each of Core's strikes throws.
#define DJINNI_CHARGE_STRIKE_DEBRIS          16

// Core's ball of fire: DustBlobSheet in the orange palette on a sphere of
// DJINNI_CHARGE_FIREBALL_RINGS rings of DJINNI_CHARGE_FIREBALL_RING_VERTICES, from pole to pole,
// rolled DJINNI_CHARGE_FIREBALL_TILT one way or the other by the targets' side. Its corners are in
// the map cell buffer, its projected corners and its mesh after them.
#define DJINNI_CHARGE_FIREBALL_RINGS         8
#define DJINNI_CHARGE_FIREBALL_RING_VERTICES 8
#define DJINNI_CHARGE_FIREBALL_VERTICES                                                            \
    (DJINNI_CHARGE_FIREBALL_RINGS * DJINNI_CHARGE_FIREBALL_RING_VERTICES)
#define DJINNI_CHARGE_FIREBALL_RING_TURN (DEGtoANG(180) / (DJINNI_CHARGE_FIREBALL_RINGS - 1))
#define DJINNI_CHARGE_FIREBALL_RADIUS    48
#define DJINNI_CHARGE_FIREBALL_TILT      20576
#define DJINNI_CHARGE_FIREBALL_PROJECTED (RamMapMetatileBuffer + 8192)
#define DJINNI_CHARGE_FIREBALL_MESH      (RamMapMetatileBuffer + 16384)

// The small Mars Djinni charges the target. In Core it pulls back over the caster inside a ball of
// fire, then drives in onto the target as the ball swells, strikes it seven times, throwing debris,
// and turns into a glowing orb that burns out; in Cannon it shoots straight in, a cone of blast
// trailing from it, and strikes once in a burst of flame and debris.
//
// Abilities: Core, Cannon.
void btEffectRunDjinniCharge(BATTLE_EFFECT_PARM *args, Sint32 mode)
{
    Uint8               *mesh;
    Uint8               *projected;
    Uint8               *cells;
    void                *canvas;
    BATTLE_EFFECT_STATE *state;
    // LEFTOVER(frame): blit[1], the pair's idle second blitter, which the ROM's
    // * tick reserves and never touches; as one DRAW_RECTANGLE.
    BLITTER_PAIR         blit;
    Sint32               frame;
    BATTLE_CAMERA       *camera;
    Uint8               *motes;
    Sint32               focus[3];
    SCREEN_POINT         screen;
    SCREEN_POINT         debris;
    MODEL_TEXTURE        texture;
    RENDER_MODEL        *model;
    PARTICLE            *p;
    Sint32               count;
    Sint32               i;

    mesh      = DJINNI_CHARGE_FIREBALL_MESH;
    projected = DJINNI_CHARGE_FIREBALL_PROJECTED;
    cells     = MapGridMetatileBuffer;
    state     = RamHeapSlots->battleEffect;
    canvas    = RamHeapSlots->battleCanvas;
    camera    = RamHeapSlots->cameraState;
    motes     = RamHeapSlots->battleSheet;
    btEffectStartCanvasLayer(0);
    btEffectRunDjinniEntrance(
        args,
        DJINNI_ENTRANCE(ELEMENT_MARS, args->side, DjinniChargeEntranceFlags[mode]),
        (Sint32 *)&screen,
        focus);
    {
        MOTION_OBJECT *target;
        Sint16         savedYaw;
        void          *caster;
        Sint32         ticks;
        REG_BG2CNT =
            BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
        if (args->side == EFFECT_SIDE_ENEMIES)
        {
            EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
        }
        else
        {
            EFFECT_LOAD_BLITTER(
                HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_ADDITIVE);
        }
        blit[0]      = RamHeapSlots->ramCode;
        REG_BLDALPHA = EFFECT_BLEND_ADD;
        EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
        target                                  = btObjectGetStub(args->targets[0])->object;
        savedYaw                                = camera->yaw;
        caster                                  = btObjectGetStub(args->user)->object;

        // The last two particles keep the Djinni's start and its step.
        state->particles[DJINNI_CHARGE_START].x = focus[0];
        state->particles[DJINNI_CHARGE_START].y = focus[1];
        state->particles[DJINNI_CHARGE_START].z = focus[2];
        btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, motes, 0, 0);
        btResourceLoadAndDecompress(
            RESOURCE_MARS_DJINN_SMALL_SHEET, state->sheet + DJINNI_CHARGE_DJINNI_PICTURE, 1, 1);
        if (mode == DJINNI_CHARGE_CORE)
        {
            btResourceLoadAndDecompress(RESOURCE_DUST_BLOB_SHEET, state->sheet, 1, 1);
            btResourceLoadAndDecompress(
                RESOURCE_GLOW_ORB_SHEET, state->sheet + DJINNI_CHARGE_BURST_PICTURES, 1, 1);
            LOAD_BG_PALETTE(RESOURCE_ORANGE_PALETTE_B);
            ticks = 102;
        }
        else
        {
            btResourceLoadAndDecompress(RESOURCE_PETAL_SHEET, cells, 1, 0);
            btResourceLoadAndDecompress(
                RESOURCE_FLASH_BURST_SHEET, state->sheet + DJINNI_CHARGE_BURST_PICTURES, 1, 0);
            ticks = 50;
        }
        REG_BG2PA = Q_8_8(0.5);
        fdActorSetAnimationSpeedStub(caster, ANIM_STEP_NORMAL * 15);

        for (frame = 0; frame != ticks; frame++)
        {
            if (frame == 0)
            {
                camera->yaw                            = savedYaw;
                focus[0]                               = state->particles[DJINNI_CHARGE_START].x;
                focus[1]                               = state->particles[DJINNI_CHARGE_START].y;
                focus[2]                               = state->particles[DJINNI_CHARGE_START].z;
                state->particles[DJINNI_CHARGE_STEP].x = 0;
                state->particles[DJINNI_CHARGE_STEP].y = 0;
                state->particles[DJINNI_CHARGE_STEP].z = 0;
                state->particles[DJINNI_CHARGE_STEP].velocityX = target->x - focus[0];
                // DEAD: both arms alike; the ROM keeps the test.
                if (mode == DJINNI_CHARGE_CORE)
                {
                    state->particles[DJINNI_CHARGE_STEP].velocityY =
                        target->y - focus[1] + toFIXED(30);
                }
                else
                {
                    state->particles[DJINNI_CHARGE_STEP].velocityY =
                        target->y - focus[1] + toFIXED(30);
                }
                state->particles[DJINNI_CHARGE_STEP].velocityZ = target->z - focus[2];
                for (i = 0; i != DJINNI_CHARGE_START; i++)
                {
                    state->particles[i].variant = -1;
                }
                if (mode == DJINNI_CHARGE_CANNON)
                {
                    sySoundPlayStub(SE_CRASH);
                }
            }
            syInitMatrix();
            syLookAtLoad(camera->eye, camera->focus);

            if (mode == DJINNI_CHARGE_CORE)
            {
                // The camera turns for the first 16 ticks.
                if ((Uint32)frame <= 15)
                {
                    Sint32 turn;

                    // DEAD: tick is at most 15 here, so the second arm never runs; the ROM keeps
                    // its test.
                    if (frame <= 71)
                    {
                        turn = TRIG_TURN / 256;
                    }
                    else
                    {
                        turn = 2560 - (frame << 5);
                    }
                    if (args->side == EFFECT_SIDE_ENEMIES)
                    {
                        camera->yaw -= turn;
                    }
                    else
                    {
                        camera->yaw += turn;
                    }
                }
                if (frame <= 31)
                {
                    // Pull back...
                    focus[0] -= state->particles[DJINNI_CHARGE_STEP].x;
                    focus[1] -= state->particles[DJINNI_CHARGE_STEP].y;
                    focus[2] -= state->particles[DJINNI_CHARGE_STEP].z;
                    if (frame <= 15)
                    {
                        state->particles[DJINNI_CHARGE_STEP].x +=
                            state->particles[DJINNI_CHARGE_STEP].velocityX
                            >> DJINNI_CHARGE_PULL_BACK_SHIFT;
                        state->particles[DJINNI_CHARGE_STEP].y +=
                            state->particles[DJINNI_CHARGE_STEP].velocityY
                            >> DJINNI_CHARGE_PULL_BACK_SHIFT;
                        state->particles[DJINNI_CHARGE_STEP].z +=
                            state->particles[DJINNI_CHARGE_STEP].velocityZ
                            >> DJINNI_CHARGE_PULL_BACK_SHIFT;
                    }
                    state->particles[DJINNI_CHARGE_STEP].x =
                        state->particles[DJINNI_CHARGE_STEP].x * 56 / DAMPING_SCALE;
                    state->particles[DJINNI_CHARGE_STEP].y =
                        state->particles[DJINNI_CHARGE_STEP].y * 56 / DAMPING_SCALE;
                    state->particles[DJINNI_CHARGE_STEP].z =
                        state->particles[DJINNI_CHARGE_STEP].z * 56 / DAMPING_SCALE;
                    if (frame == 31)
                    {
                        sySoundPlayStub(SE_ITEM_BREAK);
                    }
                }
                else if (frame <= 45)
                {
                    // ...drive in...
                    focus[0] += state->particles[DJINNI_CHARGE_STEP].x;
                    focus[1] += state->particles[DJINNI_CHARGE_STEP].y;
                    focus[2] += state->particles[DJINNI_CHARGE_STEP].z;
                    state->particles[DJINNI_CHARGE_STEP].x +=
                        state->particles[DJINNI_CHARGE_STEP].velocityX >> DJINNI_CHARGE_DRIVE_SHIFT;
                    state->particles[DJINNI_CHARGE_STEP].y +=
                        state->particles[DJINNI_CHARGE_STEP].velocityY >> DJINNI_CHARGE_DRIVE_SHIFT;
                    state->particles[DJINNI_CHARGE_STEP].z +=
                        state->particles[DJINNI_CHARGE_STEP].velocityZ >> DJINNI_CHARGE_DRIVE_SHIFT;
                }
                else if (frame <= 72 && !(frame & 3))
                {
                    Sint32 thrown  = 0;

                    // ...and strike, every fourth tick.
                    focus[0]      += state->particles[DJINNI_CHARGE_STEP].x / 2;
                    focus[1]      += state->particles[DJINNI_CHARGE_STEP].y / 2;
                    focus[2]      += state->particles[DJINNI_CHARGE_STEP].z / 2;
                    if (frame == 72)
                    {
                        EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
                        state->shakeFrame = 8;
                    }
                    else
                    {
                        sySoundPlayStub(SE_ITEM_BREAK);
                        state->shakeFrame = 2;
                    }
                    btEffectTintBattler(args->targets[0], SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 6);
                    for (i = 0, p = state->particles; i != DJINNI_CHARGE_STEP; i++, p++)
                    {
                        if (p->variant != -1)
                        {
                            continue;
                        }
                        SET_POSITION(p, target->x, target->y + toFIXED(5), target->z);
                        p->velocityX = ((syRandom() & 511) + 256) << 10;
                        if (args->side == EFFECT_SIDE_PARTY)
                        {
                            p->velocityX = -p->velocityX;
                        }
                        p->velocityY = ((syRandom() & 511) + 256) << 9;
                        p->velocityZ = 0;
                        p->variant   = (syRandom() & 15) + 16;
                        if (++thrown == DJINNI_CHARGE_STRIKE_DEBRIS)
                        {
                            break;
                        }
                    }
                }
            }
            else
            {
                if ((Uint32)frame <= 22)
                {
                    focus[0] += state->particles[DJINNI_CHARGE_STEP].x;
                    focus[1] += state->particles[DJINNI_CHARGE_STEP].y;
                    focus[2] += state->particles[DJINNI_CHARGE_STEP].z;
                    state->particles[DJINNI_CHARGE_STEP].x +=
                        state->particles[DJINNI_CHARGE_STEP].velocityX >> DJINNI_CHARGE_DRIVE_SHIFT;
                    state->particles[DJINNI_CHARGE_STEP].y +=
                        state->particles[DJINNI_CHARGE_STEP].velocityY >> DJINNI_CHARGE_DRIVE_SHIFT;
                    state->particles[DJINNI_CHARGE_STEP].z +=
                        state->particles[DJINNI_CHARGE_STEP].velocityZ >> DJINNI_CHARGE_DRIVE_SHIFT;
                }
                if (frame == 15)
                {
                    state->shakeFrame = 6;
                    EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
                    EFFECT_STRIKE_TARGET(args, 0, 6, KNOCKBACK_NUDGE);
                    for (i = 0; i != DJINNI_CHARGE_STEP; i++)
                    {
                        Sint32 angle;
                        Sint32 speed;

                        p = &state->particles[i];
                        SET_POSITION(p, target->x, target->y + toFIXED(10), target->z);
                        angle = (syRandom() & 0x7fff) - DEGtoANG(90);
                        // DEAD: both arms alike; the ROM keeps the test and both calls.
                        if (i >= DJINNI_CHARGE_FIRST_BURST)
                        {
                            speed = (syRandom() & 511) + 256;
                        }
                        else
                        {
                            speed = (syRandom() & 511) + 256;
                        }
                        p->velocityX = (sySin(angle) * speed) >> 6;
                        p->velocityY = (syCos(angle) * speed) >> 6;
                        p->velocityZ = 0;
                        p->variant   = (syRandom() & 15) + 16;
                    }
                }
            }

            // The Djinni, which in Core turns into a glowing orb, and Cannon's blast.
            btEffectPosProjectPoint(focus, &screen);
            if (mode == DJINNI_CHARGE_CORE)
            {
                if (frame <= 73)
                {
                    blit[0](canvas,
                            state->sheet + DJINNI_CHARGE_DJINNI_PICTURE,
                            screen.x / 2 - SMALL_DJINNI_WIDTH / 2,
                            screen.y - SMALL_DJINNI_HEIGHT / 2,
                            SMALL_DJINNI_WIDTH,
                            SMALL_DJINNI_HEIGHT);
                }
                else if (frame <= 93)
                {
                    blit[0](canvas,
                            state->sheet + DJINNI_CHARGE_BURST_PICTURES
                                + ((frame - 74) / 4) * PICTURE_BYTES(GLOW_ORB),
                            screen.x / 2 - GLOW_ORB_WIDTH / 2,
                            screen.y - GLOW_ORB_HEIGHT / 2,
                            GLOW_ORB_WIDTH,
                            GLOW_ORB_HEIGHT);
                }
            }
            else if (frame <= 22)
            {
                blit[0](canvas,
                        state->sheet + DJINNI_CHARGE_DJINNI_PICTURE,
                        screen.x / 2 - SMALL_DJINNI_WIDTH / 2,
                        screen.y - SMALL_DJINNI_HEIGHT / 2,
                        SMALL_DJINNI_WIDTH,
                        SMALL_DJINNI_HEIGHT);
                if (mode == DJINNI_CHARGE_CANNON && frame > 1)
                {
                    Sint32 picture = (frame / 4) & 3;
                    Sint32 x;

                    if (args->side == EFFECT_SIDE_ENEMIES)
                    {
                        x = screen.x / 2 - 14;
                    }
                    else
                    {
                        x = screen.x / 2 - 46;
                    }
                    blit[0](canvas,
                            RamMapMetatileBuffer + picture * PICTURE_BYTES(DJINNI_CHARGE_BLAST),
                            x,
                            screen.y - DJINNI_CHARGE_BLAST_HEIGHT / 2,
                            DJINNI_CHARGE_BLAST_WIDTH,
                            DJINNI_CHARGE_BLAST_HEIGHT);
                }
            }

            // Debris, bouncing.
            count = DJINNI_CHARGE_START;
            if (mode != DJINNI_CHARGE_CORE)
            {
                count = DJINNI_CHARGE_FIRST_BURST;
            }
            for (i = 0; i != count; i++)
            {
                Sint32 size = state->particles[i].variant;

                if (size < 0)
                {
                    continue;
                }
                btEffectPosProjectPoint(&state->particles[i], &debris);
                size >>= 3;
                size  += 2;
                blit[0](canvas,
                        motes + BattleEffectMotePictures[size - 1],
                        debris.x / 2 - size / 2,
                        debris.y - size,
                        size,
                        size * 2);
                btParticleAdvanceWithGravity3D(
                    &state->particles[i], 62, DjinniChargeDebrisGravity[i & 3]);
                if (state->particles[i].y < 0)
                {
                    state->particles[i].velocityY = -state->particles[i].velocityY / 2;
                }
                state->particles[i].variant--;
            }
            if (mode == DJINNI_CHARGE_CANNON)
            {
                for (i = 0; i != DJINNI_CHARGE_START - DJINNI_CHARGE_FIRST_BURST; i++)
                {
                    Sint32 picture;

                    p = &state->particles[DJINNI_CHARGE_FIRST_BURST + i];

                    if (p->variant < 0)
                    {
                        continue;
                    }
                    picture = 5 - p->variant / 6;
                    btEffectPosProjectPoint(p, &debris);
                    blit[0](canvas,
                            state->sheet + DJINNI_CHARGE_BURST_PICTURES
                                + picture * PICTURE_BYTES(FLASH_BURST),
                            debris.x / 2 - FLASH_BURST_WIDTH / 2,
                            debris.y - FLASH_BURST_HEIGHT / 2,
                            FLASH_BURST_WIDTH,
                            FLASH_BURST_HEIGHT);
                    btParticleAdvanceWithGravity3D(p, 62, -toFIXED(1) / 64);
                    p->variant--;
                }
            }
            if (mode == DJINNI_CHARGE_CORE)
            {
                if (frame == 24)
                {
                    btEffectTintBattler(args->user, TINT_KEEP, POSE_LEAP, 0, -1);
                }
            }
            else if (frame == 0)
            {
                btEffectTintBattler(args->user, TINT_KEEP, POSE_LEAP, 0, -1);
            }

            // Core's ball of fire, its sphere built on the first tick.
            if (mode == DJINNI_CHARGE_CORE && frame <= 89)
            {
                FIXED scale;

                model = btRenderAllocateModelList(1);
                if (frame == 0)
                {
                    Sint32 row;
                    Sint32 angle;

                    btMeshBuildGrid(mesh,
                                    DJINNI_CHARGE_FIREBALL_RING_VERTICES,
                                    DJINNI_CHARGE_FIREBALL_RINGS,
                                    8,
                                    8);
                    for (row = 0, angle = 0; row != DJINNI_CHARGE_FIREBALL_RINGS;
                         angle += DJINNI_CHARGE_FIREBALL_RING_TURN, row++)
                    {
                        Sint32 y      = FIXEDtoINT(-syCos(angle) * DJINNI_CHARGE_FIREBALL_RADIUS);
                        Sint32 radius = FIXEDtoINT(sySin(angle) * DJINNI_CHARGE_FIREBALL_RADIUS);
                        MODEL_VERTEX *vertex = (MODEL_VERTEX *)cells;
                        Sint32        k;

                        for (k = 0; k != DJINNI_CHARGE_FIREBALL_RING_VERTICES; k++)
                        {
                            Sint32 turn = k * (TRIG_TURN / DJINNI_CHARGE_FIREBALL_RING_VERTICES);
                            Sint32 x    = FIXEDtoINT(sySin(turn) * radius);
                            MODEL_VERTEX *corner =
                                &vertex[row * DJINNI_CHARGE_FIREBALL_RING_VERTICES + k];

                            corner->y = y;
                            corner->x = x;
                            corner->z = FIXEDtoINT(syCos(turn) * radius);
                        }
                    }
                }
                texture.logWidth  = 5;
                texture.logHeight = 5;
                texture.pixels    = state->sheet;
                syScale(toFIXED(1) / 2, toFIXED(1), toFIXED(1) / 2);
                ProjectionWork.centerX = BATTLE_PROJECTION_CENTER_X / 2;
                syMatrixTranslateVector(focus);
                syTranslate(0, toFIXED(16), 0);
                if (args->side == EFFECT_SIDE_ENEMIES)
                {
                    syRotZ(DJINNI_CHARGE_FIREBALL_TILT);
                }
                else
                {
                    syRotZ(TRIG_TURN - DJINNI_CHARGE_FIREBALL_TILT);
                }
                scale = toFIXED(5) / 8;
                if (frame > 31)
                {
                    scale = frame * (toFIXED(1) / 64) + toFIXED(1) / 8;
                }
                syMatrixScaleUniform(scale);
                if (frame > 3)
                {
                    Sint32 shade = frame * 4 - 80;

                    if (frame > 73)
                    {
                        shade = (74 - frame) * 4;
                    }
                    if (shade > 0)
                    {
                        shade = 0;
                    }
                    model->kind     = MODEL_SHADED_MAX;
                    model->texture  = &texture;
                    model->mesh     = mesh;
                    model->vertices = projected;
                    model->scrollU  = (frame / 4) & 63;
                    model->scrollV  = frame & 63;
                    model->shade    = shade;
                    model->flags    = MODEL_ONE_SIDED | MODEL_CLOCKWISE;
                    btRenderProjectVertices(cells, projected, DJINNI_CHARGE_FIREBALL_VERTICES);
                    btRenderDrawModelList(model);
                }
                ProjectionWork.centerX = BATTLE_PROJECTION_CENTER_X;
                syHeapRelease(model);
            }
            btCameraShake(8, 8);
            EFFECT_END_TICK(state);
        }
        fdActorSetAnimationSpeedStub(caster, ANIM_STEP_NORMAL);
        syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
        syCloseEvent(btEffectTransferCanvas);
    }
    btEffectEndCanvasLayer();
}

// Rime's Djinni rises over the targets while its seal spins and shrinks onto the target
// (btEffectRunDjinniSign).
//
// Abilities: Rime.
void btEffectRunRime(BATTLE_EFFECT_PARM *args)
{
    btEffectRunDjinniSign(args, DJINNI_SIGN_RIME);
}

// Haze's Djinni rises over the targets while sixteen stars bloom and fade (btEffectRunDjinniSign).
//
// Abilities: Haze.
void btEffectRunHaze(BATTLE_EFFECT_PARM *args)
{
    btEffectRunDjinniSign(args, DJINNI_SIGN_HAZE);
}

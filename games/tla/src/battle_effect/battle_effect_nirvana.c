// Nirvana, Stone Justice, Soul Shatter and Serac, and the entries of Heavy Divide and Strong Hit.
// Part of battle_effect.c.

#define NIRVANA_TICKS          80
// The orb is drawn as two rings of NIRVANA_SPARKS / 2 sparks, a quarter turn apart, turning about
// it; the rings open to NIRVANA_RING_RADIUS pixels and close again over the effect.
#define NIRVANA_SPARKS         24
#define NIRVANA_RING_RADIUS    64
// What the orb's speed gains toward the user each tick: 1/1024 of the distance between them.
#define NIRVANA_PULL(distance) ((distance) >> 10)

// The Meditation Rod's unleash: an orb leaves the first target, drawn as rings of sparks turning
// about it, and is pulled back into the user, who flashes white.
//
// Abilities: Nirvana.
void btEffectRunNirvana(BATTLE_EFFECT_PARM *args)
{
    BATTLE_EFFECT_STATE *state;
    void                *canvas;
    DRAW_RECTANGLE       blit;
    BATTLE_CAMERA       *view;
    Uint8               *sheet;
    MOTION_OBJECT       *object;
    PARTICLE            *orb;
    Sint32               half;
    Sint32               frame;
    Sint32               i;

    state  = RamHeapSlots->battleEffect;
    canvas = RamHeapSlots->battleCanvas;
    view   = RamHeapSlots->cameraState;
    sheet  = RamHeapSlots->battleSheet;
    btEffectStartCanvasLayer(BGCNT_PRIORITY(1));
    btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sheet, 0, 0);
    LOAD_BG_PALETTE(RESOURCE_PINK_BURST_SHEET);
    object = btObjectGetStub(args->targets[0])->object;
    half   = btUnitGetEffectHeightStub(args->targets[0]) / 2;
    orb    = state->particles;
    SET_POSITION(orb, object->x, object->y + half, object->z);
    orb->velocityX = 0;
    orb->velocityY = 0;
    orb->velocityZ = 0;
    orb->variant   = 0;
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
    blit = RamHeapSlots->ramCode;
    EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
    sySoundPlayStub(SE_SWIRL);

    for (frame = 0; frame != NIRVANA_TICKS; frame++)
    {
        MOTION_OBJECT *user     = btObjectGetStub(args->user)->object;
        Sint32         userHalf = btUnitGetEffectHeightStub(args->user) / 2;

        if (frame == 64)
            EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
        syInitMatrix();
        syLookAtLoad(view->eye, view->focus);
        if (frame == 72)
            btEffectTintBattler(args->user, SPRITE_COLOR_WHITE, TINT_KEEP, TINT_KEEP, 0);
        if (frame == 79)
            btEffectTintBattler(args->user, SPRITE_COLOR_NORMAL, TINT_KEEP, TINT_KEEP, 0);
        if (frame == 0)
            btEffectTintBattler(args->targets[0], SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 42);
        if (orb->variant == 0)
        {
            Sint32 size = 6;

            for (i = 0; i != NIRVANA_SPARKS; i++)
            {
                Sint32       point[3];
                SCREEN_POINT out;

                syPushMatrix();
                syTranslate(orb->x, orb->y, orb->z);
                if (i < NIRVANA_SPARKS / 2)
                    syRotX(frame * (TRIG_TURN / 128));
                else
                    syRotX(frame * (TRIG_TURN / 128) + DEGtoANG(90));
                syRotZ(i * (TRIG_TURN / 12) + frame * (TRIG_TURN / 64));
                syTranslate(0, sySin(frame * (TRIG_TURN / 160)) * NIRVANA_RING_RADIUS, 0);
                point[0] = 0;
                point[1] = 0;
                point[2] = 0;
                btEffectPosProjectPoint(point, &out);
                out.x >>= 1;
                syPopMatrix();
                blit(canvas,
                     sheet + BattleEffectMotePictures[size - 1],
                     out.x - size / 2,
                     out.y - size,
                     size,
                     size * 2);
            }
            btParticleAdvanceWithGravity3D(orb, 62, 0);
            if (frame > 0)
            {
                Sint32 dx = NIRVANA_PULL(user->x - orb->x);
                Sint32 dy = NIRVANA_PULL(user->y + userHalf - orb->y);
                Sint32 dz = NIRVANA_PULL(user->z - orb->z);
                Sint32 ox;
                Sint32 ux;

                orb->velocityX += dx;
                orb->velocityY += dy;
                orb->velocityZ += dz;
                ux              = user->x;
                if (ux < 0)
                    ux = -ux;
                ox = orb->x;
                if (ox < 0)
                    ox = -ox;
                // Gone once it has passed the user.
                if (ux < ox)
                    orb->variant = -1;
            }
        }
        EFFECT_END_TICK(state);
    }
    EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

#define STONE_JUSTICE_ROCKS     21
#define STONE_JUSTICE_TICKS     107
// The tick the rocks are hurled at the target and the battle background starts to slide after them.
#define STONE_JUSTICE_HURL_TICK 64
// A rock shows the next picture of its tumble each time its spin passes this much.
#define STONE_JUSTICE_SPIN_STEP 96

// Themis' Axe's unleash: rocks rise in a column beside the user and spin as they fly; then they are
// hurled across the screen into the target while the battle background slides after them, and every
// rock that reaches the target shakes the screen and knocks it back.
//
// Abilities: Stone Justice.
void btEffectRunStoneJustice(BATTLE_EFFECT_PARM *args)
{
    BATTLE_EFFECT_STATE *state;
    void                *canvas;
    DRAW_RECTANGLE       blit;
    FIXED                scroll;
    Sint32               scrollSpeed;
    Sint32               soundTimer;
    MOTION_OBJECT       *user;
    Sint32               scrollX;
    SCREEN_POINT         userPos;
    SCREEN_POINT         targetPos;
    PARTICLE            *rock;
    Sint32               frame;
    Sint32               hit;
    Sint32               i;

    state  = RamHeapSlots->battleEffect;
    canvas = RamHeapSlots->battleCanvas;
    EFFECT_START_NARROW_CANVAS(BGCNT_PRIORITY(1));
    REG_BLDCNT = 0;
    btResourceLoadAndDecompress(RESOURCE_BOULDER_SHEET, state->sheet, 1, 1);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_TRANSPARENT);
    blit = RamHeapSlots->ramCode;
    EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
    scroll      = 0;
    scrollSpeed = 0;
    soundTimer  = 0;
    user        = btObjectGetStub(args->user)->object;
    fdActorSetAnimationSpeedStub(user, ANIM_STEP_NORMAL * 3);

    for (frame = 0; frame != STONE_JUSTICE_TICKS; frame++)
    {
        btEffectPosProjectScaledBattler(args->user, &userPos);
        btEffectPosProjectScaledBattler(args->targets[0], &targetPos);
        if (frame == 0)
            sySoundPlayStub(SE_GATE_MOVE);
        if (frame == STONE_JUSTICE_HURL_TICK)
            sySoundPlayStub(SE_LAUNCH);

        // Tick 0: the rocks stand in a column 2 pixels apart from canvas row 96 down, on the user's
        // side of the canvas, and are thrown upward at random speeds.
        if (frame == 0)
        {
            for (i = 0; i != STONE_JUSTICE_ROCKS; i++)
            {
                rock = &state->particles[i];
                if (args->side == EFFECT_SIDE_ENEMIES)
                    rock->x = INTtoFIXED(syRandom() % 40 + 8);
                else
                    rock->x = toFIXED(120 - syRandom() % 40);
                syRandom(); // a number drawn and not used
                rock->y         = INTtoFIXED(i * 2 + 96);
                rock->velocityX = 0;
                rock->velocityY = -((syRandom() & 127) + 32) << 11;
                rock->z         = (syRandom() & 15) + 8;
                rock->velocityZ = 0;
                rock->variant   = 0;
            }
        }
        if (frame == 52)
            fdActorSelectAnimationStub(user, POSE_LEAP);

        // The background follows the rocks, ever faster.
        if (frame > STONE_JUSTICE_HURL_TICK)
        {
            if (args->side == EFFECT_SIDE_ENEMIES)
                scroll += scrollSpeed;
            else
                scroll -= scrollSpeed;
            scrollSpeed += toFIXED(1) / 2;
            scrollSpeed  = scrollSpeed * 60 / DAMPING_SCALE;
        }
        scrollX  = FIXEDtoINT(scroll);
        REG_BG2X = (64 - (userPos.x - scrollX)) << 8;

        // Each rock tumbles through three pictures of one of Spire's shards (z is its spin rate,
        // velocityZ the angle) and hits the target once it passes it.
        hit      = 0;
        for (i = 0; i != STONE_JUSTICE_ROCKS; i++)
        {
            Sint32 cell;
            Uint32 width;
            Sint32 left;
            Sint32 top;

            rock  = &state->particles[i];
            cell  = (i % 5) * 3 + rock->velocityZ / STONE_JUSTICE_SPIN_STEP % 3;
            width = SpireShardWidths[cell];
            left  = FIXED_INTEGER_PART(rock->x) - width / 2 + scrollX;
            top   = FIXED_INTEGER_PART(rock->y) - SpireShardHeights[cell] / 2;
            blit(canvas,
                 state->sheet + SpireShardPictures[cell] + BOULDER_SHARD_PICTURES,
                 left,
                 top,
                 width,
                 SpireShardHeights[cell]);
            if (rock->variant == 0)
            {
                if (args->side == EFFECT_SIDE_ENEMIES)
                {
                    if (targetPos.x > FIXED_INTEGER_PART(rock->x) + userPos.x - 64)
                    {
                        rock->variant = 1;
                        hit           = 1;
                    }
                }
                else
                {
                    if (targetPos.x < FIXED_INTEGER_PART(rock->x) + userPos.x - 64)
                    {
                        rock->variant = 1;
                        hit           = 1;
                    }
                }
            }
            btParticleAdvanceWithGravity2D(rock, 60, 0);
            rock->velocityZ += rock->z;
            if (frame > i / 2 + STONE_JUSTICE_HURL_TICK)
            {
                if (args->side == EFFECT_SIDE_ENEMIES)
                    rock->velocityX -= toFIXED(1) / 2;
                else
                    rock->velocityX += toFIXED(1) / 2;
                if (rock->z > 1)
                    rock->z--;
            }
        }

        // A hit flashes and pushes the target; the impact sound plays at most every eight ticks.
        if (soundTimer > 0)
            soundTimer--;
        if (hit == 1)
        {
            if (soundTimer == 0)
            {
                soundTimer = 8;
                sySoundPlayStub(SE_HEAVY_IMPACT);
            }
            btEffectStartActorMotion(args->targets[0], 1, toFIXED(2), toFIXED(4), toFIXED(2), 150);
            btEffectTintBattler(args->targets[0], SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 4);
            state->shakeFrame = 2;
        }

        btCameraShake(2, 4);
        EFFECT_END_TICK(state);
    }

    fdActorSetAnimationSpeedStub(user, ANIM_STEP_NORMAL);
    EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

#define SOUL_SHATTER_TICKS          106
// The column, LimeColumnSheet's half of it at the start of the effect's pictures, drawn twice side
// by side, the right half mirrored; BlastSheet's spark pictures follow it.
#define SOUL_SHATTER_COLUMN_WIDTH   14
#define SOUL_SHATTER_COLUMN_HEIGHT  80
#define SOUL_SHATTER_SPARK_PICTURES PICTURE_BYTES(SOUL_SHATTER_COLUMN)
// The sparks, each drawn for SOUL_SHATTER_SPARK_TICKS, and the motes of the target's soul, after
// twice as many particles.
#define SOUL_SHATTER_SPARKS         16
#define SOUL_SHATTER_SPARK_TICKS    29
#define SOUL_SHATTER_MOTES          32

// The Soul Brand's unleash: a column bursts up under the target in a spray of sparks and throws it
// into the air; the user glows, and from tick 32 motes of the target's soul fly out and are drawn
// back into the user, who flashes white. The column rises from tick 8, stands at its full height
// and sinks from tick 80, as the target comes down.
//
// Abilities: Soul Shatter.
void btEffectRunSoulShatter(BATTLE_EFFECT_PARM *args)
{
    BATTLE_EFFECT_STATE *state;
    void                *canvas;
    Uint8               *sheet;
    MOTION_OBJECT       *target;
    DRAW_RECTANGLE       draw[2];
    SCREEN_POINT         pos;
    SCREEN_POINT         screen;
    Sint32               frame;
    Sint32               i;

    state  = RamHeapSlots->battleEffect;
    canvas = RamHeapSlots->battleCanvas;
    sheet  = RamHeapSlots->battleSheet;
    btEffectStartCanvasLayer(BGCNT_PRIORITY(1));
    REG_BLDALPHA = BLEND_ALPHA(16, 12);
    btResourceLoadAndDecompress(RESOURCE_LIME_COLUMN_SHEET, state->sheet, 1, 1);
    btResourceLoadAndDecompress(
        RESOURCE_BLAST_SHEET, state->sheet + SOUL_SHATTER_SPARK_PICTURES, 1, 0);
    btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sheet, 0, 0);
    LOAD_BG_PALETTE(RESOURCE_ORANGE_PALETTE_B);
    EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
    target = btObjectGetStub(args->targets[0])->object;
    btEffectPosProjectBattler(args->targets[0], &pos);
    pos.x             /= 2;
    state->shakeFrame  = 8;

    for (frame = 0; frame != SOUL_SHATTER_TICKS; frame++)
    {
        // Tick 0: sparks leave the column's foot upward, and the motes start at the target, 52
        // pixels up, in every direction.
        if (frame == 0)
        {
            target->y = 0;
            {
                PARTICLE *spark;

                for (i = 0, spark = state->particles; i != SOUL_SHATTER_SPARKS; i++, spark++)
                {
                    Sint32 speed     = (syRandom() & 0xff) + 256;
                    Sint32 angle     = (syRandom() & 0x7fff) + DEGtoANG(90);

                    spark->x         = toFIXED(pos.x);
                    spark->y         = toFIXED(96 - (syRandom() & 31));
                    spark->velocityX = (sySin(angle) * speed) >> 7;
                    spark->velocityY = (syCos(angle) * speed) >> 9;
                    spark->variant   = syRandom() & 7;
                }
            }
            {
                PARTICLE *mote;

                for (i = 0, mote = &state->particles[SOUL_SHATTER_SPARKS * 2];
                     i != SOUL_SHATTER_MOTES;
                     i++, mote++)
                {
                    Sint32 speed = (syRandom() & 127) + 128;
                    Sint32 angle = syRandom() & 0x7fff;

                    SET_POSITION(mote, target->x, toFIXED(52), target->z);
                    mote->velocityY = ((syRandom() & 0xff) - 128) << 9;
                    mote->velocityX = (sySin(angle) * speed) >> 7;
                    mote->velocityZ = (syCos(angle) * speed) >> 7;
                    mote->variant   = 0;
                }
            }
        }

        // The sounds: the burst, the target thrown up 32 pixels, the user raising a hand, the drain
        // and the target's fall.
        if (frame == 4)
            sySoundPlayStub(SE_POWER_UP);
        if (frame == 10)
        {
            state->shakeFrame = 8;
            sySoundPlayStub(SE_CLATTER);
            btEffectTintBattler(args->targets[0], SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 80);
            target->verticalMotionStrength = 0;
            target->y                      = toFIXED(32);
        }
        if (frame == 80)
            target->verticalMotionStrength = BATTLE_UNIT_GRAVITY;
        if (frame == 24)
            btEffectTintBattler(args->user, TINT_KEEP, POSE_CAST, TINT_KEEP, 0);
        if (frame == 40)
            sySoundPlayStub(SE_SWIRL);
        if (frame == 32)
            EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
        if (frame == 64)
            btEffectTintBattler(args->user, SPRITE_COLOR_WHITE, TINT_KEEP, TINT_KEEP, 0);
        if (frame == 88)
            btEffectTintBattler(args->user, SPRITE_COLOR_NORMAL, TINT_KEEP, TINT_KEEP, 0);

        // The column: its two halves, the right one mirrored.
        if (frame > 7)
        {
            Sint32 centre = pos.x;
            Sint32 height;

            if (frame <= 79)
                height = frame * 12 - 96;
            else
                height = 560 - frame * 6;
            if (height > 0)
            {
                Uint32 mode;
                Sint32 x;

                if (height > SOUL_SHATTER_COLUMN_HEIGHT)
                {
                    height = SOUL_SHATTER_COLUMN_HEIGHT;
                    mode   = BLITTER_MAX;
                }
                else
                {
                    mode = BLITTER_ADDITIVE;
                }
                for (i = 0, x  = centre - SOUL_SHATTER_COLUMN_WIDTH; i != 2;
                     i++, x   += SOUL_SHATTER_COLUMN_WIDTH)
                {
                    if (i == 0)
                        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, mode);
                    else
                        EFFECT_LOAD_BLITTER(
                            HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, mode);
                    draw[0] = RamHeapSlots->ramCode;
                    draw[0](
                        canvas, state->sheet, x, 104 - height, SOUL_SHATTER_COLUMN_WIDTH, height);
                    syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
                }
            }
        }

        // The sparks: from tick 8, two more each tick, through seven pictures three ticks apiece,
        // the last held; one in four is drawn mirrored.
        btEffectBuildCanvasBlitters(args->side, draw);
        {
            PARTICLE *spark;

            for (i = 0, spark = state->particles; i != SOUL_SHATTER_SPARKS; i++, spark++)
            {
                if (frame >= i / 2 + 8 && spark->variant < SOUL_SHATTER_SPARK_TICKS)
                {
                    Sint32 cell = spark->variant / 3;
                    Sint32 x    = FIXED_INTEGER_PART(spark->x);
                    Sint32 y    = FIXED_INTEGER_PART(spark->y);

                    if (cell > 6)
                        cell = 6;
                    draw[(i & 3) ? 0 : 1](canvas,
                                          state->sheet + SoulShatterSparkPictures[cell]
                                              + SOUL_SHATTER_SPARK_PICTURES,
                                          x - SoulShatterSparkSizes[cell] / 2,
                                          y - SoulShatterSparkSizes[cell] / 2,
                                          SoulShatterSparkSizes[cell],
                                          SoulShatterSparkSizes[cell]);
                    spark->variant++;
                    btParticleAdvanceWithGravity2D(spark, 62, -toFIXED(1) / 8);
                }
            }
        }

        // From tick 32 the motes fly; mote i turns toward the user's middle from tick 32 + i and
        // vanishes once it passes the user.
        if (frame > 31)
        {
            MOTION_OBJECT *user;
            Sint32         userHalfHeight;
            BATTLE_CAMERA *camera;

            user           = btObjectGetStub(args->user)->object;
            userHalfHeight = btUnitGetEffectHeightStub(args->user) / 2;
            camera         = RamHeapSlots->cameraState;
            syInitMatrix();
            syLookAtLoad(camera->eye, camera->focus);
            for (i = 0; i != SOUL_SHATTER_MOTES; i++)
            {
                PARTICLE *mote = &state->particles[SOUL_SHATTER_SPARKS * 2 + i];
                if (mote->variant >= 0)
                {
                    Sint32 size = (i & 1) + 9;
                    Sint32 userX;
                    Sint32 sameSide;
                    Sint32 moteX;

                    btEffectPosProjectPoint(&mote->x, &screen);
                    draw[0](canvas,
                            sheet + BattleEffectMotePictures[size - 1],
                            (screen.x >> 1) - size / 2,
                            screen.y - size,
                            size,
                            size * 2);
                    btParticleAdvanceWithGravity3D(mote, 62, 0);
                    if (frame > i + 32)
                    {
                        Sint32 dx        = (user->x - mote->x) >> 7;
                        Sint32 dy        = (user->y + userHalfHeight - mote->y) >> 7;
                        Sint32 dz        = (user->z - mote->z) >> 7;

                        mote->velocityX += dx;
                        mote->velocityY += dy;
                        mote->velocityZ += dz;
                        moteX            = mote->x;
                        if (moteX < 0)
                            moteX = -moteX;
                        // FAKEMATCH(dead): the read of user->x in a once-only
                        // * block; as the plain read.
                        do
                        {
                            userX = user->x;
                        } while (0);
                        if (userX < 0)
                            userX = -userX;
                        // Gone once it has passed the user on the user's side of x = 0.
                        // FAKEMATCH(dead): each arm stores its test in
                        // * sameSide, never read; as the bare tests.
                        if (moteX >= userX
                            && (user->x < 0 ? (sameSide = mote->x < 0) : (sameSide = mote->x >= 0)))
                            mote->variant = -1;
                    }
                }
            }
        }

        EFFECT_RELEASE_BLITTERS();
        if (frame <= 7)
            btCameraShake(4, 4);
        else
            btCameraShake(16, 16);
        EFFECT_END_TICK(state);
    }

    syCloseEvent(btEffectTransferCanvas);
    btEffectEndCanvasLayer();
}

// A point projected onto the screen: x, y and depth.
typedef struct PROJECTED_POINT
{
    Sint32 x, y, depth;
} PROJECTED_POINT;

// The spire's corners and its mesh, in the map cell buffer.
#define SERAC_SPIRE_CORNERS          ((MODEL_VERTEX *)RamMapMetatileBuffer)
#define SERAC_SPIRE_MESH             (RamMapMetatileBuffer + 8192)

// Where MercuryDjinnSheet is decoded in the effect's pictures.
#define SERAC_MERCURY_DJINN_PICTURES 16384

#define SERAC_TICKS                  88

// Where BlueCloudPuffSheet is decoded in the effect's pictures.
#define SERAC_CLOUD_PUFFS            1024
// Where FlameBandTexture, the mist inside the spire, is decoded in the map cell buffer.
#define SERAC_MIST                   16384

// The spire of ice: a cone of SERAC_SPIRE_RINGS rings of SERAC_SPIRE_SIDES corners, each quad of
// its mesh 16 texels across and 8 high. It grows from tick 26 by SERAC_SPIRE_GROWTH a tick to its
// scale SERAC_SPIRE_SCALE.
#define SERAC_SPIRE_RINGS            8
#define SERAC_SPIRE_SIDES            8
#define SERAC_SPIRE_QUAD_TEXELS      16
#define SERAC_SPIRE_GROWTH           toFIXED(0.1526)
#define SERAC_SPIRE_SCALE            toFIXED(0.91553)

// The camera circles the spire SERAC_CAMERA_TURN a tick, slowing from tick 64, and turns no further
// than SERAC_CAMERA_YAW_MAX either way.
#define SERAC_CAMERA_TURN            320
#define SERAC_CAMERA_YAW_MAX         14000

// The puffs of dust where the Djinni lands, each drawn for SERAC_DUST_PUFF_TICKS through
// BlueCloudPuffSheet's four pictures.
#define SERAC_DUST_PUFFS             32
#define SERAC_DUST_PUFF_TICKS        24

// The four rings of mist spread from the spire's foot from tick SERAC_RINGS_TICK; a ring of
// BattleEffectCylinderMesh starts at SERAC_RING_START_SCALE and grows by its growth times
// SERAC_RING_GROWTH a tick.
#define SERAC_RINGS_TICK             22
#define SERAC_RING_START_SCALE       toFIXED(0.25586)
#define SERAC_RING_GROWTH            toFIXED(0.0229)

// A shard's y speed is three times a random fraction of a pixel less SERAC_SHARD_SPEED_BIAS: from
// about -0.58 to 2.42 pixels a tick, 0.92 on average.
#define SERAC_SHARD_SPEED_BIAS       12768

// "Strike a chilling finishing blow." The Mercury Djinni drops onto the target, dust bursting where
// it lands; a spire of ice (its mesh built on the first tick) grows up around the target in rings
// of mist while the camera circles, the caster closes in at tick 56, and at 64 the spire shatters
// into 64 tumbling shards.
//
// Abilities: Serac.
void btEffectRunSerac(BATTLE_EFFECT_PARM *args)
{
    void                *canvas;
    BATTLE_EFFECT_STATE *state;
    Sint32               frame;
    DRAW_RECTANGLE       blitters[2];
    Sint32               screenB[3];
    Sint32               screenA[3];
    Uint8               *mistSheet;

    canvas = RamHeapSlots->battleCanvas;
    state  = RamHeapSlots->battleEffect;
    btEffectConfigureEffectDisplay();
    if (args->side == EFFECT_SIDE_ENEMIES)
    {
        btEffectStartTiledCanvasFilled(BGCNT_WRAP);
    }
    else
    {
        btEffectStartTiledCanvas(BGCNT_WRAP);
    }
    btEffectRunDjinniEntrance(
        args,
        DJINNI_ENTRANCE(ELEMENT_MERCURY, args->side, DJINNI_ANCHOR_OVER_TARGET_SHORT),
        screenA,
        screenB);
    syWordsClear((void *)BG_CHAR_BLOCK(1), CANVAS_VIEW_BYTES);
    syWordsClearEntry(canvas, CANVAS_VIEW_BYTES);
    REG_BLDALPHA = EFFECT_BLEND_ADD;
    REG_BG2PA    = Q_8_8(1);
    btEffectBuildCanvasBlitters(0, blitters);
    EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
    mistSheet = RamMapMetatileBuffer + SERAC_MIST;
    btResourceLoadAndDecompress(RESOURCE_FLAME_BAND_TEXTURE, mistSheet, 0, 0);
    btResourceLoadAndDecompress(RESOURCE_DUST_BLOB_SHEET, state->sheet, 1, 1);
    btResourceLoadAndDecompress(
        RESOURCE_BLUE_CLOUD_PUFF_SHEET, state->sheet + SERAC_CLOUD_PUFFS, 0, 0);
    btResourceLoadAndDecompress(
        RESOURCE_MERCURY_DJINN_SHEET, state->sheet + SERAC_MERCURY_DJINN_PICTURES, 1, 0);
    LOAD_BG_PALETTE(RESOURCE_BLUE_FLAME_SHEET);
    {
        BATTLE_CAMERA  *camera;
        Sint16          savedYaw;
        MOTION_OBJECT  *target;
        PROJECTED_POINT screen;
        Sint32          world[3];
        PROJECTED_POINT point;
        PROJECTED_POINT dropScreen;
        MODEL_TEXTURE   texture;
        RENDER_MODEL   *model;
        void           *vertices;
        Uint8           color[1];
        Sint32          savedCenterX;
        Sint32          savedCenterY;
        Sint32          row;
        Sint32          column;
        Sint32          i;

        camera    = (BATTLE_CAMERA *)RamHeapSlots->cameraState;
        savedYaw  = camera->yaw;
        target    = btObjectGetStub(args->targets[0])->object;
        REG_BG2PA = Q_8_8(1);
        REG_BG2X  = 0;

        for (frame = 0; frame != SERAC_TICKS; frame++)
        {
            if (frame == 0)
            {
                camera->yaw = savedYaw;
                for (row = 0; row != SERAC_SPIRE_RINGS; row++)
                {
                    for (column = 0; column != SERAC_SPIRE_SIDES; column++)
                    {
                        Sint32 height;
                        Sint32 radius;
                        Sint32 angle;

                        if (row == 0)
                        {
                            height = SeracRowHeights[row];
                            radius = SeracRowRadii[row];
                        }
                        else
                        {
                            height = SeracRowHeights[row] + (syRandom() & 3);
                            radius = SeracRowRadii[row] + (syRandom() & 3);
                        }
                        angle = column * (TRIG_TURN / SERAC_SPIRE_SIDES) + TRIG_TURN / 16;
                        SERAC_SPIRE_CORNERS[row * SERAC_SPIRE_SIDES + column].x =
                            FIXEDtoINT(sySin(angle) * radius);
                        SERAC_SPIRE_CORNERS[row * SERAC_SPIRE_SIDES + column].y = -height;
                        SERAC_SPIRE_CORNERS[row * SERAC_SPIRE_SIDES + column].z =
                            FIXEDtoINT(syCos(angle) * radius);
                        // Each quad between this ring and the next is two triangles of the mesh.
                        if (row < SERAC_SPIRE_RINGS - 1)
                        {
                            MESH_TRIANGLE *firstTriangle = (MESH_TRIANGLE *)SERAC_SPIRE_MESH
                                                         + (row * SERAC_SPIRE_SIDES + column) * 2;
                            MESH_TRIANGLE *secondTriangle;
                            Sint32         bottom = row * 8 + 7;
                            Sint32         left   = column * SERAC_SPIRE_QUAD_TEXELS;
                            Sint32         right  = left + SERAC_SPIRE_QUAD_TEXELS - 1;
                            Sint32         next;
                            Sint32         here;

                            firstTriangle->uv[0][0] = right;
                            firstTriangle->uv[0][1] = row * 8;
                            firstTriangle->uv[1][0] = right;
                            firstTriangle->uv[2][1] = bottom;
                            firstTriangle->uv[1][1] = bottom;
                            firstTriangle->uv[2][0] = left;
                            secondTriangle          = (MESH_TRIANGLE *)SERAC_SPIRE_MESH
                                           + (row * SERAC_SPIRE_SIDES + column) * 2 + 1;
                            secondTriangle->uv[0][0] = left;
                            secondTriangle->uv[2][0] = left;
                            secondTriangle->uv[0][1] = row * 8;
                            secondTriangle->uv[2][1] = bottom;
                            secondTriangle->uv[1][0] = right;
                            secondTriangle->uv[1][1] = row * 8;
                            next = (column + 1) % SERAC_SPIRE_SIDES + row * SERAC_SPIRE_SIDES;
                            firstTriangle->vertex[0] = next;
                            here = column % SERAC_SPIRE_SIDES + row * SERAC_SPIRE_SIDES;
                            firstTriangle->vertex[1]  = here + SERAC_SPIRE_SIDES;
                            firstTriangle->vertex[2]  = next + SERAC_SPIRE_SIDES;
                            secondTriangle->vertex[0] = here;
                            secondTriangle->vertex[1] = here + SERAC_SPIRE_SIDES;
                            secondTriangle->vertex[2] = next;
                        }
                    }
                }
                SeracMeshEnd[0] = 0;
                SeracMeshEnd[1] = 0;
                SeracMeshEnd[2] = 0;
            }

            // The camera circles the spire.
            if ((Uint32)(frame - 4) <= 79)
            {
                Sint32 turn;

                if (frame <= 63)
                {
                    turn = SERAC_CAMERA_TURN;
                }
                else
                {
                    turn = 1600 - frame * 20;
                }
                if (args->side == EFFECT_SIDE_ENEMIES)
                {
                    camera->yaw -= turn;
                }
                else
                {
                    camera->yaw += turn;
                }
                if (args->side == EFFECT_SIDE_ENEMIES)
                {
                    if (camera->yaw < -SERAC_CAMERA_YAW_MAX)
                    {
                        camera->yaw = -SERAC_CAMERA_YAW_MAX;
                    }
                }
                else if (camera->yaw > SERAC_CAMERA_YAW_MAX)
                {
                    camera->yaw = SERAC_CAMERA_YAW_MAX;
                }
            }

            vertices               = syHeapAllocateEWRAM(1024);
            model                  = btRenderAllocateModelList(1);
            savedCenterX           = ProjectionWork.centerX;
            savedCenterY           = ProjectionWork.centerY;
            ProjectionWork.centerY = savedCenterY - 16;
            syInitMatrix();
            syLookAtLoad(camera->eye, camera->focus);
            world[0] = target->x;
            world[1] = 0;
            world[2] = target->z;
            syConvert3Dto2D(world, &screen);
            if (screen.x <= 63)
            {
                screen.x = 64;
            }
            if (screen.x > 176)
            {
                screen.x = 176;
            }
            ProjectionWork.centerX = 184 - screen.x;
            IO_WRITE_QUEUE_PUSH((Uint32)&REG_BG2X, (64 - screen.x) << 8, IO_WRITE_WORD);

            // Dust where the Djinni lands.
            if ((Uint32)frame <= 63)
            {
                if (frame == 0)
                {
                    for (i = 0; i != SERAC_DUST_PUFFS; i++)
                    {
                        PARTICLE *p     = &state->particles[i];
                        Sint32    speed = (syRandom() & 1023) + 1024;
                        Sint32    angle = syRandom() & TRIG_ANGLE_MASK;

                        SET_POSITION(p, target->x, 0, target->z);
                        p->velocityX = (sySin(angle) * speed) >> 9;
                        p->velocityY = 0;
                        p->velocityZ = (syCos(angle) * speed) >> 9;
                        p->variant   = -i;
                    }
                }
                for (i = 0; i != SERAC_DUST_PUFFS; i++)
                {
                    PARTICLE *p = &state->particles[i];
                    Sint32    picture;
                    Sint32    x;
                    Sint32    y;

                    if ((Uint32)++p->variant >= SERAC_DUST_PUFF_TICKS)
                    {
                        continue;
                    }
                    picture = p->variant / 6;
                    syConvert3Dto2D((const FIXED *)p, &point);
                    btParticleAdvanceWithGravity3D(p, 60, toFIXED(1) / 8);
                    x = point.x - CLOUD_PUFF_WIDTH / 2;
                    y = point.y - CLOUD_PUFF_HEIGHT / 2;
                    blitters[0](canvas,
                                state->sheet + SERAC_CLOUD_PUFFS
                                    + picture * PICTURE_BYTES(CLOUD_PUFF),
                                x,
                                y,
                                CLOUD_PUFF_WIDTH,
                                CLOUD_PUFF_HEIGHT);
                }
            }
            // The Djinni falls.
            if (frame <= 29)
            {
                point.x     = target->x;
                point.y     = toFIXED(60) - frame * toFIXED(2);
                point.depth = target->z + (sySin(frame * (TRIG_TURN / 128)) << 5);
                syConvert3Dto2D((const FIXED *)&point, &dropScreen);
                blitters[1](canvas,
                            state->sheet + SERAC_MERCURY_DJINN_PICTURES,
                            dropScreen.x - 20,
                            dropScreen.y - 30,
                            DJINNI_WIDTH,
                            DJINNI_HEIGHT);
            }
            if (frame == 4)
            {
                sySoundPlayStub(SE_SWITCH_MECHANISM);
            }
            if (frame == 30)
            {
                sySoundPlayStub(SE_DJINNI_SET);
            }
            if (frame == 60)
            {
                sySoundPlayStub(SE_POWER_UP);
            }
            if (frame == 64)
            {
                sySoundPlayStub(SE_HEAVY_IMPACT);
                EFFECT_SHOW_DAMAGE(HIT_SOUND_NONE);
            }
            if (frame == 30)
            {
                btEffectTintBattler(
                    args->targets[0], SPRITE_COLOR_BLUE_TINT, POSE_DAMAGE, TINT_KEEP, 0);
            }
            if (frame == 26)
            {
                state->shakeFrame = 8;
            }
            if (frame == 64)
            {
                state->shakeFrame = 4;
                btEffectTintBattler(args->targets[0], SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 4);
            }
            if (frame == 70)
            {
                btEffectTintBattler(args->targets[0], SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 4);
            }
            if (frame == 56)
            {
                // The caster closes four fifths of the way.
                ACTOR *caster = (ACTOR *)btObjectGetStub(args->user)->object;
                ACTOR *to     = (ACTOR *)btObjectGetStub(args->targets[0])->object;
                Sint32 x      = caster->x.fixed;
                FIXED  z;
                Sint32 dx = (to->x.fixed - x) * 80 / 100;
                Sint32 dz;
                Sint32 speed;

                z                       = caster->z.fixed;
                dz                      = (to->z.fixed - z) * 80 / 100;
                x                      += dx;
                z                      += dz;
                dx                    >>= 8;
                dz                    >>= 8;
                speed                   = (sySquartIWRAM(dx * dx + dz * dz) << 8) / 10;
                caster->acceleration    = speed;
                caster->speedLimit      = speed;
                caster->snapToTarget    = 1;
                caster->velocityY       = toFIXED(6);
                caster->gravity         = toFIXED(6) / 5;
                caster->bounce          = 0;
                caster->steeringFlags   = 1;
                fdActorStartIdleStub(caster);
                fdActorSetMoveTargetStub(caster, x, 0, z);
                fdActorSelectAnimationStub(caster, POSE_LEAP);
            }

            if (frame > 63)
            {
                if (frame == 64)
                {
                    syWordsFillEntry(
                        canvas, CANVAS_BYTES, CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
                    for (i = 0; i != ARRAY_COUNT(state->particles); i++)
                    {
                        PARTICLE *p     = &state->particles[i];
                        Sint32    angle = (i & 7) * (TRIG_TURN / 8);
                        Sint32    ring;

                        SET_POSITION(p,
                                     target->x + ((sySin(angle) * (ring = i / 8)) << 2),
                                     ((syRandom() & TRIG_ANGLE_MASK) << 2) - ring * toFIXED(10)
                                         + toFIXED(64),
                                     target->z + ((syCos(angle) * ring) << 2));
                        p->velocityX = ((syRandom() & TRIG_ANGLE_MASK) << 1) + toFIXED(1);
                        p->velocityY =
                            ((syRandom() & TRIG_ANGLE_MASK) - SERAC_SHARD_SPEED_BIAS) * 3;
                        p->velocityZ = ((syRandom() & TRIG_ANGLE_MASK) - toFIXED(0.5)) << 1;
                        if (p->x < 0)
                        {
                            p->velocityX = -p->velocityX;
                        }
                        p->variant = (syRandom() & 31) + 32;
                    }
                }
                else
                {
                    model->kind     = MODEL_SOLID_MAX;
                    model->flags    = 0;
                    model->mesh     = BattleEffectTriangleMesh;
                    model->vertices = vertices;
                    for (i = 0; i != ARRAY_COUNT(state->particles); i++)
                    {
                        PARTICLE *p     = &state->particles[i];
                        Sint32    angle = (frame + i) * (TRIG_TURN / 64);

                        if (p->variant < 0)
                        {
                            continue;
                        }
                        color[0]      = p->variant;
                        p->variant   -= 2;
                        model->shade  = color;
                        btParticleAdvanceWithGravity3D(p, 62, toFIXED(1) / 64);
                        syPushMatrix();
                        syTranslate(p->x, p->y, p->z);
                        syRotX(angle);
                        syRotZ(angle);
                        syMatrixScaleUniform(toFIXED(1));
                        btRenderProjectVertices(BattleEffectTriangleCorners, vertices, 3);
                        btRenderDrawModelList(model);
                        syPopMatrix();
                    }
                }
            }
            if (frame < 64)
            {
                if (frame > 25)
                {
                    Sint32      scale;
                    Sint32      start;
                    // The spire is drawn at its own shade, the mist inside it 32 levels dimmer.
                    Sint32      spireShade;
                    const void *mesh  = SERAC_SPIRE_MESH;

                    // The spire, then the mist inside it.
                    texture.logWidth  = 5;
                    texture.logHeight = 5;
                    texture.pixels    = (const Uint8 *)state->sheet;
                    model->kind       = MODEL_SHADED_MAX;
                    model->flags      = MODEL_ONE_SIDED | MODEL_CLOCKWISE;
                    model->vertices   = vertices;
                    model->scrollU    = 0;
                    model->texture    = &texture;
                    model->mesh       = mesh;
                    model->scrollV    = -((frame / 2) & 31);
                    syPushMatrix();
                    spireShade = 0;
                    if (frame <= 31)
                    {
                        scale = frame * SERAC_SPIRE_GROWTH - 26 * SERAC_SPIRE_GROWTH;
                    }
                    else
                    {
                        scale = SERAC_SPIRE_SCALE;
                    }
                    model->shade = (const void *)spireShade;
                    syTranslate(target->x, -toFIXED(4), target->z);
                    syScale(scale, scale, scale);
                    btRenderProjectVertices((const Uint8 *)MapGridMetatileBuffer,
                                            vertices,
                                            SERAC_SPIRE_RINGS * SERAC_SPIRE_SIDES);
                    btRenderDrawModelList(model);
                    spireShade     = -32;
                    model->shade   = (const void *)spireShade;
                    model->kind    = MODEL_SHADED_ADDITIVE;
                    model->scrollU = 0;
                    model->scrollV = (frame / 2) & 31;
                    model->flags   = MODEL_ONE_SIDED;
                    btRenderDrawModelList(model);
                    syPopMatrix();

                    // Four rings of mist spread from its foot.
                    texture.logWidth  = 7;
                    texture.logHeight = 7;
                    texture.pixels    = mistSheet;
                    model->kind       = MODEL_SHADED_ADDITIVE;
                    model->texture    = &texture;
                    model->mesh       = BattleEffectCylinderMesh;
                    model->flags      = 0;
                    model->vertices   = vertices;
                    model->scrollU    = 0;
                    model->scrollV    = 0;
                    for (i = 0; i != 4; i++)
                    {
                        Sint32 size;
                        Sint32 shade;

                        start = SeracRingStarts[i] + SERAC_RINGS_TICK;
                        if (frame <= start)
                        {
                            continue;
                        }
                        size = (frame - start) * SeracRingGrowth[i] * SERAC_RING_GROWTH
                             + SERAC_RING_START_SCALE;
                        shade = (start - frame) * 8 + 56;
                        if (shade > 0)
                        {
                            shade = 0;
                        }
                        if (shade <= MODEL_SHADE_INVISIBLE)
                        {
                            continue;
                        }
                        syPushMatrix();
                        model->shade = (const void *)shade;
                        syTranslate(target->x, toFIXED(SeracRingHeights[i]), target->z);
                        syScale(size, size, size);
                        model->scrollU = ((frame + i * 5) & 7) << 4;
                        btRenderProjectVertices(BattleEffectCylinderCorners, vertices, 32);
                        btRenderDrawModelList(model);
                        syPopMatrix();
                    }
                }
            }
            syHeapRelease(model);
            syHeapRelease(vertices);
            ProjectionWork.centerX = savedCenterX;
            ProjectionWork.centerY = savedCenterY;
            btCameraShake(8, 8);
            EFFECT_END_TICK(state);
        }
    }
    EFFECT_FINISH_CANVAS();
}

// A great sword swings down onto the target, trailing blue after-images.
//
// Abilities: Heavy Divide.
void btEffectRunHeavyDivide(BATTLE_EFFECT_PARM *args)
{
    btEffectRunHeavyBlow(args, HEAVY_BLOW_HEAVY_DIVIDE);
}

// An axe spins across from the user to the target.
//
// Abilities: Strong Hit.
void btEffectRunStrongHit(BATTLE_EFFECT_PARM *args)
{
    btEffectRunHeavyBlow(args, HEAVY_BLOW_STRONG_HIT);
}

#undef NIRVANA_TICKS
#undef NIRVANA_SPARKS
#undef NIRVANA_RING_RADIUS
#undef NIRVANA_PULL
#undef STONE_JUSTICE_ROCKS
#undef STONE_JUSTICE_TICKS
#undef STONE_JUSTICE_HURL_TICK
#undef STONE_JUSTICE_SPIN_STEP
#undef SOUL_SHATTER_TICKS
#undef SOUL_SHATTER_COLUMN_WIDTH
#undef SOUL_SHATTER_COLUMN_HEIGHT
#undef SOUL_SHATTER_SPARK_PICTURES
#undef SOUL_SHATTER_SPARKS
#undef SOUL_SHATTER_SPARK_TICKS
#undef SOUL_SHATTER_MOTES
#undef SERAC_SPIRE_CORNERS
#undef SERAC_SPIRE_MESH
#undef SERAC_MERCURY_DJINN_PICTURES
#undef SERAC_TICKS
#undef SERAC_CLOUD_PUFFS
#undef SERAC_MIST
#undef SERAC_SPIRE_RINGS
#undef SERAC_SPIRE_SIDES
#undef SERAC_SPIRE_QUAD_TEXELS
#undef SERAC_SPIRE_GROWTH
#undef SERAC_SPIRE_SCALE
#undef SERAC_CAMERA_TURN
#undef SERAC_CAMERA_YAW_MAX
#undef SERAC_DUST_PUFFS
#undef SERAC_DUST_PUFF_TICKS
#undef SERAC_RINGS_TICK
#undef SERAC_RING_START_SCALE
#undef SERAC_RING_GROWTH
#undef SERAC_SHARD_SPEED_BIAS

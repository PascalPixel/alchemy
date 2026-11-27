// Streak volleys (Sonic Wave, Shriek, War Cry ...) and mote fountains (Sleep Star, Soothing Star,
// Slaver). Part of battle_effect.c.

#include "battle/stubs.h"

// Column `n` of style `mode`'s row of StreakStyles, four flags a style.
#define STREAK_VALUE(mode, n) StreakStyles[(mode) * 4 + (n)]
// Nonzero: the streak cycles through three images.
#define STREAK_ANIMATE        0
// Nonzero: play the hit sound and flash the target.
#define STREAK_HIT            1
// The target's knockback on the hit (BATTLE_KNOCKBACK, -1: none).
#define STREAK_MOTION         2
// Nonzero: the struck target trembles from side to side.
#define STREAK_SHAKE          3

#define STREAK_MAX            3 // streaks fired at each target

// The streak volley's styles (btEffectRunStreakVolley): its colour and its row of StreakStyles.
typedef enum STREAK_STYLE
{
    STREAK_STYLE_EMBER, // Sonic Wave
    // Shriek and Harpy, and the unused btEffectRunLimeStreaks
    STREAK_STYLE_LIME,
    STREAK_STYLE_VIOLET, // Banshee Howl
    STREAK_STYLE_RED,    // Wicked Howl
    STREAK_STYLE_CYAN    // War Cry
} STREAK_STYLE;

// Three streaks of ember light fly from the user at each target in turn; each target flashes and
// recoils.
//
// Abilities: Sonic Wave.
void btEffectRunSonicWave(BATTLE_EFFECT_PARM *args)
{
    btEffectRunStreakVolley(args, STREAK_STYLE_EMBER);
}

// Three streaks of lime light fly from the user at each target in turn; each target flashes and
// recoils.
//
// Abilities: Shriek, Harpy.
void btEffectRunShriek(BATTLE_EFFECT_PARM *args)
{
    btEffectRunStreakVolley(args, STREAK_STYLE_LIME);
}

// Three streaks of violet light fly from the user at each target in turn; each target flashes and
// trembles.
//
// Abilities: Banshee Howl.
void btEffectRunBansheeHowl(BATTLE_EFFECT_PARM *args)
{
    btEffectRunStreakVolley(args, STREAK_STYLE_VIOLET);
}

// Three flickering cyan streaks fly from the user at each target in turn; each target flashes and
// recoils.
//
// Abilities: War Cry.
void btEffectRunWarCry(BATTLE_EFFECT_PARM *args)
{
    btEffectRunStreakVolley(args, STREAK_STYLE_CYAN);
}

// Three flickering red streaks fly from the user at each target in turn; each target flashes and
// trembles.
//
// Abilities: Wicked Howl.
void btEffectRunWickedHowl(BATTLE_EFFECT_PARM *args)
{
    btEffectRunStreakVolley(args, STREAK_STYLE_RED);
}

// Unused: no entry of the effect table points here. It would have played the same lime streaks as
// Shriek.
UNUSED void btEffectRunLimeStreaks(BATTLE_EFFECT_PARM *args)
{
    btEffectRunStreakVolley(args, STREAK_STYLE_LIME);
}

// FlameArcSheet's streaks: three animation frames, each six pictures that grow; each picture is the
// lower half of a ring, and the upper half is it drawn flipped, so a streak flies as a ring of
// light.
#define STREAK_WIDTH        20
#define STREAK_HEIGHT       40
#define STREAK_FRAME_BYTES  (6 * PICTURE_BYTES(STREAK))
#define STREAK_SPACING      6  // ticks between streaks
#define STREAK_FLIGHT       24 // ticks from user to target
#define STREAK_TARGET_DELAY 32 // ticks between targets

// Fires the streaks at each target in turn from above the user; each leaves a fading trail.
void btEffectRunStreakVolley(BATTLE_EFFECT_PARM *args, Sint32 mode)
{
    SCREEN_POINT         pos;
    HEAP_SLOTS          *heap;
    BATTLE_EFFECT_STATE *state;
    void                *canvas;
    Sint32               k;
    DRAW_RECTANGLE       drawLower;
    DRAW_RECTANGLE       drawUpper;
    BATTLE_CAMERA       *camera;
    MOTION_OBJECT       *src;
    MOTION_OBJECT       *target;
    Sint32               resource;
    Sint32               frame;
    Sint32               j;
    Sint32               streaks = STREAK_MAX;
    Sint32               lift    = toFIXED(20);

    // FAKEMATCH(dead): heap set through its own address; as heap =
    // * RamHeapSlots.
    *&heap                       = RamHeapSlots;
    state                        = (BATTLE_EFFECT_STATE *)heap->battleEffect;
    canvas                       = heap->battleCanvas;
    camera                       = heap->cameraState;
    btEffectStartCanvasLayer(BGCNT_PRIORITY(1));

    // The upper half draws mirrored top to bottom; the far side mirrors left to right as well.
    if (args->side == EFFECT_SIDE_ENEMIES)
    {
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y, BLITTER_MAX);
        drawUpper = heap->ramCode;
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
        drawLower = heap->ramCodePaired;
    }
    else
    {
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP, BLITTER_MAX);
        drawUpper = heap->ramCode;
        EFFECT_LOAD_BLITTER(
            HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
        drawLower = heap->ramCodePaired;
    }

    // The streak pictures, and the mode's colours.
    btResourceLoadAndDecompress(RESOURCE_FLAME_ARC_SHEET, state->sheet, 0, 0);
    switch (mode)
    {
        case STREAK_STYLE_EMBER : resource = RESOURCE_EMBER_STREAK_SHEET; break;
        case STREAK_STYLE_LIME  : resource = RESOURCE_LIME_PALETTE; break;
        case STREAK_STYLE_VIOLET: resource = RESOURCE_VIOLET_PALETTE_D; break;
        case STREAK_STYLE_RED   : resource = RESOURCE_RED_PALETTE_B; break;
        case STREAK_STYLE_CYAN  :
        default                 : resource = RESOURCE_CYAN_PALETTE; break;
    }
    syWordsCopyEntry((void *)BG_PLTT, syResourceGetTableEntry(resource), SHEET_PALETTE_BYTES);
    // Each transfer halves the canvas, leaving a trail.
    EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

    src = btObjectGetStub(args->user)->object;
    {
        // The loop counts STREAK_SPACING for each streak though targets are STREAK_TARGET_DELAY
        // apart, so with three or four targets it stops before the last one's second flash.
        Sint32 totalTicks = streaks * args->targetCount * STREAK_SPACING + 48;

        // Each streak flies from lift above the user to lift above its target.
        EFFECT_FOR_EACH_TARGET(k, args)
        {
            target = btObjectGetStub(args->targets[k])->object;
            for (j = 0; j != streaks; j++)
            {
                PARTICLE *streak = &state->particles[k * streaks + j];

                SET_POSITION(streak, src->x, src->y + lift, src->z);
                SET_VELOCITY(streak,
                             (target->x - streak->x) / STREAK_FLIGHT,
                             (target->y + lift - streak->y) / STREAK_FLIGHT,
                             (target->z - streak->z) / STREAK_FLIGHT);
                streak->variant = 0; // ticks since the streak set off
            }
        }

        for (frame = 0; frame != totalTicks; frame++)
        {
            Sint32 *row = state->bg2xLines;

            // A sine wave of BG2X offsets for the wave HBlank DMA, which nothing arms here, so the
            // canvas does not ripple. The last 16 ticks fade out.
            for (k = 0; k != DISPLAY_HEIGHT; k++)
                *row++ = CANVAS_WAVE_BG2X(toFIXED(8) - sySin((frame + k) * (TRIG_TURN / 16)) * 2);
            if (frame > totalTicks - 16)
                REG_BLDALPHA = EFFECT_BLEND_CANVAS(totalTicks - frame);

            EFFECT_VIEW_FROM_CAMERA(camera);
            EFFECT_FOR_EACH_TARGET(k, args)
            {
                // Each target's streaks set out with the style's cry.
                if (frame == k * STREAK_TARGET_DELAY)
                {
                    if (mode == STREAK_STYLE_LIME)
                        sySoundPlayStub(SE_SHRIEK);
                    if (mode == STREAK_STYLE_VIOLET)
                        sySoundPlayStub(SE_STONE_GRIND);
                    if (mode == STREAK_STYLE_RED)
                        sySoundPlayStub(SE_BOSS_ROAR);
                    if (mode == STREAK_STYLE_CYAN)
                        sySoundPlayStub(SE_ITEM_BREAK);
                }
                if (frame >= k * STREAK_TARGET_DELAY)
                {
                    for (j = 0; j != streaks; j++)
                    {
                        // From here lift holds the streak's start tick.
                        lift = k * STREAK_TARGET_DELAY + j * STREAK_SPACING;
                        if (frame >= lift)
                        {
                            PARTICLE *streak = &state->particles[k * streaks + j];
                            Sint32    step;

                            // The picture changes every 8 ticks, stopping at the sixth.
                            btEffectPosProjectPoint((Sint32 *)streak, &pos);
                            pos.x >>= 1;
                            step    = streak->variant / 8;
                            if (step > 5)
                                step = 5;
                            if (STREAK_VALUE(mode, STREAK_ANIMATE))
                            {
                                Uint8 *picture =
                                    &state->sheet[step * PICTURE_BYTES(STREAK)
                                                  + frame / 2 % 3 * STREAK_FRAME_BYTES];

                                drawUpper(canvas,
                                          picture,
                                          pos.x - STREAK_WIDTH / 2,
                                          pos.y - STREAK_HEIGHT,
                                          STREAK_WIDTH,
                                          STREAK_HEIGHT);
                                drawLower(canvas,
                                          picture,
                                          pos.x - STREAK_WIDTH / 2,
                                          pos.y,
                                          STREAK_WIDTH,
                                          STREAK_HEIGHT);
                            }
                            else
                            {
                                Uint8 *picture = &state->sheet[step * PICTURE_BYTES(STREAK)
                                                               + 2 * STREAK_FRAME_BYTES];

                                drawUpper(canvas,
                                          picture,
                                          pos.x - STREAK_WIDTH / 2,
                                          pos.y - STREAK_HEIGHT,
                                          STREAK_WIDTH,
                                          STREAK_HEIGHT);
                                drawLower(canvas,
                                          picture,
                                          pos.x - STREAK_WIDTH / 2,
                                          pos.y,
                                          STREAK_WIDTH,
                                          STREAK_HEIGHT);
                            }
                            btParticleAdvanceWithGravity3D(streak, PARTICLE_DAMPING_NONE, 0);
                            streak->variant++;
                        }
                    }

                    // From 6 ticks after the hit, tremble the target sideways for 32 ticks.
                    if (STREAK_VALUE(mode, STREAK_SHAKE) && frame >= k * STREAK_TARGET_DELAY + 30
                        && frame < k * STREAK_TARGET_DELAY + 62)
                    {
                        MOTION_OBJECT *struck = btObjectGetStub(args->targets[k])->object;

                        struck->x +=
                            toFIXED(StreakTremble[(frame - k * STREAK_TARGET_DELAY - 30) % 8]);
                        if (struck->x > 0)
                            struck->x += toFIXED(1) / 2;
                        else
                            struck->x -= toFIXED(1) / 2;
                        btEffectTintBattler(args->targets[k], TINT_KEEP, POSE_DAMAGE, TINT_KEEP, 0);
                    }

                    // The hit: a sound and two flashes, 16 ticks apart. The first hit lets the
                    // battle show its results.
                    if (STREAK_VALUE(mode, STREAK_HIT))
                    {
                        if (frame == k * STREAK_TARGET_DELAY + 24)
                        {
                            sySoundPlayStub(SE_IMPACT);
                            if (k == 0)
                                EFFECT_SHOW_DAMAGE(HIT_SOUND_NONE);
                            btEffectTintBattler(
                                args->targets[k], SPRITE_COLOR_WHITE, POSE_DAMAGE, k, 8);
                        }
                        if (frame == k * STREAK_TARGET_DELAY + 40)
                            btEffectTintBattler(
                                args->targets[k], SPRITE_COLOR_WHITE, POSE_DAMAGE, k, 8);
                    }
                    if (STREAK_VALUE(mode, STREAK_MOTION) != -1
                        && frame == k * STREAK_TARGET_DELAY + 24)
                    {
                        state->shakeFrame = 4;
                        btMotionApplyKnockbackStub(args->targets[k],
                                                   STREAK_VALUE(mode, STREAK_MOTION));
                    }
                }
            }
            btCameraShake(8, 8);
            EFFECT_END_TICK(state);
        }
    }

    EFFECT_FINISH_CANVAS();
}

// The mote fountain's modes (btEffectRunMoteFountain).
typedef enum MOTE_FOUNTAIN_MODE
{
    MOTE_FOUNTAIN_HEALING,  // Soothing Star
    MOTE_FOUNTAIN_DRIFTING, // Sleep Star
    MOTE_FOUNTAIN_SLAVER    // Slaver
} MOTE_FOUNTAIN_MODE;

// The motes the fountain sets up, and how many of them it moves and draws.
enum
{
    MOTE_FOUNTAIN_INITIALIZED_COUNT = 256,
    MOTE_VISIBLE_COUNT              = 128
};

// Motes burst out of the user and float up, wobbling, as they drift across the field onto the
// targets, and each target flinches in turn.
//
// Abilities: Sleep Star.
void btEffectRunSleepStar(BATTLE_EFFECT_PARM *args)
{
    btEffectRunMoteFountain(args, MOTE_FOUNTAIN_DRIFTING);
}

// Orange healing motes burst out of the user and float up close to it, and each target glows in
// turn with the recovery sound.
//
// Abilities: Soothing Star.
void btEffectRunSoothingStar(BATTLE_EFFECT_PARM *args)
{
    btEffectRunMoteFountain(args, MOTE_FOUNTAIN_HEALING);
}

// Green drops are flung up from the user in a half-circle fan and fall, bouncing off the ground,
// then Impair plays on the targets.
//
// Abilities: Slaver.
void btEffectRunSlaver(BATTLE_EFFECT_PARM *args)
{
    btEffectRunMoteFountain(args, MOTE_FOUNTAIN_SLAVER);
}

#define MOTE_FOUNTAIN_TICKS        128
#define MOTE_FOUNTAIN_SLAVER_TICKS 96

// The projected depths over which a mote is sized, nearest to farthest: it is drawn
// MOTE_FOUNTAIN_LARGEST pixels wide (and twice as tall) at FLARE_DEPTH_NEAR or nearer, a pixel
// narrower for every MOTE_FOUNTAIN_DEPTH_STEP farther.
#define FLARE_DEPTH_NEAR           314
#define FLARE_DEPTH_FAR            634
#define MOTE_FOUNTAIN_LARGEST      6
#define MOTE_FOUNTAIN_DEPTH_STEP   64

// Motes burst out of the user and float upwards, wobbling, and each target reacts in turn.
void btEffectRunMoteFountain(BATTLE_EFFECT_PARM *args, Sint32 mode)
{
    FIXED                point[3];
    SCREEN_POINT         pos;
    BATTLE_EFFECT_STATE *state;
    void                *canvas;
    BLITTER_PAIR         draw;
    BATTLE_CAMERA       *camera;
    Uint8               *sheet;
    Sint32               ticks;
    MOTION_OBJECT       *src;
    PARTICLE            *mote;
    Sint32               resource;
    Sint32               frame;
    Sint32               i;
    // four groups set out, 8 ticks apart
    Sint32               group = MOTE_VISIBLE_COUNT / 4;

    state                      = RamHeapSlots->battleEffect;
    canvas                     = RamHeapSlots->battleCanvas;
    camera                     = RamHeapSlots->cameraState;
    sheet                      = RamHeapSlots->battleSheet;
    if (mode == MOTE_FOUNTAIN_HEALING)
        btEffectStartCanvasLayer(0);
    else
        btEffectStartCanvasLayer(BGCNT_PRIORITY(1));

    // Motes keep the brighter of their level and the canvas's; the second blitter, mirrored top to
    // bottom, is loaded but not used.
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
    draw[0] = RamHeapSlots->ramCode;
    EFFECT_LOAD_BLITTER(
        HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y, BLITTER_MAX);
    draw[1] = RamHeapSlots->ramCodePaired;
    btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sheet, 0, 0);
    if (mode == MOTE_FOUNTAIN_HEALING)
        resource = RESOURCE_ORANGE_PALETTE_A;
    else if (mode == MOTE_FOUNTAIN_DRIFTING)
        resource = RESOURCE_EARTH_WALL_SHEET;
    else
        resource = RESOURCE_GREEN_PALETTE;
    syWordsCopyEntry((void *)BG_PLTT, syResourceGetTableEntry(resource), SHEET_PALETTE_BYTES);
    EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

    // Launch every mote from the user, with a speed and a direction of its own. Slaver throws them
    // up, faster, in a half-circle fan turned by the user's side, from up to 16 pixels higher.
    src = btObjectGetStub(args->user)->object;
    for (i = 0; i != MOTE_FOUNTAIN_INITIALIZED_COUNT; i++)
    {
        Sint32 speed;
        Sint32 angle;

        mote = &PARTICLES[i];
        if (mode == MOTE_FOUNTAIN_SLAVER)
        {
            if (args->side == EFFECT_SIDE_ENEMIES)
                angle = (syRandom() & 0x7fff) + DEGtoANG(180);
            else
                angle = syRandom() & 0x7fff;
            speed   = (syRandom() & 0x3ff) + 2048;
            mote->y = src->y + (syRandom() << 4) + toFIXED(5);
        }
        else
        {
            angle   = syRandom();
            speed   = (syRandom() & 0x3ff) + 32;
            mote->y = src->y + toFIXED(5);
        }
        mote->x         = src->x;
        mote->z         = src->z;
        mote->velocityX = sySin(angle) * speed >> 8;
        if (mode == MOTE_FOUNTAIN_SLAVER)
        {
            mote->velocityY = ((syRandom() & 255) + 128) << 10;
            mote->velocityZ = -(syCos(angle) * speed) >> 9;
        }
        else
        {
            mote->velocityY = ((syRandom() & 255) - 32) << 9;
            mote->velocityZ = -(syCos(angle) * speed << 1) >> 8;
        }
        mote->variant = (syRandom() & 31) + 48;
        if (mode == MOTE_FOUNTAIN_HEALING)
        {
            mote->velocityX /= 2;
            mote->velocityZ /= 2;
        }
    }

    ticks = MOTE_FOUNTAIN_SLAVER_TICKS;
    if (mode != MOTE_FOUNTAIN_SLAVER)
        ticks = MOTE_FOUNTAIN_TICKS;
    for (frame = 0; frame != ticks; frame++)
    {
        EFFECT_VIEW_FROM_CAMERA(camera);
        if (mode == MOTE_FOUNTAIN_SLAVER && frame == 0)
            sySoundPlayStub(SE_CURSE);

        for (i = 0; i != MOTE_VISIBLE_COUNT; i++)
        {
            mote = &PARTICLES[i];
            if (frame >= i / group * 8 && mote->variant >= 0)
            {
                Sint32 size;

                // Motes wobble and grow as they near the camera.
                point[0] = mote->x + sySin((i * 4 + mote->variant) * (TRIG_TURN / 64)) * 16;
                point[1] = mote->y;
                point[2] = mote->z;
                btEffectPosProjectPoint(point, &pos);
                pos.x >>= 1;
                if (pos.depth < FLARE_DEPTH_NEAR)
                    pos.depth = FLARE_DEPTH_NEAR;
                if (pos.depth > FLARE_DEPTH_FAR)
                    pos.depth = FLARE_DEPTH_FAR;
                size = MOTE_FOUNTAIN_LARGEST
                     - (pos.depth - FLARE_DEPTH_NEAR) / MOTE_FOUNTAIN_DEPTH_STEP;
                EFFECT_DRAW_MOTE(draw[0], canvas, sheet, pos.x, pos.y, size);

                // Slaver's drops fall and bounce off the ground; the others rise slowly. Sleep
                // Star's drift away from the user's side.
                if (mode == MOTE_FOUNTAIN_SLAVER)
                {
                    btParticleAdvanceWithGravity3D(mote, 62, -toFIXED(1) / 8);
                    if (mote->y < toFIXED(5))
                        mote->velocityY = -mote->velocityY;
                }
                else
                {
                    btParticleAdvanceWithGravity3D(mote, 62, toFIXED(1) / 64);
                }
                if (mode == MOTE_FOUNTAIN_DRIFTING)
                {
                    if (src->x < 0)
                        mote->velocityX += toFIXED(1) / 8;
                    else
                        mote->velocityX -= toFIXED(1) / 8;
                }
                mote->variant--;
            }
        }

        // From tick 48, flash the targets one after another, 8 ticks apart.
        if (mode == MOTE_FOUNTAIN_DRIFTING)
        {
            for (i = 0; i != args->targetCount; i++)
            {
                if (frame == i * 8 + 48)
                {
                    EFFECT_SHOW_DAMAGE(HIT_SOUND_NONE);
                    btEffectTintBattler(args->targets[i], SPRITE_COLOR_WHITE, POSE_DAMAGE, i, 8);
                }
            }
        }
        else if (mode == MOTE_FOUNTAIN_HEALING)
        {
            for (i = 0; i != args->targetCount; i++)
            {
                if (frame == i * 8 + 48)
                {
                    sySoundPlayStub(SE_RECOVERY);
                    EFFECT_SHOW_DAMAGE(HIT_SOUND_NONE);
                    btEffectTintBattler(args->targets[i], SPRITE_COLOR_WHITE, TINT_KEEP, i, 8);
                }
            }
        }
        EFFECT_END_TICK(state);
    }

    syCloseEvent(btEffectTransferCanvas);
    EFFECT_RELEASE_BLITTERS();
    if (mode == MOTE_FOUNTAIN_SLAVER)
    {
        // Clear the canvas and run Impair's effect on the targets.
        syWordsClear((void *)BG_CHAR_BLOCK(1), CANVAS_VIEW_BYTES);
        syWordsClear(canvas, CANVAS_VIEW_BYTES);
        syCloseEvent(btEffectDarkenBackgroundStep);
        btEffectRunImpair(args);
    }
    else
    {
        btEffectEndCanvasLayer();
    }
}

#undef STREAK_VALUE
#undef STREAK_ANIMATE
#undef STREAK_HIT
#undef STREAK_MOTION
#undef STREAK_SHAKE
#undef STREAK_MAX
#undef STREAK_WIDTH
#undef STREAK_HEIGHT
#undef STREAK_FRAME_BYTES
#undef STREAK_SPACING
#undef STREAK_FLIGHT
#undef STREAK_TARGET_DELAY
#undef MOTE_FOUNTAIN_TICKS
#undef MOTE_FOUNTAIN_SLAVER_TICKS
#undef FLARE_DEPTH_NEAR
#undef FLARE_DEPTH_FAR
#undef MOTE_FOUNTAIN_LARGEST
#undef MOTE_FOUNTAIN_DEPTH_STEP

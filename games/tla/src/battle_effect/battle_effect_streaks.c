/// @file
/// Streak volleys (Sonic Wave, Shriek, War Cry ...) and mote fountains (Sleep
/// Star, Soothing Star, Slaver).
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"
#include "gamepak/far_call/battle.h"
#include "resource_ids.h"

/// Column `n` of style `mode`'s row of gStreak_Styles, four flags a style.
#define STREAK_VALUE(mode, n) gStreak_Styles[(mode) * 4 + (n)]
/// Nonzero: the streak cycles through three images.
#define STREAK_ANIMATE 0
/// Nonzero: play the hit sound and flash the target.
#define STREAK_HIT 1
/// The target's knockback on the hit (enum BattleKnockback, -1: none).
#define STREAK_MOTION 2
/// Nonzero: the struck target trembles from side to side.
#define STREAK_SHAKE 3

#define STREAK_MAX 3 ///< streaks fired at each target

/// The streak volley's styles (BattleEffect_RunStreakVolley): its colour and
/// its row of gStreak_Styles.
enum StreakStyle {
  STREAK_STYLE_EMBER,  ///< Sonic Wave
  STREAK_STYLE_LIME,   ///< Shriek and Harpy, and the unused
                       ///< BattleEffect_RunLimeStreaks
  STREAK_STYLE_VIOLET, ///< Banshee Howl
  STREAK_STYLE_RED,    ///< Wicked Howl
  STREAK_STYLE_CYAN    ///< War Cry
};

/// Three streaks of ember light fly from the user at each target in turn; each
/// target flashes and recoils.
///
/// Abilities: Sonic Wave.
void BattleEffect_RunSonicWave(struct BattleEffectParams *args) {
  BattleEffect_RunStreakVolley(args, STREAK_STYLE_EMBER);
}

/// Three streaks of lime light fly from the user at each target in turn; each
/// target flashes and recoils.
///
/// Abilities: Shriek, Harpy.
void BattleEffect_RunShriek(struct BattleEffectParams *args) {
  BattleEffect_RunStreakVolley(args, STREAK_STYLE_LIME);
}

/// Three streaks of violet light fly from the user at each target in turn; each
/// target flashes and trembles.
///
/// Abilities: Banshee Howl.
void BattleEffect_RunBansheeHowl(struct BattleEffectParams *args) {
  BattleEffect_RunStreakVolley(args, STREAK_STYLE_VIOLET);
}

/// Three flickering cyan streaks fly from the user at each target in turn;
/// each target flashes and recoils.
///
/// Abilities: War Cry.
void BattleEffect_RunWarCry(struct BattleEffectParams *args) {
  BattleEffect_RunStreakVolley(args, STREAK_STYLE_CYAN);
}

/// Three flickering red streaks fly from the user at each target in turn; each
/// target flashes and trembles.
///
/// Abilities: Wicked Howl.
void BattleEffect_RunWickedHowl(struct BattleEffectParams *args) {
  BattleEffect_RunStreakVolley(args, STREAK_STYLE_RED);
}

/// Unused: no entry of the effect table points here. It would have played
/// the same lime streaks as Shriek.
UNUSED void BattleEffect_RunLimeStreaks(struct BattleEffectParams *args) {
  BattleEffect_RunStreakVolley(args, STREAK_STYLE_LIME);
}

/// FlameArcSheet's streaks: three animation frames, each six pictures that
/// grow; each picture is the lower half of a ring, and the upper half is it
/// drawn flipped, so a streak flies as a ring of light.
#define STREAK_WIDTH 20
#define STREAK_HEIGHT 40
#define STREAK_FRAME_BYTES (6 * PICTURE_BYTES(STREAK))
#define STREAK_SPACING 6       ///< frames between streaks
#define STREAK_FLIGHT 24       ///< frames from user to target
#define STREAK_TARGET_DELAY 32 ///< frames between targets

/// Fires the streaks at each target in turn from above the user; each leaves a
/// fading trail.
void BattleEffect_RunStreakVolley(struct BattleEffectParams *args, s32 mode) {
  struct ScreenPoint position;
  struct HeapSlots *heap;
  struct BattleEffectState *state;
  void *canvas;
  s32 k;
  DrawRectangle drawLower;
  DrawRectangle drawUpper;
  struct BattleCamera *camera;
  struct MotionObject *source;
  struct MotionObject *target;
  s32 resource;
  s32 frame;
  s32 j;
  s32 streaks = STREAK_MAX;
  s32 lift = toFIXED(20);

  /* FAKEMATCH(dead): heap set through its own address; as heap = Ram_HeapSlots.
   */
  *&heap = Ram_HeapSlots;
  state = (struct BattleEffectState *)heap->battleEffect;
  canvas = heap->battleCanvas;
  camera = heap->cameraState;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));

  // The upper half draws mirrored top to bottom; the far side mirrors
  // left to right as well.
  if (args->side == EFFECT_SIDE_ENEMIES) {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y, BLITTER_MAX);
    drawUpper = heap->ramCode;
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                        BLITTER_MAX);
    drawLower = heap->ramCodePaired;
  } else {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP,
                        BLITTER_MAX);
    drawUpper = heap->ramCode;
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
    drawLower = heap->ramCodePaired;
  }

  // The streak pictures, and the mode's colours.
  Resource_LoadAndDecompress(RESOURCE_FLAME_ARC_SHEET, state->sheet, 0, 0);
  switch (mode) {
  case STREAK_STYLE_EMBER:
    resource = RESOURCE_EMBER_STREAK_SHEET;
    break;
  case STREAK_STYLE_LIME:
    resource = RESOURCE_LIME_PALETTE;
    break;
  case STREAK_STYLE_VIOLET:
    resource = RESOURCE_VIOLET_PALETTE_D;
    break;
  case STREAK_STYLE_RED:
    resource = RESOURCE_RED_PALETTE_B;
    break;
  case STREAK_STYLE_CYAN:
  default:
    resource = RESOURCE_CYAN_PALETTE;
    break;
  }
  Iwram_CopyWordsEntry((void *)BG_PLTT, Resource_GetTableEntry(resource),
                       SHEET_PALETTE_BYTES);
  // Each transfer halves the canvas, leaving a trail.
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

  source = BattleObject_GetFar(args->user)->object;
  {
    // The loop counts STREAK_SPACING for each streak though targets are
    // STREAK_TARGET_DELAY apart, so with three or four targets it stops
    // before the last one's second flash.
    s32 totalFrames = streaks * args->targetCount * STREAK_SPACING + 48;

    // Each streak flies from lift above the user to lift above its target.
    EFFECT_FOR_EACH_TARGET(k, args) {
      target = BattleObject_GetFar(args->targets[k])->object;
      for (j = 0; j != streaks; j++) {
        struct Particle *streak = &state->particles[k * streaks + j];

        SET_POSITION(streak, source->x, source->y + lift, source->z);
        SET_VELOCITY(streak, (target->x - streak->x) / STREAK_FLIGHT,
                     (target->y + lift - streak->y) / STREAK_FLIGHT,
                     (target->z - streak->z) / STREAK_FLIGHT);
        streak->variant = 0; // frames since the streak set off
      }
    }

    for (frame = 0; frame != totalFrames; frame++) {
      s32 *row = state->bg2xLines;

      // A sine wave of BG2X offsets for the wave HBlank DMA, which nothing arms
      // here, so the canvas does not ripple. The last 16 frames fade out.
      for (k = 0; k != DISPLAY_HEIGHT; k++)
        *row++ = CANVAS_WAVE_BG2X(toFIXED(8) -
                                  Trig_Sin((frame + k) * (TRIG_TURN / 16)) * 2);
      if (frame > totalFrames - 16)
        REG_BLDALPHA = EFFECT_BLEND_CANVAS(totalFrames - frame);

      EFFECT_VIEW_FROM_CAMERA(camera);
      EFFECT_FOR_EACH_TARGET(k, args) {
        // Each target's streaks set out with the style's cry.
        if (frame == k * STREAK_TARGET_DELAY) {
          if (mode == STREAK_STYLE_LIME)
            Sound_PlayFar(SE_SHRIEK);
          if (mode == STREAK_STYLE_VIOLET)
            Sound_PlayFar(SE_STONE_GRIND);
          if (mode == STREAK_STYLE_RED)
            Sound_PlayFar(SE_BOSS_ROAR);
          if (mode == STREAK_STYLE_CYAN)
            Sound_PlayFar(SE_ITEM_BREAK);
        }
        if (frame >= k * STREAK_TARGET_DELAY) {
          for (j = 0; j != streaks; j++) {
            // From here lift holds the streak's start frame.
            lift = k * STREAK_TARGET_DELAY + j * STREAK_SPACING;
            if (frame >= lift) {
              struct Particle *streak = &state->particles[k * streaks + j];
              s32 step;

              // The picture changes every 8 frames, stopping at the sixth.
              EffectPosition_ProjectPoint((s32 *)streak, &position);
              position.x >>= 1;
              step = streak->variant / 8;
              if (step > 5)
                step = 5;
              if (STREAK_VALUE(mode, STREAK_ANIMATE)) {
                u8 *picture = &state->sheet[step * PICTURE_BYTES(STREAK) +
                                            frame / 2 % 3 * STREAK_FRAME_BYTES];

                drawUpper(canvas, picture, position.x - STREAK_WIDTH / 2,
                          position.y - STREAK_HEIGHT, STREAK_WIDTH,
                          STREAK_HEIGHT);
                drawLower(canvas, picture, position.x - STREAK_WIDTH / 2,
                          position.y, STREAK_WIDTH, STREAK_HEIGHT);
              } else {
                u8 *picture = &state->sheet[step * PICTURE_BYTES(STREAK) +
                                            2 * STREAK_FRAME_BYTES];

                drawUpper(canvas, picture, position.x - STREAK_WIDTH / 2,
                          position.y - STREAK_HEIGHT, STREAK_WIDTH,
                          STREAK_HEIGHT);
                drawLower(canvas, picture, position.x - STREAK_WIDTH / 2,
                          position.y, STREAK_WIDTH, STREAK_HEIGHT);
              }
              Particle_AdvanceWithGravity3D(streak, PARTICLE_DAMPING_NONE, 0);
              streak->variant++;
            }
          }

          // From 6 frames after the hit, tremble the target sideways for
          // 32 frames.
          if (STREAK_VALUE(mode, STREAK_SHAKE) &&
              frame >= k * STREAK_TARGET_DELAY + 30 &&
              frame < k * STREAK_TARGET_DELAY + 62) {
            struct MotionObject *struck =
                BattleObject_GetFar(args->targets[k])->object;

            struck->x += INTtoFIXED(
                gStreak_Tremble[(frame - k * STREAK_TARGET_DELAY - 30) % 8]);
            if (struck->x > 0)
              struck->x += toFIXED(1) / 2;
            else
              struck->x -= toFIXED(1) / 2;
            BattleEffect_TintBattler(args->targets[k], TINT_KEEP, POSE_DAMAGE,
                                     TINT_KEEP, 0);
          }

          // The hit: a sound and two flashes, 16 frames apart. The first
          // hit lets the battle show its results.
          if (STREAK_VALUE(mode, STREAK_HIT)) {
            if (frame == k * STREAK_TARGET_DELAY + 24) {
              Sound_PlayFar(SE_IMPACT);
              if (k == 0)
                EFFECT_SHOW_DAMAGE(HIT_SOUND_NONE);
              BattleEffect_TintBattler(args->targets[k], SPRITE_COLOR_WHITE,
                                       POSE_DAMAGE, k, 8);
            }
            if (frame == k * STREAK_TARGET_DELAY + 40)
              BattleEffect_TintBattler(args->targets[k], SPRITE_COLOR_WHITE,
                                       POSE_DAMAGE, k, 8);
          }
          if (STREAK_VALUE(mode, STREAK_MOTION) != -1 &&
              frame == k * STREAK_TARGET_DELAY + 24) {
            state->shakeFrames = 4;
            BattleMotion_ApplyKnockbackFar(args->targets[k],
                                           STREAK_VALUE(mode, STREAK_MOTION));
          }
        }
      }
      BattleCamera_Shake(8, 8);
      EFFECT_END_FRAME(state);
    }
  }

  EFFECT_FINISH_CANVAS();
}

/// The mote fountain's modes (BattleEffect_RunMoteFountain).
enum MoteFountainMode {
  MOTE_FOUNTAIN_HEALING,  ///< Soothing Star
  MOTE_FOUNTAIN_DRIFTING, ///< Sleep Star
  MOTE_FOUNTAIN_SLAVER    ///< Slaver
};

/// The motes the fountain sets up, and how many of them it moves and draws.
enum { MOTE_FOUNTAIN_INITIALIZED_COUNT = 256, MOTE_VISIBLE_COUNT = 128 };

/// Motes burst out of the user and float up, wobbling, as they drift across the
/// field onto the targets, and each target flinches in turn.
///
/// Abilities: Sleep Star.
void BattleEffect_RunSleepStar(struct BattleEffectParams *args) {
  BattleEffect_RunMoteFountain(args, MOTE_FOUNTAIN_DRIFTING);
}

/// Orange healing motes burst out of the user and float up close to it, and
/// each target glows in turn with the recovery sound.
///
/// Abilities: Soothing Star.
void BattleEffect_RunSoothingStar(struct BattleEffectParams *args) {
  BattleEffect_RunMoteFountain(args, MOTE_FOUNTAIN_HEALING);
}

/// Green drops are flung up from the user in a half-circle fan and fall,
/// bouncing off the ground, then Impair plays on the targets.
///
/// Abilities: Slaver.
void BattleEffect_RunSlaver(struct BattleEffectParams *args) {
  BattleEffect_RunMoteFountain(args, MOTE_FOUNTAIN_SLAVER);
}

#define MOTE_FOUNTAIN_FRAMES 128
#define MOTE_FOUNTAIN_SLAVER_FRAMES 96

/// The projected depths over which a mote is sized, nearest to farthest: it is
/// drawn MOTE_FOUNTAIN_LARGEST pixels wide (and twice as tall) at
/// FLARE_DEPTH_NEAR or nearer, a pixel narrower for every
/// MOTE_FOUNTAIN_DEPTH_STEP farther.
#define FLARE_DEPTH_NEAR 314
#define FLARE_DEPTH_FAR 634
#define MOTE_FOUNTAIN_LARGEST 6
#define MOTE_FOUNTAIN_DEPTH_STEP 64

/// Motes burst out of the user and float upwards, wobbling, and each target
/// reacts in turn.
void BattleEffect_RunMoteFountain(struct BattleEffectParams *args, s32 mode) {
  FIXED point[3];
  struct ScreenPoint position;
  struct BattleEffectState *state;
  void *canvas;
  BlitterPair draw;
  struct BattleCamera *camera;
  u8 *sheet;
  s32 frames;
  struct MotionObject *source;
  struct Particle *mote;
  s32 resource;
  s32 frame;
  s32 i;
  s32 group = MOTE_VISIBLE_COUNT / 4; // four groups set out, 8 frames apart

  state = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  camera = Ram_HeapSlots->cameraState;
  sheet = Ram_HeapSlots->battleSheet;
  if (mode == MOTE_FOUNTAIN_HEALING)
    BattleEffect_StartCanvasLayer(0);
  else
    BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));

  // Motes keep the brighter of their level and the canvas's; the second
  // blitter, mirrored top to bottom, is loaded but not used.
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y, BLITTER_MAX);
  draw[1] = (DrawRectangle)Ram_HeapSlots->ramCodePaired;
  Resource_LoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sheet, 0, 0);
  if (mode == MOTE_FOUNTAIN_HEALING)
    resource = RESOURCE_ORANGE_PALETTE_A;
  else if (mode == MOTE_FOUNTAIN_DRIFTING)
    resource = RESOURCE_EARTH_WALL_SHEET;
  else
    resource = RESOURCE_GREEN_PALETTE;
  Iwram_CopyWordsEntry((void *)BG_PLTT, Resource_GetTableEntry(resource),
                       SHEET_PALETTE_BYTES);
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE,
                        CANVAS_FADE_THREE_QUARTERS);

  // Launch every mote from the user, with a speed and a direction of its
  // own. Slaver throws them up, faster, in a half-circle
  // fan turned by the user's side, from up to 16 pixels higher.
  source = BattleObject_GetFar(args->user)->object;
  for (i = 0; i != MOTE_FOUNTAIN_INITIALIZED_COUNT; i++) {
    s32 speed;
    s32 angle;

    mote = &PARTICLES[i];
    if (mode == MOTE_FOUNTAIN_SLAVER) {
      if (args->side == EFFECT_SIDE_ENEMIES)
        angle = (Random16() & 0x7fff) + DEGtoANG(180);
      else
        angle = Random16() & 0x7fff;
      speed = (Random16() & 0x3ff) + 2048;
      mote->y = source->y + (Random16() << 4) + toFIXED(5);
    } else {
      angle = Random16();
      speed = (Random16() & 0x3ff) + 32;
      mote->y = source->y + toFIXED(5);
    }
    mote->x = source->x;
    mote->z = source->z;
    mote->velocityX = Trig_Sin(angle) * speed >> 8;
    if (mode == MOTE_FOUNTAIN_SLAVER) {
      mote->velocityY = ((Random16() & 255) + 128) << 10;
      mote->velocityZ = -(Trig_Cos(angle) * speed) >> 9;
    } else {
      mote->velocityY = ((Random16() & 255) - 32) << 9;
      mote->velocityZ = -(Trig_Cos(angle) * speed << 1) >> 8;
    }
    mote->variant = (Random16() & 31) + 48;
    if (mode == MOTE_FOUNTAIN_HEALING) {
      mote->velocityX /= 2;
      mote->velocityZ /= 2;
    }
  }

  frames = MOTE_FOUNTAIN_SLAVER_FRAMES;
  if (mode != MOTE_FOUNTAIN_SLAVER)
    frames = MOTE_FOUNTAIN_FRAMES;
  for (frame = 0; frame != frames; frame++) {
    EFFECT_VIEW_FROM_CAMERA(camera);
    if (mode == MOTE_FOUNTAIN_SLAVER && frame == 0)
      Sound_PlayFar(SE_CURSE);

    for (i = 0; i != MOTE_VISIBLE_COUNT; i++) {
      mote = &PARTICLES[i];
      if (frame >= i / group * 8 && mote->variant >= 0) {
        s32 size;

        // Motes wobble and grow as they near the camera.
        point[0] =
            mote->x + Trig_Sin((i * 4 + mote->variant) * (TRIG_TURN / 64)) * 16;
        point[1] = mote->y;
        point[2] = mote->z;
        EffectPosition_ProjectPoint(point, &position);
        position.x >>= 1;
        if (position.depth < FLARE_DEPTH_NEAR)
          position.depth = FLARE_DEPTH_NEAR;
        if (position.depth > FLARE_DEPTH_FAR)
          position.depth = FLARE_DEPTH_FAR;
        size = MOTE_FOUNTAIN_LARGEST -
               (position.depth - FLARE_DEPTH_NEAR) / MOTE_FOUNTAIN_DEPTH_STEP;
        EFFECT_DRAW_MOTE(draw[0], canvas, sheet, position.x, position.y, size);

        // Slaver's drops fall and bounce off the ground; the others rise
        // slowly. Sleep Star's drift away from the user's side.
        if (mode == MOTE_FOUNTAIN_SLAVER) {
          Particle_AdvanceWithGravity3D(mote, 62, -toFIXED(1) / 8);
          if (mote->y < toFIXED(5))
            mote->velocityY = -mote->velocityY;
        } else {
          Particle_AdvanceWithGravity3D(mote, 62, toFIXED(1) / 64);
        }
        if (mode == MOTE_FOUNTAIN_DRIFTING) {
          if (source->x < 0)
            mote->velocityX += toFIXED(1) / 8;
          else
            mote->velocityX -= toFIXED(1) / 8;
        }
        mote->variant--;
      }
    }

    // From frame 48, flash the targets one after another, 8 frames apart.
    if (mode == MOTE_FOUNTAIN_DRIFTING) {
      for (i = 0; i != args->targetCount; i++) {
        if (frame == i * 8 + 48) {
          EFFECT_SHOW_DAMAGE(HIT_SOUND_NONE);
          BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, i, 8);
        }
      }
    } else if (mode == MOTE_FOUNTAIN_HEALING) {
      for (i = 0; i != args->targetCount; i++) {
        if (frame == i * 8 + 48) {
          Sound_PlayFar(SE_RECOVERY);
          EFFECT_SHOW_DAMAGE(HIT_SOUND_NONE);
          BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                   TINT_KEEP, i, 8);
        }
      }
    }
    EFFECT_END_FRAME(state);
  }

  Task_Remove(BattleEffect_TransferCanvas);
  EFFECT_RELEASE_BLITTERS();
  if (mode == MOTE_FOUNTAIN_SLAVER) {
    // Clear the canvas and run Impair's effect on the targets.
    Iwram_ClearWords((void *)BG_CHAR_BLOCK(1), CANVAS_VIEW_BYTES);
    Iwram_ClearWords(canvas, CANVAS_VIEW_BYTES);
    Task_Remove(BattleEffect_DarkenBackgroundStep);
    BattleEffect_RunImpair(args);
  } else {
    BattleEffect_EndCanvasLayer();
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
#undef MOTE_FOUNTAIN_FRAMES
#undef MOTE_FOUNTAIN_SLAVER_FRAMES
#undef FLARE_DEPTH_NEAR
#undef FLARE_DEPTH_FAR
#undef MOTE_FOUNTAIN_LARGEST
#undef MOTE_FOUNTAIN_DEPTH_STEP

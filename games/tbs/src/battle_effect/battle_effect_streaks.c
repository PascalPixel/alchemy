/// @file
/// Streak volleys (Sonic Wave, Shriek, War Cry ...) and mote fountains (Sleep
/// Star, Soothing Star).
// Part of battle_effect.c.

#include "battle_effect_internal.h"

/// Column `n` of style `mode`'s row of gStreak_Styles, four flags a style.
#define STREAK_VALUE(mode, n) gStreak_Styles[(mode)][(n)]
#define STREAK_ANIMATE 0 ///< nonzero: the streak cycles through three images
#define STREAK_HIT 1     ///< nonzero: play the hit sound and flash the target
/// The target's knockback on the hit (enum BattleKnockback), or -1 for none.
#define STREAK_MOTION 2
/// Nonzero: the struck target trembles from side to side.
#define STREAK_SHAKE 3

#define STREAK_MAX 3 ///< streaks fired at each target

/// The streak volley's styles: its colour and its row of gStreak_Styles.
enum StreakStyle {
  STREAK_STYLE_EMBER,  ///< Sonic Wave
  STREAK_STYLE_LIME,   ///< Shriek, and the unused BattleEffect_RunLimeStreaks
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
/// Abilities: Shriek.
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
/// Three flickering cyan streaks, cycling through three colour rows, fly from
/// the user at each target in turn; each target flashes and recoils.
///
/// Abilities: War Cry.
void BattleEffect_RunWarCry(struct BattleEffectParams *args) {
  BattleEffect_RunStreakVolley(args, STREAK_STYLE_CYAN);
}
/// Three flickering red streaks, cycling through three colour rows, fly from
/// the user at each target in turn; each target flashes and trembles.
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
/// grow; the lower half is the upper drawn flipped.
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
  struct BattleEffectState *state;
  void *canvas;
  s32 k;
  DrawRectangle drawLower;
  DrawRectangle drawUpper;
  struct BattleCamera *camera;
  struct Actor *source;
  struct Actor *target;
  s32 resource;
  s32 frame;
  s32 streakIndex;
  s32 streaks = STREAK_MAX;
  FIXED lift = toFIXED(20); // the streaks fly 20 pixels above the ground

  state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
  state->args = args;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));

  if (state->args->side == EFFECT_SIDE_ENEMIES) {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y,
                        COMPOSITE_LIGHTEN);
    drawUpper = HEAP_SLOT_BLOCK(HEAP_SLOT_RAM_CODE);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                        COMPOSITE_LIGHTEN);
    drawLower = HEAP_SLOT_BLOCK(HEAP_SLOT_RAM_CODE_PAIRED);
  } else {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP,
                        COMPOSITE_LIGHTEN);
    drawUpper = HEAP_SLOT_BLOCK(HEAP_SLOT_RAM_CODE);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                        COMPOSITE_LIGHTEN);
    drawLower = HEAP_SLOT_BLOCK(HEAP_SLOT_RAM_CODE_PAIRED);
  }

  Resource_LoadAndDecompress(RESOURCE_FLAME_ARC_SHEET, state->sheet, 0, 0);
  switch (mode) {
  case STREAK_STYLE_EMBER:
    resource = RESOURCE_EMBER_STREAK_SHEET;
    break;
  case STREAK_STYLE_LIME:
    resource = RESOURCE_LIME_PALETTE;
    break;
  case STREAK_STYLE_VIOLET:
    resource = RESOURCE_JUPITER_STAR_FIELD_PALETTE;
    break;
  case STREAK_STYLE_RED:
    resource = RESOURCE_VOLCANO_RED_PALETTE;
    break;
  case STREAK_STYLE_CYAN:
  default:
    resource = RESOURCE_CYAN_PALETTE;
    break;
  }
  Iwram_CopyWords(BG_PLTT, Resource_GetTableEntry(resource),
                  SHEET_PALETTE_BYTES);
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

  source = BattleObject_GetFar(state->args->user)->actor;
  {
    // The loop counts 6 frames for each streak though targets are 32 frames
    // apart, so with three or four targets it stops before the last one's
    // second flash.
    s32 totalFrames = streaks * state->args->targetCount * STREAK_SPACING + 48;

    EFFECT_FOR_EACH_TARGET(k, state->args) {
      target = BattleObject_GetFar(state->args->targets[k])->actor;
      for (streakIndex = 0; streakIndex != streaks; streakIndex++) {
        struct Particle *streak = &state->particles[k * streaks + streakIndex];

        SET_POSITION(streak, source->x.fixed, source->y.fixed + lift,
                     source->z.fixed);
        SET_VELOCITY(streak, (target->x.fixed - streak->x) / STREAK_FLIGHT,
                     (target->y.fixed + lift - streak->y) / STREAK_FLIGHT,
                     (target->z.fixed - streak->z) / STREAK_FLIGHT);
        streak->variant = 0; // frames since the streak set off
      }
    }

    for (frame = 0; frame != totalFrames; frame++) {
      s32 *row = state->bg2xLines;

      // A sine wave of BG2X offsets for the wave HBlank DMA, which nothing arms
      // here, so the canvas does not ripple.
      for (k = 0; k != DISPLAY_HEIGHT; k++)
        *row++ = CANVAS_WAVE_BG2X(toFIXED(8) -
                                  Trig_Sin((frame + k) * (TRIG_TURN / 16)) * 2);
      if (frame > totalFrames - 16)
        REG_BLDALPHA = EFFECT_BLEND_CANVAS(totalFrames - frame);
      EFFECT_VIEW_FROM_CAMERA(camera);
      EFFECT_FOR_EACH_TARGET(k, state->args) {
        if (frame >= k * STREAK_TARGET_DELAY) {
          for (streakIndex = 0; streakIndex != streaks; streakIndex++) {
            if (frame >=
                k * STREAK_TARGET_DELAY + streakIndex * STREAK_SPACING) {
              struct Particle *streak =
                  &state->particles[k * streaks + streakIndex];
              s32 step;

              Projection_Point(streak, &position);
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

          // The target trembles.
          if (STREAK_VALUE(mode, STREAK_SHAKE) &&
              frame >= k * STREAK_TARGET_DELAY + 30 &&
              frame < k * STREAK_TARGET_DELAY + 62) {
            struct Actor *struck =
                BattleObject_GetFar(state->args->targets[k])->actor;

            struck->x.fixed += INTtoFIXED(
                gStreak_Tremble[(frame - k * STREAK_TARGET_DELAY - 30) % 8]);
            if (struck->x.fixed > 0)
              struck->x.fixed += toFIXED(1) / 2;
            else
              struck->x.fixed -= toFIXED(1) / 2;
            BattleEffect_TintBattler(state->args->targets[k], TINT_KEEP,
                                     POSE_DAMAGE, TINT_KEEP, 0);
          }

          // The hit: the target flashes, and again 16 frames later.
          if (STREAK_VALUE(mode, STREAK_HIT)) {
            if (frame == k * STREAK_TARGET_DELAY + 24) {
              Sound_PlayFar(SE_IMPACT);
              if (k == 0) // the first hit lets the battle show the damage
                EFFECT_SHOW_DAMAGE(-1);
              BattleEffect_TintBattler(state->args->targets[k],
                                       SPRITE_COLOR_WHITE, POSE_DAMAGE, k, 8);
            }
            if (frame == k * STREAK_TARGET_DELAY + 40)
              BattleEffect_TintBattler(state->args->targets[k],
                                       SPRITE_COLOR_WHITE, POSE_DAMAGE, k, 8);
          }
          if (STREAK_VALUE(mode, STREAK_MOTION) != -1 &&
              frame == k * STREAK_TARGET_DELAY + 24) {
            state->shakeFrames = 4;
            BattleMotion_ApplyKnockbackFar(state->args->targets[k],
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
  MOTE_FOUNTAIN_HEALING, ///< Soothing Star
  MOTE_FOUNTAIN_DRIFTING ///< Sleep Star
};

/// The motes the fountains set out, and the ones drawn.
enum { MOTE_FOUNTAIN_MOTES = 256, MOTE_FOUNTAIN_MOTES_DRAWN = 128 };

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
#define MOTE_FOUNTAIN_FRAMES 128

/// The projected depths over which a mote is sized, nearest to farthest: it is
/// drawn MOTE_FOUNTAIN_LARGEST pixels wide (and twice as tall) at
/// MOTE_FOUNTAIN_DEPTH_NEAR or nearer, a pixel narrower for every
/// MOTE_FOUNTAIN_DEPTH_STEP farther.
#define MOTE_FOUNTAIN_DEPTH_NEAR 314
#define MOTE_FOUNTAIN_DEPTH_FAR 634
#define MOTE_FOUNTAIN_LARGEST 6
#define MOTE_FOUNTAIN_DEPTH_STEP 64

/// Motes burst out of the user and float upwards, wobbling, and each target
/// reacts in turn.
void BattleEffect_RunMoteFountain(struct BattleEffectParams *args, s32 mode) {
  s32 worldPoint[3];
  struct ScreenPoint position;
  struct BattleEffectState *state;
  void *canvas;
  BlitterPair draw;
  struct BattleCamera *camera;
  u8 *motes;
  struct Actor *source;
  struct Particle *mote;
  s32 resource;
  s32 frame;
  s32 i;

  state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
  motes = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
  state->args = args;
  if (mode == MOTE_FOUNTAIN_HEALING)
    BattleEffect_StartCanvasLayer(0);
  else
    BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_LIGHTEN);
  draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y, COMPOSITE_LIGHTEN);
  draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
  Resource_LoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, motes, 0, 0);
  if (mode == MOTE_FOUNTAIN_HEALING)
    resource = RESOURCE_GAIA_ORANGE_PALETTE;
  else
    resource = RESOURCE_EARTH_WALL_SHEET;
  Iwram_CopyWords(BG_PLTT, Resource_GetTableEntry(resource),
                  SHEET_PALETTE_BYTES);
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE,
                        CANVAS_FADE_THREE_QUARTERS);

  source = BattleObject_GetFar(state->args->user)->actor;
  for (i = 0; i != MOTE_FOUNTAIN_MOTES; i++) {
    s32 speed;
    s32 angle;

    mote = &PARTICLES[i];

    speed = (Random16() & 0x3ff) + 32;
    angle = Random16() & TRIG_ANGLE_MASK;
    SET_POSITION(mote, source->x.fixed, source->y.fixed + toFIXED(5),
                 source->z.fixed);
    SET_VELOCITY(mote, Trig_Sin(angle) * speed >> 8,
                 ((Random16() & 255) - 32) << 9,
                 -(Trig_Cos(angle) * speed << 1) >> 8);
    mote->variant = (Random16() & 31) + 48;
    if (mode == MOTE_FOUNTAIN_HEALING) {
      mote->velocityX /= 2;
      mote->velocityZ /= 2;
    }
  }

  for (frame = 0; frame != MOTE_FOUNTAIN_FRAMES; frame++) {
    EFFECT_VIEW_FROM_CAMERA(camera);
    for (i = 0; i != MOTE_FOUNTAIN_MOTES_DRAWN; i++) {
      mote = &PARTICLES[i];
      if (frame >= i / 32 * 8 && mote->variant >= 0) {
        s32 size;

        // Motes wobble and grow as they near the camera.
        SET_VECTOR(
            worldPoint,
            mote->x + Trig_Sin((i * 4 + mote->variant) * (TRIG_TURN / 64)) * 16,
            mote->y, mote->z);
        Projection_Point(worldPoint, &position);
        position.x >>= 1;
        if (position.depth < MOTE_FOUNTAIN_DEPTH_NEAR)
          position.depth = MOTE_FOUNTAIN_DEPTH_NEAR;
        if (position.depth > MOTE_FOUNTAIN_DEPTH_FAR)
          position.depth = MOTE_FOUNTAIN_DEPTH_FAR;
        size = MOTE_FOUNTAIN_LARGEST -
               (position.depth - MOTE_FOUNTAIN_DEPTH_NEAR) /
                   MOTE_FOUNTAIN_DEPTH_STEP;
        EFFECT_DRAW_MOTE(draw[0], canvas, motes, position.x, position.y, size);
        Particle_AdvanceWithGravity3D(mote, 62, toFIXED(1) / 64);
        // The drifting mode pushes the motes across the field.
        if (mode == MOTE_FOUNTAIN_DRIFTING) {
          if (source->x.fixed < 0)
            mote->velocityX += toFIXED(1) / 8;
          else
            mote->velocityX -= toFIXED(1) / 8;
        }
        mote->variant--;
      }
    }
    if (mode == MOTE_FOUNTAIN_DRIFTING) {
      EFFECT_FOR_EACH_TARGET(i, state->args) {
        if (frame == i * 8 + 48) {
          EFFECT_SHOW_DAMAGE(-1);
          BattleEffect_TintBattler(state->args->targets[i], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, i, 8);
        }
      }
    } else {
      EFFECT_FOR_EACH_TARGET(i, state->args) {
        if (frame == i * 8 + 48) {
          Sound_PlayFar(SE_RECOVERY);
          EFFECT_SHOW_DAMAGE(-1);
          BattleEffect_TintBattler(state->args->targets[i], SPRITE_COLOR_WHITE,
                                   TINT_KEEP, i, 8);
        }
      }
    }
    EFFECT_END_FRAME(state);
  }

  EFFECT_FINISH_CANVAS();
}

#undef STREAK_VALUE
#undef STREAK_ANIMATE
#undef STREAK_HIT
#undef STREAK_MOTION
#undef STREAK_SHAKE
#undef STREAK_MAX
#undef STREAK_SPACING
#undef STREAK_FLIGHT
#undef STREAK_TARGET_DELAY
#undef MOTE_FOUNTAIN_FRAMES
#undef MOTE_FOUNTAIN_DEPTH_NEAR
#undef MOTE_FOUNTAIN_DEPTH_FAR
#undef MOTE_FOUNTAIN_LARGEST
#undef MOTE_FOUNTAIN_DEPTH_STEP

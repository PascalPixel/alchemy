/// @file
/// Streak volleys (Sonic Wave, Shriek, War Cry ...) and mote fountains (Sleep
/// Star, Soothing Star).
// Part of battle_effect.c.

#include "../battle_effect_internal.h"

/// Column `n` of style `mode`'s row of gStreaks_Styles, four flags a style.
#define STREAKS_VALUE(mode, n) gStreaks_Styles[(mode)][(n)]
#define STREAKS_ANIMATE 0 ///< nonzero: the streak cycles through three images
#define STREAKS_HIT 1     ///< nonzero: play the hit sound and flash the target
#define STREAKS_MOTION                                                         \
  2 ///< the target's knockback on the hit (enum BattleKnockback, -1: none)
/// nonzero: the struck target trembles from side to side
#define STREAKS_SHAKE 3

#define STREAKS_MAX 3 ///< streaks fired at each target

/// The streak volley's styles: its colour and its row of gStreaks_Styles.
enum StreakStyle {
  STREAKS_STYLE_EMBER,  ///< Sonic Wave
  STREAKS_STYLE_LIME,   ///< Shriek, and the unused BattleEffect_RunLimeStreaks
  STREAKS_STYLE_VIOLET, ///< Banshee Howl
  STREAKS_STYLE_RED,    ///< Wicked Howl
  STREAKS_STYLE_CYAN    ///< War Cry
};

/// Three streaks of ember light fly from the user at each target in turn; each
/// target flashes and recoils.
///
/// Abilities: Sonic Wave.
void BattleEffect_RunSonicWave(struct BattleEffectParams *args) {
  BattleEffect_RunStreakVolley(args, STREAKS_STYLE_EMBER);
}
/// Three streaks of lime light fly from the user at each target in turn; each
/// target flashes and recoils.
///
/// Abilities: Shriek.
void BattleEffect_RunShriek(struct BattleEffectParams *args) {
  BattleEffect_RunStreakVolley(args, STREAKS_STYLE_LIME);
}
/// Three streaks of violet light fly from the user at each target in turn; each
/// target flashes and trembles.
///
/// Abilities: Banshee Howl.
void BattleEffect_RunBansheeHowl(struct BattleEffectParams *args) {
  BattleEffect_RunStreakVolley(args, STREAKS_STYLE_VIOLET);
}
/// Three flickering cyan streaks, cycling through three colour rows, fly from
/// the user at each target in turn; each target flashes and recoils.
///
/// Abilities: War Cry.
void BattleEffect_RunWarCry(struct BattleEffectParams *args) {
  BattleEffect_RunStreakVolley(args, STREAKS_STYLE_CYAN);
}
/// Three flickering red streaks, cycling through three colour rows, fly from
/// the user at each target in turn; each target flashes and trembles.
///
/// Abilities: Wicked Howl.
void BattleEffect_RunWickedHowl(struct BattleEffectParams *args) {
  BattleEffect_RunStreakVolley(args, STREAKS_STYLE_RED);
}
/// Unused: no entry of the effect table points here. It would have played
/// the same lime streaks as Shriek.
UNUSED void BattleEffect_RunLimeStreaks(struct BattleEffectParams *args) {
  BattleEffect_RunStreakVolley(args, STREAKS_STYLE_LIME);
}

#define STREAKS_SPACING 6       ///< frames between streaks
#define STREAKS_FLIGHT 24       ///< frames from user to target
#define STREAKS_TARGET_DELAY 32 ///< frames between targets

/// Fires the streaks at each target in turn from above the user; each leaves a
/// fading trail.
void BattleEffect_RunStreakVolley(struct BattleEffectParams *args, s32 mode) {
  struct ScreenPoint position;
  struct BattleEffectWork *work;
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
  s32 streaks = STREAKS_MAX;
  s32 lift = px(20); // 20 pixels, 16.16

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
  work->args = args;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));

  if (work->args->side == EFFECT_SIDE_ENEMIES) {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y,
                        COMPOSITE_LIGHTEN);
    drawUpper = HEAP_SLOT_BLOCK(HEAP_SLOT_RAM_CODE);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, EFFECT_CLIP_BOUNDS,
                        COMPOSITE_LIGHTEN);
    drawLower = HEAP_SLOT_BLOCK(HEAP_SLOT_RAM_CODE_2);
  } else {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP,
                        COMPOSITE_LIGHTEN);
    drawUpper = HEAP_SLOT_BLOCK(HEAP_SLOT_RAM_CODE);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                        COMPOSITE_LIGHTEN);
    drawLower = HEAP_SLOT_BLOCK(HEAP_SLOT_RAM_CODE_2);
  }

  Resource_LoadAndDecompress((s32)&ResourceId_FiveModeImage, work->sheet, 0, 0);
  switch (mode) {
  case STREAKS_STYLE_EMBER:
    resource = (s32)&ResourceId_EmberStreakSheet;
    break;
  case STREAKS_STYLE_LIME:
    resource = (s32)&ResourceId_LimePalette;
    break;
  case STREAKS_STYLE_VIOLET:
    resource = (s32)&ResourceId_VioletPaletteD;
    break;
  case STREAKS_STYLE_RED:
    resource = (s32)&ResourceId_RedPaletteB;
    break;
  case STREAKS_STYLE_CYAN:
  default:
    resource = (s32)&ResourceId_CyanPalette;
    break;
  }
  Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(resource), 128);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

  source = GetBattleObjectSlotFar(work->args->user)->actor;
  {
    // The loop counts 6 frames for each streak though targets are 32 frames
    // apart, so with three or four targets it stops before the last one's
    // second flash.
    s32 totalFrames = streaks * work->args->targetCount * STREAKS_SPACING + 48;

    EFFECT_FOR_EACH_TARGET(k, work->args) {
      target = GetBattleObjectSlotFar(work->args->targets[k])->actor;
      for (streakIndex = 0; streakIndex != streaks; streakIndex++) {
        struct Particle *streak = &work->particles[k * streaks + streakIndex];

        SET_POSITION(streak, source->x.fixed, source->y.fixed + lift,
                     source->z.fixed);
        SET_VELOCITY(streak, (target->x.fixed - streak->x) / STREAKS_FLIGHT,
                     (target->y.fixed + lift - streak->y) / STREAKS_FLIGHT,
                     (target->z.fixed - streak->z) / STREAKS_FLIGHT);
        streak->variant = 0; // frames since the streak set off
      }
    }

    for (frame = 0; frame != totalFrames; frame++) {
      s32 *row = work->bg2xLines;

      // A sine wave of BG2X offsets for the wave HBlank DMA, which nothing arms
      // here, so the canvas does not ripple.
      for (k = 0; k != DISPLAY_HEIGHT; k++)
        *row++ = ((8 << 16) - Trig_Sin((frame + k) << 12) * 2) >> 10;
      if (frame > totalFrames - 16)
        REG_BLDALPHA = BLEND_ALPHA(totalFrames - frame, 16);
      EFFECT_VIEW_FROM_CAMERA(camera);
      EFFECT_FOR_EACH_TARGET(k, work->args) {
        if (frame >= k * STREAKS_TARGET_DELAY) {
          for (streakIndex = 0; streakIndex != streaks; streakIndex++) {
            if (frame >=
                k * STREAKS_TARGET_DELAY + streakIndex * STREAKS_SPACING) {
              struct Particle *streak =
                  &work->particles[k * streaks + streakIndex];
              s32 step;

              Projection_Point(streak, &position);
              position.x >>= 1;
              step = streak->variant / 8;
              if (step > 5)
                step = 5;
              if (STREAKS_VALUE(mode, STREAKS_ANIMATE)) {
                u8 *picture = &work->sheet[step * 800 + frame / 2 % 3 * 4800];

                drawUpper(canvas, picture, position.x - 10, position.y - 40, 20,
                          40);
                drawLower(canvas, picture, position.x - 10, position.y, 20, 40);
              } else {
                u8 *picture = &work->sheet[step * 800 + 2 * 4800];

                drawUpper(canvas, picture, position.x - 10, position.y - 40, 20,
                          40);
                drawLower(canvas, picture, position.x - 10, position.y, 20, 40);
              }
              Particle_AdvanceWithGravity3D(streak, 64, 0);
              streak->variant++;
            }
          }

          // The target trembles.
          if (STREAKS_VALUE(mode, STREAKS_SHAKE) &&
              frame >= k * STREAKS_TARGET_DELAY + 30 &&
              frame < k * STREAKS_TARGET_DELAY + 62) {
            struct Actor *struck =
                GetBattleObjectSlotFar(work->args->targets[k])->actor;

            struck->x.fixed += px(
                gStreaks_Tremble[(frame - k * STREAKS_TARGET_DELAY - 30) % 8]);
            if (struck->x.fixed > 0)
              struck->x.fixed += px(1) / 2;
            else
              struck->x.fixed -= px(1) / 2;
            BattleEffect_TintBattler(work->args->targets[k], TINT_KEEP,
                                     POSE_DAMAGE, TINT_KEEP, 0);
          }

          // The hit: the target flashes, and again 16 frames later.
          if (STREAKS_VALUE(mode, STREAKS_HIT)) {
            if (frame == k * STREAKS_TARGET_DELAY + 24) {
              Sound_PlayFar(SE_IMPACT);
              if (k == 0) // the first hit lets the battle show the damage
                EFFECT_SHOW_DAMAGE(-1);
              BattleEffect_TintBattler(work->args->targets[k],
                                       SPRITE_COLOR_WHITE, POSE_DAMAGE, k, 8);
            }
            if (frame == k * STREAKS_TARGET_DELAY + 40)
              BattleEffect_TintBattler(work->args->targets[k],
                                       SPRITE_COLOR_WHITE, POSE_DAMAGE, k, 8);
          }
          if (STREAKS_VALUE(mode, STREAKS_MOTION) != -1 &&
              frame == k * STREAKS_TARGET_DELAY + 24) {
            work->shakeFrames = 4;
            BattleMotion_ApplyKnockbackFar(work->args->targets[k],
                                           STREAKS_VALUE(mode, STREAKS_MOTION));
          }
        }
      }
      BattleCamera_Shake(8, 8);
      EFFECT_END_FRAME(work);
    }
  }

  EFFECT_FINISH_CANVAS();
}

/// The mote fountain's modes (BattleEffect_RunMoteFountain).
enum MoteFountainMode {
  MOTE_FOUNTAIN_FOUNTAIN_HEALING, ///< Soothing Star
  MOTE_FOUNTAIN_DRIFTING          ///< Sleep Star
};

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
  BattleEffect_RunMoteFountain(args, MOTE_FOUNTAIN_FOUNTAIN_HEALING);
}
#define MOTE_FOUNTAIN_FOUNTAIN_FRAMES 128

/// Motes burst out of the user and float upwards, wobbling, and each target
/// reacts in turn.
void BattleEffect_RunMoteFountain(struct BattleEffectParams *args, s32 mode) {
  s32 worldPoint[3];
  struct ScreenPoint position;
  struct BattleEffectWork *work;
  void *canvas;
  BlitterPair draw;
  struct BattleCamera *camera;
  u8 *sheet;
  struct Actor *source;
  struct Particle *mote;
  s32 resource;
  s32 frame;
  s32 i;

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
  sheet = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
  work->args = args;
  if (mode == MOTE_FOUNTAIN_FOUNTAIN_HEALING)
    BattleEffect_StartCanvasLayer(0);
  else
    BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_LIGHTEN);
  draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y, COMPOSITE_LIGHTEN);
  draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_2);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  if (mode == MOTE_FOUNTAIN_FOUNTAIN_HEALING)
    resource = (s32)&ResourceId_OrangePaletteA;
  else
    resource = (s32)&ResourceId_EarthWallSheet;
  Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(resource), 128);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

  source = GetBattleObjectSlotFar(work->args->user)->actor;
  for (i = 0; i != MOTE_FOUNTAIN_INITIALIZED_COUNT; i++) {
    s32 speed;
    s32 angle;

    mote = &PARTICLES[i];

    speed = (Random16() & 0x3ff) + 32;
    angle = Random16() & TRIG_ANGLE_MASK;
    SET_POSITION(mote, source->x.fixed, source->y.fixed + px(5),
                 source->z.fixed);
    SET_VELOCITY(mote, Trig_Sin(angle) * speed >> 8,
                 ((Random16() & 255) - 32) << 9,
                 -(Trig_Cos(angle) * speed << 1) >> 8);
    mote->variant = (Random16() & 31) + 48;
    if (mode == MOTE_FOUNTAIN_FOUNTAIN_HEALING) {
      mote->velocityX /= 2;
      mote->velocityZ /= 2;
    }
  }

  for (frame = 0; frame != MOTE_FOUNTAIN_FOUNTAIN_FRAMES; frame++) {
    EFFECT_VIEW_FROM_CAMERA(camera);
    for (i = 0; i != MOTE_VISIBLE_COUNT; i++) {
      mote = &PARTICLES[i];
      if (frame >= i / 32 * 8 && mote->variant >= 0) {
        s32 size;

        // Motes wobble and grow as they near the camera.
        SET_VECTOR(worldPoint,
                   mote->x + Trig_Sin((i * 4 + mote->variant) << 10) * 16,
                   mote->y, mote->z);
        Projection_Point(worldPoint, &position);
        position.x >>= 1;
        if (position.depth <= 313)
          position.depth = 314;
        if (position.depth > 634)
          position.depth = 634;
        size = 6 - (position.depth - 314) / 64;
        EFFECT_DRAW_MOTE(draw[0], canvas, sheet, position.x, position.y, size);
        Particle_AdvanceWithGravity3D(mote, 62, px(1) / 64);
        // The drifting mode pushes the motes across the field.
        if (mode == MOTE_FOUNTAIN_DRIFTING) {
          if (source->x.fixed < 0)
            mote->velocityX += px(1) / 8;
          else
            mote->velocityX -= px(1) / 8;
        }
        mote->variant--;
      }
    }
    if (mode == MOTE_FOUNTAIN_DRIFTING) {
      EFFECT_FOR_EACH_TARGET(i, work->args) {
        if (frame == i * 8 + 48) {
          EFFECT_SHOW_DAMAGE(-1);
          BattleEffect_TintBattler(work->args->targets[i], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, i, 8);
        }
      }
    } else {
      EFFECT_FOR_EACH_TARGET(i, work->args) {
        if (frame == i * 8 + 48) {
          Sound_PlayFar(SE_RECOVERY);
          EFFECT_SHOW_DAMAGE(-1);
          BattleEffect_TintBattler(work->args->targets[i], SPRITE_COLOR_WHITE,
                                   TINT_KEEP, i, 8);
        }
      }
    }
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS();
}

#undef STREAKS_VALUE
#undef STREAKS_ANIMATE
#undef STREAKS_HIT
#undef STREAKS_MOTION
#undef STREAKS_SHAKE
#undef STREAKS_MAX
#undef STREAKS_SPACING
#undef STREAKS_FLIGHT
#undef STREAKS_TARGET_DELAY
#undef MOTE_FOUNTAIN_FOUNTAIN_FRAMES

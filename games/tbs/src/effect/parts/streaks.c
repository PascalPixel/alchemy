/// @file
/// Streak volleys (Sonic Wave, Shriek, War Cry ...) and mote fountains (Sleep
/// Star, Soothing Star).
// Part of effect.c.

#include "../effect_internal.h"

// Streak volleys: three streaks of light fly from the user to each target in
// turn, in one of five colours. The streaks are work->particles, one for each
// target and streak, and their variant counts the frames since they set off.

/// Column `n` of style `mode`'s row of gStreaks_Records, four flags a style.
#define STREAKS_VALUE(mode, n) gStreaks_Records[(mode)][(n)]
#define STREAKS_ANIMATE 0 ///< nonzero: the streak cycles through three images
#define STREAKS_HIT 1     ///< nonzero: play the hit sound and flash the target
#define STREAKS_MOTION                                                         \
  2 ///< the target's knockback on the hit (enum BattleKnockback, -1: none)
/// nonzero: the struck target trembles from side to side
#define STREAKS_SHAKE 3

#define STREAKS_MAX 3 ///< streaks fired at each target

/// The streak volley's styles: its colour and its row of gStreaks_Records.
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
/// Abilities: Sonic Wave (BATTLE_EFFECT_SONIC_WAVE).
void BattleEffect_RunSonicWave(struct BattleEffectArgument *effect) {
  BattleEffect_RunStreakVolley(effect, STREAKS_STYLE_EMBER);
}
/// Three streaks of lime light fly from the user at each target in turn; each
/// target flashes and recoils.
///
/// Abilities: Shriek (BATTLE_EFFECT_SHRIEK).
void BattleEffect_RunShriek(struct BattleEffectArgument *effect) {
  BattleEffect_RunStreakVolley(effect, STREAKS_STYLE_LIME);
}
/// Three streaks of violet light fly from the user at each target in turn; each
/// target flashes and trembles.
///
/// Abilities: Banshee Howl (BATTLE_EFFECT_BANSHEE_HOWL).
void BattleEffect_RunBansheeHowl(struct BattleEffectArgument *effect) {
  BattleEffect_RunStreakVolley(effect, STREAKS_STYLE_VIOLET);
}
/// Three flickering cyan streaks, cycling through three colour rows, fly from
/// the user at each target in turn; each target flashes and recoils.
///
/// Abilities: War Cry (BATTLE_EFFECT_WAR_CRY).
void BattleEffect_RunWarCry(struct BattleEffectArgument *effect) {
  BattleEffect_RunStreakVolley(effect, STREAKS_STYLE_CYAN);
}
/// Three flickering red streaks, cycling through three colour rows, fly from
/// the user at each target in turn; each target flashes and trembles.
///
/// Abilities: Wicked Howl (BATTLE_EFFECT_WICKED_HOWL).
void BattleEffect_RunWickedHowl(struct BattleEffectArgument *effect) {
  BattleEffect_RunStreakVolley(effect, STREAKS_STYLE_RED);
}
/// Unused: no entry of the effect table points here. It would have played the
/// same lime streaks as Shriek.
UNUSED void BattleEffect_RunLimeStreaks(struct BattleEffectArgument *effect) {
  BattleEffect_RunStreakVolley(effect, STREAKS_STYLE_LIME);
}

#define STREAKS_SPACING 6       ///< frames between streaks
#define STREAKS_FLIGHT 24       ///< frames from user to target
#define STREAKS_TARGET_DELAY 32 ///< frames between targets

/// Fires STREAKS_MAX streaks at each target from 20 pixels above the user,
/// one every 6 frames, each reaching 20 pixels above its target 24 frames
/// later; each target's volley starts 32 frames after the previous one's.
/// A streak image is 20x40 and drawn twice, mirrored upwards, as a 20x80
/// streak that leaves a fading trail.
///
/// Called by the five streak handlers above and the unused
/// BattleEffect_RunLimeStreaks. Loads the style's colours over the first 64 of
/// the background palette, writes work->bg2xLines and, over the last 16
/// frames, BLDALPHA; sets work->shakeFrames on a recoiling hit, and moves a
/// trembling target's x.
///
/// mode : the style, 0 to 4: the streak colour (ember, lime, violet, red,
///        cyan) and the row of gStreaks_Records that says how a hit looks.
void BattleEffect_RunStreakVolley(struct BattleEffectArgument *effect,
                                  s32 mode) {
  struct EffectPosition position;
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 k;
  DrawRectangle drawLower;
  DrawRectangle drawUpper;
  struct BattleCamera *camera;
  struct MotionObject *source;
  struct MotionObject *target;
  s32 resource;
  s32 frame;
  s32 streakIndex;
  s32 streaks = STREAKS_MAX;
  s32 lift = 20 << 16; // 20 pixels, 16.16

  EFFECT_GET_SLOTS(slots, cursor, work, canvas);
  camera = EFFECT_SLOT(slots, HEAP_SLOT_CAMERA);
  work->effect = effect;
  BattleEffect_BeginCanvasLayer(1);

  // The upper half of a streak is its image flipped vertically; on the
  // party's side both halves are mirrored as well.
  if (work->effect->side == 0) {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y, COMPOSITE_MAX);
    drawUpper = EFFECT_SLOT(slots, HEAP_SLOT_BLITTER);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE, EFFECT_CLIP_BOUNDS,
                        COMPOSITE_MAX);
    drawLower = EFFECT_SLOT(slots, HEAP_SLOT_BLITTER_ALTERNATE);
  } else {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP,
                        COMPOSITE_MAX);
    drawUpper = EFFECT_SLOT(slots, HEAP_SLOT_BLITTER);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_MAX);
    drawLower = EFFECT_SLOT(slots, HEAP_SLOT_BLITTER_ALTERNATE);
  }

  // The streak images, and the style's colour as the first 128 bytes of the
  // background palette. The sheet holds three colour rows of six images,
  // 4800 bytes a row and 800 (20x40) an image.
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
  // Halve the canvas each frame, so streaks leave a trail.
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 50);

  source = GetBattleObjectSlotFar(work->effect->actor)->object;
  {
    // XXX: the length counts 6 frames for every streak although targets are
    // 32 frames apart, so with four targets the fourth's hit (frame 120)
    // falls on the frame the loop stops at and never plays; the abilities'
    // target ranges would settle whether four targets can occur. With three,
    // the loop stops at 102, before the third target's second flash (104).
    s32 totalFrames = streaks * work->effect->count * STREAKS_SPACING + 48;

    // Every streak starts 20 pixels above the user and moves in a straight
    // line to 20 pixels above its target over 24 frames.
    EFFECT_FOR_EACH_TARGET(k, work->effect) {
      target = GetBattleObjectSlotFar(work->effect->actors[k])->object;
      for (streakIndex = 0; streakIndex != streaks; streakIndex++) {
        struct EffectStep *streak = &work->particles[k * streaks + streakIndex];

        SET_POSITION(streak, source->x, source->y + lift, source->z);
        SET_VELOCITY(streak, (target->x - streak->x) / STREAKS_FLIGHT,
                     (target->y + lift - streak->y) / STREAKS_FLIGHT,
                     (target->z - streak->z) / STREAKS_FLIGHT);
        streak->variant = 0; // frames since the streak set off
      }
    }

    for (frame = 0; frame != totalFrames; frame++) {
      s32 *row = work->bg2xLines;

      // A sine wave of BG2X offsets, one per scanline, for the wave
      // HBlank DMA.
      // XXX: nothing here arms BattleEffect_ArmBg2AffineHBlankDma, so the table
      // seems unread; a scheduler trace during Sonic Wave would settle it.
      for (k = 0; k != 160; k++)
        *row++ = (0x80000 - Trig_Sin((frame + k) << 12) * 2) >> 10;
      // Fade the canvas out over the last 16 frames.
      if (frame > totalFrames - 16)
        REG_BLDALPHA = BLEND_ALPHA(totalFrames - frame, 16);
      EFFECT_VIEW_FROM_CAMERA(camera);
      EFFECT_FOR_EACH_TARGET(k, work->effect) {
        if (frame >= k * STREAKS_TARGET_DELAY) {
          for (streakIndex = 0; streakIndex != streaks; streakIndex++) {
            if (frame >=
                k * STREAKS_TARGET_DELAY + streakIndex * STREAKS_SPACING) {
              struct EffectStep *streak =
                  &work->particles[k * streaks + streakIndex];
              s32 step;

              // Project the streak onto the half-width canvas; its image
              // changes every 8 frames and stays on the sixth.
              EffectPosition_ProjectPoint(streak, &position);
              position.x >>= 1;
              step = streak->variant / 8;
              if (step > 5)
                step = 5;
              if (STREAKS_VALUE(mode, STREAKS_ANIMATE)) {
                u8 *cell = &work->sheet[step * 800 + frame / 2 % 3 * 4800];

                drawUpper(canvas, cell, position.x - 10, position.y - 40, 20,
                          40);
                drawLower(canvas, cell, position.x - 10, position.y, 20, 40);
              } else {
                u8 *cell = &work->sheet[step * 800 + 2 * 4800];

                drawUpper(canvas, cell, position.x - 10, position.y - 40, 20,
                          40);
                drawLower(canvas, cell, position.x - 10, position.y, 20, 40);
              }
              // No drag and no gravity: a straight line.
              EffectStep_AdvanceWithGravity3D(streak, 64, 0);
              streak->variant++;
            }
          }

          // From 6 frames after the first streak hits, for 32 frames, the
          // target trembles from side to side. The tremble sums to nothing
          // over its 8 frames, but each frame also moves the target half a
          // pixel further from the middle (x 0), 16 pixels in all.
          if (STREAKS_VALUE(mode, STREAKS_SHAKE) &&
              frame >= k * STREAKS_TARGET_DELAY + 30 &&
              frame < k * STREAKS_TARGET_DELAY + 62) {
            struct MotionObject *struck =
                GetBattleObjectSlotFar(work->effect->actors[k])->object;

            struck->x +=
                gStreaks_Tremble[(frame - k * STREAKS_TARGET_DELAY - 30) % 8]
                << 16;
            if (struck->x > 0)
              struck->x += 0x8000;
            else
              struck->x -= 0x8000;
            ObjectGroup_UpdateMembers(work->effect->actors[k], -1, POSE_DAMAGE,
                                      -1, 0);
          }

          // The hit: the first streak plays the hit sound and flashes the
          // target white in its damage pose for 8 frames, and it
          // flashes again 16 frames later.
          if (STREAKS_VALUE(mode, STREAKS_HIT)) {
            if (frame == k * STREAKS_TARGET_DELAY + 24) {
              AudioCommand_PlayFar(SE_IMPACT);
              if (k == 0) // the first hit lets the battle show the damage
                EFFECT_SHOW_DAMAGE(-1);
              ObjectGroup_UpdateMembers(work->effect->actors[k],
                                        SPRITE_COLOR_WHITE, POSE_DAMAGE, k, 8);
            }
            if (frame == k * STREAKS_TARGET_DELAY + 40)
              ObjectGroup_UpdateMembers(work->effect->actors[k],
                                        SPRITE_COLOR_WHITE, POSE_DAMAGE, k, 8);
          }
          // The style's recoil motion, with a camera shake.
          if (STREAKS_VALUE(mode, STREAKS_MOTION) != -1 &&
              frame == k * STREAKS_TARGET_DELAY + 24) {
            work->shakeFrames = 4;
            BattleMotion_ApplyVariantMotionFar(
                work->effect->actors[k], STREAKS_VALUE(mode, STREAKS_MOTION));
          }
        }
      }
      Camera_ApplyShake(8, 8);
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

/// The fountain launches 256 motes in PARTICLES but draws only the first 128.
enum { MOTE_FOUNTAIN_INITIALIZED_COUNT = 256, MOTE_VISIBLE_COUNT = 128 };

/// Motes burst out of the user and float up, wobbling, as they drift across the
/// field onto the targets, and each target flinches in turn.
///
/// Abilities: Sleep Star (BATTLE_EFFECT_SLEEP_STAR).
void BattleEffect_RunSleepStar(struct BattleEffectArgument *effect) {
  BattleEffect_RunMoteFountain(effect, MOTE_FOUNTAIN_DRIFTING);
}
/// Orange healing motes burst out of the user and float up close to it, and
/// each target glows in turn with the recovery sound.
///
/// Abilities: Soothing Star (BATTLE_EFFECT_SOOTHING_STAR).
void BattleEffect_RunSoothingStar(struct BattleEffectArgument *effect) {
  BattleEffect_RunMoteFountain(effect, MOTE_FOUNTAIN_FOUNTAIN_HEALING);
}
#define MOTE_FOUNTAIN_FOUNTAIN_FRAMES 128

/// Motes burst out of the user in every direction and float upwards,
/// wobbling, for 128 frames; a new group of 32 joins every 8 frames. Each
/// target in turn reacts from frame 48, 8 frames apart.
///
/// Called by BattleEffect_RunSleepStar and BattleEffect_RunSoothingStar. Fills
/// PARTICLES 0 to 255 and loads the mode's colours over the first 64 of the
/// background palette; the healing mode opens the canvas layer with BG
/// control 0 and the drifting mode with 1.
///
/// mode: MOTE_FOUNTAIN_FOUNTAIN_HEALING throws orange motes that stay close to
///       the user, and each target plays the recovery sound and glows (a
///       healing effect); MOTE_FOUNTAIN_DRIFTING throws motes on the earth
///       palette that drift across towards the far side of the field, and
///       each target flinches (an attack).
void BattleEffect_RunMoteFountain(struct BattleEffectArgument *effect,
                                  s32 mode) {
  s32 worldPoint[3];
  struct EffectPosition position;
  struct BattleEffectWork *work;
  void *canvas;
  BlitterPair draw;
  struct BattleCamera *camera;
  u8 *sheet;
  struct MotionObject *source;
  struct EffectStep *mote;
  s32 resource;
  s32 frame;
  s32 i;

  work = WORK_SLOT(HEAP_SLOT_BATTLE_EFFECT);
  canvas = WORK_SLOT(HEAP_SLOT_BATTLE_CANVAS);
  camera = WORK_SLOT(HEAP_SLOT_CAMERA);
  sheet = WORK_SLOT(HEAP_SLOT_BATTLE_SHEET);
  work->effect = effect;
  if (mode == MOTE_FOUNTAIN_FOUNTAIN_HEALING)
    BattleEffect_BeginCanvasLayer(0);
  else
    BattleEffect_BeginCanvasLayer(1);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS, COMPOSITE_MAX);
  draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y, COMPOSITE_MAX);
  draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  if (mode == MOTE_FOUNTAIN_FOUNTAIN_HEALING)
    resource = (s32)&ResourceId_OrangePaletteA;
  else
    resource = (s32)&ResourceId_EarthWallSheet;
  Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(resource), 128);
  // Trails keep 3/4 a frame.
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);

  // Every mote starts 5 pixels above the user with a random direction and
  // speed, mostly upwards, and a life of 48 to 79 frames; the healing mode
  // halves the spread across the ground. In pixels a frame: up to 4 across,
  // up to 8 in depth, and -0.25 to 1.75 up.
  source = GetBattleObjectSlotFar(work->effect->actor)->object;
  for (i = 0; i != MOTE_FOUNTAIN_INITIALIZED_COUNT; i++) {
    s32 speed;
    s32 angle;

    mote = &PARTICLES[i];

    speed = 0x3ff;
    speed &= Random16();
    angle = Random16() & 0xffff;
    SET_POSITION(mote, source->x, source->y + 0x50000, source->z);
    SET_VELOCITY(mote, Trig_Sin(angle) * (speed + 32) >> 8,
                 ((Random16() & 255) - 32) << 9,
                 -(Trig_Cos(angle) * (speed + 32) << 1) >> 8);
    mote->variant = (Random16() & 31) + 48;
    if (mode == MOTE_FOUNTAIN_FOUNTAIN_HEALING) {
      mote->velocityX /= 2;
      mote->velocityZ /= 2;
    }
  }

  for (frame = 0; frame != MOTE_FOUNTAIN_FOUNTAIN_FRAMES; frame++) {
    EFFECT_VIEW_FROM_CAMERA(camera);
    // Mote i joins on frame i / 32 * 8, and goes out when its life is spent.
    for (i = 0; i != MOTE_VISIBLE_COUNT; i++) {
      mote = &PARTICLES[i];
      if (frame >= i / 32 * 8 && mote->variant >= 0) {
        s32 size;

        // Wobble up to 16 pixels sideways, project onto the half-width canvas
        // and draw a 1x2 to 6x12 mote, larger the nearer it is to the camera:
        // depths are held to 314 to 634, one size a 64 of depth.
        SET_VECTOR(worldPoint,
                   mote->x + Trig_Sin((i * 4 + mote->variant) << 10) * 16,
                   mote->y, mote->z);
        EffectPosition_ProjectPoint(worldPoint, &position);
        position.x >>= 1;
        if (position.depth <= 313)
          position.depth = 314;
        if (position.depth > 634)
          position.depth = 634;
        size = 6 - (position.depth - 314) / 64;
        EFFECT_DRAW_MOTE(draw[0], canvas, sheet, position.x, position.y, size);
        // Damped by 62/64 a frame, with a slight lift (y grows upwards) of
        // 1/64 pixel a frame each frame.
        EffectStep_AdvanceWithGravity3D(mote, 62, 0x400);
        // The drifting mode pushes every mote towards the other side of the
        // field, 1/8 pixel a frame faster each frame.
        if (mode == MOTE_FOUNTAIN_DRIFTING) {
          if (source->x < 0)
            mote->velocityX += 0x2000;
          else
            mote->velocityX -= 0x2000;
        }
        mote->variant--;
      }
    }
    // Each target reacts in turn, and the battle shows its result.
    if (mode == MOTE_FOUNTAIN_DRIFTING) {
      EFFECT_FOR_EACH_TARGET(i, work->effect) {
        if (frame == i * 8 + 48) {
          EFFECT_SHOW_DAMAGE(-1);
          ObjectGroup_UpdateMembers(work->effect->actors[i], SPRITE_COLOR_WHITE,
                                    POSE_DAMAGE, i, 8);
        }
      }
    } else {
      EFFECT_FOR_EACH_TARGET(i, work->effect) {
        if (frame == i * 8 + 48) {
          AudioCommand_PlayFar(SE_RECOVERY);
          EFFECT_SHOW_DAMAGE(-1);
          ObjectGroup_UpdateMembers(work->effect->actors[i], SPRITE_COLOR_WHITE,
                                    -1, i, 8);
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

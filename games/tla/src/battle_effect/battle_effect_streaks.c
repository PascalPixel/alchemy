/// @file
/// Streak volleys and mote fountains.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"
#include "gamepak/far_call/battle.h"
#include "resource_ids.h"

/// The streak volley's styles (BattleEffect_RunStreakVolley), each a row of
/// gStreaks_Styles.
enum StreakStyle {
  STREAKS_STYLE_EMBER,  ///< Sonic Wave
  STREAKS_STYLE_LIME,   ///< Shriek and Harpy, and the unused
                        ///< BattleEffect_RunLimeStreaks
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
/// Abilities: Shriek, Harpy.
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

/// Three flickering cyan streaks fly from the user at each target in turn;
/// each target flashes and recoils.
///
/// Abilities: War Cry.
void BattleEffect_RunWarCry(struct BattleEffectParams *args) {
  BattleEffect_RunStreakVolley(args, STREAKS_STYLE_CYAN);
}

/// Three flickering red streaks fly from the user at each target in turn; each
/// target flashes and trembles.
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

#define CLIP (COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y)

#define STREAKS_VALUE(mode, n) gStreaks_Styles[(mode) * 4 + (n)]
#define STREAKS_ANIMATE 0 ///< cycle the streak through its three pictures
#define STREAKS_HIT 1     ///< sound the hit and flash the target
#define STREAKS_MOTION 2  ///< the target's recoil motion, or -1
#define STREAKS_SHAKE 3   ///< tremble the target after the hit

#define STREAKS_MAX 3 ///< streaks for each target

// Sonic Wave (mode 0), Shriek (1), Banshee Howl (2), Wicked Howl (3) and War
// Cry (4): effect kinds 300 to 305.

#define STREAK_HEIGHT 40
#define STREAKS_TARGET_DELAY 32 ///< frames between targets

/// FiveModeImage's streaks: three animation frames, each six pictures that
/// grow; the lower half is the upper drawn flipped.
#define STREAK_WIDTH 20
#define STREAK_FRAME_BYTES (6 * PICTURE_BYTES(STREAK))
#define STREAKS_SPACING 6 ///< frames between streaks

/// Fires the streaks at each target in turn from above the user; each leaves a
/// fading trail.
void BattleEffect_RunStreakVolley(struct BattleEffectParams *args, s32 mode) {
  struct ScreenPoint position;
  struct HeapSlots *heap;
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
  s32 j;
  s32 streaks = STREAKS_MAX;
  s32 lift = toFIXED(20);

  /* FAKEMATCH(dead): heap set through its own address, so it is in memory
   * while the reads are expanded and the paired blitter's slot is reached
   * from its register; as heap = Ram_HeapSlots, 16 of 580 instructions differ
   * in tla-en. */
  *&heap = Ram_HeapSlots;
  work = (struct BattleEffectWork *)heap->battleEffect;
  canvas = heap->battleCanvas;
  camera = heap->cameraWork;
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
  Resource_LoadAndDecompress(RESOURCE_FIVE_MODE_IMAGE, work->sheet, 0, 0);
  switch (mode) {
  case 0:
    resource = RESOURCE_EMBER_STREAK_SHEET;
    break;
  case 1:
    resource = RESOURCE_LIME_PALETTE;
    break;
  case 2:
    resource = RESOURCE_VIOLET_PALETTE_D;
    break;
  case 3:
    resource = RESOURCE_RED_PALETTE_B;
    break;
  case 4:
  default:
    resource = RESOURCE_CYAN_PALETTE;
    break;
  }
  Iwram_CopyWordsEntry((void *)BG_PLTT, Resource_GetTableEntry(resource),
                       SHEET_PALETTE_BYTES);
  work->transferMode =
      CANVAS_COPY_AND_FADE; // each transfer halves the old picture,
  work->transferValue = CANVAS_FADE_HALF; // leaving a trail
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK);

  source = BattleObject_GetFar(args->user)->object;
  {
    s32 total = streaks * args->targetCount * STREAKS_SPACING + 48;

    // Each streak flies from 20 pixels above the user to 20 pixels above
    // its target in 24 frames.
    for (k = 0; k != args->targetCount; k++) {
      target = BattleObject_GetFar(args->targets[k])->object;
      for (j = 0; j != streaks; j++) {
        struct Particle *streak = &work->particles[k * streaks + j];

        SET_POSITION(streak, source->x, source->y + lift, source->z);
        streak->velocityX = (target->x - streak->x) / 24;
        streak->velocityY = (target->y + lift - streak->y) / 24;
        streak->velocityZ = (target->z - streak->z) / 24;
        streak->variant = 0; // frames since it set out
      }
    }

    for (frame = 0; frame != total; frame++) {
      s32 *row = work->bg2xLines;

      // Wobble BG2's 160 lines on a sine wave, and fade the last 16
      // frames out.
      for (k = 0; k != DISPLAY_HEIGHT; k++)
        *row++ = (toFIXED(8) - Trig_Sin((frame + k) << 12) * 2) >> 10;
      if (frame > total - 16)
        REG_BLDALPHA = BLEND_ALPHA(total - frame, BLEND_WEIGHT_MAX);

      Matrix_Initialize();
      Matrix_SetLookAt(camera->eye, camera->focus);
      for (k = 0; k != args->targetCount; k++) {
        // Each target's streaks set out with the mode's cry.
        if (frame == k * STREAKS_TARGET_DELAY) {
          if (mode == 1)
            Sound_PlayFar(SE_SHRIEK);
          if (mode == 2)
            Sound_PlayFar(SE_STONE_GRIND);
          if (mode == 3)
            Sound_PlayFar(SE_BOSS_ROAR);
          if (mode == 4)
            Sound_PlayFar(SE_ITEM_BREAK);
        }
        if (frame >= k * STREAKS_TARGET_DELAY) {
          for (j = 0; j != streaks; j++) {
            // lift is reused for the streak's start frame; a local of its own
            // differs.
            lift = k * STREAKS_TARGET_DELAY + j * 6;
            if (frame >= lift) {
              struct Particle *streak = &work->particles[k * streaks + j];
              s32 step;

              // The picture changes every 8 frames, stopping at the sixth.
              EffectPosition_ProjectPoint((s32 *)streak, &position);
              position.x >>= 1;
              step = streak->variant / 8;
              if (step > 5)
                step = 5;
              if (STREAKS_VALUE(mode, STREAKS_ANIMATE)) {
                u8 *cell = &work->sheet[step * 800 +
                                        frame / 2 % 3 * STREAK_FRAME_BYTES];

                drawUpper(canvas, cell, position.x - 10,
                          position.y - STREAK_HEIGHT, STREAK_WIDTH,
                          STREAK_HEIGHT);
                drawLower(canvas, cell, position.x - 10, position.y,
                          STREAK_WIDTH, STREAK_HEIGHT);
              } else {
                u8 *cell = &work->sheet[step * 800 + 2 * STREAK_FRAME_BYTES];

                drawUpper(canvas, cell, position.x - 10,
                          position.y - STREAK_HEIGHT, STREAK_WIDTH,
                          STREAK_HEIGHT);
                drawLower(canvas, cell, position.x - 10, position.y,
                          STREAK_WIDTH, STREAK_HEIGHT);
              }
              Particle_AdvanceWithGravity3D(streak, PARTICLE_DAMPING_NONE, 0);
              streak->variant++;
            }
          }

          // From 6 frames after the hit, tremble the target sideways for
          // 32 frames.
          if (STREAKS_VALUE(mode, STREAKS_SHAKE) &&
              frame >= k * STREAKS_TARGET_DELAY + 30 &&
              frame < k * STREAKS_TARGET_DELAY + 62) {
            struct MotionObject *struck =
                BattleObject_GetFar(args->targets[k])->object;

            struck->x += INTtoFIXED(
                gStreaks_Tremble[(frame - k * STREAKS_TARGET_DELAY - 30) % 8]);
            if (struck->x > 0)
              struck->x += toFIXED(1) / 2;
            else
              struck->x -= toFIXED(1) / 2;
            BattleEffect_TintBattler(args->targets[k], TINT_KEEP, POSE_DAMAGE,
                                     TINT_KEEP, 0);
          }

          // The hit: a sound and two flashes, 16 frames apart. The first
          // hit lets the battle show its results.
          if (STREAKS_VALUE(mode, STREAKS_HIT)) {
            if (frame == k * STREAKS_TARGET_DELAY + 24) {
              Sound_PlayFar(SE_IMPACT);
              if (k == 0)
                EFFECT_SHOW_DAMAGE(-1);
              BattleEffect_TintBattler(args->targets[k], SPRITE_COLOR_WHITE,
                                       POSE_DAMAGE, k, 8);
            }
            if (frame == k * STREAKS_TARGET_DELAY + 40)
              BattleEffect_TintBattler(args->targets[k], SPRITE_COLOR_WHITE,
                                       POSE_DAMAGE, k, 8);
          }
          if (STREAKS_VALUE(mode, STREAKS_MOTION) != -1 &&
              frame == k * STREAKS_TARGET_DELAY + 24) {
            work->shakeFrames = 4;
            BattleMotion_ApplyKnockbackFar(args->targets[k],
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
  MOTE_FOUNTAIN_DRIFTING,         ///< Sleep Star
  MOTE_FOUNTAIN_SLAVER            ///< Slaver
};

/// Abilities: Sleep Star.
void BattleEffect_RunSleepStar(struct BattleEffectParams *args) {
  BattleEffect_RunMoteFountain(args, MOTE_FOUNTAIN_DRIFTING);
}

/// Abilities: Soothing Star.
void BattleEffect_RunSoothingStar(struct BattleEffectParams *args) {
  BattleEffect_RunMoteFountain(args, MOTE_FOUNTAIN_FOUNTAIN_HEALING);
}

/// Abilities: Slaver.
void BattleEffect_RunSlaver(struct BattleEffectParams *args) {
  BattleEffect_RunMoteFountain(args, MOTE_FOUNTAIN_SLAVER);
}

/// The motes live in the map cell buffer.
#define EFFECT_STEPS ((struct Particle *)Ram_MapCellBuffer)

#define MOTE_FOUNTAIN_FOUNTAIN_FRAMES 128

/// Motes burst out of the user and float upwards, wobbling, and each target
/// reacts in turn.
void BattleEffect_RunMoteFountain(struct BattleEffectParams *args, s32 mode) {
  FIXED point[3];
  struct ScreenPoint position;
  struct BattleEffectWork *work;
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
  s32 group = 32; // motes that start together

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  camera = Ram_HeapSlots->cameraWork;
  sheet = Ram_HeapSlots->battleSheet;
  if (mode == MOTE_FOUNTAIN_FOUNTAIN_HEALING)
    BattleEffect_StartCanvasLayer(0);
  else
    BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));

  // Motes draw at their maximum; the second blitter, mirrored top to
  // bottom, is loaded but not used.
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y, BLITTER_MAX);
  draw[1] = (DrawRectangle)Ram_HeapSlots->ramCodePaired;
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, sheet, 0, 0);
  if (mode == MOTE_FOUNTAIN_FOUNTAIN_HEALING)
    resource = RESOURCE_ORANGE_PALETTE_A;
  else if (mode == MOTE_FOUNTAIN_DRIFTING)
    resource = RESOURCE_EARTH_WALL_SHEET;
  else
    resource = RESOURCE_GREEN_PALETTE;
  Iwram_CopyWordsEntry((void *)BG_PLTT, Resource_GetTableEntry(resource),
                       SHEET_PALETTE_BYTES);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

  // Launch every mote from the user in 16.16 fixed point, with a speed and
  // a direction of its own. Slaver throws them up, faster, in a half-circle
  // fan turned by the user's side, from up to 16 pixels higher.
  source = BattleObject_GetFar(args->user)->object;
  for (i = 0; i != 256; i++) {
    s32 speed;
    s32 angle;

    mote = &EFFECT_STEPS[i];
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
    if (mode == MOTE_FOUNTAIN_FOUNTAIN_HEALING) {
      mote->velocityX /= 2;
      mote->velocityZ /= 2;
    }
  }

  frames = 96;
  if (mode != MOTE_FOUNTAIN_SLAVER)
    frames = 128;
  for (frame = 0; frame != frames; frame++) {
    Matrix_Initialize();
    Matrix_SetLookAt(camera->eye, camera->focus);
    if (mode == MOTE_FOUNTAIN_SLAVER && frame == 0)
      Sound_PlayFar(SE_CURSE);

    for (i = 0; i != MOTE_FOUNTAIN_FOUNTAIN_FRAMES; i++) {
      mote = &EFFECT_STEPS[i];
      if (frame >= i / group * 8 && mote->variant >= 0) {
        s32 size;

        // Sway the mote, project it and size it by its depth: 6 pixels
        // near, 1 far.
        point[0] = mote->x + Trig_Sin((i * 4 + mote->variant) << 10) * 16;
        point[1] = mote->y;
        point[2] = mote->z;
        EffectPosition_ProjectPoint(point, &position);
        position.x >>= 1;
        if (position.depth <= 313)
          position.depth = 314;
        if (position.depth > 634)
          position.depth = 634;
        size = 6 - (position.depth - 314) / 64;
        draw[0](canvas, sheet + gBattleEffect_MotePictures[size - 1],
                position.x - size / 2, position.y - size, size, size * 2);

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
          EFFECT_SHOW_DAMAGE(-1);
          BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, i, 8);
        }
      }
    } else if (mode == MOTE_FOUNTAIN_FOUNTAIN_HEALING) {
      for (i = 0; i != args->targetCount; i++) {
        if (frame == i * 8 + 48) {
          Sound_PlayFar(SE_RECOVERY);
          EFFECT_SHOW_DAMAGE(-1);
          BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                   TINT_KEEP, i, 8);
        }
      }
    }
    EFFECT_END_FRAME(work);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  EFFECT_RELEASE_BLITTERS();
  if (mode == MOTE_FOUNTAIN_SLAVER) {
    // Clear the canvas and run Impair's effect on the targets.
    Iwram_ClearWords((void *)BG_CHAR_BLOCK(1), CANVAS_VIEW_BYTES);
    Iwram_ClearWords(canvas, CANVAS_VIEW_BYTES);
    Task_Remove(Palette_StepFadeTransfer);
    BattleEffect_RunImpair(args);
  } else {
    BattleEffect_EndCanvasLayer();
  }
}
#undef EFFECT_STEPS

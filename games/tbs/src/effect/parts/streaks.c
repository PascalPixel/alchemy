// Part of effect.c, the battle effect object, compiled only through it.
// Streak volleys (Sonic Wave, Shriek, War Cry ...) and mote fountains (Sleep
// Star, Soothing Star).

#include "../effect_internal.h"

// Streak volleys: three streaks of light fly from the user to each target in
// turn, in one of five colours.

// gStreaks_Records has four flags for each style.
#define STREAKS_VALUE(mode, n) gStreaks_Records[(mode)][(n)]
#define STREAKS_ANIMATE 0 // nonzero: the streak cycles through three images
#define STREAKS_HIT 1     // nonzero: play the hit sound and flash the target
#define STREAKS_MOTION 2  // the target's recoil motion on the hit (-1: none)
#define STREAKS_SHAKE 3 // nonzero: the struck target trembles from side to side

#define STREAKS_MAX 3 // streaks fired at each target

enum StreakStyle {
  STREAKS_STYLE_EMBER,
  STREAKS_STYLE_LIME,
  STREAKS_STYLE_VIOLET,
  STREAKS_STYLE_RED,
  STREAKS_STYLE_CYAN
};

// Abilities: Sonic Wave (kind 300).
//
// Ember streaks. Three streaks of ember light fly from the user at each target
// in turn; each target flashes and recoils.
void BattleFx_RunSonicWave(struct BattleEffectArgument *effect) {
  BattleFx_RunStreakVolley(effect, STREAKS_STYLE_EMBER);
}
// Abilities: Shriek (kind 301).
//
// Lime streaks. Three streaks of lime light fly from the user at each target in
// turn; each target flashes and recoils.
void BattleFx_RunShriek(struct BattleEffectArgument *effect) {
  BattleFx_RunStreakVolley(effect, STREAKS_STYLE_LIME);
}
// Abilities: Banshee Howl (kind 302).
//
// Violet streaks. Three streaks of violet light fly from the user at each
// target in turn; each target flashes and trembles.
void BattleFx_RunBansheeHowl(struct BattleEffectArgument *effect) {
  BattleFx_RunStreakVolley(effect, STREAKS_STYLE_VIOLET);
}
// Abilities: War Cry (kind 304).
//
// Cyan streaks. Three flickering cyan streaks, cycling through three colour
// rows, fly from the user at each target in turn; each target flashes and
// recoils.
void BattleFx_RunWarCry(struct BattleEffectArgument *effect) {
  BattleFx_RunStreakVolley(effect, STREAKS_STYLE_CYAN);
}
// Abilities: Wicked Howl (kind 305).
//
// Red streaks. Three flickering red streaks, cycling through three colour rows,
// fly from the user at each target in turn; each target flashes and trembles.
void BattleFx_RunWickedHowl(struct BattleEffectArgument *effect) {
  BattleFx_RunStreakVolley(effect, STREAKS_STYLE_RED);
}
// The streak volley in lime. UNUSED: no effect table entry points here.
UNUSED void BattleFx_RunLimeStreaks(struct BattleEffectArgument *effect) {
  BattleFx_RunStreakVolley(effect, STREAKS_STYLE_LIME);
}

#define STREAKS_SPACING 6       // frames between streaks
#define STREAKS_FLIGHT 24       // frames from user to target
#define STREAKS_TARGET_DELAY 32 // frames between targets

// Fires STREAKS_MAX streaks at each target from 20 pixels above the user,
// one every 6 frames, each reaching 20 pixels above its target 24 frames
// later; each target's volley starts 32 frames after the previous one's.
// A streak image is 20x40 and drawn twice, mirrored upwards, as a 20x80
// streak that wavers from side to side and leaves a fading trail.
// mode : the style, 0 to 4: the streak colour (ember, lime, violet, red,
//        cyan) and the row of gStreaks_Records that says how a hit looks.
void BattleFx_RunStreakVolley(struct BattleEffectArgument *effect, s32 mode) {
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
  BattleFx_BeginCanvasLayer(1);

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

  // The streak images, and the background palette for the style's colour.
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
      // HBlank DMA; this effect does not start that DMA itself.
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
              EffectStep_AdvanceWithGravity3D(streak, 64, 0);
              streak->variant++;
            }
          }

          // From 6 frames after the first streak hits, for 32 frames, the
          // target trembles from side to side.
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
              AudioCommand_PlayFar(SE_133);
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

enum MoteFountainMode {
  MOTE_FOUNTAIN_FOUNTAIN_HEALING,
  MOTE_FOUNTAIN_DRIFTING
};
enum { MOTE_FOUNTAIN_INITIALIZED_COUNT = 256, MOTE_VISIBLE_COUNT = 128 };

// Abilities: Sleep Star (kind 349).
//
// Rising motes that drift onto the targets and strike.
// Motes burst out of the user and float up, wobbling, as they drift across the
// field; each target flinches in turn.
void BattleFx_RunSleepStar(struct BattleEffectArgument *effect) {
  BattleFx_RunMoteFountain(effect, MOTE_FOUNTAIN_DRIFTING);
}
// Abilities: Soothing Star (kind 344).
//
// Rising healing motes. Orange motes burst out of the
// user and float up close to it, and each target glows in turn with the
// recovery sound.
void BattleFx_RunSoothingStar(struct BattleEffectArgument *effect) {
  BattleFx_RunMoteFountain(effect, MOTE_FOUNTAIN_FOUNTAIN_HEALING);
}
#define MOTE_FOUNTAIN_FOUNTAIN_FRAMES 128

// Motes burst out of the user in every direction and float upwards,
// wobbling, for 128 frames; a new group of 32 joins every 8 frames. Each
// target in turn reacts from frame 48, 8 frames apart.
// mode 0: orange motes that stay close to the user; each target plays the
//         recovery sound and glows (a healing effect).
// mode 1: motes on the earth palette that drift across towards the far side
//         of the field; each target flinches (an attack).
void BattleFx_RunMoteFountain(struct BattleEffectArgument *effect, s32 mode) {
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
    BattleFx_BeginCanvasLayer(0);
  else
    BattleFx_BeginCanvasLayer(1);
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
  // speed, mostly upwards, and a life of 48 to 79 frames. Only the first 128
  // are ever drawn.
  source = GetBattleObjectSlotFar(work->effect->actor)->object;
  for (i = 0; i != MOTE_FOUNTAIN_INITIALIZED_COUNT; i++) {
    s32 speed;
    s32 angle;

    mote = &PARTICLES[i];

    /* FAKEMATCH: the mask set before the Random16() call it masks; as
     * Random16() & 0x3ff, 5 lines of tbs-en effect.s differ (2026-10-18). */
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
    for (i = 0; i != MOTE_VISIBLE_COUNT; i++) {
      mote = &PARTICLES[i];
      if (frame >= i / 32 * 8 && mote->variant >= 0) {
        s32 size;

        // Wobble sideways, project onto the half-width canvas and draw a
        // 1x2 to 6x12 mote, larger the nearer it is to the camera.
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
        EffectStep_AdvanceWithGravity3D(mote, 62, 0x400);
        // Mode 1 pushes every mote towards the other side of the field.
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

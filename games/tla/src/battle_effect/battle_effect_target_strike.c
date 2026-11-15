/// @file
/// Monster bites and slashes on one target, and Mad Zephyr.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"
#include "gamepak/far_call/battle.h"
#include "system/system.h"
#include "resource_ids.h"

/// One slash strikes the first target among glints, in lightning colours.
///
/// Abilities: Numbing Sting, Paralytail.
void BattleEffect_RunNumbingSting(struct BattleEffectParams *args) {
  BattleEffect_RunTargetStrike(args, TARGET_STRIKE_STYLE_LIGHTNING_GLINT_SLASH);
}

/// Fangs bite the first target three times while motes stream out of it and
/// back into the user.
///
/// Abilities: Cannibal Fang.
void BattleEffect_RunCannibalFang(struct BattleEffectParams *args) {
  BattleEffect_RunTargetStrike(args, TARGET_STRIKE_STYLE_EMBER_DRAIN_BITES);
}

/// Fangs bite the first target once, in lime, with smoke.
///
/// Abilities: Poisonous Bite; Poison Fang.
void BattleEffect_RunPoisonousBite(struct BattleEffectParams *args) {
  BattleEffect_RunTargetStrike(args, TARGET_STRIKE_STYLE_LIME_SMOKE_BITE);
}

/// Fangs bite the first target once, with smoke, in Mars Djinn colours.
///
/// Abilities: Rabid Fang.
void BattleEffect_RunRabidFang(struct BattleEffectParams *args) {
  BattleEffect_RunTargetStrike(args, TARGET_STRIKE_STYLE_MARS_SMOKE_BITE);
}

/// Fangs bite the first target three times among glints, in lightning
/// colours.
///
/// Abilities: Electric Bite.
void BattleEffect_RunElectricBite(struct BattleEffectParams *args) {
  BattleEffect_RunTargetStrike(args, TARGET_STRIKE_STYLE_LIGHTNING_GLINT_BITES);
}

/// One lime slash strikes the first target, with smoke.
///
/// Abilities: Poison Tail; Poison Sting.
void BattleEffect_RunPoisonTail(struct BattleEffectParams *args) {
  BattleEffect_RunTargetStrike(args, TARGET_STRIKE_STYLE_LIME_SMOKE_SLASH);
}

/// Fangs bite the first target three times, draining motes back into the user,
/// in Mars Djinn colours.
///
/// Abilities: Vampiric Fang.
void BattleEffect_RunVampiricFang(struct BattleEffectParams *args) {
  BattleEffect_RunTargetStrike(args, TARGET_STRIKE_STYLE_MARS_DRAIN_BITES);
}

/// Fangs bite the first target twice, in blue, with sparks.
///
/// Abilities: Double Fang.
void BattleEffect_RunDoubleFang(struct BattleEffectParams *args) {
  BattleEffect_RunTargetStrike(args, TARGET_STRIKE_STYLE_BLUE_SPARK_BITES);
}

/// One heavy slash strikes the first target in Mars Djinn colours, throwing
/// sparks.
///
/// Abilities: Mortal Blow.
void BattleEffect_RunMortalBlow(struct BattleEffectParams *args) {
  BattleEffect_RunTargetStrike(args, TARGET_STRIKE_STYLE_MARS_HEAVY_SLASH);
}

/// Two slashes strike the first target, throwing sparks, in lightning colours.
///
/// Abilities: Twin Beaks, Gryphon.
void BattleEffect_RunTwinBeaks(struct BattleEffectParams *args) {
  BattleEffect_RunTargetStrike(args,
                               TARGET_STRIKE_STYLE_LIGHTNING_SPARK_SLASHES);
}

/// Fangs bite the first target three times, in pink, with smoke.
///
/// Abilities: Acid Bite.
void BattleEffect_RunAcidBite(struct BattleEffectParams *args) {
  BattleEffect_RunTargetStrike(args, TARGET_STRIKE_STYLE_PINK_SMOKE_BITES);
}

/// Fangs bite the first target three times, draining motes back into the user,
/// added in Mars Djinn colours.
///
/// Abilities: Drain Fang.
void BattleEffect_RunDrainFang(struct BattleEffectParams *args) {
  BattleEffect_RunTargetStrike(args,
                               TARGET_STRIKE_STYLE_MARS_ADDITIVE_DRAIN_BITES);
}

/// One orange slash strikes the first target with a flash and sparks.
///
/// Abilities: Angle Spike.
void BattleEffect_RunAngleSpike(struct BattleEffectParams *args) {
  BattleEffect_RunTargetStrike(args,
                               TARGET_STRIKE_STYLE_ORANGE_FLASH_SPARK_SLASH);
}

/// One orange slash strikes the first target with a flash, glints and sparks.
///
/// Abilities: Heat Stun.
void BattleEffect_RunHeatStun(struct BattleEffectParams *args) {
  BattleEffect_RunTargetStrike(args,
                               TARGET_STRIKE_STYLE_ORANGE_FLASH_GLINT_SLASH);
}

/// Fangs bite the first target three times, draining motes back into the user,
/// added in Mars Djinn colours.
///
/// Abilities: Maneater.
void BattleEffect_RunManeater(struct BattleEffectParams *args) {
  BattleEffect_RunTargetStrike(args, TARGET_STRIKE_STYLE_MANEATER);
}

/// Unused: no ability's effect names kind 438, so the player never sees it:
/// fangs bite the first target once, hard, throwing sparks, in Mars Djinn
/// colours.
void BattleEffect_RunUnusedHeavyBite(struct BattleEffectParams *args) {
  BattleEffect_RunTargetStrike(args, TARGET_STRIKE_STYLE_MARS_HEAVY_BITE);
}

/// Unused: no ability's effect names kind 439, so the player never sees it:
/// fangs bite the first target once with a flash and sparks, in orange.
void BattleEffect_RunUnusedFlashBite(struct BattleEffectParams *args) {
  BattleEffect_RunTargetStrike(args, TARGET_STRIKE_STYLE_ORANGE_FLASH_BITE);
}

/// One slash strikes the first target while a star swells on it, in Mars Djinn
/// colours.
///
/// Abilities: Kill Sting.
void BattleEffect_RunKillSting(struct BattleEffectParams *args) {
  BattleEffect_RunTargetStrike(args, TARGET_STRIKE_STYLE_MARS_STAR_SLASH);
}

/// Unused: no ability's effect names kind 456, so the player never sees it:
/// fangs bite the first target once, in blue, with sparks.
void BattleEffect_RunUnusedSparkBite(struct BattleEffectParams *args) {
  BattleEffect_RunTargetStrike(args, TARGET_STRIKE_STYLE_BLUE_SPARK_BITE);
}

/// Abilities: Mad Zephyr.
void BattleEffect_RunMadZephyr(struct BattleEffectParams *args) {
  BattleEffect_SetApproachMotion(args->user, args->targets[0], 4, 20);
  wait(2);
  if (args->variant == EFFECT_VARIANT_BASIC)
    BattleEffect_RunTargetStrike(
        args, TARGET_STRIKE_STYLE_LIGHTNING_FOUR_SPARK_SLASHES);
  else
    BattleEffect_RunTargetStrike(
        args, TARGET_STRIKE_STYLE_LIGHTNING_EIGHT_SPARK_SLASHES);
}

#undef RECORD

#define gBlitter ((DrawRectangle)Ram_HeapSlots->ramCode)
#define gBlitterAlternate ((DrawRectangle)Ram_HeapSlots->ramCodePaired)

#define RECORD(mode, field) gTargetStrike_Styles[(mode)][(field)]

/// The motes live in the map cell buffer, the sparks 0x3800 bytes on.
#define gMotes ((struct Particle *)Ram_MapCellBuffer)
#define gSparks ((struct Particle *)(Ram_MapCellBuffer + 0x3800))

/// The motes in PARTICLES, 128 for each target, then the strike sparks.
#define TARGET_STRIKE_MOTES_PER_TARGET 128
/// The smoke pictures in the sheet: six 24x48 puffs.
#define TARGET_STRIKE_SMOKE_PICTURES 12800
/// The bite's stages after the tornado sheet: BeamImage for a slash,
/// TwelveModeImage otherwise.
#define TARGET_STRIKE_BITE_PICTURES 3200

#define TARGET_STRIKE_BITE_WIDTH 18
#define TARGET_STRIKE_BITE_HEIGHT 48

/// Strikes the first target with slashes or a bite of closing fangs, with the
/// extras its flags choose. Each hit flashes and shakes the target and the
/// battle moves on at the first; the style (enum TargetStrikeStyle) adds smoke,
/// motes that stream back to the user, glints, sparks, a flash or a star.
void BattleEffect_RunTargetStrike(struct BattleEffectParams *args, s32 mode) {
  struct ScreenPoint ground;
  struct ScreenPoint targetPosition;
  struct ScreenPoint alternate;
  struct ScreenPoint center;
  struct ScreenPoint position;
  BlitterPair draw;
  struct ModelTexture texture;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 k;
  u8 *sheet;
  struct BattleCamera *camera;
  struct MotionObject *target;
  struct MotionObject *source;
  s32 resource;
  s32 i;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  sheet = Ram_HeapSlots->battleSheet;
  camera = Ram_HeapSlots->cameraWork;
  if (mode == TARGET_STRIKE_STYLE_MARS_HEAVY_SLASH)
    BattleEffect_StartCanvasLayer(0);
  else
    BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  if (mode == TARGET_STRIKE_STYLE_MARS_HEAVY_SLASH) {
    work->fadeFrames = BACKGROUND_DARKEN_FRAMES;
    work->fadeStep = 0;
  }

  // The sheets: motes, the tornado, the strikes or columns, the mode's
  // colours and the wisps (smoke, flash or star).
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, work->sheet, 1, 0);
  if (RECORD(mode, TARGET_STRIKE_KIND) == STRIKE_KIND_SLASH)
    Resource_LoadAndDecompress((s32)&ResourceId_BeamImage,
                               work->sheet + TARGET_STRIKE_BITE_PICTURES, 0, 0);
  else
    Resource_LoadAndDecompress((s32)&ResourceId_TwelveModeImage,
                               work->sheet + TARGET_STRIKE_BITE_PICTURES, 0, 0);
  switch (RECORD(mode, TARGET_STRIKE_COLOR)) {
  case STRIKE_COLOR_BLUE:
    resource = (s32)&ResourceId_BlueBeamSheet;
    break;
  case STRIKE_COLOR_LIME:
    resource = (s32)&ResourceId_LimePalette;
    break;
  case STRIKE_COLOR_PINK:
    resource = (s32)&ResourceId_PinkPalette;
    break;
  case STRIKE_COLOR_EMBER:
    resource = (s32)&ResourceId_EmberStreakSheet;
    break;
  case STRIKE_COLOR_MARS:
    resource = (s32)&ResourceId_MarsDjinnSheet;
    break;
  case STRIKE_COLOR_LIGHTNING:
  default:
    resource = (s32)&ResourceId_LightningBoltSheet;
    break;
  case STRIKE_COLOR_ORANGE:
    resource = (s32)&ResourceId_OrangePaletteB;
    break;
  }
  Iwram_CopyWordsEntry((void *)BG_PLTT, Resource_GetTableEntry(resource),
                       SHEET_PALETTE_BYTES);
  if (RECORD(mode, TARGET_STRIKE_FLAGS) & STRIKE_FLASH)
    Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                               work->sheet + TARGET_STRIKE_SMOKE_PICTURES, 1,
                               0);
  else if (RECORD(mode, TARGET_STRIKE_FLAGS) & STRIKE_STAR)
    Resource_LoadAndDecompress((s32)&ResourceId_StarCrossSheet,
                               work->sheet + TARGET_STRIKE_SMOKE_PICTURES, 1,
                               0);
  else
    Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet,
                               work->sheet + TARGET_STRIKE_SMOKE_PICTURES, 1,
                               0);

  for (k = 0; k != 1; k++) {
    // The wisps start at the target's feet, waiting (-1) until a hit.
    target = BattleObject_GetFar(args->targets[k])->object;
    EffectPosition_ProjectBattler(args->targets[k], &ground);
    for (i = 0; i != ARRAY_COUNT(work->particles); i++) {
      struct Particle *step = &work->particles[i];

      SET_POSITION(step, INTtoFIXED(ground.x / 2), toFIXED(80), 0);
      step->velocityX = ((Random16() & 255) - 128) << 9;
      step->velocityY = ((Random16() & 255) - 128) << 9;
      step->velocityZ = 0;
      step->variant = -1;
    }

    // The motes burst from 20 pixels above the target.
    for (i = 0; i != TARGET_STRIKE_MOTES_PER_TARGET; i++) {
      struct Particle *step = &gMotes[k * TARGET_STRIKE_MOTES_PER_TARGET + i];

      SET_POSITION(step, target->x, toFIXED(20), target->z);
      step->velocityX = ((Random16() & 255) - 128) << 11;
      step->velocityY = ((Random16() & 255) - 128) << 11;
      step->velocityZ = ((Random16() & 255) - 128) << 11;
      step->variant = -1;
    }

    // The sparks fly away from the hit: heavy hits throw them in a fan
    // away from the user, the others towards the middle of the field.
    for (i = 0; i != 256; i++) {
      SET_POSITION(&gSparks[i], target->x, toFIXED(20), target->z);
      if (RECORD(mode, TARGET_STRIKE_FLAGS) & STRIKE_HEAVY) {
        s32 angle = Random16();
        s32 speed = Random16() & 255;

        gSparks[i].velocityX = (Random16() & 0x1ff) << 11;
        if (args->side == EFFECT_SIDE_PARTY)
          gSparks[i].velocityX = -gSparks[i].velocityX;
        gSparks[i].velocityY = Trig_Sin(angle) * speed >> 4;
        gSparks[i].velocityZ = Trig_Cos(angle) * speed >> 4;
      } else {
        if (RECORD(mode, TARGET_STRIKE_KIND) == STRIKE_KIND_BITE) {
          gSparks[i].velocityX = ((Random16() & 255) - 128) << 11;
          gSparks[i].velocityY = ((Random16() & 255) - 128) << 11;
          gSparks[i].velocityZ = ((Random16() & 255) - 128) << 11;
        } else {
          gSparks[i].velocityX = (Random16() & 255) << 11;
          gSparks[i].velocityY = ((Random16() & 255) - 128) << 11;
          gSparks[i].velocityZ = ((Random16() & 255) - 128) << 11;
        }
        if (gSparks[i].x > 0)
          gSparks[i].velocityX = -gSparks[i].velocityX;
      }
      gSparks[i].variant = -1;
    }
  }

  work->transferMode = CANVAS_COPY_AND_FADE;
  if (RECORD(mode, TARGET_STRIKE_FLAGS) & STRIKE_STAR)
    work->transferValue = CANVAS_FADE_HALF;
  else
    work->transferValue = CANVAS_FADE_THREE_QUARTERS;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  // Aim at the middle of the first target.
  EffectPosition_ProjectScaledBattler(args->targets[0], &targetPosition);
  EffectPosition_ProjectBattler(args->targets[0], &alternate);
  targetPosition.y += (alternate.y - targetPosition.y) / 2;

  for (frame = 0; frame != RECORD(mode, TARGET_STRIKE_FRAMES); frame++) {
    s32 span = RECORD(mode, TARGET_STRIKE_HITS) *
               RECORD(mode, TARGET_STRIKE_PICTURE_FRAMES);
    s32 length = span * 4;
    s32 glint = 0;

    if (RECORD(mode, TARGET_STRIKE_KIND) == STRIKE_KIND_SLASH) {
      // Strikes: six pictures each, crossing the target from the user's
      // side.
      if (frame < span * 6) {
        s32 picture = frame / RECORD(mode, TARGET_STRIKE_PICTURE_FRAMES) % 6;

        if (frame % (RECORD(mode, TARGET_STRIKE_PICTURE_FRAMES) * 6) == 0 &&
            (u32)(mode - 12) <= 1) {
          EffectPosition_ProjectScaledBattler(args->targets[0],
                                              &targetPosition);
          EffectPosition_ProjectBattler(args->targets[0], &alternate);
          targetPosition.y += (alternate.y - targetPosition.y) / 2;
          targetPosition.y += (Random16() & 15) - 8;
        }
        if (args->side == EFFECT_SIDE_PARTY) {
          EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                              EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                              BLITTER_MAX);
          gBlitter(canvas, work->sheet + gTargetStrike_Pictures[picture],
                   targetPosition.x / 2 - (gTargetStrike_Reach[picture] / 2) -
                       gTargetStrike_Widths[picture] + 8,
                   targetPosition.y - (gTargetStrike_Heights[picture] / 2),
                   gTargetStrike_Widths[picture],
                   gTargetStrike_Heights[picture]);
        } else {
          EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                              BLITTER_MAX);
          gBlitter(
              canvas, work->sheet + gTargetStrike_Pictures[picture],
              targetPosition.x / 2 + (gTargetStrike_Reach[picture] / 2) - 8,
              targetPosition.y - (gTargetStrike_Heights[picture] / 2),
              gTargetStrike_Widths[picture], gTargetStrike_Heights[picture]);
        }
        HeapSlot_Release(HEAP_SLOT_RAM_CODE);

        // A star sounds four frames before its hit.
        if (frame % (RECORD(mode, TARGET_STRIKE_PICTURE_FRAMES) * 6) ==
                RECORD(mode, TARGET_STRIKE_PICTURE_FRAMES) * 4 - 4 &&
            (RECORD(mode, TARGET_STRIKE_FLAGS) & STRIKE_STAR))
          Sound_PlayFar(SE_POWER_UP);

        // The hit, on the strike's fifth picture.
        if (frame % (RECORD(mode, TARGET_STRIKE_PICTURE_FRAMES) * 6) ==
            RECORD(mode, TARGET_STRIKE_PICTURE_FRAMES) * 4) {
          if (mode == 12) {
            if (frame / RECORD(mode, TARGET_STRIKE_PICTURE_FRAMES) / 6 <= 2)
              Sound_PlayFar(SE_IMPACT);
            else
              EFFECT_SHOW_DAMAGE(SE_IMPACT);
          } else if (mode == 7) {
            if (frame <= 8)
              Sound_PlayFar(SE_IMPACT);
            else
              EFFECT_SHOW_DAMAGE(SE_IMPACT);
          } else if (mode == 13) {
            if (frame / RECORD(mode, TARGET_STRIKE_PICTURE_FRAMES) / 6 > 6)
              EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
            else
              Sound_PlayFar(SE_HEAVY_IMPACT);
          } else if ((RECORD(mode, TARGET_STRIKE_FLAGS) & STRIKE_STAR) ||
                     (RECORD(mode, TARGET_STRIKE_FLAGS) & STRIKE_FLASH)) {
            EFFECT_SHOW_DAMAGE(SE_CLATTER);
          } else if (mode == TARGET_STRIKE_STYLE_MARS_HEAVY_SLASH) {
            EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
          } else {
            EFFECT_SHOW_DAMAGE(SE_IMPACT);
          }

          if (RECORD(mode, TARGET_STRIKE_FLAGS) & STRIKE_FLASH) {
            work->shakeFrames = 8;
            EFFECT_STRIKE_TARGET(args, 0, 12, KNOCKBACK_HURL);
            for (i = 0; i != 32; i++)
              gSparks[i].variant = (Random16() & 15) + 15;
          } else if (RECORD(mode, TARGET_STRIKE_FLAGS) & STRIKE_HEAVY) {
            work->shakeFrames = 8;
            EFFECT_STRIKE_TARGET(args, 0, 12, KNOCKBACK_HURL);
            for (i = 0; i != 256; i++)
              gSparks[i].variant = (Random16() & 15) + 15;
          } else {
            work->shakeFrames = 4;
            BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                                     POSE_DAMAGE, 0, 8);
            for (i = 0; i != 32; i++) {
              struct Particle *step =
                  &gSparks[frame /
                               (RECORD(mode, TARGET_STRIKE_PICTURE_FRAMES) *
                                6) *
                               32 +
                           i];

              step->variant = (Random16() & 15) + 7;
            }
          }
          if (mode == 16) {
            for (i = 0; i != 4; i++) {
              struct Particle *step =
                  &work->particles
                       [frame /
                            (RECORD(mode, TARGET_STRIKE_PICTURE_FRAMES) * 6) *
                            16 +
                        i];

              step->variant = 0;
            }
          } else {
            for (i = 0; i != 8; i++) {
              struct Particle *step =
                  &work->particles
                       [frame /
                            (RECORD(mode, TARGET_STRIKE_PICTURE_FRAMES) * 6) *
                            16 +
                        i];

              step->variant = 0;
            }
          }
        }
      }
      if (frame >= 12 && frame <= 31)
        glint = 1;
    } else if (frame < length + 4) {
      // Columns: a mirrored pair rising through its stages.
      s32 stage;

      if (frame < length) {
        s32 picture = frame / RECORD(mode, TARGET_STRIKE_PICTURE_FRAMES);

        while (picture > 4)
          picture -= 4;
        stage = gTargetStrike_BiteStages[picture];
      } else {
        stage = 3;
      }
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, CLIP,
                          RECORD(mode, TARGET_STRIKE_COMPOSITE));
      gBlitter(canvas, work->sheet + stage * 864 + 0xc80,
               targetPosition.x / 2 - TARGET_STRIKE_BITE_WIDTH, 56, 18,
               TARGET_STRIKE_BITE_HEIGHT);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, CLIP | COMPOSITE_FLIP_X,
                          RECORD(mode, TARGET_STRIKE_COMPOSITE));
      gBlitter(canvas, work->sheet + stage * 864 + 0xc80, targetPosition.x / 2,
               56, TARGET_STRIKE_BITE_WIDTH, TARGET_STRIKE_BITE_HEIGHT);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE);

      // The hit, on each column's fourth picture.
      if (frame % (RECORD(mode, TARGET_STRIKE_PICTURE_FRAMES) * 4) ==
          RECORD(mode, TARGET_STRIKE_PICTURE_FRAMES) * 3) {
        BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, 0, 8);
        work->shakeFrames = 4;
        if ((RECORD(mode, TARGET_STRIKE_FLAGS) & STRIKE_FLASH) ||
            (RECORD(mode, TARGET_STRIKE_FLAGS) & STRIKE_HEAVY))
          EFFECT_SHOW_DAMAGE(SE_CLATTER);
        else if (frame > (RECORD(mode, TARGET_STRIKE_HITS) * 4 - 4) *
                             RECORD(mode, TARGET_STRIKE_PICTURE_FRAMES))
          EFFECT_SHOW_DAMAGE(SE_IMPACT);
        else
          Sound_PlayFar(SE_IMPACT);
        if (RECORD(mode, TARGET_STRIKE_FLAGS) & STRIKE_FLASH) {
          work->shakeFrames = 8;
          EFFECT_STRIKE_TARGET(args, 0, 12, KNOCKBACK_HURL);
          for (i = 0; i != 32; i++)
            gSparks[i].variant = (Random16() & 15) + 15;
        } else if (RECORD(mode, TARGET_STRIKE_FLAGS) & STRIKE_HEAVY) {
          work->shakeFrames = 8;
          EFFECT_STRIKE_TARGET(args, 0, 12, KNOCKBACK_HURL);
          for (i = 0; i != 256; i++)
            gSparks[i].variant = (Random16() & 15) + 15;
        }
        for (i = 0; i != 64; i++) {
          struct Particle *step =
              &gSparks[frame /
                           (RECORD(mode, TARGET_STRIKE_PICTURE_FRAMES) * 6) *
                           64 +
                       i];

          step->variant = (Random16() & 15) + 7;
        }
        for (i = 0; i != 8; i++) {
          struct Particle *step =
              &work->particles[frame /
                                   (RECORD(mode, TARGET_STRIKE_PICTURE_FRAMES) *
                                    6) *
                                   16 +
                               i];

          step->variant = 0;
        }
        for (i = 0; i != 16; i++)
          gMotes[frame / (RECORD(mode, TARGET_STRIKE_PICTURE_FRAMES) * 6) * 16 +
                 i]
              .variant = 0;
      }
      if (stage == 3)
        glint = 1;
    }

    // Three glints a frame around the target's middle.
    if ((RECORD(mode, TARGET_STRIKE_FLAGS) & STRIKE_GLINTS) && glint) {
      EffectPosition_ProjectScaledBattler(args->targets[0], &center);
      EffectPosition_ProjectBattler(args->targets[0], &alternate);
      center.y += (alternate.y - center.y) / 2;
      for (i = 0; i != 3; i++) {
        s32 image = frame & 3;
        s32 angle = Random16() & TRIG_ANGLE_MASK;
        s32 radius = (Random16() & 31) + 4;
        s32 x;
        s32 y;
        u32 flip;

        x = center.x / 2 + (Trig_Sin(angle) * radius >> 17) -
            (gBattleEffect_GlintPictureWidths[image] / 2);
        y = center.y - (Trig_Cos(angle) * radius >> 17) -
            (gBattleEffect_GlintPictureHeights[image] / 2);
        flip = Random16();
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                            gTargetStrike_GlintDrawFlags[flip & 3] | CLIP,
                            BLITTER_ADDITIVE);
        gBlitterAlternate(canvas,
                          work->sheet + gBattleEffect_GlintPictures[image], x,
                          y, gBattleEffect_GlintPictureWidths[image],
                          gBattleEffect_GlintPictureHeights[image]);
        HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
      }
    }

    Matrix_Initialize();
    Matrix_SetLookAt(camera->eye, camera->focus);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
    draw[0] = gBlitter;
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                        BLITTER_ADDITIVE);
    draw[1] = gBlitterAlternate;
    for (k = 0; k != 1; k++) {
      source = BattleObject_GetFar(args->user)->object;

      // Wisps of smoke rise and spread for 24 frames.
      if (RECORD(mode, TARGET_STRIKE_FLAGS) & STRIKE_SMOKE) {
        for (i = 0; i != ARRAY_COUNT(work->particles); i++) {
          struct Particle *step = &work->particles[i];

          if (step->variant >= 0 && step->variant <= 23) {
            s32 picture = step->variant / 4;

            draw[i & 1](canvas,
                        work->sheet + picture * PICTURE_BYTES(SMOKE_PUFF) +
                            TARGET_STRIKE_SMOKE_PICTURES,
                        FIXED_INTEGER_PART(step->x) - 12,
                        FIXED_INTEGER_PART(step->y) - 24, SMOKE_PUFF_WIDTH,
                        SMOKE_PUFF_HEIGHT);
            BattleEffectKernels_IntegrateVector2(step, 62, -toFIXED(1) / 64);
            step->variant++;
          }
        }
      }

      // Flashes rise twice as fast, 32x64.
      if (RECORD(mode, TARGET_STRIKE_FLAGS) & STRIKE_FLASH) {
        for (i = 0; i != ARRAY_COUNT(work->particles); i++) {
          struct Particle *step = &work->particles[i];

          if (step->variant >= 0 && step->variant <= 23) {
            draw[0](canvas, work->sheet + (step->variant / 4 << 11) + 0x3200,
                    FIXED_INTEGER_PART(step->x) - 16,
                    FIXED_INTEGER_PART(step->y) - 32, FLASH_BURST_WIDTH,
                    FLASH_BURST_HEIGHT);
            BattleEffectKernels_IntegrateVector2(step, 62, -toFIXED(1) / 32);
            BattleEffectKernels_IntegrateVector2(step, 62, -toFIXED(1) / 32);
            step->variant++;
          }
        }
      }

      // Motes burst out, then stream back to the user, who flashes.
      if (RECORD(mode, TARGET_STRIKE_FLAGS) & STRIKE_ABSORB) {
        s32 size = 3;

        if (mode == 11)
          size = 8;
        if (frame == 55)
          BattleEffect_TintBattler(args->user, SPRITE_COLOR_WHITE, TINT_KEEP,
                                   TINT_KEEP, 0);
        if (frame == 90)
          BattleEffect_TintBattler(args->user, SPRITE_COLOR_NORMAL, TINT_KEEP,
                                   TINT_KEEP, 0);
        for (i = 0; i != 64; i++) {
          struct Particle *step = &gMotes[i];

          if (step->variant >= 0) {
            EffectPosition_ProjectPoint((s32 *)step, &position);
            position.x >>= 1;
            draw[1](canvas, sheet + gBattleEffect_MotePictures[size - 1],
                    position.x - ((u32)size >> 1), position.y - size, size,
                    size * 2);
            BattleEffectKernels_IntegrateVector3(step, 60, 0);
            step->variant++;
            if (step->variant > 10) {
              s32 dx = (source->x - step->x) >> 8;
              s32 dy = (source->y - step->y + toFIXED(20)) >> 8;
              s32 dz = (source->z - step->z) >> 8;

              step->velocityX += dx;
              step->velocityY += dy;
              step->velocityZ += dz;
              if ((dx >= -0xfff && dx <= 0xfff) &&
                  (dz >= -0xfff && dz <= 0xfff))
                step->variant = -1;
            }
          }
        }
      }

      // Sparks fall away, shrinking.
      if (RECORD(mode, TARGET_STRIKE_FLAGS) & STRIKE_SPARKS) {
        for (i = 0; i != 256; i++) {
          struct Particle *step = &gSparks[i];
          s32 size = step->variant;

          if (size >= 0) {
            EffectPosition_ProjectPoint((s32 *)step, &position);
            position.x >>= 1;
            BattleEffectKernels_IntegrateVector3(step, 60, 0);
            size >>= 3;
            size++;
            draw[1](canvas, sheet + gBattleEffect_MotePictures[size - 1],
                    position.x - size / 2, position.y - size, size, size * 2);
            step->variant--;
          }
        }
      }
    }
    EFFECT_RELEASE_BLITTERS();

    // The star: a textured square, swelling on a sine from frame 8 to 17
    // over the target's middle (the canvas is cleared to colour 0x3f first).
    if (RECORD(mode, TARGET_STRIKE_FLAGS) & STRIKE_STAR) {
      void *corners = Heap_AllocEwram(32);
      struct RenderModel *model = Render_AllocateModelList(1);

      model->shade = 0;
      texture.logWidth = 6;
      texture.logHeight = 6;
      texture.pixels = work->sheet + 0x3200;
      model->kind = MODEL_SHADED_MAX;
      model->mesh = gBattleEffect_SquareMesh;
      model->texture = &texture;
      model->vertices = corners;
      if (frame == 8)
        Iwram_FillWordsEntry(canvas, CANVAS_BYTES,
                             CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
      if (frame >= 8 && frame <= 17) {
        s32 scale =
            Trig_Sin(((frame - 8) << 11) + TRIG_TURN * 3 / 16) * 612 >> 8;

        Matrix_Initialize();
        Matrix_Translate(INTtoFIXED(targetPosition.x / 2 - 64),
                         INTtoFIXED(targetPosition.y - 64), 0);
        Matrix_RotateX(DEGtoANG(90));
        Matrix_RotateY(DEGtoANG(45));
        Matrix_Scale(scale, scale / 2, scale / 2);
        Render_TransformVertices(gBattleEffect_GroundSquareCorners, corners, 4);
        Render_DrawModelList(model);
      }
      Heap_Release(model);
      Heap_Release(corners);
    }

    if (RECORD(mode, TARGET_STRIKE_FLAGS) & STRIKE_HEAVY)
      BattleCamera_Shake(8, 8);
    else
      BattleCamera_Shake(2, 4);
    EFFECT_END_FRAME(work);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

#define BLADE_RAIN_FRAMES 120

/// Sixteen swords fall slanting in on the first target, one every 4 frames, and
/// stick in the ground.
void BattleEffect_RunBladeRain(struct BattleEffectParams *args) {
  struct ScreenPoint pos;
  BlitterPair draw;
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle *blit;
  struct Particle *dagger;
  s32 frame;
  s32 i;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  EFFECT_START_NARROW_CANVAS(BGCNT_PRIORITY(1));
  REG_BLDALPHA = EFFECT_BLEND_CANVAS(0);

  // [0] as drawn, [1] mirrored left to right; both skip clear pixels.
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      BLITTER_TRANSPARENT);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                      BLITTER_TRANSPARENT);
  draw[1] = (DrawRectangle)Ram_HeapSlots->ramCodePaired;
  blit = draw;
  Resource_LoadAndDecompress((s32)&ResourceId_CrossedDaggersSheet, work->sheet,
                             1, 1);
  work->transferMode = CANVAS_COPY_AND_CLEAR; // each transfer clears the canvas
  work->transferValue = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  // Centre BG2 on the first target.
  EffectPosition_ProjectScaledBattler(args->targets[0], &pos);
  REG_BG2X = (64 - pos.x) << 8;

  // Daggers start in columns 16 to 111; the further from the middle, the
  // faster they drift towards it (in steps of 2 pixels a frame).
  for (i = 0; i != ARRAY_COUNT(work->particles); i++) {
    s32 column;

    dagger = &work->particles[i];
    column = Random16() % 96 + 16;
    dagger->x = column;
    dagger->y = INTtoFIXED(24 - i / 4);
    if (column <= 43)
      dagger->velocityX = 3;
    else if (column <= 51)
      dagger->velocityX = 2;
    else if (column <= 59)
      dagger->velocityX = 1;
    else if (column <= 67)
      dagger->velocityX = 0;
    else if (column <= 75)
      dagger->velocityX = -1;
    else if (column <= 83)
      dagger->velocityX = -2;
    else
      dagger->velocityX = -3;
    dagger->velocityX <<= 17;
    dagger->velocityY = toFIXED(8);
    dagger->x <<= 16;
  }
  Sound_PlayFar(SE_POWER_UP);

  for (frame = 0; frame != BLADE_RAIN_FRAMES; frame++) {
    if (frame <= 16) {
      REG_BLDALPHA = BLEND_ALPHA(frame, BLEND_WEIGHT_MAX);
      if (frame == 16)
        REG_BLDCNT = 0;
    }
    if (frame > 103) {
      REG_BLDALPHA = BLEND_ALPHA(120 - frame, BLEND_WEIGHT_MAX);
      if (frame == 104)
        REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
    }

    // The drift picks the picture; a dagger going left is drawn mirrored.
    // Each one falls from 16 frames after its turn until it lands at 25.
    for (i = 15; i != -1; i--) {
      s32 drift;
      s32 speed;
      s32 cell;

      dagger = &work->particles[i];
      drift = dagger->velocityX;
      speed = drift;
      if (drift < 0)
        speed = -drift;
      cell = speed >> 17;
      if (frame < i * 4 + 25) {
        blit[(u32)drift >> 31](canvas, work->sheet + (cell << 10),
                               (FIXEDtoINT(dagger->x)) - 16,
                               (FIXEDtoINT(dagger->y)) - 16, 32, 32);
        if (frame >= i * 4 + 16) {
          dagger->x += dagger->velocityX;
          dagger->y += dagger->velocityY;
        }
      } else {
        blit[(u32)drift >> 31](canvas, work->sheet + (cell << 10),
                               (FIXEDtoINT(dagger->x)) - 16,
                               (FIXEDtoINT(dagger->y)) - 16, 32, 32);
      }
    }

    // From frame 24 to 84, every 4 frames: flash the target for 2 frames
    // and shake the screen; every 8, a sound.
    if (frame >= 23 && frame <= 87 && (frame & 3) == 0) {
      BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                               POSE_DAMAGE, 0, 2);
      work->shakeFrames = 1;
      if ((frame & 7) == 0)
        Sound_PlayFar(SE_IMPACT);
    }
    BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS();
}

// The whole effect work block: struct BattleEffectWork names only its first
// 0x78f4 bytes.

/// The flecks live in the map cell buffer.
#define gFlecks ((struct Particle *)Ram_MapCellBuffer)

// A pillar of ice rising at the user's feet with flecks spiralling up
// around it, for the scenes.

#define ELEMENT_PILLAR_FRAMES 56

#define ELEMENT_PILLAR_GLOW_HEIGHT 43

/// IceTileSheet: the pillar's half in four pictures, then its glow in three.
#define ELEMENT_PILLAR_HALF_WIDTH 17
#define ELEMENT_PILLAR_HALF_HEIGHT 64
#define ELEMENT_PILLAR_GLOW_WIDTH 24

/// A pillar of light in an element's colours stands at a unit's feet while
/// motes spiral up around it.
void BattleEffect_RunElementPillar(struct BattleEffectParams *args) {
  struct ScreenPoint ground;
  struct ScreenPoint pos;
  s32 point[3];
  struct BattleEffectWork *work;
  void *canvas;
  BlitterPair draw;
  u8 *sheet;
  struct BattleCamera *camera;
  s32 frame;
  void *palette;
  s32 i;

  work = HeapSlot_Alloc(HEAP_SLOT_BATTLE_EFFECT, BATTLE_EFFECT_WORK_SIZE);
  canvas = HeapSlot_Alloc(HEAP_SLOT_BATTLE_CANVAS, CANVAS_BYTES);
  sheet = HeapSlot_Alloc(HEAP_SLOT_BATTLE_SHEET, 1550);
  camera = Ram_HeapSlots->cameraWork;
  work->vblanksSinceTransfer = 1;
  work->args = args;
  BattleEffect_StartCanvasLayer(0);
  work->fadeFrames = BACKGROUND_DARKEN_FRAMES;
  work->fadeStep = 0;
  REG_BLDALPHA = EFFECT_BLEND_CANVAS(12);
  REG_BG2PA = Q_8_8(1);
  Resource_LoadAndDecompress((s32)&ResourceId_IceTileSheet, work->sheet, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD, sheet, 0, 0);
  switch (args->kind) {
  case 0:
    palette = Resource_GetTableEntry((s32)&ResourceId_YellowPaletteA);
    break;
  case 1:
    palette = Resource_GetTableEntry((s32)&ResourceId_IceTileSheet);
    break;
  case 2:
    palette = Resource_GetTableEntry((s32)&ResourceId_RedPaletteA);
    break;
  default:
    palette = Resource_GetTableEntry((s32)&ResourceId_VioletPaletteA);
    break;
  }
  Iwram_CopyWordsEntry((void *)BG_PLTT, palette, SHEET_PALETTE_BYTES);

  // A fleck's x is its angle and z its radius, in 1/16 pixels; fleck i
  // waits i frames before it rises.
  for (i = 0; i != 128; i++) {
    gFlecks[i].y = 0;
    gFlecks[i].x = Random16() & TRIG_ANGLE_MASK;
    gFlecks[i].z = (Random16() & 0x1ff) + i * 2;
    gFlecks[i].variant = -i;
  }
  work->transferMode =
      CANVAS_COPY_AND_FADE; // each transfer leaves 3/4 of the old picture,
  work->transferValue = CANVAS_FADE_THREE_QUARTERS; // a trail
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  args->variant++;
  if (args->variant <= 0)
    args->variant = 1;
  if (args->variant > 4)
    args->variant = 4;
  Sound_PlayFar(SE_POWER_UP);

  for (frame = 0; frame != ELEMENT_PILLAR_FRAMES; frame++) {
    EffectPosition_ProjectBattler(args->user, &ground);
    REG_BG2X = (64 - ground.x) << 8;
    if (frame > 49)
      REG_BLDALPHA = BLEND_ALPHA(112 - frame * 2, BLEND_WEIGHT_MAX);
    if (frame == 16)
      BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE, TINT_KEEP,
                               0, 20);

    // The pillar: the left half at x 47, then the right half mirrored at
    // x 64, each with its glow.
    if (frame <= 55) {
      s32 n;
      s32 flags;

      n = frame / 2 % 4; // a pillar picture every 2 frames
      flags = EFFECT_CLIP_BOUNDS;
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, flags, BLITTER_MAX);
      draw[1] = (DrawRectangle)Ram_HeapSlots->ramCodePaired;
      draw[1](canvas, work->sheet + n * 1088, 47,
              ground.y - ELEMENT_PILLAR_HALF_HEIGHT, ELEMENT_PILLAR_HALF_WIDTH,
              ELEMENT_PILLAR_HALF_HEIGHT);
      n = frame / 4 % 3; // a glow picture every 4 frames
      draw[1](canvas, work->sheet + n * 1032 + 0x1100, 40, ground.y - 36,
              ELEMENT_PILLAR_GLOW_WIDTH, ELEMENT_PILLAR_GLOW_HEIGHT);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
      flags = EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X;
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, flags, BLITTER_MAX);
      draw[1] = (DrawRectangle)Ram_HeapSlots->ramCodePaired;
      n = frame / 2 % 4;
      draw[1](canvas, work->sheet + n * 1088, 64,
              ground.y - ELEMENT_PILLAR_HALF_HEIGHT, ELEMENT_PILLAR_HALF_WIDTH,
              ELEMENT_PILLAR_HALF_HEIGHT);
      n = frame / 4 % 3;
      draw[1](canvas, work->sheet + n * 1032 + 0x1100, 64, ground.y - 36,
              ELEMENT_PILLAR_GLOW_WIDTH, ELEMENT_PILLAR_GLOW_HEIGHT);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
    }
    Matrix_Initialize();
    Matrix_SetLookAt(camera->eye, camera->focus);

    // The first 32 flecks turn 0x400, rise 5 pixels and widen 4 pixels a
    // frame.
    for (i = 0; i != 32; i++) {
      struct Particle *fleck = &gFlecks[i];

      if (fleck->variant >= 0) {
        s32 size;

        point[0] = fleck->z * Trig_Sin(fleck->x) >> 4;
        point[2] = -(fleck->z * Trig_Cos(fleck->x) >> 4);
        point[1] = fleck->y;
        fleck->x += toFIXED(1) / 64;
        fleck->y += toFIXED(5);
        fleck->z += 64;
        EffectPosition_ProjectPoint(point, &pos);
        pos.x /= 2;
        size = (i & 1) + args->variant;
        draw[0](canvas, sheet + gBattleEffect_FlarePictures[size - 1],
                pos.x - size, pos.y - size, size * 2, size * 2);
      }
      fleck->variant++;
    }
    EFFECT_END_FRAME(work);
  }

  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
  HeapSlot_Release(HEAP_SLOT_BATTLE_SHEET);
  HeapSlot_Release(HEAP_SLOT_BATTLE_CANVAS);
  HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
}

/// Unused: nothing refers to it. Shows the cloud swirl
/// (BattleEffect_RunCloudSwirl), then the battle background fading in, until A.
void BattleEffect_RunBackgroundFadeIn(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  struct BattleWork *battle;
  struct BattleBackgroundView *view;
  s32 frame;
  s32 i;
  s32 j;
  s32 k;

  work = HeapSlot_Alloc(HEAP_SLOT_BATTLE_EFFECT, BATTLE_EFFECT_WORK_SIZE);
  canvas = HeapSlot_Alloc(HEAP_SLOT_BATTLE_CANVAS, CANVAS_BYTES);
  battle = Ram_HeapSlots->battleWork;
  view = (struct BattleBackgroundView *)Ram_HeapSlots->battleBackground;
  work->args = args;
  work->vblanksSinceTransfer = 1;
  BattleEffect_RunCloudSwirl(args);

  // Make BG2 a 128x128 canvas, as BattleEffect_StartCanvasLayer does.
  BattleEffect_ClearActorTimers();
  view->secondMode = BATTLE_BACKGROUND_FIXED;
  gBgScroll[1].y = BATTLE_BACKGROUND_SCROLL_Y;
  BattleBackground_SetModeFar(BATTLE_BACKGROUND_FIXED,
                              BATTLE_BACKGROUND_ID(battle), 0);
  REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
  BattleCamera_SetViewFar(0, 0, 0, BATTLE_ZOOM_NORMAL);
  view->secondMode = BATTLE_BACKGROUND_NONE;
  REG_BG2X = 0;
  REG_BG2Y = -(BATTLE_VIEW_TOP << 8);
  REG_BG2PA = Q_8_8(0.5);
  REG_BG2PB = 0;
  REG_BG2PC = 0;
  REG_BG2PD = Q_8_8(1);
  EFFECT_SET_BATTLE_WINDOWS();
  REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG2 |
              WININ_WIN1_OBJ | WININ_WIN1_CLR;
  REG_WINOUT = WINOUT_WIN01_BG0 | WINOUT_WIN01_CLR | WINOUT_WINOBJ_ALL;

  i = 0;
  for (k = 0; k != 16; k++) {
    for (j = 0; j != 8; j++, i += 2) {
      s32 entry;

      entry = (s16)(((k * 16 + j * 2 + 1) << 8) | ((k * 8 + j) * 2));
      *(volatile u16 *)(BG_SCREEN_BLOCK(7) + i) = *(u16 *)&entry;
    }
  }

  Iwram_ClearWords(canvas, CANVAS_BYTES);
  Iwram_ClearWords((void *)BG_CHAR_BLOCK(1), CANVAS_BYTES);
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, EFFECT_DISPCNT, IO_WRITE_HALFWORD);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  REG_BLDCNT = 0;
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      BLITTER_TRANSPARENT);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                      BLITTER_MAX);

  // Keep the picture at 0x06008000 aside, show the battle background with
  // its colours black, and flash the first target.
  Iwram_CopyWords(Ram_MapCellBuffer, (void *)BG_CHAR_BLOCK(2), 0x7800);
  Iwram_FillWords(Ram_MapCellBuffer, BATTLE_BACKGROUND_BYTES,
                  CANVAS_FILL_WORD(1));
  work->savedBg1X = gBgScroll[1].x;
  work->savedBg1Y = gBgScroll[1].y;
  BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED,
                           BATTLE_BACKGROUND_ID(battle), -1);
  Palette_BrightenSavedBattleBg(-RGB_LEVEL_MAX, -RGB_LEVEL_MAX, -RGB_LEVEL_MAX);
  BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE, 3, 0, 30);

  // Fade the colours up a shade every two frames; B starts again, A ends.
  for (frame = 0; frame != 32096; frame++) {
    if (gInput.held & KEY_B)
      frame = 0;
    if (gInput.held & KEY_A)
      break;
    if (frame <= 63)
      Palette_BrightenSavedBattleBg(frame / 2 - 31, frame / 2 - 31,
                                    frame / 2 - 31);
    EFFECT_END_FRAME(work);
  }

  // Put the background and the display back.
  EFFECT_RELEASE_BLITTERS();
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_TintBattler(args->targets[0], TINT_KEEP, POSE_STANDING,
                           TINT_KEEP, 0);
  gBgScroll[1].x = work->savedBg1X;
  gBgScroll[1].y = BATTLE_BACKGROUND_SCROLL_Y;
  BattleBackground_SetModeFar(BATTLE_BACKGROUND_ZOOMED,
                              BATTLE_BACKGROUND_ID(battle), 0);
  wait(1);
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                          DISPCNT_BG2_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON |
                          DISPCNT_WIN1_ON,
                      IO_WRITE_HALFWORD);
  HeapSlot_Release(HEAP_SLOT_BATTLE_SHEET);
  HeapSlot_Release(HEAP_SLOT_BATTLE_CANVAS);
  HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
}
#undef BLITTER
#undef CLIP
#undef RECORD
#undef gBlitter
#undef gBlitterAlternate
#undef TARGET_STRIKE_KIND
#undef TARGET_STRIKE_HITS
#undef TARGET_STRIKE_FLAGS
#undef TARGET_STRIKE_COLOR
#undef TARGET_STRIKE_PICTURE_FRAMES
#undef TARGET_STRIKE_COMPOSITE
#undef TARGET_STRIKE_FRAMES
#undef STRIKE_SMOKE
#undef STRIKE_ABSORB
#undef STRIKE_GLINTS
#undef STRIKE_SPARKS
#undef STRIKE_HEAVY
#undef STRIKE_FLASH
#undef STRIKE_STAR
#undef gMotes
#undef gSparks
#undef gFlecks

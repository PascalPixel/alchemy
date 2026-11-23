/// @file
/// Monster bites and slashes on one target, and Mad Zephyr; then the unused
/// Blade Rain, the element pillar the battle raises at a unit's feet, and an
/// unused fade-in of the battle background.
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

/// Unused: no ability plays it, so the player never sees it: fangs bite the
/// first target once, hard, throwing sparks, in Mars Djinn colours.
void BattleEffect_RunUnusedHeavyBite(struct BattleEffectParams *args) {
  BattleEffect_RunTargetStrike(args, TARGET_STRIKE_STYLE_MARS_HEAVY_BITE);
}

/// Unused: no ability plays it, so the player never sees it: fangs bite the
/// first target once with a flash and sparks, in orange.
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

/// Unused: no ability plays it, so the player never sees it: fangs bite the
/// first target once, in blue, with sparks.
void BattleEffect_RunUnusedSparkBite(struct BattleEffectParams *args) {
  BattleEffect_RunTargetStrike(args, TARGET_STRIKE_STYLE_BLUE_SPARK_BITE);
}

/// The user lunges a fifth of the way at the first target, then four slashes
/// strike it, throwing sparks, in lightning colours; eight in its stronger
/// variant.
///
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

/// The blitter the effect draws with, made in the heap's RAM code slot.
#define BLITTER ((DrawRectangle)Ram_HeapSlots->ramCode)

/// Column `field` of style `mode`'s row of gTargetStrike_Styles.
#define TARGET_STRIKE_VALUE(mode, field) gTargetStrike_Styles[(mode)][(field)]

/// The motes in PARTICLES, 128 for each target, then the strike sparks.
#define TARGET_STRIKE_MOTES_PER_TARGET 128
#define TARGET_STRIKE_MOTE_SLOTS 512
#define TARGET_STRIKE_SPARK_COUNT 256
#define TARGET_STRIKE_SPARKS (PARTICLES + TARGET_STRIKE_MOTE_SLOTS)
/// The sparks a slash's hit or a flash sets flying, and a bite's hit; a heavy
/// hit sets them all flying.
#define TARGET_STRIKE_HIT_SPARKS 32
#define TARGET_STRIKE_BITE_SPARKS 64
/// The motes a bite's hit sends out; the first 64 are drawn.
#define TARGET_STRIKE_BITE_MOTES 16
#define TARGET_STRIKE_DRAWN_MOTES 64
/// A flash lives through FlashBurstSheet's six pictures, four frames each.
#define TARGET_STRIKE_FLASH_FRAMES (FLASH_BURST_PICTURE_COUNT * 4)
/// The frames a drained mote flies out before the user pulls it in.
#define TARGET_STRIKE_MOTE_FLIGHT_FRAMES 10
/// The star's sine steps a 32nd of a turn a frame, and its scale rises to
/// TARGET_STRIKE_STAR_SCALE 256ths, about 2.4 times.
#define TARGET_STRIKE_STAR_STEP_SHIFT 11
#define TARGET_STRIKE_STAR_SCALE 612
/// The hit's pictures in the sheet, by style: SmokeSheet's six 24x48 puffs,
/// FlashBurstSheet's six 32x64 bursts, or StarCrossSheet's star.
#define TARGET_STRIKE_HIT_PICTURES 12800
/// The bite's stages after the tornado sheet: BeamImage for a slash,
/// TwelveModeImage otherwise.
#define TARGET_STRIKE_BITE_PICTURES 3200

/// Strikes the first target with slashes or a bite of closing fangs, with the
/// extras its style's flags choose (enum TargetStrikeStyle): smoke, motes that
/// stream back into the user, glints, sparks, a flash or a star. Each hit
/// flashes and shakes the target, and the battle shows the damage at the last.
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

  // The sheets: motes, the tornado, the slashes or fangs, the style's
  // colours and the hit's pictures (smoke, flashes or the star).
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, sheet, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_TORNADO_SHEET, work->sheet, 1, 0);
  if (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_KIND) == STRIKE_KIND_SLASH)
    Resource_LoadAndDecompress(RESOURCE_BEAM_IMAGE,
                               work->sheet + TARGET_STRIKE_BITE_PICTURES, 0, 0);
  else
    Resource_LoadAndDecompress(RESOURCE_TWELVE_MODE_IMAGE,
                               work->sheet + TARGET_STRIKE_BITE_PICTURES, 0, 0);
  switch (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_COLOR)) {
  case STRIKE_COLOR_BLUE:
    resource = RESOURCE_BLUE_BEAM_SHEET;
    break;
  case STRIKE_COLOR_LIME:
    resource = RESOURCE_LIME_PALETTE;
    break;
  case STRIKE_COLOR_PINK:
    resource = RESOURCE_PINK_PALETTE;
    break;
  case STRIKE_COLOR_EMBER:
    resource = RESOURCE_EMBER_STREAK_SHEET;
    break;
  case STRIKE_COLOR_MARS:
    resource = RESOURCE_MARS_DJINN_SHEET;
    break;
  case STRIKE_COLOR_LIGHTNING:
  default:
    resource = RESOURCE_LIGHTNING_BOLT_SHEET;
    break;
  case STRIKE_COLOR_ORANGE:
    resource = RESOURCE_ORANGE_PALETTE_B;
    break;
  }
  Iwram_CopyWordsEntry((void *)BG_PLTT, Resource_GetTableEntry(resource),
                       SHEET_PALETTE_BYTES);
  if (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_FLAGS) & STRIKE_FLASH)
    Resource_LoadAndDecompress(RESOURCE_FLASH_BURST_SHEET,
                               work->sheet + TARGET_STRIKE_HIT_PICTURES, 1, 0);
  else if (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_FLAGS) & STRIKE_STAR)
    Resource_LoadAndDecompress(RESOURCE_STAR_CROSS_SHEET,
                               work->sheet + TARGET_STRIKE_HIT_PICTURES, 1, 0);
  else
    Resource_LoadAndDecompress(RESOURCE_SMOKE_SHEET,
                               work->sheet + TARGET_STRIKE_HIT_PICTURES, 1, 0);

  /* LEFTOVER(template): a per-target loop whose count is 1, the first target
   * only. */
  for (k = 0; k != 1; k++) {
    // The smoke or flashes start at the target's feet, waiting (-1) until a
    // hit.
    target = BattleObject_GetFar(args->targets[k])->object;
    EffectPosition_ProjectBattler(args->targets[k], &ground);
    for (i = 0; i != ARRAY_COUNT(work->particles); i++) {
      struct Particle *puff = &work->particles[i];

      SET_POSITION(puff, INTtoFIXED(ground.x / 2), toFIXED(80), 0);
      puff->velocityX = ((Random16() & 255) - 128) << 9;
      puff->velocityY = ((Random16() & 255) - 128) << 9;
      puff->velocityZ = 0;
      puff->variant = -1;
    }

    // The motes burst from 20 pixels above the target.
    for (i = 0; i != TARGET_STRIKE_MOTES_PER_TARGET; i++) {
      struct Particle *mote =
          &PARTICLES[k * TARGET_STRIKE_MOTES_PER_TARGET + i];

      SET_POSITION(mote, target->x, toFIXED(20), target->z);
      mote->velocityX = ((Random16() & 255) - 128) << 11;
      mote->velocityY = ((Random16() & 255) - 128) << 11;
      mote->velocityZ = ((Random16() & 255) - 128) << 11;
      mote->variant = -1;
    }

    // The sparks fly away from the hit: heavy hits throw them in a fan
    // away from the user, the others towards the middle of the field.
    for (i = 0; i != TARGET_STRIKE_SPARK_COUNT; i++) {
      SET_POSITION(&TARGET_STRIKE_SPARKS[i], target->x, toFIXED(20), target->z);
      if (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_FLAGS) & STRIKE_HEAVY) {
        s32 angle = Random16();
        s32 speed = Random16() & 255;

        TARGET_STRIKE_SPARKS[i].velocityX = (Random16() & 0x1ff) << 11;
        if (args->side == EFFECT_SIDE_PARTY)
          TARGET_STRIKE_SPARKS[i].velocityX =
              -TARGET_STRIKE_SPARKS[i].velocityX;
        TARGET_STRIKE_SPARKS[i].velocityY = Trig_Sin(angle) * speed >> 4;
        TARGET_STRIKE_SPARKS[i].velocityZ = Trig_Cos(angle) * speed >> 4;
      } else {
        if (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_KIND) == STRIKE_KIND_BITE) {
          TARGET_STRIKE_SPARKS[i].velocityX = ((Random16() & 255) - 128) << 11;
          TARGET_STRIKE_SPARKS[i].velocityY = ((Random16() & 255) - 128) << 11;
          TARGET_STRIKE_SPARKS[i].velocityZ = ((Random16() & 255) - 128) << 11;
        } else {
          TARGET_STRIKE_SPARKS[i].velocityX = (Random16() & 255) << 11;
          TARGET_STRIKE_SPARKS[i].velocityY = ((Random16() & 255) - 128) << 11;
          TARGET_STRIKE_SPARKS[i].velocityZ = ((Random16() & 255) - 128) << 11;
        }
        if (TARGET_STRIKE_SPARKS[i].x > 0)
          TARGET_STRIKE_SPARKS[i].velocityX =
              -TARGET_STRIKE_SPARKS[i].velocityX;
      }
      TARGET_STRIKE_SPARKS[i].variant = -1;
    }
  }

  work->transferMode = CANVAS_COPY_AND_FADE;
  if (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_FLAGS) & STRIKE_STAR)
    work->transferValue = CANVAS_FADE_HALF;
  else
    work->transferValue = CANVAS_FADE_THREE_QUARTERS;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK);

  // Aim at the middle of the first target.
  EffectPosition_ProjectScaledBattler(args->targets[0], &targetPosition);
  EffectPosition_ProjectBattler(args->targets[0], &alternate);
  targetPosition.y += (alternate.y - targetPosition.y) / 2;

  for (frame = 0; frame != TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_FRAMES);
       frame++) {
    s32 span = TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_HITS) *
               TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_PICTURE_FRAMES);
    s32 length = span * 4;
    s32 glint = 0;

    if (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_KIND) == STRIKE_KIND_SLASH) {
      // Slashes: six pictures each, sweeping across the target from the
      // user's side.
      if (frame < span * 6) {
        s32 picture =
            frame / TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_PICTURE_FRAMES) % 6;

        // Mad Zephyr aims each slash a little higher or lower.
        if (frame % (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_PICTURE_FRAMES) *
                     6) ==
                0 &&
            (mode == TARGET_STRIKE_STYLE_LIGHTNING_FOUR_SPARK_SLASHES ||
             mode == TARGET_STRIKE_STYLE_LIGHTNING_EIGHT_SPARK_SLASHES)) {
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
          BLITTER(canvas, work->sheet + gTargetStrike_Pictures[picture],
                  targetPosition.x / 2 - (gTargetStrike_Reach[picture] / 2) -
                      gTargetStrike_Widths[picture] + 8,
                  targetPosition.y - (gTargetStrike_Heights[picture] / 2),
                  gTargetStrike_Widths[picture],
                  gTargetStrike_Heights[picture]);
        } else {
          EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                              BLITTER_MAX);
          BLITTER(canvas, work->sheet + gTargetStrike_Pictures[picture],
                  targetPosition.x / 2 + (gTargetStrike_Reach[picture] / 2) - 8,
                  targetPosition.y - (gTargetStrike_Heights[picture] / 2),
                  gTargetStrike_Widths[picture],
                  gTargetStrike_Heights[picture]);
        }
        HeapSlot_Release(HEAP_SLOT_RAM_CODE);

        // A star sounds four frames before its hit.
        if (frame % (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_PICTURE_FRAMES) *
                     6) ==
                TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_PICTURE_FRAMES) * 4 -
                    4 &&
            (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_FLAGS) & STRIKE_STAR))
          Sound_PlayFar(SE_POWER_UP);

        // The hit, on the slash's fifth picture.
        if (frame %
                (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_PICTURE_FRAMES) * 6) ==
            TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_PICTURE_FRAMES) * 4) {
          if (mode == TARGET_STRIKE_STYLE_LIGHTNING_FOUR_SPARK_SLASHES) {
            if (frame /
                    TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_PICTURE_FRAMES) /
                    6 <=
                2)
              Sound_PlayFar(SE_IMPACT);
            else
              EFFECT_SHOW_DAMAGE(SE_IMPACT);
          } else if (mode == TARGET_STRIKE_STYLE_LIGHTNING_SPARK_SLASHES) {
            if (frame <= 8)
              Sound_PlayFar(SE_IMPACT);
            else
              EFFECT_SHOW_DAMAGE(SE_IMPACT);
          } else if (mode ==
                     TARGET_STRIKE_STYLE_LIGHTNING_EIGHT_SPARK_SLASHES) {
            if (frame /
                    TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_PICTURE_FRAMES) /
                    6 >
                6)
              EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
            else
              Sound_PlayFar(SE_HEAVY_IMPACT);
          } else if ((TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_FLAGS) &
                      STRIKE_STAR) ||
                     (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_FLAGS) &
                      STRIKE_FLASH)) {
            EFFECT_SHOW_DAMAGE(SE_CLATTER);
          } else if (mode == TARGET_STRIKE_STYLE_MARS_HEAVY_SLASH) {
            EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
          } else {
            EFFECT_SHOW_DAMAGE(SE_IMPACT);
          }

          if (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_FLAGS) & STRIKE_FLASH) {
            work->shakeFrames = 8;
            EFFECT_STRIKE_TARGET(args, 0, 12, KNOCKBACK_HURL);
            for (i = 0; i != TARGET_STRIKE_HIT_SPARKS; i++)
              TARGET_STRIKE_SPARKS[i].variant = (Random16() & 15) + 15;
          } else if (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_FLAGS) &
                     STRIKE_HEAVY) {
            work->shakeFrames = 8;
            EFFECT_STRIKE_TARGET(args, 0, 12, KNOCKBACK_HURL);
            for (i = 0; i != TARGET_STRIKE_SPARK_COUNT; i++)
              TARGET_STRIKE_SPARKS[i].variant = (Random16() & 15) + 15;
          } else {
            work->shakeFrames = 4;
            BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                                     POSE_DAMAGE, 0, 8);
            for (i = 0; i != TARGET_STRIKE_HIT_SPARKS; i++) {
              struct Particle *spark =
                  &TARGET_STRIKE_SPARKS[frame /
                                            (TARGET_STRIKE_VALUE(
                                                 mode,
                                                 TARGET_STRIKE_PICTURE_FRAMES) *
                                             6) *
                                            TARGET_STRIKE_HIT_SPARKS +
                                        i];

              spark->variant = (Random16() & 15) + 7;
            }
          }
          if (mode == TARGET_STRIKE_STYLE_ORANGE_FLASH_SPARK_SLASH) {
            for (i = 0; i != 4; i++) {
              struct Particle *puff =
                  &work->particles[frame /
                                       (TARGET_STRIKE_VALUE(
                                            mode,
                                            TARGET_STRIKE_PICTURE_FRAMES) *
                                        6) *
                                       16 +
                                   i];

              puff->variant = 0;
            }
          } else {
            for (i = 0; i != 8; i++) {
              struct Particle *puff =
                  &work->particles[frame /
                                       (TARGET_STRIKE_VALUE(
                                            mode,
                                            TARGET_STRIKE_PICTURE_FRAMES) *
                                        6) *
                                       16 +
                                   i];

              puff->variant = 0;
            }
          }
        }
      }
      if (frame >= 12 && frame <= 31)
        glint = 1;
    } else if (frame < length + 4) {
      // Bites: rows of fangs close above and below the target, its left half
      // drawn and then mirrored.
      s32 stage;

      if (frame < length) {
        s32 picture =
            frame / TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_PICTURE_FRAMES);

        while (picture > 4)
          picture -= 4;
        stage = gTargetStrike_BiteStages[picture];
      } else {
        stage = 3;
      }
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                          TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_COMPOSITE));
      BLITTER(canvas,
              work->sheet + stage * PICTURE_BYTES(TARGET_STRIKE_BITE) +
                  TARGET_STRIKE_BITE_PICTURES,
              targetPosition.x / 2 - TARGET_STRIKE_BITE_WIDTH,
              TARGET_STRIKE_BITE_Y, TARGET_STRIKE_BITE_WIDTH,
              TARGET_STRIKE_BITE_HEIGHT);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                          TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_COMPOSITE));
      BLITTER(canvas,
              work->sheet + stage * PICTURE_BYTES(TARGET_STRIKE_BITE) +
                  TARGET_STRIKE_BITE_PICTURES,
              targetPosition.x / 2, TARGET_STRIKE_BITE_Y,
              TARGET_STRIKE_BITE_WIDTH, TARGET_STRIKE_BITE_HEIGHT);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE);

      // The hit, on each bite's fourth picture.
      if (frame %
              (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_PICTURE_FRAMES) * 4) ==
          TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_PICTURE_FRAMES) * 3) {
        BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, 0, 8);
        work->shakeFrames = 4;
        if ((TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_FLAGS) & STRIKE_FLASH) ||
            (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_FLAGS) & STRIKE_HEAVY))
          EFFECT_SHOW_DAMAGE(SE_CLATTER);
        else if (frame >
                 (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_HITS) * 4 - 4) *
                     TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_PICTURE_FRAMES))
          EFFECT_SHOW_DAMAGE(SE_IMPACT);
        else
          Sound_PlayFar(SE_IMPACT);
        if (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_FLAGS) & STRIKE_FLASH) {
          work->shakeFrames = 8;
          EFFECT_STRIKE_TARGET(args, 0, 12, KNOCKBACK_HURL);
          for (i = 0; i != TARGET_STRIKE_HIT_SPARKS; i++)
            TARGET_STRIKE_SPARKS[i].variant = (Random16() & 15) + 15;
        } else if (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_FLAGS) &
                   STRIKE_HEAVY) {
          work->shakeFrames = 8;
          EFFECT_STRIKE_TARGET(args, 0, 12, KNOCKBACK_HURL);
          for (i = 0; i != TARGET_STRIKE_SPARK_COUNT; i++)
            TARGET_STRIKE_SPARKS[i].variant = (Random16() & 15) + 15;
        }
        for (i = 0; i != TARGET_STRIKE_BITE_SPARKS; i++) {
          struct Particle *spark =
              &TARGET_STRIKE_SPARKS[frame /
                                        (TARGET_STRIKE_VALUE(
                                             mode,
                                             TARGET_STRIKE_PICTURE_FRAMES) *
                                         6) *
                                        TARGET_STRIKE_BITE_SPARKS +
                                    i];

          spark->variant = (Random16() & 15) + 7;
        }
        for (i = 0; i != 8; i++) {
          struct Particle *puff =
              &work->particles[frame /
                                   (TARGET_STRIKE_VALUE(
                                        mode, TARGET_STRIKE_PICTURE_FRAMES) *
                                    6) *
                                   16 +
                               i];

          puff->variant = 0;
        }
        for (i = 0; i != TARGET_STRIKE_BITE_MOTES; i++)
          PARTICLES[frame /
                        (TARGET_STRIKE_VALUE(mode,
                                             TARGET_STRIKE_PICTURE_FRAMES) *
                         6) *
                        TARGET_STRIKE_BITE_MOTES +
                    i]
              .variant = 0;
      }
      if (stage == 3)
        glint = 1;
    }

    // Three glints a frame around the target's middle.
    if ((TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_FLAGS) & STRIKE_GLINTS) &&
        glint) {
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

        x = center.x / 2 + (FIXEDtoINT(Trig_Sin(angle) * radius) >> 1) -
            (gBattleEffect_GlintPictureWidths[image] / 2);
        y = center.y - (FIXEDtoINT(Trig_Cos(angle) * radius) >> 1) -
            (gBattleEffect_GlintPictureHeights[image] / 2);
        flip = Random16();
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                            gTargetStrike_GlintDrawFlags[flip & 3] |
                                EFFECT_CLIP_BOUNDS,
                            BLITTER_ADDITIVE);
        ALT_BLITTER(canvas, work->sheet + gBattleEffect_GlintPictures[image], x,
                    y, gBattleEffect_GlintPictureWidths[image],
                    gBattleEffect_GlintPictureHeights[image]);
        HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
      }
    }

    Matrix_Initialize();
    Matrix_SetLookAt(camera->eye, camera->focus);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
    draw[0] = BLITTER;
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                        BLITTER_ADDITIVE);
    draw[1] = ALT_BLITTER;
    /* LEFTOVER(template): a per-target loop whose count is 1, the user only. */
    for (k = 0; k != 1; k++) {
      source = BattleObject_GetFar(args->user)->object;

      // Wisps of smoke rise and spread for SMOKE_PUFF_FRAMES.
      if (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_FLAGS) & STRIKE_SMOKE) {
        for (i = 0; i != ARRAY_COUNT(work->particles); i++) {
          struct Particle *puff = &work->particles[i];

          if (puff->variant >= 0 && puff->variant < SMOKE_PUFF_FRAMES) {
            s32 picture = puff->variant / 4;

            draw[i & 1](canvas,
                        work->sheet + picture * PICTURE_BYTES(SMOKE_PUFF) +
                            TARGET_STRIKE_HIT_PICTURES,
                        FIXED_INTEGER_PART(puff->x) - SMOKE_PUFF_WIDTH / 2,
                        FIXED_INTEGER_PART(puff->y) - SMOKE_PUFF_HEIGHT / 2,
                        SMOKE_PUFF_WIDTH, SMOKE_PUFF_HEIGHT);
            Particle_AdvanceWithGravity2D(puff, 62, -toFIXED(1) / 64);
            puff->variant++;
          }
        }
      }

      // Flashes rise twice as fast as smoke.
      if (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_FLAGS) & STRIKE_FLASH) {
        for (i = 0; i != ARRAY_COUNT(work->particles); i++) {
          struct Particle *flash = &work->particles[i];

          if (flash->variant >= 0 &&
              flash->variant < TARGET_STRIKE_FLASH_FRAMES) {
            draw[0](canvas,
                    work->sheet +
                        flash->variant / 4 * PICTURE_BYTES(FLASH_BURST) +
                        TARGET_STRIKE_HIT_PICTURES,
                    FIXED_INTEGER_PART(flash->x) - FLASH_BURST_WIDTH / 2,
                    FIXED_INTEGER_PART(flash->y) - FLASH_BURST_HEIGHT / 2,
                    FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
            Particle_AdvanceWithGravity2D(flash, 62, -toFIXED(1) / 32);
            Particle_AdvanceWithGravity2D(flash, 62, -toFIXED(1) / 32);
            flash->variant++;
          }
        }
      }

      // Motes burst out, then stream back to the user, who flashes.
      if (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_FLAGS) & STRIKE_ABSORB) {
        s32 size = 3;

        if (mode == TARGET_STRIKE_STYLE_MARS_ADDITIVE_DRAIN_BITES)
          size = 8;
        if (frame == 55)
          BattleEffect_TintBattler(args->user, SPRITE_COLOR_WHITE, TINT_KEEP,
                                   TINT_KEEP, 0);
        if (frame == 90)
          BattleEffect_TintBattler(args->user, SPRITE_COLOR_NORMAL, TINT_KEEP,
                                   TINT_KEEP, 0);
        for (i = 0; i != TARGET_STRIKE_DRAWN_MOTES; i++) {
          struct Particle *mote = &PARTICLES[i];

          if (mote->variant >= 0) {
            EffectPosition_ProjectPoint((s32 *)mote, &position);
            position.x >>= 1;
            EFFECT_DRAW_MOTE(draw[1], canvas, sheet, position.x, position.y,
                             size);
            Particle_AdvanceWithGravity3D(mote, 60, 0);
            mote->variant++;
            if (mote->variant > TARGET_STRIKE_MOTE_FLIGHT_FRAMES) {
              s32 dx = (source->x - mote->x) >> 8;
              s32 dy = (source->y - mote->y + toFIXED(20)) >> 8;
              s32 dz = (source->z - mote->z) >> 8;

              mote->velocityX += dx;
              mote->velocityY += dy;
              mote->velocityZ += dz;
              if ((dx > -(toFIXED(1) / 16) && dx < toFIXED(1) / 16) &&
                  (dz > -(toFIXED(1) / 16) && dz < toFIXED(1) / 16))
                mote->variant = -1;
            }
          }
        }
      }

      // Sparks fall away, shrinking.
      if (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_FLAGS) & STRIKE_SPARKS) {
        for (i = 0; i != TARGET_STRIKE_SPARK_COUNT; i++) {
          struct Particle *spark = &TARGET_STRIKE_SPARKS[i];
          s32 size = spark->variant;

          if (size >= 0) {
            EffectPosition_ProjectPoint((s32 *)spark, &position);
            position.x >>= 1;
            Particle_AdvanceWithGravity3D(spark, 60, 0);
            size >>= 3;
            size++;
            EFFECT_DRAW_MOTE(draw[1], canvas, sheet, position.x, position.y,
                             size);
            spark->variant--;
          }
        }
      }
    }
    EFFECT_RELEASE_BLITTERS();

    // The star: StarCrossSheet's star of light on a square over the target's
    // middle swells and shrinks from frame 8 to 17, as the canvas flashes to
    // its brightest.
    if (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_FLAGS) & STRIKE_STAR) {
      void *corners = Heap_AllocEwram(32);
      struct RenderModel *model = Render_AllocateModelList(1);

      model->shade = 0;
      texture.logWidth = 6;
      texture.logHeight = 6;
      texture.pixels = work->sheet + TARGET_STRIKE_HIT_PICTURES;
      model->kind = MODEL_SHADED_MAX;
      model->mesh = gBattleEffect_SquareMesh;
      model->texture = &texture;
      model->vertices = corners;
      if (frame == 8)
        Iwram_FillWordsEntry(canvas, CANVAS_BYTES,
                             CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
      if (frame >= 8 && frame <= 17) {
        s32 scale = Trig_Sin(((frame - 8) << TARGET_STRIKE_STAR_STEP_SHIFT) +
                             TRIG_TURN * 3 / 16) *
                        TARGET_STRIKE_STAR_SCALE >>
                    8;

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

    if (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_FLAGS) & STRIKE_HEAVY)
      BattleCamera_Shake(8, 8);
    else
      BattleCamera_Shake(2, 4);
    EFFECT_END_FRAME(work);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

#define BLADE_RAIN_FRAMES 120
#define BLADE_RAIN_SWORDS 16
/// A sword drifts sideways 0 to 3 steps of two pixels a frame, and its picture
/// is its step's: velocityX is the step << BLADE_RAIN_STEP_SHIFT.
#define BLADE_RAIN_STEP_SHIFT 17
/// Where on the canvas the first sword waits for its turn; each four after it
/// wait a pixel higher.
#define BLADE_RAIN_WAIT_Y 24
/// CrossedDaggersSheet: four 32x32 pictures of a sword falling point down, each
/// slanting more than the last.
#define BLADE_RAIN_SWORD_WIDTH 32
#define BLADE_RAIN_SWORD_HEIGHT 32

/// Unused: nothing calls it. Swords fall slanting in on the first target, one
/// every 4 frames, and stick in the ground.
UNUSED void BattleEffect_RunBladeRain(struct BattleEffectParams *args) {
  struct ScreenPoint screenPosition;
  BlitterPair draw;
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle *blit;
  struct Particle *blade;
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
  Resource_LoadAndDecompress(RESOURCE_CROSSED_DAGGERS_SHEET, work->sheet, 1, 1);
  work->transferMode = CANVAS_COPY_AND_CLEAR; // each transfer clears the canvas
  work->transferValue = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK);

  // Centre BG2 on the first target.
  EffectPosition_ProjectScaledBattler(args->targets[0], &screenPosition);
  REG_BG2X = (CANVAS_SIZE / 2 - screenPosition.x) << 8;

  // Swords start in columns 16 to 111 near the top of the canvas; the further
  // from the middle, the faster they drift towards it.
  for (i = 0; i != ARRAY_COUNT(work->particles); i++) {
    s32 column;

    blade = &work->particles[i];
    column = Random16() % 96 + 16;
    blade->x = column;
    blade->y = toFIXED(BLADE_RAIN_WAIT_Y - i / 4);
    if (column <= 43)
      blade->velocityX = 3;
    else if (column <= 51)
      blade->velocityX = 2;
    else if (column <= 59)
      blade->velocityX = 1;
    else if (column <= 67)
      blade->velocityX = 0;
    else if (column <= 75)
      blade->velocityX = -1;
    else if (column <= 83)
      blade->velocityX = -2;
    else
      blade->velocityX = -3;
    blade->velocityX <<= BLADE_RAIN_STEP_SHIFT;
    blade->velocityY = toFIXED(8);
    blade->x <<= 16;
  }
  Sound_PlayFar(SE_POWER_UP);

  for (frame = 0; frame != BLADE_RAIN_FRAMES; frame++) {
    if (frame <= 16) {
      REG_BLDALPHA = EFFECT_BLEND_CANVAS(frame);
      if (frame == 16)
        REG_BLDCNT = 0;
    }
    if (frame > 103) {
      REG_BLDALPHA = EFFECT_BLEND_CANVAS(BLADE_RAIN_FRAMES - frame);
      if (frame == 104)
        REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
    }

    // The faster a sword drifts, the more its picture slants, and one
    // drifting left is mirrored. Sword i falls from frame i * 4 + 16 until it
    // sticks in the ground at i * 4 + 25.
    for (i = BLADE_RAIN_SWORDS - 1; i != -1; i--) {
      s32 drift;
      s32 speed;
      s32 picture;

      blade = &work->particles[i];
      drift = blade->velocityX;
      speed = drift;
      if (drift < 0)
        speed = -drift;
      picture = speed >> BLADE_RAIN_STEP_SHIFT;
      if (frame < i * 4 + 25) {
        blit[(u32)drift >> 31](
            canvas, work->sheet + picture * PICTURE_BYTES(BLADE_RAIN_SWORD),
            FIXEDtoINT(blade->x) - BLADE_RAIN_SWORD_WIDTH / 2,
            FIXEDtoINT(blade->y) - BLADE_RAIN_SWORD_HEIGHT / 2,
            BLADE_RAIN_SWORD_WIDTH, BLADE_RAIN_SWORD_HEIGHT);
        if (frame >= i * 4 + 16) {
          blade->x += blade->velocityX;
          blade->y += blade->velocityY;
        }
      } else {
        blit[(u32)drift >> 31](
            canvas, work->sheet + picture * PICTURE_BYTES(BLADE_RAIN_SWORD),
            FIXEDtoINT(blade->x) - BLADE_RAIN_SWORD_WIDTH / 2,
            FIXEDtoINT(blade->y) - BLADE_RAIN_SWORD_HEIGHT / 2,
            BLADE_RAIN_SWORD_WIDTH, BLADE_RAIN_SWORD_HEIGHT);
      }
    }

    // From frame 24 to 84, every 4 frames, the target flashes for 2 frames and
    // the screen shakes; every 8, a blow sounds.
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

#define ELEMENT_PILLAR_FRAMES 56
/// IceTileSheet: the pillar's half in four pictures, then its glow in three.
#define ELEMENT_PILLAR_HALF_WIDTH 17
#define ELEMENT_PILLAR_HALF_HEIGHT 64
#define ELEMENT_PILLAR_GLOW_WIDTH 24
#define ELEMENT_PILLAR_GLOW_HEIGHT 43
#define ELEMENT_PILLAR_GLOW (4 * PICTURE_BYTES(ELEMENT_PILLAR_HALF))
/// The pillar spirals the first ELEMENT_PILLAR_MOTES of its
/// ELEMENT_SPIRAL_MOTES up around it, each circling once in
/// ELEMENT_PILLAR_TURN_FRAMES.
#define ELEMENT_PILLAR_MOTES 32
#define ELEMENT_PILLAR_TURN_FRAMES 64
/// The bytes of ParticleSpritesD, the motes' sheet, which the pillar
/// allocates for itself.
#define ELEMENT_PILLAR_SHEET_BYTES 1550

/// A pillar of light in an element's colours stands at a unit's feet while
/// motes spiral up around it. The battle's element glow and the auto battle
/// run it, and it sets up its own heap blocks.
void BattleEffect_RunElementPillar(struct BattleEffectParams *args) {
  struct ScreenPoint ground;
  struct ScreenPoint screenPosition;
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
  sheet = HeapSlot_Alloc(HEAP_SLOT_BATTLE_SHEET, ELEMENT_PILLAR_SHEET_BYTES);
  camera = Ram_HeapSlots->cameraWork;
  work->vblanksSinceTransfer = 1;
  work->args = args;
  BattleEffect_StartCanvasLayer(0);
  work->fadeFrames = BACKGROUND_DARKEN_FRAMES;
  work->fadeStep = 0;
  REG_BLDALPHA = EFFECT_BLEND_CANVAS(ELEMENT_BLEND_LEVEL);
  REG_BG2PA = Q_8_8(1);
  Resource_LoadAndDecompress(RESOURCE_ICE_TILE_SHEET, work->sheet, 1, 0);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_D, sheet, 0, 0);
  switch (args->kind) {
  case ELEMENT_VENUS:
    palette = Resource_GetTableEntry(RESOURCE_YELLOW_PALETTE_A);
    break;
  case ELEMENT_MERCURY:
    palette = Resource_GetTableEntry(RESOURCE_ICE_TILE_SHEET);
    break;
  case ELEMENT_MARS:
    palette = Resource_GetTableEntry(RESOURCE_RED_PALETTE_A);
    break;
  default:
    palette = Resource_GetTableEntry(RESOURCE_VIOLET_PALETTE_A);
    break;
  }
  Iwram_CopyWordsEntry((void *)BG_PLTT, palette, SHEET_PALETTE_BYTES);

  // A mote's x is its angle and z its distance from the unit, in sixteenths
  // of a pixel; mote i waits i frames before it rises.
  for (i = 0; i != ELEMENT_SPIRAL_MOTES; i++) {
    PARTICLES[i].y = 0;
    PARTICLES[i].x = Random16() & TRIG_ANGLE_MASK;
    PARTICLES[i].z = (Random16() & 0x1ff) + i * 2;
    PARTICLES[i].variant = -i;
  }
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  // The motes' size grows with the variant, from 1 to 4.
  args->variant++;
  if (args->variant <= 0)
    args->variant = 1;
  if (args->variant > 4)
    args->variant = 4;
  Sound_PlayFar(SE_POWER_UP);

  for (frame = 0; frame != ELEMENT_PILLAR_FRAMES; frame++) {
    EffectPosition_ProjectBattler(args->user, &ground);
    REG_BG2X = (CANVAS_SIZE / 2 - ground.x) << 8;
    if (frame > 49)
      REG_BLDALPHA = EFFECT_BLEND_CANVAS(112 - frame * 2);
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
      draw[1](canvas, work->sheet + n * PICTURE_BYTES(ELEMENT_PILLAR_HALF), 47,
              ground.y - ELEMENT_PILLAR_HALF_HEIGHT, ELEMENT_PILLAR_HALF_WIDTH,
              ELEMENT_PILLAR_HALF_HEIGHT);
      n = frame / 4 % 3; // a glow picture every 4 frames
      draw[1](canvas,
              work->sheet + n * PICTURE_BYTES(ELEMENT_PILLAR_GLOW) +
                  ELEMENT_PILLAR_GLOW,
              40, ground.y - 36, ELEMENT_PILLAR_GLOW_WIDTH,
              ELEMENT_PILLAR_GLOW_HEIGHT);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
      flags = EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X;
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, flags, BLITTER_MAX);
      draw[1] = (DrawRectangle)Ram_HeapSlots->ramCodePaired;
      n = frame / 2 % 4;
      draw[1](canvas, work->sheet + n * PICTURE_BYTES(ELEMENT_PILLAR_HALF), 64,
              ground.y - ELEMENT_PILLAR_HALF_HEIGHT, ELEMENT_PILLAR_HALF_WIDTH,
              ELEMENT_PILLAR_HALF_HEIGHT);
      n = frame / 4 % 3;
      draw[1](canvas,
              work->sheet + n * PICTURE_BYTES(ELEMENT_PILLAR_GLOW) +
                  ELEMENT_PILLAR_GLOW,
              64, ground.y - 36, ELEMENT_PILLAR_GLOW_WIDTH,
              ELEMENT_PILLAR_GLOW_HEIGHT);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
    }
    Matrix_Initialize();
    Matrix_SetLookAt(camera->eye, camera->focus);

    // Each mote circles, rising 5 pixels and widening 4 pixels a frame.
    for (i = 0; i != ELEMENT_PILLAR_MOTES; i++) {
      struct Particle *fleck = &PARTICLES[i];

      if (fleck->variant >= 0) {
        s32 size;

        point[0] = fleck->z * Trig_Sin(fleck->x) >> 4;
        point[2] = -(fleck->z * Trig_Cos(fleck->x) >> 4);
        point[1] = fleck->y;
        fleck->x += TRIG_TURN / ELEMENT_PILLAR_TURN_FRAMES;
        fleck->y += toFIXED(5);
        fleck->z += 64;
        EffectPosition_ProjectPoint(point, &screenPosition);
        screenPosition.x /= 2;
        size = (i & 1) + args->variant;
        EFFECT_DRAW_FLARE(draw[0], canvas, sheet, screenPosition.x,
                          screenPosition.y, size);
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
  struct BattleBackgroundWork *view;
  s32 frame;
  s32 i;
  s32 j;
  s32 k;

  work = HeapSlot_Alloc(HEAP_SLOT_BATTLE_EFFECT, BATTLE_EFFECT_WORK_SIZE);
  canvas = HeapSlot_Alloc(HEAP_SLOT_BATTLE_CANVAS, CANVAS_BYTES);
  battle = Ram_HeapSlots->battleWork;
  view = Ram_HeapSlots->battleBackground;
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
  REG_WINOUT = EFFECT_WINOUT;

  i = 0;
  for (k = 0; k != CANVAS_TILES; k++)
    EFFECT_WRITE_CANVAS_MAP_ROW(k, j, i);

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

  // Keep the battle background's picture aside, show the battle background
  // with its colours black, and flash the first target.
  Iwram_CopyWords(Ram_MapCellBuffer, (void *)BG_CHAR_BLOCK(2),
                  BATTLE_BACKGROUND_BYTES);
  Iwram_FillWords(Ram_MapCellBuffer, BATTLE_BACKGROUND_BYTES,
                  CANVAS_FILL_WORD(1));
  work->savedBg1X = gBgScroll[1].x;
  work->savedBg1Y = gBgScroll[1].y;
  BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED,
                           BATTLE_BACKGROUND_ID(battle), -1);
  Palette_BrightenSavedBattleBg(-RGB_LEVEL_MAX, -RGB_LEVEL_MAX, -RGB_LEVEL_MAX);
  BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE, POSE_CAST, 0,
                           30);

  // Fade the colours up a shade every two frames; B starts again, A ends.
  for (frame = 0; frame != 32096; frame++) {
    if (gInput.held & KEY_B)
      frame = 0;
    if (gInput.held & KEY_A)
      break;
    if (frame <= 63)
      Palette_BrightenSavedBattleBg(frame / 2 - RGB_LEVEL_MAX,
                                    frame / 2 - RGB_LEVEL_MAX,
                                    frame / 2 - RGB_LEVEL_MAX);
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
#undef TARGET_STRIKE_VALUE
#undef TARGET_STRIKE_MOTES_PER_TARGET
#undef TARGET_STRIKE_MOTE_SLOTS
#undef TARGET_STRIKE_SPARK_COUNT
#undef TARGET_STRIKE_HIT_SPARKS
#undef TARGET_STRIKE_BITE_SPARKS
#undef TARGET_STRIKE_BITE_MOTES
#undef TARGET_STRIKE_DRAWN_MOTES
#undef TARGET_STRIKE_FLASH_FRAMES
#undef TARGET_STRIKE_STAR_STEP_SHIFT
#undef TARGET_STRIKE_STAR_SCALE
#undef TARGET_STRIKE_SPARKS
#undef ELEMENT_PILLAR_SHEET_BYTES
#undef TARGET_STRIKE_MOTE_FLIGHT_FRAMES
#undef TARGET_STRIKE_HIT_PICTURES
#undef TARGET_STRIKE_BITE_PICTURES
#undef BLADE_RAIN_FRAMES
#undef BLADE_RAIN_SWORDS
#undef BLADE_RAIN_STEP_SHIFT
#undef BLADE_RAIN_WAIT_Y
#undef BLADE_RAIN_SWORD_WIDTH
#undef BLADE_RAIN_SWORD_HEIGHT
#undef ELEMENT_PILLAR_FRAMES
#undef ELEMENT_PILLAR_HALF_WIDTH
#undef ELEMENT_PILLAR_HALF_HEIGHT
#undef ELEMENT_PILLAR_GLOW_WIDTH
#undef ELEMENT_PILLAR_GLOW_HEIGHT
#undef ELEMENT_PILLAR_GLOW
#undef ELEMENT_PILLAR_MOTES
#undef ELEMENT_PILLAR_TURN_FRAMES

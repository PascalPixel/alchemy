/// @file
/// Monster bites and slashes on one target.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"

/// The strike sparks, after the motes in PARTICLES.
#define TARGET_STRIKE_SPARKS (PARTICLES + 512)

/// The smoke pictures in the sheet: six 24x48 puffs.
#define TARGET_STRIKE_SMOKE_PICTURES 12800

/// Column `field` of style `mode`'s row of gTargetStrike_Styles.
#define TARGET_STRIKE_VALUE(mode, field) gTargetStrike_Styles[(mode)][(field)]

/// The columns of gTargetStrike_Styles.
enum {
  TARGET_STRIKE_KIND,           ///< STRIKE_KIND_SLASH or STRIKE_KIND_BITE
  TARGET_STRIKE_HITS,           ///< how many times the target is struck
  TARGET_STRIKE_FLAGS,          ///< STRIKE_SMOKE and so on
  TARGET_STRIKE_COLOR,          ///< background palette, a STRIKE_COLOR_
  TARGET_STRIKE_PICTURE_FRAMES, ///< frames each strike picture is shown
  TARGET_STRIKE_COMPOSITE, ///< how the fangs are drawn (COMPOSITE_LIGHTEN or
                           ///< _ADDITIVE)
  TARGET_STRIKE_FRAMES     ///< length of the effect in frames
};

/// One slash strikes the first target among glints, in lightning colours.
///
/// Abilities: Numbing Sting.
void BattleEffect_RunNumbingSting(struct BattleEffectParams *args) {
  BattleEffect_RunTargetStrike(args, TARGET_STRIKE_STYLE_LIGHTNING_GLINT_SLASH);
}
/// Fangs bite the first target three times, in ember colours, and drain it
/// into the user.
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
/// Abilities: Poison Tail.
void BattleEffect_RunPoisonTail(struct BattleEffectParams *args) {
  BattleEffect_RunTargetStrike(args, TARGET_STRIKE_STYLE_LIME_SMOKE_SLASH);
}
/// Fangs bite the first target three times and drain it, in Mars Djinn
/// colours.
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
/// One heavy slash strikes the first target, with sparks, in Mars Djinn
/// colours.
///
/// Abilities: Mortal Blow.
void BattleEffect_RunMortalBlow(struct BattleEffectParams *args) {
  BattleEffect_RunTargetStrike(args, TARGET_STRIKE_STYLE_MARS_HEAVY_SLASH);
}
/// Two slashes strike the first target, with sparks, in lightning colours.
///
/// Abilities: Twin Beaks.
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
/// Fangs bite the first target three times, added to what lies under them,
/// and drain it with large motes, in Mars Djinn colours.
///
/// Abilities: Drain Fang.
void BattleEffect_RunDrainFang(struct BattleEffectParams *args) {
  BattleEffect_RunTargetStrike(args,
                               TARGET_STRIKE_STYLE_MARS_ADDITIVE_DRAIN_BITES);
}
/// Strikes the first target with slashes or a bite of closing fangs, with the
/// extras its flags choose.
void BattleEffect_RunTargetStrike(struct BattleEffectParams *args, s32 mode) {
  struct ScreenPoint ground;
  struct ScreenPoint targetPosition;
  struct ScreenPoint alternate;
  struct ScreenPoint position;
  BlitterPair draw;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 k;
  u8 *particleSheet;
  struct BattleCamera *camera;
  struct Actor *target;
  struct Actor *source;
  s32 resource;
  s32 i;
  DrawRectangle *lower;

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  lower = (DrawRectangle *)&HEAP_SLOT_BLOCK(HEAP_SLOT_RAM_CODE_2);
  particleSheet = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
  camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
  work->args = args;
  if (mode == TARGET_STRIKE_STYLE_MARS_HEAVY_SLASH)
    BattleEffect_StartCanvasLayer(0);
  else
    BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  REG_BLDALPHA = BLEND_ALPHA(16, 16);

  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, particleSheet, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_TORNADO_SHEET, work->sheet, 1, 0);
  if (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_KIND) == STRIKE_KIND_SLASH)
    Resource_LoadAndDecompress(RESOURCE_BEAM_IMAGE, work->sheet + 3200, 0, 0);
  else
    Resource_LoadAndDecompress(RESOURCE_TWELVE_MODE_IMAGE, work->sheet + 3200,
                               0, 0);
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
  }
  Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(resource), 128);
  Resource_LoadAndDecompress(RESOURCE_SMOKE_SHEET,
                             work->sheet + TARGET_STRIKE_SMOKE_PICTURES, 1, 0);

  /* LEFTOVER(template): a per-target loop whose count is 1, the first target
   * only; Camelot's template: the sibling effects BattleEffect_RunStreakVolley
   * and BattleEffect_RunDjinnBeams open the same loop, run to the target count,
   * with the same lookup of targets[k]; as its body alone, with 0 for k, 967 of
   * 1259 instructions differ in tbs-en. */
  for (k = 0; k != 1; k++) {
    target = GetBattleObjectSlotFar(work->args->targets[k])->actor;
    for (i = 0; i != 64; i++) {
      struct Particle *step = &work->particles[i];

      Projection_Unit(work->args->targets[k], &ground);
      SET_POSITION(step, px(ground.x / 2), px(80), 0);
      SET_VELOCITY(step, ((Random16() & 255) - 128) << 9,
                   ((Random16() & 255) - 128) << 9, 0);
      step->variant = -1;
    }
    for (i = 0; i != 128; i++) {
      struct Particle *step = &PARTICLES[k * 128 + i];

      SET_POSITION(step, target->x.fixed, px(20), target->z.fixed);
      SET_VELOCITY(step, ((Random16() & 255) - 128) << 11,
                   ((Random16() & 255) - 128) << 11,
                   ((Random16() & 255) - 128) << 11);
      step->variant = -1;
    }
    for (i = 0; i != 512; i++) {
      TARGET_STRIKE_SPARKS[i].x = target->x.fixed;
      TARGET_STRIKE_SPARKS[i].y = px(20);
      TARGET_STRIKE_SPARKS[i].z = target->z.fixed;
      if (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_KIND) == STRIKE_KIND_BITE) {
        TARGET_STRIKE_SPARKS[i].velocityX = ((Random16() & 255) - 128) << 11;
        TARGET_STRIKE_SPARKS[i].velocityY = ((Random16() & 255) - 128) << 11;
        TARGET_STRIKE_SPARKS[i].velocityZ = ((Random16() & 255) - 128) << 11;
      } else {
        if (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_FLAGS) & STRIKE_HEAVY) {
          TARGET_STRIKE_SPARKS[i].velocityX = (Random16() & 0x1ff) << 11;
          TARGET_STRIKE_SPARKS[i].velocityY = ((Random16() & 0x1ff) - 256)
                                              << 11;
          TARGET_STRIKE_SPARKS[i].velocityZ = ((Random16() & 0x1ff) - 256)
                                              << 11;
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

  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

  Projection_ScaledUnit(work->args->targets[0], &targetPosition);
  Projection_Unit(work->args->targets[0], &alternate);
  targetPosition.y += (alternate.y - targetPosition.y) / 2;

  for (frame = 0; frame != TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_FRAMES);
       frame++) {
    s32 span = TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_HITS) *
               TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_PICTURE_FRAMES);
    s32 length = span * 4;
    s32 glint = 0;

    if (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_KIND) == STRIKE_KIND_SLASH) {
      // Slashes sweep across the target from the user's side.
      if (frame < span * 6) {
        s32 picture =
            frame / TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_PICTURE_FRAMES) % 6;

        if (work->args->side == EFFECT_SIDE_PARTY) {
          EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                              EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                              COMPOSITE_LIGHTEN);
          EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE](
              canvas, work->sheet + gTargetStrike_Pictures[picture],
              targetPosition.x / 2 - (gTargetStrike_Reach[picture] / 2) -
                  gTargetStrike_Widths[picture] + 8,
              targetPosition.y - (gTargetStrike_Heights[picture] / 2),
              gTargetStrike_Widths[picture], gTargetStrike_Heights[picture]);
        } else {
          EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                              COMPOSITE_LIGHTEN);
          EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE](
              canvas, work->sheet + gTargetStrike_Pictures[picture],
              targetPosition.x / 2 + (gTargetStrike_Reach[picture] / 2) - 8,
              targetPosition.y - (gTargetStrike_Heights[picture] / 2),
              gTargetStrike_Widths[picture], gTargetStrike_Heights[picture]);
        }
        HeapSlot_Release(HEAP_SLOT_RAM_CODE);
        if (frame %
                (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_PICTURE_FRAMES) * 6) ==
            TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_PICTURE_FRAMES) * 4) {
          if (mode == TARGET_STRIKE_STYLE_MARS_HEAVY_SLASH) {
            EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
          } else {
            Sound_PlayFar(SE_IMPACT);
            EFFECT_SHOW_DAMAGE(SE_IMPACT);
          }
          if (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_FLAGS) & STRIKE_HEAVY) {
            work->shakeFrames = 8;
            EFFECT_STRIKE_TARGET(work->args, 0, 12, KNOCKBACK_HURL);
            for (i = 0; i != 512; i++)
              TARGET_STRIKE_SPARKS[i].variant = (Random16() & 15) + 15;
          } else {
            work->shakeFrames = 4;
            BattleEffect_TintBattler(work->args->targets[0], SPRITE_COLOR_WHITE,
                                     POSE_DAMAGE, 0, 8);
            for (i = 0; i != 32; i++) {
              struct Particle *step =
                  &TARGET_STRIKE_SPARKS[frame /
                                            (TARGET_STRIKE_VALUE(
                                                 mode,
                                                 TARGET_STRIKE_PICTURE_FRAMES) *
                                             6) *
                                            32 +
                                        i];

              step->variant = (Random16() & 15) + 7;
            }
          }
          for (i = 0; i != 8; i++) {
            struct Particle *step =
                &work->particles[frame /
                                     (TARGET_STRIKE_VALUE(
                                          mode, TARGET_STRIKE_PICTURE_FRAMES) *
                                      6) *
                                     16 +
                                 i];

            step->variant = 0;
          }
        }
      }
      if (frame >= 12 && frame <= 31)
        glint = 1;
    } else if (frame < length + 4) {
      // Bites: rows of fangs close above and below the target and meet in a
      // flash.
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
      EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE](
          canvas, work->sheet + stage * (18 * 48) + 3200,
          targetPosition.x / 2 - 18, 56, 18, 48);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                          TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_COMPOSITE));
      EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE](
          canvas, work->sheet + stage * (18 * 48) + 3200, targetPosition.x / 2,
          56, 18, 48);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE);
      if (frame %
              (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_PICTURE_FRAMES) * 4) ==
          TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_PICTURE_FRAMES) * 3) {
        // A hit: the target flashes, and the last lets the battle show the
        // damage.
        BattleEffect_TintBattler(work->args->targets[0], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, 0, 8);
        work->shakeFrames = 4;
        if (frame > (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_HITS) * 4 - 4) *
                        TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_PICTURE_FRAMES))
          EFFECT_SHOW_DAMAGE(SE_IMPACT);
        else
          Sound_PlayFar(SE_IMPACT);
        for (i = 0; i != 64; i++) {
          struct Particle *step =
              &TARGET_STRIKE_SPARKS[frame /
                                        (TARGET_STRIKE_VALUE(
                                             mode,
                                             TARGET_STRIKE_PICTURE_FRAMES) *
                                         6) *
                                        64 +
                                    i];

          step->variant = (Random16() & 15) + 7;
        }
        for (i = 0; i != 8; i++) {
          struct Particle *step =
              &work->particles[frame /
                                   (TARGET_STRIKE_VALUE(
                                        mode, TARGET_STRIKE_PICTURE_FRAMES) *
                                    6) *
                                   16 +
                               i];

          step->variant = 0;
        }
        for (i = 0; i != 16; i++)
          PARTICLES[frame /
                        (TARGET_STRIKE_VALUE(mode,
                                             TARGET_STRIKE_PICTURE_FRAMES) *
                         6) *
                        16 +
                    i]
              .variant = 0;
      }
      if (stage == 3)
        glint = 1;
    }

    // Glints scatter around the target.
    if ((TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_FLAGS) & STRIKE_GLINTS) &&
        glint) {
      for (i = 0; i != 3; i++) {
        s32 image = frame & 3;
        s32 angle = Random16() & TRIG_ANGLE_MASK;
        s32 radius = (Random16() & 31) + 4;
        s32 x;
        s32 y;

        x = targetPosition.x / 2 + (Trig_Sin(angle) * radius >> 17) -
            (gBattleEffect_GlintPictureWidths[image] / 2);
        y = targetPosition.y - (Trig_Cos(angle) * radius >> 17) -
            (gBattleEffect_GlintPictureHeights[image] / 2);
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                            gTargetStrike_GlintDrawFlags[Random16() & 3] |
                                EFFECT_CLIP_BOUNDS,
                            COMPOSITE_ADDITIVE);
        (*lower)(canvas, work->sheet + gBattleEffect_GlintPictures[image], x, y,
                 gBattleEffect_GlintPictureWidths[image],
                 gBattleEffect_GlintPictureHeights[image]);
        HeapSlot_Release(HEAP_SLOT_RAM_CODE_2);
      }
    }

    EFFECT_VIEW_FROM_CAMERA(camera);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                        COMPOSITE_LIGHTEN);
    draw[0] = EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE];
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, EFFECT_CLIP_BOUNDS,
                        COMPOSITE_ADDITIVE);
    draw[1] = *lower;
    /* LEFTOVER(template): a per-target loop whose count is 1, the user only;
     * Camelot's template, as the loop above: the sibling effects
     * BattleEffect_RunStreakVolley and BattleEffect_RunDjinnBeams run k to the
     * target count; as its body alone, with 0 for k, 891 of 1259
     * instructions differ in tbs-en. */
    for (k = 0; k != 1; k++) {
      source = GetBattleObjectSlotFar(work->args->user)->actor;
      // Smoke puffs rise and spread.
      if (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_FLAGS) & STRIKE_SMOKE) {
        for (i = 0; i != 64; i++) {
          struct Particle *step = &work->particles[i];

          if (step->variant >= 0 && step->variant <= 23) {
            s32 picture = step->variant / 4;

            draw[i & 1](canvas,
                        work->sheet + picture * (24 * 48) +
                            TARGET_STRIKE_SMOKE_PICTURES,
                        Q_16_16_TO_INT(step->x) - 12,
                        Q_16_16_TO_INT(step->y) - 24, 24, 48);
            Particle_AdvanceWithGravity2D(step, 62, -px(1) / 64);
            step->variant++;
          }
        }
      }
      // Motes burst out of the target and are pulled into the user, who glows.
      if (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_FLAGS) & STRIKE_ABSORB) {
        s32 size = 3;

        if (mode == TARGET_STRIKE_STYLE_MARS_ADDITIVE_DRAIN_BITES)
          size = 8;
        if (frame == 55)
          BattleEffect_TintBattler(work->args->user, SPRITE_COLOR_WHITE,
                                   TINT_KEEP, TINT_KEEP, 0);
        if (frame == 90)
          BattleEffect_TintBattler(work->args->user, SPRITE_COLOR_NORMAL,
                                   TINT_KEEP, TINT_KEEP, 0);
        for (i = 0; i != 64; i++) {
          struct Particle *step = &PARTICLES[i];

          if (step->variant >= 0) {
            Projection_Point(step, &position);
            position.x >>= 1;
            draw[1](canvas,
                    particleSheet + gBattleEffect_MotePictures[size - 1],
                    position.x - ((u32)size >> 1), position.y - size, size,
                    size * 2);
            Particle_AdvanceWithGravity3D(step, 60, 0);
            step->variant++;
            if (step->variant > 10) {
              s32 dx = (source->x.fixed - step->x) >> 8;
              s32 dy = (source->y.fixed - step->y + px(20)) >> 8;
              s32 dz = (source->z.fixed - step->z) >> 8;

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
      if (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_FLAGS) & STRIKE_SPARKS) {
        for (i = 0; i != 128; i++) {
          struct Particle *step = &TARGET_STRIKE_SPARKS[i];
          s32 size = step->variant;

          if (size >= 0) {
            Projection_Point(step, &position);
            position.x >>= 1;
            Particle_AdvanceWithGravity3D(step, 60, 0);
            size >>= 3;
            size++;
            EFFECT_DRAW_MOTE(draw[1], canvas, particleSheet, position.x,
                             position.y, size);
            step->variant--;
          }
        }
      }
    }
    EFFECT_RELEASE_BLITTERS();
    if (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_FLAGS) & STRIKE_HEAVY)
      BattleCamera_Shake(8, 8);
    else
      BattleCamera_Shake(2, 4);
    EFFECT_END_FRAME(work);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

#undef TARGET_STRIKE_SPARKS
#undef TARGET_STRIKE_VALUE
#undef TARGET_STRIKE_SMOKE_PICTURES

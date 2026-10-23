/// @file
/// Monster bites, fangs and slashes on one target.
// Part of effect.c.

#include "../effect_internal.h"

// Target strikes: repeated slashes or columns of light on the first target.
// The folder README describes them under "Effect families".

/// The strike sparks, 512 of them, from PARTICLES[512] (0x3800 bytes in);
/// the motes use PARTICLES 0 to 127. Only the first 128 sparks are drawn,
/// though a heavy hit lights all 512.
#define TARGET_STRIKE_SPARKS                                                   \
  ((struct Particle *)(Ram_MapGridCellBuffer + 0x3800))

/// The smoke pictures in the sheet: six 24x48 puffs.
#define TARGET_STRIKE_SMOKE_CELLS 0x3200

/// Column `field` of style `mode`'s row of gTargetStrike_Records.
#define TARGET_STRIKE_VALUE(mode, field) gTargetStrike_Records[(mode)][(field)]

/// The columns of gTargetStrike_Records.
enum {
  TARGET_STRIKE_KIND,         ///< STRIKE_KIND_SLASH or STRIKE_KIND_COLUMN
  TARGET_STRIKE_HITS,         ///< how many times the target is struck
  TARGET_STRIKE_FLAGS,        ///< STRIKE_SMOKE and so on
  TARGET_STRIKE_COLOR,        ///< background palette, a STRIKE_COLOR_
  TARGET_STRIKE_IMAGE_FRAMES, ///< frames each strike image is shown
  TARGET_STRIKE_COMPOSITE,    ///< how the columns are drawn (BLITTER_MAX or
                              ///< _ADDITIVE)
  TARGET_STRIKE_FRAMES        ///< length of the effect in frames
};
// STRIKE_KIND_*, the STRIKE_SMOKE flags, the STRIKE_COLOR_ palettes and enum
// TargetStrikeStyle are in effect_table.h.

/// One slash strikes the first target among glints, in lightning colours.
///
/// Abilities: Numbing Sting (BATTLE_ANIM_NUMBING_STING).
void BattleAnim_RunNumbingSting(struct BattleAnimArgs *effect) {
  BattleAnim_RunTargetStrike(effect, TARGET_STRIKE_STYLE_LIGHTNING_GLINT_SLASH);
}
/// Three ember columns strike the first target and drain it into the user.
///
/// Abilities: Cannibal Fang (BATTLE_ANIM_CANNIBAL_FANG).
void BattleAnim_RunCannibalFang(struct BattleAnimArgs *effect) {
  BattleAnim_RunTargetStrike(effect, TARGET_STRIKE_STYLE_EMBER_DRAIN_COLUMNS);
}
/// One lime column strikes the first target, with smoke.
///
/// Abilities: Poisonous Bite (BATTLE_ANIM_POISONOUS_BITE); Poison Fang
/// (BATTLE_ANIM_POISONOUS_BITE_323).
void BattleAnim_RunPoisonousBite(struct BattleAnimArgs *effect) {
  BattleAnim_RunTargetStrike(effect, TARGET_STRIKE_STYLE_LIME_SMOKE_COLUMN);
}
/// One column strikes the first target, with smoke, in Mars Djinn colours.
///
/// Abilities: Rabid Fang (BATTLE_ANIM_RABID_FANG).
void BattleAnim_RunRabidFang(struct BattleAnimArgs *effect) {
  BattleAnim_RunTargetStrike(effect, TARGET_STRIKE_STYLE_MARS_SMOKE_COLUMN);
}
/// Three lightning columns strike the first target among glints.
///
/// Abilities: Electric Bite (BATTLE_ANIM_ELECTRIC_BITE).
void BattleAnim_RunElectricBite(struct BattleAnimArgs *effect) {
  BattleAnim_RunTargetStrike(effect,
                             TARGET_STRIKE_STYLE_LIGHTNING_GLINT_COLUMNS);
}
/// One lime slash strikes the first target, with smoke.
///
/// Abilities: Poison Tail (BATTLE_ANIM_POISON_TAIL).
void BattleAnim_RunPoisonTail(struct BattleAnimArgs *effect) {
  BattleAnim_RunTargetStrike(effect, TARGET_STRIKE_STYLE_LIME_SMOKE_SLASH);
}
/// Three columns strike the first target and drain it, in Mars Djinn colours.
///
/// Abilities: Vampiric Fang (BATTLE_ANIM_VAMPIRIC_FANG).
void BattleAnim_RunVampiricFang(struct BattleAnimArgs *effect) {
  BattleAnim_RunTargetStrike(effect, TARGET_STRIKE_STYLE_MARS_DRAIN_COLUMNS);
}
/// Two blue columns strike the first target, with sparks.
///
/// Abilities: Double Fang (BATTLE_ANIM_DOUBLE_FANG).
void BattleAnim_RunDoubleFang(struct BattleAnimArgs *effect) {
  BattleAnim_RunTargetStrike(effect, TARGET_STRIKE_STYLE_BLUE_SPARK_COLUMNS);
}
/// One heavy slash strikes the first target, with sparks, in Mars Djinn
/// colours.
///
/// Abilities: Mortal Blow (BATTLE_ANIM_MORTAL_BLOW).
void BattleAnim_RunMortalBlow(struct BattleAnimArgs *effect) {
  BattleAnim_RunTargetStrike(effect, TARGET_STRIKE_STYLE_MARS_HEAVY_SLASH);
}
/// Two slashes strike the first target, with sparks, in lightning colours.
///
/// Abilities: Twin Beaks (BATTLE_ANIM_TWIN_BEAKS).
void BattleAnim_RunTwinBeaks(struct BattleAnimArgs *effect) {
  BattleAnim_RunTargetStrike(effect,
                             TARGET_STRIKE_STYLE_LIGHTNING_SPARK_SLASHES);
}
/// Three pink columns strike the first target, with smoke.
///
/// Abilities: Acid Bite (BATTLE_ANIM_ACID_BITE).
void BattleAnim_RunAcidBite(struct BattleAnimArgs *effect) {
  BattleAnim_RunTargetStrike(effect, TARGET_STRIKE_STYLE_PINK_SMOKE_COLUMNS);
}
/// Three additive columns strike the first target and drain it with large
/// motes, in Mars Djinn colours.
///
/// Abilities: Drain Fang (BATTLE_ANIM_DRAIN_FANG).
void BattleAnim_RunDrainFang(struct BattleAnimArgs *effect) {
  BattleAnim_RunTargetStrike(effect,
                             TARGET_STRIKE_STYLE_MARS_ADDITIVE_DRAIN_COLUMNS);
}
/// Strikes the first target TARGET_STRIKE_HITS times with a sideways slash or a
/// column of light, with the extras its flags choose, and lets the battle
/// show the damage on the hits.
///
/// Called by the twelve wrappers above, from BattleAnim_RunNumbingSting to
/// BattleAnim_RunDrainFang. Loads the style's colours over the first 64 of
/// the background palette; uses work->particles 0 to 63 for smoke, PARTICLES 0
/// to 127 for motes and TARGET_STRIKE_SPARKS for sparks; builds and frees its
/// blitters in both slots every frame, sets work->shakeFrames on each hit and
/// lights the user while an absorb runs. Leaves the canvas layer closed.
///
/// mode: the style (enum TargetStrikeStyle), a row of gTargetStrike_Records.
void BattleAnim_RunTargetStrike(struct BattleAnimArgs *effect, s32 mode) {
  struct ScreenPoint ground;
  struct ScreenPoint targetPosition;
  struct ScreenPoint alternate;
  struct ScreenPoint position;
  BlitterPair draw;
  void **slots;
  void **cursor;
  struct BattleAnimWork *work;
  void *canvas;
  s32 frame;
  s32 k;
  u8 *particleSheet;
  struct BattleCamera *camera;
  struct MotionObject *target;
  struct MotionObject *source;
  s32 resource;
  s32 i;
  s32 wispSheet = TARGET_STRIKE_SMOKE_CELLS;
  DrawRectangle *lower;

  ANIM_GET_SLOTS(slots, cursor, work, canvas);
  lower = (DrawRectangle *)&ANIM_SLOT(slots, HEAP_SLOT_RAM_CODE_2);
  particleSheet = ANIM_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  camera = ANIM_SLOT(slots, HEAP_SLOT_CAMERA);
  work->effect = effect;
  if (mode == TARGET_STRIKE_STYLE_MARS_HEAVY_SLASH)
    BattleAnim_StartCanvasLayer(0);
  else
    BattleAnim_StartCanvasLayer(1);
  // The canvas adds its full colour to everything under it.
  REG_BLDALPHA = BLEND_ALPHA(16, 16);

  // Setup. The images: glints at the start of the sheet, the slashes or the
  // columns at 0xc80, smoke at TARGET_STRIKE_SMOKE_CELLS, and the style's
  // palette.
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, particleSheet,
                             0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, work->sheet, 1, 0);
  if (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_KIND) == STRIKE_KIND_SLASH)
    Resource_LoadAndDecompress((s32)&ResourceId_BeamSequenceImage,
                               work->sheet + 0xc80, 0, 0);
  else
    Resource_LoadAndDecompress((s32)&ResourceId_TwelveModeImage,
                               work->sheet + 0xc80, 0, 0);
  switch (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_COLOR)) {
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
  }
  Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(resource), 128);
  Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet,
                             work->sheet + TARGET_STRIKE_SMOKE_CELLS, 1, 0);

  // Park every particle, idle, where it will start: smoke at the target's
  // feet, motes and sparks 20 pixels up its body, each with a random
  // velocity.
  // DEAD: runs once, for the first target only; the ROM keeps the loop.
  for (k = 0; k != 1; k++) {
    target = GetBattleObjectSlotFar(work->effect->targets[k])->object;
    // Smoke works on the canvas: x under the target, line 80, drifting up
    // to 1 pixel a frame each way.
    for (i = 0; i != 64; i++) {
      struct Particle *step = &work->particles[i];

      Projection_Unit(work->effect->targets[k], &ground);
      SET_POSITION(step, (ground.x / 2) << 16, 0x500000, 0);
      SET_VELOCITY(step, ((Random16() & 255) - 128) << 9,
                   ((Random16() & 255) - 128) << 9, 0);
      step->variant = -1;
    }
    // Motes and sparks work in battle space, projected each frame, from 20
    // up at the target. Motes and a column's sparks fly up to 4 a frame
    // along each axis; a slash's sparks up to 8 across, towards the middle,
    // and the heavy slash's twice as fast as that.
    for (i = 0; i != 128; i++) {
      struct Particle *step = &PARTICLES[k * 128 + i];

      SET_POSITION(step, target->x, 0x140000, target->z);
      SET_VELOCITY(step, ((Random16() & 255) - 128) << 11,
                   ((Random16() & 255) - 128) << 11,
                   ((Random16() & 255) - 128) << 11);
      step->variant = -1;
    }
    for (i = 0; i != 512; i++) {
      TARGET_STRIKE_SPARKS[i].x = target->x;
      TARGET_STRIKE_SPARKS[i].y = 0x140000;
      TARGET_STRIKE_SPARKS[i].z = target->z;
      if (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_KIND) == STRIKE_KIND_COLUMN) {
        TARGET_STRIKE_SPARKS[i].velocityX = ((Random16() & 255) - 128) << 11;
        TARGET_STRIKE_SPARKS[i].velocityY = ((Random16() & 255) - 128) << 11;
        TARGET_STRIKE_SPARKS[i].velocityZ = ((Random16() & 255) - 128) << 11;
      } else {
        // The heavy slash throws its sparks twice as wide.
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
        // Slashes throw sparks towards the middle of the field.
        if (TARGET_STRIKE_SPARKS[i].x > 0)
          TARGET_STRIKE_SPARKS[i].velocityX =
              -TARGET_STRIKE_SPARKS[i].velocityX;
      }
      TARGET_STRIKE_SPARKS[i].variant = -1;
    }
  }

  // Trails keep 3/4 a frame.
  ANIM_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);

  // Strike halfway between the target's middle and its feet.
  Projection_ScaledUnit(work->effect->targets[0], &targetPosition);
  Projection_Unit(work->effect->targets[0], &alternate);
  targetPosition.y += (alternate.y - targetPosition.y) / 2;

  for (frame = 0; frame != TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_FRAMES);
       frame++) {
    // Frame loop: the strike, then glints, then smoke, motes and sparks,
    // each drawn over the last. Hit n lights particle group n, found as
    // frame / (6 * IMAGE_FRAMES).
    s32 span = TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_HITS) *
               TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_IMAGE_FRAMES);
    s32 length = span * 4;
    s32 glint = 0;

    if (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_KIND) == STRIKE_KIND_SLASH) {
      // Slashes: six images sweep across the target from the user's side,
      // once per hit; the fifth image of each sweep is the hit.
      if (frame < span * 6) {
        s32 picture =
            frame / TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_IMAGE_FRAMES) % 6;

        // The picture's left edge stands Reach / 2 - 8 pixels right of the
        // target's centre, 21 for the first picture down to -8 for the last,
        // so the sweep crosses the centre; mirrored when the targets are the
        // party, its right edge stands as far to the left.
        if (work->effect->side == 1) {
          ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                            ANIM_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
          ANIM_BLITTERS[HEAP_SLOT_RAM_CODE](
              canvas, work->sheet + gTargetStrike_Offsets[picture],
              targetPosition.x / 2 - (gTargetStrike_Reach[picture] / 2) -
                  gTargetStrike_Widths[picture] + 8,
              targetPosition.y - (gTargetStrike_Heights[picture] / 2),
              gTargetStrike_Widths[picture], gTargetStrike_Heights[picture]);
        } else {
          ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, ANIM_CLIP_BOUNDS, BLITTER_MAX);
          ANIM_BLITTERS[HEAP_SLOT_RAM_CODE](
              canvas, work->sheet + gTargetStrike_Offsets[picture],
              targetPosition.x / 2 + (gTargetStrike_Reach[picture] / 2) - 8,
              targetPosition.y - (gTargetStrike_Heights[picture] / 2),
              gTargetStrike_Widths[picture], gTargetStrike_Heights[picture]);
        }
        FreeHeapSlot(HEAP_SLOT_RAM_CODE);
        if (frame %
                (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_IMAGE_FRAMES) * 6) ==
            TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_IMAGE_FRAMES) * 4) {
          if (mode == TARGET_STRIKE_STYLE_MARS_HEAVY_SLASH) {
            ANIM_SHOW_DAMAGE(SE_HEAVY_IMPACT);
          } else {
            Sound_PlaySongFar(SE_IMPACT);
            ANIM_SHOW_DAMAGE(SE_IMPACT);
          }
          // A heavy hit lights every spark at once; otherwise the hit
          // lights the next 32. Each hit also starts 8 smoke puffs, the
          // first half of its group of 16.
          if (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_FLAGS) & STRIKE_HEAVY) {
            work->shakeFrames = 8;
            ANIM_STRIKE_TARGET(work->effect, 0, 12, KNOCKBACK_HURL);
            for (i = 0; i != 512; i++)
              TARGET_STRIKE_SPARKS[i].variant = (Random16() & 15) + 15;
          } else {
            work->shakeFrames = 4;
            ObjectGroup_UpdateMembers(work->effect->targets[0],
                                      SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 8);
            for (i = 0; i != 32; i++) {
              struct Particle *step =
                  &TARGET_STRIKE_SPARKS[frame /
                                            (TARGET_STRIKE_VALUE(
                                                 mode,
                                                 TARGET_STRIKE_IMAGE_FRAMES) *
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
                                          mode, TARGET_STRIKE_IMAGE_FRAMES) *
                                      6) *
                                     16 +
                                 i];

            step->variant = 0;
          }
        }
      }
      // Slashes show their glints on frames 12 to 31; columns while stage 3
      // is drawn.
      if (frame >= 12 && frame <= 31)
        glint = 1;
    } else if (frame < length + 4) {
      // Columns: a column of light, drawn as two mirrored halves, rises
      // through its stages on the target once per hit, then holds stage 3
      // for 4 frames.
      s32 stage;

      if (frame < length) {
        s32 picture =
            frame / TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_IMAGE_FRAMES);

        // Pictures above 4 wrap back by 4, so each later hit reads pictures
        // 4, 1, 2 and 3 (gTargetStrike_ColumnStages).
        while (picture > 4)
          picture -= 4;
        stage = gTargetStrike_ColumnStages[picture];
      } else {
        stage = 3;
      }
      ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, ANIM_CLIP_BOUNDS,
                        TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_COMPOSITE));
      ANIM_BLITTERS[HEAP_SLOT_RAM_CODE](canvas,
                                        work->sheet + stage * (18 * 48) + 0xc80,
                                        targetPosition.x / 2 - 18, 56, 18, 48);
      FreeHeapSlot(HEAP_SLOT_RAM_CODE);
      ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, ANIM_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                        TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_COMPOSITE));
      ANIM_BLITTERS[HEAP_SLOT_RAM_CODE](canvas,
                                        work->sheet + stage * (18 * 48) + 0xc80,
                                        targetPosition.x / 2, 56, 18, 48);
      FreeHeapSlot(HEAP_SLOT_RAM_CODE);
      if (frame % (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_IMAGE_FRAMES) * 4) ==
          TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_IMAGE_FRAMES) * 3) {
        // A hit: the target flashes; the last hit lets the battle show the
        // damage, the others only play the hit sound. Each hit lights a
        // group of 64 sparks, 8 smoke puffs and 16 motes.
        // XXX: the group is frame / (6 * IMAGE_FRAMES), a slash's period,
        // though column hits are four images apart, so a three-hit column's
        // second and third hits light the same group; a capture of Acid Bite
        // would show whether the third hit throws anything new.
        ObjectGroup_UpdateMembers(work->effect->targets[0], SPRITE_COLOR_WHITE,
                                  POSE_DAMAGE, 0, 8);
        work->shakeFrames = 4;
        if (frame > (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_HITS) * 4 - 4) *
                        TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_IMAGE_FRAMES))
          ANIM_SHOW_DAMAGE(SE_IMPACT);
        else
          Sound_PlaySongFar(SE_IMPACT);
        for (i = 0; i != 64; i++) {
          struct Particle *step =
              &TARGET_STRIKE_SPARKS[frame /
                                        (TARGET_STRIKE_VALUE(
                                             mode, TARGET_STRIKE_IMAGE_FRAMES) *
                                         6) *
                                        64 +
                                    i];

          step->variant = (Random16() & 15) + 7;
        }
        for (i = 0; i != 8; i++) {
          struct Particle *step =
              &work->particles[frame /
                                   (TARGET_STRIKE_VALUE(
                                        mode, TARGET_STRIKE_IMAGE_FRAMES) *
                                    6) *
                                   16 +
                               i];

          step->variant = 0;
        }
        for (i = 0; i != 16; i++)
          PARTICLES[frame /
                        (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_IMAGE_FRAMES) *
                         6) *
                        16 +
                    i]
              .variant = 0;
      }
      if (stage == 3)
        glint = 1;
    }

    // Glints: three a frame, scattered at random around the target in
    // random flips; a blitter is built for one flip, so each glint builds
    // its own. radius + 4 is 4 to 35 and the >> 17 halves it, so a
    // glint lands 2 to 17 canvas pixels from the strike point each way,
    // twice that across on the wide screen.
    if ((TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_FLAGS) & STRIKE_GLINTS) &&
        glint) {
      for (i = 0; i != 3; i++) {
        s32 image = frame & 3;
        s32 angle = Random16() & 0xffff;
        s32 range = 31;
        s32 radius = Random16() & range;
        s32 x;
        s32 y;

        x = targetPosition.x / 2 + (Trig_Sin(angle) * (radius + 4) >> 17) -
            (gBattleAnim_GlintCellWidths[image] / 2);
        y = targetPosition.y - (Trig_Cos(angle) * (radius + 4) >> 17) -
            (gBattleAnim_GlintCellHeights[image] / 2);
        ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                          gTargetStrike_GlintDrawFlags[Random16() & 3] |
                              ANIM_CLIP_BOUNDS,
                          COMPOSITE_ADDITIVE);
        (*lower)(canvas, work->sheet + gBattleAnim_GlintCells[image], x, y,
                 gBattleAnim_GlintCellWidths[image],
                 gBattleAnim_GlintCellHeights[image]);
        FreeHeapSlot(HEAP_SLOT_RAM_CODE_2);
      }
    }

    ANIM_VIEW_FROM_CAMERA(camera);
    ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, ANIM_CLIP_BOUNDS, BLITTER_MAX);
    draw[0] = ANIM_BLITTERS[HEAP_SLOT_RAM_CODE];
    ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, ANIM_CLIP_BOUNDS,
                      COMPOSITE_ADDITIVE);
    draw[1] = *lower;
    // DEAD: runs once, for the user only; the ROM keeps the loop.
    for (k = 0; k != 1; k++) {
      source = GetBattleObjectSlotFar(work->effect->user)->object;
      // Smoke puffs rise and spread through six images over 24 frames,
      // pulled up by 1/64 pixel a frame each frame and slowed to 62/64.
      if (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_FLAGS) & STRIKE_SMOKE) {
        for (i = 0; i != 64; i++) {
          struct Particle *step = &work->particles[i];

          if (step->variant >= 0 && step->variant <= 23) {
            s32 picture = step->variant / 4;

            draw[i & 1](canvas, work->sheet + picture * (24 * 48) + wispSheet,
                        Q_16_16_TO_INT(step->x) - 12,
                        Q_16_16_TO_INT(step->y) - 24, 24, 48);
            Particle_AdvanceWithGravity2D(step, 62, -0x400);
            step->variant++;
          }
        }
      }
      // Motes burst out of the target, then after 10 frames are pulled
      // into the user, who glows from frame 55 to 90; a mote goes out when
      // it is within 16 pixels of the user across the ground (dx and dz are
      // 1/256 of the distance).
      if (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_FLAGS) & STRIKE_ABSORB) {
        s32 size = 3;

        if (mode == TARGET_STRIKE_STYLE_MARS_ADDITIVE_DRAIN_COLUMNS)
          size = 8;
        if (frame == 55)
          ObjectGroup_UpdateMembers(work->effect->user, SPRITE_COLOR_WHITE, -1,
                                    -1, 0);
        if (frame == 90)
          ObjectGroup_UpdateMembers(work->effect->user, 0, -1, -1, 0);
        for (i = 0; i != 64; i++) {
          struct Particle *step = &PARTICLES[i];

          if (step->variant >= 0) {
            Projection_Point(step, &position);
            position.x >>= 1;
            draw[1](canvas, particleSheet + gBattleAnim_MoteCells[size - 1],
                    position.x - ((u32)size >> 1), position.y - size, size,
                    size * 2);
            Particle_AdvanceWithGravity3D(step, 60, 0);
            step->variant++;
            if (step->variant > 10) {
              s32 dx = (source->x - step->x) >> 8;
              s32 dy = (source->y - step->y + 0x140000) >> 8;
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
      // Sparks fly out and shrink as their lives run out.
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
            ANIM_DRAW_MOTE(draw[1], canvas, particleSheet, position.x,
                           position.y, size);
            step->variant--;
          }
        }
      }
    }
    ANIM_RELEASE_BLITTERS();
    if (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_FLAGS) & STRIKE_HEAVY)
      Camera_ApplyShake(8, 8);
    else
      Camera_ApplyShake(2, 4);
    ANIM_END_FRAME(work);
  }

  // Finish: the blitters were freed each frame.
  Task_Remove(BattleAnim_ProcessPendingGraphicsTransfer);
  BattleAnim_EndCanvasLayer();
}

#undef TARGET_STRIKE_SPARKS
#undef TARGET_STRIKE_VALUE
#undef TARGET_STRIKE_SMOKE_CELLS

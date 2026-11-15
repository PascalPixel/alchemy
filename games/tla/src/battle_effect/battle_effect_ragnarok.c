/// @file
/// Ragnarok, and the great sword that falls onto the target.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

/// The falling sword's modes (BattleEffect_RunFallingSword).
enum FallingSwordMode {
  FALLING_SWORD_TITAN_BLADE,
  FALLING_SWORD_ACHERONS_GRIEF
};

#undef RESULTS_FRAME

/// The sheet holds the blade (64 x 128) and then the flash pictures.
#define FLASH_SHEET (64 * 128)

/// The sparks, in the map cell buffer.
#define SPARKS ((struct Particle *)Ram_MapCellBuffer)
#define SPARKS_PER_COLUMN 340
#define SPARKS_DRAWN 256
#define FLASHES_PER_COLUMN 16
#define FLASHES_DRAWN 12

#define BLADE_FRAMES 80
#define BLADE_SWING_FRAMES 10 ///< the blade comes down
#define BLADE_HOLD_FRAME 20   ///< then holds, then creeps on
#define BLADE_LAST_FRAME 39
#define FIRST_STRIKE_FRAME 16 ///< each column strikes 8 frames after the last
#define RESULTS_FRAME 40

/// A huge flaming blade swings down over the targets and light bursts over
/// them twice.
///
/// Abilities: Ragnarok.
void BattleEffect_RunRagnarok(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 col;
  u8 *cells;
  struct MotionObject *user;
  DrawRectangle draw[2];
  struct ScreenPoint target;
  s32 baseX;
  s32 frame;
  s32 i;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  cells = Ram_HeapSlots->battleSheet;

  // Open the canvas, pose the user and place the columns.
  user = BattleObject_GetFar(args->user)->object;
  BattleEffect_StartCanvasLayer(1);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  BattleEffect_FetchRectangleBlitters(args->side, draw);
  Actor_SelectAnimationFar(user, 2);
  Actor_SetAnimationSpeedFar(user, ANIMATION_STEP_NORMAL * 3);
  EffectPosition_ProjectBattler(args->targets[0], &target);
  if (args->side == EFFECT_SIDE_ENEMIES)
    baseX = target.x / 2 - 24;
  else
    baseX = target.x / 2 - 92;

  Resource_LoadAndDecompress((s32)&ResourceId_FlameBladeSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                             work->sheet + FLASH_SHEET, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, cells, 0, 0);

  // Each column's flashes (a ring, 2 pixels apart, living 25 to 32 frames)
  // and sparks (from y = 88, at random speeds and angles, living 32 to 39).
  col = 0;
  do {
    i = 0;
    do {
      struct Particle *flash = &work->particles[col * FLASHES_PER_COLUMN + i];
      s32 radius = i * 2;
      s32 angle = Random16() & TRIG_ANGLE_MASK;

      flash->x = Trig_Sin(angle) * radius;
      flash->y = -(Trig_Cos(angle) * radius);
      flash->variant = i / 2 + 25;
      i++;
    } while (i != FLASHES_PER_COLUMN);

    i = 0;
    do {
      struct Particle *spark = &SPARKS[col * SPARKS_PER_COLUMN + i];
      s32 speed;
      s32 angle;

      speed = (Random16() & 0x1ff) + 32;
      angle = Random16() & TRIG_ANGLE_MASK;
      spark->x = px(gRagnarok_StrikeColumns[col + args->side * 3] + baseX);
      spark->y = px(88);
      spark->velocityX = (Trig_Sin(angle) * speed) >> 6;
      spark->velocityY = -(Trig_Cos(angle) * speed * 2) >> 6;
      spark->variant = (Random16() & 7) + 32;
      i++;
    } while (i != SPARKS_PER_COLUMN);
    col++;
  } while (col != 3);

  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

  frame = 0;
  do {
    if (frame == 4)
      Sound_PlayFar(SE_POWER_UP);
    if (frame == 8)
      work->shakeFrames = frame;
    if (frame == 18)
      Sound_PlayFar(SE_CRASH);
    if (frame == RESULTS_FRAME)
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);

    // The blade: down 16 pixels a frame, held, then creeping on; nothing
    // below y = 104 is drawn.
    if (frame <= BLADE_LAST_FRAME) {
      s32 height;
      s32 x;
      s32 top;

      height = 128;
      if (args->side == EFFECT_SIDE_PARTY) {
        if (frame < BLADE_SWING_FRAMES) {
          x = frame * 10 - 8;
          top = frame * 16 - 128;
        } else if (frame > BLADE_HOLD_FRAME) {
          x = frame + 62;
          top = frame * 2 - 24;
        } else {
          x = 82;
          top = 16;
        }
      } else {
        if (frame < BLADE_SWING_FRAMES) {
          x = 128 - frame * 10;
          top = frame * 16 - 128;
        } else if (frame > BLADE_HOLD_FRAME) {
          x = 58 - frame;
          top = frame * 2 - 24;
        } else {
          x = 38;
          top = 16;
        }
      }
      if (top + 128 > 104)
        height = height - top - 24;
      if (height > 0)
        draw[0](canvas, work->sheet, x + baseX - 32, top, 64, height);
    }

    if (frame > FIRST_STRIKE_FRAME)
      BattleEffect_StepPaletteToResource((s32)&ResourceId_BlastSheet);

    col = 0;
    do {
      s32 start = col * 8 + FIRST_STRIKE_FRAME;

      // The strike: a shake, two frames of the big flash, then the ring.
      if (frame == start)
        work->shakeFrames = 12;
      if (frame >= start) {
        if (frame < start + 2)
          draw[0](canvas, work->sheet + FLASH_SHEET,
                  gRagnarok_StrikeColumns[col + args->side * 3] + baseX - 16,
                  56, FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
        i = 0;
        do {
          struct Particle *flash =
              &work->particles[col * FLASHES_PER_COLUMN + i];
          s32 x;
          s32 life;
          s32 y;

          y = Q_16_16_UPPER_HALF(flash->y);
          x = Q_16_16_UPPER_HALF(flash->x) +
              gRagnarok_StrikeColumns[col + args->side * 3] + baseX;
          life = flash->variant;
          if (life >= 0 && life <= 17)
            draw[0](canvas,
                    work->sheet + (gRagnarok_FlashPictures[life / 3] << 11) +
                        FLASH_SHEET,
                    x - 16, y + 56, FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
          life = flash->variant;
          flash->variant = life > 0 ? life - 1 : -1;
          i++;
        } while (i != FLASHES_DRAWN);
      }

      // The sparks, from six frames after the strike, bouncing at y = 108.
      if (frame > start + 5) {
        i = 0;
        do {
          struct Particle *spark = &SPARKS[col * SPARKS_PER_COLUMN + i];

          if (spark->variant > 0) {
            s32 x;
            s32 y;
            s32 size;

            BattleEffectKernels_IntegrateVector2(spark, PARTICLE_DAMPING_NONE,
                                                 px(1) / 16);
            spark->variant--;
            y = spark->y;
            if (y > px(108)) {
              spark->velocityY = -spark->velocityY / 2;
            } else {
              x = spark->x;
              if ((x >= 0 && x <= 0x7effff) && y >= 0) {
                u8 *picture;
                s32 lane;
                s32 py = Q_16_16_TO_INT(y);
                s32 px = Q_16_16_TO_INT(x);

                size = spark->variant / 5 + 1;
                lane = i & 1;
                picture = cells + gBattleEffect_MotePictures[size - 1];
                px -= size / 2;
                py -= size;
                draw[lane](canvas, picture, px, py, size, size * 2);
              }
            }
          }
          i++;
        } while (i != SPARKS_DRAWN);
      }

      // The column's targets flash at the strike and are struck by its
      // sparks.
      for (i = 0; i != args->targetCount; i++) {
        if (frame == start)
          BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, i, 10);
        if (frame == start + 6) {
          EFFECT_STRIKE_TARGET(args, i, 10, KNOCKBACK_HURL);
        }
      }
      col++;
    } while (col != 2);

    BattleCamera_Shake(16, 16);
    EFFECT_END_FRAME(work);
    frame++;
  } while (frame != BLADE_FRAMES);

  Actor_SetAnimationSpeedFar(user, ANIMATION_STEP_NORMAL);
  EFFECT_FINISH_CANVAS();
}

/// Abilities: Titan Blade.
void BattleEffect_RunTitanBlade(struct BattleEffectParams *args) {
  BattleEffect_RunFallingSword(args, FALLING_SWORD_TITAN_BLADE);
}

/// Abilities: Acheron's Grief.
void BattleEffect_RunAcheronsGrief(struct BattleEffectParams *args) {
  BattleEffect_RunFallingSword(args, FALLING_SWORD_ACHERONS_GRIEF);
}

#undef FLASH_SHEET

// The sheet: the fire pillar, the flashes after it, the sword far after.
#define FLASH_SHEET (221 << 4)
#define SWORD_SHEET 20000

/// The map cell buffer: 684 motes of dust, then the sword's sparks.
#define DUST ((struct Particle *)Ram_MapCellBuffer)
#define SPARK                                                                  \
  ((struct Particle *)(Ram_MapCellBuffer + 684 * sizeof(struct Particle)))

/// The sword strikes: the crash, the damage and the canvas filled bright.
#define TITAN_BLADE_STRIKE_FRAME 60

/// A great sword falls from the sky onto the target, a pillar of fire and dust
/// spouting under it, and drives into the ground in a burst of flashes and
/// sparks.
///
/// The sword comes down 16 pixels a frame for 10 frames, holds, sinks slowly
/// from 33 and plunges after 60; a fountain of dust at frame 28 and 16 more a
/// frame from 32 to 63, while the pillar rises; the flashes from 56 and the
/// sparks from 60, bouncing on the ground; the canvas floods at frames 9 and
/// 60; every target is struck at frame 68.
void BattleEffect_RunFallingSword(struct BattleEffectParams *args, s32 mode) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  DrawRectangle draw[2];
  u8 *cells;
  struct ScreenPoint target;
  s32 centerX;
  s32 step;
  s32 origin;
  s32 i;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  cells = Ram_HeapSlots->battleSheet;

  // Open the canvas on BG1 over the target and build the blitters.
  BattleEffect_StartCanvasLayer(1);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  EffectPosition_ProjectBattler(args->targets[0], &target);
  centerX = target.x / 2;

  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                      BLITTER_TRANSPARENT);
  draw[1] = (DrawRectangle)Ram_HeapSlots->ramCodePaired;

  if (mode != 2)
    Resource_LoadAndDecompress((s32)&ResourceId_SwordSheet,
                               work->sheet + SWORD_SHEET, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_FirePillarSheetA, work->sheet, 1,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                             work->sheet + FLASH_SHEET, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, cells, 0, 0);

  // Mode 1 turns the first 64 BG colours: red to blue, blue to green, green
  // to red.
  if (mode == FALLING_SWORD_ACHERONS_GRIEF) {
    u16 *color = (u16 *)BG_PLTT;

    for (i = 0; i != 64; i++) {
      s32 red = RGB_RED(*color);
      s32 green = RGB_GREEN(*color);
      s32 blue = RGB_BLUE(*color);

      *color++ = RGB(green, blue, red);
    }
  }

  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

  // 32 flashes on rings, two pixels apart.
  i = 0;
  do {
    struct Particle *flash = &work->particles[i];
    s32 scale = i * 2;
    s32 angle = Random16() & TRIG_ANGLE_MASK;

    flash->x = Trig_Sin(angle) * scale;
    flash->y = -(Trig_Cos(angle) * scale);
    flash->variant = i / 2 + 25;
    i++;
  } while (i != 32);

  i = 0;
  do {
    DUST[i].variant = -1;
    i++;
  } while (i != 171 << 2);

  // The sparks wait at the target's feet.
  i = 0;
  origin = px(centerX);
  do {
    struct Particle *spark = &SPARK[i];
    s32 speed;
    s32 angle;

    speed = 0x1FF;
    speed &= Random16();
    angle = Random16() & TRIG_ANGLE_MASK;
    spark->x = origin;
    spark->y = 176 << 15;
    spark->velocityX = (Trig_Sin(angle) * (speed + 32)) >> 5;
    spark->velocityY = -(Trig_Cos(angle) * (speed + 32)) >> 6;
    spark->variant = (Random16() & 7) + 32;
    i++;
  } while (i != 170 << 1);

  frame = 0;
  do {
    if (mode == FALLING_SWORD_TITAN_BLADE) {
      if (frame >= 41 && frame <= 63)
        BattleEffect_StepPaletteToResource((s32)&ResourceId_BlastSheet);
    } else if (mode == FALLING_SWORD_ACHERONS_GRIEF) {
      if (frame == 61) {
        Iwram_CopyWordsEntry(
            (void *)BG_PLTT,
            Resource_GetTableEntry((s32)&ResourceId_LightningBoltSheet), 128);
        Palette_MirrorBgColors();
      }
    }

    if (frame == 8)
      work->shakeFrames = frame;
    if (frame == 48)
      work->shakeFrames = 8;
    if (frame == TITAN_BLADE_STRIKE_FRAME)
      work->shakeFrames = 16;

    if (frame == 4)
      Sound_PlayFar(SE_POWER_UP);
    if (frame == 32)
      Sound_PlayFar(SE_SCATTER);
    if (frame == TITAN_BLADE_STRIKE_FRAME) {
      if (mode == FALLING_SWORD_ACHERONS_GRIEF)
        EFFECT_SHOW_DAMAGE(SE_CLATTER);
      else
        EFFECT_SHOW_DAMAGE(SE_CRASH);
    }

    // The flashes, from frame 56.
    if (frame > 55) {
      i = 0;
      do {
        struct Particle *flash = &work->particles[i];
        s32 x;
        s32 life;
        s32 y;

        y = Q_16_16_UPPER_HALF(flash->y);
        x = Q_16_16_UPPER_HALF(flash->x) + centerX;
        life = flash->variant;
        if (life >= 0 && life <= 17)
          draw[0](canvas,
                  work->sheet + (gTitanBlade_FlashPictures[life / 3] << 11) +
                      FLASH_SHEET,
                  x - 16, y + 48, FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
        life = flash->variant;
        flash->variant = life > 0 ? life - 1 : -1;
        i++;
      } while (i != 16);
    }

    // The dust: a fountain at frame 28, then 16 more a frame until 63.
    if (frame == 28) {
      i = 0;
      do {
        struct Particle *dust = &DUST[i];

        if (dust->variant == -1) {
          s32 speed;
          s32 angle;

          speed = Random16() & 63;
          angle = Random16() & TRIG_ANGLE_MASK;
          dust->x = ((Trig_Sin(angle) * speed) >> 3) + origin;
          dust->y = ((Trig_Cos(angle) * speed) >> 2) + (192 << 15);
          dust->velocityX = ((Random16() & 63) - 32) << 14;
          dust->velocityY = (-(Random16() & 63) - 8) << 13;
          dust->variant = 0;
        }
        i++;
      } while (i != 128 << 1);
    }

    step = frame - 32;
    if (step >= 0 && step <= 31) {
      s32 spawned;

      spawned = 0;
      i = 0;
      do {
        struct Particle *dust = &DUST[i];

        if (dust->variant == -1) {
          s32 speed;
          s32 angle;

          speed = Random16() & 63;
          angle = Random16() & TRIG_ANGLE_MASK;
          dust->x = ((Trig_Sin(angle) * speed) >> 3) + origin;
          dust->y = ((Trig_Cos(angle) * speed) >> 2) + (192 << 15);
          dust->velocityX = ((Random16() & 63) - 32) << 14;
          dust->velocityY = (-(Random16() & 63) - 8) << 13;
          dust->variant = 0;
          spawned++;
          if (spawned == 16)
            break;
        }
        i++;
      } while (i != 171 << 2);
    }

    if (step >= 0 && step <= 31) {
      s32 phase = frame * 16;
      s32 height = 104;
      u8 *src = work->sheet;
      s32 x = centerX - 17;
      DrawRectangle blit = draw[0];
      s32 size = (phase - 256) % height;

      blit(canvas, src, x, 4 - size, 34, height);
      blit(canvas, src, x, 108 - size, 34, size);
    }

    // The dust grows as it falls and toward the end.
    if (frame <= 71) {
      i = 0;
      do {
        struct Particle *dust = &DUST[i];

        if (dust->variant >= 0) {
          s32 size;

          size = i % 3 + 2;
          if (dust->velocityY > 0)
            size += 2;
          if (frame > 68 && size <= 5)
            size = 6;
          if (frame > 70 && size <= 6)
            size = 7;
          if (frame > 72 && size <= 7)
            size = 8;
          if (frame > 74 && size <= 8)
            size = 9;
          if (frame > 76)
            size = 10;
          draw[0](canvas, cells + gBattleEffect_MotePictures[size - 1],
                  Q_16_16_UPPER_HALF(dust->x) - size / 2,
                  Q_16_16_UPPER_HALF(dust->y) - size, size, size * 2);

          dust->x += dust->velocityX;
          dust->y += dust->velocityY;
          if (frame > 80)
            dust->velocityY += -32768;
          else
            dust->velocityY += gTitanBlade_DustGravity[i & 3];
          dust->velocityX = dust->velocityX * 62 / DAMPING_SCALE;
          dust->velocityY = dust->velocityY * 62 / DAMPING_SCALE;
          dust->variant++;
          if (dust->velocityY > 0 && Q_16_16_UPPER_HALF(dust->y) > 108)
            dust->variant = -1;
        }
        i++;
      } while (i != 171 << 1);
    }

    // The sword: down for 10 frames, held, sinking from 33, plunging after
    // 60; nothing below y = 108 is drawn.
    if (frame <= 95) {
      s32 x;
      s32 high;
      s32 top;

      x = centerX - 18;
      high = 120;
      if (frame > TITAN_BLADE_STRIKE_FRAME)
        top = frame * 8 - 450;
      else if (frame > 32)
        top = step / 2 + 16;
      else if (frame <= 9)
        top = frame * 16 - 128;
      else
        top = 16;
      if (top + 120 > 108)
        high = high - top - 12;
      if (high > 0)
        draw[1](canvas, work->sheet + SWORD_SHEET, x, top, 36, high);
    }

    // The sparks bounce on the ground.
    if (frame > 59) {
      i = 0;
      do {
        struct Particle *spark = &SPARK[i];

        if (spark->variant > 0) {
          s32 x;
          s32 y;
          s32 size;

          BattleEffectKernels_IntegrateVector2(spark, PARTICLE_DAMPING_NONE,
                                               128 << 6);
          y = spark->y;
          spark->variant--;
          if (y > (216 << 15)) {
            spark->velocityY = -spark->velocityY / 2;
          } else {
            x = spark->x;
            if ((x >= 0 && x <= 0x7effff) && y >= 0) {
              u8 *cell;

              size = spark->variant / 5 + 1;
              cell = cells + gBattleEffect_MotePictures[size - 1];
              x >>= 16;
              y >>= 16;
              x -= size / 2;
              y -= size;
              draw[0](canvas, cell, x, y, size, size * 2);
            }
          }
        }
        i++;
      } while (i != 170 << 1);
    }

    // The hit, and the canvas flooded at frames 9 and 60.
    if (frame == 68) {
      for (i = 0; i != args->targetCount; i++) {
        EFFECT_STRIKE_TARGET(args, i, 16, KNOCKBACK_JOLT);
      }
    }

    if (frame == 9) {
      BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                               POSE_DAMAGE, 0, 8);
      Iwram_FillWordsEntry(canvas, 128 << 7, 0x3F3F3F3F);
    }
    if (frame == TITAN_BLADE_STRIKE_FRAME)
      Iwram_FillWordsEntry(canvas, 128 << 7, 0x3F3F3F3F);

    BattleCamera_Shake(16, 16);
    EFFECT_END_FRAME(work);
    frame++;
  } while (frame != 102);

  EFFECT_FINISH_CANVAS();
}

struct StripQuad;

extern const struct Scale gJudgment_UnitScale;

/// Fills words through the IWRAM fill as one statement, the way Judgment clears
/// and floods its canvas.
static inline void BattleEffect_FillWords(void *dst, s32 size, s32 value) {
  do {
    Iwram_FillWordsEntry(dst, size, value);
  } while (0);
}
#define DRAW_ALT(mode, sheet, x, y, w, h)                                      \
  BattleEffect_LoadBlitter(HEAP_SLOT_RAM_CODE_PAIRED, mode);                   \
  ((DrawRectangle)Ram_HeapSlots->ramCodePaired)(canvas, sheet, x, y, w, h);    \
  HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED)
/// The same draw, the routine read through a pointer to its slot.
#define DRAW_ALT_THROUGH(slot, mode, sheet, x, y, w, h)                        \
  BattleEffect_LoadBlitter(HEAP_SLOT_RAM_CODE_PAIRED, mode);                   \
  (*(slot))(canvas, sheet, x, y, w, h);                                        \
  HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED)

// IO_WRITE_QUEUE_PUSH with an empty once-only block before the count store,
// the value store and the address store: the first tile queue matches only
// with each of the three parted from the one before.
#define QUEUE_PUSH_PARTED(address, value, control)                             \
  {                                                                            \
    struct IoWriteQueue *q = &gIoWriteQueue;                                   \
    u32 saved;                                                                 \
    s32 count;                                                                 \
                                                                               \
    IRQ_LOCK(saved);                                                           \
    count = *&q->count;                                                        \
    if (count <= 31) {                                                         \
      u32 *entry = q->entries[count];                                          \
                                                                               \
      do {                                                                     \
      } while (0);                                                             \
      *&q->count = count + 1;                                                  \
      do {                                                                     \
      } while (0);                                                             \
      *entry++ = (value);                                                      \
      do {                                                                     \
      } while (0);                                                             \
      *entry++ = (address);                                                    \
      *entry = (control);                                                      \
    }                                                                          \
    IRQ_UNLOCK(saved);                                                         \
  }

/// Hands back the buffer it is given.
static inline u8 *PassBuffer(u8 *buffer) {
  /* FAKEMATCH(wrapper): the second queue's source base passed through this
   * inline. */
  return buffer;
}

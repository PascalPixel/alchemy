/// @file
/// Ice and Douse: shards and bolts falling on the targets' side.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"

/// Fills window 0's per-line edges with the slanted band the shards and bolts
/// fall down.
#define FALLING_BUILD_SLANTED_WINDOW(line, i, last, slant, span)               \
  for ((i) = 0; (i) != DISPLAY_HEIGHT; (i)++) {                                \
    if ((i) >= 8 && (i) <= (last))                                             \
      (line)[i] = (slant);                                                     \
    else if ((i) <= 135)                                                       \
      (line)[i] = (span);                                                      \
    else                                                                       \
      (line)[i] = WINDOW_SPAN_INVERTED;                                        \
  }

// The falling shards (Ice) and bolts (Douse) rain diagonally onto the targets'
// side, seen only through a slanted band of window 0 whose edges an HBlank DMA
// feeds line by line; each lands just above line 80, shards shattering into
// sparkle and bolts throwing sparks.

enum { FALLING_SHARD_COUNT = 32, FALLING_SHARD_PUFF_COUNT = 32 };

/// A falling shard or bolt keeps falling while its y, in eighths of a pixel, is
/// at most this.
#define FALLING_LAND_Y (80 * 8 - 1)

/// Ice shards rain down diagonally onto the targets' side of the field and
/// shatter into puffs of sparkle, flashing and shaking each target as they
/// land.
///
/// Abilities: Ice, Ice Horn, Ice Missile.
void BattleEffect_RunIce(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  u8 *sparkles;
  BlitterPair draw;
  struct Particle *shard;
  struct Particle *puff;
  u16 *line;
  s32 i;
  s32 k;

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  sparkles = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
  work->args = args;
  EFFECT_START_NARROW_CANVAS(BGCNT_WRAP | BGCNT_PRIORITY(1));
  Resource_LoadAndDecompress(RESOURCE_ICE_SHARD_SHEET, work->sheet, 1, 1);
  Resource_LoadAndDecompress(RESOURCE_SPARKLE_DOTS, sparkles, 0, 0);
  BattleEffect_ConfigureEffectDisplay();
  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG1 |
              WININ_WIN1_OBJ | WININ_WIN1_CLR;

  {
    s32 y;
    s32 slant;

    for (i = 0, y = -128, slant = -16; i != FALLING_SHARD_COUNT;
         y -= 64, i++, slant -= 8) {
      s32 x;

      shard = &work->particles[i];
      x = (Random16() & 63) + (Random16() & 7) + 24;
      if (work->args->side == EFFECT_SIDE_PARTY)
        x = x + slant + 24;
      else
        x = x - slant + 80;
      shard->y = y;
      shard->x = x << 3;
      shard->variant = -1; // still falling
    }
  }
  for (i = 0; i != FALLING_SHARD_PUFF_COUNT; i++)
    work->particles[FALLING_SHARD_COUNT + i].variant = -1; // puff slot free

  if (work->args->side == EFFECT_SIDE_ENEMIES) {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, BLITTER_CLIP_Y, COMPOSITE_LIGHTEN);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, BLITTER_CLIP_Y,
                        COMPOSITE_ADDITIVE);
  } else {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, BLITTER_CLIP_Y | COMPOSITE_FLIP_X,
                        COMPOSITE_LIGHTEN);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, BLITTER_CLIP_Y | COMPOSITE_FLIP_X,
                        COMPOSITE_ADDITIVE);
  }
  draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  draw[1] = EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE_2];

  if (work->args->side == EFFECT_SIDE_ENEMIES) {
    line = (u16 *)Ram_MapGridCellBuffer;
    FALLING_BUILD_SLANTED_WINDOW(line, i, 103, (240 - i) | ((112 - i) << 8),
                                 WIN_RANGE(8, 136));
  } else {
    line = (u16 *)Ram_MapGridCellBuffer;
    FALLING_BUILD_SLANTED_WINDOW(line, i, 95, (i - 8 + 160) | ((i + 24) << 8),
                                 WIN_RANGE(120, 248));
  }

  Task_Add(BattleEffect_ArmWin0HBlankDma, TASK_PASS_VBLANK);
  work->transferMode = CANVAS_COPY_AND_FADE;
  if (work->args->variant == EFFECT_VARIANT_STRONGER)
    work->transferValue = CANVAS_FADE_THREE_QUARTERS;
  else
    work->transferValue = CANVAS_FADE_HALF;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK);

  for (frame = 0; frame != gFalling_IceCounts[work->args->variant][ICE_FRAMES];
       frame++) {
    if (frame == gFalling_IceCounts[work->args->variant][ICE_FRAMES] - 16)
      EFFECT_SHOW_DAMAGE(SE_WATER);

    // Shards fall down the slant, and on landing shatter, kicking up puffs.
    for (i = 0; i != gFalling_IceCounts[work->args->variant][ICE_SHARD_COUNT];
         i++) {
      shard = &work->particles[i];
      if (shard->variant == -1) {
        s32 x = shard->x / 8;
        s32 y = shard->y / 8;

        draw[work->args->variant == EFFECT_VARIANT_STRONGEST ? 1 : 0](
            canvas, work->sheet, x, y, 32, 32);
        if (shard->y <= FALLING_LAND_Y) { // above y 80
          if (work->args->side == EFFECT_SIDE_ENEMIES)
            shard->x -= 64;
          else
            shard->x += 64;
          shard->y += 64;
        } else {
          // Every target flashes blue as the shards land.
          if ((i & 3) == 0)
            Sound_PlayFar(SE_ICE);
          work->shakeFrames = 2;
          shard->variant = 0;
          EFFECT_FOR_EACH_TARGET(k, work->args)
            BattleEffect_TintBattler(work->args->targets[k],
                                     SPRITE_COLOR_BLUE_TINT, POSE_DAMAGE, k, 8);
        }
      }
      if (shard->variant != -1) {
        s32 x = shard->x / 8;
        s32 y = shard->y / 8;

        if (shard->variant >= 1 && shard->variant <= 14) {
          draw[work->args->variant == EFFECT_VARIANT_STRONGEST ? 1 : 0](
              canvas, work->sheet + 32 * 32 + ((shard->variant / 3) << 10), x,
              y, 32, 32);
        }
        if (shard->variant >= 9 && shard->variant <= 11) {
          for (k = 0; k != FALLING_SHARD_PUFF_COUNT; k++) {
            puff = &work->particles[FALLING_SHARD_COUNT + k];
            if (puff->variant == -1) {
              puff->variant = 18;
              puff->x = (((Random16() & 31) + shard->x / 8) << 3) + 8;
              puff->y = ((Random16() & 15) + shard->y / 8 - 15) << 3;
              break;
            }
          }
        }
        if (shard->variant <= 14)
          shard->variant++;
      }
    }

    // Puffs twinkle out.
    for (i = 0; i != FALLING_SHARD_PUFF_COUNT; i++) {
      puff = &work->particles[FALLING_SHARD_COUNT + i];
      if (puff->variant != -1) {
        if (puff->variant <= 17) {
          s32 image = puff->variant / 2;
          s32 x = puff->x / 8 - (gBattleEffect_SparkleSizes[image] >> 1);
          s32 y = puff->y / 8 - (gBattleEffect_SparkleSizes[image] >> 1);

          draw[work->args->variant == EFFECT_VARIANT_STRONGEST ? 1 : 0](
              canvas, sparkles + gBattleEffect_SparklePictures[image], x, y,
              gBattleEffect_SparkleSizes[image],
              gBattleEffect_SparkleSizes[image]);
        }
        if (puff->variant > -1)
          puff->variant--;
      }
    }
    BattleEffect_CountDownBattlerTints();
    BattleCamera_Shake(4, 4);
    EFFECT_PRESENT_CANVAS(work);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  Task_Remove(BattleEffect_ArmWin0HBlankDma);
  EFFECT_RELEASE_BLITTERS();
  BattleEffect_EndCanvasLayer();
  BattleEffect_ConfigureEffectDisplay();
}

enum { FALLING_BOLT_COUNT = 64, FALLING_BOLT_SPARK_COUNT = 512 };

/// Ram_MapGridCellBuffer as the falling bolts use it: window 0's table, then
/// the bolts' sparks.
struct BoltSparkBuffer {
  u16 lines[DISPLAY_HEIGHT]; ///< WIN0H for each line, read by the HBlank DMA
  struct Particle sparks[FALLING_BOLT_SPARK_COUNT]; ///< free while -1
};

/// Ram_MapGridCellBuffer seen as Douse's struct BoltSparkBuffer.
#define FALLING_BOLT_SPARKS ((struct BoltSparkBuffer *)Ram_MapGridCellBuffer)

#define DOUSE_FALLING_PICTURE (2 * MOTE_SET_BYTES)
#define DOUSE_FLASH_PICTURE (DOUSE_FALLING_PICTURE + 24 * 24)
#define DOUSE_BURST_PICTURE (DOUSE_FLASH_PICTURE + 24 * 24)

/// Blue bolts streak down diagonally onto the targets' side of the field, each
/// bursting into a fountain of falling sparks where it lands, while the targets
/// flash.
///
/// Abilities: Douse, Drench, Deluge.
void BattleEffect_RunDouse(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  DrawRectangle drawSpark;
  DrawRectangle drawBolt;
  struct Particle *bolt;
  struct Particle *spark;
  u16 *line;
  s32 i;
  s32 k;

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  work->args = args;
  EFFECT_START_NARROW_CANVAS(BGCNT_WRAP | BGCNT_PRIORITY(1));
  Resource_LoadAndDecompress(RESOURCE_BLUE_BEAM_SHEET,
                             work->sheet + DOUSE_FALLING_PICTURE, 1, 1);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_D, work->sheet, 0, 0);
  BattleEffect_ConfigureEffectDisplay();
  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG1 |
              WININ_WIN1_OBJ | WININ_WIN1_CLR;

  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, BLITTER_CLIP_Y, COMPOSITE_LIGHTEN);
  drawBolt = HEAP_SLOT_BLOCK(HEAP_SLOT_RAM_CODE);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, BLITTER_CLIP_Y, COMPOSITE_ADDITIVE);
  drawSpark = HEAP_SLOT_BLOCK(HEAP_SLOT_RAM_CODE_2);

  EFFECT_FREE_PARTICLES(i, FALLING_BOLT_SPARKS->sparks,
                        FALLING_BOLT_SPARK_COUNT);

  for (i = 0; i != FALLING_BOLT_COUNT; i++) {
    s32 x;
    s32 y;

    bolt = &work->particles[i];
    x = Random16() & 63;
    y = -(i * gFalling_Counts[work->args->variant][FALLING_BOLT_SPACING] + 16);
    if (work->args->side == EFFECT_SIDE_PARTY)
      x = x + y / 2 - 48;
    else
      x = x - y / 2 + 72;
    bolt->x = x << 3;
    bolt->y = y << 3;
    bolt->variant = -1; // still falling
  }

  if (work->args->side == EFFECT_SIDE_ENEMIES) {
    line = (u16 *)Ram_MapGridCellBuffer;
    FALLING_BUILD_SLANTED_WINDOW(
        line, i, 103, ((52 - i / 2) << 8) | (180 - i / 2), WIN_RANGE(0, 128));
  } else {
    line = (u16 *)Ram_MapGridCellBuffer;
    FALLING_BUILD_SLANTED_WINDOW(
        line, i, 103, ((i / 2 + 60) << 8) | (i / 2 + 188), WIN_RANGE(112, 240));
  }

  Task_Add(BattleEffect_ArmWin0HBlankDma, TASK_PASS_VBLANK);
  if (work->args->variant == EFFECT_VARIANT_BASIC) {
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  } else if (work->args->variant == EFFECT_VARIANT_STRONGER) {
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  } else {
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  }
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK);

  for (frame = 0;
       frame != gFalling_Counts[work->args->variant][FALLING_BOLT_DURATION];
       frame++) {
    if (frame ==
        gFalling_Counts[work->args->variant][FALLING_BOLT_DURATION] - 64)
      EFFECT_SHOW_DAMAGE(SE_WATER);

    // Bolts fall down the slant, then flash and burst.
    for (i = 0;
         i != gFalling_Counts[work->args->variant][FALLING_BOLT_WAVE_COUNT];
         i++) {
      s32 x;
      s32 y;

      bolt = &work->particles[i];
      x = bolt->x / 8;
      y = bolt->y / 8;
      if (bolt->variant == -1) {
        drawBolt(canvas, work->sheet + DOUSE_FALLING_PICTURE, x, y, 24, 24);
        if (bolt->y <= FALLING_LAND_Y) {
          if (work->args->side == EFFECT_SIDE_ENEMIES)
            bolt->x -= 32;
          else
            bolt->x += 32;
          bolt->y += 64;
        } else {
          // Sparks fly up from the landed bolt.
          bolt->variant = 0;
          for (k = 0; k != gFalling_Counts[work->args->variant]
                                          [FALLING_BOLT_SPARKS_PER_BOLT];
               k++) {
            spark = &FALLING_BOLT_SPARKS->sparks
                         [i * gFalling_Counts[work->args->variant]
                                             [FALLING_BOLT_SPARKS_PER_BOLT] +
                          k];
            spark->x = px(x + 12);
            spark->y = px(y);
            spark->velocityX = ((Random16() & 255) - 128) << 9;
            if (work->args->variant == EFFECT_VARIANT_STRONGEST)
              spark->velocityY = ((Random16() & 0x1ff) - 384) << 10;
            else
              spark->velocityY = ((Random16() & 255) - 255) << 10;
            spark->variant = (Random16() & 15) + 16;
          }
          // Every target flashes white as the bolts land.
          if ((i & 3) == 0)
            Sound_PlayFar(SE_WATER);
          EFFECT_FOR_EACH_TARGET(k, work->args)
            BattleEffect_TintBattler(work->args->targets[k], SPRITE_COLOR_WHITE,
                                     POSE_DAMAGE, k, 2);
        }
      } else {
        if (bolt->variant >= 0 && bolt->variant <= 3)
          drawBolt(canvas, work->sheet + DOUSE_FLASH_PICTURE, x, y, 24, 24);
        else if (bolt->variant <= 7)
          drawBolt(canvas, work->sheet + DOUSE_BURST_PICTURE, x - 9, y - 9, 42,
                   42);
        if (bolt->variant <= 14)
          bolt->variant++;
      }
    }
    // Sparks fall and shrink as they die.
    for (i = 0; i != FALLING_BOLT_SPARK_COUNT; i++) {
      spark = &FALLING_BOLT_SPARKS->sparks[i];
      if (spark->variant != -1) {
        s32 size = spark->variant + 1;

        if (size > 6)
          size = 6;
        EFFECT_DRAW_FLARE(drawSpark, canvas, work->sheet,
                          Q_16_16_TO_INT(spark->x), Q_16_16_TO_INT(spark->y),
                          size);
        Particle_AdvanceWithGravity2D(spark, 60, px(1) / 8);
        spark->variant--;
      }
    }
    EFFECT_END_FRAME(work);
  }

  Task_Remove(BattleEffect_ArmWin0HBlankDma);
  EFFECT_FINISH_CANVAS();
  BattleEffect_ConfigureEffectDisplay();
}

#undef FALLING_BUILD_SLANTED_WINDOW
#undef FALLING_LAND_Y
#undef FALLING_BOLT_SPARKS
#undef DOUSE_FALLING_PICTURE
#undef DOUSE_FLASH_PICTURE
#undef DOUSE_BURST_PICTURE

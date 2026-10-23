/// @file
/// Ice and Douse: shards and bolts falling on the targets' side.
// Part of battle_anim.c.

#include "../battle_anim_internal.h"

/// Fills window 0's per-scanline edges for the falling shards and bolts:
/// lines 8 to `last` take `slant`, an expression of the line `i`; the rest of
/// the battle view, down to line 135, takes the fixed `span`; and the lines
/// below it are empty. Every band the animations pass is 128 pixels wide.
#define FALLING_BUILD_SLANTED_WINDOW(line, i, last, slant, span)               \
  for ((i) = 0; (i) != 160; (i)++) {                                           \
    if ((i) >= 8 && (i) <= (last))                                             \
      (line)[i] = (slant);                                                     \
    else if ((i) <= 135)                                                       \
      (line)[i] = (span);                                                      \
    else                                                                       \
      (line)[i] = WINDOW_SPAN_INVERTED;                                        \
  }

// The falling shards (Ice) and bolts (Douse) rain diagonally onto the
// targets' side. Both show the canvas only inside a slanted band of window 0,
// whose edges BattleAnim_ArmWin0HBlankDma feeds line by line from a table at
// the start of Ram_MapGridCellBuffer. Each particle falls until it passes y 80,
// then lands: shards shatter and kick up sparkle puffs, bolts flash and throw a
// fountain of sparks. A falling particle's variant is -1 until it lands, then
// counts the frames since. Shards and bolts are positioned in eighths of a
// pixel, sparks in 16.16 fixed point.
//
// A WIN0H value is the left edge << 8 | the right edge, so a slant whose two
// edges both lose one per line moves the band one pixel left a line.

/// Ice keeps its shards in work->particles 0 to 31 and their puffs in 32
/// to 63.
enum { FALLING_SHARD_COUNT = 32, FALLING_SHARD_PUFF_COUNT = 32 };

/// A falling shard or bolt keeps falling while its y (in eighths of a pixel)
/// is at most this, just above 80 pixels.
#define FALLING_LAND_Y 0x27f

/// Ice shards rain down diagonally onto the targets' side of the field and
/// shatter into puffs of sparkle, flashing and shaking each target as they
/// land.
///
/// Abilities: Ice, Ice Horn, Ice Missile (BATTLE_ANIM_ICE).
///
/// The canvas shows only inside a slanted band of window 0, so the shards
/// stream out of the sky above the targets. effect->variant picks the size of
/// the volley from gFalling_IceCounts (shards and frames: 16 for 46 frames
/// for Ice, 20 for 52 for Ice Horn, 32 for 80 for Ice Missile); Ice Horn
/// leaves longer trails and Ice Missile draws additively. effect->side is 1
/// when the targets are the party, 0 when they are enemies.
void BattleAnim_RunIce(struct BattleAnimArgs *effect) {
  void **slots;
  void **cursor;
  struct BattleAnimWork *work;
  void *canvas;
  s32 frame;
  u8 *sparkles;
  BlitterPair draw;
  struct Particle *shard;
  struct Particle *puff;
  u16 *line;
  s32 i;
  s32 k;

  ANIM_GET_SLOTS(slots, cursor, work, canvas);
  sparkles = ANIM_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  work->effect = effect;
  // Setup: the narrow canvas, the shard sheet and the sparkle dots in the
  // sheet slot, then the display; window 1 hides BG2.
  ANIM_START_NARROW_CANVAS(BGCNT_WRAP | BGCNT_PRIORITY(1));
  Resource_LoadAndDecompress((s32)&ResourceId_IceShardSheet, work->sheet, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_SparkleDots, sparkles, 0, 0);
  BattleAnim_ConfigureEffectDisplay();
  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_WININ = ANIM_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG1 |
              WININ_WIN1_OBJ | WININ_WIN1_CLR;

  // Line the shards up above the screen, each one 8 pixels higher and
  // further back along the slant than the one before; x is a random 24 to
  // 94 shifted onto the targets' half.
  {
    s32 y;
    s32 slant;

    for (i = 0, y = -128, slant = -16; i != FALLING_SHARD_COUNT;
         y -= 64, i++, slant -= 8) {
      s32 x;

      shard = &work->particles[i];
      x = (Random16() & 63) + (Random16() & 7) + 24;
      if (work->effect->side == 1)
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

  // The shards fall towards the targets' side, so the blitters mirror them
  // when the targets are the party.
  if (work->effect->side == 0) {
    ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, BLITTER_CLIP_Y, BLITTER_MAX);
    ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, BLITTER_CLIP_Y, COMPOSITE_ADDITIVE);
  } else {
    ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, BLITTER_CLIP_Y | COMPOSITE_FLIP_X,
                      BLITTER_MAX);
    ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, BLITTER_CLIP_Y | COMPOSITE_FLIP_X,
                      COMPOSITE_ADDITIVE);
  }
  draw[0] = (DrawRectangle)gHeapSlots[HEAP_SLOT_RAM_CODE];
  draw[1] = ANIM_BLITTERS[HEAP_SLOT_RAM_CODE_2];

  // Window 0's edges, one per scanline: a 128-pixel band slanting one pixel
  // a line the way the shards fall (on line 8 from x 104 against the
  // enemies, moving left; from x 32 against the party, moving right), then a
  // fixed span down to line 135.
  if (work->effect->side == 0) {
    line = (u16 *)Ram_MapGridCellBuffer;
    FALLING_BUILD_SLANTED_WINDOW(line, i, 103, (0xf0 - i) | ((0x70 - i) << 8),
                                 WIN_RANGE(8, 136));
  } else {
    line = (u16 *)Ram_MapGridCellBuffer;
    FALLING_BUILD_SLANTED_WINDOW(line, i, 95, (i - 8 + 160) | ((i + 0x18) << 8),
                                 WIN_RANGE(120, 248));
  }

  Task_Add(BattleAnim_ArmWin0HBlankDma, ANIM_CALLBACK_ORDER);
  // Trails: Ice Horn keeps 75% of each pixel a frame, the others 50%.
  work->transferMode = CANVAS_COPY_AND_FADE;
  if (work->effect->variant == ANIM_VARIANT_STRONGER)
    work->transferValue = 75;
  else
    work->transferValue = 50;
  Task_Add(BattleAnim_ProcessPendingGraphicsTransfer, ANIM_CALLBACK_ORDER);

  for (frame = 0;
       frame != gFalling_IceCounts[work->effect->variant][ICE_FRAMES];
       frame++) {
    // Frame loop. 16 frames before the end, the battle shows the damage,
    // with sound 132 as each target is hit.
    if (frame == gFalling_IceCounts[work->effect->variant][ICE_FRAMES] - 16)
      ANIM_SHOW_DAMAGE(SE_132);

    // Shards: fall 8 pixels a frame down the slant; on landing, shatter
    // through five images, kicking up puffs in the middle of it.
    for (i = 0; i != gFalling_IceCounts[work->effect->variant][ICE_SHARD_COUNT];
         i++) {
      shard = &work->particles[i];
      if (shard->variant == -1) {
        s32 x = shard->x / 8;
        s32 y = shard->y / 8;

        draw[work->effect->variant == ANIM_VARIANT_STRONGEST ? 1 : 0](
            canvas, work->sheet, x, y, 32, 32);
        if (shard->y <= FALLING_LAND_Y) { // above y 80
          if (work->effect->side == 0)
            shard->x -= 64;
          else
            shard->x += 64;
          shard->y += 64;
        } else {
          // Landed: every fourth shard plays the impact sound, and every
          // target flashes blue in its damage pose for 8 frames.
          if ((i & 3) == 0)
            Sound_PlaySongFar(SE_ICE);
          work->shakeFrames = 2;
          shard->variant = 0;
          ANIM_FOR_EACH_TARGET(k, work->effect)
          ObjectGroup_UpdateMembers(work->effect->targets[k],
                                    SPRITE_COLOR_BLUE_TINT, POSE_DAMAGE, k, 8);
        }
      }
      if (shard->variant != -1) {
        s32 x = shard->x / 8;
        s32 y = shard->y / 8;

        // Shattering: frames 1 to 14 step through the sheet's 32x32 images
        // 1 to 5, three frames each (0x400 bytes a picture); frame 15 ends
        // it.
        if (shard->variant >= 1 && shard->variant <= 14) {
          draw[work->effect->variant == ANIM_VARIANT_STRONGEST ? 1 : 0](
              canvas, work->sheet + 0x400 + ((shard->variant / 3) << 10), x, y,
              32, 32);
        }
        // Frames 9 to 11 each start a puff in the first free slot, near
        // the shard.
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

    // Puffs: count down from 18, two frames on each of the nine sparkle
    // images (8 down to 0), centred on the puff's position.
    for (i = 0; i != FALLING_SHARD_PUFF_COUNT; i++) {
      puff = &work->particles[FALLING_SHARD_COUNT + i];
      if (puff->variant != -1) {
        if (puff->variant <= 17) {
          s32 image = puff->variant / 2;
          s32 x = puff->x / 8 - (gBattleAnim_PuffSizes[image] >> 1);
          s32 y = puff->y / 8 - (gBattleAnim_PuffSizes[image] >> 1);

          draw[work->effect->variant == ANIM_VARIANT_STRONGEST ? 1 : 0](
              canvas, sparkles + gBattleAnim_PuffCells[image], x, y,
              gBattleAnim_PuffSizes[image], gBattleAnim_PuffSizes[image]);
        }
        if (puff->variant > -1)
          puff->variant--;
      }
    }
    ObjectGroup_TickMemberTimers();
    // Shakes while work->shakeFrames, which each landing sets to 2, lasts.
    Camera_ApplyShake(4, 4);
    ANIM_PRESENT_FRAME(work);
  }

  // Finish: the callbacks before the blocks they read, in the reverse of
  // the order they were added.
  Task_Remove(BattleAnim_ProcessPendingGraphicsTransfer);
  Task_Remove(BattleAnim_ArmWin0HBlankDma);
  ANIM_RELEASE_BLITTERS();
  BattleAnim_EndCanvasLayer();
  BattleAnim_ConfigureEffectDisplay();
}

/// Douse's bolts are work->particles 0 to 63; their sparks follow window 0's
/// scanline table in Ram_MapGridCellBuffer.
enum { FALLING_BOLT_COUNT = 64, FALLING_BOLT_SPARK_COUNT = 512 };

/// Ram_MapGridCellBuffer as the falling bolts use it: window 0's scanline
/// table, then the bolts' sparks.
struct BoltSparkBuffer {
  u16 lines[160]; ///< WIN0H for each screen line, read by the HBlank DMA
  struct Particle sparks[FALLING_BOLT_SPARK_COUNT]; ///< free while -1
};

/// Ram_MapGridCellBuffer seen as Douse's struct BoltSparkBuffer.
#define FALLING_BOLT_SPARKS ((struct BoltSparkBuffer *)Ram_MapGridCellBuffer)

#define DOUSE_FALLING_CELL 0x604 ///< the falling bolt picture in the sheet
#define DOUSE_FLASH_CELL 0x844   ///< the landing flash picture in the sheet
#define DOUSE_BURST_CELL 0xa84   ///< the 42x42 burst picture in the sheet

/// Blue bolts streak down diagonally onto the targets' side of the field, each
/// bursting into a fountain of falling sparks where it lands, while the targets
/// flash.
///
/// Abilities: Douse, Drench, Deluge (BATTLE_ANIM_DOUSE).
///
/// Like the falling shards, the canvas shows only inside a slanted band of
/// window 0. effect->variant picks the volley from gFalling_Counts: 6 bolts of
/// 8 sparks (Douse), 8 of 16 (Drench) or 32 of 4 (Deluge), spaced 32, 24 or 8
/// pixels apart, over 80 frames; it also picks how the canvas fades (see
/// below). effect->side is 1 when the targets are the party, 0 when they are
/// enemies.
void BattleAnim_RunDouse(struct BattleAnimArgs *effect) {
  void **slots;
  void **cursor;
  struct BattleAnimWork *work;
  void *canvas;
  s32 frame;
  DrawRectangle drawSpark;
  DrawRectangle drawBolt;
  struct Particle *bolt;
  struct Particle *spark;
  u16 *line;
  s32 i;
  s32 k;

  ANIM_GET_SLOTS(slots, cursor, work, canvas);
  work->effect = effect;
  // Setup: the bolt pictures go after the spark cells, which the particle
  // sheet decodes at the start of work->sheet; window 1 hides BG2.
  ANIM_START_NARROW_CANVAS(BGCNT_WRAP | BGCNT_PRIORITY(1));
  Resource_LoadAndDecompress((s32)&ResourceId_BlueBeamSheet,
                             work->sheet + DOUSE_FALLING_CELL, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD, work->sheet, 0,
                             0);
  BattleAnim_ConfigureEffectDisplay();
  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_WININ = ANIM_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG1 |
              WININ_WIN1_OBJ | WININ_WIN1_CLR;

  ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, BLITTER_CLIP_Y, BLITTER_MAX);
  drawBolt = ANIM_SLOT(slots, HEAP_SLOT_RAM_CODE);
  ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, BLITTER_CLIP_Y, COMPOSITE_ADDITIVE);
  drawSpark = ANIM_SLOT(slots, HEAP_SLOT_RAM_CODE_2);

  ANIM_FREE_PARTICLES(i, FALLING_BOLT_SPARKS->sparks, FALLING_BOLT_SPARK_COUNT);

  // Line the bolts up above the screen along the slant, FALLING_BOLT_SPACING
  // apart; all 64 are placed, though a variant drops only its first
  // FALLING_BOLT_WAVE_COUNT.
  for (i = 0; i != FALLING_BOLT_COUNT; i++) {
    s32 x;
    s32 y;

    bolt = &work->particles[i];
    x = Random16() & 63;
    y = -(i * gFalling_Counts[work->effect->variant][FALLING_BOLT_SPACING] +
          16);
    if (work->effect->side == 1)
      x = x + y / 2 - 48;
    else
      x = x - y / 2 + 72;
    bolt->x = x << 3;
    bolt->y = y << 3;
    bolt->variant = -1; // still falling
  }

  // Window 0's edges, one per scanline: a 128-pixel band slanting half a
  // pixel a line, as the bolts fall, then a fixed span down to line 135.
  if (work->effect->side == 0) {
    line = (u16 *)Ram_MapGridCellBuffer;
    FALLING_BUILD_SLANTED_WINDOW(line, i, 103,
                                 ((0x34 - i / 2) << 8) | (0xb4 - i / 2),
                                 WIN_RANGE(0, 128));
  } else {
    line = (u16 *)Ram_MapGridCellBuffer;
    FALLING_BUILD_SLANTED_WINDOW(
        line, i, 103, ((i / 2 + 60) << 8) | (i / 2 + 188), WIN_RANGE(112, 240));
  }

  Task_Add(BattleAnim_ArmWin0HBlankDma, ANIM_CALLBACK_ORDER);
  // Douse clears the canvas every frame; Drench keeps half of each pixel and
  // Deluge three quarters, for trails.
  if (work->effect->variant == ANIM_VARIANT_BASIC) {
    ANIM_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  } else if (work->effect->variant == ANIM_VARIANT_STRONGER) {
    ANIM_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, 50);
  } else {
    ANIM_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);
  }
  Task_Add(BattleAnim_ProcessPendingGraphicsTransfer, ANIM_CALLBACK_ORDER);

  for (frame = 0;
       frame != gFalling_Counts[work->effect->variant][FALLING_BOLT_DURATION];
       frame++) {
    // Frame loop. 64 frames before the end, the battle shows the damage,
    // with sound 132 as each target is hit.
    if (frame ==
        gFalling_Counts[work->effect->variant][FALLING_BOLT_DURATION] - 64)
      ANIM_SHOW_DAMAGE(SE_132);

    // Bolts: fall 8 pixels a frame (4 across) until they pass y 80, then
    // flash for frames 0 to 3 and burst for 4 to 7; the count stops at 15.
    for (i = 0;
         i != gFalling_Counts[work->effect->variant][FALLING_BOLT_WAVE_COUNT];
         i++) {
      s32 x;
      s32 y;

      bolt = &work->particles[i];
      x = bolt->x / 8;
      y = bolt->y / 8;
      if (bolt->variant == -1) {
        drawBolt(canvas, work->sheet + DOUSE_FALLING_CELL, x, y, 24, 24);
        if (bolt->y <= FALLING_LAND_Y) {
          if (work->effect->side == 0)
            bolt->x -= 32;
          else
            bolt->x += 32;
          bolt->y += 64;
        } else {
          // Landed: throw this bolt's sparks out from its tip, up to 1
          // pixel a frame sideways and up at 0 to 4 (Deluge's from 6 up to 2
          // down, so some fall), each living 16 to 31 frames. Bolt i owns its
          // own run of sparks.
          bolt->variant = 0;
          for (k = 0; k != gFalling_Counts[work->effect->variant]
                                          [FALLING_BOLT_SPARKS_PER_BOLT];
               k++) {
            spark = &FALLING_BOLT_SPARKS->sparks
                         [i * gFalling_Counts[work->effect->variant]
                                             [FALLING_BOLT_SPARKS_PER_BOLT] +
                          k];
            spark->x = (x + 12) << 16;
            spark->y = y << 16;
            spark->velocityX = ((Random16() & 255) - 128) << 9;
            if (work->effect->variant == ANIM_VARIANT_STRONGEST)
              spark->velocityY = ((Random16() & 0x1ff) - 0x180) << 10;
            else
              spark->velocityY = ((Random16() & 255) - 255) << 10;
            spark->variant = (Random16() & 15) + 16;
          }
          // Every fourth bolt plays the hit sound, and every target
          // flashes white in its damage pose for 2 frames.
          if ((i & 3) == 0)
            Sound_PlaySongFar(SE_132);
          ANIM_FOR_EACH_TARGET(k, work->effect)
          ObjectGroup_UpdateMembers(work->effect->targets[k],
                                    SPRITE_COLOR_WHITE, POSE_DAMAGE, k, 2);
        }
      } else {
        if (bolt->variant >= 0 && bolt->variant <= 3)
          drawBolt(canvas, work->sheet + DOUSE_FLASH_CELL, x, y, 24, 24);
        else if (bolt->variant <= 7)
          drawBolt(canvas, work->sheet + DOUSE_BURST_CELL, x - 9, y - 9, 42,
                   42);
        if (bolt->variant <= 14)
          bolt->variant++;
      }
    }
    // Sparks: a dot that shrinks as its life runs out (size 6 down to 1),
    // falling under a gravity of 1/8 pixel a frame per frame, its speed
    // damped to 60/64.
    for (i = 0; i != FALLING_BOLT_SPARK_COUNT; i++) {
      spark = &FALLING_BOLT_SPARKS->sparks[i];
      if (spark->variant != -1) {
        s32 size = spark->variant + 1;

        if (size > 6)
          size = 6;
        ANIM_DRAW_FLARE(drawSpark, canvas, work->sheet,
                        Q_16_16_TO_INT(spark->x), Q_16_16_TO_INT(spark->y),
                        size);
        Particle_AdvanceWithGravity2D(spark, 60, 0x2000);
        spark->variant--;
      }
    }
    ANIM_END_FRAME(work);
  }

  // Finish: stop both transfers before releasing the blitters and canvas.
  Task_Remove(BattleAnim_ArmWin0HBlankDma);
  ANIM_FINISH_CANVAS();
  BattleAnim_ConfigureEffectDisplay();
}

#undef FALLING_BUILD_SLANTED_WINDOW
#undef FALLING_LAND_Y
#undef FALLING_BOLT_SPARKS
#undef DOUSE_FALLING_CELL
#undef DOUSE_FLASH_CELL
#undef DOUSE_BURST_CELL

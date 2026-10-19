/// @file
/// Ice and Douse: shards and bolts falling on the targets' side.
/// Part of effect.c, the battle effect object, compiled only through it.

#include "../effect_internal.h"

/// Fills window 0's per-scanline edges for the falling shards and bolts:
/// lines 8 to `last` take `slant`, an expression of the line `i`; the rest of
/// the battle view, down to line 135, takes the fixed `span`; and the lines
/// below it are empty. Every band the effects pass is 128 pixels wide.
#define FALLING_BUILD_SLANTED_WINDOW(line, i, last, slant, span)               \
  for ((i) = 0; (i) != 160; (i)++) {                                           \
    if ((i) >= 8 && (i) <= (last))                                             \
      (line)[i] = (slant);                                                     \
    else if ((i) <= 135)                                                       \
      (line)[i] = (span);                                                      \
    else                                                                       \
      (line)[i] = WINDOW_SPAN_INVERTED;                                        \
  }

/// Falling shards and bolts, streak volleys and the mote fountain: four effects
/// that throw many small pictures at the targets.
///
/// - The falling shards (Ice) and bolts (Douse) rain diagonally onto the
///   targets' side. Both show the canvas only inside a slanted band of
///   window 0, whose edges an HBlank DMA feeds line by line from a table at
///   the start of PARTICLES. Each particle falls until it passes y 80,
///   then lands: shards shatter and kick up sparkle puffs, bolts flash and
///   throw a fountain of sparks. A small timing table per effect picks the
///   count, spacing and duration from effect->variant.
/// - The streak volley fires three streaks of light from the user at each
///   target in turn; a settings row per style says whether the streak
///   animates and how a hit looks.
/// - The mote fountain bursts motes out of the user that float upwards and
///   wobble while each target reacts.
///
/// Particle slots: the shards use work->particles, 0 to 31 for the shards and
/// 32 to 63 for their puffs; the bolts use work->particles for the bolts and
/// Ram_MapCellBuffer, after the window table, for their sparks; the streaks use
/// work->particles, one per target and streak; the motes use PARTICLES. A
/// falling particle's variant is -1 until it lands, then counts the frames
/// since. Shards and bolts are positioned in eighths of a pixel; sparks,
/// streaks and motes in 16.16 fixed point.
enum { FALLING_SHARD_COUNT = 32, FALLING_SHARD_PUFF_COUNT = 32 };

/// A falling shard or bolt keeps falling while its y (in eighths of a pixel)
/// is at most this, just above 80 pixels.
#define FALLING_LAND_Y 0x27f

/// Ice shards rain down diagonally onto the targets' side of the field and
/// shatter into puffs of sparkle, flashing and shaking each target as they
/// land.
///
/// Abilities: Ice, Ice Horn, Ice Missile (kind 7).
///
/// The canvas shows only inside a slanted band of window 0, so the shards
/// stream out of the sky above the targets. effect->variant picks the size of
/// the volley from gFalling_IceCounts (shards and frames: 16 for 46 frames, 20
/// for 52, 32 for 80); variant 1 leaves longer trails and variant 2 draws
/// additively. effect->side is 1 when the targets are the party, 0 when they
/// are enemies.
void BattleFx_RunIce(struct BattleEffectArgument *effect) {
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  u8 *sparkles;
  BlitterPair draw;
  struct EffectStep *shard;
  struct EffectStep *puff;
  u16 *line;
  s32 i;
  s32 k;

  EFFECT_GET_SLOTS(slots, cursor, work, canvas);
  sparkles = EFFECT_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  work->effect = effect;
  // Load the shard and puff images before shaping their visible canvas.
  EFFECT_BEGIN_NARROW_CANVAS(BGCNT_WRAP | BGCNT_PRIORITY(1));
  Resource_LoadAndDecompress((s32)&ResourceId_IceShardSheet, work->sheet, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_SparkleDots, sparkles, 0, 0);
  BattleFx_ConfigureEffectDisplay();
  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  // window 1 hides BG2
  REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG1 |
              WININ_WIN1_OBJ | WININ_WIN1_CLR;

  // Line the shards up above the screen, each one 8 pixels higher and
  // further back along the slant than the one before.
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
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, COMPOSITE_CLIP_Y, COMPOSITE_MAX);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE, COMPOSITE_CLIP_Y,
                        COMPOSITE_ADDITIVE);
  } else {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X,
                        COMPOSITE_MAX);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                        COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X,
                        COMPOSITE_ADDITIVE);
  }
  draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
  /* FAKEMATCH: the slot read through union HeapState keeps its load and
   * spill before the next two instructions; as gWorkSlot[n] both move two
   * later (4 lines of tbs-en effect.s). */
  draw[1] = (DrawRectangle)((union HeapState *)gWorkSlot)
                ->slots[HEAP_SLOT_BLITTER_ALTERNATE];

  // Window 0's edges, one per scanline: a band slanting the way the shards
  // fall, then a fixed span down to line 135, and nothing below that.
  if (work->effect->side == 0) {
    line = (u16 *)Ram_MapCellBuffer;
    FALLING_BUILD_SLANTED_WINDOW(line, i, 103, (0xf0 - i) | ((0x70 - i) << 8),
                                 WIN_RANGE(8, 136));
  } else {
    line = (u16 *)Ram_MapCellBuffer;
    FALLING_BUILD_SLANTED_WINDOW(line, i, 95, (i - 8 + 160) | ((i + 0x18) << 8),
                                 WIN_RANGE(120, 248));
  }

  Scheduler_AddOrUpdateCallback(BattleFx_ArmWin0HBlankDma,
                                EFFECT_CALLBACK_ORDER);
  work->transferMode = CANVAS_COPY_AND_FADE;
  if (work->effect->variant == 1)
    work->transferValue = 75;
  else
    work->transferValue = 50;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer,
                                EFFECT_CALLBACK_ORDER);

  for (frame = 0;
       frame != gFalling_IceCounts[work->effect->variant][ICE_FRAMES];
       frame++) {
    // 16 frames before the end, the battle shows the damage, with sound
    // 132 as each target is hit.
    if (frame == gFalling_IceCounts[work->effect->variant][ICE_FRAMES] - 16)
      EFFECT_SHOW_DAMAGE(SE_132);

    // Shards: fall 8 pixels a frame down the slant; on landing, shatter
    // through five images, kicking up puffs in the middle of it.
    for (i = 0; i != gFalling_IceCounts[work->effect->variant][ICE_SHARD_COUNT];
         i++) {
      shard = &work->particles[i];
      if (shard->variant == -1) {
        s32 x = shard->x / 8;
        s32 y = shard->y / 8;

        draw[work->effect->variant == 2 ? 1 : 0](canvas, work->sheet, x, y, 32,
                                                 32);
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
            AudioCommand_PlayFar(SE_115);
          work->shakeFrames = 2;
          shard->variant = 0;
          EFFECT_FOR_EACH_TARGET(k, work->effect)
            ObjectGroup_UpdateMembers(work->effect->actors[k],
                                      SPRITE_COLOR_BLUE_TINT, POSE_DAMAGE, k,
                                      8);
        }
      }
      if (shard->variant != -1) {
        s32 x = shard->x / 8;
        s32 y = shard->y / 8;

        // Shattering: frames 1 to 14 step through the sheet's 32x32 images
        // 1 to 5.
        if (shard->variant >= 1 && shard->variant <= 14) {
          draw[work->effect->variant == 2 ? 1 : 0](
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
          s32 x = puff->x / 8 - (gBattleFx_PuffSizes[image] >> 1);
          s32 y = puff->y / 8 - (gBattleFx_PuffSizes[image] >> 1);

          draw[work->effect->variant == 2 ? 1 : 0](
              canvas, sparkles + gBattleFx_PuffCells[image], x, y,
              gBattleFx_PuffSizes[image], gBattleFx_PuffSizes[image]);
        }
        if (puff->variant > -1)
          puff->variant--;
      }
    }
    ObjectGroup_TickMemberTimers();
    Camera_ApplyShake(4, 4);
    EFFECT_PRESENT_FRAME(work);
  }

  // Tear down in the reverse order.
  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  Scheduler_RemoveCallback(BattleFx_ArmWin0HBlankDma);
  EFFECT_RELEASE_BLITTERS();
  BattleFx_EndCanvasLayer();
  BattleFx_ConfigureEffectDisplay();
}

/// The falling bolts keep their sparks after window 0's scanline table in
/// Ram_MapCellBuffer.
enum { FALLING_BOLT_COUNT = 64, FALLING_BOLT_SPARK_COUNT = 512 };

/// Ram_MapCellBuffer as the falling bolts use it: window 0's scanline table,
/// then the bolts' sparks.
struct BoltSparkBuffer {
  u16 lines[160];
  struct EffectStep sparks[FALLING_BOLT_SPARK_COUNT];
};

#define FALLING_BOLT_SPARKS ((struct BoltSparkBuffer *)Ram_MapCellBuffer)

#define DOUSE_FALLING_CELL 0x604 ///< the falling bolt picture in the sheet
#define DOUSE_FLASH_CELL 0x844   ///< the landing flash picture in the sheet
#define DOUSE_BURST_CELL 0xa84   ///< the 42x42 burst picture in the sheet

/// Blue bolts streak down diagonally onto the targets' side of the field, each
/// bursting into a fountain of falling sparks where it lands, while the targets
/// flash.
///
/// Abilities: Douse, Drench, Deluge (kind 9).
///
/// Like the falling shards, the canvas shows only inside a slanted band of
/// window 0. effect->variant picks the volley from gFalling_Counts: 6 bolts of
/// 8 sparks, 8 of 16 or 32 of 4, spaced 32, 24 or 8 pixels apart, over 80
/// frames; it also picks how the canvas fades (see below). effect->side is 1
/// when the targets are the party, 0 when they are enemies.
void BattleFx_RunDouse(struct BattleEffectArgument *effect) {
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  DrawRectangle drawSpark;
  DrawRectangle drawBolt;
  struct EffectStep *bolt;
  struct EffectStep *spark;
  u16 *line;
  s32 i;
  s32 k;

  EFFECT_GET_SLOTS(slots, cursor, work, canvas);
  work->effect = effect;
  // The beam and its sparks use separate images but the same clipped canvas.
  EFFECT_BEGIN_NARROW_CANVAS(BGCNT_WRAP | BGCNT_PRIORITY(1));
  Resource_LoadAndDecompress((s32)&ResourceId_BlueBeamSheet,
                             work->sheet + DOUSE_FALLING_CELL, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD, work->sheet, 0,
                             0);
  BattleFx_ConfigureEffectDisplay();
  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  // window 1 hides BG2
  REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG1 |
              WININ_WIN1_OBJ | WININ_WIN1_CLR;

  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, COMPOSITE_CLIP_Y, COMPOSITE_MAX);
  drawBolt = EFFECT_SLOT(slots, HEAP_SLOT_BLITTER);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE, COMPOSITE_CLIP_Y,
                      COMPOSITE_ADDITIVE);
  drawSpark = EFFECT_SLOT(slots, HEAP_SLOT_BLITTER_ALTERNATE);

  EFFECT_FREE_PARTICLES(i, FALLING_BOLT_SPARKS->sparks,
                        FALLING_BOLT_SPARK_COUNT);

  // Line the bolts up above the screen along the slant, FALLING_BOLT_SPACING
  // apart.
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

  // Window 0's edges, one per scanline: a band slanting the way the bolts
  // fall, then a fixed span down to line 135, and nothing below that.

  if (work->effect->side == 0) {
    line = (u16 *)Ram_MapCellBuffer;
    FALLING_BUILD_SLANTED_WINDOW(line, i, 103,
                                 ((0x34 - i / 2) << 8) | (0xb4 - i / 2),
                                 WIN_RANGE(0, 128));
  } else {
    line = (u16 *)Ram_MapCellBuffer;
    FALLING_BUILD_SLANTED_WINDOW(
        line, i, 103, ((i / 2 + 60) << 8) | (i / 2 + 188), WIN_RANGE(112, 240));
  }

  Scheduler_AddOrUpdateCallback(BattleFx_ArmWin0HBlankDma,
                                EFFECT_CALLBACK_ORDER);
  // Variant 0 clears the canvas every frame; the others leave trails.
  if (work->effect->variant == 0) {
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  } else if (work->effect->variant == 1) {
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, 50);
  } else {
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);
  }
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer,
                                EFFECT_CALLBACK_ORDER);

  for (frame = 0;
       frame != gFalling_Counts[work->effect->variant][FALLING_BOLT_DURATION];
       frame++) {
    // 64 frames before the end, the battle shows the damage, with sound
    // 132 as each target is hit.
    if (frame ==
        gFalling_Counts[work->effect->variant][FALLING_BOLT_DURATION] - 64)
      EFFECT_SHOW_DAMAGE(SE_132);

    // Bolts: fall 8 pixels a frame (4 across) until they pass y 80, then
    // burst and play a two-image flash.
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
          // Landed: throw this bolt's sparks out from its tip, mostly
          // upwards, each living 16 to 31 frames.
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
            if (work->effect->variant == 2)
              spark->velocityY = ((Random16() & 0x1ff) - 0x180) << 10;
            else
              spark->velocityY = ((Random16() & 255) - 255) << 10;
            spark->variant = (Random16() & 15) + 16;
          }
          // Every fourth bolt plays the hit sound, and every target
          // flashes white in its damage pose for 2 frames.
          if ((i & 3) == 0)
            AudioCommand_PlayFar(SE_132);
          EFFECT_FOR_EACH_TARGET(k, work->effect)
            ObjectGroup_UpdateMembers(work->effect->actors[k],
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
    // Sparks: a dot that shrinks as its life runs out, falling under
    // gravity.
    for (i = 0; i != FALLING_BOLT_SPARK_COUNT; i++) {
      spark = &FALLING_BOLT_SPARKS->sparks[i];
      if (spark->variant != -1) {
        s32 size = spark->variant + 1;

        if (size > 6)
          size = 6;
        EFFECT_DRAW_FLARE(drawSpark, canvas, work->sheet,
                          Q_16_16_TO_INT(spark->x), Q_16_16_TO_INT(spark->y),
                          size);
        EffectStep_AdvanceWithGravity2D(spark, 60, 0x2000);
        spark->variant--;
      }
    }
    EFFECT_END_FRAME(work);
  }

  // Stop both transfers before releasing the blitters and canvas.
  Scheduler_RemoveCallback(BattleFx_ArmWin0HBlankDma);
  EFFECT_FINISH_CANVAS();
  BattleFx_ConfigureEffectDisplay();
}

#undef FALLING_BUILD_SLANTED_WINDOW
#undef FALLING_LAND_Y
#undef FALLING_BOLT_SPARKS
#undef DOUSE_FALLING_CELL
#undef DOUSE_FLASH_CELL
#undef DOUSE_BURST_CELL

/// @file
/// The Boreas summon.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

/// The sparkles that trail the spray, after the spray in PARTICLES.
#define BOREAS_SPRAY_SPARKLES TRAILS

/// The pictures in the ice block sheet that this file draws; the tables in
/// effect_table.s give the rest.
enum IceBlockSheetPicture {
  /// After the three crystals and the four large shards.
  BOREAS_BIG_BLOCK = 24 * 48 + 24 * 38 + 20 * 26 + 12 * 10 + 11 * 16 + 8 * 14 +
      10 * 15,
  /// After the big block, the three spray pictures and the seven small shards.
  BOREAS_STREAK = BOREAS_BIG_BLOCK + 24 * 48 + 3 * (16 * 16) + 8 * 4 + 12 * 4 +
      5 * 3 + 7 * 4 + 7 * 6 + 6 * 12 + 7 * 10
};

/// gBoreas_Points has an x and a y for each crystal.
#define BOREAS_POINT_X(crystal) gBoreas_Points[crystal][0]
#define BOREAS_POINT_Y(crystal) gBoreas_Points[crystal][1]

#define BOREAS_OPENING_FRAMES 288 ///< frames of the crystals, block and spray
#define BOREAS_TORRENT_FRAMES 146 ///< frames of the torrent
/// The opening: the crystals crumble three times, the big ice block drops,
/// lands on Boreas's head and sinks, and the spray streams out.
#define BOREAS_CRUMBLE_A_FRAME 32
#define BOREAS_CRUMBLE_B_FRAME 64
#define BOREAS_CRUMBLE_C_FRAME 104
#define BOREAS_BLOCK_DROP_FRAME 160
#define BOREAS_BLOCK_LAND_FRAME 176
#define BOREAS_BLOCK_SINK_FRAME 208
#define BOREAS_SPRAY_FRAME 224
/// The torrent: crystals rise among the targets, then crash down as the
/// streak of ice falls.
#define BOREAS_TORRENT_RISE_FRAME 16
#define BOREAS_TORRENT_CRASH_FRAME 72
/// The second part: the crystals of the torrent in work->particles and the
/// sparkles among them in PARTICLES.
#define BOREAS_TORRENT_CRYSTALS 64
#define BOREAS_STREAK_WIDTH 40
#define BOREAS_STREAK_HEIGHT 80

/// The first part's particles in PARTICLES: the crystals of the mass, the
/// shards that fly from it in two batches, and the flecks of spray, with a
/// sparkle trailing each after them.
#define BOREAS_MASS_CRYSTALS 64
#define BOREAS_SHARDS 64
#define BOREAS_SPRAY 128
#define BOREAS_TORRENT_SPARKLES 64
#define BOREAS_CRYSTAL_HEIGHT 48
#define BOREAS_BIG_BLOCK_HEIGHT 48

/// The screen opens on Boreas standing in a mass of ice crystals over the
/// violet sky; the crystals break away, a big ice block drops onto its head
/// and a spray of ice streams out to the left. Then a torrent of ice crystals
/// sweeps down over the targets, crystals rise among them and burst, and a
/// streak of ice falls through them.
///
/// Abilities: Boreas.
void BattleEffect_RunBoreas(struct BattleEffectParams *args) {
  UNUSED s32 crystalPoints[32][2];
  struct SpritePlacement position;
  void *canvas;
  struct BattleEffectWork *work;
  s32 frame;
  DrawRectangle draw;
  DrawRectangle drawAlt;
  u8 *sparkleSheet;
  s32 edgeSpeed;
  s32 row;
  s32 count;
  s32 i;
  s32 j;
  s32 shakeX;
  s32 shakeY;
  s32 y;
  s32 dropFrame;
  struct Particle *spray;
  struct Particle *trail;
  struct Scale scale;

  canvas = Ram_HeapSlots->battleCanvas;
  work = Ram_HeapSlots->battleEffect;
  sparkleSheet = Ram_HeapSlots->battleSheet;
  edgeSpeed = 1;
  BattleEffect_StartCanvasLayer(0);
  BattleEffect_ConfigureEffectDisplay();
  PLTT_BACKDROP = 0;
  BG_PLTT_COLOR(0, 1) = 0;
  work->transferMode = CANVAS_COPY;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_CLR;
  BattleEffect_WipeCanvas(CANVAS_WIPE_ACROSS, CANVAS_WIPE_FILL);
  REG_WIN0H = WIN_RANGE(240, 240); // an empty window
  Resource_LoadAndDecompress((s32)&ResourceId_IceBlockSheet, work->sheet, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_SparkleDots, sparkleSheet, 0, 0);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  draw = (DrawRectangle)Ram_HeapSlots->ramCode;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                      BLITTER_TRANSPARENT);
  drawAlt = (DrawRectangle)Ram_HeapSlots->ramCodePaired;
  gProjection.centerY = SUMMON_PROJECTION_CENTER_Y;
  BattleEffect_SelectLivingTargets(work->args);
  wait(1);
  BattleBackground_LoadFar(1, (s32)&ResourceId_VioletSkyBackdrop, 0);
  BattleEffect_SpawnObjects(9, FORMATION_ICE_BLOCKS, 1);
  EFFECT_SHOW_WIDE_CANVAS(BLEND_WEIGHT_MAX);
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

  for (i = 0; i != BOREAS_MASS_CRYSTALS; i++) {
    PARTICLES[i].x = (Random16() & 63) + 32;
    PARTICLES[i].y = (Random16() & 31) + 120;
    PARTICLES[i].variant = -1;
  }

  // DEAD: a copy of gBoreas_Points into crystalPoints that nothing reads; the
  // ROM keeps the loop and its stores.
  // UB: the copy takes 32 rows from a table of 16, so rows 16 to 31 come from
  // the 32 bytes after the table.
#ifdef UBFIX
  for (i = 0; i != 16; i++) {
#else
  for (i = 0; i != 32; i++) {
#endif
    crystalPoints[i][0] = BOREAS_POINT_X(i);
    crystalPoints[i][1] = BOREAS_POINT_Y(i);
  }

  Sound_PlayFar(SE_RUMBLE);

  // First half: the crystals, the big ice block and the spray.
  for (frame = 0; frame != BOREAS_OPENING_FRAMES; frame++) {
    count = 16;
    if ((gInput.repeat & (KEY_A | KEY_B)) && frame > 16)
      break;

    // Open the window from the left with a ragged edge.
    if (frame >= 0 && frame < 16) {
      u16 *edge = (u16 *)Ram_MapCellBuffer;

      if (frame == 1) {
        u8 *inset = Ram_MapCellBuffer + 2;
        s32 line;

        for (line = 0; line != WINDOW_EDGE_INSET_LINES; line++)
          inset[line] = Random16() & 63;
        *edge = 0;
        Task_Add(BattleEffect_BuildWindowEdgeTable, TASK_PASS_FRAME);
      }
      *edge += edgeSpeed;
      edgeSpeed += 3;
      if (frame == 15)
        Task_Remove(BattleEffect_BuildWindowEdgeTable);
    }

    if (frame > 103)
      count = 0;
    else if (frame > 63)
      count = 6;
    else if (frame > 31)
      count = 10;

    if (frame <= 167) {
      shakeX = (Random16() & 3) - 1;
      shakeY = (Random16() & 3) - 1;
      gBgScroll[1].x = shakeX;
      gBgScroll[1].y = shakeY + BATTLE_BACKGROUND_SCROLL_Y;
    } else {
      shakeX = 0;
      shakeY = 0;
      gBgScroll[1].x = 0;
      gBgScroll[1].y = BATTLE_BACKGROUND_SCROLL_Y;
    }
    if (frame >= BOREAS_BLOCK_LAND_FRAME && frame < 180) {
      shakeX = -gBoreas_ShakeOffsets[frame - BOREAS_BLOCK_LAND_FRAME];
      shakeY = gBoreas_ShakeOffsets[frame - BOREAS_BLOCK_LAND_FRAME];
      gBgScroll[1].x = shakeX;
      gBgScroll[1].y = shakeY + BATTLE_BACKGROUND_SCROLL_Y;
    }

    for (i = 0; i != count; i++) {
      s32 picture = i % 3;

      draw(canvas, work->sheet + gBoreas_CrystalPictures[picture],
           BOREAS_POINT_X(i) - shakeX,
           BOREAS_POINT_Y(i) - gBoreas_CrystalHeights[picture] - shakeY,
           gBoreas_CrystalWidths[picture], gBoreas_CrystalHeights[picture]);
    }

    scale = gBoreas_UnitScale;
    if (frame == 174) {
      for (i = 0; i != 9; i++)
        ((struct SpriteAttributes *)work->sprites[i])->priority = 0;
    }
    if (frame > BOREAS_BLOCK_SINK_FRAME)
      Sprite_StartAnimationFar(work->sprites[2],
                               gBoreas_Animations[(frame / 4) & 3]);

    // DEAD: stores of the 1.0 the copy of gBoreas_UnitScale already holds; the
    // ROM keeps the stores.
    scale.x = Q16_ONE;
    scale.y = Q16_ONE;
    position.groundHeight = 0;
    position.height = px(255);
    for (row = 0; row != 3; row++) {
      for (j = 0; j != 3; j++) {
        position.x = px(j * 32 + 144 - shakeX);
        position.y = px(row * 32 + 76 - shakeY);
        Render_ApplyProjectedPlacementFar(work->sprites[row * 3 + j],
                                          &position.x, &scale.x, 0);
      }
    }

    // The big ice block falls onto Boreas's head and slowly sinks.
    dropFrame = frame - BOREAS_BLOCK_DROP_FRAME;
    if (dropFrame >= 0 && dropFrame < 158) {
      s32 sinkFrame = frame - BOREAS_BLOCK_SINK_FRAME;
      s32 blockX = 80;
      s32 blockY = 8;

      if (frame <= 175) {
        blockX = 96 - dropFrame;
        blockY = dropFrame * 4 - 56;
      } else if (frame > BOREAS_BLOCK_SINK_FRAME) {
        blockY = sinkFrame / 4 + 8;
      }
      drawAlt(canvas, work->sheet + BOREAS_BIG_BLOCK, blockX, blockY, 24,
              BOREAS_BIG_BLOCK_HEIGHT);
    }

    if (frame == BOREAS_CRUMBLE_A_FRAME)
      Sound_PlayFar(SE_HEAVY_IMPACT);
    if (frame == BOREAS_CRUMBLE_B_FRAME)
      Sound_PlayFar(SE_HEAVY_IMPACT);
    if (frame == BOREAS_CRUMBLE_C_FRAME)
      Sound_PlayFar(SE_HEAVY_IMPACT);
    if (frame == BOREAS_BLOCK_LAND_FRAME)
      Sound_PlayFar(SE_HEAVY_IMPACT);
    if (frame == 226)
      Sound_PlayFar(SE_CRASH);
    Transform_Reset();

    // Each time crystals crumble, shards fly up.
    if (frame == BOREAS_CRUMBLE_A_FRAME) {
      for (i = 0; i != 32; i++) {
        PARTICLES[i].x = px((Random16() & 31) + 68);
        PARTICLES[i].y = px((Random16() & 31) + 8);
        PARTICLES[i].velocityX = ((Random16() & 127) - 63) << 11;
        PARTICLES[i].velocityY = ((-Random16() & 127) - 64) << 11;
        PARTICLES[i].variant = (Random16() & 15) + 32;
      }
    }
    if (frame == BOREAS_CRUMBLE_B_FRAME) {
      for (i = 0; i != 32; i++) {
        PARTICLES[i + 32].x = px(Random16() % 48 + 60);
        PARTICLES[i + 32].y = px((Random16() & 31) + 52);
        PARTICLES[i + 32].velocityX = ((Random16() & 127) - 63) << 12;
        PARTICLES[i + 32].velocityY = ((-Random16() & 31) - 32) << 13;
        PARTICLES[i + 32].variant = (Random16() & 15) + 32;
      }
    }
    if (frame == BOREAS_CRUMBLE_C_FRAME) {
      for (i = 0; i != 32; i++) {
        PARTICLES[i].x = px((Random16() & 63) + 52);
        PARTICLES[i].y = px((Random16() & 31) + 72);
        PARTICLES[i].velocityX = ((Random16() & 127) - 63) << 11;
        PARTICLES[i].velocityY = ((-Random16() & 31) - 32) << 13;
        PARTICLES[i].variant = (Random16() & 15) + 32;
      }
    }

    if (frame >= BOREAS_CRUMBLE_A_FRAME && frame < BOREAS_BLOCK_SINK_FRAME) {
      for (i = 0; i != BOREAS_SHARDS; i++) {
        if (PARTICLES[i].variant >= 0) {
          s32 picture;

          if (frame > 191)
            picture = i % 7 + 4;
          else
            picture = i & 3;
          drawAlt(canvas, work->sheet + gBoreas_DebrisPictures[picture],
                  Q_16_16_UPPER_HALF(PARTICLES[i].x),
                  Q_16_16_UPPER_HALF(PARTICLES[i].y),
                  gBoreas_DebrisWidths[picture],
                  gBoreas_DebrisHeights[picture]);
          PARTICLES[i].x += PARTICLES[i].velocityX;
          PARTICLES[i].y += PARTICLES[i].velocityY;
          PARTICLES[i].velocityY += px(1) / 8;
        }
      }
    }

    // The spray streams out to the bottom left, leaving sparkles.
    if (frame > 223) {
      if (frame == BOREAS_SPRAY_FRAME) {
        for (i = 0; i != BOREAS_SPRAY; i++) {
          PARTICLES[i].x = px(72);
          PARTICLES[i].y = px(56);
          PARTICLES[i].velocityX = (-(Random16() & 127) - 64) << 11;
          PARTICLES[i].velocityY = ((Random16() & 127) + 16) << 11;
          PARTICLES[i].variant = Random16();
        }
      }
      for (i = 0; i != BOREAS_SPRAY; i++) {
        if (frame >= i / 4 + 224) {
          s32 picture = i % 3;

          if ((i & 1) == 0) {
            draw(canvas, work->sheet + gBoreas_SprayPictures[picture],
                 Q_16_16_UPPER_HALF(PARTICLES[i].x),
                 Q_16_16_UPPER_HALF(PARTICLES[i].y),
                 gBoreas_SprayWidths[picture], gBoreas_SprayHeights[picture]);
          }
          PARTICLES[i].x += PARTICLES[i].velocityX;
          PARTICLES[i].y += PARTICLES[i].velocityY;
          if ((Q_16_16_TO_INT(PARTICLES[i].x)) < -16 ||
              (Q_16_16_TO_INT(PARTICLES[i].y)) > 120) {
            PARTICLES[i].x = px(72);
            PARTICLES[i].y = px(56);
          }
          // DEAD: a fleck's variant, random from frame 224, is never read
          // (the second half sets 0 to 63 anew); the ROM keeps the count.
          PARTICLES[i].variant++;
        }
      }
      if (frame == 228) {
        for (i = 0; i != BOREAS_SPRAY; i++) {
          spray = &PARTICLES[i];
          trail = &BOREAS_SPRAY_SPARKLES[i];
          trail->x = spray->x;
          trail->y = spray->y;
          trail->variant = 0;
        }
      }
      for (i = 0, trail = BOREAS_SPRAY_SPARKLES, spray = PARTICLES;
           i != BOREAS_SPRAY; i++, trail++, spray++) {
        if (frame >= i + 228) {
          s32 picture = trail->variant / 2 % 9;
          s32 size = gBattleEffect_SparkleSizes[picture];

          draw(canvas, sparkleSheet + gBattleEffect_SparklePictures[picture],
               Q_16_16_UPPER_HALF(trail->x) - size / 2,
               Q_16_16_UPPER_HALF(trail->y) - size / 2, size, size);
          if (++trail->variant == 18) {
            trail->x = spray->x;
            trail->y = spray->y;
            trail->variant = 0;
          }
        }
      }
    }

    EFFECT_PRESENT_CANVAS(work);
  }

  // Second half: ice crystals rain down on the targets, and more rise among
  // them and burst.
  for (i = 0; i != 9; i++)
    Sprite_DestroyFar(work->sprites[i]);
  BattleEffect_SetupBlendedDisplay();
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  Sound_PlayFar(SOUND_FADE_OUT_ACTION_SE);

  for (i = 0; i != ARRAY_COUNT(work->particles); i++) {
    work->particles[i].x = px((Random16() & 127) + 64);
    work->particles[i].z = -px(16);
    work->particles[i].y = px(-(Random16() & 127) - 64);
    work->particles[i].velocityX = (-(Random16() & 63) - 127) << 12;
    work->particles[i].velocityY = ((Random16() & 63) + 127) << 12;
    work->particles[i].variant = 0;
  }
  for (i = 0; i != BOREAS_TORRENT_SPARKLES; i++) {
    PARTICLES[i].x = Random16() & 127;
    PARTICLES[i].y = (Random16() & 63) + i / 2;
    PARTICLES[i].variant = -i / 2;
  }

  for (frame = 0; frame != BOREAS_TORRENT_FRAMES; frame++) {
    count = 0;
    if (frame == 96)
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
    if (frame == BOREAS_TORRENT_RISE_FRAME)
      work->shakeFrames = 32;
    // Crystals rise among the targets.
    if (frame > BOREAS_TORRENT_RISE_FRAME) {
      count = (frame - BOREAS_TORRENT_RISE_FRAME) / 2;
      if (count > 16)
        count = 16;
    }
    if (frame >= 9 && frame < BOREAS_TORRENT_CRASH_FRAME && (frame & 3) == 0)
      Sound_PlayFar(SE_WATER);
    if (frame == BOREAS_TORRENT_CRASH_FRAME)
      Sound_PlayFar(SE_CRASH);
    // A streak of ice falls from the top right.
    if (frame > 64)
      draw(canvas, work->sheet + BOREAS_STREAK, (64 - frame) * 7 + 88,
           frame * 14 - 992, BOREAS_STREAK_WIDTH, BOREAS_STREAK_HEIGHT);
    if (frame <= 71) {
      for (i = 0; i != count; i++) {
        s32 picture = i % 3;

        draw(canvas, work->sheet + gBoreas_RisingCrystalPictures[picture],
             BOREAS_POINT_X(i) - 56,
             BOREAS_POINT_Y(i) - gBoreas_RisingCrystalHeights[picture],
             gBoreas_RisingCrystalWidths[picture],
             gBoreas_RisingCrystalHeights[picture]);
      }
    }
    if (frame == BOREAS_TORRENT_CRASH_FRAME) {
      for (i = 0; i != ARRAY_COUNT(work->particles); i++) {
        work->particles[i].x = px(BOREAS_POINT_X(i & 15) - 56);
        work->particles[i].y = px(BOREAS_POINT_Y(i & 15));
        work->particles[i].velocityX = ((Random16() & 127) - 63) << 13;
        work->particles[i].velocityY = (-(Random16() & 31) - 16) << 14;
      }
      // DEAD: always true inside the frame 72 test; the ROM keeps the second
      // compare.
      if (frame == BOREAS_TORRENT_CRASH_FRAME)
        work->shakeFrames = 4;
    }
    if (frame > 71) {
      count = 32;
      if (frame != BOREAS_TORRENT_CRASH_FRAME)
        count = BOREAS_TORRENT_CRYSTALS;
      for (i = 0; i != count; i++) {
        struct Particle *crystal = &work->particles[i];

        y = Q_16_16_UPPER_HALF(crystal->y);
        if (y <= 135) {
          s32 picture = i % 3;

          draw(canvas, work->sheet + gBoreas_FallPictures[picture],
               Q_16_16_UPPER_HALF(crystal->x), y - gBoreas_FallHeights[picture],
               gBoreas_FallWidths[picture], gBoreas_FallHeights[picture]);
          BattleEffectKernels_IntegrateVector2(crystal, PARTICLE_DAMPING_NONE,
                                               px(1));
          if (Q_16_16_UPPER_HALF(crystal->y) > 120 &&
              crystal->velocityY > px(8)) {
            crystal->velocityY = -crystal->velocityY / 4;
            crystal->y = px(120);
            work->shakeFrames = 1;
          }
        }
      }
    }
    // Crystals rain down.
    if (frame <= 71) {
      for (i = 0; i != ARRAY_COUNT(work->particles); i++) {
        draw(canvas, work->sheet, Q_16_16_UPPER_HALF(work->particles[i].x) - 12,
             Q_16_16_UPPER_HALF(work->particles[i].y) - 24, 24,
             BOREAS_CRYSTAL_HEIGHT);
        work->particles[i].x += work->particles[i].velocityX;
        work->particles[i].y += work->particles[i].velocityY;
        if ((Q_16_16_TO_INT(work->particles[i].y)) > 120 && frame <= 47) {
          work->particles[i].x = px((Random16() & 127) + 64);
          work->particles[i].y = -px(16);
        }
      }
    }

    // Each target turns blue and flinches in turn.
    for (i = 0; i != args->targetCount; i++) {
      if (frame == i * 8 + 32)
        BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_BLUE_TINT,
                                 POSE_DAMAGE, TINT_KEEP, 0);
    }

    if (frame > BOREAS_TORRENT_CRASH_FRAME) {
      for (i = 0; i != BOREAS_TORRENT_SPARKLES; i++) {
        if (PARTICLES[i].variant >= 0 && PARTICLES[i].variant < 18) {
          s32 picture = PARTICLES[i].variant / 2 % 9;
          s32 size = gBattleEffect_SparkleSizes[picture];

          draw(canvas, sparkleSheet + gBattleEffect_SparklePictures[picture],
               PARTICLES[i].x - size / 2, PARTICLES[i].y - size / 2, size,
               size);
        }
        if (++PARTICLES[i].variant == 18 && frame <= 127) {
          PARTICLES[i].x = Random16() & 127;
          PARTICLES[i].y = (Random16() & 63) + (frame - 54) / 2;
          PARTICLES[i].variant = 0;
        }
      }
    }

    if (frame >= BOREAS_TORRENT_CRASH_FRAME && frame < 80)
      BattleCamera_Shake(8, 8);
    else
      BattleCamera_Shake(2, 2);
    EFFECT_PRESENT_CANVAS(work);
  }

  EFFECT_RELEASE_BLITTERS();
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

#undef BOREAS_SPRAY_SPARKLES
#undef BOREAS_POINT_X
#undef BOREAS_POINT_Y
#undef PARTICLES

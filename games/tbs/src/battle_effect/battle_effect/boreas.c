/// @file
/// The Boreas summon.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"

/// The sparkles that trail the spray, after the 128 spray particles in
/// PARTICLES (28 bytes each, 3584 in all).
#define BOREAS_SPRAY_SPARKLES (PARTICLES + 128)

/// The pictures in the ice block sheet that this file draws, each starting
/// where the pictures before it end. The sheet opens with the tallest of the
/// three ice crystals, the one the falling crystals draw. The tables in
/// battle_effect_table.c read the rest: the three crystals, the shards and
/// the spray.
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

/// The screen opens on Boreas standing in a mass of ice crystals over the
/// violet sky; the crystals break away, a big ice block drops onto its head
/// and a spray of ice streams out to the left. Then a torrent of ice crystals
/// sweeps down over the targets, crystals rise among them and burst, and a
/// streak of ice falls through them.
///
/// Abilities: Boreas.
void BattleEffect_RunBoreas(struct BattleEffectParams *args) {
  static struct SpriteScale unitScale = {{{Q16_ONE, Q16_ONE}}};
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
  struct SpriteScale scale;

  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  sparkleSheet = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
  edgeSpeed = 1;
  work->args = args;
  BattleEffect_StartCanvasLayer(0);
  BattleEffect_ConfigureEffectDisplay();
  PLTT_BACKDROP = 0;
  *(u16 *)&BG_PLTT[1] = 0; // background palette 0, colour 1
  work->transferMode = CANVAS_COPY;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK);
  REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_CLR;
  BattleEffect_WipeCanvas(CANVAS_WIPE_ACROSS, CANVAS_WIPE_FILL);
  REG_WIN0H = WIN_RANGE(240, 240); // an empty window
  Resource_LoadAndDecompress((s32)&ResourceId_IceBlockSheet, work->sheet, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_SparkleDots, sparkleSheet, 0, 0);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_LIGHTEN);
  draw = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_TRANSPARENT);
  drawAlt = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_2);
  gProjection.centerY = 240;
  BattleEffect_SelectLivingTargets(work->args);
  wait(1);
  BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED,
                           (s32)&ResourceId_VioletSkyBackdrop, 0);
  // Boreas: nine sprites of SPRITE_BOREAS, sprite priority 1.
  BattleEffect_SpawnObjects(9, SPRITE_BOREAS, 1);
  EFFECT_SHOW_WIDE_CANVAS(16);
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

  // Mark the shards unthrown.
  for (i = 0; i != 64; i++) {
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

  // ------------------------------------------------------------------
  // First half: the crystals, the big ice block and the spray (A or B skips
  // to the second half).
  // ------------------------------------------------------------------
  for (frame = 0; frame != 288; frame++) {
    count = 16;
    if ((gKeysRepeat & (KEY_A | KEY_B)) && frame > 16) {
      break;
    }

    // Open the window from the left: its right edge speeds up, and each line
    // lags 0 to 63 pixels behind it.
    if (frame >= 0 && frame < 16) {
      u16 *edge = (u16 *)Ram_MapGridCellBuffer;
      if (frame == 1) {
        u8 *inset = Ram_MapGridCellBuffer + 2;
        s32 line;
        for (line = 0; line != 128; line++) {
          inset[line] = Random16() & 63;
        }
        *edge = 0;
        Task_Add(BattleEffect_BuildWindowEdgeTable, TASK_PASS_VBLANK);
      }
      *edge += edgeSpeed;
      edgeSpeed += 3;
      if (frame == 15) {
        Task_Remove(BattleEffect_BuildWindowEdgeTable);
      }
    }

    // The crystals still standing.
    if (frame > 103) {
      count = 0;
    } else if (frame > 63) {
      count = 6;
    } else if (frame > 31) {
      count = 10;
    }

    // Shake: random until frame 167, from the table on frames 176 to 179, when
    // the big block lands.
    if (frame <= 167) {
      shakeX = (Random16() & 3) - 1;
      shakeY = (Random16() & 3) - 1;
      gBgScroll[1].x = shakeX;
      gBgScroll[1].y = shakeY + 32;
    } else {
      shakeX = 0;
      shakeY = 0;
      gBgScroll[1].x = 0;
      gBgScroll[1].y = 32;
    }
    if (frame >= 176 && frame < 180) {
      shakeX = -gBoreas_ShakeOffsets[frame - 176];
      shakeY = gBoreas_ShakeOffsets[frame - 176];
      gBgScroll[1].x = shakeX;
      gBgScroll[1].y = shakeY + 32;
    }

    for (i = 0; i != count; i++) {
      s32 picture = i % 3;
      draw(canvas, work->sheet + gBoreas_CrystalPictures[picture],
           BOREAS_POINT_X(i) - shakeX,
           BOREAS_POINT_Y(i) - gBoreas_CrystalHeights[picture] - shakeY,
           gBoreas_CrystalWidths[picture], gBoreas_CrystalHeights[picture]);
    }

    // Lay out Boreas's 3x3 sprites 32 pixels apart, moved against the shake.
    scale = unitScale;
    if (frame == 174) {
      for (i = 0; i != 9; i++) {
        work->sprites[i]->image.priority = 0;
      }
    }
    if (frame > 208) {
      Sprite_StartAnimationFar(work->sprites[2],
                               gBoreas_Animations[(frame / 4) & 3]);
    }

    // DEAD: stores of the 1.0 the copy of unitScale already holds; the ROM
    // keeps the stores.
    scale.x = Q16_ONE;
    scale.y = Q16_ONE;
    position.groundHeight = 0;
    position.height = px(255);
    for (row = 0; row != 3; row++) {
      for (j = 0; j != 3; j++) {
        position.x = px(j * 32 + 144 - shakeX);
        position.y = px(row * 32 + 76 - shakeY);
        Actor_ApplyProjectedPlacementFar(work->sprites[row * 3 + j], &position,
                                         &scale, 0);
      }
    }

    // The big ice block falls from the top right onto Boreas's head on frames
    // 160 to 175 and sinks slowly from frame 208.
    dropFrame = frame - 160;
    if (dropFrame >= 0 && dropFrame < 158) {
      s32 sinkFrame = frame - 208;
      s32 blockX = 80;
      s32 blockY = 8;
      if (frame <= 175) {
        blockX = 96 - dropFrame;
        blockY = dropFrame * 4 - 56;
      } else if (frame > 208) {
        blockY = sinkFrame / 4 + 8;
      }
      drawAlt(canvas, work->sheet + BOREAS_BIG_BLOCK, blockX, blockY, 24, 48);
    }

    if (frame == 32) {
      Sound_PlayFar(SE_HEAVY_IMPACT);
    }
    if (frame == 64) {
      Sound_PlayFar(SE_HEAVY_IMPACT);
    }
    if (frame == 104) {
      Sound_PlayFar(SE_HEAVY_IMPACT);
    }
    if (frame == 176) {
      Sound_PlayFar(SE_HEAVY_IMPACT);
    }
    if (frame == 226) {
      Sound_PlayFar(SE_CRASH);
    }
    Transform_Reset();

    // Each time crystals crumble, throw up 32 shards.
    if (frame == 32) {
      for (i = 0; i != 32; i++) {
        PARTICLES[i].x = px((Random16() & 31) + 68);
        PARTICLES[i].y = px((Random16() & 31) + 8);
        PARTICLES[i].velocityX = ((Random16() & 127) - 63) << 11;
        PARTICLES[i].velocityY = ((-Random16() & 127) - 64) << 11;
        PARTICLES[i].variant = (Random16() & 15) + 32;
      }
    }
    if (frame == 64) {
      for (i = 0; i != 32; i++) {
        PARTICLES[i + 32].x = px(Random16() % 48 + 60);
        PARTICLES[i + 32].y = px((Random16() & 31) + 52);
        PARTICLES[i + 32].velocityX = ((Random16() & 127) - 63) << 12;
        PARTICLES[i + 32].velocityY = ((-Random16() & 31) - 32) << 13;
        PARTICLES[i + 32].variant = (Random16() & 15) + 32;
      }
    }
    if (frame == 104) {
      for (i = 0; i != 32; i++) {
        PARTICLES[i].x = px((Random16() & 63) + 52);
        PARTICLES[i].y = px((Random16() & 31) + 72);
        PARTICLES[i].velocityX = ((Random16() & 127) - 63) << 11;
        PARTICLES[i].velocityY = ((-Random16() & 31) - 32) << 13;
        PARTICLES[i].variant = (Random16() & 15) + 32;
      }
    }

    // The shards fly on frames 32 to 207: the four large pictures, then the
    // seven small ones from frame 192.
    if (frame >= 32 && frame < 208) {
      for (i = 0; i != 64; i++) {
        if (PARTICLES[i].variant >= 0) {
          s32 picture;
          if (frame > 191) {
            picture = i % 7 + 4;
          } else {
            picture = i & 3;
          }
          drawAlt(canvas, work->sheet + gBoreas_DebrisPictures[picture],
                  Q_16_16_TO_INT(PARTICLES[i].x),
                  Q_16_16_TO_INT(PARTICLES[i].y), gBoreas_DebrisWidths[picture],
                  gBoreas_DebrisHeights[picture]);
          PARTICLES[i].x += PARTICLES[i].velocityX;
          PARTICLES[i].y += PARTICLES[i].velocityY;
          PARTICLES[i].velocityY += px(1) / 8;
        }
      }
    }

    // Spray: 128 flecks of ice from (72, 56) towards the bottom left, leaving
    // sparkles behind them from frame 228.
    if (frame > 223) {
      if (frame == 224) {
        for (i = 0; i != 128; i++) {
          PARTICLES[i].x = px(72);
          PARTICLES[i].y = px(56);
          PARTICLES[i].velocityX = (-(Random16() & 127) - 64) << 11;
          PARTICLES[i].velocityY = ((Random16() & 127) + 16) << 11;
          PARTICLES[i].variant = Random16();
        }
      }
      for (i = 0; i != 128; i++) {
        if (frame >= i / 4 + 224) {
          s32 picture = i % 3;
          if ((i & 1) == 0) {
            draw(canvas, work->sheet + gBoreas_SprayPictures[picture],
                 Q_16_16_TO_INT(PARTICLES[i].x), Q_16_16_TO_INT(PARTICLES[i].y),
                 gBoreas_SprayWidths[picture], gBoreas_SprayHeights[picture]);
          }
          PARTICLES[i].x += PARTICLES[i].velocityX;
          PARTICLES[i].y += PARTICLES[i].velocityY;
          if ((PARTICLES[i].x >> 16) < -16 || (PARTICLES[i].y >> 16) > 120) {
            PARTICLES[i].x = px(72);
            PARTICLES[i].y = px(56);
          }
          // DEAD: a fleck's variant, random from frame 224, is never read
          // (the second half sets 0 to 63 anew); the ROM keeps the count.
          PARTICLES[i].variant++;
        }
      }
      if (frame == 228) {
        for (i = 0; i != 128; i++) {
          spray = &PARTICLES[i];
          trail = &BOREAS_SPRAY_SPARKLES[i];
          trail->x = spray->x;
          trail->y = spray->y;
          trail->variant = 0;
        }
      }
      for (i = 0, trail = BOREAS_SPRAY_SPARKLES, spray = PARTICLES; i != 128;
           i++, trail++, spray++) {
        if (frame >= i + 228) {
          s32 picture = trail->variant / 2 % 9;
          s32 size = gBattleEffect_SparkleSizes[picture];
          draw(canvas, sparkleSheet + gBattleEffect_SparklePictures[picture],
               Q_16_16_TO_INT(trail->x) - size / 2,
               Q_16_16_TO_INT(trail->y) - size / 2, size, size);
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

  // ------------------------------------------------------------------
  // Second half: ice crystals rain down on the targets, and more rise among
  // them and burst into bouncing crystals.
  // ------------------------------------------------------------------
  for (i = 0; i != 9; i++) {
    Sprite_DestroyFar(work->sprites[i]);
  }
  // Blend the canvas fully over the battle background, set the falling
  // crystals above the canvas to the right, and stagger the sparkles,
  // sparkle i waiting i / 2 frames (its negative variant) before it shows.
  BattleEffect_SetupBlendedDisplay();
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  Sound_PlayFar(SOUND_FADE_OUT_ACTION_SE);

  for (i = 0; i != 64; i++) {
    work->particles[i].x = px((Random16() & 127) + 64);
    work->particles[i].z = -px(16);
    work->particles[i].y = px(-(Random16() & 127) - 64);
    work->particles[i].velocityX = (-(Random16() & 63) - 127) << 12;
    work->particles[i].velocityY = ((Random16() & 63) + 127) << 12;
    work->particles[i].variant = 0;
  }
  for (i = 0; i != 64; i++) {
    PARTICLES[i].x = Random16() & 127;
    PARTICLES[i].y = (Random16() & 63) + i / 2;
    PARTICLES[i].variant = -i / 2;
  }

  for (frame = 0; frame != 146; frame++) {
    count = 0;
    if (frame == 96) {
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
    }
    if (frame == 16) {
      work->shakeFrames = 32;
    }
    // Raise one crystal every 2 frames from frame 18, up to 16.
    if (frame > 16) {
      count = (frame - 16) / 2;
      if (count > 16) {
        count = 16;
      }
    }
    if (frame >= 9 && frame < 72 && (frame & 3) == 0) {
      Sound_PlayFar(SE_WATER);
    }
    if (frame == 72) {
      Sound_PlayFar(SE_CRASH);
    }
    // From frame 65 a streak of ice falls diagonally from the top right, 7
    // pixels left and 14 down a frame.
    if (frame > 64) {
      draw(canvas, work->sheet + BOREAS_STREAK, (64 - frame) * 7 + 88,
           frame * 14 - 992, 40, 80);
    }
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
    if (frame == 72) {
      for (i = 0; i != 64; i++) {
        work->particles[i].x = px(BOREAS_POINT_X(i & 15) - 56);
        work->particles[i].y = px(BOREAS_POINT_Y(i & 15));
        work->particles[i].velocityX = ((Random16() & 127) - 63) << 13;
        work->particles[i].velocityY = (-(Random16() & 31) - 16) << 14;
      }
      // DEAD: always true inside the frame 72 test; the ROM keeps the second
      // compare.
      if (frame == 72) {
        work->shakeFrames = 4;
      }
    }
    // The burst crystals, 32 on the first frame and 64 after.
    if (frame > 71) {
      count = 32;
      if (frame != 72) {
        count = 64;
      }
      for (i = 0; i != count; i++) {
        struct Particle *crystal = &work->particles[i];
        y = Q_16_16_TO_INT(crystal->y);
        if (y <= 135) {
          s32 picture = i % 3;
          draw(canvas, work->sheet + gBoreas_FallPictures[picture],
               Q_16_16_TO_INT(crystal->x), y - gBoreas_FallHeights[picture],
               gBoreas_FallWidths[picture], gBoreas_FallHeights[picture]);
          Particle_AdvanceWithGravity2D(crystal, 64, px(1));
          if (Q_16_16_TO_INT(crystal->y) > 120 && crystal->velocityY > px(8)) {
            crystal->velocityY = -crystal->velocityY / 4;
            crystal->y = px(120);
            work->shakeFrames = 1;
          }
        }
      }
    }
    // The falling crystals: until frame 47, one that lands starts again at
    // the top.
    if (frame <= 71) {
      for (i = 0; i != 64; i++) {
        draw(canvas, work->sheet, Q_16_16_TO_INT(work->particles[i].x) - 12,
             Q_16_16_TO_INT(work->particles[i].y) - 24, 24, 48);
        work->particles[i].x += work->particles[i].velocityX;
        work->particles[i].y += work->particles[i].velocityY;
        if ((work->particles[i].y >> 16) > 120 && frame <= 47) {
          work->particles[i].x = px((Random16() & 127) + 64);
          work->particles[i].y = -px(16);
        }
      }
    }

    // Target i takes a blue tint and its damage pose on frame 32 + 8i.
    EFFECT_FOR_EACH_TARGET(i, work->args) {
      if (frame == i * 8 + 32) {
        BattleEffect_TintBattler(work->args->targets[i], SPRITE_COLOR_BLUE_TINT,
                                 POSE_DAMAGE, TINT_KEEP, 0);
      }
    }

    // Sparkles (from frame 73).
    if (frame > 72) {
      for (i = 0; i != 64; i++) {
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

    // The camera shakes hard while the crystals burst, gently otherwise.
    if (frame >= 72 && frame < 80) {
      BattleCamera_Shake(8, 8);
    } else {
      BattleCamera_Shake(2, 2);
    }
    EFFECT_PRESENT_CANVAS(work);
  }

  EFFECT_RELEASE_BLITTERS();
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

#undef BOREAS_SPRAY_SPARKLES
#undef BOREAS_POINT_X
#undef BOREAS_POINT_Y

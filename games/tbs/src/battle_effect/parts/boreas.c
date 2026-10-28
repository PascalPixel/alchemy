/// @file
/// The Boreas summon.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"

/// The smoke trails behind the spray, after the 128 spray particles in
/// PARTICLES (28 bytes each, 0xe00 in all).
#define BOREAS_ICE_BLOCK_TRAILS (PARTICLES + 128)
#define BOREAS_BIG_BLOCK_OFFSETS 0xc46      ///< the dropping 24x48 ice block
#define BOREAS_FALLING_BLOCK_OFFSETS 0x14f9 ///< the diagonal 40x80 ice block

/// gBoreas_Points has an x and a y for each rock.
#define BOREAS_POINT_X(rock) gBoreas_Points[rock][0]
#define BOREAS_POINT_Y(rock) gBoreas_Points[rock][1]

/// A window opens on rocks that crumble, and a big ice block drops in and
/// throws up spray; then ice blocks rain down on the targets, and boulders
/// crumble into bouncing chips.
///
/// Abilities: Boreas (SUMMON_EFFECT_BOREAS).
///
/// The longest of the effects, in two halves. The first (288 frames, which A
/// or B cuts short after frame 16) opens a window on a field of 16 rocks over
/// the violet sky and nine sprites; the rocks crumble in three
/// waves (frames 32, 64 and 104), throwing up chips, a big ice block drops in
/// on frames 160 to 175 and, from frame 224, bursts into spray trailed by
/// smoke. The second (146 frames) gives the battle background back and rains
/// ice blocks on the targets while boulders rise, crumble on frame 72 and
/// bounce on the ground; the targets tint blue from frame 32 and the damage
/// shows on frame 96.
///
/// Particles, by half: in the first, PARTICLES 0 to 63 are the chips (screen
/// pixels in 16.16; a negative variant is one not yet thrown), then 0 to 127
/// the spray, with its smoke at BOREAS_ICE_BLOCK_TRAILS; in the second,
/// work->particles 0 to 63 are the falling ice blocks and then the boulder
/// chips, and PARTICLES 0 to 63 the smoke puffs, in whole pixels.
///
/// Also clears background colours 0 and 1 for the wipe and then loads the
/// ice block palette over the first 64, writes the window registers and the
/// window-edge table at Ram_MapGridCellBuffer (through
/// BattleEffect_BuildWindowEdgeTable), gBgScroll[1] for the shake,
/// gProjection.centerY and work->shakeFrames; it releases the nine sprites
/// before the second half and its tasks and blitters at the end.
void BattleEffect_RunBoreas(struct BattleEffectParams *args) {
  // One to one; the frame loop copies it and then sets both factors to 1.0
  // again.
  static struct SpriteScale unitScale = {{{Q16_ONE, Q16_ONE}}};
  // A copy of gBoreas_Points that nothing reads. It takes 32 rows from a
  // table of 16, so rows 16 to 31 come from the 32 bytes after the table.
  UNUSED s32 rockPoints[32][2];
  struct SpritePlacement position;
  void *canvas;
  struct BattleEffectWork *work;
  s32 frame;
  DrawRectangle draw;
  DrawRectangle drawAlt;
  u8 *puffSheet;
  s32 edgeSpeed;
  s32 row;
  u32 *slots;
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

  slots = (u32 *)&gHeapSlots[HEAP_SLOT_BATTLE_CANVAS];
  canvas = (void *)slots[0];
  work = (struct BattleEffectWork *)CANVAS_SLOT(slots, HEAP_SLOT_BATTLE_EFFECT);
  puffSheet = (u8 *)CANVAS_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
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
  Resource_LoadAndDecompress((s32)&ResourceId_SparkleDots, puffSheet, 0, 0);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_LIGHTEN);
  draw = (DrawRectangle)CANVAS_SLOT(slots, HEAP_SLOT_RAM_CODE);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_TRANSPARENT);
  drawAlt = (DrawRectangle)CANVAS_SLOT(slots, HEAP_SLOT_RAM_CODE_2);
  gProjection.centerY = 240;
  BattleEffect_SelectLivingTargets(work->args);
  wait(1);
  BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED,
                           (s32)&ResourceId_VioletSkyBackdrop, 0);
  // The formation: nine sprites of SPRITE_BOREAS, sprite priority 1.
  BattleEffect_SpawnObjects(9, SPRITE_BOREAS, 1);
  EFFECT_SHOW_WIDE_CANVAS(16);
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

  // Mark the chips unthrown. Their positions here are never drawn: each chip
  // gets a new one when its wave throws it. The window's edge (from frame 0),
  // its insets and its table (frames 1 to 15,
  // BattleEffect_BuildWindowEdgeTable) are written over chips 0 to 16, the
  // first 0x1c2 bytes of the buffer; the first wave throws chips 0 to 31 anew
  // on frame 32, before any chip is drawn.
  for (i = 0; i != 64; i++) {
    PARTICLES[i].x = (Random16() & 63) + 32;
    PARTICLES[i].y = (Random16() & 31) + 120;
    PARTICLES[i].variant = -1;
  }

  for (i = 0; i != 32; i++) {
    rockPoints[i][0] = BOREAS_POINT_X(i);
    rockPoints[i][1] = BOREAS_POINT_Y(i);
  }

  Sound_PlayFar(SE_RUMBLE);

  //------------------------------------------------------------------
  // First half: the rocks, the big ice block and the spray (A or B skips
  // to the second half).
  //------------------------------------------------------------------
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

    // The rocks still standing: 16, then 10 from frame 32, 6 from frame 64
    // and none from frame 104, as each wave crumbles into chips.
    if (frame > 103) {
      count = 0;
    } else if (frame > 63) {
      count = 6;
    } else if (frame > 31) {
      count = 10;
    }

    // Shake: random until frame 167, from the table on frames 176 to 179,
    // when the big block lands. The battle background (BG1) sits 32 lines down,
    // and the rocks and sprites are drawn offset by the same amount, so they
    // shake with it.
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
      s32 cell = i % 3;
      draw(canvas, work->sheet + gBoreas_RockCells[cell],
           BOREAS_POINT_X(i) - shakeX,
           BOREAS_POINT_Y(i) - gBoreas_RockHeights[cell] - shakeY,
           gBoreas_RockWidths[cell], gBoreas_RockHeights[cell]);
    }

    // Lay out the 3x3 sprites 32 pixels apart, moved against the shake. They
    // come to the front on frame 174, and from frame 209 the third one
    // animates.
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

    scale.x = Q16_ONE;
    scale.y = Q16_ONE;
    position.groundHeight = 0;
    position.height = 255 << 16;
    for (row = 0; row != 3; row++) {
      for (j = 0; j != 3; j++) {
        position.x = (j * 32 + 144 - shakeX) << 16;
        position.y = (row * 32 + 76 - shakeY) << 16;
        Actor_ApplyProjectedPlacementFar(work->sprites[row * 3 + j], &position,
                                         &scale, 0);
      }
    }

    // The big ice block (24x48) falls from the top right on frames 160 to
    // 175 and sinks slowly from frame 208.
    dropFrame = frame - 160;
    if (dropFrame >= 0 && dropFrame < 158) {
      s32 sinkFrame = frame - 208;
      s32 cellPictureX = 80;
      s32 blockY = 8;
      if (frame <= 175) {
        cellPictureX = 96 - dropFrame;
        blockY = dropFrame * 4 - 56;
      } else if (frame > 208) {
        blockY = sinkFrame / 4 + 8;
      }
      drawAlt(canvas, work->sheet + BOREAS_BIG_BLOCK_OFFSETS, cellPictureX,
              blockY, 24, 48);
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

    // Each time rocks crumble, throw up 32 chips: the first and third waves
    // reuse chips 0 to 31, the second takes 32 to 63, and all fall at 1/8
    // pixel a frame per frame. The first wave's chips start at up to 2
    // pixels a frame either way; the later waves' rise at up to 4.
    if (frame == 32) {
      for (i = 0; i != 32; i++) {
        PARTICLES[i].x = ((Random16() & 31) + 68) << 16;
        PARTICLES[i].y = ((Random16() & 31) + 8) << 16;
        PARTICLES[i].velocityX = ((Random16() & 127) - 63) << 11;
        PARTICLES[i].velocityY = ((-Random16() & 127) - 64) << 11;
        PARTICLES[i].variant = (Random16() & 15) + 32;
      }
    }
    if (frame == 64) {
      for (i = 0; i != 32; i++) {
        PARTICLES[i + 32].x = (Random16() % 48 + 60) << 16;
        PARTICLES[i + 32].y = ((Random16() & 31) + 52) << 16;
        PARTICLES[i + 32].velocityX = ((Random16() & 127) - 63) << 12;
        PARTICLES[i + 32].velocityY = ((-Random16() & 31) - 32) << 13;
        PARTICLES[i + 32].variant = (Random16() & 15) + 32;
      }
    }
    if (frame == 104) {
      for (i = 0; i != 32; i++) {
        PARTICLES[i].x = ((Random16() & 63) + 52) << 16;
        PARTICLES[i].y = ((Random16() & 31) + 72) << 16;
        PARTICLES[i].velocityX = ((Random16() & 127) - 63) << 11;
        PARTICLES[i].velocityY = ((-Random16() & 31) - 32) << 13;
        PARTICLES[i].variant = (Random16() & 15) + 32;
      }
    }

    // The chips fly on frames 32 to 207: the four large pictures, then the
    // seven small ones from frame 192. A chip whose variant is negative has
    // not been thrown.
    if (frame >= 32 && frame < 208) {
      for (i = 0; i != 64; i++) {
        if (PARTICLES[i].variant >= 0) {
          s32 cell;
          if (frame > 191) {
            cell = i % 7 + 4;
          } else {
            cell = i & 3;
          }
          drawAlt(canvas, work->sheet + gBoreas_DebrisCells[cell],
                  Q_16_16_TO_INT(PARTICLES[i].x),
                  Q_16_16_TO_INT(PARTICLES[i].y), gBoreas_DebrisWidths[cell],
                  gBoreas_DebrisHeights[cell]);
          PARTICLES[i].x += PARTICLES[i].velocityX;
          PARTICLES[i].y += PARTICLES[i].velocityY;
          PARTICLES[i].velocityY += Q16_ONE / 8;
        }
      }
    }

    // Spray: 128 drops from (72, 56) towards the bottom left, leaving smoke
    // behind them from frame 228. Drop i starts on frame 224 + i / 4 and
    // every other drop is drawn; one that leaves the canvas starts again.
    // Trail i starts on frame 228 + i and, each time its 18-frame puff ends,
    // starts again where its drop is.
    if (frame > 223) {
      if (frame == 224) {
        for (i = 0; i != 128; i++) {
          PARTICLES[i].x = 72 << 16;
          PARTICLES[i].y = 56 << 16;
          PARTICLES[i].velocityX = (-(Random16() & 127) - 64) << 11;
          PARTICLES[i].velocityY = ((Random16() & 127) + 16) << 11;
          PARTICLES[i].variant = Random16();
        }
      }
      for (i = 0; i != 128; i++) {
        if (frame >= i / 4 + 224) {
          s32 cell = i % 3;
          if ((i & 1) == 0) {
            draw(canvas, work->sheet + gBoreas_SprayCells[cell],
                 Q_16_16_TO_INT(PARTICLES[i].x), Q_16_16_TO_INT(PARTICLES[i].y),
                 gBoreas_SprayWidths[cell], gBoreas_SprayHeights[cell]);
          }
          PARTICLES[i].x += PARTICLES[i].velocityX;
          PARTICLES[i].y += PARTICLES[i].velocityY;
          if ((PARTICLES[i].x >> 16) < -16 || (PARTICLES[i].y >> 16) > 120) {
            PARTICLES[i].x = 72 << 16;
            PARTICLES[i].y = 56 << 16;
          }
          // DEAD: a drop's variant, random from frame 224, is never read (the
          // second half sets 0 to 63 anew); the ROM keeps the count.
          PARTICLES[i].variant++;
        }
      }
      if (frame == 228) {
        for (i = 0; i != 128; i++) {
          spray = &PARTICLES[i];
          trail = &BOREAS_ICE_BLOCK_TRAILS[i];
          trail->x = spray->x;
          trail->y = spray->y;
          trail->variant = 0;
        }
      }
      for (i = 0, trail = BOREAS_ICE_BLOCK_TRAILS, spray = PARTICLES; i != 128;
           i++, trail++, spray++) {
        if (frame >= i + 228) {
          s32 cell = trail->variant / 2 % 9;
          s32 size = gBattleEffect_PuffSizes[cell];
          draw(canvas, puffSheet + gBattleEffect_PuffCells[cell],
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

  //------------------------------------------------------------------
  // Second half: ice blocks rain down on the targets, and boulders crumble
  // into bouncing chips.
  //------------------------------------------------------------------
  for (i = 0; i != 9; i++) {
    Sprite_DestroyFar(work->sprites[i]);
  }
  // Blend the canvas fully over the battle background, set the ice blocks
  // above the canvas to the right, and stagger the smoke puffs, puff i
  // waiting i / 2 frames (its negative variant) before it shows.
  BattleEffect_SetupBlendedDisplay();
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  Sound_PlayFar(SOUND_FADE_OUT_ACTION_SE);

  for (i = 0; i != 64; i++) {
    work->particles[i].x = ((Random16() & 127) + 64) << 16;
    work->particles[i].z = -(16 << 16);
    work->particles[i].y = (-(Random16() & 127) - 64) << 16;
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
    // Raise one boulder every 2 frames from frame 18, up to 16; frame 16
    // starts a 32-frame shake, and a thud plays every 4 frames from 12 to 68.
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
    // From frame 65 a 40x80 block falls diagonally from the top right, 7
    // pixels left and 14 down a frame.
    if (frame > 64) {
      draw(canvas, work->sheet + BOREAS_FALLING_BLOCK_OFFSETS,
           (64 - frame) * 7 + 88, frame * 14 - 992, 40, 80);
    }
    if (frame <= 71) {
      for (i = 0; i != count; i++) {
        s32 cell = i % 3;
        draw(canvas, work->sheet + gBoreas_BoulderCells[cell],
             BOREAS_POINT_X(i) - 56,
             BOREAS_POINT_Y(i) - gBoreas_BoulderHeights[cell],
             gBoreas_BoulderWidths[cell], gBoreas_BoulderHeights[cell]);
      }
    }
    // Frame 72: the boulders crumble, throwing up 64 chips from where they
    // stood, four from each, and the ice blocks are gone; the chips take
    // over work->particles. The second frame 72 test is redundant.
    if (frame == 72) {
      for (i = 0; i != 64; i++) {
        work->particles[i].x = (BOREAS_POINT_X(i & 15) - 56) << 16;
        work->particles[i].y = BOREAS_POINT_Y(i & 15) << 16;
        work->particles[i].velocityX = ((Random16() & 127) - 63) << 13;
        work->particles[i].velocityY = (-(Random16() & 31) - 16) << 14;
      }
      if (frame == 72) {
        work->shakeFrames = 4;
      }
    }
    // Chips, 32 on the first frame and 64 after: one below y 135 is gone, and
    // one that lands faster than 8 pixels a frame at y 120 bounces back at a
    // quarter of its speed, jolting the screen for a frame.
    if (frame > 71) {
      count = 32;
      if (frame != 72) {
        count = 64;
      }
      for (i = 0; i != count; i++) {
        struct Particle *chip = &work->particles[i];
        y = Q_16_16_TO_INT(chip->y);
        if (y <= 135) {
          s32 cell = i % 3;
          draw(canvas, work->sheet + gBoreas_FallCells[cell],
               Q_16_16_TO_INT(chip->x), y - gBoreas_FallHeights[cell],
               gBoreas_FallWidths[cell], gBoreas_FallHeights[cell]);
          Particle_AdvanceWithGravity2D(chip, 64, 1 << 16);
          if (Q_16_16_TO_INT(chip->y) > 120 && chip->velocityY > 8 << 16) {
            chip->velocityY = -chip->velocityY / 4;
            chip->y = 120 << 16;
            work->shakeFrames = 1;
          }
        }
      }
    }
    // The falling ice blocks (24x48): until frame 47, one that lands starts
    // again at the top.
    if (frame <= 71) {
      for (i = 0; i != 64; i++) {
        draw(canvas, work->sheet, Q_16_16_TO_INT(work->particles[i].x) - 12,
             Q_16_16_TO_INT(work->particles[i].y) - 24, 24, 48);
        work->particles[i].x += work->particles[i].velocityX;
        work->particles[i].y += work->particles[i].velocityY;
        if ((work->particles[i].y >> 16) > 120 && frame <= 47) {
          work->particles[i].x = ((Random16() & 127) + 64) << 16;
          work->particles[i].y = -(16 << 16);
        }
      }
    }

    // Target i takes a blue tint and its damage pose on frame 32 + 8i.
    EFFECT_FOR_EACH_TARGET(i, work->args) {
      if (frame == i * 8 + 32) {
        BattleEffect_TintBattler(work->args->targets[i], SPRITE_COLOR_BLUE_TINT,
                                 POSE_DAMAGE, -1, 0);
      }
    }

    // Smoke (from frame 73): a puff lives 18 frames; until frame 127, each
    // puff that ends starts again somewhere new, lower as the frames pass.
    if (frame > 72) {
      for (i = 0; i != 64; i++) {
        if (PARTICLES[i].variant >= 0 && PARTICLES[i].variant < 18) {
          s32 cell = PARTICLES[i].variant / 2 % 9;
          s32 size = gBattleEffect_PuffSizes[cell];
          draw(canvas, puffSheet + gBattleEffect_PuffCells[cell],
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

    // The camera shakes hard while the boulders crumble, gently otherwise.
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

#undef BOREAS_ICE_BLOCK_TRAILS
#undef BOREAS_BIG_BLOCK_OFFSETS
#undef BOREAS_FALLING_BLOCK_OFFSETS
#undef BOREAS_POINT_X
#undef BOREAS_POINT_Y

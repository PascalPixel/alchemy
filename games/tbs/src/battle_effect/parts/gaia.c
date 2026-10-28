/// @file
/// Gaia.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"

/// The standing wall's top in the rebuilt sheet, 48x96 (stretched from 48x32).
#define GAIA_WALL_TOP_OFFSETS 0x13c0
/// The standing wall's foot in the rebuilt sheet, 48x21.
#define GAIA_WALL_FOOT_OFFSETS 0x25c0
#define GAIA_WALL_FOOT_BYTES (48 * 21)
#define GAIA_FRAMES 192
#define GAIA_ROCKS 11 ///< rocks that tumble up the wall

/// The ground swells on the targets' side of the field under a narrow shaft of
/// orange light, which widens into a glowing wall and stands while rocks tumble
/// up through it and the targets are battered, then narrows away again.
///
/// Abilities: Gaia, Mother Gaia, Grand Gaia, Rockfall, Rockslide, Avalanche
/// (BATTLE_EFFECT_GAIA).
///
/// The wall is symmetric: its left half is drawn, then mirrored for the right.
/// Its centre is at canvas x 64 + wallX. The rocks are 11 sprites,
/// moved by particles[10] to particles[20].
///
/// args: the targets; the variant widens the wall: narrowest for Gaia and
///         Rockfall, wider for Mother Gaia and Rockslide, widest for Grand
///         Gaia and Avalanche.
void BattleEffect_RunGaia(struct BattleEffectParams *args) {
  static struct SpriteScale unitScale = {{{Q16_ONE, Q16_ONE}}};
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  BlitterPair draw;
  u8 *sprites;
  s32 wallX;
  s32 rockX;
  struct SpriteScale scale;
  struct SpritePlacement position;
  u8 *src;
  u16 *color;
  void *palette = (void *)BG_PLTT;
  s32 sheetIndex;
  s32 frame;
  s32 i;
  s32 j;
  s32 k;

  EFFECT_GET_SLOTS(slots, cursor, work, canvas);
  sprites = EFFECT_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  work->args = args;
  BattleEffect_StartCanvasLayer(0);

  // The higher the variant, the wider the wall: BG2 stretches the canvas 1.25
  // or 1.5 times, or twice as BattleEffect_StartCanvasLayer left it.
  if (work->args->variant == EFFECT_VARIANT_BASIC)
    REG_BG2PA = Q_8_8(0.8);
  else if (work->args->variant == EFFECT_VARIANT_STRONGER)
    REG_BG2PA = Q_8_8(1.0 / 1.5);
  if (work->args->side == EFFECT_SIDE_PARTY) {
    wallX = 8;
    if (work->args->variant == EFFECT_VARIANT_BASIC)
      rockX = 40;
    else if (work->args->variant == EFFECT_VARIANT_STRONGER)
      rockX = 36;
    else
      rockX = 40;
  } else {
    wallX = -16;
    rockX = -12;
  }
  REG_BG2X = gGaia_Bg2Shifts[work->args->side][work->args->variant] << 8;

  // Decode the EarthWall sheet into Ram_MapGridCellBuffer, and load the orange
  // palette with background colours 1 to 63 each 8 steps darker a channel.
  Resource_LoadAndDecompress((s32)&ResourceId_EarthWallSheet,
                             Ram_MapGridCellBuffer, 1, 0);
  Iwram_CopyWords(palette,
                  Resource_GetTableEntry((s32)&ResourceId_OrangePaletteA), 128);
  color = (u16 *)&BG_PLTT[1];
  for (i = 0; i != 63; i++) {
    s32 entry = *color;
    s32 blue = ((u16)entry >> 10) & 31;
    s32 green = ((u16)entry >> 5) & 31;
    s32 red = 31;

    red &= entry;
    blue -= 8;
    green -= 8;
    red -= 8;

    if (blue < 0)
      blue = 0;
    if (green < 0)
      green = 0;
    if (red < 0)
      red = 0;
    *color++ = RGB(red, green, blue);
  }

  // Rebuild the pictures in work->sheet:
  //   the growing wall: nine one-line rows, each stretched to 32 lines (the
  //   frame loop draws only the first eight);
  //   the top of the wall (GAIA_WALL_TOP_OFFSETS): 48x32 stretched to 48x96;
  //   the foot of the wall (GAIA_WALL_FOOT_OFFSETS): 48x21;
  //   the swelling ground: three frames.
  src = Ram_MapGridCellBuffer;
  sheetIndex = 0;
  for (j = 0; j != 9; j++) {
    for (i = 0; i != 32; i++) {
      for (k = 0; k != gGaia_RowWidths[j]; k++, sheetIndex++)
        work->sheet[sheetIndex] = src[k];
    }
    src += gGaia_RowWidths[j];
  }
  for (j = 0; j != 32; j++) {
    for (i = 0; i != 3; i++) {
      for (k = 0; k != 48; k++, sheetIndex++)
        work->sheet[sheetIndex] = src[k];
    }
    src += 48;
  }
  for (i = 0; i != GAIA_WALL_FOOT_BYTES; i++)
    work->sheet[sheetIndex++] = *src++;
  for (i = 0; i != 3; i++) {
    for (k = 0; k != gGaia_MoundWidths[i] * gGaia_MoundHeights[i]; k++)
      work->sheet[sheetIndex++] = *src++;
  }
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sprites, 0, 0);

  // The rock sprites, of SPRITE_BATTLE_ROCKS: sprites 0 to 3 play animation
  // 0, 4 to 7 effect 1 and 8 to 10 effect 2.
  for (i = 0; i != GAIA_ROCKS; i++) {
    struct Sprite *rock = Sprite_CreateFar(SPRITE_BATTLE_ROCKS);

    work->sprites[i] = rock;
    if (rock != 0) {
      rock->flags = 0;
      Sprite_StartAnimationFar(rock, i / 4);
      work->sprites[i]->image.priority = 1;
    }
  }
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_LIGHTEN);
  draw[0] = (DrawRectangle)gHeapSlots[HEAP_SLOT_RAM_CODE];
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_LIGHTEN);
  draw[1] = (DrawRectangle)gHeapSlots[HEAP_SLOT_RAM_CODE_2];
  REG_BLDCNT =
      BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  // Trails: each transfer keeps 3/4 of every canvas level.
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

  // Rocks: x is the column, y counts down from 128 by z pixels a frame (2
  // to 17, and 8 to 15 for a rock restarted from frame 88), velocityX
  // cycles through 1 to 12 but nothing else reads it, and variant is the
  // wait before the rock appears (44 frames for the first, 4 fewer for each
  // after).
  for (i = 0; i != GAIA_ROCKS; i++) {
    struct Particle *rock = &work->particles[10 + i];

    SET_POSITION(rock, (Random16() & 15) + 88, 128, (Random16() & 15) + 2);
    SET_VELOCITY(rock, i, 1, 0x8000);
    rock->variant = 44 - i * 4;
  }
  // Then particles[16] to particles[21] take the fixed columns of
  // gGaia_LateRockColumns: the five rocks that join on frame 72, and
  // particles[21], which no rock uses.
  for (i = 0; i != 6; i++)
    work->particles[16 + i].x = gGaia_LateRockColumns[i];

  // Dust rises from the foot of the wall, across its middle 64 pixels, at 4
  // to 12 pixels a frame, each mote after a random wait of up to 31 frames
  // (its variant). 256 are placed; the frame loop draws 64.
  for (i = 0; i != 256; i++) {
    struct Particle *dust = &PARTICLES[i];

    dust->x = ((Random16() & 63) + wallX + 32) << 16;
    dust->y = ((Random16() & 7) + 96) << 16;
    dust->velocityY = ((Random16() & 63) + 32) << 13;
    dust->variant = Random16() & 31;
  }
  REG_BG2CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7);
  // The screen shakes throughout: 250 frames outlast the effect, and the
  // ranges given to BattleCamera_Shake below set how hard.
  work->shakeFrames = 250;
  for (frame = 0; frame != GAIA_FRAMES; frame++) {
    if (frame == 0)
      Sound_PlayFar(SE_POWER_UP);
    if (frame == 40)
      Sound_PlayFar(SE_RUMBLE);
    if (frame == 96)
      Sound_PlayFar(SE_CRASH);
    if (frame == 120)
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);

    // The swelling ground (frames 0 to 81).
    if (frame <= 81) {
      s32 cell = frame / 4;

      if (cell > 2)
        cell = (cell & 1) + 1;
      draw[0](canvas, work->sheet + gGaia_MoundCells[cell],
              wallX - gGaia_MoundWidths[cell] + 64,
              116 - gGaia_MoundHeights[cell], gGaia_MoundWidths[cell],
              gGaia_MoundHeights[cell]);
      draw[1](canvas, work->sheet + gGaia_MoundCells[cell], wallX + 64,
              116 - gGaia_MoundHeights[cell], gGaia_MoundWidths[cell],
              gGaia_MoundHeights[cell]);
    }

    // The rising wall (frames 12 to 87): its narrowest row until frame 66,
    // then a row wider every 3 frames, the widest from frame 85.
    if (frame >= 12 && frame <= 87) {
      s32 row = (frame - 64) / 3;

      CLAMP(row, 0, 7);
      for (i = 0; i != 4; i++) {
        draw[0](canvas, work->sheet + gGaia_RowCells[row],
                wallX - gGaia_RowWidths[row] + 64, i * 32 - 12,
                gGaia_RowWidths[row], 32);
        draw[1](canvas, work->sheet + gGaia_RowCells[row], wallX + 64,
                i * 32 - 12, gGaia_RowWidths[row], 32);
      }
    }

    // The sinking wall (frames 160 to 183).
    if (frame >= 160 && frame <= 183) {
      s32 row = 7 - (frame - 160) / 3;

      CLAMP(row, 0, 7);
      for (i = 0; i != 4; i++) {
        draw[0](canvas, work->sheet + gGaia_RowCells[row],
                wallX - gGaia_RowWidths[row] + 64, i * 32 - 12,
                gGaia_RowWidths[row], 32);
        draw[1](canvas, work->sheet + gGaia_RowCells[row], wallX + 64,
                i * 32 - 12, gGaia_RowWidths[row], 32);
      }
    }

    // The standing wall (frames 88 to 159).
    if (frame >= 88 && frame <= 159) {
      s32 width = 48;

      draw[0](canvas, &work->sheet[GAIA_WALL_TOP_OFFSETS], wallX + 16, 0, width,
              96);
      draw[1](canvas, &work->sheet[GAIA_WALL_TOP_OFFSETS], wallX + 64, 0, width,
              96);
      draw[0](canvas, &work->sheet[GAIA_WALL_FOOT_OFFSETS], wallX + 16, 96,
              width, 21);
      draw[1](canvas, &work->sheet[GAIA_WALL_FOOT_OFFSETS], wallX + 64, 96,
              width, 21);
    }

    // Dust (from frame 88): until frame 159, dust that leaves the top
    // returns to the foot.
    if (frame > 87) {
      for (i = 0; i != 64; i++) {
        struct Particle *dust = &PARTICLES[i];

        if (dust->variant == 0) {
          s32 size = (i & 3) + 5;

          EFFECT_DRAW_MOTE(draw[0], canvas, sprites, Q_16_16_TO_INT(dust->x),
                           Q_16_16_TO_INT(dust->y), size);
          dust->y -= dust->velocityY;
          if (dust->y < 0 && frame <= 159)
            dust->y = 0x600000;
        } else {
          dust->variant--;
        }
      }
    }

    // Rocks (from frame 5): six at half size, then from frame 72 all 11, each
    // larger than the last and larger with the variant. The sprites take
    // turns showing the rocks, moving on every 2 frames; four sprites play
    // each of effects 0 and 1, three effect 2.
    if (frame > 4) {
      s32 rocks = 6;

      if (frame > 71)
        rocks = GAIA_ROCKS;
      for (k = 0; k != rocks; k++) {
        struct Particle *rock = &work->particles[10 + k];
        s32 slot;

        if (rock->variant == 0) {
          scale = unitScale;
          if (frame > 71) {
            scale.x = (k << 12) + 0x8000;
            scale.x += work->args->variant << 14;
          } else {
            scale.x = 0x8000;
          }
          scale.y = scale.x;
          position.groundHeight = 0;
          position.x = (rock->x + rockX * 2) << 16;
          position.height = 0x2000000 - (rock->y << 16);
          position.y = 0x2000000;
          slot = (frame / 2 + k) % GAIA_ROCKS;
          // DEAD: always true, as slot is a remainder of a positive count; the
          // ROM keeps the test.
          if (slot != -1)
            Actor_ApplyProjectedPlacementFar(work->sprites[slot], &position,
                                             &scale, 0);
          rock->y -= rock->z;
          rock->velocityX += rock->velocityY;
          if (rock->velocityX > 12)
            rock->velocityX -= 12;
          if (rock->y < 0) {
            // Below 0 the rock starts again at 128 (from frame 88 in a new
            // column, at a new speed).
            if (frame > 159) {
              rock->variant = -1;
            } else {
              if (frame > 87) {
                rock->z = (Random16() & 7) + 8;
                if (work->args->variant == EFFECT_VARIANT_BASIC)
                  rock->x = Random16() % 96 + 42;
                else if (work->args->variant == EFFECT_VARIANT_STRONGER)
                  rock->x = Random16() % 112 + 34;
                else
                  rock->x = Random16() % 160 + 10;
              }
              rock->y = 128;
              rock->variant = 8;
            }
          }
        } else {
          rock->variant--;
        }
      }
    }

    // The targets (frames 86 to 158): a flash every 12 frames and a recoil
    // every 4.
    if (frame <= 158) {
      EFFECT_FOR_EACH_TARGET(i, work->args) {
        if (frame > 85) {
          if (frame % 12 == 0)
            BattleEffect_TintBattler(work->args->targets[i], SPRITE_COLOR_WHITE,
                                     POSE_DAMAGE, i, 6);
          if ((frame & 3) == 0)
            BattleMotion_ApplyKnockbackFar(work->args->targets[i],
                                           KNOCKBACK_SKID);
        }
      }
    }
    if (frame < 90 || frame > 160)
      BattleCamera_Shake(2, 2);
    else
      BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(work);
  }
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  EFFECT_RELEASE_BLITTERS();
  for (i = 0; i != GAIA_ROCKS; i++)
    Sprite_DestroyFar(work->sprites[i]);
  BattleEffect_EndCanvasLayer();
}

#undef GAIA_WALL_TOP_OFFSETS
#undef GAIA_WALL_FOOT_OFFSETS
#undef GAIA_WALL_FOOT_BYTES
#undef GAIA_FRAMES
#undef GAIA_ROCKS

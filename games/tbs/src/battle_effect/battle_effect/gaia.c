/// @file
/// The Gaia and Rockfall Psynergy.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"

/// The pictures Gaia rebuilds from the earth wall sheet: the rising wall's
/// rows, the standing wall's top and foot, then the flames.
enum EarthWallSheetPicture {
  /// 48x96, stretched from 48x32
  GAIA_WALL_TOP = 32 * (5 + 6 + 9 + 12 + 15 + 18 + 21 + 32 + 40),
  /// 48x21
  GAIA_WALL_FOOT = GAIA_WALL_TOP + 48 * 96
};

/// Flames flicker at the targets' feet under a narrow shaft of orange light,
/// which widens into a glowing wall and stands while rocks tumble up through it
/// and the targets are battered, then narrows away again.
///
/// Abilities: Gaia, Mother Gaia, Grand Gaia, Rockfall, Rockslide, Avalanche.
void BattleEffect_RunGaia(struct BattleEffectParams *args) {
  static struct SpriteScale unitScale = {{{Q16_ONE, Q16_ONE}}};
  struct BattleEffectWork *work;
  void *canvas;
  BlitterPair draw;
  u8 *sprites;
  s32 wallX;
  s32 rockX;
  struct SpriteScale scale;
  struct SpritePlacement position;
  u8 *src;
  volatile u16 *color;
  s32 sheetIndex;
  s32 frame;
  s32 i;
  s32 j;
  s32 k;

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  sprites = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
  work->args = args;
  BattleEffect_StartCanvasLayer(0);

  // The higher the variant, the wider the wall.
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

  Resource_LoadAndDecompress((s32)&ResourceId_EarthWallSheet,
                             Ram_MapGridCellBuffer, 1, 0);
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_OrangePaletteA), 128);
  color = &BG_PLTT[1];
  for (i = 0; i != 63; i++) {
    s32 entry = *color;
    s32 blue = ((u16)entry >> 10) & 31;
    s32 green = ((u16)entry >> 5) & 31;
    s32 red = entry & 31;

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
  for (i = 0; i != 48 * 21; i++)
    work->sheet[sheetIndex++] = *src++;
  for (i = 0; i != 3; i++) {
    for (k = 0; k != gGaia_FlameWidths[i] * gGaia_FlameHeights[i]; k++)
      work->sheet[sheetIndex++] = *src++;
  }
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sprites, 0, 0);

  for (i = 0; i != 11; i++) {
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
  draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_LIGHTEN);
  draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_2);
  REG_BLDCNT =
      BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

  // Rocks wait their turn and tumble up through the wall.
  for (i = 0; i != 11; i++) {
    struct Particle *rock = &work->particles[10 + i];

    SET_POSITION(rock, (Random16() & 15) + 88, 128, (Random16() & 15) + 2);
    SET_VELOCITY(rock, i, 1, 32768);
    rock->variant = 44 - i * 4;
  }
  for (i = 0; i != 6; i++)
    work->particles[16 + i].x = gGaia_LateRockColumns[i];

  // Dust rises from the foot of the wall.
  for (i = 0; i != 256; i++) {
    struct Particle *dust = &PARTICLES[i];

    dust->x = ((Random16() & 63) + wallX + 32) << 16;
    dust->y = ((Random16() & 7) + 96) << 16;
    dust->velocityY = ((Random16() & 63) + 32) << 13;
    dust->variant = Random16() & 31;
  }
  REG_BG2CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7);
  // The screen shakes throughout.
  work->shakeFrames = 250;
  for (frame = 0; frame != 192; frame++) {
    if (frame == 0)
      Sound_PlayFar(SE_POWER_UP);
    if (frame == 40)
      Sound_PlayFar(SE_RUMBLE);
    if (frame == 96)
      Sound_PlayFar(SE_CRASH);
    if (frame == 120)
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);

    // The flames flicker at the wall's foot.
    if (frame <= 81) {
      s32 picture = frame / 4;

      if (picture > 2)
        picture = (picture & 1) + 1;
      draw[0](canvas, work->sheet + gGaia_FlamePictures[picture],
              wallX - gGaia_FlameWidths[picture] + 64,
              116 - gGaia_FlameHeights[picture], gGaia_FlameWidths[picture],
              gGaia_FlameHeights[picture]);
      draw[1](canvas, work->sheet + gGaia_FlamePictures[picture], wallX + 64,
              116 - gGaia_FlameHeights[picture], gGaia_FlameWidths[picture],
              gGaia_FlameHeights[picture]);
    }

    // The rising wall widens a row at a time.
    if (frame >= 12 && frame <= 87) {
      s32 row = (frame - 64) / 3;

      CLAMP(row, 0, 7);
      for (i = 0; i != 4; i++) {
        draw[0](canvas, work->sheet + gGaia_RowPictures[row],
                wallX - gGaia_RowWidths[row] + 64, i * 32 - 12,
                gGaia_RowWidths[row], 32);
        draw[1](canvas, work->sheet + gGaia_RowPictures[row], wallX + 64,
                i * 32 - 12, gGaia_RowWidths[row], 32);
      }
    }

    // The wall sinks away.
    if (frame >= 160 && frame <= 183) {
      s32 row = 7 - (frame - 160) / 3;

      CLAMP(row, 0, 7);
      for (i = 0; i != 4; i++) {
        draw[0](canvas, work->sheet + gGaia_RowPictures[row],
                wallX - gGaia_RowWidths[row] + 64, i * 32 - 12,
                gGaia_RowWidths[row], 32);
        draw[1](canvas, work->sheet + gGaia_RowPictures[row], wallX + 64,
                i * 32 - 12, gGaia_RowWidths[row], 32);
      }
    }

    // The standing wall.
    if (frame >= 88 && frame <= 159) {
      s32 width = 48;

      draw[0](canvas, &work->sheet[GAIA_WALL_TOP], wallX + 16, 0, width, 96);
      draw[1](canvas, &work->sheet[GAIA_WALL_TOP], wallX + 64, 0, width, 96);
      draw[0](canvas, &work->sheet[GAIA_WALL_FOOT], wallX + 16, 96, width, 21);
      draw[1](canvas, &work->sheet[GAIA_WALL_FOOT], wallX + 64, 96, width, 21);
    }

    // Dust keeps rising up the wall.
    if (frame > 87) {
      for (i = 0; i != 64; i++) {
        struct Particle *dust = &PARTICLES[i];

        if (dust->variant == 0) {
          s32 size = (i & 3) + 5;

          EFFECT_DRAW_MOTE(draw[0], canvas, sprites, Q_16_16_TO_INT(dust->x),
                           Q_16_16_TO_INT(dust->y), size);
          dust->y -= dust->velocityY;
          if (dust->y < 0 && frame <= 159)
            dust->y = px(96);
        } else {
          dust->variant--;
        }
      }
    }

    // Rocks tumble up through the wall, larger with the variant.
    if (frame > 4) {
      s32 rocks = 6;

      if (frame > 71)
        rocks = 11;
      for (k = 0; k != rocks; k++) {
        struct Particle *rock = &work->particles[10 + k];
        s32 slot;

        if (rock->variant == 0) {
          // DEAD: scale copied from unitScale before both its fields are set;
          // the ROM keeps the copy.
          scale = unitScale;
          if (frame > 71) {
            scale.x = (k << 12) + Q16_ONE / 2;
            scale.x += work->args->variant << 14;
          } else {
            scale.x = Q16_ONE / 2;
          }
          scale.y = scale.x;
          position.groundHeight = 0;
          position.x = px(rock->x + rockX * 2);
          position.height = px(512) - px(rock->y);
          position.y = px(512);
          slot = (frame / 2 + k) % 11;
          // DEAD: slot != -1 always holds, slot being a remainder of a
          // positive count; the ROM keeps the test.
          if (slot != -1)
            Actor_ApplyProjectedPlacementFar(work->sprites[slot], &position,
                                             &scale, 0);
          rock->y -= rock->z;
          rock->velocityX += rock->velocityY;
          if (rock->velocityX > 12)
            rock->velocityX -= 12;
          if (rock->y < 0) {
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

    // The targets flash and recoil.
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
  for (i = 0; i != 11; i++)
    Sprite_DestroyFar(work->sprites[i]);
  BattleEffect_EndCanvasLayer();
}

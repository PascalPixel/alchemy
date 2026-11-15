/// @file
/// The Atalanta summon.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

/// The pictures in the slash sheet: five flash pictures, then the shaft in
/// three parts, each in its own size.
#define ATALANTA_SHAFT_FLASH_A_WIDTH 14
#define ATALANTA_SHAFT_FLASH_A_HEIGHT 28
#define ATALANTA_SHAFT_FLASH_B_WIDTH 23
#define ATALANTA_SHAFT_FLASH_B_HEIGHT 44
#define ATALANTA_SHAFT_FLASH_C_TOP_WIDTH 20
#define ATALANTA_SHAFT_FLASH_C_TOP_HEIGHT 30
#define ATALANTA_SHAFT_FLASH_C_BOTTOM_WIDTH 22
#define ATALANTA_SHAFT_FLASH_C_BOTTOM_HEIGHT 33
#define ATALANTA_SHAFT_FLASH_D_TOP_WIDTH 18
#define ATALANTA_SHAFT_FLASH_D_TOP_HEIGHT 27
#define ATALANTA_SHAFT_FLASH_D_MIDDLE_WIDTH 22
#define ATALANTA_SHAFT_FLASH_D_MIDDLE_HEIGHT 22
#define ATALANTA_SHAFT_FLASH_D_BOTTOM_WIDTH 19
#define ATALANTA_SHAFT_FLASH_D_BOTTOM_HEIGHT 28
#define ATALANTA_SHAFT_FLASH_E_TOP_WIDTH 16
#define ATALANTA_SHAFT_FLASH_E_TOP_HEIGHT 23
#define ATALANTA_SHAFT_FLASH_E_MIDDLE_WIDTH 23
#define ATALANTA_SHAFT_FLASH_E_MIDDLE_HEIGHT 28
#define ATALANTA_SHAFT_FLASH_E_BOTTOM_WIDTH 20
#define ATALANTA_SHAFT_FLASH_E_BOTTOM_HEIGHT 28
#define ATALANTA_SHAFT_TOP_WIDTH 16
#define ATALANTA_SHAFT_TOP_HEIGHT 21
#define ATALANTA_SHAFT_MIDDLE_WIDTH 29
#define ATALANTA_SHAFT_MIDDLE_HEIGHT 35
#define ATALANTA_SHAFT_BOTTOM_WIDTH 21
#define ATALANTA_SHAFT_BOTTOM_HEIGHT 24

enum SlashSheetPicture {
  ATALANTA_SHAFT_FLASH_A = 0,
  ATALANTA_SHAFT_FLASH_B = ATALANTA_SHAFT_FLASH_A +
      PICTURE_BYTES(ATALANTA_SHAFT_FLASH_A),
  ATALANTA_SHAFT_FLASH_C_TOP = ATALANTA_SHAFT_FLASH_B +
      PICTURE_BYTES(ATALANTA_SHAFT_FLASH_B),
  ATALANTA_SHAFT_FLASH_C_BOTTOM = ATALANTA_SHAFT_FLASH_C_TOP +
      PICTURE_BYTES(ATALANTA_SHAFT_FLASH_C_TOP),
  ATALANTA_SHAFT_FLASH_D_TOP = ATALANTA_SHAFT_FLASH_C_BOTTOM +
      PICTURE_BYTES(ATALANTA_SHAFT_FLASH_C_BOTTOM),
  ATALANTA_SHAFT_FLASH_D_MIDDLE = ATALANTA_SHAFT_FLASH_D_TOP +
      PICTURE_BYTES(ATALANTA_SHAFT_FLASH_D_TOP),
  ATALANTA_SHAFT_FLASH_D_BOTTOM = ATALANTA_SHAFT_FLASH_D_MIDDLE +
      PICTURE_BYTES(ATALANTA_SHAFT_FLASH_D_MIDDLE),
  ATALANTA_SHAFT_FLASH_E_TOP = ATALANTA_SHAFT_FLASH_D_BOTTOM +
      PICTURE_BYTES(ATALANTA_SHAFT_FLASH_D_BOTTOM),
  ATALANTA_SHAFT_FLASH_E_MIDDLE = ATALANTA_SHAFT_FLASH_E_TOP +
      PICTURE_BYTES(ATALANTA_SHAFT_FLASH_E_TOP),
  ATALANTA_SHAFT_FLASH_E_BOTTOM = ATALANTA_SHAFT_FLASH_E_MIDDLE +
      PICTURE_BYTES(ATALANTA_SHAFT_FLASH_E_MIDDLE),
  ATALANTA_SHAFT_TOP = ATALANTA_SHAFT_FLASH_E_BOTTOM +
      PICTURE_BYTES(ATALANTA_SHAFT_FLASH_E_BOTTOM),
  ATALANTA_SHAFT_MIDDLE = ATALANTA_SHAFT_TOP +
      PICTURE_BYTES(ATALANTA_SHAFT_TOP),
  ATALANTA_SHAFT_BOTTOM = ATALANTA_SHAFT_MIDDLE +
      PICTURE_BYTES(ATALANTA_SHAFT_MIDDLE)
};

#define TRAILS ((struct Particle *)Ram_MapCellBuffer)
#define MATRICES (Ram_MapCellBuffer + 0x3800)
#define OBJ_VRAM ((u8 *)OBJ_VRAM0)

#define ATALANTA_PIECES 11

#define ATALANTA_FRAMES 88

#define ATALANTA_SHAFTS 16 ///< shafts of light that gather over the field

#define ATALANTA_TRAIL_MOTES 24    ///< motes in each gathering shaft's trail
#define ATALANTA_FALLING_SHAFTS 24 ///< shafts that rain down on the targets

/// Under a violet sky Atalanta flies across the screen while sixteen shafts of
/// light gather trails of motes, flash and fly off; then shafts rain down
/// diagonally on the targets and burst in light, hitting each target as they
/// land.
///
/// Atalanta is built from eleven sprites. First loop (220 frames, A or B
/// skips): she follows her path table, and from frame 64 a shaft every eight
/// frames draws its trails in, flashes for five frames and flies off. Second
/// loop (88 frames): 32 shafts fall from the sky, the first 24 two frames
/// apart, and burst on the ground, each one shaking the screen and flashing a
/// target.
///
/// Abilities: Atalanta.
void BattleEffect_RunAtalanta(struct BattleEffectParams *args) {
  s32 point[3];
  struct ScreenPoint screen;
  struct SpritePlacement position;
  struct ScreenPoint target;
  DrawRectangle draw[2];
  struct BattlePresentationTransition *presentation;
  void *canvas;
  struct BattleEffectWork *work;
  s8 *path;
  s32 i;
  s32 j;
  s32 frame;
  u8 *sheet;
  s32 scrollX;
  s32 pathX;
  s32 pathY;
  s32 row;
  s32 column;
  u8 *tiles;
  u8 *picture;

  presentation = Ram_HeapSlots->battleBackground;
  tiles = Ram_MapCellBuffer;
  canvas = Ram_HeapSlots->battleCanvas;
  work = Ram_HeapSlots->battleEffect;
  sheet = Ram_HeapSlots->battleSheet;
  scrollX = gBgScroll[1].x;
  EFFECT_START_NARROW_CANVAS(BGCNT_WRAP);
  BattleEffect_ConfigureEffectDisplay();
  PLTT_BACKDROP = 0;
  BG_PLTT_COLOR(0, 1) = 0;
  work->transferMode = CANVAS_COPY;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  BattleEffect_WipeCanvas(CANVAS_WIPE_DOWN, CANVAS_WIPE_FILL);
  BattleEffect_SelectLivingTargets(work->args);

  // Atalanta: eleven sprite objects with their tiles.
  Ram_HeapSlots->battleWork->keepBackdropPalette = 1;
  picture = Resource_GetTableEntry((s32)&ResourceId_LeafWarriorTiles);
  Iwram_CopyWords(OBJ_PLTT, picture, 512);
  // Two steps: one `+= 512` reuses the size's constant across the call.
  picture += 256;
  picture += 256;
  Decompress_TaggedLz(picture, tiles);
  for (i = 0; i != ATALANTA_PIECES; i++) {
    struct Sprite *piece = EffectSprite_CreateWithTiles(
        32, 32, OBJ_SIZE_32X32 | OBJ_256_COLORS, 0);

    piece->image.priority = 1;
    work->sprites[i] = piece;
    Iwram_CopyWordsEntry(OBJ_VRAM + gVramCacheEntries[piece->slot].offset,
                         tiles + i * 0x400, 0x400);
  }

  // The shafts, the sky and Atalanta's path.
  Resource_LoadAndDecompress((s32)&ResourceId_SlashSheet, work->sheet, 1, 1);
  picture = Resource_GetTableEntry((s32)&ResourceId_LimePalette);
  Iwram_CopyWordsEntry((void *)BG_PLTT, picture, SHEET_PALETTE_BYTES);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  path = Resource_GetTableEntry((s32)&ResourceId_StagedParticlePath);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                      BLITTER_ADDITIVE);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  draw[1] = ALTERNATE_BLITTER_SLOT;
  gProjection.centerY = SUMMON_PROJECTION_CENTER_Y;
  wait(1);
  BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED,
                           (s32)&ResourceId_VioletSkyBackdrop, 0);
  work->scrollTimer = 0;
  work->scrollInterval = 4;
  work->scrollStepX = -1;
  work->scrollStepY = 0;
  Task_Add(BattleEffect_AdvanceScrollOnInterval, TASK_PASS_FRAME);
  presentation->active = 1;
  BattleEffect_WipeCanvas(CANVAS_WIPE_DOWN, CANVAS_WIPE_CLEAR);
  EFFECT_SHOW_WIDE_CANVAS(BLEND_WEIGHT_MAX);
  pathX = 0;
  pathY = 0;

  // Each shaft: a place on the canvas and 24 trails in random orientations.
  for (i = 0; i != ATALANTA_SHAFTS; i++) {
    struct Particle *shaft = &work->particles[i];

    shaft->x = px(Random16() % 96 + 12);
    shaft->y = px((Random16() & 63) + 32);
    shaft->velocityX = 0;
    shaft->velocityY = 0;
    shaft->variant = 0;
    for (j = 0; j != ATALANTA_TRAIL_MOTES; j++) {
      TRAILS[i * ATALANTA_TRAIL_MOTES + j].x = (Random16() & 15) + 48;
      Transform_Reset();
      Transform_ApplyRoll(Random16() & TRIG_ANGLE_MASK);
      Transform_ApplyPitch(Random16() & TRIG_ANGLE_MASK);
      Transform_ApplyYaw(Random16() & TRIG_ANGLE_MASK);
      Transform_Store(MATRICES + (i * ATALANTA_TRAIL_MOTES + j) * 48);
    }
  }
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);

  for (frame = 0; frame != 220 && !(gInput.repeat & (KEY_A | KEY_B)); frame++) {
    // Atalanta flies her path: two absolute words, then a step a frame.
    if (frame <= 209) {
      if (frame == 0) {
        pathX = (path[0] << 8) + (u8)path[1];
        pathY = (path[2] << 8) + (u8)path[3];
        path += 4;
      } else {
        pathX += path[0];
        pathY += path[1];
        path += 2;
      }
      position.groundHeight = 0;
      position.height = px(255);
      for (row = 0; row != 4; row++) {
        for (column = 0; column != 3; column++) {
          if (row != 3 || column != 2) {
            position.x = (pathX + 80 + column * 32) * 65536;
            position.y = px(64 - pathY + row * 32);
            if (row == 3)
              position.x = (pathX + 80 + column * 32 + 32) * 65536;
            Render_ApplyProjectedPlacementFar(work->sprites[row * 3 + column],
                                              &position, gAtalanta_UnitScale,
                                              0);
          }
        }
      }
    }
    point[1] = 0;
    point[2] = 0;
    if (frame == 48) {
      work->fadeFrames = BACKGROUND_DARKEN_FRAMES;
      work->fadeStep = 0;
    }

    // From frame 64 a shaft every eight frames: its trails draw in, it
    // flashes for five frames and flies off.
    for (i = 0; i != ATALANTA_SHAFTS; i++) {
      s32 start = i * 8 + 64;

      if (frame >= start) {
        struct Particle *shaft = &work->particles[i];
        s32 x = Q_16_16_UPPER_HALF(shaft->x);
        s32 y = Q_16_16_UPPER_HALF(shaft->y);

        if (frame == i * 8 + 84)
          Sound_PlayFar(SE_POWER_UP);
        if (frame >= i * 8 + 85) {
          shaft->x += shaft->velocityX;
          shaft->y += shaft->velocityY;
          shaft->velocityX -= px(1);
          shaft->velocityY += px(2);
          EFFECT_DRAW_PICTURE(draw[0], canvas, work->sheet, ATALANTA_SHAFT_TOP,
                              x + 4, y - 40);
          EFFECT_DRAW_PICTURE(draw[0], canvas, work->sheet,
                              ATALANTA_SHAFT_MIDDLE, x - 16, y - 19);
          EFFECT_DRAW_PICTURE(draw[0], canvas, work->sheet,
                              ATALANTA_SHAFT_BOTTOM, x - 20, y + 16);
        } else if (frame >= i * 8 + 80) {
          switch (frame - start - 16) {
          case 0:
            EFFECT_DRAW_PICTURE(draw[0], canvas, work->sheet,
                                ATALANTA_SHAFT_FLASH_A, x - 7, y - 14);
            break;
          case 1:
            EFFECT_DRAW_PICTURE(draw[0], canvas, work->sheet,
                                ATALANTA_SHAFT_FLASH_B, x - 11, y - 22);
            break;
          case 2:
            EFFECT_DRAW_PICTURE(draw[0], canvas, work->sheet,
                                ATALANTA_SHAFT_FLASH_C_TOP, x - 4, y - 31);
            EFFECT_DRAW_PICTURE(draw[0], canvas, work->sheet,
                                ATALANTA_SHAFT_FLASH_C_BOTTOM, x - 16, y - 1);
            break;
          case 3:
            EFFECT_DRAW_PICTURE(draw[0], canvas, work->sheet,
                                ATALANTA_SHAFT_FLASH_D_TOP, x + 1, y - 38);
            EFFECT_DRAW_PICTURE(draw[0], canvas, work->sheet,
                                ATALANTA_SHAFT_FLASH_D_MIDDLE, x - 11, y - 11);
            EFFECT_DRAW_PICTURE(draw[0], canvas, work->sheet,
                                ATALANTA_SHAFT_FLASH_D_BOTTOM, x - 19, y + 11);
            break;
          case 4:
            EFFECT_DRAW_PICTURE(draw[0], canvas, work->sheet,
                                ATALANTA_SHAFT_FLASH_E_TOP, x + 4, y - 40);
            EFFECT_DRAW_PICTURE(draw[0], canvas, work->sheet,
                                ATALANTA_SHAFT_FLASH_E_MIDDLE, x - 10, y - 17);
            EFFECT_DRAW_PICTURE(draw[0], canvas, work->sheet,
                                ATALANTA_SHAFT_FLASH_E_BOTTOM, x - 20, y + 11);
            break;
          }
        } else {
          for (j = 0; j != ATALANTA_TRAIL_MOTES; j++) {
            struct Particle *trail = &TRAILS[i * ATALANTA_TRAIL_MOTES + j];

            if (trail->x > 0) {
              Transform_Load(MATRICES + (i * ATALANTA_TRAIL_MOTES + j) * 48);
              point[0] = trail->x;
              EffectPosition_ProjectPoint(point, &screen);
              screen.x = (screen.x >> 1) + x;
              screen.y = screen.y + y + 16;
              trail->x -= 4;
              draw[1](canvas, sheet + gBattleEffect_MotePictures[4],
                      screen.x - 2, screen.y - 5, 5, 10);
            }
          }
        }
      }
    }
    EFFECT_PRESENT_CANVAS(work);
  }

  // Atalanta leaves: the sky stops, the palette is restored and the
  // pieces go.
  Task_Remove(BattleEffect_AdvanceScrollOnInterval);
  presentation->active = 0;
  gBgScroll[1].x = scrollX;
  BattleEffect_SetupBlendedDisplay();
  Palette_LoadBasicSpriteColors();
  Dma_Set(OBJ_PLTT, (void *)&BG_PLTT_COLOR(5, 0),
          DMA_COPY_HALFWORDS(PALETTE_COLORS), (volatile u32 *)0x040000d4);
  BG_PLTT_COLOR(5, 14) = BG_PLTT_COLOR(15, 4);
  for (i = 0; i != ATALANTA_PIECES; i++)
    Sprite_DestroyFar(work->sprites[i]);
  REG_BG2PA = Q_8_8(0.5);
  REG_DISPCNT = EFFECT_DISPCNT;
  Resource_LoadAndDecompress((s32)&ResourceId_EmberStreakSheet,
                             Ram_MapCellBuffer, 1, 0);

  // 32 shafts fall from the sky, over the targets where there are any.
  for (i = 0; i != 32; i++) {
    struct Particle *shaft = &work->particles[i];
    s32 member = i % 6;

    if (member < args->targetCount) {
      EffectPosition_ProjectScaledBattler(args->targets[member], &target);
      shaft->y = -((Random16() & 31) + 40);
      shaft->x = target.x / 2 + (80 - shaft->y) / 2;
    } else {
      shaft->x = (Random16() & 63) + 80;
      shaft->y = -((Random16() & 31) + 40);
    }
    shaft->variant = -1;
  }
  for (frame = 0; frame != ATALANTA_FRAMES; frame++) {
    for (i = 0; i != ATALANTA_FALLING_SHAFTS; i++) {
      struct Particle *shaft = &work->particles[i];

      if (frame >= i * 2 || frame > 40) {
        if (shaft->variant >= 0) {
          // On the ground: six flame pictures, four frames each, the shaft
          // still showing for the first three.
          if (shaft->variant <= 23) {
            s32 cell = shaft->variant / 4;
            u32 width;

            draw[i & 1](
                canvas, Ram_MapCellBuffer + gBattleEffect_PuffArcPictures[cell],
                shaft->x -
                    ((width = gBattleEffect_PuffArcPictureWidths[cell]) >> 1) -
                    8,
                shaft->y + gBattleEffect_PuffArcPictureBiasY[cell] - 40, width,
                gBattleEffect_PuffArcPictureHeights[cell]);
            if (shaft->variant <= 11) {
              EFFECT_DRAW_PICTURE(draw[0], canvas, work->sheet,
                                  ATALANTA_SHAFT_TOP, shaft->x + 4,
                                  shaft->y - 40);
              EFFECT_DRAW_PICTURE(draw[0], canvas, work->sheet,
                                  ATALANTA_SHAFT_MIDDLE, shaft->x - 16,
                                  shaft->y - 19);
            }
          }
          shaft->variant++;
        } else {
          // Falling: the blade sinks into the ground below y = 56.
          s32 height = 24;

          if (shaft->y > 56)
            height = height - shaft->y + 56;
          EFFECT_DRAW_PICTURE(draw[0], canvas, work->sheet, ATALANTA_SHAFT_TOP,
                              shaft->x + 4, shaft->y - 40);
          EFFECT_DRAW_PICTURE(draw[0], canvas, work->sheet,
                              ATALANTA_SHAFT_MIDDLE, shaft->x - 16,
                              shaft->y - 19);
          if (height > 0)
            draw[0](canvas, work->sheet + ATALANTA_SHAFT_BOTTOM, shaft->x - 20,
                    shaft->y + 16, ATALANTA_SHAFT_BOTTOM_WIDTH, height);
          shaft->x -= 6;
          shaft->y += 12;
          if (shaft->y > 79) {
            s32 member;

            shaft->variant = 0;
            work->shakeFrames = 2;
            Sound_PlayFar(SE_HEAVY_IMPACT);
            member = i % 6;
            if (member < args->targetCount) {
              EFFECT_STRIKE_TARGET(args, member, 8, KNOCKBACK_HOP);
            }
          }
        }
      }
    }
    BattleCamera_Shake(4, 8);
    EFFECT_END_FRAME(work);
  }
  EFFECT_FINISH_CANVAS();
}
#undef OBJ_VRAM

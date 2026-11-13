/// @file
/// The Atalanta summon.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"

/* VIEW(cybele): routine EffectSprite_CreateWithTiles as struct SpriteObject
 * *EffectSprite_CreateWithTiles(s32 width, s32 height, u32 shape, u32
 * attributes); battle_effect/ability_effects has struct EffectSprite
 * *EffectSprite_CreateWithTiles(s32 width, s32 height, s32 attributes, s32
 * flags) {; tried as the definition's struct EffectSprite
 * *EffectSprite_CreateWithTiles(s32 width, s32 height, s32 attributes, s32
 * flags), which the object already declares, it does not compile in tla-ja
 * (make modern): games/tla/include/module/battle_effect/battle_effect.h:210:64:
 * note: expected 'struct BattleEffectArgument *' but argument is of type 'u8
 * *'. */
struct SpriteObject *EffectSprite_CreateWithTiles__cybele(
    s32 width, s32 height, u32 shape,
    u32 attributes) __asm__("EffectSprite_CreateWithTiles");
#define EffectSprite_CreateWithTiles EffectSprite_CreateWithTiles__cybele

#define TRAILS ((struct EffectStep *)Ram_MapCellBuffer)
#define MATRICES (Ram_MapCellBuffer + 0x3800)
#define OBJ_VRAM ((u8 *)0x06010000)

#define GIANT_PIECES 11

// Summon 4: a giant strides across a violet sky and cuts the air; sixteen
// slashes gather light and fly, then rain down on the targets.

// Setup: the giant's pieces are built as sprite objects; sixteen slashes
// each get 24 trails of light in random orientations. First loop (220
// frames, A or B skips): the giant follows its path table; from frame 64
// a slash every eight frames draws its trails in, flashes for five frames
// and flies off. Second loop (88 frames): 32 slashes fall from the sky, the
// first 24 two frames apart, and burst into flame on the ground, each one
// shaking the screen and flashing a target.
void BattleEffect_RunAtalanta(struct BattleEffectArgument *effect) {
  s32 point[3];
  struct EffectPosition screen;
  struct SpritePlacement position;
  struct EffectPosition target;
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

  presentation = Ram_HeapSlots->transitionWork;
  tiles = Ram_MapCellBuffer;
  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  sheet = Ram_HeapSlots->unk64[0];
  scrollX = gBgScroll[1].x;
  BattleEffect_StartCanvasLayer(0x2000);
  REG_BG2PA = 0x100;
  BattleEffect_ConfigureEffectDisplay();
  BG_PLTT[0] = 0;
  BG_PLTT[1] = 0;
  work->transferMode = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  BattleEffect_WipeCanvas(0, 0);
  BattleEffect_SelectLivingTargets(work->effect);

  // The giant: eleven sprite objects with their tiles.
  ((u8 *)Ram_HeapSlots->battleWork)[0x867] = 1;
  picture = Resource_GetTableEntry((s32)&ResourceId_LeafWarriorTiles);
  Iwram_CopyWords(OBJ_PLTT, picture, 512);
  // Two steps: one `+= 512` reuses the size's constant across the call.
  picture += 256;
  picture += 256;
  Decompress_TaggedLz(picture, tiles);
  for (i = 0; i != GIANT_PIECES; i++) {
    struct SpriteObject *piece =
        EffectSprite_CreateWithTiles(32, 32, 0x80002000, 0);

    piece->priority = 1;
    work->objects[i] = piece;
    Iwram_CopyWordsEntry(OBJ_VRAM + gResourceTableEntries[piece->slot].offset,
                         tiles + i * 0x400, 0x400);
  }

  // The slashes, the sky and the giant's path.
  Resource_LoadAndDecompress((s32)&ResourceId_SlashSheet, work->sheet, 1, 1);
  picture = Resource_GetTableEntry((s32)&ResourceId_LimePalette);
  Iwram_CopyWordsEntry((void *)BG_PLTT, picture, 128);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);
  path = Resource_GetTableEntry((s32)&ResourceId_StagedParticlePath);
  BattleEffect_LoadBlitter(104, BLIT_MODE(BLIT_MAX, CLIP));
  BattleEffect_LoadBlitter(188, BLIT_MODE(BLIT_ADDITIVE, CLIP));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  draw[1] = ALTERNATE_BLITTER_SLOT;
  gCameraSceneParameters[4] = 240;
  wait(1);
  BattleBackground_LoadFar(1, (s32)&ResourceId_VioletSkyBackdrop, 0);
  work->scrollTimer = 0;
  work->scrollInterval = 4;
  work->scrollStepX = -1;
  work->scrollStepY = 0;
  Task_Add(BattleEffect_AdvanceScrollOnInterval, TASK_PASS_FRAME);
  presentation->active = 1;
  BattleEffect_WipeCanvas(0, 1);
  REG_DISPCNT = 0x7741;
  REG_BG2PA = 0x80;
  REG_BLDALPHA = 0x1010;
  REG_BLDCNT = 0x3f44;
  pathX = 0;
  pathY = 0;

  // Each slash: a place on the canvas and 24 trails in random orientations.
  for (i = 0; i != 16; i++) {
    struct EffectStep *slash = &work->particles[i];

    slash->x = (Random16() % 96 + 12) << 16;
    slash->y = ((Random16() & 63) + 32) << 16;
    slash->velocityX = 0;
    slash->velocityY = 0;
    slash->variant = 0;
    for (j = 0; j != 24; j++) {
      TRAILS[i * 24 + j].x = (Random16() & 15) + 48;
      Transform_Reset();
      Transform_ApplyRoll(Random16() & 0xffff);
      Transform_ApplyPitch(Random16() & 0xffff);
      Transform_ApplyYaw(Random16() & 0xffff);
      Transform_Store(MATRICES + (i * 24 + j) * 48);
    }
  }
  work->transferMode = 2;
  work->transferValue = 50;
  REG_BG2CNT = 0x784;

  for (frame = 0; frame != 220 && !(gInput.repeat & 3); frame++) {
    // The giant walks its path: two absolute words, then a step a frame.
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
      position.height = 0xff0000;
      for (row = 0; row != 4; row++) {
        for (column = 0; column != 3; column++) {
          if (row != 3 || column != 2) {
            position.x = (pathX + 80 + column * 32) * 65536;
            position.y = (64 - pathY + row * 32) << 16;
            if (row == 3)
              position.x = (pathX + 80 + column * 32 + 32) * 65536;
            Render_ApplyProjectedPlacementFar(work->objects[row * 3 + column],
                                              &position, gAtalanta_UnitScale,
                                              0);
          }
        }
      }
    }
    point[1] = 0;
    point[2] = 0;
    if (frame == 48) {
      work->fadeFrames = 24;
      work->fadeStep = 0;
    }

    // From frame 64 a slash every eight frames: its trails draw in, it
    // flashes for five frames and flies off.
    for (i = 0; i != 16; i++) {
      s32 start = i * 8 + 64;

      if (frame >= start) {
        struct EffectStep *slash = &work->particles[i];
        s32 x = HI(slash->x);
        s32 y = HI(slash->y);

        if (frame == i * 8 + 84)
          Sound_PlayFar(212);
        if (frame >= i * 8 + 85) {
          slash->x += slash->velocityX;
          slash->y += slash->velocityY;
          slash->velocityX -= 0x10000;
          slash->velocityY += 0x20000;
          draw[0](canvas, work->sheet + SLASH_TOP, x + 4, y - 40, 16, 21);
          draw[0](canvas, work->sheet + SLASH_MIDDLE, x - 16, y - 19, 29, 35);
          draw[0](canvas, work->sheet + SLASH_BOTTOM, x - 20, y + 16, 21, 24);
        } else if (frame >= i * 8 + 80) {
          switch (frame - start - 16) {
          case 0:
            draw[0](canvas, work->sheet + SLASH_FLASH_1, x - 7, y - 14, 14, 28);
            break;
          case 1:
            draw[0](canvas, work->sheet + SLASH_FLASH_2, x - 11, y - 22, 23,
                    44);
            break;
          case 2:
            draw[0](canvas, work->sheet + SLASH_FLASH_3_TOP, x - 4, y - 31, 20,
                    30);
            draw[0](canvas, work->sheet + SLASH_FLASH_3_BOTTOM, x - 16, y - 1,
                    22, 33);
            break;
          case 3:
            draw[0](canvas, work->sheet + SLASH_FLASH_4_TOP, x + 1, y - 38, 18,
                    27);
            draw[0](canvas, work->sheet + SLASH_FLASH_4_MIDDLE, x - 11, y - 11,
                    22, 22);
            draw[0](canvas, work->sheet + SLASH_FLASH_4_BOTTOM, x - 19, y + 11,
                    19, 28);
            break;
          case 4:
            draw[0](canvas, work->sheet + SLASH_FLASH_5_TOP, x + 4, y - 40, 16,
                    23);
            draw[0](canvas, work->sheet + SLASH_FLASH_5_MIDDLE, x - 10, y - 17,
                    23, 28);
            draw[0](canvas, work->sheet + SLASH_FLASH_5_BOTTOM, x - 20, y + 11,
                    20, 28);
            break;
          }
        } else {
          for (j = 0; j != 24; j++) {
            struct EffectStep *trail = &TRAILS[i * 24 + j];

            if (trail->x > 0) {
              Transform_Load(MATRICES + (i * 24 + j) * 48);
              point[0] = trail->x;
              EffectPosition_ProjectPoint(point, &screen);
              screen.x = (screen.x >> 1) + x;
              screen.y = screen.y + y + 16;
              trail->x -= 4;
              draw[1](canvas, sheet + gBattleEffect_MoteCells[4], screen.x - 2,
                      screen.y - 5, 5, 10);
            }
          }
        }
      }
    }
    work->transferPending = 1;
    wait(1);
  }

  // The giant leaves: the sky stops, the palette is restored and the
  // pieces go.
  Task_Remove(BattleEffect_AdvanceScrollOnInterval);
  presentation->active = 0;
  gBgScroll[1].x = scrollX;
  BattleEffect_SetupBlendedDisplay();
  Palette_LoadBasicSpriteColors();
  Dma_Set(OBJ_PLTT, (void *)(BG_PLTT + 80), 0x80000010,
          (volatile u32 *)0x040000d4);
  BG_PLTT[94] = BG_PLTT[244];
  for (i = 0; i != GIANT_PIECES; i++)
    Sprite_DestroyFar(work->objects[i]);
  REG_BG2PA = 0x80;
  REG_DISPCNT = 0x7741;
  Resource_LoadAndDecompress((s32)&ResourceId_EmberStreakSheet,
                             Ram_MapCellBuffer, 1, 0);

  // 32 slashes fall from the sky, over the targets where there are any.
  for (i = 0; i != 32; i++) {
    struct EffectStep *slash = &work->particles[i];
    s32 member = i % 6;

    if (member < effect->count) {
      EffectPosition_ProjectScaledBattler(effect->actors[member], &target);
      slash->y = -((Random16() & 31) + 40);
      slash->x = target.x / 2 + (80 - slash->y) / 2;
    } else {
      slash->x = (Random16() & 63) + 80;
      slash->y = -((Random16() & 31) + 40);
    }
    slash->variant = -1;
  }
  for (frame = 0; frame != 88; frame++) {
    for (i = 0; i != 24; i++) {
      struct EffectStep *slash = &work->particles[i];

      if (frame >= i * 2 || frame > 40) {
        if (slash->variant >= 0) {
          // On the ground: six flame pictures, four frames each, the slash
          // still showing for the first three.
          if (slash->variant <= 23) {
            s32 cell = slash->variant / 4;
            u32 width;

            draw[i & 1](canvas,
                        Ram_MapCellBuffer + gBattleEffect_PuffArcCells[cell],
                        slash->x - ((width = gData_08197467[cell]) >> 1) - 8,
                        slash->y + gBattleEffect_PuffArcCellBiasY[cell] - 40,
                        width, gBattleEffect_PuffArcCellHeights[cell]);
            if (slash->variant <= 11) {
              draw[0](canvas, work->sheet + SLASH_TOP, slash->x + 4,
                      slash->y - 40, 16, 21);
              draw[0](canvas, work->sheet + SLASH_MIDDLE, slash->x - 16,
                      slash->y - 19, 29, 35);
            }
          }
          slash->variant++;
        } else {
          // Falling: the blade sinks into the ground below y = 56.
          s32 height = 24;

          if (slash->y > 56)
            height = height - slash->y + 56;
          draw[0](canvas, work->sheet + SLASH_TOP, slash->x + 4, slash->y - 40,
                  16, 21);
          draw[0](canvas, work->sheet + SLASH_MIDDLE, slash->x - 16,
                  slash->y - 19, 29, 35);
          if (height > 0)
            draw[0](canvas, work->sheet + SLASH_BOTTOM, slash->x - 20,
                    slash->y + 16, 21, height);
          slash->x -= 6;
          slash->y += 12;
          if (slash->y > 79) {
            s32 member;

            slash->variant = 0;
            work->shakeFrames = 2;
            Sound_PlayFar(134);
            member = i % 6;
            if (member < effect->count) {
              BattleEffect_TintBattler(effect->actors[member], 7, 5, member, 8);
              BattleMotion_ApplyKnockbackFar(effect->actors[member], 1);
            }
          }
        }
      }
    }
    BattleCamera_Shake(4, 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(188);
  HeapSlot_Release(104);
  BattleEffect_EndCanvasLayer();
}

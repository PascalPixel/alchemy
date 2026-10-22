// Draft of BattleEffect_RunAtalanta, summon scene 4 (BattleEffect_RunSceneEffect's case 4).

// Twin: The Broken Seal's BattleEffect_RunAtalanta (effect.c), call
// for call, with The Lost Age's changes:
// - the giant is eleven sprite objects (Func_0815b290, priority 1, their
// 1 KB of tiles copied to object VRAM) in a grid of four rows of three,
// the bottom row two pieces moved half a piece over, where The Broken
// Seal places nine resource objects; battle work byte 0x867 is set to
// 1 for them;
// - the targets come from work->effect, which the dispatcher stored;
// - after the giant, object palette 0's sixteen colors are copied into
// background palette 5 by DMA and color 0x5e takes object color 0xf4
// (Func_0814cca8, then Palette_LoadBasicObjColors);
// - blitters through BattleEffect_LoadBlitter, callbacks ordered 0xc80, the backdrop
// ResourceId_VioletSkyBackdrop, the skip on gInput.repeat.
// Resources not yet named in directory.s, by row and asset: 0xa6
// Summon_LeafWarriorTiles, 0x12b BattleEffect_SlashSheet, 0x163
// BattleEffect_LimePalette, 0x195 BattleEffect_StagedParticlePath and 0x178
// BattleEffect_EmberStreakSheet.

// Draft: differ 966 of 1115 (draft 1147).

// Left: allocation from the prologue on. The native keeps work in r9 for
// the whole function and spills the path pointer and the piece counter;
// here work is spilled (sp+52) and the counters take the high registers,
// which adds about 32 instructions of reloads. The giant's grid, the
// flashes and both slash loops follow the listing's blocks in order.

// Tried (2026-10-15):
// - greedy declaration-order sweep, two passes: 964.

#include "types.h"
#include "battle_display.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "battle_session.h"
#include "bg_scroll.h"
#include "callback_scheduler.h"
#include "dma.h"
#include "effect_step.h"
#include "graphics.h"
#include "input.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "mem.h"
#include "motobj.h"
#include "object_runtime.h"
#include "ram_buffer.h"
#include "resource.h"
#include "system.h"
#include "transform.h"
#include "vram_tab.h"

// The canvas layer and its helpers; the comments give The Broken Seal's
// names where the twin is known.
void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_EndCanvasLayer(void); // BattleEffect_EndCanvasLayer
void BattleEffect_ProcessPendingGraphicsTransfer(void); // BattleEffect_ProcessPendingGraphicsTransfer
void Func_0813baec(void); // BattleEffect_AdvanceScrollOnInterval
void Func_0813ba50(void); // BattleEffect_ConfigureEffectDisplay
void Func_0814cca8(void); // BattleEffect_SetupBlendedDisplay
void ObjectGroup_TickMemberTimers(void); // ObjectGroup_TickMemberTimers
void BattleEffect_LoadBlitter(s32 slot, s32 modeAndFlags); // BattleEffect_BuildBlitter
void Func_0814cd48(s32 actor, s32 palette, s32 frames, s32 timerSlot,
                   s32 timerFrames);       // ObjectGroup_UpdateMembers
void Func_08118088(s32 actor, s32 motion); // BattleMotion_ApplyVariantMotionFar
void Func_08158ce0(s32 x, s32 y);          // Camera_ApplyShake
void Func_08163c2c(s32 layer, s32 value);  // Func_08163c2c
void BattleEffect_SelectLivingTargets(struct BattleEffectArgument *effect); // SelectLivingTargets
void Func_080150e4(s32 angle); // SceneTransform_ApplyRoll
void Palette_LoadBasicObjColors(void);
void BattleBackground_LoadFar(s32 layer, s32 resource, s32 mode);
void Render_ApplyProjectedPlacementFar(void *object, s32 *position,
                                       const s32 *scale, s32 mode);

// A sprite object as Func_0815b290 returns it.
struct SpriteObject {
  u8 unk00[9];
  u8 tileHigh : 2;
  u8 priority : 2;
  u8 palette : 4;
  u8 unk0a[6];
  u8 slot; // its row of gResourceTableEntries
};

struct SpriteObject *Func_0815b290(s32 width, s32 height, u32 shape,
                                   u32 attributes);

// The Lost Age's blitter modes, BattleEffect_LoadBlitter's high nibble: 3 minus The
// Broken Seal's COMPOSITE_ADDITIVE, COMPOSITE_MAX and COMPOSITE_TRANSPARENT.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT };
#define BLIT_MODE(mode, flags) ((mode) << 4 | (flags))
#define CLIP (COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y)

#define TRAILS ((struct EffectStep *)Ram_MapCellBuffer)
#define MATRICES (Ram_MapCellBuffer + 0x3800)
#define OBJ_VRAM ((u8 *)0x06010000)
#define HI(v) (((s16 *)&(v))[1]) // the pixel half of a 16.16 value
#define GIANT_PIECES 11

// Places in the slash sheet.
enum SlashSheetCell {
  SLASH_FLASH_1 = 0x0000,
  SLASH_FLASH_2 = 0x0188,
  SLASH_FLASH_3_TOP = 0x057c,
  SLASH_FLASH_3_BOTTOM = 0x07d4,
  SLASH_FLASH_4_TOP = 0x0aaa,
  SLASH_FLASH_4_MIDDLE = 0x0c90,
  SLASH_FLASH_4_BOTTOM = 0x0e74,
  SLASH_FLASH_5_TOP = 0x1088,
  SLASH_FLASH_5_MIDDLE = 0x11f8,
  SLASH_FLASH_5_BOTTOM = 0x147c,
  SLASH_TOP = 0x16ac,
  SLASH_MIDDLE = 0x17fc,
  SLASH_BOTTOM = 0x1bf3
};

extern char ResourceId_LeafWarriorTiles;
extern char ResourceId_SlashSheet;
extern char ResourceId_LimePalette;
extern char ResourceId_Data134; // the particle sheet
extern char ResourceId_StagedParticlePath;
extern char ResourceId_VioletSkyBackdrop;
extern char ResourceId_EmberStreakSheet;
extern s32 Data_081983cc[];  // gAtalanta_UnitScale
extern u16 gData_08197410[]; // gBattleEffect_MoteCells
extern u16 gData_0819747a[]; // gBattleEffect_PuffArcCells
extern u8 gData_08197467[];  // gBattleEffect_PuffArcCellWidths
extern u8 gData_08197473[];  // gBattleEffect_PuffArcCellBiasY
extern u8 gData_0819746d[];  // gBattleEffect_PuffArcCellHeights

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
  s32 position[4];
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

  presentation = Ram_HeapSlots->transitionWork;
  tiles = Ram_MapCellBuffer;
  scrollX = gBgScroll[1].x;
  canvas = Ram_HeapSlots->battleFxWork[1];
  sheet = Ram_HeapSlots->unk64[0];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  BattleEffect_StartCanvasLayer(0x2000);
  REG_BG2PA = 0x100;
  Func_0813ba50();
  BG_PLTT[0] = 0;
  BG_PLTT[1] = 0;
  work->transferMode = 0;
  Scheduler_AddOrUpdateCallback(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  Func_08163c2c(0, 0);
  BattleEffect_SelectLivingTargets(work->effect);

  // The giant: eleven sprite objects with their tiles.
  ((u8 *)Ram_HeapSlots->battleWork)[0x867] = 1;
  {
    u8 *picture = Resource_GetTableEntry((s32)&ResourceId_LeafWarriorTiles);

    Iwram_CopyWords(OBJ_PLTT, picture, 512);
    Resource_DecodeType01(picture + 512, tiles);
  }
  for (i = 0; i != GIANT_PIECES; i++) {
    struct SpriteObject *piece = Func_0815b290(32, 32, 0x80002000, 0);

    piece->priority = 1;
    work->objects[i] = piece;
    Iwram_CopyWords(OBJ_VRAM + gResourceTableEntries[piece->slot].offset, tiles,
                    0x400);
    tiles += 0x400;
  }

  // The slashes, the sky and the giant's path.
  Resource_LoadAndDecompress((s32)&ResourceId_SlashSheet, work->sheet, 1, 1);
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_LimePalette), 128);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);
  path = Resource_GetTableEntry((s32)&ResourceId_StagedParticlePath);
  BattleEffect_LoadBlitter(104, BLIT_MODE(BLIT_MAX, CLIP));
  BattleEffect_LoadBlitter(188, BLIT_MODE(BLIT_ADDITIVE, CLIP));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
  gCameraSceneParameters[4] = 240;
  WaitFrames(1);
  BattleBackground_LoadFar(1, (s32)&ResourceId_VioletSkyBackdrop, 0);
  work->scrollTimer = 0;
  work->scrollInterval = 4;
  work->scrollStepX = -1;
  work->scrollStepY = 0;
  Scheduler_AddOrUpdateCallback(Func_0813baec, 0xc80);
  presentation->active = 1;
  Func_08163c2c(0, 1);
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
      Render_ResetTransformState();
      Func_080150e4(Random16() & 0xffff);
      SceneTransform_ApplyPitch(Random16() & 0xffff);
      SceneTransform_ApplyYaw(Random16() & 0xffff);
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
      position[3] = 0;
      position[1] = 0xff0000;
      for (row = 0; row != 4; row++) {
        for (column = 0; column != 3; column++) {
          if (row != 3 || column != 2) {
            position[0] = (pathX + 80 + column * 32) << 16;
            position[2] = (64 - pathY + row * 32) << 16;
            if (row == 3)
              position[0] += 32 << 16;
            Render_ApplyProjectedPlacementFar(work->objects[row * 3 + column],
                                              position, Data_081983cc, 0);
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
          Audio_PlayCue(212);
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
              draw[1](canvas, sheet + gData_08197410[4], screen.x - 2,
                      screen.y - 5, 5, 10);
            }
          }
        }
      }
    }
    work->transferPending = 1;
    WaitFrames(1);
  }

  // The giant leaves: the sky stops, the palette is restored and the
  // pieces go.
  Scheduler_RemoveCallback(Func_0813baec);
  presentation->active = 0;
  gBgScroll[1].x = scrollX;
  Func_0814cca8();
  Palette_LoadBasicObjColors();
  Dma_Set(OBJ_PLTT, (void *)(BG_PLTT + 80), 0x80000010,
          (volatile u32 *)0x040000d4);
  BG_PLTT[94] = OBJ_PLTT[244];
  for (i = 0; i != GIANT_PIECES; i++)
    ResourceObject_ReleaseFar(work->objects[i]);
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

            draw[i & 1](canvas, Ram_MapCellBuffer + gData_0819747a[cell],
                        slash->x - ((width = gData_08197467[cell]) >> 1) - 8,
                        slash->y + gData_08197473[cell] - 40, width,
                        gData_0819746d[cell]);
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
            Audio_PlayCue(134);
            member = i % 6;
            if (member < effect->count) {
              Func_0814cd48(effect->actors[member], 7, 5, member, 8);
              Func_08118088(effect->actors[member], 1);
            }
          }
        }
      }
    }
    Func_08158ce0(4, 8);
    ObjectGroup_TickMemberTimers();
    work->transferPending = 1;
    WaitFrames(1);
  }
  Scheduler_RemoveCallback(BattleEffect_ProcessPendingGraphicsTransfer);
  Runtime_ReleaseHeapBlock(188);
  Runtime_ReleaseHeapBlock(104);
  BattleEffect_EndCanvasLayer();
}

/// @file
/// Geode.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"
#include "sounds.h"

struct MeshTriangle;

#define DOME_ROWS 16
#define DOME_COLUMNS 16

/// The Venus Djinni tears up the battlefield itself: the screen is copied
/// into a texture and peeled up into a spinning dome over the target as the
/// palette fades, the Djinni flies across to it, and at frame 48 the dome
/// slams down in rings of dust before the backdrop fades back in.
///
/// Abilities: Geode.
void BattleEffect_RunGeode(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  // Unused: blit[1], the idle half of the pair, gives the stack frame its 4
  // bytes.
  DrawRectangle blit[2];
  s32 *transition;
  u8 *projected;
  struct ModelVertex *vertices;
  u8 *mesh;
  s32 row;
  s32 curl;
  s32 pitch;
  u8 *battle;
  u8 *picture;
  void *caster;
  s32 screenB[3];
  s32 screenA[3];
  struct ScreenPoint source;
  struct ScreenPoint target;
  struct ModelTexture texture;
  struct ModelTexture dust;
  struct RenderModel *model;
  struct RenderModel *rings;
  s32 frame;
  s32 i;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  transition = (s32 *)Ram_HeapSlots->battleBackground;
  BattleEffect_StartCanvasLayer(2);
  BattleEffect_RunDjinniEntrance(
      args,
      DJINNI_ENTRANCE(ELEMENT_VENUS, args->side,
                      DJINNI_ANCHOR_CENTRE_BRIGHTENING),
      screenA, screenB);
  BattleEffect_ConfigureEffectDisplay();
  REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG2 |
              WININ_WIN1_OBJ | WININ_WIN1_CLR;
  REG_WINOUT = WINOUT_WIN01_BG0 | WINOUT_WIN01_OBJ | WINOUT_WIN01_CLR |
               WINOUT_WINOBJ_ALL;
  transition[4] = 1;
  if (args->side == EFFECT_SIDE_ENEMIES) {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                        BLITTER_ADDITIVE);
  } else {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                        BLITTER_ADDITIVE);
  }
  blit[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  REG_BLDCNT = 0;
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME + 15);
  Task_Add(Display_ApplyBg2Affine, TASK_PASS_FRAME + 127);
  projected = work->sheet;
  vertices = (struct ModelVertex *)(work->sheet + 0x800);
  mesh = work->sheet + 0xc00;
  pitch = 0;
  battle = (u8 *)Ram_HeapSlots->battleWork;
  caster = BattleObject_GetFar(args->user)->object;
  gBgScroll[1].x = -56;
  // The battlefield's picture, later the dome's texture.
  picture = gMapGridCellBuffer;
  {
    u8 *cell = picture;
    s32 x;

    // Take the battlefield's picture off BG2's tiles, row by row.
    for (i = 0; i <= 119; i++) {
      for (x = 0; x <= 2047; x += 64) {
        Iwram_CopyWordsEntry(
            cell, (u8 *)BG_CHAR_BLOCK(2) + (((i / 8) << 8) + (i & 7)) * 8 + x,
            8);
        cell += 8;
      }
    }
  }
  Resource_LoadAndDecompress((s32)&ResourceId_VenusDjinnSmallSheet,
                             work->sheet + 0x65c0, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0DA, work->sheet + 0x4710, 0,
                             0);
  Iwram_CopyWordsEntry((void *)BG_PLTT,
                       Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB),
                       128);
  IO_WRITE_QUEUE_PUSH(0x04000020, 128, 0x20000);
  Actor_SetAnimationSpeedFar(caster, 48);
  Mesh_BuildGrid(mesh, DOME_COLUMNS, DOME_ROWS, 16, 8);

  for (frame = 0; frame != 74; frame++) {
    EffectPosition_ProjectBattler(args->user, &source);
    EffectPosition_ProjectBattler(args->targets[0], &target);

    // The backdrop's palette fades toward the battle's.
    if (frame <= 11) {
      u16 *from = (u16 *)(battle + 0x56c);
      u16 *to = (u16 *)(BG_PLTT + 96);

      for (i = 0; i != 128; i++, from++, to++) {
        s32 red = *from & 31;
        s32 green = (*from >> 5) & 31;
        s32 blue = (*from >> 10) & 31;
        s32 r = *to & 31;
        s32 g = (*to >> 5) & 31;
        s32 b = (*to >> 10) & 31;

        if (r > red + 3) {
          r -= 4;
        } else {
          r = red;
        }
        if (g > green + 3) {
          g -= 4;
        } else {
          g = green;
        }
        if (b > blue + 3) {
          b -= 4;
        } else {
          b = blue;
        }
        *to = (b << 10) | (g << 5) | r;
      }
    }
    if (frame == 1) {
      IO_WRITE_QUEUE_PUSH(0x04000000, 0x7541, 0x20000);
      BattleEffect_BuildCanvasTilemap();
    }
    if (frame == 34) {
      Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
      Task_Add(BattleEffect_ProcessPendingBlock2Transfer, TASK_PASS_FRAME + 15);
      IO_WRITE_QUEUE_PUSH(0x04000000, 0x7341, 0x20000);
      Iwram_ClearWordsEntry(canvas, 0x3c00);
      gBgScroll[1].y = 78;
      gBgScroll[1].x = 64 - source.x;
      REG_BG1CNT &= 0xfffc;
      REG_BG1CNT |= 1;
      REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
    }
    if (frame == 35) {
      Task_Remove(Display_ApplyBg2Affine);
      Task_Remove(BattleEffect_ProcessPendingBlock2Transfer);
      Task_Add(BattleEffect_ProcessPendingGraphicsTransfer,
               TASK_PASS_FRAME + 15);
      IO_WRITE_QUEUE_PUSH(0x04000028, 0, 0x30000);
      IO_WRITE_QUEUE_PUSH(0x0400002c, -0x1000, 0x30000);
      IO_WRITE_QUEUE_PUSH(0x04000020, 128, 0x20000);
      IO_WRITE_QUEUE_PUSH(0x04000050, 0x3f44, 0x20000);
    }
    if (frame == 18) {
      BattleEffect_TintBattler(args->user, TINT_KEEP, 2, 0, -1);
    }
    if (frame == 36) {
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
      IO_WRITE_QUEUE_PUSH(0x04000000, 0x7741, 0x20000);
    }
    if ((u32)(frame - 34) <= 5) {
      if (args->side == EFFECT_SIDE_ENEMIES) {
        REG_WIN0H = WIN_RANGE(32, 240);
        REG_WIN1H = WIN_RANGE(32, 240);
      } else {
        REG_WIN0H = WIN_RANGE(0, 208);
        REG_WIN1H = WIN_RANGE(0, 208);
      }
    } else if (frame == 40) {
      REG_WIN0H = WIN_RANGE(0, 240);
      REG_WIN1H = WIN_RANGE(0, 240);
    }

    // The Djinni flies to the target.
    if ((u32)(frame - 36) <= 25) {
      s32 t = frame - 36;
      s32 x;
      s32 y;

      if (t > 12) {
        t = 12;
      }
      x = source.x + (target.x - source.x) * t / 12;
      y = (t << 6) / 12 + 16;
      gBgScroll[1].x = 64 - x;
      gBgScroll[1].y = t * -46 / 12 + 78;
      if (args->side == EFFECT_SIDE_ENEMIES) {
        x = x / 2 + 20;
      } else {
        x = x / 2 - 20;
      }
      blit[0](canvas, work->sheet + 0x65c0, x - 10, y - 48, 20, 40);
      blit[0](canvas, work->sheet + 0x65c0, x - 10, y - 48, 20, 40);
    }

    // The battlefield peels up into a dome.
    if ((u32)frame <= 34) {
      s32 size;
      s32 centerX;
      s32 centerY;
      s32 height;
      s32 angle;

      curl = frame;
      if (curl > 16) {
        curl = 16;
      }
      size = 64 - curl * 2;
      // DEAD: the ROM sets these defaults and tests frame >= 0, which inside
      // this range is always true, so the defaults are always overwritten.
      centerX = 120;
      centerY = -16;
      if (frame >= 0) {
        s32 f = frame;

        if (f > 16) {
          f = 16;
        }
        centerX = (source.x - 120) * f / 16 + 120;
        centerY = 46 * f / 16 - 16;
      }
      work->bg2x = (64 - (centerX << 5) / size) << 8;
      work->bg2y = centerY << 8;
      {
        s32 ratio = 0x2000 / size;

        // DEAD: the ROM reads each scale before storing it and drops the value
        // (as plain stores, 4 of 1139 differ).
        *(volatile u16 *)&work->bg2pa;
        work->bg2pa = ratio;
        *(volatile u16 *)&work->bg2pd;
        work->bg2pd = 0x100;
      }
      for (row = 0, height = -64, angle = 0; row != DOME_ROWS;
           height += 8, angle += 2184, row++) {
        s32 lift = ((-Trig_Cos(angle) * 3) << 4) >> 16;
        s32 reach = (Trig_Sin(angle) * 48) >> 16;
        s32 column;
        s32 y;
        s32 radius;
        y = (lift - height) * curl / 16 + height;
        radius = (reach - 56) * curl / 16 + 56;
        for (column = 0; column != DOME_COLUMNS; column++) {
          vertices[row * DOME_COLUMNS + column].x =
              (Trig_Sin(column << 12) * radius) >> 16;
          vertices[row * DOME_COLUMNS + column].y = y;
          vertices[row * DOME_COLUMNS + column].z =
              (Trig_Cos(column << 12) * radius) >> 16;
        }
      }
      model = Render_AllocateModelList(1);
      texture.logWidth = 8;
      texture.logHeight = 7;
      texture.pixels = picture;
      model->flags = 3;
      model->kind = 8;
      model->texture = &texture;
      model->mesh = mesh;
      model->vertices = projected;
      model->scrollU = 0;
      model->scrollV = 0;
      Transform_Reset();
      if (frame > 7) {
        pitch += Trig_Sin(frame * 924 - 7392) >> 5;
        Transform_ApplyPitch(pitch * 3 / 2);
      }
      Transform_ApplyYaw(frame << 9);
      Render_TransformVertices(vertices, projected, 256);
      Render_DrawModelList(model);
      Heap_Release(model);
    }
    if (frame == 48) {
      Sound_PlayFar(SE_CRASH);
      Iwram_FillWordsEntry(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(63));
    }
    if (frame == 64) {
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
    }

    // Rings of dust.
    if (frame > 47) {
      void *ring = Heap_AllocEwram(0x200);

      rings = Render_AllocateModelList(1);
      dust.logWidth = 6;
      dust.logHeight = 6;
      dust.pixels = work->sheet + 0x4710;
      rings->kind = 7;
      rings->texture = &dust;
      rings->mesh = gBattleEffect_RingMesh;
      rings->vertices = ring;
      rings->scrollU = 0;
      rings->scrollV = 0;
      for (i = 0; i != 4; i++) {
        s32 start = gGeode_WaveStarts[i] + 48;
        s32 scale;
        s32 shade;

        if (frame <= start) {
          continue;
        }
        scale =
            (((frame - start) * gGeode_WaveGrowth[i] * 125) << 4) + Q16_ONE / 2;
        shade = (start - frame) * 8 + 56;
        if (shade > 0) {
          shade = 0;
        }
        if (shade <= -64) {
          continue;
        }
        rings->shade = shade;
        Transform_Reset();
        SceneTransform_ApplyPosition3(px(target.x / 2 - 64),
                                      px(gGeode_WaveHeights[i]), 0);
        SceneTransform_ApplyScale3(scale >> 1, scale << 2, scale >> 1);
        Transform_ApplyPitch(1000);
        if (i & 1) {
          Transform_ApplyYaw((i * 4 + frame) << 11);
        } else {
          Transform_ApplyYaw((i * 4 - frame) << 11);
        }
        Render_TransformVertices(gGeode_RingCorners, ring, 32);
        Render_DrawModelList(rings);
      }
      Heap_Release(rings);
      Heap_Release(ring);
    }
    if (frame == 61) {
      transition[4] = 0;
      Iwram_FillWordsEntry((void *)(BG_PLTT + 96), 256, 0);
    }
    if (frame == 62) {
      BattleBackground_LoadFar(1, *(u16 *)(battle + 0x670), -1);
    }
    if ((u32)(frame - 63) <= 4) {
      *(s32 *)(battle + 0x66c) = (frame << 13) - 0x7c000;
      ColorBuffer_ScaleFar(battle + 0x56c, (void *)(BG_PLTT + 96),
                           *(s32 *)(battle + 0x66c), 128);
    }
    if (frame == 48) {
      work->shakeFrames = 16;
    }
    if (frame > 47) {
      BattleCamera_Shake(8, 8);
    }
    EFFECT_END_FRAME(work);
  }
  Actor_SetAnimationSpeedFar(caster, 16);
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}
#undef BLITTER
#undef DOME_ROWS
#undef DOME_COLUMNS

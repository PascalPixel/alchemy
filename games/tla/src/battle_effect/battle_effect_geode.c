/// @file
/// Geode.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

struct MeshTriangle;

#define DOME_ROWS 16
#define DOME_COLUMNS 16

/// Where VenusDjinnSmallSheet is decoded in the effect's pictures.
#define GEODE_VENUS_DJINN_SMALL_PICTURES 26048

#define GEODE_FRAMES 74

/// Where BillowTexture is decoded in the effect's pictures.
#define GEODE_BILLOW 18192
/// The dome's projected corners, its corners and its mesh, in the effect's
/// pictures.
#define GEODE_VERTICES 2048
#define GEODE_MESH 3072

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
  struct BattleBackgroundWork *backdrop;
  u8 *projected;
  struct ModelVertex *vertices;
  u8 *mesh;
  s32 row;
  s32 curl;
  s32 pitch;
  struct BattleWork *battle;
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
  backdrop = Ram_HeapSlots->battleBackground;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(2));
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
  backdrop->busy = 1;
  if (args->side == EFFECT_SIDE_ENEMIES) {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                        BLITTER_ADDITIVE);
  } else {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                        BLITTER_ADDITIVE);
  }
  blit[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  REG_BLDCNT = 0;
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME + 15);
  Task_Add(Display_ApplyBg2Affine, TASK_PASS_FRAME + 127);
  projected = work->sheet;
  vertices = (struct ModelVertex *)(work->sheet + GEODE_VERTICES);
  mesh = work->sheet + GEODE_MESH;
  pitch = 0;
  battle = Ram_HeapSlots->battleWork;
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
  Resource_LoadAndDecompress(RESOURCE_VENUS_DJINN_SMALL_SHEET,
                             work->sheet + GEODE_VENUS_DJINN_SMALL_PICTURES, 1,
                             1);
  Resource_LoadAndDecompress(RESOURCE_BILLOW_TEXTURE,
                             work->sheet + GEODE_BILLOW, 0, 0);
  Iwram_CopyWordsEntry((void *)BG_PLTT,
                       Resource_GetTableEntry(RESOURCE_ORANGE_PALETTE_B),
                       SHEET_PALETTE_BYTES);
  IO_WRITE_QUEUE_PUSH((u32)&REG_BG2PA, Q_8_8(0.5), IO_WRITE_HALFWORD);
  Actor_SetAnimationSpeedFar(caster, ANIMATION_STEP_NORMAL * 3);
  Mesh_BuildGrid(mesh, DOME_COLUMNS, DOME_ROWS, 16, 8);

  for (frame = 0; frame != GEODE_FRAMES; frame++) {
    EffectPosition_ProjectBattler(args->user, &source);
    EffectPosition_ProjectBattler(args->targets[0], &target);

    // The backdrop's palette fades toward the battle's.
    if (frame <= 11) {
      u16 *from = battle->palette;
      u16 *to = (u16 *)&BG_PLTT_COLOR(6, 0);

      for (i = 0; i != BATTLE_BACKGROUND_COLORS; i++, from++, to++) {
        s32 red = RGB_RED(*from);
        s32 green = RGB_GREEN(*from);
        s32 blue = RGB_BLUE(*from);
        s32 r = RGB_RED(*to);
        s32 g = RGB_GREEN(*to);
        s32 b = RGB_BLUE(*to);

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
        *to = RGB(r, g, b);
      }
    }
    if (frame == 1) {
      IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                          DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                              DISPCNT_BG2_ON | DISPCNT_OBJ_ON |
                              DISPCNT_WIN0_ON | DISPCNT_WIN1_ON,
                          IO_WRITE_HALFWORD);
      BattleEffect_BuildCanvasTilemap();
    }
    if (frame == 34) {
      Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
      Task_Add(BattleEffect_ProcessPendingBlock2Transfer, TASK_PASS_FRAME + 15);
      IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                          DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                              DISPCNT_BG1_ON | DISPCNT_OBJ_ON |
                              DISPCNT_WIN0_ON | DISPCNT_WIN1_ON,
                          IO_WRITE_HALFWORD);
      Iwram_ClearWordsEntry(canvas, CANVAS_VIEW_BYTES);
      gBgScroll[1].y = 78;
      gBgScroll[1].x = 64 - source.x;
      REG_BG1CNT &= ~BGCNT_PRIORITY(3);
      REG_BG1CNT |= BGCNT_PRIORITY(1);
      REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
    }
    if (frame == 35) {
      Task_Remove(Display_ApplyBg2Affine);
      Task_Remove(BattleEffect_ProcessPendingBlock2Transfer);
      Task_Add(BattleEffect_ProcessPendingGraphicsTransfer,
               TASK_PASS_FRAME + 15);
      IO_WRITE_QUEUE_PUSH((u32)&REG_BG2X, 0, IO_WRITE_WORD);
      IO_WRITE_QUEUE_PUSH((u32)&REG_BG2Y, -(BATTLE_VIEW_TOP << 8),
                          IO_WRITE_WORD);
      IO_WRITE_QUEUE_PUSH((u32)&REG_BG2PA, Q_8_8(0.5), IO_WRITE_HALFWORD);
      IO_WRITE_QUEUE_PUSH((u32)&REG_BLDCNT,
                          BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND |
                              BLDCNT_TGT2_ALL,
                          IO_WRITE_HALFWORD);
    }
    if (frame == 18) {
      BattleEffect_TintBattler(args->user, TINT_KEEP, 2, 0, -1);
    }
    if (frame == 36) {
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
      IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, EFFECT_DISPCNT, IO_WRITE_HALFWORD);
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
      REG_WIN0H = WIN_RANGE(0, DISPLAY_WIDTH);
      REG_WIN1H = WIN_RANGE(0, DISPLAY_WIDTH);
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
      blit[0](canvas, work->sheet + GEODE_VENUS_DJINN_SMALL_PICTURES, x - 10,
              y - 48, SMALL_DJINNI_WIDTH, SMALL_DJINNI_HEIGHT);
      blit[0](canvas, work->sheet + GEODE_VENUS_DJINN_SMALL_PICTURES, x - 10,
              y - 48, SMALL_DJINNI_WIDTH, SMALL_DJINNI_HEIGHT);
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
      // DEAD: frame >= 0 is always true inside this range, so the defaults are
      // always overwritten; the ROM keeps both moves and the compare.
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
        s32 ratio = 8192 / size;

        work->bg2pa = ratio;
        work->bg2pd = Q_8_8(1);
      }
      for (row = 0, height = -64, angle = 0; row != DOME_ROWS;
           height += 8, angle += 2184, row++) {
        s32 lift = FIXEDtoINT((-Trig_Cos(angle) * 3) << 4);
        s32 reach = FIXEDtoINT(Trig_Sin(angle) * 48);
        s32 column;
        s32 y;
        s32 radius;
        y = (lift - height) * curl / 16 + height;
        radius = (reach - 56) * curl / 16 + 56;
        for (column = 0; column != DOME_COLUMNS; column++) {
          vertices[row * DOME_COLUMNS + column].x =
              FIXEDtoINT(Trig_Sin(column << 12) * radius);
          vertices[row * DOME_COLUMNS + column].y = y;
          vertices[row * DOME_COLUMNS + column].z =
              FIXEDtoINT(Trig_Cos(column << 12) * radius);
        }
      }
      model = Render_AllocateModelList(1);
      texture.logWidth = 8;
      texture.logHeight = 7;
      texture.pixels = picture;
      model->flags = 3;
      model->kind = MODEL_TEXTURED;
      model->texture = &texture;
      model->mesh = mesh;
      model->vertices = projected;
      model->scrollU = 0;
      model->scrollV = 0;
      Matrix_Initialize();
      if (frame > 7) {
        pitch += Trig_Sin(frame * 924 - 7392) >> 5;
        Matrix_RotateX(pitch * 3 / 2);
      }
      Matrix_RotateY(frame << 9);
      Render_TransformVertices(vertices, projected, 256);
      Render_DrawModelList(model);
      Heap_Release(model);
    }
    if (frame == 48) {
      Sound_PlayFar(SE_CRASH);
      Iwram_FillWordsEntry(canvas, CANVAS_BYTES,
                           CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
    }
    if (frame == 64) {
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
    }

    // Rings of dust.
    if (frame > 47) {
      void *ring = Heap_AllocEwram(512);

      rings = Render_AllocateModelList(1);
      dust.logWidth = 6;
      dust.logHeight = 6;
      dust.pixels = work->sheet + GEODE_BILLOW;
      rings->kind = MODEL_SHADED_ADDITIVE;
      rings->texture = &dust;
      rings->mesh = gBattleEffect_RingMesh;
      rings->vertices = ring;
      rings->scrollU = 0;
      rings->scrollV = 0;
      for (i = 0; i != 4; i++) {
        s32 start = gGeode_WaveStarts[i] + 48;
        FIXED scale;
        s32 shade;

        if (frame <= start) {
          continue;
        }
        scale = (((frame - start) * gGeode_WaveGrowth[i] * 125) << 4) +
                FIXED_ONE / 2;
        shade = (start - frame) * 8 + 56;
        if (shade > 0) {
          shade = 0;
        }
        if (shade <= MODEL_SHADE_INVISIBLE) {
          continue;
        }
        rings->shade = shade;
        Matrix_Initialize();
        Matrix_Translate(INTtoFIXED(target.x / 2 - 64),
                         INTtoFIXED(gGeode_WaveHeights[i]), 0);
        Matrix_Scale(scale >> 1, scale << 2, scale >> 1);
        Matrix_RotateX(1000);
        if (i & 1) {
          Matrix_RotateY((i * 4 + frame) << 11);
        } else {
          Matrix_RotateY((i * 4 - frame) << 11);
        }
        Render_TransformVertices(gGeode_RingCorners, ring, 32);
        Render_DrawModelList(rings);
      }
      Heap_Release(rings);
      Heap_Release(ring);
    }
    if (frame == 61) {
      backdrop->busy = 0;
      Iwram_FillWordsEntry((void *)&BG_PLTT_COLOR(6, 0), 256, 0);
    }
    if (frame == 62) {
      BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED, battle->background, -1);
    }
    if ((u32)(frame - 63) <= 4) {
      battle->brightness = (frame << 13) - toFIXED(7.75);
      ColorBuffer_ScaleFar(battle->palette, (void *)&BG_PLTT_COLOR(6, 0),
                           battle->brightness, BATTLE_BACKGROUND_COLORS);
    }
    if (frame == 48) {
      work->shakeFrames = 16;
    }
    if (frame > 47) {
      BattleCamera_Shake(8, 8);
    }
    EFFECT_END_FRAME(work);
  }
  Actor_SetAnimationSpeedFar(caster, ANIMATION_STEP_NORMAL);
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}
#undef BLITTER
#undef DOME_ROWS
#undef DOME_COLUMNS

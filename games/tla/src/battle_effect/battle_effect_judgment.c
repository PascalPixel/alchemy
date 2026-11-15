/// @file
/// The Judgment summon.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

/// The battle wipes away to a violet sky where Judgment flies forward while
/// motes gather into it; it bursts in a flash, and the blast falls on the land
/// below and spreads in rings of light.
void BattleEffect_RunJudgment(struct BattleEffectParams *args) {
  s32 anchor[3];
  struct ScreenPoint pos;
  s32 place[4];
  s32 drift[4];
  struct Scale req;
  struct ModelTexture texture;
  struct ModelTexture glare;
  s32 scale[2];
  u8 *canvas;
  struct BattleEffectWork *work;
  s32 *transition;
  DrawRectangle draw[2];
  s32 frame;
  u8 *sprites;
  s32 depth;
  s32 zoom;
  s32 addR, addG, addB;
  s32 step;
  s32 cx, cy, squash, sx, ring, rings, shift;
  s32 i;
  u8 *cells;
  u8 *pal;
  s32 width;

  canvas = Ram_HeapSlots->battleCanvas;
  work = Ram_HeapSlots->battleEffect;
  transition = Ram_HeapSlots->battleBackground;
  sprites = Ram_HeapSlots->battleSheet;
  cells = Ram_MapCellBuffer;
  EFFECT_START_NARROW_CANVAS(0x2000);
  BattleEffect_ConfigureEffectDisplay();
  ((volatile u16 *)BG_PLTT)[0] = 0;
  ((volatile u16 *)BG_PLTT)[1] = 0;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
  work->transferMode = CANVAS_COPY;
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  BattleEffect_WipeCanvas(CANVAS_WIPE_DOWN, CANVAS_WIPE_FILL);
  BattleEffect_SelectLivingTargets(work->args);
  BattleBackground_LoadFar(1, (s32)&ResourceId_VioletSkyBackdrop, 0);

  pal = Resource_GetTableEntry((s32)&ResourceId_WingedKnightATiles);
  Iwram_CopyWords((void *)OBJ_PLTT, pal, 512);
  pal += 512;
  Decompress_TaggedLz(pal, cells);
  for (i = 0; i != 16; i++) {
    struct JudgmentObject *obj =
        EffectSprite_CreateWithTiles(32, 32, 0x80002000, 0);

    obj->priority = 2;
    work->sprites[i] = obj;
    Iwram_CopyWordsEntry((void *)OBJ_TILES(obj), cells + i * 0x400, 0x400);
  }
  gProjection.centerY = SUMMON_PROJECTION_CENTER_Y;
  wait(1);
  transition[4] = 1;
  gBgScroll[1].x = 0;
  BattleEffect_WipeCanvas(CANVAS_WIPE_DOWN, CANVAS_WIPE_CLEAR);
  EFFECT_SHOW_WIDE_CANVAS(16);
  depth = -0x400000;
  zoom = 0x80000;
  Resource_LoadAndDecompress((s32)&ResourceId_PinkBurstSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, work->sheet + 0x600,
                             1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C2, gData_02014000, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sprites, 0, 0);
  pal = Resource_GetTableEntry((s32)&ResourceId_BlueCrescentSheet);
  Iwram_CopyWordsEntry((void *)BG_PLTT, pal, SHEET_PALETTE_BYTES);
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);

  // The gathering: motes rise, the tiles fold in and flash.
  frame = 0;
  do {
    if (frame == 0) {
      struct Particle *p = work->particles;

      for (i = 0; i != 64; i++, p++) {
        p->x = (Random16() & 15) + 72;
        p->velocityX = Random16() & TRIG_ANGLE_MASK;
        p->velocityY = Random16() & TRIG_ANGLE_MASK;
        p->velocityY = Random16() & TRIG_ANGLE_MASK;
      }
      pal = Resource_GetTableEntry((s32)&ResourceId_WingedKnightATiles);
      Iwram_CopyWordsEntry((void *)OBJ_PLTT, pal, 512);
      pal += 512;
      Decompress_TaggedLz(pal, Ram_MapCellBuffer);
      for (i = 0; i != 16; i++) {
        struct JudgmentObject *obj = work->sprites[i];

        Iwram_CopyWordsEntry((void *)OBJ_TILES(obj),
                             Ram_MapCellBuffer + i * 0x400, 0x400);
      }
      depth = -0x400000;
      zoom = 0x80000;
    }
    if (frame == 143) {
      FILL__actor_move_random(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(42));
      Sound_PlayFar(SE_CRASH);
    }
    if (frame == 80) {
      Sound_PlayFar(SE_SWIRL);
    }
    if (frame == 72) {
      work->fadeFrames = BACKGROUND_DARKEN_FRAMES;
      work->fadeStep = 0;
    }

    anchor[2] = 0;
    anchor[1] = 0;
    {
      struct Particle *p = work->particles;

      for (i = 0; i != 64; i++, p++) {
        s32 size;

        if (frame > i / 4 + 80 && p->x > 0) {
          Transform_Reset();
          Transform_ApplyRoll(p->velocityY);
          Transform_ApplyPitch(p->velocityX);
          Transform_ApplyYaw(p->velocityY + (frame << 9));
          anchor[0] = p->x;
          p->x -= 2;
          if (p->x < 0) {
            p->x = 0;
          }
          EffectPosition_ProjectPoint(anchor, &pos);
          if (pos.depth < -60) {
            pos.depth = -60;
          }
          if (pos.depth > 60) {
            pos.depth = 60;
          }
          pos.depth += 60;
          size = pos.depth / 20 + 2;
          pos.x += 56;
          pos.y += 96;
          draw[0](canvas, sprites + gBattleEffect_MotePictures[size - 1],
                  pos.x - size / 2, pos.y - size, size, size * 2);
        }
      }
    }

    req = gJudgment_UnitScale;
    place[3] = 0;
    place[1] = 0;
    for (i = 0; i != 16; i++) {
      if (frame > 75 && (i == 8 || i == 12 || i == 15)) {
        continue;
      }
      place[0] = ((i % 4) << 21) + (152 << 15);
      place[2] = ((i / 4) << 21) + depth;
      Render_ApplyProjectedPlacementFar(work->sprites[i], place, (s32 *)&req,
                                        0);
    }
    depth += zoom;
    if (frame <= 47) {
      zoom -= 0x4000;
    }
    if (frame > 32) {
      zoom = zoom * 60 / 64;
    }
    if (frame == 144) {
      zoom = -0x80000;
    }
    if (frame == 146) {
      zoom = 0x10000;
    }

    if (frame == 72) {
      pal = Resource_GetTableEntry((s32)&ResourceId_WingedKnightBTiles);
      Iwram_CopyWordsEntry((void *)OBJ_PLTT, pal, 512);
      pal += 512;
      Decompress_TaggedLz(pal, Ram_MapCellBuffer);
      for (i = 0; i != 16; i++) {
        QUEUE_PUSH_PARTED(OBJ_TILES((struct JudgmentObject *)work->sprites[i]),
                          (u32)(cells + i * 0x400), 0x84000100);
      }
    }
    if (frame == 76) {
      pal = Resource_GetTableEntry((s32)&ResourceId_WingedKnightCTiles);
      Iwram_CopyWordsEntry((void *)OBJ_PLTT, pal, 512);
      pal += 512;
      Decompress_TaggedLz(pal, Ram_MapCellBuffer);
      for (i = 0; i != 13; i++) {
        IO_WRITE_QUEUE_PUSH(
            OBJ_TILES((struct JudgmentObject *)
                          work->sprites[gJudgment_BurstPictures[i]]),
            (u32)(PassBuffer(Ram_MapCellBuffer) + i * 0x400), 0x84000100);
      }
    }

    if ((u32)(frame - 116) <= 27) {
      s32 n = frame * 3 - 346;
      if (n > 80) {
        n = 80;
      } else {
        BattleEffect_DrawShadedCircles(work->sheet + 0x1256, n, 0x10000);
      }
      Canvas_BlitAtFoot(work->sheet + 0x1256, 60, 80, n);
      /* FAKEMATCH(dead): the test of the block it stands in, made again. */
      if ((u32)(frame - 116) <= 27) {
        s32 size = (frame - 116) / 3 + 16;

        if (size > 32) {
          size = 32;
        }
        for (i = 0; i != 6; i++) {
          s32 k = i & 3;
          s32 a = Random16() & TRIG_ANGLE_MASK;
          s32 x = (Q_16_16_TO_INT(size * Trig_Sin(a))) -
                  (gJudgment_ShardWidths[k] >> 1);
          s32 y = (Q_16_16_TO_INT(size * Trig_Cos(a))) -
                  (gJudgment_ShardHeights[k] / 2);

          draw[0](canvas, work->sheet + gJudgment_ShardPictures[k], x + 60,
                  y + 80, gJudgment_ShardWidths[k], gJudgment_ShardHeights[k]);
        }
      }
    }

    {
      s32 flag = 0;

      if ((u32)(frame - 108) <= 34) {
        s32 t = (frame - 108) / 2;

        addR = t / 4;
        addG = t / 2;
        addB = t;
        flag = 1;
      }
      if (frame == 143) {
        addB = addG = addR = 31;
        flag = 1;
      }
      if ((u32)(frame - 144) <= 8) {
        /* FAKEMATCH(temporary): the step taken through a local assigned twice.
         */
        addB = addR = addG = step = (step = 304 - frame * 2);
        flag = 1;
      }
      if (flag == 1) {
        u16 *src = Resource_GetTableEntry((s32)&ResourceId_WingedKnightCTiles);
        volatile u16 *dst = (volatile u16 *)OBJ_PLTT;

        for (i = 0; i != 256; i++, src++, dst++) {
          s32 r = RGB_RED(*src) + addR;
          s32 g = RGB_GREEN(*src) + addG;
          s32 b = RGB_BLUE(*src) + addB;

          if (r > 31)
            r = 31;
          if (g > 31)
            g = 31;
          if (b > 31)
            b = 31;
          if (r < 0)
            r = 0;
          if (g < 0)
            g = 0;
          if (b < 0)
            b = 0;
          *dst = RGB(r, g, b);
        }
      }
    }

    if (frame > 143) {
      s32 y = frame * 16 - 2272;

      DRAW_ALT(3, work->sheet, 36, y, 24, 64);
      /* FAKEMATCH(dead): a test that is never true. */
      if (!cells && !canvas)
        y = 0;
      DRAW_ALT(7, work->sheet, 60, y, 24, 64);
    }

    if ((u32)(frame - 144) <= 15) {
      s32 s;
      s32 shade;
      void *verts;
      struct RenderModel *model;

      verts = Heap_AllocEwram(32);
      model = Render_AllocateModelList(1);
      shade = 0;
      s = (frame << 13) - 0x11e000;
      texture.logWidth = 7;
      texture.logHeight = 7;
      texture.pixels = gData_02014000;
      model->texture = &texture;
      model->kind = 7;
      model->mesh = gBattleEffect_LargeSquareMesh;
      model->vertices = verts;
      if (frame > 151) {
        shade = (frame * -8) + 1216;
      }
      model->shade = shade;
      Transform_Reset();
      Transform_ApplyTranslation(-px(4), px(24), 0);
      Transform_ApplyUniformScale(s);
      Transform_ApplyPitch(3000);
      Render_TransformVertices(gBattleEffect_GroundSquareCorners, verts, 4);
      Render_DrawModelList(model);
      Heap_Release(model);
      Heap_Release(verts);
    }

    EFFECT_PRESENT_CANVAS(work);
    frame++;
  } while (frame != 160 && (frame <= 4 || !(gInput.repeat & (KEY_A | KEY_B))));

  Iwram_FillWords(canvas, CANVAS_BYTES, 0);
  for (i = 0; i != 16; i++) {
    Sprite_DestroyFar(work->sprites[i]);
  }
  wait(1);
  Palette_LoadBasicSpriteColors();
  Dma_Set((const void *)OBJ_PLTT, (void *)&BG_PLTT_COLOR(5, 0),
          DMA_COPY_HALFWORDS(16), REG_DMA3);
  BG_PLTT_COLOR(5, 14) = BG_PLTT_COLOR(15, 4);
  for (i = 0; i != 16; i++) {
    struct JudgmentObject *obj = Sprite_CreateFar(0x28d);

    work->sprites[i] = obj;
    if (obj) {
      obj->unk1a = 0;
      Sprite_StartAnimationFar(obj, i % 3);
      ((struct JudgmentObject *)work->sprites[i])->priority = 0;
    }
  }
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  Resource_LoadAndDecompress((s32)&ResourceId_BlueCrescentSheet,
                             work->sheet + 0x4000, 1, 1);
  {
    u8 *c = sprites;

    for (i = 0; i != 770; i++, c++) {
      if (*c > 32) {
        *c += 224;
      } else if (*c != 0) {
        *c = 1;
      }
    }
  }
  work->fadeFrames = 0;
  work->fadeStep = 0;
  BattleBackground_LoadFar(1, (s32)&ResourceId_AerialViewBackdrop, 0);
  REG_BG2PA = Q_8_8(1);
  REG_BG2X = -0x3c00;
  REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
  FILL__actor_move_random(canvas, CANVAS_BYTES, 0);
  cx = 60;
  cy = 44;
  squash = 6;
  ring = 0;
  shift = 0;
  sx = 8;
  rings = 2;
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  Iwram_FillWordsEntry(work->sheet, 0x3840, 0);

  // The strike: the beam, the shockwave rings and the debris.
  for (frame = 0; frame != 336; frame++) {
    if (frame == 66) {
      Sound_PlayFar(SE_BANE_ESCAPE);
    }
    if (frame == 88) {
      Sound_PlayFar(SE_230);
    }
    if (frame == 155) {
      Sound_PlayFar(SE_162);
    }
    if (frame == 217) {
      Sound_PlayFar(SE_SWITCH_MECHANISM);
    }
    if (frame == 280) {
      Sound_PlayFar(SE_GATE_MOVE);
    }
    if (frame == 300) {
      for (i = 0; i != args->targetCount; i++) {
        EFFECT_STRIKE_TARGET(args, i, 8, KNOCKBACK_HOP);
      }
      EFFECT_SHOW_DAMAGE(SE_CRASH);
    }
    if (gInput.repeat & (KEY_A | KEY_B)) {
      if ((u32)(frame - 5) <= 144) {
        frame = 150;
        IO_WRITE_QUEUE_PUSH((u32)&REG_BG2PA, 128, IO_WRITE_16);
        IO_WRITE_QUEUE_PUSH((u32)&REG_BG2X, 0, IO_WRITE_32);
        Resource_LoadAndDecompress((s32)&ResourceId_ExplosionSheet,
                                   work->sheet + 0x4000, 1, 0);
      } else if ((u32)(frame - 155) <= 58) {
        frame = 214;
      } else if ((u32)(frame - 219) <= 60) {
        frame = 280;
      }
    }
    if (frame == 318) {
      FILL__actor_move_random(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(63));
    }
    if (frame == 319) {
      Graphics_LoadGreyRamp();
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(4));
    }
    if (frame == 64) {
      IO_WRITE_QUEUE_PUSH((u32)&REG_BG2PA, 128, IO_WRITE_16);
      IO_WRITE_QUEUE_PUSH((u32)&REG_BG2X, 0, IO_WRITE_32);
      FILL__actor_move_random(canvas, CANVAS_BYTES, 0);
      Resource_LoadAndDecompress((s32)&ResourceId_ExplosionSheet,
                                 work->sheet + 0x4000, 1, 0);
      Resource_LoadAndDecompress((s32)&ResourceId_OverlayImageA,
                                 work->sheet + 0x6000, 0, 0);
    }
    if (frame == 66) {
      volatile u16 *c = &BG_PLTT_COLOR(6, 0);

      for (i = 0; i != 128; i++, c++) {
        *c ^= 0x7fff;
      }
    }
    if (frame == 69) {
      FILL__actor_move_random(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(63));
    }
    if (frame == 70) {
      BattleBackground_SetModeFar(1, (s32)&ResourceId_AerialViewBackdrop, 7);
    }
    if (frame == 150) {
      cx = 112;
      cy = 32;
      ring = 0;
      rings = 4;
      squash = 8;
      FILL__actor_move_random(work->sheet, 0x3840, 0);
      FILL__actor_move_random(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(63));
      BattleBackground_LoadFar(1, (s32)&ResourceId_BeachBackdrop, 0);
    }
    if (frame == 214) {
      FILL__actor_move_random(work->sheet, 0x3840, 0);
      FILL__actor_move_random(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(63));
      BattleBackground_LoadFar(1, (s32)&ResourceId_ForestBackdrop, 0);
      {
        struct Particle *p = work->particles;

        for (i = 0; i != 16; i++, p++) {
          p->x = px((Random16() & 63) + 32);
          p->y = px((Random16() & 31) + 96);
          p->velocityY = ((Random16() & 15) + 16) << 15;
          p->z = px(1) / 2 - (p->x >> 7);
          p->variant = 1;
        }
      }
    }
    if (frame == 280) {
      BattleEffect_SetupBlendedDisplay();
      FILL__actor_move_random(work->sheet, 0x3840, 0);
      FILL__actor_move_random(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(1));
      transition[4] = 0;
      REG_BLDALPHA = EFFECT_BLEND_ADD;
      {
        struct Particle *p = work->particles;

        for (i = 0; i != 16; i++, p++) {
          p->x = px((Random16() & 63) + 256);
          p->y = px((Random16() & 31) + 96);
          p->velocityY = ((Random16() & 15) + 16) << 15;
          p->z = 0;
        }
      }
      shift = 272;
      EFFECT_SET_TRANSFER(work, CANVAS_COPY, 0);
    }
    if ((u32)(frame - 280) <= 38) {
      volatile u16 *c = &BG_PLTT_COLOR(0, 1);

      for (i = 0; i != 63; i++, c++) {
        s32 v = *c;
        s32 b = ((u16)v >> 10) & 31;
        s32 g = ((u16)v >> 5) & 31;
        s32 r = v & 31;

        b++;
        g++;
        r++;

        if (b > 31)
          b = 31;
        if (g > 31)
          g = 31;
        if (r > 31)
          r = 31;
        *c = RGB(r, g, b);
      }
    }
    if (frame == 182) {
      FILL__actor_move_random(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(63));
    }

    if (frame <= 63) {
      u8 *sheet;
      s32 top = frame - 4;
      DrawRectangle *slot;

      if ((frame & 7) < 4) {
        sheet = work->sheet + 0x4000;
      } else {
        sheet = work->sheet + 0x4240;
      }
      slot = (DrawRectangle *)&Ram_HeapSlots->ramCodePaired;
      DRAW_ALT_THROUGH(slot, 19, sheet, 36, top - 24, 24, 24);
      DRAW_ALT_THROUGH(slot, 23, sheet, 59, top - 24, 24, 24);
      DRAW_ALT_THROUGH(slot, 27, sheet, 36, top - 1, 24, 24);
      DRAW_ALT_THROUGH(slot, 31, sheet, 59, top - 1, 24, 24);
    }
    if ((u32)(frame - 64) <= 1) {
      draw[0](canvas, work->sheet + 0x6000, 52, 51, 16, 17);
    }
    if ((u32)(frame - 66) <= 1) {
      /* FAKEMATCH(temporary): the width through a local set at both draws of
       * this picture. */
      width = 24;
      draw[0](canvas, work->sheet + 0x6110, 48, 40, width, 41);
    }
    if ((u32)(frame - 68) <= 7) {
      s32 d = 76 - frame;

      draw[0](canvas, work->sheet + 0x64e8, 49 - d / 2, 38 - d, 22, 44);
      draw[0](canvas, work->sheet + 0x64e8, 49 - d / 2, d + 38, 22, 44);
      draw[0](canvas, work->sheet + 0x64e8, d / 2 + 49, 38 - d, 22, 44);
      draw[0](canvas, work->sheet + 0x64e8, d / 2 + 49, d + 38, 22, 44);
    }
    if ((u32)(frame - 78) <= 1) {
      draw[0](canvas, work->sheet + 0x6000, 52, 51, 16, 17);
    }
    if ((u32)(frame - 80) <= 1) {
      width = 24;
      draw[0](canvas, work->sheet + 0x6110, 48, 40, width, 41);
    }
    if ((u32)(frame - 82) <= 3) {
      s32 d = frame * 2 - 164;

      draw[0](canvas, work->sheet + 0x64e8, 49 - d / 4, 38 - d, 22, 44);
      draw[0](canvas, work->sheet + 0x64e8, 49 - d / 4, d + 38, 22, 44);
      draw[0](canvas, work->sheet + 0x64e8, d / 4 + 49, 38 - d, 22, 44);
      draw[0](canvas, work->sheet + 0x64e8, d / 4 + 49, d + 38, 22, 44);
    }

    if ((u32)(frame - 72) <= 15) {
      s32 s;
      s32 shade;
      void *verts;
      struct RenderModel *model;

      verts = Heap_AllocEwram(32);
      model = Render_AllocateModelList(1);
      s = (frame << 13) - 0x8f800;
      glare.logWidth = 7;
      glare.logHeight = 7;
      glare.pixels = gData_02014000;
      shade = 0;
      model->kind = 7;
      model->texture = &glare;
      model->mesh = gBattleEffect_LargeSquareMesh;
      model->vertices = verts;
      if (frame > 79) {
        shade = (frame * -8) + 640;
      }
      model->shade = shade;
      Transform_Reset();
      Transform_ApplyTranslation(-px(4), px(8), 0);
      Transform_ApplyUniformScale(s);
      Transform_ApplyPitch(5000);
      Render_TransformVertices(gBattleEffect_GroundSquareCorners, verts, 4);
      Render_DrawModelList(model);
      Heap_Release(model);
      Heap_Release(verts);
    }

    if (frame > 85) {
      if (frame <= 213) {
        for (i = 0; i != rings; i++, ring++) {
          s32 radius = ring;
          s32 slope = 0;
          s32 err = ring;

          while (radius >= slope) {
            s32 half, halfSlope, top, bottom, left, right, dx;

            half = radius / 2;
            dx = (half * sx) / 8;
            /* FAKEMATCH(dead): a store top's own line overwrites. */
            if (dx > 1000)
              top = 0;
            right = cx + dx;
            /* FAKEMATCH(dead): a branch whose store the next line overwrites
             * restores the label CSE and reload saw between the edges. */
            if (dx > cx)
              left = 0;
            left = cx - dx;
            bottom = cy + (slope * squash) / 8;
            top = cy - (slope * sx) / 8;
            if (top < 0)
              top = 0;
            if (bottom > 119)
              bottom = 119;
            if (left < 0)
              left = 0;
            if (right > 119)
              right = 119;
            work->sheet[bottom * 120 + right] = 20;
            work->sheet[top * 120 + right] = 20;
            work->sheet[bottom * 120 + left] = 20;
            work->sheet[top * 120 + left] = 20;
            right = cx + half + 1;
            left = cx - half + 1;
            if (left < 0)
              left = 0;
            if (right > 119)
              right = 119;
            work->sheet[bottom * 120 + right] = 20;
            work->sheet[top * 120 + right] = 20;
            work->sheet[bottom * 120 + left] = 20;
            work->sheet[top * 120 + left] = 20;

            halfSlope = slope / 2;
            dx = (halfSlope * sx) / 8;
            /* FAKEMATCH(dead): as above, the pick before cx's reload in the
             * second half. */
            if (dx > 1000)
              top = 0;
            right = cx + dx;
            /* FAKEMATCH(dead): a branch whose store the next line overwrites
             * restores the label CSE and reload saw between the edges. */
            if (dx > cx)
              left = 0;
            left = cx - dx;
            bottom = cy + (radius * squash) / 8;
            top = cy - (radius * sx) / 8;
            if (left < 0)
              left = 0;
            if (right > 119)
              right = 119;
            if (top < 0)
              top = 0;
            if (bottom > 119)
              bottom = 119;
            work->sheet[bottom * 120 + right] = 20;
            work->sheet[top * 120 + right] = 20;
            work->sheet[bottom * 120 + left] = 20;
            work->sheet[top * 120 + left] = 20;
            right = cx + halfSlope + 1;
            left = cx - halfSlope + 1;
            if (left < 0)
              left = 0;
            if (right > 119)
              right = 119;
            work->sheet[bottom * 120 + right] = 20;
            work->sheet[top * 120 + right] = 20;
            work->sheet[bottom * 120 + left] = 20;
            work->sheet[top * 120 + left] = 20;

            err = err - slope * 2 - 1;
            if (err < 0) {
              err = err + radius * 2 - 2;
              radius--;
            }
            slope++;
          }
        }
      }
    }

    if ((u32)(frame - 86) <= 63) {
      s32 angle = -TRIG_TURN / 4;

      for (i = 0; i != 9; i++, angle += TRIG_TURN / 16) {
        s32 x = Q_16_16_TO_INT((frame - 86) * Trig_Sin(angle));
        s32 y = Q_16_16_TO_INT((frame - 70) * Trig_Cos(angle));

        draw[0](canvas, work->sheet + ((frame / 2 + i) % 4) * 1728 + 0x4000,
                x + 44, y + 17, 32, 54);
      }
    }

    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                        BLITTER_ADDITIVE);
    draw[1] = (DrawRectangle)Ram_HeapSlots->ramCodePaired;
    if (frame > 85) {
      draw[1](canvas, work->sheet, 0, 0, 120, 120);
    }
    if (frame > 279) {
      shift -= 8;
    }
    if (frame == 238) {
      FILL__actor_move_random(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(63));
    }
    if ((u32)(frame - 214) <= 65) {
      gBgScroll[1].x += 8;
    }
    if ((u32)(frame - 246) <= 33) {
      shift += 8;
      EFFECT_SET_TRANSFER(work, CANVAS_COPY, 75);
    }

    if (frame > 213) {
      struct Particle *p;

      drift[3] = 0;
      drift[1] = 0;
      for (i = 0; i != 16; i++) {
        p = &work->particles[i];
        if (p->variant != 0) {
          scale[0] = p->z + (p->y >> 7);
          if (scale[0] <= 0x7ff) {
            scale[0] = 0x800;
          }
          scale[1] = scale[0];
          drift[0] = p->x;
          drift[1] = 0xff0000;
          drift[2] = p->y + px(255);
          Render_ApplyProjectedPlacementFar(work->sprites[i], drift, scale, 0);
          p->y -= p->velocityY;
          if (p->y <= 0xfffff) {
            if (frame <= 279) {
              p->x = px((Random16() & 63) + shift + 32);
            } else {
              p->x = px((Random16() & 63) + shift - 32);
            }
            if (frame > 301) {
              p->variant = 0;
            } else if (frame > 245) {
              p->y = px(96);
            } else {
              p->y = px(192) - p->x;
            }
          }
        }
      }
      if (frame <= 279) {
        for (i = 0; i != 8; i++) {
          s32 x =
              (i & 1) * 20 - (Random16() & 15) + (i / 2) * 4 + shift / 2 - 16;
          s32 y = (i / 2) * 32 - (Random16() & 15);

          draw[1](canvas, work->sheet + ((frame / 2 + i) % 4) * 1728 + 0x4000,
                  x, y, 32, 54);
        }
      } else {
        for (i = 0; i != 8; i++) {
          s32 x =
              (i & 1) * 20 + (Random16() & 15) - (i / 2) * 4 + shift / 2 - 16;
          s32 y = (i / 2) * 32 - (Random16() & 15);

          draw[1](canvas, work->sheet + ((frame / 2 + i) % 4) * 1728 + 0x4000,
                  x, y, 32, 54);
        }
      }
    }
    HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
    if (frame > 63) {
      /* FAKEMATCH(temporary): the scroll value through a local. */
      s32 y = Random16() % 4 + 30;

      gBgScroll[1].y = y;
    }
    EFFECT_END_FRAME(work);
  }

  for (i = 0; i != 16; i++) {
    Sprite_DestroyFar(work->sprites[i]);
  }
  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

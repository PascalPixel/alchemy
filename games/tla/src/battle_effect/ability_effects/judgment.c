/// @file
/// The Judgment summon.
// Part of ability_effects.c.

#include "../ability_effects_internal.h"

// Judgment: the sky gathers sixteen tiles of light that fold into a sigil,
// then the sigil's beam strikes and the ground erupts in a ring of
// shockwaves and debris.
void BattleEffect_RunJudgment(struct BattleEffectArgument *effect) {
  s32 anchor[3];
  struct EffectPosition pos;
  s32 place[4];
  s32 drift[4];
  struct JudgmentScale req;
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

  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  transition = Ram_HeapSlots->transitionWork;
  sprites = Ram_HeapSlots->unk64[0];
  cells = Ram_MapCellBuffer;
  BattleEffect_StartCanvasLayer(0x2000);
  REG_BG2PA = 0x100;
  BattleEffect_ConfigureEffectDisplay();
  ((volatile u16 *)BG_PLTT)[0] = 0;
  ((volatile u16 *)BG_PLTT)[1] = 0;
  BattleEffect_LoadBlitter(SLOT_BLITTER, 3);
  work->transferMode = 0;
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  BattleEffect_WipeCanvas(0, 0);
  BattleEffect_SelectLivingTargets(work->effect);
  BattleBackground_LoadFar(1, (s32)&ResourceId_VioletSkyBackdrop, 0);

  pal = Resource_GetTableEntry((s32)&ResourceId_WingedKnightATiles);
  Iwram_CopyWords((void *)OBJ_PLTT, pal, 512);
  pal += 512;
  Decompress_TaggedLz(pal, cells);
  for (i = 0; i != 16; i++) {
    struct JudgmentObject *obj =
        Func_0815b290__actor_move_random(32, 32, 0x80002000, 0);

    obj->priority = 2;
    work->objects[i] = obj;
    Iwram_CopyWordsEntry((void *)OBJ_TILES(obj), cells + i * 0x400, 0x400);
  }
  gCameraSceneParameters[4] = 240;
  wait(1);
  transition[4] = 1;
  gBgScroll[1].x = 0;
  BattleEffect_WipeCanvas(0, 1);
  REG_DISPCNT = 0x7741;
  REG_BG2PA = 0x80;
  REG_BLDALPHA = 0x1010;
  REG_BLDCNT = 0x3f44;
  depth = -0x400000;
  zoom = 0x80000;
  Resource_LoadAndDecompress((s32)&ResourceId_PinkBurstSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, work->sheet + 0x600,
                             1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C2, gData_02014000, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
  pal = Resource_GetTableEntry((s32)&ResourceId_BlueCrescentSheet);
  Iwram_CopyWordsEntry((void *)BG_PLTT, pal, 128);
  work->transferMode = 2;
  work->transferValue = 50;
  REG_BG2CNT = 0x784;

  // The gathering: motes rise, the tiles fold in and flash.
  frame = 0;
  do {
    if (frame == 0) {
      struct EffectStep *p = work->particles;

      for (i = 0; i != 64; i++, p++) {
        p->x = (Random16() & 15) + 72;
        p->velocityX = Random16() & 0xffff;
        p->velocityY = Random16() & 0xffff;
        p->velocityY = Random16() & 0xffff;
      }
      pal = Resource_GetTableEntry((s32)&ResourceId_WingedKnightATiles);
      Iwram_CopyWordsEntry((void *)OBJ_PLTT, pal, 512);
      pal += 512;
      Decompress_TaggedLz(pal, Ram_MapCellBuffer);
      for (i = 0; i != 16; i++) {
        struct JudgmentObject *obj = work->objects[i];

        Iwram_CopyWordsEntry((void *)OBJ_TILES(obj),
                             Ram_MapCellBuffer + i * 0x400, 0x400);
      }
      depth = -0x400000;
      zoom = 0x80000;
    }
    if (frame == 143) {
      FILL__actor_move_random(canvas, 0x4000, 0x2a2a2a2a);
      Sound_PlayFar(145);
    }
    if (frame == 80) {
      Sound_PlayFar(142);
    }
    if (frame == 72) {
      work->fadeFrames = 24;
      work->fadeStep = 0;
    }

    anchor[2] = 0;
    anchor[1] = 0;
    {
      struct EffectStep *p = work->particles;

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
          draw[0](canvas, sprites + gBattleEffect_MoteCells[size - 1],
                  pos.x - size / 2, pos.y - size, size, size * 2);
        }
      }
    }

    req = Data_08196e74;
    place[3] = 0;
    place[1] = 0;
    for (i = 0; i != 16; i++) {
      if (frame > 75 && (i == 8 || i == 12 || i == 15)) {
        continue;
      }
      place[0] = ((i % 4) << 21) + (152 << 15);
      place[2] = ((i / 4) << 21) + depth;
      Render_ApplyProjectedPlacementFar(work->objects[i], place, (s32 *)&req,
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
        QUEUE_PUSH_PARTED(OBJ_TILES((struct JudgmentObject *)work->objects[i]),
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
            OBJ_TILES((struct JudgmentObject *)work->objects[Data_08198ab4[i]]),
            (u32)(PassBuffer(Ram_MapCellBuffer) + i * 0x400), 0x84000100);
      }
    }

    if ((u32)(frame - 116) <= 27) {
      s32 n = frame * 3 - 346;
      if (n > 80) {
        n = 80;
      } else {
        Func_0815b434(work->sheet + 0x1256, n, 0x10000);
      }
      Func_0818caa8(work->sheet + 0x1256, 60, 80, n);
      /* FAKEMATCH(dead): the test of the block it stands in, made again; as a
       * plain block, 8 of 2680 instructions differ in tla-en. */
      if ((u32)(frame - 116) <= 27) {
        s32 size = (frame - 116) / 3 + 16;

        if (size > 32) {
          size = 32;
        }
        for (i = 0; i != 6; i++) {
          s32 k = i & 3;
          s32 a = Random16() & 0xffff;
          s32 x = ((size * Trig_Sin(a)) >> 16) - (Data_08198ace[k] >> 1);
          s32 y = ((size * Trig_Cos(a)) >> 16) - (Data_08198ad4[k] / 2);

          draw[0](canvas, work->sheet + Data_08198ac2[k], x + 60, y + 80,
                  Data_08198ace[k], Data_08198ad4[k]);
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
        /* FAKEMATCH(temporary): the step taken through a local assigned twice;
         * as addB = addR = addG = 304 - frame * 2, 40 of 2680 instructions
         * differ in tla-en. */
        addB = addR = addG = step = (step = 304 - frame * 2);
        flag = 1;
      }
      if (flag == 1) {
        u16 *src = Resource_GetTableEntry((s32)&ResourceId_WingedKnightCTiles);
        volatile u16 *dst = (volatile u16 *)OBJ_PLTT;

        for (i = 0; i != 256; i++, src++, dst++) {
          s32 r = (*src & 31) + addR;
          s32 g = ((*src >> 5) & 31) + addG;
          s32 b = ((*src >> 10) & 31) + addB;

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
          *dst = b << 10 | g << 5 | r;
        }
      }
    }

    if (frame > 143) {
      s32 y = frame * 16 - 2272;

      DRAW_ALT(3, work->sheet, 36, y, 24, 64);
      /* FAKEMATCH(dead): a test that is never true, cells being the cell
       * buffer's address, kept between the two draws so that each builds
       * the sheet's address anew; as no test, 104 of 2680 instructions differ
       * in tla-en. */
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
      model->mesh = gData_08199364;
      model->vertices = verts;
      if (frame > 151) {
        shade = (frame * -8) + 1216;
      }
      model->shade = shade;
      Transform_Reset();
      Func_08015160(-0x40000, 0x180000, 0);
      Func_0801521c(s);
      Transform_ApplyPitch(3000);
      Render_TransformVertices(gData_08199210, verts, 4);
      Render_DrawModelList(model);
      Heap_Release(model);
      Heap_Release(verts);
    }

    work->transferPending = 1;
    wait(1);
    frame++;
  } while (frame != 160 && (frame <= 4 || !(gInput.repeat & 3)));

  Iwram_FillWords(canvas, 0x4000, 0);
  for (i = 0; i != 16; i++) {
    ResourceObject_ReleaseFar(work->objects[i]);
  }
  wait(1);
  Palette_LoadBasicSpriteColors();
  Dma_Set((const void *)OBJ_PLTT, (void *)&BG_PLTT_COLOR(5, 0), 0x80000010,
          REG_DMA3);
  BG_PLTT_COLOR(5, 14) = BG_PLTT_COLOR(15, 4);
  for (i = 0; i != 16; i++) {
    struct JudgmentObject *obj = ResourceObject_CreateFar(0x28d);

    work->objects[i] = obj;
    if (obj) {
      obj->unk1a = 0;
      Animation_ApplyChildArgumentFar(obj, i % 3);
      ((struct JudgmentObject *)work->objects[i])->priority = 0;
    }
  }
  HeapSlot_Release(SLOT_BLITTER);
  BattleEffect_LoadBlitter(SLOT_BLITTER, 19);
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
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
  REG_BG2PA = 0x100;
  REG_BG2X = -0x3c00;
  REG_BG2CNT = 0x784;
  FILL__actor_move_random(canvas, 0x4000, 0);
  cx = 60;
  cy = 44;
  squash = 6;
  ring = 0;
  shift = 0;
  sx = 8;
  rings = 2;
  work->transferMode = 2;
  work->transferValue = 75;
  Iwram_FillWordsEntry(work->sheet, 0x3840, 0);

  // The strike: the beam, the shockwave rings and the debris.
  for (frame = 0; frame != 336; frame++) {
    if (frame == 66) {
      Sound_PlayFar(208);
    }
    if (frame == 88) {
      Sound_PlayFar(230);
    }
    if (frame == 155) {
      Sound_PlayFar(162);
    }
    if (frame == 217) {
      Sound_PlayFar(156);
    }
    if (frame == 280) {
      Sound_PlayFar(157);
    }
    if (frame == 300) {
      for (i = 0; i != effect->count; i++) {
        BattleEffect_TintBattler(effect->actors[i], 7, 5, i, 8);
        BattleMotion_ApplyKnockbackFar(effect->actors[i], 1);
      }
      BattlePlayback_StartFar(145);
    }
    if (gInput.repeat & 3) {
      if ((u32)(frame - 5) <= 144) {
        frame = 150;
        IO_WRITE_QUEUE_PUSH(0x04000020, 128, 0x20000);
        IO_WRITE_QUEUE_PUSH(0x04000028, 0, 0x30000);
        Resource_LoadAndDecompress((s32)&ResourceId_ExplosionSheet,
                                   work->sheet + 0x4000, 1, 0);
      } else if ((u32)(frame - 155) <= 58) {
        frame = 214;
      } else if ((u32)(frame - 219) <= 60) {
        frame = 280;
      }
    }
    if (frame == 318) {
      FILL__actor_move_random(canvas, 0x4000, 0x3f3f3f3f);
    }
    if (frame == 319) {
      Graphics_LoadGreyRamp();
      work->transferMode = 3;
      work->transferValue = 0x04040404;
    }
    if (frame == 64) {
      IO_WRITE_QUEUE_PUSH(0x04000020, 128, 0x20000);
      IO_WRITE_QUEUE_PUSH(0x04000028, 0, 0x30000);
      FILL__actor_move_random(canvas, 0x4000, 0);
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
      FILL__actor_move_random(canvas, 0x4000, 0x3f3f3f3f);
    }
    if (frame == 70) {
      BattleStaging_ConfigurePaletteFadeFar(
          1, (s32)&ResourceId_AerialViewBackdrop, 7);
    }
    if (frame == 150) {
      cx = 112;
      cy = 32;
      ring = 0;
      rings = 4;
      squash = 8;
      FILL__actor_move_random(work->sheet, 0x3840, 0);
      FILL__actor_move_random(canvas, 0x4000, 0x3f3f3f3f);
      BattleBackground_LoadFar(1, (s32)&ResourceId_BeachBackdrop, 0);
    }
    if (frame == 214) {
      FILL__actor_move_random(work->sheet, 0x3840, 0);
      FILL__actor_move_random(canvas, 0x4000, 0x3f3f3f3f);
      BattleBackground_LoadFar(1, (s32)&ResourceId_ForestBackdrop, 0);
      {
        struct EffectStep *p = work->particles;

        for (i = 0; i != 16; i++, p++) {
          p->x = ((Random16() & 63) + 32) << 16;
          p->y = ((Random16() & 31) + 96) << 16;
          p->velocityY = ((Random16() & 15) + 16) << 15;
          p->z = 0x8000 - (p->x >> 7);
          p->variant = 1;
        }
      }
    }
    if (frame == 280) {
      BattleEffect_SetupBlendedDisplay();
      FILL__actor_move_random(work->sheet, 0x3840, 0);
      FILL__actor_move_random(canvas, 0x4000, 0x01010101);
      transition[4] = 0;
      REG_BLDALPHA = 0x1010;
      {
        struct EffectStep *p = work->particles;

        for (i = 0; i != 16; i++, p++) {
          p->x = ((Random16() & 63) + 256) << 16;
          p->y = ((Random16() & 31) + 96) << 16;
          p->velocityY = ((Random16() & 15) + 16) << 15;
          p->z = 0;
        }
      }
      shift = 272;
      work->transferMode = 0;
      work->transferValue = 0;
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
        *c = b << 10 | g << 5 | r;
      }
    }
    if (frame == 182) {
      FILL__actor_move_random(canvas, 0x4000, 0x3f3f3f3f);
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
      slot = (DrawRectangle *)&Ram_HeapSlots->unkb8[1];
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
       * this picture; as 24 at both, 116 of 2680 instructions differ in
       * tla-en. */
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
      model->mesh = gData_08199364;
      model->vertices = verts;
      if (frame > 79) {
        shade = (frame * -8) + 640;
      }
      model->shade = shade;
      Transform_Reset();
      Func_08015160(-0x40000, 0x80000, 0);
      Func_0801521c(s);
      Transform_ApplyPitch(5000);
      Render_TransformVertices(gData_08199210, verts, 4);
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
            /* FAKEMATCH(dead): a store top's own line overwrites; its test
             * builds 1000 in a reload register before cx's reload, the one
             * pick that sends cx to r4; without it, 28 of 2680 instructions
             * differ in tla-en. */
            if (dx > 1000)
              top = 0;
            right = cx + dx;
            /* FAKEMATCH(dead): a branch whose store the next line overwrites
             * restores the label CSE and reload saw between the edges; without
             * it, 231 of 2680 instructions differ in tla-en. */
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
             * second half; without it, 28 of 2680 instructions differ in
             * tla-en. */
            if (dx > 1000)
              top = 0;
            right = cx + dx;
            /* FAKEMATCH(dead): a branch whose store the next line overwrites
             * restores the label CSE and reload saw between the edges; without
             * it, 205 of 2680 instructions differ in tla-en. */
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
      s32 angle = -0x4000;

      for (i = 0; i != 9; i++, angle += 0x1000) {
        s32 x = ((frame - 86) * Trig_Sin(angle)) >> 16;
        s32 y = ((frame - 70) * Trig_Cos(angle)) >> 16;

        draw[0](canvas, work->sheet + ((frame / 2 + i) % 4) * 1728 + 0x4000,
                x + 44, y + 17, 32, 54);
      }
    }

    BattleEffect_LoadBlitter(SLOT_ALT, 3);
    draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
    if (frame > 85) {
      draw[1](canvas, work->sheet, 0, 0, 120, 120);
    }
    if (frame > 279) {
      shift -= 8;
    }
    if (frame == 238) {
      FILL__actor_move_random(canvas, 0x4000, 0x3f3f3f3f);
    }
    if ((u32)(frame - 214) <= 65) {
      gBgScroll[1].x += 8;
    }
    if ((u32)(frame - 246) <= 33) {
      shift += 8;
      work->transferMode = 0;
      work->transferValue = 75;
    }

    if (frame > 213) {
      struct EffectStep *p;

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
          drift[2] = p->y + 0xff0000;
          Render_ApplyProjectedPlacementFar(work->objects[i], drift, scale, 0);
          p->y -= p->velocityY;
          if (p->y <= 0xfffff) {
            if (frame <= 279) {
              p->x = ((Random16() & 63) + shift + 32) << 16;
            } else {
              p->x = ((Random16() & 63) + shift - 32) << 16;
            }
            if (frame > 301) {
              p->variant = 0;
            } else if (frame > 245) {
              p->y = 0x600000;
            } else {
              p->y = 0xc00000 - p->x;
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
    HeapSlot_Release(SLOT_ALT);
    if (frame > 63) {
      /* FAKEMATCH(temporary): the scroll value through a local; as stored
       * straight to gBgScroll[1].y, 2 of 2680 instructions differ in tla-en. */
      s32 y = Random16() % 4 + 30;

      gBgScroll[1].y = y;
    }
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  for (i = 0; i != 16; i++) {
    ResourceObject_ReleaseFar(work->objects[i]);
  }
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(SLOT_BLITTER);
  BattleEffect_EndCanvasLayer();
}

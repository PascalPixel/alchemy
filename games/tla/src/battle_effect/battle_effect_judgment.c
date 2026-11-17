/// @file
/// The Judgment summon.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

/// Where TornadoSheet is decoded in the effect's pictures.
#define JUDGMENT_TORNADO_PICTURES 1536
// OverlayImageA's three flash pictures, a byte a pixel, one after another in
// the sheet: the small flash, the large flash and the glow.
#define JUDGMENT_SHEET_FLASH_SMALL_WIDTH 16
#define JUDGMENT_SHEET_FLASH_SMALL_HEIGHT 17
#define JUDGMENT_SHEET_FLASH_LARGE_WIDTH 24
#define JUDGMENT_SHEET_FLASH_LARGE_HEIGHT 41
#define JUDGMENT_SHEET_FLASH_GLOW_WIDTH 22
#define JUDGMENT_SHEET_FLASH_GLOW_HEIGHT 44
#define JUDGMENT_SHEET_FLASH_SMALL 24576
#define JUDGMENT_SHEET_FLASH_LARGE                                             \
  (JUDGMENT_SHEET_FLASH_SMALL + PICTURE_BYTES(JUDGMENT_SHEET_FLASH_SMALL))
#define JUDGMENT_SHEET_FLASH_GLOW                                              \
  (JUDGMENT_SHEET_FLASH_LARGE + PICTURE_BYTES(JUDGMENT_SHEET_FLASH_LARGE))

#define JUDGMENT_FRAMES 336

/// The gathering's crash: the canvas fills, then the beams fall from the next
/// frame.
#define JUDGMENT_BEAM_FRAME 144
/// The beam, at the sheet's start.
#define JUDGMENT_BEAM_WIDTH 24
#define JUDGMENT_BEAM_HEIGHT 64
/// The explosion's four pictures, loaded over the flares on the sweep's
/// frame 64.
#define JUDGMENT_SHEET_EXPLOSION 16384
/// The two flare pictures, from the crescent sheet the sweep loads.
#define JUDGMENT_FLARE_WIDTH 24
#define JUDGMENT_FLARE_HEIGHT 24
#define JUDGMENT_SHEET_FLARE_A 16384
#define JUDGMENT_SHEET_FLARE_B                                                 \
  (JUDGMENT_SHEET_FLARE_A + PICTURE_BYTES(JUDGMENT_FLARE))
#define JUDGMENT_EXPLOSION_WIDTH 32
#define JUDGMENT_EXPLOSION_HEIGHT 54
/// The sweep's scenes, by the frame each opens on: the flare lands on the
/// aerial view and it explodes, the wave crosses the beach, the debris rises
/// in the forest, and the battle returns. A or B skips to the next scene from
/// five frames into one.
#define JUDGMENT_SWEEP_EXPLOSION 64
#define JUDGMENT_SWEEP_BEACH 150
#define JUDGMENT_SWEEP_FOREST 214
#define JUDGMENT_SWEEP_RETURN 280

#define JUDGMENT_FORMATION_OBJECTS 16 ///< sprites in the formation, 4x4
#define JUDGMENT_DEBRIS_OBJECTS 16    ///< debris sprites of the sweep
#define JUDGMENT_EXPLOSION_PICTURE_BYTES PICTURE_BYTES(JUDGMENT_EXPLOSION)
#define JUDGMENT_WAVE_SIZE 120 ///< the wave picture is 120x120

/// Where FireRingTexture is decoded in the map cell buffer.
#define JUDGMENT_FIRE_RING 16384

/// The rings are drawn at JUDGMENT_RING_LEVEL into a square picture at the
/// sheet's start, cleared each frame.
#define JUDGMENT_RINGS_WIDTH 120
#define JUDGMENT_RINGS_HEIGHT 120
#define JUDGMENT_RING_LEVEL 20

/// The battle wipes away to a violet sky where Judgment flies forward while
/// motes gather into it; it bursts in a flash, and the blast falls on the land
/// below and spreads in rings of light.
void BattleEffect_RunJudgment(struct BattleEffectParams *args) {
  s32 anchor[3];
  struct ScreenPoint pos;
  FIXED place[4];
  FIXED drift[4];
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
  FIXED depth;
  s32 zoom;
  s32 addR, addG, addB;
  s32 step;
  s32 cx, cy, squash, sx, ring, rings, shift;
  s32 i;
  u8 *cells;
  u8 *pal;

  canvas = Ram_HeapSlots->battleCanvas;
  work = Ram_HeapSlots->battleEffect;
  transition = Ram_HeapSlots->battleBackground;
  sprites = Ram_HeapSlots->battleSheet;
  cells = Ram_MapCellBuffer;
  EFFECT_START_NARROW_CANVAS(BGCNT_WRAP);
  BattleEffect_ConfigureEffectDisplay();
  PLTT_BACKDROP = 0;
  BG_PLTT_COLOR(0, 1) = 0;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
  work->transferMode = CANVAS_COPY;
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK);
  BattleEffect_WipeCanvas(CANVAS_WIPE_DOWN, CANVAS_WIPE_FILL);
  BattleEffect_SelectLivingTargets(work->args);
  BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED,
                           RESOURCE_VIOLET_SKY_BACKDROP, 0);

  pal = Resource_GetTableEntry(RESOURCE_WINGED_KNIGHT_A_TILES);
  Iwram_CopyWords((void *)OBJ_PLTT, pal, OBJ_PLTT_SIZE);
  pal += OBJ_PLTT_SIZE;
  Decompress_TaggedLz(pal, cells);
  for (i = 0; i != JUDGMENT_FORMATION_OBJECTS; i++) {
    struct Sprite *obj = EffectSprite_CreateWithTiles(
        32, 32, OBJ_SIZE_32X32 | OBJ_256_COLORS, 0);

    obj->image.priority = 2;
    work->sprites[i] = obj;
    Iwram_CopyWordsEntry((void *)OBJ_TILES(obj),
                         cells + i * OBJ_32X32_8BPP_BYTES,
                         OBJ_32X32_8BPP_BYTES);
  }
  gProjection.centerY = SUMMON_PROJECTION_CENTER_Y;
  wait(1);
  transition[4] = 1;
  gBgScroll[1].x = 0;
  BattleEffect_WipeCanvas(CANVAS_WIPE_DOWN, CANVAS_WIPE_CLEAR);
  EFFECT_SHOW_WIDE_CANVAS(BLEND_WEIGHT_MAX);
  depth = -toFIXED(64);
  zoom = FIXED_ONE * 8;
  Resource_LoadAndDecompress(RESOURCE_PINK_BURST_SHEET, work->sheet, 1, 1);
  Resource_LoadAndDecompress(RESOURCE_TORNADO_SHEET,
                             work->sheet + JUDGMENT_TORNADO_PICTURES, 1, 0);
  Resource_LoadAndDecompress(RESOURCE_FIRE_RING_TEXTURE,
                             Ram_MapCellBuffer + JUDGMENT_FIRE_RING, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, sprites, 0, 0);
  pal = Resource_GetTableEntry(RESOURCE_BLUE_CRESCENT_SHEET);
  Iwram_CopyWordsEntry((void *)BG_PLTT, pal, SHEET_PALETTE_BYTES);
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);

  // The gathering: motes rise, the tiles fold in and flash.
  frame = 0;
  do {
    if (frame == 0) {
      struct Particle *p = work->particles;

      for (i = 0; i != ARRAY_COUNT(work->particles); i++, p++) {
        p->x = (Random16() & 15) + 72;
        p->velocityX = Random16() & TRIG_ANGLE_MASK;
        p->velocityY = Random16() & TRIG_ANGLE_MASK;
        p->velocityY = Random16() & TRIG_ANGLE_MASK;
      }
      pal = Resource_GetTableEntry(RESOURCE_WINGED_KNIGHT_A_TILES);
      Iwram_CopyWordsEntry((void *)OBJ_PLTT, pal, OBJ_PLTT_SIZE);
      pal += OBJ_PLTT_SIZE;
      Decompress_TaggedLz(pal, Ram_MapCellBuffer);
      for (i = 0; i != JUDGMENT_FORMATION_OBJECTS; i++) {
        struct Sprite *obj = work->sprites[i];

        Iwram_CopyWordsEntry((void *)OBJ_TILES(obj),
                             Ram_MapCellBuffer + i * OBJ_32X32_8BPP_BYTES,
                             OBJ_32X32_8BPP_BYTES);
      }
      depth = -toFIXED(64);
      zoom = FIXED_ONE * 8;
    }
    if (frame == 143) {
      IWRAM_FILL_WORDS(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(42));
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

      for (i = 0; i != ARRAY_COUNT(work->particles); i++, p++) {
        s32 size;

        if (frame > i / 4 + 80 && p->x > 0) {
          Matrix_Initialize();
          Matrix_RotateZ(p->velocityY);
          Matrix_RotateX(p->velocityX);
          Matrix_RotateY(p->velocityY + (frame << 9));
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
    for (i = 0; i != JUDGMENT_FORMATION_OBJECTS; i++) {
      if (frame > 75 && (i == 8 || i == 12 || i == 15)) {
        continue;
      }
      place[0] = INTtoFIXED((i % 4) * 32) + toFIXED(76);
      place[2] = INTtoFIXED((i / 4) * 32) + depth;
      Render_ApplyProjectedPlacementFar(work->sprites[i], place, (s32 *)&req,
                                        0);
    }
    depth += zoom;
    if (frame <= 47) {
      zoom -= FIXED_ONE / 4;
    }
    if (frame > 32) {
      zoom = zoom * 60 / DAMPING_SCALE;
    }
    if (frame == JUDGMENT_BEAM_FRAME) {
      zoom = -FIXED_ONE * 8;
    }
    if (frame == 146) {
      zoom = FIXED_ONE;
    }

    if (frame == 72) {
      pal = Resource_GetTableEntry(RESOURCE_WINGED_KNIGHT_B_TILES);
      Iwram_CopyWordsEntry((void *)OBJ_PLTT, pal, OBJ_PLTT_SIZE);
      pal += OBJ_PLTT_SIZE;
      Decompress_TaggedLz(pal, Ram_MapCellBuffer);
      for (i = 0; i != JUDGMENT_FORMATION_OBJECTS; i++) {
        IO_WRITE_QUEUE_PUSH(OBJ_TILES((struct Sprite *)work->sprites[i]),
                            (u32)(cells + i * OBJ_32X32_8BPP_BYTES),
                            DMA_COPY_WORDS(256));
      }
    }
    if (frame == 76) {
      pal = Resource_GetTableEntry(RESOURCE_WINGED_KNIGHT_C_TILES);
      Iwram_CopyWordsEntry((void *)OBJ_PLTT, pal, OBJ_PLTT_SIZE);
      pal += OBJ_PLTT_SIZE;
      Decompress_TaggedLz(pal, Ram_MapCellBuffer);
      for (i = 0; i != 13; i++) {
        IO_WRITE_QUEUE_PUSH(
            OBJ_TILES(
                (struct Sprite *)work->sprites[gJudgment_BurstPictures[i]]),
            (u32)(cells + i * OBJ_32X32_8BPP_BYTES), DMA_COPY_WORDS(256));
      }
    }

    if ((u32)(frame - 116) <= 27) {
      s32 n = frame * 3 - 346;
      if (n > 80) {
        n = 80;
      } else {
        BattleEffect_DrawShadedCircles(work->sheet + 4694, n, FIXED_ONE);
      }
      Canvas_BlitAtFoot(work->sheet + 4694, 60, 80, n);
    }
    if ((u32)(frame - 116) <= 27) {
      s32 size = (frame - 116) / 3 + 16;

      if (size > 32) {
        size = 32;
      }
      for (i = 0; i != 6; i++) {
        s32 k = i & 3;
        s32 a = Random16() & TRIG_ANGLE_MASK;
        s32 x =
            (FIXEDtoINT(size * Trig_Sin(a))) - (gJudgment_ShardWidths[k] >> 1);
        s32 y =
            (FIXEDtoINT(size * Trig_Cos(a))) - (gJudgment_ShardHeights[k] / 2);

        draw[0](canvas, work->sheet + gJudgment_ShardPictures[k], x + 60,
                y + 80, gJudgment_ShardWidths[k], gJudgment_ShardHeights[k]);
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
        u16 *src = Resource_GetTableEntry(RESOURCE_WINGED_KNIGHT_C_TILES);
        volatile u16 *dst = (volatile u16 *)OBJ_PLTT;

        for (i = 0; i != 256; i++, src++, dst++) {
          s32 r = RGB_RED(*src) + addR;
          s32 g = RGB_GREEN(*src) + addG;
          s32 b = RGB_BLUE(*src) + addB;

          if (r > RGB_LEVEL_MAX)
            r = RGB_LEVEL_MAX;
          if (g > RGB_LEVEL_MAX)
            g = RGB_LEVEL_MAX;
          if (b > RGB_LEVEL_MAX)
            b = RGB_LEVEL_MAX;
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
      s32 flags;

      flags = EFFECT_CLIP_BOUNDS;
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, flags, BLITTER_ADDITIVE);
      ((DrawRectangle)Ram_HeapSlots->ramCodePaired)(canvas, work->sheet, 36, y,
                                                    JUDGMENT_BEAM_WIDTH,
                                                    JUDGMENT_BEAM_HEIGHT);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
      flags = EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X;
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, flags, BLITTER_ADDITIVE);
      ((DrawRectangle)Ram_HeapSlots->ramCodePaired)(canvas, work->sheet, 60, y,
                                                    JUDGMENT_BEAM_WIDTH,
                                                    JUDGMENT_BEAM_HEIGHT);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
    }

    if ((u32)(frame - JUDGMENT_BEAM_FRAME) <= 15) {
      s32 s;
      s32 shade;
      void *verts;
      struct RenderModel *model;

      verts = Heap_AllocEwram(32);
      model = Render_AllocateModelList(1);
      shade = 0;
      s = (frame << 13) - toFIXED(17.875);
      texture.logWidth = 7;
      texture.logHeight = 7;
      texture.pixels = Ram_MapCellBuffer + JUDGMENT_FIRE_RING;
      model->texture = &texture;
      model->kind = MODEL_SHADED_ADDITIVE;
      model->mesh = gBattleEffect_LargeSquareMesh;
      model->vertices = verts;
      if (frame > 151) {
        shade = (frame * -8) + 1216;
      }
      model->shade = shade;
      Matrix_Initialize();
      Matrix_Translate(-toFIXED(4), toFIXED(24), 0);
      Matrix_ScaleUniform(s);
      Matrix_RotateX(3000);
      Render_TransformVertices(gBattleEffect_GroundSquareCorners, verts, 4);
      Render_DrawModelList(model);
      Heap_Release(model);
      Heap_Release(verts);
    }

    EFFECT_PRESENT_CANVAS(work);
    frame++;
  } while (frame != 160 && (frame <= 4 || !(gInput.repeat & (KEY_A | KEY_B))));

  Iwram_FillWords(canvas, CANVAS_BYTES, 0);
  for (i = 0; i != JUDGMENT_FORMATION_OBJECTS; i++) {
    Sprite_DestroyFar(work->sprites[i]);
  }
  wait(1);
  Palette_LoadBasicSpriteColors();
  Dma_Set((const void *)OBJ_PLTT, (void *)&BG_PLTT_COLOR(5, 0),
          DMA_COPY_HALFWORDS(16), REG_DMA3);
  BG_PLTT_COLOR(5, 14) = BG_PLTT_COLOR(15, 4);
  for (i = 0; i != JUDGMENT_DEBRIS_OBJECTS; i++) {
    struct Sprite *obj = Sprite_CreateFar(SPRITE_BATTLE_ROCKS);

    work->sprites[i] = obj;
    if (obj) {
      obj->flags = 0;
      Sprite_StartAnimationFar(obj, i % 3);
      ((struct Sprite *)work->sprites[i])->image.priority = 0;
    }
  }
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  Resource_LoadAndDecompress(RESOURCE_BLUE_CRESCENT_SHEET,
                             work->sheet + JUDGMENT_SHEET_FLARE_A, 1, 1);
  {
    u8 *c = sprites;

    for (i = 0; i != MOTE_SET_BYTES; i++, c++) {
      if (*c > 32) {
        *c += 224;
      } else if (*c != 0) {
        *c = 1;
      }
    }
  }
  work->fadeFrames = 0;
  work->fadeStep = 0;
  BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED,
                           RESOURCE_AERIAL_VIEW_BACKDROP, 0);
  REG_BG2PA = Q_8_8(1);
  REG_BG2X = -(60 << 8);
  REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
  IWRAM_FILL_WORDS(canvas, CANVAS_BYTES, 0);
  cx = 60;
  cy = 44;
  squash = 6;
  ring = 0;
  shift = 0;
  sx = 8;
  rings = 2;
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  Iwram_FillWordsEntry(work->sheet, PICTURE_BYTES(JUDGMENT_RINGS), 0);

  // The strike: the beam, the shockwave rings and the debris.
  for (frame = 0; frame != JUDGMENT_FRAMES; frame++) {
    if (frame == 66) {
      Sound_PlayFar(SE_BANE_ESCAPE);
    }
    if (frame == 88) {
      Sound_PlayFar(SE_WATER_DRAIN);
    }
    if (frame == 155) {
      Sound_PlayFar(SE_WAVE);
    }
    if (frame == 217) {
      Sound_PlayFar(SE_SWITCH_MECHANISM);
    }
    if (frame == JUDGMENT_SWEEP_RETURN) {
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
        frame = JUDGMENT_SWEEP_BEACH;
        IO_WRITE_QUEUE_PUSH((u32)&REG_BG2PA, Q_8_8(0.5), IO_WRITE_HALFWORD);
        IO_WRITE_QUEUE_PUSH((u32)&REG_BG2X, 0, IO_WRITE_WORD);
        Resource_LoadAndDecompress(RESOURCE_EXPLOSION_SHEET,
                                   work->sheet + JUDGMENT_SHEET_EXPLOSION, 1,
                                   0);
      } else if ((u32)(frame - 155) <= 58) {
        frame = JUDGMENT_SWEEP_FOREST;
      } else if ((u32)(frame - 219) <= 60) {
        frame = JUDGMENT_SWEEP_RETURN;
      }
    }
    if (frame == 318) {
      IWRAM_FILL_WORDS(canvas, CANVAS_BYTES,
                       CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
    }
    if (frame == 319) {
      Graphics_LoadGreyRamp();
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(4));
    }
    if (frame == JUDGMENT_SWEEP_EXPLOSION) {
      IO_WRITE_QUEUE_PUSH((u32)&REG_BG2PA, Q_8_8(0.5), IO_WRITE_HALFWORD);
      IO_WRITE_QUEUE_PUSH((u32)&REG_BG2X, 0, IO_WRITE_WORD);
      IWRAM_FILL_WORDS(canvas, CANVAS_BYTES, 0);
      Resource_LoadAndDecompress(RESOURCE_EXPLOSION_SHEET,
                                 work->sheet + JUDGMENT_SHEET_EXPLOSION, 1, 0);
      Resource_LoadAndDecompress(RESOURCE_OVERLAY_IMAGE_A,
                                 work->sheet + JUDGMENT_SHEET_FLASH_SMALL, 0,
                                 0);
    }
    if (frame == 66) {
      volatile u16 *c = &BG_PLTT_COLOR(6, 0);

      for (i = 0; i != BATTLE_BACKGROUND_COLORS; i++, c++) {
        *c ^= RGB_WHITE;
      }
    }
    if (frame == 69) {
      IWRAM_FILL_WORDS(canvas, CANVAS_BYTES,
                       CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
    }
    if (frame == 70) {
      BattleBackground_SetModeFar(BATTLE_BACKGROUND_FIXED,
                                  RESOURCE_AERIAL_VIEW_BACKDROP, 7);
    }
    if (frame == JUDGMENT_SWEEP_BEACH) {
      cx = 112;
      cy = 32;
      ring = 0;
      rings = 4;
      squash = 8;
      IWRAM_FILL_WORDS(work->sheet, PICTURE_BYTES(JUDGMENT_RINGS), 0);
      IWRAM_FILL_WORDS(canvas, CANVAS_BYTES,
                       CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
      BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED, RESOURCE_BEACH_BACKDROP,
                               0);
    }
    if (frame == JUDGMENT_SWEEP_FOREST) {
      IWRAM_FILL_WORDS(work->sheet, PICTURE_BYTES(JUDGMENT_RINGS), 0);
      IWRAM_FILL_WORDS(canvas, CANVAS_BYTES,
                       CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
      BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED,
                               RESOURCE_FOREST_BACKDROP, 0);
      {
        struct Particle *p = work->particles;

        for (i = 0; i != JUDGMENT_DEBRIS_OBJECTS; i++, p++) {
          p->x = INTtoFIXED((Random16() & 63) + 32);
          p->y = INTtoFIXED((Random16() & 31) + 96);
          p->velocityY = ((Random16() & 15) + 16) << 15;
          p->z = toFIXED(1) / 2 - (p->x >> 7);
          p->variant = 1;
        }
      }
    }
    if (frame == JUDGMENT_SWEEP_RETURN) {
      BattleEffect_SetupBlendedDisplay();
      IWRAM_FILL_WORDS(work->sheet, PICTURE_BYTES(JUDGMENT_RINGS), 0);
      IWRAM_FILL_WORDS(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(1));
      transition[4] = 0;
      REG_BLDALPHA = EFFECT_BLEND_ADD;
      {
        struct Particle *p = work->particles;

        for (i = 0; i != JUDGMENT_DEBRIS_OBJECTS; i++, p++) {
          p->x = INTtoFIXED((Random16() & 63) + 256);
          p->y = INTtoFIXED((Random16() & 31) + 96);
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
        s32 b = RGB_BLUE((u16)v);
        s32 g = RGB_GREEN((u16)v);
        s32 r = RGB_RED(v);

        b++;
        g++;
        r++;

        if (b > RGB_LEVEL_MAX)
          b = RGB_LEVEL_MAX;
        if (g > RGB_LEVEL_MAX)
          g = RGB_LEVEL_MAX;
        if (r > RGB_LEVEL_MAX)
          r = RGB_LEVEL_MAX;
        *c = RGB(r, g, b);
      }
    }
    if (frame == 182) {
      IWRAM_FILL_WORDS(canvas, CANVAS_BYTES,
                       CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
    }

    if (frame <= 63) {
      u8 *sheet;
      s32 top = frame - 4;
      s32 flags;

      if ((frame & 7) < 4) {
        sheet = work->sheet + JUDGMENT_SHEET_FLARE_A;
      } else {
        sheet = work->sheet + JUDGMENT_SHEET_FLARE_B;
      }
      flags = EFFECT_CLIP_BOUNDS;
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, flags, BLITTER_MAX);
      ((DrawRectangle)Ram_HeapSlots->ramCodePaired)(canvas, sheet, 36, top - 24,
                                                    JUDGMENT_FLARE_WIDTH,
                                                    JUDGMENT_FLARE_HEIGHT);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
      flags = EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X;
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, flags, BLITTER_MAX);
      ((DrawRectangle)Ram_HeapSlots->ramCodePaired)(canvas, sheet, 59, top - 24,
                                                    JUDGMENT_FLARE_WIDTH,
                                                    JUDGMENT_FLARE_HEIGHT);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
      flags = EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y;
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, flags, BLITTER_MAX);
      ((DrawRectangle)Ram_HeapSlots->ramCodePaired)(canvas, sheet, 36, top - 1,
                                                    JUDGMENT_FLARE_WIDTH,
                                                    JUDGMENT_FLARE_HEIGHT);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
      flags = EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP;
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, flags, BLITTER_MAX);
      ((DrawRectangle)Ram_HeapSlots->ramCodePaired)(canvas, sheet, 59, top - 1,
                                                    JUDGMENT_FLARE_WIDTH,
                                                    JUDGMENT_FLARE_HEIGHT);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
    }
    if ((u32)(frame - 64) <= 1) {
      EFFECT_DRAW_PICTURE(draw[0], canvas, work->sheet,
                          JUDGMENT_SHEET_FLASH_SMALL, 52, 51);
    }
    if ((u32)(frame - 66) <= 1) {
      s32 width = JUDGMENT_SHEET_FLASH_LARGE_WIDTH;

      draw[0](canvas, work->sheet + JUDGMENT_SHEET_FLASH_LARGE, 48, 40, width,
              JUDGMENT_SHEET_FLASH_LARGE_HEIGHT);
    }
    if ((u32)(frame - 68) <= 7) {
      s32 d = 76 - frame;

      EFFECT_DRAW_PICTURE(draw[0], canvas, work->sheet,
                          JUDGMENT_SHEET_FLASH_GLOW, 49 - d / 2, 38 - d);
      EFFECT_DRAW_PICTURE(draw[0], canvas, work->sheet,
                          JUDGMENT_SHEET_FLASH_GLOW, 49 - d / 2, d + 38);
      EFFECT_DRAW_PICTURE(draw[0], canvas, work->sheet,
                          JUDGMENT_SHEET_FLASH_GLOW, d / 2 + 49, 38 - d);
      EFFECT_DRAW_PICTURE(draw[0], canvas, work->sheet,
                          JUDGMENT_SHEET_FLASH_GLOW, d / 2 + 49, d + 38);
    }
    if ((u32)(frame - 78) <= 1) {
      EFFECT_DRAW_PICTURE(draw[0], canvas, work->sheet,
                          JUDGMENT_SHEET_FLASH_SMALL, 52, 51);
    }
    if ((u32)(frame - 80) <= 1) {
      s32 width = JUDGMENT_SHEET_FLASH_LARGE_WIDTH;

      draw[0](canvas, work->sheet + JUDGMENT_SHEET_FLASH_LARGE, 48, 40, width,
              JUDGMENT_SHEET_FLASH_LARGE_HEIGHT);
    }
    if ((u32)(frame - 82) <= 3) {
      s32 d = frame * 2 - 164;

      EFFECT_DRAW_PICTURE(draw[0], canvas, work->sheet,
                          JUDGMENT_SHEET_FLASH_GLOW, 49 - d / 4, 38 - d);
      EFFECT_DRAW_PICTURE(draw[0], canvas, work->sheet,
                          JUDGMENT_SHEET_FLASH_GLOW, 49 - d / 4, d + 38);
      EFFECT_DRAW_PICTURE(draw[0], canvas, work->sheet,
                          JUDGMENT_SHEET_FLASH_GLOW, d / 4 + 49, 38 - d);
      EFFECT_DRAW_PICTURE(draw[0], canvas, work->sheet,
                          JUDGMENT_SHEET_FLASH_GLOW, d / 4 + 49, d + 38);
    }

    if ((u32)(frame - 72) <= 15) {
      s32 s;
      s32 shade;
      void *verts;
      struct RenderModel *model;

      verts = Heap_AllocEwram(32);
      model = Render_AllocateModelList(1);
      s = (frame << 13) - toFIXED(287) / 32;
      glare.logWidth = 7;
      glare.logHeight = 7;
      glare.pixels = Ram_MapCellBuffer + JUDGMENT_FIRE_RING;
      shade = 0;
      model->kind = MODEL_SHADED_ADDITIVE;
      model->texture = &glare;
      model->mesh = gBattleEffect_LargeSquareMesh;
      model->vertices = verts;
      if (frame > 79) {
        shade = (frame * -8) + 640;
      }
      model->shade = shade;
      Matrix_Initialize();
      Matrix_Translate(-toFIXED(4), toFIXED(8), 0);
      Matrix_ScaleUniform(s);
      Matrix_RotateX(5000);
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
            /* FAKEMATCH(dead): a branch whose store the next line overwrites.
             */
            if (dx > cx)
              left = 0;
            left = cx - dx;
            bottom = cy + (slope * squash) / 8;
            top = cy - (slope * sx) / 8;
            if (top < 0)
              top = 0;
            if (bottom > JUDGMENT_RINGS_HEIGHT - 1)
              bottom = JUDGMENT_RINGS_HEIGHT - 1;
            if (left < 0)
              left = 0;
            if (right > JUDGMENT_RINGS_WIDTH - 1)
              right = JUDGMENT_RINGS_WIDTH - 1;
            work->sheet[bottom * JUDGMENT_RINGS_WIDTH + right] =
                JUDGMENT_RING_LEVEL;
            work->sheet[top * JUDGMENT_RINGS_WIDTH + right] =
                JUDGMENT_RING_LEVEL;
            work->sheet[bottom * JUDGMENT_RINGS_WIDTH + left] =
                JUDGMENT_RING_LEVEL;
            work->sheet[top * JUDGMENT_RINGS_WIDTH + left] =
                JUDGMENT_RING_LEVEL;
            right = cx + half + 1;
            left = cx - half + 1;
            if (left < 0)
              left = 0;
            if (right > JUDGMENT_RINGS_WIDTH - 1)
              right = JUDGMENT_RINGS_WIDTH - 1;
            work->sheet[bottom * JUDGMENT_RINGS_WIDTH + right] =
                JUDGMENT_RING_LEVEL;
            work->sheet[top * JUDGMENT_RINGS_WIDTH + right] =
                JUDGMENT_RING_LEVEL;
            work->sheet[bottom * JUDGMENT_RINGS_WIDTH + left] =
                JUDGMENT_RING_LEVEL;
            work->sheet[top * JUDGMENT_RINGS_WIDTH + left] =
                JUDGMENT_RING_LEVEL;

            halfSlope = slope / 2;
            dx = (halfSlope * sx) / 8;
            /* FAKEMATCH(dead): as above, the pick before cx's reload in the
             * second half. */
            if (dx > 1000)
              top = 0;
            right = cx + dx;
            /* FAKEMATCH(dead): a branch whose store the next line overwrites.
             */
            if (dx > cx)
              left = 0;
            left = cx - dx;
            bottom = cy + (radius * squash) / 8;
            top = cy - (radius * sx) / 8;
            if (left < 0)
              left = 0;
            if (right > JUDGMENT_RINGS_WIDTH - 1)
              right = JUDGMENT_RINGS_WIDTH - 1;
            if (top < 0)
              top = 0;
            if (bottom > JUDGMENT_RINGS_HEIGHT - 1)
              bottom = JUDGMENT_RINGS_HEIGHT - 1;
            work->sheet[bottom * JUDGMENT_RINGS_WIDTH + right] =
                JUDGMENT_RING_LEVEL;
            work->sheet[top * JUDGMENT_RINGS_WIDTH + right] =
                JUDGMENT_RING_LEVEL;
            work->sheet[bottom * JUDGMENT_RINGS_WIDTH + left] =
                JUDGMENT_RING_LEVEL;
            work->sheet[top * JUDGMENT_RINGS_WIDTH + left] =
                JUDGMENT_RING_LEVEL;
            right = cx + halfSlope + 1;
            left = cx - halfSlope + 1;
            if (left < 0)
              left = 0;
            if (right > JUDGMENT_RINGS_WIDTH - 1)
              right = JUDGMENT_RINGS_WIDTH - 1;
            work->sheet[bottom * JUDGMENT_RINGS_WIDTH + right] =
                JUDGMENT_RING_LEVEL;
            work->sheet[top * JUDGMENT_RINGS_WIDTH + right] =
                JUDGMENT_RING_LEVEL;
            work->sheet[bottom * JUDGMENT_RINGS_WIDTH + left] =
                JUDGMENT_RING_LEVEL;
            work->sheet[top * JUDGMENT_RINGS_WIDTH + left] =
                JUDGMENT_RING_LEVEL;

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
      s32 angle = -DEGtoANG(90);

      for (i = 0; i != 9; i++, angle += TRIG_TURN / 16) {
        s32 x = FIXEDtoINT((frame - 86) * Trig_Sin(angle));
        s32 y = FIXEDtoINT((frame - 70) * Trig_Cos(angle));

        draw[0](canvas,
                work->sheet +
                    ((frame / 2 + i) % 4) * JUDGMENT_EXPLOSION_PICTURE_BYTES +
                    JUDGMENT_SHEET_EXPLOSION,
                x + 44, y + 17, JUDGMENT_EXPLOSION_WIDTH,
                JUDGMENT_EXPLOSION_HEIGHT);
      }
    }

    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                        BLITTER_ADDITIVE);
    draw[1] = (DrawRectangle)Ram_HeapSlots->ramCodePaired;
    if (frame > 85) {
      draw[1](canvas, work->sheet, 0, 0, JUDGMENT_WAVE_SIZE, 120);
    }
    if (frame > 279) {
      shift -= 8;
    }
    if (frame == 238) {
      IWRAM_FILL_WORDS(canvas, CANVAS_BYTES,
                       CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
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
      for (i = 0; i != JUDGMENT_DEBRIS_OBJECTS; i++) {
        p = &work->particles[i];
        if (p->variant != 0) {
          scale[0] = p->z + (p->y >> 7);
          if (scale[0] <= toFIXED(1) / 32 - 1) {
            scale[0] = toFIXED(1) / 32;
          }
          scale[1] = scale[0];
          drift[0] = p->x;
          drift[1] = toFIXED(255);
          drift[2] = p->y + toFIXED(255);
          Render_ApplyProjectedPlacementFar(work->sprites[i], drift, scale, 0);
          p->y -= p->velocityY;
          if (p->y <= toFIXED(16) - 1) {
            if (frame <= 279) {
              p->x = INTtoFIXED((Random16() & 63) + shift + 32);
            } else {
              p->x = INTtoFIXED((Random16() & 63) + shift - 32);
            }
            if (frame > 301) {
              p->variant = 0;
            } else if (frame > 245) {
              p->y = toFIXED(96);
            } else {
              p->y = toFIXED(192) - p->x;
            }
          }
        }
      }
      if (frame <= 279) {
        for (i = 0; i != 8; i++) {
          s32 x =
              (i & 1) * 20 - (Random16() & 15) + (i / 2) * 4 + shift / 2 - 16;
          s32 y = (i / 2) * 32 - (Random16() & 15);

          draw[1](canvas,
                  work->sheet +
                      ((frame / 2 + i) % 4) * JUDGMENT_EXPLOSION_PICTURE_BYTES +
                      JUDGMENT_SHEET_EXPLOSION,
                  x, y, JUDGMENT_EXPLOSION_WIDTH, JUDGMENT_EXPLOSION_HEIGHT);
        }
      } else {
        for (i = 0; i != 8; i++) {
          s32 x =
              (i & 1) * 20 + (Random16() & 15) - (i / 2) * 4 + shift / 2 - 16;
          s32 y = (i / 2) * 32 - (Random16() & 15);

          draw[1](canvas,
                  work->sheet +
                      ((frame / 2 + i) % 4) * JUDGMENT_EXPLOSION_PICTURE_BYTES +
                      JUDGMENT_SHEET_EXPLOSION,
                  x, y, JUDGMENT_EXPLOSION_WIDTH, JUDGMENT_EXPLOSION_HEIGHT);
        }
      }
    }
    HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
    if (frame > 63) {
      s32 y = Random16() % 4 + 30;

      gBgScroll[1].y = y;
    }
    EFFECT_END_FRAME(work);
  }

  for (i = 0; i != JUDGMENT_DEBRIS_OBJECTS; i++) {
    Sprite_DestroyFar(work->sprites[i]);
  }
  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

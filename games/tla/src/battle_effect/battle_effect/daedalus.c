/// @file
/// A symmetrical burst over the target, and the Daedalus summon.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"
#include "fixed_math.h"
#include "sounds.h"

#define BURST_FRAMES 48
#define BURST_DRAW_FRAMES 28

/// The three 64x64 cels of the burst, built in the sheet's first 0x3000
/// bytes; the picture loads behind them.
#define CEL_SIZE 0x1000
#define PICTURE_OFFSET 0x4000

/// Copies a half-picture of width by height pixels into the bottom middle of
/// a 64x64 cel, once as it is and once mirrored, so the cel is symmetrical.
#define MIRROR_HALF(cel, half, width, height)                                  \
  for (y = 0; y != (height); y++) {                                            \
    for (x = 0; x != (width); x++) {                                           \
      work->sheet[(cel) * CEL_SIZE + (y + 64 - (height)) * 64 + 32 - (width) + \
                  x] = work->sheet[(half) + y * (width) + x];                  \
      work->sheet[(cel) * CEL_SIZE + (y + 64 - (height)) * 64 + 31 + (width) - \
                  x] = work->sheet[(half) + y * (width) + x];                  \
    }                                                                          \
  }

/// A burst cel's texture: log2 of its width and height in the first word
/// (64x64 here), then its pixels.
struct BurstTexture {
  u32 size;
  void *pixels;
};

// One entry of the list Render_DrawModelList draws; kind 6 is a textured quad.

/// Raises a mirrored wall at the first target's feet while the target glows.
void BattleEffect_RunMirroredWall(struct BattleEffectParams *args, s32 mode) {
  struct BattleEffectWork *work =
      (struct BattleEffectWork *)Ram_HeapSlots->battleEffect;
  struct BurstTexture texture;
  struct ScreenPoint target;
  struct RenderPrimitive *quad;
  void *vertices;
  s32 frame;
  s32 cel;
  s32 y;
  s32 x;

  // Build the three cels from their halves.
  BattleEffect_StartCanvasLayer(0);
  Resource_LoadAndDecompress((s32)&ResourceId_EarthWallSheet,
                             work->sheet + PICTURE_OFFSET, 1, 1);
  Iwram_ClearWords(work->sheet, 0x4000);
  MIRROR_HALF(0, PICTURE_OFFSET + 0xa8e, 17, 49)
  MIRROR_HALF(1, PICTURE_OFFSET + 0xdcf, 19, 55)
  MIRROR_HALF(2, PICTURE_OFFSET + 0x11e4, 20, 64)
  if (mode == 3)
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB),
                    128);
  else if (mode == 2)
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_FireballSheet),
                    128);
  else
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet),
                    128);

  // Centre the canvas's x 64 on the target.
  EffectPosition_ProjectBattler(args->targets[0], &target);
  REG_BG2PA = Q_8_8(1);
  REG_BG2X = (64 - target.x) << 8;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  Task_Add((s32)BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  if (mode == 2) {
    work->shakeFrames = 0;
    Sound_PlayFar(SE_POWER_UP);
  } else if (mode == 1 || mode == 3) {
    work->shakeFrames = 8;
    Sound_PlayFar(SE_POWER_UP);
  } else {
    work->shakeFrames = 32;
  }

  for (frame = 0; frame != BURST_FRAMES; frame++) {
    if (frame == 0) {
      if (mode == 2)
        BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                                 TINT_KEEP, 0, 32);
      else
        BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_RED_TINT,
                                 TINT_KEEP, 0, 32);
    }
    if (frame == 24)
      EFFECT_SHOW_DAMAGE(0);
    if (frame == 8 && mode == 0)
      Sound_PlayFar(SE_RECOVERY);

    // Cels 0, 1 and 2 four frames each, then 1 and 2 by turns.
    if (frame < 32) {
      cel = frame / 4;
      if (cel > 2)
        cel = (cel & 1) + 1;
      if (frame < BURST_DRAW_FRAMES) {
        vertices = Heap_AllocEwram(32);
        quad = Render_AllocateModelList(1);
        texture.size = (texture.size & ~0xff) | 6;
        texture.size = (texture.size & ~0xff00) | 6 << 8;
        texture.pixels = work->sheet + cel * CEL_SIZE;
        quad->kind = 6;
        quad->faces = gBattleEffect_SquareMesh;
        quad->texture = &texture;
        quad->unk14 = gMirroredWall_BurstShades[mode];
        quad->vertices = vertices;
        Transform_Reset();
        SceneTransform_ApplyPosition3(0, px(target.y - 58), 0);
        SceneTransform_ApplyUniformScale(gMirroredWall_BurstScales[mode] << 1);
        Render_TransformVertices(gBattleEffect_StandingSquareCorners, vertices,
                                 4);
        Render_DrawModelList(quad);
        Heap_Release(quad);
        Heap_Release(vertices);
      }
    }

    if (mode == 0)
      BattleCamera_Shake(4, 4);
    else
      BattleCamera_Shake(16, 16);
    EFFECT_END_FRAME(work);
  }

  Task_Remove((s32)BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  BattleEffect_EndCanvasLayer();
}

// Tiles: pack each horizontal pair of bytes into one byte.

/// Copies 288 rows of 40 bytes, reducing each horizontal pair to one byte.
/// The second byte, from the odd column, survives.
/// stride: source bytes per row; destination uses half as many.
void BattleEffect_PackTileRows(u8 *src, u8 *dst, s32 stride) {
  s32 y = 0;
  s32 h = 288;
  s32 offset = 0;

  do {
    s32 x;

    for (x = 0; x != 40; x++) {
      dst[offset / 2 + x / 2] = src[offset + x];
    }
    offset += stride;
    y++;
  } while (y != h);
}

typedef struct Sprite {
  u8 unk00[8];
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[2];
  s32 scale;
  u8 slot;
  u8 cacheFlags;
  u16 rotation;
  u8 size[2];
  u8 offsetX;
  u8 offsetY;
} Sprite;

/* VIEW(ability_effects): tag struct SpriteAttributes as struct
 * SpriteAttributes__ability_effects; battle_effect/cybele has another
 * definition; not tried: struct SpriteAttributes__ability_effects was not
 * found. */
/// A sprite's first ten bytes, its attribute 2 fields after eight others.
struct SpriteAttributes {
  u8 unk00[8];
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
};

typedef struct VramSlot {
  u16 unk0;
  u16 offset;
} VramSlot;

/* VIEW(f17e698): tag struct SummonWork as struct SummonWork__f17e698;
 * ability_effects.c has another definition; tried with the definition the rest
 * of the object has, it does not compile in tla-ja:
 * games/tla/src/battle_effect/battle_effect/daedalus.c:434: structure has no
 * member named `transferMode'. */
struct SummonWork__f17e698 {
  struct Particle particles[48];
  u8 unk0540[0x1c0];
  u8 frames[0x7080];
  s32 transferMode;
  s32 transferValue;
  u8 unk_7788[8];
  s32 scrollTimer;
  s32 scrollInterval;
  s32 scrollStepX;
  s32 scrollStepY;
  u8 unk77a0[8];
  s32 shakeFrames;
  u8 unk77ac[0x30];
  Sprite *sprites[57];
  u8 unk78c0[0x28];
  s32 transferPending;
};

typedef void (*CopyFunc)(void *, const void *, s32);
typedef void (*ClearFunc)(void *, s32);
typedef void (*BlitFunc)(void *, const void *, s32, s32, s32, s32);

/// Iwram_CopyWords reached from IwramIrqMain rather than at its fixed address,
/// as the trade's snapshot copies reach it: its address is then a pool load,
/// made after the arguments and after the call before it.
#define IWRAM_COPY_WORDS_LINKED                                                \
  ((CopyFunc)(IwramIrqMain + Iwram_CopyWordsOffset))
#define Iwram_Clear Iwram_ClearWordsEntry

#define DECODE_BUFFER Ram_MapCellBuffer
#define PARTICLES ((struct Particle *)(Ram_MapCellBuffer + 0x4000))

/// Daedalus: the great machine glides in over the field and fires on the foes,
/// then the second scene (BattleEffect_RunDaedalusAlt) plays.
///
/// Abilities: Daedalus.
void BattleEffect_RunDaedalus(struct BattleEffectParams *args) {
  s16 targets[14];
  struct SpritePlacement pos;
  struct SpritePlacement shipPlace;
  struct SpritePlacement particlePlace;
  struct Scale scale;
  void *blit[2];
  struct Scale shipScale;
  struct ModelTexture bitmap;
  struct Scale particleScale;
  u8 *dest;
  s32 frame;
  u8 *map;
  s32 *ctl;
  s32 *bank;
  s32 camera;
  s32 prevCamera;
  s32 waveX;
  s32 waveScroll;
  s32 shipY;
  s32 wavePhase;
  s32 savedY;
  s32 bgX;
  struct SummonWork__f17e698 *work;
  s32 k;
  s32 i;
  s32 j;
  // The map rows are 16 words apart; held in a local for its whole life.
  s32 step = 16;
  // Reused: the second phase's ring radius, and the 0 the third phase stores.
  s32 radius;

  dest = Ram_HeapSlots->battleCanvas;
  work = (struct SummonWork__f17e698 *)Ram_HeapSlots->battleEffect;
  map = Ram_HeapSlots->battleSheet;
  ctl = (s32 *)Ram_HeapSlots->battleBackground;
  bank = (s32 *)Ram_HeapSlots->battleWork;
  BattleEffect_StartCanvasLayer(0);
  BattleEffect_ConfigureEffectDisplay();
  RagingHeat_FillGradientCanvas();
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
  blit[0] = Ram_HeapSlots->ramCode;
  work->transferMode = CANVAS_COPY;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  camera = 0;
  waveScroll = 0;
  shipY = -0x1d0000;
  wavePhase = 0;
  savedY = gBgScroll[1].x;
  bgX = -80;

  for (frame = 0; frame != 52; frame++) {
    if (frame == 27) {
      u16 *col;
      s32 n;
      s32 w;
      s32 off;
      u8 *mp;
      s32 k;
      s32 r, g, b;
      s32 v;
      const u8 *src;
      Sprite *obj;

      VramCache_FreeEntry(bank[21]);
      gCameraSceneParameters[4] = 240;
      targets[0] = 255;
      BattleActor_SpawnObjectsForListFar(targets, 0);
      BattleBackground_LoadFar(1, (s32)&ResourceId_VioletSkyBackdrop, 0);
      for (col = (u16 *)(BG_PLTT + 6 * 16), i = 0; i != 128; i++, col++) {
        s32 mid;
        s32 s;

        s = *col & 0x1f;
        mid = (*col >> 5) & 0x1f;
        b = ((*col >> 10) & 0x1f) - 4;
        g = mid - 4;
        r = s - 4;
        if (g < 0)
          g = 0;
        if (r < 0)
          r = 0;
        if (b < 0)
          b = 0;
        if (g > 31)
          g = 31;
        if (r > 31)
          r = 31;
        if (b > 31)
          b = 31;
        *col = (b << 10) | (r << 5) | g;
      }
      work->scrollTimer = 0;
      work->scrollInterval = 5;
      work->scrollStepX = -1;
      work->scrollStepY = 0;
      Task_Add(BattleEffect_AdvanceScrollOnInterval, TASK_PASS_VBLANK);
      ctl[4] = 1;
      Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA,
                                 work->frames, 0, 0);
      off = 0x700;
      n = 3;
      for (j = 0; j != 10; j++) {
        k = j * step;
        i = 0;
        // v, the loop's value local, first holds the row's byte offset.
        v = k * 2;
        mp = (u8 *)(v + (s32)map);
        do {
          v = ((u8 *)work)[gBattleEffect_MotePictures[n] + i + off];
          /* FAKEMATCH(dead): w set to 32 inside the row loop, after the load.
           */
          w = 32;
          if (v != 0) {
            v = v - j * 4 - 40;
            if (v <= 0)
              v = 1;
          }
          *mp++ = v;
        } while (++i != w);
      }
      src = (const u8 *)Resource_GetTableEntry(
          (s32)&ResourceId_ForestCanopyTiles);
      Iwram_CopyWords((void *)(OBJ_PLTT + 15 * 16), src, 32);
      src += 32;
      Decompress_TaggedLz(src, work->frames);
      for (j = 0; j != 2; j++) {
        for (i = 0; i != 8; i++) {
          obj = EffectSprite_CreateWithTiles(32, 32, 0x80000000, 0xf000);
          work->sprites[j * 9 + i] = obj;
          obj->priority = 1;
          Iwram_CopyWords(
              (void *)(gVramCacheEntries[obj->slot].offset + 0x06010000),
              &work->frames[(j << 12) + i * 0x200], 0x200);
        }
        obj = EffectSprite_Acquire(32, 32, 0x80000000, 0xf000);
        work->sprites[j * 9 + 8] = obj;
        Iwram_CopyWords(obj, work->sprites[j * 9], 24);
        for (i = 0; i != 8; i++)
          work->particles[j * 9 + i].variant = work->sprites[j * 9 + i]->tile;
      }
      src = (const u8 *)Resource_GetTableEntry(
          (s32)&ResourceId_BronzeAutomatonTiles);
      Iwram_CopyWords((void *)OBJ_PLTT, src, 0x1c0);
      src += 0x1c0;
      Decompress_TaggedLz(src, DECODE_BUFFER);
      for (i = 0; i != 21; i++) {
        obj = EffectSprite_CreateWithTiles(32, 32, 0x80002000, 0);
        obj->priority = 3;
        work->sprites[i + 18] = obj;
        Iwram_CopyWords(
            (void *)(gVramCacheEntries[obj->slot].offset + 0x06010000),
            DECODE_BUFFER + i * 0x400, 0x400);
      }
      obj = EffectSprite_CreateWithTiles(32, 64, 0xc000a000, 0);
      // Once-only: the store alone in its block.
      do {
        work->sprites[i + 18] = obj;
      } while (0);
      obj->priority = 3;
      /* FAKEMATCH(barrier): an asm operand cuts i, known 21, from the copy's
       * source. */
      __asm__("" : "=r"(i) : "0"(i));
      Iwram_CopyWords(
          (void *)(gVramCacheEntries[obj->slot].offset + 0x06010000),
          DECODE_BUFFER + i * 0x400, 0x800);
      src = (const u8 *)Resource_GetTableEntry((s32)&ResourceId_MissileTiles);
      Iwram_CopyWords((void *)(OBJ_PLTT + 14 * 16), src, 32);
      src += 32;
      Decompress_TaggedLz(src, DECODE_BUFFER);
      obj = EffectSprite_CreateWithTiles(16, 8, 0x4000, 0xe000);
      obj->priority = 1;
      work->sprites[41] = obj;
      Iwram_CopyWords(
          (void *)(gVramCacheEntries[obj->slot].offset + 0x06010000),
          DECODE_BUFFER, 0x100);
      /* FAKEMATCH(barrier): an empty asm. */
      __asm__("");
      for (i = 0; i != 15; i++) {
        work->sprites[42 + i] = EffectSprite_Acquire(16, 8, 0x4000, 0xf000);
        Iwram_CopyWords(work->sprites[42 + i], work->sprites[41], 24);
        work->sprites[42 + i]->tile = work->sprites[41]->tile;
        if (i > 6)
          work->sprites[42 + i]->priority = 2;
      }
    }
    if (frame > 27) {
      struct SpritePlacement *posp;
      s32 row;
      Sprite **slot;

      scale = gDaedalus_WaveScale;
      pos.groundHeight = 0;
      pos.height = px(255);
      waveScroll += 0x8000;
      waveX = waveScroll + camera;
      if (waveX > 0x1fffff) {
        wavePhase = (wavePhase + 7) % 8;
        waveX -= 0x200000;
        waveScroll -= 0x200000;
      }
      for (j = 0; j != 2; j++) {
        for (i = 0; i != 9; i++) {
          Sprite *obj;
          s32 v;

          posp = &pos;
          {
            s32 h;

            k = (i << 21) + waveX;
            h = (j << 5) + 103;
            posp->x = k;
            posp->y = px(h);
          }
          if (i == 8) {
            obj = work->sprites[j * 9 + 8];
            v = work->particles[j * 9 + (wavePhase + 71) % 8].variant;
          } else {
            row = j * 9;
            k = row;
            k += i;
            obj = work->sprites[k];
            v = work->particles[row + (wavePhase + 64 + i) % 8].variant;
          }
          ((struct SpriteAttributes *)obj)->tile = v & 0x3ff;
          if (pos.x > 0xffffff)
            pos.x -= px(288);
          Render_ApplyProjectedPlacementFar(work->sprites[j * 9 + i], &pos,
                                            &scale, 0);
        }
      }
    }
    bgX += 4;
    REG_BG2X = bgX << 8;
    EFFECT_PRESENT_CANVAS(work);
  }

  {
    void **blits;
    u16 *pal;
    s32 swayX;
    s32 swayY;
    s32 bobX;
    s32 bobY;
    s32 *ringX;
    void *pixels;

    HeapSlot_Release(HEAP_SLOT_RAM_CODE);
    Iwram_Clear(dest, 0x3c00);
    Iwram_Clear((void *)BG_CHAR_BLOCK(1), 0x4000);
    REG_BG2CNT = BGCNT_PRIORITY(2) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
                 BGCNT_SCREEN_BASE(7);
    REG_BG2PA = Q_8_8(0.5);
    REG_BG2X = 0;
    blits = blit;
    BattleEffect_FetchRectangleBlitters(0, blits);
    work->scrollTimer = 0;
    work->scrollInterval = 5;
    work->scrollStepX = -1;
    work->scrollStepY = 0;
    ctl[4] = 1;
    Resource_LoadAndDecompress((s32)&ResourceId_Data0BA, work->frames, 0, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                               work->frames + 0x6c0, 1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_Data0BB, work->frames + 0x36c0,
                               0, 0);
    pixels = DECODE_BUFFER;
    Resource_LoadAndDecompress((s32)&ResourceId_Data0C2, pixels, 0, 0);
    Iwram_CopyWords(
        (void *)BG_PLTT,
        (const void *)Resource_GetTableEntry((s32)&ResourceId_SmokeSheet), 128);
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
    REG_BLDALPHA = BLEND_ALPHA(16, 16);
    REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
    for (i = 0; i != 224; i++) {
      pal = (u16 *)OBJ_PLTT + i;
      if (((*pal >> 5) & 0x1f) == 31)
        break;
    }
    for (i = 0; i != 512; i++)
      PARTICLES[i].variant = -1;

    frame = 0;
    if (!(gInput.held & KEY_B) && !(gInput.repeat & KEY_A)) {
    phase2:
      if (frame == 0) {
        REG_BG2PA = Q_8_8(1);
        *pal = 0x190b;
        shipY = -0x1d0000;
        camera = 0;
        work->sprites[39]->rotation = TRIG_TURN * 3 / 4;
        work->sprites[38]->rotation = 0;
        work->sprites[31]->rotation = 0;
        work->particles[20].x = 0;
        work->particles[20].y = 0;
        work->particles[20].velocityX = -px(6);
        work->particles[20].velocityY = 0;
        work->particles[20].variant = 0;
        work->particles[23].x = 0;
        work->particles[23].y = 0;
        work->particles[23].velocityX = -px(12);
        work->particles[23].velocityY = 0;
        work->particles[23].variant = 0;
        work->particles[21].x = 0;
        work->particles[21].y = 0;
        work->particles[21].velocityX = -px(1);
        work->particles[21].velocityY = 0;
        work->particles[21].variant = 0;
        work->particles[22].x = px(160);
        work->particles[22].y = 0;
        work->particles[22].velocityX = 0;
        work->particles[22].velocityY = 0;
        work->particles[22].variant = 0;
        for (i = 0; i != 8; i++) {
          struct Particle *p;

          p = &work->particles[32 + i];
          p->x = ((i / 3) << 18) + px(176);
          p->y = ((i % 3) * 3 << 17) + px(74);
          p->velocityX = 0;
          p->velocityY = 0;
          p->variant = 0;
        }
        for (i = 0; i != 8; i++) {
          struct Particle *p;

          p = &work->particles[40 + i];
          p->x = ((i / 3) << 18) + px(112);
          p->y = ((i % 3) * 3 << 17) + px(82);
          p->velocityX = 0;
          p->velocityY = 0;
          p->variant = 0;
        }
      }
      prevCamera = camera;
      if (frame >= 542 && frame < 558) {
        s32 r, g, b;

        r = *pal & 0x1f;
        g = (*pal >> 5) & 0x1f;
        b = (*pal >> 10) & 0x1f;
        if (r < 31)
          r++;
        if (g < 20)
          g++;
        if (b < 6)
          b++;
        *pal = (b << 10) | (g << 5) | r;
      }
      if (frame > 557) {
        s32 r, g, b;

        r = *pal & 0x1f;
        g = (*pal >> 5) & 0x1f;
        b = (*pal >> 10) & 0x1f;
        if (r > 20)
          r--;
        if (g > 0)
          g--;
        if (b > 0)
          b--;
        *pal = (b << 10) | (g << 5) | r;
      }
      if (frame == 280) {
        Iwram_CopyWords(
            (void *)BG_PLTT,
            (const void *)Resource_GetTableEntry((s32)&ResourceId_SmokeSheet),
            128);
        work->particles[20].variant = 1;
      }
      if (frame == 288)
        work->particles[23].variant = 1;
      if (frame == 279) {
        REG_BG2PA = Q_8_8(0.5);
        REG_BG2CNT =
            BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
        REG_BG2PA = Q_8_8(0.5);
      }
      if (frame == 506)
        work->particles[21].variant = 1;
      if (frame == 489) {
        REG_BG2PA = Q_8_8(0.5);
        REG_BG2CNT =
            BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
        work->scrollStepX = 0;
      }
      if (frame == 528) {
        REG_BG2PA = Q_8_8(0.5);
        REG_BG2CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
                     BGCNT_SCREEN_BASE(7);
      }
      if (frame == 0) {
        EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
        EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
        for (i = 0; i != 10; i++) {
          struct Particle *p;

          p = &PARTICLES[502 + i];
          p->x = px(gDaedalus_DriftStarts[i][0]);
          p->y = px(gDaedalus_DriftStarts[i][1]);
          p->velocityX = gDaedalus_DriftSpeeds[i][0];
          p->velocityY = gDaedalus_DriftSpeeds[i][1];
          p->variant = Random16() & 127;
        }
      }
      for (i = 0; i != 10; i++) {
        struct Particle *p;

        p = &PARTICLES[502 + i];
        if (frame >= gDaedalus_DriftFrames[i]) {
          ((BlitFunc)blit[0])(
              dest,
              &work->frames[(gDaedalus_DriftPictures[(p->variant / 4) % 12]
                             << 8) +
                            0x36c0],
              p->x >> 16, p->y >> 16, 16, 16);
          p->x += p->velocityX;
          p->y += p->velocityY;
          p->variant++;
        }
      }
      if (frame <= 473)
        waveScroll += 0x8000;
      waveX = waveScroll + camera;
      if (waveX >= 0x200000) {
        wavePhase = (wavePhase + 7) % 8;
        waveX -= 0x200000;
        waveScroll -= 0x200000;
      }
      if (shipY < 0)
        shipY += 0x2000;
      shipScale = gDaedalus_ShipScale;
      bobX = 0;
      bobY = 0;
      if (frame > 537) {
        swayX = Trig_Cos(0x21a00) << 2;
        swayY = Trig_Sin(0x21a00) << 2;
        if (frame >= 542 && frame < 558) {
          s32 v;

          v = (frame << 11) - 0x10f000;
          bobX = Trig_Sin(v) * 3;
          bobY = Trig_Sin(v) * 2;
        }
        if (frame >= 560 && frame < 576) {
          s32 v;

          v = (frame << 11) - 0x118000;
          bobX = Trig_Sin(v) * 2;
          bobY = Trig_Sin(v) * 3;
        }
      } else {
        swayX = Trig_Cos(frame << 8) << 2;
        swayY = Trig_Sin(frame << 8) << 2;
      }
      if (swayY > 0)
        swayY = -swayY;
      swayY -= shipY * 2;
      shipPlace.groundHeight = 0;
      shipPlace.height = px(255);
      for (i = 0; i != 5; i++) {
        if (frame == gDaedalus_ClatterFrames[i]) {
          work->shakeFrames = 23;
          Sound_PlayFar(SE_CLATTER);
        }
      }
      if (frame == 510)
        Sound_PlayFar(SE_SOL_SANCTUM_LIGHT_UP);
      if (frame == 568) {
        work->shakeFrames = 10;
        Sound_PlayFar(SE_CRASH);
      }
      if (frame == 578)
        Sound_PlayFar(SE_163);
      for (i = 0; i != 16; i++) {
        struct Particle *p;

        p = &work->particles[32 + i];
        if (p->variant != 0) {
          struct Particle *q;

          p->variant++;
          if (p->x > -px(16)) {
            p->x += p->velocityX;
            p->y += p->velocityY;
            if (p->velocityY > -px(1) && p->velocityY < px(1))
              p->velocityY += p->velocityZ;
          }
          if (p->velocityX > -px(8))
            p->velocityX -= px(1) / 2;
          q = &PARTICLES[i * 32 + (p->variant & 31)];
          q->variant = 0;
          q->x = p->x / 2;
          q->y = p->y - px(20);
          q->velocityX = 0;
          q->velocityY = 0;
        }
        if (work->particles[20].variant == (i & 7) * 6 + 10) {
          Sound_PlayFar(SE_ITEM_BREAK);
          p->variant = 1;
          p->x += swayX;
          p->y += swayY - px(gDaedalus_ShakeOffsets[work->shakeFrames]);
          p->velocityY = (Random16() & 0xffff) - 0x8000;
          p->velocityZ = (Random16() & 0x3fff) - 0x2000;
        }
      }
      if (frame <= 489) {
        for (i = 8; i != 16; i++) {
          struct Particle *p;

          p = &work->particles[32 + i];
          shipPlace.x = p->x + camera;
          shipPlace.y = p->y;
          if (p->variant != 0)
            Render_ApplyProjectedPlacementFar(work->sprites[40 + i], &shipPlace,
                                              &shipScale, 0);
        }
      }
      if (frame == 578) {
        work->scrollInterval = 1;
        work->scrollTimer = 0;
      }
      if (frame >= 578 && frame < 594)
        work->scrollStepX = ~((frame - 578) / 8);
      {
        for (i = 0; i != 2; i++) {
          shipPlace.x = camera - swayX + bobX + px(120);
          shipPlace.y =
              px((i << 5) - gDaedalus_ShakeOffsets[work->shakeFrames]) + swayY +
              bobY + 0x640000;
          Render_ApplyProjectedPlacementFar(work->sprites[32 + i], &shipPlace,
                                            &shipScale, 0);
        }
      }
      if (frame <= 316) {
        struct Particle *p;

        p = &work->particles[23];
        if (p->variant != 0) {
          p->variant++;
          p->x += p->velocityX;
          p->y += p->velocityY;
          p->velocityY += px(1) / 8;
          p->velocityX = p->velocityX * 48 / 64;
          p->velocityY = p->velocityY * 63 / 64;
          work->sprites[38]->rotation += 0x200;
        }
        shipPlace.x = p->x - swayX + camera + px(120);
        shipPlace.y =
            px(99 - gDaedalus_ShakeOffsets[work->shakeFrames]) + swayY + p->y;
        Render_ApplyProjectedPlacementFar(work->sprites[38], &shipPlace,
                                          &shipScale, 0);
      }
      {
        struct Particle *p;
        s32 v;

        v = camera + bobX;
        shipPlace.x = v + px(142);
        shipPlace.y =
            px(72 - gDaedalus_ShakeOffsets[work->shakeFrames]) + swayY + bobY;
        Render_ApplyProjectedPlacementFar(work->sprites[18], &shipPlace,
                                          &shipScale, 0);
        shipPlace.x = v + px(174);
        shipPlace.y =
            px(72 - gDaedalus_ShakeOffsets[work->shakeFrames]) + swayY + bobY;
        Render_ApplyProjectedPlacementFar(work->sprites[19], &shipPlace,
                                          &shipScale, 0);
        v += 0x800000;
        shipPlace.x = v;
        shipPlace.y =
            px(104 - gDaedalus_ShakeOffsets[work->shakeFrames]) + swayY + bobY;
        Render_ApplyProjectedPlacementFar(work->sprites[20], &shipPlace,
                                          &shipScale, 0);
        p = &work->particles[22];
        if (frame > 602)
          BattleEffect_StepPaletteToResource((s32)&ResourceId_OrangePaletteB);
        if (frame == 570) {
          p->x = px(152);
          p->y = px(92 - gDaedalus_ShakeOffsets[work->shakeFrames]) + swayY;
          p->variant = 1;
          p->velocityX = -px(4);
          p->velocityY = -px(5) / 16;
          for (i = 0; i != 16; i++) {
            struct Particle *q;

            q = &PARTICLES[312 + i];
            q->x = px((Random16() & 15) + 56);
            q->y = px((Random16() & 31) + 64);
            q->velocityX = -((Random16() & 31) + 8) << 12;
            q->velocityY = -((Random16() & 31) + 8) << 12;
            q->variant = 0;
          }
          for (i = 0; i != 32; i++)
            PARTICLES[400 + i].variant = -1;
          for (i = 0; i != 32; i++)
            PARTICLES[432 + i].variant = -1;
        }
        if (p->variant != 0) {
          if (p->variant > 9) {
            struct Particle *q;

            q = &PARTICLES[400 + (p->variant & 31)];
            q->variant = 0;
            q->velocityX = -(Trig_Sin(work->sprites[39]->rotation) * 2);
            q->velocityY = Trig_Cos(work->sprites[39]->rotation) * 2;
            q->x = (p->x + camera) / 2 -
                   ((Trig_Sin(work->sprites[39]->rotation) * 27) >> 1);
            q->y = p->y + Trig_Cos(work->sprites[39]->rotation) * 27 - 0xc0000;
          }
          if (((p->variant >= 50 && p->variant < 60) ||
               (p->variant >= 113 && p->variant < 200)) &&
              (p->variant & 3) == 0) {
            struct Particle *q;

            q = &PARTICLES[432 + (p->variant & 31)];
            q->variant = 0;
            q->velocityX = ((Random16() & 31) + 16) << 12;
            q->velocityY = (-Random16() | ~15) << 14;
            q->x = px(88);
            q->y = px(128);
          }
          prevCamera = camera;
          {
            struct Particle *f;

            f = &work->particles[22];
            if (f->variant > 229)
              camera +=
                  (px(152) - (f->x + ((f->variant - 230) << 17)) - camera) / 4;
            else if (f->variant > 0)
              camera += (px(152) - f->x - camera) / 4;
          }
          if (p->variant > 1) {
            p->x += p->velocityX;
            p->y += p->velocityY;
          }
          if (p->variant > 7) {
            p->velocityX = p->velocityX * 63 / 64;
            p->velocityY = p->velocityY * 63 / 64 + 0x8c8;
          }
          if (p->variant < 80)
            p->velocityY += 0x200;
          if (p->variant > 29)
            p->velocityX -= px(1) / 4;
          if (p->variant >= 40 && p->variant < 68)
            p->velocityY -= px(1) / 16;
          if (p->variant >= 50 && p->variant < 60) {
            work->sprites[39]->rotation = (p->variant << 7) - 0x5900;
            p->velocityY -= px(1) / 16;
          }
          if (p->variant >= 108 && p->variant < 140) {
            if (p->variant >= 120 && p->variant < 123)
              work->sprites[39]->rotation -= 0x180;
            else
              work->sprites[39]->rotation += 0x100;
            p->velocityY -= px(1) / 16;
          }
          if (p->variant >= 140 && p->variant < 160)
            p->velocityY -= px(1) / 32;
          if (p->variant > 159)
            p->velocityY -= px(1) / 8;
          for (i = 0; i != 32; i++) {
            struct Particle *q;

            q = &PARTICLES[400 + i];
            if ((u32)q->variant <= 5) {
              ((BlitFunc)blits[1])(dest, &work->frames[q->variant * 288],
                                   (q->x >> 16) - 2, (q->y >> 16) - 12, 12, 24);
              BattleEffectKernels_IntegrateVector2(q, 64, 0);
              q->variant++;
            }
          }
          for (i = 0; i != 32; i++) {
            struct Particle *q;

            q = &PARTICLES[432 + i];
            if ((u32)q->variant <= 23) {
              ((BlitFunc)blits[1])(
                  dest, &work->frames[(q->variant / 4 << 11) + 0x6c0],
                  (q->x >> 16) - 16, (q->y >> 16) - 32, 32, 64);
              q->velocityX += px(1) / 16;
              BattleEffectKernels_IntegrateVector2(q, 64, 0);
              q->variant++;
            }
          }
          p->variant++;
        }
        shipScale.x = 0x103ff;
        shipScale.y = 0x103ff;
        if (p->variant == 0) {
          shipPlace.x = camera + bobX + px(152);
          shipPlace.y =
              px(92 - gDaedalus_ShakeOffsets[work->shakeFrames]) + swayY + bobY;
        } else {
          shipPlace.x = p->x + camera;
          shipPlace.y = p->y;
        }
        Render_ApplyProjectedPlacementFar(work->sprites[39], &shipPlace,
                                          &shipScale, 0);
      }
      work->sprites[39]->offsetX = 0;
      work->sprites[39]->offsetY = 32;
      if (frame >= 568) {
        s32 *beam;
        void *buffer;
        s32 v;
        s32 z;

        buffer = Heap_AllocEwram(512);
        beam = Render_AllocateModelList(1);
        bitmap.logWidth = 7;
        bitmap.logHeight = 7;
        bitmap.pixels = pixels;
        beam[4] = (s32)&bitmap;
        beam[1] = 0;
        beam[2] = (s32)gBattleEffect_LargeSquareMesh;
        beam[0] = 7;
        beam[3] = (s32)buffer;
        v = frame - 568;
        z = 96 - (v << 4);
        if (z > 0)
          z = 0;
        if (z >= -63) {
          beam[5] = z;
          Transform_Reset();
          SceneTransform_ApplyPosition3(camera + px(2), px(8), 0);
          Transform_ApplyRoll(0x4300);
          Transform_ApplyPitch(-TRIG_TURN / 32);
          SceneTransform_ApplyUniformScale((v << 14) + 0x4180);
          Render_TransformVertices(gBattleEffect_GroundSquareCorners, buffer,
                                   4);
          Render_DrawModelList(beam);
        }
        Heap_Release(beam);
        Heap_Release(buffer);
      }
      if (frame == 568) {
        work->particles[30].x = 0;
        work->particles[30].velocityX = px(8);
      }
      for (j = 0; j != 1; j++) {
        ringX = &work->particles[30].x;
        if (frame >= gDaedalus_RingStarts[j] &&
            frame < gDaedalus_RingStarts[j] + 26) {
          s32 n;

          radius = *ringX >> 16;
          n = (frame - gDaedalus_RingStarts[j]) / 2;
          *ringX += work->particles[30].velocityX;
          work->particles[30].velocityX =
              work->particles[30].velocityX * 48 / 64;
          if (n <= 5) {
            for (i = 0; i != 32; i++) {
              s32 v;

              v = (Trig_Sin(i << 11) * radius / 2 >> 16) + gDaedalus_RingX[j];
              ((BlitFunc)blit[0])(dest, &work->frames[n * 288], v - 6,
                                  (Trig_Cos(i << 11) * radius * 2 >> 16) + 60,
                                  12, 24);
            }
          }
        }
      }
      shipScale.x = Q16_ONE;
      shipScale.y = Q16_ONE;
      work->sprites[21]->priority = 2;
      work->sprites[22]->priority = 2;
      work->sprites[23]->priority = 2;
      work->sprites[24]->priority = 2;
      {
        s32 v;

        v = prevCamera + bobX;
        shipPlace.x = v + px(160);
        shipPlace.y =
            px(104 - gDaedalus_ShakeOffsets[work->shakeFrames]) + swayY + bobY;
        Render_ApplyProjectedPlacementFar(work->sprites[21], &shipPlace,
                                          &shipScale, 0);
        shipPlace.x = v + px(128);
        shipPlace.y =
            px(136 - gDaedalus_ShakeOffsets[work->shakeFrames]) + swayY + bobY;
        Render_ApplyProjectedPlacementFar(work->sprites[22], &shipPlace,
                                          &shipScale, 0);
        shipPlace.x = v + px(160);
        shipPlace.y =
            px(136 - gDaedalus_ShakeOffsets[work->shakeFrames]) + swayY + bobY;
        Render_ApplyProjectedPlacementFar(work->sprites[23], &shipPlace,
                                          &shipScale, 0);
        shipPlace.x = v + px(192);
        shipPlace.y =
            px(136 - gDaedalus_ShakeOffsets[work->shakeFrames]) + swayY + bobY;
        Render_ApplyProjectedPlacementFar(work->sprites[24], &shipPlace,
                                          &shipScale, 0);
        work->sprites[25]->priority = 2;
        shipPlace.x = v + px(128);
        shipPlace.y =
            px(104 - gDaedalus_ShakeOffsets[work->shakeFrames]) + swayY + bobY;
        Render_ApplyProjectedPlacementFar(work->sprites[25], &shipPlace,
                                          &shipScale, 0);
      }
      for (i = 0; i != 5; i++) {
        shipPlace.x = px(gDaedalus_SpritePoints[i][0]) + swayX + prevCamera +
                      bobX + 0xb00000;
        shipPlace.y = ((gDaedalus_SpritePoints[i][1] -
                        gDaedalus_ShakeOffsets[work->shakeFrames])
                       << 16) +
                      swayY + bobY + 0x540000;
        work->sprites[26 + i]->priority = 2;
        Render_ApplyProjectedPlacementFar(work->sprites[26 + i], &shipPlace,
                                          &shipScale, 0);
      }
      if (frame <= 489) {
        for (i = 0; i != 8; i++) {
          struct Particle *p;

          p = &work->particles[32 + i];
          shipPlace.x = p->x + camera;
          shipPlace.y = p->y;
          if (p->variant != 0)
            Render_ApplyProjectedPlacementFar(work->sprites[41 + i], &shipPlace,
                                              &shipScale, 0);
        }
      }
      if (frame < 540) {
        struct Particle *p;

        p = &work->particles[21];
        if (p->variant == 1) {
          p->x += p->velocityX;
          p->y += p->velocityY;
          p->velocityY += px(1) / 8;
          p->velocityX = p->velocityX * 60 / 64;
          p->velocityY = p->velocityY * 63 / 64;
        }
        for (i = 0; i != 4; i++) {
          shipPlace.x = ((i & 1) << 21) + p->x + camera + px(128);
          shipPlace.y =
              ((((i / 2) << 5) - gDaedalus_ShakeOffsets[work->shakeFrames])
               << 16) +
              swayY + p->y + px(104);
          work->sprites[34 + i]->priority = 1;
          Render_ApplyProjectedPlacementFar(work->sprites[34 + i], &shipPlace,
                                            &shipScale, 0);
        }
      }
      if (frame <= 316) {
        struct Particle *p;

        p = &work->particles[20];
        if (p->variant != 0) {
          p->variant++;
          p->x += p->velocityX;
          p->y += p->velocityY;
          p->velocityY += px(1) / 8;
          p->velocityX = p->velocityX * 48 / 64;
          p->velocityY = p->velocityY * 63 / 64;
          work->sprites[31]->rotation += 0x100;
        }
        shipPlace.x = swayX + p->x + camera + px(184);
        shipPlace.y =
            px(92 - gDaedalus_ShakeOffsets[work->shakeFrames]) + swayY + p->y;
        work->sprites[31]->priority = 1;
        Render_ApplyProjectedPlacementFar(work->sprites[31], &shipPlace,
                                          &shipScale, 0);
      }
      {
        struct SpritePlacement *wave;
        s32 row;
        Sprite **slot;

        for (j = 0; j != 2; j++) {
          for (i = 0; i != 9; i++) {
            Sprite *obj;
            s32 v;

            wave = &shipPlace;
            wave->x = (i << 21) + waveX;
            wave->y = px((j << 5) - gDaedalus_ShakeOffsets[work->shakeFrames]) +
                      shipY + 0x840000;
            if (i == 8) {
              work->sprites[j * 9 + i]->tile =
                  work->particles[j * 9 + (wavePhase + 71) % 8].variant & 0x3ff;
            } else {
              row = j * 9;
              k = row;
              k += i;
              work->sprites[k]->tile =
                  work->particles[row + (wavePhase + 64 + i) % 8].variant &
                  0x3ff;
            }
            if (shipPlace.x > 0xffffff)
              shipPlace.x -= px(288);
            Render_ApplyProjectedPlacementFar(work->sprites[j * 9 + i],
                                              &shipPlace, &shipScale, 0);
          }
        }
      }
      for (j = 0; j != 4; j++) {
        for (i = 0; i != 6; i++) {
          if (frame >= i * 2 + 490 && frame < i * 2 + 502) {
            s32 x, y, n;

            x = gDaedalus_SplashPoints[j][i][0];
            n = (frame - (i * 2 + 490)) / 2;
            y = gDaedalus_SplashPoints[j][i][1] + (swayY >> 16) -
                gDaedalus_ShakeOffsets[work->shakeFrames];
            ((BlitFunc)blit[0])(dest, &work->frames[n * 288], x - 6, y - 12, 12,
                                24);
          }
        }
      }
      for (i = 0; i != 16; i++) {
        for (j = 0; j != 32; j++) {
          struct Particle *q;

          q = &PARTICLES[i * 32 + j];
          if ((u32)q->variant <= 5) {
            ((BlitFunc)blit[0])(dest, &work->frames[q->variant * 288],
                                (q->x >> 16) - 2, (q->y >> 16) - 12, 12, 24);
            q->x += q->velocityX;
            q->y += q->velocityY;
            q->velocityY -= px(1) / 4;
            q->variant++;
          }
        }
      }
      {
        s32 *shake;
        s32 v;
        struct BgScroll *bg;
        s32 *params;

        shake = &work->shakeFrames;
        bg = gBgScroll;
        v = gDaedalus_ShakeOffsets[*shake];
        bg[1].y = v + 32;
        gCameraSceneParameters[4] = 120 - v;
        if (*shake > 0)
          *shake -= 1;
      }
      EFFECT_END_FRAME(work);
      frame++;
      if (frame != 800 && !(gInput.held & KEY_B) && !(gInput.repeat & KEY_A))
        goto phase2;
    }

    Task_Remove(BattleEffect_AdvanceScrollOnInterval);
    gBgScroll[1].x = savedY;
    ctl[4] = 0;
    BattleEffect_SetupBlendedDisplay();
    for (i = 0; i != 57; i++)
      Sprite_DestroyFar(work->sprites[i]);
    bank[21] = VramCache_AllocEntry(128);
    Palette_LoadBasicSpriteColors();
    Dma_Set((void *)OBJ_PLTT, (void *)(BG_PLTT + 5 * 16), 0x80000010, REG_DMA3);
    *(u16 *)(BG_PLTT + 94) = *(u16 *)(BG_PLTT + 244);
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
    REG_BG2X = 0;
    REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
    REG_BG2PA = Q_8_8(0.5);
    REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
    REG_BLDALPHA = BLEND_ALPHA(16, 16);
    args->targets[args->targetCount] = 0xff;
    BattleActor_SpawnObjectsForListFar(args->targets, 0);
    BattleEffect_SpawnObjects(1, 663, 1);
    for (i = 0; i != 15; i++) {
      work->sprites[1 + i] = EffectSprite_Acquire(32, 16, 0x80006000, 0);
      IWRAM_COPY_WORDS_LINKED(work->sprites[1 + i], work->sprites[0], 24);
      work->sprites[1 + i]->offsetX = 32;
      work->sprites[1 + i]->offsetY = 8;
    }
    Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, work->frames,
                               1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_Data0BA, work->frames + 0x3000,
                               0, 0);
    {
      const void *colors =
          (const void *)Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB);
      CopyFunc copy = IWRAM_COPY_WORDS_LINKED;

      copy((void *)BG_PLTT, colors, 128);
    }

    for (frame = 0; frame != 109; frame++) {
      if (frame == 0) {
        s32 z;
        struct Particle *p;

        i = 0;
        radius = 0;
        for (; i != 16; i++) {
          /* FAKEMATCH(dead): a store nothing reads. */
          if (work == NULL)
            k = 0;
          p = &work->particles[i];
          z = i * -0x100;
          p->variant = radius;
          Random16();
          p->velocityZ = radius;
          p->z = z - (*(volatile s32 *)&p->velocityZ * 40) - 0x800;
          p->velocityX = -Trig_Cos(p->z) * 8;
          p->velocityY = -Trig_Sin(p->z) * 8;
          p->x = px(Random16() % 80 + 80) - p->velocityX * 24;
          p->y = px((Random16() & 7) + 96) - p->velocityY * 24;
          work->sprites[i]->rotation = p->z;
          work->sprites[i]->offsetX = radius;
          work->sprites[i]->offsetY = 8;
        }
        for (i = 0; i != 512; i++)
          PARTICLES[i].variant = -1;
      }
      particleScale = gDaedalus_ParticleScale;
      particlePlace.groundHeight = 0;
      particlePlace.height = px(255);
      for (i = 0; i != 16; i++) {
        struct Particle *p;

        p = &work->particles[i];
        if (frame >= gDaedalus_ParticleStarts[i] && p->variant == 0) {
          particlePlace.x = p->x;
          particlePlace.y = p->y;
          if (particlePlace.y > 0 && particlePlace.y < px(128))
            Render_ApplyProjectedPlacementFar(work->sprites[i], &particlePlace,
                                              &particleScale, 0);
          if (p->y > 0x6fffff) {
            p->variant = 1;
            for (j = 0; j != 2; j++) {
              s32 speed;
              s32 angle;
              struct Particle *q;

              q = &PARTICLES[i * 8 + j];
              speed = (Random16() & 31) + 32;
              angle = (Random16() & 0x7fff) + TRIG_TURN / 4;
              q->x = p->x / 2;
              q->y = p->y;
              q->velocityX = Trig_Sin(angle) * speed >> 4;
              q->velocityY = Trig_Cos(angle) * speed >> 4;
              q->variant = 0;
            }
            Sound_PlayFar(SE_CLATTER);
            for (j = 0; j != args->targetCount; j++) {
              BattleEffect_StartActorMotion(args->targets[j], 1, 0, 0, 0x20000,
                                            100);
              BattleEffect_TintBattler(args->targets[j], SPRITE_COLOR_WHITE,
                                       POSE_DAMAGE, j, 7);
              work->shakeFrames = 4;
            }
          } else {
            struct Particle *q;

            {
              s32 m = (frame - gDaedalus_ParticleStarts[i]) % 16;
              q = &PARTICLES[(i << 4) + m + 256];
            }
            q->x = p->x / 2;
            q->y = p->y - px(16);
            q->velocityX = 0;
            q->velocityY = 0;
            q->variant = 0;
          }
          work->sprites[i]->rotation = p->z;
          BattleEffectKernels_IntegrateVector3(p, 64, 0);
          p->velocityX = -Trig_Cos(work->sprites[i]->rotation) * 8;
          p->velocityY = -Trig_Sin(work->sprites[i]->rotation) * 8;
          work->sprites[i]->tile = work->sprites[0]->tile;
        }
      }
      for (i = 0; i != 256; i++) {
        struct Particle *q;

        q = &PARTICLES[256 + i];
        if ((u32)q->variant <= 7) {
          s32 v;

          v = q->variant - 2;
          if (v >= 0)
            ((BlitFunc)blits[1])(dest, &work->frames[v * 288 + 0x3000],
                                 (q->x >> 16) - 6, (q->y >> 16) - 12, 12, 24);
          q->variant++;
        }
      }
      for (i = 0; i != 128; i++) {
        struct Particle *q;

        q = &PARTICLES[i];
        if ((u32)q->variant <= 17) {
          s32 v;

          v = q->variant / 3;
          ((BlitFunc)blits[i & 1])(dest, &work->frames[v << 11],
                                   (q->x >> 16) - 16, (q->y >> 16) - 48, 32,
                                   64);
          BattleEffectKernels_IntegrateVector2(q, 56, -px(1) / 8);
          q->variant++;
        }
      }
      BattleCamera_Shake(8, 8);
      EFFECT_END_FRAME(work);
    }
    for (i = 0; i != 16; i++)
      Sprite_DestroyFar(work->sprites[i]);
    EFFECT_FINISH_CANVAS();
  }
}

#undef BLITTER

/* VIEW(f17e698): tag struct PlacementSource as struct PlacementSource__f17e698;
 * ability_effects.c has another definition; tried with the definition the rest
 * of the object has, it does not compile in tla-ja:
 * games/tla/src/battle_effect/battle_effect/daedalus.c:1719: structure has no
 * member named `x'. */
struct PlacementSource__f17e698 {
  s32 x;
  s32 y;
};
extern const struct PlacementSource__f17e698
    gDaedalusAlt_SummonScale; // the summon's placement source
/// The sparks, after the map cells.
#define STRIKE_SPARKS ((struct Particle *)gData_02014000)

#define BLITTER ((DrawRectangle)Ram_HeapSlots->ramCode)
#define DAEDALUS_FRAMES 108
#define DAEDALUS_SPARKS 64

/// The summoned object's display fields that the scene sets.
struct SummonObject {
  u8 unk00[18];
  u16 rotation;
  u8 size[2];
  u8 offsetX;
  u8 offsetY;
};

/// The summoned object glides in from the right and down over 92 frames
/// while, from frame 56, a shaded disc swells at the centre and six rings of
/// light spin out from it. At the seven frames of gDaedalusAlt_StrikeFrames
/// every target is knocked back and flashes; from frame 80 eight bursts of
/// eight sparks, one a frame, scatter over the canvas.
void BattleEffect_RunDaedalusAlt(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle draw[2];
  struct ModelTexture texture;
  s32 request[4];
  struct PlacementSource__f17e698 source;
  s32 frame;
  s32 i;
  s32 j;
  s32 centerX;
  s32 centerY;
  s32 burst;
  struct Particle *spark;
  s32 mask;
  s32 discX;
  s32 ringOffset;
  s32 ringX;
  s32 discY;

  canvas = Ram_HeapSlots->battleCanvas;
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleEffect;
  BattleEffect_ConfigureEffectDisplay();
  REG_WIN0V = WIN_RANGE(16, 128);
  REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG1 |
              WININ_WIN1_BG2 | WININ_WIN1_CLR;
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  BattleEffect_SpawnObjects(1, 0x296, 3);
  BattleEffect_SelectLivingTargets(work->args);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
  draw[0] = BLITTER;
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, work->sheet, 1,
                             0);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  work->fadeFrames = 24;
  work->fadeStep = 0;

  frame = 0;
  burst = -80 * 8;
  do {
    if (frame == 0) {
      discX = 0x400000;
      ringOffset = -0x400000;
      ringX = 0x420000;
      discY = 0x500000;
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
      Iwram_CopyWordsEntry(
          (void *)BG_PLTT,
          Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB), 128);
    }
    if (frame == 56) {
      Sound_PlayFar(SE_SPIN);
    }
    for (i = 0; i != 7; i++) {
      if (frame == gDaedalusAlt_StrikeFrames[i]) {
        for (j = 0; j != args->targetCount; j++) {
          BattleEffect_StartActorMotion(args->targets[j], 1, 0x20000, 0x80000,
                                        0x20000, 120);
          BattleEffect_TintBattler(args->targets[j], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, j, 7);
        }
        work->shakeFrames = 4;
        Sound_PlayFar(SE_CRASH);
      }
    }

    // From frame 80: eight bursts of sparks, one a frame.
    if (frame > 79) {
      // One pointer to the sparks for the burst and the draw loop below.
      struct Particle *sparks;

      if (frame == 80) {
        // UB: the lives are cleared through one s32 pointer stepped a whole
        // step at a time, past the step's scalar members.
        s32 *life = &STRIKE_SPARKS[0].variant;

        for (i = 0; i != DAEDALUS_SPARKS; i++) {
          *life = -1;
          life += sizeof(struct Particle) / sizeof(s32);
        }
        work->shakeFrames = 8;
        Iwram_FillWordsEntry(canvas, 0x3c00, 0x3f3f3f3f);
      }
      sparks = STRIKE_SPARKS;
      if ((u32)(frame - 80) <= 7) {
        centerX = px((Random16() & 63) + 28);
        centerY = px((Random16() & 31) + 80);

        j = 0;
        mask = 15;
        spark = sparks + burst;
        for (; j != 8; j++, spark++) {
          s32 speed;
          s32 angle;
          speed = 31;
          speed &= Random16();
          angle = Random16();

          spark->x = centerX + px((Random16() & mask) - 8);
          spark->y = centerY + px((Random16() & mask) - 8);
          speed += 32;
          spark->velocityX = (Trig_Sin(angle) * speed) >> 3;
          spark->velocityY = (Trig_Cos(angle) * speed) >> 3;
          spark->variant = 0;
        }
      }
      {
        struct Particle *mote;

        for (i = 0, mote = sparks; i != DAEDALUS_SPARKS; i++, mote++) {
          if ((u32)mote->variant <= 17) {
            draw[0](canvas, work->sheet + ((mote->variant / 3) << 11),
                    Q_16_16_UPPER_HALF(mote->x) - 16,
                    Q_16_16_UPPER_HALF(mote->y) - 48, 32, 64);
            BattleEffectKernels_IntegrateVector2(mote, 62, -px(1) / 32);
            mote->variant++;
          }
        }
      }
    }

    // The summoned object glides in over 92 frames.
    if (frame <= 91) {
      struct SummonObject *object;

      source = gDaedalusAlt_SummonScale;
      object = work->sprites[0];
      object->offsetY = 32;
      object->offsetX = 0;
      request[3] = 0;
      object->rotation = 0x8800;
      request[1] = 0xff0000;
      if (frame <= 63) {
        request[0] = px(152 - frame / 4);
        request[2] = (frame << 17) - 0x400000;
      } else if ((u32)(frame - 64) <= 63) {
        s32 t = frame - 64;

        request[0] = px(136 - t / 16);
        request[2] = px(t / 2) + px(64);
      } else {
        request[0] = px(136);
        request[2] = 0x400000;
      }
      Render_ApplyProjectedPlacementFar(work->sprites[0], request, &source.x,
                                        0);
    }

    // Frames 56 to 91: the disc and the rings.
    if ((u32)(frame - 56) <= 35) {
      s32 size;
      s32 grow = frame * 2 - 104;
      struct RenderModel *model;
      void *vertices;
      struct Particle *ring;
      u8 *cells;

      /* FAKEMATCH(temporary): the disc's x copied into the loop counter i. */
      i = discX;
      size = 128;
      if (grow <= 64) {
        size = grow * 2;
        cells = gMapGridCellBuffer;
        BattleEffect_DrawShadedCircles(cells, size, 0x10000);
      }
      cells = gMapGridCellBuffer;
      Canvas_BlitAtFoot(cells, i >> 16, discY >> 16, size);
      if (frame == 56) {
        s32 delay;

        i = 0;
        delay = 0;
        for (ring = work->particles; i != 16; i++, ring++) {
          ring->x = Random16() & 0xffff;
          ring->y = (Random16() & 255) + 128;
          if (Random16() & 1) {
            ring->y = -ring->y;
          }
          ring->variant = delay;
          delay -= 4;
        }
        Resource_LoadAndDecompress((s32)&ResourceId_Data0C9,
                                   work->sheet + 0x4000, 0, 0);
      }
      vertices = Heap_AllocEwram(32);
      model = Render_AllocateModelList(1);
      model->shade = 0;
      texture.logWidth = 5;
      texture.logHeight = 2;
      model->vertices = vertices;
      texture.pixels = work->sheet + 0x4000;
      model->kind = 7;
      model->texture = &texture;
      model->mesh = gBattleEffect_SmallSquareMesh;

      for (i = 0; i != 6; i++) {
        ring = &work->particles[i];
        ring->variant += 2;
        ring->x += ring->y * 2;
        if ((u32)ring->variant <= 47) {
          s32 k;

          Transform_Reset();
          k = i & 3;
          SceneTransform_ApplyPosition3((k << 17) + ringX + ringOffset,
                                        discY - px(k + 2) + ringOffset, 0);
          SceneTransform_ApplyScale3(Q16_ONE / 2, Q16_ONE, Q16_ONE);
          Transform_ApplyRoll(ring->x);
          SceneTransform_ApplyScale3(Trig_Sin(ring->variant * 682) + TRIG_TURN,
                                     0x20000, 0x10000);
          Render_TransformVertices(gBattleEffect_TaperedBeamCorners, vertices,
                                   4);
          Render_DrawModelList(model);
        }
      }
      Heap_Release(model);
      Heap_Release(vertices);
    }
    BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(work);
    burst += 8;
    frame++;
  } while (frame != DAEDALUS_FRAMES);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  Sprite_DestroyFar(work->sprites[0]);
  BattleEffect_EndCanvasLayer();
}
#undef BLITTER
#undef BURST_FRAMES
#undef BURST_DRAW_FRAMES
#undef CEL_SIZE
#undef PICTURE_OFFSET
#undef MIRROR_HALF
#undef SYSTEM
#undef IWRAM_COPY_WORDS_LINKED
#undef Iwram_Clear
#undef DECODE_BUFFER
#undef PARTICLES
#undef DAEDALUS_FRAMES
#undef DAEDALUS_SPARKS

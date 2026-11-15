/// @file
/// The Catastrophe summon.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

/// A corner of the swaying strip in the last act, as signed bytes.
struct MeshVertex {
  s8 x;
  s8 y;
  s8 z;
};

/// The VRAM address of an effect sprite's tiles.
#define OBJ_TILES(obj)                                                         \
  (0x06010000 + gVramCacheEntries[((struct SpriteObject *)(obj))->slot].offset)

/// Catastrophe: over a red sky the chained demon in golden armour spreads its
/// wings and drives down a sword of light; the blade's orb falls on the land
/// far below and bursts in a ring and a white flash; then violet lightning
/// and smoke sweep in from the right over the foes. Holding A or B shortens
/// each act.
///
/// Abilities: Catastrophe.
void BattleEffect_RunCatastrophe(struct BattleEffectParams *args) {
  u8 *canvas;
  struct BattleEffectWork *work;
  DrawRectangle blit[2];
  s32 savedScroll;
  s32 *transition;
  u8 *sprites;
  struct BattleWork *battle;
  s32 camZ;
  s32 swapped;
  s32 drift;
  s32 sway;
  s32 cueTimer;
  s32 cueIndex;
  void *verts;
  s32 shade;
  struct RenderModel *model;
  struct RenderModel *aerialModel;
  struct RenderModel *sweepModel;
  s32 frame;
  s32 i;
  s32 j;
  struct Particle *p;
  struct Particle *q;
  struct SpriteObject *obj;
  struct ModelTexture aerialTexture;
  s16 list[14];
  s32 place[4];
  struct ScreenPoint pos;
  struct Scale req;
  struct ModelTexture texture;
  struct ModelTexture sweepTexture;
  struct Particle *p2;
  struct Particle *p3;
  struct Particle *p4;
  struct Particle *q2;
  u16 *src2;
  u8 *e;
  s32 zero;
  u8 *cellBuffer = Ram_MapCellBuffer;
  s32 span = 0x80000;
  s32 rowStep = 8;

  canvas = Ram_HeapSlots->battleCanvas;
  work = Ram_HeapSlots->battleEffect;
  savedScroll = gBgScroll[1].x;
  transition = (s32 *)Ram_HeapSlots->battleBackground;
  sprites = Ram_HeapSlots->battleSheet;
  battle = Ram_HeapSlots->battleWork;
  VramCache_FreeEntry(battle->arrowTiles);
  VramBlock_Reset93Far();
  EFFECT_START_NARROW_CANVAS(0x2000);
  BattleEffect_ConfigureEffectDisplay();
  *(volatile u16 *)BG_PLTT = 0;
  *(volatile u16 *)(BG_PLTT + 1) = 0;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
  blit[0] = Ram_HeapSlots->ramCode;
  work->transferMode = CANVAS_COPY;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  BattleEffect_WipeCanvas(CANVAS_WIPE_DOWN, CANVAS_WIPE_FILL);
  list[0] = 255;
  BattleActor_SpawnObjectsForListFar(list, 0);
  gProjection.centerY = 240;
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  BattleBackground_LoadFar(1, (s32)&ResourceId_RedHazeBackdrop, 0);
  Palette_BrightenSavedBattleBg(-31, -31, -31);
  transition[4] = 1;
  gBgScroll[1].x = 0;
  REG_BG2PA = Q_8_8(0.5);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  {
    u16 *map = (u16 *)BG_SCREEN_BLOCK(31);

    {
      s32 i;

      for (i = 0; i != 30; i++) {
        if (i > 14) {
          {
            s32 j;

            for (j = 0; j != 32; j++) {
              *map++ = i * 32 + (j & 15) + 48;
            }
          }
        } else {
          {
            s32 j;

            for (j = 0; j != 32; j++) {
              *map++ = i * 32 + (j & 15) + 0x200;
            }
          }
        }
      }
    }
  }
  {
    u8 *cells;

    e = Resource_GetTableEntry((s32)&ResourceId_ChainedDemonTiles);
    Iwram_CopyWords((void *)OBJ_PLTT, e, 384);
    e += 384;
    Decompress_TaggedLz(e, cellBuffer);
    cells = Ram_MapCellBuffer;
    for (i = 0; i != 24; cells += 1024, i++) {
      obj = EffectSprite_CreateWithTiles(32, 32, 0x80002000, 0);
      obj->priority = 2;
      work->sprites[i] = obj;
      Iwram_CopyWords((void *)OBJ_TILES(obj), cells, 1024);
    }
    {
      u8 *cells = Data_02016000;

      for (i = 0; i != 4; i++) {
        obj = EffectSprite_Acquire(32, 32, 0x80002000, 0);
        work->sprites[24 + i] = obj;
        Iwram_CopyWords(obj, work->sprites[i], 24);
        Iwram_CopyWords((void *)OBJ_TILES(work->sprites[24 + i]),
                        cells + i * 1024, 1024);
      }
    }
    Iwram_CopyWords(cellBuffer + 0x1000, Data_02016000, 0x1000);
  }
  for (i = 0; i != 6; i++) {
    obj = EffectSprite_Acquire(32, 32, 0x80002000, 0);
    work->sprites[28 + i] = obj;
    Iwram_CopyWords(obj, work->sprites[i], 24);
  }
  {
    u8 *cells;

    e = Resource_GetTableEntry((s32)&ResourceId_GoldenScalesTiles);
    Iwram_CopyWords((void *)(OBJ_PLTT + 0xc0), e, 32);
    e += 32;
    Decompress_TaggedLz(e, work->sheet);
    cells = work->sheet;
    for (i = 0; i != 5; cells += 512, i++) {
      obj = EffectSprite_CreateWithTiles(32, 32, 0x80000000, 0xc000);
      obj->priority = 1;
      work->sprites[34 + i] = obj;
      Iwram_CopyWords((void *)OBJ_TILES(obj), cells, 512);
    }
  }
  {
    u8 *cells;

    e = Resource_GetTableEntry((s32)&ResourceId_PurpleArmourTiles);
    Iwram_CopyWords((void *)(OBJ_PLTT + 0xd0), e, 32);
    e += 32;
    Decompress_TaggedLz(e, work->sheet);
    cells = work->sheet;
    for (i = 0; i != 5; cells += 512, i++) {
      obj = EffectSprite_CreateWithTiles(32, 32, 0x80000000, 0xd000);
      obj->priority = 1;
      work->sprites[39 + i] = obj;
      Iwram_CopyWords((void *)OBJ_TILES(obj), cells, 512);
    }
  }
  {
    u8 *cells;

    e = Resource_GetTableEntry((s32)&ResourceId_WhiteSpikesTiles);
    Iwram_CopyWords((void *)(OBJ_PLTT + 0xe0), e, 32);
    e += 32;
    Decompress_TaggedLz(e, work->sheet);
    cells = work->sheet;
    for (i = 0; i != 6; cells += 512, i++) {
      obj = EffectSprite_CreateWithTiles(32, 32, 0x80000000, 0xe000);
      obj->priority = 1;
      work->sprites[44 + i] = obj;
      Iwram_CopyWords((void *)OBJ_TILES(obj), cells, 512);
    }
  }
  Iwram_CopyWords(work->sheet + 0x5000, (void *)OBJ_PLTT, 480);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0F9, work->sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0F8, work->sheet + 0x2000, 0,
                             0);
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_LightningBoltSheet),
                  128);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sprites, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0B4, Data_02012000, 0, 0);
  zero = 0;
  camZ = 0;
  swapped = 0;
  drift = 0;
  sway = 0;
  cueTimer = 0;
  cueIndex = 0;
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  blit[0] = Ram_HeapSlots->ramCode;
  // Act one: the demon breaks out of its chains and drives down its sword.
  frame = 0;
  while (frame != 637) {
    s32 level;
    s32 d;

    // Fades the screen in over the first frames; a button skips the scene.
    if (frame <= 15) {
      s32 v = frame * 2 - 30;

      Palette_BrightenSavedBattleBg(v, v, v);
    }
    if (gInput.repeat & (KEY_A | KEY_B)) {
      break;
    }
    if (frame == 0) {
      REG_BG2CNT = BGCNT_PRIORITY(2) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
                   BGCNT_SCREEN_BASE(7);
      for (i = 3; i != 64; i++) {
        work->particles[i].variant = -1;
      }
    }
    if (frame == 440) {
      REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
    }
    if (frame <= 369) {
      camZ = (frame * 334 / 379 << 16) + -0x11a0000;
    } else if ((u32)(d = frame - 370) <= 19) {
      camZ = Trig_Sin(d * 1638) * 9 + 0x2b0000;
    } else if ((u32)(frame - 390) <= 95) {
      camZ += -0x8000;
    } else if ((u32)(frame - 486) <= 19) {
      camZ += -0x10000;
    } else if ((u32)(frame - 506) <= 47) {
      s32 v = (frame * 57 << 7) + -0x378eb0;

      if (v > 120000) {
        v = 120000;
      }
      camZ -= v << 1;
    }
    if (frame <= 379) {
      gBgScroll[1].y = 104 - frame * 104 / 379;
    } else {
      s32 v;

      gBgScroll[1].y = v = (frame - 380) / 4;
      if ((u32)px(v) > px(104)) {
        gBgScroll[1].y = 104;
      }
    }
    {
      s32 e2 = frame - 370;
      if ((u32)e2 <= 59) {
        sway = Trig_Sin(e2 * 273 / 2) << 4;
      }
    }
    if ((u32)(frame - 420) <= 51) {
      drift = (420 - frame) << 12;
      sway += -0x4000;
    } else if ((u32)(frame - 472) <= 13) {
      drift = drift - ((frame - 472) << 12) + 120000;
      sway += 75536;
    } else if ((u32)(frame - 486) <= 8) {
      drift += 0x2000;
      sway -= Trig_Sin((frame << 7) + -0xf300);
    } else if ((u32)(frame - 554) <= 29) {
      sway += -0x10000;
    } else if ((u32)(frame - 584) <= 9) {
      sway += -0x80000;
    }
    if (frame == 0) {
      cueTimer = -1;
      cueIndex = 0;
    }
    if (cueIndex <= 17) {
      level = -1;
      if (frame == gCatastrophe_FlashFrames[cueIndex]) {
        cueTimer = gCatastrophe_FlashLengths[cueIndex];
        switch (cueIndex & 3) {
        case 0:
          Sound_PlayFar(SE_TOPPLE);
          break;
        case 1:
          Sound_PlayFar(SE_THUNDER);
          break;
        case 2:
          Sound_PlayFar(SE_BANE_ESCAPE);
          break;
        case 3:
          Sound_PlayFar(SE_THUNDER_LOUD);
          break;
        }
      }
      if (cueTimer >= 0) {
        level = cueTimer * 31 / gCatastrophe_FlashLengths[cueIndex];
        if (--cueTimer == -1) {
          cueIndex++;
        }
      }
      if (level >= 0) {
        u16 *dst = (u16 *)(BG_PLTT + 0x60);
        src2 = battle->palette;

        for (i = 0; i != 128; i++) {
          s32 r = *src2 & 31;
          s32 g = (*src2 >> 5) & 31;
          s32 b = ((*src2 >> 10) & 31) + level;

          if (r > 31) {
            r = 31;
          }
          if (g > 31) {
            g = 31;
          }
          if (b > 31) {
            b = 31;
          }
          src2++;
          *dst++ = b << 10 | g << 5 | r;
        }
      }
    }
    {
      s32 fr;
      s32 fg;
      s32 fb;
      s32 apply[2] = {0, 0};

      fr = 0;
      fg = 0;
      fb = 0;
      if (frame == 0) {
        fg = -31;
        fr = fg;
        fb = fg;
        apply[0] = 1;
      } else if (frame == 106) {
        fg = -31;
        fr = fg;
        fb = -15;
        apply[0] = 1;
      } else if ((u32)(frame - 154) <= 59) {
        s32 h = (frame - 154) / 2;

        fg = 5 - h * 2;
        fr = fg;
        fb = 5 - h;
        apply[0] = 1;
      } else if ((u32)(frame - 226) <= 15) {
        fg = (frame - 226) / 2 - 31;
        fr = fg;
        fb = frame - 257;
        apply[0] = 1;
      } else if (frame == 242) {
        fr = -24;
        fg = -31;
        apply[0] = 1;
      } else if ((u32)(frame - 256) <= 21) {
        s32 d = frame - 256;

        fr = -24 - d * 2;
        fg = -31 - d * 2;
        fb = -d;
        apply[0] = 1;
      } else if ((u32)(frame - 282) <= 40) {
        s32 d = frame - 282;

        fg = -d;
        fr = fg;
        fb = 16 - d;
        apply[0] = 1;
      } else if (frame == 324) {
        fr = 31;
        fg = 31;
        fb = 31;
        apply[0] = 1;
        apply[1] = 1;
      } else if ((u32)(frame - 332) <= 59) {
        fg = 30 - (frame - 332) / 2;
        apply[0] = 1;
        fr = fg;
        fb = fg;
        apply[1] = 1;
      } else if ((u32)(frame - 489) <= 42) {
        s32 d = frame - 489;

        fr = d / 4;
        fg = d / 6;
        fb = d / 16;
        apply[0] = 1;
      }
      for (j = 0; j != 2; j++) {
        if (apply[j] != 0) {
          u16 *src;
          u16 *dst;
          s32 n;

          if (j == 0) {
            src = (u16 *)(work->sheet + 0x5002);
            n = 240;
            dst = (u16 *)(OBJ_PLTT + 1);
          } else {
            dst = (u16 *)(BG_PLTT + 0x60);
            src = battle->palette;
            n = 128;
          }
          for (i = 1; i != n; i++) {
            s32 r = (*src & 31) + fr;
            s32 g = ((*src >> 5) & 31) + fg;
            s32 b = ((*src >> 10) & 31) + fb;

            if (r > 31) {
              r = 31;
            }
            if (g > 31) {
              g = 31;
            }
            if (b > 31) {
              b = 31;
            }
            if (r < 0) {
              r = 0;
            }
            if (g < 0) {
              g = 0;
            }
            if (b < 0) {
              b = 0;
            }
            *dst++ = b << 10 | g << 5 | r;
            src++;
          }
        }
      }
    }
    if ((u32)(frame - 554) <= 31 && !((frame - 554) & 1)) {
      ColorBuffer_ScaleFar(battle->palette, (void *)(BG_PLTT + 0x60),
                           0x10000 - ((frame - 554) << 10), 128);
    }
    if (swapped == 0) {
      if (frame == 339) {
        swapped = 1;
        for (i = 0; i != 4; i++) {
          void *tiles = (void *)OBJ_TILES(work->sprites[24 + i]);

          Iwram_CopyWords(tiles, cellBuffer + i * 1024, 1024);
        }
      }
    } else if (frame == 450) {
      u8 *cells;

      swapped = 0;
      cells = Data_02011000;
      for (i = 0; i != 4; i++) {
        Iwram_CopyWords((void *)OBJ_TILES(work->sprites[24 + i]),
                        cells + i * 1024, 1024);
      }
    }
    if (frame <= 593) {
      req = gCatastrophe_UnitScale;
      place[3] = 0;
      place[1] = px(255);
      {

        for (i = 0; i != 28; i++) {
          place[0] = px(gCatastrophe_DemonX[i]) + px(8);
          place[2] = px(gCatastrophe_DemonZ[i]) + camZ;
          if (place[2] > 0xfffff) {
            if (place[2] <= px(168)) {
              Render_ApplyProjectedPlacementFar(work->sprites[i], place,
                                                (s32 *)&req, 0);
            }
          }
        }
      }
      for (i = 0; i != 5; i++) {
        s32 h = i / 2;

        place[0] = ((i - h * 2) << 21) + drift + px(72);
        place[2] = (h << 21) + camZ + sway + px(60);
        if (place[2] > 0xfffff) {
          if (place[2] <= px(168)) {
            Render_ApplyProjectedPlacementFar(work->sprites[34 + i], place,
                                              (s32 *)&req, 0);
          }
        }
      }
      for (i = 0; i != 11; i++) {
        place[0] = px(gCatastrophe_ArmourX[i]) - drift + px(132);
        place[2] = px(gCatastrophe_ArmourZ[i]) + camZ + sway + px(29);
        if (place[2] > 0xfffff) {
          if (place[2] <= px(168)) {
            Render_ApplyProjectedPlacementFar(work->sprites[39 + i], place,
                                              (s32 *)&req, 0);
          }
        }
      }
      req.x = -0x10001;
      for (i = 0; i != 6; i++) {
        place[0] = px(gCatastrophe_MirroredDemonX[i]) + px(8);
        place[2] = px(gCatastrophe_MirroredDemonZ[i]) + camZ;
        if (place[2] > 0xfffff) {
          if (place[2] <= px(168)) {
            Render_ApplyProjectedPlacementFar(work->sprites[28 + i], place,
                                              (s32 *)&req, 0);
          }
        }
      }
    }
    verts = Heap_AllocEwram(512);
    model = Render_AllocateModelList(1);
    texture.logWidth = 5;
    shade = 0;
    texture.logHeight = 6;
    model->mesh = gCatastrophe_BurstMesh;
    model->texture = &texture;
    model->kind = 6;
    model->vertices = verts;
    for (i = 0; i != 7; i++) {
      s32 st = gCatastrophe_BurstStarts[i];

      if (frame >= st && frame < st + 8) {
        s32 c = gCatastrophe_BurstPictures[i];

        shade = (st - frame) << 3;
        model->shade = shade;
        texture.pixels = work->sheet + c * 0x600;
        Transform_Reset();
        SceneTransform_ApplyPosition3(px(gCatastrophe_BurstX[i] - 48),
                                      px(gCatastrophe_BurstY[i] - 44), 0);
        Transform_ApplyRoll(0);
        SceneTransform_ApplyScale3(Q16_ONE / 2, Q16_ONE, Q16_ONE / 2);
        Render_TransformVertices(gBattleEffect_SquareCorners, verts, 4);
        Render_DrawModelList(model);
      }
    }
    if (frame == 489) {
      for (i = 0; i != 3; i++) {
        work->particles[i].variant = 0;
      }
    }
    for (j = 3; j != 64; j++) {
      p = &work->particles[j];
      if (p->variant >= 0) {
        s32 n = (p->variant >> 3) + 1;

        blit[0](canvas, sprites + gBattleEffect_MotePictures[n - 1],
                Q_16_16_UPPER_HALF(p->x) - n / 2, Q_16_16_UPPER_HALF(p->y) - n,
                n, n * 2);
        BattleEffectKernels_IntegrateVector2(p, 63, 0);
        p->variant--;
      }
    }
    {
      for (i = 0; i != 17; i++) {
        u16 t = gCatastrophe_CanvasFlashFrames[i];

        if (frame == t + 242) {
          EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR,
                              CANVAS_FILL_WORD(31));
        }
        if (frame == t + 243) {
          EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
        }
      }
    }
    if ((u32)(frame - 537) <= 55) {
      if (frame & 1) {
        EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, CANVAS_FILL_WORD(31));
      } else {
        EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
      }
    }
    if (frame == 324) {
      Sound_PlayFar(SE_CURSE_FALL);
    }
    if (frame == 560) {
      Sound_PlayFar(SE_SHIMMER);
    }
    if (frame == 602) {
      Sound_PlayFar(SE_CRASH);
    }
    texture.logWidth = 5;
    texture.logHeight = 7;
    model->shade = shade;
    model->kind = 6;
    model->mesh = gBattleEffect_ColumnMesh;
    model->texture = &texture;
    model->vertices = verts;
    if ((u32)(frame - 489) <= 130) {
      shade = 0;
      for (i = 0; i != 3; i++) {
        s32 off;
        s32 idx;

        p = &work->particles[i];
        off = i * 12;
        idx = p->variant;

        if (frame ==
            (gCatastrophe_StrikeFrames[i * 13 + idx] - 492) / 2 + 489) {
          s32 count = 0;
          const s16 *table = gCatastrophe_StrikeRolls;
          s32 n = off + idx;
          s32 k = n * 2;
          s32 a = *(const s16 *)((s32)table + k);
          u8 *src;
          s32 bx;
          s32 h;
          s32 w;
          s32 x;
          s32 x0;
          s32 y0;
          s32 y;

          if (a != 0) {
            SET_POSITION(p, gCatastrophe_StrikeX[n] << 3,
                         gCatastrophe_StrikeY[n] << 3, a << 8);
          }
          /* FAKEMATCH(barrier): idx held through an empty asm. */
          __asm__("" : "=r"(idx) : "0"(idx));
          p->velocityY = gCatastrophe_StrikeSpeedsY[idx + off];
          p->velocityZ = gCatastrophe_StrikeTurns[idx + off];
          p->velocityX = gCatastrophe_StrikePictures[idx + off];
          p->variant = idx + 1;
          if (p->velocityX == -1) {
            continue;
          }
          w = 10;
          h = 20;
          src = sprites + gBattleEffect_MotePictures[9];
          x0 = p->x / 8 - 5;
          bx = span - drift;
          x = x0 + (bx / 2 >> 16);
          y0 = p->y / 8 - 10;
          y = y0 + ((camZ + sway) >> 16);
          blit[0](canvas, src, x, y, w, h);
          switch (i) {
          case 0:
            Sound_PlayFar(SE_SOL_SANCTUM_LIGHT_UP);
            break;
          case 1:
            Sound_PlayFar(SE_FALLING_LIGHT);
            break;
          case 2:
            Sound_PlayFar(SE_WHIRLWIND);
            break;
          }
          {
            s32 half;
            s32 j;

            j = 3;
            q = &work->particles[3];
            half = (bx + (s32)((u32)bx >> 31)) >> 1;
            for (; j != 64; j++, q++) {
              if (q->variant < 0) {
                s32 angle;
                s32 speed;

                q->x = (p->x << 13) + half;
                q->y = (p->y << 13) + camZ + sway;
                angle = Random16();
                speed = (Random16() & 255) + 127;
                q->velocityX = Trig_Sin(angle) * speed >> 6;
                q->velocityY = Trig_Cos(angle) * speed >> 6;
                q->variant = (Random16() & 7) + 16;
                if (++count == 10) {
                  break;
                }
              }
            }
          }
        }
        if (p->velocityX != -1) {
          s32 cell = p->velocityX;

          model->shade = shade;
          texture.pixels = work->sheet + 0x2000 + (cell << 12);
          Transform_Reset();
          {
            s32 t = p->x / 8;
            s32 b = span - drift;

            SceneTransform_ApplyPosition3(px(t - 64) + b / 2,
                                          px(p->y / 8 - 64) + camZ + sway, 0);
          }
          SceneTransform_ApplyScale3(Q16_ONE / 2, Q16_ONE, Q16_ONE);
          Transform_ApplyRoll(p->z);
          SceneTransform_ApplyScale3(Q16_ONE * 4, 85536, Q16_ONE / 2);
          Render_TransformVertices(gBattleEffect_LongBeamCorners, verts, 4);
          Render_DrawModelList(model);
          p->y += p->velocityY * 2;
          p->z += p->velocityZ * 2;
        }
      }
    }
    if (frame == 602) {
      Iwram_FillWordsEntry(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(63));
    }
    if (frame == 630) {
      Iwram_FillWordsEntry(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(31));
    }
    if (frame == 634) {
      Iwram_FillWordsEntry(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(31));
    }
    if ((u32)(frame - 552) <= 43) {
      p = work->particles;
      if (!(frame & 1)) {
        p->y = (Random16() & 0x3fff) + 0x6000;
        p->z = (Random16() % 3 << 12) + 0x2000;
      }
      texture.pixels = work->sheet + p->z;
      Transform_Reset();
      SceneTransform_ApplyPosition3((span - drift) / 2 + px(4), camZ + 0xcc0000,
                                    0);
      SceneTransform_ApplyScale3(Q16_ONE / 2, Q16_ONE, Q16_ONE);
      Transform_ApplyRoll(p->y);
      SceneTransform_ApplyScale3(Q16_ONE * 4, 85536, Q16_ONE / 2);
      Render_TransformVertices(gBattleEffect_LongBeamCorners, verts, 4);
      Render_DrawModelList(model);
    }
    if ((u32)(frame - 602) <= 15 || (u32)(frame - 630) <= 3) {
      s32 n = 1;
      s32 sx = 0x50000;
      s32 base = Random16() % 3;
      s32 spread = 8;

      if (frame > 611) {
        sx = sx - frame * 61845 + 0x2418834;
      }
      if (sx <= 65535) {
        sx = 0x10000;
      }
      if (frame <= 613) {
        if (frame > 605) {
          n = 3;
        } else if (frame > 603) {
          n = 2;
        }
      }
      model->kind = 6;
      model->shade = shade;
      for (i = 0; i != n; i++) {
        struct Particle *p4 = &work->particles[i];

        SET_POSITION(p4, px(Random16() % spread - spread / 2), camZ + 0xc80000,
                     ((base + i) % 3 << 12) + 0x2000);
        texture.pixels = work->sheet + p4->z;
        Transform_Reset();
        SceneTransform_ApplyPosition3(px(1), p4->y, 0);
        SceneTransform_ApplyScale3(sx, Q16_ONE, Q16_ONE / 2);
        SceneTransform_ApplyPosition3(p4->x, 0, 0);
        SceneTransform_ApplyScale3(Q16_ONE / 2, Q16_ONE * 2, Q16_ONE);
        Transform_ApplyRoll(TRIG_TURN / 2);
        Render_TransformVertices(gBattleEffect_LongBeamCorners, verts, 4);
        Render_DrawModelList(model);
      }
    }
    if (frame == 552) {
      Resource_LoadAndDecompress((s32)&ResourceId_Data0B4, Data_02012000, 0, 0);
    }
    if (frame == 551) {
      Resource_LoadAndDecompress((s32)&ResourceId_Data0EE, work->sheet, 0, 0);
      Resource_LoadAndDecompress((s32)&ResourceId_Data0EF, work->sheet + 0x1000,
                                 0, 0);
    }
    if (frame == 602) {
      Resource_LoadAndDecompress((s32)&ResourceId_Data0C2, Data_02012000, 0, 0);
    }
    if ((u32)(frame - 602) <= 6) {
      camZ += 0x100000;
    }
    {
      s32 d = frame - 552;

      if ((u32)d <= 58) {
        s32 s = d * 4424;
        s32 a;
        s32 b;

        shade = -16;
        if (s > 0x22538) {
          s = 0x22538;
        }
        texture.logWidth = 6;
        texture.logHeight = 6;
        model->shade = shade;
        model->kind = 7;
        model->texture = &texture;
        model->mesh = gBattleEffect_SquareMesh;
        model->vertices = verts;
        texture.pixels = work->sheet + 0x1000;
        Transform_Reset();
        b = span - drift;
        SceneTransform_ApplyPosition3(b / 2 + px(5), camZ + px(230), 0);
        SceneTransform_ApplyScale3(Q16_ONE / 2, Q16_ONE, Q16_ONE / 2);
        Transform_ApplyPitch(18768);
        a = -frame;
        a <<= 10;
        Transform_ApplyRoll(a);
        SceneTransform_ApplyUniformScale(s);
        SceneTransform_ApplyPosition3(0, -px(15), 0);
        Render_TransformVertices(gBattleEffect_SquareCorners, verts, 4);
        SceneTransform_ApplyPosition3(0, px(15), 0);
        Render_DrawModelList(model);
        texture.pixels = work->sheet;
        for (i = 0; i != 3; i++) {
          Transform_Push();
          Transform_ApplyRoll(i * 21845 + 10922);
          {
            s32 phase = i * 21845 + 10922;
            model->shade = -24 - (Trig_Cos(phase + a) << 3 >> 16);
          }
          SceneTransform_ApplyPosition3(0, -px(16), 0);
          if (frame > 579) {
            s32 v = (frame - 580) * 1096;

            if (v > 19712) {
              v = 19712;
            }
            Transform_ApplyPitch(-v);
          }
          Render_TransformVertices(gBattleEffect_StandingSquareCorners, verts,
                                   4);
          Render_DrawModelList(model);
          Transform_Pop();
        }
        if (frame > 589) {
          s = s + frame * 11060 + -0x6391d8;
        }
        if (s > 0 && frame <= 601) {
          shade = -16;
          if (frame > 589) {
            shade = shade - frame * 2 + 1180;
          }
          if (shade > -64) {
            model->shade = shade;
            texture.pixels = Data_02012000;
            Transform_Reset();
            if (frame > 553) {
              s32 v = (554 - frame) * 120000;

              if (v < -0xcd140) {
                v = -0xcd140;
              }
              SceneTransform_ApplyPosition3(0, v, 0);
            }
            SceneTransform_ApplyPosition3(
                ((b + (s32)((u32)b >> 31)) >> 1) + 0x50000, camZ + 0xdc0000, 0);
            SceneTransform_ApplyScale3(Q16_ONE, 32000, Q16_ONE);
            Transform_ApplyRoll(frame << 11);
            SceneTransform_ApplyUniformScale(s);
            Render_TransformVertices(gBattleEffect_SquareCorners, verts, 4);
            Render_DrawModelList(model);
          }
        }
      }
    }
    if (frame > 601) {
      s32 s = (frame - 602) * 12192 + 0x2000;

      shade = 0;
      if (frame > 611) {
        shade = shade - frame * 8 + 4896;
      }
      if (shade > -64) {
        model->shade = shade;
        model->mesh = gBattleEffect_LargeSquareMesh;
        model->kind = 7;
        texture.pixels = Data_02012000;
        texture.logWidth = 7;
        texture.logHeight = 7;
        Transform_Reset();
        SceneTransform_ApplyPosition3(px(1), -px(16), 0);
        SceneTransform_ApplyScale3(Q16_ONE, 32000, Q16_ONE);
        Transform_ApplyRoll(frame << 10);
        SceneTransform_ApplyUniformScale(s);
        Render_TransformVertices(gBattleEffect_SquareCorners, verts, 4);
        Render_DrawModelList(model);
      }
    }
    Heap_Release(model);
    Heap_Release(verts);
    if ((u32)(frame - 554) <= 51) {
      s32 r = frame - 553;
      s32 y = 90 - (frame - 554) / 2;

      if (frame > 600) {
        y = y + (frame << 4) - 9616;
      }
      if (y <= 63) {
        y = 64;
      }
      if (r <= 38) {
        BattleEffect_DrawShadedCircles(cellBuffer, r, 95536);
      } else {
        r = 38;
      }
      Canvas_BlitAtFoot(cellBuffer, 62, y, r);
    }
    EFFECT_PRESENT_CANVAS(work);
    frame++;
  }
  for (i = 0; i != 50; i++) {
    Sprite_DestroyFar(work->sprites[i]);
  }
  VramBlock_Load93Far();
  battle->arrowTiles = VramCache_AllocEntry(128);
  BattleEffect_SelectLivingTargets(work->args);
  for (i = 0; i != args->targetCount; i++) {
    BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_GRB, TINT_KEEP,
                             TINT_KEEP, 0);
    BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_NORMAL, TINT_KEEP,
                             TINT_KEEP, 0);
  }
  Palette_LoadBasicSpriteColors();
  BattleEffect_SetupBlendedDisplay();
  gProjection.centerY = 240;
  work->fadeFrames = 0;
  work->fadeStep = 0;
  // Act two: the land seen from the sky, where the orb falls and bursts.
  gBgScroll[1].y = 32;
  BattleBackground_LoadFar(1, (s32)&ResourceId_AerialViewBackdrop, 0);
  Palette_BrightenSavedBattleBg(0, -16, -16);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  Resource_LoadAndDecompress((s32)&ResourceId_LightningBoltSheet, cellBuffer, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, work->sheet + 0x800,
                             1, 0);
  Iwram_ClearWordsEntry(work->sheet + 0x1456, 0x4000);
  for (i = 0; i != 4; i++) {
    {
      s32 j;
      s32 row = i << 12;
      u8 *cells = cellBuffer;

      for (j = 0; j != 120; j++) {
        u8 *line = (u8 *)work + (row + j * 32);
        s32 offset = i * 45 * 64 + j * 24;
        u8 *dst = line + 0x1b5a;
        u8 *src = cells + offset;
        s32 m;

        for (m = 0; m != 24; m++) {
          *dst++ = *src++;
        }
      }
    }
  }
  Resource_LoadAndDecompress((s32)&ResourceId_Data0EE, cellBuffer, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0EF, Data_02011000, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C2, Data_02012000, 0, 0);
  {
    s32 rise;
    s32 top;
    void *verts2;
    s32 sh2;
    s32 r;
    s32 ry;

    rise = 0;
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
    frame = 0;
    if (!(gInput.repeat & (KEY_A | KEY_B))) {
      do {
        if (frame == 0) {
          Iwram_FillWordsEntry(canvas, CANVAS_BYTES, 0);
          rise = -0x400000;
          Palette_BrightenSavedBattleBg(0, -16, -16);
          EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
          Iwram_CopyWords(
              (void *)BG_PLTT,
              Resource_GetTableEntry((s32)&ResourceId_EffectPalette02), 128);
        }
        if (rise <= 0xfffff) {
          rise += 0x10000;
        }
        {
          s32 d = frame - 96;

          if ((u32)d <= 23) {
            Palette_BrightenSavedBattleBg(-d, -16, frame - 112);
            work->shakeFrames = 8;
          }
        }
        if (frame > 139) {
          work->transferMode = CANVAS_COPY_AND_CLEAR;
          if (frame > 153) {
            work->transferValue = CANVAS_FILL_WORD(63);
          } else {
            work->transferValue = CANVAS_FILL_WORD(16);
          }
          Palette_BrightenBgEntries(1, 1, 1);
        }
        if (frame == 80 || frame == 83) {
          u16 *c = (u16 *)(BG_PLTT + 0x60);

          for (i = 0; i != 128; i++, c++) {
            *c ^= 0x7fff;
          }
        }
        if (frame == 82) {
          work->shakeFrames = 8;
          Sound_PlayFar(SE_BANE_ESCAPE);
          IWRAM_FILL_WORDS(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(63));
        }
        if (frame == 96) {
          Sound_PlayFar(SE_104);
        }
        if (frame == 128) {
          u16 *c;

          work->shakeFrames = 16;
          c = (u16 *)(BG_PLTT + 0x60);
          for (i = 0; i != 128; i++, c++) {
            *c ^= 0x7fff;
          }
        }
        if (frame > 128) {
          s32 d = (frame - 128) << 1;

          Palette_BrightenSavedBattleBg(d - 11, frame - 136, 3 - d);
        }
        r = 16;
        ry = rise >> 16;
        if (frame > 92) {
          r = frame - 75;
        }
        if (frame <= 79) {
          r += Trig_Sin(frame << 11) * 2 >> 16;
        }
        if (r <= 64) {
          BattleEffect_DrawShadedCircles(work->sheet, r, 95536);
        } else {
          r = 64;
        }
        top = ry - r / 2 + 60;
        Canvas_BlitAtFoot(work->sheet, 60, top, r);
        if (frame > 95) {
          {
            for (i = 0; i != 4; i++) {
              s32 n = i & 3;
              s32 angle = Random16();
              s32 x = (Trig_Sin(angle) * r >> 17) + 60;
              s32 y;

              x -= gBattleEffect_GlintPictureWidths[n] >> 1;
              y = top + (Trig_Cos(angle) * r >> 17) -
                  (gBattleEffect_GlintPictureHeights[n] >> 1);

              {
                s32 c = gCatastrophe_GlintFlips[Random16() & 3] | 3;

                BattleEffect_BuildBlitter(HEAP_SLOT_RAM_CODE_2, 7, 7, c, 0);
              }
              {
                s32 at = gBattleEffect_GlintPictures[n];

                /* FAKEMATCH(dead): an empty once-only block between the
                 * picture's offset and the draw. */
                do {
                } while (0);
                ((DrawRectangle)Ram_HeapSlots->ramCode2)(
                    canvas, work->sheet + 0x800 + at, x, y,
                    gBattleEffect_GlintPictureWidths[n],
                    gBattleEffect_GlintPictureHeights[n]);
              }
              HeapSlot_Release(HEAP_SLOT_RAM_CODE_2);
            }
          }
        }
        verts2 = Heap_AllocEwram(512);
        aerialModel = Render_AllocateModelList(1);
        if (frame <= 127) {
          s32 s = 85536;
          s32 a;

          if (frame > 71) {
            s = (frame << 11) + -0xf1e0;
          }
          sh2 = -16;
          if (frame > 119) {
            sh2 = 944 - frame * 8;
          }
          if (s > 0x22538) {
            s = 0x22538;
          }
          aerialTexture.logWidth = 6;
          aerialTexture.logHeight = 6;
          aerialModel->shade = sh2;
          aerialModel->kind = 6;
          aerialModel->texture = &aerialTexture;
          aerialModel->mesh = gBattleEffect_SquareMesh;
          aerialModel->vertices = verts2;
          aerialTexture.pixels = Data_02011000;
          Transform_Reset();
          SceneTransform_ApplyPosition3(-px(2), rise, 0);
          SceneTransform_ApplyScale3(Q16_ONE / 2, Q16_ONE, Q16_ONE / 2);
          Transform_ApplyPitch(18768);
          a = frame << 9;
          Transform_ApplyRoll(a);
          SceneTransform_ApplyUniformScale(s);
          SceneTransform_ApplyPosition3(0, -px(15), 0);
          Render_TransformVertices(gBattleEffect_SquareCorners, verts2, 4);
          SceneTransform_ApplyPosition3(0, px(15), 0);
          Render_DrawModelList(aerialModel);
          aerialTexture.pixels = cellBuffer;
          for (i = 0; i != 3; i++) {
            s32 k = i * 21845 + 10922;
            s32 tone = sh2 + (Trig_Cos(a + k) << 3 >> 16) - 16;
            s32 v;

            Transform_Push();
            Transform_ApplyRoll(i * 21845 + 10922);
            aerialModel->shade = tone;
            SceneTransform_ApplyPosition3(0, -px(16), 0);
            v = 19712;
            if (frame > 63) {
              v = v - (frame << 10) + 0x10000;
            }
            if (v < 0) {
              v = 0;
            }
            Transform_ApplyPitch(-v);
            Render_TransformVertices(gBattleEffect_StandingSquareCorners,
                                     verts2, 4);
            Render_DrawModelList(aerialModel);
            Transform_Pop();
          }
        }
        {
          s32 t = frame * 3;

          {
            for (i = 0; i != 2; i++) {
              s32 d = i * 8;
              s32 start = d + 80;
              s32 s;

              if (frame < start) {
                continue;
              }
              sh2 = 0;
              if (i == 0) {
                s = (t - start * 3) * 127 * 32 + 0x2000;
                if (frame >= d + 96) {
                  sh2 = (start - frame) * 8 + 128;
                }
              } else {
                s = (t - start * 3) * 127 * 16 + 0x2000;
                if (frame >= d + 112) {
                  sh2 = (start - frame) * 8 + 256;
                }
              }
              if (sh2 > -64) {
                aerialModel->shade = sh2;
                aerialModel->mesh = gBattleEffect_LargeSquareMesh;
                aerialModel->kind = 6;
                aerialTexture.pixels = Data_02012000;
                aerialTexture.logWidth = 7;
                aerialTexture.logHeight = 7;
                Transform_Reset();
                SceneTransform_ApplyPosition3(0, px(16), 0);
                SceneTransform_ApplyScale3(Q16_ONE, 32000, Q16_ONE);
                Transform_ApplyRoll(frame << 10);
                SceneTransform_ApplyUniformScale(s);
                Render_TransformVertices(gBattleEffect_SquareCorners, verts2,
                                         4);
                Render_DrawModelList(aerialModel);
              }
            }
          }
        }
        aerialTexture.logWidth = 5;
        aerialTexture.logHeight = 7;
        aerialModel->kind = 6;
        aerialModel->mesh = gBattleEffect_ColumnMesh;
        aerialModel->shade = zero;
        if (frame == 0) {
          p = work->particles;
          for (i = 0; i != 16; p++, i++) {
            p->z = gCatastrophe_ShardRolls[i];
            p->velocityZ = (Random16() & 255) + 128;
            if (Random16() & 1) {
              p->velocityZ = -p->velocityZ;
            }
            p->variant = Random16() & 3;
          }
        }
        for (i = 0; i != 8; i++) {
          if (frame == 128 + i * 4) {
            Sound_PlayFar(SE_CLATTER);
          }
        }
        p = work->particles;
        for (i = 0; i != 16; i++, p++) {
          s32 st = i + 128;

          if (frame >= st && frame < i + 144) {
            s32 s = (frame - st) << 13;

            if (s > 0x10000) {
              s = 0x10000;
            }
            aerialTexture.pixels = work->sheet + 0x1456 + (p->variant << 12);
            Transform_Reset();
            Transform_ApplyRoll(p->z);
            p->z += p->velocityZ;
            SceneTransform_ApplyScale3(s << 2, s, Q16_ONE);
            Render_TransformVertices(gBattleEffect_TaperedBeamCorners, verts2,
                                     4);
            Render_DrawModelList(aerialModel);
          }
        }
        Heap_Release(aerialModel);
        Heap_Release(verts2);
        if (frame > 133) {
          if (frame == 134) {
            struct Particle *p;

            i = 0;
            p = &work->particles[16];
            for (; i != 48; i++, p++) {
              s32 a = Random16();
              s32 angle = i * 1365;
              s32 rr = (a & 31) + 32;

              SET_POSITION(p, (Trig_Sin(angle) * rr >> 16) + 60,
                           (Trig_Cos(angle) * rr >> 16) + 60,
                           (Random16() & 3) + 5);
              p->variant = -(Random16() & 15);
            }
          }
          i = 0;
          p3 = &work->particles[16];
          for (; i != 48; i++, p3++) {
            if ((u32)p3->variant <= 7) {
              s32 n = (Trig_Sin(p3->variant << 12) * p3->z >> 16) + 1;

              blit[0](canvas, sprites + gBattleEffect_MotePictures[n - 1],
                      p3->x - n / 2, p3->y - n, n, n * 2);
            }
            p3->variant++;
          }
        }
        if (work->shakeFrames > 0) {
          s32 m = 7;
          s32 a = Random16();
          s32 b = Random16();

          a &= m;
          b &= m;
          a -= 4;
          b += 28;
          gBgScroll[1].x = a;
          gBgScroll[1].y = b;
          work->shakeFrames--;
        } else {
          gBgScroll[1].x = 0;
          gBgScroll[1].y = work->savedBg1Y;
        }
        EFFECT_PRESENT_CANVAS(work);
        frame++;
      } while (frame != 160 && !(gInput.repeat & (KEY_A | KEY_B)));
    }
  }
  {
    s32 slide;
    s32 slideStep;
    u8 *bolts;
    void *quads = Data_02011000;
    void *strip = Ram_MapCellBuffer + 0x2000;
    struct Particle *steps = Data_02016d00;
    struct Particle *debris = &Data_02016d00[64];

    // Act three: back on the battlefield, lightning and smoke sweep the foes.
    bolts = Ram_MapCellBuffer + 0x4000;
    slide = 0;
    slideStep = 0;
    Iwram_FillWords(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(63));
    EFFECT_PRESENT_CANVAS(work);
    gBgScroll[1].x = savedScroll;
    BattleBackground_LoadFar(1, battle->background, 24);
    Palette_BrightenSavedBattleBg(-16, -16, -4);
    Resource_LoadAndDecompress((s32)&ResourceId_SmokeBandSheet, work->sheet, 1,
                               1);
    Resource_LoadAndDecompress((s32)&ResourceId_ClawSlashSheet,
                               work->sheet + 0x800, 1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sprites, 0,
                               0);
    Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                               work->sheet + 0x1800, 1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_LightningBoltSheet, bolts, 1,
                               0);
    Render_BuildQuadStrip(Data_02012000, 4, 32, 11);
    REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
    frame = 0;
    do {
      if ((gInput.repeat & (KEY_A | KEY_B)) && frame > 2 && frame <= 87) {
        frame = 88;
      }
      if (frame == 88) {
        Iwram_FillWordsEntry(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(63));
        EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(2));
        gBgScroll[1].x = savedScroll;
        gProjection.centerX = 120;
        work->scrollStepX = 0;
        EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      }
      if (frame == 0) {
        IWRAM_FILL_WORDS(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(63));
        work->scrollStepX = -16;
        gProjection.centerX = -968;
        gProjection.centerY = 120;
        gBgScroll[1].x = savedScroll + 1088;
        Graphics_LoadGreyRamp();
        EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(2));
        for (i = 0; i != 164; i++) {
          ((struct Particle *)(Ram_MapCellBuffer + 0x6d00))[i].variant = -1;
        }
        slide = 0x800000;
        slideStep = 0;
      }
      if (frame == 16) {
        EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE,
                            CANVAS_FADE_THREE_QUARTERS);
      }
      if ((u32)(frame - 16) <= 47) {
        BattleEffect_StepPaletteToResource((s32)&ResourceId_EffectPalette02);
      }
      if (frame == 66) {
        for (i = 0; i != args->targetCount; i++) {
          BattleEffect_StartActorMotion(args->targets[i], 1, 0x40000, 0x80000,
                                        0x40000, 150);
          BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, i, 16);
          {
            s32 a = args->targets[i];
            s32 k = 448;
            EffectPosition_ProjectBattler(a, &pos);
            j = 0;
            q = (struct Particle *)(i * k + (s32)debris);
          }
          for (; j != 16; j++, q++) {
            s32 angle;
            s32 speed;

            q->x = px(pos.x / 2);
            q->y = px(80);
            angle = (Random16() & 0x7fff) + TRIG_TURN / 4;
            speed = (Random16() & 127) + 128;
            q->velocityX = Trig_Sin(angle) * speed >> 5;
            q->velocityY = Trig_Cos(angle) * speed >> 4;
            q->variant = 16;
          }
        }
        work->shakeFrames = 16;
        Sound_PlayFar(SE_CRASH);
      }
      if ((u32)(frame - 72) <= 15) {
        EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_BRIGHTEN,
                            CANVAS_FILL_WORD(1));
        Palette_BrightenBgEntries(1, 1, 1);
      }
      if (frame >= 0) {
        s32 x = 0;

        if (frame > 71) {
          x = x - frame * 8 + 576;
        }
        for (i = 0; i != 2; i++) {
          s32 sum = x + 116;
          struct Particle *base =
              (struct Particle *)((frame & 31) * 56 + (s32)steps);
          struct Particle *q = &base[i];
          s32 angle;
          s32 speed;

          q->x = px(sum);
          q->y = px(Random16() % 112 + 4);
          angle = (Random16() & 0x7fff) + TRIG_TURN / 2;
          speed = (Random16() & 127) + 128;
          q->velocityX = Trig_Sin(angle) * speed >> 6;
          q->velocityY = Trig_Cos(angle) * speed >> 6;
          q->variant = 0;
        }
        q2 = Data_02016d00;
        for (i = 0; i != 64; i++, q2++) {
          if ((u32)q2->variant <= 11) {
            blit[0](canvas, work->sheet + 0x1800 + (q2->variant / 2 << 11),
                    Q_16_16_UPPER_HALF(q2->x) - 16,
                    Q_16_16_UPPER_HALF(q2->y) - 32, 32, 64);
            BattleEffectKernels_IntegrateVector2(q2, 63, 0);
            q2->variant++;
          }
        }
      }
      if (frame <= 87) {
        s32 off;
        s32 jitter = 0;
        s32 z;
        s32 y;
        s32 a;
        s32 b;
        s32 c;

        j = 0;
        z = 0;
        y = 0;
        for (; j != 12; j++) {
          struct MeshVertex *v;
          s32 x;

          off = j * rowStep;

          for (i = 0, x = 0, v = (struct MeshVertex *)(off + (s32)cellBuffer);
               i != 2; i++, x += 10, v++) {
            SET_POSITION(
                v, x + (Trig_Sin((frame + j * 2) << 10) * 2 >> 16) + jitter - 5,
                y, z);
          }
          jitter = jitter + (Random16() & 7) - 4;
          y -= 6;
        }
        sweepModel = Render_AllocateModelList(1);
        sweepModel->kind = 6;
        sweepModel->texture = &sweepTexture;
        sweepModel->shade = 0;
        sweepModel->scrollV = 0;
        sweepModel->vertices = quads;
        if (frame == 16) {
          slideStep = -0x80000;
        }
        if ((u32)(frame - 48) <= 5) {
          slideStep = 0x80000;
        }
        if (frame > 59) {
          slideStep = -0x100000;
        }
        if (frame > 31) {
          slideStep = slideStep * 48 / 64;
        }
        slide += slideStep;
        sweepTexture.logWidth = 6;
        sweepTexture.logHeight = 5;
        sweepTexture.pixels = work->sheet;
        sweepModel->mesh = strip;
        sweepModel->scrollU = (frame << 3) & 127;
        Transform_Reset();
        SceneTransform_ApplyScale3(Q16_ONE / 2, Q16_ONE, Q16_ONE / 2);
        a = frame * 3 * 127 * 16;
        b = frame << 13;
        c = Trig_Sin(a);
        SceneTransform_ApplyPosition3(slide, (c << 5) + Trig_Cos(b) * 24, 0);
        SceneTransform_ApplyUniformScale(55536);
        Transform_Push();
        Transform_ApplyRoll(TRIG_TURN / 4);
        SceneTransform_ApplyUniformScale(0x42ee0);
        Render_TransformVertices(cellBuffer, quads, 24);
        Render_DrawModelList(sweepModel);
        Transform_Pop();
        sweepTexture.logWidth = 6;
        sweepModel->mesh = gBattleEffect_SquareMesh;
        sweepTexture.logHeight = 6;
        sweepModel->scrollU = 0;
        sweepTexture.pixels = work->sheet + 0x800;
        SceneTransform_ApplyPosition3(
            -0x20000, (Trig_Sin(frame << 10) << 2) + -0x60000, 0);
        Transform_ApplyRoll(-TRIG_TURN / 32);
        {
          s32 s0 = Trig_Sin(a);
          s32 y0 = (s0 << 5) + Trig_Cos(b) * 24;
          s32 s1;
          s32 y1;

          a += 6096;
          s1 = Trig_Sin(a);
          b += 0x2000;
          y1 = (s1 << 5) + Trig_Cos(b) * 24;
          Transform_ApplyRoll((y0 - y1) / 512 + -0x400);
        }
        SceneTransform_ApplyScale3(0x21170, 105536, Q16_ONE);
        Render_TransformVertices(gBattleEffect_SquareCorners, quads, 4);
        Render_DrawModelList(sweepModel);
        Heap_Release(sweepModel);
        if (frame <= 87) {

          i = 0;
          p2 = work->particles;
          {

            for (; i != 6; p2++, i++) {
              s32 lim = i * 8 + 16;

              if (frame == lim) {
                p2->x = (Random16() & 31) + 64;
                p2->variant = Random16();
                work->shakeFrames = 4;
                switch (i & 3) {
                case 0:
                  Sound_PlayFar(SE_TOPPLE);
                  break;
                case 1:
                  Sound_PlayFar(SE_THUNDER);
                  break;
                case 2:
                  Sound_PlayFar(SE_BANE_ESCAPE);
                  break;
                case 3:
                  Sound_PlayFar(SE_THUNDER_LOUD);
                  break;
                }
                j = 0;
                q = (struct Particle *)((i & 7) * 336 + (s32)debris);
                for (; j != 12; j++, q++) {
                  s32 angle;
                  s32 speed;

                  q->x = px(p2->x);
                  q->y = px(104);
                  angle = (Random16() & 0x7fff) + TRIG_TURN / 4;
                  speed = (Random16() & 127) + 128;
                  q->velocityX = Trig_Sin(angle) * speed >> 5;
                  q->velocityY = Trig_Cos(angle) * speed >> 4;
                  q->variant = 16;
                }
              }
              if (frame >= lim && frame < lim + 8) {
                u8 *src = gData_02014000;

                blit[0](canvas, ((p2->variant / 2) & 3) * 2880 + src,
                        p2->x - 12, 0, 24, 120);
                p2->variant++;
              }
            }
          }
          {
            struct Particle *d = debris;

            for (i = 0; i != 100; i++, d++) {
              if (d->variant >= 0) {
                s32 n = (d->variant >> 2) + 2;

                s32 h = n * 2;

                blit[0](canvas, sprites + gBattleEffect_MotePictures[n - 1],
                        Q_16_16_UPPER_HALF(d->x) - n / 2,
                        Q_16_16_UPPER_HALF(d->y) - n, n, h);
                BattleEffectKernels_IntegrateVector2(d, 56, px(1) / 8);
                d->velocityX -= work->scrollStepX << 12;
                d->variant--;
              }
            }
          }
        }
      }
      gProjection.centerX -= work->scrollStepX;
      gBgScroll[1].x += work->scrollStepX;
      for (i = 0; i != 16; i++) {
        if (frame == i + 62) {
          work->scrollStepX++;
        }
      }
      Camera_ApplyVerticalShake(8);
      BattleEffect_CountDownBattlerTints();
      {
        s32 *pending = &work->transferPending;

        frame++;
        *pending = 1;
      }
      wait(1);
    } while (frame != 104);
    transition[4] = 0;
    EFFECT_FINISH_CANVAS_ONE_BLITTER();
  }
}
#undef BLITTER
#undef OBJ_TILES

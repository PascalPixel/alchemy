/// @file
/// The Ulysses summon and Spider Web, with the effect sprites, grey ramp and
/// circle maps they draw with.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"
#include "fixed_math.h"
#include "sounds.h"

/* VIEW(f15b3b0): routine Animation_ApplyChildArgumentFar as s32
 * Animation_ApplyChildArgumentFar(void *, s32); ability_effects has void
 * Animation_ApplyChildArgumentFar(void *object, s32 value); tried as the rest
 * of the object declares it, EffectSprite_Acquire moves: 1 of 46 instructions
 * in tla-ja. */
/// Animation_ApplyChildArgumentFar as Ulysses's part sees it: returning a word.
extern s32 Animation_ApplyChildArgumentFar__f15b3b0(void *, s32) __asm__(
    "Animation_ApplyChildArgumentFar");

/* VIEW(f15b3b0): tag struct EffectSprite as struct EffectSprite__f15b3b0;
 * ability_effects.c has another definition; tried with the definition the rest
 * of the object has, it does not compile in tla-ja:
 * games/tla/src/battle_effect/battle_effect/spider_web.c:27: structure has no
 * member named `kind'. */
struct EffectSprite__f15b3b0 {
  u8 unk00[0x0c];
  s32 scale;
  u8 slot;
  u8 retainCacheEntry : 1;
  u8 otherFlags : 7;
  u16 rotation;
  u8 width;
  u8 height;
  u8 offsetX;
  u8 offsetY;
  u8 lastNoAndDirty[2];
  u8 flags;
  u8 count;
  u8 shadow[0x0c];
  struct AnimationEntry *entry;
  u8 moreEntries[0x0c];
};

/// Takes the first free effect sprite of the 64 with a new animation entry.
/// Its callers also pass the sprite's OAM shape and mode, which it leaves for
/// the caller to set.
void *EffectSprite_Acquire(s32 width, s32 height, u32 shape, s32 mode) {
  struct EffectSprite__f15b3b0 *sprite = Ram_HeapSlots->sprites;
  struct EffectSprite__f15b3b0 *found = NULL;
  s32 i = 0;

  while (i <= 63) {
    if (sprite->width == 0) {
      found = sprite;
      break;
    }
    i++;
    sprite++;
  }
  sprite->entry = AnimationObject_AllocateEmpty();
  sprite->width = width;
  sprite->height = height;
  sprite->scale = 0x10000;
  sprite->retainCacheEntry = 0;
  sprite->offsetY = 0;
  sprite->offsetX = 0;
  sprite->count = 1;
  sprite->flags = 0;
  sprite->rotation = 0;
  Animation_ApplyChildArgumentFar__f15b3b0(sprite, 0);
  return found;
}

/// Fills BG palette colours 1 to 63 with a grey ramp, each level i / 2.
void Graphics_LoadGreyRamp(void) {
  s32 i = 0;
  u16 *color = (u16 *)(BG_PLTT + 1);

  do {
    {
      s32 level = i / 2;

      i++;
      *color = level << 10 | level << 5 | level;
    }
    color++;
  } while (i != 63);
}

void BattleEffect_DrawShadedCircles(u8 *map, s32 size, s32 scale) {
  s32 half = size / 2;
  s32 center = half - 1;
  s32 last = size - 1;
  s32 radius;

  Iwram_ClearWordsEntry(map, half * size);
  for (radius = 0; radius != last; radius++) {
    s32 x = radius;
    s32 d = radius;
    s32 y = 0;
    s32 shade;
    s32 col;

    shade = (Trig_Cos((radius << 14) / last) * 63) >> 16;
    shade = shade * scale / (Q16_ONE);

    if (shade > 63)
      shade = 63;
    while (x >= y) {
      col = center - x / 2;
      map[half * (last - y) + col] = shade;
      col = center - y / 2;
      map[half * (last - x) + col] = shade;
      d = d - y * 2 - 1;
      if (d < 0) {
        d = d + x * 2 - 2;
        x--;
      }
      y++;
    }
  }
}

void BattleEffect_DrawShadedCirclesMirrored(u8 *map, s32 size, s32 scale) {
  s32 center = size / 2 - 1;
  s32 last = size - 1;
  s32 radius;

  Iwram_ClearWordsEntry(map, size * size * 2);
  for (radius = 0; radius != last; radius++) {
    s32 x = radius;
    s32 d = radius;
    s32 y = 0;
    s32 shade;
    s32 col;
    s32 row;

    shade = (Trig_Cos((radius << 14) / last) * 63) >> 16;
    shade = shade * scale / (Q16_ONE);
    if (shade > 63)
      shade = 63;
    while (x >= y) {
      col = center - x / 2;
      row = last - y;
      map[row * size + col] = shade;
      map[(size * 2 - row - 1) * size + col] = shade;
      map[row * size + (size - col) - 1] = shade;
      map[(size * 2 - row - 1) * size + (size - col) - 1] = shade;
      col = center - y / 2;
      row = last - x;
      map[row * size + col] = shade;
      map[(size * 2 - row - 1) * size + col] = shade;
      map[row * size + (size - col) - 1] = shade;
      map[(size * 2 - row - 1) * size + (size - col) - 1] = shade;
      d = d - y * 2 - 1;
      if (d < 0) {
        d = d + x * 2 - 2;
        x--;
      }
      y++;
    }
  }
}

/// Mirrors the first 64 background colours, keeping the backdrop colour.
void Palette_MirrorBgColors(void) {
  u16 saved[64];
  u16 flipped[64];
  s32 i;

  Iwram_CopyWords(saved, (void *)BG_PLTT, 128);
  for (i = 1; i != 64; i++)
    flipped[i] = saved[64 - i];
  flipped[0] = saved[0];
  Iwram_CopyWords((void *)BG_PLTT, flipped, 128);
  REG_BLDALPHA = BLEND_ALPHA(16, 8);
}

/// A frame callback of Ulysses (BattleEffect_RunUlysses adds and removes it):
/// places effect object 7 at the last particle's point (x across, y in depth)
/// at a scale of one, moves the particle by its velocity (its depth speed
/// grows for 20 frames, then shrinks, and its sideways speed grows from frame
/// 12), and steps the object's animation through cels 7 to 14, seven frames
/// each.
void Ulysses_MoveLastParticle(void) {
  struct BattleEffectWork *work =
      (struct BattleEffectWork *)Ram_HeapSlots->battleEffect;
  struct SpriteScale scale = gUlysses_ParticleScale;
  struct SpritePlacement place;

  place.groundHeight = 0;

  place.height = 0;

  place.x = work->particles[63].x;
  place.y = work->particles[63].y;
  Render_ApplyProjectedPlacementFar(work->sprites[7], &place.x, &scale.x, 0);
  work->particles[63].x += work->particles[63].velocityX;
  work->particles[63].y += work->particles[63].velocityY;
  if (work->frame <= 19)
    work->particles[63].velocityY += px(1) / 16;
  else
    work->particles[63].velocityY -= px(1) / 16;
  if (work->frame > 11)
    work->particles[63].velocityX += px(1) / 16;
  Animation_ApplyChildArgumentFar__f15b3b0(work->sprites[7],
                                           work->frame / 7 % 8 + 7);
  work->frame++;
}

struct StripQuad;

#define BLITTER(mode, flags) ((mode) << 4 | (flags))

extern const struct Scale gUlysses_ShipScale;

/// Ulysses: the hero's ship sails out of a whirlpool, its crew of six
/// warriors leap at the foe and the ship's broadside lands as a storm of
/// explosions.
void BattleEffect_RunUlysses(struct BattleEffectParams *args) {
  u8 *canvas;
  s32 *transition;
  struct BattleEffectWork *work;
  s32 frame;
  u8 *sprites;
  s32 dmaArmed;
  s32 skip;
  DrawRectangle *blit;
  struct {
    s32 x;
    s32 z;
  } ship;
  s32 loaded;
  s32 draw;
  s32 i;
  u8 *cells;
  u8 *picture;
  s32 place[4];
  DrawRectangle blits[2];
  struct ModelTexture texture;
  struct ModelTexture quadTexture;
  struct Scale req;
  u8 *battle;
  s32 mask;

  canvas = Ram_HeapSlots->battleCanvas;
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleEffect;
  transition = Ram_HeapSlots->battleBackground;
  sprites = Ram_HeapSlots->battleSheet;
  dmaArmed = 0;
  skip = 0;
  BattleEffect_SelectLivingTargets(work->args);
  for (frame = 0, mask = 31; frame != 32; frame++) {
    volatile u16 *c = (volatile u16 *)(BG_PLTT + 96);

    for (i = 0; i != 128; i++, c++) {
      s32 v = *c;
      s32 r = mask & v;
      s32 g;
      s32 b;

      g = (u16)(((u16)v >> 5) & 31);
      b = (u16)(((u16)v >> 10) & 31);
      if (g == 0) {
        if (b > 0) {
          b--;
        }
        if (r > 0) {
          r--;
        }
      }
      if (g > 0) {
        g--;
      }
      *c = b << 10 | g << 5 | r;
    }
    wait(1);
  }
  BattleEffect_StartCanvasLayer(0);
  IO_WRITE_QUEUE_PUSH(0x04000000, 0x1141, 0x20000);
  wait(1);
  {
    u32 *map = (u32 *)BG_SCREEN_BLOCK(31);
    u32 v;
    s32 y;
    s32 x;

    Iwram_FillWords(map, 256, -1);
    map += 64;
    Iwram_FillWords(map, 128, 0x03ff03ff);
    v = (u32)Data_02010200;
    map += 32;
    for (y = 0; y != 16; y++) {
      for (x = 0; x != 8; x++) {
        *map++ = v;
        v += 0x20002;
      }
      for (x = 0; x != 8; x++) {
        *map++ = 0x03ff03ff;
      }
    }
  }
  BattleEffect_SpawnObjects(8, 0x28f, 3);
  REG_BG2PA = Q_8_8(0.5);
  REG_BG2PA = Q_8_8(1);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  REG_BLDCNT =
      BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BG1CNT = BGCNT_PRIORITY(3) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(31);
  wait(1);
  blit = blits;
  BattleEffect_FetchRectangleBlitters(0, (void **)blit);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  cells = work->sheet + 0x1781;
  Resource_LoadAndDecompress((s32)&ResourceId_Data0B1, cells, 0, 0);
  for (i = 0; i != 0x2300; i++, cells++) {
    if (*cells) {
      *cells += 128;
    }
  }
  picture = work->sheet + 0x1781;
  blits[0](canvas, picture, 24, 8, 80, 112);
  EFFECT_PRESENT_CANVAS(work);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  Resource_LoadAndDecompress((s32)&ResourceId_EmberStreakSheet, work->sheet, 1,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0B3, picture, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD, sprites, 0, 0);
  loaded = 0;
  ship.x = px(200);
  ship.z = px(84);
  transition[4] = 1;
  Iwram_ClearWords(canvas, 0x3c00);
  REG_DISPCNT = DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                DISPCNT_OBJ_ON | DISPCNT_WIN0_ON | DISPCNT_WIN1_ON;
  wait(1);
  for (frame = 0; frame != 326; frame++) {
    struct Particle *step;
    draw = 1;
    if (gInput.repeat & (KEY_A | KEY_B)) {
      skip = 1;
    }
    if ((frame & draw) && skip == 1) {
      draw = 0;
    }
    if (frame == 0) {
      struct Particle *p;

      if (!loaded) {
        Task_Add(BattleEffect_ProcessPendingBlock2Transfer,
                 TASK_PASS_FRAME + 15);
        Task_Add(Display_ApplyBg2Affine, TASK_PASS_FRAME + 127);
        Resource_LoadAndDecompress((s32)&ResourceId_RuneCircleSheet,
                                   work->sheet + 0x2781, 1, 0);
        loaded = 1;
      }
      gBgScroll[1].x = 0xff90;
      REG_BLDALPHA = BLEND_ALPHA(16, 16);
      REG_DISPCNT = EFFECT_DISPCNT;
      REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
      REG_BG1CNT = BGCNT_256_COLORS | BGCNT_SCREEN_BASE(31);
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE,
                          CANVAS_FADE_THREE_QUARTERS);
      Iwram_CopyWordsEntry(
          (void *)BG_PLTT,
          Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet), 128);
      for (i = 0; i != 9; i++) {
        p = &work->particles[i];
        p->variant = -2 * i;
        p->x = (Random16() & 15) + 60;
        p->y = (Random16() & 15) + 44;
      }
    }
    if (frame <= 75) {
      s32 k = (frame / 4) % 18;
      s32 x = 60 - ((Trig_Sin(frame * 860) << 3) >> 16);
      s32 y = frame - 8;
      u8 *source = work->sheet + 0x1781 + gUlysses_Pictures[k];
      u8 w = gUlysses_PictureWidths[k];
      s32 left = x - (w >> 1);
      u8 h = gUlysses_PictureHeights[k];

      blit[1](canvas, source, left, y - (h >> 1), w, h);
    }
    if ((u32)(frame - 76) <= 31) {
      struct Particle *p;

      for (i = 0, p = work->particles; i != 4; i++, p++) {
        if ((u32)p->variant <= 17) {
          s32 k = p->variant / 3;
          s32 side = i & 1;
          u8 *source = work->sheet + gBattleEffect_PuffArcPictures[k];
          u8 w = gBattleEffect_PuffArcPictureWidths[k];

          blit[side](canvas, source, p->x - (w >> 1),
                     p->y + gBattleEffect_PuffArcPictureBiasY[k] - 32, w,
                     gBattleEffect_PuffArcPictureHeights[k]);
        }
        p->variant++;
      }
    }
    if (frame == 108) {
      REG_BLDALPHA = BLEND_ALPHA(16, 0);
    }
    if ((u32)(frame - 108) <= 8) {
      REG_BLDALPHA = ((frame - 108) * 2) << 8 | 16;
    }
    if ((u32)(frame - 92) <= 39) {
      s32 t = frame - 92;
      s32 z = Trig_Cos(t * 800) / 3 + TRIG_TURN;
      s32 q16;
      s32 level = -1;

      if (z <= 0x107ff) {
        z = 0x10800;
      }
      q16 = 0x1000000 / z;
      *(volatile u16 *)&work->bg2pa;
      *(volatile u16 *)&work->bg2pa = q16;
      *(volatile u16 *)&work->bg2pd;
      *(volatile u16 *)&work->bg2pd = q16;
      work->bg2x = (64 - 0xb80000 / z) << 8;
      work->bg2y = (64 - 0x4c0000 / z) << 8;
      if (t <= 15) {
        level = t;
      } else if ((u32)(frame - 116) <= 8) {
        level = 64 - t * 2;
      }
      if (level != -1) {
        for (i = 0; i != 32; i++) {
          s32 g = (i * ((level * 9 * 4) / 16)) / 64;
          s32 r = (i * level * 3) / 64;

          ((u16 *)(BG_PLTT + 128))[i] = r << 10 | g;
        }
      }
      if (frame == 116) {
        REG_DISPCNT = DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                      DISPCNT_BG1_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON |
                      DISPCNT_WIN1_ON;
      }
    }
    if ((u32)(frame - 124) <= 7) {
      volatile u16 *c = (volatile u16 *)BG_PLTT;

      for (i = 0; i != 64; i++, c++) {
        s32 v = *c;
        s32 low = 31;
        s32 r;
        s32 g;
        s32 b;

        low &= v;
        r = low - 3;
        g = ((u16)(((u16)v >> 5) & 31)) - 3;
        b = ((u16)(((u16)v >> 10) & 31)) - 3;

        if (r < 0)
          r = 0;
        if (g < 0)
          g = 0;
        if (b < 0)
          b = 0;
        *c = b << 10 | g << 5 | r;
      }
    }
    if (frame == 132) {
      if (loaded == 1) {
        Task_Remove(BattleEffect_ProcessPendingBlock2Transfer);
        Task_Remove(Display_ApplyBg2Affine);
        Task_Add(BattleEffect_ProcessPendingGraphicsTransfer,
                 TASK_PASS_FRAME + 15);
        Resource_LoadAndDecompress((s32)&ResourceId_YellowOrbSheet,
                                   work->sheet + 0x1e97, 1, 0);
        loaded = 0;
      }
      REG_DISPCNT = DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                    DISPCNT_OBJ_ON | DISPCNT_WIN0_ON | DISPCNT_WIN1_ON;
      REG_BG2X = -0x800;
      REG_BG2Y = -0x1000;
      REG_BG2PA = 0xcc;
      REG_BG2PD = Q_8_8(1);
      REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
      REG_BLDALPHA = BLEND_ALPHA(16, 16);
      Iwram_CopyWordsEntry((void *)BG_PLTT,
                           Resource_GetTableEntry((s32)&ResourceId_RuneSheet),
                           128);
    }
    if (frame == 133) {
      const s8 *src;
      struct Particle *p;
      struct Particle *q;

      Iwram_FillWords(canvas, 0x3c00, 0);
      Iwram_ClearWords((void *)BG_CHAR_BLOCK(1), 0x3c00);
      REG_DISPCNT = DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                    DISPCNT_BG2_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON |
                    DISPCNT_WIN1_ON;
      src = gUlysses_ParticleStarts;
      q = (struct Particle *)(work->sheet + 0x4b98);
      for (i = 0; i != 6; i++, src += 5) {
        p = &work->particles[i];
        p->x = px(src[0]);
        p->variant = -8 * i;
        /* FAKEMATCH(barrier): an empty asm parts the particle's first stores
         * from the rest. */
        __asm__("");
        p->y = px(src[1]);
        p->z = src[2] << 6;
        p->velocityX = src[3] << 15;
        p->velocityY = src[4] << 15;
        p->velocityZ = 0;
      }
      for (i = 0; i != 96; i++) {
        q[i].variant = -1;
      }
      Resource_LoadAndDecompress((s32)&ResourceId_Data0C8, gMapGridCellBuffer,
                                 0, 0);
    }
    if (frame == 76) {
      Sound_PlayFar(SE_ITEM_BREAK);
    }
    if (frame == 266) {
      Sound_PlayFar(SE_ITEM_BREAK);
    }

    if ((u32)(frame - 133) <= 132) {
      void *verts;
      struct RenderModel *model;
      struct Particle *sparks;
      u8 *portraits = gMapGridCellBuffer;
      struct Particle *q;
      s32 shade;
      s32 j;

      verts = Heap_AllocEwram(32);
      model = Render_AllocateModelList(1);
      shade = 0;
      sparks = (struct Particle *)(work->sheet + 0x4b98);
      if (frame > 196) {
        BattleEffect_StepPaletteToResource((s32)&ResourceId_MarsDjinnSheet);
      }
      texture.logWidth = 6;
      texture.logHeight = 6;
      model->kind = 7;
      model->shade = shade;
      model->texture = &texture;
      model->mesh = gBattleEffect_SquareMesh;
      model->vertices = verts;
      for (i = 0, step = work->particles; i != 6; step++, i++) {
        s32 delay = i * 8;
        struct Particle *anchor = step + 8;
        s32 t;

        if (++step->variant == 0) {
          Sound_PlayFar(SE_ITEM_BREAK);
        }
        t = step->variant;
        if (t == 60) {
          Sound_PlayFar(SE_POWER_UP);
          t = step->variant;
        }
        if (t == 64) {
          Sound_PlayFar(SE_HEAVY_IMPACT);
          t = step->variant;
        }
        if ((u32)t <= 35) {
          s32 k = (t / 6) % 6;
          u8 *source = work->sheet + gBattleEffect_PuffArcPictures[k];
          u8 w = gBattleEffect_PuffArcPictureWidths[k];

          blits[0](canvas, source, Q_16_16_TO_INT(step->x) - (w >> 1),
                   Q_16_16_TO_INT(step->y) +
                       gBattleEffect_PuffArcPictureBiasY[k] - 36,
                   w, gBattleEffect_PuffArcPictureHeights[k]);
          t = step->variant;
        }
        if ((u32)(t - 12) <= 31) {
          s32 k = ((t - 12) / 4) % 18;
          u8 *source = work->sheet + 0x1781 + gUlysses_Pictures[k];
          s32 x = Q_16_16_TO_INT(step->x);
          u8 w = gUlysses_PictureWidths[k];
          s32 left = x - (w >> 1);
          s32 y = Q_16_16_TO_INT(step->y);
          u8 h = gUlysses_PictureHeights[k];

          blit[1](canvas, source, left, y - (h >> 1), w, h);
          t = step->variant;
        }
        if ((u32)(t - 44) <= 21) {
          s32 s = (Trig_Sin((t - 44) << 10) * 48) / 64;
          s32 vx;
          s32 x = step->x;
          s32 mx = x - 0x400000;
          s32 my = step->y - px(64);
          s32 u = t - 59;

          if (u < 0) {
            u = 0;
          }
          vx = step->velocityX;
          step->y += step->velocityY;
          x += vx;
          step->x = x;
          if (x <= 0x3fffff) {
            step->velocityX = vx + 0x8000;
          }
          if (step->y <= 0x3bffff) {
            step->velocityY += px(1) / 2;
          } else {
            step->velocityY -= px(1) / 2;
          }
          if (step->z >= 0) {
            step->z += (Trig_Sin(step->velocityZ) * 3 << 8) >> 16;
          } else {
            step->z -= (Trig_Sin(step->velocityZ) * 3 << 8) >> 16;
          }
          step->velocityZ += 1024;
          if (draw) {
            texture.pixels = (u8 *)((u << 12) + (s32)portraits);
            Transform_Reset();
            SceneTransform_ApplyPosition3(mx, my, 0);
            Transform_ApplyPitch(TRIG_TURN * 3 / 4);
            Transform_ApplyYaw(step->z);
            SceneTransform_ApplyUniformScale(s);
            Render_TransformVertices(gBattleEffect_GroundSquareCorners, verts,
                                     4);
            Render_DrawModelList(model);
          }
          t = step->variant;
        }
        if (t == 64) {
          anchor->x = Q_16_16_TO_INT(step->x);
          anchor->y = Q_16_16_TO_INT(step->y);
          j = 0;
          {
            s32 first = i * 16;

            q = (struct Particle *)(first * (s32)sizeof(*q) + (s32)sparks);
          }
          for (; j != 16; j++, q++) {
            s32 a = Random16() & 0xffff;
            s32 sp = (Random16() & 15) + 8;

            q->variant = (Random16() & 7) + 8;
            q->x = step->x;
            q->y = step->y;
            q->velocityX = (Trig_Sin(a) * sp) / 4;
            q->velocityY = (Trig_Cos(a) * sp) / 4;
          }
          for (j = 0; j != args->targetCount; j++) {
            s32 m = (Random16() << 2) + 0x10000;

            BattleEffect_StartActorMotion(args->targets[j], 1, 0x20000, 0x80000,
                                          m, delay + (Random16() & 31) + 52);
            BattleEffect_TintBattler(args->targets[j], SPRITE_COLOR_WHITE,
                                     TINT_KEEP, j, 4);
          }
        }
        if (draw && (u32)(step->variant - 64) <= 11) {
          s32 k = ((step->variant - 64) / 2) % 6;

          blit[1](canvas, work->sheet + 0x1e97 + k * 0x780, anchor->x - 20,
                  anchor->y - 24, 40, 48);
        }
      }
      for (i = 0; i != 96; i++) {
        q = &sparks[i];
        if (q->variant >= 0) {
          s32 n = q->variant / 4 + 2;

          if (draw) {
            blit[1](canvas, sprites + gBattleEffect_FlarePictures[n - 1],
                    Q_16_16_TO_INT(q->x) - n, Q_16_16_TO_INT(q->y) - n, n * 2,
                    n * 2);
          }
          BattleEffectKernels_IntegrateVector2(q, 60, px(1) / 8);
          q->variant--;
        }
      }
      Heap_Release(model);
      Heap_Release(verts);
    }

    if (draw && frame <= 131) {
      void *verts;
      struct RenderModel *model;
      s32 j;

      verts = Heap_AllocEwram(32);
      model = Render_AllocateModelList(1);
      /* FAKEMATCH(dead): j takes frame * 3 + 1 here and 0 below before any
       * read. */
      j = frame * 3 + 1;
      model->shade = 0;
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
      quadTexture.logWidth = 7;
      quadTexture.logHeight = 7;
      model->texture = &quadTexture;
      quadTexture.pixels = work->sheet + 0x2781;
      model->kind = 7;
      model->mesh = gBattleEffect_LargeSquareMesh;
      model->vertices = verts;
      j = 0;
      for (; j != 2; j++) {
        s32 t = frame - 76 - j * 16;

        if (t >= 0) {
          // One scale for both arms: the else arm's constant then keeps a
          // register of its own.
          s32 sc;

          Transform_Reset();
          SceneTransform_ApplyPosition3(px(8), 0, 0);
          Transform_ApplyPitch(TRIG_TURN / 4);
          if (j == 0) {
            sc = t * 3072 - 0x12000;
            if (t <= 39) {
              sc = t << 11;
              if (sc > 0xc000) {
                sc = 0xc000;
              }
            }
            SceneTransform_ApplyUniformScale(sc);
            Transform_ApplyYaw(-t * 512);
          } else {
            if (t > 23) {
              sc = 0x1c000 - (t << 11);
              SceneTransform_ApplyUniformScale(sc);
            } else {
              Transform_ApplyPitch(0x3ff0 - t * 682);
              Transform_ApplyRoll(t * 169 + 0xf028);
            }
            Transform_ApplyYaw(t << 9);
          }
          Render_TransformVertices(gBattleEffect_GroundSquareCorners, verts, 4);
          Render_DrawModelList(model);
        }
      }
      Heap_Release(model);
      Heap_Release(verts);
    }

    if ((u32)(frame - 108) <= 179) {
      req = gUlysses_ShipScale;
      if (frame > 131) {
        s32 u = frame - 132;

        ship.x = (Trig_Sin(u << 11) << 1) + 0xc80000;
        ship.z = (Trig_Sin(u << 10) << 1) + 0x540000;
      }
      if (draw) {
        place[1] = place[3] = 0;
        for (i = 0; i != 7; i++) {
          place[0] = px(gUlysses_ObjectX[i]) + ship.x - px(32);
          place[2] = px(gUlysses_ObjectZ[i]) + ship.z - px(32);
          Render_ApplyProjectedPlacementFar(work->sprites[i], place,
                                            (s32 *)&req, 0);
        }
      }
    }
    if (frame > 265) {
      if (frame == 266) {
        struct Particle *p;

        REG_BG2X = -0x7000;
        REG_BG2PA = Q_8_8(1);
        REG_BG2PD = Q_8_8(1);
        for (i = 0, p = work->particles; i != 32; i++, p++) {
          p->x = Random16() % 48 + 40;
          p->y = Random16() % 96 + 16;
          p->variant = -i / 2;
        }
      }
      for (i = 0, step = work->particles; i != 32; i++, step++) {
        if ((u32)step->variant <= 23 && draw) {
          s32 k = (step->variant / 4) % 6;
          u8 *source = work->sheet + gBattleEffect_PuffArcPictures[k];
          u8 w = gBattleEffect_PuffArcPictureWidths[k];

          blits[0](canvas, source, step->x - (w >> 1),
                   step->y + gBattleEffect_PuffArcPictureBiasY[k] - 48, w,
                   gBattleEffect_PuffArcPictureHeights[k]);
        }
        step->variant++;
      }
    }
    if (frame == 288) {
      Task_Add(Ulysses_MoveLastParticle, TASK_PASS_VBLANK);
      work->particles[63].x = px(188);
      work->particles[63].y = px(96);
      work->particles[63].velocityX = 0;
      work->particles[63].velocityY = 0;
      work->frame = 0;
      dmaArmed = 1;
    }
    if (draw) {
      EFFECT_END_FRAME(work);
    }
  }

  EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
  REG_BG1CNT = BGCNT_PRIORITY(3) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(31);
  wait(1);
  /* FAKEMATCH(dead): a once-only block. */
  {
    void *destination = Ram_HeapSlots->battleCanvas;

    IWRAM_FILL_WORDS(destination, 0x4000, 0);
  }
  IWRAM_FILL_WORDS((void *)BG_CHAR_BLOCK(1), 0x3c00, 0);
  Iwram_FillWords((void *)BG_CHAR_BLOCK(2), 0x7800, 0);
  wait(1);
  REG_BLDALPHA = BLEND_ALPHA(14, 16);
  REG_BLDCNT =
      BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_DISPCNT = EFFECT_DISPCNT;
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  EFFECT_RELEASE_BLITTERS();
  transition[4] = 0;
  battle = Ram_HeapSlots->battleWork;
  Sound_PlayFar(SOUND_FADE_OUT_ACTION_SE);
  gBgScroll[1].x = work->savedBg1X;
  gBgScroll[1].y = work->savedBg1Y;
  gCameraSceneParameters[3] = 120;
  gCameraSceneParameters[4] = 120;
  Iwram_ClearWords((void *)(BG_PLTT + 96), 256);
  wait(1);
  BattleBackground_LoadFar(2, *(u16 *)(battle + 0x670), -1);
  REG_BG2CNT = BGCNT_PRIORITY(3) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7);
  Iwram_ClearWords((void *)BG_CHAR_BLOCK(1), 0x3c00);
  Task_Remove(Palette_StepFadeTransfer);
  gBgScroll[1].y = 32;
  IO_WRITE_QUEUE_PUSH(0x04000000, 0x7341, 0x20000);
  REG_BLDCNT = 0;
  wait(1);
  for (frame = 0; frame != 40; frame++) {
    u16 *src = (u16 *)(battle + 0x56c);
    volatile u16 *dst = (volatile u16 *)(BG_PLTT + 96);

    for (i = 0; i != 128; i++, src++, dst++) {
      s32 v = *src;
      s32 r = 31;
      s32 g;
      s32 b;
      s32 nr;
      s32 ng;
      s32 nb;

      r &= v;
      g = (u16)(((u16)v >> 5) & 31);
      b = (u16)(((u16)v >> 10) & 31);
      nr = r + frame - 30;
      ng = g + frame - 40;
      nb = b + frame - 30;
      if (nr < 0) {
        nr = 0;
      } else if (nr > r) {
        nr = r;
      }
      if (ng < 0) {
        ng = 0;
      } else if (ng > g) {
        ng = g;
      }
      if (nb < 0) {
        nb = 0;
      } else if (nb > b) {
        nb = b;
      }
      *dst = nb << 10 | ng << 5 | nr;
    }
    wait(1);
  }
  IO_WRITE_QUEUE_PUSH(0x04000000, 0x7541, 0x20000);
  if (dmaArmed == 1) {
    Task_Remove(Ulysses_MoveLastParticle);
  }
  for (i = 0; i != 8; i++) {
    Sprite_DestroyFar(work->sprites[i]);
  }
}

/// Abilities: Poison Beat (kind 382).
void BattleEffect_RunPoisonBeat(struct BattleEffectParams *args) {
  BattleEffect_RunSwirlSwipe(args, 0);
}

/// Abilities: Spinning Beat (kind 383).
void BattleEffect_RunSpinningBeat(struct BattleEffectParams *args) {
  BattleEffect_RunSwirlSwipe(args, 1);
}

/// A swirl sweeps across the first target from the user's side.
void BattleEffect_RunSwirlSwipe(struct BattleEffectParams *args, s32 mode) {
  struct HeapSlots *slots;
  struct BattleEffectWork *work;
  void *canvas;
  struct MotionObject *object;
  struct ScreenPoint position;
  // Only draw[0] is used; the pair takes two registers (see the header).
  DrawRectangle draw[2];
  s32 frames;
  s32 frame;
  s32 cel;

  // Setup: the canvas at a scale of one, a transparent blitter, mirrored
  // for side 0.
  slots = Ram_HeapSlots;
  work = (struct BattleEffectWork *)slots->battleEffect;
  canvas = slots->battleCanvas;
  EFFECT_START_NARROW_CANVAS(0);
  if (args->side == EFFECT_SIDE_PARTY) {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                        BLITTER_TRANSPARENT);
    draw[0] = (DrawRectangle)slots->ramCode;
  } else {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                        BLITTER_TRANSPARENT);
    draw[0] = (DrawRectangle)slots->ramCode;
  }
  Resource_LoadAndDecompress((s32)&ResourceId_MagentaSwirlSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_MagentaTailSheet,
                             Ram_MapCellBuffer, 1, 0);
  if (mode == 0) {
    void *palette = Resource_GetTableEntry((s32)&ResourceId_LimePalette);
    s32 (*copy)(void *, const void *, s32) = Iwram_CopyWordsEntry;
    copy((void *)BG_PLTT, palette, 128);
  }
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

  // BG2 is placed on the first target.
  object = BattleObject_GetFar(args->targets[0])->object;
  EffectPosition_ProjectScaledBattler(args->targets[0], &position);
  if (args->side == EFFECT_SIDE_ENEMIES)
    REG_BG2X = (16 - position.x) << 8;
  else
    REG_BG2X = (112 - position.x) << 8;
  frames = 74;
  if (mode != 1)
    frames = 48;

  for (frame = 0; frame != frames; frame++) {
    // The swirl: six cels, four frames each, the last two from the tail.
    cel = frame / 4;
    if (cel <= 5) {
      if (cel <= 3)
        draw[0](canvas, work->sheet + gSpiderWeb_Pictures[cel],
                gSpiderWeb_PictureX[cel + args->side * 6],
                gSpiderWeb_PictureBiasY[cel] + 32,
                gSpiderWeb_PictureWidths[cel], gSpiderWeb_PictureHeights[cel]);
      else
        draw[0](canvas, Ram_MapCellBuffer + gSpiderWeb_Pictures[cel],
                gSpiderWeb_PictureX[cel + args->side * 6],
                gSpiderWeb_PictureBiasY[cel] + 32,
                gSpiderWeb_PictureWidths[cel], gSpiderWeb_PictureHeights[cel]);
    }
    // The hit.
    if (frame == 8) {
      if (mode == 0) {
        EFFECT_SHOW_DAMAGE(SE_IMPACT);
        BattleMotion_ApplyKnockbackFar(args->targets[0], KNOCKBACK_HOP);
      } else {
        Sound_PlayFar(SE_HEAVY_IMPACT);
        BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, 0, 4);
      }
      work->shakeFrames = 8;
    }
    // Mode 1 throws the target up at frame 13; it lands at frame 65.
    if (mode == 1) {
      if (frame == 13) {
        object->velocityY = px(12);
        object->verticalMotionStrength = 0x7851;
        object->verticalMotionPhase = 0x4000;
      }
      if (frame == 65) {
        work->shakeFrames = 4;
        EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      }
    }
    BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(work);
  }

  // Teardown.
  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

#define CLIP (COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y)

/// Builds the transparent blitter for one quarter.
#define LOAD_QUARTER(flags)                                                    \
  BattleEffect_LoadBlitter(HEAP_SLOT_RAM_CODE,                                 \
                           BLITTER(BLITTER_TRANSPARENT, flags))

/// Draws one quarter of the web with the blitter just built.
#define DRAW_QUARTER(x, y)                                                     \
  ((DrawRectangle)Ram_HeapSlots->ramCode)(canvas, work->sheet, x, y, size, size)

// Releases the quarter's blitter.
// UB: HeapSlot_Release returns nothing; the releases between the quarters
// call it through a function type that returns s64.
#ifdef UBFIX
#define RELEASE_QUARTER() HeapSlot_Release(HEAP_SLOT_RAM_CODE)
#else
#define RELEASE_QUARTER() ((s64 (*)(s32))HeapSlot_Release)(HEAP_SLOT_RAM_CODE)
#endif

/// A spider web spreads over the targets in four mirrored quarters.
///
/// Abilities: Spider Web.
void BattleEffect_RunSpiderWeb(struct BattleEffectParams *args) {
  struct ScreenPoint middle;
  void *canvas;
  struct BattleEffectWork *work;
  volatile u16 *blend;
  s32 size;
  s32 frame;
  s32 i;

  // Setup: the canvas on BG2, blending, the layer on the targets' middle.
  canvas = Ram_HeapSlots->battleCanvas;
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleEffect;
  EFFECT_START_NARROW_CANVAS(2);
  REG_BLDALPHA = BLEND_ALPHA(0, 16);
  BattleEffect_GetTargetsMidpoint(&middle);
  REG_BG2X = (64 - middle.x) << 8;
  Resource_LoadAndDecompress((s32)&ResourceId_SpiderWebSheet, work->sheet, 1,
                             1);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  Sound_PlayFar(SE_CRACK);
  blend = &REG_BLDALPHA;

  frame = 0;
  size = 32;
  do {
    // It fades in over nine frames and out over the last ten.
    if (frame <= 8)
      *blend = (frame << 1) | 0x1000;
    if (frame > 53)
      *blend = (0x7c - (frame << 1)) | 0x1000;

    // The four quarters: the one picture as it is, mirrored, flipped, and
    // both. Each blitter is built, drawn with and released before the next
    // is built; each once-only block holds a draw, its release and the next
    // build (as one block per quarter, 12 differ); the last draw needs none.
    LOAD_QUARTER(CLIP);
    do {
      DRAW_QUARTER(33, 41);
      RELEASE_QUARTER();
      LOAD_QUARTER(CLIP | COMPOSITE_FLIP_X);
    } while (0);
    do {
      DRAW_QUARTER(64, 41);
      RELEASE_QUARTER();
      LOAD_QUARTER(CLIP | COMPOSITE_FLIP_Y);
    } while (0);
    do {
      DRAW_QUARTER(33, 72);
      RELEASE_QUARTER();
      LOAD_QUARTER(CLIP | COMPOSITE_FLIP);
    } while (0);
    DRAW_QUARTER(64, 72);
    HeapSlot_Release(HEAP_SLOT_RAM_CODE);

    // The battle resumes, hit cue 143; every target flashes at frame 10.
    if (frame == 32)
      EFFECT_SHOW_DAMAGE(SE_CRACK);
    for (i = 0; i != args->targetCount; i++) {
      if (frame == 10)
        BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                 TINT_KEEP, i, 8);
    }
    EFFECT_END_FRAME(work);
    frame++;
  } while (frame != 63);

  // Teardown: clear both canvases and hand over to the summon canvas.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  Iwram_ClearWords((void *)BG_CHAR_BLOCK(1), 0x3c00);
  Iwram_ClearWords(canvas, 0x3c00);
  args->prelude = 0;
  Task_Remove(Palette_StepFadeTransfer);
  BattleEffect_RunImpair(args);
}

#undef LOAD_QUARTER
#undef DRAW_QUARTER
#undef RELEASE_QUARTER
#undef BLITTER
#undef CLIP

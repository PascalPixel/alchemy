/// @file
/// The Flora summon, Sabre Dance and Backstab.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

// Flora's summon.

struct StripQuad;

/// A sprite as Flora reads it: its tile, flags and resource.
struct FloraObject {
  u8 unk0[8];
  u16 tile : 10;
  u16 flags : 2;
  u16 unk08 : 4;
  u16 unk0a;
  u8 unk0c[4];
  u8 resource;
};

/// Flora's petals in the scratch block at 0x02014000: the bloom's fifteen
/// tile numbers, each petal's animation phase, position and velocity, and
/// the object palette as it fades.
struct FloraWork {
  u16 tiles[15];
  u16 phase[32];
  u16 unk5e;
  s32 x[32];
  s32 y[32];
  s32 vx[32];
  s32 vy[32];
  u16 palette[240];
};

#define w ((struct FloraWork *)petals)
#define OBJ_TILES(obj) (0x06010000 + gVramCacheEntries[(obj)->resource].offset)

/// The effect work's sheet.
static inline u8 *SheetOf(struct BattleEffectWork *owner) {
  /* FAKEMATCH(wrapper): the sheet's address through this inline. */
  return owner->sheet;
}

/// Flora: after a flash, petals blow across a violet sky and Flora drifts in
/// among them while rings of light spin about her; then a storm of petals
/// sweeps over the foes.
///
/// Abilities: Flora.
void BattleEffect_RunFlora(struct BattleEffectParams *args) {
  u8 *canvas;
  struct BattleEffectWork *work;
  DrawRectangle draw[2];
  s32 frame;
  s32 savedScroll;
  s32 *transition;
  u8 *sprites;
  s32 scroll;
  s32 posX;
  s32 posY;
  s32 angle;
  s32 spin;
  s32 spread;
  s32 grow;
  s32 petals; // the block's address as a number, see the header
  s32 i;
  s32 j;
  s32 quarter;
  s32 size;
  s32 dx;
  s32 dy;
  u32 u;
  s32 place[4];
  struct Scale req;
  struct ModelTexture texture;
  struct RenderModel *model;
  void *verts;
  struct Particle *p;
  u8 *cells;
  u16 *phase;
  u8 *source;
  struct ScreenPoint out;
  struct ScreenPoint pos;
  u8 dot[2];
  struct ScreenPoint ringScreen;
  struct ScreenPoint ringPoint;
  struct ScreenPoint fadeRingScreen;
  struct ScreenPoint fadeRingPoint;

  canvas = Ram_HeapSlots->battleCanvas;
  work = Ram_HeapSlots->battleEffect;
  savedScroll = gBgScroll[1].x;
  transition = (s32 *)Ram_HeapSlots->battleBackground;
  petals = (s32)&gData_02014000;
  source = Ram_MapCellBuffer;
  sprites = Ram_HeapSlots->battleSheet;
  BattleEffect_StartCanvasLayer(0);
  BattleEffect_ConfigureEffectDisplay();
  RagingHeat_FillGradientCanvas();
  work->transferMode = CANVAS_COPY;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  scroll = -80;
  for (frame = 0; frame != 52; frame++) {
    if (frame == 27) {
      u8 *e;

      gProjection.centerY = SUMMON_PROJECTION_CENTER_Y;
      BattleEffect_SelectLivingTargets(work->args);
      BattleBackground_LoadFar(1, (s32)&ResourceId_VioletSkyBackdrop, 0);
      work->scrollTimer = 0;
      work->scrollInterval = 1;
      work->scrollStepX = -3;
      work->scrollStepY = 0;
      Task_Add(BattleEffect_AdvanceScrollOnInterval, TASK_PASS_FRAME);
      transition[4] = 1;
      e = Resource_GetTableEntry((s32)&ResourceId_PinkPetalsTiles);
      Iwram_CopyWordsEntry((void *)(OBJ_PLTT + 15 * 16), e, 32);
      e += 32;
      Decompress_TaggedLz(e, source);
      cells = Ram_MapCellBuffer;
      for (i = 0; i != 15; i++) {
        struct FloraObject *obj =
            EffectSprite_CreateWithTiles(16, 16, 0x40000000, 0xf000);
        work->sprites[12 + i] = obj;
        obj->flags = 2;
        Iwram_CopyWordsEntry((void *)OBJ_TILES(obj), cells + i * 128, 128);
        w->tiles[i] = ((struct FloraObject *)work->sprites[12 + i])->tile;
      }
      for (i = 0; i != 17; i++) {
        struct FloraObject *obj =
            EffectSprite_Acquire(16, 16, 0x40000000, 0xf000);
        work->sprites[27 + i] = obj;
        Iwram_CopyWordsEntry(obj, work->sprites[12], 24);
      }
      i = 0;
      phase = Data_0201401e;
      for (; i != 32; i++, phase++) {
        *phase = (Random16() % 15) << 8;
        ((struct FloraObject *)work->sprites[12 + i])->tile =
            w->tiles[*phase / 256];
      }
      e = Resource_GetTableEntry((s32)&ResourceId_GreenHairedMaidenTiles);
      e += 480;
      Decompress_TaggedLz(e, source);
      cells = Ram_MapCellBuffer;
      for (i = 0; i != 12; i++) {
        struct FloraObject *obj =
            EffectSprite_CreateWithTiles(32, 32, 0x80002000, 0);
        obj->flags = 2;
        work->sprites[i] = obj;
        Iwram_CopyWordsEntry((void *)OBJ_TILES(obj), cells + i * 0x400, 0x400);
      }
      Resource_LoadAndDecompress((s32)&ResourceId_Data0C2, work->sheet, 0, 0);
    }
    scroll += 4;
    REG_BG2X = scroll << 8;
    EFFECT_PRESENT_CANVAS(work);
  }
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, source, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sprites, 0, 0);
  size = 3;
  for (i = 0; i != 10; i++) {
    for (j = 0; j != 18; j++) {
      s32 v = source[gBattleEffect_MotePictures[size - 1] + j];

      if (v > 0) {
        v = 63 - i * 7;
        if (v <= 0) {
          v = 1;
        }
      }
      sprites[i * 18 + 590 + j] = v;
    }
  }
  REG_DISPCNT = EFFECT_DISPCNT;
  REG_BG2PA = Q_8_8(1);
  REG_BLDALPHA = EFFECT_BLEND_CANVAS(14);
  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BG2X = 0;
  posX = 128;
  posY = 44;
  angle = 0;
  spin = 512;
  spread = 0;
  grow = 0;
  for (i = 0; i != 32; i++) {
    s32 q = i / 8;
    s32 left = 256;
    s32 t;
    s32 u2;

    t = (i - q * 8) << 5;
    u2 = q * 22;
    w->x[i] = t + left;
    w->y[i] = u2 + 128;
  }
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  draw[0] = Ram_HeapSlots->ramCode;
  for (frame = 0;
       frame != 300 && (!(gInput.repeat & (KEY_A | KEY_B)) || frame <= 16);
       frame++) {
    if (frame == 0) {
      Iwram_ClearWords((void *)BG_CHAR_BLOCK(1), CANVAS_BYTES);
      Iwram_ClearWordsEntry(canvas, CANVAS_BYTES);
      for (i = 0; i != 240; i++) {
        w->palette[i] = 0x7fff;
      }
      Iwram_CopyWordsEntry((void *)OBJ_PLTT, Data_02014260, 480);
      REG_BG2PA = Q_8_8(1);
      Graphics_LoadGreyRamp();
      REG_BG2X = -0x3c00;
    }
    if (frame > 285) {
      BattleEffect_StepPaletteToResource((s32)&ResourceId_CyanPalette);
    } else if (frame > 199) {
      BattleEffect_StepPaletteToResource((s32)&ResourceId_TanPalette);
    } else if (frame > 180) {
      BattleEffect_StepPaletteToResource((s32)&ResourceId_MarsDjinnSheet);
    }
    if (frame == 108) {
      REG_BG2PA = Q_8_8(0.5);
      REG_BG2X = 0;
      BattleEffect_DrawShadedCircles(source, 8, 0x10000);
      BattleEffect_DrawShadedCircles(Data_02010020, 16, 0xc000);
      BattleEffect_DrawShadedCircles(Data_020100a0, 32, 0xc000);
      BattleEffect_DrawShadedCircles(Data_020102a0, 48, 0xc000);
    }
    if ((u32)(frame - 223) <= 29) {
      s32 k = Random16() & 3;
      s32 n = gFlora_PetalSizes[k] * 2;

      Canvas_BlitAtFoot(source + gFlora_PetalPictures[k], 44, 78, n);
    }
    u = frame - 112;
    if (u <= 31) {
      u16 *e = Resource_GetTableEntry((s32)&ResourceId_GreenHairedMaidenTiles);

      for (i = 0; i != 240; i++, e++) {
        s32 c = w->palette[i];
        s32 b = (u16)c >> 10 & 31;
        s32 g = (u16)c >> 5 & 31;
        s32 r = c & 31;
        s32 t = *e;
        s32 tb = (u16)t >> 10 & 31;
        s32 tg = (u16)t >> 5 & 31;
        s32 tr = t & 31;

        if (b > tb) {
          b--;
        }
        if (g > tg) {
          g--;
        }
        if (r > tr) {
          r--;
        }
        w->palette[i] = RGB(r, g, b);
      }
      Iwram_CopyWordsEntry((void *)OBJ_PLTT, Data_02014260, 480);
    }
    if (u <= 7) {
      Iwram_FillWordsEntry(canvas, CANVAS_BYTES,
                           CANVAS_FILL_WORD(63) - u * 0x01010101 * 8);
    }
    req = gFlora_UnitScale;
    dy = 0;
    place[3] = 0;
    dx = 0;
    place[1] = px(255);
    if (frame <= 119) {
      posX = Trig_Sin(0xcd00) * 24 + 0x980000;
      posY = Trig_Cos(0xcd00) * 20 + 0x2c0000;
    } else if (frame <= 179) {
      s32 a = frame * 3 * 256 + -0x9800;

      posX = Trig_Sin(a) * 24 + 0x980000;
      posY = Trig_Cos(a) * 20 + 0x2c0000;
    } else if (frame <= 221) {
      posX += -px(9) / 8;
      posY += px(1) / 2;
    } else if (frame <= 251) {
      s32 m = frame % 6;
      s32 m4;

      if (m == 1) {
        dx = 1;
      }
      if (m == 2) {
        dx = 1;
      }
      if (m == 3) {
        dx = 2;
        dy = -1;
      }
      if (m == 4) {
        dx = 2;
        dy = -1;
      }
      if (m == 5) {
        dx = 2;
        dy = -1;
      }
      if (m == 0) {
        dx = 0;
        dy = 0;
      }
      if ((frame & 3) == 0) {
        gBgScroll[1].y = 30;
      }
      m4 = frame - frame / 4 * 4;
      if (m4 == 1) {
        gBgScroll[1].y = 30;
      }
      if (m4 == 2) {
        gBgScroll[1].y = 34;
      }
      if (m4 == 3) {
        gBgScroll[1].y = 34;
      }
    } else {
      gBgScroll[1].y = 32;
      posX += px(8);
      posY += -px(6);
    }
    if (frame > 111) {
      for (i = 0; i != 12; i++) {
        place[0] = posX + px(i % 3 * 32 + dx);
        place[2] = posY + px(i / 3 * 32 + dy);
        Render_ApplyProjectedPlacementFar(work->sprites[i], place, (s32 *)&req,
                                          0);
      }
    }
    for (i = 0; i != 32; i++) {
      s32 start = 211 + i * 2;

      if (i == 0) {
        if (frame == 0) {
          w->x[i] = px(240);
          w->y[i] = px(48);
        } else if (frame <= 47) {
          w->x[i] += -px(3);
          w->y[i] += px(3) / 2;
        } else if (frame <= 79) {
          w->x[i] += -px(1);
          w->y[i] += -px(1);
        } else if (frame <= 111) {
          w->x[i] += px(3);
          // DEAD: y moves by 0; the ROM keeps its load and store.
          w->y[i] += 0;
        }
        if (frame == 112) {
          w->x[i] = px(256);
        }
      } else if (i == 1) {
        if (frame <= 27) {
          w->x[i] = px(256);
          w->y[i] = px(48);
        } else if (frame == 28) {
          w->x[i] = px(16);
          w->y[i] = px(32);
        } else if (frame <= 62) {
          w->x[i] += 0x1d6d8;
          w->y[i] += 0x1fde8;
        } else if (frame <= 79) {
          w->x[i] += 0x19c40;
          w->y[i] += 0x6710;
        } else if (frame <= 141) {
          w->x[i] += px(3);
          // DEAD: y moves by 0; the ROM keeps its load and store.
          w->y[i] += 0;
        }
        if (frame == 112) {
          w->x[i] = px(256);
        }
      } else if (i == 2) {
        if (frame <= 49) {
          w->x[i] = px(256);
          w->y[i] = px(48);
        } else if (frame == 50) {
          w->x[i] = 0;
          w->y[i] = px(124);
        } else if (frame <= 62) {
          w->x[i] += px(4);
          w->y[i] += -px(5) / 4;
        } else if (frame <= 93) {
          w->x[i] += px(3);
          w->y[i] += px(3) / 4;
        } else if (frame <= 125) {
          w->x[i] += px(3);
          w->y[i] += -px(1);
        }
        if (frame == 112) {
          w->x[i] = px(256);
        }
      } else if (i == 3) {
        if (frame <= 49) {
          w->x[i] = px(256);
          w->y[i] = px(48);
        } else if (frame == 50) {
          w->x[i] = 0;
          w->y[i] = px(108);
        } else if (frame <= 103) {
          w->x[i] += px(3);
          w->y[i] += -px(5) / 4;
        } else if (frame <= 135) {
          w->x[i] += px(1);
          w->y[i] += px(1);
        }
        if (frame == 112) {
          w->x[i] = px(256);
        }
      } else if (i == 4) {
        if (frame <= 59) {
          w->x[i] = px(256);
          w->y[i] = px(48);
        } else if (frame == 60) {
          w->x[i] = px(16);
          w->y[i] = px(32);
        } else if (frame <= 82) {
          w->x[i] += px(4);
          w->y[i] += px(3);
        } else if (frame <= 135) {
          w->x[i] += px(4);
        }
        if (frame == 112) {
          w->x[i] = px(256);
        }
      } else if (i == 5) {
        if (frame <= 85) {
          w->x[i] = px(256);
          w->y[i] = px(48);
        } else if (frame == 86) {
          w->x[i] = px(144);
          w->y[i] = px(32);
        } else if (frame <= 135) {
          w->x[i] += px(1);
          w->y[i] += px(3);
        }
        if (frame == 112) {
          w->x[i] = px(256);
        }
      } else if (i <= 15) {
        if (frame == 0) {
          w->x[i] = px(248);
          w->y[i] = frame;
        } else if (frame == start) {
          w->x[i] = px(Random16() & 7) + px(92);
          w->y[i] = px(Random16() & 7) + px(104);
          w->vx[i] = (Random16() & TRIG_ANGLE_MASK) * 12 + -0xc0000;
          w->vy[i] = 0x40000;
        } else if (frame >= start) {
          if (w->x[i] >= -px(16) && w->y[i] <= px(168)) {
            w->x[i] += w->vx[i];
            w->y[i] += w->vy[i];
          } else if (frame <= 251) {
            w->x[i] = px(Random16() & 7) + px(92);
            w->y[i] = px(Random16() & 7) + px(104);
          }
        }
      } else if (i <= 31) {
        if (frame == 0) {
          w->x[i] = px(248);
          w->y[i] = frame;
        } else if (frame == 112) {
          u32 a = Random16() & TRIG_ANGLE_MASK;
          s32 s = ((Random16() & 255) << 1) + 256;

          w->x[i] = px(Random16() & 7) + px(168);
          w->y[i] = px(Random16() & 7) + px(92);
          w->vx[i] = Trig_Sin(a) * s / 256;
          w->vy[i] = Trig_Cos(a) * s / 256;
        } else if (frame == 252) {
          u32 a = Random16() & TRIG_ANGLE_MASK;
          s32 s = ((Random16() & 255) << 1) + 256;

          w->x[i] = px(Random16() & 7) + px(92);
          w->y[i] = px(Random16() & 7) + px(104);
          w->vx[i] = Trig_Sin(a) * s / 256;
          w->vy[i] = Trig_Cos(a) * s / 256;
        } else if (frame > 111) {
          w->x[i] += w->vx[i];
          w->y[i] += w->vy[i];
          w->vx[i] = w->vx[i] * 63 / 64;
          w->vy[i] = w->vy[i] * 63 / 64;
          if ((u32)(w->vx[i] + 0xffff) <= 0x1fffe &&
              (u32)(w->vy[i] + 0xffff) <= 0x1fffe) {
            w->x[i] = px(256);
            w->y[i] = 0;
          }
        }
      }
      if (i <= 5) {
        Data_02014002[14 + i] = (Data_02014002[14 + i] + 32) % 3840;
      } else {
        Data_02014002[14 + i] = (Data_02014002[14 + i] + 128) % 3840;
      }
      ((struct FloraObject *)work->sprites[12 + i])->tile =
          w->tiles[Data_02014002[14 + i] >> 8];
    }
    for (i = 0; i != 32; i++) {
      place[0] = w->x[i] - px(8);
      place[2] = w->y[i] - px(8);
      Render_ApplyProjectedPlacementFar(work->sprites[12 + i], place,
                                        (s32 *)&req, 0);
    }
    if (frame == 114) {
      Sound_PlayFar(SE_POWER_UP);
    }
    verts = Heap_AllocEwram(512);
    model = Render_AllocateModelList(1);
    texture.logWidth = 7;
    texture.logHeight = 7;
    texture.pixels = work->sheet;
    model->kind = 7;
    model->texture = &texture;
    model->mesh = gBattleEffect_LargeSquareMesh;
    model->vertices = verts;
    for (i = 0; i != 4; i++) {
      s32 d = frame - gFlora_BloomFrames[i];

      if (d >= 0) {
        s32 scale = d << 13;
        s32 sh = 0;

        if (d > 7) {
          sh = (8 - d) * 6;
        }
        model->shade = sh;
        if (sh > -64) {
          Transform_Reset();
          Transform_ApplyTranslation(px(gFlora_BloomX[i]), px(gFlora_BloomY[i]),
                                     0);
          Transform_ApplyAxisScale(Q16_ONE / 2, Q16_ONE, Q16_ONE / 2);
          Transform_ApplyRoll(gFlora_BloomRolls[i]);
          Transform_ApplyPitch(gFlora_BloomPitches[i]);
          Transform_ApplyUniformScale(scale);
          Transform_ApplyYaw(frame << 11);
          Render_TransformVertices(gBattleEffect_GroundSquareCorners, verts, 4);
          Render_DrawModelList(model);
        }
      }
    }
    Heap_Release(model);
    Heap_Release(verts);
    Transform_Reset();
    Transform_ApplyPitch(0x9400);
    Transform_ApplyYaw(0x6800);
    if (frame == 156) {
      spin = 512;
      angle = 0;
    }
    Transform_ApplyRoll(angle);
    if (frame == 156) {
      Sound_PlayFar(SE_SWIRL);
    }
    if (frame == 223) {
      Sound_PlayFar(SE_104);
    }
    if (frame == 252) {
      Sound_PlayFar(SE_POWER_UP);
    }
    if ((u32)(frame - 156) <= 74) {
      s32 r = 80 - (frame - 156) / 2;

      if (frame == 156) {
        spread = 0;
        grow = 0;
      }
      if (frame <= 194) {
        grow += 32;
        spread += 3;
      }
      spin += 16;
      angle += spin;
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                          BLITTER_ADDITIVE);
      draw[1] = Ram_HeapSlots->ramCodePaired;
      for (i = 0; i != 32; i++) {
        s32 a = (i / 4) << 13;
        s32 m = i % 4;
        s32 b = m * grow;
        s32 rr = r + m * spread / 30;
        s32 x;
        s32 y;

        size = 7;
        for (quarter = 0; quarter != 4; quarter++) {
          if (frame >= 208 + quarter * 4 && i % 4 == 3 - quarter) {
            s32 late = frame - quarter * 4 - 208;

            rr = r - late * 4;
            if (rr < 0) {
              rr = 0;
            }
            size = 7 - late;
            if (size <= 2) {
              size = 3;
            }
          }
        }
        pos.x = Q_16_16_TO_INT(Trig_Sin(a + b) * rr);
        pos.y = Q_16_16_TO_INT(Trig_Cos(a + b) * rr);
        pos.depth = 0;
        EffectPosition_ProjectPoint(&pos, &out);
        out.x >>= 1;
        x = out.x + 48;
        y = out.y + 92;
        draw[1](canvas, sprites + gBattleEffect_MotePictures[size - 1],
                x - size / 2, y - size, size, size * 2);
      }
      HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
    }
    if ((u32)(frame - 186) <= 79) {
      s32 v = frame - 186;
      s32 inner;
      s32 c;
      s32 ox;
      s32 outerY;
      s32 innerY;
      s32 sx;
      s32 sy;
      s32 x1;
      s32 y1;
      s32 x2;
      s32 y2;

      if (frame > 247) {
        v = 532 - frame * 2;
      } else if (v > 36) {
        v = 36;
      }
      inner = 64 - v;
      c = v;
      sx = 18 - c / 2;
      sy = 36 - c;
      dot[0] = c;
      dot[1] = c;
      Transform_ApplyRoll(frame << 10);
      if (frame <= 221) {
        Transform_ApplyPitch(sy << 10);
      }
      if (sx < 0) {
        sx = 0;
      }
      if (sy < 0) {
        sy = 0;
      }
      ox = sx + 48;
      outerY = 92 - sy;
      innerY = 88 - sy;
      for (i = 0; i != 128; i++) {
        ringPoint.x = Q_16_16_TO_INT(Trig_Sin(i << 9) * inner);
        ringPoint.y = Q_16_16_TO_INT(Trig_Cos(i << 9) * inner);
        ringPoint.depth = 0;
        EffectPosition_ProjectPoint(&ringPoint, &ringScreen);
        ringScreen.x >>= 1;
        x1 = ringScreen.x + ox;
        y1 = ringScreen.y + outerY;
        x2 = ringScreen.x * 3 / 4 + ox;
        y2 = ringScreen.y * 3 / 4 + innerY;
        draw[0](canvas, dot, x1 - 1, y1 - 1, 1, 2);
        draw[0](canvas, dot, x2 - 1, y2 - 1, 1, 2);
        if ((i & 31) == 0) {
          BattleEffect_DrawClippedCanvasLine(x1, y1, x2, y2, v);
          BattleEffect_DrawClippedCanvasLine(x1, y1 + 1, x2, y2 + 1, v);
        }
      }
      if ((u32)(frame - 228) <= 24) {
        s32 x1b;
        s32 y1b;
        s32 x2b;
        s32 y2b;

        c = 256 - frame;
        inner = frame - 204;
        dot[0] = c;
        dot[1] = c;
        for (i = 0; i != 128; i++) {
          fadeRingPoint.x = Q_16_16_TO_INT(Trig_Sin(i << 9) * inner);
          fadeRingPoint.y = Q_16_16_TO_INT(Trig_Cos(i << 9) * inner);
          fadeRingPoint.depth = 0;
          EffectPosition_ProjectPoint(&fadeRingPoint, &fadeRingScreen);
          fadeRingScreen.x >>= 1;
          x1b = ox + fadeRingScreen.x;
          y1b = fadeRingScreen.y + outerY;
          x2b = fadeRingScreen.x * 3 / 4 + ox;
          y2b = fadeRingScreen.y * 3 / 4 + innerY;
          draw[0](canvas, dot, x1b - 1, y1b - 1, 1, 2);
          draw[0](canvas, dot, x2b - 1, y2b - 1, 1, 2);
          if ((i & 31) == 0) {
            BattleEffect_DrawClippedCanvasLine(x1b, y1b, x2b, y2b, c);
            BattleEffect_DrawClippedCanvasLine(x1b, y1b + 1, x2b, y2b + 1, c);
          }
        }
      }
    }
    EFFECT_PRESENT_CANVAS(work);
  }
  Task_Remove(BattleEffect_AdvanceScrollOnInterval);
  gBgScroll[1].x = savedScroll;
  transition[4] = 0;
  BattleEffect_SetupBlendedDisplay();
  for (i = 0; i != 44; i++) {
    Sprite_DestroyFar(work->sprites[i]);
  }
  Palette_LoadBasicSpriteColors();
  Dma_Set((const void *)OBJ_PLTT, (void *)(BG_PLTT + 5 * 16), 0x80000010,
          (volatile u32 *)0x040000d4);
  *(u16 *)(BG_PLTT + 94) = *(u16 *)(BG_PLTT + 244);
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  REG_BG2X = 0;
  REG_BLDCNT = 0;
  REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  REG_BG2PA = Q_8_8(0.5);
  {
    /* FAKEMATCH(pin): e kept in r6. */
    register u8 *e asm("r6") =
        Resource_GetTableEntry((s32)&ResourceId_PinkPetalsTiles);
    s32 row;
    s32 a;
    s32 b;
    s32 k;
    s32 m;
    s32 mask;
    s32 t;
    u8 *sheet;
    s32 hi;
    /* FAKEMATCH(pin): c kept in r1. */
    register u32 c asm("r1");

    Iwram_CopyWordsEntry((void *)BG_PLTT, e, 32);
    e += 32;
    Decompress_TaggedLz(e, SheetOf(work));
    sheet = work->sheet + 0x1000;
    i = 0;
    mask = 15;
    for (row = 0; row != 15; row++) {
      for (a = 0; a != 2; a++) {
        for (b = 0; b != 2; b++) {
          s32 offset;

          t = row * 2;
          t += a;
          t <<= 4;
          t += b;
          k = 0;
          offset = t << 3;
          for (; k != 8; k++, offset += 16) {
            u8 *d = (u8 *)(offset + (s32)sheet);

            for (m = 0; m != 4; m++) {
              c = work->sheet[i];
              hi = c >> 4 & mask;
              d[0] = c & 15;
              d[1] = hi;
              i++;
              d += 2;
            }
          }
        }
      }
    }
  }
  for (frame = 0; frame != 128; frame++) {
    if (frame == 0) {
      p = (struct Particle *)source;
      for (i = 0; i != 128; i++, p++) {
        p->x = px((Random16() & 63) + 96);
        p->y = -(Random16() & 63) << 16;
        p->velocityX = -((Random16() & 255) + 128) << 7;
        p->velocityY = ((Random16() & 255) + 128) << 9;
        p->variant = Random16() & 31;
      }
      for (i = 0; i != args->targetCount; i++) {
        work->particles[i].variant = (Random16() & 15) + 1;
      }
    }
    p = (struct Particle *)source;
    for (i = 0; i != 64; i++, p++) {
      if (frame > i) {
        draw[0](canvas, work->sheet + 0x1000 + (p->variant / 4 % 15 << 8),
                Q_16_16_UPPER_HALF(p->x) - 4, Q_16_16_UPPER_HALF(p->y), 16, 16);
        BattleEffectKernels_IntegrateVector2(p, 64, 0);
        p->velocityX += -(px(1) / 16);
        p->variant++;
      }
    }
    if (frame == 56) {
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
    }
    if ((u32)(frame - 57) <= 50) {
      for (i = 0; i != args->targetCount; i++) {
        struct Particle *s = &work->particles[i];

        if (--s->variant == 0) {
          s->variant = 15;
          work->shakeFrames = 4;
          Sound_PlayFar(SE_IMPACT);
          BattleMotion_ApplyKnockbackFar(args->targets[i], KNOCKBACK_NUDGE);
          BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                   TINT_KEEP, i, 8);
        }
      }
    }
    BattleCamera_Shake(2, 2);
    EFFECT_END_FRAME(work);
  }
  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

/// BattleEffect_StartActorMotion with fixed values: unless the unit is one of
/// four enemies, the actor steps 15 pixels further from x = 0 on its
/// anchor's side, its y velocity growing by 0x28000 while y is under 10.
void BattleEffect_StartActorMotionOutward(s32 id) {
  struct BattleObject *slot;
  struct MotionObject *object;
  u32 enemy;
  s32 x;

  slot = BattleObject_GetFar(id);
  object = slot->object;
  enemy = Unit_Get(id)->enemyId;
  if (enemy != 357 && enemy != 369 && enemy != 373 && enemy != 376) {
    object->acceleration = px(2);
    object->speedLimit = px(8);
    if (object->y < px(10))
      object->velocityY += px(5) / 2;
    Actor_StartIdleFar(object);
    if (slot->anchorX > 0)
      x = object->x + px(15);
    else
      x = object->x - px(15);
    Actor_SetMoveTargetFar(object, x, 0, slot->anchorZ);
  }
  object->verticalMotionStrength = 0x9999;
  object->verticalMotionPhase = 0;
  object->autoFaceMotion = 0;
  Actor_SelectAnimationFar(object, 5);
}

#define DAGGER_FRAMES 96
#define DAGGER_COUNT 8       ///< of the 32 steps set up, the first 8 are drawn
#define DAGGER_FALL_FRAME 80 ///< the daggers turn and fall on the target
#define DAGGER_LAST_HIT 90

/// A dagger's texture: log2 of its width and height in the first word (32x32
/// here), then its pixels.
struct DaggerTexture {
  u32 logWidth : 8;
  u32 logHeight : 8;
  u32 rest : 16;
  void *pixels;
};

/// One entry of the model list Flora draws its dagger with.
struct RenderPrimitive {
  s32 kind;
  s32 unk04;
  const void *faces;
  void *vertices;
  struct DaggerTexture *texture;
  s32 unk14;
};

/// Crossed daggers. The canvas is moved over the target. Eight daggers rise
/// spinning from below it, and the target is struck eight times, six frames
/// apart from frame 20: each strike shakes the screen, flashes the target and
/// knocks it up. At frame 80 the daggers turn point down in a fan above the
/// target and plunge into it; the last strike, at frame 90, starts the
/// result phase.
///
/// Abilities: Sabre Dance.
void BattleEffect_RunSabreDance(struct BattleEffectParams *args) {
  struct BattleEffectWork *work = Ram_HeapSlots->battleEffect;
  struct DaggerTexture texture;
  struct ScreenPoint target;
  struct Particle *dagger;
  struct RenderPrimitive *quad;
  void *vertices;
  s32 frame;
  s32 i;
  s32 strikeFrame;

  BattleEffect_StartCanvasLayer(0);
  Resource_LoadAndDecompress((s32)&ResourceId_CrossedDaggersSheet, work->sheet,
                             1, 1);
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  REG_BLDCNT = 0;
  Task_Add((s32)BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  // Centre the canvas's x 64 on the target.
  BattleObject_GetFar(args->targets[0]);
  EffectPosition_ProjectBattler(args->targets[0], &target);
  REG_BG2PA = Q_8_8(1);
  REG_BG2X = (64 - target.x) << 8;

  for (frame = 0; frame != DAGGER_FRAMES; frame++) {
    // The first frame lines the daggers up below the canvas, each spinning
    // the other way from its neighbour.
    if (frame == 0) {
      s32 y;
      s32 zero;

      for (i = 0, zero = 0, y = px(128), dagger = work->particles; i != 32;
           i++, dagger++) {
        SET_POSITION(dagger, px((Random16() & 63) + 32), y, Random16());
        dagger->velocityX = zero;
        dagger->velocityY = -((Random16() & 31) + 32) << 10;
        dagger->velocityZ = (Random16() & 0x3ff) + 0x400;
        if (i & 1)
          dagger->velocityZ = -dagger->velocityZ;
        dagger->variant = zero;
        y += px(8);
      }
    }
    Transform_Reset();
    gProjection.centerY = 104;

    // The eight strikes.
    for (i = 0, strikeFrame = 20; i != 8; i++, strikeFrame += 6) {
      if (frame == strikeFrame) {
        work->shakeFrames = 4;
        Sound_PlayFar(SE_IMPACT);
        BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, 0, 6);
        BattleEffect_StartActorMotion(args->targets[0], 1, 0, 0, px(3), 100);
      }
    }
    if (frame == DAGGER_LAST_HIT) {
      work->shakeFrames = 4;
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                               POSE_DAMAGE, 0, 10);
      BattleEffect_StartActorMotion(args->targets[0], 1, 0, 0, px(3), 100);
    }

    // One textured quad, drawn once for each dagger.
    vertices = Heap_AllocEwram(32);
    quad = Render_AllocateModelList(1);
    quad->unk14 = 0;
    texture.logWidth = 5;
    texture.logHeight = 5;
    quad->texture = &texture;
    texture.pixels = work->sheet;
    quad->kind = 9;
    quad->faces = gSabreDance_DaggerMesh;
    quad->vertices = vertices;
    for (i = 0, dagger = work->particles; i != DAGGER_COUNT; i++, dagger++) {
      // The fall: from 48 pixels above the canvas, fanned out by angle and
      // closing on the middle.
      if (frame == DAGGER_FALL_FRAME) {
        s32 angle;
        s32 drift;

        dagger->y = -px(48);
        angle = (i << 10) + (Random16() & 0x3ff) - 0x1200;
        dagger->x = (angle << 10) + px(64);
        dagger->z = angle;
        drift = -angle << 5;
        dagger->velocityY = px(12);
        dagger->velocityX = drift;
        dagger->velocityZ = 0;
      }
      Transform_Reset();
      Transform_ApplyTranslation(dagger->x - px(64), dagger->y - px(64), 0);
      Transform_ApplyUniformScale(Q16_ONE / 2);
      Transform_ApplyRoll(dagger->z);
      Render_TransformVertices(gSabreDance_DaggerCorners, vertices, 4);
      Render_DrawModelList(quad);
      if (frame >= DAGGER_FALL_FRAME)
        BattleEffectKernels_IntegrateVector3(dagger, 64, 0);
      else
        BattleEffectKernels_IntegrateVector3(dagger, 64, -px(1) / 16);
    }
    Heap_Release(quad);
    Heap_Release(vertices);
    gProjection.centerX = BATTLE_PROJECTION_CENTER_X;
    gProjection.centerY = BATTLE_PROJECTION_CENTER_Y;

    BattleCamera_Shake(4, 4);
    EFFECT_END_FRAME(work);
  }

  Task_Remove((s32)BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

/// A sprite record as Flora sets its priority.
struct FxRecord {
  u8 unk00[9];
  u8 low : 2;
  u8 priority : 2;
  u8 high : 4;
};

/// Sets the OBJ priority of every motion record of the battle object, unless
/// the object is fading out.
void BattleObject_SetRecordPriority(s32 objectId, s32 priority) {
  struct BattleObject *slot = BattleObject_GetFar(objectId);
  struct FxRecord *record;
  s32 i;

  for (i = 0; (record = GetMotionRecordFar(slot->object, i)) != NULL; i++) {
    if (slot->fading == 0)
      record->priority = priority;
  }
}

#define STAR_FRAMES 60
#define STAR_LEAP_FRAME 16  ///< the caster leaps at the target
#define STAR_BURST_FRAME 32 ///< and strikes it
#define SMOKE_COUNT 32
#define STAR_COUNT 16
#define STAR_CELS 0x3e80 ///< the pink star's six 16x16 cels, after the smoke's

/// A pink star burst. Smoke puffs from the caster for its first 24 frames
/// while the caster stands just past the target; at frame 16 it leaps at
/// the target, and at frame 32 the palette turns pink, the caster lands
/// beside the target, which is knocked back and flashes, and sixteen pink
/// stars burst from it for 24 frames. Battle work byte 0x866 is set for the
/// whole effect.
///
/// Abilities: Backstab.
void BattleEffect_RunBackstab(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  u8 *canvas;
  struct ScreenPoint caster;
  struct ScreenPoint target;
  s32 frame;
  // Unused: draw[1] gives the stack frame its 4 bytes.
  DrawRectangle draw[2];
  struct MotionObject *targetObject;
  struct MotionObject *casterObject;
  struct BattleWork *battle;
  struct Particle *step;
  s32 i;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  Resource_LoadAndDecompress((s32)&ResourceId_PinkStarSheet,
                             work->sheet + STAR_CELS, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet, work->sheet, 1, 1);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  Task_Add((s32)BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  targetObject = BattleObject_GetFar(args->targets[0])->object;
  casterObject = BattleObject_GetFar(args->user)->object;
  battle = Ram_HeapSlots->battleWork;
  battle->keepObjectPriorities = 1;

  for (frame = 0; frame != STAR_FRAMES; frame++) {
    BattleObject_SetRecordPriority(args->user, 3);

    // The first frame sends the smoke out from the caster's feet.
    if (frame == 0) {
      EffectPosition_ProjectBattler(args->user, &caster);
      for (i = 0; i != SMOKE_COUNT; i++) {
        s32 angle;
        s32 speed;

        step = &work->particles[i];
        step->variant = 0;
        step->x = px(caster.x / 2);
        step->y = px(caster.y - 8);
        angle = Random16();
        speed = (Random16() & 255) + 127;
        step->velocityX = (Trig_Sin(angle) * speed) >> 5;
        step->velocityY = (Trig_Cos(angle) * speed) >> 5;
      }
      Sound_PlayFar(SE_WHOOSH);
    }

    // The smoke rises through its six 24x48 cels, slowing; the caster
    // starts 160 percent of the way out to the target.
    // DEAD: always true, as frame counts up from 0; the ROM keeps the test.
    if (frame >= 0) {
      struct Particle *puff;

      for (i = 0, puff = work->particles; i != 24; i++, puff++) {
        if (frame >= i / 8 && puff->variant <= 23) {
          draw[0](canvas,
                  work->sheet + (puff->variant / 4) * PICTURE_BYTES(SMOKE_PUFF),
                  Q_16_16_UPPER_HALF(puff->x) - 12,
                  Q_16_16_UPPER_HALF(puff->y) - 24, SMOKE_PUFF_WIDTH,
                  SMOKE_PUFF_HEIGHT);
          BattleEffectKernels_IntegrateVector2(puff, 48, -px(1) / 8);
          puff->variant++;
        }
      }
      if (frame == 0) {
        casterObject->z = targetObject->z;
        casterObject->x = targetObject->x * 160 / 100;
        casterObject->angle = targetObject->angle;
      }
    }

    if (frame == STAR_LEAP_FRAME) {
      BattleEffect_SetApproachMotion(args->user, args->targets[0], 16, 120);
      casterObject->velocityY = px(7);
    }

    // The strike: pink palette, the caster lands at 120 percent and the
    // target is pushed to 80, and the stars burst upward from it.
    if (frame == STAR_BURST_FRAME) {
      Iwram_CopyWordsEntry(
          (void *)BG_PLTT,
          Resource_GetTableEntry((s32)&ResourceId_PinkStarSheet), 128);
      casterObject->z = targetObject->z;
      casterObject->x = targetObject->x * 120 / 100;
      targetObject->x = targetObject->x * 80 / 100;
      casterObject->angle = targetObject->angle;
      EffectPosition_ProjectScaledBattler(args->targets[0], &target);
      for (i = 0, step = &work->particles[SMOKE_COUNT]; i != STAR_COUNT;
           i++, step++) {
        s32 angle;
        s32 speed;

        step->variant = (Random16() & 15) + 16;
        step->x = px(target.x / 2);
        step->y = px(target.y - 8);
        angle = (Random16() & 0x7fff) + TRIG_TURN / 4;
        speed = (Random16() & 255) + 127;
        step->velocityX = (Trig_Sin(angle) * speed) >> 7;
        step->velocityY = (Trig_Cos(angle) * speed) >> 5;
      }
      work->shakeFrames = 4;
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                               POSE_DAMAGE, 0, 16);
      BattleEffect_StartActorMotion(args->targets[0], 1, px(8), px(20), px(6),
                                    10);
    }

    // The stars twinkle through their six cels as they fly.
    if (frame >= STAR_BURST_FRAME && frame < STAR_BURST_FRAME + 24) {
      struct Particle *star;

      for (i = 0, star = &work->particles[SMOKE_COUNT]; i != STAR_COUNT;
           i++, star++) {
        if (star->variant >= 0) {
          draw[0](canvas,
                  work->sheet + STAR_CELS + (((frame + i) / 4) % 6) * 256,
                  Q_16_16_UPPER_HALF(star->x) - 8,
                  Q_16_16_UPPER_HALF(star->y) - 8, 16, 16);
          BattleEffectKernels_IntegrateVector2(star, 56, 0);
          star->variant--;
        }
      }
    }

    BattleCamera_Shake(4, 4);
    EFFECT_END_FRAME(work);
  }

  {
    s32 cleared = 0;

    battle->keepObjectPriorities = cleared;
  }
  Task_Remove((s32)BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  BattleEffect_EndCanvasLayer();
}
#undef BLITTER
#undef w
#undef OBJ_TILES
#undef DAGGER_FRAMES
#undef DAGGER_COUNT
#undef DAGGER_FALL_FRAME
#undef DAGGER_LAST_HIT
#undef STAR_FRAMES
#undef STAR_LEAP_FRAME
#undef STAR_BURST_FRAME
#undef SMOKE_COUNT
#undef STAR_COUNT
#undef STAR_CELS

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

/// Where FlashBurstSheet is decoded in the effect's pictures.
#define CATASTROPHE_FLASH_BURST_PICTURES 0x1800

/// Where Catastrophe decodes its pictures: JaggedStreakSheet in act one;
/// PentagonSealTexture after TriangleSealTexture, in the pictures in act one
/// and the map cell buffer in act two; SwirlTexture and then FireRingTexture in
/// the map cell buffer; and TornadoSheet (act two) or ClawSlashSheet (act
/// three), whose glints and slash the last acts sweep across.
#define CATASTROPHE_JAGGED_STREAKS 0x2000
#define CATASTROPHE_PENTAGON_SEAL 0x1000
#define CATASTROPHE_RING 0x2000
#define CATASTROPHE_SWEEP_PICTURES 0x800

/// Catastrophe's sprites, all 32x32: the chained demon (ChainedDemonTiles, its
/// last four drawn from pictures swapped in and out at frames 339 and 450), its
/// mirrored copy's six, the golden scales, and the eleven of the armour (the
/// purple armour's five, then the white spikes' six).
#define CATASTROPHE_DEMON 0
#define CATASTROPHE_DEMON_TILED 24
#define CATASTROPHE_DEMON_SWAPPED 24
#define CATASTROPHE_DEMON_SWAPPED_COUNT 4
#define CATASTROPHE_DEMON_COUNT 28
#define CATASTROPHE_MIRRORED_DEMON 28
#define CATASTROPHE_MIRRORED_DEMON_COUNT 6
#define CATASTROPHE_SCALES 34
#define CATASTROPHE_SCALES_COUNT 5
#define CATASTROPHE_ARMOUR 39
#define CATASTROPHE_PURPLE_ARMOUR_COUNT 5
#define CATASTROPHE_SPIKES 44
#define CATASTROPHE_SPIKES_COUNT 6
#define CATASTROPHE_ARMOUR_COUNT                                               \
  (CATASTROPHE_PURPLE_ARMOUR_COUNT + CATASTROPHE_SPIKES_COUNT)
#define CATASTROPHE_SPRITES 50

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
  FIXED camZ;
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
  struct Sprite *obj;
  struct ModelTexture aerialTexture;
  s16 list[UNIT_MAX];
  FIXED place[4];
  struct ScreenPoint pos;
  struct Scale req;
  struct ModelTexture texture;
  struct ModelTexture sweepTexture;
  struct Particle *p2;
  struct Particle *p3;
  struct Particle *p4;
  struct Particle *q2;
  u16 *backdropColor;
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
  ActorShadow_FreeTilesFar();
  EFFECT_START_NARROW_CANVAS(BGCNT_WRAP);
  BattleEffect_ConfigureEffectDisplay();
  PLTT_BACKDROP = 0;
  BG_PLTT_COLOR(0, 1) = 0;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
  blit[0] = Ram_HeapSlots->ramCode;
  work->transferMode = CANVAS_COPY;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  BattleEffect_WipeCanvas(CANVAS_WIPE_DOWN, CANVAS_WIPE_FILL);
  list[0] = BATTLE_UNIT_LIST_END;
  BattleActor_SpawnObjectsForListFar(list, 0);
  gProjection.centerY = SUMMON_PROJECTION_CENTER_Y;
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED, RESOURCE_RED_HAZE_BACKDROP,
                           0);
  Palette_BrightenSavedBattleBg(-RGB_LEVEL_MAX, -RGB_LEVEL_MAX, -RGB_LEVEL_MAX);
  transition[4] = 1;
  gBgScroll[1].x = 0;
  REG_BG2PA = Q_8_8(0.5);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
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

    e = Resource_GetTableEntry(RESOURCE_CHAINED_DEMON_TILES);
    Iwram_CopyWords((void *)OBJ_PLTT, e, 384);
    e += 384;
    Decompress_TaggedLz(e, cellBuffer);
    cells = Ram_MapCellBuffer;
    for (i = 0; i != CATASTROPHE_DEMON_TILED;
         cells += OBJ_32X32_8BPP_BYTES, i++) {
      obj = EffectSprite_CreateWithTiles(32, 32,
                                         OBJ_SIZE_32X32 | OBJ_256_COLORS, 0);
      obj->image.priority = 2;
      work->sprites[i] = obj;
      Iwram_CopyWords((void *)OBJ_TILES(obj), cells, OBJ_32X32_8BPP_BYTES);
    }
    {
      u8 *cells = Ram_MapCellBuffer + 0x6000;

      for (i = 0; i != CATASTROPHE_DEMON_SWAPPED_COUNT; i++) {
        obj = EffectSprite_Acquire(32, 32, OBJ_SIZE_32X32 | OBJ_256_COLORS, 0);
        work->sprites[CATASTROPHE_DEMON_SWAPPED + i] = obj;
        Iwram_CopyWords(obj, work->sprites[i], 24);
        Iwram_CopyWords(
            (void *)OBJ_TILES(work->sprites[CATASTROPHE_DEMON_SWAPPED + i]),
            cells + i * OBJ_32X32_8BPP_BYTES, OBJ_32X32_8BPP_BYTES);
      }
    }
    Iwram_CopyWords(cellBuffer + 0x1000, Ram_MapCellBuffer + 0x6000, 0x1000);
  }
  for (i = 0; i != CATASTROPHE_MIRRORED_DEMON_COUNT; i++) {
    obj = EffectSprite_Acquire(32, 32, OBJ_SIZE_32X32 | OBJ_256_COLORS, 0);
    work->sprites[CATASTROPHE_MIRRORED_DEMON + i] = obj;
    Iwram_CopyWords(obj, work->sprites[i], 24);
  }
  {
    u8 *cells;

    e = Resource_GetTableEntry(RESOURCE_GOLDEN_SCALES_TILES);
    Iwram_CopyWords((void *)&OBJ_PLTT_COLOR(12, 0), e, PALETTE_BANK_BYTES);
    e += PALETTE_BANK_BYTES;
    Decompress_TaggedLz(e, work->sheet);
    cells = work->sheet;
    for (i = 0; i != CATASTROPHE_SCALES_COUNT;
         cells += OBJ_32X32_4BPP_BYTES, i++) {
      obj =
          EffectSprite_CreateWithTiles(32, 32, OBJ_SIZE_32X32, OBJ_PALETTE(12));
      obj->image.priority = 1;
      work->sprites[CATASTROPHE_SCALES + i] = obj;
      Iwram_CopyWords((void *)OBJ_TILES(obj), cells, OBJ_32X32_4BPP_BYTES);
    }
  }
  {
    u8 *cells;

    e = Resource_GetTableEntry(RESOURCE_PURPLE_ARMOUR_TILES);
    Iwram_CopyWords((void *)&OBJ_PLTT_COLOR(13, 0), e, PALETTE_BANK_BYTES);
    e += PALETTE_BANK_BYTES;
    Decompress_TaggedLz(e, work->sheet);
    cells = work->sheet;
    for (i = 0; i != CATASTROPHE_PURPLE_ARMOUR_COUNT;
         cells += OBJ_32X32_4BPP_BYTES, i++) {
      obj =
          EffectSprite_CreateWithTiles(32, 32, OBJ_SIZE_32X32, OBJ_PALETTE(13));
      obj->image.priority = 1;
      work->sprites[CATASTROPHE_ARMOUR + i] = obj;
      Iwram_CopyWords((void *)OBJ_TILES(obj), cells, OBJ_32X32_4BPP_BYTES);
    }
  }
  {
    u8 *cells;

    e = Resource_GetTableEntry(RESOURCE_WHITE_SPIKES_TILES);
    Iwram_CopyWords((void *)&OBJ_PLTT_COLOR(14, 0), e, PALETTE_BANK_BYTES);
    e += PALETTE_BANK_BYTES;
    Decompress_TaggedLz(e, work->sheet);
    cells = work->sheet;
    for (i = 0; i != CATASTROPHE_SPIKES_COUNT;
         cells += OBJ_32X32_4BPP_BYTES, i++) {
      obj =
          EffectSprite_CreateWithTiles(32, 32, OBJ_SIZE_32X32, OBJ_PALETTE(14));
      obj->image.priority = 1;
      work->sprites[CATASTROPHE_SPIKES + i] = obj;
      Iwram_CopyWords((void *)OBJ_TILES(obj), cells, OBJ_32X32_4BPP_BYTES);
    }
  }
  Iwram_CopyWords(work->sheet + 0x5000, (void *)OBJ_PLTT, 480);
  Resource_LoadAndDecompress(RESOURCE_LIGHTNING_TEXTURE, work->sheet, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_JAGGED_STREAK_SHEET,
                             work->sheet + CATASTROPHE_JAGGED_STREAKS, 0, 0);
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry(RESOURCE_LIGHTNING_BOLT_SHEET),
                  SHEET_PALETTE_BYTES);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, sprites, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_SWIRL_TEXTURE,
                             Ram_MapCellBuffer + CATASTROPHE_RING, 0, 0);
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
      for (i = 3; i != ARRAY_COUNT(work->particles); i++) {
        work->particles[i].variant = -1;
      }
    }
    if (frame == 440) {
      REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
    }
    if (frame <= 369) {
      camZ = INTtoFIXED(frame * 334 / 379) + -0x11a0000;
    } else if ((u32)(d = frame - 370) <= 19) {
      camZ = Trig_Sin(d * 1638) * 9 + 0x2b0000;
    } else if ((u32)(frame - 390) <= 95) {
      camZ += -toFIXED(1) / 2;
    } else if ((u32)(frame - 486) <= 19) {
      camZ += -toFIXED(1);
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
      if ((u32)INTtoFIXED(v) > toFIXED(104)) {
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
      sway += -toFIXED(1) / 4;
    } else if ((u32)(frame - 472) <= 13) {
      drift = drift - ((frame - 472) << 12) + 120000;
      sway += 75536;
    } else if ((u32)(frame - 486) <= 8) {
      drift += 0x2000;
      sway -= Trig_Sin((frame << 7) + -0xf300);
    } else if ((u32)(frame - 554) <= 29) {
      sway += -toFIXED(1);
    } else if ((u32)(frame - 584) <= 9) {
      sway += -toFIXED(8);
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
        u16 *dst = (u16 *)&BG_PLTT_COLOR(6, 0);
        backdropColor = battle->palette;

        for (i = 0; i != BATTLE_BACKGROUND_COLORS; i++) {
          s32 r = RGB_RED(*backdropColor);
          s32 g = RGB_GREEN(*backdropColor);
          s32 b = RGB_BLUE(*backdropColor) + level;

          if (r > RGB_LEVEL_MAX) {
            r = RGB_LEVEL_MAX;
          }
          if (g > RGB_LEVEL_MAX) {
            g = RGB_LEVEL_MAX;
          }
          if (b > RGB_LEVEL_MAX) {
            b = RGB_LEVEL_MAX;
          }
          backdropColor++;
          *dst++ = RGB(r, g, b);
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
        fr = RGB_LEVEL_MAX;
        fg = RGB_LEVEL_MAX;
        fb = RGB_LEVEL_MAX;
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
            dst = (u16 *)&OBJ_PLTT_COLOR(0, 1);
          } else {
            dst = (u16 *)&BG_PLTT_COLOR(6, 0);
            src = battle->palette;
            n = 128;
          }
          for (i = 1; i != n; i++) {
            s32 r = RGB_RED(*src) + fr;
            s32 g = RGB_GREEN(*src) + fg;
            s32 b = RGB_BLUE(*src) + fb;

            if (r > RGB_LEVEL_MAX) {
              r = RGB_LEVEL_MAX;
            }
            if (g > RGB_LEVEL_MAX) {
              g = RGB_LEVEL_MAX;
            }
            if (b > RGB_LEVEL_MAX) {
              b = RGB_LEVEL_MAX;
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
            *dst++ = RGB(r, g, b);
            src++;
          }
        }
      }
    }
    if ((u32)(frame - 554) <= 31 && !((frame - 554) & 1)) {
      ColorBuffer_ScaleFar(battle->palette, (void *)&BG_PLTT_COLOR(6, 0),
                           0x10000 - ((frame - 554) << 10),
                           BATTLE_BACKGROUND_COLORS);
    }
    if (swapped == 0) {
      if (frame == 339) {
        swapped = 1;
        for (i = 0; i != CATASTROPHE_DEMON_SWAPPED_COUNT; i++) {
          void *tiles =
              (void *)OBJ_TILES(work->sprites[CATASTROPHE_DEMON_SWAPPED + i]);

          Iwram_CopyWords(tiles, cellBuffer + i * OBJ_32X32_8BPP_BYTES,
                          OBJ_32X32_8BPP_BYTES);
        }
      }
    } else if (frame == 450) {
      u8 *cells;

      swapped = 0;
      cells = Ram_MapCellBuffer + 0x1000;
      for (i = 0; i != CATASTROPHE_DEMON_SWAPPED_COUNT; i++) {
        Iwram_CopyWords(
            (void *)OBJ_TILES(work->sprites[CATASTROPHE_DEMON_SWAPPED + i]),
            cells + i * OBJ_32X32_8BPP_BYTES, OBJ_32X32_8BPP_BYTES);
      }
    }
    if (frame <= 593) {
      req = gCatastrophe_UnitScale;
      place[3] = 0;
      place[1] = toFIXED(255);
      {

        for (i = 0; i != CATASTROPHE_DEMON_COUNT; i++) {
          place[0] = INTtoFIXED(gCatastrophe_DemonX[i]) + toFIXED(8);
          place[2] = INTtoFIXED(gCatastrophe_DemonZ[i]) + camZ;
          if (place[2] > 0xfffff) {
            if (place[2] <= toFIXED(168)) {
              Render_ApplyProjectedPlacementFar(work->sprites[i], place,
                                                (s32 *)&req, 0);
            }
          }
        }
      }
      for (i = 0; i != CATASTROPHE_SCALES_COUNT; i++) {
        s32 h = i / 2;

        place[0] = INTtoFIXED((i - h * 2) * 32) + drift + toFIXED(72);
        place[2] = INTtoFIXED(h * 32) + camZ + sway + toFIXED(60);
        if (place[2] > 0xfffff) {
          if (place[2] <= toFIXED(168)) {
            Render_ApplyProjectedPlacementFar(
                work->sprites[CATASTROPHE_SCALES + i], place, (s32 *)&req, 0);
          }
        }
      }
      for (i = 0; i != CATASTROPHE_ARMOUR_COUNT; i++) {
        place[0] = INTtoFIXED(gCatastrophe_ArmourX[i]) - drift + toFIXED(132);
        place[2] =
            INTtoFIXED(gCatastrophe_ArmourZ[i]) + camZ + sway + toFIXED(29);
        if (place[2] > 0xfffff) {
          if (place[2] <= toFIXED(168)) {
            Render_ApplyProjectedPlacementFar(
                work->sprites[CATASTROPHE_ARMOUR + i], place, (s32 *)&req, 0);
          }
        }
      }
      req.x = -0x10001;
      for (i = 0; i != CATASTROPHE_MIRRORED_DEMON_COUNT; i++) {
        place[0] = INTtoFIXED(gCatastrophe_MirroredDemonX[i]) + toFIXED(8);
        place[2] = INTtoFIXED(gCatastrophe_MirroredDemonZ[i]) + camZ;
        if (place[2] > 0xfffff) {
          if (place[2] <= toFIXED(168)) {
            Render_ApplyProjectedPlacementFar(
                work->sprites[CATASTROPHE_MIRRORED_DEMON + i], place,
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
    model->kind = MODEL_SHADED_MAX;
    model->vertices = verts;
    for (i = 0; i != 7; i++) {
      s32 st = gCatastrophe_BurstStarts[i];

      if (frame >= st && frame < st + 8) {
        s32 c = gCatastrophe_BurstPictures[i];

        shade = (st - frame) << 3;
        model->shade = shade;
        texture.pixels = work->sheet + c * 0x600;
        Matrix_Initialize();
        Matrix_Translate(INTtoFIXED(gCatastrophe_BurstX[i] - 48),
                         INTtoFIXED(gCatastrophe_BurstY[i] - 44), 0);
        Matrix_RotateZ(0);
        Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE / 2);
        Render_TransformVertices(gBattleEffect_SquareCorners, verts, 4);
        Render_DrawModelList(model);
      }
    }
    if (frame == 489) {
      for (i = 0; i != 3; i++) {
        work->particles[i].variant = 0;
      }
    }
    for (j = 3; j != ARRAY_COUNT(work->particles); j++) {
      p = &work->particles[j];
      if (p->variant >= 0) {
        s32 n = (p->variant >> 3) + 1;

        blit[0](canvas, sprites + gBattleEffect_MotePictures[n - 1],
                FIXED_INTEGER_PART(p->x) - n / 2, FIXED_INTEGER_PART(p->y) - n,
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
    model->kind = MODEL_SHADED_MAX;
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
          p->velocityY = gCatastrophe_StrikeSpeedsY[p->variant + off];
          p->velocityZ = gCatastrophe_StrikeTurns[p->variant + off];
          p->velocityX = gCatastrophe_StrikePictures[p->variant + off];
          p->variant++;
          if (p->velocityX == -1) {
            continue;
          }
          w = 10;
          h = 20;
          src = sprites + gBattleEffect_MotePictures[9];
          x0 = p->x / 8 - 5;
          bx = span - drift;
          x = x0 + (FIXEDtoINT(bx / 2));
          y0 = p->y / 8 - 10;
          y = y0 + (FIXEDtoINT(camZ + sway));
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
          texture.pixels =
              work->sheet + CATASTROPHE_JAGGED_STREAKS + (cell << 12);
          Matrix_Initialize();
          {
            s32 t = p->x / 8;
            s32 b = span - drift;

            Matrix_Translate(INTtoFIXED(t - 64) + b / 2,
                             INTtoFIXED(p->y / 8 - 64) + camZ + sway, 0);
          }
          Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE);
          Matrix_RotateZ(p->z);
          Matrix_Scale(FIXED_ONE * 4, 85536, FIXED_ONE / 2);
          Render_TransformVertices(gBattleEffect_LongBeamCorners, verts, 4);
          Render_DrawModelList(model);
          p->y += p->velocityY * 2;
          p->z += p->velocityZ * 2;
        }
      }
    }
    if (frame == 602) {
      Iwram_FillWordsEntry(canvas, CANVAS_BYTES,
                           CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
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
      Matrix_Initialize();
      Matrix_Translate((span - drift) / 2 + toFIXED(4), camZ + toFIXED(204), 0);
      Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE);
      Matrix_RotateZ(p->y);
      Matrix_Scale(FIXED_ONE * 4, 85536, FIXED_ONE / 2);
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
      model->kind = MODEL_SHADED_MAX;
      model->shade = shade;
      for (i = 0; i != n; i++) {
        struct Particle *p4 = &work->particles[i];

        SET_POSITION(p4, INTtoFIXED(Random16() % spread - spread / 2),
                     camZ + toFIXED(200), ((base + i) % 3 << 12) + 0x2000);
        texture.pixels = work->sheet + p4->z;
        Matrix_Initialize();
        Matrix_Translate(toFIXED(1), p4->y, 0);
        Matrix_Scale(sx, FIXED_ONE, FIXED_ONE / 2);
        Matrix_Translate(p4->x, 0, 0);
        Matrix_Scale(FIXED_ONE / 2, FIXED_ONE * 2, FIXED_ONE);
        Matrix_RotateZ(DEGtoANG(180));
        Render_TransformVertices(gBattleEffect_LongBeamCorners, verts, 4);
        Render_DrawModelList(model);
      }
    }
    if (frame == 552) {
      Resource_LoadAndDecompress(RESOURCE_SWIRL_TEXTURE,
                                 Ram_MapCellBuffer + CATASTROPHE_RING, 0, 0);
    }
    if (frame == 551) {
      Resource_LoadAndDecompress(RESOURCE_TRIANGLE_SEAL_TEXTURE, work->sheet, 0,
                                 0);
      Resource_LoadAndDecompress(RESOURCE_PENTAGON_SEAL_TEXTURE,
                                 work->sheet + CATASTROPHE_PENTAGON_SEAL, 0, 0);
    }
    if (frame == 602) {
      Resource_LoadAndDecompress(RESOURCE_FIRE_RING_TEXTURE,
                                 Ram_MapCellBuffer + CATASTROPHE_RING, 0, 0);
    }
    if ((u32)(frame - 602) <= 6) {
      camZ += toFIXED(16);
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
        model->kind = MODEL_SHADED_ADDITIVE;
        model->texture = &texture;
        model->mesh = gBattleEffect_SquareMesh;
        model->vertices = verts;
        texture.pixels = work->sheet + CATASTROPHE_PENTAGON_SEAL;
        Matrix_Initialize();
        b = span - drift;
        Matrix_Translate(b / 2 + toFIXED(5), camZ + toFIXED(230), 0);
        Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE / 2);
        Matrix_RotateX(18768);
        a = -frame;
        a <<= 10;
        Matrix_RotateZ(a);
        Matrix_ScaleUniform(s);
        Matrix_Translate(0, -toFIXED(15), 0);
        Render_TransformVertices(gBattleEffect_SquareCorners, verts, 4);
        Matrix_Translate(0, toFIXED(15), 0);
        Render_DrawModelList(model);
        texture.pixels = work->sheet;
        for (i = 0; i != 3; i++) {
          Matrix_Push();
          Matrix_RotateZ(i * (DEGtoANG(120)) + DEGtoANG(60));
          {
            s32 phase = i * (DEGtoANG(120)) + DEGtoANG(60);
            model->shade = -24 - (FIXEDtoINT(Trig_Cos(phase + a) << 3));
          }
          Matrix_Translate(0, -toFIXED(16), 0);
          if (frame > 579) {
            s32 v = (frame - 580) * 1096;

            if (v > 19712) {
              v = 19712;
            }
            Matrix_RotateX(-v);
          }
          Render_TransformVertices(gBattleEffect_StandingSquareCorners, verts,
                                   4);
          Render_DrawModelList(model);
          Matrix_Pop();
        }
        if (frame > 589) {
          s = s + frame * 11060 + -0x6391d8;
        }
        if (s > 0 && frame <= 601) {
          shade = -16;
          if (frame > 589) {
            shade = shade - frame * 2 + 1180;
          }
          if (shade > MODEL_SHADE_INVISIBLE) {
            model->shade = shade;
            texture.pixels = Ram_MapCellBuffer + CATASTROPHE_RING;
            Matrix_Initialize();
            if (frame > 553) {
              s32 v = (554 - frame) * 120000;

              if (v < -0xcd140) {
                v = -0xcd140;
              }
              Matrix_Translate(0, v, 0);
            }
            Matrix_Translate(((b + (s32)((u32)b >> 31)) >> 1) + 0x50000,
                             camZ + toFIXED(220), 0);
            Matrix_Scale(FIXED_ONE, 32000, FIXED_ONE);
            Matrix_RotateZ(frame << 11);
            Matrix_ScaleUniform(s);
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
      if (shade > MODEL_SHADE_INVISIBLE) {
        model->shade = shade;
        model->mesh = gBattleEffect_LargeSquareMesh;
        model->kind = MODEL_SHADED_ADDITIVE;
        texture.pixels = Ram_MapCellBuffer + CATASTROPHE_RING;
        texture.logWidth = 7;
        texture.logHeight = 7;
        Matrix_Initialize();
        Matrix_Translate(toFIXED(1), -toFIXED(16), 0);
        Matrix_Scale(FIXED_ONE, 32000, FIXED_ONE);
        Matrix_RotateZ(frame << 10);
        Matrix_ScaleUniform(s);
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
  for (i = 0; i != CATASTROPHE_SPRITES; i++) {
    Sprite_DestroyFar(work->sprites[i]);
  }
  ActorShadow_LoadTilesFar();
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
  gProjection.centerY = SUMMON_PROJECTION_CENTER_Y;
  work->fadeFrames = 0;
  work->fadeStep = 0;
  // Act two: the land seen from the sky, where the orb falls and bursts.
  gBgScroll[1].y = BATTLE_BACKGROUND_SCROLL_Y;
  BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED,
                           RESOURCE_AERIAL_VIEW_BACKDROP, 0);
  Palette_BrightenSavedBattleBg(0, -16, -16);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  Resource_LoadAndDecompress(RESOURCE_LIGHTNING_BOLT_SHEET, cellBuffer, 1, 1);
  Resource_LoadAndDecompress(RESOURCE_TORNADO_SHEET,
                             work->sheet + CATASTROPHE_SWEEP_PICTURES, 1, 0);
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
  Resource_LoadAndDecompress(RESOURCE_TRIANGLE_SEAL_TEXTURE, cellBuffer, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_PENTAGON_SEAL_TEXTURE,
                             Ram_MapCellBuffer + CATASTROPHE_PENTAGON_SEAL, 0,
                             0);
  Resource_LoadAndDecompress(RESOURCE_FIRE_RING_TEXTURE,
                             Ram_MapCellBuffer + CATASTROPHE_RING, 0, 0);
  {
    FIXED rise;
    s32 top;
    void *aerialVertices;
    s32 aerialShade;
    s32 r;
    s32 ry;

    rise = 0;
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
    frame = 0;
    if (!(gInput.repeat & (KEY_A | KEY_B))) {
      do {
        if (frame == 0) {
          Iwram_FillWordsEntry(canvas, CANVAS_BYTES, 0);
          rise = -toFIXED(64);
          Palette_BrightenSavedBattleBg(0, -16, -16);
          EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
          Iwram_CopyWords((void *)BG_PLTT,
                          Resource_GetTableEntry(RESOURCE_VIOLET_PALETTE_F),
                          SHEET_PALETTE_BYTES);
        }
        if (rise <= 0xfffff) {
          rise += toFIXED(1);
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
            work->transferValue = CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST);
          } else {
            work->transferValue = CANVAS_FILL_WORD(16);
          }
          Palette_BrightenBgEntries(1, 1, 1);
        }
        if (frame == 80 || frame == 83) {
          u16 *c = (u16 *)&BG_PLTT_COLOR(6, 0);

          for (i = 0; i != BATTLE_BACKGROUND_COLORS; i++, c++) {
            *c ^= 0x7fff;
          }
        }
        if (frame == 82) {
          work->shakeFrames = 8;
          Sound_PlayFar(SE_BANE_ESCAPE);
          IWRAM_FILL_WORDS(canvas, CANVAS_BYTES,
                           CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
        }
        if (frame == 96) {
          Sound_PlayFar(SE_LAUNCH);
        }
        if (frame == 128) {
          u16 *c;

          work->shakeFrames = 16;
          c = (u16 *)&BG_PLTT_COLOR(6, 0);
          for (i = 0; i != BATTLE_BACKGROUND_COLORS; i++, c++) {
            *c ^= 0x7fff;
          }
        }
        if (frame > 128) {
          s32 d = (frame - 128) << 1;

          Palette_BrightenSavedBattleBg(d - 11, frame - 136, 3 - d);
        }
        r = 16;
        ry = FIXEDtoINT(rise);
        if (frame > 92) {
          r = frame - 75;
        }
        if (frame <= 79) {
          r += FIXEDtoINT(Trig_Sin(frame << 11) * 2);
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
              u32 flip;

              x -= gBattleEffect_GlintPictureWidths[n] >> 1;
              y = top + (Trig_Cos(angle) * r >> 17) -
                  (gBattleEffect_GlintPictureHeights[n] >> 1);
              flip = Random16();
              EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                                  gCatastrophe_GlintFlips[flip & 3] | 3, 0);
              ((DrawRectangle)Ram_HeapSlots->ramCodePaired)(
                  canvas,
                  work->sheet + CATASTROPHE_SWEEP_PICTURES +
                      gBattleEffect_GlintPictures[n],
                  x, y, gBattleEffect_GlintPictureWidths[n],
                  gBattleEffect_GlintPictureHeights[n]);
              HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
            }
          }
        }
        aerialVertices = Heap_AllocEwram(512);
        aerialModel = Render_AllocateModelList(1);
        if (frame <= 127) {
          s32 s = 85536;
          s32 a;

          if (frame > 71) {
            s = (frame << 11) + -0xf1e0;
          }
          aerialShade = -16;
          if (frame > 119) {
            aerialShade = 944 - frame * 8;
          }
          if (s > 0x22538) {
            s = 0x22538;
          }
          aerialTexture.logWidth = 6;
          aerialTexture.logHeight = 6;
          aerialModel->shade = aerialShade;
          aerialModel->kind = MODEL_SHADED_MAX;
          aerialModel->texture = &aerialTexture;
          aerialModel->mesh = gBattleEffect_SquareMesh;
          aerialModel->vertices = aerialVertices;
          aerialTexture.pixels = Ram_MapCellBuffer + CATASTROPHE_PENTAGON_SEAL;
          Matrix_Initialize();
          Matrix_Translate(-toFIXED(2), rise, 0);
          Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE / 2);
          Matrix_RotateX(18768);
          a = frame << 9;
          Matrix_RotateZ(a);
          Matrix_ScaleUniform(s);
          Matrix_Translate(0, -toFIXED(15), 0);
          Render_TransformVertices(gBattleEffect_SquareCorners, aerialVertices,
                                   4);
          Matrix_Translate(0, toFIXED(15), 0);
          Render_DrawModelList(aerialModel);
          aerialTexture.pixels = cellBuffer;
          for (i = 0; i != 3; i++) {
            s32 k = i * (DEGtoANG(120)) + DEGtoANG(60);
            s32 tone = aerialShade + (FIXEDtoINT(Trig_Cos(a + k) << 3)) - 16;
            s32 v;

            Matrix_Push();
            Matrix_RotateZ(i * (DEGtoANG(120)) + DEGtoANG(60));
            aerialModel->shade = tone;
            Matrix_Translate(0, -toFIXED(16), 0);
            v = 19712;
            if (frame > 63) {
              v = v - (frame << 10) + 0x10000;
            }
            if (v < 0) {
              v = 0;
            }
            Matrix_RotateX(-v);
            Render_TransformVertices(gBattleEffect_StandingSquareCorners,
                                     aerialVertices, 4);
            Render_DrawModelList(aerialModel);
            Matrix_Pop();
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
              aerialShade = 0;
              if (i == 0) {
                s = (t - start * 3) * 127 * 32 + 0x2000;
                if (frame >= d + 96) {
                  aerialShade = (start - frame) * 8 + 128;
                }
              } else {
                s = (t - start * 3) * 127 * 16 + 0x2000;
                if (frame >= d + 112) {
                  aerialShade = (start - frame) * 8 + 256;
                }
              }
              if (aerialShade > MODEL_SHADE_INVISIBLE) {
                aerialModel->shade = aerialShade;
                aerialModel->mesh = gBattleEffect_LargeSquareMesh;
                aerialModel->kind = MODEL_SHADED_MAX;
                aerialTexture.pixels = Ram_MapCellBuffer + CATASTROPHE_RING;
                aerialTexture.logWidth = 7;
                aerialTexture.logHeight = 7;
                Matrix_Initialize();
                Matrix_Translate(0, toFIXED(16), 0);
                Matrix_Scale(FIXED_ONE, 32000, FIXED_ONE);
                Matrix_RotateZ(frame << 10);
                Matrix_ScaleUniform(s);
                Render_TransformVertices(gBattleEffect_SquareCorners,
                                         aerialVertices, 4);
                Render_DrawModelList(aerialModel);
              }
            }
          }
        }
        aerialTexture.logWidth = 5;
        aerialTexture.logHeight = 7;
        aerialModel->kind = MODEL_SHADED_MAX;
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
            Matrix_Initialize();
            Matrix_RotateZ(p->z);
            p->z += p->velocityZ;
            Matrix_Scale(s << 2, s, FIXED_ONE);
            Render_TransformVertices(gBattleEffect_TaperedBeamCorners,
                                     aerialVertices, 4);
            Render_DrawModelList(aerialModel);
          }
        }
        Heap_Release(aerialModel);
        Heap_Release(aerialVertices);
        if (frame > 133) {
          if (frame == 134) {
            struct Particle *p;

            i = 0;
            p = &work->particles[16];
            for (; i != 48; i++, p++) {
              s32 a = Random16();
              s32 angle = i * 1365;
              s32 rr = (a & 31) + 32;

              SET_POSITION(p, (FIXEDtoINT(Trig_Sin(angle) * rr)) + 60,
                           (FIXEDtoINT(Trig_Cos(angle) * rr)) + 60,
                           (Random16() & 3) + 5);
              p->variant = -(Random16() & 15);
            }
          }
          i = 0;
          p3 = &work->particles[16];
          for (; i != 48; i++, p3++) {
            if ((u32)p3->variant <= 7) {
              s32 n = (FIXEDtoINT(Trig_Sin(p3->variant << 12) * p3->z)) + 1;

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
    FIXED slide;
    s32 slideStep;
    u8 *bolts;
    void *quads = Ram_MapCellBuffer + 0x1000;
    void *strip = Ram_MapCellBuffer + 0x2000;
    struct Particle *steps = ((struct Particle *)(Ram_MapCellBuffer + 0x6d00));
    struct Particle *debris =
        &((struct Particle *)(Ram_MapCellBuffer + 0x6d00))[64];

    // Act three: back on the battlefield, lightning and smoke sweep the foes.
    bolts = Ram_MapCellBuffer + 0x4000;
    slide = 0;
    slideStep = 0;
    Iwram_FillWords(canvas, CANVAS_BYTES,
                    CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
    EFFECT_PRESENT_CANVAS(work);
    gBgScroll[1].x = savedScroll;
    BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED, battle->background, 24);
    Palette_BrightenSavedBattleBg(-16, -16, -4);
    Resource_LoadAndDecompress(RESOURCE_SMOKE_BAND_SHEET, work->sheet, 1, 1);
    Resource_LoadAndDecompress(RESOURCE_CLAW_SLASH_SHEET,
                               work->sheet + CATASTROPHE_SWEEP_PICTURES, 1, 0);
    Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, sprites, 0, 0);
    Resource_LoadAndDecompress(RESOURCE_FLASH_BURST_SHEET,
                               work->sheet + CATASTROPHE_FLASH_BURST_PICTURES,
                               1, 0);
    Resource_LoadAndDecompress(RESOURCE_LIGHTNING_BOLT_SHEET, bolts, 1, 0);
    Render_BuildQuadStrip(Ram_MapCellBuffer + 0x2000, 4, 32, 11);
    REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
    frame = 0;
    do {
      if ((gInput.repeat & (KEY_A | KEY_B)) && frame > 2 && frame <= 87) {
        frame = 88;
      }
      if (frame == 88) {
        Iwram_FillWordsEntry(canvas, CANVAS_BYTES,
                             CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
        EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(2));
        gBgScroll[1].x = savedScroll;
        gProjection.centerX = BATTLE_PROJECTION_CENTER_X;
        work->scrollStepX = 0;
        EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      }
      if (frame == 0) {
        IWRAM_FILL_WORDS(canvas, CANVAS_BYTES,
                         CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
        work->scrollStepX = -16;
        gProjection.centerX = -968;
        gProjection.centerY = BATTLE_PROJECTION_CENTER_Y;
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
        BattleEffect_StepPaletteToResource(RESOURCE_VIOLET_PALETTE_F);
      }
      if (frame == 66) {
        for (i = 0; i != args->targetCount; i++) {
          BattleEffect_StartActorMotion(args->targets[i], 1, toFIXED(4),
                                        toFIXED(8), toFIXED(4), 150);
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

            q->x = INTtoFIXED(pos.x / 2);
            q->y = toFIXED(80);
            angle = (Random16() & 0x7fff) + DEGtoANG(90);
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

          q->x = INTtoFIXED(sum);
          q->y = INTtoFIXED(Random16() % 112 + 4);
          angle = (Random16() & 0x7fff) + DEGtoANG(180);
          speed = (Random16() & 127) + 128;
          q->velocityX = Trig_Sin(angle) * speed >> 6;
          q->velocityY = Trig_Cos(angle) * speed >> 6;
          q->variant = 0;
        }
        q2 = ((struct Particle *)(Ram_MapCellBuffer + 0x6d00));
        for (i = 0; i != 64; i++, q2++) {
          if ((u32)q2->variant <= 11) {
            blit[0](canvas,
                    work->sheet + CATASTROPHE_FLASH_BURST_PICTURES +
                        (q2->variant / 2 << 11),
                    FIXED_INTEGER_PART(q2->x) - 16,
                    FIXED_INTEGER_PART(q2->y) - 32, FLASH_BURST_WIDTH,
                    FLASH_BURST_HEIGHT);
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
            SET_POSITION(v,
                         x + (FIXEDtoINT(Trig_Sin((frame + j * 2) << 10) * 2)) +
                             jitter - 5,
                         y, z);
          }
          jitter = jitter + (Random16() & 7) - 4;
          y -= 6;
        }
        sweepModel = Render_AllocateModelList(1);
        sweepModel->kind = MODEL_SHADED_MAX;
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
          slideStep = slideStep * 48 / DAMPING_SCALE;
        }
        slide += slideStep;
        sweepTexture.logWidth = 6;
        sweepTexture.logHeight = 5;
        sweepTexture.pixels = work->sheet;
        sweepModel->mesh = strip;
        sweepModel->scrollU = (frame << 3) & 127;
        Matrix_Initialize();
        Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE / 2);
        a = frame * 3 * 127 * 16;
        b = frame << 13;
        c = Trig_Sin(a);
        Matrix_Translate(slide, (c << 5) + Trig_Cos(b) * 24, 0);
        Matrix_ScaleUniform(55536);
        Matrix_Push();
        Matrix_RotateZ(DEGtoANG(90));
        Matrix_ScaleUniform(0x42ee0);
        Render_TransformVertices(cellBuffer, quads, 24);
        Render_DrawModelList(sweepModel);
        Matrix_Pop();
        sweepTexture.logWidth = 6;
        sweepModel->mesh = gBattleEffect_SquareMesh;
        sweepTexture.logHeight = 6;
        sweepModel->scrollU = 0;
        sweepTexture.pixels = work->sheet + CATASTROPHE_SWEEP_PICTURES;
        Matrix_Translate(-0x20000, (Trig_Sin(frame << 10) << 2) + -0x60000, 0);
        Matrix_RotateZ(-TRIG_TURN / 32);
        {
          FIXED s0 = Trig_Sin(a);
          s32 y0 = (s0 << 5) + Trig_Cos(b) * 24;
          FIXED s1;
          FIXED y1;

          a += 6096;
          s1 = Trig_Sin(a);
          b += 0x2000;
          y1 = (s1 << 5) + Trig_Cos(b) * 24;
          Matrix_RotateZ((y0 - y1) / 512 + -0x400);
        }
        Matrix_Scale(0x21170, 105536, FIXED_ONE);
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

                  q->x = INTtoFIXED(p2->x);
                  q->y = toFIXED(104);
                  angle = (Random16() & 0x7fff) + DEGtoANG(90);
                  speed = (Random16() & 127) + 128;
                  q->velocityX = Trig_Sin(angle) * speed >> 5;
                  q->velocityY = Trig_Cos(angle) * speed >> 4;
                  q->variant = 16;
                }
              }
              if (frame >= lim && frame < lim + 8) {
                u8 *src = Ram_MapCellBuffer + 0x4000;

                blit[0](
                    canvas,
                    ((p2->variant / 2) & 3) * PICTURE_BYTES(LIGHTNING_BOLT) +
                        src,
                    p2->x - 12, 0, LIGHTNING_BOLT_WIDTH, LIGHTNING_BOLT_HEIGHT);
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
                        FIXED_INTEGER_PART(d->x) - n / 2,
                        FIXED_INTEGER_PART(d->y) - n, n, h);
                BattleEffectKernels_IntegrateVector2(d, 56, toFIXED(1) / 8);
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

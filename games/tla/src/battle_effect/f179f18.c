// Flora, the blossom spirit's summon.

#include "types.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "effect_step.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "heap_release.h"
#include "ram_buffer.h"

struct FxPartyList;
struct Model;
struct StripQuad;

struct ModelTexture {
  u32 logWidth : 8;
  u32 logHeight : 8;
  u32 unk02 : 16;
  u8 *pixels;
};

struct RenderModel {
  s32 kind;
  s32 unk04;
  const void *mesh;
  void *vertices;
  struct ModelTexture *texture;
  s32 shade;
  u8 scrollU;
  u8 scrollV;
};

void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_ProcessPendingGraphicsTransfer(void);
void BattleEffect_CountDownBattlerTints(void);
s32 BattleEffect_LoadBlitter(s32 slot,
                             s32 modeAndFlags); // BattleEffect_BuildBlitter
void Func_0814cd48(s32 actor, s32 palette, s32 pose, s32 timerSlot,
                   s32 timerFrames);              // BattleEffect_TintBattler
void Func_0815b434(u8 *map, s32 size, s32 scale); // shaded circles
void Func_0816ae40(u8 *src, u8 *dst, s32 width, s32 height);
void Func_0815585c(struct BattleEffectArgument *effect, u32 packed, s32 *screen,
                   s32 *world);
void Func_0818caa8(void *cells, s32 x, s32 y, s32 size);
struct PoolEntry *Render_AllocateModelList(s32 count);
void Render_TransformVertices(const void *corners, void *vertices, s32 count);
void Render_DrawModelList(struct Model *model);
void Func_08015160(s32 x, s32 y, s32 z);
void Func_0801521c(s32 scale);
void SceneTransform_ApplyPitch(s32 angle);
void SceneTransform_ApplyYaw(s32 angle);
void Render_ResetTransformState(void);
void Func_080151e4(s32 x, s32 y, s32 z); // SceneTransform_ApplyScale3
void EffectPosition_ProjectBattler(s32 id, struct EffectPosition *out);
void BattleEffect_SpawnObjects(s32 entryCount, s32 kind,
                               u32 variant); // SpawnObjects
void BattleEffect_SelectLivingTargets(
    struct FxPartyList *party); // SelectLivingTargets
void Func_080150e4(s32 angle);  // SceneTransform_ApplyRoll
void BattleEffect_StartActorMotion(s32 id, s32 force, s32 acceleration,
                                   s32 speedLimit, s32 velocityY, s32 percent);

void BattleEffect_FetchRectangleBlitters(s32 flip, void **out);
void Func_08118088(s32 actor, s32 motion); // BattleMotion_ApplyVariantMotionFar
void Func_081180e8(s32 phase);             // BattlePlayback_StartFar
void BattleCamera_Shake(s32 rangeX, u32 rangeY);
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 gravity,
                                          s32 drag);

void *Resource_GetTableEntry(s32 resource);

void Sound_PlayFar(s32 cue);
void wait(s32 frames);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);

void Render_BuildQuadStrip(struct StripQuad *quad, s32 width, s32 height,
                           s32 count);
void BattleEffectKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                          s32 gravity);
s32 BattleUnit_GetEffectHeightFar(s32 unit);
void Transform_SetLookAt(s32 *eye, s32 *target);
void BattleEffect_StartTiledCanvas(s32 bgControl);
void BattleEffect_StartTiledCanvasFilled(s32 bgControl);
void Transform_Push(void);
void Transform_Pop(void);

// The blitter's heap slot, as a byte offset into Ram_HeapSlots.
#define SLOT_BLITTER 104 // unk64[1]

#define BLITTER(mode, flags) ((mode) << 4 | (flags))
#define HI(v) (((s16 *)&(v))[1]) // the pixel half of a 16.16 value

#include "bg_scroll.h"
#include "dma.h"
#include "input.h"
#include "task.h"
#include "battle.h"
#include "motobj.h"
#include "resource.h"
struct ResourceTableEntry {
  u16 unk0;
  u16 tile;
};

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

struct WordPair {
  s32 a;
  s32 b;
};

extern struct ResourceTableEntry gResourceTableEntries[];
extern u8 gMapCellBuffer[];
extern u8 gData_02014000[];
extern u32 gCameraSceneParameters[];
extern const struct WordPair Data_08196ec8;
extern u16 gData_08197410[];
extern const u8 Data_08199421[];
extern const u16 Data_08199426[];
extern const u16 Data_0819942e[];
extern const s16 Data_08199436[];
extern const s16 Data_0819943e[];
extern const s8 Data_08199446[];
extern const s8 Data_0819944a[];
extern const u8 gData_08199210[];
extern const u8 gData_08199364[];
extern char ResourceId_VioletSkyBackdrop;
extern char ResourceId_GreenHairedMaidenTiles;
extern char ResourceId_PinkPetalsTiles;
extern char ResourceId_Data0C2;
extern char ResourceId_Data134;
extern char ResourceId_MarsDjinnSheet;
extern char ResourceId_CyanPalette;
extern char ResourceId_TanPalette;
extern u8 Data_02010020[];
extern u8 Data_020100a0[];
extern u8 Data_020102a0[];
extern u16 Data_0201401e[];
extern u16 Data_02014002[];
extern u16 Data_02014260[];

struct SpriteObject *Func_0815b290(s32 width, s32 height, u32 shape,
                                   u32 attributes);
struct FloraObject *Func_0815b3b0(s32 width, s32 height, u32 shape, s32 mode);

void BattleEffect_ConfigureEffectDisplay(void);
void Func_08179e6c(void);
void Func_0813baec(void);
void Func_0814cca8(void);
void BattleBackground_LoadFar(s32 layer, s32 background, s32 palette);
void Palette_LoadBasicObjColors(void);
void Graphics_LoadGreyRamp(void);
void Func_0815f0a0(s32 palette);
void Func_08143eb4(s32 x0, s32 y0, s32 x1, s32 y1, s32 color);

#define SLOT_ALT 188 // unkb8[1]
#define w ((struct FloraWork *)petals)
#define OBJ_TILES(obj)                                                         \
  (0x06010000 + gResourceTableEntries[(obj)->resource].tile)

static inline s32 ClearWords(void *destination, s32 size) {
  /* FAKEMATCH(wrapper): the VRAM clear through this inline; as a direct call
   * to Iwram_ClearWords, 16 of 2583 instructions of BattleEffect_RunFlora
   * differ in tla-en. */
  return Iwram_ClearWords(destination, size);
}

static inline u8 *SheetOf(struct BattleEffectWork *owner) {
  /* FAKEMATCH(wrapper): the sheet's address through this inline; as
   * work->sheet at the call, 56 of 2583 instructions of BattleEffect_RunFlora
   * differ in tla-en. */
  return owner->sheet;
}

/// Flora: petals swirl in on the wind, the blossom spirit rises among them,
/// her spiral of light and its rings sweep the field, and a storm of petals
/// strikes the foes.
void BattleEffect_RunFlora(struct BattleEffectArgument *effect) {
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
  s32 row2;
  s32 size;
  s32 dx;
  s32 dy;
  u32 u;
  s32 place[4];
  struct WordPair req;
  struct ModelTexture texture;
  struct RenderModel *model;
  void *verts;
  struct EffectStep *p;
  u8 *cells;
  u16 *phase;
  u8 *source;
  struct EffectPosition out;
  struct EffectPosition pos;
  u8 dot[2];
  struct EffectPosition out2;
  struct EffectPosition pos2;
  struct EffectPosition out3;
  struct EffectPosition pos3;

  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  savedScroll = gBgScroll[1].x;
  transition = (s32 *)Ram_HeapSlots->transitionWork;
  petals = (s32)&gData_02014000;
  source = Ram_MapCellBuffer;
  sprites = Ram_HeapSlots->unk64[0];
  BattleEffect_StartCanvasLayer(0);
  BattleEffect_ConfigureEffectDisplay();
  Func_08179e6c();
  work->transferMode = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 3200);
  scroll = -80;
  for (frame = 0; frame != 52; frame++) {
    if (frame == 27) {
      u8 *e;

      gCameraSceneParameters[4] = 240;
      BattleEffect_SelectLivingTargets(work->effect);
      BattleBackground_LoadFar(1, (s32)&ResourceId_VioletSkyBackdrop, 0);
      work->scrollTimer = 0;
      work->scrollInterval = 1;
      work->scrollStepX = -3;
      work->scrollStepY = 0;
      Task_Add(Func_0813baec, 3200);
      transition[4] = 1;
      e = Resource_GetTableEntry((s32)&ResourceId_PinkPetalsTiles);
      Iwram_CopyWords((void *)0x050003e0, e, 32);
      e += 32;
      Resource_DecodeType01(e, source);
      cells = Ram_MapCellBuffer;
      for (i = 0; i != 15; i++) {
        struct FloraObject *obj = Func_0815b290(16, 16, 0x40000000, 0xf000);
        work->objects[12 + i] = obj;
        obj->flags = 2;
        Iwram_CopyWords((void *)OBJ_TILES(obj), cells + i * 128, 128);
        w->tiles[i] = ((struct FloraObject *)work->objects[12 + i])->tile;
      }
      for (i = 0; i != 17; i++) {
        struct FloraObject *obj = Func_0815b3b0(16, 16, 0x40000000, 0xf000);
        work->objects[27 + i] = obj;
        Iwram_CopyWords(obj, work->objects[12], 24);
      }
      i = 0;
      phase = Data_0201401e;
      for (; i != 32; i++, phase++) {
        *phase = (Random16() % 15) << 8;
        ((struct FloraObject *)work->objects[12 + i])->tile =
            w->tiles[*phase / 256];
      }
      e = Resource_GetTableEntry((s32)&ResourceId_GreenHairedMaidenTiles);
      e += 480;
      Resource_DecodeType01(e, source);
      cells = Ram_MapCellBuffer;
      for (i = 0; i != 12; i++) {
        struct FloraObject *obj = Func_0815b290(32, 32, 0x80002000, 0);
        obj->flags = 2;
        work->objects[i] = obj;
        Iwram_CopyWords((void *)OBJ_TILES(obj), cells + i * 0x400, 0x400);
      }
      Resource_LoadAndDecompress((s32)&ResourceId_Data0C2, work->sheet, 0, 0);
    }
    scroll += 4;
    REG_BG2X = scroll << 8;
    work->transferPending = 1;
    wait(1);
  }
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, source, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
  size = 3;
  for (i = 0; i != 10; i++) {
    for (j = 0; j != 18; j++) {
      s32 v = source[gData_08197410[size - 1] + j];

      if (v > 0) {
        v = 63 - i * 7;
        if (v <= 0) {
          v = 1;
        }
      }
      sprites[i * 18 + 590 + j] = v;
    }
  }
  REG_DISPCNT = 0x7741;
  REG_BG2PA = 0x100;
  REG_BLDALPHA = 0x100e;
  REG_BLDCNT = 0x3f44;
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
  work->transferMode = 2;
  work->transferValue = 50;
  BattleEffect_LoadBlitter(SLOT_BLITTER, 19);
  draw[0] = Ram_HeapSlots->unk64[1];
  for (frame = 0; frame != 300 && (!(gInput.repeat & 3) || frame <= 16);
       frame++) {
    if (frame == 0) {
      ClearWords((void *)0x06004000, 0x4000);
      Iwram_ClearWords(canvas, 0x4000);
      for (i = 0; i != 240; i++) {
        w->palette[i] = 0x7fff;
      }
      Iwram_CopyWords((void *)OBJ_PLTT, Data_02014260, 480);
      REG_BG2PA = 0x100;
      Graphics_LoadGreyRamp();
      REG_BG2X = -0x3c00;
    }
    if (frame > 285) {
      Func_0815f0a0((s32)&ResourceId_CyanPalette);
    } else if (frame > 199) {
      Func_0815f0a0((s32)&ResourceId_TanPalette);
    } else if (frame > 180) {
      Func_0815f0a0((s32)&ResourceId_MarsDjinnSheet);
    }
    if (frame == 108) {
      REG_BG2PA = 0x80;
      REG_BG2X = 0;
      Func_0815b434(source, 8, 0x10000);
      Func_0815b434(Data_02010020, 16, 0xc000);
      Func_0815b434(Data_020100a0, 32, 0xc000);
      Func_0815b434(Data_020102a0, 48, 0xc000);
    }
    if ((u32)(frame - 223) <= 29) {
      s32 k = Random16() & 3;
      s32 n = Data_08199421[k] * 2;

      Func_0818caa8(source + Data_08199426[k], 44, 78, n);
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
        w->palette[i] = b << 10 | g << 5 | r;
      }
      Iwram_CopyWords((void *)OBJ_PLTT, Data_02014260, 480);
    }
    if (u <= 7) {
      Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f - u * 0x01010101 * 8);
    }
    req = Data_08196ec8;
    dy = 0;
    place[3] = 0;
    dx = 0;
    place[1] = 0xff0000;
    if (frame <= 119) {
      posX = Trig_Sin(0xcd00) * 24 + 0x980000;
      posY = Trig_Cos(0xcd00) * 20 + 0x2c0000;
    } else if (frame <= 179) {
      s32 a = frame * 3 * 256 + -0x9800;

      posX = Trig_Sin(a) * 24 + 0x980000;
      posY = Trig_Cos(a) * 20 + 0x2c0000;
    } else if (frame <= 221) {
      posX += -0x12000;
      posY += 0x8000;
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
      posX += 0x80000;
      posY += -0x60000;
    }
    if (frame > 111) {
      for (i = 0; i != 12; i++) {
        place[0] = posX + ((i % 3 * 32 + dx) << 16);
        place[2] = posY + ((i / 3 * 32 + dy) << 16);
        Render_ApplyProjectedPlacementFar(work->objects[i], place, (s32 *)&req,
                                          0);
      }
    }
    for (i = 0; i != 32; i++) {
      s32 start = 211 + i * 2;

      if (i == 0) {
        if (frame == 0) {
          w->x[i] = 0xf00000;
          w->y[i] = 0x300000;
        } else if (frame <= 47) {
          w->x[i] += -0x30000;
          w->y[i] += 0x18000;
        } else if (frame <= 79) {
          w->x[i] += -0x10000;
          w->y[i] += -0x10000;
        } else if (frame <= 111) {
          w->x[i] += 0x30000;
          // DEAD: y moves by 0; the ROM keeps its load and store.
          w->y[i] += 0;
        }
        if (frame == 112) {
          w->x[i] = 0x1000000;
        }
      } else if (i == 1) {
        if (frame <= 27) {
          w->x[i] = 0x1000000;
          w->y[i] = 0x300000;
        } else if (frame == 28) {
          w->x[i] = 0x100000;
          w->y[i] = 0x200000;
        } else if (frame <= 62) {
          w->x[i] += 0x1d6d8;
          w->y[i] += 0x1fde8;
        } else if (frame <= 79) {
          w->x[i] += 0x19c40;
          w->y[i] += 0x6710;
        } else if (frame <= 141) {
          w->x[i] += 0x30000;
          // DEAD: y moves by 0; the ROM keeps its load and store.
          w->y[i] += 0;
        }
        if (frame == 112) {
          w->x[i] = 0x1000000;
        }
      } else if (i == 2) {
        if (frame <= 49) {
          w->x[i] = 0x1000000;
          w->y[i] = 0x300000;
        } else if (frame == 50) {
          w->x[i] = 0;
          w->y[i] = 0x7c0000;
        } else if (frame <= 62) {
          w->x[i] += 0x40000;
          w->y[i] += -0x14000;
        } else if (frame <= 93) {
          w->x[i] += 0x30000;
          w->y[i] += 0xc000;
        } else if (frame <= 125) {
          w->x[i] += 0x30000;
          w->y[i] += -0x10000;
        }
        if (frame == 112) {
          w->x[i] = 0x1000000;
        }
      } else if (i == 3) {
        if (frame <= 49) {
          w->x[i] = 0x1000000;
          w->y[i] = 0x300000;
        } else if (frame == 50) {
          w->x[i] = 0;
          w->y[i] = 0x6c0000;
        } else if (frame <= 103) {
          w->x[i] += 0x30000;
          w->y[i] += -0x14000;
        } else if (frame <= 135) {
          w->x[i] += 0x10000;
          w->y[i] += 0x10000;
        }
        if (frame == 112) {
          w->x[i] = 0x1000000;
        }
      } else if (i == 4) {
        if (frame <= 59) {
          w->x[i] = 0x1000000;
          w->y[i] = 0x300000;
        } else if (frame == 60) {
          w->x[i] = 0x100000;
          w->y[i] = 0x200000;
        } else if (frame <= 82) {
          w->x[i] += 0x40000;
          w->y[i] += 0x30000;
        } else if (frame <= 135) {
          w->x[i] += 0x40000;
        }
        if (frame == 112) {
          w->x[i] = 0x1000000;
        }
      } else if (i == 5) {
        if (frame <= 85) {
          w->x[i] = 0x1000000;
          w->y[i] = 0x300000;
        } else if (frame == 86) {
          w->x[i] = 0x900000;
          w->y[i] = 0x200000;
        } else if (frame <= 135) {
          w->x[i] += 0x10000;
          w->y[i] += 0x30000;
        }
        if (frame == 112) {
          w->x[i] = 0x1000000;
        }
      } else if (i <= 15) {
        if (frame == 0) {
          w->x[i] = 0xf80000;
          w->y[i] = frame;
        } else if (frame == start) {
          w->x[i] = ((Random16() & 7) << 16) + 0x5c0000;
          w->y[i] = ((Random16() & 7) << 16) + 0x680000;
          w->vx[i] = (Random16() & 0xffff) * 12 + -0xc0000;
          w->vy[i] = 0x40000;
        } else if (frame >= start) {
          if (w->x[i] >= -0x100000 && w->y[i] <= 0xa80000) {
            w->x[i] += w->vx[i];
            w->y[i] += w->vy[i];
          } else if (frame <= 251) {
            w->x[i] = ((Random16() & 7) << 16) + 0x5c0000;
            w->y[i] = ((Random16() & 7) << 16) + 0x680000;
          }
        }
      } else if (i <= 31) {
        if (frame == 0) {
          w->x[i] = 0xf80000;
          w->y[i] = frame;
        } else if (frame == 112) {
          u32 a = Random16() & 0xffff;
          s32 s = ((Random16() & 255) << 1) + 256;

          w->x[i] = ((Random16() & 7) << 16) + 0xa80000;
          w->y[i] = ((Random16() & 7) << 16) + 0x5c0000;
          w->vx[i] = Trig_Sin(a) * s / 256;
          w->vy[i] = Trig_Cos(a) * s / 256;
        } else if (frame == 252) {
          u32 a = Random16() & 0xffff;
          s32 s = ((Random16() & 255) << 1) + 256;

          w->x[i] = ((Random16() & 7) << 16) + 0x5c0000;
          w->y[i] = ((Random16() & 7) << 16) + 0x680000;
          w->vx[i] = Trig_Sin(a) * s / 256;
          w->vy[i] = Trig_Cos(a) * s / 256;
        } else if (frame > 111) {
          w->x[i] += w->vx[i];
          w->y[i] += w->vy[i];
          w->vx[i] = w->vx[i] * 63 / 64;
          w->vy[i] = w->vy[i] * 63 / 64;
          if ((u32)(w->vx[i] + 0xffff) <= 0x1fffe &&
              (u32)(w->vy[i] + 0xffff) <= 0x1fffe) {
            w->x[i] = 0x1000000;
            w->y[i] = 0;
          }
        }
      }
      if (i <= 5) {
        Data_02014002[14 + i] = (Data_02014002[14 + i] + 32) % 3840;
      } else {
        Data_02014002[14 + i] = (Data_02014002[14 + i] + 128) % 3840;
      }
      ((struct FloraObject *)work->objects[12 + i])->tile =
          w->tiles[Data_02014002[14 + i] >> 8];
    }
    for (i = 0; i != 32; i++) {
      place[0] = w->x[i] + -0x80000;
      place[2] = w->y[i] + -0x80000;
      Render_ApplyProjectedPlacementFar(work->objects[12 + i], place,
                                        (s32 *)&req, 0);
    }
    if (frame == 114) {
      Sound_PlayFar(212);
    }
    verts = Heap_AllocEwram(512);
    model = Render_AllocateModelList(1);
    texture.logWidth = 7;
    texture.logHeight = 7;
    texture.pixels = work->sheet;
    model->kind = 7;
    model->texture = &texture;
    model->mesh = gData_08199364;
    model->vertices = verts;
    for (i = 0; i != 4; i++) {
      s32 d = frame - Data_0819942e[i];

      if (d >= 0) {
        s32 scale = d << 13;
        s32 sh = 0;

        if (d > 7) {
          sh = (8 - d) * 6;
        }
        model->shade = sh;
        if (sh > -64) {
          Render_ResetTransformState();
          Func_08015160(Data_08199446[i] << 16, Data_0819944a[i] << 16, 0);
          Func_080151e4(0x8000, 0x10000, 0x8000);
          Func_080150e4(Data_08199436[i]);
          SceneTransform_ApplyPitch(Data_0819943e[i]);
          Func_0801521c(scale);
          SceneTransform_ApplyYaw(frame << 11);
          Render_TransformVertices(gData_08199210, verts, 4);
          Render_DrawModelList(model);
        }
      }
    }
    Heap_Release(model);
    Heap_Release(verts);
    Render_ResetTransformState();
    SceneTransform_ApplyPitch(0x9400);
    SceneTransform_ApplyYaw(0x6800);
    if (frame == 156) {
      spin = 512;
      angle = 0;
    }
    Func_080150e4(angle);
    if (frame == 156) {
      Sound_PlayFar(142);
    }
    if (frame == 223) {
      Sound_PlayFar(104);
    }
    if (frame == 252) {
      Sound_PlayFar(212);
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
      BattleEffect_LoadBlitter(SLOT_ALT, 3);
      draw[1] = Ram_HeapSlots->unkb8[1];
      for (i = 0; i != 32; i++) {
        s32 a = (i / 4) << 13;
        s32 m = i % 4;
        s32 b = m * grow;
        s32 rr = r + m * spread / 30;
        s32 x;
        s32 y;

        size = 7;
        for (row2 = 0; row2 != 4; row2++) {
          if (frame >= 208 + row2 * 4 && i % 4 == 3 - row2) {
            s32 late = frame - row2 * 4 - 208;

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
        pos.x = Trig_Sin(a + b) * rr >> 16;
        pos.y = Trig_Cos(a + b) * rr >> 16;
        pos.depth = 0;
        EffectPosition_ProjectPoint(&pos, &out);
        out.x >>= 1;
        x = out.x + 48;
        y = out.y + 92;
        draw[1](canvas, sprites + gData_08197410[size - 1], x - size / 2,
                y - size, size, size * 2);
      }
      HeapSlot_Release(SLOT_ALT);
    }
    if ((u32)(frame - 186) <= 79) {
      s32 v = frame - 186;
      s32 inner;
      s32 c;
      s32 ox;
      s32 oy1;
      s32 oy2;
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
      Func_080150e4(frame << 10);
      if (frame <= 221) {
        SceneTransform_ApplyPitch(sy << 10);
      }
      if (sx < 0) {
        sx = 0;
      }
      if (sy < 0) {
        sy = 0;
      }
      ox = sx + 48;
      oy1 = 92 - sy;
      oy2 = 88 - sy;
      for (i = 0; i != 128; i++) {
        pos2.x = Trig_Sin(i << 9) * inner >> 16;
        pos2.y = Trig_Cos(i << 9) * inner >> 16;
        pos2.depth = 0;
        EffectPosition_ProjectPoint(&pos2, &out2);
        out2.x >>= 1;
        x1 = out2.x + ox;
        y1 = out2.y + oy1;
        x2 = out2.x * 3 / 4 + ox;
        y2 = out2.y * 3 / 4 + oy2;
        draw[0](canvas, dot, x1 - 1, y1 - 1, 1, 2);
        draw[0](canvas, dot, x2 - 1, y2 - 1, 1, 2);
        if ((i & 31) == 0) {
          Func_08143eb4(x1, y1, x2, y2, v);
          Func_08143eb4(x1, y1 + 1, x2, y2 + 1, v);
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
          pos3.x = Trig_Sin(i << 9) * inner >> 16;
          pos3.y = Trig_Cos(i << 9) * inner >> 16;
          pos3.depth = 0;
          EffectPosition_ProjectPoint(&pos3, &out3);
          out3.x >>= 1;
          x1b = ox + out3.x;
          y1b = out3.y + oy1;
          x2b = out3.x * 3 / 4 + ox;
          y2b = out3.y * 3 / 4 + oy2;
          draw[0](canvas, dot, x1b - 1, y1b - 1, 1, 2);
          draw[0](canvas, dot, x2b - 1, y2b - 1, 1, 2);
          if ((i & 31) == 0) {
            Func_08143eb4(x1b, y1b, x2b, y2b, c);
            Func_08143eb4(x1b, y1b + 1, x2b, y2b + 1, c);
          }
        }
      }
    }
    work->transferPending = 1;
    wait(1);
  }
  Task_Remove(Func_0813baec);
  gBgScroll[1].x = savedScroll;
  transition[4] = 0;
  Func_0814cca8();
  for (i = 0; i != 44; i++) {
    ResourceObject_ReleaseFar(work->objects[i]);
  }
  Palette_LoadBasicObjColors();
  Dma_Set((const void *)OBJ_PLTT, (void *)0x050000a0, 0x80000010,
          (volatile u32 *)0x040000d4);
  *(u16 *)0x050000bc = *(u16 *)0x050001e8;
  work->transferMode = 2;
  work->transferValue = 50;
  work->transferMode = 1;
  work->transferValue = 0;
  REG_BG2X = 0;
  REG_BLDCNT = 0;
  REG_BG2CNT = 0x784;
  REG_BLDALPHA = 0x1010;
  REG_BG2PA = 0x80;
  {
    /* FAKEMATCH(pin): e kept in r6; as a plain u8 *, 61 of 2583 instructions
     * differ in tla-en. */
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
    /* FAKEMATCH(pin): c kept in r1; as a plain u32, 641 of 2583 instructions
     * differ in tla-en. */
    register u32 c asm("r1");

    Iwram_CopyWords((void *)BG_PLTT, e, 32);
    e += 32;
    Resource_DecodeType01(e, SheetOf(work));
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
      p = (struct EffectStep *)source;
      for (i = 0; i != 128; i++, p++) {
        p->x = ((Random16() & 63) + 96) << 16;
        p->y = -(Random16() & 63) << 16;
        p->velocityX = -((Random16() & 255) + 128) << 7;
        p->velocityY = ((Random16() & 255) + 128) << 9;
        p->variant = Random16() & 31;
      }
      for (i = 0; i != effect->count; i++) {
        work->particles[i].variant = (Random16() & 15) + 1;
      }
    }
    p = (struct EffectStep *)source;
    for (i = 0; i != 64; i++, p++) {
      if (frame > i) {
        draw[0](canvas, work->sheet + 0x1000 + (p->variant / 4 % 15 << 8),
                HI(p->x) - 4, HI(p->y), 16, 16);
        BattleEffectKernels_IntegrateVector2(p, 64, 0);
        p->velocityX += -0x1000;
        p->variant++;
      }
    }
    if (frame == 56) {
      Func_081180e8(134);
    }
    if ((u32)(frame - 57) <= 50) {
      for (i = 0; i != effect->count; i++) {
        struct EffectStep *s = &work->particles[i];

        if (--s->variant == 0) {
          s->variant = 15;
          work->shakeFrames = 4;
          Sound_PlayFar(133);
          Func_08118088(effect->actors[i], 0);
          Func_0814cd48(effect->actors[i], 7, -1, i, 8);
        }
      }
    }
    BattleCamera_Shake(2, 2);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(SLOT_BLITTER);
  BattleEffect_EndCanvasLayer();
}

#include "battle_action.h"
#include "battle_unit.h"
#include "actor.h"

void *Unit_Get(s32 owner);

/// BattleEffect_StartActorMotion with fixed values: unless the unit is one of
/// four enemies, the actor steps 15 pixels further from x = 0 on its
/// anchor's side, its y velocity growing by 0x28000 while y is under 10.
void BattleEffect_StartActorMotionOutward(s32 id) {
  struct BattleObj *slot;
  struct MotionObject *object;
  u32 enemy;
  s32 x;

  slot = GetBattleObjectSlotFar(id);
  object = slot->object;
  enemy = ((struct BattleUnit *)Unit_Get(id))->enemyId;
  if (enemy != 357 && enemy != 369 && enemy != 373 && enemy != 376) {
    object->acceleration = 0x20000;
    object->speedLimit = 0x80000;
    if (object->y < 0xa0000)
      object->velocityY += 0x28000;
    Actor_ResetMotion(object);
    if (slot->anchorX > 0)
      x = object->x + 0xf0000;
    else
      x = object->x - 0xf0000;
    Actor_SetTargetPosition(object, x, 0, slot->anchorZ);
  }
  object->verticalMotionStrength = 0x9999;
  object->verticalMotionPhase = 0;
  object->autoFaceMotion = 0;
  Actor_SetMode(object, 5);
}

#include "graphics.h"

#define DAGGER_FRAMES 96
#define DAGGER_COUNT 8       // of the 32 steps set up, the first 8 are drawn
#define DAGGER_FALL_FRAME 80 // the daggers turn and fall on the target
#define DAGGER_LAST_HIT 90

// A dagger's texture: log2 of its width and height in the first word (32x32
// here), then its pixels.
struct DaggerTexture {
  u32 logWidth : 8;
  u32 logHeight : 8;
  u32 rest : 16;
  void *pixels;
};

// One entry of the list Render_DrawModelList draws; kind 9 is a textured quad.
// Render_AllocateModelList allocates the list with an empty entry after it.
struct RenderPrimitive {
  s32 kind;
  s32 unk04;
  const void *faces;
  void *vertices;
  struct DaggerTexture *texture;
  s32 unk14;
};

extern char ResourceId_CrossedDaggersSheet;

// The dagger quad: its face list and its four corners, 32x80 pixels from
// (-16, -16) to (16, 64), the point down.
extern const u8 gData_08199450[];
extern const u8 gData_08199474[];

// Crossed daggers. The canvas is moved over the target. Eight daggers rise
// spinning from below it, and the target is struck eight times, six frames
// apart from frame 20: each strike shakes the screen, flashes the target and
// knocks it up. At frame 80 the daggers turn point down in a fan above the
// target and plunge into it; the last strike, at frame 90, starts the
// result phase.
void BattleEffect_RunSabreDance(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work =
      (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  struct DaggerTexture texture;
  struct EffectPosition target;
  struct EffectStep *dagger;
  struct RenderPrimitive *quad;
  void *vertices;
  s32 frame;
  s32 i;
  s32 strikeFrame;

  BattleEffect_StartCanvasLayer(0);
  Resource_LoadAndDecompress((s32)&ResourceId_CrossedDaggersSheet, work->sheet,
                             1, 1);
  work->transferMode = 1;
  work->transferValue = 0;
  REG_BLDCNT = 0;
  Task_Add((s32)BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  // Centre the canvas's x 64 on the target.
  GetBattleObjectSlotFar(effect->actors[0]);
  EffectPosition_ProjectBattler(effect->actors[0], &target);
  REG_BG2PA = 0x100;
  REG_BG2X = (64 - target.x) << 8;

  for (frame = 0; frame != DAGGER_FRAMES; frame++) {
    // The first frame lines the daggers up below the canvas, each spinning
    // the other way from its neighbour.
    if (frame == 0) {
      s32 y;
      s32 zero;

      for (i = 0, zero = 0, y = 128 << 16, dagger = work->particles; i != 32;
           i++, dagger++) {
        dagger->x = ((Random16() & 63) + 32) << 16;
        dagger->y = y;
        dagger->z = Random16();
        dagger->velocityX = zero;
        dagger->velocityY = -((Random16() & 31) + 32) << 10;
        dagger->velocityZ = (Random16() & 0x3ff) + 0x400;
        if (i & 1)
          dagger->velocityZ = -dagger->velocityZ;
        dagger->variant = zero;
        y += 8 << 16;
      }
    }
    Render_ResetTransformState();
    gCameraSceneParameters[4] = 104;

    // The eight strikes.
    for (i = 0, strikeFrame = 20; i != 8; i++, strikeFrame += 6) {
      if (frame == strikeFrame) {
        work->shakeFrames = 4;
        Sound_PlayFar(133);
        Func_0814cd48(effect->actors[0], 7, 5, 0, 6);
        BattleEffect_StartActorMotion(effect->actors[0], 1, 0, 0, 3 << 16, 100);
      }
    }
    if (frame == DAGGER_LAST_HIT) {
      work->shakeFrames = 4;
      Func_081180e8(134);
      Func_0814cd48(effect->actors[0], 7, 5, 0, 10);
      BattleEffect_StartActorMotion(effect->actors[0], 1, 0, 0, 3 << 16, 100);
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
    quad->faces = gData_08199450;
    quad->vertices = vertices;
    for (i = 0, dagger = work->particles; i != DAGGER_COUNT; i++, dagger++) {
      // The fall: from 48 pixels above the canvas, fanned out by angle and
      // closing on the middle.
      if (frame == DAGGER_FALL_FRAME) {
        s32 angle;
        s32 drift;

        dagger->y = -48 << 16;
        angle = (i << 10) + (Random16() & 0x3ff) - 0x1200;
        dagger->x = (angle << 10) + (64 << 16);
        dagger->z = angle;
        drift = -angle << 5;
        dagger->velocityY = 12 << 16;
        dagger->velocityX = drift;
        dagger->velocityZ = 0;
      }
      Render_ResetTransformState();
      Func_08015160(dagger->x - (64 << 16), dagger->y - (64 << 16), 0);
      Func_0801521c(0x8000);
      Func_080150e4(dagger->z);
      Render_TransformVertices(gData_08199474, vertices, 4);
      Render_DrawModelList(quad);
      if (frame >= DAGGER_FALL_FRAME)
        BattleEffectKernels_IntegrateVector3(dagger, 64, 0);
      else
        BattleEffectKernels_IntegrateVector3(dagger, 64, -0x1000);
    }
    Heap_Release(quad);
    Heap_Release(vertices);
    gCameraSceneParameters[3] = 120;
    gCameraSceneParameters[4] = 120;

    BattleCamera_Shake(4, 4);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove((s32)BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

struct FxSlot {
  void *object;
  u8 unk04[0x26];
  u8 hidden;
};

struct FxRecord {
  u8 unk00[9];
  u8 low : 2;
  u8 priority : 2;
  u8 high : 4;
};

void *GetMotionRecordFar(void *object, s32 index);

/// Sets the OBJ priority of every motion record of the battle object, unless
/// its slot is hidden.
void BattleObject_SetRecordPriority(s32 objectId, s32 priority) {
  struct FxSlot *slot = GetBattleObjectSlotFar(objectId);
  struct FxRecord *record;
  s32 i;

  for (i = 0; (record = GetMotionRecordFar(slot->object, i)) != NULL; i++) {
    if (slot->hidden == 0)
      record->priority = priority;
  }
}

#define HEAP_SLOT_BLITTER 104 // Ram_HeapSlots->unk64[1]

// The Lost Age's blitter modes run the other way from The Broken Seal's:
// additive 0, max 1, transparent 2, copy 3.
#define BLITTER_ADDITIVE 0

#define CLIP_XY 3

#define STAR_FRAMES 60
#define STAR_LEAP_FRAME 16  // the caster leaps at the target
#define STAR_BURST_FRAME 32 // and strikes it
#define SMOKE_COUNT 32
#define STAR_COUNT 16
#define STAR_CELS 0x3e80 // the pink star's six 16x16 cels, after the smoke's

extern char ResourceId_PinkStarSheet;
extern char ResourceId_SmokeSheet;

void BattleEffect_SetApproachMotion(s32 id, s32 targetId, s32 frames, s32 rate);

// A pink star burst. Smoke puffs from the caster for its first 24 frames
// while the caster stands just past the target; at frame 16 it leaps at
// the target, and at frame 32 the palette turns pink, the caster lands
// beside the target, which is knocked back and flashes, and sixteen pink
// stars burst from it for 24 frames. Battle work byte 0x866 is set for the
// whole effect.
void BattleEffect_RunBackstab(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  u8 *canvas;
  struct EffectPosition caster;
  struct EffectPosition target;
  s32 frame;
  // Unused: draw[1] gives the stack frame its 4 bytes.
  DrawRectangle draw[2];
  struct MotionObject *targetObject;
  struct MotionObject *casterObject;
  u8 *battle;
  struct EffectStep *step;
  s32 i;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = 0x1010;
  Resource_LoadAndDecompress((s32)&ResourceId_PinkStarSheet,
                             work->sheet + STAR_CELS, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet, work->sheet, 1, 1);
  BattleEffect_LoadBlitter(HEAP_SLOT_BLITTER,
                           BLITTER(BLITTER_ADDITIVE, CLIP_XY));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add((s32)BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  targetObject = GetBattleObjectSlotFar(effect->actors[0])->object;
  casterObject = GetBattleObjectSlotFar(effect->actor)->object;
  battle = (u8 *)Ram_HeapSlots->battleWork;
  battle[0x866] = 1;

  for (frame = 0; frame != STAR_FRAMES; frame++) {
    BattleObject_SetRecordPriority(effect->actor, 3);

    // The first frame sends the smoke out from the caster's feet.
    if (frame == 0) {
      EffectPosition_ProjectBattler(effect->actor, &caster);
      for (i = 0; i != SMOKE_COUNT; i++) {
        s32 angle;
        s32 speed;

        step = &work->particles[i];
        step->variant = 0;
        step->x = (caster.x / 2) << 16;
        step->y = (caster.y - 8) << 16;
        angle = Random16();
        speed = (Random16() & 255) + 127;
        step->velocityX = (Trig_Sin(angle) * speed) >> 5;
        step->velocityY = (Trig_Cos(angle) * speed) >> 5;
      }
      Sound_PlayFar(136);
    }

    // The smoke rises through its six 24x48 cels, slowing; the caster
    // starts 160 percent of the way out to the target.
    // DEAD: always true, as frame counts up from 0; the ROM keeps the test.
    if (frame >= 0) {
      struct EffectStep *puff;

      for (i = 0, puff = work->particles; i != 24; i++, puff++) {
        if (frame >= i / 8 && puff->variant <= 23) {
          draw[0](canvas, work->sheet + (puff->variant / 4) * 0x480,
                  HI(puff->x) - 12, HI(puff->y) - 24, 24, 48);
          BattleEffectKernels_IntegrateVector2(puff, 48, -0x2000);
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
      BattleEffect_SetApproachMotion(effect->actor, effect->actors[0], 16, 120);
      casterObject->velocityY = 7 << 16;
    }

    // The strike: pink palette, the caster lands at 120 percent and the
    // target is pushed to 80, and the stars burst upward from it.
    if (frame == STAR_BURST_FRAME) {
      Iwram_CopyWords((void *)BG_PLTT,
                      Resource_GetTableEntry((s32)&ResourceId_PinkStarSheet),
                      128);
      casterObject->z = targetObject->z;
      casterObject->x = targetObject->x * 120 / 100;
      targetObject->x = targetObject->x * 80 / 100;
      casterObject->angle = targetObject->angle;
      EffectPosition_ProjectScaledBattler(effect->actors[0], &target);
      for (i = 0, step = &work->particles[SMOKE_COUNT]; i != STAR_COUNT;
           i++, step++) {
        s32 angle;
        s32 speed;

        step->variant = (Random16() & 15) + 16;
        step->x = (target.x / 2) << 16;
        step->y = (target.y - 8) << 16;
        angle = (Random16() & 0x7fff) + 0x4000;
        speed = (Random16() & 255) + 127;
        step->velocityX = (Trig_Sin(angle) * speed) >> 7;
        step->velocityY = (Trig_Cos(angle) * speed) >> 5;
      }
      work->shakeFrames = 4;
      Func_081180e8(134);
      Func_0814cd48(effect->actors[0], 7, 5, 0, 16);
      BattleEffect_StartActorMotion(effect->actors[0], 1, 8 << 16, 20 << 16,
                                    6 << 16, 10);
    }

    // The stars twinkle through their six cels as they fly.
    if (frame >= STAR_BURST_FRAME && frame < STAR_BURST_FRAME + 24) {
      struct EffectStep *star;

      for (i = 0, star = &work->particles[SMOKE_COUNT]; i != STAR_COUNT;
           i++, star++) {
        if (star->variant >= 0) {
          draw[0](canvas,
                  work->sheet + STAR_CELS + (((frame + i) / 4) % 6) * 256,
                  HI(star->x) - 8, HI(star->y) - 8, 16, 16);
          BattleEffectKernels_IntegrateVector2(star, 56, 0);
          star->variant--;
        }
      }
    }

    BattleCamera_Shake(4, 4);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  {
    // A local for the 0, so the address is built first, in r2.
    s32 cleared = 0;

    battle[0x866] = cleared;
  }
  Task_Remove((s32)BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_SLOT_BLITTER);
  BattleEffect_EndCanvasLayer();
}

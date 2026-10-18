// Draft of BattleFx_RunFlora, decompiled from the listing: differ 2885 of 2583
// (draft 2588), the same 138 calls and control flow; allocation differs.

#include "types.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "effect_step.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "mem.h"
#include "ram_buffer.h"

struct ModelTexture {
  u32 logWidth : 8;
  u32 logHeight : 8;
  u32 unk02 : 16;
  const void *pixels;
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

void BattleFx_BeginCanvasLayer(s32 bgControl);
void BattleFx_EndCanvasLayer(void); // BattleFx_EndCanvasLayer
void BattleFx_ProcessPendingGraphicsTransfer(void); // BattleFx_ProcessPendingGraphicsTransfer
void ObjectGroup_TickMemberTimers(void); // ObjectGroup_TickMemberTimers
void BattleFx_LoadBlitter(s32 slot, s32 modeAndFlags); // BattleFx_BuildBlitter
void Func_0814cd48(s32 actor, s32 palette, s32 pose, s32 timerSlot,
                   s32 timerFrames);              // ObjectGroup_UpdateMembers
void Func_0815b434(u8 *map, s32 size, s32 scale); // shaded circles
void Func_0816ae40(u8 *src, u8 *dst, s32 width, s32 height);
void Func_0815585c(struct BattleEffectArgument *effect, s32 packed, s32 *screen,
                   s32 *otherPoint);
void Func_0818caa8(void *cells, s32 x, s32 y, s32 size);
struct RenderModel *Render_AllocateModelList(s32 count);
void Render_TransformVertices(const void *corners, void *vertices, s32 count);
void Render_DrawModelList(struct RenderModel *model);
void Func_08015160(s32 x, s32 y, s32 z);
void Func_0801521c(s32 scale);
void SceneTransform_ApplyPitch(s32 angle);
void SceneTransform_ApplyYaw(s32 angle);
void Render_ResetTransformState(void);
void Func_080151e4(s32 x, s32 y, s32 z); // SceneTransform_ApplyScale3
void EffectPosition_ApplyAlternateStepAndYOffset(s32 id,
                                                 struct EffectPosition *out);
void BattleFx_SpawnObjects(s32 count, s32 resource, s32 mode);   // SpawnObjects
void BattleFx_SelectLivingTargets(struct BattleEffectArgument *effect); // SelectLivingTargets
void Func_080150e4(s32 angle); // SceneTransform_ApplyRoll
void BattleFx_StartActorMotion(s32 id, s32 force, s32 acceleration,
                               s32 speedLimit, s32 velocityY, s32 percent);
void Render_ApplyProjectedPlacementFar(void *object, s32 *request, s32 *source,
                                       s32 flags);
void BattleFx_FetchRectangleBlitters(s32 flip, void **out);
void Func_08118088(s32 actor, s32 motion); // BattleMotion_ApplyVariantMotionFar
void Func_081180e8(s32 phase);             // BattleEventRuntime_BeginPhaseFar
void Func_08158ce0(s32 randomMask, u32 shakeRange); // Camera_ApplyShake
void BattleFxKernels_IntegrateVector2(struct EffectStep *step, s32 gravity,
                                      s32 drag);
void **GetBattleObjectSlotFar(s32 objectId);
void *Runtime_BumpAllocateAlternatePool(s32 size);
void Resource_LoadAndDecompress(s32 resource, void *destination, s32 a, s32 b);
void *Resource_GetTableEntry(s32 resource);
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 order);
void Scheduler_RemoveCallback(void (*callback)(void));
void Runtime_ReleaseHeapBlock(s32 slot);
void Audio_PlayCue(s32 cue);
void WaitFrames(s32 frames);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);

void Render_BuildQuadStrip(void *mesh, s32 columns, s32 width, s32 height);
void BattleFxKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                      s32 gravity);
s32 Battle_GetObjectTableValueFar(s32 unit);
void Transform_SetLookAt(s32 *eye, s32 *target);
void BattleFx_BeginTiledCanvas(s32 bgControl);
void BattleFx_BeginTiledCanvasFilled(s32 bgControl);
void Transform_Push(void);
void Transform_Pop(void);

// The blitter's heap slot, as a byte offset into Ram_HeapSlots.
#define SLOT_BLITTER 104 // unk64[1]

// ⚓️'s blitter modes run the other way from The Broken Seal's.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT, BLIT_COPY };
#define BLITTER(mode, flags) ((mode) << 4 | (flags))
#define HI(v) (((s16 *)&(v))[1])

#include "bg_scroll.h"
#include "dma.h"
#include "input.h"

struct ResourceTableEntry {
  u16 unk0;
  u16 tile;
};

struct FloraObject {
  u8 unk0[8];
  u16 tile : 10;
  u16 unk08 : 6;
  u8 flags;
  u8 unk0b[5];
  u8 resource;
};

// Flora's petals in the scratch block at 0x02014000: the bloom's fifteen
// tile numbers, each petal's animation phase, position and velocity, and
// the object palette as it fades.
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
extern struct FloraWork gData_02014000;
extern u32 gCameraSceneParameters[];
extern const struct WordPair Data_08196ec8;
extern const u16 gData_08197410[];
extern const u8 Data_08199421[];
extern const u16 Data_08199426[];
extern const u16 Data_0819942e[];
extern const s16 Data_08199436[];
extern const s16 Data_0819943e[];
extern const s8 Data_08199446[];
extern const s8 Data_0819944a[];
extern const u8 gData_08199210[];
extern const u8 gData_08199364[];

struct FloraObject *Func_0815b290(s32 width, s32 height, u32 shape, s32 mode);
struct FloraObject *Func_0815b3b0(s32 width, s32 height, u32 shape, s32 mode);
void ResourceObject_ReleaseFar(void *object);
void Func_0813ba50(void);
void Func_08179e6c(void);
void Func_0813baec(void);
void Func_0814cca8(void);
void BattleBackground_LoadFar(s32 layer, s32 background, s32 palette);
void Resource_DecodeType01(const void *source, void *destination);
void Palette_LoadBasicObjColors(void);
void Graphics_LoadGreyRamp(void);
void Func_0815f0a0(s32 palette);
void Func_08143eb4(s32 x0, s32 y0, s32 x1, s32 y1, s32 color);
void MemFree(void *buffer);

#define SLOT_ALT 188 // unkb8[1]
#define OBJ_TILES(obj)                                                         \
  (0x06010000 + gResourceTableEntries[(obj)->resource].tile)

// Flora: petals swirl in on the wind, the blossom spirit rises among them,
// her spiral of light and its rings sweep the field, and a storm of petals
// strikes the foes.
void BattleFx_RunFlora(struct BattleEffectArgument *effect) {
  u8 *canvas;
  struct BattleEffectWork *work;
  u16 savedScroll;
  s32 *transition;
  struct FloraWork *w;
  u8 *sprites;
  DrawRectangle blit;
  DrawRectangle blit2;
  s32 frame;
  s32 i;
  s32 j;
  s32 scroll;
  s32 posX;
  s32 posY;
  s32 spin;
  s32 angle;
  s32 grow;
  s32 spread;
  s32 dx;
  s32 dy;
  u32 u;
  s32 place[4];
  struct WordPair req;
  struct ModelTexture texture;
  struct RenderModel *model;
  void *verts;
  struct EffectStep *p;
  struct FloraObject *obj;
  struct EffectPosition pos;
  struct EffectPosition out;
  struct EffectPosition pos2;
  struct EffectPosition out2;
  struct EffectPosition pos3;
  struct EffectPosition out3;
  u8 dot[2];

  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  savedScroll = gBgScroll[1].x;
  transition = (s32 *)Ram_HeapSlots->transitionWork;
  scroll = -80;
  w = &gData_02014000;
  sprites = Ram_HeapSlots->unk64[0];
  BattleFx_BeginCanvasLayer(0);
  Func_0813ba50();
  Func_08179e6c();
  work->transferMode = 0;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 3200);
  for (frame = 0; frame != 52; frame++) {
    if (frame == 27) {
      u8 *e;
      u8 *cells;

      gCameraSceneParameters[4] = 240;
      BattleFx_SelectLivingTargets(work->effect);
      BattleBackground_LoadFar(1, 0x45, 0);
      work->scrollTimer = 0;
      work->scrollInterval = 1;
      work->scrollStepX = -3;
      work->scrollStepY = 0;
      Scheduler_AddOrUpdateCallback(Func_0813baec, 3200);
      transition[4] = 1;
      e = Resource_GetTableEntry(0x9a);
      Iwram_CopyWords((void *)(BG_PLTT + 0x3e0), e, 32);
      Resource_DecodeType01(e + 32, gMapCellBuffer);
      cells = gMapCellBuffer;
      for (i = 0; i != 15; i++) {
        obj = Func_0815b290(16, 16, 0x40000000, 0xf000);
        work->objects[12 + i] = obj;
        obj->flags = (obj->flags & ~12) | 8;
        Iwram_CopyWords((void *)OBJ_TILES(obj), cells, 128);
        cells += 128;
        w->tiles[i] = ((struct FloraObject *)work->objects[12 + i])->tile;
      }
      for (i = 0; i != 17; i++) {
        obj = Func_0815b3b0(16, 16, 0x40000000, 0xf000);
        work->objects[27 + i] = obj;
        Iwram_CopyWords(obj, work->objects[12], 24);
      }
      for (i = 0; i != 32; i++) {
        w->phase[i] = (Random16() % 15) << 8;
        obj = work->objects[12 + i];
        obj->tile = w->tiles[w->phase[i] >> 8];
      }
      e = Resource_GetTableEntry(0x98);
      Resource_DecodeType01(e + 480, gMapCellBuffer);
      cells = gMapCellBuffer;
      for (i = 0; i != 12; i++) {
        obj = Func_0815b290(32, 32, 0x80002000, 0);
        obj->flags = (obj->flags & ~12) | 8;
        work->objects[i] = obj;
        Iwram_CopyWords((void *)OBJ_TILES(obj), cells, 0x400);
        cells += 0x400;
      }
      Resource_LoadAndDecompress(0xc2, work->sheet, 0, 0);
    }
    scroll += 4;
    REG_BG2X = scroll << 8;
    work->transferPending = 1;
    WaitFrames(1);
  }
  Resource_LoadAndDecompress(0x134, gMapCellBuffer, 0, 0);
  Resource_LoadAndDecompress(0x134, sprites, 0, 0);
  for (i = 0; i != 10; i++) {
    u8 *dst = sprites + i * 18 + 590;

    for (j = 0; j != 18; j++) {
      s32 v = gMapCellBuffer[gData_08197410[2] + j];

      if (v > 0) {
        v = 63 - i * 7;
        if (v <= 0) {
          v = 1;
        }
      }
      *dst++ = v;
    }
  }
  REG_DISPCNT = 0x7741;
  REG_BG2PA = 0x100;
  REG_BLDALPHA = 0x100e;
  REG_BLDCNT = 0x3f44;
  REG_BG2X = 0;
  posX = 128;
  posY = 44;
  spin = 512;
  angle = 0;
  grow = 0;
  spread = 0;
  for (i = 0; i != 32; i++) {
    w->x[i] = (i % 8 << 5) + 256;
    w->y[i] = i / 8 * 22 + 128;
  }
  work->transferMode = 2;
  work->transferValue = 50;
  BattleFx_LoadBlitter(SLOT_BLITTER, 19);
  blit = Ram_HeapSlots->unk64[1];
  for (frame = 0; frame != 300 && (!(gInput.repeat & 3) || frame <= 16);
       frame++) {
    if (frame == 0) {
      Iwram_ClearWords((void *)0x06004000, 0x4000);
      Iwram_ClearWords(canvas, 0x4000);
      for (i = 0; i != 240; i++) {
        w->palette[i] = 0x7fff;
      }
      Iwram_CopyWords((void *)OBJ_PLTT, w->palette, 480);
      REG_BG2PA = 0x100;
      Graphics_LoadGreyRamp();
      REG_BG2X = -0x3c00;
    }
    if (frame > 285) {
      Func_0815f0a0(0x166);
    } else if (frame > 199) {
      Func_0815f0a0(0x167);
    } else if (frame > 180) {
      Func_0815f0a0(0x150);
    }
    if (frame == 108) {
      REG_BG2PA = 0x80;
      REG_BG2X = 0;
      Func_0815b434(gMapCellBuffer, 8, 0x10000);
      Func_0815b434(gMapCellBuffer + 0x20, 16, 0xc000);
      Func_0815b434(gMapCellBuffer + 0xa0, 32, 0xc000);
      Func_0815b434(gMapCellBuffer + 0x2a0, 48, 0xc000);
    }
    if ((u32)(frame - 223) <= 29) {
      s32 k = Random16() & 3;

      Func_0818caa8(gMapCellBuffer + Data_08199426[k], 44, 78,
                    Data_08199421[k] * 2);
    }
    u = frame - 112;
    if (u <= 31) {
      u16 *e = Resource_GetTableEntry(0x98);

      for (i = 0; i != 240; i++) {
        u32 c = w->palette[i];
        u32 t = *e++;
        s32 b = (c << 16) >> 26 & 31;
        s32 g = (c << 16) >> 21 & 31;
        s32 r = c & 31;
        s32 tb = (t << 16) >> 26 & 31;
        s32 tg = (t << 16) >> 21 & 31;
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
      Iwram_CopyWords((void *)OBJ_PLTT, w->palette, 480);
    }
    if (u <= 7) {
      Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f - u * 0x01010101 * 8);
    }
    req = Data_08196ec8;
    dx = 0;
    place[3] = 0;
    place[1] = 0xff0000;
    dy = 0;
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

      if (m == 1) {
        dx = 1;
      }
      if (m == 2) {
        dx = 1;
      }
      if (m == 3) {
        dy = -1;
        dx = 2;
      }
      if (m == 4) {
        dy = -1;
        dx = 2;
      }
      if (m == 5) {
        dy = -1;
        dx = 2;
      }
      if (m == 0) {
        dy = 0;
        dx = 0;
      }
      if ((frame & 3) == 0) {
        gBgScroll[1].y = 30;
      }
      m = frame % 4;
      if (m == 1) {
        gBgScroll[1].y = 30;
      }
      if (m == 2) {
        gBgScroll[1].y = 34;
      }
      if (m == 3) {
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
          w->x[0] = 0xf00000;
          w->y[0] = 0x300000;
        } else if (frame <= 47) {
          w->x[0] += -0x30000;
          w->y[0] += 0x18000;
        } else if (frame <= 79) {
          w->x[0] += -0x10000;
          w->y[0] += -0x10000;
        } else if (frame <= 111) {
          w->x[0] += 0x30000;
        }
        if (frame == 112) {
          w->x[0] = 0x1000000;
        }
      } else if (i == 1) {
        if (frame <= 27) {
          w->x[1] = 0x1000000;
          w->y[1] = 0x300000;
        } else if (frame == 28) {
          w->x[1] = 0x100000;
          w->y[1] = 0x200000;
        } else if (frame <= 62) {
          w->x[1] += 0x1d6d8;
          w->y[1] += 0x1fde8;
        } else if (frame <= 79) {
          w->x[1] += 0x19c40;
          w->y[1] += 0x6710;
        } else if (frame <= 141) {
          w->x[1] += 0x30000;
        }
        if (frame == 112) {
          w->x[1] = 0x1000000;
        }
      } else if (i == 2) {
        if (frame <= 49) {
          w->x[2] = 0x1000000;
          w->y[2] = 0x300000;
        } else if (frame == 50) {
          w->x[2] = 0;
          w->y[2] = 0x7c0000;
        } else if (frame <= 62) {
          w->x[2] += 0x40000;
          w->y[2] += -0x14000;
        } else if (frame <= 93) {
          w->x[2] += 0x30000;
          w->y[2] += 0xc000;
        } else if (frame <= 125) {
          w->x[2] += 0x30000;
          w->y[2] += -0x10000;
        }
        if (frame == 112) {
          w->x[2] = 0x1000000;
        }
      } else if (i == 3) {
        if (frame <= 49) {
          w->x[3] = 0x1000000;
          w->y[3] = 0x300000;
        } else if (frame == 50) {
          w->x[3] = 0;
          w->y[3] = 0x6c0000;
        } else if (frame <= 103) {
          w->x[3] += 0x30000;
          w->y[3] += -0x14000;
        } else if (frame <= 135) {
          w->x[3] += 0x10000;
          w->y[3] += 0x10000;
        }
        if (frame == 112) {
          w->x[3] = 0x1000000;
        }
      } else if (i == 4) {
        if (frame <= 59) {
          w->x[4] = 0x1000000;
          w->y[4] = 0x300000;
        } else if (frame == 60) {
          w->x[4] = 0x100000;
          w->y[4] = 0x200000;
        } else if (frame <= 82) {
          w->x[4] += 0x40000;
          w->y[4] += 0x30000;
        } else if (frame <= 135) {
          w->x[4] += 0x40000;
        }
        if (frame == 112) {
          w->x[4] = 0x1000000;
        }
      } else if (i == 5) {
        if (frame <= 85) {
          w->x[5] = 0x1000000;
          w->y[5] = 0x300000;
        } else if (frame == 86) {
          w->x[5] = 0x900000;
          w->y[5] = 0x200000;
        } else if (frame <= 135) {
          w->x[5] += 0x10000;
          w->y[5] += 0x30000;
        }
        if (frame == 112) {
          w->x[5] = 0x1000000;
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
          w->vx[i] = s * Trig_Sin(a) / 256;
          w->vy[i] = s * Trig_Cos(a) / 256;
        } else if (frame == 252) {
          u32 a = Random16() & 0xffff;
          s32 s = ((Random16() & 255) << 1) + 256;

          w->x[i] = ((Random16() & 7) << 16) + 0x5c0000;
          w->y[i] = ((Random16() & 7) << 16) + 0x680000;
          w->vx[i] = s * Trig_Sin(a) / 256;
          w->vy[i] = s * Trig_Cos(a) / 256;
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
        w->phase[i] = (w->phase[i] + 32) % 3840;
      } else {
        w->phase[i] = (w->phase[i] + 128) % 3840;
      }
      obj = work->objects[12 + i];
      obj->tile = w->tiles[w->phase[i] >> 8];
    }
    for (i = 0; i != 32; i++) {
      place[0] = w->x[i] + -0x80000;
      place[2] = w->y[i] + -0x80000;
      Render_ApplyProjectedPlacementFar(work->objects[12 + i], place,
                                        (s32 *)&req, 0);
    }
    if (frame == 114) {
      Audio_PlayCue(212);
    }
    verts = Runtime_BumpAllocateAlternatePool(512);
    model = Render_AllocateModelList(1);
    texture.logWidth = 7;
    texture.logHeight = 7;
    model->kind = 7;
    texture.pixels = work->sheet;
    model->texture = &texture;
    model->mesh = gData_08199364;
    model->vertices = verts;
    for (i = 0; i != 4; i++) {
      s32 d = frame - Data_0819942e[i];

      if (d >= 0) {
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
          Func_0801521c(d << 13);
          SceneTransform_ApplyYaw(frame << 11);
          Render_TransformVertices(gData_08199210, verts, 4);
          Render_DrawModelList(model);
        }
      }
    }
    MemFree(model);
    MemFree(verts);
    Render_ResetTransformState();
    SceneTransform_ApplyPitch(0x9400);
    SceneTransform_ApplyYaw(0x6800);
    if (frame == 156) {
      spin = 512;
      angle = 0;
    }
    Func_080150e4(angle);
    if (frame == 156) {
      Audio_PlayCue(142);
    }
    if (frame == 223) {
      Audio_PlayCue(104);
    }
    if (frame == 252) {
      Audio_PlayCue(212);
    }
    if ((u32)(frame - 156) <= 74) {
      s32 r = 80 - (frame - 156) / 2;

      if (frame == 156) {
        spread = 0;
        grow = 0;
      }
      if (frame > 194) {
        grow += 32;
        spread += 3;
      }
      spin += 16;
      angle += spin;
      BattleFx_LoadBlitter(SLOT_ALT, 3);
      blit2 = Ram_HeapSlots->unkb8[1];
      for (i = 0; i != 32; i++) {
        s32 q = i / 4;
        s32 m = i % 4;
        s32 a = q << 13;
        s32 b = grow * m;
        s32 rr = r + spread * m / 30;
        s32 size = 7;

        for (j = 0; j != 4; j++) {
          if (frame >= 208 + j * 4 && m == 3 - j) {
            rr = r - (frame * 4 + -0x340 - j * 16);
            if (rr < 0) {
              rr = 0;
            }
            size = 7 - (frame - j * 4 - 208);
            if (size <= 2) {
              size = 3;
            }
          }
        }
        pos.x = rr * Trig_Sin(a + b) >> 16;
        pos.y = rr * Trig_Cos(a + b) >> 16;
        pos.depth = 0;
        EffectPosition_ApplyBaseAndYOffset(&pos, &out);
        out.x >>= 1;
        blit2(canvas, sprites + gData_08197410[size - 1], out.x + 48 - size / 2,
              out.y + 92 - size, size, size * 2);
      }
      Runtime_ReleaseHeapBlock(SLOT_ALT);
    }
    if ((u32)(frame - 186) <= 79) {
      s32 v = frame - 186;
      s32 inner;
      s32 ox;
      s32 oy1;
      s32 oy2;
      s32 sx;
      s32 sy;

      if (frame > 247) {
        v = 532 - frame * 2;
      } else if (v > 36) {
        v = 36;
      }
      inner = 64 - v;
      dot[0] = v;
      dot[1] = v;
      sx = 18 - v / 2;
      sy = 36 - v;
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
        s32 x1;
        s32 y1;
        s32 x2;
        s32 y2;

        pos2.x = inner * Trig_Sin(i << 9) >> 16;
        pos2.y = inner * Trig_Cos(i << 9) >> 16;
        pos2.depth = 0;
        EffectPosition_ApplyBaseAndYOffset(&pos2, &out2);
        out2.x >>= 1;
        x1 = out2.x + ox;
        y1 = out2.y + oy1;
        x2 = out2.x * 3 / 4 + ox;
        y2 = out2.y * 3 / 4 + oy2;
        blit(canvas, dot, x1 - 1, y1 - 1, 1, 2);
        blit(canvas, dot, x2 - 1, y2 - 1, 1, 2);
        if ((i & 31) == 0) {
          Func_08143eb4(x1, y1, x2, y2, v);
          Func_08143eb4(x1, y1 + 1, x2, y2 + 1, v);
        }
      }
      if ((u32)(frame - 228) <= 24) {
        s32 c = 256 - frame;

        inner = frame - 204;
        dot[0] = c;
        dot[1] = c;
        for (i = 0; i != 128; i++) {
          s32 x1;
          s32 y1;
          s32 x2;
          s32 y2;

          pos3.x = inner * Trig_Sin(i << 9) >> 16;
          pos3.y = inner * Trig_Cos(i << 9) >> 16;
          pos3.depth = 0;
          EffectPosition_ApplyBaseAndYOffset(&pos3, &out3);
          out3.x >>= 1;
          x1 = ox + out3.x;
          y1 = out3.y + oy1;
          x2 = out3.x * 3 / 4 + ox;
          y2 = out3.y * 3 / 4 + oy2;
          blit(canvas, dot, x1 - 1, y1 - 1, 1, 2);
          blit(canvas, dot, x2 - 1, y2 - 1, 1, 2);
          if ((i & 31) == 0) {
            Func_08143eb4(x1, y1, x2, y2, c);
            Func_08143eb4(x1, y1 + 1, x2, y2 + 1, c);
          }
        }
      }
    }
    work->transferPending = 1;
    WaitFrames(1);
  }
  Scheduler_RemoveCallback(Func_0813baec);
  gBgScroll[1].x = savedScroll;
  transition[4] = 0;
  Func_0814cca8();
  for (i = 0; i != 44; i++) {
    ResourceObject_ReleaseFar(work->objects[i]);
  }
  Palette_LoadBasicObjColors();
  Dma_Set((const void *)OBJ_PLTT, (void *)(BG_PLTT + 0xa0), 0x84000010,
          (volatile u32 *)0x040000d4);
  *(volatile u16 *)(BG_PLTT + 0xbc) = *(volatile u16 *)(BG_PLTT + 0x1e8);
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
    u8 *e = Resource_GetTableEntry(0x9a);
    s32 row;
    s32 a;
    s32 b;
    s32 k;
    s32 n = 0;

    Iwram_CopyWords((void *)BG_PLTT, e, 32);
    Resource_DecodeType01(e + 32, work->sheet);
    for (row = 0; row != 15; row++) {
      for (a = 0; a != 2; a++) {
        for (b = 0; b != 2; b++) {
          s32 offset = ((row * 2 + a) * 16 + b) * 8;

          for (k = 0; k != 8; k++, offset += 16) {
            u8 *d = work->sheet + 0x1000 + offset;
            u8 *s = work->sheet + n;

            for (j = 0; j != 4; j++) {
              u32 c = *s++;

              d[0] = c & 15;
              d[1] = c >> 4 & 15;
              n++;
              d += 2;
            }
          }
        }
      }
    }
  }
  for (frame = 0; frame != 128; frame++) {
    if (frame == 0) {
      p = (struct EffectStep *)gMapCellBuffer;
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
    p = (struct EffectStep *)gMapCellBuffer;
    for (i = 0; i != 64; i++, p++) {
      if (frame > i) {
        blit(canvas, work->sheet + 0x1000 + (p->variant / 4 % 15 << 8),
             HI(p->x) - 4, HI(p->y), 16, 16);
        BattleFxKernels_IntegrateVector2(p, 64, 0);
        p->velocityX += -0x1000;
        p->variant++;
      }
    }
    if (frame == 56) {
      Func_081180e8(134);
    }
    if ((u32)(frame - 57) <= 50) {
      for (i = 0; i != effect->count; i++) {
        if (--work->particles[i].variant == 0) {
          work->particles[i].variant = 15;
          work->shakeFrames = 4;
          Audio_PlayCue(133);
          Func_08118088(effect->actors[i], 0);
          Func_0814cd48(effect->actors[i], 7, -1, i, 8);
        }
      }
    }
    Func_08158ce0(2, 2);
    ObjectGroup_TickMemberTimers();
    work->transferPending = 1;
    WaitFrames(1);
  }
  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  Runtime_ReleaseHeapBlock(SLOT_BLITTER);
  BattleFx_EndCanvasLayer();
}

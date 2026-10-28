// Draft of BattleEffect_RunCoatlicue, decompiled from the listing: differ 3007 of
// 2781 (draft 2762), the same 196 calls and control flow; allocation differs.

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

void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_ProcessPendingGraphicsTransfer(void);
void BattleEffect_CountDownBattlerTints(void);
void BattleEffect_LoadBlitter(s32 slot, s32 modeAndFlags); // BattleEffect_BuildBlitter
void Func_0814cd48(s32 actor, s32 palette, s32 pose, s32 timerSlot,
                   s32 timerFrames);              // BattleEffect_TintBattler
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
void EffectPosition_ProjectBattler(s32 id,
                                                 struct EffectPosition *out);
void BattleEffect_SpawnObjects(s32 count, s32 resource, s32 mode);   // SpawnObjects
void BattleEffect_SelectLivingTargets(struct BattleEffectArgument *effect); // SelectLivingTargets
void Func_080150e4(s32 angle); // SceneTransform_ApplyRoll
void BattleEffect_StartActorMotion(s32 id, s32 force, s32 acceleration,
                               s32 speedLimit, s32 velocityY, s32 percent);
void Render_ApplyProjectedPlacementFar(void *object, s32 *request, s32 *source,
                                       s32 flags);
void BattleEffect_FetchRectangleBlitters(s32 flip, void **out);
void Func_08118088(s32 actor, s32 motion); // BattleMotion_ApplyVariantMotionFar
void Func_081180e8(s32 phase);             // BattlePlayback_StartFar
void Func_08158ce0(s32 randomMask, u32 shakeRange); // Camera_ApplyShake
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 gravity,
                                      s32 drag);
void **GetBattleObjectSlotFar(s32 objectId);
void *Heap_AllocEwram(s32 size);
void Resource_LoadAndDecompress(s32 resource, void *destination, s32 a, s32 b);
void *Resource_GetTableEntry(s32 resource);
void Task_Add(void (*func)(void), s32 order);
void Task_Remove(void (*func)(void));
void HeapSlot_Release(s32 slot);
void Sound_PlayFar(s32 cue);
void wait(s32 frames);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);

void Render_BuildQuadStrip(void *mesh, s32 columns, s32 width, s32 height);
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

// ⚓️'s blitter modes run the other way from The Broken Seal's.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT, BLIT_COPY };
#define BLITTER(mode, flags) ((mode) << 4 | (flags))
#define HI(v) (((s16 *)&(v))[1])

#include "bg_scroll.h"
#include "dma.h"
#include "input.h"
#include "io_write_queue.h"

struct ResourceTableEntry {
  u16 unk0;
  u16 tile;
};

struct WordPair {
  s32 a;
  s32 b;
};

extern struct ResourceTableEntry gResourceTableEntries[];
extern u8 gMapCellBuffer[];
extern struct EffectStep gData_02015000[];
extern u8 Data_02014400[];
extern u8 Data_02014800[];
extern u32 gCameraSceneParameters[];
extern const struct WordPair Data_08196ef0;
extern const u8 Data_081996ac[];
extern const u8 Data_081996bb[];
extern const u16 Data_081996ca[];
extern const u16 Data_081996e8[];
extern const u8 Data_081996fa[];
extern const u8 Data_08199703[];
extern const u8 Data_0819970c[];
extern const u8 Data_08199715[];
extern const s8 Data_0819971e[];
extern const s8 Data_08199727[];
extern const s8 Data_08199730[];
extern const s8 Data_08199739[];
extern const u8 Data_08199742[];
extern const u16 Data_08199746[];
extern const u16 Data_0819974e[];
extern const u16 Data_08199754[];
extern const u8 Data_08199756[];
extern const u16 Data_08199758[];
extern const u16 Data_0819975c[];
extern const u16 Data_08199760[];
extern const u16 Data_08199764[];
extern const u8 gData_08199210[];
extern const u8 gData_08199364[];

u8 *Func_0815b290(s32 width, s32 height, u32 shape, s32 mode);
void ResourceObject_ReleaseFar(void *object);
void Func_0813ba50(void);
void Func_08179e6c(void);
void BattleEffect_ArmPaletteHBlankDma(void);
void Func_081885f0(void *vertices, void *triangles, s32 count);
void BattleActor_SpawnObjectsForListFar(s16 *list, s32 mode);
void Resource_DecodeType01(const void *source, void *destination);
void Palette_LoadBasicObjColors(void);
void Graphics_LoadGreyRamp(void);
void Func_08164a4c(s32 red, s32 green, s32 blue); // Palette_BrightenBgEntries
void Func_08164b2c(s32 r, s32 g, s32 b);
void Heap_Release(void *buffer);

#define OBJ_TILES(obj) (0x06010000 + gResourceTableEntries[(obj)[16]].tile)
#define HALF_HI(v) (((s16 *)&(v))[1])

// Fills the scanline gradient behind the serpent: dark rows above and below,
// a ramp shaded by level in between.
#define FILL_GRADIENT(table, i, level)                                         \
  do {                                                                         \
    for (; i != 135; i++) {                                                    \
      s32 t_ = i - 15;                                                         \
      s32 b_ = (level) - t_ / 15 + 32;                                         \
      s32 g_ = (level) - t_ / 16 + 16;                                         \
      s32 r_ = (level) - t_ / 17;                                              \
                                                                               \
      if (b_ < 0)                                                              \
        b_ = 0;                                                                \
      if (b_ > 31)                                                             \
        b_ = 31;                                                               \
      if (g_ < 0)                                                              \
        g_ = 0;                                                                \
      if (g_ > 31)                                                             \
        g_ = 31;                                                               \
      if (r_ < 0)                                                              \
        r_ = 0;                                                                \
      if (r_ > 31)                                                             \
        r_ = 31;                                                               \
      *table++ = b_ << 10 | g_ << 5 | r_;                                      \
    }                                                                          \
    for (; i != 160; i++) {                                                    \
      *table++ = 0;                                                            \
    }                                                                          \
  } while (0)

// Draws one spark of the falling stone sheet and drops it when it leaves the
// top of the screen.
#define DRAW_SPARK(p)                                                          \
  do {                                                                         \
    s32 n_ = (p)->z;                                                           \
                                                                               \
    blit(canvas, work->sheet + 0x4800 + Data_081996ca[n_],                     \
         HALF_HI((p)->x) - (Data_081996ac[n_] >> 1),                           \
         HALF_HI((p)->y) - (Data_081996bb[n_] >> 1), Data_081996ac[n_],        \
         Data_081996bb[n_]);                                                   \
    BattleEffectKernels_IntegrateVector2(p, 63, -0x4000);                          \
    if ((p)->y < -(Data_081996bb[n_] << 16)) {                                 \
      (p)->variant = -1;                                                       \
    }                                                                          \
  } while (0)

// Coatlicue: the ground opens over a green dusk, the serpent goddess rises in
// a spiral of stone, her coils sweep the field and her light heals the party.
void BattleEffect_RunCoatlicue(struct BattleEffectArgument *effect) {
  u8 *canvas;
  struct BattleEffectWork *work;
  u8 *sprites;
  DrawRectangle blit;
  u16 *table;
  s32 frame;
  s32 i;
  s32 j;
  s32 k;
  s32 y;
  s32 level;
  s32 skip;
  s32 rise;
  s32 spin;
  s32 gridX;
  s32 gridY;
  s32 t;
  s32 place[4];
  struct WordPair req;
  s16 list[2];
  struct EffectPosition pos;
  struct ModelTexture texture;
  struct ModelTexture texture2;
  struct RenderModel *model;
  void *verts;
  struct EffectStep *p;
  u8 *obj;

  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  y = -80;
  sprites = Ram_HeapSlots->unk64[0];
  Func_0813ba50();
  BattleEffect_StartCanvasLayer(0);
  Func_08179e6c();
  work->transferMode = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 3200);
  for (frame = 0; frame != 27; frame++) {
    y += 4;
    REG_BG2X = y << 8;
    work->transferPending = 1;
    wait(1);
  }
  BattleEffect_LoadBlitter(SLOT_BLITTER, 19);
  blit = Ram_HeapSlots->unk64[1];
  Resource_LoadAndDecompress(0x134, sprites, 0, 0);
  work->transferMode = 2;
  work->transferValue = 75;
  level = -8;
  table = (u16 *)(work->unk7300 + 0x340);
  for (i = 0; i != 15; i++) {
    *table++ = 0;
  }
  FILL_GRADIENT(table, i, level);
  Task_Add(BattleEffect_ArmPaletteHBlankDma, 3326);
  skip = 0;
  Func_081885f0(Data_02014400, Data_02014800, 32);
  list[0] = 255;
  BattleActor_SpawnObjectsForListFar(list, 0);
  {
    u8 *e = Resource_GetTableEntry(0xa7);
    u8 *cells;

    Iwram_CopyWords((void *)OBJ_PLTT, e, 512);
    Resource_DecodeType01(e + 512, gMapCellBuffer);
    cells = gMapCellBuffer;
    for (i = 0; i != 12; i++, cells += 1024) {
      obj = Func_0815b290(32, 32, 0x80002000, 0);
      obj[9] = (obj[9] & ~12) | 8;
      work->objects[i] = obj;
      Iwram_CopyWords((void *)OBJ_TILES(obj), cells, 1024);
    }
  }
  Graphics_LoadGreyRamp();
  Resource_LoadAndDecompress(0xfb, gMapCellBuffer, 0, 0);
  Func_0816ae40(gMapCellBuffer, work->sheet, 64, 64);
  Resource_LoadAndDecompress(0xfc, work->sheet + 0x4000, 0, 0);
  {
    u8 *src = work->sheet + 0x4800;
    u8 *dst;

    Resource_LoadAndDecompress(0xfd, src, 0, 0);
    dst = work->sheet + 0x53b8;
    for (i = 0; i != 3000; i++) {
      *dst++ = *src++ >> 1;
    }
  }
  Resource_LoadAndDecompress(0xc2, gMapCellBuffer, 0, 0);
  IO_WRITE_QUEUE_PUSH(0x04000000, 0x7541, 0x20000);
  gCameraSceneParameters[4] = 240;
  REG_BG2PA = 0x80;
  REG_BG2X = 0;
  REG_BLDCNT = 0x3f44;
  REG_BLDALPHA = 0x1010;
  rise = 0xde500000;
  spin = 0;
  for (frame = 0; frame != 602; frame++, rise += 0x100000, spin += 512) {
    if ((gInput.repeat & 3) && frame <= 581 && skip == 0) {
      skip = 1;
    }
    if (skip > 0) {
      s32 v = skip * 0x101;

      v = (v + (v << 16)) << 2;
      work->transferValue = v;
      if (v > 0x3f3f3f3f) {
        work->transferValue = 0x3f3f3f3f;
      }
      work->transferMode = 1;
      if (++skip == 19) {
        break;
      }
    }
    if (frame == 0) {
      for (i = 0; i != 384; i++) {
        gData_02015000[i].variant = -1;
      }
      for (i = 0; i != 64; i++) {
        work->particles[i].variant = -1;
      }
      gridX = 0x580000;
      gridY = 0x240000;
      work->transferMode = 2;
      work->transferValue = 50;
    }
    if (frame == 82) {
      Iwram_FillWords(canvas, 0x4000, 0x2f2f2f2f);
      Sound_PlayFar(130);
    }
    if (frame == 210) {
      Iwram_FillWords(canvas, 0x4000, 0x2f2f2f2f);
      Sound_PlayFar(130);
    }
    if (frame == 274) {
      Sound_PlayFar(139);
    }
    if (frame == 310) {
      Sound_PlayFar(206);
    }
    if (frame == 316) {
      Sound_PlayFar(154);
    }
    if (frame == 466) {
      Sound_PlayFar(142);
    }
    if (frame == 498) {
      Sound_PlayFar(154);
    }
    if (frame == 503) {
      Sound_PlayFar(154);
    }
    if (frame == 508) {
      Sound_PlayFar(154);
    }
    if (frame == 520) {
      Sound_PlayFar(212);
    }
    if (frame == 540) {
      Sound_PlayFar(186);
    }
    if (frame <= 1999999) {
      table = (u16 *)(work->unk7300 + 0x340);
      level = frame / 32 - 8;
      if (level > 8) {
        level = 8;
      }
      for (i = 0; i != 15; i++) {
        *table++ = 0;
      }
      if (frame > 539) {
        level = (frame - 540) / 2 + 8;
      } else if (frame > 311) {
        level = 14 - (frame - 312) / 4;
        if (level <= 7) {
          level = 8;
        }
      } else if (frame > 299) {
        level += (frame - 300) / 2;
        if (level > 14) {
          level = 14;
        }
      }
      i = 15;
      FILL_GRADIENT(table, i, level);
    }
    if ((u32)(frame - 282) <= 17) {
      s32 g = (frame - 282) / 2 + 14;
      s32 b = frame - 253;
      u16 *dst = (u16 *)OBJ_PLTT;
      u32 c;

      if (g > 31) {
        g = 31;
      }
      if (b > 31) {
        b = 31;
      }
      c = b << 10 | g << 5;
      for (i = 0; i != 256; i++) {
        *dst++ = c;
      }
    }
    if (frame > 299) {
      u16 *e = Resource_GetTableEntry(0xa7);
      u16 *dst = (u16 *)OBJ_PLTT;
      s32 tr = 31;
      s32 tg = 31;
      s32 tb = 15;

      if (frame > 315) {
        tr = 4;
        tg = 12;
        tb = 16;
      }
      if (frame > 337) {
        tr = 0;
        tg = 0;
        tb = 0;
      }
      for (i = 0; i != 256; i++, e++, dst++) {
        u32 c = *dst;
        u32 s = *e;
        s32 cb = (c << 16) >> 26 & 31;
        s32 cg = (c << 16) >> 21 & 31;
        s32 cr = c & 31;
        s32 sb = ((s << 16) >> 26 & 31) + tb;
        s32 sg = ((s << 16) >> 21 & 31) + tg;
        s32 sr = (s & 31) + tr;

        if (cb < sb - 1) {
          cb += 2;
        } else if (cb > sb + 1) {
          cb -= 2;
        }
        if (cg < sg - 1) {
          cg += 2;
        } else if (cg > sg + 1) {
          cg -= 2;
        }
        if (cr < sr - 1) {
          cr += 2;
        } else if (cr > sr + 1) {
          cr -= 2;
        }
        if (cb > 31) {
          cb = 31;
        }
        if (cg > 31) {
          cg = 31;
        }
        if (cr > 31) {
          cr = 31;
        }
        if (cb < 0) {
          cb = 0;
        }
        if (cg < 0) {
          cg = 0;
        }
        if (cr < 0) {
          cr = 0;
        }
        *dst = cb << 10 | cg << 5 | cr;
      }
    }
    if (frame > 519) {
      s32 d = frame - 520;
      u16 *e = Resource_GetTableEntry(0xa7);
      u16 *dst = (u16 *)OBJ_PLTT;
      s32 half = d / 2;

      for (i = 0; i != 256; i++, e++, dst++) {
        u32 s = *e;
        s32 r = (s & 31) + d;
        s32 g = ((s << 16) >> 21 & 31) + half;
        s32 b = ((s << 16) >> 26 & 31) + d / 4;

        if (r > 31) {
          r = 31;
        }
        if (g > 31) {
          g = 31;
        }
        if (b > 31) {
          b = 31;
        }
        *dst = b << 10 | g << 5 | r;
      }
    }
    if ((u32)(frame - 292) <= 253) {
      req = Data_08196ef0;
      place[1] = 0xff0000;
      place[3] = 0;
      if (frame == 360) {
        gridY = 0x240000;
      }
      if ((u32)(frame - 396) <= 111) {
        gridY += 0x4000;
      }
      if (frame > 539) {
        gridY += 0x100000;
      }
      for (i = 0; i != 12; i++) {
        place[0] = ((i % 3) << 21) + gridX;
        place[2] = ((i / 3) << 21) + gridY;
        Render_ApplyProjectedPlacementFar(work->objects[i], place, (s32 *)&req,
                                          0);
      }
    }
    if ((u32)(frame - 274) <= 23) {
      p = &gData_02015000[80 + frame - 274];
      p->y = 0x640000;
      p->x = ((Random16() & 15) + 52) << 16;
      p->velocityX = ((Random16() & 31) - 16) << 12;
      p->velocityY = -((Random16() & 15) + 8) << 12;
      p->z = (Random16() & 7) + 5;
      p->variant = 32;
    }
    if (frame == 310) {
      p = gData_02015000;
      for (i = 0; i != 80; i++, p++) {
        p->x = ((Random16() & 63) + 28) << 16;
        p->y = (Random16() % 80 + 56) << 16;
        p->velocityX = ((Random16() & 31) - 16) << 12;
        p->velocityY = (8 - (Random16() & 31)) << 12;
        p->z = (Random16() & 7) + 5;
        p->variant = 32;
      }
    }
    for (k = 0; k != 9; k++) {
      if (frame == Data_081996e8[k]) {
        for (j = 0; j != 7; j++) {
          p = &work->particles[(k % 9) * 7 + j];
          p->x = (Random16() % Data_0819970c[k] + Data_081996fa[k]) << 16;
          p->y = ((Random16() & 3) + j * Data_08199715[k] + Data_08199703[k])
                 << 16;
          p->velocityX = Data_0819971e[k] << 12;
          p->velocityY = Data_08199727[k] << 12;
          p->z = ((Random16() & 3) + Data_08199730[k]) << 16;
          p->velocityZ = Data_08199739[k] << 10;
          p->variant = 0;
        }
      }
    }
    if (frame <= 358) {
      p = gData_02015000;
      for (i = 0; i != 384; i++, p++) {
        if (p->variant >= 0) {
          DRAW_SPARK(p);
        }
      }
    } else {
      Render_ResetTransformState();
      Func_08015160(0, 0, 0x1000000);
      if (frame > 539) {
        Func_080151e4(frame * 1500 - 0xc4c10, 0x10000, 0x10000);
      } else if (frame > 517) {
        Func_080151e4(0, 0, 0x10000);
      } else if (frame > 485) {
        s32 s = 0x103000 - (frame << 11);

        Func_080151e4(s, s, 0x10000);
      }
      if (frame > 539) {
        SceneTransform_ApplyYaw(frame << 10);
      } else if (frame > 517) {
        SceneTransform_ApplyYaw(0x10000);
        SceneTransform_ApplyPitch(0x8000);
      } else if (frame > 453) {
        s32 a = (frame - 454) << 10;

        SceneTransform_ApplyYaw(a);
        SceneTransform_ApplyPitch(Trig_Sin(a) >> 3);
      }
      Func_080151e4(55536, 45536, 27768);
      if (frame == 360) {
        if (skip == 0) {
          work->transferMode = 2;
          work->transferValue = 75;
        }
        p = gData_02015000;
        for (i = 0; i != 384; i++, p++) {
          p->variant = Random16() & 3;
          p->x = (Random16() % 160 - 80) << 16;
          p->y = (Random16() % 160 + 48) << 16;
          p->z = (Random16() % 160 - 80) << 16;
          p->velocityX = 0;
          p->velocityY =
              (-((Random16() & 63) + 32) << 10) - (1 << (16 - p->variant));
          p->velocityZ = Random16();
        }
      }
      if (frame == 540) {
        p = gData_02015000;
        for (i = 0; i != 384; i++, p++) {
          p->y = 0;
          p->velocityY = -((Random16() & 63) + 32) << 15;
        }
      }
      p = gData_02015000;
      for (i = 0; i != 80; i++, p++) {
        s32 n;

        if (p->variant < 0) {
          continue;
        }
        EffectPosition_ProjectPoint(p, &pos);
        pos.depth = HALF_HI(pos.depth) - 228;
        n = pos.depth * 6 / 50 + Data_08199742[p->variant];
        if (n < 0) {
          n = 0;
        }
        if (n > 14) {
          n = 14;
        }
        pos.y = HALF_HI(pos.y);
        pos.x = HALF_HI(pos.x) + 60;
        if (frame > 539) {
          pos.y = pos.y + (frame << 4) - 8608;
        } else if (frame > 517) {
          pos.y = pos.y + 16;
        } else if (frame <= 453) {
          pos.y = pos.y + 80;
        } else {
          pos.y = pos.y - frame + 534;
        }
        if (p->variant <= 1) {
          blit(canvas, work->sheet + 0x4800 + Data_081996ca[n],
               pos.x - (Data_081996ac[n] >> 1), pos.y - (Data_081996bb[n] >> 1),
               Data_081996ac[n], Data_081996bb[n]);
        } else {
          blit(canvas, work->sheet + 0x53b8 + Data_081996ca[n],
               pos.x - (Data_081996ac[n] >> 1), pos.y - (Data_081996bb[n] >> 1),
               Data_081996ac[n], Data_081996bb[n]);
        }
        if (frame <= 453 || frame >= i / 16 + 540) {
          p->y += p->velocityY;
        }
      }
    }
    for (k = 0; k != 9; k++) {
      s32 top = 0;

      p = &work->particles[k * 7];
      for (j = 0; j != 7; j++, p++) {
        if (work->particles[k * 7 + 6].variant >= 0) {
          s32 n = 14 - HALF_HI(p->z);
          s32 x = (p->x + (Trig_Sin((frame - (j % 7) * 8) << 10) << 2)) >> 16;

          if (j == 0) {
            top = p->y;
          }
          if (n < 0) {
            n = 0;
          }
          blit(canvas, work->sheet + 0x53b8 + Data_081996ca[n],
               x - (Data_081996ac[n] >> 1),
               (top >> 16) - (Data_081996bb[n] >> 1), Data_081996ac[n],
               Data_081996bb[n]);
          BattleEffectKernels_IntegrateVector3(p, 64, 0);
          if (p->z < 0) {
            p->z = 0;
          }
          if (j == 6 && p->y < -(Data_081996bb[n] << 16)) {
            p->variant = -1;
          }
          top = top + p->z + 0x80000;
        }
      }
    }
    verts = Heap_AllocEwram(512);
    model = Render_AllocateModelList(1);
    texture.logWidth = 7;
    model->texture = &texture;
    texture.logHeight = 7;
    model->mesh = gData_08199364;
    model->vertices = verts;
    t = frame - 210;
    if ((u32)t <= 107) {
      s32 ty = (frame * 3 << 13) - 0x4ec000;
      s32 pitch = 4000;
      s32 s;
      s32 sh;

      if (ty > 0x180000) {
        ty = 0x180000;
      }
      s = frame * 611 - 111926;
      if (s > 55536) {
        s = 55536;
      }
      if (frame > 273) {
        ty = (frame << 15) - 0x710000;
        if (ty > 0x200000) {
          ty = 0x200000;
        }
        pitch = 21536 - (frame << 6);
        if (pitch <= 2999) {
          pitch = 3000;
        }
        s = s + frame * 1875 - 513750;
        if (s > 85536) {
          s = 85536;
        }
      }
      model->kind = 6;
      texture.pixels = work->sheet;
      sh = (frame << 3) - 1744;
      if (sh > 0) {
        sh = 0;
      }
      if (frame > 309) {
        sh = (310 - frame) << 3;
      }
      model->shade = sh;
      Render_ResetTransformState();
      Func_08015160(0, ty, 0);
      Func_080151e4(0x8000, 0x10000, 0x8000);
      Func_0801521c(s);
      if (frame <= 273) {
        Func_080150e4(spin - 0x12400);
      }
      SceneTransform_ApplyPitch(pitch);
      SceneTransform_ApplyYaw(spin);
      Render_TransformVertices(gData_08199210, verts, 64);
      Render_DrawModelList(model);
    }
    if ((u32)(frame - 273) <= 7 && skip == 0) {
      s32 d = frame - 273;
      s32 v;

      work->transferMode = 1;
      v = d * 0x101;
      v = v + (v << 16);
      work->transferValue = 0x3f3f3f3f - (v << 2);
    }
    if (frame == 281 && skip == 0) {
      work->transferMode = 2;
      work->transferValue = 50;
    }
    if ((u32)(frame - 274) <= 27) {
      s32 s = (frame << 11) - 0x85000;
      s32 sh;

      texture.pixels = gMapCellBuffer;
      model->kind = 7;
      sh = 0;
      if (frame > 293) {
        sh = (294 - frame) << 3;
      }
      model->shade = sh;
      Render_ResetTransformState();
      Func_08015160(0, 0x1c0000, 0);
      Func_0801521c(s);
      SceneTransform_ApplyPitch(4000);
      SceneTransform_ApplyYaw(spin);
      Render_TransformVertices(gData_08199210, verts, 64);
      Render_DrawModelList(model);
    }
    if (frame > 80) {
      for (i = 0; i != 4; i++) {
        s32 st = Data_08199746[i];

        if (frame > st && frame < st + 10) {
          s32 d;
          s32 s;

          texture.pixels = gMapCellBuffer;
          model->kind = 6;
          model->shade = -16;
          d = frame - st;
          Render_ResetTransformState();
          s = d << 13;
          if (i > 1) {
            s32 ty = -0x2c0000;

            if (frame > 539) {
              ty += rise;
            }
            Func_08015160(-0x40000, ty, 0);
            Func_080151e4(s, s, s);
          } else {
            Func_080151e4(s, d << 14, s);
          }
          SceneTransform_ApplyPitch(0x4000);
          SceneTransform_ApplyYaw(frame << 9);
          Render_TransformVertices(gData_08199210, verts, 64);
          Render_DrawModelList(model);
        }
      }
    }
    if (frame > 80) {
      for (i = 0; i != 3; i++) {
        if (frame == Data_0819974e[i]) {
          Iwram_FillWords(canvas, 0x4000, 0x2f2f2f2f);
        }
        if (frame > Data_0819974e[i] && frame < Data_0819974e[i] + 10) {
          s32 d = frame - Data_0819974e[i];
          s32 s = 0x14000 - (d << 13);
          s32 sh = d * 16 - 56;

          texture.pixels = gMapCellBuffer;
          model->kind = 7;
          if (sh > 0) {
            sh = 0;
          }
          model->shade = sh;
          Render_ResetTransformState();
          Func_08015160(-0x40000, -0x2c0000, 0);
          Func_080151e4(s, s << 1, s);
          Func_080150e4((i << 14) + 0x2000);
          SceneTransform_ApplyPitch(-0x1000);
          SceneTransform_ApplyYaw(frame << 9);
          Render_TransformVertices(gData_08199210, verts, 64);
          Render_DrawModelList(model);
        }
      }
    }
    if (frame > 80) {
      for (i = 0; i != 3; i++) {
        s32 st = Data_08199754[i];

        if (frame > st && frame < st + 100) {
          s32 s = (frame - st) * 2978;
          s32 ty = -0x2c0000;

          if (s > 65535) {
            s = 0x10000;
          }
          texture.pixels = work->sheet;
          model->kind = 6;
          model->shade = -16;
          Render_ResetTransformState();
          if (frame > 539) {
            ty = rise + -0x2c0000;
          }
          Func_08015160(-0x40000, ty, 0);
          Func_080151e4(s, s / 2, s);
          SceneTransform_ApplyPitch(0x4000);
          SceneTransform_ApplyYaw(frame << 11);
          Render_TransformVertices(gData_08199210, verts, 64);
          Render_DrawModelList(model);
        }
      }
    }
    texture.logWidth = 7;
    texture.logHeight = 4;
    texture.pixels = work->sheet + 0x4000;
    model->kind = 7;
    model->mesh = Data_02014800;
    model->vertices = verts;
    model->scrollU = 0;
    model->scrollV = 0;
    for (i = 0; i != 2; i++) {
      s32 sh;
      s32 s;

      if (frame < Data_08199756[i]) {
        continue;
      }
      if (frame <= 293) {
        s = Data_08199758[i] + Data_08199760[i] * (frame - Data_08199756[i]);
        if (s > Data_08199764[i]) {
          s = Data_08199764[i];
        }
        if (frame > 209) {
          s -= Data_08199760[i] * t;
        }
        Render_ResetTransformState();
        Func_080151e4(0x8000, 0x10000, 0x8000);
        if (frame > 273) {
          s = s + (frame << 13) + -0x224000;
        }
        Func_0801521c(s);
        if (i == 0) {
          if (frame > 209) {
            s32 v = t;

            if (v > 48) {
              v = 48;
            }
            Func_08015160(0, v << 16, 0);
          }
          Func_080150e4(Data_0819975c[i] - (frame << 8));
          sh = -16;
        } else {
          s32 x = -48;

          if (frame > 127) {
            x = frame - 176;
          }
          if (x > -16) {
            x = -16;
          }
          Func_08015160(x << 16, 0, 0);
          Func_080150e4(Data_0819975c[i] + (frame << 8));
          if (frame > 209) {
            Func_080150e4((frame << 8) + -0xd200);
          }
          sh = ((frame - Data_08199756[i]) << 2) - 64;
          if (sh > -16) {
            sh = -16;
          }
        }
        if (frame > 273) {
          sh = 1080 - (frame << 2);
          if (i == 0) {
            Func_080150e4((274 - frame) << 10);
          } else {
            Func_080150e4(spin + -0x22400);
          }
        }
      } else {
        sh = (frame << 2) - (i << 3) - 1240;
        if (sh > -16) {
          sh = -16;
        }
        s = i * 26384 - (frame << 12) + 0x138710;
        if (s <= 16383) {
          continue;
        }
        Render_ResetTransformState();
        Func_080151e4(0x8000, 0x10000, 0x8000);
        Func_08015160(0, 0x200000, 0);
        Func_0801521c(s);
      }
      if (sh > -64) {
        model->shade = sh;
        SceneTransform_ApplyPitch((frame - i * 82) * 22 - 3000);
        SceneTransform_ApplyYaw(-frame << 10);
        Render_TransformVertices(Data_02014400, verts, 64);
        Render_DrawModelList(model);
      }
    }
    Heap_Release(model);
    Heap_Release(verts);
    while (*(volatile u32 *)&work->vblanksSinceTransfer <= 1) {
    }
    work->transferPending = 1;
    wait(1);
  }
  Iwram_FillWords((void *)0x06004000, 0x4000, 0x3f3f3f3f);
  IO_WRITE_QUEUE_PUSH(0x04000000, 0x7741, 0x20000);
  for (i = 0; i != 12; i++) {
    ResourceObject_ReleaseFar(work->objects[i]);
  }
  Palette_LoadBasicObjColors();
  Dma_Set((const void *)OBJ_PLTT, (void *)(BG_PLTT + 0xa0), 0x80000010,
          (volatile u32 *)0x040000d4);
  *(volatile u16 *)(BG_PLTT + 0xbc) = *(volatile u16 *)(BG_PLTT + 0x1e8);
  BattleEffect_SelectLivingTargets(work->effect);
  wait(1);
  Task_Remove(BattleEffect_ArmPaletteHBlankDma);
  gCameraSceneParameters[4] = 120;
  wait(1);
  Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(0x130), 512);
  Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
  Func_08164b2c(0, 0, 11);
  for (frame = 0; frame != 60; frame++) {
    s32 s;
    s32 lift;
    s32 d;

    if (frame == 16) {
      work->transferMode = 2;
      work->transferValue = 75;
    }
    if (frame > 52) {
      Func_08164a4c(-4, -4, -4);
    }
    if (frame > 48) {
      d = frame - 48;
      Func_08164b2c(-d, -d, 11 - d * 2);
    }
    if (frame == 0) {
      work->transferMode = 3;
      work->transferValue = 0x04040404;
      p = gData_02015000;
      for (i = 0; i != 256; i++, p++) {
        p->x = (Random16() & 127) << 16;
        p->y = ((Random16() & 255) + 120) << 16;
        p->velocityX = ((Random16() & 31) - 16) << 13;
        p->velocityY = (8 - (Random16() & 31)) << 12;
        p->z = (Random16() & 7) + 5;
        p->variant = 32;
      }
    }
    p = gData_02015000;
    for (i = 0; i != 256; i++, p++) {
      if (p->variant >= 0) {
        DRAW_SPARK(p);
      }
    }
    for (i = 0; i != effect->count; i++) {
      if (frame == i * 8 + 18) {
        if (i == 0) {
          Func_0814cd48(effect->actors[0], 7, -1, 0, 16);
          Func_081180e8(126);
          Sound_PlayFar(126);
        } else {
          Sound_PlayFar(126);
          Func_0814cd48(effect->actors[i], 7, -1, i, 16);
        }
      }
    }
    verts = Heap_AllocEwram(512);
    model = Render_AllocateModelList(1);
    model->texture = &texture2;
    texture2.logWidth = 7;
    texture2.logHeight = 7;
    model->mesh = gData_08199364;
    model->vertices = verts;
    model->kind = 6;
    texture2.pixels = work->sheet;
    lift = frame * 8 - 128;
    s = (frame << 13) + -0x20000;
    d = frame - 16;
    for (i = 0; i != 3; i++, lift -= 64, s += -0x10000, d -= 8) {
      if (d >= 0) {
        s32 sh = 0;

        if (d > 15) {
          sh = (16 - d) << 3;
        }
        model->shade = sh;
        if (sh > -64) {
          Render_ResetTransformState();
          Func_08015160(0, (112 - lift) << 16, 0);
          Func_080151e4(0x8000, 0x10000, 0x8000);
          Func_0801521c(s);
          Func_080150e4((i << 14) + 0x2000);
          SceneTransform_ApplyPitch(-0x1000);
          SceneTransform_ApplyYaw(frame << 10);
          Render_TransformVertices(gData_08199210, verts, 4);
          Render_DrawModelList(model);
        }
      }
    }
    Heap_Release(model);
    Heap_Release(verts);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }
  HeapSlot_Release(SLOT_BLITTER);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

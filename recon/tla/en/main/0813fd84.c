// Draft of BattleFx_RunCruelRuin, decompiled from the listing: differ 2715 of
// 2579 (draft 2630), the same 189 calls and control flow; allocation differs.

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

struct RuinObject {
  u8 unk0[8];
  u16 tile : 10;
  u16 unk08 : 6;
  u8 flags;
  u8 unk0b[5];
  u8 resource;
};

struct WordPair {
  s32 a;
  s32 b;
};

extern struct ResourceTableEntry gResourceTableEntries[];
extern u8 gMapCellBuffer[];
extern u8 gData_02014000[];
extern u8 Data_02012000[];
extern u32 gCameraSceneParameters[];
extern const struct WordPair Data_08196dd8;
extern const u16 Data_08197438[];
extern const u8 Data_081976e0[];
extern const u16 Data_081976f0[];
extern const u16 Data_081976f8[];
extern const u8 Data_08197700[];
extern const u8 Data_08197702[];
extern const u8 Data_08197704[];
extern const u8 Data_0819770a[];
extern const u8 Data_08197710[];
extern const u8 Data_08197716[];
extern const u8 Data_0819771c[];
extern const u8 Data_08197720[];
extern const u8 Data_08197724[];
extern const u8 Data_08197728[];
extern const u8 Data_0819772c[];
extern const u8 Data_08197730[];
extern const s32 Data_08197734[];
extern const u8 Data_081991c0[];
extern const u8 Data_081991e0[];
extern const u8 Data_08199220[];
extern const u8 gData_08199340[];
extern const u8 gData_08199364[];

struct RuinObject *Func_0815b290(s32 width, s32 height, u32 shape, s32 mode);
struct RuinObject *Func_0815b3b0(s32 width, s32 height, u32 shape, s32 mode);
void ResourceObject_ReleaseFar(void *object);
void Func_0813ba50(void);
void BattleBackground_LoadFar(s32 layer, s32 background, s32 palette);
void BattleActor_SpawnObjectsForListFar(s16 *list, s32 mode);
void Resource_DecodeType01(const void *source, void *destination);
void Palette_LoadBasicObjColors(void);
void Graphics_LoadGreyRamp(void);
void Func_0815b510(void *cells, s32 size, s32 scale);
void Func_08164a4c(s32 r, s32 g, s32 b);
void Func_08164abc(s32 r, s32 g, s32 b);
void Func_08164b2c(s32 r, s32 g, s32 b);
void ColorBuffer_ScaleFar(void *source, void *destination, s32 scale,
                          s32 count);
void MemFree(void *buffer);

#define SLOT_ALT 188 // unkb8[1]
#define OBJ_TILES(obj)                                                         \
  (0x06010000 + gResourceTableEntries[(obj)->resource].tile)
// The listing reads the transfer count twice per pass of its wait.
#define WAIT_TRANSFER(work)                                                    \
  while (*(volatile u32 *)&(work)->framesSinceTransfer,                        \
         *(volatile u32 *)&(work)->framesSinceTransfer <= 1)

// Cruel Ruin: a pillar of dark light swallows the caster, a shadowed sky
// rains ruin in six blasts while the ground splits, then a black sphere
// bursts over the foes.
void BattleFx_RunCruelRuin(struct BattleEffectArgument *effect) {
  u8 *canvas;
  struct BattleEffectWork *work;
  u8 *sprites;
  s32 *transition;
  DrawRectangle blit;
  u16 savedScroll;
  s32 scrollX;
  s32 phase;
  s32 *actor;
  s32 frame;
  s32 i;
  s32 j;
  s32 k;
  s32 fr;
  s32 fg;
  s32 fb;
  s32 dr;
  s32 dg;
  s32 db;
  s32 steps;
  s32 count;
  s32 level;
  u32 u;
  s32 place[4];
  struct WordPair req;
  struct ModelTexture texture;
  struct ModelTexture texture2;
  struct ModelTexture texture3;
  struct RenderModel *model;
  void *verts;
  struct EffectStep *p;
  struct RuinObject *obj;
  struct EffectPosition pos;

  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  sprites = Ram_HeapSlots->unk64[0];
  transition = (s32 *)Ram_HeapSlots->transitionWork;
  Func_0813ba50();
  BattleFx_BeginCanvasLayer(0);
  REG_BLDALPHA = 0x1010;
  BattleFx_LoadBlitter(SLOT_BLITTER, 3);
  work->transferMode = 2;
  work->transferValue = 50;
  blit = Ram_HeapSlots->unk64[1];
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 3200);
  savedScroll = gBgScroll[1].x;
  scrollX = 0;
  phase = 0;
  actor = *GetBattleObjectSlotFar(effect->actor);
  actor[18] = phase;
  Graphics_LoadGreyRamp();
  Audio_PlayCue(140);
  for (frame = 0; frame != 36; frame++) {
    s32 size;

    if (frame == 24) {
      work->transferMode = 4;
      work->transferValue = 0x06060606;
    }
    EffectPosition_ApplyAlternateStepAndYOffset(effect->actor, &pos);
    size = frame * 6 + 2;
    if (size > 96) {
      size = 96;
    }
    Func_0815b434(work->sheet, size, 0x10000);
    Func_0818caa8(work->sheet, pos.x / 2, pos.y - 16, size);
    if (frame > 15) {
      actor[3] += 0x100000;
    }
    work->transferPending = 1;
    WaitFrames(1);
  }
  actor[18] = 0xab85;
  actor[3] = 0;
  Iwram_FillWords((void *)0x06004000, 0x4000, 0x3f3f3f3f);
  gCameraSceneParameters[4] = 240;
  BattleFx_SelectLivingTargets(work->effect);
  BattleBackground_LoadFar(1, 0x71, 0);
  transition[4] = 1;
  gBgScroll[1].x = 0;
  REG_BG2PA = 0x80;
  REG_BG2X = 0;
  BattleFx_LoadBlitter(SLOT_BLITTER, 3);
  blit = Ram_HeapSlots->unk64[1];
  Resource_LoadAndDecompress(0x134, sprites, 0, 0);
  Resource_LoadAndDecompress(0xc2, gMapCellBuffer, 0, 0);
  Resource_LoadAndDecompress(0xec, work->sheet, 1, 0);
  Func_0816ae40(work->sheet, gData_02014000, 64, 64);
  Resource_LoadAndDecompress(0xb4, work->sheet + 0x2648, 0, 0);
  Resource_LoadAndDecompress(0xc9, work->sheet + 0x3648, 0, 0);
  Resource_LoadAndDecompress(0xca, work->sheet + 0x3688, 0, 0);
  for (i = 0; i != 10; i++) {
    Func_0815b510(work->sheet + 0x3a48 + Data_08197438[i], (i + 1) * 2,
                  0x10000);
  }
  Graphics_LoadGreyRamp();
  work->transferMode = 2;
  work->transferValue = 50;
  REG_BLDALPHA = 0x1010;
  fr = 0;
  fg = 0;
  fb = 0;
  dr = 0;
  dg = 0;
  db = 0;
  steps = 0;
  p = work->particles;
  for (i = 0; i != 24; i++, p++) {
    p->z = i * 0xaaa + (Random16() & 0xfff);
    p->variant = -(Random16() & 15);
  }
  p = &work->particles[32];
  for (i = 0; i != 31; i++, p++) {
    p->x = Random16() & 127;
    p->y = (Random16() & 31) + 88;
    p->z = (Random16() & 3) + 5;
    p->variant = i * -2;
  }
  for (frame = 0; frame != 180; frame++) {
    if ((gInput.repeat & 3) && frame <= 168) {
      frame = 169;
    }
    if (frame == 4) {
      Audio_PlayCue(220);
    }
    if (frame == 40) {
      Audio_PlayCue(154);
    }
    if (frame == 64) {
      Audio_PlayCue(139);
    }
    if (frame == 74) {
      Audio_PlayCue(139);
    }
    if (frame == 84) {
      Audio_PlayCue(139);
    }
    if (frame == 94) {
      Audio_PlayCue(139);
    }
    if (frame == 124) {
      Audio_PlayCue(144);
    }
    for (i = 0; i != 6; i++) {
      if (frame == 140 + i * 4) {
        Audio_PlayCue(104);
      }
    }
    if (frame == 169) {
      Audio_PlayCue(145);
      work->shakeFrames = 16;
    }
    if (frame == 0) {
      Func_08164b2c(0, 0, 0);
    }
    if (frame == 32) {
      fr = 0;
      fg = 0;
      fb = 0;
      dr = -0x2492;
      dg = -0x6db6;
      db = -0x2492;
      steps = 28;
    }
    if (frame == 124) {
      fr = 0x1f0000;
      fg = 0x80000;
      fb = -0x40000;
      dr = 0;
      dg = 0;
      db = 0;
      steps = 1;
    }
    if (frame == 128) {
      dr = (-0x10000 - fr) / 16;
      dg = (0x60000 - fg) / 16;
      db = -fb / 16;
      steps = 16;
    }
    if (frame == 144) {
      dr = (-0x90000 - fr) / 8;
      dg = (0xd0000 - fg) / 8;
      db = (-0x50000 - fb) / 8;
      steps = 8;
    }
    if (frame == 156) {
      dr = (0x1f0000 - fr) / 16;
      dg = (0x80000 - fg) / 16;
      db = (-0x40000 - fb) / 16;
      steps = 16;
    }
    if (steps > 0) {
      steps--;
      fb += db;
      fr += dr;
      fg += dg;
      Func_08164b2c(fr >> 16, fg >> 16, fb >> 16);
    }
    if (frame == 124) {
      Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
    }
    if (frame <= 123) {
      s32 y = 58;
      s32 scale = 0x20000;
      s32 size;

      if (frame <= 29) {
        size = 140 - frame * 4;
      } else {
        size = 114 - frame * 3;
      }
      if (frame > 7) {
        scale = 0x48000 - (frame << 11);
      }
      if (scale <= 0x7fff) {
        scale = 0x8000;
      }
      if (frame > 35) {
        y = 20;
      } else if (frame > 19) {
        y = (Trig_Cos((frame << 10) + -0x5000) * 44 >> 16) + 14;
      }
      if (size <= 7) {
        size = 8;
      }
      if (frame > 67) {
        size += (frame - 68) / 6;
      }
      Func_0815b434(work->sheet, size, scale);
      Func_0818caa8(work->sheet, 60, y, size);
    }
    if (frame > 127) {
      p = &work->particles[32];
      for (i = 0; i != 31; i++, p++) {
        if ((u32)p->variant <= 15) {
          s32 s = Trig_Sin(p->variant << 10) * p->z >> 16;

          blit(canvas, work->sheet + 0x3a48 + Data_08197438[s - 1], p->x - s,
               p->y - s * 2, s * 2, s * 4);
        }
        p->variant++;
      }
    }
    verts = Runtime_BumpAllocateAlternatePool(32);
    model = Render_AllocateModelList(1);
    texture.logWidth = 7;
    texture.logHeight = 7;
    model->kind = 6;
    model->texture = &texture;
    model->mesh = gData_08199364;
    model->shade = 0;
    model->vertices = verts;
    texture.pixels = gMapCellBuffer;
    {
      s32 t = frame - 64;
      s32 s = (frame << 15) + -0x200000;

      for (i = 0; i != 4; i++, s += -0x50000, t -= 10) {
        if ((u32)t <= 15) {
          s32 sc = 0x80000 - s;

          Render_ResetTransformState();
          Func_08015160(-0x40000, -0x2c0000, 0);
          Func_080151e4(sc / 2, sc, sc);
          SceneTransform_ApplyYaw(Data_081976f8[i]);
          SceneTransform_ApplyPitch(Data_081976f0[i]);
          Render_TransformVertices(Data_081991e0, verts, 4);
          Render_DrawModelList(model);
        }
      }
    }
    u = frame - 124;
    if (u <= 15) {
      s32 sc = u << 15;
      s32 sh = 64 - u * 8;

      if (sh > 0) {
        sh = 0;
      }
      model->shade = sh;
      Render_ResetTransformState();
      Func_08015160(-0x40000, -0x2c0000, 0);
      Func_080151e4(sc >> 1, sc, sc);
      Render_TransformVertices(Data_081991e0, verts, 4);
      Render_DrawModelList(model);
    }
    {
      s32 t = frame - 24;

      if ((u32)t <= 99) {
        s32 sc = 0x30000 - (t << 11);
        s32 sh = -32;
        s32 half;
        s32 angle;

        if (sc <= 0xffff) {
          sc = 0x10000;
        }
        if (t <= 7) {
          sh = t * 4 - 64;
        }
        if (frame > 63) {
          sh = 224 - frame * 4;
        }
        model->shade = sh;
        texture.pixels = gData_02014000;
        angle = t << 9;
        Render_ResetTransformState();
        Func_08015160(-0x40000, -0x2c0000, 0);
        half = sc >> 1;
        Func_080151e4(sc, sc / 3, half);
        Func_080150e4(angle);
        if (sh > -64) {
          Render_TransformVertices(Data_081991e0, verts, 4);
          Render_DrawModelList(model);
        }
        texture.pixels = work->sheet + 0x2648;
        texture.logWidth = 6;
        texture.logHeight = 6;
        model->mesh = gData_08199340;
        if (frame > 63) {
          sh = frame * 4 + -320;
          if (sh > 0) {
            sh = 0;
          }
          model->shade = sh;
          Render_ResetTransformState();
          Func_08015160(-0x40000, -0x2c0000, 0);
          Func_080151e4(sc, half, half);
          Func_080150e4(angle);
          Render_TransformVertices(Data_081991e0, verts, 4);
          Render_DrawModelList(model);
        }
      }
    }
    if (frame > 123) {
      texture.logWidth = 5;
      texture.logHeight = 2;
      texture.pixels = work->sheet + 0x3648;
      model->mesh = Data_08199220;
      model->kind = 6;
      model->shade = -16;
      p = work->particles;
      for (i = 0; i != 24; i++, p++) {
        if (p->variant >= 0) {
          s32 sy = p->variant << 13;

          if (sy > 80000) {
            sy = 80000;
          }
          Render_ResetTransformState();
          Func_08015160(-0x40000, -0x2c0000, 0);
          Func_080151e4(0x8000, 0x10000, 0x8000);
          Func_080150e4(p->z);
          Func_080151e4(30384, sy, 0x8000);
          Render_TransformVertices(Data_081991c0, verts, 4);
          Render_DrawModelList(model);
        }
        p->variant++;
      }
      if (frame > 163) {
        work->transferMode = 1;
        work->transferValue = 0x01010101;
        Func_08164a4c(2, 2, 2);
      }
      if (frame > 159) {
        s32 r = frame * 2 + -288;
        struct EffectStep *q = &work->particles[63];

        if (r > 64) {
          r = 64;
        }
        if (frame == 160) {
          q->x = 0x800000;
          q->y = -0x200000;
        }
        if (frame <= 171) {
          q->x += -0x80000;
          q->y += 0x100000;
          Func_0815b434(work->sheet, r, 0x10000);
          Func_0818caa8(work->sheet, HI(q->x), HI(q->y), r);
        }
        model->kind = 6;
        model->shade = 0;
        texture.pixels = work->sheet + 0x3688;
        Render_ResetTransformState();
        Func_08015160(q->x + -0x3f0000, q->y + -0x400000, 0);
        Func_080151e4(0x8000, 0x10000, 0x8000);
        Func_080150e4(0xa000);
        Func_080151e4(r * 9400, 0x20000, 0x8000);
        Render_TransformVertices(Data_081976e0, verts, 4);
        Render_DrawModelList(model);
      }
    }
    MemFree(model);
    MemFree(verts);
    if (work->shakeFrames > 0) {
      gBgScroll[1].y = (Random16() & 7) + 28;
      work->shakeFrames--;
    } else {
      gBgScroll[1].y = work->savedBg1Y;
    }
    ObjectGroup_TickMemberTimers();
    WAIT_TRANSFER(work);
    work->transferPending = 1;
    WaitFrames(1);
  }
  count = -1;
  Render_BuildQuadStrip(Data_02012000, 8, 16, 32);
  for (k = 0; k != 2; k++) {
    s32 offset = k * 32;
    s32 c = 0;

    for (j = 0; j != 33; j++, c += 7) {
      s32 w = Data_08197702[k];
      s8 *v;
      s32 acc;
      s32 m;

      if (j > 15) {
        w = w + j * 2 - 32;
      }
      v = (s8 *)gMapCellBuffer + (offset + k + j) * 8;
      acc = 0;
      for (m = 0; m != 2; m++, acc += w, v += 4) {
        v[0] = 126 - c;
        v[1] = (Trig_Sin(j * 0x92c) * Data_08197700[k] >> 16) + acc - w / 2;
        v[2] = 0;
      }
    }
  }
  work->transferMode = 2;
  work->transferValue = 50;
  Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
  work->transferPending = 1;
  WaitFrames(1);
  Iwram_FillWords(canvas, 0x4000, 0);
  REG_BG2CNT = 0x786;
  BattleBackground_LoadFar(1, 0x45, 0);
  Func_08164abc(31, 31, 31);
  Resource_LoadAndDecompress(0xca, work->sheet + 0x3648, 0, 0);
  Resource_LoadAndDecompress(0x104, work->sheet + 0x36c8, 0, 0);
  Graphics_LoadGreyRamp();
  {
    u8 *e = Resource_GetTableEntry(0xa0);

    Iwram_FillWords((void *)(BG_PLTT + 0x3e0), 32, 0x7fff7fff);
    Resource_DecodeType01(e + 32, work->sheet);
  }
  for (k = 0; k != 2; k++) {
    u8 *src = work->sheet + k * 0x1000;

    for (j = 0; j != 8; j++) {
      obj = Func_0815b290(32, 32, 0x80000000, 0xf000);
      work->objects[k * 9 + j] = obj;
      obj->flags = (obj->flags & ~12) | 4;
      Iwram_CopyWords((void *)OBJ_TILES(obj), src, 512);
      src += 512;
    }
    obj = Func_0815b3b0(32, 32, 0x80000000, 0xf000);
    work->objects[k * 9 + 8] = obj;
    Iwram_CopyWords(obj, work->objects[k * 9], 24);
    for (j = 0; j != 8; j++) {
      work->particles[k * 9 + j].variant =
          ((struct RuinObject *)work->objects[k * 9 + j])->tile;
    }
  }
  WaitFrames(1);
  level = 0;
  for (frame = 0; frame != 102; frame++, level += 2) {
    if ((gInput.repeat & 3) && frame <= 84 && count == -1) {
      work->transferMode = 4;
      work->transferValue = 0x04040404;
      count = 17;
      REG_BG2CNT = 0x784;
    }
    if (frame <= 17) {
      s32 g = 31 - level;
      s32 b = 31 - (level + frame);
      s32 r = g;

      if (g < 0) {
        r = 0;
      }
      if (g <= 5) {
        g = 6;
      }
      if (b < -18) {
        b = -18;
      }
      Func_08164b2c(r, g, b);
      if (frame <= 16) {
        u16 *e = Resource_GetTableEntry(0xa0);
        u16 *dst = (u16 *)(BG_PLTT + 0x3e0);
        s32 add = 32 - level;

        for (i = 0; i != 16; i++) {
          u32 c = *e++;
          s32 cr = (c & 31) + add;
          s32 cg = ((c << 16) >> 21 & 31) + add;
          s32 cb = ((c << 16) >> 26 & 31) + add;

          if (cr > 31) {
            cr = 31;
          }
          if (cg > 31) {
            cg = 31;
          }
          if (cb > 31) {
            cb = 31;
          }
          *dst++ = cb << 10 | cg << 5 | cr;
        }
      }
    }
    if (frame == 0) {
      REG_BG2CNT = 0x786;
    }
    if (frame == 75) {
      REG_BG2CNT = 0x784;
    }
    if (frame == 85) {
      work->transferMode = 4;
      work->transferValue = 0x04040404;
    }
    verts = Runtime_BumpAllocateAlternatePool(528);
    model = Render_AllocateModelList(1);
    texture2.logWidth = 5;
    texture2.logHeight = 2;
    texture2.pixels = work->sheet + 0x3648;
    model->shade = 0;
    model->mesh = Data_08199220;
    model->texture = &texture2;
    model->vertices = verts;
    model->kind = 6;
    for (i = 0; i != 6; i++) {
      s32 d = frame - Data_08197704[i];

      if (d == 0) {
        if (i == 5) {
          Audio_PlayCue(145);
        } else {
          Audio_PlayCue(144);
        }
        work->shakeFrames = 8;
        Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
      }
      if ((u32)d <= 15) {
        s32 r = (Trig_Sin(d << 11) * Data_0819770a[i] >> 16) + 2;

        if (r > 1) {
          s32 x = Data_08197710[i] + d * 8;
          s32 y = Data_08197716[i] - r + 8;
          s32 half;

          Func_0815b434(work->sheet, r, 0x10000);
          BattleFx_LoadBlitter(SLOT_ALT, 19);
          half = r / 2;
          ((DrawRectangle)Ram_HeapSlots->unkb8[1])(canvas, work->sheet,
                                                   x - half, y, half, r);
          Runtime_ReleaseHeapBlock(SLOT_ALT);
          BattleFx_LoadBlitter(SLOT_ALT, 23);
          ((DrawRectangle)Ram_HeapSlots->unkb8[1])(canvas, work->sheet, x, y,
                                                   half, r);
          Runtime_ReleaseHeapBlock(SLOT_ALT);
        }
      }
    }
    p = &work->particles[18];
    for (i = 0; i != 4; i++, p++) {
      s32 start = Data_0819771c[i] >> 1;

      if (frame == 0) {
        p->y = frame;
        p->variant = frame;
        p->x = Data_08197724[i] << 16;
      }
      if (frame >= start && frame < start + 32) {
        s32 size = Data_08197720[i];

        if (p->y < Data_08197730[i] << 16) {
          p->x -= Data_08197728[i] << 16;
          p->y += Data_0819772c[i] << 16;
        } else {
          p->variant++;
        }
        p->x += 0x80000;
        Render_ResetTransformState();
        Func_08015160(p->x + -0x400000, p->y + -0x400000, 0);
        Func_080151e4(0x8000, 0x10000, 0x8000);
        Func_080150e4(Data_08197734[i]);
        Func_080151e4(size * 9400, 0x20000, 0x8000);
        Render_TransformVertices(Data_081976e0, verts, 4);
        Func_0815b434(work->sheet, size, 0x10000);
        Func_0818caa8(work->sheet, HI(p->x) - 1, HI(p->y), size);
        Render_DrawModelList(model);
      }
    }
    texture2.logWidth = 6;
    texture2.logHeight = 5;
    texture2.pixels = work->sheet + 0x36c8;
    Render_BuildQuadStrip(Data_02012000, 2, 32, 32);
    model->mesh = Data_02012000;
    model->scrollU = 64;
    model->kind = 6;
    {
      u8 *cells = gMapCellBuffer;

      for (i = 0; i != 2; i++, cells += 264) {
        s32 t = frame - i * 46 - 12;

        if ((u32)t <= 45) {
          s32 x;

          if (i == 0) {
            model->scrollU = 127 & ~(t * 2 + 63);
            if (t <= 32) {
              u8 *m = Data_02012000 + t * 24;

              m[0] = i;
              m[1] = i;
              m[2] = i;
            }
          } else {
            model->scrollU = 127 & ~(t * 4 + 63);
            if (t <= 15) {
              u8 *m = Data_02012000 + t * 48;

              m[0] = 0;
              m[1] = 0;
              m[2] = 0;
            }
          }
          Render_ResetTransformState();
          x = -0x180000;
          if (t >= 0) {
            x = (t << 16) + -0x180000;
          }
          if (i == 0) {
            if (t > 31) {
              x += (t * 7 - 224) << 16;
            }
          } else if (t > 15) {
            x += (t * 7 - 112) << 16;
          }
          Func_08015160(x, -0x100000, 0);
          Func_080151e4(45536, 0x10000, 0x8000);
          if (i == 0) {
            Func_080150e4(-0xbe8);
          } else {
            Func_080150e4(-0x400);
          }
          Render_TransformVertices(cells, verts, 66);
          Render_DrawModelList(model);
        }
      }
    }
    MemFree(model);
    MemFree(verts);
    req = Data_08196dd8;
    place[3] = 0;
    place[1] = 0xff0000;
    scrollX += 0x60000;
    if (scrollX > 0x1fffff) {
      phase = (phase + 7) % 8;
      scrollX += -0x200000;
    }
    gBgScroll[1].x += 0xfff8;
    for (k = 0; k != 2; k++) {
      for (j = 0; j != 9; j++) {
        s32 m;

        place[0] = scrollX + (j << 21);
        place[2] = 0x790000 + (k << 21);
        obj = work->objects[k * 9 + j];
        if (j == 8) {
          m = (phase + 71) % 8;
        } else {
          m = (phase + 64 + j) % 8;
        }
        obj->tile = work->particles[k * 9 + m].variant;
        if (place[0] > 0xffffff) {
          place[0] += -0x1200000;
        }
        Render_ApplyProjectedPlacementFar(work->objects[k * 9 + j], place,
                                          (s32 *)&req, 0);
      }
    }
    ObjectGroup_TickMemberTimers();
    WAIT_TRANSFER(work);
    if (work->shakeFrames > 0) {
      gBgScroll[1].y = (Random16() & 7) + 28;
      work->shakeFrames--;
    } else {
      gBgScroll[1].y = work->savedBg1Y;
    }
    work->transferPending = 1;
    WaitFrames(1);
    if (count > 0 && --count == 0) {
      break;
    }
  }
  gBgScroll[1].x = savedScroll;
  transition[4] = 0;
  gBgScroll[1].y = 32;
  BattleBackground_LoadFar(1, *(u16 *)((u8 *)Ram_HeapSlots->battleWork + 0x670),
                           24);
  WaitFrames(1);
  gCameraSceneParameters[4] = 120;
  Audio_PlayCue(390);
  for (i = 0; i != 18; i++) {
    ResourceObject_ReleaseFar(work->objects[i]);
  }
  Palette_LoadBasicObjColors();
  Dma_Set((const void *)OBJ_PLTT, (void *)(BG_PLTT + 0xa0), 0x84000010,
          (volatile u32 *)0x040000d4);
  *(volatile u16 *)(BG_PLTT + 0xbc) = *(volatile u16 *)(BG_PLTT + 0x1e8);
  work->transferMode = 3;
  work->transferValue = 0x02020202;
  Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
  REG_BG2CNT = 0x784;
  effect->actors[effect->count] = 0xff;
  BattleActor_SpawnObjectsForListFar(effect->actors, 0);
  Resource_LoadAndDecompress(0xca, work->sheet + 0x3840, 0, 0);
  REG_BLDALPHA = 0x1010;
  for (i = 0; i != 120; i++) {
    s32 v = i / 2;
    u8 *c;

    if (v > 63) {
      v = 63;
    }
    c = work->sheet + i;
    for (j = 0; j != 120; j++) {
      *c = v;
      c += 120;
    }
  }
  {
    s32 cr = -0xa00;
    s32 cg = -0x14000;
    s32 cb = -80;

    for (frame = 0; frame != 140; frame++, cr += 32, cg += 0x400, cb++) {
      if (frame == 0) {
        for (i = 0; i != effect->count; i++) {
          Func_0814cd48(effect->actors[i], 0, -1, i, 16);
        }
      }
      if (frame == 127) {
        Func_081180e8(134);
      }
      if (frame == 107) {
        Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
      }
      if (frame > 107) {
        work->transferMode = 3;
        work->transferValue = 0x02020202;
        Func_08164b2c(0, 0, 0);
      }
      if (frame == 32) {
        Audio_PlayCue(140);
      }
      if (frame == 48) {
        work->shakeFrames = 90;
        Audio_PlayCue(145);
      }
      if (frame == 72) {
        Audio_PlayCue(208);
      }
      if (frame <= 109) {
        if (frame > 31) {
          s32 a = (frame - 32) / 4;
          s32 b = 0;

          if (frame > 47) {
            b = (frame - 48) / 2;
          }
          Func_08164abc(a, a / 2, b);
        }
        if ((u32)cb <= 31) {
          u32 c = cg | cr | cb;
          u16 *dst = (u16 *)(BG_PLTT + 2);

          for (i = 0; i != 223; i++) {
            *dst++ = c;
          }
        } else if (frame > 47) {
          ColorBuffer_ScaleFar((void *)BG_PLTT, (void *)BG_PLTT, 0xf000, 224);
        }
        if (frame > 47) {
          blit(canvas, work->sheet, 216 - frame * 2, 0, 120, 120);
        }
        verts = Runtime_BumpAllocateAlternatePool(32);
        model = Render_AllocateModelList(1);
        texture3.logWidth = 5;
        texture3.logHeight = 2;
        texture3.pixels = work->sheet + 0x3840;
        model->shade = 0;
        model->mesh = Data_08199220;
        model->texture = &texture3;
        model->vertices = verts;
        model->kind = 6;
        if (frame > 31) {
          s32 r = frame * 2 - 32;
          struct EffectStep *q = &work->particles[63];

          if (r > 64) {
            r = 64;
          }
          if (frame == 32) {
            q->x = 0x800000;
            q->y = -0x200000;
          }
          if (frame <= 43) {
            q->x += -0x80000;
            q->y += 0x100000;
            Func_0815b434(work->sheet + 0x38c0, r, 0x10000);
            Func_0818caa8(work->sheet + 0x38c0, HI(q->x), HI(q->y), r);
          }
          Render_ResetTransformState();
          Func_08015160(q->x + -0x3f0000, q->y + -0x400000, 0);
          Func_080151e4(0x8000, 0x10000, 0x8000);
          Func_080150e4(0xa000);
          Func_080151e4(r * 9400, 0x20000, 0x8000);
          Render_TransformVertices(Data_081976e0, verts, 4);
          Render_DrawModelList(model);
        }
        MemFree(model);
        MemFree(verts);
      }
      {
        s32 r = 8;

        if (frame > 130) {
          r = 2;
        } else if (frame > 120) {
          r = 4;
        } else if (frame > 110) {
          r = 6;
        }
        Func_08158ce0(r, r);
      }
      work->transferPending = 1;
      WaitFrames(1);
    }
  }
  Palette_LoadBasicObjColors();
  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  Runtime_ReleaseHeapBlock(SLOT_BLITTER);
  BattleFx_EndCanvasLayer();
}

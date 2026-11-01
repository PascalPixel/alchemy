// Draft, differ 1641 of 1932, skeleton gap 401 (tla-en, 2026-11-01)
// BattleEffect_RunUlysses: the same calls and control flow as the listing.
// Most of the gap is scheduling and allocation following from the frame: 160
// here, 152 in the listing, whose spill slots from the top read effect, canvas,
// transition, frame, sprites, dmaArmed, skip, blit, shipZ, shipX, loaded, draw,
// frame - 108, verts, model, sparks, &texture, delay, block two's verts and j,
// the anchor pointer, frame * 2 - 184, frame - 92, &req.
// Left to find:
// - battle is spilled here and sits in r10 in the listing. The constants 0x3c00
//   and 0x06004000 take r8 and r10 here; the listing has no register for them,
//   so reload rebuilds them at each use. That one change clears many hunks.
// - frame - 92 stays unreduced: as a user variable it loses copy_cost and as a
//   temporary its benefit is only add_cost, so loop.c rejects it (the listing
//   keeps it and frame * 2 - 184 at sp+12 and sp+16). A second, uncombined copy
//   of the expression would raise the benefit; CSE and GCSE merged all tried.
// - delay and i * 448 are reduced here, not in the listing: particles[i + 8]
//   shares i * 8 and i * 7 with them. A real anchor pointer stops the i * 7
//   sharing and brings the frame to 156, but the gap rises to 424.
// - bg2pa and bg2pd: the listing does a discarded ldrh before each strh, as a
//   store to a volatile 16-bit field would; not tried, it would be a device.
// - The first colour loop's v & 31 mask is hoisted in the listing; here its life
//   of 1 fails loop.c's test. The last loop's *src read combines into ldrsh;
//   a volatile src closes that hunk, also a device.

#include "types.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "effect_step.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "heap_release.h"
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
#include "input.h"
#include "io_write_queue.h"

extern char ResourceId_Data0B1;
extern char ResourceId_Data0B3;
extern char ResourceId_Data0C7;
extern char ResourceId_Data0C8;
extern char ResourceId_Data137;
extern char ResourceId_Data13A;
extern char ResourceId_Data150;
extern char ResourceId_Data15C;
extern char ResourceId_Data178;
extern u8 gMapCellBuffer[];
extern u8 Data_02010200[];
extern const u16 Data_08197424[];
extern const u16 gData_0819747a[];
extern const u8 gData_08197467[];
extern const u8 gData_08197473[];
extern const u8 gData_0819746d[];
extern const u16 Data_08198686[];
extern const u8 Data_08198662[];
extern const u8 Data_08198674[];
extern const s8 Data_081986aa[];
extern const u8 Data_08198654[];
extern const u8 Data_0819865b[];
extern const s32 Data_08196e54[2];
extern const u8 gData_08199340[];
extern const u8 gData_08199364[];
extern const u8 gData_08199210[];
extern s32 gCameraSceneParameters[];

void BattleEffect_ProcessPendingBlock2Transfer(void);
void Display_ApplyBg2Affine(void);
void Palette_StepFadeTransfer(void);
void Func_0815b68c(void);
void Func_0815f0a0(s32 palette);
void ResourceObject_ReleaseFar(void *object);
void BattleBackground_LoadFar(s32 layer, s32 background, s32 palette);

// Ulysses: the hero's ship sails out of a whirlpool, its crew of six
// warriors leap at the foe and the ship's broadside lands as a storm of
// explosions.
void BattleEffect_RunUlysses(struct BattleEffectArgument *effect) {
  u8 *canvas;
  s32 *transition;
  struct BattleEffectWork *work;
  s32 frame;
  u8 *sprites;
  s32 dmaArmed;
  s32 skip;
  DrawRectangle *blit;
  s32 shipZ;
  s32 shipX;
  s32 loaded;
  s32 draw;
  s32 t;
  s32 i;
  u8 *cells;
  s32 place[4];
  DrawRectangle blits[2];
  struct EffectStep *p;
  struct EffectStep *q;
  struct ModelTexture texture;
  struct ModelTexture texture2;
  s32 req[2];
  u8 *battle;

  canvas = Ram_HeapSlots->battleFxWork[1];
  transition = Ram_HeapSlots->transitionWork;
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  sprites = Ram_HeapSlots->unk64[0];
  dmaArmed = 0;
  skip = 0;
  BattleEffect_SelectLivingTargets(work->effect);
  for (frame = 0; frame != 32; frame++) {
    volatile u16 *c = (volatile u16 *)(BG_PLTT + 96);

    for (i = 0; i != 128; i++, c++) {
      u16 v = *c;
      s32 r = v & 31;
      s32 g = (u16)((v >> 5) & 31);
      s32 b = (u16)((v >> 10) & 31);

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
    u32 *map = (u32 *)0x0600f800;
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
  REG_BG2PA = 0x80;
  REG_BG2PA = 0x100;
  REG_BLDALPHA = 0x1010;
  REG_BLDCNT = 0x3f46;
  REG_BG1CNT = 0x1f83;
  wait(1);
  blit = blits;
  BattleEffect_FetchRectangleBlitters(0, (void **)blit);
  work->transferMode = 1;
  work->transferValue = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 3200);
  cells = work->sheet + 0x1781;
  Resource_LoadAndDecompress((s32)&ResourceId_Data0B1, cells, 0, 0);
  for (i = 0; i != 0x2300; i++, cells++) {
    if (*cells) {
      *cells += 128;
    }
  }
  cells = work->sheet + 0x1781;
  blits[0](canvas, cells, 24, 8, 80, 112);
  work->transferPending = 1;
  wait(1);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  Resource_LoadAndDecompress((s32)&ResourceId_Data178, work->sheet, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0B3, cells, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data137, sprites, 0, 0);
  transition[4] = 1;
  loaded = 0;
  shipX = 0xc80000;
  shipZ = 0x540000;
  Iwram_ClearWords(canvas, 0x3c00);
  REG_DISPCNT = 0x7141;
  wait(1);
  for (frame = 0; frame != 326; frame++) {
    draw = 1;
    if (gInput.repeat & 3) {
      skip = 1;
    }
    // The listing masks the frame with draw (still 1) here, not a literal 1.
    if ((frame & draw) && skip == 1) {
      draw = 0;
    }
    if (frame == 0) {
      if (!loaded) {
        Task_Add(BattleEffect_ProcessPendingBlock2Transfer, 0xc8f);
        Task_Add(Display_ApplyBg2Affine, 0xcff);
        Resource_LoadAndDecompress((s32)&ResourceId_Data0C7, work->sheet + 0x2781, 1, 0);
        loaded = 1;
      }
      gBgScroll[1].x = 0xff90;
      REG_BLDALPHA = 0x1010;
      REG_DISPCNT = 0x7741;
      REG_BG2CNT = 0x784;
      REG_BG1CNT = 0x1f80;
      work->transferMode = 2;
      work->transferValue = 75;
      Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry((s32)&ResourceId_Data150), 128);
      p = work->particles;
      for (i = 0; i != 9; i++, p++) {
        p->variant = -2 * i;
        p->x = (Random16() & 15) + 60;
        p->y = (Random16() & 15) + 44;
      }
    }
    if (frame <= 75) {
      s32 k = (frame / 4) % 18;
      s32 x = 60 - ((Trig_Sin(frame * 860) << 3) >> 16);
      u8 w = Data_08198662[k];
      u8 h = Data_08198674[k];

      blit[1](canvas, work->sheet + 0x1781 + Data_08198686[k], x - (w >> 1),
              frame - 8 - (h >> 1), w, h);
    }
    if ((u32)(frame - 76) <= 31) {
      p = work->particles;
      for (i = 0; i != 4; i++, p++) {
        if ((u32)p->variant <= 17) {
          s32 k = p->variant / 3;
          u8 w = gData_08197467[k];

          blit[i & 1](canvas, work->sheet + gData_0819747a[k], p->x - (w >> 1),
                      p->y + gData_08197473[k] - 32, w, gData_0819746d[k]);
        }
        p->variant++;
      }
    }
    if (frame == 108) {
      REG_BLDALPHA = 16;
    }
    if ((u32)(frame - 108) <= 8) {
      REG_BLDALPHA = ((frame - 108) * 512) | 16;
    }
    t = frame - 92;
    if ((u32)t <= 39) {
      s32 z = Trig_Cos(t * 800) / 3 + 0x10000;
      s32 q16;
      s32 level = -1;

      if (z <= 0x107ff) {
        z = 0x10800;
      }
      q16 = 0x1000000 / z;
      work->bg2pa = q16;
      work->bg2pd = q16;
      work->bg2x = (64 - 0xb80000 / z) << 8;
      work->bg2y = (64 - 0x4c0000 / z) << 8;
      if (t <= 15) {
        level = t;
      } else if ((u32)(frame - 116) <= 8) {
        level = 64 - t * 2;
      }
      if (level != -1) {
        volatile u16 *pal = (volatile u16 *)(BG_PLTT + 128);

        for (i = 0; i != 32; i++) {
          s32 g = (i * ((level * 9 * 4) / 16)) / 64;

          *pal++ = ((i * level * 3) / 64) << 10 | g;
        }
      }
      if (frame == 116) {
        REG_DISPCNT = 0x7341;
      }
    }
    if ((u32)(frame - 124) <= 7) {
      volatile u16 *c = (volatile u16 *)BG_PLTT;

      for (i = 0; i != 64; i++, c++) {
        u16 v = *c;
        s32 r = (v & 31) - 3;
        s32 g = ((u16)((v >> 5) & 31)) - 3;
        s32 b = ((u16)((v >> 10) & 31)) - 3;

        if (r < 0)
          r = 0;
        if (g < 0)
          g = 0;
        if (b < 0)
          b = 0;
        *c = g << 5 | b << 10 | r;
      }
    }
    if (frame == 132) {
      if (loaded == 1) {
        Task_Remove(BattleEffect_ProcessPendingBlock2Transfer);
        Task_Remove(Display_ApplyBg2Affine);
        Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc8f);
        Resource_LoadAndDecompress((s32)&ResourceId_Data15C, work->sheet + 0x1e97, 1, 0);
        loaded = 0;
      }
      REG_DISPCNT = 0x7141;
      REG_BG2X = -0x800;
      REG_BG2Y = -0x1000;
      REG_BG2PA = 0xcc;
      REG_BG2PD = 0x100;
      REG_BG2CNT = 0x784;
      REG_BLDALPHA = 0x1010;
      Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry((s32)&ResourceId_Data13A), 128);
    }
    if (frame == 133) {
      const s8 *src = Data_081986aa;

      Iwram_FillWords(canvas, 0x3c00, 0);
      Iwram_ClearWords((void *)0x06004000, 0x3c00);
      REG_DISPCNT = 0x7541;
      q = (struct EffectStep *)(work->sheet + 0x4b98);
      p = work->particles;
      for (i = 0; i != 6; i++, p++, src += 5) {
        p->variant = -8 * i;
        p->x = src[0] << 16;
        p->y = src[1] << 16;
        p->z = src[2] << 6;
        p->velocityX = src[3] << 15;
        p->velocityY = src[4] << 15;
        p->velocityZ = 0;
      }
      for (i = 0; i != 96; i++) {
        q[i].variant = -1;
      }
      Resource_LoadAndDecompress((s32)&ResourceId_Data0C8, gMapCellBuffer, 0, 0);
    }
    if (frame == 76) {
      Sound_PlayFar(138);
    }
    if (frame == 266) {
      Sound_PlayFar(138);
    }

    if ((u32)(frame - 133) <= 132) {
      void *verts;
      struct RenderModel *model;
      struct EffectStep *sparks;
      s32 j;

      verts = Heap_AllocEwram(32);
      model = Render_AllocateModelList(1);
      sparks = (struct EffectStep *)(work->sheet + 0x4b98);
      if (frame > 196) {
        Func_0815f0a0((s32)&ResourceId_Data150);
      }
      texture.logWidth = 6;
      texture.logHeight = 6;
      model->kind = 7;
      model->shade = 0;
      model->texture = &texture;
      model->mesh = gData_08199340;
      model->vertices = verts;
      p = work->particles;
      for (i = 0; i != 6; i++, p++) {
        s32 delay = i * 8;
        s32 t = ++p->variant;

        if (t == 0) {
          Sound_PlayFar(138);
        }
        if (p->variant == 60) {
          Sound_PlayFar(212);
        }
        if (p->variant == 64) {
          Sound_PlayFar(134);
        }
        t = p->variant;
        if ((u32)t <= 35) {
          s32 k = (t / 6) % 6;
          u8 w = gData_08197467[k];

          blits[0](canvas, work->sheet + gData_0819747a[k], HI(p->x) - (w >> 1),
                   HI(p->y) + gData_08197473[k] - 36, w, gData_0819746d[k]);
          t = p->variant;
        }
        if ((u32)(t - 12) <= 31) {
          s32 k = ((t - 12) / 4) % 18;
          u8 w = Data_08198662[k];
          u8 h = Data_08198674[k];

          blit[1](canvas, work->sheet + 0x1781 + Data_08198686[k],
                  HI(p->x) - (w >> 1), HI(p->y) - (h >> 1), w, h);
          t = p->variant;
        }
        if ((u32)(t - 44) <= 21) {
          s32 s = (Trig_Sin((t - 44) << 10) * 48) / 64;
          s32 mx = p->x - 0x400000;
          s32 my = p->y - 0x400000;
          s32 u = t - 59;

          if (u < 0) {
            u = 0;
          }
          p->y += p->velocityY;
          p->x += p->velocityX;
          if (p->x <= 0x3fffff) {
            p->velocityX += 0x8000;
          }
          if (p->y <= 0x3bffff) {
            p->velocityY += 0x8000;
          } else {
            p->velocityY -= 0x8000;
          }
          if (p->z >= 0) {
            p->z += (Trig_Sin(p->velocityZ) * 3 << 8) >> 16;
          } else {
            p->z -= (Trig_Sin(p->velocityZ) * 3 << 8) >> 16;
          }
          p->velocityZ += 1024;
          if (draw) {
            texture.pixels = gMapCellBuffer + (u << 12);
            Render_ResetTransformState();
            Func_08015160(mx, my, 0);
            SceneTransform_ApplyPitch(0xc000);
            SceneTransform_ApplyYaw(p->z);
            Func_0801521c(s);
            Render_TransformVertices(gData_08199210, verts, 4);
            Render_DrawModelList(model);
          }
          t = p->variant;
        }
        if (t == 64) {
          work->particles[i + 8].x = HI(p->x);
          work->particles[i + 8].y = HI(p->y);
          q = sparks + i * 16;
          for (j = 0; j != 16; j++, q++) {
            s32 a = Random16() & 0xffff;
            s32 sp = (Random16() & 15) + 8;

            q->variant = (Random16() & 7) + 8;
            q->x = p->x;
            q->y = p->y;
            q->velocityX = (sp * Trig_Sin(a)) / 4;
            q->velocityY = (sp * Trig_Cos(a)) / 4;
          }
          for (j = 0; j != effect->count; j++) {
            s32 m = (Random16() << 2) + 0x10000;

            BattleEffect_StartActorMotion(effect->actors[j], 1, 0x20000, 0x80000, m,
                                      delay + (Random16() & 31) + 52);
            Func_0814cd48(effect->actors[j], 7, -1, j, 4);
          }
        }
        if (draw && (u32)(p->variant - 64) <= 11) {
          s32 k = ((p->variant - 64) / 2) % 6;

          blit[1](canvas, work->sheet + 0x1e97 + k * 0x780,
                  work->particles[i + 8].x - 20,
                   work->particles[i + 8].y - 24, 40, 48);
        }
      }
      q = sparks;
      for (i = 0; i != 96; i++, q++) {
        if (q->variant >= 0) {
          s32 n = q->variant / 4 + 2;

          if (draw) {
            blit[1](canvas, sprites + Data_08197424[n - 1], HI(q->x) - n,
                     HI(q->y) - n, n * 2, n * 2);
          }
          BattleEffectKernels_IntegrateVector2(q, 60, 0x2000);
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
      model->shade = 0;
      work->transferMode = 2;
      work->transferValue = 50;
      texture2.logWidth = 7;
      texture2.logHeight = 7;
      texture2.pixels = work->sheet + 0x2781;
      model->texture = &texture2;
      model->mesh = gData_08199364;
      model->kind = 7;
      model->vertices = verts;
      for (j = 0; j != 2; j++) {
        s32 t = frame - 76 - j * 16;

        if (t >= 0) {
          Render_ResetTransformState();
          Func_08015160(0x80000, 0, 0);
          SceneTransform_ApplyPitch(0x4000);
          if (j == 0) {
            s32 sc = t * 3 * 1024 - 0x12000;

            if (t <= 39) {
              sc = t << 11;
              if (sc > 0xc000) {
                sc = 0xc000;
              }
            }
            Func_0801521c(sc);
            SceneTransform_ApplyYaw(-t * 512);
          } else {
            if (t > 23) {
              Func_0801521c(0x1c000 - (t << 11));
            } else {
              SceneTransform_ApplyPitch(0x3ff0 - t * 682);
              Func_080150e4(t * 169 + 0xf028);
            }
            SceneTransform_ApplyYaw(t << 9);
          }
          Render_TransformVertices(gData_08199210, verts, 4);
          Render_DrawModelList(model);
        }
      }
      Heap_Release(model);
      Heap_Release(verts);
    }

    if ((u32)(frame - 108) <= 179) {
      req[0] = Data_08196e54[0];
      req[1] = Data_08196e54[1];
      if (frame > 131) {
        s32 u = frame - 132;

        shipX = (Trig_Sin(u << 11) << 1) + 0xc80000;
        shipZ = (Trig_Sin(u << 10) << 1) + 0x540000;
      }
      if (draw) {
        place[3] = place[1] = 0;
        for (i = 0; i != 7; i++) {
          place[0] = (Data_08198654[i] << 16) + shipX - 0x200000;
          place[2] = (Data_0819865b[i] << 16) + shipZ - 0x200000;
          Render_ApplyProjectedPlacementFar(work->objects[i], place, req, 0);
        }
      }
    }
    if (frame > 265) {
      if (frame == 266) {
        REG_BG2X = -0x7000;
        REG_BG2PA = 0x100;
        REG_BG2PD = 0x100;
        p = work->particles;
        for (i = 0; i != 32; i++, p++) {
          p->x = Random16() % 48 + 40;
          p->y = Random16() % 96 + 16;
          p->variant = -i / 2;
        }
      }
      p = work->particles;
      for (i = 0; i != 32; i++, p++) {
        if ((u32)p->variant <= 23 && draw) {
          s32 k = (p->variant / 4) % 6;
          u8 w = gData_08197467[k];

          blits[0](canvas, work->sheet + gData_0819747a[k], p->x - (w >> 1),
                   p->y + gData_08197473[k] - 48, w, gData_0819746d[k]);
        }
        p->variant++;
      }
    }
    if (frame == 288) {
      Task_Add(Func_0815b68c, 1152);
      work->particles[63].x = 0xbc0000;
      work->particles[63].y = 0x600000;
      work->particles[63].velocityX = 0;
      work->particles[63].velocityY = 0;
      work->frame = 0;
      dmaArmed = 1;
    }
    if (draw) {
      BattleEffect_CountDownBattlerTints();
      work->transferPending = 1;
      wait(1);
    }
  }

  Func_081180e8(134);
  REG_BG1CNT = 0x1f83;
  wait(1);
  Iwram_FillWords(Ram_HeapSlots->battleFxWork[1], 0x4000, 0);
  Iwram_FillWords((void *)0x06004000, 0x3c00, 0);
  Iwram_FillWords((void *)0x06008000, 0x7800, 0);
  wait(1);
  REG_BLDALPHA = 0x100e;
  REG_BLDCNT = 0x3f46;
  REG_DISPCNT = 0x7741;
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(188);
  HeapSlot_Release(SLOT_BLITTER);
  transition[4] = 0;
  battle = Ram_HeapSlots->battleWork;
  Sound_PlayFar(390);
  gBgScroll[1].x = work->savedBg1X;
  gBgScroll[1].y = work->savedBg1Y;
  gCameraSceneParameters[3] = 120;
  gCameraSceneParameters[4] = 120;
  Iwram_ClearWords((void *)(BG_PLTT + 96), 256);
  wait(1);
  BattleBackground_LoadFar(2, *(u16 *)(battle + 0x670), -1);
  REG_BG2CNT = 0x787;
  Iwram_ClearWords((void *)0x06004000, 0x3c00);
  Task_Remove(Palette_StepFadeTransfer);
  gBgScroll[1].y = 32;
  IO_WRITE_QUEUE_PUSH(0x04000000, 0x7341, 0x20000);
  REG_BLDCNT = 0;
  wait(1);
  for (frame = 0; frame != 40; frame++) {
    u16 *src = (u16 *)(battle + 0x56c);
    volatile u16 *dst = (volatile u16 *)(BG_PLTT + 96);

    for (i = 0; i != 128; i++) {
      u16 v = *src++;
      s32 r = v & 31;
      s32 g = (u16)((v >> 5) & 31);
      s32 b = (u16)((v >> 10) & 31);
      s32 nr = r + frame - 30;
      s32 ng = g + frame - 40;
      s32 nb = b + frame - 30;

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
      *dst++ = nb << 10 | ng << 5 | nr;
    }
    wait(1);
  }
  IO_WRITE_QUEUE_PUSH(0x04000000, 0x7541, 0x20000);
  if (dmaArmed == 1) {
    Task_Remove(Func_0815b68c);
  }
  for (i = 0; i != 8; i++) {
    ResourceObject_ReleaseFar(work->objects[i]);
  }
}

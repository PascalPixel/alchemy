// Draft of BattleEffect_RunHaures, decompiled from the listing: differ 2685 of 2667
// (draft 2650), the same 178 calls and control flow; allocation differs.

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
void WaitFrames(s32 frames);
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

struct ResourceTableEntry {
  u16 unk0;
  u16 tile;
};

struct WordPair {
  s32 a;
  s32 b;
};

struct HalfPair {
  u16 x;
  u16 y;
};

extern struct ResourceTableEntry gResourceTableEntries[];
extern u8 gMapCellBuffer[];
extern u8 gData_02014000[];
extern u8 Data_02010ae8[];
extern u8 Data_02010be0[];
extern u8 Data_02010fa0[];
extern s8 Data_020104f0[];
extern u32 gCameraSceneParameters[];
extern const struct WordPair Data_08196f00;
extern const struct WordPair Data_08196f08;
extern const struct WordPair Data_08196f10;
extern const u16 gData_08197410[];
extern const u8 gData_08197467[];
extern const u8 gData_0819746d[];
extern const u8 gData_08197473[];
extern const u16 gData_0819747a[];
extern const u8 Data_081991e0[];
extern const u8 gData_08199210[];
extern const u8 Data_08199244[];
extern const u8 Data_081992f8[];
extern const u8 gData_08199340[];
extern const u8 gData_08199364[];
extern const u8 Data_08199388[];
extern const struct HalfPair Data_08199a12[];
extern const u8 Data_08199a3e[];
extern s32 Data_08199a44[];
extern const u8 Data_08199a4c[];
extern const u8 Data_08199bec[];
extern const u8 Data_08199bf9[];
extern const u8 Data_08199d2f[];

void *ResourceObject_CreateFar(s32 resource);
void Animation_ApplyChildArgumentFar(void *object, s32 argument);
u8 *Func_0815b290(s32 width, s32 height, u32 shape, s32 mode);
void ResourceObject_ReleaseFar(void *object);
void Func_0813ba50(void);
void Func_08163c2c(s32 a, s32 b);
void Func_0814cca8(void); // BattleEffect_SetupBlendedDisplay
void BattleBackground_LoadFar(s32 layer, s32 background, s32 palette);
void Func_08164abc(s32 r, s32 g, s32 b);
void Func_08164b2c(s32 r, s32 g, s32 b);
void Func_08196404(s32 slot, s32 width, s32 height, s32 mode, s32 flags);
void BattleEffect_ProcessPendingStampTransfer(void); // copies the 0x6780 canvas to the stamp's tiles
void Heap_Release(void *buffer);

#define SLOT_ALT 188 // unkb8[1]
#define OBJ_FLAGS(obj, n) (((u8 *)(obj))[n])
#define OBJ_TILES(obj)                                                         \
  (0x06010000 + gResourceTableEntries[((u8 *)(obj))[16]].tile)
#define STAMP_TILES(work) (*(u32 *)((u8 *)(work) + 0x7b74))
#define STAMP_CANVAS(work) ((u8 *)(work) + 0x6780)
#define HALF_HI(v) (((s16 *)&(v))[1])

// Haures: shadowed wings unfold over a dark pool, the demon climbs out of a
// vortex and dives through the foes, scattering into black feathers.
void BattleEffect_RunHaures(struct BattleEffectArgument *effect) {
  u8 *battle;
  u8 *canvas;
  struct BattleEffectWork *work;
  u8 *sprites;
  s32 *transition;
  DrawRectangle blit;
  DrawRectangle stamp;
  s32 frame;
  s32 i;
  s32 j;
  s32 k;
  s32 m;
  s32 vx;
  s32 vy;
  s32 px;
  s32 py;
  s32 morph;
  s32 morphStep;
  s32 dived;
  s32 row;
  s32 cut;
  s32 scroll;
  s32 place[4];
  s32 place2[4];
  struct WordPair req;
  struct WordPair req2;
  struct WordPair req3;
  struct ModelTexture texture;
  struct ModelTexture texture2;
  struct ModelTexture texture3;
  struct ModelTexture texture4;
  struct RenderModel *model;
  void *verts;
  struct EffectStep *p;
  u8 *obj;

  battle = Ram_HeapSlots->battleWork;
  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  sprites = Ram_HeapSlots->unk64[0];
  transition = (s32 *)Ram_HeapSlots->transitionWork;
  BattleEffect_StartCanvasLayer(0);
  Func_0813ba50();
  *(volatile u16 *)BG_PLTT = 0;
  *(volatile u16 *)(BG_PLTT + 2) = 0;
  work->transferMode = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 3200);
  Func_08163c2c(1, 0);
  REG_BLDCNT = 0x3f40;
  REG_BG2X = 0x400;
  gCameraSceneParameters[4] = 240;
  BattleEffect_SelectLivingTargets(work->effect);
  transition[4] = 1;
  gBgScroll[1].x = 0;
  work->transferMode = 1;
  work->transferValue = 0;
  Task_Add(BattleEffect_ProcessPendingStampTransfer, 3200);
  morph = 0;
  morphStep = 0;
  dived = 0;
  BattleEffect_SpawnObjects(2, 0x293, 2);
  for (i = 0; i != 4; i++) {
    obj = ResourceObject_CreateFar(0x294);
    work->objects[12 + i] = obj;
    if (obj != NULL) {
      obj[26] = 0;
      Animation_ApplyChildArgumentFar(obj, 0);
      obj = work->objects[12 + i];
      OBJ_FLAGS(obj, 9) = (OBJ_FLAGS(obj, 9) & ~12) | 4;
      OBJ_FLAGS(obj, 5) = (OBJ_FLAGS(obj, 5) & ~12) | 4;
    }
  }
  for (i = 0; i != 2; i++) {
    obj = ResourceObject_CreateFar(0x295);
    work->objects[16 + i] = obj;
    if (obj != NULL) {
      obj[26] = 0;
      Animation_ApplyChildArgumentFar(obj, 0);
      obj = work->objects[16 + i];
      OBJ_FLAGS(obj, 9) = (OBJ_FLAGS(obj, 9) & ~12) | 4;
    }
  }
  obj = Func_0815b290(64, 64, 0xc0002000, 0);
  work->objects[18] = obj;
  OBJ_FLAGS(obj, 5) = (OBJ_FLAGS(obj, 5) & ~12) | 4;
  STAMP_TILES(work) = OBJ_TILES(obj);
  {
    u8 *c = work->sheet;

    Resource_LoadAndDecompress(0xf7, c, 0, 0);
    Func_0816ae40(c, gData_02014000, 64, 64);
    Resource_LoadAndDecompress(0x178, c, 1, 0);
    for (i = 0; i != 0x1781; i++, c++) {
      if (*c != 0) {
        *c = (*c >> 2) + 224;
      }
    }
  }
  {
    u32 *w = (u32 *)(work->sheet + 0x4000);

    Resource_LoadAndDecompress(0xb4, w, 0, 0);
    for (i = 0; i != 1024; i++) {
      *w = (*w >> 1) & 0x7f7f7f7f;
      w++;
    }
  }
  {
    u8 *q = Data_02010ae8;

    for (i = 0; i != 31; i++, q += 8) {
      q[0] = i * 2;
      q[1] = i * 2 + 1;
      q[2] = i * 2 + 2;
      q[4] = i * 2 + 1;
      q[5] = i * 2 + 3;
      q[6] = i * 2 + 2;
    }
  }
  Data_02010be0[0] = 0;
  Data_02010be0[1] = 0;
  Data_02010be0[2] = 0;
  {
    volatile u16 *dst = (volatile u16 *)(BG_PLTT + 2);

    for (i = 1; i != 64; i++) {
      s32 r;
      s32 g;
      s32 b;

      if (i <= 31) {
        b = 0;
        g = 0;
        r = i;
        if (i < 0) {
          r = 0;
        }
      } else {
        g = i - 32;
        b = g * 4;
        r = b;
        if (r > 31) {
          r = 31;
        }
        if (b > 31) {
          b = 31;
        }
      }
      *dst++ = b << 10 | g << 5 | r;
    }
  }
  {
    volatile u16 *dst = (volatile u16 *)(BG_PLTT + 0x3c0);

    for (i = 0; i != 16; i++) {
      s32 r = i * 2 + 2;
      s32 b = i * 3 + 2;

      if (r > 31) {
        r = 31;
      }
      if (b > 31) {
        b = 31;
      }
      *dst++ = b << 10 | (i + 2) << 5 | r;
    }
  }
  BattleBackground_LoadFar(1, 0x5a, 0);
  frame = 0;
  if (!(gInput.repeat & 3)) {
    row = -0x1c29;
    cut = -0x10b;
    do {
      if (frame == 0) {
        work->transferValue = frame;
        px = 0x6c0000;
        py = 0x780000;
        vx = 0;
        vy = 0;
        Iwram_CopyWords((void *)(BG_PLTT + 0xc0), battle + 0x56c, 256);
        {
          u8 *q = Data_02010fa0;

          for (i = 0; i != 31; i++, q += 2) {
            q[0] = 1;
            q[1] = 1;
          }
        }
        p = work->particles;
        for (i = 0; i != 4; i++, p++) {
          p->x = Data_08199a3e[i] << 16;
          p->y = 0x100000;
          p->velocityX = 0;
          p->velocityY = 0;
          p->variant = 0;
        }
        p = &work->particles[4];
        for (i = 0; i != 8; i++, p++) {
          p->x = (Random16() & 15) + 24;
          p->variant = i * 3;
          p->y = Random16() & 7;
          p->velocityX = 0;
          p->velocityY = 0;
        }
        p = &work->particles[63];
        p->x = 0x3c0000;
        p->variant = 0;
        p->y = 0x700000;
        {
          s8 *v = Data_020104f0;
          s32 angle = 0;

          for (i = 0; i != 32; i++, angle += 992, v += 8) {
            v[0] = -(Trig_Cos(angle) << 6) >> 16;
            v[1] = -(Trig_Sin(angle) * 18) >> 16;
            v[4] = i * 4 - 64;
            v[2] = 0;
            v[4] = -(Trig_Cos(angle) << 6) >> 16;
            v[5] = (u32)(Trig_Sin(angle) * 9) >> 15;
            v[6] = 0;
          }
        }
      }
      if ((u32)frame <= 31) {
        s32 a = frame * 2 / 3 - 35;

        Func_08164b2c(a, a, frame - 35);
      }
      if ((u32)(frame - 242) <= 21 && (frame & 1)) {
        Func_08164abc(1, 0, 0);
      }
      if (frame == 277) {
        Func_08164b2c(8, -12, -12);
      }
      if (frame > 359) {
        Func_08164b2c(8 - (frame - 360) / 4, -12, (frame - 360) / 8 - 12);
      }
      if (frame == 48) {
        Sound_PlayFar(136);
      }
      if (frame == 74) {
        Sound_PlayFar(136);
      }
      if (frame == 100) {
        Sound_PlayFar(136);
      }
      if (frame == 126) {
        Sound_PlayFar(136);
      }
      if (frame == 140) {
        Sound_PlayFar(141);
      }
      if (frame <= 266) {
        if (frame <= 639) {
          place[1] = 0;
          place[3] = 0;
          p = work->particles;
          for (i = 0; i != 4; i++, p++) {
            if (frame > i * 26) {
              if (p->variant == 0) {
                place[0] = p->x;
                place[2] = p->y;
                Render_ApplyProjectedPlacementFar(work->objects[16 + (i & 1)],
                                                  place, Data_08199a44, 0);
                BattleEffectKernels_IntegrateVector2(p, 63, 0x2000);
                if (HALF_HI(p->y) > 119) {
                  p->variant = 1;
                }
              } else if (p->variant <= 23) {
                place[0] = p->x;
                place[2] = p->y;
                Animation_ApplyChildArgumentFar(work->objects[12 + (i & 1)],
                                                (p->variant / 6) & 3);
                Render_ApplyProjectedPlacementFar(work->objects[12 + (i & 1)],
                                                  place, Data_08199a44, 0);
                p->variant++;
              }
            }
          }
        }
        if (frame > 46) {
          s32 t;
          s32 sh;
          s32 s;

          verts = Heap_AllocEwram(32);
          model = Render_AllocateModelList(1);
          texture.logWidth = 6;
          texture.logHeight = 6;
          t = frame - 47;
          texture.pixels = work->sheet + 0x4000;
          sh = frame - 111;
          if (sh > 0) {
            sh = 0;
          }
          model->kind = 6;
          model->shade = sh;
          model->texture = &texture;
          model->mesh = gData_08199340;
          model->vertices = verts;
          Render_ResetTransformState();
          Func_08015160(0, 0x240000, 0);
          SceneTransform_ApplyPitch(4000);
          SceneTransform_ApplyYaw(-t << 8);
          s = t << 10;
          if (s > 60000) {
            s = 60000;
          }
          Func_0801521c(s);
          Render_TransformVertices(gData_08199210, verts, 4);
          Render_DrawModelList(model);
          texture.logWidth = 5;
          texture.logHeight = 5;
          texture.pixels = work->sheet + 0x1000;
          model->kind = 9;
          model->texture = &texture;
          model->mesh = Data_08199244;
          model->shade = sh;
          model->vertices = verts;
          Render_ResetTransformState();
          Func_08015160(0, 0x240000, 0);
          s = (t * 3 << 8) + 1024;
          if (s > 38768) {
            s = 38768;
          }
          Func_0801521c(s);
          if (frame > 73) {
            s32 a;
            s32 b;
            s32 n;

            if (frame == 74) {
              morph = 0;
              morphStep = 0;
            }
            a = morph % 13;
            b = (morph + 1) % 13;
            n = morphStep;
            morphStep = n + 1;
            if (morphStep == Data_08199bec[a]) {
              morphStep = 0;
              morph = morph + 1;
            }
            for (j = 0; j != 32; j++) {
              s32 lo = Data_08199a4c[a * 32 + j];

              Data_020104f0[j * 8 + 1] = -(
                  lo + n * (Data_08199a4c[b * 32 + j] - lo) / Data_08199bec[a]);
            }
            {
              s8 *v = Data_020104f0;
              s32 y = -64;

              for (j = 0; j != 32; j++, v += 8, y += 4) {
                *v = y;
              }
            }
          }
          model->unk04 = 0;
          model->kind = 1;
          model->mesh = Data_02010ae8;
          model->vertices = gMapCellBuffer;
          if ((u32)(frame - 236) <= 31) {
            u8 *q = Data_02010fa0;
            s32 v = frame - 235;

            for (j = 0; j != 31; j++, q += 2) {
              q[0] = v;
              q[1] = v;
            }
          }
          model->shade = (s32)Data_02010fa0;
          Render_TransformVertices(Data_020104f0, gMapCellBuffer, 64);
          Render_DrawModelList(model);
          Heap_Release(model);
          Heap_Release(verts);
        }
        req = Data_08196f00;
        place2[3] = 0;
        if (frame > 149) {
          s32 y;

          px += vx;
          py += vy;
          if (frame <= 159) {
            vy += -0x4000;
          } else if ((u32)(frame - 210) <= 13) {
            vx += -0x800;
            vy += -0x1000;
          } else if ((u32)(frame - 236) <= 3) {
            vx += 0x2000;
            vy += -0x4000;
          }
          vx = vx * 60 / 64;
          vy = vy * 60 / 64;
          req.a = 0x10000;
          req.b = 0x10000;
          y = py;
          for (i = 0; i != 2; i++, y += 0x200000) {
            place2[0] = px;
            place2[1] = 0x1100000 - y;
            place2[2] = 0x1000000;
            Render_ApplyProjectedPlacementFar(work->objects[i], place2,
                                              (s32 *)&req, 0);
          }
          if ((u32)(frame - 150) <= 116) {
            Func_08196404(SLOT_ALT, 6, 6, 3, 1);
            stamp = Ram_HeapSlots->unkb8[1];
            p = &work->particles[4];
            for (i = 0; i != 8; i++, p++) {
              s32 n = (p->variant / 4) % 6;
              s32 w = gData_08197467[n];

              stamp(STAMP_CANVAS(work), work->sheet + gData_0819747a[n],
                    p->x - (w >> 1), p->y + gData_08197473[n], w,
                    gData_0819746d[n]);
              if (++p->variant == 24) {
                p->variant = 0;
                p->x = (Random16() & 15) + 24;
                p->y = Random16() & 7;
              }
            }
            HeapSlot_Release(SLOT_ALT);
          }
          req.a = frame * 1128 - 103664;
          if (req.a > 0x40000) {
            req.a = 0x40000;
          }
          req.b = frame * 564 - 19064;
          if (req.b > 0x20000) {
            req.b = 0x20000;
          }
          place2[1] = 0x780000 - py;
          place2[0] = px;
          place2[2] = 0x800000;
          Render_ApplyProjectedPlacementFar(work->objects[18], place2,
                                            (s32 *)&req, 0);
        }
      }
      if (frame == 267) {
        Sound_PlayFar(212);
      }
      if (frame == 271) {
        Sound_PlayFar(144);
      }
      if ((u32)cut <= 9) {
        s32 s;

        if (frame == 267) {
          volatile u16 *dst = (volatile u16 *)(BG_PLTT + 2);

          for (i = 1; i != 64; i++) {
            s32 v = i;

            if (v > 31) {
              v = 31;
            }
            if (v < 0) {
              v = 0;
            }
            *dst++ = v;
          }
          Resource_LoadAndDecompress(0x17a, work->sheet, 1, 0);
          Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(0x150), 512);
          work->transferMode = 1;
          work->transferValue = 0x01010101;
        }
        verts = Heap_AllocEwram(32);
        model = Render_AllocateModelList(1);
        texture2.logWidth = 7;
        texture2.logHeight = 7;
        texture2.pixels = gData_02014000;
        model->shade = 0;
        model->kind = 7;
        model->texture = &texture2;
        model->mesh = gData_08199364;
        model->vertices = verts;
        Render_ResetTransformState();
        Func_08015160(0, -0x40000, 0);
        s = 0x1d8f0;
        if (frame > 268) {
          s = (frame << 12) - 0xef710;
        }
        Func_080151e4(s >> 1, s, s >> 1);
        Render_TransformVertices(Data_081991e0, verts, 4);
        Render_DrawModelList(model);
        if ((u32)cut <= 5) {
          Iwram_ClearWords(gMapCellBuffer, 0xd80);
          for (j = 0; j != 72; j++) {
            u8 *src = work->sheet + row * 128 + j * 48;
            u8 *dst = gMapCellBuffer + j * 64;

            for (m = 0; m != 48; m++) {
              *dst++ = *src++;
            }
          }
          if ((u32)cut <= 5) {
            texture2.logWidth = 6;
            texture2.logHeight = 7;
            texture2.pixels = gMapCellBuffer;
            model->mesh = Data_08199388;
            Render_ResetTransformState();
            Func_08015160(0, -0x40000, 0);
            Func_080151e4(0x1b1e0, 0x29c40, 0x20000);
            Render_TransformVertices(Data_081991e0, verts, 4);
            Render_DrawModelList(model);
          }
        }
        Heap_Release(model);
        Heap_Release(verts);
      }
      if (frame > 276) {
        u8 *cells;
        u8 *mesh;

        if (frame == 277) {
          const u8 *q = Data_08199bf9;

          work->transferMode = 1;
          work->transferValue = 0;
          dived = 1;
          ResourceObject_ReleaseFar(work->objects[0]);
          ResourceObject_ReleaseFar(work->objects[1]);
          for (i = 0; i != 4; i++) {
            ResourceObject_ReleaseFar(work->objects[12 + i]);
          }
          for (i = 0; i != 2; i++) {
            ResourceObject_ReleaseFar(work->objects[16 + i]);
          }
          ResourceObject_ReleaseFar(work->objects[18]);
          BattleEffect_SpawnObjects(11, 0x293, 2);
          p = work->particles;
          for (i = 0; i != 31; i++, q += 10, p++) {
            s32 x = q[0] >> 1;
            s32 y = q[1];

            p->x = x << 16;
            p->y = y << 16;
            p->velocityX = (x - 120) << 12;
            p->velocityY = (60 - y) << 10;
            p->z = 0;
            p->velocityX += ((Random16() & 31) - 16) << 13;
            p->velocityY += ((Random16() & 31) - 16) << 13;
            p->velocityZ = (Random16() & 127) * 5 + 640;
            if (Random16() & 1) {
              p->velocityZ = -p->velocityZ;
            }
          }
          vx = 0;
        }
        gBgScroll[1].x += 0xfff4;
        req3 = Data_08196f08;
        place2[3] = 0;
        px = (Trig_Sin(frame << 9) << 3) + 0x6c0000;
        py = 0x380000 - (Trig_Cos(frame << 9) << 3);
        if (frame > 396) {
          vx -= (frame - 397) << 14;
          vy -= (frame - 397) << 13;
          px += vx;
          py += vy;
        }
        for (i = 0; i != 11; i++) {
          place2[0] = (Data_08199a12[i].x << 16) + px;
          place2[1] = 0x1100000 - ((Data_08199a12[i].y << 16) + py);
          place2[2] = 0x1000000;
          Render_ApplyProjectedPlacementFar(work->objects[i], place2,
                                            (s32 *)&req3, 0);
        }
        verts = Heap_AllocEwram(32);
        model = Render_AllocateModelList(1);
        cells = work->sheet;
        mesh = work->sheet + 0x800;
        if (frame == 277) {
          const u8 *q = Data_08199bf9;
          u8 *f = mesh;

          for (i = 0; i != 31; i++, q += 10, f += 12) {
            const u8 *src = Data_08199bf9 + i * 10 + 2;
            s8 *dst = (s8 *)cells + i * 16;
            s32 y0 = q[1];
            s32 x0 = q[0];

            for (m = 0; m != 4; m++, src += 2, dst += 4) {
              dst[2] = 0;
              dst[0] = src[0] - x0;
              dst[1] = src[1] - y0;
            }
            f[1] = 1;
            f[2] = 2;
            f[5] = 2;
            f[6] = 3;
            f[0] = 0;
            f[4] = 0;
            f[8] = 0;
            f[9] = 0;
            f[10] = 0;
          }
        }
        texture3.logWidth = 8;
        texture3.logHeight = 7;
        texture3.pixels = gMapCellBuffer;
        model->texture = &texture3;
        model->kind = 1;
        model->vertices = verts;
        model->shade = (s32)Data_02010fa0;
        {
          u8 *q;

          for (q = Data_02010fa0; q != Data_02010fa0 + 31; q++) {
            *q = 1;
          }
        }
        p = work->particles;
        for (i = 0; i != 31; i++, p++, mesh += 12) {
          model->mesh = mesh;
          if ((u32)(HALF_HI(p->x) + 16) <= 152 && HALF_HI(p->y) >= -16 &&
              HALF_HI(p->y) <= 136) {
            Render_ResetTransformState();
            Func_08015160(p->x + -0x400000, p->y + -0x400000, 0);
            Func_080151e4(0x8000, 0x10000, 0x8000);
            switch (i & 3) {
            case 0:
              SceneTransform_ApplyPitch(p->z);
              Func_080150e4(p->z);
              break;
            case 1:
              SceneTransform_ApplyPitch(p->z);
              Func_080150e4(-p->z);
              break;
            case 2:
              SceneTransform_ApplyPitch(p->z);
              SceneTransform_ApplyYaw(p->z);
              break;
            case 3:
              Func_080150e4(p->z);
              SceneTransform_ApplyYaw(-p->z);
              break;
            }
            Render_TransformVertices(cells + i * 16, verts, 4);
            Render_DrawModelList(model);
          }
          BattleEffectKernels_IntegrateVector3(p, 62, 0);
          p->x += 0x40000;
        }
        Heap_Release(model);
        Heap_Release(verts);
      }
      BattleEffect_CountDownBattlerTints();
      work->transferPending = 1;
      if (frame <= 266) {
        work->unk78ec = 1;
      }
      WaitFrames(1);
      row += 27;
      cut++;
      frame++;
    } while (frame != 440 && !(gInput.repeat & 3));
  }
  if (dived == 0) {
    ResourceObject_ReleaseFar(work->objects[0]);
    ResourceObject_ReleaseFar(work->objects[1]);
    for (i = 0; i != 4; i++) {
      ResourceObject_ReleaseFar(work->objects[12 + i]);
    }
    for (i = 0; i != 2; i++) {
      ResourceObject_ReleaseFar(work->objects[16 + i]);
    }
    ResourceObject_ReleaseFar(work->objects[18]);
  } else {
    for (i = 0; i != 11; i++) {
      ResourceObject_ReleaseFar(work->objects[i]);
    }
  }
  transition[4] = 0;
  Task_Remove(BattleEffect_ProcessPendingStampTransfer);
  Func_0814cca8();
  gCameraSceneParameters[4] = 120;
  BattleEffect_SpawnObjects(11, 0x293, 2);
  Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(0x148), 512);
  Resource_LoadAndDecompress(0x17a, work->sheet, 1, 0);
  Iwram_ClearWords(gMapCellBuffer, 0x6c00);
  for (k = 0; k != 6; k++) {
    for (j = 0; j != 72; j++) {
      u8 *src = work->sheet + k * 27 * 128 + j * 48;
      u8 *dst = gMapCellBuffer + k * 9 * 512 + j * 64;

      for (m = 0; m != 48; m++) {
        *dst++ = *src++;
      }
    }
  }
  Resource_LoadAndDecompress(0xf4, work->sheet, 1, 0);
  Resource_LoadAndDecompress(0x134, sprites, 0, 0);
  work->transferMode = 2;
  work->transferValue = 50;
  REG_BLDALPHA = 0x1010;
  BattleEffect_LoadBlitter(SLOT_BLITTER, 19);
  blit = Ram_HeapSlots->unk64[1];
  scroll = 0;
  for (frame = 0; frame != 92; frame++, scroll += 8) {
    struct EffectStep *sparks = (struct EffectStep *)gData_02014000;

    if (frame == 0) {
      vx = -0xc0000;
      vy = 0x60000;
      px = 0x10e0000;
      py = -0x160000;
      for (i = 0; i != 512; i++) {
        sparks[i].variant = -1;
      }
    }
    if (frame == 20) {
      px = -0xa0000;
      py = -0x160000;
      vx = 0xc0000;
      vy = 0x60000;
    }
    if (frame == 40) {
      px = 0x1040000;
      py = 0x400000;
      vx = -0x80000;
      vy = 0;
    }
    p = sparks;
    for (i = 0; i != 512; i++, p++) {
      if (p->variant >= 0) {
        s32 n = (p->variant >> 3) + 1;

        blit(canvas, sprites + gData_08197410[n - 1], HALF_HI(p->x) - n / 2,
             HALF_HI(p->y) - n, n, n * 2);
        BattleEffectKernels_IntegrateVector2(p, 63, -0x8000);
        p->variant--;
      }
    }
    if (frame <= 39) {
      s32 half = px / 2;

      p = &sparks[frame * 12];
      for (i = 0; i != 12; i++, p++) {
        s32 angle;
        s32 speed;

        p->x = (((Random16() & 63) - 16) << 16) + half;
        p->y = ((Random16() & 31) << 16) + py;
        angle = Random16();
        speed = (Random16() & 127) + 128;
        p->velocityX = speed * Trig_Sin(angle) >> 7;
        p->velocityY = speed * Trig_Cos(angle) >> 6;
        p->variant = 16;
      }
    }
    if (frame <= 73) {
      s32 t = frame - 20;

      req2 = Data_08196f10;
      place2[3] = 0;
      if ((u32)t <= 19) {
        req2.a = -0x10001;
        for (i = 0; i != 11; i++) {
          place2[0] = px - (Data_08199a12[i].x << 16);
          place2[1] = 0x1100000 - ((Data_08199a12[i].y << 16) + py);
          place2[2] = 0x1000000;
          Render_ApplyProjectedPlacementFar(work->objects[i], place2,
                                            (s32 *)&req2, 0);
        }
      } else {
        for (i = 0; i != 11; i++) {
          place2[0] = (Data_08199a12[i].x << 16) + px;
          place2[1] = 0x1100000 - ((Data_08199a12[i].y << 16) + py);
          place2[2] = 0x1000000;
          Render_ApplyProjectedPlacementFar(work->objects[i], place2,
                                            (s32 *)&req2, 0);
        }
      }
      px += vx;
      py += vy;
      if ((u32)frame <= 19) {
        vx += -0x8000;
        vy += 0x8000;
      } else if ((u32)t <= 19) {
        vx += 0x8000;
        vy += 0x4000;
      } else if ((u32)(frame - 40) <= 19) {
        vx += -0x8000;
      }
    }
    if (frame == 10) {
      Sound_PlayFar(212);
    }
    if (frame == 14) {
      Sound_PlayFar(144);
      for (i = 0; i != effect->count; i++) {
        BattleEffect_StartActorMotion(effect->actors[i], 1, 0x40000, 0x80000,
                                  0x40000, 150);
        Func_0814cd48(effect->actors[i], 7, 5, i, 16);
        work->shakeFrames = 8;
      }
    }
    if (frame == 30) {
      Sound_PlayFar(212);
    }
    if (frame == 34) {
      Sound_PlayFar(144);
      for (i = 0; i != effect->count; i++) {
        BattleEffect_StartActorMotion(effect->actors[i], 1, 0x40000, 0x80000,
                                  0x40000, 50);
        Func_0814cd48(effect->actors[i], 7, 5, i, 16);
        work->shakeFrames = 8;
      }
    }
    if (frame == 58) {
      Sound_PlayFar(145);
      for (i = 0; i != effect->count; i++) {
        BattleEffect_StartActorMotion(effect->actors[i], 1, 0x40000, 0x80000,
                                  0x40000, 200);
        Func_0814cd48(effect->actors[i], 7, 5, i, 16);
        work->shakeFrames = 8;
      }
    }
    verts = Heap_AllocEwram(32);
    model = Render_AllocateModelList(1);
    model->shade = -8;
    model->kind = 7;
    texture4.logWidth = 6;
    texture4.logHeight = 5;
    model->texture = &texture4;
    model->mesh = Data_081992f8;
    model->vertices = verts;
    texture4.pixels = work->sheet;
    model->scrollU = scroll & 127;
    for (i = 0; i != 2; i++) {
      s32 start = Data_08199d2f[i];

      if (frame >= start && frame < start + 16) {
        s32 amp = Trig_Sin((frame - start) << 11) / 8;
        s32 x = -0x100000;

        for (m = 0; m != 3; m++, x += 0x140000) {
          Render_ResetTransformState();
          if (i == 0) {
            Func_08015160(x, -0x40000, 0);
            Func_080150e4(-0x2000);
          } else {
            Func_08015160(x + -0x100000, -0x40000, 0);
            Func_080150e4(0x27d0);
          }
          Func_080151e4(0x30000, amp, 0x20000);
          Render_TransformVertices(Data_081991e0, verts, 4);
          Render_DrawModelList(model);
        }
      }
    }
    texture4.logWidth = 6;
    texture4.logHeight = 5;
    model->mesh = Data_081992f8;
    model->kind = 7;
    if ((u32)(frame - 56) <= 31) {
      s32 sh = 0;
      s32 v;
      s16 *corner = verts;

      if (frame > 79) {
        sh = (80 - frame) * 8;
      }
      model->shade = sh;
      model->scrollU = scroll & 127;
      texture4.pixels = work->sheet;
      if (frame <= 71) {
        v = Trig_Sin((frame << 10) + -0xe000) * 80 >> 16;
      } else if (frame > 79) {
        v = Trig_Sin((frame << 10) + -0x10000) * 80 >> 16;
      } else {
        v = 80;
      }
      corner[1] = 60 - v;
      corner[5] = 60 - v;
      corner[0] = 4;
      corner[4] = 123;
      corner[8] = 4;
      corner[9] = v + 60;
      corner[12] = 123;
      corner[13] = v + 60;
      Render_DrawModelList(model);
    }
    Heap_Release(model);
    Heap_Release(verts);
    Func_08158ce0(8, 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    WaitFrames(1);
  }
  for (i = 0; i != 11; i++) {
    ResourceObject_ReleaseFar(work->objects[i]);
  }
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(SLOT_BLITTER);
  BattleEffect_EndCanvasLayer();
}

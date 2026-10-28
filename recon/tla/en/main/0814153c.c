// Draft of BattleEffect_RunEclipse, decompiled from the listing: differ 2468 of
// 2159 (draft 2171), the same 176 calls and control flow; allocation differs.

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

struct ResourceTableEntry {
  u16 unk0;
  u16 tile;
};

struct EclipseObject {
  u8 unk0[9];
  u8 flags;
  u8 unk0a[6];
  u8 resource;
};

struct WordPair {
  s32 a;
  s32 b;
};

extern struct ResourceTableEntry gResourceTableEntries[];
extern u8 gMapCellBuffer[];
extern u8 gData_02014000[];
extern u8 Data_02010400[];
extern u8 Data_02010800[];
extern u8 Data_02011000[];
extern u8 Data_02012000[];
extern u8 Data_02012800[];
extern u32 gCameraSceneParameters[];
extern const struct WordPair Data_08196de0;
extern const u8 Data_08197744[];
extern const u8 Data_08197748[];
extern const u8 Data_0819774f[];
extern const u32 Data_08197760[];
extern const u8 Data_081977a4[];
extern const u8 Data_081977c4[];
extern const u8 Data_081977e4[];
extern const s8 Data_081977e8[];
extern const u16 gData_08197410[];
extern const u16 gData_08197486[];
extern const u8 gData_08197492[];
extern const u8 gData_08197498[];
extern const u8 gData_081991b0[];
extern const u8 Data_081991c0[];
extern const u8 gData_08199210[];
extern const u8 Data_08199220[];
extern const u8 gData_08199340[];

struct EclipseObject *Func_0815b290(s32 width, s32 height, u32 shape, s32 mode);
void ResourceObject_ReleaseFar(void *object);
void Func_08179e6c(void);
void Func_0813baec(void);
void BattleActor_SpawnObjectsForListFar(s16 *list, s32 mode);
void Resource_ResetEntry(s32 entry);
void VramBlock_Reset93Far(void);
void BattleBackground_LoadFar(s32 layer, s32 background, s32 palette);
void Resource_DecodeType01(const void *source, void *destination);
void Func_08196404(s32 slot, s32 width, s32 height, s32 mode, s32 flags);
void Func_08020388(void);
s32 VramCache_AllocEntry(s32 size);
void Palette_LoadBasicObjColors(void);
void Func_08164abc(s32 r, s32 g, s32 b);
void Func_08164b2c(s32 r, s32 g, s32 b);
void Func_081885f0(void *a, void *b, s32 count);
void Graphics_LoadGreyRamp(void);
void Func_0818de3c(void *triangles, s32 columns, s32 rows, s32 cellWidth,
                   s32 cellHeight);
void Heap_Release(void *buffer);

#define SLOT_ALT 188 // unkb8[1]
#define OBJ_TILES(obj)                                                         \
  (0x06010000 + gResourceTableEntries[(obj)->resource].tile)

// Eclipse: the battlefield scrolls up into a night sky, thirty-two shards
// of a black moon rise over the foes, its ring of light flares and
// collapses, then the moon's beam strikes and a dark sphere bursts.
void BattleEffect_RunEclipse(struct BattleEffectArgument *effect) {
  u8 *canvas;
  struct BattleEffectWork *work;
  u16 savedScroll;
  s32 *transition;
  u8 *sprites;
  u8 *battle;
  DrawRectangle blit;
  s32 frame;
  s32 i;
  s32 j;
  s32 scroll;
  s32 baseX;
  s32 baseY;
  s32 velX;
  s32 velY;
  s32 cx;
  s32 cy;
  u32 u;
  s32 count;
  s32 shade;
  s32 *radius;
  s32 place[4];
  struct WordPair req;
  struct ModelTexture texture;
  struct ModelTexture texture2;
  struct ModelTexture texture3;
  struct RenderModel *model;
  void *verts;
  struct EffectStep *p;
  struct EclipseObject *obj;
  s16 list[1];

  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  savedScroll = gBgScroll[1].x;
  transition = (s32 *)Ram_HeapSlots->transitionWork;
  sprites = Ram_HeapSlots->unk64[0];
  battle = (u8 *)Ram_HeapSlots->battleWork;
  BattleEffect_StartCanvasLayer(0);
  Func_08179e6c();
  work->transferMode = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 3200);
  scroll = -80;
  for (frame = 0; frame != 52; frame++) {
    if (frame == 27) {
      u8 *e;

      list[0] = 0xff;
      BattleActor_SpawnObjectsForListFar(list, 0);
      Resource_ResetEntry(*(s32 *)(battle + 84));
      VramBlock_Reset93Far();
      gCameraSceneParameters[4] = 240;
      BattleBackground_LoadFar(1, 0x78, 0);
      work->scrollTimer = 0;
      work->scrollInterval = 1;
      work->scrollStepX = -1;
      work->scrollStepY = 0;
      Task_Add(Func_0813baec, 0x480);
      transition[4] = 1;
      e = Resource_GetTableEntry(0xa4);
      Iwram_CopyWords((void *)OBJ_PLTT, e, 480);
      Resource_DecodeType01(e + 480, gMapCellBuffer);
      for (i = 0; i != 31; i++) {
        obj = Func_0815b290(32, 32, 0x80002000, 0);
        obj->flags = (obj->flags & ~12) | 8;
        work->objects[i] = obj;
        Iwram_CopyWords((void *)OBJ_TILES(obj), gMapCellBuffer + i * 0x400,
                        0x400);
      }
      e = Resource_GetTableEntry(0xa3);
      Iwram_CopyWords((void *)OBJ_PLTT, e, 480);
      Resource_DecodeType01(e + 480, gMapCellBuffer);
      obj = Func_0815b290(32, 32, 0x80002000, 0);
      work->objects[31] = obj;
      obj->flags = (obj->flags & ~12) | 8;
      Iwram_CopyWords((void *)OBJ_TILES(obj), gMapCellBuffer, 0x400);
    }
    scroll += 4;
    REG_BG2X = scroll << 8;
    work->transferPending = 1;
    wait(1);
  }
  Iwram_FillWords(canvas, 0x3c00, 0);
  Iwram_FillWords((void *)0x06004000, 0x3c00, 0);
  Resource_LoadAndDecompress(0x134, sprites, 0, 0);
  REG_DISPCNT = 0x7741;
  REG_BG2PA = 0x80;
  REG_BLDALPHA = 0x1010;
  REG_BLDCNT = 0x3f44;
  REG_BG2X = 0;
  baseX = 0;
  baseY = 0;
  velX = 0;
  velY = 0;
  work->transferMode = 2;
  work->transferValue = 50;
  BattleEffect_LoadBlitter(SLOT_BLITTER, 19);
  blit = Ram_HeapSlots->unk64[1];
  Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(0x130), 128);
  radius = &work->particles[16].variant;
  for (frame = 0; frame != 220 && !(gInput.repeat & 3); frame++) {
    if (frame == 0) {
      baseX = -0x300000;
      baseY = 0xd40000;
      velX = 0x40000;
      velY = -0xa0000;
      work->fadeFrames = 1;
      work->fadeStep = frame;
      *radius = frame;
      Resource_LoadAndDecompress(0xb4, gData_02014000, 0, 0);
    }
    if (frame == 136) {
      for (i = 0; i != 7; i++) {
        Iwram_CopyWords(
            (void *)OBJ_TILES(
                (struct EclipseObject *)work->objects[Data_08197748[i]]),
            Data_02010400 + i * 0x400, 0x400);
      }
    }
    if (frame == 48) {
      Sound_PlayFar(139);
    }
    if (frame == 136) {
      Sound_PlayFar(140);
    }
    if (frame == 172) {
      Sound_PlayFar(144);
    }
    for (i = 0; i != 17; i++) {
      if (frame == Data_0819774f[i]) {
        Iwram_FillWords(canvas, 0x3c00, Data_08197760[i]);
      }
    }
    req = Data_08196de0;
    place[3] = 0;
    place[1] = 0xff0000;
    baseX += velX;
    baseY += velY;
    if (frame > 171) {
      if (frame > 183) {
        velX += -0x200;
        velY += 0x400;
      }
      if (frame <= 205) {
        velX = velX * 54 / 64;
        velY = velY * 54 / 64;
      }
    } else {
      if (velY < 0) {
        velY += 0x4000;
      }
      if (velX > 0) {
        velX += -0x2000;
      }
      if (frame > 31) {
        baseY += Trig_Sin((frame - 32) << 9) << 2 >> 4;
        if (frame <= 159) {
          baseX += 0x4000;
        }
      }
    }
    for (i = 0; i != 32; i++) {
      if (i == 25 && frame > 135) {
        place[0] = baseX + 0x200000;
        place[2] = baseY + 0x400000;
      } else {
        place[0] = baseX + (Data_081977a4[i] << 16);
        place[2] = baseY + (Data_081977c4[i] << 16);
      }
      if (place[2] <= 0xc7ffff) {
        Render_ApplyProjectedPlacementFar(work->objects[i], place, (s32 *)&req,
                                          0);
      }
    }
    if (frame == 136) {
      work->fadeFrames = 24;
      work->fadeStep = 0;
    }
    cx = baseX / 2 - (*radius << 13) + 0x1c0000;
    cy = baseY + (*radius << 14) + 0x380000;
    u = frame - 136;
    if (u <= 84) {
      if (frame == 136) {
        *radius = 4;
        work->particles[16].x = 0;
        work->particles[16].z = 0;
        Func_0815b434(work->sheet, *radius * 2, 0x10000);
      }
      if (frame > 171) {
        if (frame == 172) {
          *radius = 64;
          Func_0815b434(work->sheet, 128, 0x10000);
          velX = 0x30000;
          velY = -0x30000;
        }
        if (frame > 203 && *radius > 1) {
          *radius -= 4;
          if (*radius <= 1) {
            *radius = 2;
          }
          Func_0815b434(work->sheet, *radius * 2, 0x10000);
        }
      } else if (frame > 135) {
        velX += -0x800;
        velY += 0x800;
        if (++*radius <= 32) {
          Func_0815b434(work->sheet, *radius * 2, 0x10000);
        } else {
          *radius = 32;
        }
      }
      Func_0818caa8(work->sheet, cx >> 16, cy >> 16, *radius * 2);
    }
    if (frame > 47) {
      if (frame == 48) {
        p = work->particles;
        for (i = 0; i != 16; i++, p++) {
          p->x = Random16() & 0xffff;
          p->y = (Random16() & 255) + 128;
          if (Random16() & 1) {
            p->y = -p->y;
          }
          p->variant = i * -8;
        }
        Resource_LoadAndDecompress(0x192, work->sheet + 0x2000, 1, 0);
        Resource_LoadAndDecompress(0xc9, work->sheet + 0x4000, 0, 0);
      }
      verts = Heap_AllocEwram(32);
      model = Render_AllocateModelList(1);
      model->shade = 0;
      texture.logWidth = 5;
      texture.logHeight = 2;
      texture.pixels = work->sheet + 0x4000;
      model->kind = 6;
      model->mesh = Data_08199220;
      model->texture = &texture;
      model->vertices = verts;
      p = work->particles;
      for (i = 0; i != 8; i++, p++) {
        p->variant++;
        p->x += p->y;
        if ((u32)p->variant <= 47) {
          Render_ResetTransformState();
          Func_08015160(cx + ((i & 3) << 17) + 0x20000 + -0x400000,
                        cy - (((i & 3) + 2) << 16) + -0x400000, 0);
          Func_080151e4(0x8000, 0x10000, 0x10000);
          Func_080150e4(p->x);
          Func_080151e4(Trig_Sin(p->variant * 682) + 0x4000, 0x20000, 0x10000);
          Render_TransformVertices(gData_081991b0, verts, 4);
          Render_DrawModelList(model);
        }
      }
      if (u <= 35) {
        s32 s = 0x40000 - u * 6000;
        s32 sh = u * 4 - 64;

        if (sh > -16) {
          sh = -16;
        }
        model->shade = sh;
        texture.logWidth = 6;
        texture.logHeight = 6;
        model->kind = 7;
        texture.pixels = gData_02014000;
        model->texture = &texture;
        model->mesh = gData_08199340;
        model->vertices = verts;
        Render_ResetTransformState();
        Func_08015160(cx + -0x3e0000, cy + -0x420000, 0);
        Func_080151e4(s >> 1, s, s);
        SceneTransform_ApplyPitch(0x2000);
        Func_080150e4(0x2800);
        SceneTransform_ApplyYaw(-frame << 10);
        Render_TransformVertices(gData_08199210, verts, 4);
        Render_DrawModelList(model);
      }
      if ((u32)(frame - 172) <= 48) {
        Render_ResetTransformState();
        Func_08015160(cx + -0x3e0000, cy + -0x420000, 0);
        Func_080151e4(0x8000, 0x10000, 0x10000);
        Func_080150e4(0x2800);
        if (frame > 203) {
          Func_080151e4(0x80000 - ((frame - 204) << 15), 0x20000, 0x10000);
        } else {
          Func_080151e4(0x80000, 0x20000, 0x10000);
        }
        Render_TransformVertices(Data_081991c0, verts, 4);
        Render_DrawModelList(model);
      }
      Heap_Release(model);
      Heap_Release(verts);
    }
    if ((u32)(frame - 148) <= 71) {
      s32 x0 = cx >> 16;
      s32 y0 = cy >> 16;

      for (i = 0; i != 4; i++) {
        s32 k = (frame + i) & 3;
        s32 x;
        s32 y;

        if (frame <= 171) {
          s32 size = *radius * 2;

          x = Random16() % size + x0 - (size >> 1) - (gData_08197492[k] >> 1);
          y = Random16() % size + y0 - (size >> 1) - (gData_08197498[k] >> 1);
        } else {
          u32 a = Random16() & 63;
          s32 r = *radius;

          x = x0 - (a >> 1) - (gData_08197492[k] >> 1);
          x = x + Random16() % r - r / 2 - 8;
          y = a + y0 - (gData_08197498[k] >> 1);
        }
        Func_08196404(SLOT_ALT, 7, 7, Data_08197744[Random16() & 3] | 3, 1);
        ((DrawRectangle)Ram_HeapSlots->unkb8[1])(
            canvas, work->sheet + 0x2000 + gData_08197486[k], x, y,
            gData_08197492[k], gData_08197498[k]);
        HeapSlot_Release(SLOT_ALT);
      }
    }
    HeapSlot_Release(SLOT_ALT);
    work->transferPending = 1;
    wait(1);
  }
  Task_Remove(Func_0813baec);
  gBgScroll[1].x = savedScroll;
  transition[4] = 0;
  for (i = 0; i != 32; i++) {
    ResourceObject_ReleaseFar(work->objects[i]);
  }
  Func_08020388();
  *(s32 *)(battle + 84) = VramCache_AllocEntry(128);
  BattleEffect_SelectLivingTargets(work->effect);
  for (i = 0; i != effect->count; i++) {
    Func_0814cd48(effect->actors[i], 1, -1, -1, 0);
    Func_0814cd48(effect->actors[i], 0, -1, -1, 0);
  }
  Palette_LoadBasicObjColors();
  REG_DISPCNT = 1;
  gBgScroll[1].y = 32;
  BattleBackground_LoadFar(1, *(u16 *)(battle + 0x670), 24);
  Iwram_FillWords(canvas, 0x4000, 0);
  Iwram_FillWords((void *)0x06004000, 0x4000, 0);
  wait(1);
  REG_BLDALPHA = 0x1010;
  REG_BLDCNT = 0x3f46;
  REG_DISPCNT = 0x7741;
  gCameraSceneParameters[4] = 120;
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
  for (frame = 0; frame != 48; frame++) {
    if (frame > 39) {
      work->transferMode = 4;
      work->transferValue = 0x04040404;
    }
    if (frame == 16) {
      Sound_PlayFar(140);
    }
    if (frame == 18) {
      work->shakeFrames = 90;
      Sound_PlayFar(145);
      Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
      for (i = 0; i != effect->count; i++) {
        Func_0814cd48(effect->actors[i], 7, 5, i, 4);
      }
    }
    if (frame <= 109) {
      verts = Heap_AllocEwram(32);
      model = Render_AllocateModelList(1);
      texture2.logWidth = 5;
      texture2.logHeight = 2;
      texture2.pixels = work->sheet + 0x3840;
      model->mesh = Data_08199220;
      model->shade = 0;
      model->texture = &texture2;
      model->vertices = verts;
      model->kind = 6;
      if (frame > 15) {
        struct EffectStep *q = &work->particles[63];

        if (frame == 16) {
          q->x = 0x800000;
          q->y = -0x280000;
        }
        if (frame <= 19) {
          q->x += -0x200000;
          q->y += 0x400000;
        }
        if (frame <= 27) {
          Func_0815b434(work->sheet + 0x38c0, 96, 0x10000);
          Func_0818caa8(work->sheet + 0x38c0, HI(q->x), HI(q->y), 96);
        }
        Render_ResetTransformState();
        Func_08015160(q->x + -0x3f0000, q->y + -0x400000, 0);
        Func_080151e4(0x8000, 0x10000, 0x8000);
        Func_080150e4(0x9800);
        Func_080151e4(0xdc500, 0x29c40, 0x8000);
        Render_TransformVertices(Data_081991c0, verts, 4);
        Render_DrawModelList(model);
      }
      Heap_Release(model);
      Heap_Release(verts);
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
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }
  count = -1;
  transition[4] = 1;
  BattleBackground_LoadFar(1, 0x75, 0);
  Func_08164b2c(31, 31, 31);
  gBgScroll[1].x = 0;
  gCameraSceneParameters[4] = 240;
  wait(1);
  Resource_LoadAndDecompress(0xcd, work->sheet, 1, 0);
  Resource_LoadAndDecompress(0xc1, work->sheet + 0x400, 0, 0);
  Resource_LoadAndDecompress(0xca, work->sheet + 0x3840, 0, 0);
  p = (struct EffectStep *)gData_02014000;
  for (i = 0; i != 512; i++) {
    p[i].variant = count;
  }
  work->transferMode = 2;
  work->transferValue = 50;
  gBgScroll[1].y = 32;
  for (frame = 0; frame != 26; frame++) {
    if ((gInput.repeat & 3) && count == -1) {
      count = 8;
    }
    if (frame <= 6 && count == -1) {
      s32 v = 31 - frame * 8;
      s32 a = v;
      s32 b = v;
      s32 c = v;

      if (c < -10) {
        a = -10;
      }
      if (c < -15) {
        b = -15;
      }
      if (c <= 3) {
        c = 4;
      }
      Func_08164b2c(a, b, c);
    }
    if (frame > 17 || count > 0) {
      Func_08164abc(4, 4, 4);
      if (--count == 0) {
        break;
      }
    }
    for (i = 0; i != 16; i++) {
      s32 a;
      s32 r;

      p = (struct EffectStep *)gData_02014000 + ((frame * 16 + i) & 511);
      a = Random16();
      r = (Random16() & 15) + frame * 2;
      p->x = r * Trig_Sin(a) + 0x3c0000;
      p->y = r * Trig_Cos(a) + 0x3c0000;
      p->velocityX = 0;
      p->velocityY = 0;
      p->variant = (Random16() & 15) + 16;
    }
    p = (struct EffectStep *)gData_02014000;
    for (i = 0; i != 512; i++, p++) {
      if (p->variant >= 0) {
        s32 n = (p->variant >> 3) + 1;

        blit(canvas, sprites + gData_08197410[n - 1], HI(p->x) - n / 2,
             HI(p->y) - n, n, n * 2);
        BattleEffectKernels_IntegrateVector2(p, 60, -0x4000);
        p->variant--;
      }
    }
    model = Render_AllocateModelList(1);
    shade = 0;
    if (frame <= 31) {
      s32 s = 0x10000;

      if (frame > 15) {
        s = 0x10000 - (frame << 12) + 0x10000;
      }
      texture3.logWidth = 5;
      texture3.logHeight = 2;
      texture3.pixels = work->sheet + 0x3840;
      model->mesh = Data_08199220;
      model->shade = shade;
      model->vertices = gMapCellBuffer;
      model->kind = 6;
      model->texture = &texture3;
      Render_ResetTransformState();
      Func_08015160(-0x20000, 0x100000, 0);
      Func_080151e4(0x8000, 0x10000, 0x8000);
      Func_080150e4(0x9800);
      Func_080151e4(s, 0x20000, 0x8000);
      Render_TransformVertices(Data_081991c0, gMapCellBuffer, 4);
      Render_DrawModelList(model);
    }
    if (frame == 0) {
      s32 angle = 0;
      s32 offset = 0;

      Sound_PlayFar(208);
      Func_0818de3c(Data_02011000, 16, 8, 8, 8);
      for (i = 0; i != 8; i++, angle += 0x888, offset += 64) {
        s32 y = -Trig_Cos(angle) * 96 >> 16;
        s32 rr = Trig_Sin(angle) * 48 >> 16;
        s8 *v = (s8 *)Data_02010800 + offset;

        for (j = 0; j != 16; j++) {
          v[0] = rr * Trig_Sin(j << 12) >> 16;
          v[1] = y;
          v[2] = rr * Trig_Cos(j << 12) >> 16;
          v += 4;
        }
      }
      Func_081885f0(Data_02012000, Data_02012800, 32);
    }
    texture3.logWidth = 5;
    texture3.logHeight = 5;
    texture3.pixels = work->sheet;
    model->kind = 6;
    model->mesh = Data_02011000;
    model->vertices = gMapCellBuffer;
    model->texture = &texture3;
    model->unk04 = 3;
    model->shade = shade;
    {
      s32 s = (frame << 9) + 0x8000;

      if (s > 0x10000) {
        s = 0x10000;
      }
      Render_ResetTransformState();
      Func_08015160(-0x20000, 0xc0000, 0);
      SceneTransform_ApplyPitch(0x2000);
      SceneTransform_ApplyYaw(-frame << 10);
      Func_0801521c(s);
      Render_TransformVertices(Data_02010800, gMapCellBuffer, 128);
      Render_DrawModelList(model);
    }
    texture3.logHeight = 5;
    texture3.logWidth = 7;
    texture3.pixels = work->sheet + 0x400;
    model->texture = &texture3;
    model->kind = 7;
    model->mesh = Data_02012800;
    model->vertices = gMapCellBuffer;
    model->unk04 = shade;
    model->scrollU = shade;
    model->scrollV = shade;
    {
      s32 t = frame + 24;
      s32 fade = frame * 4 + 96;
      s32 angle = frame << 10;

      for (i = 0; i != 4; i++, fade -= 24, t -= 6, angle += 0x2000) {
        if ((u32)t <= 63) {
          s32 s = (Data_081977e4[i] * t << 10) + 0x4000;

          shade = 0;
          if (t <= 15) {
            shade = fade - 64;
          }
          if (t > 40) {
            shade = (40 - t) * 4;
          }
          model->shade = shade;
          Render_ResetTransformState();
          Func_08015160(-0x20000, 0x200000, 0);
          Func_080151e4(s / 4, s / 4, s / 4);
          SceneTransform_ApplyPitch(10000);
          SceneTransform_ApplyYaw(angle);
          Func_080151e4(0x10000, 0x60000, 0x10000);
          Func_08015160(0, Data_081977e8[i] << 16, 0);
          Render_TransformVertices(Data_02012000, gMapCellBuffer, 64);
          Render_DrawModelList(model);
        }
      }
    }
    Heap_Release(model);
    work->transferPending = 1;
    wait(1);
  }
  Iwram_FillWords((void *)0x06004000, 0x3c00, 0x3f3f3f3f);
  gBgScroll[1].x = savedScroll;
  transition[4] = 0;
  gCameraSceneParameters[4] = 120;
  BattleBackground_LoadFar(1, *(u16 *)(battle + 0x670), 24);
  wait(1);
  work->transferMode = 3;
  work->transferValue = 0x02020202;
  Graphics_LoadGreyRamp();
  Iwram_FillWords(canvas, 0x3c00, 0x3f3f3f3f);
  Func_081180e8(134);
  for (frame = 0; frame != 32; frame++) {
    work->transferPending = 1;
    wait(1);
  }
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(SLOT_BLITTER);
  BattleEffect_EndCanvasLayer();
}

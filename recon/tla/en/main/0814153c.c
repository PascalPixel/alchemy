// Draft, differ 821 of 2159, skeleton gap 90 (tla-en, 2026-11-01)
// BattleEffect_RunEclipse: control flow, frame (184) and spill slots follow the
// listing; nearly all that is left is one cause, the Iwram fills and copies.
// A direct Iwram_FillWords or Iwram_CopyWords sends a costly constant size through
// a pseudo (calls.c, precompute_register_parameters), so CSE shares it across
// calls and loop.c hoists it. The listing rebuilds the size at each call, which
// only a constant reaching the hard register does: integrate.c substitutes it
// when the call is forwarded through a static inline. The same forwarding is
// already adopted as FAKEMATCH(wrapper) in f14cca8, f143354 and f0416cc, and The
// Broken Seal's iwram_call.h has that inline layer. Measured with both forwarded
// in a scratch copy: differ 341, skeleton gap 28. It stays out of this draft
// because this round adds no device; whether the layer is source in this game
// too is a counting decision (S2) for Pascal.
// Left in that forwarded state:
// - The 136 loop: size is hoisted into r7 and the dst step rebuilt, a reduced giv.
//   Base and size as body locals with dst + i * size reduce the giv, but size
//   then gets no register (gResourceTableEntries takes r7, not r4 with a save).
// - The ring loop's 0x20000 is shared (fp) and -0x400000 is not hoisted; the
//   listing has the reverse. Moving origin into the body is worse.
// - Ring: angle and y swap r9 and r8. j's load rotates the sparks' scratch.
// - The late 0x4000 fills load the routine first, against the argument order.
// Measured: blit is two words (slot 88, 92 unused); one i for shard, flash,
// streak and slot; j for the fill columns and sparks, k for the ring columns;
// model and verts belong to each block; ring and offset are set together so
// ring spills; t, its shade and the yaw are values of i; cells is set several
// times so that loop.c leaves it in the block; fade is its own variable.
// x0 and y0 are set inside the sparks loop, hoisted after j = 0. The ring's
// vertices are struct stores: the second spill of rr is then dropped, which
// bytes through s8 * do not allow. mesh and vertices are read before the ring
// model's stores, and Ram_MapCellBuffer at that one site stops PRE sharing it.
// 0x02014000 is a number in q so that CSE cannot share it with the symbol below
// (as the symbol, differ 786 forwarded). Dropping c costs 47; both are kept.
// A pool word for a small number is a symbol: the ResourceId_ names here have
// to be given to rows of Resource_DirectoryTable before adoption, and the list
// end 0xff and the REG_DISPCNT = 1 pool words are still movs here.

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
s32 BattleEffect_LoadBlitter(s32 slot, s32 modeAndFlags); // BattleEffect_BuildBlitter
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
s32 Task_Add(void (*func)(void), s32 order);
s32 Task_Remove(void (*func)(void));
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

#include "bg_scroll.h"
#include "vram_tab.h"
#include "input.h"

struct EclipseObject {
  u8 unk0[9];
  u8 unk09 : 2;
  u8 priority : 2;
  u8 palette : 4;
  u8 unk0a[6];
  u8 slot;
};

struct EclipseVertex {
  s8 x;
  s8 y;
  s8 z;
  s8 pad;
};

struct WordPair {
  s32 a;
  s32 b;
};

// Rows of Resource_DirectoryTable; the directory has yet to name most of them.
extern char ResourceId_EarthHorizonBackdrop, ResourceId_AerialViewBackdrop;
extern char ResourceId_GoldenDragonTiles, ResourceId_GoldenDragonHeadTiles;
extern char ResourceId_BlueFlameSheet, ResourceId_DustBlobSheet;
extern char ResourceId_Data134, ResourceId_TornadoSheet, ResourceId_Data0B4;
extern char ResourceId_Data0C1, ResourceId_Data0C9, ResourceId_Data0CA;

extern u8 gMapCellBuffer[];
extern struct EffectStep gData_02014000[];
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
s32 Resource_DecodeType01(const void *source, void *destination);
s32 Func_08196404(s32 slot, s32 width, s32 height, s32 mode, s32 flags);
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
  (0x06010000 + gResourceTableEntries[(obj)->slot].offset)

// Eclipse: the battlefield scrolls up into a night sky, thirty-two shards
// of a black moon rise over the foes, its ring of light flares and
// collapses, then the moon's beam strikes and a dark sphere bursts.
void BattleEffect_RunEclipse(struct BattleEffectArgument *effect) {
  u8 *canvas;
  struct BattleEffectWork *work;
  DrawRectangle blit[2];
  s32 frame;
  s32 savedScroll;
  s32 *transition;
  u8 *sprites;
  u8 *battle;
  s32 i;
  s32 j;
  s32 k;
  s32 scroll;
  s32 baseX;
  s32 baseY;
  s32 velX;
  s32 velY;
  s32 cx;
  s32 cy;
  u32 u;
  s32 count;
  s32 ring;
  s32 shade;
  s32 offset;
  u8 *cells;
  s32 origin;
  s32 x0;
  s32 y0;
  s32 fade;
  s16 list[14];
  s32 place[4];
  struct WordPair req;
  struct ModelTexture texture;
  struct ModelTexture texture2;
  struct ModelTexture texture3;
  struct RenderModel *model;
  struct EffectStep *p;
  struct EclipseObject *obj;

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
      u8 *tiles;

      list[0] = 0xff;
      BattleActor_SpawnObjectsForListFar(list, 0);
      Resource_ResetEntry(*(s32 *)(battle + 84));
      VramBlock_Reset93Far();
      gCameraSceneParameters[4] = 240;
      BattleBackground_LoadFar(1, (s32)&ResourceId_EarthHorizonBackdrop, 0);
      work->scrollTimer = 0;
      work->scrollInterval = 1;
      work->scrollStepX = -1;
      work->scrollStepY = 0;
      Task_Add(Func_0813baec, 0x480);
      transition[4] = 1;
      cells = Resource_GetTableEntry((s32)&ResourceId_GoldenDragonTiles);
      Iwram_CopyWords((void *)OBJ_PLTT, cells, 480);
      cells += 480;
      Resource_DecodeType01(cells, gMapCellBuffer);
      tiles = gMapCellBuffer;
      for (i = 0; i != 31; i++) {
        obj = Func_0815b290(32, 32, 0x80002000, 0);
        obj->priority = 2;
        work->objects[i] = obj;
        Iwram_CopyWords((void *)OBJ_TILES(obj), tiles, 0x400);
        tiles += 0x400;
      }
      cells = Resource_GetTableEntry((s32)&ResourceId_GoldenDragonHeadTiles);
      Iwram_CopyWords((void *)OBJ_PLTT, cells, 480);
      cells += 480;
      Resource_DecodeType01(cells, gMapCellBuffer);
      obj = Func_0815b290(32, 32, 0x80002000, 0);
      work->objects[31] = obj;
      obj->priority = 2;
      Iwram_CopyWords((void *)OBJ_TILES(obj), gMapCellBuffer, 0x400);
    }
    scroll += 4;
    REG_BG2X = scroll << 8;
    work->transferPending = 1;
    wait(1);
  }
  Iwram_FillWords(canvas, 0x3c00, 0);
  Iwram_FillWords((void *)0x06004000, 0x3c00, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
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
  blit[0] = Ram_HeapSlots->unk64[1];
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_BlueFlameSheet), 128);
  for (frame = 0; frame != 220 && !(gInput.repeat & 3); frame++) {
    if (frame == 0) {
      baseX = -0x300000;
      baseY = 0xd40000;
      velX = 0x40000;
      velY = -0xa0000;
      work->fadeFrames = 1;
      work->fadeStep = frame;
      work->particles[16].variant = frame;
      Resource_LoadAndDecompress((s32)&ResourceId_Data0B4, gData_02014000, 0, 0);
    }
    if (frame == 136) {
      u8 *dst = Data_02010400;

      for (i = 0; i != 7; i++) {
        s32 size = 0x400;

        Iwram_CopyWordsEntry(
            (void *)OBJ_TILES(
                (struct EclipseObject *)work->objects[Data_08197748[i]]),
            dst, size);
        dst += 0x400;
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
    cx = baseX / 2 - (work->particles[16].variant << 13) + 0x1c0000;
    cy = baseY + (work->particles[16].variant << 14) + 0x380000;
    u = frame - 136;
    if (u <= 84) {
      if (frame == 136) {
        work->particles[16].variant = 4;
        work->particles[16].x = 0;
        work->particles[16].z = 0;
        Func_0815b434(work->sheet, work->particles[16].variant * 2, 0x10000);
      }
      if (frame > 171) {
        if (frame == 172) {
          work->particles[16].variant = 64;
          Func_0815b434(work->sheet, 128, 0x10000);
          velX = 0x30000;
          velY = -0x30000;
        }
        if (frame > 203 && work->particles[16].variant > 1) {
          work->particles[16].variant -= 4;
          if (work->particles[16].variant <= 1) {
            work->particles[16].variant = 2;
          }
          Func_0815b434(work->sheet, work->particles[16].variant * 2, 0x10000);
        }
      } else if (frame > 135) {
        velX += -0x800;
        velY += 0x800;
        if (++work->particles[16].variant <= 32) {
          Func_0815b434(work->sheet, work->particles[16].variant * 2, 0x10000);
        } else {
          work->particles[16].variant = 32;
        }
      }
      {
        s32 size = work->particles[16].variant;

        x0 = cx >> 16;
        y0 = cy >> 16;
        Func_0818caa8(work->sheet, x0, y0, size * 2);
      }
    }
    if (frame > 47) {
      struct RenderModel *model;
      void *verts;

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
        Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, work->sheet + 0x2000, 1, 0);
        Resource_LoadAndDecompress((s32)&ResourceId_Data0C9, work->sheet + 0x4000, 0, 0);
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
      origin = -0x400000;
      for (i = 0; i != 8; i++, p++) {
        p->variant++;
        p->x += p->y;
        if ((u32)p->variant <= 47) {
          Render_ResetTransformState();
          Func_08015160(cx + ((i & 3) << 17) + 0x20000 + origin, cy - (((i & 3) + 2) << 16) + origin, 0);
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
      for (j = 0; j != 4; j++) {
        s32 t = (frame + j) & 3;
        s32 x;
        s32 y;

        x0 = cx >> 16;
        y0 = cy >> 16;
        if (frame <= 171) {
          s32 size = work->particles[16].variant * 2;

          x = Random16() % size + x0 - (size >> 1) - (gData_08197492[t] >> 1);
          y = Random16() % size + y0 - (size >> 1) - (gData_08197498[t] >> 1);
        } else {
          u32 a = Random16() & 63;
          s32 r = work->particles[16].variant;

          x = x0 - (a >> 1) - (gData_08197492[t] >> 1) + Random16() % r - r / 2 - 8;
          y = a + y0 - (gData_08197498[t] >> 1);
        }
        Func_08196404(SLOT_ALT, 7, 7, Data_08197744[Random16() & 3] | 3, 1);
        ((DrawRectangle)Ram_HeapSlots->unkb8[1])(
            canvas, work->sheet + 0x2000 + gData_08197486[t], x, y,
            gData_08197492[t], gData_08197498[t]);
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
  Resource_LoadAndDecompress((s32)&ResourceId_Data0CA, work->sheet + 0x3840, 0, 0);
  REG_BLDALPHA = 0x1010;
  for (i = 0; i != 120; i++) {
    s32 v = i / 2;
    u8 *c;

    if (v > 63) {
      v = 63;
    }
    for (j = 0, c = work->sheet + i; j != 120; j++) {
      *c = v;
      c += 120;
    }
  }
  fade = 0;
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
      struct RenderModel *model;
      void *verts;

      verts = Heap_AllocEwram(32);
      model = Render_AllocateModelList(1);
      texture2.logWidth = 5;
      texture2.logHeight = 2;
      texture2.pixels = work->sheet + 0x3840;
      model->mesh = Data_08199220;
      model->shade = fade;
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
          cells = work->sheet + 0x38c0;
          Func_0815b434(cells, 96, 0x10000);
          Func_0818caa8(cells, q->x >> 16, q->y >> 16, 96);
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
  BattleBackground_LoadFar(1, (s32)&ResourceId_AerialViewBackdrop, 0);
  Func_08164b2c(31, 31, 31);
  gBgScroll[1].x = 0;
  gCameraSceneParameters[4] = 240;
  wait(1);
  Resource_LoadAndDecompress((s32)&ResourceId_DustBlobSheet, work->sheet, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C1, work->sheet + 0x400, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0CA, work->sheet + 0x3840, 0, 0);
  {
    s32 *v = &gData_02014000[0].variant;
    s32 c = count;

    for (i = 0; i != 512; i++) {
      *v = c;
      v += 7;
    }
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
      struct EffectStep *q = (struct EffectStep *)0x02014000 + ((frame * 16 + i) & 511);
      s32 a;
      s32 r;


      a = Random16();
      r = (Random16() & 15) + frame * 2;
      q->x = Trig_Sin(a) * r + 0x3c0000;
      q->y = Trig_Cos(a) * r + 0x3c0000;
      q->velocityX = 0;
      q->velocityY = 0;
      q->variant = (Random16() & 15) + 16;
    }
    p = gData_02014000;
    for (i = 0; i != 512; i++, p++) {
      if (p->variant >= 0) {
        s32 n = (p->variant >> 3) + 1;

        blit[0](canvas, sprites + gData_08197410[n - 1], (p->x >> 16) - n / 2,
             (p->y >> 16) - n, n, n * 2);
        BattleEffectKernels_IntegrateVector2(p, 60, -0x4000);
        p->variant--;
      }
    }
    model = Render_AllocateModelList(1);
    shade = 0;
    if (frame <= 31) {
      s32 s = 0x10000;

      if (frame > 15) {
        s += 0x10000 - (frame << 12);
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
      Sound_PlayFar(208);
      Func_0818de3c(Data_02011000, 16, 8, 8, 8);
      for (ring = 0, offset = 0; ring != 8; offset += 64, ring++) {
        s32 y = -Trig_Cos(ring * 0x888) * 96 >> 16;
        s32 rr = Trig_Sin(ring * 0x888) * 48 >> 16;
        struct EclipseVertex *v = (struct EclipseVertex *)Data_02010800;

        v = (struct EclipseVertex *)((u8 *)v + offset);
        for (k = 0; k != 16; k++) {
          v->x = Trig_Sin(k << 12) * rr >> 16;
          v->y = y;
          v->z = Trig_Cos(k << 12) * rr >> 16;
          v++;
        }
      }
      Func_081885f0(Data_02012000, Data_02012800, 32);
    }
    texture3.logWidth = 5;
    texture3.logHeight = 5;
    texture3.pixels = work->sheet;
    {
      const void *mesh = Data_02011000;
      void *vertices = Ram_MapCellBuffer;

      model->kind = 6;
      model->mesh = mesh;
      model->vertices = vertices;
      model->texture = &texture3;
      model->shade = shade;
      model->unk04 = 3;
    }
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
    model->vertices = gMapCellBuffer;
    model->mesh = Data_02012800;
    model->kind = 7;
    texture3.pixels = work->sheet + 0x400;
    model->texture = &texture3;
    model->unk04 = shade;
    model->scrollU = shade;
    model->scrollV = shade;
    {
      for (i = 0; i != 4; i++) {
        s32 t = frame + 24 - i * 6;

        if ((u32)t <= 63) {
          s32 s = (Data_081977e4[i] * t << 10) + 0x4000;

          shade = 0;
          if (t <= 15) {
            shade = t * 4 - 64;
          }
          if (t > 40) {
            shade = (40 - t) * 4;
          }
          model->shade = shade;
          Render_ResetTransformState();
          Func_08015160(-0x20000, 0x200000, 0);
          Func_080151e4(s / 4, s / 4, s / 4);
          SceneTransform_ApplyPitch(10000);
          SceneTransform_ApplyYaw((frame + i * 8) << 10);
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

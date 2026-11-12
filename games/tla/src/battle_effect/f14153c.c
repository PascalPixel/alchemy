#include "types.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "effect_step.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "heap_release.h"
#include "ram_buffer.h"

struct FxPartyList;
struct MeshTriangle;
struct MeshVertex;
struct Model;
struct ResourceObjectWork;
struct StripQuad;

// A texture for the IWRAM renderer.
struct ModelTexture {
  u32 logWidth : 8;
  u32 logHeight : 8;
  u32 unk02 : 16;
  const u8 *pixels;
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

#include "bg_scroll.h"
#include "vram_tab.h"
#include "input.h"
#include "task.h"
#include "battle.h"
#include "motobj.h"
#include "item_icon.h"
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
extern u16 gData_08197410[];
extern u16 gData_08197486[];
extern u8 gData_08197492[];
extern u8 gData_08197498[];
extern const u8 gData_081991b0[];
extern const u8 Data_081991c0[];
extern const u8 gData_08199210[];
extern const u8 Data_08199220[];
extern const u8 gData_08199340[];

struct SpriteObject *Func_0815b290(s32 width, s32 height, u32 shape,
                                   u32 attributes);
void ResourceObject_ReleaseFar(struct ResourceObjectWork *work);
void Func_08179e6c(void);
void Func_0813baec(void);
void BattleActor_SpawnObjectsForListFar(s16 *list, s32 mode);

void VramBlock_Reset93Far(void);
void BattleBackground_LoadFar(s32 layer, s32 background, s32 palette);
s32 Resource_DecodeType01(const void *source, void *destination);
s32 Func_08196404(s32 slot, s32 widthShift, s32 heightShift, s32 flags,
                  u32 mode);
void Func_08020388(void);
s32 VramCache_AllocEntry(s32 size);
void Palette_LoadBasicObjColors(void);
void Func_08164abc(s32 r, s32 g, s32 b);
void Func_08164b2c(s32 r, s32 g, s32 b);
void Func_081885f0(struct MeshVertex *vertices, struct MeshTriangle *triangles,
                   s32 segments);
void Graphics_LoadGreyRamp(void);
void Func_0818de3c(struct MeshTriangle *triangles, s32 columns, s32 rows,
                   s32 cellWidth, s32 cellHeight);

#define SLOT_ALT 188 // unkb8[1]
#define OBJ_TILES(obj) (0x06010000 + gResourceTableEntries[(obj)->slot].offset)

// Forwarded so that a constant size reaches the call's register at each call.
static __inline__ void FillWords_(void *destination, s32 size, u32 value) {
  Iwram_FillWords(destination, size, value);
}
static __inline__ void CopyWords_(void *destination, const void *source,
                                  s32 size) {
  Iwram_CopyWords(destination, source, size);
}

// Eclipse: the battlefield scrolls up, thirty-two objects (31 with
// Summon_GoldenDragonTiles, 1 with Summon_GoldenDragonHeadTiles) rise over
// the foes, a ring flares and collapses, then a beam strikes and a sphere
// bursts.
// XXX: what the player sees in each stage; a tbs-en-style render of the
// effect would settle it.
void BattleEffect_RunEclipse(struct BattleEffectArgument *effect) {
  u8 *canvas;
  struct BattleEffectWork *work;
  // Unused: blit[1] gives the stack frame its 4 bytes.
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

  /* FAKEMATCH(wrapper): every fill through FillWords_, which sets a constant
   * size in the call's register at each call; as direct Iwram_FillWords calls,
   * 501 of 2159 instructions differ in tla-en. */
  /* FAKEMATCH(wrapper): every copy through CopyWords_, which sets a constant
   * size in the call's register at each call; as direct Iwram_CopyWords calls,
   * 133 of 2159 instructions differ in tla-en. */
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
      CopyWords_((void *)OBJ_PLTT, cells, 480);
      cells += 480;
      Resource_DecodeType01(cells, gMapCellBuffer);
      tiles = gMapCellBuffer;
      for (i = 0; i != 31; i++) {
        obj = Func_0815b290(32, 32, 0x80002000, 0);
        obj->priority = 2;
        work->objects[i] = obj;
        CopyWords_((void *)OBJ_TILES(obj), tiles, 0x400);
        tiles += 0x400;
      }
      cells = Resource_GetTableEntry((s32)&ResourceId_GoldenDragonHeadTiles);
      CopyWords_((void *)OBJ_PLTT, cells, 480);
      cells += 480;
      Resource_DecodeType01(cells, gMapCellBuffer);
      obj = Func_0815b290(32, 32, 0x80002000, 0);
      work->objects[31] = obj;
      obj->priority = 2;
      CopyWords_((void *)OBJ_TILES(obj), gMapCellBuffer, 0x400);
    }
    scroll += 4;
    REG_BG2X = scroll << 8;
    work->transferPending = 1;
    wait(1);
  }
  FillWords_(canvas, 0x3c00, 0);
  FillWords_((void *)0x06004000, 0x3c00, 0);
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
  CopyWords_((void *)BG_PLTT,
             Resource_GetTableEntry((s32)&ResourceId_BlueFlameSheet), 128);
  {
    const u8 *texturePixels = (const u8 *)gData_02014000;

    for (frame = 0; frame != 220 && !(gInput.repeat & 3); frame++) {
      if (frame == 0) {
        baseX = -0x300000;
        baseY = 0xd40000;
        velX = 0x40000;
        velY = -0xa0000;
        work->fadeFrames = 1;
        work->fadeStep = frame;
        work->particles[16].variant = frame;
        Resource_LoadAndDecompress((s32)&ResourceId_Data0B4, gData_02014000, 0,
                                   0);
      }
      if (frame == 136) {
        u8 *dst = Data_02010400;
        for (i = 0; i != 7; i++) {

          void *o = (void *)OBJ_TILES(
              (struct EclipseObject *)work->objects[Data_08197748[i]]);
          // Default: set for real below; as s32 size = 0x400, 762 differ.
          s32 size = 0;
          size = 0x400;
          Iwram_CopyWordsEntry(o, &dst[i * 0x400], size);
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
          FillWords_(canvas, 0x3c00, Data_08197760[i]);
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
          Render_ApplyProjectedPlacementFar(work->objects[i], place,
                                            (s32 *)&req, 0);
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
            Func_0815b434(work->sheet, work->particles[16].variant * 2,
                          0x10000);
          }
        } else if (frame > 135) {
          velX += -0x800;
          velY += 0x800;
          if (++work->particles[16].variant <= 32) {
            Func_0815b434(work->sheet, work->particles[16].variant * 2,
                          0x10000);
          } else {
            work->particles[16].variant = 32;
          }
        }
        {
          /* FAKEMATCH(temporary): the variant copied into size in a block of
           * its own; as Func_0818caa8(..., work->particles[16].variant * 2), 50
           * of 2159 instructions differ in tla-en. */
          s32 size = work->particles[16].variant;

          Func_0818caa8(work->sheet, cx >> 16, cy >> 16, size * 2);
        }
      }
      if (frame > 47) {
        struct RenderModel *model;
        void *verts;
        s32 modelShade;

        if (frame == 48) {
          for (i = 0; i != 16; i++) {
            work->particles[i].x = Random16() & 0xffff;
            work->particles[i].y = (Random16() & 255) + 128;
            if (Random16() & 1) {
              work->particles[i].y = -work->particles[i].y;
            }
            work->particles[i].variant = i * -8;
          }
          Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet,
                                     work->sheet + 0x2000, 1, 0);
          Resource_LoadAndDecompress((s32)&ResourceId_Data0C9,
                                     work->sheet + 0x4000, 0, 0);
        }
        verts = Heap_AllocEwram(32);
        model = Render_AllocateModelList(1);
        modelShade = 0;
        model->shade = modelShade;
        texture.logWidth = 5;
        texture.logHeight = 2;
        texture.pixels = work->sheet + 0x4000;
        model->kind = 6;
        model->mesh = Data_08199220;
        model->texture = &texture;
        model->vertices = verts;
        for (i = 0; i != 8; i++) {
          work->particles[i].variant++;
          work->particles[i].x += work->particles[i].y;
          if (work->particles[i].variant >= 0 &&
              work->particles[i].variant <= 47) {
            Render_ResetTransformState();
            {
              s32 x = ((i & 3) << 17) + 0x20000;
              s32 o = -0x400000;
              Func_08015160(cx + x + o, cy - (((i & 3) + 2) << 16) + o, 0);
            }
            Func_080151e4(0x8000, 0x10000, 0x10000);
            Func_080150e4(work->particles[i].x);
            Func_080151e4(Trig_Sin(work->particles[i].variant * 682) + 0x4000,
                          0x20000, 0x10000);
            Render_TransformVertices(gData_081991b0, verts, 4);
            Render_DrawModelList(model);
          }
        }
        if (frame >= 136 && frame <= 171) {
          s32 s = 0x40000 - u * 6000;

          modelShade = u * 4 - 64;
          if (modelShade > -16) {
            modelShade = -16;
          }
          texture.logWidth = 6;
          model->shade = modelShade;
          texture.logHeight = 6;
          model->kind = 7;
          texture.pixels = texturePixels;
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
        if (frame >= 172 && frame <= 220) {
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
      if (frame >= 148 && frame <= 219) {
        for (j = 0; j != 4; j++) {
          s32 t = (frame + j) & 3;
          // Default: set for real below.
          s32 x = 0;
          s32 y = 0;

          if (frame <= 171) {
            s32 size = work->particles[16].variant * 2;

            x = Random16() % size + (cx >> 16) - (size >> 1) -
                (gData_08197492[t] >> 1);
            y = Random16() % size + (cy >> 16) - (size >> 1) -
                (gData_08197498[t] >> 1);
          } else {
            u32 a = Random16() & 63;
            s32 r = work->particles[16].variant;

            x = (cx >> 16) - (a >> 1) - (gData_08197492[t] >> 1) +
                Random16() % r - r / 2 - 8;
            y = a + (cy >> 16) - (gData_08197498[t] >> 1);
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
  // Two empty once-only blocks before the fills: without them, 18 differ.
  do {
  } while (0);
  FillWords_(canvas, 0x4000, 0);
  do {
  } while (0);
  FillWords_((void *)0x06004000, 0x4000, 0);
  wait(1);
  REG_BLDALPHA = 0x1010;
  REG_BLDCNT = 0x3f46;
  REG_DISPCNT = 0x7741;
  gCameraSceneParameters[4] = 120;
  work->transferMode = 3;
  work->transferValue = 0x02020202;
  FillWords_(canvas, 0x4000, 0x3f3f3f3f);
  REG_BG2CNT = 0x784;
  effect->actors[effect->count] = 0xff;
  BattleActor_SpawnObjectsForListFar(effect->actors, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0CA, work->sheet + 0x3840, 0,
                             0);
  REG_BLDALPHA = 0x1010;
  for (i = 0; i != 120; i++) {
    s32 v = i / 2;

    if (v > 63) {
      v = 63;
    }
    for (j = 0; j != 120; j++) {
      work->sheet[j * 120 + i] = v;
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
      FillWords_(canvas, 0x4000, 0x3f3f3f3f);
      for (i = 0; i != effect->count; i++) {
        Func_0814cd48(effect->actors[i], 7, 5, i, 4);
      }
    }
    if (frame <= 109) {
      // Default: set for real below.
      struct RenderModel *model = 0;
      void *verts = 0;

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
      BattleCamera_Shake(r, r);
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
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C1, work->sheet + 0x400, 0,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0CA, work->sheet + 0x3840, 0,
                             0);
  {
    s32 *v = &gData_02014000[0].variant;
    s32 c = -1;

    for (i = 0; i != 512; i++) {
      *v = c;
      v += 7;
    }
  }
  work->transferMode = 2;
  work->transferValue = 50;
  gBgScroll[1].y = 32;
  {
    const void *mesh = Data_02011000;
    void *vertices = gMapCellBuffer;
    const void *mesh2 = Data_02012800;

    for (frame = 0; frame != 26; frame++) {
      if ((gInput.repeat & 3) && count == -1) {
        count = 8;
      }
      if (frame <= 6 && count == -1) {
        s32 b = 31 - frame * 8;
        s32 a = b;
        s32 c = b;

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
        // A constant here and in p below, so both share one pool word (as
        // gData_02014000, 70 to 786 differ).
        struct EffectStep *q =
            (struct EffectStep *)(Ram_MapCellBuffer + 0x4000) +
            ((frame * 16 + i) & 511);
        // Default: set for real below.
        s32 a = 0;
        s32 r = 0;

        a = Random16();
        r = (Random16() & 15) + frame * 2;
        q->x = Trig_Sin(a) * r + 0x3c0000;
        q->y = Trig_Cos(a) * r + 0x3c0000;
        q->velocityX = 0;
        q->velocityY = 0;
        q->variant = (Random16() & 15) + 16;
      }
      {
        const u16 *sizeOffsets = gData_08197410;
        for (i = 0; i != 512; i++) {
          p = (struct EffectStep *)(Ram_MapCellBuffer + 0x4000) + i;
          if (p->variant >= 0) {
            s32 n = (p->variant >> 3) + 1;
            // sizeOffsets[n - 1] spelled as a byte offset: plain, 49 differ.
            s32 off = n * 2 - 2;

            blit[0](canvas,
                    sprites + *(const u16 *)((const u8 *)sizeOffsets + off),
                    (p->x >> 16) - n / 2, (p->y >> 16) - n, n, n * 2);
            BattleEffectKernels_IntegrateVector2(p, 60, -0x4000);
            p->variant--;
          }
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
        {
          s32 ringAngle = 0;
          for (ring = 0, offset = 0; ring != 8;
               ringAngle += 0x888, offset += 64, ring++) {
            s32 y = -Trig_Cos(ringAngle) * 96 >> 16;
            s32 rr = Trig_Sin(ringAngle) * 48 >> 16;
            struct EclipseVertex *v = (struct EclipseVertex *)Data_02010800;

            k = 0;
            v = (struct EclipseVertex *)((u8 *)v + offset);
            for (; k != 16; k++) {
              v->x = Trig_Sin(k << 12) * rr >> 16;
              v->y = y;
              v->z = Trig_Cos(k << 12) * rr >> 16;
              v++;
            }
          }
        }
        Func_081885f0(Data_02012000, Data_02012800, 32);
      }
      texture3.logWidth = 5;
      texture3.logHeight = 5;
      texture3.pixels = work->sheet;
      {
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
        Render_TransformVertices(Data_02010800, vertices, 128);
        Render_DrawModelList(model);
      }
      texture3.logHeight = 5;
      texture3.logWidth = 7;
      texture3.pixels = work->sheet + 0x400;
      model->texture = &texture3;
      model->kind = 7;
      model->mesh = mesh2;
      model->unk04 = shade;
      model->vertices = vertices;
      model->scrollU = shade;
      model->scrollV = shade;
      {
        for (i = 0; i != 4; i++) {
          s32 t = frame + 24 - i * 6;

          if (t >= 0 && t <= 63) {
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
            Render_TransformVertices(Data_02012000, vertices, 64);
            Render_DrawModelList(model);
          }
        }
      }
      Heap_Release(model);
      work->transferPending = 1;
      wait(1);
    }
  }
  FillWords_((void *)0x06004000, 0x3c00, 0x3f3f3f3f);
  gBgScroll[1].x = savedScroll;
  transition[4] = 0;
  gCameraSceneParameters[4] = 120;
  BattleBackground_LoadFar(1, *(u16 *)(battle + 0x670), 24);
  wait(1);
  work->transferMode = 3;
  work->transferValue = 0x02020202;
  Graphics_LoadGreyRamp();
  FillWords_(canvas, 0x3c00, 0x3f3f3f3f);
  Func_081180e8(134);
  for (frame = 0; frame != 32; frame++) {
    work->transferPending = 1;
    wait(1);
  }
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(SLOT_BLITTER);
  BattleEffect_EndCanvasLayer();
}

// Unused: two empty functions the ROM keeps after Eclipse; nothing calls them.
void Func_0814293c(void) {}

void Func_08142940(void) {}

#include "fixed_math.h"
#include "graphics.h"
#include "resource.h"
#include "battle_display.h"

struct Model;

// Heap slots, as byte offsets into Ram_HeapSlots.
#define SLOT_BATTLE_EFFECT 92 // battleFxWork[0]
#define SLOT_BATTLE_CANVAS 96 // battleFxWork[1]

// The whole effect work block: struct BattleEffectWork names only its first
// 0x78f4 bytes.
#define BATTLE_EFFECT_WORK_SIZE 0x7b7c

#define CLIP (COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y)

// The pixel half of a 16.16 value.
#define HI(value) (((s16 *)&(value))[1])

// The three kinds the scenes ask for.
enum { KIND_RIGHT = 78, KIND_LEFT = 79, KIND_FINALE = 80 };

// One model for the IWRAM renderer.
struct ModelDraw {
  s32 kind;
  s32 next;
  const void *model;
  void *corners;
  struct ModelTexture *texture;
  s32 depthBias;
  u8 unk18;
  u8 shade;
  u8 unk1a[2];
};

// The four crystals: the frame each starts after 48, how fast it grows and
// how tall.
extern u8 Data_081977ec[];
extern u8 Data_081977f0[];
extern u8 Data_081977f4[];

// The models: the rings' quad, its corners, the crystal and its corners.
extern const u8 Data_08199220[];
extern const u8 gData_081990d0[];
extern const u8 gData_08199090[];

// The crystal texture and the flashes live in EWRAM after the map cells.
extern u8 Data_02012000[];

extern char ResourceId_FlashBurstSheet;
extern char ResourceId_Data0C9;
extern char ResourceId_Data0DA;
extern char ResourceId_OrangePaletteB;
extern char ResourceId_EffectPalette03;

void Func_0814cd48(s32 actor, s32 palette, s32 frames, s32 timerSlot,
                   s32 timerFrames); // BattleEffect_TintBattler
void Func_08164a4c(s32 blueDelta, s32 greenDelta, s32 redDelta);
void Func_08118150(s32 kind);

// Rings and crystals gather on the user, for the scenes.

// A glow on the canvas map widens over the user for 56 frames while 16
// spinning rings rise around it, four abreast. From frame 48 the screen
// shakes, flashes burst upward and four crystals grow out of the ground one
// after another, spinning. The user flashes at frames 32 to 68. Kinds 78
// and 79 stand to either side and hand over to the battle (Func_08118150)
// at frame 62; kind 80 runs 100 frames, fades to white and restores the
// scene's view itself.

void Func_08142944(struct BattleEffectArgument *effect, s32 kind) {
  struct EffectPosition center;
  struct ModelTexture texture;
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle draw[2];
  s32 frames;
  s32 frame;
  s32 index;
  s32 i;

  work = HeapSlot_Alloc(SLOT_BATTLE_EFFECT, BATTLE_EFFECT_WORK_SIZE);
  canvas = HeapSlot_Alloc(SLOT_BATTLE_CANVAS, 0x4000);
  work->vblanksSinceTransfer = 1;
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = 0x1010;
  BattleEffect_LoadBlitter(SLOT_BLITTER, BLITTER(BLIT_ADDITIVE, CLIP));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, work->sheet, 1,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C9, work->sheet + 0x3000, 0,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0DA, Data_02012000, 0, 0);
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  work->fadeFrames = 24;
  work->fadeStep = 0;

  frames = 100;
  if (kind != KIND_FINALE)
    frames = 76;
  for (frame = 0; frame != frames; frame++) {
    index = (frame - 48) * 2;
    // The effect's centre, beside or above the user.
    EffectPosition_ProjectBattler(effect->actor, &center);
    if (kind == KIND_RIGHT) {
      center.x += 40;
      center.y -= 24;
    } else if (kind == KIND_LEFT) {
      center.x -= 16;
      center.y -= 24;
    } else {
      center.y -= 48;
      /* FAKEMATCH(barrier): an empty asm taking the centre's address gives it
       * two more references, so the register allocator ranks it above the
       * corners; without it, 189 of 734 instructions differ in tla-en. */
      __asm__("" : : "r"(&center));
    }

    if (frame == 0) {
      work->transferMode = 2;
      work->transferValue = 50;
      if (kind == KIND_LEFT)
        Iwram_CopyWords((void *)BG_PLTT,
                        Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB),
                        128);
      else
        Iwram_CopyWords(
            (void *)BG_PLTT,
            Resource_GetTableEntry((s32)&ResourceId_EffectPalette03), 128);
      if (frame == 0)
        Sound_PlayFar(140);
    }
    if (frame == 48)
      Sound_PlayFar(144);
    if (frame == 56)
      Sound_PlayFar(148);
    if (frame == 32)
      Func_0814cd48(effect->actor, 7, -1, -1, -1);
    if (frame == 68)
      Func_0814cd48(effect->actor, 0, -1, -1, -1);
    if (kind == KIND_FINALE) {
      // The finale fades to white from frame 62.
      if (frame > 61) {
        work->transferMode = 1;
        work->transferValue = 0x10101010;
        Func_08164a4c(2, 2, 2);
      }
    } else if (frame == 62) {
      Func_08118150(kind);
    }

    // From frame 48: shake, clear the canvas, and for 8 frames burst two
    // flashes a frame from a point near the centre.
    if (frame >= 48) {
      struct EffectStep *flashes;

      if (frame == 48) {
        {
          s32 *variant = &gData_02014000[0].variant;

          for (i = 0; i != 64; i++) {
            *variant = -1;
            variant += 7;
          }
        }
        work->shakeFrames = 8;
        Iwram_FillWords(canvas, 0x3c00, 0x3f3f3f3f);
      }
      flashes = gData_02014000;
      if (frame >= 48 && frame <= 55) {
        s32 x = ((Random16() & 63) + center.x / 2 - 32) << 16;
        s32 y = ((Random16() & 31) + center.y) << 16;
        s32 k = 0;
        s32 spread = 15;
        struct EffectStep *burst = &flashes[index];

        for (; k != 2; k++) {
          struct EffectStep *flash = &burst[k];
          s32 speed = 31; // Set first: the mask is the tied operand.
          s32 angle;

          speed &= Random16();
          angle = Random16();

          flash->x = x + (((Random16() & spread) - 8) << 16);
          flash->y = y + (((Random16() & spread) - 8) << 16);
          flash->velocityX = Trig_Sin(angle) * (speed + 32) >> 3;
          flash->velocityY = Trig_Cos(angle) * (speed + 32) >> 3;
          flash->variant = 0;
        }
      }
      {
        struct EffectStep *flash = flashes;

        for (i = 0; i != 16; i++, flash++) {
          if (flash->variant >= 0 && flash->variant <= 17) {
            draw[0](canvas, work->sheet + (flash->variant / 3 << 11),
                    HI(flash->x) - 16, HI(flash->y) - 48, 32, 64);
            BattleEffectKernels_IntegrateVector2(flash, 62, -0x800);
            flash->variant++;
          }
        }
      }
    }

    if (frame >= 0) {
      s32 cx = center.x / 2 << 16;
      s32 cy = center.y << 16;
      struct ModelDraw *model;
      void *corners;
      s32 bias;

      // The glow on the canvas map widens for 56 frames.
      if (frame <= 63) {
        s32 size = 128;
        s32 width = frame + 8;

        if (width <= 64) {
          size = width * 2;
          Func_0815b434(Ram_MapCellBuffer, size, 0x10000);
        }
        Func_0818caa8(Ram_MapCellBuffer, cx >> 16, cy >> 16, size);
      }

      // Each ring has an angle (x), a spin (y) and an age (variant), the
      // later ones 4 frames apart.
      if (frame == 0) {
        for (i = 0; i != 16; i++) {
          struct EffectStep *ring = &work->particles[i];

          ring->x = Random16() & 0xffff;
          ring->y = (Random16() & 255) + 128;
          if (Random16() & 1)
            ring->y = -ring->y;
          ring->variant = -4 * i;
        }
      }

      corners = Heap_AllocEwram(0x200);
      model = Render_AllocateModelList(1);
      texture.logWidth = 5;
      bias = 0;
      model->depthBias = bias;
      texture.logHeight = 2;
      texture.pixels = work->sheet + 0x3000;
      model->kind = 7;
      model->texture = &texture;
      model->model = Data_08199220;
      model->corners = corners;

      // The rings rise for 24 frames, spinning and swelling.
      for (i = 0; i != 16; i++) {
        struct EffectStep *ring = &work->particles[i];

        ring->variant += 2;
        ring->x += ring->y * 2;
        if (ring->variant >= 0 && ring->variant <= 47) {
          Render_ResetTransformState();
          {
            s32 column = i & 3;

            Func_08015160(cx + ((column * 2 + 2) << 16) - 0x400000,
                          cy - ((column + 2) << 16) - 0x400000, 0);
          }
          Func_080151e4(0x8000, 0x10000, 0x10000);
          Func_080150e4(ring->x);
          Func_080151e4(Trig_Sin(ring->variant * 682) + 0x10000, 0x20000,
                        0x10000);
          Render_TransformVertices(gData_081991b0, corners, 4);
          Render_DrawModelList(model);
        }
      }

      // The four crystals grow out of the ground from frame 48 on.
      model->texture = &texture;
      model->corners = corners;
      texture.logWidth = 6;
      texture.pixels = Data_02012000;
      model->kind = 7;
      texture.logHeight = 6;
      model->model = gData_081990d0;
      for (i = 0; i != 4; i++) {
        s32 start = Data_081977ec[i] + 48;

        model->shade = (frame * 8 + i * 32) & 127;
        if (frame > start) {
          bias = (start - frame) * 8 + 56;
          if (bias > -16)
            bias = -16;
          if (bias > -64) {
            s32 growth = (frame - start) * Data_081977f0[i] * 2000 + 0x4000;

            model->depthBias = bias;
            Render_ResetTransformState();
            Func_08015160((center.x / 2 - 60) << 16, 0x200000, 0);
            Func_080151e4(growth * 2, Data_081977f4[i] * growth + 0x40000,
                          growth / 4);
            SceneTransform_ApplyPitch(1000);
            if (i & 1)
              SceneTransform_ApplyYaw((i * 8 + frame) << 10);
            else
              SceneTransform_ApplyYaw((i * 8 - frame) << 10);
            Render_TransformVertices(gData_08199090, corners, 32);
            Render_DrawModelList(model);
          }
        }
      }
      Heap_Release(model);
      Heap_Release(corners);
    }

    BattleCamera_Shake(8, 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(SLOT_BLITTER);
  if (kind == KIND_FINALE) {
    // Put the scene's view back by hand.
    Sound_PlayFar(390);
    gBgScroll[1].x = work->savedBg1X;
    gBgScroll[1].y = work->savedBg1Y;
    gCameraSceneParameters[3] = 120;
    gCameraSceneParameters[4] = 120;
    Task_Remove(Palette_StepFadeTransfer);
  } else {
    BattleEffect_EndCanvasLayer();
  }
  HeapSlot_Release(SLOT_BATTLE_CANVAS);
  HeapSlot_Release(SLOT_BATTLE_EFFECT);
}

#include "irq_lock.h"

/// The battle effect work, as far as the canvas transfer and the canvas layer
/// read it.
struct CanvasWork {
  u8 unk0000[0x7780];
  s32 transferMode;
  s32 transferValue;
  u8 unk7788[0x18];
  s32 savedBg1X;
  s32 savedBg1Y;
  s32 shakeFrames;
  u8 unk77ac[8];
  s32 fadeFrames;
  s32 fadeStep;
  u8 unk77bc[0x128];
  s32 vblanksSinceTransfer;
  s32 transferPending;
};

void ColorBuffer_Halve(void *source, void *destination, s32 size);
void ColorBuffer_ScaleThreeQuarters(void *source, void *destination, s32 size);
void ColorBuffer_Darken(void *source, s32 amount, void *destination, s32 size);
void ColorBuffer_Brighten(void *source, s32 amount, void *destination,
                          s32 size);

/// The frame callback that copies the effect canvas to BG character block 1
/// when a transfer is pending, in the way the transfer mode asks for.
void BattleEffect_ProcessPendingGraphicsTransfer(void) {
  struct CanvasWork *work = (struct CanvasWork *)Ram_HeapSlots->battleFxWork[0];
  void *source = Ram_HeapSlots->battleFxWork[1];
  u32 ime;

  IRQ_LOCK(ime);
  if (work->transferPending == 1) {
    work->transferPending = 0;
    IRQ_UNLOCK(ime);
    switch (work->transferMode) {
    case 0:
      /* FAKEMATCH(wrapper): the copy forwarded through CopyWords_, which keeps
       * each call's own constants, as the scene layer does; as direct
       * Iwram_CopyWords calls in this callback and in
       * BattleEffect_ProcessPendingStampTransfer,
       * BattleEffect_FlushPendingGraphicsTransfer and
       * BattleEffect_ProcessPendingBlock2Transfer, 128 of 369 instructions
       * differ in tla-en. */
      CopyWords_((void *)0x06004000, source, 0x4000);
      break;
    case 1:
      CopyWords_((void *)0x06004000, source, 0x4000);
      /* FAKEMATCH(wrapper): the fill forwarded through FillWords_, which keeps
       * each call's own constants, as the scene layer does; as direct
       * Iwram_FillWords calls in this callback and in the same three
       * callbacks, 16 of 369 instructions differ in tla-en. */
      FillWords_(source, 0x4000, work->transferValue);
      break;
    case 2:
      if (work->transferValue == 50)
        ColorBuffer_Halve(source, (void *)0x06004000, 0x4000);
      else
        ColorBuffer_ScaleThreeQuarters(source, (void *)0x06004000, 0x4000);
      break;
    case 3:
      ColorBuffer_Darken(source, work->transferValue, (void *)0x06004000,
                         0x4000);
      break;
    case 4:
      ColorBuffer_Brighten(source, work->transferValue, (void *)0x06004000,
                           0x4000);
      break;
    }
    // DEAD: nothing uses the counter read here; its load is in the ROM.
    (void)*(volatile s32 *)&work->vblanksSinceTransfer;
    work->vblanksSinceTransfer = 1;
  } else {
    IRQ_UNLOCK(ime);
    work->vblanksSinceTransfer++;
  }
}

/// When a transfer is pending, copies the 0x1000-byte canvas at work 0x6780
/// to the stamp's tiles (the pointer at work 0x7b74), then clears it.
void BattleEffect_ProcessPendingStampTransfer(void) {
  u8 *work = Ram_HeapSlots->battleFxWork[0];
  u32 ime;
  s32 *pending;

  IRQ_LOCK(ime);
  pending = (s32 *)(work + 0x78ec);
  if (*pending == 1) {
    *pending = 0;
    IRQ_UNLOCK(ime);
    CopyWords_(*(void **)(work + 0x7b74), work + 0x6780, 0x1000);
    FillWords_(work + 0x6780, 0x1000, 0);
  } else {
    IRQ_UNLOCK(ime);
  }
}

/// The same callback for the 0x7800-byte canvas in the map cell buffer, copied
/// to BG character block 2.
void BattleEffect_FlushPendingGraphicsTransfer(void) {
  struct CanvasWork *work = (struct CanvasWork *)Ram_HeapSlots->battleFxWork[0];
  void *source = Ram_MapCellBuffer;
  u32 ime;

  IRQ_LOCK(ime);
  if (work->transferPending == 1) {
    work->transferPending = 0;
    IRQ_UNLOCK(ime);
    switch (work->transferMode) {
    case 0:
      CopyWords_((void *)0x06008000, source, 0x7800);
      break;
    case 1:
      CopyWords_((void *)0x06008000, source, 0x7800);
      FillWords_(source, 0x7800, work->transferValue);
      break;
    case 2:
      if (work->transferValue == 50)
        ColorBuffer_Halve(source, (void *)0x06008000, 0x7800);
      else
        ColorBuffer_ScaleThreeQuarters(source, (void *)0x06008000, 0x7800);
      break;
    case 3:
      ColorBuffer_Darken(source, work->transferValue, (void *)0x06008000,
                         0x7800);
      break;
    }
    // DEAD: nothing uses the counter read here; its load is in the ROM.
    (void)*(volatile s32 *)&work->vblanksSinceTransfer;
    work->vblanksSinceTransfer = 1;
  } else {
    IRQ_UNLOCK(ime);
    work->vblanksSinceTransfer++;
  }
}

/// The same for the canvas in the second effect work block.
void BattleEffect_ProcessPendingBlock2Transfer(void) {
  struct CanvasWork *work = (struct CanvasWork *)Ram_HeapSlots->battleFxWork[0];
  void *source;
  u32 ime;

  IRQ_LOCK(ime);
  if (work->transferPending == 1) {
    source = Ram_HeapSlots->battleFxWork[1];
    work->transferPending = 0;
    IRQ_UNLOCK(ime);
    switch (work->transferMode) {
    case 0:
      CopyWords_((void *)0x06008000, source, 0x7800);
      break;
    case 1:
      CopyWords_((void *)0x06008000, source, 0x7800);
      FillWords_(source, 0x3c00, work->transferValue);
      break;
    case 2:
      if (work->transferValue == 50)
        ColorBuffer_Halve(source, (void *)0x06008000, 0x3c00);
      else
        ColorBuffer_ScaleThreeQuarters(source, (void *)0x06008000, 0x3c00);
      break;
    case 3:
      ColorBuffer_Darken(source, work->transferValue, (void *)0x06008000,
                         0x3c00);
      break;
    }
    // DEAD: nothing uses the counter read here; its load is in the ROM.
    (void)*(volatile s32 *)&work->vblanksSinceTransfer;
    work->vblanksSinceTransfer = 1;
  } else {
    IRQ_UNLOCK(ime);
    work->vblanksSinceTransfer++;
  }
}

void Func_08143354(void) {
  u32 *map = (u32 *)0x0600f800;
  u32 tiles;
  s32 row;
  s32 column;

  /* FAKEMATCH(wrapper): both fills through FillWords_, which sets each call's
   * own constants anew; as direct Iwram_FillWords calls, 33 of 40 instructions
   * differ in tla-en. */
  FillWords_((void *)map, 0x100, -1);
  map += 0x40;
  FillWords_((void *)map, 0x80, 0x03ff03ff);
  map += 0x20;
  tiles = (u32)(Ram_MapCellBuffer + 0x200);
  for (row = 0; row != 16; row++) {
    for (column = 0; column != 8; column++) {
      *map++ = tiles;
      tiles += 0x00020002;
    }
    for (column = 0; column != 8; column++)
      *map++ = 0x03ff03ff;
  }
}

#include "battle_fx_work.h"

// The scenes' frame callbacks that copy the display registers the effect
// keeps in its work to the hardware.

/// Writes the window, display and blend registers, all at once.
void Graphics_ApplyWindowBlendRegisters(void) {
  /* FAKEMATCH(pin): the work pointer kept in r2, so the registers' address
   * takes r1; as a plain local, 31 of 55 instructions differ in tla-en. */
  register struct BattleFxWork *work asm("r2") =
      (struct BattleFxWork *)Ram_HeapSlots->battleFxWork[0];

  {
    u32 value = work->win0h;

    /* FAKEMATCH(barrier): an empty asm through the first value loads it before
     * the register's address is built; without it, 14 of 55 instructions
     * differ in tla-en. */
    __asm__ volatile("" : "+l"(value));
    REG_WIN0H = value;
  }
  REG_WIN0V = work->win0v;
  REG_WIN1H = work->win1h;
  REG_WIN1V = work->win1v;
  REG_WININ = work->winin;
  REG_WINOUT = work->winout;
  REG_DISPCNT = work->dispcnt;
  REG_BLDCNT = work->bldcnt;
  REG_BLDALPHA = work->bldalpha;
}

/// Writes background 2's scale (BG2PA and BG2PD) and its reference point.
void Display_ApplyBg2Affine(void) {
  /* FAKEMATCH(pin): the work pointer kept in r2, so the registers' address
   * takes r1; as a plain local, 32 of 30 instructions differ in tla-en. */
  register struct BattleFxWork *work asm("r2") =
      (struct BattleFxWork *)Ram_HeapSlots->battleFxWork[0];

  {
    u32 value = work->bg2pa;

    /* FAKEMATCH(barrier): an empty asm through the first value loads it before
     * the register's address is built; without it, 14 of 30 instructions
     * differ in tla-en. */
    __asm__ volatile("" : "+l"(value));
    REG_BG2PA = value;
  }
  REG_BG2PD = work->bg2pd;
  /* FAKEMATCH(barrier): an empty asm keeps the next offset's step after the
   * store before it; without it, 2 of 30 instructions differ in tla-en. */
  __asm__ volatile("");
  REG_BG2X = work->bg2x;
  REG_BG2Y = work->bg2y;
}

/// Writes background 2's reference point.
void Display_ApplyBg2Reference(void) {
  /* FAKEMATCH(pin): the work pointer kept in r2, so the registers' address
   * takes r1; as a plain local, 20 of 20 instructions differ in tla-en. */
  register struct BattleFxWork *work asm("r2") =
      (struct BattleFxWork *)Ram_HeapSlots->battleFxWork[0];
  u32 hold;

  /* FAKEMATCH(barrier): an empty asm sets hold, which keeps r1 until the
   * first value is read, so the offset takes r0; without hold and its asm,
   * 8 of 20 instructions differ in tla-en. */
  __asm__ volatile("" : "=l"(hold));
  {
    u32 value = work->bg2x;

    /* FAKEMATCH(barrier): an empty asm through the first value loads it before
     * the register's address is built, and ends hold; as an asm on hold
     * alone, 14 of 20 instructions differ in tla-en. */
    __asm__ volatile("" : "+l"(value) : "l"(hold));
    REG_BG2X = value;
  }
  REG_BG2Y = work->bg2y;
}

void ColorBuffer_ScaleFar(void *source, void *destination, s32 scale,
                          s32 count);

void Palette_StepFadeTransfer(void) {
  u8 *work = Ram_HeapSlots->battleFxWork[0];
  s32 *framesLeft;
  u8 *battle;

  framesLeft = (s32 *)(work + 0x77b4);
  battle = (u8 *)Ram_HeapSlots->battleWork;

  if (*framesLeft > 0) {
    s32 *step = (s32 *)(work + 0x77b8);

    (*step)++;
    ColorBuffer_ScaleFar(battle + 0x56c, (void *)&BG_PLTT_COLOR(6, 0),
                         0x10000 - *step * 1092, 128);
    (*framesLeft)--;
  }
}

/// Clears every target's flash timer.
void BattleEffect_ClearActorTimers(void) {
  Iwram_ClearWords(Ram_HeapSlots->battleFxWork[0] + 0x78dc, 8);
}

/// Counts down each target's flash timer; when one runs out, the target gets
/// its own palette back.
void BattleEffect_CountDownBattlerTints(void) {
  u8 *base;
  s32 i;
  s32 index;

  base = Ram_HeapSlots->battleFxWork[0];
  i = 0;
  do {
    if (base[0x78dc + i] != 0) {
      if ((base[0x78dc + i] = base[0x78dc + i] - 1) == 0) {
        index = i * 2 + 36;
        Func_0814cd48(*(s16 *)(*(u8 **)(base + 0x78f0) + index), 0, -1, -1, 0);
      }
    }
    i += 1;
  } while (i != 8);
}

void Func_0814355c(void) {
  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BLDALPHA = BLEND_ALPHA(14, 16);
  REG_BG2X = 0;
  REG_BG2Y = 0xfffff000;
  REG_BG2PA = Q_8_8(0.5);
  REG_BG2PB = 0;
  REG_BG2PC = 0;
  REG_BG2PD = Q_8_8(1);
  REG_WIN0H = WIN_RANGE(0, 240);
  REG_WIN0V = WIN_RANGE(16, 136);
  REG_WIN1H = WIN_RANGE(0, 240);
  REG_WIN1V = WIN_RANGE(16, 136);
  REG_WINOUT = WINOUT_WINOBJ_ALL | WINOUT_WIN01_CLR | WINOUT_WIN01_BG0;
}

#undef HI

#include "system.h"
#include "io_write_queue.h"
#include "battle_work.h"

static __inline__ s32 ClearWords_(void *destination, s32 size) {
  return Iwram_ClearWords(destination, size);
}

void BattleEffect_ConfigureEffectDisplay(void);
void BattleStaging_ConfigurePaletteFadeFar(s32 mode, s32 background, s32 value);
void BattleCamera_SetViewFar(s32 a, s32 b, s32 c, s32 d);

// Twin: The Broken Seal's BattleEffect_StartCanvasLayer. The Lost Age's
// changes: BattleEffect_ConfigureEffectDisplay for the display set-up, DISPCNT
// 0x1741, Palette_StepFadeTransfer at 0x480, Func_0814355c for the display
// registers and one more work word cleared.
void BattleEffect_StartCanvasLayer(s32 bgControl) {
  struct CanvasWork *work = (struct CanvasWork *)Ram_HeapSlots->battleFxWork[0];
  struct BattleWork *battle = Ram_HeapSlots->battleWork;
  void *canvas = Ram_HeapSlots->battleFxWork[1];
  struct BattleBackgroundView *display =
      (struct BattleBackgroundView *)Ram_HeapSlots->transitionWork;
  s32 row;
  s32 col;
  s32 offset;

  BattleEffect_ConfigureEffectDisplay();
  BattleEffect_ClearActorTimers();
  display->secondMode = 1;
  wait(1);
  REG_BLDCNT = 0;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_OBJ_ON | DISPCNT_BG2_ON | DISPCNT_BG1_ON |
                          DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP | DISPCNT_MODE_1,
                      IO_WRITE_16);
  gBgScroll[1].y = 32;
  wait(1);

  BattleStaging_ConfigurePaletteFadeFar(1, *(u16 *)battle->unk670, 0);
  work->fadeFrames = 0;
  work->fadeStep = 0;
  Task_Add(Palette_StepFadeTransfer, TASK_PASS_VBLANK);

  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_WIN1_ON | DISPCNT_WIN0_ON | DISPCNT_OBJ_ON |
                          DISPCNT_BG1_ON | DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP |
                          DISPCNT_MODE_1,
                      IO_WRITE_16);
  wait(1);
  REG_BG2CNT =
      bgControl | BGCNT_SCREEN_BASE(7) | BGCNT_256_COLORS | BGCNT_CHAR_BASE(1);
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_WIN1_ON | DISPCNT_WIN0_ON | DISPCNT_OBJ_ON |
                          DISPCNT_BG1_ON | DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP |
                          DISPCNT_MODE_1,
                      IO_WRITE_16);
  BattleCamera_SetViewFar(0, 0, 0, 100);
  display->secondMode = 0;
  wait(1);

  Func_0814355c();
  REG_WININ = WININ_WIN1_CLR | WININ_WIN1_OBJ | WININ_WIN1_BG2 |
              WININ_WIN1_BG0 | WININ_WIN0_CLR | WININ_WIN0_OBJ |
              WININ_WIN0_BG2 | WININ_WIN0_BG1 | WININ_WIN0_BG0;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_WIN1_ON | DISPCNT_WIN0_ON | DISPCNT_OBJ_ON |
                          DISPCNT_BG2_ON | DISPCNT_BG1_ON | DISPCNT_BG0_ON |
                          DISPCNT_OBJ_1D_MAP | DISPCNT_MODE_1,
                      IO_WRITE_16);

  offset = 0;
  for (row = 0; row != 16; row++) {
    for (col = 0; col != 8; col++, offset += 2) {
      s32 chr;
      s16 pair;

      chr = row * 16 + col * 2;
      pair = ((chr + 1) << 8) | chr;
      *(volatile s16 *)((u8 *)BG_CANVAS_MAP + offset) = pair;
    }
  }

  ClearWords_(canvas, 0x4000);
  ClearWords_(BG_CHAR_BLOCK(1), 0x4000);
  work->shakeFrames = 0;
  work->transferPending = 0;
  work->savedBg1X = gBgScroll[1].x;
  work->savedBg1Y = gBgScroll[1].y;
  wait(1);
}

void BattleBackground_SetPaletteLevelFar(s32 background, s32 level);

/// Opens the effect canvas layer: BG2 becomes a 2x-wide 128x128 canvas of
/// 16x16 characters, blended over the battle background, whose palette
/// starts fading.
void BattleEffect_OpenCanvasLayer(s32 bgControl) {
  struct CanvasWork *work = (struct CanvasWork *)Ram_HeapSlots->battleFxWork[0];
  struct BattleWork *battle = Ram_HeapSlots->battleWork;
  void *canvas = Ram_HeapSlots->battleFxWork[1];
  struct BattleBackgroundView *display =
      (struct BattleBackgroundView *)Ram_HeapSlots->transitionWork;
  s32 row;
  s32 col;
  s32 offset;

  BattleEffect_ClearActorTimers();
  display->secondMode = 1;
  wait(1);
  REG_BLDCNT = 0;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_OBJ_ON | DISPCNT_BG2_ON | DISPCNT_BG1_ON |
                          DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP | DISPCNT_MODE_1,
                      IO_WRITE_16);
  gBgScroll[1].y = 32;
  wait(1);
  BattleStaging_ConfigurePaletteFadeFar(1, *(u16 *)battle->unk670, 128);
  work->fadeFrames = 24;
  work->fadeStep = 0;
  Task_Add(Palette_StepFadeTransfer, TASK_PASS_VBLANK);
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_OBJ_ON | DISPCNT_BG1_ON | DISPCNT_BG0_ON |
                          DISPCNT_OBJ_1D_MAP | DISPCNT_MODE_1,
                      IO_WRITE_16);
  wait(1);
  REG_BG2CNT =
      bgControl | BGCNT_SCREEN_BASE(7) | BGCNT_256_COLORS | BGCNT_CHAR_BASE(1);
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_OBJ_ON | DISPCNT_BG1_ON | DISPCNT_BG0_ON |
                          DISPCNT_OBJ_1D_MAP | DISPCNT_MODE_1,
                      IO_WRITE_16);
  BattleCamera_SetViewFar(0, 0, 0, 100);
  display->secondMode = 0;
  wait(1);

  Func_0814355c();
  REG_WININ = WININ_WIN1_CLR | WININ_WIN1_OBJ | WININ_WIN1_BG2 |
              WININ_WIN1_BG0 | WININ_WIN0_CLR | WININ_WIN0_OBJ |
              WININ_WIN0_BG2 | WININ_WIN0_BG1 | WININ_WIN0_BG0;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_WIN1_ON | DISPCNT_WIN0_ON | DISPCNT_OBJ_ON |
                          DISPCNT_BG2_ON | DISPCNT_BG1_ON | DISPCNT_BG0_ON |
                          DISPCNT_OBJ_1D_MAP | DISPCNT_MODE_1,
                      IO_WRITE_16);
  wait(1);

  // The canvas map: 16 rows of 8 character pairs.
  offset = 0;
  for (row = 0; row != 16; row++) {
    for (col = 0; col != 8; col++, offset += 2) {
      s32 chr;
      s16 pair;

      chr = row * 16 + col * 2;
      pair = ((chr + 1) << 8) | chr;
      *(volatile s16 *)((u8 *)BG_CANVAS_MAP + offset) = pair;
    }
  }

  /* FAKEMATCH(wrapper): the clear forwarded through an inline, which keeps each
   * call's own constants and the routine in a callee-saved register; as direct
   * Iwram_ClearWords calls, 24 of 259 instructions of
   * BattleEffect_OpenCanvasLayer differ in tla-en. */
  ClearWords_(canvas, 0x4000);
  ClearWords_(BG_CHAR_BLOCK(1), 0x4000);
  work->shakeFrames = 0;
  work->transferPending = 0;
  wait(1);
}

void BattleEffect_StartTiledCanvas(s32 bg_control) {
  s32 row;
  s32 col;
  s32 offset;

  BattleEffect_StartCanvasLayer(bg_control);
  REG_BG2CNT = bg_control | BGCNT_SIZE(1) | BGCNT_SCREEN_BASE(7) |
               BGCNT_256_COLORS | BGCNT_CHAR_BASE(1);
  offset = 0;
  for (row = 0; row != 16; row++) {
    for (col = 0; col != 8; col++, offset += 2) {
      s32 tile;
      s16 pair;

      tile = row * 16 + col * 2;
      pair = ((tile + 1) << 8) | tile;
      *(volatile s16 *)(0x06003800 + offset) = pair;
    }
    for (col = 0; col != 8; col++, offset += 2) {
      *(volatile u16 *)(0x06003800 + offset) = 0;
    }
  }
}

void BattleEffect_StartTiledCanvasFilled(s32 bg_control) {
  s32 row;
  s32 col;
  s32 offset;

  BattleEffect_StartCanvasLayer(bg_control);
  REG_BG2CNT = bg_control | BGCNT_SIZE(1) | BGCNT_SCREEN_BASE(7) |
               BGCNT_256_COLORS | BGCNT_CHAR_BASE(1);
  offset = 0;
  for (row = 0; row != 16; row++) {
    for (col = 0; col != 8; col++, offset += 2) {
      s32 tile;
      s16 pair;

      tile = row * 16 + col * 2;
      pair = ((tile + 1) << 8) | tile;
      *(volatile s16 *)(0x06003800 + offset) = pair;
    }
    for (col = 0; col != 8; col++, offset += 2) {
      *(volatile u16 *)(0x06003800 + offset) = 0xf0f;
    }
  }
}

/// Closes the effect canvas layer: BG1 back where it was, BG2 and its
/// characters cleared, then the background palette faded back in.
void BattleEffect_EndCanvasLayer(void) {
  struct CanvasWork *work = (struct CanvasWork *)Ram_HeapSlots->battleFxWork[0];
  struct BattleWork *battle = Ram_HeapSlots->battleWork;
  s32 i;

  Sound_PlayFar(390);
  gBgScroll[1].x = work->savedBg1X;
  gBgScroll[1].y = work->savedBg1Y;
  gCameraSceneParameters[3] = 120;
  gCameraSceneParameters[4] = 120;
  REG_BG2CNT = BGCNT_SCREEN_BASE(7) | BGCNT_256_COLORS | BGCNT_CHAR_BASE(1) |
               BGCNT_PRIORITY(3);
  Iwram_ClearWords(BG_CHAR_BLOCK(1), 0x4000);
  Task_Remove(Palette_StepFadeTransfer);
  gBgScroll[1].y = 32;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_WIN1_ON | DISPCNT_WIN0_ON | DISPCNT_OBJ_ON |
                          DISPCNT_BG1_ON | DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP |
                          DISPCNT_MODE_1,
                      IO_WRITE_16);
  REG_BLDCNT = 0;
  wait(1);

  BattleStaging_ConfigurePaletteFadeFar(2, *(u16 *)battle->unk670, 7);
  wait(1);
  for (i = 0; i != 8; i++) {
    BattleBackground_SetPaletteLevelFar(*(u16 *)battle->unk670, 21 - i * 3);
    wait(1);
  }

  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_WIN1_ON | DISPCNT_WIN0_ON | DISPCNT_OBJ_ON |
                          DISPCNT_BG2_ON | DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP |
                          DISPCNT_MODE_1,
                      IO_WRITE_16);
  wait(1);
}

/// Hides the canvas layer: marks the background view, queues DISPCNT 0x1541,
/// fades the background palette back and queues BG1's control word.
void BattleEffect_HideBg1AndRestoreBackdrop(void) {
  struct BattleWork *battle;
  s32 *flag;

  flag = &((struct BattleBackgroundView *)Ram_HeapSlots->transitionWork)
              ->secondMode;
  battle = Ram_HeapSlots->battleWork;
  *flag = 1;
  QueueIoWriteDelay2((u32)&REG_DISPCNT,
                     DISPCNT_OBJ_ON | DISPCNT_BG2_ON | DISPCNT_BG0_ON |
                         DISPCNT_OBJ_1D_MAP | DISPCNT_MODE_1);
  wait(1);
  BattleStaging_ConfigurePaletteFadeFar(2, *(u16 *)battle->unk670, 0);
  IO_WRITE_QUEUE_PUSH((u32)&REG_BG1CNT,
                      BGCNT_SCREEN_BASE(31) | BGCNT_256_COLORS |
                          BGCNT_PRIORITY(3),
                      IO_WRITE_16);
  wait(1);
}

void Func_08143d80(void) {
  void *canvas = Ram_HeapSlots->battleFxWork[1];
  struct BattleWork *screen = Ram_HeapSlots->battleWork;
  struct BattleBackgroundView *layer =
      (struct BattleBackgroundView *)Ram_HeapSlots->transitionWork;
  s32 row;
  s32 col;
  s32 off;

  BattleEffect_ClearActorTimers();
  BattleStaging_ConfigurePaletteFadeFar(2, *(u16 *)screen->unk670, 0);
  gBgScroll[1].y = 32;
  layer->secondMode = 0;
  ClearWords_((void *)0x06003fc0, 64);
  FillWords_((void *)0x0600f900, 0x200, -1);
  off = 0;
  for (row = 0; row != 16; row++) {
    for (col = 0; col != 32; col++, off += 2) {
      if (col > 15)
        *(volatile u16 *)(0x0600fb00 + off) = 0xff;
      else
        *(volatile u16 *)(0x0600fb00 + off) = row * 16 + 0x100 + col;
    }
  }
  REG_DISPCNT = DISPCNT_WIN1_ON | DISPCNT_WIN0_ON | DISPCNT_OBJ_ON |
                DISPCNT_BG2_ON | DISPCNT_BG1_ON | DISPCNT_BG0_ON |
                DISPCNT_OBJ_1D_MAP | DISPCNT_MODE_1;
  REG_BG1CNT = BGCNT_SCREEN_BASE(31) | BGCNT_256_COLORS | BGCNT_PRIORITY(1);
  REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_WIN0H = WIN_RANGE(0, 240);
  REG_WIN0V = WIN_RANGE(16, 136);
  REG_WIN1H = WIN_RANGE(0, 240);
  REG_WIN1V = WIN_RANGE(16, 136);
  REG_WININ = WININ_WIN1_CLR | WININ_WIN1_OBJ | WININ_WIN1_BG2 |
              WININ_WIN1_BG0 | WININ_WIN0_CLR | WININ_WIN0_OBJ |
              WININ_WIN0_BG2 | WININ_WIN0_BG1 | WININ_WIN0_BG0;
  REG_WINOUT = WINOUT_WINOBJ_ALL | WINOUT_WIN01_CLR | WINOUT_WIN01_BG0;
  REG_BLDALPHA = BLEND_ALPHA(14, 16);
  ClearWords_(canvas, 0x4000);
  ClearWords_(BG_CHAR_BLOCK(1), 0x4000);
  wait(1);
}

#define ABS(v) ((v) < 0 ? -(v) : (v))

/// The Broken Seal's BattleEffect_DrawClippedCanvasLine, with the fraction in
/// bit 31 (started at 0x80 << 23, carried when negative and cleared with a
/// shift pair) and dx and dy reused as the step direction.
void Func_08143eb4(s32 x0, s32 y0, s32 x1, s32 y1, s32 color) {
  s32 dx = x1 - x0;
  s32 dy = y1 - y0;
  s32 frac = 0x80 << 23;
  u8 *canvas = Ram_HeapSlots->battleFxWork[1];
  s32 step;
  s32 i;
  s32 x;
  s32 y;
  s32 tmp;
  u32 offset;

  if (y0 < 0)
    y0 = 0;
  if (y0 > 127)
    y0 = 127;
  if (y1 < 0)
    y1 = 0;
  if (y1 > 127)
    y1 = 127;

  if (ABS(dx) < ABS(dy)) {
    if (dy < 0) {
      tmp = x0;
      x0 = x1;
      x1 = tmp;
      tmp = y0;
      y0 = y1;
      y1 = tmp;
      dx = x1 - x0;
      dy = y1 - y0;
    }
    step = (ABS(x1 - x0) << 8) / ABS(y1 - y0);
    step <<= 23;
    x = x0;
    if (dx < 0)
      dx = -1;
    else
      dx = 1;
    for (i = y0; i != y1; i++) {
      offset = (((((u32)i >> 3) << 4) + ((u32)x >> 3)) << 3) + (i & 7);
      offset = (offset << 3) + (x & 7);
      if (canvas[offset] < color)
        canvas[offset] = color;
      frac += step;
      if (frac < 0) {
        x += dx;
        frac = (u32)(frac << 1) >> 1;
      }
    }
  } else {
    if (dx < 0) {
      tmp = x0;
      x0 = x1;
      x1 = tmp;
      tmp = y0;
      y0 = y1;
      y1 = tmp;
      dx = x1 - x0;
      dy = y1 - y0;
    }
    step = (ABS(y1 - y0) << 8) / ABS(x1 - x0);
    step <<= 23;
    y = y0;
    if (dy < 0)
      dy = -1;
    else
      dy = 1;
    for (i = x0; i != x1; i++) {
      offset = (((((u32)y >> 3) << 4) + ((u32)i >> 3)) << 3) + (y & 7);
      offset = (offset << 3) + (i & 7);
      if (canvas[offset] < color)
        canvas[offset] = color;
      frac += step;
      if (frac < 0) {
        y += dy;
        frac = (u32)(frac << 1) >> 1;
      }
    }
  }
}

#include "lib.h"
#include "sound.h"

// The skulls live in the map buffer, 64 for each target.
#define PARTICLES ((struct EffectStep *)Ram_MapCellBuffer)

// The canvas routines every effect shares, with their Broken Seal names.
void BattleEffect_CountDownBattlerTints(void);
void Func_081180e8(s32 phase); // BattlePlayback_StartFar
void Func_0814cd48(s32 actor, s32 palette, s32 duration, s32 timer,
                   s32 frames); // BattleEffect_TintBattler
void BattleEffect_FetchRectangleBlitters(
    s32 flip, void **out); // ..FetchRectangleBlitters
void SceneTransform_ApplyPosition(const s32 *pos);

extern char ResourceId_SkullSheet;
extern char ResourceId_PinkBurstSheet;

#define SKULLS 8 // per target
#define SKULL_SIZE 24
#define SKULL_CELL (24 * 24)

// Closing skulls: for each target in turn, 32 frames apart, eight skulls
// appear one every four frames from all around, up to 64 pixels away, and
// close in on a point 20 pixels above the target while the view of them
// turns; the background waves line by line throughout.
void BattleEffect_RunHaunt(struct BattleEffectArgument *effect) {
  s32 k;
  struct EffectPosition pos;
  s32 point[3];
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  DrawRectangle draw[2];
  s32 *camera;
  s32 i;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];

  // Set up: the skull sheet with its palette, the pink burst palette, the
  // max and additive blitters, the wave's HBlank DMA and a transfer that
  // fades the canvas by 4 each frame.
  BattleEffect_StartCanvasLayer(0);
  Resource_LoadAndDecompress((s32)&ResourceId_SkullSheet, work->sheet, 1, 1);
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_PinkBurstSheet), 128);
  BattleEffect_FetchRectangleBlitters(0, draw);
  Task_Add(BattleEffect_ArmBg2AffineHBlankDma, 0xc80);
  work->transferMode = 3;
  work->transferValue = 0x04040404;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  // Scatter every skull up to 64 pixels from its centre.
  for (i = 0; i != 512; i++) {
    PARTICLES[i].x = ((Random16() & 255) - 127) << 15;
    PARTICLES[i].y = ((Random16() & 255) - 127) << 15;
    PARTICLES[i].z = ((Random16() & 255) - 127) << 15;
  }
  Sound_PlayFar(142);

  for (frame = 0; frame != effect->count * 32 + 64; frame++) {
    s32 *row;

    camera = Ram_HeapSlots->cameraWork;
    if (frame == 92)
      Func_081180e8(0);

    // The background waves sideways, one sine step per line.
    row = work->bg2xLines;
    if (effect->side == 0) {
      for (i = 0; i != 160; i++)
        *row++ = (0x60000 - Trig_Sin((frame + i) << 11) * 6) >> 10;
    } else {
      for (i = 0; i != 160; i++)
        *row++ = Trig_Sin((frame + i) << 11) * 6 >> 10;
    }

    for (k = 0; k != effect->count; k++) {
      struct MotionObject *target =
          GetBattleObjectSlotFar(effect->actors[k])->object;

      // Each target's skulls turn about a point 20 pixels above it.
      Render_ResetTransformState();
      // The look-at call in a once-only block, so the eye loads before the
      // point it looks at; as the plain call, 2 of 355 instructions differ in
      // tla-en.
      do {
        Transform_SetLookAt(camera, camera + 3);
      } while (0);
      point[0] = target->x;
      point[1] = 20 << 16;
      point[2] = target->z;
      SceneTransform_ApplyPosition(point);
      if (frame > k * 32) {
        SceneTransform_ApplyPitch(frame << 9);
        if (frame == k * 32 + 32)
          Func_0814cd48(effect->actors[k], 7, 5, k, 32);

        // A skull moves one part in its distance toward the centre each
        // frame, and is gone once it arrives.
        for (i = 0; i != SKULLS; i++) {
          struct EffectStep *skull = &PARTICLES[k * 64 + i];

          if (frame > (k * 8 + i) * 4) {
            s32 x = (skull->x >> 8) * (skull->x >> 8);
            s32 y = (skull->y >> 8) * (skull->y >> 8);
            s32 z = (skull->z >> 8) * (skull->z >> 8);
            s32 dist = Iwram_Sqrt(x + y + z) >> 8;

            if (dist != 0) {
              EffectPosition_ProjectPoint((s32 *)skull, &pos);
              pos.x >>= 1;
              draw[0](canvas, work->sheet + i % 3 * SKULL_CELL,
                      pos.x - SKULL_SIZE / 2, pos.y - SKULL_SIZE / 2,
                      SKULL_SIZE, SKULL_SIZE);
              skull->x -= skull->x / dist;
              skull->y -= skull->y / dist;
              skull->z -= skull->z / dist;
              skull->variant++;
            }
          }
        }
      }
    }
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  Task_Remove(BattleEffect_ArmBg2AffineHBlankDma);
  HeapSlot_Release(188);
  HeapSlot_Release(104);
  BattleEffect_EndCanvasLayer();
}

// Heap blocks are named by their byte offset in Ram_HeapSlots.
#define HEAP_OFFSET(field) ((s32)&((struct HeapSlots *)0)->field)
#define HEAP_BLITTER HEAP_OFFSET(unk64[1])
#define HEAP_BLITTER_ALTERNATE HEAP_OFFSET(unkb8[1])

// The blitter generator's modes, The Broken Seal's in reverse order.

s32 BattleEffect_LoadBlitter(s32 slot,
                             s32 modeAndFlags); // BattleEffect_BuildBlitter

extern char ResourceId_SpiralSheet;

#define SPIRAL_SIZE 32
#define SPIRAL_CELS 4

// Member orbit: over each target in turn, sixteen frames apart, four
// spirals circle a point 40 pixels above it for 60 frames, the blitter
// flipping every sixteen frames; the background waves line by line.
void BattleEffect_RunOrbitingSpirals(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle draw[2];
  s32 point[3];
  struct EffectPosition screen;
  s32 member;
  s32 offsetX;
  s32 frame;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];

  // Set up: the spiral sheet with its palette, the two blitters, the wave's
  // HBlank DMA and the transfer at half strength.
  BattleEffect_StartCanvasLayer(0);
  REG_BG2PA = 0x100;
  Resource_LoadAndDecompress((s32)&ResourceId_SpiralSheet, work->sheet, 1, 1);
  BattleEffect_LoadBlitter(HEAP_BLITTER, BLITTER(BLIT_MAX, CLIP));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                           BLITTER(BLIT_MAX, CLIP | COMPOSITE_FLIP));
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
  Task_Add(BattleEffect_ArmBg2AffineHBlankDma, 0xc80);
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  // On the far side the background and the spirals move 112 pixels left.
  if (effect->side == 1) {
    REG_BG2X = -0x6800;
    offsetX = -112;
  } else {
    offsetX = 0;
  }

  for (frame = 0; frame != (effect->count * 16) + 48; frame++) {
    s32 *camera;
    s32 *row;
    s32 i;

    // The background waves sideways line by line.
    camera = Ram_HeapSlots->cameraWork;
    row = work->bg2xLines;
    if (effect->side == 0) {
      s32 angle;
      s32 base;

      for (i = 0, base = 0x80000, angle = frame << 10; i != 160; i++) {
        *row++ = (base - (Trig_Sin(angle) << 3)) >> 10;
        angle += 1024;
      }
    } else {
      s32 angle;

      for (i = 0, angle = frame << 10; i != 160; i++) {
        *row++ = ((Trig_Sin(angle) << 3) >> 10) - 0x7000;
        angle += 1024;
      }
    }

    Render_ResetTransformState();
    Transform_SetLookAt(camera, camera + 3);
    member = 0;
    if (effect->count != 0) {
      while (member != effect->count) {
        struct MotionObject *target;

        target = GetBattleObjectSlotFar(effect->actors[member])->object;
        if (frame > member * 16 && frame < (member * 16) + 60) {
          s32 spin;

          // Halfway through, the target's flash stops.
          if (frame == (member * 16) + 32)
            Func_0814cd48(effect->actors[member], 0, 5, -1, 0);
          point[0] = target->x;
          point[1] = 40 << 16;
          point[2] = target->z;
          EffectPosition_ProjectPoint(point, &screen);

          // The four spirals, a quarter turn apart, each a 32 x 32 cel of
          // the sheet's four-frame animation.
          for (i = 0; i != SPIRAL_CELS; i++) {
            s32 x;
            s32 y;
            s32 half;

            spin = (frame + i * 32) << 9;
            x = (screen.x + ((Trig_Sin(spin) << 4) >> 16)) + offsetX;
            y = screen.y + ((Trig_Cos(spin) << 4) >> 16);
            half = frame / 16;
            draw[half & 1](canvas,
                           work->sheet + (((frame / 4) - (half * 4)) << 10),
                           x - SPIRAL_SIZE / 2, y - SPIRAL_SIZE / 2,
                           SPIRAL_SIZE, SPIRAL_SIZE);
          }
        }
        member++;
      }
    }
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  Task_Remove(BattleEffect_ArmBg2AffineHBlankDma);
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}

#include "battle_effect_argument.h"

void Func_08144628(struct BattleEffectArgument *effect, s32 mode);

/// Abilities: Demon Night (kind 50).
DEFINE_EFFECT_KIND(BattleEffect_RunDemonNight, Func_08144628, 0)

/// Abilities: Rumble (kind 308).
DEFINE_EFFECT_KIND(BattleEffect_RunRumble, Func_08144628, 1)

/// Abilities: Haunting (kind 351).
DEFINE_EFFECT_KIND(BattleEffect_RunHaunting, Func_08144628, 2)

/// Abilities: Thorny Grave (kind 127).
DEFINE_EFFECT_KIND(BattleEffect_RunThornyGrave, Func_08144628, 3)

void BattleEffect_ArmBg1HBlankDma(void); // arms the BG1 scroll's HBlank DMA
void Func_0815b634(void);                // mode 3 only

extern char ResourceId_MemberBurstImage;

extern char ResourceId_MarsDjinnSheet;
extern char ResourceId_MercuryDjinnSheet;
// Per mode: how many motes each target gets, and the effect's frame count
// before the targets' share.
extern u8 Data_08197864[];

#define MOTES_PER_TARGET 128
#define MOTE_CELL (20 * 32)

// Member burst (kinds through four wrappers; mode picks the colours): from
// the caster's body, 128 motes per target scatter every way, and from 30
// frames on each is pulled toward the middle of its target's body, vanishing
// once it has crossed it. The background sways sideways, swelling and dying
// away over the effect. In mode 3 every mote that arrives also pushes its
// target back.
void Func_08144628(struct BattleEffectArgument *effect, s32 mode) {
  struct EffectPosition pos;
  DrawRectangle draw[2];
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 k;
  s32 *camera;
  s32 palette;
  s32 i;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  camera = Ram_HeapSlots->cameraWork;

  // Set up: the mote picture, the mode's palette, and the max and additive
  // blitters for the caster's side.
  BattleEffect_StartCanvasLayer(1);
  Resource_LoadAndDecompress((s32)&ResourceId_MemberBurstImage, work->sheet, 0,
                             0);
  if (mode == 0 || mode == 3)
    palette = (s32)&ResourceId_PinkBurstSheet;
  else if (mode == 1)
    palette = (s32)&ResourceId_MarsDjinnSheet;
  else
    palette = (s32)&ResourceId_MercuryDjinnSheet;
  // The entry held in a local loads before the arguments; as a plain
  // Iwram_CopyWords call, 2 of 511 instructions differ in tla-en.
  {
    const void *source_ = Resource_GetTableEntry(palette);
    __typeof__(Iwram_CopyWords) routine = Iwram_CopyWords;
    routine((void *)BG_PLTT, source_, 128);
  }
  if (mode == 3)
    Func_0815b634();
  BattleEffect_FetchRectangleBlitters(effect->side, draw);

  // The motes start at the caster's body height and scatter every way at up
  // to two pixels a frame.
  for (i = 0; i != 1024; i++)
    PARTICLES[i].variant = -1;
  for (k = 0; k != effect->count; k++) {
    struct MotionObject *source = GetBattleObjectSlotFar(effect->actor)->object;
    s32 height = BattleUnit_GetEffectHeightFar(effect->actor);

    for (i = 0; i != MOTES_PER_TARGET; i++) {
      struct EffectStep *mote = &PARTICLES[k * 128 + i];

      mote->x = source->x;
      mote->y = height;
      mote->z = source->z;
      mote->velocityX = ((Random16() & 255) - 128) << 10;
      mote->velocityY = ((Random16() & 255) - 128) << 10;
      mote->velocityZ = ((Random16() & 255) - 128) << 10;
      mote->variant = 0;
    }
  }

  Task_Add(BattleEffect_ArmBg1HBlankDma, 0xc80);
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  Sound_PlayFar(146);

  for (frame = 0; frame != Data_08197864[mode * 2 + 1] + effect->count * 20;
       frame++) {
    s32 *row;
    s32 sway;
    s32 scrollX;

    if (frame == 80) {
      if (mode == 0)
        Func_081180e8(134);
      else if (mode == 3)
        Func_081180e8(134);
      else
        Func_081180e8(133);
    }
    Render_ResetTransformState();
    Transform_SetLookAt(camera, camera + 3);

    // BG1 sways line by line, by up to 16 pixels at the effect's midpoint.
    // A negative scroll wraps into the halfword one short of 0x10000.
    /* FAKEMATCH(barrier): an empty asm after the look-at call keeps its
     * arguments loaded in order, r0 before r1; without it, 2 of 511
     * instructions differ in tla-en. */
    __asm__("");
    row = work->bg2xLines;
    sway = Trig_Sin((frame << 15) /
                    (Data_08197864[mode * 2 + 1] + effect->count * 20)) *
               16 >>
           16;
    scrollX = gBgScroll[1].x;
    for (i = 0; i != 160; i++) {
      s32 x = (Trig_Sin((frame + i) << 10) * sway >> 16) + scrollX;

      if (x < 0)
        x += 0xffff;
      *row++ = x | 32 << 16;
    }

    for (k = 0; k != effect->count; k++) {
      struct MotionObject *target =
          GetBattleObjectSlotFar(effect->actors[k])->object;
      s32 half = BattleUnit_GetEffectHeightFar(effect->actors[k]) / 2;

      if (frame == k * 20 + 71) {
        if (mode == 0)
          Sound_PlayFar(134);
        else if (mode != 3)
          Sound_PlayFar(133);
      }
      if (frame == k * 20 + 70)
        Func_0814cd48(effect->actors[k], 7, 5, k, 26);
      if (frame > k * 20) {
        for (i = 0; i != Data_08197864[mode * 2]; i++) {
          struct EffectStep *mote = &PARTICLES[k * 128 + i];

          if (frame > (k * 10 + i) * 2 && mote->variant >= 0) {
            EffectPosition_ProjectPoint((s32 *)mote, &pos);
            pos.x >>= 1;
            draw[0](canvas, work->sheet + i % 3 * MOTE_CELL, pos.x - 10,
                    pos.y - 16, 20, 32);
            BattleEffectKernels_IntegrateVector3(mote, 62, 0);

            // Thirty frames on, the mote is pulled toward the middle of its
            // target's body.
            if (frame > k * 20 + i + 30) {
              s32 x = mote->x;
              s32 dx = (target->x - x) >> 8;
              s32 dy = (target->y + half - mote->y) >> 9;
              s32 dz = (target->z - mote->z) >> 8;
              s32 targetDistance;
              s32 moteDistance;
              s32 crossed;

              mote->velocityX += dx;
              mote->velocityY += dy;
              mote->velocityZ += dz;
              // It has crossed once it is as far out from the middle as its
              // target is, on the target's side.
              targetDistance = target->x < 0 ? -target->x : target->x;
              moteDistance = x;
              /* FAKEMATCH(barrier): an empty asm through moteDistance keeps it
               * a copy apart from x, so the sign test and the negation read the
               * copy; as x < 0 ? -x : x, 4 of 511 instructions differ in
               * tla-en. */
              __asm__("" : "=r"(moteDistance) : "0"(moteDistance));
              if (moteDistance < 0)
                moteDistance = -moteDistance;
              /* FAKEMATCH(dead): each arm stores its test in crossed, never
               * read; as the tests without the stores, 81 of 511 instructions
               * differ in tla-en. */
              if (targetDistance <= moteDistance &&
                  ((x < 0 && (crossed = target->x < 0)) ||
                   (x >= 0 && (crossed = target->x >= 0)))) {
                mote->variant = -1;
                if (mode == 3) {
                  BattleEffect_StartActorMotion(effect->actors[k], 1, 0x20000,
                                                0x20000, 0x20000, i * 8 + 100);
                  if ((i & 1) && frame != 80)
                    Sound_PlayFar(134);
                }
              }
            }
          }
        }
      }
    }
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(188);
  HeapSlot_Release(104);
  Task_Remove(BattleEffect_ArmBg1HBlankDma);
  BattleEffect_EndCanvasLayer();
}

/// Builds the two usual routines that draw onto the 128x128 canvas, clipped,
/// mirrored when flip is nonzero. out[0] keeps the higher colour index (heap
/// slot 26), out[1] adds the colours (slot 47). BattleEffect_LoadBlitter takes
/// each slot's heap offset and the blitter flags.
// The Broken Seal's BattleEffect_FetchRectangleBlitters.
void BattleEffect_FetchRectangleBlitters(s32 flip, void **out) {
  // Unused: gives the stack frame its 4 bytes.
  UNUSED u8 unused[4];

  if (flip == 0) {
    u32 heap = 0xc0;
    s32 flags = 19;
    void **slots;

    /* FAKEMATCH(barrier): an empty asm on heap and flags, so both are set
     * before the heap base is shifted and the slot offset built; without it,
     * 2 of 30 instructions differ in tla-en. */
    __asm__("" : "+l"(heap), "+l"(flags));
    slots = (void **)(heap << 18);
    BattleEffect_LoadBlitter(104, flags);
    out[0] = slots[26];
    BattleEffect_LoadBlitter(188, 3);
    out[1] = slots[47];
  } else {
    u32 heap = 0xc0;
    s32 flags = 23;
    void **slots;

    /* FAKEMATCH(barrier): the same asm for the mirrored blitters; without it,
     * 2 of 30 instructions differ in tla-en. */
    __asm__("" : "+l"(heap), "+l"(flags));
    slots = (void **)(heap << 18);
    BattleEffect_LoadBlitter(104, flags);
    out[0] = slots[26];
    BattleEffect_LoadBlitter(188, 7);
    out[1] = slots[47];
  }
}

void Func_08144b14(struct BattleEffectArgument *effect, s32 mode);

/// Abilities: Revive, Water of Life, Lich (kind 25); Quartz (kind 201).
DEFINE_EFFECT_KIND(BattleEffect_RunRevive, Func_08144b14, 0)

/// Abilities: Dew (kind 209).
DEFINE_EFFECT_KIND(BattleEffect_RunDew, Func_08144b14, 1)

/// Abilities: Spark (kind 255).
DEFINE_EFFECT_KIND(BattleEffect_RunSpark, Func_08144b14, 2)

#include "transform.h"

void Func_0815585c(struct BattleEffectArgument *effect, u32 packed, s32 *screen,
                   s32 *world); // BattleEffect_RunDjinniEntrance

extern char ResourceId_EarthWallSheet;
extern char ResourceId_CrescentMoonSheet;
extern char ResourceId_VenusDjinnSheet;

extern u8 Data_0819786c[];  // the mote's picture for each step of its sway
extern u16 Data_08197874[]; // the beam's head: three pictures, offsets
extern u8 Data_0819787a[];  // ... their widths
extern u8 Data_0819787d[];  // ... their heights

// The pictures in the work sheet, after the beam (EarthWallSheet).
#define PIC_MOTE 0x2710  // the crescent motes, 24 x 24
#define PIC_DJINN 0x65c0 // the Djinn, 40 x 40
#define MOTES 8          // per target

// Member beam (kinds through three wrappers; the mode picks the Djinn:
// 0 Venus, 1 Mercury, 2 Mars): with a prelude, the Djinn appears, sways and
// rises away; for each target in turn, sixteen frames apart, eight motes
// drift down swaying; from frame 72 a beam of light falls on each target,
// flashing it, and narrows from frame 88.
void Func_08144b14(struct BattleEffectArgument *effect, s32 mode) {
  s32 djinn[3];
  s32 djinnSize[3];
  struct EffectPosition one;
  struct EffectPosition pos;
  s32 ground[3];
  s32 point[3];
  DrawRectangle draw[2];
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 offsetX;
  s32 i;
  s32 k;
  struct HeapSlots *heap;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  // heap holds the heap slots from here for the second blitter's read, so the
  // 0x03000000 base is built again there; as Ram_HeapSlots at that read, 102
  // of 652 instructions differ in tla-en.
  heap = Ram_HeapSlots;

  // Set up: the canvas for the Djinn when there is a prelude, then the beam,
  // the motes and the mode's Djinn, each with its palette.
  BattleEffect_StartCanvasLayer(0);
  if (effect->prelude == 1)
    Func_0815585c(effect, (effect->side ^ effect->prelude) << 4 | mode, djinn,
                  djinnSize);
  Resource_LoadAndDecompress((s32)&ResourceId_EarthWallSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_CrescentMoonSheet,
                             work->sheet + PIC_MOTE, 1, 1);
  if (mode == 0)
    Resource_LoadAndDecompress((s32)&ResourceId_VenusDjinnSheet,
                               work->sheet + PIC_DJINN, 1, 1);
  else if (mode == 1)
    Resource_LoadAndDecompress((s32)&ResourceId_MercuryDjinnSheet,
                               work->sheet + PIC_DJINN, 1, 1);
  else
    Resource_LoadAndDecompress((s32)&ResourceId_MarsDjinnSheet,
                               work->sheet + PIC_DJINN, 1, 1);

  // The motes start 80 pixels up, each at its own phase of the sway.
  for (i = 0; i != 512; i++) {
    PARTICLES[i].x = 0;
    PARTICLES[i].y = 80 << 16;
    PARTICLES[i].z = (Random16() | -32) << 14;
    PARTICLES[i].variant = Random16() & 255;
  }

  // With one target, BG2 is shifted so that it stands at x = 64.
  REG_BG2PA = 0x100;
  if (effect->count == 1) {
    EffectPosition_ProjectScaledBattler(effect->actors[0], &one);
    offsetX = 64 - one.x;
  } else {
    if (effect->side == 1)
      offsetX = -112;
    else
      offsetX = 0;
  }
  REG_BG2X = offsetX << 8;
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  for (frame = 0; frame != effect->count * 16 + 116; frame++) {
    struct BattleCamera *camera = Ram_HeapSlots->cameraWork;
    s32 prelude;

    if (frame == 64)
      Sound_PlayFar(212);
    if (frame == 80)
      Func_081180e8(0);

    // The Djinn sways 20 pixels either way and rises from frame 32.
    prelude = effect->prelude;
    if (prelude == 1) {
      s32 angle = frame << 11;
      s32 x = (-Trig_Sin(angle) * 20 >> 16) + djinn[0] + offsetX - 20;
      s32 y = (Trig_Cos(angle) * 4 >> 16) + djinn[1] - 24;

      BattleEffect_FetchRectangleBlitters(effect->side ^ prelude, draw);
      if (frame > 32)
        y = y - frame * 2 + 64;
      draw[1](canvas, work->sheet + PIC_DJINN, x, y, 40, 40);
      if (frame <= 3)
        draw[1](canvas, work->sheet + PIC_DJINN, x, y, 40, 40);
      HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
      HeapSlot_Release(HEAP_BLITTER);
    }
    BattleEffect_LoadBlitter(HEAP_BLITTER, BLITTER(BLIT_MAX, CLIP));
    draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
    BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                             BLITTER(BLIT_MAX, CLIP | COMPOSITE_FLIP_X));
    draw[1] = (DrawRectangle)heap->unkb8[1];

    // A mote every eight frames drifts down a pixel a frame, swaying 16
    // pixels, until it is 8 pixels from the ground.
    for (k = 0; k != effect->count; k++) {
      struct MotionObject *target =
          GetBattleObjectSlotFar(effect->actors[k])->object;
      s32 start = k * 16;

      Render_ResetTransformState();
      Transform_SetLookAt(camera->eye, camera->pos);
      point[0] = target->x;
      point[1] = 0;
      point[2] = target->z;
      SceneTransform_ApplyPosition(point);
      if (frame > start) {
        for (i = 0; i != MOTES; i++) {
          struct EffectStep *mote = &PARTICLES[k * 64 + i];

          if (frame > i * 8 + start && mote->y > 8 << 16) {
            s32 sway;

            EffectPosition_ProjectPoint((s32 *)mote, &pos);
            pos.x += offsetX;
            sway = Trig_Sin(mote->variant << 10) * 16 >> 16;
            if (i & 1)
              pos.x -= sway;
            else
              pos.x += sway;
            draw[i & 1](canvas,
                        work->sheet +
                            Data_0819786c[mote->variant / 8 & 7] * 576 +
                            PIC_MOTE,
                        pos.x - 12, pos.y - 12, 24, 24);
            mote->y -= 1 << 16;
            mote->variant++;
          }
        }
      }
    }

    // The beam: from the top of the screen to the target's feet, drawn in
    // two mirrored halves, six pixels wide until frame 88, then narrowing a
    // pixel every three frames; its head flickers at the feet.
    for (i = 0; i != effect->count; i++) {
      s32 start = i * 16;

      if (frame >= start + 72) {
        struct MotionObject *target =
            GetBattleObjectSlotFar(effect->actors[i])->object;

        Render_ResetTransformState();
        Transform_SetLookAt(camera->eye, camera->pos);
        if (frame == start + 72)
          Func_0814cd48(effect->actors[i], 1, -1, -1, 0);
        if (frame == start + 88)
          Func_0814cd48(effect->actors[i], 0, -1, -1, 0);
        ground[0] = target->x;
        ground[1] = 0;
        ground[2] = target->z;
        EffectPosition_ProjectPoint(ground, &pos);
        pos.x += offsetX;
        if (frame < start + 104) {
          s32 pic = frame / 4;
          s32 width = 6;

          if (frame > start + 88)
            width = 6 - (frame - start - 88) / 3;
          if (pic > 2)
            pic = (pic & 1) + 1;
          if (frame < start + 100) {
            draw[0](canvas, work->sheet + Data_08197874[pic],
                    pos.x - Data_0819787a[pic], pos.y - Data_0819787d[pic] + 8,
                    Data_0819787a[pic], Data_0819787d[pic]);
            draw[1](canvas, work->sheet + Data_08197874[pic], pos.x,
                    pos.y - Data_0819787d[pic] + 8, Data_0819787a[pic],
                    Data_0819787d[pic]);
          }
          for (k = 0; k != pos.y; k++) {
            draw[0](canvas, work->sheet + 5, pos.x - width, k, width, 1);
            draw[1](canvas, work->sheet + 5, pos.x, k, width, 1);
          }
        }
      }
    }
    HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
    HeapSlot_Release(HEAP_BLITTER);
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

void Func_0814512c(struct BattleEffectArgument *effect, s32 mode);

/// Abilities: Mad Spatter (kind 347).
DEFINE_EFFECT_KIND(BattleEffect_RunMadSpatter, Func_0814512c, 4)

/// Abilities: Mud (kind 237).
DEFINE_EFFECT_KIND(BattleEffect_RunMud, Func_0814512c, 7)

/// Abilities: Decompose (kind 350).
DEFINE_EFFECT_KIND(BattleEffect_RunDecompose, Func_0814512c, 5)

/// Abilities: Bone Chiller (kind 309).
DEFINE_EFFECT_KIND(BattleEffect_RunBoneChiller, Func_0814512c, 0)

/// Abilities: Sticky Goo (kind 315).
DEFINE_EFFECT_KIND(BattleEffect_RunStickyGoo, Func_0814512c, 1)

/// Abilities: Poison Ink (kind 340).
DEFINE_EFFECT_KIND(BattleEffect_RunPoisonInk, Func_0814512c, 6)

/// Abilities: Sticky Poison (kind 322).
DEFINE_EFFECT_KIND(BattleEffect_RunStickyPoison, Func_0814512c, 2)

/// Abilities: Rotten Blood (kind 330).
DEFINE_EFFECT_KIND(BattleEffect_RunRottenBlood, Func_0814512c, 3)

/// Abilities: Rising Venom (kind 430).
DEFINE_EFFECT_KIND(BattleEffect_RunRisingVenom, Func_0814512c, 8)

/// Abilities: Formic Acid (kind 453).
DEFINE_EFFECT_KIND(BattleEffect_RunFormicAcid, Func_0814512c, 9)

// The shots live in the map buffer, 16 for each target, and the bursts
// after them, 32 for each target.

#define BURSTS ((struct EffectStep *)(Ram_MapCellBuffer + 0x1c00))

void SummonEffect_RunCanvasMode1(void *action);
void SummonEffect_RunCanvasMode3(void *action);

extern char ResourceId_GoldShellSheet;
extern char ResourceId_SmokeSheet;
extern char ResourceId_BluePalette;
extern char ResourceId_SpiderWebSheet;
extern char ResourceId_LimePalette;
extern char ResourceId_GreenPalette;

// Each mode's record: whether its shots burst into shards (else into smoke),
// the shots' blitter, shots per target, and frames before the targets'
// share.
extern s8 Data_08197880[];
#define RECORD_SHARDS 0
#define RECORD_SHOT_BLITTER 1
#define RECORD_SHOTS 2
#define RECORD_FRAMES 3
extern u8 Data_081978a8[];  // shard widths
extern u8 Data_081978ba[];  // shard heights
extern u16 Data_081978cc[]; // shard cell offsets

#define SMOKE_SHEET 0x3e80 // the smoke, after the gold shells
#define SHARD_SHEET 0x1e00 // the shards, among the gold shells

// Shell shot (the body of ten kinds; the mode picks the colours and the
// record above the rest): shots arc from the caster to each target, one or
// four per target eight frames apart, turning as they fly; each bursts where
// it lands into smoke or shards, flashing the target. Mode 7 prepares a
// canvas for its prelude first; modes 3 and 5 hand the canvas to a summon
// canvas effect at the end.
void Func_0814512c(struct BattleEffectArgument *effect, s32 mode) {
  struct EffectPosition position;
  s32 preludeSize[3];
  s32 preludeOrigin[3];
  DrawRectangle draw[2];
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 i;
  s32 k;
  s32 total;
  struct MotionObject *source;
  s32 *camera;
  u8 *palette;
  s32 height;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  camera = Ram_HeapSlots->cameraWork;

  // Set up: the canvas (mode 7 with its prelude), the gold shells and the
  // smoke with their palettes, then the mode's palette.
  if (mode == 7) {
    BattleEffect_StartCanvasLayer(0);
    Func_0815585c(effect, effect->side << 4 | 0x44, preludeOrigin, preludeSize);
    REG_BG2CNT = 0x785;
  } else {
    BattleEffect_StartCanvasLayer(1);
    REG_BLDALPHA = 0x1010;
  }
  Resource_LoadAndDecompress((s32)&ResourceId_GoldShellSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet,
                             work->sheet + SMOKE_SHEET, 1, 1);
  if (mode == 0)
    palette = Resource_GetTableEntry((s32)&ResourceId_BluePalette);
  else if (mode == 1)
    palette = Resource_GetTableEntry((s32)&ResourceId_SpiderWebSheet);
  else if (mode == 2)
    palette = Resource_GetTableEntry((s32)&ResourceId_LimePalette);
  else if (mode == 8)
    palette = Resource_GetTableEntry((s32)&ResourceId_LimePalette);
  else if (mode == 3)
    palette = Resource_GetTableEntry((s32)&ResourceId_GreenPalette);
  else if (mode == 4 || mode == 7)
    palette = Resource_GetTableEntry((s32)&ResourceId_GoldShellSheet);
  else if (mode == 6)
    palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
  else if (mode == 9)
    palette = Resource_GetTableEntry((s32)&ResourceId_PinkBurstSheet);
  else
    palette = Resource_GetTableEntry((s32)&ResourceId_GreenPalette);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);

  // The shots leave the caster at the mode's height, each aimed at its
  // target with a little spread in depth.
  for (i = 0; i != 1024; i++)
    PARTICLES[i].variant = -1;
  source = GetBattleObjectSlotFar(effect->actor)->object;
  if (mode == 7)
    height = 0x280000;
  else if (mode == 8)
    height = 0x140000;
  else
    height = 0x40000;
  for (k = 0; k != effect->count; k++) {
    struct MotionObject *target =
        GetBattleObjectSlotFar(effect->actors[k % effect->count])->object;

    for (i = 0; i != 16; i++) {
      struct EffectStep *shot = &PARTICLES[k * 16 + i];
      s32 aimZ;

      shot->x = source->x;
      shot->y = height;
      shot->z = source->z;
      aimZ = target->z + (((Random16() & 31) - 16) << 17);
      shot->velocityX = (target->x - source->x) >> 4;
      shot->velocityY = 0x40000;
      shot->velocityZ = (aimZ - source->z) >> 4;
      shot->variant = 0;
    }
  }
  for (i = 0; i != 256; i++)
    BURSTS[i].variant = -1;

  // The blitter pair for the caster's side; modes 4 and 7 clear the canvas
  // each frame and turn blending off, the others fade it to three quarters.
  BattleEffect_FetchRectangleBlitters(effect->side, draw);
  if (mode == 4 || mode == 7) {
    work->transferMode = 1;
    work->transferValue = 0;
    REG_BLDCNT = 0;
  } else {
    work->transferMode = 2;
    work->transferValue = 75;
  }
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  total = Data_08197880[mode * 4 + RECORD_FRAMES] + effect->count * 8;
  Sound_PlayFar(103);

  for (frame = 0; frame != total; frame++) {
    struct EffectStep *burst;
    struct EffectStep *piece;

    Render_ResetTransformState();
    Transform_SetLookAt(camera, camera + 3);

    // The shots: each starts four frames after the last, flies under
    // gravity and turns through ten pictures; at the ground it bursts.
    for (k = 0; k != effect->count; k++) {
      for (i = 0; i != Data_08197880[mode * 4 + RECORD_SHOTS]; i++) {
        s32 start = k * 8 + i * 4;

        if (frame >= start) {
          struct EffectStep *shot = &PARTICLES[k * 16 + i];

          if (frame == start + 17) {
            Func_0814cd48(effect->actors[k % effect->count], 7, 5, k, 16);
            Func_081180e8(133);
          }
          if (shot->variant >= 0) {
            s32 step = (frame - start) / 3;

            if (step > 9)
              step = 9;
            EffectPosition_ProjectPoint((s32 *)shot, &position);
            position.x >>= 1;
            position.y -= 8;
            if (step > 4)
              draw[Data_08197880[mode * 4 + RECORD_SHOT_BLITTER]](
                  canvas, work->sheet + step * 768, position.x - 16,
                  position.y - 12, 32, 24);
            else
              draw[Data_08197880[mode * 4 + RECORD_SHOT_BLITTER]](
                  canvas, work->sheet + step * 768, position.x - 12,
                  position.y - 16, 24, 32);
            if (shot->variant == 0)
              BattleEffectKernels_IntegrateVector3(shot, 63, -0x8000);
            if (shot->y < 0) {
              s32 count;
              s32 n;

              shot->y = 0;
              shot->variant = 1;
              if (Data_08197880[mode * 4 + RECORD_SHARDS] == 0)
                count = 4;
              else
                count = 16;
              for (n = 0; n != count; n++) {
                piece = &BURSTS[(k * 4 + i) * 8 + n];
                piece->x = shot->x;
                piece->y = shot->y;
                piece->z = shot->z;
                if (Data_08197880[mode * 4 + RECORD_SHARDS] == 0) {
                  piece->velocityX = ((Random16() & 63) - 32) << 11;
                  piece->velocityY = 0;
                  piece->velocityZ = ((Random16() & 63) - 32) << 11;
                } else {
                  piece->velocityX = ((Random16() & 63) - 32) << 13;
                  piece->velocityY = ((Random16() & 31) + 32) << 12;
                  piece->velocityZ = ((Random16() & 63) - 32) << 13;
                }
                piece->variant = 0;
              }
            }
          }
        }
      }
    }

    // The bursts: smoke rises slowly and spreads; shards fly up, fall and
    // tumble, facing the way they move. Each lives 45 frames.
    for (i = 0, burst = BURSTS; i != 256; burst++, i++) {
      if ((u32)burst->variant <= 44 && burst->y >= 0) {
        EffectPosition_ProjectPoint((s32 *)burst, &position);
        position.x >>= 1;
        if (Data_08197880[mode * 4 + RECORD_SHARDS] == 0) {
          draw[1](canvas, work->sheet + burst->variant / 8 * 1152 + SMOKE_SHEET,
                  position.x - 12, position.y - 24, 24, 48);
        } else {
          s32 picture = burst->variant / 5;
          s32 flip;

          if (i & 1)
            picture += 9;
          flip = effect->side;
          draw[burst->velocityX > 0 ? flip ^ 1 : flip](
              canvas, work->sheet + Data_081978cc[picture] + SHARD_SHEET,
              position.x - Data_081978a8[picture] / 2,
              position.y - Data_081978ba[picture] / 2, Data_081978a8[picture],
              Data_081978ba[picture]);
        }
        if (Data_08197880[mode * 4 + RECORD_SHARDS] == 0)
          BattleEffectKernels_IntegrateVector3(burst, 62, 0x800);
        else
          BattleEffectKernels_IntegrateVector3(burst, 62, -0x8000);
        burst->variant++;
      }
    }
    BattleCamera_Shake(4, 4);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(188);
  HeapSlot_Release(104);

  // Modes 3 and 5 clear both canvases and go on into a summon canvas effect.
  if (mode == 3 || mode == 5) {
    /* FAKEMATCH(wrapper): both clears through ClearWords_, which builds the
     * size anew for each; as direct Iwram_ClearWords calls, 16 of 718
     * instructions differ in tla-en. */
    ClearWords_((void *)0x06004000, 0x3c00);
    ClearWords_(canvas, 0x3c00);
    Task_Remove(Palette_StepFadeTransfer);
    if (mode == 3)
      SummonEffect_RunCanvasMode1(effect);
    else
      SummonEffect_RunCanvasMode3(effect);
  } else {
    BattleEffect_EndCanvasLayer();
  }
}

void Func_08118088(s32 actor, s32 variant); // ..ApplyVariantMotionFar

extern char ResourceId_CounterRevealSheet;
extern char ResourceId_RedCrescentSheetA;
extern u8 Data_081978f0[]; // each panel's x, by side (7 a side)
extern u8 Data_081978fe[]; // each panel's y

// The seven panels of the counter picture: the first two in the work sheet,
// the rest in the map buffer.
#define PANEL_B 0x15d2
#define PANEL_C 0x3081
#define PANEL_D 0x0000
#define PANEL_E 0x2d80
#define PANEL_F 0x4b00
#define PANEL_G 0x58d2

// Counter reveal (kind 40): a shimmering image circles in, fades out and in
// again twice as it closes on the target, then splits into its panels one
// after another with a white flash at frame 72, when the target is thrown
// and the screen shakes. The background waves while the image fades.
void BattleEffect_RunCondemn(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle draw[2];
  // Unused: the second position gives the stack frame its 72 bytes, as in
  // The Broken Seal's twin (one position: 4 of 549 differ, 2026-11-08).
  struct EffectPosition pos[2];
  s32 curve[2];
  s32 visible;
  s32 column;
  s32 row;
  s32 zero;
  u8 *cells;
  s32 frame;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];

  // The zero is held in r8 through the setup calls, as in The Broken Seal's
  // twin (the literal 0 stored directly: 81 of 549 differ, 2026-11-08).
  zero = 0;

  // Set up: half-and-half blending, the image with its palette, the crescent
  // panels into the map buffer, two transparent blitters (the second
  // flipped), the wave's HBlank DMA and a transfer that clears the canvas.
  BattleEffect_StartTiledCanvas(0x2000);
  REG_BG2PA = 0x100;
  cells = gMapCellBuffer;
  REG_BLDALPHA = 0x1010;
  Resource_LoadAndDecompress((s32)&ResourceId_CounterRevealSheet, work->sheet,
                             1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_RedCrescentSheetA, cells, 1, 0);
  BattleEffect_LoadBlitter(HEAP_BLITTER, BLITTER(BLIT_TRANSPARENT, CLIP));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                           BLITTER(BLIT_TRANSPARENT, CLIP | COMPOSITE_FLIP_X));
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
  Task_Add(BattleEffect_ArmBg2AffineHBlankDma, 0xc80);
  work->transferMode = 1;
  work->transferValue = zero;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  // The image starts off to the side, above the field.
  visible = 1;
  if (effect->side == 1)
    curve[0] = -0x500000;
  else
    curve[0] = 0x700000;
  curve[1] = -0x200000;

  for (frame = 0; frame != 104; frame++) {
    s32 spin;
    s32 screenX;
    s32 screenY;
    s32 amp;
    s32 rowBase;
    s32 angle;
    s32 i;
    s32 *scanline;

    spin = frame << 9;
    screenX = (curve[0] >> 16) + ((Trig_Sin(spin) << 4) >> 16) + 48;
    screenY = (curve[1] >> 16) + ((Trig_Cos(spin) << 2) >> 16) + 16;

    // It jumps closer at frame 24 and onto the target at frame 48, hidden
    // for the frame of each jump.
    if (frame == 24) {
      if (effect->side == 1)
        curve[0] = -0x200000;
      else
        curve[0] = 0x480000;
      curve[1] = 0x180000;
      visible = 0;
    }
    if (frame == 25) {
      REG_BLDALPHA = 0x1010;
      visible = 1;
    }
    if (frame == 48) {
      EffectPosition_ProjectScaledBattler(effect->actors[0], pos);
      if (effect->side == 1)
        curve[0] = (pos[0].x - 128) << 16;
      else
        curve[0] = (pos[0].x - 64) << 16;
      curve[1] = 0;
      visible = 0;
    }
    if (frame == 49) {
      REG_BLDALPHA = 0x1010;
      visible = 1;
    }

    // Before each jump the image fades out over eight frames while the
    // background waves harder.
    scanline = work->bg2xLines;
    amp = 0;
    if (frame <= 23) {
      if (frame > 15) {
        amp = (frame * 2) - 32;
        REG_BLDALPHA = (31 - frame) | 0x1000;
      }
    } else if (frame <= 47) {
      if (frame > 31) {
        amp = (frame * 2) - 64;
        REG_BLDALPHA = (47 - frame) | 0x1000;
      }
    }
    if (amp < 0)
      amp = 0;

    i = 0;
    rowBase = (6 - screenX) << 8;
    angle = frame << 11;
    for (; i != 160; i++) {
      *scanline++ = rowBase - ((Trig_Sin(angle) * amp) >> 10);
      angle += 0x800;
    }

    // The image, then from frame 72 its panels, each shown for two frames.
    if (visible != 0) {
      s32 side = effect->side;

      if (side == 0) {
        column = 0;
        row = 0;
      } else {
        column = 1;
        /* FAKEMATCH(dead): i is 160 here, so i >> 31 folds to 0 and only
         * changes allocation; as row = 0, 477 of 549 instructions differ in
         * tla-en. */
        row = i >> 31;
        /* FAKEMATCH(dead): side = 0, never read, keeps the side's copy from
         * replacing the draws' index, so the test and the draws share one
         * register; without it, 31 of 549 instructions differ in tla-en. */
        side = 0;
      }
      if (frame <= 71) {
        draw[effect->side](canvas, work->sheet, Data_081978f0[column * 7],
                           Data_081978fe[row * 7] + screenY, 57, 98);
      } else {
        if (frame <= 75)
          draw[effect->side](canvas, work->sheet, Data_081978f0[column * 7],
                             Data_081978fe[row * 7] + screenY, 57, 98);
        draw[effect->side](canvas, work->sheet + PANEL_B,
                           Data_081978f0[column * 7 + 1],
                           Data_081978fe[row * 7 + 1] + screenY, 99, 69);
        if ((u32)(frame - 72) <= 1)
          Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
        if ((u32)(frame - 74) <= 1)
          draw[effect->side](canvas, work->sheet + PANEL_C,
                             Data_081978f0[column * 7 + 2],
                             Data_081978fe[row * 7 + 2] + screenY, 128, 91);
        if ((u32)(frame - 76) <= 1)
          draw[effect->side](canvas, gMapCellBuffer + PANEL_D,
                             Data_081978f0[column * 7 + 3],
                             Data_081978fe[row * 7 + 3] + screenY, 128, 91);
        if ((u32)(frame - 78) <= 1)
          draw[effect->side](canvas, gMapCellBuffer + PANEL_E,
                             Data_081978f0[column * 7 + 4],
                             Data_081978fe[row * 7 + 4] + screenY, 128, 59);
        if ((u32)(frame - 80) <= 1)
          draw[effect->side](canvas, gMapCellBuffer + PANEL_F,
                             Data_081978f0[column * 7 + 5],
                             Data_081978fe[row * 7 + 5] + screenY, 122, 29);
        if ((u32)(frame - 82) <= 1)
          draw[effect->side](canvas, gMapCellBuffer + PANEL_G,
                             Data_081978f0[column * 7 + 6],
                             Data_081978fe[row * 7 + 6] + screenY, 76, 25);
      }
    }

    // The strike: a cue at 68; at 72 the target is thrown, the screen
    // shakes and the results begin.
    if (frame == 68)
      Sound_PlayFar(212);
    if (frame == 72) {
      Func_08118088(effect->actors[0], 0);
      work->shakeFrames = 8;
      Func_081180e8(144);
    }
    BattleCamera_Shake(8, 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  Task_Remove(BattleEffect_ArmBg2AffineHBlankDma);
  // The battle's own background (its id leads unk670) comes back.
  BattleBackground_LoadFar(1, *(u16 *)Ram_HeapSlots->battleWork->unk670, 24);
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}

#define PIXEL_BUFFER Ram_MapCellBuffer

/// Turns the background canvas into a blue ramp, row by row from the top, each
/// column starting after its own random delay.
void Graphics_ConvertBackgroundToBlueRamp(void) {
  s32 i, x, y, row, end, speed;
  s32 red, green, blue;
  u16 color;
  u8 *delays, *p, *pixel;
  u16 *palette;
  u32 delayMask;

  palette = (u16 *)0x05000040;
  i = 0;
  do {
    s32 half;

    half = i / 2;
    *palette++ = RGB(half, half, i);
    i++;
  } while (i != 32);
  delays = Ram_HeapSlots->unk64[0];
  end = 0;
  speed = 16;
  /* FAKEMATCH(wrapper): the copy forwarded through CopyWords_, which loads the
   * routine's entry before the arguments, as the native does at all three
   * copies; as direct Iwram_CopyWords calls, 8 of 181 instructions differ in
   * tla-en. */
  CopyWords_(PIXEL_BUFFER, (void *)0x06008000, 0x7800);
  delayMask = 63;
  p = delays;
  do {
    *p++ = Random16() & delayMask;
  } while (p != delays + 256);
  row = 0;
  i = 0;
  do {
    end += speed / 4;
    speed++;
    while (row != end) {
      x = 0;
      do {
        y = row - delays[x];
        if (y >= 0) {
          if (y <= 119) {
            pixel = PIXEL_BUFFER + ((x & 7) + ((x / 8) << 6) + ((y & 7) << 3) +
                                    ((y / 8) << 11));
            color = ((s16 *)0x05000000)[*pixel];
            red = color & 31;
            green = (color >> 5) & 31;
            blue = (color >> 10) & 31;
            if (red < green)
              red = green;
            if (red < blue)
              red = blue;
            *pixel = 63 - red;
          }
        }
        x++;
      } while (x != 256);
      row++;
    }
    CopyWords_((void *)0x06008000, PIXEL_BUFFER, 0x7800);
    wait(1);
    if (end > 248)
      break;
    i++;
  } while (i != 27);
  palette = (u16 *)0x050000c0;
  i = 0;
  do {
    s32 half;

    half = i / 2;
    *palette++ = RGB(half, half, i);
    i++;
  } while (i != 32);
  {
    u8 *buffer = PIXEL_BUFFER;

    i = 0;
    do {
      *buffer++ += 64;
      i++;
    } while (i != 0x7800);
  }
  CopyWords_((void *)0x06008000, PIXEL_BUFFER, 0x7800);
  wait(1);
}

void Func_081496c8(s32 frame, s32 redPhase, s32 greenPhase,
                   s32 bluePhase); // Graphics_UpdatePhasePalette
void Func_080150e4(s32 angle);     // SceneTransform_ApplyRoll
void Func_080151ac(s32 *scale);    // SceneTransform_ApplyScale

extern char ResourceId_RuneSheet;
extern char ResourceId_Data134;
extern char ResourceId_ParticleSpritesD;
extern char ResourceId_JupiterDjinnSheet;
extern s32 Data_0819790c[]; // the triangle's first corner: (0, 24, 0)
extern u16 Data_08197424[]; // flare sizes 1 to 5: cell offsets
#define FLARE_SHEET 0x1000  // the flares, after the rune picture
#define DJINN_SHEET 0x2000  // the Jupiter Djinn, after the flares
#define DJINN_SIZE 40

#define CORNERS 3
#define EDGE_STEPS 24 // flares drawn along each edge

// Spinning triangle (kinds 38 and 218): as BattleEffect_RunTriangleStrike, a
// triangle of flares spins down onto the target and leaves the rune picture
// where it lands; when the effect has a prelude, the Jupiter Djinn also circles
// above its place for the first frames and rises away after frame 32.
void BattleEffect_RunBind(struct BattleEffectArgument *effect) {
  s32 origin[3];
  s32 originSize[3];
  struct EffectPosition anchor;
  s32 vector[3];
  struct EffectPosition screen;
  s32 scale[3];
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  DrawRectangle draw[2];
  s32 member;
  s32 *camera;
  s32 shift;
  u8 *graphics;
  struct MotionObject *motion;
  s32 memberFrame;
  struct EffectStep *point;
  s32 size;
  s32 j;
  s32 k;
  struct HeapSlots *heap;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  graphics = Ram_HeapSlots->unk64[0];
  camera = Ram_HeapSlots->cameraWork;
  // heap holds the heap slots from here for the second blitter's read, so the
  // 0x03000000 base is built again there; as Ram_HeapSlots at that read, 28
  // of 518 instructions differ in tla-en.
  heap = Ram_HeapSlots;

  // Set up: the rune picture with its palette, the dots, the flares, the
  // Djinn with its palette, and a transfer that fades the canvas by 4 each
  // frame.
  BattleEffect_StartCanvasLayer(1);
  if (effect->prelude == 1)
    Func_0815585c(effect, effect->side << 4 | 3, origin, originSize);
  REG_BG2PA = 0x100;
  Resource_LoadAndDecompress((s32)&ResourceId_RuneSheet, work->sheet, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, graphics, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD,
                             work->sheet + FLARE_SHEET, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_JupiterDjinnSheet,
                             work->sheet + DJINN_SHEET, 1, 0);
  work->transferMode = 3;
  work->transferValue = 0x04040404;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  // The background is shifted so that the target stands at x = 64.
  EffectPosition_ProjectScaledBattler(effect->actors[0], &anchor);
  shift = 64 - anchor.x;
  REG_BG2X = shift << 8;
  Sound_PlayFar(142);

  for (frame = 0; frame != effect->count * 20 + 72; frame++) {
    if (frame == 64)
      Func_081180e8(0);
    Func_081496c8(frame, 0xaaab, 0x5555, 0);

    // The prelude: the Djinn circles where the effect was prepared, and
    // rises away after frame 32; its blitters live for this frame only.
    if (effect->prelude == 1) {
      s32 x;
      s32 y;

      x = ((Trig_Sin(frame << 11) * 20) >> 16) + origin[0] + shift - 20;
      y = ((Trig_Cos(frame << 11) << 2) >> 16) + origin[1] - 24;
      BattleEffect_FetchRectangleBlitters(effect->side, draw);
      if (frame > 32)
        y = y - frame * 2 + 64;
      draw[0](canvas, work->sheet + DJINN_SHEET, x, y, DJINN_SIZE, DJINN_SIZE);
      if (frame <= 3)
        draw[1](canvas, work->sheet + DJINN_SHEET, x, y, DJINN_SIZE,
                DJINN_SIZE);
      HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
      HeapSlot_Release(HEAP_BLITTER);
    }

    // A fresh pair of max blitters each frame, the second flipped.
    BattleEffect_LoadBlitter(HEAP_BLITTER, BLITTER(BLIT_MAX, CLIP));
    draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
    BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                             BLITTER(BLIT_MAX, CLIP | COMPOSITE_FLIP_X));
    draw[1] = (DrawRectangle)heap->unkb8[1];
    if (frame > 16 && (frame & 15) == 0)
      work->transferValue += 0x01010101;

    for (member = 0; member != 1; member++) {
      motion = GetBattleObjectSlotFar(effect->actors[member])->object;
      memberFrame = frame - member * 8;
      if (memberFrame >= 0 && memberFrame < 96) {
        Render_ResetTransformState();
        Transform_SetLookAt(camera, camera + 3);
        vector[0] = motion->x;
        vector[1] = motion->y;
        vector[2] = motion->z;
        EffectPosition_ProjectPoint(vector, &screen);
        screen.x = anchor.x + shift;
        screen.y -= 24;

        // The three corners turn and shrink toward the target until frame
        // 64, then hold; flares are drawn along the edges between them.
        if (memberFrame <= 67) {
          for (j = 0; j != CORNERS; j++) {
            point = &work->particles[member * 32 + j];
            Render_ResetTransformState();
            if (memberFrame <= 63) {
              scale[0] = 0x2a000 - (memberFrame * 3 << 9);
              scale[1] = 0x2a000 - (memberFrame * 3 << 9);
              scale[2] = 0x2a000 - (memberFrame * 3 << 9);
              Func_080151ac(scale);
              Func_080150e4((64 - memberFrame) << 9);
              SceneTransform_ApplyYaw((64 - memberFrame) << 9);
            }
            Func_080150e4(j * 0x5555);
            EffectPosition_ProjectPoint(Data_0819790c,
                                        (struct EffectPosition *)vector);
            point->velocityX = vector[0] + screen.x;
            point->velocityY = vector[1] + screen.y + 16;
          }
          for (j = 0; j != CORNERS; j++) {
            struct EffectStep *from = &work->particles[j + member * 32];
            struct EffectStep *to = &work->particles[(j + 1) % 3 + member * 32];

            size = 5 - memberFrame / 16;
            for (k = 0; k != EDGE_STEPS; k++) {
              s32 x =
                  from->velocityX + k * (to->velocityX - from->velocityX) / 24;
              s32 y =
                  from->velocityY + k * (to->velocityY - from->velocityY) / 24;

              draw[0](canvas,
                      work->sheet + Data_08197424[size - 1] + FLARE_SHEET,
                      x - size, y - size, size * 2, size * 2);
            }
          }
        }

        // Landed: the rune picture, its right half mirrored from the left.
        if (memberFrame > 63) {
          draw[0](canvas, work->sheet, screen.x - 24, screen.y - 24, 24, 48);
          draw[1](canvas, work->sheet, screen.x, screen.y - 24, 24, 48);
        }
      }
    }
    HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
    HeapSlot_Release(HEAP_BLITTER);
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

extern s32 Data_08197918[]; // the triangle's first corner: (0, 24, 0)

// Triangle strike (kind 67): a triangle of flares spins down onto the
// target, shrinking, for 64 frames; then the rune picture is stamped in two
// halves where it landed. The caster approaches the target at frame 46, the
// palette pulses throughout and the trail fades a step every 16 frames.
void BattleEffect_RunTriangleStrike(struct BattleEffectArgument *effect) {
  struct EffectPosition anchor;
  s32 vector[3];
  struct EffectPosition screen;
  s32 scale[3];
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  DrawRectangle draw[2];
  s32 member;
  s32 *camera;
  s32 shift;
  u8 *graphics;
  struct MotionObject *object;
  s32 memberFrame;
  struct EffectStep *point;
  s32 size;
  s32 j;
  s32 k;
  struct HeapSlots *heap;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  graphics = Ram_HeapSlots->unk64[0];
  camera = Ram_HeapSlots->cameraWork;
  // heap holds the heap slots from here for the second blitter's read, so the
  // 0x03000000 base is built again there; as Ram_HeapSlots at that read, 32
  // of 428 instructions differ in tla-en.
  heap = Ram_HeapSlots;

  // Set up: the rune picture with its palette, the dots, the flares, and a
  // transfer that fades the canvas by 4 each frame.
  BattleEffect_StartCanvasLayer(0);
  REG_BG2PA = 0x100;
  Resource_LoadAndDecompress((s32)&ResourceId_RuneSheet, work->sheet, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, graphics, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD,
                             work->sheet + FLARE_SHEET, 0, 0);
  work->transferMode = 3;
  work->transferValue = 0x04040404;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  // The background is shifted so that the target stands at x = 64.
  EffectPosition_ProjectScaledBattler(effect->actors[0], &anchor);
  shift = 64 - anchor.x;
  REG_BG2X = shift << 8;
  Sound_PlayFar(142);

  for (frame = 0; frame != effect->count * 20 + 72; frame++) {
    if (frame == 64)
      Func_081180e8(0);
    if (frame == 46)
      BattleMotion_ApproachTargetFar(effect->actor, effect->actors[0], 16, 0);
    Func_081496c8(frame, 0xaaab, 0x5555, 0);

    // A fresh pair of max blitters each frame, the second flipped.
    BattleEffect_LoadBlitter(HEAP_BLITTER, BLITTER(BLIT_MAX, CLIP));
    draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
    BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                             BLITTER(BLIT_MAX, CLIP | COMPOSITE_FLIP_X));
    draw[1] = (DrawRectangle)heap->unkb8[1];
    if (frame > 16 && (frame & 15) == 0)
      work->transferValue += 0x01010101;

    for (member = 0; member != 1; member++) {
      object = GetBattleObjectSlotFar(effect->actors[member])->object;
      memberFrame = frame - member * 8;
      if (memberFrame >= 0 && memberFrame < 96) {
        Render_ResetTransformState();
        Transform_SetLookAt(camera, camera + 3);
        vector[0] = object->x;
        vector[1] = object->y;
        vector[2] = object->z;
        EffectPosition_ProjectPoint(vector, &screen);
        screen.x = anchor.x + shift;
        screen.y -= 24;

        // The three corners turn and shrink toward the target until frame
        // 64, then hold; flares are drawn along the edges between them.
        if (memberFrame <= 67) {
          for (j = 0; j != CORNERS; j++) {
            point = &work->particles[member * 32 + j];
            Render_ResetTransformState();
            if (memberFrame <= 63) {
              scale[0] = 0x2a000 - (memberFrame * 3 << 9);
              scale[1] = 0x2a000 - (memberFrame * 3 << 9);
              scale[2] = 0x2a000 - (memberFrame * 3 << 9);
              Func_080151ac(scale);
              Func_080150e4((64 - memberFrame) << 9);
              SceneTransform_ApplyYaw((64 - memberFrame) << 9);
            }
            Func_080150e4(j * 0x5555);
            EffectPosition_ProjectPoint(Data_08197918,
                                        (struct EffectPosition *)vector);
            point->velocityX = vector[0] + screen.x;
            point->velocityY = vector[1] + screen.y + 16;
          }
          for (j = 0; j != CORNERS; j++) {
            struct EffectStep *from = &work->particles[j + member * 32];
            struct EffectStep *to = &work->particles[(j + 1) % 3 + member * 32];

            size = 5 - memberFrame / 16;
            for (k = 0; k != EDGE_STEPS; k++) {
              s32 x =
                  from->velocityX + k * (to->velocityX - from->velocityX) / 24;
              s32 y =
                  from->velocityY + k * (to->velocityY - from->velocityY) / 24;

              draw[0](canvas,
                      work->sheet + Data_08197424[size - 1] + FLARE_SHEET,
                      x - size, y - size, size * 2, size * 2);
            }
          }
        }

        // Landed: the rune picture, its right half mirrored from the left.
        if (memberFrame > 63) {
          draw[0](canvas, work->sheet, screen.x - 24, screen.y - 24, 24, 48);
          draw[1](canvas, work->sheet, screen.x, screen.y - 24, 24, 48);
        }
      }
    }
    HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
    HeapSlot_Release(HEAP_BLITTER);
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

#include "actor.h"

// The camera's heading, as the camera work holds it.
struct CameraView {
  u8 unk00[0x36];
  u16 yaw;
};

extern s32 gData_08197924[]; // the two ring points, alternating
extern u16 gData_08197410[]; // dot sizes 1 to 10: cell offsets in the sheet

#define BOLT_COUNT 3
#define RING_POINTS 10
#define RING_STEPS 16 // dots drawn between two ring points
#define FAR_DEPTH 399999

#define ACTOR(id) ((struct Actor *)GetBattleObjectSlotFar(id)->object)

// Three rings of light fly one after another from the caster to the target,
// each growing as it spins, while the camera swings round; each ring that
// lands throws the target back, and the last begins the results.
void BattleEffect_RunAstralBlast(struct BattleEffectArgument *effect) {
  struct EffectPosition pos;
  struct EffectPosition base;
  s32 scale[3];
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  DrawRectangle draw[2];
  s32 i;
  u8 *graphics;
  s32 *camera;
  struct Actor *caster;
  struct Actor *source;
  struct Actor *target;
  struct EffectStep *point;
  struct EffectStep *bolt;
  s32 start;
  s32 nearest;
  s32 size;
  s32 j;
  s32 k;
  struct EffectStep *ring;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  graphics = Ram_HeapSlots->unk64[0];
  camera = Ram_HeapSlots->cameraWork;
  caster = ACTOR(effect->actor);

  // Set up: the rune palette, the dot sheet, the caster's casting pose, one
  // max blitter and the transfer.
  BattleEffect_StartCanvasLayer(1);
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_RuneSheet), 128);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, graphics, 0, 0);
  Actor_SetMode(caster, 2);
  Actor_SetAnimationSpeedFar((struct ActorView *)caster, 48);
  BattleEffect_LoadBlitter(
      HEAP_BLITTER, BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  work->transferMode = 2;
  work->transferValue = 75;
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  // The rings leave from above the caster and reach the target in twelve
  // frames; the second and third aim past it.
  source = ACTOR(effect->actor);
  target = ACTOR(effect->actors[0]);
  for (i = 0, point = work->particles; i != BOLT_COUNT; i++) {
    point->x = source->x;
    point->y = source->y + 0x280000;
    point->z = source->z;
    if (i == 0)
      point->velocityX = (target->x - point->x) / 12;
    else
      point->velocityX = (target->x * 2 - point->x) / 12;
    point->velocityY = (target->y - point->y + 0x280000) / 12;
    point->velocityZ = (target->z - point->z) / 12;
    point->variant = 0;
    point++;
  }

  ring = (struct EffectStep *)gMapCellBuffer;
  for (frame = 0; frame != 60; frame++) {
    // The camera turns toward the caster's side, slowing from frame 40.
    if (frame <= 47) {
      struct CameraView *view = (struct CameraView *)camera;
      s32 speed;

      if (frame <= 39)
        speed = 128;
      else
        speed = 0x300 - frame * 16;
      if (effect->side == 0)
        view->yaw -= speed;
      else
        view->yaw += speed;
    }

    // A new ring every twelve frames: it spins and grows from a dot to
    // full size, and is drawn while it is near enough to the camera.
    for (i = 0; i != BOLT_COUNT; i++) {
      start = i * 12;
      if (frame >= start) {
        bolt = &work->particles[i];
        size = (frame - start) / 4 + 2;
        if (size > 10)
          size = 10;
        Render_ResetTransformState();
        Transform_SetLookAt(camera, camera + 3);
        SceneTransform_ApplyPosition(bolt);
        nearest = 0;
        for (j = 0; j != RING_POINTS; j++) {
          s32 depth;

          Transform_Push();
          Func_080150e4((frame - start) << 10);
          SceneTransform_ApplyYaw(0x4000);
          scale[0] = ((frame - start) << 12) + 0x1000;
          if (scale[0] > 0x10000)
            scale[0] = 0x10000;
          scale[1] = scale[0];
          scale[2] = scale[0];
          Func_080151ac(scale);
          Func_080150e4(j * 0x199a);
          depth =
              EffectPosition_ProjectPoint(&gData_08197924[(j & 1) * 3], &pos);
          if (nearest < depth)
            nearest = depth;
          pos.x >>= 1;
          // The first two stores read base, which nothing sets, and are
          // overwritten at once; both games' code keeps them.
          ring[i * 10 + j].velocityX = pos.x + base.x;
          ring[i * 10 + j].velocityY = pos.y + base.y;
          ring[i * 10 + j].velocityX = pos.x;
          ring[i * 10 + j].velocityY = pos.y;
          Transform_Pop();
        }
        if (nearest <= FAR_DEPTH) {
          for (j = 0; j != RING_POINTS; j++) {
            struct EffectStep *from =
                &((struct EffectStep *)gMapCellBuffer)[j + i * 10];
            struct EffectStep *to =
                &((struct EffectStep *)gMapCellBuffer)[(j + 1) % 10 + i * 10];

            for (k = 0; k != RING_STEPS; k++) {
              s32 x =
                  from->velocityX + (to->velocityX - from->velocityX) * k / 16;
              s32 y =
                  from->velocityY + (to->velocityY - from->velocityY) * k / 16;

              draw[0](canvas, graphics + gData_08197410[size - 1], x - size / 2,
                      y - size, size, size * 2);
            }
          }
        }
        bolt->x += bolt->velocityX;
        bolt->y += bolt->velocityY;
        bolt->z += bolt->velocityZ;

        // Ten frames after it starts (plus one per ring) the ring lands: the
        // target is thrown back 40 pixels and up, and flashes; the third ring
        // begins the results instead.
        if (frame == start + i + 10) {
          target->acceleration = 0x20000;
          target->speedLimit = 0x80000;
          target->velocityY = 0x50000;
          target->gravity = 0xab85;
          target->actionFlags = 0;
          Actor_ResetMotion(target);
          if (target->x < 0)
            Actor_SetTargetPosition(target, target->x - 0x280000, 0, target->z);
          else
            Actor_SetTargetPosition(target, target->x + 0x280000, 0, target->z);
          if (i == 2) {
            Func_081180e8(134);
          } else {
            Sound_PlayFar(134);
            Func_0814cd48(effect->actors[0], 7, 5, 0, 8);
          }
          work->shakeFrames = 4;
        }
      }
    }
    BattleCamera_Shake(8, 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}

#undef RING_STEPS

// Shining Star's rings of stars.

extern u8 gData_0819793c[]; // ring radii, alternating: 32, 16

#define STAR_COUNT 8

#define RING_STEPS 12 // dots drawn between two ring points
#define DOT_SIZE 2
#define GROUND_Y 0x1dffff

/// Eight rings of stars fly from the caster to the target, one every eight
/// frames and spinning as they go; each bounces where it meets the ground,
/// shaking the screen and flashing the targets.
void BattleEffect_RunShiningStar(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  // Unused: draw[1] gives the stack frame its 4 bytes.
  DrawRectangle draw[2];
  s32 i;
  u8 *graphics;
  s32 *camera;
  struct MotionObject *source;
  struct MotionObject *target;
  struct EffectStep *point;
  struct EffectStep *star;
  struct EffectPosition pos;
  s32 size;
  s32 j;
  s32 k;
  struct EffectStep *ring;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  graphics = Ram_HeapSlots->unk64[0];
  camera = Ram_HeapSlots->cameraWork;

  // Set up: the rune palette, the dot sheet, one max blitter and the transfer.
  BattleEffect_StartCanvasLayer(1);
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_RuneSheet), 128);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, graphics, 0, 0);
  BattleEffect_LoadBlitter(
      HEAP_BLITTER, BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  // Each star leaves from above the caster on its own course to the target,
  // the later ones after a longer delay (held in variant).
  source = GetBattleObjectSlotFar(effect->actor)->object;
  target = GetBattleObjectSlotFar(effect->actors[0])->object;
  for (i = 0, point = work->particles; i != STAR_COUNT; i++) {
    s32 delay = i * 8;

    point->x = source->x / 2;
    point->y = source->y + 0x780000;
    point->z = source->z;
    point->velocityX =
        (target->x + ((s32)((Random16() & 0x7f) - 64) << 16) - point->x) / 12;
    point->velocityY = (target->y - point->y + 0x140000) / 12;
    point->velocityZ = (target->z - point->z) / 12;
    point->variant = (Random16() & 15) + delay;
    point++;
  }

  size = DOT_SIZE;
  ring = (struct EffectStep *)gMapCellBuffer;
  for (frame = 0; frame != 128; frame++) {
    Func_081496c8(frame, 0xaaab, 0x5555, 0);
    if (frame == 96)
      Func_081180e8(134);
    for (i = 0; i != STAR_COUNT; i++) {
      star = &work->particles[i];
      if (frame >= star->variant) {
        // Project the star and, while it is on screen, lay out its ring of
        // ten points (the velocity fields of the map buffer's steps), then
        // join each point to the next with a row of dots.
        Render_ResetTransformState();
        Transform_SetLookAt(camera, camera + 3);
        EffectPosition_ProjectPoint((s32 *)star, &pos);
        pos.x >>= 1;
        if (pos.x >= -8 && pos.x <= 127) {
          if (pos.y <= 127) {
            if (pos.y >= -8) {
              for (j = 0; j != RING_POINTS; j++) {
                ring[i * 10 + j].velocityX =
                    pos.x +
                    ((Trig_Sin(j * 0x199a - ((frame - star->variant) << 11)) *
                      gData_0819793c[j & 1]) /
                         2 >>
                     16);
                ring[i * 10 + j].velocityY =
                    pos.y -
                    (Trig_Cos(j * 0x199a - ((frame - star->variant) << 11)) *
                         gData_0819793c[j & 1] >>
                     16);
              }
              for (j = 0; j != RING_POINTS; j++) {
                struct EffectStep *from = &ring[j + i * 10];
                struct EffectStep *to = &ring[i * 10 + (j + 1) % 10];

                for (k = 0; k != RING_STEPS; k++) {
                  s32 x = from->velocityX +
                          (to->velocityX - from->velocityX) * k / 12;
                  s32 y = from->velocityY +
                          (to->velocityY - from->velocityY) * k / 12;

                  draw[0](canvas, graphics + gData_08197410[size - 1],
                          x - size / 2, y - size, size, size * 2);
                }
              }
            }
          }
        }

        // At the ground the star bounces up at half its sideways speed.
        if (star->y <= GROUND_Y) {
          star->velocityY = -star->velocityY;
          star->velocityX /= 2;
          star->velocityZ /= 2;
          work->shakeFrames = 4;
          Sound_PlayFar(134);
          for (j = 0; j != effect->count; j++)
            Func_0814cd48(effect->actors[j], 7, 5, j, 8);
        }
        star->x += star->velocityX;
        star->y += star->velocityY;
        star->z += star->velocityZ;
      }
    }
    BattleCamera_Shake(4, 4);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}

// Summon 8 (BattleEffect_RunSceneEffect's case 8). Twin: The Broken Seal's
// BattleEffect_RunProcne, ported by trial 2 and rewritten here. The
// Lost Age's changes: BattleEffect_FetchRectangleBlitters for the pair; the
// whirlwind unpacked with its palette by Resource_LoadAndDecompress into the
// work sheet, the comets into work->sheet + 0x3600; frame / 4 % 3 written
// through Math_UnsignedMulHighAdd(x, 0x55555556, -3) and the depth divisions by
// 36 and 90 through Math_UnsignedMulHigh with their reciprocals (calls the
// source makes: agscc emits __modsi3 and __divsi3 for the operators);
// Iwram_TransformVector at the comet sparks; formation resource 646; the effect
// argument for the actor loops; gCameraSceneParameters declared unsized (with
// the shared u32[3], word 3 folds into the address).

// What fixed the frame and slots: screen.y >> 16 and screen.depth >>= 16 as the
// twin writes them (combine makes the ldrsh off the pointer to screen; a cast
// to s16 * gave screen + 6 its own register and two spills); the twin's
// declaration order; wave and drift in a block opened after the blitter fetch,
// whose spill slots fall below the pointer to draw.

struct FxPartyList;

void BattleEffect_SelectLivingTargets(
    struct FxPartyList *party); // ..SelectLivingTargets
void BattleEffect_SpawnObjects(s32 entryCount, s32 kind,
                               u32 variant); // ..SpawnObjects
void Func_0814cca8(void);                    // BattleEffect_SetupBlendedDisplay

s32 ArcTan2(s32 x, s32 y);
u32 Math_UnsignedMulHigh(u32 value, u32 magic);
s32 Math_UnsignedMulHighAdd(s32 value, u32 magic, s32 multiplier);

struct Point2D {
  s32 x;
  s32 y;
};

struct Scale {
  s32 x;
  s32 y;
};

extern char ResourceId_WhirlwindSheet;

extern char ResourceId_VioletSkyBackdrop;
extern char ResourceId_VioletCometSheet;

extern struct Scale Data_08196df8; // the comets' unit scale
extern u8 Data_0819793e[];         // the nine comets' grid: x
extern u8 Data_08197947[];         // ... and depth
extern u16 Data_08197950[];        // the four comet strikes' frames
extern u8 Data_08197958[];         // a strike's three pictures: widths
extern u8 Data_0819795b[];         // ... heights
extern u8 Data_0819795e[];         // ... raise above the mark
extern u16 Data_08197962[];        // ... cell offsets

// frame / 4 % 3, as the source writes it: the multiply-high routine with the
// reciprocal of 3, where a plain % calls __modsi3.
#define MOD3(v) Math_UnsignedMulHighAdd((v), 0x55555556, -3)

#define WHIRLWIND_CEL (48 * 64) // one of the whirlwind's three frames
#define DUST_SHEET 0x2400       // the dust motes, after the whirlwind
#define COMET_SHEET 0x3600      // the violet comets
#define COMETS 9
#define FORMATION_RESOURCE 646

// Summon 8: a whirlwind rises under the enemies and lifts them, spinning,
// while dust motes swirl up through it (frames 0 to 159; A or B skips to
// 160). Then the sky turns violet and a formation of nine comets wheels
// in; a cloud of sparks gathers on its centre, and from frame 330 four
// comets strike one after another, each with a burst of sparks, throwing
// the enemies down. A or B skips frames 161 to 394.
void BattleEffect_RunProcne(struct BattleEffectArgument *effect) {
  void *canvas;
  void *camera;
  struct BattleEffectWork *work;
  DrawRectangle draw[2];
  s32 frame;
  u8 *graphics;
  struct MotionObject *object;
  struct BattleObj *context;
  struct EffectStep *point;
  struct EffectStep *spark;
  struct EffectStep *mark;
  s32 index;
  s32 slot;
  s32 size;
  s32 span;
  s32 dist;
  s32 alive;
  s32 heading[8];
  s32 radius[8];
  s32 spin[8];
  struct Point2D home[8];
  s32 baseAngle[8];
  s32 vector[3];
  struct EffectPosition screen;
  s32 shake[3];
  s32 place[4];
  struct Scale scale;

  canvas = Ram_HeapSlots->battleFxWork[1];
  camera = Ram_HeapSlots->cameraWork;
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  graphics = Ram_HeapSlots->unk64[0];

  // Set up: the max and additive pair, the whirlwind with its palette, the
  // dots, and the transfer at half strength.
  BattleEffect_StartCanvasLayer(0);
  BattleEffect_FetchRectangleBlitters(0, draw);
  {
    struct Point2D wave;
    struct Point2D drift;
    Resource_LoadAndDecompress((s32)&ResourceId_WhirlwindSheet, work->sheet, 1,
                               1);
    Resource_LoadAndDecompress((s32)&ResourceId_Data134, graphics, 0, 0);
    work->transferMode = 2;
    work->transferValue = 50;
    Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
    wave.x = 0x800000;
    wave.y = 0x280000;
    drift.x = 0;
    drift.y = 0;

    // Each enemy's place, heading, distance and facing, to restore at the
    // end; its bobbing stops.
    for (index = 0; index != effect->count; index++) {
      context = GetBattleObjectSlotFar(effect->actors[index]);
      object = context->object;
      home[index].x = object->x;
      home[index].y = object->z;
      baseAngle[index] = object->angle;
      heading[index] = (u16)ArcTan2(object->x, object->z);
      radius[index] = Iwram_Sqrt((object->x >> 8) * (object->x >> 8) +
                                 (object->z >> 8) * (object->z >> 8)) >>
                      7;
      spin[index] = 0;
      object->verticalMotionStrength = 0;
    }

    // The dust motes start at the whirlwind's foot, each after its own wait.
    index = 0;
    do {
      point = &work->particles[index];
      point->z = 120;
      point->y = 0;
      Random16();
      point->velocityY = 0;
      point->velocityX = 0;
      point->variant = Random16() & 63;
      index++;
    } while (index != 64);

    frame = 0;
    do {
      if ((gInput.repeat & 3) != 0) {
        if (frame <= 159)
          frame = 160;
        else if (frame <= 394)
          frame = 395;
      }

      Render_ResetTransformState();
      Transform_SetLookAt(camera, (s32 *)((u8 *)camera + 12));

      if (frame == 16)
        Sound_PlayFar(141);
      if (frame == 256)
        Sound_PlayFar(140);
      if (frame == 334)
        Sound_PlayFar(212);
      if (frame == 347)
        Sound_PlayFar(212);
      if (frame == 359)
        Sound_PlayFar(212);
      if (frame == 372)
        Sound_PlayFar(212);

      // The enemies, one every 16 frames, spiral in toward the centre and
      // rise, spinning faster; each comet strike knocks them down.
      for (index = 0; index != effect->count; index++) {
        if (frame > index * 16) {
          context = GetBattleObjectSlotFar(effect->actors[index]);
          object = context->object;
          object->x = (radius[index] * Trig_Sin(heading[index])) >> 1;
          object->z = (radius[index] * Trig_Cos(heading[index])) >> 1;

          if (frame <= 159) {
            if (frame > index * 16 + 16)
              spin[index] += 48;
            if (radius[index] <= 31)
              object->y += 0x60000;
            else
              object->y += 0x8000;
            if (object->y > 0x7c0000)
              object->y = 0x7c0000;
          }

          if (frame <= 490) {
            heading[index] += spin[index];
            if (heading[index] > 0x10000)
              heading[index] -= 0x10000;
            object->angle += spin[index] / 4;
          }

          if (frame == 395)
            object->verticalMotionStrength = 0xab85;

          slot = 0;
          do {
            if (frame == Data_08197950[slot]) {
              object->velocityY = 0;
              Func_0814cd48(effect->actors[index], 7, 5, index, 8);
            }
            slot++;
          } while (slot != 4);

          if (radius[index] > 16)
            radius[index] -= 2;
        }
      }

      // The whirlwind: its foot grows from frame 16, its top from 48, and
      // from 160 it stands whole, two cels high.
      if ((u32)(frame - 16) <= 143) {
        size = frame * 2 - 32;
        if (size > 48)
          size = 48;
        draw[0](
            canvas,
            &work->sheet[MOD3(frame / 4) * WHIRLWIND_CEL + (48 - size) * 48],
            32, 112 - size, 48, size);
      }
      if ((u32)(frame - 48) <= 111) {
        size = frame * 2 - 96;
        if (size > 64)
          size = 64;
        draw[0](
            canvas,
            &work->sheet[MOD3(frame / 4) * WHIRLWIND_CEL + (64 - size) * 48],
            32, 64 - size, 48, size);
      }
      if ((u32)(frame - 160) <= 239) {
        draw[0](canvas, &work->sheet[MOD3(frame / 4) * WHIRLWIND_CEL], 8, 0, 48,
                64);
        draw[0](canvas, &work->sheet[MOD3(frame / 4) * WHIRLWIND_CEL], 8, 64,
                48, 64);
      }

      // The dust motes swirl up the whirlwind, speeding up, and start again
      // from the foot when they pass the top.
      if (frame <= 159) {
        index = 0;
        point = work->particles;
        do {
          if (point[index].variant == 0) {
            vector[0] = point[index].z * Trig_Sin(point[index].velocityY);
            vector[1] = point[index].y;
            vector[2] = point[index].z * Trig_Cos(point[index].velocityY);
            EffectPosition_ProjectPoint(vector, &screen);
            screen.x >>= 1;
            if (point[index].y <= 0x3fffff)
              draw[0](canvas, work->sheet + DUST_SHEET, screen.x - 12,
                      screen.y - 12, 16, 16);
            if (point[index].z > 24)
              point[index].z -= 4;
            point[index].velocityY += point[index].velocityX * 2;
            if (point[index].velocityY > 0x10000)
              point[index].velocityY -= 0x10000;
            point[index].velocityX += 50;
            if (point[index].velocityX > 4096)
              point[index].velocityX = 4096;
            point[index].velocityY += 2048;
            point[index].y += point[index].velocityY * 2;
            if (point[index].y > 0x300000) {
              point[index].y = 0;
              point[index].z = 100;
              point[index].velocityY = 0;
              point[index].velocityX = 0;
            }
          } else {
            point[index].variant--;
          }
          index++;
        } while (index != 24);
      }

      // Frame 160: the violet sky, the formation of comets, sparks scattered
      // round its centre, and the enemies dropped back at random heights.
      if (frame == 160) {
        s32 speed;
        s32 angle;

        BattleBackground_LoadFar(1, (s32)&ResourceId_VioletSkyBackdrop, 0);
        BattleStaging_ConfigurePaletteFadeFar(
            1, (s32)&ResourceId_VioletSkyBackdrop, 8);
        BattleEffect_SelectLivingTargets(work->effect);
        BattleEffect_SpawnObjects(COMETS, FORMATION_RESOURCE, 2);
        Resource_LoadAndDecompress((s32)&ResourceId_VioletCometSheet,
                                   work->sheet + COMET_SHEET, 1, 1);

        index = 0;
        do {
          PARTICLES[index].variant = 0;
          index++;
        } while (index != 512);

        for (index = 0; index != effect->count; index++) {
          context = GetBattleObjectSlotFar(effect->actors[index]);
          object = context->object;
          object->y = ((Random16() & 15) + 8) << 16;
        }

        gCameraSceneParameters[3] = 72;

        index = 0;
        do {
          speed = Random16() & 127;
          angle = (Random16() & 0x7fff) + 0x9fff;
          work->particles[index].x = Trig_Sin(angle) * speed;
          work->particles[index].y = Trig_Cos(angle) * speed;
          work->particles[index].z = (Random16() % 200 - 100) << 16;
          work->particles[index].variant = 0;
          index++;
        } while (index != 32);

        for (index = 0; index != effect->count; index++) {
          radius[index] = 16;
          spin[index] = 6000;
        }
      }

      if (frame > 159) {
        // The formation sways on a slow wave, pushed back by each strike.
        scale = Data_08196df8;
        size = 16;
        if (frame - 160 <= 64)
          size = 32;
        wave.x -= (Trig_Sin((frame - 160) << 7) * 32) >> 6;
        wave.y += (Trig_Sin((frame - 160) << 9) * size) >> 6;
        wave.x += drift.x;
        wave.y += drift.y;
        drift.x /= 2;
        drift.y /= 2;

        scale.x = 0x10000;
        scale.y = 0x10000;
        place[3] = 0;
        index = 0;
        do {
          place[0] = (Data_0819793e[index] << 16) + wave.x + 0x280000;
          place[1] = 0xff0000 + index * 0x400000;
          place[2] = ((Data_08197947[index] + index * 64) << 16) + wave.y;
          Render_ApplyProjectedPlacementFar(
              (struct ScaledSprite *)work->objects[index], place, &scale.x, 0);
          index++;
        } while (index != COMETS);

        if (frame > 255) {
          s32 threshold;

          shake[2] = 0x1000000;
          alive = 0;
          shake[0] = 0;
          shake[1] = 0;
          Render_ResetTransformState();
          SceneTransform_ApplyPosition(shake);

          // Until frame 330 the scattered sparks gather on the centre, each
          // a part of its distance a frame; those that have arrived make one
          // spark that grows with their number.
          if (frame <= 329) {
            index = 0;
            do {
              s32 xx;
              s32 yy;
              s32 zz;

              point = &work->particles[index];
              xx = (point->x >> 8) * (point->x >> 8);
              yy = (point->y >> 8) * (point->y >> 8);
              zz = (point->z >> 8) * (point->z >> 8);
              dist = Iwram_Sqrt(xx + yy + zz) >> 9;
              if (dist != 0) {
                EffectPosition_ProjectPoint(&point->x, &screen);
                screen.x = (screen.x >> 17) + (wave.x >> 17) + 32;
                screen.y = (screen.y >> 16) + (wave.y >> 16) - 4;
                screen.depth >>= 16;
                if (screen.depth <= 169)
                  screen.depth = 170;
                if (screen.depth > 350)
                  screen.depth = 350;
                span = 6 - Math_UnsignedMulHigh(screen.depth - 170, 0x071c71c8);
                draw[1](canvas, graphics + gData_08197410[span - 1],
                        screen.x - span / 2, screen.y - span, span, span * 2);
                point->x -= point->x / dist;
                point->y -= point->y / dist;
                point->z -= point->z / dist;
              } else {
                alive++;
              }
              index++;
            } while (index != 32);

            if (alive > 0) {
              span = alive / 10 + 1;
              draw[1](canvas, graphics + gData_08197410[span - 1],
                      (wave.x >> 17) - span / 2 + 32, (wave.y >> 16) - span - 4,
                      span, span * 2);
            }
          }

          // Each strike throws out 96 sparks that fly and fade.
          slot = 0;
          do {
            if (frame == Data_08197950[slot]) {
              s32 speed;
              s32 angle;

              index = 0;
              do {
                speed = (Random16() & 127) + 16;
                angle = Random16() & 0xffff;
                spark = &PARTICLES[slot * 128 + index];
                spark->velocityX = (Trig_Sin(angle) * speed) >> 6;
                spark->velocityY = -(Trig_Cos(angle) * speed) >> 6;
                spark->velocityY = ((Random16() & 255) - 128) << 10;
                spark->x = 0;
                spark->y = 0;
                spark->z = 0;
                spark->variant = (Random16() & 15) + 64;
                index++;
              } while (index != 96);
            }

            if (frame >= Data_08197950[0]) {
              spark = PARTICLES;
              index = 0;
              do {
                if (spark[index].variant > 0) {
                  // The transform in a once-only block, which ends CSE's block
                  // before it; as a plain block, 6 of 1493 instructions differ
                  // in tla-en.
                  do {
                    void (*transform)(const void *, s32 *) =
                        Iwram_TransformVector;
                    /* FAKEMATCH(pin): the result pointer in r1, set before the
                     * point in r0; as a plain argument, 2 of 1493 instructions
                     * differ in tla-en. */
                    register s32 *result __asm__("r1") = &screen.x;
                    transform(&spark[index].x, result);
                  } while (0);
                  screen.x = (screen.x >> 17) + 32;
                  screen.y = (screen.y >> 16) + 56;
                  screen.depth >>= 16;
                  if (screen.depth <= 169)
                    screen.depth = 170;
                  if (screen.depth > 350)
                    screen.depth = 350;
                  span =
                      3 - Math_UnsignedMulHigh(screen.depth - 170, 0x02d82d83);
                  draw[0](canvas, graphics + gData_08197410[span - 1],
                          screen.x - span / 2, screen.y - span, span, span * 2);
                  spark[index].x += spark[index].velocityX;
                  spark[index].y += spark[index].velocityY;
                  spark[index].z += spark[index].velocityY;
                  spark[index].variant--;
                }
                index++;
              } while (index != 512);
            }
            slot++;
          } while (slot != 4);

          // From frame 330, a comet every twelve frames: it strikes at the
          // formation's centre, the screen shakes and the formation is
          // pushed back; a ring of 28 sparks opens round the mark.
          index = 0;
          do {
            mark = &work->particles[index + 60];
            threshold = index * 12 + 330;
            if (frame == threshold) {
              mark->x = (wave.x >> 17) + 32;
              mark->y = (wave.y >> 16) - 4;
              mark->velocityX = (wave.x >> 17) + 32;
              mark->velocityY = (wave.y >> 16) - 4;

              slot = 0;
              do {
                work->particles[slot + 32].variant = 4;
                slot++;
              } while (slot != 28);

              drift.x = 0x80000;
              drift.y = -0x20000;
              work->shakeFrames = 8;
            }

            if (frame >= threshold) {
              s32 cel;

              cel = (frame - threshold) / 2;
              if (cel > 2)
                cel = 2;
              draw[1](canvas, work->sheet + Data_08197962[cel], mark->x,
                      mark->y - Data_0819795e[cel], Data_08197958[cel],
                      Data_0819795b[cel]);
              mark->x -= 8;
              mark->y += 2;

              if (frame < threshold + 8) {
                Func_080150e4(-2048);
                SceneTransform_ApplyYaw(-4096);

                slot = 0;
                spark = &work->particles[32];
                do {
                  struct EffectStep *ring;

                  ring = &spark[slot];
                  vector[0] = 0;
                  vector[1] = ring->variant * Trig_Cos(slot * 2340);
                  vector[2] = ring->variant * Trig_Sin(slot * 2340);
                  ring->variant += 2;
                  EffectPosition_ProjectPoint(vector, &screen);
                  screen.x =
                      (screen.x >> 17) + work->particles[index + 60].velocityX;
                  screen.y =
                      (screen.y >> 16) + work->particles[index + 60].velocityY;
                  screen.depth >>= 16;
                  if (screen.depth <= 169)
                    screen.depth = 170;
                  if (screen.depth > 350)
                    screen.depth = 350;
                  span =
                      3 - Math_UnsignedMulHigh(screen.depth - 170, 0x02d82d83);
                  draw[0](canvas, graphics + gData_08197410[span - 1],
                          screen.x - span / 2, screen.y - span, span, span * 2);
                  slot++;
                } while (slot != 28);
              }
            }
            index++;
          } while (index != 4);
        }
      }

      BattleEffect_CountDownBattlerTints();
      work->transferPending = 1;
      wait(1);
      frame++;
    } while (frame != 400);

    // The results; the enemies go back where they stood, facing as before.
    Func_081180e8(134);
    for (index = 0; index != effect->count; index++) {
      struct MotionObject *enemy;

      context = GetBattleObjectSlotFar(effect->actors[index]);
      enemy = context->object;
      enemy->x = home[index].x;
      enemy->z = home[index].y;
      enemy->angle = baseAngle[index];
    }
    gCameraSceneParameters[3] = 120;
    Func_0814cca8();

    index = 0;
    do {
      ResourceObject_ReleaseFar(work->objects[index]);
      index++;
    } while (index != COMETS);

    Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
    HeapSlot_Release(188);
    HeapSlot_Release(104);
    BattleEffect_EndCanvasLayer();
  }
}

void Func_08147aec(struct BattleEffectArgument *effect, s32 mode);

/// Abilities: Dragon Driver (kind 385).
DEFINE_EFFECT_KIND(BattleEffect_RunDragonDriver, Func_08147aec, 1)

/// Abilities: Tiamat (scene kind 7).
DEFINE_EFFECT_KIND(BattleEffect_RunTiamat, Func_08147aec, 0)

#undef FORMATION_RESOURCE

// The rising grains live in the map buffer.

// The high halfword of a 16.16 value: its whole pixels.
#define HI(v) (((s16 *)&(v))[1])

void Func_0816442c(s32 formation, s32 x, s32 y);  // ..PlaceFormationObjects
void Func_0816467c(s32 formation, s32 x, s32 y);  // ..RunImpactBurst
void Func_08164a4c(s32 blue, s32 green, s32 red); // Palette_BrightenBgEntries
void Animation_ApplyChildArgumentFar(void *object, s32 animation);

extern char ResourceId_EmberStreakSheet;

extern char ResourceId_LightningBoltSheet;
extern u8 Data_08197968[];  // a grain's added speed, by grain
extern u8 Data_0819796c[];  // a spark's two pixels
extern u8 Data_0819796e[];  // a grain's size, by grain
extern u8 Data_08197972[];  // the streak's six pictures: widths
extern u8 Data_08197978[];  // ... heights
extern u8 Data_0819797e[];  // ... drops below the streak
extern u16 Data_08197984[]; // ... cell offsets

#define STREAK_SHEET 0x3000 // the ember streaks, after the flashes
#define FORMATION_RESOURCE 647

// Summon 7 (mode 0; mode 1 is the same scene cast from the caster's side,
// without the formation): streaks rain down and splash, sparks and grains
// rise from where the formation stands, a burst of flashes at frame 128,
// then the screen whitens and the canvas fades away. In mode 0 a formation
// of eight drops in at frame 40 and bursts at the end. A or B skips frames
// 49 to 159.
void Func_08147aec(struct BattleEffectArgument *effect, s32 mode) {
  struct EffectPosition origin;
  DrawRectangle draw[2];
  void *canvas;
  s32 *camera;
  struct BattleEffectWork *work;
  s32 frame;
  u8 *sheet;
  s32 i;

  canvas = Ram_HeapSlots->battleFxWork[1];
  camera = Ram_HeapSlots->cameraWork;
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  sheet = Ram_HeapSlots->unk64[0];

  // Set up: half-and-half blending, the max and additive pair, the flashes
  // with their palette, the streaks, the dots, and in mode 1 the lightning
  // palette.
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = 0x1010;
  REG_BG2CNT = 0x784;
  BattleEffect_FetchRectangleBlitters(0, draw);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_EmberStreakSheet,
                             work->sheet + STREAK_SHEET, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);
  if (mode == 1) {
    const void *lightning =
        Resource_GetTableEntry((s32)&ResourceId_LightningBoltSheet);
    s32 (*copy)(void *destination, const void *source, s32 size) =
        Iwram_CopyWords;

    // The entry held in a local loads ahead of the destination's shift; as a
    // plain Iwram_CopyWords call, 2 of 1072 instructions differ in tla-en.
    copy((void *)BG_PLTT, lightning, 128);
  }
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  {
    struct Point2D focus;
    s32 dir;
    s32 alternate;

    // When the first target's animation kind is 2 the formation stands 8
    // pixels further right and lower, with its sparks and flashes.
    alternate = 0;

    if (((struct Actor *)GetBattleObjectSlotFar(effect->actors[0])->object)
            ->animationKind == 2)
      alternate = 1;

    // Mode 1 on the far side mirrors everything sideways, and starts from
    // the caster.
    if (mode == 0)
      dir = 1;
    else if (effect->side == 1)
      dir = -1;
    else
      dir = 1;
    if (mode == 1) {
      EffectPosition_ProjectScaledBattler(effect->actor, &origin);
      origin.x /= 2;
      origin.y = 66;
      if (effect->side == 1)
        origin.x = 76;
      else
        origin.x = 44;
    }

    // The formation enters from above the right edge.
    if (alternate == 1) {
      focus.x = 220 << 16;
      focus.y = -(52 << 16);
    } else {
      focus.x = 212 << 16;
      focus.y = -(60 << 16);
    }
    for (i = 0; i != 64; i++)
      work->particles[i].variant = -1;
    for (i = 0; i != 16; i++) {
      struct EffectStep *streak = &work->particles[24 + i];

      if (dir == 1)
        streak->x = (Random16() & 127) + 128;
      else
        streak->x = (Random16() & 127) - 128;
      streak->y = (Random16() & 7) - 72;
      streak->variant = -(Random16() & 31); // frames before it falls
    }
    for (i = 0; i != 512; i++)
      PARTICLES[i].variant = -1;
    if (mode == 0) {
      BattleEffect_SelectLivingTargets(work->effect);
      wait(1);
      BattleEffect_SpawnObjects(8, FORMATION_RESOURCE, 2);
    }

    for (frame = 0; frame != 208; frame++) {
      if ((gInput.repeat & 3) && frame > 48 && frame <= 159) {
        if (mode == 0) {
          Animation_ApplyChildArgumentFar(work->objects[0], 8);
          Animation_ApplyChildArgumentFar(work->objects[1], 9);
          Animation_ApplyChildArgumentFar(work->objects[3], 10);
          Animation_ApplyChildArgumentFar(work->objects[4], 11);
        }
        for (i = 0; i != effect->count; i++) {
          Func_0814cd48(effect->actors[i], 10, 5, -1, 0);
          Func_08118088(effect->actors[i], 4);
        }
        frame = 160;
      }
      Render_ResetTransformState();
      Transform_SetLookAt(camera, camera + 3);
      if (frame == 178)
        Func_081180e8(134);

      // From frame 128 the trails halve; from 160 to 175 the canvas is filled
      // and the palette brightens; from 176 the canvas fades by 2 a frame.
      if (frame == 128)
        work->transferValue = 50;
      if (frame == 176) {
        work->transferMode = 3;
        work->transferValue = 0x02020202;
        Iwram_CopyWords(
            (void *)BG_PLTT,
            Resource_GetTableEntry((s32)&ResourceId_EmberStreakSheet), 128);
      } else if (frame >= 160 && frame <= 175) {
        work->transferMode = 1;
        work->transferValue = 0x10101010;
        if (frame > 173)
          work->transferValue = 0x3f3f3f3f;
        Func_08164a4c(2, 2, 2);
      }

      // Grains rise from y = 112: one a frame from 33, eight from 104.
      if (frame >= 33 && frame <= 175) {
        s32 spawned = 0;
        s32 limit = 1;

        if (frame > 103)
          limit = 8;
        for (i = 0; i != 512; i++) {
          struct EffectStep *grain = &PARTICLES[i];

          if (grain->variant == -1) {
            grain->x = ((Random16() & 255) - 32) << 16;
            grain->y = 112 << 16;
            grain->velocityX = ((Random16() & 127) + Data_08197968[i & 3]) << 9;
            grain->velocityY = -((Random16() & 127) + Data_08197968[i & 3])
                               << 11;
            grain->variant = 0;
            spawned++;
            if (spawned == limit)
              break;
          }
        }
      }

      // Sparks fly up and out from the formation, one every other frame.
      if (frame >= 41 && frame <= 127) {
        s32 spawned = 0;

        if (frame & 1) {
          for (i = 0; i != 24; i++) {
            struct EffectStep *spark = &work->particles[40 + i];

            if (spark->variant == -1) {
              s32 speed = (Random16() & 255) + 128;
              s32 angle = (Random16() & 0x1fff) + 20000;

              if (mode == 0) {
                if (alternate == 1) {
                  spark->x = ((Random16() & 7) + 78) << 16;
                  spark->y = 78 << 16;
                } else {
                  spark->x = ((Random16() & 7) + 78) << 16;
                  spark->y = 70 << 16;
                }
              } else {
                spark->x = ((Random16() & 7) + origin.x - 8) << 16;
                spark->y = origin.y << 16;
              }
              spark->velocityX = speed * Trig_Sin(angle) >> 9;
              spark->velocityY = speed * Trig_Cos(angle) >> 9;
              spark->variant = 0;
              spawned++;
              if (spawned == 1)
                break;
            }
          }
        }
      }
      if (frame == 48)
        Sound_PlayFar(141);
      if (frame == 128)
        Sound_PlayFar(145);

      // Flashes fly out sideways, one a frame from 129.
      if (frame >= 129 && frame <= 175) {
        s32 spawned = 0;

        for (i = 0; i != 24; i++) {
          struct EffectStep *flash = &work->particles[i];

          if (flash->variant == -1) {
            s32 speed = (Random16() & 255) + 128;
            s32 angle = (Random16() & 0x1fff) - 20000;

            if (mode == 0) {
              if (alternate == 1) {
                flash->x = 72 << 16;
                flash->y = 72 << 16;
              } else {
                flash->x = 68 << 16;
                flash->y = 64 << 16;
              }
            } else {
              flash->x = origin.x << 16;
              flash->y = origin.y << 16;
            }
            flash->velocityX = speed * Trig_Sin(angle) >> 6;
            flash->velocityY = speed * Trig_Cos(angle) >> 6;
            flash->variant = 0;
            spawned++;
            if (spawned == 1)
              break;
          }
        }
      }

      // A flash steps its picture every four frames and lives 24.
      if (frame <= 175) {
        for (i = 0; i != 24; i++) {
          struct EffectStep *flash = &work->particles[i];

          if (flash->variant >= 0) {
            draw[0](canvas, work->sheet + ((flash->variant >> 2) << 11),
                    HI(flash->x) - 16, HI(flash->y) - 32, 32, 64);
            flash->x += flash->velocityX * dir;
            flash->y += flash->velocityY;
            flash->variant++;
            if (flash->variant == 24)
              flash->variant = -1;
          }
        }
      }

      // A spark is two pixels, speeding upward, and lives 48 frames.
      for (i = 0; i != 24; i++) {
        struct EffectStep *spark = &work->particles[40 + i];
        s32 size = 1;

        if (spark->variant >= 0) {
          draw[1](canvas, Data_0819796c, HI(spark->x), HI(spark->y) - size,
                  size, size * 2);
          spark->x += spark->velocityX * dir;
          spark->y += spark->velocityY;
          spark->velocityY += -0x400;
          spark->variant++;
          if (spark->variant == 48)
            spark->variant = -1;
        }
      }

      // Grains speed upward until frame 128, then bend away sideways (the odd
      // ones harder); each lives 256 frames.
      if (frame <= 175) {
        for (i = 0; i != 512; i++) {
          struct EffectStep *grain = &PARTICLES[i];

          if (grain->variant >= 0) {
            u8 size = Data_0819796e[i & 3];
            s32 height = size * 2;

            draw[0](canvas, sheet + gData_08197410[size - 1],
                    HI(grain->x) - size / 2, HI(grain->y) - size, size, height);
            grain->x += grain->velocityX * dir;
            grain->y += grain->velocityY;
            if (frame > 128) {
              if (i & 1)
                grain->velocityX += -0x8000;
              else
                grain->velocityX += -0x2000;
            } else {
              grain->velocityY += -0x400;
            }
            grain->variant++;
            if (grain->variant == 256)
              grain->variant = -1;
          }
        }
      }

      // The shakes, and the formation: it drops in over frames 40 to 47 and
      // turns to face the targets from 128 to 175.
      if (frame == 128)
        work->shakeFrames = 48;
      if (mode == 0 && frame == 48)
        work->shakeFrames = 8;
      if (frame >= 40 && frame <= 47) {
        focus.x += -(8 << 16);
        focus.y += 16 << 16;
      }
      if (mode == 0) {
        if (frame == 128) {
          Animation_ApplyChildArgumentFar(work->objects[0], 8);
          Animation_ApplyChildArgumentFar(work->objects[1], 9);
          Animation_ApplyChildArgumentFar(work->objects[3], 10);
          Animation_ApplyChildArgumentFar(work->objects[4], 11);
        }
        if (frame == 176) {
          Animation_ApplyChildArgumentFar(work->objects[0], 0);
          Animation_ApplyChildArgumentFar(work->objects[1], 1);
          Animation_ApplyChildArgumentFar(work->objects[3], 3);
          Animation_ApplyChildArgumentFar(work->objects[4], 4);
        }
        if (alternate == 1)
          Func_0816442c(6, focus.x, focus.y);
        else
          Func_0816442c(3, focus.x, focus.y);
      }
      if (frame == 138) {
        for (i = 0; i != effect->count; i++) {
          Func_0814cd48(effect->actors[i], 10, 5, -1, 0);
          Func_08118088(effect->actors[i], 4);
        }
      }

      // A streak falls six pixels a frame after its wait, then below y = 55
      // splashes through six pictures, two frames each, over and over.
      if (frame <= 175) {
        for (i = 0; i != 16; i++) {
          struct EffectStep *streak = &work->particles[24 + i];

          if (streak->y > 55) {
            if (streak->variant >= 0 && streak->variant <= 11) {
              s32 cel = streak->variant / 2;

              draw[0](canvas, work->sheet + Data_08197984[cel] + STREAK_SHEET,
                      streak->x - Data_08197972[cel] / 2,
                      streak->y + Data_0819797e[cel], Data_08197972[cel],
                      Data_08197978[cel]);
            }
            streak->variant++;
            if (streak->variant == 12)
              streak->variant = 0;
          } else if (streak->variant == 0) {
            s32 size = 10;

            streak->x -= dir * 6;
            streak->y += 6;
            if (frame <= 47 && streak->y > 55)
              Sound_PlayFar(136);
            draw[0](canvas, sheet + gData_08197410[size - 1],
                    streak->x - size / 2, streak->y + 30, size, size * 2);
          } else {
            streak->variant++;
          }
        }
      }
      BattleCamera_Shake(8, 8);
      BattleEffect_CountDownBattlerTints();
      work->transferPending = 1;
      wait(1);
    }

    // Tear down: the formation bursts where it stands and is released.
    Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
    HeapSlot_Release(188);
    HeapSlot_Release(104);
    if (mode == 0) {
      if (alternate == 1)
        Func_0816467c(6, focus.x, focus.y);
      else
        Func_0816467c(3, focus.x, focus.y);
    }
    if (mode == 0) {
      for (i = 0; i != 8; i++)
        ResourceObject_ReleaseFar(work->objects[i]);
    }
  }
  BattleEffect_EndCanvasLayer();
}

#undef FORMATION_RESOURCE

// Nereid, summon 2.

extern char ResourceId_WaterSpraySheet;
extern char ResourceId_FirePillarSheetA;

extern u8 Data_08197990[];  // the three columns' feet, x and y
extern s32 Data_08197998[]; // a falling spark's gravity, by spark

#define SPARK_COUNT 1024
#define FORMATION_SIZE 12
#define FORMATION_RESOURCE 649
#define COLUMN_SHEET 0x6e4 // the fire pillar, after the water spray band
#define COLUMN_WIDTH 34
#define COLUMN_HEIGHT 104

/// Summon 2: a formation of twelve sweeps in from the right and settles over
/// the enemies while a band of spray rises from below. At frame 28, and every
/// frame from 32 to 79, sparks burst up from beside the formation and fall
/// back; three columns of fire stream up meanwhile, and every twelve frames
/// the targets flash and jump. After frame 80 the sparks rise away. A or B
/// skips frames 33 to 97.
/// Nereid (summon 2, BattleEffect_RunSceneEffect's case 2), ported call for
/// call from The Broken Seal's BattleEffect_RunNereid. This game reads its heap
/// blocks canvas, work, sheet; loads three resources where The Broken Seal
/// copies a palette and decodes two sheets; takes its blitter pair from
/// BattleEffect_FetchRectangleBlitters and its formation from resource 649; and
/// reads the effect argument in the hit loop.
void BattleEffect_RunNereid(struct BattleEffectArgument *effect) {
  DrawRectangle draw[2];
  void *canvas;
  struct BattleEffectWork *work;
  u8 *graphics;

  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  graphics = Ram_HeapSlots->unk64[0];

  // Set up: half-and-half blending, the max and additive blitters, the
  // spray band and the fire column into the work sheet, the sparks into the
  // sheet slot, and the transfer at three quarters.
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = 0x1010;
  BattleEffect_FetchRectangleBlitters(0, draw);
  Resource_LoadAndDecompress((s32)&ResourceId_WaterSpraySheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_FirePillarSheetA,
                             work->sheet + COLUMN_SHEET, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, graphics, 0, 0);
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  {
    struct Point2D focus;
    struct Point2D velocity;
    s32 rise;
    s32 frame;
    s32 k;
    struct EffectStep *point;
    struct EffectStep *spark;
    struct EffectStep *sparks;
    s32 *life;

    // The formation enters from (256, 88), moving left and up.
    rise = 0;
    focus.x = 256 << 16;
    focus.y = 88 << 16;
    velocity.x = -(16 << 16);
    velocity.y = -(4 << 16);
    for (k = 0; k != 64; k++)
      work->particles[k].variant = -1;
    for (k = 0; k != 16; k++) {
      point = &work->particles[24 + k];
      point->x = Random16() & 127;
      point->y = (Random16() & 7) + 56;
      point->variant = -(Random16() & 15);
    }

    // Every spark in the map buffer starts free (variant -1).
    life = &((struct EffectStep *)Ram_MapCellBuffer)->variant;
    for (k = 0; k != SPARK_COUNT; k++) {
      *life = -1;
      life += sizeof(struct EffectStep) / 4;
    }
    sparks = (struct EffectStep *)Ram_MapCellBuffer;
    BattleEffect_SelectLivingTargets(work->effect);
    wait(1);
    BattleEffect_SpawnObjects(FORMATION_SIZE, FORMATION_RESOURCE, 2);

    for (frame = 0; frame != 124; frame++) {
      if ((gInput.repeat & 3) && frame > 32 && frame <= 97)
        frame = 98;
      if (frame == 120)
        Func_081180e8(134);
      if (frame <= 15)
        rise += 2;

      // The formation slows (x 58/64, y 56/64 each frame) and is pushed
      // back right once it passes x = 120.
      if (frame <= 99) {
        focus.x += velocity.x;
        focus.y += velocity.y;
        velocity.x = velocity.x * 58 / 64;
        velocity.y = velocity.y * 56 / 64;
        if (focus.x < 120 << 16)
          velocity.x += 0x8000;
      }
      Func_0816442c(1, focus.x, focus.y);

      // Sparks burst up from around (32, 96): every free one at frame 28,
      // then sixteen a frame from 32 to 79.
      if (frame == 28) {
        for (k = 0; k != 256; k++) {
          spark = &sparks[k];
          if (spark->variant == -1) {
            s32 radius = Random16() & 63;
            s32 angle = Random16() & 0xffff;

            spark->x = ((Trig_Sin(angle) * radius) >> 3) + (32 << 16);
            spark->y = ((Trig_Cos(angle) * radius) >> 2) + (96 << 16);
            spark->velocityX = ((Random16() & 63) - 32) << 14;
            spark->velocityY = (-(Random16() & 63) - 8) << 13;
            spark->variant = 0;
          }
        }
      }
      if (frame >= 32 && frame < 80) {
        s32 spawned = 0;

        for (k = 0; k != SPARK_COUNT; k++) {
          spark = &sparks[k];
          if (spark->variant == -1) {
            s32 radius = Random16() & 63;
            s32 angle = Random16() & 0xffff;

            spark->x = ((Trig_Sin(angle) * radius) >> 3) + (32 << 16);
            spark->y = ((Trig_Cos(angle) * radius) >> 2) + (96 << 16);
            spark->velocityX = ((Random16() & 63) - 32) << 14;
            spark->velocityY = (-(Random16() & 63) - 8) << 13;
            spark->variant = 0;
            spawned++;
            if (spawned == 16)
              break;
          }
        }
      }
      if (frame == 0)
        Sound_PlayFar(164);
      if (frame == 32)
        Sound_PlayFar(145);
      if (frame == 80)
        Sound_PlayFar(144);

      // Three columns of fire scroll up while the sparks burst.
      if (frame >= 32 && frame < 80) {
        for (k = 0; k != 3; k++) {
          s32 span = (frame * 16 - 256 + k * 25) % COLUMN_HEIGHT;

          draw[0](canvas, work->sheet + COLUMN_SHEET,
                  Data_08197990[k * 2] - COLUMN_WIDTH / 2,
                  Data_08197990[k * 2 + 1] - span - COLUMN_HEIGHT, COLUMN_WIDTH,
                  COLUMN_HEIGHT);
          draw[0](canvas, work->sheet + COLUMN_SHEET,
                  Data_08197990[k * 2] - COLUMN_WIDTH / 2,
                  Data_08197990[k * 2 + 1] - span, COLUMN_WIDTH, span);
        }
      }

      // The spray band rises sixteen pixels and drifts right.
      if (frame <= 95) {
        for (k = 0; k != 5; k++)
          draw[0](canvas, work->sheet, k * 32 + ((frame / 4) & 31) - 32,
                  120 - rise, 32, 32);
      }

      // Each live spark: rising ones are drawn larger and by the max
      // blitter, falling ones by the additive one; all grow from frame 68.
      // They fall under gravity until frame 80, then accelerate upward, and
      // a falling spark that reaches y = 104 is freed.
      for (k = 0; k != SPARK_COUNT; k++) {
        spark = &sparks[k];
        if (spark->variant >= 0) {
          s32 size = k % 3 + 2;

          if (spark->velocityY > 0)
            size += 2;
          if (frame > 68 && size <= 5)
            size = 6;
          if (frame > 70 && size <= 6)
            size = 7;
          if (frame > 72 && size <= 7)
            size = 8;
          if (frame > 74 && size <= 8)
            size = 9;
          if (frame > 76)
            size = 10;
          draw[spark->velocityY > 0](
              canvas, graphics + gData_08197410[size - 1],
              HI(spark->x) - size / 2, HI(spark->y) - size, size, size * 2);
          spark->x += spark->velocityX;
          spark->y += spark->velocityY;
          if (frame > 80)
            spark->velocityY += -0x8000;
          else
            spark->velocityY += Data_08197998[k & 3];
          spark->velocityX = spark->velocityX * 62 / 64;
          spark->velocityY = spark->velocityY * 62 / 64;
          spark->variant++;
          if (spark->velocityY > 0 && HI(spark->y) > 104)
            spark->variant = -1;
        }
      }

      // From frame 30 to 79 the targets flash and are knocked upward every
      // twelve frames, and stop flashing six frames later.
      if (frame <= 79) {
        for (k = 0; k != effect->count; k++) {
          if (frame > 29) {
            s32 phase = frame % 12;

            if (phase == 0) {
              struct MotionObject *target =
                  GetBattleObjectSlotFar(effect->actors[k])->object;

              Func_0814cd48(effect->actors[k], 7, 5, -1, 0);
              target->velocityY = 0x48000;
              target->verticalMotionStrength = 0xab85;
            }
            if (phase == 6)
              Func_0814cd48(effect->actors[k], 0, 5, -1, 0);
          }
        }
      }
      work->transferPending = 1;
      wait(1);
    }

    // Tear down: the formation bursts where it stands and is released.
    Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
    HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
    HeapSlot_Release(HEAP_BLITTER);
    Func_0816467c(1, focus.x, focus.y);
    for (k = 0; k != FORMATION_SIZE; k++)
      ResourceObject_ReleaseFar(work->objects[k]);
  }
  BattleEffect_EndCanvasLayer();
}

#undef HEAP_BLITTER
#undef HEAP_BLITTER_ALTERNATE

// Heap slots The Lost Age addresses by byte offset in Ram_HeapSlots.
#define HEAP_BLITTER 104           // ☀️ HEAP_SLOT_BLITTER
#define HEAP_BLITTER_ALTERNATE 188 // ☀️ HEAP_SLOT_BLITTER_ALTERNATE

// The Lost Age numbers its blitter modes in the reverse of The Broken Seal's
// order: Func_08196404 builds additive for 0, max for 1, transparent for 2.
enum { BLITTER_ADDITIVE, BLITTER_MAX, BLITTER_TRANSPARENT, BLITTER_COPY };

#define MOTE_COUNT 30
#define SEAT_FIRST 32 // the particles that hold each target's seat

extern char ResourceId_VortexSheet;

// ☀️ gVortices_Counts: motes, then frames, for each variant.
extern u8 gData_081979a8[];

void Camera_ApplyPhasedDelta(void);

// Thirty motes rise from the caster, swirl, and home in on the targets; each
// one that reaches the ground flashes and knocks back its target and plays a
// three-cel splash where it landed.
void BattleEffect_RunFroth(struct BattleEffectArgument *effect) {
  struct EffectPosition pos;
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle draw[2];
  struct MotionObject *source;
  struct MotionObject *object;
  struct EffectStep *point;
  struct EffectStep *seat;
  struct EffectStep *target;
  struct BattleCamera *camera;
  s32 frame;
  s32 y;
  s32 i;
  s8 *landed;

  // Set up: the canvas layer, the vortex sheet and two blitters, the one on
  // the right mirrored.
  landed = (s8 *)Ram_MapCellBuffer;
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  BattleEffect_StartCanvasLayer(1);
  Resource_LoadAndDecompress((s32)&ResourceId_VortexSheet, work->sheet, 1, 1);
  BattleEffect_LoadBlitter(HEAP_BLITTER, BLITTER_MAX << 4 | COMPOSITE_CLIP_X |
                                             COMPOSITE_CLIP_Y);
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                           BLITTER_MAX << 4 | COMPOSITE_CLIP_X |
                               COMPOSITE_CLIP_Y | COMPOSITE_FLIP);
  REG_BLDALPHA = BLEND_ALPHA(15, 15);
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];

  // The motes start at the caster's chest with random velocities.
  source = GetBattleObjectSlotFar(effect->actor)->object;
  y = source->y + BattleUnit_GetEffectHeightFar(effect->actor);
  for (i = 0; i != MOTE_COUNT; i++) {
    point = &work->particles[i];
    point->x = source->x;
    point->y = y;
    point->z = source->z;
    point->velocityX = ((s32)((Random16() & 255) - 127) << 16) >> 5;
    point->velocityY = ((s32)((Random16() & 127) - 16) << 16) >> 6;
    point->velocityZ = ((s32)((Random16() & 255) - 127) << 16) >> 5;
    point->variant = -1;
    landed[i] = 0;
  }

  // Each target's seat on the ground.
  for (i = 0; i != effect->count; i++) {
    seat = &work->particles[SEAT_FIRST + i];
    object = GetBattleObjectSlotFar(effect->actors[i])->object;
    seat->x = object->x;
    seat->y = 0;
    seat->z = object->z;
  }

  work->cameraDelta = 0;
  work->cameraPhase = 0;
  Task_Add(Camera_ApplyPhasedDelta, TASK_PASS_FRAME);
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  Sound_PlayFar(164);

  for (frame = 0; frame != gData_081979a8[effect->variant * 2 + 1]; frame++) {
    // The camera turns between frames 17 and 63.
    camera = Ram_HeapSlots->cameraWork;
    if (frame >= 17 && frame < 64)
      work->cameraDelta = 0x180;
    else
      work->cameraDelta = 0;
    if (frame == gData_081979a8[effect->variant * 2 + 1] - 16)
      Func_081180e8(133);
    Render_ResetTransformState();
    Transform_SetLookAt(camera->eye, camera->pos);

    for (i = 0; i != gData_081979a8[effect->variant * 2]; i++) {
      point = &work->particles[i];

      // A mote leaves the caster two frames after the one before it.
      if (frame > i * 2 && landed[i] == 0) {
        EffectPosition_ProjectPoint((s32 *)point, &pos);
        pos.x >>= 1;
        if (pos.depth < 160)
          pos.depth = 160;
        if (pos.depth > 0x31f)
          pos.depth = 0x31f;
        draw[0](canvas, work->sheet + 0xc00, pos.x - 6, pos.y - 12, 12, 24);
        point->x += point->velocityX;
        point->y += point->velocityY;
        point->z += point->velocityZ;
      }

      // After 48 frames it steers toward its target's seat, damped for the
      // first 37 of them, and lands.
      if (frame > i * 2 + 48 && landed[i] == 0) {
        target = &work->particles[SEAT_FIRST + i % effect->count];
        point->velocityX += (target->x - point->x) >> 9;
        point->velocityY += (target->y - point->y) >> 9;
        point->velocityZ += (target->z - point->z) >> 9;
        if (frame < i * 2 + 85) {
          point->velocityX = point->velocityX * 60 / 64;
          point->velocityY = point->velocityY * 60 / 64;
          point->velocityZ = point->velocityZ * 60 / 64;
        }
        if (point->y < 0) {
          landed[i] = 1;
          point->variant = 0;
          point->x = pos.x;
          point->y = pos.y + (Random16() & 31) - 16;
          Func_0814cd48(effect->actors[i % effect->count], 7, 5,
                        i % effect->count, 4);
          Func_08118088(effect->actors[i % effect->count], 0);
          work->shakeFrames = 4;
          Sound_PlayFar(132);
        }
      }

      // The splash: three cels, two frames each, drawn as mirrored halves.
      if (point->variant >= 0 && point->variant < 16) {
        draw[0](canvas, work->sheet + ((point->variant / 2) % 3 << 10),
                point->x - 16, point->y - 56, 16, 64);
        draw[1](canvas, work->sheet + ((point->variant / 2) % 3 << 10),
                point->x, point->y - 56, 16, 64);
        point->variant++;
      }
    }

    BattleCamera_Shake(effect->variant * 2 + 2, effect->variant * 2 + 2);
    if (work->cameraPhase == 0)
      work->cameraPhase = 1;
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  // Tear down.
  Task_Remove(Camera_ApplyPhasedDelta);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}

#define TORNADO_FRAMES 80
#define GLINT_COUNT 16 // four glints for each of up to four funnels
#define GLINT_CELS 6   // three cels of two frames each

extern char ResourceId_TornadoSheet;

// ☀️ gVortices_Shapes: funnels, sway radius and shake, for each variant.
extern u8 gData_081979ae[];
// ☀️ gBattleEffect_GlintCells, Widths and Heights.
extern u16 gData_08197486[];

// One to four funnels of wind sway across the targets for 80 frames, each
// drawn as three stacked segments with glints swirling around it; the canvas
// is wiped every eight frames, and each target is struck seven times.
void BattleEffect_RunWhirlwind(struct BattleEffectArgument *effect) {
  DrawRectangle draw[2];
  struct BattleEffectWork *work;
  void *canvas;
  s32 wave;
  struct EffectStep *glint;
  s32 frame;
  s32 x;
  s32 y;
  s32 segment;
  s32 cell;
  s32 i;
  s32 j;

  // Set up: the canvas layer, the tornado sheet and two max blitters, the
  // second mirrored.
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  BattleEffect_StartCanvasLayer(1);
  REG_BLDALPHA = 0x1010;
  Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, work->sheet, 1, 1);
  BattleEffect_LoadBlitter(HEAP_BLITTER, BLITTER_MAX << 4 | COMPOSITE_CLIP_X |
                                             COMPOSITE_CLIP_Y);
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                           BLITTER_MAX << 4 | COMPOSITE_CLIP_X |
                               COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X);
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];

  // The glints start at random places in their funnel, staggered in time.
  for (j = 0; j != GLINT_COUNT; j++) {
    glint = &work->particles[j];
    glint->x = Random16() & 31;
    glint->y = (Random16() & 63) + 16;
    glint->variant = -(Random16() & 15);
  }
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  REG_BLDALPHA = 0x1000;
  wait(1);
  Sound_PlayFar(141);

  for (frame = 0; frame != TORNADO_FRAMES; frame++) {
    wave = Trig_Sin(frame << 10) << 4;
    if (frame == 32)
      Func_081180e8(133);

    // Wipe the canvas every eight frames from frame 16, leaving a trail.
    for (i = 0; i != 7; i++) {
      if (frame == i * 8 + 16)
        Iwram_FillWords(canvas, 0x4000, 0x08080808);
    }

    // Sway toward the targets' side; fade in over 16 frames and out over the
    // last 16.
    if (effect->side == 1)
      wave += 0x200000;
    else
      wave -= 0x200000;
    if (frame <= 16)
      REG_BLDALPHA = frame | 0x1000;
    if (frame > 63)
      REG_BLDALPHA = (0x4f - frame) | 0x1000;

    for (i = 0; i != gData_081979ae[effect->variant * 3]; i++) {
      // Each funnel circles its own phase, a quarter turn from the next.
      x = ((gData_081979ae[effect->variant * 3 + 1] *
                Trig_Sin((frame << 11) + i * 0x4000) +
            wave) >>
           16) +
          40;
      y = (Trig_Cos((frame << 11) + i * 0x4000) * 2) >> 16;
      segment = (frame / 2) % 3;
      draw[0](canvas, work->sheet + segment * 0xa00 + 0xc56, x, y + 16, 40, 32);
      draw[0](canvas, work->sheet + segment * 0x500 + 0x2a56, x, y + 48, 40,
              32);
      draw[0](canvas, work->sheet + segment * 0xa00 + 0x1156, x, y + 80, 40,
              32);

      // Its four glints, the second of each pair mirrored; each restarts at
      // a new place when its cels run out.
      for (j = 0; j != 4; j++) {
        glint = &work->particles[i * 4 + j];
        if (glint->variant >= 0) {
          cell = glint->variant / 2 + (j / 2) * 3;
          draw[j & 1](canvas, work->sheet + gData_08197486[cell], glint->x + x,
                      glint->y + y, gData_08197492[cell], gData_08197498[cell]);
        }
        glint->variant++;
        if (glint->variant == GLINT_CELS) {
          glint->x = Random16() & 31;
          glint->y = (Random16() & 63) + 16;
          glint->variant = 0;
        }
      }
    }

    // Strike each target seven times, every eight frames, three frames
    // after the target before it.
    for (j = 0; j != effect->count; j++) {
      for (i = 0; i != 7; i++) {
        if (frame == i * 8 + j * 3 + 16) {
          Func_0814cd48(effect->actors[j], 7, 5, j, 4);
          Func_08118088(effect->actors[j], 6);
        }
      }
    }

    work->shakeFrames = 1;
    BattleCamera_Shake(gData_081979ae[effect->variant * 3 + 2],
                       gData_081979ae[effect->variant * 3 + 2] * 2);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  // Tear down.
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

#undef GROUND_Y

#define SHARDS_PER_CRYSTAL 16
#define GROUND_Y (56 << 16)
#define SHATTERED 1 // a crystal's z once it has hit the ground

extern char ResourceId_BoulderSheet; // directory row 0x14d
extern char ResourceId_CrystalSheet; // directory row 0x193

// ☀️ gPrism_Counts, with a third byte: crystals, frames and the frame
// that begins phase 134, for each variant.
extern u8 Data_081979b7[];
extern s32 gData_08197834[]; // ☀️ gPrism_ShardOffsets
extern u8 gData_0819781a[];  // ☀️ gPrism_ShardWidths
extern u8 gData_08197826[];  // ☀️ gPrism_ShardHeights
extern u8 gData_081977f8[];  // ☀️ gPrism_ShardStarts: x, y for each shard

// Crystals fall one after another, eight frames apart, onto the targets'
// ground and shatter into sixteen spinning shards each; the screen darkens
// over the last eight frames.
void BattleEffect_RunPrism(struct BattleEffectArgument *effect) {
  DrawRectangle draw[2];
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  struct EffectStep *seed;
  struct EffectStep *cry;
  struct EffectStep *shards;
  struct EffectStep *shard;
  s32 x;
  s32 cell;
  s32 i;
  s32 j;
  s32 k;

  // Set up: the canvas layer, the two sheets and the blitter pair.
  shards = (struct EffectStep *)Ram_MapCellBuffer;
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = 0x1010;
  Resource_LoadAndDecompress((s32)&ResourceId_BoulderSheet, work->sheet, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_CrystalSheet, work->sheet, 1, 1);
  BattleEffect_FetchRectangleBlitters(effect->side, draw);
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  // The crystals wait above the screen, each eight frames behind the last,
  // and drift sideways so that they land over the targets.
  for (k = 0; k != Data_081979b7[effect->variant * 3]; k++) {
    seed = &work->particles[k];
    x = Random16() & 31;
    seed->y = -0x400000;
    if (effect->side == 1) {
      x = ((Random16() & 31) + 80) << 16;
      seed->velocityX = (Random16() & 63) << 12;
    } else {
      x = ((Random16() & 31) + 8) << 16;
      seed->velocityX = -(Random16() & 63) << 12;
    }
    seed->x = x - seed->velocityX * 18;
    seed->velocityY = 0;
    seed->z = 0;
    seed->variant = k * 8;
  }

  for (frame = 0; frame != Data_081979b7[effect->variant * 3 + 1]; frame++) {
    // The strongest variant turns the camera, slowing from frame 96.
    if (effect->variant == 2 && frame <= 103) {
      struct BattleCamera *camera = Ram_HeapSlots->cameraWork;
      s32 speed;

      if (frame <= 95)
        speed = 192;
      else
        speed = 2496 - frame * 24;
      if (effect->side == 0)
        camera->yaw -= speed;
      else
        camera->yaw += speed;
    }
    if (frame == Data_081979b7[effect->variant * 3 + 2])
      Func_081180e8(134);

    // Darken over the last eight frames.
    if (frame == Data_081979b7[effect->variant * 3 + 1] - 8) {
      work->transferMode = 3;
      work->transferValue = 0x06060606;
    }

    if (frame <= Data_081979b7[effect->variant * 3 + 1] - 8) {
      for (i = 0; i != Data_081979b7[effect->variant * 3]; i++) {
        cry = &work->particles[i];
        if (cry->z == SHATTERED) {
          // A shattered crystal: its shards fly and spin, slowing down.
          for (j = 0; j != SHARDS_PER_CRYSTAL; j++) {
            cell = (j % 5) * 3 +
                   (shards[i * SHARDS_PER_CRYSTAL + j].variant / 96) % 3;
            shard = &shards[i * SHARDS_PER_CRYSTAL + j];
            draw[j <= 2](canvas, work->sheet + gData_08197834[cell] + 0x800,
                         HI(shard->x) - gData_0819781a[cell] / 2,
                         HI(shard->y) - gData_08197826[cell] / 2,
                         gData_0819781a[cell], gData_08197826[cell]);
            BattleEffectKernels_IntegrateVector2(shard, 64, 0x2000);
            shard->variant += shard->z;
            if (shard->z > 1 && (frame & 1))
              shard->z--;
          }
        } else if (frame >= cry->variant) {
          // A falling crystal (32x64), until it reaches the ground.
          draw[i & 1](canvas, work->sheet, HI(cry->x) - 16, HI(cry->y), 32, 64);
          BattleEffectKernels_IntegrateVector2(cry, 64, 0x10000);
          if (cry->y > GROUND_Y) {
            cry->z = SHATTERED;
            cry->y = GROUND_Y;
            for (j = 0; j != SHARDS_PER_CRYSTAL; j++) {
              shard = &shards[i * SHARDS_PER_CRYSTAL + j];
              shard->x = ((gData_081977f8[j * 2] - 40) << 16) + cry->x;
              shard->y = gData_081977f8[j * 2 + 1] << 16;
              shard->velocityX = ((Random16() & 127) - 64) << 11;
              shard->velocityY = -(Random16() & 127) << 11;
              if (i & 1) {
                shard->velocityX *= 2;
                shard->velocityY *= 2;
              }
              shard->z = 32;
              shard->variant = 0;
            }
            work->shakeFrames = 8;
            Sound_PlayFar(144);
            for (j = 0; j != effect->count; j++)
              Func_0814cd48(effect->actors[j], 7, 5, j, 4);
          }
        }
      }
    }

    BattleCamera_Shake(effect->variant * 2 + 4, effect->variant * 4 + 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  // Tear down.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}

#undef SPARK_COUNT
#undef GROUND_Y

void Func_081496c8(s32 frame, s32 redPhase, s32 greenPhase, s32 bluePhase) {
  u16 palette[64];
  s32 phase;
  s32 redOffset;
  s32 greenOffset;
  s32 blueOffset;
  s32 index;

  phase = frame * 0x400;
  redOffset = (Trig_Sin(phase + redPhase) * 16) >> 15;
  greenOffset = (Trig_Sin(phase + greenPhase) * 16) >> 15;
  blueOffset = (Trig_Sin(phase + bluePhase) * 16) >> 15;
  palette[0] = 0;

  index = 1;
  do {
    s32 red;
    s32 green;
    s32 blue;

    red = (redOffset + 16) * index / 64;
    green = (greenOffset + 16) * index / 64;
    blue = (blueOffset + 16) * index / 64;

    if (red < 0)
      red = 0;
    if (red > 31)
      red = 31;
    if (green < 0)
      green = 0;
    if (green > 31)
      green = 31;
    if (blue < 0)
      blue = 0;
    if (blue > 31)
      blue = 31;

    palette[index] = (blue << 10) | (green << 5) | red;
    index++;
  } while (index != 64);

  /* FAKEMATCH(wrapper): the copy through CopyWords_, which builds the palette's
   * address anew for it; as a direct Iwram_CopyWords call, 47 of 96
   * instructions differ in tla-en. */
  CopyWords_((void *)0x05000002, palette, sizeof(palette));
}

#define SPARK_MAX 1024
#define PILLAR_BASE_Y 104

// The camera work, as far as this effect reads it.
struct CameraWork {
  u8 unk00[0x36];
  u16 scroll;
};

extern char ResourceId_LightningPillarSheet; // directory row 0x194
extern char ResourceId_VioletPaletteB;       // directory row 0x122

extern u8 Data_081979da[];   // ☀️ gPlasma_Counts
extern u8 Data_081979cc[];   // ☀️ gPlasma_Columns, 7 for each side
extern u16 Data_081979c0[];  // ☀️ gPlasma_Sparks: sparks, shake
extern u16 gData_08197410[]; // ☀️ gBattleEffect_MoteCells

// ☀️ ProcessPendingGraphicsTransfer
void BattleEffect_ProcessPendingGraphicsTransfer(
    void); // ☀️ ProcessPendingGraphicsTransfer
// ☀️ BattleEvent_StartPhaseFar
void Func_081180e8(s32 phase);             // ☀️ BattleEvent_StartPhaseFar
void Func_08118088(s32 actor, s32 motion); // ☀️ ApplyVariantMotionFar
// ☀️ BattleEffect_BuildBlitter
s32 BattleEffect_LoadBlitter(s32 slot,
                             s32 flags); // ☀️ BattleEffect_BuildBlitter
void Func_0814cd48(s32 actor, s32 palette, s32 frames, s32 timerSlot,
                   s32 timerFrames); // ☀️ BattleEffect_TintBattler

// Pillars of lightning strike the targets' row one after another, eight
// frames apart; each flashes the screen, throws a fan of sparks up from its
// base and shakes the camera, and the sparks bounce on the ground.
void BattleEffect_RunPlasma(struct BattleEffectArgument *effect) {
  DrawRectangle draw[2];
  struct BattleEffectWork *work;
  void *canvas;
  s32 i;
  u8 *sprites;
  s32 count;
  s32 frame;
  s32 k;
  s32 start;
  s32 *variant;
  struct EffectStep *spark;
  struct EffectStep *sparks;

  // Set up: the canvas layer, an additive blitter and a mirrored max one,
  // the pillar sheet and the particle sprites, and a violet palette for all
  // but the strongest variant.
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sprites = Ram_HeapSlots->unk64[0];
  BattleEffect_StartCanvasLayer(1);
  REG_BLDALPHA = 0x1010;
  BattleEffect_LoadBlitter(HEAP_BLITTER, BLITTER_ADDITIVE << 4 |
                                             COMPOSITE_CLIP_X |
                                             COMPOSITE_CLIP_Y);
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                           BLITTER_MAX << 4 | COMPOSITE_CLIP_X |
                               COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X);
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
  Resource_LoadAndDecompress((s32)&ResourceId_LightningPillarSheet, work->sheet,
                             1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
  if (effect->variant != 2) {
    void *palette = Resource_GetTableEntry((s32)&ResourceId_VioletPaletteB);
    s32 (*copy)(void *destination, const void *source, s32 size) =
        Iwram_CopyWords;

    // The entry held in a local loads ahead of the destination's shift; as a
    // plain Iwram_CopyWords call, 2 of 457 instructions differ in tla-en.
    copy((void *)BG_PLTT, palette, 128);
  }

  // Every spark starts at rest.
  variant = &((struct EffectStep *)Ram_MapCellBuffer)->variant;
  for (k = 0; k != SPARK_MAX; k++) {
    *variant = 0;
    variant += 7;
  }
  sparks = (struct EffectStep *)Ram_MapCellBuffer;
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  count = Data_081979da[effect->variant];

  for (frame = 0; frame != count * 8 - count + 42; frame++) {
    // The strongest variant pans the camera over the first 64 frames.
    if (effect->variant == 2 && frame <= 63) {
      struct CameraWork *camera = Ram_HeapSlots->cameraWork;
      s32 speed;

      if (frame <= 55)
        speed = 256;
      else
        speed = 704 - frame * 8;
      if (effect->side == 1)
        camera->scroll -= speed;
      else
        camera->scroll += speed;
    }
    if (frame == 24)
      Func_081180e8(134);

    // Pillar i strikes at frame 8i: a flash, then its first cel for one
    // frame and its second for two.
    for (i = 0; i != count; i++) {
      start = i * 8;
      if (frame == start) {
        Sound_PlayFar(134);
        Iwram_FillWords(canvas, 0x4000, 0x10101010);
      }
      if (frame >= start && frame < start + 9) {
        if (frame >= start + 1 && frame < start + 2)
          draw[0](canvas, work->sheet, Data_081979cc[i + effect->side * 7] - 24,
                  0, 48, 112);
        if (frame >= start + 2 && frame < start + 4)
          draw[0](canvas, work->sheet + 0x1500,
                  Data_081979cc[i + effect->side * 7] - 24, 0, 48, 112);

        // Throw sparks up from the pillar's base, within 90 degrees of
        // straight up, and shake the camera.
        if (frame == start + 2) {
          s32 thrown = 0;

          for (k = 0; k != SPARK_MAX; k++) {
            spark = &sparks[k];
            if (spark->variant == 0) {
              s32 speed;
              s32 angle;

              speed = (Random16() & 0x3ff) + 32;
              angle = (Random16() & 0x7fff) - 0x4000;

              spark->x = Data_081979cc[i + effect->side * 7] << 16;
              spark->y = PILLAR_BASE_Y << 16;
              spark->velocityX = (Trig_Sin(angle) * speed) >> 7;
              spark->velocityY = -((Trig_Cos(angle) * speed) << 1) >> 7;
              spark->variant = (Random16() & 7) + 32;
              thrown++;
              if (thrown == Data_081979c0[effect->variant * 2])
                break;
            }
          }
          work->shakeFrames = Data_081979c0[effect->variant * 2 + 1];
        }
      }

      // Four frames after each strike the targets recoil and flash.
      if (frame == start + 4) {
        for (k = 0; k != effect->count; k++) {
          Func_08118088(effect->actors[k], 1);
          Func_0814cd48(effect->actors[k], 7, 5, k, 8);
        }
      }
    }

    // The sparks fall and bounce at half speed off the ground, drawn
    // smaller as they die out.
    for (k = 0; k != SPARK_MAX; k++) {
      spark = &sparks[k];
      if (spark->variant > 0) {
        spark->variant--;
        BattleEffectKernels_IntegrateVector2(spark, 60, 0x1000);
        if (spark->y > PILLAR_BASE_Y << 16) {
          spark->velocityY = -spark->velocityY / 2;
        } else {
          s32 x = spark->x >> 16;
          s32 y;
          s32 size;

          if (spark->x >= 0 && x <= 119 && spark->y >= 0) {
            y = spark->y >> 16;
            size = spark->variant / 8 + 1;
            draw[k & 1](canvas, sprites + gData_08197410[size - 1],
                        x - size / 2, y - size, size, size * 2);
          }
        }
      }
    }

    BattleCamera_Shake(8, 16);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  // Tear down.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}

/// Abilities: Blast, Nova, Supernova, Fire Bomb, Cluster Bomb, Carpet Bomb
/// (kind 15).
DEFINE_EFFECT_KIND(BattleEffect_RunNova, BattleEffect_RunSparkGroups, 0)

/// Abilities: Dynamite, Angry Mine (kind 338).
DEFINE_EFFECT_KIND(BattleEffect_RunDynamite, BattleEffect_RunSparkGroups, 1)

// The pixel half of a 16.16 value.

#define FLASH_POINTS 16 // flash points kept for each group
#define FLASH_DRAWN 12  // of which the first 12 are drawn
#define FLASH_FRAMES 18 // six cels of three frames
#define GROUND_Y (108 << 16)

// The kinds of spark group, chosen by the effect's entry.
enum {
  SPARKS_FIXED,     // from a fixed point, canvas at priority 1
  SPARKS_ORANGE,    // from a lower point, orange, with a palette fade
  SPARKS_FROM_ACTOR // from the caster, lightning colours, sparks rising
};

extern char ResourceId_FlashBurstSheet; // directory row 0x13e

extern char ResourceId_OrangePaletteB;     // directory row 0x148
extern char ResourceId_LightningBoltSheet; // directory row 0x188

// ☀️ gPlasma_Shapes, five halfwords for each variant: sparks in each
// group, groups, and each group's x offset.
extern u16 Data_081979de[][5];
extern u8 Data_08197a1a[]; // ☀️ gPlasma_FlashCells

// The sparks live in the map cell buffer.
extern u8 gMapCellBuffer[];

void BattleEffect_FetchRectangleBlitters(
    s32 flip, void **out); // ☀️ FetchRectangleBlitters

#define SHAPE(n) Data_081979de[effect->variant][n]
#define SPARK_COUNT SHAPE(0)
#define GROUP_COUNT SHAPE(1)
#define GROUP_X(i) SHAPE((i) + 2)

// One to three groups of sparks burst one after another, eight frames
// apart: each opens with a big flash, a ring of small flashes and a spray
// of sparks that fall (or, from the caster, rise) and bounce; six frames
// after each burst the targets flash and recoil.
void BattleEffect_RunSparkGroups(struct BattleEffectArgument *effect,
                                 s32 kind) {
  struct EffectPosition pos;
  DrawRectangle draw[2];
  struct BattleEffectWork *work;
  void *canvas;
  s32 i;
  s32 frame;
  u8 *graphics;
  s32 originX;
  s32 originY;
  s32 j;
  s32 start;
  struct EffectStep *sparks;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  graphics = Ram_HeapSlots->unk64[0];

  // The burst's origin on the canvas.
  if (kind == SPARKS_FIXED) {
    BattleEffect_StartCanvasLayer(1);
    originX = 60;
    originY = 48;
  } else if (kind == SPARKS_ORANGE) {
    BattleEffect_StartCanvasLayer(0);
    originX = 60;
    originY = 64;
    work->fadeFrames = 24;
    work->fadeStep = 0;
  } else {
    BattleEffect_StartCanvasLayer(0);
    EffectPosition_ProjectScaledBattler(effect->actor, &pos);
    originX = pos.x / 2;
    originY = pos.y + 48;
  }

  // The flash sheet, the particle sprites and the kind's palette.
  REG_BLDALPHA = 0x1010;
  BattleEffect_FetchRectangleBlitters(0, draw);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, graphics, 0, 0);
  if (kind == SPARKS_ORANGE) {
    const void *palette =
        Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB);
    s32 (*copy)(void *destination, const void *source, s32 size) =
        Iwram_CopyWords;

    // The entry held in a local loads ahead of the destination's shift; as a
    // plain Iwram_CopyWords call, 2 of 783 instructions differ in tla-en.
    copy((void *)BG_PLTT, palette, 128);
  } else if (kind == SPARKS_FROM_ACTOR) {
    const void *palette =
        Resource_GetTableEntry((s32)&ResourceId_LightningBoltSheet);
    s32 (*copy)(void *destination, const void *source, s32 size) =
        Iwram_CopyWords;

    // The entry held in a local loads ahead of the destination's shift; as a
    // plain Iwram_CopyWords call, 2 of 783 instructions differ in tla-en.
    copy((void *)BG_PLTT, palette, 128);
  }

  sparks = gMapCellBuffer;

  for (i = 0; i != GROUP_COUNT; i++) {
    // The group's ring of flash points, wider and later further out.
    for (j = 0; j != FLASH_POINTS; j++) {
      struct EffectStep *point;
      s32 radius;
      s32 angle;

      point = &work->particles[i * FLASH_POINTS + j];
      radius = j * 2;
      angle = Random16() & 0xffff;
      point->x = Trig_Sin(angle) * radius;
      point->y = -(Trig_Cos(angle) * radius);
      point->variant = j / 2 + 25;
    }

    // Its sparks, thrown in every direction from the group's centre.
    for (j = 0; j != SPARK_COUNT; j++) {
      struct EffectStep *spark;
      s32 speed;
      s32 angle;

      spark = sparks + i * SPARK_COUNT + j;
      speed = (Random16() & 0x3ff) + 32;
      angle = Random16() & 0xffff;
      if (effect->side == 1)
        spark->x = (originX - GROUP_X(i) + 28) << 16;
      else
        spark->x = (originX + GROUP_X(i) - 28) << 16;
      spark->y = originY << 16;
      spark->velocityX = (Trig_Sin(angle) * speed) >> 6;
      spark->velocityY = -((Trig_Cos(angle) * speed) << 1) >> 6;
      spark->variant = (Random16() & 7) + 32;
    }
  }
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  for (frame = 0; frame != GROUP_COUNT * 8 + 40; frame++) {
    struct CameraWork *camera = Ram_HeapSlots->cameraWork;

    // The strongest variant pans the camera; the next one wipes the canvas
    // at frame 4.
    if (effect->variant == 2 && frame <= 51) {
      if (effect->side == 0)
        camera->scroll += 256;
      else
        camera->scroll -= 256;
    }
    if (effect->variant == 3 && frame == 4)
      Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);

    if (kind == SPARKS_ORANGE || kind == SPARKS_FROM_ACTOR) {
      if (frame == 2) {
        Sound_PlayFar(145);
        Func_081180e8(145);
      }
    } else {
      if (frame == 2)
        Sound_PlayFar(145);
      if (frame == 24)
        Func_081180e8(134);
    }

    for (i = 0; i != GROUP_COUNT; i++) {
      start = i * 8;
      if (frame == start)
        work->shakeFrames = 12;

      // The big flash, for two frames.
      if (frame >= start && frame < start + 2) {
        if (effect->side == 1)
          draw[0](canvas, work->sheet, originX - GROUP_X(i) + 12, originY - 32,
                  32, 64);
        else
          draw[0](canvas, work->sheet, originX + GROUP_X(i) - 44, originY - 32,
                  32, 64);
      }

      // The ring of small flashes, each counting down through six cels.
      if (frame >= start) {
        for (j = 0; j != FLASH_DRAWN; j++) {
          struct EffectStep *point;
          s32 x;
          s32 y;

          point = &work->particles[i * FLASH_POINTS + j];
          y = HI(point->y) + originY;
          if (effect->side == 1)
            x = HI(point->x) + originX - GROUP_X(i) + 28;
          else
            x = HI(point->x) + originX + GROUP_X(i) - 28;
          if (point->variant >= 0 && point->variant < FLASH_FRAMES)
            draw[0](canvas,
                    work->sheet + (Data_08197a1a[point->variant / 3] << 11),
                    x - 16, y - 32, 32, 64);
          if (point->variant > 0)
            point->variant--;
          else
            point->variant = -1;
        }
      }

      // From the sixth frame the sparks fly, bounce off the ground at half
      // speed, and shrink as they die out.
      if (frame > start + 5) {
        s32 gravity;

        if (kind == SPARKS_FROM_ACTOR)
          gravity = -0x1000;
        else
          gravity = 0x1000;
        for (j = 0; j != SPARK_COUNT; j++) {
          struct EffectStep *spark = sparks + i * SPARK_COUNT + j;
          if (spark->variant > 0) {
            BattleEffectKernels_IntegrateVector2(spark, 60, gravity);
            spark->variant--;
            if (spark->y > GROUND_Y) {
              spark->velocityY = -spark->velocityY / 2;
            } else if (spark->x >= 0 && spark->x < 0x7f0000 && spark->y >= 0) {
              s16 y = spark->y >> 16;
              s16 x = spark->x >> 16;
              s32 size = spark->variant / 5 + 1;

              draw[j & 1](canvas, graphics + gData_08197410[size - 1],
                          x - size / 2, y - size, size, size * 2);
            }
          }
        }
      }

      // The targets flash and recoil six frames after the burst.
      for (j = 0; j != effect->count; j++) {
        if (frame == start + 6) {
          Func_0814cd48(effect->actors[j], 7, 5, j, 10);
          Func_08118088(effect->actors[j], 4);
        }
      }
    }

    BattleCamera_Shake(16, 16);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  // Tear down.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}

#define PILLAR_FRAMES 96

// The sparks live in the map cell buffer.

extern char ResourceId_FirePillarSheetA; // directory row 0x146

extern char ResourceId_RedPaletteB; // directory row 0x147

extern u8 Data_08197a20[];    // ☀️ gVolcano_Counts, for each variant
extern s8 Data_08197a23[][4]; // ☀️ gVolcano_Depths
extern u8 Data_08197a2f[];    // ☀️ gVolcano_StartFrames

// One, two or four pillars of fire rise in front of the targets, one after
// another; each burns for the rest of the first 64 frames, scrolling its
// flames upward, throws a burst of sparks when it starts and a few more
// every frame after its sixteenth, and strikes every target as it starts.
void BattleEffect_RunVolcano(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 pillar;
  u8 *sheet;
  struct EffectPosition screen;
  DrawRectangle draw[2];
  struct MotionObject *object;
  struct BattleCamera *camera;
  s32 i;

  // Set up: the canvas layer, the pillar sheet, the particle sprites, the
  // variant's palette and two max blitters, the second mirrored.
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sheet = Ram_HeapSlots->unk64[0];
  BattleEffect_StartCanvasLayer(1);
  REG_BLDALPHA = 0x1010;
  Resource_LoadAndDecompress((s32)&ResourceId_FirePillarSheetA, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);
  if (effect->variant == 0) {
    const void *palette = Resource_GetTableEntry((s32)&ResourceId_RedPaletteB);
    s32 (*copy)(void *destination, const void *source, s32 size) =
        Iwram_CopyWords;

    // The entry held in a local loads ahead of the destination's shift; as a
    // plain Iwram_CopyWords call, 2 of 604 instructions differ in tla-en.
    copy((void *)BG_PLTT, palette, 128);
  } else if (effect->variant == 2) {
    const void *palette =
        Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB);
    s32 (*copy)(void *destination, const void *source, s32 size) =
        Iwram_CopyWords;

    // The entry held in a local loads ahead of the destination's shift; as a
    // plain Iwram_CopyWords call, 2 of 604 instructions differ in tla-en.
    copy((void *)BG_PLTT, palette, 128);
  }
  BattleEffect_LoadBlitter(HEAP_BLITTER, BLITTER_MAX << 4 | COMPOSITE_CLIP_X |
                                             COMPOSITE_CLIP_Y);
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                           BLITTER_MAX << 4 | COMPOSITE_CLIP_X |
                               COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X);
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];

  for (i = 0; i != SPARK_MAX; i++)
    PARTICLES[i].variant = 0;

  // The pillars stand in front of the first target, on its side of the
  // field, at the variant's depths.
  object = GetBattleObjectSlotFar(effect->actors[0])->object;
  for (i = 0; i != 4; i++) {
    struct EffectStep *base = &work->particles[i];
    s32 x;

    x = ((Random16() & 15) + 72) << 16;
    base->x = x;
    base->y = 0;
    base->z = Data_08197a23[effect->variant][i] << 16;
    if (object->x < 0)
      base->x = -x;
  }

  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  for (frame = 0; frame != PILLAR_FRAMES; frame++) {
    // The strongest variant turns the camera for 64 frames.
    camera = Ram_HeapSlots->cameraWork;
    if (effect->variant == 2 && frame < 64) {
      if (effect->side == 0)
        camera->yaw = camera->yaw + 192;
      else
        camera->yaw = (s16)camera->yaw - 192;
    }
    if (frame == 16)
      Func_081180e8(134);
    Render_ResetTransformState();
    Transform_SetLookAt(camera->eye, camera->pos);

    if (frame < 64) {
      for (pillar = 0; pillar != Data_08197a20[effect->variant]; pillar++) {
        EffectPosition_ProjectPoint((s32 *)&work->particles[pillar], &screen);
        screen.x /= 2;
        screen.y -= 8;
        if (frame == Data_08197a2f[pillar])
          Sound_PlayFar(145);

        // Four frames after it starts the pillar burns: its flames scroll
        // up through two copies, and its base flickers between two cels.
        if (frame >= Data_08197a2f[pillar] + 4) {
          s32 scroll = (frame * 16 + pillar * 25) % 104;

          draw[pillar & 1](canvas, work->sheet, screen.x - 17,
                           screen.y - scroll - 104, 34, 104);
          draw[pillar & 1](canvas, work->sheet, screen.x - 17,
                           screen.y - scroll, 34, scroll);
          if (frame & 1) {
            draw[0](canvas, work->sheet + 0xdd0, screen.x - 20, screen.y - 24,
                    20, 37);
            draw[1](canvas, work->sheet + 0xdd0, screen.x, screen.y - 24, 20,
                    37);
          } else {
            draw[0](canvas, work->sheet + 0x10b4, screen.x - 20, screen.y - 24,
                    20, 37);
            draw[1](canvas, work->sheet + 0x10b4, screen.x, screen.y - 24, 20,
                    37);
          }
        }

        // Sparks: 200 as the pillar starts, then four a frame from its
        // sixteenth frame on.
        if (frame == Data_08197a2f[pillar] ||
            frame >= Data_08197a2f[pillar] + 16) {
          s32 spawned = 0;

          for (i = 0; i != SPARK_MAX; i++) {
            struct EffectStep *spark = &PARTICLES[i];

            if (spark->variant == 0) {
              s32 speed;
              s32 angle;

              speed = (Random16() & 0x3ff) + 32;
              angle = (Random16() & 0x7fff) - 0x4000;
              spark->x = screen.x << 8;
              spark->y = (screen.y << 8) + 0x1000;
              spark->z = (Trig_Sin(angle) * speed) >> 15;
              spark->velocityY = -(Trig_Cos(angle) * speed * 2) >> 15;
              spawned++;
              if (frame == Data_08197a2f[pillar]) {
                spark->variant = (Random16() & 7) + 48;
                if (spawned == 200)
                  break;
              } else {
                spark->variant = (Random16() & 7) + 24;
                if (spawned == 4)
                  break;
              }
            }
          }
        }

        // The pillar strikes every target as it starts.
        if (frame == Data_08197a2f[pillar]) {
          work->shakeFrames = 2;
          for (i = 0; i != effect->count; i++) {
            Func_0814cd48(effect->actors[i], 10, 5, i, 8);
            Func_08118088(effect->actors[i], 1);
          }
        }
      }
    }

    // The sparks: 8.8 fixed point, z holding the x speed; they slow, fall,
    // bounce at half speed below line 120, and shrink as they burn out.
    for (i = 0; i != SPARK_MAX; i++) {
      struct EffectStep *spark = &PARTICLES[i];
      s32 life = spark->variant;

      if (life > 0) {
        s32 x;
        s32 y;
        s32 py;

        spark->variant = life - 1;
        x = spark->x += spark->z;
        y = spark->y += spark->velocityY;
        spark->z = spark->z * 60 / 64;
        spark->velocityY = spark->velocityY * 60 / 64 - 16;
        py = y / 256;
        if (py > 120) {
          spark->velocityY = -spark->velocityY / 2;
        } else if (x >= 0 && x >> 8 <= 126 && y >= 0) {
          s32 px = x >> 8;
          s32 size = (life - 17) / 8;

          if (size <= 0)
            size = 1;
          draw[i & 1](canvas, sheet + gData_08197410[size - 1], px - size / 2,
                      py - size, size, size * 2);
        }
      }
    }

    BattleCamera_Shake(16, 16);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  // Tear down.
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

// Battle effects: particle effect entry points.

/// Each entry passes a particle type (color and count) to
/// BattleEffect_RunTargetMotes. effect: effect arguments.
void BattleEffect_RunTaint(struct BattleEffectArgument *effect) {
  if (effect->variant == 0) {
    BattleEffect_RunTargetMotes(effect, 0);
    return;
  }
  BattleEffect_RunTargetMotes(effect, 1);
}

DEFINE_EFFECT_KIND(BattleEffect_RunTargetMotesMode1,
                   BattleEffect_RunTargetMotes, 1)

DEFINE_EFFECT_KIND(BattleEffect_RunDelude, BattleEffect_RunTargetMotes, 2)

DEFINE_EFFECT_KIND(BattleEffect_RunParalyze, BattleEffect_RunTargetMotes, 3)

DEFINE_EFFECT_KIND(BattleEffect_RunHeartrender, BattleEffect_RunTargetMotes, 4)

DEFINE_EFFECT_KIND(BattleEffect_RunMysticFlame, BattleEffect_RunTargetMotes, 5)

DEFINE_EFFECT_KIND(BattleEffect_RunDemonFire, BattleEffect_RunTargetMotes, 7)

DEFINE_EFFECT_KIND(BattleEffect_RunDrown, BattleEffect_RunTargetMotes, 6)

#define MOTE_MAX 512

// The motes live in the map cell buffer, 64 for each target.

// Modes whose motes close in on the target and start again; the others
// drift up and are thrown back at random. Mode 7 closes in once.
#define CONVERGES(mode)                                                        \
  ((mode) <= 1 || (mode) == 4 || (mode) == 5 || (mode) == 6 || (mode) == 7)
#define DECAYS(mode) ((mode) <= 1 || (mode) == 4 || (mode) == 5 || (mode) == 6)

// Where mode 3 unpacks the tornado's glints in the sheet.
enum { RENDER_MODE_TORNADO = 0x2580 };

extern char ResourceId_SmokeSheet;     // directory row 0x161
extern char ResourceId_LimePalette;    // directory row 0x163
extern char ResourceId_MarsDjinnSheet; // directory row 0x150
extern char ResourceId_CyanPalette;    // directory row 0x166
extern char ResourceId_BlastSheet;     // directory row 0x184
extern char ResourceId_PinkBurstSheet; // directory row 0x17f
extern char ResourceId_GlowOrbSheet;   // directory row 0x17b
extern char ResourceId_HeartSheet;     // directory row 0x16e

extern u16 gData_08197486[]; // ☀️ gBattleEffect_GlintCells
extern u8 gData_08197492[];  // ☀️ gBattleEffect_GlintCellWidths
extern u8 gData_08197498[];  // ☀️ gBattleEffect_GlintCellHeights
extern u8 Data_08197a34[];   // ☀️ gTargetMotes_GlintFlips

s32 Func_08196404(s32 slot, s32 widthShift, s32 heightShift, s32 flags,
                  u32 mode); // ☀️ BattleEffect_BuildBlitter

// Motes swirl around each target in turn, eight frames apart, over a
// rippling canvas: smoke, hearts, glowing orbs or vortex motes by mode,
// in the mode's palette; mode 3 adds the tornado's glints, and mode 7 sends
// the caster at the first target for the final blow.
void BattleEffect_RunTargetMotes(struct BattleEffectArgument *effect,
                                 u32 mode) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 total;
  s32 member;
  s32 count;
  struct EffectPosition screen;
  s32 point[3];
  struct EffectPosition base;
  DrawRectangle draw[2];
  void *palette;
  s32 *row;
  s32 i;

  // Set up: the canvas layer and the mode's sheet and palette.
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  count = 16;
  BattleEffect_StartCanvasLayer(0);
  Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet, work->sheet, 1, 1);
  if (mode == 0) {
    palette = Resource_GetTableEntry((s32)&ResourceId_LimePalette);
  } else if (mode == 1) {
    palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
  } else if (mode == 2) {
    palette = Resource_GetTableEntry((s32)&ResourceId_CyanPalette);
  } else if (mode == 3) {
    palette = Resource_GetTableEntry((s32)&ResourceId_BlastSheet);
  } else if (mode == 4) {
    palette = Resource_GetTableEntry((s32)&ResourceId_PinkBurstSheet);
  } else if (mode == 5) {
    Resource_LoadAndDecompress((s32)&ResourceId_GlowOrbSheet, work->sheet, 1,
                               0);
    palette = Resource_GetTableEntry((s32)&ResourceId_GlowOrbSheet);
  } else if (mode == 7) {
    count = 24;
    Resource_LoadAndDecompress((s32)&ResourceId_GlowOrbSheet, work->sheet, 1,
                               0);
    palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
  } else {
    count = 32;
    Resource_LoadAndDecompress((s32)&ResourceId_VortexSheet, work->sheet, 1, 0);
    palette = Resource_GetTableEntry((s32)&ResourceId_VortexSheet);
  }
  CopyWords_((void *)BG_PLTT, palette, 128);
  if (mode == 4)
    Resource_LoadAndDecompress((s32)&ResourceId_HeartSheet, work->sheet, 1, 1);
  if (mode == 3)
    Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet,
                               work->sheet + RENDER_MODE_TORNADO, 1, 0);

  // Scatter the motes around the origin.
  for (i = 0; i != MOTE_MAX; i++) {
    struct EffectStep *mote = &PARTICLES[i];

    if (CONVERGES(mode)) {
      mote->x = ((Random16() & 0xff) - 127) << 15;
      mote->y = ((Random16() & 0xff) - 127) << 14;
      mote->z = ((Random16() & 0xff) - 127) << 15;
    } else {
      mote->x = ((Random16() & 0xff) - 127) << 13;
      mote->y = ((Random16() & 0xff) - 255) << 13;
      mote->z = ((Random16() & 0xff) - 127) << 13;
    }
    mote->variant = 0;
  }

  if (CONVERGES(mode))
    total = effect->count * 8 + 64;
  else
    total = effect->count * 8 + 32;
  if (!(mode <= 1 || mode == 3))
    Task_Add(BattleEffect_ArmBg2AffineHBlankDma, TASK_PASS_FRAME);
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  Sound_PlayFar(142);

  for (frame = 0; frame != total; frame++) {
    struct BattleCamera *camera = Ram_HeapSlots->cameraWork;

    // Mode 7 sends the caster at the first target 46 frames before the
    // end and strikes it 14 frames later; the others begin phase 133.
    if (mode == 7) {
      if (frame == total - 46)
        BattleMotion_ApproachTargetFar(effect->actor, effect->actors[0], 16, 0);
      if (frame == total - 32) {
        Func_081180e8(134);
        Func_08118088(effect->actors[0], 4);
        work->shakeFrames = 8;
      }
    } else if (frame == total - 32) {
      Func_081180e8(133);
    }

    // Ripple the canvas: one horizontal offset for each line.
    row = work->bg2xLines;
    for (i = 0; i != 160; i++)
      *row++ = (0x40000 - (Trig_Sin((frame << 12) + i * 0x800) << 2)) >> 10;

    for (member = 0; member != effect->count; member++) {
      struct MotionObject *object =
          GetBattleObjectSlotFar(effect->actors[member])->object;

      // Mode 3: two tornado glints a frame around the target, each drawn
      // with a blitter of its own, mirrored at random.
      if (mode == 3 && frame > member * 8 && frame < member * 8 + 32) {
        EffectPosition_ProjectScaledBattler(effect->actors[member], &base);

        for (i = 0; i != 2; i++) {
          s32 angle;
          s32 radius;
          s32 x;
          s32 y;
          const u8 *h;
          const u8 *w;

          /* FAKEMATCH(temporary): w and h set inside statement expressions at
           * their first reads; as (w = gData_08197492)[frame & 3] and its twin
           * for h, 4 of 755 instructions differ in tla-en. */
          angle = Random16() & 0xffff;
          radius = (Random16() & 31) + 4;
          x = base.x / 2 + ((Trig_Sin(angle) * radius) >> 17) -
              (({
                 w = gData_08197492;
                 w;
               })[frame & 3] >>
               1);
          /* FAKEMATCH(barrier): an empty asm through w hides that it holds the
           * width table's address, so the draw reads through w's register;
           * without it, 233 of 755 instructions differ in tla-en. */
          __asm__("" : "=r"(w) : "0"(w));
          y = base.y - ((Trig_Cos(angle) * radius) >> 16) -
              (({
                 h = gData_08197498;
                 h;
               })[frame & 3] >>
               1);
          /* FAKEMATCH(barrier): an empty asm through h, the same for the
           * height table; without it, 331 of 755 instructions differ in
           * tla-en. */
          __asm__("" : "=r"(h) : "0"(h));
          Func_08196404(HEAP_BLITTER_ALTERNATE, 7, 7,
                        COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                            Data_08197a34[Random16() & 3],
                        BLITTER_MAX);
          ((DrawRectangle)Ram_HeapSlots->unkb8[1])(
              canvas,
              work->sheet + gData_08197486[frame & 3] + RENDER_MODE_TORNADO, x,
              y + 16, w[frame & 3], h[frame & 3]);
          HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
        }
      }

      // An additive and a max blitter for this target's motes.
      BattleEffect_LoadBlitter(HEAP_BLITTER, BLITTER_ADDITIVE << 4 |
                                                 COMPOSITE_CLIP_X |
                                                 COMPOSITE_CLIP_Y);
      draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
      BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE, BLITTER_MAX << 4 |
                                                           COMPOSITE_CLIP_X |
                                                           COMPOSITE_CLIP_Y);
      draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
      Render_ResetTransformState();
      Transform_SetLookAt(camera->eye, camera->pos);
      point[0] = object->x;
      point[1] = 160 << 13;
      point[2] = object->z;
      SceneTransform_ApplyPosition(point);

      // From its turn on, the target's motes turn around it.
      if (frame > member * 8) {
        SceneTransform_ApplyYaw(frame << 9);
        if (mode <= 1 || mode == 4)
          SceneTransform_ApplyPitch(frame << 9);
        for (i = 0; i != count; i++) {
          struct EffectStep *mote = &PARTICLES[member * 64 + i];
          s32 distance;

          if (frame > member * 8 + i) {
            s32 xx = (mote->x >> 8) * (mote->x >> 8);
            s32 yy = (mote->y >> 8) * (mote->y >> 8);
            s32 zz = (mote->z >> 8) * (mote->z >> 8);

            distance = Iwram_Sqrt(xx + yy + zz) >> 9;
            if (distance != 0 && mote->variant <= 23) {
              s32 cel = mote->variant / 4;

              EffectPosition_ProjectPoint((s32 *)mote, &screen);
              screen.x >>= 1;
              if (mode == 5 || mode == 7)
                draw[1](canvas, work->sheet + cel * 1600, screen.x - 20,
                        screen.y - 20, 40, 40);
              else if (mode == 6)
                draw[1](canvas, work->sheet + 0xc00, screen.x - 6,
                        screen.y - 12, 12, 24);
              else if (mode == 4)
                draw[1](canvas, work->sheet, screen.x - 11, screen.y - 21, 22,
                        42);
              else
                draw[(i & 3) != 0](canvas, work->sheet + cel * 1152,
                                   screen.x - 12, screen.y - 24, 24, 48);

              // Close in by a step of the distance, or drift up.
              if (DECAYS(mode)) {
                mote->x -= mote->x / distance;
                mote->y -= mote->y / distance;
                mote->z -= mote->z / distance;
              } else {
                mote->y += 0x10000;
              }
              mote->variant++;
              if (mote->variant == 24) {
                if (DECAYS(mode)) {
                  mote->variant = 0;
                } else {
                  mote->x = ((Random16() & 0xff) - 127) << 13;
                  mote->y = ((Random16() & 0xff) - 255) << 12;
                  mote->z = ((Random16() & 0xff) - 127) << 13;
                }
              }
            }
          }
        }
      }
      HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
      HeapSlot_Release(HEAP_BLITTER);

      // The target flashes 16 frames into its turn, until the end.
      if (frame == member * 8 + 16) {
        s32 delay = total - frame;

        if (delay > 31)
          delay = 31;
        Func_0814cd48(effect->actors[member], 7, 5, member, delay);
      }
    }

    BattleCamera_Shake(16, 16);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  // Tear down.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  if (!(mode <= 1 || mode == 3))
    Task_Remove(BattleEffect_ArmBg2AffineHBlankDma);
  BattleEffect_EndCanvasLayer();
}

#undef GROUND_Y

#define SHEEP_COUNT 12
#define SHEEP_FRAMES 98
#define GROUND_Y (92 << 16)

extern char ResourceId_SheepCardSheet; // directory row 0xe3

void Func_0815e22c(struct EffectPosition *middle); // the targets' middle

// A flock of sheep cards drops in over the targets, one every four frames,
// and bounces on the ground, tumbling through eight cels; at frame 80 the
// next phase begins, and each target turns back to its own palette in turn.
void BattleEffect_RunSleep(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  struct EffectPosition middle;
  s32 record[3];
  struct EffectStep *one;
  struct EffectStep *sheep;
  s32 i;
  s32 frame;
  DrawRectangle drawB;
  DrawRectangle drawA;
  s32 scroll;
  s32 member;

  // Set up: the canvas layer, the sheep sheet and a transparent blitter
  // pair, the second mirrored.
  sheep = (struct EffectStep *)Ram_MapCellBuffer;
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  scroll = 0;
  BattleEffect_StartCanvasLayer(1);
  REG_BG2PA = 0x100;
  REG_BLDCNT = scroll;
  Resource_LoadAndDecompress((s32)&ResourceId_SheepCardSheet, work->sheet, 1,
                             1);
  BattleEffect_LoadBlitter(HEAP_BLITTER, BLITTER_TRANSPARENT << 4 |
                                             COMPOSITE_CLIP_X |
                                             COMPOSITE_CLIP_Y);
  drawA = (DrawRectangle)Ram_HeapSlots->unk64[1];
  BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                           BLITTER_TRANSPARENT << 4 | COMPOSITE_CLIP_X |
                               COMPOSITE_CLIP_Y | COMPOSITE_FLIP);
  drawB = (DrawRectangle)Ram_HeapSlots->unkb8[1];

  // The sheep wait above the screen with a random spin and starting cel.
  for (i = 0; i != 32; i++) {
    one = &sheep[i];
    one->x = ((Random16() & 63) + 32) << 16;
    one->y = -0x200000;
    one->velocityY = Random16() & 0;
    one->z = Random16() & 3;
    one->variant = Random16() & 255;
  }

  // Centre the canvas on the targets; what BG2 cannot reach moves the sheep.
  Func_0815e22c(&middle);
  middle.x -= 64;
  if (middle.x < 0) {
    scroll = middle.x;
    middle.x = 0;
  } else if (middle.x > 112) {
    scroll = middle.x - 112;
    middle.x = 112;
  }
  REG_BG2X = -middle.x << 8;
  work->transferMode = 1;
  work->transferValue = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  Sound_PlayFar(142);

  for (frame = 0; frame != SHEEP_FRAMES; frame++) {
    struct BattleCamera *camera;

    camera = Ram_HeapSlots->cameraWork;
    if (frame == 80)
      Func_081180e8(0);

    // Place each target, and from frame 64 restore its palette, one
    // target every 16 frames.
    for (member = 0; member != effect->count; member++) {
      struct MotionObject *object;

      object = GetBattleObjectSlotFar(effect->actors[member])->object;
      Render_ResetTransformState();
      Transform_SetLookAt(camera->eye, camera->pos);
      record[0] = object->x;
      record[1] = 160 << 14;
      record[2] = object->z;
      SceneTransform_ApplyPosition(record);
      if (frame == member * 16 + 64)
        Func_0814cd48(effect->actors[member], 0, 5, -1, 0);
    }

    // The sheep fall and bounce, each spinning through its eight cels:
    // the first four drawn as they are, the last four mirrored.
    one = sheep;
    for (i = 0; i != SHEEP_COUNT; i++, one++) {
      if (frame > i * 4 && one->y <= 0x7fffff) {
        s32 cel;

        cel = (one->variant / 16) & 7;
        if (cel < 4)
          drawA(canvas, work->sheet + (cel << 10) + 0x800,
                scroll + (one->x >> 16) - 16, (one->y >> 16) - 16, 32, 32);
        else
          drawB(canvas, work->sheet + (cel << 10) - 0x800,
                scroll + (one->x >> 16) - 16, (one->y >> 16) - 16, 32, 32);
        one->y += one->velocityY;
        one->velocityY += 0x2000;
        one->variant += one->z;
        if (one->y > GROUND_Y && one->velocityY == 0) {
          s32 speed = one->velocityY + 1;
          one->y = GROUND_Y;
          one->z += 4;
          one->velocityY = -speed / 2;
        }
      }
    }
    work->transferPending = 1;
    wait(1);
  }

  // Tear down.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}

#define MARKER_FRAMES 72 // how long each marker stays
#define MARKER_STAGGER 16
#define MARKER_CELS 9

extern char ResourceId_FlameColumnSheet;

// As in The Broken Seal's twin, which loads the same sheet: a small flame
// burns above each target in turn, MARKER_STAGGER frames apart, for up to
// MARKER_FRAMES frames each, over a marker at its feet.
void BattleEffect_RunCurse(struct BattleEffectArgument *effect) {
  void *canvas;
  struct BattleEffectWork *work;
  // Unused: the second blitter gives the stack frame its 4 bytes (as one, 154
  // of 222 differ in tla-en); the family's pair, as in The Broken Seal.
  DrawRectangle draw[2];
  struct EffectStep *marker;
  s32 i;
  s32 frame;
  s32 start;
  s32 cel;
  s32 y;
  struct EffectPosition pos;

  // Set up: the canvas layer, unscaled; for side 1 the canvas shows the
  // right half of the field.
  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  BattleEffect_StartCanvasLayer(1);
  REG_BG2PA = 0x100;
  REG_BLDCNT = 0;
  Resource_LoadAndDecompress((s32)&ResourceId_FlameColumnSheet, work->sheet, 1,
                             1);
  if (effect->side == 1) {
    REG_BG2X = -0x7000;
  }
  BattleEffect_LoadBlitter(HEAP_BLITTER, BLITTER_TRANSPARENT << 4 |
                                             COMPOSITE_CLIP_X |
                                             COMPOSITE_CLIP_Y);
  work->transferMode = 1;
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  work->transferValue = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  // Each flame starts at a random point in its turn.
  for (i = 0; i != effect->count; i++) {
    work->particles[i].variant = Random16() & 63;
  }

  frame = 0;
  while (frame != (effect->count << 5) + 32) {
    if (frame == 32) {
      Func_081180e8(0);
    }
    i = 0;
    if (effect->count != 0) {
      do {
        start = i * MARKER_STAGGER;
        if (frame == start) {
          Sound_PlayFar(143);
        }
        if (frame >= start && frame < start + MARKER_FRAMES) {
          // The marker at the target's feet, then the flame, six frames a cel.
          marker = &work->particles[i];
          EffectPosition_ProjectScaledBattler(effect->actors[i], &pos);
          if (effect->side == 1) {
            pos.x -= 112;
          }
          y = pos.y;
          pos.y = y - 16;
          draw[0](canvas, work->sheet + 1728, pos.x - 8, y - 20, 16, 20);
          // DEAD: always true here; the ROM keeps the test.
          if (frame >= start) {
            cel = ((frame - start) + marker->variant) / 6 % MARKER_CELS;
            draw[0](canvas, work->sheet + cel * 192, pos.x - 8, pos.y - 16, 16,
                    12);
          }
        }
        i++;
      } while (i != effect->count);
    }
    work->transferPending = 1;
    wait(1);
    frame += 1;
  }

  // Tear down.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}

// The sparks live in the map cell buffer.

extern char ResourceId_MercuryDjinnSmallSheet; // directory row 0x155

extern char ResourceId_SparkleDots; // directory row 0x17e

extern u16 Data_0819744c[]; // ☀️ gBattleEffect_PuffCells
extern u8 Data_0819745e[];  // ☀️ gBattleEffect_PuffSizes

void Func_080150e4(s32 angle); // ☀️ SceneTransform_ApplyRoll

// A Djinn rides a swarm of sparks from the caster to the first target: 32
// sparks spin around it in four planes while it travels, then it hovers
// over the targets while puffs of sparkle gather on each in turn.
void Func_0814b350(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 member;
  u8 *sheet;
  DrawRectangle *draw;
  struct MotionObject *actor;
  struct MotionObject *goal;
  s32 frame;
  s32 i;
  s32 position[3];
  s32 target[3];
  s32 delta[3];
  s32 rider[3];
  struct EffectPosition screen;
  s32 point[3];
  struct EffectPosition view;
  DrawRectangle callbacks[2];

  // Set up: the canvas layer, the Djinn and the particle sprites, and the
  // blitter pair for the side the Djinn faces.
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sheet = Ram_HeapSlots->unk64[0];
  BattleEffect_StartCanvasLayer(0);
  Resource_LoadAndDecompress((s32)&ResourceId_MercuryDjinnSmallSheet,
                             work->sheet, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);
  BattleEffect_FetchRectangleBlitters(effect->side ^ 1, draw = callbacks);
  actor = GetBattleObjectSlotFar(effect->actor)->object;
  goal = GetBattleObjectSlotFar(effect->actors[0])->object;

  // The swarm: 64 sparks thrown out level from the Djinn.
  for (i = 0; i != 64; i++) {
    struct EffectStep *spark = &PARTICLES[i];
    s32 heading = Random16() & 0xffff;
    s32 speed = (Random16() & 0xff) + 128;

    spark->x = 0;
    spark->y = ((Random16() & 31) + 20) << 16;
    spark->z = 0;
    spark->velocityX = (Trig_Sin(heading) * speed) >> 5;
    spark->velocityY = 0;
    spark->velocityZ = (Trig_Cos(heading) * speed) >> 5;
    spark->variant = 0;
  }
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  // The Djinn travels from the caster to above the first target in 40
  // frames.
  position[0] = actor->x;
  position[1] = 0;
  position[2] = actor->z;
  target[0] = goal->x;
  target[1] = 180 << 15;
  target[2] = 0;
  delta[0] = (target[0] - position[0]) / 40;
  delta[1] = (target[1] - position[1]) / 40;
  delta[2] = (target[2] - position[2]) / 40;

  for (frame = 0; frame != 98; frame++) {
    struct BattleCamera *camera = Ram_HeapSlots->cameraWork;

    if (frame == 8)
      Sound_PlayFar(212);
    if (frame == 80)
      Sound_PlayFar(142);
    Render_ResetTransformState();
    Transform_SetLookAt(camera->eye, camera->pos);
    if (frame >= 30 && frame < 70) {
      position[0] += delta[0];
      position[1] += delta[1];
      position[2] += delta[2];
    }
    SceneTransform_ApplyPosition(position);

    // The caster flashes for the first 24 frames.
    if (frame == 0)
      Func_0814cd48(effect->actor, 7, -1, -1, 0);
    if (frame == 24)
      Func_0814cd48(effect->actor, 0, -1, -1, 0);

    // Each spark spins in one of four planes, smaller further away, and
    // after 30 frames is pulled back toward the Djinn.
    for (i = 0; i != 32; i++) {
      struct EffectStep *spark = &PARTICLES[i];

      if (frame > i && spark->variant == 0) {
        s32 size;

        Transform_Push();
        switch (i & 3) {
        case 0:
          SceneTransform_ApplyYaw(frame * (i * 32 + 256));
          break;
        case 1:
          SceneTransform_ApplyPitch(-frame * (i * 32 + 256));
          break;
        case 2:
          Func_080150e4(-frame * (i * 32 + 256));
          break;
        case 3:
          SceneTransform_ApplyPitch(-frame * (i * 32 + 256));
          Func_080150e4(-frame * (i * 32 + 256));
          break;
        }
        EffectPosition_ProjectPoint((s32 *)spark, &screen);
        screen.x >>= 1;
        Transform_Pop();
        if (screen.depth < 250)
          screen.depth = 250;
        if (screen.depth > 634)
          screen.depth = 634;
        size = 9 - (screen.depth - 250) / 64;
        callbacks[0](canvas, sheet + gData_08197410[size - 1],
                     screen.x - size / 2, screen.y - size, size, size * 2);
        BattleEffectKernels_IntegrateVector3(spark, 60, 0);
        if (frame > i + 30) {
          s32 dx = -spark->x >> 8;
          s32 dy = -spark->y >> 8;
          s32 dz = -spark->z >> 8;

          spark->velocityX += dx;
          spark->velocityY += dy;
          spark->velocityZ += dz;
        }
      }
    }

    // The Djinn bobs over the target for the last 15 frames.
    if (frame > 82) {
      rider[0] = 0;
      rider[1] = Trig_Sin(frame << 10) << 2;
      rider[2] = 0;
      EffectPosition_ProjectPoint(rider, &screen);
      screen.x >>= 1;
      draw[1](canvas, work->sheet, screen.x - 10, screen.y - 17, 20, 34);
    }
    work->transferPending = 1;
    wait(1);
  }

  // The second part: 64 puffs of sparkle for each target.
  for (i = 0; i != 512; i++) {
    struct EffectStep *spark = &PARTICLES[i];

    spark->x = ((Random16() & 0xff) - 127) << 15;
    spark->y = ((Random16() & 0x7f) + 64) << 15;
    spark->z = ((Random16() & 0xff) - 127) << 15;
    spark->variant = 0;
  }
  Resource_LoadAndDecompress((s32)&ResourceId_SparkleDots, sheet, 0, 0);

  for (frame = 0; frame != effect->count * 8 + 72; frame++) {
    struct BattleCamera *camera = Ram_HeapSlots->cameraWork;

    // The Djinn sways over the targets, then rises away for the last 32
    // frames.
    Render_ResetTransformState();
    Transform_SetLookAt(camera->eye, camera->pos);
    if (frame >= effect->count * 8 + 40)
      target[1] += 0x40000;
    point[0] = target[0];
    point[1] = target[1];
    point[2] = target[2] + Trig_Sin(frame << 11) * 40;
    EffectPosition_ProjectPoint(point, &view);
    view.x >>= 1;
    callbacks[0](canvas, work->sheet, view.x - 10, view.y - 17, 20, 34);

    // Each target in turn, eight frames apart: its puffs close in around
    // it, and it flashes from 40 to 64 frames into its turn.
    for (member = 0; member != effect->count; member++) {
      struct MotionObject *object =
          GetBattleObjectSlotFar(effect->actors[member])->object;
      s32 start = member * 8;

      Render_ResetTransformState();
      Transform_SetLookAt(camera->eye, camera->pos);
      position[0] = object->x;
      position[1] = 160 << 14;
      position[2] = object->z;
      SceneTransform_ApplyPosition(position);
      if (frame == start + 30)
        Sound_PlayFar(126);
      if (frame == start + 40)
        Func_0814cd48(effect->actors[member], 7, -1, -1, 0);
      if (frame == start + 64)
        Func_0814cd48(effect->actors[member], 0, -1, -1, 0);
      if (frame > start) {
        SceneTransform_ApplyYaw((frame - start) << 9);
        for (i = 0; i != 64; i++) {
          struct EffectStep *spark = &PARTICLES[member * 64 + i];

          if (frame > start + i / 2) {
            s32 xx = (spark->x >> 8) * (spark->x >> 8);
            s32 yy = (spark->y >> 8) * (spark->y >> 8);
            s32 zz = (spark->z >> 8) * (spark->z >> 8);
            s32 distance = Iwram_Sqrt(xx + yy + zz) >> 9;

            if (distance != 0) {
              s32 cel;

              EffectPosition_ProjectPoint((s32 *)spark, &view);
              view.x >>= 1;
              if (view.depth < 314)
                view.depth = 314;
              if (view.depth > 634)
                view.depth = 634;
              // DEAD: BattleEffect_RunHealingMotes' size by depth, copied with
              // its code; the next line sets cel for real.
              cel = 3 - (view.depth - 314) / 128;
              cel = (i * 4 + frame) % 9;
              {
                u8 *cell = sheet + Data_0819744c[cel];
                u32 edge = Data_0819745e[cel];
                u32 half = edge >> 1;

                draw[1](canvas, cell, view.x - half, view.y - half, edge, edge);
              }
              spark->x -= spark->x / distance;
              spark->y -= spark->y / distance;
              spark->z -= spark->z / distance;
            }
          }
        }
      }
    }
    work->transferPending = 1;
    wait(1);
  }

  // Tear down.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}

void Func_0814b9e8(struct BattleEffectArgument *effect, s32 mode);

/// Abilities: Balm (kind 230).
void BattleEffect_RunBalm(struct BattleEffectArgument *effect) {
  Func_0814b9e8(effect, 0);
}

/// Abilities: Tinder (kind 231).
void BattleEffect_RunTinder(struct BattleEffectArgument *effect) {
  Func_0814b9e8(effect, 1);
}

/// Abilities: Phoenix, Regen Dance (kind 436).
void BattleEffect_RunPhoenix(struct BattleEffectArgument *effect) {
  Func_0814b9e8(effect, 2);
}

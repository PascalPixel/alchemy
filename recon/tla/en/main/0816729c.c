// 2026-11-05: differ 104 of 2680 (tla-en). The flash reads *src per channel, no u16 c;
// that is item (3) below closed, and the count now equal.
// 2026-11-05: differ 137 of 2680 (tla-en). With the twin's two dead branches
// and a wrapper on the first palette copy. Left: scheduling rows and pool
// offsets that follow them, across the draft.
// Draft, differ 258 of 2680, skeleton gap 41/39 (tla-en, 2026-11-01)
// BattleEffect_RunJudgment, the same calls and control flow as the listing; its
// twin is The Broken Seal's judgment.c. Re-measured under the 1 November
// compiler: the stale notes are gone, and the root causes below are what is
// left. The frame is the listing's 188.
// Closed this round, for whoever meets the same shapes: the ring locals
// declared top, bottom, left, right (local-alloc's ties, 140 instructions);
// Ram_MapCellBuffer, the constant, at every site but the top's cells (it lets
// the frame-0 cell loop reduce its giv); the debris loop's p = &particles[i]
// inside the loop; height / 2 with the shard tables' reads left in place;
// shade's default set before model->texture; u16 c in the flash loop, which is
// what pools the 31 mask; the twin's sx = 8 in place of a cast through u32.
// DRAW_ALT and FILL are do { } while (0) macros that every use matches through
// (S2): their loop notes stop CSE sharing work->sheet between draws and are
// sched2's barrier around each fill. Their review is open: the round's brief
// forbade one-pass blocks, and they are kept only on S2's macro wording. As plain
// statements the draft measures differ 462, skeleton 96/88.
// What is left, with its root first. (1) The rings, about 90 instructions: the
// listing reloads cx twice between right and left, so reload saw a label there.
// The twin's two tagged FAKEMATCH(dead) branches (if (dx > cx) left = 0; before
// each left = cx - dx) take this draft to differ 171, skeleton 37/38; they are
// not added, since this round adds no device. A plain form that leaves that
// label would close it. (2) First palette load, about 30: the listing's 512 is
// one shared pseudo with no hard register, rebuilt into r2 for the call and by
// reload into r1 after it for pal += 512, and that extra reload is why the next
// reloads start one register on. Copy-then-add gives the pseudo r5 and shifts
// everything; add-before-copy, the other three sites' form, is kept as the
// nearer. A one-pass copy macro and a one-constant size local both lose. (3)
// The flash's red channel: u16 c gives the pooled mask for green and blue but
// ldrsh and a 31 << 16 mask for red, where the listing has ldrh and movs 31;
// s32 c gives the reverse. r = 31; r &= c, bitfields and several u16 forms all
// lose. (4) DRAW_ALT's LoadBlitter arguments come out r0 first after the
// loop-begin note, and draw[0]'s pointer load one slot early; moving the
// do { } to the load, the draw or the release alone costs more than it gains.
// (5) Scheduling ties: the queue loop's value store, the first model's constant
// (r0 here, r1 in the listing, so shade takes r2).
// loop.c's dump shows the second pass moves a constant when
// 9 * savings * life >= the loop's real insns; that is how the 24 rebuilt across
// the strike loop was traced.
// Adopting needs names in resource_directory.s for the ResourceId_ symbols
// declared below that it does not yet name.
// The native's arrays, from the top of its frame: anchor (sp 176), pos (164),
// the swords' placement (148), the falling objects' placement (132, drift
// here), req (124), the first texture (116), the glare's texture (108) and
// scale (100).

#include "battle_effect.h"
#include "battle_effect_work.h"
#include "dma.h"
#include "effect_step.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "heap_release.h"
#include "ram_buffer.h"
#include "types.h"

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
void EffectPosition_ProjectBattler(s32 id, struct EffectPosition *out);
void BattleEffect_SpawnObjects(s32 count, s32 resource,
                               s32 mode); // SpawnObjects
void BattleEffect_SelectLivingTargets(
    struct BattleEffectArgument *effect); // SelectLivingTargets
void Func_080150e4(s32 angle);            // SceneTransform_ApplyRoll
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

struct ResourceTableEntry {
  u16 unk0;
  u16 tile;
};

struct JudgmentObject {
  u8 unk0[9];
  u8 flagsLow : 2;
  u8 priority : 2;
  u8 flagsHigh : 4;
  u8 unk0a[6];
  u8 resource;
  u8 unk11[9];
  u8 unk1a;
};

extern char ResourceId_VioletSkyBackdrop;
extern char ResourceId_AerialViewBackdrop;
extern char ResourceId_BeachBackdrop;
extern char ResourceId_ForestBackdrop;
extern char ResourceId_WingedKnightATiles;
extern char ResourceId_WingedKnightBTiles;
extern char ResourceId_WingedKnightCTiles;
extern char ResourceId_PinkBurstSheet;
extern char ResourceId_TornadoSheet;
extern char ResourceId_Data0C2;
extern char ResourceId_Data134;
extern char ResourceId_BlueCrescentSheet;
extern char ResourceId_ExplosionSheet;
extern char ResourceId_OverlayImageA;

extern struct ResourceTableEntry gResourceTableEntries[];
extern u8 gData_02014000[];
extern const u16 gData_08197410[]; // gBattleEffect_MoteCells
struct JudgmentScale {
  s32 x;
  s32 y;
};

extern const struct JudgmentScale Data_08196e74;
extern const u8 Data_08198ab4[];
extern const u8 Data_08198ace[];
extern const u8 Data_08198ad4[];
extern const u16 Data_08198ac2[];
extern const u8 gData_08199364[];
extern const u8 gData_08199210[];
extern u32 gCameraSceneParameters[];

struct JudgmentObject *Func_0815b290(s32 width, s32 height, u32 shape,
                                     s32 mode);
void *ResourceObject_CreateFar(s32 resource);
void ResourceObject_ReleaseFar(void *object);
void Animation_ApplyChildArgumentFar(void *object, s32 argument);
void Palette_LoadBasicObjColors(void);
void BattleStaging_ConfigurePaletteFadeFar(s32 mode, s32 background, s32 speed);
void Graphics_LoadGreyRamp(void);
void Func_0814cca8(void);
void Func_0813ba50(void);
void Func_08163c2c(s32 mode, s32 value);
void Resource_DecodeType01(const void *source, void *destination);
void BattleBackground_LoadFar(s32 layer, s32 background, s32 palette);

#define SLOT_ALT 188 // unkb8[1]
#define FILL(dst, size, value)                                                 \
  do {                                                                         \
    Iwram_FillWords(dst, size, value);                                         \
  } while (0)
#define DRAW_ALT(mode, sheet, x, y, w, h)                                      \
  do {                                                                         \
    BattleEffect_LoadBlitter(SLOT_ALT, mode);                                  \
    ((DrawRectangle)Ram_HeapSlots->unkb8[1])(canvas, sheet, x, y, w, h);       \
    HeapSlot_Release(SLOT_ALT);                                                \
  } while (0)
#define OBJ_TILES(obj)                                                         \
  (0x06010000 + gResourceTableEntries[(obj)->resource].tile)

static inline s32 CopyWords(void *destination, const void *source, s32 size) {
  /* FAKEMATCH(wrapper): the call through this inline; as a direct call to
   * Iwram_CopyWords, 186 of 2680 instructions differ in tla-en. */
  return Iwram_CopyWords(destination, source, size);
}

// Judgment: the sky gathers sixteen tiles of light that fold into a sigil,
// then the sigil's beam strikes and the ground erupts in a ring of
// shockwaves and debris.
void BattleEffect_RunJudgment(struct BattleEffectArgument *effect) {
  s32 anchor[3];
  struct EffectPosition pos;
  s32 place[4];
  s32 drift[4];
  struct JudgmentScale req;
  struct ModelTexture texture;
  struct ModelTexture glare;
  s32 scale[2];
  u8 *canvas;
  struct BattleEffectWork *work;
  s32 *transition;
  DrawRectangle draw[2];
  s32 frame;
  u8 *sprites;
  s32 depth;
  s32 zoom;
  s32 addR, addG, addB;
  s32 cx, cy, squash, sx, ring, rings, shift;
  s32 i;
  u8 *cells;
  u8 *pal;

  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  transition = Ram_HeapSlots->transitionWork;
  sprites = Ram_HeapSlots->unk64[0];
  cells = Ram_MapCellBuffer;
  BattleEffect_StartCanvasLayer(0x2000);
  REG_BG2PA = 0x100;
  Func_0813ba50();
  ((volatile u16 *)BG_PLTT)[0] = 0;
  ((volatile u16 *)BG_PLTT)[1] = 0;
  BattleEffect_LoadBlitter(SLOT_BLITTER, 3);
  work->transferMode = 0;
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  Func_08163c2c(0, 0);
  BattleEffect_SelectLivingTargets(work->effect);
  BattleBackground_LoadFar(1, (s32)&ResourceId_VioletSkyBackdrop, 0);

  pal = Resource_GetTableEntry((s32)&ResourceId_WingedKnightATiles);
  CopyWords((void *)OBJ_PLTT, pal, 512);
  pal += 512;
  Resource_DecodeType01(pal, cells);
  for (i = 0; i != 16; i++) {
    struct JudgmentObject *obj = Func_0815b290(32, 32, 0x80002000, 0);

    obj->priority = 2;
    work->objects[i] = obj;
    Iwram_CopyWords((void *)OBJ_TILES(obj), cells + i * 0x400, 0x400);
  }
  gCameraSceneParameters[4] = 240;
  wait(1);
  transition[4] = 1;
  gBgScroll[1].x = 0;
  Func_08163c2c(0, 1);
  REG_DISPCNT = 0x7741;
  REG_BG2PA = 0x80;
  REG_BLDALPHA = 0x1010;
  REG_BLDCNT = 0x3f44;
  depth = -0x400000;
  zoom = 0x80000;
  Resource_LoadAndDecompress((s32)&ResourceId_PinkBurstSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, work->sheet + 0x600,
                             1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C2, gData_02014000, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
  pal = Resource_GetTableEntry((s32)&ResourceId_BlueCrescentSheet);
  Iwram_CopyWords((void *)BG_PLTT, pal, 128);
  work->transferMode = 2;
  work->transferValue = 50;
  REG_BG2CNT = 0x784;

  // The gathering: motes rise, the tiles fold in and flash.
  frame = 0;
  do {
    if (frame == 0) {
      struct EffectStep *p = work->particles;

      for (i = 0; i != 64; i++, p++) {
        p->x = (Random16() & 15) + 72;
        p->velocityX = Random16() & 0xffff;
        p->velocityY = Random16() & 0xffff;
        p->velocityY = Random16() & 0xffff;
      }
      pal = Resource_GetTableEntry((s32)&ResourceId_WingedKnightATiles);
      {
        u8 *palette = pal;

        pal += 512;
        Iwram_CopyWords((void *)OBJ_PLTT, palette, 512);
      }
      Resource_DecodeType01(pal, Ram_MapCellBuffer);
      for (i = 0; i != 16; i++) {
        struct JudgmentObject *obj = work->objects[i];

        Iwram_CopyWords((void *)OBJ_TILES(obj), Ram_MapCellBuffer + i * 0x400,
                        0x400);
      }
      depth = -0x400000;
      zoom = 0x80000;
    }
    if (frame == 143) {
      FILL(canvas, 0x4000, 0x2a2a2a2a);
      Sound_PlayFar(145);
    }
    if (frame == 80) {
      Sound_PlayFar(142);
    }
    if (frame == 72) {
      work->fadeFrames = 24;
      work->fadeStep = 0;
    }

    anchor[2] = 0;
    anchor[1] = 0;
    {
      struct EffectStep *p = work->particles;

      for (i = 0; i != 64; i++, p++) {
        s32 size;

        if (frame > i / 4 + 80 && p->x > 0) {
          Render_ResetTransformState();
          Func_080150e4(p->velocityY);
          SceneTransform_ApplyPitch(p->velocityX);
          SceneTransform_ApplyYaw(p->velocityY + (frame << 9));
          anchor[0] = p->x;
          p->x -= 2;
          if (p->x < 0) {
            p->x = 0;
          }
          EffectPosition_ProjectPoint(anchor, &pos);
          if (pos.depth < -60) {
            pos.depth = -60;
          }
          if (pos.depth > 60) {
            pos.depth = 60;
          }
          pos.depth += 60;
          size = pos.depth / 20 + 2;
          pos.x += 56;
          pos.y += 96;
          draw[0](canvas, sprites + gData_08197410[size - 1], pos.x - size / 2,
                  pos.y - size, size, size * 2);
        }
      }
    }

    req = Data_08196e74;
    place[3] = 0;
    place[1] = 0;
    for (i = 0; i != 16; i++) {
      if (frame > 75 && (i == 8 || i == 12 || i == 15)) {
        continue;
      }
      place[0] = ((i % 4) << 21) + (152 << 15);
      place[2] = ((i / 4) << 21) + depth;
      Render_ApplyProjectedPlacementFar(work->objects[i], place, (s32 *)&req,
                                        0);
    }
    depth += zoom;
    if (frame <= 47) {
      zoom -= 0x4000;
    }
    if (frame > 32) {
      zoom = zoom * 60 / 64;
    }
    if (frame == 144) {
      zoom = -0x80000;
    }
    if (frame == 146) {
      zoom = 0x10000;
    }

    if (frame == 72) {
      pal = Resource_GetTableEntry((s32)&ResourceId_WingedKnightBTiles);
      {
        u8 *palette = pal;

        pal += 512;
        Iwram_CopyWords((void *)OBJ_PLTT, palette, 512);
      }
      Resource_DecodeType01(pal, Ram_MapCellBuffer);
      for (i = 0; i != 16; i++) {
        IO_WRITE_QUEUE_PUSH(
            OBJ_TILES((struct JudgmentObject *)work->objects[i]),
            (u32)(Ram_MapCellBuffer + i * 0x400), 0x84000100);
      }
    }
    if (frame == 76) {
      pal = Resource_GetTableEntry((s32)&ResourceId_WingedKnightCTiles);
      {
        u8 *palette = pal;

        pal += 512;
        Iwram_CopyWords((void *)OBJ_PLTT, palette, 512);
      }
      Resource_DecodeType01(pal, Ram_MapCellBuffer);
      for (i = 0; i != 13; i++) {
        IO_WRITE_QUEUE_PUSH(
            OBJ_TILES((struct JudgmentObject *)work->objects[Data_08198ab4[i]]),
            (u32)(Ram_MapCellBuffer + i * 0x400), 0x84000100);
      }
    }

    if ((u32)(frame - 116) <= 27) {
      s32 n = frame * 3 - 346;
      if (n > 80) {
        n = 80;
      } else {
        Func_0815b434(work->sheet + 0x1256, n, 0x10000);
      }
      Func_0818caa8(work->sheet + 0x1256, 60, 80, n);
      if ((u32)(frame - 116) <= 27) {
        s32 size = (frame - 116) / 3 + 16;

        if (size > 32) {
          size = 32;
        }
        for (i = 0; i != 6; i++) {
          s32 k = i & 3;
          s32 a = Random16() & 0xffff;
          s32 x = ((size * Trig_Sin(a)) >> 16) - (Data_08198ace[k] >> 1);
          s32 y = ((size * Trig_Cos(a)) >> 16) - (Data_08198ad4[k] / 2);

          draw[0](canvas, work->sheet + Data_08198ac2[k], x + 60, y + 80,
                  Data_08198ace[k], Data_08198ad4[k]);
        }
      }
    }

    {
      s32 flag = 0;

      if ((u32)(frame - 108) <= 34) {
        s32 t = (frame - 108) / 2;

        addR = t / 4;
        addG = t / 2;
        addB = t;
        flag = 1;
      }
      if (frame == 143) {
        addB = addG = addR = 31;
        flag = 1;
      }
      if ((u32)(frame - 144) <= 8) {
        addB = addR = addG = 304 - frame * 2;
        flag = 1;
      }
      if (flag == 1) {
        u16 *src = Resource_GetTableEntry((s32)&ResourceId_WingedKnightCTiles);
        volatile u16 *dst = (volatile u16 *)OBJ_PLTT;

        for (i = 0; i != 256; i++, src++, dst++) {
          s32 r = (*src & 31) + addR;
          s32 g = ((*src >> 5) & 31) + addG;
          s32 b = ((*src >> 10) & 31) + addB;

          if (r > 31)
            r = 31;
          if (g > 31)
            g = 31;
          if (b > 31)
            b = 31;
          if (r < 0)
            r = 0;
          if (g < 0)
            g = 0;
          if (b < 0)
            b = 0;
          *dst = b << 10 | g << 5 | r;
        }
      }
    }

    if (frame > 143) {
      s32 y = frame * 16 - 2272;

      DRAW_ALT(3, work->sheet, 36, y, 24, 64);
      DRAW_ALT(7, work->sheet, 60, y, 24, 64);
    }

    if ((u32)(frame - 144) <= 15) {
      s32 s;
      s32 shade;
      void *verts;
      struct RenderModel *model;

      verts = Heap_AllocEwram(32);
      model = Render_AllocateModelList(1);
      s = (frame << 13) - 0x11e000;
      texture.logWidth = 7;
      texture.logHeight = 7;
      texture.pixels = gData_02014000;
      shade = 0;
      model->texture = &texture;
      model->kind = 7;
      model->mesh = gData_08199364;
      model->vertices = verts;
      if (frame > 151) {
        shade = (frame * -8) + 1216;
      }
      model->shade = shade;
      Render_ResetTransformState();
      Func_08015160(-0x40000, 0x180000, 0);
      Func_0801521c(s);
      SceneTransform_ApplyPitch(3000);
      Render_TransformVertices(gData_08199210, verts, 4);
      Render_DrawModelList(model);
      Heap_Release(model);
      Heap_Release(verts);
    }

    work->transferPending = 1;
    wait(1);
    frame++;
  } while (frame != 160 && (frame <= 4 || !(gInput.repeat & 3)));

  FILL(canvas, 0x4000, 0);
  for (i = 0; i != 16; i++) {
    ResourceObject_ReleaseFar(work->objects[i]);
  }
  wait(1);
  Palette_LoadBasicObjColors();
  Dma_Set((const void *)OBJ_PLTT, (void *)&BG_PLTT_COLOR(5, 0), 0x80000010,
          REG_DMA3);
  BG_PLTT_COLOR(5, 14) = BG_PLTT_COLOR(15, 4);
  for (i = 0; i != 16; i++) {
    struct JudgmentObject *obj = ResourceObject_CreateFar(0x28d);

    work->objects[i] = obj;
    if (obj) {
      obj->unk1a = 0;
      Animation_ApplyChildArgumentFar(obj, i % 3);
      ((struct JudgmentObject *)work->objects[i])->priority = 0;
    }
  }
  HeapSlot_Release(SLOT_BLITTER);
  BattleEffect_LoadBlitter(SLOT_BLITTER, 19);
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  Resource_LoadAndDecompress((s32)&ResourceId_BlueCrescentSheet,
                             work->sheet + 0x4000, 1, 1);
  {
    u8 *c = sprites;

    for (i = 0; i != 770; i++, c++) {
      if (*c > 32) {
        *c += 224;
      } else if (*c != 0) {
        *c = 1;
      }
    }
  }
  work->fadeFrames = 0;
  work->fadeStep = 0;
  BattleBackground_LoadFar(1, (s32)&ResourceId_AerialViewBackdrop, 0);
  REG_BG2PA = 0x100;
  REG_BG2X = -0x3c00;
  REG_BG2CNT = 0x784;
  FILL(canvas, 0x4000, 0);
  cx = 60;
  cy = 44;
  squash = 6;
  shift = 0;
  ring = 0;
  sx = 8;
  rings = 2;
  work->transferMode = 2;
  work->transferValue = 75;
  FILL(work->sheet, 0x3840, 0);

  // The strike: the beam, the shockwave rings and the debris.
  for (frame = 0; frame != 336; frame++) {
    if (frame == 66) {
      Sound_PlayFar(208);
    }
    if (frame == 88) {
      Sound_PlayFar(230);
    }
    if (frame == 155) {
      Sound_PlayFar(162);
    }
    if (frame == 217) {
      Sound_PlayFar(156);
    }
    if (frame == 280) {
      Sound_PlayFar(157);
    }
    if (frame == 300) {
      for (i = 0; i != effect->count; i++) {
        Func_0814cd48(effect->actors[i], 7, 5, i, 8);
        Func_08118088(effect->actors[i], 1);
      }
      Func_081180e8(145);
    }
    if (gInput.repeat & 3) {
      if ((u32)(frame - 5) <= 144) {
        frame = 150;
        IO_WRITE_QUEUE_PUSH(0x04000020, 128, 0x20000);
        IO_WRITE_QUEUE_PUSH(0x04000028, 0, 0x30000);
        Resource_LoadAndDecompress((s32)&ResourceId_ExplosionSheet,
                                   work->sheet + 0x4000, 1, 0);
      } else if ((u32)(frame - 155) <= 58) {
        frame = 214;
      } else if ((u32)(frame - 219) <= 60) {
        frame = 280;
      }
    }
    if (frame == 318) {
      FILL(canvas, 0x4000, 0x3f3f3f3f);
    }
    if (frame == 319) {
      Graphics_LoadGreyRamp();
      work->transferMode = 3;
      work->transferValue = 0x04040404;
    }
    if (frame == 64) {
      IO_WRITE_QUEUE_PUSH(0x04000020, 128, 0x20000);
      IO_WRITE_QUEUE_PUSH(0x04000028, 0, 0x30000);
      FILL(canvas, 0x4000, 0);
      Resource_LoadAndDecompress((s32)&ResourceId_ExplosionSheet,
                                 work->sheet + 0x4000, 1, 0);
      Resource_LoadAndDecompress((s32)&ResourceId_OverlayImageA,
                                 work->sheet + 0x6000, 0, 0);
    }
    if (frame == 66) {
      volatile u16 *c = &BG_PLTT_COLOR(6, 0);

      for (i = 0; i != 128; i++, c++) {
        *c ^= 0x7fff;
      }
    }
    if (frame == 69) {
      FILL(canvas, 0x4000, 0x3f3f3f3f);
    }
    if (frame == 70) {
      BattleStaging_ConfigurePaletteFadeFar(
          1, (s32)&ResourceId_AerialViewBackdrop, 7);
    }
    if (frame == 150) {
      cx = 112;
      cy = 32;
      ring = 0;
      rings = 4;
      squash = 8;
      FILL(work->sheet, 0x3840, 0);
      FILL(canvas, 0x4000, 0x3f3f3f3f);
      BattleBackground_LoadFar(1, (s32)&ResourceId_BeachBackdrop, 0);
    }
    if (frame == 214) {
      FILL(work->sheet, 0x3840, 0);
      FILL(canvas, 0x4000, 0x3f3f3f3f);
      BattleBackground_LoadFar(1, (s32)&ResourceId_ForestBackdrop, 0);
      {
        struct EffectStep *p = work->particles;

        for (i = 0; i != 16; i++, p++) {
          p->x = ((Random16() & 63) + 32) << 16;
          p->y = ((Random16() & 31) + 96) << 16;
          p->velocityY = ((Random16() & 15) + 16) << 15;
          p->z = 0x8000 - (p->x >> 7);
          p->variant = 1;
        }
      }
    }
    if (frame == 280) {
      Func_0814cca8();
      FILL(work->sheet, 0x3840, 0);
      FILL(canvas, 0x4000, 0x01010101);
      transition[4] = 0;
      REG_BLDALPHA = 0x1010;
      {
        struct EffectStep *p = work->particles;

        for (i = 0; i != 16; i++, p++) {
          p->x = ((Random16() & 63) + 256) << 16;
          p->y = ((Random16() & 31) + 96) << 16;
          p->velocityY = ((Random16() & 15) + 16) << 15;
          p->z = 0;
        }
      }
      shift = 272;
      work->transferMode = 0;
      work->transferValue = 0;
    }
    if ((u32)(frame - 280) <= 38) {
      volatile u16 *c = &BG_PLTT_COLOR(0, 1);

      for (i = 0; i != 63; i++, c++) {
        s32 v = *c;
        s32 b = ((u16)v >> 10) & 31;
        s32 g = ((u16)v >> 5) & 31;
        s32 r = v & 31;

        b++;
        g++;
        r++;

        if (b > 31)
          b = 31;
        if (g > 31)
          g = 31;
        if (r > 31)
          r = 31;
        *c = b << 10 | g << 5 | r;
      }
    }
    if (frame == 182) {
      FILL(canvas, 0x4000, 0x3f3f3f3f);
    }

    if (frame <= 63) {
      u8 *sheet;
      s32 top = frame - 4;

      if ((frame & 7) < 4) {
        sheet = work->sheet + 0x4000;
      } else {
        sheet = work->sheet + 0x4240;
      }
      DRAW_ALT(19, sheet, 36, top - 24, 24, 24);
      DRAW_ALT(23, sheet, 59, top - 24, 24, 24);
      DRAW_ALT(27, sheet, 36, top - 1, 24, 24);
      DRAW_ALT(31, sheet, 59, top - 1, 24, 24);
    }
    if ((u32)(frame - 64) <= 1) {
      draw[0](canvas, work->sheet + 0x6000, 52, 51, 16, 17);
    }
    if ((u32)(frame - 66) <= 1) {
      draw[0](canvas, work->sheet + 0x6110, 48, 40, 24, 41);
    }
    if ((u32)(frame - 68) <= 7) {
      s32 d = 76 - frame;

      draw[0](canvas, work->sheet + 0x64e8, 49 - d / 2, 38 - d, 22, 44);
      draw[0](canvas, work->sheet + 0x64e8, 49 - d / 2, d + 38, 22, 44);
      draw[0](canvas, work->sheet + 0x64e8, d / 2 + 49, 38 - d, 22, 44);
      draw[0](canvas, work->sheet + 0x64e8, d / 2 + 49, d + 38, 22, 44);
    }
    if ((u32)(frame - 78) <= 1) {
      draw[0](canvas, work->sheet + 0x6000, 52, 51, 16, 17);
    }
    if ((u32)(frame - 80) <= 1) {
      draw[0](canvas, work->sheet + 0x6110, 48, 40, 24, 41);
    }
    if ((u32)(frame - 82) <= 3) {
      s32 d = frame * 2 - 164;

      draw[0](canvas, work->sheet + 0x64e8, 49 - d / 4, 38 - d, 22, 44);
      draw[0](canvas, work->sheet + 0x64e8, 49 - d / 4, d + 38, 22, 44);
      draw[0](canvas, work->sheet + 0x64e8, d / 4 + 49, 38 - d, 22, 44);
      draw[0](canvas, work->sheet + 0x64e8, d / 4 + 49, d + 38, 22, 44);
    }

    if ((u32)(frame - 72) <= 15) {
      s32 s;
      s32 shade;
      void *verts;
      struct RenderModel *model;

      verts = Heap_AllocEwram(32);
      model = Render_AllocateModelList(1);
      s = (frame << 13) - 0x8f800;
      glare.logWidth = 7;
      glare.logHeight = 7;
      glare.pixels = gData_02014000;
      shade = 0;
      model->kind = 7;
      model->texture = &glare;
      model->mesh = gData_08199364;
      model->vertices = verts;
      if (frame > 79) {
        shade = (frame * -8) + 640;
      }
      model->shade = shade;
      Render_ResetTransformState();
      Func_08015160(-0x40000, 0x80000, 0);
      Func_0801521c(s);
      SceneTransform_ApplyPitch(5000);
      Render_TransformVertices(gData_08199210, verts, 4);
      Render_DrawModelList(model);
      Heap_Release(model);
      Heap_Release(verts);
    }

    if (frame > 85) {
      if (frame <= 213) {
        for (i = 0; i != rings; i++, ring++) {
          s32 radius = ring;
          s32 slope = 0;
          s32 err = ring;

          while (radius >= slope) {
            s32 half, halfSlope, top, bottom, left, right, dx;

            half = radius / 2;
            dx = (half * sx) / 8;
            right = cx + dx;
            /* FAKEMATCH(dead): a branch whose store the next line overwrites
             * restores the label CSE and reload saw between the edges; without
             * it, 324 of 2680 instructions differ in tla-en. */
            if (dx > cx)
              left = 0;
            left = cx - dx;
            bottom = cy + (slope * squash) / 8;
            top = cy - (slope * sx) / 8;
            if (top < 0)
              top = 0;
            if (bottom > 119)
              bottom = 119;
            if (left < 0)
              left = 0;
            if (right > 119)
              right = 119;
            work->sheet[bottom * 120 + right] = 20;
            work->sheet[top * 120 + right] = 20;
            work->sheet[bottom * 120 + left] = 20;
            work->sheet[top * 120 + left] = 20;
            right = cx + half + 1;
            left = cx - half + 1;
            if (left < 0)
              left = 0;
            if (right > 119)
              right = 119;
            work->sheet[bottom * 120 + right] = 20;
            work->sheet[top * 120 + right] = 20;
            work->sheet[bottom * 120 + left] = 20;
            work->sheet[top * 120 + left] = 20;

            halfSlope = slope / 2;
            dx = (halfSlope * sx) / 8;
            right = cx + dx;
            /* FAKEMATCH(dead): a branch whose store the next line overwrites
             * restores the label CSE and reload saw between the edges; without
             * it, 199 of 2680 instructions differ in tla-en. */
            if (dx > cx)
              left = 0;
            left = cx - dx;
            bottom = cy + (radius * squash) / 8;
            top = cy - (radius * sx) / 8;
            if (left < 0)
              left = 0;
            if (right > 119)
              right = 119;
            if (top < 0)
              top = 0;
            if (bottom > 119)
              bottom = 119;
            work->sheet[bottom * 120 + right] = 20;
            work->sheet[top * 120 + right] = 20;
            work->sheet[bottom * 120 + left] = 20;
            work->sheet[top * 120 + left] = 20;
            right = cx + halfSlope + 1;
            left = cx - halfSlope + 1;
            if (left < 0)
              left = 0;
            if (right > 119)
              right = 119;
            work->sheet[bottom * 120 + right] = 20;
            work->sheet[top * 120 + right] = 20;
            work->sheet[bottom * 120 + left] = 20;
            work->sheet[top * 120 + left] = 20;

            err = err - slope * 2 - 1;
            if (err < 0) {
              err = err + radius * 2 - 2;
              radius--;
            }
            slope++;
          }
        }
      }
    }

    if ((u32)(frame - 86) <= 63) {
      s32 angle = -0x4000;

      for (i = 0; i != 9; i++, angle += 0x1000) {
        s32 x = ((frame - 86) * Trig_Sin(angle)) >> 16;
        s32 y = ((frame - 70) * Trig_Cos(angle)) >> 16;

        draw[0](canvas, work->sheet + ((frame / 2 + i) % 4) * 1728 + 0x4000,
                x + 44, y + 17, 32, 54);
      }
    }

    BattleEffect_LoadBlitter(SLOT_ALT, 3);
    draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
    if (frame > 85) {
      draw[1](canvas, work->sheet, 0, 0, 120, 120);
    }
    if (frame > 279) {
      shift -= 8;
    }
    if (frame == 238) {
      FILL(canvas, 0x4000, 0x3f3f3f3f);
    }
    if ((u32)(frame - 214) <= 65) {
      gBgScroll[1].x += 8;
    }
    if ((u32)(frame - 246) <= 33) {
      shift += 8;
      work->transferMode = 0;
      work->transferValue = 75;
    }

    if (frame > 213) {
      struct EffectStep *p;

      drift[3] = 0;
      drift[1] = 0;
      for (i = 0; i != 16; i++) {
        p = &work->particles[i];
        if (p->variant != 0) {
          scale[0] = p->z + (p->y >> 7);
          if (scale[0] <= 0x7ff) {
            scale[0] = 0x800;
          }
          scale[1] = scale[0];
          drift[0] = p->x;
          drift[1] = 0xff0000;
          drift[2] = p->y + 0xff0000;
          Render_ApplyProjectedPlacementFar(work->objects[i], drift, scale, 0);
          p->y -= p->velocityY;
          if (p->y <= 0xfffff) {
            if (frame <= 279) {
              p->x = ((Random16() & 63) + shift + 32) << 16;
            } else {
              p->x = ((Random16() & 63) + shift - 32) << 16;
            }
            if (frame > 301) {
              p->variant = 0;
            } else if (frame > 245) {
              p->y = 0x600000;
            } else {
              p->y = 0xc00000 - p->x;
            }
          }
        }
      }
      if (frame <= 279) {
        for (i = 0; i != 8; i++) {
          s32 x =
              (i & 1) * 20 - (Random16() & 15) + (i / 2) * 4 + shift / 2 - 16;
          s32 y = (i / 2) * 32 - (Random16() & 15);

          draw[1](canvas, work->sheet + ((frame / 2 + i) % 4) * 1728 + 0x4000,
                  x, y, 32, 54);
        }
      } else {
        for (i = 0; i != 8; i++) {
          s32 x =
              (i & 1) * 20 + (Random16() & 15) - (i / 2) * 4 + shift / 2 - 16;
          s32 y = (i / 2) * 32 - (Random16() & 15);

          draw[1](canvas, work->sheet + ((frame / 2 + i) % 4) * 1728 + 0x4000,
                  x, y, 32, 54);
        }
      }
    }
    HeapSlot_Release(SLOT_ALT);
    if (frame > 63) {
      gBgScroll[1].y = Random16() % 4 + 30;
    }
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  for (i = 0; i != 16; i++) {
    ResourceObject_ReleaseFar(work->objects[i]);
  }
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(SLOT_BLITTER);
  BattleEffect_EndCanvasLayer();
}

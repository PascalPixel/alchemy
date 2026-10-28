// Draft of BattleEffect_RunJudgment, decompiled from the listing: differ 2553 of
// 2680 (draft 2655), the same 172 calls and control flow; allocation differs.
// 2026-10-19: the native's arrays, from the top of its 188-byte stack frame:
// anchor (sp 176), pos (164), the swords' placement (148), the falling
// objects' placement (132, its own array: drift here), req (124), the
// first texture (116), the glare's texture (108, its own: glare here) and
// scale (100). Declared so, the arrays sit as the native's but the frame
// is 196: the spills below scale take two words more than the native's.

#include "battle_effect.h"
#include "battle_effect_work.h"
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
void BattleEffect_ProcessPendingGraphicsTransfer(
    void);
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
void EffectPosition_ProjectBattler(s32 id, struct EffectPosition *out);
void BattleEffect_SpawnObjects(s32 count, s32 resource, s32 mode); // SpawnObjects
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

struct ResourceTableEntry {
  u16 unk0;
  u16 tile;
};

struct JudgmentObject {
  u8 unk0[9];
  u8 flags;
  u8 unk0a[6];
  u8 resource;
  u8 unk11[9];
  u8 unk1a;
};

extern struct ResourceTableEntry gResourceTableEntries[];
extern u8 gMapCellBuffer[];
extern u8 gData_02014000[];
extern const u16 gData_08197410[]; // gBattleEffect_MoteCells
extern const s32 Data_08196e74[2];
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
void BattleStaging_ConfigurePaletteFadeFar(s32 mode, s32 background,
                                                s32 speed);
void Graphics_LoadGreyRamp(void);
void Func_0814cca8(void);
void Func_0813ba50(void);
void Func_08163c2c(s32 mode, s32 value);
void Resource_DecodeType01(const void *source, void *destination);
void BattleBackground_LoadFar(s32 layer, s32 background, s32 palette);

#define SLOT_ALT 188 // unkb8[1]
#define OBJ_TILES(obj)                                                         \
  (0x06010000 + gResourceTableEntries[(obj)->resource].tile)

// Judgment: the sky gathers sixteen tiles of light that fold into a sigil,
// then the sigil's beam strikes and the ground erupts in a ring of
// shockwaves and debris.
void BattleEffect_RunJudgment(void) {
  struct BattleEffectArgument *effect;
  u8 *canvas;
  struct BattleEffectWork *work;
  s32 *transition;
  u8 *sprites;
  DrawRectangle blit;
  DrawRectangle draw;
  struct EffectStep *p;
  struct JudgmentObject *obj;
  struct RenderModel *model;
  void *verts;
  s32 anchor[3];
  struct EffectPosition pos;
  s32 place[4];
  s32 drift[4];
  s32 req[2];
  struct ModelTexture texture;
  struct ModelTexture glare;
  s32 scale[2];
  s32 depth;
  s32 zoom;
  s32 frame;
  s32 i;
  s32 n;
  u8 *pal;
  u8 *cells;
  s32 addR, addG, addB;
  s32 flag;
  s32 cx, cy, squash, rings, ring, shift;

  effect = Ram_HeapSlots->battleWork;
  BattleEffect_StartCanvasLayer(0x2000);
  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  transition = Ram_HeapSlots->transitionWork;
  sprites = Ram_HeapSlots->unk64[0];
  REG_BG2PA = 0x100;
  Func_0813ba50();
  ((volatile u16 *)BG_PLTT)[0] = 0;
  ((volatile u16 *)BG_PLTT)[1] = 0;
  BattleEffect_LoadBlitter(SLOT_BLITTER, 3);
  blit = (DrawRectangle)Ram_HeapSlots->unk64[1];
  work->transferMode = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  Func_08163c2c(0, 0);
  BattleEffect_SelectLivingTargets(work->effect);
  BattleBackground_LoadFar(1, 0x45, 0);

  pal = Resource_GetTableEntry(0xa8);
  Iwram_CopyWords((void *)OBJ_PLTT, pal, 512);
  Resource_DecodeType01(pal + 512, gMapCellBuffer);
  cells = gMapCellBuffer;
  for (i = 0; i < 16; i++) {
    obj = Func_0815b290(32, 32, 0x80002000, 0);
    obj->flags = (obj->flags & ~12) | 8;
    work->objects[i] = obj;
    Iwram_CopyWords((void *)OBJ_TILES(obj), cells, 0x400);
    cells += 0x400;
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
  Resource_LoadAndDecompress(0x17f, work->sheet, 1, 1);
  Resource_LoadAndDecompress(0x192, work->sheet + 0x600, 1, 0);
  Resource_LoadAndDecompress(0xc2, gData_02014000, 0, 0);
  Resource_LoadAndDecompress(0x134, sprites, 0, 0);
  Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(0x126), 128);
  work->transferMode = 2;
  work->transferValue = 50;
  REG_BG2CNT = 0x784;

  // The gathering: motes rise, the tiles fold in and flash.
  frame = 0;
  do {
    if (frame == 0) {
      p = work->particles;
      for (i = 0; i < 64; i++, p++) {
        p->x = (Random16() & 15) + 72;
        p->velocityX = Random16() & 0xffff;
        p->velocityY = Random16() & 0xffff;
        p->velocityY = Random16() & 0xffff;
      }
      pal = Resource_GetTableEntry(0xa8);
      Iwram_CopyWords((void *)OBJ_PLTT, pal, 512);
      Resource_DecodeType01(pal + 512, gMapCellBuffer);
      for (i = 0; i < 16; i++) {
        obj = work->objects[i];
        Iwram_CopyWords((void *)OBJ_TILES(obj), gMapCellBuffer + i * 0x400,
                        0x400);
      }
      depth = -0x400000;
      zoom = 0x80000;
    }
    if (frame == 143) {
      Iwram_FillWords(canvas, 0x4000, 0x2a2a2a2a);
      Sound_PlayFar(145);
    }
    if (frame == 80) {
      Sound_PlayFar(142);
    }
    if (frame == 72) {
      work->fadeFrames = 24;
      work->fadeStep = 0;
    }

    anchor[2] = anchor[1] = 0;
    p = work->particles;
    for (i = 0; i < 64; i++, p++) {
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
        blit(canvas, sprites + gData_08197410[size - 1], pos.x - size / 2,
             pos.y - size, size, size * 2);
      }
    }

    req[0] = Data_08196e74[0];
    req[1] = Data_08196e74[1];
    place[3] = place[1] = 0;
    for (i = 0; i < 16; i++) {
      if (frame > 75 && (i == 8 || i == 12 || i == 15)) {
        continue;
      }
      place[0] = ((i % 4) << 21) + (152 << 15);
      place[2] = ((i / 4) << 21) + depth;
      Render_ApplyProjectedPlacementFar(work->objects[i], place, req, 0);
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
      pal = Resource_GetTableEntry(0xa9);
      Iwram_CopyWords((void *)OBJ_PLTT, pal, 512);
      Resource_DecodeType01(pal + 512, gMapCellBuffer);
      cells = gMapCellBuffer;
      for (i = 0; i < 16; i++) {
        obj = work->objects[i];
        IO_WRITE_QUEUE_PUSH(OBJ_TILES(obj), (u32)cells, 0x84000100);
        cells += 0x400;
      }
    }
    if (frame == 76) {
      pal = Resource_GetTableEntry(0xaa);
      Iwram_CopyWords((void *)OBJ_PLTT, pal, 512);
      Resource_DecodeType01(pal + 512, gMapCellBuffer);
      cells = gMapCellBuffer;
      for (i = 0; i < 13; i++) {
        obj = work->objects[Data_08198ab4[i]];
        IO_WRITE_QUEUE_PUSH(OBJ_TILES(obj), (u32)cells, 0x84000100);
        cells += 0x400;
      }
    }

    if ((u32)(frame - 116) <= 27) {
      n = frame * 3 - 346;
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
        for (i = 0; i < 6; i++) {
          s32 k = i & 3;
          s32 a = Random16() & 0xffff;
          s32 x = ((size * Trig_Sin(a)) >> 16) - (Data_08198ace[k] >> 1);
          s32 y = ((size * Trig_Cos(a)) >> 16) - (Data_08198ad4[k] >> 1);

          blit(canvas, work->sheet + Data_08198ac2[k], x + 60, y + 80,
               Data_08198ace[k], Data_08198ad4[k]);
        }
      }
    }

    flag = 0;
    if ((u32)(frame - 108) <= 34) {
      s32 t = (frame - 108) / 2;

      addR = t / 4;
      addB = t;
      addG = t / 2;
      flag = 1;
    }
    if (frame == 143) {
      addR = addG = addB = 31;
      flag = 1;
    }
    if ((u32)(frame - 144) <= 8) {
      addG = addR = addB = 304 - frame * 2;
      flag = 1;
    }
    if (flag == 1) {
      u16 *src = Resource_GetTableEntry(0xaa);
      volatile u16 *dst = (volatile u16 *)OBJ_PLTT;

      for (i = 0; i != 256; i++) {
        u32 c = *src++;
        s32 r = (c & 31) + addR;
        s32 g = ((c << 16) >> 21 & 31) + addG;
        s32 b = ((c << 16) >> 26 & 31) + addB;

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
        *dst++ = b << 10 | g << 5 | r;
      }
    }

    if (frame > 143) {
      s32 y = frame * 16 - 2272;

      BattleEffect_LoadBlitter(SLOT_ALT, 3);
      ((DrawRectangle)Ram_HeapSlots->unkb8[1])(canvas, work->sheet, 36, y, 24,
                                               64);
      HeapSlot_Release(SLOT_ALT);
      BattleEffect_LoadBlitter(SLOT_ALT, 7);
      ((DrawRectangle)Ram_HeapSlots->unkb8[1])(canvas, work->sheet, 60, y, 24,
                                               64);
      HeapSlot_Release(SLOT_ALT);
    }

    if ((u32)(frame - 144) <= 15) {
      s32 s;

      verts = Heap_AllocEwram(32);
      model = Render_AllocateModelList(1);
      s = (frame << 13) - 0x11e000;
      texture.logWidth = 7;
      texture.logHeight = 7;
      texture.pixels = gData_02014000;
      model->texture = &texture;
      model->kind = 7;
      model->mesh = gData_08199364;
      model->vertices = verts;
      model->shade = 0;
      if (frame > 151) {
        model->shade = 1216 - frame * 8;
      }
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

  Iwram_FillWords(canvas, 0x4000, 0);
  for (i = 0; i < 16; i++) {
    ResourceObject_ReleaseFar(work->objects[i]);
  }
  wait(1);
  Palette_LoadBasicObjColors();
  {
    volatile u32 *dma = (volatile u32 *)0x040000d4;

    dma[0] = OBJ_PLTT;
    dma[1] = BG_PLTT + 0xa0;
    dma[2] = 0x80000010;
  }
  *(volatile u16 *)(BG_PLTT + 0xbc) =
      *(volatile u16 *)(OBJ_PLTT + 0x1e8 - 0x200);
  for (i = 0; i < 16; i++) {
    obj = ResourceObject_CreateFar(0x28d);
    work->objects[i] = obj;
    if (obj) {
      obj->unk1a = 0;
      Animation_ApplyChildArgumentFar(obj, i % 3);
      ((struct JudgmentObject *)work->objects[i])->flags &= ~12;
    }
  }
  HeapSlot_Release(SLOT_BLITTER);
  BattleEffect_LoadBlitter(SLOT_BLITTER, 19);
  blit = (DrawRectangle)Ram_HeapSlots->unk64[1];
  Resource_LoadAndDecompress(0x126, work->sheet + 0x4000, 1, 1);
  cells = sprites;
  for (i = 0; i != 770; i++, cells++) {
    if (*cells > 32) {
      *cells += 224;
    } else if (*cells) {
      *cells = 1;
    }
  }
  work->fadeFrames = 0;
  work->fadeStep = 0;
  BattleBackground_LoadFar(1, 0x75, 0);
  REG_BG2PA = 0x100;
  REG_BG2X = -0x3c00;
  REG_BG2CNT = 0x784;
  Iwram_FillWords(canvas, 0x4000, 0);
  cx = 60;
  squash = 6;
  cy = 44;
  rings = 2;
  ring = 0;
  shift = 0;
  work->transferMode = 2;
  work->transferValue = 75;
  Iwram_FillWords(work->sheet, 0x3840, 0);

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
        Resource_LoadAndDecompress(0x131, work->sheet + 0x4000, 1, 0);
      } else if ((u32)(frame - 155) <= 58) {
        frame = 214;
      } else if ((u32)(frame - 219) <= 60) {
        frame = 280;
      }
    }
    if (frame == 318) {
      Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
    }
    if (frame == 319) {
      Graphics_LoadGreyRamp();
      work->transferMode = 3;
      work->transferValue = 0x04040404;
    }
    if (frame == 64) {
      IO_WRITE_QUEUE_PUSH(0x04000020, 128, 0x20000);
      IO_WRITE_QUEUE_PUSH(0x04000028, 0, 0x30000);
      Iwram_FillWords(canvas, 0x4000, 0);
      Resource_LoadAndDecompress(0x131, work->sheet + 0x4000, 1, 0);
      Resource_LoadAndDecompress(0x127, work->sheet + 0x6000, 0, 0);
    }
    if (frame == 66) {
      volatile u16 *c = (volatile u16 *)(BG_PLTT + 0xc0);

      for (i = 0; i != 128; i++, c++) {
        *c ^= 0x7fff;
      }
    }
    if (frame == 69) {
      Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
    }
    if (frame == 70) {
      BattleStaging_ConfigurePaletteFadeFar(1, 0x75, 7);
    }
    if (frame == 150) {
      cx = 112;
      cy = 32;
      ring = 0;
      rings = 4;
      squash = 8;
      Iwram_FillWords(work->sheet, 0x3840, 0);
      Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
      BattleBackground_LoadFar(1, 0x40, 0);
    }
    if (frame == 214) {
      Iwram_FillWords(work->sheet, 0x3840, 0);
      Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
      BattleBackground_LoadFar(1, 0x44, 0);
      p = work->particles;
      for (i = 0; i != 16; i++, p++) {
        p->x = ((Random16() & 63) + 32) << 16;
        p->y = ((Random16() & 31) + 96) << 16;
        p->velocityY = ((Random16() & 15) + 16) << 15;
        p->z = 0x8000 - (p->x >> 7);
        p->variant = 1;
      }
    }
    if (frame == 280) {
      Func_0814cca8();
      Iwram_FillWords(work->sheet, 0x3840, 0);
      Iwram_FillWords(canvas, 0x4000, 0x01010101);
      transition[4] = 0;
      REG_BLDALPHA = 0x1010;
      p = work->particles;
      for (i = 0; i != 16; i++, p++) {
        p->x = ((Random16() & 63) + 256) << 16;
        p->y = ((Random16() & 31) + 96) << 16;
        p->velocityY = ((Random16() & 15) + 16) << 15;
        p->z = 0;
      }
      shift = 272;
      work->transferMode = 0;
      work->transferValue = 0;
    }
    if ((u32)(frame - 280) <= 38) {
      volatile u16 *c = (volatile u16 *)(BG_PLTT + 2);

      for (i = 0; i != 63; i++, c++) {
        u32 v = *c;
        s32 b = ((v << 16) >> 26 & 31) + 1;
        s32 g = ((v << 16) >> 21 & 31) + 1;
        s32 r = (v & 31) + 1;

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
      Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
    }

    if (frame <= 63) {
      s32 top = frame - 4;

      cells = work->sheet + 0x4000;
      if ((frame & 7) > 3) {
        cells = work->sheet + 0x4240;
      }
      BattleEffect_LoadBlitter(SLOT_ALT, 19);
      ((DrawRectangle)Ram_HeapSlots->unkb8[1])(canvas, cells, 36, top - 24, 24,
                                               24);
      HeapSlot_Release(SLOT_ALT);
      BattleEffect_LoadBlitter(SLOT_ALT, 23);
      ((DrawRectangle)Ram_HeapSlots->unkb8[1])(canvas, cells, 59, top - 24, 24,
                                               24);
      HeapSlot_Release(SLOT_ALT);
      BattleEffect_LoadBlitter(SLOT_ALT, 27);
      ((DrawRectangle)Ram_HeapSlots->unkb8[1])(canvas, cells, 36, top - 1, 24,
                                               24);
      HeapSlot_Release(SLOT_ALT);
      BattleEffect_LoadBlitter(SLOT_ALT, 31);
      ((DrawRectangle)Ram_HeapSlots->unkb8[1])(canvas, cells, 59, top - 1, 24,
                                               24);
      HeapSlot_Release(SLOT_ALT);
    }
    if ((u32)(frame - 64) <= 1) {
      blit(canvas, work->sheet + 0x6000, 52, 51, 16, 17);
    }
    if ((u32)(frame - 66) <= 1) {
      blit(canvas, work->sheet + 0x6110, 48, 40, 24, 41);
    }
    if ((u32)(frame - 68) <= 7) {
      s32 d = 76 - frame;

      blit(canvas, work->sheet + 0x64e8, 49 - d / 2, 38 - d, 22, 44);
      blit(canvas, work->sheet + 0x64e8, 49 - d / 2, d + 38, 22, 44);
      blit(canvas, work->sheet + 0x64e8, d / 2 + 49, 38 - d, 22, 44);
      blit(canvas, work->sheet + 0x64e8, d / 2 + 49, d + 38, 22, 44);
    }
    if ((u32)(frame - 78) <= 1) {
      blit(canvas, work->sheet + 0x6000, 52, 51, 16, 17);
    }
    if ((u32)(frame - 80) <= 1) {
      blit(canvas, work->sheet + 0x6110, 48, 40, 24, 41);
    }
    if ((u32)(frame - 82) <= 3) {
      s32 d = frame * 2 - 164;

      blit(canvas, work->sheet + 0x64e8, 49 - d / 4, 38 - d, 22, 44);
      blit(canvas, work->sheet + 0x64e8, 49 - d / 4, d + 38, 22, 44);
      blit(canvas, work->sheet + 0x64e8, d / 4 + 49, 38 - d, 22, 44);
      blit(canvas, work->sheet + 0x64e8, d / 4 + 49, d + 38, 22, 44);
    }

    if ((u32)(frame - 72) <= 15) {
      s32 s;

      verts = Heap_AllocEwram(32);
      model = Render_AllocateModelList(1);
      s = (frame << 13) - 0x8f800;
      glare.logWidth = 7;
      glare.logHeight = 7;
      glare.pixels = gData_02014000;
      model->texture = &glare;
      model->kind = 7;
      model->mesh = gData_08199364;
      model->vertices = verts;
      model->shade = 0;
      if (frame > 79) {
        model->shade = 640 - frame * 8;
      }
      Render_ResetTransformState();
      Func_08015160(-0x40000, 0x80000, 0);
      Func_0801521c(s);
      SceneTransform_ApplyPitch(5000);
      Render_TransformVertices(gData_08199210, verts, 4);
      Render_DrawModelList(model);
      Heap_Release(model);
      Heap_Release(verts);
    }

    if (frame > 85 && frame <= 213) {
      for (i = 0; i != rings; i++, ring++) {
        s32 radius = ring;
        s32 slope = 0;
        s32 err = ring;

        while (radius >= slope) {
          s32 half, left, right, top, bottom, dx;

          half = radius / 2;
          dx = (half * 8) / 8;
          right = cx + dx;
          left = cx - dx;
          bottom = cy + (slope * squash) / 8;
          top = cy - (slope * 8) / 8;
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

          half = slope / 2;
          dx = (half * 8) / 8;
          right = cx + dx;
          left = cx - dx;
          bottom = cy + (radius * squash) / 8;
          top = cy - (radius * 8) / 8;
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

          err = err - slope * 2 - 1;
          if (err < 0) {
            err = err + radius * 2 - 2;
            radius--;
          }
          slope++;
        }
      }
    }

    if ((u32)(frame - 86) <= 63) {
      s32 angle = -0x4000;

      for (i = 0; i != 9; i++, angle += 0x1000) {
        s32 x = ((frame - 86) * Trig_Sin(angle)) >> 16;
        s32 y = ((frame - 70) * Trig_Cos(angle)) >> 16;

        blit(canvas, work->sheet + ((frame / 2 + i) % 4) * 1728 + 0x4000,
             x + 44, y + 17, 32, 54);
      }
    }

    BattleEffect_LoadBlitter(SLOT_ALT, 3);
    draw = (DrawRectangle)Ram_HeapSlots->unkb8[1];
    if (frame > 85) {
      draw(canvas, work->sheet, 0, 0, 120, 120);
    }
    if (frame > 279) {
      shift -= 8;
    }
    if (frame == 238) {
      Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
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
      drift[3] = drift[1] = 0;
      p = work->particles;
      for (i = 0; i != 16; i++, p++) {
        if (p->variant != 0) {
          scale[0] = p->z + (p->y >> 7);
          if (scale[0] <= 0x7ff) {
            scale[0] = 0x800;
          }
          scale[1] = scale[0];
          drift[1] = 0xff0000;
          drift[0] = p->x;
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

          draw(canvas, work->sheet + ((frame / 2 + i) % 4) * 1728 + 0x4000, x,
               y, 32, 54);
        }
      } else {
        for (i = 0; i != 8; i++) {
          s32 x =
              (i & 1) * 20 + (Random16() & 15) - (i / 2) * 4 + shift / 2 - 16;
          s32 y = (i / 2) * 32 - (Random16() & 15);

          draw(canvas, work->sheet + ((frame / 2 + i) % 4) * 1728 + 0x4000, x,
               y, 32, 54);
        }
      }
    }
    HeapSlot_Release(SLOT_ALT);
    if (frame > 63) {
      gBgScroll[1].y = (Random16() & 3) + 30;
    }
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  for (i = 0; i < 16; i++) {
    ResourceObject_ReleaseFar(work->objects[i]);
  }
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(SLOT_BLITTER);
  BattleEffect_EndCanvasLayer();
}

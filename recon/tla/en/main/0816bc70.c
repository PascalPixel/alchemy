// Draft of BattleEffect_RunDiamondBerg, differ 24 of 658, skeleton gap 22 (tla-en, 2026-11-01)
// Same instructions but for order, plus one zero loaded once where the native
// loads it twice (11 removed, 11 added). Left:
// - Stores before a loop start run late: top before the frame and particle
//   loops, top += 8 before the shard loop, scrollU and scrollV before the flash
//   loop. sched2 gives a last store priority 1, below the moves after it; the
//   native acts as if each had one more dependant. do/while for the frame loop
//   and reordering the frame = 0 assignment do not change it.
// - The model's first setup: the native stores unk04 first with its own zero in
//   r3 and a second zero in r0 for scrollU and scrollV. Here one zero (a pseudo
//   hoisted out of the frame loop, then rematerialized) serves both. All 720
//   orders of those six stores were measured: this one has the lowest differ;
//   kind and unk04 last reach gap 20, differ 34, and contradict the native's
//   store order. All orders of the flash's setup gain nothing.
// - The shards' draw: str r6, [sp] one place early (a pair).
// Facts that closed the rest:
// - cells is one local for the whole function, spilled and rematerialized from
//   its constant; that is why each load reloads gMapCellBuffer.
// - each shard loop has its own shard pointer (r5, r5, r7).
// - BattleEffect_LoadBlitter, Task_Add and Task_Remove return s32, as defined;
//   the void prototype swaps the blitter call's argument moves.
// - pixels is u32, as in project_vertices.c's struct Texture: it shares an alias
//   set with the model's words, which keeps those stores in order.
// - pixels, x, y and height are read in that order before the draw.

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
  u32 pixels;
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
#define HI(v) (((s16 *)&(v))[1])

void BattleEffect_SetApproachMotion(s32 id, s32 targetId, s32 frames, s32 rate);

extern char ResourceId_CrystalSheet;
extern char ResourceId_CrystalBitmapSheet;
extern char ResourceId_Data0C1;
extern char ResourceId_Data0DA;
extern char ResourceId_Data134;
extern char ResourceId_Data0C2;
extern u8 gMapCellBuffer[];
extern u8 Data_02014e20[];         // gMapCellBuffer + 20000, the berg
extern const s32 gData_08197834[]; // the shards' cells: offsets,
extern const u8 gData_0819781a[];  // widths
extern const u8 gData_08197826[];  // and heights
extern const u8 Data_08198ec4[];   // the ripple's mesh
extern const u8 Data_08198cac[];   // its corners
extern const u8 Data_08198b26[4];  // per ripple: its start,
extern const u8 Data_08198b2a[4];  // its growth
extern const s8 Data_08198b2e[4];  // and its height
extern const s16 Data_08198b32[8]; // per flash: its roll
extern const s16 Data_08198b42[8]; // and pitch
extern const u8 gData_08199364[];  // the flash's mesh
extern const u8 gData_08199210[];  // its corners

#define DIAMOND_BERG_FRAMES 148

// Abilities: Diamond Berg.
//
// An iceberg drops onto the target and is driven down in eight blows (every
// eighth frame from 31), each throwing eight shards; the last shatters it
// into a burst of light. Ripples of frost spread over the ground beneath.
void BattleEffect_RunDiamondBerg(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  u8 *sprites;
  /* FAKEMATCH(frame): the blitter is declared as The Broken Seal's pair
   * though only the first is used; its second word is the unused word at
   * sp 28 of the native's stack frame; as one DrawRectangle every stack offset
   * below it moves (differ 365). */
  DrawRectangle blit[2];
  struct EffectPosition pos;
  struct EffectPosition target;
  struct ModelTexture texture;
  struct RenderModel *model;
  s32 centerX;
  s32 top;
  s32 frame;
  s32 i;
  s32 k;
  u8 *cells;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sprites = Ram_HeapSlots->unk64[0];
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = 0x1010;
  EffectPosition_ProjectBattler(effect->actors[0], &pos);
  centerX = pos.x / 2;
  BattleEffect_LoadBlitter(SLOT_BLITTER,
                       BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  blit[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  Resource_LoadAndDecompress((s32)&ResourceId_CrystalSheet, work->sheet, 1, 0);
  cells = gMapCellBuffer;
  Resource_LoadAndDecompress((s32)&ResourceId_CrystalBitmapSheet, cells + 20000,
                             1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C1, cells, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0DA, cells + 0x1000, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C2, work->sheet + 0x3080, 0,
                             0);
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  top = -128;

  for (frame = 0; frame != DIAMOND_BERG_FRAMES; frame++) {
    if (frame == 0) {
      top = -104;
      for (i = 0; i != 64; i++) {
        work->particles[i].variant = -1;
      }
    }
    if (frame <= 63) {
      EffectPosition_ProjectBattler(effect->actors[0], &target);
    }
    if (frame == 20) {
      BattleEffect_SetApproachMotion(effect->actor, effect->actors[0], 8, 60);
    }
    if (frame == 2) {
      Sound_PlayFar(212);
    }

    // The berg: it drops, then each blow drives it eight lines down.
    if (frame <= 127) {
      s32 height = 104;

      if (frame <= 8) {
        top += 16;
      } else if ((u32)(frame - 24) <= 64) {
        s32 t = frame - 24;
        // t % 8, spelled so the quotient is its own register.
        s32 beat = t - t / 8 * 8;

        if (beat == 4) {
          Sound_PlayFar(212);
        }
        if (beat == 7) {
          work->shakeFrames = 4;
          if (frame <= 79) {
            Sound_PlayFar(144);
            BattleEffect_SetApproachMotion(effect->actor, effect->actors[0], 16,
                                       70);
          } else {
            Func_08118088(effect->actors[0], 4);
            Func_081180e8(144);
            Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
          }
          Func_0814cd48(effect->actors[0], 7, 5, 0, 6);
          top += 8;
          for (k = 0; k != 8; k++) {
            struct EffectStep *shard;

            shard = &work->particles[((t - 7) / 8 * 8 + k + 32) & 63];
            shard->y = 0x600000;
            shard->x = centerX << 16;
            shard->velocityX = ((Random16() & 127) - 64) << 11;
            shard->velocityY = -((Random16() & 127) + 64) << 11;
            shard->z = 32;
            shard->variant = 0;
          }
        }
      }
      if (top + 104 > 108) {
        height = height - top + 4;
      }
      if (height > 0) {
        blit[0](canvas, Data_02014e20, centerX - 24, top, 48, height);
      }
    }
    if (frame == 7) {
      struct EffectStep *shard;

      work->shakeFrames = 8;
      Sound_PlayFar(145);
      for (k = 0, shard = work->particles; k != 32; k++, shard++) {
        shard->y = 0x600000;
        shard->x = centerX << 16;
        shard->velocityX = ((Random16() & 127) - 64) << 12;
        shard->velocityY = -((Random16() & 127) + 64) << 12;
        shard->z = 32;
        shard->variant = 0;
      }
    }

    // The shards tumble and bounce; z is their spin.
    if (frame > 6) {
      struct EffectStep *shard;

      for (i = 0, shard = work->particles; i != 64; i++, shard++) {
        s32 cell;
        u8 *pixels;
        s32 x;
        s32 y;
        u32 width;
        u32 height;

        if ((u32)shard->variant > 1023) {
          continue;
        }
        cell = i % 5 * 3 + shard->variant / 96 % 3;
        pixels = work->sheet + 0x800 + gData_08197834[cell];
        width = gData_0819781a[cell];
        x = HI(shard->x) - (width >> 1);
        y = HI(shard->y);
        height = gData_08197826[cell];
        y -= height >> 1;
        blit[0](canvas, pixels, x, y, width, height);
        BattleEffectKernels_IntegrateVector2(shard, 64, 0x4000);
        if (HI(shard->y) > 104) {
          shard->velocityY = -shard->velocityY / 2;
        }
        shard->variant += shard->z;
        if (shard->z > 1 && (frame & 1)) {
          shard->z--;
        }
      }
    }

    if (frame > 5) {
      void *vertices;

      vertices = Heap_AllocEwram(0x400);
      model = Render_AllocateModelList(1);
      texture.logWidth = 7;
      texture.logHeight = 7;
      model->texture = &texture;
      model->unk04 = 0;
      model->kind = 7;
      texture.pixels = (u32)cells;
      model->mesh = Data_08198ec4;
      model->vertices = vertices;
      model->scrollU = 0;
      model->scrollV = 0;

      // The frost ripples.
      for (k = 0; k != 4; k++) {
        s32 start = Data_08198b26[k] + 6;
        s32 scale;
        s32 shade;

        if (frame <= start) {
          continue;
        }
        scale = (frame - start) * Data_08198b2a[k] * 1400 + 16768;
        shade = (start - frame) * 8 + 56;
        if (shade > 0) {
          shade = 0;
        }
        if (shade <= -64) {
          continue;
        }
        model->shade = shade;
        Render_ResetTransformState();
        Func_08015160((centerX - 64) << 16, Data_08198b2e[k] << 16, 0);
        Func_080151e4(scale, scale, scale);
        model->scrollU = ((frame + k * 5) & 7) << 4;
        SceneTransform_ApplyPitch(3000);
        Render_TransformVertices(Data_08198cac, vertices, 32);
        Render_DrawModelList(model);
      }

      // A flash at each blow, at the target.
      texture.logWidth = 7;
      texture.logHeight = 7;
      texture.pixels = (u32)(work->sheet + 0x3080);
      model->mesh = gData_08199364;
      model->scrollU = 0;
      model->scrollV = 0;
      for (k = 0; k != 8; k++) {
        s32 t = frame - 31 - k * 8;
        s32 scale;
        s32 shade;

        if (t < 0) {
          continue;
        }
        scale = ((k << 11) + 0x2000) * t + 0x8000;
        shade = 0;
        if (t > 3) {
          shade = (4 - t) * 16;
        }
        model->shade = shade;
        if (shade <= -64) {
          continue;
        }
        Render_ResetTransformState();
        Func_08015160((target.x / 2 - 64) << 16, (target.y - 80) << 16, 0);
        Func_080151e4(0x8000, 0x10000, 0x8000);
        SceneTransform_ApplyYaw((effect->side << 15) + 0x8000);
        Func_080150e4(Data_08198b32[k]);
        SceneTransform_ApplyPitch(Data_08198b42[k]);
        Func_0801521c(scale);
        Render_TransformVertices(gData_08199210, vertices, 4);
        Render_DrawModelList(model);
      }
      Heap_Release(model);
      Heap_Release(vertices);
    }
    Func_08158ce0(4, 4);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(104);
  BattleEffect_EndCanvasLayer();
}

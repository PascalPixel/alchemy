// Draft, differ 478 of 747, skeleton gap 100 (tla-en, 2026-11-01)
// Control flow and calls follow the listing; the two ball arms merge by
// cross-jumping as there. What fixed the shape, so keep it: explicit __divsi3
// (the listing rereads ball->y after it, a call clobbers memory), each arm with
// its own divides, Ram_MapCellBuffer as a constant (the listing reloads it from
// the pool at each use), one shared counter i and separate k, j, n and step
// (step lands in r4 with caller saves only because every callee save is taken),
// radius set in both arms of the mode test.
// Left:
// - frames: the listing keeps both arms of if (mode == 0); here IF-CASE-1 in ifcvt
//   moves frames = 24 above the test. Ternary, switch and narrower types did not
//   stop it; XXX what makes THEN's set unmovable there.
// - The copy nest: the listing hoists 0x1b5a (r12) out of both loops and keeps
//   i << 12 in the loop; the constant form leaves 0x1b5a in the row loop, the
//   symbol form hoists it but also hoists gMapCellBuffer.
// - Flash loop: the listing loads widths (r10) and heights (r6) inside the loop;
//   here loop.c hoists both, widths gets no register and is rematerialised.
//   Making one pointer serve widths and the bounce's cells gives the listing's
//   flash loop but hoists cells + size - 1 in the bounce (gap 123).
// - The rest is sched2 order after allocation: Iwram entry pool loads after a
//   split constant, and r7 as the reload register that survives calls.
// - Frame 92 against 96: sp+44 in the listing is an unused slot no pseudo here
//   reproduces.

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
void Task_Add(void (*func)(void), s32 order);
void Task_Remove(void (*func)(void));
void HeapSlot_Release(s32 slot);
void Sound_PlayFar(s32 cue);
void wait(s32 frames);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);
s32 __divsi3(s32, s32);

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

s32 Func_08196404(s32 slot, s32 widthShift, s32 heightShift, s32 flags,
                   s32 mode);

extern char ResourceId_TornadoSheet;
extern char ResourceId_Data134;
extern char ResourceId_LightningBoltSheet;
extern char ResourceId_VioletPaletteC;
extern struct EffectStep gData_02014000[]; // the sparks
extern u16 gData_08197410[];               // gBattleEffect_MoteCells
extern const u16 gData_08197486[];         // the flashes' cells: offsets,
extern const u8 gData_08197492[];          // widths
extern const u8 gData_08197498[];          // and heights
extern const u8 Data_08198bee[4];          // their flips
extern const u8 gData_08199268[];          // a bolt's mesh
extern const u8 gData_081991b0[];          // its corners (Thunder Mine)
extern const u8 Data_081991c0[];           // and (Earth Force)

// The body of Thunder Mine (mode 0) and Earth Force (mode 1): a ball of
// light (Thunder Mine) or a falling rock's glow (Earth Force) crosses to the
// target, trailed by 40 spinning bolts, and grows; when it arrives the
// target is hit, and in Thunder Mine 128 sparks scatter and bounce.
void Func_0816fca8(struct BattleEffectArgument *effect, s32 mode) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  DrawRectangle blit;
  u8 *sprites;
  s32 radius;
  s32 frames;
  s32 x;
  s32 y;
  struct EffectPosition source;
  struct EffectPosition target;
  struct ModelTexture texture;
  struct RenderModel *model;
  struct EffectStep *ball;
  struct EffectStep *bolt;
  struct EffectStep *spark;
  void *vertices;
  s32 i;
  s32 k;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sprites = Ram_HeapSlots->unk64[0];
  BattleEffect_StartCanvasLayer(1);
  if (mode == 0) {
    REG_BLDALPHA = 0x1010;
  } else {
    REG_BLDALPHA = 0x810;
  }
  BattleEffect_LoadBlitter(SLOT_BLITTER,
                BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  blit = (DrawRectangle)Ram_HeapSlots->unk64[1];
  radius = 0;
  Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, work->sheet + 0x800,
                             1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_LightningBoltSheet,
                             Ram_MapCellBuffer, 1, 1);
  if (mode == 1) {
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_VioletPaletteC),
                    128);
  }

  // The bolts' four pictures, 24 by 120, each in a 32-wide cell.
  Iwram_ClearWords(work->sheet + 0x1456, 0x4000);
  for (i = 0; i != 4; i++) {
    for (k = 0; k != 120; k++) {
      s32 j;

      for (j = 0; j != 24; j++) {
        work->sheet[0x145a + i * 0x1000 + k * 32 + j] =
            Ram_MapCellBuffer[(i * 45 << 6) + k * 24 + j];
      }
    }
  }
  if (mode == 0) {
    work->transferMode = 2;
    work->transferValue = 50;
  } else {
    work->transferMode = 1;
    work->transferValue = 0;
  }
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  if (mode == 0) {
    frames = 24;
  } else {
    frames = 40;
  }

  for (frame = 0; frame != 50; frame++) {
    ball = work->particles;
    if (frame == 0) {
      EffectPosition_ProjectBattler(effect->actor, &source);
      EffectPosition_ProjectBattler(effect->actors[0], &target);
      if (mode == 0) {
        ball->x = source.x << 16;
        ball->y = (source.y - 32) << 16;
        ball->velocityX = __divsi3((target.x << 16) - ball->x, frames);
        ball->velocityY = __divsi3(((target.y - 32) << 16) - ball->y, frames);
      } else {
        if (effect->side == 0) {
          ball->x = (source.x - 32) << 16;
        } else {
          ball->x = (source.x + 32) << 16;
        }
        ball->y = (source.y - 80) << 16;
        ball->velocityX = __divsi3(((target.x - 32) << 16) - ball->x, frames);
        ball->velocityY = __divsi3(((target.y - 32) << 16) - ball->y, frames);
      }
      Sound_PlayFar(221);
      for (i = 0, bolt = &work->particles[1]; i != 63; i++, bolt++) {
        bolt->z = Random16();
        bolt->velocityZ = (Random16() & 255) + 128;
        if (Random16() & 1) {
          bolt->velocityZ = -bolt->velocityZ;
        }
        bolt->variant = Random16() & 3;
      }
      {
        for (i = 0, spark = gData_02014000; i != 512; i++, spark++) {
          spark->variant = -1;
        }
      }
    }

    // Arrival.
    if (frame == frames) {
      s32 n;

      for (n = 0, spark = gData_02014000; n != 128; n++, spark++) {
        s32 angle;
        s32 speed;

        spark->x = ball->x / 2;
        spark->y = ball->y;
        angle = Random16();
        speed = Random16() % 400 + 32;
        spark->velocityX = (Trig_Sin(angle) * speed) >> 7;
        spark->velocityY = (Trig_Cos(angle) * speed) >> 6;
        spark->variant = (Random16() & 7) + 16;
      }
      Iwram_FillWords(canvas, 0x3c00, 0x3f3f3f3f);
      work->shakeFrames = 16;
      Func_08118088(effect->actors[0], 4);
      Func_0814cd48(effect->actors[0], 7, 5, 0, 8);
      if (mode == 0) {
        Func_081180e8(145);
      } else {
        Func_081180e8(-1);
        Sound_PlayFar(145);
      }
    }

    // The ball, growing to 48 (64), with four flashes about it.
    if (frame <= frames) {
      s32 largest;

      ball->variant++;
      x = ball->x >> 17;
      y = HI(ball->y);
      if (mode == 0) {
        radius = frame * 2 + 16;
        largest = 48;
      } else {
        radius = frame * 2 + 16;
        largest = 64;
      }
      if (radius > largest) {
        radius = largest;
      } else {
        Func_0815b434(Ram_MapCellBuffer, radius, 85536);
        if (mode == 1) {
          s32 count = radius * radius / 2;

          for (i = 0; i != count; i++) {
            if (Ram_MapCellBuffer[i] != 0) {
              Ram_MapCellBuffer[i] = 64 - Ram_MapCellBuffer[i];
            }
          }
        }
      }
      Func_0818caa8(Ram_MapCellBuffer, x, y, radius);
      for (i = 0; i != 4; i++) {
        s32 cell = i & 3;
        s32 half = radius / 2;
        s32 angle = Random16();
        s32 flashX =
            x + ((Trig_Sin(angle) * half) >> 16) - (gData_08197492[cell] >> 1);
        s32 flashY =
            y + ((Trig_Cos(angle) * half) >> 16) - (gData_08197498[cell] >> 1);

        Func_08196404(188, 7, 7, 3 | Data_08198bee[Random16() & 3], 1);
        ((DrawRectangle)Ram_HeapSlots->unkb8[1])(
            canvas, work->sheet + 0x800 + gData_08197486[cell], flashX, flashY,
            gData_08197492[cell], gData_08197498[cell]);
        HeapSlot_Release(188);
      }
      BattleEffectKernels_IntegrateVector2(ball, 64, 0);
    }

    // Thunder Mine's sparks bounce.
    if (mode == 0 && frame >= frames) {
      for (i = 0, spark = gData_02014000; i != 512; i++, spark++) {
        if (spark->variant >= 0) {
          s32 size = (spark->variant >> 3) + 2;
          s32 step;

          for (step = 0; step != 8; step++) {
            blit(canvas, sprites + gData_08197410[size - 1],
                 HI(spark->x) - size / 2, HI(spark->y) - size, size, size * 2);
            BattleEffectKernels_IntegrateVector2(spark, 62, 0x200);
          }
          if (HI(spark->y) > 111) {
            spark->velocityY = -spark->velocityY * 48 / 64;
          }
          spark->variant--;
        }
      }
    }

    // The trailing bolts, each eight frames long.
    if (frame <= frames) {
      vertices = Heap_AllocEwram(32);
      model = Render_AllocateModelList(1);
      texture.logWidth = 5;
      texture.logHeight = 7;
      if (mode == 0) {
        model->kind = 6;
      } else {
        model->kind = 9;
      }
      model->kind = 6;
      model->texture = &texture;
      model->mesh = gData_08199268;
      model->vertices = vertices;
      for (i = 0, bolt = &work->particles[1]; i != 40; i++, bolt++) {
        s32 shade;

        if (frame < i || frame >= i + 8) {
          continue;
        }
        shade = 0;
        if (frame == i) {
          shade = -16;
        } else if (frame >= i + 4) {
          s32 fade = (frame - i) * 16 - 64;

          shade = -fade;
        }
        texture.pixels = work->sheet + 0x1456 + (bolt->variant << 12);
        model->shade = shade;
        Render_ResetTransformState();
        Func_08015160(ball->x / 2 - 0x400000, ball->y - 0x400000, 0);
        Func_080150e4(bolt->z);
        bolt->z += bolt->velocityZ;
        if (mode == 0) {
          Func_080151e4(0x40000, 0x10000, 0x10000);
          Render_TransformVertices(gData_081991b0, vertices, 4);
        } else {
          Func_080151e4(radius << 11, radius * 312, 0x10000);
          Render_TransformVertices(Data_081991c0, vertices, 4);
        }
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

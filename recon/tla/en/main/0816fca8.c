// Differ 6 of 747 (tla-en, 2026-11-08, -mtune=gba): two stack reloads issue
// a place earlier here, and one ldrb takes its address registers swapped.
// 2026-11-07: from 20 by ordinary forms: an empty once-only block before the
// second blitter build (16), the last height as *(table + cell) (12),
// ball->variant++ in a once-only block (8), the flip flags in a local (6).
// Left: frames' stack load ahead of the ball->y store, y's ldrsh ahead of the
// mode reload, ldrb r3, [r4, r6] for [r6, r4] in the flash draw. No gain: an
// array for flight, other spellings of height and cell, blocks round the
// ball's stores, 15 permuter minutes.
// 2026-11-05: differ 20 of 747 (tla-en). A wrapper on the palette copy gains 2.
// 2026-11-01: differ 32 of 747, skeleton gap 32 (tla-en).
// Every register and branch now agrees; what is left is five sched2 orders, no
// shape and no allocation. All five are the K2 load model's: a load or a pooled
// chain issued ahead of a store or spill the listing does first.
// - After the ball's ball->y store, frames' stack load goes before the store.
// - ball->y's high halfword loads before the variant store; the listing does
// the
//   x store and the mode reload first.
// - The flash draw: the table load, heights and the canvas reload.
// - The bounce's preheader: the spill of size * 2 and its shift.
// - The bolts' preheader: i, bolt and the texture address before the three
// model
//   stores.
// Tried: an empty do-while or asm("") before y gives 28, and one before the
// bolt loop gives 26 (with a picture local set after it, every instruction but
// with r1 and r2 swapped, differ 52); the one in the arm leaves ldr r0 and ldr
// r1 swapped. Five or more devices would not reach 0, so none is kept. Also no
// gain: locals for size * 2 and size / 2, a union view of the halfword, s32
// model fields. What fixed the shape, so keep it:
// - flight: x and frames are one 8-byte local. Its partial sets keep frames
// live
//   into the else arm, which stops IF-CASE-1 and keeps both arms of if (mode ==
//   0); the frame then has its slots (frames at sp 28, x at sp 24). As a pair
//   {frames, radius} it loses the hoisted radius >> 31 and the frame is 92.
// - draw[2]: the unused word is sp 44.
// - buffer is set once at the top and lives across calls, so no register
// survives
//   for it and it is rematerialized at each use; 0x1b5a then takes ip.
// - The empty do-while before the second blitter build ends CSE's block, so the
// width and height tables stay
//   in the flash loop, each in its own short-lived pseudo until cse2 merges
//   them.
// - The bounce has its own pointer dot, apart from the arrival's spark: r6 and
// r7.
// - The two arms join after velocityX with a plain /, so the second divide
// reloads
//   work where cross-jumping shares only the bl and its store.
// - radius * radius / 2 stays in the loop test; variant is stepped by hand so
// the
//   pool holds the +24 address; Task_Add, Task_Remove and Func_08196404 as
//   declared.

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
s32 BattleEffect_LoadBlitter(s32 slot,
                             s32 modeAndFlags); // BattleEffect_BuildBlitter
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

s32 Func_08196404(s32 slot, s32 widthShift, s32 heightShift, s32 flags,
                  u32 mode);

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

static inline s32 CopyWords(void *destination, const void *source, s32 size) {
  /* FAKEMATCH(wrapper): the call through this inline; as a direct call to
   * Iwram_CopyWords, 8 of 747 instructions differ in tla-en. */
  return Iwram_CopyWords(destination, source, size);
}

// The body of Thunder Mine (mode 0) and Earth Force (mode 1): a ball of
// light (Thunder Mine) or a falling rock's glow (Earth Force) crosses to the
// target, trailed by 40 spinning bolts, and grows; when it arrives the
// target is hit, and in Thunder Mine 128 sparks scatter and bounce.
void Func_0816fca8(struct BattleEffectArgument *effect, s32 mode) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  DrawRectangle draw[2];
  u8 *sprites;
  s32 radius;
  struct {
    s32 x;
    s32 frames;
  } flight;
  s32 y;
  struct EffectPosition target;
  struct EffectPosition source;
  struct ModelTexture texture;
  struct RenderModel *model;
  struct EffectStep *ball;
  struct EffectStep *bolt;
  struct EffectStep *spark;
  struct EffectStep *dot;
  void *vertices;
  s32 i;
  s32 k;
  u8 *buffer;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sprites = Ram_HeapSlots->unk64[0];
  buffer = Ram_MapCellBuffer;
  BattleEffect_StartCanvasLayer(1);
  if (mode == 0) {
    REG_BLDALPHA = 0x1010;
  } else {
    REG_BLDALPHA = 0x810;
  }
  BattleEffect_LoadBlitter(
      SLOT_BLITTER, BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  radius = 0;
  Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, work->sheet + 0x800,
                             1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_LightningBoltSheet, buffer, 1, 1);
  if (mode == 1) {
    CopyWords((void *)BG_PLTT,
              Resource_GetTableEntry((s32)&ResourceId_VioletPaletteC), 128);
  }

  // The bolts' four pictures, 24 by 120, each in a 32-wide cell.
  Iwram_ClearWords(work->sheet + 0x1456, 0x4000);
  for (i = 0; i != 4; i++) {
    for (k = 0; k != 120; k++) {
      s32 j;

      for (j = 0; j != 24; j++) {
        work->sheet[0x145a + i * 0x1000 + k * 32 + j] =
            buffer[(i * 45 << 6) + k * 24 + j];
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
    flight.frames = 24;
  } else {
    flight.frames = 40;
  }

  for (frame = 0; frame != 50; frame++) {
    ball = work->particles;
    if (frame == 0) {
      EffectPosition_ProjectBattler(effect->actor, &source);
      EffectPosition_ProjectBattler(effect->actors[0], &target);
      if (mode == 0) {
        ball->x = source.x << 16;
        ball->y = (source.y - 32) << 16;
        ball->velocityX = ((target.x << 16) - ball->x) / flight.frames;
      } else {
        if (effect->side == 0) {
          ball->x = (source.x - 32) << 16;
        } else {
          ball->x = (source.x + 32) << 16;
        }
        ball->y = (source.y - 80) << 16;
        ball->velocityX = (((target.x - 32) << 16) - ball->x) / flight.frames;
      }
      ball->velocityY = (((target.y - 32) << 16) - ball->y) / flight.frames;
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
        s32 *variant = &gData_02014000[0].variant;

        for (i = 0; i != 512; i++) {
          *variant = -1;
          variant += 7;
        }
      }
    }

    // Arrival.
    if (frame == flight.frames) {
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
    if (frame <= flight.frames) {
      s32 largest;

      // Once-only block: keeps y's load after the variant store.
      do {
        ball->variant++;
      } while (0);
      flight.x = ball->x >> 17;
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
        Func_0815b434(buffer, radius, 85536);
        if (mode == 1) {
          for (i = 0; i != radius * radius / 2; i++) {
            if (buffer[i] != 0) {
              buffer[i] = 64 - buffer[i];
            }
          }
        }
      }
      Func_0818caa8(buffer, flight.x, y, radius);
      for (i = 0; i != 4; i++) {
        s32 cell = i & 3;
        s32 half = radius / 2;
        s32 angle = Random16();
        s32 flashX = flight.x + ((Trig_Sin(angle) * half) >> 16) -
                     (gData_08197492[cell] >> 1);
        s32 flashY =
            y + ((Trig_Cos(angle) * half) >> 16) - (gData_08197498[cell] >> 1);
        s32 flags = 3 | Data_08198bee[Random16() & 3];

        // Once-only block: it ends CSE's block, so the width and height tables
        // stay in the flash loop.
        do {
        } while (0);
        Func_08196404(188, 7, 7, flags, 1);
        ((DrawRectangle)Ram_HeapSlots->unkb8[1])(
            canvas, work->sheet + 0x800 + gData_08197486[cell], flashX, flashY,
            gData_08197492[cell], *(gData_08197498 + cell));
        HeapSlot_Release(188);
      }
      BattleEffectKernels_IntegrateVector2(ball, 64, 0);
    }

    // Thunder Mine's sparks bounce.
    if (mode == 0 && frame >= flight.frames) {
      for (i = 0, dot = gData_02014000; i != 512; i++, dot++) {
        if (dot->variant >= 0) {
          s32 size = (dot->variant >> 3) + 2;
          s32 step;

          for (step = 0; step != 8; step++) {
            draw[0](canvas, sprites + gData_08197410[size - 1],
                    HI(dot->x) - size / 2, HI(dot->y) - size, size, size * 2);
            BattleEffectKernels_IntegrateVector2(dot, 62, 0x200);
          }
          if (HI(dot->y) > 111) {
            dot->velocityY = -dot->velocityY * 48 / 64;
          }
          dot->variant--;
        }
      }
    }

    // The trailing bolts, each eight frames long.
    if (frame <= flight.frames) {
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

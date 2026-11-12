#include "types.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "effect_step.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "heap_release.h"
#include "ram_buffer.h"
#include "task.h"
#include "battle.h"
#include "motobj.h"

struct FxPartyList;
struct Model;
struct StripQuad;

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
void Func_0815585c(struct BattleEffectArgument *effect, u32 packed, s32 *screen, s32 *world);
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
void BattleEffect_SpawnObjects(s32 entryCount, s32 kind, u32 variant); // SpawnObjects
void BattleEffect_SelectLivingTargets(struct FxPartyList *party); // SelectLivingTargets
void Func_080150e4(s32 angle);            // SceneTransform_ApplyRoll
void BattleEffect_StartActorMotion(s32 id, s32 force, s32 acceleration,
                                   s32 speedLimit, s32 velocityY, s32 percent);

void BattleEffect_FetchRectangleBlitters(s32 flip, void **out);
void Func_08118088(s32 actor, s32 motion); // BattleMotion_ApplyVariantMotionFar
void Func_081180e8(s32 phase);             // BattlePlayback_StartFar
void BattleCamera_Shake(s32 rangeX, u32 rangeY);
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 gravity,
                                          s32 drag);

void *Heap_AllocEwram(s32 size);
void Resource_LoadAndDecompress(s32 resource, void *destination, s32 a, s32 b);
void *Resource_GetTableEntry(s32 resource);


void HeapSlot_Release(s32 slot);
void Sound_PlayFar(s32 cue);
void wait(s32 frames);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);

void Render_BuildQuadStrip(struct StripQuad *quad, s32 width, s32 height, s32 count);
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

s32 Func_08196404(s32 slot, s32 widthShift, s32 heightShift, s32 flags,
                  u32 mode);

extern char ResourceId_TornadoSheet;
extern char ResourceId_Data134;
extern char ResourceId_LightningBoltSheet;
extern char ResourceId_VioletPaletteC;
extern struct EffectStep gData_02014000[]; // the sparks
extern u16 gData_08197410[];               // gBattleEffect_MoteCells
// Heights not const: a const table's second height read issues two places
// early (6).
extern u16 gData_08197486[]; // the flashes' cells: offsets,
extern u8 gData_08197492[];  // widths
extern u8 gData_08197498[];        // and heights
extern const u8 Data_08198bee[4];  // their flips
extern const u8 gData_08199268[];  // a bolt's mesh
extern const u8 gData_081991b0[];  // its corners (Thunder Mine)
extern const u8 Data_081991c0[];   // and (Earth Force)

static inline s32 CopyWords(void *destination, const void *source, s32 size) {
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
  // Unused: draw[1] gives the stack frame its 4 bytes.
  DrawRectangle draw[2];
  u8 *sprites;
  s32 radius;
  s32 frames;
  s32 x;
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
    /* FAKEMATCH(wrapper): Iwram_CopyWords called through CopyWords; as a
     * direct call, 2 of 747 instructions differ in tla-en. */
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
    frames = 24;
    /* FAKEMATCH(barrier): an empty asm after Thunder Mine's 24, which puts
     * its arm over BRANCH_COST so if-conversion keeps both arms; as the bare
     * `frames = 24;`, 27 of 747 instructions differ in tla-en. */
    asm("");
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
        ball->velocityX = ((target.x << 16) - ball->x) / frames;
      } else {
        if (effect->side == 0) {
          ball->x = (source.x - 32) << 16;
        } else {
          ball->x = (source.x + 32) << 16;
        }
        ball->y = (source.y - 80) << 16;
        ball->velocityX = (((target.x - 32) << 16) - ball->x) / frames;
      }
      ball->velocityY = (((target.y - 32) << 16) - ball->y) / frames;
      Sound_PlayFar(221);
      for (i = 0, bolt = &work->particles[1]; i != 63; i++, bolt++) {
        bolt->z = Random16();
        bolt->velocityZ = (Random16() & 255) + 128;
        if (Random16() & 1) {
          bolt->velocityZ = -bolt->velocityZ;
        }
        bolt->variant = Random16() & 3;
      }
      // UB: the variants are cleared through one s32 pointer stepped a whole
      // step at a time, past the step's scalar members.
#ifdef UBFIX
      for (i = 0; i != 512; i++)
        gData_02014000[i].variant = -1;
#else
      {
        s32 *variant = &gData_02014000[0].variant;

        for (i = 0; i != 512; i++) {
          *variant = -1;
          variant += sizeof(struct EffectStep) / sizeof(s32);
        }
      }
#endif
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
      y = ball->y >> 16;
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
      Func_0818caa8(buffer, x, y, radius);
      for (i = 0; i != 4; i++) {
        // DEAD: i runs 0 to 3, so the mask changes nothing; the source keeps
        // it.
        s32 cell = i & 3;
        s32 half = radius / 2;
        s32 angle = Random16();
        s32 flashX =
            x + ((Trig_Sin(angle) * half) >> 16) - (gData_08197492[cell] >> 1);
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
            gData_08197492[cell], gData_08197498[cell]);
        HeapSlot_Release(188);
      }
      BattleEffectKernels_IntegrateVector2(ball, 64, 0);
    }

    // Thunder Mine's sparks bounce.
    if (mode == 0 && frame >= frames) {
      for (i = 0, dot = gData_02014000; i != 512; i++, dot++) {
        if (dot->variant >= 0) {
          s32 size = (dot->variant >> 3) + 2;
          s32 step;

          for (step = 0; step != 8; step++) {
            draw[0](canvas, sprites + gData_08197410[size - 1],
                    (dot->x >> 16) - size / 2, (dot->y >> 16) - size, size,
                    size * 2);
            BattleEffectKernels_IntegrateVector2(dot, 62, 0x200);
          }
          if ((dot->y >> 16) > 111) {
            dot->velocityY = -dot->velocityY * 48 / 64;
          }
          dot->variant--;
        }
      }
    }

    // The trailing bolts, each eight frames long.
    if (frame <= frames) {
      vertices = Heap_AllocEwram(32);
      model = Render_AllocateModelList(1);
      texture.logWidth = 5;
      texture.logHeight = 7;
      // DEAD: both kinds are overwritten by the 6 below; the source keeps them.
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
          /* FAKEMATCH(temporary): the fade negated through a local; as shade =
           * 64 - (frame - i) * 16, 4 of 747 instructions differ in tla-en. */
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
    BattleCamera_Shake(4, 4);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(SLOT_BLITTER);
  BattleEffect_EndCanvasLayer();
}

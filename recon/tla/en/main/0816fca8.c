// Draft of Func_0816fca8, decompiled from the listing: differ 644 of 747 (draft
// 751), the same 60 calls and control flow; allocation differs.

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

void BattleFx_BeginCanvasLayer(s32 bgControl);
void BattleFx_EndCanvasLayer(void); // BattleFx_EndCanvasLayer
void BattleFx_ProcessPendingGraphicsTransfer(void); // BattleFx_ProcessPendingGraphicsTransfer
void ObjectGroup_TickMemberTimers(void); // ObjectGroup_TickMemberTimers
void BattleFx_LoadBlitter(s32 slot, s32 modeAndFlags); // BattleFx_BuildBlitter
void Func_0814cd48(s32 actor, s32 palette, s32 pose, s32 timerSlot,
                   s32 timerFrames);              // ObjectGroup_UpdateMembers
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
void EffectPosition_ApplyAlternateStepAndYOffset(s32 id,
                                                 struct EffectPosition *out);
void BattleFx_SpawnObjects(s32 count, s32 resource, s32 mode);   // SpawnObjects
void BattleFx_SelectLivingTargets(struct BattleEffectArgument *effect); // SelectLivingTargets
void Func_080150e4(s32 angle); // SceneTransform_ApplyRoll
void BattleFx_StartActorMotion(s32 id, s32 force, s32 acceleration,
                               s32 speedLimit, s32 velocityY, s32 percent);
void Render_ApplyProjectedPlacementFar(void *object, s32 *request, s32 *source,
                                       s32 flags);
void BattleFx_FetchRectangleBlitters(s32 flip, void **out);
void Func_08118088(s32 actor, s32 motion); // BattleMotion_ApplyVariantMotionFar
void Func_081180e8(s32 phase);             // BattleEventRuntime_BeginPhaseFar
void Func_08158ce0(s32 randomMask, u32 shakeRange); // Camera_ApplyShake
void BattleFxKernels_IntegrateVector2(struct EffectStep *step, s32 gravity,
                                      s32 drag);
void **GetBattleObjectSlotFar(s32 objectId);
void *Runtime_BumpAllocateAlternatePool(s32 size);
void Resource_LoadAndDecompress(s32 resource, void *destination, s32 a, s32 b);
void *Resource_GetTableEntry(s32 resource);
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 order);
void Scheduler_RemoveCallback(void (*callback)(void));
void Runtime_ReleaseHeapBlock(s32 slot);
void Audio_PlayCue(s32 cue);
void WaitFrames(s32 frames);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);

void Render_BuildQuadStrip(void *mesh, s32 columns, s32 width, s32 height);
void BattleFxKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                      s32 gravity);
s32 Battle_GetObjectTableValueFar(s32 unit);
void Transform_SetLookAt(s32 *eye, s32 *target);
void BattleFx_BeginTiledCanvas(s32 bgControl);
void BattleFx_BeginTiledCanvasFilled(s32 bgControl);
void Transform_Push(void);
void Transform_Pop(void);

// The blitter's heap slot, as a byte offset into Ram_HeapSlots.
#define SLOT_BLITTER 104 // unk64[1]

// ⚓️'s blitter modes run the other way from The Broken Seal's.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT, BLIT_COPY };
#define BLITTER(mode, flags) ((mode) << 4 | (flags))
#define HI(v) (((s16 *)&(v))[1])

void Func_08196404(s32 slot, s32 widthShift, s32 heightShift, s32 flags,
                   s32 mode);

extern char ResourceId_TornadoSheet;
extern char ResourceId_Data134;
extern char ResourceId_LightningBoltSheet;
extern char ResourceId_VioletPaletteC;
extern u8 gMapCellBuffer[];
extern struct EffectStep gData_02014000[]; // the sparks
extern u16 gData_08197410[];               // gBattleFx_MoteCellOffsets
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
  u8 *sprites;
  DrawRectangle blit;
  struct EffectPosition source;
  struct EffectPosition target;
  struct ModelTexture texture;
  struct RenderModel *model;
  struct EffectStep *ball;
  struct EffectStep *bolt;
  struct EffectStep *spark;
  void *vertices;
  s32 radius;
  s32 frames;
  s32 frame;
  s32 i;
  s32 k;

  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  sprites = Ram_HeapSlots->unk64[0];
  BattleFx_BeginCanvasLayer(1);
  if (mode == 0) {
    REG_BLDALPHA = 0x1010;
  } else {
    REG_BLDALPHA = 0x810;
  }
  BattleFx_LoadBlitter(SLOT_BLITTER,
                BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  blit = (DrawRectangle)Ram_HeapSlots->unk64[1];
  radius = 0;
  Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, work->sheet + 0x800,
                             1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_LightningBoltSheet,
                             gMapCellBuffer, 1, 1);
  if (mode == 1) {
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_VioletPaletteC),
                    128);
  }

  // The bolts' four pictures, 24 by 120, each in a 32-wide cell.
  Iwram_ClearWords(work->sheet + 0x1456, 0x4000);
  for (k = 0; k != 4; k++) {
    for (i = 0; i != 120; i++) {
      u8 *source = gMapCellBuffer + k * 2880 + i * 24;
      u8 *dest = work->sheet + 0x145a + k * 0x1000 + i * 32;
      s32 j;

      for (j = 0; j != 24; j++) {
        *dest++ = *source++;
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
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0xc80);
  if (mode == 0) {
    frames = 24;
  } else {
    frames = 40;
  }

  for (frame = 0; frame != 50; frame++) {
    ball = work->particles;
    if (frame == 0) {
      s32 distance;

      EffectPosition_ApplyAlternateStepAndYOffset(effect->actor, &source);
      EffectPosition_ApplyAlternateStepAndYOffset(effect->actors[0], &target);
      if (mode == 0) {
        ball->x = source.x << 16;
        ball->y = (source.y - 32) << 16;
        distance = (target.x << 16) - ball->x;
      } else {
        if (effect->side == 0) {
          ball->x = (source.x - 32) << 16;
        } else {
          ball->x = (source.x + 32) << 16;
        }
        ball->y = (source.y - 80) << 16;
        distance = ((target.x - 32) << 16) - ball->x;
      }
      ball->velocityX = distance / frames;
      ball->velocityY = (((target.y - 32) << 16) - ball->y) / frames;
      Audio_PlayCue(221);
      for (i = 0, bolt = &work->particles[1]; i != 63; i++, bolt++) {
        bolt->z = Random16();
        bolt->velocityZ = (Random16() & 255) + 128;
        if (Random16() & 1) {
          bolt->velocityZ = -bolt->velocityZ;
        }
        bolt->variant = Random16() & 3;
      }
      for (i = 0; i != 512; i++) {
        gData_02014000[i].variant = -1;
      }
    }

    // Arrival.
    if (frame == frames) {
      for (i = 0, spark = gData_02014000; i != 128; i++, spark++) {
        s32 angle;
        s32 speed;

        spark->x = ball->x / 2;
        spark->y = ball->y;
        angle = Random16();
        speed = Random16() % 400 + 32;
        spark->velocityX = (speed * Trig_Sin(angle)) >> 7;
        spark->velocityY = (speed * Trig_Cos(angle)) >> 6;
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
        Audio_PlayCue(145);
      }
    }

    // The ball, growing to 48 (64), with four flashes about it.
    if (frame <= frames) {
      s32 x;
      s32 y;
      s32 largest;

      ball->variant++;
      x = ball->x >> 17;
      y = HI(ball->y);
      radius = frame * 2 + 16;
      if (mode == 0) {
        largest = 48;
      } else {
        largest = 64;
      }
      if (radius > largest) {
        radius = largest;
      } else {
        Func_0815b434(gMapCellBuffer, radius, 85536);
        if (mode == 1) {
          for (i = 0; i != radius * radius / 2; i++) {
            if (gMapCellBuffer[i] != 0) {
              gMapCellBuffer[i] = 64 - gMapCellBuffer[i];
            }
          }
        }
      }
      Func_0818caa8(gMapCellBuffer, x, y, radius);
      for (k = 0; k != 4; k++) {
        s32 cell = k & 3;
        s32 half = radius / 2;
        s32 angle = Random16();
        s32 flashX =
            x + ((half * Trig_Sin(angle)) >> 16) - (gData_08197492[cell] >> 1);
        s32 flashY =
            y + ((half * Trig_Cos(angle)) >> 16) - (gData_08197498[cell] >> 1);

        Func_08196404(188, 7, 7, 3 | Data_08198bee[Random16() & 3], 1);
        ((DrawRectangle)Ram_HeapSlots->unkb8[1])(
            canvas, work->sheet + 0x800 + gData_08197486[cell], flashX, flashY,
            gData_08197492[cell], gData_08197498[cell]);
        Runtime_ReleaseHeapBlock(188);
      }
      BattleFxKernels_IntegrateVector2(ball, 64, 0);
    }

    // Thunder Mine's sparks bounce.
    if (mode == 0 && frame >= frames) {
      for (i = 0, spark = gData_02014000; i != 512; i++, spark++) {
        if (spark->variant >= 0) {
          s32 size = (spark->variant >> 3) + 2;

          for (k = 0; k != 8; k++) {
            blit(canvas, sprites + gData_08197410[size - 1],
                 HI(spark->x) - size / 2, HI(spark->y) - size, size, size * 2);
            BattleFxKernels_IntegrateVector2(spark, 62, 0x200);
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
      vertices = Runtime_BumpAllocateAlternatePool(32);
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
          shade = -((frame - i) * 16 - 64);
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
      MemFree(model);
      MemFree(vertices);
    }
    Func_08158ce0(4, 4);
    ObjectGroup_TickMemberTimers();
    work->transferPending = 1;
    WaitFrames(1);
  }
  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  Runtime_ReleaseHeapBlock(104);
  BattleFx_EndCanvasLayer();
}

// Draft of BattleFx_RunDaedalusAlt, decompiled from the listing: differ 328
// of 575 (draft 573), the same 51 calls and control flow; allocation and
// stack slots differ.

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
void ResourceObject_ReleaseFar(void *object);
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

extern char ResourceId_FlashBurstSheet;
extern char ResourceId_OrangePaletteB;
extern char ResourceId_Data0C9;
extern u8 gMapCellBuffer[];
extern const u16 gData_08197410[];         // gBattleFx_MoteCellOffsets
extern const u8 Data_081995ec[];           // the seven strike frames
extern const s32 Data_08196ee8[2];         // the summon's placement source
extern const u8 Data_08199220[];           // the rings' mesh
extern const u8 gData_081991b0[];          // their corners
extern struct EffectStep gData_02014000[]; // the sparks
extern const u8 gData_08199210[];          // its four corners
extern u32 gCameraSceneParameters[];

#define BLITTER ((DrawRectangle)Ram_HeapSlots->unk64[1])
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT };
#define BLIT_MODE(mode, flags) ((mode) << 4 | (flags))
#define BLIT_CLIP (COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y)
#define HI(v) (((s16 *)&(v))[1])
#define DAEDALUS_FRAMES 108
#define DAEDALUS_SPARKS 64

// The summoned object's display fields that the scene sets.
struct SummonObject {
  u8 unk00[18];
  u16 unk12;
  u8 unk14[2];
  u8 unk16;
  u8 unk17;
};

// Abilities: Daedalus (the second scene).
//
// The summoned object glides in from the right and down over 92 frames
// while, from frame 56, a shaded disc swells at the centre and six rings of
// light spin out from it. At the seven frames of Data_081995ec every target
// is knocked back and flashes; from frame 80 eight bursts of eight sparks,
// one a frame, scatter over the canvas.
void BattleFx_RunDaedalusAlt(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle draw;
  struct ModelTexture texture;
  s32 source[2];
  s32 request[4];
  s32 frame;
  s32 i;
  s32 j;
  s32 burst;
  struct EffectStep *spark;

  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  Func_0813ba50();
  REG_WIN0V = 0x1080;
  REG_WININ = 0x2737;
  BattleFx_BeginCanvasLayer(0);
  REG_BLDALPHA = 0x1010;
  BattleFx_SpawnObjects(1, 0x296, 3);
  BattleFx_SelectLivingTargets(work->effect);
  BattleFx_LoadBlitter(104, BLIT_MODE(BLIT_ADDITIVE, BLIT_CLIP));
  draw = BLITTER;
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, work->sheet, 1,
                             0);
  work->transferMode = 2;
  work->transferValue = 50;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0xc80);
  work->fadeFrames = 24;
  work->fadeStep = 0;

  for (frame = 0, burst = -80 * 8; frame != DAEDALUS_FRAMES;
       frame++, burst += 8) {
    if (frame == 0) {
      work->transferMode = 2;
      work->transferValue = 50;
      Iwram_CopyWords((void *)BG_PLTT,
                      Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB),
                      128);
    }
    if (frame == 56) {
      Audio_PlayCue(140);
    }
    for (i = 0; i != 7; i++) {
      if (frame == Data_081995ec[i]) {
        for (j = 0; j != effect->count; j++) {
          BattleFx_StartActorMotion(effect->actors[j], 1, 0x20000, 0x80000,
                                    0x20000, 120);
          Func_0814cd48(effect->actors[j], 7, 5, j, 7);
        }
        work->shakeFrames = 4;
        Audio_PlayCue(145);
      }
    }

    // From frame 80: eight bursts of sparks, one a frame.
    if (frame > 79) {
      if (frame == 80) {
        for (i = 0; i != DAEDALUS_SPARKS; i++) {
          gData_02014000[i].variant = -1;
        }
        work->shakeFrames = 8;
        Iwram_FillWords(canvas, 0x3c00, 0x3f3f3f3f);
      }
      if ((u32)(frame - 80) <= 7) {
        s32 centerX = ((Random16() & 63) + 28) << 16;
        s32 centerY = ((Random16() & 31) + 80) << 16;

        for (j = 0, spark = &gData_02014000[burst]; j != 8; j++, spark++) {
          s32 speed = Random16() & 31;
          s32 angle = Random16();

          spark->x = centerX + (((Random16() & 15) - 8) << 16);
          spark->y = centerY + (((Random16() & 15) - 8) << 16);
          speed += 32;
          spark->velocityX = (Trig_Sin(angle) * speed) >> 3;
          spark->velocityY = (Trig_Cos(angle) * speed) >> 3;
          spark->variant = 0;
        }
      }
      for (i = 0, spark = gData_02014000; i != DAEDALUS_SPARKS; i++, spark++) {
        if ((u32)spark->variant <= 17) {
          draw(canvas, work->sheet + ((spark->variant / 3) << 11),
               HI(spark->x) - 16, HI(spark->y) - 48, 32, 64);
          BattleFxKernels_IntegrateVector2(spark, 62, -0x800);
          spark->variant++;
        }
      }
    }

    // The summoned object glides in over 92 frames.
    if (frame <= 91) {
      struct SummonObject *object;

      source[0] = Data_08196ee8[0];
      source[1] = Data_08196ee8[1];
      object = work->objects[0];
      object->unk17 = 32;
      object->unk12 = 0x8800;
      object->unk16 = 0;
      request[3] = 0;
      request[1] = 0xff0000;
      if (frame <= 63) {
        request[0] = (152 - frame / 4) << 16;
        request[2] = (frame << 17) - 0x400000;
      } else if ((u32)(frame - 64) <= 63) {
        s32 t = frame - 64;

        request[0] = (136 - t / 16) << 16;
        request[2] = ((t / 2) << 16) + 0x400000;
      } else {
        request[0] = 136 << 16;
        request[2] = 0x400000;
      }
      Render_ApplyProjectedPlacementFar(work->objects[0], request, source, 0);
    }

    // Frames 56 to 91: the disc and the rings.
    if ((u32)(frame - 56) <= 35) {
      s32 x = 0x400000;
      s32 size = 128;
      s32 grow = frame * 2 - 104;
      struct RenderModel *model;
      void *vertices;
      struct EffectStep *ring;
      s32 one;

      if (grow <= 64) {
        size = grow * 2;
        Func_0815b434(gMapCellBuffer, size, 0x10000);
      }
      Func_0818caa8(gMapCellBuffer, x >> 16, 0x500000 >> 16, size);
      if (frame == 56) {
        s32 delay = 0;

        for (i = 0, ring = work->particles; i != 16; i++, ring++) {
          ring->x = Random16() & 0xffff;
          ring->y = (Random16() & 255) + 128;
          if (Random16() & 1) {
            ring->y = -ring->y;
          }
          ring->variant = delay;
          delay -= 4;
        }
        Resource_LoadAndDecompress((s32)&ResourceId_Data0C9,
                                   work->sheet + 0x4000, 0, 0);
      }
      vertices = Runtime_BumpAllocateAlternatePool(32);
      model = Render_AllocateModelList(1);
      model->shade = 0;
      texture.logWidth = 5;
      texture.logHeight = 2;
      model->vertices = vertices;
      texture.pixels = work->sheet + 0x4000;
      model->kind = 7;
      model->texture = &texture;
      model->mesh = Data_08199220;
      one = 0x10000;
      for (i = 0, ring = work->particles; i != 6; i++, ring++) {
        ring->x += ring->y * 2;
        ring->variant += 2;
        if ((u32)ring->variant <= 47) {
          s32 k;

          Render_ResetTransformState();
          k = i & 3;
          Func_08015160((k << 17) + 0x420000 - 0x400000,
                        0x500000 - ((k + 2) << 16) - 0x400000, 0);
          Func_080151e4(0x8000, one, one);
          Func_080150e4(ring->x);
          Func_080151e4(Trig_Sin(ring->variant * 682) + one, 0x20000, one);
          Render_TransformVertices(gData_081991b0, vertices, 4);
          Render_DrawModelList(model);
        }
      }
      MemFree(model);
      MemFree(vertices);
    }
    Func_08158ce0(8, 8);
    ObjectGroup_TickMemberTimers();
    work->transferPending = 1;
    WaitFrames(1);
  }
  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  Runtime_ReleaseHeapBlock(104);
  ResourceObject_ReleaseFar(work->objects[0]);
  BattleFx_EndCanvasLayer();
}

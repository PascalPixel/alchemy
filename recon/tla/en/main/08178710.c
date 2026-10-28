// Draft of BattleEffect_RunRisingDragon, decompiled from the listing: differ 695 of
// 884 (draft 877), the same 65 calls and control flow; allocation differs.
// 2026-10-19 (run 33), from 773: the blitter as a pair (tagged below): 723;
// blit declared before sprites and camera, then frame right after it, so
// the spill slots run effect, work, canvas, the pair, frame, camera as the
// native's: 701; the aggregates declared arcs, the three positions, endX,
// endY, texture (the native's 148, 112-147, 104, 96, 88): 695. Every spill
// slot but the caller-save ones at sp+8 to sp+16 now has the native's place.
// Left: the native clears endX and endY with a zero register pair (a
// DImode store, as an initializer would) after the Task_Add call; a
// block with the two arrays initialized there moves three other spills
// (734). In the arc loops the native keeps &arcs[dragon * 2] in r11, j * 2
// as an induction variable at sp+24, the height spilled at sp+40 and the
// distance in r4 (caller-saved at sp+8); the allocation there differs.

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

void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_EndCanvasLayer(void); // BattleEffect_EndCanvasLayer
void BattleEffect_ProcessPendingGraphicsTransfer(void); // BattleEffect_ProcessPendingGraphicsTransfer
void ObjectGroup_CountDownMemberTimers(void); // ObjectGroup_CountDownMemberTimers
void BattleEffect_LoadBlitter(s32 slot, s32 modeAndFlags); // BattleEffect_BuildBlitter
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
void Func_081180e8(s32 phase);             // BattleEventRuntime_StartPhaseFar
void Func_08158ce0(s32 randomMask, u32 shakeRange); // Camera_ApplyShake
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 gravity,
                                      s32 drag);
void **GetBattleObjectSlotFar(s32 objectId);
void *Heap_AllocEwram(s32 size);
void Resource_LoadAndDecompress(s32 resource, void *destination, s32 a, s32 b);
void *Resource_GetTableEntry(s32 resource);
void Task_Add(void (*callback)(void), s32 order);
void Task_Remove(void (*callback)(void));
void HeapSlot_Release(s32 slot);
void Sound_PlayFar(s32 cue);
void WaitFrames(s32 frames);
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

void ObjectDispatch_SetAnimationSpeedFar(void *object, s32 value);
void Object_SetMode(void *object, s32 mode);

extern char ResourceId_SmokeBandSheet;
extern char ResourceId_Data134;
extern char ResourceId_ClawSlashSheet;
extern char ResourceId_BlueFlameSheet;
extern u8 gMapCellBuffer[];
extern u8 Data_02011000[];
extern u8 Data_02012000[];
extern const u16 gData_08197410[]; // gBattleEffect_MoteCells
extern const u8 gData_08199340[];  // a head's mesh
extern const u8 Data_081991e0[];   // its corners

#define DRAGON_SEGMENTS 33

// Abilities: Rising Dragon.
//
// One dragon (two at a higher level: effect->variant + 1) of smoke rises
// from the caster and arcs onto the target, each a scrolling textured strip
// of 33 segments with a clawed head; each strikes ten frames after the
// last, scattering 32 sparks.
void BattleEffect_RunRisingDragon(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  /* FAKEMATCH(frame): the blitter as a pair of which only the first is used;
   * as one two-word DImode pseudo its spill takes sp+68 and the unused sp+72
   * the native stack frame has. As a single DrawRectangle the stack frame is 160 bytes
   * (773 of 884 instructions differ in tla-en, 723 with the pair). */
  DrawRectangle blit[2];
  s32 frame;
  u8 *sprites;
  s32 *camera;
  void *caster;
  s32 arcs[4];
  struct EffectPosition source;
  struct EffectPosition target;
  struct EffectPosition hit;
  s32 endX[2];
  s32 endY[2];
  struct ModelTexture texture;
  struct RenderModel *model;
  struct EffectStep *spark;
  s32 dragon;
  s32 i;
  s32 j;
  s32 k;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  camera = Ram_HeapSlots->cameraWork;
  sprites = Ram_HeapSlots->unk64[0];
  BattleEffect_StartCanvasLayer(1);
  if (effect->side == 0) {
    BattleEffect_LoadBlitter(SLOT_BLITTER,
                  BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  } else {
    BattleEffect_LoadBlitter(SLOT_BLITTER,
                  BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                                        COMPOSITE_FLIP_X));
  }
  blit[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  REG_BLDALPHA = 0x1010;
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  endX[0] = 0;
  endX[1] = 0;
  endY[0] = 0;
  endY[1] = 0;
  caster = *GetBattleObjectSlotFar(effect->actor);
  ObjectDispatch_SetAnimationSpeedFar(caster, 48);
  Object_SetMode(caster, 2);
  EffectPosition_ProjectBattler(effect->actor, &source);
  EffectPosition_ProjectBattler(effect->actors[0], &target);
  Resource_LoadAndDecompress((s32)&ResourceId_SmokeBandSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_ClawSlashSheet,
                             work->sheet + 0x4000, 1, 1);
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_BlueFlameSheet), 128);
  REG_BG2PA = 0x80;
  Render_BuildQuadStrip(Data_02012000, 4, 32, 32);

  for (frame = 0; frame != effect->variant * 10 + 60; frame++) {
    Render_ResetTransformState();
    Transform_SetLookAt(camera, camera + 3);
    if (frame == 0) {
      arcs[0] = 0;
      arcs[1] = 0;
      arcs[2] = 0;
      arcs[3] = 0;
      for (i = 0; i != 64; i++) {
        work->particles[i].variant = -1;
      }
    }

    // Each dragon's strike.
    for (dragon = 0; dragon != effect->variant + 1; dragon++) {
      if (frame != dragon * 10 + 28) {
        continue;
      }
      EffectPosition_ProjectBattler(effect->actors[0], &hit);
      for (i = 0, spark = &work->particles[dragon * 32]; i != 32;
           i++, spark++) {
        s32 angle;
        s32 speed;

        spark->x = (hit.x / 2) << 16;
        spark->y = (hit.y - 16) << 16;
        angle = Random16();
        speed = Random16() & 255;
        spark->velocityX = (Trig_Sin(angle) * (speed + 127)) >> 6;
        spark->velocityY = (Trig_Cos(angle) * (speed + 127)) >> 6;
        spark->variant = (Random16() & 15) + 16;
      }
      Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
      work->shakeFrames = 8;
      if (frame == 28) {
        if (effect->variant == 0) {
          Func_081180e8(144);
        } else {
          Sound_PlayFar(144);
        }
        BattleEffect_StartActorMotion(effect->actors[0], 1, 0x20000, 0x40000,
                                  0x40000, 150);
      } else {
        Func_081180e8(144);
        BattleEffect_StartActorMotion(effect->actors[0], 1, 0x20000, 0x40000,
                                  0x70000, 200);
      }
      Func_0814cd48(effect->actors[0], 7, 5, 0, 8);
    }
    for (i = 0, spark = work->particles; i != 64; i++, spark++) {
      if (spark->variant >= 0) {
        s32 size = (spark->variant >> 3) + 2;

        blit[0](canvas, sprites + gData_08197410[size - 1],
             HI(spark->x) - size / 2, HI(spark->y) - size, size, size * 2);
        BattleEffectKernels_IntegrateVector2(spark, 60, 0x1000);
        spark->variant--;
      }
    }

    if (frame > 15) {
      // The arcs' heads and tails, 44 units a frame.
      arcs[0] = frame * 44 - 704;
      if (arcs[0] > 950) {
        arcs[0] = 950;
      }
      if (frame > 31) {
        arcs[1] = frame * 44 - 1408;
        if (arcs[1] > 950) {
          arcs[1] = 950;
        }
      }
      if (frame > 29) {
        arcs[2] = frame * 44 - 1320;
        if (arcs[2] > 950) {
          arcs[2] = 950;
        }
      }
      if (frame > 45) {
        arcs[3] = frame * 44 - 2024;
        if (arcs[3] > 950) {
          arcs[3] = 950;
        }
      }

      // The first dragon arcs over, the second under.
      for (j = 0; j != DRAGON_SEGMENTS; j++) {
        s32 distance = source.x / 4 - target.x / 4;
        s32 angle = j * (arcs[0] - arcs[1]) + arcs[1] * 33;
        s32 height;
        s32 width;
        s32 radiusX;
        s32 radiusY;
        s8 *corner;

        if (distance < 0) {
          distance = -distance;
        }
        height = distance / 3;
        width = j * 2 + 16;
        if (width > 32) {
          width = 32;
        }
        width = width * 20768 / 0x10000;
        radiusY = height - width / 2;
        radiusX = distance - width / 2;
        corner = (s8 *)gMapCellBuffer + j * 8;
        for (k = 0; k != 2; k++) {
          corner[0] = ((radiusX * Trig_Cos(angle)) >> 16) - distance;
          corner[1] = -((radiusY * Trig_Sin(angle)) >> 16);
          corner[2] = 0;
          radiusX += width;
          radiusY += width;
          corner += 4;
        }
        if (j == 32) {
          endX[0] = ((distance * Trig_Cos(angle)) >> 16) - distance;
          endY[0] = -((height * Trig_Sin(angle)) >> 16);
        }
      }
      for (j = 0; j != DRAGON_SEGMENTS; j++) {
        s32 distance = source.x / 4 - target.x / 4;
        s32 angle = j * (arcs[2] - arcs[3]) + arcs[3] * 33;
        s32 height;
        s32 width;
        s32 radiusX;
        s32 radiusY;
        s8 *corner;

        if (distance < 0) {
          distance = -distance;
        }
        height = distance * 32 / 36;
        width = j * 4;
        if (width > 24) {
          width = 24;
        }
        width = width * 20768 / 0x10000;
        radiusY = height - width / 2;
        radiusX = distance - width / 2;
        corner = (s8 *)gMapCellBuffer + 0x108 + j * 8;
        for (k = 0; k != 2; k++) {
          corner[0] = ((radiusX * Trig_Cos(angle)) >> 16) - distance;
          corner[1] = (radiusY * Trig_Sin(angle)) >> 16;
          corner[2] = 0;
          radiusY += width;
          radiusX += width;
          corner += 4;
        }
        if (j == 32) {
          endX[1] = ((distance * Trig_Cos(angle)) >> 16) - distance;
          endY[1] = (height * Trig_Sin(angle)) >> 16;
        }
      }

      model = Render_AllocateModelList(1);
      model->kind = 6;
      model->scrollV = 0;
      model->texture = &texture;
      model->shade = 0;
      model->vertices = Data_02011000;
      for (dragon = 0; dragon != effect->variant + 1; dragon++) {
        if (arcs[dragon * 2 + 1] >= arcs[dragon * 2]) {
          continue;
        }
        texture.logWidth = 6;
        texture.logHeight = 5;
        texture.pixels = work->sheet;
        model->mesh = Data_02012000;
        model->scrollU = 127 & ~(frame << 3);
        Render_ResetTransformState();
        if (effect->side == 0) {
          Func_08015160((source.x / 2 - 68) << 16, (source.y << 16) - 0x600000,
                        0);
        } else {
          Func_08015160((source.x / 2 - 52) << 16, (source.y << 16) - 0x600000,
                        0);
        }
        Func_080151e4(0x8000, 0x10000, 0x8000);
        if (effect->side == 1) {
          SceneTransform_ApplyYaw(0x8000);
        }
        Func_080150e4(dragon * 14192 - 0x1000);
        Func_0801521c(0x40000);
        Render_TransformVertices(gMapCellBuffer + dragon * 264, Data_02011000, 66);
        Render_DrawModelList(model);
        if (frame < dragon * 16 + 40) {
          texture.logWidth = 6;
          model->scrollU = 0;
          texture.logHeight = 6;
          model->mesh = gData_08199340;
          texture.pixels = work->sheet + 0x4000;
          Func_08015160(endX[dragon] << 16, endY[dragon] << 16, 0);
          Func_080150e4(0x3060 - (arcs[dragon * 2] << 5));
          Func_0801521c(20768);
          Render_TransformVertices(Data_081991e0, Data_02011000, 4);
          Render_DrawModelList(model);
        }
      }
      Heap_Release(model);
    }
    ObjectGroup_CountDownMemberTimers();
    Func_08158ce0(2, 2);
    work->transferPending = 1;
    WaitFrames(1);
  }
  ObjectDispatch_SetAnimationSpeedFar(caster, 16);
  HeapSlot_Release(104);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

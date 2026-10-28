// Draft of BattleEffect_RunRagingHeat, decompiled from the listing: differ 374 of
// 669 (draft 673), the same 54 calls and control flow; allocation differs.
// 2026-10-19 (run 33), from 410: the plain and the flipped blitter as one
// pair (draw[0] at sp+48, draw[1] at sp+52, as the native) with frame
// declared before it and sprites after: 390; centerX and baseY declared
// before the model, spark and vertex locals: 374. The slots from sp+64 down
// to sp+36 are now the native's. Left: the native's hoisted
// (centerX - ...) << 16 takes sp+32 above vertices (28) and shade (24),
// as if those two had registers until reload spilled them; here the order
// is reversed.

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
void BattleEffect_CountDownBattlerTints(void); // BattleEffect_CountDownBattlerTints
void BattleEffect_LoadBlitter(s32 slot, s32 modeAndFlags); // BattleEffect_BuildBlitter
void Func_0814cd48(s32 actor, s32 palette, s32 pose, s32 timerSlot,
                   s32 timerFrames);              // Actor_UpdateMembers
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

void Func_0815f0a0(s32 palette);

extern char ResourceId_CornerRuneBSheet;
extern char ResourceId_Data134;
extern char ResourceId_FlashBurstSheet;
extern char ResourceId_PinkBurstSheet;
extern char ResourceId_Data0C2;
extern char ResourceId_OrangePaletteB;
extern u8 gMapCellBuffer[];
extern u8 Data_02011000[];
extern struct EffectStep gData_02015000[]; // the embers
extern const u16 gData_08197410[];         // gBattleEffect_MoteCells
// Per level (effect->variant): the sparks, their spread, the rings' size
// and the embers.
extern const u8 Data_08199400[][4];
extern const u8 gData_08199364[]; // the ring's mesh
extern const u8 gData_08199210[]; // its corners

#define RAGING_HEAT_FRAMES 96

// Abilities: Raging Heat.
//
// A ring of heat spins over the targets' middle and swells; from frame 56 a
// pillar of flame (two mirrored halves) falls onto them, bursting into
// sparks and embers when it lands, under a second, fading ring.
void BattleEffect_RunRagingHeat(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  // The plain and the flipped blitter, as one pair.
  DrawRectangle draw[2];
  u8 *sprites;
  struct EffectPosition targets[8];
  struct ModelTexture texture;
  s32 centerX;
  s32 baseY;
  struct RenderModel *model;
  struct EffectStep *spark;
  struct EffectStep *ember;
  void *vertices;
  s32 shade;
  s32 i;
  s32 j;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sprites = Ram_HeapSlots->unk64[0];
  BattleEffect_StartCanvasLayer(1);
  REG_BLDALPHA = 0xa10;
  BattleEffect_LoadBlitter(SLOT_BLITTER,
                BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  BattleEffect_LoadBlitter(188,
                BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                                           COMPOSITE_FLIP_X));
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
  for (i = 0; i != effect->count; i++) {
    EffectPosition_ProjectScaledBattler(effect->actors[i], &targets[i]);
  }
  centerX = targets[0].x + (targets[effect->count - 1].x - targets[0].x) / 2;
  baseY = targets[0].y - 48;
  Resource_LoadAndDecompress((s32)&ResourceId_CornerRuneBSheet, gMapCellBuffer,
                             1, 1);
  Func_0816ae40(gMapCellBuffer, work->sheet, 64, 64);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                             work->sheet + 0x4000, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_PinkBurstSheet, gMapCellBuffer, 1,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C2, Data_02011000, 0, 0);
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  for (frame = 0; frame != RAGING_HEAT_FRAMES; frame++) {
    if (frame == 0) {
      for (i = 0; i != 64; i++) {
        work->particles[i].variant = 24;
      }
      for (i = 0; i != 256; i++) {
        gData_02015000[i].variant = -1;
      }
      Sound_PlayFar(140);
    }
    if ((u32)(frame - 40) <= 15) {
      Func_0815f0a0((s32)&ResourceId_OrangePaletteB);
    }
    if (frame == 56) {
      Sound_PlayFar(144);
    }

    // The pillar falls eight lines a frame and bursts where it lands.
    if (frame > 55) {
      s32 y = baseY + frame * 8 - 480;

      if ((u32)(y - 52) <= 7) {
        for (i = 0, spark = work->particles; i != 32; i++, spark++) {
          u32 spread;

          spark->variant = -i / 4 + 2;
          spark->x = (centerX / 2) << 16;
          spark->y = ((Random16() & 15) + y + 16) << 16;
          spread = Data_08199400[effect->variant][1];
          spark->velocityX = ((Random16() % spread) - (spread >> 1)) << 12;
          spark->velocityY = (192 - (Random16() & 255)) << 10;
        }
        for (i = 0, ember = gData_02015000;
             i != Data_08199400[effect->variant][3]; i++, ember++) {
          s32 angle;
          s32 speed;

          ember->x = (centerX / 2) << 16;
          ember->y = ((Random16() & 15) + y + 24) << 16;
          angle = (Random16() & 0x7fff) + 0x4000;
          speed = (Random16() & 255) + 127;
          ember->velocityX = (speed * Trig_Sin(angle)) >> 5;
          ember->velocityY = (speed * Trig_Cos(angle)) >> 5;
          ember->variant = (Random16() & 15) + 16;
        }
        Func_081180e8(145);
        for (i = 0; i != effect->count; i++) {
          Func_08118088(effect->actors[i], 0);
          Func_0814cd48(effect->actors[i], 7, 5, i, 16);
        }
        work->shakeFrames = 16;
      }
      if (y <= 52) {
        draw[0](canvas, gMapCellBuffer, centerX / 2 - 24, y, 24, 64);
        draw[1](canvas, gMapCellBuffer, centerX / 2, y, 24, 64);
      }
      for (i = 0, spark = work->particles;
           i != Data_08199400[effect->variant][0]; i++, spark++) {
        if ((u32)spark->variant <= 23) {
          if (spark->variant > 1) {
            draw[0](canvas, work->sheet + 0x4000 + ((spark->variant / 4) << 11),
                 HI(spark->x) - 16, HI(spark->y) - 32, 32, 64);
          }
          BattleEffectKernels_IntegrateVector2(spark, 60, -0x1000);
        }
        spark->variant++;
      }
      for (i = 0, ember = gData_02015000;
           i != Data_08199400[effect->variant][3]; i++, ember++) {
        if (ember->variant >= 0) {
          s32 size = (ember->variant >> 3) + 1;

          draw[0](canvas, sprites + gData_08197410[size - 1],
               HI(ember->x) - size / 2, HI(ember->y) - size, size, size * 2);
          BattleEffectKernels_IntegrateVector2(ember, 60, 0x1000);
          ember->variant--;
        }
      }
    }

    // The rings: the spinning one, and from frame 56 the fading one.
    vertices = Heap_AllocEwram(32);
    model = Render_AllocateModelList(1);
    shade = 0;
    model->kind = 7;
    model->texture = &texture;
    model->mesh = gData_08199364;
    model->vertices = vertices;
    model->shade = shade;
    texture.logWidth = 7;
    texture.logHeight = 7;
    for (j = 0; j != 2; j++) {
      s32 size = Data_08199400[effect->variant][2];
      s32 largest = size << 10;
      s32 step = size << 5;
      s32 scale;

      Render_ResetTransformState();
      Func_080151e4(0x8000, 0x10000, 0x8000);
      if (j == 0) {
        Func_08015160((centerX << 16) - 0x800000, (baseY << 16) - 0x400000, 0);
      } else {
        Func_08015160((centerX << 16) - 0x800000,
                      ((baseY + 8) << 16) - 0x400000, 0);
      }
      SceneTransform_ApplyPitch(2000);
      if (frame <= 47) {
        SceneTransform_ApplyPitch((48 - frame) << 9);
      }
      if (j == 0) {
        scale = step * (frame - 16);
        if (scale > largest) {
          scale = largest;
        }
        if (frame > 60) {
          scale = largest - step * (frame * 2 - 120);
        }
        texture.pixels = work->sheet;
        SceneTransform_ApplyYaw(-frame * 3 << 8);
      } else {
        scale = step * (frame * 4 - 224);
        if (scale < 0) {
          continue;
        }
        shade = 512 - frame * 8;
        model->shade = shade;
        texture.pixels = Data_02011000;
        scale += 0x4000;
      }
      if (scale > 0 && shade > -64) {
        Func_0801521c(scale);
        Render_TransformVertices(gData_08199210, vertices, 4);
        Render_DrawModelList(model);
      }
    }
    Heap_Release(model);
    Heap_Release(vertices);
    Func_08158ce0(8, 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    WaitFrames(1);
  }
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(188);
  HeapSlot_Release(104);
  BattleEffect_EndCanvasLayer();
}

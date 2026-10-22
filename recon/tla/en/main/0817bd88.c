// Draft of BattleEffect_RunFirePuppet, decompiled from the listing: differ 272 of
// 543 (draft 537), the same 49 calls and control flow; allocation differs.
// 2026-10-19 (run 33), from 364: the arm's sweep written as length << 5 at
// each use, so the native's two copies of it (sp+36 and sp+32) appear and
// the frame is its 92 bytes: 317; frame declared after canvas, its slot
// above camera's: 289; the segment's angle and vertex offset written as
// j * length and j * 8, which loop.c reduces after the hoisted camera + 3 as
// in the native's slot order: 272. alchemy-devtools permute (6 minutes from 272):
// nothing better. Left: inside the segment loop the native keeps radiusX in
// r6 and radiusY in r7 and saves wobble, width and the angle around the
// calls (sp+16, 12, 8); here the allocation differs there.

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
void ObjectGroup_TickMemberTimers(void); // ObjectGroup_TickMemberTimers
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

// The target's motion object, as the effect reads it.
struct PuppetTarget {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  s32 unk14;
  s32 unk18;
  s32 unk1c;
};

extern char ResourceId_Data0D3;
extern char ResourceId_Data134;
extern char ResourceId_Data0F2;
extern char ResourceId_MercuryDjinnSheet;
extern u8 gMapCellBuffer[];
extern u8 Data_02011000[];
extern u8 Data_02012000[];
extern const u8 gData_08199340[]; // the hand's mesh
extern const u8 Data_081991e0[];  // its corners

#define FIRE_PUPPET_FRAMES 103
#define PUPPET_SEGMENTS 33

// Abilities: Fire Puppet.
//
// From the caster rises a flaming arm (33 segments of two vertices each, a
// scrolling textured strip) that reaches up and over to 1424 units, with a
// hand at its end; at frame 54 it strikes, hitting every target, and from
// frame 74 it shrinks back. The target's position is kept in the work's
// last particle.
void BattleEffect_RunFirePuppet(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  u8 *sprites;
  s32 *camera;
  struct PuppetTarget *target;
  struct EffectPosition pos;
  struct ModelTexture texture;
  struct RenderModel *model;
  s32 endX;
  s32 endY;
  s32 i;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  camera = Ram_HeapSlots->cameraWork;
  sprites = Ram_HeapSlots->unk64[0];
  BattleEffect_StartCanvasLayer(0);
  if (effect->side == 0) {
    BattleEffect_LoadBlitter(SLOT_BLITTER,
                  BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  } else {
    BattleEffect_LoadBlitter(SLOT_BLITTER,
                  BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                                        COMPOSITE_FLIP_X));
  }
  REG_BLDALPHA = 0x1010;
  work->transferMode = 2;
  work->transferValue = 50;
  Scheduler_AddOrUpdateCallback(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  target = *GetBattleObjectSlotFar(effect->actors[0]);
  endX = 0;
  endY = 0;
  work->particles[63].x = target->x;
  work->particles[63].y = target->y;
  work->particles[63].z = target->z;
  work->particles[63].velocityX = target->unk18;
  work->particles[63].velocityY = target->unk1c;
  Resource_LoadAndDecompress((s32)&ResourceId_Data0D3, work->sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0F2, work->sheet + 0x4000, 0,
                             0);
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_MercuryDjinnSheet),
                  128);
  REG_BG2PA = 0x80;
  Render_BuildQuadStrip(Data_02012000, 4, 32, 32);

  for (frame = 0; frame != FIRE_PUPPET_FRAMES; frame++) {
    Render_ResetTransformState();
    Transform_SetLookAt(camera, camera + 3);
    if (frame == 0) {
      Func_0814cd48(effect->actor, 6, 4, 0, -1);
    }
    if (frame == 8) {
      Audio_PlayCue(103);
    }
    if (frame == 100) {
      Func_0814cd48(effect->actor, 0, 0, 0, -1);
    }
    if (frame == 54) {
      Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
      Audio_PlayCue(212);
      Func_081180e8(212);
      work->shakeFrames = 8;
      for (i = 0; i != effect->count; i++) {
        BattleEffect_StartActorMotion(effect->actors[i], 1, 0x20000, 0x40000,
                                  0x40000, 150);
        Func_0814cd48(effect->actors[i], 7, 5, i, 8);
      }
    }
    if (frame > 15) {
      s32 length;
      s32 j;

      length = frame * 44 - 704;
      if (length > 1424) {
        length = 1424;
      }
      if (frame > 73) {
        length = 4680 - frame * 44;
      }

      // The arm's vertices, thicker at its root.
      for (j = 0; j != PUPPET_SEGMENTS; j++) {
        s32 width = 50 - ((Trig_Sin(j * 862 + 4310) * 44) >> 16);
        s32 wobble = j * ((Trig_Sin((frame + j) << 11) << 4) >> 16) / 32;
        s32 outer = width + wobble;
        s32 radiusY = j + wobble + 32;
        s32 radiusX = wobble + 16;
        s8 *corner = (s8 *)gMapCellBuffer + j * 8;
        s32 k;

        for (k = 0; k != 2; k++) {
          corner[0] = ((radiusX * Trig_Cos(j * length)) >> 16) - outer / 2 - 16;
          corner[1] = -((radiusY * Trig_Sin(j * length)) >> 16);
          corner[2] = 0;
          radiusY += width;
          radiusX += width;
          corner += 4;
        }
        if (j == 32) {
          endX = (((width / 2 + wobble + 16) * Trig_Cos(length << 5)) >> 16) -
                 outer / 2 - 16;
          endY = -(((width / 2 + wobble + 64) * Trig_Sin(length << 5)) >> 16);
        }
      }

      model = Render_AllocateModelList(1);
      texture.logWidth = 7;
      texture.logHeight = 5;
      texture.pixels = work->sheet;
      model->mesh = Data_02012000;
      model->vertices = Data_02011000;
      model->shade = 0;
      model->kind = 6;
      model->texture = &texture;
      EffectPosition_ProjectScaledBattler(effect->actor, &pos);
      model->scrollV = 0;
      model->scrollU = 127 & ~(frame << 2);
      Render_ResetTransformState();
      Func_08015160((pos.x / 2 - 64) << 16, (pos.y << 16) + 0xffd60000, 0);
      Func_080151e4(0x8000, 0x10000, 0x8000);
      if (effect->side == 1) {
        SceneTransform_ApplyYaw(0x8000);
      }
      Func_080150e4(0x4000);
      if (frame > 73) {
        Func_0801521c((212 << 10) - (frame << 11));
      }
      Render_TransformVertices(gMapCellBuffer, Data_02011000, 66);
      Render_DrawModelList(model);

      // The hand at the arm's end.
      model->mesh = gData_08199340;
      texture.logWidth = 6;
      texture.logHeight = 6;
      model->vertices = Data_02011000;
      model->scrollU = 0;
      model->scrollV = 0;
      texture.pixels = work->sheet + 0x4000;
      model->shade = 0;
      Func_08015160(endX << 16, endY << 16, 0);
      Func_080150e4(22384 - (length << 5));
      Func_0801521c(length * 23 + 0x8000);
      Render_TransformVertices(Data_081991e0, Data_02011000, 4);
      Render_DrawModelList(model);
      MemFree(model);
    }
    ObjectGroup_TickMemberTimers();
    Func_08158ce0(4, 4);
    work->transferPending = 1;
    WaitFrames(1);
  }
  Runtime_ReleaseHeapBlock(104);
  Scheduler_RemoveCallback(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

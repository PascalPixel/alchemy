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
void BattleEffect_LoadBlitter(s32 slot,
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
void Task_Remove(void (*func)(void));
void HeapSlot_Release(s32 slot);
void Sound_PlayFar(s32 cue);
void wait(s32 frames);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);

void BattleEffect_StartTiledCanvas(s32 bgControl);
void BattleEffect_StartTiledCanvasFilled(s32 bgControl);
void Transform_Push(void);
void Transform_Pop(void);

// The scythe, in the work's first particle: swing is its velocityZ and timer
// its variant.
struct Scythe {
  s32 x;
  s32 y;
  s32 z;
  s32 velocityX;
  s32 velocityY;
  s32 swing;
  s32 timer;
};

extern char ResourceId_Data0F5;
extern u8 gMapCellBuffer[];
extern u8 Data_020112e8[];        // the reaper's raised pose
extern u8 Data_0201276b[];        // the blade's texture
extern u8 Data_0201476b[];        // the handle's texture
extern u8 Data_0201576b[];        // the flash's texture
extern const u8 Data_08199608[];  // the blade's mesh
extern const u8 Data_0819962c[];  // the handle's mesh
extern const u8 Data_08199650[];  // their corners
extern const u8 Data_08199244[];  // the flash's mesh
extern const u8 gData_081991f0[]; // its corners

// The blitter's heap slot, as a byte offset into Ram_HeapSlots.
#define SLOT_BLITTER 104 // unk64[1]

// ⚓️'s blitter modes run the other way from The Broken Seal's.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT, BLIT_COPY };
#define BLITTER(mode, flags) ((mode) << 4 | (flags))

#define LIFE_SHEAR_FRAMES 90

// Abilities: Life Shear.
//
// The reaper (a tiled background picture) glides in from the far side and
// swings its scythe (a two-part model) through the target at frame 50, with
// a four-blade flash; on the right side everything is mirrored. From frame
// 62 the reaper drifts off and the scythe falls away.
void BattleEffect_RunLifeShear(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  // Unused: blit[1] gives the stack frame its 8-byte spill at sp+32.
  DrawRectangle blit[2];
  struct EffectPosition target;
  struct ModelTexture texture;
  s32 reaperX;
  s32 reaperY;
  s32 speed;
  struct EffectPosition *position;
  s32 shift;
  s32 frame;
  void *vertices;
  struct RenderModel *model;
  struct Scythe *scythe;
  s32 i;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  if (effect->side == 0) {
    BattleEffect_StartTiledCanvas(SPLIT_CONSTANT(0x2000, 1));
  } else {
    BattleEffect_StartTiledCanvasFilled(SPLIT_CONSTANT(0x2000, 1));
  }
  REG_BG2PA = 0x100;
  REG_BLDALPHA = 0x610;
  Resource_LoadAndDecompress((s32)&ResourceId_Data0F5, gMapCellBuffer, 1, 1);
  if (effect->side == 0) {
    BattleEffect_LoadBlitter(
        SLOT_BLITTER,
        BLITTER(BLIT_TRANSPARENT, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  } else {
    BattleEffect_LoadBlitter(SLOT_BLITTER,
                             BLITTER(BLIT_TRANSPARENT, COMPOSITE_CLIP_X |
                                                           COMPOSITE_CLIP_Y |
                                                           COMPOSITE_FLIP_X));
  }
  blit[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  work->transferMode = 1;
  work->transferValue = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  reaperX = 0;
  reaperY = 0;
  speed = 0;
  EffectPosition_ProjectScaledBattler(effect->actors[0], position = &target);

  for (frame = 0; frame != LIFE_SHEAR_FRAMES; frame++) {
    s32 x;
    s32 phase;
    s32 scale;
    s32 timer;
    s32 zero;
    s32 still;
    s32 width;

    shift = 0;
    if (frame == 0) {
      Sound_PlayFar(206);
      if (effect->side == 0) {
        work->particles[0].x = 0x200000;
        work->particles[0].y = 0xffc00000;
        work->particles[0].z = 0xffffe3d0;
      } else {
        work->particles[0].x = 0xffe00000;
        work->particles[0].y = 0xffc00000;
        work->particles[0].z = 0xffffe3d0;
      }
      work->particles[0].velocityZ = 0;
      work->particles[0].variant = 0;
      reaperY = 0x180000;
      if (effect->side == 0) {
        reaperX = (position->x + 176) << 16;
        speed = 0xf0000;
      } else {
        reaperX = (position->x - 304) << 16;
        speed = 0xfff10000;
      }
    }
    if (frame > 61) {
      if (effect->side == 0) {
        reaperX += 0x20000;
      } else {
        reaperX -= 0x20000;
      }
      reaperY -= 0x4aaaa;
    }
    reaperX -= speed;
    speed = speed * 60 / 64;
    x = reaperX >> 16;
    if (x > 120) {
      shift = x - 120;
      x = 120;
    } else if (x < 0) {
      shift = x;
      x = 0;
    }
    REG_BG2X = -x << 8;
    if ((u32)(frame - 24) <= 39) {
      x = effect->side == 0 ? 64 : 5;
      blit[0](canvas, Data_020112e8, x + shift, reaperY >> 16, 59, 89);
    } else {
      if (effect->side == 0) {
        x = frame * 2 + 16;
        if (x > 64) {
          x = 64;
        }
      } else {
        x = 57 - frame * 2;
        if (x <= 8) {
          x = 9;
        }
      }
      blit[0](canvas, gMapCellBuffer, x + shift, reaperY >> 16, 55, 88);
    }

    // The scythe's path: still, a backswing at 24, the strike at 33.
    i = 0;
    still = 0;
    scythe = (struct Scythe *)work->particles;
    for (; i != 1; i++, scythe++) {
      if (++scythe->timer <= 0) {
        continue;
      }
      if (scythe->timer == 1) {
        scythe->velocityX = still;
        scythe->velocityY = still;
      }
      if (scythe->timer == 24) {
        scythe->velocityX = -0x40000;
        scythe->velocityY = 0x80000;
      }
      if (scythe->timer == 33) {
        scythe->velocityX = 0x40000;
        scythe->velocityY = -0x80000;
        work->shakeFrames = 4;
        speed = 0;
        Sound_PlayFar(133);
        Func_0814cd48(effect->actors[0], 7, 5, 0, 8);
      }
      if (scythe->timer == 35) {
        scythe->velocityX = still;
        scythe->velocityY = still;
      }
      if (frame > 63) {
        s32 fall = (frame << 7) - 0x2000;

        scythe->velocityY -= 0x4000;
        if (fall > 0x400) {
          fall = 0x400;
        }
        scythe->z -= fall;
      }
      if (scythe->timer > 8) {
        if (effect->side == 0) {
          scythe->x += scythe->velocityX;
        } else {
          scythe->x -= scythe->velocityX;
        }
        scythe->y += scythe->velocityY;
      }
    }

    vertices = Heap_AllocEwram(32);
    model = Render_AllocateModelList(1);
    zero = 0;
    phase = frame - 50;
    if (phase == -4) {
      Sound_PlayFar(212);
    }
    if (phase == 0) {
      Func_081180e8(134);
      work->shakeFrames = 12;
      Func_0814cd48(effect->actors[0], 7, 5, 0, 8);
      Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
      Func_08118088(effect->actors[0], 4);
    }
    timer = work->particles[0].variant;
    if ((u32)(frame - 44) <= 13) {
      work->particles[0].velocityZ++;
    }
    if (timer >= 0) {
      scale = 0xb1e0;
      if (frame > 63) {
        scale = scale - (frame << 9) + 0x8000;
      }
      width = 7;
      if (scale > 0) {
        model->shade = zero;
        model->kind = 9;
        model->vertices = vertices;
        model->texture = &texture;
        Render_ResetTransformState();
        Func_08015160(work->particles[0].x + (shift << 16),
                      work->particles[0].y, 0);
        if (effect->side == 1) {
          SceneTransform_ApplyYaw(0x8000);
        }
        Func_080150e4(work->particles[0].z);
        Func_0801521c(scale);
        texture.logHeight = 5;
        texture.pixels = Data_0201476b;
        texture.logWidth = width;
        model->mesh = Data_0819962c;
        Transform_Push();
        Func_080150e4((Trig_Sin(work->particles[0].velocityZ << 11) >> 3) -
                      0x2000);
        Render_TransformVertices(Data_08199650, vertices, 4);
        Render_DrawModelList(model);
        Transform_Pop();
        model->mesh = Data_08199608;
        texture.logWidth = width;
        texture.logHeight = 6;
        texture.pixels = Data_0201276b;
        Func_080150e4(0x2000 -
                      (Trig_Sin(work->particles[0].velocityZ << 11) >> 3));
        Func_080151e4(0x10000, 0x15f90, 0x10000);
        Render_TransformVertices(Data_08199650, vertices, 4);
        Render_DrawModelList(model);
      }

      // The flash: four blades, shrinking over eight frames.
      if ((u32)phase <= 7) {
        scale = 0x78e20 - (frame << 13);
        Render_ResetTransformState();
        if (effect->side == 0) {
          Func_08015160(0xfff00000, 0, 0);
        } else {
          Func_08015160(0x100000, 0, 0);
        }
        Func_080151e4(scale, scale, scale);
        if (effect->side == 1) {
          SceneTransform_ApplyYaw(0x8000);
        }
        model->kind = 6;
        model->mesh = Data_08199244;
        texture.logWidth = 5;
        texture.logHeight = 5;
        texture.pixels = Data_0201576b;
        for (i = 0; i != 4; i++) {
          Transform_Push();
          Func_080150e4(i * 0x4000 + 0x5650);
          Func_080151e4(0x4000, ((i & 1) << 15) + 0xc000, 0x8000);
          Func_0801521c(0x10000);
          Render_TransformVertices(gData_081991f0, vertices, 4);
          Render_DrawModelList(model);
          Transform_Pop();
        }
      }
    }
    Heap_Release(model);
    Heap_Release(vertices);
    Func_08158ce0(8, 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(104);
  BattleEffect_EndCanvasLayer();
}

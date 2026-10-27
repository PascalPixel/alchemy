// Draft of BattleEffect_RunLifeShear, decompiled from the listing: differ 318 of
// 602 (draft 598), the same 55 calls and control flow; allocation differs.
// 2026-10-19 (run 33): the blitter as a pair (tagged below), which gives
// the native's 64-byte frame and its unused sp+36: 334 to 318. Left: a
// six-use spill slot at sp+28 the native keeps in a register, and with it
// the order of the slots below.

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
void WaitTicks(s32 ticks);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);

void BattleEffect_StartTiledCanvas(s32 bgControl);
void BattleEffect_StartTiledCanvasFilled(s32 bgControl);
void Transform_Push(void);
void Transform_Pop(void);

// The scythe, in the work's first particle.
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
  /* FAKEMATCH(frame): the blitter as a pair of which only the first is used;
   * as one two-word DImode pseudo its spill takes sp+32 and the unused sp+36
   * the native frame has. As a single DrawRectangle the frame is 60 bytes
   * (334 of 602 instructions differ in tla-en, 318 with the pair). */
  DrawRectangle blit[2];
  struct EffectPosition target;
  struct ModelTexture texture;
  struct RenderModel *model;
  struct Scythe *scythe;
  void *vertices;
  s32 reaperX;
  s32 reaperY;
  s32 speed;
  s32 shift;
  s32 frame;
  s32 i;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  if (effect->side == 0) {
    BattleEffect_StartTiledCanvas(0x2001);
  } else {
    BattleEffect_StartTiledCanvasFilled(0x2001);
  }
  REG_BG2PA = 0x100;
  REG_BLDALPHA = 0x610;
  Resource_LoadAndDecompress((s32)&ResourceId_Data0F5, gMapCellBuffer, 1, 1);
  if (effect->side == 0) {
    BattleEffect_LoadBlitter(SLOT_BLITTER, BLITTER(BLIT_TRANSPARENT,
                                        COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  } else {
    BattleEffect_LoadBlitter(SLOT_BLITTER, BLITTER(BLIT_TRANSPARENT,
                                        COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                                            COMPOSITE_FLIP_X));
  }
  blit[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  work->transferMode = 1;
  work->transferValue = 0;
  Scheduler_AddOrUpdateCallback(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  reaperX = 0;
  reaperY = 0;
  speed = 0;
  EffectPosition_ProjectScaledBattler(effect->actors[0], &target);

  for (frame = 0; frame != LIFE_SHEAR_FRAMES; frame++) {
    s32 x;
    s32 phase;
    s32 scale;

    shift = 0;
    scythe = (struct Scythe *)work->particles;
    if (frame == 0) {
      Audio_PlayCue(206);
      if (effect->side == 0) {
        scythe->x = 0x200000;
      } else {
        scythe->x = 0xffe00000;
      }
      scythe->y = 0xffc00000;
      scythe->z = 0xffffe3d0;
      scythe->swing = 0;
      scythe->timer = 0;
      reaperY = 0x180000;
      if (effect->side == 0) {
        reaperX = (target.x + 176) << 16;
        speed = 0xf0000;
      } else {
        reaperX = (target.x - 304) << 16;
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
      blit[0](canvas, Data_020112e8, (effect->side != 0 ? 5 : 64) + shift,
           reaperY >> 16, 59, 89);
    } else {
      s32 column;

      if (effect->side == 0) {
        column = frame * 2 + 16;
        if (column > 64) {
          column = 64;
        }
      } else {
        column = 57 - frame * 2;
        if (column <= 8) {
          column = 9;
        }
      }
      blit[0](canvas, gMapCellBuffer, column + shift, reaperY >> 16, 55, 88);
    }

    // The scythe's path: still, a backswing at 24, the strike at 33.
    for (i = 0; i != 1; i++, scythe++) {
      if (++scythe->timer <= 0) {
        continue;
      }
      if (scythe->timer == 1) {
        scythe->velocityX = 0;
        scythe->velocityY = 0;
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
        Audio_PlayCue(133);
        Func_0814cd48(effect->actors[0], 7, 5, 0, 8);
      }
      if (scythe->timer == 35) {
        scythe->velocityX = 0;
        scythe->velocityY = 0;
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

    vertices = Runtime_BumpAllocateAlternatePool(32);
    model = Render_AllocateModelList(1);
    scythe = (struct Scythe *)work->particles;
    phase = frame - 50;
    if (phase == -4) {
      Audio_PlayCue(212);
    }
    if (phase == 0) {
      Func_081180e8(134);
      work->shakeFrames = 12;
      Func_0814cd48(effect->actors[0], 7, 5, 0, 8);
      Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
      Func_08118088(effect->actors[0], 4);
    }
    if ((u32)(frame - 44) <= 13) {
      scythe->swing++;
    }
    if (scythe->timer >= 0) {
      scale = 0xb1e0;
      if (frame > 63) {
        scale = scale - (frame << 9) + 0x8000;
      }
      if (scale > 0) {
        model->shade = 0;
        model->kind = 9;
        model->vertices = vertices;
        model->texture = &texture;
        Render_ResetTransformState();
        Func_08015160(scythe->x + (shift << 16), scythe->y, 0);
        if (effect->side == 1) {
          SceneTransform_ApplyYaw(0x8000);
        }
        Func_080150e4(scythe->z);
        Func_0801521c(scale);
        texture.logHeight = 5;
        texture.pixels = Data_0201476b;
        texture.logWidth = 7;
        model->mesh = Data_0819962c;
        Transform_Push();
        Func_080150e4((Trig_Sin(scythe->swing << 11) >> 3) - 0x2000);
        Render_TransformVertices(Data_08199650, vertices, 4);
        Render_DrawModelList(model);
        Transform_Pop();
        model->mesh = Data_08199608;
        texture.logWidth = 7;
        texture.logHeight = 6;
        texture.pixels = Data_0201276b;
        Func_080150e4(0x2000 - (Trig_Sin(scythe->swing << 11) >> 3));
        Func_080151e4(0x10000, 0x15f90, 0x10000);
        Render_TransformVertices(Data_08199650, vertices, 4);
        Render_DrawModelList(model);
      }

      // The flash: four blades, shrinking over eight frames.
      if ((u32)phase <= 7) {
        s32 size = 0x78e20 - (frame << 13);
        s32 angle;

        Render_ResetTransformState();
        if (effect->side == 0) {
          Func_08015160(0xfff00000, 0, 0);
        } else {
          Func_08015160(0x100000, 0, 0);
        }
        Func_080151e4(size, size, size);
        if (effect->side == 1) {
          SceneTransform_ApplyYaw(0x8000);
        }
        model->kind = 6;
        model->mesh = Data_08199244;
        texture.logWidth = 5;
        texture.logHeight = 5;
        texture.pixels = Data_0201576b;
        for (i = 0, angle = 0x5650; i != 4; i++, angle += 0x4000) {
          Transform_Push();
          Func_080150e4(angle);
          Func_080151e4(0x4000, ((i & 1) << 15) + 0xc000, 0x8000);
          Func_0801521c(0x10000);
          Render_TransformVertices(gData_081991f0, vertices, 4);
          Render_DrawModelList(model);
          Transform_Pop();
        }
      }
    }
    MemFree(model);
    MemFree(vertices);
    Func_08158ce0(8, 8);
    ObjectGroup_TickMemberTimers();
    work->transferPending = 1;
    WaitTicks(1);
  }
  Scheduler_RemoveCallback(BattleEffect_ProcessPendingGraphicsTransfer);
  Runtime_ReleaseHeapBlock(104);
  BattleEffect_EndCanvasLayer();
}

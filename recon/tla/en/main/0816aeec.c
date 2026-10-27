// Draft of Func_0816aeec, decompiled from the listing: differ 955 of 902 (draft
// 903), the same 55 calls and control flow; allocation differs.

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

extern char ResourceId_CrackedTextureSheet;
extern char ResourceId_Data0B4;
extern char ResourceId_SmokeSheet;
extern char ResourceId_CrystalSheet;
extern u8 gMapCellBuffer[];
extern u8 gData_02014000[];       // the water's mesh, built here
extern const u8 gData_08199340[]; // the splash's mesh
extern const u8 gData_08199210[]; // its corners

#define WATER_COLUMNS 16
#define WATER_ROWS 8

// The body of Dreamtide (mode 0) and Watery Grave (mode 1): a sheet of
// water (a 16 by 8 grid of vertices, its mesh built here) rolls in under
// the targets in waves while the camera turns; Dreamtide strikes the first
// target with two splashes at frame 17, Watery Grave strikes every target
// at frames 17 and 46 while bubbles rise from the deep.
void Func_0816aeec(struct BattleEffectArgument *effect, s32 mode) {
  struct BattleEffectWork *work;
  void *canvas;
  s16 *camera;
  DrawRectangle blit;
  struct EffectPosition screen;
  struct EffectPosition target;
  struct ModelTexture texture;
  struct RenderModel *model;
  struct EffectStep *bubble;
  void *vertices;
  s16 savedYaw;
  s32 frames;
  s32 frame;
  s32 scroll;
  s32 height;
  s32 phase;
  s32 shade;
  s32 row;
  s32 column;
  s32 i;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  Func_0813ba50();
  BattleEffect_StartCanvasLayer(1);
  if (mode == 0) {
    REG_BLDALPHA = 0xc10;
  } else {
    REG_BLDALPHA = 0x1010;
  }
  work->transferMode = 2;
  work->transferValue = 50;
  Scheduler_AddOrUpdateCallback(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  BattleEffect_LoadBlitter(SLOT_BLITTER,
                BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  blit = (DrawRectangle)Ram_HeapSlots->unk64[1];
  camera = (s16 *)Ram_HeapSlots->cameraWork;
  savedYaw = camera[27];
  Resource_LoadAndDecompress((s32)&ResourceId_CrackedTextureSheet, work->sheet,
                             1, 1);
  Func_0816ae40(work->sheet, work->sheet + 0x400, 32, 32);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0B4, work->sheet + 0x2000, 0,
                             0);
  if (mode == 1) {
    u8 *pixel = work->sheet + 0x3000;

    Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet, pixel, 1, 0);
    for (i = 0; i != 0x1b00; i++, pixel++) {
      if (*pixel != 0) {
        *pixel += 8;
        if (*pixel > 63) {
          *pixel = 63;
        }
      }
    }
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_CrystalSheet), 128);
  }

  // The water's grid: 16 by 8 vertices, and a quad (two triangles) for
  // each cell but the last row and column.
  for (row = 0; row != WATER_ROWS; row++) {
    for (column = 0; column != WATER_COLUMNS; column++) {
      s8 *vertex = (s8 *)gMapCellBuffer + (row * 16 + column) * 4;
      u8 *quad = gData_02014000 + (row * 15 + column) * 24;
      s32 v = row * 16;

      vertex[0] = column * 11 - 82;
      vertex[1] = 60;
      vertex[2] = row * 24 - 84;
      if (row <= 6 && column <= 14) {
        quad[4] = column * 8 + 7;
        quad[6] = column * 8 + 7;
        quad[18] = column * 8 + 7;
        quad[7] = v + 15;
        quad[8] = column * 8;
        quad[9] = v + 15;
        quad[16] = column * 8;
        quad[20] = column * 8;
        quad[5] = v;
        quad[17] = v;
        quad[19] = v;
        quad[21] = v + 15;
        quad[0] = v + column + 1;
        quad[1] = v + column + 17;
        quad[2] = v + column + 16;
        quad[12] = v + column;
        quad[13] = v + column + 1;
        quad[14] = v + column + 16;
      }
    }
  }
  gData_02014000[105 * 24] = 0;
  gData_02014000[105 * 24 + 1] = 0;
  gData_02014000[105 * 24 + 2] = 0;
  frames = 48;
  if (mode != 0) {
    frames = 81;
  }

  for (frame = 0, scroll = 0; frame != frames; frame++, scroll += 8) {
    if (frame == 0) {
      camera[27] = savedYaw;
      for (i = 0; i != 64; i++) {
        work->particles[i].variant = -1;
      }
    }
    if (effect->side == 0) {
      if (camera[27] <= 11999) {
        camera[27] += 128;
      }
    } else if (camera[27] >= -11999) {
      camera[27] -= 128;
    }

    if (mode == 0) {
      if (frame == 17) {
        Func_081180e8(144);
        BattleEffect_StartActorMotion(effect->actors[0], 1, 0, 0, 0x80000, 100);
        Func_0814cd48(effect->actors[0], 7, 5, 0, 16);
        work->shakeFrames = 8;
      }
    } else {
      if (frame == 17) {
        Audio_PlayCue(145);
        for (i = 0; i != effect->count; i++) {
          Func_08118088(effect->actors[i], 0);
          Func_0814cd48(effect->actors[i], 7, 5, i, 16);
        }
        work->shakeFrames = 8;
      }
      if (frame == 46) {
        Func_081180e8(134);
        for (i = 0; i != effect->count; i++) {
          BattleEffect_StartActorMotion(effect->actors[i], 1, 0, 0, 0x80000, 100);
          Func_0814cd48(effect->actors[i], 7, 5, i, 16);
        }
        work->shakeFrames = 8;
      }
    }
    if (mode == 0) {
      if ((u32)(frame - 33) <= 14) {
        REG_BLDALPHA = (48 - frame) | 0x1000;
      }
    } else if ((u32)(frame - 65) <= 14) {
      REG_BLDALPHA = (80 - frame) | 0x1000;
    }

    vertices = Runtime_BumpAllocateAlternatePool(0x400);
    model = Render_AllocateModelList(1);
    shade = 0;

    // The waves rise, then ebb.
    if (mode == 0) {
      if (frame <= 31) {
        height = frame * 4;
        if (height > 64) {
          height = 64;
        }
      } else {
        height = 320 - scroll;
        if (height < 0) {
          height = 0;
        }
      }
    } else if (frame <= 63) {
      height = frame * 2;
      if (height > 64) {
        height = 64;
      }
    } else {
      height = 576 - scroll;
      if (height < 0) {
        height = 0;
      }
    }
    phase = frame * 2;
    for (row = 1; row != 7; row++) {
      s8 *vertex = (s8 *)gMapCellBuffer + row * 64 + 1;

      for (column = 0; column != WATER_COLUMNS; column++) {
        s32 amplitude = height * (16 - column) / 32;

        *vertex = ((amplitude * Trig_Sin((frame << 11) + column * 5000 +
                                         Trig_Sin(row * 3553))) >>
                   16) -
                  amplitude + 64;
        vertex += 4;
      }
    }
    height /= 2;
    for (i = 0, bubble = work->particles; i != 20; i++, bubble++) {
      if (bubble->variant == 0) {
        bubble->y =
            (((height * Trig_Sin((frame << 11) + Trig_Sin(3552 + i * 888))) >>
              16) -
             height + 64)
            << 16;
      }
    }

    texture.logWidth = 6;
    texture.logHeight = 6;
    texture.pixels = work->sheet + 0x400;
    Render_ResetTransformState();
    if (mode != 0) {
      SceneTransform_ApplyPitch(4000);
    }
    if (effect->side == 0) {
      SceneTransform_ApplyYaw(camera[27]);
    } else {
      SceneTransform_ApplyYaw(camera[27] + 0x8000);
    }
    {
      s32 shift = 128 - scroll;

      if (shift < -64) {
        shift = -64;
      }
      Func_08015160(shift << 16, 0, 0);
    }
    model->kind = 7;
    model->texture = &texture;
    model->mesh = gData_02014000;
    model->vertices = vertices;
    model->shade = shade;
    model->scrollU = (frame + phase) & 127;
    model->scrollV = 127 & ~frame;
    Render_TransformVertices(gMapCellBuffer, vertices, 128);
    Render_DrawModelList(model);

    if (frame == 0) {
      s32 z = -0x3c0000;

      for (i = 0, bubble = work->particles; i != 20; i++, bubble++) {
        bubble->variant = Random16() % 6;
        bubble->z = z;
        bubble->x = -0x3c0000;
        bubble->velocityX = 0;
        bubble->velocityY = 0;
        bubble->velocityZ = 0;
        z += 0x60000;
      }
    }
    if (mode == 1) {
      for (i = 0, bubble = work->particles; i != 20; i++, bubble++) {
        s32 picture = bubble->variant;

        if (picture < 0) {
          continue;
        }
        if (EffectPosition_ProjectPoint(bubble, &screen) == 0 &&
            HI(screen.depth) > 10) {
          s32 x = HI(screen.x);
          s32 y;

          screen.x = x + 64;
          y = HI(screen.y);
          screen.y = y + 64;
          blit(canvas, work->sheet + 0x3000 + picture * 0x480, x + 52, y + 40,
               24, 48);
          BattleEffectKernels_IntegrateVector3(bubble, 62, -0x2000);
        }
        if (++bubble->variant == 6) {
          bubble->variant = 0;
        }
      }
    } else {
      s32 t;
      s32 scale;

      EffectPosition_ProjectBattler(effect->actors[0], &target);
      texture.logWidth = 6;
      texture.logHeight = 6;
      model->mesh = gData_08199340;
      model->vertices = vertices;
      model->scrollU = 0;
      model->scrollV = 0;
      for (i = 0, t = frame - 16, scale = frame * 3 * 0x1000 - 0x30000; i != 2;
           i++, t -= 4, scale -= 0xc000) {
        if (t < 0) {
          continue;
        }
        texture.pixels = work->sheet + 0x2000;
        if (t > 7) {
          shade = (8 - t) * 4;
        }
        model->shade = shade;
        if (shade <= -64) {
          continue;
        }
        Render_ResetTransformState();
        Func_08015160((target.x / 2 - 64) << 16, (target.y - 64) << 16, 0);
        Func_080151e4(0x8000, 0x10000, 0x8000);
        Func_080150e4((i << 14) + 0x2000);
        SceneTransform_ApplyPitch(0x1000);
        SceneTransform_ApplyYaw(frame << 11);
        Func_0801521c(scale);
        Render_TransformVertices(gData_08199210, vertices, 4);
        Render_DrawModelList(model);
      }
    }
    MemFree(model);
    MemFree(vertices);
    Func_08158ce0(8, 8);
    ObjectGroup_TickMemberTimers();
    work->transferPending = 1;
    WaitTicks(1);
  }
  Runtime_ReleaseHeapBlock(104);
  Scheduler_RemoveCallback(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

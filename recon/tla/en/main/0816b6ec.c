// Draft of Func_0816b6ec, decompiled from the listing: differ 509 of 609 (draft
// 606), the same 48 calls and control flow; allocation differs.

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
void Func_08143bb8(void); // BattleFx_EndCanvasLayer
void Func_08143000(void); // BattleFx_ProcessPendingGraphicsTransfer
void Func_081434f8(void); // ObjectGroup_TickMemberTimers
void Func_081963ec(s32 slot, s32 modeAndFlags); // BattleFx_LoadWork
void Func_0814cd48(s32 actor, s32 palette, s32 pose, s32 timerSlot,
                   s32 timerFrames);              // ObjectGroup_UpdateMembers
void Func_0815b434(u8 *map, s32 size, s32 scale); // shaded circles
void Func_0816ae40(u8 *src, u8 *dst, s32 width, s32 height);
void Func_0815585c(struct BattleEffectArgument *effect, s32 packed, s32 *screen,
                   s32 *otherPoint);
void Func_0818caa8(void *cells, s32 x, s32 y, s32 size);
struct RenderModel *Func_081969f8(s32 count);
void Func_08196958(const void *corners, void *vertices, s32 count);
void Func_08196a7c(struct RenderModel *model);
void Func_08015160(s32 x, s32 y, s32 z);
void Func_0801521c(s32 scale);
void SceneTransform_ApplyPitch(s32 angle);
void SceneTransform_ApplyYaw(s32 angle);
void Render_ResetTransformState(void);
void Func_080151e4(s32 x, s32 y, s32 z); // SceneTransform_ApplyScale3
void EffectPosition_ApplyAlternateStepAndYOffset(s32 id,
                                                 struct EffectPosition *out);
void Func_08152404(s32 count, s32 resource, s32 mode);   // SpawnObjects
void Func_0814cc4c(struct BattleEffectArgument *effect); // SelectLivingTargets
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

void Func_08178680(void *mesh, s32 columns, s32 width, s32 height);
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

void Func_0815f0a0(s32 palette);

extern char ResourceId_Data0D3;
extern char ResourceId_Data134;
extern char ResourceId_JupiterDjinnSmallSheet;
extern char ResourceId_CyanPalette;
extern char ResourceId_TanPalette;
extern u8 gMapCellBuffer[];
extern u8 Data_02010108[];
extern u8 Data_02010318[];
extern u32 gCameraSceneParameters[];
extern const u8 Data_08198b24[]; // the funnels per mode

#define WAFT_SEGMENTS 33

// The body of Waft (mode 0) and Aroma (mode 1): the djinni appears over the
// caster, bobs and rises, while a funnel of wind (33 segments of two
// vertices, each frame's head copied down the tail) twists up from it; Waft
// strikes the first target at frame 28, Aroma touches each target in turn,
// four frames apart.
void Func_0816b6ec(struct BattleEffectArgument *effect, s32 mode) {
  struct BattleEffectWork *work;
  void *canvas;
  u8 *sprites;
  s32 *camera;
  DrawRectangle blit;
  struct EffectPosition screen;
  struct EffectPosition other;
  struct ModelTexture texture;
  struct RenderModel *model;
  s32 frames;
  s32 frame;
  s32 yaw;
  s32 phase;
  s32 i;
  s32 k;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  camera = Ram_HeapSlots->cameraWork;
  sprites = Ram_HeapSlots->unk64[0];
  BattleFx_BeginCanvasLayer(0);
  if (mode == 0) {
    Func_0815585c(effect, effect->side << 4 | 3, (s32 *)&screen, (s32 *)&other);
    REG_BG2CNT = 0x785;
  } else {
    Func_0815585c(effect, effect->side << 4 | 35, (s32 *)&screen,
                  (s32 *)&other);
  }
  if (effect->side == 0) {
    Func_081963ec(SLOT_BLITTER,
                  BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  } else {
    Func_081963ec(SLOT_BLITTER,
                  BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                                        COMPOSITE_FLIP_X));
  }
  blit = (DrawRectangle)Ram_HeapSlots->unk64[1];
  REG_BLDALPHA = 0x1010;
  work->transferMode = 2;
  work->transferValue = 50;
  Scheduler_AddOrUpdateCallback(Func_08143000, 0xc80);
  yaw = 0;
  frames = 36;
  if (mode == 1 && effect->count > 4) {
    frames = effect->count * 6 + 12;
  }
  Resource_LoadAndDecompress((s32)&ResourceId_Data0D3, work->sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_JupiterDjinnSmallSheet,
                             work->sheet + 0x2000, 1, 1);
  Func_08178680(Data_02010318, 4, 32, 32);

  for (frame = 0, phase = 0; frame != frames; frame++, phase += 0x800) {
    s32 shade;
    s32 height;
    s32 angle;
    s8 *corner;
    s32 j;

    if (frame == 0) {
      s32 length;

      yaw = 0;
      length = mode == 0 ? 80 : 30;
      for (i = 0; i != WAFT_SEGMENTS; i++) {
        s32 y = -length;

        corner = (s8 *)gMapCellBuffer + i * 8;
        for (k = 0; k != 2; k++) {
          corner[1] = y;
          corner[0] = 0;
          corner[2] = 32;
          y -= 24;
          corner += 4;
        }
      }
      Audio_PlayCue(179);
    }
    if (frame > 19) {
      if (mode == 0) {
        Func_0815f0a0((s32)&ResourceId_CyanPalette);
      } else if (mode == 1) {
        Func_0815f0a0((s32)&ResourceId_TanPalette);
      }
    }

    // The djinni, bobbing over the caster and rising from frame 8.
    if (frame <= 79) {
      s32 x = ((Trig_Sin(phase) << 3) >> 16) + screen.x / 2;
      s32 y = ((Trig_Cos(phase) << 2) >> 16) + screen.y;

      x -= 10;
      y -= 24;
      if (frame > 7) {
        y = y - frame * 4 + 32;
      }
      blit(canvas, work->sheet + 0x2000, x, y, 20, 40);
    }

    // The funnel's tail trails its head.
    if ((frame & 1) || frame > 47 || frame <= 31) {
      for (i = 32; i != 0; i--) {
        corner = (s8 *)gMapCellBuffer + (i - 1) * 8;
        for (k = 0; k != 2; k++) {
          corner[8] = corner[0];
          corner[9] = corner[1];
          corner[10] = corner[2];
          corner += 4;
        }
      }
    }
    if (frame > 23) {
      yaw = yaw - frame * 16 + 2432;
    } else {
      yaw += 2048;
    }
    if (mode == 0) {
      angle = frame << 12;
      height = 80 - ((Trig_Sin(frame * 820) * 80) >> 16);
    } else {
      angle = yaw;
      height = 30 - ((Trig_Sin(frame * 820) * 20) >> 16);
    }
    corner = (s8 *)gMapCellBuffer;
    height = -height;
    for (k = 0; k != 2; k++) {
      corner[0] = (u32)Trig_Sin(angle) >> 11;
      corner[1] = height;
      corner[2] = (u32)Trig_Cos(angle) >> 11;
      height -= 24;
      corner += 4;
    }

    if (mode == 0) {
      if (frame == 28) {
        work->shakeFrames = 6;
        Func_081180e8(133);
        Func_0814cd48(effect->actors[0], 7, 5, 0, 8);
      }
    } else {
      if (frame == 12) {
        Func_081180e8(-1);
      }
      for (i = 0; i != effect->count; i++) {
        if (frame == i * 4 + 12) {
          Audio_PlayCue(126);
          Func_0814cd48(effect->actors[i], 7, -1, i, 8);
        }
      }
    }

    Render_ResetTransformState();
    Transform_SetLookAt(camera, camera + 3);
    model = Func_081969f8(1);
    shade = 0;
    if (frame <= 15) {
      shade = frame * 4 - 64;
      if (shade > 0) {
        shade = 0;
      }
    } else if (frame > 27) {
      shade = (28 - frame) * 8;
      if (shade < -64) {
        shade = -64;
      }
    }
    texture.logWidth = 7;
    texture.logHeight = 5;
    texture.pixels = work->sheet;
    model->kind = 6;
    model->texture = &texture;
    model->mesh = Data_02010318;
    model->scrollU = 0;
    model->scrollV = 0;
    gCameraSceneParameters[4] = 104;
    model->vertices = Data_02010108;
    model->shade = shade;
    for (j = 0; j != Data_08198b24[mode]; j++) {
      s32 scale;

      Render_ResetTransformState();
      if (mode == 0) {
        Func_08015160((screen.x / 2 - 64) << 16, 0x280000, 0);
        SceneTransform_ApplyPitch(j << 11);
        SceneTransform_ApplyYaw(j * 21000 + yaw);
        scale = 85536 - Trig_Sin(frame * 964);
        if (frame > 27) {
          scale = scale + phase - 0xe000;
        }
        Func_080151e4(scale * 3 / 2, 0x10000, scale * 3 / 2);
      } else {
        Func_08015160((screen.x / 2 - 64) << 16, (j << 20) + 0x100000, 0);
        SceneTransform_ApplyPitch((j << 12) - 0x800);
        SceneTransform_ApplyYaw(j * 21000 + yaw);
        scale = 0x10000 - Trig_Sin(frame * 682) / 4;
        if (frame > 27) {
          scale = scale + phase - 0xe000;
        }
        Func_080151e4(scale * 2, 0x10000, scale * 2);
      }
      Func_08196958(gMapCellBuffer, Data_02010108, 66);
      Func_08196a7c(model);
    }
    gCameraSceneParameters[3] = 120;
    gCameraSceneParameters[4] = 120;
    MemFree(model);
    Func_08158ce0(4, 4);
    Func_081434f8();
    work->transferPending = 1;
    WaitFrames(1);
  }
  Runtime_ReleaseHeapBlock(104);
  Scheduler_RemoveCallback(Func_08143000);
  Func_08143bb8();
}

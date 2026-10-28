// Draft of BattleEffect_RunWyrdCurse, decompiled from the listing: differ 395
// of 568 (draft 563), the same 47 calls and control flow. The glow's mirror
// loop runs to size / 2 with its right-hand columns written from size, so
// it keeps the native's six row pointers stepping by two; the last
// particle is read in place.
// Left: allocation, and a stack frame of 60 bytes for 68.

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
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_ProcessPendingGraphicsTransfer(void);
void BattleEffect_CountDownBattlerTints(void);
void BattleEffect_LoadBlitter(s32 slot, s32 modeAndFlags); // BattleEffect_BuildBlitter
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
void EffectPosition_ProjectBattler(s32 id,
                                                 struct EffectPosition *out);
void Actor_SetAnimationSpeedFar(void *object, s32 value);
void Func_08118088(s32 actor, s32 motion); // BattleMotion_ApplyVariantMotionFar
void Func_081180e8(s32 phase);             // BattlePlayback_StartFar
void Func_08158ce0(s32 randomMask, u32 shakeRange); // Camera_ApplyShake
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 gravity,
                                      s32 drag);
void **GetBattleObjectSlotFar(s32 objectId);
void *Heap_AllocEwram(s32 size);
void Resource_LoadAndDecompress(s32 resource, void *destination, s32 a, s32 b);
void *Resource_GetTableEntry(s32 resource);
void Task_Add(void (*func)(void), s32 order);
void Task_Remove(void (*func)(void));
void HeapSlot_Release(s32 slot);
void Sound_PlayFar(s32 cue);
void wait(s32 frames);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);

extern char ResourceId_Data134;
extern char ResourceId_Data0D8;
extern u8 gMapCellBuffer[];
extern u8 gData_02014000[];
extern const u16 gData_08197410[]; // gBattleEffect_MoteCells
extern const s8 Data_08198bea[];   // the drops' bounce speeds
extern const u8 Data_08199244[];   // the glow's and the splashes' mesh
extern const u8 gData_08199210[];  // its four corners
extern u32 gCameraSceneParameters[];

#define BLITTER ((DrawRectangle)Ram_HeapSlots->unk64[1])
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT };
#define BLIT_MODE(mode, flags) ((mode) << 4 | (flags))
#define BLIT_CLIP (COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y)
#define HI(v) (((s16 *)&(v))[1])
#define WYRD_FRAMES 120
#define WYRD_DROPS 16

// Abilities: Wyrd Curse.
//
// A 32x32 glow is mirrored from a shaded disc, and BG palette 0 becomes a
// dark ramp. Sixteen drops form over the target; from frame 0 one falls
// every eight frames, and on reaching the ground it bounces away and adds
// its weight to the glow (the last particle), which grows over the target
// and fades from frame 64. From frame 64 six splashes, four frames apart,
// rise from the drops, and from frame 80 the target is struck six times.
void BattleEffect_RunWyrdCurse(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  u8 *sprites;
  DrawRectangle draw;
  void *caster;
  struct ModelTexture texture;
  struct EffectPosition screen;
  s32 frame;
  s32 i;
  struct EffectStep *drop;

  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  sprites = Ram_HeapSlots->unk64[0];
  BattleEffect_StartCanvasLayer(1);
  REG_BLDALPHA = 0x1010;
  REG_BLDCNT = 0;
  if (effect->side == 0) {
    BattleEffect_LoadBlitter(104, BLIT_MODE(BLIT_MAX, BLIT_CLIP));
  } else {
    BattleEffect_LoadBlitter(104, BLIT_MODE(BLIT_MAX, BLIT_CLIP | COMPOSITE_FLIP_X));
  }
  draw = BLITTER;
  work->transferMode = 2;
  work->transferValue = 50;
  work->transferMode = 1;
  work->transferValue = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  caster = *GetBattleObjectSlotFar(effect->actor);
  Iwram_ClearWords((void *)BG_PLTT, 128);
  for (i = 0; i != 63; i++) {
    ((u16 *)BG_PLTT)[i + 1] = i / 3 + 8;
  }

  // The glow: a 16-pixel disc mirrored into the four quarters of 32x32.
  Iwram_ClearWords(work->sheet + 0x1000, 0x800);
  Func_0815b434(work->sheet, 16, 0x10000);
  {
    s32 size = 16;
    u8 *glow = work->sheet + 0x1000;
    s32 y;
    s32 x;

    for (y = 0; y != size; y++) {
      for (x = 0; x != size / 2; x++) {
        u8 v = work->sheet[y * size / 2 + x];

        if (v != 0) {
          glow[y * 32 + x * 2] = v;
          glow[y * 32 + x * 2 + 1] = v;
          glow[(y + 1) * size * 2 - 2 - x * 2] = v;
          glow[(y + 1) * size * 2 - 1 - x * 2] = v;
          glow[(size * 2 - 1 - y) * 32 + x * 2] = v;
          glow[(size * 2 - 1 - y) * 32 + x * 2 + 1] = v;
          glow[(size * 2 - 1 - y) * 32 + size * 2 - 2 - x * 2] = v;
          glow[(size * 2 - 1 - y) * 32 + size * 2 - 1 - x * 2] = v;
        }
      }
    }
  }
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0D8, work->sheet + 0x4000, 0,
                             0);

  for (frame = 0; frame != WYRD_FRAMES; frame++) {
    struct RenderModel *model;
    void *vertices;
    s32 level;

    if (frame == 0) {
      EffectPosition_ProjectBattler(effect->actors[0], &screen);
      for (i = 0, drop = work->particles; i != WYRD_DROPS; i++, drop++) {
        drop->x = (screen.x / 2 + (Random16() & 15) - 8) << 16;
        drop->y = -8;
        drop->velocityX = 0;
        drop->velocityY = 0;
        drop->z = (Random16() & 3) + 4;
        drop->velocityZ = 160 - i * 16;
        drop->variant = 0;
      }
      work->particles[63].x = (screen.x / 2) << 16;
      work->particles[63].y = 0x700000;
      work->particles[63].variant = 0;
    }

    // The falling drops, one more every eight frames.
    if (frame <= 63) {
      for (i = 0, drop = work->particles; i != 6; i++, drop++) {
        if (frame > i * 8 && drop->variant == 0) {
          s32 size = drop->z;

          draw(canvas, sprites + gData_08197410[size - 1],
               HI(drop->x) - size / 2, HI(drop->y) - size, size, size * 2);
          BattleEffectKernels_IntegrateVector2(drop, 64, 0x8000);
          if (HI(drop->y) > 111) {
            drop->variant = 1;
            drop->velocityX = ((Random16() & 31) - 16) << 12;
            drop->velocityX = Data_08198bea[i % 4] << 13;
            drop->velocityY = 0;
          }
        }
      }
    }

    vertices = Heap_AllocEwram(32);
    model = Render_AllocateModelList(1);
    texture.logWidth = 5;
    texture.logHeight = 5;
    texture.pixels = work->sheet + 0x1000;
    model->kind = 6;
    model->mesh = Data_08199244;
    model->texture = &texture;
    model->vertices = vertices;
    model->shade = 0;

    // The glow gathers what the landed drops bring.
    for (i = 0, drop = work->particles; i != 8; i++, drop++) {
      if (drop->variant > 0) {
        work->particles[63].variant += drop->velocityZ;
        drop->velocityZ /= 2;
      }
    }
    level = work->particles[63].variant;
    if (level > 0) {
      s32 scale = (level << 13) / 824;

      if (frame > 63) {
        work->particles[63].variant = level - 24;
      }
      Render_ResetTransformState();
      Func_08015160(work->particles[63].x - 0x400000,
                    work->particles[63].y - 0x400000, 0);
      Func_080151e4(scale * 2, scale, scale * 2);
      SceneTransform_ApplyPitch(-0x2000);
      Render_TransformVertices(gData_08199210, vertices, 4);
      Render_DrawModelList(model);
    }

    // The splashes, from frame 64.
    texture.logWidth = 5;
    texture.logHeight = 5;
    model->kind = 7;
    model->mesh = Data_08199244;
    for (i = 0, drop = work->particles; i != 6; i++, drop++) {
      if (frame > i * 4 + 64) {
        model->shade = 0;
        texture.pixels = work->sheet + 0x4000 + ((i % 2) << 10);
        Render_ResetTransformState();
        Func_08015160(drop->x - 0x400000, drop->y - 0x300000, 0);
        Func_080151e4(25000 / 2, 25000, 25000 / 2);
        SceneTransform_ApplyPitch(-0x4000);
        Render_TransformVertices(gData_08199210, vertices, 4);
        Render_DrawModelList(model);
        BattleEffectKernels_IntegrateVector2(drop, 60, -0x8000);
      }
    }
    Heap_Release(model);
    Heap_Release(vertices);

    if (frame == 64) {
      Sound_PlayFar(147);
    }
    for (i = 0; i != 6; i++) {
      if (frame == i * 4 + 80) {
        work->shakeFrames = 2;
        Func_08118088(effect->actors[0], 0);
        Func_0814cd48(effect->actors[0], 7, 5, 0, 2);
        if (i == 0) {
          Func_081180e8(134);
        } else {
          Sound_PlayFar(133);
        }
      }
    }
    Func_08158ce0(4, 4);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }
  Actor_SetAnimationSpeedFar(caster, 16);
  HeapSlot_Release(104);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

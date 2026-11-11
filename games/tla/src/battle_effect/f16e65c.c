#include "battle_effect.h"
#include "battle_effect_work.h"
#include "effect_step.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "heap_release.h"
#include "ram_buffer.h"
#include "types.h"

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
s32 BattleEffect_LoadBlitter(s32 slot,
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
void BattleCamera_Shake(s32 rangeX, u32 rangeY);
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

void Actor_SetAnimationSpeedFar(void *object, s32 value);

extern char ResourceId_GreatSwordSheet;
extern char ResourceId_AxeSheet;
extern u8 gMapCellBuffer[];
extern const u8 Data_0819928c[];  // the sword's mesh
extern const u8 Data_081991c0[];  // its corners
extern const u8 gData_08199340[]; // the axe's mesh
extern const u8 Data_081991e0[];  // its corners
extern u8 Data_08198b96[];        // per mode: the strike's frame
extern u8 Data_08198b98[];        // per mode: the chips
extern const u8 Data_08198b9a[];  // a chip's two pixels

// The body of Heavy Divide (mode 0) and Strong Hit (mode 1). Heavy Divide:
// a great sword (with four fading after-images, each a dimmer copy of the
// sheet) swings down from over the caster, striking at frame
// Data_08198b96[0]. Strong Hit: an axe is thrown, spinning, from the caster
// to the target. Either way chips of stone fly from the target and bounce
// on the ground.
void Func_0816e65c(struct BattleEffectArgument *effect, s32 mode) {
  struct BattleEffectWork *work;
  void *canvas;
  // Unused: blit[1] gives the stack frame its 4 bytes.
  DrawRectangle blit[2];
  s32 frame;
  void *caster;
  s32 frames;
  struct EffectPosition hit;
  struct EffectPosition source;
  struct EffectPosition target;
  struct ModelTexture texture;
  struct RenderModel *model;
  void *vertices;
  s32 i;
  s32 k;
  s32 zero; // One constant, rebuilt at each store.

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  BattleEffect_StartCanvasLayer(1);
  REG_BLDALPHA = 0x410;
  BattleEffect_LoadBlitter(
      SLOT_BLITTER,
      BLITTER(BLIT_TRANSPARENT, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  work->transferMode = 1;
  work->transferValue = 0;
  blit[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  caster = *GetBattleObjectSlotFar(effect->actor);
  if (mode == 0) {
    frames = 80;
    Resource_LoadAndDecompress((s32)&ResourceId_GreatSwordSheet, work->sheet, 1,
                               1);

    // Four after-images, each in its own dimmer colour.
    for (k = 1; k != 5; k++) {
      ((u16 *)BG_PLTT)[59 + k] = (4 - k) * 0x1000 + 0x3c00;
      for (i = 0; i != 0x1000; i++) {
        if (work->sheet[i] == 0) {
          work->sheet[k * 0x1000 + i] = 0;
        } else {
          work->sheet[k * 0x1000 + i] = k + 59;
        }
      }
    }
  } else {
    Resource_LoadAndDecompress((s32)&ResourceId_AxeSheet, work->sheet, 1, 1);
    frames = 64;
  }
  Sound_PlayFar(212);
  zero = 0;

  for (frame = 0; frame != frames; frame++) {
    if (frame == 0) {
      s32 actor = effect->actors[0];
      struct EffectPosition *at = &hit;
      struct EffectStep *chip;

      // The store in a once-only block, so the buffer's address loads before
      // at is passed; as the plain store, 2 of 671 instructions differ in
      // tla-en.
      do {
        chip = (struct EffectStep *)gMapCellBuffer;
      } while (0);
      EffectPosition_ProjectBattler(actor, at);
      for (i = 0; i != 256; i++, chip++) {
        s32 angle = Random16();
        s32 speed = 255;

        speed &= Random16();
        chip->x = (hit.x / 2) << 16;
        chip->y = (hit.y - 16) << 16;
        chip->velocityX = (Trig_Sin(angle) * (speed + 128)) >> 6;
        chip->velocityY = (Trig_Cos(angle) * (speed + 128)) >> 6;
        chip->variant = (Random16() & 15) + 16;
      }
    }
    if (mode == 0) {
      if (frame == 8) {
        Func_0814cd48(effect->actor, -1, 2, 0, -1);
      }
    } else if (frame == 0) {
      Func_0814cd48(effect->actor, -1, 2, 0, -1);
      Actor_SetAnimationSpeedFar(caster, 32);
    }

    vertices = Heap_AllocEwram(32);
    model = Render_AllocateModelList(1);
    EffectPosition_ProjectBattler(effect->actor, &source);
    EffectPosition_ProjectBattler(effect->actors[0], &target);
    model->kind = 9;
    model->texture = &texture;
    model->vertices = vertices;
    model->shade = zero;
    if (mode == 0) {
      s32 size;
      s32 angle;

      texture.logWidth = 5;
      texture.logHeight = 7;
      model->mesh = Data_0819928c;
      if (frame > 59) {
        size = 68 - frame;
      } else {
        size = frame + 2;
      }
      if (size > 8) {
        size = 8;
      }
      if (size > 0) {
        if (frame > 38) {
          angle = 0x4000 - Trig_Sin((frame << 9) - 0x4e00) / 16;
        } else if (frame > 15) {
          angle = (0x10000 - Trig_Cos((frame << 9) - 0x2000)) / 2;
        } else {
          angle = 0;
        }

        // The after-images keep the sword's last four angles.
        for (i = 4; i != 0; i--) {
          work->particles[i].variant = work->particles[i - 1].variant;
        }
        work->particles[0].variant = angle;
        for (i = 4; i != -1; i--) {
          struct EffectStep *step = &work->particles[i];

          if (i != 0 && frame > i + 38) {
            continue;
          }
          if (frame < i * 2) {
            continue;
          }
          texture.pixels = work->sheet + i * 0x1000;
          Render_ResetTransformState();
          Func_08015160((source.x / 2 - 64) << 16, (source.y - 88) << 16, 0);
          Func_080151e4(0x8000, 0x10000, 0x8000);
          if (effect->side == 0) {
            Func_080150e4(0x8000 - step->variant);
          } else {
            Func_080150e4(step->variant + 0x8000);
          }
          Func_08015160(0, 0xfff00000, 0);
          Func_080151e4(size << 15, size * 13192, size << 13);
          if (effect->side == 0) {
            SceneTransform_ApplyYaw(0x8000);
          }
          Render_TransformVertices(Data_081991c0, vertices, 4);
          Render_DrawModelList(model);
        }
      }
    } else {
      model->mesh = gData_08199340;
      texture.logWidth = 6;
      texture.logHeight = 6;
      if (frame == 0) {
        for (i = 0; i != 5; i++) {
          work->particles[i].x = (source.x / 2) << 16;
          work->particles[i].y = (source.y - 32) << 16;
          work->particles[i].velocityX = (target.x / 2 - source.x / 2) << 11;
          work->particles[i].velocityY = (target.y - source.y) << 11;
          work->particles[i].z = zero;
          if (effect->side == 0) {
            work->particles[i].velocityZ = -0x1000;
          } else {
            work->particles[i].velocityZ = 0x1000;
          }
        }
      }
      if (frame > 7) {
        s32 scale = 80000;

        texture.pixels = work->sheet;
        Render_ResetTransformState();
        Func_08015160(work->particles[0].x - 0x400000,
                      work->particles[0].y - 0x400000, 0);
        Func_080151e4(scale >> 1, scale, 0x8000);
        Func_080150e4(work->particles[0].z);
        if (effect->side == 0) {
          SceneTransform_ApplyYaw(0x8000);
        }
        Render_TransformVertices(Data_081991e0, vertices, 4);
        Render_DrawModelList(model);
        BattleEffectKernels_IntegrateVector3(work->particles, 64, 0);
      }
    }
    Heap_Release(model);
    Heap_Release(vertices);

    if (frame == Data_08198b96[mode]) {
      Func_081180e8(144);
      Func_08118088(effect->actors[0], 4);
      Func_0814cd48(effect->actors[0], 7, 5, 0, 8);
      work->shakeFrames = 8;
    }
    if (frame >= Data_08198b96[mode]) {
      i = 0;
      if (i != Data_08198b98[mode]) {
        DrawRectangle blitter = blit[0];
        struct EffectStep *chip = (struct EffectStep *)gMapCellBuffer;

        do {
          if (chip->variant >= 0) {
            blitter(canvas, Data_08198b9a, HI(chip->x), HI(chip->y) - 1, 1, 2);
            BattleEffectKernels_IntegrateVector2(chip, 62, 0x1000);
            blitter(canvas, Data_08198b9a, HI(chip->x), HI(chip->y) - 1, 1, 2);
            BattleEffectKernels_IntegrateVector2(chip, 62, 0x1000);
            blitter(canvas, Data_08198b9a, HI(chip->x), HI(chip->y) - 1, 1, 2);
            BattleEffectKernels_IntegrateVector2(chip, 62, 0x1000);
            if (HI(chip->y) > 104) {
              chip->velocityY = -chip->velocityY;
              chip->variant -= 8;
            }
            chip->variant--;
          }
          i++;
          chip++;
        } while (i != Data_08198b98[mode]);
      }
    }
    if (mode == 0) {
      BattleCamera_Shake(4, 16);
    } else {
      BattleCamera_Shake(8, 8);
    }
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }
  Actor_SetAnimationSpeedFar(caster, 16);
  HeapSlot_Release(104);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

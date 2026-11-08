#include "types.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "system.h"
#include "fixed_math.h"
#include "task.h"
#include "resource.h"
#include "heap_release.h"
#include "motobj.h"
#include "effect_step.h"
#include "battle_display.h"
#include "battle_effect.h"
#include "battle_effect_work.h"

// The Lost Age numbers the blitter modes from the other end of The Broken
// Seal's COMPOSITE_* list; the packed loader argument is mode << 4 | flags.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT, BLIT_COPY };
#define BLITTER(mode, flags) ((mode) << 4 | (flags))

#define FLAMES 32

// A texture for the ARM polygon kernels: 1 << logWidth by 1 << logHeight
// pixels.
struct Texture {
  u32 logWidth : 8;
  u32 logHeight : 8;
  u32 unk02 : 16;
  void *pixels;
};

// One model for Render_DrawModelList, the polygon kernel runner.
struct RenderModel {
  s32 kind;
  s32 unk04;
  const void *mesh;
  void *vertices;
  struct Texture *texture;
  s32 brightness;
};

extern char ResourceId_Data0C2;
extern char ResourceId_OrangePaletteB;

extern const u16 gData_08199364[]; // a textured square
extern const u16 Data_081991e0[];  // its four corners

// The canvas layer and its helpers, with The Broken Seal's names.
void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_ProcessPendingGraphicsTransfer(void);
s32 BattleEffect_LoadBlitter(s32 slot,
                             s32 blitter); // BattleEffect_BuildBlitter, packed
void Func_0814cd48(s32 actor, s32 palette, s32 group, s32 timerSlot,
                   s32 frames); // BattleEffect_TintBattler
void BattleEffect_CountDownBattlerTints(void);
void Func_08158ce0(s32 mask, u32 range);   // Camera_ApplyShake
void Func_08118088(s32 actor, s32 motion); // BattleMotion_ApplyVariantFar
void Func_081180e8(s32 phase);             // BattlePlayback_StartFar
void BattleEffect_SetApproachMotion(s32 actor, s32 target, s32 frames,
                                    s32 percent);
void Func_0815b434(void *map, s32 size, s32 scale); // a shaded disc
void Func_0818caa8(void *picture, s32 x, s32 y, s32 size);
void Render_ResetTransformState(void);
void Transform_SetLookAt(s32 *eye, s32 *position);
void Func_08015160(s32 x, s32 y, s32 z);
void Func_080150e4(s32 angle);
void SceneTransform_ApplyPitch(s32 angle);
void Func_0801521c(s32 angle);
void *Heap_AllocEwram(s32 size);
struct RenderModel *Render_AllocateModelList(s32 count);
void Render_TransformVertices(const void *corners, void *vertices, s32 count);
void Render_DrawModelList(struct RenderModel *model);
void Heap_Release(void *block);
void Sound_PlayFar(s32 cue);

// Radiant Fire: eight bursts of fire bloom one after another around the
// target, each a shaded disc that swells and shrinks under a spinning
// square of flame, with a white flash of the canvas as the first lands.
//
// Setup: canvas layer 0 at half blend; one max blitter; the flame texture
// (resource 0x0c2) and the orange palette; 32 shaded discs of 2 to 64
// pixels drawn into the map cell buffer, their offsets kept in the work
// sheet.
// Frame loop (66 frames): at frame 0 the bursts are placed at random
// around the target and the user closes in; from frame 8 burst i lands at
// frame 16 + 4i and burns for 24 frames.
void BattleEffect_RunRadiantFire(struct BattleEffectArgument *effect) {
  struct EffectPosition screen;
  struct Texture texture;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  struct BattleCamera *camera;
  s32 savedYaw;
  struct MotionObject *target;
  s16 *discs;
  struct EffectStep *flame;
  struct EffectStep *burst;
  struct EffectStep *square;
  struct RenderModel *model;
  void *vertices;
  s32 offset;
  s32 size;
  s32 cells;
  s32 i;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = 0x1010;
  BattleEffect_LoadBlitter(
      104, BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C2, work->sheet + 0x1000, 0,
                             0);
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB), 128);
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  camera = Ram_HeapSlots->cameraWork;
  offset = 0;
  savedYaw = (s16)camera->yaw;
  size = 2;
  target = GetBattleObjectSlotFar(effect->actors[0])->object;

  // The discs, smallest first, one after another in the map cell buffer.
  cells = (s32)Ram_MapCellBuffer;
  discs = (s16 *)work->sheet;
  for (i = 0; i != FLAMES; i++) {
    Func_0815b434((void *)(offset + cells), size, 0x14e20);
    discs[i] = offset;
    offset += (i * 2 + 3) * size / 2;
    size += 2;
  }

  for (frame = 0; frame != 66; frame++) {
    // Frame 0: each burst takes a random place on a ring about the target
    // that widens with its number, 48 pixels up, and is projected to the
    // screen; it waits 4 frames more than the one before it.
    if (frame == 0) {
      camera->yaw = savedYaw;
      Render_ResetTransformState();
      Transform_SetLookAt(camera->eye, camera->pos);
      flame = work->particles;
      for (i = 0; i != FLAMES; i++, flame++) {
        s32 angle;
        s32 radius;

        Random16();
        angle = Random16() & 0xffff;
        radius = (Random16() & 7) + i * 4;
        flame->x = target->x + Trig_Sin(angle) * radius * 2;
        flame->y = target->y + Trig_Cos(angle) * radius + (48 << 16);
        flame->z = target->z;
        EffectPosition_ProjectPoint(&flame->x, &screen);
        flame->x = screen.x / 2;
        flame->y = screen.y;
        Random16();
        flame->velocityX = 32 - i;
        flame->velocityY = Random16();
        flame->velocityZ = Random16();
        flame->variant = -i * 4;
      }
    }
    if (frame == 0)
      BattleEffect_SetApproachMotion(effect->actor, effect->actors[0], 8, 80);

    // Frame 8: the canvas flashes white and the target is struck.
    if (frame == 8) {
      Iwram_FillWords(canvas, 0x3c00, 0x3f3f3f3f);
      work->shakeFrames = 16;
      Func_08118088(effect->actors[0], 0);
      Func_0814cd48(effect->actors[0], 7, 5, 0, 8);
      Func_081180e8(145);
    }

    if (frame >= 8) {
      // Each burst strikes the target as it lands, then its disc swells
      // and shrinks with a sine over 32 frames.
      i = 0;
      burst = work->particles;
      for (; i != 8; i++, burst++) {
        burst->variant++;
        if (burst->variant == 8) {
          Sound_PlayFar(144);
          work->shakeFrames = 8;
          Func_08118088(effect->actors[0], 0);
          Func_0814cd48(effect->actors[0], 7, 5, 0, 8);
        }
        if (burst->variant >= 0 && burst->variant < 32) {
          s32 diameter;
          s32 index;

          diameter =
              ((Trig_Sin(burst->variant << 10) * burst->velocityX) >> 16) * 2;
          index = diameter - 2;
          if ((u32)index <= 61)
            Func_0818caa8((void *)(*(s16 *)(index + (s32)discs) + cells),
                          burst->x, burst->y, diameter);
        }
      }

      // The square of flame turns over each burst for 24 frames, fading
      // over the last eight.
      vertices = Heap_AllocEwram(32);
      model = Render_AllocateModelList(1);
      texture.logWidth = 7;
      texture.logHeight = 7;
      texture.pixels = work->sheet + 0x1000;
      model->kind = 6;
      model->vertices = vertices;
      model->mesh = gData_08199364;
      model->texture = &texture;
      i = 0;
      square = work->particles;
      for (; i != 8; i++, square++) {
        if (square->variant >= 0 && square->variant < 24) {
          s32 brightness = 0;
          s32 roll = square->variant * 3 << 11;

          if (square->variant > 15)
            brightness = (16 - square->variant) * 8;
          model->brightness = brightness;
          Render_ResetTransformState();
          Func_08015160((square->x - 64) << 16, (square->y - 64) << 16, 0);
          Func_080150e4(square->velocityZ);
          SceneTransform_ApplyPitch(square->velocityY);
          Func_0801521c(roll);
          Render_TransformVertices(Data_081991e0, vertices, 4);
          Render_DrawModelList(model);
        }
      }
      Heap_Release(model);
      Heap_Release(vertices);
    }

    Func_08158ce0(4, 4);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(104);
  BattleEffect_EndCanvasLayer();
}

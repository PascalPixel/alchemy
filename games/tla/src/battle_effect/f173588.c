#include "types.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "system.h"
#include "fixed_math.h"
#include "task.h"
#include "resource.h"
#include "heap_release.h"
#include "effect_step.h"
#include "battle_effect.h"
#include "battle_effect_work.h"

// The Lost Age numbers the blitter modes from the other end of The Broken
// Seal's COMPOSITE_* list; the packed loader argument is mode << 4 | flags.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT, BLIT_COPY };
#define BLITTER(mode, flags) ((mode) << 4 | (flags))

// The Djinni's picture, 40 pixels wide, at the end of the work sheet.
#define DJINNI_OFFSET 0x65c0
// Rime's crystal: a 48x48 picture mirrored from its left half into a
// 64-pixel-wide square after the rune picture.
#define CRYSTAL_OFFSET 0x480
#define CRYSTAL_ORIGIN (CRYSTAL_OFFSET + 8 * 64 + 8)

// A texture for the ARM polygon kernels: 1 << logWidth by 1 << logHeight
// pixels.
struct Texture {
  u32 logWidth : 8;
  u32 logHeight : 8;
  u32 unk02 : 16;
  u8 *pixels;
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

extern char ResourceId_MercuryDjinnSheet;
extern char ResourceId_RuneSheet;
extern char ResourceId_JupiterDjinnSheet;
extern char ResourceId_StarCrossSheet;

extern s32 gCameraSceneParameters[];
extern const u16 gData_08199340[]; // a textured square
extern const u16 gData_08199210[]; // its four corners

// The canvas layer and its helpers, with The Broken Seal's names.
void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_ProcessPendingGraphicsTransfer(void);
void BattleEffect_LoadBlitter(s32 slot,
                              s32 blitter); // BattleEffect_BuildBlitter, packed
void Func_0814cd48(s32 actor, s32 palette, s32 group, s32 timerSlot,
                   s32 frames); // BattleEffect_TintBattler
void BattleEffect_CountDownBattlerTints(void);
void Func_081180e8(s32 phase);    // BattlePlayback_StartFar
void Func_0815f0a0(s32 resource); // BattleEffect_StepPaletteToResource
void Func_0815585c(struct BattleEffectArgument *effect, s32 placement,
                   s32 *center, s32 *base); // BattleEffect_RunDjinniEntrance
void Render_ResetTransformState(void);
void Func_08015160(s32 x, s32 y, s32 z);
void SceneTransform_ApplyPitch(s32 angle);
void SceneTransform_ApplyYaw(s32 angle);
void Func_080151e4(s32 x, s32 y, s32 z);
void *Heap_AllocEwram(s32 size);
struct RenderModel *Render_AllocateModelList(s32 count);
void Render_TransformVertices(const void *corners, void *vertices, s32 count);
void Render_DrawModelList(struct RenderModel *model);
void Heap_Release(void *block);
void Sound_PlayFar(s32 cue);

// Rime and Haze: the Djinni rises swaying through the canvas over the
// party while its sign turns in the air: for Rime a crystal that spins in
// two mirrored copies and shrinks, for Haze sixteen stars that bloom and
// fade one after another.
//
// Setup: canvas layer 0 placed on the far side of the targets by
// Func_0815585c; the Djinni's picture with its palette; Rime's rune
// picture mirrored into a 48x48 crystal, Haze's star sheet; one additive
// blitter, flipped on the player's side.
// Frame loop (48 frames).
void Func_08173588(struct BattleEffectArgument *effect, s32 mode) {
  s32 center[3];
  s32 base[3];
  struct BattleEffectWork *work;
  void *canvas;
  // Unused: blit[1] gives the stack frame its 4 bytes at sp 24.
  DrawRectangle blit[2];
  void *vertices;
  struct Texture texture;
  struct RenderModel *model;
  s32 frame;
  s32 i;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  BattleEffect_StartCanvasLayer(0);
  if (mode == 0) {
    s32 row;
    s32 column;

    Func_0815585c(effect, (effect->side ^ 1) << 4 | 1, center, base);
    Resource_LoadAndDecompress((s32)&ResourceId_MercuryDjinnSheet,
                               work->sheet + DJINNI_OFFSET, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_RuneSheet, work->sheet, 1, 0);

    // Mirror the rune's 24x48 left half into a 48x48 crystal.
    Iwram_ClearWords(work->sheet + CRYSTAL_OFFSET, 0x4000);
    for (row = 0; row != 48; row++) {
      for (column = 0; column != 24; column++) {
        work->sheet[CRYSTAL_ORIGIN + row * 64 + column] =
            work->sheet[row * 24 + column];
        work->sheet[CRYSTAL_ORIGIN + row * 64 + 47 - column] =
            work->sheet[row * 24 + column];
      }
    }
    work->transferMode = 2;
    work->transferValue = 50;
  } else {
    s32 haze[3];

    Func_0815585c(effect, (effect->side ^ 1) << 4 | 3, center, haze);
    Resource_LoadAndDecompress((s32)&ResourceId_JupiterDjinnSheet,
                               work->sheet + DJINNI_OFFSET, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_StarCrossSheet, work->sheet, 1,
                               0);
    work->transferMode = 2;
    work->transferValue = 75;
  }
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  REG_BG2PA = 0x100;
  REG_BG2X = (64 - center[0]) << 8;
  if (effect->side == 0)
    BattleEffect_LoadBlitter(104, BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_X |
                                                             COMPOSITE_CLIP_Y |
                                                             COMPOSITE_FLIP_X));
  else
    BattleEffect_LoadBlitter(
        104, BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  blit[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];

  for (frame = 0; frame != 48; frame++) {
    // Frame 0: Haze's stars take random places about the centre, one more
    // every two frames.
    if (frame == 0) {
      for (i = 0; i != 32; i++) {
        struct EffectStep *star = &work->particles[i];

        star->x = ((Random16() & 31) - 16) << 16;
        star->y = ((Random16() & 63) - 16) << 16;
        star->z = (Random16() & 127) + 128;
        star->velocityY = Random16();
        star->variant = -i * 2;
      }
    }
    if (frame == 38)
      Func_081180e8(-1);
    if (mode == 0) {
      if (frame == 32)
        Sound_PlayFar(212);
    } else {
      if (frame > 7)
        Func_0815f0a0((s32)&ResourceId_StarCrossSheet);
      if (frame == 32)
        Func_0814cd48(effect->actors[0], 7, -1, 0, 24);
    }

    // The Djinni rises a pixel a frame, swaying, until it leaves the top.
    if (--center[1] > 0) {
      s32 height = center[1];

      if (height > 39)
        height = 40;
      blit[0](canvas, work->sheet + DJINNI_OFFSET,
              ((Trig_Sin(frame << 10) << 5) >> 16) + 44, center[1] - 20, 40,
              height);
    }

    Render_ResetTransformState();
    gCameraSceneParameters[4] = 104;
    vertices = Heap_AllocEwram(32);
    model = Render_AllocateModelList(1);
    texture.logWidth = 6;
    texture.logHeight = 6;
    model->kind = 6;
    model->texture = &texture;
    model->mesh = gData_08199340;
    model->vertices = vertices;
    if (mode == 0) {
      // Rime's crystal fades in over the first eight frames and turns in
      // two mirrored copies that come together at frame 32, shrinking
      // from twice its size.
      s32 brightness = frame * 8 - 64;

      if (brightness > 0)
        brightness = 0;
      if (frame > 55)
        brightness = (56 - frame) * 8;
      model->brightness = brightness;
      texture.pixels = work->sheet + CRYSTAL_OFFSET;
      for (i = 0; i != 2; i++) {
        s32 scale = 0x20000 - frame * 0x800;

        if (frame > 31 && i == 1)
          continue;
        if (scale < 0x10000)
          scale = 0x10000;
        Render_ResetTransformState();
        SceneTransform_ApplyPitch(-0x4000);
        if (frame <= 31) {
          if (i == 0)
            SceneTransform_ApplyYaw((32 - frame) << 9);
          else
            SceneTransform_ApplyYaw((frame << 9) - 0x4000);
        }
        Func_080151e4(scale >> 1, scale >> 1, scale >> 1);
        Render_TransformVertices(gData_08199210, vertices, 4);
        Render_DrawModelList(model);
      }
    } else {
      // Each of Haze's stars swells and shrinks over 16 frames, and every
      // fourth one chimes as it opens.
      struct EffectStep *star;

      model->brightness = 0;
      texture.pixels = work->sheet;
      for (i = 0, star = work->particles; i != 16; i++, star++) {
        if (star->variant == 4 && (i & 3) == 0)
          Sound_PlayFar(130);
        if (star->variant >= 0 && star->variant < 16) {
          s32 swell = Trig_Sin(star->variant << 11) * star->z;
          s32 size;

          Render_ResetTransformState();
          Func_08015160(star->x, star->y, 0);
          size = swell >> 8;
          SceneTransform_ApplyPitch(0x4000);
          SceneTransform_ApplyYaw(frame << 9);
          size /= 2;
          Func_080151e4(size, size, size);
          Render_TransformVertices(gData_08199210, vertices, 4);
          Render_DrawModelList(model);
        }
        star->variant++;
      }
    }
    Heap_Release(model);
    Heap_Release(vertices);
    gCameraSceneParameters[3] = 120;
    gCameraSceneParameters[4] = 120;

    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  HeapSlot_Release(104);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

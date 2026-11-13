#include "types.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "heap_release.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "task.h"

struct Model;

// The integer (pixel) half of a 16.16 value, read as the halfword it is.
#define HI(v) (((s16 *)&(v))[1])

#define KIND123_FRAMES 111

// A texture for the ARM model renderer: its size as powers of two and the
// address of its pixels.
struct ModelTexture {
  u32 logWidth : 8;
  u32 logHeight : 8;
  u32 unk02 : 16;
  const void *pixels;
};

// A model for the ARM renderer (Render_DrawModelList).
struct RenderModel {
  s32 kind;
  s32 unk04;
  const u8 *mesh;
  u8 *vertices;
  struct ModelTexture *texture;
  s32 shade;
};

extern char ResourceId_FlashBurstSheet;
extern char ResourceId_Data0CB;
extern char ResourceId_Data0C9;
extern char ResourceId_Data0C2;
extern char ResourceId_Data134;
extern char ResourceId_OrangePaletteB;
extern u8 gData_02014000[];
// The first load names the buffer by its address: as the symbol it would
// combine with the second.
#define EFFECT_PICTURES (Ram_MapCellBuffer + 0x4000)
extern u16 gData_08197410[];
extern u8 Data_081992b0[];
extern const u8 Data_08199220[];
extern const u8 gData_08199364[];
extern const u8 Data_08199eb4[];
extern const u8 Data_08199ebc[];
extern const u8 Data_08199ea4[];
extern const u8 Data_081991c0[];
extern const u8 Data_081991e0[];
extern const u8 gData_08199210[];

void BattleEffect_StartCanvasLayer(s32 layer);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_FetchRectangleBlitters(s32 flip, void **out);
void BattleEffect_ProcessPendingGraphicsTransfer(void);
void *Resource_GetTableEntry(s32 id);

void EffectPosition_ProjectBattler(s32 id, struct EffectPosition *out);
void wait(s32 frames);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                          s32 gravity);
struct PoolEntry *Render_AllocateModelList(s32 count);
void Render_ResetTransformState(void);
void Func_08015160(s32 x, s32 y, s32 z);
void Func_080151e4(s32 x, s32 y, s32 z);
void Func_080150e4(s32 angle);
void SceneTransform_ApplyPitch(s32 angle);
void Render_TransformVertices(const void *corners, void *vertices, s32 count);
void Render_DrawModelList(struct Model *model);
void Func_0814cd48(s32 actor, s32 palette, s32 mode, s32 timer, s32 frames);
void BattleEffect_StartActorMotion(s32 id, s32 force, s32 acceleration,
                                   s32 speedLimit, s32 velocityY, s32 percent);
void BattleCamera_Shake(s32 rangeX, u32 rangeY);
void BattleEffect_CountDownBattlerTints(void);
void Func_081180e8(s32 phase);
void Sound_PlayFar(s32 cue);

// Effect kind 123, 111 frames. Sparks gather at the caster (frames 0-15)
// and burst from it toward the target (16-110); eight flat flashes turn about
// the caster; sixteen textured shards fly from the target (54-101), each
// leaving a fading splash; two rings swell at the caster (0-15); every four
// frames from 54 the target is struck, and the last strike ends the phase.
//
// The work's particles 0-31 are the sparks, 32-47 the shards (z holds each
// shard's roll); the map cell buffer is cleared but not drawn.
void BattleEffect_RunLiquifier(struct BattleEffectArgument *effect) {
  struct EffectPosition from;
  struct EffectPosition to;
  DrawRectangle draw[2];
  struct ModelTexture texture;
  void *canvas;
  struct BattleEffectWork *work;
  s32 frame;
  u8 *graphics;
  s32 i;

  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  graphics = Ram_HeapSlots->unk64[0];
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = 0x1010;
  BattleEffect_FetchRectangleBlitters(0, draw);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, EFFECT_PICTURES,
                             1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0CB, work->sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C9, work->sheet + 0x1000, 0,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C2, gData_02014000, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, graphics, 0, 0);
  Iwram_CopyWordsEntry((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB), 128);
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  EffectPosition_ProjectBattler(effect->actor, &from);
  EffectPosition_ProjectBattler(effect->actors[0], &to);

  for (frame = 0; frame != KIND123_FRAMES; frame++) {
    if (frame == 0) {
      struct EffectStep *shard;

      for (i = 0; i != 512; i++)
        ((struct EffectStep *)Ram_MapCellBuffer)[i].variant = -1;

      // The shards start about the target, each with its own roll.
      for (i = 0, shard = &work->particles[32]; i != 16; i++, shard++) {
        shard->x = (to.x + (Random16() & 127) - 64) << 16;
        shard->y = to.y << 16;
        shard->z = (Random16() & 0x1fff) + 0x7000;
      }
    }
    if (frame == 0) {
      work->transferMode = 2;
      work->transferValue = 50;
    }
    if (frame == 12) {
      work->transferMode = 3;
      work->transferValue = 0x04040404;
    }
    if (frame == 54) {
      work->transferMode = 2;
      work->transferValue = 50;
    }
    if (frame == 0)
      Sound_PlayFar(139);
    if (frame == 16)
      Sound_PlayFar(134);

    if (frame > 15) {
      // The burst: sparks fly out of the caster, upward and forward.
      if (frame == 16) {
        struct EffectStep *step = work->particles;

        for (i = 0; i != 32; i++) {
          u32 angle = Random16() % 0x5fff + 0x5000;
          s32 speed = (Random16() & 0x1ff) + 0x200;

          step->velocityX = (Trig_Sin(angle) * speed) >> 8;
          step->velocityY = (Trig_Cos(angle) * speed) >> 7;
          step->x = (from.x / 2) << 16;
          step->y = (from.y - (Random16() & 15) - 8) << 16;
          step->variant = 0;
          step++;
        }
      }
      // Two steps a frame, the n-th spark from frame n / 4.
      for (i = 0; i != 32; i++) {
        s32 one = 1;
        struct EffectStep *step = &work->particles[i];

        if (step->variant >= 0 && frame >= i / 4) {
          s32 odd = i;
          s32 size;
          s32 blend;
          s32 j;
          odd &= one;
          size = odd + 5;
          j = 0;
          blend = odd * 4;

          {
            s32 twice = size * 2;
            s32 half = (u32)size >> 1;

            for (; j != 2; j++) {
              (*(DrawRectangle *)((u8 *)draw + blend))(
                  canvas, graphics + gData_08197410[size - 1],
                  HI(step->x) - half, HI(step->y) - size, size, twice);
              BattleEffectKernels_IntegrateVector2(step, 64, 0);
            }
          }
          step->variant++;
        }
      }
    } else {
      struct EffectStep *step;

      // The gathering: sparks start 64 steps out and fall into the caster.
      if (frame == 0) {
        struct EffectStep *step = work->particles;

        for (i = 0; i != 32; i++) {
          s32 angle = (Random16() & 0x7fff) - 0x4000;
          s32 speed = (Random16() & 0x1ff) + 0x200;
          s32 y;

          step->velocityX = (Trig_Sin(angle) * speed) >> 10;
          step->velocityY = (Trig_Cos(angle) * speed) >> 9;
          step->x = (from.x / 2) << 16;
          y = from.y - (Random16() & 15) - 8;
          step->x -= step->velocityX << 6;
          step->y = (y << 16) - (step->velocityY << 6);
          step->variant = 0;
          step++;
        }
      }
      // Four steps a frame.
      for (i = 0, step = work->particles; i != 32; i++, step++) {
        s32 one = 1;
        if (step->variant >= 0) {
          s32 odd = i;
          s32 size;
          s32 blend;
          s32 j;
          odd &= one;
          size = odd + 2;
          j = 0;
          blend = odd * 4;

          {
            s32 twice = size * 2;
            s32 half = (u32)size >> 1;

            for (; j != 4; j++) {
              (*(DrawRectangle *)((u8 *)draw + blend))(
                  canvas, graphics + gData_08197410[size - 1],
                  HI(step->x) - half, HI(step->y) - size, size, twice);
              BattleEffectKernels_IntegrateVector2(step, 64, 0);
            }
          }
          step->variant++;
        }
      }
    }

    {
      void *projected = Heap_AllocEwram(32);
      struct RenderModel *model = Render_AllocateModelList(1);
      void *splash = gData_02014000;

      // Eight flashes turn about the caster, each for 16 frames from its
      // start, swinging wider and back with a sine.
      model->kind = 6;
      model->texture = &texture;
      model->vertices = projected;
      texture.logWidth = 6;
      texture.logHeight = 6;
      texture.pixels = work->sheet;
      model->mesh = Data_081992b0;
      for (i = 0; i != 8; i++) {
        s32 start = Data_08199eb4[i];

        if (frame >= start && frame < start + 16) {
          s32 age;

          model->shade = 0;
          age = frame - start;
          Render_ResetTransformState();
          Func_080151e4(0x8000, 0x10000, 0x8000);
          Func_08015160((from.x - 128) << 16, (from.y - 74) << 16, 0);
          Func_080150e4((i << 12) - 0x3800);
          Func_080151e4(0x5555, Trig_Sin(age << 11) * Data_08199ebc[i] / 64 * 2,
                        0x8000);
          Render_TransformVertices(Data_08199ea4, projected, 4);
          Render_DrawModelList(model);
        }
      }

      // The shards: each from frame 54 + 2i for 16 frames, shrinking.
      texture.logWidth = 5;
      texture.logHeight = 2;
      texture.pixels = work->sheet + 0x1000;
      model->mesh = Data_08199220;
      model->kind = 7;
      for (i = 0; i != 16; i++) {
        s32 zero = 0;
        struct EffectStep *step = &work->particles[32 + i];
        s32 start = i * 2 + 54;

        if (frame >= start && frame < i * 2 + 70) {
          s32 scale;

          scale = 0x20000 - ((frame - start) << 13);
          model->shade = zero;
          Render_ResetTransformState();
          Func_080151e4(0x8000, 0x10000, 0x8000);
          Func_08015160(step->x - (128 << 16), step->y - (64 << 16), 0);
          Func_080150e4(step->z);
          Func_080151e4(scale, 0x10000, 0x8000);
          Render_TransformVertices(Data_081991c0, projected, 4);
          Render_DrawModelList(model);
        }
      }

      // Their splashes: growing and darkening, faster after eight frames.
      texture.logWidth = 7;
      texture.logHeight = 7;
      texture.pixels = splash;
      model->mesh = gData_08199364;
      model->kind = 7;
      for (i = 0; i != 16; i++) {
        struct EffectStep *step = &work->particles[i + 32];
        s32 age = frame - 54 - i * 2;
        s32 back = -age;

        if (frame >= i * 2 + 54 && frame < i * 2 + 70) {
          s32 shade = back * 4;
          s32 scale = age << 14;

          if (age > 7) {
            shade -= age * 8;
            shade += 64;
          }
          model->shade = shade;
          Render_ResetTransformState();
          Func_080151e4(0x8000, 0x10000, 0x8000);
          Func_08015160(step->x - (128 << 16), step->y - (64 << 16), 0);
          SceneTransform_ApplyPitch(0x3c00);
          Func_080151e4(scale, scale, scale);
          Render_TransformVertices(Data_081991e0, projected, 4);
          Render_DrawModelList(model);
        }
      }

      // Two rings at the caster, from frames 0 and 4 for 12 frames each.
      for (i = 0; i != 2; i++) {
        if (frame >= i * 4 && frame < i * 4 + 12) {
          s32 age = frame - i * 4;
          s32 shade = age * 8 - 64;
          s32 scale = 0x18000 - (age << 13);

          if (shade > 0)
            shade = 0;
          model->shade = shade;
          Render_ResetTransformState();
          Func_080151e4(0x8000, 0x10000, 0x8000);
          Func_08015160((from.x - 128) << 16, (from.y - 80) << 16, 0);
          Func_080151e4(scale * 2, scale * 2, scale);
          Func_080150e4(0x6000 - (i << 14));
          SceneTransform_ApplyPitch(-0x1000);
          Render_TransformVertices(gData_08199210, projected, 4);
          Render_DrawModelList(model);
        }
      }

      // A strike every four frames from 54; the last one ends the phase.
      for (i = 0; i != 8; i++) {
        if (frame == i * 4 + 54) {
          if (i == 7)
            Func_081180e8(144);
          else
            Sound_PlayFar(144);
          BattleEffect_StartActorMotion(effect->actors[0], 1, 0, 0, 0, 100);
          Func_0814cd48(effect->actors[0], 7, 5, 0, 5);
          work->shakeFrames = 4;
        }
      }
      Heap_Release(model);
      Heap_Release(projected);
    }

    BattleCamera_Shake(8, 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(188);
  HeapSlot_Release(104);
  BattleEffect_EndCanvasLayer();
}

#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"

#include "motobj.h"
#include "field_event.h"

#define Actor_SetAnimationSpeedFar                                             \
  ((s32 (*)(struct Actor *, s32))Actor_SetAnimationSpeedFar)

void BattleEffect_RunCanvasModeByCount(struct BattleEffectArgument *effect) {
  if (effect->kind > 199)
    Func_0818f620(effect, 2);
  else
    Func_0818f620(effect, 1);
}

DEFINE_EFFECT_KIND(BattleEffect_RunCanvasMode0, Func_0818f620, 0)

void BattleEffect_RunCanvasMode3WithAction(
    struct BattleEffectArgument *effect) {
  struct Actor *object;

  object = (struct Actor *)GetBattleObjectSlotFar(effect->actor)->object;
  Actor_SetMode(object, 2);
  Actor_SetAnimationSpeedFar(object, 0x1c);
  Func_0818f620(effect, 3);
  Actor_SetAnimationSpeedFar(object, 0x10);
}

#include "io_write_queue.h"
#include "bg_scroll.h"
#include "battle_work.h"

// The ball is the last particle, never a local: the native rereads its address.
#define ball (&work->particles[63])
extern char ResourceId_BlueFlameSheet;
extern const u8 Data_08199ec5[];

s32 BattleEffect_LoadBlitter(s32 slot, s32 value);
void Palette_StepFadeTransfer(void);
void Func_0815b434(u8 *map, s32 size, s32 scale);
void Func_0815e22c(struct EffectPosition *position);

void Func_08118088(s32 actor, s32 mode);
void SummonEffect_RunCanvasMode1(void *action);
void BattleStaging_ConfigurePaletteFadeFar(s32 mode, s32 background, s32 value);

// Draws a disc of the given size from the disc sheet in the map cell buffer
// as four quarter blits about (x, y), with the second blitter loaded for
// each quarter's flip (flags 3, 7, 11 and 15 of the maximum mode). The sheet
// offset comes first: the sum only matches in integer form, cells on the right.
static inline void DrawDisc(void *canvas, s16 *discs, s32 cells, s32 x, s32 y,
                            s32 size, s32 index) {
  BattleEffect_LoadBlitter(188, 1 << 4 | 3);
  ((DrawRectangle)Ram_HeapSlots->unkb8[1])(canvas, (u8 *)(discs[index] + cells),
                                           x - (size >> 1), y - size, size >> 1,
                                           size);
  HeapSlot_Release(188);
  BattleEffect_LoadBlitter(188, 1 << 4 | 7);
  ((DrawRectangle)Ram_HeapSlots->unkb8[1])(canvas, (u8 *)(discs[index] + cells),
                                           x, y - size, size >> 1, size);
  HeapSlot_Release(188);
  BattleEffect_LoadBlitter(188, 1 << 4 | 11);
  ((DrawRectangle)Ram_HeapSlots->unkb8[1])(canvas, (u8 *)(discs[index] + cells),
                                           x - (size >> 1), y, size >> 1, size);
  HeapSlot_Release(188);
  BattleEffect_LoadBlitter(188, 1 << 4 | 15);
  ((DrawRectangle)Ram_HeapSlots->unkb8[1])(canvas, (u8 *)(discs[index] + cells),
                                           x, y, size >> 1, size);
  HeapSlot_Release(188);
}

// The canvas burst family (wrappers in f18eda4.c). A glowing ball (or
// in mode 0 a growing glow at the target, with sixteen rays that close on
// it) hits the target; at the hit the targets flash and recoil, sixteen
// flames rise and 47 sparks scatter. Modes:
//   0  (BattleEffect_RunCanvasMode0): 78 frames, the glow at the target; the
//      hit at frame 54, then the canvas is cleared and
//      SummonEffect_RunCanvasMode1 runs on;
//   1  (by count, kinds up to 199): 66 frames, a small ball (24 pixels),
//      half-speed sparks, hit at 32, no flames;
//   2  (by count, kinds 200 and up): 66 frames, a large ball (40), hit at
//      32;
//   3  (BattleEffect_RunCanvasMode3WithAction): 58 frames, the ball flies to
//   the
//      midpoint of the targets, its own palette, hit at 24.
// Modes 1 and 2 close by restoring the battle display themselves.
void Func_0818f620(struct BattleEffectArgument *effect, u32 mode) {
  struct EffectPosition from;
  struct EffectPosition to;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  DrawRectangle draw[2]; // Unused: draw[1] gives the stack frame its 4 bytes.
  u8 *graphics;
  s16 *discs;
  s32 frames;
  s32 hitFrame;
  s32 offset;
  s32 i;
  s32 j;
  s32 cells = (s32)Ram_MapCellBuffer;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  graphics = Ram_HeapSlots->unk64[0];
  if (mode <= 1)
    BattleEffect_StartCanvasLayer(1);
  else
    BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = 0x1010;
  BattleEffect_LoadBlitter(104, 1 << 4 | 3);
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  discs = (s16 *)work->sheet;
  if (mode == 0)
    frames = 78;
  else if (mode == 3)
    frames = 58;
  else
    frames = 66;
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, graphics, 0, 0);

  // Discs of sizes 2 to 64 one after another in the map cell buffer, with
  // their offsets at the start of the sheet.
  offset = 0;
  for (i = 0; i != 32; i++) {
    s32 size = i * 2 + 2;

    Func_0815b434((u8 *)(offset + cells), size, 0x14e20);
    discs[i] = offset;
    offset += (i * 2 + 3) * size / 2;
  }
  if (mode == 3)
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_BlueFlameSheet),
                    128);
  else
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB),
                    128);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                             work->sheet + 0x800, 1, 0);

  for (frame = 0; frame != frames; frame++) {
    EffectPosition_ProjectBattler(effect->actor, &from);
    if (mode == 3)
      Func_0815e22c(&to);
    else
      EffectPosition_ProjectBattler(effect->actors[0], &to);
    if (mode == 0 && frame == 0)
      Sound_PlayFar(140);

    if (frame == 0) {
      if (mode == 0) {
        struct EffectStep *step;

        // The rays: one every 1/16 turn, each starting after its own delay.
        for (i = 0; i != 16; i++) {
          step = &work->particles[i];
          step->z = i << 12;
          step->variant = (Random16() & 31) + 16;
        }
      } else {
        // The ball leaves the caster's chest, aimed to arrive in 128
        // frames' worth of steps, already seven steps along.
        ball->x = (from.x / 2) << 16;
        ball->y = (from.y - 36) << 16;
        ball->velocityX = (to.x - from.x) << 10;
        ball->velocityY = (to.y - from.y) << 10;
        ball->x += ball->velocityX * 7;
        ball->y += ball->velocityY * 7;
      }
    }

    if (mode == 0) {
      // Frames 18-54: the glow grows at the target, up to 64 pixels.
      if ((u32)(frame - 18) <= 36) {
        s32 size = frame * 2 - 32;
        s32 x;
        s32 y;

        if (size > 64)
          size = 64;
        x = to.x / 2;
        y = to.y - 32;
        DrawDisc(canvas, discs, cells, x, y, size, (size - 2) >> 1);
      }
    } else if ((u32)(frame - 2) <= 30) {
      // Frames 2-32: the ball flies, growing to 24 (mode 1) or 40 pixels.
      s32 size = frame / 4 * 2 + 16;
      s32 index;
      s32 x;
      s32 y;

      if (mode == 1) {
        if (size > 24)
          size = 24;
      } else if (size > 40) {
        size = 40;
      }
      index = (size - 2) >> 1;
      x = ball->x >> 16;
      y = ball->y >> 16;
      ball->x += ball->velocityX;
      ball->y += ball->velocityY;
      DrawDisc(canvas, discs, cells, x, y, size, index);
    }

    // Mode 0, frames 0-53: sixteen rays of motes close on the target.
    if (mode == 0 && frame <= 53) {
      struct EffectStep *step = work->particles;

      for (i = 0; i != 16; i++) {
        if ((u32)(--step->variant + 32) <= 47) {
          s32 size = 4;

          for (j = 0; j != 16; j++) {
            s32 radius = (step->variant * 2 + j) * 2;

            if (radius >= 0) {
              s32 width = 4;
              s32 x = to.x / 2 + ((Trig_Sin(step->z) * radius) >> 16);
              s32 height = size * 2;
              s32 half = size >> 1;
              s32 y = to.y + ((Trig_Cos(step->z) * radius) >> 16);

              draw[0](canvas, graphics + gData_08197410[size - 1], x - half,
                      y - 32 - size, width, height);
            }
          }
        }
        step++;
      }
    }

    if (mode == 0)
      hitFrame = 54;
    else if (mode == 3)
      hitFrame = 24;
    else
      hitFrame = 32;
    if (frame >= hitFrame) {
      struct EffectStep *newFlame;
      struct EffectStep *newSpark;
      struct EffectStep *flame;
      struct EffectStep *spark;

      if (frame == hitFrame) {
        // The hit: the targets recoil (mode 1 keeps them still) and flash.
        if (mode == 0 || mode == 2 || mode == 3) {
          Func_081180e8(145);
          for (i = 0; i != effect->count; i++)
            Func_08118088(effect->actors[i], 4);
        } else {
          Func_081180e8(133);
          for (i = 0; i != effect->count; i++)
            Func_08118088(effect->actors[i], 0);
        }
        for (i = 0; i != effect->count; i++)
          Func_0814cd48(effect->actors[i], 7, 5, i, 8);
        work->shakeFrames = 8;

        // Sixteen flames, faster one by one.
        for (i = 0; i != 16; i++) {
          s32 angle = Random16() & 0xffff;
          s32 speed;

          newFlame = &work->particles[i];
          speed = i * 4 + 4;
          newFlame->x = (to.x / 2) << 16;
          newFlame->y = (to.y - 32) << 16;
          newFlame->velocityX = (speed * Trig_Sin(angle)) >> 3;
          newFlame->velocityY = -(speed * Trig_Cos(angle)) >> 3;
          newFlame->variant = i / 3 + 17;
        }

        // 47 sparks, at half speed in mode 1.
        for (i = 0; i != 47; i++) {
          s32 speed = (Random16() & 255) + 128;
          s32 angle = Random16() & 0xffff;

          newSpark = &work->particles[16 + i];

          newSpark->x = (to.x / 2) << 16;
          newSpark->y = (to.y - 32) << 16;
          newSpark->velocityX = (Trig_Sin(angle) * speed) >> 6;
          newSpark->velocityY = -(Trig_Cos(angle) * speed) >> 5;
          if (mode == 1) {
            newSpark->velocityX /= 2;
            newSpark->velocityY /= 2;
          }
          newSpark->variant = i / 3 + 17;
        }
      }

      // The flames: six frames of the burst sheet, three frames each.
      if (mode == 0 || mode == 2 || mode == 3) {
        flame = work->particles;
        for (i = 0; i != 16; i++) {
          s32 x = HI(flame->x);
          s32 y = HI(flame->y);

          BattleEffectKernels_IntegrateVector2(flame, 60, 0);
          if ((u32)flame->variant <= 17)
            draw[0](canvas,
                    work->sheet + 0x800 +
                        (Data_08199ec5[flame->variant / 3] << 11),
                    x - 16, y - 32, 32, 64);
          if (flame->variant > 0)
            flame->variant = flame->variant - 1;
          else
            flame->variant = -1;
          flame++;
        }
      }

      // The sparks shrink as they fly.
      for (i = 0; i != 47; i++) {
        spark = &work->particles[16 + i];
        if (spark->variant >= 0) {
          s32 size = (spark->variant >> 2) + 1;

          draw[0](canvas, graphics + gData_08197410[size - 1],
                  HI(spark->x) - size / 2, HI(spark->y) - size, size, size * 2);
          BattleEffectKernels_IntegrateVector2(spark, 60, 0);
          spark->variant--;
        }
      }
    }

    BattleCamera_Shake(8, 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(104);
  if (mode == 0) {
    Iwram_ClearWords((void *)0x06004000, 0x3c00);
    Iwram_ClearWords(canvas, 0x3c00);
    Task_Remove(Palette_StepFadeTransfer);
    SummonEffect_RunCanvasMode1(effect);
  } else if (mode == 3) {
    BattleEffect_EndCanvasLayer();
  } else {
    // Back to the battle display: BG1's scroll, BG2 as before, the palette
    // fade off and the backgrounds on again.
    struct BattleWork *battle = Ram_HeapSlots->battleWork;

    Sound_PlayFar(390);
    gBgScroll[1].x = work->savedBg1X;
    gBgScroll[1].y = work->savedBg1Y;
    REG_BG2CNT = 0x787;
    Iwram_ClearWords((void *)0x06004000, 0x4000);
    Task_Remove(Palette_StepFadeTransfer);
    IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, 0x7341, 0x20000);
    REG_BLDCNT = 0;
    wait(1);
    BattleStaging_ConfigurePaletteFadeFar(2, *(u16 *)battle->unk670, 0);
    IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, 0x7541, 0x20000);
    wait(1);
  }
}


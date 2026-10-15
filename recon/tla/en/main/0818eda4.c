/* Draft, differ 809 of 895 (draft 874 instructions, no device): effect kind
 * 123, written from the listing on The Broken Seal's canvas skeleton
 * (twins: BattleFx_RunFallingSword 0.46, by call order
 * BattleFx_RunJupiterDjinnFall 0.50), with the ARM model renderer for its
 * flashes, shards, splashes and rings.
 * Every call, cue and table read is in the native's order; what is left is
 * the frame. The native frame is 92 bytes: from at sp+80, to at sp+68, the
 * blitter pair at sp+60, the texture at sp+52, effect sp+48, canvas sp+44,
 * work sp+40, frame sp+36, graphics sp+32, &draw sp+28, &from sp+24, &to
 * sp+20, the blend offset sp+16, projected sp+12, &texture sp+8. Here it is
 * 100: two more spills move every stack offset by 8, and the loops'
 * registers follow (frame, i and the step pointer).
 * Tried: the arrays declared from, to, draw, texture (no change). Next: the
 * spark loops' blend and size as the native's spills (blend * 4 at sp+16),
 * the step pointer of the burst loop walked rather than indexed. */
#include "types.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "mem.h"
#include "battle_effect.h"
#include "battle_effect_work.h"

// The integer (pixel) half of a 16.16 value, read as the halfword it is.
#define HI(v) (((s16 *)&(v))[1])

#define KIND123_FRAMES 111

// A texture for the ARM model renderer: its size as powers of two and the
// address of its pixels.
struct ModelTexture {
  u8 logWidth;
  u8 logHeight;
  u16 unk02;
  const void *pixels;
};

// A model for the ARM renderer (Func_08196a7c).
struct RenderModel {
  s32 kind;
  s32 unk04;
  const void *mesh;
  void *vertices;
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
extern struct EffectStep gMapCellBuffer[];
extern u16 gData_08197410[];
extern const u16 Data_081992b0[];
extern const u16 Data_08199220[];
extern const u16 gData_08199364[];
extern const u8 Data_08199eb4[];
extern const u8 Data_08199ebc[];
extern const u8 Data_08199ea4[];
extern const u8 Data_081991c0[];
extern const u8 Data_081991e0[];
extern const u8 gData_08199210[];

void BattleFx_BeginCanvasLayer(s32 layer);
void Func_08143bb8(void);
void Func_08144aac(s32 flip, DrawRectangle *out);
void Func_08143000(void);
void Resource_LoadAndDecompress(s32 id, void *target, s32 unpack, s32 wait);
void *Resource_GetTableEntry(s32 id);
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 order);
s32 Scheduler_RemoveCallback(void (*callback)(void));
void EffectPosition_ApplyAlternateStepAndYOffset(s32 id,
                                                 struct EffectPosition *out);
void Runtime_ReleaseHeapBlock(s32 slot);
void WaitFrames(s32 frames);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);
void BattleFxKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                      s32 gravity);
struct RenderModel *Func_081969f8(s32 count);
void Render_ResetTransformState(void);
void Func_08015160(s32 x, s32 y, s32 z);
void Func_080151e4(s32 x, s32 y, s32 z);
void Func_080150e4(s32 angle);
void SceneTransform_ApplyPitch(s32 angle);
void Func_08196958(const u8 *vertices, void *projected, s32 count);
void Func_08196a7c(struct RenderModel *model);
void Func_0814cd48(s32 actor, s32 palette, s32 mode, s32 timer, s32 frames);
void BattleFx_StartActorMotion(s32 id, s32 force, s32 acceleration,
                               s32 speedLimit, s32 velocityY, s32 percent);
void Func_08158ce0(s32 randomMask, u32 range);
void Func_081434f8(void);
void Func_081180e8(s32 phase);
void Audio_PlayCue(s32 cue);

// Effect kind 123, 111 frames. Sparks gather at the caster (frames 0-15)
// and burst from it toward the target (16-110); eight flat flashes turn about
// the caster; sixteen textured shards fly from the target (54-101), each
// leaving a fading splash; two rings swell at the caster (0-15); every four
// frames from 54 the target is struck, and the last strike ends the phase.
//
// The work's particles 0-31 are the sparks, 32-47 the shards (z holds each
// shard's roll); the map cell buffer is cleared but not drawn.
void Func_0818eda4(struct BattleEffectArgument *effect) {
  struct EffectPosition from;
  struct EffectPosition to;
  DrawRectangle draw[2];
  struct ModelTexture texture;
  void *canvas;
  struct BattleEffectWork *work;
  u8 *graphics;
  struct RenderModel *model;
  void *projected;
  struct EffectStep *step;
  s32 frame;
  s32 i;
  s32 j;
  s32 k;

  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  graphics = Ram_HeapSlots->unk64[0];
  BattleFx_BeginCanvasLayer(0);
  REG_BLDALPHA = 0x1010;
  Func_08144aac(0, draw);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, gData_02014000,
                             1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0CB, work->sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C9, work->sheet + 0x1000, 0,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C2, gData_02014000, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, graphics, 0, 0);
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB),
                  128);
  work->transferMode = 2;
  work->transferValue = 50;
  Scheduler_AddOrUpdateCallback(Func_08143000, 0xc80);
  EffectPosition_ApplyAlternateStepAndYOffset(effect->actor, &from);
  EffectPosition_ApplyAlternateStepAndYOffset(effect->actors[0], &to);

  for (frame = 0; frame != KIND123_FRAMES; frame++) {
    if (frame == 0) {
      for (i = 0; i != 512; i++)
        gMapCellBuffer[i].variant = -1;

      // The shards start about the target, each with its own roll.
      step = &work->particles[32];
      for (i = 0; i != 16; i++) {
        step->x = (to.x + (Random16() & 127) - 64) << 16;
        step->y = to.y << 16;
        step->z = (Random16() & 0x1fff) + 0x7000;
        step++;
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
      Audio_PlayCue(139);
    if (frame == 16)
      Audio_PlayCue(134);

    if (frame > 15) {
      // The burst: sparks fly out of the caster, upward and forward.
      if (frame == 16) {
        step = work->particles;
        for (i = 0; i != 32; i++) {
          u32 angle = Random16() % 0x5fff + 0x5000;
          s32 speed = (Random16() & 0x1ff) + 0x200;

          step->velocityX = (speed * Trig_Sin(angle)) >> 8;
          step->velocityY = (speed * Trig_Cos(angle)) >> 7;
          step->x = (from.x / 2) << 16;
          step->y = (from.y - (Random16() & 15) - 8) << 16;
          step->variant = 0;
          step++;
        }
      }
      // Two steps a frame, the n-th spark from frame n / 4.
      for (i = 0; i != 32; i++) {
        step = &work->particles[i];
        if (step->variant >= 0 && frame >= i / 4) {
          s32 blend = i & 1;
          u32 size = blend + 5;

          for (j = 0; j != 2; j++) {
            draw[blend](canvas, graphics + gData_08197410[size - 1],
                        HI(step->x) - (size >> 1), HI(step->y) - size, size,
                        size * 2);
            BattleFxKernels_IntegrateVector2(step, 64, 0);
          }
          step->variant++;
        }
      }
    } else {
      // The gathering: sparks start 64 steps out and fall into the caster.
      if (frame == 0) {
        step = work->particles;
        for (i = 0; i != 32; i++) {
          s32 angle = (Random16() & 0x7fff) - 0x4000;
          s32 speed = (Random16() & 0x1ff) + 0x200;

          step->velocityX = (speed * Trig_Sin(angle)) >> 10;
          step->velocityY = (speed * Trig_Cos(angle)) >> 9;
          step->x = (from.x / 2) << 16;
          step->x -= step->velocityX << 6;
          step->y = ((from.y - (Random16() & 15) - 8) << 16) -
                    (step->velocityY << 6);
          step->variant = 0;
          step++;
        }
      }
      // Four steps a frame.
      step = work->particles;
      for (i = 0; i != 32; i++) {
        if (step->variant >= 0) {
          s32 blend = i & 1;
          u32 size = blend + 2;

          for (j = 0; j != 4; j++) {
            draw[blend](canvas, graphics + gData_08197410[size - 1],
                        HI(step->x) - (size >> 1), HI(step->y) - size, size,
                        size * 2);
            BattleFxKernels_IntegrateVector2(step, 64, 0);
          }
          step->variant++;
        }
        step++;
      }
    }

    projected = Runtime_BumpAllocateAlternatePool(32);
    model = Func_081969f8(1);

    // Eight flashes turn about the caster, each for 16 frames from its
    // start, swinging wider and back with a sine.
    model->kind = 6;
    model->texture = &texture;
    texture.logWidth = 6;
    model->vertices = projected;
    texture.logHeight = 6;
    texture.pixels = work->sheet;
    model->mesh = Data_081992b0;
    for (k = 0; k != 8; k++) {
      s32 start = Data_08199eb4[k];

      if (frame >= start && frame < start + 16) {
        s32 age;

        model->shade = 0;
        age = frame - start;
        Render_ResetTransformState();
        Func_080151e4(0x8000, 0x10000, 0x8000);
        Func_08015160((from.x - 128) << 16, (from.y - 74) << 16, 0);
        Func_080150e4((k << 12) - 0x3800);
        Func_080151e4(0x5555,
                      Data_08199ebc[k] * Trig_Sin(age << 11) / 64 * 2,
                      0x8000);
        Func_08196958(Data_08199ea4, projected, 4);
        Func_08196a7c(model);
      }
    }

    // The shards: each from frame 54 + 2k for 16 frames, shrinking.
    texture.logWidth = 5;
    texture.logHeight = 2;
    texture.pixels = work->sheet + 0x1000;
    model->mesh = Data_08199220;
    model->kind = 7;
    step = &work->particles[32];
    for (k = 0; k != 16; k++) {
      if (frame >= k * 2 + 54 && frame < k * 2 + 70) {
        s32 scale;

        model->shade = 0;
        scale = 0x20000 - ((frame - 54 - k * 2) << 13);
        Render_ResetTransformState();
        Func_080151e4(0x8000, 0x10000, 0x8000);
        Func_08015160(step->x - (128 << 16), step->y - (64 << 16), 0);
        Func_080150e4(step->z);
        Func_080151e4(scale, 0x10000, 0x8000);
        Func_08196958(Data_081991c0, projected, 4);
        Func_08196a7c(model);
      }
      step++;
    }

    // Their splashes: growing and darkening, faster after eight frames.
    texture.logWidth = 7;
    texture.logHeight = 7;
    texture.pixels = gData_02014000;
    model->mesh = gData_08199364;
    model->kind = 7;
    step = &work->particles[32];
    for (k = 0; k != 16; k++) {
      if (frame >= k * 2 + 54 && frame < k * 2 + 70) {
        s32 age = frame - 54 - k * 2;
        s32 shade = -age * 4;

        if (age > 7)
          shade = shade - age * 8 + 64;
        model->shade = shade;
        Render_ResetTransformState();
        Func_080151e4(0x8000, 0x10000, 0x8000);
        Func_08015160(step->x - (128 << 16), step->y - (64 << 16), 0);
        SceneTransform_ApplyPitch(0x3c00);
        Func_080151e4(age << 14, age << 14, age << 14);
        Func_08196958(Data_081991e0, projected, 4);
        Func_08196a7c(model);
      }
      step++;
    }

    // Two rings at the caster, from frames 0 and 4 for 12 frames each.
    for (k = 0; k != 2; k++) {
      if (frame >= k * 4 && frame < k * 4 + 12) {
        s32 shade = frame * 8 - k * 32 - 64;
        s32 scale = 0x18000 - ((frame - k * 4) << 13);

        if (shade > 0)
          shade = 0;
        model->shade = shade;
        Render_ResetTransformState();
        Func_080151e4(0x8000, 0x10000, 0x8000);
        Func_08015160((from.x - 128) << 16, (from.y - 80) << 16, 0);
        Func_080151e4(scale * 2, scale * 2, scale);
        Func_080150e4(0x6000 - (k << 14));
        SceneTransform_ApplyPitch(-0x1000);
        Func_08196958(gData_08199210, projected, 4);
        Func_08196a7c(model);
      }
    }

    // A strike every four frames from 54; the last one ends the phase.
    for (k = 0; k != 8; k++) {
      if (frame == k * 4 + 54) {
        if (k == 7)
          Func_081180e8(144);
        else
          Audio_PlayCue(144);
        BattleFx_StartActorMotion(effect->actors[0], 1, 0, 0, 0, 100);
        Func_0814cd48(effect->actors[0], 7, 5, 0, 5);
        work->shakeFrames = 4;
      }
    }
    MemFree(model);
    MemFree(projected);

    Func_08158ce0(8, 8);
    Func_081434f8();
    work->transferPending = 1;
    WaitFrames(1);
  }

  Scheduler_RemoveCallback(Func_08143000);
  Runtime_ReleaseHeapBlock(188);
  Runtime_ReleaseHeapBlock(104);
  Func_08143bb8();
}

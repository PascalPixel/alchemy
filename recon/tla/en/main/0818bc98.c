// Near miss: listing 1251 instructions, draft 1252, differ 203. No close ☀️ twin
// (call order BattleFx_RunDragonCloud 0.417): ⚓️'s own effect on ☀️'s
// frame, its bursts and jaws drawn as textured quads by ⚓️'s model kernels, as
// in Triple Chomp (BattleFx_RunTripleChomp). The native frame (108) holds the mode at
// sp+64, work 60, canvas 56, frame 52, graphics 48, the user's position's
// address 44 (and a copy at 36), the target's 40, the mouth's x 32 and y 28,
// the sheet's glow cell 24, the blitter pair's address 20, the vertices 16 and
// the texture's address 12; the texture is at sp+68, the blitter pair at 76,
// the target at 84 and the user at 96. effect stays in r11. Written from the
// listing (2026-10-15, fx-n3), measured in order: 328 at the first compile
// (with or without a goto for Rolling Flame's early frames: the same); frame
// declared before graphics (their slots): 282; the mouth's x and y declared in
// the frame loop's block, in that order: 242 (y first: 282); the range tests
// grouped as their own operand of the &&, so they fold into the native's
// unsigned compares: 211; the vertex pool of 1024 bytes and the sparks' size
// stepped with ++: 207; the model's locals scoped to its block: 203. Worse: the
// beam's y after the ring's clamp (464). Left: the native keeps a second copy
// of the user's address at sp+36, above the mouth's slots, where this draft's
// copy falls below them (sp+28), and the TLA load-before-constant pairs.
#include "types.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "battle_effect.h"
#include "battle_effect_work.h"

// The Lost Age's heap slots, by byte offset into Ram_HeapSlots.
#define HEAP_SLOT_BLITTER 104
#define HEAP_SLOT_BLITTER_ALTERNATE 188

// The transfer callback runs at this order, where ☀️'s runs at 0x480.
#define TRANSFER_CALLBACK_ORDER 0xc80

// ⚓️'s blitter modes count the other way from ☀️'s COMPOSITE_* modes.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT, BLIT_COPY };

// Func_081963ec's packed argument: the mode in the high nibble.
#define BLITTER(flags, mode) ((mode) << 4 | (flags))

#define HI(v) (((s16 *)&(v))[1])

// The jaws' texture, unpacked into the map cell buffer's second 16 KB.
#define JAW_TEXTURE (Ram_MapCellBuffer + 0x4000)

// The four breaths, as the wrappers pass them.
enum {
  BREATH_BEAM,          // Beam, kind 73: a beam of fading rings.
  BREATH_ROLLING_FLAME, // Rolling Flame, kind 434: a ball rolls to the target.
  BREATH_PIKE,          // Ghost Soldier and Pike Assault, kind 449.
  BREATH_BLAST,         // Blast Breath, kind 414: a blast and a jet.
};

// A texture for the model kernels: its size as powers of two, then its
// pixels.
struct ModelTexture {
  u32 logWidth : 8;
  u32 logHeight : 8;
  u32 unk02 : 16;
  void *pixels;
};

// One entry of a model list, as Func_081969f8 allocates them and
// Func_08196a7c draws them.
struct Model {
  s32 kind;
  s32 next;
  void *mesh;
  void *vertices;
  struct ModelTexture *texture;
  s32 alpha;
  s32 tail;
};

extern char ResourceId_Data134;
extern char ResourceId_BlueFlameSheet;
extern char ResourceId_OrangePaletteB;
extern char ResourceId_Data0C2;
extern char ResourceId_Data0C1;
extern char ResourceId_Data0C9;
extern char ResourceId_DustCloudSheet;

// Each breath's length, the frame its burst starts, and the glow's widest
// size by variant.
extern u8 Data_08199d97[];
extern u8 Data_08199d9b[];
extern u8 Data_08199d9f[];

// The four jaws of variant 2, by breath: their start frames, their growth,
// and their x offsets from the mouth.
extern u8 Data_08199da3[];
extern u8 Data_08199daf[];
extern s8 Data_08199dbb[];

// The frame each breath hits the targets.
extern u8 Data_08199dc7[];

// The quad and jaw meshes and corners.
extern u8 gData_08199364[];
extern s32 gData_08199210[];
extern u8 Data_08198ec4[];
extern s32 Data_08198cac[];
extern u8 Data_08199220[];
extern s32 Data_081991c0[];

// The spark cells: offsets into the particle sheet by size.
extern u16 gData_08197410[];

void BattleFx_BeginCanvasLayer(s32 bgControl);
void *Resource_GetTableEntry(s32 id);
void Func_081963ec(s32 slot, s32 packed);
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 order);
s32 Scheduler_RemoveCallback(void (*callback)(void));
void Func_08157530(s32 actor, s32 target, s32 frames, s32 percent);
void EffectPosition_ApplyAlternateStepAndYOffset(
    s32 id, struct EffectPosition *position);
void Func_0815b510(u8 *map, s32 size, s32 scale);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);
void BattleFxKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                      s32 gravity);
void *Runtime_BumpAllocateAlternatePool(s32 size);
struct Model *Func_081969f8(s32 count);
void Render_ResetTransformState(void);
void Func_080151e4(s32 x, s32 y, s32 z);
void Func_08015160(s32 x, s32 y, s32 z);
void SceneTransform_ApplyPitch(s32 angle);
void SceneTransform_ApplyYaw(s32 angle);
void Func_080150e4(s32 angle);
void Func_08196958(void *corners, void *vertices, s32 count);
void Func_08196a7c(struct Model *model);
void MemFree(void *block);
void Audio_PlayCue(s32 cue);
void Func_081180e8(s32 phase);
void Func_0814cd48(s32 actor, s32 palette, s32 group, s32 timerSlot,
                   s32 frames);
void BattleFx_StartActorMotion(s32, s32, s32, s32, s32, s32);
void Func_08118088(s32 actor, s32 motion);
void Func_08158ce0(s32 randomMask, u32 shakeRange);
void Func_081434f8(void);
void WaitFrames(s32 frames);
void Runtime_ReleaseHeapBlock(s32 slot);
void Func_08143bb8(void);
void Func_08143000(void);

// The breaths (Beam, Rolling Flame, Ghost Soldier and Pike Assault, Blast
// Breath): 32 motes gather at the user's mouth, then burst out of it as
// sparks while a flash grows there. The Beam glows and fires a beam of
// fading rings; Rolling Flame rolls a ball of dust to the target; Blast
// Breath glows, blasts and jets. With variant 2, four jaws open one after
// another in front of the user. Each breath hits every target at its frame
// (Blast Breath strikes them in turn, four frames apart).
void Func_0818bc98(struct BattleEffectArgument *effect, s32 mode) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  u8 *graphics;
  struct EffectPosition user;
  struct EffectPosition target;
  DrawRectangle draw[2];
  struct ModelTexture texture;
  struct EffectStep *step;
  s32 i;
  s32 j;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  graphics = Ram_HeapSlots->unk64[0];

  // Set up the canvas, the sparks, the colours (blue for the pikes, orange
  // otherwise) and the jaws' texture, then each breath's own sheet: the Beam
  // computes 16 fading rings of 32 x 32.
  if (mode != BREATH_BEAM)
    BattleFx_BeginCanvasLayer(0);
  else
    BattleFx_BeginCanvasLayer(1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, graphics, 0, 0);
  if (mode == BREATH_PIKE)
    Iwram_CopyWords((void *)0x05000000,
                    Resource_GetTableEntry((s32)&ResourceId_BlueFlameSheet),
                    128);
  else
    Iwram_CopyWords((void *)0x05000000,
                    Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB),
                    128);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C2, Ram_MapCellBuffer, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C1, JAW_TEXTURE, 0, 0);
  if (mode == BREATH_BEAM) {
    for (i = 0; i != 16; i++) {
      u8 *pixel = work->sheet + i * 0x400;

      for (j = 0; j != 32; j++) {
        s32 shade;
        s32 k;

        if (effect->variant == 0)
          shade = ((Trig_Sin(j << 10) << 7) >> 16) - i * 4 - 64;
        else
          shade = ((Trig_Sin(j << 10) * 63) >> 16) - i * 4;
        if (shade > 63)
          shade = 63;
        if (shade < 0)
          shade = 0;
        for (k = 0; k != 32; k++)
          *pixel++ = shade;
      }
    }
  } else if (mode == BREATH_BLAST) {
    Resource_LoadAndDecompress((s32)&ResourceId_Data0C9, work->sheet, 0, 0);
  } else {
    Resource_LoadAndDecompress((s32)&ResourceId_DustCloudSheet, work->sheet, 1,
                               0);
  }

  // A clipped MAX blitter facing the targets, and a clipped additive one.
  REG_BLDALPHA = 0x1010;
  if (effect->side == 0)
    Func_081963ec(
        HEAP_SLOT_BLITTER,
        BLITTER(COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X,
                BLIT_MAX));
  else
    Func_081963ec(HEAP_SLOT_BLITTER,
                  BLITTER(COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y, BLIT_MAX));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  Func_081963ec(HEAP_SLOT_BLITTER_ALTERNATE,
                BLITTER(COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y, BLIT_ADDITIVE));
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
  work->transferMode = 2;
  work->transferValue = 50;
  Scheduler_AddOrUpdateCallback(Func_08143000, TRANSFER_CALLBACK_ORDER);

  // The pikes lunge; Blast Breath comes from 32 pixels in front of the user.
  if (mode == BREATH_PIKE)
    Func_08157530(effect->actor, effect->actors[0], 10, 80);
  EffectPosition_ApplyAlternateStepAndYOffset(effect->actor, &user);
  if (mode == BREATH_BLAST) {
    if (effect->side == 0)
      user.x -= 32;
    else
      user.x += 32;
    user.y -= 8;
  }
  EffectPosition_ApplyAlternateStepAndYOffset(effect->actors[0], &target);

  for (frame = 0; frame != Data_08199d97[mode]; frame++) {
    s32 mouthX;
    s32 mouthY;

    // Frame 0: 32 motes wait around the mouth, each a frame later; the
    // rolling ball sets out from the mouth toward the target.
    if (frame == 0) {
      for (i = 0; i != 32; i++) {
        step = &work->particles[32 + i];
        step->variant = -i;
        step->x = Random16();
        step->y = (Random16() & 31) + 32;
      }
      step = &work->particles[0];
      step->x = (user.x / 2) << 16;
      step->velocityX = 0;
      step->y = (user.y - 32) << 16;
      step->velocityY = 0;
      step->z = (target.x / 2 - user.x / 2) << 10;
      step->variant = 0;
      step->velocityZ = (target.y - user.y) << 10;
    }
    mouthY = user.y - 32;
    if (effect->side == 0)
      mouthX = user.x / 2 - 8;
    else
      mouthX = user.x / 2 + 8;

    // The Beam's glow swells and shrinks at the mouth, frames 16-79.
    if (mode == BREATH_BEAM && (frame >= 16 && frame <= 79)) {
      s32 size = frame - 16;

      if (frame > 63)
        size = Data_08199d9f[effect->variant] - frame + 64;
      size = size / 2 * 2;
      if (size > Data_08199d9f[effect->variant])
        size = Data_08199d9f[effect->variant];
      if (size > 0) {
        Func_0815b510(work->sheet + 0x4000, size, 0x10000);
        draw[1](canvas, work->sheet + 0x4000, mouthX - size / 2, mouthY - size,
                size, size * 2);
      }
    }

    // Blast Breath's glow, frames 0-47, shrinking from frame 40.
    if (mode == BREATH_BLAST && (frame >= 0 && frame <= 47)) {
      s32 size = 40;
      s32 half;

      if (frame > 39)
        size = 104 - frame * 2;
      half = size >> 1;
      size = half * 2;
      Func_0815b510(work->sheet + 0x4000, size, 0x10000);
      draw[1](canvas, work->sheet + 0x4000, mouthX - (size >> 1), mouthY - size,
              size, half * 4);
    }

    if (frame >= Data_08199d9b[mode]) {
      // The burst: the motes fly out of the mouth, away from the targets.
      if (frame == Data_08199d9b[mode]) {
        for (i = 0; i != 32; i++) {
          step = &work->particles[32 + i];
          step->x = mouthX << 16;
          step->y = mouthY << 16;
          step->velocityY = ((Random16() & 63) - 32) << 14;
          if (effect->side == 0)
            step->velocityX = -(Random16() & 63) << 13;
          else
            step->velocityX = (Random16() & 63) << 13;
          step->variant = (Random16() & 15) + 16;
        }
      }
      for (i = 0; i != 32; i++) {
        step = &work->particles[32 + i];
        if (step->variant >= 0) {
          s32 size = (i & 3) + 1;

          if (effect->variant == 1)
            size++;
          else if (effect->variant == 2)
            size = (i & 3) + 3;
          draw[i & 1](canvas, graphics + gData_08197410[size - 1],
                      HI(step->x) - size / 2, HI(step->y) - size, size,
                      size * 2);
          BattleFxKernels_IntegrateVector2(step, 60, 0);
          step->variant--;
        }
      }
    } else {
      // Before it: the motes spiral into the mouth.
      for (i = 0; i != 32; i++) {
        step = &work->particles[32 + i];
        if (++step->variant >= 0) {
          s32 x = mouthX + ((Trig_Sin(step->x) * step->y) >> 16);
          s32 y = mouthY + ((Trig_Cos(step->x) * step->y) >> 16);
          s32 size = (i & 3) + 3;

          if (step->y > 1) {
            step->y -= 2;
            draw[i & 1](canvas, graphics + gData_08197410[size - 1],
                        x - size / 2, y - size, size, size * 2);
          }
        }
      }
    }

    if (mode == BREATH_BEAM) {
      // From frame 56 the beam: four rings in a row, fading from frame 72.
      if (frame > 55) {
        s32 x = user.x / 2;
        s32 ring = frame - 72;
        s32 y = user.y - 48;

        if (ring < 0)
          ring = 0;
        if (ring > 15)
          ring = 15;
        for (i = 0; i != 4; i++) {
          if (effect->side == 0) {
            draw[0](canvas, work->sheet + (ring << 10), x - 40, y, 32, 32);
            x -= 32;
          } else {
            draw[0](canvas, work->sheet + (ring << 10), x + 8, y, 32, 32);
            x += 32;
          }
        }
      }
    } else if (mode == BREATH_ROLLING_FLAME && frame >= Data_08199d9b[mode]) {
      // The ball of dust rolls to the target.
      step = &work->particles[0];
      draw[0](canvas, work->sheet, HI(step->x) - 16, HI(step->y) - 24, 32, 48);
      BattleFxKernels_IntegrateVector2(step, 56, 0);
      step->velocityX += step->z;
      step->velocityY += step->velocityZ;
      step->variant++;
    }

    if (frame >= Data_08199d9b[mode]) {
      void *vertices;
      struct Model *model;

      vertices = Runtime_BumpAllocateAlternatePool(1024);
      model = Func_081969f8(1);
      texture.logWidth = 7;
      texture.logHeight = 7;
      model->kind = 6;
      model->mesh = gData_08199364;
      model->texture = &texture;
      model->vertices = vertices;
      texture.pixels = Ram_MapCellBuffer;

      // The flash at the mouth grows and fades over 16 frames; Blast
      // Breath's is two, crossed.
      if (mode != BREATH_ROLLING_FLAME) {
        s32 age = frame - Data_08199d9b[mode];
        s32 scale = age * 10192 + 0x8000;
        s32 alpha = 128 - age * 8;

        if (effect->variant == 0)
          scale >>= 1;
        else if (effect->variant == 1)
          scale = scale * 3 / 2;
        if (alpha > 0)
          alpha = 0;
        if (alpha > -64) {
          s32 count = 2 - (mode != BREATH_BLAST);

          model->alpha = alpha;
          for (i = 0; i != count; i++) {
            Render_ResetTransformState();
            if (mode == BREATH_BLAST) {
              Func_08015160((mouthX - 64) << 16, (mouthY - 64) << 16, 0);
              Func_080151e4(0x10000, 0x20000, 0x10000);
              Func_080150e4((i << 14) + 0x2000);
              SceneTransform_ApplyPitch(-0x1000);
            } else {
              if (effect->side == 0)
                Func_08015160((mouthX - 72) << 16, (mouthY - 64) << 16, 0);
              else
                Func_08015160((mouthX - 56) << 16, (mouthY - 64) << 16, 0);
              SceneTransform_ApplyYaw(0x32c8);
              SceneTransform_ApplyPitch(0x4000);
            }
            if (mode == BREATH_PIKE)
              scale /= 2;
            Func_080151e4(scale / 2, scale, scale / 2);
            Func_08196958(gData_08199210, vertices, 4);
            Func_08196a7c(model);
          }
        }
      }

      // Variant 2: four jaws open one after another.
      if (effect->variant == 2) {
        texture.pixels = JAW_TEXTURE;
        model->kind = 7;
        model->mesh = Data_08198ec4;
        for (i = 0; i != 4; i++) {
          s32 start = Data_08199da3[mode * 4 + i];

          if (frame > start) {
            s32 scale =
                (frame - start) * Data_08199daf[mode * 4 + i] * 1500 + 0x4180;
            s32 alpha = (start - frame) * 8 + 56;

            if (alpha > 0)
              alpha = 0;
            if (alpha > -64) {
              model->alpha = alpha;
              Render_ResetTransformState();
              if (mode == BREATH_PIKE) {
                scale /= 2;
                if (effect->side == 0)
                  Func_08015160((mouthX + Data_08199dbb[mode * 4 + i] - 48)
                                    << 16,
                                (mouthY - 64) << 16, 0);
                else
                  Func_08015160((mouthX - Data_08199dbb[mode * 4 + i] - 80)
                                    << 16,
                                (mouthY - 64) << 16, 0);
              } else {
                if (effect->side == 0)
                  Func_08015160((mouthX + Data_08199dbb[mode * 4 + i] - 64)
                                    << 16,
                                (mouthY - 64) << 16, 0);
                else
                  Func_08015160((mouthX - Data_08199dbb[mode * 4 + i] - 64)
                                    << 16,
                                (mouthY - 64) << 16, 0);
              }
              Func_080151e4(scale, scale, scale);
              SceneTransform_ApplyYaw(0x36b0);
              SceneTransform_ApplyPitch(0x4000);
              SceneTransform_ApplyYaw(frame << 10);
              Func_08196958(Data_08198cac, vertices, 32);
              Func_08196a7c(model);
            }
          }
        }
      }

      // Blast Breath's jet, sweeping round from frame 8 and fading from
      // frame 32.
      if (mode == BREATH_BLAST) {
        s32 angle = 0x3060;
        s32 alpha = 0;

        if (frame > 31)
          alpha = (32 - frame) * 4;
        model->alpha = alpha;
        texture.logWidth = 5;
        texture.logHeight = 2;
        model->kind = 7;
        model->mesh = Data_08199220;
        texture.pixels = work->sheet;
        Render_ResetTransformState();
        Func_08015160((mouthX - 64) << 16, (mouthY - 64) << 16, 0);
        Func_080151e4(0x8000, 0x10000, 0x10000);
        if (frame > 7)
          angle = (frame << 7) + 0x2c60;
        if (effect->side == 1)
          angle = -angle;
        Func_080150e4(angle);
        Func_080151e4(0x40000, 0x20000, 0x10000);
        Func_08196958(Data_081991c0, vertices, 4);
        Func_08196a7c(model);
      }
      MemFree(model);
      MemFree(vertices);
    }

    if (mode == BREATH_BEAM) {
      if (frame == 0)
        Audio_PlayCue(140);
    } else if (mode == BREATH_PIKE) {
      if (frame == 2)
        Audio_PlayCue(212);
    }

    if (mode == BREATH_BLAST) {
      // Blast Breath strikes each target in turn, every fourth frame.
      if (frame == 0) {
        work->shakeFrames = 8;
        Audio_PlayCue(221);
      }
      if (frame == 16) {
        Audio_PlayCue(134);
        Func_081180e8(134);
      }
      for (i = 0; i != effect->count; i++) {
        if (frame >= Data_08199d9b[mode] + i * 4 &&
            frame < Data_08199d97[mode] - 8 && (frame & 3) == i) {
          work->shakeFrames = 8;
          if (mode == 6)
            Func_0814cd48(effect->actors[i], 14, 5, i, 3);
          else
            Func_0814cd48(effect->actors[i], 7, 5, i, 3);
          BattleFx_StartActorMotion(effect->actors[i], 1, 0x10000, 0x40000,
                                    0x20000, frame + 100);
        }
      }
    } else if (frame == Data_08199dc7[mode]) {
      // The others hit every target at once, and the screen flashes.
      for (i = 0; i != effect->count; i++) {
        Func_08118088(effect->actors[i], 4);
        Func_0814cd48(effect->actors[i], 7, 5, i, 16);
      }
      work->shakeFrames = 8;
      Func_081180e8(144);
      Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
    }
    Func_08158ce0(8, 8);
    Func_081434f8();
    work->transferPending = 1;
    WaitFrames(1);
  }

  Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
  Scheduler_RemoveCallback(Func_08143000);
  Func_08143bb8();
}

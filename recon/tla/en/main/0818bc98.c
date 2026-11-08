// Differ 2 of 1251 (tla-en, 2026-11-07).
// 2026-11-07: the texture's pixels as u8 * (alias set apart from the model's
// vertices) put mov r8 ahead of the vertices store (4 to 2). Left: mov sl
// after asrs r6 at the flash loop's preheader; gcse's PRE inserts mouthY - 64
// (expression 262) before scale >> 31 (272), and reload's copy to sl then wins
// sched2's LUID tie. A fix needs scale's sign hashed before the flash loop.
// Draft, differ 32 of 1251, skeleton gap 14 (tla-en, 2026-11-01)
// All that is left is order, with the same instructions and registers. In every
// site a store to the stack comes earlier in the
// native than sched2 puts it here: the frame's store before the mode's reload
// at the loop's entry and foot; the outgoing arguments' strs before the reload
// of the blitter address (sp+20) at both glows' draws and both sparks' draws;
// the spilled x-64 (sp+8) ahead of y-64 before the flash loop; str to sp+12
// ahead of mov r8 after the model's allocation. At the Beam's glow the sched2 dump shows the moved
// instructions tied at priority 5 and broken by last-scheduled and LUID. The
// reading that the native costs a stack store higher is an inference, the
// twin notes' "spill store under-costed" family, checked in detail at that one
// site only. If it holds, it is a question about the compiler's model (K2) and
// out of scope for a draft.
// Tried without gain on these sites: top and height computed first at the glow
// (64), height alone (66), left alone (32, no change); left and top as
// loop-body locals (177) or set before the flash loop (142 to 185); kind stored
// before the texture's size (40). No device was added: the glow's interleaving
// of the x and y chains looks out of reach of one, and a device that stops
// short of differ 0 comes back out; none was measured.
// What closed the rest, measured in order from 129:
// - The mouth's x and y live in a block after the frame-0 setup, x first: the
//   address of user that loop.c hoists to sp+36 is a pseudo made at the frame-0
//   code, so the mouth's pseudos must come after it (107).
// - The beam reads user.y before the ring's clamp and takes 48 off at each use,
//   hoisted after the clamp (91).
// - Frame 0's stores in the order x, y, velocities, z, velocityZ, variant (85).
// - i * 4 first in Blast Breath's range test (44).
// - The jet's alpha declared before its angle (42).
// - 10192 as a local set before the age: reload then cannot use r3 for frame,
//   and the rotation matches (36).
// - The palette as a local and the copy routine as a local after it, so its
//   address is loaded before the destination constant (32; the routine local
//   alone: 40).
// No close ☀️ twin (call order BattleEffect_RunDragonCloud 0.417): ⚓️'s own
// effect on ☀️'s frame, its bursts and jaws drawn as textured quads by ⚓️'s
// model kernels, as in Triple Chomp (BattleEffect_RunTripleChomp).
// BattleEffect_LoadBlitter returns s32, as nibble_pair.c defines it. The native
// stack frame (108): the mode at sp+64, work 60, canvas 56, frame 52, graphics
// 48, the user's position's address 44 (and a copy at 36), the target's 40, the
// mouth's x 32 and y 28, the sheet's glow cell 24, the blitter pair's address
// 20, the vertices 16, the texture's address 12 and x-64 at 8; the texture is
// at sp+68, the blitter pair at 76, the target at 84 and the user at 96. effect
// stays in r11. The range tests are grouped as their own operand of the &&, so
// they fold into the native's unsigned compares; the vertex pool is 1024 bytes.
#include "types.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "battle_effect.h"
#include "battle_effect_work.h"

// The Lost Age's heap slots, by byte offset into Ram_HeapSlots.
#define HEAP_SLOT_BLITTER 104
#define HEAP_SLOT_BLITTER_ALTERNATE 188

// The transfer task runs at this priority, where ☀️'s runs at 0x480.
#define TASK_PASS_FRAME 0xc80

// ⚓️'s blitter modes count the other way from ☀️'s COMPOSITE_* modes.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT, BLIT_COPY };

// BattleEffect_LoadBlitter's packed argument: the mode in the high nibble.
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
  u8 *pixels;
};

// One entry of a model list, as Render_AllocateModelList allocates them and
// Render_DrawModelList draws them.
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

void BattleEffect_StartCanvasLayer(s32 bgControl);
void *Resource_GetTableEntry(s32 id);
s32 BattleEffect_LoadBlitter(s32 slot, s32 packed);
s32 Task_Add(void (*func)(void), s32 order);
s32 Task_Remove(void (*func)(void));
void BattleEffect_SetApproachMotion(s32 actor, s32 target, s32 frames,
                                    s32 percent);
void EffectPosition_ProjectBattler(s32 id, struct EffectPosition *position);
void Func_0815b510(u8 *map, s32 size, s32 scale);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                          s32 gravity);
void *Heap_AllocEwram(s32 size);
struct Model *Render_AllocateModelList(s32 count);
void Render_ResetTransformState(void);
void Func_080151e4(s32 x, s32 y, s32 z);
void Func_08015160(s32 x, s32 y, s32 z);
void SceneTransform_ApplyPitch(s32 angle);
void SceneTransform_ApplyYaw(s32 angle);
void Func_080150e4(s32 angle);
void Render_TransformVertices(void *corners, void *vertices, s32 count);
void Render_DrawModelList(struct Model *model);
void Heap_Release(void *block);
void Sound_PlayFar(s32 cue);
void Func_081180e8(s32 phase);
void Func_0814cd48(s32 actor, s32 palette, s32 group, s32 timerSlot,
                   s32 frames);
void BattleEffect_StartActorMotion(s32, s32, s32, s32, s32, s32);
void Func_08118088(s32 actor, s32 motion);
void Func_08158ce0(s32 randomMask, u32 shakeRange);
void BattleEffect_CountDownBattlerTints(void);
void wait(s32 frames);
void HeapSlot_Release(s32 slot);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_ProcessPendingGraphicsTransfer(void);

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
    BattleEffect_StartCanvasLayer(0);
  else
    BattleEffect_StartCanvasLayer(1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, graphics, 0, 0);
  if (mode == BREATH_PIKE) {
    void *palette = Resource_GetTableEntry((s32)&ResourceId_BlueFlameSheet);
    s32 (*copyWords)(void *, const void *, s32) = Iwram_CopyWordsEntry;

    copyWords((void *)BG_PLTT, palette, 128);
  } else {
    void *palette = Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB);
    s32 (*copyWords)(void *, const void *, s32) = Iwram_CopyWordsEntry;

    copyWords((void *)BG_PLTT, palette, 128);
  }
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
    BattleEffect_LoadBlitter(
        HEAP_SLOT_BLITTER,
        BLITTER(COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X,
                BLIT_MAX));
  else
    BattleEffect_LoadBlitter(
        HEAP_SLOT_BLITTER,
        BLITTER(COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y, BLIT_MAX));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  BattleEffect_LoadBlitter(
      HEAP_SLOT_BLITTER_ALTERNATE,
      BLITTER(COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y, BLIT_ADDITIVE));
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  // The pikes lunge; Blast Breath comes from 32 pixels in front of the user.
  if (mode == BREATH_PIKE)
    BattleEffect_SetApproachMotion(effect->actor, effect->actors[0], 10, 80);
  EffectPosition_ProjectBattler(effect->actor, &user);
  if (mode == BREATH_BLAST) {
    if (effect->side == 0)
      user.x -= 32;
    else
      user.x += 32;
    user.y -= 8;
  }
  EffectPosition_ProjectBattler(effect->actors[0], &target);

  for (frame = 0; frame != Data_08199d97[mode]; frame++) {
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
      step->y = (user.y - 32) << 16;
      step->velocityX = 0;
      step->velocityY = 0;
      step->z = (target.x / 2 - user.x / 2) << 10;
      step->velocityZ = (target.y - user.y) << 10;
      step->variant = 0;
    }
    {
      s32 mouthX;
      s32 mouthY;

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
          draw[1](canvas, work->sheet + 0x4000, mouthX - size / 2,
                  mouthY - size, size, size * 2);
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
        draw[1](canvas, work->sheet + 0x4000, mouthX - (size >> 1),
                mouthY - size, size, half * 4);
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
            BattleEffectKernels_IntegrateVector2(step, 60, 0);
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
          s32 y = user.y;
          s32 ring = frame - 72;

          if (ring < 0)
            ring = 0;
          if (ring > 15)
            ring = 15;
          for (i = 0; i != 4; i++) {
            if (effect->side == 0) {
              draw[0](canvas, work->sheet + (ring << 10), x - 40, y - 48, 32,
                      32);
              x -= 32;
            } else {
              draw[0](canvas, work->sheet + (ring << 10), x + 8, y - 48, 32,
                      32);
              x += 32;
            }
          }
        }
      } else if (mode == BREATH_ROLLING_FLAME && frame >= Data_08199d9b[mode]) {
        // The ball of dust rolls to the target.
        step = &work->particles[0];
        draw[0](canvas, work->sheet, HI(step->x) - 16, HI(step->y) - 24, 32,
                48);
        BattleEffectKernels_IntegrateVector2(step, 56, 0);
        step->velocityX += step->z;
        step->velocityY += step->velocityZ;
        step->variant++;
      }

      if (frame >= Data_08199d9b[mode]) {
        void *vertices;
        struct Model *model;

        vertices = Heap_AllocEwram(1024);
        model = Render_AllocateModelList(1);
        texture.logWidth = 7;
        texture.logHeight = 7;
        model->kind = 6;
        model->texture = &texture;
        model->mesh = gData_08199364;
        model->vertices = vertices;
        texture.pixels = Ram_MapCellBuffer;

        // The flash at the mouth grows and fades over 16 frames; Blast
        // Breath's is two, crossed.
        if (mode != BREATH_ROLLING_FLAME) {
          s32 growth = 10192;
          s32 age = frame - Data_08199d9b[mode];
          s32 scale = age * growth + 0x8000;
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
              Render_TransformVertices(gData_08199210, vertices, 4);
              Render_DrawModelList(model);
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
                Render_TransformVertices(Data_08198cac, vertices, 32);
                Render_DrawModelList(model);
              }
            }
          }
        }

        // Blast Breath's jet, sweeping round from frame 8 and fading from
        // frame 32.
        if (mode == BREATH_BLAST) {
          s32 alpha = 0;
          s32 angle = 0x3060;

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
          Render_TransformVertices(Data_081991c0, vertices, 4);
          Render_DrawModelList(model);
        }
        Heap_Release(model);
        Heap_Release(vertices);
      }

      if (mode == BREATH_BEAM) {
        if (frame == 0)
          Sound_PlayFar(140);
      } else if (mode == BREATH_PIKE) {
        if (frame == 2)
          Sound_PlayFar(212);
      }

      if (mode == BREATH_BLAST) {
        // Blast Breath strikes each target in turn, every fourth frame.
        if (frame == 0) {
          work->shakeFrames = 8;
          Sound_PlayFar(221);
        }
        if (frame == 16) {
          Sound_PlayFar(134);
          Func_081180e8(134);
        }
        for (i = 0; i != effect->count; i++) {
          if (frame >= i * 4 + Data_08199d9b[mode] &&
              frame < Data_08199d97[mode] - 8 && (frame & 3) == i) {
            work->shakeFrames = 8;
            if (mode == 6)
              Func_0814cd48(effect->actors[i], 14, 5, i, 3);
            else
              Func_0814cd48(effect->actors[i], 7, 5, i, 3);
            BattleEffect_StartActorMotion(effect->actors[i], 1, 0x10000,
                                          0x40000, 0x20000, frame + 100);
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
      BattleEffect_CountDownBattlerTints();
      work->transferPending = 1;
      wait(1);
    }
  }

  HeapSlot_Release(HEAP_SLOT_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_SLOT_BLITTER);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

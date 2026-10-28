// Draft: listing 760 instructions, draft 760, differ 510. No close ☀️ twin (call
// order BattleEffect_RunRamses 0.455): ⚓️'s own effect on ☀️'s
// frame, with its rune and swords drawn as textured quads by ⚓️'s model
// kernels, as in Triple Chomp (BattleEffect_RunTripleChomp).
// The native stack frame (88) holds effect at sp+52, work 48, canvas 44, frame 40,
// the blitter 32, graphics 28, the start's address 24, the vertices 20, the
// texture's address 16, the sword's delay (16 * sword) at 12 and its sparks'
// offset at 8; the texture is at sp+56, the target's screen position at 64
// and the start at 76.
// Written from the listing (2026-10-15, fx-n3): 560 at the first compile,
// with the instruction count already the native's. The blitter as a pair
// (its second word fills sp+36) and the aggregates declared start, target,
// texture (the first declared takes the highest slot): 510.
// Left, first: the native reloads the map cell buffer's address from the
// pool for each of the prologue's three calls, where this draft keeps it in
// r5 across them (as Ram_MapCellBuffer or as a linked gMapCellBuffer[]: 510
// both), and every slot and register after that follows. The fan's texture
// is written as two byte stores natively (5, then 6, the texture's address
// stored again between them), not this draft's bitfield update.
#include "types.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "battle_effect.h"
#include "battle_effect_work.h"

// The Lost Age's heap slots, by byte offset into Ram_HeapSlots.
#define HEAP_SLOT_BLITTER 104

// The transfer callback runs at this order, where ☀️'s runs at 0x480.
#define TASK_PASS_FRAME 0xc80

// ⚓️'s blitter modes count the other way from ☀️'s COMPOSITE_* modes.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT, BLIT_COPY };

// BattleEffect_LoadBlitter's packed argument: the mode in the high nibble.
#define BLITTER(flags, mode) ((mode) << 4 | (flags))

#define HI(v) (((s16 *)&(v))[1])

// The sparks live in the map cell buffer's second 16 KB, 128 for each sword.
#define SPARKS ((struct EffectStep *)(Ram_MapCellBuffer + 0x4000))
#define SPARK_COUNT 512

// A texture for the model kernels: its size as powers of two, then its
// pixels.
struct ModelTexture {
  u32 logWidth : 8;
  u32 logHeight : 8;
  u32 unk02 : 16;
  void *pixels;
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

extern char ResourceId_CornerRuneASheet;
extern char ResourceId_Data134;
extern char ResourceId_BladeSheet;
extern char ResourceId_Data0E9;
extern char ResourceId_Data0C2;

// The quad mesh and the corners of the rune, the jaw-like flash and the
// eight blades of the opening fan.
extern u8 gData_08199364[];
extern u8 Data_081992b0[];
extern s32 Data_081991e0[];
extern s32 Data_08199200[];

// The spark cells: offsets into the particle sheet by size.
extern u16 gData_08197410[];

void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_LoadBlitter(s32 slot, s32 packed);
void Func_0816ae40(u8 *src, u8 *dst, s32 width, s32 height);
s32 Task_Add(void (*callback)(void), s32 order);
s32 Task_Remove(void (*callback)(void));
void EffectPosition_ProjectBattler(
    s32 id, struct EffectPosition *position);
void Sound_PlayFar(s32 cue);
void Func_081180e8(s32 phase);
void BattleEffect_StartActorMotion(s32, s32, s32, s32, s32, s32);
void Func_0814cd48(s32 actor, s32 palette, s32 group, s32 timerSlot,
                   s32 frames);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                      s32 gravity);
void *Runtime_BumpAllocateAlternatePool(s32 size);
struct Model *Render_AllocateModelList(s32 count);
void Render_ResetTransformState(void);
void Func_080151e4(s32 x, s32 y, s32 z);
void Func_08015160(s32 x, s32 y, s32 z);
void SceneTransform_ApplyPitch(s32 angle);
void SceneTransform_ApplyYaw(s32 angle);
void Func_080150e4(s32 angle);
void Func_0801521c(s32 scale);
void Render_TransformVertices(void *corners, void *vertices, s32 count);
void Render_DrawModelList(struct Model *model);
void MemFree(void *block);
void Func_08158ce0(s32 randomMask, u32 shakeRange);
void ObjectGroup_CountDownMemberTimers(void);
void WaitFrames(s32 frames);
void Runtime_ReleaseHeapBlock(s32 slot);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_ProcessPendingGraphicsTransfer(void);

// Legend (effect kind 122): a fan of eight blades opens over the target for
// 50 frames while a rune, mirrored from one corner, rises and turns behind
// it. Then one sword for each step of the variant (variant + 1 in all, 16
// frames apart) falls on the target from above the user's side: it strikes
// at 59 frames plus its delay, throwing 128 sparks that bounce on the ground,
// while a flash grows and fades where it lands. The effect lasts 86 frames
// and 16 more for each further sword.
void BattleEffect_RunLegend(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  DrawRectangle draw[2];
  u8 *graphics;
  struct EffectPosition start;
  struct EffectPosition target;
  struct ModelTexture texture;
  struct EffectStep *step;
  struct Model *model;
  void *vertices;
  s32 i;
  s32 j;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  graphics = Ram_HeapSlots->unk64[0];

  // Set up the canvas and a clipped MAX blitter, mirrored to face the
  // target; mirror the rune's corner into the sheet; load the sparks, the
  // sword and the flash.
  BattleEffect_StartCanvasLayer(1);
  REG_BLDALPHA = 0x1010;
  if (effect->side == 0)
    BattleEffect_LoadBlitter(HEAP_SLOT_BLITTER,
                  BLITTER(COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y, BLIT_ADDITIVE));
  else
    BattleEffect_LoadBlitter(
        HEAP_SLOT_BLITTER,
        BLITTER(COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X,
                BLIT_ADDITIVE));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  Resource_LoadAndDecompress((s32)&ResourceId_CornerRuneASheet,
                             Ram_MapCellBuffer, 1, 1);
  Func_0816ae40(Ram_MapCellBuffer, work->sheet, 64, 64);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, graphics, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_BladeSheet, work->sheet + 0x4000,
                             1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0E9, work->sheet + 0x5000, 0,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C2, Ram_MapCellBuffer, 0, 0);
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  for (frame = 0; frame != effect->variant * 16 + 86; frame++) {
    // Frame 0: the swords set out from 80 pixels above and beside the
    // target, on the user's side, toward it.
    if (frame == 0) {
      EffectPosition_ProjectBattler(effect->actors[0], &target);
      if (effect->side == 0)
        start.x = target.x + 80;
      else
        start.x = target.x - 80;
      start.y = target.y - 80;
      for (i = 0; i != 3; i++) {
        step = &work->particles[i];
        step->x = start.x << 15;
        step->y = start.y << 16;
        step->velocityX = (target.x - start.x) << 11;
        step->velocityY = (target.y - start.y) << 12;
        step->variant = 0;
      }
      for (i = 0; i != SPARK_COUNT; i++)
        SPARKS[i].variant = -1;
    }

    // The swords, 16 frames apart.
    for (i = 0; i != effect->variant + 1; i++) {
      step = &work->particles[i];
      if (frame == i * 16 + 54)
        Sound_PlayFar(212);

      // The strike: the target reels and flashes, and 128 sparks fly up
      // from it. The last sword ends the action.
      if (frame == i * 16 + 59) {
        if (i == effect->variant)
          Func_081180e8(144);
        else
          Sound_PlayFar(144);
        BattleEffect_StartActorMotion(effect->actors[0], 1, 0, 0, 0, 100);
        Func_0814cd48(effect->actors[0], 7, 5, 0, 8);
        work->shakeFrames = 4;
        for (j = 0; j != 128; j++) {
          struct EffectStep *spark = &SPARKS[i * 128 + j];
          s32 angle;
          s32 force;

          spark->x = target.x << 15;
          spark->y = 96 << 16;
          angle = Random16() % 0xc000 + 0x2000;
          force = Random16() & 127;
          spark->velocityX = (Trig_Sin(angle) * force) >> 4;
          spark->velocityY = (Trig_Cos(angle) * force) >> 3;
          spark->variant = (Random16() & 7) + 16;
        }
      }

      // From frame 50 plus its delay the sword falls, faster each frame,
      // until it sticks in the ground 12 frames later.
      if (frame >= i * 16 + 50) {
        s32 x = HI(step->x) - 20;
        s32 y = HI(step->y) - 50;
        s32 height = 80;

        if (y > 24)
          height = 104 - y;
        draw[0](canvas, work->sheet + 0x4000, x, y, 40, height);
        if (step->variant <= 12) {
          step->x += step->velocityX;
          step->y += step->velocityY;
          step->velocityX += (target.x - start.x) << 9;
          step->velocityY += (target.y - start.y) << 10;
        }
        step->variant++;
      }
    }

    // The sparks bounce once on the ground at y = 104.
    for (i = 0; i != SPARK_COUNT; i++) {
      step = &SPARKS[i];
      if (step->variant >= 0) {
        s32 size = (step->variant >> 2) + 1;

        draw[0](canvas, graphics + gData_08197410[size - 1],
                HI(step->x) - size / 2, HI(step->y) - size, size, size * 2);
        if (step->y > 104 << 16) {
          step->y = 104 << 16;
          step->velocityY = -step->velocityY / 2;
        }
        BattleEffectKernels_IntegrateVector2(step, 62, 0x4000);
        step->variant--;
      }
    }

    // This frame's models: one quad, retextured for each part.
    vertices = Runtime_BumpAllocateAlternatePool(32);
    model = Render_AllocateModelList(1);
    texture.logWidth = 7;
    texture.logHeight = 7;
    texture.pixels = work->sheet;
    model->mesh = gData_08199364;
    model->kind = 7;
    model->texture = &texture;
    model->vertices = vertices;

    // Frames 16-107: the rune rises behind the target, turning, then fades.
    if (frame >= 16 && frame <= 107) {
      s32 height = frame * 2 - 96;
      s32 scale = (frame << 13) - 0x20000;
      s32 alpha = 0;

      if (height > -48)
        height = -48;
      if (frame >= effect->variant * 16 + 60)
        alpha = -((frame - effect->variant * 16) * 4 - 240);
      if (scale > 0x20000)
        scale = 0x20000;
      model->alpha = alpha;
      Render_ResetTransformState();
      Func_080151e4(0x8000, 0x10000, 0x8000);
      Func_08015160((start.x - 128) << 16, height << 16, 0);
      SceneTransform_ApplyPitch(0x5900);
      if (effect->side == 0)
        SceneTransform_ApplyYaw(0x1900);
      else
        SceneTransform_ApplyYaw(-0x1900);
      Func_080150e4(frame << 9);
      Func_0801521c(scale);
      Func_080151e4(0x10000, 0x10000, 0x10000);
      Render_TransformVertices(Data_081991e0, vertices, 4);
      Render_DrawModelList(model);
    }

    // Each sword's flash grows where it lands and fades over 16 frames.
    texture.pixels = Ram_MapCellBuffer;
    for (i = 0; i != effect->variant + 1; i++) {
      if (frame >= i * 16 + 50) {
        s32 age = frame - i * 16 - 50;
        s32 alpha = 64 - age * 8;

        if (alpha > 0)
          alpha = 0;
        if (alpha > -64) {
          s32 scale = age * 12384 + 0x8000;

          model->alpha = alpha;
          Render_ResetTransformState();
          Func_080151e4(0x8000, 0x10000, 0x8000);
          Func_08015160((start.x - 128) << 16, -48 << 16, 0);
          SceneTransform_ApplyPitch(0x5900);
          if (effect->side == 0)
            SceneTransform_ApplyYaw(0x1900);
          else
            SceneTransform_ApplyYaw(-0x1900);
          Func_080150e4(frame << 9);
          Func_0801521c(scale);
          Render_TransformVertices(Data_081991e0, vertices, 4);
          Render_DrawModelList(model);
        }
      }
    }

    // Frames 0-49: the fan of eight blades opens, pulsing, and closes.
    if (frame <= 49) {
      texture.logWidth = 5;
      texture.logHeight = 6;
      texture.pixels = work->sheet + 0x5000;
      model->mesh = Data_081992b0;
      for (i = 0; i != 8; i++) {
        s32 length;

        model->alpha = 0;
        Render_ResetTransformState();
        Func_080151e4(0x8000, 0x10000, 0x8000);
        Func_08015160((start.x - 128) << 16, -80 << 16, 0);
        SceneTransform_ApplyPitch(0x4750);
        Func_080150e4((frame << 9) + i * 0x2000);
        length = (frame << 12) - 0x4000;
        if (length > 0x10000)
          length = 0x10000;
        if (frame > 31)
          length = 0x30000 - (frame << 12);
        Func_080151e4(0x8000, 0x10000,
                      (Trig_Sin((frame << 9) + i * 0x1000) / 8 + length) * 2);
        Render_TransformVertices(Data_08199200, vertices, 4);
        Render_DrawModelList(model);
      }
    }
    MemFree(model);
    MemFree(vertices);
    Func_08158ce0(4, 4);
    ObjectGroup_CountDownMemberTimers();
    work->transferPending = 1;
    WaitFrames(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
  BattleEffect_EndCanvasLayer();
}

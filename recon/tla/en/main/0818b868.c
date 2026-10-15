/* Near miss: listing 434 instructions, draft 433, differ 151. No ☀️ twin: the
 * frame and the spark blitting are ☀️'s (BattleFx_RunVenusDjinnColumns, shape
 * 0.572), the bite drawn as a textured quad through ⚓️'s model kernels is ⚓️'s
 * own. The native frame (84) holds effect at sp+60, work 56, canvas 52, frame
 * 48, the blitter pair at 40 and 44 (the second word unused), graphics 36, the
 * vertices 32, the model 28, the texture's and the position's addresses at 24
 * and 20, the speed (120 + 20 * bite) at 16 and the bite's particle offset at
 * 12; the texture is at sp+64 and the position at sp+72.
 * Earlier: differ 411 (draft 425; the alignment drifts from the first spill).
 * A two-word blitter array (one DImode pseudo, as in Func_08182898 and
 * Func_0818c7b8): 463 to 411.
 * 2026-10-15 (fx-n3), rewritten for reading and measured, in order: the
 * sparks at the fixed map cell buffer + 0x4000 rather than an EWRAM label
 * (the native folds the variant's offset into one pooled address): 336; the
 * burst loop with its own counter: 332; declarations in the native's spill
 * order (work, canvas, frame, the blitter pair, graphics, vertices, model)
 * and the hit's speed as i * 20 + 120: 320; one counter for the clear, bite
 * and draw loops, as the native keeps them in r9 (the frame then is 84): 284;
 * the tables extern without const (the native reloads them after calls):
 * 165; the anchor as (&work->particles[i])->x, written BITE_ANCHOR (an
 * alchemy permute find; work->particles[i].x is 396): 151.
 * No effect: the order of the three aggregate locals (all six, 332); the
 * scalar declarations' order (spark, i, j, model, 24 orders, 284).
 * Worse: each spark as &SPARKS[i * 128 + j] in the burst loop: 387.
 * Left: the model's allocation (the native spills it and reloads it in r0,
 * so its kind is a fresh 7, not the bitfield's), the burst loop's spark and
 * angle registers swapped (r7 and r6 natively), one draw-loop instruction
 * short, and the TLA load-before-constant pairs. alchemy permute (10 min, 2
 * jobs): 151 as above. */
#include "types.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "battle_effect.h"
#include "battle_effect_work.h"

// The Lost Age's heap slots, by byte offset into Ram_HeapSlots.
#define HEAP_SLOT_BLITTER 104

// The transfer callback runs at this order, where ☀️'s runs at 0x480.
#define TRANSFER_CALLBACK_ORDER 0xc80

// ⚓️'s blitter modes count the other way from ☀️'s COMPOSITE_* modes.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT, BLIT_COPY };

// Func_081963ec's packed argument: the mode in the high nibble.
#define BLITTER(flags, mode) ((mode) << 4 | (flags))

// The sparks live in the map cell buffer's second 16 KB.
#define SPARKS ((struct EffectStep *)(Ram_MapCellBuffer + 0x4000))
#define SPARK_COUNT 512
#define HI(v) (((s16 *)&(v))[1])

// Each bite's anchor, where its jaw is drawn: the first three particles.
#define BITE_ANCHOR(bite) (&work->particles[bite])

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
  const void *mesh;
  void *vertices;
  struct ModelTexture *texture;
  s32 alpha;
  s32 tail;
};

extern char ResourceId_Data0C2;
extern char ResourceId_Data134;
extern char ResourceId_BlueFlameSheet;

// The bite's quad: its mesh and its four corners, 192 x 128 around the
// origin.
extern u8 gData_08199364[];
extern s32 gData_08199210[];

// The three bites' frames: 0, 12 and 24.
extern u8 gData_08199d94[];

// The spark cells: offsets into the particle sheet by size.
extern u16 gData_08197410[];

void BattleFx_BeginCanvasLayer(s32 bgControl);
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 order);
s32 Scheduler_RemoveCallback(void (*callback)(void));
void Func_08157530(s32 actor, s32 target, s32 frames, s32 percent);
void WaitFrames(s32 frames);
void Func_081963ec(s32 slot, s32 packed);
void *Resource_GetTableEntry(s32 id);
void *Runtime_BumpAllocateAlternatePool(s32 size);
struct Model *Func_081969f8(s32 count);
void EffectPosition_ApplyAlternateStepAndYOffset(
    s32 id, struct EffectPosition *position);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);
void Func_081180e8(s32 phase);
void Audio_PlayCue(s32 cue);
void BattleFx_StartActorMotion(s32, s32, s32, s32, s32, s32);
void Func_0814cd48(s32 actor, s32 palette, s32 group, s32 timerSlot,
                   s32 frames);
void Render_ResetTransformState(void);
void Func_080151e4(s32 x, s32 y, s32 z);
void Func_08015160(s32 x, s32 y, s32 z);
void Func_080150e4(s32 angle);
void SceneTransform_ApplyPitch(s32 angle);
void Func_0801521c(s32 scale);
void Func_08196958(void *corners, void *vertices, s32 count);
void Func_08196a7c(struct Model *model);
void BattleFxKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                      s32 gravity);
void MemFree(void *block);
void Func_08158ce0(s32 randomMask, u32 shakeRange);
void Func_081434f8(void);
void Runtime_ReleaseHeapBlock(s32 slot);
void Func_08143bb8(void);
void Func_08143000(void);

// Triple Chomp (effect kind 426): the caster lunges 80% of the way to the
// target, and three bites close on it at frames 0, 12 and 24. Each bite
// throws 32 blue sparks that fly off away from the caster, jolts and flashes
// the target, and draws a jaw: a textured quad tilted back, turned a quarter
// further each bite, that grows and fades over 16 frames.
void Func_0818b868(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  DrawRectangle draw[2];
  u8 *graphics;
  void *vertices;
  struct Model *model;
  struct ModelTexture texture;
  struct EffectPosition position;
  struct EffectStep *spark;
  s32 i;
  s32 j;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  graphics = Ram_HeapSlots->unk64[0];

  // Set up the canvas, then let the caster lunge.
  BattleFx_BeginCanvasLayer(1);
  REG_BLDALPHA = 0x1010;
  work->transferMode = 2;
  work->transferValue = 50;
  Scheduler_AddOrUpdateCallback(Func_08143000, TRANSFER_CALLBACK_ORDER);
  Func_08157530(effect->actor, effect->actors[0], 8, 80);
  WaitFrames(8);

  // A clipped, mirrored MAX blitter for the sparks; the jaw's texture, the
  // spark sheet and the blue flame's colours.
  Func_081963ec(HEAP_SLOT_BLITTER,
                BLITTER(COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X,
                        BLIT_MAX));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C2, Ram_MapCellBuffer, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, graphics, 0, 0);
  Iwram_CopyWords((void *)0x05000000,
                  Resource_GetTableEntry((s32)&ResourceId_BlueFlameSheet), 128);

  for (frame = 0; frame != 50; frame++) {
    if (frame == 0) {
      for (i = 0; i != SPARK_COUNT; i++)
        SPARKS[i].variant = -1;
    }

    // This frame's jaw: one model drawing the quad with the 128 x 128
    // texture.
    vertices = Runtime_BumpAllocateAlternatePool(32);
    model = Func_081969f8(1);
    EffectPosition_ApplyAlternateStepAndYOffset(effect->actors[0], &position);
    texture.logWidth = 7;
    texture.logHeight = 7;
    texture.pixels = Ram_MapCellBuffer;
    model->mesh = gData_08199364;
    model->kind = 7;
    model->texture = &texture;
    model->vertices = vertices;

    // The three bites; the native counts them in the same register as the
    // spark loops, as one variable.
    for (i = 0; i != 3; i++) {
      // The bite closes: 32 sparks fly from the target's middle, and the
      // target jolts and flashes. The last bite ends the action.
      if (frame == gData_08199d94[i]) {
        spark = &SPARKS[i * 128];
        for (j = 0; j != 32; j++) {
          s32 angle;
          s32 force;

          spark->x = (position.x / 2) << 16;
          spark->y = (position.y - 24) << 16;
          angle = Random16();
          force = (Random16() & 255) + 32;
          spark->velocityX = (Trig_Sin(angle) * force) >> 8;
          spark->velocityY = (Trig_Cos(angle) * force) >> 6;
          if (effect->side == 0)
            spark->velocityX += -0x8000;
          else
            spark->velocityX += 0x8000;
          BITE_ANCHOR(i)->x = position.x;
          BITE_ANCHOR(i)->y = position.y - 24;
          spark->variant = (Random16() & 7) + 16;
          spark++;
        }
        if (i == 2)
          Func_081180e8(134);
        else
          Audio_PlayCue(134);
        BattleFx_StartActorMotion(effect->actors[0], 1, 0x20000, 0x80000, 0,
                                  i * 20 + 120);
        Func_0814cd48(effect->actors[0], 7, 5, 0, 8);
        work->shakeFrames = 8;
      }

      // For 16 frames the jaw grows from a quarter of its size and fades.
      if (frame >= gData_08199d94[i] && frame < gData_08199d94[i] + 16) {
        s32 age = frame - gData_08199d94[i];
        s32 alpha = 64 - age * 8;
        s32 scale = age * 10192 + 0x4000;

        if (alpha > 0)
          alpha = 0;
        if (alpha > -64) {
          model->alpha = alpha;
          Render_ResetTransformState();
          Func_080151e4(0x8000, 0x10000, 0x8000);
          Func_08015160((BITE_ANCHOR(i)->x - 128) << 16,
                        (BITE_ANCHOR(i)->y - 64) << 16, 0);
          Func_080150e4((i << 14) + 0x2000);
          SceneTransform_ApplyPitch(-0x1000);
          Func_0801521c(scale);
          Func_08196958(gData_08199210, vertices, 4);
          Func_08196a7c(model);
        }
      }
    }

    // Each spark is drawn and moved twice a frame, shrinking as it dies.
    spark = SPARKS;
    for (i = 0; i != SPARK_COUNT; i++) {
      s32 size = spark->variant;

      if (size >= 0) {
        size = (size >> 2) + 3;
        draw[0](canvas, graphics + gData_08197410[size - 1],
                HI(spark->x) - size / 2, HI(spark->y) - size, size, size * 2);
        BattleFxKernels_IntegrateVector2(spark, 62, 0);
        draw[0](canvas, graphics + gData_08197410[size - 1],
                HI(spark->x) - size / 2, HI(spark->y) - size, size, size * 2);
        BattleFxKernels_IntegrateVector2(spark, 62, 0);
        spark->variant--;
      }
      spark++;
    }

    MemFree(model);
    MemFree(vertices);
    Func_08158ce0(8, 8);
    Func_081434f8();
    work->transferPending = 1;
    WaitFrames(1);
  }

  Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
  Scheduler_RemoveCallback(Func_08143000);
  Func_08143bb8();
}

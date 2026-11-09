#include "battle_effect.h"
#include "battle_effect_work.h"
#include "effect_step.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "ram_buffer.h"
#include "task.h"
#include "types.h"

#define HEAP_SLOT_BLITTER 104 // Ram_HeapSlots->unk64[1]

// The Lost Age's blitter modes run the other way from The Broken Seal's:
// additive 0, max 1, transparent 2, copy 3; 1 << 4 | 3 is max and clipped,
// and | 4 mirrors it.
#define BLITTER_MAX 1
#define BLITTER(mode, flags) ((mode) << 4 | (flags))
#define CLIP_XY 3
#define FLIP_X 4

#define SWING_FRAMES 52
#define SWING_HIT_FRAME 18
#define SPARK_COUNT 256

// The pixel half of a 16.16 value.
#define HI(v) (((s16 *)&(v))[1])

extern char ResourceId_Data134;
extern char ResourceId_Data0F3;
extern char ResourceId_EffectPalette02;

// The offsets of the spark cels: cel n is n pixels wide and 2n tall.
extern const u16 gData_08197410[];

void BattleEffect_StartCanvasLayer(s32 bgControl);
void *Resource_GetTableEntry(s32 resourceId);
void EffectPosition_ProjectBattler(s32 id, struct EffectPosition *position);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);
void Func_08118088(s32 actor, s32 motion);
void Func_081180e8(s32 phase);
void Func_0814cd48(s32 actor, s32 palette, s32 member, s32 timer, s32 frames);
s32 BattleEffect_LoadBlitter(s32 slot, s32 blitter);
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                          s32 gravity);
void HeapSlot_Release(s32 slot);
void Func_08158ce0(s32 x, s32 y);
void BattleEffect_CountDownBattlerTints(void);
void wait(s32 frames);
void BattleEffect_ProcessPendingGraphicsTransfer(void);
void BattleEffect_EndCanvasLayer(void);

// A swing. A five-cel slash sweeps across the caster's front, six frames a
// cel, mirrored on the far side; at frame 18 the target is knocked back and
// flashes, the result phase starts, and 256 sparks spray from the slash's
// end toward the target, half of them falling and half rising as they burn
// down.
void BattleEffect_RunWhiplash(struct BattleEffectArgument *effect) {
  struct HeapSlots *heap;
  struct BattleEffectWork *work;
  u8 *canvas;
  struct EffectPosition caster;
  s32 frame;
  struct EffectStep *spark;
  // Unused: draw[1] gives the stack frame its 4 bytes.
  DrawRectangle draw[2];
  u8 *sparkCels;
  s32 i;

  heap = Ram_HeapSlots;
  work = (struct BattleEffectWork *)heap->battleFxWork[0];
  canvas = heap->battleFxWork[1];
  sparkCels = heap->unk64[0];

  BattleEffect_StartCanvasLayer(1);
  if (effect->side == 0) {
    BattleEffect_LoadBlitter(HEAP_SLOT_BLITTER, BLITTER(BLITTER_MAX, CLIP_XY));
    draw[0] = (DrawRectangle)heap->unk64[1];
  } else {
    BattleEffect_LoadBlitter(HEAP_SLOT_BLITTER,
                             BLITTER(BLITTER_MAX, CLIP_XY | FLIP_X));
    draw[0] = (DrawRectangle)heap->unk64[1];
  }
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sparkCels, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0F3, work->sheet, 0, 0);
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_EffectPalette02),
                  128);
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  for (frame = 0; frame != SWING_FRAMES; frame++) {
    // The slash, its cels anchored to the caster's front.
    EffectPosition_ProjectBattler(effect->actor, &caster);
    if (effect->side == 0) {
      switch (frame / 6) {
      case 0:
        draw[0](canvas, work->sheet, caster.x / 2 - 48, caster.y - 120, 48, 96);
        break;
      case 1:
        draw[0](canvas, work->sheet + 0x1200, caster.x / 2 - 38, caster.y - 120,
                38, 92);
        break;
      case 2:
        draw[0](canvas, work->sheet + 0x1fa8, caster.x / 2 - 38, caster.y - 100,
                30, 76);
        break;
      case 3:
        draw[0](canvas, work->sheet + 0x2890, caster.x / 2 - 64, caster.y - 88,
                56, 96);
        break;
      case 4:
        draw[0](canvas, work->sheet + 0x3d90, caster.x / 2 - 64, caster.y - 88,
                56, 88);
        break;
      }
    } else {
      switch (frame / 6) {
      case 0:
        draw[0](canvas, work->sheet, caster.x / 2, caster.y - 120, 48, 96);
        break;
      case 1:
        draw[0](canvas, work->sheet + 0x1200, caster.x / 2, caster.y - 120, 38,
                92);
        break;
      case 2:
        draw[0](canvas, work->sheet + 0x1fa8, caster.x / 2, caster.y - 100, 30,
                76);
        break;
      case 3:
        draw[0](canvas, work->sheet + 0x2890, caster.x / 2, caster.y - 88, 56,
                96);
        break;
      case 4:
        draw[0](canvas, work->sheet + 0x3d90, caster.x / 2, caster.y - 88, 56,
                88);
        break;
      }
    }

    // The hit, and the sparks sprayed from 48 pixels out.
    if (frame == SWING_HIT_FRAME) {
      Func_08118088(effect->actors[0], 1);
      Func_0814cd48(effect->actors[0], 7, 5, 0, 16);
      Func_081180e8(134);
      work->shakeFrames = 8;
      for (i = 0; i != SPARK_COUNT; i++) {
        s32 angle;
        s32 speed;

        spark = &((struct EffectStep *)Ram_MapCellBuffer)[i];
        if (effect->side == 0)
          spark->x = (caster.x / 2 - 48) << 16;
        else
          spark->x = (caster.x / 2 + 48) << 16;
        spark->y = (caster.y - (Random16() & 7)) << 16;
        angle = (Random16() & 0x7fff) + 0x4000;
        speed = Random16() & 255;
        if (effect->side == 0)
          spark->velocityX = ((Trig_Sin(angle) * speed) >> 4) - (8 << 16);
        else
          spark->velocityX = ((Trig_Sin(angle) * speed) >> 4) + (8 << 16);
        spark->velocityY = (Trig_Cos(angle) * (speed + 64)) >> 5;
        spark->variant = (Random16() & 15) + 16;
      }
    }
    if (frame > SWING_HIT_FRAME - 1) {
      for (i = 0; i != SPARK_COUNT; i++) {
        spark = &((struct EffectStep *)Ram_MapCellBuffer)[i];
        if (spark->variant >= 0) {
          s32 size = (spark->variant >> 3) + 1;

          draw[0](canvas, sparkCels + gData_08197410[size - 1],
                  HI(spark->x) - size / 2, HI(spark->y) - size, size, size * 2);
          if (i & 1)
            BattleEffectKernels_IntegrateVector2(spark, 60, -0x2000);
          else
            BattleEffectKernels_IntegrateVector2(spark, 60, 0x1000);
          spark->variant--;
        }
      }
    }

    Func_08158ce0(8, 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_SLOT_BLITTER);
  BattleEffect_EndCanvasLayer();
}

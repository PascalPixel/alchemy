// 2026-10-25, with the Makefile's options (-mthumb-slow-loads): differ 6 of
// 464 with two empty asm: one before the heap reads stores the argument at
// sp+24 first, as the native, and one before the frame loop (10 without
// either). Left: the stack frame's 0 stored before &caster is built, and the spark
// loop's 0 after the shake store (an empty asm there moves the
// gMapCellBuffer pool load: 12).
// Draft (tla-en): listing 464 instructions, draft 464, common 447, differ 34.
// Written from the listing (☀️'s BattleEffect_RunRushingStrike is only
// a hint). A five-cell burst drawn on the actor's side for 30 frames;
// at frame 18 256 particles (in gMapCellBuffer) scatter from it, drawn
// from frame 18 on with sizes from gData_08197410.
// 2026-10-19 (run 33): the native stack frame's unreferenced slot at sp+16 is the
// second half of a blitter pair (tagged below): 285 to 34, with the frame,
// canvas in r11, the allocation and the call sites all the native's. What
// is left is 17 reordered pairs, each the native loading caster.y or a
// pool word one row before the neighbouring constant (the draw calls'
// widths and sheet offsets), the scheduling difference most ⚓️ near misses
// share.
// Earlier notes, from before the pair:
// - the native stack frame is 40 bytes with an unreferenced slot at sp+16
// between frame (sp+20) and draw (sp+12), as in 0817b9b8; canvas stays
// in r11 and draw and sheet are spilled. Here the stack frame is 36 and canvas
// is spilled.
// - the heap slot base stays in r5 from the first loads to draw's load
// after the BattleEffect_LoadBlitter call, as through the "heap" pointer here.
// - each arm loads the blitter after its BattleEffect_LoadBlitter call, so the two
// calls end alike and are merged into one, as in the native (with the
// load after the if, the else arm's call keeps a USE and stays apart).
// - the first switch's case 0 draws through r6 in the native, which keeps
// its call apart; here it loads r4 and joins the shared draw (31 call
// sites for 32).
// - cross-jumping of the switch's draw calls: in the native side 0's case
// 0 keeps its own call (its draw is reloaded into r6, the rest into r4),
// cases 1 and 2 of both sides share one call and cases 3 and 4 share
// "subs r3, #88" before it.
// Resources 308, 243 and 329 (BattleEffect_EffectPalette02) need names in
// system/resource/directory.s when this is adopted.
// 2026-10-15 (lane N2): rewritten with the work struct, named cels and
// constants and comments. The same differ 284; one BattleEffect_LoadBlitter call with
// the mode chosen by a conditional expression: 289 (the native's single
// call is the two arms cross-jumped).
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "effect_step.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "ram_buffer.h"
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
void Task_Add(s32 callback, s32 priority);
s32 Task_Remove(s32 callback);
void EffectPosition_ProjectBattler(
    s32 id, struct EffectPosition *position);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);
void Func_08118088(s32 actor, s32 motion);
void Func_081180e8(s32 phase);
void Func_0814cd48(s32 actor, s32 palette, s32 member, s32 timer, s32 frames);
void BattleEffect_LoadBlitter(s32 slot, s32 blitter);
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                      s32 gravity);
void HeapSlot_Release(s32 slot);
void Func_08158ce0(s32 x, s32 y);
void ObjectGroup_CountDownMemberTimers(void);
void WaitFrames(s32 frames);
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
  /* FAKEMATCH(frame): the blitter as a pair of which only the first is used;
   * as one two-word DImode pseudo its spill takes sp+12 and the unused sp+16
   * the native stack frame has. As a single DrawRectangle the stack frame is 36 bytes
   * and the allocation moves (285 of 464 instructions differ in tla-en, 34
   * with the pair). */
  DrawRectangle draw[2];
  u8 *sparkCels;
  s32 i;

  __asm__("");
  heap = Ram_HeapSlots;
  work = (struct BattleEffectWork *)heap->battleFxWork[0];
  canvas = heap->battleFxWork[1];
  sparkCels = heap->unk64[0];

  BattleEffect_StartCanvasLayer(1);
  if (effect->side == 0) {
    BattleEffect_LoadBlitter(HEAP_SLOT_BLITTER, BLITTER(BLITTER_MAX, CLIP_XY));
    draw[0] = (DrawRectangle)heap->unk64[1];
  } else {
    BattleEffect_LoadBlitter(HEAP_SLOT_BLITTER, BLITTER(BLITTER_MAX, CLIP_XY | FLIP_X));
    draw[0] = (DrawRectangle)heap->unk64[1];
  }
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sparkCels, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0F3, work->sheet, 0, 0);
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_EffectPalette02),
                  128);
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add((s32)BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  __asm__("");

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
        draw[0](canvas, work->sheet + 0x2890, caster.x / 2 - 64, caster.y - 88, 56,
             96);
        break;
      case 4:
        draw[0](canvas, work->sheet + 0x3d90, caster.x / 2 - 64, caster.y - 88, 56,
             88);
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
        draw[0](canvas, work->sheet + 0x2890, caster.x / 2, caster.y - 88, 56, 96);
        break;
      case 4:
        draw[0](canvas, work->sheet + 0x3d90, caster.x / 2, caster.y - 88, 56, 88);
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
    ObjectGroup_CountDownMemberTimers();
    work->transferPending = 1;
    WaitFrames(1);
  }

  Task_Remove((s32)BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_SLOT_BLITTER);
  BattleEffect_EndCanvasLayer();
}

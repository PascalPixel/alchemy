// Draft (tla-en): listing 464 instructions, draft 461, differ 285.
// Written from the listing (☀️'s BattleFx_RunRushingStrike is only
// a hint). A five-cell burst drawn on the actor's side for 30 frames;
// at frame 18 256 particles (in gMapCellBuffer) scatter from it, drawn
// from frame 18 on with sizes from gData_08197410.
// Left:
// - the native frame is 40 bytes with an unreferenced slot at sp+16
// between frame (sp+20) and draw (sp+12), as in 0817b9b8; canvas stays
// in r11 and draw and sheet are spilled. Here the frame is 36 and canvas
// is spilled.
// - the heap slot base stays in r5 from the first loads to draw's load
// after the BattleFx_LoadBlitter call, as through the "heap" pointer here.
// - each arm loads the blitter after its BattleFx_LoadBlitter call, so the two
// calls end alike and are merged into one, as in the native (with the
// load after the if, the else arm's call keeps a USE and stays apart).
// - the first switch's case 0 draws through r6 in the native, which keeps
// its call apart; here it loads r4 and joins the shared draw (31 call
// sites for 32).
// - cross-jumping of the switch's draw calls: in the native side 0's case
// 0 keeps its own call (its draw is reloaded into r6, the rest into r4),
// cases 1 and 2 of both sides share one call and cases 3 and 4 share
// "subs r3, #88" before it.
// Resources 308, 243 and 329 (BattleFx_EffectPalette02) need names in
// system/resource/directory.s when this is adopted.
// 2026-10-15 (lane N2): rewritten with the work struct, named cels and
// constants and comments. The same differ 284; one BattleFx_LoadBlitter call with
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

void BattleFx_BeginCanvasLayer(s32 bgControl);
void *Resource_GetTableEntry(s32 resourceId);
void Scheduler_AddOrUpdateCallback(s32 callback, s32 priority);
s32 Scheduler_RemoveCallback(s32 callback);
void EffectPosition_ProjectBattler(
    s32 id, struct EffectPosition *position);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);
void Func_08118088(s32 actor, s32 motion);
void Func_081180e8(s32 phase);
void Func_0814cd48(s32 actor, s32 palette, s32 member, s32 timer, s32 frames);
void BattleFx_LoadBlitter(s32 slot, s32 blitter);
void BattleFxKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                      s32 gravity);
void Runtime_ReleaseHeapBlock(s32 slot);
void Func_08158ce0(s32 x, s32 y);
void ObjectGroup_TickMemberTimers(void);
void WaitFrames(s32 frames);
void BattleFx_ProcessPendingGraphicsTransfer(void);
void BattleFx_EndCanvasLayer(void);

// A swing. A five-cel slash sweeps across the caster's front, six frames a
// cel, mirrored on the far side; at frame 18 the target is knocked back and
// flashes, the result phase starts, and 256 sparks spray from the slash's
// end toward the target, half of them falling and half rising as they burn
// down.
void BattleFx_RunWhiplash(struct BattleEffectArgument *effect) {
  struct HeapSlots *heap = Ram_HeapSlots;
  struct BattleEffectWork *work =
      (struct BattleEffectWork *)heap->battleFxWork[0];
  u8 *canvas = heap->battleFxWork[1];
  struct EffectPosition caster;
  s32 frame;
  struct EffectStep *spark;
  DrawRectangle draw;
  u8 *sparkCels = heap->unk64[0];
  s32 i;

  BattleFx_BeginCanvasLayer(1);
  if (effect->side == 0) {
    BattleFx_LoadBlitter(HEAP_SLOT_BLITTER, BLITTER(BLITTER_MAX, CLIP_XY));
    draw = (DrawRectangle)heap->unk64[1];
  } else {
    BattleFx_LoadBlitter(HEAP_SLOT_BLITTER, BLITTER(BLITTER_MAX, CLIP_XY | FLIP_X));
    draw = (DrawRectangle)heap->unk64[1];
  }
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sparkCels, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0F3, work->sheet, 0, 0);
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_EffectPalette02),
                  128);
  work->transferMode = 2;
  work->transferValue = 50;
  Scheduler_AddOrUpdateCallback((s32)BattleFx_ProcessPendingGraphicsTransfer, 0xc80);

  for (frame = 0; frame != SWING_FRAMES; frame++) {
    // The slash, its cels anchored to the caster's front.
    EffectPosition_ProjectBattler(effect->actor, &caster);
    if (effect->side == 0) {
      switch (frame / 6) {
      case 0:
        draw(canvas, work->sheet, caster.x / 2 - 48, caster.y - 120, 48, 96);
        break;
      case 1:
        draw(canvas, work->sheet + 0x1200, caster.x / 2 - 38, caster.y - 120,
             38, 92);
        break;
      case 2:
        draw(canvas, work->sheet + 0x1fa8, caster.x / 2 - 38, caster.y - 100,
             30, 76);
        break;
      case 3:
        draw(canvas, work->sheet + 0x2890, caster.x / 2 - 64, caster.y - 88, 56,
             96);
        break;
      case 4:
        draw(canvas, work->sheet + 0x3d90, caster.x / 2 - 64, caster.y - 88, 56,
             88);
        break;
      }
    } else {
      switch (frame / 6) {
      case 0:
        draw(canvas, work->sheet, caster.x / 2, caster.y - 120, 48, 96);
        break;
      case 1:
        draw(canvas, work->sheet + 0x1200, caster.x / 2, caster.y - 120, 38,
             92);
        break;
      case 2:
        draw(canvas, work->sheet + 0x1fa8, caster.x / 2, caster.y - 100, 30,
             76);
        break;
      case 3:
        draw(canvas, work->sheet + 0x2890, caster.x / 2, caster.y - 88, 56, 96);
        break;
      case 4:
        draw(canvas, work->sheet + 0x3d90, caster.x / 2, caster.y - 88, 56, 88);
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

          draw(canvas, sparkCels + gData_08197410[size - 1],
               HI(spark->x) - size / 2, HI(spark->y) - size, size, size * 2);
          if (i & 1)
            BattleFxKernels_IntegrateVector2(spark, 60, -0x2000);
          else
            BattleFxKernels_IntegrateVector2(spark, 60, 0x1000);
          spark->variant--;
        }
      }
    }

    Func_08158ce0(8, 8);
    ObjectGroup_TickMemberTimers();
    work->transferPending = 1;
    WaitFrames(1);
  }

  Scheduler_RemoveCallback((s32)BattleFx_ProcessPendingGraphicsTransfer);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
  BattleFx_EndCanvasLayer();
}

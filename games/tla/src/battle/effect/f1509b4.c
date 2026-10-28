#include "types.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "task.h"
#include "effect_step.h"
#include "fixed_math.h"
#include "io_reg.h"
#include "mem.h"
#include "object_runtime.h"
#include "ram_buffer.h"
#include "system.h"

// The canvas layer and its helpers; the comments give The Broken Seal's
// names where the twin is known.
void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_EndCanvasLayer(void); // BattleEffect_EndCanvasLayer
void BattleEffect_ProcessPendingGraphicsTransfer(void); // BattleEffect_ProcessPendingGraphicsTransfer
void ObjectGroup_CountDownMemberTimers(void); // ObjectGroup_CountDownMemberTimers
void BattleEffect_LoadBlitter(s32 slot, s32 modeAndFlags); // BattleEffect_BuildBlitter
void Func_0814cd48(s32 actor, s32 palette, s32 frames, s32 timerSlot,
                   s32 timerFrames); // ObjectGroup_UpdateMembers
void Func_081180e8(s32 phase);       // BattleEventRuntime_StartPhaseFar

// The Lost Age's blitter modes, BattleEffect_LoadBlitter's high nibble: 3 minus The
// Broken Seal's COMPOSITE_ADDITIVE, COMPOSITE_MAX and COMPOSITE_TRANSPARENT.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT };
#define BLIT_MODE(mode, flags) ((mode) << 4 | (flags))

#define FLAME_COUNT 9

extern char ResourceId_EmberStreakSheet;
extern u16 gData_0819747a[]; // gBattleEffect_PuffArcCells
extern u8 gData_08197467[];  // gBattleEffect_PuffArcCellWidths
extern u8 gData_08197473[];  // gBattleEffect_PuffArcCellBiasY
extern u8 gData_0819746d[];  // gBattleEffect_PuffArcCellHeights

// Nine flames puff up along a half-circle arc, four frames apart.

// Setup: the flames are placed on an arc 32 pixels wide and 16 high, from
// the left when the first target is an enemy (128 and up), else from the
// right, each one starting four frames after the last. Frame loop: a flame
// steps through six pictures, four frames each; variant 1 adds a copy 16
// pixels higher, variant 2 another 32 higher and a brighter trail. The
// targets are hit from frame 16, eight frames apart.
void BattleEffect_RunFlare(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  DrawRectangle draw;
  s32 i;
  s32 cell;
  u32 w;
  s32 sign;
  s32 org;
  s32 angle;
  s32 stagger;
  struct EffectStep *puff;
  struct EffectStep *p;

  /* FAKEMATCH(one-pass): an empty one-pass block keeps the argument's spill
   * ahead of the RAM base, as the native stores it; as the function without
   * it, 2 of 274 instructions differ in tla-en. */
  do {
  } while (0);
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  BattleEffect_StartCanvasLayer(0);
  REG_BLDCNT = 0x3f46;
  REG_BLDALPHA = 0x1010;
  Resource_LoadAndDecompress((s32)&ResourceId_EmberStreakSheet, work->sheet, 1,
                             1);
  BattleEffect_LoadBlitter(104,
                BLIT_MODE(BLIT_ADDITIVE, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  draw = (DrawRectangle)Ram_HeapSlots->unk64[1];

  // The arc: from -90 degrees in steps of 22.5.
  if (effect->actors[0] > 127) {
    org = 0;
    sign = 1;
  } else {
    org = 64;
    sign = -1;
  }
  angle = -0x4000;
  i = 0;
  stagger = 0;
  puff = work->particles;
  do {
    puff->x = ((sign * ((Trig_Sin(angle) << 5) >> 16)) + org) + 20;
    puff->y = ((Trig_Cos(angle) << 4) >> 16) + 40;
    puff->variant = stagger;
    angle += 0x1000;
    stagger -= 4;
    i += 1;
    puff++;
  } while (i != FLAME_COUNT);

  work->transferMode = 2;
  if (effect->variant == 2)
    work->transferValue = 75;
  else
    work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  Sound_PlayFar(136);
  frame = 0;
  do {
    if (frame == 24)
      Func_081180e8(133);

    // A flame moves to its next picture every four frames and goes out
    // after the sixth.
    i = 0;
    p = work->particles;
    do {
      if (p->variant >= 0 && p->variant <= 23) {
        cell = p->variant / 4;
        draw(canvas, work->sheet + gData_0819747a[cell],
             p->x - ((w = gData_08197467[cell]) >> 1),
             p->y + gData_08197473[cell], w, gData_0819746d[cell]);
        if (effect->variant != 0)
          draw(canvas, work->sheet + gData_0819747a[cell],
               p->x - ((w = gData_08197467[cell]) >> 1),
               (p->y + gData_08197473[cell]) - 16, w, gData_0819746d[cell]);
        if (effect->variant == 2)
          draw(canvas, work->sheet + gData_0819747a[cell],
               p->x - ((w = gData_08197467[cell]) >> 1),
               (p->y + gData_08197473[cell]) - 32, w, gData_0819746d[cell]);
      }
      i += 1;
      p->variant += 1;
      p++;
    } while (i != FLAME_COUNT);

    // Hit the targets, eight frames apart.
    i = 0;
    while (i != effect->count) {
      if (frame == (i * 8) + 16)
        Func_0814cd48(effect->actors[i], 10, 5, i, 12);
      i += 1;
    }
    ObjectGroup_CountDownMemberTimers();
    work->transferPending = 1;
    WaitFrames(1);
    frame += 1;
  } while (frame != 60);
  Runtime_ReleaseHeapBlock(104);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

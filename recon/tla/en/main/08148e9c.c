// Near miss, differ 12 of 429 (effect kind 20, the tornado).
//
// Measured 2026-10-25 under -mthumb-slow-loads: differ 12. Most rows left are
// loads from memory or the stack that alchemy-gcc issues one slot earlier than
// the native, which issues only its pool loads early. The first is the
// argument's spill, which the native stores before it starts the 0x03000000
// base; an empty asm before the first statement closes that row (differ 10).

// ☀️ twin: BattleEffect_RunWhirlwind (games/tbs/src/effect/effect.c), shape 0.835,
// structure 0.941. Ported with ⚓️'s checklist: the effect argument read
// directly, the tornado sheet unpacked into the work with
// Resource_LoadAndDecompress, max blitters from BattleEffect_LoadBlitter(104, 0x13)
// and (188, 0x17), the transfer callback at 0xc80, the shape table
// gData_081979ae (☀️ gVortices_Shapes) and the glint cell tables
// gData_08197486/92/98, Func_08118088 for the struck targets, and the two
// blitters released before the callback is removed, as ☀️ does.

// Same frame, registers and instruction count. What is left is order: each
// row a load (pool word, work slot, the argument spill) that the native
// issues one instruction earlier than agscc, and the literal pools that
// move with them (each pool load's offset differs by 4). The prologue's
// first row is the argument spill (str r0) that the native issues before
// the 0x03000000 base; the glint loop's cell offset load and the funnel's
// segment sums account for most of the rest.

// Tried (differ):
// the repository draft (local copy of the work struct)                66
// the shared struct BattleEffectWork                                  66
// BattleEffect_LoadBlitter declared returning s32 (plan 0c)                      64
// greedy declaration-order scan, every local through every place     64

#include "types.h"
#include "system.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "task.h"
#include "mem.h"
#include "effect_step.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "fixed_math.h"

// Heap slots The Lost Age addresses by byte offset in Ram_HeapSlots.
#define HEAP_BLITTER 104           // ☀️ HEAP_SLOT_BLITTER
#define HEAP_BLITTER_ALTERNATE 188 // ☀️ HEAP_SLOT_BLITTER_ALTERNATE

// The Lost Age numbers its blitter modes in the reverse of The Broken Seal's
// order: Func_08196404 builds additive for 0, max for 1, transparent for 2.
enum { BLITTER_ADDITIVE, BLITTER_MAX, BLITTER_TRANSPARENT, BLITTER_COPY };

#define TORNADO_FRAMES 80
#define GLINT_COUNT 16 // four glints for each of up to four funnels
#define GLINT_CELS 6   // three cels of two frames each

extern char ResourceId_TornadoSheet;

// ☀️ gVortices_Shapes: funnels, sway radius and shake, for each variant.
extern u8 gData_081979ae[];
// ☀️ gBattleEffect_GlintCells, Widths and Heights.
extern u16 gData_08197486[];
extern u8 gData_08197492[];
extern u8 gData_08197498[];

void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_EndCanvasLayer(void);                  // ☀️ BattleEffect_EndCanvasLayer
void BattleEffect_ProcessPendingGraphicsTransfer(void);                  // ☀️ ProcessPendingGraphicsTransfer
void ObjectGroup_CountDownMemberTimers(void);                  // ☀️ ObjectGroup_CountDownMemberTimers
void Func_08158ce0(s32 x, s32 y);          // ☀️ Camera_ApplyShake
void Func_081180e8(s32 phase);             // ☀️ BattleEvent_StartPhaseFar
void Func_08118088(s32 actor, s32 motion); // ☀️ ApplyVariantMotionFar
s32 BattleEffect_LoadBlitter(s32 slot, s32 flags);    // ☀️ BattleEffect_BuildBlitter
void Func_0814cd48(s32 actor, s32 palette, s32 frames, s32 timerSlot,
                   s32 timerFrames); // ☀️ ObjectGroup_UpdateMembers
void Sound_PlayFar(s32 cue);

// One to four funnels of wind sway across the targets for 80 frames, each
// drawn as three stacked segments with glints swirling around it; the canvas
// is wiped every eight frames, and each target is struck seven times.
void BattleEffect_RunWhirlwind(struct BattleEffectArgument *effect) {
  DrawRectangle draw[2];
  struct BattleEffectWork *work;
  void *canvas;
  s32 wave;
  struct EffectStep *glint;
  s32 frame;
  s32 x;
  s32 y;
  s32 segment;
  s32 cell;
  s32 i;
  s32 j;

  // Set up: the canvas layer, the tornado sheet and two max blitters, the
  // second mirrored.
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  BattleEffect_StartCanvasLayer(1);
  REG_BLDALPHA = 0x1010;
  Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, work->sheet, 1, 1);
  BattleEffect_LoadBlitter(HEAP_BLITTER,
                BLITTER_MAX << 4 | COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y);
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE, BLITTER_MAX << 4 | COMPOSITE_CLIP_X |
                                            COMPOSITE_CLIP_Y |
                                            COMPOSITE_FLIP_X);
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];

  // The glints start at random places in their funnel, staggered in time.
  for (j = 0; j != GLINT_COUNT; j++) {
    glint = &work->particles[j];
    glint->x = Random16() & 31;
    glint->y = (Random16() & 63) + 16;
    glint->variant = -(Random16() & 15);
  }
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  REG_BLDALPHA = 0x1000;
  WaitFrames(1);
  Sound_PlayFar(141);

  for (frame = 0; frame != TORNADO_FRAMES; frame++) {
    wave = Trig_Sin(frame << 10) << 4;
    if (frame == 32)
      Func_081180e8(133);

    // Wipe the canvas every eight frames from frame 16, leaving a trail.
    for (i = 0; i != 7; i++) {
      if (frame == i * 8 + 16)
        Iwram_FillWords(canvas, 0x4000, 0x08080808);
    }

    // Sway toward the targets' side; fade in over 16 frames and out over the
    // last 16.
    if (effect->side == 1)
      wave += 0x200000;
    else
      wave -= 0x200000;
    if (frame <= 16)
      REG_BLDALPHA = frame | 0x1000;
    if (frame > 63)
      REG_BLDALPHA = (0x4f - frame) | 0x1000;

    for (i = 0; i != gData_081979ae[effect->variant * 3]; i++) {
      // Each funnel circles its own phase, a quarter turn from the next.
      x = ((gData_081979ae[effect->variant * 3 + 1] *
                Trig_Sin((frame << 11) + i * 0x4000) +
            wave) >>
           16) +
          40;
      y = (Trig_Cos((frame << 11) + i * 0x4000) * 2) >> 16;
      segment = (frame / 2) % 3;
      draw[0](canvas, work->sheet + segment * 0xa00 + 0xc56, x, y + 16, 40, 32);
      draw[0](canvas, work->sheet + segment * 0x500 + 0x2a56, x, y + 48, 40,
              32);
      draw[0](canvas, work->sheet + segment * 0xa00 + 0x1156, x, y + 80, 40,
              32);

      // Its four glints, the second of each pair mirrored; each restarts at
      // a new place when its cels run out.
      for (j = 0; j != 4; j++) {
        glint = &work->particles[i * 4 + j];
        if (glint->variant >= 0) {
          cell = glint->variant / 2 + (j / 2) * 3;
          draw[j & 1](canvas, work->sheet + gData_08197486[cell], glint->x + x,
                      glint->y + y, gData_08197492[cell], gData_08197498[cell]);
        }
        glint->variant++;
        if (glint->variant == GLINT_CELS) {
          glint->x = Random16() & 31;
          glint->y = (Random16() & 63) + 16;
          glint->variant = 0;
        }
      }
    }

    // Strike each target seven times, every eight frames, three frames
    // after the target before it.
    for (j = 0; j != effect->count; j++) {
      for (i = 0; i != 7; i++) {
        if (frame == i * 8 + j * 3 + 16) {
          Func_0814cd48(effect->actors[j], 7, 5, j, 4);
          Func_08118088(effect->actors[j], 6);
        }
      }
    }

    work->shakeFrames = 1;
    Func_08158ce0(gData_081979ae[effect->variant * 3 + 2],
                  gData_081979ae[effect->variant * 3 + 2] * 2);
    ObjectGroup_CountDownMemberTimers();
    work->transferPending = 1;
    WaitFrames(1);
  }

  // Tear down.
  Runtime_ReleaseHeapBlock(HEAP_BLITTER_ALTERNATE);
  Runtime_ReleaseHeapBlock(HEAP_BLITTER);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

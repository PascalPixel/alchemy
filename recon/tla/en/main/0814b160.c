// Differ 16 of 222 (tla-en, 2026-11-08, -mtune=gba): the marker loop's
// head sets 36 (the actors offset) and work in swapped registers, and its tail
// steps i and marker with r1 and r2 swapped. No gain (16 to 24) from marker stepped first,
// ++i in the test, a for loop (both init and step orders), marker
// set outside the guard, and an 8-minute permuter run (best 22).
// Also 16 or worse (2026-11-08): marker block-scoped, set before i, or as
// &work->particles[i] (31: the giv then points at variant), i and marker as
// += 1 or x = x + 1, ++i in the test, the test reversed, effect->count in a
// local, declaration orders, once-only blocks around each step and the
// marker's set, start as i * 16 or u32. The head's order follows the marker's
// move, which source order puts before the giv for 36.
// Measured 2026-11-05 (tla-en): differ 16 of 222. Left: two hoisted constants
// in swapped registers (36 and the stride), r0 and r1 at the head of the loop,
// r1 and r2 at its tail.

// Near miss, differ 18 of 222 (effect kinds 41 and 354, the target markers).

// ☀️ twin: BattleEffect_RunCurse (games/tbs/src/effect/effect.c), shape
// 0.771, structure 1.000. ⚓️'s changes: the effect argument read directly
// (the dispatcher stores work->effect), the star cross sheet (directory row
// 0x13b), the blitter from BattleEffect_LoadBlitter(104, 0x23) (transparent, ⚓️ mode
// 2) and the transfer callback at 0xc80. ☀️ needs its family's blitter pair
// for the frame (a FAKEMATCH tag there), and so does this one: as a single
// DrawRectangle the stack frame is 4 bytes short (differ 170).

// The frame slots and every instruction match. What is left is order: the
// native loads the heap slot before it sets the first argument, loads each
// pool word (0x100, -0x7000) ahead of the register's address, sets r1
// before r0 for BattleEffect_LoadBlitter, and reloads its spills one row earlier in the
// marker loop.

// Tried (differ):
// the repository draft (local copy of the work struct)                44
// the blitter flags in a local first                                  44
// BattleEffect_LoadBlitter declared returning s32 as ☀️'s LoadWork does          42
// ☀️'s slot form (slot = &battleFxWork[1]; canvas = slot[0];
// work = slot[-1]; draw[0] = slot[2])                               48
// greedy declaration-order scan, every local through every place     42
// the frame's blitter read before the transfer value is cleared, and
// i stepped before marker in the marker loop (statement swaps)       18

#include "types.h"
#include "system.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "task.h"
#include "heap_release.h"
#include "effect_step.h"
#include "battle_effect.h"
#include "battle_effect_work.h"

#define HEAP_BLITTER 104 // ☀️ HEAP_SLOT_BLITTER

// The Lost Age numbers its blitter modes in the reverse of The Broken Seal's
// order: Func_08196404 builds additive for 0, max for 1, transparent for 2.
enum { BLITTER_ADDITIVE, BLITTER_MAX, BLITTER_TRANSPARENT, BLITTER_COPY };

#define MARKER_FRAMES 72 // how long each marker stays
#define MARKER_STAGGER 16
#define MARKER_CELS 9

extern char ResourceId_StarCrossSheet; // directory row 0x13b

void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_ProcessPendingGraphicsTransfer(void);               // ☀️ ProcessPendingGraphicsTransfer
void Func_081180e8(s32 phase);          // ☀️ BattleEvent_StartPhaseFar
s32 BattleEffect_LoadBlitter(s32 slot, s32 flags); // ☀️ BattleEffect_BuildBlitter
void Sound_PlayFar(s32 cue);

// A spinning star marks each target in turn, sixteen frames apart: a post
// under the target and the star turning above it, each for 72 frames.
void BattleEffect_RunCurse(struct BattleEffectArgument *effect) {
  void *canvas;
  struct BattleEffectWork *work;
  // Unused: the second blitter gives the stack frame its 4 bytes (as one, 156
  // of 222 differ in tla-en); the family's pair, as in The Broken Seal.
  DrawRectangle draw[2];
  struct EffectStep *marker;
  s32 i;
  s32 frame;
  s32 start;
  s32 cel;
  s32 y;
  struct EffectPosition pos;

  // Set up: the canvas layer, unscaled; for side 1 the canvas shows the
  // right half of the field.
  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  BattleEffect_StartCanvasLayer(1);
  REG_BG2PA = 0x100;
  REG_BLDCNT = 0;
  Resource_LoadAndDecompress((s32)&ResourceId_StarCrossSheet, work->sheet, 1,
                             1);
  if (effect->side == 1) {
    REG_BG2X = -0x7000;
  }
  BattleEffect_LoadBlitter(HEAP_BLITTER,
                BLITTER_TRANSPARENT << 4 | COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y);
  work->transferMode = 1;
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  work->transferValue = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  // Each star starts at a random point in its turn.
  for (i = 0; i != effect->count; i++) {
    work->particles[i].variant = Random16() & 63;
  }

  frame = 0;
  while (frame != (effect->count << 5) + 32) {
    if (frame == 32) {
      Func_081180e8(0);
    }
    i = 0;
    if (effect->count != 0) {
      marker = work->particles;
      do {
        start = i << 4;
        if (frame == start) {
          Sound_PlayFar(143);
        }
        if (frame >= start && frame < start + MARKER_FRAMES) {
          // The post under the target, then the star, six frames a cel.
          EffectPosition_ProjectScaledBattler(effect->actors[i], &pos);
          if (effect->side == 1) {
            pos.x -= 112;
          }
          y = pos.y;
          pos.y = y - 16;
          draw[0](canvas, work->sheet + 1728, pos.x - 8, y - 20, 16, 20);
          // DEAD: always true here; the ROM keeps the test.
          if (frame >= start) {
            cel = ((frame - start) + marker->variant) / 6 % MARKER_CELS;
            draw[0](canvas, work->sheet + cel * 192, pos.x - 8, pos.y - 16, 16,
                    12);
          }
        }
        i++;
        marker++;
      } while (i != effect->count);
    }
    work->transferPending = 1;
    wait(1);
    frame += 1;
  }

  // Tear down.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}

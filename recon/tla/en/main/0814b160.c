// Near miss, differ 42 of 222 (effect kinds 41 and 354, the target markers).

// ☀️ twin: BattleFx_RunCurse (games/tbs/src/effect/effect.c), shape
// 0.771, structure 1.000. ⚓️'s changes: the effect argument read directly
// (the dispatcher stores work->effect), the star cross sheet (directory row
// 0x13b), the blitter from BattleFx_LoadBlitter(104, 0x23) (transparent, ⚓️ mode
// 2) and the transfer callback at 0xc80. ☀️ needs its family's blitter pair
// for the frame (a FAKEMATCH tag there), and so does this one: as a single
// DrawRectangle the frame is 4 bytes short (differ 170).

// The frame slots and every instruction match. What is left is order: the
// native loads the work slot before it sets the first argument, loads each
// pool word (0x100, -0x7000) ahead of the register's address, sets r1
// before r0 for BattleFx_LoadBlitter, and reloads its spills one row earlier in the
// marker loop.

// Tried (differ):
// the repository draft (local copy of the work struct)                44
// the blitter flags in a local first                                  44
// BattleFx_LoadBlitter declared returning s32 as ☀️'s LoadWork does          42
// ☀️'s slot form (slot = &battleFxWork[1]; canvas = slot[0];
// work = slot[-1]; draw[0] = slot[2])                               48
// greedy declaration-order scan, every local through every place     42

#include "types.h"
#include "system.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "callback_scheduler.h"
#include "mem.h"
#include "effect_step.h"
#include "battle_effect.h"
#include "battle_effect_work.h"

#define HEAP_BLITTER 104 // ☀️ HEAP_SLOT_BLITTER

// The Lost Age numbers its blitter modes in the reverse of The Broken Seal's
// order: Func_08196404 builds additive for 0, max for 1, transparent for 2.
enum { BLITTER_ADDITIVE, BLITTER_MAX, BLITTER_TRANSPARENT, BLITTER_COPY };

#define CALLBACK_ORDER 0xc80
#define MARKER_FRAMES 72 // how long each marker stays
#define MARKER_STAGGER 16
#define MARKER_CELS 9

extern char ResourceId_StarCrossSheet; // directory row 0x13b

void BattleFx_BeginCanvasLayer(s32 bgControl);
void BattleFx_EndCanvasLayer(void);               // ☀️ BattleFx_EndCanvasLayer
void BattleFx_ProcessPendingGraphicsTransfer(void);               // ☀️ ProcessPendingGraphicsTransfer
void Func_081180e8(s32 phase);          // ☀️ BattleEvent_BeginPhaseFar
s32 BattleFx_LoadBlitter(s32 slot, s32 flags); // ☀️ BattleFx_BuildBlitter
void Audio_PlayCue(s32 cue);

// A spinning star marks each target in turn, sixteen frames apart: a post
// under the target and the star turning above it, each for 72 frames.
void BattleFx_RunCurse(struct BattleEffectArgument *effect) {
  void *canvas;
  struct BattleEffectWork *work;
  /* FAKEMATCH: the family's blitter pair, as in ☀️: this effect loads only
   * one. A single DrawRectangle leaves the frame 4 bytes short: differ 170. */
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
  BattleFx_BeginCanvasLayer(1);
  REG_BG2PA = 0x100;
  REG_BLDCNT = 0;
  Resource_LoadAndDecompress((s32)&ResourceId_StarCrossSheet, work->sheet, 1,
                             1);
  if (effect->side == 1) {
    REG_BG2X = -0x7000;
  }
  BattleFx_LoadBlitter(HEAP_BLITTER,
                BLITTER_TRANSPARENT << 4 | COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y);
  work->transferMode = 1;
  work->transferValue = 0;
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, CALLBACK_ORDER);

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
          Audio_PlayCue(143);
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
          if (frame >= start) {
            cel = ((frame - start) + marker->variant) / 6 % MARKER_CELS;
            draw[0](canvas, work->sheet + cel * 192, pos.x - 8, pos.y - 16, 16,
                    12);
          }
        }
        marker++;
        i++;
      } while (i != effect->count);
    }
    work->transferPending = 1;
    WaitFrames(1);
    frame += 1;
  }

  // Tear down.
  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  Runtime_ReleaseHeapBlock(HEAP_BLITTER);
  BattleFx_EndCanvasLayer();
}

// Matches: differ 0 (tla-en, 2026-11-08)
// Also differ 0 of 222 in tla-ja, tla-de, tla-es, tla-fr and tla-it
// (2026-11-08), plain C with no FAKEMATCH device. The last 16 were the marker
// loop's register order: marker as its own pointer stepped beside i (The
// Broken Seal's form) puts its step between the actors offset's and i's.
// Set instead as &work->particles[i] at the top of the drawing branch,
// before the target's lookup, it becomes a conditionally set induction
// value: loop.c reduces it after the actors offset (36, then work, at the
// head; 2, 28, then 1 at the tail) and, past the side test's label, keeps
// its read at [marker, #24]. Set every iteration or after the lookup, the
// reduction points at variant instead (differ 31).

// ☀️ twin: BattleEffect_RunCurse
// (games/tbs/src/battle_effect/battle_effect/target_motes.c). ⚓️'s changes:
// the effect argument read directly (the dispatcher stores work->effect), the
// star cross sheet (directory row 0x13b), the blitter from
// BattleEffect_LoadBlitter(104, 0x23) (transparent, ⚓️ mode 2), the transfer
// callback at 0xc80 and the marker taken by index. Both need the family's
// blitter pair for the frame: as a single DrawRectangle the stack frame is 4
// bytes short (differ 170).

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
// ☀️ ProcessPendingGraphicsTransfer
void BattleEffect_ProcessPendingGraphicsTransfer(void);
// ☀️ BattleEvent_StartPhaseFar
void Func_081180e8(s32 phase);
// ☀️ BattleEffect_BuildBlitter
s32 BattleEffect_LoadBlitter(s32 slot, s32 flags);
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
  BattleEffect_LoadBlitter(HEAP_BLITTER, BLITTER_TRANSPARENT << 4 |
                                             COMPOSITE_CLIP_X |
                                             COMPOSITE_CLIP_Y);
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
      do {
        start = i << 4;
        if (frame == start) {
          Sound_PlayFar(143);
        }
        if (frame >= start && frame < start + MARKER_FRAMES) {
          // The post under the target, then the star, six frames a cel.
          marker = &work->particles[i];
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

// Matches: differ 0 (tla-en, 2026-11-08)
// Two tagged devices, both on ⚓️'s allowance: the fill wrapper (4 without it)
// and Nothing(), an s32 inline that returns nothing (2 without it).
// Round 4 (2026-11-08): why the flags move needs Nothing(). In block 5 the
// stack store of the mode argument holds the unit for two cycles; at the next
// pick it is still sched2's last insn, and rank_for_schedule puts the r3 move
// (anti-dependent on the store through r3, cost 0) in class 2 below the
// independent r0, r1 and r2 moves, so r1 goes first. An insn with no unit
// issues in that stall and takes the store's place as the last insn; the four
// moves then tie on class, and the r3 move's five dependants against r1's and
// r2's three and r0's two give the native's r3, r1, r2, r0. Reload keeps one
// such insn only: a clobber of the return register, left where an inline
// function with a return type falls off its end and its value is ignored.
// Nothing() called between the flags and the call leaves exactly that; the
// sched2 dump (diff --rtl) shows it issued on no unit at t=2. Tried for it:
// an s32 wrapper around the blitter build falling off its end (2: its clobber
// follows the call and waits on it), the pair initialized in a block after the
// call, { draw, 0 } (11: the clobber is a pseudo's, deleted by reload, and r7's
// zero stays), the fill through a local pointer in place of its wrapper (4).
// Every edition has these bytes but for relocations: tla-de, tla-es, tla-fr
// and tla-it at 0818c7b8 differ from tla-en in 4 bytes (two bl offsets, at
// +0x82 Resource_GetTableEntry and +0xf4, and two pool words), tla-ja at
// 0818c7bc in bl offsets, jump-table entries and pool words only; the diff
// tool measures none of them (their scaffold sections carry no Thumb marker).
// Round 3 (2026-11-08): the native emits the flags move (r3) ahead of r1 and
// r2; here all four argument moves are expanded r0, r1, r2, r3 and tie. Tried
// u32, u8 and u16 flags, narrow and unsigned parameters, a void return, a
// shift local, a mode local, a flags copy, once-only blocks around and after
// the call, the fill wrapper dropped (6); permute 6 min: nothing below 2.
// 2026-11-07: Iwram_FillWords through an inline wrapper (as the Broken Seal's
// iwram_call.h) fixes the fill's order, 6 to 2; left: the slot 104 argument
// is loaded first before Func_08196404 (the native loads it last: r3, r1, r2,
// r0); all four candidates have priority 9 and sched2 breaks the tie by LUID,
// so the native's r0 move came after the others in its RTL. Tried:
// slot types, a slot local, an inline call wrapper, per-branch calls, argument
// spellings; alchemy-devtools permute (6 min, 4 jobs): nothing below 2.
// 2026-11-08: also a function-scope slot local used by both calls, a mode
// local (as f16ec58.c), flags types and declaration places, flags |=, an
// unprototyped call; permute 8 min: nothing below 2. Func_08196404 returns s32,
// as the adopted files declare it (no change).
// Earlier: differ 16 to 6 over 2026-10-15 to 11-05 (the pooled-store device,
// since dropped, was no longer needed). Allocation and frame match (effect
// spilled to sp+8 because the blitter pair holds r6 and r7; the cell buffer
// through a pointer the build rematerializes).
#include "types.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "battle_effect.h"
#include "battle_effect_work.h"

// The fill goes through an inline wrapper, as the Broken Seal's iwram_call.h
// writes it: called straight through the fixed entry, the build loads 0x4000
// before the routine's address.
#undef Iwram_FillWords
static inline s32 Iwram_FillWords(void *destination, s32 size, u32 value) {
  /* FAKEMATCH(wrapper): the call through this inline; as a direct call to
   * Iwram_FillWordsEntry, 4 of 303 instructions differ in tla-en. */
  return Iwram_FillWordsEntry(destination, size, value);
}

// Does nothing and gives no value: inlined with its value ignored, it leaves
// only a clobber of the return register, an insn with no code.
static inline s32 Nothing(void) {}

// The Lost Age's heap slots, by byte offset into Ram_HeapSlots.
#define HEAP_SLOT_BLITTER 104

// The transfer task runs at this priority, where ☀️'s runs at 0x480.
#define TASK_PASS_FRAME 0xc80

// ⚓️'s blitter modes count the other way from ☀️'s COMPOSITE_* modes.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT, BLIT_COPY };

struct EffectPosition;

extern char ResourceId_GreyCrescentSheet;
extern char ResourceId_StarCrescentSheet;
extern char ResourceId_MarsDjinnSheet;

void BattleEffect_StartTiledCanvas(s32 bgControl);
s32 Func_08196404(s32 slot, s32 widthShift, s32 heightShift, s32 flags,
                  s32 mode);
void *Resource_GetTableEntry(s32 id);
s32 Task_Add(void (*func)(void), s32 order);
s32 Task_Remove(void (*func)(void));
void BattleMotion_ApproachTargetFar(s32, s16, s32, s32);
void EffectPosition_ProjectBattler(s32 id, struct EffectPosition *position);
void Sound_PlayFar(s32 cue);
void Func_081180e8(s32 phase);
void BattleEffect_StartActorMotion(s32, s32, s32, s32, s32, s32);
void Func_08158ce0(s32 randomMask, u32 shakeRange);
void BattleEffect_CountDownBattlerTints(void);
void wait(s32 frames);
void HeapSlot_Release(s32 slot);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_ProcessPendingGraphicsTransfer(void);

// The crescent slash of Human Hunt (kind 437, mode 0) and Echo Cut (kind 390,
// mode 1). The caster closes in on the target; from frame 8 a crescent is cut
// in five steps of three frames: the star crescent, a blank frame, the two
// halves of the grey crescent and the star crescent's last cell. Echo Cut
// cuts it upside down and higher, and throws the target into the air at
// frame 12. Human Hunt loads the Mars Djinn's colours into the background
// palette and blends its slash more faintly.
void Func_0818c7b8(struct BattleEffectArgument *effect, s32 mode) {
  struct BattleEffectWork *work =
      (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  u8 *canvas = Ram_HeapSlots->battleFxWork[1];
  u8 *cells = Ram_MapCellBuffer;
  // The blitter pair, as a two-word array one DImode pseudo: it takes r6 and
  // r7, the second unused, which is why the native pushes r7 and spills
  // effect.
  DrawRectangle draw[2];
  s32 flags;
  s32 frame;
  s32 position[3];

  // Set up the canvas and a clipped MAX blitter, mirrored to face the target
  // and flipped upside down for Echo Cut.
  BattleEffect_StartTiledCanvas(0);
  if (mode == 0) {
    flags = COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y;
    if (effect->side != 0)
      flags = COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X;
  } else {
    flags = COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP_Y;
    if (effect->side != 0)
      flags = COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP;
  }
  /* FAKEMATCH(signature): Nothing() typed s32 though it returns nothing; its
   * return-register clobber issues in the mode store's second cycle, so the
   * flags move is picked before r1 and r2; without the call, 2 of 303
   * instructions differ in tla-en. */
  Nothing();
  Func_08196404(HEAP_SLOT_BLITTER, 7, 7, flags, BLIT_MAX);
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];

  // Load both crescents and set the blend.
  Resource_LoadAndDecompress((s32)&ResourceId_GreyCrescentSheet, cells, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_StarCrescentSheet, work->sheet, 1,
                             0);
  if (mode == 0) {
    Iwram_CopyWords((void *)0x05000000,
                    Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet),
                    128);
    REG_BLDALPHA = 0x810;
    work->transferMode = 2;
    work->transferValue = 50;
  } else {
    REG_BLDALPHA = 0x1010;
    work->transferMode = 2;
    work->transferValue = 75;
  }
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  // The caster closes in on the target.
  BattleMotion_ApproachTargetFar(effect->actor, effect->actors[0], 8, 0x40000);

  for (frame = 0; frame != 32; frame++) {
    if (frame == 0)
      EffectPosition_ProjectBattler(effect->actors[0],
                                    (struct EffectPosition *)position);
    if (frame == 8)
      Sound_PlayFar(212);

    // Frame 12: the hit. Echo Cut throws the target up.
    if (frame == 12) {
      if (mode == 1)
        Func_081180e8(144);
      else
        Func_081180e8(134);
      BattleEffect_StartActorMotion(effect->actors[0], 1, 0x40000, 0x100000,
                                    0x40000, 250);
      work->shakeFrames = 4;
    }

    // From frame 8: the slash, one step every three frames.
    if (frame > 7) {
      s32 step = (frame - 8) / 3;
      s32 y = mode != 0 ? 16 : 0;
      s32 x;

      if (effect->side == 0)
        x = position[0] / 2 - 48;
      else
        x = position[0] / 2 - 80;
      if (mode == 0) {
        switch (step) {
        case 0:
          draw[0](canvas, work->sheet, x, y, 120, 120);
          break;
        case 1:
          Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
          break;
        case 2:
          draw[0](canvas, cells, x - 4, y, 128, 128);
          break;
        case 3:
          draw[0](canvas, cells + 0x4000, x - 4, y, 128, 128);
          break;
        case 4:
          draw[0](canvas, work->sheet + 0x3840, x, y, 120, 120);
          break;
        }
      } else {
        switch (step) {
        case 1:
          Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
          break;
        case 0:
          draw[0](canvas, work->sheet, x, y, 120, 120);
          break;
        case 2:
          draw[0](canvas, cells, x - 4, y - 8, 128, 128);
          break;
        case 3:
          draw[0](canvas, cells + 0x4000, x - 4, y - 8, 128, 128);
          break;
        case 4:
          draw[0](canvas, work->sheet + 0x3840, x, y, 120, 120);
          break;
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

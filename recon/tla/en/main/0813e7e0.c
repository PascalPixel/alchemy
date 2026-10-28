// Near miss, differ 4 of 376 (2026-10-25, under -mthumb-slow-loads).

// No twin: an unused ⚓️ function that no handler or caller reaches. Its
// set-up is 0813eb70's (The Broken Seal's
// BattleEffect_RunShatteringOpening): the same BG2 canvas, tile map,
// clears and blitters. After running Func_08191530 on the effect it holds
// the battle background on screen, fading its colours up from black over 64
// frames, until A is pressed; B starts the fade again. It reads as a viewer
// left in from development.

// Left: two rows where alchemy-gcc issues a load one slot earlier than the
// native: the background view's load before the effect argument's copy
// (mov r2, fp), and the scroll's store before view->secondMode's.

// Tried:
// from the listing on 0813eb70's set-up                      46
// the held keys read through a volatile (tagged): the native
// reloads them for the second test                         36
// under -mthumb-slow-loads, the two devices tagged below          4

#include "types.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "mem.h"
#include "io_write_queue.h"
#include "input.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "battle_work.h"
#include "battle_display.h"
#include "bg_scroll.h"
#include "iwram_call.h"

// Heap slots, as byte offsets into Ram_HeapSlots.
#define SLOT_BATTLE_EFFECT 92      // battleFxWork[0]
#define SLOT_BATTLE_CANVAS 96      // battleFxWork[1]
#define SLOT_BATTLE_SHEET 100      // unk64[0]
#define SLOT_BLITTER 104           // unk64[1]
#define SLOT_BLITTER_ALTERNATE 188 // unkb8[1]

// The whole effect work block: struct BattleEffectWork names only its first
// 0x78f4 bytes.
#define BATTLE_EFFECT_WORK_SIZE 0x7b7c

// ⚓️'s blitter modes run the other way from The Broken Seal's.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT, BLIT_COPY };
#define BLITTER(mode, flags) ((mode) << 4 | (flags))
#define CLIP (COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y)

#define TASK_PASS_FRAME 0xc80

// The battle background's id.
#define BACKGROUND(battle) (*(u16 *)(battle)->unk670)

void BattleEffect_ClearActorTimers(void); // BattleEffect_ClearActorTimers
void Func_08191530(struct BattleEffectArgument *effect);
s32 BattleEffect_LoadBlitter(s32 slot, s32 flags);
s32 Task_Add(void *callback, s32 order);
s32 Task_Remove(void *callback);
void Runtime_ReleaseHeapBlock(s32 slot);
void BattlePresentation_ConfigurePaletteFadeFar(s32 mode, s32 background,
                                                s32 value);
void BattlePres_SetupTransitionSceneFar(s32 a, s32 b, s32 c, s32 d);
void BattleBackground_LoadFar(s32 layer, s32 background, s32 palette);
void Func_08164b2c(s32 redDelta, s32 greenDelta, s32 blueDelta);
void Func_0814cd48(s32 actor, s32 palette, s32 frames, s32 timerSlot,
                   s32 timerFrames); // ObjectGroup_UpdateMembers
void ObjectGroup_CountDownMemberTimers(void);            // ObjectGroup_CountDownMemberTimers
void WaitFrames(s32 frames);

// The IWRAM routines are called through one inline each, as The Broken
// Seal's iwram_call.h layer calls them.
static inline s32 ClearWords(void *destination, s32 size) {
  return Iwram_ClearWords(destination, size);
}

static inline s32 FillWords(void *destination, s32 size, u32 value) {
  return Iwram_FillWords(destination, size, value);
}

static inline s32 CopyWords(void *destination, const void *source, s32 size) {
  return Iwram_CopyWords(destination, source, size);
}

// Shows the battle background fading in, until A (unused).

void Func_0813e7e0(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  struct BattleWork *battle;
  struct BattleBackgroundView *view;
  s32 frame;
  s32 i;
  s32 j;
  s32 k;

  work = Runtime_AllocateHeapBlock(SLOT_BATTLE_EFFECT, BATTLE_EFFECT_WORK_SIZE);
  canvas = Runtime_AllocateHeapBlock(SLOT_BATTLE_CANVAS, 0x4000);
  /* FAKEMATCH(barrier): the canvas pointer's spill is stored before the heap
   * base starts; as plain C, 6 of 376 instructions differ in tla-en. */
  __asm__ volatile("");
  battle = Ram_HeapSlots->battleWork;
  view = (struct BattleBackgroundView *)Ram_HeapSlots->transitionWork;
  work->effect = effect;
  work->vblanksSinceTransfer = 1;
  Func_08191530(effect);

  // Make BG2 a 128x128 canvas, as BattleEffect_StartCanvasLayer does.
  BattleEffect_ClearActorTimers();
  view->secondMode = 1;
  gBgScroll[1].y = 32;
  BattlePresentation_ConfigurePaletteFadeFar(1, BACKGROUND(battle), 0);
  REG_BG2CNT = 0x784;
  BattlePres_SetupTransitionSceneFar(0, 0, 0, 100);
  view->secondMode = 0;
  REG_BG2X = 0;
  REG_BG2Y = 0xfffff000;
  REG_BG2PA = 0x80;
  REG_BG2PB = 0;
  REG_BG2PC = 0;
  REG_BG2PD = 0x100;
  REG_WIN0H = 0xf0;
  REG_WIN0V = 0x1088;
  REG_WIN1H = 0xf0;
  REG_WIN1V = 0x1088;
  REG_WININ = 0x3537;
  REG_WINOUT = 0x3f21;

  i = 0;
  for (k = 0; k != 16; k++) {
    for (j = 0; j != 8; j++, i += 2) {
      s32 entry;

      entry = (s16)(((k * 16 + j * 2 + 1) << 8) | ((k * 8 + j) * 2));
      *(volatile u16 *)(0x06003800 + i) = *(u16 *)&entry;
    }
  }

  ClearWords(canvas, 0x4000);
  ClearWords((void *)0x06004000, 0x4000);
  IO_WRITE_QUEUE_PUSH(0x04000000, 0x7741, 0x20000);
  REG_BLDALPHA = 0x1010;
  REG_BLDCNT = 0;
  work->transferMode = 1;
  work->transferValue = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  BattleEffect_LoadBlitter(SLOT_BLITTER, BLITTER(BLIT_TRANSPARENT, CLIP));
  BattleEffect_LoadBlitter(SLOT_BLITTER_ALTERNATE, BLITTER(BLIT_MAX, CLIP));

  // Keep the picture at 0x06008000 aside, show the battle background with
  // its colours black, and flash the first target.
  CopyWords(Ram_MapCellBuffer, (void *)0x06008000, 0x7800);
  FillWords(Ram_MapCellBuffer, 0x7800, 0x01010101);
  work->savedBg1X = gBgScroll[1].x;
  work->savedBg1Y = gBgScroll[1].y;
  BattleBackground_LoadFar(1, BACKGROUND(battle), -1);
  Func_08164b2c(-31, -31, -31);
  Func_0814cd48(effect->actors[0], 7, 3, 0, 30);

  // Fade the colours up a shade every two frames; B starts again, A ends.
  for (frame = 0; frame != 32096; frame++) {
    /* FAKEMATCH(volatile): the held keys are read afresh for each test, as
     * the native reads them; as gInput.held, 14 of 376 instructions differ in
     * tla-en. */
    if (*(volatile u32 *)&gInput.held & 2)
      frame = 0;
    if (*(volatile u32 *)&gInput.held & 1)
      break;
    if (frame <= 63)
      Func_08164b2c(frame / 2 - 31, frame / 2 - 31, frame / 2 - 31);
    ObjectGroup_CountDownMemberTimers();
    work->transferPending = 1;
    WaitFrames(1);
  }

  // Put the background and the display back.
  Runtime_ReleaseHeapBlock(SLOT_BLITTER_ALTERNATE);
  Runtime_ReleaseHeapBlock(SLOT_BLITTER);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  Func_0814cd48(effect->actors[0], -1, 1, -1, 0);
  gBgScroll[1].x = work->savedBg1X;
  gBgScroll[1].y = 32;
  BattlePresentation_ConfigurePaletteFadeFar(2, BACKGROUND(battle), 0);
  WaitFrames(1);
  IO_WRITE_QUEUE_PUSH(0x04000000, 0x7541, 0x20000);
  Runtime_ReleaseHeapBlock(SLOT_BATTLE_SHEET);
  Runtime_ReleaseHeapBlock(SLOT_BATTLE_CANVAS);
  Runtime_ReleaseHeapBlock(SLOT_BATTLE_EFFECT);
}

// Near miss, differ 2 of 280 (10 without its devices).

// Twin: The Broken Seal's BattlePresentation_PrepareScene, the start of the
// light-streak backdrop the scenes raise behind the battle (Func_0813fd40,
// already C, takes it down; both are reached through the far-call table,
// table_47.s). ⚓️ rebuilt it: instead of BattleFx_BeginCanvasLayer it sets
// the BG2 canvas up step by step, a frame at a time, as 0813eb70 does; it
// fades the battle background's colours over 24 frames (Func_0813fa44), and
// the streaks are halved into VRAM each frame (Func_0813f89c) rather than
// through the canvas transfer.

// Left: one row in the prologue. The native stores the canvas pointer to its
// stack slot before it starts the heap base for the battle and view reads;
// here the base's movs comes first.

// The other eight rows were three values the native loads from the pool
// before it starts their register's address (BLDCNT, BG2CNT, BG2PA), the
// same rows BattleFx_OpenCanvasLayer (canvas.c) closes with pinned values
// behind barriers. They are closed the same way here, tagged; S2's counts
// of pins and barriers may only fall, so adoption waits on Pascal (plan
// D2), and the last row in any case.

// Tried:
// from the listing, on canvas.c's set-up                     10
// BG2CNT's value set in a one-pass block: it becomes a split
// constant (movs, lsls, adds)                              29
// the three pins and barriers as in canvas.c                  2
// one-pass blocks for the last row: around the canvas
// allocation 2 (the work's copy then moves), empty after it
// 2, around the battle and view reads 4, around both
// allocations 6

#include "types.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "mem.h"
#include "io_write_queue.h"
#include "battle_work.h"
#include "battle_display.h"
#include "bg_scroll.h"
#include "iwram_call.h"
#include "irqlock.h"
#include "resource.h"

// Heap slots, as byte offsets into Ram_HeapSlots.
#define SLOT_BATTLE_EFFECT 92 // battleFxWork[0]
#define SLOT_BATTLE_CANVAS 96 // battleFxWork[1]

// The battle background's id.
#define BACKGROUND(battle) (*(u16 *)(battle)->unk670)

// The backdrop's work, in place of the battle effect work.
struct StreakSceneWork {
  s32 frame;        // the streaks' frame count (Func_0813f8d4)
  s32 halvePending; // the streaks are drawn: halve the canvas into VRAM
  s32 fadeFrames;   // frames of the background's fade left (Func_0813fa44)
  s32 fadeStep;
};

extern char ResourceId_YellowPaletteB;
extern char ResourceId_CrystalSheet;
extern char ResourceId_EmberStreakSheet;
extern char ResourceId_VioletPaletteD;
extern char ResourceId_DustPuffSheet;

void Func_0813ba50(void); // BattlePres_ConfigureEffectDisplay
void Func_0814355c(void); // the BG2 canvas's registers
void Func_0813fa44(void); // fades the battle background
void Func_0813f89c(void); // halves the canvas into VRAM
void Func_0813f8d4(void); // BattlePresentation_DrawStreaks
s32 Scheduler_AddOrUpdateCallback(void *callback, s32 order);
void BattlePresentation_ConfigurePaletteFadeFar(s32 mode, s32 background,
                                                s32 value);
void BattlePres_SetupTransitionSceneFar(s32 a, s32 b, s32 c, s32 d);
void WaitFrames(s32 frames);

// The IWRAM routines are called through one inline each, as The Broken
// Seal's iwram_call.h layer calls them.
static inline s32 ClearWords(void *destination, s32 size) {
  return Iwram_ClearWords(destination, size);
}

// Raises the light-streak backdrop in colour set kind (yellow, crystal,
// ember, violet or dust).

void Func_0813fa88(s32 kind) {
  struct StreakSceneWork *work;
  void *canvas;
  struct BattleWork *battle;
  struct BattleBackgroundView *view;
  s32 id;
  s32 i;
  s32 j;
  s32 k;

  work = Runtime_AllocateHeapBlock(SLOT_BATTLE_EFFECT, sizeof *work);
  canvas = Runtime_AllocateHeapBlock(SLOT_BATTLE_CANVAS, 0x4000);
  battle = Ram_HeapSlots->battleWork;
  view = (struct BattleBackgroundView *)Ram_HeapSlots->transitionWork;

  // Turn BG2 into a 128x128 canvas, one step a frame.
  Func_0813ba50();
  view->secondMode = 1;
  WaitFrames(1);
  {
    /* FAKEMATCH: as in BattleFx_OpenCanvasLayer (canvas.c), whose start this
     * is: the native loads the zero from the pool before it starts the
     * register address, pinned to the r3 it takes behind a barrier that holds
     * r2 (differ 4 plainly). */
    register u16 zero asm("r3") = 0;
    struct IoWriteQueue *q;
    u32 saved;
    s32 count;
    u16 *count_;

    /* FAKEMATCH: the barrier of the pin above. */
    __asm__("" : "+l"(zero) : : "r2");
    /* FAKEMATCH: IO_WRITE_QUEUE_PUSH written out, so that the queue's
     * address is taken between the halves of the register address, as in
     * canvas.c. */
    q = &gIoWriteQueue;
    count_ = &q->count;
    REG_BLDCNT = zero;
    IRQ_LOCK(saved);
    count = *count_;
    if (count <= 31) {
      u32 *entry = q->entries[count];

      *count_ = count + 1;
      *entry++ = 0x1741;
      *entry++ = (u32)&REG_DISPCNT;
      *entry = 0x20000;
    }
    IRQ_UNLOCK(saved);
  }
  gBgScroll[1].y = 32;
  WaitFrames(1);
  BattlePresentation_ConfigurePaletteFadeFar(1, BACKGROUND(battle), 0);
  IO_WRITE_QUEUE_PUSH(0x04000000, 0x7341, 0x20000);
  WaitFrames(1);
  {
    /* FAKEMATCH: the native loads the control word from the pool before it
     * starts the register address; plainly the address starts first
     * (differ 2). Pinned to the r2 it takes, behind a barrier that holds r3,
     * as BattleFx_OpenCanvasLayer (canvas.c) does. A one-pass block cannot:
     * the value as a local of its own is a split constant. */
    register u16 control asm("r2") = 0x784;

    /* FAKEMATCH: the barrier of the pin above. */
    __asm__("" : "+l"(control) : : "r3");
    REG_BG2CNT = control;
  }
  IO_WRITE_QUEUE_PUSH(0x04000000, 0x7341, 0x20000);
  BattlePres_SetupTransitionSceneFar(0, 0, 0, 100);
  view->secondMode = 0;
  WaitFrames(1);
  Func_0814355c();
  {
    /* FAKEMATCH: the native loads the scale from the pool before it starts
     * the register address (differ 2 plainly). Pinned to the r3 it takes,
     * behind a barrier that holds r2. */
    register u16 scale asm("r3") = 0x100;

    /* FAKEMATCH: the barrier of the pin above. */
    __asm__("" : "+l"(scale) : : "r2");
    REG_BG2PA = scale;
  }
  REG_WININ = 0x3537;
  IO_WRITE_QUEUE_PUSH(0x04000000, 0x7741, 0x20000);

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
  WaitFrames(1);

  switch (kind) {
  case 0:
    id = (s32)&ResourceId_YellowPaletteB;
    break;
  case 1:
    id = (s32)&ResourceId_CrystalSheet;
    break;
  case 2:
    id = (s32)&ResourceId_EmberStreakSheet;
    break;
  case 3:
    id = (s32)&ResourceId_VioletPaletteD;
    break;
  case 4:
  default:
    id = (s32)&ResourceId_DustPuffSheet;
    break;
  }
  Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(id), 128);

  // Fade the background over 24 frames, and draw and halve the streaks
  // every frame.
  work->fadeFrames = 24;
  work->fadeStep = 0;
  Scheduler_AddOrUpdateCallback(Func_0813fa44, 0x480);
  work->halvePending = 0;
  Scheduler_AddOrUpdateCallback(Func_0813f89c, 0xc80);
  work->frame = 0;
  Scheduler_AddOrUpdateCallback(Func_0813f8d4, 0x480);
}

// Near miss, differ 2 of 55 with a barrier before each store: the native runs
// in source order throughout, as if nothing were scheduled; only the first
// store still differs, where the native reads the value before it starts the
// registers' address (ldrh r3, [r3]; movs r1, #128). A temporary with a barrier
// there moves the value to r1 (differ 14); volatile registers or a volatile
// work pointer change nothing; a block that runs once around each store starts
// the address first (differ 18). 08143424 and 08143460 are the same family.
#include "types.h"
#include "ram_buffer.h"

struct DisplayWork {
  u8 unk0000[0x77bc];
  u16 win0h;
  u16 win0v;
  u16 win1h;
  u16 win1v;
  u16 winin;
  u16 winout;
  u16 dispcnt;
  u16 bldcnt;
  u16 bldalpha;
  u16 padding;
  s32 bg2x;
  s32 bg2y;
};

#define REG_WIN0H (*(volatile u16 *)0x04000040)
#define REG_WIN1H (*(volatile u16 *)0x04000042)
#define REG_WIN0V (*(volatile u16 *)0x04000044)
#define REG_WIN1V (*(volatile u16 *)0x04000046)
#define REG_WININ (*(volatile u16 *)0x04000048)
#define REG_WINOUT (*(volatile u16 *)0x0400004a)
#define REG_DISPCNT_PLAIN (*(volatile u16 *)0x04000000)
#define REG_BLDCNT (*(volatile u16 *)0x04000050)
#define REG_BLDALPHA (*(volatile u16 *)0x04000052)

void Func_081433b4(void) {
  struct DisplayWork *work =
      (struct DisplayWork *)Ram_HeapSlots->battleFxWork[0];

  /* FAKEMATCH: the native keeps every step in source order: it loads the work
   * pointer before it starts the first offset, and steps the shared offset
   * (adds r0, #2) only after each store. Without the barriers the scheduler
   * starts each constant first (differ 18). */
  /* FAKEMATCH: keeps this store in source order, as above. */
  __asm__ volatile("");
  REG_WIN0H = work->win0h;
  /* FAKEMATCH: keeps this store in source order, as above. */
  __asm__ volatile("");
  REG_WIN0V = work->win0v;
  /* FAKEMATCH: keeps this store in source order, as above. */
  __asm__ volatile("");
  REG_WIN1H = work->win1h;
  /* FAKEMATCH: keeps this store in source order, as above. */
  __asm__ volatile("");
  REG_WIN1V = work->win1v;
  /* FAKEMATCH: keeps this store in source order, as above. */
  __asm__ volatile("");
  REG_WININ = work->winin;
  /* FAKEMATCH: keeps this store in source order, as above. */
  __asm__ volatile("");
  REG_WINOUT = work->winout;
  /* FAKEMATCH: keeps this store in source order, as above. */
  __asm__ volatile("");
  REG_DISPCNT_PLAIN = work->dispcnt;
  /* FAKEMATCH: keeps this store in source order, as above. */
  __asm__ volatile("");
  REG_BLDCNT = work->bldcnt;
  /* FAKEMATCH: keeps this store in source order, as above. */
  __asm__ volatile("");
  REG_BLDALPHA = work->bldalpha;
}

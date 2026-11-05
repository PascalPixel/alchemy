// Draft, differ 16 of 156 (tla-en, 2026-11-05). Older notes below may predate this figure.
// Measured under -Os, 5 November 2026: differ 26 (tla-en); notes below may predate it.
// Draft, differ 85 of 156, skeleton gap 9 (tla-en, 2026-11-01)
// Func_02000540's twin for the palettes at 0x05000060 and 0x05000080 (the
// listing's 160 << 19 plus 96 and 128; earlier headers said 0x05000160 and
// 0x05000180, which is wrong), with its own buffers.
// Left: one cause, then its cascade.
// - The native reads the counter again at the join after the if; here gcse's
//   PRE deletes that read (the --rtl gcse dump: "redundant insn" in bb 4, "edge
//   (3,4), copy expression") and leaves movs r3, #0. Without the read
//   &gData_020024c4 loses refs, so r7 and r8 swap: the zero kept for the store
//   sits in r7, not r8. Tried and no help: a local pointer for the counter in
//   any mix of reads and the store (differ 82 at best, but it moves the address
//   load to the function's top, gap 13); `busy == 0 &&` in the test (the jump
//   is threaded before cse runs); `+ 1 == 0`. Only a differing address pseudo or
//   alias set at gcse would stop it, as cse_end_of_basic_block skips the block;
//   a label inside the block would do that, and nothing in the listing shows one.
// - The byte's ldrb, lsls, asrs is not a source form: it is *thumb_extendqisi2's
//   output when the destination is the base register, so it follows once the
//   zero gets r8 and an output reload to r3. `(s8)*(u8 *)` compiles the same.
// - Measured, not kept: extern volatile s32 gData_020024c4 gives differ 46, gap
//   6, top of the function all but matched (a new device, so left out; sibling
//   Func_020006bc carries it). Left there: the eighth count sits in r1 where the
//   native has r2 (`phase = x / 8 % 4` restores r2 but puts phase in r4), and
//   the four addresses take r5, r7, lr, ip where the native takes r7, r6, r8, ip,
//   with lr unused.
// Settled here: the copies' addresses are locals set after the phase, which is
// what lets the queue reuse them; offset is a byte offset, as a[12 + j] on a
// pointer makes (a + j * 2) + 24 and a[j] with j + 12 adds before the shift,
// while the native shifts, adds 24 and stores to [base, offset]. Written with
// the arrays directly in the loop and locals only for the queue, gcse's edge
// insert gives the loop two entries and loop.c ignores it (differ 63 with the
// volatile counter). The stored zero needs no local: cse reuses the register
// the byte test left holding 0.
#include "types.h"
#include "call.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "io_write_queue.h"
#include "system/io_reg.h"

extern s32 gData_020024c4;
extern u16 Data_02002958[], Data_02002978[], gData_02002998[], gData_020029b8[];

void Func_020003e8(void) {
  u8 *work = Ram_HeapSlots->eventWork;
  s32 phase, k, offset;
  u16 *firstCopy, *secondCopy;

  if (*(s16 *)(work + 0x166) != 0)
    return;
  if (*(s8 *)(work + 0xca4) != 0)
    return;
  if (gData_020024c4 == -1) {
    Call3((void (*)())Iwram_CopyWords, Data_02002958, (void *)0x05000060, 32);
    Iwram_CopyWords(Data_02002978, Data_02002958, 32);
    Iwram_CopyWords(gData_02002998, (void *)0x05000080, 32);
    Iwram_CopyWords(gData_020029b8, gData_02002998, 32);
    gData_020024c4 = 0;
  }
  phase = gData_020024c4 / 8;
  /* FAKEMATCH(one-pass): the statement in a once-only block, whose end holds the
   * order around it; as the plain statement, 24 of 156 instructions differ
   * in tla-en. */
  do {
    phase %= 4;
  } while (0);
  /* FAKEMATCH(barrier): an empty asm holds the order around it; as no asm,
   * 20 of 156 instructions differ in tla-en. */
  asm("");
  firstCopy = Data_02002978;
  secondCopy = gData_020029b8;
  for (k = 0; k <= 3; k++) {
    offset = (phase + k) % 4 * 2 + 24;
    *(u16 *)((u8 *)firstCopy + offset) = Data_02002958[12 + k];
    *(u16 *)((u8 *)secondCopy + offset) = gData_02002998[12 + k];
  }
  IO_WRITE_QUEUE_PUSH(0x05000060, (u32)firstCopy, 0x84000008);
  IO_WRITE_QUEUE_PUSH(0x05000080, (u32)secondCopy, 0x84000008);
  gData_020024c4++;
}

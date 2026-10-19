// Draft, differ 2 of 17: The Broken Seal's GameFlag_SetNibble as it is. The
// native shifts the byte index (lsrs r6, r3, #23) before it loads the mask 15
// (movs r5, #15); here sched2 ties the two at priority 38 with four
// dependants each and takes the mask by its earlier place in the RTL. A
// two-cycle-plus load latency for the ldrb the index feeds would order them as
// the native. Measured: the statements in other orders and an index
// temporary (differ 18 to 29), an operand barrier on the index (2); two
// minutes of permuter found nothing. 2026-10-18: the mask set after an
// empty asm that follows the index shift orders the pair but the 15 and the
// flag bytes trade r4 and r5 (differ 18), and pinning the bytes to r4 then
// moves their load down (14).
#include "types.h"

extern u8 gGameFlagBytes[];

void GameFlag_SetNibble(s32 flag, s32 value) {
  s32 fieldMask = 0xF;
  s32 shift = 4 & flag;
  s32 mask = fieldMask << shift;
  u8 *bytes = gGameFlagBytes;

  flag = ((u32)flag << 0x14) >> 0x17;
  bytes[flag] = (bytes[flag] & ~mask) | ((value & fieldMask) << shift);
}

// Draft, differ 2 of 32: the CRC-16 (polynomial 0x1021, seed 0xFFFF,
// inverted) written as the listing's loops, both counted up and tested at the
// bottom, with the polynomial's add kept inside the bit loop, as the listing
// reloads it each pass. Left: one reordered pair, the listing loads the
// polynomial (ldr r7) between the movs and lsls that build 0x8000; the draft
// loads it after both. Tried: the plain for loops of the first draft (29), no
// r5 pin (12: count and the byte index swap r5 and r6), the polynomial
// assigned first (2), next before high (4), j++ after the sum (4), the sum
// and the shift as if/else either way (12, 2), an in-place shift with
// -= 0x1021 or a conditional expression (31), an empty asm after the if (2),
// and alchemy permute (best an empty do-while after the sum, still 2).
#include "types.h"

u16 Func_0200217c(u32 count, u8 *p) {
  u32 crc;
  /* FAKEMATCH: without the pin count takes r5 and the byte index r6, the
   * reverse of the listing's (differ 12). */
  register u32 i asm("r5");
  u32 j;

  crc = 0xFFFF;
  i = 0;
  if (count != 0) {
  next_byte:
    crc ^= *p << 8;
    j = 0;
  next_bit:
    {
      u32 high = crc & 0x8000;
      u32 next = crc << 1;

      j++;
      crc = next + 0xFFFFEFDF;
      if (high == 0)
        crc = next;
    }
    if (j != 8)
      goto next_bit;
    i++;
    p++;
    if (i != count)
      goto next_byte;
  }
  return ~crc;
}

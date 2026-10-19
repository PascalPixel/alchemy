#include "types.h"

// The CRC-16 of length bytes at p (polynomial 0x1021, seed 0xffff, the
// result inverted): the clear screen checks each decoded block with it,
// over all but the block's last two bytes. The loops are the listing's,
// counted up and tested at the bottom.
u16 Func_0200217c(u32 length, u8 *p) {
  /* FAKEMATCH(pin): the count in r6; as a plain parameter, 10 of 32
   * instructions differ in tla-en. */
  register u32 count asm("r6") = length;
  u32 crc;
  /* FAKEMATCH(pin): the byte index in r5; as a plain local, 10 of 32
   * instructions differ in tla-en. */
  register u32 i asm("r5");
  u32 j;

  crc = 0xffff;
  i = 0;
  if (count != 0) {
  next_byte:
    crc ^= *p << 8;
    j = 0;
  next_bit:
    // One bit: shift, adding the polynomial when the top bit was set.
    {
      /* FAKEMATCH(pin): the polynomial's add, 0xffffefdf, loaded into r7
       * between the halves of 0x8000; as a plain local, 2 of 32
       * instructions differ in tla-en. */
      register u32 poly asm("r7") = 0xffffefdf;
      u32 high = crc & 0x8000;
      u32 next = crc << 1;

      j++;
      crc = next + poly;
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

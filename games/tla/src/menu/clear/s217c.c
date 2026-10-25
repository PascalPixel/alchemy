#include "types.h"

/// The CRC-16 of length bytes at p (polynomial 0x1021, seed 0xffff, the
/// result inverted): the clear screen checks each decoded block with it,
/// over all but the block's last two bytes. The loops are the listing's,
/// counted up and tested at the bottom.
u16 Func_0200217c(u32 length, u8 *p) {
  u32 crc;
  /* FAKEMATCH(pin): the byte index in r5; as a plain local, the index and
   * length swap r5 and r6, and 5 of 32 instructions differ in tla-en. */
  register u32 i asm("r5");
  u32 j;

  crc = 0xffff;
  i = 0;
  if (length != 0) {
  next_byte:
    crc ^= *p << 8;
    j = 0;
  next_bit:
    // One bit: shift, adding the polynomial when the top bit was set.
    {
      u32 high = crc & 0x8000;
      u32 next = crc << 1;

      j++;
      crc = next + 0xffffefdf;
      if (high == 0)
        crc = next;
    }
    if (j != 8)
      goto next_bit;
    i++;
    p++;
    if (i != length)
      goto next_byte;
  }
  return ~crc;
}

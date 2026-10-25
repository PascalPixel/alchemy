// Draft, differ 106 of 137: turns packed clear data into password
// characters, six bits each with every tenth skipped, and scrambles them.
// Left: register choice and the loops' constants kept inside them.
#include "types.h"

s32 Func_02002068(u8 *src, s32 length, u8 *dst) {
  u8 saved[260];
  s32 i;
  s32 k = 0;
  s32 n = 0;
  s32 shift = 2;
  s32 size = 11;
  u8 *p;
  u8 last;

  if (length != 16) {
    size = 41;
    if (length != 61)
      size = 175;
  }
  for (i = 0; i != size; i++)
    dst[i] = 0;
  for (i = 0; i != length; i++) {
    saved[i] = src[i];
    src[i] = (src[i] - i) & 63;
  }
  p = dst + n;
  for (i = 0; i != length; i++, k++) {
    u8 c;

    if (i % 10 > 8)
      continue;
    c = src[k];
    if (shift < 0) {
      *p += c >> -shift;
      n++;
      p++;
      if (n == size)
        break;
      shift += 8;
    }
    *p += c << shift;
    shift -= 6;
    if (shift == -6) {
      n++;
      shift = 2;
      p++;
      if (n == size)
        break;
    }
  }
  last = dst[n - 1];
  for (i = 0; i != n - 1; i++)
    dst[i] ^= last;
  for (i = 0; i != length; i++)
    src[i] = saved[i];
  return n;
}

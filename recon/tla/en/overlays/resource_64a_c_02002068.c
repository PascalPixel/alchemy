// Draft, differ 83 of 137 (was 106): turns packed clear data into password
// characters, six bits each with every tenth skipped, and scrambles them.
// The clearing and saving loops are goto loops walking a pointer, as the
// native keeps their constants inside them and indexes saved by i (now
// instruction for instruction but registers). Left: register choice (the
// native keeps length in r7, shift in r4, size in r9 and dst in r10; here
// length and shift, size and dst trade places), and p = dst + n, which the
// native computes from n (r8) where gcse folds n's zero here. Tried: dst[n]
// for p (121: n then takes r4).
#include "types.h"

s32 Func_02002068(u8 *src, s32 length, u8 *dst) {
  u8 saved[260];
  s32 i;
  s32 k = 0;
  s32 n = 0;
  s32 shift = 2;
  s32 size = 11;
  u8 *p;
  u8 *q;
  u8 last;
  u8 c;

  if (length != 16) {
    size = 41;
    if (length != 61)
      size = 175;
  }
  i = 0;
  if (size != 0) {
    q = dst;
  clear:
    *q++ = 0;
    if (++i != size)
      goto clear;
  }
  i = 0;
  if (length != 0) {
    q = src;
  save:
    saved[i] = *q;
    *q = (saved[i] - i) & 63;
    q++;
    if (++i != length)
      goto save;
  }
  p = dst + n;
  for (i = 0; i != length; i++, k++) {
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

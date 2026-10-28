#include "edition.h"
#include "types.h"

void Tile_BuildCellPictures(u16 *dst, u16 *src) {
  u32 row;
  u32 col;
  s32 a;
  s32 b;
  s32 c;
  s32 d;
  s32 e;
  s32 f;
  s32 found;
  s32 v;

  for (row = 0; row < 20; row++) {
    d = 0;
    a = d;
    c = d;
    f = d;
    b = f;
    e = f;
    found = f;
    col = f;
    dst++;
    for (; col < 30; col++) {
      u16 cmd = *src++;

      if (cmd == 0)
        continue;
      if (found == 0) {
        switch (cmd) {
        case 0xf010:
          a = col * 8;
          b = a + 8;
          c = a + 1;
          d = a;
          e = d + 7;
          f = b;
          found++;
          break;
        case 0xf013:
          a = col * 8;
          b = a + 8;
          c = a;
          d = a + 1;
          e = b;
          f = a + 7;
          found++;
          break;
        case 0xf007:
        case 0xf016:
        case 0xf01a:
          a = col * 8;
          b = a + 8;
          d = a;
          c = d;
          f = b;
          e = f;
          found++;
        }
      } else {
        switch (cmd) {
        case 0xf012:
          b = col * 8 + 8;
          e = col * 8 + 7;
          f = b;
          break;
        case 0xf015:
          b = col * 8 + 8;
          e = b;
          f = col * 8 + 7;
          break;
        case 0xf007:
        case 0xf017:
        case 0xf01b:
          f = col * 8 + 8;
          b = f;
          e = f;
          break;
        }
      }
    }
    v = e + (c << 8);
    *dst++ = v;
    v = b + (a << 8);
    dst++;
    *dst++ = v;
    dst++;
    *dst++ = v;
    dst++;
    *dst++ = v;
    dst++;
    src += 2;
    *dst++ = v;
    dst++;
    *dst++ = v;
    dst++;
    *dst++ = v;
    v = f + (d << 8);
    dst++;
    *dst++ = v;
  }
}

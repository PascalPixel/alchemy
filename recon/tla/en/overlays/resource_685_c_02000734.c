// Measured under -Os, 5 November 2026: differ 79 (tla-en); notes below may predate it.
// Draft, differ 83 of 97, skeleton gap 15 (tla-en, 2026-11-01): fades five
// background colours, from palette entry 114 (0x050000e4, not the object
// palette an older note gave), from one set to another over the given number
// of frames, one step a frame.
// Left: allocation, and the skeleton follows from it. Listing has c in r5 and
// d in r6, each dying at its own lsls 16, with toGreen in r2, step in r9 and
// offset in r11; here d is in r2, toGreen in r4, step in r11 and offset in r9.
// c and d dying there needs red and toRed masked before the shifts, but those
// orders (red, green, ...; d, toRed, toGreen, ...) measured 112: toGreen then
// takes r2, r4 falls free, green takes it as a caller save, left gets a high
// register and the frame grows a slot. The order below keeps green in r8 and
// left on the stack only because toGreen sits in r4. Green and red have 21
// refs against 3 calls crossed, so as local-alloc pseudos they are profitable
// caller saves; the listing's r8 and r10 with r4 unused imply either fewer refs
// or pseudos local-alloc passes over (REG_N_DEATHS != 1). Separate result
// locals, delta locals and inline expressions did not lower them enough, and
// reusing a local for it would be a device. The offset and step swap is priority:
// equal refs (17), step lives 112 insns, offset 98.
// Both loops are goto: a real outer loop hoists the mask and the address (120).
// The pooled 31 comes from the (u16) cast's HImode AND, the inline 0x050000e4
// splits by itself, red = 31 then &= gives the movs in the loop, and declaring
// offset before left gained 6.
#include "types.h"

void wait(s32 frames);

void Func_02000734(u8 *from, u8 *to, s32 frames) {
  s32 step = 0;

  if (step <= frames) {
  next: {
    s32 offset = 0;
    s32 left = 4;

  loop: {
    s32 c, d, red, green, blue, toRed, toGreen, toBlue;

    c = *(u16 *)(from + offset);
    green = ((u16)c >> 5) & 31;
    red = 31;
    red &= c;
    d = *(u16 *)(to + offset);
    toGreen = ((u16)d >> 5) & 31;
    toRed = 31;
    toRed &= d;
    blue = ((u16)c >> 10) & 31;
    toBlue = ((u16)d >> 10) & 31;
    red += step * (toRed - red) / frames;
    green += step * (toGreen - green) / frames;
    blue += step * (toBlue - blue) / frames;
    *(u16 *)(0x050000e4 + offset) = blue << 10 | green << 5 | red;
    offset += 2;
    if (--left >= 0)
      goto loop;
  }
    wait(1);
    step++;
    if (step <= frames)
      goto next;
  }
  }
}

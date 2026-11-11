#include "yampi_desert_cave.h"
#include "system/io_reg.h"

extern u16 *gData_02001fcc;
extern u16 *gData_02001fdc;
extern u16 *gData_02001fe0;

/// Reads one colour's three channels, red first, stepping p past them. Its
/// once-only block orders the shifts as the game does; see the note at its use.
#define READ_COLOR(r, g, b, p)                                                 \
  do {                                                                         \
    (r) = *(p)++;                                                              \
    (g) = *(p)++;                                                              \
    (b) = *(p)++;                                                              \
  } while (0)

void Func_0200099c(void) {
  s32 step = 0;
  u16 *color;
  volatile u16 *dst;
  u32 i;
  s32 r, g, b;

  if (gData_02001fd0 != 0 && --gData_02001fd0 == 0) {
    step = 1;
    gData_02001fd0 = gData_02001fd4;
    if (gData_02001fd4 > 2)
      gData_02001fd4--;
  }
  if (!step)
    return;
  color = gData_02001fe0;
  i = 0;
  while (i < gData_02001fe4) {
    dst = &BG_PLTT[gData_02001fcc[gData_02001fe8 * gData_02001fe4 + i]];
    // READ_COLOR's block, at both uses, orders the reads as the game does; as
    // three plain reads, 6 of 120 instructions differ in tla-en.
    READ_COLOR(r, g, b, color);
    *dst = b << 10 | g << 5 | r;
    /* FAKEMATCH(dead): i++ in a once-only block; as a plain i++, 2 of 120
     * instructions differ in tla-en. */
    do {
      i++;
    } while (0);
  }
  dst = &BG_PLTT[gData_02001fd8[gData_02001fe4]];
  if (gData_02001fc8)
    color = gData_02001fdc + gData_02001fe4 * 3;
  else
    color = gData_02001fe0 + gData_02001fe4 * 3;
  READ_COLOR(r, g, b, color);
  *dst = b << 10 | g << 5 | r;
  gData_02001fc8 ^= 1;
  if (++gData_02001fe8 == gData_02001fe4)
    gData_02001fe8 = 0;
}

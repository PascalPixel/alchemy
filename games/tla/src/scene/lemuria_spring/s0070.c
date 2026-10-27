#include "types.h"
#include "ram_buffer.h"
#include "io_reg.h"

extern const u16 gData_02001678[];
void Func_02001408(s32 cue);
void WaitFrames(s32 frames);

static inline void RegSet(s32 value, s32 reg) {
  /* FAKEMATCH: the value passes through an s32 inline parameter: the native
   * builds 0x3f42 in a register (stored directly it is a pool word) and holds
   * the value in r2, the register address in r3. */
  *(u16 *)reg = value;
}

void Func_02000070(void) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  s32 *layer;
  s32 i;

  /* FAKEMATCH: the loop note ends the scheduling block: the native loads the
   * map work before it builds the cue (2 differ without it). */
  do {
  } while (0);
  Func_02001408(216);
  layer = (s32 *)(map + 0x178);
  i = 15;
next:
  layer[3] -= 0x10000;
  WaitFrames(4);
  i--;
  if (i >= 0)
    goto next;
  i = 0;
next2:
  RegSet(0x3f42, 0x04000050);
  RegSet(gData_02001678[i], 0x04000052);
  WaitFrames(8);
  i++;
  if (i <= 7)
    goto next2;
}

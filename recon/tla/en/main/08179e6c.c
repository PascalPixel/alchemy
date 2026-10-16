// Draft (tla-en): listing 74 instructions, draft 74, common 73, differ 2.
// After the call the native loads 0x784 from the pool before it starts
// BG2CNT's three-instruction address, and 0x4e before BG2PA's: sched2 ranks
// the pool load (priority 13) under the address chain (14), so plain stores
// put the address first (differ 14). The native's hoisted 7 sits in the
// delay slot of the canvas load before the call, so no loop note or
// volatile asm may stand between the call and the loop (a pinned value with
// a one-pass block before each store: differ 6). Here each value is pinned
// to r3 behind the empty asm that holds r2 (f18284c.c's device): both
// orders and the 7 match, but the two asms count eight estimated bytes and
// the literal pool's jump comes one instruction early (mov ip, r3 after
// it). With only BG2CNT's asm the pool is right and BG2PA's pair is
// swapped; with only BG2PA's, BG2CNT's (differ 2 each). A forced mask
// variable for the 7 (differ 41), a u16 local (built in a word: differ 30)
// and a one-pass block around the whole body (differ 8) do not help.
// The if/else-if order of the level (y < 32, y > 95, else 63) is the
// native's: jump optimisation turns it into its "level = y * 2" default.

#include "types.h"
#include "io_reg.h"
#include "ram_buffer.h"

void Func_0815b410(void);

void Func_08179e6c(void) {
  u8 *canvas = Ram_HeapSlots->battleFxWork[1];
  s32 y;
  s32 x;

  Func_0815b410();
  {
    register u16 control __asm__("r3") = 0x784;

    __asm__("" : "+l"(control) : : "r2");
    REG_BG2CNT = control;
  }
  PLTT_BACKDROP = 0;
  {
    register u16 scale __asm__("r3") = 0x4e;

    __asm__("" : "+l"(scale) : : "r2");
    REG_BG2PA = scale;
  }
  REG_BLDALPHA = 0x1010;
  REG_BLDCNT = 0x3f44;
  for (y = 0; y != 128; y++) {
    for (x = 0; x != 128; x++) {
      s32 level;

      if (y < 32)
        level = y * 2;
      else if (y > 95)
        level = 255 - y * 2;
      else
        level = 63;
      canvas[(((x / 8) * 16 + y / 8) * 8 + (x & 7)) * 8 + (y & 7)] = level;
    }
  }
}

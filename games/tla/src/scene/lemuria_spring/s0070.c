#include "types.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "event_inline.h"

extern const u16 gData_02001678[];

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


  Event_PlaySound(216);
  layer = (s32 *)(map + 0x178);
  i = 15;
next:
  layer[3] -= 0x10000;
  System_Wait(4);
  i--;
  if (i >= 0)
    goto next;
  i = 0;
next2:
  RegSet(BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL,
         (s32)&REG_BLDCNT);
  RegSet(gData_02001678[i], (s32)&REG_BLDALPHA);
  System_Wait(8);
  i++;
  if (i <= 7)
    goto next2;
}

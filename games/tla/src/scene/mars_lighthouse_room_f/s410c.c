#include "room.h"

void Func_02005640(void);

void Func_0200410c(void) {
  u8 *event = Ram_HeapSlots->eventWork;

  /* FAKEMATCH: ⚓️ loads the event work before it builds the offset */
  asm volatile("");
  if (S16_AT(event, 0xca2)) {
    Flag_Set(0x106);
    Func_02005608();
    Func_02005640();
    Flag_Clear(0x106);
  }
}

#include "types.h"
#include "ram_buffer.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)


void Func_0200042c(void) {
  FIELD_AT(((s32)Ram_HeapSlots->eventWork + 0x16a), u16, 0) = 0x20;
}

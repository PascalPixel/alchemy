#include "types.h"
#include "ram_buffer.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_02000830(void) {
  FIELD_AT(((s32)Ram_HeapSlots->effectWork + 0x34), u8, 0) = 1;
}

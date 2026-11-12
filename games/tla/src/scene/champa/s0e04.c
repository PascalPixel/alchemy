/// @file
/// Scene code of overlay 0x682, in ROM order, from s0e04's offset up to the next object the overlay links.

#include "types.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

s32 ObjectTable_ReadActiveValue();
s32 Window_CloseMatchingId();

void Func_02000e04(s32 a0, s32 a1) {
  s32 t0;
  s32 t1;

  t0 = ObjectTable_ReadActiveValue();
  t1 = ObjectTable_ReadActiveValue(a1);
  Window_CloseMatchingId(t0);
  Window_CloseMatchingId(t1);
}

#include "event_inline.h"

void Func_02000e24(void) { GameFlag_SetBit(0x970); }

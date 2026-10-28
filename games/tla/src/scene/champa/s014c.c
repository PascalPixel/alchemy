#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
s32 Func_02002b5c();
s32 Actor_GetById();

void Func_0200014c(void) {
  s32 t2;
  s32 t1;

  Call3(Func_02002b5c, 0x40, 0x1880000, 0x2380000);
  t1 = Value1(Actor_GetById, 0x40);
  FIELD_AT((t1 + 0x55), u8, 0) = 0;
  t2 = Value1(Actor_GetById, 0x40);
  FIELD_AT(t2, s32, 12) = 0x200000;
}

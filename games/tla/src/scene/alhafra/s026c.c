#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
s32 Func_02001d2c();
s32 Actor_GetById();
extern u8 gData_02001d54[];

void Func_0200026c(s32 a0, s32 a1) {
  s32 t1;

  Call1(Func_02001d2c, (s32)gData_02001d54);
  t1 = Value1(Actor_GetById, a1);
  FIELD_AT((t1 + 0x23), u8, 0) = 0;
}

#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
s32 Actor_GetById();
s32 Func_02003348();
extern u8 gMainRoutineWork[];

void Func_02001614(void) {
  s32 t0;
  s32 t1;

  t0 = Value1(Actor_GetById, FIELD_AT(gMainRoutineWork, s32, 532));
  t1 = Value3(Func_02003348, 2, FIELD_AT(t0, s32, 8), FIELD_AT(t0, s32, 16));
  if (t1 == 0xe8) {
    FIELD_AT((t0 + 0x22), u8, 0) = 2;
  }
}

#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
s32 Actor_GetById();
s32 Func_02003bc8();
s32 Func_02003c88();
s32 GameFlag_SetBit();
s32 GameFlag_ClearBit();
s32 Func_02003bd0();

void Func_020005ec(void) {
  s32 t0;

  t0 = Value1(Actor_GetById, 10);
  Func_02003bc8();
  Call1(Func_02003c88, 0);
  if ((FIELD_AT(t0, s32, 8) >> 20) == 0x1f) {
    Call1(GameFlag_SetBit, 0x310);
  } else {
    Call1(GameFlag_ClearBit, 0x310);
  }
  Func_02003bd0();
}

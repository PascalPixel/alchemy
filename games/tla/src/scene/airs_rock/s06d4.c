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
s32 Func_02003bd0();

void Func_020006d4(void) {
  /* FAKEMATCH(barrier): SPLIT_CONSTANT builds each constant as the game does, a
   * shifted byte plus an addend; as plain constants, 4 of 17 instructions
   * differ in tla-en. */
  s32 t0;

  t0 = Actor_GetById(13);
  Func_02003bc8();
  Func_02003c88(0);
  if ((FIELD_AT(t0, s32, 8) >> 19) == 0x17) {
    GameFlag_SetBit(SPLIT_CONSTANT(0x879));
  }
  Func_02003bd0();
}

#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
s32 GameFlag_Test();
s32 Func_02002a0c();
s32 Func_02002a04();

s32 Func_02000c04(void) {
  /* FAKEMATCH(barrier): SPLIT_CONSTANT builds each constant as the game does, a
   * shifted byte plus an addend; as plain constants, 4 of 27 instructions
   * differ in tla-en. */
  s32 t0;

  t0 = GameFlag_Test(SPLIT_CONSTANT(0x97f));
  if (t0 != 0) {
    Call6(Func_02002a0c, 0xb, 0x33, 5, 4, 0x10, 0x17);
    Func_02002a04(11, 51, 5, 6, 16, 23);
  }
  return 0;
}

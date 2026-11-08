#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
s32 Func_02000644();
s32 Func_02004634();
s32 Func_02004464();
s32 Func_020045ec();
s32 GameFlag_Test();
s32 Func_0200451c();
s32 Func_02004534();
s32 Func_0200446c();
extern u8 MsgProx135[];
extern u8 MsgProx56[];

void Func_02000674(void) {
  /* FAKEMATCH(barrier): SPLIT_CONSTANT builds each constant as the game does, a
   * shifted byte plus an addend; as plain constants, 8 of 27 instructions
   * differ in tla-en. */
  s32 t0;
  s32 t4;

  t0 = Func_02000644();
  if (t0 != 0) {
    Func_02004634(0xe, 0x16);
  } else {
    Func_02004464();
    Func_020045ec(0);
    t4 = GameFlag_Test(SPLIT_CONSTANT(0xa00, 61));
    if (t4 != 0) {
      Func_0200451c((s32)MsgProx135);
    } else {
      Func_0200451c((s32)MsgProx56);
    }
    Call2(Func_02004534, 22, 0);
    Func_0200446c();
  }
}

#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
s32 Actor_SetSpritePriority();
s32 Actor_GetById();
s32 Actor_SetPartAttribute();
s32 GameFlag_Test();
s32 Func_020044cc();
s32 Func_02004404();
s32 wait();
s32 Func_0200442c();

void Func_02002878(void) {
  /* FAKEMATCH(barrier): SPLIT_CONSTANT builds each constant as the game does, a
   * shifted byte plus an addend; as plain constants, 16 of 128 instructions
   * differ in tla-en. */
  s32 t9;
  s32 t3;
  s32 t4;
  s32 t6;
  s32 t8;
  s32 t11;
  s32 t16;
  s32 t20;

  Actor_SetSpritePriority(19, 1);
  Actor_SetPartAttribute(Actor_GetById(19), 0xf);
  t3 = GameFlag_Test(SPLIT_CONSTANT(0xa3d));
  if (t3 == 0) {
    t4 = GameFlag_Test(0x9c0);
    if (t4 == 0) {
      Func_020044cc(8, 0, 0);
    }
    t6 = GameFlag_Test(0x200);
    if (t6 != 0) {
      Call6(Func_02004404, 0x59, 0, 0x4d, 0x10, 1, 2);
    }
    t8 = GameFlag_Test(SPLIT_CONSTANT(0x9c1));
    if (t8 != 0) {
      t9 = Actor_GetById(10);
      FIELD_AT(t9, u16, 6) = 0xb000;
      wait(1);
    }
  }
  t11 = GameFlag_Test(SPLIT_CONSTANT(0xa3d));
  if (t11 != 0) {
    Call6(Func_02004404, 0x48, 3, 0x45, 3, 3, 3);
    Call6(Func_02004404, 0x30, 0x24, 0x45, 8, 3, 2);
    Call6(Func_0200442c, 0x48, 0xd, 1, 3, 0x46, 0x14);
    Func_020044cc(20, 0, 0);
  } else {
    t16 = GameFlag_Test(SPLIT_CONSTANT(0xa2d));
    if (t16 != 0) {
      Call6(Func_02004404, 0x48, 3, 0x45, 3, 3, 3);
      Call6(Func_02004404, 0x30, 0x24, 0x45, 8, 3, 2);
      Call6(Func_0200442c, 0x48, 0xd, 1, 3, 0x46, 0x14);
      t20 = GameFlag_Test(0x40);
      if (t20 == 0) {
        Func_020044cc(20, 0, 0);
      }
    }
  }
}

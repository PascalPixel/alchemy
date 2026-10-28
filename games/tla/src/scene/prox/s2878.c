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
s32 WaitFrames();
s32 Func_0200442c();

void Func_02002878(void) {
  s32 t9;
  s32 t3;
  s32 t4;
  s32 t6;
  s32 t8;
  s32 t11;
  s32 t16;
  s32 t20;

  Call2(Actor_SetSpritePriority, 0x13, 1);
  Call2(Actor_SetPartAttribute, Value1(Actor_GetById, 0x13), 0xf);
  t3 = Value1(GameFlag_Test, 0xa3d);
  if (t3 == 0) {
    t4 = Value1(GameFlag_Test, 0x9c0);
    if (t4 == 0) {
      Call3(Func_020044cc, 8, 0, 0);
    }
    t6 = Value1(GameFlag_Test, 0x200);
    if (t6 != 0) {
      Call6(Func_02004404, 0x59, 0, 0x4d, 0x10, 1, 2);
    }
    t8 = Value1(GameFlag_Test, 0x9c1);
    if (t8 != 0) {
      t9 = Value1(Actor_GetById, 0xa);
      FIELD_AT(t9, u16, 6) = 0xb000;
      Call1(WaitFrames, 1);
    }
  }
  t11 = Value1(GameFlag_Test, 0xa3d);
  if (t11 != 0) {
    Call6(Func_02004404, 0x48, 3, 0x45, 3, 3, 3);
    Call6(Func_02004404, 0x30, 0x24, 0x45, 8, 3, 2);
    Call6(Func_0200442c, 0x48, 0xd, 1, 3, 0x46, 0x14);
    Call3(Func_020044cc, 0x14, 0, 0);
  } else {
    t16 = Value1(GameFlag_Test, 0xa2d);
    if (t16 != 0) {
      Call6(Func_02004404, 0x48, 3, 0x45, 3, 3, 3);
      Call6(Func_02004404, 0x30, 0x24, 0x45, 8, 3, 2);
      Call6(Func_0200442c, 0x48, 0xd, 1, 3, 0x46, 0x14);
      t20 = Value1(GameFlag_Test, 0x40);
      if (t20 == 0) {
        Call3(Func_020044cc, 0x14, 0, 0);
      }
    }
  }
}
